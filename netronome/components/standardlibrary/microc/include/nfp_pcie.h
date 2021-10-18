/*
 * Copyright (C) 2008-2014 Netronome Systems, Inc.  All rights reserved.
 */

/* NFP PCI Express intrinsic function definitions */

#ifndef __NFP_PCIE_H__
#define __NFP_PCIE_H__

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


/** @file nfp_pcie.h
 * @addtogroup pcie PCI Express Intrinsics
 * @{
 */


/** read from PCIe into transfer registers, 40-bit addr
 *
 * @param data     Address to read data into
 * @param address  Address to read data from (aligned on 4-byte boundary)
 * @param count    Number of 32-bit words to read (1-32).
 * @param sync     Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr  Signal to raise upon completion
 */
__intrinsic
void cmd_pcie_read_ptr40(
    __xread void *data,
    volatile void __addr40 *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
    );


/** write to PCIe from transfer register, 40-bit addr
 *
 * @param data     Data to write to PCIe
 * @param address  Address to write data to (aligned on 4-byte boundary)
 * @param count    Number of 32-bit words to write (1-32).
 * @param sync     Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr  Signal to raise upon completion
 */
__intrinsic
void cmd_pcie_write_ptr40(
    __xwrite void *data,
    volatile void __addr40 *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
    );


/** read from PCIe internal target into transfer registers, 40-bit addr
 *
 * @param data     Xfer reg to read data into
 * @param address  Address, [9:16] target number, [15:0] target addr
 * @param count    Number of 32-bit words to read (1-32).
 * @param sync     Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr  Signal to raise upon completion
 */
__intrinsic
void cmd_pcie_read_pci_ptr40(
    __xread void *data,
    volatile void __addr40 *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
    );


/** write to PCIe internal target from transfer register, 40-bit addr
 *
 * @param data     Xfer reg to write to PCIe
 * @param address  Address, [9:16] target number, [15:0] target addr
 * @param count    Number of 32-bit words to write (1-32).
 * @param sync     Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr  Signal to raise upon completion
 */
__intrinsic
void cmd_pcie_write_pci_ptr40(
    __xwrite void *data,
    volatile void __addr40 *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
    );


/** Read data from PCIe into transfer registers; overriding the PCIe Requester ID
 * to use for the PCIe transaction, 40-bit addr
 *
 * @param data          Address to read data into
 * @param requester_id  PCIe requester ID to override
 * @param address       40-bit address
 * @param count         Number of 32-bit words to read (1-32).
 * @param sync          Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr       Signal to raise upon completion
 *
 */
__intrinsic
void cmd_pcie_read_rid_ptr40(
    __xread void *data,
    uint32_t requester_id,
    volatile void __addr40 *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
    );


/** Write data to PCIe from transfer register; overriding the PCIe Requester ID
 * to use for the PCIe transaction, 40-bit addr
 *
 * @param data          Data to write to PCIe
 * @param requester_id  PCIe requester ID to override
 * @param address       Address to write data to (aligned on 4-byte boundary)
 * @param count         Number of 32-bit words to write (1-32).
 * @param sync          Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr       Signal to raise upon completion
 *
 */
__intrinsic
void cmd_pcie_write_rid_ptr40(
    __xwrite void *data,
    uint32_t requester_id,
    volatile void __addr40 *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
    );


/** @name General memory access functions for PCIe.
 *  @{
 */
/** 32-bit read from PCIe into transfer registers.
 *
 * @param data     Address to read data into
 * @param address  Address to read data from (aligned on 4-byte boundary)
 * @param count    Number of 32-bit words to read (1-32).
 * @param sync     Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr  Signal to raise upon completion
 */
__intrinsic
void cmd_pcie_read_ptr32(
    __xread void *data,
    volatile void *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
    );


/** Read data from PCIe into transfer register indirect mode.
 *
 * @param data     Address to read data into
 * @param address  Address to read data from (aligned on 4-byte boundary)
 * @param max_nn   Maximum number of 32-bit words to read (1-32)
 * @param ind      Indirect word
 * @param sync     Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr  Signal to raise upon completion
 */
__intrinsic
void cmd_pcie_read_ind_ptr32(
    __xread void *data,
    volatile void *address,
    uint32_t max_nn,
    generic_ind_t ind,
    sync_t sync,
    SIGNAL *sig_ptr
    );


/** 32-bit write to PCIe from transfer register.
 *
 * @param data     Data to write to PCIe
 * @param address  Address to write data to (aligned on 4-byte boundary)
 * @param count    Number of 32-bit words to write (1-32).
 * @param sync     Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr  Signal to raise upon completion
 */
__intrinsic
void cmd_pcie_write_ptr32(
    __xwrite void *data,
    volatile void *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
    );


/** Write data to PCIe from transfer register indirect mode.
 *
 * @param data     Data to write to PCIe
 * @param address  Address to write data to (aligned on 4-byte boundary)
 * @param max_nn   Maximum number of 32-bit words to write (1-32)
 * @param ind      Indirect word
 * @param sync     Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr  Signal to raise upon completion
 */
__intrinsic
void cmd_pcie_write_ind_ptr32(
    __xwrite void *data,
    volatile void *address,
    uint32_t max_nn,
    generic_ind_t ind,
    sync_t sync,
    SIGNAL *sig_ptr
    );


/** Read 4-byte words from PCIe internal into transfer register.
 *
 * @param data     Address to read data into
 * @param address  Address to read from
 * @param count    Number of 32-bit words to read (1-16).
 * @param sync     Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr  Signal to raise upon completion
 *
 */
__intrinsic
void cmd_pcie_read_pci_ptr32(
    __xread void *data,
    volatile void *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
    );


/** Read data from PCIe internal to transfer register indirect mode.
 *
 * @param data     Address to read data into
 * @param address  Address to read data from
 * @param max_nn   Maximum number of 32-bit words to read (1-16)
 * @param ind      Indirect word
 * @param sync     Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr  Signal to raise upon completion
 */
__intrinsic
void cmd_pcie_read_pci_ind_ptr32(
    __xread void *data,
    volatile void *address,
    uint32_t max_nn,
    generic_ind_t ind,
    sync_t sync,
    SIGNAL *sig_ptr
    );


/** Write 4-byte words to PCI internal from transfer register.
 *
 * @param data     Data to write to PCIe internal
 * @param address  Address to write data to (aligned on 4-byte boundary)
 * @param count    Number of 32-bit words to write (1-16).
 * @param sync     Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr  Signal to raise upon completion
 */
__intrinsic
void cmd_pcie_write_pci_ptr32(
    __xwrite void *data,
    volatile void *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
    );


/** Write data to PCI internal from transfer register indirect mode.
 *
 * @param data     Data to write to PCIe internal
 * @param address  Address to write data to
 * @param max_nn   Maximum number of 32-bit words to write (1-16)
 * @param ind      Indirect word
 * @param sync     Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr  Signal to raise upon completion
 *
 */
__intrinsic
void cmd_pcie_write_pci_ind_ptr32(
    __xwrite void *data,
    volatile void *address,
    uint32_t max_nn,
    generic_ind_t ind,
    sync_t sync,
    SIGNAL *sig_ptr
    );


/** Read data from PCIe into transfer registers; overriding the PCIe Requester ID
 * to use for the PCIe transaction.
 *
 * @param data          Address to read data into
 * @param requester_id  PCIe requester ID to override
 * @param address       Address to read data from (aligned on 4-byte boundary)
 * @param count         Number of 32-bit words to read (1-32).
 * @param sync          Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr       Signal to raise upon completion
 *
 */
__intrinsic
void cmd_pcie_read_rid_ptr32(
    __xread void *data,
    uint32_t requester_id,
    volatile void *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
    );


/** Read 4-byte words from PCIe internal into transfer register, indirect mode
 *
 * @param data     Address to read data into
 * @param address  Address to read from (aligned on 4-byte boundary)
 * @param max_nn   Maximum number of 32-bit words to read (1-32)
 * @param ind      Indirect word
 * @param sync     Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr  Signal to raise upon completion
 *
 */
__intrinsic
void cmd_pcie_read_rid_ind_ptr32(
    __xread void *data,
    volatile void *address,
    uint32_t max_nn,
    generic_ind_t ind,
    sync_t sync,
    SIGNAL *sig_ptr
    );


/** Write data to PCIe from transfer register; overriding the PCIe Requester ID
 * to use for the PCIe transaction.
 *
 * @param data          Data to write to PCIe
 * @param requester_id  PCIe requester ID to override
 * @param address       Address to write data to (aligned on 4-byte boundary)
 * @param count         Number of 32-bit words to write (1-32).
 * @param sync          Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr       Signal to raise upon completion
 *
 */
__intrinsic
void cmd_pcie_write_rid_ptr32(
    __xwrite void *data,
    uint32_t requester_id,
    volatile void *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
    );


/** Write data to PCIe from transfer register; overriding the PCIe Requester ID
 * to use for the PCIe transaction, indirect mode.
 *
 * @param data          Data to write to PCIe
 * @param address       Address to write data to (aligned on 4-byte boundary)
 * @param max_nn        Maximum number of 32-bit words to write (1-32).
 * @param ind           Indirect word
 * @param sync          Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr       Signal to raise upon completion
 *
 */
__intrinsic
void cmd_pcie_write_rid_ind_ptr32(
    __xwrite void *data,
    volatile void *address,
    uint32_t max_nn,
    generic_ind_t ind,
    sync_t sync,
    SIGNAL *sig_ptr
    );


/**
 *  @}
 * @}
 */


#endif /* __NFP_PCIE_H__ */
