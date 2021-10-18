/*
 * Copyright (C) 2008-2014 Netronome Systems, Inc.  All rights reserved.
 */

#ifndef __NFP_CLS_REFLECT_H__
#define __NFP_CLS_REFLECT_H__

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
 * @addtogroup cls_reflect CLS Reflect Intrinsics
 * @{
 */


/** @name Reflect functions for Cluster Local Scratch
 * @{
 */

/** Copy local transfer registers to remote microengine. Signal local ME (source)
 * after transfer registers are read.
 * This intrinsic was formerly named cls_reflect_from_sig_src().
 *
 * @param data          Data to reflect
 * @param remote_ME     Remote FPC or ME number
 * @param remote_xfer   Remote xfer register reg number
 * @param count         Number of 32-bit words to reflect
 * @param sync          Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr       Signal to raise upon completion
 *
 * @note __xfer_reg_number() can be used to get remote_xfer register number.
 *
 */
void cmd_cls_reflect_write_sig_local(
    __xwrite void *data,
    uint32_t remote_ME,
    uint32_t remote_xfer,
    uint32_t count,
    sync_t sync,
    volatile SIGNAL *sig_ptr
);

/** Copy local transfer registers to remote microengine. Signal local ME (source)
 * after transfer registers are read.
 *
 * @param data          Data to reflect
 * @param remote_island Remote island number (0 for local island)
 * @param remote_ME     Remote FPC or ME number
 * @param remote_xfer   Remote xfer register reg number
 * @param count         Number of 32-bit words to reflect
 * @param sync          Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr       Signal to raise upon completion
 *
 * @note __xfer_reg_number() can be used to get remote_xfer register number.
 *
 */
void cmd_cls_reflect_write_sig_local_ptr40(
    __xwrite void *data,
    uint32_t remote_island,
    uint32_t remote_ME,
    uint32_t remote_xfer,
    uint32_t count,
    sync_t sync,
    volatile SIGNAL *sig_ptr
);


/** Copy local transfer registers to remote microengine.
 *  Signal remote ME (destination) after transfer registers are written.
 *  This intrinsic was formerly named cls_reflect_from_sig_dst().
 *
 * @param data          Data to reflect
 * @param remote_ME     Remote FPC or ME number
 * @param remote_xfer   Remote xfer register reg number
 * @param sig           Triggered signal number
 * @param count         Number of 32-bit words to reflect
 *
 * @note __xfer_reg_number() can be used to get remote_xfer register number.
 *
 */
__intrinsic
void cmd_cls_reflect_write_sig_remote(
    __xwrite void *data,
    uint32_t remote_ME,
    uint32_t remote_xfer,
    int32_t sig,
    uint32_t count
);

/** Copy local transfer registers to remote microengine.
 * Signal remote ME (destination) after transfer registers are written.
 *
 * @param data          Data to reflect
 * @param remote_island Remote island number (0 for local island)
 * @param remote_ME     Remote FPC or ME number
 * @param remote_xfer   Remote xfer register reg number
 * @param sig           Triggered signal number
 * @param count         Number of 32-bit words to reflect
 *
 * @note __xfer_reg_number() can be used to get remote_xfer register number.
 *
 */
__intrinsic
void cmd_cls_reflect_write_sig_remote_ptr40(
    __xwrite void *data,
    uint32_t remote_island,
    uint32_t remote_ME,
    uint32_t remote_xfer,
    int32_t sig,
    uint32_t count
);



/** Copy local transfer registers to remote microengine. Signal local ME after
 *  transfer registers are read and remote ME after transfer registers are written.
 *  This intrinsic was formerly named cls_reflect_from_sig_both().
 *
 * @param data          Data to reflect
 * @param remote_ME     Remote FPC or ME number
 * @param remote_xfer   Remote transfer register
 * @param count         Number of 32-bit words to reflect
 * @param sync          Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr       Signal to raise upon completion
 *
 * @note __xfer_reg_number() can be used to get remote_xfer register number.
 *
 */
__intrinsic
void cmd_cls_reflect_write_sig_both(
    __xwrite void *data,
    uint32_t remote_ME,
    uint32_t remote_xfer,
    uint32_t count,
    sync_t sync,
    volatile SIGNAL *sig_ptr
);

/** Copy local transfer registers to remote microengine. Signal local ME after
 * transfer registers are read and remote ME after transfer registers are written.
 *
 * @param data          Data to reflect
 * @param remote_island Remote island number (0 for local island)
 * @param remote_ME     Remote FPC or ME number
 * @param remote_xfer   Remote transfer register
 * @param count         Number of 32-bit words to reflect
 * @param sync          Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr       Signal to raise upon completion
 *
 * @note __xfer_reg_number() can be used to get remote_xfer register number.
 *
 */
__intrinsic
void cmd_cls_reflect_write_sig_both_ptr40(
    __xwrite void *data,
    uint32_t remote_island,
    uint32_t remote_ME,
    uint32_t remote_xfer,
    uint32_t count,
    sync_t sync,
    volatile SIGNAL *sig_ptr
);

/** Copy transfer registers from remote microengine. Signal remote ME (source)
 *  after transfer registers are read.
 *  This intrinsic was formerly named cls_reflect_to_sig_src().
 *
 * @param data          Data from reflect
 * @param remote_ME     Remote FPC or ME number
 * @param remote_xfer   Remote xfer register reg number
 * @param sig           Triggered signal number
 * @param count         Number of 32-bit words to reflect
 *
 * @note __xfer_reg_number() can be used to get remote_xfer register number.
 *
 */
__intrinsic
void cmd_cls_reflect_read_sig_remote(
    __xread void *data,
    uint32_t remote_ME,
    uint32_t remote_xfer,
    int32_t sig,
    uint32_t count
);

/** Copy transfer registers from remote microengine. Signal remote ME (source)
 * after transfer registers are read.
 *
 * @param data          Data from reflect
 * @param remote_island Remote island number (0 for local island)
 * @param remote_ME     Remote FPC or ME number
 * @param remote_xfer   Remote xfer register reg number
 * @param sig           Triggered signal number
 * @param count         Number of 32-bit words to reflect
 *
 * @note __xfer_reg_number() can be used to get remote_xfer register number.
 *
 */
__intrinsic
void cmd_cls_reflect_read_sig_remote_ptr40(
    __xread void *data,
    uint32_t remote_island,
    uint32_t remote_ME,
    uint32_t remote_xfer,
    int32_t sig,
    uint32_t count
);



/** Copy transfer registers from remote microengine.
 *  Signal local ME (destination) after transfer registers are written.
 *  This intrinsic was formerly named cls_reflect_to_sig_dst().
 *
 * @param data          Data from reflect
 * @param remote_ME     Remote FPC or ME number
 * @param remote_xfer   Remote xfer register reg number
 * @param count         Number of 32-bit words to reflect
 * @param sync          Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr       Signal to raise upon completion
 *
 * @note __xfer_reg_number() can be used to get remote_xfer register number.
 *
 */
__intrinsic
void cmd_cls_reflect_read_sig_local(
    __xread void *data,
    uint32_t remote_ME,
    uint32_t remote_xfer,
    uint32_t count,
    sync_t sync,
    volatile SIGNAL *sig_ptr
);


/** Copy transfer registers from remote microengine.
 * Signal local ME (destination) after transfer registers are written.
 *
 * @param data          Data from reflect
 * @param remote_island Remote island number (0 for local island)
 * @param remote_ME     Remote FPC or ME number
 * @param remote_xfer   Remote xfer register reg number
 * @param count         Number of 32-bit words to reflect
 * @param sync          Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr       Signal to raise upon completion
 *
 * @note __xfer_reg_number() can be used to get remote_xfer register number.
 *
 */
__intrinsic
void cmd_cls_reflect_read_sig_local_ptr40(
    __xread void *data,
    uint32_t remote_island,
    uint32_t remote_ME,
    uint32_t remote_xfer,
    uint32_t count,
    sync_t sync,
    volatile SIGNAL *sig_ptr
);

/** Copy transfer registers from remote microengine.
 *  Signal remote ME after transfer registers are read and local ME after transfer registers are written.
 *  This intrinsic was formerly named cls_reflect_to_sig_both().
 *
 * @param data          Data from reflect
 * @param remote_ME     Remote FPC or ME number
 * @param remote_xfer   Remote xfer register number
 * @param count         Number of 32-bit words to reflect
 * @param sync          Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr       Signal to raise upon completion
 *
 * @note __xfer_reg_number() can be used to get remote_xfer register number.
 *
 */
__intrinsic
void cmd_cls_reflect_read_sig_both(
    __xread void *data,
    uint32_t remote_ME,
    uint32_t remote_xfer,
    uint32_t count,
    sync_t sync,
    volatile SIGNAL *sig_ptr
);


/** Copy transfer registers from remote microengine.
 * Signal remote ME after transfer registers are read and local ME after transfer registers are written.
 *
 * @param data          Data from reflect
 * @param remote_island Remote island number (0 for local island)
 * @param remote_ME     Remote FPC or ME number
 * @param remote_xfer   Remote xfer register number
 * @param count         Number of 32-bit words to reflect
 * @param sync          Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr       Signal to raise upon completion
 *
 * @note __xfer_reg_number() can be used to get remote_xfer register number.
 *
 */
__intrinsic
void cmd_cls_reflect_read_sig_both_ptr40(
    __xread void *data,
    uint32_t remote_island,
    uint32_t remote_ME,
    uint32_t remote_xfer,
    uint32_t count,
    sync_t sync,
    volatile SIGNAL *sig_ptr
);


/** Copy local transfer registers to remote microengine. Signal local ME (source)
 * after transfer registers are read in indirect mode, 40 bit address mode
 *
 * @param data                      Data to reflect
 * @param remote_island             Remote island number (0 for local island)
 * @param remote_ME                 Remote FPC or ME number
 * @param remote_xfer_reg_number    Remote xfer register reg number
 * @param max_nn                    Max Number of 32-bit words to reflect
 * @param ind                       Indirect word
 * @param sync                      Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr                   Signal to raise upon completion
 *
 * @note __xfer_reg_number() can be used to get remote_xfer register number.
 *
 */
void cmd_cls_reflect_write_sig_local_ind_ptr40(
    __xwrite void *data,
    uint32_t remote_island,
    uint32_t remote_ME,
    uint32_t remote_xfer_reg_number,
    uint32_t max_nn,
    generic_ind_t ind,
    sync_t sync,
    volatile SIGNAL *sig_ptr
    );


/** Copy local transfer registers to remote microengine. Signal local ME (source)
 * after transfer registers are read in indirect mode, 32 bit address mode
 * This intrinsic was formerly named cls_reflect_from_sig_src_ind().
 *
 * @param data                      Data to reflect
 * @param remote_ME                 Remote FPC or ME number
 * @param remote_xfer_reg_number    Remote xfer register reg number
 * @param max_nn                    Max Number of 32-bit words to reflect
 * @param ind                       Indirect word
 * @param sync                      Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr                   Signal to raise upon completion
 *
 * @note __xfer_reg_number() can be used to get remote_xfer register number.
 *
 */
void cmd_cls_reflect_write_sig_local_ind(
    __xwrite void *data,
    uint32_t remote_ME,
    uint32_t remote_xfer_reg_number,
    uint32_t max_nn,
    generic_ind_t ind,
    sync_t sync,
    volatile SIGNAL *sig_ptr
    );


/** Copy local transfer registers to remote microengine.
 * Signal remote ME (destination) after transfer registers are written in indirect mode, 40 bit address mode
 *
 * @param data                      Data to reflect
 * @param remote_island             Remote island number (0 for local island)
 * @param remote_ME                 Remote FPC or ME number
 * @param remote_xfer_reg_number    Remote xfer register reg number
 * @param sig                       Triggered signal number
 * @param max_nn                    Max number of 32-bit words to reflect
 * @param ind                       Indirect word
 *
 * @note __xfer_reg_number() can be used to get remote_xfer register number.
 *
 */
__intrinsic
void cmd_cls_reflect_write_sig_remote_ind_ptr40(
    __xwrite void *data,
    uint32_t remote_island,
    uint32_t remote_ME,
    uint32_t remote_xfer_reg_number,
    int32_t sig,
    uint32_t max_nn,
    generic_ind_t ind
    );


/** Copy local transfer registers to remote microengine.
 * Signal remote ME (destination) after transfer registers are written in indirect mode, 32 bit address mode
 *  This intrinsic was formerly named cls_reflect_from_sig_dst_ind().
 *
 * @param data                      Data to reflect
 * @param remote_ME                 Remote FPC or ME number
 * @param remote_xfer_reg_number    Remote xfer register reg number
 * @param sig                       Triggered signal number
 * @param max_nn                    Max number of 32-bit words to reflect
 * @param ind                       Indirect word
 *
 * @note __xfer_reg_number() can be used to get remote_xfer register number.
 *
 */
__intrinsic
void cmd_cls_reflect_write_sig_remote_ind(
    __xwrite void *data,
    uint32_t remote_ME,
    uint32_t remote_xfer_reg_number,
    int32_t sig,
    uint32_t max_nn,
    generic_ind_t ind
    );


/** Local Scratch reflect from signal both in indirect mode.
 * Copy local transfer registers to remote microengine. Signal local ME after
 * transfer registers are read and remote ME after transfer registers are written with indirection,
 * 40 bit address mode
 *
 * @param data                      Data to reflect
 * @param remote_island             Remote island number (0 for local island)
 * @param remote_ME                 Remote FPC or ME number
 * @param remote_xfer_reg_number    Remote xfer register reg number
 * @param max_nn                    Max Number of 32-bit words to reflect
 * @param ind                       Indirect word
 * @param sync                      Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr                   Signal to raise upon completion
 *
 * @note __xfer_reg_number() can be used to get remote_xfer register number.
 *
 */
__intrinsic
void cmd_cls_reflect_write_sig_both_ind_ptr40(
    __xwrite void *data,
    uint32_t remote_island,
    uint32_t remote_ME,
    uint32_t remote_xfer_reg_number,
    uint32_t max_nn,
    generic_ind_t ind,
    sync_t sync,
    volatile SIGNAL *sig_ptr
    );


/** Local Scratch reflect from signal both in indirect mode.
 * Copy local transfer registers to remote microengine. Signal local ME after
 * transfer registers are read and remote ME after transfer registers are written with indirection,
 * 32 bit address mode
 *  This intrinsic was formerly named cls_reflect_from_sig_both_ind().
 *
 * @param data                      Data to reflect
 * @param remote_ME                 Remote FPC or ME number
 * @param remote_xfer_reg_number    Remote xfer register reg number
 * @param max_nn                    Max Number of 32-bit words to reflect
 * @param ind                       Indirect word
 * @param sync                      Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr                   Signal to raise upon completion
 *
 * @note __xfer_reg_number() can be used to get remote_xfer register number.
 *
 */
__intrinsic
void cmd_cls_reflect_write_sig_both_ind(
    __xwrite void *data,
    uint32_t remote_ME,
    uint32_t remote_xfer_reg_number,
    uint32_t max_nn,
    generic_ind_t ind,
    sync_t sync,
    volatile SIGNAL *sig_ptr
    );


/** Copy transfer registers from remote microengine. Signal remote ME (source)
 * after transfer registers are read in indirect mode, 40 bit address mode
 *
 * @param data                      Data from reflect
 * @param remote_island             Remote island number (0 for local island)
 * @param remote_ME                 Remote FPC or ME number
 * @param remote_xfer_reg_number    Remote xfer register reg number
 * @param sig                       Triggered signal number
 * @param max_nn                    Max number of 32-bit words to reflect
 * @param ind                       Indirect word
 *
 * @note __xfer_reg_number() can be used to get remote_xfer register number.
 *
 */
__intrinsic
void cmd_cls_reflect_read_sig_remote_ind_ptr40(
    __xread void *data,
    uint32_t remote_island,
    uint32_t remote_ME,
    uint32_t remote_xfer_reg_number,
    int32_t sig,
    uint32_t max_nn,
    generic_ind_t ind
    );


/** Copy transfer registers from remote microengine. Signal remote ME (source)
 * after transfer registers are read in indirect mode, 32 bit address mode
 *  This intrinsic was formerly named cls_reflect_to_sig_src_ind().
 *
 * @param data                      Data from reflect
 * @param remote_ME                 Remote FPC or ME number
 * @param remote_xfer_reg_number    Remote xfer register reg number
 * @param sig                       Triggered signal number
 * @param max_nn                    Max number of 32-bit words to reflect
 * @param ind                       Indirect word
 *
 * @note __xfer_reg_number() can be used to get remote_xfer register number.
 *
 */
__intrinsic
void cmd_cls_reflect_read_sig_remote_ind(
    __xread void *data,
    uint32_t remote_ME,
    uint32_t remote_xfer_reg_number,
    int32_t sig,
    uint32_t max_nn,
    generic_ind_t ind
    );


/** Copy transfer registers from remote microengine
 * Signal local ME (destination) after transfer registers are written in indirect mode,
 * 40 bit address mode
 *
 * @param data                      Data from reflect
 * @param remote_island             Remote island number (0 for local island)
 * @param remote_ME                 Remote FPC or ME number
 * @param remote_xfer_reg_number    Remote xfer register reg number
 * @param max_nn                    Max Number of 32-bit words to reflect
 * @param ind                       Indirect word
 * @param sync                      Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr                   Signal to raise upon completion
 * @note __xfer_reg_number() can be used to get remote_xfer register number.
 *
 */
__intrinsic
void cmd_cls_reflect_read_sig_local_ind_ptr40(
    __xread void *data,
    uint32_t remote_island,
    uint32_t remote_ME,
    uint32_t remote_xfer_reg_number,
    uint32_t max_nn,
    generic_ind_t ind,
    sync_t sync,
    volatile SIGNAL *sig_ptr
    );


/** Copy transfer registers from remote microengine
 *  Signal local ME (destination) after transfer registers are written in indirect mode,
 *  32 bit address mode
 *  This intrinsic was formerly named cls_reflect_to_sig_dst_ind().
 *
 * @param data                      Data from reflect
 * @param remote_ME                 Remote FPC or ME number
 * @param remote_xfer_reg_number    Remote xfer register reg number
 * @param max_nn                    Max Number of 32-bit words to reflect
 * @param ind                       Indirect word
 * @param sync                      Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr                   Signal to raise upon completion
 * @note __xfer_reg_number() can be used to get remote_xfer register number.
 *
 */
__intrinsic
void cmd_cls_reflect_read_sig_local_ind(
    __xread void *data,
    uint32_t remote_ME,
    uint32_t remote_xfer_reg_number,
    uint32_t max_nn,
    generic_ind_t ind,
    sync_t sync,
    volatile SIGNAL *sig_ptr
    );




/** Copy transfer registers from remote microengine.
 * Signal remote ME after transfer registers are read and local ME after transfer registers are written
 * in indirect mode, 40 bit address mode
 *
 * @param data                      Data from reflect
 * @param remote_island             Remote island number (0 for local island)
 * @param remote_ME                 Remote FPC or ME number
 * @param remote_xfer_reg_number    Remote xfer register reg number
 * @param max_nn                    Max Number of 32-bit words to reflect
 * @param ind                       Indirect word
 * @param sync                      Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr                   Signal to raise upon completion
 *
 * @note __xfer_reg_number() can be used to get remote_xfer register number.
 *
 */
__intrinsic
void cmd_cls_reflect_read_sig_both_ind_ptr40(
    __xread void *data,
    uint32_t remote_island,
    uint32_t remote_ME,
    uint32_t remote_xfer_reg_number,
    uint32_t max_nn,
    generic_ind_t ind,
    sync_t sync,
    volatile SIGNAL *sig_ptr
    );


/** Copy transfer registers from remote microengine.
 * Signal remote ME after transfer registers are read and local ME after transfer registers are written
 * in indirect mode, 32 bit address mode
 *  This intrinsic was formerly named cls_reflect_to_sig_both_ind().
 *
 * @param data                      Data from reflect
 * @param remote_ME                 Remote FPC or ME number
 * @param remote_xfer_reg_number    Remote xfer register reg number
 * @param max_nn                    Max Number of 32-bit words to reflect
 * @param ind                       Indirect word
 * @param sync                      Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr                   Signal to raise upon completion
 *
 * @note __xfer_reg_number() can be used to get remote_xfer register number.
 *
 */
__intrinsic
void cmd_cls_reflect_read_sig_both_ind(
    __xread void *data,
    uint32_t remote_ME,
    uint32_t remote_xfer_reg_number,
    uint32_t max_nn,
    generic_ind_t ind,
    sync_t sync,
    volatile SIGNAL *sig_ptr
    );

/** @}
 * @}
 */

#endif /* __NFP_CLS_REFLECT_H__ */
