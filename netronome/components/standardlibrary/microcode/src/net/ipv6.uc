/*
 * Copyright (C) 2008-2011 Netronome Systems, Inc.  All rights reserved.
 */

#ifndef __IPV6_UC__
#define __IPV6_UC__

#ifndef NFP_LIB_ANY_NFAS_VERSION
    #if (!defined(__NFP_TOOL_NFAS) ||                       \
        (__NFP_TOOL_NFAS < NFP_SW_VERSION(5, 0, 0, 0)) ||   \
        (__NFP_TOOL_NFAS > NFP_SW_VERSION(6, 1, 0, 255)))
        #error "This standard library is not supported for the version of the SDK currently in use."
    #endif
#endif


#include <net/ipv6.h>
#include <stdmac.uc>
#include <xbuf.uc>
#include <hash.uc>

/** @file ipv6.uc Microengine IPv6 Macros
 * @addtogroup ipv6 Microengine IPv6
 * @{
 *
 * @name Microengine IPv6 Macros
 * @{
 *
 */


// API:
//
//  ipv6_version_verify(in_data, IPHDR_START_BYTE)
//  ipv6_addr_verify(out_result, in_data, IPHDR_START_BYTE, in_addr_origin)
//  ipv6_hop_limit_verify(in_data, IPHDR_START_BYTE)
//  ipv6_next_hdr_verify(out_result, in_data, IPHDR_START_BYTE)
//  ipv6_verify(out_result, in_data, IPHDR_START_BYTE)
//  ipv6_modify(out_data, IPHDR_WR_START_BYTE, in_data, IPHDR_RD_START_BYTE, in_prepend)
//  ipv6_proc(out_result, out_data, IPHDR_WR_START_BYTE, in_data, IPHDR_RD_START_BYTE, in_prepend)
//  ipv6_route_lookup(out_rt_ptr, in_data, IPHDR_START_BYTE, in_trie_addr, TRIE_DEPTH)
//  ipv6_5tuple_lookup(out_index, in_data, IPHDR_START_BYTE, TRIE_ADDR_BASE, KEY_DATA_SD_BASE, TRIE_TYPE)
//
/////////////////////////////////////////////////////////////////////////////////////


// IPv6 Header Format:
//
//  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//  |Version| Traffic Class |           Flow Label                  |
//  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//  |         Payload Length        |  Next Header  |   Hop Limit   |
//  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//  |                                                               |
//  +                                                               +
//  |                                                               |
//  +                         Source Address                        +
//  |                                                               |
//  +                                                               +
//  |                                                               |
//  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//  |                                                               |
//  +                                                               +
//  |                                                               |
//  +                      Destination Address                      +
//  |                                                               |
//  +                                                               +
//  |                                                               |
//  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//

//  ipv6_version_verify
//  Description:
//      Verify version field contains 6 for ipv6 header
//  Inputs:
//      in_data             - name of transfer buffer that contains packet header
//      IPHDR_START_BYTE    - constant to specify the beginning of the IPv6 header
//                            inside the packet header buffer
//
//  Output:
//      condition code      - Z=1 if correct IPV6 version, Z=0 if bad version.
//
//  Size:
//      2 - 4 instructions
//
//  Example:
//      ipv6_version_verify($$packet, 14)
//      bne[bad_version#]
//
#macro ipv6_version_verify(in_data, IPHDR_START_BYTE)
.begin

    .reg version
    xbuf_extract(version, in_data, IPHDR_START_BYTE, IPV6_VERSION_TRAFFIC_CLASS_HI4)
    alu_shf_right(--, 6, -, version, 4)

.end
#endm


/** Check for invalid router addresses.
 *
 * For source address: @n
 * -# multicast address: drop
 * For destination address:
 * -# multicast address: exception
 * -# link-local address: exception
 * -# 0x0 or 0x1: drop
 *
 * @param out_result          SUCCESS or exception/drop code
 * @param in_data             Name of transfer buffer that contains packet header
 * @param IPHDR_START_BYTE    Constant to specify beginning of IPv6 header
 *                            inside packet header buffer
 * @param in_addr_origin      IP_SOURCE or IP_DESTINATION
 *
 * @condition_codes Z=1 if exception/drop, Z=0 if success
 *
 * @instruction_cnt 4 - 35
 *
 * @b Example:
 * @code
 * ipv6_addr_verify(out_result, $$packet, 14, IP_DESTINATION)
 * beq[exception_or_drop#]
 * @endcode
 *
 */
#macro ipv6_addr_verify(out_result, in_data, IPHDR_START_BYTE, in_addr_origin)
    #if (streq(in_addr_origin, IP_SOURCE))
        // check source address
        .begin

            .reg src_addr_127_120
            // check for multicast address, i.e. 8 MSB bits = 0xFF
            xbuf_extract(src_addr_127_120, in_data, IPHDR_START_BYTE, IPV6_SRC_ADDR_127_120)
            alu[--, 0xFF, -, src_addr_127_120]
            beq[end#], defer[1]
            immed[out_result, IPV6_SOURCE_ADDR_MULTICAST]

        .end
    #elif (streq(in_addr_origin, IP_DESTINATION))
        // check destination address
        .begin

            .reg dst_addr_127_96 dst_addr_95_64 dst_addr_63_32 dst_addr_31_0

            // extract 1st 4-byte addr
            xbuf_extract(dst_addr_127_96, in_data, IPHDR_START_BYTE, IPV6_DST_ADDR_127_96)

            // check for multicast address, i.e. 8 MSB bits = 0xFF
            // alu_shf[--, 0xFF, AND~, dst_addr_127_96, >>24]
            alu_shf_right(--, 0xFF, AND~, dst_addr_127_96, 24)
            beq[end#], defer[1]
            immed[out_result, IPV6_EXCP_MULTICAST]

            // check for link-local address, i.e.
            sub_shf_right(--, 0xFE80, dst_addr_127_96, 16)
            beq[end#], defer[1]
            immed[out_result, IPV6_EXCP_LINKLOCAL]

            // check for address 0x0 or 0x1
            alu[--, --, B, dst_addr_127_96]
            bne[success#]

            // extract 2nd 4-byte addr
            xbuf_extract(dst_addr_95_64, in_data, IPHDR_START_BYTE, IPV6_DST_ADDR_95_64)
            alu[--, --, B, dst_addr_95_64]
            bne[success#]

            // extract 3rd 4-byte addr
            xbuf_extract(dst_addr_63_32, in_data, IPHDR_START_BYTE, IPV6_DST_ADDR_63_32)
            alu[--, --, B, dst_addr_63_32]
            bne[success#]

            // extract last 4-byte addr
            xbuf_extract(dst_addr_31_0, in_data, IPHDR_START_BYTE, IPV6_DST_ADDR_31_0)
            alu[--, dst_addr_31_0, AND~, 0x1]
            beq[end#], defer[1]
            immed[out_result, IPV6_DEST_ADDR_ZERO_OR_ONE]

        .end
    #else
        #error "in_addr_origin must be either IP_SOURCE or IP_DESTINATION."
    #endif

success#:
    immed[out_result, IPV6_SUCCESS]

end#:
#endm


/** Verify that hop limit value is greater than 1.
 *
 * @param in_data             Name of transfer buffer that contains packet header
 * @param IPHDR_START_BYTE    Constant to specify beginning of IPv6 header
 *                            inside packet header buffer
 *
 * @condition_codes Exception if >0
 *
 * @instruction_cnt 2 - 4
 *
 * @b Example:
 * @code
 * ipv6_hop_limit_verify($$packet, 14)
 * bgt[exception#]
 * @endcode
 */
#macro ipv6_hop_limit_verify(in_data, IPHDR_START_BYTE)
.begin

    .reg hop_limit
    xbuf_extract(hop_limit, in_data, IPHDR_START_BYTE, IPV6_HOP_LIMIT)
    alu[--, 2, -, hop_limit]

.end
#endm


/** Check for invalid next header values.
 *
 * @param out_result         SUCCESS or exception/drop codes
 * @param in_data            Name of transfer buffer that contains packet header
 * @param IPHDR_START_BYTE   Constant to specify beginning of IPv6 header
 *                           inside packet header buffer
 *
 * @condition_codes Z if exception
 *
 * @instruction_cnt 4 - 13
 *
 * @b Example:
 * @code
 * ipv6_next_hdr_verify(out_result, $$packet, 14)
 * beq[exception#]
 * @endcode
 */
#macro ipv6_next_hdr_verify(out_result, in_data, IPHDR_START_BYTE)
.begin

    .reg next_hdr
    // extract next header
    xbuf_extract(next_hdr, in_data, IPHDR_START_BYTE, IPV6_NEXT_HEADER)

    // check for 0 (Hop-by-hop options header)
    alu[--, --, B, next_hdr]
    beq[end#], defer[1]
    immed[out_result, IPV6_EXCP_OPTIONS]

    // check for 43 (source routing header)
    alu[--, 43, -, next_hdr]
    beq[end#], defer[1]
    immed[out_result, IPV6_EXCP_OPTIONS]

    // check for 60 (destination options header)
    alu[--, 60, -, next_hdr]
    beq[end#], defer[1]
    immed[out_result, IPV6_EXCP_OPTIONS]

    // IPV6_SUCCESS if not exception/drop
    immed[out_result, IPV6_SUCCESS]

end#:

.end
#endm


/** Perform Ipv6 header validation.
 *
 * Verify the following:
 * @arg Version
 * @arg Source address
 * @arg Destination address
 * @arg Hop Limit
 * @arg Next header
 *
 * out_result                0 if IP_SUCCESS, otherwise exception/drop codes
 * @param in_data            Name of transfer buffer that contains packet header
 * @param IPHDR_START_BYTE   Constant to specify beginning of IPv6 header
 *                           inside packet header buffer
 *
 * @instruction_cnt 4 - 99
 *
 * @b Example:
 * @code
 * ipv6_verify(out_result, $$packet, 14)
 * alu[--, out_result, -, IPV6_SUCCESS]
 * bne[exception_or_drop#]
 * @endcode
 */
#macro ipv6_verify(out_result, in_data, IPHDR_START_BYTE)
    // check version
    ipv6_version_verify(in_data, IPHDR_START_BYTE)
    bne[end#], defer[1]
    immed[out_result, IPV6_EXCP_VERSION]

    // check source address
    ipv6_addr_verify(out_result, in_data, IPHDR_START_BYTE, IP_SOURCE)
    beq[end#]

    // check destination address
    ipv6_addr_verify(out_result, in_data, IPHDR_START_BYTE, IP_DESTINATION)
    beq[end#]

    // check hop limit
    ipv6_hop_limit_verify(in_data, IPHDR_START_BYTE)
    bgt[end#], defer[1]
    immed[out_result, IPV6_EXCP_BAD_HOPLIMIT]

    // check next header
    ipv6_next_hdr_verify(out_result, in_data, IPHDR_START_BYTE)
    beq[end#]

    immed[out_result, IPV6_SUCCESS]

end#:
#endm


/** Decrement hop limit by 1 and write the result back.
 *
 * Merge "in_prepend" data to first word if ip header offset is not longword aligned.
 *
 * @param out_data               Name of dram write xfer transfer buffer to contain packet data
 * @param IPHDR_WR_START_BYTE    Constant byte address of ip header start relative to out_data
 * @param in_data                Name of buffer containing packet data.
 *                               If header is in SRAM or DRAM transfer register,
 *                               they must be read xfer registers.
 * @param IPHDR_RD_START_BYTE    Constant byte address of ip header start relative to in_data
 * @param in_prepend             Longword containing bytes to be merged with first word, prepended
 *                               if byte alignment within first ip longword is not 0. Bytes
 *                               to be merged must be at exact byte locations that it will occupy
 *                               in output header. All other bytes must be 0.
 *
 * @note The writes (copies) will not be performed for GPR or local memory types if the input
 *       and output start byte offsets are the same.
 *
 * @instruction_cnt 10 - 16
 *
 * @b Example:
 * @code
 * move(in_prepend, 0xb0b1b200)
 * ipv6_modify(out_gpr, 14, in_prepend, $$in_pkt, 14)
 * @endcode
 */
#macro ipv6_modify(out_data, IPHDR_WR_START_BYTE, in_data, IPHDR_RD_START_BYTE, in_prepend)
    #ifdef READ_ALIGN
        #warning "READ_ALIGN is being redefined."
    #endif
    #ifdef WRITE_ALIGN
        #warning "WRITE_ALIGN is being redefined."
    #endif

    xbuf_xfer_set(_BUF0, in_data, IPHDR_RD_START_BYTE)
    xbuf_xfer_set(_BUF1, out_data, IPHDR_WR_START_BYTE)

    #define_eval READ_ALIGN  (IPHDR_RD_START_BYTE & 0x3)
    #define_eval WRITE_ALIGN (IPHDR_WR_START_BYTE & 0x3)

    #if (READ_ALIGN != WRITE_ALIGN)
        #error 2 "ipv6_modify. read align != write align."
    #else
        #if ( (streq(out_data, in_data)) && (IPHDR_RD_START_BYTE == IPHDR_WR_START_BYTE) )
        // For GPR and local memory xbuf type we do not perform copy when src & dest
        // are same name and start byte offsets are the same
            _GET_XBUF_TYPE(in_data)
            #if ((_XBUF_TYPE == _XBUF_GPR) || (_XBUF_TYPE == _XBUF_LMEM))
                #define _IPV6_NO_NEED_TO_COPY
            #endif
            #undef _XBUF_TYPE
        #endif

        .begin

            .reg temp
            #if (READ_ALIGN == 0)   // iphdr starts at byte 0 of longword
                // iphdr byte offset 7, hop limit (see endian.uc)
                bytefield_decr(_BUF1_REG1, _BUF0_REG1, 3, 3)

                #ifndef _IPV6_NO_NEED_TO_COPY
                    move(_BUF1_REG0, _BUF0_REG0)
                    move(_BUF1_REG2, _BUF0_REG2)
                #endif

            #elif(READ_ALIGN == 1)  // iphdr starts at byte 1 of longword
                // iphdr byte offset 7+1, hop limit (see endian.uc)
                bytefield_decr(_BUF1_REG2, _BUF0_REG2, 0, 0)

                #ifndef _IPV6_NO_NEED_TO_COPY
                    bytefield_select(temp, 1000, in_prepend, NO_LOAD_CC)
                    bytefield_insert(temp, 0111, _BUF0_REG0, NO_LOAD_CC)
                    move(_BUF1_REG0, temp)
                    move(_BUF1_REG1, _BUF0_REG1)
                    move(_BUF1_REG10, _BUF0_REG10)
                #endif

            #elif(READ_ALIGN == 2)  // iphdr starts at byte 2 of longword
                // iphdr byte offset 7+2, hop limit (see endian.uc)
                bytefield_decr(_BUF1_REG2, _BUF0_REG2, 1, 1)

                #ifndef _IPV6_NO_NEED_TO_COPY
                    bytefield_select(temp, 1100, in_prepend, NO_LOAD_CC)
                    alu_op(_BUF1_REG0, temp, +16, _BUF0_REG0)
                    move(_BUF1_REG1, _BUF0_REG1)
                    move(_BUF1_REG10, _BUF0_REG10)
                #endif

            #else
                // iphdr byte offset 7+3, hop limit (see endian.uc)
                bytefield_decr(_BUF1_REG2, _BUF0_REG2, 2, 2)

                #ifndef _IPV6_NO_NEED_TO_COPY
                    bytefield_select(temp, 1110, in_prepend, NO_LOAD_CC)
                    alu_op(_BUF1_REG0, temp, +8, _BUF0_REG0)
                    move(_BUF1_REG1, _BUF0_REG1)
                    move(_BUF1_REG10, _BUF0_REG10)
                #endif

            #endif

        .end

        #ifndef _IPV6_NO_NEED_TO_COPY
            // Copy the rest
            move(_BUF1_REG3, _BUF0_REG3)
            move(_BUF1_REG4, _BUF0_REG4)
            move(_BUF1_REG5, _BUF0_REG5)
            move(_BUF1_REG6, _BUF0_REG6)
            move(_BUF1_REG7, _BUF0_REG7)
            move(_BUF1_REG8, _BUF0_REG8)
            move(_BUF1_REG9, _BUF0_REG9)
        #endif

    #endif

    #ifdef _IPV6_NO_NEED_TO_COPY
        #undef _IPV6_NO_NEED_TO_COPY
    #endif
    #undef READ_ALIGN
    #undef WRITE_ALIGN
#endm


/** Process IPV6 header that starts at read xbuf byte pointer.
 *
 * -# Call ipv6_verify to validate:
 *    @arg version
 *    @arg source and destination addresses
 *    @arg hop limit
 *    @arg next header
 * -# Call ipv6_modify to decrement hop limit value.
 *
 * @param out_result             0 if IP_SUCCESS, otherwise, exception/drop codes
 * @param out_data               Name of dram write xfer transfer buffer to contain packet data
 * @param IPHDR_WR_START_BYTE    Constant byte address of ip header start relative to out_data
 * @param in_data                Name of buffer containing packet data.
 *                               If header is in SRAM or DRAM transfer register,
 *                               they must be read xfer registers.
 * @param IPHDR_RD_START_BYTE    Constant byte address of ip header start relative to in_data
 * @param in_prepend             Longword containing bytes to be merged with first word, prepended
 *                               if byte alignment within first ip longword is not 0. Bytes
 *                               to be merged must be at exact byte locations that it will occupy
 *                               in output header. All other bytes must be 0.
 *
 * @note @arg The writes (copies) will not be performed for GPR or local memory types. The update
 *            is done directly for these types of memory.
 *       @arg in_prepend is not used for GPR/LOCAL_MEM memory types since no copy is performed.
 */
#macro ipv6_proc(out_result, out_data, IPHDR_WR_START_BYTE, in_data, IPHDR_RD_START_BYTE, in_prepend)
    ipv6_verify(out_result, in_data, IPHDR_RD_START_BYTE)
    alu[--, out_result, -, IPV6_SUCCESS]
    bne[end#]

    ipv6_modify(out_data, IPHDR_WR_START_BYTE, in_data, IPHDR_RD_START_BYTE, in_prepend)
end#:
#endm


/// @cond INTERNAL_MACROS
/** Extract 8 bits from the ipaddr into an index.
 *
 * Read the trie entry at Current_Trie_Block[index].
 * If Next_hop field of this trie-entry!= 0, update prev_valid_next_hop.
 * Place the value of Next_trie field into next_trie_block.
 *
 * @param ipaddr                Current 32 bits of IP address that are being looked up
 * @param SHIFT_AMT             Pointer to LSBit which needs to be extracted
 *                              from ipaddr for this step of trie walk, e.g.
 *                              @arg 0 -> Extract bits 7-0 for walk
 *                              @arg 8 -> Extract bits 15-7 for walk
 * @param prev_valid_next_hop   Value of the trie entry of the previous trie walk step.
 *                              Will be updated with value of trie entry
 *                              corresponding to current trie walk step.
 * @param next_trie_block       Index of next trie extracted from trie entry
 *                              corresponding to current 8 bits of IP address
 * @param trie_base             Base address of 0th 8 bit trie block.
 *
 * @instruction_cnt 7-8
 */
#macro _ipv6_trie_8(ipaddr,                \
                    SHIFT_AMT,             \
                    prev_valid_next_hop,   \
                    next_trie_block,       \
                    trie_base)
.begin

    .reg current_next_hop
    .sig sram_trie_signal
    .reg $xfer_reg


    // Convert trie block index to word offset into SRAM
    // next_trie_block = next_trie_block * 256
    shf_left(next_trie_block, next_trie_block, 8)

    // Extract the specified 8 bits from the IP address
    // Use these 8 bits to index into into the trie block
    // Note: The index is in terms of 32 bit long words
    alu_shf_right(next_trie_block, next_trie_block, +8, ipaddr, SHIFT_AMT)

    // Convert to get a byte offset
    // i.e., Multiply by 4
    shf_left(next_trie_block, next_trie_block, 2)

    // by now, we have the trie entry to read
    sram[read, $xfer_reg, trie_base, next_trie_block, 1], \
        ctx_swap[sram_trie_signal]

    // get the next hop pointer
    bytefield_extract(current_next_hop, $xfer_reg, 0, 1, DO_LOAD_CC)

    beq[read_next_trie#]
    // If the next hop is valid at this trie level, we need to update the
    // prev_valid_next_hop with the new value so that it gets carried forward
    move(prev_valid_next_hop, current_next_hop)

read_next_trie#:
    // get the next trie pointer, set cond code
    bytefield_select(next_trie_block, 0011, $xfer_reg, DO_LOAD_CC)

.end
#endm
/// @endcond


/** Perform ip longest prefix match lookup.
 *
 * Use tables written by core Route Table Manager.
 *
 * @param out_rt_ptr         GPR result of the lookup.
 * @param in_data            Name of transfer buffer that contains packet header
 * @param IPHDR_START_BYTE   Constant to specify beginning of IPv6 header
 *                           inside packet header buffer
 * @param in_trie_addr       Constant base address of IPv6 Trie Table.
 *                           Trie table consists of an initial 16 bit trie
 *                           block (64k) followed by 8 bit (1k) trie blocks.
 * @param TRIE_DEPTH         Trie depth 15: 16bit index table followed by 14 8bit index tables
 *                           16-8-8-8-8-8-8-8-8-8-8-8-8-8-8
 * If _IPV6_ROUTE_LOOKUP_IN_CACHE is defined:
 * @arg min: 8 + sizeof(ipv6_route_cache_lookup)
 * @arg max: min + 118 instr(worst case 15 sram access).
 *      else
 * 13 - 125 instr(worst case 15 sram access).
 *
 * @b Example:
 * @code
 * .reg out_rt_ptr
 * ipv6_route_lookup(out_rt_ptr, in_data, 0, in_trie_addr, TRIE15)
 * @endcode
 */
#macro ipv6_route_lookup(out_rt_ptr,       \
                            in_data,          \
                            IPHDR_START_BYTE, \
                            in_trie_addr,     \
                            TRIE_DEPTH)
.begin

    .reg base_16bit_trie, base_8bit_tries
    .reg next_trie_ptr
    .reg result, offset
    .sig sram_swap_signal
    .reg $curr_trie_entry
    .reg da_127_96, da_95_64, da_63_32, da_31_0

    #if (TRIE_DEPTH != TRIE15)
        #error "ipv6_route_lookup. unsupported trie type."
    #endif


    // extract 1st 4-byte addr of dest IP address
    xbuf_extract(da_127_96, in_data, IPHDR_START_BYTE, IPV6_DST_ADDR_127_96)

    // extract 2nd 4-byte addr of dest IP address
    xbuf_extract(da_95_64, in_data, IPHDR_START_BYTE, IPV6_DST_ADDR_95_64)

    // extract 3rd 4-byte addr of dest IP address
    xbuf_extract(da_63_32, in_data, IPHDR_START_BYTE, IPV6_DST_ADDR_63_32)

    // extract last 4-byte addr of dest IP address
    xbuf_extract(da_31_0, in_data, IPHDR_START_BYTE, IPV6_DST_ADDR_31_0)

    immed32(out_rt_ptr, 0)

    #ifdef _IPV6_ROUTE_LOOKUP_IN_CACHE

        // Perform a quick lookup in the cache.
        // This macro shall be implemented by the application
        ipv6_route_cache_lookup(out_rt_ptr,
                                next_trie_ptr,
                                da_127_96,
                                da_95_64,
                                da_63_32,
                                da_31_0)
        alu[--, --, B, next_trie_ptr]
        beq[lookup_end#]

    #endif

    // base_16bit_trie can be either a static constant or from
    // an .import_var. It points to the base address of the 16 bit
    // 64k entry trie table
    immed32(base_16bit_trie, in_trie_addr)

    // The 16 bit 64k entry trie block is
    // followed by 8 bit (256 entry) trie blocks
    // base_8bit_tries points to the base of the 8 bit trie block memory
    // Add 0x40000 (64k * 4) to the table base to get location of
    // 8 bit trie blocks
    alu_shf_left(base_8bit_tries, base_16bit_trie, +, 4, 16)

    ; LPM on first 64 bits of dest IP (SRAM Trie walk)

SRAM_trie_walk_high_64#:

    lookup_127_112#:

    // Extract first 16 bits from IP addr and use them to index into
    // the 16 bit table. Each trie entry is 4 bytes long. So the index
    // needs to be multiplied by 4 to get to correct SRAM location

    ; 16-bit trie-block offset
    alu_shf_right(offset, 0x3, ~AND, da_127_96, 14)
    // Read the trie entry at that 16 bit offset
    sram[read, $curr_trie_entry, base_16bit_trie, offset, 1],
        ctx_swap[sram_swap_signal]

    // Get next hop pointer
    bytefield_extract(out_rt_ptr, $curr_trie_entry, 0, 1, DO_LOAD_CC)

    // get next trie pointer
    bytefield_select(next_trie_ptr, 0011, $curr_trie_entry, DO_LOAD_CC)

    // next_trie_ptr = 0, no route found
    beq[lookup_end#]

    // Start 8 bit trie walk
    // Completely unroll the lookup loop for efficiency
    lookup_111_104#:
    _ipv6_trie_8(da_127_96, 8, out_rt_ptr, next_trie_ptr, base_8bit_tries)
    beq[lookup_end#]

    lookup_103_96#:
    _ipv6_trie_8(da_127_96, 0, out_rt_ptr, next_trie_ptr, base_8bit_tries)
    beq[lookup_end#]

    lookup_95_88#:
    _ipv6_trie_8(da_95_64, 24, out_rt_ptr, next_trie_ptr, base_8bit_tries)
    beq[lookup_end#]

    lookup_87_80#:
    _ipv6_trie_8(da_95_64, 16, out_rt_ptr, next_trie_ptr, base_8bit_tries)
    beq[lookup_end#]

    lookup_79_72#:
    _ipv6_trie_8(da_95_64, 8, out_rt_ptr, next_trie_ptr, base_8bit_tries)
    beq[lookup_end#]

    lookup_71_64#:
    _ipv6_trie_8(da_95_64, 0, out_rt_ptr, next_trie_ptr, base_8bit_tries)
    beq[lookup_end#]

    ; LPM on lower 64 bits of dest IP (SRAM Trie walk)


SRAM_trie_walk_lower_64#:

    lookup_63_56#:
    _ipv6_trie_8(da_63_32, 24, out_rt_ptr, next_trie_ptr, base_8bit_tries)
    beq[lookup_end#]

    lookup_55_48#:
    _ipv6_trie_8(da_63_32, 16, out_rt_ptr, next_trie_ptr, base_8bit_tries)
    beq[lookup_end#]

    lookup_47_40#:
    _ipv6_trie_8(da_63_32, 8, out_rt_ptr, next_trie_ptr, base_8bit_tries)
    beq[lookup_end#]

    lookup_39_32#:
    _ipv6_trie_8(da_63_32, 0, out_rt_ptr, next_trie_ptr, base_8bit_tries)
    beq[lookup_end#]

    lookup_31_24#:
    _ipv6_trie_8(da_31_0, 24, out_rt_ptr, next_trie_ptr, base_8bit_tries)
    beq[lookup_end#]

    lookup_23_16#:
    _ipv6_trie_8(da_31_0, 16, out_rt_ptr, next_trie_ptr, base_8bit_tries)
    beq[lookup_end#]

    lookup_15_8#:
    _ipv6_trie_8(da_31_0, 8, out_rt_ptr, next_trie_ptr, base_8bit_tries)
    beq[lookup_end#]

    lookup_7_0#:
    _ipv6_trie_8(da_31_0, 0, out_rt_ptr, next_trie_ptr, base_8bit_tries)

lookup_end#:

.end
#endm


/** Carries out CRC operation on five tuple(IPv6 source Address, IPv6 Destination Address
 * TCP/UDP source port, TCP/UDP destination port, next header) to get 32-bit crc residue.
 *
 * This 32-bit crc residue is used as hash key to perform hash lookup and find route entry.
 *
 * @param out_index           Index of route entry if success. 0 if fail.
 * @param in_data             Name of transfer buffer that contains packet header
 * @param IPHDR_START_BYTE    Constant to specify beginning of IPv6 header
 *                            inside packet header buffer
 * @param TRIE_ADDR_BASE      Address of trie table, which is sram primary base table
 * @param KEY_DATA_SD_BASE    Base address of key database located in SDRAM
 * @param TRIE_TYPE           Index bits used to address hash trie: HASH_16_4, HASH_FOLD_16:
 *                            @arg HASH_16_4: First lookup uses 16 bits of index, subsequent
 *                                 lookups use 4 bits of index.
 *                            @arg HASH_FOLD_16: First lookup XORs initial index to reduce it by
 *                                 half, then performs a tabqle lookup using 16 bits of
 *                                 half-index, with subsequent chain search until no collision.
 *
 * @instruction_cnt 29 + sizeof(hash_lookup)
 *
 * @b Example:
 * @code
 * .reg out_index
 *  ipv6_5tuple_lookup(out_index,
 *                     in_data,
 *                     IPHDR_START_BYTE,
 *                     TRIE_ADDR_BASE,
 *                     KEY_DATA_SD_BASE,
 *                     HASH_16_4)
 * @endcode
 */
#macro ipv6_5tuple_lookup(out_index, in_data, IPHDR_START_BYTE, TRIE_ADDR_BASE, KEY_DATA_SD_BASE, TRIE_TYPE)
.begin

    .reg tmp_val $table_val
    xbuf_alloc(key, 2, read_write)

    ; Initializie CRC_REMAINDER
    move(tmp_val, 0xffffffff)
    local_csr_wr[CRC_REMAINDER, tmp_val]

    // set high LW to zero
    move(key[1], 0)

    // extract 1st 4-byte of source IP addr and do CRC operation
    xbuf_extract(tmp_val, in_data, IPHDR_START_BYTE, IPV6_SRC_ADDR_127_96)
    crc_be[crc_32, --, tmp_val]

    // extract 2nd 4-byte of source IP addr and do CRC operation
    xbuf_extract(tmp_val, in_data, IPHDR_START_BYTE, IPV6_SRC_ADDR_95_64)
    crc_be[crc_32, --, tmp_val]

    // extract 3rd 4-byte of source IP addr and do CRC operation
    xbuf_extract(tmp_val, in_data, IPHDR_START_BYTE, IPV6_SRC_ADDR_63_32)
    crc_be[crc_32, --, tmp_val]

    // extract last 4-byte of source IP addr and do CRC operation
    xbuf_extract(tmp_val, in_data, IPHDR_START_BYTE, IPV6_SRC_ADDR_31_0)
    crc_be[crc_32, --, tmp_val]

    // extract 1st 4-byte of dest IP addr and do CRC operation
    xbuf_extract(tmp_val, in_data, IPHDR_START_BYTE, IPV6_DST_ADDR_127_96)
    crc_be[crc_32, --, tmp_val]

    // extract 2nd 4-byte of dest IP addr and do CRC operation
    xbuf_extract(tmp_val, in_data, IPHDR_START_BYTE, IPV6_DST_ADDR_95_64)
    crc_be[crc_32, --, tmp_val]

    // extract 3rd 4-byte of dest IP addr and do CRC operation
    xbuf_extract(tmp_val, in_data, IPHDR_START_BYTE, IPV6_DST_ADDR_63_32)
    crc_be[crc_32, --, tmp_val]

    // extract last 4-byte of dest IP addr and do CRC operation
    xbuf_extract(tmp_val, in_data, IPHDR_START_BYTE, IPV6_DST_ADDR_31_0)
    crc_be[crc_32, --, tmp_val]

    // Extract TCP/UDP source port and destination port and carry out CRC
    // Extract 4bytes starting from src port number since
    // 2bytes dest port number follows src port
    xbuf_extract(tmp_val, in_data, IPHDR_START_BYTE, IPV6_SOURCE_PORT, 4)
    crc_be[crc_32, --, tmp_val]

    // Extract next header byte and carry out CRC
    xbuf_extract(tmp_val, in_data, IPHDR_START_BYTE, IPV6_NEXT_HEADER)
    crc_be[crc_32, --, tmp_val]

    ; Five cycle latency to allow CRC to finish.
    nop
    nop
    nop
    nop
    nop

    ; Read CRC remainder
    local_csr_rd[CRC_REMAINDER]
    immed[tmp_val, 0]
    move(key[0], tmp_val)

    #define_eval PRIM_INDEX_START_BIT ((TRIE_TYPE >> 24) & 0xFF)
    #define_eval PRIM_INDEX_BIT_LEN   ((TRIE_TYPE >> 16) & 0xFF)
    #define_eval COLL_INDEX_START_BIT ((TRIE_TYPE >> 8) & 0xFF)
    #define_eval COLL_SHIFT_BITS      ((TRIE_TYPE >> 4) & 0xF)
    #define_eval LOOKUP_TYPE          ((TRIE_TYPE) & 0xF)
    #define_eval COLL_INDEX_BIT_LEN   48
    #define_eval KEY_SIZE             32

    #if (LOOKUP_TYPE == _LOOKUP_TYPE_16_4)
        _hash_read_prim_table($table_val, \
                                key, \
                                TRIE_ADDR_BASE, \
                                PRIM_INDEX_START_BIT, \
                                PRIM_INDEX_BIT_LEN)

        _hash_resolve_16_4(out_index, \
                            $table_val, \
                            key, \
                            KEY_SIZE, \
                            COLL_INDEX_START_BIT, \
                            COLL_INDEX_BIT_LEN, \
                            COLL_SHIFT_BITS, \
                            KEY_DATA_SD_BASE)

    #elif (LOOKUP_TYPE == _LOOKUP_TYPE_FOLD_16)
        _hash_resolve_fold_16(out_index, key, KEY_SIZE, TRIE_ADDR_BASE, KEY_DATA_SD_BASE)
    #else
        #error "Invalid Trie types, valid values are HASH_16_4 & HASH_FOLD_16"
    #endif

    // clean up name space
    #undef PRIM_INDEX_START_BIT
    #undef COLL_INDEX_START_BIT
    #undef COLL_INDEX_BIT_LEN
    #undef PRIM_INDEX_BIT_LEN
    #undef COLL_SHIFT_BITS
    #undef LOOKUP_TYPE
    xbuf_free(key)

.end
#endm


/** @}
 * @}
 */

#endif /* __IPV6_UC__ */
