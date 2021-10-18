/*
 * Copyright (C) 2009-2014 Netronome Systems, Inc.  All rights reserved.
 */

#ifndef NFP_LIB_ANY_NFAS_VERSION
    #if (!defined(__NFP_TOOL_NFAS) ||                       \
        (__NFP_TOOL_NFAS < NFP_SW_VERSION(5, 0, 0, 0)) ||   \
        (__NFP_TOOL_NFAS > NFP_SW_VERSION(6, 1, 0, 255)))
        #error "This standard library is not supported for the version of the SDK currently in use."
    #endif
#endif

#ifndef __EVENT_AP_UC__
#define __EVENT_AP_UC__

#include <stdmac.uc>

/** @file cls_ap.uc Cluster Scratch Event Filters & Autopush config Macros
 * @addtogroup cls_ap Event filters and autopush API
 * @{
 *
 * @name CLS Filters and Autopush Macros
 * @{
 * Cluster Scratch Event Filters & Autopush config Macros
 */

// API Quick Reference:
// evntm_cls_event_filter_config(filter, mask, match, filter_type)
// evntm_cls_autopush_monitor_config(filter, me_num, ctxt_num, auto_push_xfer_reg, signal)
// evntm_cls_autopush_monitor_config(filter, auto_push_xfer_reg, signal)
// evntm_cls_autopush_monitor_engage(filter, in_xfer, signal, sig_action)
// evntm_cls_autopush_user_event(event, in_xfer, signal, sig_action)

/// @cond INTERNAL_MACROS
/**
 *  Wait for signal to be asserted, optionally swapping out
 *
 *  @param WAIT_TYPE  THD_SWAP/THD_SPIN/NONE
 */
#macro _evntm_signal_wait(wait_sig, WAIT_TYPE)
.begin

    #if( streq('WAIT_TYPE', 'THD_SWAP') )
        ctx_arb[wait_sig]
    #elif( streq('WAIT_TYPE', 'THD_SPIN') )
        wait_signal_spin_loop#:
            br_!signal[wait_sig, wait_signal_spin_loop#]
    #endif

.end
#endm /* _evntm_signal_wait */
/// @endcond

/** This macro configures an event filter to a provided mask/match and filter type
 * @param filter
 * @param mask
 * @param match
 * @param filter_type
 */
#macro evntm_cls_event_filter_config(filter, mask, match, filter_type)
.begin
    .reg $xfer[4]
    .xfer_order $xfer
    .reg addr
    .reg _mask
    .reg _match
    .sig signal
    move(addr, 0x20000)
    alu[addr, addr, OR, filter, <<6]
    #if (isnum(mask))
        move(_mask, mask)
    #else
        alu[_mask, --, b, mask]
    #endif
    #if (isnum(match))
        move(_match, match)
    #else
        alu[_match, --, b, match]
    #endif
    alu[_mask, _mask, OR, filter_type, <<24]
    alu[$xfer[0], --, b, _mask]
    alu[$xfer[1], --, b, 0]
    cls[write, $xfer[0], addr, 0x10, 1], sig_done[signal]
    _evntm_signal_wait(signal, THD_SPIN)
    alu[$xfer[2], --, b, _match]
    alu[$xfer[3], --, b, 0]
    alu[$xfer[2], --, b, _match]
    alu[$xfer[3], --, b, 0]
    cls[write, $xfer[2], addr, 0x18, 1], sig_done[signal]
    _evntm_signal_wait(signal, THD_SPIN)
.end
#endm /* evntm_cls_event_filter_config */

/** This macro configures an autopush monitor for a filter with ME number, transfer register and signal
 *
 * @param filter        Filter number to be monitored
 * @param me_num        Microengine number to signal and push data to when a FilterStatusMonitor fires. Range[4-15]
 * @param ctxt_num      Context to be signaled on event. Range[0-7]
 * @auto_push_xfer_reg  ME Transfer register to push event status
 * @param signal        Signal reference to use on auto push.
 */
#macro evntm_cls_autopush_monitor_config(filter, me_num, ctxt_num, auto_push_xfer_reg, signal)
.begin
    .reg addr
    .reg temp
    .reg $xfer
    .sig local_signal
    move(addr, 0x30200)
    alu[addr, addr, OR, filter, <<3]
    /*xfer = ((me_num) << 24 ) | (signal<<16) | (xfer_reg<<2) | (ctxt_num<<7) | (ctxt_num<<19) */
    immed[temp, 0]
    alu[temp, --, b, me_num, <<24]
    alu[temp, temp, OR, signal, <<16]
    alu[temp, temp, OR, auto_push_xfer_reg, <<2]
    alu[temp, temp, OR, ctxt_num, <<7]
    alu[temp, temp, OR, ctxt_num, <<20]
    alu[$xfer, --, b, temp]
    cls[write, $xfer, addr, 0, 1], sig_done[local_signal]
    _evntm_signal_wait(local_signal, THD_SPIN)
.end
#endm /* evntm_cls_autopush_monitor_config */

/** This macro configures an autopush monitor for a filter.transfer register and signal
 *  ME number and Context number are taken from ACTIVE_CTX_STS i.e. which ever ME & Context is calling this macro.
 *
 * @param filter        Filter number to be monitored
 * @auto_push_xfer_reg  ME Transfer register to push event status
 * @param signal        Signal reference to use on auto push
 */
#macro evntm_cls_autopush_monitor_config(filter, auto_push_xfer_reg, signal)
.begin
    .reg addr
    .reg temp
    .reg ctxt_num me_num
    .reg $xfer
    .sig local_signal
    move(addr, 0x30200)
    alu[addr, addr, OR, filter, <<3]
    /*xfer = ((me_num+4) << 24 ) | (signal<<16) | (xfer_reg<<2) | (ctxt_num<<7) | (ctxt_num<<19) */
    rd_act_ctx_sts#:
    local_csr_rd[ACTIVE_CTX_STS]
    immed[temp, 0]
    br_bclr[temp, 31, rd_act_ctx_sts#]
    alu[ctxt_num, temp, AND, 0x7]
    alu[me_num, 0xf, AND, temp, >>3]

    alu[temp, --, b, me_num, <<24]
    alu[temp, temp, OR, signal, <<16]
    alu[temp, temp, OR, auto_push_xfer_reg, <<2]
    alu[temp, temp, OR, ctxt_num, <<7] //OR ctx to get right xfer reg in data_ref
    alu[temp, temp, OR, ctxt_num, <<20] //OR with SigRef[3;4]
    alu[$xfer, --, b, temp]
    cls[write, $xfer, addr, 0, 1], sig_done[local_signal]
    _evntm_signal_wait(local_signal, THD_SPIN)
.end
#endm /* evntm_cls_autopush_monitor_config */

/** This macro should be called to start monitoring an event filter, after evntm_cls_autopush_monitor_config has been called once.
 *  This macro uses 'one shot acknowledge'
 *
 * @param filter        Filter number to be monitored
 * @param in_xfer       Xfer register to be used for I/O
 * @param signal        I/O Signal to use
 * @param sig_action    THD_SWAP/THD_SPIN/NONE
 */
#macro evntm_cls_autopush_monitor_engage(filter, in_xfer, signal, sig_action)
.begin
    .reg addr
    .reg _filter
    move(addr, 0x30000)
    alu[addr, addr, OR, filter, <<3]
    alu[_filter, --, b, filter]
    alu[in_xfer, _filter, OR, 3, <<8]
    cls[write, in_xfer, addr, 0, 1], sig_done[signal]
    _evntm_signal_wait(signal, sig_action)
.end
#endm /* evntm_cls_autopush_monitor_engage */

/** This macro pushes an event into UserEvent in the CLS event manager
 *
 * @param event         Event to push
 * @param signal        I/O Signal to use
 * @param sig_action    THD_SWAP/THD_SPIN/NONE
 */
#macro evntm_cls_autopush_user_event(event, in_xfer, signal, sig_action)
.begin
    .reg addr
    move(addr, 0x30400)
    alu[in_xfer, --, b, event]
    cls[write, in_xfer, addr, 0, 1], sig_done[signal]
    _evntm_signal_wait(signal, sig_action)
.end
#endm /* evntm_cls_autopush_user_event */

/** @}
 * @}
 */


#endif /* __EVENT_AP_UC__ */
