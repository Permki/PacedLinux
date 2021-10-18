/*
 * Copyright (C) 2009-2013 Netronome Systems, Inc.  All rights reserved.
 */

#ifndef __INDIRECT_REF_UC__
#define __INDIRECT_REF_UC__

#ifndef NFP_LIB_ANY_NFAS_VERSION
    #if (!defined(__NFP_TOOL_NFAS) ||                       \
        (__NFP_TOOL_NFAS < NFP_SW_VERSION(5, 0, 0, 0)) ||   \
        (__NFP_TOOL_NFAS > NFP_SW_VERSION(6, 1, 0, 255)))
        #error "This standard library is not supported for the version of the SDK currently in use."
    #endif
#endif

/** @file indirect_ref.uc Indirect Reference Mode Macros
 * @addtogroup indref_mode Indirect Reference Mode
 * @{
 *
 * @name Indirect Reference Mode Macros
 * @{
 *
 * For advanced use only.
 *
 */


/**
 * Wrapper macro to set indirect reference mode
 *
 * @param __MODE    Chip mode: "LEGACY" or "EXTENDED" or "NORMAL"
 * @deprecated
 */
#macro set_indirect_ref_mode(__MODE)
    #warning "This macro has been deprecated and will be removed in a future release."
    #if (streq(__MODE, "LEGACY"))
        set_legacy_indirect_ref_mode()
    #else
        #if (streq(__MODE, "EXTENDED"))
            set_extended_indirect_ref_mode()
        #else
            #if ( streq(__MODE, "NORMAL") && (IS_NFPTYPE(__NFP6000) || IS_NFPTYPE(__NFP3800)))
                set_normal_indirect_ref_mode()
            #else
                #error "Unknown or invalid indirect ref mode specified."
            #endif
        #endif
    #endif
#endm

/**
 * Set legacy (IXP) indirect reference mode.
 * For NFP6000, 'legacy' mode is equivalent to NFP3200 Extended Mode
 *
 * @deprecated
 * @note This should only be used in rare cases where
 *       the ME loader does not initialize the MISC_CONTROL bits
 */
#macro set_legacy_indirect_ref_mode
    #warning "This macro has been deprecated and will be removed in a future release."
    #if (defined(__NFP_INDIRECT_REF_FORMAT_IXP))
        #warning "The loader will initialize IXP indirect reference mode. This macro call does nothing."
    #elif (defined(__NFP_INDIRECT_REF_FORMAT_NFP))
        #error "The file is being assembled for NFP indirect reference mode using -indirect_ref_format_nfp."
    #elif (IS_NFPTYPE(__NFP3200))
        #warning "** INDIRECT REFERENCE MODE SHOULD BE INITIALIZED BY ME LOADER **"
        .begin

            .reg r
            local_csr_rd[MISC_CONTROL]
            immed[r, 0]
            alu[r, r, AND~, 1, <<3] // clear bit to enable legacy indirect references
            local_csr_wr[MISC_CONTROL, r]
            nop
            nop
            nop

        .end
    #elif (IS_NFPTYPE(__NFP6000) || IS_NFPTYPE(__NFP3800))
        #warning "** INDIRECT REFERENCE MODE SHOULD BE INITIALIZED BY ME LOADER **"
        .begin

            .reg r
            local_csr_rd[MISC_CONTROL]
            immed[r, 0]
            alu[r, r, OR, 1, <<3] // set bit to enable legacy indirect references
            local_csr_wr[MISC_CONTROL, r]
            nop
            nop
            nop

        .end
    #endif

#endm


/**
 * Set extended (NFP) indirect reference mode
 *
 * @deprecated
 * @note This should only be used in rare cases where
 *       the ME loader does not initialize the MISC_CONTROL bits
 */
#macro set_extended_indirect_ref_mode
    #warning "This macro has been deprecated and will be removed in a future release."
    #if (defined(__NFP_INDIRECT_REF_FORMAT_NFP))
        #warning "The loader will initialize NFP indirect reference mode. This macro call does nothing."
    #elif (defined(__NFP_INDIRECT_REF_FORMAT_IXP))
        #error "The file is being assembled for IXP indirect reference mode using -indirect_ref_format_ixp."
    #elif ( IS_NFPTYPE(__NFP3200) || IS_NFPTYPE(__NFP6000) || IS_NFPTYPE(__NFP3800))
        #warning "** INDIRECT REFERENCE MODE SHOULD BE INITIALIZED BY ME LOADER **"
        .begin

            .reg r
            local_csr_rd[MISC_CONTROL]
            immed[r, 0]
            alu[r, r, OR, 1, <<3] // set bit to enable extended indirect references
            local_csr_wr[MISC_CONTROL, r]
            nop
            nop
            nop

        .end
    #else
        #error "Non-NFP chip cannot use NFP indirect reference mode."
    #endif
#endm

/**
 * Set normal (NFP6000) indirect reference mode
 *
 * @note This should only be used in rare cases where
 *       the ME loader does not initialize the MISC_CONTROL bits
 */
#macro set_normal_indirect_ref_mode
    #if (defined(__NFP_INDIRECT_REF_FORMAT_V1))
        #error "The file is being assembled for v1 indirect reference mode using -indirect_ref_format_v1."
    #elif (defined(__NFP_INDIRECT_REF_FORMAT_IXP))
        #error "The file is being assembled for IXP indirect reference mode using -indirect_ref_format_ixp."
    #elif (IS_NFPTYPE(__NFP6000) || IS_NFPTYPE(__NFP3800))
        #warning "** INDIRECT REFERENCE MODE SHOULD BE INITIALIZED BY ME LOADER **"
        .begin

            .reg r
            local_csr_rd[MISC_CONTROL]
            immed[r, 0]
            alu[r, r, AND~, 1, <<3] // clr bit to enable nfp6000 normal indirect references
            local_csr_wr[MISC_CONTROL, r]
            nop
            nop
            nop

        .end
    #else
        #error "Non-NFP6000 chip cannot use NFP6000 normal  indirect reference mode."
    #endif
#endm


/** @}
 * @}
 */

#endif /* __INDIRECT_REF_UC__ */
