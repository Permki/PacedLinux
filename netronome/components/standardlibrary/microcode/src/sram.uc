/*
 * Copyright (C) 2008-2011 Netronome Systems, Inc.  All rights reserved.
 */

#ifndef __SRAM_UC__
#define __SRAM_UC__

#ifndef NFP_LIB_ANY_NFAS_VERSION
    #if (!defined(__NFP_TOOL_NFAS) ||                       \
        (__NFP_TOOL_NFAS < NFP_SW_VERSION(5, 0, 0, 0)) ||   \
        (__NFP_TOOL_NFAS > NFP_SW_VERSION(6, 1, 0, 255)))
        #error "This standard library is not supported for the version of the SDK currently in use."
    #endif
#endif

#include <constants.uc>
#include <stdmac.uc>
#include <xbuf.uc>
#include <util.uc>

/** @file sram.uc SRAM Operation Macros
 * @addtogroup sram SRAM Operation
 * @{
 *
 * @name SRAM Operation Macros
 * @{
 * Operations macros specific for SRAM memory (deprecated)
 */

// API Quick Reference:
//
// sram_bits_clr(in_mask, in_sram_addr, in_addr_offset, REQ_SIG, in_wakeup_sigs, Q_OPTION)
// sram_bits_set(in_mask, in_sram_addr, in_addr_offset, REQ_SIG, in_wakeup_sigs, Q_OPTION)
// sram_bits_test_and_clr(out_data, in_mask, in_sram_addr, in_addr_offset, REQ_SIG, in_wakeup_sigs, Q_OPTION)
// sram_bits_test_and_set(out_data, in_mask, in_sram_addr, in_addr_offset, REQ_SIG, in_wakeup_sigs, Q_OPTION)
// sram_read(out_data, in_sram_addr, in_addr_offset, in_lw_count, REQ_SIG, in_wakeup_sigs, Q_OPTION)
// sram_write(in_data, in_sram_addr, in_addr_offset, in_lw_count, REQ_SIG, in_wakeup_sigs, Q_OPTION)
// sram_add(in_data, in_sram_addr, in_addr_offset, REQ_SIG, in_wakeup_sigs, Q_OPTION)
// sram_decr (in_sram_addr, in_addr_offset)
// sram_incr (in_sram_addr, in_addr_offset)
// sram_sub (in_data, in_sram_addr, in_addr_offset, REQ_SIG, in_wakeup_sigs, Q_OPTION)
// sram_swap (out_data, in_data, in_sram_addr, in_addr_offset, REQ_SIG, in_wakeup_sigs, Q_OPTION)
// sram_test_and_add (out_data, in_data, in_sram_addr, in_addr_offset, REQ_SIG, in_wakeup_sigs, Q_OPTION)
// sram_test_and_decr (out_data, in_sram_addr, in_addr_offset, REQ_SIG, in_wakeup_sigs, Q_OPTION)
// sram_test_and_incr (out_data, in_sram_addr, in_addr_offset, REQ_SIG, in_wakeup_sigs, Q_OPTION)


////////////////////////// SRAM Multi-Word Operations //////////////////////////////////

// multi-word operations have the argument in_lw_count


/** Read from sram starting at address of first longword.
 *
 * @param out_data          First sram xfer reg of sequence to read to, array notation
 *                          must be in xbuf array notation, index range 0-15
 * @param in_sram_addr      Register or constant base longword address
 * @param in_addr_offset    Register or constant longword address offset
 * @param in_lw_count       Register or constant longword count
 * @param REQ_SIG           Signal associated with this request
 * @param in_wakeup_sigs    Signal or signals to wake up on
 * @param Q_OPTION          Queue option
 *                          @arg no_option or ___ : default. Order queue
 *                          @arg optimize_mem: mem controller selects cycle to issue
 *                          @arg priority: high priority
 *
 * Temp register usage: 0-2, uses registers if constant addr args > MAX_IMMEDIATE, or register length
 *
 * @instruction_cnt 1-5 (1 read SRAM access)
 *
 * @b Example:
 * @code
 * sram_read($packet[2], addr, 0, LWCOUNT3, SIG_SRAM. SIG_SRAM, ___)
 * @endcode
 *
 */
#macro sram_read(out_data, in_sram_addr, in_addr_offset, in_lw_count, REQ_SIG, in_wakeup_sigs, Q_OPTION)
    _sram_op(read, out_data, in_sram_addr, in_addr_offset, in_lw_count, REQ_SIG, in_wakeup_sigs, Q_OPTION)
#endm


/** Write to sram.
 *
 * @param in_data           First sram xfer register of sequence to write from, array notation
 *                          must be in xbuf array notation, index range 0-15
 * @param in_sram_addr      Register or constant base longword address
 * @param in_addr_offset    Register or constant longword address offset
 * @param in_lw_count       Register or constant longword count
 * @param REQ_SIG           Signal associated with this request
 * @param in_wakeup_sigs    Signal or signals to wake up on
 * @param Q_OPTION          Queue option
 *                          @arg no_option or ___ : default. Order queue
 *                          @arg optimize_mem: mem controller selects cycle to issue
 *                          @arg priority: high priority
 *
 * Temp register usage: 0-2, uses registers if constant addr args > MAX_IMMEDIATE, or register or length
 *
 * @instruction_cnt 1-5 (1 write SRAM access)
 *
 * @b Example:
 * @code
 * sram_write($packet[2], addr, 0, LWCOUNT3, SIG_SRAM. SIG_SRAM, ___)
 * @endcode
 *
 */
#macro sram_write(in_data, in_sram_addr, in_addr_offset, in_lw_count, REQ_SIG, in_wakeup_sigs, Q_OPTION)
    _sram_op(write, in_data, in_sram_addr, in_addr_offset, in_lw_count, REQ_SIG, in_wakeup_sigs, Q_OPTION)
#endm


/// @cond INTERNAL_MACROS
/** sram operation where addresses are acceptable as is to Assembler.
 * Support macros sram_read, sram_read_lock, sram_write, sram_write_unlock.
 */
#macro _sram_op(operation, in_data, in_sram_addr, in_addr_offset, in_lw_count, REQ_SIG, in_wakeup_sigs, Q_OPTION)
    #if (!streq(in_data, no_xfer))
        xbuf_find(in_data)      // if found, #define _xbuf_found, _cur_xbuf_name, _cur_xbuf_size
        #if (is_ct_const (in_lw_count) || isimport(in_lw_count))
            #ifdef _xbuf_found
                #define _perform_xbuf_op
            #endif
            #ifdef _act_wd_count
                #warning "_act_wd_count is being redefined."
            #endif
            // if all word counts are n-1
            #if (MEM_WD_COUNT_MIN_1 == TRUE)
                #define_eval _act_wd_count (in_lw_count + 1)
            #else
                #define_eval _act_wd_count (in_lw_count)
            #endif
        #endif
    #endif
    #ifdef _perform_xbuf_op
        _sram_op_xbufs(operation, in_sram_addr, in_addr_offset, _act_wd_count, REQ_SIG, in_wakeup_sigs, Q_OPTION)
        #undef _perform_xbuf_op
    #elif (is_ct_const (in_lw_count) || isimport(in_lw_count))
        _sram_const_instr(operation, in_data, in_sram_addr, in_addr_offset, _act_wd_count, REQ_SIG, in_wakeup_sigs, Q_OPTION)
    #else
        _sram_ind_instr(operation, in_data, in_sram_addr, in_addr_offset, in_lw_count, REQ_SIG, in_wakeup_sigs, Q_OPTION)
    #endif  // (is_ct_const (in_lw_count))

    #ifdef _act_wd_count
        #undef _act_wd_count
    #endif
#endm
/// @endcond


/// @cond INTERNAL_MACROS
/** Perform sram operation with one or more(linked) xbufs.
 * Support macros sram_read, sram_read_lock, sram_write, sram_write_unlock.
 */
#macro _sram_op_xbufs(operation, in_sram_addr, in_addr_offset, in_lw_count, REQ_SIG, in_wakeup_sigs, Q_OPTION)
    #ifdef _rem_buf
        #warning "_rem_buf is being redefined."
    #endif
    // xbuf_find in caller previously set these while determining whether to use xbufs
    #define_eval _rem_buf (_cur_xbuf_size - _cur_xbuf_index)

    #ifdef _cur_lw_count
        #warning "_cur_lw_count is being redefined."
    #endif
    // to enter this macro, in_lw_count had to be a constant
    #define_eval _cur_lw_count (in_lw_count)
    #if (_cur_lw_count > _rem_buf)
        #define _use_new_base
    #endif

    #ifdef _cur_lw_offset
        #warning "_cur_lw_offset is being redefined."
    #endif

    // determine longword offset
    #if (is_ct_const(in_addr_offset) || isimport(in_addr_offset))

            #define_eval _cur_lw_offset (in_addr_offset >>2)        // convert from byte address
    #else
        #ifndef _use_new_base
            #define _use_new_base
        #endif
    #endif

    #ifdef _use_new_base
        .begin // new_base_addr

            .reg new_base_addr
            alu_op(new_base_addr, in_sram_addr, +, in_addr_offset)
            #define_eval _cur_lw_offset (0)
    #endif

    #ifdef _cur_offset
        #warning "_cur_offset is being redefined."
    #endif
    #while (_cur_lw_count > _rem_buf)
        #ifdef _SECOND_SIG
            #error "maximum of 2 linked xbufs supported for a single memory operation"
        #endif
        .sig tmp_signalp
        #define_eval _cur_offset (_cur_lw_offset <<2)       // convert back to byte address

        _sram_const_instr(operation, _cur_xbuf_name/**/_cur_xbuf_index, new_base_addr, _cur_offset, _rem_buf, tmp_signalp, SIG_NONE, ___)

        #define _SECOND_SIG
        #define_eval _cur_lw_offset (_cur_lw_offset + _rem_buf)
        #define_eval _cur_lw_count (_cur_lw_count - _rem_buf)

        #ifdef _cur_xbuf_next
            xbuf_param_set(_cur_xbuf_next)
            #define_eval _cur_xbuf_index 0
            #define_eval _rem_buf _cur_xbuf_size
        #else
            #error  "_sram_op_xbufs. out of xbuf range"
        #endif  //  _cur_xbuf_next

    #endloop

    // perform last operation
    #define_eval _cur_offset (_cur_lw_offset <<2)       // convert back to byte address


    #ifdef _use_new_base
            #ifdef _SECOND_SIG
                _sram_const_instr(operation, _cur_xbuf_name/**/_cur_xbuf_index, new_base_addr, _cur_offset, _cur_lw_count, REQ_SIG, signals(tmp_signalp, in_wakeup_sigs), Q_OPTION)
            #else
                _sram_const_instr(operation, _cur_xbuf_name/**/_cur_xbuf_index, new_base_addr, _cur_offset, _cur_lw_count, REQ_SIG, in_wakeup_sigs, Q_OPTION)
            #endif
            #undef _use_new_base

        .end    // new_base_addr
    #else
        #ifdef _SECOND_SIG
            _sram_const_instr(operation, _cur_xbuf_name/**/_cur_xbuf_index, in_sram_addr, _cur_offset, _cur_lw_count, REQ_SIG, signals(tmp_signalp, in_wakeup_sigs), Q_OPTION)
        #else
            _sram_const_instr(operation, _cur_xbuf_name/**/_cur_xbuf_index, in_sram_addr, _cur_offset, _cur_lw_count, REQ_SIG, in_wakeup_sigs, Q_OPTION)
        #endif
    #endif

    #ifdef _cur_lw_offset
        #undef _cur_lw_offset   // clean up name space
    #endif
    #undef _cur_offset          // clean up name space
    #undef _rem_buf             // clean up name space
    #undef _cur_lw_count        // clean up name space
#endm
/// @endcond


////////////////////////////////// SRAM Single Word Operations ///////////////////////////////////////

// single-word operations do not have the argument in_lw_count

/** Clear in_mask bits at sram longword location.
 *
 * @param in_mask            Register or constant, mask of bits to set
 * @param in_sram_addr       Register or constant base longword address
 * @param in_addr_offset     Register or constant longword address offset
 * @param REQ_SIG            Signal associated with this request
 * @param in_wakeup_sigs     Signal or signals to wake up on
 * @param Q_OPTION           Queue option
 *                           @arg no_option or ___ : default. Order queue
 *                           @arg optimize_mem: mem controller selects cycle to issue
 *                           @arg priority: high priority
 *
 * @instruction_cnt 1-8 (1 sram read-modify-write memory access)
 */
#macro sram_bits_clr(in_mask, in_sram_addr, in_addr_offset, REQ_SIG, in_wakeup_sigs, Q_OPTION)
    _DEF_STATUS_IS_SRAM_XFER(in_mask)      // define _STATUS, test whether in_mask is already a transfer reg
    #if (_STATUS == FALSE)
    .begin

        xbuf_alloc($bset_vector, 1, read_write)
        move($bset_vector[0], in_mask)
            _sram_instr(clr, $bset_vector[0], in_sram_addr, in_addr_offset, no_wd_count, REQ_SIG, in_wakeup_sigs, Q_OPTION)

        xbuf_free($bset_vector)

    .end
    #else
        _sram_instr(clr, in_mask, in_sram_addr, in_addr_offset, no_wd_count, REQ_SIG, in_wakeup_sigs, Q_OPTION)
    #endif
#endm


/** Set in_mask bits at sram longword location.
 *
 * @param in_mask            Register or constant, mask of bits to set
 * @param in_sram_addr       Register or constant base longword address
 * @param in_addr_offset     Register or constant longword address offset
 * @param REQ_SIG            Signal associated with this request
 * @param in_wakeup_sigs     Signal or signals to wake up on
 * @param Q_OPTION           Queue option
 *                           @arg no_option or ___ : default. Order queue
 *                           @arg optimize_mem: mem controller selects cycle to issue
 *                           @arg priority: high priority
 *
 * @instruction_cnt 1-8 (1 sram read-modify-write memory access)
 *
 * @b Example:
 * @code
 * sram_bits_set(0x111, 0, bit_position, SIG_SRAM. SIG_SRAM, ___)
 * @endcode
 */
#macro sram_bits_set(in_mask, in_sram_addr, in_addr_offset, REQ_SIG, in_wakeup_sigs, Q_OPTION)
    _DEF_STATUS_IS_SRAM_XFER(in_mask)      // define _STATUS, test whether in_mask is already a transfer reg
    #if (_STATUS == FALSE)
        .begin

            xbuf_alloc($bset_vector, 1, read_write)
            move($bset_vector[0], in_mask)

            _sram_instr(set, $bset_vector[0], in_sram_addr, in_addr_offset, no_wd_count, REQ_SIG, in_wakeup_sigs, Q_OPTION)

            xbuf_free($bset_vector)

        .end
    #else
        _sram_instr(set, in_mask, in_sram_addr, in_addr_offset, no_wd_count, REQ_SIG, in_wakeup_sigs, Q_OPTION)
    #endif
#endm


/** Clear in_mask bits at sram longword location.
 *
 * Read contents of sram address prior to the write.
 *
 * @param out_data           Read xfer register result
 * @param in_mask            Register or constant, mask of bits to set
 * @param in_sram_addr       Register or constant base longword address
 * @param in_addr_offset     Register or constant longword address offset
 * @param REQ_SIG            Signal associated with this request
 * @param in_wakeup_sigs     Signal or signals to wake up on
 * @param Q_OPTION           Queue option
 *                           @arg no_option or ___ : default. Order queue
 *                           @arg optimize_mem: mem controller selects cycle to issue
 *                           @arg priority: high priority
 *
 * @instruction_cnt 1-8 (1 sram read-modify-write memory access)
 */
#macro sram_bits_test_and_clr(out_data, in_mask, in_sram_addr, in_addr_offset, REQ_SIG, in_wakeup_sigs, Q_OPTION)
    #if (!streq(out_data, in_mask))
        move(out_data, in_mask)
    #endif
    _sram_instr(test_and_clr, out_data, in_sram_addr, in_addr_offset, dual_sig_op, REQ_SIG, in_wakeup_sigs, Q_OPTION)
#endm


/** Set in_mask bits at sram longword location.
 *
 * Read contents of sram address prior to the write.
 *
 * @param out_data           Read xfer register result
 * @param in_mask            Register or constant, mask of bits to set
 * @param in_sram_addr       Register or constant base longword address
 * @param in_addr_offset     Register or constant longword address offset
 * @param REQ_SIG            Signal associated with this request
 * @param in_wakeup_sigs     Signal or signals to wake up on
 * @param Q_OPTION           Queue option
 *                           @arg no_option or ___ : default. Order queue
 *                           @arg optimize_mem: mem controller selects cycle to issue
 *                           @arg priority: high priority
 *
 * @instruction_cnt 1-8 (1 sram read-modify-write memory access)
 *
 * @b Example:
 * @code
 * sram_bits_test_and_set(prev_value, 0x1000, addr0, addr1, SIG_SRAM. SIG_SRAM, ___) // test/set bit 3
 * @endcode
 */
#macro sram_bits_test_and_set(out_data, in_mask, in_sram_addr, in_addr_offset, REQ_SIG, in_wakeup_sigs, Q_OPTION)
    #if (!streq(out_data, in_mask))
        move(out_data, in_mask)
    #endif
    _sram_instr(test_and_set, out_data, in_sram_addr, in_addr_offset, dual_sig_op, REQ_SIG, in_wakeup_sigs, Q_OPTION)
#endm


/// @cond INTERNAL_MACROS
/** Support bit_wr operations.
 */
#macro _sram_bits_op(operation, io_data, in_sram_addr, in_addr_offset, REQ_SIG, in_wakeup_sigs, Q_OPTION)
    _sram_instr(operation, io_data, in_sram_addr, in_addr_offset, no_wd_count, REQ_SIG, in_wakeup_sigs, Q_OPTION)
#endm
/// @endcond



/** Add in_data to sram location.
 *
 * @param in_data            Register or constant, data to add
 * @param in_sram_addr       Register or constant base longword address
 * @param in_addr_offset     Register or constant longword address offset
 * @param REQ_SIG            Signal associated with this request
 * @param in_wakeup_sigs     Signal or signals to wake up on
 * @param Q_OPTION           Queue option
 *                           @arg no_option or ___ : default. Order queue
 *                           @arg optimize_mem: mem controller selects cycle to issue
 *                           @arg priority: high priority
 *
 * @instruction_cnt 1-5 (1 SRAM read-modify-write access)
 */
#macro sram_add(in_data, in_sram_addr, in_addr_offset, REQ_SIG, in_wakeup_sigs, Q_OPTION)
    _sram_instr(add, in_data, in_sram_addr, in_addr_offset, no_wd_count, REQ_SIG, in_wakeup_sigs, Q_OPTION)
#endm


/** Decrement 32-bit longword at sram location.
 *
 * @param in_sram_addr       Register or constant base longword address
 * @param in_addr_offset     Register or constant longword address offset
 *
 * @instruction_cnt 1-5 (1 SRAM read-modify-write access)
 */
#macro sram_decr(in_sram_addr, in_addr_offset)
    _sram_instr(decr, no_xfer, in_sram_addr, in_addr_offset, no_wd_count, ___, ___, ___)
#endm


/** Increment 32-bit longword at to sram location.
 *
 * @param in_sram_addr       Register or constant base longword address
 * @param in_addr_offset     Register or constant longword address offset
 *
 * @instruction_cnt 1-5 (1 SRAM read-modify-write access)
 */
#macro sram_incr(in_sram_addr, in_addr_offset)
    _sram_instr(incr, no_xfer, in_sram_addr, in_addr_offset, no_wd_count, ___, ___, ___)
#endm


/** Sub in_data from sram location.
 *
 * @param in_data            Register or constant, data to subtract
 * @param in_sram_addr       Register or constant base longword address
 * @param in_addr_offset     Register or constant longword address offset
 * @param REQ_SIG            Signal associated with this request
 * @param in_wakeup_sigs     Signal or signals to wake up on
 * @param Q_OPTION           Queue option
 *                           @arg no_option or ___ : default. Order queue
 *                           @arg optimize_mem: mem controller selects cycle to issue
 *                           @arg priority: high priority
 *
 * @note sub instr is not supported in HW, so we subtract from 0 and add that number.
 *       in_data must not be a write transfer register.
 *
 * @instruction_cnt 2-6 (1 SRAM read-modify-write access)
 */
#macro sram_sub(in_data, in_sram_addr, in_addr_offset, REQ_SIG, in_wakeup_sigs, Q_OPTION)
.begin

    xbuf_alloc($sub_xfer, 1, read_write)
    alu_op($sub_xfer[0], 0, -, in_data)
    _sram_instr(add, $sub_xfer[0], in_sram_addr, in_addr_offset, no_wd_count, REQ_SIG, in_wakeup_sigs, Q_OPTION)
    xbuf_free($sub_xfer)

.end
#endm


/** Write in_data to sram location.
 *
 * Read contents of sram location prior to the operation to @p out_data.
 *
 * @param out_data           Transfer Register, returned previous data value
 * @param in_data            Register or constant, data to write
 * @param in_sram_addr       Register or constant base longword address
 * @param in_addr_offset     Register or constant longword address offset
 * @param REQ_SIG            Signal associated with this request
 * @param in_wakeup_sigs     Signal or signals to wake up on
 * @param Q_OPTION           Queue option
 *                           @arg no_option or ___ : default. Order queue
 *                           @arg optimize_mem: mem controller selects cycle to issue
 *                           @arg priority: high priority
 *
 * @note @p out_data must be a transfer register.
 *
 * @instruction_cnt 1-8 (1 SRAM read-modify-write access)
 *
 * @b Example:
 * code
 * sram_swap(prev_value, new_value, addr0, addr1, SIG_SRAM. SIG_SRAM, ___) // test/set bit 3
 * @endcode
 */
#macro sram_swap(out_data, in_data, in_sram_addr, in_addr_offset, REQ_SIG, in_wakeup_sigs, Q_OPTION)
    #if (!streq(out_data, in_data))
        move(out_data, in_data)
    #endif
    _sram_instr(swap, out_data, in_sram_addr, in_addr_offset, dual_sig_op, REQ_SIG, in_wakeup_sigs, Q_OPTION)
#endm


/** Read contents of sram location to @p out_data.
 *
 * Then add in_data to sram location contents.
 *
 * @param out_data           Transfer Register, returned previous data value
 * @param in_data            Register or constant, data to add
 * @param in_sram_addr       Register or constant base longword address
 * @param in_addr_offset     Register or constant longword address offset
 * @param REQ_SIG            Signal associated with this request
 * @param in_wakeup_sigs     Signal or signals to wake up on
 * @param Q_OPTION           Queue option
 *                           @arg no_option or ___ : default. Order queue
 *                           @arg optimize_mem: mem controller selects cycle to issue
 *                           @arg priority: high priority
 *
 * @note @p out_data must be a transfer register.
 *
 * @b Example:
 * @code
 * sram_test_and_add(prev_value, addend, addr0, addr1, SIG_SRAM. SIG_SRAM, ___) // test/set bit 3
 * @endcode
 *
 * @instruction_cnt 1-8 (1 SRAM read-modify-write access)
 */
#macro sram_test_and_add(out_data, in_data, in_sram_addr, in_addr_offset, REQ_SIG, in_wakeup_sigs, Q_OPTION)
    #if (!streq(out_data, in_data))
        move(out_data, in_data)
    #endif
    _sram_instr(test_and_add, out_data, in_sram_addr, in_addr_offset, dual_sig_op, REQ_SIG, in_wakeup_sigs, Q_OPTION)
#endm


/** Read contents of sram location to @p out_data.
 *
 * Then decrement sram location contents.
 *
 * @param out_data           Transfer Register, returned previous data value
 * @param in_sram_addr       Register or constant base longword address
 * @param in_addr_offset     Register or constant longword address offset
 * @param REQ_SIG            Signal associated with this request
 * @param in_wakeup_sigs     Signal or signals to wake up on
 * @param Q_OPTION           Queue option
 *                           @arg no_option or ___ : default. Order queue
 *                           @arg optimize_mem: mem controller selects cycle to issue
 *                           @arg priority: high priority
 *
 * @note @p out_data must be a transfer register.
 *
 * @instruction_cnt 1-5 (1 SRAM read-modify-write access)
 *
 * @b Example:
 * @code
 * sram_test_and_decr(prev_value, addr0, addr1, SIG_SRAM. SIG_SRAM, ___) // test/set bit 3
 * @endcode
 */
#macro sram_test_and_decr(out_data, in_sram_addr, in_addr_offset, REQ_SIG, in_wakeup_sigs, Q_OPTION)
    _sram_instr(test_and_decr, out_data, in_sram_addr, in_addr_offset, dual_sig_op, REQ_SIG, in_wakeup_sigs, Q_OPTION)
#endm


/** Read contents of sram location to @p out_data.
 *
 * Then increment sram location contents.
 *
 * @param out_data           Transfer Register, returned previous data value
 * @param in_sram_addr       Register or constant base longword address
 * @param in_addr_offset     Register or constant longword address offset
 * @param REQ_SIG            Signal associated with this request
 * @param in_wakeup_sigs     Signal or signals to wake up on
 * @param Q_OPTION           Queue option
 *                           @arg no_option or ___ : default. Order queue
 *                           @arg optimize_mem: mem controller selects cycle to issue
 *                           @arg priority: high priority
 *
 * @note @p out_data must be a transfer register.
 *
 * @instruction_cnt 1-5 (1 SRAM read-modify-write access)
 *
 * @b Example:
 * @code
 * sram_test_and_incr(prev_value, addr0, addr1, SIG_SRAM. SIG_SRAM, ___) // test/set bit 3
 * @endcode
 */
#macro sram_test_and_incr(out_data, in_sram_addr, in_addr_offset, REQ_SIG, in_wakeup_sigs, Q_OPTION)
    _sram_instr(test_and_incr, out_data, in_sram_addr, in_addr_offset, dual_sig_op, REQ_SIG, in_wakeup_sigs, Q_OPTION)
#endm



/// @cond INTERNAL_MACROS
////////////////////////// Lowest Level SRAM support macros //////////////////////////////////
/** Instantiate the sram instruction.
 * Convert REQ_SIG, in_wakeup_sigs, Q_OPTION to the correct optional tokens.
 * no_xfer may have been inserted by macros such as sram_incr, where xfer is not used.
 *
 * Arguments: See SRAM Single Word Operations common Output/Inputs.
 */
#macro _sram_instr(op_code, io_xfer, in_sram_addr, in_addr_offset, in_wd_count, REQ_SIG, in_wakeup_sigs, Q_OPTION)
    _DEF_MEM_TOKENS(REQ_SIG, in_wakeup_sigs, Q_OPTION, in_wd_count)

    _GET_ADDRESSES(_MEM_ADDR0, _MEM_ADDR1, in_sram_addr, in_addr_offset, SRAM)

    #if (streq(io_xfer, no_xfer))
        #if (streq(in_wd_count, no_wd_count))
            #if (REQ_SIG || in_wakeup_sigs || Q_OPTION)
                sram[op_code, --, _MEM_ADDR0, _MEM_ADDR1], _MEM_TOKENS
            #else
                sram[op_code, --, _MEM_ADDR0, _MEM_ADDR1]
            #endif
        #else
            sram[op_code, --, _MEM_ADDR0, _MEM_ADDR1, in_wd_count], _MEM_TOKENS
        #endif
    #else
        #if ( (streq(in_wd_count, no_wd_count)) || (streq(in_wd_count, dual_sig_op)) )
            sram[op_code, io_xfer, _MEM_ADDR0, _MEM_ADDR1], _MEM_TOKENS
        #else
            sram[op_code, io_xfer, _MEM_ADDR0, _MEM_ADDR1, in_wd_count], _MEM_TOKENS
        #endif
    #endif

    //_ARB_IF_HW_SIGS(ALL)

    _FREE_ADDRESSES()

    #ifdef _HW_SIG_EXPR
        #undef _HW_SIG_EXPR
        ctx_arb[in_wakeup_sigs]
    #endif
#endm
/// @endcond


/// @cond INTERNAL_MACROS
/** Instantiate the sram instruction and insert indirect token.
 * Convert REQ_SIG, in_wakeup_sigs, Q_OPTION to the correct optional tokens.
 * no_xfer may have been inserted by macros such as sram_incr, where xfer is not used.
 *
 * @note This macro is called by read and write macros that have a GPR word count.
 *
 * Arguments: See SRAM Single Word Operations common Output/Inputs.
 */
#macro _sram_ind_instr(op_code, io_xfer, in_sram_addr, in_addr_offset, in_wd_count, REQ_SIG, in_wakeup_sigs, Q_OPTION)
    _DEF_MEM_TOKENS(REQ_SIG, in_wakeup_sigs, Q_OPTION, in_wd_count)

    _GET_ADDRESSES(_MEM_ADDR0, _MEM_ADDR1, in_sram_addr, in_addr_offset, SRAM)

    // 10/11 temp determine max_nn from xbuf size, until Assembler fix is implemented
    // this requires that io_xfer either be 1 xfer or an xbuf
    xbuf_find(io_xfer)      // if found, #define _xbuf_found, _cur_xbuf_name, _cur_xbuf_size
    #ifdef _NN_SIZE
        #warning "_NN_SIZE is being redefined."
    #endif
    #ifdef _MAX_NN
        #warning "_MAX_NN is being redefined."
    #endif

    #ifdef _xbuf_found
        #define_eval _NN_SIZE (_cur_xbuf_size - _cur_xbuf_index)
        #define_eval _MAX_NN max_/**/_NN_SIZE
    #else
        #define_eval _MAX_NN max
    #endif

    .begin

        .reg ref_count
        #if (defined(__NFP_INDIRECT_REF_FORMAT_V2)
            #if (MEM_WD_COUNT_MIN_1 != TRUE)
                // convert to n-1
                alu[ref_count, in_wd_count, -, 1]
                alu[--, 0x80, OR, ref_count, <<8]
            #else
                alu[--, 0x80, OR, ref_count, <<8]
            #endif
        #elif (defined(__NFP_INDIRECT_REF_FORMAT_IXP))
            // if all memory word counts are n
            #if (MEM_WD_COUNT_MIN_1 != TRUE)
                // convert to n-1
                sub(ref_count, in_wd_count, 1)
                shf_left(ref_count, ref_count, _REF_CNT_OFFSET)
            #else
                shf_left(ref_count, in_wd_count, _REF_CNT_OFFSET)
            #endif

            alu_shf_left(ref_count, ref_count, OR, 1, _REF_CNT_OV_OFFSET)
        #else
            #error "This macro is only available in IXP or NFP6000 indirect reference format mode."
        #endif

        #if (streq(io_xfer, no_xfer))
            //value of word count is not important because the real count is in the indirect pad
            sram[op_code, --, _MEM_ADDR0, _MEM_ADDR1, _MAX_NN], indirect_ref, _MEM_TOKENS
        #else
            //value of word count is not important because the real count is in the indirect pad
            sram[op_code, io_xfer, _MEM_ADDR0, _MEM_ADDR1, _MAX_NN], indirect_ref, _MEM_TOKENS
        #endif

        // clean up name space
        #ifdef _NN_SIZE
            #undef _NN_SIZE
        #endif
        #undef _MAX_NN

    .end    //ref_count

    _FREE_ADDRESSES()
    #ifdef _HW_SIG_EXPR
        #undef _HW_SIG_EXPR
        ctx_arb[in_wakeup_sigs]
    #endif
#endm
/// @endcond


/// @cond INTERNAL_MACROS
/** A wrapper macro that calls _sram_instr if in_wd_count is <= 8 or
 * calls _sram_ind_instr with a temporary GPR if in_wd_count is > 8.
 *
 * @note in_wd_count is a constant.
 */
#macro _sram_const_instr(op_code, io_xfer, in_sram_addr, in_addr_offset, in_wd_count, REQ_SIG, in_wakeup_sigs, Q_OPTION)
    #if (in_wd_count > 8)
        .begin

            .reg _wd_count_gpr
            immed32(_wd_count_gpr, in_wd_count)
            _sram_ind_instr(op_code, io_xfer, in_sram_addr, in_addr_offset, _wd_count_gpr, REQ_SIG, in_wakeup_sigs, Q_OPTION)

        .end
    #else
        _sram_instr(op_code, io_xfer, in_sram_addr, in_addr_offset, in_wd_count, REQ_SIG, in_wakeup_sigs, Q_OPTION)
    #endif
#endm
/// @endcond


///////////////////////////////////////////////////////////////////////////////
/** Macro to do sram ring fast journal logging.
 *
 * @param in_journal_reg_orig  Register with data in lower 24 bits, upper 8 bits clear
 * @param ring_num             SRAM journal ring number - must be constant
 */
#macro sram_fast_journal(in_journal_reg_orig, ring_num)
    #if (!is_ct_const(ring_num))
        #error " Ring number must be a constant."
    #endif

    #define_eval _JBASE  (SRAM_JOURNAL_RING_/**/ring_num/**/_BASE)
    #define_eval _CHAN_NUM  ((_JBASE >> 30) & 3)
    .begin

        .reg    in_journal_reg
        //.reg    tmp
        //immed32(tmp, 0x00FFFFFF)
        alu[in_journal_reg, in_journal_reg_orig, and~, 0xFF, <<24]
        alu[in_journal_reg, in_journal_reg, or, _CHAN_NUM, <<30]
        alu[in_journal_reg, in_journal_reg, or, ring_num, <<24]
        sram[fast_journal, --, in_journal_reg, 0]

    .end

    #undef _JBASE
    #undef _CHAN_NUM
#endm

/** Given freelist ID and sram address, dump the current sram queue
 * descriptor state to specified sram address.
 *
 * @param _QID_                    Qdescriptor number to dump
 * @param _DEBUG_QDESC_SRAM_BASE_  SRAM address where q-descriptor will be written
 */
#macro sram_wr_qdesc(_QID_, _DEBUG_QDESC_SRAM_BASE_)
.begin

    .reg sram_qdesc_addr
    .reg tmp
    .reg $qdesc[4]
    .xfer_order $qdesc
    .sig qdesc_sig
    .sig qdesc_dram_sig
    .set_sig qdesc_dram_sig
    .set_sig qdesc_sig

    #if (is_ct_const(_QID_))
        #if (_QID_ > 63)
            #error "SRAM QDescriptor must be in range 0 - 63."
        #endif
    #else
        // max qid = 63
        .if(_QID_ > 63)
            immed[_QID_, 63]
        .endif
    #endif

    #if (is_ct_const(_DEBUG_QDESC_SRAM_BASE_) || isimport(_DEBUG_QDESC_SRAM_BASE_))
        #define_eval S_BASE_LW  ( (_DEBUG_QDESC_SRAM_BASE_ & 0xC0000000) | ((_DEBUG_QDESC_SRAM_BASE_ & 0x3FFFFFFF)>>2) )
        move(sram_qdesc_addr, S_BASE_LW) // Long word address
    #else
        #error "sram_wr_qdesc: SRAM Address must be constant - _DEBUG_QDESC_SRAM_BASE_"
    #endif

    //Freelist ID = bits[29:24] (Refer buf_freelist_create())
    alu_shf[sram_qdesc_addr, sram_qdesc_addr, OR, _QID_, <<24]
    // Load QDesc from SRAM QArray H/W to SRAM memory
    sram[wr_qdesc, --, sram_qdesc_addr, 0 ]

    #undef S_BASE_LW

.end
#endm

/** Initialize SRAM memory with given pattern for 'len' bytes.
 *
 * @param in_sram_addr  SRAM start address (constant or mutable GPR)
 * @param in_len        Number of bytes to initialize (blocks of 32 bytes) (constant or mutable GPR)
 * @param lw_pattern    Pattern to fill (constant)
 * @param _CHUNK_SIZE   Constant, number of bytes to transfer on each sram cmd, mutiple of 4
 */
#macro sram_memset(in_sram_addr, in_len, lw_pattern, _CHUNK_SIZE_)
.begin

    #if (!is_ct_const(_CHUNK_SIZE_))
        #error "sram_memset: Chunk size must be constant."
    #endif

    #if ( (_CHUNK_SIZE_ <= 0) || (_CHUNK_SIZE_ > 32) )
        #error "sram_memset: Chunk Size must be in multiples of 4-bytes ranging 4-32:"[_CHUNK_SIZE_]
    #endif

    #if (_CHUNK_SIZE_ & 0x3)
        #error "sram_memset: Chunk Size must be multiple of 4-bytes:" [_CHUNK_SIZE_]
    #endif

    #define_eval _NUM_XFERS  (_CHUNK_SIZE_ / 4)

    #define_eval _CHUNK_REF_CNT (_CHUNK_SIZE_ / 4)

    .reg $dest_in[_NUM_XFERS]

    .xfer_order $dest_in
    .sig sram_sig

    #if (is_ct_const(in_sram_addr))
        .reg cur_sram_addr
        move(cur_sram_addr, in_sram_addr)
        #define_eval _SRAM_BASE_ADDR_    cur_sram_addr
    #else
        #define_eval _SRAM_BASE_ADDR_    in_sram_addr
    #endif

    #if (is_ct_const(in_len))
        .reg cur_len
        move(cur_len, in_len)
        #define_eval _REM_BYTES_    cur_len
    #else
        #define_eval _REM_BYTES_    in_len
    #endif

    #if (is_ct_const(lw_pattern))
        .reg cur_pattern
        move(cur_pattern, lw_pattern)
        #define_eval _PATTERN_   cur_pattern
    #else
        #define_eval _PATTERN_   lw_pattern
    #endif

    #define_eval    _COUNT  0

    #while  (_COUNT < _NUM_XFERS)
        alu[$dest_in[_COUNT], --, B, _PATTERN_]
        #define_eval    _COUNT  (_COUNT + 1)
    #endloop

    #undef _COUNT

    .while (_REM_BYTES_ > 0)

        //
        // init gs to long word pattern specified
        //
        sram[write, $dest_in[0], _SRAM_BASE_ADDR_, 0, _CHUNK_REF_CNT], sig_done[sram_sig]
        ctx_arb[sram_sig]

        alu[_SRAM_BASE_ADDR_, _SRAM_BASE_ADDR_, +, _CHUNK_SIZE_]
        alu[_REM_BYTES_, _REM_BYTES_, -, _CHUNK_SIZE_]

    .endw

    #undef _SRAM_BASE_ADDR_
    #undef _REM_BYTES_
    #undef _PATTERN_
    #undef _NUM_XFERS
    #undef _CHUNK_REF_CNT

.end
#endm


/** @}
 * @}
 */

#endif /* __SRAM_UC__ */
