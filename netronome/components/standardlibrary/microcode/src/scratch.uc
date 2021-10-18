/*
 * Copyright (C) 2008-2011 Netronome Systems, Inc.  All rights reserved.
 */

#ifndef __SCRATCH_UC__
#define __SCRATCH_UC__

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

/** @file scratch.uc Global Scratch Operation Macros
 * @addtogroup gs Global Scratch Operation
 * @{
 *
 * @name Global Scratch Operation Macros
 * @{
 *
 */

// API Quick Reference:
//
// scratch_bits_clr(in_mask, in_scratch_addr, in_addr_offset, REQ_SIG, in_wakeup_sigs, Q_OPTION)
// scratch_bits_set(in_mask, in_scratch_addr, in_addr_offset, REQ_SIG, in_wakeup_sigs, Q_OPTION)
// scratch_bits_test_and_clr(out_data, in_mask, in_scratch_addr, in_addr_offset, REQ_SIG, in_wakeup_sigs, Q_OPTION)
// scratch_bits_test_and_set(out_data, in_mask, in_scratch_addr, in_addr_offset, REQ_SIG, in_wakeup_sigs, Q_OPTION)
// scratch_incr (in_scratch_addr, in_addr_offset)
// scratch_read(out_data, in_scratch_addr, in_addr_offset, in_lw_count, REQ_SIG, in_wakeup_sigs, Q_OPTION)
// scratch_write(in_data, in_scratch_addr, in_addr_offset, in_lw_count, REQ_SIG, in_wakeup_sigs, Q_OPTION)
// scratch_add(in_data, in_scratch_addr, in_addr_offset, REQ_SIG, in_wakeup_sigs, Q_OPTION)
// scratch_decr (in_scratch_addr, in_addr_offset)
// scratch_sub (in_data, in_scratch_addr, in_addr_offset, REQ_SIG, in_wakeup_sigs, Q_OPTION)
// scratch_swap (out_data, in_data, in_scratch_addr, in_addr_offset, REQ_SIG, in_wakeup_sigs, Q_OPTION)
// scratch_test_and_add (out_data, in_data, in_scratch_addr, in_addr_offset, REQ_SIG, in_wakeup_sigs, Q_OPTION)
// scratch_test_and_decr (out_data, in_scratch_addr, in_addr_offset, REQ_SIG, in_wakeup_sigs, Q_OPTION)
// scratch_test_and_incr (out_data, in_scratch_addr, in_addr_offset, REQ_SIG, in_wakeup_sigs, Q_OPTION)



////////////////////////// SCRATCH Multi-Word Operations //////////////////////////////////

// multi-word operations have the argument in_lw_count


/** Read from scratch starting at address of first longword.
 *
 * @param out_data            First scratch xfer reg of sequence to read to, array notation
 *                            must be in xbuf array notation, index range 0-15.
 * @param in_scratch_addr     Register or constant base longword address
 * @param in_addr_offset      Register or constant longword address offset
 * @param in_lw_count         Register or constant longword count
 * @param REQ_SIG             Signal associated with this request
 * @param in_wakeup_sigs      Signal or signals to wake up on
 * @param Q_OPTION            Queue option (no
 * @param optional_token      @arg no_option or ___: Default. Order queue.
 *                            @arg optimize_mem: Mem controller selects cycle to issue
 *                            @arg priority: High priority
 *
 * Temp register usage: 0-2, uses registers if constant addr args > MAX_IMMEDIATE, or register length.
 *
 * @b Example:
 * @code
 * scratch_read($packet[2], addr, 0, LWCOUNT3, SIG_SCRATCH. SIG_SCRATCH, ___)
 * @endcode
 */
#macro scratch_read(out_data, in_scratch_addr, in_addr_offset, in_lw_count, REQ_SIG, in_wakeup_sigs, Q_OPTION)
    _scratch_op(read, out_data, in_scratch_addr, in_addr_offset, in_lw_count, REQ_SIG, in_wakeup_sigs, Q_OPTION)
#endm


/** Write to scratch.
 *
 * @param in_data             First scratch xfer reg of sequence to write from, array notation
 *                            must be in xbuf array notation, index range 0-15
 * @param in_scratch_addr     Register or constant base longword address
 * @param in_addr_offset      Register or constant longword address offset
 * @param in_lw_count         Register or constant longword count
 * @param in_wakeup_sigs      Signal or signals to wake up on
 * @param Q_OPTION            Queue option (no
 * @param optional_token      @arg no_option or ___: Default. Order queue.
 *                            @arg optimize_mem: Mem controller selects cycle to issue
 *                            @arg priority: High priority
 *
 * Temp register usage: 0-2, uses registers if constant addr args > MAX_IMMEDIATE, or register or length.
 *
 * @b Example:
 * @code
 * scratch_write($packet[2], addr, 0, LWCOUNT3, SIG_SCRATCH. SIG_SCRATCH, ___)
 * @endcode
 */
#macro scratch_write(in_data, in_scratch_addr, in_addr_offset, in_lw_count, REQ_SIG, in_wakeup_sigs, Q_OPTION)
    _scratch_op(write, in_data, in_scratch_addr, in_addr_offset, in_lw_count, REQ_SIG, in_wakeup_sigs, Q_OPTION)
#endm


/// @cond INTERNAL_MACROS
/** Scratch operation where addresses are acceptable as is to Assembler.
 *
 * Support macros scratch_read, scratch_write.
 */
#macro _scratch_op(operation, in_data, in_scratch_addr, in_addr_offset, in_lw_count, REQ_SIG, in_wakeup_sigs, Q_OPTION)
    #if (!streq(in_data, no_xfer))
        xbuf_find(in_data)      // if found, #define _xbuf_found, _cur_xbuf_name, _cur_xbuf_size
        #if (is_ct_const (in_lw_count) || isimport(in_lw_count))
            #ifdef _xbuf_found
                #define _perform_xbuf_op
            #endif
            #ifdef _act_wd_count
                #warning "_act_wd_count is being redefined."
            #endif
            #if (MEM_WD_COUNT_MIN_1 == TRUE)
                #define_eval _act_wd_count (in_lw_count + 1)
            #else
                #define_eval _act_wd_count (in_lw_count)
            #endif
        #endif
    #endif
    #ifdef _perform_xbuf_op
        _scratch_op_xbufs(operation, in_scratch_addr, in_addr_offset, _act_wd_count, REQ_SIG, in_wakeup_sigs, Q_OPTION)
        #undef _perform_xbuf_op
    #elif (is_ct_const (in_lw_count) || isimport(in_lw_count))
        _scratch_const_instr(operation, in_data, in_scratch_addr, in_addr_offset, _act_wd_count, REQ_SIG, in_wakeup_sigs, Q_OPTION)
    #else
        _scratch_ind_instr(operation, in_data, in_scratch_addr, in_addr_offset, in_lw_count, REQ_SIG, in_wakeup_sigs, Q_OPTION)
    #endif  // (is_ct_const (in_lw_count))

    #ifdef _act_wd_count
        #undef _act_wd_count
    #endif
#endm
/// @endcond


/// @cond INTERNAL_MACROS
/** Perform scratch operation with one or more(linked) xbufs.
 *
 * Support macros scratch_read, scratch_read_lock, scratch_write, scratch_write_unlock.
 */
#macro _scratch_op_xbufs(operation, in_scratch_addr, in_addr_offset, in_lw_count, REQ_SIG, in_wakeup_sigs, Q_OPTION)
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
        .begin //new_base_addr

            .reg new_base_addr
            alu_op(new_base_addr, in_scratch_addr, +, in_addr_offset)
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

        _scratch_const_instr(operation, _cur_xbuf_name/**/_cur_xbuf_index, new_base_addr, _cur_offset, _rem_buf, tmp_signalp, SIG_NONE, ___)

        #define _SECOND_SIG
        #define_eval _cur_lw_offset (_cur_lw_offset + _rem_buf)
        #define_eval _cur_lw_count (_cur_lw_count - _rem_buf)

        #ifdef _cur_xbuf_next
            xbuf_param_set(_cur_xbuf_next)
            #define_eval _cur_xbuf_index 0
            #define_eval _rem_buf _cur_xbuf_size
        #else
            #error  "_scratch_op_xbufs. out of xbuf range"
        #endif  //  _cur_xbuf_next

    #endloop

    // perform last operation
        #define_eval _cur_offset (_cur_lw_offset <<2)       // convert back to byte address

    #ifdef _use_new_base
            #ifdef _SECOND_SIG
                _scratch_const_instr(operation, _cur_xbuf_name/**/_cur_xbuf_index, new_base_addr, _cur_offset, _cur_lw_count, REQ_SIG, signals(tmp_signalp, in_wakeup_sigs), Q_OPTION)
            #else
                _scratch_const_instr(operation, _cur_xbuf_name/**/_cur_xbuf_index, new_base_addr, _cur_offset, _cur_lw_count, REQ_SIG, in_wakeup_sigs, Q_OPTION)
            #endif
            #undef _use_new_base

        .end //new_base_addr
    #else
        #ifdef _SECOND_SIG
            _scratch_const_instr(operation, _cur_xbuf_name/**/_cur_xbuf_index, in_scratch_addr, _cur_offset, _cur_lw_count, REQ_SIG, signals(tmp_signalp, in_wakeup_sigs), Q_OPTION)
        #else
            _scratch_const_instr(operation, _cur_xbuf_name/**/_cur_xbuf_index, in_scratch_addr, _cur_offset, _cur_lw_count, REQ_SIG, in_wakeup_sigs, Q_OPTION)
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



////////////////////////////////// SCRATCH Single Word Operations ///////////////////////////////////////
/** Perform scratch single word operations.
 *
 * There is no in_lw_count argument.
 *
 * @param out_data            Read xfer register result
 * @param in_mask             Register or constant, mask of bits to set
 * @param in_scratch_addr     Register or constant base longword address
 * @param in_addr_offset      Register or constant longword address offset
 * @param in_wakeup_sigs      Signal or signals to wake up on
 * @param Q_OPTION            Queue option (no
 * @param optional_token      @arg no_option or ___: Default. Order queue.
 *                            @arg optimize_mem: Mem controller selects cycle to issue
 *                            @arg priority: High priority
 *
 * @instruction_cnt: 2-6
 *
//      xfer regs used:
 */

/** Clear in_mask bits at scratch longword location.
 *
 * Arguments: See SCRATCH Single Word Operations common Output/Inputs.
 */
#macro scratch_bits_clr(in_mask, in_scratch_addr, in_addr_offset, REQ_SIG, in_wakeup_sigs, Q_OPTION)
.begin

    _DEF_STATUS_IS_SRAM_XFER(in_mask)    // define _STATUS, test whether in_mask is already a transfer reg
    #if (_STATUS == FALSE)
        xbuf_alloc($bset_vector, 1, read_write)
        move($bset_vector[0], in_mask)
            _scratch_instr(clr, $bset_vector[0], in_scratch_addr, in_addr_offset, no_wd_count, REQ_SIG, in_wakeup_sigs, Q_OPTION)
        xbuf_free($bset_vector)
    #else
            _scratch_instr(clr, in_mask, in_scratch_addr, in_addr_offset, no_wd_count, REQ_SIG, in_wakeup_sigs, Q_OPTION)

    #endif

.end
#endm


/** Set in_mask bits at scratch longword location.
 *
 * Arguments: See SCRATCH Single Word Operations common Output/Inputs.
 *
 * @b Example:
 * @code
 * scratch_bits_set(0x111, 0, bit_position, SIG_SCRATCH. SIG_SCRATCH, ___)
 * @endcode
 */
#macro scratch_bits_set(in_mask, in_scratch_addr, in_addr_offset, REQ_SIG, in_wakeup_sigs, Q_OPTION)
.begin

    _DEF_STATUS_IS_SRAM_XFER(in_mask)    // define _STATUS, test whether in_mask is already a transfer reg
    #if (_STATUS == FALSE)
        xbuf_alloc($bset_vector, 1, read_write)
        move($bset_vector[0], in_mask)

        _scratch_instr(set, $bset_vector[0], in_scratch_addr, in_addr_offset, no_wd_count, REQ_SIG, in_wakeup_sigs, Q_OPTION)

        xbuf_free($bset_vector)
    #else
        _scratch_instr(set, in_mask, in_scratch_addr, in_addr_offset, no_wd_count, REQ_SIG, in_wakeup_sigs, Q_OPTION)
    #endif

.end
#endm


/** Clear in_mask bits at scratch longword location.
 *
 * Read contents of scratch address prior to the write.
 *
 * Arguments: See SCRATCH Single Word Operations common Output/Inputs.
 *
 */
#macro scratch_bits_test_and_clr(out_data, in_mask, in_scratch_addr, in_addr_offset, REQ_SIG, in_wakeup_sigs, Q_OPTION)
    #if (!streq(out_data, in_mask))
        move(out_data, in_mask)
    #endif

    _scratch_instr(test_clr, out_data, in_scratch_addr, in_addr_offset, dual_sig_op, REQ_SIG, in_wakeup_sigs, Q_OPTION)
#endm


/** Set in_mask bits at scratch longword location.
 *
 * Read contents of scratch address prior to the write.
 *
 * Arguments: See SCRATCH Single Word Operations common Output/Inputs.
 *
 * @b Example:
 * @code
 * scratch_bits_test_and_set(prev_value, 0x1000, addr0, addr1, SIG_SCRATCH. SIG_SCRATCH, ___) // test/set bit 3
 * @endcode
 */
#macro scratch_bits_test_and_set(out_data, in_mask, in_scratch_addr, in_addr_offset, REQ_SIG, in_wakeup_sigs, Q_OPTION)
    #if (!streq(out_data, in_mask))
        move(out_data, in_mask)
    #endif

    _scratch_instr(test_set, out_data, in_scratch_addr, in_addr_offset, dual_sig_op, REQ_SIG, in_wakeup_sigs, Q_OPTION)
#endm


/** Increment 32-bit longword at to scratch location.
 *
 * Arguments: See SCRATCH Single Word Operations common Output/Inputs.
 */
#macro scratch_incr(in_scratch_addr, in_addr_offset)
    _scratch_instr(incr, no_xfer, in_scratch_addr, in_addr_offset, no_wd_count, ___, ___, ___)
#endm


/// @cond INTERNAL_MACROS
// support bit_wr operations
//
#macro _scratch_bits_op(operation, io_data, in_scratch_addr, in_addr_offset, REQ_SIG, in_wakeup_sigs, Q_OPTION)
    _scratch_instr(operation, io_data, in_scratch_addr, in_addr_offset, no_wd_count, REQ_SIG, in_wakeup_sigs, Q_OPTION)
#endm
/// @endcond


/** Add in_data to scratch location.
 *
 * @param in_data            Data to be added to the scratch location specified by
 *                           in_scratch_addr and in_addr_offset.
 *                           in_data must be a write transfer register aka. out transfer register
 * @param in_scratch_addr    Register or constant base longword address
 * @param in_addr_offset     Register or constant longword address offset
 * @param in_wakeup_sigs     Signal or signals to wake up on
 * @param Q_OPTION           Queue option (no
 * @param optional_token     @arg no_option or ___: Default. Order queue.
 *                           @arg optimize_mem: Mem controller selects cycle to issue
 *                           @arg priority: High priority
 *
 */
#macro scratch_add(in_data, in_scratch_addr, in_addr_offset, REQ_SIG, in_wakeup_sigs, Q_OPTION)
    _scratch_instr(add, in_data, in_scratch_addr, in_addr_offset, no_wd_count, REQ_SIG, in_wakeup_sigs, Q_OPTION)
#endm


/** Decrement 32-bit longword at scratch location.
 *
 * Arguments: See SCRATCH Single Word Operations common Output/Inputs.
 */
#macro scratch_decr(in_scratch_addr, in_addr_offset)
    _scratch_instr(decr, no_xfer, in_scratch_addr, in_addr_offset, no_wd_count, ___, ___, ___)
#endm


/** Sub in_data from scratch location.
 *
 * Arguments: See SCRATCH Single Word Operations common Output/Inputs.
 *
 * @param in_data            Data to be subtracted from the scratch location specified by
 *                           in_scratch_addr and in_addr_offset
 * @param in_scratch_addr    Register or constant base longword address
 * @param in_addr_offset     Register or constant longword address offset
 * @param in_wakeup_sigs     Signal or signals to wake up on
 * @param Q_OPTION           Queue option (no
 * @param optional_token     @arg no_option or ___: Default. Order queue.
 *                           @arg optimize_mem: Mem controller selects cycle to issue
 *                           @arg priority: High priority
 *
 * @note sub instr is not supported in HW, so we subtract from 0 and add that number.
 *       in_data can be GPR or a read transfer register.
 */
#macro scratch_sub(in_data, in_scratch_addr, in_addr_offset, REQ_SIG, in_wakeup_sigs, Q_OPTION)
    _scratch_instr(sub, in_data, in_scratch_addr, in_addr_offset, no_wd_count, REQ_SIG, in_wakeup_sigs, Q_OPTION)
#endm


/** Write in_data to scratch location.
 *
 * Read contents of scratch location prior to the operation to out_data.
 *
 * @param out_data          A read/write transfer registers pair. The result is returned
 *                          in the read part of the read/write transfer registers pair.
 * @param in_data           Can be constant, gpr, read transfer register
 * @param in_scratch_addr   Register or constant base longword address
 * @param in_addr_offset    Register or constant longword address offset
 *
 * @note If in_data and out_data is a pair of read/write transfer registers
 *       with the same name, ex. $buffer0, the data from the $buffer0.write
 *       will be written to (in_scratch_addr+in_addr_offset). The data from
 *       (in_scratch_addr+in_addr_offset) will be returned in $buffer0.read.
 *
 * @b Example:
 * @code
 * scratch_swap(prev_value, new_value, addr0, addr1, SIG_SCRATCH. SIG_SCRATCH, ___) // test/set bit 3
 * @endcode
 */
#macro scratch_swap(out_data, in_data, in_scratch_addr, in_addr_offset, REQ_SIG, in_wakeup_sigs, Q_OPTION)
    #if (!streq(out_data, in_data))
    move(out_data, in_data)
    #endif

    _scratch_instr(swap, out_data, in_scratch_addr, in_addr_offset, dual_sig_op, REQ_SIG, in_wakeup_sigs, Q_OPTION)
#endm


/** Read contents of scratch location to out_data.
 *
 * Then add in_data to scratch location contents.
 *
 * @param out_data          A read/write transfer registers pair. The result is returned
 *                          in the read part of the read/write transfer registers pair.
 * @param in_data           Can be constant, gpr, read transfer reg
 * @param in_scratch_addr   Register or constant base longword address
 * @param in_addr_offset    Register or constant longword address offset
 *
 * @note If in_data and out_data is a pair of read/write transfer registers
 *       with the same name, ex. $buffer0, the data from the $buffer0.write
 *       will be written to (in_scratch_addr+in_addr_offset). The data from
 *      (in_scratch_addr+in_addr_offset) will be returned in $buffer0.read.
 *
 * @b Example:
 * @code
 * scratch_test_and_add(prev_value, addend, addr0, addr1, SIG_SCRATCH. SIG_SCRATCH, ___) // test/set bit 3
 * @endcode
 */
#macro scratch_test_and_add(out_data, in_data, in_scratch_addr, in_addr_offset, REQ_SIG, in_wakeup_sigs, Q_OPTION)
    #if (!streq(out_data, in_data))
        move(out_data, in_data)
    #endif

    _scratch_instr(test_and_add, out_data, in_scratch_addr, in_addr_offset, dual_sig_op, REQ_SIG, in_wakeup_sigs, Q_OPTION)
#endm


/** Read contents of scratch location to out_data.
 *
 * Then decrement scratch location contents.
 *
 * Arguments: See SCRATCH Single Word Operations common Output/Inputs.
 *
 * @note out_data must be a transfer register.
 *
 * @b Example:
 * @code
 * scratch_test_and_decr(prev_value, addr0, addr1, SIG_SCRATCH. SIG_SCRATCH, ___) // test/set bit 3
 * @endcode
 */
#macro scratch_test_and_decr(out_data, in_scratch_addr, in_addr_offset, REQ_SIG, in_wakeup_sigs, Q_OPTION)
    _scratch_instr(test_and_decr, out_data, in_scratch_addr, in_addr_offset, dual_sig_op, REQ_SIG, in_wakeup_sigs, Q_OPTION)
#endm


/** Read contents of scratch location to out_data.
 *
 * Then increment scratch location contents.
 *
 * Arguments: See SCRATCH Single Word Operations common Output/Inputs.
 *
 * @note out_data must be a transfer register.
 *
 * @b Example:
 * @code
 * scratch_test_and_incr(prev_value, addr0, addr1, SIG_SCRATCH. SIG_SCRATCH, ___) // test/set bit 3
 * @endcode
 */
#macro scratch_test_and_incr(out_data, in_scratch_addr, in_addr_offset, REQ_SIG, in_wakeup_sigs, Q_OPTION)
    _scratch_instr(test_and_incr, out_data, in_scratch_addr, in_addr_offset, dual_sig_op, REQ_SIG, in_wakeup_sigs, Q_OPTION)
#endm



/// @cond INTERNAL_MACROS
/** Instantiate the scratch instruction.
 *
 * Convert REQ_SIG, in_wakeup_sigs, Q_OPTION to the correct optional tokens.
 * no_xfer may have been inserted by macros such as scratch_incr, where xfer is not used.
 *
 * @param op_code           Legal SCRATCH opcode such as add, read, write (see PRM for
 *                          complete list)
 * @param io_transfer       If io_transfer is out xfer register, this is where the result is kept.
 * @param in_scratch_addr   Register or constant base longword address
 * @param in_addr_offset    Register or constant longword address offset
 * @param in_wd_count       May be a number, dual_sig_op (for 2-signal ops), or no_wd_count
 * @param in_wakeup_sigs    Signal or signals to wake up on
 * @param Q_OPTION          Queue option @n
 *                          @arg no_option or ___: Default. Order queue.
 *                          @arg optimize_mem: Mem controller selects cycle to issue
 *                          @arg priority: High priority
 */
#macro _scratch_instr(op_code, io_xfer, in_scratch_addr, in_addr_offset, in_wd_count, REQ_SIG, in_wakeup_sigs, Q_OPTION)
    _DEF_MEM_TOKENS(REQ_SIG, in_wakeup_sigs, Q_OPTION, in_wd_count)

    _GET_ADDRESSES(_MEM_ADDR0, _MEM_ADDR1, in_scratch_addr, in_addr_offset, SCRATCH)

    #if (streq(io_xfer, no_xfer))
        #if (streq(in_wd_count, no_wd_count))
            #if (REQ_SIG || in_wakeup_sigs || Q_OPTION)
                scratch[op_code, --, _MEM_ADDR0, _MEM_ADDR1], _MEM_TOKENS
            #else
                scratch[op_code, --, _MEM_ADDR0, _MEM_ADDR1]
            #endif
        #else
            scratch[op_code, --, _MEM_ADDR0, _MEM_ADDR1, in_wd_count], _MEM_TOKENS
        #endif
    #else
        #if ( (streq(in_wd_count, no_wd_count)) || (streq(in_wd_count, dual_sig_op)) )
            scratch[op_code, io_xfer, _MEM_ADDR0, _MEM_ADDR1], _MEM_TOKENS
        #else
            scratch[op_code, io_xfer, _MEM_ADDR0, _MEM_ADDR1, in_wd_count], _MEM_TOKENS
        #endif
    #endif

    _FREE_ADDRESSES()
    #ifdef _HW_SIG_EXPR
        #undef _HW_SIG_EXPR
        ctx_arb[in_wakeup_sigs]
    #endif
#endm
/// @endcond


/// @cond INTERNAL_MACROS
/** Instantiate the scratch instruction and insert indirect token.
 *
 * Convert REQ_SIG, in_wakeup_sigs, Q_OPTION to the correct optional tokens.
 * no_xfer may have been inserted by macros such as scratch_incr, where xfer is not used.
 *
 * Arguments: See SCRATCH Single Word Operations common Output/Inputs.
 */
#macro _scratch_ind_instr(op_code, io_xfer, in_scratch_addr, in_addr_offset, in_wd_count, REQ_SIG, in_wakeup_sigs, Q_OPTION)
    _DEF_MEM_TOKENS(REQ_SIG, in_wakeup_sigs, Q_OPTION, in_wd_count)

    _GET_ADDRESSES(_MEM_ADDR0, _MEM_ADDR1, in_scratch_addr, in_addr_offset, SCRATCH)

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
        #if (defined(__NFP_INDIRECT_REF_FORMAT_IXP))
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
            #error "This macro is only available in IXP indirect reference format mode."
        #endif

        #if (streq(io_xfer, no_xfer))
            //value of word count is not important because the real count is in the indirect pad
            scratch[op_code, --, _MEM_ADDR0, _MEM_ADDR1, _MAX_NN], indirect_ref, _MEM_TOKENS
        #else
            //value of word count is not important because the real count is in the indirect pad
            scratch[op_code, io_xfer, _MEM_ADDR0, _MEM_ADDR1, _MAX_NN], indirect_ref, _MEM_TOKENS
        #endif

        // clean up name space
        #ifdef _NN_SIZE
            #undef _NN_SIZE
        #endif
        #undef _MAX_NN

    .end

    _FREE_ADDRESSES()

    #ifdef _HW_SIG_EXPR
        #undef _HW_SIG_EXPR
        ctx_arb[in_wakeup_sigs]
    #endif
#endm
/// @endcond


/// @cond INTERNAL_MACROS
/** A wrapper macro that calls _scratch_instr if in_wd_count is <= 8 or
 * calls _scratch_ind_instr with a temporary GPR if in_wd_count is > 8.
 *
 * @note in_wd_count is a constant.
 */
#macro _scratch_const_instr(op_code, io_xfer, in_scratch_addr, in_addr_offset, in_wd_count, REQ_SIG, in_wakeup_sigs, Q_OPTION)
    #if (in_wd_count > 8)
        .begin

            .reg _wd_count_gpr
            immed32(_wd_count_gpr, in_wd_count)
            _scratch_ind_instr(op_code, io_xfer, in_scratch_addr, in_addr_offset, _wd_count_gpr, REQ_SIG, in_wakeup_sigs, Q_OPTION)

        .end
    #else
        _scratch_instr(op_code, io_xfer, in_scratch_addr, in_addr_offset, in_wd_count, REQ_SIG, in_wakeup_sigs, Q_OPTION)
    #endif
#endm
/// @endcond


#macro scratch_memset(in_gs_addr, in_len, lw_pattern, _CHUNK_SIZE_)
.begin

    #if (!is_ct_const(_CHUNK_SIZE_))
        #error "scratch_memset: Chunk size must be constant."
    #endif

    #if ( (_CHUNK_SIZE_ <= 0) || (_CHUNK_SIZE_ > 32) )
        #error "scratch_memset: Chunk Size must be in multiples of 4-bytes ranging 4-32:"[_CHUNK_SIZE_]
    #endif

    #if (_CHUNK_SIZE_ & 0x3)
        #error "scratch_memset: Chunk Size must be multiple of 4-bytes:" [_CHUNK_SIZE_]
    #endif

    #define_eval _NUM_XFERS  (_CHUNK_SIZE_ / 4)

    #define_eval _CHUNK_REF_CNT (_CHUNK_SIZE_ / 4)

    .reg $dest_in[_NUM_XFERS]

    .xfer_order $dest_in
    .sig gs_sig

    #if (is_ct_const(in_gs_addr))
        .reg cur_gs_addr
        move(cur_gs_addr, in_gs_addr)
        #define_eval _GS_BASE_ADDR_    cur_gs_addr
    #else
        #define_eval _GS_BASE_ADDR_    in_gs_addr
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
    #endloop        /* _COUNT */

    #undef _COUNT

    .while (_REM_BYTES_ > 0)

        //
        // init gs to long word pattern specified
        //
        scratch[write, $dest_in[0], _GS_BASE_ADDR_, 0, _CHUNK_REF_CNT], sig_done[gs_sig]
        ctx_arb[gs_sig]

        alu[_GS_BASE_ADDR_, _GS_BASE_ADDR_, +, _CHUNK_SIZE_]
        alu[_REM_BYTES_, _REM_BYTES_, -, _CHUNK_SIZE_]

    .endw

    #undef _GS_BASE_ADDR_
    #undef _REM_BYTES_
    #undef _PATTERN_
    #undef _NUM_XFERS
    #undef _CHUNK_REF_CNT

.end
#endm


/** @}
 * @}
 */

#endif /* __SCRATCH_UC__ */
