/*
 * Copyright (C) 2013, Netronome Systems, Inc.  All rights reserved.
 *
 * Disclaimer: this file is provided without any warranty
 * as part of an early-access program, and the content is
 * bound to change before the final release.
 */

#ifndef NFP6000_NBI_PC_H
#define NFP6000_NBI_PC_H

/*================================================================*/
#include "nfp_nbi.h"

/* CPP Address Map (CPP_BASE = IMB translation: island + indexes) */

/* CPP_BASE + NFP_NBI_PC + NFP_NBI_PC_* */
#define NFP_NBI_PC                                          (0x300000)

/*----------------------------------------------------------------*/
/* XPB Address Map (XPB_BASE = NFP_XPB_ISLAND(island))          */

#define NFP_NBI_PCX                                         (NBIX_BASE + 0x180000)

/* XPB_BASE + NFP_NBI_PCX_PE + NFP_NBI_PCX_PE_* */
#define NFP_NBI_PCX_PE                                      (NFP_NBI_PCX + 0x00000)
/* XPB_BASE + NFP_NBI_PCX_CHAR + NFP_NBI_PCX_CHAR_* */
#define NFP_NBI_PCX_CHAR                                    (NFP_NBI_PCX + 0x10000)
/* XPB_BASE + NFP_NBI_PCX_POL + NFP_NBI_PCX_POL_* */
#define NFP_NBI_PCX_POL                                     (NFP_NBI_PCX + 0x20000)
/* XPB_BASE + NFP_NBI_PCX_LOCAL_TABLE_ECC + (nfp_ecc.h) */
#define NFP_NBI_PCX_LOCAL_TABLE_ECC                         (NFP_NBI_PCX + 0xd0000)
/* XPB_BASE + NFP_NBI_PCX_SHARED_TABLE_ECC + (nfp_ecc.h) */
#define NFP_NBI_PCX_SHARED_TABLE_ECC                        (NFP_NBI_PCX + 0xe0000)
/* XPB_BASE + NFP_NBI_PCX_PACKET_SRAM_ECC + (nfp_ecc.h) */
#define NFP_NBI_PCX_PACKET_SRAM_ECC                         (NFP_NBI_PCX + 0xf0000)

/*================================================================*/


/* HGID: 60515d2e075a */

#ifndef NFP6000_LONGNAMES

/*----------------------------------------------------------------
  Register: AllLocalSram_NbiPreTableLut8[2048] - Memory contents for a LUT8 table lookup
  Register: LocalSram1_NbiPreTableLut8[2048] - Memory contents for a LUT8 table lookup
  Register: LocalSram2_NbiPreTableLut8[2048] - Memory contents for a LUT8 table lookup
  Register: LocalSram3_NbiPreTableLut8[2048] - Memory contents for a LUT8 table lookup
  Register: LocalSram4_NbiPreTableLut8[2048] - Memory contents for a LUT8 table lookup
  Register: LocalSram5_NbiPreTableLut8[2048] - Memory contents for a LUT8 table lookup
  Register: LocalSram6_NbiPreTableLut8[2048] - Memory contents for a LUT8 table lookup
  Register: LocalSram7_NbiPreTableLut8[2048] - Memory contents for a LUT8 table lookup
  Register: LocalSram8_NbiPreTableLut8[2048] - Memory contents for a LUT8 table lookup
  Register: LocalSram9_NbiPreTableLut8[2048] - Memory contents for a LUT8 table lookup
  Register: LocalSram10_NbiPreTableLut8[2048] - Memory contents for a LUT8 table lookup
  Register: LocalSram11_NbiPreTableLut8[2048] - Memory contents for a LUT8 table lookup
  Register: LocalSram0_NbiPreTableLut8[2048] - Memory contents for a LUT8 table lookup
  Register: AllSharedSram_NbiPreTableLut8[8192] - Memory contents for a LUT8 table lookup
  Register: SharedSram0_NbiPreTableLut8[8192] - Memory contents for a LUT8 table lookup
  Register: SharedSram1_NbiPreTableLut8[8192] - Memory contents for a LUT8 table lookup
  Register: PacketSram_NbiPreTableLut8[32768] - Memory contents for a LUT8 table lookup
    Bits: 127:120 Data15(rw) - Result if offset to data word is 15
    Bits: 119:112 Data14(rw) - Result if offset to data word is 14
    Bits: 111:104 Data13(rw) - Result if offset to data word is 13
    Bits: 103:96 Data12(rw) - Result if offset to data word is 12
    Bits: 95:88 Data11(rw) - Result if offset to data word is 11
    Bits: 87:80 Data10(rw) - Result if offset to data word is 10
    Bits: 79:72 Data9(rw) - Result if offset to data word is 9
    Bits: 71:64 Data8(rw) - Result if offset to data word is 8
    Bits: 63:56 Data7(rw) - Result if offset to data word is 7
    Bits: 55:48 Data6(rw) - Result if offset to data word is 6
    Bits: 47:40 Data5(rw) - Result if offset to data word is 5
    Bits: 39:32 Data4(rw) - Result if offset to data word is 4
    Bits: 31:24 Data3(rw) - Result if offset to data word is 3
    Bits: 23:16 Data2(rw) - Result if offset to data word is 2
    Bits: 15:8  Data1(rw) - Result if offset to data word is 1
    Bits:  7:0  Data0(rw) - Result if offset to data word is 0
*/
#define NFP_NBI_PC_AllLocalSram_NBIPRETABLELUT8(_x)          (0x00000000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram1_NBIPRETABLELUT8(_x)            (0x00004000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram2_NBIPRETABLELUT8(_x)            (0x00008000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram3_NBIPRETABLELUT8(_x)            (0x0000c000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram4_NBIPRETABLELUT8(_x)            (0x00010000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram5_NBIPRETABLELUT8(_x)            (0x00014000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram6_NBIPRETABLELUT8(_x)            (0x00018000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram7_NBIPRETABLELUT8(_x)            (0x0001c000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram8_NBIPRETABLELUT8(_x)            (0x00020000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram9_NBIPRETABLELUT8(_x)            (0x00024000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram10_NBIPRETABLELUT8(_x)           (0x00028000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram11_NBIPRETABLELUT8(_x)           (0x0002c000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram0_NBIPRETABLELUT8(_x)            (0x00030000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_AllSharedSram_NBIPRETABLELUT8(_x)         (0x00080000 + (0x8 * ((_x) & 0x1fff)))
#define NFP_NBI_PC_SharedSram0_NBIPRETABLELUT8(_x)           (0x00090000 + (0x8 * ((_x) & 0x1fff)))
#define NFP_NBI_PC_SharedSram1_NBIPRETABLELUT8(_x)           (0x000b0000 + (0x8 * ((_x) & 0x1fff)))
#define NFP_NBI_PC_PacketSram_NBIPRETABLELUT8(_x)            (0x000c0000 + (0x8 * ((_x) & 0x7fff)))
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLELUT8_DATA15(_x) (((_x) & 0xff) << 120)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLELUT8_DATA15_of(_x) (((_x) >> 120) & 0xff)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLELUT8_DATA14(_x) (((_x) & 0xff) << 112)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLELUT8_DATA14_of(_x) (((_x) >> 112) & 0xff)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLELUT8_DATA13(_x) (((_x) & 0xff) << 104)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLELUT8_DATA13_of(_x) (((_x) >> 104) & 0xff)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLELUT8_DATA12(_x) (((_x) & 0xff) << 96)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLELUT8_DATA12_of(_x) (((_x) >> 96) & 0xff)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLELUT8_DATA11(_x) (((_x) & 0xff) << 88)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLELUT8_DATA11_of(_x) (((_x) >> 88) & 0xff)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLELUT8_DATA10(_x) (((_x) & 0xff) << 80)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLELUT8_DATA10_of(_x) (((_x) >> 80) & 0xff)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLELUT8_DATA9(_x)  (((_x) & 0xff) << 72)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLELUT8_DATA9_of(_x) (((_x) >> 72) & 0xff)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLELUT8_DATA8(_x)  (((_x) & 0xff) << 64)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLELUT8_DATA8_of(_x) (((_x) >> 64) & 0xff)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLELUT8_DATA7(_x)  (((_x) & 0xff) << 56)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLELUT8_DATA7_of(_x) (((_x) >> 56) & 0xff)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLELUT8_DATA6(_x)  (((_x) & 0xff) << 48)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLELUT8_DATA6_of(_x) (((_x) >> 48) & 0xff)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLELUT8_DATA5(_x)  (((_x) & 0xff) << 40)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLELUT8_DATA5_of(_x) (((_x) >> 40) & 0xff)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLELUT8_DATA4(_x)  (((_x) & 0xff) << 32)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLELUT8_DATA4_of(_x) (((_x) >> 32) & 0xff)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLELUT8_DATA3(_x)  (((_x) & 0xff) << 24)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLELUT8_DATA3_of(_x) (((_x) >> 24) & 0xff)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLELUT8_DATA2(_x)  (((_x) & 0xff) << 16)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLELUT8_DATA2_of(_x) (((_x) >> 16) & 0xff)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLELUT8_DATA1(_x)  (((_x) & 0xff) << 8)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLELUT8_DATA1_of(_x) (((_x) >> 8) & 0xff)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLELUT8_DATA0(_x)  (((_x) & 0xff) << 0)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLELUT8_DATA0_of(_x) (((_x) >> 0) & 0xff)
/*----------------------------------------------------------------
  Register: AllLocalSram_NbiPreTableLut16[2048] - Memory contents for a LUT16 table lookup
  Register: LocalSram1_NbiPreTableLut16[2048] - Memory contents for a LUT16 table lookup
  Register: LocalSram2_NbiPreTableLut16[2048] - Memory contents for a LUT16 table lookup
  Register: LocalSram3_NbiPreTableLut16[2048] - Memory contents for a LUT16 table lookup
  Register: LocalSram4_NbiPreTableLut16[2048] - Memory contents for a LUT16 table lookup
  Register: LocalSram5_NbiPreTableLut16[2048] - Memory contents for a LUT16 table lookup
  Register: LocalSram6_NbiPreTableLut16[2048] - Memory contents for a LUT16 table lookup
  Register: LocalSram7_NbiPreTableLut16[2048] - Memory contents for a LUT16 table lookup
  Register: LocalSram8_NbiPreTableLut16[2048] - Memory contents for a LUT16 table lookup
  Register: LocalSram9_NbiPreTableLut16[2048] - Memory contents for a LUT16 table lookup
  Register: LocalSram10_NbiPreTableLut16[2048] - Memory contents for a LUT16 table lookup
  Register: LocalSram11_NbiPreTableLut16[2048] - Memory contents for a LUT16 table lookup
  Register: LocalSram0_NbiPreTableLut16[2048] - Memory contents for a LUT16 table lookup
  Register: AllSharedSram_NbiPreTableLut16[8192] - Memory contents for a LUT16 table lookup
  Register: SharedSram0_NbiPreTableLut16[8192] - Memory contents for a LUT16 table lookup
  Register: SharedSram1_NbiPreTableLut16[8192] - Memory contents for a LUT16 table lookup
  Register: PacketSram_NbiPreTableLut16[32768] - Memory contents for a LUT16 table lookup
    Bits: 127:112 Data7(rw) - Result if offset to data word is 7
    Bits: 111:96 Data6(rw) - Result if offset to data word is 6
    Bits: 95:80 Data5(rw) - Result if offset to data word is 5
    Bits: 79:64 Data4(rw) - Result if offset to data word is 4
    Bits: 63:48 Data3(rw) - Result if offset to data word is 3
    Bits: 47:32 Data2(rw) - Result if offset to data word is 2
    Bits: 31:16 Data1(rw) - Result if offset to data word is 1
    Bits: 15:0  Data0(rw) - Result if offset to data word is 0
*/
#define NFP_NBI_PC_AllLocalSram_NBIPRETABLELUT16(_x)         (0x00000000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram1_NBIPRETABLELUT16(_x)           (0x00004000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram2_NBIPRETABLELUT16(_x)           (0x00008000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram3_NBIPRETABLELUT16(_x)           (0x0000c000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram4_NBIPRETABLELUT16(_x)           (0x00010000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram5_NBIPRETABLELUT16(_x)           (0x00014000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram6_NBIPRETABLELUT16(_x)           (0x00018000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram7_NBIPRETABLELUT16(_x)           (0x0001c000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram8_NBIPRETABLELUT16(_x)           (0x00020000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram9_NBIPRETABLELUT16(_x)           (0x00024000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram10_NBIPRETABLELUT16(_x)          (0x00028000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram11_NBIPRETABLELUT16(_x)          (0x0002c000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram0_NBIPRETABLELUT16(_x)           (0x00030000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_AllSharedSram_NBIPRETABLELUT16(_x)        (0x00080000 + (0x8 * ((_x) & 0x1fff)))
#define NFP_NBI_PC_SharedSram0_NBIPRETABLELUT16(_x)          (0x00090000 + (0x8 * ((_x) & 0x1fff)))
#define NFP_NBI_PC_SharedSram1_NBIPRETABLELUT16(_x)          (0x000b0000 + (0x8 * ((_x) & 0x1fff)))
#define NFP_NBI_PC_PacketSram_NBIPRETABLELUT16(_x)           (0x000c0000 + (0x8 * ((_x) & 0x7fff)))
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLELUT16_DATA7(_x) (((_x) & 0xffff) << 112)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLELUT16_DATA7_of(_x) (((_x) >> 112) & 0xffff)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLELUT16_DATA6(_x) (((_x) & 0xffff) << 96)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLELUT16_DATA6_of(_x) (((_x) >> 96) & 0xffff)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLELUT16_DATA5(_x) (((_x) & 0xffff) << 80)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLELUT16_DATA5_of(_x) (((_x) >> 80) & 0xffff)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLELUT16_DATA4(_x) (((_x) & 0xffff) << 64)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLELUT16_DATA4_of(_x) (((_x) >> 64) & 0xffff)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLELUT16_DATA3(_x) (((_x) & 0xffff) << 48)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLELUT16_DATA3_of(_x) (((_x) >> 48) & 0xffff)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLELUT16_DATA2(_x) (((_x) & 0xffff) << 32)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLELUT16_DATA2_of(_x) (((_x) >> 32) & 0xffff)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLELUT16_DATA1(_x) (((_x) & 0xffff) << 16)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLELUT16_DATA1_of(_x) (((_x) >> 16) & 0xffff)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLELUT16_DATA0(_x) (((_x) & 0xffff) << 0)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLELUT16_DATA0_of(_x) (((_x) >> 0) & 0xffff)
/*----------------------------------------------------------------
  Register: AllLocalSram_NbiPreTableLut32[2048] - Memory contents for a LUT32 table lookup
  Register: LocalSram1_NbiPreTableLut32[2048] - Memory contents for a LUT32 table lookup
  Register: LocalSram2_NbiPreTableLut32[2048] - Memory contents for a LUT32 table lookup
  Register: LocalSram3_NbiPreTableLut32[2048] - Memory contents for a LUT32 table lookup
  Register: LocalSram4_NbiPreTableLut32[2048] - Memory contents for a LUT32 table lookup
  Register: LocalSram5_NbiPreTableLut32[2048] - Memory contents for a LUT32 table lookup
  Register: LocalSram6_NbiPreTableLut32[2048] - Memory contents for a LUT32 table lookup
  Register: LocalSram7_NbiPreTableLut32[2048] - Memory contents for a LUT32 table lookup
  Register: LocalSram8_NbiPreTableLut32[2048] - Memory contents for a LUT32 table lookup
  Register: LocalSram9_NbiPreTableLut32[2048] - Memory contents for a LUT32 table lookup
  Register: LocalSram10_NbiPreTableLut32[2048] - Memory contents for a LUT32 table lookup
  Register: LocalSram11_NbiPreTableLut32[2048] - Memory contents for a LUT32 table lookup
  Register: LocalSram0_NbiPreTableLut32[2048] - Memory contents for a LUT32 table lookup
  Register: AllSharedSram_NbiPreTableLut32[8192] - Memory contents for a LUT32 table lookup
  Register: SharedSram0_NbiPreTableLut32[8192] - Memory contents for a LUT32 table lookup
  Register: SharedSram1_NbiPreTableLut32[8192] - Memory contents for a LUT32 table lookup
  Register: PacketSram_NbiPreTableLut32[32768] - Memory contents for a LUT32 table lookup
    Bits: 127:96 Data3(rw) - Result if offset to data word is 3
    Bits: 95:64 Data2(rw) - Result if offset to data word is 2
    Bits: 63:32 Data1(rw) - Result if offset to data word is 1
    Bits: 31:0  Data0(rw) - Result if offset to data word is 0
*/
#define NFP_NBI_PC_AllLocalSram_NBIPRETABLELUT32(_x)         (0x00000000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram1_NBIPRETABLELUT32(_x)           (0x00004000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram2_NBIPRETABLELUT32(_x)           (0x00008000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram3_NBIPRETABLELUT32(_x)           (0x0000c000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram4_NBIPRETABLELUT32(_x)           (0x00010000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram5_NBIPRETABLELUT32(_x)           (0x00014000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram6_NBIPRETABLELUT32(_x)           (0x00018000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram7_NBIPRETABLELUT32(_x)           (0x0001c000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram8_NBIPRETABLELUT32(_x)           (0x00020000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram9_NBIPRETABLELUT32(_x)           (0x00024000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram10_NBIPRETABLELUT32(_x)          (0x00028000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram11_NBIPRETABLELUT32(_x)          (0x0002c000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram0_NBIPRETABLELUT32(_x)           (0x00030000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_AllSharedSram_NBIPRETABLELUT32(_x)        (0x00080000 + (0x8 * ((_x) & 0x1fff)))
#define NFP_NBI_PC_SharedSram0_NBIPRETABLELUT32(_x)          (0x00090000 + (0x8 * ((_x) & 0x1fff)))
#define NFP_NBI_PC_SharedSram1_NBIPRETABLELUT32(_x)          (0x000b0000 + (0x8 * ((_x) & 0x1fff)))
#define NFP_NBI_PC_PacketSram_NBIPRETABLELUT32(_x)           (0x000c0000 + (0x8 * ((_x) & 0x7fff)))
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLELUT32_DATA3(_x) (((_x) & 0xffffffff) << 96)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLELUT32_DATA3_of(_x) (((_x) >> 96) & 0xffffffff)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLELUT32_DATA2(_x) (((_x) & 0xffffffff) << 64)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLELUT32_DATA2_of(_x) (((_x) >> 64) & 0xffffffff)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLELUT32_DATA1(_x) (((_x) & 0xffffffff) << 32)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLELUT32_DATA1_of(_x) (((_x) >> 32) & 0xffffffff)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLELUT32_DATA0(_x) (((_x) & 0xffffffff) << 0)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLELUT32_DATA0_of(_x) (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: AllLocalSram_NbiPreTableMultibit[2048] - Memory contents for a Multibit table lookup
  Register: LocalSram1_NbiPreTableMultibit[2048] - Memory contents for a Multibit table lookup
  Register: LocalSram2_NbiPreTableMultibit[2048] - Memory contents for a Multibit table lookup
  Register: LocalSram3_NbiPreTableMultibit[2048] - Memory contents for a Multibit table lookup
  Register: LocalSram4_NbiPreTableMultibit[2048] - Memory contents for a Multibit table lookup
  Register: LocalSram5_NbiPreTableMultibit[2048] - Memory contents for a Multibit table lookup
  Register: LocalSram6_NbiPreTableMultibit[2048] - Memory contents for a Multibit table lookup
  Register: LocalSram7_NbiPreTableMultibit[2048] - Memory contents for a Multibit table lookup
  Register: LocalSram8_NbiPreTableMultibit[2048] - Memory contents for a Multibit table lookup
  Register: LocalSram9_NbiPreTableMultibit[2048] - Memory contents for a Multibit table lookup
  Register: LocalSram10_NbiPreTableMultibit[2048] - Memory contents for a Multibit table lookup
  Register: LocalSram11_NbiPreTableMultibit[2048] - Memory contents for a Multibit table lookup
  Register: LocalSram0_NbiPreTableMultibit[2048] - Memory contents for a Multibit table lookup
  Register: AllSharedSram_NbiPreTableMultibit[8192] - Memory contents for a Multibit table lookup
  Register: SharedSram0_NbiPreTableMultibit[8192] - Memory contents for a Multibit table lookup
  Register: SharedSram1_NbiPreTableMultibit[8192] - Memory contents for a Multibit table lookup
  Register: PacketSram_NbiPreTableMultibit[32768] - Memory contents for a Multibit table lookup
    Bits: 121:116 SelA(rw) - Which bit to select for 'A'
    Bits: 115:110 SelB(rw) - Which bit to select for 'B'
    Bits: 109:104 SelC(rw) - Which bit to select for 'C'
    Bits: 103:98 SelD(rw) - Which bit to select for 'D'
    Bits: 97:92 SelE(rw) - Which bit to select for 'E'
    Bits: 91:86 SelF(rw) - Which bit to select for 'F'
    Bits: 85:80 SelG(rw) - Which bit to select for 'G'
    Bits: 79:70 Data7(rw) - Result if bits ABCDEFG matches 1x1xxx1
    Bits: 69:60 Data6(rw) - Result if bits ABCDEFG matches 1x1xxx0
    Bits: 59:50 Data5(rw) - Result if bits ABCDEFG matches 1x0xx1x
    Bits: 49:40 Data4(rw) - Result if bits ABCDEFG matches 1x0xx0x
    Bits: 39:30 Data3(rw) - Result if bits ABCDEFG matches 01xx1xx
    Bits: 29:20 Data2(rw) - Result if bits ABCDEFG matches 01xx0xx
    Bits: 19:10 Data1(rw) - Result if bits ABCDEFG matches 00x1xxx
    Bits:  9:0  Data0(rw) - Result if bits ABCDEFG matches 00x0xxx
*/
#define NFP_NBI_PC_AllLocalSram_NBIPRETABLEMULTIBIT(_x)      (0x00000000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram1_NBIPRETABLEMULTIBIT(_x)        (0x00004000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram2_NBIPRETABLEMULTIBIT(_x)        (0x00008000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram3_NBIPRETABLEMULTIBIT(_x)        (0x0000c000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram4_NBIPRETABLEMULTIBIT(_x)        (0x00010000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram5_NBIPRETABLEMULTIBIT(_x)        (0x00014000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram6_NBIPRETABLEMULTIBIT(_x)        (0x00018000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram7_NBIPRETABLEMULTIBIT(_x)        (0x0001c000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram8_NBIPRETABLEMULTIBIT(_x)        (0x00020000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram9_NBIPRETABLEMULTIBIT(_x)        (0x00024000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram10_NBIPRETABLEMULTIBIT(_x)       (0x00028000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram11_NBIPRETABLEMULTIBIT(_x)       (0x0002c000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram0_NBIPRETABLEMULTIBIT(_x)        (0x00030000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_AllSharedSram_NBIPRETABLEMULTIBIT(_x)     (0x00080000 + (0x8 * ((_x) & 0x1fff)))
#define NFP_NBI_PC_SharedSram0_NBIPRETABLEMULTIBIT(_x)       (0x00090000 + (0x8 * ((_x) & 0x1fff)))
#define NFP_NBI_PC_SharedSram1_NBIPRETABLEMULTIBIT(_x)       (0x000b0000 + (0x8 * ((_x) & 0x1fff)))
#define NFP_NBI_PC_PacketSram_NBIPRETABLEMULTIBIT(_x)        (0x000c0000 + (0x8 * ((_x) & 0x7fff)))
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLEMULTIBIT_SELA(_x) (((_x) & 0x3f) << 116)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLEMULTIBIT_SELA_of(_x) (((_x) >> 116) & 0x3f)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLEMULTIBIT_SELB(_x) (((_x) & 0x3f) << 110)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLEMULTIBIT_SELB_of(_x) (((_x) >> 110) & 0x3f)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLEMULTIBIT_SELC(_x) (((_x) & 0x3f) << 104)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLEMULTIBIT_SELC_of(_x) (((_x) >> 104) & 0x3f)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLEMULTIBIT_SELD(_x) (((_x) & 0x3f) << 98)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLEMULTIBIT_SELD_of(_x) (((_x) >> 98) & 0x3f)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLEMULTIBIT_SELE(_x) (((_x) & 0x3f) << 92)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLEMULTIBIT_SELE_of(_x) (((_x) >> 92) & 0x3f)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLEMULTIBIT_SELF(_x) (((_x) & 0x3f) << 86)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLEMULTIBIT_SELF_of(_x) (((_x) >> 86) & 0x3f)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLEMULTIBIT_SELG(_x) (((_x) & 0x3f) << 80)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLEMULTIBIT_SELG_of(_x) (((_x) >> 80) & 0x3f)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLEMULTIBIT_DATA7(_x) (((_x) & 0x3ff) << 70)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLEMULTIBIT_DATA7_of(_x) (((_x) >> 70) & 0x3ff)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLEMULTIBIT_DATA6(_x) (((_x) & 0x3ff) << 60)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLEMULTIBIT_DATA6_of(_x) (((_x) >> 60) & 0x3ff)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLEMULTIBIT_DATA5(_x) (((_x) & 0x3ff) << 50)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLEMULTIBIT_DATA5_of(_x) (((_x) >> 50) & 0x3ff)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLEMULTIBIT_DATA4(_x) (((_x) & 0x3ff) << 40)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLEMULTIBIT_DATA4_of(_x) (((_x) >> 40) & 0x3ff)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLEMULTIBIT_DATA3(_x) (((_x) & 0x3ff) << 30)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLEMULTIBIT_DATA3_of(_x) (((_x) >> 30) & 0x3ff)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLEMULTIBIT_DATA2(_x) (((_x) & 0x3ff) << 20)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLEMULTIBIT_DATA2_of(_x) (((_x) >> 20) & 0x3ff)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLEMULTIBIT_DATA1(_x) (((_x) & 0x3ff) << 10)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLEMULTIBIT_DATA1_of(_x) (((_x) >> 10) & 0x3ff)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLEMULTIBIT_DATA0(_x) (((_x) & 0x3ff) << 0)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLEMULTIBIT_DATA0_of(_x) (((_x) >> 0) & 0x3ff)
/*----------------------------------------------------------------
  Register: AllLocalSram_NbiPreTableCam8[2048] - Memory contents for a CAM8 table lookup
  Register: LocalSram1_NbiPreTableCam8[2048] - Memory contents for a CAM8 table lookup
  Register: LocalSram2_NbiPreTableCam8[2048] - Memory contents for a CAM8 table lookup
  Register: LocalSram3_NbiPreTableCam8[2048] - Memory contents for a CAM8 table lookup
  Register: LocalSram4_NbiPreTableCam8[2048] - Memory contents for a CAM8 table lookup
  Register: LocalSram5_NbiPreTableCam8[2048] - Memory contents for a CAM8 table lookup
  Register: LocalSram6_NbiPreTableCam8[2048] - Memory contents for a CAM8 table lookup
  Register: LocalSram7_NbiPreTableCam8[2048] - Memory contents for a CAM8 table lookup
  Register: LocalSram8_NbiPreTableCam8[2048] - Memory contents for a CAM8 table lookup
  Register: LocalSram9_NbiPreTableCam8[2048] - Memory contents for a CAM8 table lookup
  Register: LocalSram10_NbiPreTableCam8[2048] - Memory contents for a CAM8 table lookup
  Register: LocalSram11_NbiPreTableCam8[2048] - Memory contents for a CAM8 table lookup
  Register: LocalSram0_NbiPreTableCam8[2048] - Memory contents for a CAM8 table lookup
  Register: AllSharedSram_NbiPreTableCam8[8192] - Memory contents for a CAM8 table lookup
  Register: SharedSram0_NbiPreTableCam8[8192] - Memory contents for a CAM8 table lookup
  Register: SharedSram1_NbiPreTableCam8[8192] - Memory contents for a CAM8 table lookup
  Register: PacketSram_NbiPreTableCam8[32768] - Memory contents for a CAM8 table lookup
    Bits: 127:120 Data15(rw) - Result is 15 if selected data equals this field
    Bits: 119:112 Data14(rw) - Result is 14 if selected data equals this field
    Bits: 111:104 Data13(rw) - Result is 13 if selected data equals this field
    Bits: 103:96 Data12(rw) - Result is 12 if selected data equals this field
    Bits: 95:88 Data11(rw) - Result is 11 if selected data equals this field
    Bits: 87:80 Data10(rw) - Result is 10 if selected data equals this field
    Bits: 79:72 Data9(rw) - Result is 9 if selected data equals this field
    Bits: 71:64 Data8(rw) - Result is 8 if selected data equals this field
    Bits: 63:56 Data7(rw) - Result is 7 if selected data equals this field
    Bits: 55:48 Data6(rw) - Result is 6 if selected data equals this field
    Bits: 47:40 Data5(rw) - Result is 5 if selected data equals this field
    Bits: 39:32 Data4(rw) - Result is 4 if selected data equals this field
    Bits: 31:24 Data3(rw) - Result is 3 if selected data equals this field
    Bits: 23:16 Data2(rw) - Result is 2 if selected data equals this field
    Bits: 15:8  Data1(rw) - Result is 1 if selected data equals this field
    Bits:  7:0  Data0(rw) - Result is 0 if selected data equals this field
*/
#define NFP_NBI_PC_AllLocalSram_NBIPRETABLECAM8(_x)          (0x00000000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram1_NBIPRETABLECAM8(_x)            (0x00004000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram2_NBIPRETABLECAM8(_x)            (0x00008000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram3_NBIPRETABLECAM8(_x)            (0x0000c000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram4_NBIPRETABLECAM8(_x)            (0x00010000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram5_NBIPRETABLECAM8(_x)            (0x00014000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram6_NBIPRETABLECAM8(_x)            (0x00018000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram7_NBIPRETABLECAM8(_x)            (0x0001c000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram8_NBIPRETABLECAM8(_x)            (0x00020000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram9_NBIPRETABLECAM8(_x)            (0x00024000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram10_NBIPRETABLECAM8(_x)           (0x00028000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram11_NBIPRETABLECAM8(_x)           (0x0002c000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram0_NBIPRETABLECAM8(_x)            (0x00030000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_AllSharedSram_NBIPRETABLECAM8(_x)         (0x00080000 + (0x8 * ((_x) & 0x1fff)))
#define NFP_NBI_PC_SharedSram0_NBIPRETABLECAM8(_x)           (0x00090000 + (0x8 * ((_x) & 0x1fff)))
#define NFP_NBI_PC_SharedSram1_NBIPRETABLECAM8(_x)           (0x000b0000 + (0x8 * ((_x) & 0x1fff)))
#define NFP_NBI_PC_PacketSram_NBIPRETABLECAM8(_x)            (0x000c0000 + (0x8 * ((_x) & 0x7fff)))
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLECAM8_DATA15(_x) (((_x) & 0xff) << 120)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLECAM8_DATA15_of(_x) (((_x) >> 120) & 0xff)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLECAM8_DATA14(_x) (((_x) & 0xff) << 112)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLECAM8_DATA14_of(_x) (((_x) >> 112) & 0xff)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLECAM8_DATA13(_x) (((_x) & 0xff) << 104)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLECAM8_DATA13_of(_x) (((_x) >> 104) & 0xff)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLECAM8_DATA12(_x) (((_x) & 0xff) << 96)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLECAM8_DATA12_of(_x) (((_x) >> 96) & 0xff)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLECAM8_DATA11(_x) (((_x) & 0xff) << 88)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLECAM8_DATA11_of(_x) (((_x) >> 88) & 0xff)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLECAM8_DATA10(_x) (((_x) & 0xff) << 80)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLECAM8_DATA10_of(_x) (((_x) >> 80) & 0xff)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLECAM8_DATA9(_x)  (((_x) & 0xff) << 72)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLECAM8_DATA9_of(_x) (((_x) >> 72) & 0xff)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLECAM8_DATA8(_x)  (((_x) & 0xff) << 64)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLECAM8_DATA8_of(_x) (((_x) >> 64) & 0xff)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLECAM8_DATA7(_x)  (((_x) & 0xff) << 56)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLECAM8_DATA7_of(_x) (((_x) >> 56) & 0xff)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLECAM8_DATA6(_x)  (((_x) & 0xff) << 48)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLECAM8_DATA6_of(_x) (((_x) >> 48) & 0xff)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLECAM8_DATA5(_x)  (((_x) & 0xff) << 40)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLECAM8_DATA5_of(_x) (((_x) >> 40) & 0xff)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLECAM8_DATA4(_x)  (((_x) & 0xff) << 32)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLECAM8_DATA4_of(_x) (((_x) >> 32) & 0xff)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLECAM8_DATA3(_x)  (((_x) & 0xff) << 24)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLECAM8_DATA3_of(_x) (((_x) >> 24) & 0xff)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLECAM8_DATA2(_x)  (((_x) & 0xff) << 16)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLECAM8_DATA2_of(_x) (((_x) >> 16) & 0xff)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLECAM8_DATA1(_x)  (((_x) & 0xff) << 8)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLECAM8_DATA1_of(_x) (((_x) >> 8) & 0xff)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLECAM8_DATA0(_x)  (((_x) & 0xff) << 0)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLECAM8_DATA0_of(_x) (((_x) >> 0) & 0xff)
/*----------------------------------------------------------------
  Register: AllLocalSram_NbiPreTableCam16[2048] - Memory contents for a CAM16 table lookup
  Register: LocalSram1_NbiPreTableCam16[2048] - Memory contents for a CAM16 table lookup
  Register: LocalSram2_NbiPreTableCam16[2048] - Memory contents for a CAM16 table lookup
  Register: LocalSram3_NbiPreTableCam16[2048] - Memory contents for a CAM16 table lookup
  Register: LocalSram4_NbiPreTableCam16[2048] - Memory contents for a CAM16 table lookup
  Register: LocalSram5_NbiPreTableCam16[2048] - Memory contents for a CAM16 table lookup
  Register: LocalSram6_NbiPreTableCam16[2048] - Memory contents for a CAM16 table lookup
  Register: LocalSram7_NbiPreTableCam16[2048] - Memory contents for a CAM16 table lookup
  Register: LocalSram8_NbiPreTableCam16[2048] - Memory contents for a CAM16 table lookup
  Register: LocalSram9_NbiPreTableCam16[2048] - Memory contents for a CAM16 table lookup
  Register: LocalSram10_NbiPreTableCam16[2048] - Memory contents for a CAM16 table lookup
  Register: LocalSram11_NbiPreTableCam16[2048] - Memory contents for a CAM16 table lookup
  Register: LocalSram0_NbiPreTableCam16[2048] - Memory contents for a CAM16 table lookup
  Register: AllSharedSram_NbiPreTableCam16[8192] - Memory contents for a CAM16 table lookup
  Register: SharedSram0_NbiPreTableCam16[8192] - Memory contents for a CAM16 table lookup
  Register: SharedSram1_NbiPreTableCam16[8192] - Memory contents for a CAM16 table lookup
  Register: PacketSram_NbiPreTableCam16[32768] - Memory contents for a CAM16 table lookup
    Bits: 127:112 Data7(rw) - Result is 14 if selected data equals this field
    Bits: 111:96 Data6(rw) - Result is 12 if selected data equals this field
    Bits: 95:80 Data5(rw) - Result is 10 if selected data equals this field
    Bits: 79:64 Data4(rw) - Result is 8 if selected data equals this field
    Bits: 63:48 Data3(rw) - Result is 6 if selected data equals this field
    Bits: 47:32 Data2(rw) - Result is 4 if selected data equals this field
    Bits: 31:16 Data1(rw) - Result is 2 if selected data equals this field
    Bits: 15:0  Data0(rw) - Result is 0 if selected data equals this field
*/
#define NFP_NBI_PC_AllLocalSram_NBIPRETABLECAM16(_x)         (0x00000000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram1_NBIPRETABLECAM16(_x)           (0x00004000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram2_NBIPRETABLECAM16(_x)           (0x00008000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram3_NBIPRETABLECAM16(_x)           (0x0000c000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram4_NBIPRETABLECAM16(_x)           (0x00010000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram5_NBIPRETABLECAM16(_x)           (0x00014000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram6_NBIPRETABLECAM16(_x)           (0x00018000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram7_NBIPRETABLECAM16(_x)           (0x0001c000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram8_NBIPRETABLECAM16(_x)           (0x00020000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram9_NBIPRETABLECAM16(_x)           (0x00024000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram10_NBIPRETABLECAM16(_x)          (0x00028000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram11_NBIPRETABLECAM16(_x)          (0x0002c000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram0_NBIPRETABLECAM16(_x)           (0x00030000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_AllSharedSram_NBIPRETABLECAM16(_x)        (0x00080000 + (0x8 * ((_x) & 0x1fff)))
#define NFP_NBI_PC_SharedSram0_NBIPRETABLECAM16(_x)          (0x00090000 + (0x8 * ((_x) & 0x1fff)))
#define NFP_NBI_PC_SharedSram1_NBIPRETABLECAM16(_x)          (0x000b0000 + (0x8 * ((_x) & 0x1fff)))
#define NFP_NBI_PC_PacketSram_NBIPRETABLECAM16(_x)           (0x000c0000 + (0x8 * ((_x) & 0x7fff)))
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLECAM16_DATA7(_x) (((_x) & 0xffff) << 112)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLECAM16_DATA7_of(_x) (((_x) >> 112) & 0xffff)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLECAM16_DATA6(_x) (((_x) & 0xffff) << 96)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLECAM16_DATA6_of(_x) (((_x) >> 96) & 0xffff)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLECAM16_DATA5(_x) (((_x) & 0xffff) << 80)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLECAM16_DATA5_of(_x) (((_x) >> 80) & 0xffff)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLECAM16_DATA4(_x) (((_x) & 0xffff) << 64)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLECAM16_DATA4_of(_x) (((_x) >> 64) & 0xffff)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLECAM16_DATA3(_x) (((_x) & 0xffff) << 48)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLECAM16_DATA3_of(_x) (((_x) >> 48) & 0xffff)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLECAM16_DATA2(_x) (((_x) & 0xffff) << 32)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLECAM16_DATA2_of(_x) (((_x) >> 32) & 0xffff)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLECAM16_DATA1(_x) (((_x) & 0xffff) << 16)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLECAM16_DATA1_of(_x) (((_x) >> 16) & 0xffff)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLECAM16_DATA0(_x) (((_x) & 0xffff) << 0)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLECAM16_DATA0_of(_x) (((_x) >> 0) & 0xffff)
/*----------------------------------------------------------------
  Register: AllLocalSram_NbiPreTableCam24[2048] - Memory contents for a CAM24 table lookup
  Register: LocalSram1_NbiPreTableCam24[2048] - Memory contents for a CAM24 table lookup
  Register: LocalSram2_NbiPreTableCam24[2048] - Memory contents for a CAM24 table lookup
  Register: LocalSram3_NbiPreTableCam24[2048] - Memory contents for a CAM24 table lookup
  Register: LocalSram4_NbiPreTableCam24[2048] - Memory contents for a CAM24 table lookup
  Register: LocalSram5_NbiPreTableCam24[2048] - Memory contents for a CAM24 table lookup
  Register: LocalSram6_NbiPreTableCam24[2048] - Memory contents for a CAM24 table lookup
  Register: LocalSram7_NbiPreTableCam24[2048] - Memory contents for a CAM24 table lookup
  Register: LocalSram8_NbiPreTableCam24[2048] - Memory contents for a CAM24 table lookup
  Register: LocalSram9_NbiPreTableCam24[2048] - Memory contents for a CAM24 table lookup
  Register: LocalSram10_NbiPreTableCam24[2048] - Memory contents for a CAM24 table lookup
  Register: LocalSram11_NbiPreTableCam24[2048] - Memory contents for a CAM24 table lookup
  Register: LocalSram0_NbiPreTableCam24[2048] - Memory contents for a CAM24 table lookup
  Register: AllSharedSram_NbiPreTableCam24[8192] - Memory contents for a CAM24 table lookup
  Register: SharedSram0_NbiPreTableCam24[8192] - Memory contents for a CAM24 table lookup
  Register: SharedSram1_NbiPreTableCam24[8192] - Memory contents for a CAM24 table lookup
  Register: PacketSram_NbiPreTableCam24[32768] - Memory contents for a CAM24 table lookup
    Bits: 119:96 Data3(rw) - Result is 12 if selected data equals this field
    Bits: 87:64 Data2(rw) - Result is 8 if selected data equals this field
    Bits: 55:32 Data1(rw) - Result is 4 if selected data equals this field
    Bits: 23:0  Data0(rw) - Result is 0 if selected data equals this field
*/
#define NFP_NBI_PC_AllLocalSram_NBIPRETABLECAM24(_x)         (0x00000000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram1_NBIPRETABLECAM24(_x)           (0x00004000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram2_NBIPRETABLECAM24(_x)           (0x00008000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram3_NBIPRETABLECAM24(_x)           (0x0000c000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram4_NBIPRETABLECAM24(_x)           (0x00010000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram5_NBIPRETABLECAM24(_x)           (0x00014000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram6_NBIPRETABLECAM24(_x)           (0x00018000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram7_NBIPRETABLECAM24(_x)           (0x0001c000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram8_NBIPRETABLECAM24(_x)           (0x00020000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram9_NBIPRETABLECAM24(_x)           (0x00024000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram10_NBIPRETABLECAM24(_x)          (0x00028000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram11_NBIPRETABLECAM24(_x)          (0x0002c000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram0_NBIPRETABLECAM24(_x)           (0x00030000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_AllSharedSram_NBIPRETABLECAM24(_x)        (0x00080000 + (0x8 * ((_x) & 0x1fff)))
#define NFP_NBI_PC_SharedSram0_NBIPRETABLECAM24(_x)          (0x00090000 + (0x8 * ((_x) & 0x1fff)))
#define NFP_NBI_PC_SharedSram1_NBIPRETABLECAM24(_x)          (0x000b0000 + (0x8 * ((_x) & 0x1fff)))
#define NFP_NBI_PC_PacketSram_NBIPRETABLECAM24(_x)           (0x000c0000 + (0x8 * ((_x) & 0x7fff)))
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLECAM24_DATA3(_x) (((_x) & 0xffffff) << 96)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLECAM24_DATA3_of(_x) (((_x) >> 96) & 0xffffff)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLECAM24_DATA2(_x) (((_x) & 0xffffff) << 64)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLECAM24_DATA2_of(_x) (((_x) >> 64) & 0xffffff)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLECAM24_DATA1(_x) (((_x) & 0xffffff) << 32)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLECAM24_DATA1_of(_x) (((_x) >> 32) & 0xffffff)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLECAM24_DATA0(_x) (((_x) & 0xffffff) << 0)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLECAM24_DATA0_of(_x) (((_x) >> 0) & 0xffffff)
/*----------------------------------------------------------------
  Register: AllLocalSram_NbiPreTableCam32[2048] - Memory contents for a CAM32 table lookup
  Register: LocalSram1_NbiPreTableCam32[2048] - Memory contents for a CAM32 table lookup
  Register: LocalSram2_NbiPreTableCam32[2048] - Memory contents for a CAM32 table lookup
  Register: LocalSram3_NbiPreTableCam32[2048] - Memory contents for a CAM32 table lookup
  Register: LocalSram4_NbiPreTableCam32[2048] - Memory contents for a CAM32 table lookup
  Register: LocalSram5_NbiPreTableCam32[2048] - Memory contents for a CAM32 table lookup
  Register: LocalSram6_NbiPreTableCam32[2048] - Memory contents for a CAM32 table lookup
  Register: LocalSram7_NbiPreTableCam32[2048] - Memory contents for a CAM32 table lookup
  Register: LocalSram8_NbiPreTableCam32[2048] - Memory contents for a CAM32 table lookup
  Register: LocalSram9_NbiPreTableCam32[2048] - Memory contents for a CAM32 table lookup
  Register: LocalSram10_NbiPreTableCam32[2048] - Memory contents for a CAM32 table lookup
  Register: LocalSram11_NbiPreTableCam32[2048] - Memory contents for a CAM32 table lookup
  Register: LocalSram0_NbiPreTableCam32[2048] - Memory contents for a CAM32 table lookup
  Register: AllSharedSram_NbiPreTableCam32[8192] - Memory contents for a CAM32 table lookup
  Register: SharedSram0_NbiPreTableCam32[8192] - Memory contents for a CAM32 table lookup
  Register: SharedSram1_NbiPreTableCam32[8192] - Memory contents for a CAM32 table lookup
  Register: PacketSram_NbiPreTableCam32[32768] - Memory contents for a CAM32 table lookup
    Bits: 127:96 Data3(rw) - Result is 12 if selected data equals this field
    Bits: 95:64 Data2(rw) - Result is 8 if selected data equals this field
    Bits: 63:32 Data1(rw) - Result is 4 if selected data equals this field
    Bits: 31:0  Data0(rw) - Result is 0 if selected data equals this field
*/
#define NFP_NBI_PC_AllLocalSram_NBIPRETABLECAM32(_x)         (0x00000000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram1_NBIPRETABLECAM32(_x)           (0x00004000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram2_NBIPRETABLECAM32(_x)           (0x00008000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram3_NBIPRETABLECAM32(_x)           (0x0000c000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram4_NBIPRETABLECAM32(_x)           (0x00010000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram5_NBIPRETABLECAM32(_x)           (0x00014000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram6_NBIPRETABLECAM32(_x)           (0x00018000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram7_NBIPRETABLECAM32(_x)           (0x0001c000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram8_NBIPRETABLECAM32(_x)           (0x00020000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram9_NBIPRETABLECAM32(_x)           (0x00024000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram10_NBIPRETABLECAM32(_x)          (0x00028000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram11_NBIPRETABLECAM32(_x)          (0x0002c000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram0_NBIPRETABLECAM32(_x)           (0x00030000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_AllSharedSram_NBIPRETABLECAM32(_x)        (0x00080000 + (0x8 * ((_x) & 0x1fff)))
#define NFP_NBI_PC_SharedSram0_NBIPRETABLECAM32(_x)          (0x00090000 + (0x8 * ((_x) & 0x1fff)))
#define NFP_NBI_PC_SharedSram1_NBIPRETABLECAM32(_x)          (0x000b0000 + (0x8 * ((_x) & 0x1fff)))
#define NFP_NBI_PC_PacketSram_NBIPRETABLECAM32(_x)           (0x000c0000 + (0x8 * ((_x) & 0x7fff)))
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLECAM32_DATA3(_x) (((_x) & 0xffffffff) << 96)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLECAM32_DATA3_of(_x) (((_x) >> 96) & 0xffffffff)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLECAM32_DATA2(_x) (((_x) & 0xffffffff) << 64)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLECAM32_DATA2_of(_x) (((_x) >> 64) & 0xffffffff)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLECAM32_DATA1(_x) (((_x) & 0xffffffff) << 32)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLECAM32_DATA1_of(_x) (((_x) >> 32) & 0xffffffff)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLECAM32_DATA0(_x) (((_x) & 0xffffffff) << 0)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLECAM32_DATA0_of(_x) (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: AllLocalSram_NbiPreTableCamR8[2048] - Memory contents for a CAMR8 table lookup
  Register: LocalSram1_NbiPreTableCamR8[2048] - Memory contents for a CAMR8 table lookup
  Register: LocalSram2_NbiPreTableCamR8[2048] - Memory contents for a CAMR8 table lookup
  Register: LocalSram3_NbiPreTableCamR8[2048] - Memory contents for a CAMR8 table lookup
  Register: LocalSram4_NbiPreTableCamR8[2048] - Memory contents for a CAMR8 table lookup
  Register: LocalSram5_NbiPreTableCamR8[2048] - Memory contents for a CAMR8 table lookup
  Register: LocalSram6_NbiPreTableCamR8[2048] - Memory contents for a CAMR8 table lookup
  Register: LocalSram7_NbiPreTableCamR8[2048] - Memory contents for a CAMR8 table lookup
  Register: LocalSram8_NbiPreTableCamR8[2048] - Memory contents for a CAMR8 table lookup
  Register: LocalSram9_NbiPreTableCamR8[2048] - Memory contents for a CAMR8 table lookup
  Register: LocalSram10_NbiPreTableCamR8[2048] - Memory contents for a CAMR8 table lookup
  Register: LocalSram11_NbiPreTableCamR8[2048] - Memory contents for a CAMR8 table lookup
  Register: LocalSram0_NbiPreTableCamR8[2048] - Memory contents for a CAMR8 table lookup
  Register: AllSharedSram_NbiPreTableCamR8[8192] - Memory contents for a CAMR8 table lookup
  Register: SharedSram0_NbiPreTableCamR8[8192] - Memory contents for a CAMR8 table lookup
  Register: SharedSram1_NbiPreTableCamR8[8192] - Memory contents for a CAMR8 table lookup
  Register: PacketSram_NbiPreTableCamR8[32768] - Memory contents for a CAMR8 table lookup
    Bits: 127:120 Result7(rw) - Result if selected data equals Data7
    Bits: 119:112 Data7(rw) - Data to match selected data to for Result7
    Bits: 111:104 Result6(rw) - Result if selected data equals Data6
    Bits: 103:96 Data6(rw) - Data to match selected data to for Result6
    Bits: 95:88 Result5(rw) - Result if selected data equals Data5
    Bits: 87:80 Data5(rw) - Data to match selected data to for Result5
    Bits: 79:72 Result4(rw) - Result if selected data equals Data4
    Bits: 71:64 Data4(rw) - Data to match selected data to for Result4
    Bits: 63:56 Result3(rw) - Result if selected data equals Data3
    Bits: 55:48 Data3(rw) - Data to match selected data to for Result3
    Bits: 47:40 Result2(rw) - Result if selected data equals Data2
    Bits: 39:32 Data2(rw) - Data to match selected data to for Result2
    Bits: 31:24 Result1(rw) - Result if selected data equals Data1
    Bits: 23:16 Data1(rw) - Data to match selected data to for Result1
    Bits: 15:8  Result0(rw) - Result if selected data equals Data0
    Bits:  7:0  Data0(rw) - Data to match selected data to for Result0
*/
#define NFP_NBI_PC_AllLocalSram_NBIPRETABLECAMR8(_x)         (0x00000000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram1_NBIPRETABLECAMR8(_x)           (0x00004000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram2_NBIPRETABLECAMR8(_x)           (0x00008000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram3_NBIPRETABLECAMR8(_x)           (0x0000c000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram4_NBIPRETABLECAMR8(_x)           (0x00010000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram5_NBIPRETABLECAMR8(_x)           (0x00014000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram6_NBIPRETABLECAMR8(_x)           (0x00018000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram7_NBIPRETABLECAMR8(_x)           (0x0001c000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram8_NBIPRETABLECAMR8(_x)           (0x00020000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram9_NBIPRETABLECAMR8(_x)           (0x00024000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram10_NBIPRETABLECAMR8(_x)          (0x00028000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram11_NBIPRETABLECAMR8(_x)          (0x0002c000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram0_NBIPRETABLECAMR8(_x)           (0x00030000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_AllSharedSram_NBIPRETABLECAMR8(_x)        (0x00080000 + (0x8 * ((_x) & 0x1fff)))
#define NFP_NBI_PC_SharedSram0_NBIPRETABLECAMR8(_x)          (0x00090000 + (0x8 * ((_x) & 0x1fff)))
#define NFP_NBI_PC_SharedSram1_NBIPRETABLECAMR8(_x)          (0x000b0000 + (0x8 * ((_x) & 0x1fff)))
#define NFP_NBI_PC_PacketSram_NBIPRETABLECAMR8(_x)           (0x000c0000 + (0x8 * ((_x) & 0x7fff)))
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLECAMR8_RESULT7(_x) (((_x) & 0xff) << 120)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLECAMR8_RESULT7_of(_x) (((_x) >> 120) & 0xff)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLECAMR8_DATA7(_x) (((_x) & 0xff) << 112)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLECAMR8_DATA7_of(_x) (((_x) >> 112) & 0xff)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLECAMR8_RESULT6(_x) (((_x) & 0xff) << 104)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLECAMR8_RESULT6_of(_x) (((_x) >> 104) & 0xff)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLECAMR8_DATA6(_x) (((_x) & 0xff) << 96)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLECAMR8_DATA6_of(_x) (((_x) >> 96) & 0xff)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLECAMR8_RESULT5(_x) (((_x) & 0xff) << 88)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLECAMR8_RESULT5_of(_x) (((_x) >> 88) & 0xff)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLECAMR8_DATA5(_x) (((_x) & 0xff) << 80)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLECAMR8_DATA5_of(_x) (((_x) >> 80) & 0xff)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLECAMR8_RESULT4(_x) (((_x) & 0xff) << 72)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLECAMR8_RESULT4_of(_x) (((_x) >> 72) & 0xff)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLECAMR8_DATA4(_x) (((_x) & 0xff) << 64)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLECAMR8_DATA4_of(_x) (((_x) >> 64) & 0xff)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLECAMR8_RESULT3(_x) (((_x) & 0xff) << 56)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLECAMR8_RESULT3_of(_x) (((_x) >> 56) & 0xff)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLECAMR8_DATA3(_x) (((_x) & 0xff) << 48)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLECAMR8_DATA3_of(_x) (((_x) >> 48) & 0xff)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLECAMR8_RESULT2(_x) (((_x) & 0xff) << 40)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLECAMR8_RESULT2_of(_x) (((_x) >> 40) & 0xff)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLECAMR8_DATA2(_x) (((_x) & 0xff) << 32)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLECAMR8_DATA2_of(_x) (((_x) >> 32) & 0xff)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLECAMR8_RESULT1(_x) (((_x) & 0xff) << 24)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLECAMR8_RESULT1_of(_x) (((_x) >> 24) & 0xff)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLECAMR8_DATA1(_x) (((_x) & 0xff) << 16)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLECAMR8_DATA1_of(_x) (((_x) >> 16) & 0xff)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLECAMR8_RESULT0(_x) (((_x) & 0xff) << 8)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLECAMR8_RESULT0_of(_x) (((_x) >> 8) & 0xff)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLECAMR8_DATA0(_x) (((_x) & 0xff) << 0)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLECAMR8_DATA0_of(_x) (((_x) >> 0) & 0xff)
/*----------------------------------------------------------------
  Register: AllLocalSram_NbiPreTableCamR16[2048] - Memory contents for a CAMR16 table lookup
  Register: LocalSram1_NbiPreTableCamR16[2048] - Memory contents for a CAMR16 table lookup
  Register: LocalSram2_NbiPreTableCamR16[2048] - Memory contents for a CAMR16 table lookup
  Register: LocalSram3_NbiPreTableCamR16[2048] - Memory contents for a CAMR16 table lookup
  Register: LocalSram4_NbiPreTableCamR16[2048] - Memory contents for a CAMR16 table lookup
  Register: LocalSram5_NbiPreTableCamR16[2048] - Memory contents for a CAMR16 table lookup
  Register: LocalSram6_NbiPreTableCamR16[2048] - Memory contents for a CAMR16 table lookup
  Register: LocalSram7_NbiPreTableCamR16[2048] - Memory contents for a CAMR16 table lookup
  Register: LocalSram8_NbiPreTableCamR16[2048] - Memory contents for a CAMR16 table lookup
  Register: LocalSram9_NbiPreTableCamR16[2048] - Memory contents for a CAMR16 table lookup
  Register: LocalSram10_NbiPreTableCamR16[2048] - Memory contents for a CAMR16 table lookup
  Register: LocalSram11_NbiPreTableCamR16[2048] - Memory contents for a CAMR16 table lookup
  Register: LocalSram0_NbiPreTableCamR16[2048] - Memory contents for a CAMR16 table lookup
  Register: AllSharedSram_NbiPreTableCamR16[8192] - Memory contents for a CAMR16 table lookup
  Register: SharedSram0_NbiPreTableCamR16[8192] - Memory contents for a CAMR16 table lookup
  Register: SharedSram1_NbiPreTableCamR16[8192] - Memory contents for a CAMR16 table lookup
  Register: PacketSram_NbiPreTableCamR16[32768] - Memory contents for a CAMR16 table lookup
    Bits: 127:112 Result3(rw) - Result if selected data equals Data3
    Bits: 111:96 Data3(rw) - Data to match selected data to for Result3
    Bits: 95:80 Result2(rw) - Result if selected data equals Data2
    Bits: 79:64 Data2(rw) - Data to match selected data to for Result2
    Bits: 63:48 Result1(rw) - Result if selected data equals Data1
    Bits: 47:32 Data1(rw) - Data to match selected data to for Result1
    Bits: 31:16 Result0(rw) - Result if selected data equals Data0
    Bits: 15:0  Data0(rw) - Data to match selected data to for Result0
*/
#define NFP_NBI_PC_AllLocalSram_NBIPRETABLECAMR16(_x)        (0x00000000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram1_NBIPRETABLECAMR16(_x)          (0x00004000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram2_NBIPRETABLECAMR16(_x)          (0x00008000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram3_NBIPRETABLECAMR16(_x)          (0x0000c000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram4_NBIPRETABLECAMR16(_x)          (0x00010000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram5_NBIPRETABLECAMR16(_x)          (0x00014000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram6_NBIPRETABLECAMR16(_x)          (0x00018000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram7_NBIPRETABLECAMR16(_x)          (0x0001c000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram8_NBIPRETABLECAMR16(_x)          (0x00020000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram9_NBIPRETABLECAMR16(_x)          (0x00024000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram10_NBIPRETABLECAMR16(_x)         (0x00028000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram11_NBIPRETABLECAMR16(_x)         (0x0002c000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram0_NBIPRETABLECAMR16(_x)          (0x00030000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_AllSharedSram_NBIPRETABLECAMR16(_x)       (0x00080000 + (0x8 * ((_x) & 0x1fff)))
#define NFP_NBI_PC_SharedSram0_NBIPRETABLECAMR16(_x)         (0x00090000 + (0x8 * ((_x) & 0x1fff)))
#define NFP_NBI_PC_SharedSram1_NBIPRETABLECAMR16(_x)         (0x000b0000 + (0x8 * ((_x) & 0x1fff)))
#define NFP_NBI_PC_PacketSram_NBIPRETABLECAMR16(_x)          (0x000c0000 + (0x8 * ((_x) & 0x7fff)))
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLECAMR16_RESULT3(_x) (((_x) & 0xffff) << 112)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLECAMR16_RESULT3_of(_x) (((_x) >> 112) & 0xffff)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLECAMR16_DATA3(_x) (((_x) & 0xffff) << 96)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLECAMR16_DATA3_of(_x) (((_x) >> 96) & 0xffff)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLECAMR16_RESULT2(_x) (((_x) & 0xffff) << 80)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLECAMR16_RESULT2_of(_x) (((_x) >> 80) & 0xffff)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLECAMR16_DATA2(_x) (((_x) & 0xffff) << 64)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLECAMR16_DATA2_of(_x) (((_x) >> 64) & 0xffff)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLECAMR16_RESULT1(_x) (((_x) & 0xffff) << 48)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLECAMR16_RESULT1_of(_x) (((_x) >> 48) & 0xffff)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLECAMR16_DATA1(_x) (((_x) & 0xffff) << 32)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLECAMR16_DATA1_of(_x) (((_x) >> 32) & 0xffff)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLECAMR16_RESULT0(_x) (((_x) & 0xffff) << 16)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLECAMR16_RESULT0_of(_x) (((_x) >> 16) & 0xffff)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLECAMR16_DATA0(_x) (((_x) & 0xffff) << 0)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLECAMR16_DATA0_of(_x) (((_x) >> 0) & 0xffff)
/*----------------------------------------------------------------
  Register: AllLocalSram_NbiPreTableCamR24[2048] - Memory contents for a CAMR24 table lookup
  Register: LocalSram1_NbiPreTableCamR24[2048] - Memory contents for a CAMR24 table lookup
  Register: LocalSram2_NbiPreTableCamR24[2048] - Memory contents for a CAMR24 table lookup
  Register: LocalSram3_NbiPreTableCamR24[2048] - Memory contents for a CAMR24 table lookup
  Register: LocalSram4_NbiPreTableCamR24[2048] - Memory contents for a CAMR24 table lookup
  Register: LocalSram5_NbiPreTableCamR24[2048] - Memory contents for a CAMR24 table lookup
  Register: LocalSram6_NbiPreTableCamR24[2048] - Memory contents for a CAMR24 table lookup
  Register: LocalSram7_NbiPreTableCamR24[2048] - Memory contents for a CAMR24 table lookup
  Register: LocalSram8_NbiPreTableCamR24[2048] - Memory contents for a CAMR24 table lookup
  Register: LocalSram9_NbiPreTableCamR24[2048] - Memory contents for a CAMR24 table lookup
  Register: LocalSram10_NbiPreTableCamR24[2048] - Memory contents for a CAMR24 table lookup
  Register: LocalSram11_NbiPreTableCamR24[2048] - Memory contents for a CAMR24 table lookup
  Register: LocalSram0_NbiPreTableCamR24[2048] - Memory contents for a CAMR24 table lookup
  Register: AllSharedSram_NbiPreTableCamR24[8192] - Memory contents for a CAMR24 table lookup
  Register: SharedSram0_NbiPreTableCamR24[8192] - Memory contents for a CAMR24 table lookup
  Register: SharedSram1_NbiPreTableCamR24[8192] - Memory contents for a CAMR24 table lookup
  Register: PacketSram_NbiPreTableCamR24[32768] - Memory contents for a CAMR24 table lookup
    Bits: 125:108 Result2(rw) - Result if selected data equals Data2
    Bits: 107:90 Result1(rw) - Result if selected data equals Data1
    Bits: 89:72 Result0(rw) - Result if selected data equals Data0
    Bits: 71:48 Data2(rw) - Data to match selected data to for Result2
    Bits: 47:24 Data1(rw) - Data to match selected data to for Result1
    Bits: 23:0  Data0(rw) - Data to match selected data to for Result0
*/
#define NFP_NBI_PC_AllLocalSram_NBIPRETABLECAMR24(_x)        (0x00000000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram1_NBIPRETABLECAMR24(_x)          (0x00004000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram2_NBIPRETABLECAMR24(_x)          (0x00008000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram3_NBIPRETABLECAMR24(_x)          (0x0000c000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram4_NBIPRETABLECAMR24(_x)          (0x00010000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram5_NBIPRETABLECAMR24(_x)          (0x00014000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram6_NBIPRETABLECAMR24(_x)          (0x00018000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram7_NBIPRETABLECAMR24(_x)          (0x0001c000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram8_NBIPRETABLECAMR24(_x)          (0x00020000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram9_NBIPRETABLECAMR24(_x)          (0x00024000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram10_NBIPRETABLECAMR24(_x)         (0x00028000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram11_NBIPRETABLECAMR24(_x)         (0x0002c000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram0_NBIPRETABLECAMR24(_x)          (0x00030000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_AllSharedSram_NBIPRETABLECAMR24(_x)       (0x00080000 + (0x8 * ((_x) & 0x1fff)))
#define NFP_NBI_PC_SharedSram0_NBIPRETABLECAMR24(_x)         (0x00090000 + (0x8 * ((_x) & 0x1fff)))
#define NFP_NBI_PC_SharedSram1_NBIPRETABLECAMR24(_x)         (0x000b0000 + (0x8 * ((_x) & 0x1fff)))
#define NFP_NBI_PC_PacketSram_NBIPRETABLECAMR24(_x)          (0x000c0000 + (0x8 * ((_x) & 0x7fff)))
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLECAMR24_RESULT2(_x) (((_x) & 0x3ffff) << 108)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLECAMR24_RESULT2_of(_x) (((_x) >> 108) & 0x3ffff)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLECAMR24_RESULT1(_x) (((_x) & 0x3ffff) << 90)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLECAMR24_RESULT1_of(_x) (((_x) >> 90) & 0x3ffff)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLECAMR24_RESULT0(_x) (((_x) & 0x3ffff) << 72)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLECAMR24_RESULT0_of(_x) (((_x) >> 72) & 0x3ffff)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLECAMR24_DATA2(_x) (((_x) & 0xffffff) << 48)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLECAMR24_DATA2_of(_x) (((_x) >> 48) & 0xffffff)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLECAMR24_DATA1(_x) (((_x) & 0xffffff) << 24)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLECAMR24_DATA1_of(_x) (((_x) >> 24) & 0xffffff)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLECAMR24_DATA0(_x) (((_x) & 0xffffff) << 0)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLECAMR24_DATA0_of(_x) (((_x) >> 0) & 0xffffff)
/*----------------------------------------------------------------
  Register: AllLocalSram_NbiPreTableCamR32[2048] - Memory contents for a CAMR32 table lookup
  Register: LocalSram1_NbiPreTableCamR32[2048] - Memory contents for a CAMR32 table lookup
  Register: LocalSram2_NbiPreTableCamR32[2048] - Memory contents for a CAMR32 table lookup
  Register: LocalSram3_NbiPreTableCamR32[2048] - Memory contents for a CAMR32 table lookup
  Register: LocalSram4_NbiPreTableCamR32[2048] - Memory contents for a CAMR32 table lookup
  Register: LocalSram5_NbiPreTableCamR32[2048] - Memory contents for a CAMR32 table lookup
  Register: LocalSram6_NbiPreTableCamR32[2048] - Memory contents for a CAMR32 table lookup
  Register: LocalSram7_NbiPreTableCamR32[2048] - Memory contents for a CAMR32 table lookup
  Register: LocalSram8_NbiPreTableCamR32[2048] - Memory contents for a CAMR32 table lookup
  Register: LocalSram9_NbiPreTableCamR32[2048] - Memory contents for a CAMR32 table lookup
  Register: LocalSram10_NbiPreTableCamR32[2048] - Memory contents for a CAMR32 table lookup
  Register: LocalSram11_NbiPreTableCamR32[2048] - Memory contents for a CAMR32 table lookup
  Register: LocalSram0_NbiPreTableCamR32[2048] - Memory contents for a CAMR32 table lookup
  Register: AllSharedSram_NbiPreTableCamR32[8192] - Memory contents for a CAMR32 table lookup
  Register: SharedSram0_NbiPreTableCamR32[8192] - Memory contents for a CAMR32 table lookup
  Register: SharedSram1_NbiPreTableCamR32[8192] - Memory contents for a CAMR32 table lookup
  Register: PacketSram_NbiPreTableCamR32[32768] - Memory contents for a CAMR32 table lookup
    Bits: 127:96 Result1(rw) - Result if selected data equals Data1
    Bits: 95:64 Result0(rw) - Result if selected data equals Data0
    Bits: 63:32 Data1(rw) - Data to match selected data to for Result1
    Bits: 31:0  Data0(rw) - Data to match selected data to for Result0
*/
#define NFP_NBI_PC_AllLocalSram_NBIPRETABLECAMR32(_x)        (0x00000000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram1_NBIPRETABLECAMR32(_x)          (0x00004000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram2_NBIPRETABLECAMR32(_x)          (0x00008000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram3_NBIPRETABLECAMR32(_x)          (0x0000c000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram4_NBIPRETABLECAMR32(_x)          (0x00010000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram5_NBIPRETABLECAMR32(_x)          (0x00014000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram6_NBIPRETABLECAMR32(_x)          (0x00018000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram7_NBIPRETABLECAMR32(_x)          (0x0001c000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram8_NBIPRETABLECAMR32(_x)          (0x00020000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram9_NBIPRETABLECAMR32(_x)          (0x00024000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram10_NBIPRETABLECAMR32(_x)         (0x00028000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram11_NBIPRETABLECAMR32(_x)         (0x0002c000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram0_NBIPRETABLECAMR32(_x)          (0x00030000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_AllSharedSram_NBIPRETABLECAMR32(_x)       (0x00080000 + (0x8 * ((_x) & 0x1fff)))
#define NFP_NBI_PC_SharedSram0_NBIPRETABLECAMR32(_x)         (0x00090000 + (0x8 * ((_x) & 0x1fff)))
#define NFP_NBI_PC_SharedSram1_NBIPRETABLECAMR32(_x)         (0x000b0000 + (0x8 * ((_x) & 0x1fff)))
#define NFP_NBI_PC_PacketSram_NBIPRETABLECAMR32(_x)          (0x000c0000 + (0x8 * ((_x) & 0x7fff)))
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLECAMR32_RESULT1(_x) (((_x) & 0xffffffff) << 96)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLECAMR32_RESULT1_of(_x) (((_x) >> 96) & 0xffffffff)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLECAMR32_RESULT0(_x) (((_x) & 0xffffffff) << 64)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLECAMR32_RESULT0_of(_x) (((_x) >> 64) & 0xffffffff)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLECAMR32_DATA1(_x) (((_x) & 0xffffffff) << 32)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLECAMR32_DATA1_of(_x) (((_x) >> 32) & 0xffffffff)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLECAMR32_DATA0(_x) (((_x) & 0xffffffff) << 0)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLECAMR32_DATA0_of(_x) (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: AllLocalSram_NbiPreTableTCamR8[2048] - Memory contents for a TCAMR8 table lookup
  Register: LocalSram1_NbiPreTableTCamR8[2048] - Memory contents for a TCAMR8 table lookup
  Register: LocalSram2_NbiPreTableTCamR8[2048] - Memory contents for a TCAMR8 table lookup
  Register: LocalSram3_NbiPreTableTCamR8[2048] - Memory contents for a TCAMR8 table lookup
  Register: LocalSram4_NbiPreTableTCamR8[2048] - Memory contents for a TCAMR8 table lookup
  Register: LocalSram5_NbiPreTableTCamR8[2048] - Memory contents for a TCAMR8 table lookup
  Register: LocalSram6_NbiPreTableTCamR8[2048] - Memory contents for a TCAMR8 table lookup
  Register: LocalSram7_NbiPreTableTCamR8[2048] - Memory contents for a TCAMR8 table lookup
  Register: LocalSram8_NbiPreTableTCamR8[2048] - Memory contents for a TCAMR8 table lookup
  Register: LocalSram9_NbiPreTableTCamR8[2048] - Memory contents for a TCAMR8 table lookup
  Register: LocalSram10_NbiPreTableTCamR8[2048] - Memory contents for a TCAMR8 table lookup
  Register: LocalSram11_NbiPreTableTCamR8[2048] - Memory contents for a TCAMR8 table lookup
  Register: LocalSram0_NbiPreTableTCamR8[2048] - Memory contents for a TCAMR8 table lookup
  Register: AllSharedSram_NbiPreTableTCamR8[8192] - Memory contents for a TCAMR8 table lookup
  Register: SharedSram0_NbiPreTableTCamR8[8192] - Memory contents for a TCAMR8 table lookup
  Register: SharedSram1_NbiPreTableTCamR8[8192] - Memory contents for a TCAMR8 table lookup
  Register: PacketSram_NbiPreTableTCamR8[32768] - Memory contents for a TCAMR8 table lookup
    Bits: 127:112 Result3(rw) - Result if selected data AND Mask3 equals Value3 (if (Value3 and (~Mask3))!=0 then also at least one bit of Data and (Value3 and (~Mask3)) must be set)
    Bits: 111:104 Mask3(rw) - Mask for Value3 and Data3
    Bits: 103:96 Value3(rw) - Value to match with Mask3 and Data3 to provide for Result3
    Bits: 95:80 Result2(rw) - Result if selected data AND Mask2 equals Value2 (if (Value2 and (~Mask2))!=0 then also at least one bit of Data and (Value2 and (~Mask2)) must be set)
    Bits: 79:72 Mask2(rw) - Mask for Value2 and Data2
    Bits: 71:64 Value2(rw) - Value to match with Mask2 and Data2 to provide for Result2
    Bits: 63:48 Result1(rw) - Result if selected data AND Mask1 equals Value1 (if (Value1 and (~Mask1))!=0 then also at least one bit of Data and (Value1 and (~Mask1)) must be set)
    Bits: 47:40 Mask1(rw) - Mask for Value1 and Data1
    Bits: 39:32 Value1(rw) - Value to match with Mask1 and Data1 to provide for Result1
    Bits: 31:16 Result0(rw) - Result if selected data AND Mask0 equals Value0 (if (Value0 and (~Mask0))!=0 then also at least one bit of Data and (Value0 and (~Mask0)) must be set)
    Bits: 15:8  Mask0(rw) - Mask for Value0 and Data0
    Bits:  7:0  Value0(rw) - Value to match with Mask0 and Data0 to provide for Result0
*/
#define NFP_NBI_PC_AllLocalSram_NBIPRETABLETCAMR8(_x)        (0x00000000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram1_NBIPRETABLETCAMR8(_x)          (0x00004000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram2_NBIPRETABLETCAMR8(_x)          (0x00008000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram3_NBIPRETABLETCAMR8(_x)          (0x0000c000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram4_NBIPRETABLETCAMR8(_x)          (0x00010000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram5_NBIPRETABLETCAMR8(_x)          (0x00014000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram6_NBIPRETABLETCAMR8(_x)          (0x00018000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram7_NBIPRETABLETCAMR8(_x)          (0x0001c000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram8_NBIPRETABLETCAMR8(_x)          (0x00020000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram9_NBIPRETABLETCAMR8(_x)          (0x00024000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram10_NBIPRETABLETCAMR8(_x)         (0x00028000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram11_NBIPRETABLETCAMR8(_x)         (0x0002c000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram0_NBIPRETABLETCAMR8(_x)          (0x00030000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_AllSharedSram_NBIPRETABLETCAMR8(_x)       (0x00080000 + (0x8 * ((_x) & 0x1fff)))
#define NFP_NBI_PC_SharedSram0_NBIPRETABLETCAMR8(_x)         (0x00090000 + (0x8 * ((_x) & 0x1fff)))
#define NFP_NBI_PC_SharedSram1_NBIPRETABLETCAMR8(_x)         (0x000b0000 + (0x8 * ((_x) & 0x1fff)))
#define NFP_NBI_PC_PacketSram_NBIPRETABLETCAMR8(_x)          (0x000c0000 + (0x8 * ((_x) & 0x7fff)))
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLETCAMR8_RESULT3(_x) (((_x) & 0xffff) << 112)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLETCAMR8_RESULT3_of(_x) (((_x) >> 112) & 0xffff)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLETCAMR8_MASK3(_x) (((_x) & 0xff) << 104)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLETCAMR8_MASK3_of(_x) (((_x) >> 104) & 0xff)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLETCAMR8_VALUE3(_x) (((_x) & 0xff) << 96)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLETCAMR8_VALUE3_of(_x) (((_x) >> 96) & 0xff)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLETCAMR8_RESULT2(_x) (((_x) & 0xffff) << 80)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLETCAMR8_RESULT2_of(_x) (((_x) >> 80) & 0xffff)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLETCAMR8_MASK2(_x) (((_x) & 0xff) << 72)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLETCAMR8_MASK2_of(_x) (((_x) >> 72) & 0xff)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLETCAMR8_VALUE2(_x) (((_x) & 0xff) << 64)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLETCAMR8_VALUE2_of(_x) (((_x) >> 64) & 0xff)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLETCAMR8_RESULT1(_x) (((_x) & 0xffff) << 48)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLETCAMR8_RESULT1_of(_x) (((_x) >> 48) & 0xffff)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLETCAMR8_MASK1(_x) (((_x) & 0xff) << 40)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLETCAMR8_MASK1_of(_x) (((_x) >> 40) & 0xff)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLETCAMR8_VALUE1(_x) (((_x) & 0xff) << 32)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLETCAMR8_VALUE1_of(_x) (((_x) >> 32) & 0xff)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLETCAMR8_RESULT0(_x) (((_x) & 0xffff) << 16)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLETCAMR8_RESULT0_of(_x) (((_x) >> 16) & 0xffff)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLETCAMR8_MASK0(_x) (((_x) & 0xff) << 8)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLETCAMR8_MASK0_of(_x) (((_x) >> 8) & 0xff)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLETCAMR8_VALUE0(_x) (((_x) & 0xff) << 0)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLETCAMR8_VALUE0_of(_x) (((_x) >> 0) & 0xff)
/*----------------------------------------------------------------
  Register: AllLocalSram_NbiPreTableTCamR16[2048] - Memory contents for a TCAMR16 table lookup
  Register: LocalSram1_NbiPreTableTCamR16[2048] - Memory contents for a TCAMR16 table lookup
  Register: LocalSram2_NbiPreTableTCamR16[2048] - Memory contents for a TCAMR16 table lookup
  Register: LocalSram3_NbiPreTableTCamR16[2048] - Memory contents for a TCAMR16 table lookup
  Register: LocalSram4_NbiPreTableTCamR16[2048] - Memory contents for a TCAMR16 table lookup
  Register: LocalSram5_NbiPreTableTCamR16[2048] - Memory contents for a TCAMR16 table lookup
  Register: LocalSram6_NbiPreTableTCamR16[2048] - Memory contents for a TCAMR16 table lookup
  Register: LocalSram7_NbiPreTableTCamR16[2048] - Memory contents for a TCAMR16 table lookup
  Register: LocalSram8_NbiPreTableTCamR16[2048] - Memory contents for a TCAMR16 table lookup
  Register: LocalSram9_NbiPreTableTCamR16[2048] - Memory contents for a TCAMR16 table lookup
  Register: LocalSram10_NbiPreTableTCamR16[2048] - Memory contents for a TCAMR16 table lookup
  Register: LocalSram11_NbiPreTableTCamR16[2048] - Memory contents for a TCAMR16 table lookup
  Register: LocalSram0_NbiPreTableTCamR16[2048] - Memory contents for a TCAMR16 table lookup
  Register: AllSharedSram_NbiPreTableTCamR16[8192] - Memory contents for a TCAMR16 table lookup
  Register: SharedSram0_NbiPreTableTCamR16[8192] - Memory contents for a TCAMR16 table lookup
  Register: SharedSram1_NbiPreTableTCamR16[8192] - Memory contents for a TCAMR16 table lookup
  Register: PacketSram_NbiPreTableTCamR16[32768] - Memory contents for a TCAMR16 table lookup
    Bits: 127:96 Result1(rw) - Result if selected data AND Mask1 equals Value1 (if (Value1 and (~Mask1))!=0 then also at least one bit of Data and (Value1 and (~Mask1)) must be set)
    Bits: 95:80 Mask1(rw) - Mask for Value1 and Data1
    Bits: 79:64 Value1(rw) - Value to match with Mask1 and Data1 to provide for Result1
    Bits: 63:32 Result0(rw) - Result if selected data AND Mask0 equals Value0 (if (Value0 and (~Mask0))!=0 then also at least one bit of Data and (Value0 and (~Mask0)) must be set)
    Bits: 31:16 Mask0(rw) - Mask for Value0 and Data0
    Bits: 15:0  Value0(rw) - Value to match with Mask0 and Data0 to provide for Result0
*/
#define NFP_NBI_PC_AllLocalSram_NBIPRETABLETCAMR16(_x)       (0x00000000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram1_NBIPRETABLETCAMR16(_x)         (0x00004000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram2_NBIPRETABLETCAMR16(_x)         (0x00008000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram3_NBIPRETABLETCAMR16(_x)         (0x0000c000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram4_NBIPRETABLETCAMR16(_x)         (0x00010000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram5_NBIPRETABLETCAMR16(_x)         (0x00014000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram6_NBIPRETABLETCAMR16(_x)         (0x00018000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram7_NBIPRETABLETCAMR16(_x)         (0x0001c000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram8_NBIPRETABLETCAMR16(_x)         (0x00020000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram9_NBIPRETABLETCAMR16(_x)         (0x00024000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram10_NBIPRETABLETCAMR16(_x)        (0x00028000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram11_NBIPRETABLETCAMR16(_x)        (0x0002c000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram0_NBIPRETABLETCAMR16(_x)         (0x00030000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_AllSharedSram_NBIPRETABLETCAMR16(_x)      (0x00080000 + (0x8 * ((_x) & 0x1fff)))
#define NFP_NBI_PC_SharedSram0_NBIPRETABLETCAMR16(_x)        (0x00090000 + (0x8 * ((_x) & 0x1fff)))
#define NFP_NBI_PC_SharedSram1_NBIPRETABLETCAMR16(_x)        (0x000b0000 + (0x8 * ((_x) & 0x1fff)))
#define NFP_NBI_PC_PacketSram_NBIPRETABLETCAMR16(_x)         (0x000c0000 + (0x8 * ((_x) & 0x7fff)))
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLETCAMR16_RESULT1(_x) (((_x) & 0xffffffff) << 96)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLETCAMR16_RESULT1_of(_x) (((_x) >> 96) & 0xffffffff)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLETCAMR16_MASK1(_x) (((_x) & 0xffff) << 80)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLETCAMR16_MASK1_of(_x) (((_x) >> 80) & 0xffff)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLETCAMR16_VALUE1(_x) (((_x) & 0xffff) << 64)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLETCAMR16_VALUE1_of(_x) (((_x) >> 64) & 0xffff)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLETCAMR16_RESULT0(_x) (((_x) & 0xffffffff) << 32)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLETCAMR16_RESULT0_of(_x) (((_x) >> 32) & 0xffffffff)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLETCAMR16_MASK0(_x) (((_x) & 0xffff) << 16)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLETCAMR16_MASK0_of(_x) (((_x) >> 16) & 0xffff)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLETCAMR16_VALUE0(_x) (((_x) & 0xffff) << 0)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLETCAMR16_VALUE0_of(_x) (((_x) >> 0) & 0xffff)
/*----------------------------------------------------------------
  Register: AllLocalSram_NbiPreTableTCamR24[2048] - Memory contents for a TCAMR24 table lookup
  Register: LocalSram1_NbiPreTableTCamR24[2048] - Memory contents for a TCAMR24 table lookup
  Register: LocalSram2_NbiPreTableTCamR24[2048] - Memory contents for a TCAMR24 table lookup
  Register: LocalSram3_NbiPreTableTCamR24[2048] - Memory contents for a TCAMR24 table lookup
  Register: LocalSram4_NbiPreTableTCamR24[2048] - Memory contents for a TCAMR24 table lookup
  Register: LocalSram5_NbiPreTableTCamR24[2048] - Memory contents for a TCAMR24 table lookup
  Register: LocalSram6_NbiPreTableTCamR24[2048] - Memory contents for a TCAMR24 table lookup
  Register: LocalSram7_NbiPreTableTCamR24[2048] - Memory contents for a TCAMR24 table lookup
  Register: LocalSram8_NbiPreTableTCamR24[2048] - Memory contents for a TCAMR24 table lookup
  Register: LocalSram9_NbiPreTableTCamR24[2048] - Memory contents for a TCAMR24 table lookup
  Register: LocalSram10_NbiPreTableTCamR24[2048] - Memory contents for a TCAMR24 table lookup
  Register: LocalSram11_NbiPreTableTCamR24[2048] - Memory contents for a TCAMR24 table lookup
  Register: LocalSram0_NbiPreTableTCamR24[2048] - Memory contents for a TCAMR24 table lookup
  Register: AllSharedSram_NbiPreTableTCamR24[8192] - Memory contents for a TCAMR24 table lookup
  Register: SharedSram0_NbiPreTableTCamR24[8192] - Memory contents for a TCAMR24 table lookup
  Register: SharedSram1_NbiPreTableTCamR24[8192] - Memory contents for a TCAMR24 table lookup
  Register: PacketSram_NbiPreTableTCamR24[32768] - Memory contents for a TCAMR24 table lookup
    Bits: 127:112 Result1(rw) - Result if selected data AND Mask1 equals Value1 (if (Value1 and (~Mask1))!=0 then also at least one bit of Data and (Value1 and (~Mask1)) must be set)
    Bits: 111:88 Mask1(rw) - Mask for Value1 and Data1
    Bits: 87:64 Value1(rw) - Value to match with Mask1 and Data1 to provide for Result1
    Bits: 63:48 Result0(rw) - Result if selected data AND Mask0 equals Value0 (if (Value0 and (~Mask0))!=0 then also at least one bit of Data and (Value0 and (~Mask0)) must be set)
    Bits: 47:24 Mask0(rw) - Mask for Value0 and Data0
    Bits: 23:0  Value0(rw) - Value to match with Mask0 and Data0 to provide for Result0
*/
#define NFP_NBI_PC_AllLocalSram_NBIPRETABLETCAMR24(_x)       (0x00000000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram1_NBIPRETABLETCAMR24(_x)         (0x00004000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram2_NBIPRETABLETCAMR24(_x)         (0x00008000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram3_NBIPRETABLETCAMR24(_x)         (0x0000c000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram4_NBIPRETABLETCAMR24(_x)         (0x00010000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram5_NBIPRETABLETCAMR24(_x)         (0x00014000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram6_NBIPRETABLETCAMR24(_x)         (0x00018000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram7_NBIPRETABLETCAMR24(_x)         (0x0001c000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram8_NBIPRETABLETCAMR24(_x)         (0x00020000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram9_NBIPRETABLETCAMR24(_x)         (0x00024000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram10_NBIPRETABLETCAMR24(_x)        (0x00028000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram11_NBIPRETABLETCAMR24(_x)        (0x0002c000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram0_NBIPRETABLETCAMR24(_x)         (0x00030000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_AllSharedSram_NBIPRETABLETCAMR24(_x)      (0x00080000 + (0x8 * ((_x) & 0x1fff)))
#define NFP_NBI_PC_SharedSram0_NBIPRETABLETCAMR24(_x)        (0x00090000 + (0x8 * ((_x) & 0x1fff)))
#define NFP_NBI_PC_SharedSram1_NBIPRETABLETCAMR24(_x)        (0x000b0000 + (0x8 * ((_x) & 0x1fff)))
#define NFP_NBI_PC_PacketSram_NBIPRETABLETCAMR24(_x)         (0x000c0000 + (0x8 * ((_x) & 0x7fff)))
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLETCAMR24_RESULT1(_x) (((_x) & 0xffff) << 112)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLETCAMR24_RESULT1_of(_x) (((_x) >> 112) & 0xffff)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLETCAMR24_MASK1(_x) (((_x) & 0xffffff) << 88)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLETCAMR24_MASK1_of(_x) (((_x) >> 88) & 0xffffff)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLETCAMR24_VALUE1(_x) (((_x) & 0xffffff) << 64)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLETCAMR24_VALUE1_of(_x) (((_x) >> 64) & 0xffffff)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLETCAMR24_RESULT0(_x) (((_x) & 0xffff) << 48)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLETCAMR24_RESULT0_of(_x) (((_x) >> 48) & 0xffff)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLETCAMR24_MASK0(_x) (((_x) & 0xffffff) << 24)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLETCAMR24_MASK0_of(_x) (((_x) >> 24) & 0xffffff)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLETCAMR24_VALUE0(_x) (((_x) & 0xffffff) << 0)
#define   NFP_NBI_PC_ALLLOCALSRAM_NBIPRETABLETCAMR24_VALUE0_of(_x) (((_x) >> 0) & 0xffffff)
/*----------------------------------------------------------------
  Register: PicoengineSetup - Initial state of picoengine CSRs when next allocated
    Bits: 25    HashPremult(rw) - Set the initial value for the picoengine hash preaddition of data to residue on hash generation
    Bits: 24    HashSboxEnable(rw) - Set the initial value for the picoengine hash enabling of SBOXes
    Bits: 23:20 HashSbox(rw) - Set the initial value for the picoengine hash number of SBOXes to use
    Bits: 19:16 HashMult(rw) - Set the initial value for the picoengine hash multiplier (for enable multiply-mod-2 by 4, 36, 53, 63)
    Bits: 11:10 Predicate(rw) - Set to specify amount of packet data prefetch to do; can be changed by the picoengine as it runs
    Bits:  9:8  PktPref(rw) - Set to specify amount of packet data prefetch to do; can be changed by the picoengine as it runs
    Bits:  7:6  SequenceReplace(rw) - Set to replace metadata octets[2..3] or octets[2..5] with sequence number from a pool specified by metadata[1]
    Bits:  5    PktPtrOp(rw) - Initial value for picoengine to use for the default packet pointer operation for pkt ALU operations
    Bits:  4    Fetch256(rw) - Initial code load should be a 256-bit fetch; this is also loaded into the picoengine (BUT NOT USED THERE)
    Bits:  3:0  StackPtr(rw) - Initial value for picoengine execution for the stack pointer; this is used whenever a picoengine program is initiated
*/
#define NFP_NBI_PCX_PE_SETUP                                 0x00000004
#define   NFP_NBI_PCX_PE_SETUP_HASHPREMULT                   BIT(25)
#define   NFP_NBI_PCX_PE_SETUP_HASHSBOXEN                    BIT(24)
#define   NFP_NBI_PCX_PE_SETUP_HASHSBOX(_x)                  (((_x) & 0xf) << 20)
#define   NFP_NBI_PCX_PE_SETUP_HASHSBOX_of(_x)               (((_x) >> 20) & 0xf)
#define   NFP_NBI_PCX_PE_SETUP_HASHMULT(_x)                  (((_x) & 0xf) << 16)
#define   NFP_NBI_PCX_PE_SETUP_HASHMULT_of(_x)               (((_x) >> 16) & 0xf)
#define   NFP_NBI_PCX_PE_SETUP_PRED(_x)                      (((_x) & 0x3) << 10)
#define   NFP_NBI_PCX_PE_SETUP_PRED_of(_x)                   (((_x) >> 10) & 0x3)
#define     NFP_NBI_PCX_PE_SETUP_PRED_ALWAYS                 (0)
#define     NFP_NBI_PCX_PE_SETUP_PRED_ZS                     (1)
#define     NFP_NBI_PCX_PE_SETUP_PRED_CS                     (2)
#define     NFP_NBI_PCX_PE_SETUP_PRED_GT                     (3)
#define   NFP_NBI_PCX_PE_SETUP_PKTPREF(_x)                   (((_x) & 0x3) << 8)
#define   NFP_NBI_PCX_PE_SETUP_PKTPREF_of(_x)                (((_x) >> 8) & 0x3)
#define     NFP_NBI_PCX_PE_SETUP_PKTPREF_NONE                (0)
#define     NFP_NBI_PCX_PE_SETUP_PKTPREF_PREF16              (1)
#define     NFP_NBI_PCX_PE_SETUP_PKTPREF_PREF32              (2)
#define     NFP_NBI_PCX_PE_SETUP_PKTPREF_RESVD               (3)
#define   NFP_NBI_PCX_PE_SETUP_SEQREPL(_x)                   (((_x) & 0x3) << 6)
#define   NFP_NBI_PCX_PE_SETUP_SEQREPL_of(_x)                (((_x) >> 6) & 0x3)
#define     NFP_NBI_PCX_PE_SETUP_SEQREPL_NONE                (0)
#define     NFP_NBI_PCX_PE_SETUP_SEQREPL_SEQ16               (1)
#define     NFP_NBI_PCX_PE_SETUP_SEQREPL_RESVD               (2)
#define     NFP_NBI_PCX_PE_SETUP_SEQREPL_SEQ32               (3)
#define   NFP_NBI_PCX_PE_SETUP_PKTPTROP                      BIT(5)
#define   NFP_NBI_PCX_PE_SETUP_FETCH256                      BIT(4)
#define   NFP_NBI_PCX_PE_SETUP_STACKPTR(_x)                  (((_x) & 0xf) << 0)
#define   NFP_NBI_PCX_PE_SETUP_STACKPTR_of(_x)               (((_x) >> 0) & 0xf)
/*----------------------------------------------------------------
  Register: PicoengineRunControl - Controls execution of the picoengines
    Bits: 29:28 SMem(rw) - One bit per picoengine shared memory to enable the power
    Bits: 27:16 GroupMem(rw) - One bit per picoengine group to enable the power to the memory in those groups
    Bits: 15:4  GroupClock(rw) - One bit per picoengine group to enable the clocks to those groups
    Bits:  2    IgnoreResult(rw) - If set, ignore result of any picoengine completion (i.e. do not pass to tunnel or the NBI DMA on finish)
    Bits:  1:0  Alloc(rw) - Enable allocation of picoengines
*/
#define NFP_NBI_PCX_PE_RUNCONTROL                            0x00000008
#define   NFP_NBI_PCX_PE_RUNCONTROL_SMEM(_x)                 (((_x) & 0x3) << 28)
#define   NFP_NBI_PCX_PE_RUNCONTROL_SMEM_of(_x)              (((_x) >> 28) & 0x3)
#define   NFP_NBI_PCX_PE_RUNCONTROL_GRPMEM(_x)               (((_x) & 0xfff) << 16)
#define   NFP_NBI_PCX_PE_RUNCONTROL_GRPMEM_of(_x)            (((_x) >> 16) & 0xfff)
#define   NFP_NBI_PCX_PE_RUNCONTROL_GRPCLK(_x)               (((_x) & 0xfff) << 4)
#define   NFP_NBI_PCX_PE_RUNCONTROL_GRPCLK_of(_x)            (((_x) >> 4) & 0xfff)
#define   NFP_NBI_PCX_PE_RUNCONTROL_IGNRES                   BIT(2)
#define   NFP_NBI_PCX_PE_RUNCONTROL_ALLOC(_x)                (((_x) & 0x3) << 0)
#define   NFP_NBI_PCX_PE_RUNCONTROL_ALLOC_of(_x)             (((_x) >> 0) & 0x3)
#define     NFP_NBI_PCX_PE_RUNCONTROL_ALLOC_NONE             (0)
#define     NFP_NBI_PCX_PE_RUNCONTROL_ALLOC_ALLOC            (1)
#define     NFP_NBI_PCX_PE_RUNCONTROL_ALLOC_RESVD            (2)
#define     NFP_NBI_PCX_PE_RUNCONTROL_ALLOC_ONESHOT          (3)
/*----------------------------------------------------------------
  Register: TableExtend - Extension of table configuration
    Bits:  0    LookupExt(rw) - Top bit of table lookup, used when performing LMEM or SMEM table configurations through the XPB
*/
#define NFP_NBI_PCX_PE_TBLEXT                                0x0000000c
#define   NFP_NBI_PCX_PE_TBLEXT_LOOKUPEXT                    BIT(0)
/*----------------------------------------------------------------
  Register: ActiveSet0Low - Bit-mask indicating which of the first 32 picoengines are actively classifying (i.e. neither ready with finished classification nor idle) using table set 0
  Register: ActiveSet1Low - Bit-mask indicating which of the first 32 picoengines are actively classifying (i.e. neither ready with finished classification nor idle) using table set 1
    Bits: 31:0  Status(ro) - One bit per picoengine indicating that the picoengine is actively classifying with a particular TableSet value; actively classifying means not idle and not presenting a result
*/
#define NFP_NBI_PCX_PE_ACTSET0L                              0x00000080
#define NFP_NBI_PCX_PE_ACTSET1L                              0x00000088
#define   NFP_NBI_PCX_PE_NBIPREPICOENGACTIVELOW_STATUS(_x)   (((_x) & 0xffffffff) << 0)
#define   NFP_NBI_PCX_PE_NBIPREPICOENGACTIVELOW_STATUS_of(_x) (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: ActiveSet0High - Bit-mask indicating which of the last 16 picoengines are actively classifying (i.e. neither ready with finished classification nor idle) using table set 0
  Register: ActiveSet1High - Bit-mask indicating which of the last 16 picoengines are actively classifying (i.e. neither ready with finished classification nor idle) using table set 1
    Bits: 15:0  Status(ro) - One bit per picoengine indicating that the picoengine is actively classifying with a particular TableSet value; actively classifying means not idle and not presenting a result
*/
#define NFP_NBI_PCX_PE_ACTSET0H                              0x00000084
#define NFP_NBI_PCX_PE_ACTSET1H                              0x0000008c
#define   NFP_NBI_PCX_PE_NBIPREPICOENGACTIVEHIGH_STATUS(_x)  (((_x) & 0xffff) << 0)
#define   NFP_NBI_PCX_PE_NBIPREPICOENGACTIVEHIGH_STATUS_of(_x) (((_x) >> 0) & 0xffff)
/*----------------------------------------------------------------
  Register: ClassifiedSmall - Statistics register for packets of 256 bytes or fewer that have completed classification
    Bits: 31    Saturated(rc) - If set then the number of 'small' packets that have fully completed classification (and passed to DMA) since the last read of this register exceeds 0x7fffffff
    Bits: 30:0  Count(rc) - Count of 'small' packets that have fully completed classification (and passed to DMA) since the last read of this register
*/
#define NFP_NBI_PCX_PE_CLASSSMALL                            0x000000c0
#define   NFP_NBI_PCX_PE_CLASSSMALL_SATURATED                BIT(31)
#define   NFP_NBI_PCX_PE_CLASSSMALL_COUNT(_x)                (((_x) & 0x7fffffff) << 0)
#define   NFP_NBI_PCX_PE_CLASSSMALL_COUNT_of(_x)             (((_x) >> 0) & 0x7fffffff)
/*----------------------------------------------------------------
  Register: ClassifiedLarge - Statistics register for packets of larger than 256 bytes that have completed classification
    Bits: 31    Saturated(rc) - If set then the number of 'large' packets that have fully completed classification (and passed to DMA) since the last read of this register exceeds 0x7fffffff
    Bits: 30:0  Count(rc) - Count of 'large' packets that have fully completed classification (and passed to DMA) since the last read of this register
*/
#define NFP_NBI_PCX_PE_CLASSLARGE                            0x000000c4
#define   NFP_NBI_PCX_PE_CLASSLARGE_SATURATED                BIT(31)
#define   NFP_NBI_PCX_PE_CLASSLARGE_COUNT(_x)                (((_x) & 0x7fffffff) << 0)
#define   NFP_NBI_PCX_PE_CLASSLARGE_COUNT_of(_x)             (((_x) >> 0) & 0x7fffffff)
/*----------------------------------------------------------------
  Register: Tunnel - Statistics register for packets that have been recirculated as tunneled as part of their classification
    Bits: 31    Saturated(rc) - If set then the number of packets passed through the tunnel path since the last read of this register exceeds 0x7fffffff
    Bits: 30:0  Count(rc) - Count of packets passed through the tunnel path since the last read of this register
*/
#define NFP_NBI_PCX_PE_CLASSSTUNN                            0x000000c8
#define   NFP_NBI_PCX_PE_CLASSSTUNN_SATURATED                BIT(31)
#define   NFP_NBI_PCX_PE_CLASSSTUNN_COUNT(_x)                (((_x) & 0x7fffffff) << 0)
#define   NFP_NBI_PCX_PE_CLASSSTUNN_COUNT_of(_x)             (((_x) >> 0) & 0x7fffffff)
/*----------------------------------------------------------------
  Register: LocalData[8] - Local SRAM data table description X
    Bits: 31:22 Base1(wo) - Base address for table set 1 accesses
    Bits: 21:12 Base0(wo) - Base address for table set 0 accesses
    Bits: 11:9  TableSize(wo) - Number of entries in the table - effects masking of input data, and offset into the table
    Bits:  8:4  Select(wo) - Starting bit divide by 2 for determining offset into table for an access
    Bits:  3:0  Lookup(wo) - Method of interpreting the SRAM data read, to produce a result for the lookup
*/
#define NFP_NBI_PCX_PE_LOCALDATA(_x)                         (0x00000100 + (0x4 * ((_x) & 0x7)))
#define   NFP_NBI_PCX_PE_LOCALDATA_BASE1(_x)                 (((_x) & 0x3ff) << 22)
#define   NFP_NBI_PCX_PE_LOCALDATA_BASE1_of(_x)              (((_x) >> 22) & 0x3ff)
#define   NFP_NBI_PCX_PE_LOCALDATA_BASE0(_x)                 (((_x) & 0x3ff) << 12)
#define   NFP_NBI_PCX_PE_LOCALDATA_BASE0_of(_x)              (((_x) >> 12) & 0x3ff)
#define   NFP_NBI_PCX_PE_LOCALDATA_TABLESIZE(_x)             (((_x) & 0x7) << 9)
#define   NFP_NBI_PCX_PE_LOCALDATA_TABLESIZE_of(_x)          (((_x) >> 9) & 0x7)
#define     NFP_NBI_PCX_PE_LOCALDATA_TABLESIZE_1             (0)
#define     NFP_NBI_PCX_PE_LOCALDATA_TABLESIZE_4             (1)
#define     NFP_NBI_PCX_PE_LOCALDATA_TABLESIZE_16            (2)
#define     NFP_NBI_PCX_PE_LOCALDATA_TABLESIZE_64            (3)
#define     NFP_NBI_PCX_PE_LOCALDATA_TABLESIZE_256           (4)
#define     NFP_NBI_PCX_PE_LOCALDATA_TABLESIZE_1K            (5)
#define   NFP_NBI_PCX_PE_LOCALDATA_SELECT(_x)                (((_x) & 0x1f) << 4)
#define   NFP_NBI_PCX_PE_LOCALDATA_SELECT_of(_x)             (((_x) >> 4) & 0x1f)
#define   NFP_NBI_PCX_PE_LOCALDATA_LOOKUP(_x)                (((_x) & 0xf) << 0)
#define   NFP_NBI_PCX_PE_LOCALDATA_LOOKUP_of(_x)             (((_x) >> 0) & 0xf)
#define     NFP_NBI_PCX_PE_LOCALDATA_LOOKUP_LUT8             (0)
#define     NFP_NBI_PCX_PE_LOCALDATA_LOOKUP_LUT16            (1)
#define     NFP_NBI_PCX_PE_LOCALDATA_LOOKUP_LUT32            (2)
#define     NFP_NBI_PCX_PE_LOCALDATA_LOOKUP_MULTIBIT         (3)
#define     NFP_NBI_PCX_PE_LOCALDATA_LOOKUP_CAM32            (4)
#define     NFP_NBI_PCX_PE_LOCALDATA_LOOKUP_CAM24            (5)
#define     NFP_NBI_PCX_PE_LOCALDATA_LOOKUP_CAM16            (6)
#define     NFP_NBI_PCX_PE_LOCALDATA_LOOKUP_CAM8             (7)
#define     NFP_NBI_PCX_PE_LOCALDATA_LOOKUP_CAM32R16         (8)
#define     NFP_NBI_PCX_PE_LOCALDATA_LOOKUP_CAM24R16         (9)
#define     NFP_NBI_PCX_PE_LOCALDATA_LOOKUP_CAM16R16         (10)
#define     NFP_NBI_PCX_PE_LOCALDATA_LOOKUP_CAM8R8           (11)
#define     NFP_NBI_PCX_PE_LOCALDATA_LOOKUP_CAM48R16         (12)
#define     NFP_NBI_PCX_PE_LOCALDATA_LOOKUP_TCAM24R16        (13)
#define     NFP_NBI_PCX_PE_LOCALDATA_LOOKUP_TCAM16R16        (14)
#define     NFP_NBI_PCX_PE_LOCALDATA_LOOKUP_TCAM8R16         (15)
/*----------------------------------------------------------------
  Register: LocalInst[8] - Local SRAM instruction table description X
    Bits: 31:22 Base1(wo) - Base address for table set 1 accesses
    Bits: 21:12 Base0(wo) - Base address for table set 0 accesses
    Bits: 11:9  TableSize(wo) - Number of entries in the table - effects masking of input data, and offset into the table
    Bits:  8:4  Select(wo) - Starting bit divide by 2 for determining offset into table for an access
*/
#define NFP_NBI_PCX_PE_LOCALINST(_x)                         (0x00000120 + (0x4 * ((_x) & 0x7)))
#define   NFP_NBI_PCX_PE_LOCALINST_BASE1(_x)                 (((_x) & 0x3ff) << 22)
#define   NFP_NBI_PCX_PE_LOCALINST_BASE1_of(_x)              (((_x) >> 22) & 0x3ff)
#define   NFP_NBI_PCX_PE_LOCALINST_BASE0(_x)                 (((_x) & 0x3ff) << 12)
#define   NFP_NBI_PCX_PE_LOCALINST_BASE0_of(_x)              (((_x) >> 12) & 0x3ff)
#define   NFP_NBI_PCX_PE_LOCALINST_TABLESIZE(_x)             (((_x) & 0x7) << 9)
#define   NFP_NBI_PCX_PE_LOCALINST_TABLESIZE_of(_x)          (((_x) >> 9) & 0x7)
#define     NFP_NBI_PCX_PE_LOCALINST_TABLESIZE_1             (0)
#define     NFP_NBI_PCX_PE_LOCALINST_TABLESIZE_4             (1)
#define     NFP_NBI_PCX_PE_LOCALINST_TABLESIZE_16            (2)
#define     NFP_NBI_PCX_PE_LOCALINST_TABLESIZE_64            (3)
#define     NFP_NBI_PCX_PE_LOCALINST_TABLESIZE_256           (4)
#define     NFP_NBI_PCX_PE_LOCALINST_TABLESIZE_1K            (5)
#define   NFP_NBI_PCX_PE_LOCALINST_SELECT(_x)                (((_x) & 0x1f) << 4)
#define   NFP_NBI_PCX_PE_LOCALINST_SELECT_of(_x)             (((_x) >> 4) & 0x1f)
/*----------------------------------------------------------------
  Register: SharedData[8] - Shared SRAM data table description X
    Bits: 31:22 Base1(wo) - Base address for table set 1 accesses
    Bits: 21:12 Base0(wo) - Base address for table set 0 accesses
    Bits: 11:9  TableSize(wo) - Number of entries in the table - effects masking of input data, and offset into the table
    Bits:  8:4  Select(wo) - Starting bit divide by 2 for determining offset into table for an access
    Bits:  3:0  Lookup(wo) - Method of interpreting the SRAM data read, to produce a result for the lookup
*/
#define NFP_NBI_PCX_PE_SHAREDDATA(_x)                        (0x00000140 + (0x4 * ((_x) & 0x7)))
#define   NFP_NBI_PCX_PE_SHAREDDATA_BASE1(_x)                (((_x) & 0x3ff) << 22)
#define   NFP_NBI_PCX_PE_SHAREDDATA_BASE1_of(_x)             (((_x) >> 22) & 0x3ff)
#define   NFP_NBI_PCX_PE_SHAREDDATA_BASE0(_x)                (((_x) & 0x3ff) << 12)
#define   NFP_NBI_PCX_PE_SHAREDDATA_BASE0_of(_x)             (((_x) >> 12) & 0x3ff)
#define   NFP_NBI_PCX_PE_SHAREDDATA_TABLESIZE(_x)            (((_x) & 0x7) << 9)
#define   NFP_NBI_PCX_PE_SHAREDDATA_TABLESIZE_of(_x)         (((_x) >> 9) & 0x7)
#define     NFP_NBI_PCX_PE_SHAREDDATA_TABLESIZE_1            (0)
#define     NFP_NBI_PCX_PE_SHAREDDATA_TABLESIZE_4            (1)
#define     NFP_NBI_PCX_PE_SHAREDDATA_TABLESIZE_16           (2)
#define     NFP_NBI_PCX_PE_SHAREDDATA_TABLESIZE_64           (3)
#define     NFP_NBI_PCX_PE_SHAREDDATA_TABLESIZE_256          (4)
#define     NFP_NBI_PCX_PE_SHAREDDATA_TABLESIZE_1K           (5)
#define   NFP_NBI_PCX_PE_SHAREDDATA_SELECT(_x)               (((_x) & 0x1f) << 4)
#define   NFP_NBI_PCX_PE_SHAREDDATA_SELECT_of(_x)            (((_x) >> 4) & 0x1f)
#define   NFP_NBI_PCX_PE_SHAREDDATA_LOOKUP(_x)               (((_x) & 0xf) << 0)
#define   NFP_NBI_PCX_PE_SHAREDDATA_LOOKUP_of(_x)            (((_x) >> 0) & 0xf)
#define     NFP_NBI_PCX_PE_SHAREDDATA_LOOKUP_LUT8            (0)
#define     NFP_NBI_PCX_PE_SHAREDDATA_LOOKUP_LUT16           (1)
#define     NFP_NBI_PCX_PE_SHAREDDATA_LOOKUP_LUT32           (2)
#define     NFP_NBI_PCX_PE_SHAREDDATA_LOOKUP_MULTIBIT        (3)
#define     NFP_NBI_PCX_PE_SHAREDDATA_LOOKUP_CAM32           (4)
#define     NFP_NBI_PCX_PE_SHAREDDATA_LOOKUP_CAM24           (5)
#define     NFP_NBI_PCX_PE_SHAREDDATA_LOOKUP_CAM16           (6)
#define     NFP_NBI_PCX_PE_SHAREDDATA_LOOKUP_CAM8            (7)
#define     NFP_NBI_PCX_PE_SHAREDDATA_LOOKUP_CAMR32          (8)
#define     NFP_NBI_PCX_PE_SHAREDDATA_LOOKUP_CAMR24          (9)
#define     NFP_NBI_PCX_PE_SHAREDDATA_LOOKUP_CAMR16          (10)
#define     NFP_NBI_PCX_PE_SHAREDDATA_LOOKUP_CAMR8           (11)
#define     NFP_NBI_PCX_PE_SHAREDDATA_LOOKUP_CAMR48          (12)
#define     NFP_NBI_PCX_PE_SHAREDDATA_LOOKUP_TCAMR24         (13)
#define     NFP_NBI_PCX_PE_SHAREDDATA_LOOKUP_TCAMR16         (14)
#define     NFP_NBI_PCX_PE_SHAREDDATA_LOOKUP_TCAMR8          (15)
/*----------------------------------------------------------------
  Register: SharedInst[8] - Shared SRAM instruction table description X
    Bits: 31:22 Base1(wo) - Base address for table set 1 accesses
    Bits: 21:12 Base0(wo) - Base address for table set 0 accesses
    Bits: 11:9  TableSize(wo) - Number of entries in the table - effects masking of input data, and offset into the table
    Bits:  8:4  Select(wo) - Starting bit divide by 2 for determining offset into table for an access
*/
#define NFP_NBI_PCX_PE_SHAREDINST(_x)                        (0x00000160 + (0x4 * ((_x) & 0x7)))
#define   NFP_NBI_PCX_PE_SHAREDINST_BASE1(_x)                (((_x) & 0x3ff) << 22)
#define   NFP_NBI_PCX_PE_SHAREDINST_BASE1_of(_x)             (((_x) >> 22) & 0x3ff)
#define   NFP_NBI_PCX_PE_SHAREDINST_BASE0(_x)                (((_x) & 0x3ff) << 12)
#define   NFP_NBI_PCX_PE_SHAREDINST_BASE0_of(_x)             (((_x) >> 12) & 0x3ff)
#define   NFP_NBI_PCX_PE_SHAREDINST_TABLESIZE(_x)            (((_x) & 0x7) << 9)
#define   NFP_NBI_PCX_PE_SHAREDINST_TABLESIZE_of(_x)         (((_x) >> 9) & 0x7)
#define     NFP_NBI_PCX_PE_SHAREDINST_TABLESIZE_1            (0)
#define     NFP_NBI_PCX_PE_SHAREDINST_TABLESIZE_4            (1)
#define     NFP_NBI_PCX_PE_SHAREDINST_TABLESIZE_16           (2)
#define     NFP_NBI_PCX_PE_SHAREDINST_TABLESIZE_64           (3)
#define     NFP_NBI_PCX_PE_SHAREDINST_TABLESIZE_256          (4)
#define     NFP_NBI_PCX_PE_SHAREDINST_TABLESIZE_1K           (5)
#define   NFP_NBI_PCX_PE_SHAREDINST_SELECT(_x)               (((_x) & 0x1f) << 4)
#define   NFP_NBI_PCX_PE_SHAREDINST_SELECT_of(_x)            (((_x) >> 4) & 0x1f)
/*----------------------------------------------------------------
  Register: MulticycleTable[8]Set0 - Multicycle lookup table description X for table set 0
  Register: MulticycleTable[8]Set1 - Multicycle lookup table description X for table set 1
    Bits: 31:0  Table(wo) - 32-bit table value (equivalent to MU lookup engine result) to present to MU lookup engine with the full 64-bit data
*/
#define NFP_NBI_PCX_PE_MULTCYCLESET0(_x)                     (0x00000180 + (0x4 * ((_x) & 0x7)))
#define NFP_NBI_PCX_PE_MULTCYCLESET1(_x)                     (0x000001a0 + (0x4 * ((_x) & 0x7)))
#define   NFP_NBI_PCX_PE_NBIPREMULTICYCLETABLE_TABLE(_x)     (((_x) & 0xffffffff) << 0)
#define   NFP_NBI_PCX_PE_NBIPREMULTICYCLETABLE_TABLE_of(_x)  (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: Config - Configuration of characterization
    Bits: 31:28 MaxDepth(rw) - Maximum depth of characterization for packets on minipacket bus minus one, in units of 16 bytes - a setting of zero is taken to mean 15
    Bits: 27    ExtendInitialSkip(rw) - Set to make the initial skip provided by ports to be four bits, using user_type[1] as the most significant bit; if clear, only a 3-bit initial skip is used
    Bits: 26    DisableInner(rw) - Set to disable inner protocol analysis for any packets that are characterized as IPv4/IPv6/MPLS etc; no further analysis is done beyond that point
    Bits: 25    DisablePPP(rw) - Set to disable PPP analysis for any packets that are characterized as PPP; no further analysis is done beyond that point
    Bits: 24    DisableEthernet(rw) - Set to disable ethernet analysis for any packets that are characterized as Ethernet; no further analysis is done beyond that point
    Bits: 21:20 PPPAddressSkip(rw) - Number of 16-bit octets to skip for PPP packets from start to protocol field
    Bits: 18    GFPControlIgn(rw) - Set to ignore
    Bits: 17    GFPBarker(rw) - Set to enable Barker scrambling on GFP headers
    Bits: 16    GFPExtension(rw) - Set to enable linear extension on GFP-F
    Bits: 11    PBBEnable(rw) - Set to enable PBB detection (0x88a8, X, 0x88e7) in Ethernet packets
    Bits: 10    EnableInitialTag(rw) - Set to enable TCAM0 as an initial tag; the port's user_type[0] bit must also be set for packets on the port to use TCAM0 as an initial tag
    Bits:  9:8  MaxVlans(rw) - Maximum number of VLAN tags permitted, from 0 to 3
    Bits:  7:4  InitialTagSize(rw) - Number of 16-bit words to skip if TCAM0 is used as an initial tag and it matches
    Bits:  3:0  ProtSkip(rw) - Number of 16-bit words to skip after start of Ethernet packet before looking for VLAN tags and other Ethernet Protocol types
*/
#define NFP_NBI_PCX_CHAR_CFG                                 0x00000004
#define   NFP_NBI_PCX_CHAR_CFG_MAXDEPTH(_x)                  (((_x) & 0xf) << 28)
#define   NFP_NBI_PCX_CHAR_CFG_MAXDEPTH_of(_x)               (((_x) >> 28) & 0xf)
#define   NFP_NBI_PCX_CHAR_CFG_EXTENDSKIP                    BIT(27)
#define   NFP_NBI_PCX_CHAR_CFG_INNERDISABLE                  BIT(26)
#define   NFP_NBI_PCX_CHAR_CFG_PPPDISABLE                    BIT(25)
#define   NFP_NBI_PCX_CHAR_CFG_ETHDISABLE                    BIT(24)
#define   NFP_NBI_PCX_CHAR_CFG_PPPADDSKP(_x)                 (((_x) & 0x3) << 20)
#define   NFP_NBI_PCX_CHAR_CFG_PPPADDSKP_of(_x)              (((_x) >> 20) & 0x3)
#define   NFP_NBI_PCX_CHAR_CFG_GFPCTLIGN                     BIT(18)
#define   NFP_NBI_PCX_CHAR_CFG_GFPBARK                       BIT(17)
#define   NFP_NBI_PCX_CHAR_CFG_GFPEXT                        BIT(16)
#define   NFP_NBI_PCX_CHAR_CFG_PBBENABLE                     BIT(11)
#define   NFP_NBI_PCX_CHAR_CFG_INITTAGEN                     BIT(10)
#define   NFP_NBI_PCX_CHAR_CFG_MAXVLANS(_x)                  (((_x) & 0x3) << 8)
#define   NFP_NBI_PCX_CHAR_CFG_MAXVLANS_of(_x)               (((_x) >> 8) & 0x3)
#define   NFP_NBI_PCX_CHAR_CFG_INITIALTAGSIZE(_x)            (((_x) & 0xf) << 4)
#define   NFP_NBI_PCX_CHAR_CFG_INITIALTAGSIZE_of(_x)         (((_x) >> 4) & 0xf)
#define   NFP_NBI_PCX_CHAR_CFG_PROTSKIP(_x)                  (((_x) & 0xf) << 0)
#define   NFP_NBI_PCX_CHAR_CFG_PROTSKIP_of(_x)               (((_x) >> 0) & 0xf)
/*----------------------------------------------------------------
  Register: TcamTagControl[2] - Configuration of Ethernet Tcam Tag X control
    Bits:  5:4  Length(ro) - Number of 16-bit words to skip if the tag matches
    Bits:  0    Enable(rw) - Set to enable use of the TCAM tag in Ethernet packets
*/
#define NFP_NBI_PCX_CHAR_TCAMTAGCTL(_x)                      (0x00000008 + (0x4 * ((_x) & 0x1)))
#define   NFP_NBI_PCX_CHAR_TCAMTAGCTL_LENGTH(_x)             (((_x) & 0x3) << 4)
#define   NFP_NBI_PCX_CHAR_TCAMTAGCTL_LENGTH_of(_x)          (((_x) >> 4) & 0x3)
#define     NFP_NBI_PCX_CHAR_TCAMTAGCTL_LENGTH_2             (0)
#define     NFP_NBI_PCX_CHAR_TCAMTAGCTL_LENGTH_3             (1)
#define     NFP_NBI_PCX_CHAR_TCAMTAGCTL_LENGTH_4             (2)
#define   NFP_NBI_PCX_CHAR_TCAMTAGCTL_ENABLE                 BIT(0)
/*----------------------------------------------------------------
  Register: TcamTag[2] - Configuration of Ethernet Tcam Tag X mask and match values
    Bits: 31:16 Mask(rw) - Mask for ANDing with 16-bit of Ethernet packet data
    Bits: 15:0  Value(rw) - Value to compare 16-bits of the Ethernet packet to after masking
*/
#define NFP_NBI_PCX_CHAR_TCAMTAG(_x)                         (0x00000010 + (0x4 * ((_x) & 0x1)))
#define   NFP_NBI_PCX_CHAR_TCAMTAG_MASK(_x)                  (((_x) & 0xffff) << 16)
#define   NFP_NBI_PCX_CHAR_TCAMTAG_MASK_of(_x)               (((_x) >> 16) & 0xffff)
#define   NFP_NBI_PCX_CHAR_TCAMTAG_VALUE(_x)                 (((_x) & 0xffff) << 0)
#define   NFP_NBI_PCX_CHAR_TCAMTAG_VALUE_of(_x)              (((_x) >> 0) & 0xffff)
/*----------------------------------------------------------------
  Register: Sequence - Sequence number of last packet characterized - internal use only
    Bits: 15:0  Number(ro) - Number of packets presented by characterizer to picoengine pool
*/
#define NFP_NBI_PCX_CHAR_SEQUENCE                            0x00000018
#define   NFP_NBI_PCX_CHAR_SEQUENCE_SEQNUM(_x)               (((_x) & 0xffff) << 0)
#define   NFP_NBI_PCX_CHAR_SEQUENCE_SEQNUM_of(_x)            (((_x) >> 0) & 0xffff)
/*----------------------------------------------------------------
  Register: TableSet - Table set configuration and status
    Bits: 31    OneShot(rw) - If asserted, the active table set will be cleared after the next packet utilizes table set 1; for internal use only
    Bits:  7:6  CharacterizerInUse(ro) - One bit per table set indicating that the characterizer is actively working on packets belonging to those table sets
    Bits:  5:4  PicoengineInUse(ro) - One bit per table set indicating that picoengines are actively working on packets belonging to those table sets
    Bits:  0    Active(rw) - Table set to use for packets that complete characterization
*/
#define NFP_NBI_PCX_CHAR_TABLESET                            0x0000001c
#define   NFP_NBI_PCX_CHAR_TABLESET_ONESHOT                  BIT(31)
#define   NFP_NBI_PCX_CHAR_TABLESET_CHINUSE(_x)              (((_x) & 0x3) << 6)
#define   NFP_NBI_PCX_CHAR_TABLESET_CHINUSE_of(_x)           (((_x) >> 6) & 0x3)
#define   NFP_NBI_PCX_CHAR_TABLESET_PEINUSE(_x)              (((_x) & 0x3) << 4)
#define   NFP_NBI_PCX_CHAR_TABLESET_PEINUSE_of(_x)           (((_x) >> 4) & 0x3)
#define   NFP_NBI_PCX_CHAR_TABLESET_ACTIVE                   BIT(0)
/*----------------------------------------------------------------
  Register: Override - Configuration of characterization override - internal use only
    Bits:  1    OneShot(wo) - Set if characterization override enable should be cleared after it is used once
    Bits:  0    Enable(wo) - Set if characterization override should be used - for internal test purposes only
*/
#define NFP_NBI_PCX_CHAR_OVERRIDE                            0x00000020
#define   NFP_NBI_PCX_CHAR_OVERRIDE_ONESHOT                  BIT(1)
#define   NFP_NBI_PCX_CHAR_OVERRIDE_ENB                      BIT(0)
/*----------------------------------------------------------------
  Register: OverridePort - Configuration of characterization override - internal use only
    Bits: 31:0  Port(wo) - Value to be used for the 'port' if characterization override is used for the picoengines
*/
#define NFP_NBI_PCX_CHAR_OVPORT                              0x00000024
#define   NFP_NBI_PCX_CHAR_OVPORT_PORT(_x)                   (((_x) & 0xffffffff) << 0)
#define   NFP_NBI_PCX_CHAR_OVPORT_PORT_of(_x)                (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: OverrideFlags - Configuration of characterization override - internal use only
    Bits: 31:0  Flags(wo) - Value to be used for the 'flags' if characterization override is used for the picoengines
*/
#define NFP_NBI_PCX_CHAR_OVFLAG                              0x00000028
#define   NFP_NBI_PCX_CHAR_OVFLAG_FLAGS(_x)                  (((_x) & 0xffffffff) << 0)
#define   NFP_NBI_PCX_CHAR_OVFLAG_FLAGS_of(_x)               (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: OverrideOffsets - Configuration of characterization override - internal use only
    Bits: 31:0  Offsets(wo) - Value to be used for the 'offsets' if characterization override is used for the picoengines
*/
#define NFP_NBI_PCX_CHAR_OVOFFSET                            0x0000002c
#define   NFP_NBI_PCX_CHAR_OVOFFSET_OFFSETS(_x)              (((_x) & 0xffffffff) << 0)
#define   NFP_NBI_PCX_CHAR_OVOFFSET_OFFSETS_of(_x)           (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: TcamMatchLow[7] - Results TCAM match low for entry X
    Bits: 31:0  Value(wo) - Match value for characterization TCAM; compared with the characterization result low 32 bits ANDed with the mask
*/
#define NFP_NBI_PCX_CHAR_TCAMMATCHLOW(_x)                    (0x00000200 + (0x10 * ((_x) & 0x7)))
#define   NFP_NBI_PCX_CHAR_TCAMMATCHLOW_VALUE(_x)            (((_x) & 0xffffffff) << 0)
#define   NFP_NBI_PCX_CHAR_TCAMMATCHLOW_VALUE_of(_x)         (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: TcamMatchHigh[7] - Results TCAM match high for entry X
    Bits: 31:0  Value(wo) - Match value for characterization TCAM; compared with the characterization result high 32 bits ANDed with the mask
*/
#define NFP_NBI_PCX_CHAR_TCAMMATCHHIGH(_x)                   (0x00000204 + (0x10 * ((_x) & 0x7)))
#define   NFP_NBI_PCX_CHAR_TCAMMATCHHIGH_VALUE(_x)           (((_x) & 0xffffffff) << 0)
#define   NFP_NBI_PCX_CHAR_TCAMMATCHHIGH_VALUE_of(_x)        (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: TcamMaskLow[7] - Results TCAM mask low for entry X
    Bits: 31:0  Value(wo) - Mask value for characterization TCAM; masked with the characterization result low 32 bits before comparison with the match low
*/
#define NFP_NBI_PCX_CHAR_TCAMMASKLOW(_x)                     (0x00000208 + (0x10 * ((_x) & 0x7)))
#define   NFP_NBI_PCX_CHAR_TCAMMASKLOW_VALUE(_x)             (((_x) & 0xffffffff) << 0)
#define   NFP_NBI_PCX_CHAR_TCAMMASKLOW_VALUE_of(_x)          (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: TcamMaskHigh[7] - Results TCAM mask high for entry X
    Bits: 31:0  Value(wo) - Mask value for characterization TCAM; compared with the characterization result high 32 bits before comparison with the match high
*/
#define NFP_NBI_PCX_CHAR_TCAMMASKHIGH(_x)                    (0x0000020c + (0x10 * ((_x) & 0x7)))
#define   NFP_NBI_PCX_CHAR_TCAMMASKHIGH_VALUE(_x)            (((_x) & 0xffffffff) << 0)
#define   NFP_NBI_PCX_CHAR_TCAMMASKHIGH_VALUE_of(_x)         (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: TcamMapping[8] - Picoengine start data for a match with TCAM X of characterization results
    Bits: 31:16 Index(wo) - Index to use if MetadataOverride is set
    Bits:  8    Override(wo) - Set to override the use of the metadata as an index; use index in this register instead
    Bits:  7:4  Metadata(wo) - Which metadata to use as an index for the instruction lookup, unless metadata override is set
    Bits:  2:0  Table(wo) - Local instruction table to use for initial instructions
*/
#define NFP_NBI_PCX_CHAR_TCAMMAPPING(_x)                     (0x00000300 + (0x4 * ((_x) & 0x7)))
#define   NFP_NBI_PCX_CHAR_TCAMMAPPING_INDEX(_x)             (((_x) & 0xffff) << 16)
#define   NFP_NBI_PCX_CHAR_TCAMMAPPING_INDEX_of(_x)          (((_x) >> 16) & 0xffff)
#define   NFP_NBI_PCX_CHAR_TCAMMAPPING_OVERRIDE              BIT(8)
#define   NFP_NBI_PCX_CHAR_TCAMMAPPING_METADATA(_x)          (((_x) & 0xf) << 4)
#define   NFP_NBI_PCX_CHAR_TCAMMAPPING_METADATA_of(_x)       (((_x) >> 4) & 0xf)
#define   NFP_NBI_PCX_CHAR_TCAMMAPPING_TABLE(_x)             (((_x) & 0x7) << 0)
#define   NFP_NBI_PCX_CHAR_TCAMMAPPING_TABLE_of(_x)          (((_x) >> 0) & 0x7)
/*----------------------------------------------------------------
  Register: PortCfg[128] - Characterization port config X
    Bits:  7:5  UserType(wo) - Logical port type as passed to the output of the hardware characterization
    Bits:  4:2  Skip(wo) - Initial number of 16-bits to skip before the packet starts on the logical port; use this to skip any MAC prepend or any proprietary pre-DA/SA tagging for Ethernet packets
    Bits:  1:0  Analysis(wo) - Type of outer characterization to attempt for packets on the logical port
*/
#define NFP_NBI_PCX_CHAR_PORTCFG(_x)                         (0x00000400 + (0x4 * ((_x) & 0x7f)))
#define   NFP_NBI_PCX_CHAR_PORTCFG_USERTYPE(_x)              (((_x) & 0x7) << 5)
#define   NFP_NBI_PCX_CHAR_PORTCFG_USERTYPE_of(_x)           (((_x) >> 5) & 0x7)
#define   NFP_NBI_PCX_CHAR_PORTCFG_SKIP(_x)                  (((_x) & 0x7) << 2)
#define   NFP_NBI_PCX_CHAR_PORTCFG_SKIP_of(_x)               (((_x) >> 2) & 0x7)
#define   NFP_NBI_PCX_CHAR_PORTCFG_ANALYSIS(_x)              (((_x) & 0x3) << 0)
#define   NFP_NBI_PCX_CHAR_PORTCFG_ANALYSIS_of(_x)           (((_x) >> 0) & 0x3)
#define     NFP_NBI_PCX_CHAR_PORTCFG_ANALYSIS_GFP_F          (0)
#define     NFP_NBI_PCX_CHAR_PORTCFG_ANALYSIS_ETH            (1)
#define     NFP_NBI_PCX_CHAR_PORTCFG_ANALYSIS_PPP            (2)
#define     NFP_NBI_PCX_CHAR_PORTCFG_ANALYSIS_INNERTYPE      (3)
/*----------------------------------------------------------------
  Register: Accumulator[8] - Accumulator for policer X
    Bits: 31    Saturate(rw) - If set then the credit addition saturates at 0x7fffffff when a picoengine policing operation is performed; if clear then the credit addition wraps from 0x7fffffff to 0
    Bits: 30:0  Credit(rw) - Current credit for the policing accumulator
*/
#define NFP_NBI_PCX_POL_ACC(_x)                              (0x00000000 + (0x4 * ((_x) & 0x7)))
#define   NFP_NBI_PCX_POL_ACC_SATURATE                       BIT(31)
#define   NFP_NBI_PCX_POL_ACC_CREDIT(_x)                     (((_x) & 0x7fffffff) << 0)
#define   NFP_NBI_PCX_POL_ACC_CREDIT_of(_x)                  (((_x) >> 0) & 0x7fffffff)
/*----------------------------------------------------------------
  Register: CreditRate[8] - Credit rate adjuster for policer X
    Bits: 31    CreditShift(rw) - If set then the credit subtraction is by 'Credit'*256; if clear then the credit subtraction is by just 'Credit'. (Note the binary negation described in the Credit field occurs after this credit shift)
    Bits: 30:20 Credit(rw) - Value to subtract from credit at 'interval', optionally shifted left by 8. (Note that subtraction is performed with addition of ~Credit, so a value field of 0 indicates subtract 1)
    Bits: 19:0  Interval(rw) - Interval between credit subtractions; if 0 then credit is not subtracted from the accumulator
*/
#define NFP_NBI_PCX_POL_RATE(_x)                             (0x00000020 + (0x4 * ((_x) & 0x7)))
#define   NFP_NBI_PCX_POL_RATE_SHIFT                         BIT(31)
#define   NFP_NBI_PCX_POL_RATE_CREDIT(_x)                    (((_x) & 0x7ff) << 20)
#define   NFP_NBI_PCX_POL_RATE_CREDIT_of(_x)                 (((_x) >> 20) & 0x7ff)
#define   NFP_NBI_PCX_POL_RATE_INTERVAL(_x)                  (((_x) & 0xfffff) << 0)
#define   NFP_NBI_PCX_POL_RATE_INTERVAL_of(_x)               (((_x) >> 0) & 0xfffff)
/*----------------------------------------------------------------
  Register: Comparator[8] - Policing comparator X
    Bits: 30:0  Value(rw) - Value to compare against the result of a policing operation
*/
#define NFP_NBI_PCX_POL_CMP(_x)                              (0x00000040 + (0x4 * ((_x) & 0x7)))
#define   NFP_NBI_PCX_POL_CMP_VALUE(_x)                      (((_x) & 0x7fffffff) << 0)
#define   NFP_NBI_PCX_POL_CMP_VALUE_of(_x)                   (((_x) >> 0) & 0x7fffffff)
/*----------------------------------------------------------------
  Register: Config - Policing configuration
    Bits: 15:0  ClockDivide(rw) - Clock divider value, effects rate that credit 'clock' runs
*/
#define NFP_NBI_PCX_POL_CNFG                                 0x00000060
#define   NFP_NBI_PCX_POL_CNFG_CLKDIV(_x)                    (((_x) & 0xffff) << 0)
#define   NFP_NBI_PCX_POL_CNFG_CLKDIV_of(_x)                 (((_x) >> 0) & 0xffff)
/*----------------------------------------------------------------
  Register: Sequence[8] - Picoengine sequencer X
    Bits: 31:0  Sequence(rw) - Sequence number to provide for next packet
*/
#define NFP_NBI_PCX_POL_SEQUENCE(_x)                         (0x00000080 + (0x4 * ((_x) & 0x7)))
#define   NFP_NBI_PCX_POL_SEQUENCE_SEQUENCE(_x)              (((_x) & 0xffffffff) << 0)
#define   NFP_NBI_PCX_POL_SEQUENCE_SEQUENCE_of(_x)           (((_x) >> 0) & 0xffffffff)

#else /* NFP6000_LONGNAMES */

/*----------------------------------------------------------------
  Register: AllLocalSram_NbiPreTableLut8[2048] - Memory contents for a LUT8 table lookup
  Register: LocalSram1_NbiPreTableLut8[2048] - Memory contents for a LUT8 table lookup
  Register: LocalSram2_NbiPreTableLut8[2048] - Memory contents for a LUT8 table lookup
  Register: LocalSram3_NbiPreTableLut8[2048] - Memory contents for a LUT8 table lookup
  Register: LocalSram4_NbiPreTableLut8[2048] - Memory contents for a LUT8 table lookup
  Register: LocalSram5_NbiPreTableLut8[2048] - Memory contents for a LUT8 table lookup
  Register: LocalSram6_NbiPreTableLut8[2048] - Memory contents for a LUT8 table lookup
  Register: LocalSram7_NbiPreTableLut8[2048] - Memory contents for a LUT8 table lookup
  Register: LocalSram8_NbiPreTableLut8[2048] - Memory contents for a LUT8 table lookup
  Register: LocalSram9_NbiPreTableLut8[2048] - Memory contents for a LUT8 table lookup
  Register: LocalSram10_NbiPreTableLut8[2048] - Memory contents for a LUT8 table lookup
  Register: LocalSram11_NbiPreTableLut8[2048] - Memory contents for a LUT8 table lookup
  Register: LocalSram0_NbiPreTableLut8[2048] - Memory contents for a LUT8 table lookup
  Register: AllSharedSram_NbiPreTableLut8[8192] - Memory contents for a LUT8 table lookup
  Register: SharedSram0_NbiPreTableLut8[8192] - Memory contents for a LUT8 table lookup
  Register: SharedSram1_NbiPreTableLut8[8192] - Memory contents for a LUT8 table lookup
  Register: PacketSram_NbiPreTableLut8[32768] - Memory contents for a LUT8 table lookup
    Bits: 127:120 Data15(rw) - Result if offset to data word is 15
    Bits: 119:112 Data14(rw) - Result if offset to data word is 14
    Bits: 111:104 Data13(rw) - Result if offset to data word is 13
    Bits: 103:96 Data12(rw) - Result if offset to data word is 12
    Bits: 95:88 Data11(rw) - Result if offset to data word is 11
    Bits: 87:80 Data10(rw) - Result if offset to data word is 10
    Bits: 79:72 Data9(rw) - Result if offset to data word is 9
    Bits: 71:64 Data8(rw) - Result if offset to data word is 8
    Bits: 63:56 Data7(rw) - Result if offset to data word is 7
    Bits: 55:48 Data6(rw) - Result if offset to data word is 6
    Bits: 47:40 Data5(rw) - Result if offset to data word is 5
    Bits: 39:32 Data4(rw) - Result if offset to data word is 4
    Bits: 31:24 Data3(rw) - Result if offset to data word is 3
    Bits: 23:16 Data2(rw) - Result if offset to data word is 2
    Bits: 15:8  Data1(rw) - Result if offset to data word is 1
    Bits:  7:0  Data0(rw) - Result if offset to data word is 0
*/
#define NFP_NBI_PC_AllLocalSram_NbiPreTableLut8(_x)          (0x00000000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram1_NbiPreTableLut8(_x)            (0x00004000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram2_NbiPreTableLut8(_x)            (0x00008000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram3_NbiPreTableLut8(_x)            (0x0000c000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram4_NbiPreTableLut8(_x)            (0x00010000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram5_NbiPreTableLut8(_x)            (0x00014000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram6_NbiPreTableLut8(_x)            (0x00018000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram7_NbiPreTableLut8(_x)            (0x0001c000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram8_NbiPreTableLut8(_x)            (0x00020000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram9_NbiPreTableLut8(_x)            (0x00024000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram10_NbiPreTableLut8(_x)           (0x00028000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram11_NbiPreTableLut8(_x)           (0x0002c000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram0_NbiPreTableLut8(_x)            (0x00030000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_AllSharedSram_NbiPreTableLut8(_x)         (0x00080000 + (0x8 * ((_x) & 0x1fff)))
#define NFP_NBI_PC_SharedSram0_NbiPreTableLut8(_x)           (0x00090000 + (0x8 * ((_x) & 0x1fff)))
#define NFP_NBI_PC_SharedSram1_NbiPreTableLut8(_x)           (0x000b0000 + (0x8 * ((_x) & 0x1fff)))
#define NFP_NBI_PC_PacketSram_NbiPreTableLut8(_x)            (0x000c0000 + (0x8 * ((_x) & 0x7fff)))
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableLut8_Data15(_x) (((_x) & 0xff) << 120)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableLut8_Data15_of(_x) (((_x) >> 120) & 0xff)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableLut8_Data14(_x) (((_x) & 0xff) << 112)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableLut8_Data14_of(_x) (((_x) >> 112) & 0xff)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableLut8_Data13(_x) (((_x) & 0xff) << 104)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableLut8_Data13_of(_x) (((_x) >> 104) & 0xff)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableLut8_Data12(_x) (((_x) & 0xff) << 96)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableLut8_Data12_of(_x) (((_x) >> 96) & 0xff)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableLut8_Data11(_x) (((_x) & 0xff) << 88)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableLut8_Data11_of(_x) (((_x) >> 88) & 0xff)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableLut8_Data10(_x) (((_x) & 0xff) << 80)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableLut8_Data10_of(_x) (((_x) >> 80) & 0xff)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableLut8_Data9(_x)  (((_x) & 0xff) << 72)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableLut8_Data9_of(_x) (((_x) >> 72) & 0xff)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableLut8_Data8(_x)  (((_x) & 0xff) << 64)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableLut8_Data8_of(_x) (((_x) >> 64) & 0xff)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableLut8_Data7(_x)  (((_x) & 0xff) << 56)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableLut8_Data7_of(_x) (((_x) >> 56) & 0xff)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableLut8_Data6(_x)  (((_x) & 0xff) << 48)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableLut8_Data6_of(_x) (((_x) >> 48) & 0xff)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableLut8_Data5(_x)  (((_x) & 0xff) << 40)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableLut8_Data5_of(_x) (((_x) >> 40) & 0xff)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableLut8_Data4(_x)  (((_x) & 0xff) << 32)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableLut8_Data4_of(_x) (((_x) >> 32) & 0xff)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableLut8_Data3(_x)  (((_x) & 0xff) << 24)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableLut8_Data3_of(_x) (((_x) >> 24) & 0xff)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableLut8_Data2(_x)  (((_x) & 0xff) << 16)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableLut8_Data2_of(_x) (((_x) >> 16) & 0xff)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableLut8_Data1(_x)  (((_x) & 0xff) << 8)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableLut8_Data1_of(_x) (((_x) >> 8) & 0xff)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableLut8_Data0(_x)  (((_x) & 0xff) << 0)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableLut8_Data0_of(_x) (((_x) >> 0) & 0xff)
/*----------------------------------------------------------------
  Register: AllLocalSram_NbiPreTableLut16[2048] - Memory contents for a LUT16 table lookup
  Register: LocalSram1_NbiPreTableLut16[2048] - Memory contents for a LUT16 table lookup
  Register: LocalSram2_NbiPreTableLut16[2048] - Memory contents for a LUT16 table lookup
  Register: LocalSram3_NbiPreTableLut16[2048] - Memory contents for a LUT16 table lookup
  Register: LocalSram4_NbiPreTableLut16[2048] - Memory contents for a LUT16 table lookup
  Register: LocalSram5_NbiPreTableLut16[2048] - Memory contents for a LUT16 table lookup
  Register: LocalSram6_NbiPreTableLut16[2048] - Memory contents for a LUT16 table lookup
  Register: LocalSram7_NbiPreTableLut16[2048] - Memory contents for a LUT16 table lookup
  Register: LocalSram8_NbiPreTableLut16[2048] - Memory contents for a LUT16 table lookup
  Register: LocalSram9_NbiPreTableLut16[2048] - Memory contents for a LUT16 table lookup
  Register: LocalSram10_NbiPreTableLut16[2048] - Memory contents for a LUT16 table lookup
  Register: LocalSram11_NbiPreTableLut16[2048] - Memory contents for a LUT16 table lookup
  Register: LocalSram0_NbiPreTableLut16[2048] - Memory contents for a LUT16 table lookup
  Register: AllSharedSram_NbiPreTableLut16[8192] - Memory contents for a LUT16 table lookup
  Register: SharedSram0_NbiPreTableLut16[8192] - Memory contents for a LUT16 table lookup
  Register: SharedSram1_NbiPreTableLut16[8192] - Memory contents for a LUT16 table lookup
  Register: PacketSram_NbiPreTableLut16[32768] - Memory contents for a LUT16 table lookup
    Bits: 127:112 Data7(rw) - Result if offset to data word is 7
    Bits: 111:96 Data6(rw) - Result if offset to data word is 6
    Bits: 95:80 Data5(rw) - Result if offset to data word is 5
    Bits: 79:64 Data4(rw) - Result if offset to data word is 4
    Bits: 63:48 Data3(rw) - Result if offset to data word is 3
    Bits: 47:32 Data2(rw) - Result if offset to data word is 2
    Bits: 31:16 Data1(rw) - Result if offset to data word is 1
    Bits: 15:0  Data0(rw) - Result if offset to data word is 0
*/
#define NFP_NBI_PC_AllLocalSram_NbiPreTableLut16(_x)         (0x00000000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram1_NbiPreTableLut16(_x)           (0x00004000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram2_NbiPreTableLut16(_x)           (0x00008000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram3_NbiPreTableLut16(_x)           (0x0000c000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram4_NbiPreTableLut16(_x)           (0x00010000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram5_NbiPreTableLut16(_x)           (0x00014000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram6_NbiPreTableLut16(_x)           (0x00018000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram7_NbiPreTableLut16(_x)           (0x0001c000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram8_NbiPreTableLut16(_x)           (0x00020000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram9_NbiPreTableLut16(_x)           (0x00024000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram10_NbiPreTableLut16(_x)          (0x00028000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram11_NbiPreTableLut16(_x)          (0x0002c000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram0_NbiPreTableLut16(_x)           (0x00030000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_AllSharedSram_NbiPreTableLut16(_x)        (0x00080000 + (0x8 * ((_x) & 0x1fff)))
#define NFP_NBI_PC_SharedSram0_NbiPreTableLut16(_x)          (0x00090000 + (0x8 * ((_x) & 0x1fff)))
#define NFP_NBI_PC_SharedSram1_NbiPreTableLut16(_x)          (0x000b0000 + (0x8 * ((_x) & 0x1fff)))
#define NFP_NBI_PC_PacketSram_NbiPreTableLut16(_x)           (0x000c0000 + (0x8 * ((_x) & 0x7fff)))
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableLut16_Data7(_x) (((_x) & 0xffff) << 112)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableLut16_Data7_of(_x) (((_x) >> 112) & 0xffff)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableLut16_Data6(_x) (((_x) & 0xffff) << 96)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableLut16_Data6_of(_x) (((_x) >> 96) & 0xffff)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableLut16_Data5(_x) (((_x) & 0xffff) << 80)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableLut16_Data5_of(_x) (((_x) >> 80) & 0xffff)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableLut16_Data4(_x) (((_x) & 0xffff) << 64)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableLut16_Data4_of(_x) (((_x) >> 64) & 0xffff)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableLut16_Data3(_x) (((_x) & 0xffff) << 48)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableLut16_Data3_of(_x) (((_x) >> 48) & 0xffff)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableLut16_Data2(_x) (((_x) & 0xffff) << 32)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableLut16_Data2_of(_x) (((_x) >> 32) & 0xffff)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableLut16_Data1(_x) (((_x) & 0xffff) << 16)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableLut16_Data1_of(_x) (((_x) >> 16) & 0xffff)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableLut16_Data0(_x) (((_x) & 0xffff) << 0)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableLut16_Data0_of(_x) (((_x) >> 0) & 0xffff)
/*----------------------------------------------------------------
  Register: AllLocalSram_NbiPreTableLut32[2048] - Memory contents for a LUT32 table lookup
  Register: LocalSram1_NbiPreTableLut32[2048] - Memory contents for a LUT32 table lookup
  Register: LocalSram2_NbiPreTableLut32[2048] - Memory contents for a LUT32 table lookup
  Register: LocalSram3_NbiPreTableLut32[2048] - Memory contents for a LUT32 table lookup
  Register: LocalSram4_NbiPreTableLut32[2048] - Memory contents for a LUT32 table lookup
  Register: LocalSram5_NbiPreTableLut32[2048] - Memory contents for a LUT32 table lookup
  Register: LocalSram6_NbiPreTableLut32[2048] - Memory contents for a LUT32 table lookup
  Register: LocalSram7_NbiPreTableLut32[2048] - Memory contents for a LUT32 table lookup
  Register: LocalSram8_NbiPreTableLut32[2048] - Memory contents for a LUT32 table lookup
  Register: LocalSram9_NbiPreTableLut32[2048] - Memory contents for a LUT32 table lookup
  Register: LocalSram10_NbiPreTableLut32[2048] - Memory contents for a LUT32 table lookup
  Register: LocalSram11_NbiPreTableLut32[2048] - Memory contents for a LUT32 table lookup
  Register: LocalSram0_NbiPreTableLut32[2048] - Memory contents for a LUT32 table lookup
  Register: AllSharedSram_NbiPreTableLut32[8192] - Memory contents for a LUT32 table lookup
  Register: SharedSram0_NbiPreTableLut32[8192] - Memory contents for a LUT32 table lookup
  Register: SharedSram1_NbiPreTableLut32[8192] - Memory contents for a LUT32 table lookup
  Register: PacketSram_NbiPreTableLut32[32768] - Memory contents for a LUT32 table lookup
    Bits: 127:96 Data3(rw) - Result if offset to data word is 3
    Bits: 95:64 Data2(rw) - Result if offset to data word is 2
    Bits: 63:32 Data1(rw) - Result if offset to data word is 1
    Bits: 31:0  Data0(rw) - Result if offset to data word is 0
*/
#define NFP_NBI_PC_AllLocalSram_NbiPreTableLut32(_x)         (0x00000000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram1_NbiPreTableLut32(_x)           (0x00004000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram2_NbiPreTableLut32(_x)           (0x00008000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram3_NbiPreTableLut32(_x)           (0x0000c000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram4_NbiPreTableLut32(_x)           (0x00010000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram5_NbiPreTableLut32(_x)           (0x00014000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram6_NbiPreTableLut32(_x)           (0x00018000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram7_NbiPreTableLut32(_x)           (0x0001c000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram8_NbiPreTableLut32(_x)           (0x00020000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram9_NbiPreTableLut32(_x)           (0x00024000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram10_NbiPreTableLut32(_x)          (0x00028000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram11_NbiPreTableLut32(_x)          (0x0002c000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram0_NbiPreTableLut32(_x)           (0x00030000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_AllSharedSram_NbiPreTableLut32(_x)        (0x00080000 + (0x8 * ((_x) & 0x1fff)))
#define NFP_NBI_PC_SharedSram0_NbiPreTableLut32(_x)          (0x00090000 + (0x8 * ((_x) & 0x1fff)))
#define NFP_NBI_PC_SharedSram1_NbiPreTableLut32(_x)          (0x000b0000 + (0x8 * ((_x) & 0x1fff)))
#define NFP_NBI_PC_PacketSram_NbiPreTableLut32(_x)           (0x000c0000 + (0x8 * ((_x) & 0x7fff)))
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableLut32_Data3(_x) (((_x) & 0xffffffff) << 96)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableLut32_Data3_of(_x) (((_x) >> 96) & 0xffffffff)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableLut32_Data2(_x) (((_x) & 0xffffffff) << 64)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableLut32_Data2_of(_x) (((_x) >> 64) & 0xffffffff)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableLut32_Data1(_x) (((_x) & 0xffffffff) << 32)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableLut32_Data1_of(_x) (((_x) >> 32) & 0xffffffff)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableLut32_Data0(_x) (((_x) & 0xffffffff) << 0)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableLut32_Data0_of(_x) (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: AllLocalSram_NbiPreTableMultibit[2048] - Memory contents for a Multibit table lookup
  Register: LocalSram1_NbiPreTableMultibit[2048] - Memory contents for a Multibit table lookup
  Register: LocalSram2_NbiPreTableMultibit[2048] - Memory contents for a Multibit table lookup
  Register: LocalSram3_NbiPreTableMultibit[2048] - Memory contents for a Multibit table lookup
  Register: LocalSram4_NbiPreTableMultibit[2048] - Memory contents for a Multibit table lookup
  Register: LocalSram5_NbiPreTableMultibit[2048] - Memory contents for a Multibit table lookup
  Register: LocalSram6_NbiPreTableMultibit[2048] - Memory contents for a Multibit table lookup
  Register: LocalSram7_NbiPreTableMultibit[2048] - Memory contents for a Multibit table lookup
  Register: LocalSram8_NbiPreTableMultibit[2048] - Memory contents for a Multibit table lookup
  Register: LocalSram9_NbiPreTableMultibit[2048] - Memory contents for a Multibit table lookup
  Register: LocalSram10_NbiPreTableMultibit[2048] - Memory contents for a Multibit table lookup
  Register: LocalSram11_NbiPreTableMultibit[2048] - Memory contents for a Multibit table lookup
  Register: LocalSram0_NbiPreTableMultibit[2048] - Memory contents for a Multibit table lookup
  Register: AllSharedSram_NbiPreTableMultibit[8192] - Memory contents for a Multibit table lookup
  Register: SharedSram0_NbiPreTableMultibit[8192] - Memory contents for a Multibit table lookup
  Register: SharedSram1_NbiPreTableMultibit[8192] - Memory contents for a Multibit table lookup
  Register: PacketSram_NbiPreTableMultibit[32768] - Memory contents for a Multibit table lookup
    Bits: 121:116 SelA(rw) - Which bit to select for 'A'
    Bits: 115:110 SelB(rw) - Which bit to select for 'B'
    Bits: 109:104 SelC(rw) - Which bit to select for 'C'
    Bits: 103:98 SelD(rw) - Which bit to select for 'D'
    Bits: 97:92 SelE(rw) - Which bit to select for 'E'
    Bits: 91:86 SelF(rw) - Which bit to select for 'F'
    Bits: 85:80 SelG(rw) - Which bit to select for 'G'
    Bits: 79:70 Data7(rw) - Result if bits ABCDEFG matches 1x1xxx1
    Bits: 69:60 Data6(rw) - Result if bits ABCDEFG matches 1x1xxx0
    Bits: 59:50 Data5(rw) - Result if bits ABCDEFG matches 1x0xx1x
    Bits: 49:40 Data4(rw) - Result if bits ABCDEFG matches 1x0xx0x
    Bits: 39:30 Data3(rw) - Result if bits ABCDEFG matches 01xx1xx
    Bits: 29:20 Data2(rw) - Result if bits ABCDEFG matches 01xx0xx
    Bits: 19:10 Data1(rw) - Result if bits ABCDEFG matches 00x1xxx
    Bits:  9:0  Data0(rw) - Result if bits ABCDEFG matches 00x0xxx
*/
#define NFP_NBI_PC_AllLocalSram_NbiPreTableMultibit(_x)      (0x00000000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram1_NbiPreTableMultibit(_x)        (0x00004000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram2_NbiPreTableMultibit(_x)        (0x00008000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram3_NbiPreTableMultibit(_x)        (0x0000c000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram4_NbiPreTableMultibit(_x)        (0x00010000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram5_NbiPreTableMultibit(_x)        (0x00014000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram6_NbiPreTableMultibit(_x)        (0x00018000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram7_NbiPreTableMultibit(_x)        (0x0001c000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram8_NbiPreTableMultibit(_x)        (0x00020000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram9_NbiPreTableMultibit(_x)        (0x00024000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram10_NbiPreTableMultibit(_x)       (0x00028000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram11_NbiPreTableMultibit(_x)       (0x0002c000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram0_NbiPreTableMultibit(_x)        (0x00030000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_AllSharedSram_NbiPreTableMultibit(_x)     (0x00080000 + (0x8 * ((_x) & 0x1fff)))
#define NFP_NBI_PC_SharedSram0_NbiPreTableMultibit(_x)       (0x00090000 + (0x8 * ((_x) & 0x1fff)))
#define NFP_NBI_PC_SharedSram1_NbiPreTableMultibit(_x)       (0x000b0000 + (0x8 * ((_x) & 0x1fff)))
#define NFP_NBI_PC_PacketSram_NbiPreTableMultibit(_x)        (0x000c0000 + (0x8 * ((_x) & 0x7fff)))
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableMultibit_SelA(_x) (((_x) & 0x3f) << 116)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableMultibit_SelA_of(_x) (((_x) >> 116) & 0x3f)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableMultibit_SelB(_x) (((_x) & 0x3f) << 110)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableMultibit_SelB_of(_x) (((_x) >> 110) & 0x3f)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableMultibit_SelC(_x) (((_x) & 0x3f) << 104)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableMultibit_SelC_of(_x) (((_x) >> 104) & 0x3f)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableMultibit_SelD(_x) (((_x) & 0x3f) << 98)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableMultibit_SelD_of(_x) (((_x) >> 98) & 0x3f)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableMultibit_SelE(_x) (((_x) & 0x3f) << 92)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableMultibit_SelE_of(_x) (((_x) >> 92) & 0x3f)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableMultibit_SelF(_x) (((_x) & 0x3f) << 86)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableMultibit_SelF_of(_x) (((_x) >> 86) & 0x3f)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableMultibit_SelG(_x) (((_x) & 0x3f) << 80)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableMultibit_SelG_of(_x) (((_x) >> 80) & 0x3f)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableMultibit_Data7(_x) (((_x) & 0x3ff) << 70)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableMultibit_Data7_of(_x) (((_x) >> 70) & 0x3ff)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableMultibit_Data6(_x) (((_x) & 0x3ff) << 60)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableMultibit_Data6_of(_x) (((_x) >> 60) & 0x3ff)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableMultibit_Data5(_x) (((_x) & 0x3ff) << 50)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableMultibit_Data5_of(_x) (((_x) >> 50) & 0x3ff)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableMultibit_Data4(_x) (((_x) & 0x3ff) << 40)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableMultibit_Data4_of(_x) (((_x) >> 40) & 0x3ff)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableMultibit_Data3(_x) (((_x) & 0x3ff) << 30)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableMultibit_Data3_of(_x) (((_x) >> 30) & 0x3ff)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableMultibit_Data2(_x) (((_x) & 0x3ff) << 20)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableMultibit_Data2_of(_x) (((_x) >> 20) & 0x3ff)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableMultibit_Data1(_x) (((_x) & 0x3ff) << 10)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableMultibit_Data1_of(_x) (((_x) >> 10) & 0x3ff)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableMultibit_Data0(_x) (((_x) & 0x3ff) << 0)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableMultibit_Data0_of(_x) (((_x) >> 0) & 0x3ff)
/*----------------------------------------------------------------
  Register: AllLocalSram_NbiPreTableCam8[2048] - Memory contents for a CAM8 table lookup
  Register: LocalSram1_NbiPreTableCam8[2048] - Memory contents for a CAM8 table lookup
  Register: LocalSram2_NbiPreTableCam8[2048] - Memory contents for a CAM8 table lookup
  Register: LocalSram3_NbiPreTableCam8[2048] - Memory contents for a CAM8 table lookup
  Register: LocalSram4_NbiPreTableCam8[2048] - Memory contents for a CAM8 table lookup
  Register: LocalSram5_NbiPreTableCam8[2048] - Memory contents for a CAM8 table lookup
  Register: LocalSram6_NbiPreTableCam8[2048] - Memory contents for a CAM8 table lookup
  Register: LocalSram7_NbiPreTableCam8[2048] - Memory contents for a CAM8 table lookup
  Register: LocalSram8_NbiPreTableCam8[2048] - Memory contents for a CAM8 table lookup
  Register: LocalSram9_NbiPreTableCam8[2048] - Memory contents for a CAM8 table lookup
  Register: LocalSram10_NbiPreTableCam8[2048] - Memory contents for a CAM8 table lookup
  Register: LocalSram11_NbiPreTableCam8[2048] - Memory contents for a CAM8 table lookup
  Register: LocalSram0_NbiPreTableCam8[2048] - Memory contents for a CAM8 table lookup
  Register: AllSharedSram_NbiPreTableCam8[8192] - Memory contents for a CAM8 table lookup
  Register: SharedSram0_NbiPreTableCam8[8192] - Memory contents for a CAM8 table lookup
  Register: SharedSram1_NbiPreTableCam8[8192] - Memory contents for a CAM8 table lookup
  Register: PacketSram_NbiPreTableCam8[32768] - Memory contents for a CAM8 table lookup
    Bits: 127:120 Data15(rw) - Result is 15 if selected data equals this field
    Bits: 119:112 Data14(rw) - Result is 14 if selected data equals this field
    Bits: 111:104 Data13(rw) - Result is 13 if selected data equals this field
    Bits: 103:96 Data12(rw) - Result is 12 if selected data equals this field
    Bits: 95:88 Data11(rw) - Result is 11 if selected data equals this field
    Bits: 87:80 Data10(rw) - Result is 10 if selected data equals this field
    Bits: 79:72 Data9(rw) - Result is 9 if selected data equals this field
    Bits: 71:64 Data8(rw) - Result is 8 if selected data equals this field
    Bits: 63:56 Data7(rw) - Result is 7 if selected data equals this field
    Bits: 55:48 Data6(rw) - Result is 6 if selected data equals this field
    Bits: 47:40 Data5(rw) - Result is 5 if selected data equals this field
    Bits: 39:32 Data4(rw) - Result is 4 if selected data equals this field
    Bits: 31:24 Data3(rw) - Result is 3 if selected data equals this field
    Bits: 23:16 Data2(rw) - Result is 2 if selected data equals this field
    Bits: 15:8  Data1(rw) - Result is 1 if selected data equals this field
    Bits:  7:0  Data0(rw) - Result is 0 if selected data equals this field
*/
#define NFP_NBI_PC_AllLocalSram_NbiPreTableCam8(_x)          (0x00000000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram1_NbiPreTableCam8(_x)            (0x00004000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram2_NbiPreTableCam8(_x)            (0x00008000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram3_NbiPreTableCam8(_x)            (0x0000c000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram4_NbiPreTableCam8(_x)            (0x00010000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram5_NbiPreTableCam8(_x)            (0x00014000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram6_NbiPreTableCam8(_x)            (0x00018000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram7_NbiPreTableCam8(_x)            (0x0001c000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram8_NbiPreTableCam8(_x)            (0x00020000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram9_NbiPreTableCam8(_x)            (0x00024000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram10_NbiPreTableCam8(_x)           (0x00028000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram11_NbiPreTableCam8(_x)           (0x0002c000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram0_NbiPreTableCam8(_x)            (0x00030000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_AllSharedSram_NbiPreTableCam8(_x)         (0x00080000 + (0x8 * ((_x) & 0x1fff)))
#define NFP_NBI_PC_SharedSram0_NbiPreTableCam8(_x)           (0x00090000 + (0x8 * ((_x) & 0x1fff)))
#define NFP_NBI_PC_SharedSram1_NbiPreTableCam8(_x)           (0x000b0000 + (0x8 * ((_x) & 0x1fff)))
#define NFP_NBI_PC_PacketSram_NbiPreTableCam8(_x)            (0x000c0000 + (0x8 * ((_x) & 0x7fff)))
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableCam8_Data15(_x) (((_x) & 0xff) << 120)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableCam8_Data15_of(_x) (((_x) >> 120) & 0xff)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableCam8_Data14(_x) (((_x) & 0xff) << 112)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableCam8_Data14_of(_x) (((_x) >> 112) & 0xff)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableCam8_Data13(_x) (((_x) & 0xff) << 104)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableCam8_Data13_of(_x) (((_x) >> 104) & 0xff)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableCam8_Data12(_x) (((_x) & 0xff) << 96)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableCam8_Data12_of(_x) (((_x) >> 96) & 0xff)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableCam8_Data11(_x) (((_x) & 0xff) << 88)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableCam8_Data11_of(_x) (((_x) >> 88) & 0xff)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableCam8_Data10(_x) (((_x) & 0xff) << 80)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableCam8_Data10_of(_x) (((_x) >> 80) & 0xff)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableCam8_Data9(_x)  (((_x) & 0xff) << 72)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableCam8_Data9_of(_x) (((_x) >> 72) & 0xff)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableCam8_Data8(_x)  (((_x) & 0xff) << 64)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableCam8_Data8_of(_x) (((_x) >> 64) & 0xff)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableCam8_Data7(_x)  (((_x) & 0xff) << 56)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableCam8_Data7_of(_x) (((_x) >> 56) & 0xff)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableCam8_Data6(_x)  (((_x) & 0xff) << 48)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableCam8_Data6_of(_x) (((_x) >> 48) & 0xff)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableCam8_Data5(_x)  (((_x) & 0xff) << 40)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableCam8_Data5_of(_x) (((_x) >> 40) & 0xff)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableCam8_Data4(_x)  (((_x) & 0xff) << 32)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableCam8_Data4_of(_x) (((_x) >> 32) & 0xff)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableCam8_Data3(_x)  (((_x) & 0xff) << 24)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableCam8_Data3_of(_x) (((_x) >> 24) & 0xff)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableCam8_Data2(_x)  (((_x) & 0xff) << 16)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableCam8_Data2_of(_x) (((_x) >> 16) & 0xff)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableCam8_Data1(_x)  (((_x) & 0xff) << 8)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableCam8_Data1_of(_x) (((_x) >> 8) & 0xff)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableCam8_Data0(_x)  (((_x) & 0xff) << 0)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableCam8_Data0_of(_x) (((_x) >> 0) & 0xff)
/*----------------------------------------------------------------
  Register: AllLocalSram_NbiPreTableCam16[2048] - Memory contents for a CAM16 table lookup
  Register: LocalSram1_NbiPreTableCam16[2048] - Memory contents for a CAM16 table lookup
  Register: LocalSram2_NbiPreTableCam16[2048] - Memory contents for a CAM16 table lookup
  Register: LocalSram3_NbiPreTableCam16[2048] - Memory contents for a CAM16 table lookup
  Register: LocalSram4_NbiPreTableCam16[2048] - Memory contents for a CAM16 table lookup
  Register: LocalSram5_NbiPreTableCam16[2048] - Memory contents for a CAM16 table lookup
  Register: LocalSram6_NbiPreTableCam16[2048] - Memory contents for a CAM16 table lookup
  Register: LocalSram7_NbiPreTableCam16[2048] - Memory contents for a CAM16 table lookup
  Register: LocalSram8_NbiPreTableCam16[2048] - Memory contents for a CAM16 table lookup
  Register: LocalSram9_NbiPreTableCam16[2048] - Memory contents for a CAM16 table lookup
  Register: LocalSram10_NbiPreTableCam16[2048] - Memory contents for a CAM16 table lookup
  Register: LocalSram11_NbiPreTableCam16[2048] - Memory contents for a CAM16 table lookup
  Register: LocalSram0_NbiPreTableCam16[2048] - Memory contents for a CAM16 table lookup
  Register: AllSharedSram_NbiPreTableCam16[8192] - Memory contents for a CAM16 table lookup
  Register: SharedSram0_NbiPreTableCam16[8192] - Memory contents for a CAM16 table lookup
  Register: SharedSram1_NbiPreTableCam16[8192] - Memory contents for a CAM16 table lookup
  Register: PacketSram_NbiPreTableCam16[32768] - Memory contents for a CAM16 table lookup
    Bits: 127:112 Data7(rw) - Result is 14 if selected data equals this field
    Bits: 111:96 Data6(rw) - Result is 12 if selected data equals this field
    Bits: 95:80 Data5(rw) - Result is 10 if selected data equals this field
    Bits: 79:64 Data4(rw) - Result is 8 if selected data equals this field
    Bits: 63:48 Data3(rw) - Result is 6 if selected data equals this field
    Bits: 47:32 Data2(rw) - Result is 4 if selected data equals this field
    Bits: 31:16 Data1(rw) - Result is 2 if selected data equals this field
    Bits: 15:0  Data0(rw) - Result is 0 if selected data equals this field
*/
#define NFP_NBI_PC_AllLocalSram_NbiPreTableCam16(_x)         (0x00000000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram1_NbiPreTableCam16(_x)           (0x00004000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram2_NbiPreTableCam16(_x)           (0x00008000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram3_NbiPreTableCam16(_x)           (0x0000c000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram4_NbiPreTableCam16(_x)           (0x00010000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram5_NbiPreTableCam16(_x)           (0x00014000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram6_NbiPreTableCam16(_x)           (0x00018000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram7_NbiPreTableCam16(_x)           (0x0001c000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram8_NbiPreTableCam16(_x)           (0x00020000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram9_NbiPreTableCam16(_x)           (0x00024000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram10_NbiPreTableCam16(_x)          (0x00028000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram11_NbiPreTableCam16(_x)          (0x0002c000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram0_NbiPreTableCam16(_x)           (0x00030000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_AllSharedSram_NbiPreTableCam16(_x)        (0x00080000 + (0x8 * ((_x) & 0x1fff)))
#define NFP_NBI_PC_SharedSram0_NbiPreTableCam16(_x)          (0x00090000 + (0x8 * ((_x) & 0x1fff)))
#define NFP_NBI_PC_SharedSram1_NbiPreTableCam16(_x)          (0x000b0000 + (0x8 * ((_x) & 0x1fff)))
#define NFP_NBI_PC_PacketSram_NbiPreTableCam16(_x)           (0x000c0000 + (0x8 * ((_x) & 0x7fff)))
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableCam16_Data7(_x) (((_x) & 0xffff) << 112)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableCam16_Data7_of(_x) (((_x) >> 112) & 0xffff)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableCam16_Data6(_x) (((_x) & 0xffff) << 96)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableCam16_Data6_of(_x) (((_x) >> 96) & 0xffff)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableCam16_Data5(_x) (((_x) & 0xffff) << 80)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableCam16_Data5_of(_x) (((_x) >> 80) & 0xffff)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableCam16_Data4(_x) (((_x) & 0xffff) << 64)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableCam16_Data4_of(_x) (((_x) >> 64) & 0xffff)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableCam16_Data3(_x) (((_x) & 0xffff) << 48)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableCam16_Data3_of(_x) (((_x) >> 48) & 0xffff)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableCam16_Data2(_x) (((_x) & 0xffff) << 32)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableCam16_Data2_of(_x) (((_x) >> 32) & 0xffff)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableCam16_Data1(_x) (((_x) & 0xffff) << 16)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableCam16_Data1_of(_x) (((_x) >> 16) & 0xffff)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableCam16_Data0(_x) (((_x) & 0xffff) << 0)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableCam16_Data0_of(_x) (((_x) >> 0) & 0xffff)
/*----------------------------------------------------------------
  Register: AllLocalSram_NbiPreTableCam24[2048] - Memory contents for a CAM24 table lookup
  Register: LocalSram1_NbiPreTableCam24[2048] - Memory contents for a CAM24 table lookup
  Register: LocalSram2_NbiPreTableCam24[2048] - Memory contents for a CAM24 table lookup
  Register: LocalSram3_NbiPreTableCam24[2048] - Memory contents for a CAM24 table lookup
  Register: LocalSram4_NbiPreTableCam24[2048] - Memory contents for a CAM24 table lookup
  Register: LocalSram5_NbiPreTableCam24[2048] - Memory contents for a CAM24 table lookup
  Register: LocalSram6_NbiPreTableCam24[2048] - Memory contents for a CAM24 table lookup
  Register: LocalSram7_NbiPreTableCam24[2048] - Memory contents for a CAM24 table lookup
  Register: LocalSram8_NbiPreTableCam24[2048] - Memory contents for a CAM24 table lookup
  Register: LocalSram9_NbiPreTableCam24[2048] - Memory contents for a CAM24 table lookup
  Register: LocalSram10_NbiPreTableCam24[2048] - Memory contents for a CAM24 table lookup
  Register: LocalSram11_NbiPreTableCam24[2048] - Memory contents for a CAM24 table lookup
  Register: LocalSram0_NbiPreTableCam24[2048] - Memory contents for a CAM24 table lookup
  Register: AllSharedSram_NbiPreTableCam24[8192] - Memory contents for a CAM24 table lookup
  Register: SharedSram0_NbiPreTableCam24[8192] - Memory contents for a CAM24 table lookup
  Register: SharedSram1_NbiPreTableCam24[8192] - Memory contents for a CAM24 table lookup
  Register: PacketSram_NbiPreTableCam24[32768] - Memory contents for a CAM24 table lookup
    Bits: 119:96 Data3(rw) - Result is 12 if selected data equals this field
    Bits: 87:64 Data2(rw) - Result is 8 if selected data equals this field
    Bits: 55:32 Data1(rw) - Result is 4 if selected data equals this field
    Bits: 23:0  Data0(rw) - Result is 0 if selected data equals this field
*/
#define NFP_NBI_PC_AllLocalSram_NbiPreTableCam24(_x)         (0x00000000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram1_NbiPreTableCam24(_x)           (0x00004000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram2_NbiPreTableCam24(_x)           (0x00008000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram3_NbiPreTableCam24(_x)           (0x0000c000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram4_NbiPreTableCam24(_x)           (0x00010000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram5_NbiPreTableCam24(_x)           (0x00014000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram6_NbiPreTableCam24(_x)           (0x00018000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram7_NbiPreTableCam24(_x)           (0x0001c000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram8_NbiPreTableCam24(_x)           (0x00020000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram9_NbiPreTableCam24(_x)           (0x00024000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram10_NbiPreTableCam24(_x)          (0x00028000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram11_NbiPreTableCam24(_x)          (0x0002c000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram0_NbiPreTableCam24(_x)           (0x00030000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_AllSharedSram_NbiPreTableCam24(_x)        (0x00080000 + (0x8 * ((_x) & 0x1fff)))
#define NFP_NBI_PC_SharedSram0_NbiPreTableCam24(_x)          (0x00090000 + (0x8 * ((_x) & 0x1fff)))
#define NFP_NBI_PC_SharedSram1_NbiPreTableCam24(_x)          (0x000b0000 + (0x8 * ((_x) & 0x1fff)))
#define NFP_NBI_PC_PacketSram_NbiPreTableCam24(_x)           (0x000c0000 + (0x8 * ((_x) & 0x7fff)))
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableCam24_Data3(_x) (((_x) & 0xffffff) << 96)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableCam24_Data3_of(_x) (((_x) >> 96) & 0xffffff)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableCam24_Data2(_x) (((_x) & 0xffffff) << 64)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableCam24_Data2_of(_x) (((_x) >> 64) & 0xffffff)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableCam24_Data1(_x) (((_x) & 0xffffff) << 32)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableCam24_Data1_of(_x) (((_x) >> 32) & 0xffffff)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableCam24_Data0(_x) (((_x) & 0xffffff) << 0)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableCam24_Data0_of(_x) (((_x) >> 0) & 0xffffff)
/*----------------------------------------------------------------
  Register: AllLocalSram_NbiPreTableCam32[2048] - Memory contents for a CAM32 table lookup
  Register: LocalSram1_NbiPreTableCam32[2048] - Memory contents for a CAM32 table lookup
  Register: LocalSram2_NbiPreTableCam32[2048] - Memory contents for a CAM32 table lookup
  Register: LocalSram3_NbiPreTableCam32[2048] - Memory contents for a CAM32 table lookup
  Register: LocalSram4_NbiPreTableCam32[2048] - Memory contents for a CAM32 table lookup
  Register: LocalSram5_NbiPreTableCam32[2048] - Memory contents for a CAM32 table lookup
  Register: LocalSram6_NbiPreTableCam32[2048] - Memory contents for a CAM32 table lookup
  Register: LocalSram7_NbiPreTableCam32[2048] - Memory contents for a CAM32 table lookup
  Register: LocalSram8_NbiPreTableCam32[2048] - Memory contents for a CAM32 table lookup
  Register: LocalSram9_NbiPreTableCam32[2048] - Memory contents for a CAM32 table lookup
  Register: LocalSram10_NbiPreTableCam32[2048] - Memory contents for a CAM32 table lookup
  Register: LocalSram11_NbiPreTableCam32[2048] - Memory contents for a CAM32 table lookup
  Register: LocalSram0_NbiPreTableCam32[2048] - Memory contents for a CAM32 table lookup
  Register: AllSharedSram_NbiPreTableCam32[8192] - Memory contents for a CAM32 table lookup
  Register: SharedSram0_NbiPreTableCam32[8192] - Memory contents for a CAM32 table lookup
  Register: SharedSram1_NbiPreTableCam32[8192] - Memory contents for a CAM32 table lookup
  Register: PacketSram_NbiPreTableCam32[32768] - Memory contents for a CAM32 table lookup
    Bits: 127:96 Data3(rw) - Result is 12 if selected data equals this field
    Bits: 95:64 Data2(rw) - Result is 8 if selected data equals this field
    Bits: 63:32 Data1(rw) - Result is 4 if selected data equals this field
    Bits: 31:0  Data0(rw) - Result is 0 if selected data equals this field
*/
#define NFP_NBI_PC_AllLocalSram_NbiPreTableCam32(_x)         (0x00000000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram1_NbiPreTableCam32(_x)           (0x00004000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram2_NbiPreTableCam32(_x)           (0x00008000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram3_NbiPreTableCam32(_x)           (0x0000c000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram4_NbiPreTableCam32(_x)           (0x00010000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram5_NbiPreTableCam32(_x)           (0x00014000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram6_NbiPreTableCam32(_x)           (0x00018000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram7_NbiPreTableCam32(_x)           (0x0001c000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram8_NbiPreTableCam32(_x)           (0x00020000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram9_NbiPreTableCam32(_x)           (0x00024000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram10_NbiPreTableCam32(_x)          (0x00028000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram11_NbiPreTableCam32(_x)          (0x0002c000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram0_NbiPreTableCam32(_x)           (0x00030000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_AllSharedSram_NbiPreTableCam32(_x)        (0x00080000 + (0x8 * ((_x) & 0x1fff)))
#define NFP_NBI_PC_SharedSram0_NbiPreTableCam32(_x)          (0x00090000 + (0x8 * ((_x) & 0x1fff)))
#define NFP_NBI_PC_SharedSram1_NbiPreTableCam32(_x)          (0x000b0000 + (0x8 * ((_x) & 0x1fff)))
#define NFP_NBI_PC_PacketSram_NbiPreTableCam32(_x)           (0x000c0000 + (0x8 * ((_x) & 0x7fff)))
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableCam32_Data3(_x) (((_x) & 0xffffffff) << 96)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableCam32_Data3_of(_x) (((_x) >> 96) & 0xffffffff)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableCam32_Data2(_x) (((_x) & 0xffffffff) << 64)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableCam32_Data2_of(_x) (((_x) >> 64) & 0xffffffff)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableCam32_Data1(_x) (((_x) & 0xffffffff) << 32)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableCam32_Data1_of(_x) (((_x) >> 32) & 0xffffffff)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableCam32_Data0(_x) (((_x) & 0xffffffff) << 0)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableCam32_Data0_of(_x) (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: AllLocalSram_NbiPreTableCamR8[2048] - Memory contents for a CAMR8 table lookup
  Register: LocalSram1_NbiPreTableCamR8[2048] - Memory contents for a CAMR8 table lookup
  Register: LocalSram2_NbiPreTableCamR8[2048] - Memory contents for a CAMR8 table lookup
  Register: LocalSram3_NbiPreTableCamR8[2048] - Memory contents for a CAMR8 table lookup
  Register: LocalSram4_NbiPreTableCamR8[2048] - Memory contents for a CAMR8 table lookup
  Register: LocalSram5_NbiPreTableCamR8[2048] - Memory contents for a CAMR8 table lookup
  Register: LocalSram6_NbiPreTableCamR8[2048] - Memory contents for a CAMR8 table lookup
  Register: LocalSram7_NbiPreTableCamR8[2048] - Memory contents for a CAMR8 table lookup
  Register: LocalSram8_NbiPreTableCamR8[2048] - Memory contents for a CAMR8 table lookup
  Register: LocalSram9_NbiPreTableCamR8[2048] - Memory contents for a CAMR8 table lookup
  Register: LocalSram10_NbiPreTableCamR8[2048] - Memory contents for a CAMR8 table lookup
  Register: LocalSram11_NbiPreTableCamR8[2048] - Memory contents for a CAMR8 table lookup
  Register: LocalSram0_NbiPreTableCamR8[2048] - Memory contents for a CAMR8 table lookup
  Register: AllSharedSram_NbiPreTableCamR8[8192] - Memory contents for a CAMR8 table lookup
  Register: SharedSram0_NbiPreTableCamR8[8192] - Memory contents for a CAMR8 table lookup
  Register: SharedSram1_NbiPreTableCamR8[8192] - Memory contents for a CAMR8 table lookup
  Register: PacketSram_NbiPreTableCamR8[32768] - Memory contents for a CAMR8 table lookup
    Bits: 127:120 Result7(rw) - Result if selected data equals Data7
    Bits: 119:112 Data7(rw) - Data to match selected data to for Result7
    Bits: 111:104 Result6(rw) - Result if selected data equals Data6
    Bits: 103:96 Data6(rw) - Data to match selected data to for Result6
    Bits: 95:88 Result5(rw) - Result if selected data equals Data5
    Bits: 87:80 Data5(rw) - Data to match selected data to for Result5
    Bits: 79:72 Result4(rw) - Result if selected data equals Data4
    Bits: 71:64 Data4(rw) - Data to match selected data to for Result4
    Bits: 63:56 Result3(rw) - Result if selected data equals Data3
    Bits: 55:48 Data3(rw) - Data to match selected data to for Result3
    Bits: 47:40 Result2(rw) - Result if selected data equals Data2
    Bits: 39:32 Data2(rw) - Data to match selected data to for Result2
    Bits: 31:24 Result1(rw) - Result if selected data equals Data1
    Bits: 23:16 Data1(rw) - Data to match selected data to for Result1
    Bits: 15:8  Result0(rw) - Result if selected data equals Data0
    Bits:  7:0  Data0(rw) - Data to match selected data to for Result0
*/
#define NFP_NBI_PC_AllLocalSram_NbiPreTableCamR8(_x)         (0x00000000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram1_NbiPreTableCamR8(_x)           (0x00004000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram2_NbiPreTableCamR8(_x)           (0x00008000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram3_NbiPreTableCamR8(_x)           (0x0000c000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram4_NbiPreTableCamR8(_x)           (0x00010000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram5_NbiPreTableCamR8(_x)           (0x00014000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram6_NbiPreTableCamR8(_x)           (0x00018000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram7_NbiPreTableCamR8(_x)           (0x0001c000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram8_NbiPreTableCamR8(_x)           (0x00020000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram9_NbiPreTableCamR8(_x)           (0x00024000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram10_NbiPreTableCamR8(_x)          (0x00028000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram11_NbiPreTableCamR8(_x)          (0x0002c000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram0_NbiPreTableCamR8(_x)           (0x00030000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_AllSharedSram_NbiPreTableCamR8(_x)        (0x00080000 + (0x8 * ((_x) & 0x1fff)))
#define NFP_NBI_PC_SharedSram0_NbiPreTableCamR8(_x)          (0x00090000 + (0x8 * ((_x) & 0x1fff)))
#define NFP_NBI_PC_SharedSram1_NbiPreTableCamR8(_x)          (0x000b0000 + (0x8 * ((_x) & 0x1fff)))
#define NFP_NBI_PC_PacketSram_NbiPreTableCamR8(_x)           (0x000c0000 + (0x8 * ((_x) & 0x7fff)))
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableCamR8_Result7(_x) (((_x) & 0xff) << 120)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableCamR8_Result7_of(_x) (((_x) >> 120) & 0xff)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableCamR8_Data7(_x) (((_x) & 0xff) << 112)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableCamR8_Data7_of(_x) (((_x) >> 112) & 0xff)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableCamR8_Result6(_x) (((_x) & 0xff) << 104)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableCamR8_Result6_of(_x) (((_x) >> 104) & 0xff)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableCamR8_Data6(_x) (((_x) & 0xff) << 96)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableCamR8_Data6_of(_x) (((_x) >> 96) & 0xff)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableCamR8_Result5(_x) (((_x) & 0xff) << 88)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableCamR8_Result5_of(_x) (((_x) >> 88) & 0xff)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableCamR8_Data5(_x) (((_x) & 0xff) << 80)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableCamR8_Data5_of(_x) (((_x) >> 80) & 0xff)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableCamR8_Result4(_x) (((_x) & 0xff) << 72)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableCamR8_Result4_of(_x) (((_x) >> 72) & 0xff)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableCamR8_Data4(_x) (((_x) & 0xff) << 64)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableCamR8_Data4_of(_x) (((_x) >> 64) & 0xff)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableCamR8_Result3(_x) (((_x) & 0xff) << 56)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableCamR8_Result3_of(_x) (((_x) >> 56) & 0xff)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableCamR8_Data3(_x) (((_x) & 0xff) << 48)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableCamR8_Data3_of(_x) (((_x) >> 48) & 0xff)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableCamR8_Result2(_x) (((_x) & 0xff) << 40)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableCamR8_Result2_of(_x) (((_x) >> 40) & 0xff)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableCamR8_Data2(_x) (((_x) & 0xff) << 32)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableCamR8_Data2_of(_x) (((_x) >> 32) & 0xff)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableCamR8_Result1(_x) (((_x) & 0xff) << 24)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableCamR8_Result1_of(_x) (((_x) >> 24) & 0xff)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableCamR8_Data1(_x) (((_x) & 0xff) << 16)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableCamR8_Data1_of(_x) (((_x) >> 16) & 0xff)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableCamR8_Result0(_x) (((_x) & 0xff) << 8)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableCamR8_Result0_of(_x) (((_x) >> 8) & 0xff)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableCamR8_Data0(_x) (((_x) & 0xff) << 0)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableCamR8_Data0_of(_x) (((_x) >> 0) & 0xff)
/*----------------------------------------------------------------
  Register: AllLocalSram_NbiPreTableCamR16[2048] - Memory contents for a CAMR16 table lookup
  Register: LocalSram1_NbiPreTableCamR16[2048] - Memory contents for a CAMR16 table lookup
  Register: LocalSram2_NbiPreTableCamR16[2048] - Memory contents for a CAMR16 table lookup
  Register: LocalSram3_NbiPreTableCamR16[2048] - Memory contents for a CAMR16 table lookup
  Register: LocalSram4_NbiPreTableCamR16[2048] - Memory contents for a CAMR16 table lookup
  Register: LocalSram5_NbiPreTableCamR16[2048] - Memory contents for a CAMR16 table lookup
  Register: LocalSram6_NbiPreTableCamR16[2048] - Memory contents for a CAMR16 table lookup
  Register: LocalSram7_NbiPreTableCamR16[2048] - Memory contents for a CAMR16 table lookup
  Register: LocalSram8_NbiPreTableCamR16[2048] - Memory contents for a CAMR16 table lookup
  Register: LocalSram9_NbiPreTableCamR16[2048] - Memory contents for a CAMR16 table lookup
  Register: LocalSram10_NbiPreTableCamR16[2048] - Memory contents for a CAMR16 table lookup
  Register: LocalSram11_NbiPreTableCamR16[2048] - Memory contents for a CAMR16 table lookup
  Register: LocalSram0_NbiPreTableCamR16[2048] - Memory contents for a CAMR16 table lookup
  Register: AllSharedSram_NbiPreTableCamR16[8192] - Memory contents for a CAMR16 table lookup
  Register: SharedSram0_NbiPreTableCamR16[8192] - Memory contents for a CAMR16 table lookup
  Register: SharedSram1_NbiPreTableCamR16[8192] - Memory contents for a CAMR16 table lookup
  Register: PacketSram_NbiPreTableCamR16[32768] - Memory contents for a CAMR16 table lookup
    Bits: 127:112 Result3(rw) - Result if selected data equals Data3
    Bits: 111:96 Data3(rw) - Data to match selected data to for Result3
    Bits: 95:80 Result2(rw) - Result if selected data equals Data2
    Bits: 79:64 Data2(rw) - Data to match selected data to for Result2
    Bits: 63:48 Result1(rw) - Result if selected data equals Data1
    Bits: 47:32 Data1(rw) - Data to match selected data to for Result1
    Bits: 31:16 Result0(rw) - Result if selected data equals Data0
    Bits: 15:0  Data0(rw) - Data to match selected data to for Result0
*/
#define NFP_NBI_PC_AllLocalSram_NbiPreTableCamR16(_x)        (0x00000000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram1_NbiPreTableCamR16(_x)          (0x00004000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram2_NbiPreTableCamR16(_x)          (0x00008000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram3_NbiPreTableCamR16(_x)          (0x0000c000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram4_NbiPreTableCamR16(_x)          (0x00010000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram5_NbiPreTableCamR16(_x)          (0x00014000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram6_NbiPreTableCamR16(_x)          (0x00018000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram7_NbiPreTableCamR16(_x)          (0x0001c000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram8_NbiPreTableCamR16(_x)          (0x00020000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram9_NbiPreTableCamR16(_x)          (0x00024000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram10_NbiPreTableCamR16(_x)         (0x00028000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram11_NbiPreTableCamR16(_x)         (0x0002c000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram0_NbiPreTableCamR16(_x)          (0x00030000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_AllSharedSram_NbiPreTableCamR16(_x)       (0x00080000 + (0x8 * ((_x) & 0x1fff)))
#define NFP_NBI_PC_SharedSram0_NbiPreTableCamR16(_x)         (0x00090000 + (0x8 * ((_x) & 0x1fff)))
#define NFP_NBI_PC_SharedSram1_NbiPreTableCamR16(_x)         (0x000b0000 + (0x8 * ((_x) & 0x1fff)))
#define NFP_NBI_PC_PacketSram_NbiPreTableCamR16(_x)          (0x000c0000 + (0x8 * ((_x) & 0x7fff)))
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableCamR16_Result3(_x) (((_x) & 0xffff) << 112)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableCamR16_Result3_of(_x) (((_x) >> 112) & 0xffff)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableCamR16_Data3(_x) (((_x) & 0xffff) << 96)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableCamR16_Data3_of(_x) (((_x) >> 96) & 0xffff)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableCamR16_Result2(_x) (((_x) & 0xffff) << 80)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableCamR16_Result2_of(_x) (((_x) >> 80) & 0xffff)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableCamR16_Data2(_x) (((_x) & 0xffff) << 64)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableCamR16_Data2_of(_x) (((_x) >> 64) & 0xffff)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableCamR16_Result1(_x) (((_x) & 0xffff) << 48)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableCamR16_Result1_of(_x) (((_x) >> 48) & 0xffff)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableCamR16_Data1(_x) (((_x) & 0xffff) << 32)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableCamR16_Data1_of(_x) (((_x) >> 32) & 0xffff)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableCamR16_Result0(_x) (((_x) & 0xffff) << 16)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableCamR16_Result0_of(_x) (((_x) >> 16) & 0xffff)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableCamR16_Data0(_x) (((_x) & 0xffff) << 0)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableCamR16_Data0_of(_x) (((_x) >> 0) & 0xffff)
/*----------------------------------------------------------------
  Register: AllLocalSram_NbiPreTableCamR24[2048] - Memory contents for a CAMR24 table lookup
  Register: LocalSram1_NbiPreTableCamR24[2048] - Memory contents for a CAMR24 table lookup
  Register: LocalSram2_NbiPreTableCamR24[2048] - Memory contents for a CAMR24 table lookup
  Register: LocalSram3_NbiPreTableCamR24[2048] - Memory contents for a CAMR24 table lookup
  Register: LocalSram4_NbiPreTableCamR24[2048] - Memory contents for a CAMR24 table lookup
  Register: LocalSram5_NbiPreTableCamR24[2048] - Memory contents for a CAMR24 table lookup
  Register: LocalSram6_NbiPreTableCamR24[2048] - Memory contents for a CAMR24 table lookup
  Register: LocalSram7_NbiPreTableCamR24[2048] - Memory contents for a CAMR24 table lookup
  Register: LocalSram8_NbiPreTableCamR24[2048] - Memory contents for a CAMR24 table lookup
  Register: LocalSram9_NbiPreTableCamR24[2048] - Memory contents for a CAMR24 table lookup
  Register: LocalSram10_NbiPreTableCamR24[2048] - Memory contents for a CAMR24 table lookup
  Register: LocalSram11_NbiPreTableCamR24[2048] - Memory contents for a CAMR24 table lookup
  Register: LocalSram0_NbiPreTableCamR24[2048] - Memory contents for a CAMR24 table lookup
  Register: AllSharedSram_NbiPreTableCamR24[8192] - Memory contents for a CAMR24 table lookup
  Register: SharedSram0_NbiPreTableCamR24[8192] - Memory contents for a CAMR24 table lookup
  Register: SharedSram1_NbiPreTableCamR24[8192] - Memory contents for a CAMR24 table lookup
  Register: PacketSram_NbiPreTableCamR24[32768] - Memory contents for a CAMR24 table lookup
    Bits: 125:108 Result2(rw) - Result if selected data equals Data2
    Bits: 107:90 Result1(rw) - Result if selected data equals Data1
    Bits: 89:72 Result0(rw) - Result if selected data equals Data0
    Bits: 71:48 Data2(rw) - Data to match selected data to for Result2
    Bits: 47:24 Data1(rw) - Data to match selected data to for Result1
    Bits: 23:0  Data0(rw) - Data to match selected data to for Result0
*/
#define NFP_NBI_PC_AllLocalSram_NbiPreTableCamR24(_x)        (0x00000000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram1_NbiPreTableCamR24(_x)          (0x00004000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram2_NbiPreTableCamR24(_x)          (0x00008000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram3_NbiPreTableCamR24(_x)          (0x0000c000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram4_NbiPreTableCamR24(_x)          (0x00010000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram5_NbiPreTableCamR24(_x)          (0x00014000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram6_NbiPreTableCamR24(_x)          (0x00018000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram7_NbiPreTableCamR24(_x)          (0x0001c000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram8_NbiPreTableCamR24(_x)          (0x00020000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram9_NbiPreTableCamR24(_x)          (0x00024000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram10_NbiPreTableCamR24(_x)         (0x00028000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram11_NbiPreTableCamR24(_x)         (0x0002c000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram0_NbiPreTableCamR24(_x)          (0x00030000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_AllSharedSram_NbiPreTableCamR24(_x)       (0x00080000 + (0x8 * ((_x) & 0x1fff)))
#define NFP_NBI_PC_SharedSram0_NbiPreTableCamR24(_x)         (0x00090000 + (0x8 * ((_x) & 0x1fff)))
#define NFP_NBI_PC_SharedSram1_NbiPreTableCamR24(_x)         (0x000b0000 + (0x8 * ((_x) & 0x1fff)))
#define NFP_NBI_PC_PacketSram_NbiPreTableCamR24(_x)          (0x000c0000 + (0x8 * ((_x) & 0x7fff)))
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableCamR24_Result2(_x) (((_x) & 0x3ffff) << 108)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableCamR24_Result2_of(_x) (((_x) >> 108) & 0x3ffff)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableCamR24_Result1(_x) (((_x) & 0x3ffff) << 90)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableCamR24_Result1_of(_x) (((_x) >> 90) & 0x3ffff)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableCamR24_Result0(_x) (((_x) & 0x3ffff) << 72)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableCamR24_Result0_of(_x) (((_x) >> 72) & 0x3ffff)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableCamR24_Data2(_x) (((_x) & 0xffffff) << 48)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableCamR24_Data2_of(_x) (((_x) >> 48) & 0xffffff)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableCamR24_Data1(_x) (((_x) & 0xffffff) << 24)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableCamR24_Data1_of(_x) (((_x) >> 24) & 0xffffff)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableCamR24_Data0(_x) (((_x) & 0xffffff) << 0)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableCamR24_Data0_of(_x) (((_x) >> 0) & 0xffffff)
/*----------------------------------------------------------------
  Register: AllLocalSram_NbiPreTableCamR32[2048] - Memory contents for a CAMR32 table lookup
  Register: LocalSram1_NbiPreTableCamR32[2048] - Memory contents for a CAMR32 table lookup
  Register: LocalSram2_NbiPreTableCamR32[2048] - Memory contents for a CAMR32 table lookup
  Register: LocalSram3_NbiPreTableCamR32[2048] - Memory contents for a CAMR32 table lookup
  Register: LocalSram4_NbiPreTableCamR32[2048] - Memory contents for a CAMR32 table lookup
  Register: LocalSram5_NbiPreTableCamR32[2048] - Memory contents for a CAMR32 table lookup
  Register: LocalSram6_NbiPreTableCamR32[2048] - Memory contents for a CAMR32 table lookup
  Register: LocalSram7_NbiPreTableCamR32[2048] - Memory contents for a CAMR32 table lookup
  Register: LocalSram8_NbiPreTableCamR32[2048] - Memory contents for a CAMR32 table lookup
  Register: LocalSram9_NbiPreTableCamR32[2048] - Memory contents for a CAMR32 table lookup
  Register: LocalSram10_NbiPreTableCamR32[2048] - Memory contents for a CAMR32 table lookup
  Register: LocalSram11_NbiPreTableCamR32[2048] - Memory contents for a CAMR32 table lookup
  Register: LocalSram0_NbiPreTableCamR32[2048] - Memory contents for a CAMR32 table lookup
  Register: AllSharedSram_NbiPreTableCamR32[8192] - Memory contents for a CAMR32 table lookup
  Register: SharedSram0_NbiPreTableCamR32[8192] - Memory contents for a CAMR32 table lookup
  Register: SharedSram1_NbiPreTableCamR32[8192] - Memory contents for a CAMR32 table lookup
  Register: PacketSram_NbiPreTableCamR32[32768] - Memory contents for a CAMR32 table lookup
    Bits: 127:96 Result1(rw) - Result if selected data equals Data1
    Bits: 95:64 Result0(rw) - Result if selected data equals Data0
    Bits: 63:32 Data1(rw) - Data to match selected data to for Result1
    Bits: 31:0  Data0(rw) - Data to match selected data to for Result0
*/
#define NFP_NBI_PC_AllLocalSram_NbiPreTableCamR32(_x)        (0x00000000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram1_NbiPreTableCamR32(_x)          (0x00004000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram2_NbiPreTableCamR32(_x)          (0x00008000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram3_NbiPreTableCamR32(_x)          (0x0000c000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram4_NbiPreTableCamR32(_x)          (0x00010000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram5_NbiPreTableCamR32(_x)          (0x00014000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram6_NbiPreTableCamR32(_x)          (0x00018000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram7_NbiPreTableCamR32(_x)          (0x0001c000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram8_NbiPreTableCamR32(_x)          (0x00020000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram9_NbiPreTableCamR32(_x)          (0x00024000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram10_NbiPreTableCamR32(_x)         (0x00028000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram11_NbiPreTableCamR32(_x)         (0x0002c000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram0_NbiPreTableCamR32(_x)          (0x00030000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_AllSharedSram_NbiPreTableCamR32(_x)       (0x00080000 + (0x8 * ((_x) & 0x1fff)))
#define NFP_NBI_PC_SharedSram0_NbiPreTableCamR32(_x)         (0x00090000 + (0x8 * ((_x) & 0x1fff)))
#define NFP_NBI_PC_SharedSram1_NbiPreTableCamR32(_x)         (0x000b0000 + (0x8 * ((_x) & 0x1fff)))
#define NFP_NBI_PC_PacketSram_NbiPreTableCamR32(_x)          (0x000c0000 + (0x8 * ((_x) & 0x7fff)))
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableCamR32_Result1(_x) (((_x) & 0xffffffff) << 96)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableCamR32_Result1_of(_x) (((_x) >> 96) & 0xffffffff)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableCamR32_Result0(_x) (((_x) & 0xffffffff) << 64)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableCamR32_Result0_of(_x) (((_x) >> 64) & 0xffffffff)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableCamR32_Data1(_x) (((_x) & 0xffffffff) << 32)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableCamR32_Data1_of(_x) (((_x) >> 32) & 0xffffffff)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableCamR32_Data0(_x) (((_x) & 0xffffffff) << 0)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableCamR32_Data0_of(_x) (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: AllLocalSram_NbiPreTableTCamR8[2048] - Memory contents for a TCAMR8 table lookup
  Register: LocalSram1_NbiPreTableTCamR8[2048] - Memory contents for a TCAMR8 table lookup
  Register: LocalSram2_NbiPreTableTCamR8[2048] - Memory contents for a TCAMR8 table lookup
  Register: LocalSram3_NbiPreTableTCamR8[2048] - Memory contents for a TCAMR8 table lookup
  Register: LocalSram4_NbiPreTableTCamR8[2048] - Memory contents for a TCAMR8 table lookup
  Register: LocalSram5_NbiPreTableTCamR8[2048] - Memory contents for a TCAMR8 table lookup
  Register: LocalSram6_NbiPreTableTCamR8[2048] - Memory contents for a TCAMR8 table lookup
  Register: LocalSram7_NbiPreTableTCamR8[2048] - Memory contents for a TCAMR8 table lookup
  Register: LocalSram8_NbiPreTableTCamR8[2048] - Memory contents for a TCAMR8 table lookup
  Register: LocalSram9_NbiPreTableTCamR8[2048] - Memory contents for a TCAMR8 table lookup
  Register: LocalSram10_NbiPreTableTCamR8[2048] - Memory contents for a TCAMR8 table lookup
  Register: LocalSram11_NbiPreTableTCamR8[2048] - Memory contents for a TCAMR8 table lookup
  Register: LocalSram0_NbiPreTableTCamR8[2048] - Memory contents for a TCAMR8 table lookup
  Register: AllSharedSram_NbiPreTableTCamR8[8192] - Memory contents for a TCAMR8 table lookup
  Register: SharedSram0_NbiPreTableTCamR8[8192] - Memory contents for a TCAMR8 table lookup
  Register: SharedSram1_NbiPreTableTCamR8[8192] - Memory contents for a TCAMR8 table lookup
  Register: PacketSram_NbiPreTableTCamR8[32768] - Memory contents for a TCAMR8 table lookup
    Bits: 127:112 Result3(rw) - Result if selected data AND Mask3 equals Value3 (if (Value3 and (~Mask3))!=0 then also at least one bit of Data and (Value3 and (~Mask3)) must be set)
    Bits: 111:104 Mask3(rw) - Mask for Value3 and Data3
    Bits: 103:96 Value3(rw) - Value to match with Mask3 and Data3 to provide for Result3
    Bits: 95:80 Result2(rw) - Result if selected data AND Mask2 equals Value2 (if (Value2 and (~Mask2))!=0 then also at least one bit of Data and (Value2 and (~Mask2)) must be set)
    Bits: 79:72 Mask2(rw) - Mask for Value2 and Data2
    Bits: 71:64 Value2(rw) - Value to match with Mask2 and Data2 to provide for Result2
    Bits: 63:48 Result1(rw) - Result if selected data AND Mask1 equals Value1 (if (Value1 and (~Mask1))!=0 then also at least one bit of Data and (Value1 and (~Mask1)) must be set)
    Bits: 47:40 Mask1(rw) - Mask for Value1 and Data1
    Bits: 39:32 Value1(rw) - Value to match with Mask1 and Data1 to provide for Result1
    Bits: 31:16 Result0(rw) - Result if selected data AND Mask0 equals Value0 (if (Value0 and (~Mask0))!=0 then also at least one bit of Data and (Value0 and (~Mask0)) must be set)
    Bits: 15:8  Mask0(rw) - Mask for Value0 and Data0
    Bits:  7:0  Value0(rw) - Value to match with Mask0 and Data0 to provide for Result0
*/
#define NFP_NBI_PC_AllLocalSram_NbiPreTableTCamR8(_x)        (0x00000000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram1_NbiPreTableTCamR8(_x)          (0x00004000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram2_NbiPreTableTCamR8(_x)          (0x00008000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram3_NbiPreTableTCamR8(_x)          (0x0000c000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram4_NbiPreTableTCamR8(_x)          (0x00010000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram5_NbiPreTableTCamR8(_x)          (0x00014000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram6_NbiPreTableTCamR8(_x)          (0x00018000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram7_NbiPreTableTCamR8(_x)          (0x0001c000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram8_NbiPreTableTCamR8(_x)          (0x00020000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram9_NbiPreTableTCamR8(_x)          (0x00024000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram10_NbiPreTableTCamR8(_x)         (0x00028000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram11_NbiPreTableTCamR8(_x)         (0x0002c000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram0_NbiPreTableTCamR8(_x)          (0x00030000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_AllSharedSram_NbiPreTableTCamR8(_x)       (0x00080000 + (0x8 * ((_x) & 0x1fff)))
#define NFP_NBI_PC_SharedSram0_NbiPreTableTCamR8(_x)         (0x00090000 + (0x8 * ((_x) & 0x1fff)))
#define NFP_NBI_PC_SharedSram1_NbiPreTableTCamR8(_x)         (0x000b0000 + (0x8 * ((_x) & 0x1fff)))
#define NFP_NBI_PC_PacketSram_NbiPreTableTCamR8(_x)          (0x000c0000 + (0x8 * ((_x) & 0x7fff)))
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableTCamR8_Result3(_x) (((_x) & 0xffff) << 112)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableTCamR8_Result3_of(_x) (((_x) >> 112) & 0xffff)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableTCamR8_Mask3(_x) (((_x) & 0xff) << 104)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableTCamR8_Mask3_of(_x) (((_x) >> 104) & 0xff)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableTCamR8_Value3(_x) (((_x) & 0xff) << 96)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableTCamR8_Value3_of(_x) (((_x) >> 96) & 0xff)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableTCamR8_Result2(_x) (((_x) & 0xffff) << 80)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableTCamR8_Result2_of(_x) (((_x) >> 80) & 0xffff)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableTCamR8_Mask2(_x) (((_x) & 0xff) << 72)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableTCamR8_Mask2_of(_x) (((_x) >> 72) & 0xff)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableTCamR8_Value2(_x) (((_x) & 0xff) << 64)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableTCamR8_Value2_of(_x) (((_x) >> 64) & 0xff)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableTCamR8_Result1(_x) (((_x) & 0xffff) << 48)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableTCamR8_Result1_of(_x) (((_x) >> 48) & 0xffff)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableTCamR8_Mask1(_x) (((_x) & 0xff) << 40)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableTCamR8_Mask1_of(_x) (((_x) >> 40) & 0xff)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableTCamR8_Value1(_x) (((_x) & 0xff) << 32)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableTCamR8_Value1_of(_x) (((_x) >> 32) & 0xff)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableTCamR8_Result0(_x) (((_x) & 0xffff) << 16)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableTCamR8_Result0_of(_x) (((_x) >> 16) & 0xffff)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableTCamR8_Mask0(_x) (((_x) & 0xff) << 8)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableTCamR8_Mask0_of(_x) (((_x) >> 8) & 0xff)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableTCamR8_Value0(_x) (((_x) & 0xff) << 0)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableTCamR8_Value0_of(_x) (((_x) >> 0) & 0xff)
/*----------------------------------------------------------------
  Register: AllLocalSram_NbiPreTableTCamR16[2048] - Memory contents for a TCAMR16 table lookup
  Register: LocalSram1_NbiPreTableTCamR16[2048] - Memory contents for a TCAMR16 table lookup
  Register: LocalSram2_NbiPreTableTCamR16[2048] - Memory contents for a TCAMR16 table lookup
  Register: LocalSram3_NbiPreTableTCamR16[2048] - Memory contents for a TCAMR16 table lookup
  Register: LocalSram4_NbiPreTableTCamR16[2048] - Memory contents for a TCAMR16 table lookup
  Register: LocalSram5_NbiPreTableTCamR16[2048] - Memory contents for a TCAMR16 table lookup
  Register: LocalSram6_NbiPreTableTCamR16[2048] - Memory contents for a TCAMR16 table lookup
  Register: LocalSram7_NbiPreTableTCamR16[2048] - Memory contents for a TCAMR16 table lookup
  Register: LocalSram8_NbiPreTableTCamR16[2048] - Memory contents for a TCAMR16 table lookup
  Register: LocalSram9_NbiPreTableTCamR16[2048] - Memory contents for a TCAMR16 table lookup
  Register: LocalSram10_NbiPreTableTCamR16[2048] - Memory contents for a TCAMR16 table lookup
  Register: LocalSram11_NbiPreTableTCamR16[2048] - Memory contents for a TCAMR16 table lookup
  Register: LocalSram0_NbiPreTableTCamR16[2048] - Memory contents for a TCAMR16 table lookup
  Register: AllSharedSram_NbiPreTableTCamR16[8192] - Memory contents for a TCAMR16 table lookup
  Register: SharedSram0_NbiPreTableTCamR16[8192] - Memory contents for a TCAMR16 table lookup
  Register: SharedSram1_NbiPreTableTCamR16[8192] - Memory contents for a TCAMR16 table lookup
  Register: PacketSram_NbiPreTableTCamR16[32768] - Memory contents for a TCAMR16 table lookup
    Bits: 127:96 Result1(rw) - Result if selected data AND Mask1 equals Value1 (if (Value1 and (~Mask1))!=0 then also at least one bit of Data and (Value1 and (~Mask1)) must be set)
    Bits: 95:80 Mask1(rw) - Mask for Value1 and Data1
    Bits: 79:64 Value1(rw) - Value to match with Mask1 and Data1 to provide for Result1
    Bits: 63:32 Result0(rw) - Result if selected data AND Mask0 equals Value0 (if (Value0 and (~Mask0))!=0 then also at least one bit of Data and (Value0 and (~Mask0)) must be set)
    Bits: 31:16 Mask0(rw) - Mask for Value0 and Data0
    Bits: 15:0  Value0(rw) - Value to match with Mask0 and Data0 to provide for Result0
*/
#define NFP_NBI_PC_AllLocalSram_NbiPreTableTCamR16(_x)       (0x00000000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram1_NbiPreTableTCamR16(_x)         (0x00004000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram2_NbiPreTableTCamR16(_x)         (0x00008000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram3_NbiPreTableTCamR16(_x)         (0x0000c000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram4_NbiPreTableTCamR16(_x)         (0x00010000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram5_NbiPreTableTCamR16(_x)         (0x00014000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram6_NbiPreTableTCamR16(_x)         (0x00018000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram7_NbiPreTableTCamR16(_x)         (0x0001c000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram8_NbiPreTableTCamR16(_x)         (0x00020000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram9_NbiPreTableTCamR16(_x)         (0x00024000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram10_NbiPreTableTCamR16(_x)        (0x00028000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram11_NbiPreTableTCamR16(_x)        (0x0002c000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram0_NbiPreTableTCamR16(_x)         (0x00030000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_AllSharedSram_NbiPreTableTCamR16(_x)      (0x00080000 + (0x8 * ((_x) & 0x1fff)))
#define NFP_NBI_PC_SharedSram0_NbiPreTableTCamR16(_x)        (0x00090000 + (0x8 * ((_x) & 0x1fff)))
#define NFP_NBI_PC_SharedSram1_NbiPreTableTCamR16(_x)        (0x000b0000 + (0x8 * ((_x) & 0x1fff)))
#define NFP_NBI_PC_PacketSram_NbiPreTableTCamR16(_x)         (0x000c0000 + (0x8 * ((_x) & 0x7fff)))
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableTCamR16_Result1(_x) (((_x) & 0xffffffff) << 96)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableTCamR16_Result1_of(_x) (((_x) >> 96) & 0xffffffff)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableTCamR16_Mask1(_x) (((_x) & 0xffff) << 80)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableTCamR16_Mask1_of(_x) (((_x) >> 80) & 0xffff)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableTCamR16_Value1(_x) (((_x) & 0xffff) << 64)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableTCamR16_Value1_of(_x) (((_x) >> 64) & 0xffff)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableTCamR16_Result0(_x) (((_x) & 0xffffffff) << 32)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableTCamR16_Result0_of(_x) (((_x) >> 32) & 0xffffffff)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableTCamR16_Mask0(_x) (((_x) & 0xffff) << 16)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableTCamR16_Mask0_of(_x) (((_x) >> 16) & 0xffff)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableTCamR16_Value0(_x) (((_x) & 0xffff) << 0)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableTCamR16_Value0_of(_x) (((_x) >> 0) & 0xffff)
/*----------------------------------------------------------------
  Register: AllLocalSram_NbiPreTableTCamR24[2048] - Memory contents for a TCAMR24 table lookup
  Register: LocalSram1_NbiPreTableTCamR24[2048] - Memory contents for a TCAMR24 table lookup
  Register: LocalSram2_NbiPreTableTCamR24[2048] - Memory contents for a TCAMR24 table lookup
  Register: LocalSram3_NbiPreTableTCamR24[2048] - Memory contents for a TCAMR24 table lookup
  Register: LocalSram4_NbiPreTableTCamR24[2048] - Memory contents for a TCAMR24 table lookup
  Register: LocalSram5_NbiPreTableTCamR24[2048] - Memory contents for a TCAMR24 table lookup
  Register: LocalSram6_NbiPreTableTCamR24[2048] - Memory contents for a TCAMR24 table lookup
  Register: LocalSram7_NbiPreTableTCamR24[2048] - Memory contents for a TCAMR24 table lookup
  Register: LocalSram8_NbiPreTableTCamR24[2048] - Memory contents for a TCAMR24 table lookup
  Register: LocalSram9_NbiPreTableTCamR24[2048] - Memory contents for a TCAMR24 table lookup
  Register: LocalSram10_NbiPreTableTCamR24[2048] - Memory contents for a TCAMR24 table lookup
  Register: LocalSram11_NbiPreTableTCamR24[2048] - Memory contents for a TCAMR24 table lookup
  Register: LocalSram0_NbiPreTableTCamR24[2048] - Memory contents for a TCAMR24 table lookup
  Register: AllSharedSram_NbiPreTableTCamR24[8192] - Memory contents for a TCAMR24 table lookup
  Register: SharedSram0_NbiPreTableTCamR24[8192] - Memory contents for a TCAMR24 table lookup
  Register: SharedSram1_NbiPreTableTCamR24[8192] - Memory contents for a TCAMR24 table lookup
  Register: PacketSram_NbiPreTableTCamR24[32768] - Memory contents for a TCAMR24 table lookup
    Bits: 127:112 Result1(rw) - Result if selected data AND Mask1 equals Value1 (if (Value1 and (~Mask1))!=0 then also at least one bit of Data and (Value1 and (~Mask1)) must be set)
    Bits: 111:88 Mask1(rw) - Mask for Value1 and Data1
    Bits: 87:64 Value1(rw) - Value to match with Mask1 and Data1 to provide for Result1
    Bits: 63:48 Result0(rw) - Result if selected data AND Mask0 equals Value0 (if (Value0 and (~Mask0))!=0 then also at least one bit of Data and (Value0 and (~Mask0)) must be set)
    Bits: 47:24 Mask0(rw) - Mask for Value0 and Data0
    Bits: 23:0  Value0(rw) - Value to match with Mask0 and Data0 to provide for Result0
*/
#define NFP_NBI_PC_AllLocalSram_NbiPreTableTCamR24(_x)       (0x00000000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram1_NbiPreTableTCamR24(_x)         (0x00004000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram2_NbiPreTableTCamR24(_x)         (0x00008000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram3_NbiPreTableTCamR24(_x)         (0x0000c000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram4_NbiPreTableTCamR24(_x)         (0x00010000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram5_NbiPreTableTCamR24(_x)         (0x00014000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram6_NbiPreTableTCamR24(_x)         (0x00018000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram7_NbiPreTableTCamR24(_x)         (0x0001c000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram8_NbiPreTableTCamR24(_x)         (0x00020000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram9_NbiPreTableTCamR24(_x)         (0x00024000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram10_NbiPreTableTCamR24(_x)        (0x00028000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram11_NbiPreTableTCamR24(_x)        (0x0002c000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_LocalSram0_NbiPreTableTCamR24(_x)         (0x00030000 + (0x8 * ((_x) & 0x7ff)))
#define NFP_NBI_PC_AllSharedSram_NbiPreTableTCamR24(_x)      (0x00080000 + (0x8 * ((_x) & 0x1fff)))
#define NFP_NBI_PC_SharedSram0_NbiPreTableTCamR24(_x)        (0x00090000 + (0x8 * ((_x) & 0x1fff)))
#define NFP_NBI_PC_SharedSram1_NbiPreTableTCamR24(_x)        (0x000b0000 + (0x8 * ((_x) & 0x1fff)))
#define NFP_NBI_PC_PacketSram_NbiPreTableTCamR24(_x)         (0x000c0000 + (0x8 * ((_x) & 0x7fff)))
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableTCamR24_Result1(_x) (((_x) & 0xffff) << 112)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableTCamR24_Result1_of(_x) (((_x) >> 112) & 0xffff)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableTCamR24_Mask1(_x) (((_x) & 0xffffff) << 88)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableTCamR24_Mask1_of(_x) (((_x) >> 88) & 0xffffff)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableTCamR24_Value1(_x) (((_x) & 0xffffff) << 64)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableTCamR24_Value1_of(_x) (((_x) >> 64) & 0xffffff)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableTCamR24_Result0(_x) (((_x) & 0xffff) << 48)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableTCamR24_Result0_of(_x) (((_x) >> 48) & 0xffff)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableTCamR24_Mask0(_x) (((_x) & 0xffffff) << 24)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableTCamR24_Mask0_of(_x) (((_x) >> 24) & 0xffffff)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableTCamR24_Value0(_x) (((_x) & 0xffffff) << 0)
#define   NFP_NBI_PC_AllLocalSram_NbiPreTableTCamR24_Value0_of(_x) (((_x) >> 0) & 0xffffff)
/*----------------------------------------------------------------
  Register: PicoengineSetup - Initial state of picoengine CSRs when next allocated
    Bits: 25    HashPremult(rw) - Set the initial value for the picoengine hash preaddition of data to residue on hash generation
    Bits: 24    HashSboxEnable(rw) - Set the initial value for the picoengine hash enabling of SBOXes
    Bits: 23:20 HashSbox(rw) - Set the initial value for the picoengine hash number of SBOXes to use
    Bits: 19:16 HashMult(rw) - Set the initial value for the picoengine hash multiplier (for enable multiply-mod-2 by 4, 36, 53, 63)
    Bits: 11:10 Predicate(rw) - Set to specify amount of packet data prefetch to do; can be changed by the picoengine as it runs
    Bits:  9:8  PktPref(rw) - Set to specify amount of packet data prefetch to do; can be changed by the picoengine as it runs
    Bits:  7:6  SequenceReplace(rw) - Set to replace metadata octets[2..3] or octets[2..5] with sequence number from a pool specified by metadata[1]
    Bits:  5    PktPtrOp(rw) - Initial value for picoengine to use for the default packet pointer operation for pkt ALU operations
    Bits:  4    Fetch256(rw) - Initial code load should be a 256-bit fetch; this is also loaded into the picoengine (BUT NOT USED THERE)
    Bits:  3:0  StackPtr(rw) - Initial value for picoengine execution for the stack pointer; this is used whenever a picoengine program is initiated
*/
#define NFP_NBI_PCX_PE_PicoengineSetup                       0x00000004
#define   NFP_NBI_PCX_PE_PicoengineSetup_HashPremult         BIT(25)
#define   NFP_NBI_PCX_PE_PicoengineSetup_HashSboxEnable      BIT(24)
#define   NFP_NBI_PCX_PE_PicoengineSetup_HashSbox(_x)        (((_x) & 0xf) << 20)
#define   NFP_NBI_PCX_PE_PicoengineSetup_HashSbox_of(_x)     (((_x) >> 20) & 0xf)
#define   NFP_NBI_PCX_PE_PicoengineSetup_HashMult(_x)        (((_x) & 0xf) << 16)
#define   NFP_NBI_PCX_PE_PicoengineSetup_HashMult_of(_x)     (((_x) >> 16) & 0xf)
#define   NFP_NBI_PCX_PE_PicoengineSetup_Predicate(_x)       (((_x) & 0x3) << 10)
#define   NFP_NBI_PCX_PE_PicoengineSetup_Predicate_of(_x)    (((_x) >> 10) & 0x3)
#define     NFP_NBI_PCX_PE_PicoengineSetup_Predicate_ALWAYS  (0)
#define     NFP_NBI_PCX_PE_PicoengineSetup_Predicate_ZS      (1)
#define     NFP_NBI_PCX_PE_PicoengineSetup_Predicate_CS      (2)
#define     NFP_NBI_PCX_PE_PicoengineSetup_Predicate_GT      (3)
#define   NFP_NBI_PCX_PE_PicoengineSetup_PktPref(_x)         (((_x) & 0x3) << 8)
#define   NFP_NBI_PCX_PE_PicoengineSetup_PktPref_of(_x)      (((_x) >> 8) & 0x3)
#define     NFP_NBI_PCX_PE_PicoengineSetup_PktPref_NONE      (0)
#define     NFP_NBI_PCX_PE_PicoengineSetup_PktPref_PREF16    (1)
#define     NFP_NBI_PCX_PE_PicoengineSetup_PktPref_PREF32    (2)
#define     NFP_NBI_PCX_PE_PicoengineSetup_PktPref_RESVD     (3)
#define   NFP_NBI_PCX_PE_PicoengineSetup_SequenceReplace(_x) (((_x) & 0x3) << 6)
#define   NFP_NBI_PCX_PE_PicoengineSetup_SequenceReplace_of(_x) (((_x) >> 6) & 0x3)
#define     NFP_NBI_PCX_PE_PicoengineSetup_SequenceReplace_NONE (0)
#define     NFP_NBI_PCX_PE_PicoengineSetup_SequenceReplace_SEQ16 (1)
#define     NFP_NBI_PCX_PE_PicoengineSetup_SequenceReplace_RESVD (2)
#define     NFP_NBI_PCX_PE_PicoengineSetup_SequenceReplace_SEQ32 (3)
#define   NFP_NBI_PCX_PE_PicoengineSetup_PktPtrOp            BIT(5)
#define   NFP_NBI_PCX_PE_PicoengineSetup_Fetch256            BIT(4)
#define   NFP_NBI_PCX_PE_PicoengineSetup_StackPtr(_x)        (((_x) & 0xf) << 0)
#define   NFP_NBI_PCX_PE_PicoengineSetup_StackPtr_of(_x)     (((_x) >> 0) & 0xf)
/*----------------------------------------------------------------
  Register: PicoengineRunControl - Controls execution of the picoengines
    Bits: 29:28 SMem(rw) - One bit per picoengine shared memory to enable the power
    Bits: 27:16 GroupMem(rw) - One bit per picoengine group to enable the power to the memory in those groups
    Bits: 15:4  GroupClock(rw) - One bit per picoengine group to enable the clocks to those groups
    Bits:  2    IgnoreResult(rw) - If set, ignore result of any picoengine completion (i.e. do not pass to tunnel or the NBI DMA on finish)
    Bits:  1:0  Alloc(rw) - Enable allocation of picoengines
*/
#define NFP_NBI_PCX_PE_PicoengineRunControl                  0x00000008
#define   NFP_NBI_PCX_PE_PicoengineRunControl_SMem(_x)       (((_x) & 0x3) << 28)
#define   NFP_NBI_PCX_PE_PicoengineRunControl_SMem_of(_x)    (((_x) >> 28) & 0x3)
#define   NFP_NBI_PCX_PE_PicoengineRunControl_GroupMem(_x)   (((_x) & 0xfff) << 16)
#define   NFP_NBI_PCX_PE_PicoengineRunControl_GroupMem_of(_x) (((_x) >> 16) & 0xfff)
#define   NFP_NBI_PCX_PE_PicoengineRunControl_GroupClock(_x) (((_x) & 0xfff) << 4)
#define   NFP_NBI_PCX_PE_PicoengineRunControl_GroupClock_of(_x) (((_x) >> 4) & 0xfff)
#define   NFP_NBI_PCX_PE_PicoengineRunControl_IgnoreResult   BIT(2)
#define   NFP_NBI_PCX_PE_PicoengineRunControl_Alloc(_x)      (((_x) & 0x3) << 0)
#define   NFP_NBI_PCX_PE_PicoengineRunControl_Alloc_of(_x)   (((_x) >> 0) & 0x3)
#define     NFP_NBI_PCX_PE_PicoengineRunControl_Alloc_NONE   (0)
#define     NFP_NBI_PCX_PE_PicoengineRunControl_Alloc_ALLOC  (1)
#define     NFP_NBI_PCX_PE_PicoengineRunControl_Alloc_RESVD  (2)
#define     NFP_NBI_PCX_PE_PicoengineRunControl_Alloc_ONESHOT (3)
/*----------------------------------------------------------------
  Register: TableExtend - Extension of table configuration
    Bits:  0    LookupExt(rw) - Top bit of table lookup, used when performing LMEM or SMEM table configurations through the XPB
*/
#define NFP_NBI_PCX_PE_TableExtend                           0x0000000c
#define   NFP_NBI_PCX_PE_TableExtend_LookupExt               BIT(0)
/*----------------------------------------------------------------
  Register: ActiveSet0Low - Bit-mask indicating which of the first 32 picoengines are actively classifying (i.e. neither ready with finished classification nor idle) using table set 0
  Register: ActiveSet1Low - Bit-mask indicating which of the first 32 picoengines are actively classifying (i.e. neither ready with finished classification nor idle) using table set 1
    Bits: 31:0  Status(ro) - One bit per picoengine indicating that the picoengine is actively classifying with a particular TableSet value; actively classifying means not idle and not presenting a result
*/
#define NFP_NBI_PCX_PE_ActiveSet0Low                         0x00000080
#define NFP_NBI_PCX_PE_ActiveSet1Low                         0x00000088
#define   NFP_NBI_PCX_PE_NbiPrePicoengActiveLow_Status(_x)   (((_x) & 0xffffffff) << 0)
#define   NFP_NBI_PCX_PE_NbiPrePicoengActiveLow_Status_of(_x) (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: ActiveSet0High - Bit-mask indicating which of the last 16 picoengines are actively classifying (i.e. neither ready with finished classification nor idle) using table set 0
  Register: ActiveSet1High - Bit-mask indicating which of the last 16 picoengines are actively classifying (i.e. neither ready with finished classification nor idle) using table set 1
    Bits: 15:0  Status(ro) - One bit per picoengine indicating that the picoengine is actively classifying with a particular TableSet value; actively classifying means not idle and not presenting a result
*/
#define NFP_NBI_PCX_PE_ActiveSet0High                        0x00000084
#define NFP_NBI_PCX_PE_ActiveSet1High                        0x0000008c
#define   NFP_NBI_PCX_PE_NbiPrePicoengActiveHigh_Status(_x)  (((_x) & 0xffff) << 0)
#define   NFP_NBI_PCX_PE_NbiPrePicoengActiveHigh_Status_of(_x) (((_x) >> 0) & 0xffff)
/*----------------------------------------------------------------
  Register: ClassifiedSmall - Statistics register for packets of 256 bytes or fewer that have completed classification
    Bits: 31    Saturated(rc) - If set then the number of 'small' packets that have fully completed classification (and passed to DMA) since the last read of this register exceeds 0x7fffffff
    Bits: 30:0  Count(rc) - Count of 'small' packets that have fully completed classification (and passed to DMA) since the last read of this register
*/
#define NFP_NBI_PCX_PE_ClassifiedSmall                       0x000000c0
#define   NFP_NBI_PCX_PE_ClassifiedSmall_Saturated           BIT(31)
#define   NFP_NBI_PCX_PE_ClassifiedSmall_Count(_x)           (((_x) & 0x7fffffff) << 0)
#define   NFP_NBI_PCX_PE_ClassifiedSmall_Count_of(_x)        (((_x) >> 0) & 0x7fffffff)
/*----------------------------------------------------------------
  Register: ClassifiedLarge - Statistics register for packets of larger than 256 bytes that have completed classification
    Bits: 31    Saturated(rc) - If set then the number of 'large' packets that have fully completed classification (and passed to DMA) since the last read of this register exceeds 0x7fffffff
    Bits: 30:0  Count(rc) - Count of 'large' packets that have fully completed classification (and passed to DMA) since the last read of this register
*/
#define NFP_NBI_PCX_PE_ClassifiedLarge                       0x000000c4
#define   NFP_NBI_PCX_PE_ClassifiedLarge_Saturated           BIT(31)
#define   NFP_NBI_PCX_PE_ClassifiedLarge_Count(_x)           (((_x) & 0x7fffffff) << 0)
#define   NFP_NBI_PCX_PE_ClassifiedLarge_Count_of(_x)        (((_x) >> 0) & 0x7fffffff)
/*----------------------------------------------------------------
  Register: Tunnel - Statistics register for packets that have been recirculated as tunneled as part of their classification
    Bits: 31    Saturated(rc) - If set then the number of packets passed through the tunnel path since the last read of this register exceeds 0x7fffffff
    Bits: 30:0  Count(rc) - Count of packets passed through the tunnel path since the last read of this register
*/
#define NFP_NBI_PCX_PE_Tunnel                                0x000000c8
#define   NFP_NBI_PCX_PE_Tunnel_Saturated                    BIT(31)
#define   NFP_NBI_PCX_PE_Tunnel_Count(_x)                    (((_x) & 0x7fffffff) << 0)
#define   NFP_NBI_PCX_PE_Tunnel_Count_of(_x)                 (((_x) >> 0) & 0x7fffffff)
/*----------------------------------------------------------------
  Register: LocalData[8] - Local SRAM data table description X
    Bits: 31:22 Base1(wo) - Base address for table set 1 accesses
    Bits: 21:12 Base0(wo) - Base address for table set 0 accesses
    Bits: 11:9  TableSize(wo) - Number of entries in the table - effects masking of input data, and offset into the table
    Bits:  8:4  Select(wo) - Starting bit divide by 2 for determining offset into table for an access
    Bits:  3:0  Lookup(wo) - Method of interpreting the SRAM data read, to produce a result for the lookup
*/
#define NFP_NBI_PCX_PE_LocalData(_x)                         (0x00000100 + (0x4 * ((_x) & 0x7)))
#define   NFP_NBI_PCX_PE_LocalData_Base1(_x)                 (((_x) & 0x3ff) << 22)
#define   NFP_NBI_PCX_PE_LocalData_Base1_of(_x)              (((_x) >> 22) & 0x3ff)
#define   NFP_NBI_PCX_PE_LocalData_Base0(_x)                 (((_x) & 0x3ff) << 12)
#define   NFP_NBI_PCX_PE_LocalData_Base0_of(_x)              (((_x) >> 12) & 0x3ff)
#define   NFP_NBI_PCX_PE_LocalData_TableSize(_x)             (((_x) & 0x7) << 9)
#define   NFP_NBI_PCX_PE_LocalData_TableSize_of(_x)          (((_x) >> 9) & 0x7)
#define     NFP_NBI_PCX_PE_LocalData_TableSize_1             (0)
#define     NFP_NBI_PCX_PE_LocalData_TableSize_4             (1)
#define     NFP_NBI_PCX_PE_LocalData_TableSize_16            (2)
#define     NFP_NBI_PCX_PE_LocalData_TableSize_64            (3)
#define     NFP_NBI_PCX_PE_LocalData_TableSize_256           (4)
#define     NFP_NBI_PCX_PE_LocalData_TableSize_1k            (5)
#define   NFP_NBI_PCX_PE_LocalData_Select(_x)                (((_x) & 0x1f) << 4)
#define   NFP_NBI_PCX_PE_LocalData_Select_of(_x)             (((_x) >> 4) & 0x1f)
#define   NFP_NBI_PCX_PE_LocalData_Lookup(_x)                (((_x) & 0xf) << 0)
#define   NFP_NBI_PCX_PE_LocalData_Lookup_of(_x)             (((_x) >> 0) & 0xf)
#define     NFP_NBI_PCX_PE_LocalData_Lookup_LUT8             (0)
#define     NFP_NBI_PCX_PE_LocalData_Lookup_LUT16            (1)
#define     NFP_NBI_PCX_PE_LocalData_Lookup_LUT32            (2)
#define     NFP_NBI_PCX_PE_LocalData_Lookup_Multibit         (3)
#define     NFP_NBI_PCX_PE_LocalData_Lookup_CAM32            (4)
#define     NFP_NBI_PCX_PE_LocalData_Lookup_CAM24            (5)
#define     NFP_NBI_PCX_PE_LocalData_Lookup_CAM16            (6)
#define     NFP_NBI_PCX_PE_LocalData_Lookup_CAM8             (7)
#define     NFP_NBI_PCX_PE_LocalData_Lookup_CAM32R16         (8)
#define     NFP_NBI_PCX_PE_LocalData_Lookup_CAM24R16         (9)
#define     NFP_NBI_PCX_PE_LocalData_Lookup_CAM16R16         (10)
#define     NFP_NBI_PCX_PE_LocalData_Lookup_CAM8R8           (11)
#define     NFP_NBI_PCX_PE_LocalData_Lookup_CAM48R16         (12)
#define     NFP_NBI_PCX_PE_LocalData_Lookup_TCAM24R16        (13)
#define     NFP_NBI_PCX_PE_LocalData_Lookup_TCAM16R16        (14)
#define     NFP_NBI_PCX_PE_LocalData_Lookup_TCAM8R16         (15)
/*----------------------------------------------------------------
  Register: LocalInst[8] - Local SRAM instruction table description X
    Bits: 31:22 Base1(wo) - Base address for table set 1 accesses
    Bits: 21:12 Base0(wo) - Base address for table set 0 accesses
    Bits: 11:9  TableSize(wo) - Number of entries in the table - effects masking of input data, and offset into the table
    Bits:  8:4  Select(wo) - Starting bit divide by 2 for determining offset into table for an access
*/
#define NFP_NBI_PCX_PE_LocalInst(_x)                         (0x00000120 + (0x4 * ((_x) & 0x7)))
#define   NFP_NBI_PCX_PE_LocalInst_Base1(_x)                 (((_x) & 0x3ff) << 22)
#define   NFP_NBI_PCX_PE_LocalInst_Base1_of(_x)              (((_x) >> 22) & 0x3ff)
#define   NFP_NBI_PCX_PE_LocalInst_Base0(_x)                 (((_x) & 0x3ff) << 12)
#define   NFP_NBI_PCX_PE_LocalInst_Base0_of(_x)              (((_x) >> 12) & 0x3ff)
#define   NFP_NBI_PCX_PE_LocalInst_TableSize(_x)             (((_x) & 0x7) << 9)
#define   NFP_NBI_PCX_PE_LocalInst_TableSize_of(_x)          (((_x) >> 9) & 0x7)
#define     NFP_NBI_PCX_PE_LocalInst_TableSize_1             (0)
#define     NFP_NBI_PCX_PE_LocalInst_TableSize_4             (1)
#define     NFP_NBI_PCX_PE_LocalInst_TableSize_16            (2)
#define     NFP_NBI_PCX_PE_LocalInst_TableSize_64            (3)
#define     NFP_NBI_PCX_PE_LocalInst_TableSize_256           (4)
#define     NFP_NBI_PCX_PE_LocalInst_TableSize_1k            (5)
#define   NFP_NBI_PCX_PE_LocalInst_Select(_x)                (((_x) & 0x1f) << 4)
#define   NFP_NBI_PCX_PE_LocalInst_Select_of(_x)             (((_x) >> 4) & 0x1f)
/*----------------------------------------------------------------
  Register: SharedData[8] - Shared SRAM data table description X
    Bits: 31:22 Base1(wo) - Base address for table set 1 accesses
    Bits: 21:12 Base0(wo) - Base address for table set 0 accesses
    Bits: 11:9  TableSize(wo) - Number of entries in the table - effects masking of input data, and offset into the table
    Bits:  8:4  Select(wo) - Starting bit divide by 2 for determining offset into table for an access
    Bits:  3:0  Lookup(wo) - Method of interpreting the SRAM data read, to produce a result for the lookup
*/
#define NFP_NBI_PCX_PE_SharedData(_x)                        (0x00000140 + (0x4 * ((_x) & 0x7)))
#define   NFP_NBI_PCX_PE_SharedData_Base1(_x)                (((_x) & 0x3ff) << 22)
#define   NFP_NBI_PCX_PE_SharedData_Base1_of(_x)             (((_x) >> 22) & 0x3ff)
#define   NFP_NBI_PCX_PE_SharedData_Base0(_x)                (((_x) & 0x3ff) << 12)
#define   NFP_NBI_PCX_PE_SharedData_Base0_of(_x)             (((_x) >> 12) & 0x3ff)
#define   NFP_NBI_PCX_PE_SharedData_TableSize(_x)            (((_x) & 0x7) << 9)
#define   NFP_NBI_PCX_PE_SharedData_TableSize_of(_x)         (((_x) >> 9) & 0x7)
#define     NFP_NBI_PCX_PE_SharedData_TableSize_1            (0)
#define     NFP_NBI_PCX_PE_SharedData_TableSize_4            (1)
#define     NFP_NBI_PCX_PE_SharedData_TableSize_16           (2)
#define     NFP_NBI_PCX_PE_SharedData_TableSize_64           (3)
#define     NFP_NBI_PCX_PE_SharedData_TableSize_256          (4)
#define     NFP_NBI_PCX_PE_SharedData_TableSize_1k           (5)
#define   NFP_NBI_PCX_PE_SharedData_Select(_x)               (((_x) & 0x1f) << 4)
#define   NFP_NBI_PCX_PE_SharedData_Select_of(_x)            (((_x) >> 4) & 0x1f)
#define   NFP_NBI_PCX_PE_SharedData_Lookup(_x)               (((_x) & 0xf) << 0)
#define   NFP_NBI_PCX_PE_SharedData_Lookup_of(_x)            (((_x) >> 0) & 0xf)
#define     NFP_NBI_PCX_PE_SharedData_Lookup_LUT8            (0)
#define     NFP_NBI_PCX_PE_SharedData_Lookup_LUT16           (1)
#define     NFP_NBI_PCX_PE_SharedData_Lookup_LUT32           (2)
#define     NFP_NBI_PCX_PE_SharedData_Lookup_Multibit        (3)
#define     NFP_NBI_PCX_PE_SharedData_Lookup_CAM32           (4)
#define     NFP_NBI_PCX_PE_SharedData_Lookup_CAM24           (5)
#define     NFP_NBI_PCX_PE_SharedData_Lookup_CAM16           (6)
#define     NFP_NBI_PCX_PE_SharedData_Lookup_CAM8            (7)
#define     NFP_NBI_PCX_PE_SharedData_Lookup_CAMR32          (8)
#define     NFP_NBI_PCX_PE_SharedData_Lookup_CAMR24          (9)
#define     NFP_NBI_PCX_PE_SharedData_Lookup_CAMR16          (10)
#define     NFP_NBI_PCX_PE_SharedData_Lookup_CAMR8           (11)
#define     NFP_NBI_PCX_PE_SharedData_Lookup_CAMR48          (12)
#define     NFP_NBI_PCX_PE_SharedData_Lookup_TCAMR24         (13)
#define     NFP_NBI_PCX_PE_SharedData_Lookup_TCAMR16         (14)
#define     NFP_NBI_PCX_PE_SharedData_Lookup_TCAMR8          (15)
/*----------------------------------------------------------------
  Register: SharedInst[8] - Shared SRAM instruction table description X
    Bits: 31:22 Base1(wo) - Base address for table set 1 accesses
    Bits: 21:12 Base0(wo) - Base address for table set 0 accesses
    Bits: 11:9  TableSize(wo) - Number of entries in the table - effects masking of input data, and offset into the table
    Bits:  8:4  Select(wo) - Starting bit divide by 2 for determining offset into table for an access
*/
#define NFP_NBI_PCX_PE_SharedInst(_x)                        (0x00000160 + (0x4 * ((_x) & 0x7)))
#define   NFP_NBI_PCX_PE_SharedInst_Base1(_x)                (((_x) & 0x3ff) << 22)
#define   NFP_NBI_PCX_PE_SharedInst_Base1_of(_x)             (((_x) >> 22) & 0x3ff)
#define   NFP_NBI_PCX_PE_SharedInst_Base0(_x)                (((_x) & 0x3ff) << 12)
#define   NFP_NBI_PCX_PE_SharedInst_Base0_of(_x)             (((_x) >> 12) & 0x3ff)
#define   NFP_NBI_PCX_PE_SharedInst_TableSize(_x)            (((_x) & 0x7) << 9)
#define   NFP_NBI_PCX_PE_SharedInst_TableSize_of(_x)         (((_x) >> 9) & 0x7)
#define     NFP_NBI_PCX_PE_SharedInst_TableSize_1            (0)
#define     NFP_NBI_PCX_PE_SharedInst_TableSize_4            (1)
#define     NFP_NBI_PCX_PE_SharedInst_TableSize_16           (2)
#define     NFP_NBI_PCX_PE_SharedInst_TableSize_64           (3)
#define     NFP_NBI_PCX_PE_SharedInst_TableSize_256          (4)
#define     NFP_NBI_PCX_PE_SharedInst_TableSize_1k           (5)
#define   NFP_NBI_PCX_PE_SharedInst_Select(_x)               (((_x) & 0x1f) << 4)
#define   NFP_NBI_PCX_PE_SharedInst_Select_of(_x)            (((_x) >> 4) & 0x1f)
/*----------------------------------------------------------------
  Register: MulticycleTable[8]Set0 - Multicycle lookup table description X for table set 0
  Register: MulticycleTable[8]Set1 - Multicycle lookup table description X for table set 1
    Bits: 31:0  Table(wo) - 32-bit table value (equivalent to MU lookup engine result) to present to MU lookup engine with the full 64-bit data
*/
#define NFP_NBI_PCX_PE_MulticycleTableSet0(_x)               (0x00000180 + (0x4 * ((_x) & 0x7)))
#define NFP_NBI_PCX_PE_MulticycleTableSet1(_x)               (0x000001a0 + (0x4 * ((_x) & 0x7)))
#define   NFP_NBI_PCX_PE_NbiPreMulticycleTable_Table(_x)     (((_x) & 0xffffffff) << 0)
#define   NFP_NBI_PCX_PE_NbiPreMulticycleTable_Table_of(_x)  (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: Config - Configuration of characterization
    Bits: 31:28 MaxDepth(rw) - Maximum depth of characterization for packets on minipacket bus minus one, in units of 16 bytes - a setting of zero is taken to mean 15
    Bits: 27    ExtendInitialSkip(rw) - Set to make the initial skip provided by ports to be four bits, using user_type[1] as the most significant bit; if clear, only a 3-bit initial skip is used
    Bits: 26    DisableInner(rw) - Set to disable inner protocol analysis for any packets that are characterized as IPv4/IPv6/MPLS etc; no further analysis is done beyond that point
    Bits: 25    DisablePPP(rw) - Set to disable PPP analysis for any packets that are characterized as PPP; no further analysis is done beyond that point
    Bits: 24    DisableEthernet(rw) - Set to disable ethernet analysis for any packets that are characterized as Ethernet; no further analysis is done beyond that point
    Bits: 21:20 PPPAddressSkip(rw) - Number of 16-bit octets to skip for PPP packets from start to protocol field
    Bits: 18    GFPControlIgn(rw) - Set to ignore
    Bits: 17    GFPBarker(rw) - Set to enable Barker scrambling on GFP headers
    Bits: 16    GFPExtension(rw) - Set to enable linear extension on GFP-F
    Bits: 11    PBBEnable(rw) - Set to enable PBB detection (0x88a8, X, 0x88e7) in Ethernet packets
    Bits: 10    EnableInitialTag(rw) - Set to enable TCAM0 as an initial tag; the port's user_type[0] bit must also be set for packets on the port to use TCAM0 as an initial tag
    Bits:  9:8  MaxVlans(rw) - Maximum number of VLAN tags permitted, from 0 to 3
    Bits:  7:4  InitialTagSize(rw) - Number of 16-bit words to skip if TCAM0 is used as an initial tag and it matches
    Bits:  3:0  ProtSkip(rw) - Number of 16-bit words to skip after start of Ethernet packet before looking for VLAN tags and other Ethernet Protocol types
*/
#define NFP_NBI_PCX_CHAR_Config                              0x00000004
#define   NFP_NBI_PCX_CHAR_Config_MaxDepth(_x)               (((_x) & 0xf) << 28)
#define   NFP_NBI_PCX_CHAR_Config_MaxDepth_of(_x)            (((_x) >> 28) & 0xf)
#define   NFP_NBI_PCX_CHAR_Config_ExtendInitialSkip          BIT(27)
#define   NFP_NBI_PCX_CHAR_Config_DisableInner               BIT(26)
#define   NFP_NBI_PCX_CHAR_Config_DisablePPP                 BIT(25)
#define   NFP_NBI_PCX_CHAR_Config_DisableEthernet            BIT(24)
#define   NFP_NBI_PCX_CHAR_Config_PPPAddressSkip(_x)         (((_x) & 0x3) << 20)
#define   NFP_NBI_PCX_CHAR_Config_PPPAddressSkip_of(_x)      (((_x) >> 20) & 0x3)
#define   NFP_NBI_PCX_CHAR_Config_GFPControlIgn              BIT(18)
#define   NFP_NBI_PCX_CHAR_Config_GFPBarker                  BIT(17)
#define   NFP_NBI_PCX_CHAR_Config_GFPExtension               BIT(16)
#define   NFP_NBI_PCX_CHAR_Config_PBBEnable                  BIT(11)
#define   NFP_NBI_PCX_CHAR_Config_EnableInitialTag           BIT(10)
#define   NFP_NBI_PCX_CHAR_Config_MaxVlans(_x)               (((_x) & 0x3) << 8)
#define   NFP_NBI_PCX_CHAR_Config_MaxVlans_of(_x)            (((_x) >> 8) & 0x3)
#define   NFP_NBI_PCX_CHAR_Config_InitialTagSize(_x)         (((_x) & 0xf) << 4)
#define   NFP_NBI_PCX_CHAR_Config_InitialTagSize_of(_x)      (((_x) >> 4) & 0xf)
#define   NFP_NBI_PCX_CHAR_Config_ProtSkip(_x)               (((_x) & 0xf) << 0)
#define   NFP_NBI_PCX_CHAR_Config_ProtSkip_of(_x)            (((_x) >> 0) & 0xf)
/*----------------------------------------------------------------
  Register: TcamTagControl[2] - Configuration of Ethernet Tcam Tag X control
    Bits:  5:4  Length(ro) - Number of 16-bit words to skip if the tag matches
    Bits:  0    Enable(rw) - Set to enable use of the TCAM tag in Ethernet packets
*/
#define NFP_NBI_PCX_CHAR_TcamTagControl(_x)                  (0x00000008 + (0x4 * ((_x) & 0x1)))
#define   NFP_NBI_PCX_CHAR_TcamTagControl_Length(_x)         (((_x) & 0x3) << 4)
#define   NFP_NBI_PCX_CHAR_TcamTagControl_Length_of(_x)      (((_x) >> 4) & 0x3)
#define     NFP_NBI_PCX_CHAR_TcamTagControl_Length_2         (0)
#define     NFP_NBI_PCX_CHAR_TcamTagControl_Length_3         (1)
#define     NFP_NBI_PCX_CHAR_TcamTagControl_Length_4         (2)
#define   NFP_NBI_PCX_CHAR_TcamTagControl_Enable             BIT(0)
/*----------------------------------------------------------------
  Register: TcamTag[2] - Configuration of Ethernet Tcam Tag X mask and match values
    Bits: 31:16 Mask(rw) - Mask for ANDing with 16-bit of Ethernet packet data
    Bits: 15:0  Value(rw) - Value to compare 16-bits of the Ethernet packet to after masking
*/
#define NFP_NBI_PCX_CHAR_TcamTag(_x)                         (0x00000010 + (0x4 * ((_x) & 0x1)))
#define   NFP_NBI_PCX_CHAR_TcamTag_Mask(_x)                  (((_x) & 0xffff) << 16)
#define   NFP_NBI_PCX_CHAR_TcamTag_Mask_of(_x)               (((_x) >> 16) & 0xffff)
#define   NFP_NBI_PCX_CHAR_TcamTag_Value(_x)                 (((_x) & 0xffff) << 0)
#define   NFP_NBI_PCX_CHAR_TcamTag_Value_of(_x)              (((_x) >> 0) & 0xffff)
/*----------------------------------------------------------------
  Register: Sequence - Sequence number of last packet characterized - internal use only
    Bits: 15:0  Number(ro) - Number of packets presented by characterizer to picoengine pool
*/
#define NFP_NBI_PCX_CHAR_Sequence                            0x00000018
#define   NFP_NBI_PCX_CHAR_Sequence_Number(_x)               (((_x) & 0xffff) << 0)
#define   NFP_NBI_PCX_CHAR_Sequence_Number_of(_x)            (((_x) >> 0) & 0xffff)
/*----------------------------------------------------------------
  Register: TableSet - Table set configuration and status
    Bits: 31    OneShot(rw) - If asserted, the active table set will be cleared after the next packet utilizes table set 1; for internal use only
    Bits:  7:6  CharacterizerInUse(ro) - One bit per table set indicating that the characterizer is actively working on packets belonging to those table sets
    Bits:  5:4  PicoengineInUse(ro) - One bit per table set indicating that picoengines are actively working on packets belonging to those table sets
    Bits:  0    Active(rw) - Table set to use for packets that complete characterization
*/
#define NFP_NBI_PCX_CHAR_TableSet                            0x0000001c
#define   NFP_NBI_PCX_CHAR_TableSet_OneShot                  BIT(31)
#define   NFP_NBI_PCX_CHAR_TableSet_CharacterizerInUse(_x)   (((_x) & 0x3) << 6)
#define   NFP_NBI_PCX_CHAR_TableSet_CharacterizerInUse_of(_x) (((_x) >> 6) & 0x3)
#define   NFP_NBI_PCX_CHAR_TableSet_PicoengineInUse(_x)      (((_x) & 0x3) << 4)
#define   NFP_NBI_PCX_CHAR_TableSet_PicoengineInUse_of(_x)   (((_x) >> 4) & 0x3)
#define   NFP_NBI_PCX_CHAR_TableSet_Active                   BIT(0)
/*----------------------------------------------------------------
  Register: Override - Configuration of characterization override - internal use only
    Bits:  1    OneShot(wo) - Set if characterization override enable should be cleared after it is used once
    Bits:  0    Enable(wo) - Set if characterization override should be used - for internal test purposes only
*/
#define NFP_NBI_PCX_CHAR_Override                            0x00000020
#define   NFP_NBI_PCX_CHAR_Override_OneShot                  BIT(1)
#define   NFP_NBI_PCX_CHAR_Override_Enable                   BIT(0)
/*----------------------------------------------------------------
  Register: OverridePort - Configuration of characterization override - internal use only
    Bits: 31:0  Port(wo) - Value to be used for the 'port' if characterization override is used for the picoengines
*/
#define NFP_NBI_PCX_CHAR_OverridePort                        0x00000024
#define   NFP_NBI_PCX_CHAR_OverridePort_Port(_x)             (((_x) & 0xffffffff) << 0)
#define   NFP_NBI_PCX_CHAR_OverridePort_Port_of(_x)          (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: OverrideFlags - Configuration of characterization override - internal use only
    Bits: 31:0  Flags(wo) - Value to be used for the 'flags' if characterization override is used for the picoengines
*/
#define NFP_NBI_PCX_CHAR_OverrideFlags                       0x00000028
#define   NFP_NBI_PCX_CHAR_OverrideFlags_Flags(_x)           (((_x) & 0xffffffff) << 0)
#define   NFP_NBI_PCX_CHAR_OverrideFlags_Flags_of(_x)        (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: OverrideOffsets - Configuration of characterization override - internal use only
    Bits: 31:0  Offsets(wo) - Value to be used for the 'offsets' if characterization override is used for the picoengines
*/
#define NFP_NBI_PCX_CHAR_OverrideOffsets                     0x0000002c
#define   NFP_NBI_PCX_CHAR_OverrideOffsets_Offsets(_x)       (((_x) & 0xffffffff) << 0)
#define   NFP_NBI_PCX_CHAR_OverrideOffsets_Offsets_of(_x)    (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: TcamMatchLow[7] - Results TCAM match low for entry X
    Bits: 31:0  Value(wo) - Match value for characterization TCAM; compared with the characterization result low 32 bits ANDed with the mask
*/
#define NFP_NBI_PCX_CHAR_TcamMatchLow(_x)                    (0x00000200 + (0x10 * ((_x) & 0x7)))
#define   NFP_NBI_PCX_CHAR_TcamMatchLow_Value(_x)            (((_x) & 0xffffffff) << 0)
#define   NFP_NBI_PCX_CHAR_TcamMatchLow_Value_of(_x)         (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: TcamMatchHigh[7] - Results TCAM match high for entry X
    Bits: 31:0  Value(wo) - Match value for characterization TCAM; compared with the characterization result high 32 bits ANDed with the mask
*/
#define NFP_NBI_PCX_CHAR_TcamMatchHigh(_x)                   (0x00000204 + (0x10 * ((_x) & 0x7)))
#define   NFP_NBI_PCX_CHAR_TcamMatchHigh_Value(_x)           (((_x) & 0xffffffff) << 0)
#define   NFP_NBI_PCX_CHAR_TcamMatchHigh_Value_of(_x)        (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: TcamMaskLow[7] - Results TCAM mask low for entry X
    Bits: 31:0  Value(wo) - Mask value for characterization TCAM; masked with the characterization result low 32 bits before comparison with the match low
*/
#define NFP_NBI_PCX_CHAR_TcamMaskLow(_x)                     (0x00000208 + (0x10 * ((_x) & 0x7)))
#define   NFP_NBI_PCX_CHAR_TcamMaskLow_Value(_x)             (((_x) & 0xffffffff) << 0)
#define   NFP_NBI_PCX_CHAR_TcamMaskLow_Value_of(_x)          (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: TcamMaskHigh[7] - Results TCAM mask high for entry X
    Bits: 31:0  Value(wo) - Mask value for characterization TCAM; compared with the characterization result high 32 bits before comparison with the match high
*/
#define NFP_NBI_PCX_CHAR_TcamMaskHigh(_x)                    (0x0000020c + (0x10 * ((_x) & 0x7)))
#define   NFP_NBI_PCX_CHAR_TcamMaskHigh_Value(_x)            (((_x) & 0xffffffff) << 0)
#define   NFP_NBI_PCX_CHAR_TcamMaskHigh_Value_of(_x)         (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: TcamMapping[8] - Picoengine start data for a match with TCAM X of characterization results
    Bits: 31:16 Index(wo) - Index to use if MetadataOverride is set
    Bits:  8    Override(wo) - Set to override the use of the metadata as an index; use index in this register instead
    Bits:  7:4  Metadata(wo) - Which metadata to use as an index for the instruction lookup, unless metadata override is set
    Bits:  2:0  Table(wo) - Local instruction table to use for initial instructions
*/
#define NFP_NBI_PCX_CHAR_TcamMapping(_x)                     (0x00000300 + (0x4 * ((_x) & 0x7)))
#define   NFP_NBI_PCX_CHAR_TcamMapping_Index(_x)             (((_x) & 0xffff) << 16)
#define   NFP_NBI_PCX_CHAR_TcamMapping_Index_of(_x)          (((_x) >> 16) & 0xffff)
#define   NFP_NBI_PCX_CHAR_TcamMapping_Override              BIT(8)
#define   NFP_NBI_PCX_CHAR_TcamMapping_Metadata(_x)          (((_x) & 0xf) << 4)
#define   NFP_NBI_PCX_CHAR_TcamMapping_Metadata_of(_x)       (((_x) >> 4) & 0xf)
#define   NFP_NBI_PCX_CHAR_TcamMapping_Table(_x)             (((_x) & 0x7) << 0)
#define   NFP_NBI_PCX_CHAR_TcamMapping_Table_of(_x)          (((_x) >> 0) & 0x7)
/*----------------------------------------------------------------
  Register: PortCfg[128] - Characterization port config X
    Bits:  7:5  UserType(wo) - Logical port type as passed to the output of the hardware characterization
    Bits:  4:2  Skip(wo) - Initial number of 16-bits to skip before the packet starts on the logical port; use this to skip any MAC prepend or any proprietary pre-DA/SA tagging for Ethernet packets
    Bits:  1:0  Analysis(wo) - Type of outer characterization to attempt for packets on the logical port
*/
#define NFP_NBI_PCX_CHAR_PortCfg(_x)                         (0x00000400 + (0x4 * ((_x) & 0x7f)))
#define   NFP_NBI_PCX_CHAR_PortCfg_UserType(_x)              (((_x) & 0x7) << 5)
#define   NFP_NBI_PCX_CHAR_PortCfg_UserType_of(_x)           (((_x) >> 5) & 0x7)
#define   NFP_NBI_PCX_CHAR_PortCfg_Skip(_x)                  (((_x) & 0x7) << 2)
#define   NFP_NBI_PCX_CHAR_PortCfg_Skip_of(_x)               (((_x) >> 2) & 0x7)
#define   NFP_NBI_PCX_CHAR_PortCfg_Analysis(_x)              (((_x) & 0x3) << 0)
#define   NFP_NBI_PCX_CHAR_PortCfg_Analysis_of(_x)           (((_x) >> 0) & 0x3)
#define     NFP_NBI_PCX_CHAR_PortCfg_Analysis_GFP_F          (0)
#define     NFP_NBI_PCX_CHAR_PortCfg_Analysis_Eth            (1)
#define     NFP_NBI_PCX_CHAR_PortCfg_Analysis_PPP            (2)
#define     NFP_NBI_PCX_CHAR_PortCfg_Analysis_InnerType      (3)
/*----------------------------------------------------------------
  Register: Accumulator[8] - Accumulator for policer X
    Bits: 31    Saturate(rw) - If set then the credit addition saturates at 0x7fffffff when a picoengine policing operation is performed; if clear then the credit addition wraps from 0x7fffffff to 0
    Bits: 30:0  Credit(rw) - Current credit for the policing accumulator
*/
#define NFP_NBI_PCX_POL_Accumulator(_x)                      (0x00000000 + (0x4 * ((_x) & 0x7)))
#define   NFP_NBI_PCX_POL_Accumulator_Saturate               BIT(31)
#define   NFP_NBI_PCX_POL_Accumulator_Credit(_x)             (((_x) & 0x7fffffff) << 0)
#define   NFP_NBI_PCX_POL_Accumulator_Credit_of(_x)          (((_x) >> 0) & 0x7fffffff)
/*----------------------------------------------------------------
  Register: CreditRate[8] - Credit rate adjuster for policer X
    Bits: 31    CreditShift(rw) - If set then the credit subtraction is by 'Credit'*256; if clear then the credit subtraction is by just 'Credit'. (Note the binary negation described in the Credit field occurs after this credit shift)
    Bits: 30:20 Credit(rw) - Value to subtract from credit at 'interval', optionally shifted left by 8. (Note that subtraction is performed with addition of ~Credit, so a value field of 0 indicates subtract 1)
    Bits: 19:0  Interval(rw) - Interval between credit subtractions; if 0 then credit is not subtracted from the accumulator
*/
#define NFP_NBI_PCX_POL_CreditRate(_x)                       (0x00000020 + (0x4 * ((_x) & 0x7)))
#define   NFP_NBI_PCX_POL_CreditRate_CreditShift             BIT(31)
#define   NFP_NBI_PCX_POL_CreditRate_Credit(_x)              (((_x) & 0x7ff) << 20)
#define   NFP_NBI_PCX_POL_CreditRate_Credit_of(_x)           (((_x) >> 20) & 0x7ff)
#define   NFP_NBI_PCX_POL_CreditRate_Interval(_x)            (((_x) & 0xfffff) << 0)
#define   NFP_NBI_PCX_POL_CreditRate_Interval_of(_x)         (((_x) >> 0) & 0xfffff)
/*----------------------------------------------------------------
  Register: Comparator[8] - Policing comparator X
    Bits: 30:0  Value(rw) - Value to compare against the result of a policing operation
*/
#define NFP_NBI_PCX_POL_Comparator(_x)                       (0x00000040 + (0x4 * ((_x) & 0x7)))
#define   NFP_NBI_PCX_POL_Comparator_Value(_x)               (((_x) & 0x7fffffff) << 0)
#define   NFP_NBI_PCX_POL_Comparator_Value_of(_x)            (((_x) >> 0) & 0x7fffffff)
/*----------------------------------------------------------------
  Register: Config - Policing configuration
    Bits: 15:0  ClockDivide(rw) - Clock divider value, effects rate that credit 'clock' runs
*/
#define NFP_NBI_PCX_POL_Config                               0x00000060
#define   NFP_NBI_PCX_POL_Config_ClockDivide(_x)             (((_x) & 0xffff) << 0)
#define   NFP_NBI_PCX_POL_Config_ClockDivide_of(_x)          (((_x) >> 0) & 0xffff)
/*----------------------------------------------------------------
  Register: Sequence[8] - Picoengine sequencer X
    Bits: 31:0  Sequence(rw) - Sequence number to provide for next packet
*/
#define NFP_NBI_PCX_POL_Sequence(_x)                         (0x00000080 + (0x4 * ((_x) & 0x7)))
#define   NFP_NBI_PCX_POL_Sequence_Sequence(_x)              (((_x) & 0xffffffff) << 0)
#define   NFP_NBI_PCX_POL_Sequence_Sequence_of(_x)           (((_x) >> 0) & 0xffffffff)

#endif /* NFP6000_LONGNAMES */


#endif /* NFP6000_NBI_PC_H */
