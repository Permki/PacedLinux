/*
 * Copyright (C) 2014,  Netronome Systems, Inc.  All rights reserved.
 *
 * @file          lib/dcfl/libdcfl.c
 * @brief         DCFL library for MicroC
 */

#ifndef _DCFL__LIBDCFL_C_
#define _DCFL__LIBDCFL_C_

#include <assert.h>
#include <nfp.h>
#include <nfp/me.h>
#include <stdint.h>
#include <types.h>

#ifdef DCFL_PIPELINED
#include <nfp/mem_ring.h>
#include <std/reg_utils.h>
#endif

#include <dcfl.h>
#include <dcfl_ctrl.h>

__export __emem __align64 unsigned long long
    DCFL_LKP_OVERFLOW_CNTR = 0;
__export __emem __align64 unsigned long long
    DCFL_CROSS_OVERFLOW_CNTR = 0;

#include "_c/field_lkp.c"
#include "_c/an.c"

#if defined(DCFL_DEBUG) || defined(DCFL_DEBUG_HITS)
#include <nfp/mem_ring.h>
MEM_JOURNAL_DECLARE(DCFL_DBGJRNL_NAME, DCFL_DBGJRNL_SIZE);
#endif

/**
 * ME declarations:
 */

__export __imem struct dcfl_me_config dcfl_me_cfg;

#define ACTIVE_BUFFER_EMPTY             0
#define ACTIVE_BUFFER_USE_FIRST_HALF    1
#define ACTIVE_BUFFER_USE_SECOND_HALF   2

#ifdef DCFL_PIPELINED
MEM_WORKQ_DECLARE(DCFL_ANQ_NAME, DCFL_ANQ_ENTRIES);
#endif

__addr40 __imem uint8_t
*dcfl_get_conf(uint32_t table,
               __xread void *custom_cfg)
{
    __xread uint32_t active_buffer;
    __addr40 __imem uint8_t *cfg_ptr;

    /* read the active buffer */
    cfg_ptr = (__addr40 __imem uint8_t *) &dcfl_me_cfg;
    cfg_ptr += sizeof(struct dcfl_me_table_config_buffers) * table;
    mem_read32(&active_buffer, cfg_ptr, sizeof(uint32_t));

    if (active_buffer == ACTIVE_BUFFER_EMPTY)
        return 0;
    __critical_path();

    /* move the pointer beyond the active_buffer word */
    cfg_ptr += sizeof(active_buffer);

    /* move the pointer to the correct buffer instance */
    if (active_buffer == ACTIVE_BUFFER_USE_SECOND_HALF) {
        cfg_ptr += sizeof(struct dcfl_me_table_config);
    }

#ifdef DCFL_CUSTOM_TABLE_LW
    /* read the dcfl table config */
    mem_read32((void *)custom_cfg, cfg_ptr,
               sizeof(struct dcfl_me_custom_config));
    cfg_ptr += sizeof(struct dcfl_me_custom_config);
#endif

    return cfg_ptr;
}


/**
 * Bubble sort results.
 *
 * Comparing the whole 32 bits means that results are first
 * sorted by priority (increasing), then by rule_id
 * (increasing). */
int
dcfl_sort_results(__lmem struct dcfl_label_set *results,
                  uint32_t nresults)
{
    __gpr uint32_t sorted = 0;
    __gpr uint32_t i, temp;

    while (sorted == 0) {
        sorted = 1;
        for (i = 0; i < nresults - 1; i++) {
            if (results->values[i] >
                results->values[i + 1]) {
                /* Comparing the whole 32 bits means that results are first
                    * sorted by priority (increasing), then by rule_id
                    * (increasing). */
                temp = results->values[i];
                results->values[i] = results->values[i + 1];
                results->values[i + 1] = temp;
                sorted = 0;
            }
        }
    }

    for (i = 0; i < nresults; i++) {
        results->labels[i] = results->results[i].rule_id;
    }

    return nresults;
}


#ifdef DCFL_PIPELINED
/**
 * Get island, ME nr and context number of this thread, and pack into
 * structure along with the signal that is to be set when DCFL worker
 * completes.
 *
 * @param sig_num   Signal number declared as volatile in this ME, to be
 *                  set with an interthread_signal call.
 */
struct dcfl_thread_info
dcfl_get_thread_info(int sig_num)
{
    struct dcfl_thread_info info;

    uint32_t sts = local_csr_read(local_csr_active_ctx_sts);
    info.isl_id = (sts >> 25) & 0x3f;
    info.me = (sts >> 3) & 0xf;
    info.ctx = sts & 0x7;
    info.sig = sig_num;

    return info;
}


/**
 * Write labels to a label buffer symbol.
 *
 * @param labels        Pointer to label set containing dcfl results
 * @param nrules        Number of results in label set
 * @param dest          Destination address of results
 */
static void
dcfl_write_labels(__lmem struct dcfl_label_set *labels, uint32_t nrules,
               __addr40 __mem uint8_t *dest)
{
    __xwrite uint32_t write_buf[DCFL_WRITE_BUF_LW];
    uint32_t offset = 0;
    __lmem struct dcfl_label_set *src;

    src = labels;
    for (; offset < (nrules * sizeof(uint16_t)); offset += sizeof(write_buf)) {
        reg_cp(write_buf, src, sizeof(write_buf));
        mem_write32(write_buf, (__addr40 __mem uint8_t *)dest + offset,
                    sizeof(write_buf));
        src += sizeof(write_buf);
    }
}


static void
dcfl_read_labels_to_lmem(__lmem uint32_t *labelset,
                         __addr40 __mem struct dcfl_label_set *src,
                         int nlabels
                          )
{
    __xread uint32_t read_buf[DCFL_LABELSET_LW];
    unsigned int i, lmoff = 0;

    mem_read32(read_buf, (__addr40 __mem uint32_t *)src,
                sizeof(read_buf));

    if (nlabels > DCFL_MAX_LABELS_PER_LKP) {
        nlabels = DCFL_MAX_LABELS_PER_LKP;
    }

    for (i = 0; i < ((nlabels + 1)/2); i++) {
        labelset[lmoff] = read_buf[i];
        lmoff += 1;
    }

    if (nlabels % 2) {
        labelset[lmoff] &= 0xFFFF0000;
    }
}


/**
 * Read child worker thread_info from thread_info_ptr,
 * and raise interthread_signal for child to continue.
 */
static void
dcfl_signal_an_worker(__addr40 __mem uint32_t *worker_thread_info) {
    __gpr uint32_t ithread_sig;
    __xread struct dcfl_thread_info read_buf[1];
    do {
        mem_read32(read_buf, worker_thread_info, sizeof(read_buf));
    } while (read_buf[0].value == 0);
    ithread_sig = read_buf[0].value & DCFL_ITHREAD_MASK;
    __asm { ct[interthread_signal, --, ithread_sig, 0, --] }
}


/*
 * Public methods of MicroC API:
 */

void
dcfl_pipeline_stage(struct dcfl_an_workq_entry *anq_entry,
                    mem_ring_addr_t anq_raddr)
{
    __xread struct dcfl_me_table_config_small tblcfg;
    __xread struct dcfl_me_field_config *current_f_cfg;
    __xwrite uint32_t anq_xfer[DCFL_AN_WORKQ_LW];
    __xwrite uint32_t write_buf[1];
    __xread uint32_t read_buf[1];
    struct dcfl_an_workq_entry next_anq_entry;
    __addr40 __imem uint32_t worker_thread_info = 0;
    __gpr unsigned int valid_val = 0;
    __gpr unsigned int labelset_idx, nlabels, prev_nlabels, nmatches = 0;
    __gpr unsigned int next_field_idx;
    __addr40 __mem uint8_t *cfg_ptr;
    __xrw uint32_t field_value[4];
    __lmem struct dcfl_label_set label_set[3];
    volatile SIGNAL parent_sig;

#ifdef DCFL_P4_STRICT_HDR_VALID
    __gpr unsigned int hdr_valid = 1;
    __gpr unsigned int hdr_valid_val;
#endif

#ifdef DCFL_DEBUG
    __xwrite uint32_t jrnl_buf[DCFL_DBGJRNL_BUFLEN];
    mem_ring_addr_t jrnl_raddr = mem_ring_get_addr(&DCFL_DBGJRNL_NAME);
#endif

    cfg_ptr = anq_entry->cfg_ptr;

    mem_read32(&tblcfg, (__imem void *) cfg_ptr, sizeof(tblcfg));

    if (tblcfg.num_fields == 0)
        goto dcfl_pipeline_complete;
    __critical_path();

    /* Pipeline next stage so long. Have it wait for inputs to be ready */
    if (tblcfg.num_fields > 2) {
        if (anq_entry->field_idx == 0) {
            next_field_idx = 2;
        } else {
            next_field_idx = anq_entry->field_idx + 1;
        }
        if ((tblcfg.num_fields - next_field_idx) > 0) {
            /* Setup work for next stage in pipeline */
            next_anq_entry.field_idx = next_field_idx;
            next_anq_entry.key_ptr = anq_entry->key_ptr;
            next_anq_entry.cfg_ptr = anq_entry->cfg_ptr;
            next_anq_entry.labelset_ptr = anq_entry->labelset_ptr;
            next_anq_entry.nlabels_ptr = anq_entry->nlabels_ptr;
            next_anq_entry.app_thread_info = anq_entry->app_thread_info;
            next_anq_entry.thread_info_ptr = &worker_thread_info;
            next_anq_entry.labelset_idx = 1 - anq_entry->labelset_idx;

            reg_cp(anq_xfer, (void *)&next_anq_entry, sizeof(anq_xfer));
            mem_workq_add_work(DCFL_AN_QNUM, anq_raddr, &anq_xfer,
                            sizeof(anq_xfer));
        }
    }

    /* move cfg_ptr past num_fields to the start of the field configs */
    cfg_ptr += sizeof(tblcfg.num_fields);

dcfl_next_field:
    if (anq_entry->field_idx > 0) {
        cfg_ptr += anq_entry->field_idx * sizeof(struct dcfl_me_field_config);
        mem_read32(&tblcfg.f_cfg[0], cfg_ptr,
                sizeof(struct dcfl_me_field_config));
    }
    current_f_cfg = &tblcfg.f_cfg[0];

    /* 2a) get field value from the key */
    dcfl_get_field_value_from_mem(&valid_val, &field_value[0],
                                    anq_entry->key_ptr, current_f_cfg);
    reg_cp(field_value, field_value, sizeof(field_value));

#ifdef DCFL_P4_STRICT_HDR_VALID
    if (current_f_cfg->hdr_valid_bitlen > 0) {
        /* Check header validity */
        dcfl_get_hdr_valid_val_from_mem(&hdr_valid_val, anq_entry->key_ptr,
                                        current_f_cfg);
        hdr_valid = (hdr_valid_val == current_f_cfg->valid_value);
    }
#endif

    /* 2b) field lookup */

    /* for the first field_id we put the data in the intermediate buffer
     * this is because no AN calc happens for the first field
     */
    labelset_idx = 2;
    if ((tblcfg.num_fields > 1) && (anq_entry->field_idx == 0)) {
        labelset_idx = 0;
    }

#ifdef DCFL_P4_STRICT_HDR_VALID
    nlabels =
        dcfl_field_lookup(&(label_set[labelset_idx]), valid_val,
                            field_value, current_f_cfg, hdr_valid);
#else
    nlabels =
        dcfl_field_lookup(&(label_set[labelset_idx]), valid_val,
                            field_value, current_f_cfg, 1);
#endif

#ifdef DCFL_DEBUG
    jrnl_buf[0] = ((valid_val & 0xFFFF) << 16) + nlabels;
    jrnl_buf[1] = label_set[labelset_idx].values[0];
    mem_ring_journal(DCFL_DBGJRNL_QNUM, jrnl_raddr, jrnl_buf,
                        sizeof(jrnl_buf));
#endif

    if (nlabels == 0) {
        write_buf[0] = 0;
        mem_write32(write_buf, anq_entry->nlabels_ptr, sizeof(write_buf));
        if (anq_entry->field_idx > 0) {
            dcfl_signal_an_worker(&worker_thread_info);
        }
        goto dcfl_pipeline_complete;
    }

    /* No AN work on first field */
    if (tblcfg.num_fields > 1) {
        if (anq_entry->field_idx == 0) {
            anq_entry->field_idx++;
            prev_nlabels = nlabels;
            goto dcfl_next_field;
        }
    }

    /* we ping-pong the set id for output set and intermediate set,
     * lookup set is fixed at 2
     */
    labelset_idx = 1 - anq_entry->labelset_idx;

    /* 2c) AN 'crossproduct */
    if (tblcfg.num_fields == 1) {
        /* single field AN: init a label_set with one label of value 0 */
        prev_nlabels = 1;
        (label_set+(anq_entry->labelset_idx))->labels[0] = 0;
    } else {
        /* Read "foreign" labelset from dcfl_label_buffer into LM */
        if (anq_entry->field_idx > 1) {
            /* Child worker, assign signal for parent to assert */
            __assign_relative_register(&parent_sig, DCFL_STAGE_SIG);
            {
                /* Write thread info into thread_info_ptr,
                * so that parent can signal child when inputs are ready */
                __xwrite struct dcfl_thread_info write_buf[1];
                struct dcfl_thread_info thread_info;
                write_buf[0] = dcfl_get_thread_info(DCFL_STAGE_SIG);
                mem_write32(write_buf, anq_entry->thread_info_ptr,
                            sizeof(write_buf));
            }
            wait_for_all(&parent_sig);
            mem_read32(read_buf, anq_entry->nlabels_ptr, sizeof(read_buf));
            if (read_buf[0] == 0) {
                dcfl_signal_an_worker(&worker_thread_info);
                return;
            }

            prev_nlabels = read_buf[0];
            dcfl_read_labels_to_lmem((__lmem uint32_t *)&label_set[anq_entry
                                ->labelset_idx],
                            (__addr40 __mem struct dcfl_label_set *)
                                anq_entry->labelset_ptr,
                            prev_nlabels);
        }
    }

    if (tblcfg.num_fields - anq_entry->field_idx - 1) {
        nmatches = dcfl_an_lkp(&(label_set[anq_entry->labelset_idx]),
                prev_nlabels,
                &(label_set[2]),
                nlabels,
                &(label_set[labelset_idx]),
                current_f_cfg->an_stage_base,
                current_f_cfg->an_stage_base_hi,
                current_f_cfg->an_alut_loc_reg);
    } else {
        nmatches = dcfl_an_final(&(label_set[anq_entry->labelset_idx]),
                prev_nlabels,
                &(label_set[2]),
                nlabels,
                &(label_set[labelset_idx]),
                current_f_cfg->an_stage_base,
                current_f_cfg->an_stage_base_hi);
    }

    /* Store number of matches in DRAM */
    write_buf[0] = nmatches;
    mem_write32(write_buf, anq_entry->nlabels_ptr, sizeof(write_buf));

    if (nmatches == 0) {
        /* signal child worker to complete and be freed */
        dcfl_signal_an_worker(&worker_thread_info);
        goto dcfl_pipeline_complete;
    }

    if ((tblcfg.num_fields - anq_entry->field_idx) == 1) {
        /* If this is the final stage, possibly sort results,
         * then fall through to dcfl_pipeline_complete */
#if (defined(DCFL_PRIORITY_SORT) && !defined(DCFL_SUPPORT_MATCH_FILTERING))
        dcfl_sort_results(&(label_set[labelset_idx]),
                            nmatches);
#endif
        dcfl_write_labels(&label_set[labelset_idx], DCFL_MAX_LABELS_PER_LKP,
                        (__addr40 __mem uint8_t *)anq_entry->labelset_ptr);
    } else {
        dcfl_write_labels(&label_set[labelset_idx], DCFL_MAX_LABELS_PER_LKP,
                        (__addr40 __mem uint8_t *)anq_entry->labelset_ptr);
        dcfl_signal_an_worker(&worker_thread_info);
        return;
    }

dcfl_pipeline_complete:
    local_csr_write(local_csr_mailbox_3, nmatches);
    {
        uint32_t ithread_sig;

        /* Signal app thread of completion */
        ithread_sig = anq_entry->app_thread_info.value & DCFL_ITHREAD_MASK;
        __asm { ct[interthread_signal, --, ithread_sig, 0, --] }
    }
}

uint32_t
dcfl_classify_pipeline(__addr40 __mem uint8_t *key, uint32_t table,
              __addr40 __mem struct dcfl_label_set *label_set,
              __addr40 __mem uint32_t *nlabels,
              __addr40 __imem uint8_t *cfg,
              mem_ring_addr_t anq_raddr
             )
{
    __xread uint32_t read_buf[1];
    struct dcfl_an_workq_entry anq_entry;
    volatile SIGNAL pipeline_sig;

    if (table >= DCFL_MAX_NUM_TABLES)
        return 0;
    __critical_path();

#if DCFL_ME_TABLE_CONFIG_SMALL_NUM_FIELDS != 1
    /* The code below expects the field prefetch to be 1 */
    #error DCFL_ME_TABLE_CONFIG_SMALL_NUM_FIELDS must be 1
#endif

    anq_entry.field_idx = 0;
    anq_entry.key_ptr = key;
    anq_entry.cfg_ptr = cfg;
    anq_entry.labelset_ptr = &label_set[0];
    anq_entry.nlabels_ptr = nlabels;
    anq_entry.labelset_idx = 0;
    anq_entry.app_thread_info = dcfl_get_thread_info(
        __signal_number(&pipeline_sig));

    /* Put first stage in pipeline for lookup and crossproducting.
     * This stage will kick off all further stages. */
    dcfl_pipeline_stage(&anq_entry, anq_raddr);

    wait_for_all(&pipeline_sig);

    mem_read32(read_buf, nlabels, sizeof(read_buf));

    return read_buf[0];
}
#endif


uint32_t
dcfl_classify(void *key, uint32_t table,
              __lmem struct dcfl_label_set *label_set,
              uint32_t *labels_id, __addr40 __imem uint8_t *cfg)
{
    __xread struct dcfl_me_table_config_small tblcfg;
    __xread struct dcfl_me_field_config *current_f_cfg;
    __addr40 __imem uint8_t *cfg_ptr = cfg;
    __gpr uint32_t field_id;
    __gpr uint32_t buffer;
    __lmem uint32_t nlabels[3];
    __lmem uint32_t *localkey = key;
    /* which label set to use as output */
    __gpr uint8_t id_out;

#ifdef DCFL_P4_STRICT_HDR_VALID
    __gpr unsigned int hdr_valid = 1;
    __gpr unsigned int hdr_valid_val;
#endif

#if defined(DCFL_DEBUG) || defined(DCFL_DEBUG_HITS)
    __xwrite uint32_t jrnl_buf[DCFL_DBGJRNL_BUFLEN];
    mem_ring_addr_t jrnl_raddr = mem_ring_get_addr(&DCFL_DBGJRNL_NAME);
    __gpr uint32_t dbg_word[2] = {0,0};
#endif

    if (table >= DCFL_MAX_NUM_TABLES)
        return 0;
    __critical_path();

#if DCFL_ME_TABLE_CONFIG_SMALL_NUM_FIELDS != 1
    /* The code below expects the field prefetch to be 1 */
    #error DCFL_ME_TABLE_CONFIG_SMALL_NUM_FIELDS must be 1
#endif

    mem_read32(&tblcfg, (__imem void *) cfg_ptr, sizeof(tblcfg));

    if (tblcfg.num_fields == 0)
        return 0;
    __critical_path();

    /* point cfg_ptr to the first field entry */
    cfg_ptr += sizeof(tblcfg.num_fields);

    /* 2) Start the DCFL classification process*/
    for (field_id = 0; field_id < tblcfg.num_fields; field_id++)
    {
        __gpr unsigned int lkup_set;
        __gpr unsigned int valid_val = 0;
        __xrw uint32_t field_value[4];
        /* We grab one field at a time */

        if (field_id >= 1) { /* the first one is already in the buffer */
            mem_read32(&tblcfg.f_cfg[0], cfg_ptr,
                    sizeof(struct dcfl_me_field_config));
        }
        cfg_ptr += sizeof(struct dcfl_me_field_config);
        current_f_cfg = &tblcfg.f_cfg[0];

        /* 2a) get field value from the key */
        dcfl_get_field_value(&valid_val, &field_value[0], localkey,
                             current_f_cfg);

#ifdef DCFL_DEBUG_HITS
        if (field_id < 4) {
            dbg_word[0] |= (valid_val & 0xFF) << (8 * (3 - field_id));
        } else if (field_id < 8) {
            dbg_word[1] |= (valid_val & 0xFF) << (8 * (7 - field_id));
        }
#endif

#ifdef DCFL_P4_STRICT_HDR_VALID
        if (current_f_cfg->hdr_valid_bitlen > 0) {
            /* Check header validity */
            dcfl_get_hdr_valid_val(&hdr_valid_val, localkey, current_f_cfg);
            hdr_valid = (hdr_valid_val == current_f_cfg->valid_value);
        }
#endif

        /* 2b) field lookup */

        /* for the first field_id we put the data in the intermediate buffer
         * this is because no AN calc happens for the first field
         */
        lkup_set = 2;
        if ((tblcfg.num_fields > 1) && (field_id == 0))
            lkup_set = 0;

#ifdef DCFL_P4_STRICT_HDR_VALID
        nlabels[lkup_set] =
            dcfl_field_lookup(&(label_set[lkup_set]), valid_val,
                              field_value, current_f_cfg, hdr_valid);
#else
        nlabels[lkup_set] =
            dcfl_field_lookup(&(label_set[lkup_set]), valid_val,
                              field_value, current_f_cfg, 1);
#endif

#ifdef DCFL_DEBUG
        jrnl_buf[0] = ((valid_val & 0xFFFF) << 16) + nlabels[lkup_set];
        jrnl_buf[1] = label_set[lkup_set].values[0];
        mem_ring_journal(DCFL_DBGJRNL_QNUM, jrnl_raddr, jrnl_buf,
                            sizeof(jrnl_buf));
#endif
        /* set the output buffer, used in field count is 1 and lookup miss */
        id_out = lkup_set;

        if (nlabels[lkup_set] == 0)
            break;

#ifdef DCFL_NO_AN
        goto dcfl_skip_an;
#endif

        /* No AN work on first field */
        if ((tblcfg.num_fields > 1) && (field_id == 0))
            continue;

        /* 2c) AN 'crossproduct */

        /* we ping-pong the set id for output set and intermediate set,
         * lookup set is fixed at 2
         */
        id_out = field_id & 0x1;

        if (tblcfg.num_fields == 1) {
            /* single field AN: init a label_set with one label of value 0 */
            nlabels[(id_out + 1) & 0x1] = 1;
            (label_set+((id_out + 1) & 0x1))->labels[0] = 0;
        }

        if (tblcfg.num_fields - field_id - 1) {
            nlabels[id_out] = dcfl_an_lkp(&(label_set[(id_out + 1) & 0x1]),
                    nlabels[(id_out + 1) & 0x1],
                    &(label_set[2]),
                    nlabels[2],
                    &(label_set[id_out]),
                    current_f_cfg->an_stage_base,
                    current_f_cfg->an_stage_base_hi,
                    current_f_cfg->an_alut_loc_reg);
        } else {
            nlabels[id_out] =
                dcfl_an_final(&(label_set[(id_out + 1) & 0x1]),
                        nlabels[(id_out + 1) & 0x1],
                        &(label_set[2]),
                        nlabels[2],
                        &(label_set[id_out]),
                        current_f_cfg->an_stage_base,
                        current_f_cfg->an_stage_base_hi);
        }

        if (nlabels[id_out] == 0)
            break;
    } /* for (field_id=0 */

#if (defined(DCFL_PRIORITY_SORT) && !defined(DCFL_SUPPORT_MATCH_FILTERING))
    dcfl_sort_results(&(label_set[id_out]),
                      nlabels[id_out]
    );
#endif

#ifdef DCFL_DEBUG_HITS
    jrnl_buf[0] = ((table & 0xFFFF) << 16) + nlabels[id_out];
    jrnl_buf[1] = label_set[id_out].values[0];
    jrnl_buf[2] = dbg_word[0];
    jrnl_buf[3] = dbg_word[1];
    mem_ring_journal(DCFL_DBGJRNL_QNUM, jrnl_raddr, jrnl_buf,
                        sizeof(jrnl_buf));
#endif
dcfl_skip_an:

    *labels_id = id_out;
    return nlabels[id_out];
}

#endif /* !_DCFL__LIBDCFL_C_ */
