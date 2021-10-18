/*
 * Copyright (C) 2014,  Netronome Systems, Inc.  All rights reserved.
 *
 * @file          lib/dcfl/_c/an.c
 * @brief         DCFL Aggregation network implementation
 */

#include <dcfl.h>
#include <an.h>
#include <std/reg_utils.h>
#include <nfp/mem_atomic.h>
#include <nfp_intrinsic.h>


int
dcfl_an_final(__lmem struct dcfl_label_set *labels_in1, uint32_t nlabel1,
        __lmem struct dcfl_label_set *labels_in2, uint32_t nlabel2,
        __lmem struct dcfl_label_set *labels_out,
        uint32_t stage_base, uint32_t stage_base_hi)
{
    uint32_t labels_in1_id, labels_in2_id, labels_out_cnt = 0;
    uint16_t labels_in2_label;
    uint32_t i, num_an_entries = DCFL_AN_FIN_BLOCK_NUM_RULES;
    __addr40 __mem unsigned long *an_base;
    __addr40 __mem unsigned long *addr_an_block;
    __xread struct dcfl_an_entry
        an_entries[DCFL_MEM_AN_BLOCK_LW/DCFL_AN_ENTRY_LW];
    struct dcfl_an_entry an_entry;
    __xread struct dcfl_an_entry prio_entry;
    __gpr uint32_t priority_idx, priority;
#ifdef DCFL_SUPPORT_MATCH_FILTERING
    __gpr uint32_t lowest_prio, prev_prio = DCFL_MAGIC_PRIORITY;
    __gpr uint32_t priority_rule_idx, prio_found = 0;
#else
#ifdef DCFL_PRIORITY_SORT
    __gpr uint32_t sorted;
    __gpr uint32_t temp;
#endif
#endif

    an_base = (__addr40 __mem void *) (((unsigned long long)stage_base_hi << 32) |
               (unsigned long long)stage_base);

    for (labels_in1_id=0; labels_in1_id<nlabel1 ; labels_in1_id++) {
        /* add the offset to read the AN block */
        /* TODO check if this will work for any memory type */
        addr_an_block = (__addr40 __mem void *)
            ((__addr40 __mem unsigned long *) an_base +
            ((labels_in1->labels[labels_in1_id])
                * DCFL_MEM_AN_BLOCK_LW));

        mem_read64(&an_entries, addr_an_block, sizeof(an_entries));

        start_of_block:

        for (labels_in2_id=0; labels_in2_id<nlabel2 ; labels_in2_id++) {
            labels_in2_label = labels_in2->labels[labels_in2_id];

            /* go through the entries in the AN block */
            for (i = 0; i < num_an_entries; i++) {
                an_entry = an_entries[i];
                if (an_entry.label == 0) {
                    break;
                }

                /* if there is a label match add the metalabel to the
                 * labelset and break (same label is only once in the
                 * same block, except for the final stage) */
                if (an_entry.label == labels_in2_label) {
#ifdef DCFL_SUPPORT_MATCH_FILTERING

                    priority_idx = DCFL_AN_FIN_BLOCK_NUM_RULES + (i/2);
                    mem_read32(&prio_entry, addr_an_block + priority_idx,
                                sizeof(uint32_t));
                    if (i % 2) {
                        priority = prio_entry.prio_even;
                    } else {
                        priority = prio_entry.prio_odd;
                    }

                    /* Determine the highest priority rule (lowest prio value).
                        * If all priorities are identical, no filtering will occur. */
                    if (prev_prio == DCFL_MAGIC_PRIORITY) {
                        lowest_prio = prev_prio = priority;
                    } else {
                        if (priority != prev_prio) {
                            /* Differing priority in matched result */
                            if (priority < lowest_prio) {
                                prio_found = 1;
                                lowest_prio = priority;
                                priority_rule_idx = labels_out_cnt;
                            }
                        }
                        prev_prio = priority;
                    }

                    labels_out->labels[labels_out_cnt] =
                        an_entry.mlabel;
                    labels_out_cnt++;
#else
#ifdef DCFL_PRIORITY_SORT
                    priority_idx = DCFL_AN_FIN_BLOCK_NUM_RULES + (i/2);
                    mem_read32(&prio_entry, addr_an_block + priority_idx,
                                sizeof(uint32_t));
                    if (i % 2) {
                        priority = prio_entry.prio_even;
                    } else {
                        priority = prio_entry.prio_odd;
                    }

                    labels_out->results[labels_out_cnt].priority = priority;
                    labels_out->results[labels_out_cnt].rule_id =
                        an_entry.mlabel;

                    labels_out_cnt++;
#else
                    labels_out->labels[labels_out_cnt] =
                        an_entry.mlabel;
                    labels_out_cnt++;
#endif
#endif

                    if (labels_out_cnt == DCFL_LABELSET_LW) {
                        mem_incr64(&DCFL_CROSS_OVERFLOW_CNTR);
                        goto dcfl_an_truncate;
                    }
                }
            } /* for (i=0; */
        } /* for(labels_in2_id=0; */

        /* Extra blocks? */
        an_entry = an_entries[DCFL_AN_BLOCK_NUM_ENTRIES];
        if (an_entry.values[0] > 0) {
            addr_an_block = (__addr40 __mem void *)
                (((unsigned long long)stage_base_hi << 32) |
                (unsigned long long)an_entry.values[0]);
            mem_read64(&an_entries, addr_an_block, sizeof(an_entries));
            goto start_of_block;
        }

    } /* for (labels_in1_id=0; */

dcfl_an_truncate:
#ifdef DCFL_SUPPORT_MATCH_FILTERING
    /* Return only the priority rule */
    if (prio_found) {
        labels_out->labels[0] = labels_out->labels[priority_rule_idx];
        return 1;
    }
#endif

    return labels_out_cnt;
}

__intrinsic
mem_lookup_result_in_read_reg_t *cmd_mem_lookup(
    __xwrite void *xfer,
    volatile void __addr40 __mem *address,
    uint32_t count,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
)
{
    _INTRINSIC_BEGIN;
    {
        uint32_t hi_addr, low_addr;

        _INTRINSIC_CONVERT_HI_LO_ADDRESS(address);
        if (__is_ct_const(sync))                                    \
        {                                                           \
            CT_ASSERT(sync == sig_done);                            \
        }
        CT_ASSERT(__is_write_reg(xfer));

        if (__is_ct_const(count) && count <= 2)
        {
            CT_ASSERT(count != 0);
            if (sync == sig_done)
            {
                __asm mem[lookup, *xfer, hi_addr, << 8, low_addr, __ct_const_val(count)], sig_done[*sig_pair_ptr]
            }
        }
        else
        {
            if (!__is_ct_const(count))
            {
                CT_QPERFINFO_INDIRECT_REF(cmd_mem_lookup);
            }
            if (sync == sig_done)
            {
                __asm mem[lookup, *xfer, hi_addr, << 8, low_addr,__ct_const_val(count)], sig_done[*sig_pair_ptr]
            }
        }
        return (mem_lookup_result_in_read_reg_t *)(xfer);
    }
    _INTRINSIC_END;
}

int
dcfl_an_lkp(__lmem struct dcfl_label_set *labels_in1, uint32_t nlabel1,
        __lmem struct dcfl_label_set *labels_in2, uint32_t nlabel2,
        __lmem struct dcfl_label_set *labels_out,
        uint32_t stage_base, uint32_t stage_base_hi, uint32_t an_alut_loc_reg
       )
{
    uint32_t labels_in1_id, labels_in2_id;
    __gpr uint16_t nlabels_out = 0;
    uint64_t addr_an_block;
    volatile mem_lookup_alut_t alut_table_desc;
    volatile __xwrite unsigned int lookup_index[2];
    SIGNAL_PAIR sig_pair;
    __xrw uint32_t lkp_value[4];
    mem_lookup_result_in_read_reg_t *read_result;
    __addr40 __mem void  *pmm;

    uint64_t addr_stage_hi = (uint64_t)stage_base_hi << 32;
    uint64_t addr_stage = (uint64_t) (addr_stage_hi |
               (unsigned long long)stage_base);

    alut_table_desc.value = 0;
    reg_cp(lkp_value, 0, sizeof(lkp_value));

    if (stage_base_hi >= (MULE_IMU0_HI_BYTE))
        alut_table_desc.internal_memory = 1;

    alut_table_desc.direct_lookup   = 0;
    alut_table_desc.table_number    = an_alut_loc_reg;
    alut_table_desc.table_size      = LOOKUP_ALUT_SIZE_4;

    for (labels_in1_id=0; labels_in1_id<nlabel1 ; labels_in1_id++) {
        /* add the offset to read the AN block */
        addr_an_block = addr_stage +
            ((labels_in1->labels[labels_in1_id])
                * sizeof(struct dcfl_me_an_block));

        alut_table_desc.address_bits = (uint32_t)(addr_an_block >> 4);
        pmm = (__addr40 __mem void *) (addr_stage_hi | alut_table_desc.value);

        for (labels_in2_id=0; labels_in2_id<nlabel2 ; labels_in2_id++) {
            lkp_value[0] = labels_in2->labels[labels_in2_id];

            read_result = cmd_mem_lookup((void *)&lkp_value[0], pmm, 1, sig_done, &sig_pair);
            wait_for_all(&sig_pair);

            if (read_result->result) {
                /* Match! */
                labels_out->labels[nlabels_out] = read_result->result;
                nlabels_out++;

                if (nlabels_out == DCFL_MAX_LABELS_PER_LKP) {
                    mem_incr64(&DCFL_CROSS_OVERFLOW_CNTR);
                    goto dcfl_an_lkp_truncate;
                }
            }
        }
    }

dcfl_an_lkp_truncate:
    return nlabels_out;
}
