/* Copyright (C) 2015,  Netronome Systems, Inc.  All rights reserved. */

#include <nfp.h>
#include <nfp6000/nfp_me.h>

#ifdef DCFL_SPLIT_ARCH
#include <nfp/mem_ring.h>
#include <std/reg_utils.h>
#include <nfp/xpb.h>
#endif

#include <pif_common.h>

#include <dcfl.h>

//#define DESPERATE_DEBUG

#ifndef DCFL_CUSTOM_TABLE_LW != 4
#error DCFL_CUSTOM_TABLE_LW must be defined as 4
#endif

#ifdef DCFL_SPLIT_ARCH
static __shared mem_ring_addr_t wq_raddr;
MEM_WORKQ_DECLARE(DCFL_KEYQ_NAME, DCFL_KEYQ_ENTRIES);

/**
 * Initialise work queue and possible results journals.
 */
void
pif_lookup_dcfl_init()
{
    wq_raddr = mem_workq_setup(DCFL_KEYQ_NUM,
                               (__mem __addr40 void *)DCFL_KEYQ_NAME,
                               sizeof(DCFL_KEYQ_NAME));
}

/**
 * Get island, ME nr and context number of this thread, and pack into
 * structure along with the signal that is to be set when DCFL worker
 * completes.
 *
 * @param sig_num   Signal number declared as volatile in this ME, to be
 *                  set with an interthread_signal call.
 */
__forceinline
static struct dcfl_thread_info
pif_lookup_get_thread_info(int sig_num)
{
    struct dcfl_thread_info info;

    uint32_t sts = local_csr_read(local_csr_active_ctx_sts);
    info.isl_id = (sts >> 25) & 0x3f;
    info.me = (sts >> 3) & 0xf;
    info.ctx = sts & 0x3;
    info.sig = sig_num;

    return info;
}

__forceinline
int
pif_lookup_store_key(__mem __addr40 uint64_t *key_store, __lmem uint32_t *parrep)
{
    SIGNAL sig;
    __xwrite uint32_t write_xfer[8];
    unsigned int i, tocopy, chunk, offset = 0, lmoff = 0;

    tocopy = PIF_PARREP_LEN_LW*4;

    while (tocopy > 0) {
        if (tocopy > 8*4)
            chunk = 8*4;
        else
            chunk = tocopy;

        for (i = 0; i < (chunk + 3)/4; i++) {
            write_xfer[i] = parrep[lmoff];
            lmoff += 1;
        }

        __mem_write8(write_xfer,
                     key_store + offset,
                     chunk, 8*4, ctx_swap, &sig);

        tocopy -= chunk;
        offset += chunk;
    }

    return 0;
}
#endif

struct pif_lookup_result
pif_lookup(unsigned int tableno,
           __lmem uint32_t *parrep,
           __mem __addr40 uint32_t *action_buffer,
           uint32_t action_buffer_offset)
{
    /* Memory for DCFL label sets */
    __lmem struct dcfl_label_set label_set[3];
    __xread struct pif_actiondata_table_desc d;
    struct pif_lookup_result result;
    uint32_t rule = 0;
#ifdef DCFL_SPLIT_ARCH
    __xread uint32_t rule_read[1];
    __emem unsigned long long dcfl_key[PIF_LOOKUP_KEY_SIZE_LW];
    __emem unsigned long long dcfl_result;
    __xwrite uint32_t work_xfer[DCFL_KEY_WORKQ_LW];
    struct dcfl_key_workq_entry work_entry;
    volatile SIGNAL dcfl_worker_sig;
#else
    __mem __addr40 __imem uint8_t *cfg_ptr;
#endif

    result.action_id = -1;
    result.action_len = 0;

    /* sanity check */
    if (tableno >= PIF_TABLE_COUNT) {
#ifdef DESPERATE_DEBUG
        local_csr_write(local_csr_mailbox0, 0x1);
        local_csr_write(local_csr_mailbox1, tableno);
        local_csr_write(local_csr_mailbox2, PIF_TABLE_COUNT);
        local_csr_write(local_csr_mailbox3, 0);
        __asm ctx_arb[bpt];
#endif
        return result;
    }

    /* get the information about the table */
#ifdef DCFL_SPLIT_ARCH
    work_entry.cfg_ptr = dcfl_get_conf(tableno, (void *)&d);
    if (work_entry.cfg_ptr == 0) { /* no data loaded */
#else
    cfg_ptr = dcfl_get_conf(tableno, (void *)&d);
    if (cfg_ptr == 0) { /* no data loaded */
#endif
#ifdef DESPERATE_DEBUG
        local_csr_write(local_csr_mailbox0, 0x11);
        local_csr_write(local_csr_mailbox1, tableno);
        local_csr_write(local_csr_mailbox2, PIF_TABLE_COUNT);
        local_csr_write(local_csr_mailbox3, 0);
        __asm ctx_arb[bpt];
#endif
        return result;
    }

#ifdef DESPERATE_DEBUG
    local_csr_write(local_csr_mailbox0, 0x2222);
    local_csr_write(local_csr_mailbox1, d.flags | (d.rule_count << 16));
    local_csr_write(local_csr_mailbox2, (((uint64_t)d.table_address) >> 32) & 0xffffffffULL);
    local_csr_write(local_csr_mailbox3, ((uint64_t)d.table_address) & 0xffffffffULL);
    __asm ctx_arb[bpt];
#endif

    if (!(d.flags & PIF_ACTIONDATA_FLAG_VALID)) {
#ifdef DESPERATE_DEBUG
        local_csr_write(local_csr_mailbox0, 0x2);
        local_csr_write(local_csr_mailbox1, (d.flags << 16) | tableno);
        local_csr_write(local_csr_mailbox2, d.rule_count);
        local_csr_write(local_csr_mailbox3, ((uint64_t)d.table_address) & 0xffffffff);
        __asm ctx_arb[bpt];
#endif
        return result;
    }

    if (d.rule_count > 0) {
#ifdef DCFL_SPLIT_ARCH
        work_entry.thread_info = pif_lookup_get_thread_info(
            __signal_number(&dcfl_worker_sig));
        work_entry.thread_info.test_ct_num = tableno;
        work_entry.size = PIF_LOOKUP_KEY_SIZE_LW * sizeof(uint32_t);
        work_entry.key_addr = (uint64_t)&dcfl_key;
        work_entry.result_addr = (uint64_t)&dcfl_result;

        pif_lookup_store_key(&dcfl_key[0], parrep);

        reg_cp(work_xfer, (void *)&work_entry, sizeof(work_xfer));

        /* pass dcfl mngr info and key address to workq */
        mem_workq_add_work(DCFL_KEYQ_NUM, wq_raddr, &work_xfer,
                        sizeof(work_xfer));

        wait_for_all(&dcfl_worker_sig);

        mem_read32(rule_read, (__mem __addr40 uint64_t *)work_entry.result_addr,
                   sizeof(rule_read));

        if (rule_read[0] != PIF_LOOKUP_MISS_RULE) {
            rule = rule_read[0];
        }
#else
        int32_t label_set_id = 0;
        rule = dcfl_classify(parrep, tableno, label_set, &label_set_id, cfg_ptr);

        if (rule != 0) { /* hit */
            rule = label_set[label_set_id].labels[0];
        }
#endif
    }

    if (rule == 0 && !(d.flags & PIF_ACTIONDATA_FLAG_HASDEFAULT)) {
#ifdef DESPERATE_DEBUG
        local_csr_write(local_csr_mailbox0, 0x3);
        local_csr_write(local_csr_mailbox1, d.flags);
        local_csr_write(local_csr_mailbox2, rule);
        local_csr_write(local_csr_mailbox3, 0);
        __asm ctx_arb[bpt];
#endif
        return result;
    }

    if (rule > d.rule_count) {
#ifdef DESPERATE_DEBUG
        local_csr_write(local_csr_mailbox0, 0x4);
        local_csr_write(local_csr_mailbox1, d.flags);
        local_csr_write(local_csr_mailbox2, rule);
        local_csr_write(local_csr_mailbox3, d.rule_count);
        __asm ctx_arb[bpt];
#endif
        return result;
    }

    __critical_path();

    /* read the action data from the table and write it to the action buffer */
    {
        SIGNAL sig;
        __mem __addr40 uint32_t *ruledata;
        __xread uint32_t rd_buf[PIF_ACTION_OPDATA_MAX_LW];
        __xwrite uint32_t wr_buf[PIF_ACTION_OPDATA_MAX_LW];
        __xread uint32_t *rd_buf_ptr = &rd_buf[0];
        __xread union pif_action_opdata *act_op;
        int action_id;
        int i;

        /* TODO: prefetch ? */

        /* grab the opcode
         * note to get the offset we apply a cheap shift rather than an
         * expensive multiply
         */
        ruledata = ((__mem __addr40 uint32_t *)d.table_address) + (rule << d.record_shift);
#ifdef DESPERATE_DEBUG
        local_csr_write(local_csr_mailbox0, 0x3333);
        local_csr_write(local_csr_mailbox1, ((uint64_t) ruledata) & 0xffffffff);
        local_csr_write(local_csr_mailbox2, ((uint64_t) ruledata) >> 32);
        local_csr_write(local_csr_mailbox3, rule | (d.record_shift << 16));
        __asm ctx_arb[bpt];
#endif
        /* FIXME - grab chunks into xfers - grab what you can */

        __mem_read32(rd_buf_ptr,
                     ruledata,
                     PIF_ACTION_OPDATA_LW * 4,
                     PIF_ACTION_OPDATA_MAX_LW * 4,
                     ctx_swap,
                     &sig);

        act_op = (__xread union pif_action_opdata *) rd_buf_ptr;
        result.action_len = act_op->actdata_cnt + PIF_ACTION_OPDATA_LW;

        if (result.action_len + action_buffer_offset > PIF_LOOKUP_BUFFER_MAX) {
            result.action_len = -PIF_LOOKUP_ERROR_BUFFER_OVERFLOW;
            return result;
        }

        result.action_id = act_op->action_id;

#ifdef DESPERATE_DEBUG
        local_csr_write(local_csr_mailbox0, 0x4444);
        local_csr_write(local_csr_mailbox1, act_op->actdata_cnt);
        local_csr_write(local_csr_mailbox2, act_op->action_id);
        local_csr_write(local_csr_mailbox3, rule);
        __asm ctx_arb[bpt];
#endif

        /* grab the rest */
        ruledata++;

        if (act_op->actdata_cnt == 0x00) {
            /* Do not read with length 0, on indirect it will be -1 */
#ifdef DESPERATE_DEBUG
            local_csr_write(local_csr_mailbox0, 0x5555);
            local_csr_write(local_csr_mailbox1, act_op->actdata_cnt);
            local_csr_write(local_csr_mailbox2, act_op->action_id);
            local_csr_write(local_csr_mailbox3, rule);
#endif
        } else {
            /* An action cant be longer than PIF_ACTION_OPDATA_MAX_LW */
            __mem_read32(rd_buf_ptr + PIF_ACTION_OPDATA_LW,
                        ruledata,
                        (act_op->actdata_cnt) * 4,
                        (PIF_ACTION_OPDATA_MAX_LW - PIF_ACTION_OPDATA_LW) * 4,
                        ctx_swap,
                        &sig);
        }

        /* Copy into the aggregated action data buffer */
        for (i = 0; i < result.action_len; i++)
            wr_buf[i] = rd_buf[i];

        __mem_write32(wr_buf,
                      action_buffer + action_buffer_offset,
                      result.action_len * 4,
                      PIF_ACTION_OPDATA_MAX_LW * 4,
                      ctx_swap,
                      &sig);

        return result;
    }
    /* not reached */
}
