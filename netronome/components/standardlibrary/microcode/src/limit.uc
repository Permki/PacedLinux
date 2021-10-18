/*
 * Copyright (C) 2005-2013 Netronome Systems, Inc.  All rights reserved.
 */

#ifndef _LIMIT_UC_
#define _LIMIT_UC_

#ifndef NFP_LIB_ANY_NFAS_VERSION
    #if (!defined(__NFP_TOOL_NFAS) ||                       \
        (__NFP_TOOL_NFAS < NFP_SW_VERSION(5, 0, 0, 0)) ||   \
        (__NFP_TOOL_NFAS > NFP_SW_VERSION(6, 1, 0, 255)))
        #error "This standard library is not supported for the version of the SDK currently in use."
    #endif
#endif

#include <stdmac.uc>

/** @file limit.uc Limit Operation Macros
 * @addtogroup limit Limit Operations
 * @{
 *
 * @name Limit Operation Macros
 * @{
 *
 * These macros perform limit operations
 */

// API Quick Reference:
//
// limit_min(out_c, in_a, in_b)
// limit_min(io_a, in_b)
// limit_min_cc(io_a, in_b)
// limit_max(out_c, in_a, in_b)
// limit_max(io_a, in_b)
// limit_max_cc(io_a, in_b)
// limit_min_unsigned(out_c, in_a, in_b)
// limit_min_unsigned(io_a, in_b)
// limit_min_unsigned_cc(io_a, in_b)
// limit_max_unsigned(out_c, in_a, in_b)
// limit_max_unsigned(io_a, in_b)
// limit_max_unsigned_cc(io_a, in_b)
// limit_align_first_chunk(out_chunk, in_size, in_align, in_address)


/** Calculate minimum of two values using signed arithmetic.
 *
 * @b Example:
 * @code
 * .reg minimum, ina, inb
 * immed[ina,5]
 * immed[inb,6]
 * limit_min(minimum, ina, inb)
 * beq[result_is_zero#]
 * @endcode
 *
 * @param out_c   Register written with lower of ina or inb
 * @param in_a    Register, value to compare to in_b
 * @param in_b    Register, value to compare to in_a
 *
 * Use in_a for the parameter most likely to be the smallest.
 *
 * @condition_codes N,Z set based on result in out_c
 *
 * @instruction_cnt 4
 *
 * Cycles:
 *  For 3 different parameters:
 *   4 if in_a <= in_b, else 5
 *  Where in_a or in_b is the same as out_c:
 *   5 if out_c already lowest, else 4
 */
#macro limit_min(out_c, in_a, in_b)

    #if (streq(out_c, in_a))

        alu[--, out_c, -, in_b]
        blt[done#], defer[1]
        alu[--, --, B, out_c]

        alu[out_c, --, B, in_b]

    #elif (streq(out_c, in_b))

        alu[--, out_c, -, in_a]
        blt[done#], defer[1]
        alu[--, --, B, out_c]

        alu[out_c, --, B, in_a]

    #else

        alu[--, in_b, -, in_a]
        blt[done#], defer[1]
        alu[out_c, --, B, in_b]

        alu[out_c, --, B, in_a]

    #endif

done#:

#endm


/** Calculate minimum of two values using signed arithmetic.
 *
 * @b Example:
 * @code
 * .reg ioa, inb
 * immed[ioa,5]
 * immed[inb,6]
 * limit_min(ioa, inb)
 * @endcode
 *
 * @param io_a    Register, value to compare to in_b, and resultant min value
 * @param in_b    Register, value to compare to io_a
 *
 * @condition_codes Data dependent: assume they are clobbered
 *
 * @instruction_cnt 3
 */
#macro limit_min(io_a, in_b)

    alu[--, io_a, -, in_b]
    blt[done#]
    alu[io_a, --, B, in_b]
done#:

#endm


/** Calculate minimum of two values using signed arithmetic.
 *
 * @b Example:
 * @code
 * .reg ioa, inb
 * immed[ioa,0]
 * immed[inb,6]
 * limit_min_cc(ioa, inb)
 * beq[result_is_zero#]
 * @endcode
 *
 * @param io_a    Register, value to compare to in_b, and resultant min value
 * @param in_b    Register, value to compare to io_a
 *
 * @condition_codes N,Z set based on result in io_a
 *
 * @instruction_cnt 4
 */
#macro limit_min_cc(io_a, in_b)

    alu[--, io_a, -, in_b]
    blt[done#], defer[1]
    alu[--, --, B, io_a]

    alu[io_a, --, B, in_b]
done#:

#endm


/** Calculate maximum of two values using signed arithmetic.
 *
 * @b Example:
 * @code
 * .reg maximum, ina, inb
 * immed[ina,5]
 * immed[inb,6]
 * limit_max(maximum, ina, inb)
 * beq[result_is_zero#]
 * @endcode
 *
 * @param out_c   Register written with higher of ina or inb
 * @param in_a    Register, value to compare to in_b
 * @param in_b    Register, value to compare to in_a
 *
 * Use in_b for the parameter most likely to be the highest.
 *
 * @condition_codes N,Z set based on result in out_c
 *
 * @instruction_cnt 4
 *
 * Cycles:
 *  For 3 different parameters:
 *   4 if in_a <= in_b, else 5
 *  Where in_a or in_b is the same as out_c:
 *   5 if out_c already lowest, else 4
 */
#macro limit_max(out_c, in_a, in_b)

    #if (streq(out_c, in_a))

        alu[--, out_c, -, in_b]
        bgt[done#], defer[1]
        alu[--, --, B, out_c]

        alu[out_c, --, B, in_b]

    #elif (streq(out_c, in_b))

        alu[--, out_c, -, in_a]
        bgt[done#], defer[1]
        alu[--, --, B, out_c]

        alu[out_c, --, B, in_a]

    #else

        alu[--, in_b, -, in_a]
        bgt[done#], defer[1]
        alu[out_c, --, B, in_b]

        alu[out_c, --, B, in_a]

    #endif

done#:

#endm


/** Calculate maximum of two values using signed arithmetic.
 *
 * @b Example:
 * @code
 * .reg ioa, inb
 * immed[ioa,5]
 * immed[inb,6]
 * limit_max(ioa, inb)
 * @endcode
 *
 * @param io_a    Register, value to compare to in_b, and resultant max value
 * @param in_b    Register, value to compare to io_a
 *
 * @condition_codes Data dependent: assume they are clobbered
 *
 * @instruction_cnt 3
 */
#macro limit_max(io_a, in_b)

    alu[--, io_a, -, in_b]
    bgt[done#]
    alu[io_a, --, B, in_b]
done#:

#endm


/** Calculate maximum of two values using signed arithmetic.
 *
 * @b Example:
 * @code
 * .reg ioa, inb
 * immed[ioa,5]
 * immed[inb,6]
 * limit_max(ioa, inb)
 * beq[result_is_zero#]
 * @endcode
 *
 * @param io_a    Register, value to compare to in_b, and resultant max value
 * @param in_b    Register, value to compare to io_a
 *
 * @condition_codes N,Z set based on result in io_a
 *
 * @instruction_cnt 4
 */
#macro limit_max_cc(io_a, in_b)

    alu[--, io_a, -, in_b]
    bgt[done#], defer[1]
    alu[io_a, --, B, io_a]

    alu[io_a, --, B, in_b]
done#:

#endm


/** Calculate minimum of two values using unsigned arithmetic.
 *
 * @b Example:
 * @code
 * .reg minimum, ina, inb
 * immed[ina,5]
 * immed[inb,6]
 * limit_min_unsigned(minimum, ina, inb)
 * beq[result_is_zero#]
 * @endcode
 *
 * @param out_c   Register written with lower of ina or inb
 * @param in_a    Register, value to compare to in_b
 * @param in_b    Register, value to compare to in_a
 *
 * Use in_a for the parameter most likely to be the smallest.
 *
 * @condition_codes N,Z set based on result in out_c
 *
 * @instruction_cnt 4
 *
 * Cycles:
 *  For 3 different parameters:
 *   4 if in_a <= in_b, else 5
 *  Where in_a or in_b is the same as out_c:
 *   5 if out_c already lowest, else 4
 */
#macro limit_min_unsigned(out_c, in_a, in_b)

    #if (streq(out_c, in_a))

        alu[--, out_c, -, in_b]
        blo[done#], defer[1]
        alu[--, --, B, out_c]

        alu[out_c, --, B, in_b]

    #elif (streq(out_c, in_b))

        alu[--, out_c, -, in_a]
        blo[done#], defer[1]
        alu[--, --, B, out_c]

        alu[out_c, --, B, in_a]

    #else

        alu[--, in_b, -, in_a]
        blo[done#], defer[1]
        alu[out_c, --, B, in_b]

        alu[out_c, --, B, in_a]

    #endif

done#:

#endm


/** Calculate minimum of two values using unsigned arithmetic.
 *
 * @b Example:
 * @code
 * .reg ioa, inb
 * immed[ioa,5]
 * immed[inb,6]
 * limit_min_unsigned(ioa, inb)
 * @endcode
 *
 * @param io_a    Register, value to compare to in_b, and resultant min value
 * @param in_b    Register, value to compare to io_a
 *
 * @condition_codes Data dependent: assume they are clobbered
 *
 * @instruction_cnt 3
 *
 */
#macro limit_min_unsigned(io_a, in_b)

    alu[--, io_a, -, in_b]
    blo[done#]
    alu[io_a, --, B, in_b]
done#:

#endm


/** Calculate minimum of two values using unsigned arithmetic.
 *
 * @b Example:
 * @code
 * .reg ioa, inb
 * immed[ioa,5]
 * immed[inb,6]
 * limit_min_unsigned_cc(ioa, inb)
 * beq[result_is_zero#]
 * @endcode
 *
 * @param io_a    Register, value to compare to in_b, and resultant min value
 * @param in_b    Register, value to compare to io_a
 *
 * @condition_codes N,Z set based on result in io_a
 *
 * @instruction_cnt 4
 */
#macro limit_min_unsigned_cc(io_a, in_b)

    alu[--, io_a, -, in_b]
    blo[done#], defer[1]
    alu[--, --, B, io_a]

    alu[io_a, --, B, in_b]
done#:

#endm


/** Calculate maximum of two values using unsigned arithmetic.
 *
 * @b Example:
 * @code
 * .reg maximum, ina, inb
 * immed[ina,5]
 * immed[inb,6]
 * limit_max_unsigned(maximum, ina, inb)
 * beq[result_is_zero#]
 * @endcode
 *
 * @param out_c   Register written with higher of ina or inb
 * @param in_a    Register, value to compare to in_b
 * @param in_b    Register, value to compare to in_a
 *
 * Use in_b for the parameter most likely to be the highest.
 *
 * @condition_codes N,Z set based on result in out_c
 *
 * @instruction_cnt 4
 *
 * Cycles:
 *  For 3 different parameters:
 *   4 if in_a <= in_b, else 5
 *  Where in_a or in_b is the same as out_c:
 *   5 if out_c already lowest, else 4
 */
#macro limit_max_unsigned(out_c, in_a, in_b)

    #if (streq(out_c, in_a))

        alu[--, out_c, -, in_b]
        bhs[done#], defer[1]
        alu[--, --, B, out_c]

        alu[out_c, --, B, in_b]

    #elif (streq(out_c, in_b))

        alu[--, out_c, -, in_a]
        bhs[done#], defer[1]
        alu[--, --, B, out_c]

        alu[out_c, --, B, in_a]

    #else

        alu[--, in_b, -, in_a]
        bhs[done#], defer[1]
        alu[out_c, --, B, in_b]

        alu[out_c, --, B, in_a]

    #endif

done#:

#endm


/** Calculate maximum of two values using unsigned arithmetic.
 *
 * @b Example:
 * @code
 * .reg ioa, inb
 * immed[ioa,5]
 * immed[inb,6]
 * limit_max_unsigned(ioa, inb)
 * @endcode
 *
 * @param io_a    Register, value to compare to in_b, and resultant max value
 * @param in_b    Register, value to compare to io_a
 *
 * @condition_codes Data dependent: assume they are clobbered
 *
 * @instruction_cnt 3
 */
#macro limit_max_unsigned(io_a, in_b)

    alu[--, io_a, -, in_b]
    bhs[done#]
    alu[io_a, --, B, in_b]
done#:

#endm


/** Calculate maximum of two values using unsigned arithmetic.
 *
 * @b Example:
 * @code
 * .reg ioa, inb
 * immed[ioa,5]
 * immed[inb,6]
 * limit_max_unsigned_cc(ioa, inb)
 * beq[result_is_zero#]
 * @endcode
 *
 * @param io_a    Register, value to compare to in_b, and resultant max value
 * @param in_b    Register, value to compare to io_a
 *
 * @condition_codes N,Z set based on result in io_a
 *
 * @instruction_cnt 4
 */
#macro limit_max_unsigned_cc(io_a, in_b)

    alu[--, io_a, -, in_b]
    bhs[done#], defer[1]
    alu[--, --, B, io_a]

    alu[io_a, --, B, in_b]
done#:

#endm


/** Calculate size of first chunk of work.
 *
 * @b Example:
 * @code
 * #define_eval BOUNDARY 8
 * .reg chunk_size, size, address
 * move(size,200)
 * move(address,0x12340004)
 * limit_align_first_chunk(chunk_size, size, BOUNDARY, addr)
 * @endcode
 *
 * @param out_chunk  Returned size of first chunk of work, so that it
 *                   will not cross a boundary ( mulitple of in_align)
 * @param in_size    Total work size
 * @param in_align   Alignment boundary, must be power of 2
 * @param in_address Start address of work
 *
 * Returned size (out_chunk ) will not cross alignment boundary even if total
 * size is less than the alignment size
 *
 * Works with unsigned values
 *
 * Cycles: 6/7 if in_align constant, else 7/8
 *
 */
#macro limit_align_first_chunk(out_chunk, in_size, in_align, in_address)
.begin

    #if streq(out_chunk, in_size)
        #error "limit_align_first_chunk: out_chunk and in_size may not be the same register."
    #endif

    .reg _tmp_len

    // Calculate length up to the first in_align boundary
    #if (is_ct_const(in_align))
        alu_op[_tmp_len, in_address, AND, ((in_align) - 1)]
    #else
        alu[_tmp_len, in_align, -, 1]
        alu[_tmp_len, in_address, AND, _tmp_len]
    #endif
    alu_op[out_chunk, in_align, -, _tmp_len]

    // Ensure that out_chunk is not more than in_size
    limit_min_unsigned(out_chunk, out_chunk, in_size)

.end
#endm

/** @}
 * @}
 */

#endif // _LIMIT_UC_
