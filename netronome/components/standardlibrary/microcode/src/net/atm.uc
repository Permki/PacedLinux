/*
 * Copyright (C) 2008-2011 Netronome Systems, Inc.  All rights reserved.
 */

#ifndef __ATM_UC__
#define __ATM_UC__

#ifndef NFP_LIB_ANY_NFAS_VERSION
    #if (!defined(__NFP_TOOL_NFAS) ||                       \
        (__NFP_TOOL_NFAS < NFP_SW_VERSION(5, 0, 0, 0)) ||   \
        (__NFP_TOOL_NFAS > NFP_SW_VERSION(6, 1, 0, 255)))
        #error "This standard library is not supported for the version of the SDK currently in use."
    #endif
#endif

#include <bytefield.uc>
#include <localmem.uc>
#include <crc.uc>
#include <stdmac.uc>
#include <constants.uc>
#include <xbuf.uc>

/** @file atm.uc Microengine ATM Macros
 * @addtogroup atm Microengine ATM
 * @{
 *
 * @name Microengine ATM Macros
 * @{
 *
 */



// ATM Header Format.
//
//        UNI Header (Big Endian) Format.                          NNI Header (Big Endian) Format.
//        ==============================                           ==============================
//
//        0  1 2 3 4 5 6 7  bits                                  0  1 2 3 4 5 6 7  bits
//        +-+-+-+-+-+-+-+-+                                       +-+-+-+-+-+-+-+-+
//        | GFC   | VPI   | 0 byte                                |      VPI      | 0 byte
//        +-+-+-+-+-+-+-+-+                                       +-+-+-+-+-+-+-+-+
//        |  VPI  | VCI   | 1 byte                                |  VPI  | VCI   | 1 byte
//        +-+-+-+-+-+-+-+-+                                       +-+-+-+-+-+-+-+-+
//        |     VCI       | 2 byte                                |     VCI       | 2 byte
//        +-+-+-+-+-+-+-+-+                                       +-+-+-+-+-+-+-+-+
//        |  VCI  | PTI |X---> (CLP Field)                        |  VCI  | PTI |X---> (CLP Field)
//        +-+-+-+-+-+-+-+-+ 3 byte                                +-+-+-+-+-+-+-+-+ 3 byte
//        |      HEC      | 4 byte                                |      HEC      | 4 byte
//        +-+-+-+-+-+-+-+-+                                       +-+-+-+-+-+-+-+-+
//
//
// (Note:) HEC byte would be removed and added by the Framer.


// API:
//
//   atm_compare_vpi_vci(in_headerPtr, BYTE_OFFSET, vpi, vci, IN_ATM_HDR_TYPE, ENDIAN)
//   atm_compare_vpi_vci_pti(in_headerPtr, BYTE_OFFSET, vpi, vci, pti, IN_ATM_HDR_TYPE, ENDIAN)
//   oam_gen_crc10(in_OamCell_ptr, ENDIAN)
//   oam_validate_crc10(in_OamCell_ptr, ENDIAN)
//
///////////////////////////////////////////////////////////////////////////////////////

/// @cond INTERNAL_MACROS
/** This macro generates the CRC10 checksum.
 *
 * @param out_residue      Computed CRC10 checksum
 * @param in_OamCell_ptr   Pointer to the OamCell
 * @param ENDIAN           Immediate value for endian type either big_endian, little_endian
 *
 */
#macro _gen_oam_crc10 (out_residue, in_OamCell_ptr, ENDIAN)
    crc_10 (out_residue, in_OamCell_ptr[0], ENDIAN, 0, 3)
    crc_10 (out_residue, in_OamCell_ptr[1], ENDIAN, 0, 3)
    crc_10 (out_residue, in_OamCell_ptr[2], ENDIAN, 0, 3)
    crc_10 (out_residue, in_OamCell_ptr[3], ENDIAN, 0, 3)
    crc_10 (out_residue, in_OamCell_ptr[4], ENDIAN, 0, 3)
    crc_10 (out_residue, in_OamCell_ptr[5], ENDIAN, 0, 3)
    crc_10 (out_residue, in_OamCell_ptr[6], ENDIAN, 0, 3)
    crc_10 (out_residue, in_OamCell_ptr[7], ENDIAN, 0, 3)

    _GET_XBUF_TYPE(in_OamCell_ptr)
    #define_eval    _EXTRACT_SRC_XBUF_TYPE  _XBUF_TYPE
    #undef _XBUF_TYPE

    #if (_EXTRACT_SRC_XBUF_TYPE == _XBUF_LMEM)
        /////////////LMEM WORK-AROUND////////////////////////////////
        ;copy source in temp registers because we can't increment lmem
        ;pointer and can't do byte_align with lmem index > 7
        .begin
            .reg b8

            xbuf_extract(b8, in_OamCell_ptr, 0, 32, 4)
            crc_10 (out_residue, b8, ENDIAN, 0, 3)

            xbuf_extract(b8, in_OamCell_ptr, 0, 36, 4)
            crc_10 (out_residue, b8, ENDIAN, 0, 3)

            xbuf_extract(b8, in_OamCell_ptr, 0, 40, 4)
            crc_10 (out_residue, b8, ENDIAN, 0, 3)

            xbuf_extract(b8, in_OamCell_ptr, 0, 44, 4)
            crc_10 (out_residue, b8, ENDIAN, 0, 3)
        .end
    #else
        crc_10 (out_residue, in_OamCell_ptr[8], ENDIAN, 0, 3)
        crc_10 (out_residue, in_OamCell_ptr[9], ENDIAN, 0, 3)
        crc_10 (out_residue, in_OamCell_ptr[10], ENDIAN, 0, 3)
        crc_10 (out_residue, in_OamCell_ptr[11], ENDIAN, 0, 3)
    #endif
#endm
/// @endcond


/** This compares the passed vpi, and vci values within the ATM cell header.
 *
 * This requires a pointer to the location where the Atm header is located.
 *
 * @param in_atmHeaderPtr     Pointer to ATM Header
 * @param BYTE_OFFSET         Byte position from where ATM header starts
 * @param vpi                 vpi value for comparision
 * @param vci                 vci value for comparision
 * @param IN_ATM_HDR_TYPE     ATM Header type an immediate value either ATM_UNI or ATM_NNI
 * @param ENDIAN              Immediate value for endian type either big_endian, little_endian
 *
 * @instruction_cnt 7
 *
 * @condition_codes vpi, vci matching if condition code == 0, otherwise not matching.
 *
 * @b Example:
 * @code
 * immed32(atm_header[0], 0x01000110)
 * atm_compare_vpi_vci (atm_header, 0, 0x11, 0x1000, ATM_UNI, little_endian)
 * @endcode
 */
#macro atm_compare_vpi_vci(in_headerPtr, ATM_HDR_START_BYTE, vpi, vci, IN_ATM_HDR_TYPE, ENDIAN)
    #if (ATM_HDR_START_BYTE > 3)
        #error 4 "Macro atm_compare_vpi_vci: ATM_HDR_START_BYTE must be less than or equal to 3"
    #endif
    .begin

        .reg atm_hdr vpi_vci

        xbuf_extract(atm_hdr, in_headerPtr, 0, ATM_HDR_START_BYTE, 4) // get the atm_hdr word.

        #if (streq(IN_ATM_HDR_TYPE, ATM_UNI))
            #if (streq(ENDIAN, little_endian))
                // concatenate vpi vci fields.
                move (vpi_vci, vci)
                alu_shf_left(vpi_vci, vpi, OR, vpi_vci, 8)

                // Get VPI field vpi = (atm_hdr & 0x00000FF0) >> 4
                // Get VCI field vci = (atm_hdr & 0x0FFFF000) >> 12
                alu_shf_right(atm_hdr, 0xFFFFFF, AND, atm_hdr, 4)

                //.if (vpi_fld == vpi && vci_fld == vci)
                alu[--, atm_hdr, -, vpi_vci]
            #elif (streq(ENDIAN, big_endian))
                // concatenate vpi vci fields.
                move (vpi_vci, vpi)
                alu_shf_left(vpi_vci, vci, OR, vpi_vci, 16)

                // Get VPI field vpi = (atm_hdr & 0x0FF00000) >> 20
                // Get VCI field vci = (atm_hdr & 0x000FFFF0) >> 4
                alu_shf_right(atm_hdr, 0xFFFFFF, AND, atm_hdr, 4)

                //.if (vpi_fld == vpi && vci_fld == vci)
                alu[--, atm_hdr, -, vpi_vci]
            #else
                #error 4 "Macro atm_compare_vpi_vci: invalid value for ENDIAN in atm_compare_vpi_vci macro"
            #endif
        #elif (streq(IN_ATM_HDR_TYPE, ATM_NNI))
            #if (streq(ENDIAN, little_endian))
                // concatenate vpi vci fields.
                move (vpi_vci, vci)
                alu_shf_left(vpi_vci, vpi, OR, vpi_vci, 12)

                // Get VPI field vpi = (atm_hdr & 0x00000FFF)
                // Get VCI field vci = (atm_hdr & 0x0FFFF000) >> 12
                alu_op(atm_hdr, 0xFFFFFFF, AND, atm_hdr)

                //.if (vpi_fld == vpi && vci_fld == vci)
                alu[--, atm_hdr, -, vpi_vci]

            #elif (streq(ENDIAN, big_endian))
                // concatenate vpi vci fields.
                move (vpi_vci, vpi)
                alu_shf_left(vpi_vci, vci, OR, vpi_vci, 16)

                // Get VPI field vpi = (atm_hdr & 0xFFF00000) >> 24
                // Get VCI field vci = (atm_hdr & 0x000FFFF0) >> 4
                alu_shf_right(atm_hdr, 0xFFFFFFF, AND, atm_hdr, 4)

                //.if (vpi_fld == vpi && vci_fld == vci)
                alu[--, atm_hdr, -, vpi_vci]
            #else
                #error 4 "Macro atm_compare_vpi_vci: invalid value for ENDIAN in atm_compare_vpi_vci macro"
            #endif
        #else
            #error 4 "Macro atm_compare_vpi_vci: invalid value for IN_ATM_HDR_TYPE in atm_compare_vpi_vci macro"
        #endif

    end#:

    .end
#endm

/** This compares the passed vpi, and vci values within the ATM cell header.
 *
 * This requires a pointer to the location where the Atm header is located.
 *
 * @param in_atmHeaderPtr        Pointer to the ATM Header
 * @param ATM_HDR_START_BYTE     Byte position from where ATM header starts
 * @param vpi                    vpi value for comparision
 * @param vci                    vci value for comparision
 * @param pti                    pti value for comparision
 * @param IN_ATM_HDR_TYPE        ATM Header type an immediate value either ATM_UNI or ATM_NNI
 * @param ENDIAN                 Immediate value for endian type either big_endian, little_endian
 *
 * @instruction_cnt 7
 *
 * @condition_codes vpi, vci and pti matching if condition code == 0, otherwise not matching.
 *
 * @b Example:
 * @code
 * .reg vpi vci pti atm_header
 * move (vpi, 0x11)
 * move (vci, 0x1000)
 * move (pti, 4)
 * immed32(atm_header[0], 0x01110008)
 * atm_compare_vpi_vci_pti (atm_header, 0, vpi, vci, pti, ATM_UNI, big_endian)
 * @endcode
 *
 */
#macro atm_compare_vpi_vci_pti(in_headerPtr, ATM_HDR_START_BYTE, vpi, vci, pti, IN_ATM_HDR_TYPE, ENDIAN)
    #if (ATM_HDR_START_BYTE > 3)
        #error 4 "Macro atm_compare_vpi_vci_pti: ATM_HDR_START_BYTE must be less than or equal to 3"
    #endif

    .begin

        .reg atm_hdr vpi_vci_pti

        xbuf_extract(atm_hdr, in_headerPtr, 0, ATM_HDR_START_BYTE, 4) // get the atm_hdr word.

        #if (streq(IN_ATM_HDR_TYPE, ATM_UNI))

            #if (streq(ENDIAN, little_endian))
                // concatenate vpi vci, pti fields.
                move (vpi_vci_pti, pti)
                alu_shf_left(vpi_vci_pti, vci, OR, vpi_vci_pti, 16)
                alu_shf_left(vpi_vci_pti, vpi, OR, vpi_vci_pti, 8)

                alu_shf_right(atm_hdr, 0x7FFFFFF, AND, atm_hdr, 4)

                //.if (vpi_fld == vpi && vci_fld == vci)
                alu[--, atm_hdr, -, vpi_vci_pti]

            #elif (streq(ENDIAN, big_endian))
                // concatenate vpi vci, pti fields.
                move (vpi_vci_pti, vpi)
                alu_shf_left(vpi_vci_pti, vci, OR, vpi_vci_pti, 16)
                alu_shf_left(vpi_vci_pti, pti, OR, vpi_vci_pti, 3)

                shf_right (atm_hdr, atm_hdr, 1)

                //.if (vpi_fld == vpi && vci_fld == vci)
                alu[--, atm_hdr, -, vpi_vci_pti]
            #else
                #error 4 "Macro atm_compare_vpi_vci_pti: invalid value for ENDIAN in atm_compare_vpi_vci_pti macro"
            #endif
        #elif (streq(IN_ATM_HDR_TYPE, ATM_NNI))
            #if (streq(ENDIAN, little_endian))
                // concatenate vpi vci, pti fields.
                move (vpi_vci_pti, pti)
                alu_shf_left(vpi_vci_pti, vci, OR, vpi_vci_pti, 16)
                alu_shf_left(vpi_vci_pti, vpi, OR, vpi_vci_pti, 12)

                alu_op (atm_hdr, 0x7FFFFFFF, AND, atm_hdr)

                //.if (vpi_fld == vpi && vci_fld == vci)
                alu[--, atm_hdr, -, vpi_vci_pti]

            #elif (streq(ENDIAN, big_endian))
                // concatenate vpi vci, pti fields.
                move (vpi_vci_pti, vpi)
                alu_shf_left(vpi_vci_pti, vci, OR, vpi_vci_pti, 16)
                alu_shf_left(vpi_vci_pti, pti, OR, vpi_vci_pti, 3)

                shf_right (atm_hdr, atm_hdr, 1)

                //.if (vpi_fld == vpi && vci_fld == vci)
                alu[--, atm_hdr, -, vpi_vci_pti]
            #else
                #error 4 "Macro atm_compare_vpi_vci_pti: invalid value for ENDIAN in atm_compare_vpi_vci_pti macro"
            #endif
        #else
            #error 4 "Macro atm_compare_vpi_vci_pti: invalid value for IN_ATM_HDR_TYPE in atm_compare_vpi_vci_pti macro"
        #endif
    end#:

    .end
#endm


/** This generates the CRC10 checksum for the OAM Cell.
 *
 * This requires a pointer to the location where the OAM Cell is located.
 *
 * @param in_oamCell_ptr   Pointer to OAM Cell
 * @param ENDIAN           Immediate value for endian type either big_endian, little_endian
 *
 * @instruction_cnt 960
 *
 * Outputs: None (Writes the CRC10 checksum in the checksum field.)
 *
 * @b Example:
 * @code
 * crc_load_crc10_table(2048)
 * xbuf_alloc(oam_cell, 12, read_write)
 *
 * immed32(oam_cell[0], 0x18010000)
 * immed32(oam_cell[1], 0x0000ffff)
 * immed32(oam_cell[2], 0xffffffff)
 * immed32(oam_cell[3], 0xffffffff)
 * immed32(oam_cell[4], 0xffffffff)
 * immed32(oam_cell[5], 0xffff0000)
 * immed32(oam_cell[6], 0x00000000)
 * immed32(oam_cell[7], 0x00000000)
 * immed32(oam_cell[8], 0x00000000)
 * immed32(oam_cell[9], 0x00006a6a)
 * immed32(oam_cell[10], 0x6a6a6a6a)
 * immed32(oam_cell[11], 0x6a6a0000)
 *
 * oam_gen_crc10(oam_cell, big_endian)
 * oam_validate_crc10(oam_cell, big_endian)
 *  beq[test14_pass#]
 * @endcode
 *
 */
#macro oam_gen_crc10(in_OamCell_ptr, ENDIAN)
.begin

    .reg residue

    // Initialize the CRC residue register.
    // Note: assuming that the user has already loaded the
    //       CRC10 lookup table.
    crc_write(0)
    nop
    nop
    nop

    _gen_oam_crc10 (residue, in_OamCell_ptr, ENDIAN)

    _GET_XBUF_TYPE(in_OamCell_ptr)
    #define_eval    _EXTRACT_SRC_XBUF_TYPE  _XBUF_TYPE
    #undef _XBUF_TYPE

    #if (_EXTRACT_SRC_XBUF_TYPE == _XBUF_LMEM)
        /////////////LMEM WORK-AROUND////////////////////////////////
        ;copy source in temp registers because we can't increment lmem
        ;pointer and can't do byte_align with lmem index > 7
        .reg crc_tmp

        xbuf_extract(crc_tmp, in_OamCell_ptr, 0, 44, 4)

        #if (streq(ENDIAN, little_endian))
            bytefield_shf_left_insert(crc_tmp, 1000, residue, 24, NO_LOAD_CC)
            bytefield_shf_left_insert(crc_tmp, 0100, residue, 8, NO_LOAD_CC)
        #elif (streq(ENDIAN, big_endian))
            bytefield_insert(crc_tmp, 0011, residue, NO_LOAD_CC)
        #else
            #error 4 "Macro oam_gen_crc10: invalid value for ENDIAN in oam_gen_crc10 macro"
        #endif

        xbuf_insert (in_OamCell_ptr, crc_tmp, 0, 44, 4)

    #else

        #if (streq(ENDIAN, little_endian))
            #if (_EXTRACT_SRC_XBUF_TYPE == _XBUF_GPR)
                bytefield_shf_left_insert(in_OamCell_ptr[11], 1000, residue, 24, NO_LOAD_CC)
                bytefield_shf_left_insert(in_OamCell_ptr[11], 0100, residue, 8, NO_LOAD_CC)
            #else
                .reg tmp
                // In case of xfer regs ld_field reads from read xfer reg and writes
                // into write xfer reg.
                alu[tmp, --, B, in_OamCell_ptr[11]]
                bytefield_shf_left_insert(tmp, 1000, residue, 24, NO_LOAD_CC)
                bytefield_shf_left_insert(tmp, 0100, residue, 8, NO_LOAD_CC)
                alu[in_OamCell_ptr[11], --, B, tmp]
            #endif
        #elif (streq(ENDIAN, big_endian))
            bytefield_insert(in_OamCell_ptr[11], 0011, residue, NO_LOAD_CC)
        #else
            #error 4 "Macro oam_gen_crc10: invalid value for ENDIAN in oam_gen_crc10 macro"
        #endif

    #endif

.end
#endm


/** This validates the CRC10 checksum for the OAM Cell.
 *
 * This requires a pointer to the location where the OAM Cell is located.
 *
 * @param in_oamCell_ptr    Pointer to OAM Cell
 * @param ENDIAN            Immediate value for endian type either big_endian, little_endian
 *
 * @condition_codes zero flag set if valid checksum.
 *
 * @instruction_cnt 960
 *
 * @b Example:
 * @code
 * crc_load_crc10_table(2048)
 * xbuf_alloc(oam_cell, 12, read_write)
 *
 * immed32(oam_cell[0], 0x18010000)
 * immed32(oam_cell[1], 0x0000ffff)
 * immed32(oam_cell[2], 0xffffffff)
 * immed32(oam_cell[3], 0xffffffff)
 * immed32(oam_cell[4], 0xffffffff)
 * immed32(oam_cell[5], 0xffff0000)
 * immed32(oam_cell[6], 0x00000000)
 * immed32(oam_cell[7], 0x00000000)
 * immed32(oam_cell[8], 0x00000000)
 * immed32(oam_cell[9], 0x00006a6a)
 * immed32(oam_cell[10], 0x6a6a6a6a)
 * immed32(oam_cell[11], 0x6a6a0000)
 *
 * oam_gen_crc10(oam_cell, big_endian)
 * oam_validate_crc10(oam_cell, big_endian)
 * beq[test14_pass#]
 * @endcode
 *
 */
#macro oam_validate_crc10(in_OamCell_ptr, ENDIAN)
.begin

    .reg residue

    // Initialize the CRC residue register.
    // Note: assuming that the user has already loaded the
    //       CRC10 lookup table.
    crc_write(0)
    nop
    nop
    nop

    _gen_oam_crc10 (residue, in_OamCell_ptr, ENDIAN)

    // The CRC10 checksum thus generated should be zero.
    alu[--, residue, -, 0]

.end
#endm


/** @}
 * @}
 */

#endif /* __ATM_UC__ */
