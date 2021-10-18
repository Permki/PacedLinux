/*
 * Copyright (C) 2009-2013 Netronome Systems, Inc.  All rights reserved.
 *
 * File:        aggregate.uc
 */

#ifndef _AGGREGATE_UC_
#define _AGGREGATE_UC_

#ifndef NFP_LIB_ANY_NFAS_VERSION
    #if (!defined(__NFP_TOOL_NFAS) ||                       \
        (__NFP_TOOL_NFAS < NFP_SW_VERSION(5, 0, 0, 0)) ||   \
        (__NFP_TOOL_NFAS > NFP_SW_VERSION(6, 1, 0, 255)))
        #error "This standard library is not supported for the version of the SDK currently in use."
    #endif
#endif

/** @file aggregate.uc Aggregate Operation Macros
 * @addtogroup aggregate Aggregate Operations
 * @{
 *
 * @name Aggregate Operation Macros
 * @{
 *
 * These macros perform operations on groups of registers
 */

// API Quick Reference:
//
//
// aggregate_zero(dst, COUNT)
// aggregate_zero(dst, DST_IDX, COUNT)
// aggregate_set(dst, scalar_src, COUNT)
// aggregate_set(dst, DST_IDX, scalar_src, COUNT)
// aggregate_copy(dst, src, COUNT)
// aggregate_directive(DIRECTIVE, xfer, COUNT)
// aggregate_directive(DIRECTIVE, xfer, START, COUNT)

/** Zero Aggregate.
 *
 * @b Example:
 * @code
 * .reg write $stat_wr[16]
 * aggregate_zero($stat_wr, 16)
 * @endcode
 *
 * @param dst   Destination GPRs or write transfer registers to zero
 * @param COUNT CONSTANT number of registers to set to zero
 *
 */
#macro aggregate_zero(dst, COUNT)

    aggregate_set(dst, 0, 0, COUNT)

#endm

/** Zero Aggregate using Index
 *
 * @b Example:
 * @code
 * .reg write $stat_wr[16]
 * aggregate_zero($stat_wr, 8, 2)
 * @endcode
 *
 * @param dst     Destination GPRs or write transfer registers to zero
 * @param DST_IDX Index of 1st register to zero
 * @param COUNT   CONSTANT number of registers to set to zero
 *
 */
#macro aggregate_zero(dst, DST_IDX, COUNT)

    aggregate_set(dst, DST_IDX, 0, COUNT)

#endm

/** Set value to Aggregate.
 *
 * @b Example:
 * @code
 * .reg write $stat_wr[16]
 * aggregate_set($stat_wr, 0x55, 16)
 * @endcode
 *
 * @param dst        Destination GPRs or write transfer registers to set to value
 * @param scalar_src value to set to registers
 * @param COUNT      CONSTANT number of registers to set to value
 *
 */
#macro aggregate_set(dst, scalar_src, COUNT)

    aggregate_set(dst, 0, scalar_src, COUNT)

#endm

/** Set value to Aggregate using Index
 *
 * @b Example:
 * @code
 * .reg write $stat_wr[16]
 * aggregate_set($stat_wr, 0x55, 16)
 * @endcode
 *
 * @param dst        Destination GPRs or write transfer registers to set to value
 * @param DST_IDX    Index of 1st register to set value to
 * @param scalar_src value to set to registers
 * @param COUNT      CONSTANT number of registers to set to value
 *
 */
#macro aggregate_set(dst, DST_IDX, scalar_src, COUNT)

    #ifdef _AGGREGATE_DST
        #warning "_AGGREGATE_DST is being redefined"
    #endif

    #ifdef _AGGREGATE_LOOP
        #warning "_AGGREGATE_LOOP is being redefined"
    #endif

    #if (strstr("|++|--|", '|DST_IDX|'))
        #define _AGGREGATE_DST $
    #else
        #define_eval _AGGREGATE_DST (DST_IDX)
    #endif

    #define _AGGREGATE_LOOP 0
    #while (_AGGREGATE_LOOP < COUNT)

        #if (is_ct_const(_AGGREGATE_DST))

            move(dst[_AGGREGATE_DST], scalar_src)
            #define_eval _AGGREGATE_DST (_AGGREGATE_DST + 1)

        #else

            move(dst/**/DST_IDX, scalar_src)

        #endif

        #define_eval _AGGREGATE_LOOP (_AGGREGATE_LOOP + 1)

    #endloop
    #undef _AGGREGATE_LOOP
    #undef _AGGREGATE_DST

#endm // aggregate_set()

/** Copy from Aggregate source to Aggregate destination
 *
 * @b Example:
 * @code
 * .reg $src[8]
 * .reg $dst[8]
 * aggregate_copy($dst, $src, 8)
 * @endcode
 *
 * @param dst        Destination GPRs or write transfer registers for copy
 * @param src        Source GPRs or read transfer registers for copy
 * @param COUNT      CONSTANT number of registers to copy
 *
 */
#macro aggregate_copy(dst, src, COUNT)

    aggregate_copy(dst, 0, src, 0, COUNT)

#endm

/** Copy from Aggregate source to Aggregate destination using indeces
 *
 * @b Example:
 * @code
 * .reg $src[8]
 * .reg $dst[8]
 * aggregate_copy($dst, 2, $src, 4, 2)
 * @endcode
 *
 * @param dst        Destination GPRs or write transfer registers for copy
 * @param DST_IDX    Index of 1st destination register to copy to
 * @param src        Source GPRs or read transfer registers for copy
 * @param SRC_IDX    Index of 1st source register to copy from
 * @param COUNT      CONSTANT number of registers to copy
 *
 */
#macro aggregate_copy(dst, DST_IDX, src, SRC_IDX, COUNT)

    #ifdef _AGGREGATE_DST
        #warning "_AGGREGATE_DST is being redefined"
    #endif

    #ifdef _AGGREGATE_SRC
        #warning "_AGGREGATE_SRC is being redefined"
    #endif

    #ifdef _AGGREGATE_LOOP
        #warning "_AGGREGATE_LOOP is being redefined"
    #endif

    #if (strstr("|++|--|", '|DST_IDX|'))
        #define _AGGREGATE_DST $
    #else
        #define_eval _AGGREGATE_DST (DST_IDX)
    #endif

    #if (strstr("|++|--|", '|SRC_IDX|'))
        #define _AGGREGATE_SRC $
    #else
        #define_eval _AGGREGATE_SRC (SRC_IDX)
    #endif

    #define _AGGREGATE_LOOP 0
    #while (_AGGREGATE_LOOP < COUNT)

        #if (is_ct_const(_AGGREGATE_DST) && is_ct_const(_AGGREGATE_SRC))

            alu[dst[_AGGREGATE_DST], --, B, src[_AGGREGATE_SRC]]
            #define_eval _AGGREGATE_DST (_AGGREGATE_DST + 1)
            #define_eval _AGGREGATE_SRC (_AGGREGATE_SRC + 1)

        #elif (!is_ct_const(_AGGREGATE_DST) && is_ct_const(_AGGREGATE_SRC))

            alu[dst/**/DST_IDX, --, B, src[_AGGREGATE_SRC]]
            #define_eval _AGGREGATE_SRC (_AGGREGATE_SRC + 1)

        #elif (is_ct_const(_AGGREGATE_DST) && !is_ct_const(_AGGREGATE_SRC))

            alu[dst[_AGGREGATE_DST], --, B, src/**/SRC_IDX]
            #define_eval _AGGREGATE_DST (_AGGREGATE_DST + 1)

        #else // (!is_ct_const(_AGGREGATE_DST) && !is_ct_const(_AGGREGATE_SRC))

            alu[dst/**/DST_IDX, --, B, src/**/SRC_IDX]

        #endif

        #define_eval _AGGREGATE_LOOP (_AGGREGATE_LOOP + 1)

    #endloop
    #undef _AGGREGATE_LOOP
    #undef _AGGREGATE_DST
    #undef _AGGREGATE_SRC

#endm // aggregate_copy()

/** Generate Assembler Directive for a Register Aggregate
 *
 * @b Example:
 * @code
 * .reg $xfr[8]
 * aggregate_directive(.set, $xfr, 8)
 * @endcode
 *
 * @param DIRECTIVE  directive string to generate on transfer registers
 *                   one of .set, .set_wr, .set_rd, .use, .use_wr, .use_rd, .init
 * @param xfer       Transfer registers on which to generate directive
 * @param COUNT      CONSTANT number of registers to generate directive on
 *
 */
#macro aggregate_directive(DIRECTIVE, xfer, COUNT)

    aggregate_directive(DIRECTIVE, xfer, 0, COUNT)

#endm

/** Generate Assembler Directive for a Register Aggregate using index
 *
 * @b Example:
 * @code
 * .reg $xfr[8]
 * aggregate_directive(.set, $xfr, 4, 2)
 * @endcode
 *
 * @param DIRECTIVE  directive string to generate on transfer registers
 *                   one of .set, .set_wr, .set_rd, .use, .use_wr, .use_rd, .init
 * @param xfer       Transfer registers on which to generate directive
 * @param START      START index of 1st transfer register to generate directive on
 * @param COUNT      CONSTANT number of registers to generate directive on
 *
 */
#macro aggregate_directive(DIRECTIVE, xfer, START, COUNT)

    #ifdef _AGGREGATE_LOOP
        #warning "_AGGREGATE_LOOP is being redefined"
    #endif

    #define_eval _AGGREGATE_LOOP START
    #while (_AGGREGATE_LOOP < (START + COUNT))

        #if (streq('DIRECTIVE', '.set_wr'))
            .set_wr xfer[_AGGREGATE_LOOP]
        #elif (streq('DIRECTIVE', '.set_rd'))
            .set_rd xfer[_AGGREGATE_LOOP]
        #elif (streq('DIRECTIVE', '.set'))
            .set xfer[_AGGREGATE_LOOP]
        #elif (streq('DIRECTIVE', '.use_wr'))
            .use_wr xfer[_AGGREGATE_LOOP]
        #elif (streq('DIRECTIVE', '.use_rd'))
            .use_rd xfer[_AGGREGATE_LOOP]
        #elif (streq('DIRECTIVE', '.use'))
            .use xfer[_AGGREGATE_LOOP]
        #elif (streq('DIRECTIVE', '.init'))
            .init xfer[_AGGREGATE_LOOP] 0
        #else
            #error "Unknown directive" DIRECTIVE
        #endif

        #define_eval _AGGREGATE_LOOP (_AGGREGATE_LOOP + 1)

    #endloop

    #undef _AGGREGATE_LOOP

#endm // aggregate_directive()

/** @}
 * @}
 */

#endif // _AGGREGATE_UC_
