/*
 * Copyright (C) 2008-2014 Netronome Systems, Inc.  All rights reserved.
 */

/* NFP MEM LOAD_BALANCE intrinsic function definitions */

#ifndef __NFP_CLUSTER_TARGET_H__
#define __NFP_CLUSTER_TARGET_H__

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


/** @file nfp_cluster_target.h
* @addtogroup ct Cluster Target intrinsics
* @{
*/

/** Enum for xfer register or csr register */
typedef enum CLUSTER_TARGET_REGISTER_TYPE
{
    CT_REGISTER_TYPE_XFER           = 0x00,      /**< xfer register type.   */
    CT_REGISTER_TYPE_CSR            = 0x01       /**< CSR register type. */

}  CLUSTER_TARGET_REGISTER_TYPE;


/** Enum for addressing mode.  */
typedef enum CLUSTER_TARGET_ADDRESS_MODE
{
    CT_ADDRESS_MODE_INDEX               = 0x00,     /**< NN register FIFO mode is used. */
    CT_ADDRESS_MODE_ABSOLUTE            = 0x01      /**< NN register number specified is the first to be written to.   */
}  CLUSTER_TARGET_ADDRESS_MODE;

#ifdef __BIGENDIAN

/** Layout of 32-bit address used with XPB commands. */
typedef union cluster_target_xpb_address_format_t
{
    struct
    {
        uint32_t reserved_2             : 1;    /**< Reserved. */
        uint32_t global_xpb             : 1;    /**< Global steering bit. Local = 0, global = 1. */
        uint32_t target_island          : 6;    /**< Target island. 0 if own island (local). */
        uint32_t slave                  : 2;    /**< Slave island number, for those islands that have slave XPB buses. */
        uint32_t device                 : 6;    /**< XPB device number with the island/slave island. */
        uint32_t address                : 14;   /**< XPB register address within the XPB target. */
        uint32_t reserved_1             : 2;    /**< Reserved. */
    };
    uint32_t value;                             /**< Accessor to entire lookup detail structure. */
} cluster_target_xpb_address_format_t;

#else /* __LITTLEENDIAN */

typedef union cluster_target_xpb_address_format_t
{
    struct
    {
        uint32_t reserved_1             : 2;
        uint32_t address                : 14;
        uint32_t device                 : 6;
        uint32_t slave                  : 2;
        uint32_t target_island          : 6;
        uint32_t global_xpb             : 1;
        uint32_t reserved_2             : 1;
    };
    uint32_t value;
} cluster_target_xpb_address_format_t;

#endif



#ifdef __BIGENDIAN

/** Layout of 32-bit address used with reflector commands. */
typedef union cluster_target_reflect_address_format_t
{
    struct
    {
        uint32_t reserved_4                             : 2;    /**< Reserved. */
        uint32_t remote_island                          : 6;    /**< Island Id of remote master. */
        uint32_t reserved_3                             : 7;    /**< Reserved. */
        CLUSTER_TARGET_REGISTER_TYPE register_type          : 1;    /**< Register type where 0 = transfer registers, 1 = CSR registers.*/
        uint32_t reserved_2                             : 2;    /**< Reserved. */
        uint32_t master                                 : 4;    /**< Master within specified island. See NFP-6xxx Pull IDs in 6xxx databook. */
        uint32_t address                                : 8;    /**< Transfer Register Address or Local ME CSR Address, depending on XferCsrRegSel. */
        uint32_t reserved_1                             : 2;    /**< Reserved. */
    };
    uint32_t value;                             /**< Accessor to entire lookup detail structure. */
} cluster_target_reflect_address_format_t;

#else /* __LITTLEENDIAN */

typedef union cluster_target_reflect_address_format_t
{
    struct
    {
        uint32_t reserved_1                     : 2;
        uint32_t address                        : 8;
        uint32_t master                         : 4;
        uint32_t reserved_2                     : 2;
        CLUSTER_TARGET_REGISTER_TYPE registers      : 1;
        uint32_t reserved_3                     : 7;
        uint32_t remote_island                  : 6;
        uint32_t reserved_4                     : 2;
    };
    uint32_t value;
} cluster_target_reflect_address_format_t;

#endif


#ifdef __BIGENDIAN

/** Layout of 32-bit address used with interthread signalling commands. */
typedef union cluster_target_signal_me_address_format_t
{
    struct
    {
        uint32_t reserved_3             : 2;    /**< Reserved. */
        uint32_t remote_island          : 6;    /**< Island id of remote master. */
        uint32_t reserved_2             : 11;   /**< Reserved. */
        uint32_t remote_master          : 4;    /**< Remote master id within specified island. See NFP-6xxx Pull IDs in 6xxx databook. */
        uint32_t remote_context         : 3;    /**< Context number (thread number) of remote thread. */
        uint32_t signal_number          : 4;    /**< Signal number or signal reference. */
        uint32_t reserved_1             : 2;    /**< Reserved. */
    };
    uint32_t value;                             /**< Accessor to entire lookup detail structure. */
} cluster_target_signal_me_address_format_t;

#else /* __LITTLEENDIAN */

typedef union cluster_target_signal_me_address_format_t
{
    struct
    {
        uint32_t reserved_1             : 2;
        uint32_t signal_number          : 4;
        uint32_t remote_context         : 3;
        uint32_t remote_master          : 4;
        uint32_t reserved_2             : 11;
        uint32_t remote_island          : 6;
        uint32_t reserved_3             : 2;
    };
    uint32_t value;
} cluster_target_signal_me_address_format_t;

#endif


#ifdef __BIGENDIAN

/** Layout of 32-bit address used with the next neighbour write command. */
typedef union cluster_target_next_neighbour_write_address_format_t
{
    struct
    {
        uint32_t reserved_3                         : 2;    /**< Reserved. */
        uint32_t remote_island                      : 6;    /**< Island id of remote master. */
        uint32_t reserved_2                         : 3;    /**< Reserved. */
        uint32_t master                             : 4;    /**< Master within specified island. See NFP-6xxx Pull IDs in 6xxx databook. */
        uint32_t signal_number                      : 7;    /**< If non-zero, signal number to send to the ME on completion. */
        CLUSTER_TARGET_ADDRESS_MODE address_mode        : 1;    /**< Address mode where 0 = NN register FIFO mode, 1 = absolute mode.*/
        uint32_t NN_register_number                 : 7;    /**< Next neighbour register number. */
        uint32_t reserved_1                         : 2;    /**< Reserved. */
    };
    uint32_t value;                                         /**< Accessor to entire lookup detail structure. */
} cluster_target_next_neighbour_write_address_format_t;

#else /* __LITTLEENDIAN */

typedef union cluster_target_next_neighbour_write_address_format_t
{
    struct
    {
        uint32_t reserved_1                         : 2;
        uint32_t NN_register_number                 : 7;
        CLUSTER_TARGET_ADDRESS_MODE address_mode        : 1;
        uint32_t signal_number                      : 7;
        uint32_t master                             : 4;
        uint32_t reserved_2                         : 3;
        uint32_t remote_island                      : 6;
        uint32_t reserved_3                         : 2;
    };
    uint32_t value;
} cluster_target_next_neighbour_write_address_format_t;

#endif

/** @name General memory access functions for Cluster Target
* @{
*/

/** Write data to XPB target over the XPB bus.
*
* @param xfer              Transfer registers containing data to write.
* @param address           32 bit pointer.
* @param count             Length in 32-bit words to write (valid values 1 - 16).
* @param sync              Type of synchronization to use (sig_done or ctx_swap).
* @param sig_ptr           Signal to raise upon completion.
*
* Transactions from the ARM may be global, but from other islands must be to within their island or its slaves.
*
* @note The valid range for count is between 1 and 16 for Silicon revision B0 or higher and
*       between 1 and 14 for Silicon revision less than B0.
*       See Errata 'Misc engine Pull-buffer which stores Pull-data can erroneously overflow
*       if the post-pull id Command FIFO is not full.'
*/
__intrinsic
void cmd_cluster_target_xpb_write(
    __xwrite void *xfer,
    volatile cluster_target_xpb_address_format_t *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
);


/** Read data from XPB target over the XPB bus.
*
* @param xfer              Transfer registers containing data read.
* @param address           32 bit pointer.
* @param count             Length in 32-bit words to write (valid values 1 - 16).
* @param sync              Type of synchronization to use (sig_done or ctx_swap).
* @param sig_ptr           Signal to raise upon completion.
*
* Transactions from the ARM may be global, but from other islands must be to within their island or its slaves.
*
*/
__intrinsic
void cmd_cluster_target_xpb_read(
    __xread void *xfer,
    volatile cluster_target_xpb_address_format_t *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
);



/** Write data from transfer registers of intiating ME to the transfer registers of another ME.
*
* @param xfer              Transfer registers containing data to write.
* @param address           32 bit pointer.
* @param count             Length in 32-bit words to write (valid values 1 - 16).
*
* Read data from the initiating ME and write it to a remote ME without providing a signal.  Specify
* command arguments as per cluster_target_reflect_address_format_t command address field.
*
* @note The valid range for count is between 1 and 16 for Silicon revision B0 or higher and
*       between 1 and 14 for Silicon revision less than B0.
*       See Errata 'Misc engine Pull-buffer which stores Pull-data can erroneously overflow
*       if the post-pull id Command FIFO is not full.'
*/
__intrinsic
void cmd_cluster_target_reflect_write_sig_none(
    __xwrite void *xfer,
    volatile cluster_target_reflect_address_format_t *address,
    uint32_t count
);


/** Read data from transfer registers of remote ME and write to the transfer registers of the initiating ME.
*
* @param xfer              Transfer registers containing data to read.
* @param address           32 bit pointer.
* @param count             Length in 32-bit words to write (valid values 1 - 16).
*
* Read data from the remote ME and write it to a initiating ME without providing a signal.  Specify
* command arguments as per cluster_target_reflect_address_format_t command address field.
*
* @note The valid range for count is between 1 and 16 for Silicon revision B0 or higher and
*       between 1 and 14 for Silicon revision less than B0.
*       See Errata 'Misc engine Pull-buffer which stores Pull-data can erroneously overflow
*       if the post-pull id Command FIFO is not full.'
*/
__intrinsic
void cmd_cluster_target_reflect_read_sig_none(
    __xread void *xfer,
    volatile cluster_target_reflect_address_format_t  *address,
    uint32_t count
);



/** Write data from transfer registers of intiating ME to the transfer registers of another ME.
*
* @param xfer              Transfer registers containing data to write.
* @param address           32 bit pointer.
* @param count             Length in 32-bit words to write (valid values 1 - 16).
* @param sync              Type of synchronization to use (sig_done or ctx_swap).
* @param sig_ptr           Signal to raise upon completion.
*
* Read data from the initiating ME and write it to a remote ME signallng both MEs. Specify
* command arguments as per cluster_target_reflect_address_format_t command address field.
*
* @note The valid range for count is between 1 and 16 for Silicon revision B0 or higher and
*       between 1 and 14 for Silicon revision less than B0.
*       See Errata 'Misc engine Pull-buffer which stores Pull-data can erroneously overflow
*       if the post-pull id Command FIFO is not full.'
*/
__intrinsic
void cmd_cluster_target_reflect_write_sig_both(
    __xwrite void *xfer,
    volatile cluster_target_reflect_address_format_t *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
);


/** Read data from transfer registers of remote ME and write to the transfer registers of the initiating ME.
*
* @param xfer              Transfer registers containing data to read.
* @param address           32 bit pointer.
* @param count             Length in 32-bit words to write (valid values 1 - 16).
* @param sync              Type of synchronization to use (sig_done or ctx_swap).
* @param sig_ptr           Signal to raise upon completion.
*
* Read data from the remote ME and write it to a initiating ME signallng both MEs.  Specify
* command arguments as per cluster_target_reflect_address_format_t command address field.
*
* @note The valid range for count is between 1 and 16 for Silicon revision B0 or higher and
*       between 1 and 14 for Silicon revision less than B0.
*       See Errata 'Misc engine Pull-buffer which stores Pull-data can erroneously overflow
*       if the post-pull id Command FIFO is not full.'
*/
__intrinsic
void cmd_cluster_target_reflect_read_sig_both(
    __xread void *xfer,
    volatile cluster_target_reflect_address_format_t *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
);

/** Write data from transfer registers of intiating ME to the transfer registers of another ME.
*
* @param xfer              Transfer registers containing data to write.
* @param address           32 bit pointer.
* @param count             Length in 32-bit words to write (valid values 1 - 16).
* @param sync              Type of synchronization to use (sig_done or ctx_swap).
* @param sig_ptr           Signal to raise upon completion.
*
* Read data from the initiating ME and write it to a remote ME signallng the initiating ME. Specify
* command arguments as per cluster_target_reflect_address_format_t command address field.
*
* @note The valid range for count is between 1 and 16 for Silicon revision B0 or higher and
*       between 1 and 14 for Silicon revision less than B0.
*       See Errata 'Misc engine Pull-buffer which stores Pull-data can erroneously overflow
*       if the post-pull id Command FIFO is not full.'
*/
__intrinsic
void cmd_cluster_target_reflect_write_sig_init(
    __xwrite void *xfer,
    volatile cluster_target_reflect_address_format_t *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
);


/** Read data from transfer registers of remote ME and write to the transfer registers of the initiating ME.
*
* @param xfer              Transfer registers containing data to read.
* @param address           32 bit pointer.
* @param count             Length in 32-bit words to write (valid values 1 - 16).
* @param sync              Type of synchronization to use (sig_done or ctx_swap).
* @param sig_ptr           Signal to raise upon completion.
*
* Read data from the remote ME and write it to a initiating ME signallng intiating ME.  Specify
* command arguments as per cluster_target_reflect_address_format_t command address field.
*
* @note The valid range for count is between 1 and 16 for Silicon revision B0 or higher and
*       between 1 and 14 for Silicon revision less than B0.
*       See Errata 'Misc engine Pull-buffer which stores Pull-data can erroneously overflow
*       if the post-pull id Command FIFO is not full.'
*/
__intrinsic
void cmd_cluster_target_reflect_read_sig_init(
    __xread void *xfer,
    volatile cluster_target_reflect_address_format_t *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
);


/** Write data from transfer registers of intiating ME to the transfer registers of another ME.
*
* @param xfer              Transfer registers containing data to write.
* @param address           32 bit pointer.
* @param count             Length in 32-bit words to write (valid values 1 - 16).
* @param sync              Type of synchronization to use (sig_done only).
* @param sig_ptr           Signal to raise upon completion.
*
* Read data from the initiating ME and write it to a remote ME signallng the remote ME. Specify
* command arguments as per cluster_target_reflect_address_format_t command address field.
*
* @note The valid range for count is between 1 and 16 for Silicon revision B0 or higher and
*       between 1 and 14 for Silicon revision less than B0.
*       See Errata 'Misc engine Pull-buffer which stores Pull-data can erroneously overflow
*       if the post-pull id Command FIFO is not full.'
*/
__intrinsic
void cmd_cluster_target_reflect_write_sig_remote(
    __xwrite void *xfer,
    volatile cluster_target_reflect_address_format_t *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
);


/** Read data from transfer registers of remote ME and write to the transfer registers of the initiating ME.
*
* @param xfer              Transfer registers containing data to read.
* @param address           32 bit pointer.
* @param count             Length in 32-bit words to write (valid values 1 - 16).
* @param sync              Type of synchronization to use (sig_done only).
* @param sig_ptr           Signal to raise upon completion.
*
* Read data from the remote ME and write it to a initiating ME signallng remote ME.  Specify
* command arguments as per cluster_target_reflect_address_format_t command address field.
*
* @note The valid range for count is between 1 and 16 for Silicon revision B0 or higher and
*       between 1 and 14 for Silicon revision less than B0.
*       See Errata 'Misc engine Pull-buffer which stores Pull-data can erroneously overflow
*       if the post-pull id Command FIFO is not full.'
*/
__intrinsic
void cmd_cluster_target_reflect_read_sig_remote(
    __xread void *xfer,
    volatile cluster_target_reflect_address_format_t *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
);


/** Send a signal to another ME.
*
* @param address           32 bit pointer.
*
* Signal another ME as specified in the address.
*
* @code
*  {
*    cluster_target_signal_me_address_format_t    signal_command;
*    SIGNAL                                       signal;
*
*    signal_command.value = 0;
*    signal_command.remote_island = 34;          // island 34 (i34)
*    signal_command.remote_context = 0;          // thread/context 0
*    signal_command.remote_master = 5;           // Master IDs 4..15 for MEs 0..11 on i32..i38
*    signal_command.signal_number = __signal_number(&signal);
*
*    // signal remote island 34, ME 1
*    cmd_cluster_target_sig_me_ctx(&signal_command);
*  }
* @endcode
*/
__intrinsic
void cmd_cluster_target_sig_me_ctx(
    volatile cluster_target_signal_me_address_format_t *address
);

/** Write data to the next neighbor register or FIFO of a ME.
*
* @param xfer              Transfer registers containing data to write.
* @param address           32 bit pointer.
* @param count             Length in 32-bit words to write (valid values 1 - 16).
* @param sync              Type of synchronization to use (sig_done or ctx_swap).
* @param sig_ptr           Signal to raise upon completion.
*
* A ME does a write data to the next neighbour from the xfer registers.
*
* @note The valid range for count is between 1 and 16 for Silicon revision B0 or higher and
*       between 1 and 14 for Silicon revision less than B0.
*       See Errata 'Misc engine Pull-buffer which stores Pull-data can erroneously overflow
*       if the post-pull id Command FIFO is not full.'
*/
__intrinsic
void cmd_cluster_target_next_neighbour_write(
    __xwrite void *xfer,
    volatile cluster_target_next_neighbour_write_address_format_t *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
);

/** Write data to the next neighbor register or FIFO of a ME.
*
* @param xfer              Transfer registers containing data to write.
* @param address           32 bit pointer.
* @param max_nn            Maximum number of 32-bit words to read (1 - 16)
* @param ind               Indirect word
* @param sync              Type of synchronization to use (sig_done or ctx_swap).
* @param sig_ptr           Signal to raise upon completion.
*
* A ME does a write data to the next neighbour but the data can be pulled from another ME.
* If ctnn write without indirect address then island master, data master, signal master, signal number can be left 0.
* See DB Figure 9.10 for an example of using indirect address.
*
* @note The valid range for max_nn is between 1 and 16 for Silicon revision B0 or higher and
*       between 1 and 14 for Silicon revision less than B0.
*       See Errata 'Misc engine Pull-buffer which stores Pull-data can erroneously overflow
*       if the post-pull id Command FIFO is not full.'
*
* @ind_6000
*
* Below is an example where an island (i.e. i33) does next neighbour write to i32 and the data is pulled from i34.
*
* @code
*
*  cluster_target_next_neighbour_write_address_format_t    command;
*  uint32_t    master = 4;
*  uint32_t    signal_master = 4;
*  SIGNAL          sig;
*  uint32_t    count = 16;
*  generic_ind_t   ind;
*  uint32_t    data_island = 34; // i34
*
*  xfer_write[0] = 0x12345678;
*  xfer_write[1] = 0x12345678+10;
*
*  command.value = 0;
*  command.remote_island = 32;       // i32
*  command.master = master;
*  command.signal_number = 0x1;
*  command.address_mode = CT_ADDRESS_MODE_ABSOLUTE;
*  command.NN_register_number = 0;     // start at register 0
*
*  ovr_init(&ind, ovr_signal_island_and_data_master | ovr_signal_number | ovr_length);
*  ovr_set(
*         &ind,
*         ovr_signal_island_and_data_master,
*         data_island << 8 | master << 4 | signal_master
*         );
*  ovr_set(&ind, ovr_signal_number, 1);
*  ovr_set(&ind, ovr_length, count - 1);
*
*  cmd_cluster_target_next_neighbour_write_ind
*        (
*        (void *)&xfer_write[0],
*        &command,
*        16,
*        ind,
*        ctx_swap,
*        &sig
*        );
* @endcode
*
*
*/
__intrinsic
void cmd_cluster_target_next_neighbour_write_ind(
    __xwrite void *xfer,
    volatile cluster_target_next_neighbour_write_address_format_t *address,
    uint32_t max_nn,
    generic_ind_t ind,
    sync_t sync,
    SIGNAL *sig_ptr
);

/** @}
* @}
*/

#endif /* __NFP_CLUSTER_TARGET_H__ */
