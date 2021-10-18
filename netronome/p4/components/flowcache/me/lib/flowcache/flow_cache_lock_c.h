/*
 * Copyright (C) 2014-2016,  Netronome Systems, Inc.  All rights reserved.
 * *
 * @file        flow_cache_lock_c.h
 * @brief       Prototypes for flow_cache_lock.c
 */


#ifndef _FLOW_CACHE_LOCK_C_H_
#define _FLOW_CACHE_LOCK_C_H_

#include <nfp.h>
#include <stdint.h>
#include <stdlib.h>

#include "flow_cache_lock_common_c.h"


/** fc_lock_release
 *  Release the lock on the CAM entry, xlock and shared read lock is cleared, if xlock is set.
 *  Otherwise only shared lock is cleared.
 *  The bucket mu and actual CAM address (offset) is stored in the lock.
 *
 * @param in_lock               The lock to release of type fc_lock_t
 * @param bucket_base_ptr0      First bucket base pointer
 * @param bucket_base_ptr1      Second bucket base pointer
 * @param bucket_base_ptr2      Third bucket base pointer
 */
void fc_lock_release(
    __gpr fc_lock_t in_lock,
    __mem __addr40 uint32_t *bucket_base_ptr0
#if (FC_LB_EMU > 1)
    ,__mem __addr40 uint32_t *bucket_base_ptr1
#if (FC_LB_EMU > 2)
    ,__mem __addr40 uint32_t *bucket_base_ptr2
#endif
#endif
    );


/** fc_lock_release_with_bucket_addr
 *  Release the lock on the CAM entry, xlock and shared read lock is cleared, if xlock is set.
 *  Otherwise only shared lock is cleared.
 *  Bucket base address should be provided. The actual CAM address (offset) is stored in the lock.
 *
 * @param in_lock               The lock to release of type fc_lock_t
 * @param bucket_addr_ptr       Bucket address base pointer.
 */
void fc_lock_release_with_bucket_addr(
    __gpr fc_lock_t in_lock,
    __mem __addr40 uint32_t *bucket_addr_ptr
);



/** fc_lock_exclusive
 *  Get exclusive lock on CAM entry, user should already have acquired a shared lock.
 *  The bucket mu and actual CAM address (offset) is stored in the lock.
 *  If we could not get an exclusive lock, the shared lock will also be released.
 *
 * @param in_lock               The lock of type fc_lock_t
 * @param bucket_base_ptr0      First bucket base pointer
 * @param bucket_base_ptr1      Second bucket base pointer
 * @param bucket_base_ptr2      Third bucket base pointer
 * @return                      Updated lock if xlock could be set.
 */
__gpr fc_lock_t fc_lock_exclusive(
    __gpr fc_lock_t in_lock,
    __mem __addr40 uint32_t *bucket_base_ptr0
#if (FC_LB_EMU > 1)
    ,__mem __addr40 uint32_t *bucket_base_ptr1
#if (FC_LB_EMU > 2)
    ,__mem __addr40 uint32_t *bucket_base_ptr2
#endif
#endif
);



/** fc_lock_exclusive_with_bucket_addr
 *  Get exclusive lock on CAM entry, user should already have acquired a shared lock. Bucket base address
 *  should be provided. The actual CAM address (offset) is stored in the lock.
 *  If we could not get an exclusive lock, the shared lock will also be released.
 *
 * @param in_lock               The lock of type fc_lock_t
 * @param bucket_addr_ptr       Bucket address base pointer.
 * @return                      Updated lock if xlock could be set.
 */
__gpr fc_lock_t fc_lock_exclusive_with_bucket_addr(
    __gpr fc_lock_t in_lock,
    __mem __addr40 uint32_t *bucket_addr_ptr
);


#ifdef FC_ENABLE_OVERFLOW
/** fc_lock_shared
 *  Get shared lock on CAM entry. If we have the exclusive lock, return with success.
 *  If another thread is holding an exclusive lock on CAM entry, keep on retrying up to retry count.
 *  The bucket mu and actual CAM address (offset) is stored in the lock.
 *
 * @param in_lock               The lock of type fc_lock_t
 * @param bucket_base_ptr0      First bucket base pointer
 * @param bucket_base_ptr1      Second bucket base pointer
 * @param bucket_base_ptr2      Third bucket base pointer
 * @return                      1 if shared lock or 0.
 */
__gpr int fc_lock_shared(
    __gpr fc_lock_t in_lock,
    __mem __addr40 uint32_t *bucket_base_ptr0
#if (FC_LB_EMU > 1)
    ,__mem __addr40 uint32_t *bucket_base_ptr1
#if (FC_LB_EMU > 2)
    ,__mem __addr40 uint32_t *bucket_base_ptr2
#endif
#endif
);

/** fc_lock_shared_with_bucket_addr
 *  Get shared lock on CAM entry. If already an exclusive lock on CAM entry, keep on retrying up to retry count.
 *  Bucket base address should be provided. The actual CAM address (offset) is stored in the lock.
 *
 * @param in_lock               The lock of type fc_lock_t
 * @param bucket_addr_ptr       Bucket address base pointer.
 * @return                      1 if shared lock or 0.
 */
__gpr int fc_lock_shared_with_bucket_addr(
    __gpr fc_lock_t in_lock,
    __mem __addr40 uint32_t *bucket_addr_ptr
);
#endif
#endif
