/*
 * Copyright (C) 2009-2013 Netronome Systems, Inc.  All rights reserved.
 */

#ifndef __MSF_UC__
#define __MSF_UC__

#ifndef NFP_LIB_ANY_NFAS_VERSION
    #if (!defined(__NFP_TOOL_NFAS) ||                       \
        (__NFP_TOOL_NFAS < NFP_SW_VERSION(5, 0, 0, 0)) ||   \
        (__NFP_TOOL_NFAS > NFP_SW_VERSION(6, 1, 0, 255)))
        #error "This standard library is not supported for the version of the SDK currently in use."
    #endif
#endif

#if (IS_NFPTYPE(__NFP6000) || IS_NFPTYPE(__NFP3800))
        #error "This standard library is not supported for the NFP6000."
#endif

#include <cap.uc>
#include <cycle.uc>
#include <nfp3200/nfp3200.h>

/** @file msf.uc Media Switch Fabric Macros
 * @addtogroup msf Media Switch Fabric
 * @{
 *
 * @name Media Switch Fabric Macros
 * @{
 *
 */


/**
 * Write one 32-bit word to a MSF0/1 CSR
 *
 * @param MSF_CLUSTER  msf0/msf1
 * @param addr         Address of CSR
 * @param data_31_16   Bits [31:16] of 32-bit value
 * @param data_15_0    Bits [15:0] of 32-bit value
 */
#macro msf_csr_write(MSF_CLUSTER, addr, data_31_16, data_15_0)
.begin

    .reg tmp, msf_addr offset
    .reg $xfer_msf_csr_write
    .sig sig_msf_csr_write
    .set tmp msf_addr

    immed_w0[tmp, data_15_0]
    immed_w1[tmp, data_31_16]
    alu[$xfer_msf_csr_write, --, B, tmp]

    immed[msf_addr, addr]
    immed[offset, 0]

    msf_csr_write(MSF_CLUSTER, $xfer_msf_csr_write, msf_addr, offset, 1, sig_msf_csr_write, SIG_WAIT)

.end
#endm // msf_csr_write


/// @cond INTERNAL_MACROS
#macro _msf_csr_op(MSF_CLUSTER, action, xdata, addr, offset, ref_cnt, sig, sig_action)
    #if (streq('sig_action', 'SIG_WAIT'))
        MSF_CLUSTER[action, xdata, addr, offset, ref_cnt], ctx_swap[sig]
    #else
        MSF_CLUSTER[action, xdata, addr, offset, ref_cnt], sig_done[sig]
    #endif
#endm
/// @endcond


/**
 * Write 32-bit words to MSF0/1 CSR
 *
 * @param MSF_CLUSTER  msf0/msf1
 * @param xdata        Transfer register
 * @param addr         Address of MSF CSR
 * @param offset       Offset (added to addr)
 * @param ref_cnt      Number of 32-bit words
 * @param sig          Completion signal
 * @param sig_action   SIG_NONE/SIG_WAIT
 */
#macro msf_csr_write(MSF_CLUSTER, xdata, addr, offset, ref_cnt, sig, sig_action)
    _msf_csr_op(MSF_CLUSTER, write, xdata, addr, offset, ref_cnt, sig, sig_action)
#endm


/**
 * Read 32-bit words from MSF0/1 CSR
 *
 * @param MSF_CLUSTER  msf0/msf1
 * @param xdata        Transfer register
 * @param addr         Address of MSF CSR
 * @param offset       Offset (added to addr)
 * @param ref_cnt      Number of 32-bit words
 * @param sig          Completion signal
 * @param sig_action   SIG_NONE/SIG_WAIT
 */
#macro msf_csr_read(MSF_CLUSTER, xdata, addr, offset, ref_cnt, sig, sig_action)
    _msf_csr_op(MSF_CLUSTER, read, xdata, addr, offset, ref_cnt, sig, sig_action)
#endm


/**
 * Write MSF0/1 global CSR on XPB bus
 *
 * @param MSF_CLUSTER  msf0/msf1
 * @param offset       Byte offset of CSR
 * @param val          32-bit value
 */
#macro msf_xpb_write_global_csr(MSF_CLUSTER, offset, val)
    #if (streq('MSF_CLUSTER', 'msf0') || streq('MSF_CLUSTER', 'MSF0'))
        xpb_write_val(NFP_XPB_MSF0_GLOBAL, offset, val)
    #else
        xpb_write_val(NFP_XPB_MSF1_GLOBAL, offset, val)
    #endif
#endm


/**
 * Read MSF0/1 global CSR on XPB bus
 *
 * @param MSF_CLUSTER  msf0/msf1
 * @param offset       Byte offset of CSR
 * @param xdata        Transfer register
 */
#macro msf_xpb_read_global_csr(MSF_CLUSTER, offset, xdata)
    #if (streq('MSF_CLUSTER', 'msf0') || streq('MSF_CLUSTER', 'MSF0'))
        xpb_read(NFP_XPB_MSF0_GLOBAL, offset, xdata)
    #else
        xpb_read(NFP_XPB_MSF1_GLOBAL, offset, xdata)
    #endif
#endm


/**
 * Write MSF0/1 config CSR on XPB bus
 *
 * @param MSF_CLUSTER  msf0/msf1
 * @param offset       Byte offset of CSR
 * @param val          32-bit value
 */
#macro msf_xpb_write_config_csr(MSF_CLUSTER, offset, val)
    #if (streq('MSF_CLUSTER', 'msf0') || streq('MSF_CLUSTER', 'MSF0'))
        xpb_write_val(NFP_XPB_MSF0_CONFIG, offset, val)
    #else
        xpb_write_val(NFP_XPB_MSF1_CONFIG, offset, val)
    #endif
#endm


/**
 * Read MSF0/1 config CSR on XPB bus
 *
 * @param MSF_CLUSTER  msf0/msf1
 * @param offset       Byte offset of CSR
 * @param xdata        Transfer register
 */
#macro msf_xpb_read_config_csr(MSF_CLUSTER, offset, xdata)
    #if (streq('MSF_CLUSTER', 'msf0') || streq('MSF_CLUSTER', 'MSF0'))
        xpb_read(NFP_XPB_MSF0_CONFIG, offset, xdata)
    #else
        xpb_read(NFP_XPB_MSF1_CONFIG, offset, xdata)
    #endif
#endm


/**
 * Wait for MSF0/1 event on global CSR (XPB bus).  Wait until
 * (CSR value AND mask) is equal to (val AND mask)
 *
 * @param MSF_CLUSTER  msf0/msf1
 * @param offset       Byte offset of CSR
 * @param val          Value to compare (immed or GPR)
 * @param mask         Mask applied to CSR before compare
 * @param delay        Cycle delay between each read. If delay
 *                     is not required, pass --
 */
#macro msf_xpb_global_csr_wait(MSF_CLUSTER, offset, val, mask, delay)
.begin

    .reg masked_val, masked_read
    .reg $x
    immed32(masked_val, val)
    #if is_ct_const(mask)
        .reg mask_
        immed32(mask_, mask)
    #else
        #define mask_ mask
    #endif
    alu[masked_val, masked_val, AND, mask_]
poll_loop#:
    msf_xpb_read_global_csr(MSF_CLUSTER, offset, $x)
    alu[masked_read, $x, AND, mask_]
    alu[--, masked_read, -, masked_val]
    beq[poll_done#]
    #if (!streq('delay', '--'))
    cycle_delay(delay, LOOP_TIGHT)
    #endif
    br[poll_loop#]
poll_done#:

.end
#endm


/** @}
 * @}
 */

#endif /* __MSF_UC__ */
