/*
 * Copyright (C) 2008-2014 Netronome Systems, Inc.  All rights reserved.
 */

/* NFP Cluster Target ring intrinsic function definitions */

#ifndef __NFP_CLUSTER_TARGET_RING_H__
#define __NFP_CLUSTER_TARGET_RING_H__

#include <nfp.h>
#include <stdint.h>

#ifndef NFP_LIB_ANY_NFCC_VERSION
    #if (!defined(__NFP_TOOL_NFCC) ||                       \
        (__NFP_TOOL_NFCC < NFP_SW_VERSION(5, 0, 0, 0)) ||   \
        (__NFP_TOOL_NFCC > NFP_SW_VERSION(6, 1, 0, 255)))
        #error "This standard library is not supported for the version of the SDK currently in use."
    #endif
#endif

/** @file nfp_cluster_target_ring.h
 * @addtogroup ct Cluster Target intrinsics
 * @{
 */


/** Ring word size.
 */
typedef enum CT_RING_SIZE
{
    CT_RING_SIZE_128    =   0,  /**< 128 words.
                                * Base address should be aligned to 512 byte boundary,
                                */
    CT_RING_SIZE_256   =   1,   /**< 256 words.
                                * Base address should be aligned to 1024 byte boundary.
                                */
    CT_RING_SIZE_512   =   2,   /**< 512 words.
                                * Base address should be aligned to 2048 byte boundary.
                                */
    CT_RING_SIZE_1024  =   3,   /**< 1024 words (1K).
                                * Base address should be aligned to 4096 byte boundary.
                                */
    CT_RING_SIZE_2048  =   4,   /**< 2048 words (2K).
                                * Base address should be aligned to 8192 byte boundary.
                                */
    CT_RING_SIZE_4096  =   5,   /**< 4096 words (4K).
                                * Base address should be aligned to 16384 byte boundary.
                                */
    CT_RING_SIZE_8192  =   6,   /**< 8192 words (8K).
                                * Base address should be aligned to 32768 byte boundary.
                                */
    CT_RING_SIZE_16384 =   7   /**< 16384 words (16K).
                                * Base address should be aligned to 65536 byte boundary.
                                */
} CT_RING_SIZE;




/** The type of status reported on the status bus for the ring.
 */
typedef enum CT_RING_STATUS
{
    CT_RING_EMPTY       =   0x00,    /**< If ring contains no valid entries.    */
    CT_RING_FULL        =   0x01,    /**< If ring is 3/4 full or greater. */
} CT_RING_STATUS;


/**
 * @name Ring functions for Cluster Target
 * @{
 */

/** Put data on Cluster Target ring.
 *
 * @param data              Data to put onto ring.
 * @param island_number     Island number of the target CTM.
 * @param ring_number       Ring number to initialise (0 to 15).
 * @param count             Number of 32-bit words to put into ring (valid values 1 - 16).
 * @param sync              Type of synchronization to use (sig_done or ctx_swap).
 * @param sig_ptr           Signal to raise upon completion.
 *
 * Write 32-bit words to tail of ring and update tail pointer.
 * If there are not enough empty slots in the ring, no words are added
 * and the tail pointer is not updated.
 * This will raise a status of CT_RING_FULL on the status bus of the ring if 3/4 or greater.
 *
 * @note The valid range for count is between 1 and 16 for Silicon revision B0 or higher and
 *       between 1 and 14 for Silicon revision less than B0.
 *       See Errata 'Misc engine Pull-buffer which stores Pull-data can erroneously overflow
 *       if the post-pull id Command FIFO is not full.'
 */
__intrinsic
void cmd_cluster_target_ring_put(
    __xwrite void *data,
    uint32_t island_number,
    uint32_t ring_number,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
);




/** Read data from Cluster Target ring and put in transfer register.
 *
 * @param data              Data read from ring.
 * @param island_number     Island number of the target CTM.
 * @param ring_number       Ring number to initialise (0 to 15).
 * @param count             Number of 32-bit words to read (valid values 1 - 16).
 * @param sync              Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr           Signal to raise upon completion
 *
 * Read 32-bit words from head of ring and update head pointer.
 * If there are not enough entries in the ring, no valid data is returned
 * and the head pointer is not updated.
 * This will raise a status of CT_RING_FULL on the status bus of the ring if 3/4 or greater.
 */
__intrinsic
void cmd_cluster_target_ring_get(
    __xread void *data,
    uint32_t island_number,
    uint32_t ring_number,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
);



/** Initialise a Cluster Target ring on the local island.
 *
 * @param ring_number       Ring number to initialise (0 to 15)
 * @param base_ptr          32-bit pointer to physical address in CTM
 * @param size              Size of ring of enum CT_RING_SIZE
 * @param status_events     List of statuses of enum CT_RING_STATUS which should be generated
 * @param sync              Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr           Signal to raise upon completion
 *
 * Setup a ring of specified ring number, size and address in the local island in Cluster Target memory.
 * In the example the CT ring is created on the local island.
 *
 * @code
 *
 *    __ctm __addr32 __align(128*sizeof(int32_t)) uint32_t ring_base[128];
 *
 *    uint32_t        ring_index = 2;
 *    uint32_t        island =_island();
 *    uint32_t        count = 3;
 *    SIGNAL              sig;
 *
 *    cmd_cluster_target_ring_init_ptr32(
 *        ring_index,
 *        ring_base,
 *        CT_RING_SIZE_128,
 *        CT_RING_FULL,
 *        ctx_swap,
 *        &sig
 *    );
 *
 *    // put data on the ring
 *    {
 *        __xwrite uint32_t  wr_data[3];
 *
 *        wr_data[0] = 0x12345678;
 *        wr_data[1] = 0x87654321;
 *        wr_data[2] = 0x10101010;
 *
 *        cmd_cluster_target_ring_put((void *)&wr_data[0], island, ring_index, count, ctx_swap, &sig);
 *    }
 *
 *    // get data from the ring
 *    {
 *        __xread  uint32_t  rd_data[3];
 *
 *        cmd_cluster_target_ring_get((void *)&rd_data[0], island, ring_index, count, ctx_swap, &sig);
 *
 *        if (rd_data[0] != 0x12345678)
 *        {
 *            return 0;        // We have an error
 *        }
 *    }
 *
 *
 *   return 1;
 *
 * @endcode
 */
__intrinsic
void cmd_cluster_target_ring_init_ptr32(
    uint32_t ring_number,
    volatile void __addr32 __ctm *base_ptr,
    enum CT_RING_SIZE size,
    enum CT_RING_STATUS status_events,
    sync_t sync,
    SIGNAL *sig_ptr
);

/** Initialise a Cluster Target ring on a specific (non-local) island.
 *
 * @param ring_number       Ring number to initialise (0 to 15)
 * @param base_ptr          40-bit pointer to physical address in CTM with island in top bits.
 * @param size              Size of ring of enum CT_RING_SIZE
 * @param status_events     List of statuses of enum CT_RING_STATUS which should be generated
 * @param sync              Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr           Signal to raise upon completion
 *
 * Setup a ring of specified ring number, size and address in Cluster Target memory. Island is taken
 * from the base_ptr[37:32]. In the example the CT ring is created on remote island 33.
 *
 * @code
 *
 *    __ctm_n(1) __addr40 __align(128*sizeof(uint32_t)) uint32_t ring_base_addr[128];
 *
 *    uint32_t        ring_index = 2;
 *    uint32_t        island = 33;
 *    uint32_t        count = 3;
 *    SIGNAL              sig;
 *
 *    cmd_cluster_target_ring_init_ptr40(
 *        ring_index,
 *        ring_base_addr,
 *        CT_RING_SIZE_128,
 *        CT_RING_FULL,
 *        ctx_swap,
 *        &sig
 *    );
 *
 *    // put data on the ring
 *    {
 *        __xwrite uint32_t wr_data[3];
 *
 *        wr_data[0] = 0x12345678;
 *        wr_data[1] = 0x87654321;
 *        wr_data[2] = 0x10101010;
 *
 *        cmd_cluster_target_ring_put((void *)&wr_data[0], island, ring_index, count, ctx_swap, &sig);
 *    }
 *
 *    // get data from the ring
 *    {
 *        __xread  uint32_t rd_data[3];
 *
 *        cmd_cluster_target_ring_get((void *)&rd_data[0], island, ring_index, count, ctx_swap, &sig);
 *
 *        if (rd_data[0] != 0x12345678)
 *        {
 *            return 0;        // We have an error
 *        }
 *    }
 *
 *    return 1;
 * @endcode
 */
__intrinsic
void cmd_cluster_target_ring_init_ptr40(
    uint32_t ring_number,
    volatile void __addr40 __ctm *base_ptr,
    enum CT_RING_SIZE size,
    enum CT_RING_STATUS status_events,
    sync_t sync,
    SIGNAL *sig_ptr
);


/** Initialise a Cluster Target ring.
 *
 * @param island_number     Island number of the target CTM.
 * @param ring_number       Ring number to initialise (0 to 15)
 * @param base_address      Base address of ring which could be anywhere in CT.
 * @param size              Size of ring of enum CT_RING_SIZE
 * @param status_events     List of statuses of enum CT_RING_STATUS which should be generated
 * @param sync              Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr           Signal to raise upon completion
 *
 * Setup a ring of specified ring number, size and base address in the specified island in Cluster Target memory.
 * The parameter base_address is shifted right by a number depending on the size of the ring.
 * This gives the actual byte address of the base of the ring.
 *
 */
__intrinsic
void cmd_cluster_target_ring_init(
    uint32_t island_number,
    uint32_t ring_number,
    uint32_t base_address,
    enum CT_RING_SIZE size,
    enum CT_RING_STATUS status_events,
    sync_t sync,
    SIGNAL *sig_ptr
);
/** @}
 * @}
 */


#endif /* __NFP_CLUSTER_TARGET_RING_H__ */
