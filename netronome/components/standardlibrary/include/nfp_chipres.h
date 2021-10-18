/*
 * Copyright (C) 2014-2015 Netronome Systems, Inc.  All rights reserved.
 *
 * File:        nfp_chipres.h
 */

#ifndef __NFP_CHIPRES_H__
#define __NFP_CHIPRES_H__

#if defined(__NFP_LANG_MICROC)
    #ifndef NFP_LIB_ANY_NFCC_VERSION
        #if ((__NFP_LANG_MICROC < NFP_SW_VERSION(5, 0, 0, 0)) ||   \
            (__NFP_LANG_MICROC > NFP_SW_VERSION(6, 255, 255, 255)))
            #error "This standard library is not supported for the version of the SDK currently in use."
        #endif
    #endif

    #define _NFP_CHIPRES_ASM(x)  __asm { x }
    #define _NFP_CHIPRES_HAS_ISLAND _nfp_has_island
#elif defined(__NFP_LANG_ASM)
    #ifndef NFP_LIB_ANY_NFAS_VERSION
        #if ((__NFP_LANG_ASM < NFP_SW_VERSION(5, 0, 0, 0)) ||   \
            (__NFP_LANG_ASM > NFP_SW_VERSION(6, 255, 255, 255)))
            #error "This standard library is not supported for the version of the SDK currently in use."
        #endif
    #endif

    #define _NFP_CHIPRES_ASM(x) x
    #define _NFP_CHIPRES_HAS_ISLAND __NFP_HAS_ISLAND

#else
    #error "Unknown language"
#endif

/** @file nfp_chipres.h NFP generic resources
 * @addtogroup nfp_chipres NFP generic resources
 * @{
 * @name NFP generic resources
 * @{
 */

#if defined(__NFP_IS_3200)

    /** @name Memory Unit Resources
     *  @{
     *
     * The following resource pools are declared (if the relevant island is
     * available for the target chip):
     * * dram_queues global 1024
     *
     * There are also predefined macros to aid allocation of these
     * resources. Note that using these predefined macros to allocate
     * from the resource pools will produce symbols that cannot be
     * checked with is_rt_const() due to the way the preprocessor
     * works.
     *
     */
    _NFP_CHIPRES_ASM(.declare_resource dram_queues global 1024)

    /** Allocate a queue array index number from dram_queues.
     * @param _name_    Name for the allocated resource symbol
     * @param _scope_   Scope with which to allocate the resource symbol
     */
    #if defined(__NFP_LANG_MICROC)
    #define DRAM_QUEUE_ALLOC(_name_, _scope_) \
        _NFP_CHIPRES_ASM(.alloc_resource _name_ dram_queues _scope_ 1)
    #elif defined(__NFP_LANG_ASM)
        #macro DRAM_QUEUE_ALLOC(_name_, _scope_)
            .alloc_resource _name_ dram_queues _scope_ 1
        #endm
    #endif

    /** @}
     */

#elif defined(__NFP_IS_6000) || defined(__NFP_IS_3800)

    /** @name External Memory Unit Resources
     *  @{
     *
     * The following resource pools are declared (if the relevant island is
     * available for the target chip):
     * * emem0_queues global 1024
     * * emem1_queues global 1024
     * * emem2_queues global 1024
     *
     * There are also predefined macros to aid allocation of these
     * resources. Note that using these predefined macros to allocate
     * from the resource pools will produce symbols that cannot be
     * checked with is_rt_const() due to the way the preprocessor
     * works.
     *
     */

    /** Allocate a queue array index number from an EMEM queue pool.
     * @param _name_    Name for the allocated resource symbol
     * @param _scope_   Scope with which to allocate the resource symbol
     * @param _resourcepool_ Resource pool from which to allocate.
     */
    #if defined(__NFP_LANG_MICROC)
        #define EMEM_QUEUE_ALLOC(_name_, _scope_, _resourcepool_) \
            _NFP_CHIPRES_ASM(.alloc_resource _name_ _resourcepool_ _scope_ 1)
    #elif defined(__NFP_LANG_ASM)
        #macro  EMEM_QUEUE_ALLOC(_name_, _scope_, _resourcepool_)
            .alloc_resource _name_ _resourcepool_ _scope_ 1
        #endm
    #endif

    #if _NFP_CHIPRES_HAS_ISLAND("emem0")
        _NFP_CHIPRES_ASM(.declare_resource emem0_queues global 1024)

        /** Allocate a queue array index number from emem0_queues.
         * @param _name_    Name for the allocated resource symbol
         * @param _scope_   Scope with which to allocate the resource symbol
         */
        #define EMEM0_QUEUE_ALLOC(_name_, _scope_) \
            EMEM_QUEUE_ALLOC(_name_, _scope_, emem0_queues)
    #endif

    #if _NFP_CHIPRES_HAS_ISLAND("emem1")
        _NFP_CHIPRES_ASM(.declare_resource emem1_queues global 1024)

        /** Allocate a queue array index number from emem1_queues.
         * @param _name_    Name for the allocated resource symbol
         * @param _scope_   Scope with which to allocate the resource symbol
         */
        #define EMEM1_QUEUE_ALLOC(_name_, _scope_) \
            EMEM_QUEUE_ALLOC(_name_, _scope_, emem1_queues)
    #endif

    #if _NFP_CHIPRES_HAS_ISLAND("emem2")
        _NFP_CHIPRES_ASM(.declare_resource emem2_queues global 1024)

        /** Allocate a queue array index number from emem2_queues.
         * @param _name_    Name for the allocated resource symbol
         * @param _scope_   Scope with which to allocate the resource symbol
         */
        #define EMEM2_QUEUE_ALLOC(_name_, _scope_) \
            EMEM_QUEUE_ALLOC(_name_, _scope_, emem2_queues)
    #endif

    /** @}
     */

#else
    #error "Unknown chip target"
#endif

/** @name CLS Resources
 *  @{
 * The following resource pools are declared:
 * * cls_rings island 16
 * * cls_apsignals island (NFP-32xx: 8, other: 16)
 * * cls_apfilters island 16
 *
 * There are also predefined macros to aid allocation of these
 * resources. Note that using these predefined macros to allocate
 * from the resource pools will produce symbols that cannot be
 * checked with is_rt_const() due to the way the preprocessor
 * works.
 */
_NFP_CHIPRES_ASM(.declare_resource cls_rings island 16)

/** Allocate a ring number from cls_rings.
 * @param _name_    Name for the allocated resource symbol
 * @param _scope_   Scope with which to allocate the resource symbol
 */
#if defined(__NFP_LANG_MICROC)
    #define CLS_RING_ALLOC(_name_, _scope_) \
        _NFP_CHIPRES_ASM(.alloc_resource _name_ cls_rings _scope_ 1)
#elif defined(__NFP_LANG_ASM)
    #macro CLS_RING_ALLOC(_name_, _scope_)
        .alloc_resource _name_ cls_rings _scope_ 1
    #endm
#endif


#if defined(__NFP_IS_3200)
    _NFP_CHIPRES_ASM(.declare_resource cls_apsignals island 8)
#else
    _NFP_CHIPRES_ASM(.declare_resource cls_apsignals island 16)
#endif

/** Allocate an autopush signal number from cls_apsignals.
 * @param _name_    Name for the allocated resource symbol
 * @param _scope_   Scope with which to allocate the resource symbol
 */
#if defined(__NFP_LANG_MICROC)
    #define CLS_APSIGNAL_ALLOC(_name_, _scope_) \
        _NFP_CHIPRES_ASM(.alloc_resource _name_ cls_apsignals _scope_ 1)
#elif defined(__NFP_LANG_ASM)
    #macro CLS_APSIGNAL_ALLOC(_name_, _scope_)
        .alloc_resource _name_ cls_apsignals _scope_ 1
    #endm
#endif

_NFP_CHIPRES_ASM(.declare_resource cls_apfilters island 16)

/** Allocate an autopush filter status monitor number from cls_apfilters.
 * @param _name_    Name for the allocated resource symbol
 * @param _scope_   Scope with which to allocate the resource symbol
 */
#if defined(__NFP_LANG_MICROC)
    #define CLS_APFILTER_ALLOC(_name_, _scope_) \
        _NFP_CHIPRES_ASM(.alloc_resource _name_ cls_apfilters _scope_ 1)
#elif defined(__NFP_LANG_ASM)
    #macro CLS_APFILTER_ALLOC(_name_, _scope_)
        .alloc_resource _name_ cls_apfilters _scope_ 1
    #endm
#endif

/** Allocate an autopush signal filter status monitor number
 * from cls_apsignals and cls_apfilters at the same offset.
 * @param _name_sig_    Name for the allocated signal number
 * @param _name_filter_    Name for the allocated filter number
 * @param _scope_   Scope with which to allocate the resource symbol
 */
#if defined(__NFP_LANG_MICROC)
/*    #define CLS_APSIGFILTER_ALLOC(_name_sig_, _name_filter_, _scope_) \
        _NFP_CHIPRES_ASM(.alloc_resource _name_sig_ cls_apsignals _scope_ 1) \
        _NFP_CHIPRES_ASM(.alloc_resource _name_filter_ cls_apfilters+_name_sig_ _scope_ 1)
*/
    #define CLS_APSIGFILTER_ALLOC(_name_sig_, _name_filter_, _scope_) \
        _NFP_CHIPRES_ASM(Not yet supported)

#elif defined(__NFP_LANG_ASM)
    #macro CLS_APSIGFILTER_ALLOC(_name_sig_, _name_filter_, _scope_)
        .alloc_resource _name_sig_ cls_apsignals _scope_ 1
        .alloc_resource _name_filter_ cls_apfilters+_name_sig_ _scope_ 1
    #endm
#endif

/** @}
 */

/** @name PCIe Resources
 *  @{
 * The following resource pools are declared per PCIe island:
 * * pcieX_cpp2pcie_bars global 8
 * * pcieX_dma_cfg_regs global 16
 * * pcieX_topci_hi global 64
 * * pcieX_topci_med global 64
 * * pcieX_topci_lo global 128
 * * pcieX_frompci_hi global 64
 * * pcieX_frompci_med global 64
 * * pcieX_frompci_lo global 128
 *
 * There are also predefined macros to aid allocation of these resources.
 * Each DMA queue resource entitles the user to add one DMA to the
 * appropriate DMA queue.  Once the user has received notification that the
 * DMA has completed, they can add another DMA to the queue.  The specific
 * "address" allocated by the linker is not meaningful for DMA queue
 * resources; the resources simply ensure that the total number of DMAs that
 * can be issued against a given queue will not cause it to overflow.
 *
 * The pcieX_cpp2pcie_bars resource allows a user to configure the assigned
 * CPP2PCIe BAR for use with pcie[read/write] commands.  The address allocated
 * is the BAR 0..7 that can be used.
 *
 * The pcieX_dma_cfg_regs resource allows a user to configure and use the
 * associated DMA configuration register with a DMA.  Note that the 16 DMA
 * configuration registers are stored as odd/even pairs within 32bit CSRs.
 * Users must either ensure that they do not disturb the other register within
 * their pair or allocate both.
 */

/** Allocate DMA queue slots
 * @param _name_    Name for the DMA slot allocation
 * @param _scope_   Scope with which to allocate the resource symbol
 * @param _isl_     PCIe island (0..3) from which to allocate
 * @param _q_       DMA queue from which to allocate
 * @param _num_     Number of slots for this allocation
 *
 * Valid options for the _q_ parameter are:
 * frompci_hi
 * frompci_med
 * frompci_lo
 * topci_hi
 * topci_med
 * topci_lo
 */

#define HAS_PCIE_DMA_GEN_RC

#if defined(__NFP_LANG_MICROC)
    #define PCIE_DMA_ALLOC_IND(_name_, _scope_, _isl_, _q_, _num_) \
        _NFP_CHIPRES_ASM(.alloc_resource _name_ pcie##_isl_##_##_q_ \
                         _scope_ _num_)
    #define PCIE_DMA_ALLOC(_name_, _scope_, _isl_, _q_, _num_)  \
        PCIE_DMA_ALLOC_IND(_name_, _scope_, _isl_, _q_, _num_)
#elif defined(__NFP_LANG_ASM)
    #macro PCIE_DMA_ALLOC(_name_, _scope_, _isl_, _q_, _num_)
        .alloc_resource _name_ pcie/**/_isl_/**/_/**/_q_ _scope_ _num_
    #endm
#endif

/** Allocate CPP2PCIE BAR Resources
 * @param _name_    Name for the BAR slot allocation
 * @param _scope_   Scope with which to allocate the resource symbol
 * @param _isl_     PCIe island (0..3) from which to allocate
 * @param _num_     Number of slots for this allocation
 * @param _off_     Desired memory offset for the resource symbol
 */

#define HAS_PCIE_C2P_BAR_GEN_RC

#if defined(__NFP_LANG_MICROC)
    #define PCIE_C2P_BAR_ALLOC_IND(_name_, _scope_, _isl_, _num_)           \
        _NFP_CHIPRES_ASM(.alloc_resource _name_ pcie##_isl_##_cpp2pcie_bars \
                        _scope_ _num_)
    #define PCIE_C2P_BAR_ALLOC(_name_, _scope_, _isl_, _num_)   \
        PCIE_C2P_BAR_ALLOC_IND(_name_, _scope_, _isl_, _num_)
#elif defined(__NFP_LANG_ASM)
    #macro PCIE_C2P_BAR_ALLOC(_name_, _scope_, _isl_, _num_)
        .alloc_resource _name_ pcie/**/_isl_/**/_cpp2pcie_bars _scope_ _num_
    #endm
#endif

#if defined(__NFP_LANG_MICROC)
    #define PCIE_C2P_BAR_ALLOC_OFF_IND(_name_, _scope_, _isl_, _off_, _num_) \
        _NFP_CHIPRES_ASM(.alloc_resource _name_                         \
                        pcie##_isl_##_cpp2pcie_bars##+_off_ _scope_ _num_)
    #define PCIE_C2P_BAR_ALLOC_OFF(_name_, _scope_, _isl_, _off_, _num_)    \
        PCIE_C2P_BAR_ALLOC_OFF_IND(_name_, _scope_, _isl_, _off_, _num_)
#elif defined(__NFP_LANG_ASM)
    #macro PCIE_C2P_BAR_ALLOC_OFF(_name_, _scope_, _isl_, _off_, _num_)
        .alloc_resource _name_ pcie/**/_isl_/**/_cpp2pcie_bars/**/+_off_ _scope_ _num_
    #endm
#endif

/** Allocate DMA Configuration Resources
 * @param _name_    Name for the DMA Configuration allocation
 * @param _scope_   Scope with which to allocate the resource symbol
 * @param _isl_     PCIe island (0..3) from which to allocate
 * @param _num_     Number of slots for this allocation
 * @param _off_     Desired memory offset for the resource symbol
 */

#define HAS_PCIE_DMA_CFG_GEN_RC

#if defined(__NFP_LANG_MICROC)
    #define PCIE_DMA_CFG_ALLOC_IND(_name_, _scope_, _isl_, _num_)          \
        _NFP_CHIPRES_ASM(.alloc_resource _name_ pcie##_isl_##_dma_cfg_regs \
                        _scope_ _num_)
    #define PCIE_DMA_CFG_ALLOC(_name_, _scope_, _isl_, _num_)   \
        PCIE_DMA_CFG_ALLOC_IND(_name_, _scope_, _isl_, _num_)
#elif defined(__NFP_LANG_ASM)
    #macro PCIE_DMA_CFG_ALLOC(_name_, _scope_, _isl_, _num_)
        .alloc_resource _name_ pcie/**/_isl_/**/_dma_cfg_regs _scope_ _num_
    #endm
#endif

#if defined(__NFP_LANG_MICROC)
    #define PCIE_DMA_CFG_ALLOC_OFF_IND(_name_, _scope_, _isl_, _off_, _num_) \
        _NFP_CHIPRES_ASM(.alloc_resource _name_                             \
                        pcie##_isl_##_dma_cfg_regs##+_off_ _scope_ _num_)
    #define PCIE_DMA_CFG_ALLOC_OFF(_name_, _scope_, _isl_, _off_, _num_)    \
        PCIE_DMA_CFG_ALLOC_OFF_IND(_name_, _scope_, _isl_, _off_, _num_)
#elif defined(__NFP_LANG_ASM)
    #macro PCIE_DMA_CFG_ALLOC_OFF(_name_, _scope_, _isl_, _off_, _num_)
        .alloc_resource _name_ pcie/**/_isl_/**/_dma_cfg_regs/**/+_off_ _scope_ _num_
    #endm
#endif

#if _NFP_CHIPRES_HAS_ISLAND("pcie0")
    _NFP_CHIPRES_ASM(.declare_resource pcie0_cpp2pcie_bars global 8)
    _NFP_CHIPRES_ASM(.declare_resource pcie0_dma_cfg_regs global 16)

    _NFP_CHIPRES_ASM(.declare_resource pcie0_topci_hi global 64)
    _NFP_CHIPRES_ASM(.declare_resource pcie0_topci_med global 64)
    _NFP_CHIPRES_ASM(.declare_resource pcie0_topci_lo global 128)
    _NFP_CHIPRES_ASM(.declare_resource pcie0_frompci_hi global 64)
    _NFP_CHIPRES_ASM(.declare_resource pcie0_frompci_med global 64)
    _NFP_CHIPRES_ASM(.declare_resource pcie0_frompci_lo global 128)
#endif

#if _NFP_CHIPRES_HAS_ISLAND("pcie1")
    _NFP_CHIPRES_ASM(.declare_resource pcie1_cpp2pcie_bars global 8)
    _NFP_CHIPRES_ASM(.declare_resource pcie1_dma_cfg_regs global 16)

    _NFP_CHIPRES_ASM(.declare_resource pcie1_topci_hi global 64)
    _NFP_CHIPRES_ASM(.declare_resource pcie1_topci_med global 64)
    _NFP_CHIPRES_ASM(.declare_resource pcie1_topci_lo global 128)
    _NFP_CHIPRES_ASM(.declare_resource pcie1_frompci_hi global 64)
    _NFP_CHIPRES_ASM(.declare_resource pcie1_frompci_med global 64)
    _NFP_CHIPRES_ASM(.declare_resource pcie1_frompci_lo global 128)
#endif

#if _NFP_CHIPRES_HAS_ISLAND("pcie2")
    _NFP_CHIPRES_ASM(.declare_resource pcie2_cpp2pcie_bars global 8)
    _NFP_CHIPRES_ASM(.declare_resource pcie2_dma_cfg_regs global 16)

    _NFP_CHIPRES_ASM(.declare_resource pcie2_topci_hi global 64)
    _NFP_CHIPRES_ASM(.declare_resource pcie2_topci_med global 64)
    _NFP_CHIPRES_ASM(.declare_resource pcie2_topci_lo global 128)
    _NFP_CHIPRES_ASM(.declare_resource pcie2_frompci_hi global 64)
    _NFP_CHIPRES_ASM(.declare_resource pcie2_frompci_med global 64)
    _NFP_CHIPRES_ASM(.declare_resource pcie2_frompci_lo global 128)
#endif

#if _NFP_CHIPRES_HAS_ISLAND("pcie3")
    _NFP_CHIPRES_ASM(.declare_resource pcie3_cpp2pcie_bars global 8)
    _NFP_CHIPRES_ASM(.declare_resource pcie3_dma_cfg_regs global 16)

    _NFP_CHIPRES_ASM(.declare_resource pcie3_topci_hi global 64)
    _NFP_CHIPRES_ASM(.declare_resource pcie3_topci_med global 64)
    _NFP_CHIPRES_ASM(.declare_resource pcie3_topci_lo global 128)
    _NFP_CHIPRES_ASM(.declare_resource pcie3_frompci_hi global 64)
    _NFP_CHIPRES_ASM(.declare_resource pcie3_frompci_med global 64)
    _NFP_CHIPRES_ASM(.declare_resource pcie3_frompci_lo global 128)
#endif

/** @}
 */

/** @}
 * @}
 */

#endif /* __NFP_CHIPRES_H__ */
