/*
 * Copyright (C) 2009-2013 Netronome Systems, Inc.  All rights reserved.
 */

#ifndef __CAP_UC__
#define __CAP_UC__

#ifndef NFP_LIB_ANY_NFAS_VERSION
    #if (!defined(__NFP_TOOL_NFAS) ||                       \
        (__NFP_TOOL_NFAS < NFP_SW_VERSION(5, 0, 0, 0)) ||   \
        (__NFP_TOOL_NFAS > NFP_SW_VERSION(6, 1, 0, 255)))
        #error "This standard library is not supported for the version of the SDK currently in use."
    #endif
#endif

#if (IS_NFPTYPE(__NFP6000))
        #error "This standard library is not supported for the NFP6000."
#endif

#include <stdmac.uc>
#include <nfp3200/nfp3200.h>

/** @file cap.uc CAP Operation Macros
 * @addtogroup cap CAP Operation
 * @{
 *
 * @name CAP Operation Macros
 * @{
 *
 */


/// @cond INTERNAL_MACROS
/** Perform CAP operation
 *
 * @param action CAP operation to perform
 * @param xfer Transfer register
 * @param src1 First source operand
 * @param src2 Second source operand
 * @param ref_cnt Length parameter
 */
#macro _cap_calc(action, xfer, src1, src2, ref_cnt)
.begin

    .sig cap_sig
    cap[action, xfer, src1, src2, ref_cnt], sig_done[cap_sig]
wait_cap_sig#:
    br_!signal[cap_sig, wait_cap_sig#]
    nop
    nop
    nop

.end
#endm
/// @endcond


/** CAP read (calculated addressing)
 *
 * @param addr CAP address to read from
 * @param xdata Read transfer register for result
 */
#macro cap_read(addr, xdata)
.begin

    .reg cap_addr
    move(cap_addr, (addr))
    _cap_calc(read, xdata, cap_addr, 0, 1)

.end
#endm


/** CAP write (calculated addressing)
 *
 * @param addr CAP address to write to
 * @param data Data to write. This can be a GPR, constant or read transfer register.
 */
#macro cap_write(addr, data)
.begin

    .reg $cap_xdata
    .reg cap_addr
    move(cap_addr, (addr))
    move($cap_xdata, data)
    _cap_calc(write, $cap_xdata, cap_addr, 0, 1)

.end
#endm


/// @cond INTERNAL_MACROS
/** Perform XPB CAP operation
 *
 * @param op CAP operation to perform
 * @param cluster Cluster number. Must be a constant
 * @param device Device number. Must be a constant
 * @param offset Byte offset
 * @param xdata Transfer register
 * @param req_sig Signal to use
 * @param sig_action Action to perform on the signal @n@n
 *          Possible values are: @n
 *          @arg @c SIG_NONE: Only sig_done is used
 *          @arg    Anything else: ctx_arb is used
 */
#macro _xpb_op(op, cluster, device, offset, xdata, req_sig, sig_action)
.begin

    .reg addr
    #if is_ct_const(offset)
    immed32(addr, NFP_XPB_CAP_ADDR(cluster, device, offset))
    cap[op, xdata, addr, 0, 1], sig_done[req_sig]
    #else
    immed32(addr, NFP_XPB_CAP_ADDR(cluster, device, 0))
    cap[op, xdata, addr, offset, 1], sig_done[req_sig]
    #endif
    #if (!streq('sig_action', 'SIG_NONE'))
        ctx_arb[req_sig]
    #endif

.end
#endm
/// @endcond


/** XPB write, waits for completion.
 *
 * @param cluster Cluster number. Must be a constant
 * @param device Device number. Must be a constant
 * @param offset Byte offset
 * @param xdata Write transfer register containing the data to write
 */
#macro xpb_write(cluster, device, offset, xdata)
.begin

    .sig s
    _xpb_op(write, cluster, device, offset, xdata, s, s)

.end
#endm


/** XPB write, custom signal and signal action.
 *
 * @param cluster Cluster number. Must be a constant
 * @param device Device number. Must be a constant
 * @param offset Byte offset
 * @param xdata Write transfer register containing the data to write
 * @param req_sig Signal to use
 * @param sig_action Action to perform on the signal @n@n
 *          Possible values are: @n
 *          @arg @c SIG_NONE: Does not wait for completion (sig_done)
 *          @arg    Anything else: Waits for completion (ctx_arb)
 */
#macro xpb_write(cluster, device, offset, xdata, req_sig, sig_action)
.begin

    _xpb_op(write, cluster, device, offset, xdata, req_sig, sig_action)

.end
#endm


/** XPB write 32-bit value, waits for completion.
 *
 *
 * @param cluster Cluster number. Must be a constant
 * @param device Device number. Must be a constant
 * @param offset Byte offset
 * @param val Value to write. Must be a constant
 */
#macro xpb_write_val(cluster, device, offset, val)
.begin

    .reg $x
    .sig s
    immed32($x, val)
    _xpb_op(write, cluster, device, offset, $x, s, s)

.end
#endm


/**
 * XPB write 32-bit value
 *
 * @param cluster Cluster number. Must be a constant
 * @param device Device number. Must be a constant
 * @param offset Byte offset
 * @param val Value to write. Must be a constant
 * @param req_sig Signal to use
 * @param sig_action Action to perform on the signal @n@n
 *          Possible values are: @n
 *          @arg @c SIG_NONE: Does not wait for completion (sig_done)
 *          @arg    Anything else: Waits for completion (ctx_arb)
 */
#macro xpb_write_val(cluster, device, offset, val, req_sig, sig_action)
.begin

    .reg $x
    immed32($x, val)
    _xpb_op(write, cluster, device, offset, $x, req_sig, sig_action)

.end
#endm


/** XPB read, waits for completion.
 *
 * @param cluster Cluster number. Must be a constant
 * @param device Device number. Must be a constant
 * @param offset Byte offset
 * @param xdata Read transfer register for result
 */
#macro xpb_read(cluster, device, offset, xdata)
.begin

    .sig s
    _xpb_op(read, cluster, device, offset, xdata, s, s)

.end
#endm


/** XPB read, custom signal and signal action.
 *
 * @param cluster Cluster number. Must be a constant
 * @param device Device number. Must be a constant
 * @param offset Byte offset
 * @param xdata Read transfer register for result
 * @param req_sig Signal to use
 * @param sig_action Action to perform on the signal @n@n
 *          Possible values are: @n
 *          @arg @c SIG_NONE: Does not wait for completion (sig_done)
 *          @arg    Anything else: Waits for completion (ctx_arb)
 */
#macro xpb_read(cluster, device, offset, xdata, req_sig, sig_action)
.begin

    _xpb_op(read, cluster, device, offset, xdata, req_sig, sig_action)

.end
#endm


/** XPB Modify, performs a read, modify, write to update bits and waits for completion.
 *
 * @b Operation:
 * @code
 *  reg = (reg AND ~(mask<<LSB)) OR (val<<LSB)
 * @endcode
 *
 * @param cluster Cluster number. Must be a constant
 * @param device Device number. Must be a constant
 * @param offset Byte offset
 * @param LSB Bit position to shift @p mask and @p val to. Must be a constant
 * @param mask Unshifted bit mask to apply to register value
 * @param val Unshifted value to set to register
 */
#macro xpb_bits_modify(cluster, device, offset, LSB, mask, val)
.begin

    #if is_ct_const(mask)
        .reg _mask_
        immed32(_mask_, mask)
    #else
        #define _mask_ mask
    #endif

    #if is_ct_const(val)
        .reg _val_
        immed32(_val_, val)
    #else
        #define _val_ val
    #endif

    .reg $xio, r
    xpb_read(cluster, device, offset, $xio)
    alu_shf[r, $xio, AND~, _mask_, <</**/LSB]
    alu_shf[$xio, r, OR, _val_, <</**/LSB]
    xpb_write(cluster, device, offset, $xio)

.end
#endm


/** @}
 * @}
 */

#endif /* __CAP_UC__ */
