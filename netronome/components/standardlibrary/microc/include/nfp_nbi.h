/*
 * Copyright (C) 2008-2014 Netronome Systems, Inc.  All rights reserved.
 */

/* NFP NBI  intrinsic function definitions */

#ifndef __NFP_NBI_H__
#define __NFP_NBI_H__

#include <nfp.h>
#include <stdint.h>

#ifndef NFP_LIB_ANY_NFCC_VERSION
    #if (!defined(__NFP_TOOL_NFCC) ||                       \
        (__NFP_TOOL_NFCC < NFP_SW_VERSION(5, 0, 0, 0)) ||   \
        (__NFP_TOOL_NFCC > NFP_SW_VERSION(6, 1, 0, 255)))
        #error "This standard library is not supported for the version of the SDK currently in use."
    #endif
#endif


/** @file nfp_nbi.h
 * @addtogroup nbi NBI Intrinsics
 * @{
 */

 /** @name General functions for NBI.
 *  @{
 */

/** Write data to NBI.
 *
 * @param data                  Xfer register(s) containing data to write to memory.
 * @param address               40 bit address in NBI.
 *                               See 6xxx databook for recommended addressing mode for address bits.
 * @param count                 Number of 64-bit words to read (1 - 16).
 * @param sync                  Type of synchronization to use (sig_done  or ctx_swap).
 * @param sig_ptr               Signal to raise upon completion.
 *
 */
__intrinsic
void cmd_nbi_write(
    __xwrite void *data,
    volatile void __addr40 __mem *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
);

/** Read data from ILA internal target.
 *
 * @param data                  Xfer register(s) containing data read from memory.
 * @param address               40 bit address in NBI.
 *                              See 6xxx databook for recommended addressing mode for address bits.
 * @param count                 Number of 64-bit words to read (1 - 16).
 * @param sync                  Type of synchronization to use (sig_done  or ctx_swap).
 * @param sig_ptr               Signal to raise upon completion.
 *
 */
__intrinsic
void cmd_nbi_read(
    __xread void *data,
    volatile void __addr40 __mem *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
);

/**
 *  @}
 * @}
 */

#endif /* __NFP_NBI_H__ */
