/*
 * Copyright (C) 2008-2014 Netronome Systems, Inc.  All rights reserved.
 */

/* NFP ILA Express intrinsic function definitions */

#ifndef __NFP_ILA_H__
#define __NFP_ILA_H__

#include <nfp.h>
#include <stdint.h>

#ifndef NFP_LIB_ANY_NFCC_VERSION
    #if (!defined(__NFP_TOOL_NFCC) ||                       \
        (__NFP_TOOL_NFCC < NFP_SW_VERSION(5, 0, 0, 0)) ||   \
        (__NFP_TOOL_NFCC > NFP_SW_VERSION(6, 1, 0, 255)))
        #error "This standard library is not supported for the version of the SDK currently in use."
    #endif
#endif

/** @file nfp_ila.h
 * @addtogroup ila ILA Intrinsics
 * @{
 */

/** Enum for ILA operation mode */
typedef enum ILA_OPERATION_MODE
{
    ILA_OPERATION_TCAM                  = 0x00,      /**< TCAM operations.   */
    ILA_OPERATION_ACCELERATOR           = 0x01       /**< ACCELERATOR operations. */

}  ILA_OPERATION_MODE;

/** Enum for reset logic for ILA_OPERATION_MODE setting */
typedef enum ILA_LOGIC_RESET
{
    ILA_LOGIC_OUT_OF_RESET              = 0x00,      /**< Out of reset.   */
    ILA_LOGIC_IN_RESET                  = 0x01       /**< In reset. */

}  ILA_LOGIC_RESET;


/** Enum for ILA channels */
typedef enum ILA_CHANNEL
{
    ILA_CHANNEL_0                       = 0x00,      /**< ILA channel 0.   */
    ILA_CHANNEL_1                       = 0x01       /**< ILA channel 1. */

}  ILA_CHANNEL;

#ifdef __BIGENDIAN

/** Layout of config control register for ILA. This is used when configuring ILA for TCAM or ACCELERATOR using cmd_cluster_target_xpb_write.*/
typedef union ila_config_control_register_format_t
{
    struct
    {
        uint32_t reserved_2                 : 11;   /**< Reserved. */
        uint32_t rx_fifo_pack               : 1;    /**< ILA RX FIFO packing (0=packing enabled / 1=packing disabled) */
        uint32_t tx_idle_count              : 10;   /**< ILA tx idle count. */
        uint32_t rx_fifo_size               : 1;    /**< ILA Rx FIFO size (0=4KByte / 1=8KByte). */
        uint32_t rx_tx_credit_count         : 1;    /**< ILA Tx credit count enabled (0=disabled / 1=enabled). */
        uint32_t tcam_vendor_select         : 1;    /**< TCAM vendor select (0=select NetLogic TCAM / 1=reserved). */
        uint32_t reserved_1                 : 1;    /**< Reserved. */
        uint32_t error_poison_flag          : 1;    /**< Enables poisoning of data on receipt of ILA Rx error (0=disabled/1=enabled). */
        uint32_t stats_channel_select       : 1;    /**< Select channel statistics (0=channel 0 / 1=channel 1). */
        uint32_t burst_max                  : 1;    /**< Maximum size of burst on ILA bus (0=256 bytes / 1=512 bytes). */
        ILA_LOGIC_RESET logic_reset_flag        : 1;    /**< Reset to the logic affected by ILA mode setting (0=out of reset / 1=in reset). */
        ILA_OPERATION_MODE operation_mode       : 1;    /**< Mode of ILA block (0=TCAM operation / 1= Accelerator operation). */
        uint32_t cpp_addr_mode              : 1;    /**<C PP address bit mode (0=40-bits / 1=32-bits). */
     };
    uint32_t value;                             /**< Accessor to entire structure. */
} ila_config_control_register_format_t;

#else /* __LITTLEENDIAN */

typedef union ila_config_control_register_format_t
{
    struct
    {
        uint32_t cpp_addr_mode              : 1;
        ILA_OPERATION_MODE operation_mode       : 1;
        ILA_LOGIC_RESET logic_reset_flag        : 1;
        uint32_t burst_max                  : 1;
        uint32_t stats_channel_select       : 1;
        uint32_t error_poison_flag          : 1;
        uint32_t reserved_1                 : 1;
        uint32_t tcam_vendor_select         : 1;
        uint32_t rx_tx_credit_count         : 1;
        uint32_t rx_fifo_size               : 1;
        uint32_t tx_idle_count              : 10;
        uint32_t rx_fifo_pack               : 1;
        uint32_t reserved_2                 : 11;
    };
    uint32_t value;
} ila_config_control_register_format_t;

#endif



#ifdef __BIGENDIAN

/** Layout of 32-bit address used with ILA internal commands (cmd_ila_read_internal_ptr40 and cmd_ila_write_internal_ptr40).  */
typedef union ila_internal_address_format_t
{
    struct
    {
        uint32_t reserved               : 12;
        uint32_t target                 : 4;    /**< Internal target number. */
        uint32_t address                : 16;   /**< Internal target address. */
     };
    uint32_t value;                             /**< Accessor to entire  structure. */
} ila_internal_address_format_t;

#else /* __LITTLEENDIAN */

typedef union ila_internal_address_format_t
{
    struct
    {
        uint32_t address                : 16;
        uint32_t target                 : 4;
        uint32_t reserved               : 12;
    };
    uint32_t value;
} ila_internal_address_format_t;

#endif




#ifdef __BIGENDIAN

/** Layout of 32-bit ILA to CPP BAR registers (ILA To CPP BAR Registers). */
typedef union ila_to_cpp_bar_register_format_t
{
    struct
    {
        uint32_t reserved_2             : 2;    /**< Reserved. */
        uint32_t target_id              : 5;    /**< CPP target id. */
        uint32_t token                  : 2;    /**< CPP token field. */
        uint32_t length_select          : 1;    /**< CPP length field (0=32-bit increments/1=64-bit increments. */
        uint32_t reserved_1             : 22;   /**< Reserved. */
     };
    uint32_t value;                             /**< Accessor to entire structure. */
} ila_to_cpp_bar_register_format_t;

#else /* __LITTLEENDIAN */

typedef union ila_to_cpp_bar_register_format_t
{
    struct
    {
        uint32_t reserved_1             : 22;
        uint32_t length_select          : 1;
        uint32_t token                  : 2;
        uint32_t target_id              : 5;
        uint32_t reserved_2             : 2;
    };
    uint32_t value;
} ila_to_cpp_bar_register_format_t;

#endif


#ifdef __BIGENDIAN

/** Layout of 32-bit ILA to CPP BAR registers (CPP To ILA BAR Registers). */
typedef union ila_cpp_to_ila_bar_register_format_t
{
    struct
    {
        uint32_t reserved               : 10;   /**< Reserved. */
        ILA_CHANNEL channel_select          : 1;    /**< Selects which ILA channel to send the command on. */
        uint32_t address                : 21;   /**< In 32-bit mode, ILA address bits [47:27]. In 40-bit mode, ILA address bits [47:35]. */
     };
    uint32_t value;                             /**< Accessor to entire  structure. */
} ila_cpp_to_ila_bar_register_format_t;

#else /* __LITTLEENDIAN */

typedef union ila_cpp_to_ila_bar_register_format_t
{
    struct
    {
        uint32_t address                : 21;
        ILA_CHANNEL channel_select          : 1;
        uint32_t reserved               : 10;
    };
    uint32_t value;
} ila_cpp_to_ila_bar_register_format_t;

#endif



#ifdef __BIGENDIAN

/** Layout of 128-bit ILA DMA Command Register. This breaks down into 4 transfer registers,
 *  where xfer[0] == 127:96, xfer[1] == 95:64 etc..*/
typedef union ila_dma_command_register_format_t
{
    struct
    {
        /*
        * Fourth longword
        */
        uint32_t xfer_length            : 12;  /**< Length of transfer in bytes. This should be length - 1. */
        uint32_t reserved               : 4;   /**< Reserved. */
        uint32_t ila_address_hi         : 16;  /**< Upper 16-bits of 48-bit ILA address. */

        /*
        * Third longword
        */
        uint32_t ila_address_lo         : 30;  /**< Lower 30-bits of 48-bit ILA address. */
        uint32_t complete_indication_hi : 2;   /**< Action on completed command (no signal/generate event/send signal to CPP master). */


        /*
        * Second longword
        */
        uint32_t complete_indication_lo : 16;   /**< Action on completed command (no signal/generate event/send signal to CPP master). */
        uint32_t token                  : 2;    /**< CPP token bits for CPP command. */
        uint32_t signal_both_on_error   : 1;    /**< On ILA Rx Error signal both odd or even (0=disabled / 1=enabled). */
        uint32_t cpp_is_target_64       : 1;    /**< Is CPP target 64 bit (0=32-bit / 1=64-bit). */
        uint32_t cpp_target             : 4;    /**< Target id for CPP transaction. */
        uint32_t cpp_address_hi         : 8;    /**< CPP bus address - higher 8-bit. */

        /*
        * First longword
        */
        uint32_t cpp_address_lo         : 32;    /**< CPP bus address - lower 32-bit. */
     };
    uint32_t value[4];                        /**< Accessor to entire structure. */
} ila_dma_command_register_format_t;

#else /* __LITTLEENDIAN */

typedef union ila_dma_command_register_format_t
{
    struct
    {
        uint32_t cpp_address_lo         : 32;

        uint32_t cpp_address_hi         : 8;
        uint32_t cpp_target             : 4;
        uint32_t cpp_is_target_64       : 1;
        uint32_t signal_both_on_error   : 1;
        uint32_t token                  : 2;
        uint32_t complete_indication_lo : 16;

        uint32_t complete_indication_hi : 2;
        uint32_t ila_address_lo         : 30;

        uint32_t ila_address_hi         : 16;
        uint32_t reserved               : 4;
        uint32_t xfer_length            : 12;
    };
    uint32_t value[4];
} ila_dma_command_register_format_t;

#endif


/** @name General functions for ILA.
 *  @{
 */



/** Write data to ILA atached device.
 *
 * @param data     Xfer register(s) containing data to write to memory.
 * @param address  40 bit address in ILA SRAM to write data to.
 * @param count    Number of 32-bit words to write (1 - 32).
 * @param sync     Type of synchronization to use (sig_done or ctx_swap).
 * @param sig_ptr  Signal to raise upon completion.
 */
__intrinsic
void cmd_ila_write_ptr40(
    __xwrite void *data,
    volatile void __addr40 __mem *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
);

/** Read data from ILA atached device.
 *
 * @param data     Xfer register(s) containing data read from memory.
 * @param address  40 bit address in ILA SRAM to read data from.
 * @param count    Number of 32-bit words to read (1 - 32).
 * @param sync     Type of synchronization to use (sig_done  or ctx_swap).
 * @param sig_ptr  Signal to raise upon completion.
 */
__intrinsic
void cmd_ila_read_ptr40(
    __xread void *data,
    volatile void __addr40 __mem *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
);


/** Write data to ILA attached device. Command will use even signal when write response is received from ILA
 * attached device. If an ILA Rx error is encountered, 4 bytes data is returned with both even and odd
 * signals.
 *
 * @param data          Xfer register(s) containing data to write to memory.
 * @param address       40 bit address in ILA SRAM to write data to.
 * @param count         Number of 32-bit words to write (1 - 32).
 * @param sync          Type of synchronization to use (can only be sig_done).
 * @param sig_pair_ptr  Signal pair to raise upon completion.
 */
__intrinsic
void cmd_ila_write_check_error_ptr40(
    __xwrite void *data,
    volatile void __addr40 __mem *address,
    uint32_t count,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
);

/** Read data from attached ILA device. Command will signal back immediate with read response from ILA Target.
 * If an ILA Rx error is encountered, data is returned with both even and odd signal.
 *
 * @param data          Xfer register(s) containing data read from memory.
 * @param address       40 bit address in ILA SRAM to read data from.
 * @param count         Number of 32-bit words to read (1 - 32).
 * @param sync          Type of synchronization to use (can only be sig_done).
 * @param sig_pair_ptr  Signal pair to raise upon completion.
 */
__intrinsic
void cmd_ila_read_check_error_ptr40(
    __xread void *data,
    volatile void __addr40 __mem *address,
    uint32_t count,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
);

/** Write data to ILA internal target.
 *
 * @param data                  Xfer register(s) containing data to write to memory.
 * @param address               40 bit address in ILA where the lower 32 bits are of format  ila_internal_address_format_t.
 *                               See 6xxx databook for recommended addressing mode for address bits.
 * @param address               32 bit address in ILA of type ila_internal_address_format_t.
 * @param count                 Number of 32-bit words to read (1 - 16).
 * @param sync                  Type of synchronization to use (sig_done  or ctx_swap).
 * @param sig_ptr               Signal to raise upon completion.
 *
 * @see ila_internal_address_format_t for format of 32 bit of the address parameter.
 */
__intrinsic
void cmd_ila_write_internal_ptr40(
    __xwrite void *data,
    volatile void __addr40 __mem *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
);

/** Read data from ILA internal target.
 *
 * @param data                  Xfer register(s) containing data read from memory.
 * @param address               40 bit address in ILA where the lower 32 bits are of format  ila_internal_address_format_t.
 *                               See 6xxx databook for recommended addressing mode for address bits.
 * @param address               32 bit address in ILA of type ila_internal_address_format_t.
 * @param count                 Number of 32-bit words to read (1 - 16).
 * @param sync                  Type of synchronization to use (sig_done  or ctx_swap).
 * @param sig_ptr               Signal to raise upon completion.
 *
 * @see ila_internal_address_format_t for format of 32 bit of the address parameter.
 */
__intrinsic
void cmd_ila_read_internal_ptr40(
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

#endif /* __NFP_ILA_H__ */
