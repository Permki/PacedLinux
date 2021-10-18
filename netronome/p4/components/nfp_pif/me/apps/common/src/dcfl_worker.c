/**
 * Copyright (C) 2015,  Netronome Systems, Inc. All rights reserved.
 *
 * @file        dcfl_worker.c
 * @brief       DCFL classification process in the DCFL worker ME.
 */

#include <nfp.h>
#include <nfp/me.h>
#include <nfp6000/nfp_me.h>
#include <nfp/mem_ring.h>
#include <nfp/mem_bulk.h>
#include <std/reg_utils.h>

#include <dcfl.h>
#include <field_lkp.h>
#include <an.h>
#include <pif_common.h>
#include "system_init_c.h"

//#define DCFL_DEBUG

/* Bits to mask on thread_info structure to get interthread_signal value */
#define DCFL_ITHREAD_MASK           0xFF001FFC

MEM_WORKQ_DECLARE(DCFL_KEYQ_NAME, DCFL_KEYQ_ENTRIES);

/**
 * Read key from address found in work queue.
 *
 * @param key           Pointer in local mem to copy the key to.
 * @param src           Source address
 * @param size          Size of key to copy
 */
static void
dcfl_read_key(__lmem uint32_t *key, __mem __addr40 uint64_t src, uint32_t size)
{
    __xread uint32_t read_buf[DCFL_READ_BUF_LW];
    unsigned int i, tocopy, chunk, offset = 0, lmoff = 0;

    tocopy = size;

    while (tocopy > 0) {
        if (tocopy > DCFL_READ_BUF_LW*4)
            chunk = DCFL_READ_BUF_LW*4;
        else
            chunk = tocopy;

        mem_read32(read_buf, (__mem __addr40 uint64_t *)(src + offset),
                   chunk);

        for (i = 0; i < (chunk + 3)/4; i++) {
            key[lmoff] = read_buf[i];
            lmoff += 1;
        }

        tocopy -= chunk;
        offset += chunk;
    }
}

/**
 * Write first result to this thread's reserved space in dcfl_results symbol.
 * PIF should only have one result.
 *
 * Since only the rule id is returned, a magic word is used to indicate a miss.
 *
 * @param src           Pointer to label set containing dcfl results
 * @param nrules        Number of results in label set
 * @param dest          Destination address of results
 */
static void
dcfl_write_result(__lmem struct dcfl_label_set *src, uint32_t nrules,
               __mem __addr40 uint64_t dest)
{
    __xwrite uint32_t write_buf[1];

    if (nrules > 0) {
        write_buf[0] = src->labels[0];
    } else {
        write_buf[0] = PIF_LOOKUP_MISS_RULE;
    }
    mem_write32(write_buf, (__mem __addr40 uint64_t *)dest,
                sizeof(write_buf));
}

void
main(void)
{
    __lmem uint32_t key[PIF_LOOKUP_KEY_SIZE_LW];
    __lmem struct dcfl_label_set label_set[3];
    uint32_t nrules;
    __xread uint32_t work_xfer[DCFL_KEY_WORKQ_LW];
    unsigned int work_size = sizeof(work_xfer);
    struct dcfl_key_workq_entry work_entry;
    int32_t label_set_id = 0;
    uint32_t ithread_sig;
    mem_ring_addr_t wq_raddr = mem_ring_get_addr(&DCFL_KEYQ_NAME);

    system_init_wait_done();

    for (;;) {
        /* Wait for new key address and calling thread info */
        mem_workq_add_thread(DCFL_KEYQ_NUM, wq_raddr, &work_xfer,
                             work_size);

        reg_cp((void *)&work_entry, work_xfer, sizeof(work_xfer));

        /* read classification request from emem */
        dcfl_read_key(&key[0], work_entry.key_addr, work_entry.size);

#ifdef DCFL_DEBUG
        local_csr_write(local_csr_mailbox_0, work_entry.thread_info.test_ct_num);
        local_csr_write(local_csr_mailbox_1, key[0]);
        local_csr_write(local_csr_mailbox_2, work_entry.cfg_ptr);
#endif

        /* pass classification request to dcfl_classify */
        nrules = dcfl_classify((void *)&key,
                               work_entry.thread_info.test_ct_num,
                               label_set, &label_set_id,
                               work_entry.cfg_ptr);

        dcfl_write_result(&label_set[label_set_id], nrules,
                          work_entry.result_addr);

        /* Signal calling thread that DCFL aggregation has been completed */
        ithread_sig = work_entry.thread_info.value & DCFL_ITHREAD_MASK;
        __asm { ct[interthread_signal, --, ithread_sig, 0, --] }
#ifdef DCFL_DEBUG
        local_csr_write(local_csr_mailbox_3, nrules);
        __asm ctx_arb[bpt];
#endif
    }
}
