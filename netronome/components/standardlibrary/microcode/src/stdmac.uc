/*
 * Copyright (C) 2008-2011 Netronome Systems, Inc.  All rights reserved.
 */

#ifndef __STDMAC_UC__
#define __STDMAC_UC__

#ifndef NFP_LIB_ANY_NFAS_VERSION
    #if (!defined(__NFP_TOOL_NFAS) ||                       \
        (__NFP_TOOL_NFAS < NFP_SW_VERSION(5, 0, 0, 0)) ||   \
        (__NFP_TOOL_NFAS > NFP_SW_VERSION(6, 1, 0, 255)))
        #error "This standard library is not supported for the version of the SDK currently in use."
    #endif
#endif

#include <constants.uc>

/** @file stdmac.uc Microengine Standard Macros
 * @addtogroup stdmac Microengine Standard Macros
 * @{
 *
 * @name Microengine Standard Macros
 * @{
 * This paragraph provides a list of generic macros
 */

// API Quick Reference:
//
// add(out_result, in_a, in_b)
// add_c(out_result, in_a, in_b)
// add_shf_left(out_result, in_a, in_b, in_shift_amt)
// add_shf_right(out_result, in_a, in_b, in_shift_amt)
// alu_op(out_result, in_a, op_spec, in_b)
// alu_rot_left(out_result, in_a, op_spec, in_b, in_shift_amt)
// alu_rot_right(out_result, in_a, op_spec, in_b, in_shift_amt)
// alu_shf_left(out_result, in_a, op_spec, in_b, in_shift_amt)
// alu_shf_right(out_result, in_a, op_spec, in_b, in_shift_amt)
// and_shf_left(out_result, in_a, in_b, in_shift_amt)
// and_shf_right(out_result, in_a, in_b, in_shift_amt)
// array_index_from_elem_addr(out_index, in_addr, ELEMENT_SIZE, BASE_ADDRESS)
//          alias: index_from_addr
// balr(in_link_pc, target_label)
// bitfield_extract(out_result, in_src, MSB, LSB)
// bitfield_insert(out_result, in_a, in_b, MSB, LSB)
// bits_clr(io_data, in_start_bit_pos, in_mask)
// bits_set(io_data, in_start_bit_pos, in_mask)
// divide(out_result, in_dividend, DIVISOR)
// elem_addr_from_array_index(out_addr, in_index, ELEMENT_SIZE, BASE_ADDRESS)
//          alias: addr_from_index
// immed32(out_result, VAL)
// immed40(out_result_hi, out_result_lo, VAL)
// move(out_result, in_src)
// multiply(out_result, in_a, in_b)
// or_shf_left(out_result, in_a, in_b, in_shift_amt)
// or_shf_right(out_result, in_a, in_b, in_shift_amt)
// rot_left(out_result, in_src, in_shift_amt)
// rot_right(out_result, in_src, in_shift_amt)
// shf_left(out_result, in_src, in_shift_amt)
// shf_right(out_result, in_src, in_shift_amt)
// sub(out_result, in_a, in_b)
// sub_shf_left(out_result, in_a, in_b, in_shift_amt)
// sub_shf_right(out_result, in_a, in_b, in_shift_amt)
// arith_shf_right(out_result, in_src, shift_amt, SIGN_BIT_POS)
// multiply32(out_result, in_a, in_b, OPERAND_SIZE)
// multiply64(out_result, in_a, in_b, OPERAND_SIZE)
// rand(out_result)
// srand(in_src)



// internal macros to support stdmac
#ifdef EARLY_MEV2
    #error "stdmac.uc no longer supports early versions of the Intel (R) IXP microengine"
#endif


/// @cond INTERNAL_MACROS
/** Internal macro for ALU instructions. If @p op_spec is +4, use immed to mask off
 *  the B operand.
 *
 * @param out_result    Destination GPR or write transfer register
 * @param in_a          Register or constant
 * @param op_spec       ALU operation @n @n
 *                      Legal operators are:
 *                      @arg @c B
 *                      @arg @c ~B
 *                      @arg @c +
 *                      @arg @c +carry
 *                      @arg @c +4
 *                      @arg @c +8
 *                      @arg @c +16
 *                      @arg @c -
 *                      @arg @c B-A
 *                      @arg @c AND
 *                      @arg @c ~AND
 *                      @arg @c AND~
 *                      @arg @c OR
 *                      @arg @c XOR
 * @param in_b          Register or constant
 */
#macro _nfp_alu(out_result, in_a, op_spec, in_b)
    #if (streq ('op_spec', '+4'))
    .begin

        .reg nfp_temp
        alu_shf[nfp_temp, 0xF, AND, in_b]
        alu[out_result, in_a, +, nfp_temp]

    .end
    #else
        alu[out_result, in_a, op_spec, in_b]
    #endif
#endm
/// @endcond


/// @cond INTERNAL_MACROS
/** Internal macro to detect if ALU / SHIFT operation can be done in one cycle
 *  and if so, use one instruction, else, take 2 instructions (shift and add).
 *
 * @param out_result    Destination GPR or write transfer register
 * @param in_a          Register or constant
 * @param op_spec       ALU operation @n @n
 *                      Legal operators are:
 *                      @arg @c B
 *                      @arg @c ~B
 *                      @arg @c +carry
 *                      @arg @c +4
 *                      @arg @c +8
 *                      @arg @c +16
 *                      @arg @c -
 *                      @arg @c B-A
 *                      @arg @c AND
 *                      @arg @c ~AND
 *                      @arg @c AND~
 *                      @arg @c OR
 *                      @arg @c XOR
 * @param in_b          Register or constant
 * @param in_shift_spec Register or constant (0 to 31)
 */
#macro _nfp_alu_shf(out_result, in_a, op_spec, in_b, in_shift_spec)
    #if (streq ('op_spec', 'B'))
        alu[out_result, in_a, op_spec, in_b, in_shift_spec]
    #elif (streq ('op_spec', '~B'))
        alu[out_result, in_a, op_spec, in_b, in_shift_spec]
    #elif (streq ('op_spec', 'OR'))
        alu[out_result, in_a, op_spec, in_b, in_shift_spec]
    #elif (streq ('op_spec', 'ASR'))
        alu[out_result, in_a, op_spec, in_b, in_shift_spec]
    #elif (streq ('op_spec', 'AND'))
        alu[out_result, in_a, op_spec, in_b, in_shift_spec]
    #elif (streq ('op_spec', '~AND'))
        alu[out_result, in_a, op_spec, in_b, in_shift_spec]
    #elif (streq ('op_spec', 'AND~'))
        alu[out_result, in_a, op_spec, in_b, in_shift_spec]
    #else
        .begin

            .reg nfp_temp
            #if (streq ('op_spec', '+4'))
                alu_shf[nfp_temp, 0xF, AND, in_b, in_shift_spec]
                alu[out_result, in_a, +, nfp_temp]
            #elif (streq ('op_spec', '+carry'))
                alu[out_result, in_a, +carry, 0]
                alu_shf[nfp_temp, --, B, in_b, in_shift_spec]
                alu[out_result, out_result, +, nfp_temp]
            #elif (streq ('op_spec', '-carry'))
                alu[out_result, in_a, -carry, 0]
                alu_shf[nfp_temp, --, B, in_b, in_shift_spec]
                alu[out_result, out_result, -, nfp_temp]
            #else
                alu_shf[nfp_temp, --, B, in_b, in_shift_spec]
                alu[out_result, in_a, op_spec, nfp_temp]
            #endif

        .end
    #endif
#endm
/// @endcond


/** Load @p out_result with 32 bit constant.
 *
 * If constant fits in 16 bits, expand to a single immed, if not,
 * expand into an immed_w0 and an immed_w1. If it is a name (not a number),
 * assume that it is being imported (an import variable).
 *
 * @b Example:
 * @code
 * immed32(output, 0x12345678)
 * @endcode
 *
 * @param out_result    Destination GPR or write transfer register
 * @param VAL           Constant
 *
 * @instruction_cnt 1 to 2 (3 if output is transfer register)
 */
#macro immed32(out_result, VAL)

    #if (is_ct_const(VAL))
        #define_eval VALTEST VAL

        // Verify that the signed 64-bit constant can mean the same thing
        // as a sign extended 32-bit constant.
        #if (defined(__PREPROC64))
            #if ((VALTEST > 0xFFFFFFFF) || (VALTEST < -4294967295))
                #error "Invalid 32-bit constant:" VAL "==" VALTEST
            #endif
        #endif
        #undef VALTEST
    #endif

    #if (!is_ct_const(VAL))
        #if (strstr(out_result, $)) // check for output xfer reg
            .begin

                .reg immed32_temp
                immed[immed32_temp, ((VAL) & 0xFFFF)]
                immed_w1[immed32_temp, (((VAL) >> 16) & 0xFFFF)]
                alu[out_result, --, b, immed32_temp]

            .end
        #else   // !strstr(out_result, $)
            immed[out_result, ((VAL) & 0xFFFF)]
            immed_w1[out_result, (((VAL) >> 16) & 0xFFFF)]
        #endif  // strstr(out_result, $)
    #elif ((((VAL) & 0xFFFF0000) == 0) || (((VAL) & 0xFFFF0000) == 0xFFFF0000))
        immed[out_result, (VAL)]
    #elif ((((VAL) & 0xFF0000FF) == 0) || (((VAL) & 0xFF0000FF) == 0xFF0000FF))
        #if (defined(__PREPROC64) && ((VAL & 0x80000000) != 0))
            #define_eval SIGNEX_VAL (VAL) | 0xffffffff00000000
        #else
            #define_eval SIGNEX_VAL (VAL)
        #endif

        immed[out_result, ((SIGNEX_VAL) >> 8), <<8]
        #undef SIGNEX_VAL
    #elif (((VAL) & 0x0000FFFF) == 0)
        /* make sure that number is not seen as negative */
        immed[out_result, (0x0000FFFF & ((VAL) >> 16)), <<16]
    #elif (((VAL) & 0x0000FFFF) == 0x0000FFFF)
        /* make sure that number is seen as negative */
        immed[out_result, (0xFFFF0000 | ((VAL) >> 16)), <<16]
    #else
        // const with value larger than 16 bits (normal or inverted)
        #if (strstr(out_result, $)) // check for output xfer reg
            .begin

                .reg immed32_temp
                immed[immed32_temp, ((VAL) & 0xFFFF)]
                immed_w1[immed32_temp, (((VAL) >> 16) & 0xFFFF)]
                alu[out_result, --, b, immed32_temp]

            .end
        #else   // !(strstr(out_result, $))
            immed[out_result, ((VAL) & 0xFFFF)]
            immed_w1[out_result, (((VAL) >> 16) & 0xFFFF)]
        #endif  // (strstr(out_result, $))
    #endif  // (!is_ct_const(VAL))
#endm


/** Load the upper byte of the 40-bit immediate value in the upper byte of
 * @p out_result_hi and load the lower 4 bytes into @p out_result_lo
 * Can be used with immediate constant or import variable
 *
 * This macro is specifically for defining a 40-bit addresses
 *
 * @param out_result_hi     Destination GPR or xfer register
 * @param out_result_lo     Destination GPR or xfer register
 * @param VAL               40-bit Constant
 *
 * @b Example:
 *  .reg addr_hi, addr_lo
 *  .reg write $wr_buf
 *  .sig s
 *
 *  #define TEST_BASE_ADDR_SIZE     0x100
 *  #define TEST_BASE_ADDR_ALIGN    0x4
 *  .alloc_mem TEST_BASE_ADDR       i26.emem island TEST_BASE_ADDR_SIZE TEST_BASE_ADDR_ALIGN
 *
 *  immed40(addr_hi, addr_lo, TEST_BASE_ADDR)
 *  move($wr_buf, 0x12345678)
 *  mem[write8_be, $wr_buf, addr_hi, <<8, addr_lo, 4], sig_done[s]
 *  ctx_arb[s], all
 * @endcode
 */
#macro immed40(out_result_hi, out_result_lo, VAL)
    #if (is_ct_const(VAL))

        // If (is_ct_const(VAL)) then some checking and optimization can be done
        #if (defined(__PREPROC64))
            #if ((VAL) > 0xFFFFFFFFFF)
                #error "immed40: VAL cannot exceed 0xFFFFFFFFFF"
            #endif
        #endif

        // Check if out_result_hi and out_result_lo both GPR registers
        #if (!strstr(out_result_hi, $) &&  !strstr(out_result_lo, $))
            #if ((((VAL) & 0xFFFF0000) == 0) || (((VAL) & 0xFFFF0000) == 0xFFFF0000))
                immed[out_result_lo, ((VAL) & 0xFFFFFFFF)]
            #elif (((VAL) & 0x0000FFFF) == 0)
                immed[out_result_lo, (0x0000FFFF & ((VAL) >> 16)), <<16]
            #elif (((VAL) & 0x0000FFFF) == 0x0000FFFF)
                immed[out_result_lo, (0xFFFF0000 | ((VAL) >> 16)), <<16]
            #else
                immed[out_result_lo, ((VAL) & 0xFFFF)]
                immed_w1[out_result_lo, (((VAL) >> 16) & 0xFFFF)]
            #endif
            immed[out_result_hi, (((VAL) >> 24) & 0xFF00), <<16]

        // Check if out_result_hi and out_result_lo both xfer registers
        #elif (strstr(out_result_hi, $) &&  strstr(out_result_lo, $))
            .begin
            .reg immed_tmp

            immed[immed_tmp, ((VAL) & 0xFFFF)]
            immed_w1[immed_tmp, (((VAL) >> 16) & 0xFFFF)]
            alu[out_result_lo, --, b, immed_tmp]
            immed[out_result_hi, (((VAL) >> 24) & 0xFF00), <<16]
            .end

        // Registers not same, error
        #else
            #error "out_result_hi and out_result_lo must be of same type"
        #endif

    #else   /* (is_ct_const(VAL)) */

        // If (is_rt_const(VAL)) then little checking and optimization can be done
        // VAL is run time constant when invoked like: immed40(addr_hi, addr_lo, __ADDR_I26_EMEM)

        // Check if out_result_hi and out_result_lo both GPR registers
        #if (!strstr(out_result_hi, $) &&  !strstr(out_result_lo, $))
            immed[out_result_lo, ((VAL) & 0xFFFF)]
            immed_w1[out_result_lo, (((VAL) >> 16) & 0xFFFF)]
            immed[out_result_hi, (((VAL) >> 24) & 0xFF00), <<16]

        // Check if out_result_hi and out_result_lo both xfer registers
        #elif (strstr(out_result_hi, $) &&  strstr(out_result_lo, $))
            .begin
            .reg immed_tmp

            immed[immed_tmp, ((VAL) & 0xFFFF)]
            immed_w1[immed_tmp, (((VAL) >> 16) & 0xFFFF)]
            alu[out_result_lo, --, b, immed_tmp]
            immed[out_result_hi, (((VAL) >> 24) & 0xFF00), <<16]
            .end

        // Registers not same, error
        #else
            #error "out_result_hi and out_result_lo must be of same type"
        #endif
    #endif   /* (is_ct_const(VAL)) */
#endm


/** Save the current program counter in GPR @p in_link_pc and branch to target_label.
 *
 *  Program control can be returned to the current PC (the current location) using the
 *  <code>rtn[in_link_pc]</code> instruction.
 *
 * @b Example:
 * @code
 * balr(next, target)
 * @endcode
 *
 * @param in_link_pc    GPR to hold next PC for use by rtn instruction
 * @param target_label  Subroutine start
 *
 * @instruction_cnt 2
 */
#macro balr(in_link_pc, target_label)
    br [target_label], defer[1]
    load_addr[in_link_pc, ret_lab#]
ret_lab#:
#endm


/** Move the source value into the destination register.
 *
 * @b Example:
 * @code
 * move(output, input)
 * @endcode
 *
 * @param out_result    Destination GPR or write transfer register
 * @param in_src        Source GPR, read transfer register or constant
 *
 * @instruction_cnt 1 to 3 (2 if output is GPR, 3 if output is transfer register)
 */
#macro move(out_result, in_src)
    #if (is_rt_const(in_src))
        immed32(out_result, in_src)
    #else
        alu[out_result, --, b, in_src]
    #endif
#endm


/** Perform ALU operation @p in_a @p op_spec @p in_b.
 *
 * @b Example:
 * @code
 * alu_op(output, 0x12345678, +, 10)
 * @endcode
 *
 * @param out_result    Destination GPR or write transfer register
 * @param in_a          Register or constant
 * @param op_spec       ALU operation @n @n
 *                      Legal operators are:
 *                      @arg @c B
 *                      @arg @c ~B
 *                      @arg @c +
 *                      @arg @c +carry
 *                      @arg @c +4
 *                      @arg @c +8
 *                      @arg @c +16
 *                      @arg @c -
 *                      @arg @c B-A
 *                      @arg @c AND
 *                      @arg @c ~AND
 *                      @arg @c AND~
 *                      @arg @c OR
 *                      @arg @c XOR
 * @param in_b          Register or constant
 *
 * @instruction_cnt 1 to 5 (6 if @p op_spec = +4)
 */
#macro alu_op(out_result, in_a, op_spec, in_b)
    #if ((is_ct_const(in_b) || isimport(in_b))&&(is_ct_const(in_a) || isimport(in_a))) //in_b and in_a are constant
        #if ((in_b > MAX_IMMEDIATE)||(in_b<0))
            .begin

                .reg _tempb
                immed32(_tempb, in_b)
                #if ((in_a > MAX_IMMEDIATE)||(in_a<0))
                    .begin

                        .reg _tempa
                        immed32(_tempa, in_a)
                        _nfp_alu(out_result, _tempa, op_spec, _tempb)

                    .end
                #else               // in_a < 32
                    _nfp_alu(out_result, in_a, op_spec, _tempb)
                #endif

            .end
        #else               // in_b < 32
            .begin

                .reg _tempa
                immed32(_tempa, in_a)
                _nfp_alu(out_result, _tempa, op_spec, in_b)

            .end
        #endif
    #elif (is_ct_const(in_b) || isimport(in_b))       // in_b is constant
        #if ((in_b > MAX_IMMEDIATE)||(in_b<0))
            .begin

                .reg _tempb
                immed32(_tempb, in_b)
                _nfp_alu(out_result, in_a, op_spec, _tempb)

            .end
        #else
            _nfp_alu(out_result, in_a, op_spec, in_b)
        #endif
    #elif (is_ct_const(in_a) || isimport(in_a)) //in_a is constant
        #if((in_a > MAX_IMMEDIATE)||(in_a<0))
            .begin

                .reg _tempa
                immed32(_tempa, in_a)
                _nfp_alu(out_result, _tempa, op_spec, in_b)

            .end
        #else
            _nfp_alu(out_result, in_a, op_spec, in_b)
        #endif
    #else
        _nfp_alu(out_result, in_a, op_spec, in_b)
    #endif
#endm


/** Perform 32 bit add
 *
 * This macro calculates:
 * @code
 * out_result = in_a + in_b
 * @endcode
 *
 * @b Example:
 * @code
 * add(output, 0x1234, 0x12345678)
 * @endcode
 *
 * @param out_result    Destination GPR or write transfer register
 * @param in_a          Register or constant
 * @param in_b          Register or constant
 *
 * @instruction_cnt 1 to 5
 */
#macro add(out_result, in_a, in_b)
    alu_op(out_result, in_a, +, in_b)
#endm


/** Perform 32 bit add
 *
 * This macro calculates:
 * @code
 * out_result = in_a + in_b + previous carry
 * @endcode
 *
 * @b Example:
 * @code
 * add_c(output, source_a, 0x12345678)
 * @endcode
 *
 * @param out_result    Destination GPR or write transfer register
 * @param in_a          Register or constant
 * @param in_b          Register or constant
 *
 * @instruction_cnt 1 to 5
 */
#macro add_c(out_result, in_a, in_b)
    alu_op(out_result, in_a, +carry, in_b)
#endm


/** Perform 32 bit subtract
 *
 * This macro calculates:
 * @code
 * out_result = in_a - in_b
 * @endcode
 *
 * @b Example:
 * @code
 * sub(output, 0x12345678, source)
 * @endcode
 *
 * @param out_result    Destination GPR or write transfer register
 * @param in_a          Register or constant
 * @param in_b          Register or constant
 *
 * @instruction_cnt 1 to 5
 */
#macro sub(out_result, in_a, in_b)
    alu_op(out_result, in_a, -, in_b)
#endm


/** Shift right @p in_src by @p in_shift_amt bit positions.
 *
 * @b Example:
 * @code
 * shf_right(output, input, 3)
 * @endcode
 *
 * @param out_result    Destination
 * @param in_src        Source
 * @param in_shift_amt  Register or constant (0 to 31)
 *
 * @instruction_cnt 1 to 2
 */
#macro shf_right(out_result, in_src, in_shift_amt)
    #ifdef _TMP_RESULT
        #warning "_TMP_RESULT is being redefined."
    #endif

    #if (!isrestricted(out_result))
        .begin
            .reg tmp_result_reg
            #define_eval _TMP_RESULT tmp_result_reg
    #else
        #define_eval _TMP_RESULT out_result
    #endif

    #if (is_ct_const(in_src) || isimport(in_src))
        #if ((in_src > MAX_IMMEDIATE)||(in_src<0))
            #define _use_temp_src
            .begin
                .reg shf_right_temp_src
                immed32(shf_right_temp_src, in_src)
        #endif
    #elif (!isrestricted(in_src))
            #define _use_temp_src
            .begin
                .reg shf_right_temp_src
                alu[shf_right_temp_src, --, b, in_src]
    #endif

    #if (is_ct_const(in_shift_amt) || isimport(in_shift_amt))
        #ifdef SHF_AMT
            #warning "SHF_AMT is being redefined."
        #endif
        #define_eval SHF_AMT in_shift_amt
        #ifdef _use_temp_src
                alu[_TMP_RESULT, --, B, shf_right_temp_src, >>/**/SHF_AMT/**/]
            .end    //shf_right_temp_src
            #undef _use_temp_src
        #else
            alu[_TMP_RESULT, --, B, in_src, >>/**/SHF_AMT/**/]
        #endif
        #undef SHF_AMT
    #else
        alu[--, in_shift_amt, OR, 0]
        #ifdef _use_temp_src
                alu[_TMP_RESULT, --, B, shf_right_temp_src, >>indirect]

            .end    //shf_right_temp_src
            #undef _use_temp_src
        #else
            alu[_TMP_RESULT, --, B, in_src, >>indirect]
        #endif
    #endif

    #if (!isrestricted(out_result))
            alu[out_result, --, b, _TMP_RESULT]
        .end // tmp_result_reg
    #endif

    #undef _TMP_RESULT
#endm


/** Shift left @p in_src by @p in_shift_amt bit positions.
 *
 * @b Example:
 * @code
 * shf_left(output, input, reg)
 * @endcode
 *
 * @param out_result    Destination
 * @param in_src        Source
 * @param in_shift_amt  Register or constant (0 to 31)
 *
 * @instruction_cnt 1 to 4
 */
#macro shf_left(out_result, in_src, in_shift_amt)
    #ifdef _TMP_RESULT
        #warning "_TMP_RESULT is being redefined."
    #endif

    #if (!isrestricted(out_result))
        .begin
            .reg tmp_result_reg
            #define_eval _TMP_RESULT tmp_result_reg
    #else
        #define_eval _TMP_RESULT out_result
    #endif

    #if (is_ct_const(in_src) || isimport(in_src))
        #if ((in_src > MAX_IMMEDIATE)||(in_src<0))
            #define _use_temp_src
            .begin
                .reg shf_left_temp_src
                immed32(shf_left_temp_src, in_src)
        #endif
    #elif (!isrestricted(in_src))
            #define _use_temp_src
            .begin
                .reg shf_left_temp_src
                alu[shf_left_temp_src, --, b, in_src]
    #endif

    #if (is_ct_const(in_shift_amt) || isimport(in_shift_amt))
        #ifdef SHF_AMT
            #warning "SHF_AMT is being redefined."
        #endif
        #define_eval SHF_AMT in_shift_amt
        #ifdef _use_temp_src
                alu[_TMP_RESULT, --, B, shf_left_temp_src, <</**/SHF_AMT/**/]
            .end    //shf_left_temp_src
            #undef _use_temp_src
        #else
            alu[_TMP_RESULT, --, B, in_src, <</**/SHF_AMT/**/]
        #endif
        #undef SHF_AMT
    #else
        alu[--, in_shift_amt, OR, 0]
        #ifdef _use_temp_src
                alu[_TMP_RESULT, --, B, shf_left_temp_src, <<indirect]

            .end    //shf_left_temp_src
            #undef _use_temp_src
        #else
            alu[_TMP_RESULT, --, B, in_src, <<indirect]
        #endif
    #endif

    #if (!isrestricted(out_result))
            alu[out_result, --, b, _TMP_RESULT]
        .end // tmp_result_reg
    #endif

    #undef _TMP_RESULT
#endm


/** Rotate right @p in_src by @p in_shift_amt bit positions.
 *
 * @b Example:
 * @code
 * rot_right(output, input, 3)
 * @endcode
 *
 * @param out_result    Destination GPR
 * @param in_src        Source GPR
 * @param in_shift_amt  Register or constant (0 to 31)
 *
 * @instruction_cnt 1
 */
#macro rot_right(out_result, in_src, in_shift_amt)
    #ifdef SHF_AMT
        #warning "SHF_AMT is being redefined."
    #endif
    #define_eval SHF_AMT in_shift_amt
        alu[out_result, --, B, in_src, >>rot /**/SHF_AMT/**/]
    #undef SHF_AMT
#endm


/** Rotate left @p in_src by @p in_shift_amt bit positions.
 *
 * @b Example:
 * @code
 * rot_left(output, input, 3)
 * @endcode
 *
 * @param out_result    Destination GPR
 * @param in_src        Source GPR
 * @param in_shift_amt  Register or constant (0 to 31)
 *
 * @instruction_cnt 1
 */
#macro rot_left(out_result, in_src, in_shift_amt)
    #ifdef SHF_AMT
        #warning "SHF_AMT is being redefined."
    #endif
    #define_eval SHF_AMT in_shift_amt
        alu[out_result, --, B, in_src, <<rot /**/SHF_AMT/**/]
    #undef SHF_AMT
#endm


/** Shift right @p in_b by @p in_shift_amt bit positions, then perform operation
 *  @p op_spec on @p in_a.
 *
 * @b Example:
 * @code
 * alu_shf_right(output, 0xABCD1234, +, input, 3)
 * @endcode
 *
 * @param out_result    Destination
 * @param in_a          Register or constant
 * @param op_spec       ALU operation @n @n
 *                      Legal operators are:
 *                      @arg @c B
 *                      @arg @c ~B
 *                      @arg @c +
 *                      @arg @c +carry
 *                      @arg @c +4
 *                      @arg @c +8
 *                      @arg @c +16
 *                      @arg @c -
 *                      @arg @c -carry
 *                      @arg @c B-A
 *                      @arg @c AND
 *                      @arg @c ~AND
 *                      @arg @c AND~
 *                      @arg @c OR
 *                      @arg @c XOR
 * @param in_b          Register or constant
 * @param in_shift_amt  Register or constant (0 to 31)
 *
 * @instruction_cnt 1 to 4
 */
#macro alu_shf_right(out_result, in_a, op_spec, in_b, in_shift_amt)

    #if (!isrestricted(out_result))
        .begin
            .reg tmp_result_reg
            #define_eval _TMP_RESULT tmp_result_reg
    #else
        #define_eval _TMP_RESULT out_result
    #endif

    #define_eval _TMP_A in_a
    #if (is_ct_const(in_a) || isimport(in_a))
        #if ((in_a>MAX_IMMEDIATE)||(in_a<0))
            #define _use_temp_a
            .begin
                .reg _tempa
                move(_tempa, in_a) // should not effect carry in case of +-carry ops
                #undef _TMP_A
                #define_eval _TMP_A _tempa
        #endif
    #elif (!isrestricted(in_a))
        #define _use_temp_a
        .begin
            .reg _tempa
            #if ( streq ('op_spec', '+carry')) // do the carry op here
                alu[_tempa, in_a, +carry, 0]
                #define _carry_done
            #elif ( streq ('op_spec', '-carry'))
                alu[_tempa, in_a, -carry, 0]
                #define _carry_done
            #else
                move(_tempa, in_a)
            #endif
            #undef _TMP_A
            #define_eval _TMP_A _tempa
    #endif

    #define_eval _TMP_B in_b
    #if (is_ct_const(in_b) || isimport(in_b))
        #if ((in_b>MAX_IMMEDIATE)||(in_b<0))
            #define _use_temp_b
            .begin
                .reg _tempb
                move(_tempb, in_b)
                #undef _TMP_B
                #define_eval _TMP_B _tempb
        #endif
    #elif (!isrestricted(in_b))
        #define _use_temp_b
        .begin
            .reg _tempb
            #if ( streq ('op_spec', '+carry') && !defined(_carry_done))
                alu[_tempb, in_b, +carry, 0]
                #define _carry_done
            #elif ( streq ('op_spec', '-carry') && !defined( _carry_done))
                alu[_tempb, in_b, -carry, 0]
                #define _carry_done
            #else
                move(_tempb, in_b)
            #endif
            #undef _TMP_B
            #define_eval _TMP_B _tempb
    #endif

    #if (is_ct_const(in_shift_amt) || isimport(in_shift_amt))
        #ifdef SHF_AMT
            #warning "SHF_AMT is being redefined."
        #endif
        #define_eval SHF_AMT in_shift_amt
        #if ( streq ('op_spec', '+carry') && defined(_carry_done) )
            _nfp_alu_shf(_TMP_RESULT, _TMP_A, +, _TMP_B, >>/**/SHF_AMT/**/)
        #elif ( streq ('op_spec', '-carry') && defined(_carry_done) )
            _nfp_alu_shf(_TMP_RESULT, _TMP_A, -, _TMP_B, >>/**/SHF_AMT/**/)
        #else
            _nfp_alu_shf(_TMP_RESULT, _TMP_A, op_spec, _TMP_B, >>/**/SHF_AMT/**/)
        #endif
        #undef SHF_AMT
    #else       // not is_ct_const in_shift_amt
        alu[--, in_shift_amt, OR, 0]
        #if ( streq ('op_spec', '+carry') && defined(_carry_done) )
            _nfp_alu_shf(_TMP_RESULT, _TMP_A, +, _TMP_B, >>indirect)
        #elif ( streq ('op_spec', '-carry') && defined(_carry_done) )
            _nfp_alu_shf(_TMP_RESULT, _TMP_A, -, _TMP_B, >>indirect)
        #else
            _nfp_alu_shf(_TMP_RESULT, _TMP_A, op_spec, _TMP_B, >>indirect)
        #endif
    #endif

    #ifdef _carry_done
        #undef _carry_done
    #endif

    #ifdef _use_temp_b
        .end //temp_b
        #undef _use_temp_b
    #endif

    #ifdef _use_temp_a
        .end //temp_a
        #undef _use_temp_a
    #endif

    #if (!isrestricted(out_result))
            alu[out_result, --, b, _TMP_RESULT]
        .end // tmp_result_reg
    #endif

    #undef _TMP_RESULT
    #undef _TMP_A
    #undef _TMP_B
#endm


/** Shift right @p in_b by @p in_shift_amt bit positions, then ADD @p in_a.
 *
 * @b Example:
 * @code
 * add_shf_right(output, 0xABCD1234, input, 3)
 * @endcode
 *
 * @param out_result    Destination GPR
 * @param in_a          Register or constant
 * @param in_b          Register or constant
 * @param in_shift_amt  Register or constant (0 to 31)
 *
 * @instruction_cnt 1 to 4
 */
#macro add_shf_right(out_result, in_a, in_b, in_shift_amt)
    alu_shf_right(out_result, in_a, +, in_b, in_shift_amt)
#endm


/** Shift right @p in_b by @p in_shift_amt bit positions, then SUBTRACT @p in_a.
 *
 * @b Example:
 * @code
 * sub_shf_right(output, 0xABCD1234, input, 3)
 * @endcode
 *
 * @param out_result    Destination GPR
 * @param in_a          Register or constant
 * @param in_b          Register or constant
 * @param in_shift_amt  Register or constant (0 to 31)
 *
 * @instruction_cnt 1 to 4
 */
#macro sub_shf_right(out_result, in_a, in_b, in_shift_amt)
    alu_shf_right(out_result, in_a, -, in_b, in_shift_amt)
#endm


/** Shift right @p in_b by @p in_shift_amt bit positions, then AND @p in_a.
 *
 * @b Example:
 * @code
 * and_shf_right(output, 0xABCD1234, input, 3)
 * @endcode
 *
 * @param out_result    Destination GPR
 * @param in_a          Register or constant
 * @param in_b          Register or constant
 * @param in_shift_amt  Register or constant (0 to 31)
 *
 * @instruction_cnt 1 to 4
 */
#macro and_shf_right(out_result, in_a, in_b, in_shift_amt)
    alu_shf_right(out_result, in_a, AND, in_b, in_shift_amt)
#endm


/** Shift right @p in_b by @p in_shift_amt bit positions, then OR @p in_a.
 *
 * @b Example:
 * @code
 * or_shf_right(output, 0xABCD1234, input, 3)
 * @endcode
 *
 * @param out_result    Destination GPR
 * @param in_a          Register or constant
 * @param in_b          Register or constant
 * @param in_shift_amt  Register or constant (0 to 31)
 *
 * @instruction_cnt 1 to 4
 */
#macro or_shf_right(out_result, in_a, in_b, in_shift_amt)
    alu_shf_right(out_result, in_a, OR, in_b, in_shift_amt)
#endm


/** Shift left @p in_b by @p in_shift_amt bit positions, then perform operation
 *  @p op_spec on @p in_a.
 *
 * @b Example:
 * @code
 * alu_shf_left(output, 0xABCD1234, +, input, 3)
 * @endcode
 *
 * @param out_result    Destination
 * @param in_a          Register or constant
 * @param op_spec       ALU operation @n @n
 *                      Legal operators are:
 *                      @arg @c B
 *                      @arg @c ~B
 *                      @arg @c +
 *                      @arg @c +carry
 *                      @arg @c +4
 *                      @arg @c +8
 *                      @arg @c +16
 *                      @arg @c -
 *                      @arg @c -carry
 *                      @arg @c B-A
 *                      @arg @c AND
 *                      @arg @c ~AND
 *                      @arg @c AND~
 *                      @arg @c OR
 *                      @arg @c XOR
 * @param in_b          Register or constant
 * @param in_shift_amt  Register or constant (0 to 31)
 *
 * @instruction_cnt 1 to 4
 */
#macro alu_shf_left(out_result, in_a, op_spec, in_b, in_shift_amt)
    #if (!isrestricted(out_result))
        .begin
            .reg tmp_result_reg
            #define_eval _TMP_RESULT tmp_result_reg
    #else
        #define_eval _TMP_RESULT out_result
    #endif

    #define_eval _TMP_A in_a
    #if (is_ct_const(in_a) || isimport(in_a))
        #if ((in_a>MAX_IMMEDIATE)||(in_a<0))
            #define _use_temp_a
            .begin
                .reg _tempa
                move(_tempa, in_a) // should not effect carry in case of +-carry ops
                #undef _TMP_A
                #define_eval _TMP_A _tempa
        #endif
    #elif (!isrestricted(in_a))
        #define _use_temp_a
        .begin
            .reg _tempa
            #if ( streq ('op_spec', '+carry')) // do the carry op here
                alu[_tempa, in_a, +carry, 0]
                #define _carry_done
            #elif ( streq ('op_spec', '-carry'))
                alu[_tempa, in_a, -carry, 0]
                #define _carry_done
            #else
                move(_tempa, in_a)
            #endif
            #undef _TMP_A
            #define_eval _TMP_A _tempa
    #endif

    #define_eval _TMP_B in_b
    #if (is_ct_const(in_b) || isimport(in_b))
        #if ((in_b>MAX_IMMEDIATE)||(in_b<0))
            #define _use_temp_b
            .begin
                .reg _tempb
                move(_tempb, in_b)
                #undef _TMP_B
                #define_eval _TMP_B _tempb
        #endif
    #elif (!isrestricted(in_b))
        #define _use_temp_b
        .begin
            .reg _tempb
            #if ( streq ('op_spec', '+carry') && !defined(_carry_done))
                alu[_tempb, in_b, +carry, 0]
                #define _carry_done
            #elif ( streq ('op_spec', '-carry') && !defined( _carry_done))
                alu[_tempb, in_b, -carry, 0]
                #define _carry_done
            #else
                move(_tempb, in_b)
            #endif
            #undef _TMP_B
            #define_eval _TMP_B _tempb
    #endif

    #if (is_ct_const(in_shift_amt) || isimport(in_shift_amt))
        #ifdef SHF_AMT
            #warning "SHF_AMT is being redefined."
        #endif
        #define_eval SHF_AMT in_shift_amt
        #if ( streq ('op_spec', '+carry') && defined(_carry_done) )
            _nfp_alu_shf(_TMP_RESULT, _TMP_A, +, _TMP_B, <</**/SHF_AMT/**/)
        #elif ( streq ('op_spec', '-carry') && defined(_carry_done) )
            _nfp_alu_shf(_TMP_RESULT, _TMP_A, -, _TMP_B, <</**/SHF_AMT/**/)
        #else
            _nfp_alu_shf(_TMP_RESULT, _TMP_A, op_spec, _TMP_B, <</**/SHF_AMT/**/)
        #endif
        #undef SHF_AMT
    #else       // not is_ct_const in_shift_amt
        alu[--, in_shift_amt, OR, 0]
        #if ( streq ('op_spec', '+carry') && defined(_carry_done) )
            _nfp_alu_shf(_TMP_RESULT, _TMP_A, +, _TMP_B, <<indirect)
        #elif ( streq ('op_spec', '-carry') && defined(_carry_done) )
            _nfp_alu_shf(_TMP_RESULT, _TMP_A, -, _TMP_B, <<indirect)
        #else
            _nfp_alu_shf(_TMP_RESULT, _TMP_A, op_spec, _TMP_B, <<indirect)
        #endif
    #endif

    #ifdef _carry_done
        #undef _carry_done
    #endif

    #ifdef _use_temp_b
        .end //temp_b
        #undef _use_temp_b
    #endif

    #ifdef _use_temp_a
        .end //temp_a
        #undef _use_temp_a
    #endif

    #if (!isrestricted(out_result))
            alu[out_result, --, b, _TMP_RESULT]
        .end // tmp_result_reg
    #endif

    #undef _TMP_RESULT
    #undef _TMP_A
    #undef _TMP_B
#endm


/** Shift left @p in_b by @p in_shift_amt bit positions, then ADD @p in_a.
 *
 * @b Example:
 * @code
 * add_shf_left(output, 0xABCD1234, input, 3)
 * @endcode
 *
 * @param out_result    Destination GPR
 * @param in_a          Register or constant
 * @param in_b          Register or constant
 * @param in_shift_amt  Register or constant (0 to 31)
 *
 * @instruction_cnt 1 to 4
 */
#macro add_shf_left(out_result, in_a, in_b, in_shift_amt)
    alu_shf_left(out_result, in_a, +, in_b, in_shift_amt)
#endm


/** Shift left @p in_b by @p in_shift_amt bit positions, then SUBTRACT @p in_a.
 *
 * @b Example:
 * @code
 * sub_shf_left(output, 0xABCD1234, input, 3)
 * @endcode
 *
 * @param out_result    Destination GPR
 * @param in_a          Register or constant
 * @param in_b          Register or constant
 * @param in_shift_amt  Register or constant (0 to 31)
 *
 * @instruction_cnt 1 to 4
 */
#macro sub_shf_left(out_result, in_a, in_b, in_shift_amt)
    alu_shf_left(out_result, in_a, -, in_b, in_shift_amt)
#endm


/** Shift left @p in_b by @p in_shift_amt bit positions, then AND @p in_a.
 *
 * @b Example:
 * @code
 * and_shf_left(output, 0xABCD1234, input, 3)
 * @endcode
 *
 * @param out_result    Destination GPR
 * @param in_a          Register or constant
 * @param in_b          Register or constant
 * @param in_shift_amt  Register or constant (0 to 31)
 *
 * @instruction_cnt 1 to 4
 */
#macro and_shf_left(out_result, in_a, in_b, in_shift_amt)
    alu_shf_left(out_result, in_a, AND, in_b, in_shift_amt)
#endm


/** Shift left @p in_b by @p in_shift_amt bit positions, then OR @p in_a.
 *
 * @b Example:
 * @code
 * or_shf_left(output, 0xABCD1234, input, 3)
 * @endcode
 *
 * @param out_result    Destination GPR
 * @param in_a          Register or constant
 * @param in_b          Register or constant
 * @param in_shift_amt  Register or constant (0 to 31)
 *
 * @instruction_cnt 1 to 4
 */
#macro or_shf_left(out_result, in_a, in_b, in_shift_amt)
    alu_shf_left(out_result, in_a, OR, in_b, in_shift_amt)
#endm


/** Rotate right @p in_b by @p in_shift_amt bit positions, then perform operation
 *  @p op_spec on @p in_a.
 *
 * @b Example:
 * @code
 * alu_rot_right(output, input2, +, input, 4)
 * @endcode
 * When: @p input2 = 5 and @p input = 0x12345678 results in @p out_result = 0x8123456c
 *
 * @param out_result    Destination GPR
 * @param in_a          Register or constant
 * @param op_spec       ALU operation @n @n
 *                      Legal operators are:
 *                      @arg @c B
 *                      @arg @c ~B
 *                      @arg @c +
 *                      @arg @c +carry
 *                      @arg @c +4
 *                      @arg @c +8
 *                      @arg @c +16
 *                      @arg @c -
 *                      @arg @c B-A
 *                      @arg @c AND
 *                      @arg @c ~AND
 *                      @arg @c AND~
 *                      @arg @c OR
 *                      @arg @c XOR
 * @param in_b          Register or constant
 * @param in_shift_amt  Register or constant (0 to 31)
 *
 * @instruction_cnt 1 to 3 (4 if @p op_spec = +4)
 */
#macro alu_rot_right(out_result, in_a, op_spec, in_b, in_shift_amt)
    #ifdef SHF_AMT
        #warning "SHF_AMT is being redefined."
    #endif
    #define_eval SHF_AMT in_shift_amt
        #if (is_ct_const(in_a) || isimport(in_a))
            #if ((in_a > MAX_IMMEDIATE)||(in_a<0))
                .begin

                    .reg _tempa
                    immed32(_tempa, in_a)
                    _nfp_alu_shf(out_result, _tempa, op_spec, in_b, >>rot /**/SHF_AMT/**/)

                .end
            #else
                _nfp_alu_shf(out_result, in_a, op_spec, in_b, >>rot /**/SHF_AMT/**/)
            #endif
        #else
            _nfp_alu_shf(out_result, in_a, op_spec, in_b, >>rot /**/SHF_AMT/**/)
        #endif
    #undef SHF_AMT
#endm


/** Rotate left @p in_b by @p in_shift_amt bit positions, then perform operation
 *  @p op_spec on @p in_a.
 *
 * @b Example:
 * @code
 * alu_rot_left(output, input2, +, input, 3)
 * @endcode
 *
 * @param out_result    Destination GPR
 * @param in_a          Register or constant
 * @param op_spec       ALU operation @n @n
 *                      Legal operators are:
 *                      @arg @c B
 *                      @arg @c ~B
 *                      @arg @c +
 *                      @arg @c +carry
 *                      @arg @c +4
 *                      @arg @c +8
 *                      @arg @c +16
 *                      @arg @c -
 *                      @arg @c B-A
 *                      @arg @c AND
 *                      @arg @c ~AND
 *                      @arg @c AND~
 *                      @arg @c OR
 *                      @arg @c XOR
 * @param in_b          Register or constant
 * @param in_shift_amt  Register or constant (0 to 31)
 *
 * @instruction_cnt 1 to 3 (4 if @p op_spec = +4)
 */
#macro alu_rot_left(out_result, in_a, op_spec, in_b, in_shift_amt)
    #ifdef SHF_AMT
        #warning "SHF_AMT is being redefined."
    #endif
    #define_eval SHF_AMT in_shift_amt
        #if (is_ct_const(in_a) || isimport(in_a))
            #if ((in_a > MAX_IMMEDIATE)||(in_a<0))
                .begin

                    .reg _tempa
                    immed32(_tempa, in_a)
                    _nfp_alu_shf(out_result, _tempa, op_spec, in_b, <<rot /**/SHF_AMT/**/)

                .end
            #else
                _nfp_alu_shf(out_result, in_a, op_spec, in_b, <<rot /**/SHF_AMT/**/)
            #endif
        #else
            _nfp_alu_shf(out_result, in_a, op_spec, in_b, <<rot /**/SHF_AMT/**/)
        #endif
    #undef SHF_AMT
#endm


/** Extract a bit field from a register.
 *
 * @param out_result    Extracted field
 * @param in_src        Source register with multiple fields
 * @param MSB           Most significant, left bit defining field
 * @param LSB           Least significant, right bit defining field
 *
 * @note  Bits are numbered 31-0, left to right.
 *
 * @instruction_cnt 1 to 6
 */
#macro bitfield_extract(out_result, in_src, MSB, LSB)
    #ifdef _SHF_LEFT
        #warning "_SHF_LEFT is being redefined."
    #endif
    #ifdef _SHF_RIGHT
        #warning "_SHF_RIGHT is being redefined."
    #endif

    #define_eval _SHF_LEFT (31 - MSB)
    #define_eval _SHF_RIGHT (_SHF_LEFT + LSB)

    #if (MSB == 31)
        shf_right(out_result, in_src, LSB)
    #elif ((MSB - LSB) == 15)
        alu_shf_right(out_result, 0, +16, in_src, LSB)
    #else
        _calc_bitfield_mask(MSB, LSB)
        #if !( _BITFIELD_MASK & ~MAX_IMMEDIATE )
            alu_shf_right(out_result, _BITFIELD_MASK, AND, in_src, LSB)
        #else
            shf_left(out_result, in_src, _SHF_LEFT)
            shf_right(out_result, out_result, _SHF_RIGHT)
        #endif
        #undef _BITFIELD_MASK
    #endif

    #undef _SHF_LEFT
    #undef _SHF_RIGHT
#endm


/// @cond INTERNAL_MACROS
/** Internal macro to get a mask.
 *
 * @param MSB           Most significant, left bit (bits left to right are 31-0)
 * @param LSB           Least significant, right bit
 *
 */
#macro _calc_bitfield_mask(MSB, LSB)
    #ifdef _NUM_BITS
        #warning "_NUM_BITS is being redefined."
    #endif
    #ifdef _BITFIELD_MASK
        #warning "_BITFIELD_MASK is being redefined."
    #endif

    #define_eval _NUM_BITS (MSB - LSB + 1)

    #define_eval _BITFIELD_MASK (_NUM_BITS > 31 ? 0xffffffff : ((1 << _NUM_BITS) - 1))

    #undef _NUM_BITS
#endm
/// @endcond


/** Insert bitfield @p in_b into copy of @p in_a.
 *
 * @param out_result    Merged result
 * @param in_a          Register with multiple fields
 * @param in_b          Bits to be inserted (This field always start at bit 0. Do not
 *                      shift to the position to be inserted.)
 * @param MSB           Most significant, left bit (bits left to right are 31-0)
 * @param LSB           Least significant, right bit
 *
 * @note  Bits are numbered 31-0, left to right.
 *
 * @instruction_cnt 2 to 4 (depending on size of bitfield, 2 if mask fits in immediate)
 */
#macro bitfield_insert(out_result, in_a, in_b, MSB, LSB)
.begin

    .reg temp_merged

    // define _BITFIELD_MASK
    _calc_bitfield_mask(MSB, LSB)


    #if (_BITFIELD_MASK & ~MAX_IMMEDIATE)
        .begin

            .reg mask
            immed32(mask, _BITFIELD_MASK)
            alu_shf_left(temp_merged, in_a, AND~, mask, LSB)

        .end
    #else
        alu_shf_left(temp_merged, in_a, AND~, _BITFIELD_MASK, LSB)
    #endif

    #undef _BITFIELD_MASK

    alu_shf_left(out_result, temp_merged, OR, in_b, LSB)

.end
#endm


/** Clear bits indicated by mask at starting position @p in_start_bit_pos.
 *
 * @b Example:
 * @code
 * bits_clr(reg2, bitpos, 0x3)
 * @endcode
 *
 * @param io_data           Register to modify
 * @param in_start_bit_pos  Register or constant less than 32, starting bit position
 *                          @p in_mask is shifted left by this amount
 * @param in_mask           Register or constant, mask of bits to clear
 *
 * @instruction_cnt 4 to 7
 */
#macro bits_clr(io_data, in_start_bit_pos, in_mask)
.begin

    #if is_ct_const(in_mask)
        #if (in_mask & ~MAX_IMMEDIATE)
            .reg const_mask
            immed32(const_mask, in_mask)
            alu_shf_left(io_data, io_data, AND~, const_mask, in_start_bit_pos)
        #else
            alu_shf_left(io_data, io_data, AND~, in_mask, in_start_bit_pos)
        #endif
    #else
        alu_shf_left(io_data, io_data, AND~, in_mask, in_start_bit_pos)
    #endif

.end
#endm


/** Set bits indicated by mask at starting position @p in_start_bit_pos.
 *
 * @b Example:
 * @code
 * bits_set(reg2, 5, 0xff)
 * @endcode
 *
 * @param io_data           Register to modify
 * @param in_start_bit_pos  Register or constant less than 32, starting bit position
 *                          @p in_mask is shifted left by this amount
 * @param in_mask           Register or constant, mask of bits to set
 *
 * @instruction_cnt 3 to 5
 */
#macro bits_set(io_data, in_start_bit_pos, in_mask)
    alu_shf_left(io_data, io_data, OR, in_mask, in_start_bit_pos)
#endm


/** 32-bit multiplication with 16x16 operand size.
 *
 * This macro calculates:
 * @code
 * out_result = in_a * in_b
 * @endcode
 *
 * @param out_result    Destination GPR
 * @param in_a          Multiplicand, GPR
 * @param in_b          Multiplier, any 32-bit input constant
 *
 * @note @p in_b is known at compile time, so the pre-processor uses its
 *       bit value to construct an optimal multiply via ALU/shift operations.
 *       A simple implementation would take every 1-bit position n in @p in_b,
 *       and shift and add @p mpy_in by n to accumulate the @p out_result. So if there were
 *       B 1-bits in @p in_b, this would take B instructions -- up to 32.
 *       This implementation also uses subtraction, reducing the maximum number
 *       of instructions to 16. For example given @p in_b = 123 = 1111011b
 *       123x = 64x + 32x + 16x + 8x + 2x + 1x  but also: 123x = 128x - 4x - 1x @n
 *       @n
 * @note @b Performance: @n
 *       n cycles: 1 to 16 @n
 *       Worst case performance is the number of 1 bits in @p in_b. 0xxAAAAAAAA
 *       and 0x55555555 take the maximum 16 cycles.
 *       Actual performance depends on @p in_b like so:
 *       -# Spans of two or more 1's cost 2 cycles. @n@n
 *          1a) The exception is if there is only 1 span of 1's and it is adjacent
 *              to bit 31 -- this costs 1 cycle.
 *       -# Isolated 0's or 1's cost 1 cycle each.
 * @note @b Resources: @n
 *      @arg Memory: no accesses, none consumed
 *      @arg Registers: no additional registers, just out_result & input parameters.
 *      @arg Instructions = n: (n <= 16), as described Performance.
 * @note @b Limitations: @n
 *      @p mpy_in and @p out_result typically cannot be the same GPR.
 *      This is because @ref add_shf_left and @ref sub_shf_left macros
 *      will operate on both @p out_result and @p mpy_in in the same cycle
 *      and would result in a compile-time error. However, @p mpy_in and
 *      @p out_result can be the same GPR in cases where the macro expands to
 *      just a single instruction. For example, if @p in_b is zero or a
 *      positive or negative even power of two. @n
 *      @n
 * @note @b Algorithm: @n
 *       Start scanning the @p in_b from one end looking for the first 1-bit.
 *       If it is a single 1-bit, emit an add_shift and continue. Otherwise,
 *       start counting 1-bits until you find two or more 0-bits in a row.
 *       Then add the first of the two zero bits, subtract the first 1-bit,
 *       and subtract any single 0-bits along the way.
 *
 * @condition_codes Data dependent: assume they are clobbered.
 * @instruction_cnt 3 to 4
 */
#macro multiply(out_result, in_a, in_b)
    //  multiply maps to multiply32 with 16x16 operand size
    multiply32(out_result, in_a, in_b, OP_SIZE_16X16)
#endm // multiply


/** Divide.
 *
 * This macro calculates:
 * @code
 * out_result = dividend / DIVISOR
 * @endcode
 *
 * @param out_result    Destination GPR
 * @param dividend      GPR or CONSTANT
 * @param DIVISOR       GPR or CONSTANT @n @n
 *                      For variable dividend DIVISOR must be constant power of 2
 *                      For CONSTANT dividend DIVISOR can be any CONSTANT
 *
 * @note @b Performance: @n
 *       Variable/CONSTANT: 1 cycle
 *                    CONSTANT/CONSTANT: 1 - 2 cycles  @n
 *       @n
 * @note @b Resources: @n
 *       Memory: no accesses, none consumed. @n
 *       Registers: no additional registers, just @p out_result and input parameters. @n
 *       Instructions: 1 or 2 @n
 *       @n
 * @note @b Limitations: @n
 *       Power of 2 constant DIVISOR @n
 *       @n
 * @note @b Algorithm: @n
 *       Determine what DIV_SHIFT yields (2^DIV_SHIFT == DIVISOR). @n
 *       Right shift divisor by DIV_SHIFT.
 *
 * @condition_codes Data dependent: assume they are clobbered.
 * @instruction_cnt 1 to 2
 */
#macro divide(out_result, dividend, DIVISOR)
    #if (!(is_ct_const(DIVISOR) || isimport(DIVISOR)))
        #error 4 "divide(out, dividend, DIVISOR) requires CONSTANT power of 2 DIVISOR."
    #elif (DIVISOR == 0)
        #error 4 "divide(out, dividend, DIVISOR): divide by 0"
    #elif (is_ct_const(dividend) || isimport(dividend))
        // Both dividend and DIVISOR are constant -- go for it
        immed32(out_result, ((dividend) / (DIVISOR)))   // pre-processor divide
    #else
        // dividend is variable and DIVISOR is constant

        // Find what DIV_SHIFT gives (2^DIV_SHIFT == DIVISOR)
        #ifdef DIV_SHIFT
            #warning "DIV_SHIFT is being redefined."
        #endif
        #ifdef DIV_TWO2SHIFT
            #warning "DIV_TWO2SHIFT is being redefined."
        #endif
        #define_eval DIV_SHIFT 0
        #define_eval DIV_TWO2SHIFT (1 << DIV_SHIFT)
        #while((DIV_SHIFT < 32) && (DIVISOR != DIV_TWO2SHIFT))
            #define_eval DIV_SHIFT (DIV_SHIFT + 1)
            #define_eval DIV_TWO2SHIFT (1 << DIV_SHIFT)
        #endloop

        #if (DIV_TWO2SHIFT == DIVISOR)
            shf_right(out_result, dividend, DIV_SHIFT)
        #else
            #error 1 "divide(out, dividend, DIVISOR): DIVISOR must be power of 2."
        #endif

        #undef DIV_SHIFT            // clean up name space
        #undef DIV_TWO2SHIFT        // clean up name space
    #endif // dividend is variable and DIVISOR is constant
#endm // divide


/** Convert an address into an array index.
 *
 * This macro calculates:
 * @code
 * out_index = (in_address / ELEMENT_SIZE) - (BASE_ADDRESS / ELEMENT_SIZE)
 * @endcode
 *
 * @param out_index     Index of the array element
 * @param in_address    Array element's address
 * @param BASE_ADDRESS  Memory Address of 1st element in the array
 * @param ELEMENT_SIZE  Size of Array Element in address units
 *
 * @note @b Limitations: @n
 *       Attempts to compile for non power of 2 @p ELEMENT_SIZE will fail
 *                    because divide does not currently support non power of 2 divisors.
 *
 * @instruction_cnt 3 (Depends on @p ELEMENT_SIZE and @p BASE_ADDRESS)
 */
#macro array_index_from_elem_addr(out_index, in_address, BASE_ADDRESS, ELEMENT_SIZE)
.begin

    // Find what IFA_SHIFT gives (2^IFA_SHIFT == ELEMENT_SIZE)
    #ifdef IFA_SHIFT
        #warning "IFA_SHIFT is being redefined."
    #endif
    #ifdef IFA_TWO2SHIFT
        #warning "IFA_TWO2SHIFT is being redefined."
    #endif
    #define_eval IFA_SHIFT 0
    #define_eval IFA_TWO2SHIFT (1 << IFA_SHIFT)
    #while((IFA_SHIFT < 32) && (ELEMENT_SIZE != IFA_TWO2SHIFT))
        #define_eval IFA_SHIFT (IFA_SHIFT + 1)
        #define_eval IFA_TWO2SHIFT (1 << IFA_SHIFT)
    #endloop

    // if ELEMENT_SIZE is a power of two
    #if (IFA_TWO2SHIFT == ELEMENT_SIZE)
        #ifdef _TMP_IN_ADDRESS
            #warning "_TMP_IN_ADDRESS is being redefined"
        #endif

        #if (is_ct_const(in_address) || isimport(in_address))
            .reg tmp_in_address
            move(tmp_in_address, in_address)
            #define_eval _TMP_IN_ADDRESS    tmp_in_address
        #else
            #define_eval _TMP_IN_ADDRESS    in_address
        #endif

        #if (is_ct_const(BASE_ADDRESS) || isimport(BASE_ADDRESS))
            #define_eval BASE_BY_ELEMENT ((BASE_ADDRESS) / (ELEMENT_SIZE))
        #else
            .reg base_by_element_reg

            alu_shf[base_by_element_reg, --, B, BASE_ADDRESS, >>/**/IFA_SHIFT/**/]
            #define BASE_BY_ELEMENT base_by_element_reg
        #endif

        alu_shf_right(out_index, BASE_BY_ELEMENT, b-a, _TMP_IN_ADDRESS, IFA_SHIFT)

        #undef BASE_BY_ELEMENT  // cleanup name space

        #ifdef _TMP_IN_ADDRESS
            #undef _TMP_IN_ADDRESS  // cleanup name space
        #endif
    #else   // ELEMENT_SIZE is NOT power of two

        // This is what we want to do, but
        // divide will not compile for non power of 2.
        sub(out_index, in_address, BASE_ADDRESS)
        divide(out_index, out_index, ELEMENT_SIZE)
    #endif

    #undef IFA_SHIFT            // clean up name space
    #undef IFA_TWO2SHIFT        // clean up name space

.end
#endm


/** Convert an array element address to an array index.
 *
 * This macro calculates:
 * @code
 * if (ELEMENT_SIZE is power of 2)
 *  out_address = (in_index << BUF_DAFI_SHIFT) + BASE_ADDRESS
 * else
 *  out_address = (in_index * ELEMENT_SIZE) + BASE_ADDRESS
 * @endcode
 *
 * @param out_address   Address of the array element
 * @param in_index      Index of the array element
 * @param BASE_ADDRESS  Memory Address of 1st element in the array
 * @param ELEMENT_SIZE  Size of Array Element in address units
 *
 * @instruction_cnt 1 to 21 (Depends on @p ELEMENT_SIZE and @p BASE_ADDRESS).
 *                  Best: 1 = 1 ALU. Typical: 3 = 2 immeds + 1 ALU. Worst: 21 = 4 immeds + 17 ALU
 */
#macro elem_addr_from_array_index(out_address, in_index, BASE_ADDRESS, ELEMENT_SIZE)
    #if (is_ct_const(ELEMENT_SIZE) || isimport(ELEMENT_SIZE))
        // Find what AFI_SHIFT gives (2^AFI_SHIFT == ELEMENT_SIZE)
        #ifdef AFI_SHIFT
            #warning "AFI_SHIFT is being redefined."
        #endif
        #ifdef AFI_TWO2SHIFT
            #warning "AFI_TWO2SHIFT is being redefined."
        #endif
        #define_eval AFI_SHIFT 0
        #define_eval AFI_TWO2SHIFT (1 << AFI_SHIFT)
        #while((AFI_SHIFT < 32) && (ELEMENT_SIZE != AFI_TWO2SHIFT))
            #define_eval AFI_SHIFT (AFI_SHIFT + 1)
            #define_eval AFI_TWO2SHIFT (1 << AFI_SHIFT)
        #endloop

        // if ELEMENT_SIZE is a power of two
        #if (AFI_TWO2SHIFT == ELEMENT_SIZE)
            add_shf_left(out_address, BASE_ADDRESS, in_index, AFI_SHIFT)
        #else   // else ELEMENT_SIZE NOT a power of two
            multiply(out_address, in_index, ELEMENT_SIZE)
            add(out_address, out_address, BASE_ADDRESS)
        #endif

        #undef AFI_SHIFT            // clean up name space
        #undef AFI_TWO2SHIFT        // clean up name space
    #else
        multiply(out_address, in_index, ELEMENT_SIZE)
        add(out_address, out_address, BASE_ADDRESS)
    #endif  // is_ct_const(ELEMENT_SIZE)
#endm


/** Shift right depend on the following cases: @n
 *  -# If @p SIGN_BIT_POS is set to 31, no sign extend will occur, just arithmetic shift right.
 *  -# If @p in_shift_amt is a constant 0, just sign extend will occur.
 *  -# If both @p in_shift_amt is non-zero and @p SIGN_BIT_POS is not 31, Sign extend from
 *     @p SIGN_BIT_POS left to bit 31. Then shift right, filling with sign b.
 *
 * @param out_result    Sign-extended shifted-right result
 * @param in_src        Source operand.
 * @param in_shift_amt  Register of constant right shift amount, values from 0-31 but cannot
 *                      be greater than SIGN_BIT_POS value
 * @param SIGN_BIT_POS  Constant position of sign bit. 31 if @p in_src is already sign-extended.
 *
 * @instruction_cnt 2 to 8
 */
#macro arith_shf_right(out_result, in_src, in_shift_amt, SIGN_BIT_POS)
    #ifdef _SHF_AMT
        #warning "_SHF_AMT is being redefined."
    #endif
    #define_eval _SHF_AMT (31 - SIGN_BIT_POS)

    alu_shf_left(out_result, 0, OR, in_src, _SHF_AMT)
    #if (is_ct_const(in_shift_amt) || isimport(in_shift_amt))
        #ifdef _TOTAL_SHF_AMT
            #warning "_TOTAL_SHF_AMT is being redefined."
        #endif
        #define_eval _TOTAL_SHF_AMT (_SHF_AMT + in_shift_amt)
        asr[out_result, out_result, >>/**/_TOTAL_SHF_AMT/**/]
        #undef _TOTAL_SHF_AMT
    #else
        #if (_SHF_AMT > 0)
            asr[out_result, out_result, >>/**/_SHF_AMT/**/]
        #endif

        // Preserve MSB for asr instruction by testing the SIGN_BIT_POS
        //  of in_src
        br_bclr[in_src, SIGN_BIT_POS, _arith_shf_right_continue_#]
        nop
        alu_shf[in_shift_amt, in_shift_amt, OR, 1, <<31]

    _arith_shf_right_continue_#:

        alu[--, in_shift_amt, OR, 0]
        asr[out_result, out_result, >>indirect]
    #endif
    #undef _SHF_AMT
#endm


/** 32 bit multiply optimized for @p OPERAND_SIZE specifications.
 *
 * This macro calculates:
 * @code
 * out_result = in_a * in_b
 * @endcode
 *
 * @param out_result    32 bit multiply result
 * @param in_a          Multiplicand
 * @param in_b          Multiplier
 * @param OPERAND_SIZE  Operand size @n @n
 *                      Valid sizes: @n
 *                      @arg @c OP_SIZE_8X24:   Multiplier is 8 bits, multiplicand is 24 bits (8x24)
 *                      @arg @c OP_SIZE_16X16:  Multiplier and multiplicand are 16 bits (16x16)
 *
 * @instruction_cnt 3 to 4 (3 for OP_SIZE_8x24 and 4 for OP_SIZE_16x16)
 */
#macro multiply32(out_result, in_a, in_b, OPERAND_SIZE)
    #if (OPERAND_SIZE == OP_SIZE_8X24)
        mul_step[in_a, in_b], 24x8_start
        mul_step[in_a, in_b], 24x8_step1
        mul_step[out_result, --], 24x8_last
    #elif (OPERAND_SIZE == OP_SIZE_16X16)
        mul_step[in_a, in_b], 16x16_start
        mul_step[in_a, in_b], 16x16_step1
        mul_step[in_a, in_b], 16x16_step2
        mul_step[out_result, --], 16x16_last
    #else
        #error 4 "multiply32(). illegal OPERAND_SIZE. valid sizes are OP_SIZE_8X24, OP_SIZE_16X16"
    #endif
#endm


/** 64 bit multiply optimized for @p OPERAND_SIZE specifications.
 *
 * This macro calculates:
 * @code
 * out_result = in_a * in_b
 * @endcode
 *
 * @param out_result_hi Most significant 32 bits of multiply result
 * @param out_result_lo Least significant 32 bits of multiply result
 * @param in_a          Multiplicand
 * @param in_b          Multiplier
 * @param OPERAND_SIZE  Operand size @n @n
 *                      Valid sizes: @n
 *                      @arg @c OP_SIZE_16X32:  Multiplier is 16 bits, multiplicand is 32 bits
 *                      @arg @c OP_SIZE_32X32:  Multiplier and multiplicand are 32 bits
 *
 * @instruction_cnt 5 to 7 (5 for OP_SIZE_16x32 and 7 for OP_SIZE_32x32)
 */
#macro multiply64(out_result_hi, out_result_lo, in_a, in_b, OPERAND_SIZE)
    #if (OPERAND_SIZE == OP_SIZE_16X32)
        #error 4 "multiply64(). OPERAND_SIZE 16X32 not yet supported"
        mul_step[in_a, in_b], 32x16_start
        mul_step[in_a, in_b], 32x16_step1
        mul_step[in_a, in_b], 32x16_step2
        mul_step[out_result_lo, --], 32x16_last
        mul_step[out_result_hi, --], 32x16_last2

    #elif (OPERAND_SIZE == OP_SIZE_32X32)
        mul_step[in_a, in_b], 32x32_start
        mul_step[in_a, in_b], 32x32_step1
        mul_step[in_a, in_b], 32x32_step2
        mul_step[in_a, in_b], 32x32_step3
        mul_step[in_a, in_b], 32x32_step4
        mul_step[out_result_lo, --], 32x32_last
        mul_step[out_result_hi, --], 32x32_last2
    #else
        #error 4 "multiply64(). illegal OPERAND_SIZE. valid sizes are OP_SIZE_16X32, OP_SIZE_32X32"
    #endif
#endm


/** Generate a pseudo-random number
 *
 * @param out_result    Random number
 *
 * @instruction_cnt 2
 */
#macro rand(out_result)
    local_csr_rd[PSEUDO_RANDOM_NUMBER]
    immed[out_result, 0]
#endm


/** Set pseudo-random number seed
 *
 * @param in_src    Random number seed, constant or register
 *
 * @instruction_cnt 3 to 5
 */
#macro srand(in_src)
.begin

    .reg srand_temp
    #if (is_ct_const (in_src) || isimport(in_src))
        immed32(srand_temp, in_src)
        local_csr_wr[PSEUDO_RANDOM_NUMBER, srand_temp]
    #else
        local_csr_wr[PSEUDO_RANDOM_NUMBER, in_src]
    #endif
    nop
    nop
    nop
    nop
    nop
    // Update the CSR so that subsequent reads will get
    //  random numbers
    local_csr_rd[PSEUDO_RANDOM_NUMBER]
    immed[srand_temp, 0]

.end
#endm


/** @}
 * @}
 */

#endif /* __STDMAC_UC__ */
