/*
 * Copyright (C) 2009-2011 Netronome Systems, Inc.  All rights reserved.
 */

#ifndef __SIG_MACROS_UC__
#define __SIG_MACROS_UC__

#ifndef NFP_LIB_ANY_NFAS_VERSION
    #if (!defined(__NFP_TOOL_NFAS) ||                       \
        (__NFP_TOOL_NFAS < NFP_SW_VERSION(5, 0, 0, 0)) ||   \
        (__NFP_TOOL_NFAS > NFP_SW_VERSION(6, 1, 0, 255)))
        #error "This standard library is not supported for the version of the SDK currently in use."
    #endif
#endif

/** @file sig_macros.uc Signalling Macros
 * @addtogroup sig Signalling
 * @{
 *
 * @name Signalling Macros
 * @{
 *
 *  This section provides a set of simple macros used for signalling threads and micro engines.
 *  Signalling threads within the same microengine, signalling neighbouring (next and previous) Microengines and
 *  signalling any microengine are covered.
 */


/** Signal next context (thread) in the same microengine.
 *
 * This macro can be used by any thread. Thread 7 will signal thread 0.
 * No error checking is done on signal number.
 *
 * @param in_sig    Signal number (0-0xF), &sig or register
 *
 */
#macro signal_next_ctx(in_sig)
.begin

    #if (is_ct_const(in_sig))
        local_csr_wr[SAME_ME_SIGNAL, ((1<<7) | (in_sig<<3))]
    #elif (strstr('in_sig', "&"))
        local_csr_wr[SAME_ME_SIGNAL, ((1<<7) | (in_sig<<3))]
    #else
        local_csr_wr[SAME_ME_SIGNAL, in_sig] // Temporary support
    #endif

.end
#endm


/// @cond INTERNAL_MACROS
/**
 *  Initialize next-thread/context register
 *
 *  @param in_sig  Signal (not number)
 */
#macro signal_init_next_ctx_sig_reg(sig_reg, in_sig)
.begin

    immed[sig_reg, 0x80]                        // next context bit [7]
    alu[sig_reg, sig_reg, OR, &in_sig, <<3]     // [6:3] signal number

.end
#endm
/// @endcond


/// @cond INTERNAL_MACROS
/**
 *  Wait for signal from previous thread/context
 */
#macro signal_wait_prev_ctx(wait_sig)
.begin

    .set_sig wait_sig
    ctx_arb[wait_sig]

.end
#endm
/// @endcond


/// @cond INTERNAL_MACROS
/**
 *  Clear signal mask
 */
#macro signal_clear_mask(sig_mask)
    immed[sig_mask, 0]
#endm
/// @endcond


/// @cond INTERNAL_MACROS
/**
 *  Clear plus set signal mask in one step
 */
#macro signal_initialize_mask(sig_mask, sig_num)
    alu[sig_mask, --, B, (mask(sig_num)), <<&sig_num]
#endm
/// @endcond


/// @cond INTERNAL_MACROS
/**
 *  Add signal to signal mask
 */
#macro signal_add_to_mask(sig_mask, sig_num)
    alu[sig_mask, sig_mask, OR, (mask(sig_num)), <<&sig_num]
#endm
/// @endcond


/// @cond INTERNAL_MACROS
/**
 *  Remove signal from signal mask
 */
#macro signal_remove_from_mask(sig_mask, sig_num)
    alu[sig_mask, sig_mask, AND~, (mask(sig_num)), <<&sig_num]
#endm
/// @endcond


/// @cond INTERNAL_MACROS
/**
 *  Set wakeup signal mask and swap out context
 */
#macro signal_wait_mask(sig_mask)
    local_csr_wr[ACTIVE_CTX_WAKEUP_EVENTS, sig_mask]
    ctx_arb[--]
#endm
/// @endcond


/// @cond INTERNAL_MACROS
/**
 *  Wait for signal to be asserted, optionally swapping out
 *
 *  @param WAIT_TYPE  THD_SWAP/THD_SPIN
 */
#macro signal_wait(wait_sig, WAIT_TYPE)
.begin

    #if( streq('WAIT_TYPE', 'THD_SWAP') )
        ctx_arb[wait_sig]
    #elif( streq('WAIT_TYPE', 'THD_SPIN') )
    wait_signal_spin_loop#:
        br_!signal[wait_sig, wait_signal_spin_loop#]
    #endif
    .io_completed wait_sig

.end
#endm
/// @endcond


/// @cond INTERNAL_MACROS
/**
 *  Wait for a common inter-thread/context signal.  Typically
 *  used during system initialization.
 */
#macro signal_wait_for_common_inter_ctx_signal(wait_sig_addr)
.begin

    .sig volatile common_init_complete_sig_num
    .addr common_init_complete_sig_num  wait_sig_addr
common_init_signal#:
    br_!signal[common_init_complete_sig_num, common_init_signal#]

.end
#endm
/// @endcond




/** This macro signals any context within the same microengine.
 *
 * @param in_sig_val    Ehe exact argument that will be used to write
 *                      to SAME_ME_SIGNAL CSR.
 *
 * @instruction_cnt 1
 *
 */
#macro signal_ctx(in_sig_val)
    local_csr_wr[SAME_ME_SIGNAL, in_sig_val]
#endm

/** Signal the first context (thread 0) in the same microengine.
 *
 * This macro can be used by any thread. No error checking is done on signal number.
 *
 * @param in_sig    Signal number (0-0xF)
 *
 * @instruction_cnt 3
 */
#macro signal_first_ctx(in_sig)
.begin

    .reg _sfc_temp, _zero

    immed[_zero, 0]                             // [7] = 0, ! NEXT_CONTEXT
    alu_shf[_sfc_temp, _zero, OR, in_sig, <<3]  // [6:3] = SIGNAL_NUMBER
                                                // [2:0] = 0x0, Context to signal
    local_csr_wr[SAME_ME_SIGNAL, _sfc_temp]     // Signal thread 0

.end
#endm


/** Signal context 0 (thread 0) in the next microengine.
 *
 * This macro can be used by any but the last microengine. (Last microengine has no next microengine). Behavior is
 * undefined if used by the last microengine. No error checking is done on signal number.
 *
 * @param in_sig          Signal number (0-0xF)
 *
 * @instruction_cnt 3
 */
#macro signal_next_me(in_sig)
.begin

    .reg _snm_temp, _zero

    immed[_zero, 0]                                 // [7] = 0, ! THIS_CONTEXT
    alu_shf[_snm_temp, _zero, OR, in_sig, <<3]      // [6:3] = SIGNAL_NUMBER
                                                    // [2:0] = 0x0, Context to signal
    local_csr_wr[NEXT_NEIGHBOR_SIGNAL, _snm_temp]   // Signal the thread 0 of next ME

.end
#endm


/** Signal context 0 (thread 0) in the previous microengine.
 *
 * This macro can be used by any but the first microengine. (The first microengine has no previous microengine).
 * Behavior is undefined if used by the first microengine. No error checking is done on
 * signal number.
 *
 * @param in_sig   Signal number (0-0xF)
 *
 * @instruction_cnt 3
 */
#macro signal_prev_me(in_sig)
.begin

    .reg _spm_temp, _zero

    immed[_zero, 0]                                 // [7] = 0, ! THIS_CONTEXT
    alu_shf[_spm_temp, _zero, OR, in_sig, <<3]      // [6:3] = SIGNAL_NUMBER
                                                    // [2:0] = 0x0, Context to signal
    local_csr_wr[PREV_NEIGHBOR_SIGNAL, _spm_temp]   // Signal the thread 0 of previous ME

.end
#endm


/** Signal context 0 (thread 0) of any microengine.
 *
 * Note the way the microengines are numbered. The microengines are grouped into two clusters and each cluster has
 * 8 microengines. No error checking is done on signal number. Between the parameters
 * me and sig, they should follow the op_A and op_B rules.
 *
 * @param in_cluster    Cluster number. (0-1). Can be constant or register.
 * @param in_me         Microengine number (0-7)
 * @param in_sig        Signal number (0-0xF)
 *
 * @instruction_cnt 4
 */
#macro signal_me_cluster(in_cluster, in_me, in_sig)
#if (IS_NFPTYPE(__NFP6000) || IS_NFPTYPE(__NFP3800))
    #error "signal_me_cluster: This macro is not supported for NFP6000."
#endif
.begin

    .reg _signal_me_temp, _sig

    alu[_sig, --, b, in_sig]                        // move to local register. allows in_sig to be immediate value.
    alu[_signal_me_temp, _sig, OR, in_me, <<7]      // [11-7] = ME_NUMBER
    alu[--, _signal_me_temp, OR, in_cluster, <<11]  // [6:4]  = 0x0, THREAD_NUMBER
                                                    // [3:0]  = SIGNAL
    cap[fast_wr, ALU, INTERTHREAD_SIG]              // Signal the ME.

.end
#endm


/** Signal context 0 (thread 0) of any microengine.
 *
 * Note the way the microengines are numbered. The microengines are grouped into two clusters. Each cluster has
 * 8 microengines. No error checking is done on signal number. Between the parameters
 * me and sig, they should follow the op_A and op_B rules.
 *
 * @param in_me     Microengine number.
 *                  0-0x1f. Cluster number should already be encoded in
 *                  this microengine number.
 * @param in_sig    Signal number (0-0xF)
 *
 * @instruction_cnt 3
 */
#macro signal_me(in_me, in_sig)
#if (IS_NFPTYPE(__NFP6000) || IS_NFPTYPE(__NFP3800))
    #error "signal_me: This macro is not supported for NFP6000."
#endif
.begin

    .reg _sig

    alu[_sig, --, b, in_sig]                        // move to local register. allows in_sig to be immediate value.
    alu[--, _sig, OR, in_me, <<7]                   // [11-7] = ME_NUMBER
                                                    // [6:4]  = 0x0, THREAD_NUMBER
                                                    // [3:0]  = SIGNAL
    cap[fast_wr, ALU, INTERTHREAD_SIG]              // Signal the ME.

.end
#endm

/** Signal context 1 (thread 1) of any microengine.
 *
 * Note the way the microengines are numbered. The microengines are grouped into two clusters. Each cluster has
 * 8 microengines. No error checking is done on signal number. Between the parameters
 * me and sig, they should follow the op_A and op_B rules.
 *
 * @param in_me    Microengine number.
 *                 0-0x1f. Cluster number should already be encoded in
 *                 this microengine number.
 * @param in_sig   Signal number (0-0xF)
 *
 * @instruction_cnt 4
 */
#macro signal_me_ctx1(in_me, in_sig)
#if (IS_NFPTYPE(__NFP6000) || IS_NFPTYPE(__NFP3800))
    #error "signal_me_ctx1: This macro is not supported for NFP6000."
#endif
.begin

    .reg _sig
    .reg temp
    alu[temp, --, b, 1, <<4]
    alu[_sig, in_sig, OR, temp]                     // move to local register. allows in_sig to be immediate value.
    alu[--, _sig, OR, in_me, <<7]                   // [11-7] = ME_NUMBER
                                                    // [6:4]  = 0x0, THREAD_NUMBER
                                                    // [3:0]  = SIGNAL
    cap[fast_wr, ALU, INTERTHREAD_SIG]              // Signal the ME.

.end
#endm


/** @}
 * @}
 */

#endif /* __SIG_MACROS_UC__ */
