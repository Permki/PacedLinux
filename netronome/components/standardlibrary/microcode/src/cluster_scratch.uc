/*
 * Copyright (C) 2008-2013 Netronome Systems, Inc.  All rights reserved.
 */

#ifndef __CLUSTER_SCRATCH_UC__
#define __CLUSTER_SCRATCH_UC__

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

/** @file cluster_scratch.uc Cluster Local Scratch Operation Macros
 * @addtogroup cls Cluster Local Scratch
 * @{
 *
 * @name Cluster Local Scratch Operation Macros
 * @{
 *
 *
 * Default: all memory operation word counts are actual word counts.
 * @see MEM_WD_COUNT_MIN_1
 */

// API Quick Reference:
//
// cls_bits_clr(in_mask, in_cls_addr, in_addr_offset, REQ_SIG, in_wakeup_sigs, Q_OPTION)
// cls_bits_set(in_mask, in_cls_addr, in_addr_offset, REQ_SIG, in_wakeup_sigs, Q_OPTION)
// cls_bits_test_and_clr(out_data, in_mask, in_cls_addr, in_addr_offset, REQ_SIG, in_wakeup_sigs, Q_OPTION)
// cls_bits_test_and_set(out_data, in_mask, in_cls_addr, in_addr_offset, REQ_SIG, in_wakeup_sigs, Q_OPTION)
// cls_incr (in_cls_addr, in_addr_offset)
// cls_decr (in_cls_addr, in_addr_offset)
// cls_read(out_data, in_cls_addr, in_addr_offset, in_lw_count, REQ_SIG, in_wakeup_sigs, Q_OPTION)
// cls_write(in_data, in_cls_addr, in_addr_offset, in_lw_count, REQ_SIG, in_wakeup_sigs, Q_OPTION)
// cls_add(in_data, in_cls_addr, in_addr_offset, REQ_SIG, in_wakeup_sigs, Q_OPTION)
// cls_sub (in_data, in_cls_addr, in_addr_offset, REQ_SIG, in_wakeup_sigs, Q_OPTION)
// cls_swap (out_data, in_data, in_cls_addr, in_addr_offset, REQ_SIG, in_wakeup_sigs, Q_OPTION)
// cls_test_and_add (out_data, in_data, in_cls_addr, in_addr_offset, REQ_SIG, in_wakeup_sigs, Q_OPTION)
// cls_test_and_decr (out_data, in_cls_addr, in_addr_offset, REQ_SIG, in_wakeup_sigs, Q_OPTION)
// cls_test_and_incr (out_data, in_cls_addr, in_addr_offset, REQ_SIG, in_wakeup_sigs, Q_OPTION)



/** Read from Cluster Local Scratch starting at address of first longword.
 *
 * @b Example:
 * @code
 * cls_read($packet[2], addr, 0, LWCOUNT3, SIG_CS, SIG_CS, ___)
 * @endcode
 *
 * @param out_data          First transfer register of sequence to read to, array notation
 *                          must be in xbuf array notation, index range 0-15 for NFP-32xx
 * @param in_cls_addr       Register or constant base longword address
 * @param in_addr_offset    Register or constant longword address offset
 * @param in_lw_count       Register or constant longword count
 * @param REQ_SIG           Signal associated with this request
 * @param in_wakeup_sigs    Signal or signals to wake up on
 * @param Q_OPTION          Queue options:
 *                              @arg @c no_option or @c ___: Default. Order queue
 *                              @arg @c optimize_mem: Mem controller selects cycle to issue
 *                              @arg @c priority: High priority
 *
 * @note Temporary register usage: Uses 0 to 2 registers if constant addr args > MAX_IMMEDIATE, or register length.
 *
 */
#macro cls_read(out_data, in_cls_addr, in_addr_offset, in_lw_count, REQ_SIG, in_wakeup_sigs, Q_OPTION)
    _cls_op(read, out_data, in_cls_addr, in_addr_offset, in_lw_count, REQ_SIG, in_wakeup_sigs, Q_OPTION)
#endm


/** Read from Cluster Local Scratch starting at address of first longword in little endian.
 *
 * @b Example:
 * @code
 * cls_read_le($packet[2], addr, 0, LWCOUNT3, SIG_CS, SIG_CS, ___)
 * @endcode
 *
 * @param out_data          First transfer register of sequence to read to, array notation
 *                          must be in xbuf array notation, index range 0-15 for NFP-32xx
 * @param in_cls_addr       Register or constant base longword address
 * @param in_addr_offset    Register or constant longword address offset
 * @param in_lw_count       Register or constant longword count
 * @param REQ_SIG           Signal associated with this request
 * @param in_wakeup_sigs    Signal or signals to wake up on
 * @param Q_OPTION          Queue options:
 *                              @arg @c no_option or @c ___: Default. Order queue
 *                              @arg @c optimize_mem: Mem controller selects cycle to issue
 *                              @arg @c priority: High priority
 *
 * @note Temporary register usage: uses 0 to 2 registers if constant addr args > MAX_IMMEDIATE, or register length.
 *
 */
#macro cls_read_le(out_data, in_cls_addr, in_addr_offset, in_lw_count, REQ_SIG, in_wakeup_sigs, Q_OPTION)
    _cls_op(read_le, out_data, in_cls_addr, in_addr_offset, in_lw_count, REQ_SIG, in_wakeup_sigs, Q_OPTION)
#endm


/** Write to Cluster Local Scratch.
 *
 * @b Example:
 * @code
 * cls_write($packet[2], addr, 0, LWCOUNT3, SIG_CS, SIG_CS, ___)
 * @endcode
 *
 * @param in_data           First transfer register of sequence to write from, array notation
 *                          must be in xbuf array notation, index range 0-15 for NFP-32xx
 * @param in_cls_addr       Register or constant base longword address
 * @param in_addr_offset    Register or constant longword address offset
 * @param in_lw_count       Register or constant longword count
 * @param REQ_SIG           Signal associated with this request
 * @param in_wakeup_sigs    Signal or signals to wake up on
 * @param Q_OPTION          Queue options:
 *                              @arg @c no_option or @c ___: Default. Order queue
 *                              @arg @c optimize_mem: Mem controller selects cycle to issue
 *                              @arg @c priority: High priority
 *
 * @note Temporary register usage: Uses 0 to 2 registers if constant addr args > MAX_IMMEDIATE, or register length.
 *
 */
#macro cls_write(in_data, in_cls_addr, in_addr_offset, in_lw_count, REQ_SIG, in_wakeup_sigs, Q_OPTION)
    _cls_op(write, in_data, in_cls_addr, in_addr_offset, in_lw_count, REQ_SIG, in_wakeup_sigs, Q_OPTION)
#endm


/** Write to Cluster Local Scratch (Little Endian).
 *
 * @b Example:
 * @code
 * cls_write_le($packet[2], addr, 0, LWCOUNT3, SIG_CS, SIG_CS, ___)
 * @endcode
 *
 * @param in_data           First transfer register of sequence to write from, array notation
 *                          must be in xbuf array notation, index range 0-15 for NFP-32xx
 * @param in_cls_addr       Register or constant base longword address
 * @param in_addr_offset    Register or constant longword address offset
 * @param in_lw_count       Register or constant longword count
 * @param REQ_SIG           Signal associated with this request
 * @param in_wakeup_sigs    Signal or signals to wake up on
 * @param Q_OPTION          Queue options:
 *                              @arg @c no_option or @c ___: Default. Order queue
 *                              @arg @c optimize_mem: Mem controller selects cycle to issue
 *                              @arg @c priority: High priority
 *
 * @note Temporary register usage: Uses 0 to 2 registers if constant addr args > MAX_IMMEDIATE, or register length.
 *
 */
#macro cls_write_le(in_data, in_cls_addr, in_addr_offset, in_lw_count, REQ_SIG, in_wakeup_sigs, Q_OPTION)
    _cls_op(write_le, in_data, in_cls_addr, in_addr_offset, in_lw_count, REQ_SIG, in_wakeup_sigs, Q_OPTION)
#endm


/** Write bytes to Cluster Local Scratch.
 *
 * @b Example:
 * @code
 * cls_write_byte($packet, addr, 0, BYTE_CNT, SIG_CS, SIG_CS, ___)
 * @endcode
 *
 * @param in_data           First transfer register of sequence to write from, array notation
 *                          must be in xbuf array notation, index range 0-15 for NFP-32xx
 * @param in_cls_addr       Register or constant base longword address
 * @param in_addr_offset    Register or constant longword address offset
 * @param in_lw_count       Register or constant longword count
 * @param REQ_SIG           Signal associated with this request
 * @param in_wakeup_sigs    Signal or signals to wake up on
 * @param Q_OPTION          Queue options:
 *                              @arg @c no_option or @c ___: Default. Order queue
 *                              @arg @c optimize_mem: Mem controller selects cycle to issue
 *                              @arg @c priority: High priority
 *
 * @note Temporary register usage: Uses 0 to 2 registers if constant addr args > MAX_IMMEDIATE, or register length.
 *
 */
#macro cls_write_byte(in_data, in_cls_addr, in_addr_offset, in_lw_count, REQ_SIG, in_wakeup_sigs, Q_OPTION)
    _cls_op(write8, in_data, in_cls_addr, in_addr_offset, in_lw_count, REQ_SIG, in_wakeup_sigs, Q_OPTION)
#endm


/** Write bytes to Cluster Local Scratch (Little Endian).
 *
 * @b Example:
 * @code
 * cls_write_byte_le($packet, addr, 0, BYTE_CNT, SIG_CS, SIG_CS, ___)
 * @endcode
 *
 * @param in_data           First transfer register of sequence to write from, array notation
 *                          must be in xbuf array notation, index range 0-15 for NFP-32xx
 * @param in_cls_addr       Register or constant base longword address
 * @param in_addr_offset    Register or constant longword address offset
 * @param in_lw_count       Register or constant longword count
 * @param REQ_SIG           Signal associated with this request
 * @param in_wakeup_sigs    Signal or signals to wake up on
 * @param Q_OPTION          Queue options:
 *                              @arg @c no_option or @c ___: Default. Order queue
 *                              @arg @c optimize_mem: Mem controller selects cycle to issue
 *                              @arg @c priority: High priority
 *
 * @note Temporary register usage: Uses 0 to 2 registers if constant addr args > MAX_IMMEDIATE, or register length.
 *
 */
#macro cls_write_byte_le(in_data, in_cls_addr, in_addr_offset, in_lw_count, REQ_SIG, in_wakeup_sigs, Q_OPTION)
    _cls_op(write8_le, in_data, in_cls_addr, in_addr_offset, in_lw_count, REQ_SIG, in_wakeup_sigs, Q_OPTION)
#endm


/// @cond INTERNAL_MACROS
/** Internal macro for Cluster Local Scratch operation where addresses are acceptable as is to Assembler.
 *
 * Supports the @ref cls_read and @ref cls_write macros.
 *
 * @param operation         Supported operation
 * @param in_data           First transfer register of sequence to write from, array notation
 *                          must be in xbuf array notation, index range 0-15 for NFP-32xx
 * @param in_cls_addr       Register or constant base longword address
 * @param in_addr_offset    Register or constant longword address offset
 * @param in_lw_count       Register or constant longword count
 * @param REQ_SIG           Signal associated with this request
 * @param in_wakeup_sigs    Signal or signals to wake up on
 * @param Q_OPTION          Queue options:
 *                              @arg @c no_option or @c ___: Default. Order queue
 *                              @arg @c optimize_mem: Mem controller selects cycle to issue
 *                              @arg @c priority: High priority
 *
 * @note Temporary register usage: Uses 0 to 2 registers if constant addr args > MAX_IMMEDIATE, or register length.
 * @see MEM_WD_COUNT_MIN_1
 *
 */
#macro _cls_op(operation, in_data, in_cls_addr, in_addr_offset, in_lw_count, REQ_SIG, in_wakeup_sigs, Q_OPTION)
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
        _cls_op_xbufs(operation, in_cls_addr, in_addr_offset, _act_wd_count, REQ_SIG, in_wakeup_sigs, Q_OPTION)
        #undef _perform_xbuf_op
    #elif (is_ct_const (in_lw_count) || isimport(in_lw_count))
        _cls_const_instr(operation, in_data, in_cls_addr, in_addr_offset, _act_wd_count, REQ_SIG, in_wakeup_sigs, Q_OPTION)
    #else
        _cls_ind_instr(operation, in_data, in_cls_addr, in_addr_offset, in_lw_count, REQ_SIG, in_wakeup_sigs, Q_OPTION)
    #endif  // (is_ct_const (in_lw_count))

    #ifdef _act_wd_count
        #undef _act_wd_count
    #endif
#endm
/// @endcond


/// @cond INTERNAL_MACROS
/** Internal macro to perform Cluster Local Scratch operation with one or more (linked) xbufs.
 *
 * Supports the @ref _cls_op macro.
 *
 * @param operation         Supported operation
 * @param in_cls_addr       Register or constant base longword address
 * @param in_addr_offset    Register or constant longword address offset
 * @param in_lw_count       Register or constant longword count
 * @param REQ_SIG           Signal associated with this request
 * @param in_wakeup_sigs    Signal or signals to wake up on
 * @param Q_OPTION          Queue options:
 *                              @arg @c no_option or @c ___: Default. Order queue
 *                              @arg @c optimize_mem: Mem controller selects cycle to issue
 *                              @arg @c priority: High priority
 *
 * @note Temp register usage: Uses 0 to 2 registers if constant addr args > MAX_IMMEDIATE, or register length.
 *
 */
#macro _cls_op_xbufs(operation, in_cls_addr, in_addr_offset, in_lw_count, REQ_SIG, in_wakeup_sigs, Q_OPTION)
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
            alu_op(new_base_addr, in_cls_addr, +, in_addr_offset)
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

        _cls_const_instr(operation, _cur_xbuf_name/**/_cur_xbuf_index, new_base_addr, _cur_offset, _rem_buf, tmp_signalp, SIG_NONE, ___)

        #define _SECOND_SIG
        #define_eval _cur_lw_offset (_cur_lw_offset + _rem_buf)
        #define_eval _cur_lw_count (_cur_lw_count - _rem_buf)

        #ifdef _cur_xbuf_next
            xbuf_param_set(_cur_xbuf_next)
            #define_eval _cur_xbuf_index 0
            #define_eval _rem_buf _cur_xbuf_size
        #else
            #error  "_cls_op_xbufs. out of xbuf range"
        #endif  //  _cur_xbuf_next

    #endloop

    // perform last operation
    #define_eval _cur_offset (_cur_lw_offset <<2)       // convert back to byte address

    #ifdef _use_new_base
            #ifdef _SECOND_SIG
                _cls_const_instr(operation, _cur_xbuf_name/**/_cur_xbuf_index, new_base_addr, _cur_offset, _cur_lw_count, REQ_SIG, signals(tmp_signalp, in_wakeup_sigs), Q_OPTION)
            #else
                _cls_const_instr(operation, _cur_xbuf_name/**/_cur_xbuf_index, new_base_addr, _cur_offset, _cur_lw_count, REQ_SIG, in_wakeup_sigs, Q_OPTION)
            #endif
            #undef _use_new_base

        .end //new_base_addr
    #else
        #ifdef _SECOND_SIG
            _cls_const_instr(operation, _cur_xbuf_name/**/_cur_xbuf_index, in_cls_addr, _cur_offset, _cur_lw_count, REQ_SIG, signals(tmp_signalp, in_wakeup_sigs), Q_OPTION)
        #else
            _cls_const_instr(operation, _cur_xbuf_name/**/_cur_xbuf_index, in_cls_addr, _cur_offset, _cur_lw_count, REQ_SIG, in_wakeup_sigs, Q_OPTION)
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


/** Clear @p in_mask bits at Cluster Local Scratch longword location.
 *
 * @param in_mask           Register or constant, mask of bits to set
 * @param in_cls_addr       Register or constant base longword address
 * @param in_addr_offset    Register or constant longword address offset
 * @param REQ_SIG           Signal associated with this request
 * @param in_wakeup_sigs    Signal or signals to wake up on
 * @param Q_OPTION          Queue options:
 *                              @arg @c no_option or @c ___: Default. Order queue
 *                              @arg @c optimize_mem: Mem controller selects cycle to issue
 *                              @arg @c priority: High priority
 *
 * @instruction_cnt 2 to 6
 */
#macro cls_bits_clr(in_mask, in_cls_addr, in_addr_offset, REQ_SIG, in_wakeup_sigs, Q_OPTION)
.begin

    _DEF_STATUS_IS_SRAM_XFER(in_mask)      // define _STATUS, test whether in_mask is already a transfer reg
    #if (_STATUS == FALSE)
        xbuf_alloc($bset_vector, 1, read_write)
        move($bset_vector[0], in_mask)
        _cls_instr(clr, $bset_vector[0], in_cls_addr, in_addr_offset, no_wd_count, REQ_SIG, in_wakeup_sigs, Q_OPTION)
        xbuf_free($bset_vector)
    #else
        _cls_instr(clr, in_mask, in_cls_addr, in_addr_offset, no_wd_count, REQ_SIG, in_wakeup_sigs, Q_OPTION)
    #endif

.end
#endm


/** Set @p in_mask bits at Cluster Local Scratch longword location.
 *
 * @b Example:
 * @code
 * cls_bits_set(0x111, 0, bit_position, SIG_CS, SIG_CS, ___)
 * @endcode
 *
 * @param in_mask           Register or constant, mask of bits to set
 * @param in_cls_addr       Register or constant base longword address
 * @param in_addr_offset    Register or constant longword address offset
 * @param REQ_SIG           Signal associated with this request
 * @param in_wakeup_sigs    Signal or signals to wake up on
 * @param length            Word count, @c dual_sig_op (for 2-signal ops), or @c no_wd_count
 * @param Q_OPTION          Queue options:
 *                              @arg @c no_option or @c ___: Default. Order queue
 *                              @arg @c optimize_mem: Mem controller selects cycle to issue
 *                              @arg @c priority: High priority
 *
 * @instruction_cnt 2 to 6
 */
#macro cls_bits_set(in_mask, in_cls_addr, in_addr_offset, REQ_SIG, in_wakeup_sigs, length, Q_OPTION)
.begin

    _DEF_STATUS_IS_SRAM_XFER(in_mask)      // define _STATUS, test whether in_mask is already a transfer reg
    #if (_STATUS == FALSE)
        xbuf_alloc($bset_vector, 1, read_write)
        move($bset_vector[0], in_mask)
        _cls_instr(set, $bset_vector[0], in_cls_addr, in_addr_offset, length, REQ_SIG, in_wakeup_sigs, Q_OPTION)
        xbuf_free($bset_vector)
    #else
        _cls_instr(set, in_mask, in_cls_addr, in_addr_offset, length, REQ_SIG, in_wakeup_sigs, Q_OPTION)
    #endif

.end
#endm


/** Clear @p in_mask bits at Cluster Local Scratch longword location.
 *
 * Read contents of Cluster Local Scratch
 * address prior to the write.
 *
 * @param out_data          Read transfer register result
 * @param in_mask           Register or constant, mask of bits to set
 * @param in_cls_addr       Register or constant base longword address
 * @param in_addr_offset    Register or constant longword address offset
 * @param REQ_SIG           Signal associated with this request
 * @param in_wakeup_sigs    Signal or signals to wake up on
 * @param length            Word count, @c dual_sig_op (for 2-signal ops), or @c no_wd_count
 * @param Q_OPTION          Queue options:
 *                              @arg @c no_option or @c ___: Default. Order queue
 *                              @arg @c optimize_mem: Mem controller selects cycle to issue
 *                              @arg @c priority: High priority
 *
 */
#macro cls_bits_test_and_clr(out_data, in_mask, in_cls_addr, in_addr_offset, REQ_SIG, in_wakeup_sigs, length, Q_OPTION)
    #if (!streq(out_data, in_mask))
        move(out_data, in_mask)
    #endif

    _cls_instr(test_clr, out_data, in_cls_addr, in_addr_offset, length, REQ_SIG, in_wakeup_sigs, Q_OPTION)
#endm


/** Set @p in_mask bits at Cluster Local Scratch longword location.
 *
 * Read contents of Cluster Local Scratch address prior to the write.
 *
 * @b Example:
 * @code
 * cls_bits_test_and_set(prev_value, 0x1000, addr0, addr1, SIG_CS, SIG_CS, ___) // test/set bit 3
 * @endcode
 *
 * @param out_data          Read transfer register result
 * @param in_mask           Register or constant, mask of bits to set
 * @param in_cls_addr       Register or constant base longword address
 * @param in_addr_offset    Register or constant longword address offset
 * @param REQ_SIG           Signal associated with this request
 * @param in_wakeup_sigs    Signal or signals to wake up on
 * @param length            Word count, @c dual_sig_op (for 2-signal ops), or @c no_wd_count
 * @param Q_OPTION          Queue options:
 *                              @arg @c no_option or @c ___: Default. Order queue
 *                              @arg @c optimize_mem: Mem controller selects cycle to issue
 *                              @arg @c priority: High priority
 *
 */
#macro cls_bits_test_and_set(out_data, in_mask, in_cls_addr, in_addr_offset, REQ_SIG, in_wakeup_sigs, length, Q_OPTION)
    #if (!streq(out_data, in_mask))
        move(out_data, in_mask)
    #endif

    _cls_instr(test_set, out_data, in_cls_addr, in_addr_offset, length, REQ_SIG, in_wakeup_sigs, Q_OPTION)
#endm


/** Increment 32-bit longword at Cluster Local Scratch location.
 *
 * @param in_cls_addr       Register or constant base longword address
 * @param in_addr_offset    Register or constant longword address offset
 *
 */
#macro cls_incr(in_cls_addr, in_addr_offset)
    _cls_instr(add_imm, no_xfer, in_cls_addr, in_addr_offset, 1, ___, ___, ___)
#endm

/** Decrement 32-bit longword at Cluster Local Scratch location.
 *
 * @param in_cls_addr       Register or constant base longword address
 * @param in_addr_offset    Register or constant longword address offset
 *
 */
#macro cls_decr(in_cls_addr, in_addr_offset)
    _cls_instr(sub_imm, no_xfer, in_cls_addr, in_addr_offset, 1, ___, ___, ___)
#endm

/// @cond INTERNAL_MACROS
/** Internal macro to support bit_wr operations.
 *
 * @param operation         Supported operation
 * @param io_data           IO command
 * @param in_cls_addr       Register or constant base longword address
 * @param in_addr_offset    Register or constant longword address offset
 * @param REQ_SIG           Signal associated with this request
 * @param in_wakeup_sigs    Signal or signals to wake up on
 * @param Q_OPTION          Queue options:
 *                              @arg @c no_option or @c ___: Default. Order queue
 *                              @arg @c optimize_mem: Mem controller selects cycle to issue
 *                              @arg @c priority: High priority
 *
 */
#macro _cls_bits_op(operation, io_data, in_cls_addr, in_addr_offset, REQ_SIG, in_wakeup_sigs, Q_OPTION)
    _cls_instr(operation, io_data, in_cls_addr, in_addr_offset, no_wd_count, REQ_SIG, in_wakeup_sigs, Q_OPTION)
#endm
/// @endcond


/** Add @p in_data to Cluster Local Scratch location.
 *
 * @param in_data           Data to be added to Cluster Local Scratch location specified by @p in_cls_addr
 *                          and @p in_addr_offset. @p in_data must be a write transfer register i.e. out transfer register.
 * @param in_cls_addr       Register or constant base longword address
 * @param in_addr_offset    Register or constant longword address offset
 * @param REQ_SIG           Signal associated with this request
 * @param in_wakeup_sigs    Signal or signals to wake up on
 * @param Q_OPTION          Queue options:
 *                              @arg @c no_option or @c ___: Default. Order queue
 *                              @arg @c optimize_mem: Mem controller selects cycle to issue
 *                              @arg @c priority: High priority
 *
 */
#macro cls_add(in_data, in_cls_addr, in_addr_offset, REQ_SIG, in_wakeup_sigs, Q_OPTION)
    _cls_instr(add, in_data, in_cls_addr, in_addr_offset, 1, REQ_SIG, in_wakeup_sigs, Q_OPTION)
#endm


/** Subtract @p in_data from Cluster Local Scratch location.
 *
 * @param in_data           Data to be added to Cluster Local Scratch location specified by @p in_cls_addr
 *                          and @p in_addr_offset
 * @param in_cls_addr       Register or constant base longword address
 * @param in_addr_offset    Register or constant longword address offset
 * @param REQ_SIG           Signal associated with this request
 * @param in_wakeup_sigs    Signal or signals to wake up on
 * @param Q_OPTION          Queue options:
 *                              @arg @c no_option or @c ___: Default. Order queue
 *                              @arg @c optimize_mem: Mem controller selects cycle to issue
 *                              @arg @c priority: High priority
 *
 * @note The sub instruction is not supported in HW, so subtract from 0 and add that number.
 *       @p in_data can be GPR or a read transfer register.
 */
#macro cls_sub(in_data, in_cls_addr, in_addr_offset, REQ_SIG, in_wakeup_sigs, Q_OPTION)
    _cls_instr(sub, in_data, in_cls_addr, in_addr_offset, no_wd_count, REQ_SIG, in_wakeup_sigs, Q_OPTION)
#endm


/** Write @p in_data to Cluster Local Scratch location.
 *
 * Read contents of Cluster Local Scratch location
 * prior to the operation to @p out_data.
 *
 * @b Example:
 * @code
 * cls_swap(prev_value, new_value, addr0, addr1, SIG_CS, SIG_CS, ___) // test/set bit 3
 * @endcode
 *
 * @param out_data          A read/write transfer registers pair. Result is returned in the read part
 *                          of the read/write transfer registers pair.
 * @param in_data           Can be constant, GPR, read transfer register
 * @param in_cls_addr       Register or constant base longword address
 * @param in_addr_offset    Register or constant longword address offset
 * @param REQ_SIG           Signal associated with this request
 * @param in_wakeup_sigs    Signal or signals to wake up on
 * @param Q_OPTION          Queue options:
 *                              @arg @c no_option or @c ___: Default. Order queue
 *                              @arg @c optimize_mem: Mem controller selects cycle to issue
 *                              @arg @c priority: High priority
 *
 * @note If @p in_data and @p out_data is a pair of read/write transfer registers with the same name, eg. $buffer0,
 *       the data from the $buffer0.write will be written to (@p in_cls_addr + @p in_addr_offset). The data from
 *       (@p in_cls_addr + @p in_addr_offset) will be returned in $buffer0.read.
 */
#macro cls_swap(out_data, in_data, in_cls_addr, in_addr_offset, REQ_SIG, in_wakeup_sigs, Q_OPTION)
    #if (!streq(out_data, in_data))
        move(out_data, in_data)
    #endif

    _cls_instr(swap, out_data, in_cls_addr, in_addr_offset, dual_sig_op, REQ_SIG, in_wakeup_sigs, Q_OPTION)
#endm


/** Read contents of Cluster Local Scratch location to @p out_data then add
 *  @p in_data to Cluster Local Scratch location contents.
 *
 * @b Example:
 * @code
 * cls_test_and_add(prev_value, addend, addr0, addr1, SIG_CS, SIG_CS, ___) // test/set bit 3
 * @endcode
 *
 * @param out_data          A read/write transfer registers pair. Result is returned in the read part
 *                          of the read/write transfer registers pair.
 * @param in_data           Can be constant, GPR, read transfer register
 * @param in_cls_addr       Register or constant base longword address
 * @param in_addr_offset    Register or constant longword address offset
 * @param REQ_SIG           Signal associated with this request
 * @param in_wakeup_sigs    Signal or signals to wake up on
 * @param Q_OPTION          Queue options:
 *                              @arg @c no_option or @c ___: Default. Order queue
 *                              @arg @c optimize_mem: Mem controller selects cycle to issue
 *                              @arg @c priority: High priority
 *
 * @note If @p in_data and @p out_data is a pair of read/write transfer registers with the same name, eg. $buffer0,
 *       the data from the $buffer0.write will be written to (@p in_cls_addr + @p in_addr_offset). The data from
 *       (@p in_cls_addr + @p in_addr_offset) will be returned in $buffer0.read.
 */
#macro cls_test_and_add(out_data, in_data, in_cls_addr, in_addr_offset, REQ_SIG, in_wakeup_sigs, Q_OPTION)
    #if (!streq(out_data, in_data))
        move(out_data, in_data)
    #endif

    _cls_instr(test_add, out_data, in_cls_addr, in_addr_offset, dual_sig_op, REQ_SIG, in_wakeup_sigs, Q_OPTION)
#endm


/** Read contents of Cluster Local Scratch location to @p out_data then
 *  decrement Cluster Local Scratch location contents.
 *
 * @b Example:
 * @code
 * cls_test_and_decr(prev_value, addr0, addr1, SIG_CS, SIG_CS, ___) // test/set bit 3
 * @endcode
 *
 * @param out_data          A read/write transfer registers pair. Result is returned in the read part
 *                          of the read/write transfer registers pair.
 * @param in_cls_addr       Register or constant base longword address
 * @param in_addr_offset    Register or constant longword address offset
 * @param REQ_SIG           Signal associated with this request
 * @param in_wakeup_sigs    Signal or signals to wake up on
 * @param Q_OPTION          Queue options:
 *                              @arg @c no_option or @c ___: Default. Order queue
 *                              @arg @c optimize_mem: Mem controller selects cycle to issue
 *                              @arg @c priority: High priority
 *
 * @note @p out_data must be a transfer register.
 */
#macro cls_test_and_decr(out_data, in_cls_addr, in_addr_offset, REQ_SIG, in_wakeup_sigs, Q_OPTION)
    _cls_instr(test_and_decr, out_data, in_cls_addr, in_addr_offset, dual_sig_op, REQ_SIG, in_wakeup_sigs, Q_OPTION)
#endm


/** Read contents of Cluster Local Scratch location to @p out_data then
 *  increment Cluster Local Scratch location contents.
 *
 * @b Example:
 * @code
 * cls_test_and_incr(prev_value, addr0, addr1, SIG_CS, SIG_CS, ___) // test/set bit 3
 * @endcode
 *
 * @param out_data          A read/write transfer registers pair. Result is returned in the read part
 *                          of the read/write transfer registers pair.
 * @param in_cls_addr       Register or constant base longword address
 * @param in_addr_offset    Register or constant longword address offset
 * @param REQ_SIG           Signal associated with this request
 * @param in_wakeup_sigs    Signal or signals to wake up on
 * @param Q_OPTION          Queue options:
 *                              @arg @c no_option or @c ___: Default. Order queue
 *                              @arg @c optimize_mem: Mem controller selects cycle to issue
 *                              @arg @c priority: High priority
 *
 * @note @p out_data must be a transfer register.
 */
#macro cls_test_and_incr(out_data, in_cls_addr, in_addr_offset, REQ_SIG, in_wakeup_sigs, Q_OPTION)
    _cls_instr(test_and_incr, out_data, in_cls_addr, in_addr_offset, dual_sig_op, REQ_SIG, in_wakeup_sigs, Q_OPTION)
#endm


/// @cond INTERNAL_MACROS
/** Internal macro to instantiate the Cluster Local Scratch instruction.
 *
 * Convert @p REQ_SIG, @p in_wakeup_sigs, @p Q_OPTION to the correct optional tokens.
 * @c no_xfer may have been inserted by macros such as @ref cls_incr, where @p io_xfer is not used.
 *
 * @param op_code           Legal CLUSTER SCRATCH opcode such as add, read, write (see PRM for complete list)
 * @param io_xfer           If io_transfer is out xfer register. Location of result.
 * @param in_cls_addr       Register or constant base longword address
 * @param in_addr_offset    Register or constant longword address offset
 * @param in_wd_count       May be a number, @c dual_sig_op (for 2-signal ops), or @c no_wd_count
 * @param REQ_SIG           Signal associated with this request
 * @param in_wakeup_sigs    Signal or signals to wake up on
 * @param Q_OPTION          Queue options:
 *                              @arg @c no_option or @c ___: Default. Order queue
 *                              @arg @c optimize_mem: Mem controller selects cycle to issue
 *                              @arg @c priority: High priority
 *
 */
#macro _cls_instr(op_code, io_xfer, in_cls_addr, in_addr_offset, in_wd_count, REQ_SIG, in_wakeup_sigs, Q_OPTION )
    _DEF_MEM_TOKENS(REQ_SIG, in_wakeup_sigs, Q_OPTION, in_wd_count)

    _GET_ADDRESSES(_MEM_ADDR0, _MEM_ADDR1, in_cls_addr, in_addr_offset, CLUSTER_SCRATCH)

    #if (streq(io_xfer, no_xfer))
        #if (streq(in_wd_count, no_wd_count))
            #if (REQ_SIG || in_wakeup_sigs || Q_OPTION)
                cls[op_code, --, _MEM_ADDR0, _MEM_ADDR1], _MEM_TOKENS
            #else
                cls[op_code, --, _MEM_ADDR0, _MEM_ADDR1]
            #endif
        #else
            #if (REQ_SIG || in_wakeup_sigs || Q_OPTION)
                cls[op_code, --, _MEM_ADDR0, _MEM_ADDR1, in_wd_count], _MEM_TOKENS
            #else
                cls[op_code, --, _MEM_ADDR0, _MEM_ADDR1, in_wd_count]
            #endif
        #endif
    #else
        #if ( (streq(in_wd_count, no_wd_count)) || (streq(in_wd_count, dual_sig_op)) )
            cls[op_code, io_xfer, _MEM_ADDR0, _MEM_ADDR1, in_wd_count], _MEM_TOKENS
        #else
            cls[op_code, io_xfer, _MEM_ADDR0, _MEM_ADDR1, in_wd_count], _MEM_TOKENS
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
/** Internal macro to instantiate the Cluster Local Scratch instruction and insert indirect token convert @p REQ_SIG,
 *  @p in_wakeup_sigs, @p Q_OPTION to the correct optional tokens.
 *
 * @c no_xfer may have been inserted by macros such as @ref cls_incr where @p io_xfer is not used.
 *
 * @param op_code           Legal CLUSTER SCRATCH opcode such as add, read, write (see PRM for complete list)
 * @param io_xfer           If io_transfer is out xfer register. Location of result.
 * @param in_cls_addr       Register or constant base longword address
 * @param in_addr_offset    Register or constant longword address offset
 * @param in_wd_count       May be a number, @c dual_sig_op (for 2-signal ops), or @c no_wd_count
 * @param REQ_SIG           Signal associated with this request
 * @param in_wakeup_sigs    Signal or signals to wake up on
 * @param Q_OPTION          Queue options:
 *                              @arg @c no_option or @c ___: Default. Order queue
 *                              @arg @c optimize_mem: Mem controller selects cycle to issue
 *                              @arg @c priority: High priority
 *
 * @see MEM_WD_COUNT_MIN_1
 */
#macro _cls_ind_instr(op_code, io_xfer, in_cls_addr, in_addr_offset, in_wd_count, REQ_SIG, in_wakeup_sigs, Q_OPTION)
    _DEF_MEM_TOKENS(REQ_SIG, in_wakeup_sigs, Q_OPTION, in_wd_count)

    _GET_ADDRESSES(_MEM_ADDR0, _MEM_ADDR1, in_cls_addr, in_addr_offset, CLUSTER_SCRATCH)

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

        #if (defined(__NFP_INDIRECT_REF_FORMAT_V1) || defined(__NFP_INDIRECT_REF_FORMAT_V2))
            // if all memory word counts are n
            #if (MEM_WD_COUNT_MIN_1 == FALSE)
                // convert to n-1
                alu[ref_count, in_wd_count, -, 1]
            #else
                alu[ref_count, --, B, in_wd_count]
            #endif

            #if (defined(__NFP_INDIRECT_REF_FORMAT_V1))
                alu[ref_count, ref_count, OR, 0x2, <<28] //encoding to overide ref_cnt field
            #else
                alu[ref_count, 0x80, OR, ref_count, <<8] //encoding to overide ref_cnt field
            #endif
        #else
            // IXP indirect reference format

            // if all memory word counts are n
            #if (MEM_WD_COUNT_MIN_1 == FALSE)
                // convert to n-1
                alu[ref_count, in_wd_count, -, 1]
                alu_shf[ref_count, --, B, ref_count, <<21]
            #else
                alu_shf[ref_count, --, B, in_wd_count, <<21]
            #endif

            alu_shf[ref_count, ref_count, OR, 0x1, <<25]
        #endif

        #if (streq(io_xfer, no_xfer))
            //value of word count is not important because the real count is in the indirect pad
            cls[op_code, --, _MEM_ADDR0, _MEM_ADDR1, _MAX_NN], indirect_ref, _MEM_TOKENS
        #else
            //value of word count is not important because the real count is in the indirect pad
            cls[op_code, io_xfer, _MEM_ADDR0, _MEM_ADDR1, _MAX_NN], indirect_ref, _MEM_TOKENS
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
/** An internal wrapper macro that calls @ref _cls_instr if @p in_wd_count is <= 8 or calls @ref _cls_ind_instr with
 *  a temporary GPR if @p in_wd_count is > 8.
 *
 * @param op_code           Legal CLUSTER SCRATCH opcode such as add, read, write (see PRM for complete list)
 * @param io_xfer           If io_transfer is out xfer register. Location of result.
 * @param in_cls_addr       Register or constant base longword address
 * @param in_addr_offset    Register or constant longword address offset
 * @param in_wd_count       May be a number, @c dual_sig_op (for 2-signal ops), or @c no_wd_count
 * @param REQ_SIG           Signal associated with this request
 * @param in_wakeup_sigs    Signal or signals to wake up on
 * @param Q_OPTION          Queue options:
 *                              @arg @c no_option or @c ___: Default. Order queue
 *                              @arg @c optimize_mem: Mem controller selects cycle to issue
 *                              @arg @c priority: High priority
 *
 * @note @p in_wd_count is a constant.
 */
#macro _cls_const_instr(op_code, io_xfer, in_cls_addr, in_addr_offset, in_wd_count, REQ_SIG, in_wakeup_sigs, Q_OPTION)
#if (in_wd_count > 8)
    .begin

        .reg _wd_count_gpr
        immed32(_wd_count_gpr, in_wd_count)
        _cls_ind_instr(op_code, io_xfer, in_cls_addr, in_addr_offset, _wd_count_gpr, REQ_SIG, in_wakeup_sigs, Q_OPTION)

    .end
#else
    _cls_instr(op_code, io_xfer, in_cls_addr, in_addr_offset, in_wd_count, REQ_SIG, in_wakeup_sigs, Q_OPTION)
#endif
#endm
/// @endcond


/** Initialize indirect reference register for use in IXP or NFP indirect reference format mode.
 *
 * @param indirect_ref_reg  Indirect GPR register to be initialized
 *
 */
#macro cls_add64_immed_init(indirect_ref_reg)
.begin

    immed[indirect_ref_reg, 0x0]
    #if (defined(__NFP_INDIRECT_REF_FORMAT_V1))
        alu[indirect_ref_reg, indirect_ref_reg, or, 0x8, <<28]
    #elif (defined(__NFP_INDIRECT_REF_FORMAT_V2))
        alu[indirect_ref_reg, indirect_ref_reg, or, 0x2, <<3]
    #else
        alu[indirect_ref_reg, indirect_ref_reg, or, 0x1, <<4]
        alu[indirect_ref_reg, indirect_ref_reg, or, 0x1, <<31]
    #endif

.end
#endm


/** Perform immediate add (64-bit) operation in Cluster Local Scratch.
 *
 * Can add up to a 16-bit value in NFP indirect reference mode and up to 255 in
 * IXP indirect reference format mode. For a 16-bit value, the two most significant
 * bits control sign extension (bits 15 and 14). This means only 14-bits are used
 * for an actual value.
 *
 * Bits 15 and 14 of a 16-bit value, as from the @prm, CLS (Atomic Operations):
 * @arg @c 00 indicates no sign extension
 * @arg @c 01 indicates sign extend to 32-bit/64-bit value for 32/64-bit operation respectively
 * @arg @c 10 indicates no sign extension, but duplicate immediate in both high
 *            and low 32-bit words (applicable to 64-bit operations only)
 * @arg @c 11 means sign extend to 32-bit word and duplicate value in both high
 *            and low 32-bit words (applicable to 64-bit operations only).
 *
 * Using @p val in the range from 1 to 7 (atomic increment/add), saves one instruction.
 *
 * @param indirect_ref_reg  Indirect GPR register initialized with @ref cls_add64_immed_init.
 *                          Not needed when @p val is a constant from 0 to 7.
 * @param val               Value to be added. Can be constant or GPR. If constant: Values in range 1-7,
 *                          no indirect and save 1 instruction. Values in range 8-255, indirect reference.
 *                          Values > 255 must be in GPR and use indrect reference.
 * @param addr              Base Address in cluster scratch where immed add operation is done.
 *                          Byte address. Constant or GPR.
 * @param offset            Offset from base. Add is done at (@p addr + @p offset). Byte address.
 *
 */
#macro cls_add64_immed(indirect_ref_reg, val, addr, offset)
.begin

    #if( is_ct_const(addr) )
        .reg addr_reg
        immed[addr_reg, addr]
        #define_eval BASE_ADDR   addr_reg
    #else
        #define_eval BASE_ADDR   addr
    #endif

    #if( is_ct_const(val) )
        #if ( (val > 0) && (val < 8) )
            cls[add64_imm, --, BASE_ADDR, offset, val]
        #else
            #if (defined(__NFP_INDIRECT_REF_FORMAT_V2))
                alu[--, indirect_ref_reg, or, val, <<16]
            #else
                alu[--, indirect_ref_reg, or, val, <<5]
            #endif
            cls[add64_imm, --, BASE_ADDR, offset, 0], indirect_ref
        #endif
    #else
        #if (defined(__NFP_INDIRECT_REF_FORMAT_V2))
            alu[--, indirect_ref_reg, or, val, <<16]
        #else
            alu[--, indirect_ref_reg, or, val, <<5]
        #endif
        cls[add64_imm, --, BASE_ADDR, offset, 0], indirect_ref
    #endif

.end
#endm


/** Fill a region of Cluster Local Scratch memory with a specified pattern.
 *
 * @param in_cls_addr Address to start memory fill from
 * @param in_len Number of bytes to set. Must be a multiple of @p CHUNK_SIZE.
 * @param lw_pattern 32-bit pattern to fill memory region with
 * @param CHUNK_SIZE Chunk size, a multiple of 4 bytes from 4 to 128 in
 *        NFP indirect reference format mode and 4 to 64 otherwise. Must be a constant.
 *
 */
#macro cls_memset(in_cls_addr, in_len, lw_pattern, CHUNK_SIZE)
.begin

    #if (!is_ct_const(CHUNK_SIZE))
        #error "cls_memset: Chunk size must be a constant."
    #endif

    #if (defined(__NFP_INDIRECT_REF_FORMAT_V1) || defined(__NFP_INDIRECT_REF_FORMAT_V2))
        #if ( (CHUNK_SIZE <= 0) || (CHUNK_SIZE > 128) )
            #error "cls_memset: Chunk Size must be in multiples of 4 bytes ranging 4-128:"[CHUNK_SIZE]
        #endif
    #else
        #if ( (CHUNK_SIZE <= 0) || (CHUNK_SIZE > 64) )
            #error "cls_memset: Chunk Size must be in multiples of 4 bytes ranging 4-64:"[CHUNK_SIZE]
        #endif
    #endif

    #if (CHUNK_SIZE & 0x3)
        #error "cls_memset: Chunk Size must be multiple of 4 bytes:" [CHUNK_SIZE]
    #endif

    #define_eval _NUM_XFERS  (CHUNK_SIZE / 4)

    #define_eval _CHUNK_REF_CNT (CHUNK_SIZE / 4)

    #if (_CHUNK_REF_CNT > 8)
        .reg indirect_ref_reg
        #if (defined(__NFP_INDIRECT_REF_FORMAT_V1))
            immed[indirect_ref_reg, (_CHUNK_REF_CNT - 1)]
            alu[indirect_ref_reg, indirect_ref_reg, OR, 0x2, <<28] //encoding to overide ref_cnt field
        #elif (defined(__NFP_INDIRECT_REF_FORMAT_V2))
            immed[indirect_ref_reg, (_CHUNK_REF_CNT - 1)]
            alu[indirect_ref_reg, 0x80, OR, indirect_ref_reg, <<8] //encoding to overide ref_cnt field
        #else
            // IXP indirect reference format
            alu_shf[indirect_ref_reg, --, B, 0x1, <<25]
            alu_shf[indirect_ref_reg, indirect_ref_reg, OR, (_CHUNK_REF_CNT - 1), <<21]
        #endif
    #endif

    .reg $dest_in[_NUM_XFERS]
    .xfer_order $dest_in
    .sig cls_sig

    #if (is_ct_const(in_cls_addr))
        .reg cur_cls_addr
        move(cur_cls_addr, in_cls_addr)
        #define_eval _CLS_BASE_ADDR_    cur_cls_addr
    #else
        #define_eval _CLS_BASE_ADDR_    in_cls_addr
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
        // init cls to long word pattern specified
        //
        #if (_CHUNK_REF_CNT > 8)
            alu[--, 0, OR, indirect_ref_reg]
            cls[write, $dest_in[0], _CLS_BASE_ADDR_, 0, max_/**/_CHUNK_REF_CNT], sig_done[cls_sig], indirect_ref
        #else
            cls[write, $dest_in[0], _CLS_BASE_ADDR_, 0, _CHUNK_REF_CNT], sig_done[cls_sig]
        #endif
        ctx_arb[cls_sig]

        alu[_CLS_BASE_ADDR_, _CLS_BASE_ADDR_, +, CHUNK_SIZE]
        alu[_REM_BYTES_, _REM_BYTES_, -, CHUNK_SIZE]

    .endw

    #undef _CLS_BASE_ADDR_
    #undef _REM_BYTES_
    #undef _PATTERN_
    #undef _NUM_XFERS
    #undef _CHUNK_REF_CNT

.end
#endm


#define lscratch_memset     cls_memset  /**< Deprecated alias @deprecated This alias should not be used in new code. @see cls_memset */

/** @}
 * @}
 */


#endif /* __CLUSTER_SCRATCH_UC__ */
