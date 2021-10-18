/*
 * Copyright (C) 2008-2014 Netronome Systems, Inc.  All rights reserved.
 */

/* NFP Cluster Local Scratch intrinsic function definitions */

#ifndef __NFP_CLS_H__
#define __NFP_CLS_H__

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


/** @file nfp_cls.h
 * @addtogroup cls Cluster Local Scratch Intrinsics
 * @{
 */


/** Color for CLS Metering operations
 */
typedef enum CLS_METER_COLOR
{
    CLS_METER_COLOR_GREEN   =   0x00,      /**< Packet is green    */
    CLS_METER_COLOR_YELLOW  =   0x01,      /**< Packet is yellow   */
    CLS_METER_COLOR_RED     =   0x02,      /**< Packet is red      */
} CLS_METER_COLOR;

/** RFC type to use for CLS Metering operations.
 */
typedef enum CLS_METER_RFC
{
    CLS_METER_RFC2698       =   0x00,      /**< RFC 2698 defines one method for two rate three color metering   */
    CLS_METER_RFC2697       =   0x01,      /**< RFC 2697 defines one method for two rate three color metering  */
    CLS_METER_RFC4115       =   0x01,      /**< RFC 4115 defines second method for two rate three color metering */
} CLS_METER_RFC;


/** @name General memory access functions for Cluster Local Scratch
 * @{
 */


/** Read 32-bit words from Cluster Local Scratch in Big Endian mode.
 *
 * @param data     Read Transfer Registers the read data will be placed in
 * @param address  Address to read from
 * @param count    Number of 32-bit words to read
 * @param sync     Type of synchronization (sig_done or ctx_swap)
 * @param sig_ptr  Signal to be raised on completion
 *
 * @see cmd_cls_read_ptr32()
 */
__intrinsic
void cmd_cls_read_be_ptr32(
    __xread void *data,
    volatile void __cls *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
);


/** Read 32-bit words from 40 bit Cluster Local Scratch in Big Endian mode.
 *
 * @param data     Read Transfer Registers the read data will be placed in
 * @param address  40 bit address to read from where upper 6 bits indicate the island.
 *                  See 6xxx databook for recommended addressing mode.
 * @param count    Number of 32-bit words to read
 * @param sync     Type of synchronization (sig_done or ctx_swap)
 * @param sig_ptr  Signal to be raised on completion
 *
 *
 * @see cmd_cls_read_ptr40()
 */
__intrinsic
void cmd_cls_read_be_ptr40(
    __xread void *data,
    volatile void __addr40 __cls *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
);



/** Read 32-bit words from Cluster Local Scratch in Little Endian mode.
 *
 * @param data     Read Transfer Registers the read data will be placed in
 * @param address  Address to read from
 * @param count    Number of 32-bit words to read
 * @param sync     Type of synchronization (sig_done or ctx_swap)
 * @param sig_ptr  Signal to be raised on completion
 *
 * @see cmd_cls_read_ptr32()
 */
__intrinsic
void cmd_cls_read_le_ptr32(
    __xread void *data,
    volatile void __cls *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
);

/** Read 32-bit words from (40 bit) Cluster Local Scratch in Little Endian mode.
 *
 * @param data     Read Transfer Registers the read data will be placed in
 * @param address  40 bit address to read from where upper 6 bits indicate the island.
 *                  See 6xxx databook for recommended addressing mode.
 * @param count    Number of 32-bit words to read
 * @param sync     Type of synchronization (sig_done or ctx_swap)
 * @param sig_ptr  Signal to be raised on completion
 *
 * @see cmd_cls_read_ptr40()
 */
__intrinsic
void cmd_cls_read_le_ptr40(
    __xread void *data,
    volatile void __addr40 __cls *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
);


/** Read 32-bit words from Cluster Local Scratch.
 *
 * @param data     Read Transfer Registers the read data will be placed in
 * @param address  Address to read from
 * @param count    Number of 32-bit words to read
 * @param sync     Type of synchronization (sig_done or ctx_swap)
 * @param sig_ptr  Signal to be raised on completion
 *
 * Read count number of 32-bit words from Cluster Local Scratch from address
 * and place them in the transfer registers pointed to by data. The signal
 * sig_ptr will be raised on completion and sync determines if the
 * operation switch context or completes asynchronously.
 *
 * The address is a byte address and does not need to be naturally
 * aligned. However, LW aligned reads will be faster.
 *
 * Data read will be returned either in Little Endian or Big Endian
 * depending on the compiler setting.
 *
 * @note If count is not a constant, the compiler will automatically
 *       generate an instruction with an indirect_ref optional token.
 */
__intrinsic
void cmd_cls_read_ptr32(
    __xread void *data,
    volatile void __cls *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
);


/** Read 32-bit words from 40 bit Cluster Local Scratch.
 *
 * @param data     Read Transfer Registers the read data will be placed in
 * @param address  40 bit address to read from where upper 6 bits indicate the island.
 *                  See 6xxx databook for recommended addressing mode.
 * @param count    Number of 32-bit words to read
 * @param sync     Type of synchronization (sig_done or ctx_swap)
 * @param sig_ptr  Signal to be raised on completion
 *
 * Read count number of 32-bit words from Cluster Local Scratch from address
 * and place them in the transfer registers pointed to by data. The signal
 * sig_ptr will be raised on completion and sync determines if the
 * operation switch context or completes asynchronously.
 *
 * The address is a byte address and does not need to be naturally
 * aligned. However, LW aligned reads will be faster.
 *
 * Data read will be returned either in Little Endian or Big Endian
 * depending on the compiler setting.
 *
 * @note If count is not a constant, the compiler will automatically
 *       generate an instruction with an indirect_ref optional token.
 */
__intrinsic
void cmd_cls_read_ptr40(
    __xread void *data,
    volatile void __addr40 __cls *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
);


/** Write 32-bit words to Cluster Local Scratch in Big Endian mode.
 *
 * @param data     Write Transfer Registers to be written
 * @param address  Address to write to
 * @param count    Number of 32-bit words to written
 * @param sync     Type of synchronization (sig_done or ctx_swap)
 * @param sig_ptr  Signal to be raised on completion
 *
 * @see cmd_cls_write_ptr32()
 */
__intrinsic
void cmd_cls_write_be_ptr32(
    __xwrite void *data,
    volatile void __addr32 __cls *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
);


/** Write 32-bit words to 40 bit Cluster Local Scratch in Big Endian mode.
 *
 * @param data     Write Transfer Registers to be written
 * @param address  40 bit address to write to where upper 6 bits indicate the island.
 *                  See 6xxx databook for recommended addressing mode.
 * @param count    Number of 32-bit words to written
 * @param sync     Type of synchronization (sig_done or ctx_swap)
 * @param sig_ptr  Signal to be raised on completion
 *
 * @see cmd_cls_write_ptr32()
 */
__intrinsic
void cmd_cls_write_be_ptr40(
    __xwrite void *data,
    volatile void __addr40 __cls *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
);



/** Write 32-bit words to Cluster Local Scratch in Little Endian mode.
 *
 * @param data     Write Transfer Registers to be written
 * @param address  Address to write to
 * @param count    Number of 32-bit words to be written
 * @param sync     Type of synchronization (sig_done or ctx_swap)
 * @param sig_ptr  Signal to be raised on completion
 *
 * @see cmd_cls_write_ptr32()
 */
__intrinsic
void cmd_cls_write_le_ptr32(
    __xwrite void *data,
    volatile void __cls *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
);

/** Write 32-bit words to 40 bit Cluster Local Scratch in Little Endian mode.
 *
 * @param data     Write Transfer Registers to be written
 * @param address  40 bit address to write to where upper 6 bits indicate the island.
 *                  See 6xxx databook for recommended addressing mode.
 * @param count    Number of 32-bit words to be written
 * @param sync     Type of synchronization (sig_done or ctx_swap)
 * @param sig_ptr  Signal to be raised on completion
 *
 * @see cmd_cls_write_ptr40()
 */
__intrinsic
void cmd_cls_write_le_ptr40(
    __xwrite void *data,
    volatile void __addr40 __cls *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
);


/** Write bytes to Cluster Local Scratch in Big Endian mode
 *  in indirect mode.
 *
 * @param data     Write Transfer Registers data to write to Cluster Local Scratch
 * @param address  40 Bit address to write to
 * @param max_nn    Maximum number of bytes to write (1-32)
 * @param ind      Indirection type
 * @param sync     Type of synchronization (sig_done or ctx_swap)
 * @param sig_ptr  Signal to be raised on completion
 *
 * @see cls_write_ind()
 */

__intrinsic
void cmd_cls_write8_be_ind_ptr40(
    __xwrite void *data,
    volatile void __addr40 __cls *address,
    uint32_t max_nn,
    generic_ind_t ind,
    sync_t sync,
    SIGNAL *sig_ptr
);


/** Write bytes to Cluster Local Scratch in Big Endian mode
 *  in indirect mode.
 *
 * @param data     Write Transfer Registers data to write to Cluster Local Scratch
 * @param address  32 Bit address to write to
 * @param max_nn    Maximum number of bytes to write (1-32)
 * @param ind      Indirection type
 * @param sync     Type of synchronization (sig_done or ctx_swap)
 * @param sig_ptr  Signal to be raised on completion
 *
 * @see cls_write_ind()
 */
__intrinsic
void cmd_cls_write8_be_ind_ptr32(
    __xwrite void *data,
    volatile void __cls *address,
    uint32_t max_nn,
    generic_ind_t ind,
    sync_t sync,
    SIGNAL *sig_ptr
);


/** Write bytes to Cluster Local Scratch in Little Endian mode
 *  in indirect mode.
 *
 * @param data     Write Transfer Registers data to write to Cluster Local Scratch
 * @param address  40 Bit address to write to
 * @param max_nn    Maximum number of bytes to write (1-32)
 * @param ind      Indirection type
 * @param sync     Type of synchronization (sig_done or ctx_swap)
 * @param sig_ptr  Signal to be raised on completion
 *
 * @see cls_write_ind()
 */

__intrinsic
void cmd_cls_write8_le_ind_ptr40(
    __xwrite void *data,
    volatile void __addr40 __cls *address,
    uint32_t max_nn,
    generic_ind_t ind,
    sync_t sync,
    SIGNAL *sig_ptr
);



/** Write bytes to Cluster Local Scratch in Little Endian mode
 *  in indirect mode.
 *
 * @param data     Write Transfer Registers data to write to Cluster Local Scratch
 * @param address  32 Bit address to write to
 * @param max_nn    Maximum number of bytes to write (1-32)
 * @param ind      Indirection type
 * @param sync     Type of synchronization (sig_done or ctx_swap)
 * @param sig_ptr  Signal to be raised on completion
 *
 * @see cls_write_ind()
 */
__intrinsic
void cmd_cls_write8_le_ind_ptr32(
    __xwrite void *data,
    volatile void __cls *address,
    uint32_t max_nn,
    generic_ind_t ind,
    sync_t sync,
    SIGNAL *sig_ptr
);


/** Write bytes to Cluster Local Scratch in Big or Little Endian format
 *  in indirect mode.
 *
 * @param data     Write Transfer Registers holds the data to be written
 * @param address  40 Bit address to write to
 * @param max_nn    Maximum number of bytes to write (1-32)
 * @param ind      Indirection type
 * @param sync     Type of synchronization (sig_done or ctx_swap)
 * @param sig_ptr  Signal to be raised on completion
 *
 * @see cls_write_ind()
 */

__intrinsic
void cmd_cls_write8_ind_ptr40(
    __xwrite void *data,
    volatile void __addr40 __cls *address,
    uint32_t max_nn,
    generic_ind_t ind,
    sync_t sync,
    SIGNAL *sig_ptr
);




/** Write bytes to Cluster Local Scratch in Big or Little Endian format
 *  in indirect mode.
 *
 * @param data     Write Transfer Registers holds the data to be written
 * @param address  32 Bit address to write to
 * @param max_nn    Maximum number of bytes to write (1-32)
 * @param ind      Indirection type
 * @param sync     Type of synchronization (sig_done or ctx_swap)
 * @param sig_ptr  Signal to be raised on completion
 *
 * @see cls_write_ind()
 */
__intrinsic
void cmd_cls_write8_ind_ptr32(
    __xwrite void *data,
    volatile void __cls *address,
    uint32_t max_nn,
    generic_ind_t ind,
    sync_t sync,
    SIGNAL *sig_ptr
);


/** Write 32-bit words to Cluster Local Scratch.
 *
 * @param data     Write Transfer Registers to be written
 * @param address  Address to write to
 * @param count    Number of 32-bit words to be written
 * @param sync     Type of synchronization (sig_done or ctx_swap)
 * @param sig_ptr  Signal to be raised on completion
 *
 * Write count number of 32-bit words from the write transfer registers
 * pointed to by data to Cluster Local Scratch at address. The signal
 * sig_ptr will be raised on completion and sync determines if the
 * operation switch context or completes asynchronously.
 *
 * The address has to be long word aligned.
 *
 * Data will be treated as Big Endian or Little Endian depending on the
 * compiler setting.
 */
__intrinsic
void cmd_cls_write_ptr32(
    __xwrite void *data,
    volatile void __cls *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
);

/** Write 32-bit words to 40 bit Cluster Local Scratch.
 *
 * @param data     Write Transfer Registers to be written
 * @param address  40 bit address to write to where upper 6 bits indicate the island.
 *                  See 6xxx databook for recommended addressing mode.
 * @param count    Number of 32-bit words to be written
 * @param sync     Type of synchronization (sig_done or ctx_swap)
 * @param sig_ptr  Signal to be raised on completion
 *
 * Write count number of 32-bit words from the write transfer registers
 * pointed to by data to Cluster Local Scratch at address. The signal
 * sig_ptr will be raised on completion and sync determines if the
 * operation switch context or completes asynchronously.
 *
 * The address has to be long word aligned.
 *
 * Data will be treated as Big Endian or Little Endian depending on the
 * compiler setting.
 */
__intrinsic
void cmd_cls_write_ptr40(
    __xwrite void *data,
    volatile void __addr40 __cls *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
);


/** Write bytes to Cluster Local Scratch in Big Endian mode.
 *
 * @param data     Write Transfer Registers to be written
 * @param address  Address to write to
 * @param count    Number of bytes to be written
 * @param sync     Type of synchronization (sig_done or ctx_swap)
 * @param sig_ptr  Signal to be raised on completion
 *
 * @see cmd_cls_write8_ptr32()
 */
__intrinsic
void cmd_cls_write8_be_ptr32(
    __xwrite void *data,
    volatile void __cls *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
);

/** Write bytes to 40 bit Cluster Local Scratch in Big Endian mode.
 *
 * @param data     Write Transfer Registers to be written
 * @param address  40 bit address to write to where upper 6 bits indicate the island.
 *                  See 6xxx databook for recommended addressing mode.
 * @param count    Number of bytes to be written
 * @param sync     Type of synchronization (sig_done or ctx_swap)
 * @param sig_ptr  Signal to be raised on completion
 *
 * @see cmd_cls_write8_ptr32()
 */
__intrinsic
void cmd_cls_write8_be_ptr40(
    __xwrite void *data,
    volatile void __addr40 __cls *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
);


/** Write bytes to Cluster Local Scratch in Little Endian mode.
 *
 * @param data     Write Transfer Registers to be written
 * @param address  Address to write to
 * @param count    Number of bytes to be written
 * @param sync     Type of synchronization (sig_done or ctx_swap)
 * @param sig_ptr  Signal to be raised on completion
 *
 * @see cmd_cls_write8_ptr32()
 */
__intrinsic
void cmd_cls_write8_le_ptr32(
    __xwrite void *data,
    volatile void __cls *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
);

/** Write bytes to to 40 bit Cluster Local Scratch in Little Endian mode.
 *
 * @param data     Write Transfer Registers to be written
 * @param address  40 bit address to write to where upper 6 bits indicate the island.
 *                  See 6xxx databook for recommended addressing mode.
 * @param count    Number of bytes to be written
 * @param sync     Type of synchronization (sig_done or ctx_swap)
 * @param sig_ptr  Signal to be raised on completion
 *
 * @see cmd_cls_write8_ptr32()
 */
__intrinsic
void cmd_cls_write8_le_ptr40(
    __xwrite void *data,
    volatile void __addr40 __cls *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
);


/** Write bytes to Cluster Local Scratch.
 *
 * @param data     Write Transfer Registers to be written
 * @param address  Address to write to
 * @param count    Number of bytes to be written
 * @param sync     Type of synchronization (sig_done or ctx_swap)
 * @param sig_ptr  Signal to be raised on completion
 *
 * Write count number of bytes from the write transfer registers
 * pointed to by data to Cluster Local Scratch at address. The signal
 * sig_ptr will be raised on completion and sync determines if the
 * operation switch context or completes asynchronously.
 *
 * The address can be an arbitrary byte address.
 */
__intrinsic
void cmd_cls_write8_ptr32(
    __xwrite void *data,
    volatile void __cls *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
);

/** Write bytes to 40 bit Cluster Local Scratch.
 *
 * @param data     Write Transfer Registers to be written
 * @param address  40 bit address to write to where upper 6 bits indicate the island.
 *                  See 6xxx databook for recommended addressing mode.
 * @param count    Number of bytes to be written
 * @param sync     Type of synchronization (sig_done or ctx_swap)
 * @param sig_ptr  Signal to be raised on completion
 *
 * Write count number of bytes from the write transfer registers
 * pointed to by data to Cluster Local Scratch at address. The signal
 * sig_ptr will be raised on completion and sync determines if the
 * operation switch context or completes asynchronously.
 *
 * The address can be an arbitrary byte address.
 */
__intrinsic
void cmd_cls_write8_ptr40(
    __xwrite void *data,
    volatile void __addr40 __cls *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
);


/** Clear bits in  40 bit Cluster Local Scratch with indirect word.
 *
 * @param mask     Mask of bits to clear
 * @param address  40 bit address in which to clear bits  where upper 6 bits indicate the island.
 *                 See 6xxx databook for recommended addressing mode.
 * @param max_nn    Number of 32-bit words to clear bits from
 * @param ind      Indirection type
 * @param sync     Type of synchronization (sig_done or ctx_swap)
 * @param sig_ptr  Signal to be raised on completion
 *
 * Clears the bits specified in the mask from the address onwards. Bits in
 * multiple 32-bit words can be cleared by specifying max_nn greater than 1. In
 * this case mask needs to point to an array of at least max_nn mask LWs.
 *
 * If max_nn is greater than 1, then the address needs to be QW aligned
 * otherwise it has to be LW aligned.
 *
 * @note Alignment is not currently enforced.
 */

__intrinsic
void cmd_cls_clear_bits_ind_ptr40(
    __xwrite uint32_t *mask,
    volatile void __addr40 __cls *address,
    uint32_t max_nn,
    generic_ind_t ind,
    sync_t sync,
    SIGNAL *sig_ptr
    );


/** Clear bits in Cluster Local Scratch memory with indirect word.
 *
 * @param mask     Mask of bits to clear
 * @param address  Address of 32-bit word in which to clear bits
 * @param max_nn    Number of 32-bit words to clear bits from
 * @param ind      Indirection type
 * @param sync     Type of synchronization (sig_done or ctx_swap)
 * @param sig_ptr  Signal to be raised on completion
 *
 * Clears the bits specified in the mask from the address onwards. Bits in
 * multiple 32-bit words can be cleared by specifying max_nn greater than 1. In
 * this case mask needs to point to an array of at least max_nn mask LWs.
 *
 * If max_nn is greater than 1, then the address needs to be QW aligned
 * otherwise it has to be LW aligned.
 *
 * @note Alignment is not currently enforced.
 */
__intrinsic
void cmd_cls_clear_bits_ind_ptr32(
    __xwrite uint32_t *mask,
    volatile void __cls *address,
    uint32_t max_nn,
    generic_ind_t ind,
    sync_t sync,
    SIGNAL *sig_ptr
    );


/** Set bits in Cluster Local Scratch with indirect word.
 *
 * @param mask     Mask of bits to set
 * @param address  40 bit address in which to set bits  where upper 6 bits indicate the island.
 *                 See 6xxx databook for recommended addressing mode.
 * @param max_nn    Number of 32-bit words to set bits from
 * @param ind      Indirection type
 * @param sync     Type of synchronization (sig_done or ctx_swap)
 * @param sig_ptr  Signal to be raised on completion
 *
 * Sets the bits specified in the mask from the address onwards. Bits in
 * multiple 32-bit words can be set by specifying max_nn greater than 1. In
 * this case mask needs to point to an array of at least max_nn mask LWs.
 *
 * If max_nn is greater than 1, then the address needs to be QW aligned
 * otherwise it has to be LW aligned.
 *
 * @note Alignment is not currently enforced.
 */

__intrinsic
void cmd_cls_set_bits_ind_ptr40(
    __xwrite uint32_t *mask,
    volatile void __addr40 __cls *address,
    uint32_t max_nn,
    generic_ind_t ind,
    sync_t sync,
    SIGNAL *sig_ptr
    );


/** Set bits in  40 bit Cluster Local Scratch with indirect word.
 *
 * @param mask     Mask of bits to set
 * @param address  Address of 32-bit word in which to set bits
 * @param max_nn   Number of 32-bit words to set bits from
 * @param ind      Indirection type
 * @param sync     Type of synchronization (sig_done or ctx_swap)
 * @param sig_ptr  Signal to be raised on completion
 *
 * Sets the bits specified in the mask from the address onwards. Bits in
 * multiple 32-bit words can be set by specifying max_nn greater than 1. In
 * this case mask needs to point to an array of at least max_nn mask LWs.
 *
 * If max_nn is greater than 1, then the address needs to be QW aligned
 * otherwise it has to be LW aligned.
 *
 * @note Alignment is not currently enforced.
 */
__intrinsic
void cmd_cls_set_bits_ind_ptr32(
    __xwrite uint32_t *mask,
    volatile void __cls *address,
    uint32_t max_nn,
    generic_ind_t ind,
    sync_t sync,
    SIGNAL *sig_ptr
    );


/** @}
 * @name Atomic functions for Cluster Local Scratch
 * @{
 */

/** Clear bits in Cluster Local Scratch memory.
 *
 * @param mask     Mask of bits to clear
 * @param address  Address of 32-bit word in which to clear bits
 * @param count    Number of 32-bit words to clear bits from
 * @param sync     Type of synchronization (sig_done or ctx_swap)
 * @param sig_ptr  Signal to be raised on completion
 *
 * Clears the bits specified in the mask from the address onwards. Bits in
 * multiple 32-bit words can be cleared by specifying count greater than 1. In
 * this case mask needs to point to an array of at least count mask LWs.
 *
 * If count is greater than 1, then the address needs to be QW aligned
 * otherwise it has to be LW aligned.
 *
 * @note Alignment is not currently enforced.
 */
__intrinsic
void cmd_cls_clear_bits_ptr32(
    __xwrite uint32_t *mask,
    volatile void __cls *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
);

/** Clear bits in  40 bit Cluster Local Scratch.
 *
 * @param mask     Mask of bits to clear
 * @param address  40 bit address in which to clear bits  where upper 6 bits indicate the island.
 *                  See 6xxx databook for recommended addressing mode.
 * @param count    Number of 32-bit words to clear bits from
 * @param sync     Type of synchronization (sig_done or ctx_swap)
 * @param sig_ptr  Signal to be raised on completion
 *
 * Clears the bits specified in the mask from the address onwards. Bits in
 * multiple 32-bit words can be cleared by specifying count greater than 1. In
 * this case mask needs to point to an array of at least count mask LWs.
 *
 * If count is greater than 1, then the address needs to be QW aligned
 * otherwise it has to be LW aligned.
 *
 * @note Alignment is not currently enforced.
 */
__intrinsic
void cmd_cls_clear_bits_ptr40(
    __xwrite uint32_t *mask,
    volatile void __addr40 __cls *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
);



/** Set bits in Cluster Local Scratch memory.
 *
 * @param mask     Mask of bits to set
 * @param address  Address of 32-bit word in which to set bits
 * @param count    Number of 32-bit words to clear bits from
 * @param sync     Type of synchronization (sig_done or ctx_swap)
 * @param sig_ptr  Signal to be raised on completion
 *
 * Set the bits specified in the mask from the address onwards. Bits in
 * multiple 32-bit words can be set by specifying count greater than 1. In
 * this case mask needs to point to an array of at least count mask LWs.
 *
 * If count is greater than 1, then the address needs to be QW aligned
 * otherwise it has to be LW aligned.
 *
 * @note Alignment is not currently enforced.
 */
__intrinsic
void cmd_cls_set_bits_ptr32(
    __xwrite uint32_t *mask,
    volatile void __cls *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
);

/** Set bits in 40 bit Cluster Local Scratch.
 *
 * @param mask     Mask of bits to set
 * @param address  40 bit address in which to set bits where upper 6 bits indicate the island.
 *                  See 6xxx databook for recommended addressing mode.
 * @param count    Number of 32-bit words to set bits from
 * @param sync     Type of synchronization (sig_done or ctx_swap)
 * @param sig_ptr  Signal to be raised on completion
 *
 * Set the bits specified in the mask from the address onwards. Bits in
 * multiple 32-bit words can be set by specifying count greater than 1. In
 * this case mask needs to point to an array of at least count mask LWs.
 *
 * If count is greater than 1, then the address needs to be QW aligned
 * otherwise it has to be LW aligned.
 *
 * @note Alignment is not currently enforced.
 */
__intrinsic
void cmd_cls_set_bits_ptr40(
    __xwrite uint32_t *mask,
    volatile void __addr40 __cls *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
);

/** Set bits in Cluster Local Scratch memory immediate.
 *
 * @param address  Address of 32-bit word in which to set bits
 * @param value    Mask of bits to set
 *
 * Set the bits specified in the mask in the word pointed to by
 * address. The address needs to be LW aligned.
 *
 * The immediate value is encoded in the instruction itself (in the count
 * field). For values greater than 3 bits (the width of the count field) an
 * indirect override is used. This allows for only the bottom 14 bits (bits
 * 0-13) of the CLS word to be set in immediate mode.
 *
 * The passed in value can actually be up to 16 bits wide. However, bit 14
 * is special. It works as a "extend" bit. If bit 14 is set in value and if
 * bit 13 is set, then bits 13-31 will be set in the address 32-bit word in
 * CLS. Thus, by passing a value of 0x7fff all bits in the addressed LW
 * will be set.
 *
 * Bit 15 of value is ignored.
 *
 */
__intrinsic
void cmd_cls_set_bits_imm_ptr32(
    volatile void __cls *address,
    uint32_t value
);

/** Set bits in 40 bit Cluster Local Scratch immediate.
 *
 * @param address  40 bit address in which to set bits where upper 6 bits indicate the island.
 *                  See 6xxx databook for recommended addressing mode.
 * @param value    Mask of bits to set
 *
 * Set the bits specified in the mask in the word pointed to by
 * address. The address needs to be LW aligned.
 *
 * The immediate value is encoded in the instruction itself (in the count
 * field). For values greater than 3 bits (the width of the count field) an
 * indirect override is used. This allows for only the bottom 14 bits (bits
 * 0-13) of the CLS word to be set in immediate mode.
 *
 * The passed in value can actually be up to 16 bits wide. However, bit 14
 * is special. It works as a "extend" bit. If bit 14 is set in value and if
 * bit 13 is set, then bits 13-31 will be set in the address 32-bit word in
 * CLS. Thus, by passing a value of 0x7fff all bits in the addressed LW
 * will be set.
 *
 * Bit 15 of value is ignored.
 *
 */
__intrinsic
void cmd_cls_set_bits_imm_ptr40(
    volatile void __addr40 __cls *address,
    uint32_t value
);


/** Clear bits in Cluster Local Scratch memory immediate.
 *
 * @param address  Address of 32-bit word in which to clear bits
 * @param value    Mask of bits to clear
 *
 * Clear the bits specified in the mask in the word pointed to by
 * address. The address needs to be LW aligned.
 *
 * The immediate value is encoded in the instruction itself (in the count
 * field). For values greater than 3 bits (the width of the count field)
 * and indirect override is used. This allows for only the bottom 14 bits
 * of the CLS word to be cleared in immediate mode.
 *
 * The passed in value can actually be up to 16 bits wide. However, bit 14
 * is special. It works as a "extend" bit. If bit 14 is set in value and if
 * bit 13 is set, then bits 13-31 will cleared set in the address 32-bit word in
 * CLS. Thus, by passing a value of 0x7fff all bits in the addressed LW
 * will be cleared.
 *
 * Bit 15 of value is ignored.
 */
__intrinsic
void cmd_cls_clear_bits_imm_ptr32(
    volatile void __cls *address,
    uint32_t value
);


/** Clear bits in 40 bit Cluster Local Scratch memory immediate.
 *
 * @param address  40 bit address in which to clear bits where upper 6 bits indicate the island.
 *                  See 6xxx databook for recommended addressing mode.
 * @param value    Mask of bits to clear
 *
 * Clear the bits specified in the mask in the word pointed to by
 * address. The address needs to be LW aligned.
 *
 * The immediate value is encoded in the instruction itself (in the count
 * field). For values greater than 3 bits (the width of the count field)
 * and indirect override is used. This allows for only the bottom 14 bits
 * of the CLS word to be cleared in immediate mode.
 *
 * The passed in value can actually be up to 16 bits wide. However, bit 14
 * is special. It works as a "extend" bit. If bit 14 is set in value and if
 * bit 13 is set, then bits 13-31 will cleared set in the address 32-bit word in
 * CLS. Thus, by passing a value of 0x7fff all bits in the addressed LW
 * will be cleared.
 *
 * Bit 15 of value is ignored.
 */
__intrinsic
void cmd_cls_clear_bits_imm_ptr40(
    volatile void __addr40 __cls *address,
    uint32_t value
);


/** XOR bits in 40 bit Cluster Local Scratch with indirect
 *
 * @param mask     Mask of bits to XOR
 * @param address  40 bit address in which to XOR bits where upper 6 bits indicate the island.
 *                  See 6xxx databook for recommended addressing mode.
 * @param max_nn    Number of 32-bit words to XOR bits from
 * @param ind      Indirection word
 * @param sync     Type of synchronization (sig_done or ctx_swap)
 * @param sig_ptr  Signal to be raised on completion
 *
 * XOR the bits specified in the mask with the 32-bit words at address
 * onwards. Bits in multiple 32-bit words can be XORed by specifying max_nn
 * greater than 1. In this case mask needs to point to an array of at least
 * max_nn mask LWs.
 *
 * If max_nn is greater than 1, then the address needs to be QW aligned
 * otherwise it has to be LW aligned.
 *
 * @note Alignment is not currently enforced.
 */

__intrinsic
void cmd_cls_xor_bits_ind_ptr40(
    __xwrite void *mask,
    volatile void __addr40 __cls *address,
    uint32_t max_nn,
    generic_ind_t ind,
    sync_t sync,
    SIGNAL *sig_ptr
    );


/** XOR bits in 32 bit Cluster Local Scratch with indirect
 *
 * @param mask     Mask of bits to XOR
 * @param address  32 Bit address to xor from
 * @param max_nn    Maximum number of 32-bit words to xor (1-32)
 * @param ind      Indirection type
 * @param sync     Type of synchronization (sig_done or ctx_swap)
 * @param sig_ptr  Signal to be raised on completion
 *
 * @note Alignment is not currently enforced.
 */
__intrinsic
void cmd_cls_xor_bits_ind_ptr32(
    __xwrite void *mask,
    volatile void __cls *address,
    uint32_t max_nn,
    generic_ind_t ind,
    sync_t sync,
    SIGNAL *sig_ptr
    );


/** XOR bits in Cluster Local Scratch memory.
 *
 * @param mask     Mask of bits to XOR
 * @param address  Address of 32-bit word in which to XOR bits
 * @param count    Number of 32-bit words to XOR bits from
 * @param sync     Type of synchronization (sig_done or ctx_swap)
 * @param sig_ptr  Signal to be raised on completion
 *
 * XOR the bits specified in the mask with the 32-bit words at address
 * onwards. Bits in multiple 32-bit words can be XORed by specifying count
 * greater than 1. In this case mask needs to point to an array of at least
 * count mask LWs.
 *
 * If count is greater than 1, then the address needs to be QW aligned
 * otherwise it has to be LW aligned.
 *
 * @note Alignment is not currently enforced.
 */
__intrinsic
void cmd_cls_xor_bits_ptr32(
    __xwrite void *mask,
    volatile void __cls *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
);

/** XOR bits in 40 bit Cluster Local Scratch.
 *
 * @param mask     Mask of bits to XOR
 * @param address  40 bit address in which to XOR bits where upper 6 bits indicate the island.
 *                  See 6xxx databook for recommended addressing mode.
 * @param count    Number of 32-bit words to XOR bits from
 * @param sync     Type of synchronization (sig_done or ctx_swap)
 * @param sig_ptr  Signal to be raised on completion
 *
 * XOR the bits specified in the mask with the 32-bit words at address
 * onwards. Bits in multiple 32-bit words can be XORed by specifying count
 * greater than 1. In this case mask needs to point to an array of at least
 * count mask LWs.
 *
 * If count is greater than 1, then the address needs to be QW aligned
 * otherwise it has to be LW aligned.
 *
 * @note Alignment is not currently enforced.
 */
__intrinsic
void cmd_cls_xor_bits_ptr40(
    __xwrite void *mask,
    volatile void __addr40 __cls *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
);


/** Swap bits in Cluster Local Scratch memory.
 *
 * @param data     Data to swap
 * @param address  Address of 32-bit word in which to swap bits
 * @param count    Number of 32-bit words to swap (maximum is 32)
 * @param sync     Type of synchronization (sig_done or ctx_swap)
 * @param sig_ptr  Signal to be raised on completion
 *
 * Swap the bits specified in the transfer register with the 32-bit words at address
 * onwards.
 *
 */
__intrinsic
void cmd_cls_swap_ptr32(
    __xwrite void *data,
    volatile void __cls *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
);


/** Swap bits in 40 bit Cluster Local Scratch.
 *
 * @param data     Data to swap
 * @param address  40 bit address in which to swap bits where upper 6 bits indicate the island.
 *                  See 6xxx databook for recommended addressing mode.
 * @param count    Number of 32-bit words to swap (maximum is 32)
 * @param sync     Type of synchronization (sig_done or ctx_swap)
 * @param sig_ptr  Signal to be raised on completion
 *
 * Swap the bits specified in the transfer register with the 32-bit words at address
 * onwards.
 *
 */
__intrinsic
void cmd_cls_swap_ptr40(
    __xwrite void *data,
    volatile void __addr40 __cls *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
);


/** Perform a metering operation in Cluster Local Scratch memory
 *
 * @param val      Return meter command color result i.e. 0 (green), 1 (yellow), 2 (red)
 * @param data     Data to write
 * @param address  Address to write metering information
 * @param rfc_type 0 (RFC2698), 1 (RFC2697/4115). See enum CLS_METER_RFC
 * @param color    Color of packet: 0 (green), 1 (yellow), 2 (red). See  enum CLS_METER_COLOR
 * @param sync     Type of synchronization (sig_done or ctx_swap)
 * @param sig_ptr  Signal to be raised on completion
 *
 * Perform a metering operation.
 */
__intrinsic
void cmd_cls_meter_ptr32(
    __xread uint32_t *val,        /* returned pre-modified value            */
    __xwrite uint32_t *data,      /* data to add                            */
    volatile void __cls *address,
    uint32_t rfc_type,
    uint32_t color,
    sync_t sync,
    SIGNAL *sig_ptr
);

/** Perform a metering operation in 40 bit Cluster Local Scratch memory.
 *
 * @param val      Return meter command color result i.e. 0 (green), 1 (yellow), 2 (red)
 * @param data     Data to write
 * @param address  Address to write metering information
 * @param rfc_type 0 (RFC2698), 1 (RFC2697/4115). See enum CLS_METER_RFC
 * @param color    Color of packet: 0 (green), 1 (yellow), 2 (red). See  enum CLS_METER_COLOR
 * @param sync     Type of synchronization (sig_done or ctx_swap)
 * @param sig_ptr  Signal to be raised on completion
 *
 * Perform a metering operation.
 */
__intrinsic
void cmd_cls_meter_ptr40(
    __xread uint32_t *val,        /* returned pre-modified value            */
    __xwrite uint32_t *data,      /* data to add                            */
    volatile void __addr40 __cls *address,
    uint32_t rfc_type,
    uint32_t color,
    sync_t sync,
    SIGNAL *sig_ptr
);


/** Perform a statistic in Cluster Local Scratch memory.
 *
 * @param data     Add 32-bit data to address specified
 * @param address  Address in CLS in which to add statistic
 * @param count    Number of words to add (maximum is 32)
 * @param sync     Type of synchronization (sig_done or ctx_swap)
 * @param sig_ptr  Signal to be raised on completion
 *
 * Add statistic to CLS memory.
 */
__intrinsic
void cmd_cls_statistic_ptr32(
    __xwrite void *data,
    volatile void __cls *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
);


/** Perform a statistic in 40 bit Cluster Local Scratch.
 *
 * @param data     Add 32-bit data to address specified
 * @param address  40 bit CLS address in which to perform statistic where upper 6 bits indicate the island.
 *                  See 6xxx databook for recommended addressing mode.
 * @param count    Number of words to add (maximum is 32)
 * @param sync     Type of synchronization (sig_done or ctx_swap)
 * @param sig_ptr  Signal to be raised on completion
 *
 * Add statistic to CLS memory.
 */
__intrinsic
void cmd_cls_statistic_ptr40(
    __xwrite void *data,
    volatile void __addr40 __cls *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
);

/** Add 16-bit immediate statistic data to address in Cluster Local Scratch memory.
 *
 * @param address  Address of 32-bit word in which to write statistic
 * @param value    Value to write
 *
 * Add the value to the 32-bit word at the specified address. If the
 * value is less than 3 bits it is directly encoded in the instruction,
 * otherwise an indirect reference is used.
 *
 */
__intrinsic
void cmd_cls_statistic_imm_ptr32(
    volatile void __cls *address,
    uint32_t value
);


/** Add 16-bit immediate statistic data to address in 40 bit Cluster Local Scratch memory.
 *
 * @param address  40 bit address in which to write statistic where upper 6 bits indicate the island.
 *                  See 6xxx databook for recommended addressing mode.
 * @param value    Value to write
 *
 * Add the value to the 32-bit word at the specified address. If the
 * value is less than 3 bits it is directly encoded in the instruction,
 * otherwise an indirect reference is used.
 *
 */
__intrinsic
void cmd_cls_statistic_imm_ptr40(
    volatile void __addr40 __cls *address,
    uint32_t value
);


/** Add value to Cluster Local Scratch memory address.
 *
 * @param data     Value(s) to add
 * @param address  Address of 32-bit word to which to add values
 * @param count    Number of 32-bit words to add values to
 * @param sync     Type of synchronization (sig_done or ctx_swap)
 * @param sig_ptr  Signal to be raised on completion
 *
 * Add the value in data to the 32-bit word at the specified address. Multiple
 * add operations (count > 1) can be performed in which case data needs to
 * point to an array of the appropriate size.
 *
 * If count is greater than 1, then the address needs to be QW aligned
 * otherwise it has to be LW aligned.
 *
 * @note Alignment is not currently enforced.
 */
__intrinsic
void cmd_cls_add_ptr32(
    __xwrite void *data,
    volatile void __cls *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
);

/** Add value to 40 bit Cluster Local Scratch.
 *
 * @param data     Value(s) to add
 * @param address  40 bit address in which to add values where upper 6 bits indicate the island.
 *                  See 6xxx databook for recommended addressing mode.
 * @param count    Number of 32-bit words to add values to
 * @param sync     Type of synchronization (sig_done or ctx_swap)
 * @param sig_ptr  Signal to be raised on completion
 *
 * Add the value in data to the 32-bit word at the specified address. Multiple
 * add operations (count > 1) can be performed in which case data needs to
 * point to an array of the appropriate size.
 *
 * If count is greater than 1, then the address needs to be QW aligned
 * otherwise it has to be LW aligned.
 *
 * @note Alignment is not currently enforced.
 */
__intrinsic
void cmd_cls_add_ptr40(
    __xwrite void *data,
    volatile void __addr40 __cls *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
);

/** Test and add value to Cluster Local Scratch memory address.
 *
 * @param val      Returned pre-modified value
 * @param data     Value(s) to add
 * @param address  Address of 32-bit word to which to add values
 * @param count    Number of 32-bit words to add values to (1 to 32)
 * @param sync     Type of synchronization (sig_done or ctx_swap)
 * @param sig_ptr  Signal to be raised on completion
 *
 * Add the value in data to the 32-bit word at the specified address. Multiple
 * add operations can be performed in which case data needs to
 * point to an array of the appropriate size.
 *
 * If count is greater than 1, then the address needs to be QW aligned
 * otherwise it has to be LW aligned.
 *
 * @note Alignment is not currently enforced.
 */
__intrinsic
void cmd_cls_test_and_add_ptr32(
    __xread uint32_t *val,             /* returned pre-modified value            */
    __xwrite uint32_t *data,           /* data to add                            */
    volatile void __cls *address,             /* address to add                         */
    uint32_t count,                                 /* number of words                        */
    sync_t sync,                                        /* must be sig_done                       */
    SIGNAL *sig_ptr                                     /* signal to raise upon completion        */
);

/** Test and add value to 40 bit Cluster Local Scratch memory address.
 *
 * @param val      Returned pre-modified value
 * @param data     Value(s) to add
 * @param address  40 bit address in which to add values where upper 6 bits indicate the island.
 *                  See 6xxx databook for recommended addressing mode.
 * @param count    Number of 32-bit words to add values to (1 to 32)
 * @param sync     Type of synchronization (sig_done or ctx_swap)
 * @param sig_ptr  Signal to be raised on completion
 *
 * Add the value in data to the 32-bit word at the specified address. Multiple
 * add operations can be performed in which case data needs to
 * point to an array of the appropriate size.
 *
 * If count is greater than 1, then the address needs to be QW aligned
 * otherwise it has to be LW aligned.
 *
 * @note Alignment is not currently enforced.
 */
__intrinsic
void cmd_cls_test_and_add_ptr40(
    __xread uint32_t *val,             /* returned pre-modified value            */
    __xwrite uint32_t *data,           /* data to add                            */
    volatile void __addr40 __cls *address,     /* address to add                         */
    uint32_t count,                                 /* number of words                        */
    sync_t sync,                                        /* must be sig_done                       */
    SIGNAL *sig_ptr                                     /* signal to raise upon completion        */
);


/** Add 64-bit value to Cluster Local Scratch memory address.
 *
 * @param data     Value(s) to add
 * @param address  Address of 64-bit word to which to add values
 * @param count    Number of 32-bit words to add values to and must be multiple of 2.
 * @param sync     Type of synchronization (sig_done or ctx_swap)
 * @param sig_ptr  Signal to be raised on completion
 *
 * Add the value in data to the 64-bit at the specified address using a
 * 64-bit add. Multiple add operations (count > 2) can be performed in which
 * case data needs to point to an array of the appropriate size.
 * Valid count values are 2, 4, 6, 8, 10, 12, 14 and 16.
 *
 * Address must be 64-bit aligned.
 *
 * @note Alignment is not currently enforced.
 */
__intrinsic
void cmd_cls_add64_ptr32(
    __xwrite void *data,
    volatile void __cls *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
);

/** Add 64-bit value to 40 bit Cluster Local Scratch memory address.
 *
 * @param data     Value(s) to add
 * @param address  40 bit address in which to add values where upper 6 bits indicate the island.
 *                  See 6xxx databook for recommended addressing mode.
 * @param count    Number of 32-bit words to add values to and must be multiple of 2.
 * @param sync     Type of synchronization (sig_done or ctx_swap)
 * @param sig_ptr  Signal to be raised on completion
 *
 * Add the value in data to the 64-bit at the specified address using a
 * 64-bit add. Multiple add operations (count > 2) can be performed in which
 * case data needs to point to an array of the appropriate size.
 * Valid count values are 2, 4, 6, 8, 10, 12, 14 and 16.
 *
 * Address must be 64-bit aligned.
 *
 * @note Alignment is not currently enforced.
 */
__intrinsic
void cmd_cls_add64_ptr40(
    __xwrite void *data,
    volatile void __addr40 __cls *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
);

/** Test and add  64-bit value to Cluster Local Scratch memory address.
 *
 * @param val      Returned pre-modified value
 * @param data     Value(s) to add
 * @param address  Address of 64-bit word to which to add values
 * @param count    Number of 32-bit words and must be multiple of 2 up to 16
 * @param sync     Type of synchronization (sig_done or ctx_swap)
 * @param sig_ptr  Signal to be raised on completion
 *
 * Add the value in data to the 64-bit at the specified address. Multiple
 * add operations can be performed in which case data needs to
 * point to an array of the appropriate size.
 *
 * If count is greater than 1, then the address needs to be QW aligned
 * otherwise it has to be LW aligned.
 *
 * Same as cls_add64 command, but also returns pre-modified
 * memory contents.
 * @see cls_add64()
 *
 * @note Alignment is not currently enforced.
 */
__intrinsic
void cmd_cls_test_and_add64_ptr32(
    __xread uint32_t *val,             /* returned pre-modified value            */
    __xwrite uint32_t *data,           /* data to add                            */
    volatile void __cls *address,             /* address to add                         */
    uint32_t count,                                 /* number of words                        */
    sync_t sync,                                        /* must be sig_done                       */
    SIGNAL *sig_ptr                                     /* signal to raise upon completion        */
);


/** Test and add 64-bit value to 40 bit Cluster Local Scratch memory address.
 *
 * @param val      Returned pre-modified value
 * @param data     Value(s) to add
 * @param address  40 bit address in which to add values where upper 6 bits indicate the island.
 *                  See 6xxx databook for recommended addressing mode.
 * @param count    Number of 32-bit words and must be multiple of 2 up to 16
 * @param sync     Type of synchronization (sig_done or ctx_swap)
 * @param sig_ptr  Signal to be raised on completion
 *
 * Add the value in data to the 64-bit word at the specified address. Multiple
 * add operations can be performed in which case data needs to
 * point to an array of the appropriate size.
 *
 * If count is greater than 1, then the address needs to be QW aligned
 * otherwise it has to be LW aligned.
 *
 * Same as cls_add64 command, but also returns pre-modified
 * memory contents.
 * @see cls_add64()
 *
 * @note Alignment is not currently enforced.
 *
 */
__intrinsic
void cmd_cls_test_and_add64_ptr40(
    __xread uint32_t *val,             /* returned pre-modified value            */
    __xwrite uint32_t *data,           /* data to add                            */
    volatile void __addr40 __cls *address,       /* address to add                         */
    uint32_t count,                                 /* number of words                        */
    sync_t sync,                                        /* must be sig_done                       */
    SIGNAL *sig_ptr                                     /* signal to raise upon completion        */
);

/** Add value to Cluster Local Scratch memory address with saturation.
 *
 * @param data     Value(s) to add
 * @param address  Address of 32-bit word to which to add values
 * @param count    Number of 32-bit words to add values to
 * @param sync     Type of synchronization (sig_done or ctx_swap)
 * @param sig_ptr  Signal to be raised on completion
 *
 * Add the value in data to the 32-bit word at the specified address. Multiple
 * add operations (count > 1) can be performed in which case data needs to
 * point to an array of the appropriate size.
 *
 * If the addition would overflow, the 32-bit word is set to 0xffffffff.
 *
 * If count is greater than 1, then the address needs to be QW aligned
 * otherwise it has to be LW aligned.
 *
 * @note Alignment is not currently enforced.
 */
__intrinsic
void cmd_cls_add_sat_ptr32(
    __xwrite void *data,
    volatile void __cls *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
);


/** Add value to 40 bit Cluster Local Scratch.
 *
 * @param data     Value(s) to add
 * @param address  40 bit address in which to add values where upper 6 bits indicate the island.
 *                 See 6xxx databook for recommended addressing mode.
 * @param max_nn    Number of 32-bit words to add values to
 * @param ind      Indirect word
 * @param sync     Type of synchronization (sig_done or ctx_swap)
 * @param sig_ptr  Signal to be raised on completion
 *
 * Add the value in data to the 32-bit word at the specified address. Multiple
 * add operations (max_nn > 1) can be performed in which case data needs to
 * point to an array of the appropriate size.
 *
 * If max_nn is greater than 1, then the address needs to be QW aligned
 * otherwise it has to be LW aligned.
 *
 * @note Alignment is not currently enforced.
 */

__intrinsic
void cmd_cls_add_ind_ptr40(
    __xwrite void *data,
    volatile void __addr40 __cls *address,
    uint32_t max_nn,
    generic_ind_t ind,
    sync_t sync,
    SIGNAL *sig_ptr
    );


/** Add value to 32 bit Cluster Local Scratch.
 *
 * @param data     Value(s) to add
 * @param address  Address of 32-bit word to which to add values
 * @param max_nn    Number of 32-bit words to add values to
 * @param ind      Indirect word
 * @param sync     Type of synchronization (sig_done or ctx_swap)
 * @param sig_ptr  Signal to be raised on completion
 *
 * Add the value in data to the 32-bit word at the specified address. Multiple
 * add operations (max_nn > 1) can be performed in which case data needs to
 * point to an array of the appropriate size.
 *
 * If max_nn is greater than 1, then the address needs to be QW aligned
 * otherwise it has to be LW aligned.
 *
 * @note Alignment is not currently enforced.
 */

__intrinsic
void cmd_cls_add_ind_ptr32(
    __xwrite void *data,
    volatile void __cls *address,
    uint32_t max_nn,
    generic_ind_t ind,
    sync_t sync,
    SIGNAL *sig_ptr
    );


/** Add the 64-bit value(s) in the transfer register pairs to the 64-bit value(s) at the specified address
 *
 * @param data     Value(s) to add
 * @param address  40 bit address in which to add values where upper 6 bits indicate the island.
 *                  See 6xxx databook for recommended addressing mode.
 * @param max_nn   Number of 32-bit words to add values to (2,4,6,8,10,12,14 or 16)
 * @param ind      Indirect word
 * @param sync     Type of synchronization (sig_done or ctx_swap)
 * @param sig_ptr  Signal to be raised on completion
 *
 * Add the value in data to the 64-bit word at the specified address. Multiple
 * add operations (max_nn > 1) can be performed in which case data needs to
 * point to an array of the appropriate size.
 *
 * The address needs to be QW aligned
 *
 * @note Alignment is not currently enforced.
 */

__intrinsic
void cmd_cls_add64_ind_ptr40(
    __xwrite void *data,
    volatile void __addr40 __cls *address,
    uint32_t max_nn,
    generic_ind_t ind,
    sync_t sync,
    SIGNAL *sig_ptr
    );

/** Add the 64-bit value(s) in the transfer register pairs to the 64-bit value(s) at the specified address
 *
 * @param data     Value(s) to add
 * @param address  Address of 64-bit word to which to add values
 * @param max_nn   Number of 32-bit words to add values to (2,4,6,8,10,12,14 or 16)
 * @param ind      Indirect word
 * @param sync     Type of synchronization (sig_done or ctx_swap)
 * @param sig_ptr  Signal to be raised on completion
 *
 * Add the value in data to the 64-bit word at the specified address. Multiple
 * add operations (max_nn > 1) can be performed in which case data needs to
 * point to an array of the appropriate size.
 *
 * The address needs to be QW aligned
 *
 * @note Alignment is not currently enforced.
 */
__intrinsic
void cmd_cls_add64_ind_ptr32(
    __xwrite void *data,
    volatile void __cls *address,
    uint32_t max_nn,
    generic_ind_t ind,
    sync_t sync,
    SIGNAL *sig_ptr
    );


/** Add value to 40 bit Cluster Local Scratch, with saturation limits.
 *
 * @param data     Value(s) to add
 * @param address  40 bit address in which to add values where upper 6 bits indicate the island.
 *                 See 6xxx databook for recommended addressing mode.
 * @param max_nn    Number of 32-bit words to add values to
 * @param ind      Indirect word
 * @param sync     Type of synchronization (sig_done or ctx_swap)
 * @param sig_ptr  Signal to be raised on completion
 *
 * Add the value in data to the 32-bit word at the specified address. Multiple
 * add operations (max_nn > 1) can be performed in which case data needs to
 * point to an array of the appropriate size.
 *
 * If max_nn is greater than 1, then the address needs to be QW aligned
 * otherwise it has to be LW aligned.
 *
 * @note Alignment is not currently enforced.
 */

__intrinsic
void cmd_cls_add_sat_ind_ptr40(
    __xwrite void *data,
    volatile void __addr40 __cls *address,
    uint32_t max_nn,
    generic_ind_t ind,
    sync_t sync,
    SIGNAL *sig_ptr
    );



/** Add value to 32 bit Cluster Local Scratch, with saturation limits.
 *
 * @param data     Value(s) to add
 * @param address  Address of 32-bit word to which to add values
 * @param max_nn   Number of 32-bit words to add values to
 * @param ind      Indirect word
 * @param sync     Type of synchronization (sig_done or ctx_swap)
 * @param sig_ptr  Signal to be raised on completion
 *
 * Add the value in data to the 32-bit word at the specified address. Multiple
 * add operations (max_nn > 1) can be performed in which case data needs to
 * point to an array of the appropriate size.
 *
 * If max_nn is greater than 1, then the address needs to be QW aligned
 * otherwise it has to be LW aligned.
 *
 * @note Alignment is not currently enforced.
 */
__intrinsic
void cmd_cls_add_sat_ind_ptr32(
    __xwrite void *data,
    volatile void __cls *address,
    uint32_t max_nn,
    generic_ind_t ind,
    sync_t sync,
    SIGNAL *sig_ptr
    );


/**
 * Subtract the 32-bit value(s) in the transfer register(s) from the 32-bit value(s) at the specified address.
 *
 * @param data     Value(s) to add
 * @param address  40 bit address in which to subtract values where upper 6 bits indicate the island.
 *                 See 6xxx databook for recommended addressing mode.
 * @param max_nn   Number of 32-bit words to subtract values from
 * @param ind      Indirect word
 * @param sync     Type of synchronization (sig_done or ctx_swap)
 * @param sig_ptr  Signal to be raised on completion
 *
 * Subtract the value in data from the 32-bit word at the specified address. Multiple
 * add operations (max_nn > 1) can be performed in which case data needs to
 * point to an array of the appropriate size.
 *
 * If max_nn is greater than 1, then the address needs to be QW aligned
 * otherwise it has to be LW aligned.
 *
 * @note Alignment is not currently enforced.
 */

__intrinsic
void cmd_cls_sub_ind_ptr40(
    __xwrite void *data,
    volatile void __addr40 __cls *address,
    uint32_t max_nn,
    generic_ind_t ind,
    sync_t sync,
    SIGNAL *sig_ptr
    );

/** Subtract value to 32 bit Cluster Local Scratch.
 *
 * @param data     Value(s) to subtract
 * @param address  Address of 32-bit word to which to subtract values
 * @param max_nn    Number of 32-bit words to subtract values from
 * @param ind      Indirect word
 * @param sync     Type of synchronization (sig_done or ctx_swap)
 * @param sig_ptr  Signal to be raised on completion
 *
 * Subtract the value in data from the 32-bit word at the specified address. Multiple
 * add operations (max_nn > 1) can be performed in which case data needs to
 * point to an array of the appropriate size.
 *
 * If max_nn is greater than 1, then the address needs to be QW aligned
 * otherwise it has to be LW aligned.
 *
 * @note Alignment is not currently enforced.
 */
__intrinsic
void cmd_cls_sub_ind_ptr32(
    __xwrite void *data,
    volatile void __cls *address,
    uint32_t max_nn,
    generic_ind_t ind,
    sync_t sync,
    SIGNAL *sig_ptr
    );


/** Subtract the 64-bit value(s) in the transfer register pairs to the 64-bit value(s) at the specified address
 *
 * @param data     Value(s) to subtract
 * @param address  40 bit address in which to add values where upper 6 bits indicate the island.
 *                 See 6xxx databook for recommended addressing mode.
 * @param max_nn    Number of 32-bit words to subtract values to (2,4,6,8,10,12,14 or 16)
 * @param ind      Indirect word
 * @param sync     Type of synchronization (sig_done or ctx_swap)
 * @param sig_ptr  Signal to be raised on completion
 *
 * Subtract the value in data to the 64-bit word at the specified address. Multiple
 * add operations (max_nn > 1) can be performed in which case data needs to
 * point to an array of the appropriate size.
 *
 * The address needs to be QW aligned
 *
 * @note Alignment is not currently enforced.
 */

__intrinsic
void cmd_cls_sub64_ind_ptr40(
    __xwrite void *data,
    volatile void __addr40 __cls *address,
    uint32_t max_nn,
    generic_ind_t ind,
    sync_t sync,
    SIGNAL *sig_ptr
    );

/** Subtract the 64-bit value(s) in the transfer register pairs to the 64-bit value(s) at the specified address
 *
 * @param data     Value(s) to subtract
 * @param address  Address of 64-bit word to which to subtract values
 * @param max_nn    Number of 32-bit word to add values to (2,4,6,8,10,12,14 or 16)
 * @param ind      Indirect word
 * @param sync     Type of synchronization (sig_done or ctx_swap)
 * @param sig_ptr  Signal to be raised on completion
 *
 * Subtract the value in data to the 64-bit word at the specified address. Multiple
 * add operations (max_nn > 1) can be performed in which case data needs to
 * point to an array of the appropriate size.
 *
 * The address needs to be QW aligned
 *
 * @note Alignment is not currently enforced.
 */
__intrinsic
void cmd_cls_sub64_ind_ptr32(
    __xwrite void *data,
    volatile void __cls *address,
    uint32_t max_nn,
    generic_ind_t ind,
    sync_t sync,
    SIGNAL *sig_ptr
    );


/** Subtract value to 40 bit Cluster Local Scratch, with saturation limits.
 *
 * @param data     Value(s) to sub
 * @param address  40 bit address in which to add values where upper 6 bits indicate the island.
 *                 See 6xxx databook for recommended addressing mode.
 * @param max_nn    Number of 32-bit words to subtract values to
 * @param ind      Indirect word
 * @param sync     Type of synchronization (sig_done or ctx_swap)
 * @param sig_ptr  Signal to be raised on completion
 *
 * Subtract the value in data to the 32-bit word at the specified address. Multiple
 * subtract operations (max_nn > 1) can be performed in which case data needs to
 * point to an array of the appropriate size.
 *
 * If max_nn is greater than 1, then the address needs to be QW aligned
 * otherwise it has to be LW aligned.
 *
 * @note Alignment is not currently enforced.
 */
__intrinsic
void cmd_cls_sub_sat_ind_ptr40(
    __xwrite void *data,
    volatile void __addr40 __cls *address,
    uint32_t max_nn,
    generic_ind_t ind,
    sync_t sync,
    SIGNAL *sig_ptr
    );


/** Subtract value to 32 bit Cluster Local Scratch, with saturation limits.
 *
 * @param data     Value(s) to subtract
 * @param address  Address of 32-bit word to which to subtract values
 * @param max_nn    Number of 32-bit words to subtract values to
 * @param ind      Indirect word
 * @param sync     Type of synchronization (sig_done or ctx_swap)
 * @param sig_ptr  Signal to be raised on completion
 *
 * Subtract the value in data to the 32-bit word at the specified address. Multiple
 * add operations (max_nn > 1) can be performed in which case data needs to
 * point to an array of the appropriate size.
 *
 * If max_nn is greater than 1, then the address needs to be QW aligned
 * otherwise it has to be LW aligned.
 *
 * @note Alignment is not currently enforced.
 */
__intrinsic
void cmd_cls_sub_sat_ind_ptr32(
    __xwrite void *data,
    volatile void __cls *address,
    uint32_t max_nn,
    generic_ind_t ind,
    sync_t sync,
    SIGNAL *sig_ptr
    );


/** Add value to 40 bit Cluster Local Scratch memory address with saturation.
 *
 * @param data     Value(s) to add
 * @param address  40 bit address in which to add values where upper 6 bits indicate the island.
 *                  See 6xxx databook for recommended addressing mode.
 * @param count    Number of 32-bit words to add values to
 * @param sync     Type of synchronization (sig_done or ctx_swap)
 * @param sig_ptr  Signal to be raised on completion
 *
 * Add the value in data to the 32-bit word at the specified address. Multiple
 * add operations (count > 1) can be performed in which case data needs to
 * point to an array of the appropriate size.
 *
 * If the addition would overflow, the 32-bit word is set to 0xffffffff.
 *
 * If count is greater than 1, then the address needs to be QW aligned
 * otherwise it has to be LW aligned.
 *
 * @note Alignment is not currently enforced.
 */
__intrinsic
void cmd_cls_add_sat_ptr40(
    __xwrite void *data,
    volatile void __addr40 __cls *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
);


/** Add an immediate value to Cluster Local Scratch memory address.
 *
 * @param address  Address of 32-bit word to which to add values
 * @param value    Value to add
 *
 * Add the value in data to the 32-bit word at the specified address. If the
 * value is less than 3 bits it is directly encoded in the instruction,
 * otherwise a indirect reference is used. The value can be up to 14 bits
 * long.
 *
 * This function accepts values for up to 16 bits with bit 14 being
 * used as an optional sign extension. If bit 14 is 0 no sign extension
 * is used. If bit 14 is set, the sign of value is extended to 32-bit
 * before the operation is performed
 *
 * Bit 15 of value is ignored.
 */
__intrinsic
void cmd_cls_add_imm_ptr32(
    volatile void __cls *address,
    uint32_t value
);


/** Add an immediate value to 40 bit Cluster Local Scratch  memory address.
 *
 * @param address  40 bit address in which to add values where upper 6 bits indicate the island.
 *                  See 6xxx databook for recommended addressing mode.
 * @param value    Value to add
 *
 * Add the value in data to the 32-bit word at the specified address. If the
 * value is less than 3 bits it is directly encoded in the instruction,
 * otherwise a indirect reference is used. The value can be up to 14 bits
 * long.
 *
 * This function accepts values for up to 16 bits with bit 14 being
 * used as an optional sign extension. If bit 14 is 0 no sign extension
 * is used. If bit 14 is set, the sign of value is extended to 32-bit
 * before the operation is performed
 *
 * Bit 15 of value is ignored.
 */
__intrinsic
void cmd_cls_add_imm_ptr40(
    volatile void __addr40 __cls *address,
    uint32_t value
);

/** Add an immediate value to Cluster Local Scratch memory address with
 * saturation.
 *
 * @param address  Address of 32-bit word to which to add values
 * @param value    Value to add
 *
 * Add the value in data to the 32-bit word at the specified address. If the
 * value is less than 3 bits it is directly encoded in the instruction,
 * otherwise a indirect reference is used. If the addition overflows the
 * 32-bit word is set to 0xffffffff. Values can be up to 14 bits long.
 *
 * This function accepts values for up to 16 bits with bit 14 being
 * used as an optional sign extension. If bit 14 is 0 no sign extension
 * is used. If bit 14 is set, the sign of value is extended to 32-bit
 * before the operation is performed
 *
 * Bit 15 of value is ignored.
 *
 */
__intrinsic
void cmd_cls_add_imm_sat_ptr32(
    volatile void __cls *address,
    uint32_t value
);


/** Add an immediate value to 40 bit Cluster Local Scratch memory address with
 * saturation.
 *
 * @param address  40 bit address in which to add values where upper 6 bits indicate the island.
 *                  See 6xxx databook for recommended addressing mode.
 * @param value    Value to add
 *
 * Add the value in data to the 32-bit word at the specified address. If the
 * value is less than 3 bits it is directly encoded in the instruction,
 * otherwise a indirect reference is used. If the addition overflows the
 * 32-bit word is set to 0xffffffff. Values can be up to 14 bits long.
 *
 * This function accepts values for up to 16 bits with bit 14 being
 * used as an optional sign extension. If bit 14 is 0 no sign extension
 * is used. If bit 14 is set, the sign of value is extended to 32-bit
 * before the operation is performed
 *
 * Bit 15 of value is ignored.
 *
 */
__intrinsic
void cmd_cls_add_imm_sat_ptr40(
    volatile void __addr40 __cls *address,
    uint32_t value
);

/** Add an immediate value to Cluster Local Scratch 64-bit memory location.
 *
 * @param address  Address of 64-bit word to which to add values
 * @param value    Value to add
 *
 * Add the value in data to the 64-bit word at the specified address. If the
 * value is less than 3 bits it is directly encoded in the instruction,
 * otherwise a indirect reference is used. The value can be up to 14 bits
 * long.
 *
 * This function accepts values for up to 16 bits with the top 2 bits being
 * used as an optional sign extension. If the top two bits are 00 no sign
 * extension is used. The top bits set to 01 indicate sign extend to 64-bit
 * value, top bits set to 10 indicate no sign extension but duplicate data in top
 * and bottom fields, top bits equal 11 indicate two sign extended 32-bit
 * values.
 */
__intrinsic
void cmd_cls_add64_imm_ptr32(
    volatile void __cls *address,
    uint32_t value
);

/** Add an immediate value to 40 bit Cluster Local Scratch 64-bit memory location.
 *
 * @param address  40 bit address in which to add values where upper 6 bits indicate the island.
 *                  See 6xxx databook for recommended addressing mode.
 * @param value    Value to add
 *
 * Add the value in data to the 64-bit word at the specified address. If the
 * value is less than 3 bits it is directly encoded in the instruction,
 * otherwise a indirect reference is used. The value can be up to 14 bits
 * long.
 *
 * This function accepts values for up to 16 bits with the top 2 bits being
 * used as an optional sign extension. If the top two bits are 00 no sign
 * extension is used. The top bits set to 01 indicate sign extend to 64-bit
 * value, top bits set to 10 indicate no sign extension but duplicate data in top
 * and bottom fields, top bits equal 11 indicate two sign extended 32-bit
 * values.
 */
__intrinsic
void cmd_cls_add64_imm_ptr40(
    volatile void __addr40 __cls *address,
    uint32_t value
);

/** Local scratch memory test and add immediate.
 *
 * @param val           Returned pre-modified value
 * @param address       Cluster Local Scratch address to add to
 * @param value         Immediate value to add (1 - 31)
 * @param sync          Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr       Signal to raise upon completion
 *
 * Same as cmd_cls_add_imm_ptr32 command, but also returns pre-modified
 * memory contents.
 * @see cmd_cls_add_imm_ptr32()
 *
 */
__intrinsic
void cmd_cls_test_and_add_imm_ptr32(
    __xread uint32_t *val,
    volatile void __cls *address,
    uint32_t value,
    sync_t sync,
    SIGNAL *sig_ptr
);

/** Local scratch memory test and add immediate.
 *
 * @param val           Returned pre-modified value
 * @param address       40 bit address in which to add values where upper 6 bits indicate the island.
 *                       See 6xxx databook for recommended addressing mode.
 * @param value         Immediate value to add (1 - 31)
 * @param sync          Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr       Signal to raise upon completion
 *
 * Same as cmd_cls_add_imm_ptr32 command, but also returns pre-modified
 * memory contents.
 * @see cmd_cls_add_imm_ptr40()
 *
 */
__intrinsic
void cmd_cls_test_and_add_imm_ptr40(
    __xread uint32_t *val,
    volatile void __addr40 __cls *address,
    uint32_t value,
    sync_t sync,
    SIGNAL *sig_ptr
);

/** Local scratch memory test and add immediate to 64bit location.
 *
 * @param val           Returned pre-modified value
 * @param address       Cluster Local Scratch address to add to
 * @param value         Immediate value to add (1 - 31)
 * @param sync          Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr       Signal to raise upon completion
 *
 * Same as cls_add_imm64 command, but also returns pre-modified
 * memory contents.
 * @see cmd_cls_add64_imm_ptr32()
 *
 */
__intrinsic
void cmd_cls_test_and_add64_imm_ptr32(
    __xread uint32_t *val,
    volatile void __cls *address,
    uint32_t value,
    sync_t sync,
    SIGNAL *sig_ptr
);

/** Local scratch memory test and add immediate to 64bit location.
 *
 * @param val           Returned pre-modified value
 * @param address       40 bit address in which to add values where upper 6 bits indicate the island.
 *                       See 6xxx databook for recommended addressing mode.
 * @param value         Immediate value to add (1 - 31)
 * @param sync          Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr       Signal to raise upon completion
 *
 * Same as cls_add_imm64 command, but also returns pre-modified
 * memory contents.
 * @see cmd_cls_add64_imm_ptr40()
 *
 */
__intrinsic
void cmd_cls_test_and_add64_imm_ptr40(
    __xread uint32_t *val,
    volatile void __addr40 __cls *address,
    uint32_t value,
    sync_t sync,
    SIGNAL *sig_ptr
);

/** Subtract value from Cluster Local Scratch memory address.
 *
 * @param data     Value(s) to subtract
 * @param address  Address of 32-bit word from which to subtract values
 * @param count    Number of 32-bit words to subtract values from
 * @param sync     Type of synchronization (sig_done or ctx_swap)
 * @param sig_ptr  Signal to be raised on completion
 *
 * Subtract the value in data from the 32-bit word at the specified
 * address. Multiple subtract operations (count > 1) can be performed in
 * which case data needs to point to an array of the appropriate size.
 *
 * If count is greater than 1, then the address needs to be QW aligned
 * otherwise it has to be LW aligned.
 *
 * @note Alignment is not currently enforced.
 */
__intrinsic
void cmd_cls_sub_ptr32(
    __xwrite void *data,
    volatile void __cls *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
);


/** Subtract value from 40 bit Cluster Local Scratch  memory address.
 *
 * @param data     Value(s) to subtract
 * @param address  40 bit address in which to subtract values where upper 6 bits indicate the island.
 *                  See 6xxx databook for recommended addressing mode.
 * @param count    Number of 32-bit words to subtract values from
 * @param sync     Type of synchronization (sig_done or ctx_swap)
 * @param sig_ptr  Signal to be raised on completion
 *
 * Subtract the value in data from the 32-bit word at the specified
 * address. Multiple subtract operations (count > 1) can be performed in
 * which case data needs to point to an array of the appropriate size.
 *
 * If count is greater than 1, then the address needs to be QW aligned
 * otherwise it has to be LW aligned.
 *
 * @note Alignment is not currently enforced.
 */
__intrinsic
void cmd_cls_sub_ptr40(
    __xwrite void *data,
    volatile void __addr40 __cls *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
);



/** Read 32-bit words from Cluster Local Scratch in Little Endian mode
 *  in indirect mode.
 *
 * @param data     Read Transfer Registers the read data will be placed in
 * @param address  40 Bit address to read from
 * @param max_nn    Maximum number of 32-bit words to read (1-32)
 * @param ind      Indirection type
 * @param sync     Type of synchronization (sig_done or ctx_swap)
 * @param sig_ptr  Signal to be raised on completion
 *
 * @see cls_read_ind()
 */

__intrinsic
void cmd_cls_read_le_ind_ptr40(
    __xread void *data,
    volatile void __addr40 __cls *address,
    uint32_t max_nn,
    generic_ind_t ind,
    sync_t sync,
    SIGNAL *sig_ptr
);

/** Read 32-bit words from Cluster Local Scratch in Little Endian mode
 *  in indirect mode.
 *
 * @param data     Read Transfer Registers the read data will be placed in
 * @param address  32 Bit address to read from
 * @param max_nn    Maximum number of 32-bit words to read (1-32)
 * @param ind      Indirection type
 * @param sync     Type of synchronization (sig_done or ctx_swap)
 * @param sig_ptr  Signal to be raised on completion
 *
 * @see cls_read_ind()
 */
__intrinsic
void cmd_cls_read_le_ind_ptr32(
    __xread void *data,
    volatile void __cls *address,
    uint32_t max_nn,
    generic_ind_t ind,
    sync_t sync,
    SIGNAL *sig_ptr
);


/** Read 32-bit words from Cluster Local Scratch in Big Endian mode
 *  in indirect mode.
 *
 * @param data     Read Transfer Registers the read data will be placed in
 * @param address  40 Bit address to read from
 * @param max_nn    Maximum number of 32-bit words to read (1-32)
 * @param ind      Indirection type
 * @param sync     Type of synchronization (sig_done or ctx_swap)
 * @param sig_ptr  Signal to be raised on completion
 *
 * @see cls_read_ind()
 */

__intrinsic
void cmd_cls_read_be_ind_ptr40(
    __xread void *data,
    volatile void __addr40 __cls *address,
    uint32_t max_nn,
    generic_ind_t ind,
    sync_t sync,
    SIGNAL *sig_ptr
);

/** Read 32-bit words from Cluster Local Scratch in Big Endian mode
 *  in indirect mode.
 *
 * @param data     Read Transfer Registers the read data will be placed in
 * @param address  32 Bit address to read from
 * @param max_nn    Maximum number of 32-bit words to read (1-32)
 * @param ind      Indirection type
 * @param sync     Type of synchronization (sig_done or ctx_swap)
 * @param sig_ptr  Signal to be raised on completion
 *
 * @see cls_read_ind()
 */
__intrinsic
void cmd_cls_read_be_ind_ptr32(
    __xread void *data,
    volatile void __cls *address,
    uint32_t max_nn,
    generic_ind_t ind,
    sync_t sync,
    SIGNAL *sig_ptr
);



/** Read 32-bit words from Cluster Local Scratch in Big Endian mode
 *  in indirect mode.
 *
 * @param data     Read Transfer Registers the read data will be placed in
 * @param address  40 Bit address to read from
 * @param max_nn    Maximum number of 32-bit words to read (1-32)
 * @param ind      Indirection type
 * @param sync     Type of synchronization (sig_done or ctx_swap)
 * @param sig_ptr  Signal to be raised on completion
 *
 * @see cls_read_ind()
 */

__intrinsic
void cmd_cls_read_ind_ptr40(
    __xread void *data,
    volatile void __addr40 __cls *address,
    uint32_t max_nn,
    generic_ind_t ind,
    sync_t sync,
    SIGNAL *sig_ptr
);

/** Read 32-bit words from Cluster Local Scratch, endian mode according compiler option
 *  in indirect mode.
 *
 * @param data     Read Transfer Registers the read data will be placed in
 * @param address  32 Bit address to read from
 * @param max_nn    Maximum number of 32-bit words to read (1-32)
 * @param ind      Indirection type
 * @param sync     Type of synchronization (sig_done or ctx_swap)
 * @param sig_ptr  Signal to be raised on completion
 *
 * @see cls_read_ind()
 */
__intrinsic
void cmd_cls_read_ind_ptr32(
    __xread void *data,
    volatile void __cls *address,
    uint32_t max_nn,
    generic_ind_t ind,
    sync_t sync,
    SIGNAL *sig_ptr
);




/** Write 32-bit words to Cluster Local Scratch in Little Endian mode
 *  in indirect mode.
 *
 * @param data     Write Transfer Registers data to write to Cluster Local Scratch
 * @param address  40 Bit address to write to
 * @param max_nn    Maximum number of 32-bit words to write (1-32)
 * @param ind      Indirection type
 * @param sync     Type of synchronization (sig_done or ctx_swap)
 * @param sig_ptr  Signal to be raised on completion
 *
 * @see cls_write_ind()
 */

__intrinsic
void cmd_cls_write_le_ind_ptr40(
    __xwrite void *data,
    volatile void __addr40 __cls *address,
    uint32_t max_nn,
    generic_ind_t ind,
    sync_t sync,
    SIGNAL *sig_ptr
);


/** Write 32-bit words to Cluster Local Scratch in Little Endian mode
 *  in indirect mode.
 *
 * @param data     Write Transfer Registers data to write to Cluster Local Scratch
 * @param address  32 Bit address to write to
 * @param max_nn    Maximum number of 32-bit words to write (max_1 - max_32)
 * @param ind      Indirection type
 * @param sync     Type of synchronization (sig_done or ctx_swap)
 * @param sig_ptr  Signal to be raised on completion
 *
 * @see cls_write_ind()
 */
__intrinsic
void cmd_cls_write_le_ind_ptr32(
    __xwrite void *data,
    volatile void __cls *address,
    uint32_t max_nn,
    generic_ind_t ind,
    sync_t sync,
    SIGNAL *sig_ptr
);


/** Write 32-bit words to Cluster Local Scratch in Big Endian mode
 *  in indirect mode.
 *
 * @param data     Write Transfer Registers data to write to Cluster Local Scratch
 * @param address  40 Bit address to write to
 * @param max_nn    Maximum number of 32-bit words to write (1-32)
 * @param ind      Indirection type
 * @param sync     Type of synchronization (sig_done or ctx_swap)
 * @param sig_ptr  Signal to be raised on completion
 *
 * @see cls_write_ind()
 */

__intrinsic
void cmd_cls_write_be_ind_ptr40(
    __xwrite void *data,
    volatile void __addr40 __cls *address,
    uint32_t max_nn,
    generic_ind_t ind,
    sync_t sync,
    SIGNAL *sig_ptr
);


/** Write 32-bit words to Cluster Local Scratch in Big Endian mode
 *  in indirect mode.
 *
 * @param data     Write Transfer Registers data to write to Cluster Local Scratch
 * @param address  32 Bit address to write to
 * @param max_nn    Maximum number of 32-bit words to write (max_1 - max_32)
 * @param ind      Indirection type
 * @param sync     Type of synchronization (sig_done or ctx_swap)
 * @param sig_ptr  Signal to be raised on completion
 *
 * @see cls_write_ind()
 */
__intrinsic
void cmd_cls_write_be_ind_ptr32(
    __xwrite void *data,
    volatile void __cls *address,
    uint32_t max_nn,
    generic_ind_t ind,
    sync_t sync,
    SIGNAL *sig_ptr
);



/** Write 32-bit words to Cluster Local Scratch in Big or Little Endian format
 *  in indirect mode.
 *
 * @param data     Write Transfer Registers holds the data to be written
 * @param address  40 Bit address to write to
 * @param max_nn    Maximum number of 32-bit words to write (1-32)
 * @param ind      Indirection type
 * @param sync     Type of synchronization (sig_done or ctx_swap)
 * @param sig_ptr  Signal to be raised on completion
 *
 * @see cls_write_ind()
 */

__intrinsic
void cmd_cls_write_ind_ptr40(
    __xwrite void *data,
    volatile void __addr40 __cls *address,
    uint32_t max_nn,
    generic_ind_t ind,
    sync_t sync,
    SIGNAL *sig_ptr
);


/** Write 32-bit words to Cluster Local Scratch in Big or Little Endian format
 *  in indirect mode.
 *
 * @param data     Write Transfer Registers holds the data to be written
 * @param address  32 Bit address to write to
 * @param max_nn    Maximum number of 32-bit words to write (1-32)
 * @param ind      Indirection type
 * @param sync     Type of synchronization (sig_done or ctx_swap)
 * @param sig_ptr  Signal to be raised on completion
 *
 * @see cls_write_ind()
 */
__intrinsic
void cmd_cls_write_ind_ptr32(
    __xwrite void *data,
    volatile void __cls *address,
    uint32_t max_nn,
    generic_ind_t ind,
    sync_t sync,
    SIGNAL *sig_ptr
);


/** Test and subtract value to Cluster Local Scratch memory address.
 *
 * @param val      Returned pre-modified value
 * @param data     Value(s) to subtract
 * @param address  Address of 32-bit word to which to add values
 * @param count    Number of 32-bit words to subtract values (1 to 32)
 * @param sync     Type of synchronization (sig_done or ctx_swap)
 * @param sig_ptr  Signal to be raised on completion
 *
 * Subtract the value in data to the 32-bit word at the specified address. Multiple
 * subtract operations can be performed in which case data needs to
 * point to an array of the appropriate size.
 *
 * If count is greater than 1, then the address needs to be QW aligned
 * otherwise it has to be LW aligned.
 *
 * Same as cmd_cls_sub_ptr32 command, but also returns pre-modified
 * memory contents.
 * @see cmd_cls_sub_ptr32()
 *
 * @note Alignment is not currently enforced.
 */
__intrinsic
void cmd_cls_test_and_sub_ptr32(
    __xread uint32_t *val,             /* returned pre-modified value            */
    __xwrite uint32_t *data,           /* data to subtract                       */
    volatile void __cls *address,             /* address to subtract from               */
    uint32_t count,                                 /* number of words                        */
    sync_t sync,                                        /* sig_done or ctx_swap                       */
    SIGNAL *sig_ptr                                     /* signal to raise upon completion        */
);


/** Test and subtract value from 40 bit Cluster Local Scratch memory address.
 *
 * @param val      Returned pre-modified value
 * @param data     Value(s) to subtract
 * @param address  40 bit address in which to subtract values where upper 6 bits indicate the island.
 *                  See 6xxx databook for recommended addressing mode.
 * @param count    Number of 32-bit words to subtract values (1 to 32)
 * @param sync     Type of synchronization (sig_done or ctx_swap)
 * @param sig_ptr  Signal to be raised on completion
 *
 * Subtract the value in data to the 32-bit word at the specified address. Multiple
 * subtract operations can be performed in which case data needs to
 * point to an array of the appropriate size.
 *
 * If count is greater than 1, then the address needs to be QW aligned
 * otherwise it has to be LW aligned.
 *
 * Same as cmd_cls_sub_ptr32 command, but also returns pre-modified
 * memory contents.
 * @see cmd_cls_sub_ptr40()
 *
 * @note Alignment is not currently enforced.
 */
__intrinsic
void cmd_cls_test_and_sub_ptr40(
    __xread uint32_t *val,
    __xwrite uint32_t *data,
    volatile void __addr40 __cls *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
);

/** Subtract value from Cluster Local Scratch 64-bit memory location.
 *
 * @param data     Value(s) to subtract
 * @param address  Address of 64-bit word from which to subtract values
 * @param count    Number of 32-bit words to subtract values from (2, 4, .. 16)
 * @param sync     Type of synchronization (sig_done or ctx_swap)
 * @param sig_ptr  Signal to be raised on completion
 *
 * Subtract the 64-bit value(s) in data from the 64-bit word at the specified
 * address. Multiple subtract operations (count > 2 and must be multiple of 2)
 * can be performed in which case data needs to point to an array of the appropriate size.
 *
 * Only up to 8 32-bit words are supported by passing it directly.
 * More than 8 32-bit words subtract operations will be handled in indirect format.
 *
 * Address must be 64-bit aligned.
 *
 * @note Alignment is not currently enforced.
 */
__intrinsic
void cmd_cls_sub64_ptr32(
    __xwrite void *data,
    volatile void __cls *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
);

/** Subtract value from 40 bit Cluster Local Scratch 64bit memory location.
 *
 * @param data     Value(s) to subtract
 * @param address  40 bit address in which to subtract values where upper 6 bits indicate the island.
 *                  See 6xxx databook for recommended addressing mode.
 * @param count    Number of 32-bit words to subtract values from (2, 4, .. 16)
 * @param sync     Type of synchronization (sig_done or ctx_swap)
 * @param sig_ptr  Signal to be raised on completion
 *
 * Subtract the 64-bit value(s) in data from the 64-bit word at the specified
 * address. Multiple subtract operations (count > 2 and must be multiple of 2)
 * can be performed in which case data needs to point to an array of the appropriate size.
 *
 * Only up to 8 32-bit words are supported by passing it directly.
 * More than 8 32-bit words subtract operations will be handled in indirect format.
 *
 * Address must be 64-bit aligned.
 *
 * @note Alignment is not currently enforced.
 */
__intrinsic
void cmd_cls_sub64_ptr40(
    __xwrite void *data,
    volatile void __addr40 __cls *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
);


/** Test and subtract 64-bit value to Cluster Local Scratch memory address.
 *
 * @param val      Returned pre-modified value
 * @param data     Value(s) to add
 * @param address  Address of 64-bit word from which to subtract values
 * @param count    Number of 32-bit words and must be multiple of 2 up to 16
 * @param sync     Type of synchronization (sig_done or ctx_swap)
 * @param sig_ptr  Signal to be raised on completion
 *
 * Subtract the value in data to the 64-bit word at the specified address. Multiple
 * subtract operations can be performed in which case data needs to
 * point to an array of the appropriate size.
 *
 * If count is greater than 1, then the address needs to be QW aligned
 * otherwise it has to be LW aligned.
 *
 * Same as cmd_cls_sub64_ptr32 command, but also returns pre-modified
 * memory contents.
 * @see cmd_cls_sub64_ptr32()
 *
 * @note Alignment is not currently enforced.
 */
__intrinsic
void cmd_cls_test_and_sub64_ptr32(
    __xread uint32_t *val,             /* returned pre-modified value            */
    __xwrite uint32_t *data,           /* data to add                            */
    volatile void __cls *address,             /* address to add                         */
    uint32_t count,                                 /* number of words                        */
    sync_t sync,                                        /* sig_done or ctx_swap                     */
    SIGNAL *sig_ptr                                     /* signal to raise upon completion        */
);


/** Test and subtract 64-bit value to Cluster Local Scratch memory address.
 *
 * @param val      Returned pre-modified value
 * @param data     Value(s) to add
 * @param address  40 bit address in which to subtract values where upper 6 bits indicate the island.
 *                 See 6xxx databook for recommended addressing mode.
 * @param count    Number of 32-bit words and must be multiple of 2 up to 16
 * @param sync     Type of synchronization (sig_done or ctx_swap)
 * @param sig_ptr  Signal to be raised on completion
 *
 * Subtract the value in data from the 64-bit word at the specified address. Multiple
 * subtract operations can be performed in which case data needs to
 * point to an array of the appropriate size.
 *
 * If count is greater than 1, then the address needs to be QW aligned
 * otherwise it has to be LW aligned.
 *
 * Same as cmd_cls_sub64_ptr40 command, but also returns pre-modified
 * memory contents.
 * @see cmd_cls_sub64_ptr40()
 *
 * @note Alignment is not currently enforced.
 */
__intrinsic
void cmd_cls_test_and_sub64_ptr40(
    __xread uint32_t *val,
    __xwrite uint32_t *data,
    volatile void __addr40 __cls *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
);


/** Subtract value from Cluster Local Scratch memory address with saturation.
 *
 * @param data     Value(s) to subtract
 * @param address  Address of 32-bit word from which to subtract values
 * @param count    Number of 32-bit words to subtract values from
 * @param sync     Type of synchronization (sig_done or ctx_swap)
 * @param sig_ptr  Signal to be raised on completion
 *
 * Subtract the value in data from the 32-bit word at the specified
 * address. Multiple subtract operations (count > 1) can be performed in
 * which case data needs to point to an array of the appropriate size.
 *
 * If the subtraction underflows the value of the 32-bit word will be set to 0x0.
 *
 * If count is greater than 1, then the address needs to be QW aligned
 * otherwise it has to be LW aligned.
 *
 * @note Alignment is not currently enforced.
 */
__intrinsic
void cmd_cls_sub_sat_ptr32(
    __xwrite void *data,
    volatile void __cls *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
);

/** Subtract value from 40 bit Cluster Local Scratch memory address with saturation.
 *
 * @param data     Value(s) to subtract
 * @param address  40 bit address in which to subtract values where upper 6 bits indicate the island.
 *                  See 6xxx databook for recommended addressing mode.
 * @param count    Number of 32-bit words to subtract values from
 * @param sync     Type of synchronization (sig_done or ctx_swap)
 * @param sig_ptr  Signal to be raised on completion
 *
 * Subtract the value in data from the 32-bit word at the specified
 * address. Multiple subtract operations (count > 1) can be performed in
 * which case data needs to point to an array of the appropriate size.
 *
 * If the subtraction underflows the value of the 32-bit word will be set to 0x0.
 *
 * If count is greater than 1, then the address needs to be QW aligned
 * otherwise it has to be LW aligned.
 *
 * @note Alignment is not currently enforced.
 */
__intrinsic
void cmd_cls_sub_sat_ptr40(
    __xwrite void *data,
    volatile void __addr40 __cls *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
);


/** Subtract an immediate value from Cluster Local Scratch memory address.
 *
 * @param address  Address of 32-bit word to which to add values
 * @param value    Value to subtract
 *
 * Subtract the value in data from the 32-bit word at the specified
 * address. If the value is less than 3 bits it is directly encoded in the
 * instruction, otherwise a indirect reference is used. Values up to 14
 * bits can be passed in.
 *
 * This function accepts values for up to 16 bits with bit 14 being
 * used as an optional sign extension. If bit 14 is 0, no sign extension
 * is used. If bit 14 is set, the sign of the value is extended to 32-bit
 * before performing the subtraction.
 *
 * Bit 15 of value is ignored.
 */
__intrinsic
void cmd_cls_sub_imm_ptr32(
    volatile void __cls *address,
    uint32_t value
);


/** Subtract an immediate value from 40 bit Cluster Local Scratch memory address.
 *
 * @param address  40 bit address in which to subtract values where upper 6 bits indicate the island.
 *                  See 6xxx databook for recommended addressing mode.
 * @param value    Value to subtract
 *
 * Subtract the value in data from the 32-bit word at the specified
 * address. If the value is less than 3 bits it is directly encoded in the
 * instruction, otherwise a indirect reference is used. Values up to 14
 * bits can be passed in.
 *
 * This function accepts values for up to 16 bits with bit 14 being
 * used as an optional sign extension. If bit 14 is 0, no sign extension
 * is used. If bit 14 is set, the sign of the value is extended to 32-bit
 * before performing the subtraction.
 *
 * Bit 15 of value is ignored.
 */
__intrinsic
void cmd_cls_sub_imm_ptr40(
    volatile void __addr40 __cls *address,
    uint32_t value
);


/** Sub a immediate value from Cluster Local Scratch memory address with
 * saturation.
 *
 * @param address  Address of 32-bit word to which to subtract value
 * @param value    Value to subtract
 *
 * Subtract the value in data to the 32-bit word at the specified address.
 * If the value is less than 3 bits it is directly encoded in the
 * instruction, otherwise a indirect reference is used. If the subtraction
 * underflows the 32-bit word is set to 0x0. Values subtracted can be up to
 * 14 bits long.
 *
 * This function accepts values for up to 16 bits with bit 14 being
 * used as an optional sign extension. If bit 14 is 0, no sign extension
 * is used. If bit 14 is set, the sign of the value is extended to 32-bit
 * before performing the subtraction.
 *
 * Bit 15 of value is ignored.
 */
__intrinsic
void cmd_cls_sub_imm_sat_ptr32(
    volatile void __cls *address,
    uint32_t value
);

/** Sub a immediate value from 40 bit Cluster Local Scratch memory address with
 * saturation.
 *
 * @param address  40 bit address in which to subtract values where upper 6 bits indicate the island.
 *                  See 6xxx databook for recommended addressing mode.
 * @param value    Value to subtract
 *
 * Subtract the value in data to the 32-bit word at the specified address.
 * If the value is less than 3 bits it is directly encoded in the
 * instruction, otherwise a indirect reference is used. If the subtraction
 * underflows the 32-bit word is set to 0x0. Values subtracted can be up to
 * 14 bits long.
 *
 * This function accepts values for up to 16 bits with bit 14 being
 * used as an optional sign extension. If bit 14 is 0, no sign extension
 * is used. If bit 14 is set, the sign of the value is extended to 32-bit
 * before performing the subtraction.
 *
 * Bit 15 of value is ignored.
 */
__intrinsic
void cmd_cls_sub_imm_sat_ptr40(
    volatile void __addr40 __cls *address,
    uint32_t value
);

/** Subtract an immediate value from 64-bit Cluster Local Scratch memory
 * address.
 *
 * @param address  Address of 64-bit word to which to add values
 * @param value    Value to subtract
 *
 * Subtract the value in data from the 64-bit word at the specified
 * address. If the value is less than 3 bits, it is directly encoded in the
 * instruction, otherwise a indirect reference is used. Values can be up to
 * 14 bits long.
 *
 * Address needs to be 64-bit aligned.
 *
 * This function accepts values for up to 16 bits with the top 2 bits being
 * used as an optional sign extension. If the top two bits are 00, no sign
 * extension is used. The top bits set to 01 indicate sign extend to 64-bit
 * value, top bits 10 indicate no sign extension but duplicate data in top
 * and bottom fields, top bits equal 11 indicate two sign extended 32-bit
 * values.
 */
__intrinsic
void cmd_cls_sub64_imm_ptr32(
    volatile void __cls *address,
    uint32_t value
);

/** Subtract an immediate value from 64-bit Cluster Local Scratch memory
 * address.
 *
 * @param address  40 bit address in which to subtract values where upper 6 bits indicate the island.
 *                  See 6xxx databook for recommended addressing mode.
 * @param value    Value to subtract
 *
 * Subtract the value in data from the 64-bit word at the specified
 * address. If the value is less than 3 bits, it is directly encoded in the
 * instruction, otherwise a indirect reference is used. Values can be up to
 * 14 bits long.
 *
 * Address needs to be 64-bit aligned.
 *
 * This function accepts values for up to 16 bits with the top 2 bits being
 * used as an optional sign extension. If the top two bits are 00, no sign
 * extension is used. The top bits set to 01 indicate sign extend to 64-bit
 * value, top bits 10 indicate no sign extension but duplicate data in top
 * and bottom fields, top bits equal 11 indicate two sign extended 32-bit
 * values.
 */
__intrinsic
void cmd_cls_sub64_imm_ptr40(
    volatile void __addr40 __cls *address,
    uint32_t value
);

/** Local scratch memory test and subtract immediate.
 *
 * @param val           Returned pre-modified value
 * @param address       Cluster Local Scratch address to subtract from
 * @param value         Immediate value to subtract (1 - 31)
 * @param sync          Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr       Signal to raise upon completion
 *
 * Same as cmd_cls_sub_imm_ptr32 command, but also returns pre-modified
 * memory contents.
 * @see cmd_cls_sub_imm_ptr32()
 *
 */
__intrinsic
void cmd_cls_test_and_sub_imm_ptr32(
    __xread uint32_t *val,
    volatile void __cls *address,
    uint32_t value,
    sync_t sync,
    SIGNAL *sig_ptr
);


/** 40-bit Local scratch memory test and subtract immediate.
 *
 * @param val           Returned pre-modified value
 * @param address       40 bit address in which to subtract values where upper 6 bits indicate the island.
 *                       See 6xxx databook for recommended addressing mode.
 * @param value         Immediate value to subtract (1 - 31)
 * @param sync          Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr       Signal to raise upon completion
 *
 * Same as cmd_cls_sub_imm_ptr32 command, but also returns pre-modified
 * memory contents.
 * @see cmd_cls_sub_imm_ptr40()
 *
 */
__intrinsic
void cmd_cls_test_and_sub_imm_ptr40(
    __xread uint32_t *val,
    volatile void __addr40 __cls *address,
    uint32_t value,
    sync_t sync,
    SIGNAL *sig_ptr
);

/** Local scratch memory test and sub immediate from 64bit location.
 *
 * @param val           Returned pre-modified value
 * @param address       Cluster Local Scratch address to subtract from
 * @param value         Immediate value to subtract (1 - 31)
 * @param sync          Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr       Signal to raise upon completion
 *
 * Same as cls_sub_imm64 command, but also returns pre-modified
 * memory contents.
 * @see cmd_cls_sub64_imm_ptr32()
 *
 */
__intrinsic
void cmd_cls_test_and_sub64_imm_ptr32(
    __xread uint32_t *val,
    volatile void __cls *address,
    uint32_t value,
    sync_t sync,
    SIGNAL *sig_ptr
);


/** 40-bit Local scratch memory test and sub immediate from 64bit location.
 *
 * @param val           Returned pre-modified value
 * @param address       40 bit address in which to subtract values where upper 6 bits indicate the island.
 *                       See 6xxx databook for recommended addressing mode.
 * @param value         Immediate value to subtract (1 - 31)
 * @param sync          Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr       Signal to raise upon completion
 *
 * Same as cls_sub_imm64 command, but also returns pre-modified
 * memory contents.
 * @see cmd_cls_sub64_imm_ptr32()
 *
 */
__intrinsic
void cmd_cls_test_and_sub64_imm_ptr40(
    __xread uint32_t *val,
    volatile void __addr40 __cls *address,
    uint32_t value,
    sync_t sync,
    SIGNAL *sig_ptr
);




/** Clear Cluster Local Scratch memory bits and return pre-modified value to transfer
 * register.
 *
 * @param val           Returned pre-modified value
 * @param mask          Mask of bits to clear
 * @param address       Cluster Local Scratch address to clear
 * @param count         Number of 32-bit words to clear
 * @param sync          Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr       Signal to raise upon completion
 */
__intrinsic
void cmd_cls_test_and_clear_bits_ptr32(
    __xread uint32_t *val,
    __xwrite uint32_t *mask,
    volatile void __cls *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
);


/** Clear 40 bit Cluster Local Scratch memory bits and return pre-modified value to transfer
 * register.
 *
 * @param val           Returned pre-modified value
 * @param mask          Mask of bits to clear
 * @param address       40 bit address in which to clear bits where upper 6 bits indicate the island.
 *                       See 6xxx databook for recommended addressing mode.
 * @param count         Number of 32-bit words to clear
 * @param sync          Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr       Signal to raise upon completion
 */
__intrinsic
void cmd_cls_test_and_clear_bits_ptr40(
    __xread uint32_t *val,
    __xwrite uint32_t *mask,
    volatile void __addr40 __cls *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
);


/** Test and clear 40 bit Cluster Local Scratch memory bits and return pre-modified value to transfer
 * register, with indirect.
 *
 * @param val           Returned pre-modified value
 * @param mask          Mask of bits to clear
 * @param address       40 bit address in which to clear bits where upper 6 bits indicate the island.
 *                       See 6xxx databook for recommended addressing mode.
 * @param max_nn         Number of 32-bit words to clear
 * @param ind           Indirection type
 * @param sync          Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr       Signal to raise upon completion
 */

__intrinsic
void cmd_cls_test_and_clear_bits_ind_ptr40(
    __xread uint32_t *val,
    __xwrite uint32_t *mask,
    volatile void __addr40 __cls *address,
    uint32_t max_nn,
    generic_ind_t ind,
    sync_t sync,
    SIGNAL *sig_ptr
    );


/** Test and clear Cluster Local Scratch memory bits and return pre-modified value to transfer
 * register, with indirect.
 *
 * @param val           Returned pre-modified value
 * @param mask          Mask of bits to clear
 * @param address       Cluster Local Scratch address to clear
 * @param max_nn         Number of 32-bit words to clear
 * @param ind           Indirection type
 * @param sync          Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr       Signal to raise upon completion
 */
__intrinsic
void cmd_cls_test_and_clear_bits_ind_ptr32(
    __xread uint32_t *val,
    __xwrite uint32_t *mask,
    volatile void __cls *address,
    uint32_t max_nn,
    generic_ind_t ind,
    sync_t sync,
    SIGNAL *sig_ptr
    );


/** Test and set 40 bit Cluster Local Scratch memory bits and return pre-modified value to transfer, with indirect
 * register.
 *
 * @param val           Returned pre-modified value
 * @param mask          Mask of bits to clear
 * @param address       40 bit address in which to clear bits where upper 6 bits indicate the island.
 *                       See 6xxx databook for recommended addressing mode.
 * @param max_nn         Number of 32-bit words to clear
 * @param ind           Indirection type
 * @param sync          Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr       Signal to raise upon completion
 */

__intrinsic
void cmd_cls_test_and_set_bits_ind_ptr40(
    __xread uint32_t *val,
    __xwrite uint32_t *mask,
    volatile void __addr40 __cls *address,
    uint32_t max_nn,
    generic_ind_t ind,
    sync_t sync,
    SIGNAL *sig_ptr
    );



/** Test and set bits Cluster Local Scratch memory bits and return pre-modified value to transfer
 * register, with indirect.
 *
 * @param val           Returned pre-modified value
 * @param mask          Mask of bits to clear
 * @param address       Cluster Local Scratch address to clear
 * @param max_nn         Number of 32-bit words to clear
 * @param ind           Indirection type
 * @param sync          Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr       Signal to raise upon completion
 */
__intrinsic
void cmd_cls_test_and_set_bits_ind_ptr32(
    __xread uint32_t *val,
    __xwrite uint32_t *mask,
    volatile void __cls *address,
    uint32_t max_nn,
    generic_ind_t ind,
    sync_t sync,
    SIGNAL *sig_ptr
    );


/** Local scratch memory test and clear bits immediate.
 *
 * @param val           Returned pre-modified value
 * @param address       Cluster Local Scratch address to clear
 * @param value         Immediate value to clear (1 - 31)
 * @param sync          Type of synchronization to use (must be sig_done)
 * @param sig_ptr       Signal to raise upon completion
 *
 * Same as cls_clear_bits_imm command, but also returns pre-modified
 * memory contents.
 * @see cmd_cls_clear_bits_imm_ptr32()
 *
 */
__intrinsic
void cmd_cls_test_and_clear_bits_imm_ptr32(
    __xread uint32_t *val,
    volatile void __cls *address,
    uint32_t value,
    sync_t sync,
    SIGNAL *sig_ptr
);

/** 40-bit Local scratch memory test and clear bits immediate.
 *
 * @param val           Returned pre-modified value
 * @param address       40 bit address in which to clear bits where upper 6 bits indicate the island.
 *                       See 6xxx databook for recommended addressing mode.
 * @param value         Immediate value to clear (1 - 31)
 * @param sync          Type of synchronization to use (must be sig_done)
 * @param sig_ptr       Signal to raise upon completion
 *
 * Same as cls_clear_bits_imm command, but also returns pre-modified
 * memory contents.
 * @see cmd_cls_clear_bits_imm_ptr40()
 *
 */
__intrinsic
void cmd_cls_test_and_clear_bits_imm_ptr40(
    __xread uint32_t *val,
    volatile void __addr40 __cls *address,
    uint32_t value,
    sync_t sync,
    SIGNAL *sig_ptr
);

/** Set Cluster Local Scratch memory bits and return pre-modified value to transfer
 * register.
 *
 * @param val           Returned pre-modified value
 * @param mask          Mask of bits to set
 * @param address       Cluster Local Scratch address to set
 * @param count         Number of 32-bit words to set
 * @param sync          Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr       Signal to raise upon completion
 */
__intrinsic
void cmd_cls_test_and_set_bits_ptr32(
    __xread uint32_t *val,
    __xwrite uint32_t *mask,
    volatile void __cls *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
);

/** Set 40 bit Cluster Local Scratch memory bits and return pre-modified value to transfer
 * register.
 *
 * @param val           Returned pre-modified value
 * @param mask          Mask of bits to set
 * @param address       40 bit address in which to set bits where upper 6 bits indicate the island.
 *                       See 6xxx databook for recommended addressing mode.
 * @param count         Number of 32-bit words to set
 * @param sync          Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr       Signal to raise upon completion
 */
__intrinsic
void cmd_cls_test_and_set_bits_ptr40(
    __xread uint32_t *val,
    __xwrite uint32_t *mask,
    volatile void __addr40 __cls *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
);


/** Local scratch memory test and set bits immediate.
 *
 * @param val           Returned pre-modified value
 * @param address       Cluster Local Scratch address to set
 * @param value         Immediate value to set (1 - 31)
 * @param sync          Type of synchronization to use (must be sig_done)
 * @param sig_ptr       Signal to raise upon completion
 *
 * Same as cmd_cls_set_bits_imm_ptr32 command, but also returns pre-modified
 * memory contents.
 * @see cmd_cls_set_bits_imm_ptr32()
 *
 */
__intrinsic
void cmd_cls_test_and_set_bits_imm_ptr32(
    __xread uint32_t *val,
    volatile void __cls *address,
    uint32_t value,
    sync_t sync,
    SIGNAL *sig_ptr
);

/** 40-bit Local scratch memory test and set bits immediate.
 *
 * @param val           Returned pre-modified value
 * @param address       40 bit address in which to set bits where upper 6 bits indicate the island.
 *                       See 6xxx databook for recommended addressing mode.
 * @param value         Immediate value to set (1 - 31)
 * @param sync          Type of synchronization to use (must be sig_done)
 * @param sig_ptr       Signal to raise upon completion
 *
 * Same as cmd_cls_set_bits_imm_ptr32 command, but also returns pre-modified
 * memory contents.
 * @see cmd_cls_set_bits_imm_ptr40()
 *
 */
__intrinsic
void cmd_cls_test_and_set_bits_imm_ptr40(
    __xread uint32_t *val,
    volatile void __addr40 __cls *address,
    uint32_t value,
    sync_t sync,
    SIGNAL *sig_ptr
);

/** Add with saturation data to Cluster Local Scratch memory and return
 * pre-modified value to transfer register.
 *
 * @param val           Returned pre-modified value
 * @param data          Data to add
 * @param address       Cluster Local Scratch address to add data to
 * @param count         Number of 32-bit words to add
 * @param sync          Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr       Signal to raise upon completion
 */
__intrinsic
void cmd_cls_test_and_add_sat_ptr32(
    __xread uint32_t *val,
    __xwrite uint32_t *data,
    volatile void __cls *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
);


/** Add with saturation data to 40 bit Cluster Local Scratch  memory and return
 * pre-modified value to transfer register.
 *
 * @param val           Returned pre-modified value
 * @param data          Data to add
 * @param address       40 bit address in which to add value where upper 6 bits indicate the island.
 *                       See 6xxx databook for recommended addressing mode.
 * @param count         Number of 32-bit words to add
 * @param sync          Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr       Signal to raise upon completion
 */
__intrinsic
void cmd_cls_test_and_add_sat_ptr40(
    __xread uint32_t *val,
    __xwrite uint32_t *data,
    volatile void __addr40 __cls *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
);


/** Add with saturation data to 40 bit Cluster Local Scratch  memory and return
 * pre-modified value to transfer register, with indirect.
 *
 * @param val           Returned pre-modified value
 * @param data          Data to add
 * @param address       40 bit address in which to add value where upper 6 bits indicate the island.
 *                       See 6xxx databook for recommended addressing mode.
 * @param max_nn         Number of 32-bit words to add
 * @param ind           Indirection type
 * @param sync          Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr       Signal to raise upon completion
 */
__intrinsic
void cmd_cls_test_and_add_sat_ind_ptr40(
    __xread uint32_t *val,
    __xwrite uint32_t *data,
    volatile void __addr40 __cls *address,
    uint32_t max_nn,
    generic_ind_t ind,
    sync_t sync,
    SIGNAL *sig_ptr
    );


/** Add with saturation data to 32 bit Cluster Local Scratch  memory and return
 * pre-modified value to transfer register, with indirect.
 *
 * @param val           Returned pre-modified value
 * @param data          Data to add
 * @param address       Cluster Local Scratch address to clear
 * @param max_nn         Number of 32-bit words to clear
 * @param ind           Indirection type
 * @param sync          Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr       Signal to raise upon completion
 */
__intrinsic
void cmd_cls_test_and_add_sat_ind_ptr32(
    __xread uint32_t *val,
    __xwrite uint32_t *data,
    volatile void __cls *address,
    uint32_t max_nn,
    generic_ind_t ind,
    sync_t sync,
    SIGNAL *sig_ptr
    );


/** Subtract with saturation data to 40 bit Cluster Local Scratch  memory and return
 * pre-modified value to transfer register, with indirect.
 *
 * @param val           Returned pre-modified value
 * @param data          Data to subtract
 * @param address       40 bit address in which to add value where upper 6 bits indicate the island.
 *                       See 6xxx databook for recommended addressing mode.
 * @param max_nn         Number of 32-bit words to subtract
 * @param ind           Indirection type
 * @param sync          Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr       Signal to raise upon completion
 */
__intrinsic
void cmd_cls_test_and_sub_sat_ind_ptr40(
    __xread uint32_t *val,
    __xwrite uint32_t *data,
    volatile void __addr40 __cls *address,
    uint32_t max_nn,
    generic_ind_t ind,
    sync_t sync,
    SIGNAL *sig_ptr
    );



/** Subtract with saturation data to 32 bit Cluster Local Scratch  memory and return
 * pre-modified value to transfer register, with indirect.
 *
 * @param val           Returned pre-modified value
 * @param data          Data to subtract
 * @param address       Cluster Local Scratch address to subtract
 * @param max_nn         Number of 32-bit words to subtract
 * @param ind           Indirection type
 * @param sync          Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr       Signal to raise upon completion
 */
__intrinsic
void cmd_cls_test_and_sub_sat_ind_ptr32(
    __xread uint32_t *val,
    __xwrite uint32_t *data,
    volatile void __cls *address,
    uint32_t max_nn,
    generic_ind_t ind,
    sync_t sync,
    SIGNAL *sig_ptr
    );


/** Local scratch memory test and add immediate with saturate.
 *
 * @param val           Returned pre-modified value
 * @param address       Cluster Local Scratch address to add
 * @param value         Immediate value to add (1 - 31)
 * @param sync          Type of synchronization to use (must be sig_done)
 * @param sig_ptr       Signal to raise upon completion
 *
 * Same as cmd_cls_add_imm_sat_ptr32 command, but also returns pre-modified
 * memory contents.
 * @see cmd_cls_add_imm_sat_ptr32()
 */
__intrinsic
void cmd_cls_test_and_add_imm_sat_ptr32(
    __xread uint32_t *val,
    volatile void __cls *address,
    uint32_t value,
    sync_t sync,
    SIGNAL *sig_ptr
);

/** 40-bit Local scratch memory test and add immediate with saturate.
 *
 * @param val           Returned pre-modified value
 * @param address       40 bit address in which to add value where upper 6 bits indicate the island.
 *                       See 6xxx databook for recommended addressing mode.
 * @param value         Immediate value to add (1 - 31)
 * @param sync          Type of synchronization to use (must be sig_done)
 * @param sig_ptr       Signal to raise upon completion
 *
 * Same as cmd_cls_add_imm_sat_ptr32 command, but also returns pre-modified
 * memory contents.
 *
 * @see cmd_cls_add_imm_sat_ptr40()
 */
__intrinsic
void cmd_cls_test_and_add_imm_sat_ptr40(
    __xread uint32_t *val,
    volatile void __addr40 __cls *address,
    uint32_t value,
    sync_t sync,
    SIGNAL *sig_ptr
);

/** Subtract with saturation data from Cluster Local Scratch memory and return
 * pre-modified value to transfer register.
 *
 * @param val           Returned pre-modified value
 * @param data          Data to subtract
 * @param address       Cluster Local Scratch address to subtract data from
 * @param count         Number of 32-bit words to sub
 * @param sync          Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr       Signal to raise upon completion
 */
__intrinsic
void cmd_cls_test_and_sub_sat_ptr32(
    __xread uint32_t *val,
    __xwrite uint32_t *data,
    volatile void __cls *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
);

/** Subtract with saturation data from 40 bit Cluster Local Scratch  memory and return
 * pre-modified value to transfer register.
 *
 * @param val           Returned pre-modified value
 * @param data          Data to subtract
 * @param address       40 bit address in which to subtract value where upper 6 bits indicate the island.
 *                       See 6xxx databook for recommended addressing mode.
 * @param count         Number of 32-bit words to sub
 * @param sync          Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr       Signal to raise upon completion
 */
__intrinsic
void cmd_cls_test_and_sub_sat_ptr40(
    __xread uint32_t *val,
    __xwrite uint32_t *data,
    volatile void __addr40 __cls *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
);

/** Local scratch memory test and sub immediate with saturate.
 *
 * @param val           Returned pre-modified value
 * @param address       Cluster Local Scratch address to sub
 * @param value         Immediate value to sub (1 - 31)
 * @param sync          Type of synchronization to use (must be sig_done)
 * @param sig_ptr       Signal to raise upon completion
 *
 * Same as cmd_cls_sub_imm_ptr32 command, but also returns pre-modified
 * memory contents.
 * @see cmd_cls_sub_imm_ptr32()
 *
 */
__intrinsic
void cmd_cls_test_and_sub_imm_sat_ptr32(
    __xread uint32_t *val,
    volatile void __cls *address,
    uint32_t value,
    sync_t sync,
    SIGNAL *sig_ptr
);


/** 40-bit Local scratch memory test and sub immediate with saturate.
 *
 * @param val           Returned pre-modified value
 * @param address       40 bit address in which to subtract value where upper 6 bits indicate the island.
 *                       See 6xxx databook for recommended addressing mode.
 * @param value         Immediate value to sub (1 - 31)
 * @param sync          Type of synchronization to use (must be sig_done)
 * @param sig_ptr       Signal to raise upon completion
 *
 * Same as cmd_cls_sub_imm_ptr32 command, but also returns pre-modified
 * memory contents.
 * @see cmd_cls_sub_imm_ptr40()
 *
 */
__intrinsic
void cmd_cls_test_and_sub_imm_sat_ptr40(
    __xread uint32_t *val,
    volatile void __addr40 __cls *address,
    uint32_t value,
    sync_t sync,
    SIGNAL *sig_ptr
);



/** Increment by one (1) a 32-bit value in Cluster local scratch memory.
 *
 * @param address       32-bit Cluster Local Scratch address to increment
 *
 * Implemented as sub_imm with an immediate value of 0xff (-1).
 *
 */
__intrinsic
void cmd_cls_incr_ptr32(
    volatile void __addr32 __cls *address
);

/** Increment by one (1) a 32-bit value in Cluster local scratch memory.
 *
 * @param address       40-bit Cluster Local Scratch address to increment
 *
 * Implemented as sub_imm with an immediate value of 0xff (-1).
 *
 */
__intrinsic
void cmd_cls_incr_ptr40(
    volatile void __addr40 __cls *address
);

/** Decrement by one (1) a 32-bit value in Cluster local scratch memory.
 *
 * @param address       32-bit Cluster Local Scratch address to decrement
 *
 * Implemented as add_imm with an immediate value of 0xff (-1).
 *
 */
__intrinsic
void cmd_cls_decr_ptr32(
    volatile void __addr32 __cls *address
);

/** Decrement by one (1) a 32-bit value in Cluster local scratch memory.
 *
 * @param address       40-bit Cluster Local Scratch address to decrement
 *
 * Implemented as add_imm with an immediate value of 0xff (-1).
 *
 */
__intrinsic
void cmd_cls_decr_ptr40(
    volatile void __addr40 __cls *address
);


/** Increment by one (1) a 64-bit value in Cluster local scratch memory.
 *
 * @param address       32-bit Cluster Local Scratch address to increment
 *
 * Implemented as sub64_imm with an immediate value of 0xff (-1).
 *
 */
__intrinsic
void cmd_cls_incr64_ptr32(
    volatile void __addr32 __cls *address
);

/** Increment by one (1) a 64-bit value in Cluster local scratch memory.
 *
 * @param address       40-bit Cluster Local Scratch address to increment
 *
 * Implemented as sub64_imm with an immediate value of 0xff (-1).
 *
 */
__intrinsic
void cmd_cls_incr64_ptr40(
    volatile void __addr40 __cls *address
);

/** Decrement by one (1) a 64-bit value in Cluster local scratch memory.
 *
 * @param address       32-bit Cluster Local Scratch address to decrement
 *
 * Implemented as add64_imm with an immediate value of 0xff (-1).
 *
 */
__intrinsic
void cmd_cls_decr64_ptr32(
    volatile void __addr32 __cls *address
);

/** Decrement by one (1) a 64-bit value in Cluster local scratch memory.
 *
 * @param address       40-bit Cluster Local Scratch address to decrement
 *
 * Implemented as add64_imm with an immediate value of 0xff (-1).
 *
 */
__intrinsic
void cmd_cls_decr64_ptr40(
    volatile void __addr40 __cls *address
);

/** @}
 * @name Additional Atomic functions for Cluster Local Scratch
 * @{
 */

/**
 * Read, compare then write data from Cluster Local scratch to transfer register.
 * The byte mask bits indicate which bytes must match pull data for write of all the pull data to happen.
 *
 * If it matches the 'pull' data in every byte where the byte mask is 1,
 * then the SRAM data is overwritten with the pull data.
 *
 * @param data     Data to read/write
 * @param mask     Byte mask bits value
 * @param address  Cluster Local Scratch address to read/write from
 * @param count    Number of 4-byte 32-bit words to read/write
 * @param sync     Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr  Signal to raise upon completion
 */
__intrinsic
void cmd_cls_cmp_read_write_ptr32(
    __xrw uint32_t *data,
    uint32_t mask,
    volatile void __cls *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
);


/**
 * Read, compare then write data from 40 bit Cluster Local Scratch to transfer register.
 * The byte mask bits indicate which bytes must match pull data for write of all the pull data to happen.
 *
 * If it matches the 'pull' data in every byte where the byte mask is 1,
 * then the SRAM data is overwritten with the pull data.
 *
 * @param data     Data to read/write
 * @param mask     Byte mask bits value
 * @param address  40 bit address in which to read/write from where upper 6 bits indicate the island.
 *                  See 6xxx databook for recommended addressing mode.
 * @param count    Number of 4-byte 32-bit words to read/write
 * @param sync     Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr  Signal to raise upon completion
 */
__intrinsic
void cmd_cls_cmp_read_write_ptr40(
    __xrw uint32_t *data,
    uint32_t mask,
    volatile void __addr40 __cls *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
);


/**
 * Read, compare then write data from Cluster Local scratch to transfer register
 * in indirect format.
 *
 * @param data     Data to read/write
 * @param address  40 bit address in which to read/write from where upper 6 bits indicate the island.
 *                 See 6xxx databook for recommended addressing mode.
 * @param max_nn   Max number of 4-byte 32-bit words to read/write
 * @param ind      Indirect word
 * @param sync     Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr  Signal to raise upon completion
 */
__intrinsic
void cmd_cls_cmp_read_write_ind_ptr40(
    __xrw void *data,
    volatile void __addr40 __cls *address,
    uint32_t max_nn,
    generic_ind_t ind,
    sync_t sync,
    SIGNAL *sig_ptr
    );


/**
 * Read, compare then write data from Cluster Local scratch to transfer register
 * in indirect format.
 *
 * @param data     Data to read/write
 * @param address  Cluster Local Scratch address to read/write from
 * @param max_nn   Max number of 4-byte 32-bit words to read/write
 * @param ind      Indirect word
 * @param sync     Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr  Signal to raise upon completion
 */
__intrinsic
void cmd_cls_cmp_read_write_ind_ptr32(
    __xrw void *data,
    volatile void __cls *address,
    uint32_t max_nn,
    generic_ind_t ind,
    sync_t sync,
    SIGNAL *sig_ptr
    );


/** Local scratch Queue Lock Claim in indirect mode.
 *
 * @param address       Cluster Local Scratch 40 bit address to read into
 * @param ind           Indirect word
 * @param sync          Type of synchronization to use (sig_done only)
 * @param sig_pair_ptr  Signal pair to raise upon completion
 *
 */
__intrinsic
void cmd_cls_queue_lock_ind_ptr40(
    volatile void __addr40 __cls *address,
    generic_ind_t ind,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
    );


/** Local scratch Queue Lock Claim in indirect mode.
 *
 * @param address       Cluster Local Scratch 32 bit address to read into
 * @param ind           Indirect word
 * @param sync          Type of synchronization to use (sig_done only)
 * @param sig_pair_ptr  Signal pair to raise upon completion
 *
 */
__intrinsic
void cls_queue_lock_ind_ptr32(
    volatile void __cls *address,
    generic_ind_t ind,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
    );


/** Local scratch Queue Lock Claim.
 *
 * @param address       Cluster Local Scratch address to read into
 * @param sync          Type of synchronization to use (sig_done only)
 * @param sig_pair_ptr  Signal pair to raise upon completion. Sending sig_name is used to indicate that
 *                       the operation completed. If sig_name+1 is also sent, it indicates that the operation
 *                       completed, but failed because the queue was full.
 *
 * Attempt to acquire lock and respond with sig_name when lock is acquired.
 * If lock can not be immediately acquired, enqueue the signal to send once it can be acquired.
 * Up to 5 additional signals can be enqueued. If the queue is already full, respond with sig_name and sig_name+1.
 *
 */
__intrinsic
void cmd_cls_queue_lock_ptr32(
    volatile void __cls *address,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
);


/** 40-bit Local scratch Queue Lock Claim.
 *
 * @param address       40 bit address in which to read into where upper 6 bits indicate the island.
 *                       See 6xxx databook for recommended addressing mode.
 * @param sync          Type of synchronization to use (sig_done only)
 * @param sig_pair_ptr  Signal pair to raise upon completion. Sending sig_name is used to indicate that
 *                       the operation completed. If sig_name+1 is also sent, it indicates that the operation
 *                       completed, but failed because the queue was full.
 *
 * Attempt to acquire lock and respond with sig_name when lock is acquired.
 * If lock can not be immediately acquired, enqueue the signal to send once it can be acquired.
 * Up to 5 additional signals can be enqueued. If the queue is already full, respond with sig_name and sig_name+1.
 *
 */
__intrinsic
void cmd_cls_queue_lock_ptr40(
    volatile void __addr40 __cls *address,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
);



/** Local scratch Queue UnLock Claim.
 *
 * @param address      Cluster Local Scratch address to read into
 *
 * Release an already acquired lock. If the lock-queue has any pending signals enqueued
 * do not release the lock, but instead dequeue the first signal off the queue and trigger it.
 * If there are no pending signals in the queue, then no signals are triggered and the
 * queue is marked as empty.
 *
 */
__intrinsic
void cmd_cls_queue_unlock_ptr32(
    volatile void __cls *address
);

/** 40-bit Local scratch Queue UnLock Claim.
 *
 * @param address       40 bit address in which to read into where upper 6 bits indicate the island.
 *                       See 6xxx databook for recommended addressing mode.
 *
 * Release an already acquired lock. If the lock-queue has any pending signals enqueued
 * do not release the lock, but instead dequeue the first signal off the queue and trigger it.
 * If there are no pending signals in the queue, then no signals are triggered and the
 * queue is marked as empty.
 *
 */
__intrinsic
void cmd_cls_queue_unlock_ptr40(
    volatile void __addr40 __cls *address
);

/** @}
 * @name Hash functions for Cluster Local Scratch
 * @{
 */


/** Local scratch memory Hash.
 *
 * @param data          Data to hash
 * @param address       Address to hash
 * @param count         Number of 32-bit words to produce hash_index over. Valid values in
 *                       multiples of 2 in the range 2-32.
 * @param sync          Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr       Signal to raise upon completion
 *
 * Create a 64-bit hash_index over the transfer registers as follows:
 * hash_index_select = address[18:16], hash_mask_address = address[15:0], hash_index = hash_indexes[hash_index_select].
 *
 * For each 64-bit data value in the transfer register pairs:
 * data = data AND cls_read64(hash_mask_address), calculate new hash_index as indicated above,
 * hash_mask_address = hash_mask_address + 8, hash_indexes[hash_index_select] = hash_index.
 */
__intrinsic
void cmd_cls_hash_mask_ptr32(
    __xwrite void *data,
    volatile void __cls *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
);


/** 40-bit Local scratch memory Hash.
 *
 * @param data          Data to hash
 * @param address       40 bit address to hash where upper 6 bits indicate the island.
 *                       See 6xxx databook for recommended addressing mode.
 * @param count         Number of 32-bit words to produce hash_index over. Valid values in
 *                       multiples of 2 in the range 2-32.
 * @param sync          Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr       Signal to raise upon completion
 *
 * Create a 64-bit hash_index over the transfer registers as follows:
 * hash_index_select = address[18:16], hash_mask_address = address[15:0], hash_index = hash_indexes[hash_index_select].
 *
 * For each 64-bit data value in the transfer register pairs:
 * data = data AND cls_read64(hash_mask_address), calculate new hash_index as indicated above,
 * hash_mask_address = hash_mask_address + 8, hash_indexes[hash_index_select] = hash_index.
 */
__intrinsic
void cmd_cls_hash_mask_ptr40(
    __xwrite void *data,
    volatile void __addr40 __cls *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
);


/** Local scratch memory Hash in indirect mode.
 *
 * @param data          Data to hash
 * @param address       Address to hash, 40 bit
 * @param max_nn        Max number of 32-bit words to produce hash_index over
 * @param ind           Indirect word
 * @param sync          Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr       Signal to raise upon completion
 *
 */
__intrinsic
void cmd_cls_hash_mask_ind_ptr40(
    __xwrite void *data,
    volatile void __addr40 __cls *address,
    uint32_t max_nn,
    generic_ind_t ind,
    sync_t sync,
    SIGNAL *sig_ptr
);

/** Local scratch memory Hash in indirect mode.
 *
 * @param data          Data to hash
 * @param address       Address to hash, 32 bit
 * @param max_nn        Max number of 32-bit words to produce hash_index over
 * @param ind           Indirect word
 * @param sync          Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr       Signal to raise upon completion
 *
 */
__intrinsic
void cmd_cls_hash_mask_ind_ptr32(
    __xwrite void *data,
    volatile void __cls *address,
    uint32_t max_nn,
    generic_ind_t ind,
    sync_t sync,
    SIGNAL *sig_ptr
);



/** Local scratch memory Hash Clear.
 *
 * @param data          Data to hash
 * @param address       Address to hash
 * @param count         Number of 32-bit words to produce hash_index over. Valid values in
 *                       multiples of 2 in the range 2-32.
 * @param sync          Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr       Signal to raise upon completion
 *
 * Same as hash_mask command, but also clears the initial value of hash_index
 * before calculating the new value.
 * @see cmd_cls_hash_mask_ptr32()
 *
 */
__intrinsic
void cmd_cls_hash_mask_clear_ptr32(
    __xwrite void *data,
    volatile void __cls *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
);

/** 40-bit Local scratch memory Hash Clear.
 *
 * @param data          Data to hash
 * @param address       40 bit address to hash where upper 6 bits indicate the island.
 *                       See 6xxx databook for recommended addressing mode.
 * @param count         Number of 32-bit words to produce hash_index over. Valid values in
 *                       multiples of 2 in the range 2-32.
 * @param sync          Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr       Signal to raise upon completion
 *
 * Same as hash_mask command, but also clears the initial value of hash_index
 * before calculating the new value.
 * @see cmd_cls_hash_mask_ptr32()
 *
 */
__intrinsic
void cmd_cls_hash_mask_clear_ptr40(
    __xwrite void *data,
    volatile void __addr40 __cls *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
);

/** @}
 * @name CAM functions for Cluster Local Scratch
 * @{
 */


/** Local scratch Content Addressable Memory lookup word in indirect mode.
 *
 * @param data_out Data looked up
 * @param data_in  Data to lookup
 * @param address  Local scratch CAM 40 bit address to lookup, 40 bit
 * @param ind      Indirect word
 * @param sync     Type of synchronization (sig_done or ctx_swap)
 * @param sig_ptr  Signal to be raised upon completion
 *
 * @see cmd_cls_cam_lookup_ptr32()
 */
__intrinsic
void cmd_cls_cam_lookup_ind_ptr40(
    __xread void *data_out,
    __xwrite void *data_in,
    volatile void __addr40 __cls *address,
    generic_ind_t ind,
    sync_t sync,
    SIGNAL *sig_ptr
    );


/** Local scratch Content Addressable Memory lookup word in indirect mode.
 *
 * @param data_out Data looked up
 * @param data_in  Data to lookup
 * @param address  Local scratch CAM 32 bit address to lookup
 * @param ind      Indirect word
 * @param sync     Type of synchronization (sig_done or ctx_swap)
 * @param sig_ptr  Signal to be raised upon completion
 *
 * @see cmd_cls_cam_lookup_ptr32()
 */
__intrinsic
void cmd_cls_cam_lookup_ind_ptr32(
    __xread void *data_out,
    __xwrite void *data_in,
    volatile void __cls *address,
    generic_ind_t ind,
    sync_t sync,
    SIGNAL *sig_ptr
    );


/** Local scratch Content Addressable Memory lookup word with add.
 *
 * @param data_out Data looked up
 * @param data_in  Data to lookup
 * @param address  Local scratch CAM 40 bit address to lookup, 40 bit
 * @param ind      Indirect word
 * @param sync     Type of synchronization (sig_done or ctx_swap)
 * @param sig_ptr  Signal to be raised upon completion
 *
 * @see cmd_cls_cam_lookup_ptr32()
 */
__intrinsic
void cmd_cls_cam_lookup_add_ind_ptr40(
    __xread void *data_out,
    __xwrite void *data_in,
    volatile void __addr40 __cls *address,
    generic_ind_t ind,
    sync_t sync,
    SIGNAL *sig_ptr
    );


/** Local scratch Content Addressable Memory lookup word with add.
 *
 * @param data_out Data looked up
 * @param data_in  Data to lookup
 * @param address  Local scratch CAM 32 bit address to lookup
 * @param ind      Indirect word
 * @param sync     Type of synchronization (sig_done or ctx_swap)
 * @param sig_ptr  Signal to be raised upon completion
 *
 * @see cmd_cls_cam_lookup_ptr32()
 */
__intrinsic
void cmd_cls_cam_lookup_add_ind_ptr32(
    __xread void *data_out,
    __xwrite void *data_in,
    volatile void __cls *address,
    generic_ind_t ind,
    sync_t sync,
    SIGNAL *sig_ptr
    );


/** Local scratch Content Addressable Memory lookup 24-bits in indirect mode.
 *
 * @param data_out Data looked up
 * @param data_in  Data to lookup
 * @param address  Local scratch CAM 40 bit address to lookup, 40 bit
 * @param ind      Indirect word
 * @param sync     Type of synchronization (sig_done or ctx_swap)
 * @param sig_ptr  Signal to be raised upon completion
 *
 * @see cmd_cls_cam_lookup_ptr32()
 */
__intrinsic
void cmd_cls_cam_lookup24_ind_ptr40(
    __xread void *data_out,
    __xwrite void *data_in,
    volatile void __addr40 __cls *address,
    generic_ind_t ind,
    sync_t sync,
    SIGNAL *sig_ptr
    );


/** Local scratch Content Addressable Memory lookup 24-bits in indirect mode.
 *
 * @param data_out Data looked up
 * @param data_in  Data to lookup
 * @param address  Local scratch CAM 32 bit address to lookup
 * @param ind      Indirect word
 * @param sync     Type of synchronization (sig_done or ctx_swap)
 * @param sig_ptr  Signal to be raised upon completion
 *
 * @see cmd_cls_cam_lookup_ptr32()
 */
__intrinsic
void cmd_cls_cam_lookup24_ind_ptr32(
    __xread void *data_out,
    __xwrite void *data_in,
    volatile void __cls *address,
    generic_ind_t ind,
    sync_t sync,
    SIGNAL *sig_ptr
    );


/** Local scratch Content Addressable Memory lookup 24-bits with add in indirect mode.
 *
 * @param data_out Data looked up
 * @param data_in  Data to lookup
 * @param address  Local scratch CAM 40 bit address to lookup, 40 bit
 * @param ind      Indirect word
 * @param sync     Type of synchronization (sig_done or ctx_swap)
 * @param sig_ptr  Signal to be raised upon completion
 *
 * @see cmd_cls_cam_lookup_ptr32()
 */
__intrinsic
void cmd_cls_cam_lookup24_add_ind_ptr40(
    __xread void *data_out,
    __xwrite void *data_in,
    volatile void __addr40 __cls *address,
    generic_ind_t ind,
    sync_t sync,
    SIGNAL *sig_ptr
    );


/** Local scratch Content Addressable Memory lookup 24-bits with add in indirect mode.
 *
 * @param data_out Data looked up
 * @param data_in  Data to lookup
 * @param address  Local scratch CAM 32 bit address to lookup
 * @param ind      Indirect word
 * @param sync     Type of synchronization (sig_done or ctx_swap)
 * @param sig_ptr  Signal to be raised upon completion
 *
 * @see cmd_cls_cam_lookup_ptr32()
 */
__intrinsic
void cmd_cls_cam_lookup24_add_ind_ptr32(
    __xread void *data_out,
    __xwrite void *data_in,
    volatile void __cls *address,
    generic_ind_t ind,
    sync_t sync,
    SIGNAL *sig_ptr
    );


/** Local scratch Content Addressable Memory lookup 24-bits with add or increment usage in indirect mode.
 *
 * @param data_out Data looked up
 * @param data_in  Data to lookup
 * @param address  Local scratch CAM 40 bit address to lookup, 40 bit
 * @param ind      Indirect word
 * @param sync     Type of synchronization (sig_done or ctx_swap)
 * @param sig_ptr  Signal to be raised upon completion
 *
 * @see cmd_cls_cam_lookup_ptr32()
 */
__intrinsic
void cmd_cls_cam_lookup24_add_inc_ind_ptr40(
    __xread void *data_out,
    __xwrite void *data_in,
    volatile void __addr40 __cls *address,
    generic_ind_t ind,
    sync_t sync,
    SIGNAL *sig_ptr
    );


/** Local scratch Content Addressable Memory lookup 24-bits with add or increment usage in indirect mode.
 *
 * @param data_out Data looked up
 * @param data_in  Data to lookup
 * @param address  Local scratch CAM 32 bit address to lookup
 * @param ind      Indirect word
 * @param sync     Type of synchronization (sig_done or ctx_swap)
 * @param sig_ptr  Signal to be raised upon completion
 *
 * @see cmd_cls_cam_lookup_ptr32()
 */
__intrinsic
void cmd_cls_cam_lookup24_add_inc_ind_ptr32(
    __xread void *data_out,
    __xwrite void *data_in,
    volatile void __cls *address,
    generic_ind_t ind,
    sync_t sync,
    SIGNAL *sig_ptr
    );


/** Local scratch Content Addressable Memory lookup 16-bits in indirect mode.
 *
 * @param data_out Data looked up
 * @param data_in  Data to lookup
 * @param address  Local scratch CAM 40 bit address to lookup, 40 bit
 * @param ind      Indirect word
 * @param sync     Type of synchronization (sig_done or ctx_swap)
 * @param sig_ptr  Signal to be raised upon completion
 *
 * @see cmd_cls_cam_lookup_ptr32()
 */
__intrinsic
void cmd_cls_cam_lookup16_ind_ptr40(
    __xread void *data_out,
    __xwrite void *data_in,
    volatile void __addr40 __cls *address,
    generic_ind_t ind,
    sync_t sync,
    SIGNAL *sig_ptr
    );


/** Local scratch Content Addressable Memory lookup 16-bits in indirect mode.
 *
 * @param data_out Data looked up
 * @param data_in  Data to lookup
 * @param address  Local scratch CAM 32 bit address to lookup
 * @param ind      Indirect word
 * @param sync     Type of synchronization (sig_done or ctx_swap)
 * @param sig_ptr  Signal to be raised upon completion
 *
 * @see cmd_cls_cam_lookup_ptr32()
 */
__intrinsic
void cmd_cls_cam_lookup16_ind_ptr32(
    __xread void *data_out,
    __xwrite void *data_in,
    volatile void __cls *address,
    generic_ind_t ind,
    sync_t sync,
    SIGNAL *sig_ptr
    );


/** Local scratch Content Addressable Memory lookup 16-bits with add in indirect mode.
 *
 * @param data_out Data looked up
 * @param data_in  Data to lookup
 * @param address  Local scratch CAM 40 bit address to lookup, 40 bit
 * @param ind      Indirect word
 * @param sync     Type of synchronization (sig_done or ctx_swap)
 * @param sig_ptr  Signal to be raised upon completion
 *
 * @see cmd_cls_cam_lookup_ptr32()
 */
__intrinsic
void cmd_cls_cam_lookup16_add_ind_ptr40(
    __xread void *data_out,
    __xwrite void *data_in,
    volatile void __addr40 __cls *address,
    generic_ind_t ind,
    sync_t sync,
    SIGNAL *sig_ptr
    );


/** Local scratch Content Addressable Memory lookup 16-bits with add in indirect mode.
 *
 * @param data_out Data looked up
 * @param data_in  Data to lookup
 * @param address  Local scratch CAM 32 bit address to lookup
 * @param ind      Indirect word
 * @param sync     Type of synchronization (sig_done or ctx_swap)
 * @param sig_ptr  Signal to be raised upon completion
 *
 * @see cmd_cls_cam_lookup_ptr32()
 */
__intrinsic
void cmd_cls_cam_lookup16_add_ind_ptr32(
    __xread void *data_out,
    __xwrite void *data_in,
    volatile void __cls *address,
    generic_ind_t ind,
    sync_t sync,
    SIGNAL *sig_ptr
    );


/** Local scratch Content Addressable Memory lookup 8-bits in indirect mode.
 *
 * @param data_out Data looked up
 * @param data_in  Data to lookup
 * @param address  Local scratch CAM 40 bit address to lookup, 40 bit
 * @param ind      Indirect word
 * @param sync     Type of synchronization (sig_done or ctx_swap)
 * @param sig_ptr  Signal to be raised upon completion
 *
 * @see cmd_cls_cam_lookup_ptr32()
 */
__intrinsic
void cmd_cls_cam_lookup8_ind_ptr40(
    __xread void *data_out,
    __xwrite void *data_in,
    volatile void __addr40 __cls *address,
    generic_ind_t ind,
    sync_t sync,
    SIGNAL *sig_ptr
    );


/** Local scratch Content Addressable Memory lookup 8-bits in indirect mode.
 *
 * @param data_out Data looked up
 * @param data_in  Data to lookup
 * @param address  Local scratch CAM 32 bit address to lookup
 * @param ind      Indirect word
 * @param sync     Type of synchronization (sig_done or ctx_swap)
 * @param sig_ptr  Signal to be raised upon completion
 *
 * @see cmd_cls_cam_lookup_ptr32()
 */
__intrinsic
void cmd_cls_cam_lookup8_ind_ptr32(
    __xread void *data_out,
    __xwrite void *data_in,
    volatile void __cls *address,
    generic_ind_t ind,
    sync_t sync,
    SIGNAL *sig_ptr
    );


/** Local scratch Content Addressable Memory lookup 8-bits with add in indirect mode.
 *
 * @param data_out Data looked up
 * @param data_in  Data to lookup
 * @param address  Local scratch CAM 40 bit address to lookup, 40 bit
 * @param ind      Indirect word
 * @param sync     Type of synchronization (sig_done or ctx_swap)
 * @param sig_ptr  Signal to be raised upon completion
 *
 * @see cmd_cls_cam_lookup_ptr32()
 */
__intrinsic
void cmd_cls_cam_lookup8_add_ind_ptr40(
    __xread void *data_out,
    __xwrite void *data_in,
    volatile void __addr40 __cls *address,
    generic_ind_t ind,
    sync_t sync,
    SIGNAL *sig_ptr
    );


/** Local scratch Content Addressable Memory lookup 8-bits with add in indirect mode.
 *
 * @param data_out Data looked up
 * @param data_in  Data to lookup
 * @param address  Local scratch CAM 32 bit address to lookup
 * @param ind      Indirect word
 * @param sync     Type of synchronization (sig_done or ctx_swap)
 * @param sig_ptr  Signal to be raised upon completion
 *
 * @see cmd_cls_cam_lookup_ptr32()
 */
__intrinsic
void cmd_cls_cam_lookup8_add_ind_ptr32(
    __xread void *data_out,
    __xwrite void *data_in,
    volatile void __cls *address,
    generic_ind_t ind,
    sync_t sync,
    SIGNAL *sig_ptr
    );


/** Local scratch TCAM lookup word in indirect mode.
 *
 * @param data_out Data looked up
 * @param data_in  Data to lookup
 * @param address  Local scratch TCAM 40 bit address to lookup, 40 bit
 * @param ind      Indirect word
 * @param sync     Type of synchronization (sig_done or ctx_swap)
 * @param sig_ptr  Signal to be raised upon completion
 *
 * @see cmd_cls_cam_lookup_ptr32()
 */
__intrinsic
void cmd_cls_tcam_lookup_ind_ptr40(
    __xread void *data_out,
    __xwrite void *data_in,
    volatile void __addr40 __cls *address,
    generic_ind_t ind,
    sync_t sync,
    SIGNAL *sig_ptr
    );


/** Local scratch TCAM lookup word in indirect mode.
 *
 * @param data_out Data looked up
 * @param data_in  Data to lookup
 * @param address  Local scratch TCAM 32 bit address to lookup
 * @param ind      Indirect word
 * @param sync     Type of synchronization (sig_done or ctx_swap)
 * @param sig_ptr  Signal to be raised upon completion
 *
 * @see cmd_cls_cam_lookup_ptr32()
 */
__intrinsic
void cmd_cls_tcam_lookup_ind_ptr32(
    __xread void *data_out,
    __xwrite void *data_in,
    volatile void __cls *address,
    generic_ind_t ind,
    sync_t sync,
    SIGNAL *sig_ptr
    );


/** Local scratch TCAM lookup 24-bits in indirect mode.
 *
 * @param data_out Data looked up
 * @param data_in  Data to lookup
 * @param address  Local scratch TCAM 40 bit address to lookup, 40 bit
 * @param ind      Indirect word
 * @param sync     Type of synchronization (sig_done or ctx_swap)
 * @param sig_ptr  Signal to be raised upon completion
 *
 * @see cmd_cls_cam_lookup_ptr32()
 */
__intrinsic
void cmd_cls_tcam_lookup24_ind_ptr40(
    __xread void *data_out,
    __xwrite void *data_in,
    volatile void __addr40 __cls *address,
    generic_ind_t ind,
    sync_t sync,
    SIGNAL *sig_ptr
    );


/** Local scratch TCAM lookup 24-bits in indirect mode.
 *
 * @param data_out Data looked up
 * @param data_in  Data to lookup
 * @param address  Local scratch TCAM 32 bit address to lookup
 * @param ind      Indirect word
 * @param sync     Type of synchronization (sig_done or ctx_swap)
 * @param sig_ptr  Signal to be raised upon completion
 *
 * @see cmd_cls_cam_lookup_ptr32()
 */
__intrinsic
void cmd_cls_tcam_lookup24_ind_ptr32(
    __xread void *data_out,
    __xwrite void *data_in,
    volatile void __cls *address,
    generic_ind_t ind,
    sync_t sync,
    SIGNAL *sig_ptr
    );


/** Local scratch TCAM lookup 16-bits in indirect mode.
 *
 * @param data_out Data looked up
 * @param data_in  Data to lookup
 * @param address  Local scratch TCAM 40 bit address to lookup, 40 bit
 * @param ind      Indirect word
 * @param sync     Type of synchronization (sig_done or ctx_swap)
 * @param sig_ptr  Signal to be raised upon completion
 *
 * @see cmd_cls_cam_lookup_ptr32()
 */
__intrinsic
void cmd_cls_tcam_lookup16_ind_ptr40(
    __xread void *data_out,
    __xwrite void *data_in,
    volatile void __addr40 __cls *address,
    generic_ind_t ind,
    sync_t sync,
    SIGNAL *sig_ptr
    );


/** Local scratch TCAM lookup 16-bits in indirect mode.
 *
 * @param data_out Data looked up
 * @param data_in  Data to lookup
 * @param address  Local scratch TCAM 32 bit address to lookup
 * @param ind      Indirect word
 * @param sync     Type of synchronization (sig_done or ctx_swap)
 * @param sig_ptr  Signal to be raised upon completion
 *
 * @see cmd_cls_cam_lookup_ptr32()
 */
__intrinsic
void cmd_cls_tcam_lookup16_ind_ptr32(
    __xread void *data_out,
    __xwrite void *data_in,
    volatile void __cls *address,
    generic_ind_t ind,
    sync_t sync,
    SIGNAL *sig_ptr
    );


/** Local scratch TCAM lookup 8-bits in indirect mode.
 *
 * @param data_out Data looked up
 * @param data_in  Data to lookup
 * @param address  Local scratch TCAM 40 bit address to lookup, 40 bit
 * @param ind      Indirect word
 * @param sync     Type of synchronization (sig_done or ctx_swap)
 * @param sig_ptr  Signal to be raised upon completion
 *
 * @see cmd_cls_cam_lookup_ptr32()
 */
__intrinsic
void cmd_cls_tcam_lookup8_ind_ptr40(
    __xread void *data_out,
    __xwrite void *data_in,
    volatile void __addr40 __cls *address,
    generic_ind_t ind,
    sync_t sync,
    SIGNAL *sig_ptr
    );


/** Local scratch TCAM lookup 8-bits in indirect mode.
 *
 * @param data_out Data looked up
 * @param data_in  Data to lookup
 * @param address  Local scratch TCAM 32 bit address to lookup
 * @param ind      Indirect word
 * @param sync     Type of synchronization (sig_done or ctx_swap)
 * @param sig_ptr  Signal to be raised upon completion
 *
 * @see cmd_cls_cam_lookup_ptr32()
 */
__intrinsic
void cmd_cls_tcam_lookup8_ind_ptr32(
    __xread void *data_out,
    __xwrite void *data_in,
    volatile void __cls *address,
    generic_ind_t ind,
    sync_t sync,
    SIGNAL *sig_ptr
    );


/** Local scratch Content Addressable Memory lookup word.
 *
 * @param data_out Data looked up
 * @param data_in  Data to lookup
 * @param address  32 bit local scratch CAM address to lookup
 * @param count    Size of CAM in number of 64-bit words (1-32)
 * @param sync     Type of synchronization (sig_done or ctx_swap)
 * @param sig_ptr  Signal to be raised upon completion
 *
 * Use 32-bit values from transfer register and compare with 32-bit input data.
 */

__intrinsic
void cmd_cls_cam_lookup_ptr32(
    __xread void *data_out,
    __xwrite void *data_in,
    volatile void __cls *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
);

/** Local scratch Content Addressable Memory lookup word.
 *
 * @param data_out Data looked up
 * @param data_in  Data to lookup
 * @param address  40 bit local scratch CAM address to lookup where upper 6 bits indicate the island.
 *                  See 6xxx databook for recommended addressing mode.
 * @param count    Size of CAM in number of 64-bit words (1-32)
 * @param sync     Type of synchronization (sig_done or ctx_swap)
 * @param sig_ptr  Signal to be raised upon completion
 *
 * Use 32-bit values from transfer register and compare with 32-bit input data.
 */

__intrinsic
void cmd_cls_cam_lookup_ptr40(
    __xread void *data_out,
    __xwrite void *data_in,
    volatile void __addr40 __cls *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
);

/** Local scratch Content Addressable Memory lookup word with add.
 *
 * @param data_out Data looked up
 * @param data_in  Data to lookup
 * @param address  Local scratch CAM address to lookup
 * @param count    Size of CAM in number of 64-bit words (1-16)
 * @param sync     Type of synchronization (sig_done or ctx_swap)
 * @param sig_ptr  Signal to be raised upon completion
 *
 * Use 32-bit values from transfer register and compare with 32-bit input data.
 * If no match was found insert input data into first zero entry in the CAM.
 */
__intrinsic
void cmd_cls_cam_lookup_add_ptr32(
    __xread void *data_out,
    __xwrite void *data_in,
    volatile void __cls *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
);


/** 40-bit Local scratch Content Addressable Memory lookup word with add.
 *
 * @param data_out Data looked up
 * @param data_in  Data to lookup
 * @param address  40 bit CAM address to lookup where upper 6 bits indicate the island.
 *                  See 6xxx databook for recommended addressing mode.
 * @param count    Size of CAM in number of 64-bit words (1-16)
 * @param sync     Type of synchronization (sig_done or ctx_swap)
 * @param sig_ptr  Signal to be raised upon completion
 *
 * Use 32-bit values from transfer register and compare with 32-bit input data.
 * If no match was found insert input data into first zero entry in the CAM.
 */
__intrinsic
void cmd_cls_cam_lookup_add_ptr40(
    __xread void *data_out,
    __xwrite void *data_in,
    volatile void __addr40 __cls *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
);


/** Local scratch Content Addressable Memory lookup 24-bit.
 *
 * @param data_out Data looked up
 * @param data_in  Data to lookup
 * @param address  Local scratch CAM address to lookup
 * @param count    Size of CAM in number of 64-bit words (1-32)
 * @param sync     Type of synchronization (sig_done or ctx_swap)
 * @param sig_ptr  Signal to be raised upon completion
 *
 * Use 32-bit values from transfer register and compare lower 24-bits with inputdata.
 */
__intrinsic
void cmd_cls_cam_lookup24_ptr32(
    __xread void *data_out,
    __xwrite void *data_in,
    volatile void __cls *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
);



/** 40-bit Local scratch Content Addressable Memory lookup 24-bit.
 *
 * @param data_out Data looked up
 * @param data_in  Data to lookup
 * @param address  40 bit CAM address to lookup where upper 6 bits indicate the island.
 *                  See 6xxx databook for recommended addressing mode.
 * @param count    Size of CAM in number of 64-bit words (1-32)
 * @param sync     Type of synchronization (sig_done or ctx_swap)
 * @param sig_ptr  Signal to be raised upon completion
 *
 * Use 32-bit values from transfer register and compare lower 24-bits with inputdata.
 */
__intrinsic
void cmd_cls_cam_lookup24_ptr40(
    __xread void *data_out,
    __xwrite void *data_in,
    volatile void __addr40 __cls *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
);


/** Local scratch Content Addressable Memory lookup 24-bits with add.
 *
 * @param data_out Data looked up
 * @param data_in  Data to lookup
 * @param address  Local scratch CAM address to lookup
 * @param count    Size of CAM in number of 64-bit words (1-16)
 * @param sync     Type of synchronization (sig_done or ctx_swap)
 * @param sig_ptr  Signal to be raised upon completion
 *
 * Use 32-bit values from transfer register and compare lower 24-bits with input data.
 * If no match was found insert input data (full 32-bit) into first zero entry in the CAM.
 */
__intrinsic
void cmd_cls_cam_lookup24_add_ptr32(
    __xread void *data_out,
    __xwrite void *data_in,
    volatile void __cls *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
);


/** 40-bit Local scratch Content Addressable Memory lookup 24-bits with add.
 *
 * @param data_out Data looked up
 * @param data_in  Data to lookup
 * @param address  40 bit CAM address to lookup where upper 6 bits indicate the island.
 *                  See 6xxx databook for recommended addressing mode.
 * @param count    Size of CAM in number of 64-bit words (1-16)
 * @param sync     Type of synchronization (sig_done or ctx_swap)
 * @param sig_ptr  Signal to be raised upon completion
 *
 * Use 32-bit values from transfer register and compare lower 24-bits with input data.
 * If no match was found insert input data (full 32-bit) into first zero entry in the CAM.
 */
__intrinsic
void cmd_cls_cam_lookup24_add_ptr40(
    __xread void *data_out,
    __xwrite void *data_in,
    volatile void __addr40 __cls *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
);

/** Local scratch Content Addressable Memory lookup 24-bits with add or increment usage.
 *
 * @param data_out Data looked up
 * @param data_in  Data to lookup
 * @param address  Local scratch CAM address to lookup
 * @param count    Size of CAM in number of 64-bit words + 16 (1-16 plus 16)
 * @param sync     Type of synchronization (sig_done or ctx_swap)
 * @param sig_ptr  Signal to be raised upon completion
 *
 * Use 32-bit values from transfer register and compare lower 24-bits with input data.
 * If a match was found, increment the counter in the top 8-bits of the matching entry with one (without saturation).
 * If no match was found, insert input data (full 32-bit) into first zero entry in the CAM.
 */
__intrinsic
void cmd_cls_cam_lookup24_add_inc_ptr32(
    __xread void *data_out,
    __xwrite void *data_in,
    volatile void __cls *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
);

/** 40-bit Local scratch Content Addressable Memory lookup 24-bits with add or increment usage.
 *
 * @param data_out Data looked up
 * @param data_in  Data to lookup
 * @param address  40 bit CAM address to lookup where upper 6 bits indicate the island.
 *                  See 6xxx databook for recommended addressing mode.
 * @param count    Size of CAM in number of 64-bit words + 16 (1-16 plus 16)
 * @param sync     Type of synchronization (sig_done or ctx_swap)
 * @param sig_ptr  Signal to be raised upon completion
 *
 * Use 32-bit values from transfer register and compare lower 24-bits with input data.
 * If a match was found, increment the counter in the top 8-bits of the matching entry with one (without saturation).
 * If no match was found, insert input data (full 32-bit) into first zero entry in the CAM.
 */
__intrinsic
void cmd_cls_cam_lookup24_add_inc_ptr40(
    __xread void *data_out,
    __xwrite void *data_in,
    volatile void __addr40 __cls *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
);


/** Local scratch Content Addressable Memory lookup 24-bits with add lock.
 *
 * @param data_out Data looked up
 * @param data_in  Data to lookup
 * @param address  Local scratch CAM address to lookup
 * @param count    Size of CAM in number of 64-bit words (1-16)
 * @param sync     Type of synchronization (sig_done or ctx_swap)
 * @param sig_ptr  Signal to be raised upon completion
 *
 * Use 32-bit values from transfer register and compare lower 24-bits with input data.
 * If match is found then bit[31] of the matching memory data location is set indicating that the
 * entry is now locked. The pre-modified memory data top byte is returned in the push data[15:8]
 * so that the software can learn if location was already locked.
 * If match is not found and there is room for the entry then new entry is added with data[31]
 * set indicating that the entry is now locked. If match is not found and there is no free entry
 * then no memory update is performed and (0x000000ff) is returned.
 */
__intrinsic
void cmd_cls_cam_lookup24_add_lock_ptr32(
    __xread void *data_out,
    __xwrite void *data_in,
    volatile void __cls *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
);


/** 40-bit Local scratch Content Addressable Memory lookup 24-bits with add lock.
 *
 * @param data_out Data looked up
 * @param data_in  Data to lookup
 * @param address  40 bit CAM address to lookup where upper 6 bits indicate the island.
 *                  See 6xxx databook for recommended addressing mode.
 * @param count    Size of CAM in number of 64-bit words (1-16)
 * @param sync     Type of synchronization (sig_done or ctx_swap)
 * @param sig_ptr  Signal to be raised upon completion
 *
 * Use 32-bit values from transfer register and compare lower 24-bits with input data.
 * If match is found then bit[31] of the matching memory data location is set indicating that the
 * entry is now locked. The pre-modified memory data top byte is returned in the push data[15:8]
 * so that the software can learn if location was already locked.
 * If match is not found and there is room for the entry then new entry is added with data[31]
 * set indicating that the entry is now locked. If match is not found and there is no free entry
 * then no memory update is performed and (0x000000ff) is returned.
 */
__intrinsic
void cmd_cls_cam_lookup24_add_lock_ptr40(
    __xread void *data_out,
    __xwrite void *data_in,
    volatile void __addr40 __cls *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
);



/** Local scratch Content Addressable Memory lookup 24-bits with add extend.
 *
 * @param data_out Data looked up
 * @param data_in  Data to lookup
 * @param address  Local scratch CAM address to lookup
 * @param count    Size of CAM in number of 64-bit words (1-16)
 * @param sync     Type of synchronization (sig_done or ctx_swap)
 * @param sig_ptr  Signal to be raised upon completion
 *
 * Use 32-bit values from transfer register and compare lower 24-bits with input data.
 * If match is found then bit[31] of the matching memory data location is set indicating that the
 * entry is now locked. The pre-modified memory data top byte is returned in the push data[15:8]
 * so that the software can learn if location was already locked.
 * If match is not found and there is no free entry then set bit[31] of the first memory location used for the
 * CAM command. It will also push back the previous value of bit[31] of the first memory location in bit[15] of the
 * push data (which is normally zero) along with 0xff in push data[7:0].
 */
__intrinsic
void cmd_cls_cam_lookup24_add_extend_ptr32(
    __xread void *data_out,
    __xwrite void *data_in,
    volatile void __cls *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
);


/** 40-bit Local scratch Content Addressable Memory lookup 24-bits with add extend.
 *
 * @param data_out Data looked up
 * @param data_in  Data to lookup
 * @param address  40 bit CAM address to lookup where upper 6 bits indicate the island.
 *                  See 6xxx databook for recommended addressing mode.
 * @param count    Size of CAM in number of 64-bit words (1-16)
 * @param sync     Type of synchronization (sig_done or ctx_swap)
 * @param sig_ptr  Signal to be raised upon completion
 *
 * Use 32-bit values from transfer register and compare lower 24-bits with input data.
 * If match is found then bit[31] of the matching memory data location is set indicating that the
 * entry is now locked. The pre-modified memory data top byte is returned in the push data[15:8]
 * so that the software can learn if location was already locked.
 * If match is not found and there is no free entry then set bit[31] of the first memory location used for the
 * CAM command. It will also push back the previous value of bit[31] of the first memory location in bit[15] of the
 * push data (which is normally zero) along with 0xff in push data[7:0].
 */
__intrinsic
void cmd_cls_cam_lookup24_add_extend_ptr40(
    __xread void *data_out,
    __xwrite void *data_in,
    volatile void __addr40 __cls *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
);

/** Local scratch Content Addressable Memory lookup 16-bits.
 *
 * @param data_out Data looked up
 * @param data_in  Data to lookup
 * @param address  Local scratch CAM address to lookup
 * @param count    Size of CAM in number of 64-bit words (1-32)
 * @param sync     Type of synchronization (sig_done or ctx_swap)
 * @param sig_ptr  Signal to be raised upon completion
 *
 * Fetch 16-bit values from transfer register and compare with lower 16-bits of input data.
 */
__intrinsic
void cmd_cls_cam_lookup16_ptr32(
    __xread void *data_out,
    __xwrite void *data_in,
    volatile void __cls *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
);

/** 40-bit Local scratch Content Addressable Memory lookup 16-bits.
 *
 * @param data_out Data looked up
 * @param data_in  Data to lookup
 * @param address  40 bit CAM address to lookup where upper 6 bits indicate the island.
 *                  See 6xxx databook for recommended addressing mode.
 * @param count    Size of CAM in number of 64-bit words (1-32)
 * @param sync     Type of synchronization (sig_done or ctx_swap)
 * @param sig_ptr  Signal to be raised upon completion
 *
 * Fetch 16-bit values from transfer register and compare with lower 16-bits of input data.
 */
__intrinsic
void cmd_cls_cam_lookup16_ptr40(
    __xread void *data_out,
    __xwrite void *data_in,
    volatile void __addr40 __cls *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
);

/** Local scratch Content Addressable Memory lookup 16-bits with add.
 *
 * @param data_out Data looked up
 * @param data_in  Data to lookup
 * @param address  Local scratch CAM address to lookup
 * @param count    Size of CAM in number of 64-bit words (1-16)
 * @param sync     Type of synchronization (sig_done or ctx_swap)
 * @param sig_ptr  Signal to be raised upon completion
 *
 * Use 16-bit values from transfer register and compare with lower 16-bits of input data.
 * If no match was found, insert 16-bit input data into first zero entry in the CAM.
 */
__intrinsic
void cmd_cls_cam_lookup16_add_ptr32(
    __xread void *data_out,
    __xwrite void *data_in,
    volatile void __cls *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
);

/** 40-bit Local scratch Content Addressable Memory lookup 16-bits with add.
 *
 * @param data_out Data looked up
 * @param data_in  Data to lookup
 * @param address  40 bit CAM address to lookup where upper 6 bits indicate the island.
 *                  See 6xxx databook for recommended addressing mode.
 * @param count    Size of CAM in number of 64-bit words (1-16)
 * @param sync     Type of synchronization (sig_done or ctx_swap)
 * @param sig_ptr  Signal to be raised upon completion
 *
 * Use 16-bit values from transfer register and compare with lower 16-bits of input data.
 * If no match was found, insert 16-bit input data into first zero entry in the CAM.
 */
__intrinsic
void cmd_cls_cam_lookup16_add_ptr40(
    __xread void *data_out,
    __xwrite void *data_in,
    volatile void __addr40 __cls *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
);


/** Local scratch Content Addressable Memory lookup 8-bits.
 *
 * @param data_out Data looked up
 * @param data_in  Data to lookup
 * @param address  Local scratch CAM address to lookup
 * @param count    Size of CAM in number of 64-bit words (1-32)
 * @param sync     Type of synchronization (sig_done or ctx_swap)
 * @param sig_ptr  Signal to be raised upon completion
 *
 * Use 8-bit values from transfer register and compare with lower 8-bits of input data.
 */
__intrinsic
void cmd_cls_cam_lookup8_ptr32(
    __xread void *data_out,
    __xwrite void *data_in,
    volatile void __cls *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
);

/** 40-bit Local scratch Content Addressable Memory lookup 8-bits.
 *
 * @param data_out Data looked up
 * @param data_in  Data to lookup
 * @param address  40 bit CAM address to lookup where upper 6 bits indicate the island.
 *                  See 6xxx databook for recommended addressing mode.
 * @param count    Size of CAM in number of 64-bit words (1-32)
 * @param sync     Type of synchronization (sig_done or ctx_swap)
 * @param sig_ptr  Signal to be raised upon completion
 *
 * Use 8-bit values from transfer register and compare with lower 8-bits of input data.
 */
__intrinsic
void cmd_cls_cam_lookup8_ptr40(
    __xread void *data_out,
    __xwrite void *data_in,
    volatile void __addr40 __cls *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
);

/** Local scratch Content Addressable Memory lookup 8-bits with add.
 *
 * @param data_out Data looked up
 * @param data_in  Data to lookup
 * @param address  Local scratch CAM address to lookup
 * @param count    Size of CAM in number of 64-bit words (1-16)
 * @param sync     Type of synchronization (sig_done or ctx_swap)
 * @param sig_ptr  Signal to be raised upon completion
 *
 * Use 8-bit values from transfer register and compare with lower 8-bits of input data.
 * If no match was found, insert 8-bit input data into first zero entry in the CAM.
 */
__intrinsic
void cmd_cls_cam_lookup8_add_ptr32(
    __xread void *data_out,
    __xwrite void *data_in,
    volatile void __cls *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
);

/** 40-bit Local scratch Content Addressable Memory lookup 8-bits with add.
 *
 * @param data_out Data looked up
 * @param data_in  Data to lookup
 * @param address  40 bit CAM address to lookup where upper 6 bits indicate the island.
 *                  See 6xxx databook for recommended addressing mode.
 * @param count    Size of CAM in number of 64-bit words (1-16)
 * @param sync     Type of synchronization (sig_done or ctx_swap)
 * @param sig_ptr  Signal to be raised upon completion
 *
 * Use 8-bit values from transfer register and compare with lower 8-bits of input data.
 * If no match was found, insert 8-bit input data into first zero entry in the CAM.
 */
__intrinsic
void cmd_cls_cam_lookup8_add_ptr40(
    __xread void *data_out,
    __xwrite void *data_in,
    volatile void __addr40 __cls *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
);








/** @}
 * @name TCAM functions for Cluster Local Scratch
 * @{
 */


/** Local scratch TCAM lookup word.
 *
 * @param data_out Data looked up
 * @param data_in  Data to lookup
 * @param address  Local scratch TCAM address to lookup
 * @param count    Number of 64-bit words
 * @param sync     Type of synchronization (sig_done or ctx_swap)
 * @param sig_ptr  Signal to be raised upon completion
 *
 * Compare whole 32-bit TCAM word with 32-bit input data.
 */
__intrinsic
void cmd_cls_tcam_lookup_ptr32(
    __xread void *data_out,
    __xwrite void *data_in,
    volatile void __cls *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
);


/** 40-bit Local scratch TCAM lookup word.
 *
 * @param data_out Data looked up
 * @param data_in  Data to lookup
 * @param address  40 bit TCAM address to lookup where upper 6 bits indicate the island.
 *                  See 6xxx databook for recommended addressing mode.
 * @param count    Number of 64-bit words
 * @param sync     Type of synchronization (sig_done or ctx_swap)
 * @param sig_ptr  Signal to be raised upon completion
 *
 * Compare whole 32-bit TCAM word with 32-bit input data.
 */
__intrinsic
void cmd_cls_tcam_lookup_ptr40(
    __xread void *data_out,
    __xwrite void *data_in,
    volatile void __addr40 __cls *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
);


/** Local scratch TCAM lookup 24-bits.
 *
 * @param data_out Data looked up
 * @param data_in  Data to lookup
 * @param address  Local scratch TCAM address to lookup
 * @param count    Number of 64-bit words
 * @param sync     Type of synchronization (sig_done or ctx_swap)
 * @param sig_ptr  Signal to be raised upon completion
 *
 * Compare lower 24-bits of TCAM word with lower 24-bits of input data.
 * The upper 8 bits of TCAM match word and TCAM mask are ignored.
 */
__intrinsic
void cmd_cls_tcam_lookup24_ptr32(
    __xread void *data_out,
    __xwrite void *data_in,
    volatile void __cls *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
);


/** 40-bit Local scratch TCAM lookup 24-bits.
 *
 * @param data_out Data looked up
 * @param data_in  Data to lookup
 * @param address  40 bit TCAM address to lookup where upper 6 bits indicate the island.
 *                  See 6xxx databook for recommended addressing mode.
 * @param count    Number of 64-bit words
 * @param sync     Type of synchronization (sig_done or ctx_swap)
 * @param sig_ptr  Signal to be raised upon completion
 *
 * Compare lower 24-bits of TCAM word with lower 24-bits of input data.
 * The upper 8 bits of TCAM match word and TCAM mask are ignored.
 */
__intrinsic
void cmd_cls_tcam_lookup24_ptr40(
    __xread void *data_out,
    __xwrite void *data_in,
    volatile void __addr40 __cls *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
);

/** Local scratch TCAM lookup 16-bits.
 *
 * @param data_out Data looked up
 * @param data_in  Data to lookup
 * @param address  Local scratch TCAM address to lookup
 * @param count    Number of 64-bit words
 * @param sync     Type of synchronization (sig_done or ctx_swap)
 * @param sig_ptr  Signal to be raised upon completion
 *
 * Split TCAM match word into two 16-bit words
 * and compare each with lower 16-bits of input data.
 */
__intrinsic
void cmd_cls_tcam_lookup16_ptr32(
    __xread void *data_out,
    __xwrite void *data_in,
    volatile void __cls *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
);


/** 40-bit Local scratch TCAM lookup 16-bits.
 *
 * @param data_out Data looked up
 * @param data_in  Data to lookup
 * @param address  40 bit TCAM address to lookup where upper 6 bits indicate the island.
 *                  See 6xxx databook for recommended addressing mode.
 * @param count    Number of 64-bit words
 * @param sync     Type of synchronization (sig_done or ctx_swap)
 * @param sig_ptr  Signal to be raised upon completion
 *
 * Split TCAM match word into two 16-bit words
 * and compare each with lower 16-bits of input data.
 */
__intrinsic
void cmd_cls_tcam_lookup16_ptr40(
    __xread void *data_out,
    __xwrite void *data_in,
    volatile void __addr40 __cls *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
);


/** Local scratch TCAM lookup 8-bits.
 *
 * @param data_out Data looked up
 * @param data_in  Data to lookup
 * @param address  Local scratch TCAM address to lookup
 * @param count    Number of 64-bit words
 * @param sync     Type of synchronization (sig_done or ctx_swap)
 * @param sig_ptr  Signal to be raised upon completion
 *
 * Split TCAM match word into four 8-bit words and compare each with lower 8-bits of input data.
 */
__intrinsic
void cmd_cls_tcam_lookup8_ptr32(
    __xread void *data_out,
    __xwrite void *data_in,
    volatile void __cls *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
);


/** 40-bit Local scratch TCAM lookup 8-bits.
 *
 * @param data_out Data looked up
 * @param data_in  Data to lookup
 * @param address  40 bit TCAM address to lookup where upper 6 bits indicate the island.
 *                  See 6xxx databook for recommended addressing mode.
 * @param count    Number of 64-bit words
 * @param sync     Type of synchronization (sig_done or ctx_swap)
 * @param sig_ptr  Signal to be raised upon completion
 *
 * Split TCAM match word into four 8-bit words and compare each with lower 8-bits of input data.
 */
__intrinsic
void cmd_cls_tcam_lookup8_ptr40(
    __xread void *data_out,
    __xwrite void *data_in,
    volatile void __addr40 __cls *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
);

/** @}
 * @}
 */

#endif /* __NFP_CLS_H__ */
