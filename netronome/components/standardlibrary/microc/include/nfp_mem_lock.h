/*
 * Copyright (C) 2008-2014 Netronome Systems, Inc.  All rights reserved.
 */

#ifndef __NFP_MEM_LOCK_H__
#define __NFP_MEM_LOCK_H__

#include <nfp.h>
#include <stdint.h>

#ifndef NFP_LIB_ANY_NFCC_VERSION
    #if (!defined(__NFP_TOOL_NFCC) ||                       \
        (__NFP_TOOL_NFCC < NFP_SW_VERSION(5, 0, 0, 0)) ||   \
        (__NFP_TOOL_NFCC > NFP_SW_VERSION(6, 1, 0, 255)))
        #error "This standard library is not supported for the version of the SDK currently in use."
    #endif
#endif

/** @file nfp_mem_lock.h
 *  @addtogroup mu_lock MU Lock Intrinsics
 *  @{
 */

/** @name Memory lock functions for MEM
 *  @{
 * Memory lock commands support locking of specific ranges of memory as opposed to semantic locking supported
 * by queue locking commands. It allows the user to request a lock on a memory region with a specific size and base
 * address. The size of the locked region is specified in the byte mask and its address in the write transfer register.
 * Memory locks are implemented using 16-bit TCAM lookup with addition.
 */

/** 128-bit memory lock container type. */
typedef struct mem_lock128_t
{
    uint32_t value[4]; /**< Storage for the memory lock entries. */
} mem_lock128_t;

/** 256-bit memory lock container type. */
typedef struct mem_lock256_t
{
    uint32_t value[8]; /**< Storage for the memory lock entries. */
} mem_lock256_t;

/** 384-bit memory lock container type. */
typedef struct mem_lock384_t
{
    uint32_t value[12]; /**< Storage for the memory lock entries. */
} mem_lock384_t;

/** 512-bit memory lock container type. */
typedef struct mem_lock512_t
{
    uint32_t value[16]; /**< Storage for the memory lock entries. */
} mem_lock512_t;


#ifdef __BIGENDIAN
/** Input type for the memory lock operation
 *
 * The address field must be populated before requesting the memory lock.
 */
typedef union mem_lock_in_t
{
    struct
    {
        uint32_t reserved       : 16;       /**< Unused, the regions are addressable to 64k regions */
        uint32_t address        : 16;       /**< Region address to lock
                                                     @note Bit 0 is reserved and is assumed to be 0.  */
    };
    uint32_t value;                         /**< Accessor to entire structure               */

} mem_lock_in_t;

/** Output type for the memory lock operation
 *
 *
 *
 */
typedef union mem_lock_out_t
{
    struct
    {
        uint32_t match_bitf     : 16;       /**< Bitfield of matching entries */
        uint32_t reserved       : 8;        /**< Reserved */
        uint32_t added          : 1;        /**< When set, this field indicates that an entry was added.
                                                 *   @note This field is also set when the memory lock queue is full. */
        uint32_t first_match    : 7;        /**< First matched entry number, or on a new memory lock entry where the entry was added.
                                                 *   @note All-bits in this field are set when the memory lock queue is full. */
    };
    uint32_t value;                         /**< Accessor to entire structure               */
} mem_lock_out_t;

#else
/* __LITTLEENDIAN */

/* See BE description above */
typedef union mem_lock_in_t
{
    struct
    {
        uint32_t address        : 16;
        uint32_t reserved       : 16;
    };
    uint32_t value;
} mem_lock_in_t;

/* See BE description above */
typedef union mem_lock_out_t
{
    struct
    {
        uint32_t first_match    : 7;
        uint32_t added          : 1;
        uint32_t reserved       : 8;
        uint32_t match_bitf     : 16;
    };
    uint32_t value;
} mem_lock_out_t;

#endif /* __BIGENDIAN */


/** Initialize a 128-bit memory lock container in 32-bit addressed MEM in memory.
 *
 * @param mem_lock      Pointer to the memory lock container in MEM
 */
__intrinsic
void cmd_mem_lock128_init_ptr32(__addr32 __mem __align16 mem_lock128_t *mem_lock);


/** Initialize a 128-bit memory lock container in 40-bit addressed MEM.
 *
 * @param mem_lock      Pointer to the memory lock container in MEM
 */
__intrinsic
void cmd_mem_lock128_init_ptr40(__addr40 __mem __align16 mem_lock128_t *mem_lock);



/** Initialize a 256-bit memory lock container in 32-bit addressed MEM.
 *
 * @param mem_lock      Pointer to the memory lock container in MEM
 */
__intrinsic
void cmd_mem_lock256_init_ptr32(__addr32 __mem __align16 mem_lock256_t *mem_lock);


/** Initialize a 256-bit lock container in 40-bit addressed MEM.
 *
 * @param mem_lock      Pointer to the memory lock container in MEM
 */
__intrinsic
void cmd_mem_lock256_init_ptr40(__addr40 __mem __align16 mem_lock256_t *mem_lock);


/** Initialize a 384-bit memory lock container in 32-bit addressed MEM in memory.
 *
 * @param mem_lock      Pointer to the memory lock container in MEM
 */
__intrinsic
void cmd_mem_lock384_init_ptr32(__addr32 __mem __align16 mem_lock384_t *mem_lock);


/** Initialize a 384-bit memory lock container in 40-bit addressed MEM.
 *
 * @param mem_lock      Pointer to the memory lock container in MEM
 */
__intrinsic
void cmd_mem_lock384_init_ptr40(__addr40 __mem __align16 mem_lock384_t *mem_lock);


/** Initialize a 512-bit memory lock container in 32-bit addressed MEM in memory.
 *
 * @param mem_lock      Pointer to the memory lock container in MEM
 */
__intrinsic
void cmd_mem_lock512_init_ptr32(__addr32 __mem __align16 mem_lock512_t *mem_lock);


/** Initialize a 512-bit memory lock container in 40-bit addressed MEM.
 *
 * @param mem_lock      Pointer to the memory lock container in MEM
 */
__intrinsic
void cmd_mem_lock512_init_ptr40(__addr40 __mem __align16 mem_lock512_t *mem_lock);


/** Attempt to acquire a lock entry in a 128-bit lock container in 32-bit addressed MEM, supporting 4 locks .
 *
 * @param mem_lock          Pointer to the memory lock container in MEM
 * @param xfer              Pointer to write xfer register containing the region address
 * @param lsbzeroes         Number of least significant-bits to set to zero in the lock mask. This represent the lock region size.
 * @param sync              Type of synchronization to use. (must be sig_done)
 * @param sig_pair_ptr      Signal pair to raise upon completion
 * @return                  Pointer to the read transfer register containing the result of the lock attempt
 *
 * @note                    The result is returned in a mem_lock_out_t structure.

 * @see                     mem_lock_in_t, mem_lock_out_t
 *
 *
 */
__intrinsic
__xread mem_lock_out_t *cmd_mem_lock128_ptr32(
    __addr32 __mem __align16 mem_lock128_t  *mem_lock,
    __xwrite mem_lock_in_t                  *xfer,
    uint32_t                                lsbzeroes,
    sync_t                                  sync,
    SIGNAL_PAIR                             *sig_pair_ptr);


/** Attempt to acquire a lock entry in a 128-bit lock container in 40-bit addressed MEM, supporting 4 locks .
 *
 * @param mem_lock          Pointer to the memory lock container in MEM
 * @param xfer              Pointer to write xfer register containing the region address
 * @param lsbzeroes         Number of least significant-bits to set to zero in the lock mask. This represent the lock region size.
 * @param sync              Type of synchronization to use. (must be sig_done)
 * @param sig_pair_ptr      Signal pair to raise upon completion
 * @return                  Pointer to the read transfer register containing the result of the lock attempt
 *
 * @note                    The result is returned in a mem_lock_out_t structure.

 * @see                     mem_lock_in_t, mem_lock_out_t
 *
 *
 */
__intrinsic
__xread mem_lock_out_t *cmd_mem_lock128_ptr40(
    __addr40 __mem __align16 mem_lock128_t  *mem_lock,
    __xwrite mem_lock_in_t                  *xfer,
    uint32_t                                lsbzeroes,
    sync_t                                  sync,
    SIGNAL_PAIR                             *sig_pair_ptr);


/** Attempt to acquire a lock entry in a 256-bit lock container in 32-bit addressed MEM, supporting 8 locks.
 *
 * @param mem_lock          Pointer to the memory lock container in MEM
 * @param xfer              Pointer to write xfer register containing the region address
 * @param lsbzeroes         Number of least significant-bits to set to zero in the lock mask. This represent the lock region size.
 * @param sync              Type of synchronization to use. (must be sig_done)
 * @param sig_pair_ptr      Signal pair to raise upon completion
 * @return                  Pointer to the read transfer register containing the result of the lock attempt
 *
 * @note                    The result is returned in a mem_lock_out_t structure.

 * @see                     mem_lock_in_t, mem_lock_out_t
 *
 *
 */
__intrinsic
__xread mem_lock_out_t *mem_lock256_ptr32(
    __addr32 __mem __align16 mem_lock256_t *mem_lock,
    __xwrite mem_lock_in_t *xfer,
    uint32_t lsbzeroes,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr);


/** Attempt to acquire a lock entry in a 256-bit lock container in 40-bit addressed MEM, supporting 8 locks.
 *
 * @param mem_lock          Pointer to the memory lock container in MEM
 * @param xfer              Pointer to write xfer register containing the region address
 * @param lsbzeroes         Number of least significant-bits to set to zero in the lock mask. This represent the lock region size.
 * @param sync              Type of synchronization to use. (must be sig_done)
 * @param sig_pair_ptr      Signal pair to raise upon completion
 * @return                  Pointer to the read transfer register containing the result of the lock attempt
 *
 * @note                    The result is returned in a mem_lock_out_t structure.

 * @see                     mem_lock_in_t, mem_lock_out_t
 *
 *
 */
__intrinsic
__xread mem_lock_out_t *mem_lock256_ptr40(
    __addr40 __mem __align16 mem_lock256_t *mem_lock,
    __xwrite mem_lock_in_t *xfer,
    uint32_t lsbzeroes,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr);


/** Attempt to acquire a lock entry in a 384-bit lock container in 32-bit addressed MEM, supporting 12 locks.
 *
 * @param mem_lock          Pointer to the memory lock container in MEM
 * @param xfer              Pointer to write xfer register containing the region address
 * @param lsbzeroes         Number of least significant-bits to set to zero in the lock mask. This represent the lock region size.
 * @param sync              Type of synchronization to use. (must be sig_done)
 * @param sig_pair_ptr      Signal pair to raise upon completion
 * @return                  Pointer to the read transfer register containing the result of the lock attempt
 *
 * @note                    The result is returned in a mem_lock_out_t structure.

 * @see                     mem_lock_in_t, mem_lock_out_t
 *
 *
 */
__intrinsic
__xread mem_lock_out_t *mem_lock384_ptr32(
    __addr32 __mem __align16 mem_lock384_t *mem_lock,
    __xwrite mem_lock_in_t *xfer,
    uint32_t lsbzeroes,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr);


/** Attempt to acquire a lock entry in a 384-bit lock container in 40-bit addressed MEM, supporting 12 locks.
 *
 * @param mem_lock          Pointer to the memory lock container in MEM
 * @param xfer              Pointer to write xfer register containing the region address
 * @param lsbzeroes         Number of least significant-bits to set to zero in the lock mask. This represent the lock region size.
 * @param sync              Type of synchronization to use. (must be sig_done)
 * @param sig_pair_ptr      Signal pair to raise upon completion
 * @return                  Pointer to the read transfer register containing the result of the lock attempt
 *
 * @note                    The result is returned in a mem_lock_out_t structure.

 * @see                     mem_lock_in_t, mem_lock_out_t
 *
 *
 */

__intrinsic
__xread mem_lock_out_t *mem_lock384_ptr40(
    __addr40 __mem __align16 mem_lock384_t *mem_lock,
    __xwrite mem_lock_in_t *xfer,
    uint32_t lsbzeroes,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr);


/** Attempt to acquire a lock entry in a 512-bit lock container in 32-bit addressed MEM, supporting 16 locks.
 *
 * @param mem_lock          Pointer to the memory lock container in MEM
 * @param xfer              Pointer to write xfer register containing the region address
 * @param lsbzeroes         Number of least significant-bits to set to zero in the lock mask. This represent the lock region size.
 * @param sync              Type of synchronization to use. (must be sig_done)
 * @param sig_pair_ptr      Signal pair to raise upon completion
 * @return                  Pointer to the read transfer register containing the result of the lock attempt
 *
 * @note                    The result is returned in a mem_lock_out_t structure.

 * @see                     mem_lock_in_t, mem_lock_out_t
 *
 *
 */
__intrinsic
__xread mem_lock_out_t *mem_lock512_ptr32(
    __addr32 __mem __align16 mem_lock512_t *mem_lock,
    __xwrite mem_lock_in_t *xfer,
    uint32_t lsbzeroes,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr);


/** Attempt to acquire a lock entry in a 512-bit lock container in 40-bit addressed MEM, supporting 16 locks.
 *
 * @param mem_lock          Pointer to the memory lock container in MEM
 * @param xfer              Pointer to write xfer register containing the region address
 * @param lsbzeroes         Number of least significant-bits to set to zero in the lock mask. This represent the lock region size.
 * @param sync              Type of synchronization to use. (must be sig_done)
 * @param sig_pair_ptr      Signal pair to raise upon completion
 * @return                  Pointer to the read transfer register containing the result of the lock attempt
 *
 * @note                    The result is returned in a mem_lock_out_t structure.

 * @see                     mem_lock_in_t, mem_lock_out_t
 *
 *
 */
__intrinsic
__xread mem_lock_out_t *mem_lock512_ptr40(
    __addr40 __mem __align16 mem_lock512_t *mem_lock,
    __xwrite mem_lock_in_t *xfer,
    uint32_t lsbzeroes,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr);


/** Unlock a memory lock entry in a 128-bit lock container in 32-bit addressed MEM
 *
 * @param mem_lock          Pointer to the memory lock container in MEM
 * @param index             Memory lock entry to unlock (0 is the first entry)
 * @param sync              Type of synchronization to use. (sig_done or ctx_swap)
 * @param sig_ptr           Signal to raise upon completion
 *
 * @note The maximum index is 3.
 *
 *
 */
__intrinsic
void cmd_mem_unlock128_ptr32(
    __addr32 __mem __align16 mem_lock128_t  *mem_lock,
    uint32_t                                index,
    sync_t                                  sync,
    SIGNAL                                  *sig_ptr);


/** Unlock a memory lock entry in a 128-bit lock container in 40-bit addressed MEM
 *
 * @param mem_lock          Pointer to the memory lock container in MEM
 * @param index             Memory lock entry to unlock (0 is the first entry)
 * @param sync              Type of synchronization to use. (sig_done or ctx_swap)
 * @param sig_ptr           Signal to raise upon completion
 *
 * @note The maximum index is 3.
 *
 *
 */
__intrinsic
void cmd_mem_unlock128_ptr40(
    __addr40 __mem __align16 mem_lock128_t  *mem_lock,
    uint32_t                                index,
    sync_t                                  sync,
    SIGNAL                                  *sig_ptr);


/** Unlock a memory lock entry in a 256-bit lock container in 32-bit addressed MEM
 *
 * @param mem_lock          Pointer to the memory lock container in MEM
 * @param index             Memory lock entry to unlock (0 is the first entry)
 * @param sync              Type of synchronization to use. (sig_done or ctx_swap)
 * @param sig_ptr           Signal to raise upon completion
 *
 * @note The maximum index is 7.
 *
 *
 */
__intrinsic
void mem_unlock256_ptr32(
    __addr32 __mem __align16 mem_lock256_t *mem_lock,
    uint32_t index,
    sync_t sync,
    SIGNAL *sig_ptr);


/** Unlock a memory lock entry in a 256-bit lock container in 40-bit addressed MEM
 *
 * @param mem_lock          Pointer to the memory lock container in MEM
 * @param index             Memory lock entry to unlock (0 is the first entry)
 * @param sync              Type of synchronization to use. (sig_done or ctx_swap)
 * @param sig_ptr           Signal to raise upon completion
 *
 * @note The maximum index is 7.
 *
 *
 */
__intrinsic
void mem_unlock256_ptr40(
    __addr40 __mem __align16 mem_lock256_t *mem_lock,
    uint32_t index,
    sync_t sync,
    SIGNAL *sig_ptr);


/** Unlock a memory lock entry in a 384-bit lock container in 32-bit addressed MEM
 *
 * @param mem_lock          Pointer to the memory lock container in MEM
 * @param index             Memory lock entry to unlock (0 is the first entry)
 * @param sync              Type of synchronization to use. (sig_done or ctx_swap)
 * @param sig_ptr           Signal to raise upon completion
 *
 * @note The maximum index is 11.
 *
 *
 */
__intrinsic
void mem_unlock384_ptr32(
    __addr32 __mem __align16 mem_lock384_t *mem_lock,
    uint32_t index,
    sync_t sync,
    SIGNAL *sig_ptr);


/** Unlock a memory lock entry in a 384-bit lock container in 40-bit addressed MEM
 *
 * @param mem_lock          Pointer to the memory lock container in MEM
 * @param index             Memory lock entry to unlock (0 is the first entry)
 * @param sync              Type of synchronization to use. (sig_done or ctx_swap)
 * @param sig_ptr           Signal to raise upon completion
 *
 * @note The maximum index is 11.
 *
 *
 */
__intrinsic
void mem_unlock384_ptr40(
    __addr40 __mem __align16 mem_lock384_t *mem_lock,
    uint32_t index,
    sync_t sync,
    SIGNAL *sig_ptr);


/** Unlock a memory lock entry in a 512-bit lock container in 32-bit addressed MEM
 *
 * @param mem_lock          Pointer to the memory lock container in MEM
 * @param index             Memory lock entry to unlock (0 is the first entry)
 * @param sync              Type of synchronization to use. (sig_done or ctx_swap)
 * @param sig_ptr           Signal to raise upon completion
 *
 * @note The maximum index is 15.
 *
 *
 */
__intrinsic
void mem_unlock512_ptr32(
    __addr32 __mem __align16 mem_lock512_t *mem_lock,
    uint32_t index,
    sync_t sync,
    SIGNAL *sig_ptr);


/** Unlock a memory lock entry in a 512-bit lock container in 40-bit addressed MEM
 *
 * @param mem_lock          Pointer to the memory lock container in MEM
 * @param index             Memory lock entry to unlock (0 is the first entry)
 * @param sync              Type of synchronization to use. (sig_done or ctx_swap)
 * @param sig_ptr           Signal to raise upon completion
 *
 * @note The maximum index is 15.
 *
 *
 */
__intrinsic
void mem_unlock512_ptr40(
    __addr40 __mem __align16 mem_lock512_t *mem_lock,
    uint32_t index,
    sync_t sync,
    SIGNAL *sig_ptr);


/** @}
 * @}
 */

#endif /* __NFP_MEM_LOCK_H__ */
