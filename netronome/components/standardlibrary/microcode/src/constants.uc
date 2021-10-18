/*
 * Copyright (C) 2009-2013 Netronome Systems, Inc.  All rights reserved.
 */

#ifndef __CONSTANTS_UC__
#define __CONSTANTS_UC__

#ifndef NFP_LIB_ANY_NFAS_VERSION
    #if (!defined(__NFP_TOOL_NFAS) ||                       \
        (__NFP_TOOL_NFAS < NFP_SW_VERSION(5, 0, 0, 0)) ||   \
        (__NFP_TOOL_NFAS > NFP_SW_VERSION(6, 1, 0, 255)))
        #error "This standard library is not supported for the version of the SDK currently in use."
    #endif
#endif

/** @file constants.uc Common and Global Constants
 * @addtogroup constants Common and Global Constants
 * @{
 *
 * @name Common and Global Constants
 * @{
 *
 */


#ifndef MEM_WD_COUNT_MIN_1
    #define MEM_WD_COUNT_MIN_1 FALSE
#endif
/** Control word count passed to memory access macros.
 * @def MEM_WD_COUNT_MIN_1
 * @b Values:
 * @arg @c TRUE: All memory access macros must be called with
 *               <tt>word count = actual number of word - 1</tt>.
 *               This will help save 1 cycle.
 * @arg @c FALSE: All memory access macros are called with <tt>word count = actual number of word</tt>
 *
 * @note Default is @c FALSE.
 */


// Simplify robust compatibility checks on 4.6.2+ nfas predefines
/// @cond INTERNAL_MACROS
#if (IS_NFPTYPE(__NFP3200) && \
     (defined(__NFP_INDIRECT_REF_FORMAT_V1) || (!defined(__NFP_INDIRECT_REF_FORMAT_IXP) && !defined(__NFP_INDIRECT_REF_FORMAT_V2))))
    #ifndef __NFP_INDIRECT_REF_FORMAT_V1
        .indirect_ref_mode nfp_v1
        #define __NFP_INDIRECT_REF_FORMAT_V1
    #endif
#elif (IS_NFPTYPE(__NFP6000) && \
     (defined(__NFP_INDIRECT_REF_FORMAT_V2) || (!defined(__NFP_INDIRECT_REF_FORMAT_IXP) && !defined(__NFP_INDIRECT_REF_FORMAT_V1))))
    #ifndef __NFP_INDIRECT_REF_FORMAT_V2
        .indirect_ref_mode nfp_v2
        #define __NFP_INDIRECT_REF_FORMAT_V2
    #endif
#else
    #ifndef __NFP_INDIRECT_REF_FORMAT_IXP
        #if (defined(__NFP3200) && IS_NFPTYPE(__NFP3200))
            .indirect_ref_mode ixp_compatible
        #endif
        #define __NFP_INDIRECT_REF_FORMAT_IXP
    #endif
#endif

#if (IS_NFPTYPE(__NFP3200) && \
     (defined(__NFP_SINGLE_DRAM_SIGNAL) || !defined(__NFP_DUAL_DRAM_SIGNAL)))
    #ifndef __NFP_SINGLE_DRAM_SIGNAL
        .dram_signals 1
        #define __NFP_SINGLE_DRAM_SIGNAL
    #endif
#elif (IS_NFPTYPE(__NFP6000) && \
     (defined(__NFP_SINGLE_DRAM_SIGNAL) || !defined(__NFP_DUAL_DRAM_SIGNAL)))
     /* .dram_signals not available for nfp6000 */
#else
    #ifndef __NFP_DUAL_DRAM_SIGNAL
        #if (defined(__NFP3200) && IS_NFPTYPE(__NFP3200))
            .dram_signals 2
        #endif
        #define __NFP_DUAL_DRAM_SIGNAL
    #endif
#endif

#if (IS_NFPTYPE(__NFP3200) && \
     (defined(__NFP_THIRD_PARTY_ADDRESSING_40_BIT) || !defined(__NFP_THIRD_PARTY_ADDRESSING_32_BIT)))
    #ifndef __NFP_THIRD_PARTY_ADDRESSING_40_BIT
        #define __NFP_THIRD_PARTY_ADDRESSING_40_BIT
    #endif
#elif (IS_NFPTYPE(__NFP6000) && \
     (defined(__NFP_THIRD_PARTY_ADDRESSING_40_BIT) || !defined(__NFP_THIRD_PARTY_ADDRESSING_32_BIT)))
    #ifndef __NFP_THIRD_PARTY_ADDRESSING_40_BIT
        #define __NFP_THIRD_PARTY_ADDRESSING_40_BIT
    #endif
#else
    #ifndef __NFP_THIRD_PARTY_ADDRESSING_32_BIT
        #define __NFP_THIRD_PARTY_ADDRESSING_32_BIT
    #endif
#endif



#if ( (!defined(__PREPROC32)) && (!defined(__PREPROC64)))
    #define __PREPROC32
#endif
/// @endcond

#define MAX_IMMEDIATE       0xFF

#define MAX_IMMEDIATE_ADDR  0x7f /**< Only 7 bits available in memory operations */

#define SIG_NONE 0
#define ___ 0
#define optimize_mem unordered

#define FALSE 0
#define TRUE 1
#define UNALLOCATED 0

#define FREELIST0 0
#define FREELIST1 1
#define FREELIST2 2
#define FREELIST3 3
#define FREELIST4 4
#define FREELIST5 5
#define FREELIST6 6
#define FREELIST7 7

#define XFRINDEX0 0
#define XFRINDEX1 1
#define XFRINDEX2 2
#define XFRINDEX3 3
#define XFRINDEX4 4
#define XFRINDEX5 5
#define XFRINDEX6 6
#define XFRINDEX7 7

#define QWCOUNT1    1
#define QWCOUNT2    2
#define QWCOUNT3    3
#define QWCOUNT4    4
#define QWCOUNT5    5
#define QWCOUNT6    6
#define QWCOUNT7    7
#define QWCOUNT8    8

#define QWOFFSET0   0
#define QWOFFSET1   1
#define QWOFFSET2   2
#define QWOFFSET3   3
#define QWOFFSET4   4
#define QWOFFSET5   5
#define QWOFFSET6   6
#define QWOFFSET7   7

#define LWCOUNT1    1
#define LWCOUNT2    2
#define LWCOUNT3    3
#define LWCOUNT4    4
#define LWCOUNT5    5
#define LWCOUNT6    6
#define LWCOUNT7    7
#define LWCOUNT8    8

#define LWOFFSET0   0
#define LWOFFSET1   1
#define LWOFFSET2   2
#define LWOFFSET3   3
#define LWOFFSET4   4
#define LWOFFSET5   5
#define LWOFFSET6   6
#define LWOFFSET7   7

#define BYTEOFFSET0 0
#define BYTEOFFSET1 1
#define BYTEOFFSET2 2
#define BYTEOFFSET3 3
#define BYTEOFFSET4 4
#define BYTEOFFSET5 5
#define BYTEOFFSET6 6
#define BYTEOFFSET7 7
#define BYTEOFFSET8 8
#define BYTEOFFSET9 9
#define BYTEOFFSET10 10
#define BYTEOFFSET11 11
#define BYTEOFFSET12 12
#define BYTEOFFSET13 13
#define BYTEOFFSET14 14
#define BYTEOFFSET15 15
#define BYTEOFFSET16 16
#define BYTEOFFSET17 17
#define BYTEOFFSET18 18
#define BYTEOFFSET19 19
#define BYTEOFFSET20 20
#define BYTEOFFSET21 21
#define BYTEOFFSET22 22
#define BYTEOFFSET23 23
#define BYTEOFFSET24 24
#define BYTEOFFSET25 25
#define BYTEOFFSET26 26
#define BYTEOFFSET27 27
#define BYTEOFFSET28 28
#define BYTEOFFSET29 29
#define BYTEOFFSET30 30
#define BYTEOFFSET31 31

/** @name Packet Actions
 *  @{
 */
#define PKT_DENY        0x00
#define PKT_PERMIT      0x01
#define PKT_QUEUE_TO_CORE   0x02
/** @}
 */


/** @name OPERAND_SIZE parameter for Multiply32 / Multiply64
 *  @{
 */
#define_eval OP_SIZE_8X24   1
#define_eval OP_SIZE_16X16  2
#define_eval OP_SIZE_16X32  3
#define_eval OP_SIZE_32X32  4
/** @}
 */


/** @name Memory size conversions
 *  @{
 */
#define BYTES_PER_LW        4
#define BYTES_PER_QW        8
/** @}
 */


/// @cond INTERNAL_MACROS
#ifdef __NFP_INDIRECT_REF_FORMAT_IXP
    #define _DRAM_INDIRECT_REF_BYTE_MASK_OV_OFFSET   20
    #define _DRAM_INDIRECT_REF_BYTE_MASK_OFFSET      12

    #define _REF_CNT_OV_OFFSET   25
    #define _REF_CNT_OFFSET      21

    #define _TBUF_ADDR_OV_OFFSET 4
    #define _TBUF_ADDR_OFFSET    5
#endif
/// @endcond


/** @name Memory unit addressing locality modes
 *  @{
 */
#define MU_LOCALITY_HIGH                0
#define MU_LOCALITY_LOW                 1
#define MU_LOCALITY_DIRECT_ACCESS       2
#define MU_LOCALITY_DISCARD_AFTER_READ  3
/** @}
 */


/** @}
 * @}
 */


#endif /* __CONSTANTS_UC__ */
