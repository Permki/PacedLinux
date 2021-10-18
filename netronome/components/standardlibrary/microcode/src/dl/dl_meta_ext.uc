/*
 * Copyright (C) 2009-2011 Netronome Systems, Inc.  All rights reserved.
 */

#ifndef __DL_META_EXT_UC__
#define __DL_META_EXT_UC__

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

// use existing infrastructure
#include <dl/dl_meta.uc>

/** @file dl_meta_ext.uc Packet Meta-data Utility Macros for Multicasting and Fragmentation
 * @addtogroup dl_meta_ext Packet Meta-data Utility Macros for Multicasting and Fragmentation
 * @{
 *
 * @name Packet Meta-data Utility Macros for Multicasting and Fragmentation
 * @{
 *
 */


//  Two modes of operation for these macros

#define     META_MODE_IGNORE            0
#define     META_PARENT_PACKET_MODE     1
#define     META_PARENT_CELL_MODE       2
#define     META_CHILD_PACKET_MODE      3
#define     META_CHILD_CELL_MODE        4

////////////////////////////////////////////////////////////////////////////
//
//  Meta Data structure of the original packet, when copies are created.
//  This is also called PARENT META DATA structure.
//
//  Parent Meta data in PACKET MODE
//
//  LW      Bits        Size    Description
//  ========================================================================
//  0 thru 6    ALL     --      These LW are same as regular meta data
//                              format
//  7       31:16       16      Reserved.
//  7       15:0        16      Reference Count.
//
////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////
//
//  Meta Data structure of the original packet, when copies are created.
//  This is also called PARENT META DATA structure.
//
//  Parent Meta data in CELL MODE
//
//  LW      Bits        Size    Description
//  ========================================================================
//  0 thru 6    ALL     --      These LW are same as regular meta data
//                              format
//  7       31:16       16      Reserved.
//  7       15:0        16      Reference Count.
//
////////////////////////////////////////////////////////////////////////////

/** This macro obtains the reference count field in parent meta data.
 */
#macro dl_meta_parent_get_ref_cnt(MODE, cnt)
    // dl_meta7[15:0]
    ld_field_w_clr[cnt, 0011, dl_meta[7]]
#endm

/** This macro obtains the reference count field in parent meta data.
 *
 * Same as dl_meta_parent_get_ref_cnt macro.
 */
#macro dl_meta_get_ref_cnt(MODE, cnt)
    dl_meta_parent_get_ref_cnt(MODE, cnt)
#endm

////////////////////////////////////////////////////////////////////////////
//
//  Meta Data structure of the copied packet, when copies are created.
//  This is also called CHILD META DATA structure.
//
//  Child Meta data in PACKET MODE.
//
//  It is worth noting that some of the fields in the meta data are same as
//  parent meta data. But these fields assume a different meaning in child
//  meta data. This will facilitate microblocks which are agnostic to
//  packet replication, function as usual without any modification.
//
//
//  LW      Bits        Size    Description
//  ========================================================================
//  0       31:22       10      Offset of start of data in child buffer in
//                              bytes
//  0       21:14       8       Size of data in child buffer
//  0       13:4        10      Reserved.
//  0       3:0         4       Freelist ID for child buffer
//
//  1       31:16       16      Parent buffer data size in bytes
//  1       15:0        16      Offset of start of data in parent buffer in
//                              bytes
//
//  2       31:16       16      Total packet size across all buffers including
//                              the child buffer
//  2       15:12       4       Freelist ID of parent buffer
//  2       11:8        4       Rx Status
//  2       7:0         8       Type of header at offset in child buffer if
//                              data exists in child buffer. Otherwise it is
//                              for parent buffer data.
//
//  3 thru 5    --      --      Same as regular meta data
//
//  6       31:16       16      Class ID
//  6       15:0        16      Parent buffer ID. This is nothing but the handle
//                              serial number.
//
//  7       --          --      Same as regular meta data
//
////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////
//
//  Meta Data structure of the copied packet, when copies are created.
//  This is also called CHILD META DATA structure.
//
//  Child Meta data in CELL MODE
//
//  It is worth noting that some of the fields in the meta data are same as
//  parent meta data. But these fields assume a different meaning in child
//  meta data. This will facilitate microblocks which are agnostic to
//  packet replication, function as usual without any modification.
//
//
//  LW      Bits        Size    Description
//  ========================================================================
//  0       31:0        32      Next CHILD handle if one exists. Otherwise,
//                              IX_NULL.
//
//  1       31:16       16      Parent buffer data size in bytes
//  1       15:0        16      Offset of start of data in parent buffer in
//                              bytes
//
//  2       31:16       16      Total packet size across all buffers including
//                              the child buffer
//  2       15:12       4       Freelist ID of parent buffer
//  2       11:8        4       Rx Status
//  2       7:0         8       Type of header at offset in child buffer if
//                              data exists in child buffer. Otherwise it is
//                              for parent buffer data.
//
//  3 thru 5    --      --      Same as regular meta data
//
//  6       31:16       16      Class ID
//  6       15:0        16      Parent buffer ID. This is nothing but the handle
//                              serial number.
//
//  7       31:22       10      Offset of start of data in child buffer in
//                              bytes
//  7       21:14       8       Size of data in child buffer
//  7       13:4        10      Reserved.
//  7       3:0         4       Freelist ID for child buffer
//
////////////////////////////////////////////////////////////////////////////

//
//  Since both meta formats are similar, only one macro interface is exported.
//  This interface requires additional parameter. Also note that, the macro
//  interface is used to access only certain fields only. The remaining fields
//  are accessed using the regular dl_meta_xxx calls. For now, we have
//  wrapper macros to facilitate this.
//
//  It is also assumed that if a block specifically uses these macros, the
//  block must be aware of existence of child and parent buffer meta data.
//

/** This macro obtains the child buffer start of data offset in bytes.
 */
#macro dl_meta_child_get_child_offset(MODE, child_offset)
    #if (MODE == META_CHILD_CELL_MODE)
        alu_shf[child_offset, --, B, dl_meta[7], >>22]
    #elif   (MODE == META_CHILD_PACKET_MODE)
        alu_shf[child_offset, --, B, dl_meta[0], >>22]
    #else
        #error  "Specify either META_CHILD_CELL_MODE or META_CHILD_PACKET_MODE"
    #endif
#endm


/** This macro sets the child buffer start of data offset in bytes.
 */
#macro dl_meta_child_set_child_offset(MODE, child_offset)
    #if (MODE == META_CHILD_CELL_MODE)

        bits_clr(dl_meta[7], 22, 0x3ff)

        #if (is_ct_const(child_offset) && (child_offset == 0) )

            // do nothing

        #else

            alu_shf[dl_meta[7], dl_meta[7], OR, child_offset, <<22]

        #endif

    #elif   (MODE == META_CHILD_PACKET_MODE)

        bits_clr(dl_meta[0], 22, 0x3ff)

        #if (is_ct_const(child_offset) && (child_offset == 0) )

            // do nothing

        #else

            alu_shf[dl_meta[0], dl_meta[0], OR, child_offset, <<22]

        #endif

    #else

        #error  "Specify either META_CHILD_CELL_MODE or META_CHILD_PACKET_MODE"

    #endif
#endm


/** this macro obtains the child buffer size in bytes.
 */
#macro dl_meta_child_get_child_buffer_size(MODE, buf_size)
    #if (MODE == META_CHILD_CELL_MODE)

        ld_field_w_clr[buf_size, 0001, dl_meta[7], >>14]

    #elif   (MODE == META_CHILD_PACKET_MODE)

        ld_field_w_clr[buf_size, 0001, dl_meta[0], >>14]

    #else

        #error  "Specify either META_CHILD_CELL_MODE or META_CHILD_PACKET_MODE"

    #endif
#endm

/** This macro sets the child buffer size in bytes.
 */
#macro dl_meta_child_set_child_buffer_size(MODE, buf_size)
    #if (MODE == META_CHILD_CELL_MODE)

        // first clear the field

        bits_clr(dl_meta[7], 14, 0xff)

        #if (is_ct_const(buf_size) && (buf_size == 0) )

            // do nothing

        #else

            alu_shf[dl_meta[7], dl_meta[7], OR, buf_size, <<14]

        #endif

    #elif   (MODE == META_CHILD_PACKET_MODE)

        // first clear the field

        bits_clr(dl_meta[0], 14, 0xff)

        #if (is_ct_const(buf_size) && (buf_size == 0) )

            // do nothing

        #else

            alu_shf[dl_meta[0], dl_meta[0], OR, buf_size, <<14]

        #endif

    #else

    #error  "Specify either META_CHILD_CELL_MODE or META_CHILD_PACKET_MODE"

    #endif
#endm


/** This macro obtains the Freelist ID of child buffer.
 */
#macro dl_meta_child_get_child_freelist_id(MODE, freelist_id)
    #if (MODE == META_CHILD_CELL_MODE)

        alu[freelist_id, 0xf, AND, dl_meta[7]]

    #elif (MODE == META_CHILD_PACKET_MODE)

        alu[freelist_id, 0xf, AND, dl_meta[0]]

    #else

        #error  "Specify either META_CHILD_CELL_MODE or META_CHILD_PACKET_MODE"

    #endif
#endm


/** This macro sets the Freelist ID of child buffer.
 */
#macro dl_meta_child_set_child_freelist_id(MODE, freelist_id)
    #if (MODE == META_CHILD_CELL_MODE)

        // first clear the field

        bits_clr(dl_meta[7], 0, 0xf)

        #if (is_ct_const(freelist_id) && (freelist_id == 0) )

            // do nothing

        #else

            alu[dl_meta[7], dl_meta[7], OR, freelist_id]

        #endif

    #elif   (MODE == META_CHILD_PACKET_MODE)

        // first clear the field

        bits_clr(dl_meta[0], 0, 0xf)

        #if (is_ct_const(freelist_id) && (freelist_id == 0) )

            // do nothing

        #else

            alu[dl_meta[0], dl_meta[0], OR, freelist_id]

        #endif

    #else

        #error  "Specify either META_CHILD_CELL_MODE or META_CHILD_PACKET_MODE"

    #endif
#endm



/** This macro obtains the parent buffer start of data offset in bytes.
 */

#macro dl_meta_child_get_parent_offset(MODE, parent_offset)
    dl_meta_get_offset(parent_offset)
#endm

/** This macro obtains the parent buffer start of data offset in bytes.
 */
#macro dl_meta_child_set_parent_offset(MODE, parent_offset)
    dl_meta_set_offset(parent_offset)
#endm


/** This macro obtain the parent buffer size in bytes.
 */
#macro dl_meta_child_get_parent_buffer_size(MODE, buf_size)
    dl_meta_get_buffer_size(buf_size)
#endm

/** This macro obtains the parent buffer size in bytes.
 */
#macro dl_meta_child_set_parent_buffer_size(MODE, buf_size)
    dl_meta_set_buffer_size(buf_size)
#endm

/** This macro obtains the header type of the child data if exists, otherwise this is
 * for parent data.
 */
#macro dl_meta_child_get_header_type(MODE, header_type)
    dl_meta_get_header_type(header_type)
#endm

/** This macro sets the header type of the child data if exists, otherwise this is
 * for parent data.
 */
#macro dl_meta_child_set_header_type(MODE, header_type)
    dl_meta_set_header_type(header_type)
#endm

/** This macro obtains the free list ID of the parent buffer in child meta data.
 */
#macro dl_meta_child_get_parent_free_list(MODE, free_list)
    dl_meta_get_free_list(free_list)
#endm


/** This macro sets the free list ID of the parent buffer in child meta data.
 */
#macro dl_meta_child_set_parent_free_list(MODE, free_list)
    dl_meta_set_free_list(free_list)
#endm


/** This macro obtains the Receive Status of the child buffer.
 */
#macro dl_meta_child_get_rx_stat(MODE, rx_stat)
    dl_meta_get_rx_stat(rx_stat)
#endm
/** This macro obtains the Receive Status of the child buffer.
 */

#macro dl_meta_child_set_rx_stat(MODE, rx_stat)
    dl_meta_set_rx_stat(rx_stat)
#endm


/** This macro obtains the size of the packet across all buffers including the child
 *  buffer.
 */
#macro dl_meta_child_get_packet_size(MODE, pkt_len)
    dl_meta_get_packet_size(pkt_len)
#endm


/** This macro sets the size of the packet across all buffers including the child
 * buffer.
 */
#macro dl_meta_child_set_packet_size(MODE, pkt_len)
    dl_meta_set_packet_size(pkt_len)
#endm


/** This macro obtains the output port for this packet.
 */
#macro dl_meta_child_get_output_port(MODE, oport)
    dl_meta_get_output_port(oport)
#endm

/** This macro obtains the output port for this packet.
 */
#macro dl_meta_child_set_output_port(MODE, oport)
    dl_meta_set_output_port(oport)
#endm

/** This macro obtains the input port for this packet.
 */
#macro dl_meta_child_get_input_port(MODE, iport)
    dl_meta_get_input_port(iport)
#endm

/** This macro obtains the input port for this packet.
 */
#macro dl_meta_child_set_input_port(MODE, iport)
    dl_meta_set_input_port(iport)
#endm

/** This macro obtains the nexthop ID for the packet.
 */
#macro dl_meta_child_get_nexthop_id(MODE, nexthop)
    dl_meta_get_nexthop_id(nexthop)
#endm

/** This macro sets the nexthop ID for the packet.
 */
#macro dl_meta_child_set_nexthop_id(MODE, nexthop)
    dl_meta_set_nexthop_id(nexthop)
#endm

/** This macro obtains the outgoing blade ID for this packet.
 */
#macro dl_meta_child_get_fabric_port(MODE, fabric)
    dl_meta_get_fabric_port(fabric)
#endm


/** This macro sets the outgoing blade ID for this packet.
 */
#macro dl_meta_child_set_fabric_port(MODE, fabric)
    dl_meta_set_fabric_port(fabric)
#endm

////////////////////////////////////////////////////////////////////////////
//  dl_meta_child_get_nexthop_id_type
//
//  Self explanatory
//
////////////////////////////////////////////////////////////////////////////

#macro dl_meta_child_get_nexthop_id_type(MODE, nhid_type)
    dl_meta_get_nexthop_id_type(nhid_type)
#endm


////////////////////////////////////////////////////////////////////////////
//  dl_meta_child_set_nexthop_id_type
//
//  Self explanatory
//
////////////////////////////////////////////////////////////////////////////

#macro dl_meta_child_set_nexthop_id_type(MODE, nhid_type)
    dl_meta_set_nexthop_id_type(nhid_type)
#endm


////////////////////////////////////////////////////////////////////////////
//  dl_meta_child_get_flow_id
//
//  Self explanatory
//
////////////////////////////////////////////////////////////////////////////

#macro dl_meta_child_get_flow_id(MODE, flow_id)
    dl_meta_get_flow_id(flow_id)
#endm

////////////////////////////////////////////////////////////////////////////
//  dl_meta_child_set_flow_id
//
//  Self explanatory
//
////////////////////////////////////////////////////////////////////////////

#macro dl_meta_child_set_flow_id(MODE, flow_id)
    dl_meta_set_flow_id(flow_id)
#endm

////////////////////////////////////////////////////////////////////////////
//  dl_meta_child_get_color
//
//  Self explanatory
//
////////////////////////////////////////////////////////////////////////////

#macro dl_meta_child_get_color(MODE, color)
    dl_meta_get_color(color)
#endm

////////////////////////////////////////////////////////////////////////////
//  dl_meta_child_set_color
//
//  Self explanatory
//
////////////////////////////////////////////////////////////////////////////

#macro dl_meta_child_set_color(MODE, color)
    dl_meta_set_color(color)
#endm


////////////////////////////////////////////////////////////////////////////
//  dl_meta_child_get_class_id
//
//  Self explanatory
//
////////////////////////////////////////////////////////////////////////////

#macro dl_meta_child_get_class_id(MODE, class_id)
    dl_meta_get_class_id(class_id)
#endm

////////////////////////////////////////////////////////////////////////////
//  dl_meta_child_set_class_id
//
//  Self explanatory
//
////////////////////////////////////////////////////////////////////////////

#macro dl_meta_child_set_class_id(MODE, class_id)
    dl_meta_set_class_id(class_id)
#endm


////////////////////////////////////////////////////////////////////////////
//  dl_meta_child_get_parent_buffer_id
//
//  Self explanatory
//
////////////////////////////////////////////////////////////////////////////

#macro dl_meta_child_get_parent_buffer_id(MODE, buf_id)
    ld_field_w_clr[buf_id, 0011, dl_meta[6]]
#endm

////////////////////////////////////////////////////////////////////////////
//  dl_meta_child_set_parent_buffer_id
//
//  Self explanatory
//
////////////////////////////////////////////////////////////////////////////

#macro dl_meta_child_set_parent_buffer_id(MODE, buf_id)
    bits_clr(dl_meta[6], 0, 0xffff)

    // set the ID.

    #if is_ct_const(buf_id)

        #if (buf_id == 0)

            // nothing to do

        #else   // buf_id is not zero

            #if (buf_id <= 0xff)

                alu[dl_meta[6], dl_meta[6], OR, buf_id]

            #else   // buf_id can't fit as unrestricted operand

                .begin

                    .reg    tmp

                    move(tmp, buf_id)

                    alu[dl_meta[6], dl_meta[6], OR, tmp]

                .end

            #endif

        #endif

    #else   // buf_id is not a number

        alu[dl_meta[6], dl_meta[6], OR, buf_id]

    #endif
#endm


/** This macro obtains the next buffer handle.
 */
#macro dl_meta_child_get_buffer_next(MODE, buf_next)
    #if (MODE != META_CHILD_CELL_MODE)
        #error  "This macro must be used in META_CHILD_CELL_MODE only"
    #endif

    .begin

        alu[buf_next, --, B, dl_meta[0]]

    .end
#endm

/** This macro sets the next buffer handle.
 */
#macro dl_meta_child_set_buffer_next(MODE, buf_next)
    #if (MODE != META_CHILD_CELL_MODE)
        #error  "This macro must be used in META_CHILD_CELL_MODE only"
    #endif

    .begin

        alu[dl_meta[0], --, B, buf_next]

    .end
#endm


/** This macro obtains the next packet handle.
 */
#macro dl_meta_child_get_packet_next(MODE, pkt_next)
    #if (MODE != META_CHILD_PACKET_MODE)
        #error  "This macro must be used in META_CHILD_PACKET_MODE only"
    #endif

    .begin

        alu[pkt_next, --, B, dl_meta[7]]

    .end
#endm

/** This macro sets the next packet handle.
 */
#macro dl_meta_child_set_packet_next(MODE, pkt_next)
    #if (MODE != META_CHILD_PACKET_MODE)
        #error  "This macro must be used in META_CHILD_PACKET_MODE only"
    #endif

    .begin

        alu[dl_meta[7], --, B, pkt_next]

    .end
#endm


/** @}
 * @}
 */

#endif /* __DL_META_EXT_UC__ */
