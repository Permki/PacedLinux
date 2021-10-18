/*
 * Copyright (C) 2014-2016,  Netronome Systems, Inc.  All rights reserved.
 *
 * @file        flow_cache_lock_common_c.h
 * @brief       Common macros used by flow cache and flow cache timeout locking.
 */

#ifndef _FLOW_CACHE_LOCK_COMMON_C_H_
#define _FLOW_CACHE_LOCK_COMMON_C_H_

#include <nfp.h>
#include <stdint.h>
#include <stdlib.h>


/**
 * Helper macro to extract the address of the CAM ENTRY from the lock.
 * The index in the CAM bucket (0,1,2,3) indicates which entry.
 * Return type should be uint32_t.
 */
#ifndef FC_EXTRACT_CAM_ENTRY_ADDRESS_FROM_LOCK
    #define FC_EXTRACT_CAM_ENTRY_ADDRESS_FROM_LOCK(lock)  (lock & ~0x23)
#endif

/**
 * Helper macro to extract the address of the CAM BUCKET from the lock.
 * Return type should be fc_cam_bucket_t.
 */
#ifndef FC_EXTRACT_CAM_BUCKET_ADDRESS_FROM_LOCK
    #define FC_EXTRACT_CAM_BUCKET_ADDRESS_FROM_LOCK(lock)  (lock & ~0x3f)
#endif


/*
 * CAM lock bits and settings. Read lock = shared lock.  Exclusive lock = write lock.
 * Exclusive lock also includes shared lock.
 */
#ifndef FC_CAM_BUCKET_XLOCK
    #define FC_CAM_BUCKET_XLOCK                        (0x80 << 24)
#endif

#ifndef FC_CAM_BUCKET_SHARED_LOCK
    #define FC_CAM_BUCKET_SHARED_LOCK                  (0x01 << 24)
#endif

#ifndef FC_CAM_BUCKET_SHARED_LOCK_MASK
    #define FC_CAM_BUCKET_SHARED_LOCK_MASK             (0x7F << 24)
#endif


/*
 * Return true if an exclusive lock is on the CAM entry (bit 31 is set)
 */
#ifndef FC_CAM_BUCKET_IS_EXCLUSIVE_LOCK
    #define FC_CAM_BUCKET_IS_EXCLUSIVE_LOCK(value)     ((value & FC_CAM_BUCKET_XLOCK) == FC_CAM_BUCKET_XLOCK)
#endif


/*
 * Return true if CAM lookup result shows exclusive lock on CAM entry (bit 7 is set)
 * Use match_high_byte of CAM lookup result.
 */
#ifndef FC_CAM_BUCKET_IS_EXCLUSIVE_LOCK_IN_LOOKUP
    #define FC_CAM_BUCKET_IS_EXCLUSIVE_LOCK_IN_LOOKUP(value)     ((value & 0x80) == 0x80)
#endif


/*
 * Return true if any shared locks on CAM entry
 */
#ifndef FC_CAM_BUCKET_IS_ANY_SHARED_LOCK
    #define FC_CAM_BUCKET_IS_ANY_SHARED_LOCK(value)    ((value & FC_CAM_BUCKET_SHARED_LOCK_MASK) > 0)
#endif


/*
 * Verify there is only one shared lock on CAM entry
 */
#ifndef FC_CAM_BUCKET_IS_ONE_SHARED_LOCK
    #define FC_CAM_BUCKET_IS_ONE_SHARED_LOCK(value)    ((value & FC_CAM_BUCKET_SHARED_LOCK_MASK) == FC_CAM_BUCKET_SHARED_LOCK)
#endif

#endif
