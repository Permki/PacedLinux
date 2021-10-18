/*
 * Copyright (C) 2008-2011 Netronome Systems, Inc.  All rights reserved.
 */

#ifndef __RBUF_UC__
#define __RBUF_UC__

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
#include <nfp3200/nfp_msf.h>

/** @file rbuf.uc RBUF Operation Macros
 * @addtogroup rbuf RBUF Operation
 * @{
 *
 * @name RBUF Operation Macros
 * @{
 *
 */

// API Quick Reference:
//
// rbuf_addr_from_elem(out_rbuf_elem, in_rbuf_addr, ELEMENT_SIZE)
// rbuf_elem_free(in_rbuf_elem)
// rbuf_elem_from_addr(out_rbuf_elem, in_rbuf_addr, ELEMENT_SIZE)
// rbuf_read(out_xfer, in_rbuf_addr, in_addr_offset, in_qw_count, optional_token)



/** Convert rbuf element index to rbuf address.
 *
 * out_rbuf_addr = in_rbuf_elem  * ELEMENT_SIZE
 *
 * @param out_rbuf_addr   Byte address
 * @param in_rbuf_elem    rbuf element index
 * @param ELEMENT_SIZE    Size of rbuf element in bytes. Should be one of:
 *                        NETIF_ELEM_64, NETIF_ELEM_128, NETIF_ELEM_256
 *
 * @instruction_cnt 1
 *
 * @note Element numbering is according to size setting in msf.
 *       Therefore 128 byte elements are numbered 0, 2, 4, ...
 *
 * @b Example:
 * @code
 * rbuf_addr_from_elem(addr, elem, NETIF_ELEM_64)
 * @endcode
 */
#macro rbuf_addr_from_elem(out_rbuf_addr, in_rbuf_elem, ELEMENT_SIZE)
.begin

    #ifdef _SHF_AMOUNT
        #undef _SHF_AMOUNT
    #endif

    #if streq(ELEMENT_SIZE, NETIF_ELEM_64)
        #define _SHF_AMOUNT 6
    #elif streq(ELEMENT_SIZE, NETIF_ELEM_128)
        #define _SHF_AMOUNT 7
    #elif streq(ELEMENT_SIZE, NETIF_ELEM_256)
        #define _SHF_AMOUNT 8
    #else
        #error "Illegal ELEMENT_SIZE."
    #endif

    #ifdef __SHF_AMOUNT
        #warning "__SHF_AMOUNT is being redefined."
    #endif

    #if IS_IXPTYPE(__IXP28XX)
        #define_eval __SHF_AMOUNT   6
    #else
        #error "Chip type must be IXP28XX."
    #endif

    shf_left(out_rbuf_addr, in_rbuf_elem, __SHF_AMOUNT)
    #undef __SHF_AMOUNT

.end
#endm


/** Free an rbuf element so it can be reused.
 *
 * The MSF hardware manages the elements and makes
 * them available for use at reset or when microthreads frees them.
 * Elements are consumed when data is received from the network.
 *
 * @param in_rbuf_elem     rbuf element index
 *
 * @instruction_cnt 2-3
 */
#macro rbuf_elem_free(in_rbuf_elem)
.begin

    .reg AddrAndData
    // For fast_wr the address is in bits 15:2 & data is in bits 31:16
    #if (is_ct_const (in_rbuf_elem) || isimport(in_rbuf_elem))
        #ifdef __RBUF_ELEM_FAST_WR_DATA
            #warning "__RBUF_ELEM_FAST_WR_DATA is being redefined."
        #endif
        #define_eval __RBUF_ELEM_FAST_WR_DATA ((NFP_MSF_RBUF_ELEM_DONE) | (in_rbuf_elem<<16))
        move(AddrAndData, __RBUF_ELEM_FAST_WR_DATA)
        msf[fast_wr, --, AddrAndData, 0]
        #undef __RBUF_ELEM_FAST_WR_DATA
    #else
        #define __SHIFTED_RBUF_ELEMENT_DONE_ADDR (NFP_MSF_RBUF_ELEM_DONE)
        move(AddrAndData, __SHIFTED_RBUF_ELEMENT_DONE_ADDR)
        alu_shf[AddrAndData, AddrAndData, OR, in_rbuf_elem, <<16]
        msf[fast_wr, --, AddrAndData, 0]
    #endif

.end
#endm


/** Convert rbuf address to rbuf element index.
 *
 * out_index = in_rbuf_addr / ELEMENT_SIZE
 *
 * @param out_rbuf_elem   rbuf element index
 * @param in_rbuf_addr    Byte address
 * @param ELEMENT_SIZE    Size of rbuf element in bytes. Should be one of:
 *                        NETIF_ELEM_64, NETIF_ELEM_128, NETIF_ELEM_256
 *
 * @instruction_cnt 1
 *
 * @note Element numbering is according to size setting in msf.
 *       Therefore 128B elements are numbered 0, 2, 4, ...
 *
 * @b Example:
 * @code
 * rbuf_elem_from_addr(elem, addr, NETIF_ELEM_64)
 * @endcode
 */
#macro rbuf_elem_from_addr(out_rbuf_elem, in_rbuf_addr, ELEMENT_SIZE)
.begin

    #ifdef _SHF_AMOUNT
        #undef _SHF_AMOUNT
    #endif
    #if streq(ELEMENT_SIZE, NETIF_ELEM_64)
        #define _SHF_AMOUNT 6
    #elif streq(ELEMENT_SIZE, NETIF_ELEM_128)
        #define _SHF_AMOUNT 7
    #elif streq(ELEMENT_SIZE, NETIF_ELEM_256)
        #define _SHF_AMOUNT 8
    #else
        #error "Illegal ELEMENT_SIZE."
    #endif

    #ifdef __SHF_AMOUNT
        #warning "__SHF_AMOUNT is being redefined."
    #endif

    #if IS_IXPTYPE(__IXP28XX)
        #define_eval __SHF_AMOUNT   6
    #else
        #error "Chip type must be IXP28XX."
    #endif

    shf_right(out_rbuf_elem, in_rbuf_addr, __SHF_AMOUNT)
    #undef __SHF_AMOUNT

.end
#endm


/** Read from rbuf.
 *
 * @param out_xfer         First xfer register of sequence to read to, array notation
 * @param in_rbuf_addr     Register or constant base 64 bit (quadword) address
 * @param in_addr_offset   Register or constant 64 bit (quadword) address offset
 * @param in_qw_count      Register or constant 64 bit (quadword) count
 * @param REQ_SIG          Signal associated with this request
 * @param in_wakeup_sigs   Signal or signals to wake up on
 * @param Q_OPTION         Queue option
 *
 * @instruction_cnt 3-8
 *
 * @b Example:
 * @code
 * rbuf_read($xfer[1], base_addr, base_offset, ref_count, ctx_swap]
 * @endcode
 */
#macro rbuf_read(out_xfer, in_rbuf_addr, in_addr_offset, in_qw_count, REQ_SIG, in_wakeup_sigs, Q_OPTION)
.begin

    xbuf_find(out_xfer)     // if found, #define _xbuf_found, _cur_d_xbuf_name, _cur_d_xbuf_size
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
        _rbuf_read_to_xbufs(in_rbuf_addr, in_addr_offset, _act_wd_count, REQ_SIG, in_wakeup_sigs, Q_OPTION)
        #undef _perform_xbuf_op
    #elif (is_ct_const (in_qw_count) || isimport(in_qw_count))
        _rbuf_instr(read64, out_xfer, in_rbuf_addr, in_addr_offset, _act_wd_count, REQ_SIG, in_wakeup_sigs, Q_OPTION)
    #else
        _rbuf_ind_instr(read64, out_xfer, in_rbuf_addr, in_addr_offset, in_qw_count, REQ_SIG, in_wakeup_sigs, Q_OPTION)
    #endif  // (is_ct_const (in_qw_count))
    #ifdef _act_wd_count
        #undef _act_wd_count
    #endif

.end
#endm


/// @cond INTERNAL_MACROS
/** Read to one or more (linked) xbufs.
 *
 * Support macro rbuf_read.
 */
#macro _rbuf_read_to_xbufs(in_rbuf_addr, in_addr_offset, in_qw_count, REQ_SIG, in_wakeup_sigs, Q_OPTION)
    #ifdef _rem_buf
        #warning "_rem_buf is being redefined."
    #endif
    // xbuf_find in caller previously set these while determining whether to use xbufs
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
        alu_op(new_base_addr, in_rbuf_addr, +, in_addr_offset)
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

        _rbuf_instr(read64, _cur_xbuf_name/**/_cur_xbuf_index, new_base_addr, _cur_offset, _cur_qw_count, tmp_signalp, SIG_NONE, Q_OPTION)

        #define _SECOND_SIG
        #define_eval _cur_qw_offset (_cur_qw_offset + _cur_qw_count)
        #define_eval _cur_lw_count (_cur_lw_count - _rem_buf)
        #define_eval _cur_qw_count (_cur_lw_count / 2)

        #ifdef _cur_xbuf_next
            xbuf_param_set(_cur_xbuf_next)
            #define_eval _cur_xbuf_index 0
            #define_eval _rem_buf _cur_xbuf_size
        #else
            #error  "_rbuf_read_to_xbufs. out of xbuf range"
        #endif  //  _cur_xbuf_next
    #endloop

    // perform last operation
    #define_eval _cur_offset (_cur_qw_offset <<3)       // convert back to byte address

    #ifdef _use_new_base
        #ifdef _SECOND_SIG
            _rbuf_instr(read64, _cur_xbuf_name/**/_cur_xbuf_index, new_base_addr, _cur_offset, _cur_qw_count, REQ_SIG, signals(tmp_signalp, in_wakeup_sigs), Q_OPTION)
            #undef _SECOND_SIG
        #else
            _rbuf_instr(read64, _cur_xbuf_name/**/_cur_xbuf_index, new_base_addr, _cur_offset, _cur_qw_count, REQ_SIG, in_wakeup_sigs, Q_OPTION)
        #endif

        #undef _use_new_base
    #else
        #ifdef _SECOND_SIG
            _rbuf_instr(read64, _cur_xbuf_name/**/_cur_xbuf_index, in_rbuf_addr, _cur_offset, _cur_qw_count, REQ_SIG, signals(tmp_signalp, in_wakeup_sigs), Q_OPTION)
            #undef _SECOND_SIG
        #else
            _rbuf_instr(read64, _cur_xbuf_name/**/_cur_xbuf_index, in_rbuf_addr, _cur_offset, _cur_qw_count, REQ_SIG, in_wakeup_sigs, Q_OPTION)
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
/** Instantiate the msf instruction,
 * convert REQ_SIG, in_wakeup_sigs, Q_OPTION to the correct optional tokens.
 *
 * Arguments: See rbuf_read common Output/Inputs.
 */
#macro _rbuf_instr(op_code, io_xfer, in_rbuf_addr, in_addr_offset, in_wd_count, REQ_SIG, in_wakeup_sigs, Q_OPTION)
    _DEF_MEM_TOKENS(REQ_SIG, in_wakeup_sigs, Q_OPTION, in_wd_count)
    _GET_ADDRESSES(_MEM_ADDR0, _MEM_ADDR1, in_rbuf_addr, in_addr_offset, RBUF)

    msf[op_code, io_xfer, _MEM_ADDR0, _MEM_ADDR1, in_wd_count], _MEM_TOKENS

    _FREE_ADDRESSES()
    #ifdef _HW_SIG_EXPR
        #undef _HW_SIG_EXPR
        ctx_arb[in_wakeup_sigs]
    #endif
#endm
/// @endcond


/// @cond INTERNAL_MACROS
/** Instantiate the msf instruction,
 * convert REQ_SIG, in_wakeup_sigs, Q_OPTION to the correct optional tokens.
 * Insert indirect token.
 *
 * Arguments: See rbuf_read common Output/Inputs.
 */
#macro _rbuf_ind_instr(op_code, io_xfer, in_rbuf_addr, in_addr_offset, in_wd_count, REQ_SIG, in_wakeup_sigs, Q_OPTION)
    _DEF_MEM_TOKENS(REQ_SIG, in_wakeup_sigs, Q_OPTION, in_wd_count)

    _GET_ADDRESSES(_MEM_ADDR0, _MEM_ADDR1, in_rbuf_addr, in_addr_offset, RBUF)

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
            alu[ref_count, in_wd_count, -, 1]
            alu_shf[ref_count, --, b, ref_count, <<_REF_CNT_OFFSET]
        #else
            alu_shf[ref_count, --, b, in_wd_count, <<_REF_CNT_OFFSET]
        #endif

        alu_shf[ref_count, ref_count, OR, 1, <<_REF_CNT_OV_OFFSET]
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

#endif /* __RBUF_UC__ */
