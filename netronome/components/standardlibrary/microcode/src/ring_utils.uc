/*
 * Copyright (C) 2009-2014 Netronome Systems, Inc.  All rights reserved.
 */

#ifndef __RING_UTILS_UC__
#define __RING_UTILS_UC__

#ifndef NFP_LIB_ANY_NFAS_VERSION
    #if (!defined(__NFP_TOOL_NFAS) ||                       \
        (__NFP_TOOL_NFAS < NFP_SW_VERSION(5, 0, 0, 0)) ||   \
        (__NFP_TOOL_NFAS > NFP_SW_VERSION(6, 1, 0, 255)))
        #error "This standard library is not supported for the version of the SDK currently in use."
    #endif
#endif

#include <xbuf.uc>
#include <constants.uc>

/** @file ring_utils.uc Ring Utility Macros
 * @addtogroup ring_utils Ring Utility
 * @{
 *
 * @name Ring Utility Macros
 * @{
 * List of macros specific to memory ring utilities
 */


#ifndef _CLS_RINGS_CSR_OFFSET_
    #ifdef _CS_RINGS_CSR_OFFSET_
        #define _CLS_RINGS_CSR_OFFSET_ _CS_RINGS_CSR_OFFSET_
    #else
        #define _CLS_RINGS_CSR_OFFSET_   0x10000
    #endif
#endif  /* _CLS_RINGS_CSR_OFFSET_ */

#ifndef K
    #define K   (1024)
#endif  /* K */

#ifndef M
    #define M   (1024 * 1024)
#endif  /* K */



/** Set up a type 2 ring with variable q-descriptor and ring bases addresses
 *
 * @param _in_addr_hi           Upper byte of the 40-bit address of the ring descriptor in emem0, emem1 or emem2
 *                              Note that it is located in the lower byte of _in_addr_hi
 * @param _in_q_desc_addr_lo    32-bit address identifying the location of the descriptor
 * @param _in_ring_base_addr_lo 32-bit address identifying the location of the rings in emem0, emem1 or emem2
 * @param _in_ring_num          Number of emem rings to configure, valid values 0-1023, GPR or constant value
 * @param _IN_SIZE_LW           size of ring in long words, must be between 512 and 16M and a power of 2
 * @param _IN_Q_LOC             Queue locality mode: @n
 *                              @arg @c MU_LOCALITY_HIGH
 *                              @arg @c MU_LOCALITY_LOW
 *                              @arg @c MU_LOCALITY_DIRECT_ACCESS
 *                              @arg @c MU_LOCALITY_DISCARD_AFTER_READ
 *
 * This macro is also available with fixed q-descriptor and ring addresses (5 parameters)
 *
 */
#macro ru_emem_ring_setup(_in_addr_hi, _in_q_desc_addr_lo,  _in_ring_base_addr_lo, _in_ring_num, _IN_SIZE_LW, _IN_Q_LOC)
    #if (!defined(__NFP_INDIRECT_REF_FORMAT_V2))
        #error "ru_emem_ring_setup: maccro requires NFP6000 indirect reference format (-indirect_ref_format_nfp6000)."
    #endif

    #if ((_IN_SIZE_LW < 512) | (_IN_SIZE_LW > (16 * M)))
        #error "ru_emem_ring_setup: Invalid ring size specified (Ring #" _IN_SIZE_LW"): Ring size must be in range of [512 16M]."
    #endif

    #if (_IN_Q_LOC > 3)
        #error "ru_emem_ring_setup: Invalid _IN_Q_LOC value, must be a 2-bit value."
    #endif

    // The encoding is pretty much log2(_IN_SIZE_LW/512) [512 is the minimum size]
    // So make sure all sizes is a power of 2
    #if (_IN_SIZE_LW & (_IN_SIZE_LW - 1) != 0)
        #error "ru_emem_ring_setup: Invalid ring size specified (Ring #" _in_ring_num"): Ring size must be a power of 2."
    #endif

    .begin
    .reg r_temp, r_addr
    .sig ring_sig
    xbuf_alloc($ring_desc, 4, read_write)

    #define_eval    _RING_SIZE_ENC_     log2(_IN_SIZE_LW >>9)

    alu[r_addr, --, b, _in_addr_hi, <<24]

    alu[r_temp, _in_ring_base_addr_lo, and~, 0x3f, <<26]
    alu[$ring_desc0, r_temp, or, _RING_SIZE_ENC_, <<28]
    alu[$ring_desc1, r_temp, or, 2]  // 2: type 2 ring descriptor
    immed[$ring_desc2, (_IN_Q_LOC << 14), <<16]
    alu[$ring_desc3, --, b, 0]

    mem[write, $ring_desc0, r_addr, <<8, _in_q_desc_addr_lo, 2], sig_done[ring_sig]
    ctx_arb[ring_sig]

    mem[read, $ring_desc0, r_addr, <<8, _in_q_desc_addr_lo, 2], sig_done[ring_sig]
    ctx_arb[ring_sig]

    #if (is_ct_const(_in_ring_num))
        #if ((_in_ring_num < 0) | (_in_ring_num > 1023))
            #error "ru_emem_ring_setup: _in_ring_num must be between 0 and 1023 (inclusive)"
        #endif

        immed[r_temp, _in_ring_num, <<16]
    #else
        immed[r_temp, 0x3FF, <<16]                  // ring number cannot exceded 1023
        alu[r_temp, r_temp, and, _in_ring_num, <<16]
    #endif

    alu[r_temp, r_temp, or, 0x08]               // PREV_ALU OVE_DATA field is bits 5:3, must be 0b001
    alu[--, --, b, r_temp]                      // r_temp in PREV_ALU
    mem[rd_qdesc, --, r_addr, <<8, _in_q_desc_addr_lo], indirect_ref

    // The purpose of this instruction is to make sure rd_qdesc has completed internal processing
    // before possibly continuing with put/get instructions that require a valid descriptor
    // The descriptor is pushed into the ring_desc0 xfer regs but not used
    mem[push_qdesc, $ring_desc0, r_addr, <<8, r_temp], sig_done[ring_sig]
    ctx_arb[ring_sig]
    .end
#endm


/** Set up a type 2 ring with constant q-descriptor and ring bases addresses
 *
 * @param _IN_Q_DESC_ADDR       40-bit address identifying the location of the ring descriptor in emem0, emem1 or emem2
 * @param _IN_RING_BASE_ADDR    40-bit address identifying the location of the rings in emem0, emem1 or emem2
 * @param _in_ring_num          Number of emem rings to configure, valid values 0-1023, GPR or constant value
 * @param _IN_SIZE_LW           size of ring in long words, must be between 512 and 16M and a power of 2
 * @param _IN_Q_LOC             Queue locality mode: @n
 *                              @arg @c MU_LOCALITY_HIGH
 *                              @arg @c MU_LOCALITY_LOW
 *                              @arg @c MU_LOCALITY_DIRECT_ACCESS
 *                              @arg @c MU_LOCALITY_DISCARD_AFTER_READ
 *
 * @b Example:
 * Create ring in emem island 26
 * @code
 *  #define EMEM_RING_NUM           12
 *  #define EMEM_RING_SIZE          512
 *
 *  .alloc_mem RING_BASE_ADDR       i26.emem island EMEM_RING_SIZE EMEM_RING_SIZE
 *  .alloc_mem Q_DESC_BASE_ADDR     i26.emem island 16
 *
 *  .reg $xdout[2], $xdin[2]
 *  .xfer_order $xdout, $xdin
 *  .sig g1, g2
 *
 *  ru_emem_ring_setup(Q_DESC_BASE_ADDR, RING_BASE_ADDR, EMEM_RING_NUM, EMEM_RING_SIZE, 0)
 *  ru_emem_ring_put(RING_BASE_ADDR, $xdout[0], EMEM_RING_NUM, 2, g1]
 *  ru_emem_ring_get(RING_BASE_ADDR, $xdin[0], EMEM_RING_NUM, 2, g2]
 * @endcode
 *
 * This macro is also available with dynamic q-descriptor and ring addresses (6 parameters)
 *
 */
#macro ru_emem_ring_setup(_IN_Q_DESC_ADDR, _IN_RING_BASE_ADDR, _in_ring_num, _IN_SIZE_LW, _IN_Q_LOC)
.begin
    #if (!defined(__NFP_INDIRECT_REF_FORMAT_V2))
        #error "ru_emem_ring_setup: maccro requires NFP6000 indirect reference format (-indirect_ref_format_nfp6000)."
    #endif

    #if ((_IN_SIZE_LW < 512) | (_IN_SIZE_LW > (16 * M)))
        #error "ru_emem_ring_setup: Invalid ring size specified (Ring #" _IN_SIZE_LW"): Ring size must be in range of [512 16M]."
    #endif

    #if (_IN_Q_LOC > 3)
        #error "ru_emem_ring_setup: Invalid _IN_Q_LOC value, must be a 2-bit value."
    #endif

    // The encoding is pretty much log2(_IN_SIZE_LW/512) [512 is the minimum size]
    // So make sure all sizes is a power of 2
    #if (_IN_SIZE_LW & (_IN_SIZE_LW - 1) != 0)
        #error "ru_emem_ring_setup: Invalid ring size specified (Ring #" _in_ring_num"): Ring size must be a power of 2."
    #endif

    .reg qdesc_addr, r_temp, r_addr
    .sig ring_sig
    xbuf_alloc($ring_desc, 4, read_write)

    #define_eval    _RING_SIZE_ENC_     log2(_IN_SIZE_LW >>9)

    immed32($ring_desc0, (_IN_RING_BASE_ADDR & 0x03ffFFFC) | _RING_SIZE_ENC_ << 28)
    immed32($ring_desc1, ((_IN_RING_BASE_ADDR & 0xffffFFFC) | 0x2)) // 2: type 2 ring descriptor
    immed32($ring_desc2, (((_IN_Q_LOC & 0x3) << 30) | (((_IN_RING_BASE_ADDR >> 32) & 0x3) << 24)))
    alu[$ring_desc3, --, b, 0]

    immed40(r_addr, qdesc_addr, _IN_Q_DESC_ADDR)

    mem[write, $ring_desc0, r_addr, <<8, qdesc_addr, 2], sig_done[ring_sig]
    ctx_arb[ring_sig]

    mem[read, $ring_desc0, r_addr, <<8, qdesc_addr, 2], sig_done[ring_sig]
    ctx_arb[ring_sig]

    #if (is_ct_const(_in_ring_num))
        #if ((_in_ring_num < 0) | (_in_ring_num > 1023))
            #error "ru_emem_ring_setup: _in_ring_num must be between 0 and 1023 (inclusive)"
        #endif

        immed[r_temp, _in_ring_num, <<16]
    #else
        immed[r_temp, 0x3FF, <<16]
        alu[r_temp, r_temp, and, _in_ring_num, <<16]
    #endif

    alu[r_temp, r_temp, or, 0x08]       // PREV_ALU OVE_DATA field is bits 5:3, must be 0b001
    alu[--, --, b, r_temp]              // r_temp in PREV_ALU
    mem[rd_qdesc, --, r_addr, <<8, qdesc_addr], indirect_ref

    // The purpose of this instruction is to make sure rd_qdesc has completed internal processing
    // before possibly continuing with put/get instructions that require a valid descriptor
    // The descriptor is pushed into the ring_desc0 xfer regs but not used
    mem[push_qdesc, $ring_desc0, r_addr, <<8, _in_ring_num], sig_done[ring_sig]
    ctx_arb[ring_sig]
.end
#endm   // ru_emem_ring_setup


/** Add entries to the tail of the circular buffer in emem based on parameters passed to the macro
 *
 * @param _in_ring_base_addr    40-bit address identifying the location of the rings in emem0, emem1 or emem2
 * @param _in_xfer_reg          Entries to add to the circular buffer
 * @param _in_ring_num          Select the ring number to add to, between 0-1023
 * @param _IN_REF_CNT           Number of 32-bit words to add to the circular buffer, must be between 1-16
 * @param _in_sig_name          Signal to use for ring operation
 *
 * See ru_emem_ring_setup() macro for implementation details
 * Some compile time error checking is done.
 *
 */
#macro ru_emem_ring_put(_in_ring_base_addr, _in_xfer_reg, _in_ring_num, _IN_REF_CNT, _in_sig_name)
    #if (!defined(__NFP_INDIRECT_REF_FORMAT_V2))
        #error "ru_emem_ring_put: maccro requires NFP6000 indirect reference format (-indirect_ref_format_nfp6000)."
    #endif

    #if (!strstr(_in_xfer_reg, $))
        #error "ru_emem_ring_put: _in_xfer_reg must be xfer register"
    #endif

    #if (!is_ct_const(_IN_REF_CNT))
        #error "ru_emem_ring_put: _IN_REF_CNT must be constant."
    #endif

    #if (_IN_REF_CNT > 16)
        #error "ru_emem_ring_put: _IN_REF_CNT cannot exceed 16."
    #endif

    .begin
    .reg src_op1, src_op2

    // For the addressing only upper byte of 40-bit address is needed
    // If _in_ring_base_addr is ct or rt constant then upper byte is bits [39:32], these must move to [31:24]
    // If _in_ring_base_addr is variable then the upper byte is bits [7:0]
    #if (is_ct_const(_in_ring_base_addr) || is_rt_const(_in_ring_base_addr))
        immed[src_op1, ((_in_ring_base_addr >>24) & 0xFFFF), <<16]
    #else
        alu[src_op1, --, b, _in_ring_base_addr, <<24]
    #endif

    #if (is_rt_const(_in_ring_num))
        #if ((_in_ring_num < 0) | (_in_ring_num > 1023))
            #error "ru_emem_ring_put: _in_ring_num must be between 0 and 1023 (inclusive)"
        #endif
        immed[src_op2, _in_ring_num]
    #else
        immed[src_op2, 0x3FF]   // ring numbers limited to 1023
        alu[src_op2, _in_ring_num, and, src_op2]
    #endif

    #if (_IN_REF_CNT > 8)
        .reg cnt

        immed[cnt, (_IN_REF_CNT - 1)]   // override zero indexed
        alu[--, 0x80, OR, cnt, <<8]
        mem[put, _in_xfer_reg, src_op1, <<8, src_op2, max_/**/_IN_REF_CNT], indirect_ref, sig_done[_in_sig_name]
    #else
        mem[put, _in_xfer_reg, src_op1, <<8, src_op2, _IN_REF_CNT], sig_done[_in_sig_name]
    #endif

    ctx_arb[_in_sig_name]
    .end
#endm


/** Remove entries from the head of the circular buffer in emem based on parameters passed to the macro
 *
 * @param _in_ring_base_addr    40-bit address identifying the location of the rings in emem0, emem1 or emem2
 * @param _out_xfer_reg         Entries to remove from the circular buffer
 * @param _in_ring_num          Select the ring number to remove from, between 0-1023
 * @param _IN_REF_CNT           Number of 32-bit words to remove from the circular buffer must between 1-16
 * @param _in_sig_name          Signal to use for ring operation
 *
 * See ru_emem_ring_setup() macro for implementation details
 * Some compile time error checking is done.
 *
 */
#macro ru_emem_ring_get(_in_ring_base_addr, _out_xfer_reg, _in_ring_num, _IN_REF_CNT, _in_sig_name)
    #if (!defined(__NFP_INDIRECT_REF_FORMAT_V2))
        #error "ru_emem_ring_get: maccro requires NFP6000 indirect reference format (-indirect_ref_format_nfp6000)."
    #endif

    #if (!strstr(_out_xfer_reg, $))
        #error "ru_emem_ring_get: _out_xfer_reg must be xfer register"
    #endif

    #if (!is_ct_const(_IN_REF_CNT))
        #error "ru_emem_ring_get: _IN_REF_CNT must be constant."
    #endif

    #if (_IN_REF_CNT > 16)
        #error "ru_emem_ring_get: _IN_REF_CNT cannot exceed 16."
    #endif

    .begin
    .reg src_op1, src_op2

    // For the addressing only upper byte of 40-bit address is needed
    // If _in_ring_base_addr is ct or rt constant then upper byte is bits [39:32], these must move to [31:24]
    // If _in_ring_base_addr is variable then the upper byte is bits [7:0]
    #if (is_ct_const(_in_ring_base_addr) || is_rt_const(_in_ring_base_addr))
        immed[src_op1, ((_in_ring_base_addr >>24) & 0xFFFF), <<16]
    #else
        alu[src_op1, --, b, _in_ring_base_addr, <<24]
    #endif

    #if (is_rt_const(_in_ring_num))
        #if ((_in_ring_num < 0) | (_in_ring_num > 1023))
            #error "ru_emem_ring_get: _in_ring_num must be between 0 and 1023 (inclusive)"
        #endif
        immed[src_op2, _in_ring_num]
    #else
        immed[src_op2, 0x3FF]   // ring numbers limited to 1023
        alu[src_op2, _in_ring_num, and, src_op2]
    #endif

    #if (_IN_REF_CNT > 8)
        .reg cnt

        immed[cnt, (_IN_REF_CNT - 1)]   // override zero indexed
        alu[--, 0x80, OR, cnt, <<8]
        mem[get, _out_xfer_reg, src_op1, <<8, src_op2, max_/**/_IN_REF_CNT], indirect_ref, sig_done[_in_sig_name]
    #else
        mem[get, _out_xfer_reg, src_op1, <<8, src_op2, _IN_REF_CNT], sig_done[_in_sig_name]
    #endif

    ctx_arb[_in_sig_name[0]]
    .end
#endm


/**
 * Next-neighbor ring initialization
 *
 * @param NN_EMPTY_THRESHOLD  Threshold when NN_Empty asserts.
 *                            Valid values are 0-3.
 *
 */
#macro ru_nn_ring_init(NN_EMPTY_THRESHOLD)
.begin

    .reg ctx_enable
    local_csr_rd[ctx_enables]
    immed[ctx_enable, 0]
    alu_shf[ctx_enable, ctx_enable, AND~, 3, <<18]
    alu_shf[ctx_enable, ctx_enable, OR, NN_EMPTY_THRESHOLD, <<18]
    local_csr_wr[ctx_enables, ctx_enable]
    local_csr_wr[nn_put, 0]
    local_csr_wr[nn_get, 0]
    alu[--, --, b, 0]
    alu[--, --, b, 0]
    alu[--, --, b, 0]

.end
#endm // _nn_ring_init




/** Set up a type 2 DRAM ring based on parameters passed to the macro.
 *
 * Some compile time error checking is done.
 *
 * @param _IN_RING_NUM_    Number of DRAM ring to configure
 * @param _IN_BASE_ADDR_   DRAM address where ring starts
 * @param _IN_SIZE_LW_     LW size of ring, must be between 512 and 16M and a power of 2
 * @param _IN_Q_LOC_       Queue locality mode: @n
 *                         @arg @c MU_LOCALITY_HIGH
 *                         @arg @c MU_LOCALITY_LOW
 *                         @arg @c MU_LOCALITY_DIRECT_ACCESS
 *                         @arg @c MU_LOCALITY_DISCARD_AFTER_READ
 * @param _IN_Q_PAGE_      Top two bits of the queue entry addresses.
 *
 */
#macro ru_dram_ring_setup(_IN_RING_NUM_, _IN_BASE_ADDR_, _IN_SIZE_LW_, _IN_Q_LOC_, _IN_Q_PAGE_)
    #if (!defined(__NFP_INDIRECT_REF_FORMAT_V1) && !defined(__NFP_INDIRECT_REF_FORMAT_V2))
        #error "This macro is only available in NFP indirect reference format mode."
    #endif

    .begin
        // First perform some error checking
        #if ((_IN_SIZE_LW_ < 512) | (_IN_SIZE_LW_ > (16 * M)))
            #error "ru_dram_ring_setup: Invalid ring size specified (Ring #" _IN_RING_NUM_"): Ring size must be in range of [512 16M]."
        #endif

        #if (_IN_Q_PAGE_ > 3)
            #error "ru_dram_ring_setup: Invalid q_page value, must be a 2-bit value."
        #endif

        #if (_IN_Q_LOC_ > 3)
            #error "ru_dram_ring_setup: Invalid q_loc value, must be a 2-bit value."
        #endif

        // The encoding is pretty much log2(_IN_SIZE_LW_/512) [512 is the minimum size]
        // So make sure all sizes is a power of 2
        #if (_IN_SIZE_LW_ & (_IN_SIZE_LW_ - 1) != 0)
            #error "ru_dram_ring_setup: Invalid ring size specified (Ring #" _IN_RING_NUM_"): Ring size must be a power of 2."
        #endif  /* _IN_SIZE_LW_ */

        #define_eval    _RING_SIZE_ENC_     log2(_IN_SIZE_LW_ >>9)

        // The ring must be aligned to the ring size
        #if (is_ct_const(_IN_BASE_ADDR_))
            #if (_IN_BASE_ADDR_ & ((_IN_SIZE_LW_ <<2) -  1))
                #error "ru_dram_ring_setup: Ring base address must be aligned based on the ring size  (Ring #" _IN_RING_NUM_")"
            #endif  /* _IN_BASE_ADDR_ */
        #elif (is_rt_const(_IN_BASE_ADDR_))
            .assert ((_IN_BASE_ADDR_ % (_IN_SIZE_LW_ <<2)) == 0) "ru_dram_ring_setup: Ring base address must be aligned based on the ring size"
        #endif

        // make sure the ring number is valid
        #if ((_IN_RING_NUM_ < 0) | (_IN_RING_NUM_ > 1023))
            #error "ru_dram_ring_setup: DRAM ring numbers must be between 0 and 1023 (inclusive)"
        #endif  /* _IN_RING_NUM_ */

        .reg    ring_addr
        .reg    ring_temp

        .reg    ring_setup[4]

        xbuf_alloc($ring_desc, 4, read_write)

        .sig    ring_sig

        move(ring_addr, _IN_BASE_ADDR_)

        move(ring_temp, _IN_BASE_ADDR_)
        alu[ring_setup[0], ring_temp, and~, 0x3F, <<26]              // clear top 4 bits and 2 reserved bits of addr for this word
        alu[ring_setup[0], ring_setup[0], or, _RING_SIZE_ENC_, <<28]// set the encoded ring size in those bits
        alu[ring_setup[1], ring_temp, or, 2]                        // 2 low order bits are the ring type

        immed[ring_setup[2], ((_IN_Q_LOC_ << 14) | (_IN_Q_PAGE_ << 8)), <<16] // count is zero

        alu[ring_setup[3], --, b, 0]

        alu[$ring_desc0, --, b, ring_setup[0]]
        alu[$ring_desc1, --, b, ring_setup[1]]
        alu[$ring_desc2, --, b, ring_setup[2]]
        alu[$ring_desc3, --, b, ring_setup[3]]

        mem[write, $ring_desc0, ring_addr, 0, 2], sig_done[ring_sig]
        ctx_arb[ring_sig]

        // read the data back so we can be sure it is in memory
        mem[read, $ring_desc0, ring_addr, 0, 2], sig_done[ring_sig]
        ctx_arb[ring_sig]

        // now we load the descriptor into the queue hardware
        #if (defined(__NFP_INDIRECT_REF_FORMAT_V2))
            #if is_ct_const(_IN_RING_NUM_)
                move(ring_temp, _IN_RING_NUM_)
                alu[ring_temp, --, b, ring_temp, <<16]
            #else
                alu[ring_temp, --, b, _IN_RING_NUM_, <<16]
            #endif  /* is_ct_const(_IN_RING_NUM_) */
        #else
            // use mode 0 for the indirect format
            ; NB: It is important to note that we are assuming the enhanced indirect format
            #if is_ct_const(_IN_RING_NUM_)
                move(ring_temp, _IN_RING_NUM_)
                alu[ring_temp, --, b, ring_temp, <<5]
            #else
                alu[ring_temp, --, b, _IN_RING_NUM_, <<5]
            #endif  /* is_ct_const(_IN_RING_NUM_) */
        #endif
        mem[rd_qdesc, --, ring_addr, 0], indirect_ref
        br[ru_dram_ring_setup_end#]


    ru_dram_ring_setup_end#:
        // clean up namespace
        xbuf_free($ring_desc)
        #undef  _RING_SIZE_ENC_

    .end
#endm // ru_dram_ring_setup


/** Set up a type 2 DRAM ring based on parameters passed to the macro.
 *
 * This is an overloaded macro which uses @c MU_LOCALITY_HIGH for
 * @c _IN_Q_LOC_ and @c 0 for @c _IN_Q_PAGE_.
 *
 * @param _IN_RING_NUM_    Number of DRAM ring to configure
 * @param _IN_BASE_ADDR_   DRAM address where ring starts
 * @param _IN_SIZE_LW_     LW size of ring, must be between 512 and 16M and a power of 2
 *
 */
#macro ru_dram_ring_setup(_IN_RING_NUM_, _IN_BASE_ADDR_, _IN_SIZE_LW_)
    ru_dram_ring_setup(_IN_RING_NUM_, _IN_BASE_ADDR_, _IN_SIZE_LW_, MU_LOCALITY_HIGH, 0)
#endm // ru_dram_ring_setup


/** Set up a SRAM ring Based on parameters passed into the macro.
 *
 * Some compile time error checking is done.
 *
 * @param _IN_RING_NUM_    Number of SRAM ring to configure
 * @param _IN_BASE_ADDR_   SRAM address where ring starts (channel value will be extracted)
 * @param _IN_SIZE_LW_     LW size of ring, must be between 512 and 64K and a power of 2
 *
 */
#macro ru_sram_ring_setup(_IN_RING_NUM_, _IN_BASE_ADDR_, _IN_SIZE_LW_)
.begin

    // First perform some error checking
    #if ((_IN_SIZE_LW_ < 512) | (_IN_SIZE_LW_ > (64 * K)))
        #error "ru_sram_ring_setup: Invalid ring size specified (Ring #" _IN_RING_NUM_"): Ring size must be in range of [512 64K]."
    #endif

    // The encoding is pretty much log2(_IN_SIZE_LW_/512) [512 is the minimum size]
    // So make sure all sizes is a power of 2
    #if (_IN_SIZE_LW_ & (_IN_SIZE_LW_ - 1) != 0)
        #error "ru_sram_ring_setup: Invalid ring size specified (Ring #" _IN_RING_NUM_"): Ring size must be a power of 2."
    #endif  /* _IN_SIZE_LW_ */

    #define_eval    _RING_SIZE_ENC_     log2(_IN_SIZE_LW_ >>9)

    // Extract the channel number and remove it from the base address
    #define_eval    _RING_CHAN_NUM_ ((_IN_BASE_ADDR_ >>30) & 3)
    #define_eval    _RING_BASE_ADDR_    (_IN_BASE_ADDR_ & ~(3<<30))

    // The ring must be aligned to the ring size
    #if (_RING_BASE_ADDR_ & ((_IN_SIZE_LW_ <<2) -  1))
        #error "ru_sram_ring_setup: Ring base address must be aligned based on the ring size  (Ring #" _IN_RING_NUM_")"
    #endif  /* _RING_BASE_ADDR_ */

    // make sure the ring number is valid
    #if ((_IN_RING_NUM_ < 0) | (_IN_RING_NUM_ > 63))
        #error "ru_sram_ring_setup: SRAM ring numbers must be between 0 and 63 (inclusive)"
    #endif  /* _IN_RING_NUM_ */

    .reg    ring_addr
    .reg    ring_temp

    .reg    ring_setup[4]

    xbuf_alloc($ring_desc, 4, read_write)

    .sig    ring_sig

    move(ring_addr, _IN_BASE_ADDR_)
    move(ring_temp, ( (1<<23)-1 ) )
    alu[ring_temp, ring_temp, and, ring_addr]
    alu[ring_temp, --, b, ring_temp, >>2]   // word address


    alu[ring_setup[0], ring_temp, or, _RING_SIZE_ENC_, <<29]    // head with ring size encoding
    alu[ring_setup[1], --, b, ring_temp]                        // tail
    alu[ring_setup[2], --, b, 0]                                // count is zero
    alu[ring_setup[3], --, b, 0]

    alu[$ring_desc0, --, b, ring_setup[0]]
    alu[$ring_desc1, --, b, ring_setup[1]]
    alu[$ring_desc2, --, b, ring_setup[2]]
    alu[$ring_desc3, --, b, ring_setup[3]]

    sram[write, $ring_desc0, ring_addr, 0, 4], sig_done[ring_sig]
    ctx_arb[ring_sig]

    // read the data back so we can be sure it is in memory
    sram[read, $ring_desc0, ring_addr, 0, 4], sig_done[ring_sig]
    ctx_arb[ring_sig]

    // now we load the descriptor into the queue hardware
    alu[ring_temp, --, b, _RING_CHAN_NUM_, <<30]
    alu[ring_temp, ring_temp, or, _IN_RING_NUM_, <<24]
    alu[ring_addr, ring_addr, and~, 3, <<30]            // take out channel info (it's already being added back in)
    alu[ring_addr, ring_temp, or, ring_addr, >>2]       // address must be word address
    sram[rd_qdesc_head, $ring_desc0, ring_addr, 0, 2], ctx_swap[ring_sig]
    sram[rd_qdesc_other, --, ring_addr, 0]
    br[ru_sram_ring_setup_end#]


ru_sram_ring_setup_end#:
    // clean up namespace
    xbuf_free($ring_desc)
    #undef  _RING_SIZE_ENC_
    #undef  _RING_CHAN_NUM_
    #undef  _RING_BASE_ADDR_

.end
#endm // ru_sram_ring_setup


/** Set up a single CLS ring based on parameters passed into the macro.
 *
 * Some compile time error checking is done.
 *
 * @param _IN_RING_NUM_    Number of CLS ring to configure
 * @param _IN_BASE_ADDR_   CLS address where ring starts
 * @param _IN_SIZE_LW_     LW size of ring, must be between 32 and 1024 and a power of 2
 *
 *
 */
#macro ru_cls_ring_setup(_IN_RING_NUM_, _IN_BASE_ADDR_, _IN_SIZE_LW_)
    #if (IS_IXPTYPE(__IXP28XX))
        #error "This macro is not available for IXP code."
    #endif

    .begin

        .reg    ring_setup

        .reg    ring_csr_addr
        .reg    $ring_base
        .reg    $ring_ptrs

        .sig    ring_base_sig
        .sig    ring_ptrs_sig
        // First perform some error checking
        // Verify that a valid ring size has been given
        #if ((_IN_SIZE_LW_ < 32) | (_IN_SIZE_LW_ > (1 * K)))
            #error "ru_cls_ring_setup: Invalid ring size specified (Ring #" _IN_RING_NUM_"): Ring size must be in range of [32 1K]."
        #endif

        // The encoding is pretty much log2(_IN_SIZE_LW_/32) [32 is the minimum size]
        // So make sure all sizes is a power of 2
        #if (_IN_SIZE_LW_ & (_IN_SIZE_LW_ - 1) != 0)
            #error "ru_cls_ring_setup: Invalid ring size specified (Ring #" _IN_RING_NUM_"): Ring size must be a power of 2."
        #endif  /* _IN_SIZE_LW_ */

        #define_eval    _RING_SIZE_ENC_     log2(_IN_SIZE_LW_ >>5)

        // The lower 7 bits of the base address is ignored, so make sure these are 0
        // for alignment purposes
        // However, since the next test covers this implicitly, we only need to perform
        // that
        // The ring must be aligned to the ring size

        #if (is_ct_const(_IN_BASE_ADDR_))
            #if (_IN_BASE_ADDR_ % (_IN_SIZE_LW_ <<2))
                #error "ru_cls_ring_setup: Ring base address must be aligned based on the ring size  (Ring #:" _IN_RING_NUM_")"
            #endif  /* _IN_BASE_ADDR_ */
        #elif (is_rt_const(_IN_BASE_ADDR_))
            .assert ((_IN_BASE_ADDR_ % (_IN_SIZE_LW_ <<2)) == 0) "ru_cls_ring_setup: Ring base address must be aligned based on the ring size"
        #endif
        // Specify which events we would like to report
        // Assuming ring fullness can be tested for, we don't want to report on that
        // For now, don't request any events
        #define_eval    _RING_REPORT_ENC_   0

        immed[ring_setup, (_IN_BASE_ADDR_ >> 7)]
        alu[ring_setup, ring_setup, or, _RING_SIZE_ENC_, <<16]
        alu[$ring_base, ring_setup, or, _RING_REPORT_ENC_, <<24]
        // all other fields in ring_base is set to 0 as specified in the DB
        immed[$ring_ptrs, 0] // always set to 0

        // setup the ring CSR address for the ring base info
        move(ring_csr_addr, _CLS_RINGS_CSR_OFFSET_)
        alu[ring_csr_addr, ring_csr_addr, or, _IN_RING_NUM_, <<3]
        cls[write, $ring_base, 0, ring_csr_addr, 1], sig_done[ring_base_sig]

        // setup the ring CSR address for the ring ptrs info
        move(ring_csr_addr, (_CLS_RINGS_CSR_OFFSET_ + 0x80))
        alu[ring_csr_addr, ring_csr_addr, or, _IN_RING_NUM_, <<3]
        cls[write, $ring_ptrs, 0, ring_csr_addr, 1], sig_done[ring_ptrs_sig]

        ctx_arb[ring_base_sig, ring_ptrs_sig]


    ru_cls_ring_setup_end#:
        // clean up namespace
        #undef  _RING_SIZE_ENC_
        #undef  _RING_REPORT_ENC_

    .end
#endm // ru_cls_ring_setup


/** Set up a single CTM ring based on parameters passed into the macro.
 *
 * Some compile time error checking is done.
 *
 * @param _IN_RING_NUM_    CONST, Number of CTM ring to configure, must be between 0 and 14
 * @param _IN_BASE_ADDR_   CONST, CTM address where ring starts, must be aligned to ring size
 * @param _IN_SIZE_LW_     CONST, LW size of ring, must be between  128 and 16*1024 and a power of 2
 * @param _IN_STATUS_      CONST, status generation control, either "FULL" or "EMPTY"
 *
 */
#macro ru_ctm_ring_setup(_IN_RING_NUM_, _IN_BASE_ADDR_, _IN_SIZE_LW_, _IN_STATUS_)
    #if (!(IS_IXPTYPE(__NFP6000) || IS_NFPTYPE(__NFP3800)))
        #error "This macro is only available on NFP6000."
    #endif

    .begin

        .sig    _s
        .reg    ring_setup
        .reg    ring_ctm_addr
        .reg    $xd[3]
        .xfer_order $xd

        // First perform some error checking

        // Verify that a valid ring number has been given
        #if ( _IN_RING_NUM_ > 14 )
            #error "ru_ctm_ring_setup: max ctm ring number is 14."
        #endif

        // Verify that a valid ring size has been given
        #if ((_IN_SIZE_LW_ < 128) | (_IN_SIZE_LW_ > (16 * K)))
            #error "ru_cls_ring_setup: Invalid ring size specified (Ring #" _IN_RING_NUM_"): Ring size must be in range of [128 16K]."
        #endif

        // The encoding is pretty much log2(_IN_SIZE_LW_/128) [128 is the minimum size]
        // So make sure all sizes is a power of 2
        #if (_IN_SIZE_LW_ & (_IN_SIZE_LW_ - 1) != 0)
            #error "ru_ctm_ring_setup: Invalid ring size specified (Ring #" _IN_RING_NUM_"): Ring size must be a power of 2."
        #endif  /* _IN_SIZE_LW_ */

        #define_eval    _RING_SIZE_ENC_     log2(_IN_SIZE_LW_ >>7)

        // The lower 9 bits of the base address is ignored, so make sure these are 0
        // for alignment purposes
        // However, since the next test covers this implicitly, we only need to perform
        // that
        // The ring must be aligned to the ring size

        #if (is_ct_const(_IN_BASE_ADDR_))
            #if (_IN_BASE_ADDR_ % (_IN_SIZE_LW_ <<2))
                #error "ru_ctm_ring_setup: Ring base address must be aligned based on the ring size  (Ring #:" _IN_RING_NUM_")"
            #endif  /* _IN_BASE_ADDR_ */
        #elif (is_rt_const(_IN_BASE_ADDR_))
            .assert ((_IN_BASE_ADDR_ % (_IN_SIZE_LW_ <<2)) == 0) "ru_cls_ring_setup: Ring base address must be aligned based on the ring size"
        #endif

        // Specify which status we want
        #if streq(_IN_STATUS_, 'FULL' )
            #define_eval _RING_STATUS_ENC_   1
        #else
            #define_eval _RING_STATUS_ENC_   0
        #endif

        #if (is_ct_const(_IN_BASE_ADDR_))
            #define_eval _IBA ((_IN_BASE_ADDR_ & 0x3FE00) >> 8)
            immed[ring_setup, _IBA, <<8]
            #undef _IBA
        #else
            immed[ring_setup, 0x3FE, <<8]
            alu[ring_setup, ring_setup, AND, _IN_BASE_ADDR_]
        #endif
        alu[ring_setup, ring_setup, or, _RING_SIZE_ENC_, <<29]
        alu[$xd[0], ring_setup, or, _RING_STATUS_ENC_, <<28]
        // all other fields in ring_base is set to 0 as specified in the DB
        immed[$xd[1], 0]
        immed[$xd[2], 0]

        // setup the ring CSR address
        move(ring_ctm_addr, 0x00080100) // local xpb, xpb dev id 8, start reg = ring_0 base
        alu[ring_ctm_addr, ring_ctm_addr, OR, _IN_RING_NUM_, <<4] // offset to selected ring

        // write the regs
        ct[xpb_write, $xd[0], ring_ctm_addr, 0, 3], ctx_swap[_s]

        ru_ctm_ring_setup_end#:
        // clean up namespace
        #undef  _RING_SIZE_ENC_
        #undef  _RING_STATUS_ENC_

    .end
#endm // ru_ctm_ring_setup


/** Wrapper for CLS/GS/CTM ring commands.
 *
 * This macro is used to hide some of the internal details such as encoding the ring address.
 *
 * @param __MEM_TYPE__   One of CLS,GS, or CTM. GS not supported for NFP6000. CTM only supported for NFP6000
 * @param _IN_CMD_       Ring command to perform (put/get for CLS,GS or ring_put/get for CTM)
 * @param in_xfer_reg    Xfer register name to use in command
 * @param _IN_RING_NUM_  Ring number where data is to be placed, must be between 0-15
 * @param _IN_REF_CNT_   Reference count, must be between 1-16
 * @param in_sig         Signal to use for ring operation
 *
 * @note No swapping on the signal is done, the calling code needs to do this.
 *
 */
#macro ru_ring_op(__MEM_TYPE__, _IN_CMD_, in_xfer_reg, _IN_RING_NUM_, _IN_REF_CNT_, in_sig)
    #if (( IS_NFPTYPE(__NFP6000) || IS_NFPTYPE(__NFP3800)) && streq('__MEM_TYPE__', 'GS'))
        #error "ring_op: Mem Type GS (Global Scratch) not supported on NFP6000"
    #endif

    #if ( !(IS_NFPTYPE(__NFP6000) || IS_NFPTYPE(__NFP3800)) && streq('__MEM_TYPE__', 'CTM'))
        #error "ru_ring_op: Mem Type CTM (Cluster Target Memory) is only supported on NFP6000"
    #endif

    #if is_ct_const(_IN_RING_NUM_)
        #if ((_IN_RING_NUM_ < 0) | (_IN_RING_NUM_ > 15))
            #error "ru_ring_op: Ring number must be between 0 and 15"
        #endif
    #endif

    .begin
    .reg    addr_reg
    alu[addr_reg, --, b, _IN_RING_NUM_, <<2]    // 2: ring number is bits [5:2]

    #if (streq('__MEM_TYPE__', 'CS') || streq('__MEM_TYPE__', 'CLS'))
        #if (_IN_REF_CNT_ > 8)
            .reg _tmpc

            // Set override length in PREV_ALU according assembler option -indirect_ref_format_v2/v1
            #if (defined(__NFP_INDIRECT_REF_FORMAT_V2))
                immed[_tmpc, ((_IN_REF_CNT_) - 1)]          // note: length override field zero indexed
                alu[--, 0x80, or, _tmpc, <<8]               // 0x80: override length field flag
            #else                                           // __NFP_INDIRECT_REF_FORMAT_V1
                alu[_tmpc, --, b, 0x02, <<28]               // 0x02: override length field flags
                alu[--, _tmpc, or, ((_IN_REF_CNT_) - 1)]    // note: length override field zero indexed
            #endif
            cls[_IN_CMD_, in_xfer_reg, 0, addr_reg, max_/**/_IN_REF_CNT_], indirect_ref, sig_done[in_sig]
        #else
            cls[_IN_CMD_, in_xfer_reg, 0, addr_reg, _IN_REF_CNT_], sig_done[in_sig]
        #endif

    #elif (streq('__MEM_TYPE__', 'GS'))

        scratch[_IN_CMD_, in_xfer_reg, 0, addr_reg, _IN_REF_CNT_], sig_done[in_sig]

    #elif (streq('__MEM_TYPE__', 'CTM'))
        #if (_IN_REF_CNT_ > 8)
            .reg _tmp

            // Set override length in PREV_ALU according assembler option -indirect_ref_format_v2/v1
            #if (defined(__NFP_INDIRECT_REF_FORMAT_V2))
                immed[_tmp, ((_IN_REF_CNT_) - 1)]       // note: length override field zero indexed
                alu[--, 0x80, or, _tmp, <<8]            // 0x80: override length field flag
            #else                                       // __NFP_INDIRECT_REF_FORMAT_V1
                alu[_tmp, --, b, 0x02, <<28]            // 0x02: override length field flags
                alu[--, _tmp, or, ((_IN_REF_CNT_) - 1)] // note: length override field zero indexed
            #endif

            ct[_IN_CMD_, in_xfer_reg, 0, addr_reg, max_/**/_IN_REF_CNT_], indirect_ref, sig_done[in_sig]
        #else
            ct[_IN_CMD_, in_xfer_reg, 0, addr_reg, _IN_REF_CNT_], sig_done[in_sig]
        #endif
    #else
        #error "Ring Memory type not valid."
    #endif
    .end
#endm


/// @cond INTERNAL_MACROS
/** Wrapper for QDR ring commands.
 *
 * @param _IN_CMD_                Ring command to perform (get / put / journal)
 * @param in_xfer_reg             Xfer register name to use in command
 * @param in_src_op1/in_src_op2   Restricted operands are added (src_op1 + src_op2) to define the following:
 *                                @arg [31:30]: SRAM channel.
 *                                @arg [29:8]: Ignored.
 *                                @arg [7:2]: Ring number.
 *                                @arg [1:0]: Ignored.
 * @param _IN_REF_CNT_            Reference count in increments of 4 byte words. Valid values are 1 to 8.
 * @param in_sig_name             Signal to use for ring operation
 * @param sig_action              SIG_NONE or SIG_WAIT
 *
 * @note @arg If there are no sufficient words in the ring for get and put commands, two signals will be pushed where
 *            sig_name[1] signals error.
 */
#macro _ru_sram_ring_op(_IN_CMD_, in_xfer_reg, in_src_op1, in_src_op2, _RING_CHAN_NUM_, _IN_REF_CNT_, in_sig_name, sig_action)
.begin

    #if (!is_ct_const(_IN_REF_CNT_))
        #error "_ru_sram_ring_op: reference count must be constant."
    #endif

    #if (_RING_CHAN_NUM_ > 3 || _RING_CHAN_NUM_ < 0)
        #error "Ring channel must be in the range 0-3"
    #endif

    .reg q_id tmp
    alu_shf[tmp, --, B, _RING_CHAN_NUM_, <<30]
    move(q_id, in_src_op1)
    alu_shf[q_id, tmp, OR, q_id, <<2]
    sram[_IN_CMD_, in_xfer_reg, q_id, in_src_op2, _IN_REF_CNT_], sig_done[in_sig_name]

    #if (streq('sig_action', 'SIG_WAIT'))
        ctx_arb[in_sig_name]
    #endif
.end
#endm
/// @endcond

/** Put @p _IN_REF_CNT_ words on sram ring
 * @param in_xfer_reg             Xfer register name to use in command
 * @param in_src_op1              Restricted operands are added (src_op1 + src_op2) to define the following:
 *                                @arg [31:30]: SRAM channel.
 *                                @arg [29:8]: Ignored.
 *                                @arg [7:2]: Ring number.
 *                                @arg [1:0]: Ignored.
 * @param in_src_op2              As per above
 * @param _RING_CHAN_NUM_         SRAM channel/bank to use
 * @param _IN_REF_CNT_            Reference count in increments of 4 byte words. Valid values are 1 to 8.
 * @param in_sig_name             Signal to use for ring operation
 * @param sig_action              SIG_NONE or SIG_WAIT
 *
 * @note @arg If there are no sufficient words in the ring for get and put commands, two signals will be pushed where
 *            sig_name[1] signals error.
 */
#macro ru_sram_ring_put(in_xfer_reg, in_src_op1, in_src_op2, _RING_CHAN_NUM_, _IN_REF_CNT_, in_sig_name, sig_action)
    _ru_sram_ring_op(put, in_xfer_reg, in_src_op1, in_src_op2, _RING_CHAN_NUM_, _IN_REF_CNT_, in_sig_name, sig_action)
#endm // ru_sram_ring_put

/** Refer to description for ru_sram_ring_put macro.
 */
#macro ru_sram_ring_get(out_xfer_reg, in_src_op1, in_src_op2, _RING_CHAN_NUM_, IN_REF_CNT_, in_sig_name)
    _ru_sram_ring_op(get, out_xfer_reg, in_src_op1, in_src_op2, _RING_CHAN_NUM_, _IN_REF_CNT_, in_sig_name, SIG_NONE)
#endm // ru_sram_ring_get

/** Refer to description for ru_sram_ring_put macro.
 */
#macro ru_sram_ring_get(out_xfer_reg, in_src_op1, in_src_op2, _RING_CHAN_NUM_,  _IN_REF_CNT_, in_sig_name, sig_action)
    _ru_sram_ring_op(get, out_xfer_reg, in_src_op1, in_src_op2, _RING_CHAN_NUM_, _IN_REF_CNT_, in_sig_name, sig_action)
#endm // ru_sram_ring_get

/** Refer to description for ru_sram_ring_put macro.
 */
#macro ru_sram_ring_journal(in_xfer_reg, in_src_op1, in_src_op2, _RING_CHAN_NUM_, _IN_REF_CNT_, in_sig_name, sig_action)
    _ru_sram_ring_op(journal, in_xfer_reg, in_src_op1, in_src_op2, _RING_CHAN_NUM_, _IN_REF_CNT_, in_sig_name, sig_action)
#endm // ru_sram_ring_journal

/// @cond INTERNAL_MACROS
/** Wrapper for DDR ring commands.
 *
 * @param _IN_CMD_                Ring command to perform (get / get_safe / get_eop / get_tag_safe /
 *                                pop / pop_safe / pop_eop / pop_tag_safe /
 *                                put / put_tag / journal / journal_tag)
 * @param in_xfer_reg             Xfer register name to use in command
 * @param in_src_op1/in_src_op2   Restricted operands are added (src_op1 + src_op2) to define the following:
 *                                @arg [23:16]: Memory tag number for XX_tag_XX commands.
 *                                @arg [9:0]: Queue array entry number.
 * @param _IN_REF_CNT_            Reference count in increments of 4 byte words. Valid values are 1 to 16
 *
 * @param in_sig_name             Signal to use for ring operation
 * @param sig_action              SIG_NONE or SIG_WAIT
 *
 * @note @arg If there are not sufficient words in the ring for get, pop, and put commands, two signals will be pushed where
 *            sig_name[1] signals error.
 *       @arg If the EOP bit is set for get_eop and pop_eop commands, two signals will be pushed, where sig_name[1] signals error.
 *       @arg If the tag is not matched for get_safe_tag, pop_tag_safe, and journal_tag, two signals will be pushed, where
 *            sig_name[1] signals error.
 *
 */
#macro _ru_dram_ring_op(_IN_CMD_, in_xfer_reg, in_src_op1, in_src_op2, _IN_REF_CNT_, in_sig_name, sig_action)
    #if (!is_ct_const(_IN_REF_CNT_))
        #error "_ru_dram_ring_op: _IN_REF_CNT must be constant."
    #endif

    #if (_IN_REF_CNT_ > 16)
        #error "_ru_dram_ring_op: _IN_REF_CNT cannot exceed 16."
    #endif

    #if ((IS_NFPTYPE(__NFP6000) || IS_NFPTYPE(__NFP3800)) && ( strstr(_IN_CMD_, 'tag') != 0 ))
        #error "ru_dram_ring_op: tag cmds not supported on NFP6000"
    #endif

    .begin
    #if (is_ct_const(in_src_op1))
        .reg q_id

        immed[q_id, in_src_op1]
        #if ((_IN_REF_CNT_) > 8)
            .reg cnt

            // Set override length in PREV_ALU according assembler option -indirect_ref_format_v2/v1
            #if (defined(__NFP_INDIRECT_REF_FORMAT_V2))
                immed[cnt, ((_IN_REF_CNT_) - 1)]        // note: length override field zero indexed
                alu[--, 0x80, or, cnt, <<8]             // 0x80: indicate override length field
            #else                                       // __NFP_INDIRECT_REF_FORMAT_V1
                alu[cnt, --, b, 0x02, <<28]             // 0x02: indicate override length field
                alu[--, cnt, or, ((_IN_REF_CNT_) - 1)]  // note: length override field zero indexed
            #endif
            mem[_IN_CMD_, in_xfer_reg, q_id, in_src_op2, max_/**/_IN_REF_CNT_], indirect_ref, sig_done[in_sig_name]
        #else
            mem[_IN_CMD_, in_xfer_reg, q_id, in_src_op2, _IN_REF_CNT_], sig_done[in_sig_name]
        #endif
    #else
        #if (_IN_REF_CNT_ > 8)
            .reg cnt

            // Set override length in PREV_ALU according assembler option -indirect_ref_format_v2/v1
            #if (defined(__NFP_INDIRECT_REF_FORMAT_V2))
                immed[cnt, ((_IN_REF_CNT_) - 1)]        // note: length override field zero indexed
                alu[--, 0x80, or, cnt, <<8]             // 0x80: indicate override length field
            #else                                       // __NFP_INDIRECT_REF_FORMAT_V1
                alu[cnt, --, b, 0x02, <<28]             // 0x02: indicate override length field
                alu[--, cnt, or, ((_IN_REF_CNT_) - 1)]  // note: length override field zero indexed
            #endif
            mem[_IN_CMD_, in_xfer_reg, in_src_op1, in_src_op2, max_/**/_IN_REF_CNT_], indirect_ref, sig_done[in_sig_name]
        #else
            mem[_IN_CMD_, in_xfer_reg, in_src_op1, in_src_op2, _IN_REF_CNT_], sig_done[in_sig_name]
        #endif
    #endif  // is_ct_const(in_src_op1)
    .end

    #if ((streq('_IN_CMD_','get')     + \
          streq('_IN_CMD_','get_eop') + \
          streq('_IN_CMD_','pop')     + \
          streq('_IN_CMD_','pop_eop')) == 1)

        #if (streq('sig_action', 'SIG_WAIT'))
            ctx_arb[in_sig_name[0]]
        #endif
    #else
        #if (streq('sig_action', 'SIG_WAIT'))
            ctx_arb[in_sig_name]
        #endif
    #endif
#endm
/// @endcond

/** Put @p _IN_REF_CNT_ words on dram ring
 * @param in_xfer_reg             Xfer register name to use in command
 * @param in_src_op1              Restricted operands are added (src_op1 + src_op2) to define the following:
 *                                @arg [23:16]: Memory tag number for XX_tag_XX commands.
 *                                @arg [9:0]: Queue array entry number.
 * @param in_src_op2              As per above
 * @param _IN_REF_CNT_            Reference count in increments of 4 byte words. Valid values are 1 to 16 for NFP6000
 *                                or 1 to 8 otherwise. Specified as actual count - 1.
 * @param in_sig_name             Signal to use for ring operation
 * @param sig_action              SIG_NONE or SIG_WAIT
 *
 * @note @arg If there are not sufficient words in the ring for get, pop, and put commands, two signals will be pushed where
 *            sig_name[1] signals error.
 *       @arg If the EOP bit is set for get_eop and pop_eop commands, two signals will be pushed, where sig_name[1] signals error.
 *       @arg If the tag is not matched for get_safe_tag, pop_tag_safe, and journal_tag, two signals will be pushed, where
 *            sig_name[1] signals error.
 *
 */
#macro ru_dram_ring_put(in_xfer_reg, in_src_op1, in_src_op2, _IN_REF_CNT_, in_sig_name, sig_action)
    _ru_dram_ring_op(put, in_xfer_reg, in_src_op1, in_src_op2, _IN_REF_CNT_, in_sig_name, sig_action)
#endm // ru_dram_ring_put


/** Refer to description for ru_dram_ring_put macro.
 */
#macro ru_dram_ring_put_tag(in_xfer_reg, in_src_op1, in_src_op2, _IN_REF_CNT_, in_sig_name, sig_action)
    _ru_dram_ring_op(put_tag, in_xfer_reg, in_src_op1, in_src_op2, _IN_REF_CNT_, in_sig_name, sig_action)
#endm // ru_dram_ring_put_tag


/** Refer to description for ru_dram_ring_put macro.
 */
#macro ru_dram_ring_qadd_work(in_xfer_reg, in_src_op1, in_src_op2, _IN_REF_CNT_, in_sig_name, sig_action)
    _ru_dram_ring_op(qadd_work, in_xfer_reg, in_src_op1, in_src_op2, _IN_REF_CNT_, in_sig_name, sig_action)
#endm // ru_dram_ring_put


/** Refer to description for ru_dram_ring_put macro.
 */
#macro ru_dram_ring_qadd_thread(in_xfer_reg, in_src_op1, in_src_op2, _IN_REF_CNT_, in_sig_name, sig_action)
    _ru_dram_ring_op(qadd_thread, in_xfer_reg, in_src_op1, in_src_op2, _IN_REF_CNT_, in_sig_name, sig_action)
#endm // ru_dram_ring_put


/** Refer to description for ru_dram_ring_put macro.
 */
#macro ru_dram_ring_get(out_xfer_reg, in_src_op1, in_src_op2, _IN_REF_CNT_, in_sig_name)
    _ru_dram_ring_op(get, out_xfer_reg, in_src_op1, in_src_op2, _IN_REF_CNT_, in_sig_name, SIG_NONE)
#endm // ru_dram_ring_get

/** Refer to description for ru_dram_ring_put macro.
 */
#macro ru_dram_ring_get(out_xfer_reg, in_src_op1, in_src_op2, _IN_REF_CNT_, in_sig_name, sig_action)
    _ru_dram_ring_op(get, out_xfer_reg, in_src_op1, in_src_op2, _IN_REF_CNT_, in_sig_name, sig_action)
#endm // ru_dram_ring_get


/** Refer to description for ru_dram_ring_put macro.
 */
#macro ru_dram_ring_get_eop(out_xfer_reg, in_src_op1, in_src_op2, _IN_REF_CNT_, in_sig_name, sig_action)
    _ru_dram_ring_op(get_eop, out_xfer_reg, in_src_op1, in_src_op2, _IN_REF_CNT_, in_sig_name, sig_action)
#endm // ru_dram_ring_get_eop


/** Refer to description for ru_dram_ring_put macro.
 */
#macro ru_dram_ring_get_safe(out_xfer_reg, in_src_op1, in_src_op2, _IN_REF_CNT_, in_sig_name, sig_action)
    _ru_dram_ring_op(get_safe, out_xfer_reg, in_src_op1, in_src_op2, _IN_REF_CNT_, in_sig_name, sig_action)
#endm // ru_dram_ring_get_safe


/** Refer to description for ru_dram_ring_put macro.
 */
#macro ru_dram_ring_get_tag_safe(out_xfer_reg, in_src_op1, in_src_op2, _IN_REF_CNT_, in_sig_name, sig_action)
    _ru_dram_ring_op(get_tag_safe, out_xfer_reg, in_src_op1, in_src_op2, _IN_REF_CNT_, in_sig_name, sig_action)
#endm // ru_dram_ring_get_tag_safe


/** Refer to description for ru_dram_ring_put macro.
 */
#macro ru_dram_ring_pop(in_xfer_reg, in_src_op1, in_src_op2, _IN_REF_CNT_, in_sig_name)
    _ru_dram_ring_op(pop, in_xfer_reg, in_src_op1, in_src_op2, _IN_REF_CNT_, in_sig_name, SIG_NONE)
#endm // ru_dram_ring_pop

/** Refer to description for ru_dram_ring_put macro.
 */
#macro ru_dram_ring_pop(out_xfer_reg, in_src_op1, in_src_op2, _IN_REF_CNT_, in_sig_name, sig_action)
    #if (streq('sig_action', 'SIG_WAIT'))
        #error "SIG_WAIT may cause indefinite wait in ctx_arb[] due to double signal. Use SIG_NONE instead and handle signal external to macro."
    #endif
    #if (streq('sig_action', 'SIG_NONE'))
        #warning "This macro is deprecated. Please use the macro without sig_action parameter."
    #endif
    _ru_dram_ring_op(pop, out_xfer_reg, in_src_op1, in_src_op2, _IN_REF_CNT_, in_sig_name, sig_action)
#endm // ru_dram_ring_pop


/** Refer to description for ru_dram_ring_put macro.
 */
#macro ru_dram_ring_pop_eop(out_xfer_reg, in_src_op1, in_src_op2, _IN_REF_CNT_, in_sig_name, sig_action)
    _ru_dram_ring_op(pop_eop, out_xfer_reg, in_src_op1, in_src_op2, _IN_REF_CNT_, in_sig_name, sig_action)
#endm // ru_dram_ring_pop_eop


/** Refer to description for ru_dram_ring_put macro.
 */
#macro ru_dram_ring_pop_safe(out_xfer_reg, in_src_op1, in_src_op2, _IN_REF_CNT_, in_sig_name, sig_action)
    _ru_dram_ring_op(pop_safe, out_xfer_reg, in_src_op1, in_src_op2, _IN_REF_CNT_, in_sig_name, sig_action)
#endm // ru_dram_ring_pop_safe


/** Refer to description for ru_dram_ring_put macro.
 */
#macro ru_dram_ring_pop_tag_safe(out_xfer_reg, in_src_op1, in_src_op2, _IN_REF_CNT_, in_sig_name, sig_action)
    _ru_dram_ring_op(pop_tag_safe, out_xfer_reg, in_src_op1, in_src_op2, _IN_REF_CNT_, in_sig_name, sig_action)
#endm // ru_dram_ring_pop_tag_safe


/** Refer to description for ru_dram_ring_put macro.
 */
#macro ru_dram_ring_journal(in_xfer_reg, in_src_op1, in_src_op2, _IN_REF_CNT_, in_sig_name, sig_action)
    _ru_dram_ring_op(journal, in_xfer_reg, in_src_op1, in_src_op2, _IN_REF_CNT_, in_sig_name, sig_action)
#endm // ru_dram_ring_journal


/** Refer to description for ru_dram_ring_put macro.
 */
#macro ru_dram_ring_journal_tag(in_xfer_reg, in_src_op1, in_src_op2, _IN_REF_CNT_, in_sig_name, sig_action)
    _ru_dram_ring_op(journal_tag, in_xfer_reg, in_src_op1, in_src_op2, _IN_REF_CNT_, in_sig_name, sig_action)
#endm // ru_dram_ring_journal_tag

/**
 * Generic ring dequeue operation
 *
 * @param out_req  Register aggregate
 * @param RING_TYPE Ring type, One of NN_RING,CLS_RING,GS_RING,DDR_RING,QDR_RING
 * @param ref_cnt Number of operations to perform
 * @param ring_num Ring number
 * @param sig Signal to generate
 * @param sig_action If SIG_WAIT, waits for operation to complete
 * @param __NULL_LABEL__ label to branch to on NN_RING empty, or -- to wait for not empty
 *
 */
#macro ru_deq_from_ring(out_req, RING_TYPE, ref_cnt, ring_num, sig, sig_action, __NULL_LABEL__)
    ru_deq_from_ring(out_req, RING_TYPE, ref_cnt, ring_num, --, sig, sig_action, __NULL_LABEL__)
#endm

/**
 * Generic ring dequeue operation
 *
 * @param out_req  Register aggregate
 * @param RING_TYPE Ring type, One of NN_RING,CLS_RING,GS_RING,DDR_RING,QDR_RING
 * @param ref_cnt Number of operations to perform
 * @param ring_num Ring number
 * @param ring_chan Ring channel, only applicable to the QDR_RING
 * @param sig Signal to generate
 * @param sig_action If SIG_WAIT, waits for operation to complete
 * @param __NULL_LABEL__ label to branch to on NN_RING empty, or -- to wait for not empty
 *
 */
#macro ru_deq_from_ring(out_req, RING_TYPE, ref_cnt, ring_num, ring_chan, sig, sig_action, __NULL_LABEL__)
.begin

    #if ( ( streq('RING_TYPE', 'NN_RING')   + \
            streq('RING_TYPE', 'CLS_RING')  + \
            streq('RING_TYPE', 'GS_RING')   + \
            streq('RING_TYPE', 'DDR_RING')  + \
            streq('RING_TYPE', 'DDR_WQ_RING')  + \
            streq('RING_TYPE', 'QDR_RING')    \
        ) != 1 )
        #error "ru_deq_from_ring: Either Input interface not defined or multiple interface types defined."
    #endif

    #if ((IS_NFPTYPE(__NFP6000) || IS_NFPTYPE(__NFP3800)))
        #if ( streq('RING_TYPE', 'GS_RING'))
            #error "ru_deq_from_ring: GS_RING (Global Scratch Ring) not supported on NFP6000."
        #endif
    #endif

    #if (streq('__NULL_LABEL__', '--'))
        #define_eval __LOOP_LABEL__ ("dq_ring_empty#")
    #else
        #define_eval __LOOP_LABEL__ ('__NULL_LABEL__')
    #endif

    #if ( streq('RING_TYPE', 'NN_RING') )

        dq_ring_empty#:
        br_inp_state[nn_empty, __LOOP_LABEL__]

        #define_eval __LOOP__ 0

        #while(__LOOP__ < ref_cnt)

            alu[out_req[__LOOP__], --, b, *n$index++]

            #define_eval __LOOP__ (__LOOP__ + 1)

        #endloop

        #undef __LOOP__

        #if( streq('sig_action', 'SIG_WAIT') )
            ctx_arb[sig]
        #endif

    #elif ( streq('RING_TYPE', 'CLS_RING') )

        ru_ring_op(CLS, get, out_req[0], ring_num, ref_cnt, sig)

        #if( streq('sig_action', 'SIG_WAIT') )
            ctx_arb[sig]
        #endif

    #elif ( streq('RING_TYPE', 'GS_RING') )

        ru_ring_op(GS, get, out_req[0], ring_num, ref_cnt, sig)

        #if( streq('sig_action', 'SIG_WAIT') )
            ctx_arb[sig]
        #endif

    #elif ( streq('RING_TYPE', 'DDR_RING') )

        ru_dram_ring_get(out_req[0], ring_num, 0, ref_cnt, sig, sig_action)

    #elif( streq('RING_TYPE', 'DDR_WQ_RING') )

        ru_dram_ring_qadd_thread(out_req[0], ring_num, 0, ref_cnt, sig, sig_action)

    #elif ( streq('RING_TYPE', 'QDR_RING') )

        #if (streq('ring_chan', '--'))
            #define_eval RING_CHAN 0
            #warning "Ring channel was not specified for a QDR ring, assuming channel zero! Use ru_enq_to_ring with 8 parameters to explicitly set the channel"
        #else
            #define_eval RING_CHAN ring_chan
        #endif
        ru_sram_ring_get(out_req[0], ring_num, 0, RING_CHAN, ref_cnt, sig, sig_action)

    #else

        #error "Invalid input interface type defined:" RING_TYPE

    #endif //QDR_RING

    #undef __LOOP_LABEL__

.end
#endm // ru_deq_from_ring


/** Add n 32-bit words to the tail of the ring.
 *
 * @param in_xfer_reg   xfer registers
 * @param in_src_op1    Ring number (contant/GPR)
 * @param in_src_op2    Not used and ignored. Can be "--".
 * @param _IN_REF_CNT_  Number of 32-bit words to put on to ring
 * @param in_sig_name   Signal to wait on
 * @param sig_action    SIG_WAIT or SIG_NONE
 */
#macro ru_cls_ring_put(in_xfer_reg, in_src_op1, in_src_op2, _IN_REF_CNT_, in_sig_name, sig_action)
.begin

    ru_ring_op(CLS, put, in_xfer_reg, in_src_op1, _IN_REF_CNT_, in_sig_name)
    #if( streq('sig_action', 'SIG_WAIT') )
        ctx_arb[in_sig_name]
    #endif

.end
#endm // ru_cls_ring_put


/** Pop n 32-bit words from tail of the ring (LIFO).
 */
#macro ru_cls_ring_pop(in_xfer_reg, in_src_op1, in_src_op2, _IN_REF_CNT_, in_sig_name, sig_action)
.begin

    ru_ring_op(CLS, pop, in_xfer_reg, in_src_op1, _IN_REF_CNT_, in_sig_name)
    #if( streq('sig_action', 'SIG_WAIT') )
        ctx_arb[in_sig_name]
    #endif

.end
#endm // ru_cls_ring_pop


/** Pop n 32-bit words from tail of the ring (LIFO).
 *
 * If less than n in the ring, return zero for extra words.
 */
#macro ru_cls_ring_pop_safe(in_xfer_reg, in_src_op1, in_src_op2, _IN_REF_CNT_, in_sig_name, sig_action)
.begin

    ru_ring_op(CLS, pop_safe, in_xfer_reg, in_src_op1, _IN_REF_CNT_, in_sig_name)
    #if( streq('sig_action', 'SIG_WAIT') )
        ctx_arb[in_sig_name]
    #endif

.end
#endm // ru_cls_ring_pop_safe


/** Get n 32-bit words from head of the ring (FIFO).
 *
 * If less than n in the ring, return zero for extra words.
 */
#macro ru_cls_ring_get_safe(in_xfer_reg, in_src_op1, in_src_op2, _IN_REF_CNT_, in_sig_name, sig_action)
.begin

    ru_ring_op(CLS, get_safe, in_xfer_reg, in_src_op1, _IN_REF_CNT_, in_sig_name)
    #if( streq('sig_action', 'SIG_WAIT') )
        ctx_arb[in_sig_name]
    #endif

.end
#endm // ru_cls_ring_get_safe

/** Get n 32-bit words from head of the ring (FIFO).
 *
 */
#macro ru_cls_ring_get(in_xfer_reg, in_src_op1, in_src_op2, _IN_REF_CNT_, in_sig_name, sig_action)
.begin

    ru_ring_op(CLS, get, in_xfer_reg, in_src_op1, _IN_REF_CNT_, in_sig_name)
    #if( streq('sig_action', 'SIG_WAIT') )
        ctx_arb[in_sig_name]
    #endif

.end
#endm // ru_cls_ring_get


/** Add n 32-bit words to the tail of the ring.
 *
 * @param in_xfer_reg   xfer registers
 * @param in_src_op1    Ring number (contant/GPR)
 * @param in_src_op2    Not used and ignored. Can be "--".
 * @param _IN_REF_CNT_  Number of 32-bit words to put on to ring
 * @param in_sig_name   Signal to wait on
 * @param sig_action    SIG_WAIT or SIG_NONE
 */
#macro ru_ctm_ring_put(in_xfer_reg, in_src_op1, in_src_op2, _IN_REF_CNT_, in_sig_name, sig_action)
.begin

    ru_ring_op(CTM, ring_put, in_xfer_reg, in_src_op1, _IN_REF_CNT_, in_sig_name)
    #if( streq('sig_action', 'SIG_WAIT') )
        ctx_arb[in_sig_name]
    #endif

.end
#endm // ru_ctm_ring_put


/** Get n 32-bit words from head of the ring.
 *
 * @param in_xfer_reg   xfer registers
 * @param in_src_op1    Ring number (contant/GPR)
 * @param in_src_op2    Not used and ignored. Can be "--".
 * @param _IN_REF_CNT_  Number of 32-bit words to get from ring
 * @param in_sig_name   Signal to wait on
 * @param sig_action    SIG_WAIT or SIG_NONE
 */
#macro ru_ctm_ring_get(in_xfer_reg, in_src_op1, in_src_op2, _IN_REF_CNT_, in_sig_name, sig_action)
.begin

    ru_ring_op(CTM, ring_get, in_xfer_reg, in_src_op1, _IN_REF_CNT_, in_sig_name)
    #if( streq('sig_action', 'SIG_WAIT') )
        ctx_arb[in_sig_name]
    #endif

.end
#endm // ru_ctm_ring_get


/**
 * Generic ring enqueue operation, with optional ring channel
 *
 * @param in_req  Register aggregate
 * @param RING_TYPE Ring type, One of NN_RING,CLS_RING,GS_RING(not supported for NFP6000),DDR_RING,QDR_RING
 * @param ref_cnt Number of operations to perform
 * @param ring_num Ring number
 * @param sig Signal to generate
 * @param sig_action If SIG_WAIT, waits for operation to complete
 * @param __FULL_LABEL__ label to branch to on NN_RING full, or -- to wait for not full
 */
#macro ru_enq_to_ring(in_req, RING_TYPE, ref_cnt, ring_num, sig, sig_action, __FULL_LABEL__)
    ru_enq_to_ring(in_req, RING_TYPE, ref_cnt, ring_num, --, sig, sig_action, __FULL_LABEL__)
#endm

/**
 * Generic ring enqueue operation, with optional ring channel
 *
 * @param in_req  Register aggregate
 * @param RING_TYPE Ring type, One of NN_RING,CLS_RING,GS_RING,DDR_RING,QDR_RING
 * @param ref_cnt Number of operations to perform
 * @param ring_num Ring number
 * @param ring_chan Ring channel, only applicable to the QDR_RING
 * @param sig Signal to generate
 * @param sig_action If SIG_WAIT, waits for operation to complete
 * @param __FULL_LABEL__ label to branch to on NN_RING full, or -- to wait for not full
 */
#macro ru_enq_to_ring(in_req, RING_TYPE, ref_cnt, ring_num, ring_chan, sig, sig_action, __FULL_LABEL__)
.begin

    #if ( ( streq('RING_TYPE', 'NN_RING')   + \
            streq('RING_TYPE', 'CLS_RING')  + \
            streq('RING_TYPE', 'GS_RING')   + \
            streq('RING_TYPE', 'DDR_RING')  + \
            streq('RING_TYPE', 'DDR_WQ_RING')  + \
            streq('RING_TYPE', 'QDR_RING')    \
        ) != 1 )
        #error "ru_enq_to_ring: Either Output interface not defined or multiple interface types defined."
    #endif

    #if (IS_NFPTYPE(__NFP6000) || IS_NFPTYPE(__NFP3800))
        #if ( streq('RING_TYPE', 'GS_RING'))
            #error "ru_enq_to_ring: GS_RING (Global Scratch Ring) not supported on NFP6000."
        #endif
    #endif


    #if (streq('__FULL_LABEL__', '--'))
        #define_eval __LOOP_LABEL__ ("enq_ring_full#")
    #else
        #define_eval __LOOP_LABEL__ ('__FULL_LABEL__')
    #endif

    #if( streq('RING_TYPE', 'NN_RING') )

    enq_ring_full#:
        br_inp_state[nn_full, __LOOP_LABEL__]

        #define_eval __LOOP__ 0

        #while(__LOOP__ < ref_cnt)

            alu[*n$index++, --, b, in_req[__LOOP__]]

            #define_eval __LOOP__ (__LOOP__ + 1)

        #endloop

        #undef __LOOP__

        #if( streq('sig_action', 'SIG_WAIT') )
            ctx_arb[sig]
        #endif

    #elif( streq('RING_TYPE', 'CLS_RING') )

        #if (!streq('__FULL_LABEL__', 'PUT_BLIND'))
        #if (!is_ct_const(ring_num))
            #error "Ring number must be constant."
        #endif

    enq_ring_full#:
        br_!inp_lstate[cls_ring/**/ring_num/**/_status, put_req#]

        br[__LOOP_LABEL__]
        #endif

    put_req#:
        ru_ring_op(CLS, put, in_req[0], ring_num, ref_cnt, sig)

        #if( streq('sig_action', 'SIG_WAIT') )
            ctx_arb[sig]
        #endif

    #elif( streq('RING_TYPE', 'GS_RING') )

        #if (!streq('__FULL_LABEL__', 'PUT_BLIND'))
        #if (!is_ct_const(ring_num))
            #error "Ring number must be constant."
        #endif

    enq_ring_full#:
        br_!inp_state[scr_ring/**/ring_num/**/_status, put_req#]

        br[__LOOP_LABEL__]
        #endif

    put_req#:
        ru_ring_op(GS, put, in_req[0], ring_num, ref_cnt, sig)

        #if( streq('sig_action', 'SIG_WAIT') )
            ctx_arb[sig]
        #endif

    #elif( streq('RING_TYPE', 'DDR_RING') )

        #if (!is_ct_const(ring_num))
            #error "Ring number must be constant."
        #endif

        ru_dram_ring_put(in_req[0], ring_num, 0, ref_cnt, sig, sig_action)

    #elif( streq('RING_TYPE', 'DDR_WQ_RING') )

        #if (!is_ct_const(ring_num))
            #error "Ring number must be constant."
        #endif

        ru_dram_ring_qadd_work(in_req[0], ring_num, 0, ref_cnt, sig, sig_action)

    #elif( streq('RING_TYPE', 'QDR_RING') )

        #if (streq('ring_chan', '--'))
            #define_eval RING_CHAN 0
            #warning "Ring channel was not specified for a QDR ring, assuming channel zero! Use ru_enq_to_ring with 8 parameters to explicitly set the channel"
        #else
            #define_eval RING_CHAN ring_chan
        #endif
        ru_sram_ring_put(in_req[0], ring_num, 0, RING_CHAN, ref_cnt, sig, sig_action)
    #else

        #error "Invalid output interface type defined:" RING_TYPE

    #endif

    #undef __LOOP_LABEL__

.end
#endm // ru_enq_to_ring

/*The following are deprecated, but we keep the old names for backward compatability*/
#define ru_cs_ring_setup ru_cls_ring_setup /**< Deprecated alias @deprecated This alias should not be used in new code. @see ru_cls_ring_setup */
#define ru_cs_ring_put ru_cls_ring_put /**< Deprecated alias @deprecated This alias should not be used in new code. @see ru_cls_ring_put */
#define ru_cs_ring_pop ru_cls_ring_pop /**< Deprecated alias @deprecated This alias should not be used in new code. @see ru_cls_ring_pop */
#define ru_cs_ring_pop_safe ru_cls_ring_pop_safe /**< Deprecated alias @deprecated This alias should not be used in new code. @see ru_cls_ring_pop_safe*/
#define ru_cs_ring_get_safe ru_cls_ring_get_safe /**< Deprecated alias @deprecated This alias should not be used in new code. @see ru_cls_ring_get_safe */


/** @}
 * @}
 */

#endif /* __RING_UTILS_UC__ */



