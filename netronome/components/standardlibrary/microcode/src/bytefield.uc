/*
 * Copyright (C) 2008-2011 Netronome Systems, Inc.  All rights reserved.
 */

#ifndef __BYTEFIELD_UC__
#define __BYTEFIELD_UC__

#ifndef NFP_LIB_ANY_NFAS_VERSION
    #if (!defined(__NFP_TOOL_NFAS) ||                       \
        (__NFP_TOOL_NFAS < NFP_SW_VERSION(5, 0, 0, 0)) ||   \
        (__NFP_TOOL_NFAS > NFP_SW_VERSION(6, 1, 0, 255)))
        #error "This standard library is not supported for the version of the SDK currently in use."
    #endif
#endif

#include <endian.uc>

/** @file bytefield.uc Byte Field Manipulation Macros
 * @addtogroup byte_field_manip Byte Field Manipulation
 * @{
 *
 * @name Big / Little-endian Byte Field Extract / Compare / Branch Macros
 * @{
 *
 * Default is BIG-ENDIAN. By defining LITTLE_ENDIAN, the appropriate
 * underlying swaps will be inserted.
 * This will allow users to write code that runs big- or little-endian.
 *
 */

// API Quick Reference:
//
// bytefield_dbl_extract(out_result,  in_a, in_start_byte, in_b, in_end_byte, in_load_cc)
// bytefield_decr(out_result, in_src, in_start_byte, in_end_byte)
// bytefield_extract(out_result, in_src, in_start_byte, in_end_byte, in_load_cc)
// bytefield_incr(out_result, in_src, in_start_byte, in_end_byte)
// bytefield_insert(io_a, in_byte_mask, in_b, in_load_cc)
// bytefield_select(out_result, in_byte_mask, in_src, in_load_cc)
// bytefield_br_eq(in_src, in_start_byte, in_end_byte, COMPARE_VAL, target_label)
// bytefield_br_gtr(in_src, in_start_byte, in_end_byte, COMPARE_VAL, target_label)
// bytefield_br_less(in_src, in_start_byte, in_end_byte, COMPARE_VAL, target_label)
// bytefield_br_gtreq(in_src, in_start_byte, in_end_byte, COMPARE_VAL, target_label)
// bytefield_br_lesseq(in_src, in_start_byte, in_end_byte, COMPARE_VAL, target_label)
// bytefield_comp(in_src, in_start_byte, in_end_byte, COMPARE_VAL)
// bytefield_shf_left_insert(io_a, in_byte_mask, in_b, in_shift_amt, in_load_cc)
// bytefield_shf_right_insert(io_a, in_byte_mask, in_b, in_shift_amt, in_load_cc)
// bytefield_clr_insert(out_result, in_byte_mask, in_b, in_load_cc)
// bytefield_clr_shf_left_insert(out_result, in_byte_mask, in_b, in_shift_amt, in_load_cc)
// bytefield_clr_shf_right_insert(out_result, in_byte_mask, in_b, in_shift_amt, in_load_Cc)



/** Decrement byte field from @p in_src and place the result into @p out_result.
 *
 * @b Example:
 * @code
 * bytefield_decr(reg, $xfer[0], 1, 2) // decrement 2 byte fields
 * @endcode
 *
 * @param out_result    GPR or write transfer register
 * @param in_src        GPR or read transfer reg containing byte field. If GPR, it must
 *                      be on the opposite bank as @p out_result.
 * @param in_start_byte Starting byte position 0-3 based on endian mode
 * @param in_end_byte   Ending byte position 0-3 based on endian mode. Can be equal to
 *                      or greater than @p in_start_byte.
 *
 * @instruction_cnt 2 to 4
 */
#macro bytefield_decr(out_result, in_src, in_start_byte, in_end_byte)
#ifdef SBYTE
    #warning "SBYTE is being redefined."
#endif
#ifdef EBYTE
    #warning "EBYTE is being redefined."
#endif
#define_eval SBYTE in_start_byte
#define_eval EBYTE in_end_byte

#if (in_start_byte == in_end_byte)
    decr/**/SBYTE(out_result, in_src)
#else
    decr/**/SBYTE/**/EBYTE(out_result, in_src)
#endif
#undef SBYTE
#undef EBYTE
#endm


/** Increment byte field from @p in_src and place the result into @p out_result.
 *
 * @b Example:
 * @code
 * bytefield_incr(reg, $xfer[0], 1, 3) // increment 3 byte fields
 * @endcode
 *
 * @param out_result    GPR or write transfer register
 * @param in_src        GPR or read transfer reg containing byte field. If GPR, it must
 *                      be on the opposite bank as @p out_result.
 * @param in_start_byte Starting byte position 0-3 based on endian mode
 * @param in_end_byte   Ending byte position 0-3 based on endian mode. Can be equal to
 *                      or greater than @p in_start_byte.
 *
 * @instruction_cnt 2 to 4
 */
#macro bytefield_incr(out_result, in_src, in_start_byte, in_end_byte)
#ifdef SBYTE
    #warning "SBYTE is being redefined."
#endif
#ifdef EBYTE
    #warning "EBYTE is being redefined."
#endif
#define_eval SBYTE in_start_byte
#define_eval EBYTE in_end_byte

#if (in_start_byte == in_end_byte)
    incr/**/SBYTE(out_result, in_src)
#else
    incr/**/SBYTE/**/EBYTE(out_result, in_src)
#endif
#undef SBYTE
#undef EBYTE
#endm


/** Extract byte field from @p in_src and place the result into @p out_result.
 *
 * @b Example:
 * @code
 * bytefield_extract(reg, $xfer[0], 1, 3, DO_LOAD_CC)  // extract 3 byte fields
 * @endcode
 *
 * @param out_result    GPR
 * @param in_src        GPR or read transfer reg containing byte field. If GPR, it must
 *                      be on the opposite bank as @p out_result.
 * @param in_start_byte Starting byte position 0-3 based on endian mode
 * @param in_end_byte   Ending byte position 0-3 based on endian mode. Must be greater
 *                      than @p in_start_byte.
 * @param  in_load_cc   CONSTANT to specify whether user wants the load ALU condition codes
 *                      based on the result performed @n@n
 *                      Possible values are: @n
 *                      @arg @c NO_LOAD_CC:  Do not load condition code - suggested value if users
 *                                            only want to select byte(s) without any side effect
 *                      @arg @c DO_LOAD_CC:   Load condition code
 *
 * @instruction_cnt 1 to 3
 */
#macro bytefield_extract(out_result, in_src, in_start_byte, in_end_byte, in_load_cc)
#ifdef SBYTE
    #warning "SBYTE is being redefined."
#endif
#ifdef EBYTE
    #warning "EBYTE is being redefined."
#endif
#define_eval SBYTE in_start_byte
#define_eval EBYTE in_end_byte

#if (SBYTE == EBYTE)
    extract/**/SBYTE(out_result, in_src, 0, in_load_cc)
#else
    extract/**/SBYTE/**/EBYTE(out_result, in_src, 0, in_load_cc)
#endif
#undef SBYTE
#undef EBYTE
#endm


/** Extract a field that spans two input registers.
 *
 * @b Example:
 * @code
 * bytefield_dbl_extract(reg, $xfer[0], 3, $xfer[1], 1, DO_LOAD_CC) // extract 3 byte fields
 * @endcode
 *
 * @param out_result    GPR
 * @param in_a          GPR or read transfer reg, left source longword. If GPR, it must
 *                      be on the opposite bank as @p out_result.
 * @param in_start_byte Starting byte position 0-3 of in_a based on endian mode
 * @param in_b          GPR or read transfer register, right source longword.
 *                      If GPR, it must be on the opposite bank as @p out_result.
 * @param in_end_byte   Ending byte position 0-3 of in_b based on endian mode. Must be less
 *                      than @p in_start_byte.
 * @param in_load_cc    CONSTANT to specify whether user wants the load ALU condition codes
 *                      based on the result performed. @n@n
 *                      Possible values are: @n
 *                      @arg @c NO_LOAD_CC:  Do not load condition code - suggested value if users
 *                                            only want to select byte(s) without any side effect
 *                      @arg @c DO_LOAD_CC:   Load condition code
 *
 * @instruction_cnt 2 to 4
 */
#macro bytefield_dbl_extract(out_result, in_a, in_start_byte, in_b, in_end_byte, in_load_cc)
#ifdef SBYTE
    #warning "SBYTE is being redefined."
#endif
#ifdef EBYTE
    #warning "EBYTE is being redefined."
#endif
#ifdef _BYTEFIELD_SHIFT
    #warning "_BYTEFIELD_SHIFT is being redefined."
#endif
#define_eval SBYTE in_start_byte
#define_eval EBYTE in_end_byte
#define_eval _BYTEFIELD_SHIFT (8+in_end_byte*8)

#if (SBYTE == 3)
    extract/**/SBYTE(out_result, in_a, _BYTEFIELD_SHIFT, in_load_cc)
#else
    extract/**/SBYTE/**/3(out_result, in_a, _BYTEFIELD_SHIFT, in_load_cc)
#endif
#if (EBYTE == 0)
    merge_extract/**/EBYTE(out_result, in_b, in_load_cc)
#else
    merge_extract0/**/EBYTE(out_result, in_b, in_load_cc)
#endif

#undef SBYTE
#undef EBYTE
#undef _BYTEFIELD_SHIFT
#endm


/** Insert bytes from @p in_b specified by @p in_byte_mask into @p io_a.
 *
 * The source and destination registers must have the same endianness.
 *
 * @b Example:
 * @code
 * bytefield_insert(x, 0110, y, DO_LOAD_CC)   // insert y bytes 1,2 into x bytes 1,2
 * @endcode
 *
 * @param io_a          GPR
 * @param in_byte_mask  xxxx, where x = 0 or 1. If 1, insert byte.
 * @param in_b          GPR or read transfer register. If GPR, it must be on the
 *                      opposite bank as @p io_a.
 * @param in_load_cc    CONSTANT to specify whether user wants the load ALU condition codes
 *                      based on the result performed. @n@n
 *                      Possible values are:
 *                      @arg @c NO_LOAD_CC:  Do not load condition code - suggested value if users
 *                                           only want to select byte(s) without any side effect
 *                      @arg @c DO_LOAD_CC:   Load condition code
 *
 * @instruction_cnt 1 to 3
 */
#macro bytefield_insert(io_a, in_byte_mask, in_b, in_load_cc)
    _DEF_LOAD_CC_TOKEN(in_load_cc)

    ld_field[io_a, in_byte_mask, in_b]/**/_LOAD_CC_TOKEN
#endm


/** Insert bytes from @p in_src specified by @p in_byte_mask into @p out_result.
 *
 * The output register @p out_result is cleared prior to the insert.
 *
 * The source and destination registers must have the same endianness.
 *
 * @b Example:
 * @code
 * bytefield_select(x, 0110, y, DO_LOAD_CC) // output bytes 1-2
 * @endcode
 *
 * @param out_result    GPR or write transfer register
 * @param in_byte_mask  xxxx, where x = 0 or 1. If 1, insert byte.
 * @param in_src        GPR or read transfer register. If both @p out_result and @p in_src are GPR,
 *                      @p in_src must be on the opposite bank as @p out_result.
 * @param in_load_cc    CONSTANT to specify whether user wants the load ALU condition codes
 *                      based on the result performed @n@n
 *                      Possible values are: @n
 *                      @arg @c NO_LOAD_CC:  Do not load condition code - suggested value if users
 *                                            only want to select byte(s) without any side effect
 *                      @arg @c DO_LOAD_CC:   Load condition code
 *
 * @instruction_cnt 1 to 3
 */
#macro bytefield_select(out_result, in_byte_mask, in_src, in_load_cc)
    _DEF_LOAD_CC_TOKEN(in_load_cc)
    ld_field_w_clr[out_result, in_byte_mask, in_src]/**/_LOAD_CC_TOKEN
#endm


/** Compare byte field to a constant and branch if equal.
 *
 * @b Example:
 * @code
 * bytefield_br_eq(test_reg, 0, 2, 0xff, exception_handler#)
 * @endcode
 *
 * @param in_src        GPR or read transfer reg containing byte field
 * @param in_start_byte Starting byte position 0-3 based on endian mode
 * @param in_end_byte   Ending byte position 0-3 based on endian mode. Must be greater than @p in_start byte.
 * @param COMPARE_VAL   Constant value to compare field with
 * @param target_label  Label to branch to
 *
 * @instruction_cnt 2 to 4
 */
#macro bytefield_br_eq(in_src, in_start_byte, in_end_byte, COMPARE_VAL, target_label)
    comp_field(in_src, in_start_byte, in_end_byte, COMPARE_VAL)
    beq[target_label]
#endm


/** Compare byte field to a constant and branch if field is greater.
 *
 * @b Example:
 * @code
 * bytefield_br_gtr(test_reg, 0, 2, 0xff, exception_handler#)
 * @endcode
 *
 * @param in_src        GPR or read transfer reg containing byte field
 * @param in_start_byte Starting byte position 0-3 based on endian mode
 * @param in_end_byte   Ending byte position 0-3 based on endian mode. Must be greater than @p in_start byte.
 * @param COMPARE_VAL   Constant value to compare field with
 * @param target_label  Label to branch to
 *
 * @instruction_cnt 2 to 4
 */
#macro bytefield_br_gtr(in_src, in_start_byte, in_end_byte, COMPARE_VAL, target_label)
    comp_field(in_src, in_start_byte, in_end_byte, COMPARE_VAL)
    bgt[target_label]
#endm


/** Compare byte field to a constant and branch if field is greater or equal.
 *
 * @b Example:
 * @code
 * bytefield_br_gtreq(test_reg, 0, 2, 0xff, exception_handler#)
 * @endcode
 *
 * @param in_src        GPR or read transfer reg containing byte field
 * @param in_start_byte Starting byte position 0-3 based on endian mode
 * @param in_end_byte   Ending byte position 0-3 based on endian mode. Must be greater than @p in_start byte.
 * @param COMPARE_VAL   Constant value to compare field with
 * @param target_label  Label to branch to
 *
 * @instruction_cnt 2 to 4
 */
#macro bytefield_br_gtreq(in_src, in_start_byte, in_end_byte, COMPARE_VAL, target_label)
    comp_field(in_src, in_start_byte, in_end_byte, COMPARE_VAL)
    bpl[target_label]
#endm


/** Compare byte field to a constant and branch if field is less.
 *
 * @b Example:
 * @code
 * bytefield_br_less(test_reg, 0, 2, 0xff, exception_handler#)
 * @endcode
 *
 * @param in_src        GPR or read transfer reg containing byte field
 * @param in_start_byte Starting byte position 0-3 based on endian mode
 * @param in_end_byte   Ending byte position 0-3 based on endian mode. Must be greater than @p in_start byte.
 * @param COMPARE_VAL   Constant value to compare field with
 * @param target_label  Label to branch to
 *
 * @instruction_cnt 2 to 4
 */
#macro bytefield_br_less(in_src, in_start_byte, in_end_byte, COMPARE_VAL, target_label)
    comp_field(in_src, in_start_byte, in_end_byte, COMPARE_VAL)
    bmi[target_label]
#endm


/** Compare byte field to a constant and branch if field is less or equal.
 *
 * @b Example:
 * @code
 * bytefield_br_lesseq(test_reg, 0, 2, 0xff, exception_handler#)
 * @endcode
 *
 * @param in_src        GPR or read transfer reg containing byte field
 * @param in_start_byte Starting byte position 0-3 based on endian mode
 * @param in_end_byte   Ending byte position 0-3 based on endian mode. Must be greater than @p in_start byte.
 * @param COMPARE_VAL   Constant value to compare field with
 * @param target_label  Label to branch to
 *
 * @instruction_cnt 2 to 4
 */
#macro bytefield_br_lesseq(in_src, in_start_byte, in_end_byte, COMPARE_VAL, target_label)
    comp_field(in_src, in_start_byte, in_end_byte, COMPARE_VAL)
    ble[target_label]
#endm


/** Extract byte field based on endian mode. Compare byte field to @p COMPARE_VAL.
 *
 *  Condition code is set as result of compare.
 *
 * @b Example:
 * @code
 * bytefield_comp(test_reg, 0, 2, 0xff)
 * @endcode
 *
 * @param in_src        GPR or read transfer reg containing byte field
 * @param in_start_byte Starting byte position 0-3 based on endian mode
 * @param in_end_byte   Ending byte position 0-3 based on endian mode. Must be greater than @p in_start byte.
 * @param COMPARE_VAL   Constant value to compare field with
 *
 * @instruction_cnt 2 to 4
 */
#macro bytefield_comp(in_src, in_start_byte, in_end_byte, COMPARE_VAL)
.begin

    .reg comp_val1 comp_val2
    #ifdef SBYTE
        #warning "SBYTE is being redefined."
    #endif
    #ifdef EBYTE
        #warning "EBYTE is being redefined."
    #endif
    #define_eval SBYTE in_start_byte
    #define_eval EBYTE in_end_byte

    #if (in_start_byte == in_end_byte)
        extract/**/SBYTE(comp_val1, in_src, 0, 0)
    #else
        extract/**/SBYTE/**/EBYTE(comp_val1, in_src, 0, 0)
    #endif
    #if ( (COMPARE_VAL > MAX_IMMEDIATE) || (COMPARE_VAL < 0) )
        immed32(comp_val2, COMPARE_VAL)
        alu[--, comp_val1, -, comp_val2]
    #else
        alu[--, comp_val1, -, COMPARE_VAL]
    #endif

    #undef SBYTE
    #undef EBYTE

.end
#endm


/** Insert bytes from @p in_b specified by @p in_byte_mask into @p io_a.
 *
 * The source and destination registers must have the same endianness.
 *
 * @b Example:
 * @code
 * bytefield_shf_left_insert(x, 0110, y, 8, DO_LOAD_CC) // insert y bytes 2,3 into x bytes 1,2
 * @endcode
 *
 * @param io_a          GPR
 * @param in_byte_mask  xxxx, where x = 0 or 1. If 1, insert byte.
 * @param in_b          GPR or read transfer register. If GPR, it must be on the
 *                      opposite bank as @p io_a.
 * @param in_shift_amt  Shift amount 0-31
 * @param in_load_cc    CONSTANT to specify whether user wants the load ALU condition codes
 *                      based on the result performed. @n@n
 *                      Possible values are: @n
 *                      @arg @c NO_LOAD_CC:  Do not load condition code - suggested value if users
 *                                            only want to select byte(s) without any side effect
 *                      @arg @c DO_LOAD_CC:   Load condition code
 *
 * @instruction_cnt 1 to 3
 */
#macro bytefield_shf_left_insert(io_a, in_byte_mask, in_b, in_shift_amt, in_load_cc)
#ifdef _SHF_AMT
    #warning "_SHF_AMT is being redefined."
#endif
#define_eval _SHF_AMT in_shift_amt
    _DEF_LOAD_CC_TOKEN(in_load_cc)

    ld_field[io_a, in_byte_mask, in_b, <</**/_SHF_AMT]/**/_LOAD_CC_TOKEN
#undef _SHF_AMT
#endm


/** Insert bytes from @p in_b specified by @p in_byte_mask into @p io_a after
 *  shifting @p in_b left by @p in_shift_amt.
 *
 * The source and destination registers must have the same endianness.
 *
 * @b Example:
 * @code
 * bytefield_shf_right_insert(x, 0110, y, 8, DO_LOAD_CC) // insert y bytes 0,1 into x bytes 1,2
 * @endcode
 *
 * @param io_a          GPR
 * @param in_byte_mask  xxxx, where x = 0 or 1. If 1, insert byte.
 * @param in_b          GPR or read transfer register. If GPR, it must be on the
 *                      opposite bank as @p io_a.
 * @param in_shift_amt  Shift amount 0-31
 * @param in_load_cc    CONSTANT to specify whether user wants the load ALU condition codes
 *                      based on the result performed @n@n
 *                      Possible values are: @n
 *                      @arg @c NO_LOAD_CC:  Do not load condition code - suggested value if users
 *                                            only want to select byte(s) without any side effect
 *                      @arg @c DO_LOAD_CC:   Load condition code
 *
 * @instruction_cnt 1 to 3
 */
#macro bytefield_shf_right_insert(io_a, in_byte_mask, in_b, in_shift_amt, in_load_cc)
#ifdef _SHF_AMT
    #warning "_SHF_AMT is being redefined."
#endif
#define_eval _SHF_AMT in_shift_amt
    _DEF_LOAD_CC_TOKEN(in_load_cc)

    ld_field[io_a, in_byte_mask, in_b, >>/**/_SHF_AMT]/**/_LOAD_CC_TOKEN
#undef _SHF_AMT
#endm


/** Insert bytes from @p in_b specified by @p in_byte_mask into @p out_result.
 *
 * The output register @p out_result is cleared prior to the insert.
 *
 * The source and destination registers must have the same endianness.
 *
 * @b Example:
 * @code
 * bytefield_clr_insert(x, 0110, y, DO_LOAD_CC)       // insert y bytes 1,2 into x bytes 1,2
 * @endcode
 *
 * @param out_result    GPR or write transfer register
 * @param in_byte_mask  xxxx, where x = 0 or 1. If 1, insert byte.
 * @param in_b          GPR or read transfer register. If both @p out_result and @p in_b are GPR,
 *                      @p in_b must be on the opposite bank as @p out_result.
 * @param in_load_cc    CONSTANT to specify whether user wants the load ALU condition codes
 *                      based on the result performed. @n@n
 *                      Possible values are: @n
 *                      @arg @c NO_LOAD_CC:  Do not load condition code - suggested value if users
 *                                            only want to select byte(s) without any side effect
 *                      @arg @c DO_LOAD_CC:   Load condition code
 *
 * @instruction_cnt 1 to 3
 */
#macro bytefield_clr_insert(out_result, in_byte_mask, in_b, in_load_cc)
    _DEF_LOAD_CC_TOKEN(in_load_cc)

    ld_field_w_clr[out_result, in_byte_mask, in_b]/**/_LOAD_CC_TOKEN
#endm


/** Insert bytes from @p in_b specified by @p in_byte_mask into @p out_result
 *  after shifting @p in_b left by @p in_shift_amt.
 *
 * The output register @p out_result is cleared prior to the insert.
 *
 * The source and destination registers must have the same endianness.
 *
 * @b Example:
 * @code
 * bytefield_clr_shf_left_insert(x, 0110, y, 8, DO_LOAD_CC) // insert y bytes 2,3 into x bytes 1,2
 * @endcode
 *
 * @param out_result    GPR or write transfer register
 * @param in_byte_mask  xxxx, where x = 0 or 1. If 1, insert byte.
 * @param in_b          GPR or read transfer register. If both @p out_result and @p in_b are GPR,
 *                     @p in_b must be on the opposite bank as @p out_result.
 * @param in_shift_amt  Shift amount 0-31
 * @param in_load_cc    CONSTANT to specify whether user wants the load ALU condition codes
 *                      based on the result performed. @n@n
 *                      Possible values are: @n
 *                      @arg @c NO_LOAD_CC:  Do not load condition code - suggested value if users
 *                                            only want to select byte(s) without any side effect
 *                      @arg @c DO_LOAD_CC:   Load condition code
 *
 * @instruction_cnt 1 to 3
 */
#macro bytefield_clr_shf_left_insert(out_result, in_byte_mask, in_b, in_shift_amt, in_load_cc)
#ifdef _SHF_AMT
    #warning "_SHF_AMT is being redefined."
#endif
#define_eval _SHF_AMT in_shift_amt
    _DEF_LOAD_CC_TOKEN(in_load_cc)

    ld_field_w_clr[out_result, in_byte_mask, in_b, <</**/_SHF_AMT]/**/_LOAD_CC_TOKEN
#undef _SHF_AMT
#endm


/** Insert bytes from @p in_b specified by @p in_byte_mask into @p out_result
 *  after shifting @p in_b left by @p in_shift_amt
 *
 * The output register @p out_result is cleared prior to the insert.
 *
 * The source and destination registers must have the same endianness.
 *
 * @b Example:
 * @code
 * bytefield_clr_shf_right_insert(x, 0110, y, 8, DO_LOAD_CC) // insert y bytes 0,1 into x bytes 1,2
 * @endcode
 *
 * @param out_result    GPR or write transfer register
 * @param in_byte_mask  xxxx, where x = 0 or 1. If 1, insert byte.
 * @param in_b          GPR or read transfer register. If both @p out_result and @p in_b are GPR,
 *                      @p in_b must be on the opposite bank as @p out_result.
 * @param in_shift_amt  Shift amount 0-31
 * @param in_load_cc    CONSTANT to specify whether user wants the load ALU condition codes
 *                      based on the result performed. @n@n
 *                      Possible values are: @n
 *                      @arg @c NO_LOAD_CC:  Do not load condition code - suggested value if users
 *                                            only want to select byte(s) without any side effect
 *                      @arg @c DO_LOAD_CC:   Load condition code
 *
 * @instruction_cnt 1 to 3
 */
#macro bytefield_clr_shf_right_insert(out_result, in_byte_mask, in_b, in_shift_amt, in_load_cc)
#ifdef _SHF_AMT
    #warning "_SHF_AMT is being redefined."
#endif
#define_eval _SHF_AMT in_shift_amt
    _DEF_LOAD_CC_TOKEN(in_load_cc)

    ld_field_w_clr[out_result, in_byte_mask, in_b, >>/**/_SHF_AMT]/**/_LOAD_CC_TOKEN
#undef _SHF_AMT
#endm


/** @}
 * @}
 */

#endif /* __BYTEFIELD_UC__ */
