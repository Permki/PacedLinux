/*
 * Copyright (C) 2008-2011 Netronome Systems, Inc.  All rights reserved.
 */

#ifndef __THREAD_UC__
#define __THREAD_UC__

#ifndef NFP_LIB_ANY_NFAS_VERSION
    #if (!defined(__NFP_TOOL_NFAS) ||                       \
        (__NFP_TOOL_NFAS < NFP_SW_VERSION(5, 0, 0, 0)) ||   \
        (__NFP_TOOL_NFAS > NFP_SW_VERSION(6, 1, 0, 255)))
        #error "This standard library is not supported for the version of the SDK currently in use."
    #endif
#endif

#include <stdmac.uc>


/** @file thread.uc Thread Operation Macros
 * @addtogroup thread  Thread Operation
 * @{
 *
 * @name Thread Operation Macros
 * @{
 *
 */

// API Quick Refernece:
//
// thread_id(out_tid, UENG_ID)
// thread_init(in_curr_num_contexts, in_prev_num_contexts)
// thread_next(out_tid, UENG_ID, in_min_thread, in_max_thread)
// thread_prev(out_tid, UENG_ID, in_min_thread, in_max_thread)



/** Given @a UENG_ID, return thread id for this context.
 *
 * @b Example:
 * @code
 * .local out_id
 * thread_id(out_id, 0)
 * @endcode
 *
 * @param out_tid   GPR, thread id
 * @param UENG_ID   Constant or GPR microengine identifier
 *
 * @instruction_cnt 3
 */
#macro thread_id(out_tid, UENG_ID)
    #ifdef _THREAD_MASK
        #warning "_THREAD_MASK is being redefined."
    #endif

    #define_eval _THREAD_MASK 0x7

    local_csr_rd[ACTIVE_CTX_STS]
    immed[out_tid, 0]                                   // out_tid here is replaced by ctx#
    alu[out_tid, _THREAD_MASK, and, out_tid]

    alu_shf_left(out_tid, out_tid, +, UENG_ID, 3)   // 8 threads per ME

    #undef _THREAD_MASK     // clean up name space
#endm


/** Called to initialize number of contexts configured for the current and the previous MEs. If there's no
 *  previous ME (already ME0 or previous ME is disabled), the value is initialized to 0 and is not used.
 *
 * @b Example:
 * @code
 * thread_init(4, 0)
 * @endcode
 *
 * @param in_curr_num_contexts  Number of contexts for current ME  (4 or 8)
 * @param in_prev_num_contexts  Number of contexts for previous ME (4, 8 or 0)
 *
 * @note _CURR_NUM_CONTEXTS and _PREV_NUM_CONTEXTS are defined
 *
 * @instruction_cnt 0
 */
#macro thread_init(in_curr_num_contexts, in_prev_num_contexts)
    #ifdef _CURR_NUM_CONTEXTS
        #undef _CURR_NUM_CONTEXTS
    #endif

    #ifdef _PREV_NUM_CONTEXTS
        #undef _PREV_NUM_CONTEXTS
    #endif

    #define_eval _CURR_NUM_CONTEXTS     in_curr_num_contexts
    #define_eval _PREV_NUM_CONTEXTS     in_prev_num_contexts
#endm


/** Given @a UENG_ID, return 1 + current thread id.
 *
 * @b Example:
 * @code
 * .local next_tid
 * thread_next(next_tid, 0, 0, 30)
 * @endcode
 *
 * @param out_tid       GPR, next value of current thread id
 * @param UENG_ID       Constant or GPR microengine identifier
 * @param in_min_thread Constant or GPR lower range thread value
 * @param in_max_thread Constant or GPR upper range thread value
 *
 * @note @a in_min_thread <= current thread id <= @a in_max_thread
 *       @a in_min_thread and @a in_max_thread have to create a thread contiguous range.
 *       Thread value of 0 is the smallest value.
 *
 * @note If an ME is configured with 4 contexts, the thread values will be 0, 2, 4, 6.
 *
 * @instruction_cnt 6 to 7
 */
#macro thread_next(out_tid, UENG_ID, in_min_thread, in_max_thread)
    // get current thread id
    thread_id(out_tid, UENG_ID)
    alu[--, out_tid, -, in_max_thread]
    bne[_thread_next_within_me#]
    nop
    move(out_tid, in_min_thread)
    br[_thread_next_done#]
    nop

    _thread_next_within_me#:

    #ifdef _THREAD_SPACING
        #warning "_THREAD_SPACING is being redefined."
    #endif

    #if (_CURR_NUM_CONTEXTS == 4)
        #define_eval _THREAD_SPACING 2
    #elif (_CURR_NUM_CONTEXTS == 8)
        #define_eval _THREAD_SPACING 1
    #else
        #error "Invalid number of contexts. For MEv2 should be 4 or 8."
    #endif

    alu[out_tid, out_tid, +, _THREAD_SPACING]

    _thread_next_done#:

    #undef _THREAD_SPACING      // clean up name space
#endm

/** Given @a UENG_ID, return current thread id - 1.
 *
 * @b Example:
 * @code
 * .local prev_tid
 * thread_prev(prev_tid, 0, 0, 15)
 * @endcode
 *
 * @param out_tid       GPR, next value of current thread id
 * @param UENG_ID       Constant or GPR microengine identifier
 * @param in_min_thread Constant or GPR lower range thread value
 * @param in_max_thread Constant or GPR upper range thread value
 *
 * @note @a in_min_thread <= current thread id <= @a in_max_thread
 *       @a in_min_thread and @a in_max_thread have to create a thread contiguous range.
 *       Thread value of 0 is the smallest value.
 *
 * @note If an ME is configured with 4 contexts, the thread values will be 0, 2, 4, 6.
 *
 * @instruction_cnt 7 to 9
 */
#macro thread_prev(out_tid, UENG_ID, in_min_thread, in_max_thread)
    // get current thread id
    thread_id(out_tid, UENG_ID)
    alu[--, out_tid, -, in_min_thread]
    bne[_thread_prev_within_me#]
    nop
    move(out_tid, in_max_thread)
    br[_thread_prev_done#]
    nop

    _thread_prev_within_me#:

    #ifdef _CURR_THREAD_SPACING
        #warning "_CURR_THREAD_SPACING is being redefined."
    #endif

    #if (_CURR_NUM_CONTEXTS == 4)
        #define_eval _CURR_THREAD_SPACING 2
    #elif (_CURR_NUM_CONTEXTS == 8)
        #define_eval _CURR_THREAD_SPACING 1
    #else
        #error "Invalid number of contexts. For MEv2 should be 4 or 8."
    #endif

    // if current thread is the smallest thread in this ME, thread spacing
    //  should be taken from the previous ME, otherwise thread spacing is
    //  taken from the current ME.

    // context 0 is always the smallest thread in a ME
    br!=ctx[0, _thread_prev_continue#]
    nop

    #ifdef _PREV_THREAD_SPACING
        #warning "_PREV_THREAD_SPACING is being redefined."
    #endif

    // this thread is smallest in this ME. Cross backwards to previous ME
    #if (_PREV_NUM_CONTEXTS == 4)
        #define_eval _PREV_THREAD_SPACING 2
    #elif (_PREV_NUM_CONTEXTS == 8)
        #define_eval _PREV_THREAD_SPACING 1
    #elif (_PREV_NUM_CONTEXTS == 0)
        // the following #define is important to prevent pre-processing from
        //  assigning wrong value for _PREV_THREAD_SPACING
        #define_eval _PREV_THREAD_SPACING _CURR_THREAD_SPACING
    #else
        #error "Invalid number of contexts. For MEv2 should be 4 or 8."
    #endif

    br[_thread_prev_done#], defer[1]
    alu[out_tid, out_tid, -, _PREV_THREAD_SPACING]

    _thread_prev_continue#:

    alu[out_tid, out_tid, -, _CURR_THREAD_SPACING]

    _thread_prev_done#:

    #ifdef _CURR_THREAD_SPACING
        #undef _CURR_THREAD_SPACING
    #endif
    #ifdef _PREV_THREAD_SPACING
        #undef _PREV_THREAD_SPACING
    #endif
#endm


/** @}
 * @}
 */

#endif /* __THREAD_UC__ */
