/*
 * Copyright (C) 2011-2013 Netronome Systems, Inc.  All rights reserved.
 */

#ifndef _THREADS_UC_
#define _THREADS_UC_

#ifndef NFP_LIB_ANY_NFAS_VERSION
    #if (!defined(__NFP_TOOL_NFAS) ||                       \
        (__NFP_TOOL_NFAS < NFP_SW_VERSION(5, 0, 0, 0)) ||   \
        (__NFP_TOOL_NFAS > NFP_SW_VERSION(6, 1, 0, 255)))
        #error "This standard library is not supported for the version of the SDK currently in use."
    #endif
#endif

/** @file threads.uc Thread Synchronization Macros
 * @addtogroup threads  Thread Synchronization
 * @{
 *
 * @name Thread Synchronization Macros
 * @{
 * Thread Synchronization Macros
 */

// API Quick Refernece:
//
// threads_reorder_once(in_sig)
// threads_br_ctx(CTX_MASK, LABEL)
// threads_kill(KILL_MASK)
// threads_order(out_next, in_sig, ORDERED_MASK)


/** Reorder threads. Threads enabled to run in order 0->7
 *
 * @b Example:
 * @code
 * .sig s
 * threads_reorder_once(s)
 * [work to do before next thread runs]
 * ctx_arb[voluntary] // let next thread run
 * @endcode
 *
 * @param in_sig  signal
 *
 */
#macro threads_reorder_once(in_sig)

    // Re-order the threads
    .if (ctx() == 7)
        local_csr_wr[SAME_ME_SIGNAL, ((1 << 7) | (&in_sig << 3))]
        ctx_arb[in_sig]
    .else
        ctx_arb[in_sig]
        local_csr_wr[SAME_ME_SIGNAL, ((1 << 7) | (&in_sig << 3))]
    .endif

#endm


/** Take a branch if running on thread indicated in bit mask.
 *
 * @b Example:
 * @code
 * .reg r_thd_3_0, r_thd_7_4
 * #define THD_MASK 0xF0
 * threads_br_ctx(THD_MASK, thd_7_4_running#)
 * thd_3_0_running#:
 * immed[r_thd3_0, 1]
 * br[done#]
 * thd_7_4_running#:
 * immed[r_thd7_4, 1]
 * done#:
 * @endcode
 *
 * @param MASK    Constant, bit mask, one bit per thread
 * @param LABEL   Constant, branch target label
 */
#macro threads_br_ctx(CTX_MASK, LABEL)

    #if ((CTX_MASK) >> 8)
        #error "Only bits 0 to 7 may be set in CTX_MASK"
    #endif

    #ifdef _THREADS_LOOP
        #error "_THREADS_LOOP redefined"
    #endif

    #define_eval _THREADS_LOOP 0
    #while (_THREADS_LOOP < 8)

        #if ((CTX_MASK) & (1 << _THREADS_LOOP))
            br=ctx[_THREADS_LOOP, LABEL]
        #endif

        #define_eval _THREADS_LOOP (_THREADS_LOOP + 1)

    #endloop
    #undef _THREADS_LOOP

#endm


/** Kill threads indicated in bit mask.
 *
 * @b Example:
 * @code
 * #define THD_MASK 0xAA
 * threads_kill(THD_MASK) // kill threads 7,5,3,1
 * @endcode
 *
 * @param MASK    Constant, bit mask, one bit per thread
 */
#macro threads_kill(KILL_MASK)

    #if ((KILL_MASK) >> 8)
        #error "Only bits 0 to 7 may be set in KILL_MASK"
    #endif

    #ifdef _THREADS_LOOP
        #error "_THREADS_LOOP redefined"
    #endif

    #define_eval _THREADS_LOOP 0
    #while (_THREADS_LOOP < 8)

        #if ((KILL_MASK) & (1 << _THREADS_LOOP))
            .if (ctx() == _THREADS_LOOP)
                ctx_arb[kill]
            .endif
        #endif

        #define_eval _THREADS_LOOP (_THREADS_LOOP + 1)

    #endloop
    #undef _THREADS_LOOP

#endm


/** Order threads enabled in bit mask
 *
 * @b Example:
 * @code
 * .sig s
 * .reg next_val
 * #define THD_MASK 0xAA
 * threads_order(next_val, s, THD_MASK) // order threads 1,3,5,7
 * @endcode
 *
 * @param out_next      GPR, written with
 * @param in_sig        signal
 * @param ORDERED_MASK  Constant, bit mask, one bit per thread of threads
 *                      to be ordered
 */
#macro threads_order(out_next, in_sig, ORDERED_MASK)
.begin

    #if ((ORDERED_MASK) >> 8)
        #error "Only bits 0 to 7 may be set in ORDERED_MASK"
    #endif

    #if ((ORDERED_MASK) == 0)
        #error "At least one bit must be set in ORDERED_MASK"
    #endif

    #ifdef _THREADS_LOOP
        #error "_THREADS_LOOP redefined"
    #endif

    #ifdef _THREADS_NEXT_ORDERED
        #error "_THREADS_NEXT_ORDERED redefined"
    #endif

    #ifdef _THREADS_FIRST_ORDERED
        #error "_THREADS_FIRST_ORDERED redefined"
    #endif

    // Find the first thread in ORDERED_MASK
    #define_eval _THREADS_FIRST_ORDERED -1
    #define_eval _THREADS_LOOP 0
    #while (_THREADS_LOOP < 8)

        #if (((ORDERED_MASK) & (1 << _THREADS_LOOP)) && (_THREADS_FIRST_ORDERED == -1))
            #define_eval _THREADS_FIRST_ORDERED _THREADS_LOOP
        #endif

        #define_eval _THREADS_LOOP (_THREADS_LOOP + 1)

    #endloop


    // Do the loop in reverse to ease wrap-around of _THREADS_NEXT_ORDERED
    #define_eval _THREADS_NEXT_ORDERED _THREADS_FIRST_ORDERED
    #define_eval _THREADS_LOOP 7
    #while (_THREADS_LOOP >= 0)

        #if ((ORDERED_MASK) & (1 << _THREADS_LOOP))

            .if (ctx() == _THREADS_LOOP)

                // This thread must signal the next ordered thread

                // Next ordered thread to signal
                immed[out_next, ((&in_sig << 3) | _THREADS_NEXT_ORDERED)]

                // Start the first ordered thread
                #if (_THREADS_NEXT_ORDERED == _THREADS_FIRST_ORDERED)
                    local_csr_wr[SAME_ME_SIGNAL, ((&in_sig << 3) | _THREADS_FIRST_ORDERED)]
                #endif

                #define_eval _THREADS_NEXT_ORDERED _THREADS_LOOP

                .set_sig in_sig
                ctx_arb[in_sig]

            .endif

        #endif

        #define_eval _THREADS_LOOP (_THREADS_LOOP - 1)

    #endloop


    #undef _THREADS_LOOP
    #undef _THREADS_NEXT_ORDERED
    #undef _THREADS_FIRST_ORDERED

.end
#endm // threads_order()


/** @}
 * @}
 */

#endif // _THREADS_UC_
