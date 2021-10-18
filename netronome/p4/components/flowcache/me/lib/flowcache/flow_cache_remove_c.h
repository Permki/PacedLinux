/*
 * Copyright (C) 2014-2016,  Netronome Systems, Inc.  All rights reserved.
 *
 * @file        flow_cache_remove_c.h
 * @brief       Structures and prototypes used by flow_cache_remove.c
 */

#ifndef _FLOW_CACHE_REMOVE_C_H_
#define _FLOW_CACHE_REMOVE_C_H_

#include <nfp.h>
#include <stdint.h>
#include <stdlib.h>


typedef union fc_entry_fields_t
{
    struct
    {
        uint32_t dont_care      :7;
        uint32_t next           :1;
        uint32_t timeout        :8;
        uint32_t timestamp      :16;;
    };
    uint32_t value;
} fc_entry_fields_t;



#define FC_TIMEOUT_FREE_RING        FC_FREE_RING
#define FC_TIMEOUT_PAYLOAD_SIZE     FC_PAYLOAD_SIZE


/** fc_copy_entry
 *  Called to copy an entry from source to destination.
 *  Entry is 32 32-bit words in length. Copy in 16 32-bit word chunks.
 *
 * @param src_ptr                       Source entry pointer
 * @param dest_ptr                      Destination entry pointer
 *
 * @return                              fc_entry_fields - next/timeout and timestamp
 */
__gpr fc_entry_fields_t fc_copy_entry(
    __mem __addr40 uint32_t *src_ptr,
    __mem __addr40 uint32_t *dest_ptr
);


/** fc_copy_payload
 *  Called to copy payload from source to destination.
 *  Copy in 16 32-bit word chunks.
 *
 * @param src_ptr                       Source entry pointer
 * @param dest_ptr                      Destination entry pointer
 *
 * @return                              fc_entry_fields - next/timeout and timestamp
 */
void fc_copy_payload(
    __mem __addr40 uint32_t *src_ptr,
    __mem __addr40 uint32_t *dest_ptr
);




/** fc_remove_flow_cam_entry_1 (same as fc_remove_flow but specific to CAM entry 1 for performance purposes)
 *  Called to remove a flow of CAM entry 1. It removes the flow as set in the lock and matching payload_address.
 *  This function expects an exclusive lock to be held, as this function releases the exclusive lock after deletion.
 *  Make sure to have acquired an exclusive (plus shared) lock before calling this function.
 *
 * @param bucket_base_ptr               Bucket address base pointer
 * @param entry_base_ptr                Entry address base pointer
 * @param payload_base_ptr              Payload address base pointer
 * @param entry_idx_to_remove           Entry idx to remove
 * @param lock                          Lock of the specific cam entry to remove. There is already a shared lock on entry.
 *
 * @return                              Result indicating if entry could be removed or if there was
 *                                      an error in removing it.
 *                                      If entry found and removed: FC_REMOVE_EXACT_MATCH
 *                                      If an entry found but there was some error in removing it: FC_REMOVE_ERROR
 *                                      If an entry was not found FC_REMOVE_ENTRY_NOT_FOUND
 */
__gpr FC_REMOVE_RESULT_CODE fc_remove_flow_cam_entry_1(
    __mem __addr40 uint32_t *bucket_base_ptr,
    __mem __addr40 uint32_t *entry_base_ptr,
    __mem __addr40 uint32_t *payload_base_ptr,
    __gpr uint32_t entry_idx_to_remove,
    __gpr fc_lock_t lock
    );



/** fc_remove_flow_cam_entry_2_3_4 (same as fc_remove_flow but specific to CAM entry 2, 3 or 4 for performance purposes)
 *  Called to remove a flow of CAM entry 2, 3 or 4. It removes the flow as set in the lock and matching payload_address.
 *  This function expects an exclusive lock to be held, as this function releases the exclusive lock after deletion.
 *  Make sure to have acquired an exclusive (plus shared) lock before calling this function.
 *
 * @param bucket_base_ptr               Bucket address base pointer
 * @param entry_base_ptr                Entry address base pointer
 * @param entry_idx_to_remove           Entry index to remove
 * @param lock                          Lock of the specific cam entry to remove. There is already a shared lock on entry.
 *
 * @return                              Result indicating if entry could be removed or if there was
 *                                      an error in removing it.
 *                                      If entry found and removed: FC_REMOVE_EXACT_MATCH
 *                                      If an entry found but there was some error in removing it: FC_REMOVE_ERROR
 *                                      If an entry was not found FC_REMOVE_ENTRY_NOT_FOUND
 */
__gpr FC_REMOVE_RESULT_CODE fc_remove_flow_cam_entry_2_3_4_ovf(
    __mem __addr40 uint32_t *bucket_base_ptr,
    __mem __addr40 uint32_t *entry_base_ptr,
    __gpr uint32_t entry_idx_to_remove,
    __gpr fc_lock_t lock
    );



/** fc_remove_flow_cam_entry_2_3_4_ofc_head
 *  Called to remove a head flow of CAM entry 2, 3 or 4 or overflow.
 *  It removes the flow as set in the lock and matching payload_address.
 *  This function expects an exclusive lock to be held, as this function releases the exclusive lock after deletion.
 *  Make sure to have acquired an exclusive (plus shared) lock before calling this function.
 *
 * @param bucket_base_ptr               Bucket address base pointer
 * @param entry_base_ptr                Entry address base pointer
 * @param lock                          Lock of the specific cam entry to remove. There is already a shared lock on entry.
 * @param next_entry_idx                Index of head pointer.
 * @return                              Result indicating if entry could be removed or if there was
 *                                      an error in removing it.
 *                                      If entry found and removed: FC_REMOVE_EXACT_MATCH
 *                                      If an entry found but there was some error in removing it: FC_REMOVE_ERROR
 *                                      If an entry was not found FC_REMOVE_ENTRY_NOT_FOUND
 */
__gpr FC_REMOVE_RESULT_CODE fc_remove_flow_cam_entry_2_3_4_ovf_head(
    __mem __addr40 uint32_t *bucket_base_ptr,
    __mem __addr40 uint32_t *entry_base_ptr,
    __gpr fc_lock_t lock,
    __gpr uint32_t next_entry_idx
    );




#endif
