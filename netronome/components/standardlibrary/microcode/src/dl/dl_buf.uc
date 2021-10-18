/*
 * Copyright (C) 2009-2011 Netronome Systems, Inc.  All rights reserved.
 */

#ifndef __DL_BUF_UC__
#define __DL_BUF_UC__

#ifndef NFP_LIB_ANY_NFAS_VERSION
    #if (!defined(__NFP_TOOL_NFAS) ||                       \
        (__NFP_TOOL_NFAS < NFP_SW_VERSION(5, 0, 0, 0)) ||   \
        (__NFP_TOOL_NFAS > NFP_SW_VERSION(6, 1, 0, 255)))
        #error "This standard library is not supported for the version of the SDK currently in use."
    #endif
#endif

#include <buf.uc>
#include "dl_system.h"

// If this flag is defined, the application requires multiple buffer freelists.
// This file has buffer utility macros that support multiple buffer freelists.
#ifdef MULTIPLE_FREELIST_ENABLE
    #include <dl/dl_buf_multi.uc>
#endif

/** @file dl_buf.uc Packet Buffer Utility Macros
 * @addtogroup dl_buf Packet Buffer Utility
 * @{
 *
 * @name Packet Buffer Utility Macros
 * @{
 *
 */


//  Number of Packet Buffers

#define_eval    NO_OF_PKT_BUFFERS   BUF_SRAM_SIZE/META_DATA_SIZE

//  Some Sanity Check. Check if the specified SDRAM memory is of suffcient size
//  to hold all buffers

#define_eval    DRAM_COMPUTE        BUF_SDRAM_SIZE/BUFFER_SIZE

#if (DRAM_COMPUTE != NO_OF_PKT_BUFFERS)
    #error  "The amount of memory allocated in SRAM for packet buffers doesn't match with\
    the SDRAM size. Check BUF_SRAM_SIZE and BUF_SDRAM_SIZE. Also check META_DATA_SIZE and BUFFER_SIZE"
#endif

#undef          DRAM_COMPUTE

#define         AVLBL_PKT_BUFFERS   NO_OF_PKT_BUFFERS

//  DL_DS_RATIO = Sizeof (SDRAM entry) / Sizeof (SRAM Entry)

#define_eval    DL_DS_RATIO         BUFFER_SIZE/META_DATA_SIZE

#ifndef USE_IMPORT_VAR
    //  SRAM-QArray ID for enqueing/dequeing pkt buffers. (i.e maintinaing the free list)

    #ifndef BUF_FREE_LIST0
        #define         BUF_FREE_LIST0      BUF_QARRAY_BASE
    #endif


    //  The combination of BUF_SDRAM_BASE, BUFFER_SIZE, BUF_SRAM_BASE, META_DATA_SIZE
    //  specify a particular buffer pool.

    //  Specifying a base of address of 0 for SRAM (and DRAM) simplifies the arithmatic required
    //  to translate the handle to offset. (it saves one instruction. See dl_buf_get_desc)
    //  However, dl_buf_alloc will return 0 for a null buffer (i.e no buffer available) and it may
    //  not be possible to differentiate between a null buffer and a buffer at address 0.
    //  So we simply make that one buffer at address 0 unavailable for allocation/freeing, yet
    //  taking advantage of using a base address of 0. We end up wasting a little memory (1 buffer)
    //  but that's ok.

    #if (BUF_SRAM_BASE == 0 || BUF_SRAM_BASE == 0x40000000 || BUF_SRAM_BASE == 0x80000000 || BUF_SRAM_BASE == 0xC0000000)
        #define_eval    BUF_SRAM_BASE       BUF_SRAM_BASE  + META_DATA_SIZE
        #define_eval    BUF_SDRAM_BASE      BUF_SDRAM_BASE + BUFFER_SIZE
        #define_eval    AVLBL_PKT_BUFFERS   NO_OF_PKT_BUFFERS - 1
    #endif

    #define         BUF_POOL            BUF_SDRAM_BASE, BUFFER_SIZE, BUF_SRAM_BASE, META_DATA_SIZE


    // DL_REL_BASE = DRAM_BASE - (BUF_SRAM_BASE * DL_DS_RATIO)

    // avoid the channel number in the calculation

    #define_eval    TEMP_BUF_BASE       (BUF_SRAM_BASE & 0x3fffffff)
    #define_eval    DL_REL_BASE         BUF_SDRAM_BASE - (TEMP_BUF_BASE * DL_DS_RATIO)
    #undef          TEMP_BUF_BASE

#else // defined USE_IMPORT_VAR /* not simulation */
    #define        BUF_POOL            buf_sdram_base_reg, BUFFER_SIZE, buf_sram_base_reg, META_DATA_SIZE
#endif // USE_IMPORT_VAR

//  ln(DL_DS_RATIO). i.e 2^MULT_FACTOR = DL_DS_RATIO. This value is required to
//  reduce multiplication in to simple shift operation. The following set of lines
//  computes MULT_FACTOR (at compile time)

#define MY_COUNT            0
#define MY_RATIO        DL_DS_RATIO
#while (MY_RATIO)
    #define_eval        IS_SET      (MY_RATIO & 0x1)
    #if (IS_SET == 0)
        #define_eval    MY_COUNT    (MY_COUNT+1)
        #define_eval    MY_RATIO    (MY_RATIO>>1)
    #else
        #define_eval    MULT_FACTOR MY_COUNT
        #define_eval    MY_RATIO    0
    #endif
#endloop

#undef  IS_SET
#undef  MY_COUNT
#undef  MY_RATIO


//  Data = DL_REL_BASE + (HANDLE & 0xFFFFFF) * DL_DS_RATIO
//  This is how the above imported variables are used to find where data is in dram,
//  given the buffer handle

#ifndef DL_DROP_RING
    #define_eval DL_DROP_RING 0
    #warning "DL_DROP_RING undefined, using 0 as default."
#endif

#if (DL_DROP_RING > 11)
    #error          "For Ring Number Greater than 11, we cannot use the SCR_Ring#_Full input state. Check DL_DROP_RING"
#else
    #define_eval    DL_DROP_RING_FULL   SCR_Ring/**/DL_DROP_RING/**/_status
#endif

//  XXX - These are old names. Provided here for backward compatibility only.

#define dl_buffer_init          dl_buf_init
#define dl_buffer_alloc         dl_buf_alloc
#define dl_buffer_free          dl_buf_free
#define dl_drop_buffer          dl_buf_drop
#define dl_drop_buffer_chain    dl_buf_drop_chain


/** Initializes the buffering mechanism.
 * This macro should be called once before any of the other dl_buffer macros can be called.
 * Typically this macro will create all the freelist of buffers.
 *
//      Size:                   :
 */
#macro dl_buf_init()
    #ifdef USE_IMPORT_VAR
        .begin

            .reg buf_sram_base_reg
            .set buf_sram_base_reg      // XXX needs to be removed.

            immed32(buf_sram_base_reg, BUF_SRAM_BASE)
            buf_freelist_create(AVLBL_PKT_BUFFERS, BUF_FREE_LIST0, BUF_POOL)

        .end
    #else
        buf_freelist_create(AVLBL_PKT_BUFFERS, BUF_FREE_LIST0, BUF_POOL)
    #endif
#endm

/** Initializes the buffering mechanism.
 *
 * This macro should be called once before any of the other dl_buffer macros can be called.
 * Typically this macro will create all the freelist of buffers. Buffers
 *  are pushed to Ring Type specified.
 *
 * @param _NUM_BUFFERS_   Number of packet buffers
 * @param _PKT_BUF_BASE_  DRAM base address where packet buffers begin
 * @param _PKT_BUF_SIZE_  Packet buffer size
 * @param _RING_TYPE_     Ring type [CLS, DDR or QDR] on to which packet buffers are pushed
 * @param _IN_RING_NUM_   Ring number
 * @param _IN_BASE_ADDR_  Base address for ring number
 * @param _IN_SIZE_LW_    Size of the Ring in Long words
 *
//      Size
 */
#macro dl_buf_init_lifo(_NUM_BUFFERS_, _PKT_BUF_BASE_, _PKT_BUF_SIZE_, _RING_TYPE_, \
                            _IN_RING_NUM_, _IN_BASE_ADDR_, _IN_SIZE_LW_)
.begin

    .reg $pkt_buf[2]
    .reg tmp_pkt_buf
    .reg num_pkt_buffers
    .reg tmp_buf_size
    .sig dram_pkt_sig
    .xfer_order $pkt_buf

    #if (_PKT_BUF_BASE_ == 0)
        #error "Packet buffer DRAM base can not be zero! ring[get]/ring[pop] will cause a problem."
    #endif

    immed32(tmp_pkt_buf, _PKT_BUF_BASE_)
    immed32(tmp_buf_size, _PKT_BUF_SIZE_)
    immed32(num_pkt_buffers, 0)

    alu[$pkt_buf[0], --, b, tmp_pkt_buf]
    alu[$pkt_buf[1], --, b, 0x0]

    #if (streq('_RING_TYPE_', 'CLS'))
        ru_cs_ring_setup(_IN_RING_NUM_, _IN_BASE_ADDR_, _IN_SIZE_LW_)
        .while (num_pkt_buffers < _NUM_BUFFERS_)
            ru_enq_to_ring($pkt_buf, CLS_RING, 1, _IN_RING_NUM_, dram_pkt_sig, SIG_WAIT, PUT_BLIND)
            alu[num_pkt_buffers, num_pkt_buffers, +, 1]
            alu[tmp_pkt_buf, tmp_pkt_buf, +, tmp_buf_size]
            alu[$pkt_buf[0], --, b, tmp_pkt_buf]
        .endw
    #else
        #if (streq('_RING_TYPE_', 'DDR'))
            #ifdef DDR_Q_LOCALITY // MU_LOCALITY_HIGH, MU_LOCALITY_LOW, MU_LOCALITY_DIRECT_ACCESS, MU_LOCALITY_DISCARD_AFTER_READ
              ru_dram_ring_setup(_IN_RING_NUM_, _IN_BASE_ADDR_, _IN_SIZE_LW_, DDR_Q_LOCALITY, 0)
            #else
              ru_dram_ring_setup(_IN_RING_NUM_, _IN_BASE_ADDR_, _IN_SIZE_LW_)
            #endif

            .while (num_pkt_buffers < _NUM_BUFFERS_)
                ru_dram_ring_put($pkt_buf[0], _IN_RING_NUM_, 0, 1, dram_pkt_sig, SIG_WAIT)
                alu[num_pkt_buffers, num_pkt_buffers, +, 1]
                alu[tmp_pkt_buf, tmp_pkt_buf, +, tmp_buf_size]
                alu[$pkt_buf[0], --, b, tmp_pkt_buf]
            .endw

        #else
            #if (streq('_RING_TYPE_', 'QDR'))
                ru_sram_ring_setup(_IN_RING_NUM_, _IN_BASE_ADDR_, _IN_SIZE_LW_)
            #else
                #error "Ring Type must be one of: CLS/DDR/QDR." [_RING_TYPE_]
            #endif
        #endif
    #endif

.end
#endm

/** Allocates a free packet buffer.
 *
 * If no buffer is available will return 0 in buffer handle. There may be multiple pool of buffers
 * in which case the caller specifies the pool from which this buffer
 * needs to be allocated.
 *
 * @param buf_handle  Handle of the newly allocated buffer. SOP
 *                    and count fields will be set to 0. EOP Field
 *                    WILL be set to 1. buf_handle must be a SRAM
 *                    Transfer Register.
 * @param free_list   Specifies pool from which buffers are to be
 *                    allocated
 * @param req_sig     Signal to use in the I/O operation
 * @param sig_action  What to do with I/O operation:
 *                    @arg Wait for signal(s) as specified by sig_action
 *                    @arg Do not wait for signal, just return (SIG_NONE)
//      Size:                   :
 */
#macro dl_buf_alloc(buf_handle, free_list, req_sig, sig_action)
    #ifdef USE_IMPORT_VAR
        .begin

            .reg sram_qa_addr
            .set sram_qa_addr               // XXX - to be removed.


            // initialize the register

            immed[sram_qa_addr, 0]

            // get the channel number

            immed_w1[sram_qa_addr, ((BUF_SRAM_BASE>>16) & 0xc000)]

            // add the free list id to which to add the  buffer

            alu_shf[sram_qa_addr, sram_qa_addr, OR, free_list, <<24]

            // get the buffer handle from the free list

            sram[dequeue, buf_handle, sram_qa_addr, 0], sig_done[req_sig]

            // if required swap out and wait for dequeue to complete

            #if (!streq('sig_action', 'SIG_NONE'))
                ctx_arb[sig_action]
            #endif

        .end

    #else
        buf_alloc(buf_handle, free_list, BUF_POOL, req_sig, sig_action, --)
    #endif
#endm

/** Allocates a free packet buffer from ring.
 *
 * If no buffer is available will return 0 in buffer handle.
 *
 * @param buf_handle  Handle of newly allocated buffer. This will be 32-bit dram pointer.
 *                    Transfer register.
 * @param _RING_TYPE_ This should be one of: DDR/CLS/QDR where the packet buffers are stacked.
 * @param _RING_NUM_  DDR/CLS/QDR ring number. Must be constant.
 * @param _REF_CNT_   Number of buffers to pop from the stack. Constant.
 * @param req_sig     Signal to use in the I/O operation
 * @param sig_action  What to do with I/O operation:
 *                    @arg Wait for signal(s) as specified by sig_action
 *                    @arg Do not wait for signal, just return (SIG_NONE)
//      Size:                   :
 */
#macro dl_buf_alloc_lifo(buf_handle, _RING_TYPE_, _RING_NUM_, _REF_CNT_, sig_num, sig_action)
.begin

    #if (streq('_RING_TYPE_', 'CLS'))
        ru_cs_ring_pop_safe(buf_handle, _RING_NUM_, 0, _REF_CNT_, sig_num, sig_action)
    #else
        #if (streq('_RING_TYPE_', 'DDR'))
            ru_dram_ring_pop(buf_handle, _RING_NUM_, 0, _REF_CNT_, sig_num)
        #else
            #if (streq('_RING_TYPE_', 'QDR'))

            #else
                #error "Invalid Ring Type."
            #endif
        #endif
    #endif

.end
#endm

/** Frees the buffer that was previously allocated using dl_buffer_alloc.
 *
 * Only one buffer can be freed at a time by this macro. Chained buffers
 * (for big packets) are not supported by this macro.
 * EOP, SOP and cell count in the buffer handle will all be reset to zero
 * by this macro. So the user need not bother resetting them to zero before
 * freeing.
 *
 * @param buf_handle  Buffer handle
 * @param free_list   Specifies pool to which this buffer is to be released
 *
//      Size:           :
 */
#macro dl_buf_free(buf_handle, free_list)
    #ifdef USE_IMPORT_VAR
        .begin

            .reg __sram_addr, chan_num
            .set __sram_addr, chan_num

            // mask out the cell count and sop and eop bits from
            // the buffer before enqueuing.

            alu_shf[__sram_addr, buf_handle, AND~, 0xFF, <<24]      ; Clear out the upper bits.

            // get the channel number in bits 14 and 15

            immed_w0[chan_num, ((BUF_SRAM_BASE>>16)&0xc000)]

            // add the channel number to the sram offset.
            alu[__sram_addr, __sram_addr, OR, chan_num, <<16]

            // add the free list id to which to add the  buffer

            alu_shf[__sram_addr, __sram_addr, OR, free_list, <<24]

            // finally free the buffer

            sram[enqueue, --, __sram_addr, 0]

        .end
    #else
        buf_free(buf_handle, free_list, BUF_POOL)
    #endif
#endm

/** Given an opaq handle, this macro returns the sram address where the
 * meta data (called the Buffer descriptor) referred to by the buffer
 * handle is present.
 *
 * The lower 24 bits of the handle indicates the sram address.
 * Further arithmatic on this 24 bit content may or may not be required
 * depending on the implementation. (In this case no additional arithmatic
 * is required. The address returned is an absolute address and not an
 * offset from a given base address).
 *
 * @param sram_offset SRAM Byte Address where meta data for this buffer is present
 * @param buf_handle  Buffer handle
 *
 * @instruction_cnt 2 or 3
 */
#macro dl_buf_get_desc(sram_offset, buf_handle)
.begin

    // SRAM offset is in the lower 24 bits. The offset is in Long Words.

    #if ( is_ct_const(buf_handle))
        .local _temp_handle

            immed32(_temp_handle, buf_handle)
            alu_shf[sram_offset, _temp_handle, AND~, 0xFF, <<24]    ; Clear out the upper bits.

        .endlocal
    #else
        alu_shf[sram_offset, buf_handle, AND~, 0xFF, <<24]      ; Clear out the upper bits.
    #endif

        //  Buffer handle * 4 will give sram offset within a channel.
        //  Adding the channel nummber [31:30] will give absolute SRAM address.

        alu_shf[sram_offset, --, b, sram_offset, <<2]           ; Convert long word address to byte address

        //  Find the SRAM channel number.

    #ifdef  USE_IMPORT_VAR

        .reg    sram_ch

        //  The next two instructions are required only if the buffers are not in Channel 0.
        //  Because we patch symbols (BUF_SRAM_BASE) here, we cannot know this at compile
        //  time. So define BUF_SRAM_CHANNEL_NOT_ZERO if this is the case. This will save
        //  two instructions for channel 0 case.

        #ifdef  BUF_SRAM_CHANNEL_NOT_ZERO

            //  BUF_SRAM_BASE is a patched symbol. So an extra instruction (immed_w0)
            //  is required.

            immed[sram_ch, (BUF_SRAM_BASE>>30)]                 ; extract channel number [31:30].
            alu[sram_offset, sram_offset, OR, sram_ch, <<30]    ; set the channel number on the offset

        #endif  //  BUF_SRAM_CHANNEL_NOT_ZERO

    #else   //  USE_IMPORT_VAR

        //  BUF_SRAM_BASE is #defined (not a patched symbol).
        //  All #defines are treated as signed integers. So if MSB is set
        //  >>30 will produce -1. Hence & 0x3.

        #define_eval  CHANNEL_NUM  ((BUF_SRAM_BASE >> 30) & 0x3)

        #if (CHANNEL_NUM != 0)

            alu[sram_offset, sram_offset, OR, CHANNEL_NUM, <<30]    ; set the channel number on the offset

        #endif

        #undef  CHANNEL_NUM

    #endif  //  USE_IMPORT_VAR

.end
#endm


/** Calculate the DRAM address of a packet buffer by first determining
 * the effective index of the packet in the free list.
 *
 * This is less optimised than the Intel implementation, but it is also more
 * flexible in the sense that the DRAM buffer size does NOT need to
 * be a single power-of-2.
 *
 * @param out_sdram_offset    DRAM byte address for packet
 * @param in_buf_handle       Entry index
 *
 */
#macro dl_buf_get_data_index_method(out_sdram_offset, in_buf_handle)
.begin

    .reg    dbgdim_index
    .reg    dbgdim_temp

    // we need up to 26 valid bits of the SRAM address (64 MiB)
    #define_eval    _SRAM_WORD_BASE_    ((BUF_SRAM_BASE & (~(0x3F <<26))) >>2)

    // determine SRAM shift value, accounting for fact that buf handle is a word address
    #define_eval    _SRAM_SHIFT_VAL_    (LOG2(META_DATA_SIZE) - 2)

    alu[dbgdim_index, in_buf_handle, and~, 0xff, <<24]            ; clear out queue hardware bits
    move(dbgdim_temp, _SRAM_WORD_BASE_)
    alu[dbgdim_index, dbgdim_index, -, dbgdim_temp]            ; get rid of offset

    alu[dbgdim_index, --, b, dbgdim_index, >>_SRAM_SHIFT_VAL_]

    alu[out_sdram_offset, --, b, 0]

    #define_eval    _REMAINDER_     BUFFER_SIZE

    #while  (_REMAINDER_ > 0)
        #define_eval    _DRAM_SHIFT_VALUE_  LOG2(_REMAINDER_, -1)
        alu[dbgdim_temp, --, b, dbgdim_index, <<_DRAM_SHIFT_VALUE_]
        alu[out_sdram_offset, out_sdram_offset, +, dbgdim_temp]

        #define_eval    _REMAINDER_ (_REMAINDER_ - (1<<_DRAM_SHIFT_VALUE_))
    #endloop    /* _REMAINDER_ */
    #undef  _REMAINDER_
    #undef  _DRAM_SHIFT_VALUE_

    move(dbgdim_temp, BUF_SDRAM_BASE)
    alu[out_sdram_offset, out_sdram_offset, +, dbgdim_temp]


    #undef  _SRAM_WORD_BASE_
    #undef  _SRAM_SHIFT_VAL_

.end
#endm   /* dl_buf_get_data_index_method */


/** Given an opaq handle, this macro returns the sdram address where the
 * packet data for the buffer refered to by this handle is present.
 *
 * The lower 24 bits of the buffer handle contains the sram offset for
 * meta data. From this we can calculate the dram offset as follows:
 * @arg meta    = buf_handle & 0xffffff
 * @arg index   = (meta - sram base) / sizeof (meta data)
 * @arg data    = sdram base + index * size of packet buffer (in dram)
 *
 * However the above calculation is optimised as follows (grouping all
 * constants together):
 * @arg DL_DS_RATIO = Sizeof (Packet buffer) / Sizeof (Meta data)
 * @arg DL_REL_BASE = BUF_SDRAM_BASE - (BUF_SRAM_BASE * DL_DS_RATIO)
 * @arg Data = DL_REL_BASE + (HANDLE & 0xFFFFFF) * DL_DS_RATIO
 *
 * DL_DS_RATIO and DL_REL_BASE are constants which can be defined at
 * compile/load time so that Data = ...can be coded in two instructions.
 *
 * @param sdram_offset  SDRAM address where packet buffer for this
 *                      buffer is present
 * @param buf_handle    Buffer handle
 *
 * @instruction_cnt 2 to 4
 */
#macro dl_buf_get_data(sdram_offset, buf_handle)
    #ifdef NON_POWER_2_BUFFER

        dl_buf_get_data_index_method(sdram_offset, buf_handle)

    #else

        .begin

            .reg    sd

            //  First half of this function is similar to dl_meta_get_desc.

            // SRAM offset is in the lower 24 bits. The offset is in Long Words.

            #if ( is_ct_const(buf_handle))
                .local _temp_handle

                    immed32(_temp_handle, buf_handle)
                    alu_shf[sdram_offset, _temp_handle, AND~, 0xFF, <<24]   ; Clear out the upper bits.

                .endlocal
            #else

                alu_shf[sdram_offset, buf_handle, AND~, 0xFF, <<24]     ; Clear out the upper bits.

            #endif

            //  These two lines can be combined into a single instruction.
            //  That's why we unrolled dl_meta_get_desc here.

            //  alu_shf[sdram_offset, --, b, sdram_offset, <<2]             ; line from dl_buf_get_desc
            //  alu_shf[sdram_offset, --, b, sdram_offset, <<MULT_FACTOR]   ; line from this macro

            #define_eval    MF_PLUS_2   (MULT_FACTOR + 2)
            alu_shf[sdram_offset, --, b, sdram_offset, <<MF_PLUS_2]     ; two lines combined.


            // We add DL_REL_BASE only if it is > 0. Saves an instruction.

            #ifndef USE_IMPORT_VAR

                //  DL_REL_BASE is #defined. This allows us to save an instruction.

                #define_eval    TEMP_BUF_BASE       (BUF_SRAM_BASE & 0x3fffffff)
                #define_eval    DL_REL_BASE     BUF_SDRAM_BASE - (TEMP_BUF_BASE * DL_DS_RATIO)

                #if (DL_REL_BASE != 0)

                    move(sd, DL_REL_BASE)
                    alu[sdram_offset, sd, +, sdram_offset]

                #endif

                #undef TEMP_BUF_BASE
                #undef DL_REL_BASE

            #else /* USE IMPORT VAR */

                //  DL_REL_BASE is a patched symbol. So we cannot apply the usual preprocessing.

                #ifdef USE_BUF_FREE_LIST2

                    immed32(sd, DL_REL_BASE2)

                #else

                    immed32(sd, DL_REL_BASE)

                #endif

                alu[sdram_offset, sd, +, sdram_offset]

            #endif /* USE_IMPORT_VAR */

            #undef  MF_PLUS_2                               // clear name space

        .end

    #endif  /* NON_POWER_2_BUFFER */
#endm

/** This macro is the same as the dl_buf_get_data.
 *
 * Instead of taking a buffer handle as an input parameter, it takes the sram offset where the buffer meta
 * data is present (Calling dl_buf_get_desc(buf_handle) will provide that
 * sram offset). It saves a few instructions.
 *
 * @see dl_buf_get_data for full details.
 *
 * @param sdram_offset    SDRAM address where packet buffer for this
 *                        buffer is present
 * @param sram_offset     SRAM offset where meta data for this
 *                        buffer is present
 * @instruction_cnt 1 to 2
 */
#macro dl_buf_get_data_from_meta(sdram_offset, sram_offset)
.begin

    .reg    sd

    //  XXX - We should assume that the caller of this macro will have the
    //  channel bits cleared to 0!. Revisit here again.
    alu_shf[sram_offset, sram_offset, AND~, 0x3F, <<26] ; Clear out the upper bits of the _BYTE_ address

    alu_shf[sdram_offset, --, b, sram_offset, <<MULT_FACTOR]

    // We add DL_REL_BASE only if it is > 0. Saves an instruction.

    #ifndef USE_IMPORT_VAR

        //  DL_REL_BASE is #defined. This allows us to save an instruction.

        #define_eval    TEMP_BUF_BASE       (BUF_SRAM_BASE & 0x3fffffff)
        #define_eval    DL_REL_BASE         BUF_SDRAM_BASE - (TEMP_BUF_BASE * DL_DS_RATIO)

        #if (DL_REL_BASE != 0)

            move(sd, DL_REL_BASE)
            alu[sdram_offset, sd, +, sdram_offset]

        #endif

        #undef TEMP_BUF_BASE
        #undef DL_REL_BASE

    #else   //  USE_IMPORT_VAR

        //  DL_REL_BASE is a patched symbol. So we cannot apply the usual preprocessing.

        #ifdef USE_BUF_FREE_LIST2

            immed32(sd, DL_REL_BASE2)

        #else

            immed32(sd, DL_REL_BASE)

        #endif

        alu[sdram_offset, sd, +, sdram_offset]

    #endif  //  USE_IMPORT_VAR

.end
#endm


/** Drop the buffer.
 *
 * This is the same as freeing a buffer.
 *
 * @note Right now, we assume only one free list. When multiple free
 *       lists are used, the free list id may have to be passed in.
 *
 * @param buf_handle  Buffer handle. Buffer that needs to be dropped.
 */
#macro dl_buf_drop(buf_handle)
.begin

    #ifdef FREELIST_MANAGER

        .sig    sig_scr
        .reg    $scr
        .reg    ring

        #if (DROP_RING > 11)
            #error          "For Ring Number Greater than 11, we cannot use the SCR_Ring#_Full input state. Check DL_DROP_RING"
        #else
            #define_eval    DROP_RING_FULL  SCR_Ring/**/DROP_RING/**/_status
        #endif

    check_ring_full#:
        br_inp_state[DROP_RING_FULL, check_ring_full#]

        // Assume that any inter thread signaling is done after this

        alu[$scr, --, B, buf_handle]
        alu_shf[ring, --, b, DROP_RING, <<2]
        scratch[put, $scr, 0, ring, 1], ctx_swap[sig_scr]

    #else

        dl_buf_free(buf_handle, BUF_FREE_LIST0)

    #endif

.end
#endm


#macro dl_buf_drop(buf_handle, scr, sig_mask, sig_scr)
.begin

    #ifdef FREELIST_MANAGER

        .reg    ring

        #if (DROP_RING > 11)
            #error          "For Ring Number Greater than 11, we cannot use the SCR_Ring#_Full input state. Check DL_DROP_RING"
        #else
            #define_eval    DROP_RING_FULL  SCR_Ring/**/DROP_RING/**/_status
        #endif

    check_ring_full#:
        br_inp_state[DROP_RING_FULL, check_ring_full#]

        alu[scr[0], --, B, buf_handle]
        alu[ring, --, b, DROP_RING, <<2]
        scratch[put, scr[0], 0, ring, 1], sig_done[sig_scr]
        alu[sig_mask, sig_mask, OR, 1, <<&sig_scr]

    #else

        dl_buf_free(buf_handle, BUF_FREE_LIST0)

    #endif

.end
#endm

/** Drop a chain of buffers.
 *
 * A ctx_swap[] is performed. Care must be taken when calling this macro to ensure threads do not go out of order
 * after executing the ctx_swap.
 *
 * @param buf_handle      (sop) Buffer handle to drop
 * @param buf_eop_handle  Eop buffer handle to drop. All buffers in the chain
 *                        including these two will be dropped.
 *
 */
#macro dl_buf_drop_chain(buf_handle, buf_eop_handle)
.begin

    #if (DL_DROP_RING > 11)
        #error "For ring number greater than 11, we cannot use the SCR_Ring#_Full \
            input state. Check DL_DROP_RING"
    #else
        #define_eval    TMP                 DL_DROP_RING
        #define_eval    DL_DROP_RING_FULL   SCR_Ring/**/TMP/**/_status
        #undef TMP
    #endif

    .reg    queue_number
    .reg    ring
    .sig    sig_scr

check_ring_full#:

    br_inp_state[DL_DROP_RING_FULL, check_ring_full#]

     ; At this point ring has enough space. Write the data.

    #ifdef POTS
        xbuf_alloc($scr, 4, write)
    #else
        xbuf_alloc($scr, 3, write)
    #endif

    #if defined(IX_PLATFORM_2801) && defined(DROP_MSG_FORMAT_2400)

        // Some applications on IX_PLATFORM_2801 uses old message format for QM
        move($scr[0], buf_handle)               ; LW0 on the ring is current buffer handle
        move($scr[1], buf_eop_handle)           ; LW1 on the ring is the last buffer in the chain,
                                                ; for big pkts only. Otherwise IX_NULL.
        move(queue_number, QM_DROP_QUEUE)       ; LW2 on the ring is drop queue number.

        alu[$scr[2], queue_number, OR, 1, <<31] ; The msb is always set to 1, to prevent producing
                                                ; a value of 0 on to the ring.
        #else //original code

        // For the IXP2800 reference designs, the scratch ring format is different.
        // We add a switch to handle this.

        move($scr[0], QM_DROP_QUEUE)            ; LW0 on the ring is drop queue number.
        move($scr[1], buf_handle)               ; LW1 on the ring is current buffer handle
        move($scr[2], buf_eop_handle)           ; LW2 on the ring is the last buffer in the chain,
                                                ; for big pkts only. Otherwise IX_NULL.

    #endif //defined(IX_PLATFORM_2801) && defined(DROP_MSG_FORMAT_2400)

    alu[ring, --, b, DL_DROP_RING, <<2]

    #ifdef POTS
        move($scr[3], seq_num)
        scratch[put, $scr[0], 0, ring, 4], ctx_swap[sig_scr]    ; Produce 4 long words on the ring
    #else
        scratch[put, $scr[0], 0, ring, 3], ctx_swap[sig_scr]    ; Produce 3 long words on the ring
    #endif

    xbuf_free($scr)

.end
#endm


/** Drop a chain of buffers.
 *
 * This is almost identical to dl_buf_drop_chain().
 * The only is that instead of doing a ctx_swap[] when writing to the
 * scratch ring, it simply adds the signal to a signal mask. This makes
 * using the macro safer to use since we do not ctx_swap[].
 *
 * @param buf_handle      (sop) Buffer handle to drop
 * @param buf_eop_handle  Eop buffer handle to drop. All buffers in the chain
 *                        including these two will be dropped.
 * @param scr             SRAM transfer registers used for scratch[put]
 * @param sig_mask        Signal mask to add the scratch[put] signal
 * @param sig_scr         Signal used for scratch[put]
 *
 */
#macro dl_buf_drop_chain(buf_handle, buf_eop_handle, scr, sig_mask, sig_scr)
.begin

    #if (DL_DROP_RING > 11)
        #error "For ring number greater than 11, we cannot use the SCR_Ring#_Full \
            input state. Check DL_DROP_RING"
    #else
        #define_eval    TMP                 DL_DROP_RING
        #define_eval    DL_DROP_RING_FULL   SCR_Ring/**/TMP/**/_status
        #undef TMP
    #endif

    .reg    queue_number
    .reg    ring

check_ring_full#:

    br_inp_state[DL_DROP_RING_FULL, check_ring_full#]

    ; At this point ring has enough space. Write the data.

    #if defined(IX_PLATFORM_2801) && defined(DROP_MSG_FORMAT_2400)

        // Some applications on IX_PLATFORM_2801 uses old message format for QM
        move(scr[0], buf_handle)                ; LW0 on the ring is current buffer handle
        move(scr[1], buf_eop_handle)            ; LW1 on the ring is the last buffer in the chain,
                                                ; for big pkts only. Otherwise IX_NULL.
        move(queue_number, QM_DROP_QUEUE)       ; LW2 on the ring is drop queue number.

        alu[scr[2], queue_number, OR, 1, <<31]  ; The msb is always set to 1, to prevent producing
                                                ; a value of 0 on to the ring.
        #else //original code

        // For the IXP2800 reference designs, the scratch ring format is different.
        // We add a switch to handle this.

        move(scr[0], QM_DROP_QUEUE)             ; LW0 on the ring is drop queue number.
        move(scr[1], buf_handle)                ; LW1 on the ring is current buffer handle
        move(scr[2], buf_eop_handle)            ; LW2 on the ring is the last buffer in the chain,
                                                ; for big pkts only. Otherwise IX_NULL.

    #endif //defined(IX_PLATFORM_2801) && defined(DROP_MSG_FORMAT_2400)

    alu[ring, --, b, DL_DROP_RING, <<2]

    #ifdef POTS
        move(scr[3], seq_num)
        scratch[put, scr[0], 0, ring, 4], sig_done[sig_scr] ; Produce 4 long words on the ring
    #else
        scratch[put, scr[0], 0, ring, 3], sig_done[sig_scr] ; Produce 3 long words on the ring
    #endif

    alu[sig_mask, sig_mask, OR, 1, <<&sig_scr]

.end
#endm


/** Sets the SOP bit in the dl_buf_handle.
 *
 * It indicates that this buffer has the SOP (Start of Packet).
 *
 * @instruction_cnt 1
 */
#macro dl_buf_set_SOP(buf_handle)
.begin

    alu_shf[buf_handle, buf_handle, OR, 1, <<30]        ; bit 30 is SOP bit

.end
#endm

/** Sets the EOP bit in the dl_buf_handle.
 *
 * It indicates that this buffer has the EOP (End of Packet).
 *
 * @instruction_cnt 1
 */
#macro dl_buf_set_EOP(buf_handle)
.begin

    alu_shf[buf_handle, buf_handle, OR, 1, <<31]        ; bit 30 is EOP bit

.end
#endm

/** Sets the cellcount in the dl_buf_handle.
 *
 * It indicates the number of cells in this buffer.
 *
 * @note It can be assumed that bits [29:24] in dl_buf_handle will already
 *       be zero [dl_buf_alloc guarantees that]. So one
 *       instruction can be saved by avoiding to clear these bits to zero before setting
 *       them to cell count.
 *
 * @instruction_cnt 2
 */
#macro dl_buf_set_cell_count(buf_handle, cell_count, BITS_CLEAR)
.begin

    #if (!streq('bits_clear', 'DONT_CLEAR'))
        bits_clr(buf_handle, 24, 0x3F)                              ; clear bits [29:24]
    #endif

    alu_shf[buf_handle, buf_handle, OR, cell_count, <<24]       ; and then set it in dl_buf_handle

.end
#endm


/** Gets the cellcount in the dl_buf_handle.
 *
 * It indicates the number of cells in this buffer.
 *
 * @param cell_count  Cell count in buf_handle[29:24]
 *
 * @instruction_cnt 2
 */
#macro dl_buf_get_cell_count(buf_handle, cell_count)
.begin

    .reg temp_handle

    alu[temp_handle, --, B, buf_handle]                             ; Moving the buf_handle
                                                                    ; into a temp register
    alu[cell_count, 0x3F, AND, temp_handle, >>24]                   ; buf_handle[29:24]

.end
#endm



#ifdef  ENABLE_DROP_RING_SUPPORT

/** This macro should be used to produce dropped packets onto a drop ring.
 *
 * This ring is later consumed by another block (typically packet TX) which
 * in turn frees all buffers associated with the packet.
 *
 * @param in_handle          Sop buffer handle to drop
 * @param in_scr             SRAM transfer registers used for scratch[put]
 * @param in_sigmask         Signal mask to add the scratch[put] signal
 * @param in_sig             Signal used for scratch[put]
 * @param RING_FULL_ACTION   If SIG_NONE, then upon ring full, poll on ring to go
 *                           non-full. Otherwise, branch to this label.
 * @param WAIT_ACTION        If SIG_NONE, exit this macro. Otherwise wait for I/O
 *                           to finish.
 *
 */
#macro dl_buf_send_to_drop_ring(in_handle, in_scr, \
            in_sigmask, in_sig, RING_FULL_ACTION, WAIT_ACTION)
.begin

    .reg    drop_ring

    alu_shf[drop_ring, --, B, SYSTEM_DROP_PKT_RING, <<2]
    alu[in_scr, --, B, in_handle]

    #define_eval    DROP_PKT_RING_FULL  SCR_Ring/**/SYSTEM_DROP_PKT_RING/**/_Status

    #if (streq('RING_FULL_ACTION', 'SIG_NONE'))

    check_drop_ring#:

        br_inp_state[DROP_PKT_RING_FULL, drop_ring_full#]

        br[continue#]

    drop_ring_full#:

        nop
        br[check_drop_ring#]

    continue#:

    #else

        br_inp_state[DROP_PKT_RING_FULL, RING_FULL_ACTION]

    #endif

    #undef  DROP_PKT_RING_FULL

    scratch[put, in_scr, drop_ring, 0, 1], sig_done[in_sig]

    // add signal to sigmask

    alu_shf[in_sigmask, in_sigmask, OR, 1, <<&in_sig]

    #if (!(streq('WAIT_ACTION', 'SIG_NONE')))

        ctx_arb[--], defer[1]
        local_csr_wr[active_ctx_wakeup_events, in_sigmask]

    #endif

.end
#endm

#endif  // ENABLE_DROP_RING_SUPPORT


/** @}
 * @}
 */

#endif /* __DL_BUF_UC__ */
