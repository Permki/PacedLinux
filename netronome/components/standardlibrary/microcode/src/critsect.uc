/*
 * Copyright (C) 2008-2011 Netronome Systems, Inc.  All rights reserved.
 */

#ifndef __CRITSECT_UC__
#define __CRITSECT_UC__

#ifndef NFP_LIB_ANY_NFAS_VERSION
    #if (!defined(__NFP_TOOL_NFAS) ||                       \
        (__NFP_TOOL_NFAS < NFP_SW_VERSION(5, 0, 0, 0)) ||   \
        (__NFP_TOOL_NFAS > NFP_SW_VERSION(6, 1, 0, 255)))
        #error "This standard library is not supported for the version of the SDK currently in use."
    #endif
#endif

#include <constants.uc>
#include <stdmac.uc>
#include <xbuf.uc>
#include <util.uc>
#if !IS_NFPTYPE(__NFP6000)
#include <scratch.uc>
#endif

/** @file critsect.uc Thread and Critical Section Macros
 * @addtogroup critsect Thread and Critical Section
 * @{
 *
 * @name Thread and Critical Section Macros
 * @{
 *
 * Macros to allow one thread in a critical section at a time.
 */

// API Quick Reference:
//
// critsect_init(io_critsect, THD_MODEL)
// critsect_enter(io_critsect, THD_MODEL)
// critsect_exit(io_critsect, THD_MODEL)



/// @cond INTERNAL_MACROS
#define CLEAR_SEMA      0x0     /**< Values for setting/clearing semaphore - unblock */
#define SET_SEMA        0x1     /**< Values for setting/clearing semaphore - block */
/// @endcond

/// @cond INTERNAL_MACROS
.sig _CRITSECT_SIG_ /**< Signal used for critical section operations */
/// @endcond


/** Initialize and clear a semaphore representing a critical section of microengine code.
 *
 * @b Example:
 * @code
 * critsect_init(@foo, LOCAL_THREADS)
 *
 * critsect_init(addr, GLOBAL_THREADS)
 * @endcode
 *
 * @param io_critsect   Initialized semaphore. For LOCAL_THREADS, absolute register that is used
 *                      as critical section parameter. Note that index is not allowed since 8 bits
 *                      (0-7) of this parameter are initialized to support 8 critical sections.
 *                      For GLOBAL_THREADS, register or constant longword address. Each parameter
 *                      supports one critical section only.
 * @param THD_MODEL     LOCAL_THREADS for threads in one microengine.
 *                      GLOBAL_THREADS for threads in multiple microengines
 *
 * @instruction_cnt 1       for LOCAL_THREADS  (no memory access)
 * @instruction_cnt 2 to 5  for GLOBAL_THREADS (1 scratch write to initialize semaphore)
 */
#macro critsect_init(io_critsect, THD_MODEL)
.begin

#if (streq(THD_MODEL, GLOBAL_THREADS) && IS_NFPTYPE(__NFP6000))
    #error "critsect_init: THD_MODEL GLOBAL_THREADS not supported for NFP6000."
#endif

#if (streq(THD_MODEL, LOCAL_THREADS))
    immed[io_critsect, CLEAR_SEMA]

#elif (streq(THD_MODEL, GLOBAL_THREADS))
    xbuf_alloc($_critsect_xfer, 1, read_write)
    immed[$_critsect_xfer[0], 0]

    // use macro to take care of io_critsect input types
    scratch_write($_critsect_xfer[0], io_critsect, 0, 1, _CRITSECT_SIG_, _CRITSECT_SIG_, ___)
    xbuf_free($_critsect_xfer)

#else
    #error "critsect_init: invalid input for THD_MODEL."
#endif

.end
#endm


/** If a semaphore respresenting a critical section of microengine is set, swap and
 *  retest until it is clear. When the semaphore is clear, set it.
 *
 * @b Example:
 * @code
 * critsect_init(@foo, LOCAL_THREADS)
 * critsect_enter(@foo[0], LOCAL_THREADS) or
 * critsect_enter(@foo0, LOCAL_THREADS)
 * critsect_enter(@foo7, LOCAL_THREADS)
 *
 * critsect_init(addr, GLOBAL_THREADS)
 * critsect_enter(addr, GLOBAL_THREADS)
 * @endcode
 *
 * @param io_critsect   Set to block other threads from entering the critical section code.
 *                      A critical section specified by an index (0-7)
 * @param THD_MODEL     LOCAL_THREADS for threads in one microengine.
 *                      GLOBAL_THREADS for threads in multiple microengines
 *
 * @instruction_cnt 3 to 5  for LOCAL_THREADS  (no memory access)
 * @instruction_cnt 6 to 13 for GLOBAL_THREADS (1 atomic scratch x number of times to check for
 *                                              critsect semaphore availability)
 */
#macro critsect_enter(io_critsect, THD_MODEL)

#if (streq(THD_MODEL, GLOBAL_THREADS) && IS_NFPTYPE(__NFP6000))
    #error "critsect_enter: THD_MODEL GLOBAL_THREADS not supported for NFP6000."
#endif

#if (streq(THD_MODEL, LOCAL_THREADS))
    _DEF_NAME_FROM_ARRAY_TOK(_NAME, io_critsect)
    _DEF_INDEX_FROM_ARRAY_TOK(_INDEX, io_critsect)

    #ifdef _BIT_MASK
        #warning "_BIT_MASK is being redefined."
    #endif
    #define_eval _BIT_MASK (1 << _INDEX)

    // swap and retest if semaphore is not clear
begin_loop_local#:
    alu_op(--, _BIT_MASK, AND, _NAME)
    bne[wait_loop_local#]
    br[end_loop_local#], defer[1]

    // set semaphore to block other threads from entering critical section code
    alu_op(_NAME, _BIT_MASK, OR, _NAME)
wait_loop_local#:
    nop
    nop
    ctx_arb[voluntary], br[begin_loop_local#]
end_loop_local#:

    #undef _BIT_MASK        // clean up name space

#elif (streq(THD_MODEL, GLOBAL_THREADS))
.begin

        xbuf_alloc($_critsect_xfer, 1, read_write)
    begin_loop_global#:
        scratch_test_and_incr($_critsect_xfer[0], io_critsect, 0, _CRITSECT_SIG_, _CRITSECT_SIG_, ___)
        alu[--, $_critsect_xfer[0], -, 1]
        bmi[end_loop_global#], defer[3]
        nop
        nop
        nop
        ctx_arb[voluntary]
        br[begin_loop_global#], defer[2]
        nop
        nop
    end_loop_global#:
        xbuf_free($_critsect_xfer)
        // do nothing here since value of semaphore got incremented already by
        // test_and_incr instruction. Exit macro will reset semaphore.

.end
#else
    #error "critsect_enter: invalid input for THD_MODEL."
#endif
#endm


/** Clear a semaphore representing a critical section of microengine code.
 *
 * @b Example:
 * @code
 * critsect_init(@foo, LOCAL_THREADS)
 * critsect_enter(@foo[0], LOCAL_THREADS)
 *      <critical section code ...>
 * critsect_exit(@foo[0], LOCAL_THREADS)
 *
 * critsect_init(addr, GLOBAL_THREADS)
 * critsect_enter(addr, GLOBAL_THREADS)
 *      <critical section code ...>
 * critsect_exit(addr, GLOBAL_THREADS)
 * @endcode
 *
 * @param io_critsect   Cleared
 * @param THD_MODEL     LOCAL_THREADS for threads in one microengine.
 *                      GLOBAL_THREADS for threads in multiple microengines
 *
 * @instruction_cnt 1 to 2  for LOCAL_THREADS  (no memory access)
 * @instruction_cnt 2 to 5  for GLOBAL_THREADS (1 scratch write to clear semaphore)
 */
#macro critsect_exit(io_critsect, THD_MODEL)

#if (streq(THD_MODEL, GLOBAL_THREADS) && IS_NFPTYPE(__NFP6000))
    #error "critsect_exit: THD_MODEL GLOBAL_THREADS not supported for NFP6000."
#endif

#if (streq(THD_MODEL, LOCAL_THREADS))
    _DEF_NAME_FROM_ARRAY_TOK(_NAME, io_critsect)
    _DEF_INDEX_FROM_ARRAY_TOK(_INDEX, io_critsect)

    #ifdef _BIT_MASK
        #warning "_BIT_MASK is being redefined."
    #endif
    #define_eval _BIT_MASK (1 << _INDEX)

    // alu_op will instantiate 1 instruction for 0-7 critsects, 2 instructions
    // otherwise.
    alu_op(_NAME, _NAME, AND~, _BIT_MASK)
    #undef _BIT_MASK
#elif (streq(THD_MODEL, GLOBAL_THREADS))
.begin

    xbuf_alloc($_critsect_xfer, 1, read_write)
    immed[$_critsect_xfer[0], 0]

    // use macro to take care of io_critsect input types
    scratch_write($_critsect_xfer[0], io_critsect, 0, 1, _CRITSECT_SIG_, _CRITSECT_SIG_, ___)
    xbuf_free($_critsect_xfer)

.end
#else
    #error "critsect_exit: invalid input for THD_MODEL."
#endif
#endm

/** @}
 * @}
 */


#endif /* __CRITSECT_UC__ */
