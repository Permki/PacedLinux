/*
 * Copyright (C) 2006-2015 Netronome Systems, Inc.  All rights reserved.
 *
 */

#ifndef _BITFIELDS_UC_
#define _BITFIELDS_UC_

#ifndef NFP_LIB_ANY_NFAS_VERSION
    #if (!defined(__NFP_TOOL_NFAS) ||                       \
        (__NFP_TOOL_NFAS < NFP_SW_VERSION(5, 0, 0, 0)) ||   \
        (__NFP_TOOL_NFAS > NFP_SW_VERSION(6, 1, 0, 255)))
        #error "This standard library is not supported for the version of the SDK currently in use."
    #endif
#endif

#include <passert.uc>

/** @file bitfields.uc Bitfield Operation Macros
 * @addtogroup bitfields Bitfield Operations
 * @{
 *
 * @name Bitfield Operation Macros
 * @{
 *
 * These macros perform bitfield operations using minimum number of instructions
 */

// API Quick Reference:
//
// alu_shf_left__sz1(out_result, in_a, op_spec, in_b, in_shift_amt)
// alu_shf_right__sz1(out_result, in_a, op_spec, in_b, in_shift_amt)
// bits_clr__sz1(io_data, in_start_bit_pos, in_mask)
// bits_set__sz1(io_data, in_start_bit_pos, in_mask)
// bitfield_extract__sz1(out_result, in_src, MSB, LSB)
// bitfield_clear__sz1(io_data, MSB, LSB)
// bitfield_insert__sz2(io_data, MSB, LSB, in_mask)
// bitfield_or__sz1(io_data, LSB, in_a)
// bitfield_copy__sz2(out_reg, OUT_LSB, in_reg, IN_MSB, IN_LSB)

#define BF_AML(a, w, m, l) a[w], m, l    // Returns aggregate[word], msb, lsb - useful for bitfield_extract()
#define BF_AL(a, w, m, l) a[w], l        // Returns aggregate[word], lsb - useful for br_bset[] and bits_clr/set()
#define BF_A(a, w, m, l) a[w]            // Returns aggregate[word] - useful to access the aggregate
#define BF_ML(w, m, l) m, l              // Returns msb, lsb - useful for bitfield_extract()
#define BF_W(w, m, l) w                  // Returns word offset into aggregate
#define BF_M(w, m, l) m                  // Returns msb
#define BF_L(w, m, l) l                  // Returns lsb - useful for alu_shf[]

#define BF_WIDTH(w, m, l) (m + 1 - l)    // Width of bitfield

// Retained for backwards compatibility

#define _bitfield_insert bitfield_insert__sz2
#define _bitfield_clear  bitfield_clear__sz1
#define _bitfield_or     bitfield_or__sz1
#define _bitfield_copy   bitfield_copy__sz2

#define F_AML BF_AML
#define F_AL  BF_AL
#define F_A   BF_A
#define F_ML  BF_ML
#define F_W   BF_W
#define F_M   BF_M
#define F_L   BF_L



/** Shift left @p in_b by @p in_shift_amt bit positions, then perform operation
 *  @p op_spec on @p in_a.
 *
 * @b Example:
 * @code
 * .reg output, input
 * immed[input, 0x05]
 * alu_shf_left__sz1(output, 0x02, OR, input, 4)
 * @endcode
 *
 * @param out_result    Destination
 * @param in_a          Register or constant
 * @param op_spec       ALU operation @n @n
 *                      Legal operators are:
 *                      @arg @c B
 *                      @arg @c ~B
 *                      @arg @c AND
 *                      @arg @c ~AND
 *                      @arg @c AND~
 *                      @arg @c OR
 * @param in_b          Register or constant
 * @param in_shift_amt  Constant (0 to 31)
 *
 * Limited to restricted operands and single 8 bit constant parameter or 2 GPRs
 * Use alu_shf_left() instead for reduced limitations
 *
 * @instruction_cnt 1
 */
#macro alu_shf_left__sz1(out_result, in_a, op_spec, in_b, in_shift_amt)

    #ifdef SHF_AMT
        #warning "SHF_AMT is being redefined."
    #endif
    #define_eval SHF_AMT in_shift_amt

    alu[out_result, in_a, op_spec, in_b, <<SHF_AMT]

    #undef SHF_AMT

#endm


/** Shift right @p in_b by @p in_shift_amt bit positions, then perform operation
 *  @p op_spec on @p in_a.
 *
 * @b Example:
 * @code
 * .reg output, input
 * immed[input, 0x50]
 * alu_shf_right__sz1(output, 0x20, OR, input, 4)
 * @endcode
 *
 * @param out_result    Destination
 * @param in_a          Register or constant
 * @param op_spec       ALU operation @n @n
 *                      Legal operators are:
 *                      @arg @c B
 *                      @arg @c ~B
 *                      @arg @c AND
 *                      @arg @c ~AND
 *                      @arg @c AND~
 *                      @arg @c OR
 * @param in_b          Register or constant
 * @param in_shift_amt  Constant (0 to 31)
 *
 * Limited to restricted operands and single 8 bit constant parameter or 2 GPRs
 * Use alu_shf_left() instead for more flexibility
 *
 * @instruction_cnt 1
 */
#macro alu_shf_right__sz1(out_result, in_a, op_spec, in_b, in_shift_amt)

    #ifdef SHF_AMT
        #warning "SHF_AMT is being redefined."
    #endif
    #define_eval SHF_AMT in_shift_amt

    alu[out_result, in_a, op_spec, in_b, >>SHF_AMT]

    #undef SHF_AMT

#endm


/** Clear bits indicated by mask at starting position @p in_start_bit_pos.
 *
 * @b Example:
 * @code
 * .reg reg2, bitpos
 * bits_clr__sz1(reg2, bitpos, 0x3)
 * @endcode
 *
 * @param io_data           Register to modify
 * @param in_start_bit_pos  Constant less than 32, starting bit position
 *                          @p in_mask is shifted left by this amount
 * @param in_mask           Register or constant, mask of bits to clear
 *
 * Limited to restricted operands, use bits_clr() instead for more flexibility
 *
 * @instruction_cnt 1
 */
#macro bits_clr__sz1(io_data, in_start_bit_pos, in_mask)

    alu_shf_left__sz1(io_data, io_data, AND~, in_mask, in_start_bit_pos)

#endm


/** Set bits indicated by mask at starting position @p in_start_bit_pos.
 *
 * @b Example:
 * @code
 * .reg reg2, bitpos
 * bits_set__sz1(reg2, bitpos, 0x3)
 * @endcode
 *
 * @param io_data           Register to modify
 * @param in_start_bit_pos  Constant less than 32, starting bit position
 *                          @p in_mask is shifted left by this amount
 * @param in_mask           Register or constant, mask of bits to set
 *
 * Limited to restricted operands, use bits_set() instead for more flexibility
 *
 * @instruction_cnt 1
 */
#macro bits_set__sz1(io_data, in_start_bit_pos, in_mask)

    alu_shf_left__sz1(io_data, io_data, OR, in_mask, in_start_bit_pos)

#endm


/** Extract a bit field from a register.
 *
 * @b Example:
 * @code
 * .reg r_out, r_in
 * bitfield_extract__sz1(r_out, r_in, 15, 8)
 * @endcode
 *
 * @param out_result    Extracted field
 * @param in_src        Source register with multiple fields
 * @param MSB           Most significant, left bit defining field
 * @param LSB           Least significant, right bit defining field
 *
 * @note  Bits are numbered 31-0, left to right.

 * Limited to restricted operands in some cases, as well as certain
 * combinations of MSB/LSB. Use bitfield_extract() instead for more flexibility
 *
 * @instruction_cnt 1
 */
#macro bitfield_extract__sz1(out_result, in_src, MSB, LSB)

    passert(MSB, "IS_IN_RANGE", 0, 31)
    passert(LSB, "IS_IN_RANGE", 0, 31)
    passert(MSB, "GE", LSB)

    #ifdef _BITFIELD_WIDTH
        #error "_BITFIELD_WIDTH is already defined"
    #endif

    #define_eval _BITFIELD_WIDTH (MSB - LSB + 1)

    // Support unrestricted operands where possible
    // Do not use ld_field if another option is available to support LM
    #if (_BITFIELD_WIDTH == 32)

        alu[out_result, --, B, in_src]

    #elif (LSB == 0)

        #if (_BITFIELD_WIDTH <= 8)

            alu[out_result, ((1 << _BITFIELD_WIDTH) - 1), AND, in_src]

        #elif (_BITFIELD_WIDTH == 16)

            alu[out_result, 0, +16, in_src]

        #elif (_BITFIELD_WIDTH >= 24)

            alu_shf_left__sz1(out_result, in_src, AND~, 0xff, (MSB + 1))

        #else

            #error "Field width not supported" MSB LSB

        #endif

    #elif (MSB == 31)

        alu_shf_right__sz1(out_result, --, B, in_src, LSB)

    #elif (_BITFIELD_WIDTH <= 8)

        alu_shf_right__sz1(out_result, ((1 << _BITFIELD_WIDTH) - 1), AND, in_src, LSB)

    #elif (_BITFIELD_WIDTH == 16)

        ld_field_w_clr[out_result, 0011, in_src, >>LSB], load_cc

    #elif (_BITFIELD_WIDTH == 24)

        ld_field_w_clr[out_result, 0111, in_src, >>LSB], load_cc

    #else

        #error "Field width not supported" MSB LSB

    #endif

    #undef _BITFIELD_WIDTH

#endm // bitfield_extract__sz1()


/** Clear a bit field in a register.
 *
 * @b Example:
 * @code
 * .reg r_val
 * bitfield_clear__sz1(r_val, 15, 8)
 * @endcode
 *
 * @param io_data  Register to clear
 * @param MSB      Constant, most significant, left bit defining field
 * @param LSB      Constant, least significant, right bit defining field
 *
 * @note  Bits are numbered 31-0, left to right.
 *
 * Limited to restricted operands. Use bits_clr instead for more flexibility
 *
 * @instruction_cnt 1
 */
#macro bitfield_clear__sz1(io_data, MSB, LSB)
.begin

    bits_clr__sz1(io_data, LSB, ((1 << (MSB - LSB + 1)) - 1))

.end
#endm


/** Clear a bit field in a register, then inserts in_mask
 *
 * @b Example:
 * @code
 * .reg r_val
 * bitfield_insert__sz2(r_val, 15, 8, 0x23)
 * @endcode
 *
 * @param io_data  Register to insert value
 * @param MSB      Constant, most significant, left bit defining field
 * @param LSB      Constant, least significant, right bit defining field
 * @param in_mask  Register or constant, bit mask to insert
 * @note  Bits are numbered 31-0, left to right.
 *
 * Limited to restricted operands. Use bitfield_insert instead for more flexibility
 * If in_mask is a run-time constant, ensures that the value of in_mask is not wider than (MSB - LSB + 1) bits
 *
 * @instruction_cnt 2
 */
#macro bitfield_insert__sz2(io_data, MSB, LSB, in_mask)
.begin
    bitfield_clear__sz1(io_data, MSB, LSB)

    #if (is_rt_const(in_mask))
        _calc_bitfield_mask(MSB, LSB)       // _BITFIELD_MASK is defined in _calc_bitfield_mask
        passert((in_mask & ~_BITFIELD_MASK), "EQ", 0)
        #undef _BITFIELD_MASK
    #endif

    bits_set__sz1(io_data, LSB, in_mask)
.end
#endm


/** Retain deprecated aliases for backwards compatibility, but with compiler
 * warnings.
 */
#macro bitfield_insert__sz1(io_data, MSB, LSB, in_mask)

    #warning "Macro deprecated (misnamed as it creates 2 instructions), use bitfield_insert__sz2 instead."
    bitfield_insert__sz2(io_data, MSB, LSB, in_mask)

#endm



/** Logical Or bits in an input field into a register
 *
 * @b Example:
 * @code
 * .reg r_val
 * bitfield_or__sz1(r_val, 8, 0x23)
 * @endcode
 *
 * @param io_data  Register in which to logical or in_mask
 * @param LSB      Constant, least significant, right bit defining field
 * @param in_mask  Register or constant, bit mask to logical or into io_data
 *
 * @note  Bits are numbered 31-0, left to right.
 *
 * Limited to restricted operands. Use bitfield_insert instead for more flexibility
 *
 * @instruction_cnt 1
 */
#macro bitfield_or__sz1(io_data, LSB, in_mask)

    bits_set__sz1(io_data, LSB, in_mask)

#endm


/** Logical Or a bitfield from one register into another register
 *
 * @b Example:
 * @code
 * .reg r_val
 * bitfield_copy__sz2(r_val, 8, 0x23, 15, 0)
 * @endcode
 *
 * @param out_reg  Register in which to logical or the bitfield from in_reg
 * @param OUT_LSB  Constant, least significant, right bit defining field in out_reg
 * @param in_reg   Register containing bit field to extract and insert into out_reg
 * @param IN_MSB   Constant, most significant, left bit defining field in in_reg
 * @param IN_LSB   Constant, least significant, right bit defining field in in_reg
 *
 * @note  Bits are numbered 31-0, left to right.
 *
 * Limited to restricted operands.
 *
 * @instruction_cnt 2
 */
#macro bitfield_copy__sz2(out_reg, OUT_LSB, in_reg, IN_MSB, IN_LSB)
.begin

    .reg tmp
    bitfield_extract__sz1(tmp, in_reg, IN_MSB, IN_LSB)
    bitfield_or__sz1(out_reg, OUT_LSB, tmp)

.end
#endm

/** Retain deprecated aliases for backwards compatibility, but with compiler
 * warnings.
 */
#macro bitfield_copy__sz1(out_reg, OUT_LSB, in_reg, IN_MSB, IN_LSB)

    #warning "Macro deprecated (misnamed as it creates 2 instructions), use bitfield_copy__sz2 instead."
    bitfield_copy__sz2(out_reg, OUT_LSB, in_reg, IN_MSB, IN_LSB)

#endm


/** @}
 * @}
 */

#endif // _BITFIELDS_UC_
