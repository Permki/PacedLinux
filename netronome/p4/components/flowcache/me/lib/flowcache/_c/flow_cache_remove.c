/*
 * Copyright (C) 2014-2016,  Netronome Systems, Inc.  All rights reserved.
 *
 * @file        flow_cache_remove.c
 * @brief       Remove entry from CAM, entry table.
 */

#ifndef _FLOW_CACHE_REMOVE_C_
#define _FLOW_CACHE_REMOVE_C_

#include <assert.h>
#include <nfp.h>
#include <stdint.h>
#include <types.h>
#include <nfp/mem_atomic.h>
#include <nfp/mem_bulk.h>
#include <nfp/mem_ring.h>
#include <nfp/me.h>
#include <nfp6000/nfp_me.h>
#include <mem_atomic_indirect_c.h>

#include "flow_cache_global_c.h"
#include "flow_cache_c.h"
#include "flow_cache_remove_c.h"
#include "flow_cache_timeout_lock_c.h"
#include "flow_cache_timestamp_c.h"



#ifdef FC_TIMEOUT_JOURNAL
    #include "flow_cache_timeout_journal_c.h"

     extern __gpr uint32_t journal_ring_num;
#endif


volatile __declspec(i28.imem, shared, import) uint32_t fc_timeout_err_free_entry_zero[8];
volatile __declspec(i28.imem, shared, import) uint32_t fc_timeout_err_ovf_hash_value_zero[8];

/*
 * Explicitly copy entry from src to destination.
 * This is only done when copying from linked list entry to 1:1 case.
 * Return some of the fields read so that we do not have to do another read.
 */
__gpr fc_entry_fields_t fc_copy_entry(__mem __addr40 uint32_t *src_ptr, __mem __addr40 uint32_t *dest_ptr)
{
    __xwrite uint32_t                   xfer_write[16];
    __xread uint32_t                    xfer_read[16];
    SIGNAL                              sig;
    __gpr fc_entry_fields_t             entry_fields;
    __gpr uint32_t                      length = 16;

    entry_fields.value = 0x00;

    /*
    * Copy entry
    * Have to split from 32 xfers to 2 reads of 16 xfers each
    */
    {
        /*
         * Bulk read/write for 16 32-bit words
         */
        __mem_read32(xfer_read, src_ptr, length << 2, length << 2, sig_done, &sig);
        wait_for_all(&sig);

        xfer_write[0] = xfer_read[0];
        xfer_write[1] = xfer_read[1];
        xfer_write[2] = xfer_read[2];
        xfer_write[3] = xfer_read[3];
        xfer_write[4] = xfer_read[4];
        xfer_write[5] = xfer_read[5];
        xfer_write[6] = xfer_read[6];
        xfer_write[7] = xfer_read[7];
        xfer_write[8] = xfer_read[8];
        xfer_write[9] = xfer_read[9];
        xfer_write[10] = xfer_read[10];
        xfer_write[11] = xfer_read[11];
        xfer_write[12] = xfer_read[12];
        xfer_write[13] = xfer_read[13];
        xfer_write[14] = xfer_read[14];
        xfer_write[15] = xfer_read[15];


        __mem_write32(xfer_write, dest_ptr, length << 2, length << 2, sig_done, &sig);


        if (xfer_read[_FC_ENTRY_NEXT_wrd] & 0x00ffffff)
        {
            /*
             * set next if next field is set in entry
             */
            entry_fields.next = 1;
        }

        /*
        * Copy timestamp and timeout
        */
        entry_fields.value = (entry_fields.next << 24) | ((xfer_read[_FC_ENTRY_TIMEOUT_wrd] & 0xff000000) >> 8) | (xfer_read[_FC_ENTRY_TIMESTAMP_wrd] & 0xffff);

        wait_for_all(&sig);
    }

    {
        /*
         * Bulk read/write for only 12 32-bit words, the last four has to be with atomic engine
         */
        length = 12;

        __mem_read32(xfer_read, src_ptr + 16, length << 2, length << 2, sig_done, &sig);
        wait_for_all(&sig);

        xfer_write[0] = xfer_read[0];
        xfer_write[1] = xfer_read[1];
        xfer_write[2] = xfer_read[2];
        xfer_write[3] = xfer_read[3];
        xfer_write[4] = xfer_read[4];
        xfer_write[5] = xfer_read[5];
        xfer_write[6] = xfer_read[6];
        xfer_write[7] = xfer_read[7];
        xfer_write[8] = xfer_read[8];
        xfer_write[9] = xfer_read[9];
        xfer_write[10] = xfer_read[10];
        xfer_write[11] = xfer_read[11];


        __mem_write32(xfer_write, dest_ptr + 16, length << 2, length << 2, sig_done, &sig);

        wait_for_all(&sig);
    }

    {
        /*
         * Atomic read/write for last 4 words
         */
        length = 4;

        __mem_read_atomic(xfer_read, src_ptr + 16 + 12, length << 2, length << 2, sig_done, &sig);
        wait_for_all(&sig);

        xfer_write[0] = xfer_read[0];
        xfer_write[1] = xfer_read[1];
        xfer_write[2] = xfer_read[2];
        xfer_write[3] = xfer_read[3];


        /*
        * Atomic write for last 4 words
        */
        __mem_write_atomic(xfer_write, dest_ptr + 16 + 12, length << 2, length << 2, sig_done, &sig);
        wait_for_all(&sig);
    }

    return entry_fields;
}


void fc_copy_payload(__mem __addr40 uint32_t *src_ptr, __mem __addr40 uint32_t *dest_ptr)
{
    __gpr uint32_t                          payload_copied = 0;
    __xwrite uint32_t                       xfer_write[16];
    __xread uint32_t                        xfer_read[16];
    SIGNAL                                  sig;

    /*
    * Copy payload
    * Have to split from 32 xfers to 2 reads of 16 xfers each
    */

    while (payload_copied < FC_TIMEOUT_PAYLOAD_SIZE/4)
    {
        {
            __mem_read32(xfer_read, src_ptr + payload_copied, 16 << 2, 16 << 2, sig_done, &sig);
            wait_for_all(&sig);

            xfer_write[0] = xfer_read[0];
            xfer_write[1] = xfer_read[1];
            xfer_write[2] = xfer_read[2];
            xfer_write[3] = xfer_read[3];
            xfer_write[4] = xfer_read[4];
            xfer_write[5] = xfer_read[5];
            xfer_write[6] = xfer_read[6];
            xfer_write[7] = xfer_read[7];
            xfer_write[8] = xfer_read[8];
            xfer_write[9] = xfer_read[9];
            xfer_write[10] = xfer_read[10];
            xfer_write[11] = xfer_read[11];
            xfer_write[12] = xfer_read[12];
            xfer_write[13] = xfer_read[13];
            xfer_write[14] = xfer_read[14];
            xfer_write[15] = xfer_read[15];

            /*
            * Copy entry values
            */
            __mem_write32(xfer_write, dest_ptr + payload_copied, 16 << 2, 16 << 2, sig_done, &sig);
            wait_for_all(&sig);
        }

        payload_copied += 16;
    }

    return;
}




/*
 * This function handles the case where CAM entry 1 is empty.
 * Check if overflow list is not empty, get an exclusive lock on it. Keep on retrying to get lock.
 * We still hold a lock an exclusive lock on CAM entry 1. Hash value has not been emptied, otherwise
 * CAM lookup will write to it.
 * We get the hash value from entry (SDN-811) and link it into CAM entry 1. If more entries in the overflow
 * list with the same hash value, also link them into CAM entry 1 list.
 * Because this is 1:1 the payload and entry value has to be copied to the fixed 1:1 payload and entry address.
 *
 * If overflow list is empty, clear out overflow head etc.
 *
 * Do not clear the overflow lock or CAM entry 1 lock.
 */
static __gpr uint32_t fc_process_overflow_list_cam_entry1(
    __mem __addr40 uint32_t *bucket_base_ptr,
    __mem __addr40 uint32_t *entry_base_ptr,
    __mem __addr40 uint32_t *payload_base_ptr,
    __mem __addr40 uint32_t *cam_read_address,
    __gpr fc_lock_t lock,
    __gpr uint32_t ovf_head_idx       // entry idx stored in overflow head
    )
{
#ifdef FC_ENABLE_OVERFLOW
    /*
    * Move overflow entries to empty CAM 1
    */

        __mem __addr40 uint32_t         *ovf_entry_ptr;
        __gpr uint32_t                  hash;
        SIGNAL                          sig_1, sig_2, sig_3;

        ovf_entry_ptr = (__mem __addr40 uint32_t *)FC_SET_LINKED_ENTRY_ADDRESS(entry_base_ptr, ovf_head_idx);

        {
            /*
            * Read the new hash value from the ovf entry, also save next entry.
            */

            __xread fc_entry_t              ovf_entry;

            mem_read32(&ovf_entry.value[_FC_ENTRY_HASH_HIGH_wrd], ovf_entry_ptr + _FC_ENTRY_HASH_HIGH_wrd, 6 << 2);
            hash = FC_EXTRACT_HASH(ovf_entry.hash_hi, ovf_entry.hash_lo);

            if (hash == 0x00)
            {
#ifdef FC_TIMEOUT_JOURNAL
                fc_timeout_journal_t    error_data;
                __gpr uint32_t          hi_addr;
                FC_SET_HI_ADDRESS(journal_ring_num >> 10);

                /*
                * Hash value is empty, release overflow lock.
                * This should never happen!
                */

                error_data.error_code = JOURNAL_REMOVE_OVF_HASH_EMPTY;
                error_data.param_1 = ovf_head_idx;
                error_data.param_2 = 1;
                error_data.param_3 = 0;

                mem_ring_journal_fast((journal_ring_num & 0x3ff), hi_addr, error_data.value);
#endif
                fc_timeout_err_ovf_hash_value_zero[ctx()]++;
                return 0;
            }
        }

        {
            /*
            * Write the values of the overflow entry to the CAM entry 1.
            * Hash value, timeout, timestamp, stats.
            */
            __xwrite fc_cam_bucket_t    wr_cam_bucket;
            __gpr uint32_t              next_ovf_entry_idx = 0;

            {
                __xread fc_cam_bucket_t     rd_cam_bucket;

                __mem_read_atomic(&rd_cam_bucket.value[FC_TIMEOUT_OFV_wrd], cam_read_address + FC_TIMEOUT_OFV_wrd, 3 << 2, 3 << 2, sig_done, &sig_1);
                wait_for_all(&sig_1);

                {
                    /*
                     * timeout
                     */
                    wr_cam_bucket.value[FC_TIMEOUT_1_wrd] = (rd_cam_bucket.timeout_overflow << 16);
                    mem_write_atomic_with_bytemask(
                        &wr_cam_bucket.value[FC_TIMEOUT_1_wrd],
                        cam_read_address + FC_TIMEOUT_1_wrd,
                        1 << 2,
                        1 << 2,
                        sig_done,
                        &sig_1,
                        0x04);  // mask 0x00ff0000
                }

                {
                    /*
                     * timestamp
                     */
                    wr_cam_bucket.value[FC_TIMESTAMP_1_wrd] = rd_cam_bucket.timestamp_overflow;

                    mem_write_atomic_with_bytemask(
                        &wr_cam_bucket.value[FC_TIMESTAMP_1_wrd],
                        cam_read_address + FC_TIMESTAMP_1_wrd,
                        1 << 2,
                        1 << 2,
                        sig_done,
                        &sig_2,
                        0x03);  // mask 0x0000ffff
                }

                wait_for_all(&sig_1, &sig_2);

                {
                    /*
                     * write new hash value, exlusive lock is still in place
                     */
                    wr_cam_bucket.value[FC_CAM_HASH_1_wrd] = hash;

                    mem_write_atomic_with_bytemask(
                        &wr_cam_bucket.value[FC_CAM_HASH_1_wrd],
                        cam_read_address + FC_CAM_HASH_1_wrd,
                        1 << 2,
                        1 << 2,
                        ctx_swap,
                        &sig_1,
                        0x07);  // mask 0x00ffffff
                }
            }


            {
                /*
                * First entry  (1-1 mapping).
                * We have to copy the entry to hardcoded entry address.
                * Zero out next
                */
                __mem __addr40 uint32_t *first_entry_ptr = (__mem __addr40 uint32_t *)FC_SET_FIRST_ENTRY_ADDRESS(entry_base_ptr, (lock.bucket_address << 6));


#ifdef FC_TIMEOUT_JOURNAL
                if (((uint32_t)first_entry_ptr < (uint32_t)entry_base_ptr) || ((uint32_t)first_entry_ptr >= (uint32_t)payload_base_ptr))
                {
                    fc_timeout_journal_t    error_data;
                    __gpr uint32_t          hi_addr;
                    FC_SET_HI_ADDRESS(journal_ring_num >> 10);

                    /*
                    * First entry pointer out of bounds
                    */

                    error_data.error_code = JOURNAL_ENTRY_PTR_OUT_OF_BOUNDS;
                    error_data.param_1 = 0;
                    error_data.param_2 = (lock.bucket_address << 6);
                    error_data.param_3 = 0;

                    mem_ring_journal_fast((journal_ring_num & 0x3ff), hi_addr, error_data.value);
                }

                if (((uint32_t)ovf_entry_ptr < (uint32_t)entry_base_ptr) || ((uint32_t)ovf_entry_ptr >= (uint32_t)payload_base_ptr))
                {
                    fc_timeout_journal_t    error_data;
                    __gpr uint32_t          hi_addr;
                    FC_SET_HI_ADDRESS(journal_ring_num >> 10);

                    /*
                    * Destination entry pointer out of bounds
                    */

                    error_data.error_code = JOURNAL_ENTRY_PTR_OUT_OF_BOUNDS;
                    error_data.param_1 = 1;
                    error_data.param_2 = ovf_head_idx;
                    error_data.param_3 = 0;

                    mem_ring_journal_fast((journal_ring_num & 0x3ff), hi_addr, error_data.value);
                }
#endif

                fc_copy_entry(ovf_entry_ptr, first_entry_ptr);
            }


            {
                /*
                * Copy payload from next entry to entry to be removed.
                * Use payload size of FC_TIMEOUT_PAYLOAD_SIZE
                */

                __mem __addr40 uint32_t *first_payload_ptr = (__mem __addr40 uint32_t *)FC_SET_PAYLOAD_ADDRESS(payload_base_ptr, (lock.bucket_address << 6));
                __mem __addr40 uint32_t *ovf_payload_ptr = (__mem __addr40 uint32_t *)FC_SET_LINKED_PAYLOAD_ADDRESS(payload_base_ptr, ovf_head_idx);

#ifdef FC_TIMEOUT_JOURNAL
                if (((uint32_t)first_payload_ptr < (uint32_t)payload_base_ptr) || ((uint32_t)first_payload_ptr >= (uint32_t)payload_base_ptr + _FC_PAYLOADS_ALLOC_SIZE))
                {
                    fc_timeout_journal_t    error_data;
                    __gpr uint32_t          hi_addr;
                    FC_SET_HI_ADDRESS(journal_ring_num >> 10);

                    /*
                    * First entry pointer out of bounds
                    */

                    error_data.error_code = JOURNAL_PAYLOAD_PTR_OUT_OF_BOUNDS;
                    error_data.param_1 = 0;
                    error_data.param_2 = (lock.bucket_address << 6);
                    error_data.param_3 = 0;

                    mem_ring_journal_fast((journal_ring_num & 0x3ff), hi_addr, error_data.value);
                }

                if (((uint32_t)ovf_payload_ptr < (uint32_t)payload_base_ptr) || ((uint32_t)ovf_payload_ptr >= (uint32_t)payload_base_ptr + _FC_PAYLOADS_ALLOC_SIZE))
                {
                    fc_timeout_journal_t    error_data;
                    __gpr uint32_t          hi_addr;
                    FC_SET_HI_ADDRESS(journal_ring_num >> 10);

                    /*
                    * Destination entry pointer out of bounds
                    */

                    error_data.error_code = JOURNAL_PAYLOAD_PTR_OUT_OF_BOUNDS;
                    error_data.param_1 = 1;
                    error_data.param_2 = ovf_head_idx;
                    error_data.param_3 = 0;

                    mem_ring_journal_fast((journal_ring_num & 0x3ff), hi_addr, error_data.value);
                }
#endif

                fc_copy_payload(ovf_payload_ptr, first_payload_ptr);
            }



            {
                /*
                * Check for other entries in the overflow list which have similar hash values
                * than to the one just added to the CAM.
                * These entries have to be linked into the CAM list and removed from the overflow list.
                */
                __gpr uint32_t                  head_ovf_entry_idx = 0;
                __gpr uint32_t                  next_cam_entry_flag = 1;

                __xwrite uint32_t               next;
                __xread fc_entry_t              rd_entry;
                __mem __addr40 fc_entry_t       *head_ovf_entry_ptr = 0;    // entry list of overflow entry
                __mem __addr40 fc_entry_t       *head_cam_entry_ptr = (__mem __addr40 fc_entry_t *)FC_SET_FIRST_ENTRY_ADDRESS(entry_base_ptr, (lock.bucket_address << 6));

                mem_read32(&rd_entry.value[_FC_ENTRY_NEXT_wrd], ovf_entry_ptr + _FC_ENTRY_NEXT_wrd, 1 << 2);
                next_ovf_entry_idx = rd_entry.next;

                {
                    /*
                    * Put free cache buffer (from overflow) back on the ring
                    */

                    __xwrite uint32_t       write_xfer = ovf_head_idx;
                    uint32_t                entry_mu;
                    __gpr uint32_t          hi_addr;


                    if (ovf_head_idx != 0x00)
                    {
                        /*
                        * Set the entry address pointer to the correct MU.
                        */
                        FC_NEXT_MEMORY_UNIT(FC_TIMEOUT_LB_EMU, lock.bucket_mu, entry_mu);
                        FC_SET_HI_ADDRESS(entry_mu);

                        if (mem_ring_put(FC_TIMEOUT_FREE_RING, hi_addr, &write_xfer, 1 << 2) == -1)
                        {
                            //result = FC_REMOVE_ERROR;
                        }
                    }
                    else
                    {
                        /*
                        * Error
                        */
                        fc_timeout_err_free_entry_zero[ctx()]++;

#ifdef FC_TIMEOUT_JOURNAL
                        {
                            /*
                            * Index cannot be zero
                            */
                            fc_timeout_journal_t error_data;
                            __gpr uint32_t          hi_addr;
                            FC_SET_HI_ADDRESS(journal_ring_num >> 10);

                            error_data.value = 0x00;
                            error_data.error_code = JOURNAL_FREE_ENTRY_IDX_ZERO;
                            error_data.param_1 = ovf_head_idx;
                            error_data.param_2 = 1; // CAM entry 1

                            mem_ring_journal_fast((journal_ring_num & 0x3ff), hi_addr, error_data.value);
                        }
#endif
                    }
                }

                if (rd_entry.next != 0)
                {
                    while (next_ovf_entry_idx != 0)
                    {
                        /*
                        * while we have more entries in the overflow list, see if we can match any with
                        * the CAM entry (hash) that we have just added.
                        */

                        __gpr uint32_t              entry_idx;
                        __gpr uint32_t              rd_hash = 0;
                        __mem __addr40 uint32_t     *next_ovf_ptr = (__mem __addr40 uint32_t *) FC_SET_LINKED_ENTRY_ADDRESS(entry_base_ptr, next_ovf_entry_idx);

                        /*
                        * Read hash and next
                        */
                        __mem_read32(&rd_entry.value[_FC_ENTRY_HASH_HIGH_wrd], next_ovf_ptr + _FC_ENTRY_HASH_HIGH_wrd, 3 << 2, 3 << 2, sig_done, &sig_1);
                        wait_for_all(&sig_1);

                        entry_idx = rd_entry.next; // save xfer before context swap
                        rd_hash = FC_EXTRACT_HASH(rd_entry.hash_hi, rd_entry.hash_lo);

                        if (rd_hash == hash)
                        {
                            /*
                            * If next overflow entry have the same hash as the one we just added to the CAM,
                            * link it into head_cam_entry_ptr list and move onto next overflow entry
                            */

                            next = next_ovf_entry_idx << 8;            // head_cam_entry_ptr->next = next_ovf_entry_idx;
                            mem_write8(&next, (__mem __addr40 uint8_t *)head_cam_entry_ptr + (_FC_ENTRY_NEXT_wrd << 2) + 1, 3);

                            head_cam_entry_ptr = (__mem __addr40 fc_entry_t *)next_ovf_ptr;

                            /*
                            * set next in CAM bucket
                            */
                            if (next_cam_entry_flag)
                            {
                                mem_bitset_imm((1 << _FC_BUCKET_STATS_NEXT_lsb), cam_read_address + FC_STATS_1_wrd);
                                next_cam_entry_flag = 0;
                            }
                        }
                        else
                        {
                            /*
                            * Link entry into overflow list
                            */
                            if (head_ovf_entry_ptr != 0)
                            {
                                /*
                                * Write the next to entry - only 3 bytes
                                */
                                next = next_ovf_entry_idx << 8;             // head_ovf_entry_ptr->next = next_ovf_entry_idx;
                                mem_write8(&next, (__mem __addr40 uint8_t *)head_ovf_entry_ptr + (_FC_ENTRY_NEXT_wrd << 2) + 1, 3);
                            }
                            else
                            {
                                head_ovf_entry_idx = next_ovf_entry_idx;
                            }

                            head_ovf_entry_ptr = (__mem __addr40 fc_entry_t *)next_ovf_ptr;
                        }

                        next_ovf_entry_idx = entry_idx;

                    } // while

                    next = 0x00;            // head_cam_entry_ptr->next = 0;
                    mem_write8(&next, (__mem __addr40 uint8_t *)head_cam_entry_ptr + (_FC_ENTRY_NEXT_wrd << 2) + 1, 3);

                    if (head_ovf_entry_ptr != 0)
                    {
                        // head_ovf_entry_ptr->next = 0;
                        mem_write8(&next, (__mem __addr40 uint8_t *)head_ovf_entry_ptr + (_FC_ENTRY_NEXT_wrd << 2) + 1, 3);
                    }

                    next_ovf_entry_idx = head_ovf_entry_idx;
                }

                if (next_ovf_entry_idx != 0)
                {
                    /*
                    * Overflow list is not empty. Copy "second" in list to CAM overflow entry (overflow head, timestamp, timeout).
                    * The first has been moved to head of CAM entry1, and added back to the ring.
                    * Check overflow stats (next) bit
                    */

                    __xread fc_entry_t      entry;
                    __mem __addr40 uint32_t *next_entry_ptr = (__mem __addr40 uint32_t *) FC_SET_LINKED_ENTRY_ADDRESS(entry_base_ptr, next_ovf_entry_idx);

                    /*
                    * Read timeout, timestamp and next
                    */
                    __mem_read32(&entry.value[_FC_ENTRY_TIMESTAMP_wrd], next_entry_ptr + _FC_ENTRY_TIMESTAMP_wrd, 2 << 2, 2 << 2, sig_done, &sig_1);
                    wait_for_all(&sig_1);

                    if (entry.next == 0)
                    {
                        /*
                        * clear next in CAM bucket if next in entry zero
                        */
                        mem_bitclr_imm((1 << _FC_BUCKET_STATS_NEXT_lsb), cam_read_address + FC_STATS_OVF_wrd);
                    }

                    {
                        /*
                        * Copy CAM bucket values (timeout/timestamp and head pointer)
                        * Unlink current
                        */

                        wr_cam_bucket.value[FC_TIMEOUT_OFV_wrd] = (entry.timeout << 16);
                        mem_write_atomic_with_bytemask(
                        &wr_cam_bucket.value[FC_TIMEOUT_OFV_wrd],
                            cam_read_address + FC_TIMEOUT_OFV_wrd,
                            1 << 2,
                            1 << 2,
                            sig_done,
                            &sig_1,
                            0x04);  // mask 0x00ff0000

                    // wr_cam_bucket.timestamp_ovf = next_entry_ptr->timestamp;
                    wr_cam_bucket.value[FC_TIMESTAMP_OVF_wrd] = entry.timestamp;
                    mem_write_atomic_with_bytemask(
                            &wr_cam_bucket.value[FC_TIMESTAMP_OVF_wrd],
                            cam_read_address + FC_TIMESTAMP_OVF_wrd,
                            1 << 2,
                            1 << 2,
                            sig_done,
                            &sig_2,
                            0x03);  // mask 0x0000ffff

                        wait_for_all(&sig_1, &sig_2);

                        // unlink current
                        // wr_cam_bucket.head_ovf = next_ovf_entry_idx;
                        wr_cam_bucket.value[FC_HEAD_OVF_wrd] = next_ovf_entry_idx;
                        mem_write_atomic_with_bytemask(
                            &wr_cam_bucket.value[FC_HEAD_OVF_wrd],
                            cam_read_address + FC_HEAD_OVF_wrd,
                            1 << 2,
                            1 << 2,
                            ctx_swap,
                            &sig_1,
                            0x07);  // mask 0x00ffffff
                    }


                    return 1;
                }
            }


            /*
            * If we get here, overflow is now empty.
            * Clear out head and stats
            */
            wr_cam_bucket.stats[CAM_ENTRY_OVERFLOW].value = 0x00;
            mem_write_atomic(&wr_cam_bucket.stats[CAM_ENTRY_OVERFLOW].value, cam_read_address + FC_STATS_OVF_wrd, 1 << 2);

            // wr_cam_bucket.head_overflow = 0x00;
            wr_cam_bucket.value[FC_HEAD_OVF_wrd] = 0x00ffffff;
            __mem_bitclr(&wr_cam_bucket.value[FC_HEAD_OVF_wrd], cam_read_address + FC_HEAD_OVF_wrd, 1 << 2, 1 << 2, sig_done, &sig_1);

            wait_for_all(&sig_1);
            return 1;

        }
#endif // FC_ENABLE_OVERFLOW
    return 0;
}

/*
 * This function handles the case where CAM entry 2/3/4 is empty.
 * Check if overflow list is not empty, get an exclusive lock on it. Keep on retrying to get lock.
 * We still hold an exclusive lock on CAM entry 2/3/4. Hash value has not been emptied yet.
 *
 * We get the hash value from entry (SDN-811) and link it into CAM entry 2/3/4. If more entries in the overflow
 * list with the same hash value, also link them into CAM entry 2/3/4 list.

 * If overflow list is empty, clear out overflow head etc.
 *
 * Release lock on overflow list.
 * return 0 if overflow list is empty, else return 1 if overflow was copied to CAM entry
 */
static __gpr uint32_t fc_process_overflow_list_cam_entry_2_3_4(
    __mem __addr40 uint32_t *entry_base_ptr,
    __mem __addr40 uint32_t *cam_read_address,
    __gpr uint32_t ovf_head_idx,        // entry idx stored in overflow head
    __gpr uint32_t cam_entry            // which CAM entry empty CAM_ENTRY_SECOND, ..
    )
{

#ifdef FC_ENABLE_OVERFLOW
    /*
    * Move overflow entries to empty CAM if overflow head non-empty (ovf_head_idx)
    */

    SIGNAL                      sig_1, sig_2;
    __mem __addr40 uint32_t     *ovf_entry_ptr;
    __gpr uint32_t              hash;


    ovf_entry_ptr = (__mem __addr40 uint32_t *)FC_SET_LINKED_ENTRY_ADDRESS(entry_base_ptr, ovf_head_idx);
    {
        /*
        * Read the new hash value from the entry, also save next entry.
        */

        __xread fc_entry_t              ovf_entry;

        mem_read32(&ovf_entry.value[_FC_ENTRY_HASH_HIGH_wrd], ovf_entry_ptr + _FC_ENTRY_HASH_HIGH_wrd, 2 << 2);
        hash = FC_EXTRACT_HASH(ovf_entry.hash_hi, ovf_entry.hash_lo);

        if (hash == 0x00)
        {
#ifdef FC_TIMEOUT_JOURNAL
            fc_timeout_journal_t error_data;
            __gpr uint32_t          hi_addr;
            FC_SET_HI_ADDRESS(journal_ring_num >> 10);

            /*
            * Hash value is empty, release overflow lock.
            * This should never happen!
            */

            error_data.error_code = JOURNAL_REMOVE_OVF_HASH_EMPTY;
            error_data.param_1 = ovf_head_idx;
            error_data.param_2 = cam_entry;
            error_data.param_3 = 0;

            mem_ring_journal_fast((journal_ring_num & 0x3ff), hi_addr, error_data.value);
#endif
            fc_timeout_err_ovf_hash_value_zero[ctx()]++;
            return 0;
        }
    }

    {
        /*
        * Write the values of the overflow entry to the CAM entry 2/3/4.
        * Hash value, timeout, timestamp, stats.
        */
        __xread fc_cam_bucket_t     rd_cam_bucket;
        __xwrite fc_cam_bucket_t    wr_cam_bucket;
        __gpr uint32_t              next_ovf_entry_idx = 0;


        /*
        * Second, third or fourth CAM entry, merely link in the overflow entry into head.
        * Copy timestamp, stats, timeout too.
        */

        if (cam_entry == CAM_ENTRY_SECOND)
        {
            /*
            * Clear CAM entry 2 hash, timeout, timestamp, stats and head
            */
            wr_cam_bucket.value[FC_CAM_HASH_2_wrd] = 0x00ffffff;
            wr_cam_bucket.value[FC_CAM_HASH_3_wrd] = 0x00;
            wr_cam_bucket.value[FC_CAM_HASH_4_wrd] = 0x00;
            wr_cam_bucket.value[FC_TIMEOUT_2_wrd] = 0x0000ff00;
            wr_cam_bucket.value[5] = 0x00;
            wr_cam_bucket.value[FC_TIMESTAMP_2_wrd] = 0xffff0000;
            mem_bitclr(&wr_cam_bucket.value[FC_CAM_HASH_2_wrd], cam_read_address + FC_CAM_HASH_2_wrd, 6 << 2);

            wr_cam_bucket.stats[CAM_ENTRY_SECOND].value = 0xffffffff;
            wr_cam_bucket.stats[CAM_ENTRY_THIRD].value = 0x00;
            wr_cam_bucket.stats[CAM_ENTRY_FOURTH].value = 0x00;
            wr_cam_bucket.stats[CAM_ENTRY_OVERFLOW].value = 0x00;
            wr_cam_bucket.value[FC_HEAD_2_wrd] = 0xffffff00;
            mem_bitclr(&wr_cam_bucket.stats[CAM_ENTRY_SECOND], cam_read_address + FC_STATS_2_wrd, 5 << 2);

            /*
            * Read overflow timestamp and timeout
            */
            mem_read_atomic(&rd_cam_bucket.value[FC_TIMEOUT_OFV_wrd], cam_read_address + FC_TIMEOUT_OFV_wrd, 3 << 2);

            /*
            * Set CAM entry 2 hash, timeout, timestamp, stats and head
            */
            wr_cam_bucket.value[FC_CAM_HASH_2_wrd] = hash;
            wr_cam_bucket.value[FC_CAM_HASH_3_wrd] = 0x00;
            wr_cam_bucket.value[FC_CAM_HASH_4_wrd] = 0x00;
            wr_cam_bucket.value[FC_TIMEOUT_2_wrd] = (rd_cam_bucket.timeout_overflow << 8);
            wr_cam_bucket.value[5] = 0x00;
            wr_cam_bucket.value[FC_TIMESTAMP_2_wrd] = (rd_cam_bucket.timestamp_overflow << 16);
            __mem_bitset(&wr_cam_bucket.value[FC_CAM_HASH_2_wrd], cam_read_address + FC_CAM_HASH_2_wrd, 6 << 2, 6 << 2, sig_done, &sig_1);


            wr_cam_bucket.value[FC_HEAD_2_wrd] =(ovf_head_idx << 8);
            __mem_bitset(&wr_cam_bucket.value[FC_HEAD_2_wrd], cam_read_address + FC_HEAD_2_wrd, 1 << 2, 1 << 2, sig_done, &sig_2);
            wait_for_all(&sig_1, &sig_2);

        }
        else if (cam_entry == CAM_ENTRY_THIRD)
        {
            /*
            * Clear CAM entry 3 hash, timeout, timestamp, stats and head
            */
            wr_cam_bucket.value[FC_CAM_HASH_3_wrd] = 0x00ffffff;
            wr_cam_bucket.value[FC_CAM_HASH_4_wrd] = 0x00;
            wr_cam_bucket.value[FC_TIMEOUT_3_wrd] = 0x00ff;
            wr_cam_bucket.value[5] = 0x00;
            wr_cam_bucket.value[FC_TIMESTAMP_3_wrd] = 0x0000ffff;
            mem_bitclr(&wr_cam_bucket.value[FC_CAM_HASH_3_wrd], cam_read_address + FC_CAM_HASH_3_wrd, 5 << 2);

            wr_cam_bucket.stats[CAM_ENTRY_THIRD].value = 0xffffffff;
            wr_cam_bucket.stats[CAM_ENTRY_FOURTH].value = 0x00;
            wr_cam_bucket.stats[CAM_ENTRY_OVERFLOW].value = 0x00;
            wr_cam_bucket.value[FC_HEAD_3_HI_wrd] = 0x000000ff;
            wr_cam_bucket.value[FC_HEAD_3_LO_wrd] = 0xffff0000;
            mem_bitclr(&wr_cam_bucket.stats[CAM_ENTRY_THIRD], cam_read_address + FC_STATS_3_wrd, 5 << 2);

            /*
            * Read overflow timestamp and timeout
            */
            mem_read_atomic(&rd_cam_bucket.value[FC_TIMEOUT_OFV_wrd], cam_read_address + FC_TIMEOUT_OFV_wrd, 3 << 2);

            /*
            * Set CAM entry 3 hash, timeout, timestamp, stats and head
            */
            wr_cam_bucket.value[FC_CAM_HASH_3_wrd] = hash;
            wr_cam_bucket.value[FC_CAM_HASH_4_wrd] = 0x00;
            wr_cam_bucket.value[FC_TIMEOUT_3_wrd] = rd_cam_bucket.timeout_overflow;
            wr_cam_bucket.value[5] = 0x00;
            wr_cam_bucket.value[FC_TIMESTAMP_3_wrd] = rd_cam_bucket.timestamp_overflow;
            __mem_bitset(&wr_cam_bucket.value[FC_CAM_HASH_3_wrd], cam_read_address + FC_CAM_HASH_3_wrd, 5 << 2, 5 << 2, sig_done, &sig_1);


            wr_cam_bucket.value[FC_HEAD_3_HI_wrd] = ((ovf_head_idx >> 16) & 0xff);
            wr_cam_bucket.value[FC_HEAD_3_LO_wrd] = ((ovf_head_idx & 0xffff) << 16);
            __mem_bitset(&wr_cam_bucket.value[FC_HEAD_3_HI_wrd], cam_read_address + FC_HEAD_3_HI_wrd, 2 << 2, 2 << 2, sig_done, &sig_2);
            wait_for_all(&sig_1, &sig_2);
        }
        else if (cam_entry == CAM_ENTRY_FOURTH)
        {
            /*
            * Clear CAM entry 4 hash, timeout, timestamp, stats and head
            */
            wr_cam_bucket.value[FC_CAM_HASH_4_wrd] = 0x00ffffff;
            wr_cam_bucket.value[4] = 0x00;
            wr_cam_bucket.value[FC_TIMEOUT_4_wrd] = 0xff000000;
            wr_cam_bucket.value[6] = 0x00;
            wr_cam_bucket.value[FC_TIMESTAMP_4_wrd] = 0xffff0000;
            mem_bitclr(&wr_cam_bucket.value[FC_CAM_HASH_4_wrd], cam_read_address + FC_CAM_HASH_4_wrd, 5 << 2);

            wr_cam_bucket.stats[CAM_ENTRY_FOURTH].value = 0xffffffff;
            wr_cam_bucket.stats[CAM_ENTRY_OVERFLOW].value = 0x00;
            wr_cam_bucket.value[13] = 0x00;
            wr_cam_bucket.value[FC_HEAD_4_HI_wrd] = 0x0000ffff;
            wr_cam_bucket.value[FC_HEAD_4_LO_wrd] = 0xff000000;
            mem_bitclr(&wr_cam_bucket.stats[CAM_ENTRY_FOURTH], cam_read_address + FC_STATS_4_wrd, 5 << 2);

            /*
            * Read overflow timestamp and timeout
            */
            mem_read_atomic(&rd_cam_bucket.value[FC_TIMEOUT_OFV_wrd], cam_read_address + FC_TIMEOUT_OFV_wrd,3 << 2);

            /*
            * Set CAM entry 4 hash, timeout, timestamp, stats and head
            */
            wr_cam_bucket.value[FC_CAM_HASH_4_wrd] = hash;
            wr_cam_bucket.value[4] = 0x00;
            wr_cam_bucket.value[FC_TIMEOUT_4_wrd] = (rd_cam_bucket.timeout_overflow << 24);
            wr_cam_bucket.value[6] = 0x00;
            wr_cam_bucket.value[FC_TIMESTAMP_4_wrd] = (rd_cam_bucket.timestamp_overflow << 16);
            __mem_bitset(&wr_cam_bucket.value[FC_CAM_HASH_4_wrd], cam_read_address + FC_CAM_HASH_4_wrd, 5 << 2, 5 << 2, sig_done, &sig_1);


            wr_cam_bucket.value[FC_HEAD_4_HI_wrd] = ((ovf_head_idx >> 8) & 0xffff);
            wr_cam_bucket.value[FC_HEAD_4_LO_wrd] = ((ovf_head_idx & 0xff) << 24);
            __mem_bitset(&wr_cam_bucket.value[FC_HEAD_4_HI_wrd], cam_read_address + FC_HEAD_4_HI_wrd, 2 << 2, 2 << 2, sig_done, &sig_2);
            wait_for_all(&sig_1, &sig_2);
        }


        {
            /*
            * Check if there are entries in the overflow list which have similar hash values
            * than to the one just added to the CAM.
            * These entries have to be linked into the CAM list and removed from the overflow list.
            */
            __xwrite uint32_t               next;
            __xread fc_entry_t              rd_entry;
            __gpr uint32_t                  head_ovf_entry_idx = 0;
            __gpr uint32_t                  next_cam_entry_flag = 1;
            __mem __addr40 fc_entry_t       *head_cam_entry_ptr = (__mem __addr40 fc_entry_t *) ovf_entry_ptr; // first cam entry (copied from overflow list)
            __mem __addr40 fc_entry_t       *new_head_ovf_entry_ptr = 0;

            mem_read32(&rd_entry.value[_FC_ENTRY_NEXT_wrd], ovf_entry_ptr + _FC_ENTRY_NEXT_wrd, 1 << 2);
            next_ovf_entry_idx = rd_entry.next;

            if (rd_entry.next != 0)
            {
                while (next_ovf_entry_idx != 0)
                {
                    /*
                    * while we have more entries in the overflow list, see if we can match any with
                    * the CAM entry (hash) that we have just added.
                    */

                    __gpr uint32_t      entry_idx;
                    __gpr uint32_t      rd_hash = 0;

                    __mem __addr40 uint32_t *next_ovf_ptr = (__mem __addr40 uint32_t *) FC_SET_LINKED_ENTRY_ADDRESS(entry_base_ptr, next_ovf_entry_idx);

                    /*
                    * Read hash and next
                    */
                    __mem_read32(&rd_entry.value[_FC_ENTRY_HASH_HIGH_wrd], next_ovf_ptr + _FC_ENTRY_HASH_HIGH_wrd, 3 << 2, 3 << 2, sig_done, &sig_1);
                    wait_for_all(&sig_1);

                    entry_idx = rd_entry.next; // save xfer before context swap
                    rd_hash = FC_EXTRACT_HASH(rd_entry.hash_hi, rd_entry.hash_lo);

                    if (rd_hash == hash)
                    {
                        /*
                        * If next overflow entry have the same hash as the one we just added to the CAM,
                        * link it into head_cam_entry_ptr list and move onto next overflow entry
                        */
                        next = next_ovf_entry_idx << 8; // head_cam_entry_ptr->next = next_ovf_entry_idx;
                        mem_write8(&next, (__mem __addr40 uint8_t *)head_cam_entry_ptr + (_FC_ENTRY_NEXT_wrd << 2) + 1, 3);

                        head_cam_entry_ptr = (__mem __addr40 fc_entry_t *)next_ovf_ptr;

                        /*
                        * set next in CAM bucket
                        */
                        if (next_cam_entry_flag)
                        {
                            if (cam_entry == CAM_ENTRY_SECOND)
                            {
                                mem_bitset_imm((1 << _FC_BUCKET_STATS_NEXT_lsb), cam_read_address + FC_STATS_2_wrd);
                            }
                            else if (cam_entry == CAM_ENTRY_THIRD)
                            {
                                mem_bitset_imm((1 << _FC_BUCKET_STATS_NEXT_lsb), cam_read_address + FC_STATS_3_wrd);
                            }
                            else if (cam_entry == CAM_ENTRY_FOURTH)
                            {
                                mem_bitset_imm((1 << _FC_BUCKET_STATS_NEXT_lsb), cam_read_address + FC_STATS_4_wrd);
                            }
                            next_cam_entry_flag = 0;
                        }
                    }
                    else
                    {

                        if (new_head_ovf_entry_ptr != 0)
                        {
                            /*
                            * Link entry into overflow list
                            */
                            next = next_ovf_entry_idx << 8;  // new_head_ovf_entry_ptr->next = next_ovf_entry_idx;
                            mem_write8(&next, (__mem __addr40 uint8_t *)new_head_ovf_entry_ptr + (_FC_ENTRY_NEXT_wrd << 2) + 1, 3);
                        }
                        else
                        {
                            /*
                             * First in overflow list .. head
                             */
                            head_ovf_entry_idx = next_ovf_entry_idx;
                        }

                        new_head_ovf_entry_ptr = (__mem __addr40 fc_entry_t *)next_ovf_ptr;
                    }

                    next_ovf_entry_idx = entry_idx;

                } // while

                /*
                 * Set next of cam_entry ptr to 0x00 - end of list
                 */
                next = 0x00;            // head_cam_entry_ptr->next = 0;
                mem_write8(&next, (__mem __addr40 uint8_t *)head_cam_entry_ptr + (_FC_ENTRY_NEXT_wrd << 2) + 1, 3);

                if (new_head_ovf_entry_ptr != 0)
                {
                    // head_ovf_entry_ptr->next = 0;
                    mem_write8(&next, (__mem __addr40 uint8_t *)new_head_ovf_entry_ptr + (_FC_ENTRY_NEXT_wrd << 2) + 1, 3);
                }

                next_ovf_entry_idx = head_ovf_entry_idx;

                if (next_ovf_entry_idx != 0)
                {
                    __xread fc_entry_t      rd_entry;
                    __mem __addr40 uint32_t *next_entry_ptr = (__mem __addr40 uint32_t *) FC_SET_LINKED_ENTRY_ADDRESS(entry_base_ptr, next_ovf_entry_idx);

                    /*
                    * Overflow list is not empty. Copy "second" in list to overflow head.
                    * Only write timestamp, timeout and head fields.
                    * Check next field
                    */
                    __mem_read32(&rd_entry.value[_FC_ENTRY_TIMESTAMP_wrd], next_entry_ptr + _FC_ENTRY_TIMESTAMP_wrd, 2 << 2, 2 << 2, sig_done, &sig_1);
                    wait_for_all(&sig_1);

                    if (rd_entry.next == 0)
                    {
                        /*
                        * clear next in CAM bucket if next in entry zero
                        */
                        mem_bitclr_imm((1 << _FC_BUCKET_STATS_NEXT_lsb), cam_read_address + FC_STATS_OVF_wrd);
                    }

                    /*
                    * Copy CAM bucket values (timeout/timestamp and head pointer)
                    * Unlink current
                    */
                    {
                        // wr_cam_bucket.timeout_ovf = next_entry_ptr->timeout;
                        wr_cam_bucket.value[FC_TIMEOUT_OFV_wrd] = (rd_entry.timeout << 16);
                        mem_write_atomic_with_bytemask(
                            &wr_cam_bucket.value[FC_TIMEOUT_OFV_wrd],
                            cam_read_address + FC_TIMEOUT_OFV_wrd,
                            1 << 2,
                            1 << 2,
                            sig_done,
                            &sig_1,
                            0x04);  // mask 0x00ff0000

                        // wr_cam_bucket.timestamp_ovf = next_entry_ptr->timestamp;
                        wr_cam_bucket.value[FC_TIMESTAMP_OVF_wrd] = rd_entry.timestamp;
                        mem_write_atomic_with_bytemask(
                            &wr_cam_bucket.value[FC_TIMESTAMP_OVF_wrd],
                            cam_read_address + FC_TIMESTAMP_OVF_wrd,
                            1 << 2,
                            1 << 2,
                            sig_done,
                            &sig_2,
                            0x03);  // mask 0x0000ffff

                        wait_for_all(&sig_1, &sig_2);
                        // unlink current
                        // wr_cam_bucket.head_ovf = next_ovf_entry_idx;
                        wr_cam_bucket.value[FC_HEAD_OVF_wrd] = next_ovf_entry_idx;
                        mem_write_atomic_with_bytemask(
                            &wr_cam_bucket.value[FC_HEAD_OVF_wrd],
                            cam_read_address + FC_HEAD_OVF_wrd,
                            1 << 2,
                            1 << 2,
                            ctx_swap,
                            &sig_1,
                            0x07);  // 0x00ffffff 0x0000ffff
                    }

                    return 1;       // success
                }
            } // if
        }

        /*
        * If we get here, overflow is empty.
        * Clear out head and stats (next bit)
        */
        wr_cam_bucket.stats[CAM_ENTRY_OVERFLOW].value = 0x00;
        __mem_write_atomic(&wr_cam_bucket.stats[CAM_ENTRY_OVERFLOW].value, cam_read_address + FC_STATS_OVF_wrd, 1 << 2, 1 << 2, sig_done, &sig_1);

        // wr_cam_bucket.head_overflow = 0x00;
        wr_cam_bucket.value[FC_HEAD_OVF_wrd] = 0x00ffffff;
        __mem_bitclr(&wr_cam_bucket.value[FC_HEAD_OVF_wrd], cam_read_address + FC_HEAD_OVF_wrd, 1 << 2, 1 << 2, sig_done, &sig_2);

        wait_for_all(&sig_1, &sig_2);

        return 1;
    }



#endif // FC_ENABLE_OVERFLOW

    return 0;
}


/*
 * This code handles the CAM entry 1 removal
 * The lock is NOT RELEASED EXCEPT when hash is cleared.
 * EXPECTED that we have an exclusive lock (including shared lock)
 */
__gpr FC_REMOVE_RESULT_CODE fc_remove_flow_cam_entry_1(
    __mem __addr40 uint32_t *bucket_base_ptr,
    __mem __addr40 uint32_t *entry_base_ptr,
    __mem __addr40 uint32_t *payload_base_ptr,
    __gpr uint32_t entry_idx_to_remove,
    __gpr fc_lock_t lock
    )
{
    __gpr FC_REMOVE_RESULT_CODE        result; // = FC_REMOVE_ENTRY_NOT_FOUND;

    /*
     * Expected to hold an exclusive lock
     */
    if (!lock.exclusive_lock)
    {
        result = FC_REMOVE_EXCLUSIVE_LOCK_EXP;
        return result;
    }
    {
        __gpr uint32_t                  next_entry_idx;
        __mem __addr40 fc_entry_t       *current_entry_ptr;
        __mem __addr40 uint32_t          *cam_ptr = (__mem __addr40 uint32_t *)((uint64_t)bucket_base_ptr + FC_EXTRACT_CAM_BUCKET_ADDRESS_FROM_LOCK(lock.value));

        current_entry_ptr = (__mem __addr40 fc_entry_t *)FC_SET_FIRST_ENTRY_ADDRESS(entry_base_ptr, lock.bucket_address << 6);
        next_entry_idx = current_entry_ptr->next;

        if (next_entry_idx == 0)
        {
            /*
            * Must be deleting the head as next entry of head is zero
            */
            if (entry_idx_to_remove != 0)
            {
                /*
                * entry_idx_to_remove must be zero as we should be removing the head.
                */

                return FC_REMOVE_ENTRY_NOT_FOUND;
            }


            {
                /*
                *  If next is null (zero_head_ptr) - there is no list:
                * -zero out the cam entry (hash) and the statistics.
                * -lock is released at end of function.
                * -timeout, timestamp and head will be re-initialised when new cam entry is written.
                */


                __xwrite fc_cam_bucket_t        wr_cam_bucket;
                SIGNAL                          sig_1;


                /*
                * Write the cam entry to memory using atomic engine
                * (CAM instructions are atomic engine so have to use atomic read and write
                * to avoid corruption)
                */

                /*
                * Clear out hash and stats for cam_entry1
                */
                wr_cam_bucket.stats[0].value = 0x00;

                /*
                * Write changes back to CAM bucket
                * Stats is a 32-bit word being written so do not have to mask data on write.
                */
                mem_write_atomic(&wr_cam_bucket.value[FC_STATS_1_wrd], cam_ptr + FC_STATS_1_wrd, 1 << 2);

                result = FC_REMOVE_EXACT_MATCH;


                {
                    __gpr uint32_t                  ovf_processing = 0;
                    __gpr fc_lock_t                 ovf_lock = lock;
#ifdef FC_ENABLE_OVERFLOW


                    ovf_lock.cam_entry = CAM_ENTRY_OVERFLOW;
                    ovf_lock.exclusive_lock = 0;

                    /*
                    * Get shared lock on overflow list and check if overflow head is empty or not.
                    * Even if empty, do not relinguish lock until CAM entry has been cleared.
                    */
                    fc_to_lock_shared_with_bucket_addr(ovf_lock, FC_LOCK_RETRY_FOREVER, bucket_base_ptr);

                    {
                        __xread fc_cam_bucket_t         rd_cam_bucket;

                        mem_read_atomic(&rd_cam_bucket.value[FC_STATS_OVF_wrd], cam_ptr + FC_STATS_OVF_wrd, 4 << 2);

                        if (rd_cam_bucket.head_overflow != 0x00)
                        {
                            /*
                            * Overflow head is not-empty, meaning there is a list.
                            */
                             next_entry_idx = rd_cam_bucket.head_overflow; // reuse next_entry_idx for overflow head idx
                             ovf_processing = 1;
                        }
                    }



                    /*
                    * CAM entry 1 is empty, check overflow list for possible entries to go into this
                    * CAM entry. We still hold the lock of CAM entry 1 and have not cleared out the hash value yet,
                    * otherwise CAM lookup will add another entry if hash is zero.
                    */
                    if (ovf_processing)
                    {
                        /*
                        * Lock overflow.
                        */
                        ovf_lock = fc_to_lock_exclusive_with_bucket_addr(ovf_lock, FC_LOCK_RETRY_FOREVER, bucket_base_ptr);
                        ovf_processing = fc_process_overflow_list_cam_entry1(
                                                bucket_base_ptr,
                                                entry_base_ptr,
                                                payload_base_ptr,
                                                cam_ptr,
                                                lock,
                                                next_entry_idx
                                    );
                    }
#else
                    ovf_processing = 0;
#endif

                    result = FC_REMOVE_EXACT_MATCH_LOCK_CLEARED;

                    if (ovf_processing == 0)
                    {
                        SIGNAL      sig_2;

                        /*
                        * We did not add overflow entries to CAM entry 1.
                        * Clear the hash and release the lock in the specific sequence set out below.
                        * This will prevent race conditions.
                        */

#ifdef FC_TIMEOUT_JOURNAL
                        {
                            __xread uint32_t preclear_lock_xfer;

                            __mem_read_atomic(
                                &preclear_lock_xfer,
                                cam_ptr + FC_CAM_HASH_1_wrd,
                                1 << 2, 1 << 2,
                                sig_done,
                                &sig_2
                            );

                            wait_for_all(&sig_2);

                            if (!(preclear_lock_xfer & (FC_CAM_BUCKET_XLOCK | FC_CAM_BUCKET_SHARED_LOCK)))
                            {
                                /*
                                * This is very bad, we MUST have an exclusive and shared lock but it is not set !!!!
                                */
                                fc_timeout_journal_t error_data;
                                __gpr uint32_t          hi_addr;
                                FC_SET_HI_ADDRESS(journal_ring_num >> 10);

                                error_data.value = 0x00;
                                error_data.error_code = JOURNAL_CLEAR_HASH_LOCK_CORRUPT;
                                error_data.param_1 = preclear_lock_xfer;
                                error_data.param_2 = lock.exclusive_lock;
                                error_data.param_3 = lock.cam_entry;

                                mem_ring_journal_fast((journal_ring_num & 0x3ff), hi_addr, error_data.value);
                            }
                        }
#endif
                        /*
                        * Subtract the shared lock first.
                        */
                        wr_cam_bucket.cam_entry1.value = FC_CAM_BUCKET_SHARED_LOCK;
                        __mem_sub32(
                            &wr_cam_bucket.cam_entry1.value,
                            cam_ptr + FC_CAM_HASH_1_wrd,
                            1 << 2, 1 << 2,
                            sig_done,
                            &sig_1
                        );

                        /*
                        * Clear the exclusive lock and the hash value in one go.
                        * Using wr_cam_bucket.stats[0].value instead of  wr_cam_bucket.cam_entry1.value otherwise
                        * we overwrite above and we have not waited for signal yet.
                        */
                        wr_cam_bucket.stats[0].value = FC_CAM_BUCKET_XLOCK | 0x00ffffff;
                        __mem_bitclr(&wr_cam_bucket.stats[0].value, cam_ptr + FC_CAM_HASH_1_wrd, 1 << 2, 1 << 2, sig_done, &sig_2);

                        wait_for_all(&sig_1, &sig_2);
                    }
                    else
                    {
                        /*
                         * We have copied the overflow entries to CAM entry 1. We do not
                         * need to keep the lock on CAM entry 1 as there is nothing left to process.
                         * (Overflow entries have already been timed if necessary).
                         * Release lock on CAM entry 1.
                         */
                        fc_to_lock_release_with_bucket_addr(
                            lock,
                            (__mem __addr40 uint32_t *)bucket_base_ptr
                        );
                    }
#ifdef FC_ENABLE_OVERFLOW

                    /*
                    * Release overflow lock.
                    * This is where an overflow still exist or the overflow is empty.
                    */
                    fc_to_lock_release_with_bucket_addr(
                        ovf_lock,
                        (__mem __addr40 uint32_t *)bucket_base_ptr
                    );

#endif
                }
            }

            return result;

        }
        else
        {
            /*
            * The 1:1 is the one to be deleted but next is not null (more than one entry in the linked list of cam_entry1).
            * Either the first entry (head) of cam_entry1 has to be deleted or it is another entry in the list.
            * Use the payload_address to check which entry is to be deleted.
            */
            __mem __addr40 fc_entry_t *next_entry_ptr = (__mem __addr40 fc_entry_t *) FC_SET_LINKED_ENTRY_ADDRESS(entry_base_ptr, next_entry_idx);

            if (entry_idx_to_remove == 0)
            {
                __gpr fc_entry_fields_t             entry_fields;

                /*
                    * Payload is the same as the head entry.
                    * We copy the timestamp and timeout from the next entry to the CAM bucket (head1 entry to be removed)
                    * We copy the next entry to the 1:1 of the entry to be removed.
                    * We also put the free cache entry (next_entry_idx/next_entry_ptr) back on the ring.
                    * Lock is released at end of function.
                    */
                result = FC_REMOVE_EXACT_MATCH;

                entry_fields = fc_copy_entry((__mem __addr40 uint32_t *)next_entry_ptr, (__mem __addr40 uint32_t *)current_entry_ptr);

                {
                    SIGNAL                          sig_1;
                    __xwrite fc_cam_bucket_t        wr_cam_bucket;
                    __xread fc_entry_t              entry;
                    __mem __addr40 uint32_t         *entry_tbl_ptr = (__mem __addr40 uint32_t *)next_entry_ptr;


                    /*
                    * Write changes (timeout/timestamp) back for CAM bucket - CAM entry 1
                    * If next field is zero in the entry, then clear next bit field in CAM stats 1 field
                    */
                    {
                        wr_cam_bucket.value[FC_TIMEOUT_1_wrd] = (entry_fields.timeout << 16);
                        mem_write_atomic_with_bytemask(
                            &wr_cam_bucket.value[FC_TIMEOUT_1_wrd],
                            cam_ptr + FC_TIMEOUT_1_wrd,
                            1 << 2,
                            1 << 2,
                            ctx_swap,
                            &sig_1,
                            0x04);  // mask 0x00ff0000


                        wr_cam_bucket.value[FC_TIMESTAMP_1_wrd] = entry_fields.timestamp;
                        mem_write_atomic_with_bytemask(
                            &wr_cam_bucket.value[FC_TIMESTAMP_1_wrd],
                            cam_ptr + FC_TIMESTAMP_1_wrd,
                            1 << 2,
                            1 << 2,
                            ctx_swap,
                            &sig_1,
                            0x03);  // mask 0x0000ffff


                        if (!entry_fields.next)
                        {
                            /*
                            * clear next in CAM bucket if next in entry zero
                            */
                            mem_bitclr_imm((1 << _FC_BUCKET_STATS_NEXT_lsb), cam_ptr + FC_STATS_1_wrd);
                        }
                    }
                }

                /*
                * Copy payload from next entry to entry to be removed.
                * Use payload size of FC_TIMEOUT_PAYLOAD_SIZE
                */
                {
                    __mem __addr40 uint32_t *next_payload_ptr = (__mem __addr40 uint32_t *)FC_SET_LINKED_PAYLOAD_ADDRESS(payload_base_ptr, next_entry_idx);
                    __gpr uint64_t           remove_payload_addr = FC_SET_PAYLOAD_ADDRESS(payload_base_ptr, FC_EXTRACT_CAM_BUCKET_ADDRESS_FROM_LOCK(lock.value));

                    fc_copy_payload((__mem __addr40 uint32_t *)next_payload_ptr, (__mem __addr40 uint32_t *)remove_payload_addr);
                }

                /*
                * Put free cache buffer back on the ring
                */
                {
                    __xwrite uint32_t                   write_xfer = next_entry_idx;
                    uint32_t                            entry_mu;
                    __gpr uint32_t                       hi_addr;

                    if (next_entry_idx != 0x00)
                    {
                        /*
                        * Set the entry address pointer to the correct MU.
                        */
                        FC_NEXT_MEMORY_UNIT(FC_TIMEOUT_LB_EMU, lock.bucket_mu, entry_mu);
                        FC_SET_HI_ADDRESS(entry_mu);

                        if (mem_ring_put(FC_TIMEOUT_FREE_RING, hi_addr, &write_xfer, 1 << 2) == -1)
                        {
                            result = FC_REMOVE_ERROR;
                        }
                    }
                    else
                    {
                        /*
                        * Error
                        */
                        mem_incr32((__mem __addr40 void *)&fc_timeout_err_free_entry_zero[ctx()]);

#ifdef FC_TIMEOUT_JOURNAL
                        {
                            /*
                            * Entry idx cannot be zero
                            */
                            fc_timeout_journal_t error_data;
                            __gpr uint32_t          hi_addr;
                            FC_SET_HI_ADDRESS(journal_ring_num >> 10);

                            error_data.value = 0x00;
                            error_data.error_code = JOURNAL_FREE_ENTRY_IDX_ZERO;
                            error_data.param_1 = next_entry_idx;
                            error_data.param_2 = 1;

                            mem_ring_journal_fast((journal_ring_num & 0x3ff), hi_addr, error_data.value);
                        }
#endif
                    }
                }
            }
            else
            {
                /*
                * Search for matching payload address in entry list.
                * When found:
                *   -prev_entry_ptr is the previous entry in the list
                *   -current_entry_ptr is the one to be deleted
                *   -next_entry_ptr is the entry after current (could be null).
                *
                * Free current entry to ring and link in next with prev dropping current
                */
                __mem __addr40 fc_entry_t *prev_entry_ptr = current_entry_ptr;

                current_entry_ptr = next_entry_ptr;
                next_entry_ptr = 0;

                while (next_entry_idx != entry_idx_to_remove)
                {
                    prev_entry_ptr = current_entry_ptr;
                    next_entry_idx = current_entry_ptr->next;

                    if (next_entry_idx == 0)
                    {
                        /*
                        * We reached the end of the list.
                        */
                        break;
                    }
                    current_entry_ptr = (__declspec(mem, addr40, aligned(32)) fc_entry_t *) FC_SET_LINKED_ENTRY_ADDRESS(entry_base_ptr, next_entry_idx);
                }


                if (next_entry_idx != entry_idx_to_remove)
                {
                    /*
                    * Payload address is not the same and we are at end of list
                    * Error entry not found
                    */

                    return FC_REMOVE_ENTRY_NOT_FOUND;
                }

                /*
                * Setup next pointer if not null
                */
                next_entry_idx = current_entry_ptr->next;

                if (next_entry_idx != 0)
                {
                    next_entry_ptr = (__declspec(mem, addr40, aligned(32)) fc_entry_t *) FC_SET_LINKED_ENTRY_ADDRESS(entry_base_ptr, next_entry_idx);
                }

                /*
                * Delete the entry in the list
                * current_entry_ptr is the entry to be deleted.
                * Link next ptr with prev ptr and free current_entry_ptr.
                */

                result = FC_REMOVE_EXACT_MATCH;

                /*
                * Put free cache buffer back on the ring
                */
                {
                    __xwrite uint32_t                   write_xfer = prev_entry_ptr->next;
                    uint32_t                            entry_mu;
                    __gpr uint32_t                      hi_addr;

                    if (prev_entry_ptr->next != 0x00)
                    {
                        /*
                        * Set the entry address pointer to the correct MU.
                        */
                        FC_NEXT_MEMORY_UNIT(FC_TIMEOUT_LB_EMU, lock.bucket_mu, entry_mu);
                        FC_SET_HI_ADDRESS(entry_mu);

                        if (mem_ring_put(FC_TIMEOUT_FREE_RING, hi_addr, &write_xfer, 1 << 2) == -1)
                        {
                            result = FC_REMOVE_ERROR;
                        }
                    }
                    else
                    {
                        /*
                        * Error
                        */
                        mem_incr32((__mem __addr40 void *)&fc_timeout_err_free_entry_zero[ctx()]);
#ifdef FC_TIMEOUT_JOURNAL
                        {
                            /*
                            * Entry idx cannot be zero
                            */
                            fc_timeout_journal_t error_data;
                            __gpr uint32_t          hi_addr;
                            FC_SET_HI_ADDRESS(journal_ring_num >> 10);

                            error_data.value = 0x00;
                            error_data.error_code = JOURNAL_FREE_ENTRY_IDX_ZERO;
                            error_data.param_1 = prev_entry_ptr->next;
                            error_data.param_2 = 1;

                            mem_ring_journal_fast((journal_ring_num & 0x3ff), hi_addr, error_data.value);
                        }
#endif
                    }
                }

                /*
                * Link previous with next dropping current
                */
                prev_entry_ptr->next = next_entry_idx;

                current_entry_ptr = (__declspec(mem, addr40, aligned(32)) fc_entry_t *)FC_SET_FIRST_ENTRY_ADDRESS(entry_base_ptr, lock.bucket_address << 6);

                if (current_entry_ptr->next == 0x00)
                {
                    /*
                    * Clear the next bit in CAM cam_entry1
                    */
                    mem_bitclr_imm((1 << _FC_BUCKET_STATS_NEXT_lsb), cam_ptr + FC_STATS_1_wrd);
                }
            }
        }

    }

    return result;
}


/*
 * This code handles the CAM entry 2, 3, 4 removal
 */
__gpr FC_REMOVE_RESULT_CODE fc_remove_flow_cam_entry_2_3_4_ovf_head(
    __mem __addr40 uint32_t *bucket_base_ptr,
    __mem __addr40 uint32_t *entry_base_ptr,
    __gpr fc_lock_t lock,
    __gpr uint32_t current_entry_idx
    )
{
    __gpr FC_REMOVE_RESULT_CODE        result = FC_REMOVE_ENTRY_NOT_FOUND;

    /*
     * Expected to hold an exclusive lock
     */
    if (!lock.exclusive_lock)
    {
        result = FC_REMOVE_EXCLUSIVE_LOCK_EXP;
        return result;
    }

    {
        __mem __addr40 fc_entry_t       *current_entry_ptr;
        __mem __addr40 uint32_t         *cam_ptr = (__mem __addr40 uint32_t *)((uint64_t)bucket_base_ptr + FC_EXTRACT_CAM_BUCKET_ADDRESS_FROM_LOCK(lock.value));
        __gpr uint32_t                  next_entry_idx;
        SIGNAL                          sig_1, sig_2;


        /*
         * Setup head pointer using head entry current_entry_idx
         */
        current_entry_ptr = (__declspec(mem, addr40, aligned(32)) fc_entry_t *) FC_SET_LINKED_ENTRY_ADDRESS(entry_base_ptr, current_entry_idx);
        next_entry_idx = current_entry_ptr->next;       // advance to next in list

        /*
        * If next is null (zero_head_ptr) - there is no list
        */
        if (next_entry_idx == 0)
        {
            result = FC_REMOVE_EXACT_MATCH;

            /*
            * We zero out the cam entry (hash) and the statistics.
            * Timeout, timestamp and head will be re-initialised when new cam entry is written.
            * We also put the free cache buffer back on the ring.
            * Lock is released at end of function.
            */
#ifdef FC_ENABLE_OVERFLOW
            if (lock.cam_entry == CAM_ENTRY_OVERFLOW)
            {
                __xwrite fc_cam_bucket_t    wr_cam_bucket;
#ifdef FC_TIMEOUT_JOURNAL
                    {
                        __xread uint32_t preclear_lock_xfer;

                        __mem_read_atomic(
                            &preclear_lock_xfer,
                            cam_ptr + FC_OVF_LOCK_wrd,
                            1 << 2, 1 << 2,
                            sig_done,
                            &sig_1
                        );

                        wait_for_all(&sig_1);

                        if (!(preclear_lock_xfer & (FC_CAM_BUCKET_XLOCK | FC_CAM_BUCKET_SHARED_LOCK)))
                        {
                            /*
                            * This is very bad, we MUST have an exclusive and shared lock but it is not set !!!!
                            */
                            fc_timeout_journal_t error_data;
                            __gpr uint32_t          hi_addr;
                            FC_SET_HI_ADDRESS(journal_ring_num >> 10);

                            error_data.value = 0x00;
                            error_data.error_code = JOURNAL_CLEAR_HASH_LOCK_CORRUPT;
                            error_data.param_1 = preclear_lock_xfer;
                            error_data.param_2 = lock.exclusive_lock;
                            error_data.param_3 = lock.cam_entry;

                            mem_ring_journal_fast((journal_ring_num & 0x3ff), hi_addr, error_data.value);
                        }
                    }
#endif
                /*
                * Clear out head and stats (lock is cleared on return)
                */
                wr_cam_bucket.stats[CAM_ENTRY_OVERFLOW].value = 0x00;
                __mem_write_atomic(&wr_cam_bucket.stats[CAM_ENTRY_OVERFLOW].value, cam_ptr + FC_STATS_OVF_wrd, 1 << 2, 1 << 2, sig_done, &sig_1);

                // wr_cam_bucket.head_overflow = 0x00;
                wr_cam_bucket.value[FC_HEAD_OVF_wrd] = 0x00ffffff;
                __mem_bitclr(&wr_cam_bucket.value[FC_HEAD_OVF_wrd], cam_ptr + FC_HEAD_OVF_wrd, 1 << 2, 1 << 2, sig_done, &sig_2);

                wait_for_all(&sig_1, &sig_2);
            }
            else
#endif
            {
                __xwrite fc_cam_bucket_t        wr_cam_bucket;
                __gpr uint32_t                  ovf_processing = 1;

#ifdef FC_ENABLE_OVERFLOW
                __gpr fc_lock_t                 ovf_lock = lock;

                ovf_lock.cam_entry = CAM_ENTRY_OVERFLOW;
                ovf_lock.exclusive_lock = 0;

                /*
                 * Get shared lock on overflow list and check if overflow head is empty or not.
                 * Even if empty, do not relinguish lock until CAM entry has been cleared.
                 */
                fc_to_lock_shared_with_bucket_addr(ovf_lock, FC_LOCK_RETRY_FOREVER, bucket_base_ptr);

                {
                    __xread fc_cam_bucket_t         rd_cam_bucket;

                    mem_read_atomic(&rd_cam_bucket.value[FC_STATS_OVF_wrd], cam_ptr + FC_STATS_OVF_wrd, 4 << 2);
                    next_entry_idx = rd_cam_bucket.head_overflow; // reuse next_entry_idx for overflow head idx
                }

                if (next_entry_idx == 0x00)
                {
                    /*
                     * Overflow head is empty, meaning no list.
                     * Clear out CAM entry and only release overflow lock after
                     */
                    ovf_processing = 0;
                }
#else
                ovf_processing = 0;
#endif
                result = FC_REMOVE_EXACT_MATCH_LOCK_CLEARED;

                if (lock.cam_entry == CAM_ENTRY_SECOND)
                {
#ifdef FC_ENABLE_OVERFLOW
                    if (ovf_processing == 1)
                    {
                        /*
                        * Lock overflow.
                        */
                        ovf_lock = fc_to_lock_exclusive_with_bucket_addr(ovf_lock, FC_LOCK_RETRY_FOREVER, bucket_base_ptr);

                        /*
                        * CAM entry 2 is empty, check overflow list for possible entries to go into this
                        * CAM entry. We still hold the lock and have not cleared out the hash value yet.
                        */
                        ovf_processing = fc_process_overflow_list_cam_entry_2_3_4(
                                                entry_base_ptr,
                                                cam_ptr,
                                                next_entry_idx,
                                                CAM_ENTRY_SECOND
                                        );
                    }
#endif

                    if (ovf_processing == 0)
                    {
#ifdef FC_TIMEOUT_JOURNAL
                        {
                            __xread uint32_t preclear_lock_xfer;

                            __mem_read_atomic(
                                &preclear_lock_xfer,
                                cam_ptr + FC_CAM_HASH_2_wrd,
                                1 << 2, 1 << 2,
                                sig_done,
                                &sig_1
                            );

                            wait_for_all(&sig_1);

                            if (!(preclear_lock_xfer & (FC_CAM_BUCKET_XLOCK | FC_CAM_BUCKET_SHARED_LOCK)))
                            {
                                /*
                                * This is very bad, we MUST have an exclusive and shared lock but it is not set !!!!
                                */
                                fc_timeout_journal_t error_data;
                                __gpr uint32_t          hi_addr;
                                FC_SET_HI_ADDRESS(journal_ring_num >> 10);

                                error_data.value = 0x00;
                                error_data.error_code = JOURNAL_CLEAR_HASH_LOCK_CORRUPT;
                                error_data.param_1 = preclear_lock_xfer;
                                error_data.param_2 = lock.exclusive_lock;
                                error_data.param_3 = lock.cam_entry;

                                mem_ring_journal_fast((journal_ring_num & 0x3ff), hi_addr, error_data.value);
                            }
                        }
#endif
                        /*
                        * Clear stats and head ptr of CAM bucket if overflow did not overwrite it.
                        */
                        wr_cam_bucket.stats[CAM_ENTRY_SECOND].value = 0x00;
                        __mem_write_atomic(&wr_cam_bucket.stats[CAM_ENTRY_SECOND].value, cam_ptr + FC_STATS_2_wrd, 1 << 2, 1 << 2, sig_done, &sig_1);

                        wr_cam_bucket.value[FC_HEAD_2_wrd] = 0xffffff << 8;
                        __mem_bitclr(&wr_cam_bucket.value[FC_HEAD_2_wrd], cam_ptr + FC_HEAD_2_wrd, 1 << 2, 1 << 2, sig_done, &sig_2);

                        wait_for_all(&sig_1, &sig_2);
                        /*
                        * Subtract the shared lock
                        */
                        wr_cam_bucket.cam_entry2.value = FC_CAM_BUCKET_SHARED_LOCK;
                        __mem_sub32(
                            &wr_cam_bucket.cam_entry2.value,
                            cam_ptr + FC_CAM_HASH_2_wrd,
                            1 << 2, 1 << 2,
                            sig_done,
                            &sig_1
                        );

                        /*
                        * Clear the exclusive lock and the hash
                        * Using wr_cam_bucket.stats[CAM_ENTRY_SECOND].value instead of  wr_cam_bucket.cam_entry2.value otherwise
                        * we overwrite above and we have not waited for signal yet.
                        */
                        wr_cam_bucket.stats[CAM_ENTRY_SECOND].value = FC_CAM_BUCKET_XLOCK | 0x00ffffff;
                        __mem_bitclr(&wr_cam_bucket.stats[CAM_ENTRY_SECOND].value, cam_ptr + FC_CAM_HASH_2_wrd, 1 << 2, 1 << 2, sig_done, &sig_2);

                        wait_for_all(&sig_1, &sig_2);
                    }
                    else
                    {
                        /*
                         * We have copied the overflow entries to CAM entry 2. We do not
                         * need to keep the lock on CAM entry 2 as there is nothing left to process.
                         * (Overflow entries have already been timed if necessary).
                         * Release lock on CAM entry 2.
                         */
                        fc_to_lock_release_with_bucket_addr(
                            lock,
                            (__mem __addr40 uint32_t *)bucket_base_ptr
                        );
                    }
                }
                else if (lock.cam_entry == CAM_ENTRY_THIRD)
                {
#ifdef FC_ENABLE_OVERFLOW
                    if (ovf_processing == 1)
                    {
                        /*
                        * Overflow head is not empty, get exclusive lock on overflow.
                        */
                        ovf_lock = fc_to_lock_exclusive_with_bucket_addr(ovf_lock, FC_LOCK_RETRY_FOREVER, bucket_base_ptr);

                        ovf_processing = fc_process_overflow_list_cam_entry_2_3_4(
                                                entry_base_ptr,
                                                cam_ptr,
                                                next_entry_idx,
                                                CAM_ENTRY_THIRD
                                        );
                    }
#endif
                    if (ovf_processing == 0)
                    {
#ifdef FC_TIMEOUT_JOURNAL
                        {
                            __xread uint32_t preclear_lock_xfer;

                            __mem_read_atomic(
                                &preclear_lock_xfer,
                                cam_ptr + FC_CAM_HASH_3_wrd,
                                1 << 2, 1 << 2,
                                sig_done,
                                &sig_1
                            );

                            wait_for_all(&sig_1);

                            if (!(preclear_lock_xfer & (FC_CAM_BUCKET_XLOCK | FC_CAM_BUCKET_SHARED_LOCK)))
                            {
                                /*
                                * This is very bad, we MUST have an exclusive and shared lock but it is not set !!!!
                                */
                                fc_timeout_journal_t error_data;
                                __gpr uint32_t          hi_addr;
                                FC_SET_HI_ADDRESS(journal_ring_num >> 10);

                                error_data.value = 0x00;
                                error_data.error_code = JOURNAL_CLEAR_HASH_LOCK_CORRUPT;
                                error_data.param_1 = preclear_lock_xfer;
                                error_data.param_2 = lock.exclusive_lock;
                                error_data.param_3 = lock.cam_entry;

                                mem_ring_journal_fast((journal_ring_num & 0x3ff), hi_addr, error_data.value);
                            }
                        }
#endif
                        /*
                        * Overflow entry was not copied to CAm entry.
                        * Clear stats and head ptr of CAM bucket if overflow did not overwrite it.
                        */
                        wr_cam_bucket.stats[CAM_ENTRY_THIRD].value = 0x00;
                        __mem_write_atomic(&wr_cam_bucket.stats[CAM_ENTRY_THIRD].value, cam_ptr + FC_STATS_3_wrd, 1 << 2, 1 << 2, sig_done, &sig_1);

                        wr_cam_bucket.value[FC_HEAD_3_HI_wrd] = 0xff;
                        wr_cam_bucket.value[FC_HEAD_3_LO_wrd] = (0xffff << 16);
                        __mem_bitclr(&wr_cam_bucket.value[FC_HEAD_3_HI_wrd], cam_ptr + FC_HEAD_3_HI_wrd, 2 << 2, 2 << 2, sig_done, &sig_2);

                        wait_for_all(&sig_1, &sig_2);

                        /*
                        * Subtract the shared lock of CAM entry
                        */
                        wr_cam_bucket.cam_entry3.value = FC_CAM_BUCKET_SHARED_LOCK;
                        __mem_sub32(
                            &wr_cam_bucket.cam_entry3.value,
                            cam_ptr + FC_CAM_HASH_3_wrd,
                            1 << 2, 1 << 2,
                            sig_done,
                            &sig_1
                        );

                        /*
                        * Clear the exclusive lock and the hash
                        * Using wr_cam_bucket.stats[CAM_ENTRY_THIRD].value instead of  wr_cam_bucket.cam_entry2.value otherwise
                        * we overwrite above and we have not waited for signal yet.
                        */
                        wr_cam_bucket.stats[CAM_ENTRY_THIRD].value = FC_CAM_BUCKET_XLOCK | 0x00ffffff;
                        __mem_bitclr(&wr_cam_bucket.stats[CAM_ENTRY_THIRD].value, cam_ptr + FC_CAM_HASH_3_wrd, 1 << 2, 1 << 2, sig_done, &sig_2);

                        wait_for_all(&sig_1, &sig_2);

                    }
                    else
                    {
                        /*
                         * We have copied the overflow entries to CAM entry 3. We do not
                         * need to keep the lock on CAM entry 3 as there is nothing left to process.
                         * (Overflow entries have already been timed if necessary).
                         * Release lock on CAM entry 3.
                         */
                        fc_to_lock_release_with_bucket_addr(
                            lock,
                            (__mem __addr40 uint32_t *)bucket_base_ptr
                        );
                    }

                }
                else if (lock.cam_entry == CAM_ENTRY_FOURTH)
                {
#ifdef FC_ENABLE_OVERFLOW
                    if (ovf_processing == 1)
                    {
                        /*
                        * Lock overflow.
                        */
                        ovf_lock = fc_to_lock_exclusive_with_bucket_addr(ovf_lock, FC_LOCK_RETRY_FOREVER, bucket_base_ptr);
                        /*
                        * CAM entry 4 is empty, check overflow list for possible entries to go into this
                        * CAM entry. We still hold the lock and have not cleared out the hash value yet.
                        */

                        ovf_processing = fc_process_overflow_list_cam_entry_2_3_4(
                                                entry_base_ptr,
                                                cam_ptr,
                                                next_entry_idx,
                                                CAM_ENTRY_FOURTH
                                        );
                    }
#endif
                    if (ovf_processing == 0)
                    {
#ifdef FC_TIMEOUT_JOURNAL
                            {
                                __xread uint32_t preclear_lock_xfer;

                                __mem_read_atomic(
                                    &preclear_lock_xfer,
                                    cam_ptr + FC_CAM_HASH_4_wrd,
                                    1 << 2, 1 << 2,
                                    sig_done,
                                    &sig_1
                                );

                                wait_for_all(&sig_1);

                                if (!(preclear_lock_xfer & (FC_CAM_BUCKET_XLOCK | FC_CAM_BUCKET_SHARED_LOCK)))
                                {
                                    /*
                                    * This is very bad, we MUST have an exclusive and shared lock but it is not set !!!!
                                    */
                                    fc_timeout_journal_t error_data;
                                    __gpr uint32_t          hi_addr;
                                    FC_SET_HI_ADDRESS(journal_ring_num >> 10);

                                    error_data.value = 0x00;
                                    error_data.error_code = JOURNAL_CLEAR_HASH_LOCK_CORRUPT;
                                    error_data.param_1 = preclear_lock_xfer;
                                    error_data.param_2 = lock.exclusive_lock;
                                    error_data.param_3 = lock.cam_entry;

                                    mem_ring_journal_fast((journal_ring_num & 0x3ff), hi_addr, error_data.value);
                                }
                            }
#endif
                        /*
                        * Clear stats and head ptr of CAM bucket if overflow did not overwrite it.
                        */
                        wr_cam_bucket.stats[CAM_ENTRY_FOURTH].value = 0x00;
                        __mem_write_atomic(&wr_cam_bucket.stats[CAM_ENTRY_FOURTH].value, cam_ptr + FC_STATS_4_wrd, 1 << 2, 1 << 2, sig_done, &sig_1);


                        wr_cam_bucket.value[FC_HEAD_4_HI_wrd] = 0xffff;
                        wr_cam_bucket.value[FC_HEAD_4_LO_wrd] = (0xff << 24);
                        __mem_bitclr(&wr_cam_bucket.value[FC_HEAD_4_HI_wrd], cam_ptr + FC_HEAD_4_HI_wrd, 2 << 2, 2 << 2, sig_done, &sig_2);

                        wait_for_all(&sig_1, &sig_2);

                        /*
                        * Subtract the shared lock of CAM entry
                        */
                        wr_cam_bucket.cam_entry4.value = FC_CAM_BUCKET_SHARED_LOCK;
                        __mem_sub32(
                            &wr_cam_bucket.cam_entry4.value,
                            cam_ptr + FC_CAM_HASH_4_wrd,
                            1 << 2, 1 << 2,
                            sig_done,
                            &sig_1
                        );

                        /*
                        * Clear the exclusive lock and the hash
                        * Using wr_cam_bucket.stats[CAM_ENTRY_FOURTH].value instead of  wr_cam_bucket.cam_entry2.value otherwise
                        * we overwrite above and we have not waited for signal yet.
                        */
                        wr_cam_bucket.stats[CAM_ENTRY_FOURTH].value = FC_CAM_BUCKET_XLOCK | 0x00ffffff;
                        __mem_bitclr(&wr_cam_bucket.stats[CAM_ENTRY_FOURTH].value, cam_ptr + FC_CAM_HASH_4_wrd, 1 << 2, 1 << 2, sig_done, &sig_2);

                        wait_for_all(&sig_1, &sig_2);
                    }
                    else
                    {
                        /*
                         * We have copied the overflow entries to CAM entry 4. We do not
                         * need to keep the lock on CAM entry 4 as there is nothing left to process.
                         * (Overflow entries have already been timed if necessary).
                         * Release lock on CAM entry 4.
                         */
                        fc_to_lock_release_with_bucket_addr(
                            lock,
                            (__mem __addr40 uint32_t *)bucket_base_ptr
                        );
                    }
                }
#ifdef FC_ENABLE_OVERFLOW
                /*
                * Release overflow lock.
                * At this time if overflow entry was copied to CAM entry, then overflow lock can be cleared.
                * If overflow was not copied and CAM entry has been cleared, the CAM entry lock would have been released.
                */
                fc_to_lock_release_with_bucket_addr(
                    ovf_lock,
                    (__mem __addr40 uint32_t *)bucket_base_ptr
                );
#endif

            }

            /*
            * Put free cache buffer back on the ring
            */
            {
                __xwrite uint32_t                   write_xfer = current_entry_idx;
                uint32_t                            entry_mu;
                __gpr uint32_t                          hi_addr;

                if (current_entry_idx != 0x00)
                {
                    /*
                    * Set the entry address pointer to the correct MU.
                    */
                    FC_NEXT_MEMORY_UNIT(FC_TIMEOUT_LB_EMU, lock.bucket_mu, entry_mu);
                    FC_SET_HI_ADDRESS(entry_mu);

                    if (mem_ring_put(FC_TIMEOUT_FREE_RING, hi_addr, &write_xfer, 1 << 2) == -1)
                    {
                        result = FC_REMOVE_ERROR;
                    }
                }
                else
                {
                    /*
                    * Error
                    */
                        mem_incr32((__mem __addr40 void *)&fc_timeout_err_free_entry_zero[ctx()]);
#ifdef FC_TIMEOUT_JOURNAL
                    {
                        /*
                        * Entry idx cannot be zero
                        */
                        fc_timeout_journal_t error_data;
                        __gpr uint32_t          hi_addr;
                        FC_SET_HI_ADDRESS(journal_ring_num >> 10);

                        error_data.value = 0x00;
                        error_data.error_code = JOURNAL_FREE_ENTRY_IDX_ZERO;
                        error_data.param_1 = current_entry_idx;
                        error_data.param_2 = lock.cam_entry;

                        mem_ring_journal_fast((journal_ring_num & 0x3ff), hi_addr, error_data.value);
                    }
#endif
                }
            }

            return result;
        }
        else
        {
            __mem __addr40 fc_entry_t *next_entry_ptr = (__mem __addr40 fc_entry_t *) FC_SET_LINKED_ENTRY_ADDRESS(entry_base_ptr, next_entry_idx);

            {
                /*
                * Head entry is the one to be removed
                *
                * We copy the timestamp and timeout from the next entry to the CAM bucket (head entry to be removed)
                * We copy the next entry to the headX (where X=2, 3, 4) of the entry to be removed.
                * We also put the free cache entry (next_entry_idx/next_entry_ptr) back on the ring.
                * Lock is released at end of function.
                * Payload stays unchanged as payload buffer will align with entry buffer.
                */
                result = FC_REMOVE_EXACT_MATCH;

                {
                    __xwrite fc_cam_bucket_t    wr_cam_bucket;
                    __xread uint32_t            value[2];
                    __lmem fc_entry_t           entry;
                    __mem __addr40 uint32_t     *entry_tbl_ptr = (__mem __addr40 uint32_t *)next_entry_ptr;
                    SIGNAL                      sig_3, sig_4;

                    /*
                    * Only write timestamp, timeout and head fields.
                    * Check next field
                    */
                    __mem_read32(value, entry_tbl_ptr + _FC_ENTRY_TIMESTAMP_wrd, 2 << 2, 2 << 2, sig_done, &sig_1);
                    wait_for_all(&sig_1);

                    /*
                     * Running out of xfers, copy to lmem to access structure
                     */
                    entry.value[_FC_ENTRY_TIMESTAMP_wrd] = value[0];
                    entry.value[_FC_ENTRY_TIMESTAMP_wrd + 1] = value[1];


                    /*
                    * Copy CAM bucket values (timeout/timestamp and head pointer)
                    * Unlink current
                    * Do not ctx_swap as we are reading from entry in xfers
                    */
                    if (lock.cam_entry == CAM_ENTRY_SECOND)
                    {
                        // wr_cam_bucket.timeout2 = next_entry_ptr->timeout;
                        wr_cam_bucket.value[FC_TIMEOUT_2_wrd] = (entry.timeout << 8);
                        mem_write_atomic_with_bytemask(
                            &wr_cam_bucket.value[FC_TIMEOUT_2_wrd],
                            cam_ptr + FC_TIMEOUT_2_wrd,
                            1 << 2,
                            1 << 2,
                            sig_done,
                            &sig_1,
                            0x02);  // mask 0x0000ff00

                        // wr_cam_bucket.timestamp2 = next_entry_ptr->timestamp;
                        wr_cam_bucket.value[FC_TIMESTAMP_2_wrd] = (entry.timestamp << 16);
                        mem_write_atomic_with_bytemask(
                            &wr_cam_bucket.value[FC_TIMESTAMP_2_wrd],
                            cam_ptr + FC_TIMESTAMP_2_wrd,
                            1 << 2,
                            1 << 2,
                            sig_done,
                            &sig_2,
                            0x0c);  // mask 0xffff0000

                        // unlink current
                        // wr_cam_bucket.head2 = next_entry_idx;
                        wr_cam_bucket.value[FC_HEAD_2_wrd] = (next_entry_idx << 8);
                        mem_write_atomic_with_bytemask(
                            &wr_cam_bucket.value[FC_HEAD_2_wrd],
                            cam_ptr + FC_HEAD_2_wrd,
                            1 << 2,
                            1 << 2,
                            sig_done,
                            &sig_3,
                            0x0e);  // mask 0xffffff00

                        wait_for_all(&sig_1, &sig_2, &sig_3);

                        if (!entry.next)
                        {
                            /*
                            * clear next in CAM bucket if next in entry zero
                            */
                            mem_bitclr_imm(1 << _FC_BUCKET_STATS_NEXT_lsb, cam_ptr + FC_STATS_2_wrd);
                        }
                    }
                    else if (lock.cam_entry == CAM_ENTRY_THIRD)
                    {
                        // cam_bucket_ptr->timeout3 = next_entry_ptr->timeout;
                        wr_cam_bucket.value[FC_TIMEOUT_3_wrd] = entry.timeout;
                        mem_write_atomic_with_bytemask(
                            &wr_cam_bucket.value[FC_TIMEOUT_3_wrd],
                            cam_ptr + FC_TIMEOUT_3_wrd,
                            1 << 2,
                            1 << 2,
                            sig_done,
                            &sig_1,
                            0x01);  // mask 0x000000ff

                        // cam_bucket_ptr->timestamp3 = next_entry_ptr->timestamp;
                        wr_cam_bucket.value[FC_TIMESTAMP_3_wrd] = entry.timestamp;
                        mem_write_atomic_with_bytemask(
                            &wr_cam_bucket.value[FC_TIMESTAMP_3_wrd],
                            cam_ptr + FC_TIMESTAMP_3_wrd,
                            1 << 2,
                            1 << 2,
                            sig_done,
                            &sig_2,
                            0x03);  // mask 0x0000ffff

                        // cam_bucket_ptr->head3_hi = next_entry_idx >> 16;
                        wr_cam_bucket.value[FC_HEAD_3_HI_wrd] = ((next_entry_idx >> 16) & 0xff);
                        mem_write_atomic_with_bytemask(
                            &wr_cam_bucket.value[FC_HEAD_3_HI_wrd],
                            cam_ptr + FC_HEAD_3_HI_wrd,
                            1 << 2,
                            1 << 2,
                            sig_done,
                            &sig_3,
                            0x01);  // mask 0x000000ff

                        // cam_bucket_ptr->head3_lo = next_entry_idx & 0xffff;
                        wr_cam_bucket.value[FC_HEAD_3_LO_wrd] = ((next_entry_idx & 0xffff) << 16);
                        mem_write_atomic_with_bytemask(
                            &wr_cam_bucket.value[FC_HEAD_3_LO_wrd],
                            cam_ptr + FC_HEAD_3_LO_wrd,
                            1 << 2,
                            1 << 2,
                            sig_done,
                            &sig_4,
                            0x0c);  // mask 0xffff0000

                        wait_for_all(&sig_1, &sig_2, &sig_3, &sig_4);

                        if (!entry.next)
                        {
                            /*
                            * clear next in CAM bucket if next in entry zero
                            */
                            mem_bitclr_imm((1 << _FC_BUCKET_STATS_NEXT_lsb), cam_ptr + FC_STATS_3_wrd);
                        }
                    }
                    else if (lock.cam_entry == CAM_ENTRY_FOURTH)
                    {
                        /*
                        * CAM entry 4
                        */

                        // cam_bucket_ptr->timeout4 = next_entry_ptr->timeout;
                        wr_cam_bucket.value[FC_TIMEOUT_4_wrd] = (entry.timeout << 24);
                        mem_write_atomic_with_bytemask(
                            &wr_cam_bucket.value[FC_TIMEOUT_4_wrd],
                            cam_ptr + FC_TIMEOUT_4_wrd,
                            1 << 2,
                            1 << 2,
                            sig_done,
                            &sig_1,
                            0x08);  // mask 0xff000000

                        // cam_bucket_ptr->timestamp4 = next_entry_ptr->timestamp;
                        wr_cam_bucket.value[FC_TIMESTAMP_4_wrd] = (entry.timestamp << 16);
                        mem_write_atomic_with_bytemask(
                            &wr_cam_bucket.value[FC_TIMESTAMP_4_wrd],
                            cam_ptr + FC_TIMESTAMP_4_wrd,
                            1 << 2,
                            1 << 2,
                            sig_done,
                            &sig_2,
                            0x0c);  // mask 0xffff0000

                        // cam_bucket_ptr->head4_hi = next_entry_idx >> 8;
                        wr_cam_bucket.value[FC_HEAD_4_HI_wrd] = ((next_entry_idx >> 8) & 0xffff);
                        mem_write_atomic_with_bytemask(
                            &wr_cam_bucket.value[FC_HEAD_4_HI_wrd],
                            cam_ptr + FC_HEAD_4_HI_wrd,
                            1 << 2,
                            1 << 2,
                            sig_done,
                            &sig_3,
                            0x03);  // mask 0x0000ffff

                        // cam_bucket_ptr->head4_lo = next_entry_idx & 0xff;
                        wr_cam_bucket.value[FC_HEAD_4_LO_wrd] = ((next_entry_idx & 0xff) << 24);
                        mem_write_atomic_with_bytemask(
                            &wr_cam_bucket.value[FC_HEAD_4_LO_wrd],
                            cam_ptr + FC_HEAD_4_LO_wrd,
                            1 << 2,
                            1 << 2,
                            sig_done,
                            &sig_4,
                            0x08);  // mask 0xff000000

                        wait_for_all(&sig_1, &sig_2, &sig_3, &sig_4);

                        if (!entry.next)
                        {
                            /*
                            * clear next in CAM bucket if next in entry zero
                            */
                            mem_bitclr_imm((1 << _FC_BUCKET_STATS_NEXT_lsb), cam_ptr + FC_STATS_4_wrd);
                        }
                    }
#ifdef FC_ENABLE_OVERFLOW
                    else if (lock.cam_entry == CAM_ENTRY_OVERFLOW)
                    {

                        // wr_cam_bucket.timeout_ovf = next_entry_ptr->timeout;
                        wr_cam_bucket.value[FC_TIMEOUT_OFV_wrd] = (entry.timeout << 16);
                        mem_write_atomic_with_bytemask(
                            &wr_cam_bucket.value[FC_TIMEOUT_OFV_wrd],
                            cam_ptr + FC_TIMEOUT_OFV_wrd,
                            1 << 2,
                            1 << 2,
                            sig_done,
                            &sig_1,
                            0x04);  // mask 0x00ff0000

                        // wr_cam_bucket.timestamp_ovf = next_entry_ptr->timestamp;
                        wr_cam_bucket.value[FC_TIMESTAMP_OVF_wrd] = entry.timestamp;
                        mem_write_atomic_with_bytemask(
                            &wr_cam_bucket.value[FC_TIMESTAMP_OVF_wrd],
                            cam_ptr + FC_TIMESTAMP_OVF_wrd,
                            1 << 2,
                            1 << 2,
                            sig_done,
                            &sig_2,
                            0x03);  // mask 0x0000ffff

                        // unlink current
                        // wr_cam_bucket.head_ovf = next_entry_idx;
                        wr_cam_bucket.value[FC_HEAD_OVF_wrd] = next_entry_idx;
                        mem_write_atomic_with_bytemask(
                            &wr_cam_bucket.value[FC_HEAD_OVF_wrd],
                            cam_ptr + FC_HEAD_OVF_wrd,
                            1 << 2,
                            1 << 2,
                            sig_done,
                            &sig_3,
                            0x07);  // mask 0x00ffffff

                        wait_for_all(&sig_1, &sig_2, &sig_3);

                        if (!entry.next)
                        {
                            /*
                            * clear next in CAM bucket if next in entry zero
                            */
                            mem_bitclr_imm((1 << _FC_BUCKET_STATS_NEXT_lsb), cam_ptr + FC_STATS_OVF_wrd);
                        }
                    }
#endif
                }

                /*
                * Put free cache buffer back on the ring
                */
                {
                    __xwrite uint32_t                   write_xfer = current_entry_idx;
                    uint32_t                            entry_mu;
                    __gpr uint32_t                      hi_addr;

                    if (current_entry_idx != 0x00)
                    {
                        /*
                        * Set the entry address pointer to the correct MU.
                        */
                        FC_NEXT_MEMORY_UNIT(FC_TIMEOUT_LB_EMU, lock.bucket_mu, entry_mu);
                        FC_SET_HI_ADDRESS(entry_mu);

                        if (mem_ring_put(FC_TIMEOUT_FREE_RING, hi_addr, &write_xfer, 1 << 2) == -1)
                        {
                            result = FC_REMOVE_ERROR;
                        }
                    }
                    else
                    {
                        /*
                        * Error
                        */
                        mem_incr32((__mem __addr40 void *)&fc_timeout_err_free_entry_zero[ctx()]);
#ifdef FC_TIMEOUT_JOURNAL
                        {
                            /*
                            * Entry idx cannot be zero
                            */
                            fc_timeout_journal_t error_data;
                            __gpr uint32_t          hi_addr;
                            FC_SET_HI_ADDRESS(journal_ring_num >> 10);

                            error_data.value = 0x00;
                            error_data.error_code = JOURNAL_FREE_ENTRY_IDX_ZERO;
                            error_data.param_1 = current_entry_idx;
                            error_data.param_2 = lock.cam_entry;

                            mem_ring_journal_fast((journal_ring_num & 0x3ff), hi_addr, error_data.value);
                        }
#endif
                    }
                }
            }
        }
    }

    return result;
}


/*
 * This code handles the CAM entry 2, 3, 4 removal
 */
__gpr FC_REMOVE_RESULT_CODE fc_remove_flow_cam_entry_2_3_4_ovf(
    __mem __addr40 uint32_t *bucket_base_ptr,
    __mem __addr40 uint32_t *entry_base_ptr,
    __gpr uint32_t      entry_idx_to_remove,
    __gpr fc_lock_t lock
    )
{
    __gpr FC_REMOVE_RESULT_CODE        result = FC_REMOVE_ENTRY_NOT_FOUND;

    /*
     * Expected to hold an exclusive lock
     */
    if (!lock.exclusive_lock)
    {
        result = FC_REMOVE_EXCLUSIVE_LOCK_EXP;
        return result;
    }

    {
        __gpr uint32_t                          next_entry_idx;
        __gpr uint32_t                          bucket_addr = FC_EXTRACT_CAM_BUCKET_ADDRESS_FROM_LOCK(lock.value);
        __mem __addr40 fc_entry_t               *current_entry_ptr;
        __mem __addr40 fc_entry_t               *first_entry_ptr;
        __mem __addr40 uint32_t                 *cam_ptr = (__mem __addr40 uint32_t *)((uint64_t)bucket_base_ptr + bucket_addr);
        SIGNAL                                  sig_1, sig_2;
        __xread fc_cam_bucket_t                 rd_cam_bucket;

        /*
        * Read the cam entry from memory using atomic engine
        * (CAM instructions are atomic engine so have to use atomic read and write
        * to avoid corruption)
        */
        mem_read_atomic(&rd_cam_bucket.value[FC_HEAD_wrd], cam_ptr + FC_HEAD_wrd, 3 << 2);

        /*
        * Read the head pointer (entry idx) from the CAM xfer.
        */
        FC_EXTRACT_HEAD_PTR_FROM_CAM_BUCKET_XFERS(rd_cam_bucket, lock.cam_entry, next_entry_idx);

        current_entry_ptr = (__declspec(mem, addr40, aligned(32)) fc_entry_t *) FC_SET_LINKED_ENTRY_ADDRESS(entry_base_ptr, next_entry_idx);
        first_entry_ptr = current_entry_ptr;
        next_entry_idx = first_entry_ptr->next;

        /*
        * If next is not null (zero_head_ptr) - there is a list
        */
        if (next_entry_idx != 0)
        {
            __mem __addr40 fc_entry_t *next_entry_ptr = (__mem __addr40 fc_entry_t *) FC_SET_LINKED_ENTRY_ADDRESS(entry_base_ptr, next_entry_idx);

            first_entry_ptr = current_entry_ptr;

            {
                /*
                * Search for matching payload address in entry list.
                *
                * When found:
                *   -prev_entry_ptr is the previous entry in the list
                *   -current_entry_ptr is the one to be deleted
                *   -next_entry_ptr is the entry after current (could be null).
                *
                * Free current entry to ring and link in next with prev dropping current
                */
                __mem __addr40 fc_entry_t *prev_entry_ptr = current_entry_ptr;

                current_entry_ptr = next_entry_ptr;
                next_entry_ptr = 0;

                while (entry_idx_to_remove != next_entry_idx)
                {
                    prev_entry_ptr = current_entry_ptr;
                    next_entry_idx = current_entry_ptr->next;

                    if (next_entry_idx == 0)
                    {
                        /*
                        * We reached the end of the list.
                        */
                        break;
                    }
                    current_entry_ptr = (__declspec(mem, addr40, aligned(32)) fc_entry_t *) FC_SET_LINKED_ENTRY_ADDRESS(entry_base_ptr, next_entry_idx);
                }


                if (entry_idx_to_remove != next_entry_idx)
                {
                    /*
                    * Payload address is not the same and we are at end of list
                    * Error entry not found
                    */
                    return FC_REMOVE_ENTRY_NOT_FOUND;
                }

                /*
                * Setup next pointer if not null
                */
                next_entry_idx = current_entry_ptr->next;

                if (next_entry_idx != 0)
                {
                    next_entry_ptr = (__declspec(mem, addr40, aligned(32)) fc_entry_t *) FC_SET_LINKED_ENTRY_ADDRESS(entry_base_ptr, next_entry_idx);
                }

                /*
                * Delete the entry in the list
                * current_entry_ptr is the entry to be deleted.
                * Link next ptr with prev ptr and free current_entry_ptr.
                */

                result = FC_REMOVE_EXACT_MATCH;

                /*
                * Put free cache buffer back on the ring
                */
                {
                    __xwrite uint32_t                   write_xfer = prev_entry_ptr->next;
                    uint32_t                            entry_mu;
                    __gpr uint32_t                      hi_addr;

                    if (prev_entry_ptr->next != 0x00)
                    {
                        /*
                        * Set the entry address pointer to the correct MU.
                        */
                        FC_NEXT_MEMORY_UNIT(FC_TIMEOUT_LB_EMU, lock.bucket_mu, entry_mu);
                        FC_SET_HI_ADDRESS(entry_mu);

                        if (mem_ring_put(FC_TIMEOUT_FREE_RING, hi_addr, &write_xfer, 1 << 2) == -1)
                        {
                            result = FC_REMOVE_ERROR;
                        }
                    }
                    else
                    {
                        /*
                        * Error
                        */
                        mem_incr32((__mem __addr40 void *)&fc_timeout_err_free_entry_zero[ctx()]);
#ifdef FC_TIMEOUT_JOURNAL
                        {
                            /*
                            * Entry idx cannot be zero
                            */
                            fc_timeout_journal_t error_data;
                            __gpr uint32_t          hi_addr;
                            FC_SET_HI_ADDRESS(journal_ring_num >> 10);

                            error_data.value = 0x00;
                            error_data.error_code = JOURNAL_FREE_ENTRY_IDX_ZERO;
                            error_data.param_1 = prev_entry_ptr->next;
                            error_data.param_2 = lock.cam_entry;

                            mem_ring_journal_fast((journal_ring_num & 0x3ff), hi_addr, error_data.value);
                        }
#endif
                    }
                }

                /*
                * Link previous with next, dropping current entry
                */
                prev_entry_ptr->next = next_entry_idx;


                if (first_entry_ptr->next == 0x00)
                {
                    /*
                    * Clear the next bit in cam_entry
                    */
                    if (lock.cam_entry == CAM_ENTRY_SECOND)
                    {
                        mem_bitclr_imm((1 << _FC_BUCKET_STATS_NEXT_lsb), cam_ptr + FC_STATS_2_wrd);
                    }
                    else if (lock.cam_entry == CAM_ENTRY_THIRD)
                    {
                        mem_bitclr_imm((1 << _FC_BUCKET_STATS_NEXT_lsb), cam_ptr + FC_STATS_3_wrd);
                    }
                    else if (lock.cam_entry == CAM_ENTRY_FOURTH)
                    {
                        mem_bitclr_imm((1 << _FC_BUCKET_STATS_NEXT_lsb), cam_ptr + FC_STATS_4_wrd);
                    }
#ifdef FC_ENABLE_OVERFLOW
                    else
                    {
                        mem_bitclr_imm((1 << _FC_BUCKET_STATS_NEXT_lsb), cam_ptr + FC_STATS_OVF_wrd);
                    }
#endif
                }
            }
        }
    }

    return result;
}



#endif
