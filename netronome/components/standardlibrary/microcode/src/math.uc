/*
 * Copyright (C) 2011-2013 Netronome Systems, Inc.  All rights reserved.
 */

#ifndef _MATH_UC_
#define _MATH_UC_

#ifndef NFP_LIB_ANY_NFAS_VERSION
    #if (!defined(__NFP_TOOL_NFAS) ||                       \
        (__NFP_TOOL_NFAS < NFP_SW_VERSION(5, 0, 0, 0)) ||   \
        (__NFP_TOOL_NFAS > NFP_SW_VERSION(6, 1, 0, 255)))
        #error "This standard library is not supported for the version of the SDK currently in use."
    #endif
#endif
#include <stdmac.uc>


/** @file math.uc Math Operation Macros
 * @addtogroup math Math Operations
 * @{
 *
 * @name Math Operation Macros General
 * @{
 *
 * These macros perform math functions which are not provided by microcode instructions
 */


/** This define is used for nfp6000 predicate_cc optional token
  */
#define ENABLE_PREDICATE_CC // define to use nfp6000 predicate_cc optional token


// API Quick Reference:
//
// math_int_div(out_q, in_numerator, in_denominator)
// math_int_div_64(out_q, in_numerator_hi, in_numerator_lo, in_denominator)
// math_log2(out_result, in_arg, IN_ROUND)
// math_find_highest_set(out_result, in_arg)

/** 32 bit unsigned integer divide.
 *
 * @b Example:
 * @code
 * .reg dividend, divisor, quotient
 * immed[dividend,20]
 * immed[divisor,4]
 * math_int_div(quotient, dividend, divisor) // quotient will equal 5
 * @endcode
 *
 * @param out_q          GPR written with quotient ( in_numerator / in_denominator )
 * @param in_numerator   GPR or CONSTANT, dividend
 * @param in_denominator GPR or CONSTANT, divisor
 *
 * if in_numerator and in_denominator are constants, division is done using
 * assembler rather than at runtime
 *
 * out_q is set to -1 if in_denominator is 0
 */
#macro math_int_div(out_q, in_numerator, in_denominator)
#if (is_ct_const(in_numerator) && is_ct_const(in_denominator))

    move(out_q, (in_numerator/in_denominator))

#else

.begin
    .reg _d
    .reg _r
    .reg _nw, _dw
    .reg mid_shift

    move(_r, in_numerator)
    move(_d, in_denominator)

    math_find_highest_set(_dw, _d)
    math_find_highest_set(_nw, _r)

    alu[mid_shift, _nw, -, _dw]

    alu[--, mid_shift, OR, 0]         // For indirect shift
    alu[_d, --, b, _d, <<indirect]

    // If the denominator is zero, it's an error
    // (For now we assume that if the shifted value is zero, the original was
    //  zero as well. This should be OK, as a shifted value of zero will
    //  produce invalid results anyway)
    beq[math_int_div_by_zero#]

    alu[--, --, b, _r]
    beq[math_int_div_done#], defer[1]
        alu[out_q, --, b, 0]

    /**
     * From http://forum.allaboutcircuits.com/showthread.php?p=348156
     * (second last comment)
     * For each iteration there are 3 steps:
     * 1. Remainder= Remainder-Divisor
     *
     * 2. IF Remainder<0, then Add the divisor to the remainder(to get the old
     *       remainder), shift the quotient left, and the smallest digit of
     *       the quotient=0
     *    ELSE IF Remainder>=0, then shift the quotient left, and the smallest
     *       digit of the binary number =1
     *
     * 3.Shift Divisor Right.
     */

    #if ((IS_NFPTYPE(__NFP6000) || IS_NFPTYPE(__NFP3800)) && defined(ENABLE_PREDICATE_CC))
    local_csr_wr[INDIRECT_PREDICATE_CC, 0x3] // select ~N bit
    nop
    nop
    nop

math_int_div_loop_start#:
    alu[--, _r, -, _d]
    alu[out_q, 0, or, out_q, <<1], no_cc
    alu[_r, _r, -, _d], no_cc, predicate_cc
    alu[out_q, 1, or, out_q], predicate_cc
    alu[mid_shift, mid_shift, -, 1]
    bge[math_int_div_loop_start#], defer[1]
    alu[_d, --, b, _d, >>1]

    #else

math_int_div_loop_start#:
    alu[_r, _r, -, _d]
    bhs[math_int_div_next_bit_1#]

math_int_div_next_bit_0#:
    br[math_int_div_update_denominator#], defer[2]
        alu[_r, _r, +, _d]      // Restore remainder
        alu[out_q, 0, or, out_q, <<1]

math_int_div_next_bit_1#:
    alu[out_q, 1, or, out_q, <<1]

math_int_div_update_denominator#:
    alu[mid_shift, mid_shift, -, 1]
    bge[math_int_div_loop_start#], defer[1]
        alu[_d, --, b, _d, >>1]

    #endif

    br[math_int_div_done#]

math_int_div_by_zero#:
    move(out_q, -1)

math_int_div_done#:

.end

#endif  // Both inputs constants
#endm   //  math_int_div


/** 64 bit unsigned integer divide ( supports 64 bit dividend ).
 *
 * @b Example:
 * @code
 * .reg dividend_hi, dividend_lo, divisor, quotient
 * immed[dividend_hi,20]
 * immed[dividend_lo,10]
 * immed[divisor,5723]
 * math_int_div_64(quotient, dividend_hi, dividend_lo, divisor)
 * @endcode
 *
 * @param out_q             GPR written with quotient ( in_numerator / in_denominator )
 * @param in_numerator_hi   GPR upper 32 bits of dividend
 * @param in_numerator_lo   GPR lower 32 bits of dividend
 * @param in_denominator    GPR divisor
 *
 * out_q is set to -1 if in_denominator is 0
 *
 * Caution: This macro uses the basic subtract method of division and execution time
 * will be proportionate to the magnitude of the resulting quotient.
 */
#macro math_int_div_64(out_q, in_numerator_hi, in_numerator_lo, in_denominator)
.begin

#if (is_ct_const(in_numerator_hi) || is_ct_const(in_numerator_lo) || is_ct_const(in_denominator))
    #error "math_int_div_64: Constant inputs not supported"
#else

    .reg    id64_temp_upper
    .reg    id64_temp

    // If the divisor is zero, we have an error
    alu[--, --, b, in_denominator]
    beq[math_int_div_64_error_divide_by_zero#]

    // If the dividend is zero, the result will also
    // be zero
    alu[id64_temp, in_numerator_hi, or, in_numerator_lo]
    beq[math_int_div_64_end#], defer[1]
        alu[out_q, --, b, 0]


    // Although there are probably far better algorithms, for now we simply
    // use the subtract method to calculate the quotient
    alu[id64_temp_upper, --, b, in_numerator_hi]
    alu[id64_temp, --, b, in_numerator_lo]


math_int_div_64_outer_loop_start#:
math_int_div_64_subtract_loop_start#:
    alu[id64_temp, id64_temp, -, in_denominator]
    //blo[math_int_div_error_divide_end#]
    bhs[math_int_div_64_subtract_loop_start#], defer[1]
        alu[out_q, out_q, +, 1]

    // Here the lower value has dropped too low
    // Assuming the upper 32-bits is not yet zero, we don't
    // need to rollback the last operation, simply decrement
    // the upper 32-bits and continue
    alu[id64_temp_upper, id64_temp_upper, -, 1]
    bhs[math_int_div_64_outer_loop_start#]

    // When we reach this point, division is done and we need
    // to roll back the last result update
    br[math_int_div_64_end#], defer[1]
        alu[out_q, out_q, -, 1]


math_int_div_64_error_divide_by_zero#:
    // This is technically an error, although in math theory the
    // result will tend to infinity. For now, simply set the
    // quotient to the largest possible value
    move(out_q, 0xFFFFFFFF)
    br[math_int_div_64_end#]


math_int_div_64_end#:

#endif  // One of inputs constant

.end
#endm   //  math_int_div_64


/** calculate base 2 logarithm on unsigned input value.
 *
 * @b Example:
 * @code
 * .reg log2, value
 * immed[value, 32]
 * math_log2(log2, value, 0)
 * @endcode
 *
 * @param out_result        GPR written with log base 2 of in_arg
 * @param in_arg            GPR 32 bit unsigned int
 * @param IN_ROUND          Constant, used when result is not a power of 2:
 *                          round < 0: round result down to next smaller integer
 *                          round = 0: generate an error (negative output)
 *                          round > 0: round result up to next larger integer
 *
 * similar to LOG2 assembler function
 */
#macro math_log2(out_result, in_arg, IN_ROUND)
.begin

#if !(is_ct_const(IN_ROUND))
    #error "math_log2: IN_ROUND needs to be a constant number."
#endif  // IN_ROUND is a number

    .reg ml_temp
    .reg ml_value
    .reg ml_count

    // A value of 0 is invalid
    alu[ml_value, --, b, in_arg]
#if (IN_ROUND == 0)
    beq[math_log2_end#], defer[2]
        move(out_result, -1)
        alu[ml_count, --, b, 0]
#elif (IN_ROUND > 0)
    beq[math_log2_end#], defer[2]
        move(out_result, 0)
        alu[ml_count, --, b, 0]
#else
    beq[math_log2_end#], defer[2]
        move(out_result, -1)
        alu[ml_count, --, b, 0]
#endif  // IN_ROUND

math_log2_find_next_bit#:
    // Use the FFS operation to determine bit positions
    ffs[out_result, ml_value]
    alu[--, out_result, or, 0]  // For indirect shift
    alu[ml_value, ml_value, and~, 1, <<indirect]
    beq[math_log2_find_next_bit_done#], defer[1] // If the value is zero, we're done
        alu[ml_count, ml_count, +, 1]

#if (IN_ROUND == 0)
    // If the value under investigation isn't zero at this stage,
    // we don't have a power of 2 and we should log an error
    br[math_log2_end#], defer[1]
        move(out_result, -1)
#else
    br[math_log2_find_next_bit#]
#endif  // IN_ROUND is zero


math_log2_find_next_bit_done#:
#if (IN_ROUND > 0)
    alu[--, ml_count, -, 1]
    beq[math_log2_end#]     // At this point we're done

    // If we reach here, we need to round up to the next larger log value
    alu[out_result, out_result, +, 1]

// For IN_ROUND being negative, the result should be good already
//#elif (IN_ROUND < 0)

#endif  // IN_ROUND != 0


math_log2_end#:
.end
#endm   // math_log2


/** calculate highest set bit in a 32 bit unsigned value.
 *
 * @b Example:
 * @code
 * .reg bit, value
 * move(value, 0x01234567]
 * math_find_highest_set(bit, value) // bit should equal 24
 * @endcode
 *
 * @param out_result        GPR written with highest bit set of in_arg
 * @param in_arg            GPR 32 bit unsigned int
 *
 * if no bit is set, out_result is set to -1
 */
#macro math_find_highest_set(out_result, in_arg)
.begin

    // Ideas obtained from http://discuss.joelonsoftware.com/default.asp?interview.11.288979.14

    .reg fhs_val
    .reg fhs_temp

    move(fhs_val, in_arg)

    alu[--, --, b, fhs_val]
    beq[math_find_highest_set_none#], defer[1]
        alu[out_result, --, b, 0]

math_find_highest_set_16#:
    move(fhs_temp, 0xFFFF0000)
    alu[--, fhs_temp, and, fhs_val]
    beq[math_find_highest_set_8#]
    alu[out_result, out_result, +, 16]
    alu[fhs_val, --, b, fhs_val, >>16]


math_find_highest_set_8#:
    move(fhs_temp, 0x0000FF00)
    alu[--, fhs_temp, and, fhs_val]
    beq[math_find_highest_set_4#]
    alu[out_result, out_result, +, 8]
    alu[fhs_val, --, b, fhs_val, >>8]


math_find_highest_set_4#:
    move(fhs_temp, 0x000000F0)
    alu[--, fhs_temp, and, fhs_val]
    beq[math_find_highest_set_2#]
    alu[out_result, out_result, +, 4]
    alu[fhs_val, --, b, fhs_val, >>4]


math_find_highest_set_2#:
    move(fhs_temp, 0x0000000C)
    alu[--, fhs_temp, and, fhs_val]
    beq[math_find_highest_set_1#]
    alu[out_result, out_result, +, 2]
    alu[fhs_val, --, b, fhs_val, >>2]


math_find_highest_set_1#:
    move(fhs_temp, 0x00000002)
    alu[--, fhs_temp, and, fhs_val]
    beq[math_find_highest_set_done#]
    br[math_find_highest_set_done#], defer[1]
        alu[out_result, out_result, +, 1]


math_find_highest_set_none#:
    move(out_result, -1)


math_find_highest_set_done#:
.end
#endm   // math_find_highest_set


/** @}
 * @}
 */

#endif  // __MATH_UC__


