/*
 * Copyright (C) 2008-2011 Netronome Systems, Inc.  All rights reserved.
 */

#ifndef __TBUF_UC__
#define __TBUF_UC__

#ifndef NFP_LIB_ANY_NFAS_VERSION
    #if (!defined(__NFP_TOOL_NFAS) ||                       \
        (__NFP_TOOL_NFAS < NFP_SW_VERSION(5, 0, 0, 0)) ||   \
        (__NFP_TOOL_NFAS > NFP_SW_VERSION(6, 1, 0, 255)))
        #error "This standard library is not supported for the version of the SDK currently in use."
    #endif
#endif

#if (IS_NFPTYPE(__NFP6000) || IS_NFPTYPE(__NFP3800))
        #error "This standard library is not supported for the NFP6000."
#endif

#include <constants.uc>
#include <stdmac.uc>
#include <xbuf.uc>
#include <util.uc>

/** @file tbuf.uc TBUF Operation Macros
 * @addtogroup tbuf TBUF Operation
 * @{
 *
 * @name TBUF Operation Macros
 * @{
 *
 */

// API Quick Refernece:
//
// tbuf_addr_from_elem (out_tbuf_elem, in_tbuf_addr, ELEMENT_SIZE)
// tbuf_elem_from_addr (out_tbuf_elem, in_tbuf_addr, ELEMENT_SIZE)
// tbuf_write (in_data, in_tbuf_addr, in_addr_offset, in_qw_count, REQ_SIG, wakeup_sigs, Q_OPTION)



/** Convert TBUF element index to TBUF address.
 *
 * @param out_tbuf_addr TBUF address
 * @param in_tbuf_elem  TBUF element index
 * @param ELEMENT_SIZE  Size of TBUF element in bytes @n @n
 *                      Possible values are: @n
 *                      @arg @c NETIF_ELEM_64
 *                      @arg @c NETIF_ELEM_128
 *                      @arg @c NETIF_ELEM_256
 *
 * @note @a out_tbuf_addr = out_index * @a ELEMENT_SIZE
 *
 * @instruction_cnt 1
 */
#macro tbuf_addr_from_elem(out_tbuf_addr, in_tbuf_elem, ELEMENT_SIZE)
.begin

    // Even though we can ignore element size for  IXP28XX,
    // we may have to use it for 24 version of NP
    #ifdef _SHF_AMOUNT
        #undef _SHF_AMOUNT
    #endif

    #if streq (ELEMENT_SIZE, NETIF_ELEM_64)
        #define _SHF_AMOUNT 6
    #elif streq (ELEMENT_SIZE, NETIF_ELEM_128)
        #define _SHF_AMOUNT 7
    #elif streq(ELEMENT_SIZE, NETIF_ELEM_256)
        #define _SHF_AMOUNT 8
    #else
        #error "illegal ELEMENT_SIZE"
    #endif

    #ifdef __SHF_AMOUNT
        #warning "__SHF_AMOUNT is being redefined."
    #endif

    #if IS_IXPTYPE(__IXP28XX)
        #define_eval __SHF_AMOUNT   6
    #else
        #error "Chip type must be IXP28XX."
    #endif

    shf_left(out_tbuf_addr, in_tbuf_elem, __SHF_AMOUNT)
    #undef __SHF_AMOUNT

.end
#endm


/** Convert TBUF address to element index.
 *
 * @param out_tbuf_elem TBUF element index
 * @param in_tbuf_addr  TBUF address
 * @param ELEMENT_SIZE  Size of TBUF element in bytes @n @n
 *                      Possible values are: @n
 *                      @arg @c NETIF_ELEM_64
 *                      @arg @c NETIF_ELEM_128
 *                      @arg @c NETIF_ELEM_256
 *
 * @note out_index = @a out_tbuf_addr * @a ELEMENT_SIZE
 *
 * @instruction_cnt 1
 */
#macro tbuf_elem_from_addr(out_tbuf_elem, in_tbuf_addr, ELEMENT_SIZE)
.begin

    // we can ignore element size for IXP28XX,
    // but use it for 24 NP
    #ifdef _SHF_AMOUNT
        #undef _SHF_AMOUNT
    #endif

    #if streq (ELEMENT_SIZE, NETIF_ELEM_64)
        #define_eval _SHF_AMOUNT 6
    #elif streq (ELEMENT_SIZE, NETIF_ELEM_128)
        #define_eval _SHF_AMOUNT 7
    #elif streq(ELEMENT_SIZE, NETIF_ELEM_256)
        #define_eval _SHF_AMOUNT 8
    #else
        #error "illegal ELEMENT_SIZE"
    #endif

    #ifdef __SHF_AMOUNT
        #warning "__SHF_AMOUNT is being redefined."
    #endif

    #if IS_IXPTYPE(__IXP28XX)
        #define_eval __SHF_AMOUNT   6
    #else
        #error "Chip type must be IXP28XX."
    #endif

    shf_right(out_tbuf_elem, in_tbuf_addr, __SHF_AMOUNT)
    #undef __SHF_AMOUNT

.end
#endm


/** Write @a in_qw_count quadwords to Transmit Buffer (TBUF).
 *
 * @b Example:
 * @code
 * tbuf_write($xfer[1], base_addr, base_offset, ref_count, SIG_COMMON, SIG_COMMON, ___)
 * @endcode
 *
 * @param in_xfer           Data to write
 * @param in_tbuf_addr      TBUF address
 * @param in_addr_offset    Microengine only. Offset to be added to @a in_tbuf_addr
 *                          to form the address to be accessed
 * @param in_qw_count       Register or constant 64-bit (quadword) count. The minimum quadword count is 1
 * @param REQ_SIG           Requested signal
 * @param in_wakeup_sigs    List of signals causing thread to swap/wakeup
 * @param Q_OPTION          Directive for memory controller queue selection
 *
 * @instruction_cnt 1 to 7
 */
#macro tbuf_write(in_xfer, in_tbuf_addr, in_addr_offset, in_qw_count, REQ_SIG, in_wakeup_sigs, Q_OPTION)
.begin

    xbuf_find(in_xfer)      // if found, #define _xbuf_found, _cur_d_xbuf_name, _cur_d_xbuf_size
    #ifdef _act_wd_count
        #warning "_act_wd_count is being redefined."
    #endif
    #if (is_ct_const (in_qw_count) || isimport(in_qw_count))
        #ifdef _xbuf_found
            #define _perform_xbuf_op
        #endif
        // if all word counts are n-1
        #if (MEM_WD_COUNT_MIN_1 == TRUE)
            #define_eval _act_wd_count (in_qw_count + 1)
        #else
            #define_eval _act_wd_count (in_qw_count)
        #endif
    #endif
    #ifdef _perform_xbuf_op
        _tbuf_write_from_xbufs(in_tbuf_addr, in_addr_offset, _act_wd_count, REQ_SIG, in_wakeup_sigs, Q_OPTION)
        #undef _perform_xbuf_op
    #elif (is_ct_const (in_qw_count) || isimport(in_qw_count))
        _tbuf_instr(write64, in_xfer, in_tbuf_addr, in_addr_offset, _act_wd_count, REQ_SIG, in_wakeup_sigs, Q_OPTION)
    #else
        _tbuf_ind_instr(write64, in_xfer, in_tbuf_addr, in_addr_offset, in_qw_count, REQ_SIG, in_wakeup_sigs, Q_OPTION)
    #endif  // (is_ct_const (in_qw_count))
    #ifdef _act_wd_count
        #undef _act_wd_count
    #endif

.end
#endm


/// @cond INTERNAL_MACROS
/** Internal macro to write from one or more (linked) xbufs.
 *
 * Supports the @ref tbuf_write macro.
 *
 * @param in_tbuf_addr      TBUF address
 * @param in_addr_offset    Microengine only. Offset to be added to @a in_tbuf_addr
 *                          to form the address to be accessed
 * @param in_qw_count       Register or constant 64-bit (quadword) count. The minimum quadword count is 1
 * @param REQ_SIG           Requested signal
 * @param in_wakeup_sigs    List of signals causing thread to swap/wakeup
 * @param Q_OPTION          Directive for memory controller queue selection
 *
 * @see Common section "Signal Arguments."
 * @see Common section "microengineC Common Types. queue_t."
 *
 * @instruction_cnt 2 to 4 and 3 to 6 for linked xbufs
 */
#macro _tbuf_write_from_xbufs(in_tbuf_addr, in_addr_offset, in_qw_count, REQ_SIG, in_wakeup_sigs, Q_OPTION)
    // xbuf_find in caller previously set these while determining whether to use xbufs
    #ifdef _rem_buf
        #warning "_rem_buf is being redefined."
    #endif
    #define_eval _rem_buf (_cur_xbuf_size - _cur_xbuf_index)

    #ifdef _cur_lw_count
        #warning "_cur_lw_count is being redefined."
    #endif
    #ifdef _cur_qw_count
        #warning "_cur_qw_count is being redefined."
    #endif
    // to enter this macro, in_qw_count had to be a constant
    #define_eval _cur_lw_count (in_qw_count * 2)
    #define_eval _cur_qw_count (in_qw_count)
    #if (_cur_lw_count > _rem_buf)
        #define _use_new_base
    #endif

    #ifdef _cur_qw_offset
        #warning "_cur_qw_offset is being redefined."
    #endif
    #define_eval _cur_qw_offset (in_addr_offset)

    // determine quadword offset
    #if (is_ct_const(in_addr_offset) || isimport(in_addr_offset))
        #define_eval _cur_qw_offset (in_addr_offset >>3)        // convert from byte address
    #else
        #ifndef _use_new_base
            #define _use_new_base
        #endif
    #endif

    #ifdef _use_new_base
        .reg new_base_addr
        alu_op(new_base_addr, in_tbuf_addr, +, in_addr_offset)
        #ifdef _cur_lw_offset
            #warning "_cur_lw_offset is being redefined."
        #endif
        #define_eval _cur_lw_offset (0)
        #define_eval _cur_qw_offset (0)
    #endif

    #while (_cur_lw_count > _rem_buf)
        #ifdef _SECOND_SIG
            #error "maximum of 2 linked xbufs supported for a single memory operation"
        #endif
        .sig tmp_signalp

        #define_eval _cur_qw_count (_rem_buf / 2)
        #ifdef _cur_offset
            #warning "_cur_offset is being redefined."
        #endif

        #define_eval _cur_offset (_cur_qw_offset <<3)       // convert back to byte address

        _tbuf_instr(write64, _cur_xbuf_name/**/_cur_xbuf_index, new_base_addr, _cur_offset, _cur_qw_count, tmp_signalp, SIG_NONE, Q_OPTION)

        #define _SECOND_SIG
        #define_eval _cur_qw_offset (_cur_qw_offset + _cur_qw_count)
        #define_eval _cur_lw_count (_cur_lw_count - _rem_buf)
        #define_eval _cur_qw_count (_cur_lw_count / 2)

        #ifdef _cur_xbuf_next
            xbuf_param_set(_cur_xbuf_next)
            #define_eval _cur_xbuf_index 0
            #define_eval _rem_buf _cur_xbuf_size
        #else
            #error  "_tbuf_write_from_xbufs. out of xbuf range"
        #endif  //  _cur_xbuf_next
    #endloop

    // perform last operation

        #define_eval _cur_offset (_cur_qw_offset <<3)       // convert back to byte address

    #ifdef _use_new_base
        #ifdef _SECOND_SIG
            _tbuf_instr(write64, _cur_xbuf_name/**/_cur_xbuf_index, new_base_addr, _cur_offset, _cur_qw_count, REQ_SIG, signals(tmp_signalp, in_wakeup_sigs), Q_OPTION)
            #undef _SECOND_SIG
        #else
            _tbuf_instr(write64, _cur_xbuf_name/**/_cur_xbuf_index, new_base_addr, _cur_offset, _cur_qw_count, REQ_SIG, in_wakeup_sigs, Q_OPTION)

        #endif

        #undef _use_new_base
    #else
        #ifdef _SECOND_SIG
            _tbuf_instr(write64, _cur_xbuf_name/**/_cur_xbuf_index, in_tbuf_addr, _cur_offset, _cur_qw_count, REQ_SIG, signals(tmp_signalp, in_wakeup_sigs), Q_OPTION)
            #undef _SECOND_SIG
        #else
            _tbuf_instr(write64, _cur_xbuf_name/**/_cur_xbuf_index, in_tbuf_addr, _cur_offset, _cur_qw_count, REQ_SIG, in_wakeup_sigs, Q_OPTION)
        #endif
    #endif

    // clean up name space
    #ifdef _cur_offset
        #undef _cur_offset
    #endif
    #ifdef _cur_lw_offset
        #undef _cur_lw_offset
    #endif

    #undef _cur_qw_offset
    #undef _cur_lw_count
    #undef _cur_qw_count
    #undef _rem_buf
#endm
/// @endcond


/// @cond INTERNAL_MACROS
/** Internal macro to instantiate the MSF instruction, convert @a REQ_SIG, @a in_wakeup_sigs,
 *  @a Q_OPTION to the correct optional tokens.
 *
 * Supports the @ref tbuf_write macro.
 *
 * @param op_code           Vaild opcode
 * @param in_xfer           Data to write
 * @param in_tbuf_addr      TBUF address
 * @param in_addr_offset    Microengine only. Offset to be added to @a in_tbuf_addr
 *                          to form the address to be accessed
 * @param in_qw_count       Register or constant 64-bit (quadword) count. The minimum quadword count is 1
 * @param REQ_SIG           Requested signal
 * @param in_wakeup_sigs    List of signals causing thread to swap/wakeup
 * @param Q_OPTION          Directive for memory controller queue selection
 *
 * @see Common section "Signal Arguments."
 * @see Common section "microengineC Common Types. queue_t."
 *
 * @instruction_cnt 1 to 3
 */
#macro _tbuf_instr(op_code, io_xfer, in_tbuf_addr, in_addr_offset, in_wd_count, REQ_SIG, in_wakeup_sigs, Q_OPTION)
    _DEF_MEM_TOKENS(REQ_SIG, in_wakeup_sigs, Q_OPTION, in_wd_count)

    _GET_ADDRESSES(_MEM_ADDR0, _MEM_ADDR1, in_tbuf_addr, in_addr_offset, tbuf)

    msf[op_code, io_xfer, _MEM_ADDR0, _MEM_ADDR1, in_wd_count], _MEM_TOKENS

    _FREE_ADDRESSES()

    #ifdef _HW_SIG_EXPR
        #undef _HW_SIG_EXPR
        ctx_arb[in_wakeup_sigs]
    #endif
#endm
/// @endcond


/// @cond INTERNAL_MACROS
/** Internal macro to instantiate the MSF instruction, convert @a REQ_SIG, @a in_wakeup_sigs,
 *  @a Q_OPTION to the correct optional tokens. Insert indirect token.
 *
 * Supports the @ref tbuf_write macro.
 *
 * @param op_code           Vaild opcode
 * @param in_xfer           Data to write
 * @param in_tbuf_addr      TBUF address
 * @param in_addr_offset    Microengine only. Offset to be added to @a in_tbuf_addr
 *                          to form the address to be accessed
 * @param in_qw_count       Register or constant 64-bit (quadword) count. The minimum quadword count is 1
 * @param REQ_SIG           Requested signal
 * @param in_wakeup_sigs    List of signals causing thread to swap/wakeup
 * @param Q_OPTION          Directive for memory controller queue selection
 *
 * @instruction_cnt 5 to 7
 */
#macro _tbuf_ind_instr(op_code, io_xfer, in_tbuf_addr, in_addr_offset, in_wd_count, REQ_SIG, in_wakeup_sigs, Q_OPTION)
    _DEF_MEM_TOKENS(REQ_SIG, in_wakeup_sigs, Q_OPTION, in_wd_count)

    _GET_ADDRESSES(_MEM_ADDR0, _MEM_ADDR1, in_tbuf_addr, in_addr_offset, tbuf)

    xbuf_find(io_xfer)      // if found, #define _xbuf_found, _cur_xbuf_name, _cur_xbuf_size
    #ifdef _NN_SIZE
        #warning "_NN_SIZE is being redefined."
    #endif
    #ifdef _MAX_NN
        #warning "_MAX_NN is being redefined."
    #endif
    #ifdef _xbuf_found
        #define_eval _NN_SIZE ((_cur_xbuf_size - _cur_xbuf_index) / 2)
        #define_eval _MAX_NN max_/**/_NN_SIZE
    #else
        #define_eval _MAX_NN MAX
    #endif

    .reg ref_count

    #if (defined(__NFP_INDIRECT_REF_FORMAT_IXP))
        // if all memory word counts are n
        #if (MEM_WD_COUNT_MIN_1 != TRUE)
            // convert to n-1
            sub(ref_count, in_wd_count, 1)
            shf_left(ref_count, ref_count, REF_CNT_OFFSET)
        #else
            shf_left(ref_count, in_wd_count, REF_CNT_OFFSET)
        #endif

        alu_shf_left(ref_count, ref_count, OR, 1, REF_CNT_OV_OFFSET)
        move(--, ref_count)
    #else
        #error "This macro is currently only available in IXP indirect reference format mode."
    #endif

    msf[op_code, io_xfer, _MEM_ADDR0, _MEM_ADDR1, _MAX_NN], indirect_ref, _MEM_TOKENS

    _FREE_ADDRESSES()

    #ifdef _HW_SIG_EXPR
        #undef _HW_SIG_EXPR
        ctx_arb[in_wakeup_sigs]
    #endif
    #ifdef _NN_SIZE
        #undef _NN_SIZE
    #endif
    #undef _MAX_NN
#endm
/// @endcond


/** @}
 * @}
 */

#endif /* __TBUF_UC__ */
