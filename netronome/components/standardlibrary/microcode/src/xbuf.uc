/*
 * Copyright (C) 2008-2013 Netronome Systems, Inc.  All rights reserved.
 */

#ifndef __XBUF_UC__
#define __XBUF_UC__

#ifndef NFP_LIB_ANY_NFAS_VERSION
    #if (!defined(__NFP_TOOL_NFAS) ||                       \
        (__NFP_TOOL_NFAS < NFP_SW_VERSION(5, 0, 0, 0)) ||   \
        (__NFP_TOOL_NFAS > NFP_SW_VERSION(6, 1, 0, 255)))
        #error "This standard library is not supported for the version of the SDK currently in use."
    #endif
#endif

#include <bytefield.uc>
#include <constants.uc>

/** @file xbuf.uc XBUF Operation Macros
 * @addtogroup xbuf XBUF Operation
 * @{
 *
 * @name XBUF Operation Macros
 * @{
 * XBUF macros
 */

// API Quick Reference:
//
// xbuf_alloc(str_xbuf_name, NUMBER_OF_REGS, RW_INDICATOR)
// xbuf_free(str_xbuf_name)
// xbuf_link(str_xbuf_name, str_nextxbuf_name)
// xbuf_param_set(str_xbuf_name)
// xbuf_xfer_set(XFER_TOKEN, str_xbuf_name, BYTE_OFFSET)
// xbuf_extract(out_byte_field, str_xbuf_name, window_start, field_start, number_of_bytes)
// xbuf_extract_frm_linked_bufs(out_byte_field, str_xbuf_name, window_start,
//                              field_start, number_of_bytes, DATA_SPREAD)
// xbuf_type_extract(out_field, str_xbuf_name, WINDOW_START, FIELD_START, SIZE, DATATYPE)
// xbuf_insert(io_str_xbuf_name, in_byte_field, window_start, field_start, number_of_bytes)
// xbuf_copy(out_dest_xbuf, out_last_element, dest_start_byte, in_src_xbuf,
//           src_start_byte, in_prepend, total_bytes_to_copy, DATA_SPREAD)
//
// Internal macros: FOR DRAM
//  _xbuf_d_alloc(str_xbuf_name, NUMBER_OF_REGS, RW_INDICATOR)
//  _xbuf_d_free(str_xbuf_name)
//  _xbuf_d_link(str_xbuf_name, str_nextxbuf_name)
//  _xbuf_d_param_set(str_xbuf_name)
//
// Internal macros: FOR GPR
//  _xbuf_gpr_alloc(str_xbuf_name, NUMBER_OF_REGS, RW_INDICATOR)
//  _xbuf_gpr_free(str_xbuf_name)
//  _xbuf_gpr_link(str_xbuf_name, str_nextxbuf_name)
//  _xbuf_gpr_param_set(str_xbuf_name)
//
// Internal macros: FOR localmem
//  _xbuf_lmem_alloc(str_xbuf_name, NUMBER_OF_REGS, RW_INDICATOR)
//  _xbuf_lmem_free(str_xbuf_name)
//  _xbuf_lmem_link(str_xbuf_name, str_nextxbuf_name)
//  _xbuf_lmem_param_set(str_xbuf_name)
//
//Internal macros
//  _xbuf_copy_const_offsets_and_size(out_dest_xbuf, out_last_element,
//              DEST_START_BYTE, in_src_xbuf, SRC_START_BYTE, in_prepend,
//              TOTAL_BYTES_TO_COPY, DATA_SPREAD)
//  _copy_aligned_bufs_const_offsets_size(out_dest_xbuf, out_last_element,
//              DEST_START_BYTE, in_src_xbuf, SRC_START_BYTE, TOTAL_BYTES_TO_COPY)
//  _align_and_copy_const_offsets_size(out_dest_xbuf, out_last_element,
//              DEST_START_BYTE, in_src_xbuf,
//              SRC_START_BYTE, in_prepend, TOTAL_BYTES_TO_COPY)
//  _copy_var_aligned_dest_const_aligned_src_const_size(
//              out_dest_xbuf, out_last_element, dest_start_byte,
//              in_src_xbuf, src_start_byte, in_prepend, total_bytes_to_copy)
//  _copy_var_dest_const_src_and_size(out_dest_xbuf, out_last_element,
//              dest_start_byte, in_src_xbuf, src_start_byte,
//              in_prepend, total_bytes_to_copy)
//  _copy_var_src_const_dest_and_size(out_dest_xbuf, out_last_element,
//              dest_start_byte, in_src_xbuf, src_start_byte,
//              in_prepend, total_bytes_to_copy)
//  _copy_var_offsets_const_size(out_dest_xbuf, out_last_element,
//              dest_start_byte, in_src_xbuf, src_start_byte,
//              in_prepend, total_bytes_to_copy)
//  _copy_var_dest_var_size_const_src(out_dest_xbuf, out_last_element,
//              dest_start_byte, in_src_xbuf, src_start_byte,
//              in_prepend, total_bytes_to_copy)
//  _copy_var_src_var_size_const_dest(out_dest_xbuf, out_last_element,
//              dest_start_byte, in_src_xbuf, src_start_byte,
//              in_prepend, total_bytes_to_copy)



/// @cond INTERNAL_MACROS
// Defines for XBUF_TYPE
#define _XBUF_SRAM_XFER     1
#define _XBUF_DRAM_XFER     2
#define _XBUF_GPR           3
#define _XBUF_LMEM          4
//Defines for INSERT or EXTRACT
#define     _EXTRACT        1
#define     _INSERT         2
/// @endcond

// Constant used in xbuf_type_extract
#define BIG_ENDIAN_BYTES 0      // data is big-endian in memory
#define LITTLE_ENDIAN_BYTES 1   // data is little-endian in memory, perform swap

#if defined(__NFP_INDIRECT_REF_FORMAT_V1)
    #define __NFP_INDIRECT_REF_FORMAT_NFPXXXX
#elif defined(__NFP_INDIRECT_REF_FORMAT_V2 )
    #define __NFP_INDIRECT_REF_FORMAT_NFPXXXX
#endif

// Supress warnings, "(5145) Register "xx" should be declared as an aggregate"
#pragma warning (disable: 5145)


/// @cond INTERNAL_MACROS
/** An internal macro that returns log to the base of 2 for the value, i.e. 2**out_put = value.
 *
 * In other words, given a value (power of 2), this macro will return the shift amount.
 *
 * @param val Value (must be a power of 2)
 *
 * @macro_define _LOG_BASE_2
 *
 * @instruction_cnt 0. All are pre-processing.
 *
 * @note Callers have to undefine _LOG_BASE_2 after use.
 *
 * <b>Example:</b>
 *
 * @code
   _xbuf_find_log_base_2(64)
   _LOG_BASE_2 will be defined to be 6 (2**6 = 64)
 * @endcode
 */
#macro _xbuf_find_log_base_2(val)
    #ifdef _LOG_BASE_2
        #warning "_LOG_BASE_2 is being redefined."
    #endif

    #ifdef _LOG_BASE_2_VAL
        #warning "_LOG_BASE_2_VAL is being redefined."
    #endif

    #define_eval    _LOG_BASE_2         (0)
    #define_eval    _LOG_BASE_2_VAL     (val)

    //  Find the bit position where a '1' (and only one '1') is set
    //  in value.
    #ifdef _IS_SET
        #warning "_IS_SET is being redefined."
    #endif
    #while (_LOG_BASE_2_VAL)
        #define_eval _IS_SET    (_LOG_BASE_2_VAL & 0x1)

        #if (_IS_SET == 0)
            #define_eval    _LOG_BASE_2     (_LOG_BASE_2 + 1)
            #define_eval    _LOG_BASE_2_VAL (_LOG_BASE_2_VAL >> 1)
        #else
            #define_eval    _LOG_BASE_2_VAL 0
        #endif
    #endloop

    #undef  _IS_SET
    #undef  _LOG_BASE_2_VAL
#endm
/// @endcond


/// @cond INTERNAL_MACROS
/** An internal macro that associates the localmem buffer name to *l$index0 or *l$index1.
 *
 * This macro assumes the buffer name exists.
 *
 * @param str_xbuf_name   Valid Local Memory buffer name
 * @param INDEX           Local Memory handle: 0 or 1, for *l$index0 or *l$index1.
 *
 * @macro_define *l$index0 or *l$index1
 *
 * @instruction_cnt 0
 */
#macro _xbuf_associate_lmem_regs(str_xbuf_name, INDEX)
    #define_eval _the_xbuf_name (str_xbuf_name)

    #if (INDEX == 0)
        #if (streq(_the_xbuf_name, 'lmem_buf0'))
            #define_eval lmem_buf0      *l$index0
        #elif (streq(_the_xbuf_name, 'lmem_buf1'))
            #define_eval lmem_buf1      *l$index0
        #elif (streq(_the_xbuf_name, 'lmem_buf2'))
            #define_eval lmem_buf2      *l$index0
        #elif (streq(_the_xbuf_name, 'lmem_buf3'))
            #define_eval lmem_buf3      *l$index0
        #elif (streq(_the_xbuf_name, 'lmem_buf4'))
            #define_eval lmem_buf4      *l$index0
        #elif (streq(_the_xbuf_name, 'lmem_buf5'))
            #define_eval lmem_buf5      *l$index0
        #elif (streq(_the_xbuf_name, 'lmem_buf6'))
            #define_eval lmem_buf6      *l$index0
        #elif (streq(_the_xbuf_name, 'lmem_buf7'))
            #define_eval lmem_buf7      *l$index0
        #elif (streq(_the_xbuf_name, 'lmem_buf8'))
            #define_eval lmem_buf8      *l$index0
        #elif (streq(_the_xbuf_name, 'lmem_buf9'))
            #define_eval lmem_buf9      *l$index0
        #elif (streq(_the_xbuf_name, 'lmem_buf10'))
            #define_eval lmem_buf10     *l$index0
        #elif (streq(_the_xbuf_name, 'lmem_buf11'))
            #define_eval lmem_buf11     *l$index0
        #elif (streq(_the_xbuf_name, 'lmem_buf12'))
            #define_eval lmem_buf12     *l$index0
        #elif (streq(_the_xbuf_name, 'lmem_buf13'))
            #define_eval lmem_buf13     *l$index0
        #elif (streq(_the_xbuf_name, 'lmem_buf14'))
            #define_eval lmem_buf14     *l$index0
        #elif (streq(_the_xbuf_name, 'lmem_buf15'))
            #define_eval lmem_buf15     *l$index0
        #else
            #error "Invalid str_xbuf_name parameter value."
        #endif
    #elif (INDEX == 1)
        #if (streq(_the_xbuf_name, 'lmem_buf0'))
            #define_eval lmem_buf0      *l$index1
        #elif (streq(_the_xbuf_name, 'lmem_buf1'))
            #define_eval lmem_buf1      *l$index1
        #elif (streq(_the_xbuf_name, 'lmem_buf2'))
            #define_eval lmem_buf2      *l$index1
        #elif (streq(_the_xbuf_name, 'lmem_buf3'))
            #define_eval lmem_buf3      *l$index1
        #elif (streq(_the_xbuf_name, 'lmem_buf4'))
            #define_eval lmem_buf4      *l$index1
        #elif (streq(_the_xbuf_name, 'lmem_buf5'))
            #define_eval lmem_buf5      *l$index1
        #elif (streq(_the_xbuf_name, 'lmem_buf6'))
            #define_eval lmem_buf6      *l$index1
        #elif (streq(_the_xbuf_name, 'lmem_buf7'))
            #define_eval lmem_buf7      *l$index1
        #elif (streq(_the_xbuf_name, 'lmem_buf8'))
            #define_eval lmem_buf8      *l$index1
        #elif (streq(_the_xbuf_name, 'lmem_buf9'))
            #define_eval lmem_buf9      *l$index1
        #elif (streq(_the_xbuf_name, 'lmem_buf10'))
            #define_eval lmem_buf10     *l$index1
        #elif (streq(_the_xbuf_name, 'lmem_buf11'))
            #define_eval lmem_buf11     *l$index1
        #elif (streq(_the_xbuf_name, 'lmem_buf12'))
            #define_eval lmem_buf12     *l$index1
        #elif (streq(_the_xbuf_name, 'lmem_buf13'))
            #define_eval lmem_buf13     *l$index1
        #elif (streq(_the_xbuf_name, 'lmem_buf14'))
            #define_eval lmem_buf14     *l$index1
        #elif (streq(_the_xbuf_name, 'lmem_buf15'))
            #define_eval lmem_buf15     *l$index1
        #else
            #error "Invalid str_xbuf_name parameter value."
        #endif
    #else
        #error "Invalid INDEX parameter value."
    #endif
#endm
/// @endcond


/// @cond INTERNAL_MACROS
/** An internal macro that removes the association between the Local Memory buffer name and
 *  *l$index0 or *l$index1.
 *
 * This macro assumes the buffer name exists.
 *
 * @macro_undef Buffer name
 *
 * @param str_xbuf_name   Valid Local Memory buffer name
 *
 * @instruction_cnt 0
 */
#macro _xbuf_unassociate_lmem_regs(str_xbuf_name)
    #define_eval _the_xbuf_name (str_xbuf_name)

    #if (streq(_the_xbuf_name, 'lmem_buf0'))
        #ifdef lmem_buf0
            #undef lmem_buf0
        #endif
    #elif (streq(_the_xbuf_name, 'lmem_buf1'))
        #ifdef lmem_buf1
            #undef lmem_buf1
        #endif
    #elif (streq(_the_xbuf_name, 'lmem_buf2'))
        #ifdef lmem_buf2
            #undef lmem_buf2
        #endif
    #elif (streq(_the_xbuf_name, 'lmem_buf3'))
        #ifdef lmem_buf3
            #undef lmem_buf3
        #endif
    #elif (streq(_the_xbuf_name, 'lmem_buf4'))
        #ifdef lmem_buf4
            #undef lmem_buf4
        #endif
    #elif (streq(_the_xbuf_name, 'lmem_buf5'))
        #ifdef lmem_buf5
            #undef lmem_buf5
        #endif
    #elif (streq(_the_xbuf_name, 'lmem_buf6'))
        #ifdef lmem_buf6
            #undef lmem_buf6
        #endif
    #elif (streq(_the_xbuf_name, 'lmem_buf7'))
        #ifdef lmem_buf7
            #undef lmem_buf7
        #endif
    #elif (streq(_the_xbuf_name, 'lmem_buf8'))
        #ifdef lmem_buf8
            #undef lmem_buf8
        #endif
    #elif (streq(_the_xbuf_name, 'lmem_buf9'))
        #ifdef lmem_buf9
            #undef lmem_buf9
        #endif
    #elif (streq(_the_xbuf_name, 'lmem_buf10'))
        #ifdef lmem_buf10
            #undef lmem_buf10
        #endif
    #elif (streq(_the_xbuf_name, 'lmem_buf11'))
        #ifdef lmem_buf11
            #undef lmem_buf11
        #endif
    #elif (streq(_the_xbuf_name, 'lmem_buf12'))
        #ifdef lmem_buf12
            #undef lmem_buf12
        #endif
    #elif (streq(_the_xbuf_name, 'lmem_buf13'))
        #ifdef lmem_buf13
            #undef lmem_buf13
        #endif
    #elif (streq(_the_xbuf_name, 'lmem_buf14'))
        #ifdef lmem_buf14
            #undef lmem_buf14
        #endif
    #elif (streq(_the_xbuf_name, 'lmem_buf15'))
        #ifdef lmem_buf15
            #undef lmem_buf15
        #endif
    #else
        #error "Invalid str_xbuf_name parameter value."
    #endif
#endm
/// @endcond


/// @cond INTERNAL_MACROS
/** An internal macro that makes sure only 2 buffers are set for
 *  the 2 local memory handles 0 and 1.
 *
 * If lmem_buf3 is set to INDEX 0, _xbuf3_lmem_index is set to 0 and _XBUF_LMEM_HANDLE0
 * is set to 3. Later, if lmem_buf4 is set to INDEX 0, _xbuf3_lmem_index is undefined.
 * Also, the association between lmem_buf3 with *l$index0 is removed.
 *
 * This macro is called by xbuf_activate and is for xbuf_lmem_link macro
 * to keep track that only 2 active buffers can be linked together.
 *
 * @macro_define _XBUF_LMEM_HANDLE0 or _XBUF_LMEM_HANDLE1 is defined to buffer number.
 * @macro_undef _xbuf[i]_lmem_index if it was holding _XBUF_LMEM_HANDLE[0/1].
 * @macro_undef lmem_buf3 (which was associated with *l$index0).
 *
 * @param buf_num  Local Memory buffer number: 1 - 16
 * @param INDEX    Local Memory handle: 0 or 1
 *
 * @instruction_cnt 0
 */
#macro _xbuf_set_lmem_handle_active_buf(buf_num, INDEX)
    #if (INDEX == 0)
        #ifdef _XBUF_LMEM_HANDLE1
        // The buf_num buffer is currently active in index1, and it
        // wants to switch active to index0. This means the global handle1
        // should be undefined.
            #if (_XBUF_LMEM_HANDLE1 == buf_num)
                #undef _XBUF_LMEM_HANDLE1
            #endif
        #endif

        #ifdef _XBUF_LMEM_HANDLE0
        // The global handle0 is currently defined to some buffer. Need
        // to find and undefine that buffer
            #if (_XBUF_LMEM_HANDLE0 == 1)
                #undef _xbuf1_lmem_index
                _xbuf_unassociate_lmem_regs(_xbuf1_lmem_name)
            #elif (_XBUF_LMEM_HANDLE0 == 2)
                #undef _xbuf2_lmem_index
                _xbuf_unassociate_lmem_regs(_xbuf2_lmem_name)
            #elif (_XBUF_LMEM_HANDLE0 == 3)
                #undef _xbuf3_lmem_index
                _xbuf_unassociate_lmem_regs(_xbuf3_lmem_name)
            #elif (_XBUF_LMEM_HANDLE0 == 4)
                #undef _xbuf4_lmem_index
                _xbuf_unassociate_lmem_regs(_xbuf4_lmem_name)
            #elif (_XBUF_LMEM_HANDLE0 == 5)
                #undef _xbuf5_lmem_index
                _xbuf_unassociate_lmem_regs(_xbuf5_lmem_name)
            #elif (_XBUF_LMEM_HANDLE0 == 6)
                #undef _xbuf6_lmem_index
                _xbuf_unassociate_lmem_regs(_xbuf6_lmem_name)
            #elif (_XBUF_LMEM_HANDLE0 == 7)
                #undef _xbuf7_lmem_index
                _xbuf_unassociate_lmem_regs(_xbuf7_lmem_name)
            #elif (_XBUF_LMEM_HANDLE0 == 8)
                #undef _xbuf8_lmem_index
                _xbuf_unassociate_lmem_regs(_xbuf8_lmem_name)
            #elif (_XBUF_LMEM_HANDLE0 == 9)
                #undef _xbuf9_lmem_index
                _xbuf_unassociate_lmem_regs(_xbuf9_lmem_name)
            #elif (_XBUF_LMEM_HANDLE0 == 10)
                #undef _xbuf10_lmem_index
                _xbuf_unassociate_lmem_regs(_xbuf10_lmem_name)
            #elif (_XBUF_LMEM_HANDLE0 == 11)
                #undef _xbuf11_lmem_index
                _xbuf_unassociate_lmem_regs(_xbuf11_lmem_name)
            #elif (_XBUF_LMEM_HANDLE0 == 12)
                #undef _xbuf12_lmem_index
                _xbuf_unassociate_lmem_regs(_xbuf12_lmem_name)
            #elif (_XBUF_LMEM_HANDLE0 == 13)
                #undef _xbuf13_lmem_index
                _xbuf_unassociate_lmem_regs(_xbuf13_lmem_name)
            #elif (_XBUF_LMEM_HANDLE0 == 14)
                #undef _xbuf14_lmem_index
                _xbuf_unassociate_lmem_regs(_xbuf14_lmem_name)
            #elif (_XBUF_LMEM_HANDLE0 == 15)
                #undef _xbuf15_lmem_index
                _xbuf_unassociate_lmem_regs(_xbuf15_lmem_name)
            #elif (_XBUF_LMEM_HANDLE0 == 16)
                #undef _xbuf16_lmem_index
                _xbuf_unassociate_lmem_regs(_xbuf16_lmem_name)
            #else
                #error "_xbuf_set_lmem_handle_active_buf: _XBUF_LMEM_HANDLE0 must be 1-16."
            #endif
        #endif

        // Now define the global handle0 to this buffer
        #define_eval _XBUF_LMEM_HANDLE0     (buf_num)

    #elif (INDEX == 1)

        #ifdef _XBUF_LMEM_HANDLE0
        // The buf_num buffer is currently active in index0, and it
        // wants to switch active to index1. This means the global handle0
        // should be undefined.
            #if (_XBUF_LMEM_HANDLE0 == buf_num)
                #undef _XBUF_LMEM_HANDLE0
            #endif
        #endif

        #ifdef _XBUF_LMEM_HANDLE1
        // The global handle1 is currently defined to some buffer. Need
        // to find and undefine that buffer
            #if (_XBUF_LMEM_HANDLE1 == 1)
                #undef _xbuf1_lmem_index
                _xbuf_unassociate_lmem_regs(_xbuf1_lmem_name)
            #elif (_XBUF_LMEM_HANDLE1 == 2)
                #undef _xbuf2_lmem_index
                _xbuf_unassociate_lmem_regs(_xbuf2_lmem_name)
            #elif (_XBUF_LMEM_HANDLE1 == 3)
                #undef _xbuf3_lmem_index
                _xbuf_unassociate_lmem_regs(_xbuf3_lmem_name)
            #elif (_XBUF_LMEM_HANDLE1 == 4)
                #undef _xbuf4_lmem_index
                _xbuf_unassociate_lmem_regs(_xbuf4_lmem_name)
            #elif (_XBUF_LMEM_HANDLE1 == 5)
                #undef _xbuf5_lmem_index
                _xbuf_unassociate_lmem_regs(_xbuf5_lmem_name)
            #elif (_XBUF_LMEM_HANDLE1 == 6)
                #undef _xbuf6_lmem_index
                _xbuf_unassociate_lmem_regs(_xbuf6_lmem_name)
            #elif (_XBUF_LMEM_HANDLE1 == 7)
                #undef _xbuf7_lmem_index
                _xbuf_unassociate_lmem_regs(_xbuf7_lmem_name)
            #elif (_XBUF_LMEM_HANDLE1 == 8)
                #undef _xbuf8_lmem_index
                _xbuf_unassociate_lmem_regs(_xbuf8_lmem_name)
            #elif (_XBUF_LMEM_HANDLE1 == 9)
                #undef _xbuf9_lmem_index
                _xbuf_unassociate_lmem_regs(_xbuf9_lmem_name)
            #elif (_XBUF_LMEM_HANDLE1 == 10)
                #undef _xbuf10_lmem_index
                _xbuf_unassociate_lmem_regs(_xbuf10_lmem_name)
            #elif (_XBUF_LMEM_HANDLE1 == 11)
                #undef _xbuf11_lmem_index
                _xbuf_unassociate_lmem_regs(_xbuf11_lmem_name)
            #elif (_XBUF_LMEM_HANDLE1 == 12)
                #undef _xbuf12_lmem_index
                _xbuf_unassociate_lmem_regs(_xbuf12_lmem_name)
            #elif (_XBUF_LMEM_HANDLE1 == 13)
                #undef _xbuf13_lmem_index
                _xbuf_unassociate_lmem_regs(_xbuf13_lmem_name)
            #elif (_XBUF_LMEM_HANDLE1 == 14)
                #undef _xbuf14_lmem_index
                _xbuf_unassociate_lmem_regs(_xbuf14_lmem_name)
            #elif (_XBUF_LMEM_HANDLE1 == 15)
                #undef _xbuf15_lmem_index
                _xbuf_unassociate_lmem_regs(_xbuf15_lmem_name)
            #elif (_XBUF_LMEM_HANDLE1 == 16)
                #undef _xbuf16_lmem_index
                _xbuf_unassociate_lmem_regs(_xbuf16_lmem_name)
            #else
                #error "_xbuf_set_lmem_handle_active_buf: _XBUF_LMEM_HANDLE1 must be 1-16."
            #endif
        #endif

        // Now define the global handle1 to this buffer
        #define_eval _XBUF_LMEM_HANDLE1     (buf_num)
    #else
        #error "Invalid INDEX parameter value."
    #endif
#endm
/// @endcond


/** This macro is used to bind a buffer name with an address in a pool of buffers inside
 * local memory.
 *
 * Common designs have local memory divided into several blocks of pools. In each block,
 * the pools are contiguous, and the number of pools is equal to the number of active
 * threads so that each thread has one pool. Inside a pool, there can be several buffers.
 *
 * This call is applicable to LMEM buffers only. It is transparent to other types.
 *
 * @macro_define Set internal constants.
 * @macro_define _xbuf[i]_lmem_pools_base
 * @macro_define _xbuf[i]_lmem_pool_size
 * @macro_define _xbuf[i]_lmem_offset (where i = 0..15 since 16 buffers are supported)
 *
 * @param str_xbuf_name Name of buffer. Name has to start with "lmem". For example,
 *                      lmem_buf1. If name does not start with "lmem", the macro will
 *                      assume that buffer is not in local memory and nothing will be done.
 * @param POOLS_BASE    Base of all pools in bytes (must be multiple of @p POOL_SIZE)
 * @param POOL_SIZE     Size of each pool in bytes (must be power of 2)
 * @param BUFFER_OFFSET Offset in bytes of the buffer inside its pool.
 *                      The offset should be aligned on a long-word boundary which is
 *                      calculated as the next higher power of 2 of the buffer-size.
 *
 * <b>Example:</b>
 * @code
 *      xbuf_alloc(lmem_buf0, 8, READ_WRITE)
 *      xbuf_bind_address(lmem_buf0, 0x100, 0x20, 0x0)
 *      xbuf_activate(lmem_buf0, 0, 0, 1)
 *
 *      Start using lmem_buf0....
 *      ipv4_cksum_verify(lmem_buf0, 0xb)
 * @endcode
 *
 * @instruction_cnt 0
 *
 * @note lmem_buf0, lmem_buf1, ..., lmem_buf15 must be used for local memory buffers.
 */
#macro xbuf_bind_address(str_xbuf_name, POOLS_BASE, POOL_SIZE, BUFFER_OFFSET)
    #if ( streq(str_xbuf_name, 'lmem_buf0') || \
            streq(str_xbuf_name, 'lmem_buf1') || \
            streq(str_xbuf_name, 'lmem_buf2') || \
            streq(str_xbuf_name, 'lmem_buf3') || \
            streq(str_xbuf_name, 'lmem_buf4') || \
            streq(str_xbuf_name, 'lmem_buf5') || \
            streq(str_xbuf_name, 'lmem_buf6') || \
            streq(str_xbuf_name, 'lmem_buf7') || \
            streq(str_xbuf_name, 'lmem_buf8') || \
            streq(str_xbuf_name, 'lmem_buf9') || \
            streq(str_xbuf_name, 'lmem_buf10') || \
            streq(str_xbuf_name, 'lmem_buf11') || \
            streq(str_xbuf_name, 'lmem_buf12') || \
            streq(str_xbuf_name, 'lmem_buf13') || \
            streq(str_xbuf_name, 'lmem_buf14') || \
            streq(str_xbuf_name, 'lmem_buf15') )

        #ifdef _num_regs
            #warning "_num_regs is being redefined."
        #endif

        #if (POOL_SIZE <= 0)
            #error "Invalid value for POOL_SIZE."
        #endif

        #define_eval _the_xbuf_name (str_xbuf_name)

        #if (streq(_the_xbuf_name, _xbuf1_lmem_name))
            #define_eval _xbuf1_lmem_pools_base (POOLS_BASE)
            #define_eval _xbuf1_lmem_pool_size  (POOL_SIZE)
            #define_eval _xbuf1_lmem_offset     (BUFFER_OFFSET)
            #define_eval _num_regs             _xbuf1_lmem_size
        #elif (streq(_the_xbuf_name, _xbuf2_lmem_name))
            #define_eval _xbuf2_lmem_pools_base (POOLS_BASE)
            #define_eval _xbuf2_lmem_pool_size  (POOL_SIZE)
            #define_eval _xbuf2_lmem_offset     (BUFFER_OFFSET)
            #define_eval _num_regs             _xbuf2_lmem_size
        #elif (streq(_the_xbuf_name, _xbuf3_lmem_name))
            #define_eval _xbuf3_lmem_pools_base (POOLS_BASE)
            #define_eval _xbuf3_lmem_pool_size  (POOL_SIZE)
            #define_eval _xbuf3_lmem_offset     (BUFFER_OFFSET)
            #define_eval _num_regs             _xbuf3_lmem_size
        #elif (streq(_the_xbuf_name, _xbuf4_lmem_name))
            #define_eval _xbuf4_lmem_pools_base (POOLS_BASE)
            #define_eval _xbuf4_lmem_pool_size  (POOL_SIZE)
            #define_eval _xbuf4_lmem_offset     (BUFFER_OFFSET)
            #define_eval _num_regs             _xbuf4_lmem_size
        #elif (streq(_the_xbuf_name, _xbuf5_lmem_name))
            #define_eval _xbuf5_lmem_pools_base (POOLS_BASE)
            #define_eval _xbuf5_lmem_pool_size  (POOL_SIZE)
            #define_eval _xbuf5_lmem_offset     (BUFFER_OFFSET)
            #define_eval _num_regs             _xbuf5_lmem_size
        #elif (streq(_the_xbuf_name, _xbuf6_lmem_name))
            #define_eval _xbuf6_lmem_pools_base (POOLS_BASE)
            #define_eval _xbuf6_lmem_pool_size  (POOL_SIZE)
            #define_eval _xbuf6_lmem_offset     (BUFFER_OFFSET)
            #define_eval _num_regs             _xbuf6_lmem_size
        #elif (streq(_the_xbuf_name, _xbuf7_lmem_name))
            #define_eval _xbuf7_lmem_pools_base (POOLS_BASE)
            #define_eval _xbuf7_lmem_pool_size  (POOL_SIZE)
            #define_eval _xbuf7_lmem_offset     (BUFFER_OFFSET)
            #define_eval _num_regs             _xbuf7_lmem_size
        #elif (streq(_the_xbuf_name, _xbuf8_lmem_name))
            #define_eval _xbuf8_lmem_pools_base (POOLS_BASE)
            #define_eval _xbuf8_lmem_pool_size  (POOL_SIZE)
            #define_eval _xbuf8_lmem_offset     (BUFFER_OFFSET)
            #define_eval _num_regs             _xbuf8_lmem_size
        #elif (streq(_the_xbuf_name, _xbuf9_lmem_name))
            #define_eval _xbuf9_lmem_pools_base (POOLS_BASE)
            #define_eval _xbuf9_lmem_pool_size  (POOL_SIZE)
            #define_eval _xbuf9_lmem_offset     (BUFFER_OFFSET)
            #define_eval _num_regs             _xbuf9_lmem_size
        #elif (streq(_the_xbuf_name, _xbuf10_lmem_name))
            #define_eval _xbuf10_lmem_pools_base    (POOLS_BASE)
            #define_eval _xbuf10_lmem_pool_size     (POOL_SIZE)
            #define_eval _xbuf10_lmem_offset        (BUFFER_OFFSET)
            #define_eval _num_regs                 _xbuf10_lmem_size
        #elif (streq(_the_xbuf_name, _xbuf11_lmem_name))
            #define_eval _xbuf11_lmem_pools_base    (POOLS_BASE)
            #define_eval _xbuf11_lmem_pool_size     (POOL_SIZE)
            #define_eval _xbuf11_lmem_offset        (BUFFER_OFFSET)
            #define_eval _num_regs                 _xbuf11_lmem_size
        #elif (streq(_the_xbuf_name, _xbuf12_lmem_name))
            #define_eval _xbuf12_lmem_pools_base    (POOLS_BASE)
            #define_eval _xbuf12_lmem_pool_size     (POOL_SIZE)
            #define_eval _xbuf12_lmem_offset        (BUFFER_OFFSET)
            #define_eval _num_regs                 _xbuf12_lmem_size
        #elif (streq(_the_xbuf_name, _xbuf13_lmem_name))
            #define_eval _xbuf13_lmem_pools_base    (POOLS_BASE)
            #define_eval _xbuf13_lmem_pool_size     (POOL_SIZE)
            #define_eval _xbuf13_lmem_offset        (BUFFER_OFFSET)
            #define_eval _num_regs                 _xbuf13_lmem_size
        #elif (streq(_the_xbuf_name, _xbuf14_lmem_name))
            #define_eval _xbuf14_lmem_pools_base    (POOLS_BASE)
            #define_eval _xbuf14_lmem_pool_size     (POOL_SIZE)
            #define_eval _xbuf14_lmem_offset        (BUFFER_OFFSET)
            #define_eval _num_regs                 _xbuf14_lmem_size
        #elif (streq(_the_xbuf_name, _xbuf15_lmem_name))
            #define_eval _xbuf15_lmem_pools_base    (POOLS_BASE)
            #define_eval _xbuf15_lmem_pool_size     (POOL_SIZE)
            #define_eval _xbuf15_lmem_offset        (BUFFER_OFFSET)
            #define_eval _num_regs                 _xbuf15_lmem_size
        #elif (streq(_the_xbuf_name, _xbuf16_lmem_name))
            #define_eval _xbuf16_lmem_pools_base    (POOLS_BASE)
            #define_eval _xbuf16_lmem_pool_size     (POOL_SIZE)
            #define_eval _xbuf16_lmem_offset        (BUFFER_OFFSET)
            #define_eval _num_regs                 _xbuf16_lmem_size
        #else
            #error "xbuf_bind_address: Invalid str_xbuf_name."
        #endif

        // check alignment for POOLS_BASE
        #if (POOLS_BASE % POOL_SIZE)
            #error "xbuf_bind_address: POOLS_BASE must be multiple of POOL_SIZE."
        #endif

        // check alignment for BUFFER_OFFSET
        #if (_num_regs == 2)
            #if (BUFFER_OFFSET % 8)
                #error "xbuf_bind_address: BUFFER_OFFSET must be buffer size aligned."
            #endif
        #elif (_num_regs == 3)
            #if (BUFFER_OFFSET % 16)
                #error "xbuf_bind_address: BUFFER_OFFSET must be buffer size aligned."
            #endif
        #elif (_num_regs == 4)
            #if (BUFFER_OFFSET % 16)
                #error "xbuf_bind_address: BUFFER_OFFSET must be buffer size aligned."
            #endif
        #elif (_num_regs == 5)
            #if (BUFFER_OFFSET % 32)
                #error "xbuf_bind_address: BUFFER_OFFSET must be buffer size aligned."
            #endif
        #elif (_num_regs == 6)
            #if (BUFFER_OFFSET % 32)
                #error "xbuf_bind_address: BUFFER_OFFSET must be buffer size aligned."
            #endif
        #elif (_num_regs == 7)
            #if (BUFFER_OFFSET % 32)
                #error "xbuf_bind_address: BUFFER_OFFSET must be buffer size aligned."
            #endif
        #elif (_num_regs == 8)
            #if (BUFFER_OFFSET % 32)
                #error "xbuf_bind_address: BUFFER_OFFSET must be buffer size aligned."
            #endif
        #elif (_num_regs == 9)
            #if (BUFFER_OFFSET % 64)
                #error "xbuf_bind_address: BUFFER_OFFSET must be buffer size aligned."
            #endif
        #elif (_num_regs == 10)
            #if (BUFFER_OFFSET % 64)
                #error "xbuf_bind_address: BUFFER_OFFSET must be buffer size aligned."
            #endif
        #elif (_num_regs == 11)
            #if (BUFFER_OFFSET % 64)
                #error "xbuf_bind_address: BUFFER_OFFSET must be buffer size aligned."
            #endif
        #elif (_num_regs == 12)
            #if (BUFFER_OFFSET % 64)
                #error "xbuf_bind_address: BUFFER_OFFSET must be buffer size aligned."
            #endif
        #elif (_num_regs == 13)
            #if (BUFFER_OFFSET % 64
                #error "xbuf_bind_address: BUFFER_OFFSET must be buffer size aligned."
            #endif
        #elif (_num_regs == 14)
            #if (BUFFER_OFFSET % 64)
                #error "xbuf_bind_address: BUFFER_OFFSET must be buffer size aligned."
            #endif
        #elif (_num_regs == 15)
            #if (BUFFER_OFFSET % 64)
                #error "xbuf_bind_address: BUFFER_OFFSET must be buffer size aligned."
            #endif
        #elif (_num_regs == 16)
            #if (BUFFER_OFFSET % 64)
                #error "xbuf_bind_address: BUFFER_OFFSET must be buffer size aligned."
            #endif
        #endif

        #undef _the_xbuf_name
        #undef _num_regs

    #else
        #error "This macro only supports LMEM buffers."
    #endif
#endm


/** Undefine the binding (set by @p xbuf_activate) between the buffer name and its
 *  current Local Memory handle.
 *
 * This macro is called at the end of a block that defines
 * the scope of the binding. Users have to call @p xbuf_activate again to associate
 * the buffer name with LMEM handle before the buffer can be used.
 *
 * This call is applicable to Local Memory buffers only. It is transparent to other types.
 *
 * @macro_define @p str_xbuf_name is no longer associated with any Local Memory handle.
 *
 * @param str_xbuf_name Name of buffer. Name has to start with "lmem". For example,
 *                      lmem_buf1. If name does not start with "lmem", the macro will
 *                      assume that buffer is not in local memory and nothing will be done.
 *
 * @note lmem_buf0, lmem_buf1, ..., lmem_buf15 must be used for local memory buffers.
 *
 * <b> Example 1:</b>
 * @code
 * #define_eval ipv4_hdr   lmem_buf0
 * xbuf_alloc(ipv4_hdr, 8, read_write)
 * xbuf_bind_address(ipv4_hdr, 0x280, 0x20, 0)
 * xbuf_activate(ipv4_hdr, 0, 3, 1)
 * // uses of ipv4_hdr
 * xbuf_deactivate(ipv4_hdr)
 * xbuf_activate(ipv4_hdr, 1, 3, 1)
 * // uses of ipv4_hdr
 * xbuf_deactivate(ipv4_hdr)
 * xbuf_free(ipv4_hdr)
 * @endcode
 *
 * <b>Example 2:</b>
 * @code
 * #define_eval ipv4_hdr   lmem_buf1
 * xbuf_alloc(ipv4_hdr, 8, read_write)
 * xbuf_bind_address(ipv4_hdr, pools_base, pool_size, buf_offset)
 * #macro first_block(name)
 *     xbuf_activate(name, 1, 2, 1)    // select handle1
 *     xbuf_extract(..., name, ...)
 *     xbuf_deactivate(name)
 * #endm
 * #macro second_block(name)
 *     xbuf_activate(name, 0, 2, 1)    // switch to handle0
 *     xbuf_extract(..., name, ...)
 *     xbuf_deactivate(name)
 * #endm
 * first_block(ipv4_hdr)
 * second_block(ipv4_hdr)
 * @endcode
 *
 * @instruction_cnt 0
 */
#macro xbuf_deactivate(str_xbuf_name)
    #if ( streq(str_xbuf_name, 'lmem_buf0') || \
            streq(str_xbuf_name, 'lmem_buf1') || \
            streq(str_xbuf_name, 'lmem_buf2') || \
            streq(str_xbuf_name, 'lmem_buf3') || \
            streq(str_xbuf_name, 'lmem_buf4') || \
            streq(str_xbuf_name, 'lmem_buf5') || \
            streq(str_xbuf_name, 'lmem_buf6') || \
            streq(str_xbuf_name, 'lmem_buf7') || \
            streq(str_xbuf_name, 'lmem_buf8') || \
            streq(str_xbuf_name, 'lmem_buf9') || \
            streq(str_xbuf_name, 'lmem_buf10') || \
            streq(str_xbuf_name, 'lmem_buf11') || \
            streq(str_xbuf_name, 'lmem_buf12') || \
            streq(str_xbuf_name, 'lmem_buf13') || \
            streq(str_xbuf_name, 'lmem_buf14') || \
            streq(str_xbuf_name, 'lmem_buf15') )

        #define_eval _this_xbuf_name    (str_xbuf_name)

        // Undef _XBUF_LMEM_HANDLE<i> that this buffer is holding
        #if (streq(_this_xbuf_name, _xbuf1_lmem_name))
            #ifdef _XBUF_LMEM_HANDLE0
                #if (_XBUF_LMEM_HANDLE0 == 1)
                    #undef _XBUF_LMEM_HANDLE0
                #endif
            #endif
            #ifdef _XBUF_LMEM_HANDLE1
                #if (_XBUF_LMEM_HANDLE1 == 1)
                    #undef _XBUF_LMEM_HANDLE1
                #endif
            #endif
        #elif (streq(_this_xbuf_name, _xbuf2_lmem_name))
            #ifdef _XBUF_LMEM_HANDLE0
                #if (_XBUF_LMEM_HANDLE0 == 2)
                    #undef _XBUF_LMEM_HANDLE0
                #endif
            #endif
            #ifdef _XBUF_LMEM_HANDLE1
                #if (_XBUF_LMEM_HANDLE1 == 2)
                    #undef _XBUF_LMEM_HANDLE1
                #endif
            #endif
        #elif (streq(_this_xbuf_name, _xbuf3_lmem_name))
            #ifdef _XBUF_LMEM_HANDLE0
                #if (_XBUF_LMEM_HANDLE0 == 3)
                    #undef _XBUF_LMEM_HANDLE0
                #endif
            #endif
            #ifdef _XBUF_LMEM_HANDLE1
                #if (_XBUF_LMEM_HANDLE1 == 3)
                    #undef _XBUF_LMEM_HANDLE1
                #endif
            #endif
        #elif (streq(_this_xbuf_name, _xbuf4_lmem_name))
            #ifdef _XBUF_LMEM_HANDLE0
                #if (_XBUF_LMEM_HANDLE0 == 4)
                    #undef _XBUF_LMEM_HANDLE0
                #endif
            #endif
            #ifdef _XBUF_LMEM_HANDLE1
                #if (_XBUF_LMEM_HANDLE1 == 4)
                    #undef _XBUF_LMEM_HANDLE1
                #endif
            #endif
        #elif (streq(_this_xbuf_name, _xbuf5_lmem_name))
            #ifdef _XBUF_LMEM_HANDLE0
                #if (_XBUF_LMEM_HANDLE0 == 5)
                    #undef _XBUF_LMEM_HANDLE0
                #endif
            #endif
            #ifdef _XBUF_LMEM_HANDLE1
                #if (_XBUF_LMEM_HANDLE1 == 5)
                    #undef _XBUF_LMEM_HANDLE1
                #endif
            #endif
        #elif (streq(_this_xbuf_name, _xbuf6_lmem_name))
            #ifdef _XBUF_LMEM_HANDLE0
                #if (_XBUF_LMEM_HANDLE0 == 6)
                    #undef _XBUF_LMEM_HANDLE0
                #endif
            #endif
            #ifdef _XBUF_LMEM_HANDLE1
                #if (_XBUF_LMEM_HANDLE1 == 6)
                    #undef _XBUF_LMEM_HANDLE1
                #endif
            #endif
        #elif (streq(_this_xbuf_name, _xbuf7_lmem_name))
            #ifdef _XBUF_LMEM_HANDLE0
                #if (_XBUF_LMEM_HANDLE0 == 7)
                    #undef _XBUF_LMEM_HANDLE0
                #endif
            #endif
            #ifdef _XBUF_LMEM_HANDLE1
                #if (_XBUF_LMEM_HANDLE1 == 7)
                    #undef _XBUF_LMEM_HANDLE1
                #endif
            #endif
        #elif (streq(_this_xbuf_name, _xbuf8_lmem_name))
            #ifdef _XBUF_LMEM_HANDLE0
                #if (_XBUF_LMEM_HANDLE0 == 8)
                    #undef _XBUF_LMEM_HANDLE0
                #endif
            #endif
            #ifdef _XBUF_LMEM_HANDLE1
                #if (_XBUF_LMEM_HANDLE1 == 8)
                    #undef _XBUF_LMEM_HANDLE1
                #endif
            #endif
        #elif (streq(_this_xbuf_name, _xbuf9_lmem_name))
            #ifdef _XBUF_LMEM_HANDLE0
                #if (_XBUF_LMEM_HANDLE0 == 9)
                    #undef _XBUF_LMEM_HANDLE0
                #endif
            #endif
            #ifdef _XBUF_LMEM_HANDLE1
                #if (_XBUF_LMEM_HANDLE1 == 9)
                    #undef _XBUF_LMEM_HANDLE1
                #endif
            #endif
        #elif (streq(_this_xbuf_name, _xbuf10_lmem_name))
            #ifdef _XBUF_LMEM_HANDLE0
                #if (_XBUF_LMEM_HANDLE0 == 10)
                    #undef _XBUF_LMEM_HANDLE0
                #endif
            #endif
            #ifdef _XBUF_LMEM_HANDLE1
                #if (_XBUF_LMEM_HANDLE1 == 10)
                    #undef _XBUF_LMEM_HANDLE1
                #endif
            #endif
        #elif (streq(_this_xbuf_name, _xbuf11_lmem_name))
            #ifdef _XBUF_LMEM_HANDLE0
                #if (_XBUF_LMEM_HANDLE0 == 11)
                    #undef _XBUF_LMEM_HANDLE0
                #endif
            #endif
            #ifdef _XBUF_LMEM_HANDLE1
                #if (_XBUF_LMEM_HANDLE1 == 11)
                    #undef _XBUF_LMEM_HANDLE1
                #endif
            #endif
        #elif (streq(_this_xbuf_name, _xbuf12_lmem_name))
            #ifdef _XBUF_LMEM_HANDLE0
                #if (_XBUF_LMEM_HANDLE0 == 12)
                    #undef _XBUF_LMEM_HANDLE0
                #endif
            #endif
            #ifdef _XBUF_LMEM_HANDLE1
                #if (_XBUF_LMEM_HANDLE1 == 12)
                    #undef _XBUF_LMEM_HANDLE1
                #endif
            #endif
        #elif (streq(_this_xbuf_name, _xbuf13_lmem_name))
            #ifdef _XBUF_LMEM_HANDLE0
                #if (_XBUF_LMEM_HANDLE0 == 13)
                    #undef _XBUF_LMEM_HANDLE0
                #endif
            #endif
            #ifdef _XBUF_LMEM_HANDLE1
                #if (_XBUF_LMEM_HANDLE1 == 13)
                    #undef _XBUF_LMEM_HANDLE1
                #endif
            #endif
        #elif (streq(_this_xbuf_name, _xbuf14_lmem_name))
            #ifdef _XBUF_LMEM_HANDLE0
                #if (_XBUF_LMEM_HANDLE0 == 14)
                    #undef _XBUF_LMEM_HANDLE0
                #endif
            #endif
            #ifdef _XBUF_LMEM_HANDLE1
                #if (_XBUF_LMEM_HANDLE1 == 14)
                    #undef _XBUF_LMEM_HANDLE1
                #endif
            #endif
        #elif (streq(_this_xbuf_name, _xbuf15_lmem_name))
            #ifdef _XBUF_LMEM_HANDLE0
                #if (_XBUF_LMEM_HANDLE0 == 15)
                    #undef _XBUF_LMEM_HANDLE0
                #endif
            #endif
            #ifdef _XBUF_LMEM_HANDLE1
                #if (_XBUF_LMEM_HANDLE1 == 15)
                    #undef _XBUF_LMEM_HANDLE1
                #endif
            #endif
        #elif (streq(_this_xbuf_name, _xbuf16_lmem_name))
            #ifdef _XBUF_LMEM_HANDLE0
                #if (_XBUF_LMEM_HANDLE0 == 16)
                    #undef _XBUF_LMEM_HANDLE0
                #endif
            #endif
            #ifdef _XBUF_LMEM_HANDLE1
                #if (_XBUF_LMEM_HANDLE1 == 16)
                    #undef _XBUF_LMEM_HANDLE1
                #endif
            #endif
        #else
            #error 2 "_xbuf_deactivate: Invalid str_xbuf_name."
        #endif

        // Undef binding between lmem_buf<x> and *l$index<i>
        _xbuf_unassociate_lmem_regs(str_xbuf_name)
    #else
        #error "This macro only supports LMEM buffers."
    #endif
#endm


/** To map the specified ACTIVE_LM_ADDRESS CSR to the specified buffer.
 *
 * The absolute address of the buffer will be put in the
 * ACTIVE_LM_ADDR local CSR. The absolute address is calculated from the
 * parameters supplied from the @p xbuf_bind_address call. After this macro
 * is called, the specified buffer can be accessed via *l$index0/1.
 *
 * This call is applicable to LMEM buffers only. It is transparent to other
 * buffer types.
 *
 * @macro_define Set ACTIVE_LM_ADDR_0 or ACTIVE_LM_ADDR_1
 * @macro_define Set _xbuf[i]_lmem_index = INDEX
 *
 * @param str_xbuf_name Name of buffer. Name has to start with "lmem". For example,
 *                      lmem_buf1. If name does not start with "lmem", the macro will
 *                      assume that buffer is not in local memory and nothing will be done.
 *
 * @param INDEX         @arg 0   to map *l$index0 to the specified buffer
 *                      @arg 1   to map *l$index1 to the specified buffer
 *
 * @param thread_id      Run-time value thread ID. Can be either a GPR or a constant.
 *
 * @param WAIT_ACTION   @arg 1   wait 3 cycles after setting up local CSR (3 NOPs)
 *                      @arg 0   do not wait (0 NOP)
 *
 * @note lmem_buf0, lmem_buf1, ..., lmem_buf15 must be used for Local Memory buffers.
 *
 * @note Applications that run with 4 context mode have to explicitly define it:
 *       @code
 *       #define CONTEXT_MODE_4
 *       @endcode
 *       It is 8 context mode by default.
 *
 * @note Buffer absolute address = (thread_id << [log(2) of (_xbuf[i]_lmem_pool_size)]) +
 *       _xbuf[i]_lmem_pools_base + _xbuf[i]_lmem_offset, @n
 *       where _xbuf[i]_lmem is the buffer with the specified name.
 *
 * <b> Example:</b>
 * @code
 * #define_eval ipv6_hdr   lmem_buf0
 *
 * xbuf_alloc(ipv6_hdr, 8, read_write)
 * xbuf_bind_address(ipv6_hdr, pools_base, pool_size, buf_offset)
 * xbuf_activate(ipv6_hdr, 0, 3, 1)
 *
 * Now for thread 3, ipv6_hdr[0], ipv6_hdr[1], ... ipv6_hdr[7]
 * are associated with *l$index0[0], *l$index0[1], ... *l$index0[7]
 * @endcode
 *
 * @instruction_cnt 3 - 4 Without wait cycle
 * @instruction_cnt 6 - 7 With 3 wait cycles
 */
#macro xbuf_activate(str_xbuf_name, INDEX, thread_id, WAIT_ACTION)
.begin

    .reg _xbuf_act_buf_addr

    #if ( (INDEX < 0) || (INDEX > 1) )
        #error "xbuf_activate: INDEX must be either 0 or 1."
    #endif

    #if ( (WAIT_ACTION < 0) || (WAIT_ACTION > 1) )
        #error "xbuf_activate: WAIT_ACTION must be either 0 or 1."
    #endif

    #if ( streq(str_xbuf_name, 'lmem_buf0') || \
            streq(str_xbuf_name, 'lmem_buf1') || \
            streq(str_xbuf_name, 'lmem_buf2') || \
            streq(str_xbuf_name, 'lmem_buf3') || \
            streq(str_xbuf_name, 'lmem_buf4') || \
            streq(str_xbuf_name, 'lmem_buf5') || \
            streq(str_xbuf_name, 'lmem_buf6') || \
            streq(str_xbuf_name, 'lmem_buf7') || \
            streq(str_xbuf_name, 'lmem_buf8') || \
            streq(str_xbuf_name, 'lmem_buf9') || \
            streq(str_xbuf_name, 'lmem_buf10') || \
            streq(str_xbuf_name, 'lmem_buf11') || \
            streq(str_xbuf_name, 'lmem_buf12') || \
            streq(str_xbuf_name, 'lmem_buf13') || \
            streq(str_xbuf_name, 'lmem_buf14') || \
            streq(str_xbuf_name, 'lmem_buf15') )
        #ifdef _XBUF_BUF_ADDR
            #warning "_XBUF_BUF_ADDR is being redefined."
        #endif
        #define_eval _the_xbuf_name     (str_xbuf_name)

        #ifdef XBUF_ACTIVATE_OPTIMIZED
            #undef XBUF_ACTIVATE_OPTIMIZED
        #endif

        // 1. associate buffer name with localmem pointer *l$index/**/INDEX
        // 2. make sure only 2 buffers are active at a time: one is with *l$index0,
        //      the other is with *l$index1
        // 3. find the shift amount to calculate buffer absolute address
        #if (streq(_the_xbuf_name, _xbuf1_lmem_name))
            _xbuf_set_lmem_handle_active_buf(1, INDEX)
            _xbuf_associate_lmem_regs(_xbuf1_lmem_name, INDEX)
            _xbuf_find_log_base_2(_xbuf1_lmem_pool_size)
            #define_eval _XBUF_BUF_ADDR     (_xbuf1_lmem_pools_base + _xbuf1_lmem_offset)
            #define_eval _xbuf1_lmem_index  (INDEX)
            #if ( (_xbuf1_lmem_pools_base == 0) && (_xbuf1_lmem_offset < 256) )
                #define XBUF_ACTIVATE_OPTIMIZED 1
            #endif
        #elif (streq(_the_xbuf_name, _xbuf2_lmem_name))
            _xbuf_set_lmem_handle_active_buf(2, INDEX)
            _xbuf_associate_lmem_regs(_xbuf2_lmem_name, INDEX)
            _xbuf_find_log_base_2(_xbuf2_lmem_pool_size)
            #define_eval _XBUF_BUF_ADDR     (_xbuf2_lmem_pools_base + _xbuf2_lmem_offset)
            #define_eval _xbuf2_lmem_index  (INDEX)
            #if ( (_xbuf2_lmem_pools_base == 0) && (_xbuf2_lmem_offset < 256) )
                #define XBUF_ACTIVATE_OPTIMIZED 1
            #endif
        #elif (streq(_the_xbuf_name, _xbuf3_lmem_name))
            _xbuf_set_lmem_handle_active_buf(3, INDEX)
            _xbuf_associate_lmem_regs(_xbuf3_lmem_name, INDEX)
            _xbuf_find_log_base_2(_xbuf3_lmem_pool_size)
            #define_eval _XBUF_BUF_ADDR     (_xbuf3_lmem_pools_base + _xbuf3_lmem_offset)
            #define_eval _xbuf3_lmem_index  (INDEX)
            #if ( (_xbuf3_lmem_pools_base == 0) && (_xbuf3_lmem_offset < 256) )
                #define XBUF_ACTIVATE_OPTIMIZED 1
            #endif
        #elif (streq(_the_xbuf_name, _xbuf4_lmem_name))
            _xbuf_set_lmem_handle_active_buf(4, INDEX)
            _xbuf_associate_lmem_regs(_xbuf4_lmem_name, INDEX)
            _xbuf_find_log_base_2(_xbuf4_lmem_pool_size)
            #define_eval _XBUF_BUF_ADDR     (_xbuf4_lmem_pools_base + _xbuf4_lmem_offset)
            #define_eval _xbuf4_lmem_index  (INDEX)
            #if ( (_xbuf4_lmem_pools_base == 0) && (_xbuf4_lmem_offset < 256) )
                #define XBUF_ACTIVATE_OPTIMIZED 1
            #endif
        #elif (streq(_the_xbuf_name, _xbuf5_lmem_name))
            _xbuf_set_lmem_handle_active_buf(5, INDEX)
            _xbuf_associate_lmem_regs(_xbuf5_lmem_name, INDEX)
            _xbuf_find_log_base_2(_xbuf5_lmem_pool_size)
            #define_eval _XBUF_BUF_ADDR     (_xbuf5_lmem_pools_base + _xbuf5_lmem_offset)
            #define_eval _xbuf5_lmem_index  (INDEX)
            #if ( (_xbuf5_lmem_pools_base == 0) && (_xbuf5_lmem_offset < 256) )
                #define XBUF_ACTIVATE_OPTIMIZED 1
            #endif
        #elif (streq(_the_xbuf_name, _xbuf6_lmem_name))
            _xbuf_set_lmem_handle_active_buf(6, INDEX)
            _xbuf_associate_lmem_regs(_xbuf6_lmem_name, INDEX)
            _xbuf_find_log_base_2(_xbuf6_lmem_pool_size)
            #define_eval _XBUF_BUF_ADDR     (_xbuf6_lmem_pools_base + _xbuf6_lmem_offset)
            #define_eval _xbuf6_lmem_index  (INDEX)
            #if ( (_xbuf6_lmem_pools_base == 0) && (_xbuf6_lmem_offset < 256) )
                #define XBUF_ACTIVATE_OPTIMIZED 1
            #endif
        #elif (streq(_the_xbuf_name, _xbuf7_lmem_name))
            _xbuf_set_lmem_handle_active_buf(7, INDEX)
            _xbuf_associate_lmem_regs(_xbuf7_lmem_name, INDEX)
            _xbuf_find_log_base_2(_xbuf7_lmem_pool_size)
            #define_eval _XBUF_BUF_ADDR     (_xbuf7_lmem_pools_base + _xbuf7_lmem_offset)
            #define_eval _xbuf7_lmem_index  (INDEX)
            #if ( (_xbuf7_lmem_pools_base == 0) && (_xbuf7_lmem_offset < 256) )
                #define XBUF_ACTIVATE_OPTIMIZED 1
            #endif
        #elif (streq(_the_xbuf_name, _xbuf8_lmem_name))
            _xbuf_set_lmem_handle_active_buf(8, INDEX)
            _xbuf_associate_lmem_regs(_xbuf8_lmem_name, INDEX)
            _xbuf_find_log_base_2(_xbuf8_lmem_pool_size)
            #define_eval _XBUF_BUF_ADDR     (_xbuf8_lmem_pools_base + _xbuf8_lmem_offset)
            #define_eval _xbuf8_lmem_index  (INDEX)
            #if ( (_xbuf8_lmem_pools_base == 0) && (_xbuf8_lmem_offset < 256) )
                #define XBUF_ACTIVATE_OPTIMIZED 1
            #endif
        #elif (streq(_the_xbuf_name, _xbuf9_lmem_name))
            _xbuf_set_lmem_handle_active_buf(9, INDEX)
            _xbuf_associate_lmem_regs(_xbuf9_lmem_name, INDEX)
            _xbuf_find_log_base_2(_xbuf9_lmem_pool_size)
            #define_eval _XBUF_BUF_ADDR     (_xbuf9_lmem_pools_base + _xbuf9_lmem_offset)
            #define_eval _xbuf9_lmem_index  (INDEX)
            #if ( (_xbuf9_lmem_pools_base == 0) && (_xbuf9_lmem_offset < 256) )
                #define XBUF_ACTIVATE_OPTIMIZED 1
            #endif
        #elif (streq(_the_xbuf_name, _xbuf10_lmem_name))
            _xbuf_set_lmem_handle_active_buf(10, INDEX)
            _xbuf_associate_lmem_regs(_xbuf10_lmem_name, INDEX)
            _xbuf_find_log_base_2(_xbuf10_lmem_pool_size)
            #define_eval _XBUF_BUF_ADDR     (_xbuf10_lmem_pools_base + _xbuf10_lmem_offset)
            #define_eval _xbuf10_lmem_index (INDEX)
            #if ( (_xbuf10_lmem_pools_base == 0) && (_xbuf10_lmem_offset < 256) )
                #define XBUF_ACTIVATE_OPTIMIZED 1
            #endif
        #elif (streq(_the_xbuf_name, _xbuf11_lmem_name))
            _xbuf_set_lmem_handle_active_buf(11, INDEX)
            _xbuf_associate_lmem_regs(_xbuf11_lmem_name, INDEX)
            _xbuf_find_log_base_2(_xbuf11_lmem_pool_size)
            #define_eval _XBUF_BUF_ADDR     (_xbuf11_lmem_pools_base + _xbuf11_lmem_offset)
            #define_eval _xbuf11_lmem_index (INDEX)
            #if ( (_xbuf11_lmem_pools_base == 0) && (_xbuf11_lmem_offset < 256) )
                #define XBUF_ACTIVATE_OPTIMIZED 1
            #endif
        #elif (streq(_the_xbuf_name, _xbuf12_lmem_name))
            _xbuf_set_lmem_handle_active_buf(12, INDEX)
            _xbuf_associate_lmem_regs(_xbuf12_lmem_name, INDEX)
            _xbuf_find_log_base_2(_xbuf12_lmem_pool_size)
            #define_eval _XBUF_BUF_ADDR     (_xbuf12_lmem_pools_base + _xbuf12_lmem_offset)
            #define_eval _xbuf12_lmem_index (INDEX)
            #if ( (_xbuf12_lmem_pools_base == 0) && (_xbuf12_lmem_offset < 256) )
                #define XBUF_ACTIVATE_OPTIMIZED 1
            #endif
        #elif (streq(_the_xbuf_name, _xbuf13_lmem_name))
            _xbuf_set_lmem_handle_active_buf(13, INDEX)
            _xbuf_associate_lmem_regs(_xbuf13_lmem_name, INDEX)
            _xbuf_find_log_base_2(_xbuf13_lmem_pool_size)
            #define_eval _XBUF_BUF_ADDR     (_xbuf13_lmem_pools_base + _xbuf13_lmem_offset)
            #define_eval _xbuf13_lmem_index (INDEX)
            #if ( (_xbuf13_lmem_pools_base == 0) && (_xbuf13_lmem_offset < 256) )
                #define XBUF_ACTIVATE_OPTIMIZED 1
            #endif
        #elif (streq(_the_xbuf_name, _xbuf14_lmem_name))
            _xbuf_set_lmem_handle_active_buf(14, INDEX)
            _xbuf_associate_lmem_regs(_xbuf14_lmem_name, INDEX)
            _xbuf_find_log_base_2(_xbuf14_lmem_pool_size)
            #define_eval _XBUF_BUF_ADDR     (_xbuf14_lmem_pools_base + _xbuf14_lmem_offset)
            #define_eval _xbuf14_lmem_index (INDEX)
            #if ( (_xbuf14_lmem_pools_base == 0) && (_xbuf14_lmem_offset < 256) )
                #define XBUF_ACTIVATE_OPTIMIZED 1
            #endif
        #elif (streq(_the_xbuf_name, _xbuf15_lmem_name))
            _xbuf_set_lmem_handle_active_buf(15, INDEX)
            _xbuf_associate_lmem_regs(_xbuf15_lmem_name, INDEX)
            _xbuf_find_log_base_2(_xbuf15_lmem_pool_size)
            #define_eval _XBUF_BUF_ADDR     (_xbuf15_lmem_pools_base + _xbuf15_lmem_offset)
            #define_eval _xbuf15_lmem_index (INDEX)
            #if ( (_xbuf15_lmem_pools_base == 0) && (_xbuf15_lmem_offset < 256) )
                #define XBUF_ACTIVATE_OPTIMIZED 1
            #endif
        #elif (streq(_the_xbuf_name, _xbuf16_lmem_name))
            _xbuf_set_lmem_handle_active_buf(16, INDEX)
            _xbuf_associate_lmem_regs(_xbuf16_lmem_name, INDEX)
            _xbuf_find_log_base_2(_xbuf16_lmem_pool_size)
            #define_eval _XBUF_BUF_ADDR     (_xbuf16_lmem_pools_base + _xbuf16_lmem_offset)
            #define_eval _xbuf16_lmem_index (INDEX)
            #if ( (_xbuf16_lmem_pools_base == 0) && (_xbuf16_lmem_offset < 256) )
                #define XBUF_ACTIVATE_OPTIMIZED 1
            #endif
        #else
            #error "xbuf_activate: Invalid str_xbuf_name."
        #endif

        // For 4 context mode: 0, 2, 4 and 6, we have to shift right by 1 to get
        // 0, 1, 2 or 3 and shift lelt by _LOG_BASE_2. The net is shifting left by
        // (_LOG_BASE_2 - 1)
        #ifdef CONTEXT_MODE_4
            #define_eval _LOG_BASE_2    (_LOG_BASE_2 - 1)
        #endif

        // calculate buffer absolute address based on thread_id value
        // optimize it with 1 instruction if base=0 and size < 256, 2 instructions otherwise
        #ifdef XBUF_ACTIVATE_OPTIMIZED
            #if (is_ct_const(thread_id) || isimport(thread_id))
                add_shf_left(_xbuf_act_buf_addr, _XBUF_BUF_ADDR, thread_id, _LOG_BASE_2)
            #else
                alu_shf[_xbuf_act_buf_addr, _XBUF_BUF_ADDR, OR, thread_id, <<_LOG_BASE_2]
            #endif
        #else
            add_shf_left(_xbuf_act_buf_addr, _XBUF_BUF_ADDR, thread_id, _LOG_BASE_2)
        #endif

        // set local memory handle ACTIVE_LM_ADDR_0 or ACTIVE_LM_ADDR_1
        local_csr_wr[active_lm_addr_/**/INDEX, _xbuf_act_buf_addr]

        // optional "nops" depending on input value of WAIT_ACTION
        #if (WAIT_ACTION == 1)
            nop
            nop
            nop
        #endif

        // clean up name spaces
        #undef _XBUF_BUF_ADDR
        #undef _LOG_BASE_2
    #else
        #error "This macro only supports LMEM buffers."
    #endif

.end
#endm


/** Locally declares contiguous 'NUMBER_OF_REGS' sram xfer registers and sets an appropriate .xfer_order
 *
 * Registers may be referenced as an array.
 * This macro allows transfer registers to be divided into to a maximum of 4 buffers
 * in IXP compatible indirect reference mode and 8 buffers in NFP indirect
 * reference mode.
 *
 * @param str_xbuf_name  String uniquely identifying transfer register buffer
 * @param NUMBER_OF_REGS Number of transfer registers to allocate
 * @param RW_INDICATOR   String = read, write, or read_write, which                              which will specify the transfer registers as
 *                       read only, write only, or read/write transfer registers
 *
 * <b>Example 1:</b>
 * @code
 * xbuf_alloc($x, 8, read)                         // setup read only xfer regs $x0 thru $x7
 * sram_read($x0, addr1, addr2, LWCOUNT2, pend)    // load the first 2 xfer regs
 * xbuf_free($x)
 * @endcode
 *
 * <b>Example 2:</b> [Using localmem]
 * @code
 * xbuf_alloc(lmem_buf0, 8, READ_WRITE)
 * xbuf_bind_address(lmem_buf0, 0x100, 0x20, 0x0)
 * xbuf_activate(lmem_buf0, 0, 0, 1)
 *
 * ipv4_cksum_verify(lmem_buf0, 0xb)
 * ...
 * ...
 * xbuf_deactivate(lmem_buf0)
 * xbuf_free(lmem_buf0)
 * @endcode
 *
 * @instruction_cnt 0
 *
 * @note For local memory buffers: @n
 *       lmem_buf0, lmem_buf1, ..., lmem_buf15 must be used.
 */
#macro xbuf_alloc(str_xbuf_name, NUMBER_OF_REGS, RW_INDICATOR)
    #if (strstr(str_xbuf_name, n$) == 1)
        #error "Next neighbor registers are not supported by xbuf."
    #elif ((!defined(__NFP_INDIRECT_REF_FORMAT_NFPXXXX)) && (strstr(str_xbuf_name, $$) > 0))
        _xbuf_d_alloc(str_xbuf_name, NUMBER_OF_REGS, RW_INDICATOR)
    #elif ( streq(str_xbuf_name, 'lmem_buf0') || \
            streq(str_xbuf_name, 'lmem_buf1') || \
            streq(str_xbuf_name, 'lmem_buf2') || \
            streq(str_xbuf_name, 'lmem_buf3') || \
            streq(str_xbuf_name, 'lmem_buf4') || \
            streq(str_xbuf_name, 'lmem_buf5') || \
            streq(str_xbuf_name, 'lmem_buf6') || \
            streq(str_xbuf_name, 'lmem_buf7') || \
            streq(str_xbuf_name, 'lmem_buf8') || \
            streq(str_xbuf_name, 'lmem_buf9') || \
            streq(str_xbuf_name, 'lmem_buf10') || \
            streq(str_xbuf_name, 'lmem_buf11') || \
            streq(str_xbuf_name, 'lmem_buf12') || \
            streq(str_xbuf_name, 'lmem_buf13') || \
            streq(str_xbuf_name, 'lmem_buf14') || \
            streq(str_xbuf_name, 'lmem_buf15') )
        _xbuf_lmem_alloc(str_xbuf_name, NUMBER_OF_REGS, RW_INDICATOR)
    #elif (strstr(str_xbuf_name, $) == 1)
        #if (!defined(_xbuf1_name))
            #define_eval _xbuf1_name (str_xbuf_name)
            #define_eval _xbuf1_size (NUMBER_OF_REGS)
        #elif (!defined(_xbuf2_name))
            #define_eval _xbuf2_name (str_xbuf_name)
            #define_eval _xbuf2_size (NUMBER_OF_REGS)
        #elif (!defined(_xbuf3_name))
            #define_eval _xbuf3_name (str_xbuf_name)
            #define_eval _xbuf3_size (NUMBER_OF_REGS)
        #elif (!defined(_xbuf4_name))
            #define_eval _xbuf4_name (str_xbuf_name)
            #define_eval _xbuf4_size (NUMBER_OF_REGS)
        #elif (defined(__NFP_INDIRECT_REF_FORMAT_NFPXXXX) && (!defined(_xbuf5_name)))
            #define_eval _xbuf5_name (str_xbuf_name)
            #define_eval _xbuf5_size (NUMBER_OF_REGS)
        #elif (defined(__NFP_INDIRECT_REF_FORMAT_NFPXXXX) && (!defined(_xbuf6_name)))
            #define_eval _xbuf6_name (str_xbuf_name)
            #define_eval _xbuf6_size (NUMBER_OF_REGS)
        #elif (defined(__NFP_INDIRECT_REF_FORMAT_NFPXXXX) && (!defined(_xbuf7_name)))
            #define_eval _xbuf7_name (str_xbuf_name)
            #define_eval _xbuf7_size (NUMBER_OF_REGS)
        #elif (defined(__NFP_INDIRECT_REF_FORMAT_NFPXXXX) && (!defined(_xbuf8_name)))
            #define_eval _xbuf8_name (str_xbuf_name)
            #define_eval _xbuf8_size (NUMBER_OF_REGS)
        #else
            #error 2 "xbuf_alloc: No more xfer bufs available."
        #endif

        #ifdef _REG_QUALIFIERS
            #undef _REG_QUALIFIERS
        #endif
        #if (streq(RW_INDICATOR, read_write))
            #define _REG_QUALIFIERS
        #elif (streq(RW_INDICATOR, read_write_visible))
            #define _REG_QUALIFIERS visible
        #elif (streq(RW_INDICATOR, read))
                #define _REG_QUALIFIERS read
        #elif (streq(RW_INDICATOR, read_visible))
            #define _REG_QUALIFIERS read visible
        #elif (streq(RW_INDICATOR, write))
            #define _REG_QUALIFIERS write
        #elif (streq(RW_INDICATOR, write_visible))
            #define _REG_QUALIFIERS write visible
        #else  // if (RW_INDICATOR != read, write, read_write
            #error 2 "xbuf_alloc: Must specify read, write, read_write, read_visible, write_visible or read_write visible 3rd arg."
        #endif

        #if (NUMBER_OF_REGS == 1)
            .reg _REG_QUALIFIERS str_xbuf_name/**/0
        #elif (NUMBER_OF_REGS == 2)
            .reg _REG_QUALIFIERS str_xbuf_name/**/0, str_xbuf_name/**/1
            .xfer_order str_xbuf_name/**/0, str_xbuf_name/**/1
        #elif (NUMBER_OF_REGS == 3)
            .reg _REG_QUALIFIERS str_xbuf_name/**/0, str_xbuf_name/**/1, str_xbuf_name/**/2
            .xfer_order str_xbuf_name/**/0, str_xbuf_name/**/1, str_xbuf_name/**/2
        #elif (NUMBER_OF_REGS == 4)
            .reg _REG_QUALIFIERS str_xbuf_name/**/0, str_xbuf_name/**/1, str_xbuf_name/**/2, str_xbuf_name/**/3
            .xfer_order str_xbuf_name/**/0, str_xbuf_name/**/1, str_xbuf_name/**/2, str_xbuf_name/**/3
        #elif (NUMBER_OF_REGS == 5)
            .reg _REG_QUALIFIERS str_xbuf_name/**/0, str_xbuf_name/**/1, str_xbuf_name/**/2, str_xbuf_name/**/3, str_xbuf_name/**/4
            .xfer_order str_xbuf_name/**/0, str_xbuf_name/**/1, str_xbuf_name/**/2, str_xbuf_name/**/3, str_xbuf_name/**/4
        #elif (NUMBER_OF_REGS == 6)
            .reg _REG_QUALIFIERS str_xbuf_name/**/0, str_xbuf_name/**/1, str_xbuf_name/**/2, str_xbuf_name/**/3, str_xbuf_name/**/4, str_xbuf_name/**/5
            .xfer_order str_xbuf_name/**/0, str_xbuf_name/**/1, str_xbuf_name/**/2, str_xbuf_name/**/3, str_xbuf_name/**/4, str_xbuf_name/**/5
        #elif (NUMBER_OF_REGS == 7)
            .reg _REG_QUALIFIERS str_xbuf_name/**/0, str_xbuf_name/**/1, str_xbuf_name/**/2, str_xbuf_name/**/3, str_xbuf_name/**/4, str_xbuf_name/**/5, str_xbuf_name/**/6
            .xfer_order str_xbuf_name/**/0, str_xbuf_name/**/1, str_xbuf_name/**/2, str_xbuf_name/**/3, str_xbuf_name/**/4, str_xbuf_name/**/5, str_xbuf_name/**/6
        #elif (NUMBER_OF_REGS == 8)
            .reg _REG_QUALIFIERS str_xbuf_name/**/0, str_xbuf_name/**/1, str_xbuf_name/**/2, str_xbuf_name/**/3, str_xbuf_name/**/4, str_xbuf_name/**/5, str_xbuf_name/**/6, str_xbuf_name/**/7
            .xfer_order str_xbuf_name/**/0, str_xbuf_name/**/1, str_xbuf_name/**/2, str_xbuf_name/**/3, str_xbuf_name/**/4, str_xbuf_name/**/5, str_xbuf_name/**/6, str_xbuf_name/**/7
        #elif (NUMBER_OF_REGS == 9)
            .reg _REG_QUALIFIERS str_xbuf_name/**/0, str_xbuf_name/**/1, str_xbuf_name/**/2, str_xbuf_name/**/3, str_xbuf_name/**/4, str_xbuf_name/**/5, str_xbuf_name/**/6, str_xbuf_name/**/7, str_xbuf_name/**/8
            .xfer_order str_xbuf_name/**/0, str_xbuf_name/**/1, str_xbuf_name/**/2, str_xbuf_name/**/3, str_xbuf_name/**/4, str_xbuf_name/**/5, str_xbuf_name/**/6, str_xbuf_name/**/7, str_xbuf_name/**/8
        #elif (NUMBER_OF_REGS == 10)
            .reg _REG_QUALIFIERS str_xbuf_name/**/0, str_xbuf_name/**/1, str_xbuf_name/**/2, str_xbuf_name/**/3, str_xbuf_name/**/4, str_xbuf_name/**/5, str_xbuf_name/**/6, str_xbuf_name/**/7, str_xbuf_name/**/8, str_xbuf_name/**/9
            .xfer_order str_xbuf_name/**/0, str_xbuf_name/**/1, str_xbuf_name/**/2, str_xbuf_name/**/3, str_xbuf_name/**/4, str_xbuf_name/**/5, str_xbuf_name/**/6, str_xbuf_name/**/7, str_xbuf_name/**/8, str_xbuf_name/**/9
        #elif (NUMBER_OF_REGS == 11)
            .reg _REG_QUALIFIERS str_xbuf_name/**/0, str_xbuf_name/**/1, str_xbuf_name/**/2, str_xbuf_name/**/3, str_xbuf_name/**/4, str_xbuf_name/**/5, str_xbuf_name/**/6, str_xbuf_name/**/7, str_xbuf_name/**/8, str_xbuf_name/**/9, str_xbuf_name/**/10
            .xfer_order str_xbuf_name/**/0, str_xbuf_name/**/1, str_xbuf_name/**/2, str_xbuf_name/**/3, str_xbuf_name/**/4, str_xbuf_name/**/5, str_xbuf_name/**/6, str_xbuf_name/**/7, str_xbuf_name/**/8, str_xbuf_name/**/9, str_xbuf_name/**/10
        #elif (NUMBER_OF_REGS == 12)
            .reg _REG_QUALIFIERS str_xbuf_name/**/0, str_xbuf_name/**/1, str_xbuf_name/**/2, str_xbuf_name/**/3, str_xbuf_name/**/4, str_xbuf_name/**/5, str_xbuf_name/**/6, str_xbuf_name/**/7, str_xbuf_name/**/8, str_xbuf_name/**/9, str_xbuf_name/**/10, str_xbuf_name/**/11
            .xfer_order str_xbuf_name/**/0, str_xbuf_name/**/1, str_xbuf_name/**/2, str_xbuf_name/**/3, str_xbuf_name/**/4, str_xbuf_name/**/5, str_xbuf_name/**/6, str_xbuf_name/**/7, str_xbuf_name/**/8, str_xbuf_name/**/9, str_xbuf_name/**/10, str_xbuf_name/**/11
        #elif (NUMBER_OF_REGS == 13)
            .reg _REG_QUALIFIERS str_xbuf_name/**/0, str_xbuf_name/**/1, str_xbuf_name/**/2, str_xbuf_name/**/3, str_xbuf_name/**/4, str_xbuf_name/**/5, str_xbuf_name/**/6, str_xbuf_name/**/7, str_xbuf_name/**/8, str_xbuf_name/**/9, str_xbuf_name/**/10, str_xbuf_name/**/11, str_xbuf_name/**/12
            .xfer_order str_xbuf_name/**/0, str_xbuf_name/**/1, str_xbuf_name/**/2, str_xbuf_name/**/3, str_xbuf_name/**/4, str_xbuf_name/**/5, str_xbuf_name/**/6, str_xbuf_name/**/7, str_xbuf_name/**/8, str_xbuf_name/**/9, str_xbuf_name/**/10, str_xbuf_name/**/11, str_xbuf_name/**/12
        #elif (NUMBER_OF_REGS == 14)
            .reg _REG_QUALIFIERS str_xbuf_name/**/0, str_xbuf_name/**/1, str_xbuf_name/**/2, str_xbuf_name/**/3, str_xbuf_name/**/4, str_xbuf_name/**/5, str_xbuf_name/**/6, str_xbuf_name/**/7, str_xbuf_name/**/8, str_xbuf_name/**/9, str_xbuf_name/**/10, str_xbuf_name/**/11, str_xbuf_name/**/12, str_xbuf_name/**/13
            .xfer_order str_xbuf_name/**/0, str_xbuf_name/**/1, str_xbuf_name/**/2, str_xbuf_name/**/3, str_xbuf_name/**/4, str_xbuf_name/**/5, str_xbuf_name/**/6, str_xbuf_name/**/7, str_xbuf_name/**/8, str_xbuf_name/**/9, str_xbuf_name/**/10, str_xbuf_name/**/11, str_xbuf_name/**/12, str_xbuf_name/**/13
        #elif (NUMBER_OF_REGS == 15)
            .reg _REG_QUALIFIERS str_xbuf_name/**/0, str_xbuf_name/**/1, str_xbuf_name/**/2, str_xbuf_name/**/3, str_xbuf_name/**/4, str_xbuf_name/**/5, str_xbuf_name/**/6, str_xbuf_name/**/7, str_xbuf_name/**/8, str_xbuf_name/**/9, str_xbuf_name/**/10, str_xbuf_name/**/11, str_xbuf_name/**/12, str_xbuf_name/**/13, str_xbuf_name/**/14
            .xfer_order str_xbuf_name/**/0, str_xbuf_name/**/1, str_xbuf_name/**/2, str_xbuf_name/**/3, str_xbuf_name/**/4, str_xbuf_name/**/5, str_xbuf_name/**/6, str_xbuf_name/**/7, str_xbuf_name/**/8, str_xbuf_name/**/9, str_xbuf_name/**/10, str_xbuf_name/**/11, str_xbuf_name/**/12, str_xbuf_name/**/13, str_xbuf_name/**/14
        #elif (NUMBER_OF_REGS == 16)
            .reg _REG_QUALIFIERS str_xbuf_name/**/0, str_xbuf_name/**/1, str_xbuf_name/**/2, str_xbuf_name/**/3, str_xbuf_name/**/4, str_xbuf_name/**/5, str_xbuf_name/**/6, str_xbuf_name/**/7, str_xbuf_name/**/8, str_xbuf_name/**/9, str_xbuf_name/**/10, str_xbuf_name/**/11, str_xbuf_name/**/12, str_xbuf_name/**/13, str_xbuf_name/**/14, str_xbuf_name/**/15
            .xfer_order str_xbuf_name/**/0, str_xbuf_name/**/1, str_xbuf_name/**/2, str_xbuf_name/**/3, str_xbuf_name/**/4, str_xbuf_name/**/5, str_xbuf_name/**/6, str_xbuf_name/**/7, str_xbuf_name/**/8, str_xbuf_name/**/9, str_xbuf_name/**/10, str_xbuf_name/**/11, str_xbuf_name/**/12, str_xbuf_name/**/13, str_xbuf_name/**/14, str_xbuf_name/**/15
        #elif (defined(__NFP_INDIRECT_REF_FORMAT_NFPXXXX) && (NUMBER_OF_REGS == 17))
            .reg _REG_QUALIFIERS str_xbuf_name/**/0, str_xbuf_name/**/1, str_xbuf_name/**/2, str_xbuf_name/**/3, str_xbuf_name/**/4, str_xbuf_name/**/5, str_xbuf_name/**/6, str_xbuf_name/**/7, str_xbuf_name/**/8, str_xbuf_name/**/9, str_xbuf_name/**/10, str_xbuf_name/**/11, str_xbuf_name/**/12, str_xbuf_name/**/13, str_xbuf_name/**/14, str_xbuf_name/**/15, str_xbuf_name/**/16
            .xfer_order str_xbuf_name/**/0, str_xbuf_name/**/1, str_xbuf_name/**/2, str_xbuf_name/**/3, str_xbuf_name/**/4, str_xbuf_name/**/5, str_xbuf_name/**/6, str_xbuf_name/**/7, str_xbuf_name/**/8, str_xbuf_name/**/9, str_xbuf_name/**/10, str_xbuf_name/**/11, str_xbuf_name/**/12, str_xbuf_name/**/13, str_xbuf_name/**/14, str_xbuf_name/**/15, str_xbuf_name/**/16
        #elif (defined(__NFP_INDIRECT_REF_FORMAT_NFPXXXX) && (NUMBER_OF_REGS == 18))
            .reg _REG_QUALIFIERS str_xbuf_name/**/0, str_xbuf_name/**/1, str_xbuf_name/**/2, str_xbuf_name/**/3, str_xbuf_name/**/4, str_xbuf_name/**/5, str_xbuf_name/**/6, str_xbuf_name/**/7, str_xbuf_name/**/8, str_xbuf_name/**/9, str_xbuf_name/**/10, str_xbuf_name/**/11, str_xbuf_name/**/12, str_xbuf_name/**/13, str_xbuf_name/**/14, str_xbuf_name/**/15, str_xbuf_name/**/16, str_xbuf_name/**/17
            .xfer_order str_xbuf_name/**/0, str_xbuf_name/**/1, str_xbuf_name/**/2, str_xbuf_name/**/3, str_xbuf_name/**/4, str_xbuf_name/**/5, str_xbuf_name/**/6, str_xbuf_name/**/7, str_xbuf_name/**/8, str_xbuf_name/**/9, str_xbuf_name/**/10, str_xbuf_name/**/11, str_xbuf_name/**/12, str_xbuf_name/**/13, str_xbuf_name/**/14, str_xbuf_name/**/15, str_xbuf_name/**/16, str_xbuf_name/**/17
        #elif (defined(__NFP_INDIRECT_REF_FORMAT_NFPXXXX) && (NUMBER_OF_REGS == 19))
            .reg _REG_QUALIFIERS str_xbuf_name/**/0, str_xbuf_name/**/1, str_xbuf_name/**/2, str_xbuf_name/**/3, str_xbuf_name/**/4, str_xbuf_name/**/5, str_xbuf_name/**/6, str_xbuf_name/**/7, str_xbuf_name/**/8, str_xbuf_name/**/9, str_xbuf_name/**/10, str_xbuf_name/**/11, str_xbuf_name/**/12, str_xbuf_name/**/13, str_xbuf_name/**/14, str_xbuf_name/**/15, str_xbuf_name/**/16, str_xbuf_name/**/17, str_xbuf_name/**/18
            .xfer_order str_xbuf_name/**/0, str_xbuf_name/**/1, str_xbuf_name/**/2, str_xbuf_name/**/3, str_xbuf_name/**/4, str_xbuf_name/**/5, str_xbuf_name/**/6, str_xbuf_name/**/7, str_xbuf_name/**/8, str_xbuf_name/**/9, str_xbuf_name/**/10, str_xbuf_name/**/11, str_xbuf_name/**/12, str_xbuf_name/**/13, str_xbuf_name/**/14, str_xbuf_name/**/15, str_xbuf_name/**/16, str_xbuf_name/**/17, str_xbuf_name/**/18
        #elif (defined(__NFP_INDIRECT_REF_FORMAT_NFPXXXX) && (NUMBER_OF_REGS == 20))
            .reg _REG_QUALIFIERS str_xbuf_name/**/0, str_xbuf_name/**/1, str_xbuf_name/**/2, str_xbuf_name/**/3, str_xbuf_name/**/4, str_xbuf_name/**/5, str_xbuf_name/**/6, str_xbuf_name/**/7, str_xbuf_name/**/8, str_xbuf_name/**/9, str_xbuf_name/**/10, str_xbuf_name/**/11, str_xbuf_name/**/12, str_xbuf_name/**/13, str_xbuf_name/**/14, str_xbuf_name/**/15, str_xbuf_name/**/16, str_xbuf_name/**/17, str_xbuf_name/**/18, str_xbuf_name/**/19
            .xfer_order str_xbuf_name/**/0, str_xbuf_name/**/1, str_xbuf_name/**/2, str_xbuf_name/**/3, str_xbuf_name/**/4, str_xbuf_name/**/5, str_xbuf_name/**/6, str_xbuf_name/**/7, str_xbuf_name/**/8, str_xbuf_name/**/9, str_xbuf_name/**/10, str_xbuf_name/**/11, str_xbuf_name/**/12, str_xbuf_name/**/13, str_xbuf_name/**/14, str_xbuf_name/**/15, str_xbuf_name/**/16, str_xbuf_name/**/17, str_xbuf_name/**/18, str_xbuf_name/**/19
        #elif (defined(__NFP_INDIRECT_REF_FORMAT_NFPXXXX) && (NUMBER_OF_REGS == 21))
            .reg _REG_QUALIFIERS str_xbuf_name/**/0, str_xbuf_name/**/1, str_xbuf_name/**/2, str_xbuf_name/**/3, str_xbuf_name/**/4, str_xbuf_name/**/5, str_xbuf_name/**/6, str_xbuf_name/**/7, str_xbuf_name/**/8, str_xbuf_name/**/9, str_xbuf_name/**/10, str_xbuf_name/**/11, str_xbuf_name/**/12, str_xbuf_name/**/13, str_xbuf_name/**/14, str_xbuf_name/**/15, str_xbuf_name/**/16, str_xbuf_name/**/17, str_xbuf_name/**/18, str_xbuf_name/**/19, str_xbuf_name/**/20
            .xfer_order str_xbuf_name/**/0, str_xbuf_name/**/1, str_xbuf_name/**/2, str_xbuf_name/**/3, str_xbuf_name/**/4, str_xbuf_name/**/5, str_xbuf_name/**/6, str_xbuf_name/**/7, str_xbuf_name/**/8, str_xbuf_name/**/9, str_xbuf_name/**/10, str_xbuf_name/**/11, str_xbuf_name/**/12, str_xbuf_name/**/13, str_xbuf_name/**/14, str_xbuf_name/**/15, str_xbuf_name/**/16, str_xbuf_name/**/17, str_xbuf_name/**/18, str_xbuf_name/**/19, str_xbuf_name/**/20
        #elif (defined(__NFP_INDIRECT_REF_FORMAT_NFPXXXX) && (NUMBER_OF_REGS == 22))
            .reg _REG_QUALIFIERS str_xbuf_name/**/0, str_xbuf_name/**/1, str_xbuf_name/**/2, str_xbuf_name/**/3, str_xbuf_name/**/4, str_xbuf_name/**/5, str_xbuf_name/**/6, str_xbuf_name/**/7, str_xbuf_name/**/8, str_xbuf_name/**/9, str_xbuf_name/**/10, str_xbuf_name/**/11, str_xbuf_name/**/12, str_xbuf_name/**/13, str_xbuf_name/**/14, str_xbuf_name/**/15, str_xbuf_name/**/16, str_xbuf_name/**/17, str_xbuf_name/**/18, str_xbuf_name/**/19, str_xbuf_name/**/20, str_xbuf_name/**/21
            .xfer_order str_xbuf_name/**/0, str_xbuf_name/**/1, str_xbuf_name/**/2, str_xbuf_name/**/3, str_xbuf_name/**/4, str_xbuf_name/**/5, str_xbuf_name/**/6, str_xbuf_name/**/7, str_xbuf_name/**/8, str_xbuf_name/**/9, str_xbuf_name/**/10, str_xbuf_name/**/11, str_xbuf_name/**/12, str_xbuf_name/**/13, str_xbuf_name/**/14, str_xbuf_name/**/15, str_xbuf_name/**/16, str_xbuf_name/**/17, str_xbuf_name/**/18, str_xbuf_name/**/19, str_xbuf_name/**/20, str_xbuf_name/**/21
        #elif (defined(__NFP_INDIRECT_REF_FORMAT_NFPXXXX) && (NUMBER_OF_REGS == 23))
            .reg _REG_QUALIFIERS str_xbuf_name/**/0, str_xbuf_name/**/1, str_xbuf_name/**/2, str_xbuf_name/**/3, str_xbuf_name/**/4, str_xbuf_name/**/5, str_xbuf_name/**/6, str_xbuf_name/**/7, str_xbuf_name/**/8, str_xbuf_name/**/9, str_xbuf_name/**/10, str_xbuf_name/**/11, str_xbuf_name/**/12, str_xbuf_name/**/13, str_xbuf_name/**/14, str_xbuf_name/**/15, str_xbuf_name/**/16, str_xbuf_name/**/17, str_xbuf_name/**/18, str_xbuf_name/**/19, str_xbuf_name/**/20, str_xbuf_name/**/21, str_xbuf_name/**/22
            .xfer_order str_xbuf_name/**/0, str_xbuf_name/**/1, str_xbuf_name/**/2, str_xbuf_name/**/3, str_xbuf_name/**/4, str_xbuf_name/**/5, str_xbuf_name/**/6, str_xbuf_name/**/7, str_xbuf_name/**/8, str_xbuf_name/**/9, str_xbuf_name/**/10, str_xbuf_name/**/11, str_xbuf_name/**/12, str_xbuf_name/**/13, str_xbuf_name/**/14, str_xbuf_name/**/15, str_xbuf_name/**/16, str_xbuf_name/**/17, str_xbuf_name/**/18, str_xbuf_name/**/19, str_xbuf_name/**/20, str_xbuf_name/**/21, str_xbuf_name/**/22
        #elif (defined(__NFP_INDIRECT_REF_FORMAT_NFPXXXX) && (NUMBER_OF_REGS == 24))
            .reg _REG_QUALIFIERS str_xbuf_name/**/0, str_xbuf_name/**/1, str_xbuf_name/**/2, str_xbuf_name/**/3, str_xbuf_name/**/4, str_xbuf_name/**/5, str_xbuf_name/**/6, str_xbuf_name/**/7, str_xbuf_name/**/8, str_xbuf_name/**/9, str_xbuf_name/**/10, str_xbuf_name/**/11, str_xbuf_name/**/12, str_xbuf_name/**/13, str_xbuf_name/**/14, str_xbuf_name/**/15, str_xbuf_name/**/16, str_xbuf_name/**/17, str_xbuf_name/**/18, str_xbuf_name/**/19, str_xbuf_name/**/20, str_xbuf_name/**/21, str_xbuf_name/**/22, str_xbuf_name/**/23
            .xfer_order str_xbuf_name/**/0, str_xbuf_name/**/1, str_xbuf_name/**/2, str_xbuf_name/**/3, str_xbuf_name/**/4, str_xbuf_name/**/5, str_xbuf_name/**/6, str_xbuf_name/**/7, str_xbuf_name/**/8, str_xbuf_name/**/9, str_xbuf_name/**/10, str_xbuf_name/**/11, str_xbuf_name/**/12, str_xbuf_name/**/13, str_xbuf_name/**/14, str_xbuf_name/**/15, str_xbuf_name/**/16, str_xbuf_name/**/17, str_xbuf_name/**/18, str_xbuf_name/**/19, str_xbuf_name/**/20, str_xbuf_name/**/21, str_xbuf_name/**/22, str_xbuf_name/**/23
        #elif (defined(__NFP_INDIRECT_REF_FORMAT_NFPXXXX) && (NUMBER_OF_REGS == 25))
            .reg _REG_QUALIFIERS str_xbuf_name/**/0, str_xbuf_name/**/1, str_xbuf_name/**/2, str_xbuf_name/**/3, str_xbuf_name/**/4, str_xbuf_name/**/5, str_xbuf_name/**/6, str_xbuf_name/**/7, str_xbuf_name/**/8, str_xbuf_name/**/9, str_xbuf_name/**/10, str_xbuf_name/**/11, str_xbuf_name/**/12, str_xbuf_name/**/13, str_xbuf_name/**/14, str_xbuf_name/**/15, str_xbuf_name/**/16, str_xbuf_name/**/17, str_xbuf_name/**/18, str_xbuf_name/**/19, str_xbuf_name/**/20, str_xbuf_name/**/21, str_xbuf_name/**/22, str_xbuf_name/**/23, str_xbuf_name/**/24
            .xfer_order str_xbuf_name/**/0, str_xbuf_name/**/1, str_xbuf_name/**/2, str_xbuf_name/**/3, str_xbuf_name/**/4, str_xbuf_name/**/5, str_xbuf_name/**/6, str_xbuf_name/**/7, str_xbuf_name/**/8, str_xbuf_name/**/9, str_xbuf_name/**/10, str_xbuf_name/**/11, str_xbuf_name/**/12, str_xbuf_name/**/13, str_xbuf_name/**/14, str_xbuf_name/**/15, str_xbuf_name/**/16, str_xbuf_name/**/17, str_xbuf_name/**/18, str_xbuf_name/**/19, str_xbuf_name/**/20, str_xbuf_name/**/21, str_xbuf_name/**/22, str_xbuf_name/**/23, str_xbuf_name/**/24
        #elif (defined(__NFP_INDIRECT_REF_FORMAT_NFPXXXX) && (NUMBER_OF_REGS == 26))
            .reg _REG_QUALIFIERS str_xbuf_name/**/0, str_xbuf_name/**/1, str_xbuf_name/**/2, str_xbuf_name/**/3, str_xbuf_name/**/4, str_xbuf_name/**/5, str_xbuf_name/**/6, str_xbuf_name/**/7, str_xbuf_name/**/8, str_xbuf_name/**/9, str_xbuf_name/**/10, str_xbuf_name/**/11, str_xbuf_name/**/12, str_xbuf_name/**/13, str_xbuf_name/**/14, str_xbuf_name/**/15, str_xbuf_name/**/16, str_xbuf_name/**/17, str_xbuf_name/**/18, str_xbuf_name/**/19, str_xbuf_name/**/20, str_xbuf_name/**/21, str_xbuf_name/**/22, str_xbuf_name/**/23, str_xbuf_name/**/24, str_xbuf_name/**/25
            .xfer_order str_xbuf_name/**/0, str_xbuf_name/**/1, str_xbuf_name/**/2, str_xbuf_name/**/3, str_xbuf_name/**/4, str_xbuf_name/**/5, str_xbuf_name/**/6, str_xbuf_name/**/7, str_xbuf_name/**/8, str_xbuf_name/**/9, str_xbuf_name/**/10, str_xbuf_name/**/11, str_xbuf_name/**/12, str_xbuf_name/**/13, str_xbuf_name/**/14, str_xbuf_name/**/15, str_xbuf_name/**/16, str_xbuf_name/**/17, str_xbuf_name/**/18, str_xbuf_name/**/19, str_xbuf_name/**/20, str_xbuf_name/**/21, str_xbuf_name/**/22, str_xbuf_name/**/23, str_xbuf_name/**/24, str_xbuf_name/**/25
        #elif (defined(__NFP_INDIRECT_REF_FORMAT_NFPXXXX) && (NUMBER_OF_REGS == 27))
            .reg _REG_QUALIFIERS str_xbuf_name/**/0, str_xbuf_name/**/1, str_xbuf_name/**/2, str_xbuf_name/**/3, str_xbuf_name/**/4, str_xbuf_name/**/5, str_xbuf_name/**/6, str_xbuf_name/**/7, str_xbuf_name/**/8, str_xbuf_name/**/9, str_xbuf_name/**/10, str_xbuf_name/**/11, str_xbuf_name/**/12, str_xbuf_name/**/13, str_xbuf_name/**/14, str_xbuf_name/**/15, str_xbuf_name/**/16, str_xbuf_name/**/17, str_xbuf_name/**/18, str_xbuf_name/**/19, str_xbuf_name/**/20, str_xbuf_name/**/21, str_xbuf_name/**/22, str_xbuf_name/**/23, str_xbuf_name/**/24, str_xbuf_name/**/25, str_xbuf_name/**/26
            .xfer_order str_xbuf_name/**/0, str_xbuf_name/**/1, str_xbuf_name/**/2, str_xbuf_name/**/3, str_xbuf_name/**/4, str_xbuf_name/**/5, str_xbuf_name/**/6, str_xbuf_name/**/7, str_xbuf_name/**/8, str_xbuf_name/**/9, str_xbuf_name/**/10, str_xbuf_name/**/11, str_xbuf_name/**/12, str_xbuf_name/**/13, str_xbuf_name/**/14, str_xbuf_name/**/15, str_xbuf_name/**/16, str_xbuf_name/**/17, str_xbuf_name/**/18, str_xbuf_name/**/19, str_xbuf_name/**/20, str_xbuf_name/**/21, str_xbuf_name/**/22, str_xbuf_name/**/23, str_xbuf_name/**/24, str_xbuf_name/**/25, str_xbuf_name/**/26
        #elif (defined(__NFP_INDIRECT_REF_FORMAT_NFPXXXX) && (NUMBER_OF_REGS == 28))
            .reg _REG_QUALIFIERS str_xbuf_name/**/0, str_xbuf_name/**/1, str_xbuf_name/**/2, str_xbuf_name/**/3, str_xbuf_name/**/4, str_xbuf_name/**/5, str_xbuf_name/**/6, str_xbuf_name/**/7, str_xbuf_name/**/8, str_xbuf_name/**/9, str_xbuf_name/**/10, str_xbuf_name/**/11, str_xbuf_name/**/12, str_xbuf_name/**/13, str_xbuf_name/**/14, str_xbuf_name/**/15, str_xbuf_name/**/16, str_xbuf_name/**/17, str_xbuf_name/**/18, str_xbuf_name/**/19, str_xbuf_name/**/20, str_xbuf_name/**/21, str_xbuf_name/**/22, str_xbuf_name/**/23, str_xbuf_name/**/24, str_xbuf_name/**/25, str_xbuf_name/**/26, str_xbuf_name/**/27
            .xfer_order str_xbuf_name/**/0, str_xbuf_name/**/1, str_xbuf_name/**/2, str_xbuf_name/**/3, str_xbuf_name/**/4, str_xbuf_name/**/5, str_xbuf_name/**/6, str_xbuf_name/**/7, str_xbuf_name/**/8, str_xbuf_name/**/9, str_xbuf_name/**/10, str_xbuf_name/**/11, str_xbuf_name/**/12, str_xbuf_name/**/13, str_xbuf_name/**/14, str_xbuf_name/**/15, str_xbuf_name/**/16, str_xbuf_name/**/17, str_xbuf_name/**/18, str_xbuf_name/**/19, str_xbuf_name/**/20, str_xbuf_name/**/21, str_xbuf_name/**/22, str_xbuf_name/**/23, str_xbuf_name/**/24, str_xbuf_name/**/25, str_xbuf_name/**/26, str_xbuf_name/**/27
        #elif (defined(__NFP_INDIRECT_REF_FORMAT_NFPXXXX) && (NUMBER_OF_REGS == 29))
            .reg _REG_QUALIFIERS str_xbuf_name/**/0, str_xbuf_name/**/1, str_xbuf_name/**/2, str_xbuf_name/**/3, str_xbuf_name/**/4, str_xbuf_name/**/5, str_xbuf_name/**/6, str_xbuf_name/**/7, str_xbuf_name/**/8, str_xbuf_name/**/9, str_xbuf_name/**/10, str_xbuf_name/**/11, str_xbuf_name/**/12, str_xbuf_name/**/13, str_xbuf_name/**/14, str_xbuf_name/**/15, str_xbuf_name/**/16, str_xbuf_name/**/17, str_xbuf_name/**/18, str_xbuf_name/**/19, str_xbuf_name/**/20, str_xbuf_name/**/21, str_xbuf_name/**/22, str_xbuf_name/**/23, str_xbuf_name/**/24, str_xbuf_name/**/25, str_xbuf_name/**/26, str_xbuf_name/**/27, str_xbuf_name/**/28
            .xfer_order str_xbuf_name/**/0, str_xbuf_name/**/1, str_xbuf_name/**/2, str_xbuf_name/**/3, str_xbuf_name/**/4, str_xbuf_name/**/5, str_xbuf_name/**/6, str_xbuf_name/**/7, str_xbuf_name/**/8, str_xbuf_name/**/9, str_xbuf_name/**/10, str_xbuf_name/**/11, str_xbuf_name/**/12, str_xbuf_name/**/13, str_xbuf_name/**/14, str_xbuf_name/**/15, str_xbuf_name/**/16, str_xbuf_name/**/17, str_xbuf_name/**/18, str_xbuf_name/**/19, str_xbuf_name/**/20, str_xbuf_name/**/21, str_xbuf_name/**/22, str_xbuf_name/**/23, str_xbuf_name/**/24, str_xbuf_name/**/25, str_xbuf_name/**/26, str_xbuf_name/**/27, str_xbuf_name/**/28
        #elif (defined(__NFP_INDIRECT_REF_FORMAT_NFPXXXX) && (NUMBER_OF_REGS == 30))
            .reg _REG_QUALIFIERS str_xbuf_name/**/0, str_xbuf_name/**/1, str_xbuf_name/**/2, str_xbuf_name/**/3, str_xbuf_name/**/4, str_xbuf_name/**/5, str_xbuf_name/**/6, str_xbuf_name/**/7, str_xbuf_name/**/8, str_xbuf_name/**/9, str_xbuf_name/**/10, str_xbuf_name/**/11, str_xbuf_name/**/12, str_xbuf_name/**/13, str_xbuf_name/**/14, str_xbuf_name/**/15, str_xbuf_name/**/16, str_xbuf_name/**/17, str_xbuf_name/**/18, str_xbuf_name/**/19, str_xbuf_name/**/20, str_xbuf_name/**/21, str_xbuf_name/**/22, str_xbuf_name/**/23, str_xbuf_name/**/24, str_xbuf_name/**/25, str_xbuf_name/**/26, str_xbuf_name/**/27, str_xbuf_name/**/28, str_xbuf_name/**/29
            .xfer_order str_xbuf_name/**/0, str_xbuf_name/**/1, str_xbuf_name/**/2, str_xbuf_name/**/3, str_xbuf_name/**/4, str_xbuf_name/**/5, str_xbuf_name/**/6, str_xbuf_name/**/7, str_xbuf_name/**/8, str_xbuf_name/**/9, str_xbuf_name/**/10, str_xbuf_name/**/11, str_xbuf_name/**/12, str_xbuf_name/**/13, str_xbuf_name/**/14, str_xbuf_name/**/15, str_xbuf_name/**/16, str_xbuf_name/**/17, str_xbuf_name/**/18, str_xbuf_name/**/19, str_xbuf_name/**/20, str_xbuf_name/**/21, str_xbuf_name/**/22, str_xbuf_name/**/23, str_xbuf_name/**/24, str_xbuf_name/**/25, str_xbuf_name/**/26, str_xbuf_name/**/27, str_xbuf_name/**/28, str_xbuf_name/**/29
        #elif (defined(__NFP_INDIRECT_REF_FORMAT_NFPXXXX) && (NUMBER_OF_REGS == 31))
            .reg _REG_QUALIFIERS str_xbuf_name/**/0, str_xbuf_name/**/1, str_xbuf_name/**/2, str_xbuf_name/**/3, str_xbuf_name/**/4, str_xbuf_name/**/5, str_xbuf_name/**/6, str_xbuf_name/**/7, str_xbuf_name/**/8, str_xbuf_name/**/9, str_xbuf_name/**/10, str_xbuf_name/**/11, str_xbuf_name/**/12, str_xbuf_name/**/13, str_xbuf_name/**/14, str_xbuf_name/**/15, str_xbuf_name/**/16, str_xbuf_name/**/17, str_xbuf_name/**/18, str_xbuf_name/**/19, str_xbuf_name/**/20, str_xbuf_name/**/21, str_xbuf_name/**/22, str_xbuf_name/**/23, str_xbuf_name/**/24, str_xbuf_name/**/25, str_xbuf_name/**/26, str_xbuf_name/**/27, str_xbuf_name/**/28, str_xbuf_name/**/29, str_xbuf_name/**/30
            .xfer_order str_xbuf_name/**/0, str_xbuf_name/**/1, str_xbuf_name/**/2, str_xbuf_name/**/3, str_xbuf_name/**/4, str_xbuf_name/**/5, str_xbuf_name/**/6, str_xbuf_name/**/7, str_xbuf_name/**/8, str_xbuf_name/**/9, str_xbuf_name/**/10, str_xbuf_name/**/11, str_xbuf_name/**/12, str_xbuf_name/**/13, str_xbuf_name/**/14, str_xbuf_name/**/15, str_xbuf_name/**/16, str_xbuf_name/**/17, str_xbuf_name/**/18, str_xbuf_name/**/19, str_xbuf_name/**/20, str_xbuf_name/**/21, str_xbuf_name/**/22, str_xbuf_name/**/23, str_xbuf_name/**/24, str_xbuf_name/**/25, str_xbuf_name/**/26, str_xbuf_name/**/27, str_xbuf_name/**/28, str_xbuf_name/**/29, str_xbuf_name/**/30
        #elif (defined(__NFP_INDIRECT_REF_FORMAT_NFPXXXX) && (NUMBER_OF_REGS == 32))
            .reg _REG_QUALIFIERS str_xbuf_name/**/0, str_xbuf_name/**/1, str_xbuf_name/**/2, str_xbuf_name/**/3, str_xbuf_name/**/4, str_xbuf_name/**/5, str_xbuf_name/**/6, str_xbuf_name/**/7, str_xbuf_name/**/8, str_xbuf_name/**/9, str_xbuf_name/**/10, str_xbuf_name/**/11, str_xbuf_name/**/12, str_xbuf_name/**/13, str_xbuf_name/**/14, str_xbuf_name/**/15, str_xbuf_name/**/16, str_xbuf_name/**/17, str_xbuf_name/**/18, str_xbuf_name/**/19, str_xbuf_name/**/20, str_xbuf_name/**/21, str_xbuf_name/**/22, str_xbuf_name/**/23, str_xbuf_name/**/24, str_xbuf_name/**/25, str_xbuf_name/**/26, str_xbuf_name/**/27, str_xbuf_name/**/28, str_xbuf_name/**/29, str_xbuf_name/**/30, str_xbuf_name/**/31
            .xfer_order str_xbuf_name/**/0, str_xbuf_name/**/1, str_xbuf_name/**/2, str_xbuf_name/**/3, str_xbuf_name/**/4, str_xbuf_name/**/5, str_xbuf_name/**/6, str_xbuf_name/**/7, str_xbuf_name/**/8, str_xbuf_name/**/9, str_xbuf_name/**/10, str_xbuf_name/**/11, str_xbuf_name/**/12, str_xbuf_name/**/13, str_xbuf_name/**/14, str_xbuf_name/**/15, str_xbuf_name/**/16, str_xbuf_name/**/17, str_xbuf_name/**/18, str_xbuf_name/**/19, str_xbuf_name/**/20, str_xbuf_name/**/21, str_xbuf_name/**/22, str_xbuf_name/**/23, str_xbuf_name/**/24, str_xbuf_name/**/25, str_xbuf_name/**/26, str_xbuf_name/**/27, str_xbuf_name/**/28, str_xbuf_name/**/29, str_xbuf_name/**/30, str_xbuf_name/**/31
        #else
            #error 2 "xbuf_alloc: Maximum number of transfer registers exceeded."
        #endif
    #else // GPRs
        _xbuf_gpr_alloc(str_xbuf_name, NUMBER_OF_REGS, RW_INDICATOR)
    #endif
#endm


/** Used to deallocate transfer registers.
 *
 * @param str_xbuf_name  Name of transfer register buffer
 *
 * <b>Example:</b>
 * @code
 * xbuf_alloc($x, 8)                               // setup xfer regs $x0 thru $x7
 * sram_read($x0, addr1, addr2, LWCOUNT2, pend)    // load the first 2 xfer regs
 * xbuf_free($x)
 * @endcode
 *
 * <b>Example 2:</b> [Using Localmem]
 * @code
 * xbuf_alloc(lmem_buf0, 8, READ_WRITE)
 * xbuf_bind_address(lmem_buf0, 0x100, 0x20, 0x0)
 * xbuf_activate(lmem_buf0, 0, 0, 1)
 *
 * ipv4_cksum_verify(lmem_buf0, 0xb)
 * ...
 * ...
 * xbuf_deactivate(lmem_buf0)
 * xbuf_free(lmem_buf0)
 * @endcode
 *
 * @instruction_cnt 0
 *
 * @note For local memory buffers: @n
 *       lmem_buf0, lmem_buf1, ..., lmem_buf15 must be used.
 */
#macro xbuf_free(str_xbuf_name)
    #if (strstr(str_xbuf_name, n$) == 1)
        #error "Next neighbor registers are not supported by xbuf."
    #elif ((!defined(__NFP_INDIRECT_REF_FORMAT_NFPXXXX)) && (strstr(str_xbuf_name, $$) > 0))
        _xbuf_d_free(str_xbuf_name)
    #elif ( streq(str_xbuf_name, 'lmem_buf0') || \
            streq(str_xbuf_name, 'lmem_buf1') || \
            streq(str_xbuf_name, 'lmem_buf2') || \
            streq(str_xbuf_name, 'lmem_buf3') || \
            streq(str_xbuf_name, 'lmem_buf4') || \
            streq(str_xbuf_name, 'lmem_buf5') || \
            streq(str_xbuf_name, 'lmem_buf6') || \
            streq(str_xbuf_name, 'lmem_buf7') || \
            streq(str_xbuf_name, 'lmem_buf8') || \
            streq(str_xbuf_name, 'lmem_buf9') || \
            streq(str_xbuf_name, 'lmem_buf10') || \
            streq(str_xbuf_name, 'lmem_buf11') || \
            streq(str_xbuf_name, 'lmem_buf12') || \
            streq(str_xbuf_name, 'lmem_buf13') || \
            streq(str_xbuf_name, 'lmem_buf14') || \
            streq(str_xbuf_name, 'lmem_buf15') )
        _xbuf_lmem_free(str_xbuf_name)
    #elif (strstr(str_xbuf_name, $) == 1)
        #define_eval _this_xbuf_name (str_xbuf_name)
        #if (streq(_this_xbuf_name, _xbuf1_name))
            #undef _xbuf1_name
            #undef _xbuf1_size
            #ifdef _xbuf1_next
                #undef _xbuf1_next
            #endif
        #elif (streq(_this_xbuf_name, _xbuf2_name))
            #undef _xbuf2_name
            #undef _xbuf2_size
            #ifdef _xbuf2_next
                #undef _xbuf2_next
            #endif
        #elif (streq(_this_xbuf_name, _xbuf3_name))
            #undef _xbuf3_name
            #undef _xbuf3_size
            #ifdef _xbuf3_next
                #undef _xbuf3_next
            #endif
        #elif (streq(_this_xbuf_name, _xbuf4_name))
            #undef _xbuf4_name
            #undef _xbuf4_size
            #ifdef _xbuf4_next
                #undef _xbuf4_next
            #endif
        #elif (defined(__NFP_INDIRECT_REF_FORMAT_NFPXXXX) && streq(_this_xbuf_name, _xbuf5_name))
            #undef _xbuf5_name
            #undef _xbuf5_size
            #ifdef _xbuf5_next
                #undef _xbuf5_next
            #endif
        #elif (defined(__NFP_INDIRECT_REF_FORMAT_NFPXXXX) && streq(_this_xbuf_name, _xbuf6_name))
            #undef _xbuf6_name
            #undef _xbuf6_size
            #ifdef _xbuf6_next
                #undef _xbuf6_next
            #endif
        #elif (defined(__NFP_INDIRECT_REF_FORMAT_NFPXXXX) && streq(_this_xbuf_name, _xbuf7_name))
            #undef _xbuf7_name
            #undef _xbuf7_size
            #ifdef _xbuf7_next
                #undef _xbuf7_next
            #endif
        #elif (defined(__NFP_INDIRECT_REF_FORMAT_NFPXXXX) && streq(_this_xbuf_name, _xbuf8_name))
            #undef _xbuf8_name
            #undef _xbuf8_size
            #ifdef _xbuf8_next
                #undef _xbuf8_next
            #endif
        #else
            #error 2 "xbuf_free: Invalid str_xbuf_name."
        #endif
    #else // GPRs
        _xbuf_gpr_free(str_xbuf_name)
    #endif
#endm


/** Link one transfer register buffer to another.
 *
 * The purpose is to allow processing streams of bytes by continually loading buffers
 * and providing macros that can traverse the buffers.
 *
 * @param str_xbuf_name     Transfer buffer allocated by xbuf_alloc
 * @param str_nextxbuf_name Another transfer buffer allocated by xbuf_alloc.
 *                          Next buffer continues stream of bytes.
 *
 * <b>Example 1:</b>
 * @code
 * xbuf_alloc($pkt, 4)                                // setup xfer regs $pkt0 thru $pkt3
 * // load the xfer buf $pkt, use it
 * xbuf_alloc($morepkt, 4)                            // setup xfer regs $x0 thru $x7
 * xbuf_link($pkt, $morepkt)                          // logical link pkt to morepkt
 * // load the second xfer buf $morepkt
 * ip_verify(exception, $pkt, ip_header_byte_position)// verify ip header that spans the buffers
 * xbuf_free($pkt)
 * xbuf_free($morepkt)
 * @endcode
 *
 * <b>Example 2:</b> [Using Localmem]
 * @code
 * xbuf_alloc(lmem_buf0, 8, READ_WRITE)
 * xbuf_bind_address(lmem_buf0, 0x100, 0x20, 0x0)
 * xbuf_activate(lmem_buf0, 0, 0, 1)
 *
 * xbuf_alloc(lmem_buf1, 8, READ_WRITE)
 * xbuf_bind_address(lmem_buf1, 0x100, 0x20, 32)
 * xbuf_activate(lmem_buf1, 1, 0, 1)
 * xbuf_link(lmem_buf0, lmem_buf1)
 * ipv4_cksum_verify(lmem_buf0, 0xb)
 * ...
 * ...
 * xbuf_deactivate(lmem_buf0)
 * xbuf_free(lmem_buf0)
 * @endcode
 *
 * @instruction_cnt 0
 *
 * @note For local memory buffers: @n
 *       lmem_buf0, lmem_buf1, ..., lmem_buf15 must be used.
 */
#macro xbuf_link(str_xbuf_name, str_nextxbuf_name)
    #if (strstr(str_xbuf_name, n$) == 1)
        #error "Next neighbor registers are not supported by xbuf."
    #elif ((!defined(__NFP_INDIRECT_REF_FORMAT_NFPXXXX)) && (strstr(str_xbuf_name, $$) > 0))
        _xbuf_d_link(str_xbuf_name, str_nextxbuf_name)
    #elif ( streq(str_xbuf_name, 'lmem_buf0') || \
            streq(str_xbuf_name, 'lmem_buf1') || \
            streq(str_xbuf_name, 'lmem_buf2') || \
            streq(str_xbuf_name, 'lmem_buf3') || \
            streq(str_xbuf_name, 'lmem_buf4') || \
            streq(str_xbuf_name, 'lmem_buf5') || \
            streq(str_xbuf_name, 'lmem_buf6') || \
            streq(str_xbuf_name, 'lmem_buf7') || \
            streq(str_xbuf_name, 'lmem_buf8') || \
            streq(str_xbuf_name, 'lmem_buf9') || \
            streq(str_xbuf_name, 'lmem_buf10') || \
            streq(str_xbuf_name, 'lmem_buf11') || \
            streq(str_xbuf_name, 'lmem_buf12') || \
            streq(str_xbuf_name, 'lmem_buf13') || \
            streq(str_xbuf_name, 'lmem_buf14') || \
            streq(str_xbuf_name, 'lmem_buf15') )
        _xbuf_lmem_link(str_xbuf_name, str_nextxbuf_name)
    #elif (strstr(str_xbuf_name, $) == 1)
        #define_eval _this_xbuf_name (str_xbuf_name)

        #if (streq(_this_xbuf_name, _xbuf1_name))
            #define_eval _xbuf1_next  (str_nextxbuf_name)
        #elif (streq(_this_xbuf_name, _xbuf2_name))
            #define_eval _xbuf2_next  (str_nextxbuf_name)
        #elif (streq(_this_xbuf_name, _xbuf3_name))
            #define_eval _xbuf3_next  (str_nextxbuf_name)
        #elif (streq(_this_xbuf_name, _xbuf4_name))
            #define_eval _xbuf4_next   (str_nextxbuf_name)
        #elif (defined(__NFP_INDIRECT_REF_FORMAT_NFPXXXX) && streq(_this_xbuf_name, _xbuf5_name))
            #define_eval _xbuf5_next   (str_nextxbuf_name)
        #elif (defined(__NFP_INDIRECT_REF_FORMAT_NFPXXXX) && streq(_this_xbuf_name, _xbuf6_name))
            #define_eval _xbuf6_next   (str_nextxbuf_name)
        #elif (defined(__NFP_INDIRECT_REF_FORMAT_NFPXXXX) && streq(_this_xbuf_name, _xbuf7_name))
            #define_eval _xbuf7_next   (str_nextxbuf_name)
        #elif (defined(__NFP_INDIRECT_REF_FORMAT_NFPXXXX) && streq(_this_xbuf_name, _xbuf8_name))
            #define_eval _xbuf8_next   (str_nextxbuf_name)
        #else
            #error 2 "xbuf_link: Invalid str_xbuf_name."
        #endif

        // check if str_nextxbuf_name is valid
        #define_eval _next_xbuf_name    str_nextxbuf_name

        #if (streq(_next_xbuf_name, _xbuf1_name) || streq(_next_xbuf_name, _xbuf2_name) || \
            streq(_next_xbuf_name, _xbuf3_name) || streq(_next_xbuf_name, _xbuf4_name) || \
                (defined(__NFP_INDIRECT_REF_FORMAT_NFPXXXX) && ( \
                    streq(_next_xbuf_name, _xbuf5_name) || \
                    streq(_next_xbuf_name, _xbuf6_name) || \
                    streq(_next_xbuf_name, _xbuf7_name) || \
                    streq(_next_xbuf_name, _xbuf8_name) \
                )) \
            )
        #else
            #error 2 "xbuf_link: Invalid str_nextxbuf_name."
        #endif
    #else // GPRs
        _xbuf_gpr_link(str_xbuf_name, str_nextxbuf_name)
    #endif
#endm


/** Given a transfer register name, removes special characters and array notation
 * and then checks if the XBUF token name is an allocated XBUF.
 *
 * This macro makes this XBUF the current XBUF.
 *
 * @param xfer_name  Transfer buffer allocated by xbuf_alloc
 *
 * <b>Example:</b>
 * @code
 * xbuf_alloc($pkt, 4)                           // setup xfer regs $pkt0 thru $pkt3
 * // load the xfer buf $pkt, use it
 * xbuf_find($pkt[2])
 * // load the second xfer buf $morepkt
 * xbuf_free($pkt)
 * @endcode
 *
 * @instruction_cnt 0
 *
 * @note For local memory buffers: @n
 *       lmem_buf0, lmem_buf1, ..., lmem_buf15 must be used.
 */
#macro xbuf_find(xfer_name)
    #ifdef _xbuf_found
        #undef _xbuf_found
    #endif

    // the xfer reg name could be $name[0] - $name[15] or $$name[0] - $$name[15]
    // or the xfer reg name could be $name0 - $name15 or $$name0 - $$name15
    _DEF_NAME_FROM_ARRAY_TOK(_NAME, xfer_name)
    _DEF_INDEX_FROM_ARRAY_TOK(_INDEX, xfer_name)
    #define_eval _xfer_len (strlen(xfer_name))
    #define_eval _this_xbuf_name (_NAME)
    #define_eval _cur_xbuf_index (_INDEX)
    #if (streq(_this_xbuf_name, _xbuf1_name))
        #define  _xbuf_found
        xbuf_param_set(_this_xbuf_name)
    #elif (streq(_this_xbuf_name, _xbuf2_name))
        #define  _xbuf_found
        xbuf_param_set(_this_xbuf_name)
    #elif (streq(_this_xbuf_name, _xbuf3_name))
        #define  _xbuf_found
        xbuf_param_set(_this_xbuf_name)
    #elif (streq(_this_xbuf_name, _xbuf4_name))
        #define  _xbuf_found
        xbuf_param_set(_this_xbuf_name)
    #elif (defined(__NFP_INDIRECT_REF_FORMAT_NFPXXXX) && streq(_this_xbuf_name, _xbuf5_name))
        #define  _xbuf_found
        xbuf_param_set(_this_xbuf_name)
    #elif (defined(__NFP_INDIRECT_REF_FORMAT_NFPXXXX) && streq(_this_xbuf_name, _xbuf6_name))
        #define  _xbuf_found
        xbuf_param_set(_this_xbuf_name)
    #elif (defined(__NFP_INDIRECT_REF_FORMAT_NFPXXXX) && streq(_this_xbuf_name, _xbuf7_name))
        #define  _xbuf_found
        xbuf_param_set(_this_xbuf_name)
    #elif (defined(__NFP_INDIRECT_REF_FORMAT_NFPXXXX) && streq(_this_xbuf_name, _xbuf8_name))
        #define  _xbuf_found
        xbuf_param_set(_this_xbuf_name)
    #elif ((!defined(__NFP_INDIRECT_REF_FORMAT_NFPXXXX)) && streq(_this_xbuf_name, _xbuf_d1_name))
        #define  _xbuf_found
        xbuf_param_set(_this_xbuf_name)
    #elif ((!defined(__NFP_INDIRECT_REF_FORMAT_NFPXXXX)) && streq(_this_xbuf_name, _xbuf_d2_name))
        #define  _xbuf_found
        xbuf_param_set(_this_xbuf_name)
    #elif ((!defined(__NFP_INDIRECT_REF_FORMAT_NFPXXXX)) && streq(_this_xbuf_name, _xbuf_d3_name))
        #define  _xbuf_found
        xbuf_param_set(_this_xbuf_name)
    #elif ((!defined(__NFP_INDIRECT_REF_FORMAT_NFPXXXX)) && streq(_this_xbuf_name, _xbuf_d4_name))
        #define  _xbuf_found
        xbuf_param_set(_this_xbuf_name)
    #elif (streq(_this_xbuf_name, _xbuf1_gpr_name))
        #define  _xbuf_found
        _xbuf_gpr_param_set(_this_xbuf_name)
    #elif (streq(_this_xbuf_name, _xbuf2_gpr_name))
        #define  _xbuf_found
        _xbuf_gpr_param_set(_this_xbuf_name)
    #elif (streq(_this_xbuf_name, _xbuf3_gpr_name))
        #define  _xbuf_found
        _xbuf_gpr_param_set(_this_xbuf_name)
    #elif (streq(_this_xbuf_name, _xbuf4_gpr_name))
        #define  _xbuf_found
        _xbuf_gpr_param_set(_this_xbuf_name)
    #elif (streq(_this_xbuf_name, _xbuf1_lmem_name))
        #define  _xbuf_found
        _xbuf_lmem_param_set(_this_xbuf_name)
    #elif (streq(_this_xbuf_name, _xbuf2_lmem_name))
        #define  _xbuf_found
        _xbuf_lmem_param_set(_this_xbuf_name)
    #elif (streq(_this_xbuf_name, _xbuf3_lmem_name))
        #define  _xbuf_found
        _xbuf_lmemr_param_set[_this_xbuf_name]
    #elif (streq(_this_xbuf_name, _xbuf4_lmem_name))
        #define  _xbuf_found
        _xbuf_lmem_param_set(_this_xbuf_name)
    #elif (streq(_this_xbuf_name, _xbuf5_lmem_name))
        #define  _xbuf_found
        _xbuf_lmem_param_set(_this_xbuf_name)
    #elif (streq(_this_xbuf_name, _xbuf6_lmem_name))
        #define  _xbuf_found
        _xbuf_lmem_param_set(_this_xbuf_name)
    #elif (streq(_this_xbuf_name, _xbuf7_lmem_name))
        #define  _xbuf_found
        _xbuf_lmemr_param_set[_this_xbuf_name]
    #elif (streq(_this_xbuf_name, _xbuf8_lmem_name))
        #define  _xbuf_found
        _xbuf_lmem_param_set(_this_xbuf_name)
    #elif (streq(_this_xbuf_name, _xbuf9_lmem_name))
        #define  _xbuf_found
        _xbuf_lmem_param_set(_this_xbuf_name)
    #elif (streq(_this_xbuf_name, _xbuf10_lmem_name))
        #define  _xbuf_found
        _xbuf_lmem_param_set(_this_xbuf_name)
    #elif (streq(_this_xbuf_name, _xbuf11_lmem_name))
        #define  _xbuf_found
        _xbuf_lmem_param_set(_this_xbuf_name)
    #elif (streq(_this_xbuf_name, _xbuf12_lmem_name))
        #define  _xbuf_found
        _xbuf_lmem_param_set(_this_xbuf_name)
    #elif (streq(_this_xbuf_name, _xbuf13_lmem_name))
        #define  _xbuf_found
        _xbuf_lmem_param_set(_this_xbuf_name)
    #elif (streq(_this_xbuf_name, _xbuf14_lmem_name))
        #define  _xbuf_found
        _xbuf_lmem_param_set(_this_xbuf_name)
    #elif (streq(_this_xbuf_name, _xbuf15_lmem_name))
        #define  _xbuf_found
        _xbuf_lmem_param_set(_this_xbuf_name)
    #elif (streq(_this_xbuf_name, _xbuf16_lmem_name))
        #define  _xbuf_found
        _xbuf_lmem_param_set(_this_xbuf_name)
    #endif
#endm


/** Set global internal @p _cur_xbuf_name, @p _cur_xbuf_size and @p _cur_xbuf_next parameters.
 *
 * This macro is used for following a chain of xbufs.
 * The side-effect of this macro is to modify @p _cur_xbuf_name, @p _cur_xbuf_size
 * and @p _cur_xbuf_next.
 *
 * @param str_xbuf_name  Name of sram transfer buffer
 *
 * <b>Example:</b>
 * @code
 * xbuf_alloc(a_reg, 4, read_write)
 * xbuf_alloc(b_reg, 4, read_write)
 *
 * immed32(a_reg[0], 0x01000CCC)
 * immed32(a_reg[1], 0xCCCC0030)
 * immed32(a_reg[2], 0x788D43B7)
 * immed32(a_reg[3], 0x0032AAAA)
 *
 * xbuf_param_set(a_reg)
 * xbuf_xfer_set(_BUF0, a_reg, 0)
 * //_BUF0_REG0 should be equal to 0x01000CCC
 * //_BUF0_REG1 should be equal to 0xCCCC0030
 * //_BUF0_REG2 should be equal to 0x788D43B7
 * //_BUF0_REG3 should be equal to 0x0032AAAA
 * @endcode
 *
 * @instruction_cnt 0
 *
 * @note For local memory buffers: @n
 *       lmem_buf0, lmem_buf1, ..., lmem_buf15 must be used.
 */
#macro xbuf_param_set(str_xbuf_name)
    #if (strstr(str_xbuf_name, n$) == 1)
        #error "Next neighbor registers are not supported by xbuf."
    #elif ((!defined(__NFP_INDIRECT_REF_FORMAT_NFPXXXX)) && (strstr(str_xbuf_name, $$) > 0))
        _xbuf_d_param_set(str_xbuf_name)
    #elif ( streq(str_xbuf_name, 'lmem_buf0') || \
            streq(str_xbuf_name, 'lmem_buf1') || \
            streq(str_xbuf_name, 'lmem_buf2') || \
            streq(str_xbuf_name, 'lmem_buf3') || \
            streq(str_xbuf_name, 'lmem_buf4') || \
            streq(str_xbuf_name, 'lmem_buf5') || \
            streq(str_xbuf_name, 'lmem_buf6') || \
            streq(str_xbuf_name, 'lmem_buf7') || \
            streq(str_xbuf_name, 'lmem_buf8') || \
            streq(str_xbuf_name, 'lmem_buf9') || \
            streq(str_xbuf_name, 'lmem_buf10') || \
            streq(str_xbuf_name, 'lmem_buf11') || \
            streq(str_xbuf_name, 'lmem_buf12') || \
            streq(str_xbuf_name, 'lmem_buf13') || \
            streq(str_xbuf_name, 'lmem_buf14') || \
            streq(str_xbuf_name, 'lmem_buf15') )
        _xbuf_lmem_param_set(str_xbuf_name)     //for local mem
    #elif (strstr(str_xbuf_name, $) == 1)

        #if (strstr(str_xbuf_name, $$) > 0)
            //__NFP_INDIRECT_REF_FORMAT_NFPXXXX implied at this point
            // still define old D type tokens for compatibility
            #ifdef _cur_xbuf_d_name
                #undef _cur_xbuf_d_name
            #endif
            #ifdef _cur_xbuf_d_size
                #undef _cur_xbuf_d_size
            #endif
            #ifdef _cur_xbuf_d_next
                #undef _cur_xbuf_d_next
            #endif
        #else
            //__NFP_INDIRECT_REF_FORMAT_NFPXXXX NOT implied at this point
            #ifdef _cur_xbuf_name
                #undef _cur_xbuf_name
            #endif
            #ifdef _cur_xbuf_size
                #undef _cur_xbuf_size
            #endif
            #ifdef _cur_xbuf_next
                #undef _cur_xbuf_next
            #endif
        #endif

        #define_eval _xbuf_param_set_cur_xbuf_name (str_xbuf_name)

        #ifdef _xbuf_param_set_cur_xbuf_size
            #undef _xbuf_param_set_cur_xbuf_size
        #endif
        #ifdef _xbuf_param_set_cur_xbuf_next
            #undef _xbuf_param_set_cur_xbuf_next
        #endif

        #if (streq(_xbuf_param_set_cur_xbuf_name, _xbuf1_name))
            #define _xbuf_param_set_cur_xbuf_size  _xbuf1_size
            #ifdef _xbuf1_next
                #define _xbuf_param_set_cur_xbuf_next _xbuf1_next
            #endif
        #elif (streq(_xbuf_param_set_cur_xbuf_name, _xbuf2_name))
            #define _xbuf_param_set_cur_xbuf_size  _xbuf2_size
            #ifdef _xbuf2_next
                #define _xbuf_param_set_cur_xbuf_next _xbuf2_next
            #endif
        #elif (streq(_xbuf_param_set_cur_xbuf_name, _xbuf3_name))
            #define _xbuf_param_set_cur_xbuf_size  _xbuf3_size
            #ifdef _xbuf3_next
                #define _xbuf_param_set_cur_xbuf_next _xbuf3_next
            #endif
        #elif (streq(_xbuf_param_set_cur_xbuf_name, _xbuf4_name))
            #define _xbuf_param_set_cur_xbuf_size  _xbuf4_size
            #ifdef _xbuf4_next
                #define _xbuf_param_set_cur_xbuf_next _xbuf4_next
            #endif
        #elif (defined(__NFP_INDIRECT_REF_FORMAT_NFPXXXX) && streq(_xbuf_param_set_cur_xbuf_name, _xbuf5_name))
            #define _xbuf_param_set_cur_xbuf_size  _xbuf5_size
            #ifdef _xbuf5_next
                #define _xbuf_param_set_cur_xbuf_next _xbuf5_next
            #endif
        #elif (defined(__NFP_INDIRECT_REF_FORMAT_NFPXXXX) && streq(_xbuf_param_set_cur_xbuf_name, _xbuf6_name))
            #define _xbuf_param_set_cur_xbuf_size  _xbuf6_size
            #ifdef _xbuf6_next
                #define _xbuf_param_set_cur_xbuf_next _xbuf6_next
            #endif
        #elif (defined(__NFP_INDIRECT_REF_FORMAT_NFPXXXX) && streq(_xbuf_param_set_cur_xbuf_name, _xbuf7_name))
            #define _xbuf_param_set_cur_xbuf_size  _xbuf7_size
            #ifdef _xbuf7_next
                #define _xbuf_param_set_cur_xbuf_next _xbuf7_next
            #endif
        #elif (defined(__NFP_INDIRECT_REF_FORMAT_NFPXXXX) && streq(_xbuf_param_set_cur_xbuf_name, _xbuf8_name))
            #define _xbuf_param_set_cur_xbuf_size  _xbuf8_size
            #ifdef _xbuf8_next
                #define _xbuf_param_set_cur_xbuf_next _xbuf8_next
            #endif
        #else
            #error 4 "xbuf_param_set: no matching str_xbuf_name."
        #endif

        #if (strstr(str_xbuf_name, $$) > 0)
            #ifdef _xbuf_param_set_cur_xbuf_size
                #define_eval _cur_xbuf_d_size _xbuf_param_set_cur_xbuf_size
            #endif
            #ifdef _xbuf_param_set_cur_xbuf_next
                #define_eval _cur_xbuf_d_next _xbuf_param_set_cur_xbuf_next
            #endif
            #define_eval _cur_xbuf_d_name _xbuf_param_set_cur_xbuf_name
        #else
            #ifdef _xbuf_param_set_cur_xbuf_size
                #define_eval _cur_xbuf_size _xbuf_param_set_cur_xbuf_size
            #endif
            #ifdef _xbuf_param_set_cur_xbuf_next
                #define_eval _cur_xbuf_next _xbuf_param_set_cur_xbuf_next
            #endif
            #define_eval _cur_xbuf_name _xbuf_param_set_cur_xbuf_name
        #endif

        #ifdef _xbuf_param_set_cur_xbuf_size
            #undef _xbuf_param_set_cur_xbuf_size
        #endif
        #ifdef _xbuf_param_set_cur_xbuf_next
            #undef _xbuf_param_set_cur_xbuf_next
        #endif
        #ifdef _xbuf_param_set_cur_xbuf_name
            #undef _xbuf_param_set_cur_xbuf_name
        #endif

    #else // GPRs
        _xbuf_gpr_param_set(str_xbuf_name)
    #endif
#endm


/** Define the register token sequence from the xbufs, starting at the longword indicated by BYTE_OFFSET.
 *
 * These tokens can then be used as individual identification of transfer registers.
 * The search for transfers can span multiple linked xbufs.
 * The side-effect of this macro is to modify global accessible tokens _XFER0, _XFER1, ..., _XFER7.
 *
 * @param XFER_TOKEN
 * @code
 * _S_XFER     define tokens _XFER0 through _XFER15/_XFER31
 * _SRD_XFER   define tokens _SRD_XFER0 through _SRD_XFER15/_SRD_XFER31
 * _SWR_XFER   define tokens _SWR_XFER0 through _SWR_XFER15/_SWR_XFER31
 * _D_XFER     define tokens _D_XFER0 through _D_XFER15
               (only in IXP indirect reference format mode)
 * _DRD_XFER   define tokens _DRD_XFER0 through _DRD_XFER15
               (only in IXP indirect reference format mode)
 * _DWR_XFER   define tokens _DWR_XFER0 through _DWR_XFER15
               (only in IXP indirect reference format mode)
 * _BUF0       define tokens _BUF0_REG0 through _BUF0_REG7
 * _BUF1       define tokens _BUF1_REG0 through _BUF1_REG7
 * _BUF2       define tokens _BUF2_REG0 through _BUF2_REG7
 * _BUF3       define tokens _BUF3_REG0 through _BUF3_REG7
 * @endcode
 * @param str_xbuf_name  Name of first sram transfer buffer
 * @param BYTE_OFFSET    Byte offset where _XFER0 is
 *
 * <b>Example:</b>
 * @code
 * xbuf_alloc(a_reg, 4, read_write)
 * xbuf_alloc(b_reg, 4, read_write)
 *
 * immed32(a_reg[0], 0x01000CCC)
 * immed32(a_reg[1], 0xCCCC0030)
 * immed32(a_reg[2], 0x788D43B7)
 * immed32(a_reg[3], 0x0032AAAA)
 *
 * xbuf_param_set(a_reg)
 * xbuf_xfer_set(_BUF0, a_reg, 0)
 * //_BUF0_REG0 should be equal to 0x01000CCC
 * //_BUF0_REG1 should be equal to 0xCCCC0030
 * //_BUF0_REG2 should be equal to 0x788D43B7
 * //_BUF0_REG3 should be equal to 0x0032AAAA
 * @endcode
 *
 * @instruction_cnt 0
 */
#macro xbuf_xfer_set(XFER_TOKEN, str_xbuf_name, BYTE_OFFSET)
    #define_eval _LWORD_OFFSET (BYTE_OFFSET / 4)
    _DEFINE_THIS_XBUF(str_xbuf_name)

    _GET_XBUF_TYPE(str_xbuf_name)
    #ifdef _XBUF_XFER_SET_STR_XBUF_TYPE
        #warning "_XBUF_XFER_SET_STR_XBUF_TYPE is being redefined."
    #endif
    #define_eval _XBUF_XFER_SET_STR_XBUF_TYPE _XBUF_TYPE
    #undef _XBUF_TYPE

    #define_eval _LWORD_IN_BUF (0)
    #define_eval _LWORDS_CHECKED (0)

    // find first xfer reg
    #while (_LWORDS_CHECKED < _LWORD_OFFSET)
        #define_eval _LWORD_IN_BUF (_LWORD_IN_BUF + 1)
        #define_eval _LWORDS_CHECKED (_LWORDS_CHECKED + 1)
        #if (_LWORD_IN_BUF == _this_xbuf_size)
            #ifdef _this_xbuf_next
                #define_eval _next  _this_xbuf_next
                _UNDEFINE_THIS_XBUF()
                _DEFINE_THIS_XBUF(_next)
                #define_eval _LWORD_IN_BUF 0
            #else
                #error 2 "_xbuf_xfer_set: BYTE_OFFSET beyond buffer range"
            #endif
        #endif
    #endloop

    #define_eval _XFER_TEMP0 _this_xbuf_name[/**/_LWORD_IN_BUF]
    #define_eval _LWORD_IN_BUF (_LWORD_IN_BUF + 1)

    #if(_LWORD_IN_BUF < _this_xbuf_size)
        #define_eval _XFER_TEMP1 _this_xbuf_name[/**/_LWORD_IN_BUF]
        #define_eval _LWORD_IN_BUF (_LWORD_IN_BUF + 1)
    #else
        #ifdef _this_xbuf_next
            #define_eval _next  _this_xbuf_next
            _UNDEFINE_THIS_XBUF()
            _DEFINE_THIS_XBUF(_next)
            #define_eval _LWORD_IN_BUF (0)
            #define_eval _XFER_TEMP1 _this_xbuf_name[/**/_LWORD_IN_BUF]
            #define_eval _LWORD_IN_BUF (1)
        #endif
    #endif

    #if(_LWORD_IN_BUF < _this_xbuf_size)
        #define_eval _XFER_TEMP2 _this_xbuf_name[/**/_LWORD_IN_BUF]
        #define_eval _LWORD_IN_BUF (_LWORD_IN_BUF + 1)
    #else
        #ifdef _this_xbuf_next
            #define_eval _next  _this_xbuf_next
            _UNDEFINE_THIS_XBUF()
            _DEFINE_THIS_XBUF(_next)
            #define_eval _LWORD_IN_BUF (0)
            #define_eval _XFER_TEMP2 _this_xbuf_name[/**/_LWORD_IN_BUF]
            #define_eval _LWORD_IN_BUF (1)
        #endif
    #endif

    #if(_LWORD_IN_BUF < _this_xbuf_size)
        #define_eval _XFER_TEMP3 _this_xbuf_name[/**/_LWORD_IN_BUF]
        #define_eval _LWORD_IN_BUF (_LWORD_IN_BUF + 1)
    #else
        #ifdef _this_xbuf_next
            #define_eval _next  _this_xbuf_next
            _UNDEFINE_THIS_XBUF()
            _DEFINE_THIS_XBUF(_next)
            #define_eval _LWORD_IN_BUF (0)
            #define_eval _XFER_TEMP3 _this_xbuf_name[/**/_LWORD_IN_BUF]
            #define_eval _LWORD_IN_BUF (1)
        #endif
    #endif

    #if(_LWORD_IN_BUF < _this_xbuf_size)
        #define_eval _XFER_TEMP4 _this_xbuf_name[/**/_LWORD_IN_BUF]
        #define_eval _LWORD_IN_BUF (_LWORD_IN_BUF + 1)
    #else
        #ifdef _this_xbuf_next
            #define_eval _next  _this_xbuf_next
            _UNDEFINE_THIS_XBUF()
            _DEFINE_THIS_XBUF(_next)
            #define_eval _LWORD_IN_BUF (0)
            #define_eval _XFER_TEMP4 _this_xbuf_name[/**/_LWORD_IN_BUF]
            #define_eval _LWORD_IN_BUF (1)
        #endif
    #endif
    #if(_LWORD_IN_BUF < _this_xbuf_size)
        #define_eval _XFER_TEMP5 _this_xbuf_name[/**/_LWORD_IN_BUF]
        #define_eval _LWORD_IN_BUF (_LWORD_IN_BUF + 1)
    #else
        #ifdef _this_xbuf_next
            #define_eval _next  _this_xbuf_next
            _UNDEFINE_THIS_XBUF()
            _DEFINE_THIS_XBUF(_next)
            #define_eval _LWORD_IN_BUF (0)
            #define_eval _XFER_TEMP5 _this_xbuf_name[/**/_LWORD_IN_BUF]
            #define_eval _LWORD_IN_BUF (1)
        #endif
    #endif

    #if(_LWORD_IN_BUF < _this_xbuf_size)
        #define_eval _XFER_TEMP6 _this_xbuf_name[/**/_LWORD_IN_BUF]
        #define_eval _LWORD_IN_BUF (_LWORD_IN_BUF + 1)
    #else
        #ifdef _this_xbuf_next
            #define_eval _next  _this_xbuf_next
            _UNDEFINE_THIS_XBUF()
            _DEFINE_THIS_XBUF(_next)
            #define_eval _LWORD_IN_BUF (0)
            #define_eval _XFER_TEMP6 _this_xbuf_name[/**/_LWORD_IN_BUF]
            #define_eval _LWORD_IN_BUF (1)
        #endif
    #endif

    #if(_LWORD_IN_BUF < _this_xbuf_size)
        #define_eval _XFER_TEMP7 _this_xbuf_name[/**/_LWORD_IN_BUF]
        #define_eval _LWORD_IN_BUF (_LWORD_IN_BUF + 1)
    #else
        #ifdef _this_xbuf_next
            #define_eval _next  _this_xbuf_next
            _UNDEFINE_THIS_XBUF()
            _DEFINE_THIS_XBUF(_next)
            #define_eval _LWORD_IN_BUF (0)
            #define_eval _XFER_TEMP7 _this_xbuf_name[/**/_LWORD_IN_BUF]
            #define_eval _LWORD_IN_BUF (1)
        #endif
    #endif

    #if(_LWORD_IN_BUF < _this_xbuf_size)
        #define_eval _XFER_TEMP8 _this_xbuf_name[/**/_LWORD_IN_BUF]
        #define_eval _LWORD_IN_BUF (_LWORD_IN_BUF + 1)
    #else
        #ifdef _this_xbuf_next
            #define_eval _next  _this_xbuf_next
            _UNDEFINE_THIS_XBUF()
            _DEFINE_THIS_XBUF(_next)
            #define_eval _LWORD_IN_BUF (0)
            #define_eval _XFER_TEMP8 _this_xbuf_name[/**/_LWORD_IN_BUF]
            #define_eval _LWORD_IN_BUF (1)
        #endif
    #endif

    #if(_LWORD_IN_BUF < _this_xbuf_size)
        #define_eval _XFER_TEMP9 _this_xbuf_name[/**/_LWORD_IN_BUF]
        #define_eval _LWORD_IN_BUF (_LWORD_IN_BUF + 1)
    #else
        #ifdef _this_xbuf_next
            #define_eval _next  _this_xbuf_next
            _UNDEFINE_THIS_XBUF()
            _DEFINE_THIS_XBUF(_next)
            #define_eval _LWORD_IN_BUF (0)
            #define_eval _XFER_TEMP9 _this_xbuf_name[/**/_LWORD_IN_BUF]
            #define_eval _LWORD_IN_BUF (1)
        #endif
    #endif

    #if(_LWORD_IN_BUF < _this_xbuf_size)
        #define_eval _XFER_TEMP10 _this_xbuf_name[/**/_LWORD_IN_BUF]
        #define_eval _LWORD_IN_BUF (_LWORD_IN_BUF + 1)
    #else
        #ifdef _this_xbuf_next
            #define_eval _next  _this_xbuf_next
            _UNDEFINE_THIS_XBUF()
            _DEFINE_THIS_XBUF(_next)
            #define_eval _LWORD_IN_BUF (0)
            #define_eval _XFER_TEMP10 _this_xbuf_name[/**/_LWORD_IN_BUF]
            #define_eval _LWORD_IN_BUF (1)
        #endif
    #endif

    #if(_LWORD_IN_BUF < _this_xbuf_size)
        #define_eval _XFER_TEMP11 _this_xbuf_name[/**/_LWORD_IN_BUF]
        #define_eval _LWORD_IN_BUF (_LWORD_IN_BUF + 1)
    #else
        #ifdef _this_xbuf_next
            #define_eval _next  _this_xbuf_next
            _UNDEFINE_THIS_XBUF()
            _DEFINE_THIS_XBUF(_next)
            #define_eval _LWORD_IN_BUF (0)
            #define_eval _XFER_TEMP11 _this_xbuf_name[/**/_LWORD_IN_BUF]
            #define_eval _LWORD_IN_BUF (1)
        #endif
    #endif

    #if(_LWORD_IN_BUF < _this_xbuf_size)
        #define_eval _XFER_TEMP12 _this_xbuf_name[/**/_LWORD_IN_BUF]
        #define_eval _LWORD_IN_BUF (_LWORD_IN_BUF + 1)
    #else
        #ifdef _this_xbuf_next
            #define_eval _next  _this_xbuf_next
            _UNDEFINE_THIS_XBUF()
            _DEFINE_THIS_XBUF(_next)
            #define_eval _LWORD_IN_BUF (0)
            #define_eval _XFER_TEMP12 _this_xbuf_name[/**/_LWORD_IN_BUF]
            #define_eval _LWORD_IN_BUF (1)
        #endif
    #endif

    #if(_LWORD_IN_BUF < _this_xbuf_size)
        #define_eval _XFER_TEMP13 _this_xbuf_name[/**/_LWORD_IN_BUF]
        #define_eval _LWORD_IN_BUF (_LWORD_IN_BUF + 1)
    #else
        #ifdef _this_xbuf_next
            #define_eval _next  _this_xbuf_next
            _UNDEFINE_THIS_XBUF()
            _DEFINE_THIS_XBUF(_next)
            #define_eval _LWORD_IN_BUF (0)
            #define_eval _XFER_TEMP13 _this_xbuf_name[/**/_LWORD_IN_BUF]
            #define_eval _LWORD_IN_BUF (1)
        #endif
    #endif

    #if(_LWORD_IN_BUF < _this_xbuf_size)
        #define_eval _XFER_TEMP14 _this_xbuf_name[/**/_LWORD_IN_BUF]
        #define_eval _LWORD_IN_BUF (_LWORD_IN_BUF + 1)
    #else
        #ifdef _this_xbuf_next
            #define_eval _next  _this_xbuf_next
            _UNDEFINE_THIS_XBUF()
            _DEFINE_THIS_XBUF(_next)
            #define_eval _LWORD_IN_BUF (0)
            #define_eval _XFER_TEMP14 _this_xbuf_name[/**/_LWORD_IN_BUF]
            #define_eval _LWORD_IN_BUF (1)
        #endif
    #endif

    #if(_LWORD_IN_BUF < _this_xbuf_size)
        #define_eval _XFER_TEMP15 _this_xbuf_name[/**/_LWORD_IN_BUF]
        #define_eval _LWORD_IN_BUF (_LWORD_IN_BUF + 1)
    #else
        #ifdef _this_xbuf_next
            #define_eval _next  _this_xbuf_next
            _UNDEFINE_THIS_XBUF()
            _DEFINE_THIS_XBUF(_next)
            #define_eval _LWORD_IN_BUF (0)
            #define_eval _XFER_TEMP15 _this_xbuf_name[/**/_LWORD_IN_BUF]
            #define_eval _LWORD_IN_BUF (1)
        #endif
    #endif

    #if (defined(__NFP_INDIRECT_REF_FORMAT_NFPXXXX) && (_XBUF_XFER_SET_STR_XBUF_TYPE == _XBUF_SRAM_XFER))
        #if(_LWORD_IN_BUF < _this_xbuf_size)
            #define_eval _XFER_TEMP16 _this_xbuf_name[/**/_LWORD_IN_BUF]
            #define_eval _LWORD_IN_BUF (_LWORD_IN_BUF + 1)
        #else
            #ifdef _this_xbuf_next
                #define_eval _next  _this_xbuf_next
                _UNDEFINE_THIS_XBUF()
                _DEFINE_THIS_XBUF(_next)
                #define_eval _LWORD_IN_BUF (0)
                #define_eval _XFER_TEMP16 _this_xbuf_name[/**/_LWORD_IN_BUF]
                #define_eval _LWORD_IN_BUF (1)
            #endif
        #endif

        #if(_LWORD_IN_BUF < _this_xbuf_size)
            #define_eval _XFER_TEMP17 _this_xbuf_name[/**/_LWORD_IN_BUF]
            #define_eval _LWORD_IN_BUF (_LWORD_IN_BUF + 1)
        #else
            #ifdef _this_xbuf_next
                #define_eval _next  _this_xbuf_next
                _UNDEFINE_THIS_XBUF()
                _DEFINE_THIS_XBUF(_next)
                #define_eval _LWORD_IN_BUF (0)
                #define_eval _XFER_TEMP17 _this_xbuf_name[/**/_LWORD_IN_BUF]
                #define_eval _LWORD_IN_BUF (1)
            #endif
        #endif

        #if(_LWORD_IN_BUF < _this_xbuf_size)
            #define_eval _XFER_TEMP18 _this_xbuf_name[/**/_LWORD_IN_BUF]
            #define_eval _LWORD_IN_BUF (_LWORD_IN_BUF + 1)
        #else
            #ifdef _this_xbuf_next
                #define_eval _next  _this_xbuf_next
                _UNDEFINE_THIS_XBUF()
                _DEFINE_THIS_XBUF(_next)
                #define_eval _LWORD_IN_BUF (0)
                #define_eval _XFER_TEMP18 _this_xbuf_name[/**/_LWORD_IN_BUF]
                #define_eval _LWORD_IN_BUF (1)
            #endif
        #endif

        #if(_LWORD_IN_BUF < _this_xbuf_size)
            #define_eval _XFER_TEMP19 _this_xbuf_name[/**/_LWORD_IN_BUF]
            #define_eval _LWORD_IN_BUF (_LWORD_IN_BUF + 1)
        #else
            #ifdef _this_xbuf_next
                #define_eval _next  _this_xbuf_next
                _UNDEFINE_THIS_XBUF()
                _DEFINE_THIS_XBUF(_next)
                #define_eval _LWORD_IN_BUF (0)
                #define_eval _XFER_TEMP19 _this_xbuf_name[/**/_LWORD_IN_BUF]
                #define_eval _LWORD_IN_BUF (1)
            #endif
        #endif

        #if(_LWORD_IN_BUF < _this_xbuf_size)
            #define_eval _XFER_TEMP20 _this_xbuf_name[/**/_LWORD_IN_BUF]
            #define_eval _LWORD_IN_BUF (_LWORD_IN_BUF + 1)
        #else
            #ifdef _this_xbuf_next
                #define_eval _next  _this_xbuf_next
                _UNDEFINE_THIS_XBUF()
                _DEFINE_THIS_XBUF(_next)
                #define_eval _LWORD_IN_BUF (0)
                #define_eval _XFER_TEMP20 _this_xbuf_name[/**/_LWORD_IN_BUF]
                #define_eval _LWORD_IN_BUF (1)
            #endif
        #endif

        #if(_LWORD_IN_BUF < _this_xbuf_size)
            #define_eval _XFER_TEMP21 _this_xbuf_name[/**/_LWORD_IN_BUF]
            #define_eval _LWORD_IN_BUF (_LWORD_IN_BUF + 1)
        #else
            #ifdef _this_xbuf_next
                #define_eval _next  _this_xbuf_next
                _UNDEFINE_THIS_XBUF()
                _DEFINE_THIS_XBUF(_next)
                #define_eval _LWORD_IN_BUF (0)
                #define_eval _XFER_TEMP21 _this_xbuf_name[/**/_LWORD_IN_BUF]
                #define_eval _LWORD_IN_BUF (1)
            #endif
        #endif

        #if(_LWORD_IN_BUF < _this_xbuf_size)
            #define_eval _XFER_TEMP22 _this_xbuf_name[/**/_LWORD_IN_BUF]
            #define_eval _LWORD_IN_BUF (_LWORD_IN_BUF + 1)
        #else
            #ifdef _this_xbuf_next
                #define_eval _next  _this_xbuf_next
                _UNDEFINE_THIS_XBUF()
                _DEFINE_THIS_XBUF(_next)
                #define_eval _LWORD_IN_BUF (0)
                #define_eval _XFER_TEMP22 _this_xbuf_name[/**/_LWORD_IN_BUF]
                #define_eval _LWORD_IN_BUF (1)
            #endif
        #endif

        #if(_LWORD_IN_BUF < _this_xbuf_size)
            #define_eval _XFER_TEMP23 _this_xbuf_name[/**/_LWORD_IN_BUF]
            #define_eval _LWORD_IN_BUF (_LWORD_IN_BUF + 1)
        #else
            #ifdef _this_xbuf_next
                #define_eval _next  _this_xbuf_next
                _UNDEFINE_THIS_XBUF()
                _DEFINE_THIS_XBUF(_next)
                #define_eval _LWORD_IN_BUF (0)
                #define_eval _XFER_TEMP23 _this_xbuf_name[/**/_LWORD_IN_BUF]
                #define_eval _LWORD_IN_BUF (1)
            #endif
        #endif

        #if(_LWORD_IN_BUF < _this_xbuf_size)
            #define_eval _XFER_TEMP24 _this_xbuf_name[/**/_LWORD_IN_BUF]
            #define_eval _LWORD_IN_BUF (_LWORD_IN_BUF + 1)
        #else
            #ifdef _this_xbuf_next
                #define_eval _next  _this_xbuf_next
                _UNDEFINE_THIS_XBUF()
                _DEFINE_THIS_XBUF(_next)
                #define_eval _LWORD_IN_BUF (0)
                #define_eval _XFER_TEMP24 _this_xbuf_name[/**/_LWORD_IN_BUF]
                #define_eval _LWORD_IN_BUF (1)
            #endif
        #endif

        #if(_LWORD_IN_BUF < _this_xbuf_size)
            #define_eval _XFER_TEMP25 _this_xbuf_name[/**/_LWORD_IN_BUF]
            #define_eval _LWORD_IN_BUF (_LWORD_IN_BUF + 1)
        #else
            #ifdef _this_xbuf_next
                #define_eval _next  _this_xbuf_next
                _UNDEFINE_THIS_XBUF()
                _DEFINE_THIS_XBUF(_next)
                #define_eval _LWORD_IN_BUF (0)
                #define_eval _XFER_TEMP25 _this_xbuf_name[/**/_LWORD_IN_BUF]
                #define_eval _LWORD_IN_BUF (1)
            #endif
        #endif

        #if(_LWORD_IN_BUF < _this_xbuf_size)
            #define_eval _XFER_TEMP26 _this_xbuf_name[/**/_LWORD_IN_BUF]
            #define_eval _LWORD_IN_BUF (_LWORD_IN_BUF + 1)
        #else
            #ifdef _this_xbuf_next
                #define_eval _next  _this_xbuf_next
                _UNDEFINE_THIS_XBUF()
                _DEFINE_THIS_XBUF(_next)
                #define_eval _LWORD_IN_BUF (0)
                #define_eval _XFER_TEMP26 _this_xbuf_name[/**/_LWORD_IN_BUF]
                #define_eval _LWORD_IN_BUF (1)
            #endif
        #endif

        #if(_LWORD_IN_BUF < _this_xbuf_size)
            #define_eval _XFER_TEMP27 _this_xbuf_name[/**/_LWORD_IN_BUF]
            #define_eval _LWORD_IN_BUF (_LWORD_IN_BUF + 1)
        #else
            #ifdef _this_xbuf_next
                #define_eval _next  _this_xbuf_next
                _UNDEFINE_THIS_XBUF()
                _DEFINE_THIS_XBUF(_next)
                #define_eval _LWORD_IN_BUF (0)
                #define_eval _XFER_TEMP27 _this_xbuf_name[/**/_LWORD_IN_BUF]
                #define_eval _LWORD_IN_BUF (1)
            #endif
        #endif

        #if(_LWORD_IN_BUF < _this_xbuf_size)
            #define_eval _XFER_TEMP28 _this_xbuf_name[/**/_LWORD_IN_BUF]
            #define_eval _LWORD_IN_BUF (_LWORD_IN_BUF + 1)
        #else
            #ifdef _this_xbuf_next
                #define_eval _next  _this_xbuf_next
                _UNDEFINE_THIS_XBUF()
                _DEFINE_THIS_XBUF(_next)
                #define_eval _LWORD_IN_BUF (0)
                #define_eval _XFER_TEMP28 _this_xbuf_name[/**/_LWORD_IN_BUF]
                #define_eval _LWORD_IN_BUF (1)
            #endif
        #endif

        #if(_LWORD_IN_BUF < _this_xbuf_size)
            #define_eval _XFER_TEMP29 _this_xbuf_name[/**/_LWORD_IN_BUF]
            #define_eval _LWORD_IN_BUF (_LWORD_IN_BUF + 1)
        #else
            #ifdef _this_xbuf_next
                #define_eval _next  _this_xbuf_next
                _UNDEFINE_THIS_XBUF()
                _DEFINE_THIS_XBUF(_next)
                #define_eval _LWORD_IN_BUF (0)
                #define_eval _XFER_TEMP29 _this_xbuf_name[/**/_LWORD_IN_BUF]
                #define_eval _LWORD_IN_BUF (1)
            #endif
        #endif

        #if(_LWORD_IN_BUF < _this_xbuf_size)
            #define_eval _XFER_TEMP30 _this_xbuf_name[/**/_LWORD_IN_BUF]
            #define_eval _LWORD_IN_BUF (_LWORD_IN_BUF + 1)
        #else
            #ifdef _this_xbuf_next
                #define_eval _next  _this_xbuf_next
                _UNDEFINE_THIS_XBUF()
                _DEFINE_THIS_XBUF(_next)
                #define_eval _LWORD_IN_BUF (0)
                #define_eval _XFER_TEMP30 _this_xbuf_name[/**/_LWORD_IN_BUF]
                #define_eval _LWORD_IN_BUF (1)
            #endif
        #endif

        #if(_LWORD_IN_BUF < _this_xbuf_size)
            #define_eval _XFER_TEMP31 _this_xbuf_name[/**/_LWORD_IN_BUF]
            #define_eval _LWORD_IN_BUF (_LWORD_IN_BUF + 1)
        #else
            #ifdef _this_xbuf_next
                #define_eval _next  _this_xbuf_next
                _UNDEFINE_THIS_XBUF()
                _DEFINE_THIS_XBUF(_next)
                #define_eval _LWORD_IN_BUF (0)
                #define_eval _XFER_TEMP31 _this_xbuf_name[/**/_LWORD_IN_BUF]
                #define_eval _LWORD_IN_BUF (1)
            #endif
        #endif
    #endif // (defined(__NFP_INDIRECT_REF_FORMAT_NFPXXXX))

    _UNDEFINE_THIS_XBUF()
    #ifdef _next
        #undef _next
    #endif

    #if (streq(XFER_TOKEN, _XFER))
        #define_eval _XFER0 _XFER_TEMP0
        #define_eval _XFER1 _XFER_TEMP1
        #define_eval _XFER2 _XFER_TEMP2
        #define_eval _XFER3 _XFER_TEMP3
        #define_eval _XFER4 _XFER_TEMP4
        #define_eval _XFER5 _XFER_TEMP5
        #define_eval _XFER6 _XFER_TEMP6
        #define_eval _XFER7 _XFER_TEMP7
        #define_eval _XFER8 _XFER_TEMP8
        #define_eval _XFER9 _XFER_TEMP9
        #define_eval _XFER10 _XFER_TEMP10
        #define_eval _XFER11 _XFER_TEMP11
        #define_eval _XFER12 _XFER_TEMP12
        #define_eval _XFER13 _XFER_TEMP13
        #define_eval _XFER14 _XFER_TEMP14
        #define_eval _XFER15 _XFER_TEMP15
        #if (defined(__NFP_INDIRECT_REF_FORMAT_NFPXXXX))
            #define_eval _XFER16 _XFER_TEMP16
            #define_eval _XFER17 _XFER_TEMP17
            #define_eval _XFER18 _XFER_TEMP18
            #define_eval _XFER19 _XFER_TEMP19
            #define_eval _XFER20 _XFER_TEMP20
            #define_eval _XFER21 _XFER_TEMP21
            #define_eval _XFER22 _XFER_TEMP22
            #define_eval _XFER23 _XFER_TEMP23
            #define_eval _XFER24 _XFER_TEMP24
            #define_eval _XFER25 _XFER_TEMP25
            #define_eval _XFER26 _XFER_TEMP26
            #define_eval _XFER27 _XFER_TEMP27
            #define_eval _XFER28 _XFER_TEMP28
            #define_eval _XFER29 _XFER_TEMP29
            #define_eval _XFER30 _XFER_TEMP30
            #define_eval _XFER31 _XFER_TEMP31
        #endif
    #elif (streq(XFER_TOKEN, _S_XFER))
        #define_eval _S_XFER0 _XFER_TEMP0
        #define_eval _S_XFER1 _XFER_TEMP1
        #define_eval _S_XFER2 _XFER_TEMP2
        #define_eval _S_XFER3 _XFER_TEMP3
        #define_eval _S_XFER4 _XFER_TEMP4
        #define_eval _S_XFER5 _XFER_TEMP5
        #define_eval _S_XFER6 _XFER_TEMP6
        #define_eval _S_XFER7 _XFER_TEMP7
        #define_eval _S_XFER8 _XFER_TEMP8
        #define_eval _S_XFER9 _XFER_TEMP9
        #define_eval _S_XFER10 _XFER_TEMP10
        #define_eval _S_XFER11 _XFER_TEMP11
        #define_eval _S_XFER12 _XFER_TEMP12
        #define_eval _S_XFER13 _XFER_TEMP13
        #define_eval _S_XFER14 _XFER_TEMP14
        #define_eval _S_XFER15 _XFER_TEMP15
        #if (defined(__NFP_INDIRECT_REF_FORMAT_NFPXXXX))
            #define_eval _S_XFER16 _XFER_TEMP16
            #define_eval _S_XFER17 _XFER_TEMP17
            #define_eval _S_XFER18 _XFER_TEMP18
            #define_eval _S_XFER19 _XFER_TEMP19
            #define_eval _S_XFER20 _XFER_TEMP20
            #define_eval _S_XFER21 _XFER_TEMP21
            #define_eval _S_XFER22 _XFER_TEMP22
            #define_eval _S_XFER23 _XFER_TEMP23
            #define_eval _S_XFER24 _XFER_TEMP24
            #define_eval _S_XFER25 _XFER_TEMP25
            #define_eval _S_XFER26 _XFER_TEMP26
            #define_eval _S_XFER27 _XFER_TEMP27
            #define_eval _S_XFER28 _XFER_TEMP28
            #define_eval _S_XFER29 _XFER_TEMP29
            #define_eval _S_XFER30 _XFER_TEMP30
            #define_eval _S_XFER31 _XFER_TEMP31
        #endif
    #elif (streq(XFER_TOKEN, _SRD_XFER))
        #define_eval _SRD_XFER0 _XFER_TEMP0
        #define_eval _SRD_XFER1 _XFER_TEMP1
        #define_eval _SRD_XFER2 _XFER_TEMP2
        #define_eval _SRD_XFER3 _XFER_TEMP3
        #define_eval _SRD_XFER4 _XFER_TEMP4
        #define_eval _SRD_XFER5 _XFER_TEMP5
        #define_eval _SRD_XFER6 _XFER_TEMP6
        #define_eval _SRD_XFER7 _XFER_TEMP7
        #define_eval _SRD_XFER8 _XFER_TEMP8
        #define_eval _SRD_XFER9 _XFER_TEMP9
        #define_eval _SRD_XFER10 _XFER_TEMP10
        #define_eval _SRD_XFER11 _XFER_TEMP11
        #define_eval _SRD_XFER12 _XFER_TEMP12
        #define_eval _SRD_XFER13 _XFER_TEMP13
        #define_eval _SRD_XFER14 _XFER_TEMP14
        #define_eval _SRD_XFER15 _XFER_TEMP15
        #if (defined(__NFP_INDIRECT_REF_FORMAT_NFPXXXX))
            #define_eval _SRD_XFER16 _XFER_TEMP16
            #define_eval _SRD_XFER17 _XFER_TEMP17
            #define_eval _SRD_XFER18 _XFER_TEMP18
            #define_eval _SRD_XFER19 _XFER_TEMP19
            #define_eval _SRD_XFER20 _XFER_TEMP20
            #define_eval _SRD_XFER21 _XFER_TEMP21
            #define_eval _SRD_XFER22 _XFER_TEMP22
            #define_eval _SRD_XFER23 _XFER_TEMP23
            #define_eval _SRD_XFER24 _XFER_TEMP24
            #define_eval _SRD_XFER25 _XFER_TEMP25
            #define_eval _SRD_XFER26 _XFER_TEMP26
            #define_eval _SRD_XFER27 _XFER_TEMP27
            #define_eval _SRD_XFER28 _XFER_TEMP28
            #define_eval _SRD_XFER29 _XFER_TEMP29
            #define_eval _SRD_XFER30 _XFER_TEMP30
            #define_eval _SRD_XFER31 _XFER_TEMP31
        #endif
    #elif (streq(XFER_TOKEN, _SWR_XFER))
        #define_eval _SWR_XFER0 _XFER_TEMP0
        #define_eval _SWR_XFER1 _XFER_TEMP1
        #define_eval _SWR_XFER2 _XFER_TEMP2
        #define_eval _SWR_XFER3 _XFER_TEMP3
        #define_eval _SWR_XFER4 _XFER_TEMP4
        #define_eval _SWR_XFER5 _XFER_TEMP5
        #define_eval _SWR_XFER6 _XFER_TEMP6
        #define_eval _SWR_XFER7 _XFER_TEMP7
        #define_eval _SWR_XFER8 _XFER_TEMP8
        #define_eval _SWR_XFER9 _XFER_TEMP9
        #define_eval _SWR_XFER10 _XFER_TEMP10
        #define_eval _SWR_XFER11 _XFER_TEMP11
        #define_eval _SWR_XFER12 _XFER_TEMP12
        #define_eval _SWR_XFER13 _XFER_TEMP13
        #define_eval _SWR_XFER14 _XFER_TEMP14
        #define_eval _SWR_XFER15 _XFER_TEMP15
        #if (defined(__NFP_INDIRECT_REF_FORMAT_NFPXXXX))
            #define_eval _SWR_XFER16 _XFER_TEMP16
            #define_eval _SWR_XFER17 _XFER_TEMP17
            #define_eval _SWR_XFER18 _XFER_TEMP18
            #define_eval _SWR_XFER19 _XFER_TEMP19
            #define_eval _SWR_XFER20 _XFER_TEMP20
            #define_eval _SWR_XFER21 _XFER_TEMP21
            #define_eval _SWR_XFER22 _XFER_TEMP22
            #define_eval _SWR_XFER23 _XFER_TEMP23
            #define_eval _SWR_XFER24 _XFER_TEMP24
            #define_eval _SWR_XFER25 _XFER_TEMP25
            #define_eval _SWR_XFER26 _XFER_TEMP26
            #define_eval _SWR_XFER27 _XFER_TEMP27
            #define_eval _SWR_XFER28 _XFER_TEMP28
            #define_eval _SWR_XFER29 _XFER_TEMP29
            #define_eval _SWR_XFER30 _XFER_TEMP30
            #define_eval _SWR_XFER31 _XFER_TEMP31
        #endif
    #elif (streq(XFER_TOKEN, _D_XFER))
        #define_eval _D_XFER0 _XFER_TEMP0
        #define_eval _D_XFER1 _XFER_TEMP1
        #define_eval _D_XFER2 _XFER_TEMP2
        #define_eval _D_XFER3 _XFER_TEMP3
        #define_eval _D_XFER4 _XFER_TEMP4
        #define_eval _D_XFER5 _XFER_TEMP5
        #define_eval _D_XFER6 _XFER_TEMP6
        #define_eval _D_XFER7 _XFER_TEMP7
        #define_eval _D_XFER8 _XFER_TEMP8
        #define_eval _D_XFER9 _XFER_TEMP9
        #define_eval _D_XFER10 _XFER_TEMP10
        #define_eval _D_XFER11 _XFER_TEMP11
        #define_eval _D_XFER12 _XFER_TEMP12
        #define_eval _D_XFER13 _XFER_TEMP13
        #define_eval _D_XFER14 _XFER_TEMP14
        #define_eval _D_XFER15 _XFER_TEMP15
        #if (defined(__NFP_INDIRECT_REF_FORMAT_NFPXXXX))
            #define_eval _D_XFER16 _XFER_TEMP16
            #define_eval _D_XFER17 _XFER_TEMP17
            #define_eval _D_XFER18 _XFER_TEMP18
            #define_eval _D_XFER19 _XFER_TEMP19
            #define_eval _D_XFER20 _XFER_TEMP20
            #define_eval _D_XFER21 _XFER_TEMP21
            #define_eval _D_XFER22 _XFER_TEMP22
            #define_eval _D_XFER23 _XFER_TEMP23
            #define_eval _D_XFER24 _XFER_TEMP24
            #define_eval _D_XFER25 _XFER_TEMP25
            #define_eval _D_XFER26 _XFER_TEMP26
            #define_eval _D_XFER27 _XFER_TEMP27
            #define_eval _D_XFER28 _XFER_TEMP28
            #define_eval _D_XFER29 _XFER_TEMP29
            #define_eval _D_XFER30 _XFER_TEMP30
            #define_eval _D_XFER31 _XFER_TEMP31
        #endif
    #elif (streq(XFER_TOKEN, _DRD_XFER))
        #define_eval _DRD_XFER0 _XFER_TEMP0
        #define_eval _DRD_XFER1 _XFER_TEMP1
        #define_eval _DRD_XFER2 _XFER_TEMP2
        #define_eval _DRD_XFER3 _XFER_TEMP3
        #define_eval _DRD_XFER4 _XFER_TEMP4
        #define_eval _DRD_XFER5 _XFER_TEMP5
        #define_eval _DRD_XFER6 _XFER_TEMP6
        #define_eval _DRD_XFER7 _XFER_TEMP7
        #define_eval _DRD_XFER8 _XFER_TEMP8
        #define_eval _DRD_XFER9 _XFER_TEMP9
        #define_eval _DRD_XFER10 _XFER_TEMP10
        #define_eval _DRD_XFER11 _XFER_TEMP11
        #define_eval _DRD_XFER12 _XFER_TEMP12
        #define_eval _DRD_XFER13 _XFER_TEMP13
        #define_eval _DRD_XFER14 _XFER_TEMP14
        #define_eval _DRD_XFER15 _XFER_TEMP15
        #if (defined(__NFP_INDIRECT_REF_FORMAT_NFPXXXX))
            #define_eval _DRD_XFER16 _XFER_TEMP16
            #define_eval _DRD_XFER17 _XFER_TEMP17
            #define_eval _DRD_XFER18 _XFER_TEMP18
            #define_eval _DRD_XFER19 _XFER_TEMP19
            #define_eval _DRD_XFER20 _XFER_TEMP20
            #define_eval _DRD_XFER21 _XFER_TEMP21
            #define_eval _DRD_XFER22 _XFER_TEMP22
            #define_eval _DRD_XFER23 _XFER_TEMP23
            #define_eval _DRD_XFER24 _XFER_TEMP24
            #define_eval _DRD_XFER25 _XFER_TEMP25
            #define_eval _DRD_XFER26 _XFER_TEMP26
            #define_eval _DRD_XFER27 _XFER_TEMP27
            #define_eval _DRD_XFER28 _XFER_TEMP28
            #define_eval _DRD_XFER29 _XFER_TEMP29
            #define_eval _DRD_XFER30 _XFER_TEMP30
            #define_eval _DRD_XFER31 _XFER_TEMP31
        #endif
    #elif ((!defined(__NFP_INDIRECT_REF_FORMAT_NFPXXXX)) && streq(XFER_TOKEN, _DWR_XFER))
        #define_eval _DWR_XFER0 _XFER_TEMP0
        #define_eval _DWR_XFER1 _XFER_TEMP1
        #define_eval _DWR_XFER2 _XFER_TEMP2
        #define_eval _DWR_XFER3 _XFER_TEMP3
        #define_eval _DWR_XFER4 _XFER_TEMP4
        #define_eval _DWR_XFER5 _XFER_TEMP5
        #define_eval _DWR_XFER6 _XFER_TEMP6
        #define_eval _DWR_XFER7 _XFER_TEMP7
        #define_eval _DWR_XFER8 _XFER_TEMP8
        #define_eval _DWR_XFER9 _XFER_TEMP9
        #define_eval _DWR_XFER10 _XFER_TEMP10
        #define_eval _DWR_XFER11 _XFER_TEMP11
        #define_eval _DWR_XFER12 _XFER_TEMP12
        #define_eval _DWR_XFER13 _XFER_TEMP13
        #define_eval _DWR_XFER14 _XFER_TEMP14
        #define_eval _DWR_XFER15 _XFER_TEMP15
        #if (defined(__NFP_INDIRECT_REF_FORMAT_NFPXXXX))
            #define_eval _DWR_XFER16 _XFER_TEMP16
            #define_eval _DWR_XFER17 _XFER_TEMP17
            #define_eval _DWR_XFER18 _XFER_TEMP18
            #define_eval _DWR_XFER19 _XFER_TEMP19
            #define_eval _DWR_XFER20 _XFER_TEMP20
            #define_eval _DWR_XFER21 _XFER_TEMP21
            #define_eval _DWR_XFER22 _XFER_TEMP22
            #define_eval _DWR_XFER23 _XFER_TEMP23
            #define_eval _DWR_XFER24 _XFER_TEMP24
            #define_eval _DWR_XFER25 _XFER_TEMP25
            #define_eval _DWR_XFER26 _XFER_TEMP26
            #define_eval _DWR_XFER27 _XFER_TEMP27
            #define_eval _DWR_XFER28 _XFER_TEMP28
            #define_eval _DWR_XFER29 _XFER_TEMP29
            #define_eval _DWR_XFER30 _XFER_TEMP30
            #define_eval _DWR_XFER31 _XFER_TEMP31
        #endif
    #elif (streq(XFER_TOKEN, _BUF0))
        #define_eval _BUF0_REG0 _XFER_TEMP0
        #define_eval _BUF0_REG1 _XFER_TEMP1
        #define_eval _BUF0_REG2 _XFER_TEMP2
        #define_eval _BUF0_REG3 _XFER_TEMP3
        #define_eval _BUF0_REG4 _XFER_TEMP4
        #define_eval _BUF0_REG5 _XFER_TEMP5
        #define_eval _BUF0_REG6 _XFER_TEMP6
        #define_eval _BUF0_REG7 _XFER_TEMP7
        #define_eval _BUF0_REG8 _XFER_TEMP8
        #define_eval _BUF0_REG9 _XFER_TEMP9
        #define_eval _BUF0_REG10 _XFER_TEMP10
        #define_eval _BUF0_REG11 _XFER_TEMP11
        #define_eval _BUF0_REG12 _XFER_TEMP12
        #define_eval _BUF0_REG13 _XFER_TEMP13
        #define_eval _BUF0_REG14 _XFER_TEMP14
        #define_eval _BUF0_REG15 _XFER_TEMP15
    #elif (streq(XFER_TOKEN, _BUF1))
        #define_eval _BUF1_REG0 _XFER_TEMP0
        #define_eval _BUF1_REG1 _XFER_TEMP1
        #define_eval _BUF1_REG2 _XFER_TEMP2
        #define_eval _BUF1_REG3 _XFER_TEMP3
        #define_eval _BUF1_REG4 _XFER_TEMP4
        #define_eval _BUF1_REG5 _XFER_TEMP5
        #define_eval _BUF1_REG6 _XFER_TEMP6
        #define_eval _BUF1_REG7 _XFER_TEMP7
        #define_eval _BUF1_REG8 _XFER_TEMP8
        #define_eval _BUF1_REG9 _XFER_TEMP9
        #define_eval _BUF1_REG10 _XFER_TEMP10
        #define_eval _BUF1_REG11 _XFER_TEMP11
        #define_eval _BUF1_REG12 _XFER_TEMP12
        #define_eval _BUF1_REG13 _XFER_TEMP13
        #define_eval _BUF1_REG14 _XFER_TEMP14
        #define_eval _BUF1_REG15 _XFER_TEMP15
    #elif (streq(XFER_TOKEN, _BUF2))
        #define_eval _BUF2_REG0 _XFER_TEMP0
        #define_eval _BUF2_REG1 _XFER_TEMP1
        #define_eval _BUF2_REG2 _XFER_TEMP2
        #define_eval _BUF2_REG3 _XFER_TEMP3
        #define_eval _BUF2_REG4 _XFER_TEMP4
        #define_eval _BUF2_REG5 _XFER_TEMP5
        #define_eval _BUF2_REG6 _XFER_TEMP6
        #define_eval _BUF2_REG7 _XFER_TEMP7
        #define_eval _BUF2_REG8 _XFER_TEMP8
        #define_eval _BUF2_REG9 _XFER_TEMP9
        #define_eval _BUF2_REG10 _XFER_TEMP10
        #define_eval _BUF2_REG11 _XFER_TEMP11
        #define_eval _BUF2_REG12 _XFER_TEMP12
        #define_eval _BUF2_REG13 _XFER_TEMP13
        #define_eval _BUF2_REG14 _XFER_TEMP14
        #define_eval _BUF2_REG15 _XFER_TEMP15
    #elif (streq(XFER_TOKEN, _BUF3))
        #define_eval _BUF3_REG0 _XFER_TEMP0
        #define_eval _BUF3_REG1 _XFER_TEMP1
        #define_eval _BUF3_REG2 _XFER_TEMP2
        #define_eval _BUF3_REG3 _XFER_TEMP3
        #define_eval _BUF3_REG4 _XFER_TEMP4
        #define_eval _BUF3_REG5 _XFER_TEMP5
        #define_eval _BUF3_REG6 _XFER_TEMP6
        #define_eval _BUF3_REG7 _XFER_TEMP7
        #define_eval _BUF3_REG8 _XFER_TEMP8
        #define_eval _BUF3_REG9 _XFER_TEMP9
        #define_eval _BUF3_REG10 _XFER_TEMP10
        #define_eval _BUF3_REG11 _XFER_TEMP11
        #define_eval _BUF3_REG12 _XFER_TEMP12
        #define_eval _BUF3_REG13 _XFER_TEMP13
        #define_eval _BUF3_REG14 _XFER_TEMP14
        #define_eval _BUF3_REG15 _XFER_TEMP15
    #else
        #error "xbuf_xfer_set: first arg sram XFER_TOKEN must be _XFER, _S_XFER, _SRD_XFER, \
                _SWR_XFER, _D_XFER, _DRD_XFER, _DWR_XFER, _BUF0, _BUF1, _BUF2, or _BUF3"
    #endif

    #ifdef _XFER_TEMP0
        #undef _XFER_TEMP0
    #endif

    #ifdef _XFER_TEMP1
        #undef _XFER_TEMP1
    #endif

    #ifdef _XFER_TEMP2
        #undef _XFER_TEMP2
    #endif

    #ifdef _XFER_TEMP3
        #undef _XFER_TEMP3
    #endif

    #ifdef _XFER_TEMP4
        #undef _XFER_TEMP4
    #endif

    #ifdef _XFER_TEMP5
        #undef _XFER_TEMP5
    #endif

    #ifdef _XFER_TEMP6
        #undef _XFER_TEMP6
    #endif

    #ifdef _XFER_TEMP7
        #undef _XFER_TEMP7
    #endif

    #ifdef _XFER_TEMP8
        #undef _XFER_TEMP8
    #endif

    #ifdef _XFER_TEMP9
        #undef _XFER_TEMP9
    #endif

    #ifdef _XFER_TEMP10
        #undef _XFER_TEMP10
    #endif

    #ifdef _XFER_TEMP11
        #undef _XFER_TEMP11
    #endif

    #ifdef _XFER_TEMP12
        #undef _XFER_TEMP12
    #endif

    #ifdef _XFER_TEMP13
        #undef _XFER_TEMP13
    #endif

    #ifdef _XFER_TEMP14
        #undef _XFER_TEMP14
    #endif

    #ifdef _XFER_TEMP15
        #undef _XFER_TEMP15
    #endif


    #if (defined(__NFP_INDIRECT_REF_FORMAT_NFPXXXX))
        #ifdef _XFER_TEMP16
            #undef _XFER_TEMP16
        #endif

        #ifdef _XFER_TEMP17
            #undef _XFER_TEMP17
        #endif

        #ifdef _XFER_TEMP18
            #undef _XFER_TEMP18
        #endif

        #ifdef _XFER_TEMP19
            #undef _XFER_TEMP19
        #endif

        #ifdef _XFER_TEMP20
            #undef _XFER_TEMP20
        #endif

        #ifdef _XFER_TEMP21
            #undef _XFER_TEMP21
        #endif

        #ifdef _XFER_TEMP22
            #undef _XFER_TEMP22
        #endif

        #ifdef _XFER_TEMP23
            #undef _XFER_TEMP23
        #endif

        #ifdef _XFER_TEMP24
            #undef _XFER_TEMP24
        #endif

        #ifdef _XFER_TEMP25
            #undef _XFER_TEMP25
        #endif

        #ifdef _XFER_TEMP26
            #undef _XFER_TEMP26
        #endif

        #ifdef _XFER_TEMP27
            #undef _XFER_TEMP27
        #endif

        #ifdef _XFER_TEMP28
            #undef _XFER_TEMP28
        #endif

        #ifdef _XFER_TEMP29
            #undef _XFER_TEMP29
        #endif

        #ifdef _XFER_TEMP30
            #undef _XFER_TEMP30
        #endif

        #ifdef _XFER_TEMP31
            #undef _XFER_TEMP31
        #endif
    #endif

    #undef _XBUF_XFER_SET_STR_XBUF_TYPE
#endm


/// @cond INTERNAL_MACROS
/** An internal macro used by xbuf_extract & xbuf_insert to set the constants.
 *
 * @param str_xbuf_name     Name of first sram transfer buffer
 * @param WINDOW_START      Start byte position of window to extract from.
 *                          Offset from the beginning of the buffer to
 *                          location of window to extract from
 * @param FIELD_START       Byte offset of field to be extracted
 *                          starting from @p window_start. @n
 *                          @p window_start + @p field_start -> exact byte location
 *                          of first byte to extract.
 * @param NUMBER_OF_BYTES   Number of bytes to extract. Because @p out_byte_field
 *                          is a 32-bit GPR, the maximum number of bytes to
 *                          extract is 4.
 *
 * @instruction_cnt 0
 */
#macro _find_xbuf_and_relative_sb_eb(str_xbuf_name, WINDOW_START, FIELD_START, NUMBER_OF_BYTES)
    // get xbuf type
    _GET_XBUF_TYPE(str_xbuf_name)
    #ifdef _REL_SB_EB_XBUF_TYPE
        #warning "_REL_SB_EB_XBUF_TYPE is being redefined."
    #endif
    #define_eval _REL_SB_EB_XBUF_TYPE _XBUF_TYPE
    #undef _XBUF_TYPE

    #define_eval _START_BYTE (WINDOW_START + FIELD_START)
    #define_eval _END_BYTE (_START_BYTE + (NUMBER_OF_BYTES - 1))

    // setup word0/1 defs
    #define_eval _WORD0 (_START_BYTE / 4)
    #define_eval _WORD1 (_END_BYTE / 4)

    #if ((_WORD1 - _WORD0) > 1)
        #error 2   "Byte field greater than 4 bytes - Not supported."
    #endif

    #ifdef _str_word0_xfer
        #undef _str_word0_xfer
    #endif
    #ifdef _str_word1_xfer
        #undef _str_word1_xfer
    #endif

    _DEFINE_THIS_XBUF(str_xbuf_name)

    #define_eval _RELATIVE_SBYTE _START_BYTE

    #if (defined(__NFP_INDIRECT_REF_FORMAT_NFPXXXX) && \
            (_REL_SB_EB_XBUF_TYPE == _XBUF_SRAM_XFER))
        #define_eval _find_xbuf_and_relative_sb_eb_cnt_buffers 8
    #else
        #define_eval _find_xbuf_and_relative_sb_eb_cnt_buffers 4
    #endif

    #define_eval _find_xbuf_and_relative_sb_eb_loopA (_find_xbuf_and_relative_sb_eb_cnt_buffers - 1)
    #while ( _find_xbuf_and_relative_sb_eb_loopA > 0 )
        #if (_WORD0 < _this_xbuf_size)
            #define_eval _find_xbuf_and_relative_sb_eb_loopA 0
        #else
            #if ((_REL_SB_EB_XBUF_TYPE == _XBUF_LMEM) && \
                    (_find_xbuf_and_relative_sb_eb_loopA == (_find_xbuf_and_relative_sb_eb_cnt_buffers - 2)))
                #error 2 "Start byte is out of range of local memory buffers."
                #define_eval _find_xbuf_and_relative_sb_eb_loopA 0
            #elif (defined(_this_xbuf_next))
                #define_eval _WORD0 (_WORD0 - _this_xbuf_size)
                #define_eval _RELATIVE_SBYTE (_RELATIVE_SBYTE - (_this_xbuf_size * 4))
                #define_eval _next _this_xbuf_next
                _UNDEFINE_THIS_XBUF()
                _DEFINE_THIS_XBUF(_next)
                #define_eval _find_xbuf_and_relative_sb_eb_loopA (_find_xbuf_and_relative_sb_eb_loopA - 1)
            #else
                #define_eval _find_xbuf_and_relative_sb_eb_loopA 0
            #endif
        #endif
    #endloop
    #undef _find_xbuf_and_relative_sb_eb_loopA


    #if (_WORD0 < _this_xbuf_size)
        #define_eval _str_word0_xfer _this_xbuf_name[/**/_WORD0/**/]
    #else
        #error 2   "Start byte is out of range of buffers."
    #endif


    // setup word1 defs
    #define_eval _RELATIVE_EBYTE    (_RELATIVE_SBYTE + (NUMBER_OF_BYTES - 1))
    #define_eval _WORD1 (_RELATIVE_EBYTE / 4)

    #define_eval _find_xbuf_and_relative_sb_eb_loopA (_find_xbuf_and_relative_sb_eb_cnt_buffers - 1)
    #while ( _find_xbuf_and_relative_sb_eb_loopA > 0 )
        #if (_WORD1 < _this_xbuf_size)
            #define_eval _find_xbuf_and_relative_sb_eb_loopA 0
        #else
            #if ((_REL_SB_EB_XBUF_TYPE == _XBUF_LMEM) && \
                    (_find_xbuf_and_relative_sb_eb_loopA == (_find_xbuf_and_relative_sb_eb_cnt_buffers - 2)))
                #error 2 "End byte is out of range of local memory buffers."
                #define_eval _find_xbuf_and_relative_sb_eb_loopA 0
            #elif (defined(_this_xbuf_next))
                #define_eval _WORD1 (_WORD1 - _this_xbuf_size)
                #define_eval _RELATIVE_EBYTE (_RELATIVE_EBYTE - (_this_xbuf_size * 4))
                #define_eval _next _this_xbuf_next
                _UNDEFINE_THIS_XBUF()
                _DEFINE_THIS_XBUF(_next)
                #define_eval _find_xbuf_and_relative_sb_eb_loopA (_find_xbuf_and_relative_sb_eb_loopA - 1)
            #else
                #define_eval _find_xbuf_and_relative_sb_eb_loopA 0
            #endif
        #endif
    #endloop
    #undef _find_xbuf_and_relative_sb_eb_loopA

    #undef _find_xbuf_and_relative_sb_eb_cnt_buffers

    #if (_WORD1 < _this_xbuf_size)
        #define_eval _str_word1_xfer _this_xbuf_name[/**/_WORD1/**/]
    #else
        #error 2   "End byte is out of range of transfer buffers."
    #endif

    _UNDEFINE_THIS_XBUF()

    #undef _REL_SB_EB_XBUF_TYPE
    #ifdef _next
        #undef _next
    #endif
#endm
/// @endcond


/// @cond INTERNAL_MACROS
/** An internal macro used by xbuf_extract & xbuf_insert to unset the constants.
 *
 * @instruction_cnt 0
 */
#macro _clear_name_space_for_find_xbuf_and_relative_sb_eb()
    #undef _WORD0
    #undef _WORD1
    #ifdef _str_word0_xfer
        #undef _str_word0_xfer
    #endif
    #ifdef _str_word1_xfer
        #undef _str_word1_xfer
    #endif
    #undef _RELATIVE_SBYTE
    #undef _RELATIVE_EBYTE
#endm
/// @endcond


/** Extract a numeric byte field from a register buffer.
 *
 * @param out_byte_field  GPR output field, right justified
 * @param str_xbuf_name    Name of source to extract from. It can be a buffer
 *                         of transfer registers, local memory, or GPRs.
 * @param window_start     Start byte position of window to extract from.
 *                         Offset from the beginning of the buffer to
 *                         the location of the window to extract from.
 * @param field_start      Byte offset of the field to be extracted
 *                         starting from @p window_start. @n
 *                         @p window_start + @p field_start -> exact byte location
 *                         of first byte to extract
 * @param number_of_bytes  Number of bytes to extract. Because @p out_byte_field
 *                         is a 32-bit GPR, maximum number of bytes to
 *                         extract is 4.
 *
 * @note
 * -# No error checking in order to minimize the number of instructions.
 *    If users try to extract more than 4 bytes or
 *    pass in an field-offset that is bigger than the buffer size,
 *    the result will be undefined.
 * -# If the source buffer is GPRs, users can define a compile option
 *    _EXTRACT_FROM_BANK_B_GPR if they know for sure that the source
 *    is in bank B. When defined, this option makes this macro skip
 *    moving the source register to bank B, hence saving 1 to 2 instructions.
 *
 * <b>Example:</b>
 * @code
 * xbuf_alloc(wbuf, 2, read_write)
 * immed32(wbuf[0], 0x01020304)
 * immed32(wbuf[1], 0x50607080)
 *
 * xbuf_alloc(wbuf_next1, 2, read_write)
 * immed32(wbuf_next1[0], 0x13459851)
 * immed32(wbuf_next1[1], 0x198428e5)
 *
 * xbuf_alloc(wbuf_next2, 2, read_write)
 * immed32(wbuf_next2[0], 0x11111111)
 * immed32(wbuf_next2[1], 0x22222222)
 *
 * xbuf_link(wbuf, wbuf_next1)
 * xbuf_link(wbuf_next1, wbuf_next2)
 *
 * alu[win_start, --, B, 3]
 * alu[field_start, --, B, 2]
 * alu[num_of_bytes, --, B, 4]
 *
 * xbuf_extract(out_byte_field, wbuf, win_start, field_start, num_of_bytes)
 * @endcode
 *
 * @instruction_cnt 1-12
 */
#macro xbuf_extract(out_byte_field, str_xbuf_name, window_start, field_start, number_of_bytes)
.begin

    .reg extract_temp start_loc out_buf_num

    #define_eval    _EXTRACT_OR_INSERT  _EXTRACT

    _GET_XBUF_TYPE(str_xbuf_name)
    #define_eval    _EXTRACT_SRC_XBUF_TYPE  _XBUF_TYPE
    #undef _XBUF_TYPE

    _DEFINE_THIS_XBUF(str_xbuf_name)
    //size of buffer in bytes
    #define_eval _EXTRACT_SRC_XBUF_SIZE (_this_xbuf_size * 4)
    #ifdef _this_xbuf_next
        #define_eval    _LINKED_SRC_BUF _this_xbuf_next
    #endif
    #if (_EXTRACT_SRC_XBUF_TYPE == _XBUF_LMEM)
        #define_eval _EXTRACT_SRC_LMEM_INDEX    _this_xbuf_index
    #endif
    _UNDEFINE_THIS_XBUF()

    #if (_EXTRACT_SRC_XBUF_TYPE == _XBUF_SRAM_XFER)
        #define_eval    _EXTRACT_INDEX  *$index
    #elif ( (!defined(__NFP_INDIRECT_REF_FORMAT_NFPXXXX)) && (_EXTRACT_SRC_XBUF_TYPE == _XBUF_DRAM_XFER) )
        #define_eval    _EXTRACT_INDEX  *$$index
    #endif


    #if ((is_ct_const(window_start) || isimport(window_start)) && \
        (is_ct_const(field_start) || isimport(field_start)) && \
        (is_ct_const(number_of_bytes) || isimport(number_of_bytes)))
    ;case 1: all offsets and size are constants
        _xbuf_extract_const_offsets_and_size(out_byte_field, str_xbuf_name, window_start, field_start, number_of_bytes)

    #elif ((is_ct_const(window_start) || isimport(window_start)) && \
            (is_ct_const(field_start) || isimport(field_start)))
        ;case 2: offsets are constants, size is run-time value

        ; Skip error checking to save instruction. If users want to extract more than
        ; 4 bytes, result is undefined

        alu[extract_temp, number_of_bytes, AND~ , 4]
        beq[extract_4_bytes#]
        alu[--, extract_temp, -, 1]
        beq[extract_1_byte#]
        alu[--, extract_temp, -, 2]
        beq[extract_2_bytes#]

        br[extract_3_bytes#]

        extract_1_byte#:
            _xbuf_extract_const_offsets_and_size(out_byte_field, str_xbuf_name, window_start, field_start, 1)
            br[end_extract#]
        extract_2_bytes#:
            _xbuf_extract_const_offsets_and_size(out_byte_field, str_xbuf_name, window_start, field_start, 2)
            br[end_extract#]
        extract_3_bytes#:
            _xbuf_extract_const_offsets_and_size(out_byte_field, str_xbuf_name, window_start, field_start, 3)
            br[end_extract#]
        extract_4_bytes#:
            _xbuf_extract_const_offsets_and_size(out_byte_field, str_xbuf_name, window_start, field_start, 4)
            br[end_extract#]

    #elif (is_ct_const(number_of_bytes) || isimport(number_of_bytes))
        ;case 3: offsets are run-time values, size is constant

        #if ((_EXTRACT_SRC_XBUF_TYPE == _XBUF_SRAM_XFER) || \
                (_EXTRACT_SRC_XBUF_TYPE == _XBUF_DRAM_XFER))


            .begin

                .reg active_ctx num_ctx

                alu[start_loc, field_start, +, window_start]

                br=ctx[0, case_3_skip_find_ctx#], defer[2]

                add_shf_left(start_loc, start_loc, &str_xbuf_name[0], 2)

                _find_absolute_xfer_reg_addr(start_loc)

            case_3_skip_find_ctx#:
                #define_eval _SHF_VAL ((4 - number_of_bytes) * 8)

                local_csr_wr[T_INDEX_BYTE_INDEX, start_loc]
                //need 3 cyles before T_INDEX_BYTE_INDEX can be used
                nop
                nop
                nop
                //get 4 bytes into extract_temp
                byte_align_be[--, _EXTRACT_INDEX/**/++]
                byte_align_be[out_byte_field, _EXTRACT_INDEX]

                #if (_SHF_VAL > 0)
                    alu_shf[out_byte_field, --, B, out_byte_field, >>_SHF_VAL]
                #endif
                #undef _SHF_VAL

            .end

        #elif (_EXTRACT_SRC_XBUF_TYPE == _XBUF_LMEM)

            .begin

                .reg save_active_lm  tmp_src1 tmp_src2

                #define_eval _SHF_VAL ((4 - number_of_bytes) * 8)

                alu[start_loc, field_start, +, window_start]
                local_csr_rd[ACTIVE_LM_ADDR_/**/_EXTRACT_SRC_LMEM_INDEX]
                immed[save_active_lm, 0]

                local_csr_wr[BYTE_INDEX, start_loc]

                alu[start_loc, start_loc, +, save_active_lm]
                local_csr_wr[ACTIVE_LM_ADDR_/**/_EXTRACT_SRC_LMEM_INDEX, start_loc]
                //need 3 cyles to settle
                nop
                nop
                nop

                /////////////LMEM WORK-AROUND////////////////////////////////
                ;copy source in temp registers because we can't increment lmem
                ;pointer and can't do byte_align with lmem index > 7
                alu[tmp_src1, --, b, *l$index/**/_EXTRACT_SRC_LMEM_INDEX++]
                alu[tmp_src2, --, b, *l$index/**/_EXTRACT_SRC_LMEM_INDEX]
                /////////////END LMEM WORK-AROUND////////////////////////////////

                //restore active LM value
                local_csr_wr[ACTIVE_LM_ADDR_/**/_EXTRACT_SRC_LMEM_INDEX, save_active_lm]
                //need 3 cyles to settle

                //get 4 bytes into extract_temp
                byte_align_be[--, tmp_src1]
                byte_align_be[out_byte_field, tmp_src2]

                #if (_SHF_VAL > 0)
                    alu_shf[out_byte_field, --, B, out_byte_field, >>_SHF_VAL]
                #endif
                #undef _SHF_VAL

            .end

        #else   //end extract from xfer regs

            ; skip error checking to save instruction. If users want to read more than
            ; buffer size, result is undefined
            //_xbuf_check_space(str_xbuf_name, start_loc, number_of_bytes, 1)
            //bgt[end_extract#]

            alu[start_loc, field_start, +, window_start]
            _EXTRACT_INSERT_JUMP_TO_ENTRY(out_byte_field, str_xbuf_name, start_loc, \
                                                number_of_bytes, _EXTRACT_OR_INSERT)
        #endif //extract from xfer regs
    #else
    ;case 4: offsets and size are all run-time values

        alu[start_loc, window_start, +, field_start]

        #if ((_EXTRACT_SRC_XBUF_TYPE == _XBUF_SRAM_XFER) || \
                (_EXTRACT_SRC_XBUF_TYPE == _XBUF_DRAM_XFER))
            .begin

                .reg extract_shf

                br=ctx[0, case_4_skip_find_ctx#], defer[2]

                add_shf_left(start_loc, start_loc, &str_xbuf_name[0], 2)

                //if not context 0, need to add offset to absolute reg to start_loc
                _find_absolute_xfer_reg_addr(start_loc)

            case_4_skip_find_ctx#:
                //add_shf_left(start_loc, start_loc, &str_xbuf_name[0], 2)

                local_csr_wr[T_INDEX_BYTE_INDEX, start_loc]
                //need 3 cyles before T_INDEX_BYTE_INDEX can be used
                alu[extract_shf, 4, -, number_of_bytes]
                alu_shf[extract_shf, --, B, extract_shf, <<3]
                nop
                //get 4 bytes into extract_temp
                byte_align_be[--, _EXTRACT_INDEX/**/++]
                byte_align_be[extract_temp, _EXTRACT_INDEX]

                alu[--, extract_shf, OR, 0]
                alu_shf[out_byte_field, --, B, extract_temp, >>indirect]

            .end

        #elif (_EXTRACT_SRC_XBUF_TYPE == _XBUF_LMEM)

            .begin

                .reg save_active_lm  tmp_src1 tmp_src2 extract_shf

                local_csr_rd[ACTIVE_LM_ADDR_/**/_EXTRACT_SRC_LMEM_INDEX]
                immed[save_active_lm, 0]

                local_csr_wr[BYTE_INDEX, start_loc]

                alu[start_loc, start_loc, +, save_active_lm]
                local_csr_wr[ACTIVE_LM_ADDR_/**/_EXTRACT_SRC_LMEM_INDEX, start_loc]
                //need 3 cyles to settle
                alu[extract_shf, 4, -, number_of_bytes]
                alu_shf[extract_shf, --, B, extract_shf, <<3]
                nop

                /////////////LMEM WORK-AROUND////////////////////////////////
                ;copy source in temp registers because we can't increment lmem
                ;pointer and can't do byte_align with lmem index > 7
                alu[tmp_src1, --, b, *l$index/**/_EXTRACT_SRC_LMEM_INDEX++]
                alu[tmp_src2, --, b, *l$index/**/_EXTRACT_SRC_LMEM_INDEX]
                /////////////END LMEM WORK-AROUND////////////////////////////////

                //restore active LM value
                local_csr_wr[ACTIVE_LM_ADDR_/**/_EXTRACT_SRC_LMEM_INDEX, save_active_lm]
                //need 3 cyles to settle

                //get 4 bytes into extract_temp
                byte_align_be[--, tmp_src1]
                byte_align_be[extract_temp, tmp_src2]

                alu[--, extract_shf, OR, 0]
                alu_shf[out_byte_field, --, B, extract_temp, >>indirect]

            .end

        #else   //end extract from xfer regs

            _EXTRACT_INSERT_FIND_SIZE_THEN_JUMP_TO_ENTRY(out_byte_field, \
                            str_xbuf_name, start_loc, number_of_bytes, _EXTRACT_OR_INSERT)
        #endif

    #endif  //end case 4

end_extract#:
    .use str_xbuf_name[0]
    #ifdef  _LINKED_SRC_BUF
        .use _LINKED_SRC_BUF[0]
    #endif

    #undef _EXTRACT_SRC_XBUF_TYPE
    #ifdef _EXTRACT_SRC_XBUF_SIZE
        #undef _EXTRACT_SRC_XBUF_SIZE
    #endif

    #ifdef  _LINKED_SRC_BUF
        #undef  _LINKED_SRC_BUF
    #endif

    #ifdef _EXTRACT_INDEX
        #undef _EXTRACT_INDEX
    #endif

    #ifdef _EXTRACT_SRC_LMEM_INDEX
        #undef _EXTRACT_SRC_LMEM_INDEX
    #endif

.end
#endm


/** Extract a numeric byte field from a register buffer.
 *
 * The source of data may be spread in 2 buffers: str_xbuf_name and the
 * buffer that is linked to it. DATA_SPREAD indicates whether or
 * not the data spreads 2 buffers. If DATA_SPREAD = 0,
 * this macro functions the same way as xbuf_extract.
 *
 * @param out_byte_field    GPR output field, right justified
 * @param str_xbuf_name     Name of sram read transfer register
 * @param window_start      Start byte position of window or header
 * @param field_start       Start byte offset of field from window start
 * @param number_of_bytes   Number of bytes to extract. Because the @p out_byte_field is
 *                          a GPR (32-bit), maximum number of bytes to extract is 4.
 * @param DATA_SPREAD       CONSTANT to indicate whether source data spreads 2 buffers @n
 *                          @arg 1: If part of data to be extracted spreads into
 *                                  buffer linked to in_src_xbuf
 *                          @arg 0: All data to be extracted resides in str_xbuf_name.
 *
 * @instruction_cnt 1-15
 */
#macro xbuf_extract_frm_linked_bufs(out_byte_field, str_xbuf_name, window_start, \
        field_start, number_of_bytes, DATA_SPREAD)
.begin

    .reg extract_temp start_loc out_buf_num

    #define_eval    _EXTRACT_OR_INSERT  _EXTRACT

    _GET_XBUF_TYPE(str_xbuf_name)
    #define_eval    _EXTRACT_SRC_XBUF_TYPE  _XBUF_TYPE
    #undef _XBUF_TYPE

    _DEFINE_THIS_XBUF(str_xbuf_name)
    //size of buffer in bytes
    #define_eval _EXTRACT_SRC_XBUF_SIZE (_this_xbuf_size * 4)
    #ifdef _this_xbuf_next
        #define_eval    _LINKED_SRC_BUF _this_xbuf_next
    #endif
    #if (_EXTRACT_SRC_XBUF_TYPE == _XBUF_LMEM)
        #define_eval _EXTRACT_SRC_LMEM_INDEX    _this_xbuf_index
    #endif
    _UNDEFINE_THIS_XBUF()

    #if (_EXTRACT_SRC_XBUF_TYPE == _XBUF_SRAM_XFER)
        #define_eval    _EXTRACT_INDEX  *$index
    #elif ( (!defined(__NFP_INDIRECT_REF_FORMAT_NFPXXXX)) && (_EXTRACT_SRC_XBUF_TYPE == _XBUF_DRAM_XFER) )
        #define_eval    _EXTRACT_INDEX  *$$index
    #endif


    #if ((is_ct_const(window_start) || isimport(window_start)) && \
        (is_ct_const(field_start) || isimport(field_start)) && \
        (is_ct_const(number_of_bytes) || isimport(number_of_bytes)))
        ;case 1: all offsets and size are constants
        _xbuf_extract_const_offsets_and_size(out_byte_field, str_xbuf_name, window_start, field_start, number_of_bytes)

    #elif ((is_ct_const(window_start) || isimport(window_start)) && \
            (is_ct_const(field_start) || isimport(field_start)))
        ;case 2: offsets are constants, size is run-time value

        ; skip error checking to save instruction. If users want to extract more than
        ; 4 bytes, result is undefined

        alu[extract_temp, number_of_bytes, AND~ , 4]
        beq[extract_4_bytes#]
        alu[--, extract_temp, -, 1]
        beq[extract_1_byte#]
        alu[--, extract_temp, -, 2]
        beq[extract_2_bytes#]

        br[extract_3_bytes#]

        extract_1_byte#:
            _xbuf_extract_const_offsets_and_size(out_byte_field, str_xbuf_name, window_start, field_start, 1)
            br[end_extract#]
        extract_2_bytes#:
            _xbuf_extract_const_offsets_and_size(out_byte_field, str_xbuf_name, window_start, field_start, 2)
            br[end_extract#]
        extract_3_bytes#:
            _xbuf_extract_const_offsets_and_size(out_byte_field, str_xbuf_name, window_start, field_start, 3)
            br[end_extract#]
        extract_4_bytes#:
            _xbuf_extract_const_offsets_and_size(out_byte_field, str_xbuf_name, window_start, field_start, 4)
            br[end_extract#]

    #elif (is_ct_const(number_of_bytes) || isimport(number_of_bytes))
        ;case 3: offsets are run-time values, size is constant

        #define_eval _SHF_VAL ((4 - number_of_bytes) * 8)

        alu[start_loc, field_start, +, window_start]

        #if ((_EXTRACT_SRC_XBUF_TYPE == _XBUF_SRAM_XFER) || \
                (_EXTRACT_SRC_XBUF_TYPE == _XBUF_DRAM_XFER))
            alu_shf[out_buf_num, --, B, &str_xbuf_name[0], <<2]

            #if (DATA_SPREAD == 1)
                #ifdef _LINKED_SRC_BUF

                    #define_eval _BYTES (_EXTRACT_SRC_XBUF_SIZE - number_of_bytes)

                    alu[extract_temp, start_loc, -, /**/_BYTES]
                    ble[case_3_found_reg#], defer[1]

                    alu[--, extract_temp, -, number_of_bytes]
                    bge[case_3_found_reg#], defer[2]

                    alu[start_loc, start_loc, -, /**/_EXTRACT_SRC_XBUF_SIZE]
                    alu_shf[out_buf_num, --, B, &/**/_LINKED_SRC_BUF/**/[0], <<2]

                    .begin

                        .reg extract_mask extract_temp2

                        #define_eval _INDEX ((_EXTRACT_SRC_XBUF_SIZE/4) - 1)

                        alu[out_byte_field, --, B, str_xbuf_name[/**/_INDEX] ]
                        alu[extract_temp2, --, B, /**/_LINKED_SRC_BUF/**/[0]]

                        alu[extract_temp, --, b, extract_temp, <<3]
                        alu[extract_temp, 32, -, extract_temp]

                        alu[extract_temp, extract_temp, OR, 0]
                        dbl_shf[out_byte_field, out_byte_field, extract_temp2, >>indirect]

                        #if (number_of_bytes == 2)
                            immed32(extract_mask, 0xffff)
                            alu[out_byte_field, out_byte_field, AND, extract_mask]
                        #elif (number_of_bytes == 3)
                            immed32(extract_mask, 0xffffff)
                            alu[out_byte_field, out_byte_field, AND, extract_mask]
                        #endif
                        br[end_extract#]

                        #undef _INDEX

                    .end
                    #undef _BYTES
                #else //ifdef LINKED_SRC_BUF
                    #warning "xbuf_extract_frm_linked_bufs: DATA_SPREAD = 1 but source buffer is not linked"
                #endif
            #endif //DATA_SPREAD
    case_3_found_reg#:

        .begin

            .reg active_ctx num_ctx

            br=ctx[0, extract_frm_linked_buf_case3_skip_find_ctx#]

            //if not context 0, need to add offset to absolute reg to start_loc
            _find_absolute_xfer_reg_addr(out_buf_num)

        extract_frm_linked_buf_case3_skip_find_ctx#:
            alu[start_loc, start_loc, +, out_buf_num]

            local_csr_wr[T_INDEX_BYTE_INDEX, start_loc]
            //need 3 cyles before T_INDEX_BYTE_INDEX can be used
            nop
            nop
            nop
            //get 4 bytes into extract_temp
            byte_align_be[--, _EXTRACT_INDEX/**/++]
            byte_align_be[out_byte_field, _EXTRACT_INDEX]

            #if (_SHF_VAL > 0)
                alu_shf[out_byte_field, --, B, out_byte_field, >>_SHF_VAL]
            #endif

        .end

        #elif (_EXTRACT_SRC_XBUF_TYPE == _XBUF_LMEM)

            .begin

                .reg save_active_lm  tmp_src1 tmp_src2

                local_csr_rd[ACTIVE_LM_ADDR_/**/_EXTRACT_SRC_LMEM_INDEX]
                immed[save_active_lm, 0]

                local_csr_wr[BYTE_INDEX, start_loc]

                alu[extract_temp, start_loc, +, save_active_lm]
                local_csr_wr[ACTIVE_LM_ADDR_/**/_EXTRACT_SRC_LMEM_INDEX, extract_temp]
                //need 3 cyles to settle
                nop
                nop
                nop

                /////////////LMEM WORK-AROUND////////////////////////////////
                ;copy source in temp registers because we can't increment lmem
                ;pointer.
                #if (DATA_SPREAD == 1)
                    #ifdef _LINKED_SRC_BUF
                        #define_eval _BYTES (_EXTRACT_SRC_XBUF_SIZE - number_of_bytes)
                        #define_eval  _INDEX (_EXTRACT_SRC_LMEM_INDEX + 1)

                        alu[extract_temp, start_loc, -, /**/_BYTES]
                        ble[case_3_lm#], defer[2]
                        alu[tmp_src1, --, b, *l$index/**/_EXTRACT_SRC_LMEM_INDEX]
                        alu[tmp_src2, --, b, *l$index/**/_INDEX]
                        alu[tmp_src2, --, b, /**/_LINKED_SRC_BUF[0]]

                        #undef _BYTES
                    #else
                        #warning "xbuf_extract_frm_linked_bufs: DATA_SPREAD = 1 but source buffer is not linked"
                    #endif  //if _LINKED_SRC_BUF
                #else
                    alu[tmp_src1, --, b, *l$index/**/_EXTRACT_SRC_LMEM_INDEX++]
                    alu[tmp_src2, --, b, *l$index/**/_EXTRACT_SRC_LMEM_INDEX]
                #endif //ifdef DATA_SPREAD == 1

                /////////////END LMEM WORK-AROUND////////////////////////////////
case_3_lm#:
                //restore active LM value
                local_csr_wr[ACTIVE_LM_ADDR_/**/_EXTRACT_SRC_LMEM_INDEX, save_active_lm]
                //need 3 cyles to settle

                //get 4 bytes into extract_temp
                byte_align_be[--, tmp_src1]
                byte_align_be[extract_temp, tmp_src2]

                alu_shf[out_byte_field, --, B, extract_temp, >>_SHF_VAL]

            .end

        #else   //end extract from GPRs

            .reg xbuf_offset

            #ifdef  _LINKED_SRC_BUF
                #define_eval    _next_xbuf      _LINKED_SRC_BUF
            #else
                //if there is no next buffer, define _next_xbuf to
                //_END_XBUF_CHAIN
                #define_eval    _next_xbuf      _END_XBUF_CHAIN
            #endif

            #if (DATA_SPREAD == 0)
                _EXTRACT_INSERT_JUMP_TO_ENTRY(out_byte_field, \
                            str_xbuf_name, start_loc, number_of_bytes, _EXTRACT_OR_INSERT)
            #else
                alu[xbuf_offset, start_loc, -, _EXTRACT_SRC_XBUF_SIZE]

                bge[check_next_xbuf_case_3#], defer[1]

                _EXTRACT_INSERT_JUMP_TO_ENTRY(out_byte_field, str_xbuf_name, start_loc, number_of_bytes, _EXTRACT_OR_INSERT)

            check_next_xbuf_case_3#:
                //continue onto linked buffers, if there is any.
                #if (!streq(_next_xbuf, _END_XBUF_CHAIN))
                    // find the start element in buffer
                    _find_actual_xbuf(out_buf_num, _next_xbuf, xbuf_offset)
                    #if (defined(__NFP_INDIRECT_REF_FORMAT_NFPXXXX) && (_EXTRACT_SRC_XBUF_TYPE == _XBUF_SRAM_XFER))
                        #error "This case is not supported here."
                    #endif

                    #error "This case is not yet supported."

                    .if (out_buf_num == 0)
                        _EXTRACT_INSERT_JUMP_TO_ENTRY(out_byte_field, _start_buf_name,
                                                        xbuf_offset, number_of_bytes, _EXTRACT_OR_INSERT)
                    .elif (out_buf_num == 1)
                        _EXTRACT_INSERT_JUMP_TO_ENTRY(out_byte_field, _next_buf1_name,
                                                        xbuf_offset, number_of_bytes, _EXTRACT_OR_INSERT)
                    .elif (out_buf_num == 2)
                        _EXTRACT_INSERT_JUMP_TO_ENTRY(out_byte_field, _next_buf2_name,
                                                        xbuf_offset, number_of_bytes, _EXTRACT_OR_INSERT)
                    .else
                        _EXTRACT_INSERT_JUMP_TO_ENTRY(out_byte_field, _next_buf3_name,
                                                        xbuf_offset, number_of_bytes, _EXTRACT_OR_INSERT)
                    .endif

                    // done with nextxx_name, undefine them
                    #undef  _start_buf_name
                    #undef  _next_buf1_name
                    #undef  _next_buf2_name
                    #undef  _next_buf3_name
                    #undef  _next_xbuf
                #endif //if (!streq(_next_xbuf, _END_XBUF_CHAIN))
            #endif //DATA_SPREAD == 0
        #endif //extract from xfer regs

        #undef _SHF_VAL

    #else
    ;case 4: offsets and size are all run-time values

        alu[start_loc, window_start, +, field_start]

        #if ((_EXTRACT_SRC_XBUF_TYPE == _XBUF_SRAM_XFER) || \
                (_EXTRACT_SRC_XBUF_TYPE == _XBUF_DRAM_XFER))
        .begin

            .reg extract_shf
            alu_shf[out_buf_num, --, B, &str_xbuf_name[0], <<2]

            #if (DATA_SPREAD == 1)
                #ifdef _LINKED_SRC_BUF
                    alu[extract_temp, _EXTRACT_SRC_XBUF_SIZE, -, number_of_bytes]
                    alu[extract_temp, start_loc, -, extract_temp]
                    ble[case_4_found_reg#], defer[1]

                    alu[--, extract_temp, -, number_of_bytes]
                    bge[case_4_found_reg#], defer[2]

                    alu[start_loc, start_loc, -, /**/_EXTRACT_SRC_XBUF_SIZE]
                    alu_shf[out_buf_num, --, B, &/**/_LINKED_SRC_BUF/**/[0], <<2]

                    .begin

                        .reg extract_mask extract_temp2

                        #define_eval _INDEX ((_EXTRACT_SRC_XBUF_SIZE/4) - 1)

                        alu[out_byte_field, --, B, str_xbuf_name[/**/_INDEX] ]
                        alu[extract_temp2, --, B, /**/_LINKED_SRC_BUF/**/[0]]

                        alu[extract_temp, --, b, extract_temp, <<3]
                        alu[extract_temp, 32, -, extract_temp]

                        alu[extract_temp, extract_temp, OR, 0]
                        dbl_shf[out_byte_field, out_byte_field, extract_temp2, >>indirect]

                        alu[extract_temp, 4, -, number_of_bytes]
                        beq[end_extract#], defer[2]

                        alu[extract_temp, --, B, extract_temp, <<3]
                        alu[extract_temp, extract_temp, OR, 0]
                        alu[out_byte_field, extract_temp, b, out_byte_field, <<indirect]
                        alu[out_byte_field, --, b, out_byte_field, >>indirect]
                        br[end_extract#]

                        #undef _INDEX

                    .end
                #else  //LINKED_SRC_BUF
                    #warning "xbuf_extract_frm_linked_bufs: DATA_SPREAD = 1 but source buffer is not linked"
                #endif
            #endif //DATA_SPREAD == 1

    case_4_found_reg#:
        .begin

            br=ctx[0, extract_frm_linked_buf_case4_skip_find_ctx#]

            //if not context 0, need to add offset to absolute reg to start_loc
            _find_absolute_xfer_reg_addr(out_buf_num)

        extract_frm_linked_buf_case4_skip_find_ctx#:

            alu[start_loc, start_loc, +, out_buf_num]
            local_csr_wr[T_INDEX_BYTE_INDEX, start_loc]
            //need 3 cyles before T_INDEX_BYTE_INDEX can be used
            alu[extract_shf, 4, -, number_of_bytes]
            alu_shf[extract_shf, --, B, extract_shf, <<3]
            nop
            //get 4 bytes into extract_temp
            byte_align_be[--, _EXTRACT_INDEX/**/++]
            byte_align_be[extract_temp, _EXTRACT_INDEX]

            alu[--, extract_shf, OR, 0]
            alu_shf[out_byte_field, --, B, extract_temp, >>indirect]

        .end

        .end

        #elif (_EXTRACT_SRC_XBUF_TYPE == _XBUF_LMEM)

            .begin

                .reg save_active_lm  tmp_src1 tmp_src2 extract_shf

                local_csr_rd[ACTIVE_LM_ADDR_/**/_EXTRACT_SRC_LMEM_INDEX]
                immed[save_active_lm, 0]

                local_csr_wr[BYTE_INDEX, start_loc]

                alu[start_loc, start_loc, +, save_active_lm]
                local_csr_wr[ACTIVE_LM_ADDR_/**/_EXTRACT_SRC_LMEM_INDEX, start_loc]
                //need 3 cyles to settle
                alu[extract_shf, 4, -, number_of_bytes]
                alu_shf[extract_shf, --, B, extract_shf, <<3]
                nop

                /////////////LMEM WORK-AROUND////////////////////////////////
                ;copy source in temp registers because we can't increment lmem
                ;pointer
                #if (DATA_SPREAD == 1)
                    #ifdef _LINKED_SRC_BUF
                        #define_eval  _INDEX (_EXTRACT_SRC_LMEM_INDEX + 1)

                        alu[extract_temp, _EXTRACT_SRC_XBUF_SIZE, - , number_of_bytes]
                        alu[extract_temp, start_loc, -, extract_temp]
                        ble[case_4_lm#], defer[2]
                        alu[tmp_src1, --, b, *l$index/**/_EXTRACT_SRC_LMEM_INDEX]
                        alu[tmp_src2, --, b, *l$index/**/_INDEX]
                        alu[tmp_src2, --, b, /**/_LINKED_SRC_BUF[0]]

                    #else
                        #warning "xbuf_extract_frm_linked_bufs: DATA_SPREAD = 1 but source buffer is not linked"
                    #endif
                #else
                    alu[tmp_src1, --, b, *l$index/**/_EXTRACT_SRC_LMEM_INDEX++]
                    alu[tmp_src2, --, b, *l$index/**/_EXTRACT_SRC_LMEM_INDEX]
                #endif //DATA_SPREAD == 1

                /////////////END LMEM WORK-AROUND////////////////////////////////
case_4_lm#:
                //restore active LM value
                local_csr_wr[ACTIVE_LM_ADDR_/**/_EXTRACT_SRC_LMEM_INDEX, save_active_lm]
                //need 3 cyles to settle

                //get 4 bytes into extract_temp
                byte_align_be[--, tmp_src1]
                byte_align_be[extract_temp, tmp_src2]

                alu[--, extract_shf, OR, 0]
                alu_shf[out_byte_field, --, B, extract_temp, >>indirect]

            .end

        #else   //extract from GPRs

            .reg xbuf_offset

            #ifdef  _LINKED_SRC_BUF
                #define_eval    _next_xbuf      _LINKED_SRC_BUF
            #else
                //if there is no next buffer, define _next_xbuf to
                //_END_XBUF_CHAIN
                #define_eval    _next_xbuf      _END_XBUF_CHAIN
            #endif

            #if (DATA_SPREAD == 0)
                _EXTRACT_INSERT_FIND_SIZE_THEN_JUMP_TO_ENTRY(out_byte_field, \
                            str_xbuf_name, start_loc, number_of_bytes, _EXTRACT_OR_INSERT)
            #else
                alu[xbuf_offset, start_loc, -, _EXTRACT_SRC_XBUF_SIZE]

                bge[check_next_xbuf_case_4#], defer[1]

                _EXTRACT_INSERT_FIND_SIZE_THEN_JUMP_TO_ENTRY(out_byte_field, \
                                str_xbuf_name, start_loc, number_of_bytes, _EXTRACT_OR_INSERT)
    check_next_xbuf_case_4#:
                //continue onto linked buffers, if there is any.
                #if (!streq(_next_xbuf, _END_XBUF_CHAIN))
                    // find the start element in buffer
                    _find_actual_xbuf(out_buf_num, _next_xbuf, xbuf_offset)

                    #if (defined(__NFP_INDIRECT_REF_FORMAT_NFPXXXX) && (_EXTRACT_SRC_XBUF_TYPE == _XBUF_SRAM_XFER))
                        #error "This case is not supported here."
                    #endif

                    #error "This case is not yet supported."

                    .if (out_buf_num == 0)
                        _EXTRACT_INSERT_FIND_SIZE_THEN_JUMP_TO_ENTRY(out_byte_field, _start_buf_name,
                                                xbuf_offset, number_of_bytes, _EXTRACT_OR_INSERT)
                    .elif (out_buf_num == 1)
                        _EXTRACT_INSERT_FIND_SIZE_THEN_JUMP_TO_ENTRY(out_byte_field, _next_buf1_name,
                                                xbuf_offset, number_of_bytes, _EXTRACT_OR_INSERT)
                    .elif (out_buf_num == 2)
                        _EXTRACT_INSERT_FIND_SIZE_THEN_JUMP_TO_ENTRY(out_byte_field, _next_buf2_name,
                                                xbuf_offset, number_of_bytes, _EXTRACT_OR_INSERT)
                    .else
                        _EXTRACT_INSERT_FIND_SIZE_THEN_JUMP_TO_ENTRY(out_byte_field, _next_buf3_name,
                                                        xbuf_offset, number_of_bytes, _EXTRACT_OR_INSERT)
                    .endif

                    // done with nextxx_name, undefine them
                    #undef  _start_buf_name
                    #undef  _next_buf1_name
                    #undef  _next_buf2_name
                    #undef  _next_buf3_name
                    #undef  _next_xbuf
                #endif //if (!streq(_next_xbuf, _END_XBUF_CHAIN))
            #endif //DATA_SPREAD == 0
        #endif //extract from xfer regs

    #endif  //end case 4

end_extract#:
    .use str_xbuf_name[0]
    #ifdef  _LINKED_SRC_BUF
        .use _LINKED_SRC_BUF[0]
    #endif

    #undef _EXTRACT_SRC_XBUF_TYPE
    #ifdef _EXTRACT_SRC_XBUF_SIZE
        #undef _EXTRACT_SRC_XBUF_SIZE
    #endif

    #ifdef  _LINKED_SRC_BUF
        #undef  _LINKED_SRC_BUF
    #endif

    #ifdef _EXTRACT_INDEX
        #undef _EXTRACT_INDEX
    #endif

    #ifdef _EXTRACT_SRC_LMEM_INDEX
        #undef _EXTRACT_SRC_LMEM_INDEX
    #endif

.end
#endm


/// @cond INTERNAL_MACROS
/** Extracts or inserts bytes in str_xbuf_name depending on @p _EXTRACT_OR_INSERT.
 *
 * @param byte_field            GPR output
 * @param str_xbuf_name         Name of sram read transfer register
 * @param xbuf_offset           Offset within the array
 * @param NUMBER_OF_BYTES       Number of bytes
 * @param _EXTRACT_OR_INSERT    Possible values:
 *                              @arg @c _EXTRACT (1)     Extract operation
 *                              @arg @c _INSERT  (2)     Insert operation
 *
 * For SRAM, DRAM, or GPR buffers, the maximum buffer size is 16 registers.
 * Therefore, the xbuf_offset range is [0..63].
 * For Local Memory buffer, the maximum buffer size is 8 registers.
 * Therefore, the xbuf_offset range is [0..31]
 *
 * For this macro number_of_bytes is constant.
 */
#macro _EXTRACT_INSERT_JUMP_TO_ENTRY(byte_field, str_xbuf_name, xbuf_offset, \
                                    NUMBER_OF_BYTES, _EXTRACT_OR_INSERT)
.begin

    #if (!(is_ct_const(NUMBER_OF_BYTES) || isimport(NUMBER_OF_BYTES)))
        #error 2 "_EXTRACT_INSERT_JUMP_TO_ENTRY: number_of_bytes must be a constant."
    #endif

    #if ((_EXTRACT_OR_INSERT != _EXTRACT) && (_EXTRACT_OR_INSERT != _INSERT))
        #error 2 "_EXTRACT_OR_INSERT: must have value 1 (_EXTRACT) or 2 (_INSERT)."
    #endif

    .reg element code_entry_offset byte_start

    _GET_XBUF_TYPE(str_xbuf_name)
    #ifdef _EXTRACT_INSERT_JUMP_TO_ENTRY_STR_XBUF_TYPE
        #warning "_EXTRACT_INSERT_JUMP_TO_ENTRY_STR_XBUF_TYPE is being redefined."
    #endif
    #define_eval _EXTRACT_INSERT_JUMP_TO_ENTRY_STR_XBUF_TYPE _XBUF_TYPE
    #undef _XBUF_TYPE

    _DEFINE_THIS_XBUF(str_xbuf_name)
    #define_eval _SRC_XBUF_SIZE     _this_xbuf_size
    _UNDEFINE_THIS_XBUF()



    #if (defined(__NFP_INDIRECT_REF_FORMAT_NFPXXXX) && (_EXTRACT_INSERT_JUMP_TO_ENTRY_STR_XBUF_TYPE == _XBUF_SRAM_XFER))
        alu[element, 32, ~AND, xbuf_offset, >>2]  ?????
        jump[element, jmp_tbl1#], defer[3], targets[t_entry_0#, t_entry_1#, t_entry_2#, \
        t_entry_3#, t_entry_4#, t_entry_5#, t_entry_6#, t_entry_7#, t_entry_8#, t_entry_9#, \
        t_entry_10#, t_entry_11#, t_entry_12#, t_entry_13#, t_entry_14#, t_entry_15#, \
        t_entry_16#, t_entry_17#, t_entry_18#, t_entry_19#, t_entry_20#, t_entry_21#, \
        t_entry_22#, t_entry_23#, t_entry_24#, t_entry_25#, t_entry_26#, t_entry_27#, \
        t_entry_28#, t_entry_29#, t_entry_30#, t_entry_31#]
    #else
        alu[element, 16, ~AND, xbuf_offset, >>2]
        jump[element, jmp_tbl1#], defer[3], targets[t_entry_0#, t_entry_1#, t_entry_2#, \
        t_entry_3#, t_entry_4#, t_entry_5#, t_entry_6#, t_entry_7#, t_entry_8#, t_entry_9#, \
        t_entry_10#, t_entry_11#, t_entry_12#, t_entry_13#, t_entry_14#, t_entry_15#]
    #endif

    alu_shf[code_entry_offset, --, B, element, <<2]
    ; the ALU instruction below CAN'T be moved. It must be the LAST instruction that
    ; changes the condition code before entering the code at each table entry
    alu[byte_start, xbuf_offset, -, code_entry_offset]
    nop


//  Start of jump table 1.
jmp_tbl1#:
t_entry_0#:
    br[code_entry_0#]
t_entry_1#:
    #if (_SRC_XBUF_SIZE > 1)
        br[code_entry_1#]
    #endif
t_entry_2#:
    #if (_SRC_XBUF_SIZE > 2)
        br[code_entry_2#]
    #endif
t_entry_3#:
    #if (_SRC_XBUF_SIZE > 3)
        br[code_entry_3#]
    #endif
t_entry_4#:
    #if (_SRC_XBUF_SIZE > 4)
        br[code_entry_4#]
    #endif
t_entry_5#:
    #if (_SRC_XBUF_SIZE > 5)
        br[code_entry_5#]
    #endif
t_entry_6#:
    #if (_SRC_XBUF_SIZE > 6)
        br[code_entry_6#]
    #endif
t_entry_7#:
    #if (_SRC_XBUF_SIZE > 7)
        br[code_entry_7#]
    #endif
t_entry_8#:
    #if (_SRC_XBUF_SIZE > 8)
        br[code_entry_8#]
    #endif
t_entry_9#:
    #if (_SRC_XBUF_SIZE > 9)
        br[code_entry_9#]
    #endif
t_entry_10#:
    #if (_SRC_XBUF_SIZE > 10)
        br[code_entry_10#]
    #endif
t_entry_11#:
    #if (_SRC_XBUF_SIZE > 11)
        br[code_entry_11#]
    #endif
t_entry_12#:
    #if (_SRC_XBUF_SIZE > 12)
        br[code_entry_12#]
    #endif
t_entry_13#:
    #if (_SRC_XBUF_SIZE > 13)
        br[code_entry_13#]
    #endif
t_entry_14#:
    #if (_SRC_XBUF_SIZE > 14)
        br[code_entry_14#]
    #endif
t_entry_15#:
    #if (_SRC_XBUF_SIZE > 15)
        br[code_entry_15#]
    #endif

#if (defined(__NFP_INDIRECT_REF_FORMAT_NFPXXXX) && (_EXTRACT_INSERT_JUMP_TO_ENTRY_STR_XBUF_TYPE == _XBUF_SRAM_XFER))
    t_entry_16#:
        #if (_SRC_XBUF_SIZE > 16)
            br[code_entry_16#]
        #endif
    t_entry_17#:
        #if (_SRC_XBUF_SIZE > 17)
            br[code_entry_17#]
        #endif
    t_entry_18#:
        #if (_SRC_XBUF_SIZE > 18)
            br[code_entry_18#]
        #endif
    t_entry_19#:
        #if (_SRC_XBUF_SIZE > 19)
            br[code_entry_19#]
        #endif
    t_entry_20#:
        #if (_SRC_XBUF_SIZE > 20)
            br[code_entry_20#]
        #endif
    t_entry_21#:
        #if (_SRC_XBUF_SIZE > 21)
            br[code_entry_21#]
        #endif
    t_entry_22#:
        #if (_SRC_XBUF_SIZE > 22)
            br[code_entry_22#]
        #endif
    t_entry_23#:
        #if (_SRC_XBUF_SIZE > 23)
            br[code_entry_23#]
        #endif
    t_entry_24#:
        #if (_SRC_XBUF_SIZE > 24)
            br[code_entry_24#]
        #endif
    t_entry_25#:
        #if (_SRC_XBUF_SIZE > 25)
            br[code_entry_25#]
        #endif
    t_entry_26#:
        #if (_SRC_XBUF_SIZE > 26)
            br[code_entry_26#]
        #endif
    t_entry_27#:
        #if (_SRC_XBUF_SIZE > 27)
            br[code_entry_27#]
        #endif
    t_entry_28#:
        #if (_SRC_XBUF_SIZE > 28)
            br[code_entry_28#]
        #endif
    t_entry_29#:
        #if (_SRC_XBUF_SIZE > 29)
            br[code_entry_29#]
        #endif
    t_entry_30#:
        #if (_SRC_XBUF_SIZE > 30)
            br[code_entry_30#]
        #endif
    t_entry_31#:
        #if (_SRC_XBUF_SIZE > 31)
            br[code_entry_31#]
        #endif
#endif

//  Start of jump table 1.
code_entry_0#:

        beq[r0_b0#], defer [1]
        alu[--, byte_start, -, 1]
        beq[r0_b1#], defer [1]
        alu[--, byte_start, -, 2]
        beq[r0_b2#], defer [1]
        alu[--, byte_start, -, 3]
        beq[r0_b3#]

        #define_eval i  0
        #while (i < 4)
    r0_b/**/i#:
            #if (_EXTRACT_OR_INSERT == _EXTRACT)
                _xbuf_extract_const_offsets_and_size(byte_field, str_xbuf_name, 0, /**/i, NUMBER_OF_BYTES)
                br[end_extract#]
            #else
                _xbuf_insert_const_offsets_and_size(str_xbuf_name, byte_field, 0, /**/i, NUMBER_OF_BYTES)
                br[end_insert#]
            #endif

            #define_eval    i   (i+1)
        #endloop

code_entry_1#:
    #if (_SRC_XBUF_SIZE > 1)
        beq[r1_b0#], defer [1]
        alu[--, byte_start, -, 1]
        beq[r1_b1#], defer [1]
        alu[--, byte_start, -, 2]
        beq[r1_b2#], defer [1]
        alu[--, byte_start, -, 3]
        beq[r1_b3#]

        #define_eval i  0
        #while (i < 4)
    r1_b/**/i#:
            #if (_EXTRACT_OR_INSERT == _EXTRACT)
                _xbuf_extract_const_offsets_and_size(byte_field, str_xbuf_name, 4, /**/i, NUMBER_OF_BYTES)
                br[end_extract#]
            #else
                _xbuf_insert_const_offsets_and_size(str_xbuf_name, byte_field, 4, /**/i, NUMBER_OF_BYTES)
                br[end_insert#]
            #endif

            #define_eval    i   (i+1)
        #endloop

    #endif //_SRC_XBUF_SIZE

code_entry_2#:
    #if (_SRC_XBUF_SIZE > 2)
        beq[r2_b0#], defer [1]
        alu[--, byte_start, -, 1]
        beq[r2_b1#], defer [1]
        alu[--, byte_start, -, 2]
        beq[r2_b2#], defer [1]
        alu[--, byte_start, -, 3]
        beq[r2_b3#]

        #define_eval i  0
        #while (i < 4)
    r2_b/**/i#:
            #if (_EXTRACT_OR_INSERT == _EXTRACT)
                _xbuf_extract_const_offsets_and_size(byte_field, str_xbuf_name, 8, /**/i, NUMBER_OF_BYTES)
                br[end_extract#]
            #else
                _xbuf_insert_const_offsets_and_size(str_xbuf_name, byte_field, 8, /**/i, NUMBER_OF_BYTES)
                br[end_insert#]
            #endif

            #define_eval    i   (i+1)
        #endloop

    #endif //_SRC_XBUF_SIZE

code_entry_3#:
    #if (_SRC_XBUF_SIZE > 3)
        beq[r3_b0#], defer [1]
        alu[--, byte_start, -, 1]
        beq[r3_b1#], defer [1]
        alu[--, byte_start, -, 2]
        beq[r3_b2#], defer [1]
        alu[--, byte_start, -, 3]
        beq[r3_b3#]

        #define_eval i  0
        #while (i < 4)
    r3_b/**/i#:
            #if (_EXTRACT_OR_INSERT == _EXTRACT)
                _xbuf_extract_const_offsets_and_size(byte_field, str_xbuf_name, 12, /**/i, NUMBER_OF_BYTES)
                br[end_extract#]
            #else
                _xbuf_insert_const_offsets_and_size(str_xbuf_name, byte_field, 12, /**/i, NUMBER_OF_BYTES)
                br[end_insert#]
            #endif

            #define_eval    i   (i+1)
        #endloop

    #endif //_SRC_XBUF_SIZE

code_entry_4#:
    #if (_SRC_XBUF_SIZE > 4)
        beq[r4_b0#], defer [1]
        alu[--, byte_start, -, 1]
        beq[r4_b1#], defer [1]
        alu[--, byte_start, -, 2]
        beq[r4_b2#], defer [1]
        alu[--, byte_start, -, 3]
        beq[r4_b3#]

        #define_eval i  0
        #while (i < 4)
    r4_b/**/i#:
            #if (_EXTRACT_OR_INSERT == _EXTRACT)
                _xbuf_extract_const_offsets_and_size(byte_field, str_xbuf_name, 16, /**/i, NUMBER_OF_BYTES)
                br[end_extract#]
            #else
                _xbuf_insert_const_offsets_and_size(str_xbuf_name, byte_field, 16, /**/i, NUMBER_OF_BYTES)
                br[end_insert#]
            #endif

            #define_eval    i   (i+1)
        #endloop
    #endif  //_SRC_XBUF_SIZE

code_entry_5#:
    #if (_SRC_XBUF_SIZE > 5)
        beq[r5_b0#], defer [1]
        alu[--, byte_start, -, 1]
        beq[r5_b1#], defer [1]
        alu[--, byte_start, -, 2]
        beq[r5_b2#], defer [1]
        alu[--, byte_start, -, 3]
        beq[r5_b3#]

        #define_eval i  0
        #while (i < 4)
    r5_b/**/i#:
            #if (_EXTRACT_OR_INSERT == _EXTRACT)
                _xbuf_extract_const_offsets_and_size(byte_field, str_xbuf_name, 20, /**/i, NUMBER_OF_BYTES)
                br[end_extract#]
            #else
                _xbuf_insert_const_offsets_and_size(str_xbuf_name, byte_field, 20, /**/i, NUMBER_OF_BYTES)
                br[end_insert#]
            #endif

            #define_eval    i   (i+1)
        #endloop

    #endif //_SRC_XBUF_SIZE

code_entry_6#:
    #if (_SRC_XBUF_SIZE > 6)
        beq[r6_b0#], defer [1]
        alu[--, byte_start, -, 1]
        beq[r6_b1#], defer [1]
        alu[--, byte_start, -, 2]
        beq[r6_b2#], defer [1]
        alu[--, byte_start, -, 3]
        beq[r6_b3#]

        #define_eval i  0
        #while (i < 4)
    r6_b/**/i#:
            #if (_EXTRACT_OR_INSERT == _EXTRACT)
                _xbuf_extract_const_offsets_and_size(byte_field, str_xbuf_name, 24, /**/i, NUMBER_OF_BYTES)
                br[end_extract#]
            #else
                _xbuf_insert_const_offsets_and_size(str_xbuf_name, byte_field, 24, /**/i, NUMBER_OF_BYTES)
                br[end_insert#]
            #endif

            #define_eval    i   (i+1)
        #endloop

    #endif //_SRC_XBUF_SIZE

code_entry_7#:
    #if (_SRC_XBUF_SIZE > 7)
        beq[r7_b0#], defer [1]
        alu[--, byte_start, -, 1]
        beq[r7_b1#], defer [1]
        alu[--, byte_start, -, 2]
        beq[r7_b2#], defer [1]
        alu[--, byte_start, -, 3]
        beq[r7_b3#]

        #define_eval i  0
        #while (i < 4)
    r7_b/**/i#:
            #if (_EXTRACT_OR_INSERT == _EXTRACT)
                _xbuf_extract_const_offsets_and_size(byte_field, str_xbuf_name, 28, /**/i, NUMBER_OF_BYTES)
                br[end_extract#]
            #else
                _xbuf_insert_const_offsets_and_size(str_xbuf_name, byte_field, 28, /**/i, NUMBER_OF_BYTES)
                br[end_insert#]
            #endif

            #define_eval    i   (i+1)
        #endloop

    #endif  //_SRC_XBUF_SIZE

code_entry_8#:
    #if (_SRC_XBUF_SIZE > 8)
        beq[r8_b0#], defer [1]
        alu[--, byte_start, -, 1]
        beq[r8_b1#], defer [1]
        alu[--, byte_start, -, 2]
        beq[r8_b2#], defer [1]
        alu[--, byte_start, -, 3]
        beq[r8_b3#]

        #define_eval i  0
        #while (i < 4)
    r8_b/**/i#:
            #if (_EXTRACT_OR_INSERT == _EXTRACT)
                _xbuf_extract_const_offsets_and_size(byte_field, str_xbuf_name, 32, /**/i, NUMBER_OF_BYTES)
                br[end_extract#]
            #else
                _xbuf_insert_const_offsets_and_size(str_xbuf_name, byte_field, 32, /**/i, NUMBER_OF_BYTES)
                br[end_insert#]
            #endif

            #define_eval    i   (i+1)
        #endloop

    #endif //_SRC_XBUF_SIZE

code_entry_9#:
    #if (_SRC_XBUF_SIZE > 9)
        beq[r9_b0#], defer [1]
        alu[--, byte_start, -, 1]
        beq[r9_b1#], defer [1]
        alu[--, byte_start, -, 2]
        beq[r9_b2#], defer [1]
        alu[--, byte_start, -, 3]
        beq[r9_b3#]

        #define_eval i  0
        #while (i < 4)
    r9_b/**/i#:
            #if (_EXTRACT_OR_INSERT == _EXTRACT)
                _xbuf_extract_const_offsets_and_size(byte_field, str_xbuf_name, 36, /**/i, NUMBER_OF_BYTES)
                br[end_extract#]
            #else
                _xbuf_insert_const_offsets_and_size(str_xbuf_name, byte_field, 36, /**/i, NUMBER_OF_BYTES)
                br[end_insert#]
            #endif

            #define_eval    i   (i+1)
        #endloop

    #endif //_SRC_XBUF_SIZE

code_entry_10#:
    #if (_SRC_XBUF_SIZE > 10)
        beq[r10_b0#], defer [1]
        alu[--, byte_start, -, 1]
        beq[r10_b1#], defer [1]
        alu[--, byte_start, -, 2]
        beq[r10_b2#], defer [1]
        alu[--, byte_start, -, 3]
        beq[r10_b3#]

        #define_eval i  0
        #while (i < 4)
    r10_b/**/i#:
            #if (_EXTRACT_OR_INSERT == _EXTRACT)
                _xbuf_extract_const_offsets_and_size(byte_field, str_xbuf_name, 40, /**/i, NUMBER_OF_BYTES)
                br[end_extract#]
            #else
                _xbuf_insert_const_offsets_and_size(str_xbuf_name, byte_field, 40, /**/i, NUMBER_OF_BYTES)
                br[end_insert#]
            #endif

            #define_eval    i   (i+1)
        #endloop

    #endif //_SRC_XBUF_SIZE

code_entry_11#:
    #if (_SRC_XBUF_SIZE > 11)
        beq[r11_b0#], defer [1]
        alu[--, byte_start, -, 1]
        beq[r11_b1#], defer [1]
        alu[--, byte_start, -, 2]
        beq[r11_b2#], defer [1]
        alu[--, byte_start, -, 3]
        beq[r11_b3#]

        #define_eval i  0
        #while (i < 4)
    r11_b/**/i#:
            #if (_EXTRACT_OR_INSERT == _EXTRACT)
                _xbuf_extract_const_offsets_and_size(byte_field, str_xbuf_name, 44, /**/i, NUMBER_OF_BYTES)
                br[end_extract#]
            #else
                _xbuf_insert_const_offsets_and_size(str_xbuf_name, byte_field, 44, /**/i, NUMBER_OF_BYTES)
                br[end_insert#]
            #endif

            #define_eval    i   (i+1)
        #endloop

    #endif  //_SRC_XBUF_SIZE

code_entry_12#:
    #if (_SRC_XBUF_SIZE > 12)
        beq[r12_b0#], defer [1]
        alu[--, byte_start, -, 1]
        beq[r12_b1#], defer [1]
        alu[--, byte_start, -, 2]
        beq[r12_b2#], defer [1]
        alu[--, byte_start, -, 3]
        beq[r12_b3#]

        #define_eval i  0
        #while (i < 4)
    r12_b/**/i#:
            #if (_EXTRACT_OR_INSERT == _EXTRACT)
                _xbuf_extract_const_offsets_and_size(byte_field, str_xbuf_name, 48, /**/i, NUMBER_OF_BYTES)
                br[end_extract#]
            #else
                _xbuf_insert_const_offsets_and_size(str_xbuf_name, byte_field, 48, /**/i, NUMBER_OF_BYTES)
                br[end_insert#]
            #endif

            #define_eval    i   (i+1)
        #endloop

    #endif  //_SRC_XBUF_SIZE

code_entry_13#:
    #if (_SRC_XBUF_SIZE > 13)
        beq[r13_b0#], defer [1]
        alu[--, byte_start, -, 1]
        beq[r13_b1#], defer [1]
        alu[--, byte_start, -, 2]
        beq[r13_b2#], defer [1]
        alu[--, byte_start, -, 3]
        beq[r13_b3#]

        #define_eval i  0
        #while (i < 4)
    r13_b/**/i#:
            #if (_EXTRACT_OR_INSERT == _EXTRACT)
                _xbuf_extract_const_offsets_and_size(byte_field, str_xbuf_name, 52, /**/i, NUMBER_OF_BYTES)
                br[end_extract#]
            #else
                _xbuf_insert_const_offsets_and_size(str_xbuf_name, byte_field, 52, /**/i, NUMBER_OF_BYTES)
                br[end_insert#]
            #endif

            #define_eval    i   (i+1)
        #endloop

    #endif  //_SRC_XBUF_SIZE

code_entry_14#:
    #if (_SRC_XBUF_SIZE > 14)
        beq[r14_b0#], defer [1]
        alu[--, byte_start, -, 1]
        beq[r14_b1#], defer [1]
        alu[--, byte_start, -, 2]
        beq[r14_b2#], defer [1]
        alu[--, byte_start, -, 3]
        beq[r14_b3#]

        #define_eval i  0
        #while (i < 4)
    r14_b/**/i#:
            #if (_EXTRACT_OR_INSERT == _EXTRACT)
                _xbuf_extract_const_offsets_and_size(byte_field, str_xbuf_name, 56, /**/i, NUMBER_OF_BYTES)
                br[end_extract#]
            #else
                _xbuf_insert_const_offsets_and_size(str_xbuf_name, byte_field, 56, /**/i, NUMBER_OF_BYTES)
                br[end_insert#]
            #endif

            #define_eval    i   (i+1)
        #endloop

    #endif  //_SRC_XBUF_SIZE

code_entry_15#:
    #if (_SRC_XBUF_SIZE > 15)
        beq[r15_b0#], defer [1]
        alu[--, byte_start, -, 1]
        beq[r15_b1#], defer [1]
        alu[--, byte_start, -, 2]
        beq[r15_b2#], defer [1]
        alu[--, byte_start, -, 3]
        beq[r15_b3#]

        #define_eval i  0
        #while (i < 4)
    r15_b/**/i#:
            #if (_EXTRACT_OR_INSERT == _EXTRACT)
                _xbuf_extract_const_offsets_and_size(byte_field, str_xbuf_name, 60, /**/i, NUMBER_OF_BYTES)
                br[end_extract#]
            #else
                _xbuf_insert_const_offsets_and_size(str_xbuf_name, byte_field, 60, /**/i, NUMBER_OF_BYTES)
                br[end_insert#]
            #endif

            #define_eval    i   (i+1)
        #endloop

    #endif  //_SRC_XBUF_SIZE

    #if (defined(__NFP_INDIRECT_REF_FORMAT_NFPXXXX) && (_EXTRACT_INSERT_JUMP_TO_ENTRY_STR_XBUF_TYPE == _XBUF_SRAM_XFER))

    code_entry_16#:
        #if (_SRC_XBUF_SIZE > 16)
            beq[r16_b0#], defer [1]
            alu[--, byte_start, -, 1]
            beq[r16_b1#], defer [1]
            alu[--, byte_start, -, 2]
            beq[r16_b2#], defer [1]
            alu[--, byte_start, -, 3]
            beq[r16_b3#]

            #define_eval i  0
            #while (i < 4)
    r16_b/**/i#:
                #if (_EXTRACT_OR_INSERT == _EXTRACT)
                    _xbuf_extract_const_offsets_and_size(byte_field, str_xbuf_name, 64, /**/i, NUMBER_OF_BYTES)
                    br[end_extract#]
                #else
                    _xbuf_insert_const_offsets_and_size(str_xbuf_name, byte_field, 64, /**/i, NUMBER_OF_BYTES)
                    br[end_insert#]
                #endif

                #define_eval    i   (i+1)
            #endloop

        #endif  //_SRC_XBUF_SIZE

    code_entry_17#:
        #if (_SRC_XBUF_SIZE > 17)
            beq[r17_b0#], defer [1]
            alu[--, byte_start, -, 1]
            beq[r17_b1#], defer [1]
            alu[--, byte_start, -, 2]
            beq[r17_b2#], defer [1]
            alu[--, byte_start, -, 3]
            beq[r17_b3#]

            #define_eval i  0
            #while (i < 4)
    r17_b/**/i#:
                #if (_EXTRACT_OR_INSERT == _EXTRACT)
                    _xbuf_extract_const_offsets_and_size(byte_field, str_xbuf_name, 68, /**/i, NUMBER_OF_BYTES)
                    br[end_extract#]
                #else
                    _xbuf_insert_const_offsets_and_size(str_xbuf_name, byte_field, 68, /**/i, NUMBER_OF_BYTES)
                    br[end_insert#]
                #endif

                #define_eval    i   (i+1)
            #endloop

        #endif  //_SRC_XBUF_SIZE

    code_entry_18#:
        #if (_SRC_XBUF_SIZE > 18)
            beq[r18_b0#], defer [1]
            alu[--, byte_start, -, 1]
            beq[r18_b1#], defer [1]
            alu[--, byte_start, -, 2]
            beq[r18_b2#], defer [1]
            alu[--, byte_start, -, 3]
            beq[r18_b3#]

            #define_eval i  0
            #while (i < 4)
    r18_b/**/i#:
                #if (_EXTRACT_OR_INSERT == _EXTRACT)
                    _xbuf_extract_const_offsets_and_size(byte_field, str_xbuf_name, 72, /**/i, NUMBER_OF_BYTES)
                    br[end_extract#]
                #else
                    _xbuf_insert_const_offsets_and_size(str_xbuf_name, byte_field, 72, /**/i, NUMBER_OF_BYTES)
                    br[end_insert#]
                #endif

                #define_eval    i   (i+1)
            #endloop

        #endif  //_SRC_XBUF_SIZE

    code_entry_19#:
        #if (_SRC_XBUF_SIZE > 19)
            beq[r19_b0#], defer [1]
            alu[--, byte_start, -, 1]
            beq[r19_b1#], defer [1]
            alu[--, byte_start, -, 2]
            beq[r19_b2#], defer [1]
            alu[--, byte_start, -, 3]
            beq[r19_b3#]

            #define_eval i  0
            #while (i < 4)
    r19_b/**/i#:
                #if (_EXTRACT_OR_INSERT == _EXTRACT)
                    _xbuf_extract_const_offsets_and_size(byte_field, str_xbuf_name, 76, /**/i, NUMBER_OF_BYTES)
                    br[end_extract#]
                #else
                    _xbuf_insert_const_offsets_and_size(str_xbuf_name, byte_field, 76, /**/i, NUMBER_OF_BYTES)
                    br[end_insert#]
                #endif

                #define_eval    i   (i+1)
            #endloop

        #endif  //_SRC_XBUF_SIZE

    code_entry_20#:
        #if (_SRC_XBUF_SIZE > 20)
            beq[r20_b0#], defer [1]
            alu[--, byte_start, -, 1]
            beq[r20_b1#], defer [1]
            alu[--, byte_start, -, 2]
            beq[r20_b2#], defer [1]
            alu[--, byte_start, -, 3]
            beq[r20_b3#]

            #define_eval i  0
            #while (i < 4)
    r20_b/**/i#:
                #if (_EXTRACT_OR_INSERT == _EXTRACT)
                    _xbuf_extract_const_offsets_and_size(byte_field, str_xbuf_name, 80, /**/i, NUMBER_OF_BYTES)
                    br[end_extract#]
                #else
                    _xbuf_insert_const_offsets_and_size(str_xbuf_name, byte_field, 80, /**/i, NUMBER_OF_BYTES)
                    br[end_insert#]
                #endif

                #define_eval    i   (i+1)
            #endloop

        #endif  //_SRC_XBUF_SIZE

    code_entry_21#:
        #if (_SRC_XBUF_SIZE > 21)
            beq[r21_b0#], defer [1]
            alu[--, byte_start, -, 1]
            beq[r21_b1#], defer [1]
            alu[--, byte_start, -, 2]
            beq[r21_b2#], defer [1]
            alu[--, byte_start, -, 3]
            beq[r21_b3#]

            #define_eval i  0
            #while (i < 4)
    r21_b/**/i#:
                #if (_EXTRACT_OR_INSERT == _EXTRACT)
                    _xbuf_extract_const_offsets_and_size(byte_field, str_xbuf_name, 84, /**/i, NUMBER_OF_BYTES)
                    br[end_extract#]
                #else
                    _xbuf_insert_const_offsets_and_size(str_xbuf_name, byte_field, 84, /**/i, NUMBER_OF_BYTES)
                    br[end_insert#]
                #endif

                #define_eval    i   (i+1)
            #endloop

        #endif  //_SRC_XBUF_SIZE

    code_entry_22#:
        #if (_SRC_XBUF_SIZE > 22)
            beq[r22_b0#], defer [1]
            alu[--, byte_start, -, 1]
            beq[r22_b1#], defer [1]
            alu[--, byte_start, -, 2]
            beq[r22_b2#], defer [1]
            alu[--, byte_start, -, 3]
            beq[r22_b3#]

            #define_eval i  0
            #while (i < 4)
    r22_b/**/i#:
                #if (_EXTRACT_OR_INSERT == _EXTRACT)
                    _xbuf_extract_const_offsets_and_size(byte_field, str_xbuf_name, 88, /**/i, NUMBER_OF_BYTES)
                    br[end_extract#]
                #else
                    _xbuf_insert_const_offsets_and_size(str_xbuf_name, byte_field, 88, /**/i, NUMBER_OF_BYTES)
                    br[end_insert#]
                #endif

                #define_eval    i   (i+1)
            #endloop

        #endif  //_SRC_XBUF_SIZE

    code_entry_23#:
        #if (_SRC_XBUF_SIZE > 23)
            beq[r23_b0#], defer [1]
            alu[--, byte_start, -, 1]
            beq[r23_b1#], defer [1]
            alu[--, byte_start, -, 2]
            beq[r23_b2#], defer [1]
            alu[--, byte_start, -, 3]
            beq[r23_b3#]

            #define_eval i  0
            #while (i < 4)
    r23_b/**/i#:
                #if (_EXTRACT_OR_INSERT == _EXTRACT)
                    _xbuf_extract_const_offsets_and_size(byte_field, str_xbuf_name, 92, /**/i, NUMBER_OF_BYTES)
                    br[end_extract#]
                #else
                    _xbuf_insert_const_offsets_and_size(str_xbuf_name, byte_field, 92, /**/i, NUMBER_OF_BYTES)
                    br[end_insert#]
                #endif

                #define_eval    i   (i+1)
            #endloop

        #endif  //_SRC_XBUF_SIZE

    code_entry_24#:
        #if (_SRC_XBUF_SIZE > 24)
            beq[r24_b0#], defer [1]
            alu[--, byte_start, -, 1]
            beq[r24_b1#], defer [1]
            alu[--, byte_start, -, 2]
            beq[r24_b2#], defer [1]
            alu[--, byte_start, -, 3]
            beq[r24_b3#]

            #define_eval i  0
            #while (i < 4)
    r24_b/**/i#:
                #if (_EXTRACT_OR_INSERT == _EXTRACT)
                    _xbuf_extract_const_offsets_and_size(byte_field, str_xbuf_name, 96, /**/i, NUMBER_OF_BYTES)
                    br[end_extract#]
                #else
                    _xbuf_insert_const_offsets_and_size(str_xbuf_name, byte_field, 96, /**/i, NUMBER_OF_BYTES)
                    br[end_insert#]
                #endif

                #define_eval    i   (i+1)
            #endloop

        #endif  //_SRC_XBUF_SIZE

    code_entry_25#:
        #if (_SRC_XBUF_SIZE > 25)
            beq[r25_b0#], defer [1]
            alu[--, byte_start, -, 1]
            beq[r25_b1#], defer [1]
            alu[--, byte_start, -, 2]
            beq[r25_b2#], defer [1]
            alu[--, byte_start, -, 3]
            beq[r25_b3#]

            #define_eval i  0
            #while (i < 4)
    r25_b/**/i#:
                #if (_EXTRACT_OR_INSERT == _EXTRACT)
                    _xbuf_extract_const_offsets_and_size(byte_field, str_xbuf_name, 100, /**/i, NUMBER_OF_BYTES)
                    br[end_extract#]
                #else
                    _xbuf_insert_const_offsets_and_size(str_xbuf_name, byte_field, 100, /**/i, NUMBER_OF_BYTES)
                    br[end_insert#]
                #endif

                #define_eval    i   (i+1)
            #endloop

        #endif  //_SRC_XBUF_SIZE

    code_entry_26#:
        #if (_SRC_XBUF_SIZE > 26)
            beq[r26_b0#], defer [1]
            alu[--, byte_start, -, 1]
            beq[r26_b1#], defer [1]
            alu[--, byte_start, -, 2]
            beq[r26_b2#], defer [1]
            alu[--, byte_start, -, 3]
            beq[r26_b3#]

            #define_eval i  0
            #while (i < 4)
    r26_b/**/i#:
                #if (_EXTRACT_OR_INSERT == _EXTRACT)
                    _xbuf_extract_const_offsets_and_size(byte_field, str_xbuf_name, 104, /**/i, NUMBER_OF_BYTES)
                    br[end_extract#]
                #else
                    _xbuf_insert_const_offsets_and_size(str_xbuf_name, byte_field, 104, /**/i, NUMBER_OF_BYTES)
                    br[end_insert#]
                #endif

                #define_eval    i   (i+1)
            #endloop

        #endif  //_SRC_XBUF_SIZE

    code_entry_27#:
        #if (_SRC_XBUF_SIZE > 27)
            beq[r27_b0#], defer [1]
            alu[--, byte_start, -, 1]
            beq[r27_b1#], defer [1]
            alu[--, byte_start, -, 2]
            beq[r27_b2#], defer [1]
            alu[--, byte_start, -, 3]
            beq[r27_b3#]

            #define_eval i  0
            #while (i < 4)
    r27_b/**/i#:
                #if (_EXTRACT_OR_INSERT == _EXTRACT)
                    _xbuf_extract_const_offsets_and_size(byte_field, str_xbuf_name, 108, /**/i, NUMBER_OF_BYTES)
                    br[end_extract#]
                #else
                    _xbuf_insert_const_offsets_and_size(str_xbuf_name, byte_field, 108, /**/i, NUMBER_OF_BYTES)
                    br[end_insert#]
                #endif

                #define_eval    i   (i+1)
            #endloop

        #endif  //_SRC_XBUF_SIZE

    code_entry_28#:
        #if (_SRC_XBUF_SIZE > 28)
            beq[r28_b0#], defer [1]
            alu[--, byte_start, -, 1]
            beq[r28_b1#], defer [1]
            alu[--, byte_start, -, 2]
            beq[r28_b2#], defer [1]
            alu[--, byte_start, -, 3]
            beq[r28_b3#]

            #define_eval i  0
            #while (i < 4)
    r28_b/**/i#:
                #if (_EXTRACT_OR_INSERT == _EXTRACT)
                    _xbuf_extract_const_offsets_and_size(byte_field, str_xbuf_name, 112, /**/i, NUMBER_OF_BYTES)
                    br[end_extract#]
                #else
                    _xbuf_insert_const_offsets_and_size(str_xbuf_name, byte_field, 112, /**/i, NUMBER_OF_BYTES)
                    br[end_insert#]
                #endif

                #define_eval    i   (i+1)
            #endloop

        #endif  //_SRC_XBUF_SIZE

    code_entry_29#:
        #if (_SRC_XBUF_SIZE > 29)
            beq[r29_b0#], defer [1]
            alu[--, byte_start, -, 1]
            beq[r29_b1#], defer [1]
            alu[--, byte_start, -, 2]
            beq[r29_b2#], defer [1]
            alu[--, byte_start, -, 3]
            beq[r29_b3#]

            #define_eval i  0
            #while (i < 4)
    r29_b/**/i#:
                #if (_EXTRACT_OR_INSERT == _EXTRACT)
                    _xbuf_extract_const_offsets_and_size(byte_field, str_xbuf_name, 116, /**/i, NUMBER_OF_BYTES)
                    br[end_extract#]
                #else
                    _xbuf_insert_const_offsets_and_size(str_xbuf_name, byte_field, 116, /**/i, NUMBER_OF_BYTES)
                    br[end_insert#]
                #endif

                #define_eval    i   (i+1)
            #endloop

        #endif  //_SRC_XBUF_SIZE

    code_entry_30#:
        #if (_SRC_XBUF_SIZE > 30)
            beq[r30_b0#], defer [1]
            alu[--, byte_start, -, 1]
            beq[r30_b1#], defer [1]
            alu[--, byte_start, -, 2]
            beq[r30_b2#], defer [1]
            alu[--, byte_start, -, 3]
            beq[r30_b3#]

            #define_eval i  0
            #while (i < 4)
    r30_b/**/i#:
                #if (_EXTRACT_OR_INSERT == _EXTRACT)
                    _xbuf_extract_const_offsets_and_size(byte_field, str_xbuf_name, 120, /**/i, NUMBER_OF_BYTES)
                    br[end_extract#]
                #else
                    _xbuf_insert_const_offsets_and_size(str_xbuf_name, byte_field, 120, /**/i, NUMBER_OF_BYTES)
                    br[end_insert#]
                #endif

                #define_eval    i   (i+1)
            #endloop

        #endif  //_SRC_XBUF_SIZE

    code_entry_31#:
        #if (_SRC_XBUF_SIZE > 31)
            beq[r31_b0#], defer [1]
            alu[--, byte_start, -, 1]
            beq[r31_b1#], defer [1]
            alu[--, byte_start, -, 2]
            beq[r31_b2#], defer [1]
            alu[--, byte_start, -, 3]
            beq[r31_b3#]

            #define_eval i  0
            #while (i < 4)
    r31_b/**/i#:
                #if (_EXTRACT_OR_INSERT == _EXTRACT)
                    _xbuf_extract_const_offsets_and_size(byte_field, str_xbuf_name, 124, /**/i, NUMBER_OF_BYTES)
                    br[end_extract#]
                #else
                    _xbuf_insert_const_offsets_and_size(str_xbuf_name, byte_field, 124, /**/i, NUMBER_OF_BYTES)
                    br[end_insert#]
                #endif

                #define_eval    i   (i+1)
            #endloop

        #endif  //_SRC_XBUF_SIZE

    #endif

    #undef _SRC_XBUF_SIZE
    #undef i

    #undef _EXTRACT_INSERT_JUMP_TO_ENTRY_STR_XBUF_TYPE

.end
#endm
/// @endcond


/// @cond INTERNAL_MACROS
/** Extracts or inserts bytes in str_xbuf_name depending on the _EXTRACT_OR_INSERT flag.
 *
 * @param byte_field           GPR output
 * @param str_xbuf_name        Name of sram read transfer register
 * @param xbuf_offset          Offset within the array
 * @param number_of_bytes      Number of bytes
 * @param _EXTRACT_OR_INSERT   @arg _EXTRACT (1)     Extract operation
 *                             @arg _INSERT  (2)     Insert operation
 *
 * For SRAM, DRAM, or GPR buffers, the maximum buffer size is 16 registers.
 * Therefore, the xbuf_offset range is [0..63].
 * For Local Memory buffer, the maximum buffer size is 8 registers.
 * Therefore, the xbuf_offset range is [0..31]
 *
 * For this macro number_of_bytes is constant.
 *
 * @instruction_cnt 0
 */
#macro _EXTRACT_INSERT_FIND_SIZE_THEN_JUMP_TO_ENTRY(byte_field, str_xbuf_name, \
                xbuf_offset, number_of_bytes, _EXTRACT_OR_INSERT)
.begin

    .if (number_of_bytes==1)
        _EXTRACT_INSERT_JUMP_TO_ENTRY(byte_field, str_xbuf_name, xbuf_offset, 1, _EXTRACT_OR_INSERT)
    .elif (number_of_bytes==2)
        _EXTRACT_INSERT_JUMP_TO_ENTRY(byte_field, str_xbuf_name, xbuf_offset, 2, _EXTRACT_OR_INSERT)
    .elif (number_of_bytes==3)
        _EXTRACT_INSERT_JUMP_TO_ENTRY(byte_field, str_xbuf_name, xbuf_offset, 3, _EXTRACT_OR_INSERT)
    .elif (number_of_bytes==4)
        _EXTRACT_INSERT_JUMP_TO_ENTRY(byte_field, str_xbuf_name, xbuf_offset, 4, _EXTRACT_OR_INSERT)
    .endif

.end
#endm
/// @endcond


/// @cond INTERNAL_MACROS
/** An internal macro used by xbuf_extract and xbuf_insert to set the constants.
 *
 * @param out_byte_field    Extracted bytes will get stored in this register
 * @param str_xbuf_name     Name of first sram transfer buffer
 * @param window_start      Start byte position of the window to extract from.
 *                          This is offset from beginning of buffer to
 *                          location of window to extract from
 * @param field_start       Byte offset of field to be extracted
 *                          starting from window_start. @n
 *                          window_start + field_start -> exact byte location
 *                          of the first byte to extract
 * @param number_of_bytes   Number of bytes to extract. Because out_byte_field
 *                          is a 32-bit GPR, maximum number of bytes to
 *                          extract is 4.
 *
 * @instruction_cnt 8-10
 */
#macro _xbuf_extract_const_offsets_and_size(out_byte_field, str_xbuf_name, window_start, field_start, number_of_bytes)
.begin

    .reg temp_reg_0 temp_reg_1

    #if (!((is_ct_const(window_start) || isimport(window_start)) && \
        (is_ct_const(field_start) || isimport(field_start)) && \
        (is_ct_const(number_of_bytes) || isimport(number_of_bytes))) )
        #error 2 "xbuf_extract_const_offsets_and_size: called with non-constant input(s)"
    #endif

    _GET_XBUF_TYPE(str_xbuf_name)
    #define_eval _EXTRACT_XBUF_TYPE _XBUF_TYPE
    #undef _XBUF_TYPE

    _find_xbuf_and_relative_sb_eb(str_xbuf_name, window_start, field_start, number_of_bytes)

    #if (_EXTRACT_XBUF_TYPE == _XBUF_LMEM)
        #if (_WORD0 == _WORD1)
            #if ( ((window_start + field_start) / 4) > 7)
            ;if index of buffer > 7, must move to a GPR since ld_field
            ;doesn't allow index > 7
                #ifdef _str_word0_xfer
                    alu[temp_reg_0, --, B, _str_word0_xfer]
                    #define_eval _str_word0_xfer temp_reg_0
                #endif
            #endif
        #else
            #if ( ((window_start + field_start) / 4) > 6)
            ;if index of buffer > 6, (which means word0 maybe at 6 and word0 maybe at 7),
            ;must move to a GPR since ld_field doesn't allow index > 7
                #ifdef _str_word0_xfer
                    alu[temp_reg_0, --, B, _str_word0_xfer]
                    #define_eval _str_word0_xfer temp_reg_0
                #endif
                #ifdef _str_word1_xfer
                    alu[temp_reg_1, --, B, _str_word1_xfer]
                    #define_eval _str_word1_xfer temp_reg_1
                #endif
            #endif
        #endif
    #endif

    #define_eval _SBYTE_IN_WD (_RELATIVE_SBYTE & 0x3)
    #define_eval _EBYTE_IN_WD (_RELATIVE_EBYTE & 0x3)


    #if (_WORD0 == _WORD1)

        #ifdef _str_word0_xfer

            #if (_EXTRACT_XBUF_TYPE == _XBUF_GPR)
                #ifndef _EXTRACT_FROM_BANK_B_GPR
                    ;move source to bank B before calling bytefield_extract
                    ;because bytefield extract requires source of the extract
                    ;be on bank B
                    alu[_str_word0_xfer, --, B, _str_word0_xfer]
                #endif
            #endif


            bytefield_extract(out_byte_field, _str_word0_xfer, _SBYTE_IN_WD, \
                                _EBYTE_IN_WD, NO_LOAD_CC)
        #endif //_str_word0_xfer

    #else  //(_WORD0 == _WORD1)

        #ifdef _str_word0_xfer
            #ifdef _str_word1_xfer

                #if (_EXTRACT_XBUF_TYPE == _XBUF_GPR)
                    #ifndef _EXTRACT_FROM_BANK_B_GPR
                        ;move source to bank B before calling bytefield_extract
                        ;because bytefield extract requires source of the extract
                        ;be on bank B
                        alu[_str_word0_xfer, --, B, _str_word0_xfer]
                        alu[_str_word1_xfer, --, B, _str_word1_xfer]
                    #endif
                #endif

                bytefield_dbl_extract(out_byte_field, _str_word0_xfer, \
                                _SBYTE_IN_WD, _str_word1_xfer, _EBYTE_IN_WD, NO_LOAD_CC)

            #endif //ifdef _str_word0_xfer
        #endif //ifdef _str_word1_xfer

    #endif



    #undef _SBYTE_IN_WD
    #undef _EBYTE_IN_WD
    #undef _EXTRACT_XBUF_TYPE
    _clear_name_space_for_find_xbuf_and_relative_sb_eb()

.end
#endm
/// @endcond


/** Extract a specified number of bytes from xbuf.
 *
 * Perform endian swap if global #define LITTLE_ENDIAN.
 *
 * @param out_field       GPR containing field, right justified
 * @param str_xbuf_name   Name of sram transfer register buffer
 * @param WINDOW_START    Start position (in datatype increments) of window or header
 * @param FIELD_START     Start offset (in datatype increments) of field from window start
 * @param SIZE            Size of field
 * @param DATATYPE        Datatype of field
 *
 * @b Example:
 * @code
 * xbuf_alloc(wbuf, 8, read_write)
 *
 * #define_eval WIN_START      0
 * #define_eval FIELD_START    0
 * #define_eval NUM_OF_BYTES   1
 *
 * immed32(wbuf[0], 0x01020304)
 *
 * xbuf_type_extract(out_byte_field, wbuf, WIN_START, FIELD_START, NUM_OF_BYTES, BIG_ENDIAN_BYTES)
 * //out_byte_field should be equal to 1
 * @endcode
 *
 * @instruction_cnt 8-10
 */
#macro xbuf_type_extract(out_field, str_xbuf_name, WINDOW_START, FIELD_START, SIZE, DATATYPE)
    #if (DATATYPE == BIG_ENDIAN_BYTES)
        #ifdef LITTLE_ENDIAN
            #undef LITTLE_ENDIAN
            xbuf_extract(out_field, str_xbuf_name, WINDOW_START, FIELD_START, SIZE)
            #define LITTLE_ENDIAN
        #else
            xbuf_extract(out_field, str_xbuf_name, WINDOW_START, FIELD_START, SIZE)
        #endif

    #elif (DATATYPE == LITTLE_ENDIAN_BYTES)
        #ifndef LITTLE_ENDIAN
            #define LITTLE_ENDIAN
            xbuf_extract(out_field, str_xbuf_name, WINDOW_START, FIELD_START, SIZE)
            #undef LITTLE_ENDIAN
        #else
            xbuf_extract(out_field, str_xbuf_name, WINDOW_START, FIELD_START, SIZE)
        #endif
    #endif
#endm


/** Insert specified bytes of a numeric byte field into xbuf buffer at the
 * specified start byte offset.
 *
 * @param io_str_xbuf_name  Name of xbuf register where byte field is
 *                          to be inserted
 * @param in_byte_field    GPR, read transfer register or local memory register that
 *                         contains byte field to be inserted.
 * @param window_start     Start byte position of window to insert to.
 *                         This is offset from beginning of output
 *                         buffer to location of window to insert into.
 * @param field_start      Byte offset of field to be inserted
 *                         starting from @p window_start. @n
 *                         @p window_start + @p field_start -> exact byte location
 *                         of the first byte to insert
 * @param number_of_bytes  Number of bytes to insert. Because in_byte_field
 *                         is a 32-bit register, maximum number of bytes is 4.
 *
 * @note
 * -# No error checking in order to minimize the number of
 *    instruction. If users try to insert more than 4 bytes or
 *    pass in an field-offset that is bigger than the buffer size,
 *    the result will be undefined.
 * -# If the source register is GPRs, users can define a compile option
 *    _INSERT_BYTE_FROM_BANK_B_GPR if they know for sure that the source
 *    is in bank B. When defined, this option makes this macro
 *    skip moving the source register to bank B, hence save 1 instruction.
 *
 * <b>Example usage:</b>
 * @code
 * xbuf_alloc($$io_xbuf_dest, 8, read_write)
 * ...
 * move(in_byte_field, 0x12345678)
 * //Insert in_byte_field into io_xbuf_dest starting at byte offset 12
 * xbuf_insert($io_xbuf_dest, in_byte_field, 0, 12, 4)
 * ...
 * @endcode
 *
 * @instruction_cnt (1 * number of words to insert) for constant offsets and size
 */
#macro xbuf_insert(io_str_xbuf_name, in_byte_field, window_start, field_start, \
                    number_of_bytes)
.begin

    .reg insert_temp start_loc xbuf_offset out_buf_num save_active_lm

    #define_eval    _EXTRACT_OR_INSERT  _INSERT

    _GET_XBUF_TYPE(io_str_xbuf_name)
    #define_eval _INSERT_SRC_XBUF_TYPE _XBUF_TYPE
    #undef _XBUF_TYPE

    #if (_INSERT_SRC_XBUF_TYPE == _XBUF_SRAM_XFER)
        #define_eval    _INDEX  *$index
    #elif ( (!defined(__NFP_INDIRECT_REF_FORMAT_NFPXXXX)) && (_INSERT_SRC_XBUF_TYPE == _XBUF_DRAM_XFER) )
        #define_eval    _INDEX  *$$index
    #endif

    _DEFINE_THIS_XBUF(io_str_xbuf_name)
    //size of buffer in bytes
    #define_eval _INSERT_SRC_XBUF_SIZE  (_this_xbuf_size * 4)
    #ifdef _this_xbuf_next
        #define_eval    _LINKED_SRC_BUF _this_xbuf_next
    #endif
    #if (_INSERT_SRC_XBUF_TYPE == _XBUF_LMEM)
        #define_eval _INSERT_SRC_LMEM_INDEX _this_xbuf_index
    #endif
    _UNDEFINE_THIS_XBUF()

    #if ((is_ct_const(window_start) || isimport(window_start)) && \
        (is_ct_const(field_start) || isimport(field_start)) && \
        (is_ct_const(number_of_bytes) || isimport(number_of_bytes)))
        ;case 1: all offsets and size are constants
        _xbuf_insert_const_offsets_and_size(io_str_xbuf_name, in_byte_field, window_start, \
                                            field_start, number_of_bytes)

    #elif ((is_ct_const(window_start) || isimport(window_start)) && \
            (is_ct_const(field_start) || isimport(field_start)))
        ;case 2: offsets are constants, size is run-time value

        ; no error checking to save instruction. If users want to insert more than
        ; 4 bytes, result is undefined

        alu[insert_temp, number_of_bytes, AND~ , 4]
        beq[insert_4_bytes#]
        alu[--, insert_temp, -, 1]
        beq[insert_1_byte#]
        alu[--, insert_temp, -, 2]
        beq[insert_2_bytes#]

        br[insert_3_bytes#]

        insert_1_byte#:
            _xbuf_insert_const_offsets_and_size(io_str_xbuf_name, in_byte_field, \
                                            window_start, field_start, 1)
            br[end_insert#]
        insert_2_bytes#:
            _xbuf_insert_const_offsets_and_size(io_str_xbuf_name, in_byte_field, \
                                            window_start, field_start, 2)
            br[end_insert#]
        insert_3_bytes#:
            _xbuf_insert_const_offsets_and_size(io_str_xbuf_name, in_byte_field, \
                                            window_start, field_start, 3)
            br[end_insert#]
        insert_4_bytes#:
            _xbuf_insert_const_offsets_and_size(io_str_xbuf_name, in_byte_field, \
                                            window_start, field_start, 4)
            br[end_insert#]

    #elif (is_ct_const(number_of_bytes) || isimport(number_of_bytes))
    ;case 3: offsets are run-time values, size is constant

        alu[start_loc, field_start, +, window_start]

        #if ((_INSERT_SRC_XBUF_TYPE == _XBUF_SRAM_XFER) || (_INSERT_SRC_XBUF_TYPE == _XBUF_DRAM_XFER))

            br=ctx[0, insert_case3_skip_find_abs_addr#], defer[2]
            add_shf_left(start_loc, start_loc, &io_str_xbuf_name[0], 2)

            //if not context 0, need to add offset to absolute reg to start_loc
            _find_absolute_xfer_reg_addr(start_loc)

insert_case3_skip_find_abs_addr#:

            local_csr_wr[T_INDEX_BYTE_INDEX, start_loc]
            //need 3 cyles before T_INDEX_BYTE_INDEX can be used

            _insert_to_xfer_or_lmem_buf_const_size(io_str_xbuf_name, \
                    in_byte_field, window_start, field_start, number_of_bytes)

        #elif (_INSERT_SRC_XBUF_TYPE == _XBUF_LMEM)

            local_csr_rd[ACTIVE_LM_ADDR_/**/_INSERT_SRC_LMEM_INDEX]
            immed[save_active_lm, 0]

            local_csr_wr[BYTE_INDEX, start_loc]

            alu[start_loc, start_loc, +, save_active_lm]
            local_csr_wr[ACTIVE_LM_ADDR_/**/_INSERT_SRC_LMEM_INDEX, start_loc]
            ;need 3 cyles to before can access local memory

            _insert_to_xfer_or_lmem_buf_const_size(io_str_xbuf_name, \
                in_byte_field, window_start, field_start, number_of_bytes)


        #else   //end insert to xfer regs

            ; no error checking to save instruction. If users want to read more than
            ; buffer size, result is undefined

            _EXTRACT_INSERT_JUMP_TO_ENTRY(in_byte_field, io_str_xbuf_name, start_loc, \
                                                number_of_bytes, _EXTRACT_OR_INSERT)
            br[end_insert#]

        #endif

    #else
    ;case 4: offsets and size are all run-time values

        alu[start_loc, field_start, +, window_start]
        #if ((_INSERT_SRC_XBUF_TYPE == _XBUF_SRAM_XFER) || (_INSERT_SRC_XBUF_TYPE == _XBUF_DRAM_XFER))

            br=ctx[0, insert_case4_skip_find_abs_addr#], defer[2]
            add_shf_left(start_loc, start_loc, &io_str_xbuf_name[0], 2)

            //if not context 0, need to add offset to absolute reg to start_loc
            _find_absolute_xfer_reg_addr(start_loc)

insert_case4_skip_find_abs_addr#:

            local_csr_wr[T_INDEX_BYTE_INDEX, start_loc]
            //need 3 cyles before T_INDEX_BYTE_INDEX can be used
        #elif (_INSERT_SRC_XBUF_TYPE == _XBUF_LMEM)
            local_csr_rd[ACTIVE_LM_ADDR_/**/_INSERT_SRC_LMEM_INDEX]
            immed[save_active_lm, 0]

            local_csr_wr[BYTE_INDEX, start_loc]

            alu[start_loc, start_loc, +, save_active_lm]
            local_csr_wr[ACTIVE_LM_ADDR_/**/_INSERT_SRC_LMEM_INDEX, start_loc]
            //need 3 cyles to before can access local memory

        #endif

        #if ( (_INSERT_SRC_XBUF_TYPE == _XBUF_SRAM_XFER) || (_INSERT_SRC_XBUF_TYPE == _XBUF_DRAM_XFER) \
            || (_INSERT_SRC_XBUF_TYPE == _XBUF_LMEM) )

            alu[--, number_of_bytes, -, 3]
            bne[xfer_regs_insert_check_2_bytes#]

            _insert_to_xfer_or_lmem_buf_const_size(io_str_xbuf_name, \
                        in_byte_field, window_start, field_start, 3)

xfer_regs_insert_check_2_bytes#:

            alu[--, number_of_bytes, -, 2]
            bne[xfer_regs_insert_check_1_byte#]

            _insert_to_xfer_or_lmem_buf_const_size(io_str_xbuf_name, \
                        in_byte_field, window_start, field_start, 2)

xfer_regs_insert_check_1_byte#:

            alu[--, number_of_bytes, -, 1]
            bne[xfer_regs_insert_check_4_bytes#]

            _insert_to_xfer_or_lmem_buf_const_size(io_str_xbuf_name, \
                        in_byte_field, window_start, field_start, 1)

xfer_regs_insert_check_4_bytes#:

            _insert_to_xfer_or_lmem_buf_const_size(io_str_xbuf_name, \
                        in_byte_field, window_start, field_start, 4)

        #else //if ((_INSERT_SRC_XBUF_TYPE == _XBUF_SRAM_XFER) || (_INSERT_SRC_XBUF_TYPE == _XBUF_DRAM_XFER))

            _EXTRACT_INSERT_FIND_SIZE_THEN_JUMP_TO_ENTRY(in_byte_field, io_str_xbuf_name,
                                                start_loc, number_of_bytes, _EXTRACT_OR_INSERT)
            br[end_insert#]
        #endif //if ((_INSERT_SRC_XBUF_TYPE == _XBUF_SRAM_XFER) || (_INSERT_SRC_XBUF_TYPE == _XBUF_DRAM_XFER))

    #endif  //end case 4

end_insert#:
    #if (  (!((is_ct_const(window_start) || isimport(window_start)) && \
            (is_ct_const(field_start) || isimport(field_start)))) && \
            (_INSERT_SRC_XBUF_TYPE == _XBUF_LMEM) )
        local_csr_wr[ACTIVE_LM_ADDR_/**/_INSERT_SRC_LMEM_INDEX, save_active_lm]
        nop
        nop
        nop
    #endif

    .use io_str_xbuf_name[0]
    #ifdef  _LINKED_SRC_BUF
        .use _LINKED_SRC_BUF[0]
    #endif

    #undef _INSERT_SRC_XBUF_TYPE
    #ifdef _INSERT_SRC_XBUF_SIZE
        #undef _INSERT_SRC_XBUF_SIZE
    #endif

    #ifdef  _LINKED_SRC_BUF
        #undef  _LINKED_SRC_BUF
    #endif

    #ifdef _INDEX
        #undef _INDEX
    #endif

    #ifdef _INSERT_SRC_LMEM_INDEX
        #undef _INSERT_SRC_LMEM_INDEX
    #endif

.end
#endm


/// @cond INTERNAL_MACROS
/** An internal macro used by xbuf_insert used to set the constants.
 *
 * @param io_str_xbuf_name   Name of first sram transfer buffer
 * @param in_byte_field      Input byte field
 * @param WINDOW_START       Start byte position of window to extract from.
 *                           This is offset from beginning of buffer to
 *                           location of window to extract from.
 * @param FIELD_START        Byte offset of field to be extracted
 *                           starting from window_start. @n
 *                           window_start + field_start -> exact byte location
 *                           of first byte to extract
 * @param NUMBER_OF_BYTES    Number of bytes to extract. Because out_byte_field
 *                           is a 32-bit GPR, maximum number of bytes to
 *                           extract is 4.
 *
 * @instruction_cnt 1-3
 */
#macro _xbuf_insert_const_offsets_and_size(io_str_xbuf_name, in_byte_field, \
        WINDOW_START, FIELD_START, NUMBER_OF_BYTES)
.begin

    #if (!((is_ct_const(WINDOW_START) || isimport(WINDOW_START)) && \
        (is_ct_const(FIELD_START) || isimport(FIELD_START)) && \
        (is_ct_const(NUMBER_OF_BYTES) || isimport(NUMBER_OF_BYTES))) )
        #error 2 "xbuf_insert_const_offsets_and_size: called with non-constant input(s)"
    #endif

    _GET_XBUF_TYPE(io_str_xbuf_name)
    #define_eval _INSERT_XBUF_TYPE _XBUF_TYPE
    #undef _XBUF_TYPE

    #if (_INSERT_XBUF_TYPE == _XBUF_GPR)
        #ifndef _INSERT_BYTE_FROM_BANK_B_GPR
            ;move in_byte_field to bank B before performing insert
            ;because bytefield_xx_insert operation requires source of
            ;the insert be on bank B
            alu[in_byte_field, --, B, in_byte_field]
        #endif
    #endif

    //_find_xbuf_and_relative_sb_eb(io_str_xbuf_name, WINDOW_START, FIELD_START, NUMBER_OF_BYTES)

    _perform_xbuf_insert(io_str_xbuf_name, in_byte_field, WINDOW_START, FIELD_START, \
                            NUMBER_OF_BYTES)

    //_clear_name_space_for_find_xbuf_and_relative_sb_eb()
    #undef _INSERT_XBUF_TYPE

.end
#endm
/// @endcond


/// @cond INTERNAL_MACROS
/** An internal macro used by xbuf_insert.
 */
#macro _insert_to_xfer_or_lmem_buf_const_size(io_str_xbuf_name, in_byte_field, \
        window_start, field_start, NUMBER_OF_BYTES)

.begin

    #if !(is_ct_const(NUMBER_OF_BYTES) || isimport(NUMBER_OF_BYTES) )
        #error 2 "_insert_to_xfer_or_lmem_buf_const_size: must have \
                    constant NUMBER_OF_BYTES"
    #endif

    alu[xbuf_offset, start_loc, AND, 3]

    alu[--, xbuf_offset, -, 0]
    bne[xfer_regs_insert_chk1#]

    #if ((_INSERT_SRC_XBUF_TYPE == _XBUF_SRAM_XFER) || (_INSERT_SRC_XBUF_TYPE == _XBUF_DRAM_XFER))
        _xfer_reg_insert_const_offsets_and_size(in_byte_field, 0, NUMBER_OF_BYTES)
    #elif (_INSERT_SRC_XBUF_TYPE == _XBUF_LMEM)
        _lmem_reg_insert_const_offsets_and_size(in_byte_field, 0, NUMBER_OF_BYTES)
    #endif

    br[end_insert#]

xfer_regs_insert_chk1#:
    alu[--, xbuf_offset, -, 1]
    bne[xfer_regs_insert_chk2#]
        #if (NUMBER_OF_BYTES <= 3)
            #if ((_INSERT_SRC_XBUF_TYPE == _XBUF_SRAM_XFER) || (_INSERT_SRC_XBUF_TYPE == _XBUF_DRAM_XFER))
                _xfer_reg_insert_const_offsets_and_size(in_byte_field, 1, NUMBER_OF_BYTES)
            #elif (_INSERT_SRC_XBUF_TYPE == _XBUF_LMEM)
                _lmem_reg_insert_const_offsets_and_size(in_byte_field, 1, NUMBER_OF_BYTES)
            #endif

        #else

            alu[insert_temp, --, B, in_byte_field, >>8]

            #if ((_INSERT_SRC_XBUF_TYPE == _XBUF_SRAM_XFER) || (_INSERT_SRC_XBUF_TYPE == _XBUF_DRAM_XFER))
                _xfer_reg_insert_const_offsets_and_size(insert_temp, 1, 3)
                alu[--, --, B, /**/_INDEX++]
                alu[insert_temp, --, B, in_byte_field, <<24]
                alu[insert_temp, --, B, insert_temp, >>24]
                _xfer_reg_insert_const_offsets_and_size(insert_temp, 0, 1)
            #elif  (_INSERT_SRC_XBUF_TYPE == _XBUF_LMEM)
                _lmem_reg_insert_const_offsets_and_size(insert_temp, 1, 3)
                alu[--, --, B, *l$index/**/_INSERT_SRC_LMEM_INDEX++]
                alu[insert_temp, --, B, in_byte_field, <<24]
                alu[insert_temp, --, B, insert_temp, >>24]
                _lmem_reg_insert_const_offsets_and_size(insert_temp, 0, 1)
            #endif

        #endif

        br[end_insert#]

xfer_regs_insert_chk2#:
    alu[--, xbuf_offset, -, 2]
    bne[xfer_regs_insert_chk3#]
        #if (NUMBER_OF_BYTES <= 2)
            #if ((_INSERT_SRC_XBUF_TYPE == _XBUF_SRAM_XFER) || (_INSERT_SRC_XBUF_TYPE == _XBUF_DRAM_XFER))
                _xfer_reg_insert_const_offsets_and_size(in_byte_field, 2, NUMBER_OF_BYTES)
            #elif  (_INSERT_SRC_XBUF_TYPE == _XBUF_LMEM)
                _lmem_reg_insert_const_offsets_and_size(in_byte_field, 2, NUMBER_OF_BYTES)
            #endif
        #else
            #define_eval _SHF_VAL21 ((NUMBER_OF_BYTES - 2) * 8)
            #define_eval _SHF_VAL22 ( (4-(NUMBER_OF_BYTES-2)) * 8)

            #if ((_INSERT_SRC_XBUF_TYPE == _XBUF_SRAM_XFER) || (_INSERT_SRC_XBUF_TYPE == _XBUF_DRAM_XFER))

                //insert the bytes into insert_temp
                alu[insert_temp, --, B, in_byte_field, >>/**/_SHF_VAL21]
                _xfer_reg_insert_const_offsets_and_size(insert_temp, 2, 2)
                alu[--, --, B, /**/_INDEX++]
                alu_shf[insert_temp, --, B, in_byte_field, <</**/_SHF_VAL22]
                alu_shf[insert_temp, --, B, insert_temp, >>/**/_SHF_VAL22]
                _xfer_reg_insert_const_offsets_and_size(insert_temp, \
                                                    0, (NUMBER_OF_BYTES-2))
            #elif  (_INSERT_SRC_XBUF_TYPE == _XBUF_LMEM)

                //insert the bytes into insert_temp
                alu[insert_temp, --, B, in_byte_field, >>/**/_SHF_VAL21]
                _lmem_reg_insert_const_offsets_and_size(insert_temp, 2, 2)
                alu[--, --, B, *l$index/**/_INSERT_SRC_LMEM_INDEX++]
                alu_shf[insert_temp, --, B, in_byte_field, <</**/_SHF_VAL22]
                alu_shf[insert_temp, --, B, insert_temp, >>/**/_SHF_VAL22]
                _lmem_reg_insert_const_offsets_and_size(insert_temp, \
                                                    0, (NUMBER_OF_BYTES-2))
            #endif

        #endif

        br[end_insert#]

xfer_regs_insert_chk3#:
    #if (NUMBER_OF_BYTES <= 1)
        #if ((_INSERT_SRC_XBUF_TYPE == _XBUF_SRAM_XFER) || (_INSERT_SRC_XBUF_TYPE == _XBUF_DRAM_XFER))
            _xfer_reg_insert_const_offsets_and_size(in_byte_field, 3, NUMBER_OF_BYTES)
        #elif  (_INSERT_SRC_XBUF_TYPE == _XBUF_LMEM)
            _lmem_reg_insert_const_offsets_and_size(in_byte_field, 3, NUMBER_OF_BYTES)
        #endif
    #else
        #define_eval _SHF_VAL31 ((NUMBER_OF_BYTES - 1) * 8)
        #define_eval _SHF_VAL32 ( (4-(NUMBER_OF_BYTES-1)) * 8)

        #if ((_INSERT_SRC_XBUF_TYPE == _XBUF_SRAM_XFER) || (_INSERT_SRC_XBUF_TYPE == _XBUF_DRAM_XFER))

            //insert the bytes into insert_temp
            alu[insert_temp, --, B, in_byte_field, >>/**/_SHF_VAL31]
            _xfer_reg_insert_const_offsets_and_size(insert_temp, 3, 1)
            alu[--, --, B, /**/_INDEX++]
            alu_shf[insert_temp, --, B, in_byte_field, <</**/_SHF_VAL32]
            alu_shf[insert_temp, --, B, insert_temp, >>/**/_SHF_VAL32]
            _xfer_reg_insert_const_offsets_and_size(insert_temp, \
                                            0, (NUMBER_OF_BYTES-1))
        #elif (_INSERT_SRC_XBUF_TYPE == _XBUF_LMEM)

            //insert the bytes into insert_temp
            alu[insert_temp, --, B, in_byte_field, >>/**/_SHF_VAL31]
            _lmem_reg_insert_const_offsets_and_size(insert_temp, 3, 1)
            alu[--, --, B, *l$index/**/_INSERT_SRC_LMEM_INDEX++]
            alu_shf[insert_temp, --, B, in_byte_field, <</**/_SHF_VAL32]
            alu_shf[insert_temp, --, B, insert_temp, >>/**/_SHF_VAL32]
            _lmem_reg_insert_const_offsets_and_size(insert_temp, \
                                            0, (NUMBER_OF_BYTES-1))
        #endif

    #endif

    br[end_insert#]

.end
#endm
/// @endcond


/// @cond INTERNAL_MACROS
/** An internal macro used by xbuf_insert.
 */
#macro _xfer_reg_insert_const_offsets_and_size(in_byte_field, OFFSET, NUMBER_OF_BYTES)
    #define_eval _EBYTE (OFFSET + NUMBER_OF_BYTES - 1)

    _GET_MASK_FROM_SB_EB(OFFSET, _EBYTE)
    _GET_LSHIFT_FROM_EB(_EBYTE)

    ld_field[/**/_INDEX, _SB_EB_MASK_VAL, in_byte_field, <</**/_L_SHIFT_VAL_FROM_EB], load_cc

    #undef _SB_EB_MASK_VAL
    #undef _L_SHIFT_VAL_FROM_EB
    #undef _EBYTE
#endm
/// @endcond


/// @cond INTERNAL_MACROS
/** An internal macro used by xbuf_insert.
 *
 * @instruction_cnt 1
 */
#macro _lmem_reg_insert_const_offsets_and_size(in_byte_field, OFFSET, NUMBER_OF_BYTES)
    #define_eval _EBYTE (OFFSET + NUMBER_OF_BYTES - 1)

    _GET_MASK_FROM_SB_EB(OFFSET, _EBYTE)
    _GET_LSHIFT_FROM_EB(_EBYTE)

    ld_field[*l$index/**/_INSERT_SRC_LMEM_INDEX, _SB_EB_MASK_VAL, in_byte_field, \
        <</**/_L_SHIFT_VAL_FROM_EB], load_cc


    #undef _SB_EB_MASK_VAL
    #undef _L_SHIFT_VAL_FROM_EB
    #undef _EBYTE
#endm
/// @endcond


/** XBUF Copy
 *
 *
 * @param out_dest_xbuf       Name of output buffer.
 *                            Can be SRAM write registers, DRAM write registers, local memory, or GPR.
 * @param out_last_element    GPR to contain last element of destination buffer.
 *                            This output is very useful when the destination buffer elements are write
 *                            transfer register and the copy results in an incompleted last element (not all
 *                            4 bytes are filled).
 *                            The need for @p out_last_element arises when users want to copy another buffer
 *                            into a paritally-filled destination buffer. Users may find that the last
 *                            long-word element in the destination is incompleted as the result of the previous
 *                            copy. In that case, they will need to pass the @p out_last_element of the previous
 *                            copy as the @p in_prepend to the current @p xbuf_copy. Otherwise, the bytes in
 *                            the partially-filled write register element will be cleared.
 *                            If users are sure that they do not need this value, they can pass the
 *                            constant 0 to save one instruction.
 * @param dest_start_byte     Absolute offset in bytes from beginning of output buffer to
 *                            location where copied data will reside.
 *                            Can be longword aligned or not (dest_start_byte % 4 = 0, 1, 2, or 3).
 *                            Can be constant or GPR.
 * @param in_src_xbuf         Name of input buffer.
 *                            Can be SRAM read registers, DRAM read registers, local memory, or GPR.
 *                            Data to be copied can not spread in several buffers.
 *                            In other words, all data has to be in @p in_src_xbuf. @n
 *                            In order to have most efficient copy (ie. done with least number of
 *                            instructions), @p in_src_xbuf should be buffer that actually contains
 *                            the data. It is fine if the data spreads beyond @p in_src_xbuf, but it will
 *                            take several more instructions if ALL the data does not reside in some
 *                            buffers that are linked to @p in_src_xbuf.
 * @param src_start_byte      Absolute offset in bytes from beginning of input buffer.
 *                            Can be longword aligned or not (src_start_byte % 4 = 0, 1, 2, or 3)
 *                            Can be constant or GPR.
 * @param in_prepend          GPR or constant containing bytes to be merged with first word
 *                            in destination, prepended, if byte alignment of destination is not 0.
 *                            Bytes to be merged must be at exact byte locations that they will
 *                            occupy in first word of destination. All other bytes must be 0.
 *                            In cases @p in_prepend is not needed, just pass constant 0.
 * @param total_bytes_to_copy Total number of bytes to copy. Can be constant or GPR.
 *                            Maximum number of bytes is dependent on maximum size of output buffer.
 *                            @arg If @p out_dest_xbuf is xfer registers or GPRs buffer: maximum size is 64 bytes.
 *                            @arg If @p out_dest_xbuf is local memory buffer: maximum size is 32 bytes.
 *
 * @param DATA_SPREAD         @arg 1:  If part of data to be copied spreads into buffer(s) linked to
 *                                     @p in_src_xbuf
 *                            @arg 0:  All data to be copied resides in @p in_src_xbuf. @n
 *                            DATA_SPREAD = 0 or 1 are allowed for copy with constant offsets and size.
 *                            In other words, if ALL offsets and size are constants, data to be copied can
 *                            completely reside in @p in_src_xbuf, or be spread between @p in_src_buf and buffer
 *                            that is linked to @p in_src_buf. @n
 *                            For copy with run-time offsets and/or run-time size DATA_SPREAD must be 0.
 *                            In other words, data spreading is not allowed for xbuf_copy with any run-time
 *                            parameters.
 *
 */
#macro xbuf_copy(out_dest_xbuf, out_last_element, dest_start_byte, in_src_xbuf,\
                    src_start_byte, in_prepend, total_bytes_to_copy, DATA_SPREAD)
.begin

    _DEFINE_THIS_XBUF(in_src_xbuf)
    #if (DATA_SPREAD == 1)
        #ifdef _this_xbuf_next
            #define_eval    _LINKED_SRC_BUF _this_xbuf_next
        #endif
    #endif

    #if ((is_ct_const(dest_start_byte) || isimport(dest_start_byte)) && \
        (is_ct_const(src_start_byte) || isimport(src_start_byte)) && \
        (is_ct_const(total_bytes_to_copy) || isimport(total_bytes_to_copy)))
    ;case 1: all offsets and size are constants
        _xbuf_copy_const_offsets_and_size(out_dest_xbuf, out_last_element,\
                            dest_start_byte, in_src_xbuf, src_start_byte,\
                            in_prepend, total_bytes_to_copy, DATA_SPREAD)

    #else   ;not all offsets and size are constants

        #if (DATA_SPREAD != 0)
            #error  "ONLY xbuf_copy with constant offsets and size \
                    supports data spreading 2 buffers"
        #endif

        #if ((is_ct_const(dest_start_byte) || isimport(dest_start_byte)) && \
            (is_ct_const(src_start_byte) || isimport(src_start_byte)))
        ;case 2: offsets are constants, size is run-time value


            #define_eval _ACTUAL_SRC_XBUF   in_src_xbuf
            #define_eval _ACTUAL_SRC_SBYTE  src_start_byte

            _copy_bufs_const_offsets_var_size(out_dest_xbuf, out_last_element,
                                            dest_start_byte, _ACTUAL_SRC_XBUF,
                                            _ACTUAL_SRC_SBYTE, in_prepend,
                                            total_bytes_to_copy)
        #elif (is_ct_const(total_bytes_to_copy) || isimport(total_bytes_to_copy))
        ;case 3: offsets are run-time values, size is constant

            #if (is_ct_const(src_start_byte) || isimport(src_start_byte))
            ;case 3.1: dest offset is run-time values, source offset and size are constants

                #define_eval _CHK_SRC_ALIGN (src_start_byte % 4)
                #if (_CHK_SRC_ALIGN == 0)
                    .begin

                        .reg dest_align
                        //Take care of aligned source and destination
                        alu[dest_align, dest_start_byte, AND, 3]
                        bne[case_3_unaligned_dest#]
                        _copy_var_aligned_dest_const_aligned_src_const_size( \
                                    out_dest_xbuf, out_last_element, dest_start_byte, \
                                    in_src_xbuf, src_start_byte, in_prepend, total_bytes_to_copy)
                        br[end_xbuf_copy#]
                    case_3_unaligned_dest#:

                        _copy_var_dest_const_src_and_size(out_dest_xbuf, out_last_element,
                                                dest_start_byte, in_src_xbuf, src_start_byte,
                                                in_prepend, total_bytes_to_copy)

                    .end
                #else
                    _copy_var_dest_const_src_and_size(out_dest_xbuf, out_last_element,
                                        dest_start_byte, in_src_xbuf, src_start_byte,
                                        in_prepend, total_bytes_to_copy)
                #endif
                #undef _CHK_SRC_ALIGN

            #elif (is_ct_const(dest_start_byte) || isimport(dest_start_byte))
            ;case 3.2: source offset is run-time values, dest offset and size are constants

                _copy_var_src_const_dest_and_size(out_dest_xbuf, out_last_element,
                                        dest_start_byte, in_src_xbuf, src_start_byte,
                                        in_prepend, total_bytes_to_copy)

            #else
            ;case 3.3: both source and dest offsets are run-time, size is constant
                #error "This case is not supported."
            #endif

        #elif (!(is_ct_const(total_bytes_to_copy) || isimport(total_bytes_to_copy)))
        ;case 4: size is run-time value

            #if (is_ct_const(src_start_byte) || isimport(src_start_byte))
            ;case 4.1: dest offset and size are run-time, source offset is constant


                #define_eval _CHK_SRC_ALIGN (src_start_byte % 4)
                #if (_CHK_SRC_ALIGN == 0)
                    .begin

                        .reg dest_align
                        //Take care of aligned source and destination
                        alu[dest_align, dest_start_byte, AND, 3]
                        bne[case_4_unaligned_dest#]
                        _copy_var_aligned_dest_const_aligned_src_var_size( \
                                    out_dest_xbuf, out_last_element, dest_start_byte, \
                                    in_src_xbuf, src_start_byte, in_prepend, total_bytes_to_copy)
                        br[end_xbuf_copy#]

case_4_unaligned_dest#:
                        _copy_var_dest_var_size_const_src(out_dest_xbuf, out_last_element,
                                                dest_start_byte, in_src_xbuf, src_start_byte,
                                                in_prepend, total_bytes_to_copy)

                    .end
                #else


                    _copy_var_dest_var_size_const_src(out_dest_xbuf, out_last_element,
                                        dest_start_byte, in_src_xbuf, src_start_byte,
                                        in_prepend, total_bytes_to_copy)
                #endif

                #undef _CHK_SRC_ALIGN

            #elif (is_ct_const(dest_start_byte) || isimport(dest_start_byte))
            ;case 4.2: source offset and size are run-time, dest offset is constant

                _copy_var_src_var_size_const_dest(out_dest_xbuf, out_last_element,
                                        dest_start_byte, in_src_xbuf, src_start_byte,
                                        in_prepend, total_bytes_to_copy)

            #else //both offsets are run-time, size is also run-time

                #error "This case is not supported."
            #endif

        #endif

    #endif //(is_ct_const(window_start) && is_ct_const(field_start) && is_ct_const(total_bytes_to_copy))

end_xbuf_copy#:
    .use in_src_xbuf[0]
    #ifdef  _LINKED_SRC_BUF
        .use _LINKED_SRC_BUF[0]
    #endif

.end
#endm


/// @cond INTERNAL_MACROS
/** Internal macro used by @ref xbuf_copy.
 *
 * @param out_dest_xbuf        Name of output buffer.
 *                             Can be SRAM write registers, DRAM write registers, local memory, or GPR.
 * @param out_last_element     GPR to contain last element of destination buffer.
 *                             This output is very useful when the destination buffer elements are write
 *                             transfer register and the copy results in an incompleted last element (not all
 *                             4 bytes are filled).
 *                             The need for out_last_element arises when users want to copy another buffer
 *                             into a paritally-filled destination buffer. Users may find that the last
 *                             long-word element in the destination is incompleted as the result of the previous
 *                             copy. In that case, they will need to pass the out_last_element of the previous
 *                             copy as the in_prepend to the current xbuf_copy. Otherwise, the bytes in
 *                             the partially-filled write register element will be cleared.
 *                             If users are sure that they do not need this value, they can pass the
 *                             constant 0 to save one instruction.
 * @param DEST_START_BYTE      Absolute offset in bytes from beginning of output buffer.
 *                             Must be a constant.
 *                             Can be longword aligned or not ( DEST_START_BYTE % 4 = 0, 1, 2, or 3).
 * @param in_src_xbuf          Name of input buffer.
 *                             Can be SRAM read registers, DRAM read registers, local memory, or GPR.
 * @param SRC_START_BYTE       Absolute offset in bytes from beginning of input buffer.
 *                             Must be a constant.
 *                             Can be longword aligned or not ( SRC_START_BYTE % 4 = 0, 1, 2, or 3).
 * @param in_prepend           GPR or constant containing bytes to be merged with first word
 *                             in destination, prepended, if byte alignment of destination is not 0.
 *                             The bytes to be merged must be at the exact byte locations that they will
 *                             occupy in the first word of destination. All other bytes must be 0.
 *                             In cases in_prepend is not needed, just pass constant 0.
 * @param TOTAL_BYTES_TO_COPY  Constant to specify total number of bytes to copy
 * @param DATA_SPREAD          See @p LINKED_BUFS of @ref _xbuf_check_space.
 *
 * @instruction_cnt Worst case: n + 9 (n is the number of long words copied)
 * @instruction_cnt Best case : n + 2 (n is the number of long words copied)
 */
#macro _xbuf_copy_const_offsets_and_size(out_dest_xbuf, out_last_element, \
        DEST_START_BYTE, in_src_xbuf, SRC_START_BYTE, in_prepend, TOTAL_BYTES_TO_COPY, \
        DATA_SPREAD)
.begin

    #ifdef _DEST_ALIGN
        #warning "_DEST_ALIGN is being redefined"
    #endif

    #ifdef _SRC_ALIGN
        #warning "_SRC_ALIGN is being redefined"
    #endif

    #define_eval _DEST_ALIGN  (DEST_START_BYTE % 4)
    #define_eval _SRC_ALIGN   (SRC_START_BYTE % 4)

    #if  ( _SRC_ALIGN == 0 && _DEST_ALIGN == 0)
        #undef _DEST_ALIGN
        #undef _SRC_ALIGN
        _copy_aligned_bufs_const_offsets_size(out_dest_xbuf, out_last_element,
                                DEST_START_BYTE, in_src_xbuf,
                                SRC_START_BYTE, TOTAL_BYTES_TO_COPY, DATA_SPREAD)
    #else
        #undef _DEST_ALIGN
        #undef _SRC_ALIGN
        _align_and_copy_const_offsets_size(out_dest_xbuf, out_last_element,
                                DEST_START_BYTE, in_src_xbuf, SRC_START_BYTE, in_prepend,
                                TOTAL_BYTES_TO_COPY, DATA_SPREAD)
    #endif

.end
#endm
/// @endcond


/// @cond INTERNAL_MACROS
/** An internal function to be used by _xbuf_copy_const_offsets_and_size.
 *
 * @param out_dest_xbuf        Name of output buffer.
 *                             Can be SRAM write registers, DRAM write registers, local memory, or GPR.
 * @param out_last_element     GPR to contain last element of destination buffer.
 *                             This output is very useful when the destination buffer elements are write
 *                             transfer register and the copy results in a incompleted last element (not all
 *                             4 bytes are filled).
 *                             The need for out_last_element arises when users want to copy another buffer
 *                             into a paritally-filled destination buffer. Users may find that the last
 *                             long-word element in the destination is incompleted as the result of the previous
 *                             copy. In that case, they will need to pass the out_last_element of the previous
 *                             copy as the in_prepend to the current xbuf_copy. Otherwise, the bytes in
 *                             the paritally-filled write register element will be cleared.
 *                             If users are sure that they do not need this value, they can pass the
 *                             constant 0 to save one instruction.
 * @param DEST_START_BYTE      Absolute offset in bytes from beginning of output buffer.
 *                             Must be a constant that is equal or less than the size of out_dest_xbuf.
 *                             Must be longword aligned.
 *                             Can be the same with SRC_START_BYTE or not.
 * @param in_src_xbuf          Name of input buffer.
 *                             Can be SRAM read registers, DRAM read registers, local memory, or GPR.
 * @param SRC_START_BYTE       Absolute offset in bytes from beginning of input buffer.
 *                             Must be a constant that is equal or less than the size of in_src_xbuf.
 *                             Must be longword aligned.
 *                             Can be the same with DEST_START_BYTE or not.
 * @param TOTAL_BYTES_TO_COPY  Constant to specify total number of bytes to copy
 * @param DATA_SPREAD          See @p LINKED_BUFS of @ref _xbuf_check_space.
 *
 * @instruction_cnt Worst case : n + 5 (n is the number of long words copied)
 * @instruction_cnt Best case : n + 2 (n is the number of long words copied)
 */
#macro _copy_aligned_bufs_const_offsets_size(out_dest_xbuf, out_last_element, \
                                        DEST_START_BYTE, in_src_xbuf, SRC_START_BYTE, \
                                        TOTAL_BYTES_TO_COPY, DATA_SPREAD)
.begin

    .reg copy_temp
    .set copy_temp

    #ifdef _DEST_ALIGN
        #warning "_DEST_ALIGN is being redefined"
    #endif

    #ifdef _SRC_ALIGN
        #warning "_SRC_ALIGN is being redefined"
    #endif


    #define_eval _DEST_ALIGN  (DEST_START_BYTE % 4)
    #define_eval _SRC_ALIGN   (SRC_START_BYTE % 4)

    #if ((_DEST_ALIGN != 0) || (_SRC_ALIGN != 0))
        #error "_copy_aligned_bufs_const_offsets_size: source and destination start byte \
                must be long-word aligned"
    #endif

    #if ( streq(out_dest_xbuf, in_src_xbuf) )
        ; For GPR and local memory xbuf types we do not perform copy when src & dest
        ; are exactly the same (same name with same start byte)
        #if (DEST_START_BYTE == SRC_START_BYTE)
            _GET_XBUF_TYPE(in_src_xbuf)
            #if ( (_XBUF_TYPE == _XBUF_GPR) || (_XBUF_TYPE == _XBUF_LMEM) )
                #define _DO_NOTHING
            #endif

            #undef _XBUF_TYPE
        #endif
    #endif

    #ifndef _DO_NOTHING

        _xbuf_check_space(out_dest_xbuf, DEST_START_BYTE, TOTAL_BYTES_TO_COPY, 0)


        _xbuf_check_space(in_src_xbuf, SRC_START_BYTE, TOTAL_BYTES_TO_COPY, DATA_SPREAD)

        #ifdef _S_INDEX
            #warning "_S_INDEX is being redefined"
        #endif

        #ifdef _D_INDEX
            #warning "_D_INDEX is being redefined"
        #endif

        #ifdef _NUM_LW
            #warning "_NUM_LW is being redefined"
        #endif

        #ifdef _COPIED_LW
            #warning "_COPIED_LW is being redefined"
        #endif

        #define_eval _S_INDEX       (SRC_START_BYTE / 4)
        #define_eval _D_INDEX       (DEST_START_BYTE / 4)
        #define_eval _NUM_LW        (TOTAL_BYTES_TO_COPY/4)
        #define_eval _COPIED_LW     0

        _DEFINE_THIS_XBUF(in_src_xbuf)
        #define_eval _SRC_XBUF_SIZE _this_xbuf_size
        _UNDEFINE_THIS_XBUF()

        #if (DATA_SPREAD == 0)
            #while (_COPIED_LW < _NUM_LW)

                move(out_dest_xbuf[/**/_D_INDEX], in_src_xbuf[/**/_S_INDEX])

                #define_eval _S_INDEX       (_S_INDEX + 1)
                #define_eval _D_INDEX       (_D_INDEX + 1)
                #define_eval _COPIED_LW     (_COPIED_LW + 1)
            #endloop
        #else

            #while (_COPIED_LW < _NUM_LW)
                #if (_S_INDEX < _SRC_XBUF_SIZE)
                    move(out_dest_xbuf[/**/_D_INDEX], in_src_xbuf[/**/_S_INDEX])
                #else
                    ;find the actual buffer that contains the rest of the data
                    _find_xbuf_and_relative_sb_eb(in_src_xbuf, (_S_INDEX * 4), 0, 4)

                    move(out_dest_xbuf[/**/_D_INDEX], _str_word0_xfer]
                    _clear_name_space_for_find_xbuf_and_relative_sb_eb()

                #endif

                #define_eval _S_INDEX       (_S_INDEX + 1)
                #define_eval _D_INDEX       (_D_INDEX + 1)
                #define_eval _COPIED_LW     (_COPIED_LW + 1)
            #endloop

        #endif  //(DATA_SPREAD == 0)


        #ifdef _REMAINING_BYTES
            #warning "_REMAINING_BYTES is being redefined"
        #endif

        #define_eval _REMAINING_BYTES  (TOTAL_BYTES_TO_COPY % 4)

        #if (_REMAINING_BYTES > 0)
            xbuf_extract(copy_temp, in_src_xbuf, SRC_START_BYTE, (_NUM_LW*4), _REMAINING_BYTES)

            _GET_LSHIFT_FROM_REMAINING_BYTES(_REMAINING_BYTES)

            #if (!(is_ct_const (out_last_element) || isimport(out_last_element)))
                alu_shf[out_last_element, --, B, copy_temp, <</**/_L_SHIFT_VAL_FROM_REMAINING_BYTES]
                alu[out_dest_xbuf[_D_INDEX], --, B, out_last_element]
            #else
                alu_shf[copy_temp, --, B, copy_temp, <</**/_L_SHIFT_VAL_FROM_REMAINING_BYTES]
                alu[out_dest_xbuf[_D_INDEX], --, B, copy_temp]
            #endif
            #undef _L_SHIFT_VAL_FROM_REMAINING_BYTES

        #else
            #define_eval _LAST  (_S_INDEX - 1)
            #if (!(is_ct_const (out_last_element) || isimport(out_last_element)))
                #if (_LAST < _SRC_XBUF_SIZE)
                    move(out_last_element, in_src_xbuf[/**/_LAST])
                #elif (DATA_SPREAD == 1)
                    ;find the actual buffer that contains the rest of the data
                    _find_xbuf_and_relative_sb_eb(in_src_xbuf, (_LAST * 4), 0, 4)

                    move(out_last_element, _str_word0_xfer]
                    _clear_name_space_for_find_xbuf_and_relative_sb_eb()
                #endif
            #endif

            #undef _LAST
        #endif

        #ifdef _SRC_XBUF_SIZE
            #undef _SRC_XBUF_SIZE
        #endif
        #undef _S_INDEX
        #undef _D_INDEX
        #undef _NUM_LW
        #undef _COPIED_LW
        #undef _REMAINING_BYTES

    #else
        // To stop from unnecessary copy, return without copying for GPR & LOCAL Memory
        // when the source and destn are the same with same start byte
        //#warning "_copy_aligned_bufs_const_offsets_size: called with the exact same \
        //              buffers and start bytes. Nothing done"
        #undef _DO_NOTHING
    #endif

    #undef _DEST_ALIGN
    #undef _SRC_ALIGN

.end
#endm
/// @endcond


/// @cond INTERNAL_MACROS
/** An internal function to be used by xbuf_copy, xbuf_align_and_copy and related copy macros.
 *
 * @param out_dest_xbuf        Name of output buffer.
 *                             Can be SRAM write registers, DRAM write registers, local memory, or GPR.
 * @param out_last_element     GPR to contain last element of destination buffer.
 *                             This output is very useful when the destination buffer elements are write
 *                             transfer register and the copy results in an incompleted last element (not all
 *                             4 bytes are filled).
 *                             The need for out_last_element arises when users want to copy another buffer
 *                             into a paritally-filled destination buffer. Users may find that the last
 *                             long-word element in the destination is incompleted as the result of the previous
 *                             copy. In that case, they will need to pass the out_last_element of the previous
 *                             copy as the in_prepend to the current xbuf_copy. Otherwise, the bytes in
 *                             the partially-filled write register element will be cleared.
 *                             If users are sure that they do not need this value, they can pass the
 *                             constant 0 to save one instruction.
 * @param DEST_START_BYTE      Absolute offset in bytes from beginning of output buffer.
 *                             Must be a constant.
 *                             Can be longword aligned or not ( DEST_START_BYTE % 4 = 0, 1, 2, or 3).
 *                             Can be the same with SRC_START_BYTE or not.
 * @param in_src_xbuf          Name of input buffer.
 *                             Can be SRAM read registers, DRAM read registers, local memory, or GPR.
 * @param SRC_START_BYTE       Absolute offset in bytes from beginning of input buffer.
 *                             Must be a constant.
 *                             Can be longword aligned or not ( SRC_START_BYTE % 4 = 0, 1, 2, or 3).
 *                             Can be the same with DEST_START_BYTE or not.
 * @param in_prepend           GPR or constant containing bytes to be merged with first word
 *                             in destination, prepended, if byte alignment of destination is not 0.
 *                             The bytes to be merged must be at the exact byte locations that they will
 *                             occupy in the first word of destination. All other bytes must be 0.
 *                             In cases in_prepend is not needed, just pass constant 0.
 * @param TOTAL_BYTES_TO_COPY  Constant to specify total number of bytes to copy
 * @param DATA_SPREAD          See @p LINKED_BUFS of @ref _xbuf_check_space.
 *
 * @instruction_cnt Worst case : n + 9 (n is the number of long words copied)
 * @instruction_cnt Best case : n + 2 (n is the number of long words copied)
 */
#macro _align_and_copy_const_offsets_size(out_dest_xbuf, out_last_element, DEST_START_BYTE, \
        in_src_xbuf, SRC_START_BYTE, in_prepend, TOTAL_BYTES_TO_COPY, DATA_SPREAD)

.begin

    .reg copy_temp
    .set copy_temp

    #ifdef _DEST_ALIGN
        #warning "_DEST_ALIGN is being redefined"
    #endif

    #ifdef _SRC_ALIGN
        #warning "_SRC_ALIGN is being redefined"
    #endif

    #define_eval _DEST_ALIGN  (DEST_START_BYTE % 4)
    #define_eval _SRC_ALIGN   (SRC_START_BYTE % 4)

    #if ( streq(out_dest_xbuf, in_src_xbuf) )
        ; For GPR and local memory xbuf types we do not perform copy when src & dest
        ; are exactly the same (same name with same start byte)
        #if (DEST_START_BYTE == SRC_START_BYTE)
            _GET_XBUF_TYPE(in_src_xbuf)
            #if ( (_XBUF_TYPE == _XBUF_GPR) || (_XBUF_TYPE == _XBUF_LMEM) )
                #define _DO_NOTHING
            #endif

            #undef _XBUF_TYPE
        #endif
    #endif


    #ifndef _DO_NOTHING

        _xbuf_check_space(out_dest_xbuf, DEST_START_BYTE, TOTAL_BYTES_TO_COPY, 0)
        _xbuf_check_space(in_src_xbuf, SRC_START_BYTE, TOTAL_BYTES_TO_COPY, DATA_SPREAD)

        #if ((_DEST_ALIGN == 0) && (_SRC_ALIGN == 0))

            #undef  _DEST_ALIGN
            #undef  _SRC_ALIGN

            _copy_aligned_bufs_const_offsets_size(out_dest_xbuf, out_last_element,
                                DEST_START_BYTE, in_src_xbuf,
                                SRC_START_BYTE, TOTAL_BYTES_TO_COPY, DATA_SPREAD)
        #elif (_DEST_ALIGN == 0)

            #undef  _DEST_ALIGN
            #undef  _SRC_ALIGN
            _copy_to_aligned_dest_const_offsets_size(out_dest_xbuf, out_last_element,
                                DEST_START_BYTE, in_src_xbuf,
                                SRC_START_BYTE, TOTAL_BYTES_TO_COPY, DATA_SPREAD)
        #else

            ;first take care of unaligned bytes in destination
            #define_eval    _D_INDEX    (DEST_START_BYTE / 4)

            xbuf_extract(copy_temp, in_src_xbuf, SRC_START_BYTE, 0, (4 - _DEST_ALIGN))

            #if (!(is_ct_const(in_prepend)  || isimport(in_prepend)))
                    alu[out_dest_xbuf[_D_INDEX], copy_temp, OR, in_prepend]
            #elif ( (is_ct_const(in_prepend) || isimport(in_prepend)) && (in_prepend != 0) )
                    alu[out_dest_xbuf[_D_INDEX], copy_temp, OR, in_prepend]
            #else
                    alu[out_dest_xbuf[_D_INDEX], --, B, copy_temp]
            #endif

            #undef  _D_INDEX

            #undef  _DEST_ALIGN
            #undef  _SRC_ALIGN

            #define_eval    _NUM_UNALIGNED_BYTES    (4 - (DEST_START_BYTE % 4))
            ; new destination start byte = (DEST_START_BYTE + _NUM_UNALIGNED_BYTES)
            ; new source start byte =  (SRC_START_BYTE + _NUM_UNALIGNED_BYTES),
            ; new total bytes to copy = (TOTAL_BYTES_TO_COPY - _NUM_UNALIGNED_BYTES) )
            //r
            #if ((TOTAL_BYTES_TO_COPY - _NUM_UNALIGNED_BYTES) >0)
            _copy_to_aligned_dest_const_offsets_size(out_dest_xbuf, out_last_element,
                                (DEST_START_BYTE + _NUM_UNALIGNED_BYTES ),
                                in_src_xbuf, (SRC_START_BYTE + _NUM_UNALIGNED_BYTES ),
                                (TOTAL_BYTES_TO_COPY - _NUM_UNALIGNED_BYTES ),
                                DATA_SPREAD)
            #endif
        #endif

    #else
        // To stop from unnecessary copy, return without copying for GPR & LOCAL Memory
        // when the source and destn are the same with same start byte
        //#warning "_copy_aligned_bufs_const_offsets_size: called with the exact same buffers \
        //          and start bytes. Nothing done"
        #undef _DO_NOTHING
        #undef  _DEST_ALIGN
        #undef  _SRC_ALIGN


    #endif

    #ifdef  _NUM_UNALIGNED_BYTES
        #undef  _NUM_UNALIGNED_BYTES
    #endif

.end
#endm
/// @endcond


/// @cond INTERNAL_MACROS
/** An internal function to be used by xbuf_copy, xbuf_align_and_copy and related copy macros.
 */
#macro _copy_to_aligned_dest_const_offsets_size(out_dest_xbuf, out_last_element, DEST_START_BYTE, \
            in_src_xbuf, SRC_START_BYTE, TOTAL_BYTES_TO_COPY, DATA_SPREAD)
.begin

    .reg temp_byte_field save_src_lmem_addr save_dest_lmem_addr
    .set temp_byte_field

    #ifdef _DEST_ALIGN
        #warning "_DEST_ALIGN is being redefined"
    #endif

    #ifdef _SRC_ALIGN
        #warning "_SRC_ALIGN is being redefined"
    #endif


    #define_eval _DEST_ALIGN  (DEST_START_BYTE % 4)
    #define_eval _SRC_ALIGN   (SRC_START_BYTE % 4)

    #if (_DEST_ALIGN != 0)
        #error "_xbuf_copy_to_aligned_dest: destination start byte must be long-word aligned"
    #endif

    _GET_XBUF_TYPE(in_src_xbuf)
    #define_eval _SRC_XBUF_TYPE     _XBUF_TYPE
    #undef _XBUF_TYPE

    _GET_XBUF_TYPE(out_dest_xbuf)
    #define_eval _DEST_XBUF_TYPE    _XBUF_TYPE
    #undef _XBUF_TYPE

    _DEFINE_THIS_XBUF(in_src_xbuf)
    #define_eval _SRC_XBUF_SIZE _this_xbuf_size
    #define_eval _SRC_XBUF_NAME _this_xbuf_name
    #if (DATA_SPREAD == 1)
        #ifndef _this_xbuf_next
            #error "xbuf_copy: DATA_SPREAD == 1 but in_src_xbuf is not \
                    linked to another xbuf"
        #else
            #define_eval _NEXT_SRC_XBUF _this_xbuf_next
            _UNDEFINE_THIS_XBUF()
            _DEFINE_THIS_XBUF(_NEXT_SRC_XBUF)
            #define_eval _NEXT_SRC_XBUF_SIZE _this_xbuf_size
        #endif
    #endif
    _UNDEFINE_THIS_XBUF()


    #define_eval    _SRC_LM_INDEX_ABOVE_8   0
    #define_eval    _DEST_LM_INDEX_ABOVE_8  0

    #if ( streq(out_dest_xbuf, in_src_xbuf) )
        ; For GPR and local memory xbuf types we do not perform copy when src & dest
        ; are exactly the same (same name with same start byte)
        #if (DEST_START_BYTE == SRC_START_BYTE)
            #if ( (_SRC_XBUF_TYPE == _XBUF_GPR) || (_SRC_XBUF_TYPE == _XBUF_LMEM) )
                #define _DO_NOTHING
            #endif
        #endif
    #endif

    #ifndef _DO_NOTHING

        #if (_SRC_ALIGN == 0)

            #undef  _SRC_ALIGN
            #undef  _DEST_ALIGN
            _copy_aligned_bufs_const_offsets_size(out_dest_xbuf, out_last_element,
                    DEST_START_BYTE, in_src_xbuf, SRC_START_BYTE,
                    TOTAL_BYTES_TO_COPY, DATA_SPREAD)
        #else

            #ifdef _S_INDEX
                #warning "_S_INDEX is being redefined"
            #endif

            #ifdef _D_INDEX
                #warning "_D_INDEX is being redefined"
            #endif

            #ifdef _NUM_LW
                #warning "_NUM_LW is being redefined"
            #endif

            #ifdef _COPIED_LW
                #warning "_COPIED_LW is being redefined"
            #endif

            #ifdef _REMAINING_BYTES
                #warning "_REMAINING_BYTES is being redefined"
            #endif

            #define_eval _S_INDEX           (SRC_START_BYTE / 4)
            #define_eval _D_INDEX           (DEST_START_BYTE / 4)
            #define_eval _COPIED_LW         0
            #define_eval _NUM_LW            (TOTAL_BYTES_TO_COPY / 4)
            //bytes that are not moved from src to dest after byte_align_be operation
            #define_eval _REMAINING_BYTES  (TOTAL_BYTES_TO_COPY % 4)

            // CSR write has 3-cycle latency
            local_csr_wr[byte_index, _SRC_ALIGN]

            ///////////////////START LMEM INDEX >=8 WORK-AROUND//////////////////////////
            //For _S_ALIGN > 0, x longwords comes from x+1 source registers.
            //Hence, create temp source buffer as soon as _S_INDEX + _NUM_LW = 8
            #if ((_SRC_XBUF_TYPE == _XBUF_LMEM) && ((_S_INDEX + _NUM_LW) >= 8))
                _copy_lm_regs_above_8_to_temp_buf(in_src_xbuf, _SRC_XBUF_SIZE, _S_INDEX, _NUM_LW)
            #endif

            #if ((_SRC_XBUF_TYPE == _XBUF_LMEM) && ((_S_INDEX + _NUM_LW) > _SRC_XBUF_SIZE))
                _copy_lm_regs_above_8_to_temp_buf(_NEXT_SRC_XBUF, _NEXT_SRC_XBUF_SIZE, \
                                            0, (_NUM_LW - (_SRC_XBUF_SIZE - _S_INDEX)) )
            #endif

            #if ((_DEST_XBUF_TYPE == _XBUF_LMEM) && ((_D_INDEX + _NUM_LW) > 8))

                #if (TOTAL_BYTES_TO_COPY % 4)
                    #if (_D_INDEX < 8)
                        #define_eval _DEST_LM_INDEX_ABOVE_8 ((_D_INDEX + _NUM_LW + 1) - 8)
                    #else
                        #define_eval _DEST_LM_INDEX_ABOVE_8 ((_D_INDEX + _NUM_LW + 1) - _D_INDEX)
                    #endif
                #else
                    #if (_D_INDEX < 8)
                        #define_eval _DEST_LM_INDEX_ABOVE_8 ((_D_INDEX + _NUM_LW) - 8)
                    #else
                        #define_eval _DEST_LM_INDEX_ABOVE_8 ((_D_INDEX + _NUM_LW) - _D_INDEX)
                    #endif
                #endif

                #if (_DEST_LM_INDEX_ABOVE_8 > 0)
                    xbuf_alloc(tmp_dest_buf, _DEST_LM_INDEX_ABOVE_8, read_write)

                    #define_eval i  0
                    #while (i < _DEST_LM_INDEX_ABOVE_8)
                        .set tmp_dest_buf[i]
                        #define_eval    i (i + 1)
                    #endloop
                #endif
            #endif
            ///////////////////END LMEM INDEX >=8 WORK-AROUND//////////////////////////

            ; before doing byte align operation, fill out the last element
            ;and/or remaining bytes to utilize the 3 cycles latency
            #if (_REMAINING_BYTES > 0)
                xbuf_extract(temp_byte_field, in_src_xbuf, SRC_START_BYTE, (_NUM_LW*4),
                            _REMAINING_BYTES)

                _GET_LSHIFT_FROM_REMAINING_BYTES(_REMAINING_BYTES)
                #define_eval _LAST  (_D_INDEX + _NUM_LW)

                #if (!(is_ct_const (out_last_element) || isimport(out_last_element)))
                    alu_shf[out_last_element, --, B, temp_byte_field, <</**/_L_SHIFT_VAL_FROM_REMAINING_BYTES]
                    alu_shf[out_dest_xbuf[_LAST], --, B, out_last_element]
                #else
                    #if ((_DEST_XBUF_TYPE == _XBUF_LMEM) && (_LAST >= 8))
                        alu_shf[temp_byte_field, --, B, temp_byte_field, <</**/_L_SHIFT_VAL_FROM_REMAINING_BYTES]
                        alu_shf[out_dest_xbuf[_LAST], --, B, temp_byte_field]
                        //alu_shf[out_dest_xbuf[_LAST], --, B, out_last_element] //r
                    #else
                        alu_shf[out_dest_xbuf[_LAST], --, B, temp_byte_field, \
                                <</**/_L_SHIFT_VAL_FROM_REMAINING_BYTES]
                        //see how many nop is needed based on whether there
                        //are any alu isntructions to move in_src_xbuf to tmp_src_buf
                        #if (_SRC_LM_INDEX_ABOVE_8 == 0)
                            nop
                        #endif
                    #endif
                #endif

                #undef _L_SHIFT_VAL_FROM_REMAINING_BYTES
                #undef _LAST

            #else
                #if (!(is_ct_const (out_last_element) || isimport(out_last_element)))
                    xbuf_extract(out_last_element, in_src_xbuf, \
                                SRC_START_BYTE, (TOTAL_BYTES_TO_COPY - 4), 4)

                    //see how many nop is needed based on whether there
                    //are any alu isntructions to move in_src_xbuf to tmp_src_buf
                    #if (_SRC_LM_INDEX_ABOVE_8 == 0)
                        nop
                        nop
                    #elif (_SRC_LM_INDEX_ABOVE_8 == 1)
                        nop
                    #endif
                #else
                    #if (_SRC_LM_INDEX_ABOVE_8 == 0)
                        nop
                        nop
                        nop
                    #elif (_SRC_LM_INDEX_ABOVE_8 == 1)
                        nop
                        nop
                    #elif (_SRC_LM_INDEX_ABOVE_8 == 2)
                        nop
                    #endif
                #endif
            #endif

            /////////////////// START LMEM INDEX >=8 WORK-AROUND//////////////////////////

            #define_eval _SAVED_D_INDEX     _D_INDEX
            #if ((_D_INDEX < 8) || (_DEST_XBUF_TYPE != _XBUF_LMEM) )
                #define_eval _DESTINATION   out_dest_xbuf
            #else
                #define_eval _DESTINATION       tmp_dest_buf
                #define_eval _D_INDEX           0
            #endif
            ///////////////////END LMEM INDEX >=8 WORK-AROUND//////////////////////////

            #if (DATA_SPREAD == 0)

                ///////////////////LMEM INDEX >=8 WORK-AROUND////////////////////////////////
                #if ((_S_INDEX < 8) || (_SRC_XBUF_TYPE != _XBUF_LMEM) )
                    #define_eval _SOURCE        in_src_xbuf
                #else
                    #define_eval _SOURCE        tmp_src_buf
                    #define_eval _S_INDEX       0
                #endif

                byte_align_be[--, _SOURCE[_S_INDEX]]

                #define_eval _S_INDEX       (_S_INDEX + 1)

                #while (_COPIED_LW < _NUM_LW)

                    #if ((_S_INDEX == 8) && (_SRC_XBUF_TYPE == _XBUF_LMEM) )
                        #define_eval _SOURCE        tmp_src_buf
                        #define_eval _S_INDEX       0
                    #endif

                    #if ((_D_INDEX == 8) && (_DEST_XBUF_TYPE == _XBUF_LMEM) )
                        #define_eval _DESTINATION   tmp_dest_buf
                        #define_eval _SAVED_D_INDEX     8
                        #define_eval _D_INDEX       0
                    #endif

                    //#if ((_SRC_LM_INDEX_ABOVE_8 == 0) || (_S_INDEX < _SRC_LM_INDEX_ABOVE_8))
                        byte_align_be[_DESTINATION[/**/_D_INDEX], _SOURCE[/**/_S_INDEX]]
                    //#else
                    //  byte_align_be[_DESTINATION[/**/_D_INDEX], 0]
                    //#endif

                    #define_eval _S_INDEX       (_S_INDEX + 1)
                    #define_eval _D_INDEX       (_D_INDEX + 1)
                    #define_eval _COPIED_LW     (_COPIED_LW + 1)


                #endloop

                #if (_DEST_LM_INDEX_ABOVE_8 > 0)

                    #define_eval i  0
                    #define_eval j  _SAVED_D_INDEX
                    #define_eval _SIZE ( (DEST_START_BYTE / 4) + _NUM_LW - _SAVED_D_INDEX)
                    #while (i <  _SIZE)
                        alu[out_dest_xbuf[j], --, B, tmp_dest_buf[i]]
                        #define_eval i (i + 1)
                        #define_eval j (j + 1)
                    #endloop
                #endif

                /////////////////// END LMEM INDEX >=8 WORK-AROUND//////////////////////////
            #else

                ///////////////////LMEM INDEX >=8 WORK-AROUND////////////////////////////
                #if (_S_INDEX < _SRC_XBUF_SIZE)
                    #if ((_S_INDEX < 8) || (_SRC_XBUF_TYPE != _XBUF_LMEM) )
                        #define_eval _SOURCE        in_src_xbuf
                        #define_eval _SIZE          _SRC_XBUF_SIZE
                    #else
                        #define_eval _SOURCE        tmp_src_buf
                        #define_eval _S_INDEX       0
                        #define_eval _SIZE          _TMP_SRC_BUF_SIZE
                    #endif
                #else
                    #if (((_S_INDEX - _SRC_XBUF_SIZE) < 8) || \
                        (_SRC_XBUF_TYPE != _XBUF_LMEM) )
                        #define_eval _SOURCE        _NEXT_SRC_XBUF
                        #define_eval _SIZE          _NEXT_SRC_XBUF_SIZE
                        #define_eval _S_INDEX       0
                    #else
                        #define_eval _SOURCE        next_tmp_src_buf
                        #define_eval _SIZE          _NEXT_TMP_SRC_BUF_SIZE
                        #define_eval _S_INDEX       0
                    #endif
                #endif

                #if (_S_INDEX < _SIZE)
                    byte_align_be[--, _SOURCE[_S_INDEX]]
                #else
                    #if (_SRC_XBUF_TYPE == _XBUF_LMEM)
                        #define_eval _SAVED_S_INDEX     _S_INDEX
                        #define_eval _S_INDEX           0
                        #define_eval _SOURCE            _NEXT_SRC_XBUF
                        //_copy_lm_regs_above_8_to_temp_buf(_NEXT_SRC_XBUF, _NEXT_SRC_XBUF_SIZE,
                        //                              _S_INDEX, _NUM_LW)
                        #define_eval _SIZE              _NEXT_SRC_XBUF_SIZE
                        byte_align_be[--, _SOURCE[_S_INDEX]]
                    #else
                        _find_xbuf_and_relative_sb_eb(in_src_xbuf, (_S_INDEX * 4), 0, 4)

                        byte_align_be[--, _str_word0_xfer]
                        _clear_name_space_for_find_xbuf_and_relative_sb_eb()

                    #endif
                #endif

                #define_eval _S_INDEX       (_S_INDEX + 1)

                #while (_COPIED_LW < _NUM_LW)

                    #if ((_D_INDEX == 8) && (_DEST_XBUF_TYPE == _XBUF_LMEM) )
                        #define_eval _DESTINATION       tmp_dest_buf
                        #define_eval _SAVED_D_INDEX     8
                        #define_eval _D_INDEX           0
                    #endif

                    #if (_S_INDEX < _SIZE)

                        #if ((_S_INDEX == 8) && (_SRC_XBUF_TYPE == _XBUF_LMEM) )
                            #if (streq(_SOURCE, in_src_xbuf))
                                #define_eval _SOURCE            tmp_src_buf
                            #else
                                #define_eval _SOURCE            next_tmp_src_buf
                            #endif

                            #define_eval _SAVED_S_INDEX     8
                            #define_eval _S_INDEX           0
                        #endif

                        byte_align_be[_DESTINATION[/**/_D_INDEX], _SOURCE[/**/_S_INDEX]]

                    #else
                        #if (_SRC_XBUF_TYPE == _XBUF_LMEM)
                            #define_eval _SAVED_S_INDEX     _S_INDEX
                            #define_eval _S_INDEX           0

                            //_copy_lm_regs_above_8_to_temp_buf(_NEXT_SRC_XBUF, _NEXT_SRC_XBUF_SIZE,
                            //                          _S_INDEX, _NUM_LW)

                            #if (_S_INDEX >= 8)
                                #define_eval _SOURCE            next_tmp_src_buf
                                #define_eval _SIZE              _NEXT_TMP_SRC_BUF_SIZE
                            #else
                                #define_eval _SOURCE            _NEXT_SRC_XBUF
                                #define_eval _SIZE              _NEXT_SRC_XBUF_SIZE
                            #endif

                            byte_align_be[_DESTINATION[/**/_D_INDEX], _SOURCE[/**/_S_INDEX]]
                        #else
                            _find_xbuf_and_relative_sb_eb(in_src_xbuf, (_S_INDEX * 4), 0, 4)

                            byte_align_be[_DESTINATION[/**/_D_INDEX], _str_word0_xfer]
                            _clear_name_space_for_find_xbuf_and_relative_sb_eb()

                        #endif

                    #endif

                    #define_eval _S_INDEX       (_S_INDEX + 1)
                    #define_eval _D_INDEX       (_D_INDEX + 1)
                    #define_eval _COPIED_LW     (_COPIED_LW + 1)
                #endloop

                #if (_DEST_LM_INDEX_ABOVE_8 > 0)

                    #define_eval i  0
                    #define_eval j  _SAVED_D_INDEX
                    #define_eval _SIZE ( (DEST_START_BYTE / 4) + _NUM_LW - _SAVED_D_INDEX)
                    #while (i <  _SIZE)
                        alu[out_dest_xbuf[j], --, B, tmp_dest_buf[i]]
                        #define_eval i (i + 1)
                        #define_eval j (j + 1)
                    #endloop
                #endif
                ///////////////////END LMEM INDEX >=8 WORK-AROUND////////////////////////////
            #endif

            #undef _SRC_XBUF_SIZE
            #undef  _S_INDEX
            #undef  _D_INDEX
            #undef  _NUM_LW
            #undef  _COPIED_LW
            #undef  _REMAINING_BYTES
            #undef _DEST_ALIGN
            #undef _SRC_ALIGN
            #undef _SOURCE
            #undef _DESTINATION
            #undef _SAVED_D_INDEX

            #if (_DEST_LM_INDEX_ABOVE_8 > 0)
                xbuf_free(tmp_dest_buf)
            #endif

            #ifdef _ALLOCATED_TMP_SRC_XBUF
                xbuf_free(tmp_src_buf)
                #undef _ALLOCATED_TMP_SRC_XBUF
            #endif

            #ifdef _ALLOCATED_NEXT_TMP_SRC_XBUF
                xbuf_free(next_tmp_src_buf)
                #undef _ALLOCATED_NEXT_TMP_SRC_XBUF
            #endif

            #undef _DEST_LM_INDEX_ABOVE_8
            #undef _SRC_LM_INDEX_ABOVE_8
            #ifdef _SIZE
                #undef _SIZE
            #endif
            #ifdef _TMP_SRC_BUF_SIZE
                #undef _TMP_SRC_BUF_SIZE
            #endif
            #ifdef _NEXT_TMP_SRC_BUF_SIZE
                #undef _NEXT_TMP_SRC_BUF_SIZE
            #endif

        #endif  //SRC_ALIGN

    #else
        // To stop from unnecessary copy, return without copying for GPR & LOCAL Memory
        // when the source and destn are the same with same start byte
        //#warning "_copy_to_aligned_dest_const_offsets_size: called with the exact same \
        //          buffers and start bytes. Nothing done"
        #undef _DO_NOTHING
    #endif


.end
#endm
/// @endcond


/// @cond INTERNAL_MACROS
/**
 *
//  destination offset: run-time (variable)
//  source offset:      run-time (variable)
 *
 */
#macro _copy_var_offsets_const_size(out_dest_xbuf, out_last_element, \
                                dest_start_byte, in_src_xbuf, src_start_byte, \
                                in_prepend, NUMBER_OF_BYTES)
.begin

    #error "This case is not supported."

    .reg dest_b_index src_b_index
    .reg var_offs_temp

    #if (!(is_ct_const(NUMBER_OF_BYTES) || isimport(NUMBER_OF_BYTES)))
        #error 2   "_copy_var_offsets_const_size: NUMBER_OF_BYTES must be constant"
    #endif

    _GET_XBUF_TYPE(in_src_xbuf)
    #define_eval _SRC_XBUF_TYPE     _XBUF_TYPE
    #undef _XBUF_TYPE

    _GET_XBUF_TYPE(out_dest_xbuf)
    #define_eval _DEST_XBUF_TYPE    _XBUF_TYPE
    #undef _XBUF_TYPE

    alu[dest_b_index, dest_start_byte, AND, 3]
    beq[var_offs_aligned_dest#], defer [1]
    //alu_shf[dest_t_b_index, dest_b_index, OR, dest_start_byte, >>2]
    alu[src_b_index, src_start_byte, AND, 3]
    beq[var_offs_aligned_src#], defer [3]
    //alu_shf[src_t_b_index, src_b_index, OR, src_start_byte, >>2]

    ;dest and src are both unaligned
    ;extract bytes in src and put in destination to fill unaligned bytes

    //now call _copy_aligned_dest_var_offs_const_size
    _copy_aligned_dest_var_offs_const_size(out_dest_xbuf, out_last_element,
                            dest_b_index, dest_start_byte, in_src_xbuf, src_b_index,
                            src_start_byte, in_prepend, NUMBER_OF_BYTES)

var_offs_aligned_dest#:
    beq var_offs_aligned_both#, defer[1]
    alu_shf[src_t_b_index, src_b_index, OR, src_start_byte, >>2]
    _copy_aligned_dest_var_offs_const_size(out_dest_xbuf, out_last_element,
                            dest_b_index, dest_start_byte, in_src_xbuf, src_b_index,
                            src_start_byte, in_prepend, NUMBER_OF_BYTES)
var_offs_aligned_src#:
    _copy_aligned_src_var_offs_const_size(out_dest_xbuf, out_last_element,
                            dest_b_index, dest_start_byte, in_src_xbuf, src_b_index,
                            src_start_byte, in_prepend, NUMBER_OF_BYTES)
var_offs_aligned_both#:
    _copy_aligned_both_var_offs_const_size(out_dest_xbuf, out_last_element,
                            dest_b_index, dest_start_byte, in_src_xbuf, src_b_index,
                            src_start_byte, in_prepend, NUMBER_OF_BYTES)

end_copy_bufs_var_offsets_const_size#:

.end
#endm
/// @endcond


/// @cond INTERNAL_MACROS
/** An internal macro used by xbuf_insert.
 */
#macro _copy_aligned_dest_var_offs_const_size(out_dest_xbuf, out_last_element, \
                            dest_b_index, dest_start_byte, in_src_xbuf, src_b_index, \
                            src_start_byte, in_prepend, NUMBER_OF_BYTES)
    #error "This case is not supported."
#endm
/// @endcond


/// @cond INTERNAL_MACROS
/** An internal macro used by xbuf_insert.
 */
#macro _copy_aligned_src_var_offs_const_size(out_dest_xbuf, out_last_element, \
                            dest_b_index, dest_start_byte, in_src_xbuf, src_b_index, \
                            src_start_byte, in_prepend, NUMBER_OF_BYTES)
    #error "This case is not supported."
#endm
/// @endcond


/// @cond INTERNAL_MACROS
/** An internal macro used by xbuf_insert.
 */
#macro _copy_aligned_both_var_offs_const_size(out_dest_xbuf, out_last_element, \
                            dest_b_index, dest_start_byte, in_src_xbuf, src_b_index, \
                            src_start_byte, in_prepend, NUMBER_OF_BYTES)
    #error "This case is not supported."
#endm
/// @endcond


/// @cond INTERNAL_MACROS
/**
//  destination offset: run-time (variable)
//  source offset:      constant
//  size:               constant
 */
#macro _copy_var_dest_const_src_and_size(out_dest_xbuf, out_last_element, \
                            dest_start_byte, in_src_xbuf, SRC_START_BYTE, \
                            in_prepend, NUMBER_OF_BYTES)
.begin

    .reg dest_align num_unaligned_bytes unaligned_bytes
    .reg dest_start_lw vd_temp new_src_start_byte next_lw

    .set dest_align num_unaligned_bytes unaligned_bytes
    .set dest_start_lw vd_temp new_src_start_byte next_lw

    #if (!(is_ct_const(SRC_START_BYTE) || isimport(SRC_START_BYTE)))
        #error 2   "_copy_var_dest_const_src_and_size: SRC_START_BYTE must be constant"
    #endif

    #if (!(is_ct_const(NUMBER_OF_BYTES) || isimport(NUMBER_OF_BYTES)))
        #error 2   "_copy_var_dest_const_src_and_size: NUMBER_OF_BYTES must be constant"
    #endif

    _GET_XBUF_TYPE(in_src_xbuf)
    #define_eval _SRC_XBUF_TYPE     _XBUF_TYPE
    #undef _XBUF_TYPE

    _DEFINE_THIS_XBUF(in_src_xbuf)
    #define_eval _SRC_XBUF_SIZE     _this_xbuf_size
    _UNDEFINE_THIS_XBUF()

    _GET_XBUF_TYPE(out_dest_xbuf)
    #define_eval _DEST_XBUF_TYPE    _XBUF_TYPE
    #undef _XBUF_TYPE

    #if ( streq(out_dest_xbuf, in_src_xbuf) )
        ; For GPR and local memory xbuf types we do not perform copy when src & dest
        ; are exactly the same (same name with same start byte)
        #if ( (_SRC_XBUF_TYPE == _XBUF_GPR) || (_SRC_XBUF_TYPE == _XBUF_LMEM) )
            alu[--, dest_start_byte, -, SRC_START_BYTE]
            beq[end_xbuf_copy#]
        #endif
    #endif

    #if (NUMBER_OF_BYTES < 5)

    .begin

        .reg    shf_left shf_right extra_bytes saved_bytes
        .set    shf_left shf_right extra_bytes saved_bytes

        #define_eval    _EXTRACTED_BYTE_LOC ((4 - NUMBER_OF_BYTES) * 8)
        #define_eval    _EXTRACTED_LW_LOC   (4 - NUMBER_OF_BYTES)

        alu[dest_start_lw, dest_start_byte, AND~, 3]
        xbuf_extract(unaligned_bytes, in_src_xbuf, SRC_START_BYTE, 0, \
                            NUMBER_OF_BYTES)

        #if ((_DEST_XBUF_TYPE == _XBUF_SRAM_XFER) || (_DEST_XBUF_TYPE == _XBUF_DRAM_XFER))

            #if (_DEST_XBUF_TYPE == _XBUF_SRAM_XFER)
                #define_eval    _INDEX  *$index
            #elif ( (!defined(__NFP_INDIRECT_REF_FORMAT_NFPXXXX)) && (_DEST_XBUF_TYPE == _XBUF_DRAM_XFER) )
                #define_eval    _INDEX  *$$index
            #endif

            br=ctx[0, _copy_var_dest_skip_find_abs_addr_1#], defer[2]
            add_shf_left(vd_temp, dest_start_lw, &out_dest_xbuf[/**/0], 2)

            //if not context 0, need to add offset to absolute reg to T_INDEX value
            _find_absolute_xfer_reg_addr(vd_temp)

_copy_var_dest_skip_find_abs_addr_1#:
            local_csr_wr[T_INDEX, vd_temp]

            //calculate the number of bits must be shifted to put the extracted bytes
            //into the right location in the destination
            alu[dest_align, dest_start_byte, AND, 3]

            //if (dest_align == _EXTRACTED_LW_LOC)
            alu[--, dest_align, -, _EXTRACTED_LW_LOC]
            bne[not_equal_extracted_loc#]

            #if (!(is_ct_const(out_last_element) || isimport(out_last_element)))

                #if ( (!(is_ct_const(in_prepend) || isimport(in_prepend))) || \
                    (((is_ct_const(in_prepend) || isimport(in_prepend))) && (in_prepend != 0)) )
                    alu[out_last_element, in_prepend, OR, unaligned_bytes]
                #else
                    alu[out_last_element, --, B, unaligned_bytes]
                #endif

                alu[_INDEX, --, B, out_last_element]

            #else

                #if ( (!(is_ct_const(in_prepend) || isimport(in_prepend))) || \
                    (((is_ct_const(in_prepend) || isimport(in_prepend))) && (in_prepend != 0)) )
                    alu[_INDEX, in_prepend, OR, unaligned_bytes]
                #else
                    alu[_INDEX, --, B, unaligned_bytes]
                #endif

                //alu[_INDEX, --, B, unaligned_bytes]
            #endif

            br[end_num_bytes_less_than_5#]

not_equal_extracted_loc#:

            //elif (dest_align < _EXTRACTED_LW_LOC)
            bgt[greater_than_extracted_loc#], defer[2]

            sub_shf_left(shf_left, _EXTRACTED_BYTE_LOC, dest_align, 3)

            #if (!(is_ct_const(out_last_element) || isimport(out_last_element)))

                #if ( (!(is_ct_const(in_prepend) || isimport(in_prepend))) || \
                    (((is_ct_const(in_prepend) || isimport(in_prepend))) && (in_prepend != 0)) )
                    //one instruction needed for <<indirect to work even if it does nothing
                    alu[ --, shf_left, OR, 0]
                    alu_shf[out_last_element, in_prepend, OR, unaligned_bytes, <<indirect]
                #else
                    //one instruction needed for <<indirect to work even if it does nothing
                    alu[ --, shf_left, OR, 0]
                    alu_shf[out_last_element, --, B, unaligned_bytes, <<indirect]
                #endif

                alu[_INDEX, --, B, out_last_element]

            #else

                #if ( (!(is_ct_const(in_prepend) || isimport(in_prepend))) || \
                    (((is_ct_const(in_prepend) || isimport(in_prepend))) && (in_prepend != 0)) )
                    //one instruction needed for <<indirect to work even if it does nothing
                    alu[ --, shf_left, OR, 0]
                    alu_shf[_INDEX, in_prepend, OR, unaligned_bytes, <<indirect]
                #else
                    //one instruction needed for <<indirect to work even if it does nothing
                    alu[ --, shf_left, OR, 0]
                    alu_shf[_INDEX, --, B, unaligned_bytes, <<indirect]
                #endif

                //alu[_INDEX, --, B, unaligned_bytes]

            #endif

            br[end_num_bytes_less_than_5#]

            //.else
greater_than_extracted_loc#:

                alu[shf_right, dest_align, -, _EXTRACTED_LW_LOC]
                alu[extra_bytes, 4, -, shf_right]
                alu_shf[shf_right, --, B, shf_right, <<3]

                #if ( (!(is_ct_const(in_prepend) || isimport(in_prepend))) || \
                    (((is_ct_const(in_prepend) || isimport(in_prepend))) && (in_prepend != 0)) )
                    //one instruction needed for >>indirect to work even if it does nothing
                    alu[ --, shf_right, OR, 0]
                    alu_shf[_INDEX++, in_prepend, OR, unaligned_bytes, >>indirect]
                #else
                    //one instruction needed for >>indirect to work even if it does nothing
                    alu[ --, shf_right, OR, 0]
                    alu_shf[_INDEX++, --, B, unaligned_bytes, >>indirect]
                #endif

                //alu[_INDEX++, --, B, unaligned_bytes]

                //put the bytes to put into 2nd element of destination into vd_temp
                alu_shf[extra_bytes, --, B, extra_bytes, <<3]
                //one instruction needed for <<indirect to work even if it does nothing
                alu[--, extra_bytes, OR, 0]
                #if (!(is_ct_const(out_last_element) || isimport(out_last_element)))
                    alu_shf[out_last_element, --, B, unaligned_bytes, <<indirect]
                    alu[_INDEX, --, B, out_last_element]
                #else
                    alu_shf[_INDEX, --, B, unaligned_bytes, <<indirect]
                #endif

            //.endif
end_num_bytes_less_than_5#:
        #elif (_DEST_XBUF_TYPE == _XBUF_LMEM)
            #error "This case is not supported."
        #else
            #error 2 "Unsupport buffer type"
        #endif

        .end

    #elif (NUMBER_OF_BYTES <= (_SRC_XBUF_SIZE*4))

        #ifdef _SRC_ALIGN
            #warning "_SRC_ALIGN is being redefined"
        #endif
        #define_eval _SRC_ALIGN     (SRC_START_BYTE % 4)

        #ifdef _SRC_1ST_ELEMENT
            #warning "_SRC_1ST_ELEMENT is being redefined"
        #endif
        #define_eval _SRC_1ST_ELEMENT  (SRC_START_BYTE / 4)

        #ifdef _NUM_LW_0
            #warning "_NUM_LW_0 is being redefined"
        #endif
        #ifdef _EXTRA_BYTES_0
            #warning "_EXTRA_BYTES_0 is being redefined"
        #endif
        #ifdef _NUM_LW_1
            #warning "_NUM_LW_1 is being redefined"
        #endif
        #ifdef _EXTRA_BYTES_1
            #warning "_EXTRA_BYTES_1 is being redefined"
        #endif
        #ifdef _NUM_LW_2
            #warning "_NUM_LW_2 is being redefined"
        #endif
        #ifdef _EXTRA_BYTES_2
            #warning "_EXTRA_BYTES_2 is being redefined"
        #endif
        #ifdef _NUM_LW_3
            #warning "_NUM_LW_3 is being redefined"
        #endif
        #ifdef _EXTRA_BYTES_3
            #warning "_EXTRA_BYTES_3 is being redefined"
        #endif

        //_EXTRA_BYTES is related to the destination offset.
        //Destination offset dictates the number of unaligned bytes
        //at the beginning of destination
        //(num_unaligned_bytes = 4- (dest_offset & ~3)
        //Defines different values for all possible values off _EXTRA_BYTES
        //If  (num_unaligned_bytes == 0)
        #define_eval _NUM_LW_0      (NUMBER_OF_BYTES  / 4)
        #define_eval _EXTRA_BYTES_0 (NUMBER_OF_BYTES - (_NUM_LW_0 * 4))
        //If  (num_unaligned_bytes == 1)
        #define_eval _NUM_LW_1      ((NUMBER_OF_BYTES - 1) / 4)
        #define_eval _EXTRA_BYTES_1 ((NUMBER_OF_BYTES - 1) - (_NUM_LW_1 * 4))

        //If  (num_unaligned_bytes == 2)
        #define_eval _NUM_LW_2      ((NUMBER_OF_BYTES - 2) / 4)
        #define_eval _EXTRA_BYTES_2 ((NUMBER_OF_BYTES - 2) - (_NUM_LW_2 * 4))

        //If  (num_unaligned_bytes == 3)
        #define_eval _NUM_LW_3      ((NUMBER_OF_BYTES - 3) / 4)
        #define_eval _EXTRA_BYTES_3 ((NUMBER_OF_BYTES - 3) - (_NUM_LW_3 * 4))

        #if ((_SRC_XBUF_TYPE == _XBUF_SRAM_XFER) || (_SRC_XBUF_TYPE == _XBUF_DRAM_XFER))
            //alu[new_src_start_byte, SRC_START_BYTE, +, num_unaligned_bytes]
            //add_shf_left(vd_temp, new_src_start_byte, &in_src_xbuf/**/_SRC_1ST_ELEMENT, 2)
            //local_csr_wr[T_INDEX_BYTE_INDEX, vd_temp]*

            #error "This case is not supported."

        #elif ( (_SRC_XBUF_TYPE == _XBUF_LMEM) && \
                ((_DEST_XBUF_TYPE == _XBUF_SRAM_XFER) || \
                (_DEST_XBUF_TYPE == _XBUF_DRAM_XFER)) )

            #if (_DEST_XBUF_TYPE == _XBUF_SRAM_XFER)
                #define_eval    _INDEX  *$index
            #elif ( (!defined(__NFP_INDIRECT_REF_FORMAT_NFPXXXX)) && (_DEST_XBUF_TYPE == _XBUF_DRAM_XFER) )
                #define_eval    _INDEX  *$$index
            #endif

            alu[dest_start_lw, dest_start_byte, AND~, 3]

            br=ctx[0, _copy_var_dest_skip_find_abs_addr_2#], defer[2]
            add_shf_left(vd_temp, dest_start_lw, &out_dest_xbuf/**/0, 2)

            //if not context 0, need to add offset to absolute reg to T_INDEX value
            _find_absolute_xfer_reg_addr(vd_temp)

_copy_var_dest_skip_find_abs_addr_2#:
            local_csr_wr[T_INDEX, vd_temp]
            //need to wait 3 cycles before using T_INDEX

            //Take care of unaligned bytes in destination
            alu[dest_align, dest_start_byte, AND, 3]
            beq[var_aligned_dest#], defer [3]
            ;init num_unaligned_bytes, it may be changed later
            alu[num_unaligned_bytes, --, B, 0]
            alu[next_lw, --, B, 0]
            ;If local memory buffer is passed as source, the buffer must have been
            ;activated, ie. ACTIVE_LM_ADDR_x must have been set.
            ;Just need to set BTYE_INDEX. Even if SRC_ALIGN==0, need to set because
            ;BYTE_INDEX may contain different value
            local_csr_wr[BYTE_INDEX, SRC_START_BYTE]

            ;at this point destination_align is not 0
            ;calculate the real num_unaligned_bytes
            alu[num_unaligned_bytes, 4, -, dest_align]

            alu[new_src_start_byte, _SRC_ALIGN, +, num_unaligned_bytes]
            ;next_lw = 1 if new start byte is in the next long word, if not next_lw = 0
            alu_shf[next_lw, --, B, new_src_start_byte, >>2]
            sub_shf_left(new_src_start_byte, new_src_start_byte, next_lw, 2)

            local_csr_wr[BYTE_INDEX, new_src_start_byte]

            xbuf_extract(unaligned_bytes, in_src_xbuf, SRC_START_BYTE, 0, 4)

            alu_shf[vd_temp, --, B, dest_align, <<3]
            //put dest_align in source A reg for the next alu_shf
            alu[--, vd_temp, OR, 0]
            alu_shf[unaligned_bytes, --, B, unaligned_bytes, >>indirect]

            //merge with in_prepend
            #if ( (!(is_ct_const(in_prepend) || isimport(in_prepend))) || \
                (((is_ct_const(in_prepend) || isimport(in_prepend))) && (in_prepend != 0)) )
                alu[unaligned_bytes, unaligned_bytes, OR, in_prepend]
            #endif

            alu[_INDEX/**/++, --, B, unaligned_bytes]


var_aligned_dest#:
        ;At this point, destination is aligned

                .if (next_lw == 1)

                    #define_eval    _NEW_1ST_ELEMENT    (_SRC_1ST_ELEMENT + 1)

                    ;Now, need to find the correct number of longwords and
                    ;extra bytes
                    #if ((_NEW_1ST_ELEMENT + _NUM_LW_0) < _SRC_XBUF_SIZE)
                        alu[--, num_unaligned_bytes, -, 0]
                        beq[zero_unaligned_bytes_1#]
                    #endif

                    #if ((_NEW_1ST_ELEMENT + _NUM_LW_1) < _SRC_XBUF_SIZE)
                        alu[--, num_unaligned_bytes, -, 1]
                        beq[one_unaligned_bytes_1#], defer[1]
                    #endif

                    #if ((_NEW_1ST_ELEMENT + _NUM_LW_2) < _SRC_XBUF_SIZE)
                        alu[--, num_unaligned_bytes, -, 2]
                        beq[two_unaligned_bytes_1#], defer[1]
                    #endif

                    #if ((_NEW_1ST_ELEMENT + _NUM_LW_3) < _SRC_XBUF_SIZE)
                            //num_unaligned_bytes == 3
                            _var_dest_const_src_const_size_perform_byte_align_and_copy_extra( \
                                out_dest_xbuf, out_last_element, dest_start_byte, \
                                in_src_xbuf, _NEW_1ST_ELEMENT, \
                                (SRC_START_BYTE + NUMBER_OF_BYTES - _EXTRA_BYTES_3), \
                                _NUM_LW_3, _EXTRA_BYTES_3)
                            br [end_xbuf_copy#]
                    #endif


zero_unaligned_bytes_1#:
                    #if ((_NEW_1ST_ELEMENT + _NUM_LW_0) < _SRC_XBUF_SIZE)
                        _var_dest_const_src_const_size_perform_byte_align_and_copy_extra ( \
                            out_dest_xbuf, out_last_element, dest_start_byte, \
                            in_src_xbuf, _NEW_1ST_ELEMENT, \
                            (SRC_START_BYTE + NUMBER_OF_BYTES - _EXTRA_BYTES_0), \
                            _NUM_LW_0, _EXTRA_BYTES_0)
                        br [end_xbuf_copy#]
                    #endif

one_unaligned_bytes_1#:
                    #if ((_NEW_1ST_ELEMENT + _NUM_LW_1) < _SRC_XBUF_SIZE)
                        _var_dest_const_src_const_size_perform_byte_align_and_copy_extra ( \
                            out_dest_xbuf, out_last_element, dest_start_byte, \
                            in_src_xbuf, _NEW_1ST_ELEMENT, \
                            (SRC_START_BYTE + NUMBER_OF_BYTES - _EXTRA_BYTES_1), \
                            _NUM_LW_1, _EXTRA_BYTES_1)
                        br [end_xbuf_copy#]
                    #endif

two_unaligned_bytes_1#:
                    #if ((_NEW_1ST_ELEMENT + _NUM_LW_2) < _SRC_XBUF_SIZE)
                        _var_dest_const_src_const_size_perform_byte_align_and_copy_extra( \
                            out_dest_xbuf, out_last_element, dest_start_byte, \
                            in_src_xbuf, _NEW_1ST_ELEMENT, \
                            (SRC_START_BYTE + NUMBER_OF_BYTES - _EXTRA_BYTES_2), \
                            _NUM_LW_2, _EXTRA_BYTES_2)
                        br [end_xbuf_copy#]
                    #endif

                .else

                    #define_eval    i   0
                    //#define_eval  _SRC_INDEX  (_SRC_1ST_ELEMENT + 1)

                    ;Now, need to find the correct number of longwords and
                    ;extra bytes
                    alu[--, num_unaligned_bytes, -, 0]
                    beq[zero_unaligned_bytes_0#], defer[1]
                    alu[--, num_unaligned_bytes, -, 1]
                    beq[one_unaligned_bytes_0#], defer[1]
                    alu[--, num_unaligned_bytes, -, 2]
                    beq[two_unaligned_bytes_0#], defer[1]

                    //num_unaligned_bytes == 3
                    _var_dest_const_src_const_size_perform_byte_align_and_copy_extra( \
                        out_dest_xbuf, out_last_element, dest_start_byte, \
                        in_src_xbuf, _SRC_1ST_ELEMENT, \
                        (SRC_START_BYTE + NUMBER_OF_BYTES - _EXTRA_BYTES_3), \
                        _NUM_LW_3, _EXTRA_BYTES_3)
                    br [end_xbuf_copy#]

zero_unaligned_bytes_0#:
                    _var_dest_const_src_const_size_perform_byte_align_and_copy_extra ( \
                        out_dest_xbuf, out_last_element, dest_start_byte, \
                        in_src_xbuf, _SRC_1ST_ELEMENT, \
                        (SRC_START_BYTE + NUMBER_OF_BYTES - _EXTRA_BYTES_0), \
                        _NUM_LW_0, _EXTRA_BYTES_0)
                    br [end_xbuf_copy#]
one_unaligned_bytes_0#:
                    _var_dest_const_src_const_size_perform_byte_align_and_copy_extra ( \
                        out_dest_xbuf, out_last_element, dest_start_byte, \
                        in_src_xbuf, _SRC_1ST_ELEMENT, \
                        (SRC_START_BYTE + NUMBER_OF_BYTES - _EXTRA_BYTES_1), \
                        _NUM_LW_1, _EXTRA_BYTES_1)
                    br [end_xbuf_copy#]
two_unaligned_bytes_0#:
                    _var_dest_const_src_const_size_perform_byte_align_and_copy_extra( \
                        out_dest_xbuf, out_last_element, dest_start_byte, \
                        in_src_xbuf, _SRC_1ST_ELEMENT, \
                        (SRC_START_BYTE + NUMBER_OF_BYTES - _EXTRA_BYTES_2), \
                        _NUM_LW_2, _EXTRA_BYTES_2)
                    br [end_xbuf_copy#]

                .endif

        #endif  //src is lmem, dest is xfer regs

        #undef _SRC_ALIGN
        #undef _SRC_1ST_ELEMENT
        #undef _NUM_LW_0
        #undef _NUM_LW_1
        #undef _NUM_LW_2
        #undef _NUM_LW_3
        #undef _EXTRA_BYTES_0
        #undef _EXTRA_BYTES_1
        #undef _EXTRA_BYTES_2
        #undef _EXTRA_BYTES_3
        #undef _SRC_XBUF_TYPE
        #undef _SRC_XBUF_SIZE
        #undef _DEST_XBUF_TYPE

        #ifdef _NEW_1ST_ELEMENT
            #undef _NEW_1ST_ELEMENT
        #endif

    #else //if (NUMBER_OF_BYTES <= 64)
        #error "Maximum supported number of bytes is 64 (equals to maximum buffer size)"
    #endif

.end
#endm
/// @endcond


/// @cond INTERNAL_MACROS
/** An internal macro used by xbuf_insert.
 */
#macro _var_dest_const_src_const_size_perform_byte_align_and_copy_extra(out_dest_xbuf, \
                out_last_element, dest_start_byte, \
                in_src_xbuf, SRC_FIRST_ELEMENT, SRC_LAST_ELE_ADDR, \
                NUM_LONG_WORDS, NUM_EXTRA_BYTES)
.begin

    .reg vdp_temp
    .set vdp_temp

    #if ( !((is_ct_const(SRC_FIRST_ELEMENT) || isimport(SRC_FIRST_ELEMENT)) && \
        (is_ct_const(NUM_LONG_WORDS) || isimport(NUM_LONG_WORDS)) && \
        (is_ct_const(NUM_EXTRA_BYTES) || isimport(NUM_EXTRA_BYTES)) && \
        (is_ct_const(SRC_LAST_ELE_ADDR) || isimport(SRC_LAST_ELE_ADDR))) )
        #error 2 "_var_dest_const_src_const_size_perform_byte_align_and_copy_extra: \
                    SRC_FIRST_ELEMENT, NUM_LONG_WORDS, NUM_EXTRA_BYTES, \
                    DEST_LAST_ELE_OFFSET, SRC_LAST_ELE_ADDR must be constants."

    #elif ((SRC_FIRST_ELEMENT + NUM_LONG_WORDS) >= _SRC_XBUF_SIZE)

        //This case should only happen during compilation. Do nothing.

    #else
        #if ( (_SRC_XBUF_TYPE == _XBUF_LMEM) && \
                ( (_DEST_XBUF_TYPE == _XBUF_SRAM_XFER) || \
                (_DEST_XBUF_TYPE == _XBUF_DRAM_XFER) )  )

            #define_eval _TEMP_BUF_SIZE 0

            #if (_DEST_XBUF_TYPE == _XBUF_SRAM_XFER)
                #define_eval    _INDEX  *$index
            #elif ( (!defined(__NFP_INDIRECT_REF_FORMAT_NFPXXXX)) && (_DEST_XBUF_TYPE == _XBUF_DRAM_XFER) )
                #define_eval    _INDEX  *$$index
            #endif

            #if (NUM_LONG_WORDS > 0)

                #if ((SRC_FIRST_ELEMENT + NUM_LONG_WORDS) > 8)

                    #define_eval _TEMP_BUF_SIZE ((SRC_FIRST_ELEMENT + NUM_LONG_WORDS ) - 7)

                    xbuf_alloc (vdp_temp_buf, _TEMP_BUF_SIZE, read_write)

                    #define_eval    i   0
                    #while (i < _TEMP_BUF_SIZE)
                        .set vdp_temp_buf[i]
                        #define_eval    i (i + 1)
                    #endloop
                    ;moving in_src_xbuf[8-15] to vdp_temp_buf
                    ;because byte_align_be doesn't allow local memory index > 7
                    #define_eval    i   8
                    #while (i <= (SRC_FIRST_ELEMENT + NUM_LONG_WORDS))

                        #define_eval    j   (i - 8)
                        #if (j < _TEMP_BUF_SIZE)
                            alu[vdp_temp_buf[/**/j], --, B, in_src_xbuf[/**/i]]
                        #endif
                        #define_eval    i   (i + 1)
                    #endloop

                #endif

                #if (SRC_FIRST_ELEMENT < 8)
                    byte_align_be[--, in_src_xbuf[/**/SRC_FIRST_ELEMENT]]
                #else
                    #define_eval _FIRST (SRC_FIRST_ELEMENT - 8)
                    byte_align_be[--, vdp_temp_buf[/**/_FIRST]]
                    #undef _FIRST
                #endif

                #define_eval    i   0
                #define_eval    _CONTINUE   (SRC_FIRST_ELEMENT + 1)


                #while (i < NUM_LONG_WORDS)

                    #if (_CONTINUE < 8)

                        byte_align_be[_INDEX/**/++, in_src_xbuf[/**/_CONTINUE]]

                    #else
                        #define_eval    _VDP_TEMP_INDEX (_CONTINUE - 8)
                        #if (_VDP_TEMP_INDEX < _TEMP_BUF_SIZE)
                            byte_align_be[_INDEX/**/++, vdp_temp_buf[_VDP_TEMP_INDEX]]
                        #elif (_VDP_TEMP_INDEX == _TEMP_BUF_SIZE)
                            byte_align_be[_INDEX/**/++, 0]
                        #endif
                    #endif

                    #define_eval    _CONTINUE   (_CONTINUE + 1)
                    #define_eval    i   (i+1)
                #endloop

                #undef  _CONTINUE
                #undef i
                #ifdef  _VDP_TEMP_INDEX
                    #undef  _VDP_TEMP_INDEX
                #endif

            #endif //if (NUM_LONG_WORDS > 0)

            #if (NUM_EXTRA_BYTES > 0)
                #if (!(is_ct_const(out_last_element) || isimport(out_last_element)))

                    xbuf_extract(out_last_element, in_src_xbuf,
                        SRC_LAST_ELE_ADDR, 0, NUM_EXTRA_BYTES)

                    _GET_LSHIFT_FROM_REMAINING_BYTES(NUM_EXTRA_BYTES)
                    alu_shf[out_last_element, --, B, out_last_element, \
                            <</**/_L_SHIFT_VAL_FROM_REMAINING_BYTES]
                    #undef _L_SHIFT_VAL_FROM_REMAINING_BYTES

                    alu[_INDEX, --, B, out_last_element]

                #else
                    xbuf_extract(vdp_temp, in_src_xbuf,
                        SRC_LAST_ELE_ADDR, 0, NUM_EXTRA_BYTES)

                    _GET_LSHIFT_FROM_REMAINING_BYTES(NUM_EXTRA_BYTES)
                    alu_shf[vdp_temp, --, B, vdp_temp, <</**/_L_SHIFT_VAL_FROM_REMAINING_BYTES]
                    #undef _L_SHIFT_VAL_FROM_REMAINING_BYTES

                    alu[_INDEX, --, B, vdp_temp]

                #endif

            #else //if (NUM_EXTRA_BYTES > 0)
                #if (!(is_ct_const(out_last_element) || isimport(out_last_element)))
                    ;fill out_last_element
                    #if (_DEST_XBUF_TYPE == _XBUF_LMEM)
                        alu[out_last_element, --, B, out_dest_xbuf[NUM_LONG_WORDS]]
                    #elif ( (_DEST_XBUF_TYPE == _XBUF_SRAM_XFER) || \
                            (_DEST_XBUF_TYPE == _XBUF_DRAM_XFER) )
                        xbuf_extract(out_last_element, in_src_xbuf, (SRC_LAST_ELE_ADDR-4), 0, 4)
                    #else
                        #error 2 "Unsupported _DEST_XBUF_TYPE "
                    #endif
                #endif

            #endif

            #if ((SRC_FIRST_ELEMENT + NUM_LONG_WORDS) > 8)
                xbuf_free (vdp_temp_buf)
            #endif

            #undef  _TEMP_BUF_SIZE

        #else
            #error "This case is not supported."
        #endif

    #endif

.end
#endm
/// @endcond


/// @cond INTERNAL_MACROS
/**
//  destination offset: constant
//  source offset:      run-time (variable)
//  size:               constant
 */
#macro _copy_var_src_const_dest_and_size(out_dest_xbuf, out_last_element, \
                            DEST_START_BYTE, in_src_xbuf, src_start_byte, \
                            in_prepend, NUMBER_OF_BYTES)
.begin

    .reg src_align new_src_start_byte vs_temp
    .reg unaligned_bytes

    .set src_align new_src_start_byte vs_temp
    .set unaligned_bytes

    #if (!(is_ct_const(DEST_START_BYTE) || isimport(DEST_START_BYTE)))
        #error 2   "_copy_var_src_const_src_and_size: DEST_START_BYTE must be constant"
    #endif

    #if (!(is_ct_const(NUMBER_OF_BYTES) || isimport(NUMBER_OF_BYTES)))
        #error 2   "_copy_var_src_const_src_and_size: NUMBER_OF_BYTES must be constant"
    #endif


    _GET_XBUF_TYPE(in_src_xbuf)
    #define_eval _SRC_XBUF_TYPE     _XBUF_TYPE
    #undef _XBUF_TYPE

    _GET_XBUF_TYPE(out_dest_xbuf)
    #define_eval _DEST_XBUF_TYPE    _XBUF_TYPE
    #undef _XBUF_TYPE

    #ifdef _DEST_ALIGN
        #warning "_DEST_ALIGN is being redefined"
    #endif
    #define_eval _DEST_ALIGN    (DEST_START_BYTE % 4)

    #ifdef _DEST_FIRST_BIT_LOC
        #warning "_DEST_FIRST_BIT_LOC is being redefined"
    #endif
    #define_eval _DEST_FIRST_BIT_LOC    (_DEST_ALIGN * 8)

    #ifdef _DEST_1ST_ELEMENT
        #warning "_DEST_1ST_ELEMENT is being redefined"
    #endif
    #define_eval _DEST_1ST_ELEMENT  (DEST_START_BYTE / 4)

    #ifdef _DEST_2ND_ELEMENT
        #warning "_DEST_2ND_ELEMENT is being redefined"
    #endif
    #define_eval _DEST_2ND_ELEMENT  (_DEST_1ST_ELEMENT + 1)

    #ifdef _EXTRA_BYTES
        #warning "_EXTRA_BYTES is being redefined"
    #endif

    #if ( streq(out_dest_xbuf, in_src_xbuf) )
        ; For GPR and local memory xbuf types we do not perform copy when src & dest
        ; are exactly the same (same name with same start byte)
        #if ( (_SRC_XBUF_TYPE == _XBUF_GPR) || (_SRC_XBUF_TYPE == _XBUF_LMEM) )
            alu[--, src_start_byte, -, DEST_START_BYTE)
            beq [end_xbuf_copy#]
        #endif
    #endif

    #if (NUMBER_OF_BYTES < 5)
        #define_eval _EXTRA_BYTES (NUMBER_OF_BYTES - (4 - _DEST_ALIGN))

        #if ( (_SRC_XBUF_TYPE == _XBUF_SRAM_XFER) || \
            (_SRC_XBUF_TYPE == _XBUF_DRAM_XFER) )

        .begin

            .reg vs_mask_extra vs_mask

            #if (_SRC_XBUF_TYPE == _XBUF_SRAM_XFER)
                #define_eval    _INDEX  *$index
            #elif ( (!defined(__NFP_INDIRECT_REF_FORMAT_NFPXXXX)) && (_SRC_XBUF_TYPE == _XBUF_DRAM_XFER) )
                #define_eval    _INDEX  *$$index
            #endif

            br=ctx[0, _copy_var_src_skip_find_abs_addr_1#], defer[2]
            add_shf_left(vs_temp, src_start_byte, &in_src_xbuf[0], 2)

            //if not context 0, need to add offset to absolute reg to T_INDEX value
            _find_absolute_xfer_reg_addr(vs_temp)

_copy_var_src_skip_find_abs_addr_1#:
            local_csr_wr[T_INDEX_BYTE_INDEX, vs_temp]
            //need 3 cycles before the CSR T_INDEX_BYTE_INDEX can be used
            #if (NUMBER_OF_BYTES > (4 - _DEST_ALIGN))

                #if (_EXTRA_BYTES == 1)
                    immed32(vs_mask_extra, 0xff000000)
                #elif (_EXTRA_BYTES == 2)
                    immed32(vs_mask_extra, 0xffff0000)
                #elif (_EXTRA_BYTES == 3)
                    immed32(vs_mask_extra, 0xffffff00)
                #endif
                nop
                nop
            #else
                #if (_DEST_FIRST_BIT_LOC == 0)
                    #if (NUMBER_OF_BYTES == 1)
                        immed[vs_mask, 0xff000000]
                    #elif (NUMBER_OF_BYTES == 2)
                        immed32(vs_mask, 0xffff0000)
                    #elif (NUMBER_OF_BYTES == 3)
                        immed32(vs_mask, 0xffffff00)
                    #else
                        nop
                    #endif
                #else
                    #if ((NUMBER_OF_BYTES + _DEST_ALIGN) == 2)
                        immed32(vs_mask, 0xffff0000)
                    #elif ((NUMBER_OF_BYTES + _DEST_ALIGN) == 3)
                        immed32(vs_mask, 0xffffff00)
                    #else
                        nop
                    #endif
                #endif

                nop
                nop
            #endif

            //alu[vs_temp, --, B, _INDEX/**/++]
            byte_align_be[--, _INDEX/**/++]
            byte_align_be[vs_temp, _INDEX]

        #elif (_SRC_XBUF_TYPE == _XBUF_LMEM)

            #error "This case is not supported."
        #else
            #error "Unsupported buffer type"
        #endif

        //alu[vs_temp, vs_temp, AND, vs_mask]
        #if (NUMBER_OF_BYTES > (4 - _DEST_ALIGN))
            #define_eval _SHF_LEFT  ( 8 * (4 - _DEST_ALIGN) )

            alu_shf[unaligned_bytes, --, B, vs_temp, <</**/_SHF_LEFT]
            alu[unaligned_bytes, unaligned_bytes, AND, vs_mask_extra]
            alu[out_dest_xbuf[/**/_DEST_2ND_ELEMENT], --, B, unaligned_bytes]
            #undef _SHF_LEFT
        #endif

        #if (_DEST_FIRST_BIT_LOC == 0)
            #if (NUMBER_OF_BYTES < 4)
                alu[out_dest_xbuf[/**/_DEST_1ST_ELEMENT], vs_temp, AND, vs_mask]
            #else
                alu[out_dest_xbuf[/**/_DEST_1ST_ELEMENT], --, B, vs_temp]
            #endif

            #if (!(is_ct_const(out_last_element) || isimport(out_last_element)))
                alu[out_last_element, --, B, out_dest_xbuf[/**/_DEST_1ST_ELEMENT]]
            #endif
        #else
            #if (!(is_ct_const(out_last_element) || isimport(out_last_element)) && \
                (NUMBER_OF_BYTES <= (4 - _DEST_ALIGN)))
                #if ( (!(is_ct_const(in_prepend) || isimport(in_prepend))) || \
                    (((is_ct_const(in_prepend) || isimport(in_prepend))) && (in_prepend != 0)) )
                    alu_shf[out_last_element, in_prepend, OR, vs_temp, >>_DEST_FIRST_BIT_LOC]
                #else
                    alu_shf[out_last_element, --, B, vs_temp, >>_DEST_FIRST_BIT_LOC]
                #endif

                #if ( ((NUMBER_OF_BYTES + _DEST_ALIGN) == 2) || \
                    ((NUMBER_OF_BYTES + _DEST_ALIGN) == 3) )
                    alu[out_dest_xbuf[/**/_DEST_1ST_ELEMENT], out_last_element, AND, vs_mask]
                #else
                    alu[out_dest_xbuf[/**/_DEST_1ST_ELEMENT], --, B, out_last_element]
                #endif
            #else
                #if ( (!(is_ct_const(in_prepend) || isimport(in_prepend))) || \
                    (((is_ct_const(in_prepend) || isimport(in_prepend))) && (in_prepend != 0)) )
                    alu_shf[out_dest_xbuf[/**/_DEST_1ST_ELEMENT], in_prepend, OR, vs_temp, >>_DEST_FIRST_BIT_LOC]
                #else
                    #if ( ((NUMBER_OF_BYTES + _DEST_ALIGN) == 2) || \
                        ((NUMBER_OF_BYTES + _DEST_ALIGN) == 3) )
                        alu_shf[vs_temp, --, B, vs_temp, >>_DEST_FIRST_BIT_LOC]
                        alu[out_dest_xbuf[/**/_DEST_1ST_ELEMENT], vs_temp, AND, vs_mask]
                    #else
                        alu[out_dest_xbuf[/**/_DEST_1ST_ELEMENT], --, B, vs_temp, >>_DEST_FIRST_BIT_LOC]
                    #endif
                #endif
                #if (NUMBER_OF_BYTES > (4 - _DEST_ALIGN))
                    alu[out_last_element, --, B, unaligned_bytes]
                #endif
            #endif
        #endif



        .end
        #undef _EXTRA_BYTES

    #else //more than 5 bytes

        #ifdef _NUM_UNALIGNED_BYTES
            #warning "_NUM_UNALIGNED_BYTES is being redefined"
        #endif
        #if (_DEST_ALIGN > 0)
            #define_eval _NUM_UNALIGNED_BYTES  (4 - _DEST_ALIGN)
        #else
            #define_eval _NUM_UNALIGNED_BYTES  0
        #endif

        #ifdef _NUM_LW
            #warning "_NUM_LW is being redefined"
        #endif

        //_NUM_LW to byte_align and _EXTRA_BYTES are realted to destination offset.
        //Destination offset dictates the number of unaligned bytes
        //at the beginning of destination. These bytes will be taken care of before
        //doing byte_align.
        #define_eval _NUM_LW        ((NUMBER_OF_BYTES - _NUM_UNALIGNED_BYTES) / 4)
        #define_eval _EXTRA_BYTES   ((NUMBER_OF_BYTES - _NUM_UNALIGNED_BYTES) - (_NUM_LW * 4))

        #if ((_SRC_XBUF_TYPE == _XBUF_SRAM_XFER) || (_SRC_XBUF_TYPE == _XBUF_DRAM_XFER))

            #if (_SRC_XBUF_TYPE == _XBUF_SRAM_XFER)
                #define_eval    _INDEX  *$index
            #elif ( (!defined(__NFP_INDIRECT_REF_FORMAT_NFPXXXX)) && (_SRC_XBUF_TYPE == _XBUF_DRAM_XFER) )
                #define_eval    _INDEX  *$$index
            #endif

            #if (_NUM_UNALIGNED_BYTES > 0)

                ;init T_INDEX_BYTE_INDEX to the first element in source buffer

                br=ctx[0, _copy_var_src_skip_find_abs_addr_2#], defer[2]
                add_shf_left(vs_temp, src_start_byte, &in_src_xbuf[0], 2)

                //if not context 0, need to add offset to absolute reg to T_INDEX value
                _find_absolute_xfer_reg_addr(vs_temp)

_copy_var_src_skip_find_abs_addr_2#:

                local_csr_wr[T_INDEX_BYTE_INDEX, vs_temp]
                //need 3 cycles before the CSR T_INDEX_BYTE_INDEX can be used
                nop
                alu[new_src_start_byte, src_start_byte, +, _NUM_UNALIGNED_BYTES]
                //alu[vs_temp2, vs_temp, +, _NUM_UNALIGNED_BYTES]
                alu[vs_temp, vs_temp, +, _NUM_UNALIGNED_BYTES]

                byte_align_be[--, _INDEX/**/++]
                byte_align_be[unaligned_bytes, _INDEX]


                //load new src start location to T_INDEX_BYTE_INDEX again
                local_csr_wr[T_INDEX_BYTE_INDEX, vs_temp]

                //merge with in_prepend
                #if ( (!(is_ct_const(in_prepend) || isimport(in_prepend))) || \
                    (((is_ct_const(in_prepend) || isimport(in_prepend))) && (in_prepend != 0)) )
                    alu_shf[unaligned_bytes, in_prepend, OR, unaligned_bytes, >>_DEST_FIRST_BIT_LOC]
                #else
                    alu_shf[unaligned_bytes, --, B, unaligned_bytes, >>_DEST_FIRST_BIT_LOC]
                #endif

                alu[out_dest_xbuf[_DEST_1ST_ELEMENT], --, B, unaligned_bytes]
                nop
            #else
                ;init T_INDEX_BYTE_INDEX to the first element in source buffer

                br=ctx[0, _copy_var_src_skip_find_abs_addr_3#], defer[2]
                add_shf_left(vs_temp, src_start_byte, &in_src_xbuf[0], 2)

                //if not context 0, need to add offset to absolute reg to T_INDEX value
                _find_absolute_xfer_reg_addr(vs_temp)

_copy_var_src_skip_find_abs_addr_3#:

                local_csr_wr[T_INDEX_BYTE_INDEX, vs_temp]
                //need 3 cycles before the CSR T_INDEX_BYTE_INDEX can be used
                nop
                nop
                nop
            #endif

        #elif (_SRC_XBUF_TYPE == _XBUF_LMEM)
            #error "This case is not supported."
        #else
            #error "Unsupport buffer type"
        #endif

        ;At this point, destination is aligned
        #define_eval _EXTRA_BYTES_OFFSET (NUMBER_OF_BYTES - _NUM_UNALIGNED_BYTES - _EXTRA_BYTES)

        #if (_NUM_UNALIGNED_BYTES == 0)
            _var_src_const_dest_const_size_perform_byte_align_and_copy_extra( \
                out_dest_xbuf, out_last_element, _DEST_1ST_ELEMENT, \
                in_src_xbuf, src_start_byte, \
                _EXTRA_BYTES_OFFSET, _NUM_LW, _EXTRA_BYTES)
        #else
            //destination not aligned, the 1st element has been filled already
            _var_src_const_dest_const_size_perform_byte_align_and_copy_extra( \
                out_dest_xbuf, out_last_element, _DEST_2ND_ELEMENT, \
                in_src_xbuf, new_src_start_byte, \
                _EXTRA_BYTES_OFFSET, _NUM_LW, _EXTRA_BYTES)
        #endif
        br [end_xbuf_copy#]


        #undef _NUM_LW
        #undef _EXTRA_BYTES
        #undef _NUM_UNALIGNED_BYTES

    #endif //if (NUMBER_OF_BYTES < 4)
//br [end_xbuf_copy#]

    #undef _SRC_XBUF_TYPE
    #undef _DEST_XBUF_TYPE
    #undef _DEST_ALIGN
    #undef _DEST_FIRST_BIT_LOC
    #undef _DEST_1ST_ELEMENT
    #undef _DEST_2ND_ELEMENT

.end
#endm
/// @endcond


/// @cond INTERNAL_MACROS
/** An internal macro used by xbuf_insert.
 */
#macro _var_src_const_dest_const_size_perform_byte_align_and_copy_extra( \
                        out_dest_xbuf, out_last_element, DEST_START_ELEMENT, \
                        in_src_xbuf, src_start_byte, \
                        OFFSET_FOR_EXTRA_BYTES, NUM_LONG_WORDS, NUM_EXTRA_BYTES)

.begin

    .reg    vsp_temp vsp_temp2 vsp_mask
    .set    vsp_temp vsp_temp2 vsp_mask

    #if ( !((is_ct_const(DEST_START_ELEMENT) || isimport(DEST_START_ELEMENT)) && \
            (is_ct_const(NUM_LONG_WORDS) || isimport(NUM_LONG_WORDS)) && \
            (is_ct_const(NUM_EXTRA_BYTES) || isimport(NUM_EXTRA_BYTES)) && \
            (is_ct_const(OFFSET_FOR_EXTRA_BYTES) || isimport(OFFSET_FOR_EXTRA_BYTES))) )
        #error 2 "_var_src_const_dest_const_size_perform_byte_align_and_copy_extra: \
                    DEST_START_ELEMENT, NUM_LONG_WORDS, NUM_EXTRA_BYTES, \
                    OFFSET_FOR_EXTRA_BYTES must be constants."
    #else


        #if (NUM_EXTRA_BYTES == 1)
            immed32(vsp_mask, 0xff000000)
        #elif (NUM_EXTRA_BYTES == 2)
            immed32(vsp_mask, 0xffff0000)
        #elif (NUM_EXTRA_BYTES == 3)
            immed32(vsp_mask, 0xffffff00)
        #endif


        #if ( (_DEST_XBUF_TYPE == _XBUF_LMEM) && \
                ( (_SRC_XBUF_TYPE == _XBUF_SRAM_XFER) || \
                (_SRC_XBUF_TYPE == _XBUF_DRAM_XFER) ) \
            )

            #if (_SRC_XBUF_TYPE == _XBUF_SRAM_XFER)
                #define_eval    _INDEX  *$index
            #elif ( (!defined(__NFP_INDIRECT_REF_FORMAT_NFPXXXX)) && (_SRC_XBUF_TYPE == _XBUF_DRAM_XFER) )
                #define_eval    _INDEX  *$$index
            #endif

            #if ((DEST_START_ELEMENT + NUM_LONG_WORDS) > 8)
                #define_eval _SIZE ((DEST_START_ELEMENT + NUM_LONG_WORDS) - 8)
                xbuf_alloc (vsp_temp_buf, _SIZE, read_write)
                #undef _SIZE
            #endif

            #if (NUM_LONG_WORDS > 0)

                byte_align_be[--, _INDEX/**/++]

                #define_eval    i   0
                #define_eval    _CONTINUE   (DEST_START_ELEMENT)

                #while (i < NUM_LONG_WORDS)
                    #if (_CONTINUE < 8)
                        byte_align_be[out_dest_xbuf[/**/_CONTINUE], _INDEX/**/++]

                    #else
                        #define_eval _TEMP_BUF_INDEX    (_CONTINUE - 8)

                        ;byte_align_be doesn't allow local memory index > 7
                        byte_align_be[vsp_temp_buf[_TEMP_BUF_INDEX], _INDEX/**/++]

                    #endif

                    #define_eval    _CONTINUE   (_CONTINUE + 1)
                    #define_eval    i   (i+1)
                #endloop

                #undef  _CONTINUE
                #undef i
                #ifdef _TEMP_BUF_INDEX
                    #undef _TEMP_BUF_INDEX
                #endif
            #endif //if (NUM_LONG_WORDS > 0)

            #if (NUM_EXTRA_BYTES > 0)

                #define_eval _DEST_LAST_ELE     (DEST_START_ELEMENT + NUM_LONG_WORDS)

                #if (NUM_LONG_WORDS == 0)
                    byte_align_be[--, _INDEX/**/++]
                #endif

                #if (!(is_ct_const(out_last_element) || isimport(out_last_element)))

                    byte_align_be[out_last_element, _INDEX]

                    alu[out_last_element, out_last_element, AND, vsp_mask]
                    alu[out_dest_xbuf[_DEST_LAST_ELE], --, B, out_last_element]

                #else

                    byte_align_be[vsp_temp, _INDEX]

                    alu[vsp_temp, vsp_temp, AND, vsp_mask]
                    alu[out_dest_xbuf[_DEST_LAST_ELE], --, B, vsp_temp]
                #endif
            #endif

            #if ((DEST_START_ELEMENT + NUM_LONG_WORDS) > 8)
                #define_eval i  8
                #while ( i < (DEST_START_ELEMENT + NUM_LONG_WORDS))
                    #define_eval j  (i - 8)
                    alu[out_dest_xbuf[i], --, B, vsp_temp_buf[j]]
                    #define_eval i  (i + 1)
                #endloop

                xbuf_free (vsp_temp_buf)
            #endif

            #if (NUM_EXTRA_BYTES == 0)
                #if (!(is_ct_const(out_last_element) || isimport(out_last_element)))
                    ;fill out_last_element
                    #define_eval i  (DEST_START_ELEMENT + NUM_LONG_WORDS - 1)
                    alu[out_last_element, --, B, out_dest_xbuf[/**/i]]
                    #undef i
                #endif
            #endif  //if (NUM_EXTRA_BYTES > 0)

        #else
            #error "This case is not supported."
        #endif

    #endif

.end
#endm
/// @endcond


/// @cond INTERNAL_MACROS
/**
//  destination offset: run-time variable
//  source offset:      constant
//  size:               run-time variable
 */
#macro _copy_var_dest_var_size_const_src(out_dest_xbuf, out_last_element, \
                            dest_start_byte, in_src_xbuf, SRC_START_BYTE, \
                            in_prepend, number_of_bytes)
.begin

    .reg dest_align num_unaligned_bytes unaligned_bytes
    .reg dest_start_lw new_src_start_byte next_lw
    .reg vd_temp num_lw new_num_bytes extra_bytes

    .set dest_align num_unaligned_bytes unaligned_bytes
    .set dest_start_lw new_src_start_byte next_lw
    .set vd_temp num_lw new_num_bytes extra_bytes

    #if (!(is_ct_const(SRC_START_BYTE) || isimport(SRC_START_BYTE)))
        #error 2   "_copy_var_dest_const_src_and_size: SRC_START_BYTE must be constant"
    #endif

    _GET_XBUF_TYPE(in_src_xbuf)
    #define_eval _SRC_XBUF_TYPE     _XBUF_TYPE
    #undef _XBUF_TYPE

    #if (_SRC_XBUF_TYPE == _XBUF_LMEM)
        _DEFINE_THIS_XBUF(in_src_xbuf)
        #define_eval _SRC_LMEM_INDEX    _this_xbuf_index
        #define_eval _SRC_XBUF_SIZE _this_xbuf_size
        _UNDEFINE_THIS_XBUF()
    #endif

    _GET_XBUF_TYPE(out_dest_xbuf)
    #define_eval _DEST_XBUF_TYPE    _XBUF_TYPE
    #undef _XBUF_TYPE


    //#if ( streq(out_dest_xbuf, in_src_xbuf) )
    //  ; For GPR and local memory xbuf types we do not perform copy when src & dest
    //  ; are exactly the same (same name with same start byte)
    //  #if ( (_SRC_XBUF_TYPE == _XBUF_GPR) || (_SRC_XBUF_TYPE == _XBUF_LMEM) )
    //      alu[--, dest_start_byte, -, SRC_START_BYTE)
    //      beq [end_xbuf_copy#]
    //  #endif
    //#endif

    //do not take care of < 5 bytes separately to save code space
    //#else

        #ifdef _SRC_ALIGN
            #warning "_SRC_ALIGN is being redefined"
        #endif
        #define_eval _SRC_ALIGN     (SRC_START_BYTE % 4)

        #ifdef _SRC_1ST_ELEMENT
            #warning "_SRC_1ST_ELEMENT is being redefined"
        #endif
        #define_eval _SRC_1ST_ELEMENT  (SRC_START_BYTE / 4)


        #if ((_SRC_XBUF_TYPE == _XBUF_SRAM_XFER) || (_SRC_XBUF_TYPE == _XBUF_DRAM_XFER))
            //alu[new_src_start_byte, src_start_byte, +, num_unaligned_bytes]
            //add_shf_left(vd_temp, new_src_start_byte, &in_src_xbuf/**/_SRC_1ST_ELEMENT, 2)
            //local_csr_wr[T_INDEX_BYTE_INDEX, vd_temp]*

            #error "This case is not supported."

        #elif ( (_SRC_XBUF_TYPE == _XBUF_LMEM) && \
                ((_DEST_XBUF_TYPE == _XBUF_SRAM_XFER) || \
                (_DEST_XBUF_TYPE == _XBUF_DRAM_XFER)) )

            #if (_DEST_XBUF_TYPE == _XBUF_SRAM_XFER)
                #define_eval    _INDEX  *$index
            #elif ( (!defined(__NFP_INDIRECT_REF_FORMAT_NFPXXXX)) && (_DEST_XBUF_TYPE == _XBUF_DRAM_XFER) )
                #define_eval    _INDEX  *$$index
            #endif

            alu[dest_start_lw, dest_start_byte, AND~, 3]

            br=ctx[0, _copy_var_dest_var_size_skip_find_abs_addr_1#], defer[2]
            add_shf_left(vd_temp, dest_start_lw, &out_dest_xbuf/**/0, 2)

            //if not context 0, need to add offset to absolute reg to T_INDEX value
            _find_absolute_xfer_reg_addr(vd_temp)

_copy_var_dest_var_size_skip_find_abs_addr_1#:

            local_csr_wr[T_INDEX, vd_temp]
            //need to wait 3 cycles before using T_INDEX

            ;if we want this macro to work for aligned case also, we need to
            ;init new_src_start_byte to 0. Since this macro is only called for
            ;unaligned bytes, just not do it to save 1 instruction
            alu[new_src_start_byte, --, B, 0]

            //Take care of unaligned bytes in destination
            alu[dest_align, dest_start_byte, AND, 3]
            beq[var_aligned_dest_var_size#], defer [3]
            ;init num_unaligned_bytes and next_lw, they may be changed later
            alu[num_unaligned_bytes, --, B, 0]
            alu[next_lw, --, B, 0]
            ;If local memory buffer is passed as source, the buffer must have been
            ;activated, ie. ACTIVE_LM_ADDR_x must have been set.
            ;Just need to set BTYE_INDEX. Even if SRC_ALIGN==0, need to set because
            ;BYTE_INDEX may contain different value
            local_csr_wr[BYTE_INDEX, _SRC_ALIGN]
            //need to wait 3 cycles before using BYTE_INDEX

            ;at this point destination_align is certainly not 0.
            ;calculate the real num_unaligned_bytes
            alu[num_unaligned_bytes, 4, -, dest_align]

            alu[new_src_start_byte, _SRC_ALIGN, +, num_unaligned_bytes]
            ;next_lw = 1 if new start byte is in the next long word, if not next_lw = 0
            alu_shf[next_lw, --, B, new_src_start_byte, >>2]
            sub_shf_left(new_src_start_byte, new_src_start_byte, next_lw, 2)
            local_csr_wr[BYTE_INDEX, new_src_start_byte]

            xbuf_extract(unaligned_bytes, in_src_xbuf, SRC_START_BYTE, 0, 4)

            alu_shf[vd_temp, --, B, dest_align, <<3]
            //put dest_align in source A reg for the next alu_shf
            alu[--, vd_temp, OR, 0]
            alu_shf[unaligned_bytes, --, B, unaligned_bytes, >>indirect]

            //merge with in_prepend
            #if ( (!(is_ct_const(in_prepend) || isimport(in_prepend))) || \
                ((is_ct_const(in_prepend) || isimport(in_prepend)) && (in_prepend != 0)) )
                alu[unaligned_bytes, unaligned_bytes, OR, in_prepend]
            #endif

            alu[_INDEX/**/++, --, B, unaligned_bytes]


var_aligned_dest_var_size#:
            ;At this point, destination is aligned
            #if (!(is_ct_const(out_last_element) || isimport(out_last_element)))
                #define_eval _DF 2
            #else
                #define_eval _DF 1
            #endif

            alu[new_num_bytes, number_of_bytes, -, num_unaligned_bytes]
            beq[end_copy_vdvsz#], defer[_DF]
            #undef _DF

            #if (!(is_ct_const(out_last_element) || isimport(out_last_element)))
                alu[out_last_element, --, B, unaligned_bytes]
            #endif

            alu[--, next_lw, -, 0]
            beq[vdvsz_orig_src_lw#], defer[3]
            ;calculate the extra bytes left over after byte_align is done
            alu_shf[num_lw, --, B, new_num_bytes, >>2]
            sub_shf_left(extra_bytes, new_num_bytes, num_lw, 2)

            #define_eval    _SRC    (SRC_START_BYTE + 4)
            _var_dest_var_size_const_src_perform_byte_align_and_copy_extra( \
                            out_dest_xbuf, out_last_element, dest_start_byte, \
                            in_src_xbuf, _SRC, new_src_start_byte, num_lw, extra_bytes)
            #undef  _SRC

            br[end_xbuf_copy#]

vdvsz_orig_src_lw#:
            _var_dest_var_size_const_src_perform_byte_align_and_copy_extra( \
                            out_dest_xbuf, out_last_element, dest_start_byte, \
                            in_src_xbuf, SRC_START_BYTE, new_src_start_byte, \
                            num_lw, extra_bytes)

end_copy_vdvsz#:
            #undef _SRC_ALIGN
            #undef _SRC_1ST_ELEMENT
            #undef _SRC_XBUF_TYPE
            #undef _DEST_XBUF_TYPE

            #ifdef _NEW_1ST_ELEMENT
                #undef _NEW_1ST_ELEMENT
            #endif

        #endif  //src is lmem, dest is xfer regs


    //#endif //do not treat < 5 bytes separately to save code space

.end
#endm
/// @endcond


/// @cond INTERNAL_MACROS
/**  The strategy used in this macro to deal with run-time destination offset
 * and run-time size, is to copy 4 long-words (16 bytes) at a time into a temporary
 * gpr buffer, then put the number of bytes needed into the destination.
 *
 * This way, it solves two problems: @n
 *  -# byte_align_be does not allow incrementing index in both source and dest. @n
 *  -# byte_align_be instructions have to be next to each other, ie. inserting
 *     an alu instruction to get the result for every long-word will "break"
 *     the byte_align_be stream.
 *
 * This strategy will put some penalty on the cases where the number of bytes to
 * copy is less than 4 long-words. But copying a only few bytes AND having run-time
 * size is costly anyway, so users have to decide on the best compromise.
 */
#macro _var_dest_var_size_const_src_perform_byte_align_and_copy_extra(out_dest_xbuf, \
                out_last_element, dest_start_byte, \
                in_src_xbuf, CONST_SRC_START_BYTE, src_byte_index, num_lw, extra_bytes)
.begin

    .reg vdvszp_temp save_active_lm last_byte_loc vdvszp_mask vdvszp_temp2 vdvszp_temp3

    .set vdvszp_temp save_active_lm last_byte_loc vdvszp_mask vdvszp_temp2 vdvszp_temp3

    #if (!(is_ct_const(CONST_SRC_START_BYTE) || isimport(CONST_SRC_START_BYTE)))
        #error 2 "_var_dest_var_size_const_src_perform_byte_align_and_copy_extra: \
                    CONST_SRC_START_BYTE must be constant."
    #endif


    #if ( (_SRC_XBUF_TYPE == _XBUF_LMEM) && \
        ( (_DEST_XBUF_TYPE == _XBUF_SRAM_XFER) || \
        (_DEST_XBUF_TYPE == _XBUF_DRAM_XFER) )  )

        #if (_DEST_XBUF_TYPE == _XBUF_SRAM_XFER)
            #define_eval    _INDEX  *$index
        #elif ( (!defined(__NFP_INDIRECT_REF_FORMAT_NFPXXXX)) && (_DEST_XBUF_TYPE == _XBUF_DRAM_XFER) )
            #define_eval    _INDEX  *$$index
        #endif

        #define_eval _NEW_SRC_1ST_ELEMENT   (CONST_SRC_START_BYTE / 4)

        #if (_SRC_XBUF_SIZE > 8)
            #if (_NEW_SRC_1ST_ELEMENT > 8)
                #define_eval _TEMP_SRC_SIZE (_SRC_XBUF_SIZE - _NEW_SRC_1ST_ELEMENT)
            #else
                #define_eval _TEMP_SRC_SIZE (_SRC_XBUF_SIZE - 12)
            #endif
        #else
            #define_eval _TEMP_SRC_SIZE 0
        #endif

        #if ((_SRC_XBUF_SIZE - _NEW_SRC_1ST_ELEMENT) > 5)
            #define_eval    _TEMP_DEST_SIZE 4
            #define_eval    _TEMP_SRC_SIZE  5
        #else
            #define_eval    _TEMP_DEST_SIZE (_SRC_XBUF_SIZE - _NEW_SRC_1ST_ELEMENT)
            #define_eval    _TEMP_SRC_SIZE  (_SRC_XBUF_SIZE - _NEW_SRC_1ST_ELEMENT)
        #endif

        xbuf_alloc (vdvszp_temp_dest, _TEMP_DEST_SIZE, read_write)
        #define_eval    i   0
        #while (i < _TEMP_DEST_SIZE)
            .set vdvszp_temp_dest[/**/i]
            #define_eval i (i + 1)
        #endloop

        xbuf_alloc(vdvszp_temp_src, _TEMP_SRC_SIZE, read_write)
        #define_eval    i   0
        #while (i < _TEMP_SRC_SIZE)
            .set vdvszp_temp_src[/**/i]
            #define_eval i (i + 1)
        #endloop

        alu[--, num_lw, - , 0]
        beq[vdvszp_copy_extra_bytes_no_check#], defer[3]
        //use 3 defered cycles to prepare for extra bytes
        local_csr_rd[ACTIVE_LM_ADDR_/**/_SRC_LMEM_INDEX]
        immed[save_active_lm, 0]
        #define_eval _VAL ((CONST_SRC_START_BYTE >> 2) << 2)
        //alu[last_byte_loc, save_active_lm, +, CONST_SRC_START_BYTE]
        alu[last_byte_loc, save_active_lm, +, _VAL]
        #undef _VAL

        alu_shf[vdvszp_mask, --, B, num_lw, <<2]

        #define_eval    _SRC1   _NEW_SRC_1ST_ELEMENT
        ;read first group of data (maximum 4 long-words)
        _vdvszp_read_max_4_longwords_and_put(vdvszp_temp_dest, vdvszp_temp_src, \
                                in_src_xbuf, _SRC1, num_lw)

        alu[vdvszp_temp, num_lw, -, 4]
        ble[vdvszp_copy_extra_bytes#], defer[1]
        alu[--, extra_bytes, -, 1]

        #define_eval _SRC2  (_NEW_SRC_1ST_ELEMENT+4)
        _vdvszp_read_max_4_longwords_and_put(vdvszp_temp_dest, vdvszp_temp_src, \
                                in_src_xbuf, _SRC2, vdvszp_temp)

        alu[vdvszp_temp, vdvszp_temp, -, 4]
        ble[vdvszp_copy_extra_bytes#], defer[1]
        alu[--, extra_bytes, -, 1]

        #define_eval _SRC3  (_NEW_SRC_1ST_ELEMENT+8)
        ;read 3rd group of data (maximum 4 long-words)
        _vdvszp_read_max_4_longwords_and_put(vdvszp_temp_dest, vdvszp_temp_src, \
                                in_src_xbuf, _SRC3, vdvszp_temp)

        alu[vdvszp_temp, vdvszp_temp, -, 4]
        ble[vdvszp_copy_extra_bytes#], defer[1]
        alu[--, extra_bytes, -, 1]

        #define_eval _SRC4  (_NEW_SRC_1ST_ELEMENT+12)
        ;read 4th group of data (maximum 4 long-words)
        _vdvszp_read_max_4_longwords_and_put(vdvszp_temp_dest, vdvszp_temp_src, \
                                in_src_xbuf, _SRC4, vdvszp_temp)

vdvszp_copy_extra_bytes#:

        ;condition set in alu[--, extra_bytes, -, 1]
        #if (is_ct_const(out_last_element) || isimport(out_last_element))
            blt[end_xbuf_copy#]
        #else
            blt[vdvszp_no_extra_fill_last_ele#], defer[3]
        #endif
        alu[last_byte_loc, last_byte_loc, +, vdvszp_mask]

vdvszp_copy_extra_bytes_no_check#:
        local_csr_wr[ACTIVE_LM_ADDR_/**/_SRC_LMEM_INDEX, last_byte_loc]

        ;reuse last_byte_loc for the last bit of the extra bytes
        alu_shf[last_byte_loc, --, B, extra_bytes, <<3]
        alu[last_byte_loc, 32, - , last_byte_loc]
        alu[vdvszp_mask, --, ~B, 0]

        #if (!(is_ct_const(out_last_element) || isimport(out_last_element)))

            ;can't increment local mem pointer. Very inefficient!!!!
            //byte_align_be[--, *l$index/**/_SRC_LMEM_INDEX++]
            //byte_align_be[out_last_element, 0]

            byte_align_be[--, *l$index/**/_SRC_LMEM_INDEX]
            byte_align_be[out_last_element, 0]

            alu[vdvszp_temp2, 4, -, src_byte_index]
            alu[--, vdvszp_temp2, -, extra_bytes]
            beq[vdvszp_end_get_extra_bytes_1#]
            bgt[vdvszp_mask_extra_bytes_1#]

            alu[--, --, B, *l$index/**/_SRC_LMEM_INDEX++]
            alu[vdvszp_temp3, --, B, *l$index/**/_SRC_LMEM_INDEX]
            alu_shf[vdvszp_temp2, --, B, vdvszp_temp2, <<3]
            alu_shf[vdvszp_temp2, vdvszp_temp2, OR, 0]
            ;OR needed for >>indirect
            alu_shf[vdvszp_temp3, --, B, vdvszp_temp3, >>indirect]
            alu[out_last_element, out_last_element, OR, vdvszp_temp3]

vdvszp_mask_extra_bytes_1#:
            //OR instruction needed for indirect shift
            alu[--, last_byte_loc, OR, 0]
            alu_shf[vdvszp_mask, --, B, vdvszp_mask, <<indirect]
            alu_shf[out_last_element, out_last_element, AND, vdvszp_mask]

vdvszp_end_get_extra_bytes_1#:

            alu[/**/_INDEX, --, B, out_last_element]
        #else

            byte_align_be[--, *l$index/**/_SRC_LMEM_INDEX]
            byte_align_be[vdvszp_temp, 0]

            alu[vdvszp_temp2, 4, -, src_byte_index]
            alu[--, vdvszp_temp2, -, extra_bytes]
            beq[vdvszp_end_get_extra_bytes_2#]
            bgt[vdvszp_mask_extra_bytes_2#]

            alu[--, --, B, *l$index/**/_SRC_LMEM_INDEX++]
            alu[vdvszp_temp3, --, B, *l$index/**/_SRC_LMEM_INDEX]
            alu_shf[vdvszp_temp2, --, B, vdvszp_temp2, <<3]
            //OR needed for >>indirect
            alu_shf[vdvszp_temp2, vdvszp_temp2, OR, 0]
            alu_shf[vdvszp_temp3, --, B, vdvszp_temp3, >>indirect]
            alu[vdvszp_temp, vdvszp_temp, OR, vdvszp_temp3]

vdvszp_mask_extra_bytes_2#:
            //OR instruction needed for indirect shift
            alu[--, last_byte_loc, OR, 0]
            alu_shf[vdvszp_mask, --, B, vdvszp_mask, <<indirect]

vdvszp_end_get_extra_bytes_2#:
            alu_shf[/**/_INDEX, vdvszp_temp, AND, vdvszp_mask]

        #endif

        br[vdvszp_end#]

vdvszp_no_extra_fill_last_ele#:

        //this implementation is chosen because in best case
        //only 6 instructions (7 cycles) are needed
        #if (!(is_ct_const(out_last_element) || isimport(out_last_element)))

            ;the last element is in vdvszp_temp_dest
            ;first, back up 1 longword
            alu[num_lw, num_lw, -, 1]
            alu[vdvszp_temp, num_lw, AND, /**/(_TEMP_DEST_SIZE - 1 )]

            alu[--, vdvszp_temp, -, 0]
            beq[vdvszp_end#], defer[2]

            #if (_TEMP_DEST_SIZE > 0)
                alu[out_last_element, --, B, vdvszp_temp_dest[0]]
            #endif
            alu[--, vdvszp_temp, -, 2]

            beq[vdvszp_end#], defer[2]

            #if (_TEMP_DEST_SIZE > 2)
                alu[out_last_element, --, B, vdvszp_temp_dest[2]]
            #endif
            alu[--, vdvszp_temp, -, 1]

            beq[vdvszp_end#], defer[1]
            #if (_TEMP_DEST_SIZE > 1)
                alu[out_last_element, --, B, vdvszp_temp_dest[1]]

            #endif

            #if (_TEMP_DEST_SIZE > 3)
                alu[out_last_element, --, B, vdvszp_temp_dest[3]]
            #endif

    #endif  //is_ct_const(out_last_element)


vdvszp_end#:
        xbuf_free(vdvszp_temp_dest)
        xbuf_free(vdvszp_temp_src)

        local_csr_wr[ACTIVE_LM_ADDR_/**/_SRC_LMEM_INDEX, save_active_lm]
        br[end_xbuf_copy#], defer[2]
        nop
        nop

        #undef _NEW_SRC_1ST_ELEMENT

        #ifdef _TEMP_DEST_SIZE
            #undef _TEMP_DEST_SIZE
        #endif

        #undef _SRC1
        #undef _SRC2
        #undef _SRC3
        #undef _SRC4

    #else

        #error "This case is not supported."

    #endif

.end
#endm
/// @endcond


/// @cond INTERNAL_MACROS
/** An internal macro used by xbuf main macros.
 */
#macro _vdvszp_read_max_4_longwords_and_put(vdvszp_temp_dest, vdvszp_temp_src, \
                                        in_src_xbuf, SRC_START_ELEMENT, num_lw_left)
.begin

    .reg    count

    #define_eval    _SRC_LAST_ELEMENT   (SRC_START_ELEMENT + 4)

    #if (SRC_START_ELEMENT > 7)
    ;move source to temp gpr since byte_align_be doesn't allow local mem index > 7
        #define_eval    i   SRC_START_ELEMENT
        #define_eval    j 0
        #while ( (j < _TEMP_SRC_SIZE) && (i < _SRC_XBUF_SIZE) )
            alu[vdvszp_temp_src[/**/j], --, B, in_src_xbuf[/**/i]]
            #define_eval i (i + 1)
            #define_eval j (j + 1)
        #endloop

        #undef j
        #undef i
    #elif (_SRC_LAST_ELEMENT > 7)
    ;move source to temp gpr since byte_align_be doesn't allow local mem index > 7
        #define_eval    _SIZE   (_SRC_LAST_ELEMENT - 7)
        #define_eval i _SRC_LAST_ELEMENT
        #define_eval j (_SIZE - 1)
        #while (j >= 0)
            alu[vdvszp_temp_src[/**/j], --, B, in_src_xbuf[/**/i]]
            #define_eval i (i - 1)
            #define_eval j (j - 1)
        #endloop

        #undef j
        #undef i

    #endif

    //read first 4 long words
    #if (SRC_START_ELEMENT < 8)
        byte_align_be[--, in_src_xbuf[/**/SRC_START_ELEMENT]]

        #define_eval    _CONTINUE   (SRC_START_ELEMENT + 1)

        #define_eval    i   0
        #while (i < _TEMP_DEST_SIZE)
            #if (_CONTINUE < 8)
                byte_align_be[vdvszp_temp_dest[/**/i], in_src_xbuf[_CONTINUE]]
            #else
                #define_eval _VDP_TEMP_INDEX    (_CONTINUE - 8)
                #if (_VDP_TEMP_INDEX < _TEMP_SRC_SIZE)
                    byte_align_be[vdvszp_temp_dest[/**/i], vdvszp_temp_src[_VDP_TEMP_INDEX]]
                #elif (_VDP_TEMP_INDEX == _TEMP_SRC_SIZE)
                    byte_align_be[vdvszp_temp_dest[/**/i], 0]
                #endif
            #endif

            #define_eval    _CONTINUE   (_CONTINUE + 1)
            #define_eval    i   (i+1)

        #endloop

    #else
        byte_align_be[--, vdvszp_temp_src[0]]

        #define_eval i  1
        #define_eval j 0
        #while ((i < _TEMP_SRC_SIZE) && ( j < _TEMP_DEST_SIZE))
            byte_align_be[vdvszp_temp_dest[/**/j], vdvszp_temp_src[/**/i]]

            #define_eval i  (i + 1)
            #define_eval j  (j + 1)
        #endloop

    #endif

    //now, move the needed number of bytes to destination
    alu[--, num_lw_left, -, 4]
    blt[less_than_4_lw#], defer[2]

    alu[/**/_INDEX++, --, B, vdvszp_temp_dest[0]]
    alu[--, 1, -, num_lw_left]

    #define_eval    i   1
    #while (i < 4)
        alu[/**/_INDEX++, --, B, vdvszp_temp_dest[/**/i]]

        #define_eval i (i + 1)
    #endloop
    br[end_vdvszp_copy_4_lw#], defer[1]
    alu[--, extra_bytes, -, 1]

less_than_4_lw#:
    ;condition set by alu[--, 1, -, num_lw_left]
    bge[vdvszp_copy_extra_bytes#], defer[1]
    alu[--, extra_bytes, -, 1]

    alu[/**/_INDEX++, --, B, vdvszp_temp_dest[1]]

    alu[--, 2, -, num_lw_left]
    bge[vdvszp_copy_extra_bytes#], defer[1]
    alu[--, extra_bytes, -, 1]

    alu[/**/_INDEX++, --, B, vdvszp_temp_dest[2]]
    br[vdvszp_copy_extra_bytes#], defer[1]
    alu[--, extra_bytes, -, 1]

end_vdvszp_copy_4_lw#:

    #undef _SRC_LAST_ELEMENT

.end
#endm
/// @endcond


/// @cond INTERNAL_MACROS
/**
//  destination offset: constant
//  source offset:      run-time (variable)
//  size:               constant
 */
#macro _copy_var_src_var_size_const_dest(out_dest_xbuf, out_last_element, \
                            DEST_START_BYTE, in_src_xbuf, src_start_byte, \
                            in_prepend, number_of_bytes)
.begin

    .reg new_src_start_byte vsvsz_temp vsvsz_mask num_lw save_active_lm
    .reg unaligned_bytes extra_bytes_num extra_bytes_off num_aligned_bytes

    .set new_src_start_byte vsvsz_temp vsvsz_mask num_lw save_active_lm
    .set unaligned_bytes extra_bytes_num extra_bytes_off num_aligned_bytes

    #if (!(is_ct_const(DEST_START_BYTE) || isimport(DEST_START_BYTE)))
        #error 2   "_copy_var_src_const_src_and_size: DEST_START_BYTE must be constant"
    #endif

    _GET_XBUF_TYPE(in_src_xbuf)
    #define_eval _SRC_XBUF_TYPE     _XBUF_TYPE
    #undef _XBUF_TYPE

    _DEFINE_THIS_XBUF(in_src_xbuf)
    #define_eval _SRC_XBUF_SIZE _this_xbuf_size

    _GET_XBUF_TYPE(out_dest_xbuf)
    #define_eval _DEST_XBUF_TYPE    _XBUF_TYPE
    #undef _XBUF_TYPE

    _DEFINE_THIS_XBUF(out_dest_xbuf)
    #define_eval _DEST_XBUF_SIZE    _this_xbuf_size
    #if (_DEST_XBUF_TYPE == _XBUF_LMEM)
        #define_eval _DEST_LMEM_INDEX   _this_xbuf_index
    #endif
    _UNDEFINE_THIS_XBUF()

    #ifdef _DEST_ALIGN
        #warning "_DEST_ALIGN is being redefined"
    #endif
    #define_eval _DEST_ALIGN    (DEST_START_BYTE % 4)

    #ifdef _DEST_FIRST_BIT_LOC
        #warning "_DEST_FIRST_BIT_LOC is being redefined"
    #endif
    #define_eval _DEST_FIRST_BIT_LOC    (_DEST_ALIGN * 8)

    #ifdef _DEST_1ST_ELEMENT
        #warning "_DEST_1ST_ELEMENT is being redefined"
    #endif
    #define_eval _DEST_1ST_ELEMENT  (DEST_START_BYTE / 4)

    #ifdef _DEST_2ND_ELEMENT
        #warning "_DEST_2ND_ELEMENT is being redefined"
    #endif
    #define_eval _DEST_2ND_ELEMENT  (_DEST_1ST_ELEMENT + 1)

    #ifdef _NUM_UNALIGNED_BYTES
        #warning "_NUM_UNALIGNED_BYTES is being redefined"
    #endif
    #if (_DEST_ALIGN > 0)
        #define_eval _NUM_UNALIGNED_BYTES  (4 - _DEST_ALIGN)
    #else
        #define_eval _NUM_UNALIGNED_BYTES  0
    #endif


    #if ( streq(out_dest_xbuf, in_src_xbuf) )
        ; For GPR and local memory xbuf types we do not perform copy when src & dest
        ; are exactly the same (same name with same start byte)
        #if ( (_SRC_XBUF_TYPE == _XBUF_GPR) || (_SRC_XBUF_TYPE == _XBUF_LMEM) )
            alu[--, src_start_byte, -, DEST_START_BYTE)
            beq [end_xbuf_copy#]
        #endif
    #endif

    #if ( ((_SRC_XBUF_TYPE == _XBUF_SRAM_XFER) || (_SRC_XBUF_TYPE == _XBUF_DRAM_XFER)) && \
        (_DEST_XBUF_TYPE == _XBUF_LMEM) )

        #if (_SRC_XBUF_TYPE == _XBUF_SRAM_XFER)
            #define_eval    _INDEX  *$index
        #elif ( (!defined(__NFP_INDIRECT_REF_FORMAT_NFPXXXX)) && (_SRC_XBUF_TYPE == _XBUF_DRAM_XFER) )
            #define_eval    _INDEX  *$$index
        #endif

        #if (_NUM_UNALIGNED_BYTES > 0)

            ;init T_INDEX_BYTE_INDEX to the first element in source buffer

            br=ctx[0, _copy_var_src_var_size_skip_find_abs_addr_1#], defer[2]
            add_shf_left(vsvsz_temp, src_start_byte, &in_src_xbuf[0], 2)

            //if not context 0, need to add offset to absolute reg to T_INDEX value
            _find_absolute_xfer_reg_addr(vsvsz_temp)

_copy_var_src_var_size_skip_find_abs_addr_1#:

            local_csr_wr[T_INDEX_BYTE_INDEX, vsvsz_temp]
            //need 3 cycles before the CSR T_INDEX_BYTE_INDEX can be used

            //local_csr_rd[ACTIVE_LM_ADDR/**/_DEST_LMEM_INDEX]
            //immed[save_active_lm, 0]
            alu[new_src_start_byte, src_start_byte, +, _NUM_UNALIGNED_BYTES]
            alu[vsvsz_temp, vsvsz_temp, +, _NUM_UNALIGNED_BYTES]
            alu[num_aligned_bytes, number_of_bytes, -, _NUM_UNALIGNED_BYTES]

            byte_align_be[--, _INDEX/**/++]
            byte_align_be[unaligned_bytes, _INDEX]

            //merge with in_prepend
            #if ( (!(is_ct_const(in_prepend) || isimport(in_prepend))) || \
                (((is_ct_const(in_prepend) || isimport(in_prepend))) && (in_prepend != 0)) )
                alu_shf[out_dest_xbuf[_DEST_1ST_ELEMENT], in_prepend, OR, \
                            unaligned_bytes, >>_DEST_FIRST_BIT_LOC]
            #else
                alu_shf[out_dest_xbuf[_DEST_1ST_ELEMENT], --, B, \
                    unaligned_bytes, >>_DEST_FIRST_BIT_LOC]
            #endif



            //do not treat less than 4 bytes case separately. Take hit for < 4 bytes but
            //save code space for more common cases

//////////////////////////////////////////////

            ;load new src start location to T_INDEX_BYTE_INDEX
            //alu[vsvsz_temp, vsvsz_temp, +, _NUM_UNALIGNED_BYTES]
            local_csr_wr[T_INDEX_BYTE_INDEX, vsvsz_temp]

            alu_shf[num_lw, --, B, num_aligned_bytes, >>2]
            add_shf_left(extra_bytes_off, _NUM_UNALIGNED_BYTES, num_lw, 2)
            alu[extra_bytes_num, number_of_bytes, -, extra_bytes_off]
            alu[extra_bytes_off, extra_bytes_off, +, DEST_START_BYTE]

            //alu_shf[extra_bytes_num, extra_bytes_off, -, num_lw, <<2]
            //alu[extra_bytes_off, extra_bytes_off, -, extra_bytes_num]
        #else
            ;init T_INDEX_BYTE_INDEX to the first element in source buffer

            br=ctx[0, _copy_var_src_var_size_skip_find_abs_addr_2#], defer[2]
            add_shf_left(vsvsz_temp, src_start_byte, &in_src_xbuf[0], 2)

            //if not context 0, need to add offset to absolute reg to T_INDEX value
            _find_absolute_xfer_reg_addr(vsvsz_temp)

_copy_var_src_var_size_skip_find_abs_addr_2#:

            local_csr_wr[T_INDEX_BYTE_INDEX, vsvsz_temp]
            //need 3 cycles before the CSR T_INDEX_BYTE_INDEX can be used

            alu_shf[num_lw, --, B, number_of_bytes, >>2]
            alu_shf[extra_bytes_off, --, B, num_lw, <<2]
            alu[extra_bytes_num, number_of_bytes, -, extra_bytes_off]
            alu[extra_bytes_off, extra_bytes_off, +, DEST_START_BYTE]
        #endif

        #if (_NUM_UNALIGNED_BYTES == 0)
            _var_src_var_size_const_dest_perform_byte_align_and_copy_extra( \
                out_dest_xbuf, out_last_element, _DEST_1ST_ELEMENT, \
                in_src_xbuf, src_start_byte, \
                extra_bytes_off, num_lw, extra_bytes_num)
        #else
            //destination not aligned, the 1st element has been filled already
            _var_src_var_size_const_dest_perform_byte_align_and_copy_extra( \
                out_dest_xbuf, out_last_element, _DEST_2ND_ELEMENT, \
                in_src_xbuf, new_src_start_byte, \
                extra_bytes_off, num_lw, extra_bytes_num)
        #endif

    #elif ( (_SRC_XBUF_TYPE == _XBUF_LMEM)  &&
            ((_DEST_XBUF_TYPE == _XBUF_SRAM_XFER) || (_DEST_XBUF_TYPE == _XBUF_DRAM_XFER)) )

        #error "This case is not supported."

    #elif ( (_SRC_XBUF_TYPE == _XBUF_LMEM)  && (_DEST_XBUF_TYPE == _XBUF_LMEM) )

        #error "This case is not supported."

    #else

        #error "Unsupport buffer type"

    #endif

end_vsvsz_copy#:
    local_csr_wr[ACTIVE_LM_ADDR_/**/_DEST_LMEM_INDEX, save_active_lm]
    br[end_xbuf_copy#], defer[2]
    nop
    nop

    #undef _NUM_UNALIGNED_BYTES

    #undef _SRC_XBUF_TYPE
    #undef _SRC_XBUF_SIZE
    #undef _DEST_XBUF_TYPE
    #undef _DEST_LMEM_INDEX
    #undef _DEST_XBUF_SIZE
    #undef _DEST_ALIGN
    #undef _DEST_FIRST_BIT_LOC
    #undef _DEST_1ST_ELEMENT
    #undef _DEST_2ND_ELEMENT

.end
#endm
/// @endcond


/// @cond INTERNAL_MACROS
///////////////////////////////////////////////////////////////////////////
//  Can't do a .while (count < num_lw) loop because local mem pointer
//  can't be incremented in byte_align_be instruction.
//  The strategy used is copy 2 longwords at a time into a temp buffer then
//  move the needed longwords to destination
///////////////////////////////////////////////////////////////////////////
#macro _var_src_var_size_const_dest_perform_byte_align_and_copy_extra( \
                        out_dest_xbuf, out_last_element, DEST_START_ELEMENT, \
                        in_src_xbuf, src_start_byte, \
                        extra_bytes_off, num_lw, extra_bytes_num)

.begin

    .reg    vsvszp_temp vsvszp_temp2
    .set    vsvszp_temp vsvszp_temp2

    #if (!(is_ct_const(DEST_START_ELEMENT) || isimport(DEST_START_ELEMENT)))
        #error 2 "_var_src_const_dest_const_size_perform_byte_align_and_copy_extra: \
                    DEST_START_ELEMENT must be constants."
    #endif


    #if ( (_DEST_XBUF_TYPE == _XBUF_LMEM) && \
        ((_SRC_XBUF_TYPE == _XBUF_SRAM_XFER) || (_SRC_XBUF_TYPE == _XBUF_DRAM_XFER)) )

        #if (_SRC_XBUF_TYPE == _XBUF_SRAM_XFER)
            #define_eval    _INDEX  *$index
        #elif ( (!defined(__NFP_INDIRECT_REF_FORMAT_NFPXXXX)) && (_SRC_XBUF_TYPE == _XBUF_DRAM_XFER) )
            #define_eval    _INDEX  *$$index
        #endif

        #if ((_DEST_XBUF_SIZE - DEST_START_ELEMENT) > 4)
            #define_eval _TEMP_DEST_SIZE 4
        #else
            #define_eval _TEMP_DEST_SIZE (_DEST_XBUF_SIZE - DEST_START_ELEMENT)
        #endif

        xbuf_alloc(vsvszp_temp_dest, _TEMP_DEST_SIZE, read_write)


        alu[vsvszp_temp, num_lw, -, 1]
        blt[vsvszp_copy_extra_bytes_no_check#], defer[3]

        local_csr_rd[ACTIVE_LM_ADDR_/**/_DEST_LMEM_INDEX]
        immed[save_active_lm, 0]

        alu[vsvsz_mask, --, ~B, 0]

        //read first longword
        byte_align_be[--, _INDEX/**/++]
        #if (DEST_START_ELEMENT > 7)
            byte_align_be[vsvszp_temp2, _INDEX]
            alu[out_dest_xbuf[/**/DEST_START_ELEMENT], --, B, vsvszp_temp2]
        #else
            byte_align_be[out_dest_xbuf[/**/DEST_START_ELEMENT], _INDEX]
        #endif

        alu[vsvszp_temp, vsvszp_temp, -, 0]
        beq[vsvszp_copy_extra_bytes#], defer[1]
        alu[--, extra_bytes_num, -, 0]

        #define_eval _DEST_INDEX1   (DEST_START_ELEMENT + 1)
        //read 1st group of 4 lw here
        _vsvszp_read_max_4_lw_and_put(out_dest_xbuf, vsvszp_temp_dest, \
                                    _DEST_INDEX1, vsvszp_temp)
        #undef _DEST_INDEX1

        alu[vsvszp_temp, vsvszp_temp, -, 4]
        ble[vsvszp_copy_extra_bytes#], defer[1]
        //use the defer cycles to prepare for extra bytes
        alu[--, extra_bytes_num, -, 0]

        ;read 2nd group of data (maximum 4 long-words)
        #define_eval _DEST_INDEX2   (DEST_START_ELEMENT + 5)
        //read 1st group of 4 lw here
        _vsvszp_read_max_4_lw_and_put(out_dest_xbuf, vsvszp_temp_dest, \
                                _DEST_INDEX2, vsvszp_temp)
        #undef _DEST_INDEX2


        alu[vsvszp_temp, vsvszp_temp, -, 4]
        ble[vsvszp_copy_extra_bytes#], defer[1]
        alu[--, extra_bytes_num, -, 0]

        ;read 3rd group of data (maximum 4 long-words)
        #define_eval _DEST_INDEX3   (DEST_START_ELEMENT + 9)
        //read 1st group of 4 lw here
        _vsvszp_read_max_4_lw_and_put(out_dest_xbuf, vsvszp_temp_dest, \
                                _DEST_INDEX3, vsvszp_temp)
        #undef _DEST_INDEX3

        alu[vsvszp_temp, vsvszp_temp, -, 4]
        ble[vsvszp_copy_extra_bytes#], defer[1]
        alu[--, extra_bytes_num, -, 0]

        ;read 4th group of data (maximum 4 long-words)
        #define_eval _DEST_INDEX4   (DEST_START_ELEMENT + 13)
        //read 1st group of 4 lw here
        _vsvszp_read_max_4_lw_and_put(out_dest_xbuf, vsvszp_temp_dest, \
                                _DEST_INDEX4, vsvszp_temp)
        #undef _DEST_INDEX4

vsvszp_copy_extra_bytes#:

        ;condition set in alu[--, extra_bytes_num, -, 1]
        #if (is_ct_const(out_last_element) || isimport(out_last_element))
            ble[end_xbuf_copy#]
        #else
            ble[vsvszp_no_extra_fill_last_ele#], defer[3]
        #endif

vsvszp_copy_extra_bytes_no_check#:

        alu[vsvszp_temp, extra_bytes_off, AND~, 3]

        alu[vsvszp_temp, save_active_lm, +, vsvszp_temp]
        local_csr_wr[ACTIVE_LM_ADDR_/**/_DEST_LMEM_INDEX, vsvszp_temp]
        //wait 3 cycles for CSR write to settle

        sub_shf_left(vsvszp_temp, 32, extra_bytes_num, 3)

        //OR instruction needed for <<indirect
        alu[vsvszp_temp, vsvszp_temp, OR, 0]
        alu[vsvsz_mask, --, B, vsvsz_mask, <<indirect]

        byte_align_be[--, _INDEX++]
        byte_align_be[vsvszp_temp, _INDEX]

        br[end_vsvsz_copy#], defer[2]

        #if (!(is_ct_const(out_last_element) || isimport(out_last_element)))
            alu[out_last_element, vsvszp_temp, AND, vsvsz_mask]
            alu[*l$index/**/_DEST_LMEM_INDEX, --, B, out_last_element]
        #else
            alu[*l$index/**/_DEST_LMEM_INDEX, vsvszp_temp, AND, vsvsz_mask]
        #endif

vsvszp_no_extra_fill_last_ele#:

        #if (!(is_ct_const(out_last_element) || isimport(out_last_element)))

            //wait 3 cycles for the local csr write to settle
            nop
            nop
            br[end_vsvsz_copy#], defer[2]

            ;back up 1 longword
            alu[--, --, B, *l$index/**/_DEST_LMEM_INDEX--]
            alu[out_last_element, --, B, *l$index/**/_DEST_LMEM_INDEX]
        #endif

    #else
        #error "This case is not supported."
    #endif

    xbuf_free(vsvszp_temp_dest)
    #undef _TEMP_DEST_SIZE

.end
#endm
/// @endcond


/// @cond INTERNAL_MACROS
/** An internal macro used by xbuf main macros.
 */
#macro _vsvszp_read_max_4_lw_and_put(out_dest_xbuf, vsvszp_temp_dest, DEST_INDEX, \
                                    num_lw_left)

.begin

    #if (_DEST_XBUF_SIZE  > DEST_INDEX)

        byte_align_be[--, _INDEX/**/++]

        #define_eval    i   0
        #while (i < 4)
            //#if ((i < _TEMP_DEST_SIZE)    && (i < (_DEST_XBUF_SIZE - 1)) )
            #if (i < _TEMP_DEST_SIZE)
                //each time this macro is called, at least 1 longword
                //needs to be read into destination
                #if ( (i == 0) && (DEST_INDEX < 8) )
                    byte_align_be[out_dest_xbuf[/**/DEST_INDEX], _INDEX/**/++]
                #else
                    #if (i < 3)
                        byte_align_be[vsvszp_temp_dest[/**/i], _INDEX/**/++]
                    #else
                        byte_align_be[vsvszp_temp_dest[/**/i], _INDEX]
                    #endif
                #endif
            #endif
            #define_eval    i   (i+1)
        #endloop

        //now, move the needed number of bytes to destination
        #if (DEST_INDEX >= 8)
            #define_eval _DF 2
        #else
            #define_eval _DF 1
        #endif

        alu[--, num_lw_left, -, 4]
        blt[vsvszp_less_than_4_lw#], defer[_DF]

        #if (DEST_INDEX >= 8)
            alu[out_dest_xbuf[/**/DEST_INDEX], --, B, vsvszp_temp_dest[0]]
        #endif
        alu[--, 1, -, num_lw_left]

        #define_eval    i   1
        #while (i < 4)
            #define_eval    j (DEST_INDEX + i)
            #if ((i < _TEMP_DEST_SIZE) && (j < _DEST_XBUF_SIZE))
                alu[out_dest_xbuf[/**/j], --, B, vsvszp_temp_dest[/**/i]]
            #endif
            #define_eval i (i + 1)
        #endloop
        br[end_vsvszp_copy_4_lw#]

vsvszp_less_than_4_lw#:

    //at this point, 1 lw has been moved to destination

        bge[end_vsvszp_copy_4_lw#], defer[3]
        //since 4 lw is read and only 1 is needed, back up _INDEX in the defered cycles
        alu[--, --, B, /**/_INDEX--]
        alu[--, --, B, /**/_INDEX--]
        alu[--, --, B, /**/_INDEX--]

        #if ((DEST_INDEX + 1) < _DEST_XBUF_SIZE)
            #define_eval _D1 (DEST_INDEX + 1)
            alu[out_dest_xbuf[/**/_D1], --, B, vsvszp_temp_dest[1]]
            #undef _D1
        #endif

        #if ((DEST_INDEX + 2) < _DEST_XBUF_SIZE)

            alu[--, 2, -, num_lw_left]
            bge[vsvszp_copy_extra_bytes#], defer[2]
            alu[--, --, B, /**/_INDEX++]
            alu[--, extra_bytes_num, -, 0]

            #define_eval _D2 (DEST_INDEX + 2)
            alu[out_dest_xbuf[_D2], --, B, vsvszp_temp_dest[2]]
            #undef _D2
            br[vsvszp_copy_extra_bytes#], defer[2]
            alu[--, --, B, /**/_INDEX++]
            alu[--, extra_bytes_num, -, 0]
        #else
            br[vsvszp_copy_extra_bytes#], defer[2]
            alu[--, --, B, /**/_INDEX++]
            alu[--, extra_bytes_num, -, 0]
        #endif

    #endif //if (_DEST_XBUF_SIZE  > DEST_INDEX)

end_vsvszp_copy_4_lw#:

.end
#endm
/// @endcond


/// @cond INTERNAL_MACROS
/** An internal macro used in xbuf_copy.
 *
 * Restriction: All the bytes to be copied must fit into the dest_xbuf (spaces in
 *             those buffers linked to dest_xbuf is not included).
 *                  @arg If dest_xbuf is transfer registers buffer: since the maximum
 *                  buffer size is 16/32 registers, the maximum value for number_of_bytes
 *                  is 64/128.
 *                  @arg If dest_xbuf is local memory buffer: since the maximum buffer
 *                  size is 8 registers, the maximum value for number_of_bytes is 32.
 */
#macro _copy_bufs_const_offsets_var_size(dest_xbuf, out_last_element, \
                DEST_START_BYTE, src_xbuf, SRC_START_BYTE, in_prepend, number_of_bytes)

.begin

    #if ( !((is_ct_const(DEST_START_BYTE) || isimport(DEST_START_BYTE)) && \
            (is_ct_const(SRC_START_BYTE) || isimport(SRC_START_BYTE))) )
        #error 2 "_copy_bufs_const_offsets_var_size: both destination and source offsets \
                    must be constants."
    #endif



    //by the time this internal macro is called, the data must actually resides
    //in src_xbuf and will resides in dest_xbuf (find_actual_buffer has been called)
    _DEFINE_THIS_XBUF(dest_xbuf)
    #define_eval    _DEST_BUF_SIZE_IN_BYTES (_this_xbuf_size * 4)
    _UNDEFINE_THIS_XBUF()

    _DEFINE_THIS_XBUF(src_xbuf)
    #define_eval _SRC_BUF_SIZE_IN_BYTES (_this_xbuf_size * 4)
    _UNDEFINE_THIS_XBUF()

    _GET_XBUF_TYPE(in_src_xbuf)
    #define_eval _SRC_XBUF_TYPE     _XBUF_TYPE
    #undef _XBUF_TYPE

    _GET_XBUF_TYPE(out_dest_xbuf)
    #define_eval _DEST_XBUF_TYPE    _XBUF_TYPE
    #undef _XBUF_TYPE

    #if (defined(__NFP_INDIRECT_REF_FORMAT_NFPXXXX) && (_DEST_XBUF_TYPE == _XBUF_SRAM_XFER))
        #define_eval        _MAX_NUM_BYTES  128
    #else
        #define_eval        _MAX_NUM_BYTES  64
    #endif

    #if ( streq(out_dest_xbuf, in_src_xbuf) )
        ; For GPR and local memory xbuf types we do not perform copy when src & dest
        ; are exactly the same (same name with same start byte)
        #if ( (_SRC_XBUF_TYPE == _XBUF_GPR) || (_SRC_XBUF_TYPE == _XBUF_LMEM) )
            #if (_SRC_START_BYTE == DEST_START_BYTE)
                #define _DO_NOTHING
            #endif
        #endif
    #endif

    #ifndef _DO_NOTHING

        #if (strstr(dest_xbuf, '*l$index') > 0)
            jump[number_of_bytes, const_offsets_copy_jmp_tbl#],
            targets[const_offsets_copy_entry_0#, const_offsets_copy_entry_1#, const_offsets_copy_entry_2#, \
                    const_offsets_copy_entry_3#, const_offsets_copy_entry_4#, const_offsets_copy_entry_5#, \
                    const_offsets_copy_entry_6#, const_offsets_copy_entry_7#, const_offsets_copy_entry_8#, \
                    const_offsets_copy_entry_9#, const_offsets_copy_entry_10#, const_offsets_copy_entry_11#, \
                    const_offsets_copy_entry_12#, const_offsets_copy_entry_13#, const_offsets_copy_entry_14#, \
                    const_offsets_copy_entry_15#, const_offsets_copy_entry_16#, const_offsets_copy_entry_17#, \
                    const_offsets_copy_entry_18#, const_offsets_copy_entry_19#, const_offsets_copy_entry_20#, \
                    const_offsets_copy_entry_21#, const_offsets_copy_entry_22#, const_offsets_copy_entry_23#, \
                    const_offsets_copy_entry_24#, const_offsets_copy_entry_25#, const_offsets_copy_entry_26#, \
                    const_offsets_copy_entry_27#, const_offsets_copy_entry_28#, const_offsets_copy_entry_29#, \
                    const_offsets_copy_entry_18#, const_offsets_copy_entry_19#, const_offsets_copy_entry_20#, \
                    const_offsets_copy_entry_30#, const_offsets_copy_entry_31#, const_offsets_copy_entry_32#],
            defer[3]
        #else
            #if (_MAX_NUM_BYTES == 64)
                jump[number_of_bytes, const_offsets_copy_jmp_tbl#],
                targets[const_offsets_copy_entry_0#, const_offsets_copy_entry_1#, const_offsets_copy_entry_2#, \
                        const_offsets_copy_entry_3#, const_offsets_copy_entry_4#, const_offsets_copy_entry_5#, \
                        const_offsets_copy_entry_6#, const_offsets_copy_entry_7#, const_offsets_copy_entry_8#, \
                        const_offsets_copy_entry_9#, const_offsets_copy_entry_10#, const_offsets_copy_entry_11#, \
                        const_offsets_copy_entry_12#, const_offsets_copy_entry_13#, const_offsets_copy_entry_14#, \
                        const_offsets_copy_entry_15#, const_offsets_copy_entry_16#, const_offsets_copy_entry_17#, \
                        const_offsets_copy_entry_18#, const_offsets_copy_entry_19#, const_offsets_copy_entry_20#, \
                        const_offsets_copy_entry_21#, const_offsets_copy_entry_22#, const_offsets_copy_entry_23#, \
                        const_offsets_copy_entry_24#, const_offsets_copy_entry_25#, const_offsets_copy_entry_26#, \
                        const_offsets_copy_entry_27#, const_offsets_copy_entry_28#, const_offsets_copy_entry_29#, \
                        const_offsets_copy_entry_18#, const_offsets_copy_entry_19#, const_offsets_copy_entry_20#, \
                        const_offsets_copy_entry_30#, const_offsets_copy_entry_31#, const_offsets_copy_entry_32#, \
                        const_offsets_copy_entry_33#, const_offsets_copy_entry_34#, const_offsets_copy_entry_35#, \
                        const_offsets_copy_entry_36#, const_offsets_copy_entry_37#, const_offsets_copy_entry_38#, \
                        const_offsets_copy_entry_39#, const_offsets_copy_entry_40#, const_offsets_copy_entry_41#, \
                        const_offsets_copy_entry_42#, const_offsets_copy_entry_43#, const_offsets_copy_entry_44#, \
                        const_offsets_copy_entry_45#, const_offsets_copy_entry_46#, const_offsets_copy_entry_47#, \
                        const_offsets_copy_entry_48#, const_offsets_copy_entry_49#, const_offsets_copy_entry_50#, \
                        const_offsets_copy_entry_51#, const_offsets_copy_entry_52#, const_offsets_copy_entry_53#, \
                        const_offsets_copy_entry_54#, const_offsets_copy_entry_55#, const_offsets_copy_entry_56#, \
                        const_offsets_copy_entry_57#, const_offsets_copy_entry_58#, const_offsets_copy_entry_59#, \
                        const_offsets_copy_entry_60#, const_offsets_copy_entry_61#, const_offsets_copy_entry_62#, \
                        const_offsets_copy_entry_63#, const_offsets_copy_entry_64#],
                    defer[3]
                #elif (_MAX_NUM_BYTES == 128)
                    jump[number_of_bytes, const_offsets_copy_jmp_tbl#],
                    targets[const_offsets_copy_entry_0#, const_offsets_copy_entry_1#, const_offsets_copy_entry_2#, \
                            const_offsets_copy_entry_3#, const_offsets_copy_entry_4#, const_offsets_copy_entry_5#, \
                            const_offsets_copy_entry_6#, const_offsets_copy_entry_7#, const_offsets_copy_entry_8#, \
                            const_offsets_copy_entry_9#, const_offsets_copy_entry_10#, const_offsets_copy_entry_11#, \
                            const_offsets_copy_entry_12#, const_offsets_copy_entry_13#, const_offsets_copy_entry_14#, \
                            const_offsets_copy_entry_15#, const_offsets_copy_entry_16#, const_offsets_copy_entry_17#, \
                            const_offsets_copy_entry_18#, const_offsets_copy_entry_19#, const_offsets_copy_entry_20#, \
                            const_offsets_copy_entry_21#, const_offsets_copy_entry_22#, const_offsets_copy_entry_23#, \
                            const_offsets_copy_entry_24#, const_offsets_copy_entry_25#, const_offsets_copy_entry_26#, \
                            const_offsets_copy_entry_27#, const_offsets_copy_entry_28#, const_offsets_copy_entry_29#, \
                            const_offsets_copy_entry_18#, const_offsets_copy_entry_19#, const_offsets_copy_entry_20#, \
                            const_offsets_copy_entry_30#, const_offsets_copy_entry_31#, const_offsets_copy_entry_32#, \
                            const_offsets_copy_entry_33#, const_offsets_copy_entry_34#, const_offsets_copy_entry_35#, \
                            const_offsets_copy_entry_36#, const_offsets_copy_entry_37#, const_offsets_copy_entry_38#, \
                            const_offsets_copy_entry_39#, const_offsets_copy_entry_40#, const_offsets_copy_entry_41#, \
                            const_offsets_copy_entry_42#, const_offsets_copy_entry_43#, const_offsets_copy_entry_44#, \
                            const_offsets_copy_entry_45#, const_offsets_copy_entry_46#, const_offsets_copy_entry_47#, \
                            const_offsets_copy_entry_48#, const_offsets_copy_entry_49#, const_offsets_copy_entry_50#, \
                            const_offsets_copy_entry_51#, const_offsets_copy_entry_52#, const_offsets_copy_entry_53#, \
                            const_offsets_copy_entry_54#, const_offsets_copy_entry_55#, const_offsets_copy_entry_56#, \
                            const_offsets_copy_entry_57#, const_offsets_copy_entry_58#, const_offsets_copy_entry_59#, \
                            const_offsets_copy_entry_60#, const_offsets_copy_entry_61#, const_offsets_copy_entry_62#, \
                            const_offsets_copy_entry_63#, const_offsets_copy_entry_64#, const_offsets_copy_entry_65#, \
                            const_offsets_copy_entry_66#, const_offsets_copy_entry_67#, const_offsets_copy_entry_68#, \
                            const_offsets_copy_entry_69#, const_offsets_copy_entry_70#, const_offsets_copy_entry_71#, \
                            const_offsets_copy_entry_72#, const_offsets_copy_entry_73#, const_offsets_copy_entry_74#, \
                            const_offsets_copy_entry_75#, const_offsets_copy_entry_76#, const_offsets_copy_entry_77#, \
                            const_offsets_copy_entry_78#, const_offsets_copy_entry_79#, const_offsets_copy_entry_80#, \
                            const_offsets_copy_entry_81#, const_offsets_copy_entry_82#, const_offsets_copy_entry_83#, \
                            const_offsets_copy_entry_84#, const_offsets_copy_entry_85#, const_offsets_copy_entry_86#, \
                            const_offsets_copy_entry_87#, const_offsets_copy_entry_88#, const_offsets_copy_entry_89#, \
                            const_offsets_copy_entry_90#, const_offsets_copy_entry_91#, const_offsets_copy_entry_92#, \
                            const_offsets_copy_entry_93#, const_offsets_copy_entry_94#, const_offsets_copy_entry_95#, \
                            const_offsets_copy_entry_96#, const_offsets_copy_entry_97#, const_offsets_copy_entry_98#, \
                            const_offsets_copy_entry_99#, const_offsets_copy_entry_100#, const_offsets_copy_entry_101#, \
                            const_offsets_copy_entry_102#, const_offsets_copy_entry_103#, const_offsets_copy_entry_104#, \
                            const_offsets_copy_entry_105#, const_offsets_copy_entry_106#, const_offsets_copy_entry_107#, \
                            const_offsets_copy_entry_108#, const_offsets_copy_entry_109#, const_offsets_copy_entry_110#, \
                            const_offsets_copy_entry_111#, const_offsets_copy_entry_112#, const_offsets_copy_entry_113#, \
                            const_offsets_copy_entry_114#, const_offsets_copy_entry_115#, const_offsets_copy_entry_116#, \
                            const_offsets_copy_entry_117#, const_offsets_copy_entry_118#, const_offsets_copy_entry_119#, \
                            const_offsets_copy_entry_120#, const_offsets_copy_entry_121#, const_offsets_copy_entry_122#, \
                            const_offsets_copy_entry_123#, const_offsets_copy_entry_124#, const_offsets_copy_entry_125#, \
                            const_offsets_copy_entry_126#, const_offsets_copy_entry_127#, const_offsets_copy_entry_128#],
                    defer[3]
                #else
                    #error "This case is unsupported."
                #endif
            #endif
            //must wait 3 cycles after jump
            nop
            nop
            nop

//  Start of jump table
const_offsets_copy_jmp_tbl#:

        #define_eval i 0
        #while (i <= _MAX_NUM_BYTES)
            #define_eval    _D_LOC      DEST_START_BYTE + i
            #define_eval    _S_LOC      SRC_START_BYTE + i

const_offsets_copy_entry_/**/i#:

            #if ((_DEST_BUF_SIZE_IN_BYTES >= _D_LOC) && (_SRC_BUF_SIZE_IN_BYTES >= _S_LOC))
                br[const_offsets_copy_/**/i#]
            #endif
            #define_eval i (i+1)
        #endloop

        #define_eval i  0
        #while (i <= _MAX_NUM_BYTES)
            #define_eval    _D_LOC  DEST_START_BYTE + i
            #define_eval    _S_LOC      SRC_START_BYTE + i

const_offsets_copy_/**/i#:
            #if ((_DEST_BUF_SIZE_IN_BYTES >= _D_LOC) && (_SRC_BUF_SIZE_IN_BYTES >= _S_LOC))
                #if (i > 0)
                    _xbuf_copy_const_offsets_and_size(dest_xbuf, out_last_element, \
                        DEST_START_BYTE, src_xbuf, SRC_START_BYTE, in_prepend, /**/i, 0)
                #endif
                br[end_const_offsets_and_var_size_copy#]
            #endif
            #define_eval i (i+1)
        #endloop

        #undef _D_LOC
        #undef _S_LOC
        #undef i

    #else //do nothing
        #undef _DO_NOTHING
    #endif

end_const_offsets_and_var_size_copy#:

.end
#endm
/// @endcond


/// @cond INTERNAL_MACROS
/** An internal macro to check if the input buffer is big enough to contain number_of_bytes starting from start_byte.
 *
 * To be used by xbuf_copy, xbuf_align_and_copy and related copy macros.
 *
 * Output:
 *        @arg None - if buffer(s) has enough space
 *        @arg Error - if not enough space
 *
 * @param str_xbuf_name      Name of xbuf (one buffer or a buffer in a chain
 *                           of linked buffers)
 * @param start_byte         Constant or GPR specifies start byte offset in xbuf
 *                           where the check begins
 * @param number_of_bytes    Constant or GPR specifies total number of bytes to
 *                           check for available space
 * @param LINKED_BUFS        Constant to specify whether any buffers linked
 *                           to str_xbuf_name be considered as part of the whole
 *                           available space: @n
 *                           @arg 1:  include linked buffers (if any) @n
 *                           @arg 0:  do not include any linked buffers.
 */
#macro _xbuf_check_space(str_xbuf_name, start_byte, number_of_bytes, LINKED_BUFS)
.begin

    .reg cp_temp

    .set cp_temp

    #if (LINKED_BUFS == 1)
        _FIND_TOTAL_LINKED_XBUF_SIZE(str_xbuf_name)
    #else
        #ifdef _this_total_size_in_bytes
            #warning "_this_total_size_in_bytes is being redefined"
            #undef _this_total_size_in_bytes
        #endif
        #ifdef _this_total_size_in_long_words
            #warning "_this_total_size_in_long_words is being redefined"
            #undef _this_total_size_in_long_words
        #endif
        _DEFINE_THIS_XBUF(str_xbuf_name)
        #define_eval _this_total_size_in_long_words     _this_xbuf_size
        #define_eval _this_total_size_in_bytes          (_this_xbuf_size * 4)

    #endif

    //case 1: both inputs are constants
    #if ((is_ct_const(start_byte) || isimport(start_byte)) && \
        (is_ct_const(number_of_bytes) || isimport(number_of_bytes)))

        #define_eval _WORD0 (start_byte/ 4)
        #if (_WORD0 >= _this_total_size_in_long_words)
            #error 2   "Start byte is out of range of the input buffer."
        #endif

        //check if buffer size if big enough to contain number_of_bytes
        #define_eval _END_BYTE (start_byte+ (number_of_bytes - 1))

        #define_eval _WORD1 (_END_BYTE / 4)

        // start checking from the input buffer as first of 4 possible xbufs
        #if (_WORD1 >= _this_total_size_in_long_words)
            #error 2   /**/number_of_bytes/**/ " is out of range of transfer buffer(s)."
        #endif

    #elif (!(is_ct_const(start_byte) || isimport(start_byte)))    //case 2: start_byte is not constant

        alu[--, start_byte, -, _this_total_size_in_bytes]
        blt[continue_case_2#]
        //#error 2   "Start byte is out of range of the input buffer."
        br[end_check_space#]

        continue_case_2#:
            alu[cp_temp, start_byte, +, number_of_bytes]
            alu[--, cp_temp, -, _this_total_size_in_bytes]

        //  #error 2   "Start byte is out of range of the input buffer."

    #else //case 3: number_of_bytes is not constant

        #if (start_byte >= _this_total_size_in_bytes)
            #error 2   "Start byte is out of range of the input buffer."
        #endif

        alu[cp_temp, number_of_bytes, + , start_byte]
        alu[--, cp_temp, -, _this_total_size_in_bytes]
        //blt[end_check_space#]
        //#error 2   "Start byte is out of range of the input buffer."

    #endif

end_check_space#:

    _CLEAR_NAME_SPACE_FOR_FIND_TOTAL_LINKED_XBUF_SIZE()

    #undef _WORD0
    #undef _WORD1

.end
#endm
/// @endcond


/// @cond INTERNAL_MACROS
/** Locally declares contiguous 'NUMBER_OF_REGS' dram xfer registers and sets an appropriate .xfer_order
 *
 * Registers may be referenced as an array.
 * Allows transfer regs to be divided into 4 buffers max.
 *
 * @param str_xbuf_d_name  String uniquely identifying transfer register buffer
 * @param NUMBER_OF_REGS   Number of transfer registers to allocate
 * @param RW_INDICATOR     String = read, write, or read_write, which
 *                         which will specify the transfer registers as
 *                         read only, write only, or read/write transfer registers
 *
 * @b Example usage:
 * @code
 * _xbuf_d_alloc($$dx, 8, read)                      // setup read only xfer regs $$dx0 - $$dx7
 * dram_read($$dx0, addr1, addr2, QWCOUNT4, pend)    // load the xfer regs
 * _xbuf_d_free($$dx)
 * @endcode
 */
#macro _xbuf_d_alloc(str_xbuf_d_name, NUMBER_OF_REGS, RW_INDICATOR)
    #if (defined(__NFP_INDIRECT_REF_FORMAT_NFPXXXX))
        #error "This macro should not be used in NFP indirect reference mode."
    #endif

    #ifndef _xbuf_d1_name
        #define_eval _xbuf_d1_name (str_xbuf_d_name)
        #define_eval _xbuf_d1_size (NUMBER_OF_REGS)
    #else
        #ifndef _xbuf_d2_name
            #define_eval _xbuf_d2_name (str_xbuf_d_name)
            #define_eval _xbuf_d2_size (NUMBER_OF_REGS)
        #else
            #ifndef _xbuf_d3_name
                #define_eval _xbuf_d3_name (str_xbuf_d_name)
                #define_eval _xbuf_d3_size (NUMBER_OF_REGS)
            #else
                #ifndef _xbuf_d4_name
                    #define_eval _xbuf_d4_name (str_xbuf_d_name)
                    #define_eval _xbuf_d4_size (NUMBER_OF_REGS)
                #else
                    #error 2 "_xbuf_d_alloc: No more dram xfer bufs available."
                #endif
            #endif
        #endif
    #endif

    #ifdef _REG_QUALIFIERS
        #undef _REG_QUALIFIERS
    #endif
    #if (streq(RW_INDICATOR, read_write))
        #define _REG_QUALIFIERS
    #elif (streq(RW_INDICATOR, read_write_visible))
        #define _REG_QUALIFIERS visible
    #elif (streq(RW_INDICATOR, read))
        #define _REG_QUALIFIERS read
    #elif (streq(RW_INDICATOR, read_visible))
        #define _REG_QUALIFIERS read visible
    #elif (streq(RW_INDICATOR, write))
        #define _REG_QUALIFIERS write
    #elif (streq(RW_INDICATOR, write_visible))
        #define _REG_QUALIFIERS write visible
    #else  // if (RW_INDICATOR != read, write, read_write
        #error 2 "_xbuf_d_alloc: For dram, must specify read, write, read_write, read_visible, write_visible or read_write visible 3rd arg."
    #endif

    #if (NUMBER_OF_REGS == 1)
        .reg _REG_QUALIFIERS str_xbuf_d_name/**/0
    #elif (NUMBER_OF_REGS == 2)
        .reg _REG_QUALIFIERS str_xbuf_d_name/**/0, str_xbuf_d_name/**/1
        .xfer_order str_xbuf_d_name/**/0, str_xbuf_d_name/**/1
    #elif (NUMBER_OF_REGS == 3)
        .reg _REG_QUALIFIERS str_xbuf_d_name/**/0, str_xbuf_d_name/**/1, str_xbuf_d_name/**/2
        .xfer_order str_xbuf_d_name/**/0, str_xbuf_d_name/**/1, str_xbuf_d_name/**/2
    #elif (NUMBER_OF_REGS == 4)
        .reg _REG_QUALIFIERS str_xbuf_d_name/**/0, str_xbuf_d_name/**/1, str_xbuf_d_name/**/2, str_xbuf_d_name/**/3
        .xfer_order str_xbuf_d_name/**/0, str_xbuf_d_name/**/1, str_xbuf_d_name/**/2, str_xbuf_d_name/**/3
    #elif (NUMBER_OF_REGS == 5)
        .reg _REG_QUALIFIERS str_xbuf_d_name/**/0, str_xbuf_d_name/**/1, str_xbuf_d_name/**/2, str_xbuf_d_name/**/3, str_xbuf_d_name/**/4
        .xfer_order str_xbuf_d_name/**/0, str_xbuf_d_name/**/1, str_xbuf_d_name/**/2, str_xbuf_d_name/**/3, str_xbuf_d_name/**/4
    #elif (NUMBER_OF_REGS == 6)
        .reg _REG_QUALIFIERS str_xbuf_d_name/**/0, str_xbuf_d_name/**/1, str_xbuf_d_name/**/2, str_xbuf_d_name/**/3, str_xbuf_d_name/**/4, str_xbuf_d_name/**/5
        .xfer_order str_xbuf_d_name/**/0, str_xbuf_d_name/**/1, str_xbuf_d_name/**/2, str_xbuf_d_name/**/3, str_xbuf_d_name/**/4, str_xbuf_d_name/**/5
    #elif (NUMBER_OF_REGS == 7)
        .reg _REG_QUALIFIERS str_xbuf_d_name/**/0, str_xbuf_d_name/**/1, str_xbuf_d_name/**/2, str_xbuf_d_name/**/3, str_xbuf_d_name/**/4, str_xbuf_d_name/**/5, str_xbuf_d_name/**/6
        .xfer_order str_xbuf_d_name/**/0, str_xbuf_d_name/**/1, str_xbuf_d_name/**/2, str_xbuf_d_name/**/3, str_xbuf_d_name/**/4, str_xbuf_d_name/**/5, str_xbuf_d_name/**/6
    #elif (NUMBER_OF_REGS == 8)
        .reg _REG_QUALIFIERS str_xbuf_d_name/**/0, str_xbuf_d_name/**/1, str_xbuf_d_name/**/2, str_xbuf_d_name/**/3, str_xbuf_d_name/**/4, str_xbuf_d_name/**/5, str_xbuf_d_name/**/6, str_xbuf_d_name/**/7
        .xfer_order str_xbuf_d_name/**/0, str_xbuf_d_name/**/1, str_xbuf_d_name/**/2, str_xbuf_d_name/**/3, str_xbuf_d_name/**/4, str_xbuf_d_name/**/5, str_xbuf_d_name/**/6, str_xbuf_d_name/**/7
    #elif (NUMBER_OF_REGS == 9)
        .reg _REG_QUALIFIERS str_xbuf_d_name/**/0, str_xbuf_d_name/**/1, str_xbuf_d_name/**/2, str_xbuf_d_name/**/3, str_xbuf_d_name/**/4, str_xbuf_d_name/**/5, str_xbuf_d_name/**/6, str_xbuf_d_name/**/7, str_xbuf_d_name/**/8
        .xfer_order str_xbuf_d_name/**/0, str_xbuf_d_name/**/1, str_xbuf_d_name/**/2, str_xbuf_d_name/**/3, str_xbuf_d_name/**/4, str_xbuf_d_name/**/5, str_xbuf_d_name/**/6, str_xbuf_d_name/**/7, str_xbuf_d_name/**/8
    #elif (NUMBER_OF_REGS == 10)
        .reg _REG_QUALIFIERS str_xbuf_d_name/**/0, str_xbuf_d_name/**/1, str_xbuf_d_name/**/2, str_xbuf_d_name/**/3, str_xbuf_d_name/**/4, str_xbuf_d_name/**/5, str_xbuf_d_name/**/6, str_xbuf_d_name/**/7, str_xbuf_d_name/**/8, str_xbuf_d_name/**/9
        .xfer_order str_xbuf_d_name/**/0, str_xbuf_d_name/**/1, str_xbuf_d_name/**/2, str_xbuf_d_name/**/3, str_xbuf_d_name/**/4, str_xbuf_d_name/**/5, str_xbuf_d_name/**/6, str_xbuf_d_name/**/7, str_xbuf_d_name/**/8, str_xbuf_d_name/**/9
    #elif (NUMBER_OF_REGS == 11)
        .reg _REG_QUALIFIERS str_xbuf_d_name/**/0, str_xbuf_d_name/**/1, str_xbuf_d_name/**/2, str_xbuf_d_name/**/3, str_xbuf_d_name/**/4, str_xbuf_d_name/**/5, str_xbuf_d_name/**/6, str_xbuf_d_name/**/7, str_xbuf_d_name/**/8, str_xbuf_d_name/**/9, str_xbuf_d_name/**/10
        .xfer_order str_xbuf_d_name/**/0, str_xbuf_d_name/**/1, str_xbuf_d_name/**/2, str_xbuf_d_name/**/3, str_xbuf_d_name/**/4, str_xbuf_d_name/**/5, str_xbuf_d_name/**/6, str_xbuf_d_name/**/7, str_xbuf_d_name/**/8, str_xbuf_d_name/**/9, str_xbuf_d_name/**/10
    #elif (NUMBER_OF_REGS == 12)
        .reg _REG_QUALIFIERS str_xbuf_d_name/**/0, str_xbuf_d_name/**/1, str_xbuf_d_name/**/2, str_xbuf_d_name/**/3, str_xbuf_d_name/**/4, str_xbuf_d_name/**/5, str_xbuf_d_name/**/6, str_xbuf_d_name/**/7, str_xbuf_d_name/**/8, str_xbuf_d_name/**/9, str_xbuf_d_name/**/10, str_xbuf_d_name/**/11
        .xfer_order str_xbuf_d_name/**/0, str_xbuf_d_name/**/1, str_xbuf_d_name/**/2, str_xbuf_d_name/**/3, str_xbuf_d_name/**/4, str_xbuf_d_name/**/5, str_xbuf_d_name/**/6, str_xbuf_d_name/**/7, str_xbuf_d_name/**/8, str_xbuf_d_name/**/9, str_xbuf_d_name/**/10, str_xbuf_d_name/**/11
    #elif (NUMBER_OF_REGS == 13)
        .reg _REG_QUALIFIERS str_xbuf_d_name/**/0, str_xbuf_d_name/**/1, str_xbuf_d_name/**/2, str_xbuf_d_name/**/3, str_xbuf_d_name/**/4, str_xbuf_d_name/**/5, str_xbuf_d_name/**/6, str_xbuf_d_name/**/7, str_xbuf_d_name/**/8, str_xbuf_d_name/**/9, str_xbuf_d_name/**/10, str_xbuf_d_name/**/11, str_xbuf_d_name/**/12
        .xfer_order str_xbuf_d_name/**/0, str_xbuf_d_name/**/1, str_xbuf_d_name/**/2, str_xbuf_d_name/**/3, str_xbuf_d_name/**/4, str_xbuf_d_name/**/5, str_xbuf_d_name/**/6, str_xbuf_d_name/**/7, str_xbuf_d_name/**/8, str_xbuf_d_name/**/9, str_xbuf_d_name/**/10, str_xbuf_d_name/**/11, str_xbuf_d_name/**/12
    #elif (NUMBER_OF_REGS == 14)
        .reg _REG_QUALIFIERS str_xbuf_d_name/**/0, str_xbuf_d_name/**/1, str_xbuf_d_name/**/2, str_xbuf_d_name/**/3, str_xbuf_d_name/**/4, str_xbuf_d_name/**/5, str_xbuf_d_name/**/6, str_xbuf_d_name/**/7, str_xbuf_d_name/**/8, str_xbuf_d_name/**/9, str_xbuf_d_name/**/10, str_xbuf_d_name/**/11, str_xbuf_d_name/**/12, str_xbuf_d_name/**/13
        .xfer_order str_xbuf_d_name/**/0, str_xbuf_d_name/**/1, str_xbuf_d_name/**/2, str_xbuf_d_name/**/3, str_xbuf_d_name/**/4, str_xbuf_d_name/**/5, str_xbuf_d_name/**/6, str_xbuf_d_name/**/7, str_xbuf_d_name/**/8, str_xbuf_d_name/**/9, str_xbuf_d_name/**/10, str_xbuf_d_name/**/11, str_xbuf_d_name/**/12, str_xbuf_d_name/**/13
    #elif (NUMBER_OF_REGS == 15)
        .reg _REG_QUALIFIERS str_xbuf_d_name/**/0, str_xbuf_d_name/**/1, str_xbuf_d_name/**/2, str_xbuf_d_name/**/3, str_xbuf_d_name/**/4, str_xbuf_d_name/**/5, str_xbuf_d_name/**/6, str_xbuf_d_name/**/7, str_xbuf_d_name/**/8, str_xbuf_d_name/**/9, str_xbuf_d_name/**/10, str_xbuf_d_name/**/11, str_xbuf_d_name/**/12, str_xbuf_d_name/**/13, str_xbuf_d_name/**/14
        .xfer_order str_xbuf_d_name/**/0, str_xbuf_d_name/**/1, str_xbuf_d_name/**/2, str_xbuf_d_name/**/3, str_xbuf_d_name/**/4, str_xbuf_d_name/**/5, str_xbuf_d_name/**/6, str_xbuf_d_name/**/7, str_xbuf_d_name/**/8, str_xbuf_d_name/**/9, str_xbuf_d_name/**/10, str_xbuf_d_name/**/11, str_xbuf_d_name/**/12, str_xbuf_d_name/**/13, str_xbuf_d_name/**/14
    #elif (NUMBER_OF_REGS == 16)
        .reg _REG_QUALIFIERS str_xbuf_d_name/**/0, str_xbuf_d_name/**/1, str_xbuf_d_name/**/2, str_xbuf_d_name/**/3, str_xbuf_d_name/**/4, str_xbuf_d_name/**/5, str_xbuf_d_name/**/6, str_xbuf_d_name/**/7, str_xbuf_d_name/**/8, str_xbuf_d_name/**/9, str_xbuf_d_name/**/10, str_xbuf_d_name/**/11, str_xbuf_d_name/**/12, str_xbuf_d_name/**/13, str_xbuf_d_name/**/14, str_xbuf_d_name/**/15
        .xfer_order str_xbuf_d_name/**/0, str_xbuf_d_name/**/1, str_xbuf_d_name/**/2, str_xbuf_d_name/**/3, str_xbuf_d_name/**/4, str_xbuf_d_name/**/5, str_xbuf_d_name/**/6, str_xbuf_d_name/**/7, str_xbuf_d_name/**/8, str_xbuf_d_name/**/9, str_xbuf_d_name/**/10, str_xbuf_d_name/**/11, str_xbuf_d_name/**/12, str_xbuf_d_name/**/13, str_xbuf_d_name/**/14, str_xbuf_d_name/**/15
    #else
        #error 2 "_xbuf_d_alloc: Maximum number of dram xfer registers exceeded."
    #endif  //if (NUM_OF_REGS == 1).
#endm
/// @endcond


/// @cond INTERNAL_MACROS
/** Alias for .endlocal, used to deallocate xfer registers.
 *
 * @param str_xbuf_d_name   Name of transfer register buffer
 *
 * @b Example usage:
 * @code
 * _xbuf_d_alloc($$dx, 8)                            // setup xfer regs $$dx0 - $$dx7
 * dram_read($$dx0, addr1, addr2, QWCOUNT4, pend)    // load the xfer regs
 * _xbuf_d_free($$dx)
 * @endcode
 */
#macro _xbuf_d_free(str_xbuf_d_name)
    #if (defined(__NFP_INDIRECT_REF_FORMAT_NFPXXXX))
        #error "This macro should not be used in NFP indirect reference mode."
    #endif

    #define_eval _this_xbuf_d_name (str_xbuf_d_name)
    #if (streq(_this_xbuf_d_name, _xbuf_d1_name))
        #undef _xbuf_d1_name
        #undef _xbuf_d1_size
        #ifdef _xbuf_d1_next
            #undef _xbuf_d1_next
        #endif

    #elif (streq(_this_xbuf_d_name, _xbuf_d2_name))
        #undef _xbuf_d2_name
        #undef _xbuf_d2_size
        #ifdef _xbuf_d2_next
            #undef _xbuf_d2_next
        #endif
    #elif (streq(_this_xbuf_d_name, _xbuf_d3_name))
        #undef _xbuf_d3_name
        #undef _xbuf_d3_size
        #ifdef _xbuf_d3_next
            #undef _xbuf_d3_next
        #endif
    #elif (streq(_this_xbuf_d_name, _xbuf_d4_name))
        #undef _xbuf_d4_name
        #undef _xbuf_d4_size
        #ifdef _xbuf_d4_next
            #undef _xbuf_d4_next
        #endif
    #else
        #error 2 "_xbuf_d_free: Invalid str_xbuf_d_name."
    #endif
    //.endlocal
#endm
/// @endcond


/// @cond INTERNAL_MACROS
/** Link one dram transfer register buffer to another.
 *
 * The purpose is to allow processing streams of bytes by continually loading buffers and providing macros
 * that can traverse the buffers.
 *
 * @param str_xbuf_d_name      Transfer buffer allocated by $$xbuf_d_alloc
 * @param str_nextxbuf_name    Another transfer buffer allocated by $$xbuf_d_alloc.
 *                             The next buffer continues the stream of bytes.
 *
 * @b Example usage:
 * @code
 * _xbuf_d_alloc($$pkt, 4)                              // setup xbuf using 4 xfer regs
 * // load the xfer buf $$pkt, use it
 * _xbuf_d_alloc($$morepkt, 4)                          // setup xbuf using 4 xfer regs
 * _xbuf_d_link($$pkt, $$morepkt)                       // logical link pkt to morepkt
 * // load the second xfer buf morepkt
 * ip_verify(exception, $$pkt, ip_header_byte_position) // verify ip header that spans the buffers
 * _xbuf_d_free($$pkt)
 * _xbuf_d_free($$morepkt)
 * @endcode
 */
#macro _xbuf_d_link(str_xbuf_d_name, str_nextxbuf_name)
    #if (defined(__NFP_INDIRECT_REF_FORMAT_NFPXXXX))
        #error "This macro should not be used in NFP indirect reference mode."
    #endif

    #define_eval _this_xbuf_d_name (str_xbuf_d_name)

    #if (streq(_this_xbuf_d_name, _xbuf_d1_name))
        #define_eval _xbuf_d1_next  (str_nextxbuf_name)
    #elif (streq(_this_xbuf_d_name, _xbuf_d2_name))
        #define_eval _xbuf_d2_next  (str_nextxbuf_name)
    #elif (streq(_this_xbuf_d_name, _xbuf_d3_name))
        #define_eval _xbuf_d3_next  (str_nextxbuf_name)
    #elif (streq(_this_xbuf_d_name, _xbuf_d4_name))
        #define_eval _xbuf_d4_next   (str_nextxbuf_name)
    #else
        #error 2 "_xbuf_d_link: Invalid str_xbuf_d_name."
    #endif

        // check if str_nextxbuf_name is valid
        #define_eval _next_xbuf_d_name  str_nextxbuf_name

        #if (streq(_next_xbuf_d_name, _xbuf_d1_name) || streq(_next_xbuf_d_name, _xbuf_d2_name) || \
            streq(_next_xbuf_d_name, _xbuf_d3_name) || streq(_next_xbuf_d_name, _xbuf_d4_name))
        #else
            #error 2 "_xbuf_d_link: Invalid str_nextxbuf_name."
        #endif
#endm
/// @endcond


/// @cond INTERNAL_MACROS
/** Set global internal params _cur_xbuf_d_name, _cur_xbuf_d_size and _cur_xbuf_d_next.
 *
 * It is used for folowing a chain of xbufs.
 *
 * @param str_xbuf_d_name  Name of dram transfer buffer
 *
 * Side-effect: Modify _cur_xbuf_d_name, _cur_xbuf_d_size and _cur_xbuf_d_next.
 */
#macro _xbuf_d_param_set(str_xbuf_d_name)
    #if (defined(__NFP_INDIRECT_REF_FORMAT_NFPXXXX))
        #error "This macro should not be used in NFP indirect reference mode."
    #endif

    #ifdef _cur_xbuf_d_name
        #undef _cur_xbuf_d_name
    #endif
    #ifdef _cur_xbuf_d_size
        #undef _cur_xbuf_d_size
    #endif
    #ifdef _cur_xbuf_d_next
        #undef _cur_xbuf_d_next
    #endif
    #define_eval _cur_xbuf_d_name (str_xbuf_d_name)

    #if streq(_cur_xbuf_d_name, _xbuf_d1_name)
        #define _cur_xbuf_d_size    _xbuf_d1_size
        #ifdef _xbuf_d1_next
            #define _cur_xbuf_d_next _xbuf_d1_next
        #endif
    #elif streq(_cur_xbuf_d_name, _xbuf_d2_name)
        #define _cur_xbuf_d_size    _xbuf_d2_size
        #ifdef _xbuf_d2_next
            #define _cur_xbuf_d_next _xbuf_d2_next
        #endif
    #elif streq(_cur_xbuf_d_name, _xbuf_d3_name)
        #define _cur_xbuf_d_size    _xbuf_d3_size
        #ifdef _xbuf_d3_next
            #define _cur_xbuf_d_next _xbuf_d3_next
        #endif
    #elif streq(_cur_xbuf_d_name, _xbuf_d4_name)
        #define _cur_xbuf_d_size    _xbuf_d4_size
        #ifdef _xbuf_d4_next
            #define _cur_xbuf_d_next _xbuf_d4_next
        #endif
    #else
        #error 4 "_xbuf_d_param_set: no matching str_xbuf_d_name."
    #endif
#endm
/// @endcond


/// @cond INTERNAL_MACROS
/** An internal macro used by xbuf_alloc for GRP.
 *
 * @instruction_cnt 0
 */
#macro _xbuf_gpr_alloc(str_xbuf_name, NUMBER_OF_REGS, RW_INDICATOR)
    #ifndef _xbuf1_gpr_name
    #define_eval _xbuf1_gpr_name (str_xbuf_name)
    #define_eval _xbuf1_gpr_size (NUMBER_OF_REGS)
    #else
        #ifndef _xbuf2_gpr_name
        #define_eval _xbuf2_gpr_name (str_xbuf_name)
        #define_eval _xbuf2_gpr_size (NUMBER_OF_REGS)
        #else
            #ifndef _xbuf3_gpr_name
            #define_eval _xbuf3_gpr_name (str_xbuf_name)
            #define_eval _xbuf3_gpr_size (NUMBER_OF_REGS)
            #else
                #ifndef _xbuf4_gpr_name
                #define_eval _xbuf4_gpr_name (str_xbuf_name)
                #define_eval _xbuf4_gpr_size (NUMBER_OF_REGS)
                #else
                    #error 2 "_xbuf_gpr_alloc: No more gpr xfer bufs available."
                #endif
            #endif
        #endif
    #endif

    #ifdef _REG_QUALIFIERS
        #undef _REG_QUALIFIERS
    #endif
    #if (streq(RW_INDICATOR, read_write))
        #define _REG_QUALIFIERS
    #else  // if (RW_INDICATOR != read, write, read_write
        #error 2 "_xbuf_gpr_alloc: For GPRs, must specify read_write as 3rd arg."
    #endif

    #if (NUMBER_OF_REGS == 1)
        .reg _REG_QUALIFIERS str_xbuf_name/**/0
    #elif (NUMBER_OF_REGS == 2)
        .reg _REG_QUALIFIERS str_xbuf_name/**/0, str_xbuf_name/**/1
    #elif (NUMBER_OF_REGS == 3)
        .reg _REG_QUALIFIERS str_xbuf_name/**/0, str_xbuf_name/**/1, str_xbuf_name/**/2
    #elif (NUMBER_OF_REGS == 4)
        .reg _REG_QUALIFIERS str_xbuf_name/**/0, str_xbuf_name/**/1, str_xbuf_name/**/2, str_xbuf_name/**/3
    #elif (NUMBER_OF_REGS == 5)
        .reg _REG_QUALIFIERS str_xbuf_name/**/0, str_xbuf_name/**/1, str_xbuf_name/**/2, str_xbuf_name/**/3, str_xbuf_name/**/4
    #elif (NUMBER_OF_REGS == 6)
        .reg _REG_QUALIFIERS str_xbuf_name/**/0, str_xbuf_name/**/1, str_xbuf_name/**/2, str_xbuf_name/**/3, str_xbuf_name/**/4, str_xbuf_name/**/5
    #elif (NUMBER_OF_REGS == 7)
        .reg _REG_QUALIFIERS str_xbuf_name/**/0, str_xbuf_name/**/1, str_xbuf_name/**/2, str_xbuf_name/**/3, str_xbuf_name/**/4, str_xbuf_name/**/5, str_xbuf_name/**/6
    #elif (NUMBER_OF_REGS == 8)
        .reg _REG_QUALIFIERS str_xbuf_name/**/0, str_xbuf_name/**/1, str_xbuf_name/**/2, str_xbuf_name/**/3, str_xbuf_name/**/4, str_xbuf_name/**/5, str_xbuf_name/**/6, str_xbuf_name/**/7
    #elif (NUMBER_OF_REGS == 9)
        .reg _REG_QUALIFIERS str_xbuf_name/**/0, str_xbuf_name/**/1, str_xbuf_name/**/2, str_xbuf_name/**/3, str_xbuf_name/**/4, str_xbuf_name/**/5, str_xbuf_name/**/6, str_xbuf_name/**/7, str_xbuf_name/**/8
    #elif (NUMBER_OF_REGS == 10)
        .reg _REG_QUALIFIERS str_xbuf_name/**/0, str_xbuf_name/**/1, str_xbuf_name/**/2, str_xbuf_name/**/3, str_xbuf_name/**/4, str_xbuf_name/**/5, str_xbuf_name/**/6, str_xbuf_name/**/7, str_xbuf_name/**/8, str_xbuf_name/**/9
    #elif (NUMBER_OF_REGS == 11)
        .reg _REG_QUALIFIERS str_xbuf_name/**/0, str_xbuf_name/**/1, str_xbuf_name/**/2, str_xbuf_name/**/3, str_xbuf_name/**/4, str_xbuf_name/**/5, str_xbuf_name/**/6, str_xbuf_name/**/7, str_xbuf_name/**/8, str_xbuf_name/**/9, str_xbuf_name/**/10
    #elif (NUMBER_OF_REGS == 12)
        .reg _REG_QUALIFIERS str_xbuf_name/**/0, str_xbuf_name/**/1, str_xbuf_name/**/2, str_xbuf_name/**/3, str_xbuf_name/**/4, str_xbuf_name/**/5, str_xbuf_name/**/6, str_xbuf_name/**/7, str_xbuf_name/**/8, str_xbuf_name/**/9, str_xbuf_name/**/10, str_xbuf_name/**/11
    #elif (NUMBER_OF_REGS == 13)
        .reg _REG_QUALIFIERS str_xbuf_name/**/0, str_xbuf_name/**/1, str_xbuf_name/**/2, str_xbuf_name/**/3, str_xbuf_name/**/4, str_xbuf_name/**/5, str_xbuf_name/**/6, str_xbuf_name/**/7, str_xbuf_name/**/8, str_xbuf_name/**/9, str_xbuf_name/**/10, str_xbuf_name/**/11, str_xbuf_name/**/12
    #elif (NUMBER_OF_REGS == 14)
        .reg _REG_QUALIFIERS str_xbuf_name/**/0, str_xbuf_name/**/1, str_xbuf_name/**/2, str_xbuf_name/**/3, str_xbuf_name/**/4, str_xbuf_name/**/5, str_xbuf_name/**/6, str_xbuf_name/**/7, str_xbuf_name/**/8, str_xbuf_name/**/9, str_xbuf_name/**/10, str_xbuf_name/**/11, str_xbuf_name/**/12, str_xbuf_name/**/13
    #elif (NUMBER_OF_REGS == 15)
        .reg _REG_QUALIFIERS str_xbuf_name/**/0, str_xbuf_name/**/1, str_xbuf_name/**/2, str_xbuf_name/**/3, str_xbuf_name/**/4, str_xbuf_name/**/5, str_xbuf_name/**/6, str_xbuf_name/**/7, str_xbuf_name/**/8, str_xbuf_name/**/9, str_xbuf_name/**/10, str_xbuf_name/**/11, str_xbuf_name/**/12, str_xbuf_name/**/13, str_xbuf_name/**/14
    #elif (NUMBER_OF_REGS == 16)
        .reg _REG_QUALIFIERS str_xbuf_name/**/0, str_xbuf_name/**/1, str_xbuf_name/**/2, str_xbuf_name/**/3, str_xbuf_name/**/4, str_xbuf_name/**/5, str_xbuf_name/**/6, str_xbuf_name/**/7, str_xbuf_name/**/8, str_xbuf_name/**/9, str_xbuf_name/**/10, str_xbuf_name/**/11, str_xbuf_name/**/12, str_xbuf_name/**/13, str_xbuf_name/**/14, str_xbuf_name/**/15
    #else
        #error 2 "_xbuf_gpr_alloc: Maximum number of gpr xfer registers exceeded."
    #endif  //if (NUM_OF_REGS == 1).
#endm
/// @endcond


/// @cond INTERNAL_MACROS
/** An internal macro used by xbuf_free.
 *
 * @instruction_cnt 0
 */
#macro _xbuf_gpr_free(str_xbuf_name)
    #define_eval _this_xbuf_name (str_xbuf_name)
    #if (streq(_this_xbuf_name, _xbuf1_gpr_name))
        #undef _xbuf1_gpr_name
        #undef _xbuf1_gpr_size
    #ifdef _xbuf1_gpr_next
        #undef _xbuf1_gpr_next
    #endif

    #elif (streq(_this_xbuf_name, _xbuf2_gpr_name))
        #undef _xbuf2_gpr_name
        #undef _xbuf2_gpr_size
        #ifdef _xbuf2_gpr_next
            #undef _xbuf2_gpr_next
        #endif
    #elif (streq(_this_xbuf_name, _xbuf3_gpr_name))
        #undef _xbuf3_gpr_name
        #undef _xbuf3_gpr_size
        #ifdef _xbuf3_gpr_next
            #undef _xbuf3_gpr_next
        #endif
    #elif (streq(_this_xbuf_name, _xbuf4_gpr_name))
        #undef _xbuf4_gpr_name
        #undef _xbuf4_gpr_size
        #ifdef _xbuf4_gpr_next
            #undef _xbuf4_gpr_next
        #endif
    #else
        #error 2 "_xbuf_gpr_free: Invalid str_xbuf_name."
    #endif
#endm
/// @endcond


/// @cond INTERNAL_MACROS
/** An internal macro used by xbuf_link for GPR.
 *
 * @instruction_cnt 0
 */
#macro _xbuf_gpr_link(str_xbuf_name, str_nextxbuf_name)
    #define_eval _this_xbuf_name (str_xbuf_name)

    #if (streq(_this_xbuf_name, _xbuf1_gpr_name))
        #define_eval _xbuf1_gpr_next  (str_nextxbuf_name)
    #elif (streq(_this_xbuf_name, _xbuf2_gpr_name))
        #define_eval _xbuf2_gpr_next  (str_nextxbuf_name)
    #elif (streq(_this_xbuf_name, _xbuf3_gpr_name))
        #define_eval _xbuf3_gpr_next  (str_nextxbuf_name)
    #elif (streq(_this_xbuf_name, _xbuf4_gpr_name))
        #define_eval _xbuf4_gpr_next   (str_nextxbuf_name)
    #else
        #error 2 "_xbuf_gpr_link: Invalid str_xbuf_name."
    #endif

    // check if str_nextxbuf_name is valid
    #define_eval _next_xbuf_name    str_nextxbuf_name

    #if (streq(_next_xbuf_name, _xbuf1_gpr_name) || streq(_next_xbuf_name, _xbuf2_gpr_name) || \
        streq(_next_xbuf_name, _xbuf3_gpr_name) || streq(_next_xbuf_name, _xbuf4_gpr_name))
    #else
        #error 2 "_xbuf_gpr_link: Invalid str_nextxbuf_name."
    #endif
#endm
/// @endcond


/// @cond INTERNAL_MACROS
/** An internal macro used by xbuf_paran_set for GPR.
 *
 * @instruction_cnt 0
 */
#macro _xbuf_gpr_param_set(str_xbuf_name)
    #ifdef _cur_xbuf_gpr_name
        #undef _cur_xbuf_gpr_name
    #endif
    #ifdef _cur_xbuf_gpr_size
        #undef _cur_xbuf_gpr_size
    #endif
    #ifdef _cur_xbuf_gpr_next
        #undef _cur_xbuf_gpr_next
    #endif
    #define_eval _cur_xbuf_gpr_name (str_xbuf_name)

    #if streq(_cur_xbuf_gpr_name, _xbuf1_gpr_name)
        #define _cur_xbuf_gpr_size  _xbuf1_gpr_size
        #ifdef _xbuf1_gpr_next
            #define _cur_xbuf_gpr_next _xbuf1_gpr_next
        #endif
    #elif streq(_cur_xbuf_gpr_name, _xbuf2_gpr_name)
        #define _cur_xbuf_gpr_size  _xbuf2_gpr_size
        #ifdef _xbuf2_gpr_next
            #define _cur_xbuf_gpr_next _xbuf2_gpr_next
        #endif
    #elif streq(_cur_xbuf_gpr_name, _xbuf3_gpr_name)
        #define _cur_xbuf_gpr_size  _xbuf3_gpr_size
        #ifdef _xbuf3_gpr_next
            #define _cur_xbuf_gpr_next _xbuf3_gpr_next
        #endif
    #elif streq(_cur_xbuf_gpr_name, _xbuf4_gpr_name)
        #define _cur_xbuf_gpr_size  _xbuf4_gpr_size
        #ifdef _xbuf4_gpr_next
            #define _cur_xbuf_gpr_next _xbuf4_gpr_next
        #endif
    #else
        #error 4 "_xbuf_gpr_param_set: no matching str_xbuf_name."
    #endif
#endm
/// @endcond


/// @cond INTERNAL_MACROS
/** An internal macro used by xbuf_alloc for local memory.
 *
 * @instruction_cnt 0
////////////////////////////////////////////////////////////////////////////////////////
//*************************************************
//*********** Internal macros for localmem ********
//*************************************************
 * 16 buffers are supported for local memory. Only 2 buffers that are
 * associated with *l$index0 and *l$index1 are active at a time.
 */
#macro _xbuf_lmem_alloc(str_xbuf_name, NUMBER_OF_REGS, RW_INDICATOR)
    #ifndef _xbuf1_lmem_name
        #define_eval _xbuf1_lmem_name   (str_xbuf_name)
        #define_eval _xbuf1_lmem_size   (NUMBER_OF_REGS)
    #else
        #ifndef _xbuf2_lmem_name
            #define_eval _xbuf2_lmem_name   (str_xbuf_name)
            #define_eval _xbuf2_lmem_size   (NUMBER_OF_REGS)
        #else
            #ifndef _xbuf3_lmem_name
                #define_eval _xbuf3_lmem_name   (str_xbuf_name)
                #define_eval _xbuf3_lmem_size   (NUMBER_OF_REGS)
            #else
                #ifndef _xbuf4_lmem_name
                    #define_eval _xbuf4_lmem_name   (str_xbuf_name)
                    #define_eval _xbuf4_lmem_size   (NUMBER_OF_REGS)
                #else
                    #ifndef _xbuf5_lmem_name
                        #define_eval _xbuf5_lmem_name   (str_xbuf_name)
                        #define_eval _xbuf5_lmem_size   (NUMBER_OF_REGS)
                    #else
                        #ifndef _xbuf6_lmem_name
                            #define_eval _xbuf6_lmem_name   (str_xbuf_name)
                            #define_eval _xbuf6_lmem_size   (NUMBER_OF_REGS)
                        #else
                            #ifndef _xbuf7_lmem_name
                                #define_eval _xbuf7_lmem_name   (str_xbuf_name)
                                #define_eval _xbuf7_lmem_size   (NUMBER_OF_REGS)
                            #else
                                #ifndef _xbuf8_lmem_name
                                    #define_eval _xbuf8_lmem_name   (str_xbuf_name)
                                    #define_eval _xbuf8_lmem_size   (NUMBER_OF_REGS)
                                #else
                                    #ifndef _xbuf9_lmem_name
                                        #define_eval _xbuf9_lmem_name   (str_xbuf_name)
                                        #define_eval _xbuf9_lmem_size   (NUMBER_OF_REGS)
                                    #else
                                        #ifndef _xbuf10_lmem_name
                                            #define_eval _xbuf10_lmem_name  (str_xbuf_name)
                                            #define_eval _xbuf10_lmem_size  (NUMBER_OF_REGS)
                                        #else
                                            #ifndef _xbuf11_lmem_name
                                                #define_eval _xbuf11_lmem_name  (str_xbuf_name)
                                                #define_eval _xbuf11_lmem_size  (NUMBER_OF_REGS)
                                            #else
                                                #ifndef _xbuf12_lmem_name
                                                    #define_eval _xbuf12_lmem_name  (str_xbuf_name)
                                                    #define_eval _xbuf12_lmem_size  (NUMBER_OF_REGS)
                                                #else
                                                    #ifndef _xbuf13_lmem_name
                                                        #define_eval _xbuf13_lmem_name  (str_xbuf_name)
                                                        #define_eval _xbuf13_lmem_size  (NUMBER_OF_REGS)
                                                    #else
                                                        #ifndef _xbuf14_lmem_name
                                                            #define_eval _xbuf14_lmem_name  (str_xbuf_name)
                                                            #define_eval _xbuf14_lmem_size  (NUMBER_OF_REGS)
                                                        #else
                                                            #ifndef _xbuf15_lmem_name
                                                                #define_eval _xbuf15_lmem_name  (str_xbuf_name)
                                                                #define_eval _xbuf15_lmem_size  (NUMBER_OF_REGS)
                                                            #else
                                                                #ifndef _xbuf16_lmem_name
                                                                    #define_eval _xbuf16_lmem_name  (str_xbuf_name)
                                                                    #define_eval _xbuf16_lmem_size  (NUMBER_OF_REGS)
                                                                #else
                                                                    #error 2 "_xbuf_lmem_alloc: No more localmem transfer bufs available."
                                                                #endif
                                                            #endif
                                                        #endif
                                                    #endif
                                                #endif
                                            #endif
                                        #endif
                                    #endif
                                #endif
                            #endif
                        #endif
                    #endif
                #endif
            #endif
        #endif
    #endif

    #ifdef _REG_QUALIFIERS
        #undef _REG_QUALIFIERS
    #endif
    #if (streq(RW_INDICATOR, read_write))
        #define _REG_QUALIFIERS
    #else  // if (RW_INDICATOR !=read_write)
        #error 2 "_xbuf_lmem_alloc: For local mems, must specify read_write as 3rd arg."
    #endif

    #if (NUMBER_OF_REGS < 0)
        #error 2 "_xbuf_lmem_alloc: invalid number of local memory registers."
    #elif ( NUMBER_OF_REGS > 16)
        #error 2 "_xbuf_lmem_alloc: max number of local memory registers exceeded."
    #endif
#endm
/// @endcond


/// @cond INTERNAL_MACROS
/** An internal macro used by xbuf_free for local memory.
 *
 * @instruction_cnt 0
 */
#macro _xbuf_lmem_free(str_xbuf_name)
    #define_eval _this_xbuf_name (str_xbuf_name)
    #if (streq(_this_xbuf_name, _xbuf1_lmem_name))
        #undef _xbuf1_lmem_name
        #undef _xbuf1_lmem_size
        #ifdef _xbuf1_lmem_next
            #undef _xbuf1_lmem_next
        #endif
        #ifdef _xbuf1_lmem_pools_base
            #undef _xbuf1_lmem_pools_base
        #endif
        #ifdef _xbuf1_lmem_pool_size
            #undef _xbuf1_lmem_pool_size
        #endif
        #ifdef _xbuf1_lmem_offset
            #undef _xbuf1_lmem_offset
        #endif
        #ifdef _xbuf1_lmem_index
            #undef _xbuf1_lmem_index
        #endif
        #ifdef _XBUF_LMEM_HANDLE0
            #if (_XBUF_LMEM_HANDLE0 == 1)
                #undef _XBUF_LMEM_HANDLE0
            #endif
        #endif
        #ifdef _XBUF_LMEM_HANDLE1
            #if (_XBUF_LMEM_HANDLE1 == 1)
                #undef _XBUF_LMEM_HANDLE1
            #endif
        #endif
    #elif (streq(_this_xbuf_name, _xbuf2_lmem_name))
        #undef _xbuf2_lmem_name
        #undef _xbuf2_lmem_size
        #ifdef _xbuf2_lmem_next
            #undef _xbuf2_lmem_next
        #endif
        #ifdef _xbuf2_lmem_pools_base
            #undef _xbuf2_lmem_pools_base
        #endif
        #ifdef _xbuf2_lmem_pool_size
            #undef _xbuf2_lmem_pool_size
        #endif
        #ifdef _xbuf2_lmem_offset
            #undef _xbuf2_lmem_offset
        #endif
        #ifdef _xbuf2_lmem_index
            #undef _xbuf2_lmem_index
        #endif
        #ifdef _XBUF_LMEM_HANDLE0
            #if (_XBUF_LMEM_HANDLE0 == 2)
                #undef _XBUF_LMEM_HANDLE0
            #endif
        #endif
        #ifdef _XBUF_LMEM_HANDLE1
            #if (_XBUF_LMEM_HANDLE1 == 2)
                #undef _XBUF_LMEM_HANDLE1
            #endif
        #endif
// Support upto 16 localmem buffers
    #elif (streq(_this_xbuf_name, _xbuf3_lmem_name))
        #undef _xbuf3_lmem_name
        #undef _xbuf3_lmem_size
        #ifdef _xbuf3_lmem_next
            #undef _xbuf3_lmem_next
        #endif
        #ifdef _xbuf3_lmem_pools_base
            #undef _xbuf3_lmem_pools_base
        #endif
        #ifdef _xbuf3_lmem_pool_size
            #undef _xbuf3_lmem_pool_size
        #endif
        #ifdef _xbuf3_lmem_offset
            #undef _xbuf3_lmem_offset
        #endif
        #ifdef _xbuf3_lmem_index
            #undef _xbuf3_lmem_index
        #endif
        #ifdef _XBUF_LMEM_HANDLE0
            #if (_XBUF_LMEM_HANDLE0 == 3)
                #undef _XBUF_LMEM_HANDLE0
            #endif
        #endif
        #ifdef _XBUF_LMEM_HANDLE1
            #if (_XBUF_LMEM_HANDLE1 == 3)
                #undef _XBUF_LMEM_HANDLE1
            #endif
        #endif
    #elif (streq(_this_xbuf_name, _xbuf4_lmem_name))
        #undef _xbuf4_lmem_name
        #undef _xbuf4_lmem_size
        #ifdef _xbuf4_lmem_next
            #undef _xbuf4_lmem_next
        #endif
        #ifdef _xbuf4_lmem_pools_base
            #undef _xbuf4_lmem_pools_base
        #endif
        #ifdef _xbuf4_lmem_pool_size
            #undef _xbuf4_lmem_pool_size
        #endif
        #ifdef _xbuf4_lmem_offset
            #undef _xbuf4_lmem_offset
        #endif
        #ifdef _xbuf4_lmem_index
            #undef _xbuf4_lmem_index
        #endif
        #ifdef _XBUF_LMEM_HANDLE0
            #if (_XBUF_LMEM_HANDLE0 == 4)
                #undef _XBUF_LMEM_HANDLE0
            #endif
        #endif
        #ifdef _XBUF_LMEM_HANDLE1
            #if (_XBUF_LMEM_HANDLE1 == 4)
                #undef _XBUF_LMEM_HANDLE1
            #endif
        #endif
    #elif (streq(_this_xbuf_name, _xbuf5_lmem_name))
        #undef _xbuf5_lmem_name
        #undef _xbuf5_lmem_size
        #ifdef _xbuf5_lmem_next
            #undef _xbuf5_lmem_next
        #endif
        #ifdef _xbuf5_lmem_pools_base
            #undef _xbuf5_lmem_pools_base
        #endif
        #ifdef _xbuf5_lmem_pool_size
            #undef _xbuf5_lmem_pool_size
        #endif
        #ifdef _xbuf5_lmem_offset
            #undef _xbuf5_lmem_offset
        #endif
        #ifdef _xbuf5_lmem_index
            #undef _xbuf5_lmem_index
        #endif
        #ifdef _XBUF_LMEM_HANDLE0
            #if (_XBUF_LMEM_HANDLE0 == 5)
                #undef _XBUF_LMEM_HANDLE0
            #endif
        #endif
        #ifdef _XBUF_LMEM_HANDLE1
            #if (_XBUF_LMEM_HANDLE1 == 5)
                #undef _XBUF_LMEM_HANDLE1
            #endif
        #endif
    #elif (streq(_this_xbuf_name, _xbuf6_lmem_name))
        #undef _xbuf6_lmem_name
        #undef _xbuf6_lmem_size
        #ifdef _xbuf6_lmem_next
            #undef _xbuf6_lmem_next
        #endif
        #ifdef _xbuf6_lmem_pools_base
            #undef _xbuf6_lmem_pools_base
        #endif
        #ifdef _xbuf6_lmem_pool_size
            #undef _xbuf6_lmem_pool_size
        #endif
        #ifdef _xbuf6_lmem_offset
            #undef _xbuf6_lmem_offset
        #endif
        #ifdef _xbuf6_lmem_index
            #undef _xbuf6_lmem_index
        #endif
        #ifdef _XBUF_LMEM_HANDLE0
            #if (_XBUF_LMEM_HANDLE0 == 6)
                #undef _XBUF_LMEM_HANDLE0
            #endif
        #endif
        #ifdef _XBUF_LMEM_HANDLE1
            #if (_XBUF_LMEM_HANDLE1 == 6)
                #undef _XBUF_LMEM_HANDLE1
            #endif
        #endif
    #elif (streq(_this_xbuf_name, _xbuf7_lmem_name))
        #undef _xbuf7_lmem_name
        #undef _xbuf7_lmem_size
        #ifdef _xbuf7_lmem_next
            #undef _xbuf7_lmem_next
        #endif
        #ifdef _xbuf7_lmem_pools_base
            #undef _xbuf7_lmem_pools_base
        #endif
        #ifdef _xbuf7_lmem_pool_size
            #undef _xbuf7_lmem_pool_size
        #endif
        #ifdef _xbuf7_lmem_offset
            #undef _xbuf7_lmem_offset
        #endif
        #ifdef _xbuf7_lmem_index
            #undef _xbuf7_lmem_index
        #endif
        #ifdef _XBUF_LMEM_HANDLE0
            #if (_XBUF_LMEM_HANDLE0 == 7)
                #undef _XBUF_LMEM_HANDLE0
            #endif
        #endif
        #ifdef _XBUF_LMEM_HANDLE1
            #if (_XBUF_LMEM_HANDLE1 == 7)
                #undef _XBUF_LMEM_HANDLE1
            #endif
        #endif
    #elif (streq(_this_xbuf_name, _xbuf8_lmem_name))
        #undef _xbuf8_lmem_name
        #undef _xbuf8_lmem_size
        #ifdef _xbuf8_lmem_next
            #undef _xbuf8_lmem_next
        #endif
        #ifdef _xbuf8_lmem_pools_base
            #undef _xbuf8_lmem_pools_base
        #endif
        #ifdef _xbuf8_lmem_pool_size
            #undef _xbuf8_lmem_pool_size
        #endif
        #ifdef _xbuf8_lmem_offset
            #undef _xbuf8_lmem_offset
        #endif
        #ifdef _xbuf8_lmem_index
            #undef _xbuf8_lmem_index
        #endif
        #ifdef _XBUF_LMEM_HANDLE0
            #if (_XBUF_LMEM_HANDLE0 == 8)
                #undef _XBUF_LMEM_HANDLE0
            #endif
        #endif
        #ifdef _XBUF_LMEM_HANDLE1
            #if (_XBUF_LMEM_HANDLE1 == 8)
                #undef _XBUF_LMEM_HANDLE1
            #endif
        #endif
    #elif (streq(_this_xbuf_name, _xbuf9_lmem_name))
        #undef _xbuf9_lmem_name
        #undef _xbuf9_lmem_size
        #ifdef _xbuf9_lmem_next
            #undef _xbuf9_lmem_next
        #endif
        #ifdef _xbuf9_lmem_pools_base
            #undef _xbuf9_lmem_pools_base
        #endif
        #ifdef _xbuf9_lmem_pool_size
            #undef _xbuf9_lmem_pool_size
        #endif
        #ifdef _xbuf9_lmem_offset
            #undef _xbuf9_lmem_offset
        #endif
        #ifdef _xbuf9_lmem_index
            #undef _xbuf9_lmem_index
        #endif
        #ifdef _XBUF_LMEM_HANDLE0
            #if (_XBUF_LMEM_HANDLE0 == 9)
                #undef _XBUF_LMEM_HANDLE0
            #endif
        #endif
        #ifdef _XBUF_LMEM_HANDLE1
            #if (_XBUF_LMEM_HANDLE1 == 9)
                #undef _XBUF_LMEM_HANDLE1
            #endif
        #endif
    #elif (streq(_this_xbuf_name, _xbuf10_lmem_name))
        #undef _xbuf10_lmem_name
        #undef _xbuf10_lmem_size
        #ifdef _xbuf10_lmem_next
            #undef _xbuf10_lmem_next
        #endif
        #ifdef _xbuf10_lmem_pools_base
            #undef _xbuf10_lmem_pools_base
        #endif
        #ifdef _xbuf10_lmem_pool_size
            #undef _xbuf10_lmem_pool_size
        #endif
        #ifdef _xbuf10_lmem_offset
            #undef _xbuf10_lmem_offset
        #endif
        #ifdef _xbuf10_lmem_index
            #undef _xbuf10_lmem_index
        #endif
        #ifdef _XBUF_LMEM_HANDLE0
            #if (_XBUF_LMEM_HANDLE0 == 10)
                #undef _XBUF_LMEM_HANDLE0
            #endif
        #endif
        #ifdef _XBUF_LMEM_HANDLE1
            #if (_XBUF_LMEM_HANDLE1 == 10)
                #undef _XBUF_LMEM_HANDLE1
            #endif
        #endif
    #elif (streq(_this_xbuf_name, _xbuf11_lmem_name))
        #undef _xbuf11_lmem_name
        #undef _xbuf11_lmem_size
        #ifdef _xbuf11_lmem_next
            #undef _xbuf11_lmem_next
        #endif
        #ifdef _xbuf11_lmem_pools_base
            #undef _xbuf11_lmem_pools_base
        #endif
        #ifdef _xbuf11_lmem_pool_size
            #undef _xbuf11_lmem_pool_size
        #endif
        #ifdef _xbuf11_lmem_offset
            #undef _xbuf11_lmem_offset
        #endif
        #ifdef _xbuf11_lmem_index
            #undef _xbuf11_lmem_index
        #endif
        #ifdef _XBUF_LMEM_HANDLE0
            #if (_XBUF_LMEM_HANDLE0 == 11)
                #undef _XBUF_LMEM_HANDLE0
            #endif
        #endif
        #ifdef _XBUF_LMEM_HANDLE1
            #if (_XBUF_LMEM_HANDLE1 == 11)
                #undef _XBUF_LMEM_HANDLE1
            #endif
        #endif
    #elif (streq(_this_xbuf_name, _xbuf12_lmem_name))
        #undef _xbuf12_lmem_name
        #undef _xbuf12_lmem_size
        #ifdef _xbuf12_lmem_next
            #undef _xbuf12_lmem_next
        #endif
        #ifdef _xbuf12_lmem_pools_base
            #undef _xbuf12_lmem_pools_base
        #endif
        #ifdef _xbuf12_lmem_pool_size
            #undef _xbuf12_lmem_pool_size
        #endif
        #ifdef _xbuf12_lmem_offset
            #undef _xbuf12_lmem_offset
        #endif
        #ifdef _xbuf12_lmem_index
            #undef _xbuf12_lmem_index
        #endif
        #ifdef _XBUF_LMEM_HANDLE0
            #if (_XBUF_LMEM_HANDLE0 == 12)
                #undef _XBUF_LMEM_HANDLE0
            #endif
        #endif
        #ifdef _XBUF_LMEM_HANDLE1
            #if (_XBUF_LMEM_HANDLE1 == 12)
                #undef _XBUF_LMEM_HANDLE1
            #endif
        #endif
    #elif (streq(_this_xbuf_name, _xbuf13_lmem_name))
        #undef _xbuf13_lmem_name
        #undef _xbuf13_lmem_size
        #ifdef _xbuf13_lmem_next
            #undef _xbuf13_lmem_next
        #endif
        #ifdef _xbuf13_lmem_pools_base
            #undef _xbuf13_lmem_pools_base
        #endif
        #ifdef _xbuf13_lmem_pool_size
            #undef _xbuf13_lmem_pool_size
        #endif
        #ifdef _xbuf13_lmem_offset
            #undef _xbuf13_lmem_offset
        #endif
        #ifdef _xbuf13_lmem_index
            #undef _xbuf13_lmem_index
        #endif
        #ifdef _XBUF_LMEM_HANDLE0
            #if (_XBUF_LMEM_HANDLE0 == 13)
                #undef _XBUF_LMEM_HANDLE0
            #endif
        #endif
        #ifdef _XBUF_LMEM_HANDLE1
            #if (_XBUF_LMEM_HANDLE1 == 13)
                #undef _XBUF_LMEM_HANDLE1
            #endif
        #endif
    #elif (streq(_this_xbuf_name, _xbuf14_lmem_name))
        #undef _xbuf14_lmem_name
        #undef _xbuf14_lmem_size
        #ifdef _xbuf14_lmem_next
            #undef _xbuf14_lmem_next
        #endif
        #ifdef _xbuf14_lmem_pools_base
            #undef _xbuf14_lmem_pools_base
        #endif
        #ifdef _xbuf14_lmem_pool_size
            #undef _xbuf14_lmem_pool_size
        #endif
        #ifdef _xbuf14_lmem_offset
            #undef _xbuf14_lmem_offset
        #endif
        #ifdef _xbuf14_lmem_index
            #undef _xbuf14_lmem_index
        #endif
        #ifdef _XBUF_LMEM_HANDLE0
            #if (_XBUF_LMEM_HANDLE0 == 14)
                #undef _XBUF_LMEM_HANDLE0
            #endif
        #endif
        #ifdef _XBUF_LMEM_HANDLE1
            #if (_XBUF_LMEM_HANDLE1 == 14)
                #undef _XBUF_LMEM_HANDLE1
            #endif
        #endif
    #elif (streq(_this_xbuf_name, _xbuf15_lmem_name))
        #undef _xbuf15_lmem_name
        #undef _xbuf15_lmem_size
        #ifdef _xbuf15_lmem_next
            #undef _xbuf15_lmem_next
        #endif
        #ifdef _xbuf15_lmem_pools_base
            #undef _xbuf15_lmem_pools_base
        #endif
        #ifdef _xbuf15_lmem_pool_size
            #undef _xbuf15_lmem_pool_size
        #endif
        #ifdef _xbuf15_lmem_offset
            #undef _xbuf15_lmem_offset
        #endif
        #ifdef _xbuf15_lmem_index
            #undef _xbuf15_lmem_index
        #endif
        #ifdef _XBUF_LMEM_HANDLE0
            #if (_XBUF_LMEM_HANDLE0 == 15)
                #undef _XBUF_LMEM_HANDLE0
            #endif
        #endif
        #ifdef _XBUF_LMEM_HANDLE1
            #if (_XBUF_LMEM_HANDLE1 == 15)
                #undef _XBUF_LMEM_HANDLE1
            #endif
        #endif
    #elif (streq(_this_xbuf_name, _xbuf16_lmem_name))
        #undef _xbuf16_lmem_name
        #undef _xbuf16_lmem_size
        #ifdef _xbuf16_lmem_next
            #undef _xbuf16_lmem_next
        #endif
        #ifdef _xbuf16_lmem_pools_base
            #undef _xbuf16_lmem_pools_base
        #endif
        #ifdef _xbuf16_lmem_pool_size
            #undef _xbuf16_lmem_pool_size
        #endif
        #ifdef _xbuf16_lmem_offset
            #undef _xbuf16_lmem_offset
        #endif
        #ifdef _xbuf16_lmem_index
            #undef _xbuf16_lmem_index
        #endif
        #ifdef _XBUF_LMEM_HANDLE0
            #if (_XBUF_LMEM_HANDLE0 == 16)
                #undef _XBUF_LMEM_HANDLE0
            #endif
        #endif
        #ifdef _XBUF_LMEM_HANDLE1
            #if (_XBUF_LMEM_HANDLE1 == 16)
                #undef _XBUF_LMEM_HANDLE1
            #endif
        #endif
    #else
        #error 2 "_xbuf_lmem_free: Invalid str_xbuf_name."
    #endif

    // Unbind the xbuf name from *l$index0 or *l$index1
    _xbuf_unassociate_lmem_regs(str_xbuf_name)
#endm
/// @endcond


/// @cond INTERNAL_MACROS
/** An internal macro used by xbuf_free for local memory.
 *
 * @instruction_cnt 0
 *
 * Link for xbuf using local memory.
 * Only 2 LMEM buffers of opposite LMEM handles can be linked.
 */
#macro _xbuf_lmem_link(str_xbuf_name, str_nextxbuf_name)
    #ifdef _nextxbuf_index
        #warning "_nextxbuf_index is being redefined."
    #endif

    // check if str_nextxbuf_name is valid
    #define_eval _next_xbuf_name    str_nextxbuf_name

    #if (streq(_next_xbuf_name, _xbuf1_lmem_name))
        #ifndef _xbuf1_lmem_index
            #error "_xbuf_lmem_link: buffer must be associated with localmem handle."
        #else
            #define_eval _nextxbuf_index   (_xbuf1_lmem_index)
        #endif

    #elif (streq(_next_xbuf_name, _xbuf2_lmem_name))
        #ifndef _xbuf2_lmem_index
            #error "_xbuf_lmem_link: buffer must be associated with localmem handle."
        #else
            #define_eval _nextxbuf_index   (_xbuf2_lmem_index)
        #endif

    #elif (streq(_next_xbuf_name, _xbuf3_lmem_name))
        #ifndef _xbuf3_lmem_index
            #error "_xbuf_lmem_link: buffer must be associated with localmem handle."
        #else
            #define_eval _nextxbuf_index   (_xbuf3_lmem_index)
        #endif

    #elif (streq(_next_xbuf_name, _xbuf4_lmem_name))
        #ifndef _xbuf4_lmem_index
            #error "_xbuf_lmem_link: buffer must be associated with localmem handle."
        #else
            #define_eval _nextxbuf_index   (_xbuf4_lmem_index)
        #endif

    #elif (streq(_next_xbuf_name, _xbuf5_lmem_name))
        #ifndef _xbuf5_lmem_index
            #error "_xbuf_lmem_link: buffer must be associated with localmem handle."
        #else
            #define_eval _nextxbuf_index   (_xbuf5_lmem_index)
        #endif

    #elif (streq(_next_xbuf_name, _xbuf6_lmem_name))
        #ifndef _xbuf6_lmem_index
            #error "_xbuf_lmem_link: buffer must be associated with localmem handle."
        #else
            #define_eval _nextxbuf_index   (_xbuf6_lmem_index)
        #endif

    #elif (streq(_next_xbuf_name, _xbuf7_lmem_name))
        #ifndef _xbuf7_lmem_index
            #error "_xbuf_lmem_link: buffer must be associated with localmem handle."
        #else
            #define_eval _nextxbuf_index   (_xbuf7_lmem_index)
        #endif

    #elif (streq(_next_xbuf_name, _xbuf8_lmem_name))
        #ifndef _xbuf8_lmem_index
            #error "_xbuf_lmem_link: buffer must be associated with localmem handle."
        #else
            #define_eval _nextxbuf_index   (_xbuf8_lmem_index)
        #endif

    #elif (streq(_next_xbuf_name, _xbuf9_lmem_name))
        #ifndef _xbuf9_lmem_index
            #error "_xbuf_lmem_link: buffer must be associated with localmem handle."
        #else
            #define_eval _nextxbuf_index   (_xbuf9_lmem_index)
        #endif

    #elif (streq(_next_xbuf_name, _xbuf10_lmem_name))
        #ifndef _xbuf10_lmem_index
            #error "_xbuf_lmem_link: buffer must be associated with localmem handle."
        #else
            #define_eval _nextxbuf_index   (_xbuf10_lmem_index)
        #endif

    #elif (streq(_next_xbuf_name, _xbuf11_lmem_name))
        #ifndef _xbuf11_lmem_index
            #error "_xbuf_lmem_link: buffer must be associated with localmem handle."
        #else
            #define_eval _nextxbuf_index   (_xbuf11_lmem_index)
        #endif

    #elif (streq(_next_xbuf_name, _xbuf12_lmem_name))
        #ifndef _xbuf12_lmem_index
            #error "_xbuf_lmem_link: buffer must be associated with localmem handle."
        #else
            #define_eval _nextxbuf_index   (_xbuf12_lmem_index)
        #endif

    #elif (streq(_next_xbuf_name, _xbuf13_lmem_name))
        #ifndef _xbuf13_lmem_index
            #error "_xbuf_lmem_link: buffer must be associated with localmem handle."
        #else
            #define_eval _nextxbuf_index   (_xbuf13_lmem_index)
        #endif

    #elif (streq(_next_xbuf_name, _xbuf14_lmem_name))
        #ifndef _xbuf14_lmem_index
            #error "_xbuf_lmem_link: buffer must be associated with localmem handle."
        #else
            #define_eval _nextxbuf_index   (_xbuf14_lmem_index)
        #endif

    #elif (streq(_next_xbuf_name, _xbuf15_lmem_name))
        #ifndef _xbuf15_lmem_index
            #error "_xbuf_lmem_link: buffer must be associated with localmem handle."
        #else
            #define_eval _nextxbuf_index   (_xbuf15_lmem_index)
        #endif

    #elif (streq(_next_xbuf_name, _xbuf16_lmem_name))
        #ifndef _xbuf16_lmem_index
            #error "_xbuf_lmem_link: buffer must be associated with localmem handle."
        #else
            #define_eval _nextxbuf_index   (_xbuf16_lmem_index)
        #endif

    #else
        #error 2 "_xbuf_lmem_link: Invalid str_nextxbuf_name."
    #endif

    // check if str_xbuf_name is valid
    #define_eval _this_xbuf_name (str_xbuf_name)

    #if (streq(_this_xbuf_name, _xbuf1_lmem_name))
        #ifndef _xbuf1_lmem_index
            #error "_xbuf_lmem_link: buffer must be associated with localmem handle."
        #else
            #if (_xbuf1_lmem_index == _nextxbuf_index)
                #error "_xbuf_lmem_link: buffers must have different localmem handles."
            #endif
        #endif
        #define_eval _xbuf1_lmem_next  (str_nextxbuf_name)
    #elif (streq(_this_xbuf_name, _xbuf2_lmem_name))
        #ifndef _xbuf2_lmem_index
            #error "_xbuf_lmem_link: buffer must be associated with localmem handle."
        #else
            #if (_xbuf2_lmem_index == _nextxbuf_index)
                #error "_xbuf_lmem_link: buffers must have different localmem handles."
            #endif
        #endif
        #define_eval _xbuf2_lmem_next  (str_nextxbuf_name)
    #elif (streq(_this_xbuf_name, _xbuf3_lmem_name))
        #ifndef _xbuf3_lmem_index
            #error "_xbuf_lmem_link: buffer must be associated with localmem handle."
        #else
            #if (_xbuf3_lmem_index == _nextxbuf_index)
                #error "_xbuf_lmem_link: buffers must have different localmem handles."
            #endif
        #endif
        #define_eval _xbuf3_lmem_next  (str_nextxbuf_name)
    #elif (streq(_this_xbuf_name, _xbuf4_lmem_name))
        #ifndef _xbuf4_lmem_index
            #error "_xbuf_lmem_link: buffer must be associated with localmem handle."
        #else
            #if (_xbuf4_lmem_index == _nextxbuf_index)
                #error "_xbuf_lmem_link: buffers must have different localmem handles."
            #endif
        #endif
        #define_eval _xbuf4_lmem_next  (str_nextxbuf_name)
    #elif (streq(_this_xbuf_name, _xbuf5_lmem_name))
        #ifndef _xbuf5_lmem_index
            #error "_xbuf_lmem_link: buffer must be associated with localmem handle."
        #else
            #if (_xbuf5_lmem_index == _nextxbuf_index)
                #error "_xbuf_lmem_link: buffers must have different localmem handles."
            #endif
        #endif
        #define_eval _xbuf5_lmem_next  (str_nextxbuf_name)
    #elif (streq(_this_xbuf_name, _xbuf6_lmem_name))
        #ifndef _xbuf6_lmem_index
            #error "_xbuf_lmem_link: buffer must be associated with localmem handle."
        #else
            #if (_xbuf6_lmem_index == _nextxbuf_index)
                #error "_xbuf_lmem_link: buffers must have different localmem handles."
            #endif
        #endif
        #define_eval _xbuf6_lmem_next  (str_nextxbuf_name)
    #elif (streq(_this_xbuf_name, _xbuf7_lmem_name))
        #ifndef _xbuf7_lmem_index
            #error "_xbuf_lmem_link: buffer must be associated with localmem handle."
        #else
            #if (_xbuf7_lmem_index == _nextxbuf_index)
                #error "_xbuf_lmem_link: buffers must have different localmem handles."
            #endif
        #endif
        #define_eval _xbuf7_lmem_next  (str_nextxbuf_name)
    #elif (streq(_this_xbuf_name, _xbuf8_lmem_name))
        #ifndef _xbuf8_lmem_index
            #error "_xbuf_lmem_link: buffer must be associated with localmem handle."
        #else
            #if (_xbuf8_lmem_index == _nextxbuf_index)
                #error "_xbuf_lmem_link: buffers must have different localmem handles."
            #endif
        #endif
        #define_eval _xbuf8_lmem_next  (str_nextxbuf_name)
    #elif (streq(_this_xbuf_name, _xbuf9_lmem_name))
        #ifndef _xbuf9_lmem_index
            #error "_xbuf_lmem_link: buffer must be associated with localmem handle."
        #else
            #if (_xbuf9_lmem_index == _nextxbuf_index)
                #error "_xbuf_lmem_link: buffers must have different localmem handles."
            #endif
        #endif
        #define_eval _xbuf9_lmem_next  (str_nextxbuf_name)
    #elif (streq(_this_xbuf_name, _xbuf10_lmem_name))
        #ifndef _xbuf10_lmem_index
            #error "_xbuf_lmem_link: buffer must be associated with localmem handle."
        #else
            #if (_xbuf10_lmem_index == _nextxbuf_index)
                #error "_xbuf_lmem_link: buffers must have different localmem handles."
            #endif
        #endif
        #define_eval _xbuf10_lmem_next  (str_nextxbuf_name)
    #elif (streq(_this_xbuf_name, _xbuf11_lmem_name))
        #ifndef _xbuf11_lmem_index
            #error "_xbuf_lmem_link: buffer must be associated with localmem handle."
        #else
            #if (_xbuf11_lmem_index == _nextxbuf_index)
                #error "_xbuf_lmem_link: buffers must have different localmem handles."
            #endif
        #endif
        #define_eval _xbuf11_lmem_next  (str_nextxbuf_name)
    #elif (streq(_this_xbuf_name, _xbuf12_lmem_name))
        #ifndef _xbuf12_lmem_index
            #error "_xbuf_lmem_link: buffer must be associated with localmem handle."
        #else
            #if (_xbuf12_lmem_index == _nextxbuf_index)
                #error "_xbuf_lmem_link: buffers must have different localmem handles."
            #endif
        #endif
        #define_eval _xbuf12_lmem_next  (str_nextxbuf_name)
    #elif (streq(_this_xbuf_name, _xbuf13_lmem_name))
        #ifndef _xbuf13_lmem_index
            #error "_xbuf_lmem_link: buffer must be associated with localmem handle."
        #else
            #if (_xbuf13_lmem_index == _nextxbuf_index)
                #error "_xbuf_lmem_link: buffers must have different localmem handles."
            #endif
        #endif
        #define_eval _xbuf13_lmem_next  (str_nextxbuf_name)
    #elif (streq(_this_xbuf_name, _xbuf14_lmem_name))
        #ifndef _xbuf14_lmem_index
            #error "_xbuf_lmem_link: buffer must be associated with localmem handle."
        #else
            #if (_xbuf14_lmem_index == _nextxbuf_index)
                #error "_xbuf_lmem_link: buffers must have different localmem handles."
            #endif
        #endif
        #define_eval _xbuf14_lmem_next  (str_nextxbuf_name)
    #elif (streq(_this_xbuf_name, _xbuf15_lmem_name))
        #ifndef _xbuf15_lmem_index
            #error "_xbuf_lmem_link: buffer must be associated with localmem handle."
        #else
            #if (_xbuf15_lmem_index == _nextxbuf_index)
                #error "_xbuf_lmem_link: buffers must have different localmem handles."
            #endif
        #endif
        #define_eval _xbuf15_lmem_next  (str_nextxbuf_name)
    #elif (streq(_this_xbuf_name, _xbuf16_lmem_name))
        #ifndef _xbuf16_lmem_index
            #error "_xbuf_lmem_link: buffer must be associated with localmem handle."
        #else
            #if (_xbuf16_lmem_index == _nextxbuf_index)
                #error "_xbuf_lmem_link: buffers must have different localmem handles."
            #endif
        #endif
        #define_eval _xbuf16_lmem_next  (str_nextxbuf_name)
    #else
        #error 2 "_xbuf_lmem_link: Invalid str_xbuf_name."
    #endif

    #ifdef _nextxbuf_index
        #undef _nextxbuf_index
    #endif
#endm
/// @endcond


/// @cond INTERNAL_MACROS
/** An internal macro used by xbuf_param_set for local memory.
 *
 * @instruction_cnt 0
 *
 * Support up to 16 local memory buffers.
 */
#macro _xbuf_lmem_param_set(str_xbuf_name)
    #ifdef _cur_xbuf_lmem_name
        #undef _cur_xbuf_lmem_name
    #endif
    #ifdef _cur_xbuf_lmem_size
        #undef _cur_xbuf_lmem_size
    #endif
    #ifdef _cur_xbuf_lmem_next
        #undef _cur_xbuf_lmem_next
    #endif
    #define_eval _cur_xbuf_lmem_name (str_xbuf_name)

    #if streq(_cur_xbuf_lmem_name, _xbuf1_lmem_name)
        #define _cur_xbuf_lmem_size _xbuf1_lmem_size
        #ifdef _xbuf1_lmem_next
            #define _cur_xbuf_lmem_next _xbuf1_lmem_next
        #endif
    #elif streq(_cur_xbuf_lmem_name, _xbuf2_lmem_name)
        #define _cur_xbuf_lmem_size _xbuf2_lmem_size
        #ifdef _xbuf2_lmem_next
            #define _cur_xbuf_lmem_next _xbuf2_lmem_next
        #endif
    #elif streq(_cur_xbuf_lmem_name, _xbuf3_lmem_name)
        #define _cur_xbuf_lmem_size _xbuf3_lmem_size
        #ifdef _xbuf3_lmem_next
            #define _cur_xbuf_lmem_next _xbuf3_lmem_next
        #endif
    #elif streq(_cur_xbuf_lmem_name, _xbuf4_lmem_name)
        #define _cur_xbuf_lmem_size _xbuf4_lmem_size
        #ifdef _xbuf4_lmem_next
            #define _cur_xbuf_lmem_next _xbuf4_lmem_next
        #endif
    #elif streq(_cur_xbuf_lmem_name, _xbuf5_lmem_name)
        #define _cur_xbuf_lmem_size _xbuf5_lmem_size
        #ifdef _xbuf5_lmem_next
            #define _cur_xbuf_lmem_next _xbuf5_lmem_next
        #endif
    #elif streq(_cur_xbuf_lmem_name, _xbuf6_lmem_name)
        #define _cur_xbuf_lmem_size _xbuf6_lmem_size
        #ifdef _xbuf6_lmem_next
            #define _cur_xbuf_lmem_next _xbuf6_lmem_next
        #endif
    #elif streq(_cur_xbuf_lmem_name, _xbuf7_lmem_name)
        #define _cur_xbuf_lmem_size _xbuf7_lmem_size
        #ifdef _xbuf7_lmem_next
            #define _cur_xbuf_lmem_next _xbuf7_lmem_next
        #endif
    #elif streq(_cur_xbuf_lmem_name, _xbuf8_lmem_name)
        #define _cur_xbuf_lmem_size _xbuf8_lmem_size
        #ifdef _xbuf8_lmem_next
            #define _cur_xbuf_lmem_next _xbuf8_lmem_next
        #endif

    #elif streq(_cur_xbuf_lmem_name, _xbuf9_lmem_name)
        #define _cur_xbuf_lmem_size _xbuf9_lmem_size
        #ifdef _xbuf9_lmem_next
            #define _cur_xbuf_lmem_next _xbuf9_lmem_next
        #endif
    #elif streq(_cur_xbuf_lmem_name, _xbuf10_lmem_name)
        #define _cur_xbuf_lmem_size _xbuf10_lmem_size
        #ifdef _xbuf10_lmem_next
            #define _cur_xbuf_lmem_next _xbuf10_lmem_next
        #endif
    #elif streq(_cur_xbuf_lmem_name, _xbuf11_lmem_name)
        #define _cur_xbuf_lmem_size _xbuf11_lmem_size
        #ifdef _xbuf11_lmem_next
            #define _cur_xbuf_lmem_next _xbuf11_lmem_next
        #endif
    #elif streq(_cur_xbuf_lmem_name, _xbuf12_lmem_name)
        #define _cur_xbuf_lmem_size _xbuf12_lmem_size
        #ifdef _xbuf12_lmem_next
            #define _cur_xbuf_lmem_next _xbuf12_lmem_next
        #endif
    #elif streq(_cur_xbuf_lmem_name, _xbuf13_lmem_name)
        #define _cur_xbuf_lmem_size _xbuf13_lmem_size
        #ifdef _xbuf13_lmem_next
            #define _cur_xbuf_lmem_next _xbuf13_lmem_next
        #endif
    #elif streq(_cur_xbuf_lmem_name, _xbuf14_lmem_name)
        #define _cur_xbuf_lmem_size _xbuf14_lmem_size
        #ifdef _xbuf14_lmem_next
            #define _cur_xbuf_lmem_next _xbuf14_lmem_next
        #endif
    #elif streq(_cur_xbuf_lmem_name, _xbuf15_lmem_name)
        #define _cur_xbuf_lmem_size _xbuf15_lmem_size
        #ifdef _xbuf15_lmem_next
            #define _cur_xbuf_lmem_next _xbuf15_lmem_next
        #endif
    #elif streq(_cur_xbuf_lmem_name, _xbuf16_lmem_name)
        #define _cur_xbuf_lmem_size _xbuf16_lmem_size
        #ifdef _xbuf16_lmem_next
            #define _cur_xbuf_lmem_next _xbuf16_lmem_next
        #endif

    #else
        #error 4 "_xbuf_lmem_param_set: no matching str_xbuf_name."
    #endif
#endm
/// @endcond


/// @cond INTERNAL_MACROS
/**
 * An internal macro used by xbuf main macros.
 * @instruction_cnt 0
 *
 * Given a start byte and end byte within a word,
 * this macro returns the byte mask that can be used
 * in ld_field instruction.
 */
#macro _GET_MASK_FROM_SB_EB(SBYTE, EBYTE)
    #ifdef _SB_EB_MASK_VAL
        #error "_SB_EB_MASK_VAL is already defined"
    #endif

    #if (SBYTE == 0)
        #if (EBYTE == 0)
            #define _SB_EB_MASK_VAL 1000
        #elif (EBYTE == 1)
            #define _SB_EB_MASK_VAL 1100
        #elif (EBYTE == 2)
            #define _SB_EB_MASK_VAL 1110
        #elif (EBYTE == 3)
            #define _SB_EB_MASK_VAL 1111
        #endif
    #elif (SBYTE == 1)
        #if (EBYTE == 1)
            #define _SB_EB_MASK_VAL 0100
        #elif (EBYTE == 2)
            #define _SB_EB_MASK_VAL 0110
        #elif (EBYTE == 3)
            #define _SB_EB_MASK_VAL 0111
        #endif
    #elif (SBYTE == 2)
        #if (EBYTE == 2)
            #define _SB_EB_MASK_VAL 0010
        #elif (EBYTE == 3)
            #define _SB_EB_MASK_VAL 0011
        #endif
    #elif (SBYTE==3)
        #if (EBYTE == 3)
            #define _SB_EB_MASK_VAL 0001
        #endif
    #endif
#endm
/// @endcond


/// @cond INTERNAL_MACROS
/** An internal macro used by xbuf main macros.
 *
 * @instruction_cnt 0
 *
 * Gets the no of shift to be right shifted based on start byte.
 */
#macro _GET_RSHIFT_FROM_SB(SBYTE)
    #ifdef _R_SHIFT_VAL_FROM_SB
        #error "_R_SHIFT_VAL_FROM_SB is already defined"
    #endif

    #if (SBYTE == 0)
            #define_eval _R_SHIFT_VAL_FROM_SB 0
    #elif (SBYTE == 1)
            #define_eval _R_SHIFT_VAL_FROM_SB 8
    #elif (SBYTE == 2)
            #define_eval _R_SHIFT_VAL_FROM_SB 16
    #elif (SBYTE==3)
            #define_eval _R_SHIFT_VAL_FROM_SB 24
    #endif
#endm
/// @endcond


/// @cond INTERNAL_MACROS
/** An internal macro used by xbuf main macros.
 *
 * @instruction_cnt 0
 *
 * Gets the no of shift to be left shifted based on end byte.
 */
#macro _GET_LSHIFT_FROM_EB(EBYTE)
    #ifdef _L_SHIFT_VAL_FROM_EB
        #error "_L_SHIFT_VAL_FROM_EB is already defined"
    #endif

    #if (EBYTE == 0)
            #define _L_SHIFT_VAL_FROM_EB 24
    #elif (EBYTE == 1)
            #define _L_SHIFT_VAL_FROM_EB 16
    #elif (EBYTE == 2)
            #define _L_SHIFT_VAL_FROM_EB 8
    #elif (EBYTE==3)
            #define _L_SHIFT_VAL_FROM_EB 0
    #endif
#endm
/// @endcond


/// @cond INTERNAL_MACROS
/** An internal macro used by xbuf main macros.
 *
 * @instruction_cnt 0
 *
 * Gets the no of left shift bits based on remaining bytes.
 */
#macro _GET_LSHIFT_FROM_REMAINING_BYTES(RBYTES)
    #ifdef _L_SHIFT_VAL_FROM_REMAINING_BYTES
        #error "_L_SHIFT_VAL_FROM_REMAINING_BYTES is already defined"
    #endif

    #if (RBYTES == 0)
            #define _L_SHIFT_VAL_FROM_REMAINING_BYTES 0
    #elif (RBYTES == 1)
            #define _L_SHIFT_VAL_FROM_REMAINING_BYTES 24
    #elif (RBYTES == 2)
            #define _L_SHIFT_VAL_FROM_REMAINING_BYTES 16
    #elif (RBYTES==3)
            #define _L_SHIFT_VAL_FROM_REMAINING_BYTES 8
    #else
            #error /**/RBYTES "is illegal value"
    #endif
#endm
/// @endcond


/// @cond INTERNAL_MACROS
/** An internal macro used by xbuf main macros.
 *
 * @instruction_cnt 8-9
 *
 * Generic internal macro to perform xbuf_insert.
 * Uses _str_word0_xfer and possibly _str_word1_xfer.
 *
 */
#macro _perform_xbuf_insert(io_str_xbuf_name, in_byte_field, WINDOW_START, FIELD_START, \
                            NUMBER_OF_BYTES)
    .reg save_active_lm temp_reg_0
    .reg  temp_reg_1

    #if ((_INSERT_XBUF_TYPE == _XBUF_LMEM) && ( ((WINDOW_START + FIELD_START) / 4) > 7) )
        ;work-around for problem where ld_field can't access local mem reg with index > 7

        #define_eval _START_R((WINDOW_START + FIELD_START) >> 4) << 4

        local_csr_rd[ACTIVE_LM_ADDR_/**/_INSERT_SRC_LMEM_INDEX]
        immed[save_active_lm, 0]

        alu[temp_reg_0, _START_R, + , save_active_lm]
        local_csr_wr[ACTIVE_LM_ADDR_/**/_INSERT_SRC_LMEM_INDEX, temp_reg_0]
        nop
        nop
        nop

        #define_eval _START_B ((WINDOW_START + FIELD_START) & 0xf)

        _find_xbuf_and_relative_sb_eb(io_str_xbuf_name, 0, _START_B, NUMBER_OF_BYTES)
    #else
        _find_xbuf_and_relative_sb_eb(io_str_xbuf_name, WINDOW_START, FIELD_START, NUMBER_OF_BYTES)
    #endif

    #define_eval _SBYTE_IN_WD (_RELATIVE_SBYTE & 0x3)
    #define_eval _EBYTE_IN_WD (_RELATIVE_EBYTE & 0x3)


    #if (_WORD0 == _WORD1)
        #ifdef _str_word0_xfer
            _GET_MASK_FROM_SB_EB(_SBYTE_IN_WD, _EBYTE_IN_WD)
            _GET_LSHIFT_FROM_EB(_EBYTE_IN_WD)
            bytefield_shf_left_insert(_str_word0_xfer, _SB_EB_MASK_VAL, in_byte_field, _L_SHIFT_VAL_FROM_EB, DO_LOAD_CC)

            #undef _SB_EB_MASK_VAL
            #undef _L_SHIFT_VAL_FROM_EB
        #endif
    #else
        #ifdef _str_word0_xfer
            #ifdef _str_word1_xfer
                _GET_MASK_FROM_SB_EB(_SBYTE_IN_WD, 3)
                _GET_LSHIFT_FROM_EB(_EBYTE_IN_WD)
                bytefield_shf_right_insert(_str_word0_xfer, _SB_EB_MASK_VAL, in_byte_field, (32-_L_SHIFT_VAL_FROM_EB), DO_LOAD_CC)
                #undef _SB_EB_MASK_VAL

                _GET_MASK_FROM_SB_EB(0, _EBYTE_IN_WD)
                bytefield_shf_left_insert(_str_word1_xfer, _SB_EB_MASK_VAL, in_byte_field, _L_SHIFT_VAL_FROM_EB, DO_LOAD_CC)
                #undef _SB_EB_MASK_VAL
                #undef _L_SHIFT_VAL_FROM_EB
            #endif
        #endif
    #endif

    #if ((_INSERT_XBUF_TYPE == _XBUF_LMEM) && ( ((WINDOW_START + FIELD_START) / 4) > 7) )

        local_csr_wr[ACTIVE_LM_ADDR_/**/_INSERT_SRC_LMEM_INDEX, save_active_lm]
        nop
        nop
        nop

        #undef _START_R
        #undef _START_B
    #endif

    _clear_name_space_for_find_xbuf_and_relative_sb_eb()
#endm
/// @endcond


/// @cond INTERNAL_MACROS
/** An internal macro used by xbuf main macros.
 *
 * @instruction_cnt 0
 *
 * Internal function get get the XBUF_TYPE.
 */
#macro _GET_XBUF_TYPE(str_xbuf_name)
    #ifdef _XBUF_TYPE
        #warning "_XBUF_TYPE is being redefined"
    #endif

    #if (strstr(str_xbuf_name, n$) == 1)
        #error "Next neighbor registers are not supported by xbuf."
    #elif ((!defined(__NFP_INDIRECT_REF_FORMAT_NFPXXXX)) && (strstr(str_xbuf_name, $$) > 0))
        #define_eval _XBUF_TYPE  _XBUF_DRAM_XFER
    #elif ( streq(str_xbuf_name, 'lmem_buf0') || \
            streq(str_xbuf_name, 'lmem_buf1') || \
            streq(str_xbuf_name, 'lmem_buf2') || \
            streq(str_xbuf_name, 'lmem_buf3') || \
            streq(str_xbuf_name, 'lmem_buf4') || \
            streq(str_xbuf_name, 'lmem_buf5') || \
            streq(str_xbuf_name, 'lmem_buf6') || \
            streq(str_xbuf_name, 'lmem_buf7') || \
            streq(str_xbuf_name, 'lmem_buf8') || \
            streq(str_xbuf_name, 'lmem_buf9') || \
            streq(str_xbuf_name, 'lmem_buf10') || \
            streq(str_xbuf_name, 'lmem_buf11') || \
            streq(str_xbuf_name, 'lmem_buf12') || \
            streq(str_xbuf_name, 'lmem_buf13') || \
            streq(str_xbuf_name, 'lmem_buf14') || \
            streq(str_xbuf_name, 'lmem_buf15') )
        #define_eval _XBUF_TYPE  _XBUF_LMEM
    #elif (strstr(str_xbuf_name, $) == 1)
        #define_eval _XBUF_TYPE  _XBUF_SRAM_XFER
    #else
        #define_eval _XBUF_TYPE  _XBUF_GPR
    #endif
#endm
/// @endcond


/// @cond INTERNAL_MACROS
/** An internal macro used by xbuf_extract and xbuf_copy.
 *
 * To find the buffer that actually contains the element pointed to
 * by start_loc. The actual buffer is in the buffer chain starting with
 * buffer str_xbuf_name.
 *
 * This function supports linked buffers. Since there are maximum 4 buffers in
 * a chain (and 8 for NFP mode xfers), the actual start element can be 3 (or 7)
 * buffers away from @p str_xbuf_name
 * (next_buf3 in the illustration below). @n
 * str_xbuf_name->next_buf1->next_buf2->next_buf3
 *
 * Even though this function supports linked buffers, users should be very
 * careful when passing in buffers that do not actually contain the element to
 * be worked on. This will cause several extra intructions.
 *
 * @param out_buf_num    Set to 0, 1, 2, or 3 (or 4, 6 or 7 in NFP indirect reference mode):
 *                       @arg 0: Element is found in first buffer, ie. buffer _start_buf_name
 *                       @arg 1: Element is found in buffer that linked to str_xbuf_name,
 *                               ie. _next_buf1_name
 *                       @arg 2: Element is found in buffer that linked to 1, ie. _next_buf2_name
 *                       @arg 3: Element is found in buffer that linked to 2, ie. _next_buf3_name
 * @param str_xbuf_name  String to specify name of buffer to start
 * @param io_start_loc   GPR or constant.
 *                       Offset from beginning of buffer str_xbuf_name
 *                       where element to be found is
 * @note
 * @b Outputs: @n
 * Set global internal params:
 * @arg _start_buf_name, _next_buf1_name, _next_buf2_name, and _next_buf3_name
 * @arg io_start_loc:
 * @arg If GPR, set to the offset of the element in the absolute
 *      buffer (the buffer that actually contains the element).
 * @arg If constant, define _actual_xbuf_name to be the buffer
 *      that actually contains the element pointed by io_start_loc.
 */
#macro _find_actual_xbuf(out_buf_num, str_xbuf_name, io_start_loc)
.begin

    .reg tmp_start

    _GET_XBUF_TYPE(str_xbuf_name)
    #ifdef _FIND_ACTUAL_XBUF_STR_XBUF_TYPE
        #warning "_FIND_ACTUAL_XBUF_STR_XBUF_TYPE is being redefined."
    #endif
    #define_eval _FIND_ACTUAL_XBUF_STR_XBUF_TYPE _XBUF_TYPE
    #undef _XBUF_TYPE

    #ifdef  _actual_xbuf_name
        #warning "_actual_xbuf_name is being redefined"
    #endif
    #ifdef  _actual_sbyte
        #warning "_actual_sbyte is being redefined"
    #endif
    #ifdef  _start_buf_name
        #warning "_start_buf_name is being redefined"
    #endif
    #ifdef  _next_buf1_name
        #warning "_next_buf1_name is being redefined"
    #endif
    #ifdef  _next_buf2_name
        #warning "_next_buf2_name is being redefined"
    #endif
    #ifdef  _next_buf3_name
        #warning "_next_buf3_name is being redefined"
    #endif

    #if (defined(__NFP_INDIRECT_REF_FORMAT_NFPXXXX) && (_FIND_ACTUAL_XBUF_STR_XBUF_TYPE == _XBUF_SRAM_XFER))
        #ifdef  _next_buf4_name
            #warning "_next_buf4_name is being redefined"
        #endif
        #ifdef  _next_buf5_name
            #warning "_next_buf5_name is being redefined"
        #endif
        #ifdef  _next_buf6_name
            #warning "_next_buf6_name is being redefined"
        #endif
        #ifdef  _next_buf7_name
            #warning "_next_buf7_name is being redefined"
        #endif
    #endif

    #ifdef  _start_buf_size_in_bytes
        #warning "_start_buf_size_in_bytes is being redefined"
    #endif
    #ifdef  _next_buf1_size_in_bytes
        #warning "_next_buf1_size_in_bytes is being redefined"
    #endif
    #ifdef  _next_buf2_size_in_bytes
        #warning "_next_buf2_size_in_bytes is being redefined"
    #endif
    #ifdef  _next_buf3_size_in_bytes
        #warning "_next_buf3_size_in_bytes is being redefined"
    #endif

    #if (defined(__NFP_INDIRECT_REF_FORMAT_NFPXXXX) && (_FIND_ACTUAL_XBUF_STR_XBUF_TYPE == _XBUF_SRAM_XFER))
        #ifdef  _next_buf4_size_in_bytes
            #warning "_next_buf4_size_in_bytes is being redefined"
        #endif
        #ifdef  _next_buf5_size_in_bytes
            #warning "_next_buf5_size_in_bytes is being redefined"
        #endif
        #ifdef  _next_buf6_size_in_bytes
            #warning "_next_buf6_size_in_bytes is being redefined"
        #endif
        #ifdef  _next_buf7_size_in_bytes
            #warning "_next_buf7_size_in_bytes is being redefined"
        #endif
    #endif

    _DEFINE_THIS_XBUF(str_xbuf_name)
    #define_eval    _start_buf_name _this_xbuf_name
    #define_eval    _start_buf_size_in_bytes    (_this_xbuf_size * 4)

    #ifdef _this_xbuf_next
        #define_eval _next  _this_xbuf_next
        _UNDEFINE_THIS_XBUF()
        _DEFINE_THIS_XBUF(_next)
        #undef _next
        #define_eval    _next_buf1_name _this_xbuf_name
        #define_eval    _next_buf1_size_in_bytes    (_this_xbuf_size * 4)

        #ifdef _this_xbuf_next
            #define_eval _next  _this_xbuf_next
            _UNDEFINE_THIS_XBUF()
            _DEFINE_THIS_XBUF(_next)
            #undef _next
            #define_eval    _next_buf2_name _this_xbuf_name
            #define_eval    _next_buf2_size_in_bytes    (_this_xbuf_size * 4)

            #ifdef _this_xbuf_next
                #define_eval _next  _this_xbuf_next
                _UNDEFINE_THIS_XBUF()
                _DEFINE_THIS_XBUF(_next)
                #undef _next
                #define_eval    _next_buf3_name _this_xbuf_name
                #define_eval    _next_buf3_size_in_bytes    (_this_xbuf_size * 4)

                #if (defined(__NFP_INDIRECT_REF_FORMAT_NFPXXXX) && (_FIND_ACTUAL_XBUF_STR_XBUF_TYPE == _XBUF_SRAM_XFER))

                    #ifdef _this_xbuf_next
                        #define_eval _next  _this_xbuf_next
                        _UNDEFINE_THIS_XBUF()
                        _DEFINE_THIS_XBUF(_next)
                        #undef _next
                        #define_eval    _next_buf4_name _this_xbuf_name
                        #define_eval    _next_buf4_size_in_bytes    (_this_xbuf_size * 4)

                        #ifdef _this_xbuf_next
                            #define_eval _next  _this_xbuf_next
                            _UNDEFINE_THIS_XBUF()
                            _DEFINE_THIS_XBUF(_next)
                            #undef _next
                            #define_eval    _next_buf5_name _this_xbuf_name
                            #define_eval    _next_buf5_size_in_bytes    (_this_xbuf_size * 4)

                            #ifdef _this_xbuf_next
                                #define_eval _next  _this_xbuf_next
                                _UNDEFINE_THIS_XBUF()
                                _DEFINE_THIS_XBUF(_next)
                                #undef _next
                                #define_eval    _next_buf6_name _this_xbuf_name
                                #define_eval    _next_buf6_size_in_bytes    (_this_xbuf_size * 4)

                                #ifdef _this_xbuf_next
                                    #define_eval _next  _this_xbuf_next
                                    _UNDEFINE_THIS_XBUF()
                                    _DEFINE_THIS_XBUF(_next)
                                    #undef _next
                                    #define_eval    _next_buf7_name _this_xbuf_name
                                    #define_eval    _next_buf7_size_in_bytes    (_this_xbuf_size * 4)
                                #else
                                    #define_eval    _next_buf7_name _start_buf_name
                                    #define_eval    _next_buf7_size_in_bytes    (_start_buf_size_in_bytes * 4)
                                #endif
                            #else
                                #define_eval    _next_buf6_name _start_buf_name
                                #define_eval    _next_buf6_size_in_bytes    (_start_buf_size_in_bytes * 4)
                                #define_eval    _next_buf7_name _start_buf_name
                                #define_eval    _next_buf7_size_in_bytes    (_start_buf_size_in_bytes * 4)
                            #endif
                        #else
                            #define_eval    _next_buf5_name _start_buf_name
                            #define_eval    _next_buf5_size_in_bytes    (_start_buf_size_in_bytes * 4)
                            #define_eval    _next_buf6_name _start_buf_name
                            #define_eval    _next_buf6_size_in_bytes    (_start_buf_size_in_bytes * 4)
                            #define_eval    _next_buf7_name _start_buf_name
                            #define_eval    _next_buf7_size_in_bytes    (_start_buf_size_in_bytes * 4)
                        #endif
                    #else
                        #define_eval    _next_buf4_name _start_buf_name
                        #define_eval    _next_buf4_size_in_bytes    (_start_buf_size_in_bytes * 4)
                        #define_eval    _next_buf5_name _start_buf_name
                        #define_eval    _next_buf5_size_in_bytes    (_start_buf_size_in_bytes * 4)
                        #define_eval    _next_buf6_name _start_buf_name
                        #define_eval    _next_buf6_size_in_bytes    (_start_buf_size_in_bytes * 4)
                        #define_eval    _next_buf7_name _start_buf_name
                        #define_eval    _next_buf7_size_in_bytes    (_start_buf_size_in_bytes * 4)
                    #endif

                #endif
            #else
                //if no next xbuf, just define all name as the first buffer
                #define_eval    _next_buf3_name _start_buf_name
                #define_eval    _next_buf3_size_in_bytes    (_start_buf_size_in_bytes * 4)
                #if (defined(__NFP_INDIRECT_REF_FORMAT_NFPXXXX) && (_FIND_ACTUAL_XBUF_STR_XBUF_TYPE == _XBUF_SRAM_XFER))
                    #define_eval    _next_buf4_name _start_buf_name
                    #define_eval    _next_buf4_size_in_bytes    (_start_buf_size_in_bytes * 4)
                    #define_eval    _next_buf5_name _start_buf_name
                    #define_eval    _next_buf5_size_in_bytes    (_start_buf_size_in_bytes * 4)
                    #define_eval    _next_buf6_name _start_buf_name
                    #define_eval    _next_buf6_size_in_bytes    (_start_buf_size_in_bytes * 4)
                    #define_eval    _next_buf7_name _start_buf_name
                    #define_eval    _next_buf7_size_in_bytes    (_start_buf_size_in_bytes * 4)
                #endif
            #endif
        #else
            //if no next xbuf, just define all name as the first buffer
            #define_eval    _next_buf2_name _start_buf_name
            #define_eval    _next_buf2_size_in_bytes    (_start_buf_size_in_bytes * 4)
            #define_eval    _next_buf3_name _start_buf_name
            #define_eval    _next_buf3_size_in_bytes    (_start_buf_size_in_bytes * 4)
            #if (defined(__NFP_INDIRECT_REF_FORMAT_NFPXXXX) && (_FIND_ACTUAL_XBUF_STR_XBUF_TYPE == _XBUF_SRAM_XFER))
                #define_eval    _next_buf4_name _start_buf_name
                #define_eval    _next_buf4_size_in_bytes    (_start_buf_size_in_bytes * 4)
                #define_eval    _next_buf5_name _start_buf_name
                #define_eval    _next_buf5_size_in_bytes    (_start_buf_size_in_bytes * 4)
                #define_eval    _next_buf6_name _start_buf_name
                #define_eval    _next_buf6_size_in_bytes    (_start_buf_size_in_bytes * 4)
                #define_eval    _next_buf7_name _start_buf_name
                #define_eval    _next_buf7_size_in_bytes    (_start_buf_size_in_bytes * 4)
            #endif
        #endif
    #else
        //if no next xbuf, just define all name as the first buffer
        #define_eval    _next_buf1_name _start_buf_name
        #define_eval    _next_buf1_size_in_bytes    (_start_buf_size_in_bytes * 4)
        #define_eval    _next_buf2_name _start_buf_name
        #define_eval    _next_buf2_size_in_bytes    (_start_buf_size_in_bytes * 4)
        #define_eval    _next_buf3_name _start_buf_name
        #define_eval    _next_buf3_size_in_bytes    (_start_buf_size_in_bytes * 4)
        #if (defined(__NFP_INDIRECT_REF_FORMAT_NFPXXXX) && (_FIND_ACTUAL_XBUF_STR_XBUF_TYPE == _XBUF_SRAM_XFER))
            #define_eval    _next_buf4_name _start_buf_name
            #define_eval    _next_buf4_size_in_bytes    (_start_buf_size_in_bytes * 4)
            #define_eval    _next_buf5_name _start_buf_name
            #define_eval    _next_buf5_size_in_bytes    (_start_buf_size_in_bytes * 4)
            #define_eval    _next_buf6_name _start_buf_name
            #define_eval    _next_buf6_size_in_bytes    (_start_buf_size_in_bytes * 4)
            #define_eval    _next_buf7_name _start_buf_name
            #define_eval    _next_buf7_size_in_bytes    (_start_buf_size_in_bytes * 4)
        #endif
    #endif  //ifdef _this_xbuf_next

    _UNDEFINE_THIS_XBUF()

    #if (is_ct_const(io_start_loc) || isimport(io_start_loc))

        #define_eval _temp_start _start_buf_size_in_bytes

        #if (io_start_loc < _temp_start)
            #define_eval _actual_xbuf_name  _start_buf_name
            #define_eval _actual_sbyte io_start_loc
        #elif (io_start_loc < (_temp_start + _next_buf1_size_in_bytes)
            #define_eval _actual_xbuf_name  _next_buf1_name
            #define_eval _actual_sbyte (io_start_loc - _temp_start)
            #define_eval _temp_start (_temp_start + _next_buf1_size_in_bytes)
        #elif (io_start_loc < (_temp_start + _next_buf2_size_in_bytes)
            #define_eval _actual_xbuf_name  _next_buf2_name
            #define_eval _actual_sbyte (io_start_loc - _temp_start)
            #define_eval _temp_start (temp_start + _next_buf2_size_in_bytes)
        #elif (io_start_loc < (_temp_start + _next_buf3_size_in_bytes)
            #define_eval _actual_xbuf_name  _next_buf3_name
            #define_eval _actual_sbyte (io_start_loc - _temp_start)
            #define_eval _temp_start (tem_start + _next_buf3_size_in_bytes)
        #elif (defined(__NFP_INDIRECT_REF_FORMAT_NFPXXXX) && (_FIND_ACTUAL_XBUF_STR_XBUF_TYPE == _XBUF_SRAM_XFER))
            #if (io_start_loc < (_temp_start + _next_buf4_size_in_bytes)
                #define_eval _actual_xbuf_name  _next_buf4_name
                #define_eval _actual_sbyte (io_start_loc - _temp_start)
                #define_eval _temp_start (tem_start + _next_buf4_size_in_bytes)
            #elif  (io_start_loc < (_temp_start + _next_buf5_size_in_bytes)
                #define_eval _actual_xbuf_name  _next_buf5_name
                #define_eval _actual_sbyte (io_start_loc - _temp_start)
                #define_eval _temp_start (tem_start + _next_buf5_size_in_bytes)
            #elif  (io_start_loc < (_temp_start + _next_buf6_size_in_bytes)
                #define_eval _actual_xbuf_name  _next_buf6_name
                #define_eval _actual_sbyte (io_start_loc - _temp_start)
                #define_eval _temp_start (tem_start + _next_buf6_size_in_bytes)
            #elif  (io_start_loc < (_temp_start + _next_buf7_size_in_bytes)
                #define_eval _actual_xbuf_name  _next_buf7_name
                #define_eval _actual_sbyte (io_start_loc - _temp_start)
                #define_eval _temp_start (tem_start + _next_buf7_size_in_bytes)
            #endif
        #endif

    #else

        alu[out_buf_num, --, B, 0]

        alu[tmp_start, io_start_loc, -, _start_buf_size_in_bytes]
        //offset starts at 0. If offset == size, it's already beyond the current buffer
        blt[end_find_actual_xbuf#]
    next1#:
        #if (!streq(_next_buf1_name, _start_buf_name))
            alu[tmp_start, tmp_start, -, _next_buf1_size_in_bytes]
            //offset starts at 0. If offset == size, it's already beyond the current buffer
            bge[next2#], defer[2]
            alu[io_start_loc, tmp_start, +, _next_buf1_size_in_bytes]
            alu[out_buf_num, --, B, 1]
        #endif
            br[end_find_actual_xbuf#]
    next2#:
        #if (!streq(_next_buf2_name, _start_buf_name))
            alu[tmp_start, tmp_start, -, _next_buf2_size_in_bytes]
            //offset starts at 0. If offset == size, it's already beyond the current buffer
            bge[next3#], defer[2]
            alu[io_start_loc, tmp_start, +, _next_buf2_size_in_bytes]
            alu[out_buf_num, --, B, 2]
        #endif
            br[end_find_actual_xbuf#]

    #if (defined(__NFP_INDIRECT_REF_FORMAT_NFPXXXX) && (_FIND_ACTUAL_XBUF_STR_XBUF_TYPE == _XBUF_SRAM_XFER))

    next3#:
        #if (!streq(_next_buf3_name, _start_buf_name))
            alu[tmp_start, tmp_start, -, _next_buf3_size_in_bytes]
            //offset starts at 0. If offset == size, it's already beyond the current buffer
            bge[next4#], defer[2]
            alu[io_start_loc, tmp_start, +, _next_buf3_size_in_bytes]
            alu[out_buf_num, --, B, 3]
        #endif
            br[end_find_actual_xbuf#]

    next4#:
        #if (!streq(_next_buf4_name, _start_buf_name))
            alu[tmp_start, tmp_start, -, _next_buf4_size_in_bytes]
            //offset starts at 0. If offset == size, it's already beyond the current buffer
            bge[next5#], defer[2]
            alu[io_start_loc, tmp_start, +, _next_buf4_size_in_bytes]
            alu[out_buf_num, --, B, 4]
        #endif
            br[end_find_actual_xbuf#]

    next5#:
        #if (!streq(_next_buf5_name, _start_buf_name))
            alu[tmp_start, tmp_start, -, _next_buf5_size_in_bytes]
            //offset starts at 0. If offset == size, it's already beyond the current buffer
            bge[next6#], defer[2]
            alu[io_start_loc, tmp_start, +, _next_buf5_size_in_bytes]
            alu[out_buf_num, --, B, 5]
        #endif
            br[end_find_actual_xbuf#]

    next6#:
        #if (!streq(_next_buf6_name, _start_buf_name))
            alu[tmp_start, tmp_start, -, _next_buf6_size_in_bytes]
            //offset starts at 0. If offset == size, it's already beyond the current buffer
            bge[next7#], defer[2]
            alu[io_start_loc, tmp_start, +, _next_buf6_size_in_bytes]
            alu[out_buf_num, --, B, 6]
        #endif
            br[end_find_actual_xbuf#]

    next7#:
        #if (!streq(_next_buf7_name, _start_buf_name))
            alu[tmp_start, tmp_start, -, _next_buf7_size_in_bytes]
            //offset starts at 0. If offset == size, it's already beyond the current buffer
            bge[end_find_actual_xbuf#], defer[2]
            alu[io_start_loc, tmp_start, +, _next_buf7_size_in_bytes]
            alu[out_buf_num, --, B, 7]
        #endif

    #else
        next3#:
            #if (!streq(_next_buf3_name, _start_buf_name))
                alu[tmp_start, tmp_start, -, _next_buf3_size_in_bytes]
                //offset starts at 0. If offset == size, it's already beyond the current buffer
                bge[end_find_actual_xbuf#], defer[2]
                alu[io_start_loc, tmp_start, +, _next_buf3_size_in_bytes]
                alu[out_buf_num, --, B, 3]
            #endif
    #endif

    end_find_actual_xbuf#:

    #endif //if (is_ct_const(io_start_loc))

    #ifdef  _start_buf_size_in_bytes
        #undef _start_buf_size_in_bytes
    #endif
    #ifdef  _next_buf1_size_in_bytes
        #undef _next_buf1_size_in_bytes
    #endif
    #ifdef  _next_buf2_size_in_bytes
        #undef _next_buf2_size_in_bytes
    #endif
    #ifdef  _next_buf3_size_in_bytes
        #undef _next_buf3_size_in_bytes
    #endif
    #if (defined(__NFP_INDIRECT_REF_FORMAT_NFPXXXX) && (_FIND_ACTUAL_XBUF_STR_XBUF_TYPE == _XBUF_SRAM_XFER))
        #ifdef  _next_buf4_size_in_bytes
            #undef _next_buf4_size_in_bytes
        #endif
        #ifdef  _next_buf5_size_in_bytes
            #undef _next_buf5_size_in_bytes
        #endif
        #ifdef  _next_buf6_size_in_bytes
            #undef _next_buf6_size_in_bytes
        #endif
        #ifdef  _next_buf7_size_in_bytes
            #undef _next_buf7_size_in_bytes
        #endif
    #endif

    #undef _FIND_ACTUAL_XBUF_STR_XBUF_TYPE

.end
#endm
/// @endcond


/// @cond INTERNAL_MACROS
/** An internal macro used by xbuf main macros.
 *
 * @instruction_cnt 0
 *
 * An internal function to be used by xbuf_copy and related copy macros.
 */
#macro _DEFINE_THIS_XBUF(str_xbuf_name)
    #ifdef _this_xbuf_name
        #undef _this_xbuf_name
    #endif
    #ifdef _this_xbuf_size
        #undef _this_xbuf_size
    #endif
    #ifdef _this_xbuf_next
        #undef _this_xbuf_next
    #endif

    _GET_XBUF_TYPE(str_xbuf_name)

    #if ((!defined(__NFP_INDIRECT_REF_FORMAT_NFPXXXX)) && (_XBUF_TYPE == _XBUF_DRAM_XFER))

        #define_eval _this_xbuf_name (str_xbuf_name)

        #if streq(_this_xbuf_name, _xbuf_d1_name)
            #define _this_xbuf_size _xbuf_d1_size
            #ifdef _xbuf_d1_next
                #define _this_xbuf_next _xbuf_d1_next
            #endif
        #elif streq(_this_xbuf_name, _xbuf_d2_name)
            #define _this_xbuf_size _xbuf_d2_size
            #ifdef _xbuf_d2_next
                #define _this_xbuf_next _xbuf_d2_next
            #endif
        #elif streq(_this_xbuf_name, _xbuf_d3_name)
            #define _this_xbuf_size _xbuf_d3_size
            #ifdef _xbuf_d3_next
                #define _this_xbuf_next _xbuf_d3_next
            #endif
        #elif streq(_this_xbuf_name, _xbuf_d4_name)
            #define _this_xbuf_size _xbuf_d4_size
            #ifdef _xbuf_d4_next
                #define _this_xbuf_next _xbuf_d4_next
            #endif
        #else
            #error 4 "_DEFINE_THIS_XBUF: no matching str_xbuf_name."
        #endif

    #elif (_XBUF_TYPE == _XBUF_SRAM_XFER)

        #define_eval _this_xbuf_name (str_xbuf_name)

        #if streq(str_xbuf_name, _xbuf1_name)
            #define _this_xbuf_size _xbuf1_size
            #ifdef _xbuf1_next
                #define _this_xbuf_next _xbuf1_next
            #endif
        #elif streq(str_xbuf_name, _xbuf2_name)
            #define _this_xbuf_size _xbuf2_size
            #ifdef _xbuf2_next
                #define _this_xbuf_next _xbuf2_next
            #endif
        #elif streq(str_xbuf_name, _xbuf3_name)
            #define _this_xbuf_size _xbuf3_size
            #ifdef _xbuf3_next
                #define _this_xbuf_next _xbuf3_next
            #endif
        #elif streq(str_xbuf_name, _xbuf4_name)
            #define _this_xbuf_size _xbuf4_size
            #ifdef _xbuf4_next
                #define _this_xbuf_next _xbuf4_next
            #endif
        #elif (defined(__NFP_INDIRECT_REF_FORMAT_NFPXXXX) && streq(str_xbuf_name, _xbuf5_name))
            #define _this_xbuf_size _xbuf5_size
            #ifdef _xbuf5_next
                #define _this_xbuf_next _xbuf5_next
            #endif
        #elif (defined(__NFP_INDIRECT_REF_FORMAT_NFPXXXX) && streq(str_xbuf_name, _xbuf6_name))
            #define _this_xbuf_size _xbuf6_size
            #ifdef _xbuf6_next
                #define _this_xbuf_next _xbuf6_next
            #endif
        #elif (defined(__NFP_INDIRECT_REF_FORMAT_NFPXXXX) && streq(str_xbuf_name, _xbuf7_name))
            #define _this_xbuf_size _xbuf7_size
            #ifdef _xbuf7_next
                #define _this_xbuf_next _xbuf7_next
            #endif
        #elif (defined(__NFP_INDIRECT_REF_FORMAT_NFPXXXX) && streq(str_xbuf_name, _xbuf8_name))
            #define _this_xbuf_size _xbuf8_size
            #ifdef _xbuf8_next
                #define _this_xbuf_next _xbuf8_next
            #endif
        #else
            #error 4 "_DEFINE_THIS_XBUF: no matching str_xbuf_name."
        #endif
    #elif (_XBUF_TYPE == _XBUF_LMEM)

        #define_eval _this_xbuf_name (str_xbuf_name)

        #ifdef _this_xbuf_index
            #undef _this_xbuf_index
        #endif

        #if streq(_this_xbuf_name, _xbuf1_lmem_name)
            #define _this_xbuf_size _xbuf1_lmem_size
            #ifdef _xbuf1_lmem_next
                #define _this_xbuf_next _xbuf1_lmem_next
            #endif
            #ifdef _xbuf1_lmem_index
                #define _this_xbuf_index _xbuf1_lmem_index
            #else
                #warning "_DEFINE_THIS_XBUF: lmem buffer must be activated."
            #endif
        #elif streq(_this_xbuf_name, _xbuf2_lmem_name)
            #define _this_xbuf_size _xbuf2_lmem_size
            #ifdef _xbuf2_lmem_next
                #define _this_xbuf_next _xbuf2_lmem_next
            #endif
            #ifdef _xbuf2_lmem_index
                #define _this_xbuf_index _xbuf2_lmem_index
            #else
                #warning "_DEFINE_THIS_XBUF: lmem buffer must be activated."
            #endif
        #elif streq(_this_xbuf_name, _xbuf3_lmem_name)
            #define _this_xbuf_size _xbuf3_lmem_size
            #ifdef _xbuf3_lmem_next
                #define _this_xbuf_next _xbuf3_lmem_next
            #endif
            #ifdef _xbuf3_lmem_index
                #define _this_xbuf_index _xbuf3_lmem_index
            #else
                #warning "_DEFINE_THIS_XBUF: lmem buffer must be activated."
            #endif
        #elif streq(_this_xbuf_name, _xbuf4_lmem_name)
            #define _this_xbuf_size _xbuf4_lmem_size
            #ifdef _xbuf4_lmem_next
                #define _this_xbuf_next _xbuf4_lmem_next
            #endif
            #ifdef _xbuf4_lmem_index
                #define _this_xbuf_index _xbuf4_lmem_index
            #else
                #warning "_DEFINE_THIS_XBUF: lmem buffer must be activated."
            #endif
        #elif streq(_this_xbuf_name, _xbuf5_lmem_name)
            #define _this_xbuf_size _xbuf5_lmem_size
            #ifdef _xbuf5_lmem_next
                #define _this_xbuf_next _xbuf5_lmem_next
            #endif
            #ifdef _xbuf5_lmem_index
                #define _this_xbuf_index _xbuf5_lmem_index
            #else
                #warning "_DEFINE_THIS_XBUF: lmem buffer must be activated."
            #endif
        #elif streq(_this_xbuf_name, _xbuf6_lmem_name)
            #define _this_xbuf_size _xbuf6_lmem_size
            #ifdef _xbuf6_lmem_next
                #define _this_xbuf_next _xbuf6_lmem_next
            #endif
            #ifdef _xbuf6_lmem_index
                #define _this_xbuf_index _xbuf6_lmem_index
            #else
                #warning "_DEFINE_THIS_XBUF: lmem buffer must be activated."
            #endif
        #elif streq(_this_xbuf_name, _xbuf7_lmem_name)
            #define _this_xbuf_size _xbuf7_lmem_size
            #ifdef _xbuf7_lmem_next
                #define _this_xbuf_next _xbuf7_lmem_next
            #endif
            #ifdef _xbuf7_lmem_index
                #define _this_xbuf_index _xbuf7_lmem_index
            #else
                #warning "_DEFINE_THIS_XBUF: lmem buffer must be activated."
            #endif
        #elif streq(_this_xbuf_name, _xbuf8_lmem_name)
            #define _this_xbuf_size _xbuf8_lmem_size
            #ifdef _xbuf8_lmem_next
                #define _this_xbuf_next _xbuf8_lmem_next
            #endif
            #ifdef _xbuf8_lmem_index
                #define _this_xbuf_index _xbuf8_lmem_index
            #else
                #warning "_DEFINE_THIS_XBUF: lmem buffer must be activated."
            #endif
        #elif streq(_this_xbuf_name, _xbuf9_lmem_name)
            #define _this_xbuf_size _xbuf9_lmem_size
            #ifdef _xbuf9_lmem_next
                #define _this_xbuf_next _xbuf9_lmem_next
            #endif
            #ifdef _xbuf9_lmem_index
                #define _this_xbuf_index _xbuf9_lmem_index
            #else
                #warning "_DEFINE_THIS_XBUF: lmem buffer must be activated."
            #endif
        #elif streq(_this_xbuf_name, _xbuf10_lmem_name)
            #define _this_xbuf_size _xbuf10_lmem_size
            #ifdef _xbuf10_lmem_next
                #define _this_xbuf_next _xbuf10_lmem_next
            #endif
            #ifdef _xbuf10_lmem_index
                #define _this_xbuf_index _xbuf10_lmem_index
            #else
                #warning "_DEFINE_THIS_XBUF: lmem buffer must be activated."
            #endif
        #elif streq(_this_xbuf_name, _xbuf11_lmem_name)
            #define _this_xbuf_size _xbuf11_lmem_size
            #ifdef _xbuf11_lmem_next
                #define _this_xbuf_next _xbuf11_lmem_next
            #endif
            #ifdef _xbuf11_lmem_index
                #define _this_xbuf_index _xbuf11_lmem_index
            #else
                #warning "_DEFINE_THIS_XBUF: lmem buffer must be activated."
            #endif
        #elif streq(_this_xbuf_name, _xbuf12_lmem_name)
            #define _this_xbuf_size _xbuf12_lmem_size
            #ifdef _xbuf12_lmem_next
                #define _this_xbuf_next _xbuf12_lmem_next
            #endif
            #ifdef _xbuf12_lmem_index
                #define _this_xbuf_index _xbuf12_lmem_index
            #else
                #warning "_DEFINE_THIS_XBUF: lmem buffer must be activated."
            #endif
        #elif streq(_this_xbuf_name, _xbuf13_lmem_name)
            #define _this_xbuf_size _xbuf13_lmem_size
            #ifdef _xbuf13_lmem_next
                #define _this_xbuf_next _xbuf13_lmem_next
            #endif
            #ifdef _xbuf13_lmem_index
                #define _this_xbuf_index _xbuf13_lmem_index
            #else
                #warning "_DEFINE_THIS_XBUF: lmem buffer must be activated."
            #endif
        #elif streq(_this_xbuf_name, _xbuf14_lmem_name)
            #define _this_xbuf_size _xbuf14_lmem_size
            #ifdef _xbuf14_lmem_next
                #define _this_xbuf_next _xbuf14_lmem_next
            #endif
            #ifdef _xbuf14_lmem_index
                #define _this_xbuf_index _xbuf14_lmem_index
            #else
                #warning "_DEFINE_THIS_XBUF: lmem buffer must be activated."
            #endif
        #elif streq(_this_xbuf_name, _xbuf15_lmem_name)
            #define _this_xbuf_size _xbuf15_lmem_size
            #ifdef _xbuf15_lmem_next
                #define _this_xbuf_next _xbuf15_lmem_next
            #endif
            #ifdef _xbuf15_lmem_index
                #define _this_xbuf_index _xbuf15_lmem_index
            #else
                #warning "_DEFINE_THIS_XBUF: lmem buffer must be activated."
            #endif
        #elif streq(_this_xbuf_name, _xbuf16_lmem_name)
            #define _this_xbuf_size _xbuf16_lmem_size
            #ifdef _xbuf16_lmem_next
                #define _this_xbuf_next _xbuf16_lmem_next
            #endif
            #ifdef _xbuf16_lmem_index
                #define _this_xbuf_index _xbuf16_lmem_index
            #else
                #warning "_DEFINE_THIS_XBUF: lmem buffer must be activated."
            #endif
        #else
            #error 4 "_DEFINE_THIS_XBUF: no matching str_xbuf_name."
        #endif

    #elif (_XBUF_TYPE == _XBUF_GPR)

        #define_eval _this_xbuf_name (str_xbuf_name)

        #if streq(_this_xbuf_name, _xbuf1_gpr_name)
            #define _this_xbuf_size _xbuf1_gpr_size
            #ifdef _xbuf1_gpr_next
                #define _this_xbuf_next _xbuf1_gpr_next
            #endif
        #elif streq(_this_xbuf_name, _xbuf2_gpr_name)
            #define _this_xbuf_size _xbuf2_gpr_size
            #ifdef _xbuf2_gpr_next
                #define _this_xbuf_next _xbuf2_gpr_next
            #endif
        #elif streq(_this_xbuf_name, _xbuf3_gpr_name)
            #define _this_xbuf_size _xbuf3_gpr_size
            #ifdef _xbuf3_gpr_next
                #define _this_xbuf_next _xbuf3_gpr_next
            #endif
        #elif streq(_this_xbuf_name, _xbuf4_gpr_name)
            #define _this_xbuf_size _xbuf4_gpr_size
            #ifdef _xbuf4_gpr_next
                #define _this_xbuf_next _xbuf4_gpr_next
            #endif
        #else
            #error 4 "_DEFINE_THIS_XBUF: no matching str_xbuf_name."
        #endif
    #else
            #error 4 "_DEFINE_THIS_XBUF: illegal _XBUF_TYPE."
    #endif

    #undef _XBUF_TYPE
#endm
/// @endcond


/// @cond INTERNAL_MACROS
/** This macro sets _this_total_size_in_long_words and
 * _this_total_size_in_bytes which
 * is the total size of all the buffers in the linked chain
 * starting from the xbuf with str_xbuf_name.
 */
#macro _FIND_TOTAL_LINKED_XBUF_SIZE(str_xbuf_name)
    #ifdef _this_total_size_in_long_word
        #warning "_this_total_size_in_long_word is being redefined"
        #undef _this_total_size_in_long_word
    #endif

    #ifdef _this_total_size_in_bytes
        #warning "_this_total_size_in_bytes is being redefined"
        #undef _this_total_size_in_bytes
    #endif

    //start from the given xbuf
    _DEFINE_THIS_XBUF(str_xbuf_name)

    _GET_XBUF_TYPE(str_xbuf_name)
    #ifdef _FIND_TOTAL_LINKED_XBUF_SIZE_STR_XBUF_TYPE
        #warning "_FIND_TOTAL_LINKED_XBUF_SIZE_STR_XBUF_TYPE is being redefined."
    #endif
    #define_eval _FIND_TOTAL_LINKED_XBUF_SIZE_STR_XBUF_TYPE _XBUF_TYPE
    #undef _XBUF_TYPE

    #define_eval _this_total_size_in_long_words _this_xbuf_size


    #if (defined(__NFP_INDIRECT_REF_FORMAT_NFPXXXX) && \
            (_FIND_TOTAL_LINKED_XBUF_SIZE_STR_XBUF_TYPE == _XBUF_SRAM_XFER))
        #define_eval _FIND_TOTAL_LINKED_XBUF_SIZE_CNT_BUFFERS 8
    #else
        #define_eval _FIND_TOTAL_LINKED_XBUF_SIZE_CNT_BUFFERS 4
    #endif

    #define_eval _FIND_TOTAL_LINKED_XBUF_SIZE_LOOP_A (_FIND_TOTAL_LINKED_XBUF_SIZE_CNT_BUFFERS - 1)
    #while ( _FIND_TOTAL_LINKED_XBUF_SIZE_LOOP_A > 0 )
        #ifdef _this_xbuf_next
            _DEFINE_THIS_XBUF(_this_xbuf_next)
            #define_eval _this_total_size_in_long_words (_this_total_size_in_long_words  + _this_xbuf_size)
            #define_eval _FIND_TOTAL_LINKED_XBUF_SIZE_LOOP_A (_FIND_TOTAL_LINKED_XBUF_SIZE_LOOP_A - 1)
        #else
            #define_eval _FIND_TOTAL_LINKED_XBUF_SIZE_LOOP_A 0
        #endif
    #endloop
    #undef _FIND_TOTAL_LINKED_XBUF_SIZE_LOOP_A
    #undef _FIND_TOTAL_LINKED_XBUF_SIZE_CNT_BUFFERS

    #define_eval _this_total_size_in_bytes (_this_total_size_in_long_words * 4)
#endm
/// @endcond


/// @cond INTERNAL_MACROS
/** An internal macro used by xbuf main macros.
 *
 * @instruction_cnt 0
 */
#macro _CLEAR_NAME_SPACE_FOR_FIND_TOTAL_LINKED_XBUF_SIZE
    _UNDEFINE_THIS_XBUF()
    #undef _this_total_size_in_bytes
    #undef _this_total_size_in_long_words
#endm
/// @endcond


/// @cond INTERNAL_MACROS
/** An internal macro used by xbuf main macros.
 *
 * @instruction_cnt 0
 */
#macro _UNDEFINE_THIS_XBUF()
    #undef _this_xbuf_name
    #undef _this_xbuf_size

    #ifdef _this_xbuf_next
        #undef _this_xbuf_next
    #endif

    #ifdef _this_xbuf_index
        #undef _this_xbuf_index
    #endif
#endm
/// @endcond


/// @cond INTERNAL_MACROS
/** An internal macro used by xbuf main macros.
 *
 * @instruction_cnt 0
 */
#macro _copy_lm_regs_above_8_to_temp_buf(_src_xbuf, _SRC_SIZE, _START_INDEX, _NUM_LW)
    #ifdef _ALLOCATED_TMP_SRC_XBUF
        //xbuf_free(tmp_src_buf)
        #undef _ALLOCATED_TMP_SRC_XBUF
    #endif

    #ifdef _ALLOCATED_NEXT_TMP_SRC_XBUF
    //  xbuf_free(tmp_src_buf)
        #undef _ALLOCATED_NEXT_TMP_SRC_XBUF
    #endif

    //For _S_ALIGN > 0, x longwords comes from x+1 source registers.
    //Hence, size of tmp_src_buf is 1 more than the long words number
    #define_eval _MAX (_START_INDEX + _NUM_LW + 1)
    #if (_MAX > _SRC_SIZE)
        #define_eval _MAX   _SRC_SIZE
    #endif

    #if (_START_INDEX < 8)
        #define_eval _SRC_LM_INDEX_ABOVE_8 (_MAX - 8)
    #else
        #define_eval _SRC_LM_INDEX_ABOVE_8 (_MAX - _START_INDEX)
    #endif
    #undef _MAX

    #if (_SRC_LM_INDEX_ABOVE_8 > 0)
        _DEFINE_THIS_XBUF(_src_xbuf)
        #if (streq(_this_xbuf_name, _SRC_XBUF_NAME))
            xbuf_alloc(tmp_src_buf, _SRC_LM_INDEX_ABOVE_8, read_write)
            #define _ALLOCATED_TMP_SRC_XBUF
            #define_eval _the_buffer        tmp_src_buf
            #define_eval _TMP_SRC_BUF_SIZE  _SRC_LM_INDEX_ABOVE_8
        #else
            xbuf_alloc(next_tmp_src_buf, _SRC_LM_INDEX_ABOVE_8, read_write)
            #define _ALLOCATED_NEXT_TMP_SRC_XBUF
            #define_eval _the_buffer        next_tmp_src_buf
            #define_eval _NEXT_TMP_SRC_BUF_SIZE     _SRC_LM_INDEX_ABOVE_8
        #endif
        _UNDEFINE_THIS_XBUF()

        #define_eval i  0
        #while (i < _SRC_LM_INDEX_ABOVE_8)
            .set _the_buffer[i]
            #define_eval    i (i + 1)
        #endloop

        //moving in_src_xbuf[8-15] to tmp_src_buf
        //because byte_align_be doesn't allow local memory index > 7
        #if (_START_INDEX > 8)
            #define_eval    i   _START_INDEX
        #else
            #define_eval    i   8
        #endif
        #define_eval    j   0
        #while ((j < _SRC_LM_INDEX_ABOVE_8) && (i < _SRC_SIZE))

            alu[_the_buffer[/**/j], --, B, _src_xbuf[/**/i]]
            #define_eval    i   (i + 1)
            #define_eval    j   (j + 1)
        #endloop
    #endif

    #ifdef _the_buffer
        #undef _the_buffer
    #endif
#endm
/// @endcond


/// @cond INTERNAL_MACROS
/** An internal macro that copies from source buffer to destination buffer for the
 * following configuration:
 * @arg dest_offset: Run-time (must be longword aligned)
 * @arg src_offset:  Constant (must be longword aligned)
 * @arg size:        Constant (0 - 64)
 *
 * @param out_dest_xbuf     Destination buffer where data is copied to
 * @param out_last_element  A GPR or 0 for do not care.
 *                          Contains left justified remaining bytes from last LW.
 *                          Other bytes are 0.
 * @param dest_start_byte   Run-time destination offset where first byte of data starts
 * @param in_src_xbuf       Source buffer where data is copied from
 * @param SRC_START_BYTE    Constant source offset where first byte of data is copied to
 * @param in_prepend        NOT USED for this macro since dest_start_byte and SRC_START_BYTE
 *                          must be longword aligned
 * @param NUMBER_OF_BYTES   Constant copy size, from 0 - 64 bytes
 *
 * @instruction_cnt
 */
#macro _copy_var_aligned_dest_const_aligned_src_const_size(out_dest_xbuf, out_last_element, \
                dest_start_byte, in_src_xbuf, SRC_START_BYTE, in_prepend, NUMBER_OF_BYTES)
    #if ( !(is_ct_const(SRC_START_BYTE)  || isimport(SRC_START_BYTE)))
        #error 2 "_copy_var_aligned_dest_const_aligned_src_const_size: SRC_START_BYTE must be constant."
    #endif

    #if ( !(is_ct_const(NUMBER_OF_BYTES)  || isimport(NUMBER_OF_BYTES)))
        #error 2 "_copy_var_aligned_dest_const_aligned_src_const_size: NUMBER_OF_BYTES must be constant."
    #endif

    #if (NUMBER_OF_BYTES > 0)

        #ifdef _SRC_XBUF_TYPE
            #warning "_SRC_XBUF_TYPE is being redefined."
        #endif

        #ifdef _DEST_XBUF_TYPE
            #warning "_DEST_XBUF_TYPE is being redefined."
        #endif

        // get xbuf type for both source and destination buffers
        _GET_XBUF_TYPE(in_src_xbuf)
        #define_eval _SRC_XBUF_TYPE    _XBUF_TYPE
        #undef _XBUF_TYPE

        _GET_XBUF_TYPE(out_dest_xbuf)
        #define_eval _DEST_XBUF_TYPE   _XBUF_TYPE
        #undef _XBUF_TYPE

        #ifdef __NUM_WORD
            #warning "__NUM_WORD is being redefined."
        #endif
        #ifdef __NUM_BYTE
            #warning "__NUM_BYTE is being redefined."
        #endif
        #ifdef _LAST_ELEMENT_MASK
            #warning "_LAST_ELEMENT_MASK is being redefined."
        #endif

        // find number of longwords and number of remaining bytes to copy
        #define_eval __NUM_WORD (NUMBER_OF_BYTES / 4)
        #define_eval __NUM_BYTE (NUMBER_OF_BYTES % 4)

        // define mask to clear bytes for the last output element
        #if (__NUM_BYTE == 1)
            #define_eval _LAST_ELEMENT_MASK     1000
        #elif (__NUM_BYTE == 2)
            #define_eval _LAST_ELEMENT_MASK     1100
        #elif (__NUM_BYTE == 3)
            #define_eval _LAST_ELEMENT_MASK     1110
        #endif

        // set start register for source buffer
        xbuf_xfer_set(_BUF0, in_src_xbuf, SRC_START_BYTE)

        // for lmem, restriction applies for > 8th indexed registers
        #if (_SRC_XBUF_TYPE == _XBUF_LMEM)
            #ifdef _num_copies_
                #warning "_num_copies_ is being redefined."
            #endif

            #define_eval _num_copies_ (SRC_START_BYTE + NUMBER_OF_BYTES)
            #if (_num_copies_ > 32)
                #define _lm_more_than_8th_reg_
                .begin

                    .reg _lm_tmp_
            #endif

            #undef _num_copies_
        #endif

        #if ( streq(in_src_xbuf, out_dest_xbuf) )
        // ... then their xbuf_type must be the same.
        // If it's GPR or LMEM type, and if same start byte, do not copy.
            #if ((_SRC_XBUF_TYPE == _XBUF_GPR) || (_SRC_XBUF_TYPE == _XBUF_LMEM))
                // the following define is necessary for case "beq" is true because
                // i is only defined for SRAM/DRAM cases. The _BUF0_REG/**/i at the
                // target label will need value of i.
                #define_eval i  (__NUM_WORD)
                alu[--, dest_start_byte, -, SRC_START_BYTE]
                beq[process_out_last_element_dest_var#]
            #endif
        #endif

        // set start register for destination buffer
        #if ( (_DEST_XBUF_TYPE == _XBUF_SRAM_XFER) || (_DEST_XBUF_TYPE == _XBUF_DRAM_XFER) )
        // SRAM or DRAM type
        .begin

            .reg _t_index_val

            br=ctx[0, _copy_var_aligned_dest_skip_find_abs_addr#], defer[2]
            add_shf_left(_t_index_val, dest_start_byte, (&out_dest_xbuf/**/0), 2)

            //if not context 0, need to add offset to absolute reg to T_INDEX value
            _find_absolute_xfer_reg_addr(_t_index_val)

_copy_var_aligned_dest_skip_find_abs_addr#:

            local_csr_wr[T_INDEX, _t_index_val]
            nop
            nop
            nop

        .end

        #else
        // LMEM or GPR type
            #error "This case is not supported."
        #endif

        #if ( (_DEST_XBUF_TYPE == _XBUF_SRAM_XFER) || (_DEST_XBUF_TYPE == _XBUF_DRAM_XFER) )
        // SRAM or DRAM type
            #if (_DEST_XBUF_TYPE == _XBUF_SRAM_XFER)
                #define_eval _DST_POS   *$index
            #elif ( (!defined(__NFP_INDIRECT_REF_FORMAT_NFPXXXX)) && (_DEST_XBUF_TYPE == _XBUF_DRAM_XFER) )
                #define_eval _DST_POS   *$$index
            #endif

            // now copy _NUM_WORD longwords
            #define_eval i  0
            #while (i < __NUM_WORD)
                move(_DST_POS/**/++, _BUF0_REG/**/i)
                #define_eval i  (i + 1)
            #endloop

            // copy the remaining bytes for the last longword
            #if (__NUM_BYTE != 0)
                #ifdef _lm_more_than_8th_reg_
                    move(_lm_tmp_, _BUF0_REG/**/i)
                    ld_field_w_clr[_DST_POS, _LAST_ELEMENT_MASK, _lm_tmp_]
                #else
                    ld_field_w_clr[_DST_POS, _LAST_ELEMENT_MASK, _BUF0_REG/**/i]
                #endif
            #endif

        #else
        // LMEM or GPR type
                // **********************************
                // Provide code here
                // **********************************
        #endif

process_out_last_element_dest_var#:

        // caller wants to use out_last_element
        #if (!(is_ct_const(out_last_element) || isimport(out_last_element)))
            #if (__NUM_BYTE != 0)
                #ifdef _lm_more_than_8th_reg_
                    move(_lm_tmp_, _BUF0_REG/**/i)
                    ld_field_w_clr[out_last_element, _LAST_ELEMENT_MASK, _lm_tmp_]
                #else
                    ld_field_w_clr[out_last_element, _LAST_ELEMENT_MASK, _BUF0_REG/**/i]
                #endif
            #else
                #define_eval i  (i - 1)
                move(out_last_element, _BUF0_REG/**/i)
            #endif
        #endif

_copy_var_aligned_dest_const_aligned_src_const_size_done#:

        // clean up name spaces
        #ifdef i
            #undef i
        #endif

        #ifdef _LAST_ELEMENT_MASK
            #undef _LAST_ELEMENT_MASK
        #endif

        #ifdef _lm_more_than_8th_reg_
            #undef _lm_more_than_8th_reg_

            .end
        #endif

        #undef _SRC_XBUF_TYPE
        #undef _DEST_XBUF_TYPE
        #undef _NUM_WORD
        #undef _NUM_BYTE

    #endif  // (NUMBER_OF_BYTES > 0)
#endm
/// @endcond


/// @cond INTERNAL_MACROS
/** An internal macro that copies from source buffer to destination buffer for the
 *      following configuration:
 * @arg dest_offset: Constant (must be longword aligned)
 * @arg src_offset:  Run-time (must be longword aligned)
 * @arg size:        Constant (0 - 64)
 *
 * @param out_dest_xbuf      Destination buffer where data is copied to
 * @param out_last_element   A GPR or 0 for do not care.
 *                           Contains left justified remaining bytes from the last LW.
 *                           Other bytes are 0.
 * @param DEST_START_BYTE    Constant destination offset where first byte of data starts
 * @param in_src_xbuf        Source buffer where data is copied from
 * @param src_start_byte     Run-time source offset where first byte of data is copied to
 * @param in_prepend         NOT USED for this macro since dest_start_byte and SRC_START_BYTE
 *                           must be longword aligned
 * @param NUMBER_OF_BYTES    Constant copy size, from 0 - 64 bytes
 *
 * @instruction_cnt
 */
#macro _copy_const_aligned_dest_var_aligned_src_const_size(out_dest_xbuf, out_last_element, \
                DEST_START_BYTE, in_src_xbuf, src_start_byte, in_prepend, NUMBER_OF_BYTES)
    #if ( !(is_ct_const(DEST_START_BYTE)  || isimport(DEST_START_BYTE)))
        #error 2 "_copy_const_aligned_dest_var_aligned_src_const_size: DEST_START_BYTE must be constant."
    #endif

    #if ( !(is_ct_const(NUMBER_OF_BYTES)  || isimport(NUMBER_OF_BYTES)))
        #error 2 "_copy_const_aligned_dest_var_aligned_src_const_size: NUMBER_OF_BYTES must be constant."
    #endif

    #if (NUMBER_OF_BYTES > 0)

        #ifdef _SRC_XBUF_TYPE
            #warning "_SRC_XBUF_TYPE is being redefined."
        #endif

        #ifdef _DEST_XBUF_TYPE
            #warning "_DEST_XBUF_TYPE is being redefined."
        #endif

        // get xbuf type for both source and destination buffers
        _GET_XBUF_TYPE(in_src_xbuf)
        #define_eval _SRC_XBUF_TYPE    _XBUF_TYPE
        #undef _XBUF_TYPE

        _GET_XBUF_TYPE(out_dest_xbuf)
        #define_eval _DEST_XBUF_TYPE   _XBUF_TYPE
        #undef _XBUF_TYPE

        #ifdef __NUM_WORD
            #warning "__NUM_WORD is being redefined."
        #endif
        #ifdef __NUM_BYTE
            #warning "__NUM_BYTE is being redefined."
        #endif
        #ifdef _LAST_ELEMENT_MASK
            #warning "_LAST_ELEMENT_MASK is being redefined."
        #endif

        // find number of longwords and number of remaining bytes to copy
        #define_eval __NUM_WORD (NUMBER_OF_BYTES / 4)
        #define_eval __NUM_BYTE (NUMBER_OF_BYTES % 4)

        // define mask to clear bytes for the last output element
        #if (__NUM_BYTE == 1)
            #define_eval _LAST_ELEMENT_MASK     1000
        #elif (__NUM_BYTE == 2)
            #define_eval _LAST_ELEMENT_MASK     1100
        #elif (__NUM_BYTE == 3)
            #define_eval _LAST_ELEMENT_MASK     1110
        #endif

        // set start register for destination buffer
        xbuf_xfer_set(_BUF0, out_dest_xbuf, DEST_START_BYTE)

        // for lmem, restriction applies for > 8th indexed registers
        #if (_DEST_XBUF_TYPE == _XBUF_LMEM)
            #ifdef _num_copies_
                #warning "_num_copies_ is being redefined."
            #endif

            #define_eval _num_copies_ (DEST_START_BYTE + NUMBER_OF_BYTES)
            #if (_num_copies_ > 32)
                #define _lm_more_than_8th_reg_
                .begin

                    .reg _lm_tmp_
            #endif

            #undef _num_copies_
        #endif

        // find the buffer index for non-copy cases (same GPR, same LMEM)
        //  to update out_last_element
        #if (__NUM_BYTE != 0)
            #if (__NUM_WORD == 0)       // only 1, 2 or 3 bytes copied
                #define_eval j  0
            #else
                #define_eval j  (__NUM_WORD)
            #endif
        #else
            #define_eval j      (__NUM_WORD - 1)
        #endif

        // check for non-copy cases
        #if ( streq(in_src_xbuf, out_dest_xbuf) )
        // ... then their xbuf_type must be the same.
        // If it's GPR or LMEM type, and if same start byte, do not copy.
            #if ((_SRC_XBUF_TYPE == _XBUF_GPR) || (_SRC_XBUF_TYPE == _XBUF_LMEM))
                alu[--, src_start_byte, -, DEST_START_BYTE]

                #if (!(is_ct_const(out_last_element) || isimport(out_last_element)))
                    #ifdef _lm_more_than_8th_reg_
                        beq[_copy_const_aligned_dest_var_aligned_src_const_size_done#], defer[2]
                        move(_lm_tmp_, _BUF0_REG/**/j)
                        ld_field_w_clr[out_last_element, _LAST_ELEMENT_MASK, _lm_tmp_]
                    #else
                        beq[_copy_const_aligned_dest_var_aligned_src_const_size_done#], defer[1]
                        ld_field_w_clr[out_last_element, _LAST_ELEMENT_MASK, _BUF0_REG/**/j]
                    #endif
                #else
                    beq[_copy_const_aligned_dest_var_aligned_src_const_size_done#]
                #endif
            #endif
        #endif

        // set start register for source buffer
        #if ( (_SRC_XBUF_TYPE == _XBUF_SRAM_XFER) || (_SRC_XBUF_TYPE == _XBUF_DRAM_XFER) )
        // SRAM or DRAM type
        .begin

            .reg _t_index_val

            br=ctx[0, _copy_var_aligned_src_skip_find_abs_addr#], defer[2]
            add_shf_left(_t_index_val, src_start_byte, (&in_src_xbuf/**/0), 2)

            //if not context 0, need to add offset to absolute reg to T_INDEX value
            _find_absolute_xfer_reg_addr(_t_index_val)

_copy_var_aligned_src_skip_find_abs_addr#:
            local_csr_wr[T_INDEX, _t_index_val]
            nop
            nop
            nop

        .end

        #else
        // LMEM or GPR type
            #error "This case is not supported."
        #endif

        #if ( (_SRC_XBUF_TYPE == _XBUF_SRAM_XFER) || (_SRC_XBUF_TYPE == _XBUF_DRAM_XFER) )
        // SRAM or DRAM type
            #if (_SRC_XBUF_TYPE == _XBUF_SRAM_XFER)
                #define_eval _SRC_POS   *$index
            #elif ( (!defined(__NFP_INDIRECT_REF_FORMAT_NFPXXXX)) && (_DEST_XBUF_TYPE == _XBUF_DRAM_XFER) )
                #define_eval _SRC_POS   *$$index
            #endif

            // now copy _NUM_WORD longwords
            #define_eval i  0
            #while (i < __NUM_WORD)
                // if this is the last LW to copy, take care of the out_last_element
                //  for the case that out_last_element is also the last fully-filled LW.
                //  We need to catch it here before it's too late because:
                //      1. Source offset is run-time (costly to reverse)
                //      2. Cannot get from destination because it may be write-only buffer
                #if ( i == (__NUM_WORD - 1) )
                    #if (!(is_ct_const(out_last_element) || isimport(out_last_element)))
                        #if (__NUM_BYTE == 0)
                            move(out_last_element, _SRC_POS)
                        #endif
                    #endif
                #endif
                move(_BUF0_REG/**/i, _SRC_POS/**/++)
                #define_eval i  (i + 1)
            #endloop

            // copy the remaining bytes for the last longword
            #if (__NUM_BYTE != 0)
                // update out_last_element for non fully-filled LW case
                #if (!(is_ct_const(out_last_element) || isimport(out_last_element)))
                    ld_field_w_clr[out_last_element, _LAST_ELEMENT_MASK, _SRC_POS]
                    move(_BUF0_REG/**/i, out_last_element)
                #else
                    #ifdef _lm_more_than_8th_reg_
                        ld_field_w_clr[_lm_tmp_, _LAST_ELEMENT_MASK, _SRC_POS]
                        move(_BUF0_REG/**/i,, _lm_tmp_)
                    #else
                        ld_field_w_clr[_BUF0_REG/**/i, _LAST_ELEMENT_MASK, _SRC_POS]
                    #endif
                #endif
            #endif

        #else
        // LMEM or GPR type
            #error "This case is not supported."
        #endif

_copy_const_aligned_dest_var_aligned_src_const_size_done#:

        // clean up name spaces
        #ifdef i
            #undef i
        #endif

        #ifdef j
            #undef j
        #endif

        #ifdef _LAST_ELEMENT_MASK
            #undef _LAST_ELEMENT_MASK
        #endif

        #ifdef _lm_more_than_8th_reg_
            #undef _lm_more_than_8th_reg_

            .end
        #endif

        #undef _SRC_XBUF_TYPE
        #undef _DEST_XBUF_TYPE
        #undef _NUM_WORD
        #undef _NUM_BYTE

    #endif  // (NUMBER_OF_BYTES > 0)
#endm
/// @endcond


/// @cond INTERNAL_MACROS
/** An internal macro used by xbuf main macros.
 *
 * @instruction_cnt ~15
 */
#macro _copy_var_aligned_dest_const_aligned_src_var_size(out_dest_xbuf, out_last_element, \
                dest_start_byte, in_src_xbuf, SRC_START_BYTE, in_prepend, number_of_bytes)
.begin

    .reg _num_word _num_byte _tmp

    #if ( !(is_ct_const(SRC_START_BYTE)  || isimport(SRC_START_BYTE)))
        #error 2 "_copy_var_aligned_dest_const_aligned_src_var_size: SRC_START_BYTE must be constant."
    #endif

    #ifdef _SRC_XBUF_TYPE
        #warning "_SRC_XBUF_TYPE is being redefined."
    #endif

    #ifdef _DEST_XBUF_TYPE
        #warning "_DEST_XBUF_TYPE is being redefined."
    #endif

    // get xbuf type for both source and destination buffers
    _GET_XBUF_TYPE(in_src_xbuf)
    #define_eval _SRC_XBUF_TYPE    _XBUF_TYPE
    #undef _XBUF_TYPE

    _GET_XBUF_TYPE(out_dest_xbuf)
    #define_eval _DEST_XBUF_TYPE   _XBUF_TYPE
    #undef _XBUF_TYPE

    #if ( (_SRC_XBUF_TYPE != _XBUF_LMEM) || \
        ((_DEST_XBUF_TYPE != _XBUF_SRAM_XFER) && (_DEST_XBUF_TYPE != _XBUF_DRAM_XFER)) )
        #error "_copy_var_aligned_dest_const_aligned_src_var_size: SRC must be LMEM, DST must XFER types."
    #endif

    #if ( streq(in_src_xbuf, out_dest_xbuf) )
    // ... then their xbuf_type must be the same.
    // If it's GPR or LMEM type, and if same start byte, do not copy.
        #if (_SRC_XBUF_TYPE == _XBUF_LMEM)
            alu[--, dest_start_byte, -, SRC_START_BYTE]
            beq[_copy_var_aligned_dest_const_aligned_src_var_size_done#]
        #endif
    #endif

    // set start register for source buffer
    #if (_SRC_XBUF_TYPE == _XBUF_LMEM)
    .begin

        .reg _lm_curr_base_addr

        #ifdef _INDEX
            #warning "_INDEX is being redefined."
        #endif

        _DEFINE_THIS_XBUF(in_src_xbuf)
        #define_eval _INDEX    _this_xbuf_index

        // save current lmem base address
        local_csr_rd[active_lm_addr_/**/_INDEX]
        immed[_lm_curr_base_addr, 0]

        // calculate new lmem base address from source offset
        alu[_tmp, _lm_curr_base_addr, +, SRC_START_BYTE]

        // set new lmem base address
        local_csr_wr[active_lm_addr_/**/_INDEX, _tmp]
        // the 3 nops are filled by subsequent codes

        #define_eval _SRC_POS   *l$index/**/_INDEX

    #endif

    // set start register for destination buffer
    #if ( (_DEST_XBUF_TYPE == _XBUF_SRAM_XFER) || (_DEST_XBUF_TYPE == _XBUF_DRAM_XFER) )
    // SRAM or DRAM type
    .begin

        .reg _t_index_val

        br=ctx[0, _copy_var_aligned_dest_var_size_skip_find_abs_addr#], defer[2]
        add_shf_left(_t_index_val, dest_start_byte, (&out_dest_xbuf/**/0), 2)
        //if not context 0, need to add offset to absolute reg to T_INDEX value
        _find_absolute_xfer_reg_addr(_t_index_val)

_copy_var_aligned_dest_var_size_skip_find_abs_addr#:

        local_csr_wr[T_INDEX, _t_index_val]
        // the 3 nops are filled by subsequent codes

        #if (_DEST_XBUF_TYPE == _XBUF_SRAM_XFER)
            #define_eval _DST_POS   *$index
        #elif ( (!defined(__NFP_INDIRECT_REF_FORMAT_NFPXXXX)) && (_DEST_XBUF_TYPE == _XBUF_DRAM_XFER) )
            #define_eval _DST_POS   *$$index
        #endif

    .end
    #endif

    // feed instructions to fill 3 nop slots from local_csr_wr
    alu_shf[_num_word, --, B, number_of_bytes, >>2] // _num_word = 0, 1, ..., 16
    ble[_copy_var_aligned_dest_const_aligned_src_var_size_loop_done#]
    // stop short of 1 LW to take care of out_last_element
    alu[_num_word, _num_word, -, 1]
    beq[_copy_var_aligned_dest_const_aligned_src_var_size_last_LW#]

    // Enrolling this .while loop for efficiency
    // .while (_num_word > 0)
    //      perform copies
    //      alu[_num_word, _num_word, -, 1]
    // .endw
_copy_var_aligned_dest_const_aligned_src_var_size_loop_start#:

    move(_DST_POS/**/++, _SRC_POS/**/++)
    alu[_num_word, _num_word, -, 1]
    bgt[_copy_var_aligned_dest_const_aligned_src_var_size_loop_start#]

_copy_var_aligned_dest_const_aligned_src_var_size_last_LW#:

    // copy the last LW
    #if (!(is_ct_const(out_last_element) || isimport(out_last_element)))
        move(out_last_element, _SRC_POS/**/++)
        move(_DST_POS/**/++, out_last_element)
    #else
        move(_DST_POS/**/++, _SRC_POS/**/++)
    #endif

_copy_var_aligned_dest_const_aligned_src_var_size_loop_done#:

    // copy the remaining bytes (if any)
    alu[_num_byte, number_of_bytes, AND, 0x3]       // _num_byte = 0, 1, 2 or 3
    bne[_copy_var_aligned_dest_const_aligned_src_var_size_last_bytes#], defer[1]
    local_csr_wr[active_lm_addr_/**/_INDEX, _lm_curr_base_addr]

    .end    // end of _lm_curr_base_addr
    _UNDEFINE_THIS_XBUF()

    br[_copy_var_aligned_dest_const_aligned_src_var_size_done#], defer[1]
    nop

_copy_var_aligned_dest_const_aligned_src_var_size_last_bytes#:
    alu[_num_word, --, B, _SRC_POS]

    sub_shf_left(_tmp, 32, _num_byte, 3)            // _tmp = 32 - 8*num_byte
    alu[--, _tmp, OR, 0]
    alu_shf[_num_word, --, B, _num_word, >>indirect]

    alu[--, _tmp, OR, 0]
    #if (!(is_ct_const(out_last_element) || isimport(out_last_element)))
        alu[out_last_element, --, B, _num_word, <<indirect]
        alu[_DST_POS, --, B, out_last_element]
    #else
        alu_shf[_DST_POS, --, B, _num_word, <<indirect]
    #endif

_copy_var_aligned_dest_const_aligned_src_var_size_done#:

    // clean up name spaces
    #undef _SRC_XBUF_TYPE
    #undef _DEST_XBUF_TYPE
    #undef _SRC_POS
    #undef _DST_POS

    #ifdef _INDEX
        #undef _INDEX
    #endif

.end
#endm
/// @endcond


/// @cond INTERNAL_MACROS
/** Find absolute xfer register address for the current thread.
 *
 * The user only needs to call this macro if the current thread is NOT thread 0.
 *
 * @param io_reg_abs_addr GPR address which contain the relative address of
 *                        the register for which absolute adrress is needed.
 *                        (this is the same as the absolute register address in
 *                        thread 0).
 *                        The offset from the current thread to thread 0 is
 *                        ADDED to the content of this register.
 *                        The result is the absolute address of the register
 *                        which in-thread-relative address was the original
 *                        content of out_reg_abs_addr.
 */
#macro _find_absolute_xfer_reg_addr(io_reg_abs_addr)
.begin

    .reg active_ctx

    //read current ctx number (bits 0..2)
    local_csr_rd[active_ctx_sts]
    immed[active_ctx, 0]

    //keep only ctx bits
    alu_shf[active_ctx, active_ctx, AND, 0x7]

    #if (defined(__NFP_INDIRECT_REF_FORMAT_NFPXXXX))
        //shift left 5 for 32 regs in each context, shift left 2 for location of register
        //index in T_INDEX.
        //can safely do OR since, in 4ctx mode the maximum relative address in
        //io_reg_abs_addr is 64 and the LSB of active_ctx is always 0
        alu_shf[io_reg_abs_addr, io_reg_abs_addr, OR, active_ctx, <<7]
    #else
        //shift left 4 for 16 regs in each context, shift left 2 for location of register
        //index in T_INDEX.
        //can safely do OR since, in 4ctx mode the maximum relative address in
        //io_reg_abs_addr is 64 and the LSB of active_ctx is always 0
        alu_shf[io_reg_abs_addr, io_reg_abs_addr, OR, active_ctx, <<6]
    #endif

.end
#endm
/// @endcond


/** @}
 * @}
 */

#endif /* __XBUF_UC__ */
