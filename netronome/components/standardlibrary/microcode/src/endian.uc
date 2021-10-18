/*
 * Copyright (C) 2008-2011 Netronome Systems, Inc.  All rights reserved.
 */

#ifndef __ENDIAN_UC__
#define __ENDIAN_UC__

#ifndef NFP_LIB_ANY_NFAS_VERSION
    #if (!defined(__NFP_TOOL_NFAS) ||                       \
        (__NFP_TOOL_NFAS < NFP_SW_VERSION(5, 0, 0, 0)) ||   \
        (__NFP_TOOL_NFAS > NFP_SW_VERSION(6, 1, 0, 255)))
        #error "This standard library is not supported for the version of the SDK currently in use."
    #endif
#endif

#include <stdmac.uc>
#include <util.uc>

// @file endian.uc Big / Little-endian Related Macros
// @addtogroup endian Big / Little-endian
//
//
// @name Big / Little-endian Related Macros
//

/** @file endian.uc Big-Little-endian Related Macros
 * @addtogroup endian Big-Little-endian
 * @{
 *
 * @name Big-Little-endian Related Macros
 * @{
 *
 * Endian independent internal macros supporting @ref bytefield.uc.
 * Default is BIG_ENDIAN. LITTLE_ENDIAN is not yet supported.
 *
 * These macros perform swaps on operators and stores the result according to the
 * following naming convention:
 *    @arg swap :== e
 *    @arg no_character :==
 *    @arg result_op :== e | @<no_character@>
 *    @arg all_bytes :== @<no_character@>
 *    @arg left_to_right_bytes :== 01 | 12 | 23 | @<all_bytes@>
 *    @arg function :== add | comp
 *    @arg aoperand :== _ea@<left_to_right_bytes@> | @<no_character@>
 *    @arg boperand :== _eb@<left_to_right_bytes@> | @<no_character@>
 *    @arg prev_carry :== _c | @<no_charcter@>
 *
 *    @arg unary operator = @<result_op@>@<function@>@<left_to_right_bytes@>
 *    @arg operator = @<result_op@>@<function@>@<aoperand@>@<boperand@>@<prev_carry@>
 *
 */

// API Quick Reference:
//
// comp                                 compare. aop - bop
// comp_ea_eb                           compare. swap(aop) - swap(bop)
// comp_ea                              compare. swap(aop) - bop
// comp_ea32, comp_ea01                 compare. swap(left 2 bytes aop) - bop
// comp_ea21, comp_ea12                 compare. swap(mid 2 bytes aop) - bop
// comp_ea10, comp_ea23                 compare. swap(right 2 bytes aop) - bop
// eadd_ea_eb                           add. res = swap( swap(aop) + swap(bop) )
// add_ea_eb                            add. res = swap(aop) + swap(bop)
// eadd_ea                              add. res = swap( swap(aop) + bop )
// add_ea                               add. res = swap(aop) + bop
// add_ea10_eb, add_ea23_eb             add. res = swap(right 2 bytes aop) + swap(bop)
// add_ea32_eb, add_ea01_eb             add. res = swap(left 2 bytes aop) + swap(bop)
// add_ea_c                             add. res = swap(aop) + bop + prev_carry
// add_ea10_c, add_ea23_c               add. res = swap(right 2 bytes aop) + bop + prev_carry
// add_ea32_c, add_ea01_c               add. res = swap(left 2 bytes aop) + bop + prev_carry



#ifdef _ENDIAN_SHIFT
    #error "_ENDIAN_SHIFT is used by endian.uc and should not be used elsewhere."
#endif


/** Four byte endian swap.
 *
 * @b Example:
 * @code
 * aop = 0xAABBCCDD
 * res = 0xDDCCBBAA
 * @endcode
 *
 * @param res           Result register
 * @param aop           Input value
 * @param in_load_cc    CONSTANT to specify whether user wants the load ALU condition codes
 *                      based on the result performed @n@n
 *                      Possible values are: @n
 *                      @arg @c NO_LOAD_CC:  Do not load condition code - suggested value if users
 *                                           only want to select byte(s) without any side effect
 *                      @arg @c DO_LOAD_CC:  Load condition code
 *
 */
#macro swap(res, aop, in_load_cc)
    _DEF_LOAD_CC_TOKEN(in_load_cc)
    ld_field[res, 1111, aop, <<rot8]            // res = aop BBCCDDAA
    ld_field[res, 1010, aop, <<rot24]/**/_LOAD_CC_TOKEN // res = aop rot DDAABBCC insert DDxxBBxx
#endm


#define swap32 swap01 /**< Alias for @ref swap01 */
/** Two byte endian swap of two left-most bytes.
 *
 * @b Example:
 * @code
 * aop = 0xAABBCCDD
 * res = 0x0000BBAA
 * @endcode
 *
 * @param res           Result register
 * @param aop           Input value
 * @param in_load_cc    CONSTANT to specify whether user wants the load ALU condition codes
 *                      based on the result performed @n@n
 *                      Possible values are: @n
 *                      @arg @c NO_LOAD_CC:  Do not load condition code - suggested value if users
 *                                           only want to select byte(s) without any side effect
 *                      @arg @c DO_LOAD_CC:  Load condition code
 *
 */
#macro swap01(res, aop, in_load_cc)
    _DEF_LOAD_CC_TOKEN(in_load_cc)
    ld_field_w_clr[res, 0001, aop, >>24]        // res 000000AA = aop AABBCCDD
    ld_field[res, 0010, aop, >>8]/**/_LOAD_CC_TOKEN     // res 0000BBAA = insert aop xxxxBBxx into res
#endm


#define swap21 swap12 /**< Alias for @ref swap12 */
/** Two byte endian swap of two middle bytes.
 *
 * @b Example:
 * @code
 * aop = 0xAABBCCDD
 * res = 0x0000CCBB
 * @endcode
 *
 * @param res           Result register
 * @param aop           Input value
 * @param in_load_cc    CONSTANT to specify whether user wants the load ALU condition codes
 *                      based on the result performed @n@n
 *                      Possible values are: @n
 *                      @arg @c NO_LOAD_CC:  Do not load condition code - suggested value if users
 *                                           only want to select byte(s) without any side effect
 *                      @arg @c DO_LOAD_CC:  Load condition code
 *
 */
#macro swap12(res, aop, in_load_cc)
    _DEF_LOAD_CC_TOKEN(in_load_cc)
    ld_field_w_clr[res, 0001, aop, >>16]        // res 000000BB = aop AABBCCDD
    ld_field[res, 0010, aop]/**/_LOAD_CC_TOKEN          // res 0000CCBB = insert aop xxxxCCxx into res
#endm


#define swap10 swap23 /**< Alias for @ref swap23 */
/** Two byte endian swap of two right-most bytes.
 *
 * @b Example:
 * @code
 * aop = 0xAABBCCDD
 * res = 0x0000DDCC
 * @endcode
 *
 * @param res           Result register
 * @param aop           Input value
 * @param in_load_cc    CONSTANT to specify whether user wants the load ALU condition codes
 *                      based on the result performed @n@n
 *                      Possible values are: @n
 *                      @arg @c NO_LOAD_CC:  Do not load condition code - suggested value if users
 *                                           only want to select byte(s) without any side effect
 *                      @arg @c DO_LOAD_CC:  Load condition code
 *
 */
#macro swap23(res, aop, in_load_cc)
    _DEF_LOAD_CC_TOKEN(in_load_cc)
    ld_field_w_clr[res, 0001, aop, >>8]         // res 000000CC = aop AABBCCDD
    ld_field[res, 0010, aop, <<8]/**/_LOAD_CC_TOKEN     // res 0000DDCC = insert aop xxxxDDxx into res
#endm


/// @cond INTERNAL_MACROS
#macro _DEF_1BYTE_LOAD_MASK()
    #if (_ENDIAN_SHIFT == 0)
        #define _LOAD_MASK 0001
    #elif (_ENDIAN_SHIFT == 8)
        #define _LOAD_MASK 0010
    #elif (_ENDIAN_SHIFT == 16)
        #define _LOAD_MASK 0100
    #elif (_ENDIAN_SHIFT == 24)
        #define _LOAD_MASK 1000
    #endif
#endm
/// @endcond


/** Extract byte 0 from @p source, store in cleared @p dest at bit @p shift_amt.
 *
 * @b Example:
 * @code
 * src = 0xAABBCCDD
 *
 * extract0(out, src, 24, NO_LOAD_CC)   // out == 0xAA000000
 * extract0(out, src, 8, NO_LOAD_CC)    // out == 0x0000AA00
 * @endcode
 *
 * @param dest          Destination register
 * @param source        Input value
 * @param shift_amt     Left shift count of the extracted byte. Must be 0, 8, 16 or 24
 * @param in_load_cc    CONSTANT to specify whether user wants the load ALU condition codes
 *                      based on the result performed @n@n
 *                      Possible values are: @n
 *                      @arg @c NO_LOAD_CC:  Do not load condition code - suggested value if users
 *                                           only want to select byte(s) without any side effect
 *                      @arg @c DO_LOAD_CC:  Load condition code
 */
#macro extract0(dest, source, shift_amt, in_load_cc)
    _DEF_LOAD_CC_TOKEN(in_load_cc)

    #define_eval _ENDIAN_SHIFT shift_amt
    _DEF_1BYTE_LOAD_MASK()

    #define_eval ROTATE (24-_ENDIAN_SHIFT&0x1f)
    ld_field_w_clr[dest, _LOAD_MASK, source, >>rot/**/ROTATE]/**/_LOAD_CC_TOKEN

    #undef _LOAD_MASK
#endm


/** Extract byte 1 from @p source, store in cleared @p dest at bit @p shift_amt.
 *
 * @b Example:
 * @code
 * src = 0xAABBCCDD
 *
 * extract1(out, src, 24, NO_LOAD_CC)   // out == 0xBB000000
 * extract1(out, src, 8, NO_LOAD_CC)    // out == 0x0000BB00
 * @endcode
 *
 * @param dest          Destination register
 * @param source        Input value
 * @param shift_amt     Left shift count of the extracted byte. Must be 0, 8, 16 or 24
 * @param in_load_cc    CONSTANT to specify whether user wants the load ALU condition codes
 *                      based on the result performed @n@n
 *                      Possible values are: @n
 *                      @arg @c NO_LOAD_CC:  Do not load condition code - suggested value if users
 *                                           only want to select byte(s) without any side effect
 *                      @arg @c DO_LOAD_CC:  Load condition code
 *
 */
#macro extract1(dest, source, shift_amt, in_load_cc)
    _DEF_LOAD_CC_TOKEN(in_load_cc)

    #define_eval _ENDIAN_SHIFT shift_amt
    _DEF_1BYTE_LOAD_MASK()

    #define_eval ROTATE (16-_ENDIAN_SHIFT&0x1f)
    ld_field_w_clr[dest, _LOAD_MASK, source, >>rot/**/ROTATE]/**/_LOAD_CC_TOKEN

    #undef _LOAD_MASK
#endm


/** Extract byte 2 from @p source, store in cleared @p dest at bit @p shift_amt.
 *
 * @b Example:
 * @code
 * src = 0xAABBCCDD
 *
 * extract2(out, src, 24, NO_LOAD_CC)   // out == 0xCC000000
 * extract2(out, src, 8, NO_LOAD_CC)    // out == 0x0000CC00
 * @endcode
 *
 * @param dest          Destination register
 * @param source        Input value
 * @param shift_amt     Left shift count of the extracted byte. Must be 0, 8, 16 or 24
 * @param in_load_cc    CONSTANT to specify whether user wants the load ALU condition codes
 *                      based on the result performed @n@n
 *                      Possible values are: @n
 *                      @arg @c NO_LOAD_CC:  Do not load condition code - suggested value if users
 *                                           only want to select byte(s) without any side effect
 *                      @arg @c DO_LOAD_CC:  Load condition code
 *
 */
#macro extract2(dest, source, shift_amt, in_load_cc)
    _DEF_LOAD_CC_TOKEN(in_load_cc)
    #define_eval _ENDIAN_SHIFT shift_amt
    _DEF_1BYTE_LOAD_MASK()

    #define_eval ROTATE (8-_ENDIAN_SHIFT&0x1f)
    ld_field_w_clr[dest, _LOAD_MASK, source, >>rot/**/ROTATE]/**/_LOAD_CC_TOKEN

    #undef _LOAD_MASK
#endm


/** Extract byte 3 from @p source, store in cleared @p dest at bit @p shift_amt.
 *
 * @b Example:
 * @code
 * src = 0xAABBCCDD
 *
 * extract3(out, src, 24, NO_LOAD_CC)   // out == 0xDD000000
 * extract3(out, src, 8, NO_LOAD_CC)    // out == 0x0000DD00
 * @endcode
 *
 * @param dest          Destination register
 * @param source        Input value
 * @param shift_amt     Left shift count of the extracted byte. Must be 0, 8, 16 or 24
 * @param in_load_cc    CONSTANT to specify whether user wants the load ALU condition codes
 *                      based on the result performed @n@n
 *                      Possible values are: @n
 *                      @arg @c NO_LOAD_CC:  Do not load condition code - suggested value if users
 *                                           only want to select byte(s) without any side effect
 *                      @arg @c DO_LOAD_CC:  Load condition code
 *
 */
#macro extract3(dest, source, shift_amt, in_load_cc)
    _DEF_LOAD_CC_TOKEN(in_load_cc)
    #define_eval _ENDIAN_SHIFT shift_amt
    _DEF_1BYTE_LOAD_MASK()

    #define_eval ROTATE (0-_ENDIAN_SHIFT&0x1f)
    ld_field_w_clr[dest, _LOAD_MASK, source, >>rot/**/ROTATE]/**/_LOAD_CC_TOKEN

    #undef _LOAD_MASK
#endm


/// @cond INTERNAL_MACROS
#macro _DEF_RBYTE_LOAD_MASK()
    #if (_ENDIAN_SHIFT == 0)
        #define _RLOAD_MASK 0001
    #elif (_ENDIAN_SHIFT == 8)
        #define _RLOAD_MASK 0010
    #elif (_ENDIAN_SHIFT == 16)
        #define _RLOAD_MASK 0100
    #else
        #define _RLOAD_MASK 0000
    #endif
#endm
/// @endcond


/// @cond INTERNAL_MACROS
#macro _DEF_LBYTE_LOAD_MASK()
    #if (_ENDIAN_SHIFT == 0)
        #define _LLOAD_MASK 0010
    #elif (_ENDIAN_SHIFT == 8)
        #define _LLOAD_MASK 0100
    #elif (_ENDIAN_SHIFT == 16)
        #define _LLOAD_MASK 1000
    #else
        #define _LLOAD_MASK 0000
    #endif
#endm
/// @endcond


/// @cond INTERNAL_MACROS
#macro _DEF_2BYTE_LOAD_MASK()
    #if (_ENDIAN_SHIFT == 0)
        #define _LOAD_MASK 0011
    #elif (_ENDIAN_SHIFT == 8)
        #define _LOAD_MASK 0110
    #elif (_ENDIAN_SHIFT == 16)
        #define _LOAD_MASK 1100
    #else
        #define _LOAD_MASK 0000
    #endif
#endm
/// @endcond


/** Extract bytes 0 and 1 from @p source, store in cleared @p dest at bit @p shift_amt.
 *
 * @b Example:
 * @code
 * src = 0xAABBCCDD
 *
 * extract01(out, src, 16, NO_LOAD_CC)   // out == 0xAABB0000
 * extract01(out, src, 8, NO_LOAD_CC)    // out == 0x00AABB00
 * extract01(out, src, 0, NO_LOAD_CC)    // out == 0x0000AABB
 * @endcode
 *
 * @param dest          Destination register
 * @param source        Input value
 * @param shift_amt     Left shift count of the extracted byte. Must be 0, 8 or 16
 * @param in_load_cc    CONSTANT to specify whether user wants the load ALU condition codes
 *                      based on the result performed @n@n
 *                      Possible values are: @n
 *                      @arg @c NO_LOAD_CC:  Do not load condition code - suggested value if users
 *                                           only want to select byte(s) without any side effect
 *                      @arg @c DO_LOAD_CC:  Load condition code
 *
 */
#macro extract01(dest, source, shift_amt, in_load_cc)
    _DEF_LOAD_CC_TOKEN(in_load_cc)
    #define_eval _ENDIAN_SHIFT shift_amt
    _DEF_RBYTE_LOAD_MASK()
    _DEF_LBYTE_LOAD_MASK()
    _DEF_2BYTE_LOAD_MASK()

    #define_eval ROTATE (16-_ENDIAN_SHIFT&0x1f)
    ld_field_w_clr[dest, _LOAD_MASK, source, >>rot/**/ROTATE]/**/_LOAD_CC_TOKEN

    #undef _RLOAD_MASK
    #undef _LLOAD_MASK
    #undef _LOAD_MASK
#endm


/** Extract bytes 1 and 2 from @p source, store in cleared @p dest at bit @p shift_amt.
 *
 * @b Example:
 * @code
 * src = 0xAABBCCDD
 *
 * extract12(out, src, 16, NO_LOAD_CC)   // out == 0xBBCC0000
 * extract12(out, src, 8, NO_LOAD_CC)    // out == 0x00BBCC00
 * extract12(out, src, 0, NO_LOAD_CC)    // out == 0x0000BBCC
 * @endcode
 *
 * @param dest          Destination register
 * @param source        Input value
 * @param shift_amt     Left shift count of the extracted byte. Must be 0, 8 or 16
 * @param in_load_cc    CONSTANT to specify whether user wants the load ALU condition codes
 *                      based on the result performed @n@n
 *                      Possible values are: @n
 *                      @arg @c NO_LOAD_CC:  Do not load condition code - suggested value if users
 *                                           only want to select byte(s) without any side effect
 *                      @arg @c DO_LOAD_CC:  Load condition code
 *
 */
#macro extract12(dest, source, shift_amt, in_load_cc)
    _DEF_LOAD_CC_TOKEN(in_load_cc)

    #define_eval _ENDIAN_SHIFT shift_amt
    _DEF_RBYTE_LOAD_MASK()
    _DEF_LBYTE_LOAD_MASK()
    _DEF_2BYTE_LOAD_MASK()

    #define_eval ROTATE (8-_ENDIAN_SHIFT&0x1f)
    ld_field_w_clr[dest, _LOAD_MASK, source, >>rot/**/ROTATE]/**/_LOAD_CC_TOKEN

    #undef _RLOAD_MASK
    #undef _LLOAD_MASK
    #undef _LOAD_MASK
#endm


/** Extract bytes 2 and 3 from @p source, store in cleared @p dest at bit @p shift_amt.
 *
 * @b Example:
 * @code
 * src = 0xAABBCCDD
 *
 * extract23(out, src, 16, NO_LOAD_CC)   // out == 0xCCDD0000
 * extract23(out, src, 8, NO_LOAD_CC)    // out == 0x00CCDD00
 * extract23(out, src, 0, NO_LOAD_CC)    // out == 0x0000CCDD
 * @endcode
 *
 * @param dest          Destination register
 * @param source        Input value
 * @param shift_amt     Left shift count of the extracted byte. Must be 0, 8 or 16
 * @param in_load_cc    CONSTANT to specify whether user wants the load ALU condition codes
 *                      based on the result performed @n@n
 *                      Possible values are: @n
 *                      @arg @c NO_LOAD_CC:  Do not load condition code - suggested value if users
 *                                           only want to select byte(s) without any side effect
 *                      @arg @c DO_LOAD_CC:  Load condition code
 *
 */
#macro extract23(dest, source, shift_amt, in_load_cc)
    _DEF_LOAD_CC_TOKEN(in_load_cc)
    #define_eval _ENDIAN_SHIFT shift_amt
    _DEF_RBYTE_LOAD_MASK()
    _DEF_LBYTE_LOAD_MASK()
    _DEF_2BYTE_LOAD_MASK()

    #define_eval ROTATE (0-_ENDIAN_SHIFT&0x1f)
    ld_field_w_clr[dest, _LOAD_MASK, source, >>rot/**/ROTATE]/**/_LOAD_CC_TOKEN

    #undef _RLOAD_MASK
    #undef _LLOAD_MASK
    #undef _LOAD_MASK
#endm


/// @cond INTERNAL_MACROS
#macro _DEF_3BYTE_LOAD_MASK()
    #if (_ENDIAN_SHIFT == 0)
        #define _LOAD_MASK 0111
    #elif (_ENDIAN_SHIFT == 8)
        #define _LOAD_MASK 1110
    #endif
#endm
/// @endcond


/** Extract bytes 0 to 2 from @p source, store in cleared @p dest at bit @p shift_amt.
 *
 * @b Example:
 * @code
 * src = 0xAABBCCDD
 *
 * extract02(out, src, 8, NO_LOAD_CC)    // out == 0xAABBCC00
 * extract02(out, src, 0, NO_LOAD_CC)    // out == 0x00AABBCC
 * @endcode
 *
 * @param dest          Destination register
 * @param source        Input value
 * @param shift_amt     Left shift count of the extracted byte. Must be 0 or 8
 * @param in_load_cc    CONSTANT to specify whether user wants the load ALU condition codes
 *                      based on the result performed @n@n
 *                      Possible values are: @n
 *                      @arg @c NO_LOAD_CC:  Do not load condition code - suggested value if users
 *                                           only want to select byte(s) without any side effect
 *                      @arg @c DO_LOAD_CC:  Load condition code
 *
 */
#macro extract02(dest, source, shift_amt, in_load_cc)
    _DEF_LOAD_CC_TOKEN(in_load_cc)
    #define_eval _ENDIAN_SHIFT shift_amt

    _DEF_3BYTE_LOAD_MASK()
    #define_eval ROTATE (8-_ENDIAN_SHIFT&0x1f)
    ld_field_w_clr[dest, _LOAD_MASK, source, >>rot/**/ROTATE]/**/_LOAD_CC_TOKEN
    #undef _LOAD_MASK
#endm


/** Extract bytes 1 to 3 from @p source, store in cleared @p dest at bit @p shift_amt.
 *
 * @b Example:
 * @code
 * src = 0xAABBCCDD
 *
 * extract13(out, src, 8, NO_LOAD_CC)    // out == 0xBBCCDD00
 * extract13(out, src, 0, NO_LOAD_CC)    // out == 0x00BBCCDD
 * @endcode
 *
 * @param dest          Destination register
 * @param source        Input value
 * @param shift_amt     Left shift count of the extracted byte. Must be 0 or 8
 * @param in_load_cc    CONSTANT to specify whether user wants the load ALU condition codes
 *                      based on the result performed @n@n
 *                      Possible values are: @n
 *                      @arg @c NO_LOAD_CC:  Do not load condition code - suggested value if users
 *                                           only want to select byte(s) without any side effect
 *                      @arg @c DO_LOAD_CC:  Load condition code
 *
 */
#macro extract13(dest, source, shift_amt, in_load_cc)
    _DEF_LOAD_CC_TOKEN(in_load_cc)
    #define_eval _ENDIAN_SHIFT shift_amt

    _DEF_3BYTE_LOAD_MASK()
    #define_eval ROTATE (0-_ENDIAN_SHIFT&0x1f)
    ld_field_w_clr[dest, _LOAD_MASK, source, >>rot/**/ROTATE]/**/_LOAD_CC_TOKEN
    #undef _LOAD_MASK
#endm


/** Extract bytes 0 to 3 from @p source, store in cleared @p dest at bit @p shift_amt.
 *
 * This function is mainly provided for generic coding in other modules.
 *
 * @param dest          Destination register
 * @param source        Input value
 * @param dummySHIFT    Ignored
 * @param in_load_cc    Ignored, macro behaves as if @c DO_LOAD_CC is used
 *
 */
#macro extract03(dest, source, dummySHIFT, in_load_cc)
    _DEF_LOAD_CC_TOKEN(in_load_cc)

    alu[dest, --, B, source]
#endm


/** Extract byte 0 from @p source, store in @p dest at bit 0.
 *
 * @b Example:
 * @code
 * src = 0xAABBCCDD
 * out = 0x11223344
 *
 * merge_extract0(out, src, NO_LOAD_CC)   // out == 0x112233AA
 * @endcode
 *
 * @param dest          Destination register
 * @param source        Input value
 * @param in_load_cc    CONSTANT to specify whether user wants the load ALU condition codes
 *                      based on the result performed @n@n
 *                      Possible values are: @n
 *                      @arg @c NO_LOAD_CC:  Do not load condition code - suggested value if users
 *                                           only want to select byte(s) without any side effect
 *                      @arg @c DO_LOAD_CC:  Load condition code
 *
 */
#macro merge_extract0(dest, source, in_load_cc)
    _DEF_LOAD_CC_TOKEN(in_load_cc)

    ld_field[dest, 0001, source, >>24]/**/_LOAD_CC_TOKEN
#endm


/** Extract byte 1 from @p source, store in @p dest at bit 0.
 *
 * @b Example:
 * @code
 * src = 0xAABBCCDD
 * out = 0x11223344
 *
 * merge_extract1(out, src, NO_LOAD_CC)   // out == 0x112233BB
 * @endcode
 *
 * @param dest          Destination register
 * @param source        Input value
 * @param in_load_cc    CONSTANT to specify whether user wants the load ALU condition codes
 *                      based on the result performed @n@n
 *                      Possible values are: @n
 *                      @arg @c NO_LOAD_CC:  Do not load condition code - suggested value if users
 *                                           only want to select byte(s) without any side effect
 *                      @arg @c DO_LOAD_CC:  Load condition code
 *
 */
#macro merge_extract1(dest, source, in_load_cc)
    _DEF_LOAD_CC_TOKEN(in_load_cc)

    ld_field[dest, 0001, source, >>16]/**/_LOAD_CC_TOKEN
#endm


/** Extract byte 2 from @p source, store in @p dest at bit 0.
 *
 * @b Example:
 * @code
 * src = 0xAABBCCDD
 * out = 0x11223344
 *
 * merge_extract2(out, src, NO_LOAD_CC)   // out == 0x112233CC
 * @endcode
 *
 * @param dest          Destination register
 * @param source        Input value
 * @param in_load_cc    CONSTANT to specify whether user wants the load ALU condition codes
 *                      based on the result performed @n@n
 *                      Possible values are: @n
 *                      @arg @c NO_LOAD_CC:  Do not load condition code - suggested value if users
 *                                           only want to select byte(s) without any side effect
 *                      @arg @c DO_LOAD_CC:  Load condition code
 *
 */
#macro merge_extract2(dest, source, in_load_cc)
    _DEF_LOAD_CC_TOKEN(in_load_cc)

    ld_field[dest, 0001, source, >>8]/**/_LOAD_CC_TOKEN
#endm


/** Extract byte 3 from @p source, store in @p dest at bit 0.
 *
 * @b Example:
 * @code
 * src = 0xAABBCCDD
 * out = 0x11223344
 *
 * merge_extract3(out, src, NO_LOAD_CC)   // out == 0x112233DD
 * @endcode
 *
 * @param dest          Destination register
 * @param source        Input value
 * @param in_load_cc    CONSTANT to specify whether user wants the load ALU condition codes
 *                      based on the result performed @n@n
 *                      Possible values are: @n
 *                      @arg @c NO_LOAD_CC:  Do not load condition code - suggested value if users
 *                                           only want to select byte(s) without any side effect
 *                      @arg @c DO_LOAD_CC:  Load condition code
 *
 */
#macro merge_extract3(dest, source, in_load_cc)
    _DEF_LOAD_CC_TOKEN(in_load_cc)

    ld_field[dest, 0001, source]/**/_LOAD_CC_TOKEN
#endm


/** Extract byte 0 and 1 from @p source, store in @p dest at bit 0.
 *
 * @b Example:
 * @code
 * src = 0xAABBCCDD
 * out = 0x11223344
 *
 * merge_extract01(out, src, NO_LOAD_CC)   // out == 0x1122AABB
 * @endcode
 *
 * @param dest          Destination register
 * @param source        Input value
 * @param in_load_cc    CONSTANT to specify whether user wants the load ALU condition codes
 *                      based on the result performed @n@n
 *                      Possible values are: @n
 *                      @arg @c NO_LOAD_CC:  Do not load condition code - suggested value if users
 *                                           only want to select byte(s) without any side effect
 *                      @arg @c DO_LOAD_CC:  Load condition code
 *
 */
#macro merge_extract01(dest, source, in_load_cc)
    _DEF_LOAD_CC_TOKEN(in_load_cc)

    ld_field[dest, 0011, source, >>16]/**/_LOAD_CC_TOKEN
#endm


/** Extract byte 1 and 2 from @p source, store in @p dest at bit 0.
 *
 * @b Example:
 * @code
 * src = 0xAABBCCDD
 * out = 0x11223344
 *
 * merge_extract12(out, src, NO_LOAD_CC)   // out == 0x1122BBCC
 * @endcode
 *
 * @param dest          Destination register
 * @param source        Input value
 * @param in_load_cc    CONSTANT to specify whether user wants the load ALU condition codes
 *                      based on the result performed @n@n
 *                      Possible values are: @n
 *                      @arg @c NO_LOAD_CC:  Do not load condition code - suggested value if users
 *                                           only want to select byte(s) without any side effect
 *                      @arg @c DO_LOAD_CC:  Load condition code
 *
 */
#macro merge_extract12(dest, source, in_load_cc)
    _DEF_LOAD_CC_TOKEN(in_load_cc)

    ld_field[dest, 0011, source, >>8]/**/_LOAD_CC_TOKEN
#endm


/** Extract byte 2 and 3 from @p source, store in @p dest at bit 0.
 *
 * @b Example:
 * @code
 * src = 0xAABBCCDD
 * out = 0x11223344
 *
 * merge_extract23(out, src, NO_LOAD_CC)   // out == 0x1122CCDD
 * @endcode
 *
 * @param dest          Destination register
 * @param source        Input value
 * @param in_load_cc    CONSTANT to specify whether user wants the load ALU condition codes
 *                      based on the result performed @n@n
 *                      Possible values are: @n
 *                      @arg @c NO_LOAD_CC:  Do not load condition code - suggested value if users
 *                                           only want to select byte(s) without any side effect
 *                      @arg @c DO_LOAD_CC:  Load condition code
 *
 */
#macro merge_extract23(dest, source, in_load_cc)
    _DEF_LOAD_CC_TOKEN(in_load_cc)

    ld_field[dest, 0011, source]/**/_LOAD_CC_TOKEN
#endm


/** Extract byte 0 to 2 from @p source, store in @p dest at bit 0.
 *
 * @b Example:
 * @code
 * src = 0xAABBCCDD
 * out = 0x11223344
 *
 * merge_extract02(out, src, NO_LOAD_CC)   // out == 0x11AABBCC
 * @endcode
 *
 * @param dest          Destination register
 * @param source        Input value
 * @param in_load_cc    CONSTANT to specify whether user wants the load ALU condition codes
 *                      based on the result performed @n@n
 *                      Possible values are: @n
 *                      @arg @c NO_LOAD_CC:  Do not load condition code - suggested value if users
 *                                           only want to select byte(s) without any side effect
 *                      @arg @c DO_LOAD_CC:  Load condition code
 *
 */
#macro merge_extract02(dest, source, in_load_cc)
    _DEF_LOAD_CC_TOKEN(in_load_cc)

    ld_field[dest, 0111, source, >>8]/**/_LOAD_CC_TOKEN
#endm


/** Extract byte 1 to 3 from @p source, store in @p dest at bit 0.
 *
 * @b Example:
 * @code
 * src = 0xAABBCCDD
 * out = 0x11223344
 *
 * merge_extract13(out, src, NO_LOAD_CC)   // out == 0x11BBCCDD
 * @endcode
 *
 * @param dest          Destination register
 * @param source        Input value
 * @param in_load_cc    CONSTANT to specify whether user wants the load ALU condition codes
 *                      based on the result performed @n@n
 *                      Possible values are: @n
 *                      @arg @c NO_LOAD_CC:  Do not load condition code - suggested value if users
 *                                           only want to select byte(s) without any side effect
 *                      @arg @c DO_LOAD_CC:  Load condition code
 *
 */
#macro merge_extract13(dest, source, in_load_cc)
    _DEF_LOAD_CC_TOKEN(in_load_cc)

    ld_field[dest, 0111, source]/**/_LOAD_CC_TOKEN
#endm


/** Compare @p aop to @p bop.
 *
 * @param aop Operand A
 * @param bop Operand B
 * @condition_codes Affected by (aop - bop)
 */
#macro comp(aop, bop)
    alu[--, aop, -, bop]
#endm


/** Compare 4 byte endian swapped @p aop to 4 byte endian swapped @p bop.
 *
 * @param aop Operand A
 * @param bop Operand B
 * @condition_codes Affected by (swap(aop) - swap(bop))
 */
#macro comp_ea_eb(aop, bop)
.local aswap bswap

    swap(aswap, aop)
    swap(bswap, bop)
    alu[--, aswap, -, bswap]

.endlocal
#endm


/** Compare 4 byte endian swapped @p aop to @p bop.
 *
 * @param aop Operand A
 * @param bop Operand B
 * @condition_codes Affected by (swap(aop) - bop)
 */
#macro comp_ea(aop, bop)
.local aswap

    swap(aswap, aop)
    alu[--, aswap, -, bop]

.endlocal
#endm


#define comp_ea32 comp_ea01 /**< Alias for @ref comp_ea01 */
/** 16 bit big-endian compare aop to big-endian bop, compare left 2 bytes of aop to bop.
 *
 * @param aop Operand A
 * @param bop Operand B
 * @condition_codes Affected by (swap01(aop) - bop)
 */
#macro comp_ea01(aop, bop)
.local aswap

    swap01(aswap, aop)
    alu[--, aswap, -, bop]

.endlocal
#endm


#define comp_ea21 comp_ea12 /**< Alias for @ref comp_ea12 */
/** 16 bit big-endian compare aop to big-endian bop, compare middle 2 bytes of aop to bop.
 *
 * @param aop Operand A
 * @param bop Operand B
 * @condition_codes Affected by (swap12(aop) - bop)
 */
#macro comp_ea12(aop, bop)
.local aswap

    swap12(aswap, aop)
    alu[--, aswap, -, bop]

.endlocal
#endm


#define comp_ea10 comp_ea23 /**< Alias for @ref comp_ea23 */
/** 16 bit big-endian compare aop to big-endian bop, compare right 2 bytes of aop to bop.
 *
 * @param aop Operand A
 * @param bop Operand B
 * @condition_codes Affected by (swap23(aop) - bop)
 */
#macro comp_ea23(aop, bop)
.local aswap

    swap23(aswap, aop)
    alu[--, aswap, -, bop]

.endlocal
#endm


/** 4 byte endian add of swapped @p aop and swapped @p bop, swapped result.
 *
 * @b Executes:
 * @code
 * res = swap( swap(aop) + swap(bop) )
 * @endcode
 *
 */
#macro eadd_ea_eb(res, aop, bop)
.local aswap

    swap(aswap, aop)
    swap(res, bop)
    alu[aswap, aswap, +, res]
    swap(res, aswap)

.endlocal
#endm


/** 4 byte endian add of swapped @p aop and swapped @p bop.
 *
 * @b Executes:
 * @code
 * res = swap(aop) + swap(bop)
 * @endcode
 *
 */
#macro add_ea_eb(res, aop, bop)
.local aswap

    swap(aswap, aop)
    swap(res, bop)
    alu[res, aswap, +, res]

.endlocal
#endm


/** 4 byte endian add of swapped @p aop and @p bop, swapped result.
 *
 * @b Executes:
 * @code
 * res = swap( swap(aop) + bop )
 * @endcode
 *
 * @note @p bop can be the same register as @p res.
 *
 */
#macro eadd_ea(res, aop, bop)
.local aswap

    swap(aswap, aop)
    alu[aswap, aswap, +, bop]
    swap(res, aswap)

.endlocal
#endm


/** 4 byte endian add of swapped @p aop and @p bop.
 *
 * @b Executes:
 * @code
 * res = swap(aop) + bop
 * @endcode
 *
 * @note @p bop can be the same register as @p res.
 *
 */
#macro add_ea(res, aop, bop)
.local aswap

    swap(aswap, aop)
    alu[res, aswap, +, bop]

.endlocal
#endm


#define add_ea32_eb add_ea01_eb /**< Alias for @ref add_ea01_eb */
/** Swap(left 2 bytes of @p aop) + swap(bop).
 *
 * @b Executes:
 * @code
 * res = swap01(aop) + swap(bop)
 * @endcode
 *
 */
#macro add_ea01_eb(res, aop, bop)
.local aswap bswap

    swap01(aswap, aop)                          // halfword 0, len_version
    swap(bswap, bop)
    alu[res, aswap, +, bswap]

.endlocal
#endm


#define add_ea10_eb add_ea23_eb /**< Alias for @ref add_ea23_eb */
/** Swap(right 2 bytes aop) + swap(bop).
 *
 * @b Executes:
 * @code
 * res = swap23(aop) + swap(bop)
 * @endcode
 *
 */
#macro add_ea23_eb(res, aop, bop)
.local aswap bswap

    swap23(aswap, aop)                          // halfword 0, len_version
    swap(bswap, bop)
    alu[res, aswap, +, bswap]

.endlocal
#endm


/** 4 byte endian carry add of swapped @p aop and @p bop.
 *
 * @b Executes:
 * @code
 * res = swap(aop) + bop + previous carry
 * @endcode
 *
 * @note @p bop can be the same register as @p res.
 */
#macro add_ea_c(res, aop, bop)
.local aswap

    swap(aswap, aop)
    alu[res, aswap, +carry, bop]

.endlocal
#endm


#define add_ea10_c add_ea23_c /**< Alias for @ref add_ea23_c */
/** 4 byte endian carry add of swap(right 2 bytes @p aop) and @p bop.
 *
 * @b Executes:
 * @code
 * res = swap23(aop) + bop + previous carry
 * @endcode
 *
 * @note @p bop can be the same register as @p res.
 */
#macro add_ea23_c(res, aop, bop)
.local aswap

    swap23(aswap, aop)                          // halfword 0, len_version
    alu[res, aswap, +carry, bop]

.endlocal
#endm


#define add_ea32_c add_ea01_c /**< Alias for @ref add_ea01_c */
/** 4 byte endian carry add of swap(left 2 bytes @p aop) and @p bop.
 *
 * @b Executes:
 * @code
 * res = swap01(aop) + bop + previous carry
 * @endcode
 *
 * @note @p bop can be the same register as @p res.
 */
#macro add_ea01_c(res, aop, bop)
.local aswap

    swap01(aswap, aop)                          // halfword 0, len_version
    alu[res, aswap, +carry, bop]

.endlocal
#endm


/** Increment byte 0, other bytes are unaffected.
 *
 * Source can be a read transfer register. Destination can be a write transfer register.
 */
#macro incr0(dest, source)
    alu_shf_left(dest, source, +, 1, 24)   // add 1 to byte 0
#endm


/** Increment byte 1, other bytes are unaffected.
 *
 * Source can be a read transfer register. Destination can be a write transfer register.
 */
#macro incr1(dest, source)
.local tempa

    alu_shf_left(tempa, source, +, 1, 16) // add 1 to byte 2
    ld_field[tempa, 1011, source]
    alu[dest, --, B, tempa]

.endlocal
#endm


/** Increment byte 2, other bytes are unaffected.
 *
 * Source can be a read transfer register. Destination can be a write transfer register.
 */
#macro incr2(dest, source)
.local tempa

    alu_shf_left(tempa, source, +, 1, 8)  // add 1 to byte 2
    ld_field[tempa, 1101, source]
    alu[dest, --, B, tempa]

.endlocal
#endm


/** Increment byte 3, other bytes are unaffected.
 *
 * Source can be a read transfer register. Destination can be a write transfer register.
 */
#macro incr3(dest, source)
.local tempa

    alu[tempa, source, +, 1]                    // add 1 to byte 3
    ld_field[tempa, 1110, source]
    alu[dest, --, B, tempa]

.endlocal
#endm


/** Increment bytes 0-1.
 *
 * Source can be a read transfer register. Destination can be a write transfer register.
 */
#macro incr01(dest, source)
    alu_shf_left(dest, source, +, 1, 16)
#endm


/** Increment bytes 1-2, other bytes are unaffected.
 *
 * Source can be a read transfer register. Destination can be a write transfer register.
 */
#macro incr12(dest, source)
.local tempa

    alu_shf_left(tempa, source, +, 1, 8)
    ld_field[tempa, 1001, source]
    alu[dest, --, B, tempa]

.endlocal
#endm


/** Increment bytes 2-3, other bytes are unaffected.
 *
 * Source can be a read transfer register. Destination can be a write transfer register.
 */
#macro incr23(dest, source)
.local tempa

    alu_shf[tempa, source, +, 1]
    ld_field[tempa, 1100, source]
    alu[dest, --, B, tempa]

.endlocal
#endm


/** Increment bytes 0-2.
 *
 * Source can be a read transfer register. Destination can be a write transfer register.
 */
#macro incr02(dest, source)
.local tempa

    alu_shf_left(tempa, source, +, 1, 8)
    ld_field[tempa, 0001, source]
    alu[dest, --, B, tempa]

.endlocal
#endm

/** Increment bytes 1-3, other bytes are unaffected.
 *
 * Source can be a read transfer register. Destination can be a write transfer register.
 */
#macro incr13(dest, source)
.local tempa

    alu_shf[tempa, source, +, 1]
    ld_field[tempa, 1000, source]
    alu[dest, --, B, tempa]

.endlocal
#endm

/** Increment bytes 0-3.
 *
 * Source can be a read transfer register. Destination can be a write transfer register.
 */
#macro incr03(dest, source)
.local tempa

    alu_shf[dest, source, +, 1]

.endlocal
#endm


/** Decrement byte 0, other bytes are unaffected.
 *
 * Source can be a read transfer register. Destination can be a write transfer register.
 */
#macro decr0(dest, source)
.local tempa

    alu_shf_left(tempa, source, -, 1, 24)
    ld_field[tempa, 0111, source]

    alu[dest, --, B, tempa]

.endlocal
#endm


/** Decrement byte 1, other bytes are unaffected.
 *
 * Source can be a read transfer register. Destination can be a write transfer register.
 */
#macro decr1(dest, source)
.local tempa

    alu_shf_left(tempa, source, -, 1, 16)
    ld_field[tempa, 1011, source]

    alu[dest, --, B, tempa]

.endlocal
#endm


/** Decrement byte 2, other bytes are unaffected.
 *
 * Source can be a read transfer register. Destination can be a write transfer register.
 */
#macro decr2(dest, source)
.local output_val

    alu_shf_left(output_val, source, -, 1, 8)
    ld_field[output_val, 1101, source]

    alu[dest, --, B, output_val]

.endlocal
#endm


/** Decrement byte 3, other bytes are unaffected.
 *
 * Source can be a read transfer register. Destination can be a write transfer register.
 */
#macro decr3(dest, source)
.local tempa

    alu_shf[tempa, source, -, 1]
    ld_field[tempa, 1110, source]

    alu[dest, --, B, tempa]

.endlocal
#endm


/** Decrement byte 0-1, other bytes are unaffected.
 *
 * Source can be a read transfer register. Destination can be a write transfer register.
 */
#macro decr01(dest, source)
.local tempa

    alu_shf_left(tempa, source, -, 1, 16)
    ld_field[tempa, 0011, source]
    alu[dest, --, B, tempa]

.endlocal
#endm

/** Decrement byte 1-2, other bytes are unaffected.
 *
 * Source can be a read transfer register. Destination can be a write transfer register.
 */
#macro decr12(dest, source)
.local tempa

    alu_shf_left(tempa, source, -, 1, 8)
    ld_field[tempa, 1001, source]
    alu[dest, --, B, tempa]

.endlocal
#endm


/** Decrement byte 2-3, other bytes are unaffected.
 *
 * Source can be a read transfer register. Destination can be a write transfer register.
 */
#macro decr23(dest, source)
.local tempa

    alu_shf[tempa, source, -, 1]
    ld_field[tempa, 1100, source]
    alu[dest, --, B, tempa]

.endlocal
#endm

/** Decrement byte 0-2, other bytes are unaffected.
 *
 * Source can be a read transfer register. Destination can be a write transfer register.
 */
#macro decr02(dest, source)
.local tempa

    alu_shf_left(tempa, source, -, 1, 8)
    ld_field[tempa, 0001, source]
    alu[dest, --, B, tempa]

.endlocal
#endm

/** Decrement byte 1-3, other bytes are unaffected.
 *
 * Source can be a read transfer register. Destination can be a write transfer register.
 */
#macro decr13(dest, source)
.local tempa

    alu_shf[tempa, source, -, 1]
    ld_field[tempa, 1000, source]
    alu[dest, --, B, tempa]

.endlocal
#endm

/** Decrement byte 0-3.
 *
 * Source can be a read transfer register. Destination can be a write transfer register.
 */
#macro decr03(dest, source)
.local tempa

    alu_shf[dest, source, -, 1]

.endlocal
#endm


/** @}
 * @}
 */

#endif /* __ENDIAN_UC__ */
