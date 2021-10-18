/*
 * Copyright (C) 2014-2017,  Netronome Systems, Inc.  All rights reserved.
 *
 * @file        flow_cache_timeout.c
 * @brief       Functionality to timeout CAM entries depending on configured
 *              timeout and timestamp last used.
 */


#ifndef _FLOW_CACHE_TIMEOUT_C_
#define _FLOW_CACHE_TIMEOUT_C_

#include <assert.h>
#include <nfp.h>
#include <stdint.h>
#include <types.h>
#include <nfp/mem_atomic.h>
#include <nfp/mem_bulk.h>
#include <nfp/mem_ring.h>
#include <nfp/me.h>
#include <nfp6000/nfp_me.h>
#include <cluster_target_c.h>
#include <system_init_c.h>

#include "flow_cache_global_c.h"
#include "flow_cache_remove_c.h"
#include "flow_cache_timeout_lock_c.h"
#include "flow_cache_timeout_c.h"
#include "flow_cache_timestamp_c.h"


#ifdef FC_TIMEOUT_JOURNAL
    #include "flow_cache_timeout_journal_c.h"
#endif

#ifndef FC_TIMEOUT_LB_EMU
    #error "FC_TIMEOUT_LB_EMU must be defined"
#endif


#ifndef FC_TIMEOUT_EMU
    #warning "FC_TIMEOUT_EMU not set. Defaulting to EMU 0"
    #define FC_TIMEOUT_EMU 0
#endif


#ifndef TIMEOUT_CONTEXTS
    #define TIMEOUT_CONTEXTS    8
#endif

#ifndef TIMEOUT_CONTEXTS
    #error "TIMEOUT_CONTEXTS must be defined. Valid values are 1 <= TIMEOUT_CONTEXTS <= 8"
#endif


/*
 * Entry expired?
 *  add timeout (TO) and timestamp (TS) and compare to the current time (CT).
 *  Remember that due to clocking on different MEs, TS might be a bit more than CT.
 *  0xf000 is the magic number which indicates that if timestamp differs more wrapping has occurred
 *
 * if  CT - (TS + TO) >= 0  then the entry has expired
 * if  CT - (TS + TO) < -0xf000 then it indicates wrapping:
 * CT + 0x10000 - (TS + TO) >= 0 then it has expired
 */
#ifndef TIMEOUT_ENTRY_EXPIRED

    #define TIMEOUT_ENTRY_EXPIRED(timeout, timestamp, current, expired)         \
    {                                                                           \
        __gpr int32_t result = current - timeout - timestamp;                   \
        expired = 0;                                                            \
        if (result >= 0) expired = 1;                                           \
        else if (result < -0xf000)                                              \
        {                                                                       \
            if (result + 0x10000 >= 0) expired = 1;                             \
            else expired = 0;                                                   \
        }                                                                       \
    }
#endif


__intrinsic
unsigned long long timeout_get_64bit_timestamp(void)
{
    unsigned lo, hi;

    __asm __attribute(LITERAL_ASM) {
        local_csr_rd[local_csr_timestamp_low]
        immed[lo, 0]
        local_csr_rd[local_csr_timestamp_high]
        immed[hi, 0]
    }
    return ((unsigned long long)hi << 32) | lo;
}

/*
 * Import flow cache data structures:
 * CAM buckets
 * Key entries
 * Payload entries
 */
__asm .alloc_mem _FC_EMU_0_BUCKETS_BASE emem0+_FC_BUCKETS_ADDR global _FC_BUCKETS_ALLOC_SIZE 256 addr40
volatile __declspec(emem0, aligned(256), shared, import) fc_cam_bucket_t *fc_emu_0_buckets_ptr;

__asm .alloc_mem _FC_EMU_0_ENTRIES_BASE emem0+_FC_ENTRIES_ADDR global _FC_ENTRIES_ALLOC_SIZE 256
volatile __declspec(emem0, aligned(256), shared, import) fc_entry_t *fc_emu_0_entries_ptr;

__asm .alloc_mem _FC_EMU_0_PAYLOADS_BASE emem0+_FC_PAYLOADS_ADDR global _FC_PAYLOADS_ALLOC_SIZE 256
volatile __declspec(emem0, aligned(256), shared, import) uint32_t *fc_emu_0_payloads_ptr;

#if (FC_LB_EMU > 1)

    __asm .alloc_mem _FC_EMU_1_BUCKETS_BASE emem1+_FC_BUCKETS_ADDR global _FC_BUCKETS_ALLOC_SIZE 256 addr40
    volatile __declspec(emem1, aligned(256), shared, import) fc_cam_bucket_t *fc_emu_1_buckets_ptr;

    __asm .alloc_mem _FC_EMU_1_ENTRIES_BASE emem1+_FC_ENTRIES_ADDR global _FC_ENTRIES_ALLOC_SIZE 256
    volatile __declspec(emem1, aligned(256), shared, import) fc_entry_t *fc_emu_1_entries_ptr;

    __asm .alloc_mem _FC_EMU_1_PAYLOADS_BASE emem1+_FC_PAYLOADS_ADDR global _FC_PAYLOADS_ALLOC_SIZE 256
    volatile __declspec(emem1, aligned(256), shared, import) uint32_t *fc_emu_1_payloads_ptr;

#if (FC_LB_EMU > 2)

        __asm .alloc_mem _FC_EMU_2_BUCKETS_BASE emem2+_FC_BUCKETS_ADDR global _FC_BUCKETS_ALLOC_SIZE 256 addr40
        volatile __declspec(emem2, aligned(256), shared, import) fc_cam_bucket_t *fc_emu_2_buckets_ptr;

        __asm .alloc_mem _FC_EMU_2_ENTRIES_BASE emem2+_FC_ENTRIES_ADDR global _FC_ENTRIES_ALLOC_SIZE 256
        volatile __declspec(emem2, aligned(256), shared, import) fc_entry_t *fc_emu_2_entries_ptr;

        __asm .alloc_mem _FC_EMU_2_PAYLOADS_BASE emem2+_FC_PAYLOADS_ADDR global _FC_PAYLOADS_ALLOC_SIZE 256
        volatile __declspec(emem2, aligned(256), shared, import) uint32_t *fc_emu_2_payloads_ptr;

#endif
#endif



#ifdef FC_TIMEOUT_JOURNAL
#ifndef FC_TIMEOUT_JOURNAL_SIZE_LW
    #define FC_TIMEOUT_JOURNAL_SIZE_LW 4096
#endif

/*
 * RINGS emem_queues/
 */
__asm .declare_resource emem0_queues global 1024

#if (FC_LB_EMU > 1)
    __asm .declare_resource emem1_queues global 1024

    #if (FC_LB_EMU > 2)
        __asm .declare_resource emem2_queues global 1024
    #endif
#endif

#if (FC_TIMEOUT_EMU == 0)
    __asm .alloc_mem fc_timeout_journal_ring_data_0 emem0 global FC_TIMEOUT_JOURNAL_SIZE_LW FC_TIMEOUT_JOURNAL_SIZE_LW
    __asm .alloc_resource fc_timeout_journal_ring_0 emem0_queues global 1
    __asm .init_mu_ring fc_timeout_journal_ring_0 fc_timeout_journal_ring_data_0 0
#endif

#if (FC_TIMEOUT_EMU == 1)
    __asm .alloc_mem fc_timeout_journal_ring_data_1 emem1 global FC_TIMEOUT_JOURNAL_SIZE_LW FC_TIMEOUT_JOURNAL_SIZE_LW
    __asm .alloc_resource fc_timeout_journal_ring_1 emem1_queues global 1
    __asm .init_mu_ring fc_timeout_journal_ring_1 fc_timeout_journal_ring_data_1 0
#endif

#if (FC_TIMEOUT_EMU == 2)
    __asm .alloc_mem fc_timeout_journal_ring_data_2 emem2 global FC_TIMEOUT_JOURNAL_SIZE_LW FC_TIMEOUT_JOURNAL_SIZE_LW
    __asm .alloc_resource fc_timeout_journal_ring_2 emem2_queues global 1
    __asm .init_mu_ring fc_timeout_journal_ring_2 fc_timeout_journal_ring_data_2 0
#endif

    __gpr uint32_t journal_ring_num;

#endif


/*
 * How often to check if timeout thread is on track wrt to time?
 * Each thread should process all CAM buckets allocated to it in one second.
 * Number of blocks is calculated by dividing the total amount of memory with the number of contexts.
 * That gives the total amount to be processed per context. That amount is then divided by 256 (just a selected value).
 */
#define TUNING_NUMBER_CAM_BUCKETS_SHIFT         8
#define TUNING_NUMBER_BLOCKS                    ((_FC_BUCKETS_ALLOC_SIZE) / (TIMEOUT_CONTEXTS)) >> (TUNING_NUMBER_CAM_BUCKETS_SHIFT)



#if (SIMULATION)
    #define TUNING_TIME_PER_BLOCK           100 // small value to never wait on simulator
    #define TUNING_MAX_SLEEP_CYCLES         16
#else

/*
 * Number of cycles per seconds .. dependent on hardware ... i.e. 1200 000 000.
 * Divide this by 16 (number of cycles per timestamp unit) .. 1 200 000 000 >> 4
 * Divide this by the number of blocks that the flow cache buckets are divided in  ... (1200000000>>4) / TUNING_NUMBER_BLOCKS
 */
#define TUNING_TIME_PER_BLOCK           (((FC_CYCLES_PER_SECOND) >> 4)/(TUNING_NUMBER_BLOCKS))
#define TUNING_MAX_SLEEP_CYCLES         (1 << 19)
#endif


#if (TUNING_NUMBER_BLOCKS == 0)
    #error "TUNING_NUMBER_BLOCKS cannot be 0. Valid values are > 0"
#else
    #if (TUNING_TIME_PER_BLOCK == 0)
        #error "TUNING_TIME_PER_BLOCK must be defined. Valid values are > 0"
    #endif
#endif

volatile __lmem uint64_t tune_start_time;



/*
 * Below is padded to 4 32-bit words (16 bytes) to be able to use atomic engine commands
 */
typedef union fc_timeout_settings_t
{
    struct
    {
        uint32_t reserved       : 23;           /**< Reserved                                   */
        uint32_t disable_timeout: 1;            /**< Disable timeout                            */
        uint32_t flush_flag     : 8;            /**< Flush timeout entries, bit for each context*/

        uint32_t reserved_word2 : 32;           /**< Reserved                                   */
        uint32_t reserved_word3 : 32;           /**< Reserved                                   */
        uint32_t reserved_word4 : 32;           /**< Reserved                                   */

    };
    uint32_t value[4];                          /**< Accessor to entire structure. */
} fc_timeout_settings_t;


/*
 * Settings that can be configured for timeout loop.
 * -- disable timeout fc_timeout_settings=0x100 for each emem
 * -- flush flow cache fc_timeout_settings=0xff for each emem
 */
volatile __declspec(i28.imem, shared) fc_timeout_settings_t fc_timeout_settings;
volatile __declspec(i28.imem, shared) uint32_t fc_timeout_stats_loop_counter[8];
volatile __declspec(i28.imem, shared) uint32_t fc_timeout_stats_flows_deleted[8];        // number of flows deleted

volatile __declspec(i28.imem, shared) uint32_t fc_timeout_err_shared_lock_fail[8];         /**< Could not get shared lock          */
volatile __declspec(i28.imem, shared) uint32_t fc_timeout_err_exclusive_lock_fail[8];      /**< Could not get exclusive lock       */
volatile __declspec(i28.imem, shared) uint32_t fc_timeout_err_head_ptr_zero[8];            /**< Zero head pointers                 */
volatile __declspec(i28.imem, shared) uint32_t fc_timeout_err_next_ptr_zero[8];            /**< Next pointer zero but next bit set */
volatile __declspec(i28.imem, shared) uint32_t fc_timeout_err_free_entry_zero[8];          /**< Free entry to put back on ring zero*/
volatile __declspec(i28.imem, shared) uint32_t fc_timeout_err_ovf_hash_value_zero[8];      /**< Hash value read from entry is zero */


#ifdef TIMEOUT_TEST
/* FC_BUCKETS is calculated differently for each platform.
   We cater for SIMULATION or CDP_PLATFORM
*/
#if (SIMULATION)
    #define TEST_BUCKET_ADDRESS         (0x2008000000 + 0x2c0)
    #define TEST_BUCKET                 (0x2c0)
#else
    #define TEST_BUCKET_ADDRESS         (0x2008000000 + 0x44002c0)
    #define TEST_BUCKET                 (0x44002c0)
#endif
#endif





/*
 *
 * Implementation
 *
 */


/*
 * Divide the memory to scan into sections depending on the context.
 * The cam_base_address up to cam_end_address has to be processed.
 * This context will have to process from cam_start_address to cam_end_address.
 */
static void fc_timeout_cam_bucket_calc_addresses(
            __gpr uint32_t bucket_emu,
            __fc_addr40 fc_cam_bucket_t** cam_start_address,
            __fc_addr40 fc_cam_bucket_t** cam_end_address,
            __fc_addr40 fc_cam_bucket_t** cam_base_address)
{
    __gpr uint32_t      number_contexts = __nctx_mode();
    __gpr uint32_t active_ctx = ctx(); //(local_csr_read(local_csr_active_ctx_sts) & 0x7);

    if (number_contexts == 4)
    {
        active_ctx = active_ctx >> 1;
    }


    FC_SELECT_MU_PTR
    (
        bucket_emu,
        (__mem __addr40 uint32_t *)fc_emu_0_buckets_ptr,
#if (FC_TIMEOUT_LB_EMU > 1)
        (__mem __addr40 uint32_t *)fc_emu_1_buckets_ptr,
     #if (FC_TIMEOUT_LB_EMU > 2)
        (__mem __addr40 uint32_t *)fc_emu_2_buckets_ptr,
    #endif
#endif
     (__mem __addr40 uint32_t *)*cam_base_address
    );

    *cam_start_address = *cam_base_address + (active_ctx * ((_FC_BUCKETS_ALLOC_SIZE / TIMEOUT_CONTEXTS) / sizeof(fc_cam_bucket_t)));
    *cam_end_address = *cam_base_address + ((active_ctx + 1) * ((_FC_BUCKETS_ALLOC_SIZE / TIMEOUT_CONTEXTS) / sizeof(fc_cam_bucket_t)));
    *cam_end_address -= 1;
}



/*
 * The logic for the entry list is the same regardless of the CAM entry.
 * The head entries are not processed in this function.
 *
 * Traverse the entry list and check for timed out entries. Process
 * each entry in the list while next is not 0 (end of list).
 * If we don't have an exclusive lock, then get one.
 *
 * Return from this function should see all locks cleared.
 */
static __gpr fc_lock_t fc_timeout_process_entry_list(
    __mem __addr40 uint32_t *bucket_base_ptr,
    __mem __addr40 uint32_t *entry_base_ptr,
    __mem __addr40 uint32_t *payload_base_ptr,
    __gpr uint32_t entry_idx,
    __gpr fc_lock_t lock,
    __gpr uint8_t flush_entries_flag
    )
{
    SIGNAL                      rd_sig_1, rd_sig_2;
    __mem __addr40 uint32_t     *entry_tbl_ptr;
    __xread fc_entry_t          rd_entry;
    __gpr uint8_t               expired = 0;
    __gpr uint32_t              next_entry_idx;

    while (entry_idx != 0)
    {
        entry_tbl_ptr = (__mem __addr40 uint32_t *)FC_SET_LINKED_ENTRY_ADDRESS(entry_base_ptr, entry_idx);

        __mem_read32(&rd_entry.value[_FC_ENTRY_TIMESTAMP_wrd], entry_tbl_ptr + _FC_ENTRY_TIMESTAMP_wrd, 2 << 2, 2 << 2 , sig_done, &rd_sig_1);
        expired = 0;
        wait_for_all(&rd_sig_1);

        next_entry_idx = rd_entry.next;

        /*
        * Verify the timestamp vs timeout of the entry, and remove if we have to.
        */

        if (flush_entries_flag)
        {
            expired = 1;
        }
        else if (rd_entry.timeout != FC_NO_TIMEOUT)
        {
            __gpr uint16_t              timeout;
            timeout = FC_CONVERT_TIMEOUT(rd_entry.timeout);
            TIMEOUT_ENTRY_EXPIRED(timeout, rd_entry.timestamp, fc_get_timestamp(), expired);
        }

        if (!expired)
        {
            entry_idx = next_entry_idx;
            continue;
        }

        // We already have the shared lock, get the exclusive lock if we dont have it
        if (!lock.exclusive_lock)
        {
            /*
            * Lock the bucket entry, get shared lock first.
            * We only retry a few times, if other threads hold the lock the timestamp will get updated anyway.
            */
            lock = fc_to_lock_exclusive_with_bucket_addr(lock, FC_TIMEOUT_LOCK_RETRY, bucket_base_ptr);

            if (!lock.exclusive_lock)
            {
                /*
                * Failed to get exclusive lock. No worries. Shared lock would have been deleted
                * in trying to acquire exclusive lock.
                */
                mem_incr32((__mem __addr40 void *)&fc_timeout_err_exclusive_lock_fail[ctx()]);
                return lock;
            }
        }

        /*
        * We have the exclusive lock,
        * verify timeout and timestamp again.
        */
        mem_read32(&rd_entry.value[_FC_ENTRY_TIMESTAMP_wrd], entry_tbl_ptr + _FC_ENTRY_TIMESTAMP_wrd, 2 << 2);

        expired = 0;

        if (flush_entries_flag)
        {
            expired = 1;
        }
        else if (rd_entry.timeout != FC_NO_TIMEOUT)
        {
            __gpr uint16_t              timeout;
            timeout = FC_CONVERT_TIMEOUT(rd_entry.timeout);
            TIMEOUT_ENTRY_EXPIRED(timeout, rd_entry.timestamp, fc_get_timestamp(), expired);
        }

        if (!expired)
        {
            entry_idx = next_entry_idx;
            continue;
        }

        /*
        * Save the next entry pointer and remove the current
        */

        if (lock.cam_entry == CAM_ENTRY_FIRST)
        {
            fc_remove_flow_cam_entry_1(
                bucket_base_ptr,
                entry_base_ptr,
                payload_base_ptr,
                entry_idx,
                lock
            );
        }
        else
        {
            fc_remove_flow_cam_entry_2_3_4_ovf(
                bucket_base_ptr,
                entry_base_ptr,
                entry_idx,
                lock
            );
        }

        mem_incr32((__mem __addr40 void *)&fc_timeout_stats_flows_deleted[ctx()]);

        if (next_entry_idx == 0)
        {
            /*
            * Next is 0, nothing more to process
            */
            break;
        }

        /*
        * Set to next pointer
        */
        entry_idx = next_entry_idx;
    } // while


    /*
     * Release locks
     */
    fc_to_lock_release_with_bucket_addr(
        lock,
        bucket_base_ptr
    );

    return lock;
}


static void fc_timeout(__gpr uint32_t bucket_mu)
{

    __fc_addr40 fc_cam_bucket_t*        cam_read_address;
    __fc_addr40 fc_cam_bucket_t*        cam_start_address;
    __fc_addr40 fc_cam_bucket_t*        cam_end_address;
    __fc_addr40 fc_cam_bucket_t*        cam_base_address;
    __mem __addr40 uint32_t             *entry_base_ptr,
                                        *payload_base_ptr;
    {
        /*
        * Setup base pointers for bucket, entry and payload.
        */
        __lmem uint32_t  entry_mu,
                        payload_mu;

        FC_SET_MU_ADDRESS_PTR(
            FC_TIMEOUT_LB_EMU,
            bucket_mu,
            entry_mu,
            (__mem __addr40 uint32_t *)fc_emu_0_entries_ptr,
#if (FC_TIMEOUT_LB_EMU > 1)
            (__mem __addr40 uint32_t *)fc_emu_1_entries_ptr,
    #if (FC_TIMEOUT_LB_EMU > 2)
            (__mem __addr40 uint32_t *)fc_emu_2_entries_ptr,
    #endif
#endif
            entry_base_ptr
            );

        /*
        * Setup payload mu and base pointer
        */
        FC_SET_MU_ADDRESS_PTR(
            FC_TIMEOUT_LB_EMU,
            entry_mu,
            payload_mu,
            (__mem __addr40 uint32_t *)fc_emu_0_payloads_ptr,
#if (FC_TIMEOUT_LB_EMU > 1)
            (__mem __addr40 uint32_t *)fc_emu_1_payloads_ptr,
    #if (FC_TIMEOUT_LB_EMU > 2)
            (__mem __addr40 uint32_t *)fc_emu_2_payloads_ptr,
    #endif
#endif
            payload_base_ptr
            );
    }


    /*
    * Calculate the CAM bucket addresses this thread should scan.
    */
    fc_timeout_cam_bucket_calc_addresses(bucket_mu, &cam_start_address, &cam_end_address, &cam_base_address);

    cam_read_address = cam_start_address;


    while (1)
    {


        /*
         *
         * Check entries and remove those that have timed out
         *
         */
        {
            __gpr int32_t           i;
            __gpr uint32_t          entry_idx = 0;
            __gpr uint32_t          check_head_flag;    // do we have to check the head (has it expired)?
            __gpr uint32_t          check_next_flag;    // do we have to check if entries in the list has expired (are there any entries in the list)?
            __gpr uint32_t          shared_lock_flag;   // do we have a shared lock set? (exlucsive lock is in lock)
            __gpr fc_lock_t         lock;

            __gpr uint32_t          shared_lock_fail = 0;
            __gpr uint32_t          exclusive_lock_fail = 0;
            __gpr uint32_t          head_ptr_zero = 0;
            __gpr uint32_t          next_ptr_zero = 0;


            tune_start_time = timeout_get_64bit_timestamp();

            /*
            * Keep on processing forever
            */
            while (1)
            {
                __gpr uint32_t                  flush_entries_flag;

                {
                    /*
                    * If timeout is disabled, don't process any entries.
                    * Check if we have to flush entries during next cycle through flow cache
                    */

                    __gpr uint32_t                      timeout_disable_flag = 0;
                    __xread fc_timeout_settings_t       read_settings;
                    SIGNAL                              sig_1;

                    mem_read_atomic(&read_settings.value[0], (__mem __addr40 void *)&fc_timeout_settings.value[0], 1 << 2);

                    timeout_disable_flag = read_settings.disable_timeout;

                    if (timeout_disable_flag)
                    {
                        continue;
                    }

                    flush_entries_flag = (read_settings.flush_flag & (1 << ctx()));
                }


                lock.value = (uint32_t)((uint64_t)cam_read_address - (uint64_t)cam_base_address);

#ifdef TIMEOUT_TEST
                if (ctx() == 0x00)
                {
                    /*
                    * Test with automated tests and hard coded fc_timeout_cam_bucket_calc_addresses
                    */
                    cam_read_address = (__fc_addr40 fc_cam_bucket_t*)((uint64_t)TEST_BUCKET_ADDRESS);
                    lock.value =  TEST_BUCKET;
                }
#endif


                /*
                 * Set bucket MU in the lock handle
                 */
                lock.bucket_mu = bucket_mu;

                /*
                * For each entry in the CAM bucket
                */
#ifdef FC_ENABLE_OVERFLOW
                for (i = CAM_ENTRY_OVERFLOW; i >= CAM_ENTRY_FIRST; i--)
#else
                for (i = CAM_ENTRY_FOURTH; i >= CAM_ENTRY_FIRST; i--)
#endif
                {
                    check_head_flag = 0;
                    check_next_flag = 0;
                    shared_lock_flag = 0;
                    lock.exclusive_lock = 0;


                    /*
                    * 1:1 entry
                    */
                    if (i == CAM_ENTRY_FIRST)
                    {
                        /*
                        * Read CAM bucket and get current timestamp.
                        * Read six 32-bit words to access CAM entry 1 hash, timeout and timestamp
                        */
                        {
                            __xread fc_cam_bucket_t     fc_cam_bucket;
                            SIGNAL                      sig_1;

                            mem_read_atomic(&fc_cam_bucket.value[0], (__mem __addr40 uint32_t *)(cam_read_address), 6 << 2);

                            if (fc_cam_bucket.cam_entry1.hash == 0)
                            {
                                /*
                                * First CAM entry hash empty, carry on with for-loop
                                */
                                continue;
                            }

                            /*
                            * CAM entry 1 hash not empty,
                            * check the timestamp and timeout
                            */
                            if (flush_entries_flag)
                            {
                                check_head_flag = 1;
                            }
                            else if (fc_cam_bucket.timeout1 != FC_NO_TIMEOUT)
                            {
                                __gpr uint16_t      timeout;
                                __gpr int32_t       current_time = fc_get_timestamp();

                                /*
                                * If timeout is set to a value, calculate if expired (check_head_flag) or not
                                */
                                timeout = FC_CONVERT_TIMEOUT(fc_cam_bucket.timeout1);
                                TIMEOUT_ENTRY_EXPIRED(timeout, fc_cam_bucket.timestamp1, current_time, check_head_flag);
                            }

                            /*
                            * Read next bit from stats
                            */
                            __mem_read_atomic(&fc_cam_bucket.value[8], ((__mem __addr40 uint32_t *)(cam_read_address)) + 8, 1 << 2, 1 << 2, sig_done, &sig_1);

                            lock.cam_entry = CAM_ENTRY_FIRST;  // setup lock while waiting for read

                            wait_for_all(&sig_1);

                            check_next_flag = fc_cam_bucket.stats[CAM_ENTRY_FIRST].next;

                            if (check_head_flag || check_next_flag)
                            {
                                /*
                                 * if we have to process this CAM entry, either head or list, set the shared lock.
                                 */
                                if (!fc_to_lock_shared_with_bucket_addr(lock, FC_TIMEOUT_LOCK_RETRY, (__mem __addr40 uint32_t *)cam_base_address))
                                {
                                    /*
                                     * We could not get shared lock, give up
                                     */
                                    shared_lock_fail++;
                                    break;  // out of for-loop
                                }
                                shared_lock_flag = 1;
                            }
                            else
                            {
                                continue;  // to top of for-loop (next CAM entry)
                            }
                        }

                        /*
                        * If hash 0 then there will be no entries in the list.
                        * If hash is non-zero and we do delete head, then we want to check the "new" head
                        * again to make sure that one has not timed out.
                        * If hash is non-zero and not to be deleted, then check rest of the entries in the list.
                        */
                        while (check_head_flag)
                        {
                            /*
                            * Delete head (1:1)
                            */
#ifdef FC_TIMEOUT_ASSERT
                            if (!shared_lock_flag)
                            {
#ifdef FC_TIMEOUT_PARANOIA_CHECK
                                {
                                    /*
                                    * Shared lock should be set
                                    */
                                    fc_timeout_journal_t error_data;
                                    __gpr uint32_t          hi_addr;

                                    FC_SET_HI_ADDRESS(journal_ring_num >> 10);
                                    error_data.value = 0x00;
                                    error_data.error_code = JOURNAL_CHECK_HEAD_SHARED_FLAG;
                                    error_data.param_1 = shared_lock_flag;
                                    error_data.param_2 = lock.exclusive_lock;
                                    error_data.param_3 = i;

                                    mem_ring_journal_fast((journal_ring_num & 0x3ff), hi_addr, error_data.value);
                                }
#endif
                                check_next_flag = 0;
                                break;  // break out of while (check_head_flag)
                            }
#endif
                            if (!lock.exclusive_lock)
                            {
                                /*
                                * Lock the bucket entry, get shared lock first.
                                * We only retry a few times, if other threads hold the lock the timestamp will get updated anyway.
                                */
                                lock = fc_to_lock_exclusive_with_bucket_addr(lock, FC_TIMEOUT_LOCK_RETRY, (__mem __addr40 uint32_t *)cam_base_address);

                                if (!lock.exclusive_lock)
                                {
                                    /*
                                    * Failed to get exclusive lock. No worries. Shared lock would have been deleted
                                    * in trying to acquire exclusive lock.
                                    */

                                    shared_lock_flag = 0;
                                    check_next_flag = 0;
                                    exclusive_lock_fail++;

                                    break;  // break out of while (check_head_flag)

                                }
                            }

                            {
                                /*
                                * We have acquired the exclusive lock,
                                * verify timeout and timestamp again.
                                * Reread CAM bucket and read timestamp.
                                */
                                __xread fc_cam_bucket_t     fc_cam_bucket;
                                SIGNAL                      sig_1;

                                mem_read_atomic(&fc_cam_bucket.value[0], (__mem __addr40 uint32_t *)cam_read_address, 6 << 2);

                                check_head_flag = 0;        // clear flags while wait
                                check_next_flag = 0;


                                if (fc_cam_bucket.cam_entry1.hash == 0)
                                {
                                    break;  // break out of while (check_head_flag)
                                }

                                /*
                                    * Read next bit from stats
                                    */
                                __mem_read_atomic(&fc_cam_bucket.value[8], ((__mem __addr40 uint32_t *)cam_read_address) + 8, 1 << 2, 1 << 2, sig_done, &sig_1);
                                wait_for_all(&sig_1);

                                check_next_flag = fc_cam_bucket.stats[CAM_ENTRY_FIRST].next;

                                if (flush_entries_flag)
                                {
                                    check_head_flag = 1;
                                }
                                else if (fc_cam_bucket.timeout1 != FC_NO_TIMEOUT)
                                {
                                    __gpr uint16_t      timeout;
                                    __gpr int32_t       current_time = fc_get_timestamp();

                                    timeout = FC_CONVERT_TIMEOUT(fc_cam_bucket.timeout1);
                                    TIMEOUT_ENTRY_EXPIRED(timeout, fc_cam_bucket.timestamp1, current_time, check_head_flag);
                                }

                                if (check_head_flag)
                                {
                                    /*
                                        * Head has expired, remove head
                                        */
                                    __gpr FC_REMOVE_RESULT_CODE    remove_result;

                                    /*
                                    * Remove the head entry, the second entry is copied to the head
                                    * entry.
                                    * Locks are NOT released !!!! EXCEPT when hash was cleared
                                    */
                                    remove_result = fc_remove_flow_cam_entry_1(
                                        (__mem __addr40 uint32_t *)cam_base_address,
                                        entry_base_ptr,
                                        payload_base_ptr,
                                        0,
                                        lock
                                    );

                                    mem_incr32((__mem __addr40 void *)&fc_timeout_stats_flows_deleted[ctx()]);

                                    /*
                                    * If entry has been removed, then we have to check the next head entry
                                    * if there is one.
                                    * Keep check_head_flag set.
                                    * Otherwise move onto rest of list (if there is one).
                                    */

                                    if (remove_result == FC_REMOVE_EXACT_MATCH_LOCK_CLEARED)
                                    {
                                        /*
                                            * Entry removed and lock+hash has been cleared
                                            */
                                        lock.exclusive_lock = 0;
                                        shared_lock_flag = 0;
                                        check_head_flag = 0;
                                        check_next_flag = 0;
                                    }
                                    else if (remove_result != FC_REMOVE_EXACT_MATCH)
                                    {
                                        /*
                                        * Error code
                                        * We still have lock
                                        */
                                        check_head_flag = 0;
                                    }
                                }
                            }
                        } // while check_head_flag

                        if (check_next_flag)
                        {
                            /*
                            * Find the next pointer of head (1:1)
                            * Assumption is that check_next_flag is set.
                            * Get shared lock before we traverse the list.
                            * Should we reread the CAM bucket to check Next?
                            */

#ifdef FC_TIMEOUT_ASSERT
                            if (!shared_lock_flag)
                            {
#ifdef FC_TIMEOUT_JOURNAL
                                /*
                                * Shared lock should be set !!!
                                */
                                fc_timeout_journal_t error_data;
                                __gpr uint32_t          hi_addr;
                                FC_SET_HI_ADDRESS(journal_ring_num >> 10);

                                error_data.value = 0x00;
                                error_data.error_code = JOURNAL_CHECK_NEXT_SHARED_FLAG;
                                error_data.param_1 = shared_lock_flag;
                                error_data.param_2 = lock.exclusive_lock;
                                error_data.param_3 = i;

                                mem_ring_journal_fast((journal_ring_num & 0x3ff), hi_addr, error_data.value);
#endif
                                break; // out of for-loop
                            }
#endif

                            if (!lock.exclusive_lock)
                            {
                                /*
                                * If we don't have an exclusive lock (probably because head entry did not time out.
                                * We have to re-read  again as hash and next might have changed since
                                * we read it last before we had a shared lock.
                                * If we have an exclusive lock, then we have already reread it when checking head, this part can be skipped.
                                */
                                __xread fc_cam_bucket_t     fc_cam_bucket;
                                SIGNAL                      rd_sig_1, rd_sig_2;

                                __mem_read_atomic(&fc_cam_bucket.value[0], (__mem __addr40 uint32_t *)cam_read_address, 1 << 2, 1 << 2, sig_done, &rd_sig_1);
                                __mem_read_atomic(&fc_cam_bucket.value[8], ((__mem __addr40 uint32_t *)cam_read_address) + 8, 1 << 2, 1 << 2, sig_done, &rd_sig_2);

                                entry_idx = 0;

                                wait_for_all(&rd_sig_1, &rd_sig_2);

                                if (fc_cam_bucket.cam_entry1.hash == 0)
                                {
                                    /*
                                    * Hash is empty, don't bother to check list as it should be empty too.
                                    * Release shared lock
                                    */

                                    fc_to_lock_release_with_bucket_addr(
                                        lock,
                                        (__mem __addr40 uint32_t *)cam_base_address
                                    );

                                    shared_lock_flag = 0;
                                    lock.exclusive_lock = 0;
                                    continue;       // to top of for-loop
                                }

                                check_next_flag = fc_cam_bucket.stats[CAM_ENTRY_FIRST].next;
                            }
                        }

                        if (check_next_flag)
                        {
                            {
                                /*
                                * Assumption is that we have a shared lock and even if the CAM hash value changed between read and
                                * acquiring shared lock (i.e. hash has been zeroed), we will pick up this change when reading entry.
                                * entry_ptr->next will be 0
                                */
                                __mem __addr40 fc_entry_t           *entry_ptr;

                                entry_ptr = (__mem __addr40 fc_entry_t *)FC_SET_FIRST_ENTRY_ADDRESS(entry_base_ptr, (lock.bucket_address << 6));
                                entry_idx = entry_ptr->next;
                            }

                            if (entry_idx == 0)
                            {
                                next_ptr_zero++;
#ifdef FC_TIMEOUT_JOURNAL
                                {
                                    /*
                                    * Entry_idx cannot be zero if check_next_flag is set!!
                                    * It means the next bit in the bucket (stats) and the next pointer
                                    * is out of sync.
                                    */
                                    fc_timeout_journal_t error_data;
                                    __gpr uint32_t          hi_addr;
                                    FC_SET_HI_ADDRESS(journal_ring_num >> 10);

                                    error_data.value = 0x00;
                                    error_data.error_code = JOURNAL_CHECK_NEXT_ENTRY_IDX_ZERO;
                                    error_data.param_1 = 1;
                                    error_data.param_2 = check_next_flag;
                                    error_data.param_3 = i;

                                    mem_ring_journal_fast((journal_ring_num & 0x3ff), hi_addr, error_data.value);
                                }
#endif
                            }

                            /*
                            * Process next entry in list
                            * We already hold shared lock.
                            * fc_timeout_process_entry_list() will get and hold exclusive lock if we dont
                            * already hold one, and will release all locks when done.
                            */
                            if (entry_idx != 0)
                            {
                                lock = fc_timeout_process_entry_list(
                                    (__mem __addr40 uint32_t *)cam_base_address,
                                    entry_base_ptr,
                                    payload_base_ptr,
                                    entry_idx,
                                    lock,
                                    flush_entries_flag
                                    );

                                check_next_flag = 0;
                                shared_lock_flag = 0;
                            }
                        } // if check_next_flag


                        /*
                        * Release either shared and exclusive lock or just exclusive lock
                        */
                        if (shared_lock_flag)
                        {
                            fc_to_lock_release_with_bucket_addr(
                                lock,
                                (__mem __addr40 uint32_t *)cam_base_address
                            );
                            shared_lock_flag = 0;
                            lock.exclusive_lock = 0;
                        }
                    } // CAM_ENTRY_FIRST
                    else
                    {
                        /*
                        * Read CAM bucket and get current timestamp.
                        * Read first eight 32-bit words to access hash and timestamp and timeouts
                        */
                        {
                            __xread fc_cam_bucket_t             fc_cam_bucket;
                            SIGNAL                              sig_1;

                            mem_read_atomic(&fc_cam_bucket.value[0], (__mem __addr40 uint32_t *)FC_SET_ADDRESS_TO_LOW_LOCALITY(cam_read_address), 8 << 2);
#ifdef FC_ENABLE_OVERFLOW
                            if (i == CAM_ENTRY_OVERFLOW)
                            {
                                /*
                                * Need to read head for overflow in the last word. Also
                                * read the stats for next bits. Therefore only read four 32-bit words.
                                */
                                __mem_read_atomic(
                                    &fc_cam_bucket.value[12],
                                    ((__mem __addr40 uint32_t *)FC_SET_ADDRESS_TO_LOW_LOCALITY(cam_read_address)) + 12,
                                    4 << 2,
                                    4 << 2,
                                    sig_done,
                                    &sig_1);
                                lock.cam_entry = CAM_ENTRY_OVERFLOW;      // setup lock while waiting for read
                                wait_for_all(&sig_1);

                                if (fc_cam_bucket.head_overflow == 0)
                                {
                                    /*
                                    * If CAM overflow head is clear, jump to the next non-empty hash in the list.
                                    * By doing this we are avoiding atomic reads of zero hash values.
                                    */
                                    if (fc_cam_bucket.cam_entry4.hash == 0)
                                    {
                                        i = CAM_ENTRY_FOURTH;           // continue to CAM entry 3

                                        if (fc_cam_bucket.cam_entry3.hash == 0)
                                        {
                                            i = CAM_ENTRY_THIRD;        // continue to CAM entry 2

                                            if (fc_cam_bucket.cam_entry2.hash == 0)
                                            {
                                                i = CAM_ENTRY_SECOND;   // continue to CAM entry 1

                                                if (fc_cam_bucket.cam_entry1.hash == 0)
                                                {
                                                    break; // break out of for-loop
                                                }
                                            }
                                        }
                                    }

                                    continue; // to top of for-loop
                                }

                                if (flush_entries_flag)
                                {
                                    check_head_flag = 1;
                                }
                                else if (fc_cam_bucket.timeout_overflow != FC_NO_TIMEOUT)
                                {
                                    __gpr uint16_t      timeout;
                                    __gpr int32_t       current_time = fc_get_timestamp();

                                    timeout = FC_CONVERT_TIMEOUT(fc_cam_bucket.timeout_overflow);
                                    TIMEOUT_ENTRY_EXPIRED(timeout, fc_cam_bucket.timestamp_overflow, current_time, check_head_flag);
                                }
                            }
                            else
#endif
                            {

                                if (i == CAM_ENTRY_FOURTH)
                                {
                                    /*
                                    * If CAM entry 4 hash is zero, jump to the next non-empty hash in the list.
                                    * By doing this we are avoiding atomic reads of zero hash values.
                                    */
                                    if (fc_cam_bucket.cam_entry4.hash == 0)
                                    {
                                        if (fc_cam_bucket.cam_entry3.hash == 0)
                                        {
                                            i = CAM_ENTRY_THIRD;        // continue to CAM entry 2

                                            if (fc_cam_bucket.cam_entry2.hash == 0)
                                            {
                                                i = CAM_ENTRY_SECOND;   // continue to CAM entry 1

                                                if (fc_cam_bucket.cam_entry1.hash == 0)
                                                {
                                                    break; // break out of for-loop
                                                }
                                            }
                                        }
                                        continue; // to top of for-loop
                                    }

                                    if (flush_entries_flag)
                                    {
                                        check_head_flag = 1;
                                    }
                                    else if (fc_cam_bucket.timeout4 != FC_NO_TIMEOUT)
                                    {
                                        __gpr uint16_t      timeout;
                                        __gpr int32_t       current_time = fc_get_timestamp();

                                        timeout = FC_CONVERT_TIMEOUT(fc_cam_bucket.timeout4);
                                        TIMEOUT_ENTRY_EXPIRED(timeout, fc_cam_bucket.timestamp4, current_time, check_head_flag);
                                    }
                                }
                                else if (i == CAM_ENTRY_THIRD)
                                {
                                    if (fc_cam_bucket.cam_entry3.hash == 0)
                                    {
                                        /*
                                        * If all hash values are zero
                                        */
                                        if ((fc_cam_bucket.cam_entry1.hash |fc_cam_bucket.cam_entry2.hash) == 0)
                                        {
                                            /*
                                            * All other CAM entries hash empty, continue to next CAM bucket.
                                            */
                                            i = CAM_ENTRY_FIRST - 1;
                                            break; // break out of for-loop
                                        }

                                        continue;   // to top of for-loop (next CAM entry)
                                    }

                                    if (flush_entries_flag)
                                    {
                                        check_head_flag = 1;
                                    }
                                    else  if (fc_cam_bucket.timeout3 != FC_NO_TIMEOUT)
                                    {
                                        __gpr uint16_t      timeout;
                                        __gpr int32_t       current_time = fc_get_timestamp();

                                        timeout = FC_CONVERT_TIMEOUT(fc_cam_bucket.timeout3);
                                        TIMEOUT_ENTRY_EXPIRED(timeout, fc_cam_bucket.timestamp3, current_time, check_head_flag);
                                    }
                                }
                                else if (i == CAM_ENTRY_SECOND)
                                {
                                    if (fc_cam_bucket.cam_entry2.hash == 0)
                                    {
                                        /*
                                        * If all hash values are zero
                                        */
                                        if (fc_cam_bucket.cam_entry1.hash == 0)
                                        {
                                            /*
                                            * All other CAM entries hash empty, continue to next CAM bucket.
                                            */
                                            break;  // break out of for-loop
                                        }

                                        /*
                                        * Second CAM entry hash empty, carry on with for-loop
                                        */
                                        continue;       // to top of for-loop (next CAM entry)
                                    }

                                    if (flush_entries_flag)
                                    {
                                        check_head_flag = 1;
                                    }
                                    else if (fc_cam_bucket.timeout2 != FC_NO_TIMEOUT)
                                    {
                                        __gpr uint16_t      timeout;
                                        __gpr int32_t       current_time = fc_get_timestamp();

                                        timeout = FC_CONVERT_TIMEOUT(fc_cam_bucket.timeout2);
                                        TIMEOUT_ENTRY_EXPIRED(timeout, fc_cam_bucket.timestamp2, current_time, check_head_flag);
                                    }
                                }
                            }

                            /*
                            * Is Next bit set in stats indicating a linked list?
                            */
                            if (i != CAM_ENTRY_OVERFLOW)
                            {
                                /*
                                * We already did a second read for overflow, for other CAM entries
                                * we need to read stats only.
                                */
                                __mem_read_atomic(&fc_cam_bucket.value[8], ((__mem __addr40 uint32_t *)(cam_read_address)) + 8, 4 << 2, 4 << 2, sig_done, &sig_1);

                                lock.cam_entry = i;      // setup lock while waiting for read

                                wait_for_all(&sig_1);
                            }

                            check_next_flag = fc_cam_bucket.stats[i].next;

                            if (check_head_flag || check_next_flag)
                            {
                                /*
                                * if we have to process this CAM entry, either head or list, set the shared lock.
                                */
                                if (!fc_to_lock_shared_with_bucket_addr(lock, FC_TIMEOUT_LOCK_RETRY, (__mem __addr40 uint32_t *)cam_base_address))
                                {
                                    /*
                                    * We could not get shared lock, give up
                                    */
                                    shared_lock_fail++;
                                    break;  // break out of for-loop
                                }
                                shared_lock_flag = 1;
                            }
                            else
                            {
                                continue;  // to top of for-loop (next CAM entry)
                            }
                        }

                        /*
                        * If hash 0 then there will be no entries in the list.
                        * If hash is non-zero and we do delete head, then we want to check the "new" head
                        * again to make sure that one has not timed out.
                        * If hash is non-zero and not to be deleted, then check rest of list.
                        */
                        while (check_head_flag)
                        {
                            /*
                            * Delete head of CAM entry2/3/4/overflow
                            */

#ifdef FC_TIMEOUT_ASSERT
                            if (!shared_lock_flag)
                            {
#ifdef FC_TIMEOUT_JOURNAL
                                /*
                                * Shared lock should be set !!!
                                */
                                fc_timeout_journal_t error_data;
                                __gpr uint32_t          hi_addr;
                                FC_SET_HI_ADDRESS(journal_ring_num >> 10);

                                error_data.value = 0x00;
                                error_data.error_code = JOURNAL_CHECK_HEAD_SHARED_FLAG;
                                error_data.param_1 = shared_lock_flag;
                                error_data.param_2 = lock.exclusive_lock;
                                error_data.param_3 = i;

                                mem_ring_journal_fast((journal_ring_num & 0x3ff), hi_addr, error_data.value);
#endif
                                check_next_flag = 0;
                                break;  // break out of while (check_head_flag)
                            }
#endif
                            if (!lock.exclusive_lock)
                            {
                                /*
                                * Lock the bucket entry, get shared lock first.
                                * We only retry a few times, if other threads hold the lock the timestamp will get updated anyway.
                                */
                                lock = fc_to_lock_exclusive_with_bucket_addr(lock, FC_TIMEOUT_LOCK_RETRY, (__mem __addr40 uint32_t *)cam_base_address);

                                if (!lock.exclusive_lock)
                                {
                                    /*
                                    * Failed to get exclusive lock. No worries. Shared lock would have been deleted
                                    * in trying to acquire exclusive lock.
                                    */

                                    shared_lock_flag = 0;
                                    check_next_flag = 0;
                                    exclusive_lock_fail++;
                                    break;  // break out of while (check_head_flag)

                                }
                            }


                            /*
                            * We have acquired the exclusive lock,
                            * verify timeout and timestamp again.
                            * Reread CAM bucket and read timestamp.
                            */
                            {
                                __xread fc_cam_bucket_t     fc_cam_bucket;
                                SIGNAL                      sig_1, sig_2;

                                __mem_read_atomic(&fc_cam_bucket.value[0], (__mem __addr40 uint32_t *)cam_read_address, 8 << 2, 8 << 2, sig_done, &sig_1);
                                __mem_read_atomic(&fc_cam_bucket.value[8], ((__mem __addr40 uint32_t *)cam_read_address) + 8, 8 << 2, 8 << 2, sig_done, &sig_2);

                                check_head_flag = 0;        // clear flags while wait
                                check_next_flag = 0;

                                wait_for_all(&sig_1, &sig_2);

                                if     ((i == CAM_ENTRY_SECOND && fc_cam_bucket.cam_entry2.hash == 0)
                                    || (i == CAM_ENTRY_THIRD && fc_cam_bucket.cam_entry3.hash == 0)
                                    || (i == CAM_ENTRY_FOURTH && fc_cam_bucket.cam_entry4.hash == 0)
                                    || (i == CAM_ENTRY_OVERFLOW && fc_cam_bucket.head_overflow == 0))
                                {
                                    break;  // break out of while (check_head_flag)
                                }

                                check_next_flag = fc_cam_bucket.stats[i].next;


                                /*
                                    * Get head pointers
                                    */
                                FC_EXTRACT_HEAD_PTR_FROM_CAM_BUCKET_XFERS(fc_cam_bucket, i, entry_idx);

#ifdef FC_TIMEOUT_ASSERT
                                if (entry_idx == 0)
                                {
#ifdef FC_TIMEOUT_JOURNAL
                                    /*
                                    * Entry_idx (set to the CAM entry head) cannot be zero !!!
                                    */
                                    fc_timeout_journal_t error_data;
                                    __gpr uint32_t          hi_addr;
                                    FC_SET_HI_ADDRESS(journal_ring_num >> 10);

                                    error_data.value = 0x00;
                                    error_data.error_code = JOURNAL_CHECK_HEAD_ENTRY_IDX_ZERO;
                                    error_data.param_1 = entry_idx;
                                    error_data.param_2 = 0;
                                    error_data.param_3 = i;

                                    mem_ring_journal_fast((journal_ring_num & 0x3ff), hi_addr, error_data.value);
#endif
                                    head_ptr_zero++;
                                    break; // break out of while (check_head_flag), locks have to be cleared
                                }
#endif

                                if (flush_entries_flag)
                                {
                                    check_head_flag = 1;
                                }
                                else if (i == CAM_ENTRY_SECOND && fc_cam_bucket.timeout2 != FC_NO_TIMEOUT)
                                {
                                    __gpr uint16_t      timeout;
                                    __gpr int32_t       current_time = fc_get_timestamp();

                                    timeout = FC_CONVERT_TIMEOUT(fc_cam_bucket.timeout2);
                                    TIMEOUT_ENTRY_EXPIRED(timeout, fc_cam_bucket.timestamp2, current_time, check_head_flag);
                                }
                                else if (i == CAM_ENTRY_THIRD && fc_cam_bucket.timeout3 != FC_NO_TIMEOUT)
                                {
                                    __gpr uint16_t      timeout;
                                    __gpr int32_t       current_time = fc_get_timestamp();

                                    timeout = FC_CONVERT_TIMEOUT(fc_cam_bucket.timeout3);
                                    TIMEOUT_ENTRY_EXPIRED(timeout, fc_cam_bucket.timestamp3, current_time, check_head_flag);
                                }
                                else if (i == CAM_ENTRY_FOURTH && fc_cam_bucket.timeout4 != FC_NO_TIMEOUT)
                                {
                                    __gpr uint16_t      timeout;
                                    __gpr int32_t       current_time = fc_get_timestamp();

                                    timeout = FC_CONVERT_TIMEOUT(fc_cam_bucket.timeout4);
                                    TIMEOUT_ENTRY_EXPIRED(timeout, fc_cam_bucket.timestamp4, current_time, check_head_flag);
                                }
                                else if (i == CAM_ENTRY_OVERFLOW && fc_cam_bucket.timeout_overflow != FC_NO_TIMEOUT)
                                {
                                    __gpr uint16_t      timeout;
                                    __gpr int32_t       current_time = fc_get_timestamp();

                                    timeout = FC_CONVERT_TIMEOUT(fc_cam_bucket.timeout_overflow);
                                    TIMEOUT_ENTRY_EXPIRED(timeout, fc_cam_bucket.timestamp_overflow, current_time, check_head_flag);
                                }

                                if (check_head_flag)
                                {
                                    __gpr FC_REMOVE_RESULT_CODE    remove_result;

                                    /*
                                    * Remove the head entry
                                    * Locks are NOT released !!!! EXCEPT when hash was cleared
                                    */
                                    remove_result = fc_remove_flow_cam_entry_2_3_4_ovf_head(
                                        (__mem __addr40 uint32_t *)cam_base_address,
                                        entry_base_ptr,
                                        lock,
                                        entry_idx
                                    );

                                    mem_incr32((__mem __addr40 void *)&fc_timeout_stats_flows_deleted[ctx()]);


                                    /*
                                    * If entry has been removed, then we have to check the next head entry
                                    * if there is one.
                                    * Keep check_head_flag set.
                                    * Otherwise move onto rest of list (if there is one).
                                    */
                                    if (remove_result == FC_REMOVE_EXACT_MATCH)
                                    {
                                        /*
                                        * Entry removed, lock intact.
                                        * Continue to top of while (check_head_flag)-loop
                                        */
                                        continue;
                                    }
                                    else if (remove_result == FC_REMOVE_EXACT_MATCH_LOCK_CLEARED)
                                    {
                                        /*
                                        * Entry removed and lock+hash has been cleared
                                        */
                                        lock.exclusive_lock = 0;
                                        shared_lock_flag = 0;
                                        check_head_flag = 0;
                                        check_next_flag = 0;
                                    }
                                    else
                                    {
                                        /*
                                        * Error code
                                        * We still have lock
                                        */
                                        check_head_flag = 0;
                                    }
                                }
                            }
                        } // while check_head_flag



                        if (check_next_flag)
                        {
                            /*
                            * Find the next pointer of head2
                            * Assumption is that check_next_flag is set.
                            * Check shared lock before we traverse the list.
                            */

#ifdef FC_TIMEOUT_ASSERT
                            if (!shared_lock_flag)
                            {
#ifdef FC_TIMEOUT_JOURNAL
                                /*
                                * Shared lock should be set !!!
                                */
                                fc_timeout_journal_t error_data;
                                __gpr uint32_t          hi_addr;
                                FC_SET_HI_ADDRESS(journal_ring_num >> 10);

                                error_data.value = 0x00;
                                error_data.error_code = JOURNAL_CHECK_NEXT_SHARED_FLAG;
                                error_data.param_1 = shared_lock_flag;
                                error_data.param_2 = lock.exclusive_lock;
                                error_data.param_3 = i;

                                mem_ring_journal_fast((journal_ring_num & 0x3ff), hi_addr, error_data.value);
#endif
                                continue; // continue to top of for-loop ... next CAM entry
                            }
#endif
                            if (!lock.exclusive_lock)
                            {
                                /*
                                * If we don't have an exclusive lock (probably because head entry did not time out.
                                * We have to re-read  again as hash2 and head2 might have changed since
                                * we read it last before we had a shared lock.
                                * If we have an exclusive lock, then we have already reread it when checking head, then this part is skipped.
                                */
                                __xread fc_cam_bucket_t     fc_cam_bucket;
                                SIGNAL                      rd_sig_1, rd_sig_2;

                                __mem_read_atomic(&fc_cam_bucket.value[0], (__mem __addr40 uint32_t *)cam_read_address, 8 << 2, 8 << 2, sig_done, &rd_sig_1);
                                __mem_read_atomic(&fc_cam_bucket.value[8], ((__mem __addr40 uint32_t *)cam_read_address) + 8, 8 << 2, 8 << 2, sig_done, &rd_sig_2);

                                entry_idx = 0;

                                wait_for_all(&rd_sig_1, &rd_sig_2);

                                if    ((i == CAM_ENTRY_SECOND && fc_cam_bucket.cam_entry2.hash == 0)
                                    || (i == CAM_ENTRY_THIRD && fc_cam_bucket.cam_entry3.hash == 0)
                                    || (i == CAM_ENTRY_FOURTH && fc_cam_bucket.cam_entry4.hash == 0)
                                    || (i == CAM_ENTRY_OVERFLOW && fc_cam_bucket.head_overflow == 0))
                                {
                                    /*
                                    * Hash is empty, don't bother to check list as it should be empty too.
                                    * Release shared lock
                                    */

                                    fc_to_lock_release_with_bucket_addr(
                                        lock,
                                        (__mem __addr40 uint32_t *)cam_base_address
                                    );

                                    shared_lock_flag = 0;
                                    lock.exclusive_lock = 0;
                                    continue;       // to top of for-loop
                                }


                                check_next_flag = fc_cam_bucket.stats[i].next;

                                /*
                                    * Get head pointers
                                    */
                                FC_EXTRACT_HEAD_PTR_FROM_CAM_BUCKET_XFERS(fc_cam_bucket, i, entry_idx);

                            } // (!lock.exclusive_lock)


                            if (check_next_flag)
                            {
                                /*
                                    * Assumption is that entry_idx is set to head2/head3/head4/overflow head.
                                    * Find the following entry in list to process.
                                    * Next bit (check_next_flag) should not be set while the next of head is zero.
                                    */
                                __mem __addr40 fc_entry_t           *entry_ptr;

                                entry_ptr = (__mem __addr40 fc_entry_t *)FC_SET_LINKED_ENTRY_ADDRESS(entry_base_ptr, entry_idx);
                                entry_idx = entry_ptr->next;


                                if (entry_idx == 0)
                                {
                                    next_ptr_zero++;
#ifdef FC_TIMEOUT_JOURNAL
                                    {
                                        /*
                                        * Entry_idx cannot be zero if check_next_flag is set!!
                                        * It means the next bit in the bucket (stats) and the next pointer
                                        * is out of sync.
                                        */
                                        fc_timeout_journal_t error_data;
                                        __gpr uint32_t          hi_addr;
                                        FC_SET_HI_ADDRESS(journal_ring_num >> 10);

                                        error_data.value = 0x00;
                                        error_data.error_code = JOURNAL_CHECK_NEXT_ENTRY_IDX_ZERO;
                                        error_data.param_1 = 3;
                                        error_data.param_2 = check_next_flag;
                                        error_data.param_3 = i;
                                            mem_ring_journal_fast((journal_ring_num & 0x3ff), hi_addr, error_data.value);

                                    }
#endif
                                }

                                /*
                                * Process next entry in list (after head)
                                * fc_timeout_process_entry_list() will get and hold exclusive lock if we dont
                                * already hold one, and will release all locks when done.
                                */
                                if (entry_idx)
                                {
                                    lock = fc_timeout_process_entry_list(
                                        (__mem __addr40 uint32_t *)cam_base_address,
                                        entry_base_ptr,
                                        payload_base_ptr,
                                        entry_idx,
                                        lock,
                                        flush_entries_flag
                                        );

                                    check_next_flag = 0;
                                    shared_lock_flag = 0;
                                }
                            }
                        } // if check_next_flag


                        if (shared_lock_flag)
                        {
                            /*
                            * Release either shared and exclusive lock or just exclusive lock
                            */
                            fc_to_lock_release_with_bucket_addr(
                                lock,
                                (__mem __addr40 uint32_t *)cam_base_address
                            );
                            shared_lock_flag = 0;
                            lock.exclusive_lock = 0;
                        }
                    }
                } // for

#ifdef TIMEOUT_TEST
                if (ctx() == 0x00)
                {
                    /*
                    * Signal test thread that we have completed processing timeout entries
                    * Use signal 14 and signal to ME 0.
                    */
                    SIGNAL signal;


                    __assign_relative_register((void*)&signal, 14);

                    /* signal remote */
                    __signal_me(32, 0+4, 0, __signal_number(&signal));
                    return;
                }

#endif
                cam_read_address++;

                {
                    /*
                    * TUNING
                    * CAM bucket memory divided into blocks, after each block check the timestamp, if we are over the time limit
                    * just carry on; if we are under the time limit, wait.
                    */
                    __fc_addr40 fc_cam_bucket_t*        cam_end_ptr = cam_end_address;
                    __gpr uint32_t                      cam_block_address = (uint32_t)cam_read_address - (uint32_t)cam_start_address;

                    cam_end_ptr++;

                    if (((cam_block_address >> TUNING_NUMBER_CAM_BUCKETS_SHIFT) << TUNING_NUMBER_CAM_BUCKETS_SHIFT) == cam_block_address)
                    {
                        __gpr uint64_t          current_end_time = timeout_get_64bit_timestamp();
                        __gpr uint64_t          expected_end_time = tune_start_time + (cam_block_address >> TUNING_NUMBER_CAM_BUCKETS_SHIFT) * TUNING_TIME_PER_BLOCK;
                        __gpr uint32_t          time_to_wait = 0;


                        if (expected_end_time < tune_start_time)
                        {
                            /*
                             * expected end time has wrapped
                             */
                            if (current_end_time > tune_start_time)
                            {
                                /*
                                 * end time has not wrapped yet
                                 */
                                time_to_wait = 0xffffffffffffffff - current_end_time + expected_end_time;
                            }
                        }
                        if (current_end_time < expected_end_time)
                        {
                            time_to_wait = expected_end_time - current_end_time;
                        }


                        if (time_to_wait)
                        {
                            /*
                            * difference between start and end time is less than expected;
                            * have to wait here for TUNING_TIME_PER_BLOCK minus the difference between start and end time
                            */
                            __gpr int32_t           i = time_to_wait;
                            __gpr uint32_t          sleep_cycles = TUNING_MAX_SLEEP_CYCLES;

                            while (i > 0)
                            {
                                if (i < (TUNING_MAX_SLEEP_CYCLES >> 4))
                                {
                                    sleep_cycles = i << 4;
                                }

                                sleep(sleep_cycles);
                                i -= (sleep_cycles >> 4);
                            }
                        }
                    }
                } // end TUNING


                if ((uint32_t)cam_read_address > (uint32_t)cam_end_address)
                {
                    /*
                    * If cam_read_address passed cam_end_address, start at beginning.
                    */
                    cam_read_address = (__fc_addr40 fc_cam_bucket_t*)(uint64_t)cam_start_address;

                    /*
                    * Start measuring next cycle
                    */
                    tune_start_time = timeout_get_64bit_timestamp();

                    /*
                    * If we had to flush flow cache, reset flags as we have finished doing that.
                    */
                    if (flush_entries_flag)
                    {
                        /*
                        * clear flag for this context, we have flushed what we could
                        * We can use atomic engine commands as fc_timeout_settings_t is 16 bytes in length
                        */
                        __xread uint32_t         rd_flush_flag;

                        mem_bitclr_imm(flush_entries_flag, (__mem __addr40 void *)&fc_timeout_settings.value[0]);

                        flush_entries_flag = 0;
                    }

                    /*
                     * Set error fc_error_counters and statistics
                     */
                    {
                        __gpr uint32_t active_context = ctx();

                        mem_add32_imm(exclusive_lock_fail, (__mem __addr40 void *)&fc_timeout_err_exclusive_lock_fail[active_context]);
                        mem_add32_imm(shared_lock_fail, (__mem __addr40 void *)&fc_timeout_err_shared_lock_fail[active_context]);
                        mem_add32_imm(next_ptr_zero, (__mem __addr40 void *)&fc_timeout_err_next_ptr_zero[active_context]);
                        mem_add32_imm(head_ptr_zero, (__mem __addr40 void *)&fc_timeout_err_head_ptr_zero[active_context]);
                        mem_incr32((__mem __addr40 void *)&fc_timeout_stats_loop_counter[active_context]);

                        shared_lock_fail = 0;
                        exclusive_lock_fail = 0;
                        head_ptr_zero = 0;
                        next_ptr_zero = 0;
                    }

                } // if
            } // while timeout entries
        }
    } // while forever and ever

} // fc_timeout

/**
 *
 */
void main (void)
{
    __lmem uint32_t          active_ctx = ctx();

#ifdef TIMEOUT_TEST

    /*
     * only one context
     */
    if (active_ctx != 0)
    {
        return;
    }

#endif

    try_ctassert(sizeof(fc_cam_bucket_t) == _FC_BUCKET_SIZE);
    try_ctassert(sizeof(fc_entry_t) == _FC_ENTRY_SIZE);

    fc_emu_0_buckets_ptr = (__declspec(emem0, shared, import, addr40) fc_cam_bucket_t *)__link_sym("_FC_EMU_0_BUCKETS_BASE");
    fc_emu_0_entries_ptr = (__declspec(emem0, shared, import, addr40) fc_entry_t *)__link_sym("_FC_EMU_0_ENTRIES_BASE");
    fc_emu_0_payloads_ptr = (__declspec(emem0, shared, import, addr40) unsigned int *)__link_sym("_FC_EMU_0_PAYLOADS_BASE");

    #if (FC_TIMEOUT_LB_EMU > 1)
        fc_emu_1_buckets_ptr = (__declspec(emem1, shared, import, addr40) fc_cam_bucket_t *)__link_sym("_FC_EMU_1_BUCKETS_BASE");
        fc_emu_1_entries_ptr = (__declspec(emem1, shared, import, addr40) fc_entry_t *)__link_sym("_FC_EMU_1_ENTRIES_BASE");
        fc_emu_1_payloads_ptr = (__declspec(emem1, shared, import, addr40) unsigned int *)__link_sym("_FC_EMU_1_PAYLOADS_BASE");

        #if (FC_TIMEOUT_LB_EMU > 2)
            fc_emu_2_buckets_ptr = (__declspec(emem2, shared, import, addr40) fc_cam_bucket_t *)__link_sym("_FC_EMU_2_BUCKETS_BASE");
            fc_emu_2_entries_ptr = (__declspec(emem2, shared, import, addr40) fc_entry_t *)__link_sym("_FC_EMU_2_ENTRIES_BASE");
            fc_emu_2_payloads_ptr = (__declspec(emem2, shared, import, addr40) unsigned int *)__link_sym("_FC_EMU_2_PAYLOADS_BASE");
        #endif
    #endif

#ifdef FC_TIMEOUT_JOURNAL
#if (FC_TIMEOUT_EMU == 0)
    journal_ring_num = __link_sym("fc_timeout_journal_ring_0");
#endif
#if (FC_TIMEOUT_EMU == 1)
    journal_ring_num = __link_sym("fc_timeout_journal_ring_1");
    journal_ring_num |= (1 << 10);      // change ring num to indicate emem1
#endif
#if (FC_TIMEOUT_EMU == 2)
    journal_ring_num = __link_sym("fc_timeout_journal_ring_2");
    journal_ring_num |= (2 << 10);      // change ring num to indicate emem2
#endif
#endif


#ifndef TIMEOUT_TEST
    /*
    * All contexts to wait for system initialization to complete.
    */
    system_init_wait_done();

#else
    /*
    * Wait for signal to start processing flow cache entries
    * Use signal ME_INIT_SIGNAL.
    */
    if (active_ctx == 0)
    {
        SIGNAL sig;

        __assign_relative_register((void*)&sig, ME_INIT_SIGNAL);
        wait_for_all(&sig);
    }
#endif

    fc_timeout(FC_TIMEOUT_EMU);
}

#endif
