/*
 * Copyright (C) 2008-2011 Netronome Systems, Inc.  All rights reserved.
 */

#ifndef __ETHERNET_UC__
#define __ETHERNET_UC__

#ifndef NFP_LIB_ANY_NFAS_VERSION
    #if (!defined(__NFP_TOOL_NFAS) ||                       \
        (__NFP_TOOL_NFAS < NFP_SW_VERSION(5, 0, 0, 0)) ||   \
        (__NFP_TOOL_NFAS > NFP_SW_VERSION(6, 1, 0, 255)))
        #error "This standard library is not supported for the version of the SDK currently in use."
    #endif
#endif

#include <net/ethernet.h>
#include <stdmac.uc>
#include <xbuf.uc>
#include <bytefield.uc>


/** @file ethernet.uc Microengine Ethernet Macros
 * @addtogroup ethernet Microengine Ethernet
 * @{
 *
 * @name Microengine Ethernet Macros
 * @{
 *
 */

// API:
//
//    ether_addr_verify(out_stat, in_src, IN_HDR_START_BYTE)
//    ether_is_bpdu(out_stat, in_src, IN_HDR_START_BYTE)
//    ether_is_bcast(out_stat, in_src, IN_HDR_START_BYTE)
//    ether_is_mcast(out_stat, in_src, IN_HDR_START_BYTE)
//
//////////////////////////////////////////////////////////////////////////////////////

// Ether Header Format:
//                      1                   2                   3
//  0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
//  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//  |               MAC Destination address                         |
//  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//  |                               |    MAC source Address         |
//  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//  |                                                               |
//  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//  |         Type                  |
//  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//


/** Check the ethernet destination address for broadcast address.
 *
 * Broadcast address is FF-FF-FF-FF-FF-FF.
 *
 * @param out_status            @arg 0(ETHER_SUCCESS) - If dest addr is a broadcast  address
 *                              @arg 0x92(ETHER_BCAST_FAILURE) - If dest addr is not a broadcast address @n
                                NOTE: User can also pass in 0 for out_stat and have
 *                              condition code check in application.
 * @param in_src                Transfer buffer that contains packet header
 * @param IN_HDR_START_BYTE     Constant to specify beginning of ether header
 *                              inside packet header buffer
 *
 * @instruction_cnt 4 - 8
 *
 * @condition_codes @arg Z - 1 If broadcast
 *                  @arg Z - 0  If not broadcast.
 *
 * @b Example:
 * @code
 *     .reg in_src, result
 *
 *     xbuf_alloc(in_src, 4, read_write)
 *
 *     immed32(in_src[0], 0xAAAAAAff)
 *     immed32(in_src[1], 0xffff01ff)
 *     immed32(in_src[2], 0x01020304)
 *     immed32(in_src[3], 0xabcdefab)
 *     ether_is_bcast(0, in_src, 3)
 *     beq[pass1#]
 *     // Failure
 *     br[end1#]
 * pass1#:
 *    // Success
 * end1#:
 *    xbuf_free(in_src)
 * @endcode
 */
#macro ether_is_bcast(out_stat, in_src, IN_HDR_START_BYTE)
.begin

    _ether_is_bcast(out_stat, in_src, IN_HDR_START_BYTE, ETHER_DEST_START_ADDR)

.end
#endm


/// @cond INTERNAL_MACROS
// 4 - 8 instr
#macro _ether_is_bcast(out_stat, in_src, IN_HDR_START_BYTE, ADDR_OFFSET)
.begin

    .reg dest_addr

    #ifdef _DEST_ADDR_START
        #warning "ether_is_bcast: _DEST_ADDR_START is being re-defined"
    #endif
    #define_eval _DEST_ADDR_START ADDR_OFFSET

    // Check the 1st 4 bytes of destination address
    xbuf_extract(dest_addr, in_src, IN_HDR_START_BYTE, _DEST_ADDR_START, 4)
    bytefield_comp(dest_addr, 0, 3, ETHER_BCAST_ADDR1)
    #if (!(is_ct_const(out_stat) || isimport(out_stat)))
        bne[end#], defer[1]
        immed[out_stat, ETHER_BCAST_FAILURE]
    #else
        bne[end#]
    #endif
next#:
    // Check the remaining 2 bytes of destination address
    #define_eval _DEST_ADDR_START _DEST_ADDR_START+4
//  #warning 2 IN_HDR_START_BYTE, _DEST_ADDR_START
    xbuf_extract(dest_addr, in_src, IN_HDR_START_BYTE, _DEST_ADDR_START, 4)
    #if (!(is_ct_const(out_stat) || isimport(out_stat)))
        alu_shf_right(out_stat, ETHER_BCAST_ADDR2, - , dest_addr, 16)
        beq[end#]   //Correct bcast packet
        immed[out_stat, ETHER_BCAST_FAILURE]    //Wrong bcast packet
    #else
        alu_shf_right(--, ETHER_BCAST_ADDR2, - , dest_addr, 16)
    #endif

end#:
    #undef _DEST_ADDR_START

.end
#endm
/// @endcond


/** Check the ethernet destination address for multicast address.
 *
 * Multicast address is 01-XX-XX-XX-XX-XX.
 *
 * @param out_stat            @arg 0(ETHER_SUCCESS) - If dest addr is a multicast address
 *                            @arg 0x93(ETHER_MCAST_FAILURE) - If dest addr is not a multicast address @n
 *                            NOTE: User can also pass in 0 for out_stat and have
 *                            condition code check in application
 * @param in_src              Transfer buffer that contains packet header
 * @param IN_HDR_START_BYTE   Constant to specify beginning of ether header
 *                            inside packet header buffer
 *
 * @instruction_cnt 2 - 3

 * @condition_codes @arg Z - 1 If multicast
 *                  @arg Z - 0 If not multicast.
 *
 * @b Example:
 * @code
 *     .reg in_src, result
 *
 *     xbuf_alloc(in_src, 6, read_write)
 *
 *     immed32(in_src[0], 0xAAAAAA01)
 *     immed32(in_src[1], 0xffff0106)
 *     immed32(in_src[2], 0x01020304)
 *     immed32(in_src[3], 0xabcdefab)
 *     ether_is_mcast(0, in_src, 3)
 *     beq[pass3#]
 *     // Failure
 *     br[end3#]
 * pass3#:
 *     // Success
 * end3#:
 *     xbuf_free(in_src)
 * @endcode
 */
#macro ether_is_mcast(out_stat, in_src, IN_HDR_START_BYTE)
.begin

    _ether_is_mcast(out_stat, in_src, IN_HDR_START_BYTE, ETHER_DEST_START_ADDR)

.end
#endm


/// @cond INTERNAL_MACROS
//Size 2-3 instructions
#macro _ether_is_mcast(out_stat, in_src, IN_HDR_START_BYTE, ADDR_OFFSET)
.begin

    .reg dest_addr

    // Check the MSB for 0x01 of 1st 6 bytes of destination address
    xbuf_extract(dest_addr, in_src, IN_HDR_START_BYTE, ADDR_OFFSET, 1)
    #if (!(is_ct_const (out_stat) || isimport(out_stat)))
        alu[out_stat, 0x01, -, dest_addr]
        beq[end#]
        immed[out_stat, ETHER_MCAST_FAILURE]
    #else
        alu[--, 0x01, -, dest_addr]

    #endif
end#:

.end
#endm
/// @endcond


/** Check the ethernet type field for BPDU protocol.
 *
 * The 2 byte type field should be 0x0000.
 *
 * @param out_status           @arg 0(ETHER_SUCCESS) - If BPDU packet
 *                             @arg 0x94(ETHER_BPDU_FAILURE) - If not BPDU packet @n
 *                             NOTE: User can also pass in 0 for out_stat and have
 *                             condition code check in their application
 * @paramin_src                Transfer buffer that contains packet header
 * @param IN_HDR_START_BYTE    Constant to specify beginning of ether header
 *                             inside packet header buffer
 *
 * @instruction_cnt 2 - 3
 *
 * @condition_codes @arg Z - 1 If BPDU packet
 *                  @arg Z - 0  If not BPDU packet.
 *
 * @b Example 1:
 * @code
 * .reg in_src, result
 *
 * xbuf_alloc(in_src, 6, read_write)
 *
 * immed32(in_src[0], 0xffffffff)
 * immed32(in_src[1], 0xffff0102)
 * immed32(in_src[2], 0xffffffff)
 * immed32(in_src[3], 0x0000efab)
 * ether_is_bpdu(result, in_src, 0)
 * CHECK_VAL_EQ(result, ETHER_SUCCESS, case_num)
 * xbuf_free(in_src)
 *
 * @b Example 2:
 * @code
 *     .reg in_src, result
 *
 *     xbuf_alloc(in_src, 6, read_write)
 *
 *     immed32(in_src[0], 0xffffffff)
 *     immed32(in_src[1], 0xffff0102)
 *     immed32(in_src[2], 0xffffffff)
 *     immed32(in_src[3], 0xabcdefab)
 *     immed32(in_src[4], 0xabcdefab)
 *     ether_is_bpdu(0, in_src, 3)
 *     beq[pass7#]
 *     // Failure
 *     br[end7#]
 * pass7#:
 *    // Success
 * end7#:
 *    xbuf_free(in_src)
 * @endcode
 */
#macro ether_is_bpdu(out_stat, in_src, IN_HDR_START_BYTE)
.begin

    .reg dest_addr
    // Check for 0x0000 of the type field
    xbuf_extract(dest_addr, in_src, IN_HDR_START_BYTE, ETHER_TYPE_START_ADDR, 2)
    #if (!(is_ct_const (out_stat) || isimport(out_stat)))
        alu[out_stat, 0x0000, -, dest_addr]
        beq[end#]
        immed[out_stat, ETHER_BPDU_FAILURE]
    #else
        alu[--, 0x0000, -, dest_addr]
    #endif
end#:

.end
#endm


/** Checks for valid source and destination address.
 *
 * -# Destination should not be reserved address
 * -# Source should not be broadcast address
 * -# Source should not be multicast address
 *
 * @param out_status          @arg 0(ETHER_NO_EXCEPTION)  - If valid address
 *                            @arg 0x90(ETHER_INVALID_SRC_ADDRESS) - If invalid source address
 *                            @arg 0x91(ETHER_RESERVED_ADDRESS) - If destination is reserved address
 *                            NOTE: User can also pass in 0 for out_stat and have
 *                            condition code check in application
 * @param in_src               Transfer buffer that contains packet header
 * @param IN_HDR_START_BYTE    Constant to specify beginning of ether header
 *                             inside packet header buffer
 *
 * @condition_codes @arg Z - 1 If valid address
 *                  @arg Z - 0  If not valid address.
 *
 * @instruction _cnt 10 - 12
 *
 * @b Example:
 *     .reg in_src, result
 *
 *     xbuf_alloc(in_src, 6, read_write)
 *
 *     immed32(in_src[0], 0x01020304)
 *     immed32(in_src[1], 0xffff01ff)
 *     immed32(in_src[2], 0xffffffff)
 *     immed32(in_src[3], 0x0000efab)
 *     immed32(in_src[4], 0x12340000)
 *     ether_addr_verify(0, in_src, 0)
 *     beq[pass64#]
 *     // Failure
 *     br[end64#]
 * pass64#:
 *     // Success
 * end64#:
 *    xbuf_free(in_src)
 *
 */
#macro ether_addr_verify(out_stat, in_src, IN_HDR_START_BYTE)
.begin

    .reg dest_addr, temp

    // Check the source MAC address for multicast address
    _ether_is_mcast(out_stat, in_src, IN_HDR_START_BYTE, ETHER_SRC_START_ADDR)
    beq[invalid_address#]

    //Check the source MAC address for broadcast address
//  #warning IN_HDR_START_BYTE, ETHER_SRC_START_ADDR
    _ether_is_bcast(out_stat, in_src, IN_HDR_START_BYTE, ETHER_SRC_START_ADDR)
    beq[invalid_address#]

    //Check the destination address for reserved address
    #ifdef _DEST_ADDR_START
        #warning "ether_is_bcast: _DEST_ADDR_START is being re-defined"
    #endif
    #define_eval _DEST_ADDR_START ETHER_DEST_START_ADDR

    //Check the 1st 4 bytes of destination address
    // Reserved addresses are:
    //01-80-c2-00-00-00
    //01-80-c2-00-00-01
    //01-80-c2-00-00-02
    //01-80-c2-00-00-03
    //01-80-c2-00-00-04
    //01-80-c2-00-00-05
    //01-80-c2-00-00-06
    //01-80-c2-00-00-07
    //01-80-c2-00-00-08
    //01-80-c2-00-00-09
    //01-80-c2-00-00-0A
    //01-80-c2-00-00-0B
    //01-80-c2-00-00-0C
    //01-80-c2-00-00-0D
    //01-80-c2-00-00-0E
    //01-80-c2-00-00-0F
    xbuf_extract(dest_addr, in_src, IN_HDR_START_BYTE, _DEST_ADDR_START, 4)
    bytefield_comp(dest_addr, 0, 3, ETHER_RESERVED_ADDR1)
    #if (!(is_ct_const(out_stat) || isimport(out_stat)))
        bne[end#], defer[1] //Destination is not a reserved address
        immed[out_stat, ETHER_SUCCESS]
    #else
        bne[end#]   //Destination is not a reserved address
    #endif

reserved_addr#:
    //Check whether 5th is 0x00 and 6th byte is 0x0X from MSB
    #define_eval _DEST_ADDR_START _DEST_ADDR_START+4
    xbuf_extract(dest_addr, in_src, IN_HDR_START_BYTE, _DEST_ADDR_START, 2)
    alu_op(--, dest_addr, AND, 0x0000FFF0)
    #if (!(is_ct_const(out_stat) || isimport(out_stat)))
        beq[end#], defer[1]
        immed[out_stat, ETHER_RESERVED_ADDRESS]
        immed[out_stat, ETHER_NO_EXCEPTION]
    #endif
    br[end#]

invalid_address#:
    #if (!(is_ct_const(out_stat) || isimport(out_stat)))
        immed[out_stat, ETHER_INVALID_SRC_ADDRESS]
    #endif
end#:
    #undef _DEST_ADDR_START

.end
#endm


/** @}
 * @}
 */

#endif /* __ETHERNET_UC__ */
