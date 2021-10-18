/*
 * Copyright (C) 2008-2014 Netronome Systems, Inc.  All rights reserved.
 */

/* Crypto operations */

#ifndef __NFP_CRYPTO_H__
#define __NFP_CRYPTO_H__

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


/** @file nfp_crypto.h
 * @addtogroup crypto Crypto intrinsics
 * @{
 */

/** @name General access functions for Crypto
 * @{
 */

/** Read data from Crypto into transfer register.
 *
 * @param data     Xfer register(s) to store data read from memory
 * @param address  32 bit address in Crypto SRAM to read data from
 * @param count    Number of 64-bit words to read (1 - 16)
 * @param sync     Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr  Signal to raise upon completion
 */
__intrinsic
void cmd_crypto_read(
    __xread void *data,
    uint32_t address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
);

/** Read data from Crypto into DRAM in indirect mode.
 *
 * @param data     Xfer register(s) to store data read from memory
 * @param address  32 bit address in Crypto SRAM to read data from
 * @param max_nn   Maximum number of 64-bit words to read (1 - 16)
 * @param ind      Indirect word
 * @param sync     Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr  Signal to raise upon completion
 */
__intrinsic
void cmd_crypto_read_ind(
    __xread void *data,
    uint32_t address,
    uint32_t max_nn,
    generic_ind_t ind,
    sync_t sync,
    SIGNAL *sig_ptr
);


/** Write data to Crypto from MEM.
 *
 * @param data     Xfer register(s) containing data to write to memory
 * @param address  32 bit address in Crypto SRAM to write data to
 * @param count    Number of 64-bit words to write (1 - 16)
 * @param sync     Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr  Signal to raise upon completion
 */
__intrinsic
void cmd_crypto_write(
    __xwrite void *data,
    uint32_t address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
);

/** Write data to Crypto from DRAM in indirect mode.
 *
 * @param data     Xfer register(s) containing data to write to memory
 * @param address  32 bit address in Crypto SRAM to write data to
 * @param max_nn   Maximum number of 64-bit words to write (1 - 16)
 * @param ind      Indirect word
 * @param sync     Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr  Signal to raise upon completion
 */
__intrinsic
void cmd_crypto_write_ind(
    __xwrite void *data,
    uint32_t address,
    uint32_t max_nn,
    generic_ind_t ind,
    sync_t sync,
    SIGNAL *sig_ptr
);

/** Write data to Crypto FIFO.
 *
 * @param data     Xfer register(s) containing data (command) to write to FIFO
 * @param address  32 bit address in Crypto SRAM to write data to
 * @param count    Number of 64-bit words to write (1 - 16)
 * @param sync     Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr  Signal to raise upon completion
 */
__intrinsic
void cmd_crypto_write_fifo(
    __xwrite void *data,
    uint32_t address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
);

/** Write data to Crypto FIFO from DRAM in indirect mode.
 *
 * @param data     Xfer register(s) containing data (command) to write to FIFO
 * @param address  32 bit address in Crypto SRAM to write data to
 * @param max_nn   Maximum number of 64-bit words to write
 * @param ind      Indirect word
 * @param sync     Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr  Signal to raise upon completion
 */
__intrinsic
void cmd_crypto_write_fifo_ind(
    __xwrite void *data,
    uint32_t address,
    uint32_t max_nn,
    generic_ind_t ind,
    sync_t sync,
    SIGNAL *sig_ptr
);

/** @}
 * @}
 */

#endif /* __NFP_CRYPTO_H__ */
