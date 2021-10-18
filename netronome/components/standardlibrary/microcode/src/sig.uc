/*
 * Copyright (C) 2008-2011 Netronome Systems, Inc.  All rights reserved.
 */

#ifndef __SIG_UC__
#define __SIG_UC__

#ifndef NFP_LIB_ANY_NFAS_VERSION
    #if (!defined(__NFP_TOOL_NFAS) ||                       \
        (__NFP_TOOL_NFAS < NFP_SW_VERSION(5, 0, 0, 0)) ||   \
        (__NFP_TOOL_NFAS > NFP_SW_VERSION(6, 1, 0, 255)))
        #error "This standard library is not supported for the version of the SDK currently in use."
    #endif
#endif

#include <constants.uc>
#include <stdmac.uc>

/** @file sig.uc Interthread Signalling Macros
 * @addtogroup it_sig Interthread Signalling
 * @{
 *
 * @name Interthread Signalling Macros
 * @{
 *
 */

// API Quick Reference:
//
// sig_block(SIG_OP, in_sig_list)
// sig_post(in_thread_id, in_sig_id)
// sig_wait(SIG_OP, in_sig_list)
// sig_ifctx_block(in_ctx, SIG_OP, in_sig_list)
// sig_ifctx_post(in_ctx, in_thread_id, in_sig_id)
// sig_ifctx_wait(in_ctx, SIG_OP, in_sig_list)


/** Block this microengine's further execution until signal is received.
 *
 * @b Example:
 * @code
 * sig_block(ALL, signals(SIG0, SIG1, SIG2))
 * sig_block(ANY, signals(SIG3, SIG4))
 * sig_block(ALL, SIG5)
 * @endcode
 *
 * @param SIG_OP        Operation performed on signal list. @n@n
 *                      Possible values are: @n
 *                      @arg @c ALL: Wait until all signals in @p in_sig_list have been received.
 *                      @arg @c ANY: Wait until any signal(s) in @p in_sig_list have been received.
 * @param in_sig_list   List of signals causing thread to wake up. This list is
 *                      either a single signal or a list of signals in the form
 *                      of signals(sig1, sig2, ..., sign). @c SIG_NONE is an
 *                      invalid argument.
 *
 *
 *
 * @instruction_cnt 1 to 9
 */
#macro sig_block(SIG_OP, in_sig_list)
.begin

    #if (strstr('in_sig_list', signals))
    // list of signals case
        #ifdef _tmp_right
            #warning "_tmp_right is being redefined."
        #endif
        #ifdef _tmp_sig_list
            #warning "_tmp_sig_list is being redefined."
        #endif
        #ifdef _tmp_len
            #warning "_tmp_len is being redefined."
        #endif
        #ifdef _tmp_left
            #warning "_tmp_left is being redefined."
        #endif

        // remove keyword "signals" and left&right parentheses
        #define_eval _tmp_right (strstr('in_sig_list', ')'))
        // _tmp_sig_list = signals(sig1, sig2, ...,sign
        #define_eval _tmp_sig_list  (strleft('in_sig_list', _tmp_right - 1))
        #define_eval _tmp_len       (strlen('_tmp_sig_list'))
        #define_eval _tmp_left      (strstr('_tmp_sig_list', '('))
        // _tmp_sig_list = sig1, sig2, ..., sign for multiple signals, or
        // _tmp_sig_list = a_sig for one signal
        #define_eval _tmp_sig_list  (strright('_tmp_sig_list', _tmp_len - _tmp_left))

        #if (strstr('_tmp_sig_list', ','))
        // signals(sig1, sig2, ..., sign) case
            .reg _sig_vec _sig_events
            #ifdef _loop
                #warning "_loop is being redefined."
            #endif
            #define_eval _loop 0
            // now build signals vector _sig_vec
            #while ( !streq('_tmp_sig_list', '') )
                #ifdef _index
                    #warning "_index is being redefined."
                #endif
                #define_eval _index (strstr('_tmp_sig_list', ','))

                #ifdef _a_sig
                    #warning "_a_sig is being redefined."
                #endif
                #if (_index == 0)
                    // process the last signal:
                    //  _a_sig = the last signal
                    //  _tmp_sig_list = empty list
                    #define_eval _a_sig         '_tmp_sig_list'
                    #define_eval _tmp_sig_list  ""
                #else
                    // extract a signal to _a_sig
                    #define_eval _a_sig (strleft('_tmp_sig_list', _index - 1))
                    #define_eval _tmp_sig_list_len (strlen('_tmp_sig_list'))
                    #define_eval _tmp_sig_list (strright('_tmp_sig_list',_tmp_sig_list_len - _index))
                #endif

                // Now process this signal and merge with signals vector
                // mask(sig) returns 1 for single signal and 3 for double signal
                // &sig returns signal address which is signal value (1-15)

                // The following will be used once assembler supports the features
                // #if (_loop == 0)
                //   alu[_sig_vec, --, B, mask(_a_sig), << &_a_sig]
                // #else
                //   alu[_sig_vec, _sig_vec, OR, (mask(_a_sig)), <<&_a_sig]
                // #endif

                // use _sig_events as a temporary GPR
                immed[_sig_events, (&_a_sig)]
                alu_shf[--, _sig_events, OR, 0]

                #if (_loop == 0)
                    alu_shf[_sig_vec, --, B, 1, <<indirect]
                #else
                    alu_shf[_sig_vec, _sig_vec, OR, 1, <<indirect]
                #endif

                #define_eval _loop (_loop + 1)
                #undef _a_sig       // clean up name space
                #undef _index       // clean up name space
            #endloop
            #undef _loop        // clean up name space


            // read ACTIVE_CTX_SIG_EVENTS csr
            block_multiple_sigs#:
            local_csr_rd[ACTIVE_CTX_SIG_EVENTS]
            immed[_sig_events, 0]
            alu[_sig_events, _sig_events, AND, _sig_vec]
            #if (streq(SIG_OP, ANY))
                // result is positive if at least one of the events is set
                // ACTIVE_CTX_SIG_EVENTS is unchanged (same as ctx_arb[], any)
                beq[block_multiple_sigs#]
            #elif (streq(SIG_OP, ALL))
                // result must be the same as _sigs_vec to qualify for "ALL" events
                alu[--, _sig_events, -, _sig_vec]
                bne[block_multiple_sigs#]
                // now clear signal bits in ACTIVE_CTX_SIG_EVENTS:
                //  1. Write wakeup bits in ACTIVE_CTX_WAKEUP_EVENTS csr
                //  2. Since corresponding signal bits in ACTIVE_CTX_SIG_EVENTS are
                //          already set, the context will soon be swapped in and
                //          those bits are cleared in both CSRs by hardware.
                local_csr_wr[ACTIVE_CTX_WAKEUP_EVENTS, _sig_vec]
                ctx_arb[--]
            #else
                #error "sig_block. Invalid SIG_OP."
            #endif
        #else
        // signals(a_sig) case
            block_one_sig#:
            br_!signal[_tmp_sig_list, block_one_sig#]
        #endif

        #undef _tmp_right       // clean up name space
        #undef _tmp_sig_list    // clean up name space
        #undef _tmp_len         // clean up name space
        #undef _tmp_left        // clean up name space

    #else   // in_sig_list doesn't contain "signals"
    // single signal case
        block_single_sig#:
        br_!signal[in_sig_list, block_single_sig#]
    #endif

.end
#endm


/** Signal another microengine thread.
 *
 * @b Example:
 * @code
 * sig_post(2, SIG0)
 * @endcode
 *
 * @param in_thread_id  Constant or GPR target thread ID
 * @param in_sig_id     Constant signal id of target thread
 *
 * @instruction_cnt 3 to 8
 */
#macro sig_post(in_thread_id, in_sig_id)
.begin

    .reg _tmp
    #if (is_ct_const(in_thread_id) || isimport(in_thread_id))
        #ifdef _TARGET_ME
            #warning "_TARGET_ME is being redefined."
        #endif
        #ifdef _TARGET_CTX
            #warning "_TARGET_CTX is being redefined."
        #endif
        #define_eval _TARGET_ME (in_thread_id >> 3)
        #define_eval _TARGET_CTX (in_thread_id & 0x7)

        // ME numbers are:
        //      0-7, 0x10-0x17 for IXP28XX

        #if IS_IXPTYPE(__IXP28XX)
            #if (_TARGET_ME >= 16)
                #error "Target ME out of range."
            #endif
            #if ( _TARGET_ME > 7 )
                #define_eval _TARGET_ME ((_TARGET_ME - 8) | 0x10)
            #endif
        #else
            #error "Chip type must be IXP28XX."
        #endif

        // Interthread_sig csr layout:
        //      [11:7]  ME number
        //      [6:4]   Thread number
        //      [3:0]   signal
        #ifdef _ME_CTX_SIG
            #warning "_ME_CTX_SIG is being redefined."
        #endif
        #define_eval _ME_CTX_SIG ((_TARGET_ME<<7) | (_TARGET_CTX<<4))

        immed32(_tmp, _ME_CTX_SIG)
        alu[--, _tmp, OR, &in_sig_id]
        cap[fast_wr, ALU, INTERTHREAD_SIG]

        #undef _TARGET_ME       // clean up name space
        #undef _TARGET_CTX      // clean up name space
        #undef _ME_CTX_SIG      // clean up name space
    #else
        .begin

            .reg _tmp2

            // Get target context [0-7]
            alu_op(_tmp2, in_thread_id, AND, 0x7)
            // Merge with signal value
            alu_shf_left(_tmp2, &in_sig_id, OR, _tmp2, 4)

            // Get target ME
            shf_right(_tmp, in_thread_id, 3)
            #ifdef _CLUSTER_BIT
                #warning "_CLUSTER_BIT is being redefined."
            #endif
            #ifdef _ME_BIT_MASK
                #warning "_ME_BIT_MASK is being redefined."
            #endif

            #if IS_IXPTYPE(__IXP28XX)
                #define_eval _CLUSTER_BIT   0x3
                #define_eval _ME_BIT_MASK   0x7
            #else
                #error "Chip type must be IXP28XX."
            #endif

            br_bclr[_tmp, _CLUSTER_BIT, _lo_cluster_me#]
            alu[_tmp, _tmp, AND, _ME_BIT_MASK]
            alu[_tmp, _tmp, OR, 0x10]

        _lo_cluster_me#:

            alu_shf_left(--, _tmp2, OR, _tmp, 0x7)
            cap[fast_wr, ALU, INTERTHREAD_SIG]

            #undef _CLUSTER_BIT     // clean up name space
            #undef _ME_BIT_MASK     // clean up name space

        .end
    #endif

.end
#endm


/** Swap out and swap back in when one or more or all signals are received.
 *
 * @b Example:
 * @code
 * sig_wait(ANY, signals(SIG0, SIG1))
 * sig_wait(ANY, SIG2)
 * sig_wait(ALL, signals(SIG3, SIG4, SIG5))
 * @endcode
 *
 * @param SIG_OP        Operation performed on signal list. @n@n
 *                      Possible values are: @n
 *                      @arg @c ALL: Wait until all signals in @p in_sig_list have been received.
 *                      @arg @c ANY: Wait until any signal(s) in @p in_sig_list have been received.
 * @param in_sig_list   List of signals causing thread to wake up. This list is
 *                      either a single signal or a list of signals in the form
 *                      of signals(sig1, sig2, ..., sign). @c SIG_NONE is an
 *                      invalid argument.
 *
 * @instruction_cnt 3 to 8
 */
#macro sig_wait(SIG_OP, in_sig_list)
    #if (streq(SIG_OP, ANY))
        ctx_arb[in_sig_list], OR
    #elif (streq(SIG_OP, ALL))
        ctx_arb[in_sig_list], AND
    #else
        #error "sig_wait. Invalid SIG_OP."
    #endif
#endm


/** Perform @ref sig_block if executing context is @p in_ctx.
 *
 * @b Example:
 * @code
 * sig_ifctx_block(22, ANY, MY_SIG)
 * @endcode
 *
 * @param in_ctx        Constant context ID
 * @param SIG_OP        Operation performed on signal list. @n@n
 *                      Possible values are: @n
 *                      @arg @c ALL: Wait until all signals in @p in_sig_list have been received.
 *                      @arg @c ANY: Wait until any signal(s) in @p in_sig_list have been received.
 * @param in_sig_list   List of signals causing thread to wake up. This list is
 *                      either a single signal or a list of signals in the form
 *                      of signals(sig1, sig2, ..., sign). @c SIG_NONE is an
 *                      invalid argument.
 *
 * @instruction_cnt 1 to 10
 */
#macro sig_ifctx_block(in_ctx, SIG_OP, in_sig_list)
    br!=ctx[in_ctx, sig_ifctx_block_end#]
    sig_block(SIG_OP, in_sig_list)
sig_ifctx_block_end#:
#endm


/** Perform @ref sig_post if executing context is @p in_ctx.
 *
 * @param in_ctx        Constant context ID
 * @param in_thread_id  Constant or GPR target thread ID.
 * @param in_sig_id     Constant signal id of target thread
 *
 * @instruction_cnt 1 to 9
 */
#macro sig_ifctx_post(in_ctx, in_thread_id, in_sig_id)
    br!=ctx[in_ctx, sig_ifctx_post_end#]
    sig_post(in_thread_id, in_sig_id)
sig_ifctx_post_end#:
#endm


/** Perform @ref sig_wait if executing context is @p in_ctx.
 *
 * @b Example:
 * @code
 * sig_ifctx_wait(0, ANY, MY_SIG)
 * @endcode
 *
 * @param in_ctx        Constant context ID
 * @param SIG_OP        Operation performed on signal list. @n@n
 *                      Possible values are: @n
 *                      @arg @c ALL: Wait until all signals in @p in_sig_list have been received.
 *                      @arg @c ANY: Wait until any signal(s) in @p in_sig_list have been received.
 * @param in_sig_list   List of signals causing thread to wake up. This list is
 *                      either a single signal or a list of signals in the form
 *                      of signals(sig1, sig2, ..., sign). @c SIG_NONE is an
 *                      invalid argument.
 *
 * @instruction_cnt 1 to 2
 */
#macro sig_ifctx_wait(in_ctx, SIG_OP, in_sig_list)
    br!=ctx[in_ctx, sig_ifctx_wait_end#]
    sig_wait(SIG_OP, in_sig_list)
sig_ifctx_wait_end#:
#endm


/** @}
 * @}
 */

#endif /* __SIG_UC__ */
