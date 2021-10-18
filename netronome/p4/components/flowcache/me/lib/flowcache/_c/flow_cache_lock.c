/*
 * Copyright (C) 2014-2017,  Netronome Systems, Inc.  All rights reserved.
 *
 * @file        flow_cache_lock.c
 * @brief       Locking functionality for CAM entries, including cached locks.
 */

#ifndef _FLOW_CACHE_LOCK_C_
#define _FLOW_CACHE_LOCK_C_

#include <assert.h>
#include <nfp.h>
#include <stdint.h>
#include <types.h>
#include <nfp/mem_atomic.h>
#include <nfp/mem_ring.h>
#include <nfp/me.h>
#include <cam_c.h>

#include "flow_cache_global_c.h"
#include "flow_cache_lock_c.h"
#include "flow_cache_timestamp_c.h"
#include "jenkins_hash_c.h"

#ifdef FC_JOURNAL
    #include "flow_cache_journal_c.h"

    extern __gpr uint32_t journal_ring_num;
#endif


/*
 * Lock counters
 */
volatile __declspec(i28.imem, shared, export) uint32_t fc_stats_lock_release;
volatile __declspec(i28.imem, shared, export) uint32_t fc_stats_lock_error = 0;
volatile __declspec(i28.imem, shared, export) uint32_t fc_stats_wait_busy_bit = 0;
volatile __declspec(i28.imem, shared, export) uint32_t fc_stats_wait_shared_locks = 0;
volatile __declspec(i28.imem, shared, export) uint32_t fc_stats_wait_cache_locks = 0;



__lmem __shared fc_cached_lock_entry_t fc_lock_cache_base[16];
__lmem __shared uint32_t fc_cached_lock_free_slot_list;


void fc_lock_release(
    __gpr fc_lock_t             in_lock,
    __mem __addr40 uint32_t     *bucket_base_ptr0
#if (FC_LB_EMU > 1)
    ,__mem __addr40 uint32_t    *bucket_base_ptr1
#if (FC_LB_EMU > 2)
    ,__mem __addr40 uint32_t    *bucket_base_ptr2
#endif
#endif
)
{
    __gpr uint32_t                      bucket_mu = in_lock.bucket_mu;
    __mem __addr40 uint32_t             *bucket_addr_ptr;
    __xwrite uint32_t                   lock_xfer;
    SIGNAL                              sig;

    FC_SELECT_MU_PTR(
        bucket_mu,
        bucket_base_ptr0,
#if (FC_LB_EMU > 1)
        bucket_base_ptr1,
    #if (FC_LB_EMU > 2)
        bucket_base_ptr2,
    #endif
#endif
        bucket_addr_ptr
    );

    fc_lock_release_with_bucket_addr(
        in_lock,
        bucket_addr_ptr
    );

   return;
}


extern __forceinline void fc_lock_release_with_bucket_addr(
    __gpr fc_lock_t in_lock,
    __mem __addr40 uint32_t *bucket_addr_ptr
)
{
    __gpr uint32_t                      cam_entry_addr = FC_EXTRACT_CAM_ENTRY_ADDRESS_FROM_LOCK(in_lock.value);
    __gpr cam_lookup_t                  lock_slot;
    __gpr uint32_t                      lock_slot_idx;
    __gpr uint32_t                      lock_xfer_value = FC_CAM_BUCKET_XLOCK | FC_CAM_BUCKET_SHARED_LOCK;
#ifndef FC_JOURNAL
    __xwrite uint32_t                   lock_xfer;
#else
    __xrw uint32_t                      lock_xfer;
#endif
    SIGNAL                              sig;


#ifdef FC_JOURNAL
#ifdef FC_ENABLE_OVERFLOW
    if (in_lock.cam_entry > CAM_ENTRY_OVERFLOW)
#else
    if (in_lock.cam_entry > CAM_ENTRY_FOURTH)
#endif
    {
        /*
        * Error if cam entry out of bounds in lock
        */
        fc_journal_t            error_data;
        __gpr uint32_t          hi_addr;
        FC_SET_HI_ADDRESS(journal_ring_num >> 10);

        error_data.value = 0x00;
        error_data.error_code = JOURNAL_LOCK_CORRUPT;
        error_data.param_1 = lock_xfer_value;
        error_data.param_2 = in_lock.exclusive_lock;
        error_data.param_3 = in_lock.cam_entry;

        mem_ring_journal_fast((journal_ring_num & 0x3ff), hi_addr, error_data.value);
        return;
    }
#endif


    lock_slot = cam_lookup(FC_EXTRACT_CAM_BUCKET_ADDRESS_FROM_LOCK(in_lock.value) | (1 << 2) | in_lock.bucket_mu);


    if (!lock_slot.hit)
    {
        /*
        * This is very bad, we have released an exclusive lock and shared lock but there was not one set !!!!
        */

#ifdef FC_JOURNAL
        {
            fc_journal_t            error_data;
            __gpr uint32_t          hi_addr;

            FC_SET_HI_ADDRESS(journal_ring_num >> 10);

            error_data.value = 0x00;
            error_data.error_code = JOURNAL_LOCK_NOT_IN_CAM;
            error_data.param_1 = in_lock.cam_entry;
            error_data.param_2 = lock_slot.hit;
            error_data.param_3 = 0x00;

            mem_ring_journal_fast((journal_ring_num & 0x3ff), hi_addr, error_data.value);
        }
#endif
        mem_incr32((__mem __addr40 void *)&fc_stats_lock_release);
        return;
    }

    lock_slot_idx = lock_slot.entry_num;

    while (fc_lock_cache_base[lock_slot_idx].busy_bit)
    {
        sleep(FC_TIMEOUT_LOCK_BACKOFF);
    }

    {
        /*
         * Decrement the usage counter.
         * If exclusive lock is set, then subtract exclusive and shared lock from global lock.
         * If it is only a shared lock, then subtract one from the cached locks.
         */

        FC_CACHED_LOCK_DECREMENT_USAGE_COUNTER(lock_slot_idx);  // fc_lock_cache_base[lock_slot_idx].usage_counter--;

        if (!in_lock.exclusive_lock)
        {
            /*
             * Subtract from cached locks
             */
            lock_xfer_value = 0;

            FC_CACHED_LOCK_DECREMENT_CAM_ENTRY_COUNTER(lock_slot_idx, in_lock.cam_entry);

            if (FC_CACHED_LOCK_GET_CAM_COUNTER(lock_slot_idx, in_lock.cam_entry) == 0)
            {
                /*
                 * If no more cached locks, subtract shared lock from global locks
                 */
                lock_xfer_value = FC_CAM_BUCKET_SHARED_LOCK;
            }
        }

        if (fc_lock_cache_base[lock_slot_idx].usage_counter == 0)
        {
            /*
             * Last user, put it back onto free fc_cached_lock_free_slot_list
             */
            cam_write(lock_slot_idx, 0, 0);
            fc_cached_lock_free_slot_list |= (1 << lock_slot_idx);
        }
    }

    if (lock_xfer_value)
    {
        /*
        * Release the exclusive and shared lock or only shared lock
        */
        lock_xfer = lock_xfer_value;

#ifndef FC_JOURNAL
        __mem_sub32(
            &lock_xfer,
            (__declspec(mem, addr40, aligned(32)) uint32_t *)((uint64_t)bucket_addr_ptr + cam_entry_addr),
            1 << 2,
            1 << 2,
            sig_done,
            &sig
        );

        wait_for_all(&sig);
#else
        {
            mem_test_sub(
                &lock_xfer,
                (__declspec(mem, addr40, aligned(32)) uint32_t *)((uint64_t)bucket_addr_ptr + cam_entry_addr),
                1 << 2
            );


            if (!(lock_xfer & (lock_xfer_value)))
            {
                /*
                * This is very bad, we have released an exclusive lock and shared lock but there was not one set !!!!
                * Dump the lock we are trying to release plus what was in the CAM entry before to journal.
                */
                fc_journal_t error_data;
                __gpr uint32_t          hi_addr;
                FC_SET_HI_ADDRESS(journal_ring_num >> 10);

                error_data.value = lock_xfer_value >> 24;
                error_data.error_code = JOURNAL_XLOCK_RELEASE;
                error_data.param_1 = in_lock.exclusive_lock;
                error_data.param_2 = in_lock.cam_entry;
                //error_data.param_3 = lock_xfer_value

                mem_ring_journal_fast((journal_ring_num & 0x3ff), hi_addr, error_data.value);

                error_data.value = lock_xfer;
                mem_ring_journal_fast((journal_ring_num & 0x3ff), hi_addr, error_data.value);

                mem_incr32((__mem __addr40 void *)&fc_stats_lock_release);
                return;
            }
        }
#endif
    }

    mem_incr32((__mem __addr40 void *)&fc_stats_lock_release);
    return;
}



__gpr fc_lock_t fc_lock_exclusive(
    __gpr fc_lock_t in_lock,
    __mem __addr40 uint32_t *bucket_base_ptr0
#if (FC_LB_EMU > 1)
    ,__mem __addr40 uint32_t *bucket_base_ptr1
#if (FC_LB_EMU > 2)
    ,__mem __addr40 uint32_t *bucket_base_ptr2
#endif
#endif
)
{
    __mem __addr40 uint32_t             *bucket_addr_ptr;

    FC_SELECT_MU_PTR(
        in_lock.bucket_mu,
        bucket_base_ptr0,
#if (FC_LB_EMU > 1)
        bucket_base_ptr1,
    #if (FC_LB_EMU > 2)
        bucket_base_ptr2,
    #endif
#endif
        bucket_addr_ptr);

    return fc_lock_exclusive_with_bucket_addr(in_lock, bucket_addr_ptr);
}



__gpr fc_lock_t fc_lock_exclusive_with_bucket_addr(
    __gpr fc_lock_t             in_lock,
    __mem __addr40 uint32_t     *bucket_addr_ptr
)
{
    /*
     * we already hold the exclusive lock, return
     */
    if (in_lock.exclusive_lock)
    {
        return in_lock;
    }

    {
        __gpr cam_lookup_t              lock_slot;
        __gpr uint32_t                  lock_slot_idx;
        __gpr uint32_t                  has_global_lock = 0;
        __gpr uint32_t                  cam_entry_addr = FC_EXTRACT_CAM_ENTRY_ADDRESS_FROM_LOCK(in_lock.value);
        __xrw uint32_t                  lock_xfer;
        SIGNAL_PAIR                     sig_pair;
        SIGNAL                          sig;


        lock_slot = cam_lookup(FC_EXTRACT_CAM_BUCKET_ADDRESS_FROM_LOCK(in_lock.value) | (1 << 2) | in_lock.bucket_mu);

        if (!lock_slot.hit)
        {
            /*
            * This is very bad, we should already have a shared lock but there was not one set in cached locks!!!!
            * TODO add some stats to catch this
            */
            mem_incr32((__mem __addr40 void *)&fc_stats_lock_error);
            return in_lock;
        }

        /*
         * We already have a shared lock when trying to get exclusive lock.
         * If we fail to get an exclusive lock, we release the shared lock and back off.
         * Try again to get shared lock, try again to get exclusive lock  ...
         * When retry count has been reached ... failure ... shared lock would have been released.
         */

		__critical_path();

        lock_slot_idx = lock_slot.entry_num;

        lock_xfer = FC_CAM_BUCKET_XLOCK;
        __mem_test_set(
            &lock_xfer,
            (__declspec(mem, addr40) uint32_t *)((uint64_t)bucket_addr_ptr + cam_entry_addr),
            1 << 2,
            1 << 2,
            sig_done,
            &sig_pair
        );
        wait_for_all(&sig_pair);


        while (fc_lock_cache_base[lock_slot_idx].busy_bit)
        {
            /*
            * While another thread is operating on local memory and global locks
            * wait for it to complete
            */
            mem_incr32((__mem __addr40 void *)&fc_stats_wait_busy_bit);
            sleep(FC_TIMEOUT_LOCK_BACKOFF);
        }

        /*
        * Check if more than one cached lock, in that case we must add to the global shared lock.
        * Subtract one from local cam entry counter.
        */
        has_global_lock = FC_CACHED_LOCK_GET_CAM_COUNTER(lock_slot_idx, in_lock.cam_entry); //((fc_lock_cache_base[lock_slot_idx].value >> lock_shift) & 0x1f);
        FC_CACHED_LOCK_DECREMENT_CAM_ENTRY_COUNTER(lock_slot_idx, in_lock.cam_entry);

        if (!FC_CAM_BUCKET_IS_EXCLUSIVE_LOCK(lock_xfer))
        {
            if (has_global_lock > 1)
            {
                /*
                * We only had a cached lock, therefore add our global shared lock.
                */
                lock_xfer = FC_CAM_BUCKET_SHARED_LOCK;
                mem_add32(
                    &lock_xfer,
                    (__mem __addr40 uint32_t *)((uint64_t)bucket_addr_ptr + cam_entry_addr),
                    1 << 2
                );
            }

            while (FC_CACHED_LOCK_GET_CAM_COUNTER(lock_slot_idx, in_lock.cam_entry) != 0)
            {
                /*
                * Wait for other cached shared locks to be released.
                */
                sleep(FC_TIMEOUT_LOCK_BACKOFF);
            }

            while (!FC_CAM_BUCKET_IS_ONE_SHARED_LOCK(lock_xfer))
            {
                /*
                * We have the exclusive and shared lock, SUCCESS
                * Now we wait for other global shared locks to be released, only one (mine) should be left.
                */
                mem_incr32((__mem __addr40 void *)&fc_stats_wait_shared_locks);

                __mem_read_atomic(
                    &lock_xfer,
                    (__declspec(mem, addr40, aligned(32)) uint32_t *)((uint64_t)bucket_addr_ptr + cam_entry_addr),
                    1 << 2,
                    1 << 2,
                    ctx_swap,
                    &sig
                );
            }

            /*
            * We have the exclusive and one and only shared lock, SUCCESS
            */
            in_lock.exclusive_lock = 1;
            return in_lock;

        }
        else
        {
            /*
             * Failure, we have to back off for another thread holding the exclusive lock.
             * Do the lookup again from scratch, as hash could have changed or entry deleted.
             */
            if (has_global_lock == 1)
            {
                /*
                * Another thread holds the exclusive lock, release my global shared lock, if I had one.
                * SDN-967
                */
                lock_xfer = FC_CAM_BUCKET_SHARED_LOCK;
                __mem_sub32(
                    (__xwrite void *)&lock_xfer,
                    (__mem __addr40 uint32_t *)((uint64_t)bucket_addr_ptr + cam_entry_addr),
                    1 << 2,
                    1 << 2,
                    ctx_swap,
                    &sig
                );
            }
            return in_lock;
        }

    }

   return in_lock;
}


#ifdef FC_ENABLE_OVERFLOW
__gpr int fc_lock_shared(
    __gpr fc_lock_t             in_lock,
    __mem __addr40 uint32_t     *bucket_base_ptr0
#if (FC_LB_EMU > 1)
    ,__mem __addr40 uint32_t    *bucket_base_ptr1
#if (FC_LB_EMU > 2)
    ,__mem __addr40 uint32_t    *bucket_base_ptr2
#endif
#endif
)
{
    __mem __addr40 uint32_t             *bucket_addr_ptr;

    FC_SELECT_MU_PTR(
        in_lock.bucket_mu,
        bucket_base_ptr0,
#if (FC_LB_EMU > 1)
        bucket_base_ptr1,
    #if (FC_LB_EMU > 2)
        bucket_base_ptr2,
    #endif
#endif
        bucket_addr_ptr);

    return fc_lock_shared_with_bucket_addr(in_lock, bucket_addr_ptr);
}


__gpr int fc_lock_shared_with_bucket_addr(
    __gpr fc_lock_t             in_lock,
    __mem __addr40 uint32_t     *bucket_addr_ptr
)
{
    /*
     * we already hold the exclusive lock, return
     */
    if (in_lock.exclusive_lock)
    {
        return 1;
    }

    {
        __gpr uint32_t                  cam_entry_addr = FC_EXTRACT_CAM_ENTRY_ADDRESS_FROM_LOCK(in_lock.value);
        __xrw uint32_t                  lock_xfer = FC_CAM_BUCKET_SHARED_LOCK;
        __gpr cam_lookup_t              lock_slot;
        __gpr uint32_t                  lock_slot_idx;

        __mem __addr40 uint32_t *cam_entry_ptr = (__mem __addr40 uint32_t *)((uint64_t)bucket_addr_ptr + cam_entry_addr);


        /*
         * Do CAM lookup for cached shared locks
         */
        lock_slot = cam_lookup(FC_EXTRACT_CAM_BUCKET_ADDRESS_FROM_LOCK(in_lock.value) | (1 << 2) | in_lock.bucket_mu);

        if (!lock_slot.hit)
        {
            /*
            * Allocate cached lock and write to it
            * Set busy bit.
            */
            lock_slot_idx = ffs(fc_cached_lock_free_slot_list);
            fc_cached_lock_free_slot_list &= ~(0x01 << lock_slot_idx);

            cam_write(lock_slot_idx, FC_EXTRACT_CAM_BUCKET_ADDRESS_FROM_LOCK(in_lock.value) | (1 << 2) | in_lock.bucket_mu, 0);
            fc_lock_cache_base[lock_slot_idx].busy_bit = 1;
            FC_CACHED_LOCK_INCREMENT_USAGE_COUNTER(lock_slot_idx);

            /*
            * Use shared code at bottom to get global lock
            */

        }
        else
        {
            /*
            * We have a CAM hit, check for shared locks and so forth
            */
            lock_slot_idx = lock_slot.entry_num;
            FC_CACHED_LOCK_INCREMENT_USAGE_COUNTER(lock_slot_idx);

            while (fc_lock_cache_base[lock_slot_idx].busy_bit)
            {
                /*
                * While another thread is operating on local memory and global locks
                * wait for it to complete
                */
                sleep(FC_TIMEOUT_LOCK_BACKOFF);
            }

            fc_lock_cache_base[lock_slot_idx].busy_bit = 1;


            if (FC_CACHED_LOCK_GET_CAM_COUNTER(lock_slot_idx, in_lock.cam_entry) == 0)
            {
                /*
                 * No cached shared lock as yet, try to get global lock - shared code at bottom
                 */
            }
            else
            {
                /*
                * Existing cached shared locks, verify global lock
                */
                mem_read_atomic(&lock_xfer, cam_entry_ptr, 1 << 2);

                if (!FC_CAM_BUCKET_IS_EXCLUSIVE_LOCK(lock_xfer))
                {
                    /*
                     * Success, no exclusive lock
                     */
                    FC_CACHED_LOCK_INCREMENT_CAM_ENTRY_COUNTER(lock_slot_idx, in_lock.cam_entry);
                    fc_lock_cache_base[lock_slot_idx].busy_bit = 0;

                    return 1;  /* success */
                }
                else
                {
                    /*
                    * There is an exclusive lock on the CAM entry, try again.
                    */
                    FC_CACHED_LOCK_DECREMENT_USAGE_COUNTER(lock_slot_idx);
                    fc_lock_cache_base[lock_slot_idx].busy_bit = 0;

                    if (fc_lock_cache_base[lock_slot_idx].usage_counter == 0)
                    {
                        /*
                        * Last user, put it back onto free fc_cached_lock_free_slot_list
                        */
                        cam_write(lock_slot_idx, 0, 0);
                        fc_cached_lock_free_slot_list |= (1 << lock_slot_idx);
                    }

                    return 0; /* failure */
                }
            }
        }

        /*
         * Cached lock slot allocated and usage count incremented.
         * Busy bit set.
         * Try to get global shared lock, but release shared lock if there is an exclusive lock
         */
        mem_test_add(
            &lock_xfer,
            cam_entry_ptr,
            1 << 2
        );

        if (!FC_CAM_BUCKET_IS_EXCLUSIVE_LOCK(lock_xfer))
                    {
            /*
            * No exclusive lock, we have the global shared lock
            */
            FC_CACHED_LOCK_INCREMENT_CAM_ENTRY_COUNTER(lock_slot_idx, in_lock.cam_entry);
            fc_lock_cache_base[lock_slot_idx].busy_bit = 0;

            return 1; /* success */
        }

        {
            /*
            * Another thread has exclusive lock,
            * Subtract our global shared lock and clear busy bit.
            */
            mem_sub32(
                (__xwrite void *)&lock_xfer,
                cam_entry_ptr,
                1 << 2
            );

            FC_CACHED_LOCK_DECREMENT_USAGE_COUNTER(lock_slot_idx);
            fc_lock_cache_base[lock_slot_idx].busy_bit = 0;

            if (fc_lock_cache_base[lock_slot_idx].usage_counter == 0)
            {
                /*
                * Last user, put it back onto free fc_cached_lock_free_slot_list
                */
                cam_write(lock_slot_idx, 0, 0);
                fc_cached_lock_free_slot_list |= (1 << lock_slot_idx);
            }

            return 0;       /* failure */
        }

    }
   return 0;
}

#endif

#endif
