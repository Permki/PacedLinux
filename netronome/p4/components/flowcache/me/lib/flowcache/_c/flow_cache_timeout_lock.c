/*
 * Copyright (C) 2014-2017,  Netronome Systems, Inc.  All rights reserved.
 *
 * @file        flow_cache_timeout_lock.c
 * @brief       Locking functionality for timeout, excluding cached locks.
 *              Cached locks not needed for timeout thread.
 */

#ifndef _FLOW_CACHE_TIMEOUT_LOCK_C_
#define _FLOW_CACHE_TIMEOUT_LOCK_C_

#include <assert.h>
#include <nfp.h>
#include <stdint.h>
#include <types.h>
#include <nfp/mem_atomic.h>
#include <nfp/mem_ring.h>
#include <nfp/me.h>

#include "flow_cache_global_c.h"
#include "flow_cache_timeout_lock_c.h"
#include "flow_cache_timestamp_c.h"
#include "jenkins_hash_c.h"


#ifdef FC_TIMEOUT_JOURNAL
    #include "flow_cache_journal_c.h"

    extern __gpr uint32_t journal_ring_num;
#endif




void fc_to_lock_release(
    __gpr fc_lock_t in_lock,
    __mem __addr40 uint32_t *bucket_base_ptr0
#if (FC_TIMEOUT_LB_EMU > 1)
    ,__mem __addr40 uint32_t *bucket_base_ptr1
#if (FC_TIMEOUT_LB_EMU > 2)
    ,__mem __addr40 uint32_t *bucket_base_ptr2
#endif
#endif
)
{
    __gpr uint32_t                      bucket_mu = in_lock.bucket_mu;
    __gpr uint32_t                      cam_entry_addr = FC_EXTRACT_CAM_ENTRY_ADDRESS_FROM_LOCK(in_lock.value);
    __mem __addr40 uint32_t             *bucket_addr_ptr;
    __xwrite uint32_t                   lock_xfer;
    SIGNAL                              sig;

    FC_SELECT_MU_PTR(
        bucket_mu,
        bucket_base_ptr0,
#if (FC_TIMEOUT_LB_EMU > 1)
        bucket_base_ptr1,
    #if (FC_TIMEOUT_LB_EMU > 2)
        bucket_base_ptr2,
    #endif
#endif
        bucket_addr_ptr
    );

    fc_to_lock_release_with_bucket_addr(
        in_lock,
        bucket_addr_ptr
    );

   return;
}


void fc_to_lock_release_with_bucket_addr(
    __gpr fc_lock_t in_lock,
    __mem __addr40 uint32_t *bucket_addr_ptr
)
{
    __gpr uint32_t                      cam_entry_addr = FC_EXTRACT_CAM_ENTRY_ADDRESS_FROM_LOCK(in_lock.value);
#ifndef FC_TIMEOUT_JOURNAL
    __xwrite uint32_t                   lock_xfer;
#else
    __xrw uint32_t                      lock_xfer;
#endif
    SIGNAL                              sig;


    if (in_lock.exclusive_lock)
    {
        /*
        * If exclusive lock bit is set in the lock, then release the exclusive and read (shared) lock
        */
        lock_xfer = FC_CAM_BUCKET_XLOCK | FC_CAM_BUCKET_SHARED_LOCK;

#ifndef FC_TIMEOUT_JOURNAL
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


            if (!(lock_xfer & (FC_CAM_BUCKET_XLOCK | FC_CAM_BUCKET_SHARED_LOCK)))
            {
                /*
                * This is very bad, we have released an exclusive lock and shared lock but there was not one set !!!!
                */
                fc_timeout_journal_t error_data;
                __gpr uint32_t          hi_addr;
                FC_SET_HI_ADDRESS(journal_ring_num >> 10);

                error_data.value = 0x00;
                error_data.error_code = JOURNAL_XLOCK_RELEASE;
                error_data.param_1 = lock_xfer;
                error_data.param_2 = in_lock.exclusive_lock;
                error_data.param_3 = in_lock.cam_entry;

                mem_ring_journal_fast((journal_ring_num & 0x3ff), hi_addr, error_data.value);
            }
        }
#endif
    }
    else
    {
        /*
         * Only shared lock set, release shared lock.
         * There still could be a exclusive and shared lock held by another thread.
         */
        lock_xfer = FC_CAM_BUCKET_SHARED_LOCK;

#ifndef FC_TIMEOUT_JOURNAL
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

            if ((lock_xfer & (FC_CAM_BUCKET_SHARED_LOCK_MASK)) == 0)
            {
                /*
                * This is very bad, we MUST have a shared lock but it is not set !!!!
                */
                fc_timeout_journal_t error_data;
                __gpr uint32_t          hi_addr;
                FC_SET_HI_ADDRESS(journal_ring_num >> 10);

                error_data.value = 0x00;
                error_data.error_code = JOURNAL_SHARED_LOCK_RELEASE;
                error_data.param_1 = lock_xfer;
                error_data.param_2 = in_lock.exclusive_lock;
                error_data.param_3 = in_lock.cam_entry;

                mem_ring_journal_fast((journal_ring_num & 0x3ff), hi_addr, error_data.value);
            }
        }
#endif
    }


   return;
}

__gpr fc_lock_t fc_to_lock_exclusive(
    __gpr fc_lock_t in_lock,
    __gpr uint32_t retry_count,
    __mem __addr40 uint32_t *bucket_base_ptr0
#if (FC_TIMEOUT_LB_EMU > 1)
    ,__mem __addr40 uint32_t *bucket_base_ptr1
#if (FC_TIMEOUT_LB_EMU > 2)
    ,__mem __addr40 uint32_t *bucket_base_ptr2
#endif
#endif
)
{
    __mem __addr40 uint32_t             *bucket_addr_ptr;

    FC_SELECT_MU_PTR(
        in_lock.bucket_mu,
        bucket_base_ptr0,
#if (FC_TIMEOUT_LB_EMU > 1)
        bucket_base_ptr1,
    #if (FC_TIMEOUT_LB_EMU > 2)
        bucket_base_ptr2,
    #endif
#endif
        bucket_addr_ptr);

    return fc_to_lock_exclusive_with_bucket_addr(in_lock, retry_count, bucket_addr_ptr);
}



__gpr fc_lock_t fc_to_lock_exclusive_with_bucket_addr(
    __gpr fc_lock_t in_lock,
    __gpr uint32_t retry_count,
    __mem __addr40 uint32_t *bucket_addr_ptr
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
        __gpr uint32_t                      cam_entry_addr = FC_EXTRACT_CAM_ENTRY_ADDRESS_FROM_LOCK(in_lock.value);
        __gpr uint32_t                      i = 0;
        __xrw uint32_t                          lock_xfer;
        __xread uint32_t                    lock_xfer_read;
        __gpr uint32_t                      continue_flag = 1;
        SIGNAL_PAIR                         sig_pair;
        SIGNAL                              sig;


        lock_xfer = FC_CAM_BUCKET_XLOCK;

        /*
         * We already have a shared lock when trying to get exclusive lock.
         * If we fail to get an exclusive lock, we release the shared lock and back off.
         * Try again to get shared lock, try again to get exclusive lock  ...
         * When retry count has been reached ... failure ... shared lock would have been released.
         */
        while (continue_flag)
        {

            /*
            * No one holds the exclusive lock, try to get exclusive lock
            */
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


            if (!FC_CAM_BUCKET_IS_EXCLUSIVE_LOCK(lock_xfer))
            {
                i = 0;

                if (FC_CAM_BUCKET_IS_ONE_SHARED_LOCK(lock_xfer))
                {
                    /*
                    * We have the exclusive and one and only shared lock, SUCCESS
                    */
                    in_lock.exclusive_lock = 1;
                    return in_lock;
                }

                do
                {
                /*
                    * We have the exclusive and shared lock, SUCCESS
                    * Now we wait for other read (shared) locks to be released, only one (mine) should be left.
                    */

                    sleep(FC_TIMEOUT_LOCK_BACKOFF);
                    i++;

                    __mem_read_atomic(
                        &lock_xfer_read,
                        (__declspec(mem, addr40, aligned(32)) uint32_t *)((uint64_t)bucket_addr_ptr + cam_entry_addr),
                        1 << 2,
                        1 << 2,
                        ctx_swap,
                        &sig
                    );
                }
                while (!FC_CAM_BUCKET_IS_ONE_SHARED_LOCK(lock_xfer_read)
                    && ((retry_count == FC_LOCK_RETRY_FOREVER) || (i < retry_count)));

                if (FC_CAM_BUCKET_IS_ONE_SHARED_LOCK(lock_xfer_read))
                {
                    /*
                    * We have the exclusive and one and only shared lock, SUCCESS
                    */
                    in_lock.exclusive_lock = 1;
                    return in_lock;
                }

                /*
                * Could not get other threads to release shared locks.
                * Release my exclusive and shared lock.
                */
                lock_xfer = FC_CAM_BUCKET_XLOCK | FC_CAM_BUCKET_SHARED_LOCK;
                __mem_sub32(
                    (__xwrite void *)&lock_xfer,
                    (__mem __addr40 uint32_t *)((uint64_t)bucket_addr_ptr + cam_entry_addr),
                    1 << 2,
                    1 << 2,
                    ctx_swap,
                    &sig
                );
                return in_lock;
            }
            else
            {
                /*
                * Another thread holds the exclusive lock, release my shared lock.
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

            /*
             * Back off
             */
            sleep(FC_TIMEOUT_LOCK_BACKOFF);
            i++;
            continue_flag = 0;

            if ((retry_count == FC_LOCK_RETRY_FOREVER) || (i < retry_count))
            {
                continue_flag = 1;

                /*
                 * Get shared lock again, if we have to go through loop again.
                 */
                if (!fc_to_lock_shared_with_bucket_addr(in_lock, retry_count, bucket_addr_ptr))
                {
                    /*
                    * Could not get shared lock
                    */
                    continue_flag = 0;
                }
            }
        } //while
    }

   return in_lock;
}

__gpr int fc_to_lock_shared(
    __gpr fc_lock_t in_lock,
    __gpr uint32_t retry_count,
    __mem __addr40 uint32_t *bucket_base_ptr0
#if (FC_TIMEOUT_LB_EMU > 1)
    ,__mem __addr40 uint32_t *bucket_base_ptr1
#if (FC_TIMEOUT_LB_EMU > 2)
    ,__mem __addr40 uint32_t *bucket_base_ptr2
#endif
#endif
)
{
    __mem __addr40 uint32_t             *bucket_addr_ptr;

    FC_SELECT_MU_PTR(
        in_lock.bucket_mu,
        bucket_base_ptr0,
#if (FC_TIMEOUT_LB_EMU > 1)
        bucket_base_ptr1,
    #if (FC_TIMEOUT_LB_EMU > 2)
        bucket_base_ptr2,
    #endif
#endif
        bucket_addr_ptr);

    return fc_to_lock_shared_with_bucket_addr(in_lock, retry_count, bucket_addr_ptr);
}


__gpr int fc_to_lock_shared_with_bucket_addr(
    __gpr fc_lock_t in_lock,
    __gpr uint32_t retry_count,
    __mem __addr40 uint32_t *bucket_addr_ptr
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
        __gpr uint32_t                      cam_entry_addr = FC_EXTRACT_CAM_ENTRY_ADDRESS_FROM_LOCK(in_lock.value);
        __gpr uint32_t                      i = 0;
        __gpr int                           shared_lock = 0;
        __xrw uint32_t                      lock_xfer  = FC_CAM_BUCKET_SHARED_LOCK;
        SIGNAL                              sig;


        /*
        * Keep on trying to get shared lock, until retry count has been reached ... failure
        */
        while ((retry_count == FC_LOCK_RETRY_FOREVER) || (i < retry_count))
        {
            /*
             * Get shared lock even if someone else holds the exclusive lock.
             */
            mem_test_add(
                &lock_xfer,
                (__mem __addr40 uint32_t *)((uint64_t)bucket_addr_ptr + cam_entry_addr),
                1 << 2
            );


            if (!FC_CAM_BUCKET_IS_EXCLUSIVE_LOCK(lock_xfer))
            {
                /*
                * Return with our shared lock, success
                */
                shared_lock = 1;
                return 1;
            }

            /*
             * Someone holds the exclusive lock, clear our shared lock.
             */
            __mem_sub32(
                (__xwrite void *)&lock_xfer,
                (__mem __addr40 uint32_t *)((uint64_t)bucket_addr_ptr + cam_entry_addr),
                1 << 2,
                1 << 2,
                ctx_swap,
                &sig
            );

            /*
             * Back off
             */
            sleep(FC_TIMEOUT_LOCK_BACKOFF);

            i++;
        } // while
    }
   return 0;
}


#endif
