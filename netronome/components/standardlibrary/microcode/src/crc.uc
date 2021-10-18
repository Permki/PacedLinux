/*
 * Copyright (C) 2008-2011 Netronome Systems, Inc.  All rights reserved.
 */

#ifndef __CRC_UC__
#define __CRC_UC__

#ifndef NFP_LIB_ANY_NFAS_VERSION
    #if (!defined(__NFP_TOOL_NFAS) ||                       \
        (__NFP_TOOL_NFAS < NFP_SW_VERSION(5, 0, 0, 0)) ||   \
        (__NFP_TOOL_NFAS > NFP_SW_VERSION(6, 1, 0, 255)))
        #error "This standard library is not supported for the version of the SDK currently in use."
    #endif
#endif

#include <bytefield.uc>
#include <localmem.uc>
#include <stdmac.uc>
#include <constants.uc>

/** @file crc.uc Microengine CRC Macros
 * @addtogroup me_crc Microengine CRC
 * @{
 *
 * @name Microengine CRC Macros
 * @{
 *
 * This section creates a global absolute register:
 * @code @crc10_table_base_addr @endcode
 */

// API Quick Reference:
//
// crc_32 (out_remainder, in_data,  in_start_byte, in_end_byte)
// crc_ccitt (out_remainder, in_data,  in_start_byte, in_end_byte)
// crc_read (out_remainder)
// crc_write (in_remainder)
// crc_load_crc10_table ()
// crc_10 (out_remainder, in_data, in_start_byte, in_end_byte)



.reg global @crc10_table_base_addr


/// @cond INTERNAL_MACROS
/** Internal macro to define the appropriate assembler optional token.
 *
 * @param in_start_byte   Byte position where data starts. Must be a constant.
 *                        @p in_start_byte must be less than or equal @p in_end_byte.
 *
 * @param in_end_byte     Byte position where data ends. Must be an immediate value (a number).
 *
 * @macro_define @b _CRC_TOKENS: The @c opt_tok to assembler crc commands as
 *                               determined from the given macro parameters
 *
 * @note If @p in_start_byte = @p in_end_byte, only value 0 or 3 are legal. This means only the byte at
 *       either end of in_data can be used in the CRC operation.
 *
 * @note @p in_start_byte and @p in_end_byte should be consistent with the byte order used by the caller @n@n
 *       For example, if you want to use only the least significant byte:
 *       @arg @c big_endian: @p in_start_byte = @p in_end_byte = 3
 *       @arg @c little_endian: @p in_start_byte = @p in_end_byte = 0
 */
#macro _crc_byte_token(in_start_byte, in_end_byte)
    #ifdef _CRC_TOKENS
        #undef _CRC_TOKENS
    #endif

    #if (in_start_byte == in_end_byte)
        #if (in_start_byte == 0)
            #define _CRC_TOKENS     BYTE_0
        #elif (in_start_byte == 3)
            #define _CRC_TOKENS BYTE_3
        #endif
    #else
        #if (in_start_byte == 0)
            #if (in_end_byte == 1)
                #define _CRC_TOKENS BYTES_0_1
            #elif (in_end_byte == 2)
                #define _CRC_TOKENS BYTES_0_2
            #elif (in_end_byte == 3)
                #define _CRC_TOKENS BYTES_0_3
            #endif
        #elif ((in_start_byte == 1) && (in_end_byte == 3))
            #define _CRC_TOKENS BYTES_1_3
        #elif ((in_start_byte == 2) && (in_end_byte == 3))
            #define _CRC_TOKENS BYTES_2_3
        #else
            #error 4 "invalid combination of in_start_byte and in_end_byte for CRC macro"
        #endif
    #endif
#endm
/// @endcond


/** Load crc_10 lookup table.
 *
 * @param lm_crc10_base_addr    Base address from where CRC lookup table should start in local memory
 *
 * @note The user needs to make sure this area is not currupted later.
 *
 * @instruction_cnt 130
 */
#macro crc_load_crc10_table(lm_crc10_base_addr)
    // Set Local memory address.
    // Requires a 3 cycle delay before value is changed.
    // Make sure to write only after 3 cycles.
    .begin

        .reg tmp
        move(tmp, lm_crc10_base_addr)
        local_csr_wr[active_lm_addr_0, tmp]
        move(@crc10_table_base_addr, tmp)   // One Cycle.
        nop                                 // Two Cycle.
        nop                                 // Three Cycle.

    .end

    //Populate the CRC10 lookup table.
    immed32(*l$index0++, 0x00000233)
    immed32(*l$index0++, 0x02550066)
    immed32(*l$index0++, 0x029900aa)
    immed32(*l$index0++, 0x00cc02ff)
    immed32(*l$index0++, 0x03010132)
    immed32(*l$index0++, 0x01540367)
    immed32(*l$index0++, 0x019803ab)
    immed32(*l$index0++, 0x03cd01fe)

    immed32(*l$index0++, 0x00310202)
    immed32(*l$index0++, 0x02640057)
    immed32(*l$index0++, 0x02a8009b)
    immed32(*l$index0++, 0x00fd02ce)
    immed32(*l$index0++, 0x03300103)
    immed32(*l$index0++, 0x01650356)
    immed32(*l$index0++, 0x01a9039a)
    immed32(*l$index0++, 0x03fc01cf)

    immed32(*l$index0++, 0x00620251)
    immed32(*l$index0++, 0x02370004)
    immed32(*l$index0++, 0x02fb00c8)
    immed32(*l$index0++, 0x00ae029d)
    immed32(*l$index0++, 0x03630150)
    immed32(*l$index0++, 0x01360305)
    immed32(*l$index0++, 0x01fa03c9)
    immed32(*l$index0++, 0x03af019c)

    immed32(*l$index0++, 0x00530260)
    immed32(*l$index0++, 0x02060035)
    immed32(*l$index0++, 0x02ca00f9)
    immed32(*l$index0++, 0x009f02ac)
    immed32(*l$index0++, 0x03520161)
    immed32(*l$index0++, 0x01070334)
    immed32(*l$index0++, 0x01cb03f8)
    immed32(*l$index0++, 0x039e01ad)

    immed32(*l$index0++, 0x00c402f7)
    immed32(*l$index0++, 0x029100a2)
    immed32(*l$index0++, 0x025d006e)
    immed32(*l$index0++, 0x0008023b)
    immed32(*l$index0++, 0x03c501f6)
    immed32(*l$index0++, 0x019003a3)
    immed32(*l$index0++, 0x015c036f)
    immed32(*l$index0++, 0x0309013a)

    immed32(*l$index0++, 0x00f502c6)
    immed32(*l$index0++, 0x02a00093)
    immed32(*l$index0++, 0x026c005f)
    immed32(*l$index0++, 0x0039020a)
    immed32(*l$index0++, 0x03f401c7)
    immed32(*l$index0++, 0x01a10392)
    immed32(*l$index0++, 0x016d035e)
    immed32(*l$index0++, 0x0338010b)

    immed32(*l$index0++, 0x00a60295)
    immed32(*l$index0++, 0x02f300c0)
    immed32(*l$index0++, 0x023f000c)
    immed32(*l$index0++, 0x006a0259)
    immed32(*l$index0++, 0x03a70194)
    immed32(*l$index0++, 0x01f203c1)
    immed32(*l$index0++, 0x013e030d)
    immed32(*l$index0++, 0x036b0158)

    immed32(*l$index0++, 0x009702a4)
    immed32(*l$index0++, 0x02c200f1)
    immed32(*l$index0++, 0x020e003d)
    immed32(*l$index0++, 0x005b0268)
    immed32(*l$index0++, 0x039601a5)
    immed32(*l$index0++, 0x01c303f0)
    immed32(*l$index0++, 0x010f033c)
    immed32(*l$index0++, 0x035a0169)

    immed32(*l$index0++, 0x018803bb)
    immed32(*l$index0++, 0x03dd01ee)
    immed32(*l$index0++, 0x03110122)
    immed32(*l$index0++, 0x01440377)
    immed32(*l$index0++, 0x028900ba)
    immed32(*l$index0++, 0x00dc02ef)
    immed32(*l$index0++, 0x00100223)
    immed32(*l$index0++, 0x02450076)

    immed32(*l$index0++, 0x01b9038a)
    immed32(*l$index0++, 0x03ec01df)
    immed32(*l$index0++, 0x03200113)
    immed32(*l$index0++, 0x01750346)
    immed32(*l$index0++, 0x02b8008b)
    immed32(*l$index0++, 0x00ed02de)
    immed32(*l$index0++, 0x00210212)
    immed32(*l$index0++, 0x02740047)

    immed32(*l$index0++, 0x01ea03d9)
    immed32(*l$index0++, 0x03bf018c)
    immed32(*l$index0++, 0x03730140)
    immed32(*l$index0++, 0x01260315)
    immed32(*l$index0++, 0x02eb00d8)
    immed32(*l$index0++, 0x00be028d)
    immed32(*l$index0++, 0x00720241)
    immed32(*l$index0++, 0x02270014)

    immed32(*l$index0++, 0x01db03e8)
    immed32(*l$index0++, 0x038e01bd)
    immed32(*l$index0++, 0x03420171)
    immed32(*l$index0++, 0x01170324)
    immed32(*l$index0++, 0x02da00e9)
    immed32(*l$index0++, 0x008f02bc)
    immed32(*l$index0++, 0x00430270)
    immed32(*l$index0++, 0x02160025)

    immed32(*l$index0++, 0x014c037f)
    immed32(*l$index0++, 0x0319012a)
    immed32(*l$index0++, 0x03d501e6)
    immed32(*l$index0++, 0x018003b3)
    immed32(*l$index0++, 0x024d007e)
    immed32(*l$index0++, 0x0018022b)
    immed32(*l$index0++, 0x00d402e7)
    immed32(*l$index0++, 0x028100b2)

    immed32(*l$index0++, 0x017d034e)
    immed32(*l$index0++, 0x0328011b)
    immed32(*l$index0++, 0x03e401d7)
    immed32(*l$index0++, 0x01b10382)
    immed32(*l$index0++, 0x027c004f)
    immed32(*l$index0++, 0x0029021a)
    immed32(*l$index0++, 0x00e502d6)
    immed32(*l$index0++, 0x02b00083)

    immed32(*l$index0++, 0x012e031d)
    immed32(*l$index0++, 0x037b0148)
    immed32(*l$index0++, 0x03b70184)
    immed32(*l$index0++, 0x01e203d1)
    immed32(*l$index0++, 0x022f001c)
    immed32(*l$index0++, 0x007a0249)
    immed32(*l$index0++, 0x00b60285)
    immed32(*l$index0++, 0x02e300d0)

    immed32(*l$index0++, 0x011f032c)
    immed32(*l$index0++, 0x034a0179)
    immed32(*l$index0++, 0x038601b5)
    immed32(*l$index0++, 0x01d303e0)
    immed32(*l$index0++, 0x021e002d)
    immed32(*l$index0++, 0x004b0278)
    immed32(*l$index0++, 0x008702b4)
    immed32(*l$index0++, 0x02d200e1)
#endm


/// @cond INTERNAL_MACROS
/** Perform a crc 10 computation on a byte, using a crc10 lookup table.
 *
 * @param fcs       Value of remainder. Can be a GPR or a sram/dram transfer out
 *                  register (a.k.a. write transfer register).
 * @param in_data   Source data on which CRC operation will be performed.
 *
 * @instruction_cnt 11
 */
#macro _crc_10_fcs(fcs, in_data)
.begin

    .reg index
    .reg tmp tmp1

    // Compute the index in the CRC10 lookup table.
    // [base_address+(((fcs) >> 2) & 0xFF)]
    alu_shf[tmp, 0xff, AND, fcs, >>2]

    // We require a 32bit word index, which requires the value
    // to be divided by 2, and need to align localmem address at a
    // word boundry.
    alu_shf[tmp1, 3, ~AND, tmp, <<1]
    alu[index, @crc10_table_base_addr, +, tmp1]

    // Read a 32bit word from the crc lookup table.
    // Needs to have 3 cycle delay. Make sure to
    // read it only after 3 cycles.
    local_csr_wr[active_lm_addr_0, index]
    // Compute [((fcs) << 8) & 0x3FF)]
    alu_shf_left(fcs, 0x03FF, AND, fcs, 8) // Two cycle delay

    //.if (!odd)
    alu_shf[tmp, --, B, tmp, <<31]      // Three cycle delay.
    beq[crc_10_fcs_br#], defer[1]
        // A even number. CRC value in the Hi word of
        // the 32bit CRC word got from Lookup table.
        bytefield_extract(tmp, *l$index0, 0, 1, NO_LOAD_CC)
    //.else
        // An Odd number. CRC value in the low word of
        // the 32bit CRC word got from Lookup table.
        bytefield_extract(tmp, *l$index0, 2, 3, NO_LOAD_CC)
    //.endif

crc_10_fcs_br#:

    // Compute CRC10 =  (((fcs) << 8) & 0x3ff)^ crc10_table[((fcs) >> 2) & 0xff] ^ (c))
    // crc_lookup_value = crc10_table[index]^in_data
    alu[tmp, tmp, XOR, in_data]

    // crc_lookup_value ^= ((fcs) << 8) & 0x3ff)
    alu[fcs, fcs, XOR, tmp]

.end
#endm
/// @endcond


/** Perform a crc 10 computation.
 *
 * The CRC Remainder CSR must contain the remainder
 * intended to be used in this CRC 10 operation (usually it is the remainder of the
 * previous CRC operation).
 *
 * @param out_remainder Value of remainder. Can be GPR or sram/dram transfer out
 *                      register (a.k.a. write transfer register).
 * @param in_data       Source data on which CRC operation will be performed.
 * @param ENDIAN        String with two possible values:
 *                          @arg @c little_endian:  use crc_le operation (the data will
 *                                          be swaped before being used to calculate
 *                                          CRC remainder)
 *                          @arg @c big_endian:     use crc_be operation (no data swapping)
 * @param in_start_byte Byte position where data starts. Must be an immediate value (a number).
 *                      @p in_start_byte must be less than or equal @p in_end_byte.
 *
 * @param in_end_byte   Byte position where data ends. Must be an immediate value (a number).
 *
 * @note If @p in_start_byte = @p in_end_byte, only values 0 or 3 are legal. This means only the byte at
 *       either end of @p in_data can be used in the CRC operation.
 *
 * @note @p in_start_byte and @p in_end_byte should be consistent with the byte order specified in @p ENDIAN. @n
 *       Example: If you want to use only the least significant byte:
 *       @arg @p ENDIAN = @c big_endian    ---> @p in_start_byte = @p in_end_byte = 3
 *       @arg @p ENDIAN = @c little_endian ---> @p in_start_byte = @p in_end_byte = 0
 *
 * @note Each time this operation is performed, the new remainder is put in the CRC Remainder CSR.
 *
 * @instruction_cnt 27 for 1 byte
 * @instruction_cnt 40 for 2 bytes
 * @instruction_cnt 54 for 3 bytes
 * @instruction_cnt 68 for 4 bytes
 */
#macro crc_10(out_remainder, in_data, ENDIAN, in_start_byte, in_end_byte)
    #if !((is_ct_const(in_start_byte) || isimport(in_start_byte)) && (is_ct_const(in_end_byte) || isimport(in_end_byte)))
        #error 4 "Macro crc_10: in_start_byte and in_end_byte must be immdediate values"
    #endif

    #if (in_start_byte > in_end_byte)
        #error 4 "Macro crc_10: in_start_byte must be less than or equal to in_end_byte"
    #endif

    #if (in_start_byte > 3)
        #error 4 "Macro crc_10: in_start_byte must be less than 3"
    #endif

    #if (((in_end_byte - in_start_byte) + 1) > 4)
        #error 4 "Macro crc_10: len more than 4 bytes not valid"
    #endif

    .begin

        .reg    data tmp //save_active_lm

        // Get the Old CRC Reminder.
        crc_read(out_remainder)

        // Copy in_data byte in a local reg.
        alu[tmp, --, B, in_data]

        #if (streq(ENDIAN, big_endian))
            #if ((IN_END_BYTE - IN_START_BYTE) == 0)
                bytefield_extract(data, tmp, IN_START_BYTE, IN_START_BYTE, NO_LOAD_CC)

                // Call CRC10 helper function to calculate CRC10 reminder.
                _crc_10_fcs(out_remainder, data)
            #elif ((IN_END_BYTE - IN_START_BYTE) == 1)
                bytefield_extract(data, tmp, IN_START_BYTE, IN_START_BYTE, NO_LOAD_CC)

                // Call CRC10 helper function to calculate CRC10 reminder.
                _crc_10_fcs(out_remainder, data)

                #define_eval _IN_START_BYTE (IN_START_BYTE+1)
                bytefield_extract(data, tmp, _IN_START_BYTE, _IN_START_BYTE, NO_LOAD_CC)

                // Call CRC10 helper function to calculate CRC10 reminder.
                _crc_10_fcs(out_remainder, data)
            #elif ((IN_END_BYTE - IN_START_BYTE) == 2)
                bytefield_extract(data, tmp, IN_START_BYTE, IN_START_BYTE, NO_LOAD_CC)

                // Call CRC10 helper function to calculate CRC10 reminder.
                _crc_10_fcs(out_remainder, data)

                #define_eval _IN_START_BYTE (IN_START_BYTE+1)
                bytefield_extract(data, tmp, _IN_START_BYTE, _IN_START_BYTE, NO_LOAD_CC)

                // Call CRC10 helper function to calculate CRC10 reminder.
                _crc_10_fcs(out_remainder, data)

                #define_eval _IN_START_BYTE (IN_START_BYTE+2)
                bytefield_extract(data, tmp, _IN_START_BYTE, _IN_START_BYTE, NO_LOAD_CC)

                // Call CRC10 helper function to calculate CRC10 reminder.
                _crc_10_fcs(out_remainder, data)
            #else // ((IN_END_BYTE - IN_START_BYTE) == 3)
                bytefield_extract(data, tmp, IN_START_BYTE, IN_START_BYTE, NO_LOAD_CC)

                // Call CRC10 helper function to calculate CRC10 reminder.
                _crc_10_fcs(out_remainder, data)

                #define_eval _IN_START_BYTE (IN_START_BYTE+1)
                bytefield_extract(data, tmp, _IN_START_BYTE, _IN_START_BYTE, NO_LOAD_CC)

                // Call CRC10 helper function to calculate CRC10 reminder.
                _crc_10_fcs(out_remainder, data)

                #define_eval _IN_START_BYTE (IN_START_BYTE+2)
                bytefield_extract(data, tmp, _IN_START_BYTE, _IN_START_BYTE, NO_LOAD_CC)

                // Call CRC10 helper function to calculate CRC10 reminder.
                _crc_10_fcs(out_remainder, data)

                #define_eval _IN_START_BYTE (IN_START_BYTE+3)
                bytefield_extract(data, tmp, _IN_START_BYTE, _IN_START_BYTE, NO_LOAD_CC)

                // Call CRC10 helper function to calculate CRC10 reminder.
                _crc_10_fcs(out_remainder, data)
            #endif
        #elif (streq(ENDIAN, little_endian))
            #define_eval _IN_START_BYTE (3 - IN_START_BYTE)
            #define_eval _IN_END_BYTE (3 - IN_END_BYTE)

            #if ((_IN_START_BYTE - _IN_END_BYTE) == 0)
                bytefield_extract(data, tmp, _IN_START_BYTE, _IN_START_BYTE, NO_LOAD_CC)

                // Call CRC10 helper function to calculate CRC10 reminder.
                _crc_10_fcs(out_remainder, data)
            #elif ((_IN_START_BYTE - _IN_END_BYTE) == 1)
                bytefield_extract(data, tmp, _IN_START_BYTE, _IN_START_BYTE, NO_LOAD_CC)

                // Call CRC10 helper function to calculate CRC10 reminder.
                _crc_10_fcs(out_remainder, data)

                #define_eval _IN_START_BYTE (_IN_START_BYTE-1)
                bytefield_extract(data, tmp, _IN_START_BYTE, _IN_START_BYTE, NO_LOAD_CC)

                // Call CRC10 helper function to calculate CRC10 reminder.
                _crc_10_fcs(out_remainder, data)
            #elif ((_IN_START_BYTE - _IN_END_BYTE) == 2)
                bytefield_extract(data, tmp, _IN_START_BYTE, _IN_START_BYTE, NO_LOAD_CC)

                // Call CRC10 helper function to calculate CRC10 reminder.
                _crc_10_fcs(out_remainder, data)

                #define_eval _IN_START_BYTE (_IN_START_BYTE-1)
                bytefield_extract(data, tmp, _IN_START_BYTE, _IN_START_BYTE, NO_LOAD_CC)

                // Call CRC10 helper function to calculate CRC10 reminder.
                _crc_10_fcs(out_remainder, data)

                #define_eval _IN_START_BYTE (_IN_START_BYTE-1)
                bytefield_extract(data, tmp, _IN_START_BYTE, _IN_START_BYTE, NO_LOAD_CC)

                // Call CRC10 helper function to calculate CRC10 reminder.
                _crc_10_fcs(out_remainder, data)
            #else // ((IN_END_BYTE - IN_START_BYTE) == 3)
                bytefield_extract(data, tmp, _IN_START_BYTE, _IN_START_BYTE, NO_LOAD_CC)

                // Call CRC10 helper function to calculate CRC10 reminder.
                _crc_10_fcs(out_remainder, data)

                #define_eval _IN_START_BYTE (_IN_START_BYTE-1)
                bytefield_extract(data, tmp, _IN_START_BYTE, _IN_START_BYTE, NO_LOAD_CC)

                // Call CRC10 helper function to calculate CRC10 reminder.
                _crc_10_fcs(out_remainder, data)

                #define_eval _IN_START_BYTE (_IN_START_BYTE-1)
                bytefield_extract(data, tmp, _IN_START_BYTE, _IN_START_BYTE, NO_LOAD_CC)

                // Call CRC10 helper function to calculate CRC10 reminder.
                _crc_10_fcs(out_remainder, data)

                #define_eval _IN_START_BYTE (_IN_START_BYTE-1)
                bytefield_extract(data, tmp, _IN_START_BYTE, _IN_START_BYTE, NO_LOAD_CC)

                // Call CRC10 helper function to calculate CRC10 reminder.
                _crc_10_fcs(out_remainder, data)
            #endif
        #else
                #error 4 "Macro crc_10: invalid value for ENDIAN in crc_10 macro"
        #endif

        // Write back the new CRC in the CRC reminder register.
        crc_write(out_remainder)
        nop
        nop
        nop

    .end
#endm


/** Perform a crc 32 computation.
 *
 * The CRC Remainder CSR must contain the remainder intended
 * to be used in this CRC 32 operation (usually it is the remainder of the previous CRC
 * operation).
 *
 * @param out_remainder The value of remainder. Can be a GPR or a sram/dram transfer out
 *                      register (a.k.a. write transfer register).
 * @param in_data       Source data on which CRC operation will be performed
 * @param ENDIAN        String with two possible values:
 *                          @arg @c little_endian:  use @c crc_le operation (the data will
 *                                          be swapped before being used to calculate
 *                                          CRC remainder)
 *                          @arg @c big_endian: use crc_be operation (no data swapping)
 *
 * @param in_start_byte Byte position where data starts. Must be a immediate value (a number).
 *                     @p in_start_byte must be less than or equal @p in_end_byte
 *
 * @param in_end_byte   Byte position where data ends. Must be an immediate value (a number).
 *
 * @note If @p in_start_byte == @p in_end_byte, only values 0 or 3 are legal. This means only the byte at
 *       either end of @p in_data can be used in the CRC operation.
 *
 * @note @p in_start_byte and @p in_end_byte should be consistent with the byte order specified in @p ENDIAN. @n
 *       Example: If you want to use only the least significant byte:
 *       @arg @p ENDIAN = @c big_endian    ---> @p in_start_byte = @p in_end_byte = 3
 *       @arg @p ENDIAN = @c little_endian ---> @p in_start_byte = @p in_end_byte = 0
 *
 * @note Each time this operation is performed, the new remainder is put in the CRC Remainder CSR.
 *
 * @instruction_cnt 1
 */
#macro crc_32(out_remainder, in_data, ENDIAN, in_start_byte, in_end_byte)
    #if !((is_ct_const(in_start_byte) || isimport(in_start_byte)) && (is_ct_const(in_end_byte) || isimport(in_end_byte)))
        #error 4 "Macro crc_32: in_start_byte and in_end_byte must be immdediate values"
    #endif

    #if (in_start_byte == in_end_byte)
        #if (in_start_byte == 1)
            #error 4 "Macro crc_32: invalid byte selected. Only byte 0, byte 3 are allowed"
        #elif (in_start_byte == 2)
            #error 4 "Macro crc_32: invalid byte selected. Only byte 0, byte 3 are allowed"
        #endif
    #elif (in_start_byte > in_end_byte)
        #error 4 "Macro crc_32: in_start_byte must be less than or equal to in_end_byte"
    #endif

    _crc_byte_token(in_start_byte, in_end_byte)

    #if (streq(ENDIAN, little_endian))
        crc_le[crc_32, out_remainder, in_data], _CRC_TOKENS
    #elif (streq(ENDIAN, big_endian))
        crc_be[crc_32, out_remainder, in_data], _CRC_TOKENS
    #else
        #error 4 "Macro crc_32: invalid value for ENDIAN in crc_32 macro"
    #endif
#endm


/** Perform a crc_ccitt computation.
 *
 * The CRC Remainder CSR must contain the remainder intended
 * to be used in this CRC operation (usually it is the remainder of the previous CRC
 * operation).
 *
 * @param out_remainder Value of remainder. Can be a GPR or a sram/dram transfer out
 *                      register (a.k.a. write transfer register).
 * @param in_data       Source data on which CRC operation will be performed
 * @param ENDIAN        String with two possible values:
 *                          @arg @c little_endian:  use crc_le operation (the data will
 *                                          be swapped before being used to calculate
 *                                          CRC remainder)
 *                          @arg @c big_endian:     use crc_be operation (no data swapping)
 *
 * @param in_start_byte Byte position where data starts. Must be a immediate value (a number).
 *                      @p in_start_byte must be less than or equal @p in_end_byte.
 *
 * @param in_end_byte   Byte position where data ends. Must be an immediate value (a number).
 *
 * @note If @p in_start_byte == @p in_end_byte, only value 0 or 3 are legal.This means only the byte at
 *       either end of @p in_data can be used in the CRC operation.
 *
 * @note @p in_start_byte and @p in_end_byte should be consistent with the byte order specified in @p ENDIAN. @n
 *       Example: If you want to use only the least significant byte:
 *       @arg @p ENDIAN = @c big_endian    ---> @p in_start_byte = @p in_end_byte = 3
 *       @arg @p ENDIAN = @c little_endian ---> @p in_start_byte = @p in_end_byte = 0
 *
 * @note Each time this operation is performed, the new remainder is put in the CRC Remainder CSR.
 *
 * @note NFP-32xx can perform CRC operation on big or little endian data.
 *
 * @instruction_cnt 1
 */
#macro crc_ccitt(out_remainder, in_data, ENDIAN, in_start_byte, in_end_byte)
    #if !((is_ct_const(in_start_byte) || isimport(in_start_byte)) && (is_ct_const(in_end_byte) || isimport(in_end_byte)))
        #error 4 "Macro crc_ccitt: in_start_byte and in_end_byte must be immdediate values"
    #endif

    #if (in_start_byte == in_end_byte)
        #if (in_start_byte == 1)
            #error 4 "Macro crc_ccitt: invalid byte selected. Only byte 0, byte 3 are allowed"
        #elif (in_start_byte == 2)
            #error 4 "Macro crc_ccitt: invalid byte selected. Only byte 0, byte 3 are allowed"
        #endif
    #elif (in_start_byte > in_end_byte)
        #error 4 "Macro crc_ccitt: in_start_byte must be less than or equal to in_end_byte"
    #endif

    _crc_byte_token(in_start_byte, in_end_byte)

    #if (streq(ENDIAN, little_endian))
        crc_le[crc_ccitt, out_remainder, in_data], _CRC_TOKENS
    #elif (streq(ENDIAN, big_endian))
        crc_be[crc_ccitt, out_remainder, in_data], _CRC_TOKENS
    #else
        #error 4 "Macro crc_ccitt: invalid value for ENDIAN in crc_ccitt macro"
    #endif
#endm


/** Read the data that is currently in the CSR CRC_Remainder.
 *
 * @param out_remainder Value of remainder. Can be a GPR or a sram/dram transfer out
 *                      register (a.k.a. write transfer register).
 * @instruction_cnt 2
 */
#macro crc_read(out_remainder)
    local_csr_rd[crc_remainder]
    immed[out_remainder, 0]
#endm


/** Write the input data into the CRC unit.
 *
 *
 * @param in_remainder  Value of remainder to be written. Can be a GPR or a sram/dram
 *                      transfer in register (a.k.a. read transfer register).
 *
 * @note This operation needs 3 cycles for data to be updated. Therefore, there must be at
 *       least 3 instructions between calling this macro and reading the CRC Remainder or using
 *       the remainder via @c crc_ccitt or @c CRC_32 operation.
 *
 * @instruction_cnt 1
 */
#macro crc_write(in_remainder)
    local_csr_wr[crc_remainder, in_remainder]
#endm


/** @}
 * @}
 */

#endif /* __CRC_UC__ */
