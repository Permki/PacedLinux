/*
 * Copyright (C) 2008-2011 Netronome Systems, Inc.  All rights reserved.
 */

#ifndef __MAILBOX_UC__
#define __MAILBOX_UC__

#ifndef NFP_LIB_ANY_NFAS_VERSION
    #if (!defined(__NFP_TOOL_NFAS) ||                       \
        (__NFP_TOOL_NFAS < NFP_SW_VERSION(5, 0, 0, 0)) ||   \
        (__NFP_TOOL_NFAS > NFP_SW_VERSION(6, 1, 0, 255)))
        #error "This standard library is not supported for the version of the SDK currently in use."
    #endif
#endif

#include <stdmac.uc>
#include <util.uc>
#include <sram.uc>
#if !(IS_NFPTYPE(__NFP6000) || IS_NFPTYPE(3800))
#include <scratch.uc>
#endif

/** @file mailbox.uc SRAM, Global Scratch or Transfer Register Mailbox Macros
 * @addtogroup sgx_mailbox SRAM, Global Scratch or Transfer Register Mailboxes
 * @{
 *
 * @name SRAM, Global Scratch or Transfer Register Mailbox Macros
 * @{
 */

// API Quick Reference:
//
// mailbox_init(in_data, in_addr_or_thd, in_offset_or_xfer, MEM_TYPE, MB_SIZE, in_req_sig, in_wakeup_sigs, Q_OPTION)
// mailbox_is_full(in_addr, in_addr_offset, MEM_TYPE)
// mailbox_send(in_first_data, in_wr_xfer, in_addr_or_thd, in_offset_or_xfer, MEM_TYPE, MB_SIZE, in_req_sig, in_wakeup_sigs, Q_OPTION)
// mailbox_receive(out_data, in_addr_or_thd, in_offset_or_xfer, MEM_TYPE, MB_SIZE, in_req_sig, in_wakeup_sigs, Q_OPTION)



/** Initialize a Global Scratch, SRAM or Reflector Mailbox to 0.
 *
 *  This is called once either by the sender or receiver side.
 *
 *  @param in_addr_or_thd      If MEM_TYPE = sram or scratch, mailbox base address
 *                             If MEM_TYPE = xfer, destination thread id (must be contants)
 *  @param in_offset_or_xfer   If MEM_TYPE = sram or scratch, offset to be added to mailbox base address
 *                             If MEM_TYPE = xfer, read xfer reg used for mailbox at dest thread
 *  @param MEM_TYPE            Memory type: sram, scratch, xfer (case insensitive)
 *                             If xfer, use sig_remote.
 *  @param MB_SIZE             Constant size of mailbox. up to 8 longwords
 *  @param in_req_sig          Requested signal, indicating mailbox data has been moved to mailbox
 *  @param in_wakeup_sigs      Wakeup signals. swap out and wakeup when all of these signals are received.
 *  @param Q_OPTION
 *
 *  @note If MEM_TYPE is XFER, in_req_sig and in_wakeup_sigs must be the same, and this
 *        signal must be the same as the remote ME's signal.
 *
 *  @instruction_cnt 2-26 for xfer type (data is transferred across MEs)
 *  @instruction_cnt 2-25 for sram/scratch type (1 memory write to sram or scratch)
 *
 *  example 1:
 *  @code
 *          .sig sig_mb_send
 *          mailbox_init(mb_base, mb_offset, SRAM, 2, sig_mb_send, sig_mb_send, ___)
 *  @endcode
 *
 *  example 2:
 *  @code
 *          .sig sig_mb_send
 *          .remote sig_mb_send
 *          .visible sig_mb_send
 *           * initialize thread 20's mailbox
 *          mailbox_send(20, $mailbox[0], XFER, 2, sig_mb_send, sig_mb_send, ___)
 *  @endcode
 */
#macro mailbox_init(in_addr_or_thd, in_offset_or_xfer, MEM_TYPE, MB_SIZE, in_req_sig, in_wakeup_sigs, Q_OPTION)
.begin

    #if ( (IS_NFPTYPE(__NFP6000) || IS_NFPTYPE(__NFP3800)) && \
          (streq(MEM_TYPE, xfer) || streq(MEM_TYPE, scratch )))
        #error "mailbox_init: MEM_TYPE xfer or scratch not supported on NFP6000."
    #endif

    #if (MB_SIZE > 8)
        #error "maximum is 8 longwords for MB_SIZE."
    #endif
    xbuf_alloc($mailbox_xfer, MB_SIZE, write)

    alu_shf[$mailbox_xfer[0], --, B, 1, <<31]
    #if (MB_SIZE > 1)
        move($mailbox_xfer[1], 0)
    #endif
    #if (MB_SIZE > 2)
        move($mailbox_xfer[2], 0)
    #endif
    #if (MB_SIZE > 3)
        move($mailbox_xfer[3], 0)
    #endif
    #if (MB_SIZE > 4)
        move($mailbox_xfer[4], 0)
    #endif
    #if (MB_SIZE > 5)
        move($mailbox_xfer[5], 0)
    #endif
    #if (MB_SIZE > 6)
        move($mailbox_xfer[6], 0)
    #endif
    #if (MB_SIZE > 7)
        move($mailbox_xfer[7], 0)
    #endif

    #if (streq(MEM_TYPE, xfer))
        // cap (reflector) write

        #if (!streq(in_wakeup_sigs, in_req_sig))
            #error "mailbox_init: in_req_sig and in_wakeup_sigs must be the same for XFER type."
        #endif


        #ifdef _REMOTE_ME
            #warning "_REMOTE_ME is being redefined."
        #endif
        #ifdef _REMOTE_CTX
            #warning "_REMOTE_CTX is being redefined."
        #endif
        #define_eval _REMOTE_ME (in_addr_or_thd>>3)
        #define_eval _REMOTE_CTX (in_addr_or_thd & 0x7)


        #if IS_IXPTYPE(__IXP28XX)
            #if (_REMOTE_ME >= 16)
                #error "Remote ME out of range."
            #endif
            #if ( _REMOTE_ME > 7 )
                #define_eval _REMOTE_ME ((_REMOTE_ME - 8) | 0x10)
            #endif
        #else
            #error "Chip type must be IXP28XX."
        #endif

        cap[write, $mailbox_xfer[0], _REMOTE_ME, in_offset_or_xfer, _REMOTE_CTX, MB_SIZE], ctx_swap[in_req_sig], sig_remote[in_wakeup_sigs]

        #undef _REMOTE_ME
        #undef _REMOTE_CTX
    #else
        #ifdef _MEM_TYPE
            #warning "_MEM_TYPE is being redefined."
        #endif
        #define_eval _MEM_TYPE MEM_TYPE
        _MEM_TYPE/**/_write($mailbox_xfer[0], in_addr_or_thd, in_offset_or_xfer, MB_SIZE, in_req_sig, in_wakeup_sigs, ___)
        #undef _MEM_TYPE
    #endif
    xbuf_free($mailbox_xfer)

.end
#endm


/** Test whether the mailbox is full before sending to it.
 *
 * Only used for MEM_TYPE sram or scratch, where valid bit 31 of first longword indicates valid
 * if MEM_TYPE is xfer, signal should be used instead to indicated full or availibility
 *
 * @param in_addr           If sram or scratch, first address to be added to form mailbox address
 * @param in_addr_offset    If sram or scratch, second address to be added to form mailbox address
 * @param MEM_TYPE          Memory type: sram, scratch. (xfer type is not supported)
 *
 * @instruction_cnt 2 (1 memory read from sram or scratch)
 *
 * @condition_codes For SRAM or Global Scratch: Z = 1 if not full.
 */
#macro mailbox_is_full(in_addr, in_addr_offset, MEM_TYPE)
    #if ((IS_NFPTYPE(__NFP6000) || IS_NFPTYPE(__NFP3800)) && \
          (streq(MEM_TYPE, xfer) || streq(MEM_TYPE, scratch )))
        #error "mailbox_is_full: MEM_TYPE xfer or scratch not supported on NFP6000."
    #endif
    #if (streq(MEM_TYPE, xfer))
        #error "mailbox_is_full. MEM_TYPE xfer not supported."
    #endif
    .begin

        .sig tmp_signal
        #ifdef _MEM_TYPE
            #warning "_MEM_TYPE is being redefined."
        #endif
        #define_eval _MEM_TYPE MEM_TYPE
        xbuf_alloc($mailbox_xfer, 1, read)
        _MEM_TYPE/**/_read($mailbox_xfer[0], in_addr, in_addr_offset, 1, tmp_signal, tmp_signal, ___)
        alu_shf[--, --, B, $mailbox_xfer[0], >>31]
        #undef _MEM_TYPE
        xbuf_free($mailbox_xfer)

    .end
#endm

/** Write a scratch, sram or reflector mailbox.
 *
 * if MEM_TYPE is sram or scratch, clear bit 31 of the first longword to indicate  mailbox valid as a valid bit.
 * if MEM_TYPE is xfer (i.e. using reflector)
 *
 * @param in_first_data         GPR or constant - first word of mailbox [30:0] if using bit 31 valid bit option.
 *                              bit 31 is reserved for macro use.
 * @param in_wr_xfer            Write xfer reg. containing mailbox data if valid_bit option is used.
 *                              The 1st longword (32 bits) must be in_first_data.
 *                              If valid_bit option is not used, contain all words of mailbox data
 * @param in_addr_or_thd        If MEM_TYPE = sram or scratch, mailbox base address
 *                              If MEM_TYPE = xfer, destination thread id (must be contants)
 * @param in_offset_or_xfer     If MEM_TYPE = sram or scratch, offset to be added to mailbox base address
 *                              If MEM_TYPE = xfer, read xfer reg used for mailbox at dest thread
 * @param MEM_TYPE              Memory type: sram, scratch, xfer (case insensitive)
 *                              If xfer, use sig_remote, other thread's signal is same as this one
 * @param MB_SIZE               Constant size of mailbox
 * @param in_req_sig            Requested signal, indicating mailbox data has been moved to mailbox
 * @param in_wakeup_sigs        Wakeup signals. swap out and wakeup when all of these signals are received.
 * @param Q_OPTION              ___
 *
 *
 * @note    in_reg_sig and in_wakeup_sigs must be the same if MEM_TYPE is XFER.
 *          Make sure mailbox_init was called either by sender or receiver.
 *
 * @instruction_cnt 1-2 for xfer type (data is transferred across MEs)
 * @instruction_cnt 2-5 for scram/scratch type (1 memory write to sram or scratch)
 *
 * example 1:
 * @code
 *          .sig sig_mb_send
 *          // test mailbox full
 *          mailbox_wait#
 *          mailbox_is_full(mb_base, mb_offset, SRAM)
 *          beq[mailbox_wait#]
 *          mailbox_send(data0, $mailbox[0], mb_base, mb_offset, SRAM, 2, sig_mb_send, sig_mb_send, ___)
 * @endcode
 *
 * example 2:
 * @code
 *          .sig sig_mb_send sig_mb_avail
 *          .remote sig_mb_send $mailbox0
 *          .visible sig_mb_send $mailbox0
 *          // here assume receiver signals that mailbox is available
 *          mailbox_wait#
 *          br_!signal[sig_mb_avail, mailbox_wait#]
 *          mailbox_send(data0, $wr_xfer[0], 20, $mailbox[0], XFER, 2, sig_mb_send, sig_mb_send, ___)
 * @endcode
 */
#macro mailbox_send(in_first_data, in_wr_xfer, in_addr_or_thd, in_offset_or_xfer, MEM_TYPE, MB_SIZE, in_req_sig, in_wakeup_sigs, Q_OPTION)
    #if ((IS_NFPTYPE(__NFP6000) || IS_NFPTYPE(__NFP3800)) && \
          (streq(MEM_TYPE, xfer) || streq(MEM_TYPE, scratch )))
        #error "mailbox_send: MEM_TYPE xfer or scratch not supported on NFP6000."
    #endif
    #if (streq(MEM_TYPE, xfer))
        // cap (reflector) write

        #ifdef _REMOTE_ME
            #warning "_REMOTE_ME is being redefined."
        #endif
        #ifdef _REMOTE_CTX
            #warning "_REMOTE_CTX is being redefined."
        #endif
        #define_eval _REMOTE_ME (in_addr_or_thd>>3)
        #define_eval _REMOTE_CTX (in_addr_or_thd & 0x7)


        #if IS_IXPTYPE(__IXP28XX)
            #if (_REMOTE_ME >= 16)
                #error "Remote ME out of range."
            #endif
            #if ( _REMOTE_ME > 7 )
                #define_eval _REMOTE_ME ((_REMOTE_ME - 8) | 0x10)
            #endif
        #else
            #error "Chip type must be IXP28XX."
        #endif

        cap[write, in_wr_xfer, _REMOTE_ME, in_offset_or_xfer, _REMOTE_CTX, MB_SIZE], ctx_swap[in_req_sig], sig_remote[in_wakeup_sigs]

        #undef _REMOTE_ME
        #undef _REMOTE_CTX
    #else
        // sram or scratch
        // clear bit 31 of first word
        #if(is_ct_const(in_first_data) || isimport(in_first_data))
            #ifdef _immed_data
                #warning "_immed_data is being redefined."
            #endif
            #define_eval _immed_data (in_first_data & 0x7FFFFFFF)
            move(in_wr_xfer, _immed_data)
            #undef _immed_data
        #else
            alu_shf_left(in_wr_xfer, in_first_data, AND~, 1, 31)
        #endif
        #ifdef _MEM_TYPE
            #warning "_MEM_TYPE is being redefined."
        #endif
        #define_eval _MEM_TYPE MEM_TYPE
        _MEM_TYPE/**/_write(in_wr_xfer, in_addr_or_thd, in_offset_or_xfer, MB_SIZE, in_req_sig, in_wakeup_sigs, ___)
        #undef _MEM_TYPE
    #endif
#endm

/** Get a message from a mailbox.
 *
 *  Can be a multiword message up to the max number of xfer regs.
 *  If MEM_TYPE is sram or scratch, use bit 31 of first longword as a valid bit
 *  use test and set on first longword bit 31 valid bit.
 *  If MEM_TYPE is XFER, mailbox is valid when signal is received.
 *
 *  @param out_data             Data bits received
 *  @param in_addr_or_thd       If sram or scratch, first address to be added to form mailbox address
 *  @param in_offset_or_xfer    If sram or scratch, second address to be added to form mailbox address
 *  @param MEM_TYPE             Memory type: sram, scratch, xfer (case insensitive)
 *  @param in_req_sig           Mailbox signal
 *  @param in_wakeup_sigs       Mailbox signal and other signals to wakeup on
 *  @param Q_OPTION             If sram or scratch, ___, ordered, optimize_mem, priority
 *
 *  @note in_reg_sig and in_wakeup_sigs must be the same if MEM_TYPE is XFER.
 *        Make sure mailbox_init was called either by sender or receiver.
 *
 *  @instruction_cnt 1 for xfer type
 *  @instruction_cnt 2-8 for sram/scratch type. Best case: 1 atomic sram/scratch access.
 *                   Worst case: 1 sram/scratch read and 1 atomic sram/scratch access
 *
 *  @condition_codes For SRAM or Global Scratch: Z = 1 if not valid (stale data), Z = 0 if valid (fresh data)
 *
 *  @b Example 1:
 *  @code
 *          .sig sig_mb_rec
 *          xbuf_alloc($mailbox, 2)
 *          mailbox_receive($mailbox[0], mb_addr, mb_offset, SRAM, 2, sig_mb_rec, sig_mb_rec, ___)
 *          ...
 *          xbuf_free($mailbox)
 *  @endcode
 *
 *  @b Example 2:
 *  @code
 *          .sig sig_mb_send
 *          .visible sig_mb_send
 *          xbuf_alloc($mailbox, 2)
 *          mailbox_receive($mailbox[0], 0, 0, XFER, 2, sig_mb_send, sig_mb_send, ___)
 *          ...
 *          xbuf_free($mailbox)
 *  @endcode
 *
 */
#macro mailbox_receive(out_data, in_addr_or_thd, in_offset_or_xfer, MEM_TYPE, MB_SIZE, in_req_sig, in_wakeup_sigs, Q_OPTION)
    #if ((IS_NFPTYPE(__NFP6000) || IS_NFPTYPE(__NFP3800)) && \
          (streq(MEM_TYPE, xfer) || streq(MEM_TYPE, scratch )))
        #error "mailbox_receive: MEM_TYPE xfer or scratch not supported on NFP6000."
    #endif
    #if (streq(MEM_TYPE, xfer))
        // This is to get rid of warning that "signal is being used
        // before being set". This lets the assembler know that signal
        // is being generated somewhere else.
        .set_sig in_wakeup_sigs

        // mailbox written directly to xfer reg
        // the signal indicates the read xfer out_data is present
        ctx_arb[in_wakeup_sigs], AND
    #else
        .begin

            #ifdef _MEM_TYPE
                #warning "_MEM_TYPE is being redefined."
            #endif
            // mailbox written to sram or scratch
            #define_eval _MEM_TYPE MEM_TYPE
            #if (MB_SIZE == 1)
                _MEM_TYPE/**/_bits_test_and_set(out_data, 0x80000000, in_addr_or_thd, in_offset_or_xfer, in_req_sig, in_wakeup_sigs, Q_OPTION)
            #else
                .sig tmp_signalp
                .reg $tmp_mb_xfer

                _MEM_TYPE/**/_read(out_data, in_addr_or_thd, in_offset_or_xfer, MB_SIZE, tmp_signalp, SIG_NONE, Q_OPTION)
                _MEM_TYPE/**/_bits_test_and_set($tmp_mb_xfer, 0x80000000, in_addr_or_thd, in_offset_or_xfer, in_req_sig, signals(tmp_signalp, in_wakeup_sigs), Q_OPTION)

            #endif
            #undef _MEM_TYPE
            // set condition codes
            alu_shf[--, --, B, $tmp_mb_xfer, >>31]

        .end
    #endif
#endm


/** @}
 * @}
 */

#endif /* __MAILBOX_UC__ */
