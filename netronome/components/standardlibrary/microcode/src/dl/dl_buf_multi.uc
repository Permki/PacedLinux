/*
 * Copyright (C) 2009-2011 Netronome Systems, Inc.  All rights reserved.
 */

#ifndef __DL_BUF_MULTI_UC__
#define __DL_BUF_MULTI_UC__

#ifndef NFP_LIB_ANY_NFAS_VERSION
    #if (!defined(__NFP_TOOL_NFAS) ||                       \
        (__NFP_TOOL_NFAS < NFP_SW_VERSION(5, 0, 0, 0)) ||   \
        (__NFP_TOOL_NFAS > NFP_SW_VERSION(6, 1, 0, 255)))
        #error "This standard library is not supported for the version of the SDK currently in use."
    #endif
#endif

#include <buf.uc>
#include "dl_system.h"


/** @file dl_buf_multi.uc Packet Buffer Utility Macros for Multiple Buffer Freelists
 * @addtogroup dl_buf_multi Packet Buffer Utility Macros for Multiple Buffer Freelists
 * @{
 *
 * @name Packet Buffer Utility Macros for Multiple Buffer Freelists
 * @{
 *
 */


/** Initializes the buffering mechanism.
 *
 * This macro should be called once before any of the other dl_buf macros are called.
 * Typically this macro will create a buffer freelist.
 *
 * @param freelist_id   id of the buffer freelist to create (freelist_id must be a constant)
// Size:                :
 *
 */
#macro dl_buf_init(freelist_id)
.begin

    #if (!is_ct_const(freelist_id))
        #error "freelist_id must be a constant."
    #endif

    // Compute number of packet buffers.

    #define_eval _NO_OF_PKT_BUFFERS BUF_SRAM_SIZE_/**/freelist_id/**/ / META_DATA_SIZE_/**/freelist_id/**/

    // Sanity check. Check if the specified SDRAM memory is of suffcient size
    // to hold all buffers.

    #define_eval _SDRAM_BUFFERS     BUF_SDRAM_SIZE_/**/freelist_id/**/ / BUFFER_SIZE_/**/freelist_id/**/

    #if (_SDRAM_BUFFERS != _NO_OF_PKT_BUFFERS)
        #error  "The amount of memory allocated in SRAM for packet buffers doesn't \
            match with the SDRAM size. Check BUF_SRAM_SIZE_/**/freelist_id/**/ \
            and BUF_SDRAM_SIZE_/**/freelist_id/**/. Also check \
            META_DATA_SIZE_/**/freelist_id/**/ and BUFFER_SIZE_/**/freelist_id/**/."
    #endif

    #undef  _SDRAM_BUFFERS


    // Specifying a base of address of 0 for SRAM (and DRAM) simplifies the
    // arithmetic required to translate the handle to offset. It saves one
    // instruction. (See dl_buf_get_desc). However, dl_buf_alloc will return 0
    // for a null buffer (i.e no buffer available) and it may not be possible
    // to differentiate between a null buffer and a buffer at address 0.
    // So we simply make that one buffer at address 0 unavailable for
    // allocation/freeing, yet taking advantage of using a base address of 0.
    // We end up wasting a little memory (1 buffer) but that's ok.

    // If USE_IMPORT_VAR is defined BUF_SRAM_BASE_/**/freelist_id/**/ and
    // BUF_SDRAM_BASE_/**/freelist_id/**/ are patched symbols.So we need to use
    // gpr's to modify these values.
    // If USE_IMPORT_VAR is not defined we can modify these values using
    // #define_eval which is more efficient. But since this is init code, for
    // simplicity we can use gpr's in this case too.

    .reg _buf_sram_base _buf_sdram_base

    move(_buf_sram_base, BUF_SRAM_BASE_/**/freelist_id/**/)
    move(_buf_sdram_base, BUF_SDRAM_BASE_/**/freelist_id/**/)

    .if (_buf_sram_base==0x00000000 || _buf_sram_base==0x40000000 || \
        _buf_sram_base==0x80000000 || _buf_sram_base==0xC0000000)

        .reg _meta_data_size _buffer_size

        immed32(_meta_data_size, META_DATA_SIZE_/**/freelist_id/**/)
        immed32(_buffer_size, BUFFER_SIZE_/**/freelist_id/**/)

        alu[_buf_sram_base, _buf_sram_base, +, _meta_data_size]
        alu[_buf_sdram_base, _buf_sdram_base, +, _buffer_size]


        #ifdef USE_IMPORT_VAR
            #define_eval _NO_OF_PKT_BUFFERS _NO_OF_PKT_BUFFERS - 1
        #endif

    .endif

    buf_freelist_create(_NO_OF_PKT_BUFFERS, BUF_FREE_LIST_/**/freelist_id/**/,
                        _buf_sdram_base, BUFFER_SIZE_/**/freelist_id/**/,
                        _buf_sram_base, META_DATA_SIZE_/**/freelist_id/**/)

    #undef _NO_OF_PKT_BUFFERS

.end
#endm


/** Allocates an available packet buffer from the specified freelist.
 *
 * If no buffer is available, it returns 0.
 *
 * @param buf_handle        Handle of the newly allocated buffer [bits 23:0].
 *                          buf_handle must be a SRAM transfer register.
 * @param freelist_id       id of buffer freelist from which to allocate
 *                          a buffer (freelist_id must be a constant)
 * @param req_sig           Signal to use in dequeue operation
 * @param sig_action        What to do with during the dequeue operation:
 *                          @arg Wait for signal(s) as specified by sig_action
 *                          @arg Do not wait for signal, just return (SIG_NONE)
 * @param FREELIST_ID_TYPE  This field is added to differentiate this macro from
 *                          the macro in dl_buf.uc which takes in the q-array entry
 *                          number as an argument. This macro takes in the
 *                          as an argument. FREELIST_ID_TYPE is not used in
 *                          freelist_id this macro; its only purpose is to create
 *                          a unique signature for this macro.
 *
 * @instruction_cnt 3 - 4
 */
#macro dl_buf_alloc(buf_handle, freelist_id, req_sig, sig_action, FREELIST_ID_TYPE)
.begin

    #if (!is_ct_const(freelist_id))
        #error "freelist_id must be a constant."
    #endif

    .reg _freelist_id, _channel_number, temp_var

    ; Get the channel number from which to fetch the buffer.
    immed32(temp_var, BUF_SRAM_BASE_/**/freelist_id/**/)
    alu[_channel_number, temp_var, AND, 0x3, <<30]

    //  immed32(_channel_number, (BUF_SRAM_BASE_/**/freelist_id/**/ & (0x3 << 30)))

    ; Get the freelist id from which to fetch the buffer.

    alu[_freelist_id, --, B, BUF_FREE_LIST_/**/freelist_id/**/, <<24]

    ; Fetch the buffer handle from the free list.

    sram[dequeue, buf_handle, _freelist_id, _channel_number], sig_done[req_sig]

    ; If required swap out and wait for the dequeue to complete.

    #if (!streq('sig_action', 'SIG_NONE'))
        ctx_arb[sig_action]
    #endif

.end
#endm


/** Frees the buffer that was previously allocated using dl_buf_alloc().
 *
 * Only one buffer can be freed at a time by this macro. Chained buffers
 * (for large packets) are not supported by this macro.
 * EOP, SOP and cell count in the buffer handle will all be reset to zero
 * by this macro. So the user need not reset them before calling this macro.
 *
 * @param buf_handle        Buffer handle to free
 * @param freelist_id       id of the buffer freelist to which the buffer
 *                          will be freed (freelist_id must be a constant)
 * @param FREELIST_ID_TYPE  This field is added to differentiate this macro from
 *                          the macro in dl_buf.uc which takes in the q-array entry
 *                          number as an argument. This macro takes in the
 *                          freelist_id as an argument. FREELIST_ID_TYPE is not
 *                          used in this macro; its only purpose is to create a
 *                          unique signature for this macro.
 *
 * @instruction_cnt 4 - 5
 */
#macro dl_buf_free(buf_handle, freelist_id, FREELIST_ID_TYPE)
.begin

    #if (!is_ct_const(freelist_id))
        #error "freelist_id must be a constant."
    #endif

    .reg _channel_number _sram_addr

    ; Mask out the cell count and sop and eop bits from the buffer handle before enqueuing.

    alu[_sram_addr, buf_handle, AND~, 0xFF, <<24]

    #ifdef CHANNEL_NUMBER_IN_BUFFER_HANDLE

        ; Mask out the channel number from the buffer handle before enqueuing.

        alu[_sram_addr, _sram_addr, AND~, 0x6]

    #endif

    ; Get the channel number on which to enqueue the buffer.

    .reg temp_var

    immed32(temp_var, BUF_SRAM_BASE_/**/freelist_id/**/)
    alu[_channel_number, temp_var, AND, 0x3, <<30]

    //immed32(_channel_number, (BUF_SRAM_BASE_/**/freelist_id/**/ & (0x3 << 30)))

    ; Set the freelist id on which to enqueue the buffer.

    alu[_sram_addr, _sram_addr, OR, BUF_FREE_LIST_/**/freelist_id/**/, <<24]

    ; Set the channel number on which to enqueue the buffer and free the buffer.

    sram[enqueue, --, _sram_addr, _channel_number]

.end
#endm


/** Given a buffer handle, this macro returns the sram address where the
 * meta-data (called the buffer descriptor) for this buffer handle
 * is present.
 *
 * Bits 23:0 of the handle contain the sram address.
 * The address returned is an absolute byte address and not an offset
 * from a given base address.
 *
 * @param sram_offset   SRAM byte address [23:0] and SRAM channel number [31:30]
 *                      where the meta data for this buffer is present
 * @param buf_handle    Buffer handle
 * @param freelist_id   id of the buffer freelist from which to allocate a
 *                      buffer (freelist_id must be a constant)
 *
 * @instruction_cnt 2 - 6
 */
#macro dl_buf_get_desc(sram_offset, buf_handle, freelist_id)
.begin

    #if (!is_ct_const(freelist_id))
        #error "freelist_id must be a constant."
    #endif

    ; Clear out bits 31:24 of buf_handle.

    #if (is_ct_const(buf_handle))

        .reg _temp_handle

        immed32(_temp_handle, buf_handle)
        alu[sram_offset, _temp_handle, AND~, 0xFF, <<24]

    #else

        alu[sram_offset, buf_handle, AND~, 0xFF, <<24]

    #endif  // (is_ct_const(buf_handle))

    #ifdef CHANNEL_NUMBER_IN_BUFFER_HANDLE

        ; Mask out the channel number.

        alu[sram_offset, sram_offset, AND~, 0x6]

    #endif

    ; Convert long word address to byte address.
    ; sram_offset * 4 will give sram byte offset within a channel.

    alu[sram_offset, --, B, sram_offset, <<2]

    ; Set the channel number in sram_offset.

    #ifdef  USE_IMPORT_VAR

        // BUF_SRAM_BASE_/**/freelist_id/**/ is a patched symbol.
        .reg _channel_number temp_var

        immed32(temp_var, BUF_SRAM_BASE_/**/freelist_id/**/)
        alu[_channel_number, 0x3, AND , temp_var, >>30]
        // immed32(_channel_number, ((BUF_SRAM_BASE_/**/freelist_id/**/ >> 30) & 0x3))

        alu[sram_offset, sram_offset, OR, _channel_number, <<30]

    #else   // USE_IMPORT_VAR

        #define_eval  CHANNEL_NUMBER  ((BUF_SRAM_BASE_/**/freelist_id/**/ >> 30) & 0x3)

        #if (CHANNEL_NUMBER != 0)

            alu[sram_offset, sram_offset, OR, CHANNEL_NUMBER, <<30]

        #endif  // CHANNEL_NUMBER

        #undef  CHANNEL_NUMBER

    #endif // USE_IMPORT_VAR

.end
#endm

/** Given a buffer handle, this macro returns the sram address where the
 * meta-data (called the buffer descriptor) for this buffer handle
 * is present.
 *
 * Bits 23:0 of the handle contain the sram address.
 * The address returned is an absolute byte address and not an offset
 * from a given base address. This macro also returns meta-data address
 * for the split channel also.
 *
 * @param sram_addr    SRAM byte address [23:0] and SRAM channel number [31:30]
 *                     where the meta data for this buffer is present
 * @param sram_addr1   This output variable name is derived from 'sram_addr'. This
 *                     variable stores SRAM byte address [23:0] and SRAM channel
 *                     number [31:30] where rest of the meta data is stored (after
 *                     the split).
 * @param buf_handle   Buffer handle
 * @param freelist_id  id of the buffer freelist from which to allocate a
 *                     buffer (freelist_id must be a constant)
 *
 * @instruction_cnt 4 - 8
 *
 * @note This macro is called only when the meta data is split.
 */
#macro dl_buf_get_desc_split(sram_addr, sram_addr1, buf_handle, freelist_id)
    dl_buf_get_desc(sram_addr, buf_handle, freelist_id)

    #define_eval _DL_SPLIT_META_DATA        DL_SPLIT_META_DATA_/**/freelist_id/**/

    #if (_DL_SPLIT_META_DATA == TRUE)

        ; Clear the channel number
        alu[sram_addr1, sram_addr, AND~, 3, <<30]

        ; Set the channel number
        alu[sram_addr1, sram_addr1, OR, BUF_SRAM_SPLIT_CHANNEL_/**/freelist_id/**/, <<30]

    #else

        #error  "This macro can be called only when meta-data is split"

    #endif

    #undef  _DL_SPLIT_META_DATA
#endm

/** Given a buffer handle, this macro returns the sdram address where the
 * packet data for this buffer handle is present.
 *
 * Bits 23:0 of the handle contain the sram address. The corresponding dram address is computed by
 * multiplying the buffer handle address by a fixed ratio and adding the
 * dram base address to the result.
 *
 * @param sdram_offset SDRAM byte address where the packet data for this buffer
 *                     is present
 * @param buf_handle   Buffer handle
 * @param freelist_id  id of the buffer freelist this buffer belongs to
 *                     (freelist_id must be a constant)
 *
 * @instruction_cnt 2 - 6
 */
#macro dl_buf_get_data(sdram_offset, buf_handle, freelist_id)
.begin

    #if (!is_ct_const(freelist_id))
        #error "freelist_id must be a constant."
    #endif

    .reg _dl_rel_base

    ; Clear out bits 31:24 of buf_handle.

    #if (is_ct_const(buf_handle))

        .reg _temp_handle

        immed32(_temp_handle, buf_handle)
        alu[sdram_offset, _temp_handle, AND~, 0xFF, <<24]

    #else

        alu[sdram_offset, buf_handle, AND~, 0xFF, <<24]

    #endif  // (is_ct_const(buf_handle))

    #ifdef CHANNEL_NUMBER_IN_BUFFER_HANDLE

        ; Mask out the channel number.

        alu[sdram_offset, sdram_offset, AND~, 0x6]

    #endif

    ; Compute SDRAM offset.

    #define_eval    _DL_DS_RATIO        (BUFFER_SIZE_/**/freelist_id/**/ / META_DATA_SIZE_/**/freelist_id/**/)
    #define_eval    _MULT_FACTOR        LOG2(_DL_DS_RATIO)
    #define_eval    _MULT_FACTOR_PLUS_2 (_MULT_FACTOR + 2)

    alu[sdram_offset, --, B, sdram_offset, <<_MULT_FACTOR_PLUS_2]


    ; Add SDRAM base address (i.e DL_REL_BASE) to the computed SDRAM offset.

    #ifdef USE_IMPORT_VAR

        // DL_REL_BASE is a patched symbol. So simply use it.

        immed32(_dl_rel_base, DL_REL_BASE/**/freelist_id/**/ )
        alu[sdram_offset, _dl_rel_base, +, sdram_offset]

    #else // USE_IMPORT_VAR

        // Compute DL_REL_BASE.

        #define_eval    _TEMP_BUF_BASE      (BUF_SRAM_BASE_/**/freelist_id/**/ & 0x3FFFFFFF)
        #define_eval    _TEMP_DL_REL_BASE   (BUF_SDRAM_BASE_/**/freelist_id/**/ - (_TEMP_BUF_BASE * _DL_DS_RATIO))

        #if (_TEMP_DL_REL_BASE != 0)

            immed32(_dl_rel_base, _TEMP_DL_REL_BASE)
            alu[sdram_offset, _dl_rel_base, +, sdram_offset]

        #endif // (_DL_REL_BASE != 0)

        #undef _TEMP_DL_REL_BASE
        #undef _TEMP_BUF_BASE

    #endif // USE_IMPORT_VAR


    #undef _MULT_FACTOR_PLUS_2
    #undef _MULT_FACTOR
    #undef _DL_DS_RATIO

.end
#endm


/** This macro is almost identical to dl_buf_get_data().
 *
 * Instead of taking a buffer handle as an input parameter, it takes the sram offset where
 * the buffer meta data is present (Calling dl_buf_get_desc() will give you
 * that sram offset). It saves a couple of instructions.
 * @see dl_buf_get_data() for more details.
 *
 * @param sdram_offset   SDRAM byte address where the packet data for this buffer
 *                       is present
 * @param sram_offset    SRAM offset where the meta data for this buffer is
 *                       present
 * @param freelist_id   id of the buffer freelist this buffer belongs to
 *                      (freelist_id must be a constant)
 *
 * @instruction_cnt 2 - 6
 */
#macro dl_buf_get_data_from_meta(sdram_offset, sram_offset, freelist_id)
.begin

    #if (!is_ct_const(freelist_id))
        #error "freelist_id must be a constant."
    #endif

    .reg _dl_rel_base

    ; Clear out bits 31:24 of buf_handle.

    #if (is_ct_const(sram_offset))

        .reg _temp_handle

        immed32(_temp_handle, sram_offset)
        alu[sdram_offset, _temp_handle, AND~, 0xFF, <<24]

    #else

        alu[sdram_offset, sram_offset, AND~, 0xFF, <<24]

    #endif  // (is_ct_const(buf_handle))

    #ifdef CHANNEL_NUMBER_IN_BUFFER_HANDLE

        ; Mask out the channel number.

        alu[sdram_offset, sdram_offset, AND~, 0x6]

    #endif

    ; Compute SDRAM offset.

    #define_eval    _TMP_DL_DS_RATIO        (BUFFER_SIZE_/**/freelist_id/**/ / META_DATA_SIZE_/**/freelist_id/**/)
    #define_eval    _TMP_MULT_FACTOR        LOG2(_TMP_DL_DS_RATIO)

    alu[sdram_offset, --, B, sdram_offset, <<_TMP_MULT_FACTOR]


    ; Add SDRAM base address (i.e DL_REL_BASE) to the computed SDRAM offset

    #ifdef USE_IMPORT_VAR

        // DL_REL_BASE is a patched symbol. So simply use it.

        immed32(_dl_rel_base, DL_REL_BASE/**/freelist_id/**/)
        alu[sdram_offset, _dl_rel_base, +, sdram_offset]

    #else // USE_IMPORT_VAR

        // Compute DL_REL_BASE.

        #define_eval    _TEMP_BUF_BASE       (BUF_SRAM_BASE_/**/freelist_id/**/ & 0x3FFFFFFF)
        #define_eval    _TMP_DL_REL_BASE    (BUF_SDRAM_BASE_/**/freelist_id/**/ - (_TEMP_BUF_BASE * _TMP_DL_DS_RATIO))

        #if (_TMP_DL_REL_BASE != 0)

            immed32(_dl_rel_base, _TMP_DL_REL_BASE)
            alu[sdram_offset, _dl_rel_base, +, sdram_offset]

        #endif // (DL_REL_BASE != 0)

        #undef _TMP_DL_REL_BASE
        #undef _TEMP_BUF_BASE

    #endif // USE_IMPORT_VAR

    #undef _TMP_MULT_FACTOR
    #undef _TMP_DL_DS_RATIO

.end
#endm


/** Drop one buffer.
 *
 * This is identical to freeing the buffer. For freeing a
 * chain of buffers, use dl_buf_drop_chain(). If FREELIST_MANAGER is defined
 * the buffer handle is instead written to a scratch ring that is read by
 * the freelist manager microblock. In this case, we do a ctx_swap[].
 * Care must be taken when calling this macro to ensure threads do not go
 * out of order after executing the ctx_swap.
 *
 * @param buf_handle    Buffer handle to drop
 * @param freelist_id   id of the buffer freelist to drop to.
 *                      (freelist_id must be a constant).
 *                      Unused if FREELIST_MANAGER is defined.
 *
// Size:
 */
#macro dl_buf_drop(buf_handle, freelist_id)
.begin

    #ifdef FREELIST_MANAGER

        #if (DROP_RING > 11)
            #error "For ring number greater than 11, we cannot use the SCR_Ring#_Full \
                input state. Check DROP_RING"
        #else
            #define_eval    DROP_RING_FULL  SCR_Ring/**/DROP_RING/**/_status
        #endif

        .reg $scr
        .reg ring
        .sig sig_scr

    check_ring_full#:

        br_inp_state[DROP_RING_FULL, check_ring_full#]

        ; Assume that any inter thread signaling is done after this.

        alu[$scr, --, B, buf_handle]
        alu[ring, --, b, DROP_RING, <<2]
        scratch[put, $scr, 0, ring, 1], ctx_swap[sig_scr]

    #else   // FREELIST_MANAGER

        dl_buf_free(buf_handle, freelist_id, --)

    #endif // FREELIST_MANAGER

.end
#endm


/** Drop one buffer.
 *
 * This is almost identical to dl_buf_drop().
 *
 * The only difference is that if FREELIST_MANAGER is defined, instead of doing a
 * ctx_swap[] when writing to the scratch ring, it simply adds the signal
 * to a signal mask. This makes using the macro safer to use since ctx_swap[] is not done.
 *
 * @param buf_handle   Buffer handle to drop
 * @param freelist_id  id of the buffer freelist to drop to.
 *                     (freelist_id must be a constant).
 *                     Unused if FREELIST_MANAGER is defined.
 * @param scr          SRAM transfer registers used for scratch[put].
 *                     Unused if FREELIST_MANAGER is not defined.
 * @param sig_mask     Signal mask to add the scratch[put] signal.
 *                     Unused if FREELIST_MANAGER is not defined.
 * @param sig_scr      Signal used for scratch[put].
 *                     Unused if FREELIST_MANAGER is not defined.
 *
// Size:
 */
#macro dl_buf_drop(buf_handle, freelist_id, scr, sig_mask, sig_scr)
.begin

    #ifdef FREELIST_MANAGER

        #if (DROP_RING > 11)
            #error "For ring number greater than 11, we cannot use the SCR_Ring#_Full \
                input state. Check DROP_RING"
        #else
            #define_eval    DROP_RING_FULL  SCR_Ring/**/DROP_RING/**/_status
        #endif

        .reg ring

    check_ring_full#:

        br_inp_state[DROP_RING_FULL, check_ring_full#]

        alu[scr[0], --, B, buf_handle]
        alu[ring, --, B, DROP_RING, <<2]
        scratch[put, scr[0], 0, ring, 1], sig_done[sig_scr]
        alu[sig_mask, sig_mask, OR, 1, <<&sig_scr]

    #else   // FREELIST_MANAGER

        dl_buf_free(buf_handle, freelist_id, -- )

    #endif  // FREELIST_MANAGER

.end
#endm


/** @}
 * @}
 */

#endif /* __DL_BUF_MULTI_UC__ */
