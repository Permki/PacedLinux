/*
 * Copyright (C) 2008-2011 Netronome Systems, Inc.  All rights reserved.
 */

#ifndef __CYCLE_UC__
#define __CYCLE_UC__

#ifndef NFP_LIB_ANY_NFAS_VERSION
    #if (!defined(__NFP_TOOL_NFAS) ||                       \
        (__NFP_TOOL_NFAS < NFP_SW_VERSION(5, 0, 0, 0)) ||   \
        (__NFP_TOOL_NFAS > NFP_SW_VERSION(6, 1, 0, 255)))
        #error "This standard library is not supported for the version of the SDK currently in use."
    #endif
#endif

#include <stdmac.uc>
#include <xbuf.uc>

/** @file cycle.uc Microengine Cycle Operation Macros
 * @addtogroup me_cycle Microengine Cycle Operation
 * @{
 *
 * @name Microengine Cycle Operation Macros
 * @{
 *
 */

// API Quick Reference:
//
// cycle_delay(in_cycle_count, loop_action)
// cycle32_delay(in_cycle_count)
// cycle32_diff(out_diff, in_later_timer, in_prev_timer)
// cycle32_sleep(in_cycle_count)
// cycle32_read(out_cycle_lo)
// cycle32_diff(out_diff, in_later_timer, in_prev_timer)
// cycle64_diff(out_diff_hi, out_diff_lo, in_later_timer_hi, in_later_timer_lo, in_prev_timer_hi, in_prev_timer_lo)
// cycle64_read(out_cycle_hi, out_cycle_lo)



/** Loop for @p delay cycles and do not swap out if in tight loop mode.
 *
 * @param delay         Constant value of 16 bit. Loops @p delay cycles.
 * @param loop_action   @c "LOOP_TIGHT" or @c "--". If @c LOOP_TIGHT, spins
 *                      @c delay cycles without swapping. Otherwise the thread
 *                      voluntarily swaps out every 15 cycles
 *
 * @note In @c TIGHT_LOOP mode it consume 3 cycles per decrement so, @c delay constant passed
 *       in has to be scaled properly to account for this. In @c NON_TIGHT_LOOP (@c --), it consume 4 cycles.
 */
#macro cycle_delay(delay, loop_action)
.begin

    .reg ctr
    #if is_ct_const(delay)
        immed32(ctr, delay)
    #else
        move(ctr, delay)
    #endif
sleep_label#:
    #if (!streq('loop_action', 'LOOP_TIGHT'))
        alu[--, ctr, and, 0xf]
        bne[continue_label#]
        ctx_arb[voluntary]
    continue_label#:
        alu[ctr, ctr, -, 1]
        bne[sleep_label#]
    #else
        nop
        alu[ctr, ctr, -, 1]
        bne[sleep_label#]
    #endif

.end
#endm


/** Delay approx @c n cycles and do not swap out.
 *
 * Example:
 * @code
 * cycle32_delay(20) // delay 20 cycles
 * @endcode
 *
 * @param in_cycle_count    Cycles to delay (GPR or constant)
 *
 * @instruction_cnt 7 to 10
 */
#macro cycle32_delay(in_cycle_count)
.begin

    #if (is_ct_const(in_cycle_count) || isimport(in_cycle_count))
        .reg _delay_val
        immed32(_delay_val, in_cycle_count)
    #else
        #ifdef _delay_val
            #warning "_delay_val is being redefined."
        #endif
        #define_eval _delay_val (in_cycle_count)
    #endif

    shf_right(_delay_val, _delay_val, 3)       // to account for 8 cycles per iteration
    .while (_delay_val > 0)
        alu[_delay_val, _delay_val, -, 1]
        // these three end up in branch defer shadow
        nop
        nop
        nop
        // these 2 to get to 8 cycle
        nop
        nop
    .endw

    #ifdef _delay_val
        #undef _delay_val
    #endif

.end
#endm


/** Compare time interval with (@p in_later_timer - @p in_prev_timer), accounting for wrap
 *
 * Example:
 * @code
 * cycle32_read(timer0)
 * cycle32_read(timer1)
 * cycle32_diff(diff, timer1, timer0)
 * @endcode
 *
 * @param out_diff          Positive elapsed cycle count
 * @param in_later_timer    More recent cycle time than in_prev_timer
 * @param in_prev_timer     Previous cycle time
 *
 * @instruction_cnt 5 to 10
 */
#macro cycle32_diff(out_diff, in_later_timer, in_prev_timer)
.begin

    .reg _temp_new_timer
    shf_right(_temp_new_timer, in_later_timer, 2) // ignore low 2 bits
    alu_shf_right(--, _temp_new_timer, -, in_prev_timer, 2)
    bpl[pos_diff#]
        alu_shf_left(_temp_new_timer, _temp_new_timer, +, 1, 30)
    pos_diff#:
    alu_shf_right(out_diff, _temp_new_timer, -, in_prev_timer, 2)
    shf_left(out_diff, out_diff, 2)

.end
#endm


/** Sleep for @c n cycles, let other contexts run.
 *
 * Example:
 * @code
 * cycle32_sleep(100) //sleep 100 cycles
 * @endcode
 *
 * @param in_cycle_count    Constant or GPR. cycles to sleep
 *
 * @instruction_cnt 7 to 8
 */
#macro cycle32_sleep(in_cycle_count)
.begin

    .reg tmp_cycle_count, _ixp_future_count_signal_num
    .sig _ixp_future_count_signal_name
    // This is to get rid of warning that "signal is being used
    // before being set". This lets the assembler know that signal
    // is being generated somewhere else.
    .set_sig _ixp_future_count_signal_name

    // Passing address of signal
    alu[_ixp_future_count_signal_num, --, B, &_ixp_future_count_signal_name]

    // use future count CSR to wakeup from sleep
    // the counter resolution is 16 chip cycles, so in_cycle_count must be divided by 16
    local_csr_rd[TIMESTAMP_LOW]
    immed[tmp_cycle_count, 0]

    #if (is_ct_const(in_cycle_count) || isimport(in_cycle_count))
        #ifdef _chip_timer_count
            #warning "_chip_timer_count is being redefined."
        #endif
        #define_eval _chip_timer_count (in_cycle_count / 16)
        add(tmp_cycle_count, tmp_cycle_count, _chip_timer_count)
        #undef _chip_timer_count
    #else
        alu_shf_right(tmp_cycle_count, tmp_cycle_count, +, in_cycle_count, 4)
    #endif

    local_csr_wr[ACTIVE_CTX_FUTURE_COUNT, tmp_cycle_count]
    local_csr_wr[ACTIVE_FUTURE_COUNT_SIGNAL, _ixp_future_count_signal_num]
    ctx_arb[_ixp_future_count_signal_name]

.end
#endm


/** Read the microengine's low timestamp counter.
 *
 * Example:
 * @code
 * .xfer_order $cycle_lo $cycle_hi
 * cycle32_read($cycle_lo)
 * @endcode
 *
 * @param out_cycle_lo  Absolute, relative, or write transfer register.
 *                      If this is a write transfer register, it cannot
 *                      be used in consequent calculations such as @ref cycle32_diff.
 *
 *
 * @instruction_cnt 2
 */
#macro cycle32_read(out_cycle_lo)
    local_csr_rd[TIMESTAMP_LOW]
    immed[out_cycle_lo, 0]  // out_tid here is replaced by ctx#
#endm


/** Read the microengine's high and low timestamp counters.
 *
 * If both @p out_cycle_lo and @p out_cycle_hi are write transfer registers
 * they must be part of a .xfer_order to ensure that @p out_cycle_lo is not
 * reused as @p out_cycle_hi.
 *
 * Example:
 * @code
 * .xfer_order $cycle_lo $cycle_hi
 * cycle64_read($cycle_hi, $cycle_lo)
 *
 * xbuf_alloc($cycle)
 * cycle64_read($cycle[1], $cycle[0])   // $cycle[1] is hi, $cycle[0] is lo
 * xbuf_free($cycle)                    // use it
 *
 * cycle64_read(out_cycle_hi, out_cycle_lo)
 * @endcode
 *
 * @param out_cycle_hi  Absolute, relative, or write transfer register.
 *                      If this is a write transfer register, it cannot
 *                      be used in consequent calculations such as @ref cycle32_diff.
 * @param out_cycle_lo  Absolute, relative, or write transfer register.
 *                      If this is a write transfer register, it cannot
 *                      be used in consequent calculations such as @ref cycle32_diff.
 *
 * @note The cycle count comes back longword little-endian, i.e., first transfer register is
 *       least significant.
 *
 * @instruction_cnt 4
 */
#macro cycle64_read(out_cycle_hi, out_cycle_lo)
    local_csr_rd[TIMESTAMP_LOW]
    immed[out_cycle_lo, 0]  // out_tid here is replaced by ctx#
    local_csr_rd[TIMESTAMP_HIGH]
    immed[out_cycle_hi, 0]  // out_tid here is replaced by ctx#
#endm


/** Get time difference of two 64 bit timestamps.
 *
 * @param out_diff_hi        Positive elapsed cycle count, high 32 bits.
 * @param out_diff_lo        Positive elapsed cycle count, low 32 bits.
 * @param in_later_timer_hi  More recent cycle time, high 32 bits.
 * @param in_later_timer_lo  More recent cycle time, low 32 bits.
 * @param in_prev_timer_hi   Previous cycle time, high 32 bits.
 * @param in_prev_timer_lo   Previous cycle time, low 32 bits.
 *
 * @note Because this exceeds the life of the chips involved, no wrap is assumed.
 *
 * @instruction_cnt 2 to 3
 */
#macro cycle64_diff(out_diff_hi, out_diff_lo, in_later_timer_hi, in_later_timer_lo, in_prev_timer_hi, in_prev_timer_lo)
    sub(out_diff_lo, in_later_timer_lo, in_prev_timer_lo)
    sub(out_diff_hi, in_later_timer_hi, in_prev_timer_hi)
    alu[out_diff_lo, in_later_timer_lo, -carry, in_prev_timer_lo]
#endm

/** @}
 * @}
 */

#endif /* __CYCLE_UC__ */
