/*
 * Copyright (C) 2009-2011 Netronome Systems, Inc.  All rights reserved.
 */

#ifndef __DL_META_UC__
#define __DL_META_UC__

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

#ifdef MULTIPLE_FREELIST_ENABLE

    // If this flag is defined, the application requires multiple buffer freelists.
    // This file has meta-data utility macros that support multiple buffer freelists.
    #include <dl/dl_meta_multi.uc>

#endif


/** @file dl_meta.uc Packet Meta-data Utility Macros
 * @addtogroup dl_meta Packet Meta-data Utility
 * @{
 *
 * @name Packet Meta-data Utility Macros
 * @{
 *
 */


// Meta Data structure of Packet Buffers (LSB to MSB)
//
//  buffer_next 32 bits         Next Buffer Pointer (in a chian of buffers)
//
//  offset      16 bits         Offset to start of data in bytes
//  BufferSize  16 bits         Length of data in the current buffer in bytes
//
//  header_type 8  bits         type of header at offset bytes in to the buffer
//  rx_stat     4  bits         Receive status flags
//  free_list   4  bits         Freelist ID
//  packet_size 16 bits         (Total packet size across multiple buffers)
//
//  output_port 16 bits         Output Port on the egress processor
//  input_port  16 bits         Input Port on the ingress processor
//
//  nhid_type   4  bits         Nexthop ID type.
//  reserved    4  bits         Reserved
//  fabric_port 8  bits         Output port for fabric indicating blade ID.
//  nexthop_id  16 bits         NextHop IP ID
//
//  color       8  bits         Qos Color
//  flow_id     24 bits         QOS flow ID or MPLS label/flow id
//
//  reserved    16 bits         Reserved
//  class_id    16 bits         Class ID
//
//  packet_next     32 bits     pointer to next packet (unused in cell mode)
//
//  This SRAM META data is 32 bytes. THIS WILL HAVE TO BE ALWAYS A POWER OF 2.
//
///////////////////////////////////////////////////////////////////////////////



//  Define the No of Long Words (32 bits) of the meta data to be cached

#ifndef         META_CACHE_SIZE
    #define         META_CACHE_SIZE     4
#endif

//  These macro names are provided for backward compatibility only. They will not
//  be supported in future.

#ifdef  DL_META_BACKWARD_COMPATIBLE

    #define dl_load_cache_meta          dl_meta_load_cache
    #define dl_init_cache_meta          dl_meta_init_cache
    #define dl_flush_meta               dl_meta_flush_cache
    #define dl_get_hw_next              dl_meta_get_buffer_next
    #define dl_set_hw_next              dl_meta_set_buffer_next
    #define dl_get_offset               dl_meta_get_offset
    #define dl_set_offset               dl_meta_set_offset
    #define dl_get_buffer_size          dl_meta_get_buffer_size
    #define dl_set_buffer_size          dl_meta_set_buffer_size
    #define dl_get_header_type          dl_meta_get_header_type
    #define dl_set_header_type          dl_meta_set_header_type
    #define dl_get_free_list            dl_meta_get_free_list
    #define dl_set_free_list            dl_meta_set_free_list
    #define dl_get_rx_stat              dl_meta_get_rx_stat
    #define dl_set_rx_stat              dl_meta_set_rx_stat
    #define dl_get_packet_size          dl_meta_get_packet_size
    #define dl_set_packet_size          dl_meta_set_packet_size
    #define dl_get_output_port_egress   dl_meta_get_output_port
    #define dl_get_output_port          dl_meta_get_output_port
    #define dl_set_output_port_egress   dl_meta_set_output_port
    #define dl_set_output_port          dl_meta_set_output_port
    #define dl_get_input_port           dl_meta_get_input_port
    #define dl_set_input_port           dl_meta_set_input_port
    #define dl_get_nexthop_id           dl_meta_get_nexthop_id
    #define dl_set_nexthop_id           dl_meta_set_nexthop_id
    #define dl_get_output_port_fabric   dl_meta_get_fabric_port
    #define dl_get_fabric_port          dl_meta_get_fabric_port
    #define dl_set_output_port_fabric   dl_meta_set_fabric_port
    #define dl_set_fabric_port          dl_meta_set_fabric_port
    #define dl_get_flow_id              dl_meta_get_flow_id
    #define dl_set_flow_id              dl_meta_set_flow_id
    #define dl_get_class_id             dl_meta_get_class_id
    #define dl_set_class_id             dl_meta_set_class_id
    #define dl_get_sw_next              dl_meta_get_packet_next
    #define dl_get_packet_next          dl_meta_get_packet_next
    #define dl_set_sw_next              dl_meta_set_packet_next
    #define dl_set_packet_next          dl_meta_set_packet_next
    #define dl_write_meta               dl_meta_write

#endif


/////////////////////////////////////////////////////////////////////////////////////

#ifdef DL_META_DATA_IN_SXFER

    //  use sram xfer registers for meta data cache in read only mode

    #define dl_meta $dl_meta

#else

    #ifdef DL_META_DATA_IN_DXFER

        //  use dram xfer registers for meta data cache in read only mode

        #define dl_meta $$dl_meta

    #else

        //  GPR Register for Meta Data Caching.

        .reg dl_meta[META_CACHE_SIZE]

    #endif  // META_DATA_IN_DXFER....#else

#endif  // DL_META_DATA_IN_SXFER ....#else




//  To prevent existing code from breaking (by running out of registers)
//  define DL_THREAD_ID if you want to use dl_set/get_thread_id macros.
//  These macros share a global variable (dl_exception) which is used
//  to hold exception codes for packets going to the core. Bits[7:5] of
//  of dl_exception is used to used to store thread id.

#ifdef  USE_DL_THREAD_ID
    .reg    dl_exception
#endif

/////////////////////////////////////////////////////////////////////////////////////
//  If this switch is enabled, then we keep the incremental 16-bit checksum
//  for the packet. This checksum is obtained from RBUF when an m-pkt is received.
//  Even though, this is part of meta data, this field is *not* flushed to SRAM.
//  As a result, the meta data in SRAM will not show a checksum field associated
//  with the packet. Instead, this field is exchanged via messages between
//  different modules. For example, the packet Rx module can send this field over
//  a scratch ring to packet processing pipeline. Inside the packet processing
//  pipeline, the DL meta cache macros use this field to initialize this variable
//  for packet processing pipeline.
//
//  The format of the register is as follows.
//  Bits    Size        Description
//  -----   ----        -----------
//  31:16   16          Reserved
//  15:0    16          16-bit cksum for the entire packet (or portion of the packet
//                      assembled so far)
//
/////////////////////////////////////////////////////////////////////////////////////
#ifdef  INCLUDE_PKT_CKSUM
    .reg    dl_meta_cksum
#endif


/** This macro fetches meta-data from SRAM for the specified buffer using the buffer handle.
 *
 * param buf_handle      Buffer handle
 * @param dl_meta        Read transfer register prefix (using xbuf_alloc) to read
 *                       meta-data from SRAM. At least NUM_LW wxfer registers should have been allocated.
 * @param signal_number  Signal to be used in I/O operation.
 *                       If 'DL_SPLIT_META_DATA' is defined and the meta data to
 *                       flush spans two SRAM channels, signal 'signal_number1' will
 *                       be used by this macro. It needs to be defined by the code
 *                       that calls this macro.
 * @param START_LW       Sstarting LW from which NUM_LW long words will be fetched
 * @param NUM_LW         Number of long words to fetch. Must be a constant.
//
//   Size:
 */
#macro dl_meta_load_cache(buffer_handle, dl_meta, signal_number, START_LW, NUM_LW)
.begin

    // If 'DL_SPLIT_META_DATA' is defined and the meta data to
    // load spans two SRAM channels, signal 'signal_number1' will
    // be used by this macro. It needs to be defined by the code
    // that calls this macro.

    #ifdef DL_SPLIT_META_DATA

        //  Meta-data in IXP2800 reference designs is split into 2 sram channels.
        //  While loading or flushing the meta-data we need to account for this.

        .reg sram_offset

        //  Get the SRAM offset for the meta data. Channel number will be
        //  as implied by [31:30] of BUF_SRAM_BASE.

        dl_buf_get_desc(sram_offset, buffer_handle)

        // Read meta data from SRAM

        #define_eval START_INDEX        (START_LW)
        #define_eval TEMP_SRAM_OFFSET   (START_LW*4)

        #if (START_LW >= BUF_SRAM_SPLIT_LW)

            ; All LW's are in the split SRAM channel

            .reg split_sram_offset

            ; Clear the channel number
            alu[split_sram_offset, sram_offset, AND~, 3, <<30]

            ; Set the channel number
            alu[split_sram_offset, split_sram_offset, OR, BUF_SRAM_SPLIT_CHANNEL, <<30]

            sram[read, dl_meta[START_INDEX], split_sram_offset, TEMP_SRAM_OFFSET, NUM_LW], \
                sig_done[signal_number]

        #elif (START_LW + NUM_LW - 1 < BUF_SRAM_SPLIT_LW)

            ; All LW's are in the default SRAM channel

            #ifdef BUF_SRAM_DEFAULT_CHANNEL

                .reg default_sram_offset

                ; Clear the channel number
                alu[default_sram_offset, sram_offset, AND~, 3, <<30]
                ; Set the channel number
                alu[default_sram_offset, default_sram_offset, OR, BUF_SRAM_DEFAULT_CHANNEL, <<30]

                sram[read, dl_meta[START_INDEX], default_sram_offset, TEMP_SRAM_OFFSET, NUM_LW], \
                    sig_done[signal_number]

            #else

                // BUF_SRAM_DEFAULT_CHANNEL is assumed to be the same as the channel
                // containing BUF_SRAM_BASE

                sram[read, dl_meta[START_INDEX], sram_offset, TEMP_SRAM_OFFSET, NUM_LW], \
                    sig_done[signal_number]

            #endif // BUF_SRAM_DEFAULT_CHANNEL

        #else

            ; LW's are in default and split SRAM channels

            #define_eval NUM_LW_1               (BUF_SRAM_SPLIT_LW - START_LW)

            #ifdef BUF_SRAM_DEFAULT_CHANNEL

                .reg default_sram_offset

                ; Clear the channel number
                alu[default_sram_offset, sram_offset, AND~, 3, <<30]
                ; Set the channel number
                alu[default_sram_offset, default_sram_offset, OR, BUF_SRAM_DEFAULT_CHANNEL, <<30]

                sram[read, dl_meta[START_INDEX], default_sram_offset, TEMP_SRAM_OFFSET, NUM_LW_1], \
                    sig_done[signal_number]

            #else

                // BUF_SRAM_DEFAULT_CHANNEL is assumed to be the same as the channel
                // containing BUF_SRAM_BASE

                sram[read, dl_meta[START_INDEX], sram_offset, TEMP_SRAM_OFFSET, NUM_LW_1], \
                    sig_done[signal_number]

            #endif // BUF_SRAM_DEFAULT_CHANNEL

            #define_eval NUM_LW_2               (NUM_LW - NUM_LW_1)
            #define_eval START_INDEX_2          (BUF_SRAM_SPLIT_LW)
            #define_eval TEMP_SRAM_OFFSET_2     (START_INDEX_2*4)

            .reg split_sram_offset

            ; Clear the channel number
            alu[split_sram_offset, sram_offset, AND~, 3, <<30]
            ; Set the channel number
            alu[split_sram_offset, split_sram_offset, OR, BUF_SRAM_SPLIT_CHANNEL, <<30]

            sram[read, dl_meta[START_INDEX_2], split_sram_offset, TEMP_SRAM_OFFSET_2, NUM_LW_2], \
                sig_done[signal_number/**/1]

            #undef NUM_LW_1
            #undef NUM_LW_2
            #undef START_INDEX_2
            #undef TEMP_SRAM_OFFSET_2

        #endif

        #undef START_INDEX
        #undef TEMP_SRAM_OFFSET

    #else   //  DL_SPLIT_META_DATA

        .reg    sram_offset

        // Get the SRAM offset for the meta data
        dl_buf_get_desc(sram_offset, buffer_handle)

        // Read meta data from SRAM

        #define_eval TEMP_SRAM_OFFSET   (START_LW*4)
        #define_eval START_INDEX        (START_LW)

        sram[read, dl_meta[START_INDEX], sram_offset, TEMP_SRAM_OFFSET, NUM_LW],\
            sig_done[signal_number]

        #undef  TEMP_SRAM_OFFSET
        #undef  START_INDEX

    #endif // DL_SPLIT_META_DATA

.end
#endm


/** This macro initializes the meta data cache with given values.
 *
 * @param d0....d7   Values in registers for LW0 to LW7 of the meta data. Pass a constant (0) if that field is to be ignored.
 *
 * @instruction_cnt 1 to 7
 */
#macro dl_meta_init_cache(d0, d1, d2, d3, d4, d5, d6, d7)
.begin

    #if (!is_ct_const(d0))
        alu[dl_meta[0], --, b, d0]
    #endif

    #if ((META_CACHE_SIZE > 1) && (!is_ct_const(d1)))
        alu[dl_meta[1], --, b, d1]
    #endif

    #if ((META_CACHE_SIZE > 2) && (!is_ct_const(d2)))
        alu[dl_meta[2], --, b, d2]
    #endif

    #if ((META_CACHE_SIZE > 3) && (!is_ct_const(d3)))
        alu[dl_meta[3], --, b, d3]
    #endif

    #if ((META_CACHE_SIZE > 4) && (!is_ct_const(d4)))
        alu[dl_meta[4], --, b, d4]
    #endif

    #if ((META_CACHE_SIZE > 5) && (!is_ct_const(d5)))
        alu[dl_meta[5], --, b, d5]
    #endif

    #if ((META_CACHE_SIZE > 6) && (!is_ct_const(d6)))
        alu[dl_meta[6], --, b, d6]
    #endif

    #if ((META_CACHE_SIZE > 7) && (!is_ct_const(d7)))
        alu[dl_meta[7], --, b, d7]
    #endif

.end
#endm

/** This macro flushes the cache of meta data from GPR to SRAM.
 *
 * @param wxfer_prefix  Write transfer register prefix (using xbuf_alloc).
 *                      At least NUM_LW wxfer registers should have been allocated.
 * @param buf_handle    Buffer handle
 * @param req_sig       Signal to be used in I/O operation.
 *                      If 'DL_SPLIT_META_DATA' is defined and the meta data to
 *                      flush spans two SRAM channels, signal 'req_sig1' will
 *                      be used by this macro. It needs to be defined by the code
 *                      that calls this macro.
 * @param sig_action    What to do with I/O operation:
 *                      @arg Wait for signal(s) as specified by sig_action.
 *                      @arg Do not wait for signal, just return (SIG_NONE).
 * @param START_LW      Starting LW from which NUM_LW long words will be flushed
 * @param NUM_LW        Number of long words to flush. Must be a constant.
//
//   Size:
 */
#macro dl_meta_flush_cache(wxfer_prefix, buf_handle, req_sig, sig_action, START_LW, NUM_LW)
.begin

    // If 'DL_SPLIT_META_DATA' is defined and the meta data to
    // load spans two SRAM channels, signal 'req_sig1' will
    // be used by this macro. It needs to be defined by the code
    // that calls this macro.

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


    #ifdef DL_SPLIT_META_DATA

        //  Meta-data in IXP2800 reference designs is split into 2 sram channels.
        //  While loading or flushing the meta-data we need to account for this.

        .reg sram_offset

        // Get the SRAM offset for the meta data
        dl_buf_get_desc(sram_offset, buf_handle)

        // Write the meta data to sram

        #define_eval START_INDEX        (START_LW)
        #define_eval TEMP_SRAM_OFFSET   (START_LW*4)

        #if (START_LW >= BUF_SRAM_SPLIT_LW)

            ; All LW's are in the split SRAM channel

            .reg split_sram_offset

            ; Clear the channel number
            alu[split_sram_offset, sram_offset, AND~, 3, <<30]

            ; Set the channel number
            alu[split_sram_offset, split_sram_offset, OR, BUF_SRAM_SPLIT_CHANNEL, <<30]

            sram[write, wxfer_prefix[START_INDEX], split_sram_offset, TEMP_SRAM_OFFSET, NUM_LW], \
                sig_done[req_sig]

        #elif (START_LW + NUM_LW - 1 < BUF_SRAM_SPLIT_LW)

            ; All LW's are in the default SRAM channel

            #ifdef BUF_SRAM_DEFAULT_CHANNEL

                .reg default_sram_offset

                ; Clear the channel number
                alu[default_sram_offset, sram_offset, AND~, 3, <<30]

                ; Set the channel number
                alu[default_sram_offset, default_sram_offset, OR, BUF_SRAM_DEFAULT_CHANNEL, <<30]

                sram[write, wxfer_prefix[START_INDEX], default_sram_offset, TEMP_SRAM_OFFSET, NUM_LW], \
                    sig_done[req_sig]

            #else

                // BUF_SRAM_DEFAULT_CHANNEL is assumed to be the same as the channel
                // containing BUF_SRAM_BASE

                sram[write, wxfer_prefix[START_INDEX], sram_offset, TEMP_SRAM_OFFSET, NUM_LW], \
                    sig_done[req_sig]

            #endif // BUF_SRAM_DEFAULT_CHANNEL

        #else

            ; LW's are in default and split SRAM channels

            #define_eval NUM_LW_1           (BUF_SRAM_SPLIT_LW - START_LW)

            #ifdef BUF_SRAM_DEFAULT_CHANNEL

                .reg default_sram_offset

                ; Clear the channel number
                alu[default_sram_offset, sram_offset, AND~, 3, <<30]
                ; Set the channel number
                alu[default_sram_offset, default_sram_offset, OR, BUF_SRAM_DEFAULT_CHANNEL, <<30]

                sram[write, wxfer_prefix[START_INDEX], default_sram_offset, TEMP_SRAM_OFFSET, NUM_LW_1], \
                    sig_done[req_sig]

            #else

                // BUF_SRAM_DEFAULT_CHANNEL is assumed to be the same as the channel
                // containing BUF_SRAM_BASE

                sram[write, wxfer_prefix[START_INDEX], sram_offset, TEMP_SRAM_OFFSET, NUM_LW_1], \
                    sig_done[req_sig]

            #endif // BUF_SRAM_DEFAULT_CHANNEL

            #define_eval NUM_LW_2           (NUM_LW - NUM_LW_1)
            #define_eval START_INDEX_2      (BUF_SRAM_SPLIT_LW)
            #define_eval TEMP_SRAM_OFFSET_2 (START_INDEX_2*4)

            .reg split_sram_offset

            ; Clear the channel number
            alu[split_sram_offset, sram_offset, AND~, 3, <<30]
            ; Set the channel number
            alu[split_sram_offset, split_sram_offset, OR, BUF_SRAM_SPLIT_CHANNEL, <<30]

            sram[write, wxfer_prefix[START_INDEX_2], split_sram_offset, TEMP_SRAM_OFFSET_2, NUM_LW_2], \
                sig_done[req_sig/**/1]

            #undef NUM_LW_1
            #undef NUM_LW_2
            #undef START_INDEX_2
            #undef TEMP_SRAM_OFFSET_2

        #endif

        #undef START_INDEX
        #undef TEMP_SRAM_OFFSET

    #else   //  DL_SPLIT_META_DATA

        .reg sram_offset

        // Get the SRAM offset for the meta data
        dl_buf_get_desc(sram_offset, buf_handle)

        // Write the meta data to sram

        #define_eval START_INDEX (START_LW)
        #define_eval START_BYTE  (START_LW*4)

        sram[write, wxfer_prefix[START_INDEX], sram_offset, START_BYTE, NUM_LW],\
            sig_done[req_sig]

        #undef  START_INDEX
        #undef  START_BYTE

    #endif // DL_SPLIT_META_DATA

    #if (!streq('sig_action', 'SIG_NONE'))
        ctx_arb[sig_action]
    #endif

.end
#endm

/** This macro gets the next pointer in the meta data.
 * In the case of chained (linked list)
 * buffer, this next pointer indicates the next buffer in the chain. The format of
 * this pointer (eop, sop, cellcount(6bits), offset (24 bits)) is such that it can
 * be directly used by the SRAM Q-Array to queue this buffer.
 *
 * @param hw_next  Next pointer (32 bits). The format of this pointer should
 *                 be as specified above.
//
//      Size:
 *
 */
#macro dl_meta_get_buffer_next(buf_next)
.begin

    alu[buf_next, --, B, dl_meta[0]]            ; dl_meta0[31:0]

.end
#endm

/** This macro sets the next pointer in the meta data.
 *
 * In the case of chained (linked list)
 * buffer, this next pointer indicates the next buffer in the chain. The format of
 * this pointer (eop, sop, cellcount(6bits), offset (24 bits)) is such that it can
 * be directly used by the SRAM Q-Array to queue this buffer.
 *
 * @param hw_next   Next pointer (32 bits). The format of this pointer should
 *                  be as specified above.
//      Size:
 */
#macro dl_meta_set_buffer_next(buf_next)
.begin

    alu[dl_meta[0], --, b, buf_next]            ; dl_meta0[31:0]

.end
#endm

/** This macro gets the offset within the buffer where the actual packet data begins.
 *
 * [The data need not be at offset 0. There will be some headroom
 * at the beginning of the buffer so that headers could be prepended easily.]
 *
 * @param offset   Offset within buffer where actual data begins
 *
 *
 * @instruction_cnt 1
//
 */
#macro dl_meta_get_offset(offset)
.begin

    // in dl_meta1[0:15]
    ld_field_w_clr[offset, 0011, dl_meta[1]]        ; [0:15] of dl_meta1 is offset

.end
#endm

/** This macro sets the offset within the buffer where the actual packet data begins.
 *
 * [The data need not be at offset 0. There will be some headroom
 * at the beginning of the buffer so that headers could be prepended easily.]
 *
 * @param offset   Offset within buffer where actual data begins
 *
 * @instruction_cnt 1
 */
#macro dl_meta_set_offset(offset)
.begin

    // in dl_meta1[0:15]
    ld_field[dl_meta[1], 0011, offset]          ; [0:15] of dl_meta1 is offset

.end
#endm


/** This macro gets the Buffer size.
 * Buffer size refers to the length of actual data
 * present in this buffer (and just this buffer.)
 *
 * @param buf_len  Length of data in buffer (16 bits)
 *
 * @instruction_cnt 1
 */

#macro dl_meta_get_buffer_size(buf_len)
.begin

    ld_field_w_clr[buf_len, 0011, dl_meta[1], >>16]     ; dl_meta1 [31:16]

.end
#endm

/** This macro sets the buffer size.
 *
 * Buffer size refers to the length of actual data
 * present in this buffer (and just this buffer.)
 *
 * @param buf_len   Length of data in buffer (16 bits)
 *
 * @instruction_cnt 1
 */
#macro dl_meta_set_buffer_size(buf_len)
.begin

    ld_field[dl_meta[1], 1100, buf_len, <<16]                   ;in dl_meta1 [31:16]

.end
#endm

/** This macro gets the header type (eg.IP) at the beginning (@ "offset" bytes)
 * of the buffer.
 *
 * @param header_type  Type of header (8 bit quantity) e.g. IP, etc.
 *
 * @instruction_cnt 1
 */
#macro dl_meta_get_header_type(header_type)
.begin

    // in dl_meta2[7:0]
    ld_field_w_clr[header_type, 0001, dl_meta[2]]               ; in dl_meta2[7:0]

.end
#endm


/** This macro sets the header type (eg.IPv4) at the beginning (@ "offset" bytes)
 * of the buffer.
 *
 * @param header_type  Type of header (8 bit quantity) e.g. IPv4, etc.
 *
 * @instruction_cnt 1
 */
#macro dl_meta_set_header_type(header_type)
.begin

    // in dl_meta2[7:0]
    ld_field[dl_meta[2], 0001, header_type]                 ; in dl_meta2[7:0]

.end
#endm


/** This macro gets the free list (i.e buffer pool) from which this buffer was allocated.
 *
 * @param free_list    Free list id (4 bit quantity)
 *
 * @instruction_cnt 1
 */
#macro dl_meta_get_free_list(free_list)
.begin

    // in dl_meta2[15:12]
    alu_shf[free_list, 0xF, AND, dl_meta[2], >>12]  ; dl_meta2[15:12]

.end
#endm

/** This macro sets the free list (i.e buffer pool) from which this buffer was
 * allocated.
 *
 * @param free_list   Free list id (4 bit quantity)
 *
 * @instruction_cnt 2
 */
#macro dl_meta_set_free_list(free_list)
.begin

    // in dl_meta2[15:12]
    bits_clr(dl_meta[2], 12, 0xF)                           ; clear bits dl_meta2[12:15]

    #if ( is_ct_const(free_list) && (free_list == 0) )
        //  Do nothing
    #else
        alu_shf[dl_meta[2], dl_meta[2], OR, free_list, <<12]    ; and then set it.
    #endif

.end
#endm

/** This macro gets the receive status for this buffer.
 *
 * @param rx_stat    Receive Status (4 bits)
 *
 * @instruction_cnt 1
 */
#macro dl_meta_get_rx_stat(rx_stat)
.begin

    // in dl_meta2[11:8]
    alu_shf[rx_stat, 0xF, AND, dl_meta[2], >>8]             ; in dl_meta2[11:8]

.end
#endm

/** This macro sets the receive status in the meta data for this buffer.
 *
 * @param rx_stat     Receive status (4 bits)
 *
 * @instruction_cnt 2
 */
#macro dl_meta_set_rx_stat(rx_stat)
.begin

    bits_clr(dl_meta[2], 8, 0xF)

    #if ( is_ct_const(rx_stat) && (rx_stat == 0) )
        //  Do nothing
    #else
        ; clear bits dl_meta2[11:8]
        alu_shf[dl_meta[2], dl_meta[2], OR, rx_stat, <<8]       ; and then set it
    #endif

.end
#endm


/** This macro gets the Packet size.
 *
 * Packet size refers to the total length of data
 * present across a chain of buffers. (A big packet will be split across
 * multiple buffers. These buffers will all be linked/chained in some way).
 * If the packet is small enough to be present in a single buffer, then
 * this value will be the same as dl_get_buf_size.
 *
 * @param pkt_len    Total Length of data in packet (16 bits)
 *
 * @instruction_cnt 1
 */
#macro dl_meta_get_packet_size(pkt_len)
.begin

    ld_field_w_clr[pkt_len, 0011, dl_meta[2], >>16]     ; dl_meta2[31:16]

.end
#endm

/** This macro sets the Packet size.
 *
 * Packet size refers to the total length of data
 * present across a chain of buffers. (A big packet will be split across
 * multiple buffers. These buffers will all be linked/chained in some way).
 * If the packet is small enough to be present in a single buffer, then
 * this value should be the same as te buffer size.
 *
 * @param pkt_len      Total Length of data in packet (16 bits)
 *
 * @instruction_cnt 1
 */
#macro dl_meta_set_packet_size(pkt_len)
.begin

    ld_field[dl_meta[2], 1100, pkt_len, <<16]                   ; dl_meta2[31:16]

.end
#endm

/** This macro gets the output port for egress.
 *
 * @param oport   Output port for egress (24 bits)
 *
 * @instruction_cnt 1
 */

#macro dl_meta_get_output_port(oport)
.begin

    ld_field_w_clr[oport, 0011, dl_meta[3]]     ; dl_meta3 [15:0]

.end
#endm



/** This macro sets the output port for egress.
 *
 * @param oport   Output port for egress (24 bits)
 *
 * @instruction_cnt 1
 */
#macro dl_meta_set_output_port(oport)
.begin

    // in dl_meta3[15:0]
    ld_field[dl_meta[3], 0011, oport]           ; dl_meta3 [15:0] is output port

.end
#endm


/** This macro gets the input port through which this packet was received.
 *
 * @param input_port   Input port number (16 bits)
 *
 * @instruction_cnt 1
 */
#macro dl_meta_get_input_port(input_port)
.begin

    ld_field_w_clr[input_port, 0011, dl_meta[3], >>16]      ; dl_meta3 [31:16]

.end
#endm


/** This macro sets the input port through which this packet was received.
 *
 * @param input_port   Input port number (16 bits)
 *
 * @instruction_cnt 1
 */
#macro dl_meta_set_input_port(input_port)
.begin

    ld_field[dl_meta[3], 1100, input_port, <<16]            ; dl_meta3 [31:16]

.end
#endm


#define INPUT_PORT_MSF_CL_ID_bit    31
#define OUTPUT_PORT_MSF_CL_ID_bit   15

/** This macro sets the input MSF cluster through which this packet was received.
 *
 * @param input_msf   MSF ID through which packet was received - 0 or 1.
 *                    Input_msf can be constant or GPR.
 *
 * @instruction_cnt 1
 */
#macro dl_meta_set_input_msf_cl(input_msf)
.begin

    // Encode MSF cluster number into input port number.
    // This is useful if RX running on both MSF clusters enques to same
    // ring. It is required for application to identify through which
    // MSF and which port the packet has arrived.
    // As we have only 2 MSF's, 1 bit(MSB) of input port can be used to
    // toggle MSF cluster ID.
    alu[dl_meta[3], dl_meta[3], OR, input_msf, <<INPUT_PORT_MSF_CL_ID_bit] // 30:16 = input port, bit 31 = MSF CL ID(0 or 1)

.end
#endm

/** This macro gets the input MSF ID through which this packet was received.
 *
 * @param input_msf    MSF ID through which packet was received - 0 or 1
 *
 * @instruction_cnt 1
 */
#macro dl_meta_get_input_msf_cl(input_msf)
.begin

    alu[input_msf, 1, AND, dl_meta[3], >>INPUT_PORT_MSF_CL_ID_bit] // 30:16 = input port, bit 31 = MSF CL ID(0 or 1)

.end
#endm

/** This macro sets the output MSF cluster through which this packet will be txed.
 *
 * @param output_msf   MSF ID through which packet txed - 0 or 1.
 *                     output_msf can be constant or GPR.
 *
 * @instruction_cnt 1
 */
#macro dl_meta_set_output_msf_cl(output_msf)
.begin

    // Encode MSF cluster number into output port number.
    // As we have only 2 MSF's, 1 bit(MSB) of output port can be used to
    // toggle MSF cluster ID.
    alu[dl_meta[3], dl_meta[3], OR, output_msf, <<OUTPUT_PORT_MSF_CL_ID_bit] // 14:0 = output port, bit 15 = MSF CL ID(0 or 1)

.end
#endm

/** This macro get the output MSF ID through which this packet will be txed.
 *
 * @param output_msf   MSF ID through which packet will be txed - 0 or 1
 *
 * @instruction_cnt 1
 */
#macro dl_meta_get_output_msf_cl(output_msf)
.begin

    alu[output_msf, 1, AND, dl_meta[3], >>OUTPUT_PORT_MSF_CL_ID_bit] // 14:0 = output port, bit 15 = MSF CL ID(0 or 1)

.end
#endm



/** This macro gets the Nexthop ID.
 *
 * @param nexthop  Nexthop ID (16 bits)
 *
 * @instruction_cnt 1
 */
#macro dl_meta_get_nexthop_id(nexthop)
.begin

    #if (strstr(nexthop, '$'))
        .reg tmp
        #define_eval    NEXT_HOP    tmp

    #else
        #define_eval    NEXT_HOP    nexthop
    #endif

    ld_field_w_clr[NEXT_HOP, 0011, dl_meta[4], >>16]        ; dl_meta4 [31:16]

    #if (strstr(nexthop, '$'))
        alu[nexthop, --, B, tmp]
    #endif

    #undef  NEXT_HOP

.end
#endm

/** This macro sets the nexthop id.
 *
 * @param nexthop   Nexthop ID (16 bits)
 *
 * @instruction_cnt 1
 */
#macro dl_meta_set_nexthop_id(nexthop)
.begin

    #if (is_ct_const(nexthop))
        immed_w1[dl_meta[4], (nexthop & 0xffff)]
    #else
        ld_field[dl_meta[4], 1100, nexthop, <<16]           ; dl_meta4 [31:16]
    #endif

.end
#endm


/** This macro gets the fabric port.
 *
 * This more like a blade ID (when multiple blades
 * are connected to fabric).
 *
 * @param nexthop    Fabric port number (8 bits)
 *
 * @instruction_cnt 1
 */
#macro dl_meta_get_fabric_port(fabric)
.begin

    // in dl_meta4[15:8]
    ld_field_w_clr[fabric, 0001, dl_meta[4], >>8]       ; in dl_meta4[15:8]

.end
#endm


/** This macro sets the fabric port.
 *
 * This more like a blade ID (when multiple blades
 * are connected to fabric).
 *
 * @param nexthop     Fabric port number (8 bits)
 *
 * @instruction_cnt 1
 */
#macro dl_meta_set_fabric_port(fabric)
.begin

    // in dl_meta4[15:8]
    ld_field[dl_meta[4], 0010, fabric, <<8]         ; in dl_meta4[15:8]

.end
#endm


/** This macro gets the nexthop id type.
 *
 * Typical nexthop id types are:
 *
 * @param nexthop    nexthop id type (4 bits)
 *
 * @instruction_cnt 1
 */

#macro dl_meta_get_nexthop_id_type(nhid_type)
.begin

    //  in dl_meta4[3:0]
    alu[nhid_type, 0xF, AND, dl_meta[4]]                ; dl_meta4[3:0]

.end
#endm


/** This macro sets the nexthop id type.
 *
 * The ID type will specify which table to lookup the nexthop id.
 *
 * @param nexthop     nexthop id type (4 bits)
 *
 * @instruction_cnt 2 maximum
 */
#macro dl_meta_set_nexthop_id_type(nhid_type)
.begin

    //  in dl_meta4[3:0]
    bits_clr(dl_meta[4], 0, 0xF)                        ; clear bits dl_meta4[3:0]

    #if ( is_ct_const(nhid_type) && (nhid_type == 0) )
        //  Do nothing
    #else
        alu[dl_meta[4], dl_meta[4], OR, nhid_type]          ; and then set it.
    #endif

.end
#endm

/** This macro gets the Flow ID.
 *
 * @param flow_id    Flow ID (24 bits)
 *
 *      Inputs:
 * @instruction_cnt 1
 */
#macro dl_meta_get_flow_id(flow_id)
.begin

    ld_field_w_clr[flow_id, 0111, dl_meta[5]]       ; dl_meta5 [24:0]

.end
#endm

/** This macro sets the flow ID.
 *
 * @param flow_id        Flow ID (24 bits)
 *
 * @instruction_cnt 1
 */
#macro dl_meta_set_flow_id(flow_id)
.begin

    ld_field[dl_meta[5], 0111, flow_id]     ;dl_meta5[24:0]

.end
#endm

/** This macro gets color.
 *
 * @param color    Color   (8 bits)
 *
 * @instruction_cnt 1
 */
#macro dl_meta_get_color(color)
.begin

    ld_field_w_clr[color, 0001, dl_meta[5], >>24]       ; dl_meta5 [31:24]

.end
#endm

/** This macro sets the color.
 *
 * @param color   8 bits
 *
 * @instruction_cnt 1
 */
#macro dl_meta_set_color(color)
.begin

    ld_field[dl_meta[5], 1000, color, <<24]     ;dl_meta5[31:24]

.end
#endm

/** This macro gets Class ID.
 *
 * @param class_id    Class ID (16 bits)
 *
 * @instruction_cnt 1
 */
#macro dl_meta_get_class_id(class_id)
.begin

    ld_field_w_clr[class_id, 0011, dl_meta[6], >>16]    ; [31:16] is class id

.end
#endm

/** This macro sets the class ID.
 *
 * @param class_id     Class ID (16 bits)
 *
 * @instruction_cnt 1
 */

#macro dl_meta_set_class_id(class_id)
.begin

    ld_field[dl_meta[6], 1100, class_id, <<16]              ; [31:16] is class id

.end
#endm


/** This macro gets the Software Next pointer (i.e this pointer is maintained by
 * software).
 *
 * @param sw_next   Software next pointer (32 bits)
 *
 * @instruction_cnt 1
 */

#macro dl_meta_get_packet_next(packet_next)
.begin

    alu[packet_next, --, B, dl_meta[7]]                     ; dl_meta7[31:0]

.end
#endm


/** This macro sets the software next pointer in the meta data.
 *
 * @param sw_next    Software next pointer (32 bits) to set
 *
 * @instruction_cnt 1
 */
#macro dl_meta_set_packet_next(packet_next)
.begin

    alu[dl_meta[7], --, b, packet_next]                     ; dl_meta7[31:0]

.end
#endm

/** This macro gets the thread id which was earlier stored/initialized using
 * dl_set_thread_id.
 *
 * @param thread_id    Thread ID (0-7)
 *
 * @instruction_cnt 1
 */
#macro dl_get_thread_id(thread_id)
.begin

    #ifdef IX_PLATFORM_2801

        //  In order to save registers, we use bits[22:20] of dl_exception_reg register
        //  to store thread id.
        alu[thread_id, 0x7, AND, dl_exception, >>20] ; Bits[22:20] contain thread id

    #else

        //  In order to save registers, we use bits[7:5] of dl_exception register
        //  to store thread id.
        alu_shf[thread_id, 0x7, AND, dl_exception, >>5] ; Bits[7:5] contain thread id

    #endif

.end
#endm


/** This macro stores the given thread id for later retrival.
 *
 * @param thread_id   Thread ID (0-7)
 *
 * @instruction_cnt 1
 */
#macro dl_set_thread_id(thread_id)
.begin

    //  In order to save registers, we use bits[7:5] of dl_exception register
    //  to store thread id. Exepected usage is that that dl_meta_set_thread_id will
    //  be called once during intialisation.

    #ifdef IX_PLATFORM_2801

        bits_clr(dl_exception, 20, 0x7)  ; clear bits dl_exception[22:20]
        alu[dl_exception, dl_exception, OR, thread_id, <<20]

    #else

        .set    dl_exception

        bits_clr(dl_exception, 5, 0x7)                  ; clear bits dl_exception[7:5]
        alu_shf[dl_exception, dl_exception, OR, thread_id, <<5]

    #endif

.end
#endm


/** This macro writes meta data from GPR to SRAM.
 *
 * @param wxfer_prefix  Write transfer register prefix (using xbuf_alloc).
 *                      At least NUM_LW wxfer registers should have been allocated.
 * @param buf_handle    Buffer handle
 * @param buffer_next   Contain meta-data that needs to be written to SRAM
 * @param packet_next
 * @param req_sig       Signal to be used in I/O operation.
 *                      If 'DL_SPLIT_META_DATA' is defined and the meta data to
 *                      flush spans two SRAM channels, signal 'req_sig1' will
 *                      be used by this macro. It needs to be defined by the code
 *                      that calls this macro.
 * @param sig_action    What to do with I/O operation:
 *                      @arg Wait for signal(s) as specified by sig_action.
 *                      @arg Do not wait for signal, just return (SIG_NONE).
 * @param START_LW      Starting LW from which NUM_LW long words will be flushed
 * @param NUM_LW        Number of long words to flush. Must be a constant.
//
//   Size:
 */
#macro dl_meta_write( wxfer_prefix, buf_handle, buffer_next, offset, buffer_size, \
                        packet_size, free_list_id, rx_stat, header_type, input_port, \
                        output_port, fabric_port, next_hop_id, flow_color_id, class_id, \
                        packet_next, req_sig, sig_action, START_LW, NUM_LW )

.begin

    // If 'DL_SPLIT_META_DATA' is defined and the meta data to
    // write spans two SRAM channels, signal 'req_sig1' will
    // be used by this macro. It needs to be defined by the code
    // that calls this macro.

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

    #ifdef DL_SPLIT_META_DATA

        //  Meta-data in IXP2800 reference designs is split into 2 sram channels.
        //  While loading or flushing the meta-data we need to account for this.

        .reg sram_offset

        // Get the SRAM offset for the meta data
        dl_buf_get_desc(sram_offset, buf_handle)

        // Write the meta data to sram

        #define_eval START_INDEX        (START_LW)
        #define_eval TEMP_SRAM_OFFSET   (START_LW*4)

        #if (START_LW >= BUF_SRAM_SPLIT_LW)

            ; All LW's are in the split SRAM channel

            .reg split_sram_offset

            ; Clear the channel number
            alu[split_sram_offset, sram_offset, AND~, 3, <<30]

            ; Set the channel number
            alu[split_sram_offset, split_sram_offset, OR, BUF_SRAM_SPLIT_CHANNEL, <<30]

            sram[write, wxfer_prefix[START_INDEX], split_sram_offset, TEMP_SRAM_OFFSET, NUM_LW], \
                sig_done[req_sig]

        #elif (START_LW + NUM_LW - 1 < BUF_SRAM_SPLIT_LW)

            ; All LW's are in the default SRAM channel

            #ifdef BUF_SRAM_DEFAULT_CHANNEL

                .reg default_sram_offset

                ; Clear the channel number
                alu[default_sram_offset, sram_offset, AND~, 3, <<30]

                ; Set the channel number
                alu[default_sram_offset, default_sram_offset, OR, BUF_SRAM_DEFAULT_CHANNEL, <<30]

                sram[write, wxfer_prefix[START_INDEX], default_sram_offset, TEMP_SRAM_OFFSET, NUM_LW], \
                    sig_done[req_sig]

            #else

                // BUF_SRAM_DEFAULT_CHANNEL is assumed to be the same as the channel
                // containing BUF_SRAM_BASE

                sram[write, wxfer_prefix[START_INDEX], sram_offset, TEMP_SRAM_OFFSET, NUM_LW], \
                    sig_done[req_sig]

            #endif // BUF_SRAM_DEFAULT_CHANNEL

        #else

            ; LW's are in split and default SRAM channels

            #define_eval NUM_LW_1               (BUF_SRAM_SPLIT_LW - START_LW)

            #ifdef BUF_SRAM_DEFAULT_CHANNEL

                .reg default_sram_offset

                ; Clear the channel number
                alu[default_sram_offset, sram_offset, AND~, 3, <<30]

                ; Set the channel number
                alu[default_sram_offset, default_sram_offset, OR, BUF_SRAM_DEFAULT_CHANNEL, <<30]

                sram[write, wxfer_prefix[START_INDEX], default_sram_offset, TEMP_SRAM_OFFSET, NUM_LW_1], \
                    sig_done[req_sig]

            #else

                // BUF_SRAM_DEFAULT_CHANNEL is assumed to be the same as the channel
                // containing BUF_SRAM_BASE

                sram[write, wxfer_prefix[START_INDEX], sram_offset, TEMP_SRAM_OFFSET, NUM_LW_1], \
                    sig_done[req_sig]

            #endif // BUF_SRAM_DEFAULT_CHANNEL

            #define_eval NUM_LW_2               (NUM_LW - NUM_LW_1)
            #define_eval START_INDEX_2          (BUF_SRAM_SPLIT_LW)
            #define_eval TEMP_SRAM_OFFSET_2     (START_INDEX_2*4)

            .reg split_sram_offset

            ; Clear the channel number
            alu[split_sram_offset, sram_offset, AND~, 3, <<30]
            ; Set the channel number
            alu[split_sram_offset, split_sram_offset, OR, BUF_SRAM_SPLIT_CHANNEL, <<30]

            sram[write, wxfer_prefix[START_INDEX_2], split_sram_offset, TEMP_SRAM_OFFSET_2, NUM_LW_2], \
                sig_done[req_sig/**/1]

            #undef NUM_LW_1
            #undef NUM_LW_2
            #undef START_INDEX_2
            #undef TEMP_SRAM_OFFSET_2

        #endif

        #undef START_INDEX
        #undef TEMP_SRAM_OFFSET

    #else   //  DL_SPLIT_META_DATA

        .reg sram_offset

        // Write the meta data to sram

        dl_buf_get_desc(sram_offset, buf_handle)

        #define_eval START_INDEX (START_LW)
        #define_eval START_BYTE  (START_LW*4)

        sram[write, wxfer_prefix[START_INDEX], sram_offset, START_BYTE, NUM_LW],\
            sig_done[req_sig]

        #undef  START_INDEX
        #undef  START_BYTE

    #endif // DL_SPLIT_META_DATA

    #if (!streq('sig_action', 'SIG_NONE'))
        ctx_arb[sig_action]
    #endif

.end
#endm


#ifndef RING_SIZE_128
    #define RING_SIZE_128       0
#endif
#ifndef RING_SIZE_256
    #define RING_SIZE_256       1
#endif
#ifndef RING_SIZE_512
    #define RING_SIZE_512       2
#endif
#ifndef RING_SIZE_1024
    #define RING_SIZE_1024      3
#endif

/** This macro initializes and sets up scratch ring.
 *
 * The ring is setup at the address specified by
 * SCRATCH_RING_BASE, the ring number is specified by RING_NUMBER and the size
 * of the ring is specifed by RING_SIZE_128 (all are defined in scratchring.h).
 *
 * This macro assumes the "RING_STATUS_FLAG" to be 0.
 *
 * For this example, the ring is initialized at base address 0x1000, ring
 * number is 0, and ring size is 128.
 *
 * @param RBASE      Base address of ring in scratch memory. Should
 *                   aligned on a 4 byte boundary. This should be constant.
 * @param RSIZE      Size of ring. Either 128, 256, 512 or 1024. This
 *                   should be a constant.
 * @param RING       Ring number (0-15). This should be a constant.
 *
 * @instruction_cnt 8
 *
 * Ring size [31:30]. Hardware representation:
 * @arg 00 = 128
 * @arg 01 = 256
 * @arg 10 = 512
 * @arg 11 = 1024
 */
#macro init_scratch_ring(RBASE, RSIZE, RING)
.begin

    .sig    cw1, cw2, cw3                               // signals used in cap[write...]
    .reg    $_rhead, $_rtail, $_rbase, _base

    // These define_eval are required. Otherwise the caller cannot have spaces
    // in between parameters like init[a, b, c].

    #define_eval RN     RING
    #define_eval RS     RSIZE
    #define_eval RB     RBASE

    immed[$_rhead, 0x0]                             ; Initialise ring head to 0
    immed[$_rtail, 0x0]                             ; Initialise ring tail to 0
    immed[_base, RB]                                ; Initialise ring base to 0x1000
    alu_shf[$_rbase, _base, or, RING_SIZE_/**/RS, <<30]; [31:30]= 0 => Ring size is 128

    // Initialise the Scratch Ring base (and size), head and tail.

    // Note: We can Queue a max. of 4 commands to any external unit
    // (like sram, dram, cap, etc). Beyond this limit the ME will stall.
    // The limit of 4 includes all the commands issued by all other MEs
    // as well. It is the programmers responsibility to ensure this.

    // Since this is the only thread and ME that is queuing cmds at this time,
    // we can queue 3 commands safely.

    cap[write, $_rbase, SCRATCH_RING_BASE_/**/RN], sig_done[cw1]    ; base = 0x1000
    cap[write, $_rhead, SCRATCH_RING_HEAD_/**/RN], sig_done[cw2]    ; head = 0
    cap[write, $_rtail, SCRATCH_RING_TAIL_/**/RN], sig_done[cw3]    ; tail = 0
    ctx_arb[cw1, cw2, cw3]

    #undef RN
    #undef RS
    #undef RB

.end
#endm


#ifdef  INCLUDE_PKT_CKSUM
/** This macro gets Checksum.
 *
 * @ param cksum    Ones complement 16 bit checksum for packet
 *
 * @ instruction_cnt 1
 */
#macro dl_meta_get_cksum(cksum)
.begin

    ld_field_w_clr[cksum, 0011, dl_meta_cksum, >>0] ; [15:0] is checksum

.end
#endm

/** This macro sets the checksum.
 *
 * @note Since the DL register has the upper 16 bits as reserved, the register
 * will simply be overwritten. If this should not be done,
 * immed_w0 or ld_field should be used. Otherwise the assembler will give warnings.
 *
 * @param cksum   Ones complement 16 bit checksum for packet
 *
 * @instruction_cnt 1
 */
#macro dl_meta_set_cksum(cksum)
.begin

    #if (is_ct_const(cksum))
        immed[dl_meta_cksum, (cksum & 0xffff)]
    #else
        ld_field_w_clr[dl_meta_cksum, 0011, cksum, <<0]     ; [15:0] is checksum
    #endif

.end
#endm


#endif  // INCLUDE_PKT_CKSUM


/** @}
 * @}
 */

#endif /* __DL_META_UC__ */
