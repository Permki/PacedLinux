/*
 * Copyright (C) 2009-2011 Netronome Systems, Inc.  All rights reserved.
 */

#ifndef __DL_META_MULTI_UC__
#define __DL_META_MULTI_UC__

#ifndef NFP_LIB_ANY_NFAS_VERSION
    #if (!defined(__NFP_TOOL_NFAS) ||                       \
        (__NFP_TOOL_NFAS < NFP_SW_VERSION(5, 0, 0, 0)) ||   \
        (__NFP_TOOL_NFAS > NFP_SW_VERSION(6, 1, 0, 255)))
        #error "This standard library is not supported for the version of the SDK currently in use."
    #endif
#endif

#include <stdmac.uc>
#include <xbuf.uc>
#include <buf.uc>


/** @file dl_meta_multi.uc Packet Meta-data Utility Macros for Multiple Buffer Freelists
 * @addtogroup dl_meta_multi Packet Meta-data Utility Macros for Multiple Buffer Freelists
 * @{
 *
 * @name Packet Meta-data Utility Macros for Multiple Buffer Freelists
 * @{
 *
 */


#define     DL_META_COMPUTE_BASE    1
#define     DL_META_USE_BASE        2

/** Fetch meta-data from SRAM for the specified buffer using the buffer handle.
 *
 * @param buf_handle     Buffer handle
 * @param freelist_id    Id of buffer freelist that handle belongs to
 *                       (freelist_id must be a constant).
 * @param dl_meta        Read transfer register prefix (using xbuf_alloc) to read
 *                       meta-data from SRAM.
 *                       At least NUM_LW wxfer registers should have been allocated.
 * @param signal_number  Signal to be used in I/O operation.
 *                       If 'DL_SPLIT_META_DATA' is defined and meta data to
 *                       flush spans two SRAM channels, signal 'signal_number1' will
 *                       be used by this macro. It needs to be defined by the code
 *                       that calls this macro.
 * @param START_LW       Starting LW from which NUM_LW long words will be fetched
 * @param NUM_LW         Number of long words to fetch. Must be a constant.
 *
//   Size:
 *
 */
#macro dl_meta_load_cache(buffer_handle, freelist_id, dl_meta, signal_number, START_LW, NUM_LW)
    dl_meta_load_cache(buffer_handle, freelist_id, dl_meta, signal_number, \
        START_LW, NUM_LW, sram_addr, DL_META_COMPUTE_BASE)
#endm

/** Fetch meta-data from SRAM for the specified buffer using the buffer handle.
 *
 * @param buf_handle       Buffer handle
 * @param freelist_id      Id of buffer freelist that handle belongs to
 *                         (freelist_id must be a constant).
 * @param dl_meta          Read transfer register prefix (using xbuf_alloc) to read
 *                         meta-data from SRAM.
 *                         At least NUM_LW wxfer registers should have been allocated.
 * @param signal_number    Signal to be used in I/O operation.
 *                         If 'DL_SPLIT_META_DATA' is defined and the meta data to
 *                         flush spans two SRAM channels, signal 'signal_number1' will
 *                         be used by this macro. It needs to be defined by the code
 *                         that calls this macro.
 * @param START_LW         Starting LW from which NUM_LW long words will be fetched
 * @param NUM_LW           Number of long words to fetch. Must be a constant.
 * @param sram_addr        Address of buffer descriptor in SRAM. This is caller supplied
 *                         variable. Depending on SRAM_ADDR_USAGE parameter, either the
 *                         address is calculated or this variable is assumed to have computed address.
 *                         For SPLIT meta data, this macro assumes that another variable "sram_addr1"
 *                         is also available.
 * @param SRAM_ADDR_USAGE  Specifies what to do with sram_addr parameter: @n
 *                         @arg DL_META_USE_BASE      : Use the address
 *                         @arg DL_META_COMPUTE_BASE  : Ignore sram_addr and compute base.
 *
 * The purpose of this macro is to efficiently load selected fields of meta
 * data. If DL_META_USE_BASE is specified, then it is assumed that
 * dl_buf_get_desc_XXX is called. This macro is typically used when
 * dl_meta_load_cache is called multiple types in a program's critical path
 * to save cycles.
 *
 * @b Example:
 * If meta data is not split:
 * @code
 * dl_buf_get_desc(sram_addr, buf_handle, freelist_id)
 * ...
 * dl_meta_load_cache(buffer_handle, freelist_id, dl_meta, signal_number, \
 *     0, 3, sram_addr, DL_META_USE_BASE)
 * ...
 * dl_meta_load_cache(buffer_handle, freelist_id, dl_meta, signal_number, \
 *     5, 1, sram_addr, DL_META_USE_BASE)
 * @endcode
 * If meta data is split:
 * @code
 * dl_buf_get_desc_split(sram_addr, buf_handle, freelist_id)
 * ...
 * dl_meta_load_cache(buffer_handle, freelist_id, dl_meta, signal_number, \
 *     0, 3, sram_addr, DL_META_USE_BASE)
 * ...
 * dl_meta_load_cache(buffer_handle, freelist_id, dl_meta, signal_number, \
 *     7, 1, sram_addr, DL_META_USE_BASE)
 * @endocde
//   Size:
 *
 */
#macro dl_meta_load_cache(buffer_handle, freelist_id, dl_meta, signal_number, \
    START_LW, NUM_LW, sram_addr, SRAM_ADDR_USAGE)
.begin

    // If 'DL_SPLIT_META_DATA' is defined and the meta data to
    // load spans two SRAM channels, signal 'signal_number1' will
    // be used by this macro. It needs to be defined by the code
    // that calls this macro.

    #if (!is_ct_const(freelist_id))
        #error "freelist_id must be a constant."
    #endif

    #if ((SRAM_ADDR_USAGE != DL_META_USE_BASE) && (SRAM_ADDR_USAGE != DL_META_COMPUTE_BASE))
        #error "SRAM_ADDR_USAGE must be either DL_META_COMPUTE_BASE or DL_META_USE_BASE"
    #endif

    #define_eval _DL_SPLIT_META_DATA        DL_SPLIT_META_DATA_/**/freelist_id/**/

    #if (_DL_SPLIT_META_DATA == TRUE)

        #if (SRAM_ADDR_USAGE != DL_META_USE_BASE)

            //  Meta-data in IXP2800 reference designs is split into 2 sram channels.
            //  While loading or flushing the meta-data we need to account for this.

            .reg sram_offset

            ; Get the SRAM offset for the meta data. Channel number will be
            ; as implied by [31:30] of BUF_SRAM_BASE.

            dl_buf_get_desc(sram_offset, buffer_handle, freelist_id)

        #endif

        ; Read meta data from SRAM

        #define_eval START_INDEX        (START_LW)
        #define_eval TEMP_SRAM_OFFSET   (START_LW*4)

        #if (START_LW >= BUF_SRAM_SPLIT_LW_/**/freelist_id/**/)

            #if (SRAM_ADDR_USAGE != DL_META_USE_BASE)

                ; All LW's are in the split SRAM channel

                .reg split_sram_offset

                ; Clear the channel number
                alu[split_sram_offset, sram_offset, AND~, 3, <<30]

                ; Set the channel number
                alu[split_sram_offset, split_sram_offset, OR, BUF_SRAM_SPLIT_CHANNEL_/**/freelist_id/**/, <<30]

                sram[read, dl_meta[START_INDEX], split_sram_offset, TEMP_SRAM_OFFSET, NUM_LW], \
                    sig_done[signal_number]

            #else

                sram[read, dl_meta[START_INDEX], sram_addr1, TEMP_SRAM_OFFSET, NUM_LW], \
                    sig_done[signal_number]

            #endif

        #elif (START_LW + NUM_LW - 1 < BUF_SRAM_SPLIT_LW_/**/freelist_id/**/)

            #if (SRAM_ADDR_USAGE != DL_META_USE_BASE)

                ; All LW's are in the default SRAM channel

                .reg default_sram_offset

                ; Clear the channel number
                alu[default_sram_offset, sram_offset, AND~, 3, <<30]

                ; Set the channel number
                alu[default_sram_offset, default_sram_offset, OR, BUF_SRAM_DEFAULT_CHANNEL_/**/freelist_id/**/, <<30]

                sram[read, dl_meta[START_INDEX], default_sram_offset, TEMP_SRAM_OFFSET, NUM_LW], \
                    sig_done[signal_number]

            #else

                sram[read, dl_meta[START_INDEX], sram_addr, TEMP_SRAM_OFFSET, NUM_LW], \
                    sig_done[signal_number]

            #endif

        #else   // (START_LW >= BUF_SRAM_SPLIT_LW_/**/freelist_id/**/)

            ; LW's are in default and split SRAM channels

            #define_eval NUM_LW_1               (BUF_SRAM_SPLIT_LW_/**/freelist_id/**/ - START_LW)

            #if (SRAM_ADDR_USAGE != DL_META_USE_BASE)

                .reg default_sram_offset

                ; Clear the channel number
                alu[default_sram_offset, sram_offset, AND~, 3, <<30]

                ; Set the channel number
                alu[default_sram_offset, default_sram_offset, OR, BUF_SRAM_DEFAULT_CHANNEL_/**/freelist_id/**/, <<30]

                sram[read, dl_meta[START_INDEX], default_sram_offset, TEMP_SRAM_OFFSET, NUM_LW_1], \
                    sig_done[signal_number]

            #else

                sram[read, dl_meta[START_INDEX], sram_addr, TEMP_SRAM_OFFSET, NUM_LW_1], \
                    sig_done[signal_number]

            #endif

            #define_eval NUM_LW_2               (NUM_LW - NUM_LW_1)
            #define_eval START_INDEX_2          (BUF_SRAM_SPLIT_LW_/**/freelist_id/**/)
            #define_eval TEMP_SRAM_OFFSET_2     (START_INDEX_2*4)

            #if (SRAM_ADDR_USAGE != DL_META_USE_BASE)

                .reg split_sram_offset

                ; Clear the channel number
                alu[split_sram_offset, sram_offset, AND~, 3, <<30]

                ; Set the channel number
                alu[split_sram_offset, split_sram_offset, OR, BUF_SRAM_SPLIT_CHANNEL_/**/freelist_id/**/, <<30]

                sram[read, dl_meta[START_INDEX_2], split_sram_offset, TEMP_SRAM_OFFSET_2, NUM_LW_2], \
                    sig_done[signal_number/**/1]

            #else

                sram[read, dl_meta[START_INDEX_2], sram_addr1, TEMP_SRAM_OFFSET_2, NUM_LW_2], \
                    sig_done[signal_number/**/1]

            #endif

            #undef NUM_LW_1
            #undef NUM_LW_2
            #undef START_INDEX_2
            #undef TEMP_SRAM_OFFSET_2

        #endif  // (START_LW >= BUF_SRAM_SPLIT_LW_/**/freelist_id/**/)

        #undef START_INDEX
        #undef TEMP_SRAM_OFFSET

    #else   //  (_DL_SPLIT_META_DATA == TRUE)

    #define_eval TEMP_SRAM_OFFSET   (START_LW*4)
    #define_eval START_INDEX        (START_LW)

    #if (SRAM_ADDR_USAGE != DL_META_USE_BASE)

        .reg    sram_offset

        ; Get the SRAM offset for the meta data
        dl_buf_get_desc(sram_offset, buffer_handle, freelist_id)

        ; Read meta data from SRAM

        sram[read, dl_meta[START_INDEX], sram_offset, TEMP_SRAM_OFFSET, NUM_LW],\
            sig_done[signal_number]

    #else
        sram[read, dl_meta[START_INDEX], sram_addr, TEMP_SRAM_OFFSET, NUM_LW],\
            sig_done[signal_number]

    #endif

    #undef  TEMP_SRAM_OFFSET
    #undef  START_INDEX

    #endif  // (_DL_SPLIT_META_DATA == TRUE)

    #undef  _DL_SPLIT_META_DATA

.end
#endm


/** Flushes the cache of meta data from GPR to SRAM.
 *
 * @param wxfer_prefix  Write transfer register prefix (using xbuf_alloc).
 *                      At least NUM_LW wxfer registers should have been allocated.
 * @param buf_handle    Buffer handle
 * @param freelist_id   Id of buffer freelist that handle belongs to
 *                      (freelist_id must be a constant).
 * @param req_sig       Signal to be used in I/O operation.
 *                      If 'DL_SPLIT_META_DATA' is defined and the meta data to
 *                      flush spans two SRAM channels, signal 'req_sig1' will
 *                      be used by this macro. It needs to be defined by the code
 *                      that calls this macro.
 * @param sig_action    What to do with I/O operation:
 *                      @arg Wait for signal(s) as specified by sig_action
 *                      @arg Do not wait for signal, just return (SIG_NONE)
 * @param START_LW      Starting LW from which NUM_LW long words will be flushed
 * @param NUM_LW        Number of long words to flush. Must be a constant.
 *
//   Size:
 */
#macro dl_meta_flush_cache(wxfer_prefix, buf_handle, freelist_id, req_sig, sig_action, START_LW, NUM_LW)
.begin

    // If 'DL_SPLIT_META_DATA' is defined and the meta data to
    // load spans two SRAM channels, signal 'req_sig1' will
    // be used by this macro. It needs to be defined by the code
    // that calls this macro.

    #if (!is_ct_const(freelist_id))
        #error "freelist_id must be a constant."
    #endif

    #if (!is_ct_const(NUM_LW))
        #error  "NUM_LW should be a constant"
    #endif

    #if (NUM_LW == 0)
        #error  "NUM_LW cannot be zero"
    #endif

    #if (NUM_LW > META_CACHE_SIZE)
        #error  "NUM_LW cannot be greater than META_CACHE_SIZE"
    #endif

    #if (START_LW == 0)
        alu[wxfer_prefix[0], --, b, dl_meta[0]]
    #endif

    #define_eval END_LW (START_LW + NUM_LW)

    #if ((START_LW <= 1) && (END_LW > 1))
        alu[wxfer_prefix[1], --, b, dl_meta[1]]
    #endif

    #if ((START_LW <= 2) && (END_LW > 2))
        alu[wxfer_prefix[2], --, b, dl_meta[2]]
    #endif

    #if ((START_LW <= 3) && (END_LW > 3))
        alu[wxfer_prefix[3], --, b, dl_meta[3]]
    #endif

    #if ((START_LW <= 4) && (END_LW > 4))
        alu[wxfer_prefix[4], --, b, dl_meta[4]]
    #endif

    #if ((START_LW <= 5) && (END_LW > 5))
        alu[wxfer_prefix[5], --, b, dl_meta[5]]
    #endif

    #if ((START_LW <= 6) && (END_LW > 6))
        alu[wxfer_prefix[6], --, b, dl_meta[6]]
    #endif

    #if ((START_LW <= 7) && (END_LW > 7))
        alu[wxfer_prefix[7], --, b, dl_meta[7]]
    #endif

    dl_meta_flush(wxfer_prefix, buf_handle, freelist_id, req_sig, sig_action, START_LW, NUM_LW)

.end
#endm

/** Flushes the cache of meta data from GPR to SRAM.
 *
 * @param wxfer_prefix  Write transfer register prefix (using xbuf_alloc).
 *                      At least NUM_LW wxfer registers should have been allocated.
 * @param buf_handle    Buffer handle
 * @param freelist_id   Id of buffer freelist that handle belongs to
 *                      (freelist_id must be a constant).
 * @param req_sig       Signal to be used in I/O operation.
 *                      If 'DL_SPLIT_META_DATA' is defined and the meta data to
 *                      flush spans two SRAM channels, signal 'req_sig1' will
 *                      be used by this macro. It needs to be defined by the code
 *                      that calls this macro.
 * @param sig_action    What to do with I/O operation: @n
 *                      @arg Wait for signal(s) as specified by sig_action
 *                      @arg Do not wait for signal, just return (SIG_NONE)
 * @param START_LW      Starting LW from which NUM_LW long words will be flushed
 * @param NUM_LW        Number of long words to flush. Must be a constant.
 *
//   Size:
 */
#macro dl_meta_flush(wxfer_prefix, buf_handle, freelist_id, req_sig, sig_action, \
                    START_LW, NUM_LW)
.begin

    // If 'DL_SPLIT_META_DATA' is defined and the meta data to
    // load spans two SRAM channels, signal 'req_sig1' will
    // be used by this macro. It needs to be defined by the code
    // that calls this macro.

    #if (!is_ct_const(freelist_id))
        #error "freelist_id must be a constant."
    #endif

    #if (!is_ct_const(NUM_LW))
        #error  "NUM_LW should be a constant"
    #endif

    #if (NUM_LW == 0)
        #error  "NUM_LW cannot be zero"
    #endif

    #if (NUM_LW > META_CACHE_SIZE)
        #error  "NUM_LW cannot be greater than META_CACHE_SIZE"
    #endif


    #define_eval _DL_SPLIT_META_DATA        DL_SPLIT_META_DATA_/**/freelist_id/**/

    #if (_DL_SPLIT_META_DATA == TRUE)

        //  Meta-data in IXP2800 reference designs is split into 2 sram channels.
        //  While loading or flushing the meta-data we need to account for this.

        .reg sram_offset

        ; Get the SRAM offset for the meta data

        dl_buf_get_desc(sram_offset, buf_handle, freelist_id)

        ; Write the meta data to sram

        #define_eval START_INDEX        (START_LW)
        #define_eval TEMP_SRAM_OFFSET   (START_LW*4)

        #if (START_LW >= BUF_SRAM_SPLIT_LW_/**/freelist_id/**/)

            ; All LW's are in the split SRAM channel

            .reg split_sram_offset

            ; Clear the channel number
            alu[split_sram_offset, sram_offset, AND~, 3, <<30]

            ; Set the channel number
            alu[split_sram_offset, split_sram_offset, OR, BUF_SRAM_SPLIT_CHANNEL_/**/freelist_id/**/, <<30]

            sram[write, wxfer_prefix[START_INDEX], split_sram_offset, TEMP_SRAM_OFFSET, NUM_LW], \
                sig_done[req_sig]

        #elif (START_LW + NUM_LW - 1 < BUF_SRAM_SPLIT_LW_/**/freelist_id/**/)

            ; All LW's are in the default SRAM channel

            .reg default_sram_offset

            ; Clear the channel number
            alu[default_sram_offset, sram_offset, AND~, 3, <<30]

            ; Set the channel number
            alu[default_sram_offset, default_sram_offset, OR, BUF_SRAM_DEFAULT_CHANNEL_/**/freelist_id/**/, <<30]

            sram[write, wxfer_prefix[START_INDEX], default_sram_offset, TEMP_SRAM_OFFSET, NUM_LW], \
                sig_done[req_sig]

        #else

            ; LW's are in default and split SRAM channels

            #define_eval NUM_LW_1           (BUF_SRAM_SPLIT_LW_/**/freelist_id/**/ - START_LW)

            .reg default_sram_offset

            ; Clear the channel number
            alu[default_sram_offset, sram_offset, AND~, 3, <<30]

            ; Set the channel number
            alu[default_sram_offset, default_sram_offset, OR, BUF_SRAM_DEFAULT_CHANNEL_/**/freelist_id/**/, <<30]

            sram[write, wxfer_prefix[START_INDEX], default_sram_offset, TEMP_SRAM_OFFSET, NUM_LW_1], \
                sig_done[req_sig]

            #define_eval NUM_LW_2           (NUM_LW - NUM_LW_1)
            #define_eval START_INDEX_2      (BUF_SRAM_SPLIT_LW_/**/freelist_id/**/)
            #define_eval TEMP_SRAM_OFFSET_2 (START_INDEX_2*4)

            .reg split_sram_offset

            ; Clear the channel number
            alu[split_sram_offset, sram_offset, AND~, 3, <<30]

            ; Set the channel number
            alu[split_sram_offset, split_sram_offset, OR, BUF_SRAM_SPLIT_CHANNEL_/**/freelist_id/**/, <<30]

            sram[write, wxfer_prefix[START_INDEX_2], split_sram_offset, TEMP_SRAM_OFFSET_2, NUM_LW_2], \
                sig_done[req_sig/**/1]

            #undef NUM_LW_1
            #undef NUM_LW_2
            #undef START_INDEX_2
            #undef TEMP_SRAM_OFFSET_2

        #endif

        #undef START_INDEX
        #undef TEMP_SRAM_OFFSET

    #else   //  (_DL_SPLIT_META_DATA == TRUE)

        .reg sram_offset

        ; Get the SRAM offset for the meta data

        dl_buf_get_desc(sram_offset, buf_handle, freelist_id)

        ; Write the meta data to sram

        #define_eval START_INDEX (START_LW)
        #define_eval START_BYTE  (START_LW*4)

        sram[write, wxfer_prefix[START_INDEX], sram_offset, START_BYTE, NUM_LW],\
            sig_done[req_sig]

        #undef  START_INDEX
        #undef  START_BYTE

    #endif // (_DL_SPLIT_META_DATA == TRUE)

    #undef _DL_SPLIT_META_DATA

    #if (!streq('sig_action', 'SIG_NONE'))
        ctx_arb[sig_action]
    #endif

.end
#endm


/** Writes meta data from GPR to SRAM.
 *
 * @param wxfer_prefix  Write transfer register prefix (using xbuf_alloc).
 *                       At least NUM_LW wxfer registers should have been allocated.
 * @param buf_handle     Buffer handle
 * @param freelist_id    Id of buffer freelist that handle belongs to
 *                       (freelist_id must be a constant).
 * @param buffer_next    Fields containing meta-data that needs to be
 *                       written to SRAM
 * @param req_sig        Signal to be used in I/O operation.
 *                       If 'DL_SPLIT_META_DATA' is defined and meta data to
 *                       flush spans two SRAM channels, signal 'req_sig1' will
 *                       be used by this macro. It needs to be defined by the code
 *                       that calls this macro.
 * @param sig_action     What to do with I/O operation: @n
 *                       @arg Wait for signal(s) as specified by sig_action
 *                       @arg Do not wait for signal, just return (SIG_NONE)
 * @param START_LW       Starting LW from which NUM_LW long words will be flushed
 * @param NUM_LW         Number of long words to flush. Must be a constant.
 *
//   Size:
 *
 */
#macro dl_meta_write(wxfer_prefix, buf_handle, freelist_id, buffer_next, offset, \
                        buffer_size, packet_size, free_list_id, rx_stat, header_type, \
                        input_port, output_port, fabric_port, next_hop_id, flow_color_id, \
                        class_id, packet_next, req_sig, sig_action, START_LW, NUM_LW)

.begin

    // If 'DL_SPLIT_META_DATA' is defined and the meta data to
    // write spans two SRAM channels, signal 'req_sig1' will
    // be used by this macro. It needs to be defined by the code
    // that calls this macro.

    #if (!is_ct_const(freelist_id))
        #error "freelist_id must be a constant."
    #endif

    #if (!is_ct_const(NUM_LW))
        #error  "argument NUM_LW to dl_flush_meta should be a constant"
    #endif

    #if (NUM_LW == 0)
        #error  "argument NUM_LW to dl_flush_meta cannot be zero"
    #endif

    #if (NUM_LW > META_CACHE_SIZE)
        #error  "argument NUM_LW) to dl_flush_meta cannot be greater than META_CACHE_SIZE"
    #endif

    #if (START_LW == 0)
        alu[wxfer_prefix[0], --, b, buffer_next]
    #endif

    #define_eval END_LW (START_LW + NUM_LW)

    #if ((START_LW <= 1) && (END_LW > 1))
        alu[wxfer_prefix[1], offset, OR, buffer_size, <<16]
    #endif

    #if ((START_LW <= 2) && (END_LW > 2))
        .reg meta2
        alu[meta2, header_type, OR, packet_size, <<16]
        alu[meta2, meta2, OR, rx_stat, <<8]
        alu[wxfer_prefix[2], meta2, OR, free_list_id, <<12]
    #endif

    #if ((START_LW <= 3) && (END_LW > 3))
        alu[wxfer_prefix[3], output_port, OR, input_port, <<16]
    #endif

    #if ((START_LW <= 4) && (END_LW > 4))
        alu[wxfer_prefix[4], fabric_port, OR, next_hop_id, <<16]
    #endif

    #if ((START_LW <= 5) && (END_LW > 5))
        alu[wxfer_prefix[5], --, b, flow_color_id]
    #endif

    #if ((START_LW <= 6) && (END_LW > 6))
        alu[wxfer_prefix[6], --, b, class_id, <<16]
    #endif

    #if ((START_LW <= 7) && (END_LW > 7))
        alu[wxfer_prefix[7], --, b, packet_next]
    #endif

    #undef END_LW

    #define_eval _DL_SPLIT_META_DATA        DL_SPLIT_META_DATA_/**/freelist_id/**/

    #if (_DL_SPLIT_META_DATA == TRUE)

        ; Meta-data in IXP2800 reference designs is split into 2 sram channels.
        ; While loading or flushing the meta-data we need to account for this.

        .reg sram_offset

        ; Get the SRAM offset for the meta data

        dl_buf_get_desc(sram_offset, buf_handle, freelist_id)

        ; Write the meta data to sram

        #define_eval START_INDEX        (START_LW)
        #define_eval TEMP_SRAM_OFFSET   (START_LW*4)

        #if (START_LW >= BUF_SRAM_SPLIT_LW_/**/freelist_id/**/)

            ; All LW's are in the split SRAM channel

            .reg split_sram_offset

            ; Clear the channel number
            alu[split_sram_offset, sram_offset, AND~, 3, <<30]

            ; Set the channel number
            alu[split_sram_offset, split_sram_offset, OR, BUF_SRAM_SPLIT_CHANNEL_/**/freelist_id/**/, <<30]

            sram[write, wxfer_prefix[START_INDEX], split_sram_offset, TEMP_SRAM_OFFSET, NUM_LW], \
                sig_done[req_sig]

        #elif (START_LW + NUM_LW - 1 < BUF_SRAM_SPLIT_LW_/**/freelist_id/**/)

            ; All LW's are in the default SRAM channel

            .reg default_sram_offset

            ; Clear the channel number
            alu[default_sram_offset, sram_offset, AND~, 3, <<30]

            ; Set the channel number
            alu[default_sram_offset, default_sram_offset, OR, BUF_SRAM_DEFAULT_CHANNEL_/**/freelist_id/**/, <<30]

            sram[write, wxfer_prefix[START_INDEX], default_sram_offset, TEMP_SRAM_OFFSET, NUM_LW], \
                sig_done[req_sig]

        #else

            ; LW's are in split and default SRAM channels

            #define_eval NUM_LW_1               (BUF_SRAM_SPLIT_LW_/**/freelist_id/**/ - START_LW)

            .reg default_sram_offset

            ; Clear the channel number
            alu[default_sram_offset, sram_offset, AND~, 3, <<30]

            ; Set the channel number
            alu[default_sram_offset, default_sram_offset, OR, BUF_SRAM_DEFAULT_CHANNEL_/**/freelist_id/**/, <<30]

            sram[write, wxfer_prefix[START_INDEX], default_sram_offset, TEMP_SRAM_OFFSET, NUM_LW_1], \
                sig_done[req_sig]

            #define_eval NUM_LW_2               (NUM_LW - NUM_LW_1)
            #define_eval START_INDEX_2          (BUF_SRAM_SPLIT_LW_/**/freelist_id/**/)
            #define_eval TEMP_SRAM_OFFSET_2     (START_INDEX_2*4)

            .reg split_sram_offset

            ; Clear the channel number
            alu[split_sram_offset, sram_offset, AND~, 3, <<30]

            ; Set the channel number
            alu[split_sram_offset, split_sram_offset, OR, BUF_SRAM_SPLIT_CHANNEL_/**/freelist_id/**/, <<30]

            sram[write, wxfer_prefix[START_INDEX_2], split_sram_offset, TEMP_SRAM_OFFSET_2, NUM_LW_2], \
                sig_done[req_sig/**/1]

            #undef NUM_LW_1
            #undef NUM_LW_2
            #undef START_INDEX_2
            #undef TEMP_SRAM_OFFSET_2

        #endif

        #undef START_INDEX
        #undef TEMP_SRAM_OFFSET

    #else   //  (_DL_SPLIT_META_DATA == TRUE)

        .reg sram_offset

        ; Write the meta data to sram

        dl_buf_get_desc(sram_offset, buf_handle)

        #define_eval START_INDEX (START_LW)
        #define_eval START_BYTE  (START_LW*4)

        sram[write, wxfer_prefix[START_INDEX], sram_offset, START_BYTE, NUM_LW],\
            sig_done[req_sig]

        #undef  START_INDEX
        #undef  START_BYTE

    #endif // (_DL_SPLIT_META_DATA == TRUE)

    #undef _DL_SPLIT_META_DATA

    #if (!streq('sig_action', 'SIG_NONE'))
        ctx_arb[sig_action]
    #endif

.end
#endm


/** @}
 * @}
 */

#endif /* __DL_META_MULTI_UC__ */
