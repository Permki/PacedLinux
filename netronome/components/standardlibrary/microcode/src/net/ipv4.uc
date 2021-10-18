/*
 * Copyright (C) 2008-2011 Netronome Systems, Inc.  All rights reserved.
 */

#ifndef __IPV4_UC__
#define __IPV4_UC__

#ifndef NFP_LIB_ANY_NFAS_VERSION
    #if (!defined(__NFP_TOOL_NFAS) ||                       \
        (__NFP_TOOL_NFAS < NFP_SW_VERSION(5, 0, 0, 0)) ||   \
        (__NFP_TOOL_NFAS > NFP_SW_VERSION(6, 1, 0, 255)))
        #error "This standard library is not supported for the version of the SDK currently in use."
    #endif
#endif

#include <net/ipv4.h>
#include <stdmac.uc>
#include <bytefield.uc>
#include <sram.uc>
#include <hash.uc>
#include <localmem.uc>

/** @file ipv4.uc Microengine IPv4 Macros
 * @addtogroup ipv4 Microengine IPv4
 * @{
 *
 * @name Microengine IPv4 Macros
 * @{
 *
 */


// API:
//
//      ipv4_addr_verify(out_result, in_addr, WHICH_ADDR)
//      ipv4_cksum_verify(in_data, in_iphdr_start_byte)
//      ipv4_is_dbcast(out_status, in_addr)
//      ipv4_is_snmp(out_status, in_data, in_iphdr_start_byte)
//      ipv4_modify(out_data, in_new_iphdr_start_byte, in_prepend, in_data, in_iphdr_start_byte)
//      ipv4_proc(out_exception, out_data, in_new_iphdr_start_byte, in_data, in_iphdr_start_byte)
//      ipv4_route_lookup(out_rt_ptr, in_ip_da, in_trie_addr, TRIE_TYPE)
//      ipv4_total_len_verify(in_data, in_iphdr_start_byte)
//      ipv4_ttl_verify(in_data, in_iphdr_start_byte)
//      ipv4_verify (out_exception, in_data, in_iphdr_start_byte)
//      ipv4_5tuple_lookup(out_index, in_data, IPHDR_START_BYTE, TRIE_ADDR_BASE, KEY_DATA_SD_BASE, TRIE_TYPE)
//      ipv4_tos_modify(out_data, IPHDR_WR_START_BYTE, in_tos_value, in_data, IPHDR_RD_START_BYTE, in_prepend)
//      ipv4_n_tuple_extract(out_n_tuple, in_data, IPHDR_START_BYTE, TUPLE_TYPE)
//      ipv4_n_tuple_lookup(out_index, in_data, IPHDR_START_BYTE, TRIE_ADDR_BASE, KEY_DATA_SD_BASE, TRIE_TYPE, TUPLE_TYPE)
//
/////////////////////////////////////////////////////////////////////////////////////////


/** Validate ip source or destinaion address as per the "SHOULD" part
 * in RFC1812 (RFC's usually have MUST parts and SHOULD parts).
 *
 * @param out_result    GPR that contains IPV4_SUCCESS if no exception
 *                      appropriate exception code
 * @param in_addr       GPR that contains ipv4 address
 * @param WHICH_ADDR    Constant with possible values:
 *                      @arg IP_SRC  for IPv4 source address
 *                      @arg IP_DEST for IPv4 destination address
 *
 * @instruction_cnt 8 instructions if WHICH_ADDR == IP_SRC
 * @instruction_cnt 12 instructions if WHICH_ADDR = IP_DEST
 *
 * @see stdmac.uc
 *
 * @b Example:
 * @code
 * ipv4_addr_verify(out_result, source ip addr, IP_SRC)
 * br!=byte[out_result, 0, IPV4_SUCCESS, exception_handler_ipvr_addr#]
 * @endcode
 */
#macro ipv4_addr_verify (out_result, in_addr, WHICH_ADDR)
    #if ((WHICH_ADDR != IP_SRC) && (WHICH_ADDR != IP_DEST))
        #error "Invalid value for WHICH_ADDR. Valid values are IP_SRC or IP_DEST"
    #endif

    // check address != 0.x.x.x
    alu_shf_right(--, 0x0, -, in_addr, 24)
    beq[invalid_addr#]

    // check for 127.x.x.x in in_addr
    alu_shf_right(--, 0x7f, -, in_addr, 24)
    beq[invalid_addr#]

    // check for class D address 224.x.x.x to 239.x.x.x
    //(multicast address)
    alu_shf_right(--, 0xE, -, in_addr, 28)
    #if (WHICH_ADDR == IP_SRC)
        //drop packet if src address is multicast
        beq[invalid_addr#]
    #else
        //send exception if dest addr is multicast
        beq[multicast_addr#]
    #endif

    //check for 255.255.255.255
    .begin

        .reg temp
        alu[temp, --, ~b, 0x0]
        alu[--, in_addr, -, temp]
        #if (WHICH_ADDR == IP_SRC)
            //drop packet if src address is 255.255.255.255
            beq[invalid_addr#]
        #else
            //send exception if dest addr
            beq[limited_boardcast_addr#]
        #endif

    .end

    // check for class E
    alu_shf_right(--, 0xf, -, in_addr, 28)
    beq[invalid_addr#]

    //valid address
    br[end#], defer[1]
    alu[out_result, --, B, IPV4_SUCCESS]

    #if (WHICH_ADDR == IP_DEST)
        multicast_addr#:
            br[end#], defer[1]
            alu[out_result, --, B, IPV4_EXCP_MULTICAST]
        limited_boardcast_addr#:
            br[end#], defer[1]
            alu[out_result, --, B, IPV4_EXCP_LIMITED_BROADCAST]
    #endif

    invalid_addr#:
    alu[out_result, --, B, IPV4_INVALID_ADDRESS]

    end#:
#endm


/** Verify ip checksum.
 *
 * @param in_data                Name of read xfer transfer buffer containing packet data
 * @param IPHDR_RD_START_BYTE    Constant byte address of ip header start relative to in_data
 *
 * @condition_codes Good checksum if condition code == 0, otherwise have bad checksum.
 *
 * @instruction_cnt 13 instr for aligment 0
 * @instruction_cnt 17 instr for aligment 2
 * @instruction_cnt 24 instr for aligment 1 or 3
 *
 * @see stdmac.uc, endian.uc
 *
 * @b Example:
 * @code
 * ipv4_cksum_verify(pkt, BYTEOFFSET14)   // ip header starts at byte 14 of ethernet packet
 * @endcode
 *
 * @note LITTLE_ENDIAN is not supported yet (code is available but not validated).
 *
 */
#macro ipv4_cksum_verify(in_data, IPHDR_RD_START_BYTE)
    #ifdef LITTLE_ENDIAN
        #if (strstr(in_data, $$))
            _ipv4_checksum_verify_le_d(in_data, IPHDR_RD_START_BYTE)
        #else
            _ipv4_checksum_verify_le(in_data, IPHDR_RD_START_BYTE)
        #endif
    #else
        #if (strstr(in_data, $$))
            _ipv4_checksum_verify_be_d(in_data, IPHDR_RD_START_BYTE)
        #else
            _ipv4_checksum_verify_be(in_data, IPHDR_RD_START_BYTE)
        #endif
    #endif
#endm


/// @cond INTERNAL_MACROS
/** Verify ip header  checksum, little-endian.
 *
 * Internal macro used by ipv4_cksum_verify.
 * Uses halfword adds to reduce total instructions.
 *
 * @param in_data                Name of buffer containing packet data.
 *                               If header is in SRAM or DRAM transfer register,
 *                               they must be read xfer registers.
 * @param IPHDR_RD_START_BYTE    Constant byte address of ip header start relative to in_data
 *
 * @condition_codes: Good checksum if condition code == 0, otherwise have bad checksum.
 *
 * @instruction_cnt 14
 *
 * Latency: 14 cycles
 *
 * @b Example:
 * @code
 * _ipv4_cksum_verify_le(pkt, BYTEOFFSET14)   // ip header starts at byte 14 of ethernet packet
 * @endcode
 *
 */
#macro _ipv4_checksum_verify_le(in_data, IPHDR_RD_START_BYTE)
.begin

    .reg accum

    //xbuf_xfer_set(_SRD_XFER, in_data, IPHDR_RD_START_BYTE)  // set xbuf tokens _SRD_XFER0 to _SRD_XFER7

    xbuf_xfer_set(_BUF0, in_data, IPHDR_RD_START_BYTE)  // set xbuf tokens _SRD_XFER0 to _SRD_XFER7
    #ifdef READ_ALIGN
        #warning "READ_ALIGN is being redefined."
    #endif
    #define_eval READ_ALIGN (IPHDR_RD_START_BYTE & 0x3)

    #if (READ_ALIGN == 0)
        //shf_right(accum, _SRD_XFER0, 16)
        //alu_op(accum, accum, +16, _SRD_XFER0)
        shf_right(accum, _BUF0_REG0, 16)
        alu_op(accum, accum, +16, _BUF0_REG0)
    #elif (READ_ALIGN == 2)
        shf_right(accum, _BUF0_REG0, 16)
        alu_op(accum, accum, +16, _BUF0_REG5)
    #else
        _ipv4_checksum_verify_oddalign(in_data, IPHDR_RD_START_BYTE)
    #endif

    #if ((READ_ALIGN == 0) || (READ_ALIGN == 2))
        .begin

            .reg temp

            alu_shf_right(accum, accum, +, _BUF0_REG1, 16)
            alu_op(accum, accum, +16, _BUF0_REG1)
            alu_shf_right(accum, accum, +, _BUF0_REG2, 16)
            alu_op(accum, accum, +16, _BUF0_REG2)
            alu_shf_right(accum, accum, +, _BUF0_REG3, 16)
            alu_op(accum, accum, +16, _BUF0_REG3)
            alu_shf_right(accum, accum, +, _BUF0_REG4, 16)
            alu_op(accum, accum, +16, _BUF0_REG4)

            rot_right(temp, accum, 16)                     // rotate by 16
            alu_op(accum, accum, +, temp)
            alu_op(accum, accum, +carry, 1)                // add 1 to 0xffff ffff to get zero result

        .end //temp
    #endif
    #undef READ_ALIGN       // clean up name space

.end
#endm
/// @endcond


/// @cond INTERNAL_MACROS
/** Verify ip header  checksum, little-endian, halfword aligned.
 *
 * Internal macro used by ipv4_cksum_verify.
 * Uses halfword adds to reduce total instructions.
 * Operates on SDRAM transfer registers.
 *
 * @param in_data               Name of sdram read xfer transfer buffer containing packet data
 * @param IPHDR_RD_START_BYTE   Constant byte address of ip header start relative to in_data
 *
 * @condition_codes Good checksum if condition code == 0, otherwise have bad checksum.
 *
 * @instruction_cnt 14
 *
 * Latency: 14 cycles
 *
 * @b Example:
 * @code
 * _ipv4_cksum_verify_le_d(pkt, BYTEOFFSET14)     // ip header starts at byte 14 of ethernet packet
 * @endcode
 *
 */
#macro _ipv4_checksum_verify_le_d(in_data, IPHDR_RD_START_BYTE)
.begin

    .reg accum

    xbuf_xfer_set(_DRD_XFER, in_data, IPHDR_RD_START_BYTE)   // set xbuf tokens _DRD_XFER0 to _DRD_XFER7
    #ifdef READ_ALIGN
        #warning "READ_ALIGN is being redefined."
    #endif
    #define_eval READ_ALIGN (IPHDR_RD_START_BYTE & 0x3)
    #if (READ_ALIGN == 0)
        shf_right(accum, _DRD_XFER0, 16)
        alu_op(accum, accum, +16, _DRD_XFER0)
    #elif (READ_ALIGN == 2)
        shf_right(accum, _DRD_XFER0, 16)
        alu_op(accum, accum, +16, _DRD_XFER5)
    #else
        _ipv4_checksum_verify_oddalign_d(in_data, IPHDR_RD_START_BYTE)
    #endif
    #if ((READ_ALIGN == 0) || (READ_ALIGN == 2))
        .begin

            .reg temp
            alu_shf_right(accum, accum, +, _DRD_XFER1, 16)
            alu_op(accum, accum, +16, _DRD_XFER1)
            alu_shf_right(accum, accum, +, _DRD_XFER2, 16)
            alu_op(accum, accum, +16, _DRD_XFER2)
            alu_shf_right(accum, accum, +, _DRD_XFER3, 16)
            alu_op(accum, accum, +16, _DRD_XFER3)
            alu_shf_right(accum, accum, +, _DRD_XFER4, 16)
            alu_op(accum, accum, +16, _DRD_XFER4)
            bytefield_select(temp, 1100, accum)            // get high 16 of the total
            alu_shf_left(accum, temp, +, accum, 16)        // add low 16 bits to upper 16
            alu_shf_left(temp, 1, +carry, temp, 16)        // add last carry +1, temp B op=0
            alu_shf_left(accum, accum, +, temp, 16)        // add 1<<16 to 0xffff to get zero result

        .end //temp
    #endif
    #undef READ_ALIGN       // clean up name space

.end
#endm
/// @endcond


/// @cond INTERNAL_MACROS
/** Verify ip header  checksum, big-endian.
 *
 * Internal macro used by ipv4_cksum_verify.
 * Uses 32 bit add with carry to reduce the total number of instructions.
 *
 * @param in_data                Name of buffer containing packet data.
 *                               If header is in SRAM or DRAM transfer register,
 *                               they must be read xfer registers.
 * @param IPHDR_RD_START_BYTE    Constant byte address of ip header start relative to in_data
 *
 * @instruction_cnt 15 for aligment 0
 * @instruction_cnt 19 for aligment 2
 * @instruction_cnt 25 for aligment 1 or 3
 *
 * Latency: 15-25 cycles
 *
 * @condition_codes Good checksum if condition code == 0, otherwise have bad checksum.
 *
 * @b Example:
 * @code
 * _ipv4_cksum_verify_be(pkt, BYTEOFFSET14)      // ip header starts at byte 14 of ethernet packet
 * @endcode
 *
 */
#macro _ipv4_checksum_verify_be(in_data, IPHDR_RD_START_BYTE)
.begin

    .reg accum

    xbuf_xfer_set(_BUF0, in_data, IPHDR_RD_START_BYTE)

    #ifdef READ_ALIGN
        #warning "READ_ALIGN is being redefined."
    #endif
    #define_eval READ_ALIGN (IPHDR_RD_START_BYTE & 0x3)
    #if (READ_ALIGN == 0)

        move(accum, _BUF0_REG0)
        alu_op(accum, accum, +, _BUF0_REG1)
    #elif (READ_ALIGN == 2)
        alu_op(accum, 0, +16, _BUF0_REG0)
        alu_op(accum, accum, +, _BUF0_REG1)
        alu_shf_right(accum, accum, +carry, _BUF0_REG5, 16)
    #else
        _ipv4_checksum_verify_oddalign(in_data, IPHDR_RD_START_BYTE)
    #endif
    #if ((READ_ALIGN == 0) || (READ_ALIGN == 2))
        .begin

            .reg temp
            alu_op(accum, accum, +carry, _BUF0_REG2)
            alu_op(accum, accum, +carry, _BUF0_REG3)
            alu_op(accum, accum, +carry, _BUF0_REG4)
            alu_op(accum, accum, +carry, 0)                 // add in previous carry
            bytefield_select(temp, 1100, accum, NO_LOAD_CC) // get high 16 of the total
            alu_shf_left(accum, temp, +, accum, 16)         // add low 16 bits to upper 16
            alu_shf_left(temp, 1, +carry, temp, 16)         // temp = last carry +1
            alu_shf_left(accum, accum, +, temp, 16)         // add 1<<16 to 0xffff to get zero result

        .end    //temp

    #endif
    #undef READ_ALIGN

.end
#endm
/// @endcond


/// @cond INTERNAL_MACROS
/** Verify ip header  checksum, big-endian, halfword aligned.
 *
 * Internal macro used by ipv4_cksum_verify.
 * Uses 32 bit add with carry to reduce the total number of instructions.
 *
 * @param in_data               Name of sdram read xfer transfer buffer containing packet data
 * @param IPHDR_RD_START_BYTE   Constant byte address of ip header start relative to in_data
 *
 * @condition_codes Good checksum if condition code == 0, otherwise have bad checksum.
 *
 * @instruction_cnt 13 instr for aligment 0
 * @instruction_cnt 17 instr for aligment 2
 * @instruction_cnt 24 instr for aligment 1 or 3
 *
 * Latency: 13-24 cycles
 *
 * @b Example:
 * @code
 * _ipv4_cksum_verify_be(pkt, BYTEOFFSET14)     // ip header starts at byte 14 of ethernet packet
 * @endcode
 *
 */
#macro _ipv4_checksum_verify_be_d(in_data, IPHDR_RD_START_BYTE)
.begin

    .reg accum

    xbuf_xfer_set(_DRD_XFER, in_data, IPHDR_RD_START_BYTE) // set xbuf tokens _DRD_XFER0 to _DRD_XFER7
    #ifdef READ_ALIGN
        #warning "READ_ALIGN is being redefined."
    #endif
    #define_eval READ_ALIGN (IPHDR_RD_START_BYTE & 0x3)
    #if (READ_ALIGN == 0)
        move(accum, _DRD_XFER0)
        alu_op(accum, accum, +, _DRD_XFER1)
    #elif (READ_ALIGN == 2)
        alu_op(accum, 0, +16, _DRD_XFER0)
        alu_op(accum, accum, +, _DRD_XFER1)
        alu_shf_right(accum, accum, +carry, _DRD_XFER5, 16)
    #else
        _ipv4_checksum_verify_oddalign_d(in_data, IPHDR_RD_START_BYTE)
    #endif
    #if ((READ_ALIGN == 0) || (READ_ALIGN == 2))
        .begin

            .reg temp
            alu_op(accum, accum, +carry, _DRD_XFER2)
            alu_op(accum, accum, +carry, _DRD_XFER3)
            alu_op(accum, accum, +carry, _DRD_XFER4)
            alu_op(accum, accum, +carry, 0)                 // add in previous carry
            bytefield_select(temp, 1100, accum, NO_LOAD_CC) // get high 16 of the total
            alu_shf_left(accum, temp, +, accum, 16)         // add low 16 bits to upper 16
            alu_shf_left(temp, 1, +carry, temp, 16)         // add last carry +1, temp B op=0
            alu_shf_left(accum, accum, +, temp, 16)         // add 1<<16 to 0xffff to get zero result

        .end //temp
    #endif
    #undef READ_ALIGN

.end
#endm
/// @endcond


/// @cond INTERNAL_MACROS
/** Support for _ipv4_checksum_verify_be/le, if byte alignment is 1 or 3.
 *
 * @instruction_cnt 25
 */
#macro _ipv4_checksum_verify_oddalign(in_data, IPHDR_RD_START_BYTE)
    //xbuf_xfer_set(_SRD_XFER, in_data, IPHDR_RD_START_BYTE)    // set xbuf tokens _SRD_XFER0 to _SRD_XFER7

    xbuf_xfer_set(_BUF0, in_data, IPHDR_RD_START_BYTE)

    #ifdef START_BYTE
        #warning "START_BYTE is being redefined."
    #endif
    #ifdef END_BYTE
        #warning "END_BYTE is being redefined."
    #endif
    #define_eval START_BYTE (IPHDR_RD_START_BYTE & 0x3)
    #define_eval END_BYTE ((START_BYTE + 3) & 0x3)

    .begin

        .reg ck_temp ck_accum

        bytefield_dbl_extract(ck_accum, _BUF0_REG0, START_BYTE, _BUF0_REG1, END_BYTE, NO_LOAD_CC)
        bytefield_dbl_extract(ck_temp, _BUF0_REG1, START_BYTE, _BUF0_REG2, END_BYTE, NO_LOAD_CC)
        #if (strstr(in_data, $) == 0)   //Check for GPR
            alu[ck_temp, --, B, ck_temp]    //Move ck_temp to bank B
        #endif

        alu_op(ck_accum, ck_accum, +, ck_temp)

        bytefield_dbl_extract(ck_temp, _BUF0_REG2, START_BYTE, _BUF0_REG3, END_BYTE, NO_LOAD_CC)
        alu_op(ck_accum, ck_accum, +carry, ck_temp)
        bytefield_dbl_extract(ck_temp, _BUF0_REG3, START_BYTE, _BUF0_REG4, END_BYTE, NO_LOAD_CC)
        alu_op(ck_accum, ck_accum, +carry, ck_temp)
        bytefield_dbl_extract(ck_temp, _BUF0_REG4, START_BYTE, _BUF0_REG5, END_BYTE, NO_LOAD_CC)

        alu_op(ck_accum, ck_accum, +carry, ck_temp)
        alu_op(ck_accum, ck_accum, +carry, 0)                 // add in previous carry
        bytefield_select(ck_temp, 1100, ck_accum, NO_LOAD_CC) // get high 16 of the total
        alu_shf_left(ck_accum, ck_temp, +, ck_accum, 16)      // add low 16 bits to upper 16
        alu_shf_left(ck_temp, 1, +carry, ck_temp, 16)         // add last carry +1, ck_temp B op=0
        alu_shf_left(ck_accum, ck_accum, +, ck_temp, 16)      // add 1<<16 to 0xffff to get zero result

    .end
    #undef START_BYTE
    #undef END_BYTE
#endm
/// @endcond


/// @cond INTERNAL_MACROS
/** Support for _ipv4_checksum_verify_be_d/le_d, if byte alignment is 1 or 3.
 *
 * @instruction_cnt 24
 */
#macro _ipv4_checksum_verify_oddalign_d(in_data, IPHDR_RD_START_BYTE)
    xbuf_xfer_set(_DRD_XFER, in_data, IPHDR_RD_START_BYTE)   // set xbuf tokens _DRD_XFER0 to _DRD_XFER7

    #ifdef START_BYTE
        #warning "START_BYTE is being redefined."
    #endif
    #ifdef END_BYTE
        #warning "END_BYTE is being redefined."
    #endif
    #define_eval START_BYTE (IPHDR_RD_START_BYTE & 0x3)
    #define_eval END_BYTE ((START_BYTE + 3) & 0x3)

    .begin

        .reg ck_temp ck_accum
        bytefield_dbl_extract(ck_accum, _DRD_XFER0, START_BYTE, _DRD_XFER1, END_BYTE, NO_LOAD_CC)
        bytefield_dbl_extract(ck_temp, _DRD_XFER1, START_BYTE, _DRD_XFER2, END_BYTE, NO_LOAD_CC)
        alu_op(ck_accum, ck_accum, +, ck_temp)
        bytefield_dbl_extract(ck_temp, _DRD_XFER2, START_BYTE, _DRD_XFER3, END_BYTE, NO_LOAD_CC)
        alu_op(ck_accum, ck_accum, +carry, ck_temp)
        bytefield_dbl_extract(ck_temp, _DRD_XFER3, START_BYTE, _DRD_XFER4, END_BYTE, NO_LOAD_CC)
        alu_op(ck_accum, ck_accum, +carry, ck_temp)
        bytefield_dbl_extract(ck_temp, _DRD_XFER4, START_BYTE, _DRD_XFER5, END_BYTE, NO_LOAD_CC)
        alu_op(ck_accum, ck_accum, +carry, ck_temp)

        alu_op(ck_accum, ck_accum, +carry, 0)                 ; add in previous carry
        bytefield_select(ck_temp, 1100, ck_accum, NO_LOAD_CC) ; get high 16 of the total
        alu_shf_left(ck_accum, ck_temp, +, ck_accum, 16)      ; add low 16 bits to upper 16
        alu_shf_left(ck_temp, 1, +carry, ck_temp, 16)         ; add last carry +1, ck_temp B op=0
        alu_shf_left(ck_accum, ck_accum, +, ck_temp, 16)      ; add 1<<16 to 0xffff to get zero result

    .end
    #undef START_BYTE
    #undef END_BYTE
#endm
/// @endcond


/** Check supplied ip address to see if it is a directed broadcast address for a locally attached subnet.
 *
 * @param out_status             0 if supplied ip address is not a directed broadcast address,
 *                               otherwise IPV4_DBCAST_ADDRESS
 * @param in_addr                IP address to check
 * @param SRAM_DBCAST_BASE       Constant base address of dbcast address table
 * @param SRAM_READ_BLOCK_COUNT  Constant to specify sram read block count in long words.
 *                               This would dictate number of long words per SRAM read.
 *                               Value of SRAM_READ_BLOCK_COUNT must be
 *                               divisible by 2 and within the range [2..16].
 *
 * @condition_codes Condition codes for out_status are set.
 *
 * @instruction_cnt 16-21 instructions per lookup (the total number of instructions depends
 *                  on how many times the lookup is performed)
 *
 * Memory access: 1 SRAM access per lookup
 *
 * @see RFC 2644
 *
 * @b Example:
 * @code
 * check src_ipv4_address for directed broadcast address
 * ipv4_is_dbcast(out_status, ipv4_src_address, DBCAST_BASE_ADDR, 4)
 * nop
 * beq[drop_packet#], defer[1]
 * @endcode
 *
 * -# Compute hash on the in_addr. An IP address of form A.B.C.D produces
 *  a hash_result of form A^(A.B)^(A.B.C)^(A.B.C.D).
 * -# Take the last 8 bits of the hash result and multiply by the block size to get the offset
 *    (to align with the block boundary).
 * -# Set Dbcast_addr= SRAM_DBCAST_BASE + offset.
 * -# Perform Sram Read of SRAM_READ_BLOCK_COUNT long words at SRAM location Dbcast_addr.
 * -# Compare each entry within the block with in_addr, under the following conditions: @n
 *    -# If the entry matches with in_addr return success.
 *    -# If a zero entry is found, return failure.
 *    -# If the last entry in the block is zero, return failure.
 *    -# If the last entry is non zero, set the Dbcast_addr to the last entry
 *       and continue from step 4.
 *
 *
 *  ---------------  index 0
 * |block0 addr1   |
 * |---------------
 * |block0 addr2   |
 *  ---------------
 * |block0 addr3   |
 * |---------------
 * |addr of the    |
 * |continue block0|----->
 *  ---------------
 * |block1 addr1   |
 * |---------------
 * |block1 addr2   |
 *  ---------------
 * ....

 */
#macro ipv4_is_dbcast(out_status, in_addr, SRAM_DBCAST_BASE, SRAM_READ_BLOCK_COUNT)
.begin

    .reg dbcast_table, table_offset
    .sig tmp_signal1

    #if (SRAM_READ_BLOCK_COUNT <= 1)
        #error "SRAM_READ_BLOCK_COUNT should be > 1"
    #endif

    #if (SRAM_READ_BLOCK_COUNT > 16)
        #error "Maximum SRAM_READ_BLOCK_COUNT is 16"
    #endif

    #ifdef DIVIDEND
        #warning "DIVIDEND is being redefined"
    #endif

    #ifdef DIV_TWO2SHIFT
        #warning "DIV_TWO2SHIFT is being redefined"
    #endif

    #ifdef DIV_SHIFT
        #warning "DIV_SHIFT is being redefined"
    #endif

    // Find what DIV_SHIFT gives (2^DIV_SHIFT == DIVIDEND)
    #define_eval DIVIDEND (SRAM_READ_BLOCK_COUNT)
    #define_eval DIV_SHIFT 0
    #define_eval DIV_TWO2SHIFT (1 << DIV_SHIFT)
    #while((DIV_SHIFT < 32) && (DIVIDEND != DIV_TWO2SHIFT))
        #define_eval DIV_SHIFT (DIV_SHIFT + 1)
        #define_eval DIV_TWO2SHIFT (1 << DIV_SHIFT)
    #endloop

    #if (DIV_TWO2SHIFT != DIVIDEND)
        #error 1 "SRAM_READ_BLOCK_COUNT must be a power of 2"
    #endif

    #undef DIVIDEND     // clean up name space
    #undef DIV_TWO2SHIFT        // clean up name space

    xbuf_alloc($table_data, SRAM_READ_BLOCK_COUNT, read)

    ;  Given an IP address of form A.B.C.D produce
    ;   a hash_result of form A^(A.B)^(A.B.C)^(A.B.C.D)

    .reg hash_result
    move(hash_result, in_addr)
    alu_shf_right(hash_result, hash_result, XOR, in_addr, 8)
    alu_shf_right(hash_result, hash_result, XOR, in_addr, 16)
    alu_shf_right(hash_result, hash_result, XOR, in_addr, 24)

    //
    //  We use the hash result and & with byte_mask and use it as
    //  an index (0..255)into the directed broadcast address hash table block.
    alu_op(table_offset, hash_result, AND, 0xFF)


    // +2 to convert into byte address
    #define_eval DIV_SHIFT (DIV_SHIFT+2)
    alu_shf[table_offset, --, B, table_offset, <</**/DIV_SHIFT/**/]


    #undef DIV_SHIFT    // cleanup name space

    //  dbcast_table contains the full address (base+offset)
    add(dbcast_table, SRAM_DBCAST_BASE, table_offset)

    //
    //  Pseudo code for directed broadcast
    //  address lookup is as follows:
    //
    //    check_dbcast_table_slot:
    //      Read sram block addr:dbcast_addr
    //      For each entry in the block
    //          if dbcast_table[offset] == 0
    //              exit with cc set for not a dbcast address
    //          else if dbcast_table[offset] == in_addr
    //              exit with cc set for a dbcast address
    //          endif
    //      For the last entry in the block
    //          if dbcast_table[offset] == 0
    //              exit with cc set for not a dbcast address
    //          else
    //              set dbcast_addr = dbcast_table[offset]
    //              goto check_dbcast_table_slot
    //          endif
    //
    .reg dbcast_addr
check_dbcast_table_slot#:
    #if (SRAM_READ_BLOCK_COUNT <= 8)
    ; fetch and check dbcast table entry at current offset.
        sram_read($table_data[0], dbcast_table, 0, SRAM_READ_BLOCK_COUNT, tmp_signal1, tmp_signal1, optimize_mem)
    #else
        .begin

            .reg tmp_count
            move(tmp_count, SRAM_READ_BLOCK_COUNT)
            sram_read($table_data[0], dbcast_table, 0, tmp_count, tmp_signal1, tmp_signal1, optimize_mem)

        .end
    #endif

    #ifdef __DBCAST_SRAM_RD_INDEX
        #warning "__DBCAST_SRAM_RD_INDEX is being redefined".
    #endif

    #define_eval __DBCAST_SRAM_RD_INDEX 0

    // last entry in the table contains the next ptr
    #while (__DBCAST_SRAM_RD_INDEX < (SRAM_READ_BLOCK_COUNT-1))
        move(dbcast_addr, $table_data[/**/__DBCAST_SRAM_RD_INDEX/**/]) // fetch dbcast table entry.
        beq[exit#], defer[1]                        // if table entry is zero

        immed32(out_status, 0)                          //   then not a dbcast address,
                                                        //   exit with out_status = FALSE (0)
                                                        // get ready for the next index
        alu_op(--, dbcast_addr, XOR, in_addr)     // else if table entry doesn't matches ipv4_addr.

        beq[exit#], defer[1]    //   then check next dbcast table entry.

        immed32(out_status, IPV4_DBCAST_ADDRESS)        // exit with out_status = IPV4_DBCAST_ADDRESS
                                                        // else /* it is a dbcast address */
        #define_eval __DBCAST_SRAM_RD_INDEX (__DBCAST_SRAM_RD_INDEX+1)
    #endloop

    // get the next ptr from the last entry within the block
    move(dbcast_table, $table_data[/**/__DBCAST_SRAM_RD_INDEX/**/])
    bne[check_dbcast_table_slot#], defer[1]
    immed32(out_status, 0) // if next ptr is zero then not a dbcast address

    #undef __DBCAST_SRAM_RD_INDEX   // clean up name space

exit#:                                              // endif
    xbuf_free($table_data)

.end
#endm


/** Get protocol type of the packet and check if it is SNMP.
 *
 * @param out_status        @arg 0 if not SNMP
 *                          @arg SNMP_PROTOCOL - for snmp packets
 * @param in_data           Transfer buffer that contains packet header
 * @param IPHDR_START_BYTE  Constant to specify beginning of IPv4
 *                          header inside packet header buffer
 *
 * @instruction_cnt 7
 *
 */
#macro ipv4_is_snmp (out_status, in_data, IPHDR_START_BYTE)
.begin

    .reg dst_port

    xbuf_extract(dst_port, in_data, IPHDR_START_BYTE, IP_DESTINATION_PORT)

    alu_op(--, dst_port, -, SNMP_PORT1)  // If port is Port 160 (SNMP, send to control plane)
    beq[end#], defer[1]
    immed32(out_status, SNMP_PROTOCOL)
    alu_op(--, dst_port, -, SNMP_PORT2) // If port is  Port 161 (SNMP, send to control plane)
    beq[end#], defer[1]
    immed32(out_status, SNMP_PROTOCOL)
    immed32(out_status, 0x0)

.end
end#:
#endm


/** Modify checksum and ttl in the ip header.
 *
 * If IP header not on longword boundary, merge in_prepend with first word.
 * Write modified ip header to dram write transfer registers.
 * Read and write start bytes must be the same.
 *
 * @param out_data                Name of dram write xfer transfer buffer to contain packet data
 * @param IPHDR_WR_START_BYTE     Constant byte address of ip header start relative to out_data
 * @param in_prepend              Longword containing bytes to be merged with first word, prepended
 *                                if byte alignment within first ip longword is not 0. Bytes
 *                                to be merged must be at exact byte locations that it will occupy
 *                                in output header. All other bytes must be 0.
 * @param in_data                 Name of buffer containing packet data.
 *                                If header is in SRAM or DRAM transfer register,
 *                                they must be read xfer registers.
 * @param IPHDR_RD_START_BYTE     Constant byte address of ip header start relative to in_data
 *
 * @instruction_cnt 14 instructions for alignment 0
 * @instruction_cnt 15 instructions for alignment 2
 * @instruction_cnt 20-21 instructions for aligment 1 or 3
 *
 * Latency: 14-21 cyles
 *
 * @see stdmac.uc, endian.uc
 *
 * @b Example:
 * @code
 * // ip header starts at byte 1 of ethernet packet
 * move(in_prepend, 0xb0000000)
 * ipv4_modify(modified_pkt, BYTEOFFSET1, in_prepend, BYTEOFFSET1)
 *
 * // ip header starts at byte 3 of ethernet packet
 * move(in_prepend, 0xb0b1b200)
 * ipv4_modify(modified_pkt, BYTEOFFSET3, in_prepend, BYTEOFFSET3)
 * @endcode
 *
 */
#macro ipv4_modify(out_data, IPHDR_WR_START_BYTE, in_prepend, in_data, IPHDR_RD_START_BYTE)
#ifdef READ_ALIGN
    #warning "READ_ALIGN is being redefined."
#endif
#ifdef WRITE_ALIGN
    #warning "WRITE_ALIGN is being redefined."
#endif

#if (strstr(in_data, $$))
    xbuf_xfer_set(_DRD_XFER, in_data, IPHDR_RD_START_BYTE)     // set xbuf tokens _DRD_XFER0 to _DRD_XFER7
    xbuf_xfer_set(_DWR_XFER, out_data, IPHDR_WR_START_BYTE)    // set xbuf tokens _DWR_XFER0 to _DWR_XFER7

    #define_eval READ_ALIGN (IPHDR_RD_START_BYTE & 0x3)
    #define_eval WRITE_ALIGN (IPHDR_WR_START_BYTE & 0x3)

    #if (READ_ALIGN != WRITE_ALIGN)
        #error 2 "ipv4_modify. read align != write align."
    #else
        .begin

            .reg temp
            #if (READ_ALIGN == 0)                               ; iphdr starts at byte 0 of longword
                move(_DWR_XFER0, _DRD_XFER0)
                move(_DWR_XFER1, _DRD_XFER1)
                bytefield_decr(temp, _DRD_XFER2, 0, 0)                 ; iphdr byte offset 8, ttl(see endian.uc)
                ipv4_cksum_modify(_DWR_XFER2, temp, 2)             ; iphdr byte offset 10-11, checksum
                move(_DWR_XFER3, _DRD_XFER3)
                move(_DWR_XFER4, _DRD_XFER4)
            #elif(READ_ALIGN == 1)
                //merge byte 0 of in_prepend_lwd
                #ifdef LITTLE_ENDIAN
                    bytefield_select(temp, 1110, _DRD_XFER0, NO_LOAD_CC)
                    alu_op(_DWR_XFER0, temp, or, in_prepend)
                #else
                    ld_field_w_clr[temp, 1000, in_prepend]
                    bytefield_insert(temp, 0111, _DRD_XFER0, NO_LOAD_CC)
                    move(_DWR_XFER0, temp)
                #endif
                move(_DWR_XFER1, _DRD_XFER1)
                bytefield_decr(temp, _DRD_XFER2, 1, 1)             // (see endian.uc) byte 8+1
                _ipv4_cksum_B3align_modify(_DWR_XFER2, _DWR_XFER3, temp, _DRD_XFER3)       ; byte 10+1, straddles 2 words
                move(_DWR_XFER4, _DRD_XFER4)
                move(_DWR_XFER5, _DRD_XFER5)
            #elif(READ_ALIGN == 2)                              ; iphdr starts at byte 2 of longword 3
                #ifdef LITTLE_ENDIAN
                    bytefield_select(temp, 1100, _DRD_XFER0, NO_LOAD_CC)
                    alu_op(_DWR_XFER0, temp, or, in_prepend)
                #else
                    ld_field_w_clr[temp, 1100, in_prepend]
                    alu_op(_DWR_XFER0, temp, +16, _DRD_XFER0)
                #endif
                move(_DWR_XFER1, _DRD_XFER1)
                bytefield_decr(_DWR_XFER2, _DRD_XFER2, 2, 2)               // (see endian.uc) byte 8+2
                ipv4_cksum_modify(_DWR_XFER3, _DRD_XFER3, 0)           ; byte 10+2
                move(_DWR_XFER4, _DRD_XFER4)
                move(_DWR_XFER5, _DRD_XFER5)
            #else
                #ifdef LITTLE_ENDIAN
                    bytefield_select(temp, 1000, _DRD_XFER0, NO_LOAD_CC)
                    ld_field[temp, 0111, in_prepend]
                    move(_DWR_XFER0, temp)
                #else
                    ld_field_w_clr[temp, 1110, in_prepend]
                    alu_op(_DWR_XFER0, temp, +8, _DRD_XFER0)
                #endif
                move(_DWR_XFER1, _DRD_XFER1)
                bytefield_decr(_DWR_XFER2, _DRD_XFER2, 3, 3)               // (see endian.uc) byte 8+3
                ipv4_cksum_modify(_DWR_XFER3, _DRD_XFER3, 1)           ; byte 10+2
                move(_DWR_XFER4, _DRD_XFER4)
                move(_DWR_XFER5, _DRD_XFER5)
            #endif

        .end
    #endif

#else

//  xbuf_xfer_set(_SRD_XFER, in_data, IPHDR_RD_START_BYTE)     // set xbuf tokens _SRD_XFER0 to _SRD_XFER7
//  xbuf_xfer_set(_DWR_XFER, out_data, IPHDR_WR_START_BYTE)    // set xbuf tokens _DWR_XFER0 to _DWR_XFER7

    xbuf_xfer_set(_BUF0, in_data, IPHDR_RD_START_BYTE)

    xbuf_xfer_set(_BUF1, out_data, IPHDR_WR_START_BYTE)

    #define_eval READ_ALIGN (IPHDR_RD_START_BYTE & 0x3)
    #define_eval WRITE_ALIGN (IPHDR_WR_START_BYTE & 0x3)

    #if (READ_ALIGN != WRITE_ALIGN)
        #error 2 "ipv4_modify. read align != write align."
    #else
        .begin

            .reg temp
            #if (READ_ALIGN == 0)   ; iphdr starts at byte 0 of longword

                move(_BUF1_REG0, _BUF0_REG0)
                move(_BUF1_REG1, _BUF0_REG1)
                bytefield_decr(temp, _BUF0_REG2, 0, 0)       ; iphdr byte offset 8, ttl(see endian.uc)
                ipv4_cksum_modify(_BUF1_REG2, temp, 2)       ; iphdr byte offset 10-11, checksum
                move(_BUF1_REG3, _BUF0_REG3)
                move(_BUF1_REG4, _BUF0_REG4)

            #elif(READ_ALIGN == 1)

                #ifdef LITTLE_ENDIAN
                    bytefield_select(temp, 1110, _BUF0_REG0, NO_LOAD_CC)
                    alu_op(_BUF1_REG0, temp, +8, in_prepend)
                #else
                    bytefield_select(temp, 1000, in_prepend, NO_LOAD_CC)
                    bytefield_insert(temp, 0111, _BUF0_REG0, NO_LOAD_CC)
                    move(_BUF1_REG0, temp)
                #endif
                move(_BUF1_REG1, _BUF0_REG1)
                bytefield_decr(temp, _BUF0_REG2, 1, 1)       // (see endian.uc) byte 8+1
                ; byte 10+1, straddles 2 words
                _ipv4_cksum_B3align_modify(_BUF1_REG2, _BUF1_REG3, temp, _BUF0_REG3)
                move(_BUF1_REG4, _BUF0_REG4)
                move(_BUF1_REG5, _BUF0_REG5)

            #elif(READ_ALIGN == 2)                              // iphdr starts at byte 2 of longword 3

                #ifdef LITTLE_ENDIAN
                    bytefield_select(temp, 1100, _BUF0_REG0, NO_LOAD_CC)
                    alu_op(_BUF1_REG0, temp, +16, in_prepend)
                #else
                    bytefield_select(temp, 1100, in_prepend, NO_LOAD_CC)
                    alu_op(_BUF1_REG0, temp, +16, _BUF0_REG0)
                #endif
                move(_BUF1_REG1, _BUF0_REG1)
                bytefield_decr(_BUF1_REG2, _BUF0_REG2, 2, 2)           // (see endian.uc) byte 8+2
                ipv4_cksum_modify(_BUF1_REG3, _BUF0_REG3, 0)           ; byte 10+2
                move(_BUF1_REG4, _BUF0_REG4)
                move(_BUF1_REG5, _BUF0_REG5)
            #else
                #ifdef LITTLE_ENDIAN
                    bytefield_select(temp, 1000, _BUF0_REG0, NO_LOAD_CC)
                    bytefield_insert(temp, 0111, in_prepend)
                    move(_BUF1_REG0, temp)
                #else
                    bytefield_select(temp, 1110, in_prepend, NO_LOAD_CC)
                    alu_op(_BUF1_REG0, temp, +8, _BUF0_REG0)
                #endif
                move(_BUF1_REG1, _BUF0_REG1)
                bytefield_decr(_BUF1_REG2, _BUF0_REG2, 3, 3)          // (see endian.uc) byte 8+3
                ipv4_cksum_modify(_BUF1_REG3, _BUF0_REG3, 1)          ; byte 10+2
                move(_BUF1_REG4, _BUF0_REG4)
                move(_BUF1_REG5, _BUF0_REG5)
            #endif

        .end
    #endif

#endif

#undef READ_ALIGN
#undef WRITE_ALIGN
#endm


/// @cond INTERNAL_MACROS
/** Modify checksum and ttl in the ip header.
 *
 * Write modified ip header to dram write transfer registers.
 * Read and write start bytes must be the same.
 *
 * @param out_data               Name of dram write xfer transfer buffer to contain packet data
 * @param IPHDR_WR_START_BYTE    Constant byte address of ip header start relative to out_data
 * @param in_prepend             Longword containing bytes to be merged with first word, prepended
 *                               if byte alignment within first ip longword is not 0
 * @param in_data                Name of dram read xfer transfer buffer containing packet data
 * @param IPHDR_RD_START_BYTE    Constant byte address of ip header start relative to in_data
 *
 * @instruction_cnt 14 instructions for alignment 0
 * @instruction_cnt 15 instructions for alignment 2
 * @instruction_cnt 20-21 instructions for aligment 1 or 3
 *
 * Latency: 14-21 cycles
 *
 * @see stdmac.uc, endian.uc
 *
 * @b Example:
 * @code
 * // ip header starts at byte 14 of ethernet packet
 * ipv4_modify(modified_pkt, BYTEOFFSET14, pkt, BYTEOFFSET14)
 * @endcode
 *
 */
#macro _ipv4_modify_d(out_data, IPHDR_WR_START_BYTE, in_prepend, in_data, IPHDR_RD_START_BYTE)
xbuf_xfer_set(_DRD_XFER, in_data, IPHDR_RD_START_BYTE)     // set xbuf tokens _DRD_XFER0 to _DRD_XFER7
xbuf_xfer_set(_DWR_XFER, out_data, IPHDR_WR_START_BYTE)    // set xbuf tokens _DWR_XFER0 to _DWR_XFER7

#ifdef READ_ALIGN
    #warning "READ_ALIGN is being redefined."
#endif
#ifdef WRITE_ALIGN
    #warning "WRITE_ALIGN is being redefined."
#endif
#define_eval READ_ALIGN (IPHDR_RD_START_BYTE & 0x3)
#define_eval WRITE_ALIGN (IPHDR_WR_START_BYTE & 0x3)

#if (READ_ALIGN != WRITE_ALIGN)
    #error 2 "ipv4_modify. read align != write align."
#else
    .begin

        .reg temp
        #if (READ_ALIGN == 0)                               ; iphdr starts at byte 0 of longword
            move(_DWR_XFER0, _DRD_XFER0)
            move(_DWR_XFER1, _DRD_XFER1)
            bytefield_decr(temp, _DRD_XFER2, 0, 0)             ; iphdr byte offset 8, ttl(see endian.uc)
            ipv4_cksum_modify(_DWR_XFER2, temp, 2)             ; iphdr byte offset 10-11, checksum
            move(_DWR_XFER3, _DRD_XFER3)
            move(_DWR_XFER4, _DRD_XFER4)
        #elif(READ_ALIGN == 1)
            #ifdef LITTLE_ENDIAN
                bytefield_select(temp, 1110, _DRD_XFER0, NO_LOAD_CC)
                alu_op(_DWR_XFER0, temp, +8, in_prepend)
            #else
                bytefield_select(temp, 1000, in_prepend, NO_LOAD_CC)
                bytefield_insert(temp, 0111, _DRD_XFER0, NO_LOAD_CC)
                move(_DWR_XFER0, temp)
            #endif
            move(_DWR_XFER0, _DRD_XFER0)
            move(_DWR_XFER1, _DRD_XFER1)
            bytefield_decr(temp, _DRD_XFER2, 1, 1)             // (see endian.uc) byte 8+1
            _ipv4_cksum_B3align_modify(_DWR_XFER2, _DWR_XFER3, temp, _DRD_XFER3)       ; byte 10+1, straddles 2 words
            move(_DWR_XFER4, _DRD_XFER4)
            move(_DWR_XFER5, _DRD_XFER5)
        #elif(READ_ALIGN == 2)                              ; iphdr starts at byte 2 of longword
            #ifdef LITTLE_ENDIAN
                bytefield_select(temp, 1100, _DRD_XFER0, NO_LOAD_CC)
                alu_op(_DWR_XFER0, temp, +16, in_prepend)
            #else
                bytefield_select(temp, 1100, in_prepend, NO_LOAD_CC)
                alu_op(_DWR_XFER0, temp, +16, _DRD_XFER0)
            #endif
            move(_DWR_XFER0, _DRD_XFER0)
            move(_DWR_XFER1, _DRD_XFER1)
            bytefield_decr(_DWR_XFER2, _DRD_XFER2, 2, 2)           // (see endian.uc) byte 8+2
            ipv4_cksum_modify(_DWR_XFER3, _DRD_XFER3, 0)           // byte 10+2
            move(_DWR_XFER4, _DRD_XFER4)
            move(_DWR_XFER5, _DRD_XFER5)
        #else
            #ifdef LITTLE_ENDIAN
                bytefield_select(temp, 1000, _DRD_XFER0, NO_LOAD_CC)
                bytefield_insert(temp, 0111, in_prepend, NO_LOAD_CC)
                move(_DWR_XFER0, temp)
            #else
                bytefield_select(temp, 1110, in_prepend, NO_LOAD_CC)
                alu_op(_DWR_XFER0, temp, +8, _DRD_XFER0)
            #endif
            move(_DWR_XFER0, _DRD_XFER0)
            move(_DWR_XFER1, _DRD_XFER1)
            bytefield_decr(_DWR_XFER2, _DRD_XFER2, 3, 3)           // (see endian.uc) byte 8+3
            ipv4_cksum_modify(_DWR_XFER3, _DRD_XFER3, 1)           // byte 10+2
            move(_DWR_XFER4, _DRD_XFER4)
            move(_DWR_XFER5, _DRD_XFER5)
        #endif

    .end
#endif
#undef READ_ALIGN
#undef WRITE_ALIGN
#endm
/// @endcond


/** Increment checksum to compensate for subtracting 1 from ttl.
 *
 * checksum = bytes 10-11 of ip header
 *
 * @param out_dest        GPR destination
 * @param in_source       GPR that contains IP header
 * @param BYTE_OFFSET     Constant byte offset pointing to checksum field
 *
 * @instruction_cnt 8
 *
 * Latency: 8 cycles
 *
 * @see stdmac.uc, endian.uc. RFC 1141
 *
 * @b Example:
 * @code
 * ipv4_cksum_modify(new_ipv4_wd2, prev_ipv4_wd2, 2)   //cksum at bytes 2-3
 * @endcode
 *
 */
#macro ipv4_cksum_modify(out_dest, in_source, BYTE_OFFSET)
.begin

    .reg temp0 temp1
    #ifdef READ_BYTE0
        #warning "READ_BYTE0 is being redefined."
    #endif
    #ifdef READ_BYTE1
        #warning "READ_BYTE1 is being redefined."
    #endif
    #define_eval READ_BYTE0 BYTE_OFFSET
    #define_eval READ_BYTE1 (BYTE_OFFSET + 1)
    bytefield_incr(temp0, in_source, READ_BYTE0, READ_BYTE0)
    bytefield_extract(temp1, temp0, READ_BYTE0, READ_BYTE0, DO_LOAD_CC)
    beq[add_carry#]
    br[end#], defer[1]
    move(out_dest, temp0)      // can be a write transfer register
add_carry#:
    bytefield_incr(out_dest, temp0, READ_BYTE0, READ_BYTE1)
end#:
    #undef READ_BYTE0
    #undef READ_BYTE1

.end
#endm


/// @cond INTERNAL_MACROS
/** Support ipv4_cksum_modify if checksum byte alignment is 3 on source and destination.
 *
 * @instruction_cnt 13-14
 *
 * Latency : 13-14 cycles
 *
 */
#macro _ipv4_cksum_B3align_modify(out_dest0, out_dest1, in_source0, in_source1)
.begin

    .reg new_cksum merged_cksum
    #ifdef READ_BYTE0
        #warning "READ_BYTE0 is being redefined."
    #endif
    #ifdef READ_BYTE1
        #warning "READ_BYTE1 is being redefined."
    #endif
    #define_eval READ_BYTE0 3
    #define_eval READ_BYTE1 0
    bytefield_dbl_extract(merged_cksum, in_source0, READ_BYTE0, in_source1, READ_BYTE1, NO_LOAD_CC)
    //increment the most significant byte to compensate fo ttl decrement at same byte offset
    alu_shf_left(new_cksum, merged_cksum, +, 1, 8)
    // check MSB for zero, by shifting right and truncate
    alu_shf_right(--, 0, +8, new_cksum, 8)
    beq[add_carry#]
    br[end#]
add_carry#:
    alu_op(new_cksum, new_cksum, +, 1)    // now new_cksum in bytes 2-3
end#:

    // now insert the incremented cksum

    #ifdef LITTLE_ENDIAN
        bytefield_select(merged_cksum, 0111, in_source0, NO_LOAD_CC)
        alu_shf_left(out_dest0, merged_cksum, +, new_cksum, 16)      // le byte3
        bytefield_select(merged_cksum, 1110, in_source1, NO_LOAD_CC)
        alu_op(out_dest1, merged_cksum, +8, new_cksum)         // le byte0

    #else
        bytefield_select(merged_cksum, 1110, in_source0, NO_LOAD_CC)
        alu_shf_right(out_dest0, merged_cksum, +8, new_cksum, 8)       // be byte3
        bytefield_select(merged_cksum, 0111, in_source1, NO_LOAD_CC)
        alu_shf_left(out_dest1, merged_cksum, +, new_cksum, 24)        // be byte0
    #endif

    #undef READ_BYTE0
    #undef READ_BYTE1

.end
#endm
/// @endcond


/** Perform ip longest prefix match lookup. Uses tables written by core Route Table Manager.
 *
 * This uses a dual lookup of a 64k entry table and trie block lookups.
 * It uses a maximum of 5 sram reference signals.
 *
 * @param out_rt_ptr           GPR resulting route pointer. index to route entry. 0 if no route entry.
 * @param in_ip_da             GPR with IP destination address
 * @param CONST_LOOKUP_BASE    Constant base address of route lookup tables
 * @param TRIE_TYPE            @arg Trie depth 5: 16bit index table followed by 4 4bit index tables
 *                             @arg Trie depth 7: 8bit index table followed by 6 4bit index tables
 *
 * @instruction_cnt For TRIE5, minimum 16 instructions: matching after 1st lookup in long path table
 * @instruction_cnt For TRIE5, minimum 23 instructions: matching after 1st lookup in short path table
 * @instruction_cnt For TRIE5, maximum 83 instructions
 *
 * @see IXP Software Reference Manual(SRM) for algorithm description.
 *
 * @b Example:
 * @code
 * ipv4_route_lookup(route_entry_ptr, ipv4_da, 0x20000, 5)
 * @endcode
 */
#macro ipv4_route_lookup(out_rt_ptr, in_ip_da, CONST_LOOKUP_BASE, TRIE_TYPE)
    #if (TRIE_TYPE == 5)
        _ipv4_trie5_lookup(out_rt_ptr, in_ip_da, CONST_LOOKUP_BASE)
    #else
        #error "ipv4_route_lookup. unsupported trie type."
    #endif
#endm


/// @cond INTERNAL_MACROS
/** An internal macro used by route_lookup.
 *
 * If the trie entry contains a non-zero route ptr, the prevout_rt_ptr should be updated (longer prefix is found).
 *
 * @instruction_cnt 8-9 instructions for IXP2XXX address mode
 *
 * Memory access : 1 SRAM read
 */
#macro _next_trie(ipaddr, const_lsb, prevout_rt_ptr, lookup, xfer_reg, trie_base)
.begin

    .reg    current_rt_ptr
    #ifdef SHIFT_AMT
        #warning "SHIFT_AMT is being redefined."
    #endif
    #define_eval SHIFT_AMT (const_lsb)
    shf_left(lookup, lookup, 4)                            // using convert from index to word offset
    alu_shf_right(lookup, lookup, +4, ipaddr, SHIFT_AMT)   // form offset from ipaddr and trie block addr
    shf_left(lookup, lookup, 2)                             // get a byte offset
    // by now, we have the trie entry to read
    sram_read(xfer_reg, trie_base, lookup, 1, tmp_signal2, tmp_signal2, optimize_mem)
    // get the current route ptr
    bytefield_select(current_rt_ptr, 1100, xfer_reg, DO_LOAD_CC)
    beq[read_next_trie#]
    // else, we need to update route ptr
    move(prevout_rt_ptr, current_rt_ptr)
read_next_trie#:
    bytefield_select(lookup, 0011, xfer_reg, DO_LOAD_CC)               // get trie pointer, set cond code
    #undef SHIFT_AMT

.end
#endm
/// @endcond


/// @cond INTERNAL_MACROS
/** Support macro ipv4_route_lookup.
 *
 * This uses a dual lookup of a 64k entry table and trie block lookups.
 * It uses a maximum of 5 sram reference signals.
 *
 * @param out_rt_ptr          GPR result of the lookup. @n
 *                            @arg Bits 31-16: entry index in route table.
 *                            @arg Bits 15-0: route pointer
 *                            0 if no route entry.
 * @param in_ip_da            GPR with IP destination address
 * @param CONST_LOOKUP_BASE   Constant base address of route lookup tables
 *
 * @instruction_cnt minimum 16 instructions for matching after 1st lookup in long path table
 * @instruction_cnt 23 instructions for matching after 1st lookup in short path table
 * @instruction_cnt maximum 83 instructions
 *
 * @note
 * @b Memory accesses: @n
 * @arg minimum 2 SRAM reads
 * @arg maximum 6 SRAM reads
 *
 * @b Example:
 * @code
 * _ipv4_trie5_lookup(route_entry_ptr, ipv4_da, 0x20000)
 * @endcode
 */
#macro _ipv4_trie5_lookup(out_rt_ptr, in_ip_da, CONST_LOOKUP_BASE)
.begin

    .reg tables_base temp_base2 temp_base3, offset, lookup_short, lookup_long, prev_rt_short, prev_rt_long,\
            current_rt_short current_rt_long
    .reg $rd_xfer0 $rd_xfer1
    .sig tmp_signal2 tmp_signal3

// tables_base can be either a static constant or from an .import_var
    immed32(tables_base, CONST_LOOKUP_BASE)                // if static constant

    // byte address
    alu_shf_right(offset, 0x3, ~AND, in_ip_da, 14)         // form offset from 31:16
    alu_shf_left(temp_base2, tables_base, +, 4, 16)        // add 0x40000, 64K entry table
    alu_shf_left(temp_base3, temp_base2, +, 4, 8)          // add 0x400, multiple 16 entry tables

first_lookup#:
    sram_read($rd_xfer0, tables_base, offset, 1, tmp_signal2, SIG_NONE, optimize_mem) // direct lookup off addr 31:16

    // byte address             // form offset from 31:24
    alu_shf_right(offset, 0x3, ~AND, in_ip_da, 22)         // form offset from 31:24

    sram_read($rd_xfer1, temp_base2, offset, 1, tmp_signal3, signals(tmp_signal2, tmp_signal3), optimize_mem)       // direct lookup off addr 31:24

    // we have read two trie entries. Load the prev_rt_short
    // and prev_rt_long
    bytefield_select(prev_rt_long, 1100, $rd_xfer0, NO_LOAD_CC)
    bytefield_select(prev_rt_short, 1100, $rd_xfer1, NO_LOAD_CC)

    bytefield_select(lookup_short, 0011, $rd_xfer1, DO_LOAD_CC)    // get trie pointer, set cond code
    beq[long_path_only#]                                    // lookup_short = 0, no short path

    bytefield_select(lookup_long, 0011, $rd_xfer0, DO_LOAD_CC)     // get trie pointer, set cond code
    beq[short_path_only#]                                   // lookup_long = 0, short path only

    br[both_paths#]                                         // follow both paths

short_path_only#:

second_lookup_short#:
    _next_trie(in_ip_da, 20, prev_rt_short, lookup_short, $rd_xfer1, temp_base3)
    beq[set_route_ptr#]
third_lookup_short#:
    _next_trie(in_ip_da, 16, prev_rt_short, lookup_short, $rd_xfer1, temp_base3)
    //beq[set_route_ptr#] redundant instruction - K.Tong
    br[set_route_ptr#]

long_path_only#:
    bytefield_select(lookup_long, 0011, $rd_xfer0, DO_LOAD_CC)         // get trie pointer, set cond code
    beq[set_route_ptr#]
second_lookup_long#:
    _next_trie(in_ip_da, 12, prev_rt_long, lookup_long, $rd_xfer0, temp_base3)
    beq[set_route_ptr#]
third_lookup_long#:
    _next_trie(in_ip_da, 8, prev_rt_long, lookup_long, $rd_xfer0, temp_base3)
    beq[set_route_ptr#]
fourth_lookup_long#:
    _next_trie(in_ip_da, 4, prev_rt_long, lookup_long, $rd_xfer0, temp_base3)
    beq[set_route_ptr#]
fifth_lookup_long#:
    _next_trie(in_ip_da, 0, prev_rt_long, lookup_long, $rd_xfer0, temp_base3)
    br[set_route_ptr#]

both_paths#:
    shf_left(lookup_short, lookup_short, 4)
    alu_shf_right(lookup_short, lookup_short, +4, in_ip_da, 20)// form offset from ipaddr and trie block addr
    shf_left(lookup_short, lookup_short, 2)  // get a byte offset
    //bytefield_select(prev_rt_short, 1100, $rd_xfer1)
    sram_read($rd_xfer1, temp_base3, lookup_short, 1, tmp_signal2, SIG_NONE, optimize_mem)      // direct lookup off addr 31:16
    shf_left(lookup_long, lookup_long, 4)
    alu_shf_right(lookup_long, lookup_long, +4, in_ip_da, 12)  // form offset from ipaddr and trie block addr
    shf_left(lookup_long, lookup_long, 2)  // get a byte offset
    bytefield_select(prev_rt_long, 1100, $rd_xfer0, NO_LOAD_CC)
    sram_read($rd_xfer0, temp_base3, lookup_long, 1, tmp_signal3, signals(tmp_signal2, tmp_signal3), optimize_mem)      // direct lookup off addr 31:24

    // by now we have read the trie entries
    // now read the route ptrs
    bytefield_select(current_rt_short, 1100, $rd_xfer1, DO_LOAD_CC)
    beq[both_paths_check_long#]
    // else update short route ptr
    move(prev_rt_short, current_rt_short)

both_paths_check_long#:
    bytefield_select(current_rt_long, 1100, $rd_xfer0, DO_LOAD_CC)
    beq[both_paths_read_next_trie#]
    // else update short route ptr
    move(prev_rt_long, current_rt_long)

both_paths_read_next_trie#:
    bytefield_select(lookup_long, 0011, $rd_xfer0, DO_LOAD_CC)   // get trie pointer, set cond code
    beq[second_both_no_long#]
second_both_long#:
    bytefield_select(lookup_short, 0011, $rd_xfer1, DO_LOAD_CC)  // get trie pointer, set cond code
    beq[third_lookup_long#]
    br[third_lookup_both#]
second_both_no_long#:
    bytefield_select(lookup_short, 0011, $rd_xfer1, DO_LOAD_CC)  // get trie pointer, set cond code
    beq[set_route_ptr#]
    br[third_lookup_short#]

third_lookup_both#:
    shf_left(lookup_short, lookup_short, 4)
    alu_shf_right(lookup_short, lookup_short, +4, in_ip_da, 16)    // form offset from ipaddr and trie block addr
    shf_left(lookup_short, lookup_short, 2)  // get a byte offset
    sram_read($rd_xfer1, temp_base3, lookup_short, 1, tmp_signal2, SIG_NONE, optimize_mem)
    shf_left(lookup_long, lookup_long, 4)
    alu_shf_right(lookup_long, lookup_long, +4, in_ip_da, 8)       // form offset from ipaddr and trie block addr
    shf_left(lookup_long, lookup_long, 2)  // get a byte offset
    sram_read($rd_xfer0, temp_base3, lookup_long, 1, tmp_signal3, signals(tmp_signal2, tmp_signal3), optimize_mem)   // direct lookup off addr 31:24

    // by now we have read the trie entries
    // now read the route ptrs
    bytefield_select(current_rt_short, 1100, $rd_xfer1, DO_LOAD_CC)
    beq[third_lookup_both_check_long#]
    // else update short route ptr
    move(prev_rt_short, current_rt_short)

third_lookup_both_check_long#:
    bytefield_select(current_rt_long, 1100, $rd_xfer0, DO_LOAD_CC)
    beq[third_lookup_both_read_next_trie#]
    // else update short route ptr
    move(prev_rt_long, current_rt_long)

third_lookup_both_read_next_trie#:
    bytefield_select(lookup_long, 0011, $rd_xfer0, DO_LOAD_CC)  // get trie pointer, set cond code
    beq[set_route_ptr#]
    br[fourth_lookup_long#]

set_route_ptr#:

    shf_right(out_rt_ptr, $rd_xfer0, 16)               // long match
    bne[end#]
    shf_right(out_rt_ptr, prev_rt_long, 16)            // long match at previous trie
    bne[end#]
    shf_right(out_rt_ptr, $rd_xfer1, 16)               // short match
    bne[end#]
    shf_right(out_rt_ptr, prev_rt_short, 16)           // short match at previous trie

end#:

.end
#endm
/// @endcond


/** Process ip header that starts at read xfer byte pointer.
 *
 * -# Verify ip total length, time-to-live and checksum. If RFC1812_SHOULD verify addresses.
      (RFC's usually have MUST and SHOULD part. The MUST part of RFC 1812 is verification
      of TTL, total length and checksum. The SHOULD part of RFC1812 calls for address
      verification).
 * -# Modify time-to-live and checksum. Write modified header to sdram write xfers.
 *
 * @note ip frag and ip options are not supported.
 *
 * @param out_exception         0 if no exception, otherwise ip exception code
 * @param out_data              Name of dram write xfer transfer buffer to contain packet data
 * @param IPHDR_WR_START_BYTE   Constant byte address of ip header start relative to out_data
 * @param in_prepend            Longword containing bytes to be merged with first word, prepended
 *                              if byte alignment within first ip longword is not 0. Bytes
 *                              to be merged must be at exact byte locations that it will occupy
 *                              in long word inside output header. All other bytes must be 0.
 * @param in_data               Name of sram or sdram read xfer transfer buffer containing packet data
 * @param IPHDR_RD_START_BYTE   Constant byte address of ip header start relative to in_data
 *
 * @instruction_cnt Minimum 8 instructions - return invalid address exception
 * @instruction_cnt No exception: 48 instr for alignment 0
 * @instruction_cnt No exception: 53 instr for aligment 2
 * @instruction_cnt No exception: 60-63 instr for aligment 1 or 3
 *
 * @see stdmac.uc, endian.uc
 *
 * @b Example:
 * @code
 * // ip header starts at byte 14 of ethernet packet.
 * // Offset 14 means the header starts at byte 2 of long word 3
 * move(in_prepend, 0x12340000)
 * ipv4_proc(exception, modified_pkt, BYTEOFFSET14, in_prepend, pkt, BYTEOFFSET14)
 * @endcode
 */
#macro ipv4_proc(out_exception, out_data, IPHDR_WR_START_BYTE, in_prepend, in_data, IPHDR_RD_START_BYTE)
    ipv4_verify(out_exception, in_data, IPHDR_RD_START_BYTE)           // 18-23 instr
    bgt[end#]
    ctx_arb[voluntary]

    #if (strstr(in_data, $$))
        _ipv4_modify_d(out_data, IPHDR_WR_START_BYTE, in_prepend, in_data, IPHDR_RD_START_BYTE)
    #else
        ipv4_modify(out_data, IPHDR_WR_START_BYTE, in_prepend, in_data, IPHDR_RD_START_BYTE)  // 9-11 instr
    #endif
end#:
#endm


/** Verify total_len is at least 20.
 *
 * @condition_codes Set condition code flags. If greater than 0, total length is bad.
 *
 * @param in_data                Name of read xfer transfer buffer containing packet data
 * @param IPHDR_RD_START_BYTE    Constant byte address of ip header start relative to in_data
 *
 * @instruction_cnt 1
 *
 * @b Example:
 * @code
 * ipv4_total_len_verify($ip_header, 14)
 * bgt[exception_handler_ipv4_total_len#]
 * // continue non-exception code
 * @endcode
 */
#macro ipv4_total_len_verify(in_data, IPHDR_RD_START_BYTE)
.begin

    .reg total_len
    xbuf_extract(total_len, in_data, IPHDR_RD_START_BYTE, IP_TOTAL_LENGTH) // get total length field
    alu_op(--, 0x14, -, total_len)

.end
#endm


/** Verify ttl is greater than 1.
 *
 * @condition_codes Set condition code. If greater than zero, ttl is bad.
 *
 * @param in_data               Name of sram read xfer transfer buffer containing packet data
 * @param IPHDR_RD_START_BYTE   Constant byte address of ip header start relative to in_data
 *
 * Usage: RFC 1812 says check TTL only after fwding decision is made. At that point,
 *        if packet is to host, ignore ttl and send to host.
 *
 * @instruction_cnt 1
 *
 * @b Example:
 * @code
 * ipv4_ttl_verify($ip_header, 14)
 * bgt[exception_handler_ttl#]
 * // continue non-exception code
 * @endcode
 */
#macro ipv4_ttl_verify(in_data, IPHDR_RD_START_BYTE)
.begin

    .reg ttl
// Check that TTL in byte 8 of ip header is greater than 1.
    xbuf_extract(ttl, in_data, IPHDR_RD_START_BYTE, IP_TIME_TO_LIVE) // get ttl field
    alu[--, 2, -, ttl]

.end
#endm


/** Verify total length, time-to live and checksum.
 *
 * If RFC1812_SHOULD, verify source and dest addresses (the SHOULD part of RFC 1812 call
 * for addresses verification).
 *
 * @param out_exception         @arg 0 if no exception
 *                              @arg IPV4_BAD_TOTAL_LENGTH
 *                              @arg IPV4_EXCP_BAD_TTL
 *                              @arg IPV4_FAILURE
 *                              @arg IP_INVALID_ADDRESS
 *
 * @param in_data               Name of sram read xfer transfer buffer containing packet data
 * @param IPHDR_RD_START_BYTE   Constant byte address of ip header start relative to in_data
 *
 * @condition_codes @arg Z = 0 (not equal to 0) if total length, checksum, AND address checking are all successful
 *                  @arg Z = 1 (equal to 0) if total length and checkum are good AND address checking is skip (RFC_1812 is not defined)
 *
 * @instruction_cnt Minimum 8 instructions : return address exception
 * @instruction_cnt No exception: 34 instructions for alignment 0
 * @instruction_cnt No exception: 38 instructions for aligment 2
 * @instruction_cnt No exception: 40-42 instructions for aligment 1 or 3
 *
 * Latency: 8-37 instructions
 *
 * @see stdmac.uc, endian.uc
 *
 * @b Example:
 * @code
 * ipv4_verify(exception, pkt, BYTEOFFSET14)   //  ip header starts at byte 14 of ethernet packet
 * @endcode
 */
#macro ipv4_verify(out_exception, in_data, IPHDR_RD_START_BYTE)
    // check total ip packet length is at least 20 bytes
    ipv4_total_len_verify(in_data, IPHDR_RD_START_BYTE)
    bgt[total_len_exception#]

    // verify checksum of ip header
    ipv4_cksum_verify(in_data, IPHDR_RD_START_BYTE)
    bne[cksum_exception#]

    // check for invalid source or dest addresses
    #ifdef RFC1812_SHOULD
        .begin

            .reg temp_addr
            xbuf_extract(temp_addr, in_data, IPHDR_RD_START_BYTE, IP_SOURCE_ADDRESS)
            ipv4_addr_verify(out_exception, temp_addr, IP_SRC)

            // #define  SKIP_DEST_ADDR_CHECK if you want to skip the  the destination address check
            #ifdef SKIP_DEST_ADDR_CHECK
                br[end#]
            #else
                // if the address is invalid or caused exception, then Z=0
                bne[end#]
                xbuf_extract(temp_addr, in_data, IPHDR_RD_START_BYTE, IP_DESTINATION_ADDRESS)
                ipv4_addr_verify(out_exception, temp_addr, IP_DEST)
                br[end#]
            #endif  // SKIP_DEST_ADDR_CHECK

        .end
    #else
        // if here, it means no address checking AND both total length and
        // checksum are good
        br[end#], defer[1]
        alu[out_exception, --, B, IPV4_SUCCESS]
    #endif

total_len_exception#:
    br[end#], defer[1]
    immed32(out_exception, IPV4_BAD_TOTAL_LENGTH)

cksum_exception#:
    immed32(out_exception, IPV4_BAD_CHECKSUM)

end#:
#endm


/** Perform 5tuple route entry lookup.
 *
 * @param out_index            Index of entry if success. 0 if fail.
 * @param in_data              Buffer with IP header
 * @param IPHDR_START_BYTE     Constant byte address of ip header start relative to in_data
 * @param TRIE_ADDR_BASE       Address of trie table, which is the sram primary base table
 * @param KEY_DATA_SD_BASE     Base address of key database located in SDRAM
 * @param TRIE_TYPE            Index bits used to address the hash trie: HASH_16_4, HASH_FOLD_16
 *                             @arg HASH_16_4: First lookup uses 16 bits of index, subsequent
 *                                             lookups use 4 bitsw of index.
 *                             @arg HASH_FOLD_16: First lookup XORs initial index to reduce it by
 *                                                half, then performs a tabqle lookup using 16 bits of
 *                                                half-index, with subsequent chain search until no collision.
 *
 * @instruction_cnt 10-15 instructions + size of hash_lookup
 *
 * @b Example:
 * @code
 * .reg out_index
 * xbuf_alloc($ip_buffer, SIZE, read)  // SIZE = size of IP packet
 * // copy/read IP packet into ip_buffer
 * ipv4_5tuple_lookup(out_index, $ip_buffer, IPHDR_START_BYTE, TRIE_ADDR_BASE, KEY_DATA_SD_BASE, HASH_16_4)
 * @endcode
 */
#macro ipv4_5tuple_lookup(out_index, in_data, IPHDR_START_BYTE, TRIE_ADDR_BASE, KEY_DATA_SD_BASE, TRIE_TYPE)
.begin

    .reg temp_reg temp_port
    #define _IPV4_5_TUPLE_KEY_SIZE 104

    #ifdef __DONT_TRANSLATE_KEYS
        #error "ipv4_5tuple_lookup does not support __DONT_TRANSLATE_KEYS"
    #endif

    xbuf_alloc($key, 4, read_write)

    // get source address
    xbuf_extract(temp_reg, in_data, IPHDR_START_BYTE, IP_SOURCE_ADDRESS)
    move($key[0], temp_reg)

    // get destination address
    xbuf_extract(temp_reg, in_data, IPHDR_START_BYTE, IP_DESTINATION_ADDRESS)
    move($key[1], temp_reg)

    xbuf_extract(temp_port, in_data, IPHDR_START_BYTE, IP_SOURCE_PORT)         // get source port
    xbuf_extract(temp_reg, in_data, IPHDR_START_BYTE, IP_DESTINATION_PORT) // get destination port
    or_shf_left(temp_reg, temp_reg, temp_port, 16)
    move($key[2], temp_reg)

    // get protocol type
    xbuf_extract(temp_reg, in_data, IPHDR_START_BYTE, IP_PROTOCOL)
    move($key[3], temp_reg)

    hash_lookup(out_index, $key, _IPV4_5_TUPLE_KEY_SIZE, TRIE_ADDR_BASE, TRIE_TYPE, KEY_DATA_SD_BASE)

    xbuf_free($key)

    #undef _IPV4_5_TUPLE_KEY_SIZE

.end
#endm


// Helper macros for ipv4_tos_modify

/// @cond INTERNAL_MACROS
/** Calcualtes new checksum from modified 16 bit field, old 16 bit field and
 * old checksum. This macro uses RFC 1624 equation for calculating checksum. The equation is
 * HC' = HC - ~m - m'.
 *
 * @param out_new_checksum_16  New checksum computed using HC' = HC - ~m - m' @n
 *                             where HC' = new checksum, HC = old checksum , m = old 16 bit field,
 *                             m' = new 16 bit field
 * @param in_new_field_16      New 16bit field value of ipv4 header
 * @param in_old_field_16      Old 16bit field value of ipv4 header
 * @param in_old_checksum_16   Old 16bit checksum value of ipv4 header
 */
#macro _get_new_checksum(out_new_checksum_16, in_new_field_16, in_old_field_16, in_old_checksum_16)
.begin

    .reg temp
    alu[temp, --, ~B, in_old_field_16]
    // new field = ~m + m'
    alu[in_new_field_16, in_new_field_16, +16, temp]
    // HC' = HC - (~m + m)
    alu[out_new_checksum_16, in_old_checksum_16 , -, in_new_field_16]
    // Get 16 MSB in temp & add 16 LSB of new checksum
    alu_shf[temp, --, B, out_new_checksum_16, >>16]
    alu[temp, temp, +16, out_new_checksum_16]
    //insert 16 LSB in new_checksum
    bytefield_clr_insert(out_new_checksum_16, 0011, temp, NO_LOAD_CC)

.end
#endm
/// @endcond


/// @cond INTERNAL_MACROS
/** Modifies TOS field with new tos value and updates IPv4 checksum field according to RFC 1624.
 */
#macro _tos_modify_without_copy(BUFFER, _WRITE_ALIGN, in_tos_value, in_prepend)
.begin

    .reg old_field, new_field, old_checksum, new_checksum
    #if (_WRITE_ALIGN == 0)
        // get first 16 bit field of ipv4 header IPversion & TOS value
        bytefield_extract(old_field, BUFFER/**/0, 0, 1, DO_LOAD_CC]
        bytefield_extract(old_checksum, BUFFER/**/2, 2, 3, DO_LOAD_CC]
        //insert new tos_value
        bytefield_shf_left_insert(BUFFER/**/0, 0100, in_tos_value, 16, NO_LOAD_CC)
        bytefield_extract(new_field, BUFFER/**/0, 0, 1, DO_LOAD_CC]
        //get the new checksum. _get_new_checksum calculates new check sum based RFC 1624
        _get_new_checksum(new_checksum, new_field, old_field, old_checksum)
        //Inser new checksum in ipv4 header
        bytefield_insert(BUFFER/**/2, 0011, new_checksum, NO_LOAD_CC)
    #elif(_WRITE_ALIGN == 1)
        // get first 16 bit field of ipv4 header IPversion & TOS value
        bytefield_extract(old_field, BUFFER/**/0, 1, 2, DO_LOAD_CC)
        bytefield_dbl_extract(old_checksum, BUFFER/**/2, 3, BUFFER/**/3, 0, NO_LOAD_CC)
        bytefield_shf_left_insert(BUFFER/**/0, 0010, in_tos_value, 8, NO_LOAD_CC)
        bytefield_extract(new_field, BUFFER/**/0, 1, 2, DO_LOAD_CC)
        _get_new_checksum(new_checksum, new_field, old_field, old_checksum)
        bytefield_shf_right_insert(BUFFER/**/2, 0001, new_checksum, 8, NO_LOAD_CC)
        bytefield_shf_left_insert(BUFFER/**/3, 1000, new_checksum, 24, NO_LOAD_CC)
        #if(! streq(in_prepend, 0))
            bytefield_insert(BUFFER/**/0, 1000, in_prepend, NO_LOAD_CC)
        #endif
    #elif(_WRITE_ALIGN == 2)
        bytefield_extract(old_field, BUFFER/**/0, 2, 3, NO_LOAD_CC)
        bytefield_extract(old_checksum, BUFFER/**/3, 0, 1, NO_LOAD_CC)
        bytefield_insert(BUFFER/**/0, 0001, in_tos_value, NO_LOAD_CC)
        bytefield_extract(new_field, BUFFER/**/0, 2, 3, NO_LOAD_CC)
        _get_new_checksum(new_checksum, new_field, old_field, old_checksum)
        bytefield_shf_left_insert(BUFFER/**/3, 1100, new_checksum, 16, NO_LOAD_CC)
        #if(! streq(in_prepend, 0))
            //append prepend to two bytes in word 0 of ip header
            bytefield_insert(BUFFER/**/0, 1100, in_prepend, NO_LOAD_CC)
        #endif
    #else
        bytefield_dbl_extract(old_field, BUFFER/**/0, 3, BUFFER/**/1, 0, NO_LOAD_CC)
        bytefield_extract(old_checksum, BUFFER/**/3, 1, 2, NO_LOAD_CC)
        bytefield_shf_left_insert(BUFFER/**/1, 1000, in_tos_value, 24, NO_LOAD_CC)
        bytefield_dbl_extract(new_field, BUFFER/**/0, 3, BUFFER/**/1, 0, NO_LOAD_CC)
        _get_new_checksum(new_checksum, new_field, old_field, old_checksum)
        bytefield_shf_left_insert(BUFFER/**/3, 0110, new_checksum, 8, NO_LOAD_CC)
        //append prepend to three bytes in word 0 of ip header
        #if(! streq(in_prepend, 0))
            bytefield_insert(BUFFER/**/0, 1110, in_prepend, NO_LOAD_CC)
        #endif
    #endif

.end
#endm
/// @endcond


/// @cond INTERNAL_MACROS
/** Ipv4 header copied in local temp buffer, then tos and checksum are modified and finally new ipv4 header into out_buffer.
 */
#macro _tos_modify_with_copy(OUT_BUFFER, _WRITE_ALIGN, IN_BUFFER, _READ_ALIGN, in_tos_value, in_prepend)
.begin

    .reg old_field new_field old_checksum new_checksum dummy_reg
    // allocate temp buffer
    xbuf_alloc(temp_buf, 6, READ_WRITE)
    //copy ipv4 header in local buffer
    move(temp_buf[0], IN_BUFFER/**/0)
    move(temp_buf[1], IN_BUFFER/**/1)
    move(temp_buf[2], IN_BUFFER/**/2)
    move(temp_buf[3], IN_BUFFER/**/3)
    move(temp_buf[4], IN_BUFFER/**/4)
    #if(_READ_ALIGN > 0)
        move(temp_buf[5], IN_BUFFER/**/5)
    #endif

    #if (_READ_ALIGN == 0)
        bytefield_extract(old_field, temp_buf[0], 0, 1, DO_LOAD_CC)
        bytefield_extract(old_checksum, temp_buf[2], 2, 3, DO_LOAD_CC)
        bytefield_shf_left_insert(temp_buf[0], 0100, in_tos_value, 16, NO_LOAD_CC)
        bytefield_extract(new_field, temp_buf[0], 0, 1, DO_LOAD_CC)
        //get the new checksum. _get_new_checksum calculates new check sum based RFC 1624
        _get_new_checksum(new_checksum, new_field, old_field, old_checksum)
        bytefield_insert(temp_buf[2], 0011, new_checksum, NO_LOAD_CC)
        #if(_WRITE_ALIGN == 1)
            /* get first three bytes in first word of ip header  & insert in_prepend */
            shf_right(old_field, temp_buf[0], 8)
            #if(! streq(in_prepend, 0))
                bytefield_insert(old_field, 1000, in_prepend, NO_LOAD_CC)
            #endif
            move(OUT_BUFFER/**/0, old_field)
            dbl_shf[OUT_BUFFER/**/1, temp_buf[0], temp_buf[1], >>8]
            dbl_shf[OUT_BUFFER/**/2, temp_buf[1], temp_buf[2], >>8]
            dbl_shf[OUT_BUFFER/**/3, temp_buf[2], temp_buf[3], >>8]
            dbl_shf[OUT_BUFFER/**/4, temp_buf[3], temp_buf[4], >>8]
            bytefield_clr_shf_left_insert(OUT_BUFFER/**/5, 1000, temp_buf[4], 24, NO_LOAD_CC)
        #elif(_WRITE_ALIGN == 2)
            /* get first two bytes in first word of ip header  & insert in_prepend */
            //  bytefield_shf_right_insert(old_field, 0011, temp_buf[0], 16, NO_LOAD_CC)
            shf_right(old_field, temp_buf[0], 16)
            #if(! streq(in_prepend, 0))
                bytefield_insert(old_field, 1100, in_prepend, NO_LOAD_CC)
            #endif
            move(OUT_BUFFER/**/0, old_field)
            dbl_shf[OUT_BUFFER/**/1, temp_buf[0], temp_buf[1], >>16]
            dbl_shf[OUT_BUFFER/**/2, temp_buf[1], temp_buf[2], >>16]
            dbl_shf[OUT_BUFFER/**/3, temp_buf[2], temp_buf[3], >>16]
            dbl_shf[OUT_BUFFER/**/4, temp_buf[3], temp_buf[4], >>16]
            bytefield_clr_shf_left_insert(OUT_BUFFER/**/5, 1100, temp_buf[4], 16, NO_LOAD_CC)
        #else //_WRITE_ALIGN=3
            /* get first one byte in first word of ip header  & insert in_prepend */
            //bytefield_shf_right_insert(old_field, 0001, temp_buf[0], 24, NO_LOAD_CC)
            shf_right(old_field, temp_buf[0], 24)
            #if(! streq(in_prepend, 0))
                bytefield_insert(old_field, 1110, in_prepend, NO_LOAD_CC)
            #endif
            move(OUT_BUFFER/**/0, old_field)
            dbl_shf[OUT_BUFFER/**/1, temp_buf[0], temp_buf[1], >>24]
            dbl_shf[OUT_BUFFER/**/2, temp_buf[1], temp_buf[2], >>24]
            dbl_shf[OUT_BUFFER/**/3, temp_buf[2], temp_buf[3], >>24]
            dbl_shf[OUT_BUFFER/**/4, temp_buf[3], temp_buf[4], >>24]
            bytefield_clr_shf_left_insert(OUT_BUFFER/**/5, 1110, temp_buf[4], 8, NO_LOAD_CC)
        #endif
    #elif(_READ_ALIGN == 1)
        bytefield_extract(old_field, temp_buf[0], 1, 2, DO_LOAD_CC)
        bytefield_dbl_extract(old_checksum, temp_buf[2], 3, temp_buf[3], 0, NO_LOAD_CC)
        bytefield_shf_left_insert(temp_buf[0], 0010, in_tos_value, 8, NO_LOAD_CC)
        bytefield_extract(new_field, temp_buf[0], 1, 2, DO_LOAD_CC)
        _get_new_checksum(new_checksum, new_field, old_field, old_checksum)
        bytefield_shf_right_insert(temp_buf[2], 0001, new_checksum, 8, NO_LOAD_CC)
        bytefield_shf_left_insert(temp_buf[3], 1000, new_checksum, 24, NO_LOAD_CC)
        //align to WRITE_DATA alignment
        #if(_WRITE_ALIGN == 0)
            dbl_shf[OUT_BUFFER/**/0, temp_buf[0], temp_buf[1], >>24]
            dbl_shf[OUT_BUFFER/**/1, temp_buf[1], temp_buf[2], >>24]
            //this is added to solve issue in assembling
            move(new_field, temp_buf[2])
            dbl_shf[OUT_BUFFER/**/2, new_field, temp_buf[3], >>24]
            dbl_shf[OUT_BUFFER/**/3, temp_buf[3], temp_buf[4], >>24]
            dbl_shf[OUT_BUFFER/**/4, temp_buf[4], temp_buf[5], >>24]
        #elif(_WRITE_ALIGN == 2)
            bytefield_extract(old_field, temp_buf[0], 1 , 2, NO_LOAD_CC)
            #if(! streq(in_prepend, 0))
                bytefield_insert(old_field, 1100, in_prepend, NO_LOAD_CC)
            #endif
            move(OUT_BUFFER/**/0, old_field)
            dbl_shf[OUT_BUFFER/**/1, temp_buf[0], temp_buf[1], >>8]
            dbl_shf[OUT_BUFFER/**/2, temp_buf[1], temp_buf[2], >>8]
            //  dbl_shf[OUT_BUFFER/**/3, temp_buf[2], temp_buf[3], >>8]
            //this is added to solve issue in assembling
            move(new_field, temp_buf[2])
            dbl_shf[OUT_BUFFER/**/3, new_field, temp_buf[3], >>8]
            dbl_shf[OUT_BUFFER/**/4, temp_buf[3], temp_buf[4], >>8]
            bytefield_dbl_extract(old_field, temp_buf[4], 3, temp_buf[5], 0, NO_LOAD_CC)
            bytefield_clr_shf_left_insert(OUT_BUFFER/**/5, 1100, old_field, 16, NO_LOAD_CC)
        #else // _WRITE_ALIGN = 3
            bytefield_extract(old_field, temp_buf[0], 1 , 1, NO_LOAD_CC)
            #if(! streq(in_prepend, 0))
                bytefield_insert(old_field, 1110, in_prepend, NO_LOAD_CC)
            #endif
            move(OUT_BUFFER/**/0, old_field)
            dbl_shf[OUT_BUFFER/**/1, temp_buf[0], temp_buf[1], >>16]
            dbl_shf[OUT_BUFFER/**/2, temp_buf[1], temp_buf[2], >>16]
//          dbl_shf[OUT_BUFFER/**/3, temp_buf[2], temp_buf[3], >>16]
            //this is added to solve issue in assembling
            move(new_field, temp_buf[2])
            dbl_shf[OUT_BUFFER/**/3, new_field, temp_buf[3], >>16]
            dbl_shf[OUT_BUFFER/**/4, temp_buf[3], temp_buf[4], >>16]
            bytefield_dbl_extract(old_field, temp_buf[4], 2, temp_buf[5], 0, NO_LOAD_CC)
            bytefield_clr_shf_left_insert(OUT_BUFFER/**/5, 1110, old_field, 8, NO_LOAD_CC)
        #endif

    #elif(_READ_ALIGN == 2)
        bytefield_extract(old_field, temp_buf[0], 2, 3, NO_LOAD_CC)
        bytefield_extract(old_checksum, temp_buf[3], 0, 1, NO_LOAD_CC)
        bytefield_insert(temp_buf[0], 0001, in_tos_value, NO_LOAD_CC)
        bytefield_extract(new_field, temp_buf[0], 2, 3, NO_LOAD_CC)
        _get_new_checksum(new_checksum, new_field, old_field, old_checksum)
        bytefield_shf_left_insert(temp_buf[3], 1100, new_checksum, 16, NO_LOAD_CC)
        #if(_WRITE_ALIGN == 0)
            dbl_shf[OUT_BUFFER/**/0, temp_buf[0], temp_buf[1], >>16]
            dbl_shf[OUT_BUFFER/**/1, temp_buf[1], temp_buf[2], >>16]
            dbl_shf[OUT_BUFFER/**/2, temp_buf[2], temp_buf[3], >>16]
            dbl_shf[OUT_BUFFER/**/3, temp_buf[3], temp_buf[4], >>16]
            dbl_shf[OUT_BUFFER/**/4, temp_buf[4], temp_buf[5], >>16]
        #elif(_WRITE_ALIGN == 1)
            bytefield_dbl_extract(old_field, temp_buf[0], 2, temp_buf[1], 0, NO_LOAD_CC)
            #if(! streq(in_prepend, 0))
                bytefield_insert(old_field, 1000, in_prepend, NO_LOAD_CC)
            #endif
            move(OUT_BUFFER/**/0, old_field)
            dbl_shf[OUT_BUFFER/**/1, temp_buf[1], temp_buf[2], >>24]
            dbl_shf[OUT_BUFFER/**/2, temp_buf[2], temp_buf[3], >>24]
            dbl_shf[OUT_BUFFER/**/3, temp_buf[3], temp_buf[4], >>24]
            dbl_shf[OUT_BUFFER/**/4, temp_buf[4], temp_buf[5], >>24]
            bytefield_clr_shf_left_insert(OUT_BUFFER/**/5, 1000, temp_buf[5], 8, NO_LOAD_CC)
        #else
            //_WRITE_ALIGN = 3
            bytefield_extract(old_field, temp_buf[0], 2, 2, NO_LOAD_CC)
            #if(! streq(in_prepend, 0))
                bytefield_insert(old_field, 1110, in_prepend, NO_LOAD_CC)
            #endif
            move(OUT_BUFFER/**/0, old_field)
            dbl_shf[OUT_BUFFER/**/1, temp_buf[0], temp_buf[1], >>8]
            dbl_shf[OUT_BUFFER/**/2, temp_buf[1], temp_buf[2], >>8]
            dbl_shf[OUT_BUFFER/**/3, temp_buf[2], temp_buf[3], >>8]
            dbl_shf[OUT_BUFFER/**/4, temp_buf[3], temp_buf[4], >>8]
            bytefield_clr_shf_left_insert(old_field, 1000, temp_buf[4], 24, NO_LOAD_CC)
            bytefield_shf_right_insert(old_field, 0110, temp_buf[5], 8, NO_LOAD_CC)
            move(OUT_BUFFER/**/5, old_field)
        #endif
    #else
        // READ_ALIGN = 3
        bytefield_dbl_extract(old_field, temp_buf[0], 3, temp_buf[1], 0, NO_LOAD_CC)
        bytefield_extract(old_checksum, temp_buf[3], 1, 2, NO_LOAD_CC)
        bytefield_shf_left_insert(temp_buf[1], 1000, in_tos_value, 24, NO_LOAD_CC)
        bytefield_dbl_extract(new_field, temp_buf[0], 3, temp_buf[1], 0, NO_LOAD_CC)
        _get_new_checksum(new_checksum, new_field, old_field, old_checksum)
        bytefield_shf_left_insert(temp_buf[3], 0110, new_checksum, 8, NO_LOAD_CC)
        #if(_WRITE_ALIGN == 0)
            //following added to solve 3 sided loop
            move(new_field, temp_buf0)
            dbl_shf[OUT_BUFFER/**/0, new_field, temp_buf[1], >>8]
            dbl_shf[OUT_BUFFER/**/1, temp_buf[1], temp_buf[2], >>8]
            dbl_shf[OUT_BUFFER/**/2, temp_buf[2], temp_buf[3], >>8]
            dbl_shf[OUT_BUFFER/**/3, temp_buf[3], temp_buf[4], >>8]
            dbl_shf[OUT_BUFFER/**/4, temp_buf[4], temp_buf[5], >>8]
        #elif(_WRITE_ALIGN == 1)
            bytefield_dbl_extract(old_field, temp_buf[0], 3, temp_buf[1], 1, NO_LOAD_CC)
            #if(! streq(in_prepend, 0))
                bytefield_insert(old_field, 1000, in_prepend, NO_LOAD_CC)
            #endif
            move(OUT_BUFFER/**/0, old_field)
            dbl_shf[OUT_BUFFER/**/1, temp_buf[1], temp_buf[2], >>16]
            dbl_shf[OUT_BUFFER/**/2, temp_buf[2], temp_buf[3], >>16]
            dbl_shf[OUT_BUFFER/**/3, temp_buf[3], temp_buf[4], >>16]
            dbl_shf[OUT_BUFFER/**/4, temp_buf[4], temp_buf[5], >>16]
            bytefield_clr_shf_left_insert(OUT_BUFFER/**/5, 1000, temp_buf[5], 16, NO_LOAD_CC)
        #else // _WRITE_ALIGN = 2
            bytefield_dbl_extract(old_field, temp_buf[0], 3, temp_buf[1], 0, NO_LOAD_CC)
            #if(! streq(in_prepend, 0))
                bytefield_insert(old_field, 1100, in_prepend, NO_LOAD_CC)
            #endif
            move(OUT_BUFFER/**/0, old_field)
            dbl_shf[OUT_BUFFER/**/1, temp_buf[1], temp_buf[2], >>24]
            dbl_shf[OUT_BUFFER/**/2, temp_buf[2], temp_buf[3], >>24]
            dbl_shf[OUT_BUFFER/**/3, temp_buf[3], temp_buf[4], >>24]
            dbl_shf[OUT_BUFFER/**/4, temp_buf[4], temp_buf[5], >>24]
            bytefield_clr_shf_left_insert(OUT_BUFFER/**/5, 1100, temp_buf[5], 8, NO_LOAD_CC)
        #endif
    #endif  // _READ_ALIGN == 1
    xbuf_free(temp_buf)

.end
#endm
/// @endcond


/// @cond INTERNAL_MACROS
/** tos_modify_with_copy_rw_align_equal_00.
 *
 * Read_align = write_align = 0.
 */
#macro _tos_modify_with_copy_rw_align_equal_00(_OUT_BUFFER, _IN_BUFFER, in_tos_value, in_prepend)
.begin

    .reg old_field new_field old_checksum new_checksum
    // allocate temp buffer
    xbuf_alloc(temp_buf, 2, READ_WRITE)
    //copy ipv4 header in local buffer
    move(temp_buf[0], _IN_BUFFER/**/0)
    move(temp_buf[1], _IN_BUFFER/**/2)
    bytefield_extract(old_field, temp_buf[0], 0, 1, NO_LOAD_CC)
    bytefield_extract(old_checksum, temp_buf[1], 2, 3, NO_LOAD_CC)
    bytefield_shf_left_insert(temp_buf[0], 0100, in_tos_value, 16, NO_LOAD_CC)
    bytefield_extract(new_field, temp_buf[0], 0, 1, NO_LOAD_CC)
    //get the new checksum. _get_new_checksum calculates new check sum based RFC 1624
    _get_new_checksum(new_checksum, new_field, old_field, old_checksum)
    bytefield_insert(temp_buf[1], 0011, new_checksum, NO_LOAD_CC)
    move(_OUT_BUFFER/**/0, temp_buf[0])
    move(_OUT_BUFFER/**/1, _IN_BUFFER/**/1)
    move(_OUT_BUFFER/**/2, temp_buf[1])
    move(_OUT_BUFFER/**/3, _IN_BUFFER/**/3)
    move(_OUT_BUFFER/**/4, _IN_BUFFER/**/4)
    xbuf_free(temp_buf)

.end
#endm
/// @endcond


/// @cond INTERNAL_MACROS
/** _tos_modify_with_copy_rw_align_equal_11.
 *
 * Read_align = write_align = 1.
 */
#macro _tos_modify_with_copy_rw_align_equal_11(_OUT_BUFFER, _IN_BUFFER, in_tos_value, in_prepend)
.begin

    .reg old_field new_field old_checksum new_checksum
    // allocate temp buffer
    xbuf_alloc(temp_buf, 3, READ_WRITE)
    //copy ipv4 header in local buffer
    move(temp_buf[0], _IN_BUFFER/**/0)
    move(temp_buf[1], _IN_BUFFER/**/2)
    move(temp_buf[2], _IN_BUFFER/**/3)
    bytefield_extract(old_field, temp_buf[0], 1, 2, NO_LOAD_CC)
    bytefield_dbl_extract(old_checksum, temp_buf[1], 3, temp_buf[2], 0, NO_LOAD_CC)
    bytefield_shf_left_insert(temp_buf[0], 0010, in_tos_value, 8, NO_LOAD_CC)
    bytefield_extract(new_field, temp_buf[0], 1, 2, NO_LOAD_CC)
    //get the new checksum. _get_new_checksum calculates new check sum based RFC 1624
    _get_new_checksum(new_checksum, new_field, old_field, old_checksum)
    bytefield_shf_right_insert(temp_buf[1], 0001, new_checksum, 8, NO_LOAD_CC)
    bytefield_shf_left_insert(temp_buf[2], 1000, new_checksum, 24, NO_LOAD_CC)
    #if(! streq(in_prepend, 0))
        bytefield_insert(temp_buf[0], 1000, in_prepend, NO_LOAD_CC)
    #endif
    move(_OUT_BUFFER/**/0, temp_buf[0])
    move(_OUT_BUFFER/**/1, _IN_BUFFER/**/1)
    move(_OUT_BUFFER/**/2, temp_buf[1])
    move(_OUT_BUFFER/**/3, temp_buf[2])
    move(_OUT_BUFFER/**/4, _IN_BUFFER/**/4)
    move(_OUT_BUFFER/**/5, _IN_BUFFER/**/5)
    xbuf_free(temp_buf)

.end
#endm
/// @endcond


/// @cond INTERNAL_MACROS
/** _tos_modify_with_copy_rw_align_equal_22.
 *
 * Read_align = write_align = 2.
 */
#macro _tos_modify_with_copy_rw_align_equal_22(_OUT_BUFFER, _IN_BUFFER, in_tos_value, in_prepend)
.begin

    .reg old_field new_field old_checksum new_checksum
    // allocate temp buffer
    xbuf_alloc(temp_buf, 2, READ_WRITE)
    //copy ipv4 header in local buffer
    move(temp_buf[0], _IN_BUFFER/**/0)
    move(temp_buf[1], _IN_BUFFER/**/3)
    bytefield_extract(old_field, temp_buf[0], 2, 3, NO_LOAD_CC)
    bytefield_extract(old_checksum, temp_buf[1], 0, 1, NO_LOAD_CC)
    bytefield_insert(temp_buf[0], 0001, in_tos_value, NO_LOAD_CC)
    bytefield_extract(new_field, temp_buf[0], 2, 3, NO_LOAD_CC)
    //get the new checksum. _get_new_checksum calculates new check sum based RFC 1624
    _get_new_checksum(new_checksum, new_field, old_field, old_checksum)
    bytefield_shf_left_insert(temp_buf[1], 1100, new_checksum, 16, NO_LOAD_CC)
    #if(! streq(in_prepend, 0))
        bytefield_insert(temp_buf[0], 1100, in_prepend, NO_LOAD_CC)
    #endif
    move(_OUT_BUFFER/**/0, temp_buf[0])
    move(_OUT_BUFFER/**/1, _IN_BUFFER/**/1)
    move(_OUT_BUFFER/**/2, _IN_BUFFER/**/2)
    move(_OUT_BUFFER/**/3, temp_buf[1])
    move(_OUT_BUFFER/**/4, _IN_BUFFER/**/4)
    move(_OUT_BUFFER/**/5, _IN_BUFFER/**/5)
    xbuf_free(temp_buf)

.end
#endm
/// @endcond


/// @cond INTERNAL_MACROS
/** _tos_modify_with_copy_rw_align_equal_33.
 *
 * Read_align = write_align = 3.
 */
#macro _tos_modify_with_copy_rw_align_equal_33(_OUT_BUFFER, _IN_BUFFER, in_tos_value, in_prepend)
.begin

    .reg old_field new_field old_checksum new_checksum
    // allocate temp buffer
    xbuf_alloc(temp_buf, 3, READ_WRITE)
    //copy ipv4 header in local buffer
    move(temp_buf[0], _IN_BUFFER/**/0)
    move(temp_buf[1], _IN_BUFFER/**/1)
    move(temp_buf[2], _IN_BUFFER/**/3)
    bytefield_dbl_extract(old_field, temp_buf[0], 3, temp_buf[1], 0, NO_LOAD_CC)
    bytefield_extract(old_checksum, temp_buf[2], 1, 2, NO_LOAD_CC)
    bytefield_shf_left_insert(temp_buf[1], 1000, in_tos_value, 24, NO_LOAD_CC)
    bytefield_dbl_extract(new_field, temp_buf[0], 3, temp_buf[1], 0, NO_LOAD_CC)
    //get the new checksum. _get_new_checksum calculates new check sum based RFC 1624
    _get_new_checksum(new_checksum, new_field, old_field, old_checksum)
    bytefield_shf_left_insert(temp_buf[2], 0110, new_checksum, 8, NO_LOAD_CC)
    #if(! streq(in_prepend, 0))
        bytefield_insert(temp_buf[0], 1110, in_prepend, NO_LOAD_CC)
    #endif
    move(_OUT_BUFFER/**/0, temp_buf[0])
    move(_OUT_BUFFER/**/1, temp_buf[1])
    move(_OUT_BUFFER/**/2, _IN_BUFFER/**/2)
    move(_OUT_BUFFER/**/3, temp_buf[2])
    move(_OUT_BUFFER/**/4, _IN_BUFFER/**/4)
    move(_OUT_BUFFER/**/5, _IN_BUFFER/**/5)
    xbuf_free(temp_buf)

.end
#endm
/// @endcond


/** Modify TOS field with new value and calculate new checksum. Merge "in_prepend" data to first
 * word if ip header offset is not longword aligned.
 *
 * @param out_data               Name of output buffer containing packet data
 * @param IPHDR_WR_START_BYTE    Constant byte address of ip header start relative to out_data
 * @param in_tos_value           tos value which needs to updated in ipv4 packet header
 * @param in_data                Name of buffer containing packet data.
 *                               If header is in SRAM or DRAM transfer register,
 *                               they must be read xfer registers.
 * @param IPHDR_RD_START_BYTE    Constant byte address of ip header start relative to in_data
 * @param in_prepend             Longword containing bytes to be merged with first word, prepended
 *                               if byte alignment within first ip longword is not 0. Bytes
 *                               to be merged must be at exact byte locations that it will occupy
 *                               in output header.
 *
 * @instruction_cnt 11 - 24
 *
 * @b Example:
 * @code
 * .begin
 * .reg    tos_value   in_prepend
 * xbuf_alloc ($in_header, 7, READ)
 * xbuf_alloc ($out_header, 7, WRITE)
 *
 * //initialize ipv4 header offset 1
 * immed32($out_header[0], 0x00450000)
 * immed32($out_header[1], 0x6eabcd00)
 * immed32($out_header[2], 0x00101152)
 * immed32($out_header[3], 0x700a0313)
 * immed32($out_header[4], 0x80188776)
 * immed32($out_header[5], 0x38100000)
 *
 * immed[base, 0x1000]
 * // Write header in sram
 * sram_write($out_header[0], base, 0, 6, sig1, sig1, ___)
 * //Read ipv4 header
 * sram_read($in_header[0], base, 0, 6, sig1, sig1, ___)
 * move(tos_value, 0x12)
 * // modify tos in in_header & write new packet in out_header
 * ipv4_tos_modify($out_header, 2, tos_value, $in_header, 1, 0)
 * xbuf_free($in_header)
 * xbuf_free($out_header)
 * .end
 * @endcode
 *
 */
#macro ipv4_tos_modify(out_data, IPHDR_WR_START_BYTE, in_tos_value, in_data, IPHDR_RD_START_BYTE, in_prepend)
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

    _GET_XBUF_TYPE(in_data)
    #if ((_XBUF_TYPE == _XBUF_GPR) || (_XBUF_TYPE == _XBUF_LMEM))
        #define _IN_DATA_IS_GPR_OR_LMEM_TYPE
    #endif
    #undef _XBUF_TYPE

    _GET_XBUF_TYPE(out_data)
    #if ((_XBUF_TYPE == _XBUF_GPR) || (_XBUF_TYPE == _XBUF_LMEM))
        #define _OUT_DATA_IS_GPR_OR_LMEM_TYPE
    #endif
    #undef _XBUF_TYPE

    #if ( (streq(out_data, in_data)) && (IPHDR_RD_START_BYTE == IPHDR_WR_START_BYTE) )
    // For GPR and local memory xbuf type we do not perform copy when src & dest
    // are same name and start byte offsets are the same.And modify tos value & new checksum directly
    // on header.
        #ifdef _IN_DATA_IS_GPR_OR_LMEM_TYPE
            #ifdef _OUT_DATA_IS_GPR_OR_LMEM_TYPE
                #define _IPV4_NO_NEED_TO_COPY
            #endif
        #else
            _tos_modify_with_copy(_BUF1_REG, WRITE_ALIGN, _BUF0_REG, READ_ALIGN, in_tos_value, in_prepend)
        #endif
    #endif

    #ifdef _IPV4_NO_NEED_TO_COPY
        _tos_modify_without_copy(_BUF0_REG, READ_ALIGN, in_tos_value, in_prepend)
    #elif(READ_ALIGN == WRITE_ALIGN)
        #ifdef _OUT_DATA_IS_GPR_OR_LMEM_TYPE
            move(_BUF1_REG0, _BUF0_REG0)
            move(_BUF1_REG1, _BUF0_REG1)
            move(_BUF1_REG2, _BUF0_REG2)
            move(_BUF1_REG3, _BUF0_REG3)
            move(_BUF1_REG4, _BUF0_REG4)
            #if(READ_ALIGN > 0)
                move(_BUF1_REG5, _BUF0_REG5)
            #endif
            _tos_modify_without_copy(_BUF1_REG, WRITE_ALIGN, in_tos_value, in_prepend)
        #else
            _tos_modify_with_copy_rw_align_equal_/**/READ_ALIGN/**/WRITE_ALIGN/**/(_BUF1_REG, _BUF0_REG, in_tos_value, in_prepend)
        #endif
    #else
        _tos_modify_with_copy(_BUF1_REG, WRITE_ALIGN, _BUF0_REG, READ_ALIGN, in_tos_value, in_prepend)
    #endif
    #ifdef _IN_DATA_IS_GPR_OR_LMEM_TYPE
        #undef _IN_DATA_IS_GPR_OR_LMEM_TYPE
    #endif

    #ifdef _OUT_DATA_IS_GPR_OR_LMEM_TYPE
        #undef _OUT_DATA_IS_GPR_OR_LMEM_TYPE
    #endif

    #ifdef  _IPV4_NO_NEED_TO_COPY
        #undef _IPV4_NO_NEED_TO_COPY
    #endif
    #undef WRITE_ALIGN
    #undef READ_ALIGN
#endm


/** Extract n tuple values from ipv4 header.
 *
 * @param out_n_tuple           Name of buffer to contain n tuple data
 * @param in_data               Name of buffer containing ipv4 packet data.
 *                              If header is in SRAM or DRAM,
 *                              they must be read in xfer registers.
 * @param IPHDR_RD_START_BYTE   Constant byte address of ip header start relative to in_data
 * @param TUPLE_TYPE            FIVE_TUPLE or SIX_TUPLE. In case of FIVE_TUPLE it extracts src_ip, dest_ip, src_port, dest_port
 *                              and protocol. In case of SIX_TUPLE it extracts all FIVE_TUPLE values and
 *                              dscp values.
 *
 * @instruction_cnt 5 to 8
 *
 * @b Example:
 * @code
 * .sig sig1
 * .reg base
 * xbuf_alloc ($in_header, 7, READ)
 * xbuf_alloc ($out_header, 7, WRITE)
 * //initialize ip header in sram
 * immed32($out_header[0], 0x4510006e)
 * immed32($out_header[1], 0xabcd0000)
 * immed32($out_header[2], 0x10115270)
 * immed32($out_header[3], 0x0a031380)
 * immed32($out_header[4], 0x18877638)
 * immed32($out_header[5], 0x10000023)
 * immed[base, 0x1000]
 * sram_write($out_header[0], base, 0, 6, sig1, sig1, ___)
 * sram_read($in_header[0], base, 0, 6, sig1, sig1, ___)
 * ipv4_n_tuple_extract($out_header, $in_header, 0, FIVE_TUPLE)
 * xbuf_free($in_header)
 * xbuf_free($out_header)
 * @endcode
 */
#macro ipv4_n_tuple_extract(out_n_tuple, in_data, IPHDR_START_BYTE, TUPLE_TYPE)
.begin

    .reg temp_reg1 temp_reg2
    _GET_XBUF_TYPE(in_data)
    #if (_XBUF_TYPE == _XBUF_GPR)
        #define _IN_DATA_TYPE  IPV4_GP_REG
    #elif (_XBUF_TYPE == _XBUF_LMEM)
        #define _IN_DATA_TYPE   IPV4_LOCAL_MEM
    #else
        #define _IN_DATA_TYPE   IPV4_NOT_GPR_OR_LMEM
    #endif

    #undef _XBUF_TYPE

    _GET_XBUF_TYPE(out_n_tuple)
    #if (_XBUF_TYPE == _XBUF_GPR)
        #define _OUT_N_TUPLE_TYPE  IPV4_GP_REG
    #elif (_XBUF_TYPE == _XBUF_LMEM)
        #define _OUT_N_TUPLE_TYPE   IPV4_LOCAL_MEM
    #else
        #define  _OUT_N_TUPLE_TYPE   IPV4_NOT_GPR_OR_LMEM
    #endif
    #undef _XBUF_TYPE


    #if (IPHDR_START_BYTE == 0)
        // get source address
        xbuf_extract(out_n_tuple[0], in_data, IPHDR_START_BYTE, IP_SOURCE_ADDRESS)
        // get destination address
        xbuf_extract(out_n_tuple[1], in_data, IPHDR_START_BYTE, IP_DESTINATION_ADDRESS)
        //get src & dest port
        xbuf_extract(out_n_tuple[2], in_data, IPHDR_START_BYTE, IP_SRC_DEST_PORT)
        #if(TUPLE_TYPE == FIVE_TUPLE)
            // get protocol type
            #if(( _IN_DATA_TYPE == IPV4_GP_REG ) || (( _IN_DATA_TYPE == IPV4_LOCAL_MEM) && (_OUT_N_TUPLE_TYPE == IPV4_NOT_GPR_OR_LMEM)) \
            || (( _OUT_N_TUPLE_TYPE == IPV4_LOCAL_MEM) && (_IN_DATA_TYPE == IPV4_NOT_GPR_OR_LMEM)) ||( _OUT_N_TUPLE_TYPE == IPV4_GP_REG ))
                xbuf_extract(out_n_tuple[3], in_data, IPHDR_START_BYTE, IP_PROTOCOL)
            #else
                xbuf_extract(temp_reg1, in_data, IPHDR_START_BYTE, IP_PROTOCOL)
                move(out_n_tuple[3], temp_reg1)
            #endif
        #elif(TUPLE_TYPE == SIX_TUPLE)
            xbuf_extract(temp_reg1, in_data, IPHDR_START_BYTE, IP_PROTOCOL)    //get protocol
            xbuf_extract(temp_reg2, in_data, IPHDR_START_BYTE, IP_TYPE_OF_SERVICE) //get TOS
            alu_shf_right(temp_reg2, temp_reg2, AND, 0xFC, 0) //get dscp
            or_shf_left(temp_reg1, temp_reg1, temp_reg2, 8)
            move(out_n_tuple[3], temp_reg1)
        #endif
    #else
        // get source address
        xbuf_extract( temp_reg1, in_data, IPHDR_START_BYTE, IP_SOURCE_ADDRESS)
        move(out_n_tuple[0], temp_reg1)
        // get destination address
        xbuf_extract(temp_reg1, in_data, IPHDR_START_BYTE, IP_DESTINATION_ADDRESS)
        move(out_n_tuple[1], temp_reg1)
        //get src & dest port
        xbuf_extract(temp_reg1, in_data, IPHDR_START_BYTE, IP_SRC_DEST_PORT)
        move(out_n_tuple[2], temp_reg1)
        #if(TUPLE_TYPE == FIVE_TUPLE)
            // get protocol type
            #if(( _IN_DATA_TYPE == IPV4_GP_REG ) || (( _IN_DATA_TYPE == IPV4_LOCAL_MEM) && (_OUT_N_TUPLE_TYPE == IPV4_NOT_GPR_OR_LMEM)) \
                || (( _OUT_N_TUPLE_TYPE == IPV4_LOCAL_MEM) && (_IN_DATA_TYPE == IPV4_NOT_GPR_OR_LMEM)) ||( _OUT_N_TUPLE_TYPE == IPV4_GP_REG))
                xbuf_extract(out_n_tuple[3], in_data, IPHDR_START_BYTE, IP_PROTOCOL)
            #else
                xbuf_extract(temp_reg1, in_data, IPHDR_START_BYTE, IP_PROTOCOL)
                move(out_n_tuple[3], temp_reg1)
            #endif
        #elif(TUPLE_TYPE == SIX_TUPLE)
            xbuf_extract(temp_reg1, in_data, IPHDR_START_BYTE, IP_PROTOCOL)    //get protocol
            xbuf_extract(temp_reg2, in_data, IPHDR_START_BYTE, IP_TYPE_OF_SERVICE) //get TOS
            alu_shf_right(temp_reg2, temp_reg2, AND, 0xFC, 0) //get dscp
            or_shf_left(temp_reg1, temp_reg1, temp_reg2, 8)
            move(out_n_tuple[3], temp_reg1)
        #endif
    #endif
    #undef _IN_DATA_TYPE
    #undef _OUT_N_TUPLE_TYPE

.end
#endm

/** Perform n tuple route entry lookup.
 *
 * @param out_index           Index of entry if success, 0 if fail
 * @param in_data             Buffer with IP header
 * @param IPHDR_START_BYTE    Constant byte address of ip header start relative to in_data
 * @param TRIE_ADDR_BASE      Address of trie table, which is sram primary base table
 * @param KEY_DATA_SD_BASE    Base address of key database located in SDRAM
 * @param TRIE_TYPE           Index bits used to address hash trie: HASH_16_4, HASH_FOLD_16 @n
 *                            @arg HASH_16_4: First lookup uses 16 bits of index, subsequent
 *                                            lookups use 4 bits of index.
 *                            @arg HASH_FOLD_16: First lookup XORs initial index to reduce it by
 *                                               half, then performs a tabqle lookup using 16 bits of
 *                                               half-index, with subsequent chain search until no collision.
 *
 * @instruction_cnt 5 to 8 instructions + Hash lookup
 *
 * @b Example:
 * @code
 * .reg out_index base
 * xbuf_alloc($ip_buffer, SIZE, read) // SIZE = size of IP packet
 * xbuf_alloc($out_header, 6, WRITE)
 * //initialize ip header in sram
 * immed32($out_header[0], 0x4510006e)
 * immed32($out_header[1], 0xabcd0000)
 * immed32($out_header[2], 0x10115270)
 * immed32($out_header[3], 0x0a031380)
 * immed32($out_header[4], 0x18877638)
 * immed32($out_header[5], 0x10000023)
 * immed[base, 0x1000]
 * sram_write($out_header[0], base, 0, 6, sig1, sig1, ___)
 * //Read tcp/ip header
 * sram_read($ip_buffer[0], base, 0, 6, sig1, sig1, ___)
 * //look up for key
 * ipv4_n_tuple_lookup(out_index, $ip_buffer, IPHDR_START_BYTE, TRIE_ADDR_BASE, KEY_DATA_SD_BASE, HASH_16_4, FIVE_TUPLE)
 * @endcode
 *
 */
#macro ipv4_n_tuple_lookup(out_index, in_data, IPHDR_START_BYTE, trie_base_addr, KEY_DATA_SD_BASE, TRIE_TYPE, TUPLE_TYPE)
    #ifdef __DONT_TRANSLATE_KEYS
        #error "ipv4_5tuple_lookup does not support __DONT_TRANSLATE_KEYS"
    #endif

    xbuf_alloc($key, 4, read_write)
    #if(TUPLE_TYPE == FIVE_TUPLE)
        ipv4_n_tuple_extract($key, in_data, IPHDR_START_BYTE, FIVE_TUPLE)
    #else
        ipv4_n_tuple_extract($key, in_data, IPHDR_START_BYTE, SIX_TUPLE)
    #endif

    hash_lookup(out_index, $key, 128, trie_base_addr, TRIE_TYPE, KEY_DATA_SD_BASE)

    xbuf_free($key)
#endm


/** Check supplied IP address to see if it is a directed broadcast address for a locally
 * attached subnet.
 *
 * @param out_status              0 if supplied IP address is not a directed broadcast address, otherwise
 *                                IPV4_DBCAST_ADDRESS(1)
 * @param in_addr                 GPR containing IP address to be checked.
 * @param LM_DBCAST_BASE          Constant Base address of dbcast local memory cache (longword address)
 * @param LM_DBCAST_BLOCK_COUNT   Constant specifying number of blocks.
 *                                Value of LM_DBCAST_BLOCK_COUNT must be in range of 2 to 256 and should be
 *                                power of 2.
 * @param LM_DBCAST_BLOCK_SIZE    Constant block size in long words (4 bytes).
 *                                Value of LM_DBCAST_BLOCK_SIZE must be
 *                                power of 2 and within the range [1..512].
 * @param LM_INDEX                Constant, indicating LM index either 0 or 1.
 *
 * @condition_codes Condition codes for out_status are set.
 *
 * @instruction_cnt 16-21 instructions per lookup (the total number of instructions depends
 *                  on how many times the lookup is performed).
 *
 * Memory access: None
 *
 * @see RFC 2644
 *
 * @b Example:
 * @code
 * // check src_ipv4_address for directed broadcast address
 * ipv4_is_dbcast_lm(out_status, in_addr, LM_DBCAST_BASE, LM_DBCAST_CACHE_SIZE, LM_DBCAST_BLOCK_SIZE, LM_INDEX)
 *     beq[drop_packet#]
 * @endcode
 *
 * @note Inputs:
 *       -# Internally LM_DBCAST_INDEX_TABLE_SIZE will be calculated as LM_DBCAST_BLOCK_COUNT / 4 long words.
 *       -# Therefore the total local memory long word entries required, starting at LM_DBCAST_BASE, will be, @n
 *          (LM_DBCAST_INDEX_TABLE_SIZE  +  (LM_DBCAST_BLOCK_COUNT * LM_DBCAST_BLOCK_SIZE)) @n
 *          and the total number of local memory entries used must not exceed 640.
 *       -# LM_DBCAST_BLOCK_COUNT must not exceed 256. This is because each index table entry is byte long and can hold max value of 0xFF.
 *
 * Algorithm: @n
 * -# Calculate the number of DBCAST blocks.
 * -# Divide number of blocks into two sets:
 *           a. Blocks to store entries
 *           b. Blocks to store collision entries
 * -# Calculate the size and create two tables:
            a. Entry table
                 Table to hold DBCAST entries
            b. Index table
                  Table to store next_block index for the current block
                  Each byte in this block will corrospond to the block number and will store "Next Index" for that block.
 * -# Compute hash on the in_addr. An IP address of form A.B.C.D produces
 *    a hash_result of form A^(A.B)^(A.B.C)^(A.B.C.D)
 * -# Based on the number of blocks??????, extract the last "N" bits of hash result to get "Block Number".
 * -# Set *l$index= LM_DBCAST_ENTRY_TABLE_BASE + (N * LM_DBCAST_BLOCK_SIZE ).
 *    That is "start search from this block onwards".
 * -# Perform read of long words at local memory location indicated by *l$index.
 * -# Compare each local memory entry with in_addr, under the following conditions: @n
 *    -# If the entry matches with in_addr, return success.
 *    -# If an entry contains zero, return failure.
 *    -# If the end of block is found, then go to index table and retrieve "next block" pointer and start search from step 6.
 *
 * @b Example:
 * @code
 * LM_DBCAST_BLOCK_SIZE                4
 * LM_DBCAST_BLOCK_COUNT               8
 * LM_DBCAST_ENTRY_BLOCK_COUNT         4
 * LM_DBCAST_COLLISSION_BLOCK_COUNT    4
 * LM_DBCAST_INDEX_TABLE_SIZE          2 (Long words)
 * LM_DBCAST_ENTRY_TABLE_BASE          ((LM_DBCAST_INDEX_TABLE_SIZE + LM_DBCAST_BASE) * 4)
 * @endcode
 *
 * |---------------| LM_DBCAST_BASE
 * |I3 | I2| I1| I0|
 * |---------------|    <======= Index table
 * |I7 | I6| I5| I4|
 * |---------------| LM_DBCAST_ENTRY_TABLE_BASE = ((LM_DBCAST_INDEX_TABLE_SIZE + LM_DBCAST_BASE) * 4)
 * |Block0/addr0   |
 * |---------------|
 * |Block0/addr1   |
 * |---------------|
 * |Block0/addr2   |
 * |---------------|
 * |Block0/addr3   |
 * |---------------|
 * |Block1/addr0   |
 *        :           <======= Entry Table
 *        :
 *        :
 * |Block3/addr0   |
 * |---------------|
 * |Block3/addr1   |
 * |---------------|
 * |Block3/addr2   |
 * |---------------|
 * |Block3/addr3   |
 * |---------------|----------------------------------------
 * |ColliBlk0/addr0|
 * |---------------|
 * |ColliBlk0/addr1|
 * |---------------|
 * |ColliBlk0/addr2|
 * |---------------|
 * |ColliBlk0/addr3|
 * |---------------|
 *        :
 *        :           <======= Collision Table
 *        :
 * |ColliBlk3/addr0|
 * |---------------|
 * |ColliBlk3/addr2|
 * |---------------|
 * |ColliBlk3/addr2|
 * |---------------|
 * |ColliBlk3/addr3|
 * |---------------|
 * ....
 *
 */
#macro ipv4_is_dbcast_lm(out_status, in_addr, LM_DBCAST_BASE, LM_DBCAST_BLOCK_COUNT, LM_DBCAST_BLOCK_SIZE, LM_INDEX)
.begin

    .reg hash_result, dbcast_cache_ptr, table_offset, dbcast_tbl_entry


    .reg index_tbl_offset, byte_offset, byte_mask, block_num

    #if (!is_ct_const (LM_DBCAST_BASE))
        #error "For macro ipv4_is_dbcast_lm[], parameter LM_DBCAST_BASE must be constant"
    #endif

    #if (!is_ct_const (LM_DBCAST_BLOCK_COUNT))
        #error "For macro ipv4_is_dbcast_lm[], parameter LM_DBCAST_BLOCK_COUNT must be constant"
    #endif

    #if (!is_ct_const (LM_DBCAST_BLOCK_SIZE))
        #error "For macro ipv4_is_dbcast_lm[], parameter LM_DBCAST_BLOCK_SIZE must be constant"
    #endif

    #if (!is_ct_const (LM_INDEX))
        #error "For macro ipv4_is_dbcast_lm[], parameter LM_INDEX must be constant"
    #endif

    #if (LM_DBCAST_BLOCK_COUNT < 2)
        #error "For macro ipv4_is_dbcast_lm[], LM_DBCAST_BLOCK_COUNT should be > 2"
    #endif

    #if (LM_DBCAST_BLOCK_COUNT > 256)
        #error "For macro ipv4_is_dbcast_lm[], Maximum LM_DBCAST_BLOCK_COUNT is 256"
    #endif

    #if ((LM_INDEX > 1) || (LM_INDEX < 0))
        #error "For macro ipv4_is_dbcast_lm[], LM_INDEX should be either 0 or 1"
    #endif


    #ifndef _LM_DBCAST_ENTRY_BLOCK_COUNT
        #define_eval    _LM_DBCAST_ENTRY_BLOCK_COUNT        (LM_DBCAST_BLOCK_COUNT / 2)
    #endif

    #ifndef _LM_DBCAST_COLLISSION_BLOCK_COUNT
        #define_eval    _LM_DBCAST_COLLISSION_BLOCK_COUNT   (LM_DBCAST_BLOCK_COUNT / 2)
    #endif

    #ifndef _LM_DBCAST_INDEX_TABLE_SIZE
        #define_eval    _LM_DBCAST_INDEX_TABLE_SIZE         (LM_DBCAST_BLOCK_COUNT / 4)
        // Index table size is minimum 1 Longword
        #if (_LM_DBCAST_INDEX_TABLE_SIZE < 1)
            #define_eval    _LM_DBCAST_INDEX_TABLE_SIZE         1
        #endif
    #endif

    #ifndef _LM_DBCAST_ENTRY_TABLE_BASE
        #define_eval    _LM_DBCAST_ENTRY_TABLE_BASE         (_LM_DBCAST_INDEX_TABLE_SIZE + LM_DBCAST_BASE)
        #define_eval    _LM_DBCAST_ENTRY_TABLE_BASE         (_LM_DBCAST_ENTRY_TABLE_BASE * 4)
    #endif

    #if (((LM_DBCAST_BLOCK_COUNT * LM_DBCAST_BLOCK_SIZE ) + _LM_DBCAST_INDEX_TABLE_SIZE ) > 640)
        #error "For macro ipv4_is_dbcast_lm[], Local memory used for  DBCAST cache exceed 640"
    #endif

    #if (((LM_DBCAST_BLOCK_COUNT * LM_DBCAST_BLOCK_SIZE ) + _LM_DBCAST_INDEX_TABLE_SIZE + LM_DBCAST_BASE ) > 640)
        #error "For macro ipv4_is_dbcast_lm[], Local memory used exceed 640 longwords"
    #endif

    //Based upon the LM_DBCAST_BLOCK_SIZE Calculate SHIFT_BIT
    #ifndef _SHIFT_BIT
    #define_eval _SHIFT_BIT 0
    #define_eval _TEMP_VAR  0
        #while((_SHIFT_BIT < 9) && ((LM_DBCAST_BLOCK_SIZE - 1) > _TEMP_VAR))
            #define_eval _TEMP_VAR  (_TEMP_VAR + (1 << _SHIFT_BIT))
            #define_eval _SHIFT_BIT (_SHIFT_BIT + 1)
        #endloop
    #endif

    // Add 2 to the _SHIFT_BIT to get word aligned byte address of the block.
    // That is we need to multiply (block num * block size * 4)
    //              byte_address = (blcok_num <<_SHIFT_BIT)*4)
    //              byte_address = (blcok_num <<_SHIFT_BIT+2 )
    //
    #define_eval _SHIFT_BIT (_SHIFT_BIT + 2)

    // Create BIT_MASK to extract "N" bit out of hash key.
    #ifndef _BIT_MASK
        #define_eval _BIT_MASK  (_LM_DBCAST_ENTRY_BLOCK_COUNT - 1)
    #endif

    //  Given an IP address of form A.B.C.D produce
    //  a hash_result of form A^(A.B)^(A.B.C)^(A.B.C.D)
    move(hash_result, in_addr)//
    alu_shf_right(hash_result, hash_result, XOR, in_addr, 8)//
    alu_shf_right(hash_result, hash_result, XOR, in_addr, 16)//
    alu_shf_right(hash_result, hash_result, XOR, in_addr, 24)//


    // We use the hash result and & with bit_mask and use it as
    // an index into the directed broadcast local memory cache.
    // Extract block number ( N bits)
    alu_op(block_num, hash_result, AND, _BIT_MASK)

check_entry#:

    // Multiply block_num by LM_DBCAST_BLOCK_SIZE to get block index
    alu_shf[table_offset, --, b, block_num, <<_SHIFT_BIT]
    localmem_set_address(_LM_DBCAST_ENTRY_TABLE_BASE, table_offset, LM_INDEX)
    // dbcast_cache_ptr is used to travese inside the block (0 till LM_DBCAST_BLOCK_SIZE-1)

    immed[dbcast_cache_ptr, 0]

start_while#:
//  .set dbcast_cache_ptr
    alu[--, dbcast_cache_ptr, -, LM_DBCAST_BLOCK_SIZE]
    bge[end_while#]
    move(dbcast_tbl_entry, *l$index/**/LM_INDEX/**/++)  // fetch dbcast table entry.
    beq[exit#], defer[1]                                // if table entry is zero then not a dbcast address,
    immed32(out_status, 0)//                            // exit with out_status = FALSE (0)
    alu_op(--, dbcast_tbl_entry, XOR, in_addr)          // else if table entry matches in_addr.
    beq[exit#], defer[1]
    immed32(out_status, IPV4_DBCAST_ADDRESS)            // exit with out_status = TRUE (IPV4_DBCAST_ADDRESS)
    br[start_while#], defer[1]                              // else then check next dbcast table entry.
    alu[dbcast_cache_ptr, dbcast_cache_ptr, +, 1]
end_while#:
// End of search for the current block,
// Go to index table, extract entry (byte) corosponding to the current block
// Check if next_block entry exist
    // To calculate the address of index table entry for current block, Get,
    //          index_table_offset  = (hashkey / 4) * 4
    //          byte offset in index table entry = hashkey % 4

    // Following instruction will get word aligned byte address of index table entry for current block_num,
    // get byte address use alu with ~ of 11

    alu[index_tbl_offset, block_num, AND~, 0x03 ]
//  add(dbcast_cache_ptr, LM_DBCAST_BASE, index_tbl_offset)//
    localmem_set_address((LM_DBCAST_BASE * 4), index_tbl_offset, LM_INDEX)

    // Get byte offset for "IndexTable" entry.
    // That is byte_offset = block_num % 4
    alu[byte_offset, block_num, AND, 0x3]
    // Multiply byte offset by eight to get the number of bits to be shifted
    alu_shf[byte_offset, --, b, byte_offset, <<3]
    // nop to take care CSR set latency
    nop
    move(block_num, *l$index/**/LM_INDEX/**/++)
    // Shift read entry by number of bits
    alu[--, byte_offset, OR, 0x0 ]
    alu_shf[block_num, --, b, block_num, >>indirect]
    // Extract the byte corrosponding to the block number
    alu[block_num, 0xFF, AND, block_num ]
    // if block number is empty then its end of serch, exit with zero
    beq[exit#], defer[1]
    immed32(out_status, 0)//
    // Else go to the check entry and start search of new block.
    br[check_entry#]

exit#:

    //Clear Namespace
    #undef _BIT_MASK
    #undef _SHIFT_BIT
    #undef _LM_DBCAST_ENTRY_BLOCK_COUNT
    #undef _LM_DBCAST_COLLISSION_BLOCK_COUNT
    #undef _LM_DBCAST_INDEX_TABLE_SIZE
    #undef _LM_DBCAST_ENTRY_TABLE_BASE

.end   //End Begin
#endm  //End of ipv4_is_dbcast_lm


/** @}
 * @}
 */

#endif /* __IPV4_UC__ */
