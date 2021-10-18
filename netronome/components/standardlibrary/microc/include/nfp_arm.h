/*
 * Copyright (C) 2008-2014 Netronome Systems, Inc.  All rights reserved.
 */

/* NFP ARM operations */

#ifndef __NFP_ARM_H__
#define __NFP_ARM_H__

#include <nfp.h>
#include <stdint.h>
#include <nfp_override.h>

#ifndef NFP_LIB_ANY_NFCC_VERSION
    #if (!defined(__NFP_TOOL_NFCC) ||                       \
        (__NFP_TOOL_NFCC < NFP_SW_VERSION(5, 0, 0, 0)) ||   \
        (__NFP_TOOL_NFCC > NFP_SW_VERSION(6, 1, 0, 255)))
        #error "This standard library is not supported for the version of the SDK currently in use."
    #endif
#endif

/** @file nfp_arm.h
 * @addtogroup arm ARM Intrinsics
 * @{
 */

/** @name General functions for ARM
 * @{
 */

/** Read data from ARM into transfer registers.
 *
 * @param data     Address to read data into
 * @param address  Address to read data from
 * @param count    Number of 32-bit words to read
 * @param sync     Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr  Signal to raise upon completion
 */
__intrinsic
void cmd_arm_read(
    __xread void *data,
    volatile void __sram *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
    );


/** Read data from ARM into transfer register indirect mode.
 *
 * @param data     Address to read data into
 * @param address  Address to read data from
 * @param max_nn   Maximum number of 32-bit words to read
 * @param ind      Indirect word
 * @param sync     Type off synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr  Signal to raise upon completion
 */
__intrinsic
void cmd_arm_read_ind(
    __xread void *data,
    volatile void __sram *address,
    uint32_t max_nn,
    generic_ind_t ind,
    sync_t sync,
    SIGNAL *sig_ptr
    );


/** Write data to ARM from transfer register.
 *
 * @param data     Data to write
 * @param address  Address to write data to
 * @param count    Number of 32-bit words to write
 * @param sync     Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr  Signal to raise upon completion
 */
__intrinsic
void cmd_arm_write(
    __xwrite void *data,
    volatile void __sram *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
    );


/** Write data to ARM from transfer register indirect mode.
 *
 * @param data     Data to write
 * @param address  Address to write data to
 * @param max_nn   Maximum number of 32-bit words to write
 * @param ind      Indirect word
 * @param sync     Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr  Signal to raise upon completion
 */
__intrinsic
void cmd_arm_write_ind(
    __xwrite void *data,
    volatile void __sram *address,
    uint32_t max_nn,
    generic_ind_t ind,
    sync_t sync,
    SIGNAL *sig_ptr
    );

/** @}
 * @}
 */


#endif /* __NFP_ARM_H__ */
