/*
 * Copyright (C) 2013, Netronome Systems, Inc.  All rights reserved.
 *
 * Disclaimer: this file is provided without any warranty
 * as part of an early-access program, and the content is
 * bound to change before the final release.
 */

#ifndef NFP3800_NBI_TM_H
#define NFP3800_NBI_TM_H

/*================================================================*/
#include "nfp_nbi.h"

/*----------------------------------------------------------------*/
/* CPP Address Map (CPP_BASE = IMB translation: island + indexes) */

/* CPP_BASE + NFP3800_NBI_TM + NFP3800_NBI_TM_* */
#define NFP3800_NBI_TM                                           (0x200000)

/*----------------------------------------------------------------*/
/* XPB Address Map (XPB_BASE = NFP3800_XPB_ISLAND(island))          */

#define NFP3800_NBI_TMX                                          (NBIX_BASE + 0x040000)

/* XPB_BASE + NFP3800_NBI_TMX_CSR + NFP3800_NBI_TMX_CSR_* */
#define NFP3800_NBI_TMX_CSR                                      (NFP3800_NBI_TMX + 0x00000)
/* XPB_BASE + NFP3800_NBI_TMX_Q + NFP3800_NBI_TMX_Q_* */
#define NFP3800_NBI_TMX_Q                                        (NFP3800_NBI_TMX + 0x10000)
/* XPB_BASE + NFP3800_NBI_TMX_SCH_CSR + NFP3800_NBI_TMX_SCH_CSR_* */
#define NFP3800_NBI_TMX_SCH_CSR                                  (NFP3800_NBI_TMX + 0x20000)
/* XPB_BASE + NFP3800_NBI_TMX_SCH_L1WGT + NFP3800_NBI_TMX_SCH_WGT_* */
#define NFP3800_NBI_TMX_SCH_L1WGT                                (NFP3800_NBI_TMX + 0x30000)
/* XPB_BASE + NFP3800_NBI_TMX_SCH_L1DFT + NFP3800_NBI_TMX_SCH_DFT_* */
#define NFP3800_NBI_TMX_SCH_L1DFT                                (NFP3800_NBI_TMX + 0x40000)
/* XPB_BASE + NFP3800_NBI_TMX_SCH_L1_ECC + (nfp_ecc.h) */
#define NFP3800_NBI_TMX_SCH_L1_ECC                               (NFP3800_NBI_TMX + 0x50000)
/* XPB_BASE + NFP3800_NBI_TMX_SCH_L2WGT + NFP3800_NBI_TMX_SCH_WGT_* */
#define NFP3800_NBI_TMX_SCH_L2WGT                                (NFP3800_NBI_TMX + 0x60000)
/* XPB_BASE + NFP3800_NBI_TMX_SCH_L2DFT + NFP3800_NBI_TMX_SCH_DFT_* */
#define NFP3800_NBI_TMX_SCH_L2DFT                                (NFP3800_NBI_TMX + 0x70000)
/* XPB_BASE + NFP3800_NBI_TMX_SCH_L2_ECC + (nfp_ecc.h) */
#define NFP3800_NBI_TMX_SCH_L2_ECC                               (NFP3800_NBI_TMX + 0x80000)
/* XPB_BASE + NFP3800_NBI_TMX_SHP + NFP3800_NBI_TMX_SHP_* */
#define NFP3800_NBI_TMX_SHP                                      (NFP3800_NBI_TMX + 0x90000)
/* XPB_BASE + NFP3800_NBI_TMX_DESCMEM_ECC + (nfp_ecc.h) */
#define NFP3800_NBI_TMX_DESCMEM_ECC                              (NFP3800_NBI_TMX + 0xa0000)
/* XPB_BASE + NFP3800_NBI_TMX_HDTLMEM_ECC + (nfp_ecc.h) */
#define NFP3800_NBI_TMX_HDTLMEM_ECC                              (NFP3800_NBI_TMX + 0xb0000)
/* XPB_BASE + NFP3800_NBI_TMX_BLQMEM_ECC + (nfp_ecc.h) */
#define NFP3800_NBI_TMX_BLQMEM_ECC                               (NFP3800_NBI_TMX + 0xc0000)
/* XPB_BASE + NFP3800_NBI_TMX_EGPKT_ECC + (nfp_ecc.h) */
#define NFP3800_NBI_TMX_EGPKT_ECC                                (NFP3800_NBI_TMX + 0xd0000)
/* XPB_BASE + NFP3800_NBI_TMX_REORDERMEM_ECC + (nfp_ecc.h) */
#define NFP3800_NBI_TMX_REORDERMEM_ECC                           (NFP3800_NBI_TMX + 0xf0000)
/* XPB_BASE + NFP3800_NBI_TMX_SCH_L0_ECC + (nfp_ecc.h) */
#define NFP3800_NBI_TMX_SCH_L0_ECC                               (NFP3800_NBI_TMX + 0x100000)
/* XPB_BASE + NFP3800_NBI_TMX_DROPCNTMEM_ECC + (nfp_ecc.h) */
#define NFP3800_NBI_TMX_DROPCNTMEM_ECC                           (NFP3800_NBI_TMX + 0x110000)
/* XPB_BASE + NFP3800_NBI_TMX_FREENEXTMEM_ECC + (nfp_ecc.h) */
#define NFP3800_NBI_TMX_FREENEXTMEM_ECC                          (NFP3800_NBI_TMX + 0x120000)
/* XPB_BASE + NFP3800_NBI_TMX_QLVLMEM_ECC + (nfp_ecc.h) */
#define NFP3800_NBI_TMX_QLVLMEM_ECC                              (NFP3800_NBI_TMX + 0x130000)

/*================================================================*/


/* HGID: c3e5162fe280 */

#ifndef NFP3800_LONGNAMES

/*----------------------------------------------------------------
  Register: TMDescSram_TMDescSramEntry[4096] - NBI TM Descriptor SRAM Entry
    Bits: 63:0  DescSRAMData(rw) - 64-bit descriptor stored in the TM Descriptor SRAM. Refer to the Packet Descriptor Memory tables in the MAS for bit definitions
*/
#define NFP3800_NBI_TM_TMDescSram_TM_DESC_ENTRY(_x)          (0x00000000 + (0x8 * ((_x) & 0xfff)))
#define   NFP3800_NBI_TM_TMDESCSRAM_TM_DESC_ENTRY_DESC_SRAM_DATA(_x) (((_x) & 0xffffffffffffffff) << 0)
#define   NFP3800_NBI_TM_TMDESCSRAM_TM_DESC_ENTRY_DESC_SRAM_DATA_of(_x) (((_x) >> 0) & 0xffffffffffffffff)
/*----------------------------------------------------------------
  Register: TMNextPtrSram_TMNextPtrSramEntry[4096] - NBI TM NextPtr SRAM Entry
    Bits: 11:0  NextPtrSRAMData(rw) - 13-bits of Nextptr, initialize memory with available Nextptr's
*/
#define NFP3800_NBI_TM_TMNextPtrSram_TM_NEXTPTR_SRAM_ENTRY(_x) (0x00020000 + (0x8 * ((_x) & 0xfff)))
#define   NFP3800_NBI_TM_TMNEXTPTRSRAM_TM_NEXTPTR_SRAM_ENTRY_NEXTPTR_SRAM_DATA(_x) (((_x) & 0xfff) << 0)
#define   NFP3800_NBI_TM_TMNEXTPTRSRAM_TM_NEXTPTR_SRAM_ENTRY_NEXTPTR_SRAM_DATA_of(_x) (((_x) >> 0) & 0xfff)
/*----------------------------------------------------------------
  Register: TMBDSram_NbiBDSramEntry[4096] - NBI TM Buffer Descriptor SRAM Entry
    Bits: 28:0  MuAddr(rw) - Memory Unit Address of the buffer (internal/external MU). Maps to CPP Address[29;11] Refer to the NBI top-level Master Commands section for memory unit address translation.
*/
#define NFP3800_NBI_TM_TMBDSram_TM_BDSRAM_ENTRY(_x)          (0x00040000 + (0x8 * ((_x) & 0xfff)))
#define   NFP3800_NBI_TM_TMBDSRAM_TM_BDSRAM_ENTRY_MUADDR(_x) (((_x) & 0x1fffffff) << 0)
#define   NFP3800_NBI_TM_TMBDSRAM_TM_BDSRAM_ENTRY_MUADDR_of(_x) (((_x) >> 0) & 0x1fffffff)
/*----------------------------------------------------------------
  Register: TMBufferList_BLQueCtrl0 - Configures the buffer list circular queue and provides status
    Bits: 55:48 BLQEventFullCnt(rc) - Debug counter that increments when a BLQ Event is dropped due to a full BLQ Event FIFO. This counter saturates at all 1's
    Bits: 42:40 BDFifoEntryCnt(ro) - Number of buffer descriptors resident in the buffer descriptor register FIFO
    Bits: 39    BLQFull(ro) - The Queue is full when set.
    Bits: 38    BLQEmpty(ro) - The queue is empty when set.
    Bits: 37:36 BLQSize(rw) - Defines the Size of the Circular Buffer List Queue
    Bits: 34:24 BLEntryCnt(ro) - Hardware calculated value of the number of entries on the Buffer List Queue. A full Buffer List Queue has an entry count equal to zero, with the BLQFull flag set. An empty Buffer List Queue has an entry count equal to zero with the BLQEmpty flag set.
    Bits: 22:12 BLHdPtr(rw) - Buffer List Queue Head Pointer. Points to the next entry to be read by hardware.
    Bits: 10:0  BLTlPtr(rw) - Buffer List Queue Tail Pointer. Points to the next entry to be written at the tail.
*/
#define NFP3800_NBI_TM_TMBufferList_BL_QUE_CTRL0             0x00048000
#define   NFP3800_NBI_TM_TMBUFFERLIST_BL_QUE_CTRL0_BLQ_EVENT_FULL_CNT(_x) (((_x) & 0xff) << 48)
#define   NFP3800_NBI_TM_TMBUFFERLIST_BL_QUE_CTRL0_BLQ_EVENT_FULL_CNT_of(_x) (((_x) >> 48) & 0xff)
#define   NFP3800_NBI_TM_TMBUFFERLIST_BL_QUE_CTRL0_BD_FIFO_ENTRY_CNT(_x) (((_x) & 0x7) << 40)
#define   NFP3800_NBI_TM_TMBUFFERLIST_BL_QUE_CTRL0_BD_FIFO_ENTRY_CNT_of(_x) (((_x) >> 40) & 0x7)
#define   NFP3800_NBI_TM_TMBUFFERLIST_BL_QUE_CTRL0_BL_QUE_FULL BIT_ULL(39)
#define   NFP3800_NBI_TM_TMBUFFERLIST_BL_QUE_CTRL0_BL_QUE_EMPTY BIT_ULL(38)
#define   NFP3800_NBI_TM_TMBUFFERLIST_BL_QUE_CTRL0_BL_QUE_SIZE(_x) (((_x) & 0x3) << 36)
#define   NFP3800_NBI_TM_TMBUFFERLIST_BL_QUE_CTRL0_BL_QUE_SIZE_of(_x) (((_x) >> 36) & 0x3)
#define     NFP3800_NBI_TM_TMBUFFERLIST_BL_QUE_CTRL0_BL_QUE_SIZE_256 (0)
#define     NFP3800_NBI_TM_TMBUFFERLIST_BL_QUE_CTRL0_BL_QUE_SIZE_512 (1)
#define     NFP3800_NBI_TM_TMBUFFERLIST_BL_QUE_CTRL0_BL_QUE_SIZE_1K (2)
#define     NFP3800_NBI_TM_TMBUFFERLIST_BL_QUE_CTRL0_BL_QUE_SIZE_2K (3)
#define   NFP3800_NBI_TM_TMBUFFERLIST_BL_QUE_CTRL0_BL_ENTRY_CNT(_x) (((_x) & 0x7ff) << 24)
#define   NFP3800_NBI_TM_TMBUFFERLIST_BL_QUE_CTRL0_BL_ENTRY_CNT_of(_x) (((_x) >> 24) & 0x7ff)
#define   NFP3800_NBI_TM_TMBUFFERLIST_BL_QUE_CTRL0_BL_HEAD_PTR(_x) (((_x) & 0x7ff) << 12)
#define   NFP3800_NBI_TM_TMBUFFERLIST_BL_QUE_CTRL0_BL_HEAD_PTR_of(_x) (((_x) >> 12) & 0x7ff)
#define   NFP3800_NBI_TM_TMBUFFERLIST_BL_QUE_CTRL0_BL_TAIL_PTR(_x) (((_x) & 0x7ff) << 0)
#define   NFP3800_NBI_TM_TMBUFFERLIST_BL_QUE_CTRL0_BL_TAIL_PTR_of(_x) (((_x) >> 0) & 0x7ff)
/*----------------------------------------------------------------
  Register: TMBufferList_BLQueCtrl1 - Configures the buffer list circular queue and provides status
    Bits: 55:48 BLQEventFullCnt(rc) - Debug counter that increments when a BLQ Event is dropped due to a full BLQ Event FIFO. This counter saturates at all 1's
    Bits: 42:40 BDFifoEntryCnt(ro) - Number of buffer descriptors resident in the buffer descriptor register FIFO
    Bits: 39    BLQFull(ro) - The Queue is full when set.
    Bits: 38    BLQEmpty(ro) - The queue is empty when set.
    Bits: 37:36 BLQSize(rw) - Defines the Size of the Circular Buffer List Queue
    Bits: 34:24 BLEntryCnt(ro) - Hardware calculated value of the number of entries on the Buffer List Queue. A full Buffer List Queue has an entry count equal to zero, with the BLQFull flag set. An empty Buffer List Queue has an entry count equal to zero with the BLQEmpty flag set.
    Bits: 22:12 BLHdPtr(rw) - Buffer List Queue Head Pointer. Points to the next entry to be read by hardware.
    Bits: 10:0  BLTlPtr(rw) - Buffer List Queue Tail Pointer. Points to the next entry to be written at the tail.
*/
#define NFP3800_NBI_TM_TMBufferList_BL_QUE_CTRL1             0x00048008
#define   NFP3800_NBI_TM_TMBUFFERLIST_BL_QUE_CTRL1_BLQ_EVENT_FULL_CNT(_x) (((_x) & 0xff) << 48)
#define   NFP3800_NBI_TM_TMBUFFERLIST_BL_QUE_CTRL1_BLQ_EVENT_FULL_CNT_of(_x) (((_x) >> 48) & 0xff)
#define   NFP3800_NBI_TM_TMBUFFERLIST_BL_QUE_CTRL1_BD_FIFO_ENTRY_CNT(_x) (((_x) & 0x7) << 40)
#define   NFP3800_NBI_TM_TMBUFFERLIST_BL_QUE_CTRL1_BD_FIFO_ENTRY_CNT_of(_x) (((_x) >> 40) & 0x7)
#define   NFP3800_NBI_TM_TMBUFFERLIST_BL_QUE_CTRL1_BL_QUE_FULL BIT_ULL(39)
#define   NFP3800_NBI_TM_TMBUFFERLIST_BL_QUE_CTRL1_BL_QUE_EMPTY BIT_ULL(38)
#define   NFP3800_NBI_TM_TMBUFFERLIST_BL_QUE_CTRL1_BL_QUE_SIZE(_x) (((_x) & 0x3) << 36)
#define   NFP3800_NBI_TM_TMBUFFERLIST_BL_QUE_CTRL1_BL_QUE_SIZE_of(_x) (((_x) >> 36) & 0x3)
#define     NFP3800_NBI_TM_TMBUFFERLIST_BL_QUE_CTRL1_BL_QUE_SIZE_256 (0)
#define     NFP3800_NBI_TM_TMBUFFERLIST_BL_QUE_CTRL1_BL_QUE_SIZE_512 (1)
#define     NFP3800_NBI_TM_TMBUFFERLIST_BL_QUE_CTRL1_BL_QUE_SIZE_1K (2)
#define     NFP3800_NBI_TM_TMBUFFERLIST_BL_QUE_CTRL1_BL_QUE_SIZE_2K (3)
#define   NFP3800_NBI_TM_TMBUFFERLIST_BL_QUE_CTRL1_BL_ENTRY_CNT(_x) (((_x) & 0x7ff) << 24)
#define   NFP3800_NBI_TM_TMBUFFERLIST_BL_QUE_CTRL1_BL_ENTRY_CNT_of(_x) (((_x) >> 24) & 0x7ff)
#define   NFP3800_NBI_TM_TMBUFFERLIST_BL_QUE_CTRL1_BL_HEAD_PTR(_x) (((_x) & 0x7ff) << 12)
#define   NFP3800_NBI_TM_TMBUFFERLIST_BL_QUE_CTRL1_BL_HEAD_PTR_of(_x) (((_x) >> 12) & 0x7ff)
#define   NFP3800_NBI_TM_TMBUFFERLIST_BL_QUE_CTRL1_BL_TAIL_PTR(_x) (((_x) & 0x7ff) << 0)
#define   NFP3800_NBI_TM_TMBUFFERLIST_BL_QUE_CTRL1_BL_TAIL_PTR_of(_x) (((_x) >> 0) & 0x7ff)
/*----------------------------------------------------------------
  Register: TMBufferList_BLQueCtrl2 - Configures the buffer list circular queue and provides status
    Bits: 55:48 BLQEventFullCnt(rc) - Debug counter that increments when a BLQ Event is dropped due to a full BLQ Event FIFO. This counter saturates at all 1's
    Bits: 42:40 BDFifoEntryCnt(ro) - Number of buffer descriptors resident in the buffer descriptor register FIFO
    Bits: 39    BLQFull(ro) - The Queue is full when set.
    Bits: 38    BLQEmpty(ro) - The queue is empty when set.
    Bits: 37:36 BLQSize(rw) - Defines the Size of the Circular Buffer List Queue
    Bits: 34:24 BLEntryCnt(ro) - Hardware calculated value of the number of entries on the Buffer List Queue. A full Buffer List Queue has an entry count equal to zero, with the BLQFull flag set. An empty Buffer List Queue has an entry count equal to zero with the BLQEmpty flag set.
    Bits: 22:12 BLHdPtr(rw) - Buffer List Queue Head Pointer. Points to the next entry to be read by hardware.
    Bits: 10:0  BLTlPtr(rw) - Buffer List Queue Tail Pointer. Points to the next entry to be written at the tail.
*/
#define NFP3800_NBI_TM_TMBufferList_BL_QUE_CTRL2             0x00048010
#define   NFP3800_NBI_TM_TMBUFFERLIST_BL_QUE_CTRL2_BLQ_EVENT_FULL_CNT(_x) (((_x) & 0xff) << 48)
#define   NFP3800_NBI_TM_TMBUFFERLIST_BL_QUE_CTRL2_BLQ_EVENT_FULL_CNT_of(_x) (((_x) >> 48) & 0xff)
#define   NFP3800_NBI_TM_TMBUFFERLIST_BL_QUE_CTRL2_BD_FIFO_ENTRY_CNT(_x) (((_x) & 0x7) << 40)
#define   NFP3800_NBI_TM_TMBUFFERLIST_BL_QUE_CTRL2_BD_FIFO_ENTRY_CNT_of(_x) (((_x) >> 40) & 0x7)
#define   NFP3800_NBI_TM_TMBUFFERLIST_BL_QUE_CTRL2_BL_QUE_FULL BIT_ULL(39)
#define   NFP3800_NBI_TM_TMBUFFERLIST_BL_QUE_CTRL2_BL_QUE_EMPTY BIT_ULL(38)
#define   NFP3800_NBI_TM_TMBUFFERLIST_BL_QUE_CTRL2_BL_QUE_SIZE(_x) (((_x) & 0x3) << 36)
#define   NFP3800_NBI_TM_TMBUFFERLIST_BL_QUE_CTRL2_BL_QUE_SIZE_of(_x) (((_x) >> 36) & 0x3)
#define     NFP3800_NBI_TM_TMBUFFERLIST_BL_QUE_CTRL2_BL_QUE_SIZE_256 (0)
#define     NFP3800_NBI_TM_TMBUFFERLIST_BL_QUE_CTRL2_BL_QUE_SIZE_512 (1)
#define     NFP3800_NBI_TM_TMBUFFERLIST_BL_QUE_CTRL2_BL_QUE_SIZE_1K (2)
#define     NFP3800_NBI_TM_TMBUFFERLIST_BL_QUE_CTRL2_BL_QUE_SIZE_2K (3)
#define   NFP3800_NBI_TM_TMBUFFERLIST_BL_QUE_CTRL2_BL_ENTRY_CNT(_x) (((_x) & 0x7ff) << 24)
#define   NFP3800_NBI_TM_TMBUFFERLIST_BL_QUE_CTRL2_BL_ENTRY_CNT_of(_x) (((_x) >> 24) & 0x7ff)
#define   NFP3800_NBI_TM_TMBUFFERLIST_BL_QUE_CTRL2_BL_HEAD_PTR(_x) (((_x) & 0x7ff) << 12)
#define   NFP3800_NBI_TM_TMBUFFERLIST_BL_QUE_CTRL2_BL_HEAD_PTR_of(_x) (((_x) >> 12) & 0x7ff)
#define   NFP3800_NBI_TM_TMBUFFERLIST_BL_QUE_CTRL2_BL_TAIL_PTR(_x) (((_x) & 0x7ff) << 0)
#define   NFP3800_NBI_TM_TMBUFFERLIST_BL_QUE_CTRL2_BL_TAIL_PTR_of(_x) (((_x) >> 0) & 0x7ff)
/*----------------------------------------------------------------
  Register: TMBufferList_BLQueCtrl3 - Configures the buffer list circular queue and provides status
    Bits: 55:48 BLQEventFullCnt(rc) - Debug counter that increments when a BLQ Event is dropped due to a full BLQ Event FIFO. This counter saturates at all 1's
    Bits: 42:40 BDFifoEntryCnt(ro) - Number of buffer descriptors resident in the buffer descriptor register FIFO
    Bits: 39    BLQFull(ro) - The Queue is full when set.
    Bits: 38    BLQEmpty(ro) - The queue is empty when set.
    Bits: 37:36 BLQSize(rw) - Defines the Size of the Circular Buffer List Queue
    Bits: 34:24 BLEntryCnt(ro) - Hardware calculated value of the number of entries on the Buffer List Queue. A full Buffer List Queue has an entry count equal to zero, with the BLQFull flag set. An empty Buffer List Queue has an entry count equal to zero with the BLQEmpty flag set.
    Bits: 22:12 BLHdPtr(rw) - Buffer List Queue Head Pointer. Points to the next entry to be read by hardware.
    Bits: 10:0  BLTlPtr(rw) - Buffer List Queue Tail Pointer. Points to the next entry to be written at the tail.
*/
#define NFP3800_NBI_TM_TMBufferList_BL_QUE_CTRL3             0x00048018
#define   NFP3800_NBI_TM_TMBUFFERLIST_BL_QUE_CTRL3_BLQ_EVENT_FULL_CNT(_x) (((_x) & 0xff) << 48)
#define   NFP3800_NBI_TM_TMBUFFERLIST_BL_QUE_CTRL3_BLQ_EVENT_FULL_CNT_of(_x) (((_x) >> 48) & 0xff)
#define   NFP3800_NBI_TM_TMBUFFERLIST_BL_QUE_CTRL3_BD_FIFO_ENTRY_CNT(_x) (((_x) & 0x7) << 40)
#define   NFP3800_NBI_TM_TMBUFFERLIST_BL_QUE_CTRL3_BD_FIFO_ENTRY_CNT_of(_x) (((_x) >> 40) & 0x7)
#define   NFP3800_NBI_TM_TMBUFFERLIST_BL_QUE_CTRL3_BL_QUE_FULL BIT_ULL(39)
#define   NFP3800_NBI_TM_TMBUFFERLIST_BL_QUE_CTRL3_BL_QUE_EMPTY BIT_ULL(38)
#define   NFP3800_NBI_TM_TMBUFFERLIST_BL_QUE_CTRL3_BL_QUE_SIZE(_x) (((_x) & 0x3) << 36)
#define   NFP3800_NBI_TM_TMBUFFERLIST_BL_QUE_CTRL3_BL_QUE_SIZE_of(_x) (((_x) >> 36) & 0x3)
#define     NFP3800_NBI_TM_TMBUFFERLIST_BL_QUE_CTRL3_BL_QUE_SIZE_256 (0)
#define     NFP3800_NBI_TM_TMBUFFERLIST_BL_QUE_CTRL3_BL_QUE_SIZE_512 (1)
#define     NFP3800_NBI_TM_TMBUFFERLIST_BL_QUE_CTRL3_BL_QUE_SIZE_1K (2)
#define     NFP3800_NBI_TM_TMBUFFERLIST_BL_QUE_CTRL3_BL_QUE_SIZE_2K (3)
#define   NFP3800_NBI_TM_TMBUFFERLIST_BL_QUE_CTRL3_BL_ENTRY_CNT(_x) (((_x) & 0x7ff) << 24)
#define   NFP3800_NBI_TM_TMBUFFERLIST_BL_QUE_CTRL3_BL_ENTRY_CNT_of(_x) (((_x) >> 24) & 0x7ff)
#define   NFP3800_NBI_TM_TMBUFFERLIST_BL_QUE_CTRL3_BL_HEAD_PTR(_x) (((_x) & 0x7ff) << 12)
#define   NFP3800_NBI_TM_TMBUFFERLIST_BL_QUE_CTRL3_BL_HEAD_PTR_of(_x) (((_x) >> 12) & 0x7ff)
#define   NFP3800_NBI_TM_TMBUFFERLIST_BL_QUE_CTRL3_BL_TAIL_PTR(_x) (((_x) & 0x7ff) << 0)
#define   NFP3800_NBI_TM_TMBUFFERLIST_BL_QUE_CTRL3_BL_TAIL_PTR_of(_x) (((_x) >> 0) & 0x7ff)
/*----------------------------------------------------------------
  Register: TMPktSram_TMPktSramEntry[2048] - NBI TM Packet SRAM Entry
    Bits: 63:0  TMPktSRAMData(rw) - 64-bits of packet data within the TM Packet Buffer SRAM. Refer to Packet Buffer Entry for details
*/
#define NFP3800_NBI_TM_TMPktSram_TM_PKT_SRAM_ENTRY(_x)       (0x00060000 + (0x8 * ((_x) & 0x7ff)))
#define   NFP3800_NBI_TM_TMPKTSRAM_TM_PKT_SRAM_ENTRY_TM_PKT_SRAM_DATA(_x) (((_x) & 0xffffffffffffffff) << 0)
#define   NFP3800_NBI_TM_TMPKTSRAM_TM_PKT_SRAM_ENTRY_TM_PKT_SRAM_DATA_of(_x) (((_x) >> 0) & 0xffffffffffffffff)
/*----------------------------------------------------------------
  Register: TMHeadTailSram_TMHeadTailEntry[1024] - NBI TM Head and Tail SRAM Entry
    Bits: 25:14 HeadPointer(rw) - 13-bit Descriptor Queue Head Pointer
    Bits: 11:0  TailPointer(rw) - 13-bit Descriptor Queue Tail Pointer
*/
#define NFP3800_NBI_TM_TMHeadTailSram_TM_HEAD_TAIL_ENTRY(_x) (0x00068000 + (0x8 * ((_x) & 0x3ff)))
#define   NFP3800_NBI_TM_TMHEADTAILSRAM_TM_HEAD_TAIL_ENTRY_HEAD_POINTER(_x) (((_x) & 0xfff) << 14)
#define   NFP3800_NBI_TM_TMHEADTAILSRAM_TM_HEAD_TAIL_ENTRY_HEAD_POINTER_of(_x) (((_x) >> 14) & 0xfff)
#define   NFP3800_NBI_TM_TMHEADTAILSRAM_TM_HEAD_TAIL_ENTRY_TAIL_POINTER(_x) (((_x) & 0xfff) << 0)
#define   NFP3800_NBI_TM_TMHEADTAILSRAM_TM_HEAD_TAIL_ENTRY_TAIL_POINTER_of(_x) (((_x) >> 0) & 0xfff)
/*----------------------------------------------------------------
  Register: TMFlowControl_TMFlowControlEntry[16] - Reads 64-bits at a time of Traffic Manager Out-of-band Flow Control information
  Register: TMFlowControl_TMMacXoffFlowControlEntry[2] - Reads 64-bits at a time of Traffic Manager MAC XOFF Flow Control information
    Bits: 63:0  FlowControlData(ro) - Generic Read-Only status register for Out of Band Flow control and 128-bits of XOFF flow control from the MAC.
*/
#define NFP3800_NBI_TM_TMFlowControl_FLOW_CONTROL(_x)        (0x00070000 + (0x8 * ((_x) & 0xf)))
#define NFP3800_NBI_TM_TMFlowControl_MAC_XOFFFLOW_CONTROL(_x) (0x00070080 + (0x8 * ((_x) & 0x1)))
#define   NFP3800_NBI_TM_TMFLOWCONTROL_TMFLOWCONTROLENTRY_FLOW_CONTROL_DATA(_x) (((_x) & 0xffffffffffffffff) << 0)
#define   NFP3800_NBI_TM_TMFLOWCONTROL_TMFLOWCONTROLENTRY_FLOW_CONTROL_DATA_of(_x) (((_x) >> 0) & 0xffffffffffffffff)
/*----------------------------------------------------------------
  Register: TMFlowControl_TmMiniPktFlowControlEntry[2] - Reads 64-bits at a time of Traffic Manager Minipacket Flow Control information
    Bits: 63:0  TmMiniPktFlowControlEntry(rc) - Generic Clear-on-read status register for Minipkt Channel flow control from the MAC.
*/
#define NFP3800_NBI_TM_TMFlowControl_MINIPKT_FLOW_CONTROL(_x) (0x00070090 + (0x8 * ((_x) & 0x1)))
#define   NFP3800_NBI_TM_TMFLOWCONTROL_MINIPKT_FLOW_CONTROL_TM_MINIPKT_FLOWCONTROL_ENTRY(_x) (((_x) & 0xffffffffffffffff) << 0)
#define   NFP3800_NBI_TM_TMFLOWCONTROL_MINIPKT_FLOW_CONTROL_TM_MINIPKT_FLOWCONTROL_ENTRY_of(_x) (((_x) >> 0) & 0xffffffffffffffff)
/*----------------------------------------------------------------
  Register: TMBLQRead_BLQRead[4] - Reads a pair of entries from Buffer List Queue
    Bits: 60:32 MuAddrHi(ro) - Pointer to the memory buffer. Refer to NbiBDSramEntry register description.
    Bits: 28:0  MuAddrLo(ro) - Pointer to the memory buffer. Refer to NbiBDSramEntry register description.
*/
#define NFP3800_NBI_TM_TMBLQRead_BL_QUE_READ(_x)             (0x00080000 + (0x80 * ((_x) & 0x3)))
#define   NFP3800_NBI_TM_TMBLQREAD_BL_QUE_READ_MU_PTR_HI(_x) (((_x) & 0x1fffffff) << 32)
#define   NFP3800_NBI_TM_TMBLQREAD_BL_QUE_READ_MU_PTR_HI_of(_x) (((_x) >> 32) & 0x1fffffff)
#define   NFP3800_NBI_TM_TMBLQREAD_BL_QUE_READ_MU_PTR_LO(_x) (((_x) & 0x1fffffff) << 0)
#define   NFP3800_NBI_TM_TMBLQREAD_BL_QUE_READ_MU_PTR_LO_of(_x) (((_x) >> 0) & 0x1fffffff)
/*----------------------------------------------------------------
  Register: TMFreePtrSram_TMFreePtrSramEntry[4096] - NBI TM FreePtr SRAM Entry
    Bits: 11:0  FreePtrData(rw) - 13-bits of freeptr, initialize memory with available freeptr's
*/
#define NFP3800_NBI_TM_TMFreePtrSram_TM_FREEPTR_SRAM_ENTRY(_x) (0x000a0000 + (0x8 * ((_x) & 0xfff)))
#define   NFP3800_NBI_TM_TMFREEPTRSRAM_TM_FREEPTR_SRAM_ENTRY_FREEPTR_SRAM_DATA(_x) (((_x) & 0xfff) << 0)
#define   NFP3800_NBI_TM_TMFREEPTRSRAM_TM_FREEPTR_SRAM_ENTRY_FREEPTR_SRAM_DATA_of(_x) (((_x) >> 0) & 0xfff)
/*----------------------------------------------------------------
  Register: TMReorderBuf_TMReorderBufEntry[8192] - NBI TM Reorder Buffer Entry
    Bits: 38:0  TmReorderBufData(rw) - 39-bits of the 78-bits stored in the TM Reorder Buffer SRAM. Refer to the details on bit positions for Reorder Buffer entry table.
*/
#define NFP3800_NBI_TM_TMReorderBuf_TM_RORDR_BUF_ENTRY(_x)   (0x000d0000 + (0x8 * ((_x) & 0x1fff)))
#define   NFP3800_NBI_TM_TMREORDERBUF_TM_RORDR_BUF_ENTRY_TM_RE_BUF_DATA(_x) (((_x) & 0x7fffffffff) << 0)
#define   NFP3800_NBI_TM_TMREORDERBUF_TM_RORDR_BUF_ENTRY_TM_RE_BUF_DATA_of(_x) (((_x) >> 0) & 0x7fffffffff)
/*----------------------------------------------------------------
  Register: TMQueueLevelSram_TMQueueLevelEntry[1024] - NBI TM Queue Level SRAM Entry
    Bits: 14    QueueFullFlag(ro) - Queue full flag
    Bits: 12:0  QueueLevel(rw) - 13-bit Queue Level
*/
#define NFP3800_NBI_TM_TMQueueLevelSram_TM_QUEUE_LEVEL_ENTRY(_x) (0x000e0000 + (0x8 * ((_x) & 0x3ff)))
#define   NFP3800_NBI_TM_TMQUEUELEVELSRAM_TM_QUEUE_LEVEL_ENTRY_QUEUE_FULL_FLAG BIT(14)
#define   NFP3800_NBI_TM_TMQUEUELEVELSRAM_TM_QUEUE_LEVEL_ENTRY_QUEUE_LEVEL(_x) (((_x) & 0x1fff) << 0)
#define   NFP3800_NBI_TM_TMQUEUELEVELSRAM_TM_QUEUE_LEVEL_ENTRY_QUEUE_LEVEL_of(_x) (((_x) >> 0) & 0x1fff)
/*----------------------------------------------------------------
  Register: TrafficManagerConfig - Traffic manager configuration register. Contains global read/write configuration information.
    Bits: 29    ShaperReduceThreeFourthEnable(rw) - This bit enables Three Fourth Threshold on Shaper and its only in effect if ShaperReduceRateEnable is set.
    Bits: 28    ShaperReduceRateEnable(rw) - This bit enables Reduce rate Shaper threshold
    Bits: 27    FreePktVSAddressBitSet(rw) - In case of Drop sending a FreePkt command vs Sending a PCIE Push command with Address[31] set
    Bits: 26:25 UseTwoPullIDBuses(rw) - Enables using of both the PullID buses for CTM packets.
    Bits: 24    FlowControlEventEna(rw) - Enables Flow Control Event notifications to the Software.
    Bits: 23    PktReadyC0Enable(rw) - Eanbles decoding of previously unused bits in the Packet Ready Command for new C0 functions.
    Bits: 22    TmFreePtrNoReturn(rw) - Enable mode for Multicast Packets stream. Also, used in FreePacket Command when a Multicast Packet is dropped when the queues are full to notify PE to not return the MuPtr.
    Bits: 20    StartLinkedListInit(rw) - It triggers the initialization of linked list FreePtr Ram and Head and Tail Pointer Ram.
    Bits: 17    PacketDescriptorLinkedListEna(rw) - Enables the linked list approach to using Packet Descriptor Memory.
    Bits: 16    OOBFCEnable(rw) - MAC Out of Band Flow Control Enable. Enable the Out of Band Flow Control from the MAC external interface to shut off the 1024 level 2 scheduler ports.
    Bits: 15:13 L0ChannelMap(rw) - MAC channel number to Level 0 Scheduler port number mapping register. This mapping register is valid only when L1InputSelect = 1 and ChannelLevelSelect = 0. In this configuration there are up to 32 L0 Scheduler ports used. One MAC channel may map to multiple L0 Scheduler ports, and an offset of 64 can be used to support multiple Hydras.
    Bits:  9    ErrPoisonEna(rw) - It enables the poisoning of data and/or enables the other error porpagation mechanisms in case of errors.
    Bits:  8    SchedulerEnable(rw) - Scheduler Enable bit. Set this bit to enable the scheduling of traffic, clear to disable.
    Bits:  7    ShaperEnable(rw) - Shaper Enable bit. Enables the Rate Shaper to start rate limiting traffic based on the configured values. The default is off so that the shaper is disabled prior to configuration.
    Bits:  6    MiniPacketFCEnable(rw) - Minipacket Credit based Flow Control enable bit. Clear this bit to disable minipacket flow control. This bit is set to enable flow control by default.
    Bits:  5    ChannelLevelSelect(rw) - Channel level select bit. Select the definition of the MAC channel number and the MAC XOFF Flow control to be either the 128 level 0 scheduler ports, or 128 ports on the first 16 level 1 schedulers (or level 2 schedulers depending on the value of L1InputSelect). Note: When L1InputSelect=0 and ChannelLevelSelect=1, only 128 queues are allowed to be active in this configuration.
    Bits:  3:1  NumSequencers(rw) - Number of active sequencers. Specifies the number of active sequencers in the traffic manager.
    Bits:  0    L1InputSelect(rw) - Select the operation of the Level 1 schedulers. This bit disables or enables the Level 1 schedulers, and allows for two or three levels of scheduling in the Traffic Manager. When disabled, the Level 2 scheduler outputs are connected directly to the Level 0 scheduler inputs. When enabled, Level 2 outputs connect to Level 1 inputs, and Level 1 outputs connects to Level 0 inputs
*/
#define NFP3800_NBI_TMX_CSR_TRAFFIC_MANAGER_CONFIG           0x00000000
#define   NFP3800_NBI_TMX_CSR_TRAFFIC_MANAGER_CONFIG_SHAPER_REDUCE_THREE_FOURTH_ENABLE BIT(29)
#define   NFP3800_NBI_TMX_CSR_TRAFFIC_MANAGER_CONFIG_SHAPER_REDUCE_RATE_ENABLE BIT(28)
#define   NFP3800_NBI_TMX_CSR_TRAFFIC_MANAGER_CONFIG_FREE_PKT_VS_ADDRESS_BIT_SET BIT(27)
#define     NFP3800_NBI_TMX_CSR_TRAFFIC_MANAGER_CONFIG_FREE_PKT_VS_ADDRESS_BIT_SET_0 (0 << 27)
#define     NFP3800_NBI_TMX_CSR_TRAFFIC_MANAGER_CONFIG_FREE_PKT_VS_ADDRESS_BIT_SET_1 BIT(27)
#define   NFP3800_NBI_TMX_CSR_TRAFFIC_MANAGER_CONFIG_USETWOPULLIDBUSES(_x) (((_x) & 0x3) << 25)
#define   NFP3800_NBI_TMX_CSR_TRAFFIC_MANAGER_CONFIG_USETWOPULLIDBUSES_of(_x) (((_x) >> 25) & 0x3)
#define     NFP3800_NBI_TMX_CSR_TRAFFIC_MANAGER_CONFIG_USETWOPULLIDBUSES_0 (0)
#define     NFP3800_NBI_TMX_CSR_TRAFFIC_MANAGER_CONFIG_USETWOPULLIDBUSES_1 (1)
#define     NFP3800_NBI_TMX_CSR_TRAFFIC_MANAGER_CONFIG_USETWOPULLIDBUSES_2 (2)
#define     NFP3800_NBI_TMX_CSR_TRAFFIC_MANAGER_CONFIG_USETWOPULLIDBUSES_3 (3)
#define   NFP3800_NBI_TMX_CSR_TRAFFIC_MANAGER_CONFIG_FLOWCONTROLEVENTENA BIT(24)
#define   NFP3800_NBI_TMX_CSR_TRAFFIC_MANAGER_CONFIG_PKTREADYC0ENABLE BIT(23)
#define   NFP3800_NBI_TMX_CSR_TRAFFIC_MANAGER_CONFIG_TMFREEPTRNORETURN BIT(22)
#define   NFP3800_NBI_TMX_CSR_TRAFFIC_MANAGER_CONFIG_STARTLINKEDLISTINIT BIT(20)
#define   NFP3800_NBI_TMX_CSR_TRAFFIC_MANAGER_CONFIG_PACKETDESCRIPTORLINKEDLISTENA BIT(17)
#define     NFP3800_NBI_TMX_CSR_TRAFFIC_MANAGER_CONFIG_PACKETDESCRIPTORLINKEDLISTENA_DISABLE (0 << 17)
#define     NFP3800_NBI_TMX_CSR_TRAFFIC_MANAGER_CONFIG_PACKETDESCRIPTORLINKEDLISTENA_ENABLE BIT(17)
#define   NFP3800_NBI_TMX_CSR_TRAFFIC_MANAGER_CONFIG_OOBFCENABLE BIT(16)
#define     NFP3800_NBI_TMX_CSR_TRAFFIC_MANAGER_CONFIG_OOBFCENABLE_DISABLE (0 << 16)
#define     NFP3800_NBI_TMX_CSR_TRAFFIC_MANAGER_CONFIG_OOBFCENABLE_ENABLE BIT(16)
#define   NFP3800_NBI_TMX_CSR_TRAFFIC_MANAGER_CONFIG_L0CHANNELMAP(_x) (((_x) & 0x7) << 13)
#define   NFP3800_NBI_TMX_CSR_TRAFFIC_MANAGER_CONFIG_L0CHANNELMAP_of(_x) (((_x) >> 13) & 0x7)
#define     NFP3800_NBI_TMX_CSR_TRAFFIC_MANAGER_CONFIG_L0CHANNELMAP_0 (0)
#define     NFP3800_NBI_TMX_CSR_TRAFFIC_MANAGER_CONFIG_L0CHANNELMAP_1 (1)
#define     NFP3800_NBI_TMX_CSR_TRAFFIC_MANAGER_CONFIG_L0CHANNELMAP_2 (2)
#define     NFP3800_NBI_TMX_CSR_TRAFFIC_MANAGER_CONFIG_L0CHANNELMAP_3 (3)
#define     NFP3800_NBI_TMX_CSR_TRAFFIC_MANAGER_CONFIG_L0CHANNELMAP_4 (4)
#define     NFP3800_NBI_TMX_CSR_TRAFFIC_MANAGER_CONFIG_L0CHANNELMAP_5 (5)
#define     NFP3800_NBI_TMX_CSR_TRAFFIC_MANAGER_CONFIG_L0CHANNELMAP_6 (6)
#define     NFP3800_NBI_TMX_CSR_TRAFFIC_MANAGER_CONFIG_L0CHANNELMAP_7 (7)
#define   NFP3800_NBI_TMX_CSR_TRAFFIC_MANAGER_CONFIG_ERR_POISON_ENA BIT(9)
#define     NFP3800_NBI_TMX_CSR_TRAFFIC_MANAGER_CONFIG_ERR_POISON_ENA_DISABLE (0 << 9)
#define     NFP3800_NBI_TMX_CSR_TRAFFIC_MANAGER_CONFIG_ERR_POISON_ENA_ENABLE BIT(9)
#define   NFP3800_NBI_TMX_CSR_TRAFFIC_MANAGER_CONFIG_SCHEDULERENABLE BIT(8)
#define     NFP3800_NBI_TMX_CSR_TRAFFIC_MANAGER_CONFIG_SCHEDULERENABLE_DISABLE (0 << 8)
#define     NFP3800_NBI_TMX_CSR_TRAFFIC_MANAGER_CONFIG_SCHEDULERENABLE_ENABLE BIT(8)
#define   NFP3800_NBI_TMX_CSR_TRAFFIC_MANAGER_CONFIG_SHAPERENABLE BIT(7)
#define     NFP3800_NBI_TMX_CSR_TRAFFIC_MANAGER_CONFIG_SHAPERENABLE_DISABLE (0 << 7)
#define     NFP3800_NBI_TMX_CSR_TRAFFIC_MANAGER_CONFIG_SHAPERENABLE_ENABLE BIT(7)
#define   NFP3800_NBI_TMX_CSR_TRAFFIC_MANAGER_CONFIG_MINIPACKETFCENABLE BIT(6)
#define     NFP3800_NBI_TMX_CSR_TRAFFIC_MANAGER_CONFIG_MINIPACKETFCENABLE_DISABLE (0 << 6)
#define     NFP3800_NBI_TMX_CSR_TRAFFIC_MANAGER_CONFIG_MINIPACKETFCENABLE_ENABLE BIT(6)
#define   NFP3800_NBI_TMX_CSR_TRAFFIC_MANAGER_CONFIG_CHANNELLEVELSELECT BIT(5)
#define     NFP3800_NBI_TMX_CSR_TRAFFIC_MANAGER_CONFIG_CHANNELLEVELSELECT_LEVEL_0 (0 << 5)
#define     NFP3800_NBI_TMX_CSR_TRAFFIC_MANAGER_CONFIG_CHANNELLEVELSELECT_LEVEL_1 BIT(5)
#define   NFP3800_NBI_TMX_CSR_TRAFFIC_MANAGER_CONFIG_NUMSEQUENCERS(_x) (((_x) & 0x7) << 1)
#define   NFP3800_NBI_TMX_CSR_TRAFFIC_MANAGER_CONFIG_NUMSEQUENCERS_of(_x) (((_x) >> 1) & 0x7)
#define     NFP3800_NBI_TMX_CSR_TRAFFIC_MANAGER_CONFIG_NUMSEQUENCERS_0 (0)
#define     NFP3800_NBI_TMX_CSR_TRAFFIC_MANAGER_CONFIG_NUMSEQUENCERS_1 (1)
#define     NFP3800_NBI_TMX_CSR_TRAFFIC_MANAGER_CONFIG_NUMSEQUENCERS_2 (2)
#define     NFP3800_NBI_TMX_CSR_TRAFFIC_MANAGER_CONFIG_NUMSEQUENCERS_4 (3)
#define     NFP3800_NBI_TMX_CSR_TRAFFIC_MANAGER_CONFIG_NUMSEQUENCERS_8 (4)
#define     NFP3800_NBI_TMX_CSR_TRAFFIC_MANAGER_CONFIG_NUMSEQUENCERS_16 (5)
#define     NFP3800_NBI_TMX_CSR_TRAFFIC_MANAGER_CONFIG_NUMSEQUENCERS_NA (6)
#define   NFP3800_NBI_TMX_CSR_TRAFFIC_MANAGER_CONFIG_L1INPUTSELECT BIT(0)
#define     NFP3800_NBI_TMX_CSR_TRAFFIC_MANAGER_CONFIG_L1INPUTSELECT_L1_DISABLE (0 << 0)
#define     NFP3800_NBI_TMX_CSR_TRAFFIC_MANAGER_CONFIG_L1INPUTSELECT_L1_ENABLE BIT(0)
/*----------------------------------------------------------------
  Register: DropRate - Configurable drop rate that is programmable for each queue depth range
    Bits: 23:21 DropRateRangeB3(rw) - Drop Rate for Queue Depth Range B-3. Top 75% to 100% of the descriptor queue. Packets will be dropped n% of the time if the queue depth is within this range
    Bits: 20:18 DropRateRangeB2(rw) - Drop Rate for Queue Depth Range B-2. 50% to 74% full. Packets will be dropped n% of the time if the queue depth is within this range
    Bits: 17:15 DropRateRangeB1(rw) - Drop Rate for Queue Depth Range B-1. 25% to 49% full. Packets will be dropped n% of the time if the queue depth is within this range
    Bits: 14:12 DropRateRangeB0(rw) - Drop Rate for Queue Depth Range B-0. Bottom 0 to 24% of the descriptor queue. Packets will be dropped n% of the time if the queue depth is within this range
    Bits: 11:9  DropRateRangeA3(rw) - Drop Rate for Queue Depth Range A-3. Top 75% to 100% of the descriptor queue. Packets will be dropped n% of the time if the queue depth is within this range
    Bits:  8:6  DropRateRangeA2(rw) - Drop Rate for Queue Depth Range A-2. 50% to 74% full. Packets will be dropped n% of the time if the queue depth is within this range
    Bits:  5:3  DropRateRangeA1(rw) - Drop Rate for Queue Depth Range A-1. 25% to 49% full. Packets will be dropped n% of the time if the queue depth is within this range
    Bits:  2:0  DropRateRangeA0(rw) - Drop Rate for Queue Depth Range A-0. Bottom 0 to 24% of the descriptor queue. Packets will be dropped n% of the time if the queue depth is within this range
*/
#define NFP3800_NBI_TMX_CSR_DROP_RATE                        0x00000004
#define   NFP3800_NBI_TMX_CSR_DROP_RATE_DROPRATERANGEB3(_x)  (((_x) & 0x7) << 21)
#define   NFP3800_NBI_TMX_CSR_DROP_RATE_DROPRATERANGEB3_of(_x) (((_x) >> 21) & 0x7)
#define     NFP3800_NBI_TMX_CSR_DROP_RATE_DROPRATERANGEB3_0  (0)
#define     NFP3800_NBI_TMX_CSR_DROP_RATE_DROPRATERANGEB3_12_5 (1)
#define     NFP3800_NBI_TMX_CSR_DROP_RATE_DROPRATERANGEB3_25 (2)
#define     NFP3800_NBI_TMX_CSR_DROP_RATE_DROPRATERANGEB3_37_5 (3)
#define     NFP3800_NBI_TMX_CSR_DROP_RATE_DROPRATERANGEB3_50 (4)
#define     NFP3800_NBI_TMX_CSR_DROP_RATE_DROPRATERANGEB3_62_5 (5)
#define     NFP3800_NBI_TMX_CSR_DROP_RATE_DROPRATERANGEB3_75 (6)
#define     NFP3800_NBI_TMX_CSR_DROP_RATE_DROPRATERANGEB3_87_5 (7)
#define   NFP3800_NBI_TMX_CSR_DROP_RATE_DROPRATERANGEB2(_x)  (((_x) & 0x7) << 18)
#define   NFP3800_NBI_TMX_CSR_DROP_RATE_DROPRATERANGEB2_of(_x) (((_x) >> 18) & 0x7)
#define     NFP3800_NBI_TMX_CSR_DROP_RATE_DROPRATERANGEB2_0  (0)
#define     NFP3800_NBI_TMX_CSR_DROP_RATE_DROPRATERANGEB2_12_5 (1)
#define     NFP3800_NBI_TMX_CSR_DROP_RATE_DROPRATERANGEB2_25 (2)
#define     NFP3800_NBI_TMX_CSR_DROP_RATE_DROPRATERANGEB2_37_5 (3)
#define     NFP3800_NBI_TMX_CSR_DROP_RATE_DROPRATERANGEB2_50 (4)
#define     NFP3800_NBI_TMX_CSR_DROP_RATE_DROPRATERANGEB2_62_5 (5)
#define     NFP3800_NBI_TMX_CSR_DROP_RATE_DROPRATERANGEB2_75 (6)
#define     NFP3800_NBI_TMX_CSR_DROP_RATE_DROPRATERANGEB2_87_5 (7)
#define   NFP3800_NBI_TMX_CSR_DROP_RATE_DROPRATERANGEB1(_x)  (((_x) & 0x7) << 15)
#define   NFP3800_NBI_TMX_CSR_DROP_RATE_DROPRATERANGEB1_of(_x) (((_x) >> 15) & 0x7)
#define     NFP3800_NBI_TMX_CSR_DROP_RATE_DROPRATERANGEB1_0  (0)
#define     NFP3800_NBI_TMX_CSR_DROP_RATE_DROPRATERANGEB1_12_5 (1)
#define     NFP3800_NBI_TMX_CSR_DROP_RATE_DROPRATERANGEB1_25 (2)
#define     NFP3800_NBI_TMX_CSR_DROP_RATE_DROPRATERANGEB1_37_5 (3)
#define     NFP3800_NBI_TMX_CSR_DROP_RATE_DROPRATERANGEB1_50 (4)
#define     NFP3800_NBI_TMX_CSR_DROP_RATE_DROPRATERANGEB1_62_5 (5)
#define     NFP3800_NBI_TMX_CSR_DROP_RATE_DROPRATERANGEB1_75 (6)
#define     NFP3800_NBI_TMX_CSR_DROP_RATE_DROPRATERANGEB1_87_5 (7)
#define   NFP3800_NBI_TMX_CSR_DROP_RATE_DROPRATERANGEB0(_x)  (((_x) & 0x7) << 12)
#define   NFP3800_NBI_TMX_CSR_DROP_RATE_DROPRATERANGEB0_of(_x) (((_x) >> 12) & 0x7)
#define     NFP3800_NBI_TMX_CSR_DROP_RATE_DROPRATERANGEB0_0  (0)
#define     NFP3800_NBI_TMX_CSR_DROP_RATE_DROPRATERANGEB0_12_5 (1)
#define     NFP3800_NBI_TMX_CSR_DROP_RATE_DROPRATERANGEB0_25 (2)
#define     NFP3800_NBI_TMX_CSR_DROP_RATE_DROPRATERANGEB0_37_5 (3)
#define     NFP3800_NBI_TMX_CSR_DROP_RATE_DROPRATERANGEB0_50 (4)
#define     NFP3800_NBI_TMX_CSR_DROP_RATE_DROPRATERANGEB0_62_5 (5)
#define     NFP3800_NBI_TMX_CSR_DROP_RATE_DROPRATERANGEB0_75 (6)
#define     NFP3800_NBI_TMX_CSR_DROP_RATE_DROPRATERANGEB0_87_5 (7)
#define   NFP3800_NBI_TMX_CSR_DROP_RATE_DROPRATERANGEA3(_x)  (((_x) & 0x7) << 9)
#define   NFP3800_NBI_TMX_CSR_DROP_RATE_DROPRATERANGEA3_of(_x) (((_x) >> 9) & 0x7)
#define     NFP3800_NBI_TMX_CSR_DROP_RATE_DROPRATERANGEA3_0  (0)
#define     NFP3800_NBI_TMX_CSR_DROP_RATE_DROPRATERANGEA3_12_5 (1)
#define     NFP3800_NBI_TMX_CSR_DROP_RATE_DROPRATERANGEA3_25 (2)
#define     NFP3800_NBI_TMX_CSR_DROP_RATE_DROPRATERANGEA3_37_5 (3)
#define     NFP3800_NBI_TMX_CSR_DROP_RATE_DROPRATERANGEA3_50 (4)
#define     NFP3800_NBI_TMX_CSR_DROP_RATE_DROPRATERANGEA3_62_5 (5)
#define     NFP3800_NBI_TMX_CSR_DROP_RATE_DROPRATERANGEA3_75 (6)
#define     NFP3800_NBI_TMX_CSR_DROP_RATE_DROPRATERANGEA3_87_5 (7)
#define   NFP3800_NBI_TMX_CSR_DROP_RATE_DROPRATERANGEA2(_x)  (((_x) & 0x7) << 6)
#define   NFP3800_NBI_TMX_CSR_DROP_RATE_DROPRATERANGEA2_of(_x) (((_x) >> 6) & 0x7)
#define     NFP3800_NBI_TMX_CSR_DROP_RATE_DROPRATERANGEA2_0  (0)
#define     NFP3800_NBI_TMX_CSR_DROP_RATE_DROPRATERANGEA2_12_5 (1)
#define     NFP3800_NBI_TMX_CSR_DROP_RATE_DROPRATERANGEA2_25 (2)
#define     NFP3800_NBI_TMX_CSR_DROP_RATE_DROPRATERANGEA2_37_5 (3)
#define     NFP3800_NBI_TMX_CSR_DROP_RATE_DROPRATERANGEA2_50 (4)
#define     NFP3800_NBI_TMX_CSR_DROP_RATE_DROPRATERANGEA2_62_5 (5)
#define     NFP3800_NBI_TMX_CSR_DROP_RATE_DROPRATERANGEA2_75 (6)
#define     NFP3800_NBI_TMX_CSR_DROP_RATE_DROPRATERANGEA2_87_5 (7)
#define   NFP3800_NBI_TMX_CSR_DROP_RATE_DROPRATERANGEA1(_x)  (((_x) & 0x7) << 3)
#define   NFP3800_NBI_TMX_CSR_DROP_RATE_DROPRATERANGEA1_of(_x) (((_x) >> 3) & 0x7)
#define     NFP3800_NBI_TMX_CSR_DROP_RATE_DROPRATERANGEA1_0  (0)
#define     NFP3800_NBI_TMX_CSR_DROP_RATE_DROPRATERANGEA1_12_5 (1)
#define     NFP3800_NBI_TMX_CSR_DROP_RATE_DROPRATERANGEA1_25 (2)
#define     NFP3800_NBI_TMX_CSR_DROP_RATE_DROPRATERANGEA1_37_5 (3)
#define     NFP3800_NBI_TMX_CSR_DROP_RATE_DROPRATERANGEA1_50 (4)
#define     NFP3800_NBI_TMX_CSR_DROP_RATE_DROPRATERANGEA1_62_5 (5)
#define     NFP3800_NBI_TMX_CSR_DROP_RATE_DROPRATERANGEA1_75 (6)
#define     NFP3800_NBI_TMX_CSR_DROP_RATE_DROPRATERANGEA1_87_5 (7)
#define   NFP3800_NBI_TMX_CSR_DROP_RATE_DROPRATERANGEA0(_x)  (((_x) & 0x7) << 0)
#define   NFP3800_NBI_TMX_CSR_DROP_RATE_DROPRATERANGEA0_of(_x) (((_x) >> 0) & 0x7)
#define     NFP3800_NBI_TMX_CSR_DROP_RATE_DROPRATERANGEA0_0  (0)
#define     NFP3800_NBI_TMX_CSR_DROP_RATE_DROPRATERANGEA0_12_5 (1)
#define     NFP3800_NBI_TMX_CSR_DROP_RATE_DROPRATERANGEA0_25 (2)
#define     NFP3800_NBI_TMX_CSR_DROP_RATE_DROPRATERANGEA0_37_5 (3)
#define     NFP3800_NBI_TMX_CSR_DROP_RATE_DROPRATERANGEA0_50 (4)
#define     NFP3800_NBI_TMX_CSR_DROP_RATE_DROPRATERANGEA0_62_5 (5)
#define     NFP3800_NBI_TMX_CSR_DROP_RATE_DROPRATERANGEA0_75 (6)
#define     NFP3800_NBI_TMX_CSR_DROP_RATE_DROPRATERANGEA0_87_5 (7)
/*----------------------------------------------------------------
  Register: BlqEvent - Configures thresholds, event type and enables/disables for the buffer list queues
    Bits: 23    ReducedBLQThreshold(rw) - Reduced Thresholds would be used when this bit is set.
    Bits: 22    BypassBLQPtrReturn(rw) - When set to '1', drops Null MU Pointers (29h0) instead of returning them to the BLQ.
    Bits: 20    DisBLQEventFifoFullAssert(rw) - Set this bit to disable the BLQ Event FIFO overflow assertion
    Bits: 19:18 BLQEventThreshold3(rw) - An event is generated every N buffer descriptors written to buffer list queue3, where N is configurable from 16 to 128 or 4 to 32 if Reduced BLQ Threshold is set..
    Bits: 17:16 BLQEventThreshold2(rw) - An event is generated every N buffer descriptors written to buffer list queue3, where N is configurable from 16 to 128 or 4 to 32 if Reduced BLQ Threshold is set..
    Bits: 15:14 BLQEventThreshold1(rw) - An event is generated every N buffer descriptors written to buffer list queue3, where N is configurable from 16 to 128 or 4 to 32 if Reduced BLQ Threshold is set..
    Bits: 13:12 BLQEventThreshold0(rw) - An event is generated every N buffer descriptors written to buffer list queue3, where N is configurable from 16 to 128 or 4 to 32 if Reduced BLQ Threshold is set..
    Bits:  7:4  BLQEventNotEmptyEnable(rw) - One bit per buffer list queue. Enables the generation of BLQ Events when the queue transitions from empty to not empty. Disabled when clear. The event type value used is 0x0 (event_type_fifo_not_empty)
    Bits:  3:0  BLQEventStatusEnable(rw) - One bit per buffer list queue. Enables BLQ Status Events when set. Disabled when clear. A Status Event is sent when the number of entries written to the queue reaches the programmed value in the BLQEventThreshold register. The event type value used is 0x5 (event_type_value_update)
*/
#define NFP3800_NBI_TMX_CSR_BLQ_EVENT                        0x00000008
#define   NFP3800_NBI_TMX_CSR_BLQ_EVENT_REDUCED_BLQ_THRESHOLD BIT(23)
#define   NFP3800_NBI_TMX_CSR_BLQ_EVENT_BYPASS_BLQ_PTR_RETURN BIT(22)
#define   NFP3800_NBI_TMX_CSR_BLQ_EVENT_DIS_BLQ_EVENT_FIFO_FULL_ASSERT BIT(20)
#define   NFP3800_NBI_TMX_CSR_BLQ_EVENT_BLQEVENT_THRESHOLD3(_x) (((_x) & 0x3) << 18)
#define   NFP3800_NBI_TMX_CSR_BLQ_EVENT_BLQEVENT_THRESHOLD3_of(_x) (((_x) >> 18) & 0x3)
#define     NFP3800_NBI_TMX_CSR_BLQ_EVENT_BLQEVENT_THRESHOLD3_16_OR_4 (0)
#define     NFP3800_NBI_TMX_CSR_BLQ_EVENT_BLQEVENT_THRESHOLD3_32_OR_8 (1)
#define     NFP3800_NBI_TMX_CSR_BLQ_EVENT_BLQEVENT_THRESHOLD3_64_OR_16 (2)
#define     NFP3800_NBI_TMX_CSR_BLQ_EVENT_BLQEVENT_THRESHOLD3_128_OR_32 (3)
#define   NFP3800_NBI_TMX_CSR_BLQ_EVENT_BLQEVENT_THRESHOLD2(_x) (((_x) & 0x3) << 16)
#define   NFP3800_NBI_TMX_CSR_BLQ_EVENT_BLQEVENT_THRESHOLD2_of(_x) (((_x) >> 16) & 0x3)
#define     NFP3800_NBI_TMX_CSR_BLQ_EVENT_BLQEVENT_THRESHOLD2_16_OR_4 (0)
#define     NFP3800_NBI_TMX_CSR_BLQ_EVENT_BLQEVENT_THRESHOLD2_32_OR_8 (1)
#define     NFP3800_NBI_TMX_CSR_BLQ_EVENT_BLQEVENT_THRESHOLD2_64_OR_16 (2)
#define     NFP3800_NBI_TMX_CSR_BLQ_EVENT_BLQEVENT_THRESHOLD2_128_OR_32 (3)
#define   NFP3800_NBI_TMX_CSR_BLQ_EVENT_BLQEVENT_THRESHOLD1(_x) (((_x) & 0x3) << 14)
#define   NFP3800_NBI_TMX_CSR_BLQ_EVENT_BLQEVENT_THRESHOLD1_of(_x) (((_x) >> 14) & 0x3)
#define     NFP3800_NBI_TMX_CSR_BLQ_EVENT_BLQEVENT_THRESHOLD1_16_OR_4 (0)
#define     NFP3800_NBI_TMX_CSR_BLQ_EVENT_BLQEVENT_THRESHOLD1_32_OR_8 (1)
#define     NFP3800_NBI_TMX_CSR_BLQ_EVENT_BLQEVENT_THRESHOLD1_64_OR_16 (2)
#define     NFP3800_NBI_TMX_CSR_BLQ_EVENT_BLQEVENT_THRESHOLD1_128_OR_32 (3)
#define   NFP3800_NBI_TMX_CSR_BLQ_EVENT_BLQEVENT_THRESHOLD0(_x) (((_x) & 0x3) << 12)
#define   NFP3800_NBI_TMX_CSR_BLQ_EVENT_BLQEVENT_THRESHOLD0_of(_x) (((_x) >> 12) & 0x3)
#define     NFP3800_NBI_TMX_CSR_BLQ_EVENT_BLQEVENT_THRESHOLD0_16_OR_4 (0)
#define     NFP3800_NBI_TMX_CSR_BLQ_EVENT_BLQEVENT_THRESHOLD0_32_OR_8 (1)
#define     NFP3800_NBI_TMX_CSR_BLQ_EVENT_BLQEVENT_THRESHOLD0_64_OR_16 (2)
#define     NFP3800_NBI_TMX_CSR_BLQ_EVENT_BLQEVENT_THRESHOLD0_128_OR_32 (3)
#define   NFP3800_NBI_TMX_CSR_BLQ_EVENT_BLQ_EVENT_NOT_EMPTY_ENABLE(_x) (((_x) & 0xf) << 4)
#define   NFP3800_NBI_TMX_CSR_BLQ_EVENT_BLQ_EVENT_NOT_EMPTY_ENABLE_of(_x) (((_x) >> 4) & 0xf)
#define     NFP3800_NBI_TMX_CSR_BLQ_EVENT_BLQ_EVENT_NOT_EMPTY_ENABLE_DISABLE (0)
#define     NFP3800_NBI_TMX_CSR_BLQ_EVENT_BLQ_EVENT_NOT_EMPTY_ENABLE_ENABLE (1)
#define   NFP3800_NBI_TMX_CSR_BLQ_EVENT_BLQ_EVENT_STATUS_ENABLE(_x) (((_x) & 0xf) << 0)
#define   NFP3800_NBI_TMX_CSR_BLQ_EVENT_BLQ_EVENT_STATUS_ENABLE_of(_x) (((_x) >> 0) & 0xf)
#define     NFP3800_NBI_TMX_CSR_BLQ_EVENT_BLQ_EVENT_STATUS_ENABLE_DISABLE (0)
#define     NFP3800_NBI_TMX_CSR_BLQ_EVENT_BLQ_EVENT_STATUS_ENABLE_ENABLE (1)
/*----------------------------------------------------------------
  Register: EgressRateLimit - Rate limiting CSR for the Egress NBI Command/PullId ports.
    Bits:  3    EgressRateLimitOff(rw) - Turns the rate limiting off for both Ports 6 and 7 (P6 and P7).
    Bits:  2:0  EgressRate(rw) - Rate limiting control for the Command/PullId Ports 6 and 7 (P6 and P7).
*/
#define NFP3800_NBI_TMX_CSR_EGRESS_RATE_LIMIT                0x00000074
#define   NFP3800_NBI_TMX_CSR_EGRESS_RATE_LIMIT_EGRESS_RATE_LIMIT_OFF BIT(3)
#define     NFP3800_NBI_TMX_CSR_EGRESS_RATE_LIMIT_EGRESS_RATE_LIMIT_OFF_ON (0 << 3)
#define     NFP3800_NBI_TMX_CSR_EGRESS_RATE_LIMIT_EGRESS_RATE_LIMIT_OFF_OFF BIT(3)
#define   NFP3800_NBI_TMX_CSR_EGRESS_RATE_LIMIT_EGRESS_RATE(_x) (((_x) & 0x7) << 0)
#define   NFP3800_NBI_TMX_CSR_EGRESS_RATE_LIMIT_EGRESS_RATE_of(_x) (((_x) >> 0) & 0x7)
#define     NFP3800_NBI_TMX_CSR_EGRESS_RATE_LIMIT_EGRESS_RATE_64G (0)
#define     NFP3800_NBI_TMX_CSR_EGRESS_RATE_LIMIT_EGRESS_RATE_32G (1)
#define     NFP3800_NBI_TMX_CSR_EGRESS_RATE_LIMIT_EGRESS_RATE_16G (2)
#define     NFP3800_NBI_TMX_CSR_EGRESS_RATE_LIMIT_EGRESS_RATE_8G (3)
#define     NFP3800_NBI_TMX_CSR_EGRESS_RATE_LIMIT_EGRESS_RATE_128G (4)
#define     NFP3800_NBI_TMX_CSR_EGRESS_RATE_LIMIT_EGRESS_RATE_112G (5)
#define     NFP3800_NBI_TMX_CSR_EGRESS_RATE_LIMIT_EGRESS_RATE_96G (6)
#define     NFP3800_NBI_TMX_CSR_EGRESS_RATE_LIMIT_EGRESS_RATE_80G (7)
/*----------------------------------------------------------------
  Register: EgressPullIdPortEnable - Enables ports for transmitting Pull IDs.
    Bits:  1:0  EgressPullIdPortEnable(rw) - Egress Port enable for transmitting PullIDs.
*/
#define NFP3800_NBI_TMX_CSR_EGRESS_PULL_ID_PORT_ENABLE       0x00000078
#define   NFP3800_NBI_TMX_CSR_EGRESS_PULL_ID_PORT_ENABLE_EGRESS_PULL_ID_PORT_ENABLE(_x) (((_x) & 0x3) << 0)
#define   NFP3800_NBI_TMX_CSR_EGRESS_PULL_ID_PORT_ENABLE_EGRESS_PULL_ID_PORT_ENABLE_of(_x) (((_x) >> 0) & 0x3)
#define     NFP3800_NBI_TMX_CSR_EGRESS_PULL_ID_PORT_ENABLE_EGRESS_PULL_ID_PORT_ENABLE_P6_AND_P7 (0)
#define     NFP3800_NBI_TMX_CSR_EGRESS_PULL_ID_PORT_ENABLE_EGRESS_PULL_ID_PORT_ENABLE_P6 (1)
#define     NFP3800_NBI_TMX_CSR_EGRESS_PULL_ID_PORT_ENABLE_EGRESS_PULL_ID_PORT_ENABLE_P7 (2)
#define     NFP3800_NBI_TMX_CSR_EGRESS_PULL_ID_PORT_ENABLE_EGRESS_PULL_ID_PORT_ENABLE_UNUSED (3)
/*----------------------------------------------------------------
  Register: OutOfOrderCount[16] - Out of order counter for each sequencer. There are a total of 32 out of order counters in the traffic manager, one for each active sequencer.
    Bits: 15:0  OutOfOrderCount(ro) - Out-of-order counter to keep track of the number of packets sent out of order for this sequencer. The counter will saturate when the maximum number is reached.
*/
#define NFP3800_NBI_TMX_CSR_OUT_OF_ORDER_COUNT(_x)           (0x00000100 + (0x4 * ((_x) & 0xf)))
#define   NFP3800_NBI_TMX_CSR_OUT_OF_ORDER_COUNT_OUTOFORDERCOUNT(_x) (((_x) & 0xffff) << 0)
#define   NFP3800_NBI_TMX_CSR_OUT_OF_ORDER_COUNT_OUTOFORDERCOUNT_of(_x) (((_x) >> 0) & 0xffff)
/*----------------------------------------------------------------
  Register: OutOfOrderCountClear[16] - Clear on read out of order counter for each sequencer. One for each active sequencer.
    Bits: 15:0  OutOfOrderCountClear(rc) - Clear on read out-of-order counter to keep track of the number of packets sent out of order for this sequencer. The counter will saturate when the maximum number is reached. The read value for this register is identical to the OUTOFORDERCOUNT register value, however when this register is read the out-of-order counter is cleared.
*/
#define NFP3800_NBI_TMX_CSR_OUT_OF_ORDER_COUNT_CLEAR(_x)     (0x00000200 + (0x4 * ((_x) & 0xf)))
#define   NFP3800_NBI_TMX_CSR_OUT_OF_ORDER_COUNT_CLEAR_OUTOFORDERCOUNTCLEAR(_x) (((_x) & 0xffff) << 0)
#define   NFP3800_NBI_TMX_CSR_OUT_OF_ORDER_COUNT_CLEAR_OUTOFORDERCOUNTCLEAR_of(_x) (((_x) >> 0) & 0xffff)
/*----------------------------------------------------------------
  Register: MiniPktCreditConfig - MiniPacket Credit Flow Control Configuration Register
    Bits: 25    PerChannelCreditEnable(rw) - Enables per channel credit limit defined by ChannelCreditLimit.
    Bits: 24    MiniPacketFCMode(rw) - MiniPacket Credit Flow Control Mode bit. Select the free pool crediting mode to use.
    Bits: 21:12 FPCreditLimit(rw) - MiniPacket Transmit Free Pool Credit Counter Limit. Programmable limit for both of the free pool credit counters. The default value of 512 credits should be used for most applications using one free pool. The free pool has a reserve of 40 credits and should always be programmed to a value greater than 40.
    Bits:  9:0  CreditLimit(rw) - MiniPacket Transmit Channel Credit Counter Limit. Programmable limit for all of the channel credit counters
*/
#define NFP3800_NBI_TMX_CSR_MINIPKT_CREDIT_CONFIG            0x00000300
#define   NFP3800_NBI_TMX_CSR_MINIPKT_CREDIT_CONFIG_PERCHANNELCREDITENABLE BIT(25)
#define     NFP3800_NBI_TMX_CSR_MINIPKT_CREDIT_CONFIG_PERCHANNELCREDITENABLE_DISABLE (0 << 25)
#define     NFP3800_NBI_TMX_CSR_MINIPKT_CREDIT_CONFIG_PERCHANNELCREDITENABLE_ENABLE BIT(25)
#define   NFP3800_NBI_TMX_CSR_MINIPKT_CREDIT_CONFIG_MINIPACKETFCMODE BIT(24)
#define     NFP3800_NBI_TMX_CSR_MINIPKT_CREDIT_CONFIG_MINIPACKETFCMODE_MODE_0 (0 << 24)
#define     NFP3800_NBI_TMX_CSR_MINIPKT_CREDIT_CONFIG_MINIPACKETFCMODE_MODE_1 BIT(24)
#define   NFP3800_NBI_TMX_CSR_MINIPKT_CREDIT_CONFIG_FPCREDITLIMIT(_x) (((_x) & 0x3ff) << 12)
#define   NFP3800_NBI_TMX_CSR_MINIPKT_CREDIT_CONFIG_FPCREDITLIMIT_of(_x) (((_x) >> 12) & 0x3ff)
#define   NFP3800_NBI_TMX_CSR_MINIPKT_CREDIT_CONFIG_CREDITLIMIT(_x) (((_x) & 0x3ff) << 0)
#define   NFP3800_NBI_TMX_CSR_MINIPKT_CREDIT_CONFIG_CREDITLIMIT_of(_x) (((_x) >> 0) & 0x3ff)
/*----------------------------------------------------------------
  Register: MiniPktFreePoolCredit[2] - MiniPacket Transmit Free Pool Credit Counter. Read only free pool credit counters
    Bits:  9:0  FPCreditCounter(ro) - MiniPacket Transmit Free Pool Credit Counter. Read only counter of the free pool credits remaining
*/
#define NFP3800_NBI_TMX_CSR_MINIPKT_FREE_POOL_CREDIT(_x)     (0x00000304 + (0x4 * ((_x) & 0x1)))
#define   NFP3800_NBI_TMX_CSR_MINIPKT_FREE_POOL_CREDIT_FPCREDITCOUNT(_x) (((_x) & 0x3ff) << 0)
#define   NFP3800_NBI_TMX_CSR_MINIPKT_FREE_POOL_CREDIT_FPCREDITCOUNT_of(_x) (((_x) >> 0) & 0x3ff)
/*----------------------------------------------------------------
  Register: MiniPktChannelCredit[128] - MiniPacket Transmit Channel Credit Counter. Read only credit counter, one for each channel
    Bits:  9:0  CreditCounter(ro) - MiniPacket Transmit Channel Credit Counter. Read only counter of the channel credits remaining
*/
#define NFP3800_NBI_TMX_CSR_MINIPKT_CHANNEL_CREDIT(_x)       (0x00000400 + (0x4 * ((_x) & 0x7f)))
#define   NFP3800_NBI_TMX_CSR_MINIPKT_CHANNEL_CREDIT_CREDITCOUNT(_x) (((_x) & 0x3ff) << 0)
#define   NFP3800_NBI_TMX_CSR_MINIPKT_CHANNEL_CREDIT_CREDITCOUNT_of(_x) (((_x) >> 0) & 0x3ff)
/*----------------------------------------------------------------
  Register: ReorderActivity - Activity indication flag for each sequencer in the reorder buffer.
    Bits: 15:0  ReorderActivity(ro) - Clear on read activity flag for each sequencer in the reorder buffer. One bit for each active sequencer in the reorder buffer.
*/
#define NFP3800_NBI_TMX_CSR_REORDER_ACTIVITY                 0x00000600
#define   NFP3800_NBI_TMX_CSR_REORDER_ACTIVITY_REORDERACTIVITY(_x) (((_x) & 0xffff) << 0)
#define   NFP3800_NBI_TMX_CSR_REORDER_ACTIVITY_REORDERACTIVITY_of(_x) (((_x) >> 0) & 0xffff)
/*----------------------------------------------------------------
  Register: DropProfile[8] - 8 programmable Drop Profiles corresponding to drop precedence values 0-7
    Bits: 11:9  DropRateRange3(rw) - Drop Rate Queue Depth Range 3. Top 75% to 100% of the descriptor queue. Packets will be dropped n% of the time if the queue depth is within this range
    Bits:  8:6  DropRateRange2(rw) - Drop Rate for Queue Depth Range 2. 50% to 74% full. Packets will be dropped n% of the time if the queue depth is within this range
    Bits:  5:3  DropRateRange1(rw) - Drop Rate for Queue Depth Range 1. 25% to 49% full. Packets will be dropped n% of the time if the queue depth is within this range
    Bits:  2:0  DropRateRange0(rw) - Drop Rate for Queue Depth Range 0. Bottom 0 to 24% of the descriptor queue. Packets will be dropped n% of the time if the queue depth is within this range
*/
#define NFP3800_NBI_TMX_CSR_DROP_PROFILE(_x)                 (0x00000604 + (0x4 * ((_x) & 0x7)))
#define   NFP3800_NBI_TMX_CSR_DROP_PROFILE_DROPRATERANGE3(_x) (((_x) & 0x7) << 9)
#define   NFP3800_NBI_TMX_CSR_DROP_PROFILE_DROPRATERANGE3_of(_x) (((_x) >> 9) & 0x7)
#define     NFP3800_NBI_TMX_CSR_DROP_PROFILE_DROPRATERANGE3_0 (0)
#define     NFP3800_NBI_TMX_CSR_DROP_PROFILE_DROPRATERANGE3_12_5 (1)
#define     NFP3800_NBI_TMX_CSR_DROP_PROFILE_DROPRATERANGE3_25 (2)
#define     NFP3800_NBI_TMX_CSR_DROP_PROFILE_DROPRATERANGE3_37_5 (3)
#define     NFP3800_NBI_TMX_CSR_DROP_PROFILE_DROPRATERANGE3_50 (4)
#define     NFP3800_NBI_TMX_CSR_DROP_PROFILE_DROPRATERANGE3_62_5 (5)
#define     NFP3800_NBI_TMX_CSR_DROP_PROFILE_DROPRATERANGE3_75 (6)
#define     NFP3800_NBI_TMX_CSR_DROP_PROFILE_DROPRATERANGE3_87_5 (7)
#define   NFP3800_NBI_TMX_CSR_DROP_PROFILE_DROPRATERANGE2(_x) (((_x) & 0x7) << 6)
#define   NFP3800_NBI_TMX_CSR_DROP_PROFILE_DROPRATERANGE2_of(_x) (((_x) >> 6) & 0x7)
#define     NFP3800_NBI_TMX_CSR_DROP_PROFILE_DROPRATERANGE2_0 (0)
#define     NFP3800_NBI_TMX_CSR_DROP_PROFILE_DROPRATERANGE2_12_5 (1)
#define     NFP3800_NBI_TMX_CSR_DROP_PROFILE_DROPRATERANGE2_25 (2)
#define     NFP3800_NBI_TMX_CSR_DROP_PROFILE_DROPRATERANGE2_37_5 (3)
#define     NFP3800_NBI_TMX_CSR_DROP_PROFILE_DROPRATERANGE2_50 (4)
#define     NFP3800_NBI_TMX_CSR_DROP_PROFILE_DROPRATERANGE2_62_5 (5)
#define     NFP3800_NBI_TMX_CSR_DROP_PROFILE_DROPRATERANGE2_75 (6)
#define     NFP3800_NBI_TMX_CSR_DROP_PROFILE_DROPRATERANGE2_87_5 (7)
#define   NFP3800_NBI_TMX_CSR_DROP_PROFILE_DROPRATERANGE1(_x) (((_x) & 0x7) << 3)
#define   NFP3800_NBI_TMX_CSR_DROP_PROFILE_DROPRATERANGE1_of(_x) (((_x) >> 3) & 0x7)
#define     NFP3800_NBI_TMX_CSR_DROP_PROFILE_DROPRATERANGE1_0 (0)
#define     NFP3800_NBI_TMX_CSR_DROP_PROFILE_DROPRATERANGE1_12_5 (1)
#define     NFP3800_NBI_TMX_CSR_DROP_PROFILE_DROPRATERANGE1_25 (2)
#define     NFP3800_NBI_TMX_CSR_DROP_PROFILE_DROPRATERANGE1_37_5 (3)
#define     NFP3800_NBI_TMX_CSR_DROP_PROFILE_DROPRATERANGE1_50 (4)
#define     NFP3800_NBI_TMX_CSR_DROP_PROFILE_DROPRATERANGE1_62_5 (5)
#define     NFP3800_NBI_TMX_CSR_DROP_PROFILE_DROPRATERANGE1_75 (6)
#define     NFP3800_NBI_TMX_CSR_DROP_PROFILE_DROPRATERANGE1_87_5 (7)
#define   NFP3800_NBI_TMX_CSR_DROP_PROFILE_DROPRATERANGE0(_x) (((_x) & 0x7) << 0)
#define   NFP3800_NBI_TMX_CSR_DROP_PROFILE_DROPRATERANGE0_of(_x) (((_x) >> 0) & 0x7)
#define     NFP3800_NBI_TMX_CSR_DROP_PROFILE_DROPRATERANGE0_0 (0)
#define     NFP3800_NBI_TMX_CSR_DROP_PROFILE_DROPRATERANGE0_12_5 (1)
#define     NFP3800_NBI_TMX_CSR_DROP_PROFILE_DROPRATERANGE0_25 (2)
#define     NFP3800_NBI_TMX_CSR_DROP_PROFILE_DROPRATERANGE0_37_5 (3)
#define     NFP3800_NBI_TMX_CSR_DROP_PROFILE_DROPRATERANGE0_50 (4)
#define     NFP3800_NBI_TMX_CSR_DROP_PROFILE_DROPRATERANGE0_62_5 (5)
#define     NFP3800_NBI_TMX_CSR_DROP_PROFILE_DROPRATERANGE0_75 (6)
#define     NFP3800_NBI_TMX_CSR_DROP_PROFILE_DROPRATERANGE0_87_5 (7)
/*----------------------------------------------------------------
  Register: DescriptorMemoryRedDrop - Packet Descriptor RED drops.
    Bits: 11:9  DropRateRange3(rw) - Drop Rate for Descriptor Memory Depth Range 3. Top 75% to 100% of the last 512 space of Descriptor Memory. Packets will be dropped n% of the time if the memory depth is within this range
    Bits:  8:6  DropRateRange2(rw) - Drop Rate for Descriptor Memory Depth Range 2. 25% to 49% of the last 512 space of Descriptor Memory. Packets will be dropped n% of the time if the memory depth is within this range
    Bits:  5:3  DropRateRange1(rw) - Drop Rate for Descriptor Memory Depth Range 1. 25% to 49% of the last 512 space of Descriptor Memory. Packets will be dropped n% of the time if the memory depth is within this range
    Bits:  2:0  DropRateRange0(rw) - Drop Rate for Descriptor Memory Depth Range 0. Bottom 0 to 24% of the last 512 space of Descriptor Memory. Packets will be dropped n% of the time if the memory depth is within this range
*/
#define NFP3800_NBI_TMX_CSR_DESCRIPTOR_MEMORY_RED_DROP       0x00000628
#define   NFP3800_NBI_TMX_CSR_DESCRIPTOR_MEMORY_RED_DROP_DROPRATERANGE3(_x) (((_x) & 0x7) << 9)
#define   NFP3800_NBI_TMX_CSR_DESCRIPTOR_MEMORY_RED_DROP_DROPRATERANGE3_of(_x) (((_x) >> 9) & 0x7)
#define     NFP3800_NBI_TMX_CSR_DESCRIPTOR_MEMORY_RED_DROP_DROPRATERANGE3_0 (0)
#define     NFP3800_NBI_TMX_CSR_DESCRIPTOR_MEMORY_RED_DROP_DROPRATERANGE3_12_5 (1)
#define     NFP3800_NBI_TMX_CSR_DESCRIPTOR_MEMORY_RED_DROP_DROPRATERANGE3_25 (2)
#define     NFP3800_NBI_TMX_CSR_DESCRIPTOR_MEMORY_RED_DROP_DROPRATERANGE3_37_5 (3)
#define     NFP3800_NBI_TMX_CSR_DESCRIPTOR_MEMORY_RED_DROP_DROPRATERANGE3_50 (4)
#define     NFP3800_NBI_TMX_CSR_DESCRIPTOR_MEMORY_RED_DROP_DROPRATERANGE3_62_5 (5)
#define     NFP3800_NBI_TMX_CSR_DESCRIPTOR_MEMORY_RED_DROP_DROPRATERANGE3_75 (6)
#define     NFP3800_NBI_TMX_CSR_DESCRIPTOR_MEMORY_RED_DROP_DROPRATERANGE3_87_5 (7)
#define   NFP3800_NBI_TMX_CSR_DESCRIPTOR_MEMORY_RED_DROP_DROPRATERANGE2(_x) (((_x) & 0x7) << 6)
#define   NFP3800_NBI_TMX_CSR_DESCRIPTOR_MEMORY_RED_DROP_DROPRATERANGE2_of(_x) (((_x) >> 6) & 0x7)
#define     NFP3800_NBI_TMX_CSR_DESCRIPTOR_MEMORY_RED_DROP_DROPRATERANGE2_0 (0)
#define     NFP3800_NBI_TMX_CSR_DESCRIPTOR_MEMORY_RED_DROP_DROPRATERANGE2_12_5 (1)
#define     NFP3800_NBI_TMX_CSR_DESCRIPTOR_MEMORY_RED_DROP_DROPRATERANGE2_25 (2)
#define     NFP3800_NBI_TMX_CSR_DESCRIPTOR_MEMORY_RED_DROP_DROPRATERANGE2_37_5 (3)
#define     NFP3800_NBI_TMX_CSR_DESCRIPTOR_MEMORY_RED_DROP_DROPRATERANGE2_50 (4)
#define     NFP3800_NBI_TMX_CSR_DESCRIPTOR_MEMORY_RED_DROP_DROPRATERANGE2_62_5 (5)
#define     NFP3800_NBI_TMX_CSR_DESCRIPTOR_MEMORY_RED_DROP_DROPRATERANGE2_75 (6)
#define     NFP3800_NBI_TMX_CSR_DESCRIPTOR_MEMORY_RED_DROP_DROPRATERANGE2_87_5 (7)
#define   NFP3800_NBI_TMX_CSR_DESCRIPTOR_MEMORY_RED_DROP_DROPRATERANGE1(_x) (((_x) & 0x7) << 3)
#define   NFP3800_NBI_TMX_CSR_DESCRIPTOR_MEMORY_RED_DROP_DROPRATERANGE1_of(_x) (((_x) >> 3) & 0x7)
#define     NFP3800_NBI_TMX_CSR_DESCRIPTOR_MEMORY_RED_DROP_DROPRATERANGE1_0 (0)
#define     NFP3800_NBI_TMX_CSR_DESCRIPTOR_MEMORY_RED_DROP_DROPRATERANGE1_12_5 (1)
#define     NFP3800_NBI_TMX_CSR_DESCRIPTOR_MEMORY_RED_DROP_DROPRATERANGE1_25 (2)
#define     NFP3800_NBI_TMX_CSR_DESCRIPTOR_MEMORY_RED_DROP_DROPRATERANGE1_37_5 (3)
#define     NFP3800_NBI_TMX_CSR_DESCRIPTOR_MEMORY_RED_DROP_DROPRATERANGE1_50 (4)
#define     NFP3800_NBI_TMX_CSR_DESCRIPTOR_MEMORY_RED_DROP_DROPRATERANGE1_62_5 (5)
#define     NFP3800_NBI_TMX_CSR_DESCRIPTOR_MEMORY_RED_DROP_DROPRATERANGE1_75 (6)
#define     NFP3800_NBI_TMX_CSR_DESCRIPTOR_MEMORY_RED_DROP_DROPRATERANGE1_87_5 (7)
#define   NFP3800_NBI_TMX_CSR_DESCRIPTOR_MEMORY_RED_DROP_DROPRATERANGE0(_x) (((_x) & 0x7) << 0)
#define   NFP3800_NBI_TMX_CSR_DESCRIPTOR_MEMORY_RED_DROP_DROPRATERANGE0_of(_x) (((_x) >> 0) & 0x7)
#define     NFP3800_NBI_TMX_CSR_DESCRIPTOR_MEMORY_RED_DROP_DROPRATERANGE0_0 (0)
#define     NFP3800_NBI_TMX_CSR_DESCRIPTOR_MEMORY_RED_DROP_DROPRATERANGE0_12_5 (1)
#define     NFP3800_NBI_TMX_CSR_DESCRIPTOR_MEMORY_RED_DROP_DROPRATERANGE0_25 (2)
#define     NFP3800_NBI_TMX_CSR_DESCRIPTOR_MEMORY_RED_DROP_DROPRATERANGE0_37_5 (3)
#define     NFP3800_NBI_TMX_CSR_DESCRIPTOR_MEMORY_RED_DROP_DROPRATERANGE0_50 (4)
#define     NFP3800_NBI_TMX_CSR_DESCRIPTOR_MEMORY_RED_DROP_DROPRATERANGE0_62_5 (5)
#define     NFP3800_NBI_TMX_CSR_DESCRIPTOR_MEMORY_RED_DROP_DROPRATERANGE0_75 (6)
#define     NFP3800_NBI_TMX_CSR_DESCRIPTOR_MEMORY_RED_DROP_DROPRATERANGE0_87_5 (7)
/*----------------------------------------------------------------
  Register: MiniPktChannelCreditLimit[128] - MiniPacket Channel Credit limit Configuration Register
    Bits:  9:0  ChannelCreditLimit(rw) - MiniPacket Transmit Channel Credit Limit. Programmable limit per channel
*/
#define NFP3800_NBI_TMX_CSR_MINIPKT_CHANNEL_CREDIT_LIMIT(_x) (0x0000062c + (0x4 * ((_x) & 0x7f)))
#define   NFP3800_NBI_TMX_CSR_MINIPKT_CHANNEL_CREDIT_LIMIT_CHANNELCREDITLIMIT(_x) (((_x) & 0x3ff) << 0)
#define   NFP3800_NBI_TMX_CSR_MINIPKT_CHANNEL_CREDIT_LIMIT_CHANNELCREDITLIMIT_of(_x) (((_x) >> 0) & 0x3ff)
/*----------------------------------------------------------------
  Register: TmReorderBufferFlush - Reorder buffer flush command per sequencer.
    Bits: 15:0  SequencerFlush(rw) - This 32-bit register defines a bit per sequencer that needs to be flushed. Multiple sequencers can be flushed by setting the appropriate bits. Bit 0 represents sequencer number 0 and bit 15 represents sequencer number 15. The software can set the appropriate bits for the sequencers that need to be flushed. The hardware will clear those bits once those sequencers are flushed. It is software's responsibility to set appropriate bits in this register based on the NumSequencers register.
*/
#define NFP3800_NBI_TMX_CSR_TMREORDERBUFFERFLUSH             0x0000082c
#define   NFP3800_NBI_TMX_CSR_TMREORDERBUFFERFLUSH_SEQUENCERFLUSH(_x) (((_x) & 0xffff) << 0)
#define   NFP3800_NBI_TMX_CSR_TMREORDERBUFFERFLUSH_SEQUENCERFLUSH_of(_x) (((_x) >> 0) & 0xffff)
/*----------------------------------------------------------------
  Register: TmReorderTimeOutRange0 - Timeout base range for sequencers 0 to 7.
    Bits: 31:30 SeqRange15(rw) - Defines the timeout range select for Sequencer 15
    Bits: 29:28 SeqRange14(rw) - Defines the timeout range select for Sequencer 14
    Bits: 27:26 SeqRange13(rw) - Defines the timeout range select for Sequencer 13
    Bits: 25:24 SeqRange12(rw) - Defines the timeout range select for Sequencer 12
    Bits: 23:22 SeqRange11(rw) - Defines the timeout range select for Sequencer 11
    Bits: 21:20 SeqRange10(rw) - Defines the timeout range select for Sequencer 10
    Bits: 19:18 SeqRange9(rw) - Defines the timeout range select for Sequencer 9
    Bits: 17:16 SeqRange8(rw) - Defines the timeout range select for Sequencer 8
    Bits: 15:14 SeqRange7(rw) - Defines the timeout range select for Sequencer 7
    Bits: 13:12 SeqRange6(rw) - Defines the timeout range select for Sequencer 6
    Bits: 11:10 SeqRange5(rw) - Defines the timeout range select for Sequencer 5
    Bits:  9:8  SeqRange4(rw) - Defines the timeout range select for Sequencer 4
    Bits:  7:6  SeqRange3(rw) - Defines the timeout range select for Sequencer 3
    Bits:  5:4  SeqRange2(rw) - Defines the timeout range select for Sequencer 2
    Bits:  3:2  SeqRange1(rw) - Defines the timeout range select for Sequencer 1
    Bits:  1:0  SeqRange0(rw) - Defines the timeout range select for Sequencer 0
*/
#define NFP3800_NBI_TMX_CSR_TMREORDERTIMEOUTRANGE0           0x00000830
#define   NFP3800_NBI_TMX_CSR_TMREORDERTIMEOUTRANGE0_SEQRANGE15(_x) (((_x) & 0x3) << 30)
#define   NFP3800_NBI_TMX_CSR_TMREORDERTIMEOUTRANGE0_SEQRANGE15_of(_x) (((_x) >> 30) & 0x3)
#define     NFP3800_NBI_TMX_CSR_TMREORDERTIMEOUTRANGE0_SEQRANGE15_DISABLE (0)
#define     NFP3800_NBI_TMX_CSR_TMREORDERTIMEOUTRANGE0_SEQRANGE15_1K_CYCLES (1)
#define     NFP3800_NBI_TMX_CSR_TMREORDERTIMEOUTRANGE0_SEQRANGE15_10K_CYCLES (2)
#define     NFP3800_NBI_TMX_CSR_TMREORDERTIMEOUTRANGE0_SEQRANGE15_100K_CYCLES (3)
#define   NFP3800_NBI_TMX_CSR_TMREORDERTIMEOUTRANGE0_SEQRANGE14(_x) (((_x) & 0x3) << 28)
#define   NFP3800_NBI_TMX_CSR_TMREORDERTIMEOUTRANGE0_SEQRANGE14_of(_x) (((_x) >> 28) & 0x3)
#define     NFP3800_NBI_TMX_CSR_TMREORDERTIMEOUTRANGE0_SEQRANGE14_DISABLE (0)
#define     NFP3800_NBI_TMX_CSR_TMREORDERTIMEOUTRANGE0_SEQRANGE14_1K_CYCLES (1)
#define     NFP3800_NBI_TMX_CSR_TMREORDERTIMEOUTRANGE0_SEQRANGE14_10K_CYCLES (2)
#define     NFP3800_NBI_TMX_CSR_TMREORDERTIMEOUTRANGE0_SEQRANGE14_100K_CYCLES (3)
#define   NFP3800_NBI_TMX_CSR_TMREORDERTIMEOUTRANGE0_SEQRANGE13(_x) (((_x) & 0x3) << 26)
#define   NFP3800_NBI_TMX_CSR_TMREORDERTIMEOUTRANGE0_SEQRANGE13_of(_x) (((_x) >> 26) & 0x3)
#define     NFP3800_NBI_TMX_CSR_TMREORDERTIMEOUTRANGE0_SEQRANGE13_DISABLE (0)
#define     NFP3800_NBI_TMX_CSR_TMREORDERTIMEOUTRANGE0_SEQRANGE13_1K_CYCLES (1)
#define     NFP3800_NBI_TMX_CSR_TMREORDERTIMEOUTRANGE0_SEQRANGE13_10K_CYCLES (2)
#define     NFP3800_NBI_TMX_CSR_TMREORDERTIMEOUTRANGE0_SEQRANGE13_100K_CYCLES (3)
#define   NFP3800_NBI_TMX_CSR_TMREORDERTIMEOUTRANGE0_SEQRANGE12(_x) (((_x) & 0x3) << 24)
#define   NFP3800_NBI_TMX_CSR_TMREORDERTIMEOUTRANGE0_SEQRANGE12_of(_x) (((_x) >> 24) & 0x3)
#define     NFP3800_NBI_TMX_CSR_TMREORDERTIMEOUTRANGE0_SEQRANGE12_DISABLE (0)
#define     NFP3800_NBI_TMX_CSR_TMREORDERTIMEOUTRANGE0_SEQRANGE12_1K_CYCLES (1)
#define     NFP3800_NBI_TMX_CSR_TMREORDERTIMEOUTRANGE0_SEQRANGE12_10K_CYCLES (2)
#define     NFP3800_NBI_TMX_CSR_TMREORDERTIMEOUTRANGE0_SEQRANGE12_100K_CYCLES (3)
#define   NFP3800_NBI_TMX_CSR_TMREORDERTIMEOUTRANGE0_SEQRANGE11(_x) (((_x) & 0x3) << 22)
#define   NFP3800_NBI_TMX_CSR_TMREORDERTIMEOUTRANGE0_SEQRANGE11_of(_x) (((_x) >> 22) & 0x3)
#define     NFP3800_NBI_TMX_CSR_TMREORDERTIMEOUTRANGE0_SEQRANGE11_DISABLE (0)
#define     NFP3800_NBI_TMX_CSR_TMREORDERTIMEOUTRANGE0_SEQRANGE11_1K_CYCLES (1)
#define     NFP3800_NBI_TMX_CSR_TMREORDERTIMEOUTRANGE0_SEQRANGE11_10K_CYCLES (2)
#define     NFP3800_NBI_TMX_CSR_TMREORDERTIMEOUTRANGE0_SEQRANGE11_100K_CYCLES (3)
#define   NFP3800_NBI_TMX_CSR_TMREORDERTIMEOUTRANGE0_SEQRANGE10(_x) (((_x) & 0x3) << 20)
#define   NFP3800_NBI_TMX_CSR_TMREORDERTIMEOUTRANGE0_SEQRANGE10_of(_x) (((_x) >> 20) & 0x3)
#define     NFP3800_NBI_TMX_CSR_TMREORDERTIMEOUTRANGE0_SEQRANGE10_DISABLE (0)
#define     NFP3800_NBI_TMX_CSR_TMREORDERTIMEOUTRANGE0_SEQRANGE10_1K_CYCLES (1)
#define     NFP3800_NBI_TMX_CSR_TMREORDERTIMEOUTRANGE0_SEQRANGE10_10K_CYCLES (2)
#define     NFP3800_NBI_TMX_CSR_TMREORDERTIMEOUTRANGE0_SEQRANGE10_100K_CYCLES (3)
#define   NFP3800_NBI_TMX_CSR_TMREORDERTIMEOUTRANGE0_SEQRANGE9(_x) (((_x) & 0x3) << 18)
#define   NFP3800_NBI_TMX_CSR_TMREORDERTIMEOUTRANGE0_SEQRANGE9_of(_x) (((_x) >> 18) & 0x3)
#define     NFP3800_NBI_TMX_CSR_TMREORDERTIMEOUTRANGE0_SEQRANGE9_DISABLE (0)
#define     NFP3800_NBI_TMX_CSR_TMREORDERTIMEOUTRANGE0_SEQRANGE9_1K_CYCLES (1)
#define     NFP3800_NBI_TMX_CSR_TMREORDERTIMEOUTRANGE0_SEQRANGE9_10K_CYCLES (2)
#define     NFP3800_NBI_TMX_CSR_TMREORDERTIMEOUTRANGE0_SEQRANGE9_100K_CYCLES (3)
#define   NFP3800_NBI_TMX_CSR_TMREORDERTIMEOUTRANGE0_SEQRANGE8(_x) (((_x) & 0x3) << 16)
#define   NFP3800_NBI_TMX_CSR_TMREORDERTIMEOUTRANGE0_SEQRANGE8_of(_x) (((_x) >> 16) & 0x3)
#define     NFP3800_NBI_TMX_CSR_TMREORDERTIMEOUTRANGE0_SEQRANGE8_DISABLE (0)
#define     NFP3800_NBI_TMX_CSR_TMREORDERTIMEOUTRANGE0_SEQRANGE8_1K_CYCLES (1)
#define     NFP3800_NBI_TMX_CSR_TMREORDERTIMEOUTRANGE0_SEQRANGE8_10K_CYCLES (2)
#define     NFP3800_NBI_TMX_CSR_TMREORDERTIMEOUTRANGE0_SEQRANGE8_100K_CYCLES (3)
#define   NFP3800_NBI_TMX_CSR_TMREORDERTIMEOUTRANGE0_SEQRANGE7(_x) (((_x) & 0x3) << 14)
#define   NFP3800_NBI_TMX_CSR_TMREORDERTIMEOUTRANGE0_SEQRANGE7_of(_x) (((_x) >> 14) & 0x3)
#define     NFP3800_NBI_TMX_CSR_TMREORDERTIMEOUTRANGE0_SEQRANGE7_DISABLE (0)
#define     NFP3800_NBI_TMX_CSR_TMREORDERTIMEOUTRANGE0_SEQRANGE7_1K_CYCLES (1)
#define     NFP3800_NBI_TMX_CSR_TMREORDERTIMEOUTRANGE0_SEQRANGE7_10K_CYCLES (2)
#define     NFP3800_NBI_TMX_CSR_TMREORDERTIMEOUTRANGE0_SEQRANGE7_100K_CYCLES (3)
#define   NFP3800_NBI_TMX_CSR_TMREORDERTIMEOUTRANGE0_SEQRANGE6(_x) (((_x) & 0x3) << 12)
#define   NFP3800_NBI_TMX_CSR_TMREORDERTIMEOUTRANGE0_SEQRANGE6_of(_x) (((_x) >> 12) & 0x3)
#define     NFP3800_NBI_TMX_CSR_TMREORDERTIMEOUTRANGE0_SEQRANGE6_DISABLE (0)
#define     NFP3800_NBI_TMX_CSR_TMREORDERTIMEOUTRANGE0_SEQRANGE6_1K_CYCLES (1)
#define     NFP3800_NBI_TMX_CSR_TMREORDERTIMEOUTRANGE0_SEQRANGE6_10K_CYCLES (2)
#define     NFP3800_NBI_TMX_CSR_TMREORDERTIMEOUTRANGE0_SEQRANGE6_100K_CYCLES (3)
#define   NFP3800_NBI_TMX_CSR_TMREORDERTIMEOUTRANGE0_SEQRANGE5(_x) (((_x) & 0x3) << 10)
#define   NFP3800_NBI_TMX_CSR_TMREORDERTIMEOUTRANGE0_SEQRANGE5_of(_x) (((_x) >> 10) & 0x3)
#define     NFP3800_NBI_TMX_CSR_TMREORDERTIMEOUTRANGE0_SEQRANGE5_DISABLE (0)
#define     NFP3800_NBI_TMX_CSR_TMREORDERTIMEOUTRANGE0_SEQRANGE5_1K_CYCLES (1)
#define     NFP3800_NBI_TMX_CSR_TMREORDERTIMEOUTRANGE0_SEQRANGE5_10K_CYCLES (2)
#define     NFP3800_NBI_TMX_CSR_TMREORDERTIMEOUTRANGE0_SEQRANGE5_100K_CYCLES (3)
#define   NFP3800_NBI_TMX_CSR_TMREORDERTIMEOUTRANGE0_SEQRANGE4(_x) (((_x) & 0x3) << 8)
#define   NFP3800_NBI_TMX_CSR_TMREORDERTIMEOUTRANGE0_SEQRANGE4_of(_x) (((_x) >> 8) & 0x3)
#define     NFP3800_NBI_TMX_CSR_TMREORDERTIMEOUTRANGE0_SEQRANGE4_DISABLE (0)
#define     NFP3800_NBI_TMX_CSR_TMREORDERTIMEOUTRANGE0_SEQRANGE4_1K_CYCLES (1)
#define     NFP3800_NBI_TMX_CSR_TMREORDERTIMEOUTRANGE0_SEQRANGE4_10K_CYCLES (2)
#define     NFP3800_NBI_TMX_CSR_TMREORDERTIMEOUTRANGE0_SEQRANGE4_100K_CYCLES (3)
#define   NFP3800_NBI_TMX_CSR_TMREORDERTIMEOUTRANGE0_SEQRANGE3(_x) (((_x) & 0x3) << 6)
#define   NFP3800_NBI_TMX_CSR_TMREORDERTIMEOUTRANGE0_SEQRANGE3_of(_x) (((_x) >> 6) & 0x3)
#define     NFP3800_NBI_TMX_CSR_TMREORDERTIMEOUTRANGE0_SEQRANGE3_DISABLE (0)
#define     NFP3800_NBI_TMX_CSR_TMREORDERTIMEOUTRANGE0_SEQRANGE3_1K_CYCLES (1)
#define     NFP3800_NBI_TMX_CSR_TMREORDERTIMEOUTRANGE0_SEQRANGE3_10K_CYCLES (2)
#define     NFP3800_NBI_TMX_CSR_TMREORDERTIMEOUTRANGE0_SEQRANGE3_100K_CYCLES (3)
#define   NFP3800_NBI_TMX_CSR_TMREORDERTIMEOUTRANGE0_SEQRANGE2(_x) (((_x) & 0x3) << 4)
#define   NFP3800_NBI_TMX_CSR_TMREORDERTIMEOUTRANGE0_SEQRANGE2_of(_x) (((_x) >> 4) & 0x3)
#define     NFP3800_NBI_TMX_CSR_TMREORDERTIMEOUTRANGE0_SEQRANGE2_DISABLE (0)
#define     NFP3800_NBI_TMX_CSR_TMREORDERTIMEOUTRANGE0_SEQRANGE2_1K_CYCLES (1)
#define     NFP3800_NBI_TMX_CSR_TMREORDERTIMEOUTRANGE0_SEQRANGE2_10K_CYCLES (2)
#define     NFP3800_NBI_TMX_CSR_TMREORDERTIMEOUTRANGE0_SEQRANGE2_100K_CYCLES (3)
#define   NFP3800_NBI_TMX_CSR_TMREORDERTIMEOUTRANGE0_SEQRANGE1(_x) (((_x) & 0x3) << 2)
#define   NFP3800_NBI_TMX_CSR_TMREORDERTIMEOUTRANGE0_SEQRANGE1_of(_x) (((_x) >> 2) & 0x3)
#define     NFP3800_NBI_TMX_CSR_TMREORDERTIMEOUTRANGE0_SEQRANGE1_DISABLE (0)
#define     NFP3800_NBI_TMX_CSR_TMREORDERTIMEOUTRANGE0_SEQRANGE1_1K_CYCLES (1)
#define     NFP3800_NBI_TMX_CSR_TMREORDERTIMEOUTRANGE0_SEQRANGE1_10K_CYCLES (2)
#define     NFP3800_NBI_TMX_CSR_TMREORDERTIMEOUTRANGE0_SEQRANGE1_100K_CYCLES (3)
#define   NFP3800_NBI_TMX_CSR_TMREORDERTIMEOUTRANGE0_SEQRANGE0(_x) (((_x) & 0x3) << 0)
#define   NFP3800_NBI_TMX_CSR_TMREORDERTIMEOUTRANGE0_SEQRANGE0_of(_x) (((_x) >> 0) & 0x3)
#define     NFP3800_NBI_TMX_CSR_TMREORDERTIMEOUTRANGE0_SEQRANGE0_DISABLE (0)
#define     NFP3800_NBI_TMX_CSR_TMREORDERTIMEOUTRANGE0_SEQRANGE0_1K_CYCLES (1)
#define     NFP3800_NBI_TMX_CSR_TMREORDERTIMEOUTRANGE0_SEQRANGE0_10K_CYCLES (2)
#define     NFP3800_NBI_TMX_CSR_TMREORDERTIMEOUTRANGE0_SEQRANGE0_100K_CYCLES (3)
/*----------------------------------------------------------------
  Register: TmReorderTimeOutValue0 - Timeout value for sequencers 0 to 3.
    Bits: 31:24 SeqValue3(rw) - Defines the timeout value for Sequencer 3
    Bits: 23:16 SeqValue2(rw) - Defines the timeout value for Sequencer 2
    Bits: 15:8  SeqValue1(rw) - Defines the timeout value for Sequencer 1
    Bits:  7:0  SeqValue0(rw) - Defines the timeout value for Sequencer 0
*/
#define NFP3800_NBI_TMX_CSR_TMREORDERTIMEOUTVALUE0           0x00000840
#define   NFP3800_NBI_TMX_CSR_TMREORDERTIMEOUTVALUE0_SEQVALUE3(_x) (((_x) & 0xff) << 24)
#define   NFP3800_NBI_TMX_CSR_TMREORDERTIMEOUTVALUE0_SEQVALUE3_of(_x) (((_x) >> 24) & 0xff)
#define   NFP3800_NBI_TMX_CSR_TMREORDERTIMEOUTVALUE0_SEQVALUE2(_x) (((_x) & 0xff) << 16)
#define   NFP3800_NBI_TMX_CSR_TMREORDERTIMEOUTVALUE0_SEQVALUE2_of(_x) (((_x) >> 16) & 0xff)
#define   NFP3800_NBI_TMX_CSR_TMREORDERTIMEOUTVALUE0_SEQVALUE1(_x) (((_x) & 0xff) << 8)
#define   NFP3800_NBI_TMX_CSR_TMREORDERTIMEOUTVALUE0_SEQVALUE1_of(_x) (((_x) >> 8) & 0xff)
#define   NFP3800_NBI_TMX_CSR_TMREORDERTIMEOUTVALUE0_SEQVALUE0(_x) (((_x) & 0xff) << 0)
#define   NFP3800_NBI_TMX_CSR_TMREORDERTIMEOUTVALUE0_SEQVALUE0_of(_x) (((_x) >> 0) & 0xff)
/*----------------------------------------------------------------
  Register: TmReorderTimeOutValue1 - Timeout value for sequencers 4 to 7.
    Bits: 31:24 SeqValue7(rw) - Defines the timeout value for Sequencer 7
    Bits: 23:16 SeqValue6(rw) - Defines the timeout value for Sequencer 6
    Bits: 15:8  SeqValue5(rw) - Defines the timeout value for Sequencer 5
    Bits:  7:0  SeqValue4(rw) - Defines the timeout value for Sequencer 4
*/
#define NFP3800_NBI_TMX_CSR_TMREORDERTIMEOUTVALUE1           0x00000844
#define   NFP3800_NBI_TMX_CSR_TMREORDERTIMEOUTVALUE1_SEQVALUE7(_x) (((_x) & 0xff) << 24)
#define   NFP3800_NBI_TMX_CSR_TMREORDERTIMEOUTVALUE1_SEQVALUE7_of(_x) (((_x) >> 24) & 0xff)
#define   NFP3800_NBI_TMX_CSR_TMREORDERTIMEOUTVALUE1_SEQVALUE6(_x) (((_x) & 0xff) << 16)
#define   NFP3800_NBI_TMX_CSR_TMREORDERTIMEOUTVALUE1_SEQVALUE6_of(_x) (((_x) >> 16) & 0xff)
#define   NFP3800_NBI_TMX_CSR_TMREORDERTIMEOUTVALUE1_SEQVALUE5(_x) (((_x) & 0xff) << 8)
#define   NFP3800_NBI_TMX_CSR_TMREORDERTIMEOUTVALUE1_SEQVALUE5_of(_x) (((_x) >> 8) & 0xff)
#define   NFP3800_NBI_TMX_CSR_TMREORDERTIMEOUTVALUE1_SEQVALUE4(_x) (((_x) & 0xff) << 0)
#define   NFP3800_NBI_TMX_CSR_TMREORDERTIMEOUTVALUE1_SEQVALUE4_of(_x) (((_x) >> 0) & 0xff)
/*----------------------------------------------------------------
  Register: TmReorderTimeOutValue2 - Timeout value for sequencers 8 to 11.
    Bits: 31:24 SeqValue11(rw) - Defines the timeout value for Sequencer 11
    Bits: 23:16 SeqValue10(rw) - Defines the timeout value for Sequencer 10
    Bits: 15:8  SeqValue9(rw) - Defines the timeout value for Sequencer 9
    Bits:  7:0  SeqValue8(rw) - Defines the timeout value for Sequencer 8
*/
#define NFP3800_NBI_TMX_CSR_TMREORDERTIMEOUTVALUE2           0x00000848
#define   NFP3800_NBI_TMX_CSR_TMREORDERTIMEOUTVALUE2_SEQVALUE11(_x) (((_x) & 0xff) << 24)
#define   NFP3800_NBI_TMX_CSR_TMREORDERTIMEOUTVALUE2_SEQVALUE11_of(_x) (((_x) >> 24) & 0xff)
#define   NFP3800_NBI_TMX_CSR_TMREORDERTIMEOUTVALUE2_SEQVALUE10(_x) (((_x) & 0xff) << 16)
#define   NFP3800_NBI_TMX_CSR_TMREORDERTIMEOUTVALUE2_SEQVALUE10_of(_x) (((_x) >> 16) & 0xff)
#define   NFP3800_NBI_TMX_CSR_TMREORDERTIMEOUTVALUE2_SEQVALUE9(_x) (((_x) & 0xff) << 8)
#define   NFP3800_NBI_TMX_CSR_TMREORDERTIMEOUTVALUE2_SEQVALUE9_of(_x) (((_x) >> 8) & 0xff)
#define   NFP3800_NBI_TMX_CSR_TMREORDERTIMEOUTVALUE2_SEQVALUE8(_x) (((_x) & 0xff) << 0)
#define   NFP3800_NBI_TMX_CSR_TMREORDERTIMEOUTVALUE2_SEQVALUE8_of(_x) (((_x) >> 0) & 0xff)
/*----------------------------------------------------------------
  Register: TmReorderTimeOutValue3 - Timeout value for sequencers 12 to 15.
    Bits: 31:24 SeqValue15(rw) - Defines the timeout value for Sequencer 15
    Bits: 23:16 SeqValue14(rw) - Defines the timeout value for Sequencer 14
    Bits: 15:8  SeqValue13(rw) - Defines the timeout value for Sequencer 13
    Bits:  7:0  SeqValue12(rw) - Defines the timeout value for Sequencer 12
*/
#define NFP3800_NBI_TMX_CSR_TMREORDERTIMEOUTVALUE3           0x0000084c
#define   NFP3800_NBI_TMX_CSR_TMREORDERTIMEOUTVALUE3_SEQVALUE15(_x) (((_x) & 0xff) << 24)
#define   NFP3800_NBI_TMX_CSR_TMREORDERTIMEOUTVALUE3_SEQVALUE15_of(_x) (((_x) >> 24) & 0xff)
#define   NFP3800_NBI_TMX_CSR_TMREORDERTIMEOUTVALUE3_SEQVALUE14(_x) (((_x) & 0xff) << 16)
#define   NFP3800_NBI_TMX_CSR_TMREORDERTIMEOUTVALUE3_SEQVALUE14_of(_x) (((_x) >> 16) & 0xff)
#define   NFP3800_NBI_TMX_CSR_TMREORDERTIMEOUTVALUE3_SEQVALUE13(_x) (((_x) & 0xff) << 8)
#define   NFP3800_NBI_TMX_CSR_TMREORDERTIMEOUTVALUE3_SEQVALUE13_of(_x) (((_x) >> 8) & 0xff)
#define   NFP3800_NBI_TMX_CSR_TMREORDERTIMEOUTVALUE3_SEQVALUE12(_x) (((_x) & 0xff) << 0)
#define   NFP3800_NBI_TMX_CSR_TMREORDERTIMEOUTVALUE3_SEQVALUE12_of(_x) (((_x) >> 0) & 0xff)
/*----------------------------------------------------------------
  Register: TmReorderFlushFlag - Sticky Flush Flag, Clear on read.
    Bits: 15:0  TmReorderFlushFlag(rc) - This 32-bit register is a sticky flush flag per sequencer. Once Flush occurs due to timeout, software flush or flush due to incoming sequence for a given sequencer - respective bit would be set and the register only be cleared on read.
*/
#define NFP3800_NBI_TMX_CSR_TMREORDERFLUSHFLAG               0x00000900
#define   NFP3800_NBI_TMX_CSR_TMREORDERFLUSHFLAG_TMREORDERFLUSHFLAG(_x) (((_x) & 0xffff) << 0)
#define   NFP3800_NBI_TMX_CSR_TMREORDERFLUSHFLAG_TMREORDERFLUSHFLAG_of(_x) (((_x) >> 0) & 0xffff)
/*----------------------------------------------------------------
  Register: TMReceivedPacketCounter - This 32-bit register is a saturating counter, counting the incoming PktReady to Traffic Manager. It's a clear on read register.
    Bits: 31:0  TMReceivedPacketCounter(rc) - This 32-bit register is a saturating counter, counting the incoming PktReady to Traffic Manager. It's a clear on read register.
*/
#define NFP3800_NBI_TMX_CSR_TMRECEIVEDPACKETCOUNTER          0x00000904
#define   NFP3800_NBI_TMX_CSR_TMRECEIVEDPACKETCOUNTER_TMRECEIVEDPACKETCOUNTER(_x) (((_x) & 0xffffffff) << 0)
#define   NFP3800_NBI_TMX_CSR_TMRECEIVEDPACKETCOUNTER_TMRECEIVEDPACKETCOUNTER_of(_x) (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: TMTransmittedPacketCounter - This 32-bit register is a saturating counter, counting the descriptors going out to DMA block from the Descriptor Queues. It's a clear on read register.
    Bits: 31:0  TMTransmittedPacketCounter(rc) - This 32-bit register is a saturating counter, counting the descriptors going out to DMA block from the Descriptor Queues. It's a clear on read register.
*/
#define NFP3800_NBI_TMX_CSR_TMTRANSMITTEDPACKETCOUNTER       0x00000908
#define   NFP3800_NBI_TMX_CSR_TMTRANSMITTEDPACKETCOUNTER_TMTRANSMITTEDPACKETCOUNTER(_x) (((_x) & 0xffffffff) << 0)
#define   NFP3800_NBI_TMX_CSR_TMTRANSMITTEDPACKETCOUNTER_TMTRANSMITTEDPACKETCOUNTER_of(_x) (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: TMBlqNullMuPtr - Programmable Null Memory Buffer Pointer.
    Bits: 28:0  TMBlqNullMuPtr(rw) - Programmable Null Memory Buffer Pointer.
*/
#define NFP3800_NBI_TMX_CSR_TMBLQNULLMUPTR                   0x0000090c
#define   NFP3800_NBI_TMX_CSR_TMBLQNULLMUPTR_TMBLQNULLMUPTR(_x) (((_x) & 0x1fffffff) << 0)
#define   NFP3800_NBI_TMX_CSR_TMBLQNULLMUPTR_TMBLQNULLMUPTR_of(_x) (((_x) >> 0) & 0x1fffffff)
/*----------------------------------------------------------------
  Register: TMReorderSeqrEmpty - 32-bits indicating Empty Reorder Sequencers.
    Bits: 15:0  TMReorderSeqrEmpty(ro) - 16-bits indicating Empty Reorder Sequencers. On Reset all the Sequencers are empty
*/
#define NFP3800_NBI_TMX_CSR_TMREORDERSEQREMPTY               0x00000910
#define   NFP3800_NBI_TMX_CSR_TMREORDERSEQREMPTY_TMREORDERSEQREMPTY(_x) (((_x) & 0xffff) << 0)
#define   NFP3800_NBI_TMX_CSR_TMREORDERSEQREMPTY_TMREORDERSEQREMPTY_of(_x) (((_x) >> 0) & 0xffff)
/*----------------------------------------------------------------
  Register: TMReceivedDropPacketCounter - This 32-bit register is a saturating counter, counting the incoming PktReady to Traffic Manager which are with Token of 0. It's a clear on read register.
    Bits: 31:0  TMReceivedDropPacketCounter(rc) - This 32-bit register is a saturating counter, counting the incoming PktReady to Traffic Manager which are with Token of 0. It's a clear on read register.
*/
#define NFP3800_NBI_TMX_CSR_TMRECEIVEDDROPPACKETCOUNTER      0x00000914
#define   NFP3800_NBI_TMX_CSR_TMRECEIVEDDROPPACKETCOUNTER_TMRECEIVEDDROPPACKETCOUNTER(_x) (((_x) & 0xffffffff) << 0)
#define   NFP3800_NBI_TMX_CSR_TMRECEIVEDDROPPACKETCOUNTER_TMRECEIVEDDROPPACKETCOUNTER_of(_x) (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: NbiTmPcieCmdOutCfg[16] - Pcie Command out Configuration.
    Bits: 29:20 RingNumber(rw) - CLS/IMU/EMU Ring Number for the Pcie Command out (Only used for PCIe VM-QoS).
    Bits: 19    OutputPort(rw) - Output Command port (in NBI Island) for the Pcie Command out.
    Bits: 18:11 Island(rw) - Destination Island for the Pcie Command out.
    Bits: 10:9  Token(rw) - Token field for the Pcie Command out.
    Bits:  8:4  Action(rw) - Action field for the Pcie Command out.
    Bits:  3:0  Target(rw) - Target ID for the Destination Island target for the Pcie Command out.
*/
#define NFP3800_NBI_TMX_CSR_NBITMPCIECMDOUTCFG(_x)           (0x0000091c + (0x4 * ((_x) & 0xf)))
#define   NFP3800_NBI_TMX_CSR_NBITMPCIECMDOUTCFG_RING_NUMBER(_x) (((_x) & 0x3ff) << 20)
#define   NFP3800_NBI_TMX_CSR_NBITMPCIECMDOUTCFG_RING_NUMBER_of(_x) (((_x) >> 20) & 0x3ff)
#define   NFP3800_NBI_TMX_CSR_NBITMPCIECMDOUTCFG_OUTPUT_PORT BIT(19)
#define   NFP3800_NBI_TMX_CSR_NBITMPCIECMDOUTCFG_ISLAND(_x)  (((_x) & 0xff) << 11)
#define   NFP3800_NBI_TMX_CSR_NBITMPCIECMDOUTCFG_ISLAND_of(_x) (((_x) >> 11) & 0xff)
#define   NFP3800_NBI_TMX_CSR_NBITMPCIECMDOUTCFG_TOKEN(_x)   (((_x) & 0x3) << 9)
#define   NFP3800_NBI_TMX_CSR_NBITMPCIECMDOUTCFG_TOKEN_of(_x) (((_x) >> 9) & 0x3)
#define   NFP3800_NBI_TMX_CSR_NBITMPCIECMDOUTCFG_ACTION(_x)  (((_x) & 0x1f) << 4)
#define   NFP3800_NBI_TMX_CSR_NBITMPCIECMDOUTCFG_ACTION_of(_x) (((_x) >> 4) & 0x1f)
#define   NFP3800_NBI_TMX_CSR_NBITMPCIECMDOUTCFG_TARGET(_x)  (((_x) & 0xf) << 0)
#define   NFP3800_NBI_TMX_CSR_NBITMPCIECMDOUTCFG_TARGET_of(_x) (((_x) >> 0) & 0xf)
/*----------------------------------------------------------------
  Register: FreePktOutstandingLimit - Limits the number of outstanding FreePkt commands
    Bits:  1:0  Limit(rw) - Limits the number of Outstanding FreePkt commands
*/
#define NFP3800_NBI_TMX_CSR_FREEPKTOUTSTANDINGLIMIT          0x0000095c
#define   NFP3800_NBI_TMX_CSR_FREEPKTOUTSTANDINGLIMIT_LIMIT(_x) (((_x) & 0x3) << 0)
#define   NFP3800_NBI_TMX_CSR_FREEPKTOUTSTANDINGLIMIT_LIMIT_of(_x) (((_x) >> 0) & 0x3)
#define     NFP3800_NBI_TMX_CSR_FREEPKTOUTSTANDINGLIMIT_LIMIT_48 (0)
#define     NFP3800_NBI_TMX_CSR_FREEPKTOUTSTANDINGLIMIT_LIMIT_64 (1)
#define     NFP3800_NBI_TMX_CSR_FREEPKTOUTSTANDINGLIMIT_LIMIT_96 (2)
#define     NFP3800_NBI_TMX_CSR_FREEPKTOUTSTANDINGLIMIT_LIMIT_128 (3)
/*----------------------------------------------------------------
  Register: TMChannelPcieVmQoSEna_0 - Identifies Channel 0->31 as PCIe vs Network.
    Bits: 31:0  TMChannelPcieVmQoSEna_0(rw) - Identifies Channel 0->31 as PCIe vs Network.
*/
#define NFP3800_NBI_TMX_CSR_TMCHANNELPCIEVMQOSENA_0          0x00000960
#define   NFP3800_NBI_TMX_CSR_TMCHANNELPCIEVMQOSENA_0_TMCHANNELPCIEVMQOSENA_0(_x) (((_x) & 0xffffffff) << 0)
#define   NFP3800_NBI_TMX_CSR_TMCHANNELPCIEVMQOSENA_0_TMCHANNELPCIEVMQOSENA_0_of(_x) (((_x) >> 0) & 0xffffffff)
#define     NFP3800_NBI_TMX_CSR_TMCHANNELPCIEVMQOSENA_0_TMCHANNELPCIEVMQOSENA_0_DEFAULT (0)
#define     NFP3800_NBI_TMX_CSR_TMCHANNELPCIEVMQOSENA_0_TMCHANNELPCIEVMQOSENA_0_PCIE (1)
/*----------------------------------------------------------------
  Register: TMChannelPcieVmQoSEna_1 - Identifies Channel 32->63 as PCIe vs Network.
    Bits: 31:0  TMChannelPcieVmQoSEna_1(rw) - Identifies Channel 32->63 as PCIe vs Network.
*/
#define NFP3800_NBI_TMX_CSR_TMCHANNELPCIEVMQOSENA_1          0x00000964
#define   NFP3800_NBI_TMX_CSR_TMCHANNELPCIEVMQOSENA_1_TMCHANNELPCIEVMQOSENA_1(_x) (((_x) & 0xffffffff) << 0)
#define   NFP3800_NBI_TMX_CSR_TMCHANNELPCIEVMQOSENA_1_TMCHANNELPCIEVMQOSENA_1_of(_x) (((_x) >> 0) & 0xffffffff)
#define     NFP3800_NBI_TMX_CSR_TMCHANNELPCIEVMQOSENA_1_TMCHANNELPCIEVMQOSENA_1_DEFAULT (0)
#define     NFP3800_NBI_TMX_CSR_TMCHANNELPCIEVMQOSENA_1_TMCHANNELPCIEVMQOSENA_1_PCIE (1)
/*----------------------------------------------------------------
  Register: TMChannelPcieVmQoSEna_2 - Identifies Channel 64->95 as PCIe vs Network.
    Bits: 31:0  TMChannelPcieVmQoSEna_2(rw) - Identifies Channel 64->95 as PCIe vs Network.
*/
#define NFP3800_NBI_TMX_CSR_TMCHANNELPCIEVMQOSENA_2          0x00000968
#define   NFP3800_NBI_TMX_CSR_TMCHANNELPCIEVMQOSENA_2_TMCHANNELPCIEVMQOSENA_2(_x) (((_x) & 0xffffffff) << 0)
#define   NFP3800_NBI_TMX_CSR_TMCHANNELPCIEVMQOSENA_2_TMCHANNELPCIEVMQOSENA_2_of(_x) (((_x) >> 0) & 0xffffffff)
#define     NFP3800_NBI_TMX_CSR_TMCHANNELPCIEVMQOSENA_2_TMCHANNELPCIEVMQOSENA_2_DEFAULT (0)
#define     NFP3800_NBI_TMX_CSR_TMCHANNELPCIEVMQOSENA_2_TMCHANNELPCIEVMQOSENA_2_PCIE (1)
/*----------------------------------------------------------------
  Register: TMChannelPcieVmQoSEna_3 - Identifies Channel 96->127 as PCIe vs Network.
    Bits: 31:0  TMChannelPcieVmQoSEna_3(rw) - Identifies Channel 96->127 as PCIe vs Network.
*/
#define NFP3800_NBI_TMX_CSR_TMCHANNELPCIEVMQOSENA_3          0x0000096c
#define   NFP3800_NBI_TMX_CSR_TMCHANNELPCIEVMQOSENA_3_TMCHANNELPCIEVMQOSENA_3(_x) (((_x) & 0xffffffff) << 0)
#define   NFP3800_NBI_TMX_CSR_TMCHANNELPCIEVMQOSENA_3_TMCHANNELPCIEVMQOSENA_3_of(_x) (((_x) >> 0) & 0xffffffff)
#define     NFP3800_NBI_TMX_CSR_TMCHANNELPCIEVMQOSENA_3_TMCHANNELPCIEVMQOSENA_3_DEFAULT (0)
#define     NFP3800_NBI_TMX_CSR_TMCHANNELPCIEVMQOSENA_3_TMCHANNELPCIEVMQOSENA_3_PCIE (1)
/*----------------------------------------------------------------
  Register: ReorderBuffer_perf_seqr_selector - Reorder buffer Seqr selector for Performance bus
    Bits:  3:0  ReorderBuffer_perf_seqr_selector(rw) - Reorder buffer Seqr selector for Performance bus
*/
#define NFP3800_NBI_TMX_CSR_REORDERBUFFER_PERF_SEQR_SELECTOR 0x00000970
#define   NFP3800_NBI_TMX_CSR_REORDERBUFFER_PERF_SEQR_SELECTOR_REORDERBUFFER_PERF_SEQR_SELECTOR(_x) (((_x) & 0xf) << 0)
#define   NFP3800_NBI_TMX_CSR_REORDERBUFFER_PERF_SEQR_SELECTOR_REORDERBUFFER_PERF_SEQR_SELECTOR_of(_x) (((_x) >> 0) & 0xf)
/*----------------------------------------------------------------
  Register: ReorderBufferHeadPointerUpdate - This is a ReadOnly register that contains info about expected Sequence number based on the TrafficManagerConfig(NumSequencers) field
    Bits: 31:0  ReorderBufferHeadPointerUpdate(ro) - Based on TrafficManagerConfig(NumSequencers) field-> this 32-bit register contains following info.
*/
#define NFP3800_NBI_TMX_CSR_REORDER_BUFFER_HEAD_POINTER_UPDATE 0x00000974
#define   NFP3800_NBI_TMX_CSR_REORDER_BUFFER_HEAD_POINTER_UPDATE_REORDER_BUFFER_HEAD_POINTER_UPDATE(_x) (((_x) & 0xffffffff) << 0)
#define   NFP3800_NBI_TMX_CSR_REORDER_BUFFER_HEAD_POINTER_UPDATE_REORDER_BUFFER_HEAD_POINTER_UPDATE_of(_x) (((_x) >> 0) & 0xffffffff)
#define     NFP3800_NBI_TMX_CSR_REORDER_BUFFER_HEAD_POINTER_UPDATE_REORDER_BUFFER_HEAD_POINTER_UPDATE_NUMSEQUENCERS___ (1)
#define     NFP3800_NBI_TMX_CSR_REORDER_BUFFER_HEAD_POINTER_UPDATE_REORDER_BUFFER_HEAD_POINTER_UPDATE_NUMSEQUENCERS____ (2)
/*----------------------------------------------------------------
  Register: ShaperRateSelector - Configuration register for different ShaperRates per level
    Bits:  8:6  L2ShaperRateSelector(rw) - These three bits decide the Range of Shaper rates for Level2 Shapers
    Bits:  5:3  L1ShaperRateSelector(rw) - These three bits decide the Range of Shaper rates for Level1 Shapers
    Bits:  2:0  L0ShaperRateSelector(rw) - These three bits decide the Range of Shaper rates for Level0 Shaper
*/
#define NFP3800_NBI_TMX_CSR_SHAPER_RATE_SELECTOR             0x00000978
#define   NFP3800_NBI_TMX_CSR_SHAPER_RATE_SELECTOR_L2_SHAPER_RATE_SELECTOR(_x) (((_x) & 0x7) << 6)
#define   NFP3800_NBI_TMX_CSR_SHAPER_RATE_SELECTOR_L2_SHAPER_RATE_SELECTOR_of(_x) (((_x) >> 6) & 0x7)
#define     NFP3800_NBI_TMX_CSR_SHAPER_RATE_SELECTOR_L2_SHAPER_RATE_SELECTOR_DEFAULT (0)
#define     NFP3800_NBI_TMX_CSR_SHAPER_RATE_SELECTOR_L2_SHAPER_RATE_SELECTOR_DIVIDE_BY_2 (1)
#define     NFP3800_NBI_TMX_CSR_SHAPER_RATE_SELECTOR_L2_SHAPER_RATE_SELECTOR_DIVIDE_BY_4 (2)
#define     NFP3800_NBI_TMX_CSR_SHAPER_RATE_SELECTOR_L2_SHAPER_RATE_SELECTOR_DIVIDE_BY_8 (3)
#define     NFP3800_NBI_TMX_CSR_SHAPER_RATE_SELECTOR_L2_SHAPER_RATE_SELECTOR_DIVIDE_BY_16 (4)
#define     NFP3800_NBI_TMX_CSR_SHAPER_RATE_SELECTOR_L2_SHAPER_RATE_SELECTOR_DIVIDE_BY_128 (5)
#define     NFP3800_NBI_TMX_CSR_SHAPER_RATE_SELECTOR_L2_SHAPER_RATE_SELECTOR_DIVIDE_BY_512 (6)
#define     NFP3800_NBI_TMX_CSR_SHAPER_RATE_SELECTOR_L2_SHAPER_RATE_SELECTOR_DIVIDE_BY_1024 (7)
#define   NFP3800_NBI_TMX_CSR_SHAPER_RATE_SELECTOR_L1_SHAPER_RATE_SELECTOR(_x) (((_x) & 0x7) << 3)
#define   NFP3800_NBI_TMX_CSR_SHAPER_RATE_SELECTOR_L1_SHAPER_RATE_SELECTOR_of(_x) (((_x) >> 3) & 0x7)
#define     NFP3800_NBI_TMX_CSR_SHAPER_RATE_SELECTOR_L1_SHAPER_RATE_SELECTOR_DEFAULT (0)
#define     NFP3800_NBI_TMX_CSR_SHAPER_RATE_SELECTOR_L1_SHAPER_RATE_SELECTOR_DIVIDE_BY_2 (1)
#define     NFP3800_NBI_TMX_CSR_SHAPER_RATE_SELECTOR_L1_SHAPER_RATE_SELECTOR_DIVIDE_BY_4 (2)
#define     NFP3800_NBI_TMX_CSR_SHAPER_RATE_SELECTOR_L1_SHAPER_RATE_SELECTOR_DIVIDE_BY_8 (3)
#define     NFP3800_NBI_TMX_CSR_SHAPER_RATE_SELECTOR_L1_SHAPER_RATE_SELECTOR_DIVIDE_BY_16 (4)
#define     NFP3800_NBI_TMX_CSR_SHAPER_RATE_SELECTOR_L1_SHAPER_RATE_SELECTOR_DIVIDE_BY_128 (5)
#define     NFP3800_NBI_TMX_CSR_SHAPER_RATE_SELECTOR_L1_SHAPER_RATE_SELECTOR_DIVIDE_BY_512 (6)
#define     NFP3800_NBI_TMX_CSR_SHAPER_RATE_SELECTOR_L1_SHAPER_RATE_SELECTOR_DIVIDE_BY_1024 (7)
#define   NFP3800_NBI_TMX_CSR_SHAPER_RATE_SELECTOR_L0_SHAPER_RATE_SELECTOR(_x) (((_x) & 0x7) << 0)
#define   NFP3800_NBI_TMX_CSR_SHAPER_RATE_SELECTOR_L0_SHAPER_RATE_SELECTOR_of(_x) (((_x) >> 0) & 0x7)
#define     NFP3800_NBI_TMX_CSR_SHAPER_RATE_SELECTOR_L0_SHAPER_RATE_SELECTOR_DEFAULT (0)
#define     NFP3800_NBI_TMX_CSR_SHAPER_RATE_SELECTOR_L0_SHAPER_RATE_SELECTOR_DIVIDE_BY_2 (1)
#define     NFP3800_NBI_TMX_CSR_SHAPER_RATE_SELECTOR_L0_SHAPER_RATE_SELECTOR_DIVIDE_BY_4 (2)
#define     NFP3800_NBI_TMX_CSR_SHAPER_RATE_SELECTOR_L0_SHAPER_RATE_SELECTOR_DIVIDE_BY_8 (3)
#define     NFP3800_NBI_TMX_CSR_SHAPER_RATE_SELECTOR_L0_SHAPER_RATE_SELECTOR_DIVIDE_BY_16 (4)
#define     NFP3800_NBI_TMX_CSR_SHAPER_RATE_SELECTOR_L0_SHAPER_RATE_SELECTOR_DIVIDE_BY_128 (5)
#define     NFP3800_NBI_TMX_CSR_SHAPER_RATE_SELECTOR_L0_SHAPER_RATE_SELECTOR_DIVIDE_BY_512 (6)
#define     NFP3800_NBI_TMX_CSR_SHAPER_RATE_SELECTOR_L0_SHAPER_RATE_SELECTOR_DIVIDE_BY_1024 (7)
/*----------------------------------------------------------------
  Register: QueueStatus[1024] - Status information stored for each queue. There are a total of 1024 queue status registers in the traffic manager.
    Bits: 13    QueueFull(ro) - Queue Full register bit indicating the Descriptor Queue is Full or Almost Full
    Bits: 12:0  QueueLevel(ro) - Queue level register indicating the fill level for this particular queue. One register for each descriptor queue.
*/
#define NFP3800_NBI_TM_Q_QUEUE_STATUS(_x)                    (0x00000000 + (0x4 * ((_x) & 0x3ff)))
#define   NFP3800_NBI_TM_Q_QUEUE_STATUS_QUEUEFULL            BIT(13)
#define   NFP3800_NBI_TM_Q_QUEUE_STATUS_QUEUELEVEL(_x)       (((_x) & 0x1fff) << 0)
#define   NFP3800_NBI_TM_Q_QUEUE_STATUS_QUEUELEVEL_of(_x)    (((_x) >> 0) & 0x1fff)
/*----------------------------------------------------------------
  Register: QueueConfig[1024] - Configuration settings for each queue. There are a total of 1024 queue configuration registers in the traffic manager.
    Bits: 11    DropPrecedenceEnable(rw) - Enable the Drop Precedence feature for this queue. When enabled, the drop precedence bits in the packet descriptor will select the drop profile to be used. When disabled the static drop rate range sets A or B will be used.
    Bits: 10    DropRateRangeSelect(rw) - Select the Drop Rate Range set to use for this queue. There are two range sets A and B that may be used and are selectable per queue.
    Bits:  9:6  QueueSize(rw) - Configures the maximum size of this particular queue. If linked list is enabled, the recommended values are 1024 or higher. (TBD: requires validation in silicon)
    Bits:  2    REDEnable(rw) - Enables random early detection for packet dropping for this particular queue. Packets will be discarded for this queue based on the globally configured drop profiles. The queue is divided into four ranges, the drop percentage is configurable for each range.
    Bits:  1    DropEnable(rw) - Enables packet dropping for this particular queue. Packets will be discarded for this queue when it is full or almost full. DropEnable must be set to 1 by user.
    Bits:  0    QueueEnable(rw) - Enable bit for this particular queue. Set this bit to enable the queue, clear to disable.
*/
#define NFP3800_NBI_TM_Q_QUEUE_CONFIG(_x)                    (0x00001000 + (0x4 * ((_x) & 0x3ff)))
#define   NFP3800_NBI_TM_Q_QUEUE_CONFIG_DROPPRECEDENCEENABLE BIT(11)
#define     NFP3800_NBI_TM_Q_QUEUE_CONFIG_DROPPRECEDENCEENABLE_DISABLE (0 << 11)
#define     NFP3800_NBI_TM_Q_QUEUE_CONFIG_DROPPRECEDENCEENABLE_ENABLE BIT(11)
#define   NFP3800_NBI_TM_Q_QUEUE_CONFIG_DROPRATERANGESELECT  BIT(10)
#define     NFP3800_NBI_TM_Q_QUEUE_CONFIG_DROPRATERANGESELECT_A (0 << 10)
#define     NFP3800_NBI_TM_Q_QUEUE_CONFIG_DROPRATERANGESELECT_B BIT(10)
#define   NFP3800_NBI_TM_Q_QUEUE_CONFIG_QUEUESIZE(_x)        (((_x) & 0xf) << 6)
#define   NFP3800_NBI_TM_Q_QUEUE_CONFIG_QUEUESIZE_of(_x)     (((_x) >> 6) & 0xf)
#define     NFP3800_NBI_TM_Q_QUEUE_CONFIG_QUEUESIZE_8        (3)
#define     NFP3800_NBI_TM_Q_QUEUE_CONFIG_QUEUESIZE_16       (4)
#define     NFP3800_NBI_TM_Q_QUEUE_CONFIG_QUEUESIZE_32       (5)
#define     NFP3800_NBI_TM_Q_QUEUE_CONFIG_QUEUESIZE_64       (6)
#define     NFP3800_NBI_TM_Q_QUEUE_CONFIG_QUEUESIZE_128      (7)
#define     NFP3800_NBI_TM_Q_QUEUE_CONFIG_QUEUESIZE_256      (8)
#define     NFP3800_NBI_TM_Q_QUEUE_CONFIG_QUEUESIZE_512      (9)
#define     NFP3800_NBI_TM_Q_QUEUE_CONFIG_QUEUESIZE_1024     (10)
#define     NFP3800_NBI_TM_Q_QUEUE_CONFIG_QUEUESIZE_2048     (11)
#define     NFP3800_NBI_TM_Q_QUEUE_CONFIG_QUEUESIZE_4096     (12)
#define   NFP3800_NBI_TM_Q_QUEUE_CONFIG_REDENABLE            BIT(2)
#define     NFP3800_NBI_TM_Q_QUEUE_CONFIG_REDENABLE_DISABLED (0 << 2)
#define     NFP3800_NBI_TM_Q_QUEUE_CONFIG_REDENABLE_ENABLED  BIT(2)
#define   NFP3800_NBI_TM_Q_QUEUE_CONFIG_DROPENABLE           BIT(1)
#define     NFP3800_NBI_TM_Q_QUEUE_CONFIG_DROPENABLE_DISABLED (0 << 1)
#define     NFP3800_NBI_TM_Q_QUEUE_CONFIG_DROPENABLE_ENABLED BIT(1)
#define   NFP3800_NBI_TM_Q_QUEUE_CONFIG_QUEUEENABLE          BIT(0)
#define     NFP3800_NBI_TM_Q_QUEUE_CONFIG_QUEUEENABLE_DISABLE (0 << 0)
#define     NFP3800_NBI_TM_Q_QUEUE_CONFIG_QUEUEENABLE_ENABLE BIT(0)
/*----------------------------------------------------------------
  Register: QueueDropCount[1024] - Drop counter for each queue. There are a total of 1024 drop counters in the traffic manager.
    Bits: 31:0  DropCount(ro) - 32-bit drop counter to keep track of the number of packets dropped for this queue. The counter will saturate when the maximum number is reached.
*/
#define NFP3800_NBI_TM_Q_QUEUE_DROP_COUNT(_x)                (0x00002000 + (0x4 * ((_x) & 0x3ff)))
#define   NFP3800_NBI_TM_Q_QUEUE_DROP_COUNT_DROPCOUNT(_x)    (((_x) & 0xffffffff) << 0)
#define   NFP3800_NBI_TM_Q_QUEUE_DROP_COUNT_DROPCOUNT_of(_x) (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: QueueDropCountClear[1024] - Clear on read drop counter for each queue. There are a total of 1024 clear on read drop counters in the traffic manager.
    Bits: 31:0  DropCountClear(rc) - Clear on read 32-bit drop counter to keep track of the number of packets dropped for this queue. The counter will saturate when the maximum number is reached. The read value for this register is identical to the DROPCOUNT register value, however when DROPCOUNTCLEAR is read the drop counter is cleared. The drop count memory is undefined at start-up. This register should be read for all active queues by software prior to device operation.
*/
#define NFP3800_NBI_TM_Q_QUEUE_DROP_COUNT_CLEAR(_x)          (0x00003000 + (0x4 * ((_x) & 0x3ff)))
#define   NFP3800_NBI_TM_Q_QUEUE_DROP_COUNT_CLEAR_DROPCOUNTCLEAR(_x) (((_x) & 0xffffffff) << 0)
#define   NFP3800_NBI_TM_Q_QUEUE_DROP_COUNT_CLEAR_DROPCOUNTCLEAR_of(_x) (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: PacketDescriptorMemoryStatus - Packet Descriptor Memory Status.
    Bits: 14    PacketDescriptorMemoryFull(ro) - Packet Descriptor Memory Full register bit indicating the Descriptor Memory is Full or Almost Full
    Bits: 12:0  PacketDescriptorMemoryLevel(ro) - Packet Descriptor Memory level register indicating the fill level.
*/
#define NFP3800_NBI_TM_Q_PACKET_DESCRIPTOR_MEMORY_STATUS     0x00004000
#define   NFP3800_NBI_TM_Q_PACKET_DESCRIPTOR_MEMORY_STATUS_PACKETDESCRIPTORMEMORYFULL BIT(14)
#define   NFP3800_NBI_TM_Q_PACKET_DESCRIPTOR_MEMORY_STATUS_PACKETDESCRIPTORMEMORYLEVEL(_x) (((_x) & 0x1fff) << 0)
#define   NFP3800_NBI_TM_Q_PACKET_DESCRIPTOR_MEMORY_STATUS_PACKETDESCRIPTORMEMORYLEVEL_of(_x) (((_x) >> 0) & 0x1fff)
/*----------------------------------------------------------------
  Register: CommandDropStatus - Clear on read status register indicating that an outgoing DSF/CPP command has been dropped due to the command fifo being full for 1024 clock cycles.This register is only set once and only set again after the register is read
    Bits: 31:28 CommandDropCTMID(rc) - Read only register indicating the CTM Number for the command that was dropped.
    Bits: 27:22 CommandDropCTMIsland(rc) - Read only register indicating the CTM Island Number for the command that was dropped.
    Bits: 21:12 CommandDropQueueNumber(rc) - Read only register indicating the intended Queue Number for the command that was dropped.
    Bits: 11:3  CommandDropPacketNumber(rc) - Read only register indicating the intended Packet Number for the command that was dropped.
    Bits:  1    SecondTimeFullFIFO(ro) - Bit indicating that either a FreePkt/PPC command was dropped due to FIFO being full. - only gets cleared on reset
    Bits:  0    FirstTimeFullFIFO(ro) - Bit indicating that either a FreePkt/PPC command was dropped due to FIFO being full. - only gets cleared on reset
*/
#define NFP3800_NBI_TM_Q_COMMAND_DROP_STATUS                 0x00004004
#define   NFP3800_NBI_TM_Q_COMMAND_DROP_STATUS_COMMANDDROPCTMID(_x) (((_x) & 0xf) << 28)
#define   NFP3800_NBI_TM_Q_COMMAND_DROP_STATUS_COMMANDDROPCTMID_of(_x) (((_x) >> 28) & 0xf)
#define   NFP3800_NBI_TM_Q_COMMAND_DROP_STATUS_COMMANDDROPCTMISLAND(_x) (((_x) & 0x3f) << 22)
#define   NFP3800_NBI_TM_Q_COMMAND_DROP_STATUS_COMMANDDROPCTMISLAND_of(_x) (((_x) >> 22) & 0x3f)
#define   NFP3800_NBI_TM_Q_COMMAND_DROP_STATUS_COMMANDDROPQUEUENUMBER(_x) (((_x) & 0x3ff) << 12)
#define   NFP3800_NBI_TM_Q_COMMAND_DROP_STATUS_COMMANDDROPQUEUENUMBER_of(_x) (((_x) >> 12) & 0x3ff)
#define   NFP3800_NBI_TM_Q_COMMAND_DROP_STATUS_COMMANDDROPPACKETNUMBER(_x) (((_x) & 0x1ff) << 3)
#define   NFP3800_NBI_TM_Q_COMMAND_DROP_STATUS_COMMANDDROPPACKETNUMBER_of(_x) (((_x) >> 3) & 0x1ff)
#define   NFP3800_NBI_TM_Q_COMMAND_DROP_STATUS_SECONDTIMEFULLFIFO BIT(1)
#define     NFP3800_NBI_TM_Q_COMMAND_DROP_STATUS_SECONDTIMEFULLFIFO_NO_DROP (0 << 1)
#define     NFP3800_NBI_TM_Q_COMMAND_DROP_STATUS_SECONDTIMEFULLFIFO_DROP BIT(1)
#define   NFP3800_NBI_TM_Q_COMMAND_DROP_STATUS_FIRSTTIMEFULLFIFO BIT(0)
#define     NFP3800_NBI_TM_Q_COMMAND_DROP_STATUS_FIRSTTIMEFULLFIFO_NO_DROP (0 << 0)
#define     NFP3800_NBI_TM_Q_COMMAND_DROP_STATUS_FIRSTTIMEFULLFIFO_DROP BIT(0)
/*----------------------------------------------------------------
  Register: PCIeCommandDropStatus - Clear on read status register indicating that an outgoing DSF/CPP command has been dropped due to the command fifo being full for 1024 clock cycles.This register is only set once and only set again after the register is read
    Bits: 31:28 CommandDropLookupAddress(rc) - Read only register indicating the Lookup Address for the command that was dropped.
    Bits: 27:22 CommandDropCTMIsland(rc) - Read only register indicating the CTM Island Number for the command that was dropped.
    Bits:  9:5  CommandDropSequencerNumber(rc) - Read only register indicating the Sequence Number for the command that was dropped.
    Bits:  1    SecondTimeFullFIFO(ro) - Bit indicating that ReorderOnly/Reorder with VM-QoS command was dropped due to FIFO being full. - only gets cleared on reset
    Bits:  0    FirstTimeFullFIFO(ro) - Bit indicating that ReorderOnly/Reorder with VM-QoS command was dropped due to FIFO being full. - only gets cleared on reset
*/
#define NFP3800_NBI_TM_Q_PCIE_COMMAND_DROP_STATUS            0x00004008
#define   NFP3800_NBI_TM_Q_PCIE_COMMAND_DROP_STATUS_COMMANDDROPLOOKUPADDRESS(_x) (((_x) & 0xf) << 28)
#define   NFP3800_NBI_TM_Q_PCIE_COMMAND_DROP_STATUS_COMMANDDROPLOOKUPADDRESS_of(_x) (((_x) >> 28) & 0xf)
#define   NFP3800_NBI_TM_Q_PCIE_COMMAND_DROP_STATUS_COMMANDDROPCTMISLAND(_x) (((_x) & 0x3f) << 22)
#define   NFP3800_NBI_TM_Q_PCIE_COMMAND_DROP_STATUS_COMMANDDROPCTMISLAND_of(_x) (((_x) >> 22) & 0x3f)
#define   NFP3800_NBI_TM_Q_PCIE_COMMAND_DROP_STATUS_COMMANDDROPSEQNCERNUMBER(_x) (((_x) & 0x1f) << 5)
#define   NFP3800_NBI_TM_Q_PCIE_COMMAND_DROP_STATUS_COMMANDDROPSEQNCERNUMBER_of(_x) (((_x) >> 5) & 0x1f)
#define   NFP3800_NBI_TM_Q_PCIE_COMMAND_DROP_STATUS_SECONDTIMEFULLFIFO BIT(1)
#define     NFP3800_NBI_TM_Q_PCIE_COMMAND_DROP_STATUS_SECONDTIMEFULLFIFO_NO_DROP (0 << 1)
#define     NFP3800_NBI_TM_Q_PCIE_COMMAND_DROP_STATUS_SECONDTIMEFULLFIFO_DROP BIT(1)
#define   NFP3800_NBI_TM_Q_PCIE_COMMAND_DROP_STATUS_FIRSTTIMEFULLFIFO BIT(0)
#define     NFP3800_NBI_TM_Q_PCIE_COMMAND_DROP_STATUS_FIRSTTIMEFULLFIFO_NO_DROP (0 << 0)
#define     NFP3800_NBI_TM_Q_PCIE_COMMAND_DROP_STATUS_FIRSTTIMEFULLFIFO_DROP BIT(0)
/*----------------------------------------------------------------
  Register: SchedulerConfig[145] - Configuration register for setup of each scheduler. The level 2 schedulers are at offsets 0-127, the level 1 standard schedulers are at offsets 128-143, and the level 0 scheduler is at offset 144.
    Bits:  2:1  SPEnable(rw) - Strict Priority Enable bits. Configure the scheduler for Strict Priority operation.
    Bits:  0    DWRREnable(rw) - Deficit Weighted Round Robin Enable bit. Set this bit to enable deficit weighted round operation for the particular scheduler.
*/
#define NFP3800_NBI_TMX_SCH_CSR_SCHEDULER_CONFIG(_x)         (0x00000000 + (0x4 * ((_x) & 0xff)))
#define   NFP3800_NBI_TMX_SCH_CSR_SCHEDULER_CONFIG_SPENABLE(_x) (((_x) & 0x3) << 1)
#define   NFP3800_NBI_TMX_SCH_CSR_SCHEDULER_CONFIG_SPENABLE_of(_x) (((_x) >> 1) & 0x3)
#define     NFP3800_NBI_TMX_SCH_CSR_SCHEDULER_CONFIG_SPENABLE_0 (0)
#define     NFP3800_NBI_TMX_SCH_CSR_SCHEDULER_CONFIG_SPENABLE_1 (1)
#define     NFP3800_NBI_TMX_SCH_CSR_SCHEDULER_CONFIG_SPENABLE_2 (2)
#define     NFP3800_NBI_TMX_SCH_CSR_SCHEDULER_CONFIG_SPENABLE_3 (3)
#define   NFP3800_NBI_TMX_SCH_CSR_SCHEDULER_CONFIG_DWRRENABLE BIT(0)
#define     NFP3800_NBI_TMX_SCH_CSR_SCHEDULER_CONFIG_DWRRENABLE_DISABLE (0 << 0)
#define     NFP3800_NBI_TMX_SCH_CSR_SCHEDULER_CONFIG_DWRRENABLE_ENABLE BIT(0)
/*----------------------------------------------------------------
  Register: L1ExtSchedulerConfig[16] - Level 1 Extended Scheduler Configuration register. These registers configure the 16 level 1 extended schedulers.
    Bits:  3    L1ExtSchedEnable(rw) - Enable the extended level 1 scheduler. Each group of 8 L2 scheduler outputs may connect to either the base or extended L1 scheduler inputs with the following options
    Bits:  2:1  L1ExtSPEnable(rw) - Strict Priority Enable bits. Configure the extended scheduler for Strict Priority operation.
    Bits:  0    L1ExtDWRREnable(rw) - Deficit Weighted Round Robin Enable bit. Set this bit to enable dwrr operation for the extended scheduler.
*/
#define NFP3800_NBI_TMX_SCH_CSR_L1EXTSCHEDULER_CONFIG(_x)    (0x00000400 + (0x4 * ((_x) & 0xf)))
#define   NFP3800_NBI_TMX_SCH_CSR_L1EXTSCHEDULER_CONFIG_L1EXTSCHEDENABLE BIT(3)
#define     NFP3800_NBI_TMX_SCH_CSR_L1EXTSCHEDULER_CONFIG_L1EXTSCHEDENABLE_0 (0 << 3)
#define     NFP3800_NBI_TMX_SCH_CSR_L1EXTSCHEDULER_CONFIG_L1EXTSCHEDENABLE_1 BIT(3)
#define   NFP3800_NBI_TMX_SCH_CSR_L1EXTSCHEDULER_CONFIG_L1EXTSPENABLE(_x) (((_x) & 0x3) << 1)
#define   NFP3800_NBI_TMX_SCH_CSR_L1EXTSCHEDULER_CONFIG_L1EXTSPENABLE_of(_x) (((_x) >> 1) & 0x3)
#define     NFP3800_NBI_TMX_SCH_CSR_L1EXTSCHEDULER_CONFIG_L1EXTSPENABLE_0 (0)
#define     NFP3800_NBI_TMX_SCH_CSR_L1EXTSCHEDULER_CONFIG_L1EXTSPENABLE_1 (1)
#define     NFP3800_NBI_TMX_SCH_CSR_L1EXTSCHEDULER_CONFIG_L1EXTSPENABLE_2 (2)
#define     NFP3800_NBI_TMX_SCH_CSR_L1EXTSCHEDULER_CONFIG_L1EXTSPENABLE_3 (3)
#define   NFP3800_NBI_TMX_SCH_CSR_L1EXTSCHEDULER_CONFIG_L1EXTDWRRENABLE BIT(0)
#define     NFP3800_NBI_TMX_SCH_CSR_L1EXTSCHEDULER_CONFIG_L1EXTDWRRENABLE_DISABLE (0 << 0)
#define     NFP3800_NBI_TMX_SCH_CSR_L1EXTSCHEDULER_CONFIG_L1EXTDWRRENABLE_ENABLE BIT(0)
/*----------------------------------------------------------------
  Register: SchedulerWeight[128] - L0 Scheduler weight value to configure the relative bandwidth for each scheduler port. There are a total of 128 weight values to program in the level 0 scheduler.
    Bits: 23:0  Weight(rw) - Scheduler weight value. Program this memory with the desired weight value for the particular scheduler port. This memory is undefined at start-up and should be programmed or set to zero by software. If all weights for all ports of a particular port are set to zero, the scheduler will operate in round robin fashion. The weight value will configure the relative bandwidth for the port and is used to update the deficit value. The weight should be programmed to the desired relative bandwidth for this port as compared to the other ports on this scheduler.
*/
#define NFP3800_NBI_TMX_SCH_CSR_SCHEDULER_WEIGHT(_x)         (0x00000800 + (0x4 * ((_x) & 0x7f)))
#define   NFP3800_NBI_TMX_SCH_CSR_SCHEDULER_WEIGHT_WEIGHT(_x) (((_x) & 0xffffff) << 0)
#define   NFP3800_NBI_TMX_SCH_CSR_SCHEDULER_WEIGHT_WEIGHT_of(_x) (((_x) >> 0) & 0xffffff)
/*----------------------------------------------------------------
  Register: SchedulerDeficit[128] - L0 Scheduler deficit value for each scheduler port. There are a total of 128 deficit values in the level 0 scheduler.
    Bits: 24:0  Deficit(rw) - Scheduler deficit value. This memory provides the signed value of the deficit for the particular scheduler port. This memory is undefined at start-up and should be programmed to zero by software. If all of the ports either have a negative deficit or do not have data to send, the deficit values will be updated for all ports. During the update if the port does not have data to send the deficit will be set to the weight value. If the deficit has gone negative, the weight value will be added to the current deficit.
*/
#define NFP3800_NBI_TMX_SCH_CSR_SCHEDULER_DEFICIT(_x)        (0x00001000 + (0x4 * ((_x) & 0x7f)))
#define   NFP3800_NBI_TMX_SCH_CSR_SCHEDULER_DEFICIT_DEFICIT(_x) (((_x) & 0x1ffffff) << 0)
#define   NFP3800_NBI_TMX_SCH_CSR_SCHEDULER_DEFICIT_DEFICIT_of(_x) (((_x) >> 0) & 0x1ffffff)
/*----------------------------------------------------------------
  Register: ShaperRate[145] - Shaper rate register. Configures the maximum allowed rate. The level 2 shaper registers are at offsets 0-127, the level 1 shaper registers are at offsets 128-143, and the level 0 shaper register is at offset 144.
  Register: L1ExtShaperRate[16] - Level 1 extended shaper rate register. Configures the maximum allowed rate for the 16 Level 1 extended shapers.
    Bits: 13:0  Rate(rw) - The shaper rate register. Valid values may range in size from 1 to 12,000 and correspond to data rates of 8Mbps to 96Gbps when using a 800MHz PCLK. Values outside that range are not supported (i.e. 0, or above 12,000). There are configuration bits in TrafficManagerConfig[2;18] which is used as a divider for the clk frequency used for ShaperRate calculation. TrafficManagerConfig[2;18] set to 0: ShaperRate 8Mbps -> 96Gbps, 1: 4Mbps -> 48Gbps, 2: 2Mbps -> 24Gbps, 4: 1Mbps -> 12Gbps
*/
#define NFP3800_NBI_TMX_SHP_SHAPER_RATE(_x)                  (0x00000000 + (0x4 * ((_x) & 0xff)))
#define NFP3800_NBI_TMX_SHP_L1EXT_SHAPER_RATE(_x)            (0x00000300 + (0x4 * ((_x) & 0xf)))
#define   NFP3800_NBI_TMX_SHP_SHAPERRATE_RATE(_x)            (((_x) & 0x3fff) << 0)
#define   NFP3800_NBI_TMX_SHP_SHAPERRATE_RATE_of(_x)         (((_x) >> 0) & 0x3fff)
/*----------------------------------------------------------------
  Register: ShaperThreshold[145] - Rate limiting shaper threshold register. Configures the desired threshold level of the leaky bucket. The level 2 shaper registers are at offsets 0-127, the level 1 shaper registers are at offsets 128-143, and the level 0 shaper register is at offset 144.
  Register: L1ExtShaperThreshold[16] - Level 1 extended shaper threshold register. Configures the desired threshold level of the leaky bucket for the 16 level 1 extended shapers.
    Bits:  2:0  Threshold(rw) - The rate limiting shaper threshold register. This register configures the threshold in bytes used for comparison to the instantaneous level of the leaky bucket. When the number of bytes sent is below this threshold the shaper will be open, when the bucket level equals or exceeds the threshold the shaper will be closed.
*/
#define NFP3800_NBI_TMX_SHP_SHAPER_THRESHOLD(_x)             (0x00000800 + (0x4 * ((_x) & 0xff)))
#define NFP3800_NBI_TMX_SHP_L1EXT_SHAPER_THRESHOLD(_x)       (0x00000b00 + (0x4 * ((_x) & 0xf)))
#define   NFP3800_NBI_TMX_SHP_SHAPERTHRESHOLD_THRESHOLD(_x)  (((_x) & 0x7) << 0)
#define   NFP3800_NBI_TMX_SHP_SHAPERTHRESHOLD_THRESHOLD_of(_x) (((_x) >> 0) & 0x7)
#define     NFP3800_NBI_TMX_SHP_SHAPERTHRESHOLD_THRESHOLD_8192 (0)
#define     NFP3800_NBI_TMX_SHP_SHAPERTHRESHOLD_THRESHOLD_16384 (1)
#define     NFP3800_NBI_TMX_SHP_SHAPERTHRESHOLD_THRESHOLD_24576 (2)
#define     NFP3800_NBI_TMX_SHP_SHAPERTHRESHOLD_THRESHOLD_32768 (3)
#define     NFP3800_NBI_TMX_SHP_SHAPERTHRESHOLD_THRESHOLD_40960 (4)
#define     NFP3800_NBI_TMX_SHP_SHAPERTHRESHOLD_THRESHOLD_49152 (5)
#define     NFP3800_NBI_TMX_SHP_SHAPERTHRESHOLD_THRESHOLD_57344 (6)
#define     NFP3800_NBI_TMX_SHP_SHAPERTHRESHOLD_THRESHOLD_65535 (7)
/*----------------------------------------------------------------
  Register: ShaperMaxOvershoot[145] - Rate limiting shaper maximum overshoot register. Configures the maximum allowed overshoot for the leaky bucket. The level 2 shaper registers are at offsets 0-127, the level 1 shaper registers are at offsets 128-143, and the level 0 shaper register is at offset 144
  Register: L1ExtShaperMaxOvershoot[16] - Level 1 extended shaper maximum overshoot register. Configures the maximum allowed overshoot of the leaky bucket for the 16 level 1 extended shapers.
    Bits:  2:0  MaxOvershoot(rw) - The rate limiting shaper maximum overshoot register. This register configures the maximum allowed overshoot in bytes for the leaky bucket. Data that is sent beyond this limit will not be accounted for in the leaky bucket algorithm.
*/
#define NFP3800_NBI_TMX_SHP_SHAPER_MAX_OVERSHOOT(_x)         (0x00001000 + (0x4 * ((_x) & 0xff)))
#define NFP3800_NBI_TMX_SHP_L1EXT_SHAPER_MAX_OVERSHOOT(_x)   (0x00001300 + (0x4 * ((_x) & 0xf)))
#define   NFP3800_NBI_TMX_SHP_SHAPERMAXOVERSHOOT_MAXOVERSHOOT(_x) (((_x) & 0x7) << 0)
#define   NFP3800_NBI_TMX_SHP_SHAPERMAXOVERSHOOT_MAXOVERSHOOT_of(_x) (((_x) >> 0) & 0x7)
#define     NFP3800_NBI_TMX_SHP_SHAPERMAXOVERSHOOT_MAXOVERSHOOT_16384 (0)
#define     NFP3800_NBI_TMX_SHP_SHAPERMAXOVERSHOOT_MAXOVERSHOOT_24576 (1)
#define     NFP3800_NBI_TMX_SHP_SHAPERMAXOVERSHOOT_MAXOVERSHOOT_32768 (2)
#define     NFP3800_NBI_TMX_SHP_SHAPERMAXOVERSHOOT_MAXOVERSHOOT_40960 (3)
#define     NFP3800_NBI_TMX_SHP_SHAPERMAXOVERSHOOT_MAXOVERSHOOT_49152 (4)
#define     NFP3800_NBI_TMX_SHP_SHAPERMAXOVERSHOOT_MAXOVERSHOOT_57344 (5)
#define     NFP3800_NBI_TMX_SHP_SHAPERMAXOVERSHOOT_MAXOVERSHOOT_65536 (6)
#define     NFP3800_NBI_TMX_SHP_SHAPERMAXOVERSHOOT_MAXOVERSHOOT_131071 (7)
/*----------------------------------------------------------------
  Register: ShaperRateAdjust[145] - Shaper rate adjust register. Configures the fixed data rate adjustment for packet overhead. The level 2 shaper registers are at offsets 0-127, the level 1 shaper registers are at offsets 128-143, and the level 0 shaper register is at offset 144.
  Register: L1ExtShaperRateAdjust[16] - Level 1 extended shaper rate adjust register. Configures the fixed data rate adjustment for packet overheadfor the 16 level 1 extended shapers.
    Bits:  9:0  RateAdjust(rw) - The shaper rate adjust register. This register contains the standard number of bytes to be subtracted for every packet that is transmitted. This can acount for packet overhead or interpacket gap not to be included in the shaper rate calculation. In cases where the rate adjust value is larger than the packet size, the difference will be deducted from the shaper bucket. Positive values will subtract bytes and negative values will add bytes, program in twos compliment format -512 to 511. Negative values will limit the Max Packet size supported to 15,838 bytes
*/
#define NFP3800_NBI_TMX_SHP_SHAPER_RATE_ADJUST(_x)           (0x00001800 + (0x4 * ((_x) & 0xff)))
#define NFP3800_NBI_TMX_SHP_L1EXT_SHAPER_RATE_ADJUST(_x)     (0x00001b00 + (0x4 * ((_x) & 0xf)))
#define   NFP3800_NBI_TMX_SHP_SHAPERRATEADJUST_RATE(_x)      (((_x) & 0x3ff) << 0)
#define   NFP3800_NBI_TMX_SHP_SHAPERRATEADJUST_RATE_of(_x)   (((_x) >> 0) & 0x3ff)
/*----------------------------------------------------------------
  Register: ShaperStatus[145] - Shaper status register. Contains read-only status information for the rate liming shaper. The level 2 shaper registers are at offsets 0-127, the level 1 shaper registers are at offsets 128-143, and the level 0 shaper register is at offset 144.
  Register: L1ExtShaperStatus[16] - Level 1 extended shaper status register. Contains read-only status information for the 16 level 1 extended shapers.
    Bits: 17    ShaperOpen(ro) - Indicates the state of the leaky bucket
    Bits: 16:0  Level(ro) - The current leaky bucket level of the rate limiting shaper. This register contains the present level of the leaky bucket that is used to determine open or closed state. If this value is below the threshold the shaper will be open, if it is above the threshold the shaper will be closed.
*/
#define NFP3800_NBI_TMX_SHP_SHAPER_STATUS(_x)                (0x00002000 + (0x4 * ((_x) & 0xff)))
#define NFP3800_NBI_TMX_SHP_L1EXT_SHAPER_STATUS(_x)          (0x00002300 + (0x4 * ((_x) & 0xf)))
#define   NFP3800_NBI_TMX_SHP_SHAPERSTATUS_SHAPEROPEN        BIT(17)
#define     NFP3800_NBI_TMX_SHP_SHAPERSTATUS_SHAPEROPEN_CLOSED (0 << 17)
#define     NFP3800_NBI_TMX_SHP_SHAPERSTATUS_SHAPEROPEN_OPEN BIT(17)
#define   NFP3800_NBI_TMX_SHP_SHAPERSTATUS_LEVEL(_x)         (((_x) & 0x1ffff) << 0)
#define   NFP3800_NBI_TMX_SHP_SHAPERSTATUS_LEVEL_of(_x)      (((_x) >> 0) & 0x1ffff)

#else /* NFP3800_LONGNAMES */

/*----------------------------------------------------------------
  Register: TMDescSram_TMDescSramEntry[4096] - NBI TM Descriptor SRAM Entry
    Bits: 63:0  DescSRAMData(rw) - 64-bit descriptor stored in the TM Descriptor SRAM. Refer to the Packet Descriptor Memory tables in the MAS for bit definitions
*/
#define NFP3800_NBI_TM_TMDescSram_TMDescSramEntry(_x)        (0x00000000 + (0x8 * ((_x) & 0xfff)))
#define   NFP3800_NBI_TM_TMDescSram_TMDescSramEntry_DescSRAMData(_x) (((_x) & 0xffffffffffffffff) << 0)
#define   NFP3800_NBI_TM_TMDescSram_TMDescSramEntry_DescSRAMData_of(_x) (((_x) >> 0) & 0xffffffffffffffff)
/*----------------------------------------------------------------
  Register: TMNextPtrSram_TMNextPtrSramEntry[4096] - NBI TM NextPtr SRAM Entry
    Bits: 11:0  NextPtrSRAMData(rw) - 13-bits of Nextptr, initialize memory with available Nextptr's
*/
#define NFP3800_NBI_TM_TMNextPtrSram_TMNextPtrSramEntry(_x)  (0x00020000 + (0x8 * ((_x) & 0xfff)))
#define   NFP3800_NBI_TM_TMNextPtrSram_TMNextPtrSramEntry_NextPtrSRAMData(_x) (((_x) & 0xfff) << 0)
#define   NFP3800_NBI_TM_TMNextPtrSram_TMNextPtrSramEntry_NextPtrSRAMData_of(_x) (((_x) >> 0) & 0xfff)
/*----------------------------------------------------------------
  Register: TMBDSram_NbiBDSramEntry[4096] - NBI TM Buffer Descriptor SRAM Entry
    Bits: 28:0  MuAddr(rw) - Memory Unit Address of the buffer (internal/external MU). Maps to CPP Address[29;11] Refer to the NBI top-level Master Commands section for memory unit address translation.
*/
#define NFP3800_NBI_TM_TMBDSram_NbiBDSramEntry(_x)           (0x00040000 + (0x8 * ((_x) & 0xfff)))
#define   NFP3800_NBI_TM_TMBDSram_NbiBDSramEntry_MuAddr(_x)  (((_x) & 0x1fffffff) << 0)
#define   NFP3800_NBI_TM_TMBDSram_NbiBDSramEntry_MuAddr_of(_x) (((_x) >> 0) & 0x1fffffff)
/*----------------------------------------------------------------
  Register: TMBufferList_BLQueCtrl0 - Configures the buffer list circular queue and provides status
    Bits: 55:48 BLQEventFullCnt(rc) - Debug counter that increments when a BLQ Event is dropped due to a full BLQ Event FIFO. This counter saturates at all 1's
    Bits: 42:40 BDFifoEntryCnt(ro) - Number of buffer descriptors resident in the buffer descriptor register FIFO
    Bits: 39    BLQFull(ro) - The Queue is full when set.
    Bits: 38    BLQEmpty(ro) - The queue is empty when set.
    Bits: 37:36 BLQSize(rw) - Defines the Size of the Circular Buffer List Queue
    Bits: 34:24 BLEntryCnt(ro) - Hardware calculated value of the number of entries on the Buffer List Queue. A full Buffer List Queue has an entry count equal to zero, with the BLQFull flag set. An empty Buffer List Queue has an entry count equal to zero with the BLQEmpty flag set.
    Bits: 22:12 BLHdPtr(rw) - Buffer List Queue Head Pointer. Points to the next entry to be read by hardware.
    Bits: 10:0  BLTlPtr(rw) - Buffer List Queue Tail Pointer. Points to the next entry to be written at the tail.
*/
#define NFP3800_NBI_TM_TMBufferList_BLQueCtrl0               0x00048000
#define   NFP3800_NBI_TM_TMBufferList_BLQueCtrl0_BLQEventFullCnt(_x) (((_x) & 0xff) << 48)
#define   NFP3800_NBI_TM_TMBufferList_BLQueCtrl0_BLQEventFullCnt_of(_x) (((_x) >> 48) & 0xff)
#define   NFP3800_NBI_TM_TMBufferList_BLQueCtrl0_BDFifoEntryCnt(_x) (((_x) & 0x7) << 40)
#define   NFP3800_NBI_TM_TMBufferList_BLQueCtrl0_BDFifoEntryCnt_of(_x) (((_x) >> 40) & 0x7)
#define   NFP3800_NBI_TM_TMBufferList_BLQueCtrl0_BLQFull     BIT_ULL(39)
#define   NFP3800_NBI_TM_TMBufferList_BLQueCtrl0_BLQEmpty    BIT_ULL(38)
#define   NFP3800_NBI_TM_TMBufferList_BLQueCtrl0_BLQSize(_x) (((_x) & 0x3) << 36)
#define   NFP3800_NBI_TM_TMBufferList_BLQueCtrl0_BLQSize_of(_x) (((_x) >> 36) & 0x3)
#define     NFP3800_NBI_TM_TMBufferList_BLQueCtrl0_BLQSize_256 (0)
#define     NFP3800_NBI_TM_TMBufferList_BLQueCtrl0_BLQSize_512 (1)
#define     NFP3800_NBI_TM_TMBufferList_BLQueCtrl0_BLQSize_1K (2)
#define     NFP3800_NBI_TM_TMBufferList_BLQueCtrl0_BLQSize_2K (3)
#define   NFP3800_NBI_TM_TMBufferList_BLQueCtrl0_BLEntryCnt(_x) (((_x) & 0x7ff) << 24)
#define   NFP3800_NBI_TM_TMBufferList_BLQueCtrl0_BLEntryCnt_of(_x) (((_x) >> 24) & 0x7ff)
#define   NFP3800_NBI_TM_TMBufferList_BLQueCtrl0_BLHdPtr(_x) (((_x) & 0x7ff) << 12)
#define   NFP3800_NBI_TM_TMBufferList_BLQueCtrl0_BLHdPtr_of(_x) (((_x) >> 12) & 0x7ff)
#define   NFP3800_NBI_TM_TMBufferList_BLQueCtrl0_BLTlPtr(_x) (((_x) & 0x7ff) << 0)
#define   NFP3800_NBI_TM_TMBufferList_BLQueCtrl0_BLTlPtr_of(_x) (((_x) >> 0) & 0x7ff)
/*----------------------------------------------------------------
  Register: TMBufferList_BLQueCtrl1 - Configures the buffer list circular queue and provides status
    Bits: 55:48 BLQEventFullCnt(rc) - Debug counter that increments when a BLQ Event is dropped due to a full BLQ Event FIFO. This counter saturates at all 1's
    Bits: 42:40 BDFifoEntryCnt(ro) - Number of buffer descriptors resident in the buffer descriptor register FIFO
    Bits: 39    BLQFull(ro) - The Queue is full when set.
    Bits: 38    BLQEmpty(ro) - The queue is empty when set.
    Bits: 37:36 BLQSize(rw) - Defines the Size of the Circular Buffer List Queue
    Bits: 34:24 BLEntryCnt(ro) - Hardware calculated value of the number of entries on the Buffer List Queue. A full Buffer List Queue has an entry count equal to zero, with the BLQFull flag set. An empty Buffer List Queue has an entry count equal to zero with the BLQEmpty flag set.
    Bits: 22:12 BLHdPtr(rw) - Buffer List Queue Head Pointer. Points to the next entry to be read by hardware.
    Bits: 10:0  BLTlPtr(rw) - Buffer List Queue Tail Pointer. Points to the next entry to be written at the tail.
*/
#define NFP3800_NBI_TM_TMBufferList_BLQueCtrl1               0x00048008
#define   NFP3800_NBI_TM_TMBufferList_BLQueCtrl1_BLQEventFullCnt(_x) (((_x) & 0xff) << 48)
#define   NFP3800_NBI_TM_TMBufferList_BLQueCtrl1_BLQEventFullCnt_of(_x) (((_x) >> 48) & 0xff)
#define   NFP3800_NBI_TM_TMBufferList_BLQueCtrl1_BDFifoEntryCnt(_x) (((_x) & 0x7) << 40)
#define   NFP3800_NBI_TM_TMBufferList_BLQueCtrl1_BDFifoEntryCnt_of(_x) (((_x) >> 40) & 0x7)
#define   NFP3800_NBI_TM_TMBufferList_BLQueCtrl1_BLQFull     BIT_ULL(39)
#define   NFP3800_NBI_TM_TMBufferList_BLQueCtrl1_BLQEmpty    BIT_ULL(38)
#define   NFP3800_NBI_TM_TMBufferList_BLQueCtrl1_BLQSize(_x) (((_x) & 0x3) << 36)
#define   NFP3800_NBI_TM_TMBufferList_BLQueCtrl1_BLQSize_of(_x) (((_x) >> 36) & 0x3)
#define     NFP3800_NBI_TM_TMBufferList_BLQueCtrl1_BLQSize_256 (0)
#define     NFP3800_NBI_TM_TMBufferList_BLQueCtrl1_BLQSize_512 (1)
#define     NFP3800_NBI_TM_TMBufferList_BLQueCtrl1_BLQSize_1K (2)
#define     NFP3800_NBI_TM_TMBufferList_BLQueCtrl1_BLQSize_2K (3)
#define   NFP3800_NBI_TM_TMBufferList_BLQueCtrl1_BLEntryCnt(_x) (((_x) & 0x7ff) << 24)
#define   NFP3800_NBI_TM_TMBufferList_BLQueCtrl1_BLEntryCnt_of(_x) (((_x) >> 24) & 0x7ff)
#define   NFP3800_NBI_TM_TMBufferList_BLQueCtrl1_BLHdPtr(_x) (((_x) & 0x7ff) << 12)
#define   NFP3800_NBI_TM_TMBufferList_BLQueCtrl1_BLHdPtr_of(_x) (((_x) >> 12) & 0x7ff)
#define   NFP3800_NBI_TM_TMBufferList_BLQueCtrl1_BLTlPtr(_x) (((_x) & 0x7ff) << 0)
#define   NFP3800_NBI_TM_TMBufferList_BLQueCtrl1_BLTlPtr_of(_x) (((_x) >> 0) & 0x7ff)
/*----------------------------------------------------------------
  Register: TMBufferList_BLQueCtrl2 - Configures the buffer list circular queue and provides status
    Bits: 55:48 BLQEventFullCnt(rc) - Debug counter that increments when a BLQ Event is dropped due to a full BLQ Event FIFO. This counter saturates at all 1's
    Bits: 42:40 BDFifoEntryCnt(ro) - Number of buffer descriptors resident in the buffer descriptor register FIFO
    Bits: 39    BLQFull(ro) - The Queue is full when set.
    Bits: 38    BLQEmpty(ro) - The queue is empty when set.
    Bits: 37:36 BLQSize(rw) - Defines the Size of the Circular Buffer List Queue
    Bits: 34:24 BLEntryCnt(ro) - Hardware calculated value of the number of entries on the Buffer List Queue. A full Buffer List Queue has an entry count equal to zero, with the BLQFull flag set. An empty Buffer List Queue has an entry count equal to zero with the BLQEmpty flag set.
    Bits: 22:12 BLHdPtr(rw) - Buffer List Queue Head Pointer. Points to the next entry to be read by hardware.
    Bits: 10:0  BLTlPtr(rw) - Buffer List Queue Tail Pointer. Points to the next entry to be written at the tail.
*/
#define NFP3800_NBI_TM_TMBufferList_BLQueCtrl2               0x00048010
#define   NFP3800_NBI_TM_TMBufferList_BLQueCtrl2_BLQEventFullCnt(_x) (((_x) & 0xff) << 48)
#define   NFP3800_NBI_TM_TMBufferList_BLQueCtrl2_BLQEventFullCnt_of(_x) (((_x) >> 48) & 0xff)
#define   NFP3800_NBI_TM_TMBufferList_BLQueCtrl2_BDFifoEntryCnt(_x) (((_x) & 0x7) << 40)
#define   NFP3800_NBI_TM_TMBufferList_BLQueCtrl2_BDFifoEntryCnt_of(_x) (((_x) >> 40) & 0x7)
#define   NFP3800_NBI_TM_TMBufferList_BLQueCtrl2_BLQFull     BIT_ULL(39)
#define   NFP3800_NBI_TM_TMBufferList_BLQueCtrl2_BLQEmpty    BIT_ULL(38)
#define   NFP3800_NBI_TM_TMBufferList_BLQueCtrl2_BLQSize(_x) (((_x) & 0x3) << 36)
#define   NFP3800_NBI_TM_TMBufferList_BLQueCtrl2_BLQSize_of(_x) (((_x) >> 36) & 0x3)
#define     NFP3800_NBI_TM_TMBufferList_BLQueCtrl2_BLQSize_256 (0)
#define     NFP3800_NBI_TM_TMBufferList_BLQueCtrl2_BLQSize_512 (1)
#define     NFP3800_NBI_TM_TMBufferList_BLQueCtrl2_BLQSize_1K (2)
#define     NFP3800_NBI_TM_TMBufferList_BLQueCtrl2_BLQSize_2K (3)
#define   NFP3800_NBI_TM_TMBufferList_BLQueCtrl2_BLEntryCnt(_x) (((_x) & 0x7ff) << 24)
#define   NFP3800_NBI_TM_TMBufferList_BLQueCtrl2_BLEntryCnt_of(_x) (((_x) >> 24) & 0x7ff)
#define   NFP3800_NBI_TM_TMBufferList_BLQueCtrl2_BLHdPtr(_x) (((_x) & 0x7ff) << 12)
#define   NFP3800_NBI_TM_TMBufferList_BLQueCtrl2_BLHdPtr_of(_x) (((_x) >> 12) & 0x7ff)
#define   NFP3800_NBI_TM_TMBufferList_BLQueCtrl2_BLTlPtr(_x) (((_x) & 0x7ff) << 0)
#define   NFP3800_NBI_TM_TMBufferList_BLQueCtrl2_BLTlPtr_of(_x) (((_x) >> 0) & 0x7ff)
/*----------------------------------------------------------------
  Register: TMBufferList_BLQueCtrl3 - Configures the buffer list circular queue and provides status
    Bits: 55:48 BLQEventFullCnt(rc) - Debug counter that increments when a BLQ Event is dropped due to a full BLQ Event FIFO. This counter saturates at all 1's
    Bits: 42:40 BDFifoEntryCnt(ro) - Number of buffer descriptors resident in the buffer descriptor register FIFO
    Bits: 39    BLQFull(ro) - The Queue is full when set.
    Bits: 38    BLQEmpty(ro) - The queue is empty when set.
    Bits: 37:36 BLQSize(rw) - Defines the Size of the Circular Buffer List Queue
    Bits: 34:24 BLEntryCnt(ro) - Hardware calculated value of the number of entries on the Buffer List Queue. A full Buffer List Queue has an entry count equal to zero, with the BLQFull flag set. An empty Buffer List Queue has an entry count equal to zero with the BLQEmpty flag set.
    Bits: 22:12 BLHdPtr(rw) - Buffer List Queue Head Pointer. Points to the next entry to be read by hardware.
    Bits: 10:0  BLTlPtr(rw) - Buffer List Queue Tail Pointer. Points to the next entry to be written at the tail.
*/
#define NFP3800_NBI_TM_TMBufferList_BLQueCtrl3               0x00048018
#define   NFP3800_NBI_TM_TMBufferList_BLQueCtrl3_BLQEventFullCnt(_x) (((_x) & 0xff) << 48)
#define   NFP3800_NBI_TM_TMBufferList_BLQueCtrl3_BLQEventFullCnt_of(_x) (((_x) >> 48) & 0xff)
#define   NFP3800_NBI_TM_TMBufferList_BLQueCtrl3_BDFifoEntryCnt(_x) (((_x) & 0x7) << 40)
#define   NFP3800_NBI_TM_TMBufferList_BLQueCtrl3_BDFifoEntryCnt_of(_x) (((_x) >> 40) & 0x7)
#define   NFP3800_NBI_TM_TMBufferList_BLQueCtrl3_BLQFull     BIT_ULL(39)
#define   NFP3800_NBI_TM_TMBufferList_BLQueCtrl3_BLQEmpty    BIT_ULL(38)
#define   NFP3800_NBI_TM_TMBufferList_BLQueCtrl3_BLQSize(_x) (((_x) & 0x3) << 36)
#define   NFP3800_NBI_TM_TMBufferList_BLQueCtrl3_BLQSize_of(_x) (((_x) >> 36) & 0x3)
#define     NFP3800_NBI_TM_TMBufferList_BLQueCtrl3_BLQSize_256 (0)
#define     NFP3800_NBI_TM_TMBufferList_BLQueCtrl3_BLQSize_512 (1)
#define     NFP3800_NBI_TM_TMBufferList_BLQueCtrl3_BLQSize_1K (2)
#define     NFP3800_NBI_TM_TMBufferList_BLQueCtrl3_BLQSize_2K (3)
#define   NFP3800_NBI_TM_TMBufferList_BLQueCtrl3_BLEntryCnt(_x) (((_x) & 0x7ff) << 24)
#define   NFP3800_NBI_TM_TMBufferList_BLQueCtrl3_BLEntryCnt_of(_x) (((_x) >> 24) & 0x7ff)
#define   NFP3800_NBI_TM_TMBufferList_BLQueCtrl3_BLHdPtr(_x) (((_x) & 0x7ff) << 12)
#define   NFP3800_NBI_TM_TMBufferList_BLQueCtrl3_BLHdPtr_of(_x) (((_x) >> 12) & 0x7ff)
#define   NFP3800_NBI_TM_TMBufferList_BLQueCtrl3_BLTlPtr(_x) (((_x) & 0x7ff) << 0)
#define   NFP3800_NBI_TM_TMBufferList_BLQueCtrl3_BLTlPtr_of(_x) (((_x) >> 0) & 0x7ff)
/*----------------------------------------------------------------
  Register: TMPktSram_TMPktSramEntry[2048] - NBI TM Packet SRAM Entry
    Bits: 63:0  TMPktSRAMData(rw) - 64-bits of packet data within the TM Packet Buffer SRAM. Refer to Packet Buffer Entry for details
*/
#define NFP3800_NBI_TM_TMPktSram_TMPktSramEntry(_x)          (0x00060000 + (0x8 * ((_x) & 0x7ff)))
#define   NFP3800_NBI_TM_TMPktSram_TMPktSramEntry_TMPktSRAMData(_x) (((_x) & 0xffffffffffffffff) << 0)
#define   NFP3800_NBI_TM_TMPktSram_TMPktSramEntry_TMPktSRAMData_of(_x) (((_x) >> 0) & 0xffffffffffffffff)
/*----------------------------------------------------------------
  Register: TMHeadTailSram_TMHeadTailEntry[1024] - NBI TM Head and Tail SRAM Entry
    Bits: 25:14 HeadPointer(rw) - 13-bit Descriptor Queue Head Pointer
    Bits: 11:0  TailPointer(rw) - 13-bit Descriptor Queue Tail Pointer
*/
#define NFP3800_NBI_TM_TMHeadTailSram_TMHeadTailEntry(_x)    (0x00068000 + (0x8 * ((_x) & 0x3ff)))
#define   NFP3800_NBI_TM_TMHeadTailSram_TMHeadTailEntry_HeadPointer(_x) (((_x) & 0xfff) << 14)
#define   NFP3800_NBI_TM_TMHeadTailSram_TMHeadTailEntry_HeadPointer_of(_x) (((_x) >> 14) & 0xfff)
#define   NFP3800_NBI_TM_TMHeadTailSram_TMHeadTailEntry_TailPointer(_x) (((_x) & 0xfff) << 0)
#define   NFP3800_NBI_TM_TMHeadTailSram_TMHeadTailEntry_TailPointer_of(_x) (((_x) >> 0) & 0xfff)
/*----------------------------------------------------------------
  Register: TMFlowControl_TMFlowControlEntry[16] - Reads 64-bits at a time of Traffic Manager Out-of-band Flow Control information
  Register: TMFlowControl_TMMacXoffFlowControlEntry[2] - Reads 64-bits at a time of Traffic Manager MAC XOFF Flow Control information
    Bits: 63:0  FlowControlData(ro) - Generic Read-Only status register for Out of Band Flow control and 128-bits of XOFF flow control from the MAC.
*/
#define NFP3800_NBI_TM_TMFlowControl_TMFlowControlEntry(_x)  (0x00070000 + (0x8 * ((_x) & 0xf)))
#define NFP3800_NBI_TM_TMFlowControl_TMMacXoffFlowControlEntry(_x) (0x00070080 + (0x8 * ((_x) & 0x1)))
#define   NFP3800_NBI_TM_TMFlowControl_TMFlowControlEntry_FlowControlData(_x) (((_x) & 0xffffffffffffffff) << 0)
#define   NFP3800_NBI_TM_TMFlowControl_TMFlowControlEntry_FlowControlData_of(_x) (((_x) >> 0) & 0xffffffffffffffff)
/*----------------------------------------------------------------
  Register: TMFlowControl_TmMiniPktFlowControlEntry[2] - Reads 64-bits at a time of Traffic Manager Minipacket Flow Control information
    Bits: 63:0  TmMiniPktFlowControlEntry(rc) - Generic Clear-on-read status register for Minipkt Channel flow control from the MAC.
*/
#define NFP3800_NBI_TM_TMFlowControl_TmMiniPktFlowControlEntry(_x) (0x00070090 + (0x8 * ((_x) & 0x1)))
#define   NFP3800_NBI_TM_TMFlowControl_TmMiniPktFlowControlEntry_TmMiniPktFlowControlEntry(_x) (((_x) & 0xffffffffffffffff) << 0)
#define   NFP3800_NBI_TM_TMFlowControl_TmMiniPktFlowControlEntry_TmMiniPktFlowControlEntry_of(_x) (((_x) >> 0) & 0xffffffffffffffff)
/*----------------------------------------------------------------
  Register: TMBLQRead_BLQRead[4] - Reads a pair of entries from Buffer List Queue
    Bits: 60:32 MuAddrHi(ro) - Pointer to the memory buffer. Refer to NbiBDSramEntry register description.
    Bits: 28:0  MuAddrLo(ro) - Pointer to the memory buffer. Refer to NbiBDSramEntry register description.
*/
#define NFP3800_NBI_TM_TMBLQRead_BLQRead(_x)                 (0x00080000 + (0x80 * ((_x) & 0x3)))
#define   NFP3800_NBI_TM_TMBLQRead_BLQRead_MuAddrHi(_x)      (((_x) & 0x1fffffff) << 32)
#define   NFP3800_NBI_TM_TMBLQRead_BLQRead_MuAddrHi_of(_x)   (((_x) >> 32) & 0x1fffffff)
#define   NFP3800_NBI_TM_TMBLQRead_BLQRead_MuAddrLo(_x)      (((_x) & 0x1fffffff) << 0)
#define   NFP3800_NBI_TM_TMBLQRead_BLQRead_MuAddrLo_of(_x)   (((_x) >> 0) & 0x1fffffff)
/*----------------------------------------------------------------
  Register: TMFreePtrSram_TMFreePtrSramEntry[4096] - NBI TM FreePtr SRAM Entry
    Bits: 11:0  FreePtrData(rw) - 13-bits of freeptr, initialize memory with available freeptr's
*/
#define NFP3800_NBI_TM_TMFreePtrSram_TMFreePtrSramEntry(_x)  (0x000a0000 + (0x8 * ((_x) & 0xfff)))
#define   NFP3800_NBI_TM_TMFreePtrSram_TMFreePtrSramEntry_FreePtrData(_x) (((_x) & 0xfff) << 0)
#define   NFP3800_NBI_TM_TMFreePtrSram_TMFreePtrSramEntry_FreePtrData_of(_x) (((_x) >> 0) & 0xfff)
/*----------------------------------------------------------------
  Register: TMReorderBuf_TMReorderBufEntry[8192] - NBI TM Reorder Buffer Entry
    Bits: 38:0  TmReorderBufData(rw) - 39-bits of the 78-bits stored in the TM Reorder Buffer SRAM. Refer to the details on bit positions for Reorder Buffer entry table.
*/
#define NFP3800_NBI_TM_TMReorderBuf_TMReorderBufEntry(_x)    (0x000d0000 + (0x8 * ((_x) & 0x1fff)))
#define   NFP3800_NBI_TM_TMReorderBuf_TMReorderBufEntry_TmReorderBufData(_x) (((_x) & 0x7fffffffff) << 0)
#define   NFP3800_NBI_TM_TMReorderBuf_TMReorderBufEntry_TmReorderBufData_of(_x) (((_x) >> 0) & 0x7fffffffff)
/*----------------------------------------------------------------
  Register: TMQueueLevelSram_TMQueueLevelEntry[1024] - NBI TM Queue Level SRAM Entry
    Bits: 14    QueueFullFlag(ro) - Queue full flag
    Bits: 12:0  QueueLevel(rw) - 13-bit Queue Level
*/
#define NFP3800_NBI_TM_TMQueueLevelSram_TMQueueLevelEntry(_x) (0x000e0000 + (0x8 * ((_x) & 0x3ff)))
#define   NFP3800_NBI_TM_TMQueueLevelSram_TMQueueLevelEntry_QueueFullFlag BIT(14)
#define   NFP3800_NBI_TM_TMQueueLevelSram_TMQueueLevelEntry_QueueLevel(_x) (((_x) & 0x1fff) << 0)
#define   NFP3800_NBI_TM_TMQueueLevelSram_TMQueueLevelEntry_QueueLevel_of(_x) (((_x) >> 0) & 0x1fff)
/*----------------------------------------------------------------
  Register: TrafficManagerConfig - Traffic manager configuration register. Contains global read/write configuration information.
    Bits: 29    ShaperReduceThreeFourthEnable(rw) - This bit enables Three Fourth Threshold on Shaper and its only in effect if ShaperReduceRateEnable is set.
    Bits: 28    ShaperReduceRateEnable(rw) - This bit enables Reduce rate Shaper threshold
    Bits: 27    FreePktVSAddressBitSet(rw) - In case of Drop sending a FreePkt command vs Sending a PCIE Push command with Address[31] set
    Bits: 26:25 UseTwoPullIDBuses(rw) - Enables using of both the PullID buses for CTM packets.
    Bits: 24    FlowControlEventEna(rw) - Enables Flow Control Event notifications to the Software.
    Bits: 23    PktReadyC0Enable(rw) - Eanbles decoding of previously unused bits in the Packet Ready Command for new C0 functions.
    Bits: 22    TmFreePtrNoReturn(rw) - Enable mode for Multicast Packets stream. Also, used in FreePacket Command when a Multicast Packet is dropped when the queues are full to notify PE to not return the MuPtr.
    Bits: 20    StartLinkedListInit(rw) - It triggers the initialization of linked list FreePtr Ram and Head and Tail Pointer Ram.
    Bits: 17    PacketDescriptorLinkedListEna(rw) - Enables the linked list approach to using Packet Descriptor Memory.
    Bits: 16    OOBFCEnable(rw) - MAC Out of Band Flow Control Enable. Enable the Out of Band Flow Control from the MAC external interface to shut off the 1024 level 2 scheduler ports.
    Bits: 15:13 L0ChannelMap(rw) - MAC channel number to Level 0 Scheduler port number mapping register. This mapping register is valid only when L1InputSelect = 1 and ChannelLevelSelect = 0. In this configuration there are up to 32 L0 Scheduler ports used. One MAC channel may map to multiple L0 Scheduler ports, and an offset of 64 can be used to support multiple Hydras.
    Bits:  9    ErrPoisonEna(rw) - It enables the poisoning of data and/or enables the other error porpagation mechanisms in case of errors.
    Bits:  8    SchedulerEnable(rw) - Scheduler Enable bit. Set this bit to enable the scheduling of traffic, clear to disable.
    Bits:  7    ShaperEnable(rw) - Shaper Enable bit. Enables the Rate Shaper to start rate limiting traffic based on the configured values. The default is off so that the shaper is disabled prior to configuration.
    Bits:  6    MiniPacketFCEnable(rw) - Minipacket Credit based Flow Control enable bit. Clear this bit to disable minipacket flow control. This bit is set to enable flow control by default.
    Bits:  5    ChannelLevelSelect(rw) - Channel level select bit. Select the definition of the MAC channel number and the MAC XOFF Flow control to be either the 128 level 0 scheduler ports, or 128 ports on the first 16 level 1 schedulers (or level 2 schedulers depending on the value of L1InputSelect). Note: When L1InputSelect=0 and ChannelLevelSelect=1, only 128 queues are allowed to be active in this configuration.
    Bits:  3:1  NumSequencers(rw) - Number of active sequencers. Specifies the number of active sequencers in the traffic manager.
    Bits:  0    L1InputSelect(rw) - Select the operation of the Level 1 schedulers. This bit disables or enables the Level 1 schedulers, and allows for two or three levels of scheduling in the Traffic Manager. When disabled, the Level 2 scheduler outputs are connected directly to the Level 0 scheduler inputs. When enabled, Level 2 outputs connect to Level 1 inputs, and Level 1 outputs connects to Level 0 inputs
*/
#define NFP3800_NBI_TMX_CSR_TrafficManagerConfig             0x00000000
#define   NFP3800_NBI_TMX_CSR_TrafficManagerConfig_ShaperReduceThreeFourthEnable BIT(29)
#define   NFP3800_NBI_TMX_CSR_TrafficManagerConfig_ShaperReduceRateEnable BIT(28)
#define   NFP3800_NBI_TMX_CSR_TrafficManagerConfig_FreePktVSAddressBitSet BIT(27)
#define     NFP3800_NBI_TMX_CSR_TrafficManagerConfig_FreePktVSAddressBitSet_0 (0 << 27)
#define     NFP3800_NBI_TMX_CSR_TrafficManagerConfig_FreePktVSAddressBitSet_1 BIT(27)
#define   NFP3800_NBI_TMX_CSR_TrafficManagerConfig_UseTwoPullIDBuses(_x) (((_x) & 0x3) << 25)
#define   NFP3800_NBI_TMX_CSR_TrafficManagerConfig_UseTwoPullIDBuses_of(_x) (((_x) >> 25) & 0x3)
#define     NFP3800_NBI_TMX_CSR_TrafficManagerConfig_UseTwoPullIDBuses_0 (0)
#define     NFP3800_NBI_TMX_CSR_TrafficManagerConfig_UseTwoPullIDBuses_1 (1)
#define     NFP3800_NBI_TMX_CSR_TrafficManagerConfig_UseTwoPullIDBuses_2 (2)
#define     NFP3800_NBI_TMX_CSR_TrafficManagerConfig_UseTwoPullIDBuses_3 (3)
#define   NFP3800_NBI_TMX_CSR_TrafficManagerConfig_FlowControlEventEna BIT(24)
#define   NFP3800_NBI_TMX_CSR_TrafficManagerConfig_PktReadyC0Enable BIT(23)
#define   NFP3800_NBI_TMX_CSR_TrafficManagerConfig_TmFreePtrNoReturn BIT(22)
#define   NFP3800_NBI_TMX_CSR_TrafficManagerConfig_StartLinkedListInit BIT(20)
#define   NFP3800_NBI_TMX_CSR_TrafficManagerConfig_PacketDescriptorLinkedListEna BIT(17)
#define     NFP3800_NBI_TMX_CSR_TrafficManagerConfig_PacketDescriptorLinkedListEna_Disable (0 << 17)
#define     NFP3800_NBI_TMX_CSR_TrafficManagerConfig_PacketDescriptorLinkedListEna_Enable BIT(17)
#define   NFP3800_NBI_TMX_CSR_TrafficManagerConfig_OOBFCEnable BIT(16)
#define     NFP3800_NBI_TMX_CSR_TrafficManagerConfig_OOBFCEnable_Disable (0 << 16)
#define     NFP3800_NBI_TMX_CSR_TrafficManagerConfig_OOBFCEnable_Enable BIT(16)
#define   NFP3800_NBI_TMX_CSR_TrafficManagerConfig_L0ChannelMap(_x) (((_x) & 0x7) << 13)
#define   NFP3800_NBI_TMX_CSR_TrafficManagerConfig_L0ChannelMap_of(_x) (((_x) >> 13) & 0x7)
#define     NFP3800_NBI_TMX_CSR_TrafficManagerConfig_L0ChannelMap_0 (0)
#define     NFP3800_NBI_TMX_CSR_TrafficManagerConfig_L0ChannelMap_1 (1)
#define     NFP3800_NBI_TMX_CSR_TrafficManagerConfig_L0ChannelMap_2 (2)
#define     NFP3800_NBI_TMX_CSR_TrafficManagerConfig_L0ChannelMap_3 (3)
#define     NFP3800_NBI_TMX_CSR_TrafficManagerConfig_L0ChannelMap_4 (4)
#define     NFP3800_NBI_TMX_CSR_TrafficManagerConfig_L0ChannelMap_5 (5)
#define     NFP3800_NBI_TMX_CSR_TrafficManagerConfig_L0ChannelMap_6 (6)
#define     NFP3800_NBI_TMX_CSR_TrafficManagerConfig_L0ChannelMap_7 (7)
#define   NFP3800_NBI_TMX_CSR_TrafficManagerConfig_ErrPoisonEna BIT(9)
#define     NFP3800_NBI_TMX_CSR_TrafficManagerConfig_ErrPoisonEna_Disable (0 << 9)
#define     NFP3800_NBI_TMX_CSR_TrafficManagerConfig_ErrPoisonEna_Enable BIT(9)
#define   NFP3800_NBI_TMX_CSR_TrafficManagerConfig_SchedulerEnable BIT(8)
#define     NFP3800_NBI_TMX_CSR_TrafficManagerConfig_SchedulerEnable_Disable (0 << 8)
#define     NFP3800_NBI_TMX_CSR_TrafficManagerConfig_SchedulerEnable_Enable BIT(8)
#define   NFP3800_NBI_TMX_CSR_TrafficManagerConfig_ShaperEnable BIT(7)
#define     NFP3800_NBI_TMX_CSR_TrafficManagerConfig_ShaperEnable_Disable (0 << 7)
#define     NFP3800_NBI_TMX_CSR_TrafficManagerConfig_ShaperEnable_Enable BIT(7)
#define   NFP3800_NBI_TMX_CSR_TrafficManagerConfig_MiniPacketFCEnable BIT(6)
#define     NFP3800_NBI_TMX_CSR_TrafficManagerConfig_MiniPacketFCEnable_Disable (0 << 6)
#define     NFP3800_NBI_TMX_CSR_TrafficManagerConfig_MiniPacketFCEnable_Enable BIT(6)
#define   NFP3800_NBI_TMX_CSR_TrafficManagerConfig_ChannelLevelSelect BIT(5)
#define     NFP3800_NBI_TMX_CSR_TrafficManagerConfig_ChannelLevelSelect_Level_0 (0 << 5)
#define     NFP3800_NBI_TMX_CSR_TrafficManagerConfig_ChannelLevelSelect_Level_1 BIT(5)
#define   NFP3800_NBI_TMX_CSR_TrafficManagerConfig_NumSequencers(_x) (((_x) & 0x7) << 1)
#define   NFP3800_NBI_TMX_CSR_TrafficManagerConfig_NumSequencers_of(_x) (((_x) >> 1) & 0x7)
#define     NFP3800_NBI_TMX_CSR_TrafficManagerConfig_NumSequencers_0 (0)
#define     NFP3800_NBI_TMX_CSR_TrafficManagerConfig_NumSequencers_1 (1)
#define     NFP3800_NBI_TMX_CSR_TrafficManagerConfig_NumSequencers_2 (2)
#define     NFP3800_NBI_TMX_CSR_TrafficManagerConfig_NumSequencers_4 (3)
#define     NFP3800_NBI_TMX_CSR_TrafficManagerConfig_NumSequencers_8 (4)
#define     NFP3800_NBI_TMX_CSR_TrafficManagerConfig_NumSequencers_16 (5)
#define     NFP3800_NBI_TMX_CSR_TrafficManagerConfig_NumSequencers_NA (6)
#define   NFP3800_NBI_TMX_CSR_TrafficManagerConfig_L1InputSelect BIT(0)
#define     NFP3800_NBI_TMX_CSR_TrafficManagerConfig_L1InputSelect_L1_Disable (0 << 0)
#define     NFP3800_NBI_TMX_CSR_TrafficManagerConfig_L1InputSelect_L1_Enable BIT(0)
/*----------------------------------------------------------------
  Register: DropRate - Configurable drop rate that is programmable for each queue depth range
    Bits: 23:21 DropRateRangeB3(rw) - Drop Rate for Queue Depth Range B-3. Top 75% to 100% of the descriptor queue. Packets will be dropped n% of the time if the queue depth is within this range
    Bits: 20:18 DropRateRangeB2(rw) - Drop Rate for Queue Depth Range B-2. 50% to 74% full. Packets will be dropped n% of the time if the queue depth is within this range
    Bits: 17:15 DropRateRangeB1(rw) - Drop Rate for Queue Depth Range B-1. 25% to 49% full. Packets will be dropped n% of the time if the queue depth is within this range
    Bits: 14:12 DropRateRangeB0(rw) - Drop Rate for Queue Depth Range B-0. Bottom 0 to 24% of the descriptor queue. Packets will be dropped n% of the time if the queue depth is within this range
    Bits: 11:9  DropRateRangeA3(rw) - Drop Rate for Queue Depth Range A-3. Top 75% to 100% of the descriptor queue. Packets will be dropped n% of the time if the queue depth is within this range
    Bits:  8:6  DropRateRangeA2(rw) - Drop Rate for Queue Depth Range A-2. 50% to 74% full. Packets will be dropped n% of the time if the queue depth is within this range
    Bits:  5:3  DropRateRangeA1(rw) - Drop Rate for Queue Depth Range A-1. 25% to 49% full. Packets will be dropped n% of the time if the queue depth is within this range
    Bits:  2:0  DropRateRangeA0(rw) - Drop Rate for Queue Depth Range A-0. Bottom 0 to 24% of the descriptor queue. Packets will be dropped n% of the time if the queue depth is within this range
*/
#define NFP3800_NBI_TMX_CSR_DropRate                         0x00000004
#define   NFP3800_NBI_TMX_CSR_DropRate_DropRateRangeB3(_x)   (((_x) & 0x7) << 21)
#define   NFP3800_NBI_TMX_CSR_DropRate_DropRateRangeB3_of(_x) (((_x) >> 21) & 0x7)
#define     NFP3800_NBI_TMX_CSR_DropRate_DropRateRangeB3_0   (0)
#define     NFP3800_NBI_TMX_CSR_DropRate_DropRateRangeB3_12_5 (1)
#define     NFP3800_NBI_TMX_CSR_DropRate_DropRateRangeB3_25  (2)
#define     NFP3800_NBI_TMX_CSR_DropRate_DropRateRangeB3_37_5 (3)
#define     NFP3800_NBI_TMX_CSR_DropRate_DropRateRangeB3_50  (4)
#define     NFP3800_NBI_TMX_CSR_DropRate_DropRateRangeB3_62_5 (5)
#define     NFP3800_NBI_TMX_CSR_DropRate_DropRateRangeB3_75  (6)
#define     NFP3800_NBI_TMX_CSR_DropRate_DropRateRangeB3_87_5 (7)
#define   NFP3800_NBI_TMX_CSR_DropRate_DropRateRangeB2(_x)   (((_x) & 0x7) << 18)
#define   NFP3800_NBI_TMX_CSR_DropRate_DropRateRangeB2_of(_x) (((_x) >> 18) & 0x7)
#define     NFP3800_NBI_TMX_CSR_DropRate_DropRateRangeB2_0   (0)
#define     NFP3800_NBI_TMX_CSR_DropRate_DropRateRangeB2_12_5 (1)
#define     NFP3800_NBI_TMX_CSR_DropRate_DropRateRangeB2_25  (2)
#define     NFP3800_NBI_TMX_CSR_DropRate_DropRateRangeB2_37_5 (3)
#define     NFP3800_NBI_TMX_CSR_DropRate_DropRateRangeB2_50  (4)
#define     NFP3800_NBI_TMX_CSR_DropRate_DropRateRangeB2_62_5 (5)
#define     NFP3800_NBI_TMX_CSR_DropRate_DropRateRangeB2_75  (6)
#define     NFP3800_NBI_TMX_CSR_DropRate_DropRateRangeB2_87_5 (7)
#define   NFP3800_NBI_TMX_CSR_DropRate_DropRateRangeB1(_x)   (((_x) & 0x7) << 15)
#define   NFP3800_NBI_TMX_CSR_DropRate_DropRateRangeB1_of(_x) (((_x) >> 15) & 0x7)
#define     NFP3800_NBI_TMX_CSR_DropRate_DropRateRangeB1_0   (0)
#define     NFP3800_NBI_TMX_CSR_DropRate_DropRateRangeB1_12_5 (1)
#define     NFP3800_NBI_TMX_CSR_DropRate_DropRateRangeB1_25  (2)
#define     NFP3800_NBI_TMX_CSR_DropRate_DropRateRangeB1_37_5 (3)
#define     NFP3800_NBI_TMX_CSR_DropRate_DropRateRangeB1_50  (4)
#define     NFP3800_NBI_TMX_CSR_DropRate_DropRateRangeB1_62_5 (5)
#define     NFP3800_NBI_TMX_CSR_DropRate_DropRateRangeB1_75  (6)
#define     NFP3800_NBI_TMX_CSR_DropRate_DropRateRangeB1_87_5 (7)
#define   NFP3800_NBI_TMX_CSR_DropRate_DropRateRangeB0(_x)   (((_x) & 0x7) << 12)
#define   NFP3800_NBI_TMX_CSR_DropRate_DropRateRangeB0_of(_x) (((_x) >> 12) & 0x7)
#define     NFP3800_NBI_TMX_CSR_DropRate_DropRateRangeB0_0   (0)
#define     NFP3800_NBI_TMX_CSR_DropRate_DropRateRangeB0_12_5 (1)
#define     NFP3800_NBI_TMX_CSR_DropRate_DropRateRangeB0_25  (2)
#define     NFP3800_NBI_TMX_CSR_DropRate_DropRateRangeB0_37_5 (3)
#define     NFP3800_NBI_TMX_CSR_DropRate_DropRateRangeB0_50  (4)
#define     NFP3800_NBI_TMX_CSR_DropRate_DropRateRangeB0_62_5 (5)
#define     NFP3800_NBI_TMX_CSR_DropRate_DropRateRangeB0_75  (6)
#define     NFP3800_NBI_TMX_CSR_DropRate_DropRateRangeB0_87_5 (7)
#define   NFP3800_NBI_TMX_CSR_DropRate_DropRateRangeA3(_x)   (((_x) & 0x7) << 9)
#define   NFP3800_NBI_TMX_CSR_DropRate_DropRateRangeA3_of(_x) (((_x) >> 9) & 0x7)
#define     NFP3800_NBI_TMX_CSR_DropRate_DropRateRangeA3_0   (0)
#define     NFP3800_NBI_TMX_CSR_DropRate_DropRateRangeA3_12_5 (1)
#define     NFP3800_NBI_TMX_CSR_DropRate_DropRateRangeA3_25  (2)
#define     NFP3800_NBI_TMX_CSR_DropRate_DropRateRangeA3_37_5 (3)
#define     NFP3800_NBI_TMX_CSR_DropRate_DropRateRangeA3_50  (4)
#define     NFP3800_NBI_TMX_CSR_DropRate_DropRateRangeA3_62_5 (5)
#define     NFP3800_NBI_TMX_CSR_DropRate_DropRateRangeA3_75  (6)
#define     NFP3800_NBI_TMX_CSR_DropRate_DropRateRangeA3_87_5 (7)
#define   NFP3800_NBI_TMX_CSR_DropRate_DropRateRangeA2(_x)   (((_x) & 0x7) << 6)
#define   NFP3800_NBI_TMX_CSR_DropRate_DropRateRangeA2_of(_x) (((_x) >> 6) & 0x7)
#define     NFP3800_NBI_TMX_CSR_DropRate_DropRateRangeA2_0   (0)
#define     NFP3800_NBI_TMX_CSR_DropRate_DropRateRangeA2_12_5 (1)
#define     NFP3800_NBI_TMX_CSR_DropRate_DropRateRangeA2_25  (2)
#define     NFP3800_NBI_TMX_CSR_DropRate_DropRateRangeA2_37_5 (3)
#define     NFP3800_NBI_TMX_CSR_DropRate_DropRateRangeA2_50  (4)
#define     NFP3800_NBI_TMX_CSR_DropRate_DropRateRangeA2_62_5 (5)
#define     NFP3800_NBI_TMX_CSR_DropRate_DropRateRangeA2_75  (6)
#define     NFP3800_NBI_TMX_CSR_DropRate_DropRateRangeA2_87_5 (7)
#define   NFP3800_NBI_TMX_CSR_DropRate_DropRateRangeA1(_x)   (((_x) & 0x7) << 3)
#define   NFP3800_NBI_TMX_CSR_DropRate_DropRateRangeA1_of(_x) (((_x) >> 3) & 0x7)
#define     NFP3800_NBI_TMX_CSR_DropRate_DropRateRangeA1_0   (0)
#define     NFP3800_NBI_TMX_CSR_DropRate_DropRateRangeA1_12_5 (1)
#define     NFP3800_NBI_TMX_CSR_DropRate_DropRateRangeA1_25  (2)
#define     NFP3800_NBI_TMX_CSR_DropRate_DropRateRangeA1_37_5 (3)
#define     NFP3800_NBI_TMX_CSR_DropRate_DropRateRangeA1_50  (4)
#define     NFP3800_NBI_TMX_CSR_DropRate_DropRateRangeA1_62_5 (5)
#define     NFP3800_NBI_TMX_CSR_DropRate_DropRateRangeA1_75  (6)
#define     NFP3800_NBI_TMX_CSR_DropRate_DropRateRangeA1_87_5 (7)
#define   NFP3800_NBI_TMX_CSR_DropRate_DropRateRangeA0(_x)   (((_x) & 0x7) << 0)
#define   NFP3800_NBI_TMX_CSR_DropRate_DropRateRangeA0_of(_x) (((_x) >> 0) & 0x7)
#define     NFP3800_NBI_TMX_CSR_DropRate_DropRateRangeA0_0   (0)
#define     NFP3800_NBI_TMX_CSR_DropRate_DropRateRangeA0_12_5 (1)
#define     NFP3800_NBI_TMX_CSR_DropRate_DropRateRangeA0_25  (2)
#define     NFP3800_NBI_TMX_CSR_DropRate_DropRateRangeA0_37_5 (3)
#define     NFP3800_NBI_TMX_CSR_DropRate_DropRateRangeA0_50  (4)
#define     NFP3800_NBI_TMX_CSR_DropRate_DropRateRangeA0_62_5 (5)
#define     NFP3800_NBI_TMX_CSR_DropRate_DropRateRangeA0_75  (6)
#define     NFP3800_NBI_TMX_CSR_DropRate_DropRateRangeA0_87_5 (7)
/*----------------------------------------------------------------
  Register: BlqEvent - Configures thresholds, event type and enables/disables for the buffer list queues
    Bits: 23    ReducedBLQThreshold(rw) - Reduced Thresholds would be used when this bit is set.
    Bits: 22    BypassBLQPtrReturn(rw) - When set to '1', drops Null MU Pointers (29h0) instead of returning them to the BLQ.
    Bits: 20    DisBLQEventFifoFullAssert(rw) - Set this bit to disable the BLQ Event FIFO overflow assertion
    Bits: 19:18 BLQEventThreshold3(rw) - An event is generated every N buffer descriptors written to buffer list queue3, where N is configurable from 16 to 128 or 4 to 32 if Reduced BLQ Threshold is set..
    Bits: 17:16 BLQEventThreshold2(rw) - An event is generated every N buffer descriptors written to buffer list queue3, where N is configurable from 16 to 128 or 4 to 32 if Reduced BLQ Threshold is set..
    Bits: 15:14 BLQEventThreshold1(rw) - An event is generated every N buffer descriptors written to buffer list queue3, where N is configurable from 16 to 128 or 4 to 32 if Reduced BLQ Threshold is set..
    Bits: 13:12 BLQEventThreshold0(rw) - An event is generated every N buffer descriptors written to buffer list queue3, where N is configurable from 16 to 128 or 4 to 32 if Reduced BLQ Threshold is set..
    Bits:  7:4  BLQEventNotEmptyEnable(rw) - One bit per buffer list queue. Enables the generation of BLQ Events when the queue transitions from empty to not empty. Disabled when clear. The event type value used is 0x0 (event_type_fifo_not_empty)
    Bits:  3:0  BLQEventStatusEnable(rw) - One bit per buffer list queue. Enables BLQ Status Events when set. Disabled when clear. A Status Event is sent when the number of entries written to the queue reaches the programmed value in the BLQEventThreshold register. The event type value used is 0x5 (event_type_value_update)
*/
#define NFP3800_NBI_TMX_CSR_BlqEvent                         0x00000008
#define   NFP3800_NBI_TMX_CSR_BlqEvent_ReducedBLQThreshold   BIT(23)
#define   NFP3800_NBI_TMX_CSR_BlqEvent_BypassBLQPtrReturn    BIT(22)
#define   NFP3800_NBI_TMX_CSR_BlqEvent_DisBLQEventFifoFullAssert BIT(20)
#define   NFP3800_NBI_TMX_CSR_BlqEvent_BLQEventThreshold3(_x) (((_x) & 0x3) << 18)
#define   NFP3800_NBI_TMX_CSR_BlqEvent_BLQEventThreshold3_of(_x) (((_x) >> 18) & 0x3)
#define     NFP3800_NBI_TMX_CSR_BlqEvent_BLQEventThreshold3_16_or_4 (0)
#define     NFP3800_NBI_TMX_CSR_BlqEvent_BLQEventThreshold3_32_or_8 (1)
#define     NFP3800_NBI_TMX_CSR_BlqEvent_BLQEventThreshold3_64_or_16 (2)
#define     NFP3800_NBI_TMX_CSR_BlqEvent_BLQEventThreshold3_128_or_32 (3)
#define   NFP3800_NBI_TMX_CSR_BlqEvent_BLQEventThreshold2(_x) (((_x) & 0x3) << 16)
#define   NFP3800_NBI_TMX_CSR_BlqEvent_BLQEventThreshold2_of(_x) (((_x) >> 16) & 0x3)
#define     NFP3800_NBI_TMX_CSR_BlqEvent_BLQEventThreshold2_16_or_4 (0)
#define     NFP3800_NBI_TMX_CSR_BlqEvent_BLQEventThreshold2_32_or_8 (1)
#define     NFP3800_NBI_TMX_CSR_BlqEvent_BLQEventThreshold2_64_or_16 (2)
#define     NFP3800_NBI_TMX_CSR_BlqEvent_BLQEventThreshold2_128_or_32 (3)
#define   NFP3800_NBI_TMX_CSR_BlqEvent_BLQEventThreshold1(_x) (((_x) & 0x3) << 14)
#define   NFP3800_NBI_TMX_CSR_BlqEvent_BLQEventThreshold1_of(_x) (((_x) >> 14) & 0x3)
#define     NFP3800_NBI_TMX_CSR_BlqEvent_BLQEventThreshold1_16_or_4 (0)
#define     NFP3800_NBI_TMX_CSR_BlqEvent_BLQEventThreshold1_32_or_8 (1)
#define     NFP3800_NBI_TMX_CSR_BlqEvent_BLQEventThreshold1_64_or_16 (2)
#define     NFP3800_NBI_TMX_CSR_BlqEvent_BLQEventThreshold1_128_or_32 (3)
#define   NFP3800_NBI_TMX_CSR_BlqEvent_BLQEventThreshold0(_x) (((_x) & 0x3) << 12)
#define   NFP3800_NBI_TMX_CSR_BlqEvent_BLQEventThreshold0_of(_x) (((_x) >> 12) & 0x3)
#define     NFP3800_NBI_TMX_CSR_BlqEvent_BLQEventThreshold0_16_or_4 (0)
#define     NFP3800_NBI_TMX_CSR_BlqEvent_BLQEventThreshold0_32_or_8 (1)
#define     NFP3800_NBI_TMX_CSR_BlqEvent_BLQEventThreshold0_64_or_16 (2)
#define     NFP3800_NBI_TMX_CSR_BlqEvent_BLQEventThreshold0_128_or_32 (3)
#define   NFP3800_NBI_TMX_CSR_BlqEvent_BLQEventNotEmptyEnable(_x) (((_x) & 0xf) << 4)
#define   NFP3800_NBI_TMX_CSR_BlqEvent_BLQEventNotEmptyEnable_of(_x) (((_x) >> 4) & 0xf)
#define     NFP3800_NBI_TMX_CSR_BlqEvent_BLQEventNotEmptyEnable_Disable (0)
#define     NFP3800_NBI_TMX_CSR_BlqEvent_BLQEventNotEmptyEnable_Enable (1)
#define   NFP3800_NBI_TMX_CSR_BlqEvent_BLQEventStatusEnable(_x) (((_x) & 0xf) << 0)
#define   NFP3800_NBI_TMX_CSR_BlqEvent_BLQEventStatusEnable_of(_x) (((_x) >> 0) & 0xf)
#define     NFP3800_NBI_TMX_CSR_BlqEvent_BLQEventStatusEnable_Disable (0)
#define     NFP3800_NBI_TMX_CSR_BlqEvent_BLQEventStatusEnable_Enable (1)
/*----------------------------------------------------------------
  Register: EgressRateLimit - Rate limiting CSR for the Egress NBI Command/PullId ports.
    Bits:  3    EgressRateLimitOff(rw) - Turns the rate limiting off for both Ports 6 and 7 (P6 and P7).
    Bits:  2:0  EgressRate(rw) - Rate limiting control for the Command/PullId Ports 6 and 7 (P6 and P7).
*/
#define NFP3800_NBI_TMX_CSR_EgressRateLimit                  0x00000074
#define   NFP3800_NBI_TMX_CSR_EgressRateLimit_EgressRateLimitOff BIT(3)
#define     NFP3800_NBI_TMX_CSR_EgressRateLimit_EgressRateLimitOff_on (0 << 3)
#define     NFP3800_NBI_TMX_CSR_EgressRateLimit_EgressRateLimitOff_off BIT(3)
#define   NFP3800_NBI_TMX_CSR_EgressRateLimit_EgressRate(_x) (((_x) & 0x7) << 0)
#define   NFP3800_NBI_TMX_CSR_EgressRateLimit_EgressRate_of(_x) (((_x) >> 0) & 0x7)
#define     NFP3800_NBI_TMX_CSR_EgressRateLimit_EgressRate_64G (0)
#define     NFP3800_NBI_TMX_CSR_EgressRateLimit_EgressRate_32G (1)
#define     NFP3800_NBI_TMX_CSR_EgressRateLimit_EgressRate_16G (2)
#define     NFP3800_NBI_TMX_CSR_EgressRateLimit_EgressRate_8G (3)
#define     NFP3800_NBI_TMX_CSR_EgressRateLimit_EgressRate_128G (4)
#define     NFP3800_NBI_TMX_CSR_EgressRateLimit_EgressRate_112G (5)
#define     NFP3800_NBI_TMX_CSR_EgressRateLimit_EgressRate_96G (6)
#define     NFP3800_NBI_TMX_CSR_EgressRateLimit_EgressRate_80G (7)
/*----------------------------------------------------------------
  Register: EgressPullIdPortEnable - Enables ports for transmitting Pull IDs.
    Bits:  1:0  EgressPullIdPortEnable(rw) - Egress Port enable for transmitting PullIDs.
*/
#define NFP3800_NBI_TMX_CSR_EgressPullIdPortEnable           0x00000078
#define   NFP3800_NBI_TMX_CSR_EgressPullIdPortEnable_EgressPullIdPortEnable(_x) (((_x) & 0x3) << 0)
#define   NFP3800_NBI_TMX_CSR_EgressPullIdPortEnable_EgressPullIdPortEnable_of(_x) (((_x) >> 0) & 0x3)
#define     NFP3800_NBI_TMX_CSR_EgressPullIdPortEnable_EgressPullIdPortEnable_P6_and_P7 (0)
#define     NFP3800_NBI_TMX_CSR_EgressPullIdPortEnable_EgressPullIdPortEnable_P6 (1)
#define     NFP3800_NBI_TMX_CSR_EgressPullIdPortEnable_EgressPullIdPortEnable_P7 (2)
#define     NFP3800_NBI_TMX_CSR_EgressPullIdPortEnable_EgressPullIdPortEnable_unused (3)
/*----------------------------------------------------------------
  Register: OutOfOrderCount[16] - Out of order counter for each sequencer. There are a total of 32 out of order counters in the traffic manager, one for each active sequencer.
    Bits: 15:0  OutOfOrderCount(ro) - Out-of-order counter to keep track of the number of packets sent out of order for this sequencer. The counter will saturate when the maximum number is reached.
*/
#define NFP3800_NBI_TMX_CSR_OutOfOrderCount(_x)              (0x00000100 + (0x4 * ((_x) & 0xf)))
#define   NFP3800_NBI_TMX_CSR_OutOfOrderCount_OutOfOrderCount(_x) (((_x) & 0xffff) << 0)
#define   NFP3800_NBI_TMX_CSR_OutOfOrderCount_OutOfOrderCount_of(_x) (((_x) >> 0) & 0xffff)
/*----------------------------------------------------------------
  Register: OutOfOrderCountClear[16] - Clear on read out of order counter for each sequencer. One for each active sequencer.
    Bits: 15:0  OutOfOrderCountClear(rc) - Clear on read out-of-order counter to keep track of the number of packets sent out of order for this sequencer. The counter will saturate when the maximum number is reached. The read value for this register is identical to the OUTOFORDERCOUNT register value, however when this register is read the out-of-order counter is cleared.
*/
#define NFP3800_NBI_TMX_CSR_OutOfOrderCountClear(_x)         (0x00000200 + (0x4 * ((_x) & 0xf)))
#define   NFP3800_NBI_TMX_CSR_OutOfOrderCountClear_OutOfOrderCountClear(_x) (((_x) & 0xffff) << 0)
#define   NFP3800_NBI_TMX_CSR_OutOfOrderCountClear_OutOfOrderCountClear_of(_x) (((_x) >> 0) & 0xffff)
/*----------------------------------------------------------------
  Register: MiniPktCreditConfig - MiniPacket Credit Flow Control Configuration Register
    Bits: 25    PerChannelCreditEnable(rw) - Enables per channel credit limit defined by ChannelCreditLimit.
    Bits: 24    MiniPacketFCMode(rw) - MiniPacket Credit Flow Control Mode bit. Select the free pool crediting mode to use.
    Bits: 21:12 FPCreditLimit(rw) - MiniPacket Transmit Free Pool Credit Counter Limit. Programmable limit for both of the free pool credit counters. The default value of 512 credits should be used for most applications using one free pool. The free pool has a reserve of 40 credits and should always be programmed to a value greater than 40.
    Bits:  9:0  CreditLimit(rw) - MiniPacket Transmit Channel Credit Counter Limit. Programmable limit for all of the channel credit counters
*/
#define NFP3800_NBI_TMX_CSR_MiniPktCreditConfig              0x00000300
#define   NFP3800_NBI_TMX_CSR_MiniPktCreditConfig_PerChannelCreditEnable BIT(25)
#define     NFP3800_NBI_TMX_CSR_MiniPktCreditConfig_PerChannelCreditEnable_Disable (0 << 25)
#define     NFP3800_NBI_TMX_CSR_MiniPktCreditConfig_PerChannelCreditEnable_Enable BIT(25)
#define   NFP3800_NBI_TMX_CSR_MiniPktCreditConfig_MiniPacketFCMode BIT(24)
#define     NFP3800_NBI_TMX_CSR_MiniPktCreditConfig_MiniPacketFCMode_Mode_0 (0 << 24)
#define     NFP3800_NBI_TMX_CSR_MiniPktCreditConfig_MiniPacketFCMode_Mode_1 BIT(24)
#define   NFP3800_NBI_TMX_CSR_MiniPktCreditConfig_FPCreditLimit(_x) (((_x) & 0x3ff) << 12)
#define   NFP3800_NBI_TMX_CSR_MiniPktCreditConfig_FPCreditLimit_of(_x) (((_x) >> 12) & 0x3ff)
#define   NFP3800_NBI_TMX_CSR_MiniPktCreditConfig_CreditLimit(_x) (((_x) & 0x3ff) << 0)
#define   NFP3800_NBI_TMX_CSR_MiniPktCreditConfig_CreditLimit_of(_x) (((_x) >> 0) & 0x3ff)
/*----------------------------------------------------------------
  Register: MiniPktFreePoolCredit[2] - MiniPacket Transmit Free Pool Credit Counter. Read only free pool credit counters
    Bits:  9:0  FPCreditCounter(ro) - MiniPacket Transmit Free Pool Credit Counter. Read only counter of the free pool credits remaining
*/
#define NFP3800_NBI_TMX_CSR_MiniPktFreePoolCredit(_x)        (0x00000304 + (0x4 * ((_x) & 0x1)))
#define   NFP3800_NBI_TMX_CSR_MiniPktFreePoolCredit_FPCreditCounter(_x) (((_x) & 0x3ff) << 0)
#define   NFP3800_NBI_TMX_CSR_MiniPktFreePoolCredit_FPCreditCounter_of(_x) (((_x) >> 0) & 0x3ff)
/*----------------------------------------------------------------
  Register: MiniPktChannelCredit[128] - MiniPacket Transmit Channel Credit Counter. Read only credit counter, one for each channel
    Bits:  9:0  CreditCounter(ro) - MiniPacket Transmit Channel Credit Counter. Read only counter of the channel credits remaining
*/
#define NFP3800_NBI_TMX_CSR_MiniPktChannelCredit(_x)         (0x00000400 + (0x4 * ((_x) & 0x7f)))
#define   NFP3800_NBI_TMX_CSR_MiniPktChannelCredit_CreditCounter(_x) (((_x) & 0x3ff) << 0)
#define   NFP3800_NBI_TMX_CSR_MiniPktChannelCredit_CreditCounter_of(_x) (((_x) >> 0) & 0x3ff)
/*----------------------------------------------------------------
  Register: ReorderActivity - Activity indication flag for each sequencer in the reorder buffer.
    Bits: 15:0  ReorderActivity(ro) - Clear on read activity flag for each sequencer in the reorder buffer. One bit for each active sequencer in the reorder buffer.
*/
#define NFP3800_NBI_TMX_CSR_ReorderActivity                  0x00000600
#define   NFP3800_NBI_TMX_CSR_ReorderActivity_ReorderActivity(_x) (((_x) & 0xffff) << 0)
#define   NFP3800_NBI_TMX_CSR_ReorderActivity_ReorderActivity_of(_x) (((_x) >> 0) & 0xffff)
/*----------------------------------------------------------------
  Register: DropProfile[8] - 8 programmable Drop Profiles corresponding to drop precedence values 0-7
    Bits: 11:9  DropRateRange3(rw) - Drop Rate Queue Depth Range 3. Top 75% to 100% of the descriptor queue. Packets will be dropped n% of the time if the queue depth is within this range
    Bits:  8:6  DropRateRange2(rw) - Drop Rate for Queue Depth Range 2. 50% to 74% full. Packets will be dropped n% of the time if the queue depth is within this range
    Bits:  5:3  DropRateRange1(rw) - Drop Rate for Queue Depth Range 1. 25% to 49% full. Packets will be dropped n% of the time if the queue depth is within this range
    Bits:  2:0  DropRateRange0(rw) - Drop Rate for Queue Depth Range 0. Bottom 0 to 24% of the descriptor queue. Packets will be dropped n% of the time if the queue depth is within this range
*/
#define NFP3800_NBI_TMX_CSR_DropProfile(_x)                  (0x00000604 + (0x4 * ((_x) & 0x7)))
#define   NFP3800_NBI_TMX_CSR_DropProfile_DropRateRange3(_x) (((_x) & 0x7) << 9)
#define   NFP3800_NBI_TMX_CSR_DropProfile_DropRateRange3_of(_x) (((_x) >> 9) & 0x7)
#define     NFP3800_NBI_TMX_CSR_DropProfile_DropRateRange3_0 (0)
#define     NFP3800_NBI_TMX_CSR_DropProfile_DropRateRange3_12_5 (1)
#define     NFP3800_NBI_TMX_CSR_DropProfile_DropRateRange3_25 (2)
#define     NFP3800_NBI_TMX_CSR_DropProfile_DropRateRange3_37_5 (3)
#define     NFP3800_NBI_TMX_CSR_DropProfile_DropRateRange3_50 (4)
#define     NFP3800_NBI_TMX_CSR_DropProfile_DropRateRange3_62_5 (5)
#define     NFP3800_NBI_TMX_CSR_DropProfile_DropRateRange3_75 (6)
#define     NFP3800_NBI_TMX_CSR_DropProfile_DropRateRange3_87_5 (7)
#define   NFP3800_NBI_TMX_CSR_DropProfile_DropRateRange2(_x) (((_x) & 0x7) << 6)
#define   NFP3800_NBI_TMX_CSR_DropProfile_DropRateRange2_of(_x) (((_x) >> 6) & 0x7)
#define     NFP3800_NBI_TMX_CSR_DropProfile_DropRateRange2_0 (0)
#define     NFP3800_NBI_TMX_CSR_DropProfile_DropRateRange2_12_5 (1)
#define     NFP3800_NBI_TMX_CSR_DropProfile_DropRateRange2_25 (2)
#define     NFP3800_NBI_TMX_CSR_DropProfile_DropRateRange2_37_5 (3)
#define     NFP3800_NBI_TMX_CSR_DropProfile_DropRateRange2_50 (4)
#define     NFP3800_NBI_TMX_CSR_DropProfile_DropRateRange2_62_5 (5)
#define     NFP3800_NBI_TMX_CSR_DropProfile_DropRateRange2_75 (6)
#define     NFP3800_NBI_TMX_CSR_DropProfile_DropRateRange2_87_5 (7)
#define   NFP3800_NBI_TMX_CSR_DropProfile_DropRateRange1(_x) (((_x) & 0x7) << 3)
#define   NFP3800_NBI_TMX_CSR_DropProfile_DropRateRange1_of(_x) (((_x) >> 3) & 0x7)
#define     NFP3800_NBI_TMX_CSR_DropProfile_DropRateRange1_0 (0)
#define     NFP3800_NBI_TMX_CSR_DropProfile_DropRateRange1_12_5 (1)
#define     NFP3800_NBI_TMX_CSR_DropProfile_DropRateRange1_25 (2)
#define     NFP3800_NBI_TMX_CSR_DropProfile_DropRateRange1_37_5 (3)
#define     NFP3800_NBI_TMX_CSR_DropProfile_DropRateRange1_50 (4)
#define     NFP3800_NBI_TMX_CSR_DropProfile_DropRateRange1_62_5 (5)
#define     NFP3800_NBI_TMX_CSR_DropProfile_DropRateRange1_75 (6)
#define     NFP3800_NBI_TMX_CSR_DropProfile_DropRateRange1_87_5 (7)
#define   NFP3800_NBI_TMX_CSR_DropProfile_DropRateRange0(_x) (((_x) & 0x7) << 0)
#define   NFP3800_NBI_TMX_CSR_DropProfile_DropRateRange0_of(_x) (((_x) >> 0) & 0x7)
#define     NFP3800_NBI_TMX_CSR_DropProfile_DropRateRange0_0 (0)
#define     NFP3800_NBI_TMX_CSR_DropProfile_DropRateRange0_12_5 (1)
#define     NFP3800_NBI_TMX_CSR_DropProfile_DropRateRange0_25 (2)
#define     NFP3800_NBI_TMX_CSR_DropProfile_DropRateRange0_37_5 (3)
#define     NFP3800_NBI_TMX_CSR_DropProfile_DropRateRange0_50 (4)
#define     NFP3800_NBI_TMX_CSR_DropProfile_DropRateRange0_62_5 (5)
#define     NFP3800_NBI_TMX_CSR_DropProfile_DropRateRange0_75 (6)
#define     NFP3800_NBI_TMX_CSR_DropProfile_DropRateRange0_87_5 (7)
/*----------------------------------------------------------------
  Register: DescriptorMemoryRedDrop - Packet Descriptor RED drops.
    Bits: 11:9  DropRateRange3(rw) - Drop Rate for Descriptor Memory Depth Range 3. Top 75% to 100% of the last 512 space of Descriptor Memory. Packets will be dropped n% of the time if the memory depth is within this range
    Bits:  8:6  DropRateRange2(rw) - Drop Rate for Descriptor Memory Depth Range 2. 25% to 49% of the last 512 space of Descriptor Memory. Packets will be dropped n% of the time if the memory depth is within this range
    Bits:  5:3  DropRateRange1(rw) - Drop Rate for Descriptor Memory Depth Range 1. 25% to 49% of the last 512 space of Descriptor Memory. Packets will be dropped n% of the time if the memory depth is within this range
    Bits:  2:0  DropRateRange0(rw) - Drop Rate for Descriptor Memory Depth Range 0. Bottom 0 to 24% of the last 512 space of Descriptor Memory. Packets will be dropped n% of the time if the memory depth is within this range
*/
#define NFP3800_NBI_TMX_CSR_DescriptorMemoryRedDrop          0x00000628
#define   NFP3800_NBI_TMX_CSR_DescriptorMemoryRedDrop_DropRateRange3(_x) (((_x) & 0x7) << 9)
#define   NFP3800_NBI_TMX_CSR_DescriptorMemoryRedDrop_DropRateRange3_of(_x) (((_x) >> 9) & 0x7)
#define     NFP3800_NBI_TMX_CSR_DescriptorMemoryRedDrop_DropRateRange3_0 (0)
#define     NFP3800_NBI_TMX_CSR_DescriptorMemoryRedDrop_DropRateRange3_12_5 (1)
#define     NFP3800_NBI_TMX_CSR_DescriptorMemoryRedDrop_DropRateRange3_25 (2)
#define     NFP3800_NBI_TMX_CSR_DescriptorMemoryRedDrop_DropRateRange3_37_5 (3)
#define     NFP3800_NBI_TMX_CSR_DescriptorMemoryRedDrop_DropRateRange3_50 (4)
#define     NFP3800_NBI_TMX_CSR_DescriptorMemoryRedDrop_DropRateRange3_62_5 (5)
#define     NFP3800_NBI_TMX_CSR_DescriptorMemoryRedDrop_DropRateRange3_75 (6)
#define     NFP3800_NBI_TMX_CSR_DescriptorMemoryRedDrop_DropRateRange3_87_5 (7)
#define   NFP3800_NBI_TMX_CSR_DescriptorMemoryRedDrop_DropRateRange2(_x) (((_x) & 0x7) << 6)
#define   NFP3800_NBI_TMX_CSR_DescriptorMemoryRedDrop_DropRateRange2_of(_x) (((_x) >> 6) & 0x7)
#define     NFP3800_NBI_TMX_CSR_DescriptorMemoryRedDrop_DropRateRange2_0 (0)
#define     NFP3800_NBI_TMX_CSR_DescriptorMemoryRedDrop_DropRateRange2_12_5 (1)
#define     NFP3800_NBI_TMX_CSR_DescriptorMemoryRedDrop_DropRateRange2_25 (2)
#define     NFP3800_NBI_TMX_CSR_DescriptorMemoryRedDrop_DropRateRange2_37_5 (3)
#define     NFP3800_NBI_TMX_CSR_DescriptorMemoryRedDrop_DropRateRange2_50 (4)
#define     NFP3800_NBI_TMX_CSR_DescriptorMemoryRedDrop_DropRateRange2_62_5 (5)
#define     NFP3800_NBI_TMX_CSR_DescriptorMemoryRedDrop_DropRateRange2_75 (6)
#define     NFP3800_NBI_TMX_CSR_DescriptorMemoryRedDrop_DropRateRange2_87_5 (7)
#define   NFP3800_NBI_TMX_CSR_DescriptorMemoryRedDrop_DropRateRange1(_x) (((_x) & 0x7) << 3)
#define   NFP3800_NBI_TMX_CSR_DescriptorMemoryRedDrop_DropRateRange1_of(_x) (((_x) >> 3) & 0x7)
#define     NFP3800_NBI_TMX_CSR_DescriptorMemoryRedDrop_DropRateRange1_0 (0)
#define     NFP3800_NBI_TMX_CSR_DescriptorMemoryRedDrop_DropRateRange1_12_5 (1)
#define     NFP3800_NBI_TMX_CSR_DescriptorMemoryRedDrop_DropRateRange1_25 (2)
#define     NFP3800_NBI_TMX_CSR_DescriptorMemoryRedDrop_DropRateRange1_37_5 (3)
#define     NFP3800_NBI_TMX_CSR_DescriptorMemoryRedDrop_DropRateRange1_50 (4)
#define     NFP3800_NBI_TMX_CSR_DescriptorMemoryRedDrop_DropRateRange1_62_5 (5)
#define     NFP3800_NBI_TMX_CSR_DescriptorMemoryRedDrop_DropRateRange1_75 (6)
#define     NFP3800_NBI_TMX_CSR_DescriptorMemoryRedDrop_DropRateRange1_87_5 (7)
#define   NFP3800_NBI_TMX_CSR_DescriptorMemoryRedDrop_DropRateRange0(_x) (((_x) & 0x7) << 0)
#define   NFP3800_NBI_TMX_CSR_DescriptorMemoryRedDrop_DropRateRange0_of(_x) (((_x) >> 0) & 0x7)
#define     NFP3800_NBI_TMX_CSR_DescriptorMemoryRedDrop_DropRateRange0_0 (0)
#define     NFP3800_NBI_TMX_CSR_DescriptorMemoryRedDrop_DropRateRange0_12_5 (1)
#define     NFP3800_NBI_TMX_CSR_DescriptorMemoryRedDrop_DropRateRange0_25 (2)
#define     NFP3800_NBI_TMX_CSR_DescriptorMemoryRedDrop_DropRateRange0_37_5 (3)
#define     NFP3800_NBI_TMX_CSR_DescriptorMemoryRedDrop_DropRateRange0_50 (4)
#define     NFP3800_NBI_TMX_CSR_DescriptorMemoryRedDrop_DropRateRange0_62_5 (5)
#define     NFP3800_NBI_TMX_CSR_DescriptorMemoryRedDrop_DropRateRange0_75 (6)
#define     NFP3800_NBI_TMX_CSR_DescriptorMemoryRedDrop_DropRateRange0_87_5 (7)
/*----------------------------------------------------------------
  Register: MiniPktChannelCreditLimit[128] - MiniPacket Channel Credit limit Configuration Register
    Bits:  9:0  ChannelCreditLimit(rw) - MiniPacket Transmit Channel Credit Limit. Programmable limit per channel
*/
#define NFP3800_NBI_TMX_CSR_MiniPktChannelCreditLimit(_x)    (0x0000062c + (0x4 * ((_x) & 0x7f)))
#define   NFP3800_NBI_TMX_CSR_MiniPktChannelCreditLimit_ChannelCreditLimit(_x) (((_x) & 0x3ff) << 0)
#define   NFP3800_NBI_TMX_CSR_MiniPktChannelCreditLimit_ChannelCreditLimit_of(_x) (((_x) >> 0) & 0x3ff)
/*----------------------------------------------------------------
  Register: TmReorderBufferFlush - Reorder buffer flush command per sequencer.
    Bits: 15:0  SequencerFlush(rw) - This 32-bit register defines a bit per sequencer that needs to be flushed. Multiple sequencers can be flushed by setting the appropriate bits. Bit 0 represents sequencer number 0 and bit 15 represents sequencer number 15. The software can set the appropriate bits for the sequencers that need to be flushed. The hardware will clear those bits once those sequencers are flushed. It is software's responsibility to set appropriate bits in this register based on the NumSequencers register.
*/
#define NFP3800_NBI_TMX_CSR_TmReorderBufferFlush             0x0000082c
#define   NFP3800_NBI_TMX_CSR_TmReorderBufferFlush_SequencerFlush(_x) (((_x) & 0xffff) << 0)
#define   NFP3800_NBI_TMX_CSR_TmReorderBufferFlush_SequencerFlush_of(_x) (((_x) >> 0) & 0xffff)
/*----------------------------------------------------------------
  Register: TmReorderTimeOutRange0 - Timeout base range for sequencers 0 to 7.
    Bits: 31:30 SeqRange15(rw) - Defines the timeout range select for Sequencer 15
    Bits: 29:28 SeqRange14(rw) - Defines the timeout range select for Sequencer 14
    Bits: 27:26 SeqRange13(rw) - Defines the timeout range select for Sequencer 13
    Bits: 25:24 SeqRange12(rw) - Defines the timeout range select for Sequencer 12
    Bits: 23:22 SeqRange11(rw) - Defines the timeout range select for Sequencer 11
    Bits: 21:20 SeqRange10(rw) - Defines the timeout range select for Sequencer 10
    Bits: 19:18 SeqRange9(rw) - Defines the timeout range select for Sequencer 9
    Bits: 17:16 SeqRange8(rw) - Defines the timeout range select for Sequencer 8
    Bits: 15:14 SeqRange7(rw) - Defines the timeout range select for Sequencer 7
    Bits: 13:12 SeqRange6(rw) - Defines the timeout range select for Sequencer 6
    Bits: 11:10 SeqRange5(rw) - Defines the timeout range select for Sequencer 5
    Bits:  9:8  SeqRange4(rw) - Defines the timeout range select for Sequencer 4
    Bits:  7:6  SeqRange3(rw) - Defines the timeout range select for Sequencer 3
    Bits:  5:4  SeqRange2(rw) - Defines the timeout range select for Sequencer 2
    Bits:  3:2  SeqRange1(rw) - Defines the timeout range select for Sequencer 1
    Bits:  1:0  SeqRange0(rw) - Defines the timeout range select for Sequencer 0
*/
#define NFP3800_NBI_TMX_CSR_TmReorderTimeOutRange0           0x00000830
#define   NFP3800_NBI_TMX_CSR_TmReorderTimeOutRange0_SeqRange15(_x) (((_x) & 0x3) << 30)
#define   NFP3800_NBI_TMX_CSR_TmReorderTimeOutRange0_SeqRange15_of(_x) (((_x) >> 30) & 0x3)
#define     NFP3800_NBI_TMX_CSR_TmReorderTimeOutRange0_SeqRange15_disable (0)
#define     NFP3800_NBI_TMX_CSR_TmReorderTimeOutRange0_SeqRange15_1k_cycles (1)
#define     NFP3800_NBI_TMX_CSR_TmReorderTimeOutRange0_SeqRange15_10k_cycles (2)
#define     NFP3800_NBI_TMX_CSR_TmReorderTimeOutRange0_SeqRange15_100k_cycles (3)
#define   NFP3800_NBI_TMX_CSR_TmReorderTimeOutRange0_SeqRange14(_x) (((_x) & 0x3) << 28)
#define   NFP3800_NBI_TMX_CSR_TmReorderTimeOutRange0_SeqRange14_of(_x) (((_x) >> 28) & 0x3)
#define     NFP3800_NBI_TMX_CSR_TmReorderTimeOutRange0_SeqRange14_disable (0)
#define     NFP3800_NBI_TMX_CSR_TmReorderTimeOutRange0_SeqRange14_1k_cycles (1)
#define     NFP3800_NBI_TMX_CSR_TmReorderTimeOutRange0_SeqRange14_10k_cycles (2)
#define     NFP3800_NBI_TMX_CSR_TmReorderTimeOutRange0_SeqRange14_100k_cycles (3)
#define   NFP3800_NBI_TMX_CSR_TmReorderTimeOutRange0_SeqRange13(_x) (((_x) & 0x3) << 26)
#define   NFP3800_NBI_TMX_CSR_TmReorderTimeOutRange0_SeqRange13_of(_x) (((_x) >> 26) & 0x3)
#define     NFP3800_NBI_TMX_CSR_TmReorderTimeOutRange0_SeqRange13_disable (0)
#define     NFP3800_NBI_TMX_CSR_TmReorderTimeOutRange0_SeqRange13_1k_cycles (1)
#define     NFP3800_NBI_TMX_CSR_TmReorderTimeOutRange0_SeqRange13_10k_cycles (2)
#define     NFP3800_NBI_TMX_CSR_TmReorderTimeOutRange0_SeqRange13_100k_cycles (3)
#define   NFP3800_NBI_TMX_CSR_TmReorderTimeOutRange0_SeqRange12(_x) (((_x) & 0x3) << 24)
#define   NFP3800_NBI_TMX_CSR_TmReorderTimeOutRange0_SeqRange12_of(_x) (((_x) >> 24) & 0x3)
#define     NFP3800_NBI_TMX_CSR_TmReorderTimeOutRange0_SeqRange12_disable (0)
#define     NFP3800_NBI_TMX_CSR_TmReorderTimeOutRange0_SeqRange12_1k_cycles (1)
#define     NFP3800_NBI_TMX_CSR_TmReorderTimeOutRange0_SeqRange12_10k_cycles (2)
#define     NFP3800_NBI_TMX_CSR_TmReorderTimeOutRange0_SeqRange12_100k_cycles (3)
#define   NFP3800_NBI_TMX_CSR_TmReorderTimeOutRange0_SeqRange11(_x) (((_x) & 0x3) << 22)
#define   NFP3800_NBI_TMX_CSR_TmReorderTimeOutRange0_SeqRange11_of(_x) (((_x) >> 22) & 0x3)
#define     NFP3800_NBI_TMX_CSR_TmReorderTimeOutRange0_SeqRange11_disable (0)
#define     NFP3800_NBI_TMX_CSR_TmReorderTimeOutRange0_SeqRange11_1k_cycles (1)
#define     NFP3800_NBI_TMX_CSR_TmReorderTimeOutRange0_SeqRange11_10k_cycles (2)
#define     NFP3800_NBI_TMX_CSR_TmReorderTimeOutRange0_SeqRange11_100k_cycles (3)
#define   NFP3800_NBI_TMX_CSR_TmReorderTimeOutRange0_SeqRange10(_x) (((_x) & 0x3) << 20)
#define   NFP3800_NBI_TMX_CSR_TmReorderTimeOutRange0_SeqRange10_of(_x) (((_x) >> 20) & 0x3)
#define     NFP3800_NBI_TMX_CSR_TmReorderTimeOutRange0_SeqRange10_disable (0)
#define     NFP3800_NBI_TMX_CSR_TmReorderTimeOutRange0_SeqRange10_1k_cycles (1)
#define     NFP3800_NBI_TMX_CSR_TmReorderTimeOutRange0_SeqRange10_10k_cycles (2)
#define     NFP3800_NBI_TMX_CSR_TmReorderTimeOutRange0_SeqRange10_100k_cycles (3)
#define   NFP3800_NBI_TMX_CSR_TmReorderTimeOutRange0_SeqRange9(_x) (((_x) & 0x3) << 18)
#define   NFP3800_NBI_TMX_CSR_TmReorderTimeOutRange0_SeqRange9_of(_x) (((_x) >> 18) & 0x3)
#define     NFP3800_NBI_TMX_CSR_TmReorderTimeOutRange0_SeqRange9_disable (0)
#define     NFP3800_NBI_TMX_CSR_TmReorderTimeOutRange0_SeqRange9_1k_cycles (1)
#define     NFP3800_NBI_TMX_CSR_TmReorderTimeOutRange0_SeqRange9_10k_cycles (2)
#define     NFP3800_NBI_TMX_CSR_TmReorderTimeOutRange0_SeqRange9_100k_cycles (3)
#define   NFP3800_NBI_TMX_CSR_TmReorderTimeOutRange0_SeqRange8(_x) (((_x) & 0x3) << 16)
#define   NFP3800_NBI_TMX_CSR_TmReorderTimeOutRange0_SeqRange8_of(_x) (((_x) >> 16) & 0x3)
#define     NFP3800_NBI_TMX_CSR_TmReorderTimeOutRange0_SeqRange8_disable (0)
#define     NFP3800_NBI_TMX_CSR_TmReorderTimeOutRange0_SeqRange8_1k_cycles (1)
#define     NFP3800_NBI_TMX_CSR_TmReorderTimeOutRange0_SeqRange8_10k_cycles (2)
#define     NFP3800_NBI_TMX_CSR_TmReorderTimeOutRange0_SeqRange8_100k_cycles (3)
#define   NFP3800_NBI_TMX_CSR_TmReorderTimeOutRange0_SeqRange7(_x) (((_x) & 0x3) << 14)
#define   NFP3800_NBI_TMX_CSR_TmReorderTimeOutRange0_SeqRange7_of(_x) (((_x) >> 14) & 0x3)
#define     NFP3800_NBI_TMX_CSR_TmReorderTimeOutRange0_SeqRange7_disable (0)
#define     NFP3800_NBI_TMX_CSR_TmReorderTimeOutRange0_SeqRange7_1k_cycles (1)
#define     NFP3800_NBI_TMX_CSR_TmReorderTimeOutRange0_SeqRange7_10k_cycles (2)
#define     NFP3800_NBI_TMX_CSR_TmReorderTimeOutRange0_SeqRange7_100k_cycles (3)
#define   NFP3800_NBI_TMX_CSR_TmReorderTimeOutRange0_SeqRange6(_x) (((_x) & 0x3) << 12)
#define   NFP3800_NBI_TMX_CSR_TmReorderTimeOutRange0_SeqRange6_of(_x) (((_x) >> 12) & 0x3)
#define     NFP3800_NBI_TMX_CSR_TmReorderTimeOutRange0_SeqRange6_disable (0)
#define     NFP3800_NBI_TMX_CSR_TmReorderTimeOutRange0_SeqRange6_1k_cycles (1)
#define     NFP3800_NBI_TMX_CSR_TmReorderTimeOutRange0_SeqRange6_10k_cycles (2)
#define     NFP3800_NBI_TMX_CSR_TmReorderTimeOutRange0_SeqRange6_100k_cycles (3)
#define   NFP3800_NBI_TMX_CSR_TmReorderTimeOutRange0_SeqRange5(_x) (((_x) & 0x3) << 10)
#define   NFP3800_NBI_TMX_CSR_TmReorderTimeOutRange0_SeqRange5_of(_x) (((_x) >> 10) & 0x3)
#define     NFP3800_NBI_TMX_CSR_TmReorderTimeOutRange0_SeqRange5_disable (0)
#define     NFP3800_NBI_TMX_CSR_TmReorderTimeOutRange0_SeqRange5_1k_cycles (1)
#define     NFP3800_NBI_TMX_CSR_TmReorderTimeOutRange0_SeqRange5_10k_cycles (2)
#define     NFP3800_NBI_TMX_CSR_TmReorderTimeOutRange0_SeqRange5_100k_cycles (3)
#define   NFP3800_NBI_TMX_CSR_TmReorderTimeOutRange0_SeqRange4(_x) (((_x) & 0x3) << 8)
#define   NFP3800_NBI_TMX_CSR_TmReorderTimeOutRange0_SeqRange4_of(_x) (((_x) >> 8) & 0x3)
#define     NFP3800_NBI_TMX_CSR_TmReorderTimeOutRange0_SeqRange4_disable (0)
#define     NFP3800_NBI_TMX_CSR_TmReorderTimeOutRange0_SeqRange4_1k_cycles (1)
#define     NFP3800_NBI_TMX_CSR_TmReorderTimeOutRange0_SeqRange4_10k_cycles (2)
#define     NFP3800_NBI_TMX_CSR_TmReorderTimeOutRange0_SeqRange4_100k_cycles (3)
#define   NFP3800_NBI_TMX_CSR_TmReorderTimeOutRange0_SeqRange3(_x) (((_x) & 0x3) << 6)
#define   NFP3800_NBI_TMX_CSR_TmReorderTimeOutRange0_SeqRange3_of(_x) (((_x) >> 6) & 0x3)
#define     NFP3800_NBI_TMX_CSR_TmReorderTimeOutRange0_SeqRange3_disable (0)
#define     NFP3800_NBI_TMX_CSR_TmReorderTimeOutRange0_SeqRange3_1k_cycles (1)
#define     NFP3800_NBI_TMX_CSR_TmReorderTimeOutRange0_SeqRange3_10k_cycles (2)
#define     NFP3800_NBI_TMX_CSR_TmReorderTimeOutRange0_SeqRange3_100k_cycles (3)
#define   NFP3800_NBI_TMX_CSR_TmReorderTimeOutRange0_SeqRange2(_x) (((_x) & 0x3) << 4)
#define   NFP3800_NBI_TMX_CSR_TmReorderTimeOutRange0_SeqRange2_of(_x) (((_x) >> 4) & 0x3)
#define     NFP3800_NBI_TMX_CSR_TmReorderTimeOutRange0_SeqRange2_disable (0)
#define     NFP3800_NBI_TMX_CSR_TmReorderTimeOutRange0_SeqRange2_1k_cycles (1)
#define     NFP3800_NBI_TMX_CSR_TmReorderTimeOutRange0_SeqRange2_10k_cycles (2)
#define     NFP3800_NBI_TMX_CSR_TmReorderTimeOutRange0_SeqRange2_100k_cycles (3)
#define   NFP3800_NBI_TMX_CSR_TmReorderTimeOutRange0_SeqRange1(_x) (((_x) & 0x3) << 2)
#define   NFP3800_NBI_TMX_CSR_TmReorderTimeOutRange0_SeqRange1_of(_x) (((_x) >> 2) & 0x3)
#define     NFP3800_NBI_TMX_CSR_TmReorderTimeOutRange0_SeqRange1_disable (0)
#define     NFP3800_NBI_TMX_CSR_TmReorderTimeOutRange0_SeqRange1_1k_cycles (1)
#define     NFP3800_NBI_TMX_CSR_TmReorderTimeOutRange0_SeqRange1_10k_cycles (2)
#define     NFP3800_NBI_TMX_CSR_TmReorderTimeOutRange0_SeqRange1_100k_cycles (3)
#define   NFP3800_NBI_TMX_CSR_TmReorderTimeOutRange0_SeqRange0(_x) (((_x) & 0x3) << 0)
#define   NFP3800_NBI_TMX_CSR_TmReorderTimeOutRange0_SeqRange0_of(_x) (((_x) >> 0) & 0x3)
#define     NFP3800_NBI_TMX_CSR_TmReorderTimeOutRange0_SeqRange0_disable (0)
#define     NFP3800_NBI_TMX_CSR_TmReorderTimeOutRange0_SeqRange0_1k_cycles (1)
#define     NFP3800_NBI_TMX_CSR_TmReorderTimeOutRange0_SeqRange0_10k_cycles (2)
#define     NFP3800_NBI_TMX_CSR_TmReorderTimeOutRange0_SeqRange0_100k_cycles (3)
/*----------------------------------------------------------------
  Register: TmReorderTimeOutValue0 - Timeout value for sequencers 0 to 3.
    Bits: 31:24 SeqValue3(rw) - Defines the timeout value for Sequencer 3
    Bits: 23:16 SeqValue2(rw) - Defines the timeout value for Sequencer 2
    Bits: 15:8  SeqValue1(rw) - Defines the timeout value for Sequencer 1
    Bits:  7:0  SeqValue0(rw) - Defines the timeout value for Sequencer 0
*/
#define NFP3800_NBI_TMX_CSR_TmReorderTimeOutValue0           0x00000840
#define   NFP3800_NBI_TMX_CSR_TmReorderTimeOutValue0_SeqValue3(_x) (((_x) & 0xff) << 24)
#define   NFP3800_NBI_TMX_CSR_TmReorderTimeOutValue0_SeqValue3_of(_x) (((_x) >> 24) & 0xff)
#define   NFP3800_NBI_TMX_CSR_TmReorderTimeOutValue0_SeqValue2(_x) (((_x) & 0xff) << 16)
#define   NFP3800_NBI_TMX_CSR_TmReorderTimeOutValue0_SeqValue2_of(_x) (((_x) >> 16) & 0xff)
#define   NFP3800_NBI_TMX_CSR_TmReorderTimeOutValue0_SeqValue1(_x) (((_x) & 0xff) << 8)
#define   NFP3800_NBI_TMX_CSR_TmReorderTimeOutValue0_SeqValue1_of(_x) (((_x) >> 8) & 0xff)
#define   NFP3800_NBI_TMX_CSR_TmReorderTimeOutValue0_SeqValue0(_x) (((_x) & 0xff) << 0)
#define   NFP3800_NBI_TMX_CSR_TmReorderTimeOutValue0_SeqValue0_of(_x) (((_x) >> 0) & 0xff)
/*----------------------------------------------------------------
  Register: TmReorderTimeOutValue1 - Timeout value for sequencers 4 to 7.
    Bits: 31:24 SeqValue7(rw) - Defines the timeout value for Sequencer 7
    Bits: 23:16 SeqValue6(rw) - Defines the timeout value for Sequencer 6
    Bits: 15:8  SeqValue5(rw) - Defines the timeout value for Sequencer 5
    Bits:  7:0  SeqValue4(rw) - Defines the timeout value for Sequencer 4
*/
#define NFP3800_NBI_TMX_CSR_TmReorderTimeOutValue1           0x00000844
#define   NFP3800_NBI_TMX_CSR_TmReorderTimeOutValue1_SeqValue7(_x) (((_x) & 0xff) << 24)
#define   NFP3800_NBI_TMX_CSR_TmReorderTimeOutValue1_SeqValue7_of(_x) (((_x) >> 24) & 0xff)
#define   NFP3800_NBI_TMX_CSR_TmReorderTimeOutValue1_SeqValue6(_x) (((_x) & 0xff) << 16)
#define   NFP3800_NBI_TMX_CSR_TmReorderTimeOutValue1_SeqValue6_of(_x) (((_x) >> 16) & 0xff)
#define   NFP3800_NBI_TMX_CSR_TmReorderTimeOutValue1_SeqValue5(_x) (((_x) & 0xff) << 8)
#define   NFP3800_NBI_TMX_CSR_TmReorderTimeOutValue1_SeqValue5_of(_x) (((_x) >> 8) & 0xff)
#define   NFP3800_NBI_TMX_CSR_TmReorderTimeOutValue1_SeqValue4(_x) (((_x) & 0xff) << 0)
#define   NFP3800_NBI_TMX_CSR_TmReorderTimeOutValue1_SeqValue4_of(_x) (((_x) >> 0) & 0xff)
/*----------------------------------------------------------------
  Register: TmReorderTimeOutValue2 - Timeout value for sequencers 8 to 11.
    Bits: 31:24 SeqValue11(rw) - Defines the timeout value for Sequencer 11
    Bits: 23:16 SeqValue10(rw) - Defines the timeout value for Sequencer 10
    Bits: 15:8  SeqValue9(rw) - Defines the timeout value for Sequencer 9
    Bits:  7:0  SeqValue8(rw) - Defines the timeout value for Sequencer 8
*/
#define NFP3800_NBI_TMX_CSR_TmReorderTimeOutValue2           0x00000848
#define   NFP3800_NBI_TMX_CSR_TmReorderTimeOutValue2_SeqValue11(_x) (((_x) & 0xff) << 24)
#define   NFP3800_NBI_TMX_CSR_TmReorderTimeOutValue2_SeqValue11_of(_x) (((_x) >> 24) & 0xff)
#define   NFP3800_NBI_TMX_CSR_TmReorderTimeOutValue2_SeqValue10(_x) (((_x) & 0xff) << 16)
#define   NFP3800_NBI_TMX_CSR_TmReorderTimeOutValue2_SeqValue10_of(_x) (((_x) >> 16) & 0xff)
#define   NFP3800_NBI_TMX_CSR_TmReorderTimeOutValue2_SeqValue9(_x) (((_x) & 0xff) << 8)
#define   NFP3800_NBI_TMX_CSR_TmReorderTimeOutValue2_SeqValue9_of(_x) (((_x) >> 8) & 0xff)
#define   NFP3800_NBI_TMX_CSR_TmReorderTimeOutValue2_SeqValue8(_x) (((_x) & 0xff) << 0)
#define   NFP3800_NBI_TMX_CSR_TmReorderTimeOutValue2_SeqValue8_of(_x) (((_x) >> 0) & 0xff)
/*----------------------------------------------------------------
  Register: TmReorderTimeOutValue3 - Timeout value for sequencers 12 to 15.
    Bits: 31:24 SeqValue15(rw) - Defines the timeout value for Sequencer 15
    Bits: 23:16 SeqValue14(rw) - Defines the timeout value for Sequencer 14
    Bits: 15:8  SeqValue13(rw) - Defines the timeout value for Sequencer 13
    Bits:  7:0  SeqValue12(rw) - Defines the timeout value for Sequencer 12
*/
#define NFP3800_NBI_TMX_CSR_TmReorderTimeOutValue3           0x0000084c
#define   NFP3800_NBI_TMX_CSR_TmReorderTimeOutValue3_SeqValue15(_x) (((_x) & 0xff) << 24)
#define   NFP3800_NBI_TMX_CSR_TmReorderTimeOutValue3_SeqValue15_of(_x) (((_x) >> 24) & 0xff)
#define   NFP3800_NBI_TMX_CSR_TmReorderTimeOutValue3_SeqValue14(_x) (((_x) & 0xff) << 16)
#define   NFP3800_NBI_TMX_CSR_TmReorderTimeOutValue3_SeqValue14_of(_x) (((_x) >> 16) & 0xff)
#define   NFP3800_NBI_TMX_CSR_TmReorderTimeOutValue3_SeqValue13(_x) (((_x) & 0xff) << 8)
#define   NFP3800_NBI_TMX_CSR_TmReorderTimeOutValue3_SeqValue13_of(_x) (((_x) >> 8) & 0xff)
#define   NFP3800_NBI_TMX_CSR_TmReorderTimeOutValue3_SeqValue12(_x) (((_x) & 0xff) << 0)
#define   NFP3800_NBI_TMX_CSR_TmReorderTimeOutValue3_SeqValue12_of(_x) (((_x) >> 0) & 0xff)
/*----------------------------------------------------------------
  Register: TmReorderFlushFlag - Sticky Flush Flag, Clear on read.
    Bits: 15:0  TmReorderFlushFlag(rc) - This 32-bit register is a sticky flush flag per sequencer. Once Flush occurs due to timeout, software flush or flush due to incoming sequence for a given sequencer - respective bit would be set and the register only be cleared on read.
*/
#define NFP3800_NBI_TMX_CSR_TmReorderFlushFlag               0x00000900
#define   NFP3800_NBI_TMX_CSR_TmReorderFlushFlag_TmReorderFlushFlag(_x) (((_x) & 0xffff) << 0)
#define   NFP3800_NBI_TMX_CSR_TmReorderFlushFlag_TmReorderFlushFlag_of(_x) (((_x) >> 0) & 0xffff)
/*----------------------------------------------------------------
  Register: TMReceivedPacketCounter - This 32-bit register is a saturating counter, counting the incoming PktReady to Traffic Manager. It's a clear on read register.
    Bits: 31:0  TMReceivedPacketCounter(rc) - This 32-bit register is a saturating counter, counting the incoming PktReady to Traffic Manager. It's a clear on read register.
*/
#define NFP3800_NBI_TMX_CSR_TMReceivedPacketCounter          0x00000904
#define   NFP3800_NBI_TMX_CSR_TMReceivedPacketCounter_TMReceivedPacketCounter(_x) (((_x) & 0xffffffff) << 0)
#define   NFP3800_NBI_TMX_CSR_TMReceivedPacketCounter_TMReceivedPacketCounter_of(_x) (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: TMTransmittedPacketCounter - This 32-bit register is a saturating counter, counting the descriptors going out to DMA block from the Descriptor Queues. It's a clear on read register.
    Bits: 31:0  TMTransmittedPacketCounter(rc) - This 32-bit register is a saturating counter, counting the descriptors going out to DMA block from the Descriptor Queues. It's a clear on read register.
*/
#define NFP3800_NBI_TMX_CSR_TMTransmittedPacketCounter       0x00000908
#define   NFP3800_NBI_TMX_CSR_TMTransmittedPacketCounter_TMTransmittedPacketCounter(_x) (((_x) & 0xffffffff) << 0)
#define   NFP3800_NBI_TMX_CSR_TMTransmittedPacketCounter_TMTransmittedPacketCounter_of(_x) (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: TMBlqNullMuPtr - Programmable Null Memory Buffer Pointer.
    Bits: 28:0  TMBlqNullMuPtr(rw) - Programmable Null Memory Buffer Pointer.
*/
#define NFP3800_NBI_TMX_CSR_TMBlqNullMuPtr                   0x0000090c
#define   NFP3800_NBI_TMX_CSR_TMBlqNullMuPtr_TMBlqNullMuPtr(_x) (((_x) & 0x1fffffff) << 0)
#define   NFP3800_NBI_TMX_CSR_TMBlqNullMuPtr_TMBlqNullMuPtr_of(_x) (((_x) >> 0) & 0x1fffffff)
/*----------------------------------------------------------------
  Register: TMReorderSeqrEmpty - 32-bits indicating Empty Reorder Sequencers.
    Bits: 15:0  TMReorderSeqrEmpty(ro) - 16-bits indicating Empty Reorder Sequencers. On Reset all the Sequencers are empty
*/
#define NFP3800_NBI_TMX_CSR_TMReorderSeqrEmpty               0x00000910
#define   NFP3800_NBI_TMX_CSR_TMReorderSeqrEmpty_TMReorderSeqrEmpty(_x) (((_x) & 0xffff) << 0)
#define   NFP3800_NBI_TMX_CSR_TMReorderSeqrEmpty_TMReorderSeqrEmpty_of(_x) (((_x) >> 0) & 0xffff)
/*----------------------------------------------------------------
  Register: TMReceivedDropPacketCounter - This 32-bit register is a saturating counter, counting the incoming PktReady to Traffic Manager which are with Token of 0. It's a clear on read register.
    Bits: 31:0  TMReceivedDropPacketCounter(rc) - This 32-bit register is a saturating counter, counting the incoming PktReady to Traffic Manager which are with Token of 0. It's a clear on read register.
*/
#define NFP3800_NBI_TMX_CSR_TMReceivedDropPacketCounter      0x00000914
#define   NFP3800_NBI_TMX_CSR_TMReceivedDropPacketCounter_TMReceivedDropPacketCounter(_x) (((_x) & 0xffffffff) << 0)
#define   NFP3800_NBI_TMX_CSR_TMReceivedDropPacketCounter_TMReceivedDropPacketCounter_of(_x) (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: NbiTmPcieCmdOutCfg[16] - Pcie Command out Configuration.
    Bits: 29:20 RingNumber(rw) - CLS/IMU/EMU Ring Number for the Pcie Command out (Only used for PCIe VM-QoS).
    Bits: 19    OutputPort(rw) - Output Command port (in NBI Island) for the Pcie Command out.
    Bits: 18:11 Island(rw) - Destination Island for the Pcie Command out.
    Bits: 10:9  Token(rw) - Token field for the Pcie Command out.
    Bits:  8:4  Action(rw) - Action field for the Pcie Command out.
    Bits:  3:0  Target(rw) - Target ID for the Destination Island target for the Pcie Command out.
*/
#define NFP3800_NBI_TMX_CSR_NbiTmPcieCmdOutCfg(_x)           (0x0000091c + (0x4 * ((_x) & 0xf)))
#define   NFP3800_NBI_TMX_CSR_NbiTmPcieCmdOutCfg_RingNumber(_x) (((_x) & 0x3ff) << 20)
#define   NFP3800_NBI_TMX_CSR_NbiTmPcieCmdOutCfg_RingNumber_of(_x) (((_x) >> 20) & 0x3ff)
#define   NFP3800_NBI_TMX_CSR_NbiTmPcieCmdOutCfg_OutputPort  BIT(19)
#define   NFP3800_NBI_TMX_CSR_NbiTmPcieCmdOutCfg_Island(_x)  (((_x) & 0xff) << 11)
#define   NFP3800_NBI_TMX_CSR_NbiTmPcieCmdOutCfg_Island_of(_x) (((_x) >> 11) & 0xff)
#define   NFP3800_NBI_TMX_CSR_NbiTmPcieCmdOutCfg_Token(_x)   (((_x) & 0x3) << 9)
#define   NFP3800_NBI_TMX_CSR_NbiTmPcieCmdOutCfg_Token_of(_x) (((_x) >> 9) & 0x3)
#define   NFP3800_NBI_TMX_CSR_NbiTmPcieCmdOutCfg_Action(_x)  (((_x) & 0x1f) << 4)
#define   NFP3800_NBI_TMX_CSR_NbiTmPcieCmdOutCfg_Action_of(_x) (((_x) >> 4) & 0x1f)
#define   NFP3800_NBI_TMX_CSR_NbiTmPcieCmdOutCfg_Target(_x)  (((_x) & 0xf) << 0)
#define   NFP3800_NBI_TMX_CSR_NbiTmPcieCmdOutCfg_Target_of(_x) (((_x) >> 0) & 0xf)
/*----------------------------------------------------------------
  Register: FreePktOutstandingLimit - Limits the number of outstanding FreePkt commands
    Bits:  1:0  Limit(rw) - Limits the number of Outstanding FreePkt commands
*/
#define NFP3800_NBI_TMX_CSR_FreePktOutstandingLimit          0x0000095c
#define   NFP3800_NBI_TMX_CSR_FreePktOutstandingLimit_Limit(_x) (((_x) & 0x3) << 0)
#define   NFP3800_NBI_TMX_CSR_FreePktOutstandingLimit_Limit_of(_x) (((_x) >> 0) & 0x3)
#define     NFP3800_NBI_TMX_CSR_FreePktOutstandingLimit_Limit_48 (0)
#define     NFP3800_NBI_TMX_CSR_FreePktOutstandingLimit_Limit_64 (1)
#define     NFP3800_NBI_TMX_CSR_FreePktOutstandingLimit_Limit_96 (2)
#define     NFP3800_NBI_TMX_CSR_FreePktOutstandingLimit_Limit_128 (3)
/*----------------------------------------------------------------
  Register: TMChannelPcieVmQoSEna_0 - Identifies Channel 0->31 as PCIe vs Network.
    Bits: 31:0  TMChannelPcieVmQoSEna_0(rw) - Identifies Channel 0->31 as PCIe vs Network.
*/
#define NFP3800_NBI_TMX_CSR_TMChannelPcieVmQoSEna_0          0x00000960
#define   NFP3800_NBI_TMX_CSR_TMChannelPcieVmQoSEna_0_TMChannelPcieVmQoSEna_0(_x) (((_x) & 0xffffffff) << 0)
#define   NFP3800_NBI_TMX_CSR_TMChannelPcieVmQoSEna_0_TMChannelPcieVmQoSEna_0_of(_x) (((_x) >> 0) & 0xffffffff)
#define     NFP3800_NBI_TMX_CSR_TMChannelPcieVmQoSEna_0_TMChannelPcieVmQoSEna_0_Default (0)
#define     NFP3800_NBI_TMX_CSR_TMChannelPcieVmQoSEna_0_TMChannelPcieVmQoSEna_0_PCIe (1)
/*----------------------------------------------------------------
  Register: TMChannelPcieVmQoSEna_1 - Identifies Channel 32->63 as PCIe vs Network.
    Bits: 31:0  TMChannelPcieVmQoSEna_1(rw) - Identifies Channel 32->63 as PCIe vs Network.
*/
#define NFP3800_NBI_TMX_CSR_TMChannelPcieVmQoSEna_1          0x00000964
#define   NFP3800_NBI_TMX_CSR_TMChannelPcieVmQoSEna_1_TMChannelPcieVmQoSEna_1(_x) (((_x) & 0xffffffff) << 0)
#define   NFP3800_NBI_TMX_CSR_TMChannelPcieVmQoSEna_1_TMChannelPcieVmQoSEna_1_of(_x) (((_x) >> 0) & 0xffffffff)
#define     NFP3800_NBI_TMX_CSR_TMChannelPcieVmQoSEna_1_TMChannelPcieVmQoSEna_1_Default (0)
#define     NFP3800_NBI_TMX_CSR_TMChannelPcieVmQoSEna_1_TMChannelPcieVmQoSEna_1_PCIe (1)
/*----------------------------------------------------------------
  Register: TMChannelPcieVmQoSEna_2 - Identifies Channel 64->95 as PCIe vs Network.
    Bits: 31:0  TMChannelPcieVmQoSEna_2(rw) - Identifies Channel 64->95 as PCIe vs Network.
*/
#define NFP3800_NBI_TMX_CSR_TMChannelPcieVmQoSEna_2          0x00000968
#define   NFP3800_NBI_TMX_CSR_TMChannelPcieVmQoSEna_2_TMChannelPcieVmQoSEna_2(_x) (((_x) & 0xffffffff) << 0)
#define   NFP3800_NBI_TMX_CSR_TMChannelPcieVmQoSEna_2_TMChannelPcieVmQoSEna_2_of(_x) (((_x) >> 0) & 0xffffffff)
#define     NFP3800_NBI_TMX_CSR_TMChannelPcieVmQoSEna_2_TMChannelPcieVmQoSEna_2_Default (0)
#define     NFP3800_NBI_TMX_CSR_TMChannelPcieVmQoSEna_2_TMChannelPcieVmQoSEna_2_PCIe (1)
/*----------------------------------------------------------------
  Register: TMChannelPcieVmQoSEna_3 - Identifies Channel 96->127 as PCIe vs Network.
    Bits: 31:0  TMChannelPcieVmQoSEna_3(rw) - Identifies Channel 96->127 as PCIe vs Network.
*/
#define NFP3800_NBI_TMX_CSR_TMChannelPcieVmQoSEna_3          0x0000096c
#define   NFP3800_NBI_TMX_CSR_TMChannelPcieVmQoSEna_3_TMChannelPcieVmQoSEna_3(_x) (((_x) & 0xffffffff) << 0)
#define   NFP3800_NBI_TMX_CSR_TMChannelPcieVmQoSEna_3_TMChannelPcieVmQoSEna_3_of(_x) (((_x) >> 0) & 0xffffffff)
#define     NFP3800_NBI_TMX_CSR_TMChannelPcieVmQoSEna_3_TMChannelPcieVmQoSEna_3_Default (0)
#define     NFP3800_NBI_TMX_CSR_TMChannelPcieVmQoSEna_3_TMChannelPcieVmQoSEna_3_PCIe (1)
/*----------------------------------------------------------------
  Register: ReorderBuffer_perf_seqr_selector - Reorder buffer Seqr selector for Performance bus
    Bits:  3:0  ReorderBuffer_perf_seqr_selector(rw) - Reorder buffer Seqr selector for Performance bus
*/
#define NFP3800_NBI_TMX_CSR_ReorderBuffer_perf_seqr_selector 0x00000970
#define   NFP3800_NBI_TMX_CSR_ReorderBuffer_perf_seqr_selector_ReorderBuffer_perf_seqr_selector(_x) (((_x) & 0xf) << 0)
#define   NFP3800_NBI_TMX_CSR_ReorderBuffer_perf_seqr_selector_ReorderBuffer_perf_seqr_selector_of(_x) (((_x) >> 0) & 0xf)
/*----------------------------------------------------------------
  Register: ReorderBufferHeadPointerUpdate - This is a ReadOnly register that contains info about expected Sequence number based on the TrafficManagerConfig(NumSequencers) field
    Bits: 31:0  ReorderBufferHeadPointerUpdate(ro) - Based on TrafficManagerConfig(NumSequencers) field-> this 32-bit register contains following info.
*/
#define NFP3800_NBI_TMX_CSR_ReorderBufferHeadPointerUpdate   0x00000974
#define   NFP3800_NBI_TMX_CSR_ReorderBufferHeadPointerUpdate_ReorderBufferHeadPointerUpdate(_x) (((_x) & 0xffffffff) << 0)
#define   NFP3800_NBI_TMX_CSR_ReorderBufferHeadPointerUpdate_ReorderBufferHeadPointerUpdate_of(_x) (((_x) >> 0) & 0xffffffff)
#define     NFP3800_NBI_TMX_CSR_ReorderBufferHeadPointerUpdate_ReorderBufferHeadPointerUpdate_NumSequencers___ (1)
#define     NFP3800_NBI_TMX_CSR_ReorderBufferHeadPointerUpdate_ReorderBufferHeadPointerUpdate_NumSequencers____ (2)
/*----------------------------------------------------------------
  Register: ShaperRateSelector - Configuration register for different ShaperRates per level
    Bits:  8:6  L2ShaperRateSelector(rw) - These three bits decide the Range of Shaper rates for Level2 Shapers
    Bits:  5:3  L1ShaperRateSelector(rw) - These three bits decide the Range of Shaper rates for Level1 Shapers
    Bits:  2:0  L0ShaperRateSelector(rw) - These three bits decide the Range of Shaper rates for Level0 Shaper
*/
#define NFP3800_NBI_TMX_CSR_ShaperRateSelector               0x00000978
#define   NFP3800_NBI_TMX_CSR_ShaperRateSelector_L2ShaperRateSelector(_x) (((_x) & 0x7) << 6)
#define   NFP3800_NBI_TMX_CSR_ShaperRateSelector_L2ShaperRateSelector_of(_x) (((_x) >> 6) & 0x7)
#define     NFP3800_NBI_TMX_CSR_ShaperRateSelector_L2ShaperRateSelector_Default (0)
#define     NFP3800_NBI_TMX_CSR_ShaperRateSelector_L2ShaperRateSelector_Divide_by_2 (1)
#define     NFP3800_NBI_TMX_CSR_ShaperRateSelector_L2ShaperRateSelector_Divide_by_4 (2)
#define     NFP3800_NBI_TMX_CSR_ShaperRateSelector_L2ShaperRateSelector_Divide_by_8 (3)
#define     NFP3800_NBI_TMX_CSR_ShaperRateSelector_L2ShaperRateSelector_Divide_by_16 (4)
#define     NFP3800_NBI_TMX_CSR_ShaperRateSelector_L2ShaperRateSelector_Divide_by_128 (5)
#define     NFP3800_NBI_TMX_CSR_ShaperRateSelector_L2ShaperRateSelector_Divide_by_512 (6)
#define     NFP3800_NBI_TMX_CSR_ShaperRateSelector_L2ShaperRateSelector_Divide_by_1024 (7)
#define   NFP3800_NBI_TMX_CSR_ShaperRateSelector_L1ShaperRateSelector(_x) (((_x) & 0x7) << 3)
#define   NFP3800_NBI_TMX_CSR_ShaperRateSelector_L1ShaperRateSelector_of(_x) (((_x) >> 3) & 0x7)
#define     NFP3800_NBI_TMX_CSR_ShaperRateSelector_L1ShaperRateSelector_Default (0)
#define     NFP3800_NBI_TMX_CSR_ShaperRateSelector_L1ShaperRateSelector_Divide_by_2 (1)
#define     NFP3800_NBI_TMX_CSR_ShaperRateSelector_L1ShaperRateSelector_Divide_by_4 (2)
#define     NFP3800_NBI_TMX_CSR_ShaperRateSelector_L1ShaperRateSelector_Divide_by_8 (3)
#define     NFP3800_NBI_TMX_CSR_ShaperRateSelector_L1ShaperRateSelector_Divide_by_16 (4)
#define     NFP3800_NBI_TMX_CSR_ShaperRateSelector_L1ShaperRateSelector_Divide_by_128 (5)
#define     NFP3800_NBI_TMX_CSR_ShaperRateSelector_L1ShaperRateSelector_Divide_by_512 (6)
#define     NFP3800_NBI_TMX_CSR_ShaperRateSelector_L1ShaperRateSelector_Divide_by_1024 (7)
#define   NFP3800_NBI_TMX_CSR_ShaperRateSelector_L0ShaperRateSelector(_x) (((_x) & 0x7) << 0)
#define   NFP3800_NBI_TMX_CSR_ShaperRateSelector_L0ShaperRateSelector_of(_x) (((_x) >> 0) & 0x7)
#define     NFP3800_NBI_TMX_CSR_ShaperRateSelector_L0ShaperRateSelector_Default (0)
#define     NFP3800_NBI_TMX_CSR_ShaperRateSelector_L0ShaperRateSelector_Divide_by_2 (1)
#define     NFP3800_NBI_TMX_CSR_ShaperRateSelector_L0ShaperRateSelector_Divide_by_4 (2)
#define     NFP3800_NBI_TMX_CSR_ShaperRateSelector_L0ShaperRateSelector_Divide_by_8 (3)
#define     NFP3800_NBI_TMX_CSR_ShaperRateSelector_L0ShaperRateSelector_Divide_by_16 (4)
#define     NFP3800_NBI_TMX_CSR_ShaperRateSelector_L0ShaperRateSelector_Divide_by_128 (5)
#define     NFP3800_NBI_TMX_CSR_ShaperRateSelector_L0ShaperRateSelector_Divide_by_512 (6)
#define     NFP3800_NBI_TMX_CSR_ShaperRateSelector_L0ShaperRateSelector_Divide_by_1024 (7)
/*----------------------------------------------------------------
  Register: QueueStatus[1024] - Status information stored for each queue. There are a total of 1024 queue status registers in the traffic manager.
    Bits: 13    QueueFull(ro) - Queue Full register bit indicating the Descriptor Queue is Full or Almost Full
    Bits: 12:0  QueueLevel(ro) - Queue level register indicating the fill level for this particular queue. One register for each descriptor queue.
*/
#define NFP3800_NBI_TM_Q_QueueStatus(_x)                     (0x00000000 + (0x4 * ((_x) & 0x3ff)))
#define   NFP3800_NBI_TM_Q_QueueStatus_QueueFull             BIT(13)
#define   NFP3800_NBI_TM_Q_QueueStatus_QueueLevel(_x)        (((_x) & 0x1fff) << 0)
#define   NFP3800_NBI_TM_Q_QueueStatus_QueueLevel_of(_x)     (((_x) >> 0) & 0x1fff)
/*----------------------------------------------------------------
  Register: QueueConfig[1024] - Configuration settings for each queue. There are a total of 1024 queue configuration registers in the traffic manager.
    Bits: 11    DropPrecedenceEnable(rw) - Enable the Drop Precedence feature for this queue. When enabled, the drop precedence bits in the packet descriptor will select the drop profile to be used. When disabled the static drop rate range sets A or B will be used.
    Bits: 10    DropRateRangeSelect(rw) - Select the Drop Rate Range set to use for this queue. There are two range sets A and B that may be used and are selectable per queue.
    Bits:  9:6  QueueSize(rw) - Configures the maximum size of this particular queue. If linked list is enabled, the recommended values are 1024 or higher. (TBD: requires validation in silicon)
    Bits:  2    REDEnable(rw) - Enables random early detection for packet dropping for this particular queue. Packets will be discarded for this queue based on the globally configured drop profiles. The queue is divided into four ranges, the drop percentage is configurable for each range.
    Bits:  1    DropEnable(rw) - Enables packet dropping for this particular queue. Packets will be discarded for this queue when it is full or almost full. DropEnable must be set to 1 by user.
    Bits:  0    QueueEnable(rw) - Enable bit for this particular queue. Set this bit to enable the queue, clear to disable.
*/
#define NFP3800_NBI_TM_Q_QueueConfig(_x)                     (0x00001000 + (0x4 * ((_x) & 0x3ff)))
#define   NFP3800_NBI_TM_Q_QueueConfig_DropPrecedenceEnable  BIT(11)
#define     NFP3800_NBI_TM_Q_QueueConfig_DropPrecedenceEnable_Disable (0 << 11)
#define     NFP3800_NBI_TM_Q_QueueConfig_DropPrecedenceEnable_Enable BIT(11)
#define   NFP3800_NBI_TM_Q_QueueConfig_DropRateRangeSelect   BIT(10)
#define     NFP3800_NBI_TM_Q_QueueConfig_DropRateRangeSelect_A (0 << 10)
#define     NFP3800_NBI_TM_Q_QueueConfig_DropRateRangeSelect_B BIT(10)
#define   NFP3800_NBI_TM_Q_QueueConfig_QueueSize(_x)         (((_x) & 0xf) << 6)
#define   NFP3800_NBI_TM_Q_QueueConfig_QueueSize_of(_x)      (((_x) >> 6) & 0xf)
#define     NFP3800_NBI_TM_Q_QueueConfig_QueueSize_8         (3)
#define     NFP3800_NBI_TM_Q_QueueConfig_QueueSize_16        (4)
#define     NFP3800_NBI_TM_Q_QueueConfig_QueueSize_32        (5)
#define     NFP3800_NBI_TM_Q_QueueConfig_QueueSize_64        (6)
#define     NFP3800_NBI_TM_Q_QueueConfig_QueueSize_128       (7)
#define     NFP3800_NBI_TM_Q_QueueConfig_QueueSize_256       (8)
#define     NFP3800_NBI_TM_Q_QueueConfig_QueueSize_512       (9)
#define     NFP3800_NBI_TM_Q_QueueConfig_QueueSize_1024      (10)
#define     NFP3800_NBI_TM_Q_QueueConfig_QueueSize_2048      (11)
#define     NFP3800_NBI_TM_Q_QueueConfig_QueueSize_4096      (12)
#define   NFP3800_NBI_TM_Q_QueueConfig_REDEnable             BIT(2)
#define     NFP3800_NBI_TM_Q_QueueConfig_REDEnable_Disabled  (0 << 2)
#define     NFP3800_NBI_TM_Q_QueueConfig_REDEnable_Enabled   BIT(2)
#define   NFP3800_NBI_TM_Q_QueueConfig_DropEnable            BIT(1)
#define     NFP3800_NBI_TM_Q_QueueConfig_DropEnable_Disabled (0 << 1)
#define     NFP3800_NBI_TM_Q_QueueConfig_DropEnable_Enabled  BIT(1)
#define   NFP3800_NBI_TM_Q_QueueConfig_QueueEnable           BIT(0)
#define     NFP3800_NBI_TM_Q_QueueConfig_QueueEnable_Disable (0 << 0)
#define     NFP3800_NBI_TM_Q_QueueConfig_QueueEnable_Enable  BIT(0)
/*----------------------------------------------------------------
  Register: QueueDropCount[1024] - Drop counter for each queue. There are a total of 1024 drop counters in the traffic manager.
    Bits: 31:0  DropCount(ro) - 32-bit drop counter to keep track of the number of packets dropped for this queue. The counter will saturate when the maximum number is reached.
*/
#define NFP3800_NBI_TM_Q_QueueDropCount(_x)                  (0x00002000 + (0x4 * ((_x) & 0x3ff)))
#define   NFP3800_NBI_TM_Q_QueueDropCount_DropCount(_x)      (((_x) & 0xffffffff) << 0)
#define   NFP3800_NBI_TM_Q_QueueDropCount_DropCount_of(_x)   (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: QueueDropCountClear[1024] - Clear on read drop counter for each queue. There are a total of 1024 clear on read drop counters in the traffic manager.
    Bits: 31:0  DropCountClear(rc) - Clear on read 32-bit drop counter to keep track of the number of packets dropped for this queue. The counter will saturate when the maximum number is reached. The read value for this register is identical to the DROPCOUNT register value, however when DROPCOUNTCLEAR is read the drop counter is cleared. The drop count memory is undefined at start-up. This register should be read for all active queues by software prior to device operation.
*/
#define NFP3800_NBI_TM_Q_QueueDropCountClear(_x)             (0x00003000 + (0x4 * ((_x) & 0x3ff)))
#define   NFP3800_NBI_TM_Q_QueueDropCountClear_DropCountClear(_x) (((_x) & 0xffffffff) << 0)
#define   NFP3800_NBI_TM_Q_QueueDropCountClear_DropCountClear_of(_x) (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: PacketDescriptorMemoryStatus - Packet Descriptor Memory Status.
    Bits: 14    PacketDescriptorMemoryFull(ro) - Packet Descriptor Memory Full register bit indicating the Descriptor Memory is Full or Almost Full
    Bits: 12:0  PacketDescriptorMemoryLevel(ro) - Packet Descriptor Memory level register indicating the fill level.
*/
#define NFP3800_NBI_TM_Q_PacketDescriptorMemoryStatus        0x00004000
#define   NFP3800_NBI_TM_Q_PacketDescriptorMemoryStatus_PacketDescriptorMemoryFull BIT(14)
#define   NFP3800_NBI_TM_Q_PacketDescriptorMemoryStatus_PacketDescriptorMemoryLevel(_x) (((_x) & 0x1fff) << 0)
#define   NFP3800_NBI_TM_Q_PacketDescriptorMemoryStatus_PacketDescriptorMemoryLevel_of(_x) (((_x) >> 0) & 0x1fff)
/*----------------------------------------------------------------
  Register: CommandDropStatus - Clear on read status register indicating that an outgoing DSF/CPP command has been dropped due to the command fifo being full for 1024 clock cycles.This register is only set once and only set again after the register is read
    Bits: 31:28 CommandDropCTMID(rc) - Read only register indicating the CTM Number for the command that was dropped.
    Bits: 27:22 CommandDropCTMIsland(rc) - Read only register indicating the CTM Island Number for the command that was dropped.
    Bits: 21:12 CommandDropQueueNumber(rc) - Read only register indicating the intended Queue Number for the command that was dropped.
    Bits: 11:3  CommandDropPacketNumber(rc) - Read only register indicating the intended Packet Number for the command that was dropped.
    Bits:  1    SecondTimeFullFIFO(ro) - Bit indicating that either a FreePkt/PPC command was dropped due to FIFO being full. - only gets cleared on reset
    Bits:  0    FirstTimeFullFIFO(ro) - Bit indicating that either a FreePkt/PPC command was dropped due to FIFO being full. - only gets cleared on reset
*/
#define NFP3800_NBI_TM_Q_CommandDropStatus                   0x00004004
#define   NFP3800_NBI_TM_Q_CommandDropStatus_CommandDropCTMID(_x) (((_x) & 0xf) << 28)
#define   NFP3800_NBI_TM_Q_CommandDropStatus_CommandDropCTMID_of(_x) (((_x) >> 28) & 0xf)
#define   NFP3800_NBI_TM_Q_CommandDropStatus_CommandDropCTMIsland(_x) (((_x) & 0x3f) << 22)
#define   NFP3800_NBI_TM_Q_CommandDropStatus_CommandDropCTMIsland_of(_x) (((_x) >> 22) & 0x3f)
#define   NFP3800_NBI_TM_Q_CommandDropStatus_CommandDropQueueNumber(_x) (((_x) & 0x3ff) << 12)
#define   NFP3800_NBI_TM_Q_CommandDropStatus_CommandDropQueueNumber_of(_x) (((_x) >> 12) & 0x3ff)
#define   NFP3800_NBI_TM_Q_CommandDropStatus_CommandDropPacketNumber(_x) (((_x) & 0x1ff) << 3)
#define   NFP3800_NBI_TM_Q_CommandDropStatus_CommandDropPacketNumber_of(_x) (((_x) >> 3) & 0x1ff)
#define   NFP3800_NBI_TM_Q_CommandDropStatus_SecondTimeFullFIFO BIT(1)
#define     NFP3800_NBI_TM_Q_CommandDropStatus_SecondTimeFullFIFO_No_Drop (0 << 1)
#define     NFP3800_NBI_TM_Q_CommandDropStatus_SecondTimeFullFIFO_Drop BIT(1)
#define   NFP3800_NBI_TM_Q_CommandDropStatus_FirstTimeFullFIFO BIT(0)
#define     NFP3800_NBI_TM_Q_CommandDropStatus_FirstTimeFullFIFO_No_Drop (0 << 0)
#define     NFP3800_NBI_TM_Q_CommandDropStatus_FirstTimeFullFIFO_Drop BIT(0)
/*----------------------------------------------------------------
  Register: PCIeCommandDropStatus - Clear on read status register indicating that an outgoing DSF/CPP command has been dropped due to the command fifo being full for 1024 clock cycles.This register is only set once and only set again after the register is read
    Bits: 31:28 CommandDropLookupAddress(rc) - Read only register indicating the Lookup Address for the command that was dropped.
    Bits: 27:22 CommandDropCTMIsland(rc) - Read only register indicating the CTM Island Number for the command that was dropped.
    Bits:  9:5  CommandDropSequencerNumber(rc) - Read only register indicating the Sequence Number for the command that was dropped.
    Bits:  1    SecondTimeFullFIFO(ro) - Bit indicating that ReorderOnly/Reorder with VM-QoS command was dropped due to FIFO being full. - only gets cleared on reset
    Bits:  0    FirstTimeFullFIFO(ro) - Bit indicating that ReorderOnly/Reorder with VM-QoS command was dropped due to FIFO being full. - only gets cleared on reset
*/
#define NFP3800_NBI_TM_Q_PCIeCommandDropStatus               0x00004008
#define   NFP3800_NBI_TM_Q_PCIeCommandDropStatus_CommandDropLookupAddress(_x) (((_x) & 0xf) << 28)
#define   NFP3800_NBI_TM_Q_PCIeCommandDropStatus_CommandDropLookupAddress_of(_x) (((_x) >> 28) & 0xf)
#define   NFP3800_NBI_TM_Q_PCIeCommandDropStatus_CommandDropCTMIsland(_x) (((_x) & 0x3f) << 22)
#define   NFP3800_NBI_TM_Q_PCIeCommandDropStatus_CommandDropCTMIsland_of(_x) (((_x) >> 22) & 0x3f)
#define   NFP3800_NBI_TM_Q_PCIeCommandDropStatus_CommandDropSequencerNumber(_x) (((_x) & 0x1f) << 5)
#define   NFP3800_NBI_TM_Q_PCIeCommandDropStatus_CommandDropSequencerNumber_of(_x) (((_x) >> 5) & 0x1f)
#define   NFP3800_NBI_TM_Q_PCIeCommandDropStatus_SecondTimeFullFIFO BIT(1)
#define     NFP3800_NBI_TM_Q_PCIeCommandDropStatus_SecondTimeFullFIFO_No_Drop (0 << 1)
#define     NFP3800_NBI_TM_Q_PCIeCommandDropStatus_SecondTimeFullFIFO_Drop BIT(1)
#define   NFP3800_NBI_TM_Q_PCIeCommandDropStatus_FirstTimeFullFIFO BIT(0)
#define     NFP3800_NBI_TM_Q_PCIeCommandDropStatus_FirstTimeFullFIFO_No_Drop (0 << 0)
#define     NFP3800_NBI_TM_Q_PCIeCommandDropStatus_FirstTimeFullFIFO_Drop BIT(0)
/*----------------------------------------------------------------
  Register: SchedulerConfig[145] - Configuration register for setup of each scheduler. The level 2 schedulers are at offsets 0-127, the level 1 standard schedulers are at offsets 128-143, and the level 0 scheduler is at offset 144.
    Bits:  2:1  SPEnable(rw) - Strict Priority Enable bits. Configure the scheduler for Strict Priority operation.
    Bits:  0    DWRREnable(rw) - Deficit Weighted Round Robin Enable bit. Set this bit to enable deficit weighted round operation for the particular scheduler.
*/
#define NFP3800_NBI_TMX_SCH_CSR_SchedulerConfig(_x)          (0x00000000 + (0x4 * ((_x) & 0xff)))
#define   NFP3800_NBI_TMX_SCH_CSR_SchedulerConfig_SPEnable(_x) (((_x) & 0x3) << 1)
#define   NFP3800_NBI_TMX_SCH_CSR_SchedulerConfig_SPEnable_of(_x) (((_x) >> 1) & 0x3)
#define     NFP3800_NBI_TMX_SCH_CSR_SchedulerConfig_SPEnable_0 (0)
#define     NFP3800_NBI_TMX_SCH_CSR_SchedulerConfig_SPEnable_1 (1)
#define     NFP3800_NBI_TMX_SCH_CSR_SchedulerConfig_SPEnable_2 (2)
#define     NFP3800_NBI_TMX_SCH_CSR_SchedulerConfig_SPEnable_3 (3)
#define   NFP3800_NBI_TMX_SCH_CSR_SchedulerConfig_DWRREnable BIT(0)
#define     NFP3800_NBI_TMX_SCH_CSR_SchedulerConfig_DWRREnable_Disable (0 << 0)
#define     NFP3800_NBI_TMX_SCH_CSR_SchedulerConfig_DWRREnable_Enable BIT(0)
/*----------------------------------------------------------------
  Register: L1ExtSchedulerConfig[16] - Level 1 Extended Scheduler Configuration register. These registers configure the 16 level 1 extended schedulers.
    Bits:  3    L1ExtSchedEnable(rw) - Enable the extended level 1 scheduler. Each group of 8 L2 scheduler outputs may connect to either the base or extended L1 scheduler inputs with the following options
    Bits:  2:1  L1ExtSPEnable(rw) - Strict Priority Enable bits. Configure the extended scheduler for Strict Priority operation.
    Bits:  0    L1ExtDWRREnable(rw) - Deficit Weighted Round Robin Enable bit. Set this bit to enable dwrr operation for the extended scheduler.
*/
#define NFP3800_NBI_TMX_SCH_CSR_L1ExtSchedulerConfig(_x)     (0x00000400 + (0x4 * ((_x) & 0xf)))
#define   NFP3800_NBI_TMX_SCH_CSR_L1ExtSchedulerConfig_L1ExtSchedEnable BIT(3)
#define     NFP3800_NBI_TMX_SCH_CSR_L1ExtSchedulerConfig_L1ExtSchedEnable_0 (0 << 3)
#define     NFP3800_NBI_TMX_SCH_CSR_L1ExtSchedulerConfig_L1ExtSchedEnable_1 BIT(3)
#define   NFP3800_NBI_TMX_SCH_CSR_L1ExtSchedulerConfig_L1ExtSPEnable(_x) (((_x) & 0x3) << 1)
#define   NFP3800_NBI_TMX_SCH_CSR_L1ExtSchedulerConfig_L1ExtSPEnable_of(_x) (((_x) >> 1) & 0x3)
#define     NFP3800_NBI_TMX_SCH_CSR_L1ExtSchedulerConfig_L1ExtSPEnable_0 (0)
#define     NFP3800_NBI_TMX_SCH_CSR_L1ExtSchedulerConfig_L1ExtSPEnable_1 (1)
#define     NFP3800_NBI_TMX_SCH_CSR_L1ExtSchedulerConfig_L1ExtSPEnable_2 (2)
#define     NFP3800_NBI_TMX_SCH_CSR_L1ExtSchedulerConfig_L1ExtSPEnable_3 (3)
#define   NFP3800_NBI_TMX_SCH_CSR_L1ExtSchedulerConfig_L1ExtDWRREnable BIT(0)
#define     NFP3800_NBI_TMX_SCH_CSR_L1ExtSchedulerConfig_L1ExtDWRREnable_Disable (0 << 0)
#define     NFP3800_NBI_TMX_SCH_CSR_L1ExtSchedulerConfig_L1ExtDWRREnable_Enable BIT(0)
/*----------------------------------------------------------------
  Register: SchedulerWeight[128] - L0 Scheduler weight value to configure the relative bandwidth for each scheduler port. There are a total of 128 weight values to program in the level 0 scheduler.
    Bits: 23:0  Weight(rw) - Scheduler weight value. Program this memory with the desired weight value for the particular scheduler port. This memory is undefined at start-up and should be programmed or set to zero by software. If all weights for all ports of a particular port are set to zero, the scheduler will operate in round robin fashion. The weight value will configure the relative bandwidth for the port and is used to update the deficit value. The weight should be programmed to the desired relative bandwidth for this port as compared to the other ports on this scheduler.
*/
#define NFP3800_NBI_TMX_SCH_CSR_SchedulerWeight(_x)          (0x00000800 + (0x4 * ((_x) & 0x7f)))
#define   NFP3800_NBI_TMX_SCH_CSR_SchedulerWeight_Weight(_x) (((_x) & 0xffffff) << 0)
#define   NFP3800_NBI_TMX_SCH_CSR_SchedulerWeight_Weight_of(_x) (((_x) >> 0) & 0xffffff)
/*----------------------------------------------------------------
  Register: SchedulerDeficit[128] - L0 Scheduler deficit value for each scheduler port. There are a total of 128 deficit values in the level 0 scheduler.
    Bits: 24:0  Deficit(rw) - Scheduler deficit value. This memory provides the signed value of the deficit for the particular scheduler port. This memory is undefined at start-up and should be programmed to zero by software. If all of the ports either have a negative deficit or do not have data to send, the deficit values will be updated for all ports. During the update if the port does not have data to send the deficit will be set to the weight value. If the deficit has gone negative, the weight value will be added to the current deficit.
*/
#define NFP3800_NBI_TMX_SCH_CSR_SchedulerDeficit(_x)         (0x00001000 + (0x4 * ((_x) & 0x7f)))
#define   NFP3800_NBI_TMX_SCH_CSR_SchedulerDeficit_Deficit(_x) (((_x) & 0x1ffffff) << 0)
#define   NFP3800_NBI_TMX_SCH_CSR_SchedulerDeficit_Deficit_of(_x) (((_x) >> 0) & 0x1ffffff)
/*----------------------------------------------------------------
  Register: ShaperRate[145] - Shaper rate register. Configures the maximum allowed rate. The level 2 shaper registers are at offsets 0-127, the level 1 shaper registers are at offsets 128-143, and the level 0 shaper register is at offset 144.
  Register: L1ExtShaperRate[16] - Level 1 extended shaper rate register. Configures the maximum allowed rate for the 16 Level 1 extended shapers.
    Bits: 13:0  Rate(rw) - The shaper rate register. Valid values may range in size from 1 to 12,000 and correspond to data rates of 8Mbps to 96Gbps when using a 800MHz PCLK. Values outside that range are not supported (i.e. 0, or above 12,000). There are configuration bits in TrafficManagerConfig[2;18] which is used as a divider for the clk frequency used for ShaperRate calculation. TrafficManagerConfig[2;18] set to 0: ShaperRate 8Mbps -> 96Gbps, 1: 4Mbps -> 48Gbps, 2: 2Mbps -> 24Gbps, 4: 1Mbps -> 12Gbps
*/
#define NFP3800_NBI_TMX_SHP_ShaperRate(_x)                   (0x00000000 + (0x4 * ((_x) & 0xff)))
#define NFP3800_NBI_TMX_SHP_L1ExtShaperRate(_x)              (0x00000300 + (0x4 * ((_x) & 0xf)))
#define   NFP3800_NBI_TMX_SHP_ShaperRate_Rate(_x)            (((_x) & 0x3fff) << 0)
#define   NFP3800_NBI_TMX_SHP_ShaperRate_Rate_of(_x)         (((_x) >> 0) & 0x3fff)
/*----------------------------------------------------------------
  Register: ShaperThreshold[145] - Rate limiting shaper threshold register. Configures the desired threshold level of the leaky bucket. The level 2 shaper registers are at offsets 0-127, the level 1 shaper registers are at offsets 128-143, and the level 0 shaper register is at offset 144.
  Register: L1ExtShaperThreshold[16] - Level 1 extended shaper threshold register. Configures the desired threshold level of the leaky bucket for the 16 level 1 extended shapers.
    Bits:  2:0  Threshold(rw) - The rate limiting shaper threshold register. This register configures the threshold in bytes used for comparison to the instantaneous level of the leaky bucket. When the number of bytes sent is below this threshold the shaper will be open, when the bucket level equals or exceeds the threshold the shaper will be closed.
*/
#define NFP3800_NBI_TMX_SHP_ShaperThreshold(_x)              (0x00000800 + (0x4 * ((_x) & 0xff)))
#define NFP3800_NBI_TMX_SHP_L1ExtShaperThreshold(_x)         (0x00000b00 + (0x4 * ((_x) & 0xf)))
#define   NFP3800_NBI_TMX_SHP_ShaperThreshold_Threshold(_x)  (((_x) & 0x7) << 0)
#define   NFP3800_NBI_TMX_SHP_ShaperThreshold_Threshold_of(_x) (((_x) >> 0) & 0x7)
#define     NFP3800_NBI_TMX_SHP_ShaperThreshold_Threshold_8192 (0)
#define     NFP3800_NBI_TMX_SHP_ShaperThreshold_Threshold_16384 (1)
#define     NFP3800_NBI_TMX_SHP_ShaperThreshold_Threshold_24576 (2)
#define     NFP3800_NBI_TMX_SHP_ShaperThreshold_Threshold_32768 (3)
#define     NFP3800_NBI_TMX_SHP_ShaperThreshold_Threshold_40960 (4)
#define     NFP3800_NBI_TMX_SHP_ShaperThreshold_Threshold_49152 (5)
#define     NFP3800_NBI_TMX_SHP_ShaperThreshold_Threshold_57344 (6)
#define     NFP3800_NBI_TMX_SHP_ShaperThreshold_Threshold_65535 (7)
/*----------------------------------------------------------------
  Register: ShaperMaxOvershoot[145] - Rate limiting shaper maximum overshoot register. Configures the maximum allowed overshoot for the leaky bucket. The level 2 shaper registers are at offsets 0-127, the level 1 shaper registers are at offsets 128-143, and the level 0 shaper register is at offset 144
  Register: L1ExtShaperMaxOvershoot[16] - Level 1 extended shaper maximum overshoot register. Configures the maximum allowed overshoot of the leaky bucket for the 16 level 1 extended shapers.
    Bits:  2:0  MaxOvershoot(rw) - The rate limiting shaper maximum overshoot register. This register configures the maximum allowed overshoot in bytes for the leaky bucket. Data that is sent beyond this limit will not be accounted for in the leaky bucket algorithm.
*/
#define NFP3800_NBI_TMX_SHP_ShaperMaxOvershoot(_x)           (0x00001000 + (0x4 * ((_x) & 0xff)))
#define NFP3800_NBI_TMX_SHP_L1ExtShaperMaxOvershoot(_x)      (0x00001300 + (0x4 * ((_x) & 0xf)))
#define   NFP3800_NBI_TMX_SHP_ShaperMaxOvershoot_MaxOvershoot(_x) (((_x) & 0x7) << 0)
#define   NFP3800_NBI_TMX_SHP_ShaperMaxOvershoot_MaxOvershoot_of(_x) (((_x) >> 0) & 0x7)
#define     NFP3800_NBI_TMX_SHP_ShaperMaxOvershoot_MaxOvershoot_16384 (0)
#define     NFP3800_NBI_TMX_SHP_ShaperMaxOvershoot_MaxOvershoot_24576 (1)
#define     NFP3800_NBI_TMX_SHP_ShaperMaxOvershoot_MaxOvershoot_32768 (2)
#define     NFP3800_NBI_TMX_SHP_ShaperMaxOvershoot_MaxOvershoot_40960 (3)
#define     NFP3800_NBI_TMX_SHP_ShaperMaxOvershoot_MaxOvershoot_49152 (4)
#define     NFP3800_NBI_TMX_SHP_ShaperMaxOvershoot_MaxOvershoot_57344 (5)
#define     NFP3800_NBI_TMX_SHP_ShaperMaxOvershoot_MaxOvershoot_65536 (6)
#define     NFP3800_NBI_TMX_SHP_ShaperMaxOvershoot_MaxOvershoot_131071 (7)
/*----------------------------------------------------------------
  Register: ShaperRateAdjust[145] - Shaper rate adjust register. Configures the fixed data rate adjustment for packet overhead. The level 2 shaper registers are at offsets 0-127, the level 1 shaper registers are at offsets 128-143, and the level 0 shaper register is at offset 144.
  Register: L1ExtShaperRateAdjust[16] - Level 1 extended shaper rate adjust register. Configures the fixed data rate adjustment for packet overheadfor the 16 level 1 extended shapers.
    Bits:  9:0  RateAdjust(rw) - The shaper rate adjust register. This register contains the standard number of bytes to be subtracted for every packet that is transmitted. This can acount for packet overhead or interpacket gap not to be included in the shaper rate calculation. In cases where the rate adjust value is larger than the packet size, the difference will be deducted from the shaper bucket. Positive values will subtract bytes and negative values will add bytes, program in twos compliment format -512 to 511. Negative values will limit the Max Packet size supported to 15,838 bytes
*/
#define NFP3800_NBI_TMX_SHP_ShaperRateAdjust(_x)             (0x00001800 + (0x4 * ((_x) & 0xff)))
#define NFP3800_NBI_TMX_SHP_L1ExtShaperRateAdjust(_x)        (0x00001b00 + (0x4 * ((_x) & 0xf)))
#define   NFP3800_NBI_TMX_SHP_ShaperRateAdjust_RateAdjust(_x) (((_x) & 0x3ff) << 0)
#define   NFP3800_NBI_TMX_SHP_ShaperRateAdjust_RateAdjust_of(_x) (((_x) >> 0) & 0x3ff)
/*----------------------------------------------------------------
  Register: ShaperStatus[145] - Shaper status register. Contains read-only status information for the rate liming shaper. The level 2 shaper registers are at offsets 0-127, the level 1 shaper registers are at offsets 128-143, and the level 0 shaper register is at offset 144.
  Register: L1ExtShaperStatus[16] - Level 1 extended shaper status register. Contains read-only status information for the 16 level 1 extended shapers.
    Bits: 17    ShaperOpen(ro) - Indicates the state of the leaky bucket
    Bits: 16:0  Level(ro) - The current leaky bucket level of the rate limiting shaper. This register contains the present level of the leaky bucket that is used to determine open or closed state. If this value is below the threshold the shaper will be open, if it is above the threshold the shaper will be closed.
*/
#define NFP3800_NBI_TMX_SHP_ShaperStatus(_x)                 (0x00002000 + (0x4 * ((_x) & 0xff)))
#define NFP3800_NBI_TMX_SHP_L1ExtShaperStatus(_x)            (0x00002300 + (0x4 * ((_x) & 0xf)))
#define   NFP3800_NBI_TMX_SHP_ShaperStatus_ShaperOpen        BIT(17)
#define     NFP3800_NBI_TMX_SHP_ShaperStatus_ShaperOpen_Closed (0 << 17)
#define     NFP3800_NBI_TMX_SHP_ShaperStatus_ShaperOpen_Open BIT(17)
#define   NFP3800_NBI_TMX_SHP_ShaperStatus_Level(_x)         (((_x) & 0x1ffff) << 0)
#define   NFP3800_NBI_TMX_SHP_ShaperStatus_Level_of(_x)      (((_x) >> 0) & 0x1ffff)

#endif /* NFP3800_LONGNAMES */


#endif /* NFP3800_NBI_TM_H */
