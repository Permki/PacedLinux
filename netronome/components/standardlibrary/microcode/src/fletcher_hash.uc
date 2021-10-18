/*
 * Copyright (C) 2007-2013 Netronome Systems, Inc.  All rights reserved.
 *
 */

#ifndef __FLETCHER_HASH_UC__
#define __FLETCHER_HASH_UC__

#ifndef NFP_LIB_ANY_NFAS_VERSION
    #if (!defined(__NFP_TOOL_NFAS) ||                       \
        (__NFP_TOOL_NFAS < NFP_SW_VERSION(5, 0, 0, 0)) ||   \
        (__NFP_TOOL_NFAS > NFP_SW_VERSION(6, 1, 0, 255)))
        #error "This standard library is not supported for the version of the SDK currently in use."
    #endif
#endif

/** @file fletcher_hash.uc Fletcher Hash Macros
 * @addtogroup fletcher_hash Fletcher Hash Operations
 * @{
 *
 * @name Fletcher Hash Operation Macros
 * @{
 *
 * These macros calculate Fletcher, Jenkins, and Hardware based Hash of data in local memory
 */

// API Quick Reference:
//
// fletcher_hash(out_f_hash, in_lm_base_addr, in_data_size_lw, in_lm_handle, in_calc_upper)
// jenkins_hash(out_j_hash, in_lm_base_addr, in_data_size_lw, in_lm_handle)
// jenkins_byte_hash(out_j_hash, in_lm_base_addr, in_data_size_lw, in_lm_handle)
// hardware_hash(out_hw_hash, in_lm_base_addr, in_data_size_lw, in_lm_handle)


/** Calculate 32 bit Fletcher Hash of data in local memory
 *
 * @b Example:
 * @code
 * localmem_set_address(0, 0, LM_HANDLE_0)
 * localmem_write8( 0x12345678, \
 *  	            0x11111111, \
 *	            0x22222222, \
 *	            0x33333333, \
 *	            0x44444444, \
 *	            0x55555555, \
 *	            0x66666666, \
 *	            0x77777777, \
 *	            0,0)
 * #define_eval LM_BASE_ADDR 0
 * #define_eval LM_SIZE_LWORDS 8
 * .reg r_hash_val
 * fletcher_hash(r_hash_val, LM_BASE_ADDR, LM_SIZE_LWORDS, 0, 1)
 * @endcode
 *
 * @param out_f_hash      Register receives hash calculation result
 * @param in_lm_base_addr Register or Constant, start address of data in local memory
 * @param in_data_size_lw Register or Constant, size of data in lwords
 * @param in_lm_handle    Constant, lm handle, either 0 or 1
 * @param in_calc_upper   Constant, enable calculation of upper 16 bits if 1
 *                        can be set to 0 to save instructions if upper bits not needed
 *
 * refer http://en.wikipedia.org/wiki/Fletcher's_checksum for more information
 */
#macro fletcher_hash(out_f_hash, in_lm_base_addr, in_data_size_lw, in_lm_handle, in_calc_upper)
.begin
    .reg    fh_len
    .reg    fh_temp
    .reg    fh_a
    #define_eval FH_CALC_UPPER in_calc_upper
    #if (FH_CALC_UPPER == 1)
    .reg    fh_b
    #endif
    .reg    fh_mask

    #define_eval FH_LM_HANDLE in_lm_handle
    #if (FH_LM_HANDLE == 0)
        #define FH_LM_ADDR      active_lm_addr_0
        #define FH_LM_INDEX     *l$index0
    #else
        #define FH_LM_ADDR      active_lm_addr_1
        #define FH_LM_INDEX     *l$index1
    #endif

    local_csr_wr[FH_LM_ADDR, in_lm_base_addr]
    alu[fh_len, --, b, in_data_size_lw]

    ; initialize all the fletcher registers
    immed[fh_a, 0xffff]
    #if (FH_CALC_UPPER == 1)
    immed[fh_b, 0xffff]
    #endif
    immed[fh_mask, 0xffff]
    immed[out_f_hash, 0]    ; fletcher output can not be zero
    alu[--, --, b, fh_len]
    beq[fletcher_hash_end#]


fletcher_hash_main_loop_start#:
    alu[fh_temp, fh_mask, and, FH_LM_INDEX]
    alu[fh_a, fh_a, +, fh_temp]
    #if (FH_CALC_UPPER == 1)
    alu[fh_b, fh_b, +, fh_a]
    #endif

    alu[fh_temp, fh_mask, and, FH_LM_INDEX, >>16]
    alu[fh_len, fh_len, -, 1]
    #if (FH_CALC_UPPER == 1)
    bne[fletcher_hash_main_loop_start#], defer[3]   ; not done yet
    #else
    bne[fletcher_hash_main_loop_start#], defer[2]   ; not done yet
    #endif
    alu[fh_a, fh_a, +, fh_temp]
    #if (FH_CALC_UPPER == 1)
    alu[fh_b, fh_b, +, fh_a]
    #endif
    alu[--, --, b, FH_LM_INDEX++]   ; advance LM pointer

    ; first reduction
    alu[fh_temp, fh_mask, and, fh_a, >>16]
    alu[fh_a, fh_mask, and, fh_a]
    alu[fh_a, fh_a, +, fh_temp]

    #if (FH_CALC_UPPER == 1)
    alu[fh_temp, fh_mask, and, fh_b, >>16]
    alu[fh_b, fh_mask, and, fh_b]
    alu[fh_b, fh_b, +, fh_temp]
    #endif

    ; second reduction
    alu[fh_temp, fh_mask, and, fh_a, >>16]
    alu[fh_a, fh_mask, and, fh_a]
    alu[fh_a, fh_a, +, fh_temp]

    #if (FH_CALC_UPPER == 1)
    alu[fh_temp, fh_mask, and, fh_b, >>16]
    alu[fh_b, fh_mask, and, fh_b]
    alu[fh_b, fh_b, +, fh_temp]
    #endif

fletcher_hash_end#:
    ; store the result
    #if (FH_CALC_UPPER == 1)
    alu[fh_temp, --, b, fh_b, <<16]
    alu[out_f_hash, fh_a, or, fh_temp]
    #else
    alu[out_f_hash, --, b, fh_a]
    #endif

    // clean up namespace
    #undef  FH_LM_HANDLE
    #undef  FH_LM_ADDR
    #undef  FH_LM_INDEX
.end
#endm   /* fletcher_hash */


/** Calculate Jenkins Hash of data in local memory
 *
 * @b Example:
 * @code
 * localmem_set_address(0, 0, LM_HANDLE_0)
 * localmem_write8( 0x12345678, \
 *  	            0x11111111, \
 *	            0x22222222, \
 *	            0x33333333, \
 *	            0x44444444, \
 *	            0x55555555, \
 *	            0x66666666, \
 *	            0x77777777, \
 *	            0,0)
 * #define_eval LM_BASE_ADDR 0
 * #define_eval LM_SIZE_LWORDS 8
 * .reg r_hash_val
 * jenkins_hash(r_hash_val, LM_BASE_ADDR, LM_SIZE_LWORDS, 0)
 * @endcode
 *
 * @param out_j_hash      Register receives hash calculation result
 * @param in_lm_base_addr Register or Constant, start address of data in local memory
 * @param in_data_size_lw Register or Constant, size of data in lwords
 * @param in_lm_handle    Constant, lm handle, either 0 or 1
 *
 * refer to http://en.wikipedia.org/wiki/Jenkins_hash_function for more information
 */
#macro jenkins_hash(out_j_hash, in_lm_base_addr, in_data_size_lw, in_lm_handle)
.begin
    .reg    jh_len
    .reg    jh_temp

    #define_eval JH_LM_HANDLE in_lm_handle
    #if (JH_LM_HANDLE == 0)
        #define JH_LM_ADDR      active_lm_addr_0
        #define JH_LM_INDEX     *l$index0
    #else
        #define JH_LM_ADDR      active_lm_addr_1
        #define JH_LM_INDEX     *l$index1
    #endif

    local_csr_wr[JH_LM_ADDR, in_lm_base_addr]
    alu[jh_len, --, b, in_data_size_lw]

    immed[out_j_hash, 0]    ; jenkins output can not be zero

    alu[--, --, b, jh_len]
    beq[jenkins_hash_end#]


jenkins_hash_main_loop_start#:
    alu[out_j_hash, out_j_hash, +, JH_LM_INDEX++]
    alu[jh_temp, --, b, out_j_hash, <<10]

    alu[jh_len, jh_len, -, 1]
    bne[jenkins_hash_main_loop_start#], defer[3]   ; not done yet
        alu[out_j_hash, out_j_hash, +, jh_temp]
        alu[jh_temp, --, b, out_j_hash, >>6]
        alu[out_j_hash, out_j_hash, xor, jh_temp]

    alu[jh_temp, --, b, out_j_hash, <<3]
    alu[out_j_hash, out_j_hash, +, jh_temp]
    alu[jh_temp, --, b, out_j_hash, >>11]
    alu[out_j_hash, out_j_hash, xor, jh_temp]
    alu[jh_temp, --, b, out_j_hash, <<15]
    alu[out_j_hash, out_j_hash, +, jh_temp]


jenkins_hash_end#:

    // clean up namespace
    #undef  JH_LM_HANDLE
    #undef  JH_LM_ADDR
    #undef  JH_LM_INDEX
.end
#endm   /* jenkins_hash */


/** Calculate Jenkins Hash of data in local memory, byte wise
 *
 * @b Example:
 * @code
 * localmem_set_address(0, 0, LM_HANDLE_0)
 * localmem_write8( 0x12345678, \
 *  	            0x11111111, \
 *	            0x22222222, \
 *	            0x33333333, \
 *	            0x44444444, \
 *	            0x55555555, \
 *	            0x66666666, \
 *	            0x77777777, \
 *	            0,0)
 * #define_eval LM_BASE_ADDR 0
 * #define_eval LM_SIZE_LWORDS 8
 * .reg r_hash_val
 * jenkins_byte_hash(r_hash_val, LM_BASE_ADDR, LM_SIZE_LWORDS, 0)
 * @endcode
 *
 * @param out_j_hash      Register receives hash calculation result
 * @param in_lm_base_addr Register or Constant, start address of data in local memory
 * @param in_data_size_lw Register or Constant, size of data in lwords
 * @param in_lm_handle    Constant, lm handle, either 0 or 1
 *
 * refer to http://en.wikipedia.org/wiki/Jenkins_hash_function for more information
 */
#macro jenkins_byte_hash(out_j_hash, in_lm_base_addr, in_data_size_lw, in_lm_handle)
.begin
    .reg    jh_len
    .reg    jh_blen
    .reg    jh_key_word
    .reg    jh_temp

    #define_eval JH_LM_HANDLE in_lm_handle
    #if (JH_LM_HANDLE == 0)
        #define JH_LM_ADDR      active_lm_addr_0
        #define JH_LM_INDEX     *l$index0
    #else
        #define JH_LM_ADDR      active_lm_addr_1
        #define JH_LM_INDEX     *l$index1
    #endif  /* JH_LM_HANDLE */

    local_csr_wr[JH_LM_ADDR, in_lm_base_addr]
    alu[jh_len, --, b, in_data_size_lw]

    immed[out_j_hash, 0]    ; jenkins output can not be zero

    ; assume the length is in LWs
    alu[--, --, b, jh_len]
    beq[jenkins_hash_end#]

jenkins_hash_main_loop_start#:
    alu[jh_blen, --, b, 4]  ; 4 bytes in an LW
    alu[jh_key_word, --, b, JH_LM_INDEX++]

jenkins_hash_byte_loop_start#:
    alu[jh_temp, 4, -, jh_blen]
    alu[jh_temp, --, b, jh_temp, <<3]  ; get multiple of 8 for shift
    alu[--, jh_temp, or, 0]         ; for indirect shift
    alu[jh_temp, 0xff, and, jh_key_word, >>indirect]

    alu[out_j_hash, out_j_hash, +, jh_temp]
    alu[jh_temp, --, b, out_j_hash, <<10]

    alu[jh_blen, jh_blen, -, 1]
    bne[jenkins_hash_byte_loop_start#], defer[3]   ; not done yet
        alu[out_j_hash, out_j_hash, +, jh_temp]
        alu[jh_temp, --, b, out_j_hash, >>6]
        alu[out_j_hash, out_j_hash, xor, jh_temp]

    alu[jh_len, jh_len, -, 1]
    bne[jenkins_hash_main_loop_start#]//, defer[3]   ; not done yet

    alu[jh_temp, --, b, out_j_hash, <<3]
    alu[out_j_hash, out_j_hash, +, jh_temp]
    alu[jh_temp, --, b, out_j_hash, >>11]
    alu[out_j_hash, out_j_hash, xor, jh_temp]
    alu[jh_temp, --, b, out_j_hash, <<15]
    alu[out_j_hash, out_j_hash, +, jh_temp]

jenkins_hash_end#:

    // clean up namespace
    #undef  JH_LM_HANDLE
    #undef  JH_LM_ADDR
    #undef  JH_LM_INDEX
.end
#endm   /* jenkins_byte_hash */


/** Calculate Hash using IXP/NFP32XX hash hardware. Returns result LSW
 *
 * @b Example:
 * @code
 * localmem_set_address(0, 0, LM_HANDLE_0)
 * localmem_write4( 0x12345678, \
 *  	            0x11111111, \
 *	            0x22222222, \
 *	            0x33333333, \
 *	            0,0)
 * #define_eval LM_BASE_ADDR 0
 * #define_eval LM_SIZE_LWORDS 8
 * .reg r_hash_val
 * hardware_hash(r_hash_val, LM_BASE_ADDR, LM_SIZE_LWORDS, 0)
 * @endcode
 *
 * @param out_hw_hash      Register receives hash calculation result
 * @param in_lm_base_addr  Register or Constant, start address of data in local memory
 * @param in_data_size_lw  Register or Constant, size of data in lwords (only 4 is supported)
 * @param in_lm_handle     Constant, lm handle, either 0 or 1
 *
 * @note Not available for NFP6000. Use hash_init_cls() and hash_translate() instead.
 */
#macro hardware_hash(out_hw_hash, in_lm_base_addr, in_data_size_lw, in_lm_handle)
.begin

    #if (IS_NFPTYPE(__NFP6000) || IS_NFPTYPE(__NFP3800))
        #error "hardware_hash: This macro is not supported on NFP6000. Use cls_hash_init() and hash_translate() instead."
    #endif

    .reg    hw_len
    .reg    hw_temp

    xbuf_alloc($hw_hash_xfer, 4, read_write)

    .sig    hw_sig

    #define_eval HW_LM_HANDLE in_lm_handle
    #if (HW_LM_HANDLE == 0)
        #define HW_LM_ADDR      active_lm_addr_0
        #define HW_LM_INDEX     *l$index0
    #else
        #define HW_LM_ADDR      active_lm_addr_1
        #define HW_LM_INDEX     *l$index1
    #endif

    local_csr_wr[HW_LM_ADDR, in_lm_base_addr]

    immed[out_hw_hash, 0]    ; hash output can not be zero

    ; only 4 LWs in the key supported. will need enhancement for variable key size
    alu[$hw_hash_xfer0, --, b, HW_LM_INDEX++]
    alu[$hw_hash_xfer1, --, b, HW_LM_INDEX++]
    alu[$hw_hash_xfer2, --, b, HW_LM_INDEX++]
    alu[$hw_hash_xfer3, --, b, HW_LM_INDEX++]

    hash_128[$hw_hash_xfer0, 1], sig_done[hw_sig]
    ctx_arb[hw_sig]

    alu[out_hw_hash, --, b, $hw_hash_xfer0]

hardware_hash_end#:

    // clean up namespace
    xbuf_free($hw_hash_xfer)
    #undef  HW_LM_HANDLE
    #undef  HW_LM_ADDR
    #undef  HW_LM_INDEX
.end
#endm   /* hardware_hash */

/** @}
 * @}
 */

#endif  /* __FLETCHER_HASH_UC__ */

