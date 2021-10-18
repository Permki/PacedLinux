/*
 * Copyright (C) 2008-2014 Netronome Systems, Inc.  All rights reserved.
 */

/* DRAM extended functions */

#ifndef __NFP_MEM_H__
#define __NFP_MEM_H__

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


/* For backwards compatibility */
typedef SIGNAL MEM_BULK_SIGNAL;

/** Read data from memory into transfer register.
 *
 * @param data          Xfer register(s) to store data read from memory
 * @param address       32-bit memory address to read from
 * @param count         Number of 64-bit words to read (1-16)
 * @param sync          Type of synchronization to use  (sig_done or ctx_swap)
 * @param sig_ptr       Signal to raise upon completion
 */
__intrinsic
void cmd_mem_read64_ptr32(
    __xread void *data,
    volatile void __addr32 __mem *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
    );


/** Read data from memory into transfer registers in indirect mode.
 *
 * @param data          Xfer register(s) to store data read from memory
 * @param address       Memory address to read from
 * @param max_nn        Maximum number of 64-bit words to read (1-16)
 * @param ind           Indirect word
 * @param sync          Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr       Signal to raise upon completion
 */
__intrinsic
void cmd_mem_read64_ind_ptr32(
    __xread void *data,
    volatile void __addr32 __mem *address,
    uint32_t max_nn,
    generic_ind_t ind,
    sync_t sync,
    SIGNAL *sig_ptr
    );


/** Read data from memory into transfer registers in indirect mode.
 *
 * @param data          Xfer register(s) to store data read from memory
 * @param address       40-bit memory address to read from
 * @param max_nn        Maximum number of 64-bit words to read (1-16)
 * @param ind           Indirect word
 * @param sync          Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr       Signal to raise upon completion
 *
 */

__intrinsic
void cmd_mem_read64_ind_ptr40(
    __xread void *data,
    volatile void __addr40 __mem *address,
    uint32_t max_nn,
    generic_ind_t ind,
    sync_t sync,
    SIGNAL *sig_ptr
    );


/** Read data from memory into transfer register.
 *
 * @param data          Xfer register(s) to store data read from memory
 * @param address       40-bit memory address to read from
 * @param count         Number of 64-bit words to read (1-16)
 * @param sync          Type of synchronization to use  (sig_done or ctx_swap)
 * @param sig_ptr       Signal to raise upon completion
 *
 */

__intrinsic
void cmd_mem_read64_ptr40(
    __xread void *data,
    volatile void __addr40 __mem *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
    );



/** Write data to memory from transfer registers.
 *
 * @param data          Xfer register(s) containing data to write to memory
 * @param address       32-bit memory address at which operation should be performed
 * @param count         Number of 64-bit words to write (1-16)
 * @param sync          Type of synchronization to use  (sig_done or ctx_swap)
 * @param sig_ptr       Signal to raise upon completion
 */
__intrinsic
void cmd_mem_write64_ptr32(
    __xwrite void *data,
    volatile void __addr32 __mem *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
    );


/** Write data to memory from transfer registers.
 *
 * @param data          Xfer register(s) containing data to write to memory
 * @param address       40-bit memory address at which operation should be performed
 * @param count         Number of 64-bit words to write (1-16)
 * @param sync          Type of synchronization to use  (sig_done or ctx_swap)
 * @param sig_ptr       Signal to raise upon completion
 *
 */

__intrinsic
void cmd_mem_write64_ptr40(
    __xwrite void *data,
    volatile void __addr40 __mem *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
    );

/** Read data from memory into transfer registers (little-endian).
 *
 * @param data     Xfer register(s) to store data read from memory
 * @param address 32-bit memory address to read from
 * @param count    Number of 64-bit words to read (1-16)
 * @param sync     Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr  Signal to raise upon completion
 */
__intrinsic
void cmd_mem_read64_le_ptr32(
    __xread void *data,
    volatile void __addr32 __mem *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
    );

/** Read data from memory into transfer registers (little-endian).
 *
 * @param data     Xfer register(s) to store data read from memory
 * @param address  40-bit memory address to read from
 * @param count    Number of 64-bit words to read (1-16)
 * @param sync     Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr  Signal to raise upon completion
 *
 */

__intrinsic
void cmd_mem_read64_le_ptr40(
    __xread void *data,
    volatile void __addr40 __mem *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
    );

/** Read data from memory into transfer registers in indirect mode
 * (little-endian).
 *
 * @param data     Xfer register(s) to store data read from memory
 * @param address  Memory address to read from
 * @param max_nn   Maximum number of 64-bit words to read (1-16)
 * @param ind      Indirect word
 * @param sync     Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr  Signal to raise upon completion
 */
__intrinsic
void cmd_mem_read64_le_ind_ptr32(
    __xread void *data,
    volatile void __addr32 __mem *address,
    uint32_t max_nn,
    generic_ind_t ind,
    sync_t sync,
    SIGNAL *sig_ptr
    );


/** Read data from memory into transfer registers in indirect mode
 * (little-endian).
 *
 * @param data     Xfer register(s) to store data read from memory
 * @param address  40-bit memory address to read from
 * @param max_nn   Maximum number of 64-bit words to read (1-16)
 * @param ind      Indirect word
 * @param sync     Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr  Signal to raise upon completion
 *
 */

__intrinsic
void cmd_mem_read64_le_ind_ptr40(
    __xread void *data,
    volatile void __addr40 __mem *address,
    uint32_t max_nn,
    generic_ind_t ind,
    sync_t sync,
    SIGNAL *sig_ptr
    );

/** Read data from memory with byte swapping into transfer registers.
 *
 * @param data     Xfer register(s) to store data read from memory
 * @param address 32-bit memory address to read from
 * @param count    Number of 64-bit words to read (1-16)
 * @param sync     Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr  Signal to raise upon completion
 */
__intrinsic
void cmd_mem_read64_swap_ptr32(
    __xread void *data,
    volatile void __addr32 __mem *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
    );

/** Read data from memory with byte swapping into transfer registers.
 *
 * @param data     Xfer register(s) to store data read from memory
 * @param address  40-bit memory address to read from
 * @param count    Number of 64-bit words to read (1-16)
 * @param sync     Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr  Signal to raise upon completion
 *
 */

__intrinsic
void cmd_mem_read64_swap_ptr40(
    __xread void *data,
    volatile void __addr40 __mem *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
    );


/** Read data from memory with byte swapping into transfer registers in indirect mode.
 *
 * @param data     Xfer register(s) to store data read from memory
 * @param address  Memory address to read from
 * @param max_nn   Maximum number of 64-bit words to read (1-16)
 * @param ind      Indirect word
 * @param sync     Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr  Signal to raise upon completion
 */
__intrinsic
void cmd_mem_read64_swap_ind_ptr32(
    __xread void *data,
    volatile void __addr32 __mem *address,
    uint32_t max_nn,
    generic_ind_t ind,
    sync_t sync,
    SIGNAL *sig_ptr
    );


/** Read data from memory with byte swapping into transfer registers in indirect mode.
 *
 * @param data     Xfer register(s) to store data read from memory
 * @param address  40-bit memory address to read from
 * @param max_nn   Maximum number of 64-bit words to read (1-16)
 * @param ind      Indirect word
 * @param sync     Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr  Signal to raise upon completion
 *
 */

__intrinsic
void cmd_mem_read64_swap_ind_ptr40(
    __xread void *data,
    volatile void __addr40 __mem *address,
    uint32_t max_nn,
    generic_ind_t ind,
    sync_t sync,
    SIGNAL *sig_ptr
    );

/** Read data from memory with byte swapping into transfer registers (little-endian).
 *
 * @param data     Xfer register(s) to store data read from memory
 * @param address 32-bit memory address to read from
 * @param count    Number of 64-bit words to read (1-16)
 * @param sync     Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr  Signal to raise upon completion
 */
__intrinsic
void cmd_mem_read64_swap_le_ptr32(
    __xread void *data,
    volatile void __addr32 __mem *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
    );

/** Read data from memory with byte swapping into transfer registers (little-endian).
 *
 * @param data     Xfer register(s) to store data read from memory
 * @param address  40-bit memory address to read from
 * @param count    Number of 64-bit words to read (1-16)
 * @param sync     Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr  Signal to raise upon completion
 *
 */

__intrinsic
void cmd_mem_read64_swap_le_ptr40(
    __xread void *data,
    volatile void __addr40 __mem *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
    );


/** Read data from memory with byte swapping into transfer registers in indirect mode
 * (little-endian).
 *
 * @param data     Xfer register(s) to store data read from memory
 * @param address  Memory address to read from
 * @param max_nn   Maximum number of 64-bit words to read (1-16)
 * @param ind      Indirect word
 * @param sync     Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr  Signal to raise upon completion
 */
__intrinsic
void cmd_mem_read64_swap_le_ind_ptr32(
    __xread void *data,
    volatile void __addr32 __mem *address,
    uint32_t max_nn,
    generic_ind_t ind,
    sync_t sync,
    SIGNAL *sig_ptr
    );


/** Read data from memory with byte swapping into transfer registers in indirect mode
 * (little-endian).
 *
 * @param data     Xfer register(s) to store data read from memory
 * @param address  40-bit memory address to read from
 * @param max_nn   Maximum number of 64-bit words to read (1-16)
 * @param ind      Indirect word
 * @param sync     Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr  Signal to raise upon completion
 *
 */

__intrinsic
void cmd_mem_read64_swap_le_ind_ptr40(
    __xread void *data,
    volatile void __addr40 __mem *address,
    uint32_t max_nn,
    generic_ind_t ind,
    sync_t sync,
    SIGNAL *sig_ptr
    );


/** Write data to memory from transfer registers (little-endian).
 *
 * @param data     Xfer register(s) containing data to write to memory
 * @param address  32-bit memory address at which operation should be performed
 * @param count    Number of 64-bit words to write (1-16)
 * @param sync     Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr  Signal to raise upon completion
 */
__intrinsic
void cmd_mem_write64_le_ptr32(
    __xwrite void *data,
    volatile void __addr32 __mem *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
    );


/** Write data to memory from transfer registers (little-endian).
 *
 * @param data     Xfer register(s) containing data to write to memory
 * @param address  40-bit memory address at which operation should be performed
 * @param count    Number of 64-bit words to write (1-16)
 * @param sync     Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr  Signal to raise upon completion
 *
 */

__intrinsic
void cmd_mem_write64_le_ptr40(
    __xwrite void *data,
    volatile void __addr40 __mem *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
    );



/** Write data to memory from transfer registers in indirect mode
 * (little-endian).
 *
 * @param data     Xfer register(s) containing data to write to memory
 * @param address  Memory address at which operation should be performed
 * @param max_nn   Maximum number of 64-bit words to write (1-16)
 * @param ind      Indirect word
 * @param sync     Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr  Signal to raise upon completion
 */
__intrinsic
void cmd_mem_write64_le_ind_ptr32(
    __xwrite void *data,
    volatile void __addr32 __mem *address,
    uint32_t max_nn,
    generic_ind_t ind,
    sync_t sync,
    SIGNAL *sig_ptr
    );


/** Write data to memory from transfer registers in indirect mode
 * (little-endian).
 *
 * @param data     Xfer register(s) containing data to write to memory
 * @param address  40-bit memory address at which operation should be performed
 * @param max_nn   Maximum number of 64-bit words to write (1-16)
 * @param ind      Indirect word
 * @param sync     Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr  Signal to raise upon completion
 *
 */

__intrinsic
void cmd_mem_write64_le_ind_ptr40(
    __xwrite void *data,
    volatile void __addr40 __mem *address,
    uint32_t max_nn,
    generic_ind_t ind,
    sync_t sync,
    SIGNAL *sig_ptr
    );


/** Write data to memory from transfer registers in indirect mode.
 *
 * @param data              Xfer register(s) containing data to write to memory
 * @param address           32-bit memory address at which operation should be performed
 * @param max_nn            Maximum number of 64-bit words to write (1-16)
 * @param ind               Indirect word
 * @param sync              Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr           Signal pair to raise upon completion
 */
__intrinsic
void cmd_mem_write64_ind_ptr32(
    __xwrite void *data,
    volatile void __addr32 __mem *address,
    uint32_t max_nn,
    generic_ind_t ind,
    sync_t sync,
    SIGNAL *sig_ptr
    );

/** Write data to memory from transfer registers in indirect mode.
 *
 * @param data              Xfer register(s) containing data to write to memory
 * @param address           40-bit memory address at which operation should be performed
 * @param max_nn            Maximum number of 64-bit words to write (1-16)
 * @param ind               Indirect word
 * @param sync              Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr           Signal pair to raise upon completion
 */

__intrinsic
void cmd_mem_write64_ind_ptr40(
    __xwrite void *data,
    volatile void __addr40 __mem *address,
    uint32_t max_nn,
    generic_ind_t ind,
    sync_t sync,
    SIGNAL *sig_ptr
    );


/** Write data to memory from transfer register in big endian (LWBE) format
 * with byte swapping.
 *
 * @param data     Xfer register(s) containing data to write to memory
 * @param address  32-bit memory address at which operation should be performed
 * @param count    Number of 64-bit words to write (1-16)
 * @param sync     Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr  Signal to raise upon completion
 *
 */
__intrinsic
void cmd_mem_write64_swap_ptr32(
    __xwrite void *data,
    volatile void __addr32 __mem *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
    );

/** Write data to memory from transfer register in big endian (LWBE) format
 * with byte swapping.
 *
 * @param data     Xfer register(s) containing data to write to memory
 * @param address  40-bit memory address at which operation should be performed
 * @param count    Number of 64-bit words to write (1-16)
 * @param sync     Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr  Signal to raise upon completion
 *
 */

__intrinsic
void cmd_mem_write64_swap_ptr40(
    __xwrite void *data,
    volatile void __addr40 __mem *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
    );


/**Write data to memory from transfer register in big endian (LWBE) format
 * with byte swapping in indirect mode.
 *
 * @param data     Xfer register(s) containing data to write to memory
 * @param address  Memory address at which operation should be performed
 * @param max_nn   Maximum number of 64-bit words to write (1-16)
 * @param ind      Indirect word
 * @param sync     Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr  Signal to raise upon completion
 *
 */
__intrinsic
void cmd_mem_write64_swap_ind_ptr32(
    __xwrite void *data,
    volatile void __addr32 __mem *address,
    uint32_t max_nn,
    generic_ind_t ind,
    sync_t sync,
    SIGNAL *sig_ptr
    );

/**Write data to memory from transfer register in big endian (LWBE) format
 * with byte swapping in indirect mode.
 *
 * @param data     Xfer register(s) containing data to write to memory
 * @param address  40-bit memory address at which operation should be performed
 * @param max_nn   Maximum number of 64-bit words to write (1-16)
 * @param ind      Indirect word
 * @param sync     Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr  Signal to raise upon completion
 *
 */

__intrinsic
void cmd_mem_write64_swap_ind_ptr40(
    __xwrite void *data,
    volatile void __addr40 __mem *address,
    uint32_t max_nn,
    generic_ind_t ind,
    sync_t sync,
    SIGNAL *sig_ptr
    );


/** Write data to memory from transfer register in little endian format
 * with byte swapping.
 *
 * @param data     Xfer register(s) containing data to write to memory
 * @param address  32-bit memory address at which operation should be performed
 * @param count    Number of 64-bit words to write (1-16)
 * @param sync     Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr  Signal to raise upon completion
 *
 */
__intrinsic
void cmd_mem_write64_swap_le_ptr32(
    __xwrite void *data,
    volatile void __addr32 __mem *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
    );

/** Write data to memory from transfer register in little endian format
 * with byte swapping.
 *
 * @param data     Xfer register(s) containing data to write to memory
 * @param address  40-bit memory address at which operation should be performed
 * @param count    Number of 64-bit words to write (1-16)
 * @param sync     Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr  Signal to raise upon completion
 *
 */

__intrinsic
void cmd_mem_write64_swap_le_ptr40(
    __xwrite void *data,
    volatile void __addr40 __mem *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
    );


/**Write data to memory from transfer register in little endian format
 * with byte swapping in indirect mode.
 *
 * @param data     Xfer register(s) containing data to write to memory
 * @param address  Memory address at which operation should be performed
 * @param max_nn   Maximum number of 64-bit words to write (1-16)
 * @param ind      Indirect word
 * @param sync     Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr  Signal to raise upon completion
 *
 */
__intrinsic
void cmd_mem_write64_swap_le_ind_ptr32(
    __xwrite void *data,
    volatile void __addr32 __mem *address,
    uint32_t max_nn,
    generic_ind_t ind,
    sync_t sync,
    SIGNAL *sig_ptr
    );

/**Write data to memory from transfer register in little endian format
 * with byte swapping in indirect mode.
 *
 * @param data     Xfer register(s) containing data to write to memory
 * @param address  40-bit memory address at which operation should be performed
 * @param max_nn   Maximum number of 64-bit words to write (1-16)
 * @param ind      Indirect word
 * @param sync     Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr  Signal to raise upon completion
 *
 */

__intrinsic
void cmd_mem_write64_swap_le_ind_ptr40(
    __xwrite void *data,
    volatile void __addr40 __mem *address,
    uint32_t max_nn,
    generic_ind_t ind,
    sync_t sync,
    SIGNAL *sig_ptr
    );

/** Write data to memory from transfer registers in bytes (big-endian).
 *
 * @param data     Xfer register(s) containing data to write to memory
 * @param address  32-bit memory address at which operation should be performed
 * @param count    Number of bytes to write (1-32)
 * @param sync     Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr  Signal to raise upon completion
 *
 */
__intrinsic
void cmd_mem_write8_ptr32(
    __xwrite void *data,
    volatile void __addr32 __mem *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
    );


/** Write data to memory from transfer registers in bytes (big-endian).
 *
 * @param data     Xfer register(s) containing data to write to memory
 * @param address  40-bit memory address at which operation should be performed
 * @param count    Number of bytes to write (1-32)
 * @param sync     Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr  Signal to raise upon completion
 *
 */

__intrinsic
void cmd_mem_write8_ptr40(
    __xwrite void *data,
    volatile void __addr40 __mem *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
    );


/** Write bytes of data to memory from transfer registers in indirect mode
 * (big-endian).
 *
 * @param data     Xfer register(s) containing data to write to memory
 * @param address  Memory address at which operation should be performed
 * @param max_nn   Maximum number of bytes to write (1-32)
 * @param ind      Indirect word
 * @param sync     Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr  Signal to raise upon completion
 */
__intrinsic
void cmd_mem_write8_ind_ptr32(
    __xwrite void *data,
    volatile void __addr32 __mem *address,
    uint32_t max_nn,
    generic_ind_t ind,
    sync_t sync,
    SIGNAL *sig_ptr
    );


/** Write bytes of data to memory from transfer registers in indirect mode
 * (big-endian).
 *
 * @param data     Xfer register(s) containing data to write to memory
 * @param address  40-bit memory address at which operation should be performed
 * @param max_nn   Maximum number of bytes to write (1-32)
 * @param ind      Indirect word
 * @param sync     Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr  Signal to raise upon completion
 *
 */

__intrinsic
void cmd_mem_write8_ind_ptr40(
    __xwrite void *data,
    volatile void __addr40 __mem *address,
    uint32_t max_nn,
    generic_ind_t ind,
    sync_t sync,
    SIGNAL *sig_ptr
    );


/** Write bytes of data to memory from transfer registers (little-endian).
 *
 * @param data     Xfer register(s) containing data to write to memory
 * @param address  32-bit memory address at which operation should be performed
 * @param count    Number of bytes to write (1-32))
 * @param sync     Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr  Signal to raise upon completion
 */
__intrinsic
void cmd_mem_write8_le_ptr32(
    __xwrite void *data,
    volatile void __addr32 __mem *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
    );


/** Write bytes of data to memory from transfer registers (little-endian).
 *
 * @param data     Xfer register(s) containing data to write to memory
 * @param address  40-bit memory address at which operation should be performed
 * @param count    Number of bytes to write (1-32)
 * @param sync     Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr  Signal to raise upon completion
 *
 */

__intrinsic
void cmd_mem_write8_le_ptr40(
    __xwrite void *data,
    volatile void __addr40 __mem *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
    );

/** Write bytes of data to memory from transfer registers in indirect mode
 * (little-endian).
 *
 * @param data     Xfer register(s) containing data to write to memory
 * @param address  Memory address at which operation should be performed
 * @param max_nn   Maximum number of bytes to write (1-32)
 * @param ind      Indirect word
 * @param sync     Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr  Signal to raise upon completion
 */
__intrinsic
void cmd_mem_write8_le_ind_ptr32(
    __xwrite void *data,
    volatile void __addr32 __mem *address,
    uint32_t max_nn,
    generic_ind_t ind,
    sync_t sync,
    SIGNAL *sig_ptr
    );

/** Write bytes of data to memory from transfer registers in indirect mode
 * (little-endian).
 *
 * @param data     Xfer register(s) containing data to write to memory
 * @param address  40-bit memory address at which operation should be performed
 * @param max_nn   Maximum number of bytes to write (1-32)
 * @param ind      Indirect word
 * @param sync     Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr  Signal to raise upon completion
 *
 */

__intrinsic
void cmd_mem_write8_le_ind_ptr40(
    __xwrite void *data,
    volatile void __addr40 __mem *address,
    uint32_t max_nn,
    generic_ind_t ind,
    sync_t sync,
    SIGNAL *sig_ptr
    );

/** Write bytes to memory from transfer registers in big endian (LWBE) format
 * with byte swapping.
 *
 * @param data     Xfer register(s) containing data to write to memory
 * @param address  32-bit memory address at which operation should be performed
 * @param count    Number of bytes to write (1-32)
 * @param sync     Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr  Signal to raise upon completion
 *
 */
__intrinsic
void cmd_mem_write8_swap_ptr32(
    __xwrite void *data,
    volatile void __addr32 __mem *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
    );

/** Write bytes to memory from transfer registers in big endian (LWBE) format
 * with byte swapping.
 *
 * @param data     Xfer register(s) containing data to write to memory
 * @param address  40-bit memory address at which operation should be performed
 * @param count    Number of bytes to write (1-32)
 * @param sync     Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr  Signal to raise upon completion
 *
 */

__intrinsic
void cmd_mem_write8_swap_ptr40(
    __xwrite void *data,
    volatile void __addr40 __mem *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
    );

/** Write bytes of data to memory from transfer registers in big endian (LWBE) format
 * with byte swapping in indirect mode.
 *
 * @param data     Xfer register(s) containing data to write to memory
 * @param address  Memory address at which operation should be performed
 * @param max_nn   Maximum number of bytes to write (1-32)
 * @param ind      Indirect word
 * @param sync     Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr  Signal to raise upon completion
 *
 */
__intrinsic
void cmd_mem_write8_swap_ind_ptr32(
    __xwrite void *data,
    volatile void __addr32 __mem *address,
    uint32_t max_nn,
    generic_ind_t ind,
    sync_t sync,
    SIGNAL *sig_ptr
    );

/** Write bytes of data to memory from transfer registers in big endian (LWBE) format
 * with byte swapping in indirect mode.
 *
 * @param data     Xfer register(s) containing data to write to memory
 * @param address  40-bit memory address at which operation should be performed
 * @param max_nn   Maximum number of bytes to write (1-32)
 * @param ind      Indirect word
 * @param sync     Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr  Signal to raise upon completion
 *
 */

__intrinsic
void cmd_mem_write8_swap_ind_ptr40(
    __xwrite void *data,
    volatile void __addr40 __mem *address,
    uint32_t max_nn,
    generic_ind_t ind,
    sync_t sync,
    SIGNAL *sig_ptr
    );

/** Write bytes to memory from transfer registers in little endian format
 * with byte swapping.
 *
 * @param data     Xfer register(s) containing data to write to memory
 * @param address  32-bit memory address at which operation should be performed
 * @param count    Number of bytes to write (1-32)
 * @param sync     Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr  Signal to raise upon completion
 *
 */
__intrinsic
void cmd_mem_write8_swap_le_ptr32(
    __xwrite void *data,
    volatile void __addr32 __mem *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
    );

/** Write bytes to memory from transfer registers in little endian format
 * with byte swapping.
 *
 * @param data     Xfer register(s) containing data to write to memory
 * @param address  40-bit memory address at which operation should be performed
 * @param count    Number of bytes to write (1-32)
 * @param sync     Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr  Signal to raise upon completion
 *
 */

__intrinsic
void cmd_mem_write8_swap_le_ptr40(
    __xwrite void *data,
    volatile void __addr40 __mem *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
    );

/** Write bytes of data to memory from transfer registers in little endian format
 * with byte swapping in indirect mode.
 *
 * @param data     Xfer register(s) containing data to write to memory
 * @param address  Memory address at which operation should be performed
 * @param max_nn   Maximum number of bytes to write (1-32)
 * @param ind      Indirect word
 * @param sync     Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr  Signal to raise upon completion
 *
 */
__intrinsic
void cmd_mem_write8_swap_le_ind_ptr32(
    __xwrite void *data,
    volatile void __addr32 __mem *address,
    uint32_t max_nn,
    generic_ind_t ind,
    sync_t sync,
    SIGNAL *sig_ptr
    );

/** Write bytes of data to memory from transfer registers in little endian format
 * with byte swapping in indirect mode.
 *
 * @param data     Xfer register(s) containing data to write to memory
 * @param address  40-bit memory address at which operation should be performed
 * @param max_nn   Maximum number of bytes to write (1-32)
 * @param ind      Indirect word
 * @param sync     Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr  Signal to raise upon completion
 *
 */

__intrinsic
void cmd_mem_write8_swap_le_ind_ptr40(
    __xwrite void *data,
    volatile void __addr40 __mem *address,
    uint32_t max_nn,
    generic_ind_t ind,
    sync_t sync,
    SIGNAL *sig_ptr
    );

/** Read bytes of data from memory into transfer registers.
 *
 * @param data     Xfer register(s) to store data read from memory
 * @param address  32-bit memory address to read from
 * @param count    Number of bytes to read (1-32)
 * @param sync     Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr  Signal to raise upon completion
 */
__intrinsic
void cmd_mem_read8_ptr32(
    __xread void *data,
    volatile void __addr32 __mem *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
    );

/** Read bytes of data from memory into transfer registers.
 *
 * @param data     Xfer register(s) to store data read from memory
 * @param address  40-bit memory address to read from
 * @param count    Number of bytes to read (1-32)
 * @param sync     Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr  Signal to raise upon completion
 *
 */

__intrinsic
void cmd_mem_read8_ptr40(
    __xread void *data,
    volatile void __addr40 __mem *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
    );

/** Read bytes of data from memory into transfer registers in indirect mode.
 *
 * @param data     Xfer register(s) to store data read from memory
 * @param address  Memory address to read from
 * @param max_nn   Maximum number of bytes to read (1-32)
 * @param ind      Indirect word
 * @param sync     Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr  Signal to raise upon completion
 *
 */
__intrinsic
void cmd_mem_read8_ind_ptr32(
    __xread void *data,
    volatile void __addr32 __mem *address,
    uint32_t max_nn,
    generic_ind_t ind,
    sync_t sync,
    SIGNAL *sig_ptr
    );

/** Read bytes of data from memory into transfer registers in indirect mode.
 *
 * @param data     Xfer register(s) to store data read from memory
 * @param address  40-bit memory address to read from
 * @param max_nn   Maximum number of bytes to read (1-32)
 * @param ind      Indirect word
 * @param sync     Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr  Signal to raise upon completion
 *
 */

__intrinsic
void cmd_mem_read8_ind_ptr40(
    __xread void *data,
    volatile void __addr40 __mem *address,
    uint32_t max_nn,
    generic_ind_t ind,
    sync_t sync,
    SIGNAL *sig_ptr
    );


/**
 * @}
 * @name Atomic functions for MEM
 * @{
 */


/** Read data from memory into transfer registers (atomic).
 *
 * @param data     Xfer register(s) to store data read from memory
 * @param address 32-bit memory address to read from
 * @param count    Number of 32-bit words to read (1-8)
 * @param sync     Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr  Signal to raise upon completion
 */
__intrinsic
void cmd_mem_read_atomic_ptr32(
    __xread void *data,
    volatile void __addr32 __mem *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
    );

/** Read data from memory into transfer registers (atomic).
 *
 * @param data     Xfer register(s) to store data read from memory
 * @param address  40-bit memory address to read from
 * @param count    Number of 32-bit words to read (1-8)
 * @param sync     Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr  Signal to raise upon completion
 *
 */

__intrinsic
void cmd_mem_read_atomic_ptr40(
    __xread void *data,
    volatile void __addr40 __mem *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
    );


/** Read data from memory into transfer registers in indirect mode (atomic).
 *
 * @param data     Xfer register(s) to store data read from memory
 * @param address  Memory address to read from
 * @param max_nn   Maximum number of 32-bit words to read (1-8)
 * @param ind      Indirect word
 * @param sync     Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr  Signal to raise upon completion
 */
__intrinsic
void cmd_mem_read_atomic_ind_ptr32(
    __xread void *data,
    volatile void __addr32 __mem *address,
    uint32_t max_nn,
    generic_ind_t ind,
    sync_t sync,
    SIGNAL *sig_ptr
    );

/** Read data from memory into transfer registers in indirect mode (atomic).
 *
 * @param data     Xfer register(s) to store data read from memory
 * @param address  40-bit memory address to read from
 * @param max_nn   Maximum number of 32-bit words to read (1-8)
 * @param ind      Indirect word
 * @param sync     Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr  Signal to raise upon completion
 *
 */

__intrinsic
void cmd_mem_read_atomic_ind_ptr40(
    __xread void *data,
    volatile void __addr40 __mem *address,
    uint32_t max_nn,
    generic_ind_t ind,
    sync_t sync,
    SIGNAL *sig_ptr
    );


/** Memory compare_write.
 *
 * @param data     Xfer register(s) containing data to write to memory
 * @param address  32-bit memory address at which operation should be performed
 * @param count    Number of 32-bit words to write (1-8)
 * @param sync     Type of synchronization to use (must be sig_done)
 * @param sig_ptr  Signal to raise upon completion
 *
 * Compare bottom 16 bits of the first write transfer register with the bottom 16 bits of memory location.
 * If they are equal, set the bottom 16 bits of memory to top 16 bits of the transfer register
 * and write the remaining write registers to the subsequent memory locations.
 * If the compare fails, increment the top 16 bits of memory location saturating at 0xffff.
 *
 * @note This function requires Silicon revision B0 or higher.
 *       See Errata 'MU Atomic Engine illegally sends Pull Signal for the compare_write_or_incr command and
 *       test_compare_and_write_or_incr command.'
 */
__intrinsic
void cmd_mem_compare_write_ptr32(
    __xwrite void *data,
    volatile void __addr32 __mem *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
    );


/** Memory compare_write.
 *
 * @param data     Xfer register(s) containing data to write to memory
 * @param address  40-bit memory address at which operation should be performed
 * @param count    Number of 32-bit words to write (1-8)
 * @param sync     Type of synchronization to use (must be sig_done)
 * @param sig_ptr  Signal to raise upon completion
 *
 * Compare bottom 16 bits of the first write transfer register with the bottom 16 bits of memory location.
 * If they are equal, set the bottom 16 bits of memory to top 16 bits of the transfer register
 * and write the remaining write registers to the subsequent memory locations.
 * If the compare fails, increment the top 16 bits of memory location saturating at 0xffff.
 *
 * @note This function requires Silicon revision B0 or higher.
 *       See Errata 'MU Atomic Engine illegally sends Pull Signal for the compare_write_or_incr command and
 *       test_compare_and_write_or_incr command.'
 */

__intrinsic
void cmd_mem_compare_write_ptr40(
    __xwrite void *data,
    volatile void __addr40 __mem *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
    );


/** Memory compare_write in indirect mode.
 *
 * @param data     Xfer register(s) containing data to write to memory
 * @param address  Memory address at which operation should be performed
 * @param max_nn   Max number of 32-bit words to write (1-8)
 * @param ind      Indirect word
 * @param sync     Type of synchronization to use (must be sig_done)
 * @param sig_pair_ptr  Signal pair to raise upon completion
 *
 * Compare bottom 16 bits of the first write transfer register with the bottom 16 bits of memory location.
 * If they are equal, set the bottom 16 bits of memory to top 16 bits of the transfer register
 * and write the remaining write registers to the subsequent memory locations.
 * If the compare fails, increment the top 16 bits of memory location saturating at 0xffff.
 *
 * @note This function requires Silicon revision B0 or higher.
 *       See Errata 'MU Atomic Engine illegally sends Pull Signal for the compare_write_or_incr command and
 *       test_compare_and_write_or_incr command.'
 */
__intrinsic
void cmd_mem_compare_write_ind_ptr32(
    __xwrite void *data,
    volatile void __addr32 __mem *address,
    uint32_t max_nn,
    generic_ind_t ind,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
    );


/** Memory compare_write in indirect mode.
 *
 * @param data     Xfer register(s) containing data to write to memory
 * @param address  40-bit memory address at which operation should be performed
 * @param max_nn   Max number of 32-bit words to write (1-8)
 * @param ind      Indirect word
 * @param sync     Type of synchronization to use (must be sig_done)
 * @param sig_pair_ptr  Signal pair to raise upon completion
 *
 * Compare bottom 16 bits of the first write transfer register with the bottom 16 bits of memory location.
 * If they are equal, set the bottom 16 bits of memory to top 16 bits of the transfer register
 * and write the remaining write registers to the subsequent memory locations.
 * If the compare fails, increment the top 16 bits of memory location saturating at 0xffff.
 *
 * @note This function requires Silicon revision B0 or higher.
 *       See Errata 'MU Atomic Engine illegally sends Pull Signal for the compare_write_or_incr command and
 *       test_compare_and_write_or_incr command.'
 */

__intrinsic
void cmd_mem_compare_write_ind_ptr40(
    __xwrite void *data,
    volatile void __addr32 __mem *address,
    uint32_t max_nn,
    generic_ind_t ind,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
    );


/** Memory test_and_compare_write.
 *
 * @param val      Returned pre-modified value
 * @param data     Xfer register(s) containing data to write to memory
 * @param address  32-bit memory address at which operation should be performed
 * @param count    Number of 32-bit words to write (1-8)
 * @param sync     Type of synchronization to use (must be sig_done)
 * @param sig_pair_ptr  Signal pair to raise upon completion
 *
 * Same as the cmp_write instruction but it also returns a pre-modified
 * 32-bit word of the first memory location.
 * @see cmd_mem_compare_write_ptr40()
 *
 * @note This function requires Silicon revision B0 or higher.
 *       See Errata 'MU Atomic Engine illegally sends Pull Signal for the compare_write_or_incr command and
 *       test_compare_and_write_or_incr command.'
 */
__intrinsic
void cmd_mem_test_and_compare_write_ptr32(
    __xread uint32_t *val,
    __xwrite void *data,
    volatile void __addr32 __mem *address,
    uint32_t count,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
    );

/** Memory test_and_compare_write.
 *
 * @param val      Returned pre-modified value
 * @param data     Xfer register(s) containing data to write to memory
 * @param address  40-bit memory address at which operation should be performed
 * @param count    Number of 32-bit words to write (1-8)
 * @param sync     Type of synchronization to use (must be sig_done)
 * @param sig_pair_ptr  Signal pair to raise upon completion
 *
 * Same as the cmp_write instruction but it also returns a pre-modified
 * 32-bit word of the first memory location.
 * @see cmd_mem_compare_write_ptr40()
 *
 * @note This function requires Silicon revision B0 or higher.
 *       See Errata 'MU Atomic Engine illegally sends Pull Signal for the compare_write_or_incr command and
 *       test_compare_and_write_or_incr command.'
 */

__intrinsic
void cmd_mem_test_and_compare_write_ptr40(
    __xread uint32_t *val,
    __xwrite void *data,
    volatile void __addr40 __mem *address,
    uint32_t count,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
    );


/** Memory test_and_compare_write in indirect mode.
 *
 * @param val      Returned pre-modified value
 * @param data     Xfer register(s) containing data to write to memory
 * @param address  Memory address at which operation should be performed
 * @param max_nn   Max number of 32-bit words to write (1-8)
 * @param ind      Indirect word
 * @param sync     Type of synchronization to use (must be sig_done)
 * @param sig_pair_ptr  Signal pair to raise upon completion
 *
 * Same as the cmp_write_ind instruction but it also returns a pre-modified
 * 32-bit word of the first memory location.
 * @see cmd_mem_compare_write_ind_ptr40()
 *
 * @note This function requires Silicon revision B0 or higher.
 *       See Errata 'MU Atomic Engine illegally sends Pull Signal for the compare_write_or_incr command and
 *       test_compare_and_write_or_incr command.'
 */
__intrinsic
void cmd_mem_test_and_compare_write_ind_ptr32(
    __xread uint32_t *val,
    __xwrite void *data,
    volatile void __addr32 __mem *address,
    uint32_t max_nn,
    generic_ind_t ind,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
    );


/** Memory test_and_compare_write in indirect mode.
 *
 * @param val      Returned pre-modified value
 * @param data     Xfer register(s) containing data to write to memory
 * @param address  40-bit memory address at which operation should be performed
 * @param max_nn   Max number of 32-bit words to write (1-8)
 * @param ind      Indirect word
 * @param sync     Type of synchronization to use (must be sig_done)
 * @param sig_pair_ptr  Signal pair to raise upon completion
 *
 * Same as the cmp_write_ind instruction but it also returns a pre-modified
 * 32-bit word of the first memory location.
 * @see cmd_mem_compare_write_ind_ptr40()
 *
 * @note This function requires Silicon revision B0 or higher.
 *       See Errata 'MU Atomic Engine illegally sends Pull Signal for the compare_write_or_incr command and
 *       test_compare_and_write_or_incr command.'
 */

__intrinsic
void cmd_mem_test_and_compare_write_ind_ptr40(
    __xread uint32_t *val,
    __xwrite void *data,
    volatile void __addr40 __mem *address,
    uint32_t max_nn,
    generic_ind_t ind,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
    );

/** Write data to memory from transfer registers (atomic).
 *
 * @param data     Xfer register(s) containing data to write to memory
 * @param address  32-bit memory address at which operation should be performed
 * @param count    Number of 32-bit words to write (1-8)
 * @param sync     Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr  Signal to raise upon completion
 */
__intrinsic
void cmd_mem_write_atomic_ptr32(
    __xwrite void *data,
    volatile void __addr32 __mem *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
    );

/** Write data to memory from transfer registers (atomic).
 *
 * @param data     Xfer register(s) containing data to write to memory
 * @param address  40-bit memory address at which operation should be performed
 * @param count    Number of 32-bit words to write (1-8)
 * @param sync     Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr  Signal to raise upon completion
 *
 */

__intrinsic
void cmd_mem_write_atomic_ptr40(
    __xwrite void *data,
    volatile void __addr40 __mem *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
    );

/** Write data to memory from transfer registers in indirect mode (atomic).
 *
 * @param data     Xfer register(s) containing data to write to memory
 * @param address  40-bit memory address at which operation should be performed
 * @param max_nn   Maximum number of 32-bit words to write (1-8)
 * @param ind      Indirect word
 * @param sync     Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr  Signal to raise upon completion
 *
 */

__intrinsic
void cmd_mem_write_atomic_ind_ptr40(
    __xwrite void *data,
    volatile void __addr40 __mem *address,
    uint32_t max_nn,
    generic_ind_t ind,
    sync_t sync,
    SIGNAL *sig_ptr
    );


/** Write data to memory from transfer registers in indirect mode (atomic).
 *
 * @param data     Xfer register(s) containing data to write to memory
 * @param address  32-bit memory address at which operation should be performed
 * @param max_nn   Maximum number of 32-bit words to write (1-8)
 * @param ind      Indirect word
 * @param sync     Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr  Signal to raise upon completion
 */
__intrinsic
void cmd_mem_write_atomic_ind_ptr32(
    __xwrite void *data,
    volatile void __addr32 __mem *address,
    uint32_t max_nn,
    generic_ind_t ind,
    sync_t sync,
    SIGNAL *sig_ptr
    );

/** Memory swap.
 *
 * @param val      Returned pre-modified value
 * @param data     Xfer register(s) containing data to write to memory
 * @param address  32-bit memory address at which operation should be performed
 * @param count    Number of 32-bit words to write (1-8)
 * @param sync     Type of synchronization to use (must be sig_done)
 * @param sig_pair_ptr  Signal pair to raise upon completion
 *
 * Same as the atomic_write instruction but it also returns premodified
 * memory values in the read transfer registers.
 * @see cmd_mem_write_atomic_ptr40()
 */
__intrinsic
void cmd_mem_swap_ptr32(
    __xread uint32_t *val,
    __xwrite void *data,
    volatile void __addr32 __mem *address,
    uint32_t count,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
    );

/** Memory swap.
 *
 * @param val      Returned pre-modified value
 * @param data     Xfer register(s) containing data to write to memory
 * @param address  40-bit memory address at which operation should be performed
 * @param count    Number of 32-bit words to write (1-8)
 * @param sync     Type of synchronization to use (must be sig_done)
 * @param sig_pair_ptr  Signal pair to raise upon completion
 *
 * Same as the atomic_write instruction but it also returns premodified
 * memory values in the read transfer registers.
 * @see cmd_mem_write_atomic_ptr40()
 *
 */

__intrinsic
void cmd_mem_swap_ptr40(
    __xread uint32_t *val,
    __xwrite void *data,
    volatile void __addr40 __mem *address,
    uint32_t count,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
    );

/** Memory swap in indirect mode.
 *
 * @param val      Returned pre-modified value
 * @param data     Xfer register(s) containing data to write to memory
 * @param address  Memory address at which should be performed
 * @param max_nn   Maximum number of 32-bit words to write (1-8)
 * @param ind      Indirect word
 * @param sync     Type of synchronization to use (must be sig_done)
 * @param sig_pair_ptr  Signal pair to raise upon completion
 *
 * Same as the write_atomic_ind instruction but it also returns premodified
 * memory values in the read transfer registers.
 * @see cmd_mem_write_atomic_ind_ptr40()
 */
__intrinsic
void cmd_mem_swap_ind_ptr32(
    __xread uint32_t *val,
    __xwrite void *data,
    volatile void __addr32 __mem *address,
    uint32_t max_nn,
    generic_ind_t ind,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
    );

/** Memory swap in indirect mode.
 *
 * @param val      Returned pre-modified value
 * @param data     Xfer register(s) containing data to write to memory
 * @param address  40-bit memory address at which should be performed
 * @param max_nn   Maximum number of 32-bit words to write (1-8)
 * @param ind      Indirect word
 * @param sync     Type of synchronization to use (must be sig_done)
 * @param sig_pair_ptr  Signal pair to raise upon completion
 *
 * Same as the write_atomic_ind instruction but it also returns premodified
 * memory values in the read transfer registers.
 * @see cmd_mem_write_atomic_ind_ptr40()
 *
 */

__intrinsic
void cmd_mem_swap_ind_ptr40(
    __xread uint32_t *val,
    __xwrite void *data,
    volatile void __addr40 __mem *address,
    uint32_t max_nn,
    generic_ind_t ind,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
    );

/** Write atomic immediate.
 *
 * @param data     Immediate data to swap
 * @param address  32-bit memory address at which operation should be performed
 * @param count    Number of 32-bit words to write (1-8)
 *
 * @note Valid range for data is between 0 and 0x7f for 32xx indirect format and
 *       between 0 and 0xffff for 6xxx indirect format.
 *
 * Same as the atomic_write instruction but the value written to memory
 * is specified as the immediate data.
 * @see cmd_mem_write_atomic_ptr40()
 *
 */
__intrinsic
void cmd_mem_write_atomic_imm_ptr32(
    uint32_t data,
    volatile void __addr32 __mem *address,
    uint32_t count
    );

/** Write atomic immediate.
 *
 * @param data     Immediate data to swap
 * @param address  40-bit memory address at which operation should be performed
 * @param count    Number of 32-bit words to write (1-8)
 *
 * @note Valid range for data is between 0 and 0x7f for 32xx indirect format and
 *       between 0 and 0xffff for 6xxx indirect format.
 *
 * Same as the atomic_write instruction but the value written to memory
 * is specified as the immediate data.
 * @see cmd_mem_write_atomic_ptr40()
 *
 */

__intrinsic
void cmd_mem_write_atomic_imm_ptr40(
    uint32_t data,
    volatile void __addr40 __mem *address,
    uint32_t count
    );


/** Swap immediate.
 *
 * @param data     Immediate data to swap
 * @param rd_back  Xfer register to read back
 * @param address  32-bit memory address at which operation should be performed
 * @param count    Number of 32-bit words to write (1-8)
 * @param sync     Type of synchronization to use (must be sig_done)
 * @param sig_ptr  Signal to raise upon completion
 *
 *
 * Same as the swap instruction but it also returns the premodified
 * memory values in the read transfer registers.
 * @see cmd_mem_swap_ptr32()
 *
 */
__intrinsic
void cmd_mem_swap_imm_ptr32(
    uint32_t data,
    __xread void *rd_back,
    volatile void __addr32 __mem *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
    );

/** Swap immediate.
 *
 * @param data     Immediate data to swap (0 - 0x7f)
 * @param rd_back  Xfer register to read back
 * @param address  40-bit memory address at which operation should be performed
 * @param count    Number of 32-bit words to write (1-8)
 * @param sync     Type of synchronization to use (must be sig_done)
 * @param sig_ptr  Signal to raise upon completion
 *
 *
 * Same as the swap instruction but it also returns the premodified
 * memory values in the read transfer registers.
 * @see cmd_mem_swap_ptr32()
 *
 */

__intrinsic
void cmd_mem_swap_imm_ptr40(
    uint32_t data,
    __xread void *rd_back,
    volatile void __addr40 __mem *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
    );


/** Set bits in memory from bits set in transfer registers.
 *
 * @param data     Data to set in Memory
 * @param address  Memory address to set to given data
 * @param count    Number of 32-bit words to set (1-8)
 * @param sync     Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr  Signal to raise upon completion
 *
 * Set bits specified in write transfers registers to a number of 32-bit memory words.
 */
__intrinsic
void cmd_mem_set_ptr32(
    __xwrite void *data,
    volatile void __addr32 __mem *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
    );

/** Set bits in memory from bits set in transfer registers.
 *
 * @param data     Data to set in Memory
 * @param address  40-bit memory address to set to given data
 * @param count    Number of 32-bit words to set (1-8)
 * @param sync     Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr  Signal to raise upon completion
 *
 * Set bits specified in write transfers registers to a number of 32-bit memory words.
 *
 *
 */

__intrinsic
void cmd_mem_set_ptr40(
    __xwrite void *data,
    volatile void __addr40 __mem *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
    );

/** Set bits in memory as specified by the bits set in Xfer register(s) using
 * indirect format.
 *
 * @param data     Xfer register(s) specifying the bits to be set
 * @param address  Memory address to start setting bits in
 * @param max_nn   Maximum number of 32-bit words to set (1-8)
 * @param ind      Indirect word
 * @param sync     Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr  Signal to raise upon completion
 *
 * The byte mask in the indirect_ref word can be used to mask the operation but
 * is usually not used as clearing bits in the transfer registers has a
 * similar outcome.
 *
 * The operation performed on each 32-bit memory location is:
 *
 * @code
 * memory = memory | data
 * @endcode
 */
__intrinsic
void cmd_mem_set_ind_ptr32(
    __xwrite void *data,
    volatile void __addr32 __mem *address,
    uint32_t max_nn,
    generic_ind_t ind,
    sync_t sync,
    SIGNAL *sig_ptr
    );

/** Set bits in memory as specified by the bits set in Xfer register(s) using
 * indirect format.
 *
 * @param data     Xfer register(s) specifying the bits to be set
 * @param address  40-bit memory address to start setting bits in
 * @param max_nn   Maximum number of 32-bit words to set (1-8)
 * @param ind      Indirect word
 * @param sync     Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr  Signal to raise upon completion
 *
 * The byte mask in the indirect_ref word can be used to mask the operation but
 * is usually not used as clearing bits in the transfer registers has a
 * similar outcome.
 *
 * The operation performed on each 32-bit memory location is:
 *
 * @code
 * memory = memory | data
 * @endcode
 *
 */

__intrinsic
void cmd_mem_set_ind_ptr40(
    __xwrite void *data,
    volatile void __addr40 __mem *address,
    uint32_t max_nn,
    generic_ind_t ind,
    sync_t sync,
    SIGNAL *sig_ptr
    );



/** Set bits in memory from bits set in Xfer register(s) and return
 * pre-modified values in read Xfer register(s).
 *
 * @param val           Xfer register(s) returning pre-modified Memory values
 * @param data          Xfer register(s) specifying the bits to be set
 * @param address       Memory address to start setting bits in
 * @param count         Number of 32-bit words to set (1-8)
 * @param sync          Type of synchronization to use (must be sig_done)
 * @param sig_pair_ptr  Signal pair to raise upon completion
 *
 * The operation performed on each 32-bit memory location is:
 *
 * @code
 * val = memory
 * memory = memory | data
 * @endcode
 */
__intrinsic
void cmd_mem_test_and_set_ptr32(
    __xread uint32_t *val,
    __xwrite void *data,
    volatile void __addr32 __mem *address,
    uint32_t count,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
    );

/** Set bits in memory from bits set in Xfer register(s) and return
 * pre-modified values in read Xfer register(s).
 *
 * @param val           Xfer register(s) returning pre-modified Memory values
 * @param data          Xfer register(s) specifying the bits to be set
 * @param address       40-bit memory address to start setting bits in
 * @param count         Number of 32-bit words to set (1-8)
 * @param sync          Type of synchronization to use (must be sig_done)
 * @param sig_pair_ptr  Signal pair to raise upon completion
 *
 * The operation performed on each 32-bit memory location is:
 *
 * @code
 * val = memory
 * memory = memory | data
 * @endcode
 *
 */

__intrinsic
void cmd_mem_test_and_set_ptr40(
    __xread uint32_t *val,
    __xwrite void *data,
    volatile void __addr40 __mem *address,
    uint32_t count,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
    );



/** Set bits in memory from bits set in Xfer register(s) and return
 * pre-modified values in read transfer register(s) using indirect format.
 *
 * @param val           Xfer register(s) returning pre-modified Memory values
 * @param data          Xfer register(s) containing the bits to be set
 * @param address       Memory address to start setting bits
 * @param max_nn        Number of 32-bit words to set (1-8)
 * @param ind           Indirect word
 * @param sync          Type of synchronization to use (must be sig_done)
 * @param sig_pair_ptr  Signal pair to raise upon completion
 *
 * The operation performed on each 32-bit memory location is:
 *
 * @code
 * val = memory
 * memory = memory | data
 * @endcode
 */
__intrinsic
void cmd_mem_test_and_set_ind_ptr32(
    __xread uint32_t *val,
    __xwrite void *data,
    volatile void __addr32 __mem *address,
    uint32_t max_nn,
    generic_ind_t ind,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
    );

/** Set bits in memory from bits set in Xfer register(s) and return
 * pre-modified values in read transfer register(s) using indirect format.
 *
 * @param val           Xfer register(s) returning pre-modified Memory values
 * @param data          Xfer register(s) containing the bits to be set
 * @param address       40-bit memory address to start setting bits
 * @param max_nn        Number of 32-bit words to set (1-8)
 * @param ind           Indirect word
 * @param sync          Type of synchronization to use (must be sig_done)
 * @param sig_pair_ptr  Signal pair to raise upon completion
 *
 * The operation performed on each 32-bit memory location is:
 *
 * @code
 * val = memory
 * memory = memory | data
 * @endcode
 *
 */

__intrinsic
void cmd_mem_test_and_set_ind_ptr40(
    __xread uint32_t *val,
    __xwrite void *data,
    volatile void __addr40 __mem *address,
    uint32_t max_nn,
    generic_ind_t ind,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
    );


/** Set bits in memory from immediate 32-bit word.
 *
 * @param data          Immediate data to set in Memory
 * @param address       32-bit memory address at which operation should be performed
 * @param count         Number of 32-bit words to set (1-8)
 *
 * @note Valid range for data is between 0 and 0x7f for 32xx indirect format and
 *       between 0 and 0xffff for 6xxx indirect format.
 *
 * The operation performed on each 32-bit memory location is:
 *
 * @code
 * memory = memory | data
 * @endcode
 *
 */
__intrinsic
void cmd_mem_set_imm_ptr32(
    uint32_t data,
    volatile void __addr32 __mem *address,
    uint32_t count
    );


/** Set bits in memory from immediate 32-bit word.
 *
 * @param data          Immediate data to set in Memory
 * @param address       40-bit memory address at which operation should be performed
 * @param count         Number of 32-bit words to set (1-8)
 *
 * @note Valid range for data is between 0 and 0x7f for 32xx indirect format and
 *       between 0 and 0xffff for 6xxx indirect format.
 *
 * The operation performed on each 32-bit memory location is:
 *
 * @code
 * memory = memory | data
 * @endcode
 *
 */

__intrinsic
void cmd_mem_set_imm_ptr40(
    uint32_t data,
    volatile void __addr40 __mem *address,
    uint32_t count
    );

/** Set bits in memory from bits in immediate 32-bit word and return
 * pre-modified values in read transfer register(s).
 *
 * @param data          Immediate data to set in Memory (0x0 - 0x7f)
 * @param rd_back       Xfer register(s) for read back
 * @param address       Memory address to start setting bits
 * @param count         Number of 32-bit words to set (1-8)
 * @param sync          Type of synchronization to use (must be sig_done)
 * @param sig_ptr       Signal to raise upon completion
 *
 * Same as the set_imm instruction but it also returns the premodified
 * memory values in the read transfer registers.
 * @see cmd_mem_set_imm_ptr32()

 * The operation performed on each 32-bit memory location is:
 *
 * @code
 * rd_back = memory
 * memory = memory | data
 * @endcode
 *
 */
__intrinsic
void cmd_mem_test_and_set_imm_ptr32(
    uint32_t data,
    __xread void *rd_back,
    volatile void __addr32 __mem *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
    );

/** Set bits in memory from bits in immediate 32-bit word and return
 * pre-modified values in read transfer register(s).
 *
 * @param data          Immediate data to set in Memory (0x0 - 0x7f)
 * @param rd_back       Xfer register(s) for read back
 * @param address       40-bit memory address to start setting bits
 * @param count         Number of 32-bit words to set (1-8)
 * @param sync          Type of synchronization to use (must be sig_done)
 * @param sig_ptr       Signal to raise upon completion
 *
 * Same as the set_imm instruction but it also returns the premodified
 * memory values in the read transfer registers.
 * @see cmd_mem_set_imm_ptr32()

 * The operation performed on each 32-bit memory location is:
 *
 * @code
 * rd_back = memory
 * memory = memory | data
 * @endcode
 *
 */

__intrinsic
void cmd_mem_test_and_set_imm_ptr40(
    uint32_t data,
    __xread void *rd_back,
    volatile void __addr40 __mem *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
    );

/** Clear bits in memory as specified by the bits set in Xfer register(s).
 *
 * @param data     Xfer register(s) specifying the bits to be cleared
 * @param address  32-bit memory address to start clearing bits in
 * @param count    Number of 32-bit words to set (1-8)
 * @param sync     Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr  Signal to raise upon completion
 *
 * The operation performed on each 32-bit memory location is:
 *
 * @code
 * memory = memory & ~data
 * @endcode
 *
 */
__intrinsic
void cmd_mem_clr_ptr32(
    __xwrite void *data,
    volatile void __addr32 __mem *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
    );

/** Clear bits in memory as specified by the bits set in Xfer register(s).
 *
 * @param data     Xfer register(s) specifying the bits to be cleared
 * @param address  40-bit memory address to start clearing bits in
 * @param count    Number of 32-bit words to set (1-8)
 * @param sync     Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr  Signal to raise upon completion
 *
 * The operation performed on each 32-bit memory location is:
 *
 * @code
 * memory = memory & ~data
 * @endcode
 *
 *
 */

__intrinsic
void cmd_mem_clr_ptr40(
    __xwrite void *data,
    volatile void __addr40 __mem *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
    );

/** Clear bits in memory as specified by the bits set in Xfer register(s) using
 * indirect format.
 *
 * @param data      Xfer register(s) specifying the bits to be cleared
 * @param address   Memory address to start clearing bits in
 * @param max_nn    Max number of 32-bit words to clear (1-8)
 * @param ind       Indirect word
 * @param sync      Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr   Signal to raise upon completion
 *
 * The byte mask in the indirect_ref word can be used to mask the operation but
 * is usually not used as clearing bits in the transfer registers has a
 * similar outcome.
 *
 * The operation performed on each 32-bit memory location is:
 *
 * @code
 * memory = memory & ~data
 * @endcode
 *
 */
__intrinsic
void cmd_mem_clr_ind_ptr32(
    __xwrite void *data,
    volatile void __addr32 __mem *address,
    uint32_t max_nn,
    generic_ind_t ind,
    sync_t sync,
    SIGNAL *sig_ptr
    );


/** Clear bits in memory as specified by the bits set in Xfer register(s) using
 * indirect format.
 *
 * @param data      Xfer register(s) specifying the bits to be cleared
 * @param address   40-bit memory address to start clearing bits in
 * @param max_nn    Max number of 32-bit words to clear (1-8)
 * @param ind       Indirect word
 * @param sync      Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr   Signal to raise upon completion
 *
 * The byte mask in the indirect_ref word can be used to mask the operation but
 * is usually not used as clearing bits in the transfer registers has a
 * similar outcome.
 *
 * The operation performed on each 32-bit memory location is:
 *
 * @code
 * memory = memory & ~data
 * @endcode
 *
 *
 */

__intrinsic
void cmd_mem_clr_ind_ptr40(
    __xwrite void *data,
    volatile void __addr40 __mem *address,
    uint32_t max_nn,
    generic_ind_t ind,
    sync_t sync,
    SIGNAL *sig_ptr
    );

/** Memory test and clear.
 *
 * @param val           Returned pre-modified value
 * @param data          Mask of bits to clear
 * @param address       32-bit memory address at which operation should be performed
 * @param count         Number of 32-bit words to clear (1-8)
 * @param sync          Type of synchronization to use (must be sig_done)
 * @param sig_pair_ptr  Signal pair to raise upon completion
 *
 * Same as the clr instruction but it also returns the premodified memory
 * values in the read transfer registers.
 * @see cmd_mem_clr_ptr40()
 *
 */
__intrinsic
void cmd_mem_test_and_clr_ptr32(
    __xread uint32_t *val,
    __xwrite void *data,
    volatile void __addr32 __mem *address,
    uint32_t count,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
    );

/** Memory test and clear.
 *
 * @param val           Returned pre-modified value
 * @param data          Mask of bits to clear
 * @param address       40-bit memory address at which operation should be performed
 * @param count         Number of 32-bit words to clear (1-8)
 * @param sync          Type of synchronization to use (must be sig_done)
 * @param sig_pair_ptr  Signal pair to raise upon completion
 *
 * Same as the clr instruction but it also returns the premodified memory
 * values in the read transfer registers.
 * @see cmd_mem_clr_ptr40()
 *
 *
 */

__intrinsic
void cmd_mem_test_and_clr_ptr40(
    __xread uint32_t *val,
    __xwrite void *data,
    volatile void __addr40 __mem *address,
    uint32_t count,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
    );



/** Memory test and clear in indirect mode.
 *
 * @param val           Returned pre-modified value
 * @param data          Mask of bits to clear
 * @param address       Memory address at which operation should be performed
 * @param max_nn        Max number of 32-bit words to clear (1-8)
 * @param ind           Indirect word
 * @param sync          Type of synchronization to use (must be sig_done)
 * @param sig_pair_ptr  Signal pair to raise upon completion
 *
 */
__intrinsic
void cmd_mem_test_and_clr_ind_ptr32(
    __xread uint32_t *val,
    __xwrite void *data,
    volatile void __addr32 __mem *address,
    uint32_t max_nn,
    generic_ind_t ind,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
    );


/** Memory test and clear in indirect mode.
 *
 * @param val           Returned pre-modified value
 * @param data          Mask of bits to clear
 * @param address       40-bit memory address at which operation should be performed
 * @param max_nn        Max number of 32-bit words to clear (1-8)
 * @param ind           Indirect word
 * @param sync          Type of synchronization to use (must be sig_done)
 * @param sig_pair_ptr  Signal pair to raise upon completion
 *
 *
 */

__intrinsic
void cmd_mem_test_and_clr_ind_ptr40(
    __xread uint32_t *val,
    __xwrite void *data,
    volatile void __addr40 __mem *address,
    uint32_t max_nn,
    generic_ind_t ind,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
    );



/** Clear bits in memory from immediate 32-bit word.
 *
 * @param data     Immediate data word specifying bits to clear in Memory
 * @param address  Memory address to start clearing bits
 * @param count    Number of 32-bit words to clear (1-8)
 *
 * @note Valid range for data is between 0 and 0x7f for 32xx indirect format and
 *       between 0 and 0xffff for 6xxx indirect format.
 *
 * Same as cmd_mem_clr_ptr40() but the bit mask is specified as immediate data.
 *
 * The operation performed on each 32-bit memory location is:
 *
 * @code
 * memory = memory & ~data
 * @endcode
 *
 */
__intrinsic
void cmd_mem_clr_imm_ptr32(
    uint32_t data,
    volatile void __addr32 __mem *address,    /* address to write */
    uint32_t count                          /* number of 32-bit words to write */
    );

/** Clear bits in memory from immediate 32-bit word.
 *
 * @param data     Immediate data word specifying bits to clear in Memory
 * @param address  40-bit memory address to start clearing bits
 * @param count    Number of 32-bit words to clear (1-8)
 *
 * @note Valid range for data is between 0 and 0x7f for 32xx indirect format and
 *       between 0 and 0xffff for 6xxx indirect format.
 *
 * Same as cmd_mem_clr_ptr40() but the bit mask is specified as immediate data.
 *
 * The operation performed on each 32-bit memory location is:
 *
 * @code
 * memory = memory & ~data
 * @endcode
 *
 */

__intrinsic
void cmd_mem_clr_imm_ptr40(
    uint32_t data,
    volatile void __addr40 __mem *address,    /* address to write */
    uint32_t count                          /* number of 32-bit words to write */
    );


/** Clear bits in memory from bits in immediate 32-bit word and return
 * pre-modified values in read transfer register(s).
 *
 * @param data          Immediate data to clear in Memory (0x0 - 0x7f)
 * @param rd_back       Xfer register to read back
 * @param address       32-bit memory address at which operation should be performed
 * @param count         Number of 32-bit words to clear (1-8)
 * @param sync          Type of synchronization to use (must be sig_done)
 * @param sig_ptr       Signal to raise upon completion
 *
 * Same as the clr_imm instruction but it also returns the premodified
 * memory values in the read transfer registers.
 * @see cmd_mem_clr_imm_ptr32()
 *
 */
__intrinsic
void cmd_mem_test_and_clr_imm_ptr32(
    uint32_t data,
    __xread void *rd_back,
    volatile void __addr32 __mem *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
    );

/** Clear bits in memory from bits in immediate 32-bit word and return
 * pre-modified values in read transfer register(s).
 *
 * @param data          Immediate data to clear in Memory (0x0 - 0x7f)
 * @param rd_back       Xfer register to read back
 * @param address       40-bit memory address at which operation should be performed
 * @param count         Number of 32-bit words to clear (1-8)
 * @param sync          Type of synchronization to use (must be sig_done)
 * @param sig_ptr       Signal to raise upon completion
 *
 * Same as the clr_imm instruction but it also returns the premodified
 * memory values in the read transfer registers.
 * @see cmd_mem_clr_imm_ptr32()
 *
 */

__intrinsic
void cmd_mem_test_and_clr_imm_ptr40(
    uint32_t data,
    __xread void *rd_back,
    volatile void __addr40 __mem *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
    );


/** Memory add32.
 *
 * @param data          Data to add
 * @param address       32-bit memory address at which operation should be performed
 * @param count         Number of 32-bit words to add (1-4)
 * @param sync          Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr       Signal to raise upon completion
 *
 */
__intrinsic
void cmd_mem_add32_ptr32(
    __xwrite void *data,
    volatile void __addr32 __mem *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
    );


/** Memory add32.
 *
 * @param data          Data to add
 * @param address       40-bit memory address at which operation should be performed
 * @param count         Number of 32-bit words to add (1-4)
 * @param sync          Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr       Signal to raise upon completion
 *
 *
 */

__intrinsic
void cmd_mem_add32_ptr40(
    __xwrite void *data,
    volatile void __addr40 __mem *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
    );



/** Memory add32 in indirect mode.
 *
 * @param data          Data to add
 * @param address       Memory address at which operation should be performed
 * @param max_nn        Max number of 32-bit words to add (1-4)
 * @param ind           Indirect word
 * @param sync          Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr       Signal to raise upon completion
 *
 */
__intrinsic
void cmd_mem_add32_ind_ptr32(
    __xwrite void *data,
    volatile void __addr32 __mem *address,
    uint32_t max_nn,
    generic_ind_t ind,
    sync_t sync,
    SIGNAL *sig_ptr
    );


/** Memory add32 in indirect mode.
 *
 * @param data          Data to add
 * @param address       40-bit memory address at which operation should be performed
 * @param max_nn        Max number of 32-bit words to add (1-4)
 * @param ind           Indirect word
 * @param sync          Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr       Signal to raise upon completion
 *
 */

__intrinsic
void cmd_mem_add32_ind_ptr40(
    __xwrite void *data,
    volatile void __addr40 __mem *address,
    uint32_t max_nn,
    generic_ind_t ind,
    sync_t sync,
    SIGNAL *sig_ptr
    );


/** Memory add64.
 *
 * @param data          Data to add
 * @param address       32-bit memory address at which operation should be performed
 * @param count         Number of 64-bit words to add (1-4)
 * @param sync          Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr       Signal to raise upon completion
 *
 * Add the 64-bit value(s) in the transfer registers to the data at the address.
 *
 */
__intrinsic
void cmd_mem_add64_ptr32(
    __xwrite void *data,
    volatile void __addr32 __mem *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
    );


/** Memory add64.
 *
 * @param data          Data to add
 * @param address       40-bit memory address at which operation should be performed
 * @param count         Number of 64-bit words to add (1-4)
 * @param sync          Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr       Signal to raise upon completion
 *
 * Add the 64-bit value(s) in the transfer registers to the data at the address.
 *
 */

__intrinsic
void cmd_mem_add64_ptr40(
    __xwrite void *data,
    volatile void __addr40 __mem *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
    );

/** Memory add64 in indirect mode.
 *
 * @param data          Data to add
 * @param address       Memory address at which operation should be performed
 * @param max_nn        Max number of 64-bit words to add (1-4)
 * @param ind           Indirect word
 * @param sync          Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr       Signal to raise upon completion
 *
 * Note that length[2:2] in the indirect word has to be set to 0b1 for this 64-bit command.
 * See 6xxx databook for more information.
 *
 */
__intrinsic
void cmd_mem_add64_ind_ptr32(
    __xwrite void *data,
    volatile void __addr32 __mem *address,
    uint32_t max_nn,
    generic_ind_t ind,
    sync_t sync,
    SIGNAL *sig_ptr
    );


/** Memory add64 in indirect mode.
 *
 * @param data          Data to add
 * @param address       40-bit memory address at which operation should be performed
 * @param max_nn        Max number of 64-bit words to add (1-4)
 * @param ind           Indirect word
 * @param sync          Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr       Signal to raise upon completion
 *
 * Note that length[2:2] in the indirect word has to be set to 0b1 for this 64-bit command.
 * See 6xxx databook for more information.
 *
 */

__intrinsic
void cmd_mem_add64_ind_ptr40(
    __xwrite void *data,
    volatile void __addr40 __mem *address,
    uint32_t max_nn,
    generic_ind_t ind,
    sync_t sync,
    SIGNAL *sig_ptr
    );

/** Memory test and add32.
 *
 * @param val           Returned pre-modified value
 * @param data          Data to add
 * @param address       32-bit memory address at which operation should be performed
 * @param count         Number of 32-bit words to add (1-4)
 * @param sync          Type of synchronization to use (must be sig_done)
 * @param sig_pair_ptr  Signal pair to raise upon completion
 *
 * Same as the add32 instruction but it also returns the premodified memory
 * values in the read transfer registers.
 * @see cmd_mem_add32_ptr40()
 *
 */
__intrinsic
void cmd_mem_test_and_add32_ptr32(
    __xread uint32_t *val,
    __xwrite void *data,
    volatile void __addr32 __mem *address,
    uint32_t count,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
    );

/** Memory test and add32.
 *
 * @param val           Returned pre-modified value
 * @param data          Data to add
 * @param address       40-bit memory address at which operation should be performed
 * @param count         Number of 32-bit words to add (1-4)
 * @param sync          Type of synchronization to use (must be sig_done)
 * @param sig_pair_ptr  Signal pair to raise upon completion
 *
 * Same as the add32 instruction but it also returns the premodified memory
 * values in the read transfer registers.
 * @see cmd_mem_add32_ptr40()
 *
 *
 */

__intrinsic
void cmd_mem_test_and_add32_ptr40(
    __xread uint32_t *val,
    __xwrite void *data,
    volatile void __addr40 __mem *address,
    uint32_t count,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
    );

/** Memory test and add32 in indirect mode.
 *
 * @param val           Returned pre-modified value
 * @param data          Data to add
 * @param address       Memory address at which operation should be performed
 * @param max_nn        Max number of 32-bit words to add (1-4)
 * @param ind           Indirect word
 * @param sync          Type of synchronization to use (must be sig_done)
 * @param sig_pair_ptr  Signal pair to raise upon completion
 *
 * @see cmd_mem_test_and_add32_ptr40()
 *
 */
__intrinsic
void cmd_mem_test_and_add32_ind_ptr32(
    __xread uint32_t *val,
    __xwrite void *data,
    volatile void __addr32 __mem *address,
    uint32_t max_nn,
    generic_ind_t ind,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
    );

/** Memory test and add32 in indirect mode.
 *
 * @param val           Returned pre-modified value
 * @param data          Data to add
 * @param address       40-bit memory address at which operation should be performed
 * @param max_nn        Max number of 32-bit words to add (1-4)
 * @param ind           Indirect word
 * @param sync          Type of synchronization to use (must be sig_done)
 * @param sig_pair_ptr  Signal pair to raise upon completion
 *
 * @see cmd_mem_test_and_add32_ptr40()
 *
 *
 */

__intrinsic
void cmd_mem_test_and_add32_ind_ptr40(
    __xread uint32_t *val,
    __xwrite void *data,
    volatile void __addr40 __mem *address,
    uint32_t max_nn,
    generic_ind_t ind,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
    );


/** Memory test and add64.
 *
 * @param val           Returned pre-modified value
 * @param data          Data to add
 * @param address       32-bit memory address at which operation should be performed
 * @param count         Number of 64-bit words to write (1-4)
 * @param sync          Type of synchronization to use (must be sig_done)
 * @param sig_pair_ptr  Signal pair to raise upon completion
 *
 * Same as the add64 instruction but it also returns the premodified
 * memory values in the read transfer registers.
 * @see cmd_mem_add64_ptr40()
 *
 */
__intrinsic
void cmd_mem_test_and_add64_ptr32(
    __xread uint32_t *val,
    __xwrite void *data,
    volatile void __addr32 __mem *address,
    uint32_t count,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
    );

/** Memory test and add64.
 *
 * @param val           Returned pre-modified value
 * @param data          Data to add
 * @param address       40-bit memory address at which operation should be performed
 * @param count         Number of 64-bit words to write (1-4)
 * @param sync          Type of synchronization to use (must be sig_done)
 * @param sig_pair_ptr  Signal pair to raise upon completion
 *
 * Same as the add64 instruction but it also returns the premodified
 * memory values in the read transfer registers.
 * @see cmd_mem_add64_ptr40()
 *
 *
 */

__intrinsic
void cmd_mem_test_and_add64_ptr40(
    __xread uint32_t *val,
    __xwrite void *data,
    volatile void __addr40 __mem *address,
    uint32_t count,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
    );

/** Memory test and add64 in indirect mode.
 *
 * @param val           Returned pre-modified value
 * @param data          Data to add
 * @param address       Memory address at which operation should be performed
 * @param max_nn        Max number of 64-bit words to write (1-4)
 * @param ind           Indirect word
 * @param sync          Type of synchronization to use (must be sig_done)
 * @param sig_pair_ptr  Signal pair to raise upon completion
 *
 * Note that length[2:2] in the indirect word has to be set to 0b1 for this 64-bit command.
 * See 6xxx databook for more information.
 *
 * @see cmd_mem_test_and_add64_ptr40()
 *
 */
__intrinsic
void cmd_mem_test_and_add64_ind_ptr32(
    __xread uint32_t *val,
    __xwrite void *data,
    volatile void __addr32 __mem *address,
    uint32_t max_nn,
    generic_ind_t ind,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
    );

/** Memory test and add64 in indirect mode.
 *
 * @param val           Returned pre-modified value
 * @param data          Data to add
 * @param address       40-bit memory address at which operation should be performed
 * @param max_nn        Max number of 64-bit words to write (1-4)
 * @param ind           Indirect word
 * @param sync          Type of synchronization to use (must be sig_done)
 * @param sig_pair_ptr  Signal pair to raise upon completion
 *
 * Note that length[2:2] in the indirect word has to be set to 0b1 for this 64-bit command.
 * See 6xxx databook for more information.
 *
 * @see cmd_mem_test_and_add64_ptr40()
 *
 *
 */

__intrinsic
void cmd_mem_test_and_add64_ind_ptr40(
    __xread uint32_t *val,
    __xwrite void *data,
    volatile void __addr40 __mem *address,
    uint32_t max_nn,
    generic_ind_t ind,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
    );

/** Memory add32 immediate.
 *
 * @param data          Immediate data to add
 * @param address       32-bit memory address at which operation should be performed
 * @param count         Number of 32-bit words to write
 *
 * @note Valid range for data is between -0x80 and 0x7f for 32xx indirect format and
 *       between -0x8000 and 0x7fff for 6xxx indirect format.
 *
 * Same as the add32 instruction but the data to be added to memory is specified as
 * the immediate data and the operation is applied only to a single 32-bit memory word.
 *
 */
__intrinsic
void cmd_mem_add32_imm_ptr32(
    int32_t data,
    volatile void __addr32 __mem *address,    /* address to write */
    uint32_t count                          /* number of 32-bit words to write */
    );

/** Memory increment32.
 *
 * @param address       32-bit memory address at which operation should be performed
 *
 * Same as add32_imm_ptr32(1,address,1) without using a signal or transfer register.
 *
 * @note: Calling this function at a high rate can stall the ME
 *
 */
__intrinsic
void cmd_mem_incr32_ptr32(
    volatile void __addr32 __mem *address     /* address to increment */
    );

/** Memory add32 immediate.
 *
 * @param data          Immediate data to add
 * @param address       40-bit memory address at which operation should be performed
 * @param count         Number of 32-bit words to write
 *
 * @note Valid range for data is between -0x80 and 0x7f for 32xx indirect format and
 *       between -0x8000 and 0x7fff for 6xxx indirect format.
 *
 * Same as the add32 instruction but the data to be added to memory is specified as
 * the immediate data and the operation is applied only to a single 32-bit memory word.
 *
 *
 */

__intrinsic
void cmd_mem_add32_imm_ptr40(
    int32_t data,
    volatile void __addr40 __mem *address,    /* address to write */
    uint32_t count                          /* number of 32-bit words to write */
    );


/** Memory increment32.
 *
 * @param address       32-bit memory address at which operation should be performed
 *
 * Same as add32_imm_ptr32(1,address,1) without using a signal or transfer register.
 *
 * @note: Calling this function at a high rate can stall the ME
 *
 *
 */

__intrinsic
void cmd_mem_incr32_ptr40(
    volatile void __addr40 __mem *address /* address to increment */
    );


/** Memory test and add32 immediate.
 *
 * @param data          Immediate data to add (-0x80 - 0x7f)
 * @param rd_back       Xfer register to read back
 * @param address       32-bit memory address at which operation should be performed
 * @param count         Number of 32-bit words to write
 * @param sync          Type of synchronization to use (must be sig_done)
 * @param sig_ptr       Signal to raise upon completion
 *
 * Same as the add32_imm instruction but it also returns the premodified
 * memory value in the read transfer register.
 * @see cmd_mem_add32_imm_ptr32()
 *
 */
__intrinsic
void cmd_mem_test_and_add32_imm_ptr32(
    int32_t data,                                           /* immediate data to add */
    __xread void *rd_back,                 /* address to read back */
    volatile void __addr32 __mem *address,   /* address to write */
    uint32_t count,                                 /* number of 32-bit words to write */
    sync_t sync,                                        /* must be sig_done */
    SIGNAL *sig_ptr                                     /* signal to raise upon completion */
    );

/** Memory test and add32 immediate.
 *
 * @param data          Immediate data to add (-0x80 - 0x7f)
 * @param rd_back       Xfer register to read back
 * @param address       40-bit memory address at which operation should be performed
 * @param count         Number of 32-bit words to write
 * @param sync          Type of synchronization to use (must be sig_done)
 * @param sig_ptr       Signal to raise upon completion
 *
 * Same as the add32_imm instruction but it also returns the premodified
 * memory value in the read transfer register.
 * @see cmd_mem_add32_imm_ptr32()
 *
 */

__intrinsic
void cmd_mem_test_and_add32_imm_ptr40(
    int32_t data,                                   /* immediate data to add */
    __xread void *rd_back,         /* address to read back */
    volatile void __addr40 __mem *address,    /* address to write */
    uint32_t count,                         /* number of 32-bit words to write */
    sync_t sync,                                /* must be sig_done */
    SIGNAL *sig_ptr                             /* signal to raise upon completion */
    );

/** Memory add64 immediate.
 *
 * @param data          Immediate data to add
 * @param address       32-bit memory address at which operation should be performed
 * @param count         Number of 64-bit words to add
 *
 * @note Valid range for data is between -0x80 and 0x7f for 32xx indirect format and
 *       between -0x8000 and 0x7fff for 6xxx indirect format.
 *
 * Same as the add64 instruction but the data to be added to memory is specified as
 * the immediate data and the operation is applied only to a single 64-bit memory word.
 *
 */
__intrinsic
void cmd_mem_add64_imm_ptr32(
    int32_t data,
    volatile void __addr32 __mem *address,
    uint32_t count
    );

/** Memory increment64.
 *
 * @param address       32-bit memory address at which operation should be performed
 *
 * Same as add64_imm_ptr32(1,address,1) without using a signal or transfer register.
 *
 * @note: Calling this function at a high rate can stall the ME
 *
 */
__intrinsic
void cmd_mem_incr64_ptr32(
    volatile void __addr32 __mem *address /* address to increment */
    );

/** Memory add64 immediate.
 *
 * @param data          Immediate data to add
 * @param address       40-bit memory address at which operation should be performed
 * @param count         Number of 64-bit words to add
 *
 * @note Valid range for data is between -0x80 and 0x7f for 32xx indirect format and
 *       between -0x8000 and 0x7fff for 6xxx indirect format.
 *
 * Same as the add64 instruction but the data to be added to memory is specified as
 * the immediate data and the operation is applied only to a single 64-bit memory word.
 *
 */

__intrinsic
void cmd_mem_add64_imm_ptr40(
    int32_t data,
    volatile void __addr40 __mem *address,
    uint32_t count
    );

/** Memory increment64.
 *
 * @param address       32-bit memory address at which operation should be performed
 *
 * Same as add64_imm_ptr32(1,address,1) without using a signal or transfer register.
 *
 * @note: Calling this function at a high rate can stall the ME
 *
 */

__intrinsic
void cmd_mem_incr64_ptr40(
    volatile void __addr40 __mem *address /* address to increment */
    );

/** Memory test and add64 immediate.
 *
 * @param data          Immediate data to add (-0x80 - 0x7f)
 * @param rd_back       Xfer register to read back
 * @param address       32-bit memory address at which operation should be performed
 * @param count         Number of 64-bit words to add
 * @param sync          Type of synchronization to use (must be sig_done)
 * @param sig_ptr       Signal to raise upon completion
 *
 * Same as the add64_imm instruction but it also returns the premodified
 * memory values in the read transfer register.
 * @see cmd_mem_add64_imm_ptr32()
 *
 */
__intrinsic
void cmd_mem_test_and_add64_imm_ptr32(
    int32_t data,
    __xread void *rd_back,
    volatile void __addr32 __mem *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
    );

/** Memory test and add64 immediate.
 *
 * @param data          Immediate data to add (-0x80 - 0x7f)
 * @param rd_back       Xfer register to read back
 * @param address       40-bit memory address at which operation should be performed
 * @param count         Number of 64-bit words to add
 * @param sync          Type of synchronization to use (must be sig_done)
 * @param sig_ptr       Signal to raise upon completion
 *
 * Same as the add64_imm instruction but it also returns the premodified
 * memory values in the read transfer register.
 * @see cmd_mem_add64_imm_ptr32()
 *
 */

__intrinsic
void cmd_mem_test_and_add64_imm_ptr40(
    int32_t data,
    __xread void *rd_back,
    volatile void __addr40 __mem *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
    );

/** Memory add32 with saturation.
 *
 * @param data          Data to add
 * @param address       32-bit memory address at which operation should be performed
 * @param count         Number of 32-bit words to write (1-4)
 * @param sync          Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr       Signal to raise upon completion
 *
 * Add the signed value in the transfer registers to the data at the address.
 * Addition with results of < 0 will saturate the result in the value in memory at 0x0000 0000.
 * Positive number addition resulting in a value of > 0xFFFF FFFF,
 * will saturate the result at 0xFFFF FFFF.
 *
 */
__intrinsic
void cmd_mem_add32_sat_ptr32(
    __xwrite void *data,
    volatile void __addr32 __mem *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
    );

/** Memory add32 with saturation.
 *
 * @param data          Data to add
 * @param address       40-bit memory address at which operation should be performed
 * @param count         Number of 32-bit words to write (1-4)
 * @param sync          Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr       Signal to raise upon completion
 *
 * Add the signed value in the transfer registers to the data at the address.
 * Addition with results of < 0 will saturate the result in the value in memory at 0x0000 0000.
 * Positive number addition resulting in a value of > 0xFFFF FFFF,
 * will saturate the result at 0xFFFF FFFF.
 *
 */

__intrinsic
void cmd_mem_add32_sat_ptr40(
    __xwrite void *data,
    volatile void __addr40 __mem *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
    );

/** Memory add32 with saturation in indirect mode.
 *
 * @param data          Data to add
 * @param address       Memory address at which operation should be performed
 * @param max_nn        Max number of 32-bit words to write (1-4)
 * @param ind           Indirect word
 * @param sync          Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr       Signal to raise upon completion
 *
 * @see cmd_mem_add32_sat_ptr40()
 *
 */
__intrinsic
void cmd_mem_add32_sat_ind_ptr32(
    __xwrite void *data,
    volatile void __addr32 __mem *address,
    uint32_t max_nn,
    generic_ind_t ind,
    sync_t sync,
    SIGNAL *sig_ptr
    );


/** Memory add32 with saturation in indirect mode.
 *
 * @param data          Data to add
 * @param address       40-bit memory address at which operation should be performed
 * @param max_nn        Max number of 32-bit words to write (1-4)
 * @param ind           Indirect word
 * @param sync          Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr       Signal to raise upon completion
 *
 * @see cmd_mem_add32_sat_ptr40()
 *
 */

__intrinsic
void cmd_mem_add32_sat_ind_ptr40(
    __xwrite void *data,
    volatile void __addr40 __mem *address,
    uint32_t max_nn,
    generic_ind_t ind,
    sync_t sync,
    SIGNAL *sig_ptr
    );

/** Memory add64 with saturation.
 *
 * @param data          Data to add
 * @param address       32-bit memory address at which operation should be performed
 * @param count         Number of 64-bit words to add (1-4)
 * @param sync          Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr       Signal to raise upon completion
 *
 * Add the signed 64-bit value in the transfer registers to the data at the address.
 * Addition with results of < 0 will saturate the result in the value in memory at 0x0000 0000.
 * Positive number addition resulting in a value of > 0xFFFF FFFF FFFF FFFF,
 * will saturate the result at 0xFFFF FFFF FFFF FFFF.
 *
 */
__intrinsic
void cmd_mem_add64_sat_ptr32(
    __xwrite void *data,
    volatile void __addr32 __mem *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
    );

/** Memory add64 with saturation.
 *
 * @param data          Data to add
 * @param address       40-bit memory address at which operation should be performed
 * @param count         Number of 64-bit words to add (1-4)
 * @param sync          Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr       Signal to raise upon completion
 *
 * Add the signed 64-bit value in the transfer registers to the data at the address.
 * Addition with results of < 0 will saturate the result in the value in memory at 0x0000 0000.
 * Positive number addition resulting in a value of > 0xFFFF FFFF FFFF FFFF,
 * will saturate the result at 0xFFFF FFFF FFFF FFFF.
 *
 */

__intrinsic
void cmd_mem_add64_sat_ptr40(
    __xwrite void *data,
    volatile void __addr40 __mem *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
    );

/** Memory add64 with saturation in indirect mode.
 *
 * @param data          Data to add
 * @param address       Memory address at which operation should be performed
 * @param max_nn        Max number of 64-bit words to add (1-4)
 * @param ind           Indirect word
 * @param sync          Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr       Signal to raise upon completion
 *
 * Note that length[2:2] in the indirect word has to be set to 0b1 for this 64-bit command.
 * See 6xxx databook for more information.
 *
 * @see cmd_mem_add64_sat_ptr40()
 *
 */
__intrinsic
void cmd_mem_add64_sat_ind_ptr32(
    __xwrite void *data,
    volatile void __addr32 __mem *address,
    uint32_t max_nn,
    generic_ind_t ind,
    sync_t sync,
    SIGNAL *sig_ptr
    );


/** Memory add64 with saturation in indirect mode.
 *
 * @param data          Data to add
 * @param address       40-bit memory address at which operation should be performed
 * @param max_nn        Max number of 64-bit words to add (1-4)
 * @param ind           Indirect word
 * @param sync          Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr       Signal to raise upon completion
 *
 * Note that length[2:2] in the indirect word has to be set to 0b1 for this 64-bit command.
 * See 6xxx databook for more information.
 *
 * @see cmd_mem_add64_sat_ptr40()
 *
 */

__intrinsic
void cmd_mem_add64_sat_ind_ptr40(
    __xwrite void *data,
    volatile void __addr40 __mem *address,
    uint32_t max_nn,
    generic_ind_t ind,
    sync_t sync,
    SIGNAL *sig_ptr
    );

/** Memory test and add32 with saturation.
 *
 * @param val           Returned pre-modified value
 * @param data          Data to add
 * @param address       32-bit memory address at which operation should be performed
 * @param count         Number of 32-bit words to write (1-4)
 * @param sync          Type of synchronization to use (must be sig_done)
 * @param sig_pair_ptr  Signal pair to raise upon completion
 *
 * Same as the add32_sat instruction but it also returns the premodified
 * memory values in the read transfer registers.
 * @see cmd_mem_add32_sat_ptr32()
 *
 */
__intrinsic
void cmd_mem_test_and_add32_sat_ptr32(
    __xread uint32_t *val,
    __xwrite void *data,
    volatile void __addr32 __mem *address,
    uint32_t count,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
    );


/** Memory test and add32 with saturation.
 *
 * @param val           Returned pre-modified value
 * @param data          Data to add
 * @param address       40-bit memory address at which operation should be performed
 * @param count         Number of 32-bit words to write (1-4)
 * @param sync          Type of synchronization to use (must be sig_done)
 * @param sig_pair_ptr  Signal pair to raise upon completion
 *
 * Same as the add32_sat instruction but it also returns the premodified
 * memory values in the read transfer registers.
 * @see cmd_mem_add32_sat_ptr32()
 *
 */

__intrinsic
void cmd_mem_test_and_add32_sat_ptr40(
    __xread uint32_t *val,
    __xwrite void *data,
    volatile void __addr40 __mem *address,
    uint32_t count,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
    );

/** Memory test and add32 with saturation in indirect mode.
 *
 * @param val           Returned pre-modified value
 * @param data          Data to add
 * @param address       Memory address at which operation should be performed
 * @param max_nn        Max number of 32-bit words to add (1-4)
 * @param ind           Indirect word
 * @param sync          Type of synchronization to use (must be sig_done)
 * @param sig_pair_ptr  Signal pair to raise upon completion
 *
 * @see cmd_mem_test_and_add32_sat_ptr40()
 *
 */
__intrinsic
void cmd_mem_test_and_add32_sat_ind_ptr32(
    __xread uint32_t *val,
    __xwrite void *data,
    volatile void __addr32 __mem *address,
    uint32_t max_nn,
    generic_ind_t ind,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
    );

/** Memory test and add32 with saturation in indirect mode.
 *
 * @param val           Returned pre-modified value
 * @param data          Data to add
 * @param address       40-bit memory address at which operation should be performed
 * @param max_nn        Max number of 32-bit words to add (1-4)
 * @param ind           Indirect word
 * @param sync          Type of synchronization to use (must be sig_done)
 * @param sig_pair_ptr  Signal pair to raise upon completion
 *
 * @see cmd_mem_test_and_add32_sat_ptr40()
 *
 */

__intrinsic
void cmd_mem_test_and_add32_sat_ind_ptr40(
    __xread uint32_t *val,
    __xwrite void *data,
    volatile void __addr40 __mem *address,
    uint32_t max_nn,
    generic_ind_t ind,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
    );

/** Memory test and add64 with saturation.
 *
 * @param val           Returned pre-modified value
 * @param data          Data to add
 * @param address       32-bit memory address at which operation should be performed
 * @param count         Number of 64-bit words to add (1-4)
 * @param sync          Type of synchronization to use (must be sig_done)
 * @param sig_pair_ptr  Signal pair to raise upon completion
 *
 * Same as the addsat64 instruction but it also returns the premodified
 * memory values in the read transfer registers.
 * @see cmd_mem_add64_sat_ptr32()
 *
 */
__intrinsic
void cmd_mem_test_and_add64_sat_ptr32(
    __xread uint32_t *val,
    __xwrite void *data,
    volatile void __addr32 __mem *address,
    uint32_t count,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
    );

/** Memory test and add64 with saturation.
 *
 * @param val           Returned pre-modified value
 * @param data          Data to add
 * @param address       40-bit memory address at which operation should be performed
 * @param count         Number of 64-bit words to add (1-4)
 * @param sync          Type of synchronization to use (must be sig_done)
 * @param sig_pair_ptr  Signal pair to raise upon completion
 *
 * Same as the addsat64 instruction but it also returns the premodified
 * memory values in the read transfer registers.
 * @see cmd_mem_add64_sat_ptr32()
 *
 */

__intrinsic
void cmd_mem_test_and_add64_sat_ptr40(
    __xread uint32_t *val,
    __xwrite void *data,
    volatile void __addr40 __mem *address,
    uint32_t count,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
    );

/** Memory test and add64 with saturation in indirect mode.
 *
 * @param val           Returned pre-modified value
 * @param data          Data to add
 * @param address       Memory address at which operation should be performed
 * @param max_nn        Max number of 64-bit words to add (1-4)
 * @param ind           Indirect word
 * @param sync          Type of synchronization to use (must be sig_done)
 * @param sig_pair_ptr  Signal pair to raise upon completion
 *
 * Note that length[2:2] in the indirect word has to be set to 0b1 for this 64-bit command.
 * See 6xxx databook for more information.
 *
 * @see cmd_mem_test_and_add64_sat_ptr40()
 *
 */
__intrinsic
void cmd_mem_test_and_add64_sat_ind_ptr32(
    __xread uint32_t *val,
    __xwrite void *data,
    volatile void __addr32 __mem *address,
    uint32_t max_nn,
    generic_ind_t ind,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
    );

/** Memory test and add64 with saturation in indirect mode.
 *
 * @param val           Returned pre-modified value
 * @param data          Data to add
 * @param address       40-bit memory address at which operation should be performed
 * @param max_nn        Max number of 64-bit words to add (1-4)
 * @param ind           Indirect word
 * @param sync          Type of synchronization to use (must be sig_done)
 * @param sig_pair_ptr  Signal pair to raise upon completion
 *
 * Note that length[2:2] in the indirect word has to be set to 0b1 for this 64-bit command.
 * See 6xxx databook for more information.
 *
 * @see cmd_mem_test_and_add64_sat_ptr40()
 *
 */

__intrinsic
void cmd_mem_test_and_add64_sat_ind_ptr40(
    __xread uint32_t *val,
    __xwrite void *data,
    volatile void __addr40 __mem *address,
    uint32_t max_nn,
    generic_ind_t ind,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
    );

/** Memory add32 immediate with saturation.
 *
 * @param data          Immediate data to add
 * @param address       32-bit memory address at which operation should be performed
 * @param count         Number of 32-bit words to add
 *
 * @note Valid range for data is between -0x80 and 0x7f for 32xx indirect format and
 *       between -0x8000 and 0x7fff for 6xxx indirect format.
 *
 * Same as the add32_sat instruction but the data to be added to memory is specified
 * as the immediate data and the operation is applied only to a single 32-bit memory word.
 * @see cmd_mem_add32_sat_ptr32()
 *
 */
__intrinsic
void cmd_mem_add32_imm_sat_ptr32(
    int32_t data,
    volatile void __addr32 __mem *address,
    uint32_t count
    );


/** Memory add32 immediate with saturation.
 *
 * @param data          Immediate data to add
 * @param address       40-bit memory address at which operation should be performed
 * @param count         Number of 32-bit words to add
 *
 * @note Valid range for data is between -0x80 and 0x7f for 32xx indirect format and
 *       between -0x8000 and 0x7fff for 6xxx indirect format.
 *
 * Same as the add32_sat instruction but the data to be added to memory is specified
 * as the immediate data and the operation is applied only to a single 32-bit memory word.
 * @see cmd_mem_add32_sat_ptr32()
 *
 */

__intrinsic
void cmd_mem_add32_imm_sat_ptr40(
    int32_t data,
    volatile void __addr40 __mem *address,
    uint32_t count
    );


/** Memory test and add32 immediate with saturation.
 *
 * @param data          Immediate data to add (-0x80 - 0x7f)
 * @param rd_back       Xfer register to read back
 * @param address       32-bit memory address at which operation should be performed
 * @param count         Number of 32-bit words to add
 * @param sync          Type of synchronization to use (must be sig_done)
 * @param sig_ptr       Signal to raise upon completion
 *
 * Same as the add32_imm_sat instruction but it also returns the premodified
 * memory values in the read transfer register.
 * @see cmd_mem_add32_imm_sat_ptr32()
 *
 */
__intrinsic
void cmd_mem_test_and_add32_imm_sat_ptr32(
    int32_t data,
    __xread void *rd_back,
    volatile void __addr32 __mem *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
    );

/** Memory test and add32 immediate with saturation.
 *
 * @param data          Immediate data to add (-0x80 - 0x7f)
 * @param rd_back       Xfer register to read back
 * @param address       40-bit memory address at which operation should be performed
 * @param count         Number of 32-bit words to add
 * @param sync          Type of synchronization to use (must be sig_done)
 * @param sig_ptr       Signal to raise upon completion
 *
 * Same as the add32_imm_sat instruction but it also returns the premodified
 * memory values in the read transfer register.
 * @see cmd_mem_add32_imm_sat_ptr32()
 *
 */

__intrinsic
void cmd_mem_test_and_add32_imm_sat_ptr40(
    int32_t data,
    __xread void *rd_back,
    volatile void __addr40 __mem *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
    );


/** Memory add64 immediate with saturation.
 *
 * @param data          Immediate data to add
 * @param address       32-bit memory address at which operation should be performed
 * @param count         Number of 64-bit words to add
 *
 * @note Valid range for data is between -0x80 and 0x7f for 32xx indirect format and
 *       between -0x8000 and 0x7fff for 6xxx indirect format.
 *
 * Same as the addsat64 instruction but the data to be added to memory
 * is specified as the immediate data and the operation is applied only to
 * a single 64-bit memory word.
 * @see cmd_mem_add64_sat_ptr32()
 *
 */
__intrinsic
void cmd_mem_add64_imm_sat_ptr32(
    int32_t data,
    volatile void __addr32 __mem *address,
    uint32_t count
    );

/** Memory add64 immediate with saturation.
 *
 * @param data          Immediate data to add
 * @param address       40-bit memory address at which operation should be performed
 * @param count         Number of 64-bit words to add
 *
 * @note Valid range for data is between -0x80 and 0x7f for 32xx indirect format and
 *       between -0x8000 and 0x7fff for 6xxx indirect format.
 *
 * Same as the addsat64 instruction but the data to be added to memory
 * is specified as the immediate data and the operation is applied only to
 * a single 64-bit memory word.
 * @see cmd_mem_add64_sat_ptr32()
 *
 */

__intrinsic
void cmd_mem_add64_imm_sat_ptr40(
    int32_t data,
    volatile void __addr40 __mem *address,
    uint32_t count
    );


/** Memory test and add64 immediate with saturation.
 *
 * @param data          Immediate data to add (-0x80 - 0x7f)
 * @param rd_back       Xfer register to read back
 * @param address       32-bit memory address at which operation should be performed
 * @param count         Number of 64-bit words to add
 * @param sync          Type of synchronization to use (must be sig_done)
 * @param sig_ptr       Signal to raise upon completion
 *
 * Same as the add64_imm instruction but it also returns the premodified
 * memory values in the read transfer register.
 * @see cmd_mem_add64_imm_ptr32()
 *
 */
__intrinsic
void cmd_mem_test_and_add64_imm_sat_ptr32(
    int32_t data,
    __xread void *rd_back,
    volatile void __addr32 __mem *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
    );


/** Memory test and add64 immediate with saturation.
 *
 * @param data          Immediate data to add (-0x80 - 0x7f)
 * @param rd_back       Xfer register to read back
 * @param address       40-bit memory address at which operation should be performed
 * @param count         Number of 64-bit words to add
 * @param sync          Type of synchronization to use (must be sig_done)
 * @param sig_ptr       Signal to raise upon completion
 *
 * Same as the add64_imm instruction but it also returns the premodified
 * memory values in the read transfer register.
 * @see cmd_mem_add64_imm_ptr32()
 *
 */

__intrinsic
void cmd_mem_test_and_add64_imm_sat_ptr40(
    int32_t data,
    __xread void *rd_back,
    volatile void __addr40 __mem *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
    );


/** Memory sub32.
 *
 * @param data          Data to subtract
 * @param address       32-bit memory address at which operation should be performed
 * @param count         Number of 32-bit words to subtract (1-4)
 * @param sync          Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr       Signal to raise upon completion
 *
 * @note Valid range for data is between -0x80 and 0x7f for 32xx indirect format and
 *       between -0x8000 and 0x7fff for 6xxx indirect format.
 *
 * Subtract the 2s complement value in the transfer registers from the data
 * at the address, which is treated as an unsigned number.
 *
 */
__intrinsic
void cmd_mem_sub32_ptr32(
    __xwrite void *data,
    volatile void __addr32 __mem *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
    );


/** Memory sub32.
 *
 * @param data          Data to subtract
 * @param address       40-bit memory address at which operation should be performed
 * @param count         Number of 32-bit words to subtract (1-4)
 * @param sync          Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr       Signal to raise upon completion
 *
 * Subtract the 2s complement value in the transfer registers from the data
 * at the address, which is treated as an unsigned number.
 *
 */

__intrinsic
void cmd_mem_sub32_ptr40(
    __xwrite void *data,
    volatile void __addr40 __mem *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
    );


/** Memory sub32 in indirect mode.
 *
 * @param data          Data to subtract
 * @param address       Memory address at which operation should be performed
 * @param max_nn        Max number of 32-bit words to subtract (1-4)
 * @param ind           Indirect word
 * @param sync          Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr       Signal to raise upon completion
 *
 * @see cmd_mem_sub32_ptr32()
 *
 */
__intrinsic
void cmd_mem_sub32_ind_ptr32(
    __xwrite void *data,
    volatile void __addr32 __mem *address,
    uint32_t max_nn,
    generic_ind_t ind,
    sync_t sync,
    SIGNAL *sig_ptr
    );


/** Memory sub32 in indirect mode.
 *
 * @param data          Data to subtract
 * @param address       40-bit memory address at which operation should be performed
 * @param max_nn        Max number of 32-bit words to subtract (1-4)
 * @param ind           Indirect word
 * @param sync          Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr       Signal to raise upon completion
 *
 * @see cmd_mem_sub32_ptr32()
 *
 */

__intrinsic
void cmd_mem_sub32_ind_ptr40(
    __xwrite void *data,
    volatile void __addr40 __mem *address,
    uint32_t max_nn,
    generic_ind_t ind,
    sync_t sync,
    SIGNAL *sig_ptr
    );



/** Memory sub64.
 *
 * @param data          Data to subtract
 * @param address       32-bit memory address at which operation should be performed
 * @param count         Number of 64-bit words to subtract (1-4)
 * @param sync          Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr       Signal to raise upon completion
 *
 * Subtract the value in the transfer registers from the data at the address.
 *
 */
__intrinsic
void cmd_mem_sub64_ptr32(
    __xwrite void *data,
    volatile void __addr32 __mem *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
    );


/** Memory sub64.
 *
 * @param data          Data to subtract
 * @param address       40-bit memory address at which operation should be performed
 * @param count         Number of 64-bit words to subtract (1-4)
 * @param sync          Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr       Signal to raise upon completion
 *
 * Subtract the value in the transfer registers from the data at the address.
 *
 */

__intrinsic
void cmd_mem_sub64_ptr40(
    __xwrite void *data,
    volatile void __addr40 __mem *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
    );



/** Memory sub64 in indirect mode.
 *
 * @param data          Data to subtract
 * @param address       Memory address at which operation should be performed
 * @param max_nn        Max number of 64-bit words to subtract (1-4)
 * @param ind           Indirect word
 * @param sync          Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr       Signal to raise upon completion
 *
 * Note that length[2:2] in the indirect word has to be set to 0b1 for this 64-bit command.
 * See 6xxx databook for more information.
 *
 * @see cmd_mem_sub64_ptr32()
 *
 */
__intrinsic
void cmd_mem_sub64_ind_ptr32(
    __xwrite void *data,
    volatile void __addr32 __mem *address,
    uint32_t max_nn,
    generic_ind_t ind,
    sync_t sync,
    SIGNAL *sig_ptr
    );

/** Memory sub64 in indirect mode.
 *
 * @param data          Data to subtract
 * @param address       40-bit memory address at which operation should be performed
 * @param max_nn        Max number of 64-bit words to subtract (1-4)
 * @param ind           Indirect word
 * @param sync          Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr       Signal to raise upon completion
 *
 * Note that length[2:2] in the indirect word has to be set to 0b1 for this 64-bit command.
 * See 6xxx databook for more information.
 *
 * @see cmd_mem_sub64_ptr32()
 *
 */

__intrinsic
void cmd_mem_sub64_ind_ptr40(
    __xwrite void *data,
    volatile void __addr40 __mem *address,
    uint32_t max_nn,
    generic_ind_t ind,
    sync_t sync,
    SIGNAL *sig_ptr
    );


/** Memory test and sub32.
 *
 * @param val           Returned pre-modified value
 * @param data          Data to subtract
 * @param address       32-bit memory address at which operation should be performed
 * @param count         Number of 32-bit words to subtract (1-4)
 * @param sync          Type of synchronization to use (must be sig_done)
 * @param sig_pair_ptr  Signal pair to raise upon completion
 *
 * Same as the sub32 instruction but it also returns the premodified memory
 * values in the read transfer registers.
 * @see cmd_mem_sub32_ptr32()
 *
 */
__intrinsic
void cmd_mem_test_and_sub32_ptr32(
    __xread uint32_t *val,
    __xwrite void *data,
    volatile void __addr32 __mem *address,
    uint32_t count,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
    );


/** Memory test and sub32.
 *
 * @param val           Returned pre-modified value
 * @param data          Data to subtract
 * @param address       40-bit memory address at which operation should be performed
 * @param count         Number of 32-bit words to subtract (1-4)
 * @param sync          Type of synchronization to use (must be sig_done)
 * @param sig_pair_ptr  Signal pair to raise upon completion
 *
 * Same as the sub32 instruction but it also returns the premodified memory
 * values in the read transfer registers.
 * @see cmd_mem_sub32_ptr32()
 *
 */

__intrinsic
void cmd_mem_test_and_sub32_ptr40(
    __xread uint32_t *val,
    __xwrite void *data,
    volatile void __addr40 __mem *address,
    uint32_t count,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
    );

/** Memory test and sub32 in indirect mode.
 *
 * @param val           Returned pre-modified value
 * @param data          Data to subtract
 * @param address       Memory address at which operation should be performed
 * @param max_nn        Max number of 32-bit words to subtract (1-4)
 * @param ind           Indirect word
 * @param sync          Type of synchronization to use (must be sig_done)
 * @param sig_pair_ptr  Signal pair to raise upon completion
 *
 * @see cmd_mem_test_and_sub32_ptr40()
 *
 */
__intrinsic
void cmd_mem_test_and_sub32_ind_ptr32(
    __xread uint32_t *val,
    __xwrite void *data,
    volatile void __addr32 __mem *address,
    uint32_t max_nn,
    generic_ind_t ind,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
    );


/** Memory test and sub32 in indirect mode.
 *
 * @param val           Returned pre-modified value
 * @param data          Data to subtract
 * @param address       40-bit memory address at which operation should be performed
 * @param max_nn        Max number of 32-bit words to subtract (1-4)
 * @param ind           Indirect word
 * @param sync          Type of synchronization to use (must be sig_done)
 * @param sig_pair_ptr  Signal pair to raise upon completion
 *
 * @see cmd_mem_test_and_sub32_ptr40()
 *
 */

__intrinsic
void cmd_mem_test_and_sub32_ind_ptr40(
    __xread uint32_t *val,
    __xwrite void *data,
    volatile void __addr40 __mem *address,
    uint32_t max_nn,
    generic_ind_t ind,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
    );

/** Memory test and sub64.
 *
 * @param val           Returned pre-modified value
 * @param data          Data to subtract
 * @param address       32-bit memory address at which operation should be performed
 * @param count         Number of 64-bit words to subtract (1-4)
 * @param sync          Type of synchronization to use (must be sig_done)
 * @param sig_pair_ptr  Signal pair to raise upon completion
 *
 * Same as the sub64 instruction but it also returns the premodified memory
 * values in the read transfer registers.
 * @see cmd_mem_sub64_ptr32()
 *
 */
__intrinsic
void cmd_mem_test_and_sub64_ptr32(
    __xread uint32_t *val,
    __xwrite void *data,
    volatile void __addr32 __mem *address,
    uint32_t count,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
    );


/** Memory test and sub64.
 *
 * @param val           Returned pre-modified value
 * @param data          Data to subtract
 * @param address       40-bit memory address at which operation should be performed
 * @param count         Number of 64-bit words to subtract (1-4)
 * @param sync          Type of synchronization to use (must be sig_done)
 * @param sig_pair_ptr  Signal pair to raise upon completion
 *
 * Same as the sub64 instruction but it also returns the premodified memory
 * values in the read transfer registers.
 * @see cmd_mem_sub64_ptr32()
 *
 */

__intrinsic
void cmd_mem_test_and_sub64_ptr40(
    __xread uint32_t *val,
    __xwrite void *data,
    volatile void __addr40 __mem *address,
    uint32_t count,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
    );


/** Memory test and sub64 in indirect mode.
 *
 * @param val           Returned pre-modified value
 * @param data          Data to subtract
 * @param address       Memory address at which operation should be performed
 * @param max_nn        Max number of 64-bit words to subtract (1-4)
 * @param ind           Indirect word
 * @param sync          Type of synchronization to use (must be sig_done)
 * @param sig_pair_ptr  Signal pair to raise upon completion
 *
 * Note that length[2:2] in the indirect word has to be set to 0b1 for this 64-bit command.
 * See 6xxx databook for more information.
 *
 * @see cmd_mem_test_and_sub64_ptr40()
 *
 */
__intrinsic
void cmd_mem_test_and_sub64_ind_ptr32(
    __xread uint32_t *val,
    __xwrite void *data,
    volatile void __addr32 __mem *address,
    uint32_t max_nn,
    generic_ind_t ind,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
);


/** Memory test and sub64 in indirect mode.
 *
 * @param val           Returned pre-modified value
 * @param data          Data to subtract
 * @param address       40-bit memory address at which operation should be performed
 * @param max_nn        Max number of 64-bit words to subtract (1-4)
 * @param ind           Indirect word
 * @param sync          Type of synchronization to use (must be sig_done)
 * @param sig_pair_ptr  Signal pair to raise upon completion
 *
 * Note that length[2:2] in the indirect word has to be set to 0b1 for this 64-bit command.
 * See 6xxx databook for more information.
 *
 * @see cmd_mem_test_and_sub64_ptr40()
 *
 */

__intrinsic
void cmd_mem_test_and_sub64_ind_ptr40(
    __xread uint32_t *val,
    __xwrite void *data,
    volatile void __addr40 __mem *address,
    uint32_t max_nn,
    generic_ind_t ind,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
);


/** Memory sub32 immediate.
 *
 * @param data          Immediate data to subtract
 * @param address       32-bit memory address at which operation should be performed
 * @param count         Number of 32-bit words to subtract
 *
 * @note Valid range for data is between -0x80 and 0x7f for 32xx indirect format and
 *       between -0x8000 and 0x7fff for 6xxx indirect format.
 *
 * Same as the sub32 instruction but the data to be subtracted to memory is specified as
 * the immediate data and the operation is applied only to a single 32-bit memory word.
 *
 */
__intrinsic
void cmd_mem_sub32_imm_ptr32(
    int32_t data,
    volatile void __addr32 __mem *address,
    uint32_t count
    );

/** Memory decrement32.
 *
 * @param address       32-bit memory address at which operation should be performed
 *
 * Same as sub32_imm_ptr32(1,address,1) without using a signal or transfer register.
 *
 * @note: Calling this function at a high rate can stall the ME
 *
 */
__intrinsic
void cmd_mem_decr32_ptr32(
    volatile void __addr32 __mem *address     /* address to decrement */
    );

/** Memory sub32 immediate.
 *
 * @param data          Immediate data to subtract
 * @param address       40-bit memory address at which operation should be performed
 * @param count         Number of 32-bit words to subtract
 *
 * @note Valid range for data is between -0x80 and 0x7f for 32xx indirect format and
 *       between -0x8000 and 0x7fff for 6xxx indirect format.
 *
 * Same as the sub32 instruction but the data to be subtracted to memory is specified as
 * the immediate data and the operation is applied only to a single 32-bit memory word.
 *
 */

__intrinsic
void cmd_mem_sub32_imm_ptr40(
    int32_t data,
    volatile void __addr40 __mem *address,
    uint32_t count
    );


/** Memory decrement32.
 *
 * @param address       32-bit memory address at which operation should be performed
 *
 * Same as sub32_imm_ptr32(1,address,1) without using a signal or transfer register.
 *
 * @note: Calling this function at a high rate can stall the ME
 *
 */

__intrinsic
void cmd_mem_decr32_ptr40(
    volatile void __addr40 __mem *address     /* address to decrement */
    );


/** Memory test and sub32 immediate.
 *
 * @param data          Immediate data to subtract (-0x80 - 0x7f)
 * @param rd_back       Xfer register to read back
 * @param address       32-bit memory address at which operation should be performed
 * @param count         Number of 32-bit words to subtract
 * @param sync          Type of synchronization to use (must be sig_done)
 * @param sig_ptr       Signal to raise upon completion
 *
 * Same as the sub32_imm instruction but it also returns the premodified
 * memory values in the read transfer register.
 * @see cmd_mem_sub32_imm_ptr32()
 *
 */
__intrinsic
void cmd_mem_test_and_sub32_imm_ptr32(
    int32_t data,
    __xread void *rd_back,
    volatile void __addr32 __mem *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
    );


/** Memory test and sub32 immediate.
 *
 * @param data          Immediate data to subtract (-0x80 - 0x7f)
 * @param rd_back       Xfer register to read back
 * @param address       40-bit memory address at which operation should be performed
 * @param count         Number of 32-bit words to subtract
 * @param sync          Type of synchronization to use (must be sig_done)
 * @param sig_ptr       Signal to raise upon completion
 *
 * Same as the sub32_imm instruction but it also returns the premodified
 * memory values in the read transfer register.
 * @see cmd_mem_sub32_imm_ptr32()
 *
 */

__intrinsic
void cmd_mem_test_and_sub32_imm_ptr40(
    int32_t data,
    __xread void *rd_back,
    volatile void __addr40 __mem *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
    );

/** Memory sub64 immediate.
 *
 * @param data          Immediate data to subtract
 * @param address       32-bit memory address at which operation should be performed
 * @param count         Number of 64-bit words to subtract
 *
 * @note Valid range for data is between -0x80 and 0x7f for 32xx indirect format and
 *       between -0x8000 and 0x7fff for 6xxx indirect format.
 *
 * Same as the sub64 instruction but the data to be subtracted from memory is
 * specified as the immediate data.
 *
 */
__intrinsic
void cmd_mem_sub64_imm_ptr32(
    int32_t data,
    volatile void __addr32 __mem *address,
    uint32_t count
    );

/** Memory decrement64.
 *
 * @param address       32-bit memory address at which operation should be performed
 *
 * Same as sub64_imm_ptr32(1,address,1) without using a signal or transfer register.
 *
 * @note: Calling this function at a high rate can stall the ME
 *
 */
__intrinsic
void cmd_mem_decr64_ptr32(
    volatile void __addr32 __mem *address     /* address to decrement */
    );

/** Memory sub64 immediate.
 *
 * @param data          Immediate data to subtract
 * @param address       40-bit memory address at which operation should be performed
 * @param count         Number of 64-bit words to subtract
 *
 * @note Valid range for data is between -0x80 and 0x7f for 32xx indirect format and
 *       between -0x8000 and 0x7fff for 6xxx indirect format.
 *
 * Same as the sub64 instruction but the data to be subtracted from memory is
 * specified as the immediate data.
 *
 */

__intrinsic
void cmd_mem_sub64_imm_ptr40(
    int32_t data,
    volatile void __addr40 __mem *address,
    uint32_t count
    );

/** Memory decrement64.
 *
 * @param address       32-bit memory address at which operation should be performed
 *
 * Same as sub64_imm_ptr32(1,address,1) without using a signal or transfer register.
 *
 * @note: Calling this function at a high rate can stall the ME
 *
 */

__intrinsic
void cmd_mem_decr64_ptr40(
    volatile void __addr40 __mem *address     /* address to decrement */
    );


/** Memory test and sub64 immediate.
 *
 * @param data          Immediate data to subtract (-0x80 - 0x7f)
 * @param rd_back       Xfer register to read back
 * @param address       32-bit memory address at which operation should be performed
 * @param count         Number of 64-bit words to subtract
 * @param sync          Type of synchronization to use (must be sig_done)
 * @param sig_ptr       Signal to raise upon completion
 *
 * Same as the sub64_imm instruction but it also returns the premodified
 * memory values in the read transfer register.
 * @see cmd_mem_sub64_imm_ptr32()
 *
 */
__intrinsic
void cmd_mem_test_and_sub64_imm_ptr32(
    int32_t data,
    __xread void *rd_back,
    volatile void __addr32 __mem *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
    );


/** Memory test and sub64 immediate.
 *
 * @param data          Immediate data to subtract (-0x80 - 0x7f)
 * @param rd_back       Xfer register to read back
 * @param address       40-bit memory address at which operation should be performed
 * @param count         Number of 64-bit words to subtract
 * @param sync          Type of synchronization to use (must be sig_done)
 * @param sig_ptr       Signal to raise upon completion
 *
 * Same as the sub64_imm instruction but it also returns the premodified
 * memory values in the read transfer register.
 * @see cmd_mem_sub64_imm_ptr32()
 *
 */

__intrinsic
void cmd_mem_test_and_sub64_imm_ptr40(
    int32_t data,
    __xread void *rd_back,
    volatile void __addr40 __mem *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
    );

/** Memory sub32 with saturation.
 *
 * @param data          Data to subtract
 * @param address       32-bit memory address at which operation should be performed
 * @param count         Number of 32-bit words to subtract (1-4)
 * @param sync          Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr       Signal to raise upon completion
 *
 * Subtract the signed value in the transfer registers from the data at the address.
 * Subtraction with results of < 0 will saturate the result in the
 * value in memory at 0x0000 0000.
 *
 */
__intrinsic
void cmd_mem_sub32_sat_ptr32(
    __xwrite void *data,
    volatile void __addr32 __mem *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
    );


/** Memory sub32 with saturation.
 *
 * @param data          Data to subtract
 * @param address       40-bit memory address at which operation should be performed
 * @param count         Number of 32-bit words to subtract (1-4)
 * @param sync          Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr       Signal to raise upon completion
 *
 * Subtract the signed value in the transfer registers from the data at the address.
 * Subtraction with results of < 0 will saturate the result in the
 * value in memory at 0x0000 0000.
 *
 */

__intrinsic
void cmd_mem_sub32_sat_ptr40(
    __xwrite void *data,
    volatile void __addr40 __mem *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
    );

/** Memory sub32 with saturation in indirect mode.
 *
 * @param data          Data to subtract
 * @param address       Memory address at which operation should be performed
 * @param max_nn        Max number of 32-bit words to subtract (1-4)
 * @param ind           Indirect word
 * @param sync          Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr       Signal to raise upon completion
 *
 * @see cmd_mem_sub32_sat_ptr40()
 *
 */
__intrinsic
void cmd_mem_sub32_sat_ind_ptr32(
    __xwrite void *data,
    volatile void __addr32 __mem *address,
    uint32_t max_nn,
    generic_ind_t ind,
    sync_t sync,
    SIGNAL *sig_ptr
    );


/** Memory sub32 with saturation in indirect mode.
 *
 * @param data          Data to subtract
 * @param address       40-bit memory address at which operation should be performed
 * @param max_nn        Max number of 32-bit words to subtract (1-4)
 * @param ind           Indirect word
 * @param sync          Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr       Signal to raise upon completion
 *
 * @see cmd_mem_sub32_sat_ptr40()
 *
 */

__intrinsic
void cmd_mem_sub32_sat_ind_ptr40(
    __xwrite void *data,
    volatile void __addr40 __mem *address,
    uint32_t max_nn,
    generic_ind_t ind,
    sync_t sync,
    SIGNAL *sig_ptr
    );

/** Memory sub64 with saturation.
 *
 * @param data          Data to subtract
 * @param address       32-bit memory address at which operation should be performed
 * @param count         Number of 64-bit words to subtract (1-4)
 * @param sync          Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr       Signal to raise upon completion
 *
 * Subtract the signed 64-bit value in the transfer registers from the data at the address.
 * Subtraction with results of < 0 will saturate the result
 * in the value in memory at 0x0000 0000.
 *
 */
__intrinsic
void cmd_mem_sub64_sat_ptr32(
    __xwrite void *data,
    volatile void __addr32 __mem *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
    );


/** Memory sub64 with saturation.
 *
 * @param data          Data to subtract
 * @param address       40-bit memory address at which operation should be performed
 * @param count         Number of 64-bit words to subtract (1-4)
 * @param sync          Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr       Signal to raise upon completion
 *
 * Subtract the signed 64-bit value in the transfer registers from the data at the address.
 * Subtraction with results of < 0 will saturate the result
 * in the value in memory at 0x0000 0000.
 *
 */

__intrinsic
void cmd_mem_sub64_sat_ptr40(
    __xwrite void *data,
    volatile void __addr40 __mem *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
    );

/** Memory sub64 with saturation in indirect mode.
 *
 * @param data          Data to subtract
 * @param address       Memory address at which operation should be performed
 * @param max_nn        Max number of 64-bit words to subtract (1-4)
 * @param ind           Indirect word
 * @param sync          Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr       Signal to raise upon completion
 *
 * Note that length[2:2] in the indirect word has to be set to 0b1 for this 64-bit command.
 * See 6xxx databook for more information.
 *
 * @see cmd_mem_sub64_sat_ptr40()
 *
 */
__intrinsic
void cmd_mem_sub64_sat_ind_ptr32(
    __xwrite void *data,
    volatile void __addr32 __mem *address,
    uint32_t max_nn,
    generic_ind_t ind,
    sync_t sync,
    SIGNAL *sig_ptr
    );


/** Memory sub64 with saturation in indirect mode.
 *
 * @param data          Data to subtract
 * @param address       40-bit memory address at which operation should be performed
 * @param max_nn        Max number of 64-bit words to subtract (1-4)
 * @param ind           Indirect word
 * @param sync          Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr       Signal to raise upon completion
 *
 * Note that length[2:2] in the indirect word has to be set to 0b1 for this 64-bit command.
 * See 6xxx databook for more information.
 *
 * @see cmd_mem_sub64_sat_ptr40()
 *
 */

__intrinsic
void cmd_mem_sub64_sat_ind_ptr40(
    __xwrite void *data,
    volatile void __addr40 __mem *address,
    uint32_t max_nn,
    generic_ind_t ind,
    sync_t sync,
    SIGNAL *sig_ptr
    );

/** Memory test and sub32 with saturation.
 *
 * @param val           Returned pre-modified value
 * @param data          Data to subtract
 * @param address       32-bit memory address at which operation should be performed
 * @param count         Number of 32-bit words to subtract (1-4)
 * @param sync          Type of synchronization to use (must be sig_done)
 * @param sig_pair_ptr  Signal pair to raise upon completion
 *
 * Same as the sub32_sat instruction but it also returns the premodified
 * memory values in the read transfer registers.
 * @see cmd_mem_sub32_sat_ptr32()
 *
 */
__intrinsic
void cmd_mem_test_and_sub32_sat_ptr32(
    __xread uint32_t *val,
    __xwrite void *data,
    volatile void __addr32 __mem *address,
    uint32_t count,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
    );


/** Memory test and sub32 with saturation.
 *
 * @param val           Returned pre-modified value
 * @param data          Data to subtract
 * @param address       40-bit memory address at which operation should be performed
 * @param count         Number of 32-bit words to subtract (1-4)
 * @param sync          Type of synchronization to use (must be sig_done)
 * @param sig_pair_ptr  Signal pair to raise upon completion
 *
 * Same as the sub32_sat instruction but it also returns the premodified
 * memory values in the read transfer registers.
 * @see cmd_mem_sub32_sat_ptr32()
 *
 */

__intrinsic
void cmd_mem_test_and_sub32_sat_ptr40(
    __xread uint32_t *val,
    __xwrite void *data,
    volatile void __addr40 __mem *address,
    uint32_t count,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
    );

/** Memory test and sub32 with saturation in indirect mode.
 *
 * @param val           Returned pre-modified value
 * @param data          Data to subtract
 * @param address       Memory address at which operation should be performed
 * @param max_nn        Max number of 32-bit words to subtract (1-4)
 * @param ind           Indirect word
 * @param sync          Type of synchronization to use (must be sig_done)
 * @param sig_pair_ptr  Signal pair to raise upon completion
 *
 * @see cmd_mem_test_and_sub32_sat_ptr40()
 *
 */
__intrinsic
void cmd_mem_test_and_sub32_sat_ind_ptr32(
    __xread uint32_t *val,
    __xwrite void *data,
    volatile void __addr32 __mem *address,
    uint32_t max_nn,
    generic_ind_t ind,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
    );


/** Memory test and sub32 with saturation in indirect mode.
 *
 * @param val           Returned pre-modified value
 * @param data          Data to subtract
 * @param address       40-bit memory address at which operation should be performed
 * @param max_nn        Max number of 32-bit words to subtract (1-4)
 * @param ind           Indirect word
 * @param sync          Type of synchronization to use (must be sig_done)
 * @param sig_pair_ptr  Signal pair to raise upon completion
 *
 * @see cmd_mem_test_and_sub32_sat_ptr40()
 *
 */

__intrinsic
void cmd_mem_test_and_sub32_sat_ind_ptr40(
    __xread uint32_t *val,
    __xwrite void *data,
    volatile void __addr40 __mem *address,
    uint32_t max_nn,
    generic_ind_t ind,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
    );

/** Memory test and sub64 with saturation.
 *
 * @param val           Returned pre-modified value
 * @param data          Data to subtract
 * @param address       32-bit memory address at which operation should be performed
 * @param count         Number of 64-bit words to subtract (1-4)
 * @param sync          Type of synchronization to use (must be sig_done)
 * @param sig_pair_ptr  Signal pair to raise upon completion
 *
 * Same as the subsat64 instruction but it also returns the premodified
 * memory values in the read transfer registers.
 * @see cmd_mem_sub64_sat_ptr32()
 *
 */
__intrinsic
void cmd_mem_test_and_sub64_sat_ptr32(
    __xread uint32_t *val,
    __xwrite void *data,
    volatile void __addr32 __mem *address,
    uint32_t count,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
    );


/** Memory test and sub64 with saturation.
 *
 * @param val           Returned pre-modified value
 * @param data          Data to subtract
 * @param address       40-bit memory address at which operation should be performed
 * @param count         Number of 64-bit words to subtract (1-4)
 * @param sync          Type of synchronization to use (must be sig_done)
 * @param sig_pair_ptr  Signal pair to raise upon completion
 *
 * Same as the subsat64 instruction but it also returns the premodified
 * memory values in the read transfer registers.
 * @see cmd_mem_sub64_sat_ptr32()
 *
 */

__intrinsic
void cmd_mem_test_and_sub64_sat_ptr40(
    __xread uint32_t *val,
    __xwrite void *data,
    volatile void __addr40 __mem *address,
    uint32_t count,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
    );

/** Memory test and sub64 with saturation in indirect mode.
 *
 * @param val           Returned pre-modified value
 * @param data          Data to subtract
 * @param address       Memory address at which operation should be performed
 * @param max_nn        Max number of 64-bit words to subtract (1-4)
 * @param ind           Indirect word
 * @param sync          Type of synchronization to use (must be sig_done)
 * @param sig_pair_ptr  Signal pair to raise upon completion
 *
 * Note that length[2:2] in the indirect word has to be set to 0b1 for this 64-bit command.
 * See 6xxx databook for more information.
 *
 * @see cmd_mem_test_and_sub64_sat_ptr40()
 *
 */
__intrinsic
void cmd_mem_test_and_sub64_sat_ind_ptr32(
    __xread uint32_t *val,
    __xwrite void *data,
    volatile void __addr32 __mem *address,
    uint32_t max_nn,
    generic_ind_t ind,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
    );


/** Memory test and sub64 with saturation in indirect mode.
 *
 * @param val           Returned pre-modified value
 * @param data          Data to subtract
 * @param address       40-bit memory address at which operation should be performed
 * @param max_nn        Max number of 64-bit words to subtract (1-4)
 * @param ind           Indirect word
 * @param sync          Type of synchronization to use (must be sig_done)
 * @param sig_pair_ptr  Signal pair to raise upon completion
 *
 * Note that length[2:2] in the indirect word has to be set to 0b1 for this 64-bit command.
 * See 6xxx databook for more information.
 *
 * @see cmd_mem_test_and_sub64_sat_ptr40()
 *
 */

__intrinsic
void cmd_mem_test_and_sub64_sat_ind_ptr40(
    __xread uint32_t *val,
    __xwrite void *data,
    volatile void __addr40 __mem *address,
    uint32_t max_nn,
    generic_ind_t ind,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
    );

/** Memory sub32 immediate with saturation.
 *
 * @param data          Immediate data to subtract
 * @param address       32-bit memory address at which operation should be performed
 * @param count         Number of 32-bit words to subtract
 *
 * Same as the sub32_sat instruction, but the data to be subtracted from memory is specified as
 * the immediate data and the operation is applied only to a single 32-bit memory word.
 * @see cmd_mem_sub32_sat_ptr32()
 *
 */
__intrinsic
void cmd_mem_sub32_imm_sat_ptr32(
    int32_t data,
    volatile void __addr32 __mem *address,
    uint32_t count
    );


/** Memory sub32 immediate with saturation.
 *
 * @param data          Immediate data to subtract
 * @param address       40-bit memory address at which operation should be performed
 * @param count         Number of 32-bit words to subtract
 *
 * Same as the sub32_sat instruction, but the data to be subtracted from memory is specified as
 * the immediate data and the operation is applied only to a single 32-bit memory word.
 * @see cmd_mem_sub32_sat_ptr32()
 *
 */

__intrinsic
void cmd_mem_sub32_imm_sat_ptr40(
    int32_t data,
    volatile void __addr40 __mem *address,
    uint32_t count
    );

/** Memory test and sub32 immediate with saturation.
 *
 * @param data          Immediate data to subtract (-0x80 - 0x7f)
 * @param rd_back       Xfer register to read back
 * @param address       32-bit memory address at which operation should be performed
 * @param count         Number of 32-bit words to subtract
 * @param sync          Type of synchronization to use (must be sig_done)
 * @param sig_ptr       Signal to raise upon completion
 *
 * Same as the sub32_imm_sat instruction but it also returns the premodified
 * memory values in the read transfer register.
 * @see cmd_mem_sub32_imm_sat_ptr32()
 *
 */
__intrinsic
void cmd_mem_test_and_sub32_imm_sat_ptr32(
    int32_t data,
    __xread void *rd_back,
    volatile void __addr32 __mem *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
    );

/** Memory test and sub32 immediate with saturation.
 *
 * @param data          Immediate data to subtract (-0x80 - 0x7f)
 * @param rd_back       Xfer register to read back
 * @param address       40-bit memory address at which operation should be performed
 * @param count         Number of 32-bit words to subtract
 * @param sync          Type of synchronization to use (must be sig_done)
 * @param sig_ptr       Signal to raise upon completion
 *
 * Same as the sub32_imm_sat instruction but it also returns the premodified
 * memory values in the read transfer register.
 * @see cmd_mem_sub32_imm_sat_ptr32()
 *
 */

__intrinsic
void cmd_mem_test_and_sub32_imm_sat_ptr40(
    int32_t data,
    __xread void *rd_back,
    volatile void __addr40 __mem *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
    );

/** Memory sub64 immediate with saturation.
 *
 * @param data          Immediate data to subtract
 * @param address       32-bit memory address at which operation should be performed
 * @param count         Number of 64-bit words to subtract
 *
 * @note Valid range for data is between -0x80 and 0x7f for 32xx indirect format and
 *       between -0x8000 and 0x7fff for 6xxx indirect format.
 *
 * Same as the subsat64 instruction but the data to be subtracted from memory
 * is specified as the immediate data.
 * @see cmd_mem_sub64_sat_ptr32()
 *
 */
__intrinsic
void cmd_mem_sub64_imm_sat_ptr32(
    int32_t data,
    volatile void __addr32 __mem *address,
    uint32_t count
    );

/** Memory sub64 immediate with saturation.
 *
 * @param data          Immediate data to subtract
 * @param address       40-bit memory address at which operation should be performed
 * @param count         Number of 64-bit words to subtract
 *
 * @note Valid range for data is between -0x80 and 0x7f for 32xx indirect format and
 *       between -0x8000 and 0x7fff for 6xxx indirect format.
 *
 * Same as the subsat64 instruction but the data to be subtracted from memory
 * is specified as the immediate data.
 * @see cmd_mem_sub64_sat_ptr32()
 *
 */

__intrinsic
void cmd_mem_sub64_imm_sat_ptr40(
    int32_t data,
    volatile void __addr40 __mem *address,
    uint32_t count
    );

/** Memory test and sub64 immediate with saturation.
 *
 * @param data          Immediate data to subtract (-0x80 - 0x7f)
 * @param rd_back       Xfer register to read back
 * @param address       32-bit memory address at which operation should be performed
 * @param count         Number of 64-bit words to subtract
 * @param sync          Type of synchronization to use (must be sig_done)
 * @param sig_ptr       Signal to raise upon completion
 *
 * Same as the subsat64_imm instruction but it also returns the
 * premodified memory values in the read transfer register.
 * @see cmd_mem_sub64_imm_sat_ptr32()
 *
 */
__intrinsic
void cmd_mem_test_and_sub64_imm_sat_ptr32(
    int32_t data,
    __xread void *rd_back,
    volatile void __addr32 __mem *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
    );


/** Memory test and sub64 immediate with saturation.
 *
 * @param data          Immediate data to subtract (-0x80 - 0x7f)
 * @param rd_back       Xfer register to read back
 * @param address       40-bit memory address at which operation should be performed
 * @param count         Number of 64-bit words to subtract
 * @param sync          Type of synchronization to use (must be sig_done)
 * @param sig_ptr       Signal to raise upon completion
 *
 * Same as the subsat64_imm instruction but it also returns the
 * premodified memory values in the read transfer register.
 * @see cmd_mem_sub64_imm_sat_ptr32()
 *
 */

__intrinsic
void cmd_mem_test_and_sub64_imm_sat_ptr40(
    int32_t data,
    __xread void *rd_back,
    volatile void __addr40 __mem *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
    );

/** XOR in Memory from transfer registers.
 *
 * @param data     Data (byte mask) to xor in Memory
 * @param address 32-bit memory address to xor to given data
 * @param count    Number of 32-bit words to XOR (1-8)
 * @param sync     Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr  Signal to raise upon completion
 *
 * Do an exclusive OR operation between write transfer registers and a number of 32-bit memory words.
 * A byte mask specifies which memory bytes are affected for all memory words.
 *
 */
__intrinsic
void cmd_mem_xor_ptr32(
    __xwrite void *data,
    volatile void __addr32 __mem *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
    );


/** XOR in Memory from transfer registers.
 *
 * @param data     Data (byte mask) to xor in Memory
 * @param address  40-bit memory address to xor to given data
 * @param count    Number of 32-bit words to XOR (1-8)
 * @param sync     Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr  Signal to raise upon completion
 *
 * Do an exclusive OR operation between write transfer registers and a number of 32-bit memory words.
 * A byte mask specifies which memory bytes are affected for all memory words.
 *
 */

__intrinsic
void cmd_mem_xor_ptr40(
    __xwrite void *data,
    volatile void __addr40 __mem *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
    );

/** XOR in Memory from transfer registers in indirect mode.
 *
 * @param data      Data (byte mask) to xor in Memory
 * @param address   Memory address to xor to given data
 * @param max_nn    Max number of 32-bit words to XOR (1-8)
 * @param ind       Indirect word
 * @param sync      Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr   Signal to raise upon completion
 *
 * @see cmd_mem_xor_ptr32()
 *
 */
__intrinsic
void cmd_mem_xor_ind_ptr32(
    __xwrite void *data,
    volatile void __addr32 __mem *address,
    uint32_t max_nn,
    generic_ind_t ind,
    sync_t sync,
    SIGNAL *sig_ptr
    );


/** XOR in Memory from transfer registers in indirect mode.
 *
 * @param data      Data (byte mask) to xor in Memory
 * @param address   40-bit memory address to xor to given data
 * @param max_nn    Max number of 32-bit words to XOR (1-8)
 * @param ind       Indirect word
 * @param sync      Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr   Signal to raise upon completion
 *
 * @see cmd_mem_xor_ptr32()
 *
 */

__intrinsic
void cmd_mem_xor_ind_ptr40(
    __xwrite void *data,
    volatile void __addr40 __mem *address,
    uint32_t max_nn,
    generic_ind_t ind,
    sync_t sync,
    SIGNAL *sig_ptr
    );

/** XOR data in Memory from transfer registers and return pre-modified value
 * in transfer register.
 *
 * @param val           Returned pre-modified value
 * @param data          Data (byte mask) to xor in Memory
 * @param address       32-bit memory address at which operation should be performed
 * @param count         Number of 32-bit words to XOR (1-8)
 * @param sync          Type of synchronization to use (must be sig_done)
 * @param sig_pair_ptr  Signal pair to raise upon completion
 *
 * Same as the xor instruction but it also returns the premodified memory
 * values to the read transfer registers.
 * @see cmd_mem_xor_ptr32()
 *
 */
__intrinsic
void cmd_mem_test_and_xor_ptr32(
    __xread uint32_t *val,
    __xwrite void *data,
    volatile void __addr32 __mem *address,
    uint32_t count,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
    );


/** XOR data in Memory from transfer registers and return pre-modified value
 * in transfer register.
 *
 * @param val           Returned pre-modified value
 * @param data          Data (byte mask) to xor in Memory
 * @param address       40-bit memory address at which operation should be performed
 * @param count         Number of 32-bit words to XOR (1-8)
 * @param sync          Type of synchronization to use (must be sig_done)
 * @param sig_pair_ptr  Signal pair to raise upon completion
 *
 * Same as the xor instruction but it also returns the premodified memory
 * values to the read transfer registers.
 * @see cmd_mem_xor_ptr32()
 *
 */

__intrinsic
void cmd_mem_test_and_xor_ptr40(
    __xread uint32_t *val,
    __xwrite void *data,
    volatile void __addr40 __mem *address,
    uint32_t count,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
    );

/** XOR data into Memory from transfer registers and return pre-modified value
 * in transfer register in indirect mode.
 *
 * @param val           Returned pre-modified value
 * @param data          Data to XOR into Memory
 * @param address       Memory address at which operation should be performed
 * @param max_nn        Max number of 32-bit words to XOR (1-8)
 * @param ind           Indirect word
 * @param sync          Type of synchronization to use (must be sig_done)
 * @param sig_pair_ptr  Signal pair to raise upon completion
 *
 * @see cmd_mem_test_and_xor_ptr40()
 *
 */
__intrinsic
void cmd_mem_test_and_xor_ind_ptr32(
    __xread uint32_t *val,
    __xwrite void *data,
    volatile void __addr32 __mem *address,
    uint32_t max_nn,
    generic_ind_t ind,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
    );


/** XOR data into Memory from transfer registers and return pre-modified value
 * in transfer register in indirect mode.
 *
 * @param val           Returned pre-modified value
 * @param data          Data to XOR into Memory
 * @param address       40-bit memory address at which operation should be performed
 * @param max_nn        Max number of 32-bit words to XOR (1-8)
 * @param ind           Indirect word
 * @param sync          Type of synchronization to use (must be sig_done)
 * @param sig_pair_ptr  Signal pair to raise upon completion
 *
 * @see cmd_mem_test_and_xor_ptr40()
 *
 */

__intrinsic
void cmd_mem_test_and_xor_ind_ptr40(
    __xread uint32_t *val,
    __xwrite void *data,
    volatile void __addr40 __mem *address,
    uint32_t max_nn,
    generic_ind_t ind,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
    );

/** Memory XOR immediate.
 *
 * @param data     Immediate data to xor in Memory (0x0 - 0x7f)
 * @param address  32-bit memory address to xor to given data
 * @param count    Number of 32-bit words to set (1-8)
 *
 * Same as the xor instruction but the bit mask is specified as the immediate data.
 * @see cmd_mem_xor_ptr32()
 *
 */
__intrinsic
void cmd_mem_xor_imm_ptr32(
    uint32_t data,
    volatile void __addr32 __mem *address,
    uint32_t count
    );


/** Memory XOR immediate.
 *
 * @param data     Immediate data to xor in Memory (0x0 - 0x7f)
 * @param address  40-bit memory address to xor to given data
 * @param count    Number of 32-bit words to set (1-8)
 *
 * Same as the xor instruction but the bit mask is specified as the immediate data.
 * @see cmd_mem_xor_ptr32()
 *
 */

__intrinsic
void cmd_mem_xor_imm_ptr40(
    uint32_t data,
    volatile void __addr40 __mem *address,
    uint32_t count
    );

/** Memory test and xor immediate.
 *
 * @param data          Immediate data to xor in Memory (0x0 - 0x7f)
 * @param rd_back       Xfer register to read back
 * @param address       32-bit memory address at which operation should be performed
 * @param count         Number of 32-bit words to xor (1-8)
 * @param sync          Type of synchronization to use (must be sig_done)
 * @param sig_ptr       Signal to raise upon completion
 *
 * Same as the xor_imm instruction but it also returns the premodified
 * memory values in the read transfer registers.
 * @see cmd_mem_xor_imm_ptr32()
 *
 */
__intrinsic
void cmd_mem_test_and_xor_imm_ptr32(
    uint32_t data,
    __xread void *rd_back,
    volatile void __addr32 __mem *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
    );

/** Memory test and xor immediate.
 *
 * @param data          Immediate data to xor in Memory (0x0 - 0x7f)
 * @param rd_back       Xfer register to read back
 * @param address       40-bit memory address at which operation should be performed
 * @param count         Number of 32-bit words to xor (1-8)
 * @param sync          Type of synchronization to use (must be sig_done)
 * @param sig_ptr       Signal to raise upon completion
 *
 * Same as the xor_imm instruction but it also returns the premodified
 * memory values in the read transfer registers.
 * @see cmd_mem_xor_imm_ptr32()
 *
 */

__intrinsic
void cmd_mem_test_and_xor_imm_ptr40(
    uint32_t data,
    __xread void *rd_back,
    volatile void __addr40 __mem *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
    );

/** Memory mask compare write
 *
 * @param data     Xfer register(s) containing data to write to memory
 * @param address  32-bit memory address at which operation should be performed
 * @param mask     4-bit mask of which 4 bytes to compare
 * @param count    Number of 32-bit words to write (1-8)
 * @param sync     Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr  Signal to raise upon completion
 *
 * This command takes memory data (32-bits at a time) and data in xfer registers and it compares
 * the four bytes for equality. If the bytes in the memory and data indicated by the byte mask are equal,
 * then the whole data word overwrites the memory.
 * For example, if the byte mask is 4b1000, then the memory is overwritten ONLY if the bits [8;24] of the
 * memory and the pull data are equal. If the byte mask were 4b0110, then the middle two bytes (i.e. bits [16;8]) of
 * the memory and pull data would have to be equal for the memory to be overwritten.
 *
 */
__intrinsic
void cmd_mem_mask_compare_write_ptr32(
    __xwrite void *data,
    volatile void __addr32 __mem *address,
    uint32_t mask,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
    );

/** Memory mask compare write
 *
 * @param data     Xfer register(s) containing data to write to memory
 * @param address  40 bit memory address at which operation should be performed
 * @param mask     4-bit mask of which 4 bytes to compare, 0x0 and 0xf is not allowed
 * @param count    Number of 32-bit words to write (1-8)
 * @param sync     Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr  Signal to raise upon completion
 *
 * This command takes memory data (32-bits at a time) and data in xfer registers and it compares
 * the four bytes for equality. If the bytes in the memory and data indicated by the byte mask are equal,
 * then the whole data word overwrites the memory.
 * For example, if the byte mask is 4b1000, then the memory is overwritten ONLY if the bits [8;24] of the
 * memory and the pull data are equal. If the byte mask were 4b0110, then the middle two bytes (i.e. bits [16;8]) of
 * the memory and pull data would have to be equal for the memory to be overwritten.
 *
 */

__intrinsic
void cmd_mem_mask_compare_write_ptr40(
    __xwrite void *data,
    volatile void __addr40 __mem *address,
    uint32_t mask,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
    );

/**  Memory compare write using mask to compare.
 *
 * @param val      Returned pre-modified value
 * @param data     Xfer register(s) containing data to write to memory
 * @param address  32-bit memory address at which operation should be performed
 * @param mask     4-bit mask of which 4 bytes to compare, 0x0 and 0xf is not allowed
 * @param count    Number of 32-bit words to write (1-8)
 * @param sync     Type of synchronization to use (must be sig_done)
 * @param sig_pair_ptr  Signal pair to raise upon completion
 *
 * Same as the cmd_mem_mask_compare_write_ptr40 instruction but it also returns a pre-modified
 * 32-bit word of the first memory location.
 * @see cmd_mem_mask_compare_write_ptr40()
 *
 */
__intrinsic
void cmd_mem_test_mask_and_compare_write_ptr32(
    __xread void *val,
    __xwrite void *data,
    volatile void __addr32 __mem *address,
    uint32_t mask,
    uint32_t count,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
    );

/** Memory compare write using mask to compare.
 *
 * @param val      Returned pre-modified value
 * @param data     Xfer register(s) containing data to write to memory
 * @param address  40-bit memory address at which operation should be performed
 * @param mask     4-bit mask of which 4 bytes to compare, 0x0 and 0xf is not allowed
 * @param count    Number of 32-bit words to write (1-8)
 * @param sync     Type of synchronization to use (must be sig_done)
 * @param sig_pair_ptr  Signal pair to raise upon completion
 *
 * Same as the cmd_mem_mask_compare_write_ptr40 instruction but it also returns a pre-modified
 * 32-bit word of the first memory location.
 * @see cmd_mem_mask_compare_write_ptr40()
 *
 */

__intrinsic
void cmd_mem_test_mask_and_compare_write_ptr40(
    __xread void *val,
    __xwrite void *data,
    volatile void __addr40 __mem *address,
    uint32_t mask,
    uint32_t count,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
    );

/** Write data to memory from transfer registers.
 *
 * @param data          Xfer register(s) containing data to write to memory
 * @param address       32-bit memory address at which operation should be performed
 * @param count         Number of 32-bit words to write (1-32)
 * @param sync          Type of synchronization to use  (sig_done or ctx_swap)
 * @param sig_ptr       Signal to raise upon completion
 */
__intrinsic
void cmd_mem_write32_ptr32(
    __xwrite void *data,
    volatile void __addr32 __mem *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
    );

/** Write data to memory from transfer registers.
 *
 * @param data          Xfer register(s) containing data to write to memory
 * @param address       40-bit memory address at which operation should be performed
 * @param count         Number of 32bit words to write (1-32)
 * @param sync          Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr       Signal to raise upon completion
 *
 */

__intrinsic
void cmd_mem_write32_ptr40(
    __xwrite void *data,
    volatile void __addr40 __mem *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
    );

/** Write 32-bit data to memory from transfer register with indirect mode and ptr32 addressing.
 *
 * @param data          Xfer register(s) containing data to write to memory
 * @param address       Memory address at which operation should be performed
 * @param max_nn        Number of 32bit words to write (1-32)
 * @param ind           Indirect word
 * @param sync          Type of synchronization to use (must be sig_done)
 * @param sig_ptr       Signal pair to raise upon completion
 *
 * @see cmd_mem_write32_ptr32()
 *
 */
__intrinsic
void cmd_mem_write32_ind_ptr32(
    __xwrite void *data,
    volatile void __addr32 __mem *address,
    uint32_t max_nn,
    generic_ind_t ind,
    sync_t sync,
    SIGNAL *sig_ptr
    );


/** Write 32-bit data to memory from transfer register with indirect mode and ptr40 addressing
 *
 * @param data          Xfer register(s) containing data to write to memory
 * @param address       40-bit memory address at which operation should be performed
 * @param max_nn        Number of 32bit words to write (1-32)
 * @param ind           Indirect word
 * @param sync          Type of synchronization to use (must be sig_done)
 * @param sig_ptr       Signal pair to raise upon completion
 *
 * @see cmd_mem_write32_ptr40()
 *
 */

__intrinsic
 void cmd_mem_write32_ind_ptr40(
    __xwrite void *data,
    volatile void __addr40 __mem *address,
    uint32_t max_nn,
    generic_ind_t ind,
    sync_t sync,
    SIGNAL *sig_ptr
    );

/** Write 32-bit data to memory from transfer register with byte swapping, indirect mode and ptr32 addressing.
 *
 * @param data          Xfer register(s) containing data to write to memory
 * @param address       Memory address at which operation should be performed
 * @param max_nn        Number of 32bit words to write (1-32)
 * @param ind           Indirect word
 * @param sync          Type of synchronization to use (must be sig_done)
 * @param sig_ptr       Signal pair to raise upon completion
 *
 * @see cmd_mem_write32_swap_ptr32()
 *
 */
__intrinsic
void cmd_mem_write32_swap_ind_ptr32(
    __xwrite void *data,
    volatile void __addr32 __mem *address,
    uint32_t max_nn,
    generic_ind_t ind,
    sync_t sync,
    SIGNAL *sig_ptr
    );


/** Write 32-bit data to memory from transfer register with byte swapping, indirect mode and ptr40 addressing
 *
 * @param data          Xfer register(s) containing data to write to memory
 * @param address       40-bit memory address at which operation should be performed
 * @param max_nn        Number of 32bit words to write (1-32)
 * @param ind           Indirect word
 * @param sync          Type of synchronization to use (must be sig_done)
 * @param sig_ptr       Signal pair to raise upon completion
 *
 * @see cmd_mem_write32_swap_ptr40()
 *
 */

__intrinsic
void cmd_mem_write32_swap_ind_ptr40(
    __xwrite void *data,
    volatile void __addr40 __mem *address,
    uint32_t max_nn,
    generic_ind_t ind,
    sync_t sync,
    SIGNAL *sig_ptr
    );

/** Write data to memory from transfer registers (little-endian).
 *
 * @param data          Xfer register(s) containing data to write to memory
 * @param address       32-bit memory address at which operation should be performed
 * @param count         Number of 32-bit words to write (1-32)
 * @param sync          Type of synchronization to use  (sig_done or ctx_swap)
 * @param sig_ptr       Signal to raise upon completion
 */
__intrinsic
void cmd_mem_write32_le_ptr32(
    __xwrite void *data,
    volatile void __addr32 __mem *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
    );

/** Write data to memory from transfer registers (little-endian).
 *
 * @param data          Xfer register(s) containing data to write to memory
 * @param address       40-bit memory address at which operation should be performed
 * @param count         Number of 32-bit words to write (1-32)
 * @param sync          Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr       Signal to raise upon completion
 *
 */

__intrinsic
void cmd_mem_write32_le_ptr40(
    __xwrite void *data,
    volatile void __addr40 __mem *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
    );

/** Read data from memory into transfer register.
 *
 * @param data          Xfer register(s) to store data read from memory
 * @param address       32-bit memory address to read from
 * @param count         Number of 32-bit words to read (1-32)
 * @param sync          Type of synchronization to use  (sig_done or ctx_swap)
 * @param sig_ptr       Signal to raise upon completion
 */
__intrinsic
void cmd_mem_read32_ptr32(
    __xread void *data,
    volatile void __addr32 __mem *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
    );

/** Read data from memory into transfer register.
 *
 * @param data          Xfer register(s) to store data read from memory
 * @param address       40-bit memory address to read from
 * @param count         Number of 32-bit words to read (1-32)
 * @param sync          Type of synchronization to use  (sig_done or ctx_swap)
 * @param sig_ptr       Signal to raise upon completion
 *
 */

__intrinsic
void cmd_mem_read32_ptr40(
    __xread void *data,
    volatile void __addr40 __mem *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
    );

/** Read 32-bit data from memory into transfer register with indirect mode and ptr32 addressing.
 *
 * @param data          Xfer register(s) to store data read from memory
 * @param address       32-bit memory address to read from
 * @param max_nn        Number of 32-bit words to read (1-32)
 * @param ind           Indirect word
 * @param sync          Type of synchronization to use  (sig_done or ctx_swap)
 * @param sig_ptr       Signal pair to raise upon completion
 *
 * @see cmd_mem_read32_ptr32()
 *
 */
__intrinsic
 void cmd_mem_read32_ind_ptr32(
    __xread void *data,
    volatile void __addr32 __mem *address,
    uint32_t max_nn,
    generic_ind_t ind,
    sync_t sync,
    SIGNAL *sig_ptr
    );


/** Read 32-bit data from memory into transfer register with indirect mode and ptr40 addressing
 *
 * @param data          Xfer register(s) to store data read from memory
 * @param address       40-bit memory address to read from
 * @param max_nn        Number of 32-bit words to read (1-32)
 * @param ind           Indirect word
 * @param sync          Type of synchronization to use  (sig_done or ctx_swap)
 * @param sig_ptr       Signal pair to raise upon completion
 *
 * @see cmd_mem_read32_ptr40()
 *
 */

__intrinsic
 void cmd_mem_read32_ind_ptr40(
    __xread void *data,
    volatile void __addr40 __mem *address,
    uint32_t max_nn,
    generic_ind_t ind,
    sync_t sync,
    SIGNAL *sig_ptr
    );

/** Read data from memory into transfer registers (little-endian).
 *
 * @param data     Xfer register(s) to store data read from memory
 * @param address 32-bit memory address to read from
 * @param count    Number of 32-bit words to read (1-32)
 * @param sync     Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr  Signal to raise upon completion
 */
__intrinsic
void cmd_mem_read32_le_ptr32(
    __xread void *data,
    volatile void __addr32 __mem *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
    );

/** Read data from memory into transfer registers (little-endian).
 *
 * @param data     Xfer register(s) to store data read from memory
 * @param address  40-bit memory address to read from
 * @param count    Number of 32-bit words to read (1-32)
 * @param sync     Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr  Signal to raise upon completion
 *
 */

__intrinsic
void cmd_mem_read32_le_ptr40(
    __xread void *data,
    volatile void __addr40 __mem *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
    );

/** Read data from memory with byte swapping into transfer registers.
 *
 * @param data     Xfer register(s) to store data read from memory
 * @param address 32-bit memory address to read from
 * @param count    Number of 32-bit words to read (1-32)
 * @param sync     Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr  Signal to raise upon completion
 */
__intrinsic
void cmd_mem_read32_swap_ptr32(
    __xread void *data,
    volatile void __addr32 __mem *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
    );

/** Read data from memory with byte swapping into transfer registers.
 *
 * @param data     Xfer register(s) to store data read from memory
 * @param address  40-bit memory address to read from
 * @param count    Number of 32-bit words to read (1-32)
 * @param sync     Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr  Signal to raise upon completion
 *
 */

__intrinsic
void cmd_mem_read32_swap_ptr40(
    __xread void *data,
    volatile void __addr40 __mem *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
    );

/** Read 32-bit data from memory into transfer register with byte swapping, indirect mode and ptr32 addressing.
 *
 * @param data          Xfer register(s) to store data read from memory
 * @param address       32-bit memory address to read from
 * @param max_nn        Number of 32-bit words to read (1-32)
 * @param ind           Indirect word
 * @param sync          Type of synchronization to use  (sig_done or ctx_swap)
 * @param sig_ptr       Signal pair to raise upon completion
 *
 * @see cmd_mem_read32_swap_ptr32()
 *
 */
__intrinsic
void cmd_mem_read32_swap_ind_ptr32(
    __xread void *data,
    volatile void __addr32 __mem *address,
    uint32_t max_nn,
    generic_ind_t ind,
    sync_t sync,
    SIGNAL *sig_ptr
    );


/** Read 32-bit data from memory into transfer register with byte swapping, indirect mode and ptr40 addressing
 *
 * @param data          Xfer register(s) to store data read from memory
 * @param address       40-bit memory address to read from
 * @param max_nn        Number of 32-bit words to read (1-32)
 * @param ind           Indirect word
 * @param sync          Type of synchronization to use  (sig_done or ctx_swap)
 * @param sig_ptr       Signal pair to raise upon completion
 *
 * @see cmd_mem_read32_swap_ptr40()
 *
 */

__intrinsic
void cmd_mem_read32_swap_ind_ptr40(
    __xread void *data,
    volatile void __addr40 __mem *address,
    uint32_t max_nn,
    generic_ind_t ind,
    sync_t sync,
    SIGNAL *sig_ptr
    );

/** Read data from memory with byte swapping into transfer registers (little-endian).
 *
 * @param data     Xfer register(s) to store data read from memory
 * @param address 32-bit memory address to read from
 * @param count    Number of 32-bit words to read (1-32)
 * @param sync     Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr  Signal to raise upon completion
 */
__intrinsic
void cmd_mem_read32_swap_le_ptr32(
    __xread void *data,
    volatile void __addr32 __mem *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
    );

/** Read data from memory with byte swapping into transfer registers (little-endian).
 *
 * @param data     Xfer register(s) to store data read from memory
 * @param address  40-bit memory address to read from
 * @param count    Number of 32-bit words to read (1-32)
 * @param sync     Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr  Signal to raise upon completion
 *
 */

__intrinsic
void cmd_mem_read32_swap_le_ptr40(
    __xread void *data,
    volatile void __addr40 __mem *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
    );

/** Write data to memory with byte swapping into transfer registers.
 *
 * @param data     Xfer register(s) containing data to write to memory
 * @param address  32-bit memory address at which operation should be performed
 * @param count    Number of 32-bit words to write (1-32)
 * @param sync     Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr  Signal to raise upon completion
 */
__intrinsic
void cmd_mem_write32_swap_ptr32(
    __xwrite void *data,
    volatile void __addr32 __mem *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
    );

/** Write data to memory with byte swapping into transfer registers.
 *
 * @param data     Xfer register(s) containing data to write to memory
 * @param address  40-bit memory address at which operation should be performed
 * @param count    Number of 32-bit words to write (1-32)
 * @param sync     Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr  Signal to raise upon completion
 *
 */

__intrinsic
void cmd_mem_write32_swap_ptr40(
    __xwrite void *data,
    volatile void __addr40 __mem *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
    );

/** Write data to memory with byte swapping into transfer registers (little-endian).
 *
 * @param data     Xfer register(s) containing data to write to memory
 * @param address  32-bit memory address at which operation should be performed
 * @param count    Number of 32-bit words to write (1-32)
 * @param sync     Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr  Signal to raise upon completion
 */
__intrinsic
void cmd_mem_write32_swap_le_ptr32(
    __xwrite void *data,
    volatile void __addr32 __mem *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
    );

/** Write data to memory with byte swapping into transfer registers (little-endian).
 *
 * @param data     Xfer register(s) containing data to write to memory
 * @param address  40-bit memory address at which operation should be performed
 * @param count    Number of 32-bit words to write (1-32)
 * @param sync     Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr  Signal to raise upon completion
 *
 */

__intrinsic
void cmd_mem_write32_swap_le_ptr40(
    __xwrite void *data,
    volatile void __addr40 __mem *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
    );

/*
 * 8-bit commands
 */
/** 8-bit read from memory into transfer registers in bytes (big-endian).
 *
 * Same as: @see cmd_mem_read8_ptr32()
 */
#define mem_read8_be_ptr32              cmd_mem_read8_ptr32

/** 8-bit read from memory into transfer registers in bytes (big-endian).
 *
 * Same as: @see cmd_mem_read8_ptr40()
 */
#define mem_read8_be_ptr40              cmd_mem_read8_ptr40


/** 8-bit write to memory from transfer registers in bytes (big-endian).
 *
 * Same as: @see cmd_mem_write8_ind_ptr32()
 */
#define mem_write8_be_ind_ptr32         cmd_mem_write8_ind_ptr32

/** 8-bit write to memory from transfer registers in bytes (big-endian).
 *
 * Same as: @see cmd_mem_write8_ind_ptr40()
 */
#define mem_write8_be_ind_ptr40         cmd_mem_write8_ind_ptr40


// /** 8-bit write to memory from transfer registers in bytes (big-endian) with byte swapping and indirect
//  *
//  * Same as: @see cmd_mem_write8_swap_ind_ptr32()
//  */
// #define mem_write8_swap_be_ind_ptr32    cmd_mem_write8_swap_ind_ptr32

// /** 8-bit write to memory from transfer registers in bytes (big-endian) with byte swapping and indirect
//  *
//  * Same as: @see cmd_mem_write8_swap_ind_ptr40()
//  */
// #define mem_write8_swap_be_ind_ptr40    cmd_mem_write8_swap_ind_ptr40


/** 32-bit read from memory into transfer registers in bytes (big-endian) with byte swapping.
 *
 * Same as: @see cmd_mem_read32_swap_ptr32()
 */
#define mem_read32_swap_be_ptr32        cmd_mem_read32_swap_ptr32

/** 32-bit read from memory into transfer registers in bytes (big-endian) with byte swapping.
 *
 * Same as: @see cmd_mem_read32_swap_ptr40()
 */
#define mem_read32_swap_be_ptr40        cmd_mem_read32_swap_ptr40

/** 32-bit read from memory into transfer registers in bytes (big-endian), with indirect and byte swapping
 *
 * Same as: @see cmd_mem_read32_swap_ind_ptr32()
 */
#define mem_read32_swap_be_ind_ptr32    cmd_mem_read32_swap_ind_ptr32

// /** 32-bit read from memory into transfer registers in bytes (big-endian), with indirect and byte swapping
//  *
//  * Same as: @see cmd_mem_read32_swap_ind_ptr40()
//  */
// #define mem_read32_swap_be_ind_ptr40    cmd_mem_read32_swap_ind_ptr40


/** 32-bit write from memory into transfer registers in bytes (big-endian) with byte swapping.
 *
 * Same as: @see cmd_mem_write32_swap_ptr32()
 */
#define mem_write32_swap_be_ptr32       cmd_mem_write32_swap_ptr32

/** 32-bit write from memory into transfer registers in bytes (big-endian) with byte swapping.
 *
 * Same as: @see cmd_mem_write32_swap_ptr40()
 */
#define mem_write32_swap_be_ptr40       cmd_mem_write32_swap_ptr40


/** 64-bit write to memory from transfer registers in bytes (big-endian).
 *
 * @see cmd_mem_write64_ptr32
 */
#define mem_write64_be_ptr32            cmd_mem_write64_ptr32

/** 64-bit write to memory from transfer registers in bytes (big-endian).
 *
 * Same as: @see cmd_mem_write64_ptr40()
 */
#define mem_write64_be_ptr40            cmd_mem_write64_ptr40

/** 64-bit write to memory from transfer registers in bytes (big-endian), with indirect
 *
 * @see cmd_mem_write64_ind_ptr32
 */
#define mem_write64_be_ind_ptr32        cmd_mem_write64_ind_ptr32

/** 64-bit write to memory from transfer registers in bytes (big-endian), with indirect
 *
 * Same as: @see cmd_mem_write64_ind_ptr40()
 */
#define mem_write64_be_ind_ptr40        cmd_mem_write64_ind_ptr40



/** 64-bit write to memory from transfer registers in bytes (big-endian) with byte swapping.
 *
 * Same as:@see cmd_mem_write64_swap_ptr32()
 */
#define mem_write64_swap_be_ptr32       cmd_mem_write64_swap_ptr32

/** 64-bit write to memory from transfer registers in bytes (big-endian) with byte swapping.
 *
 * Same as: @see cmd_mem_write64_swap_ptr40()
 */
#define mem_write64_swap_be_ptr40       cmd_mem_write64_swap_ptr40


/** @}
 * @}
 */


#endif
