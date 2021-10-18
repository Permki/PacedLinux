/*
 * Copyright (C) 2013, Netronome Systems, Inc.  All rights reserved.
 *
 * Disclaimer: this file is provided without any warranty
 * as part of an early-access program, and the content is
 * bound to change before the final release.
 */

#ifndef NFP6000_NBI_TM_H
#define NFP6000_NBI_TM_H

/*================================================================*/
#include "nfp_nbi.h"

/*----------------------------------------------------------------*/
/* CPP Address Map (CPP_BASE = IMB translation: island + indexes) */

/* CPP_BASE + NFP_NBI_TM + NFP_NBI_TM_* */
#define NFP_NBI_TM                                           (0x200000)

/*----------------------------------------------------------------*/
/* XPB Address Map (XPB_BASE = NFP_XPB_ISLAND(island))          */

#define NFP_NBI_TMX                                          (NBIX_BASE + 0x040000)

/* XPB_BASE + NFP_NBI_TMX_CSR + NFP_NBI_TMX_CSR_* */
#define NFP_NBI_TMX_CSR                                      (NFP_NBI_TMX + 0x00000)
/* XPB_BASE + NFP_NBI_TMX_Q + NFP_NBI_TMX_Q_* */
#define NFP_NBI_TMX_Q                                        (NFP_NBI_TMX + 0x10000)
/* XPB_BASE + NFP_NBI_TMX_SCH_CSR + NFP_NBI_TMX_SCH_CSR_* */
#define NFP_NBI_TMX_SCH_CSR                                  (NFP_NBI_TMX + 0x20000)
/* XPB_BASE + NFP_NBI_TMX_SCH_L1WGT + NFP_NBI_TMX_SCH_WGT_* */
#define NFP_NBI_TMX_SCH_L1WGT                                (NFP_NBI_TMX + 0x30000)
/* XPB_BASE + NFP_NBI_TMX_SCH_L1DFT + NFP_NBI_TMX_SCH_DFT_* */
#define NFP_NBI_TMX_SCH_L1DFT                                (NFP_NBI_TMX + 0x40000)
/* XPB_BASE + NFP_NBI_TMX_SCH_L1_ECC + (nfp_ecc.h) */
#define NFP_NBI_TMX_SCH_L1_ECC                               (NFP_NBI_TMX + 0x50000)
/* XPB_BASE + NFP_NBI_TMX_SCH_L2WGT + NFP_NBI_TMX_SCH_WGT_* */
#define NFP_NBI_TMX_SCH_L2WGT                                (NFP_NBI_TMX + 0x60000)
/* XPB_BASE + NFP_NBI_TMX_SCH_L2DFT + NFP_NBI_TMX_SCH_DFT_* */
#define NFP_NBI_TMX_SCH_L2DFT                                (NFP_NBI_TMX + 0x70000)
/* XPB_BASE + NFP_NBI_TMX_SCH_L2_ECC + (nfp_ecc.h) */
#define NFP_NBI_TMX_SCH_L2_ECC                               (NFP_NBI_TMX + 0x80000)
/* XPB_BASE + NFP_NBI_TMX_SHP + NFP_NBI_TMX_SHP_* */
#define NFP_NBI_TMX_SHP                                      (NFP_NBI_TMX + 0x90000)
/* XPB_BASE + NFP_NBI_TMX_DESCMEM_ECC + (nfp_ecc.h) */
#define NFP_NBI_TMX_DESCMEM_ECC                              (NFP_NBI_TMX + 0xa0000)
/* XPB_BASE + NFP_NBI_TMX_HDTLMEM_ECC + (nfp_ecc.h) */
#define NFP_NBI_TMX_HDTLMEM_ECC                              (NFP_NBI_TMX + 0xb0000)
/* XPB_BASE + NFP_NBI_TMX_BLQMEM_ECC + (nfp_ecc.h) */
#define NFP_NBI_TMX_BLQMEM_ECC                               (NFP_NBI_TMX + 0xc0000)
/* XPB_BASE + NFP_NBI_TMX_EGPKT_ECC + (nfp_ecc.h) */
#define NFP_NBI_TMX_EGPKT_ECC                                (NFP_NBI_TMX + 0xd0000)
/* XPB_BASE + NFP_NBI_TMX_SLOWDMEM_ECC + (nfp_ecc.h) */
#define NFP_NBI_TMX_SLOWDMEM_ECC                             (NFP_NBI_TMX + 0xe0000)
/* XPB_BASE + NFP_NBI_TMX_REORDERMEM_ECC + (nfp_ecc.h) */
#define NFP_NBI_TMX_REORDERMEM_ECC                           (NFP_NBI_TMX + 0xf0000)
/* XPB_BASE + NFP_NBI_TMX_EGDMASTATE_ECC + (nfp_ecc.h) */
#define NFP_NBI_TMX_EGDMASTATE_ECC                           (NFP_NBI_TMX + 0x100000)
/* XPB_BASE + NFP_NBI_TMX_DROPCNTMEM_ECC + (nfp_ecc.h) */
#define NFP_NBI_TMX_DROPCNTMEM_ECC                           (NFP_NBI_TMX + 0x110000)

/*================================================================*/


/* HGID: 60515d2e075a */

#ifndef NFP6000_LONGNAMES

/*----------------------------------------------------------------
  Register: TMDescSram_TMDescSramEntry[32768] - NBI TM Descriptor SRAM Entry
    Bits: 63:0  DescSRAMData(rw) - 64-bits of the 128-bit descriptor in the TM Descriptor SRAM. Refer to the Packet Descriptor Details table for details on bit definitions
*/
#define NFP_NBI_TM_TMDescSram_TM_DESC_ENTRY(_x)              (0x00000000 + (0x8 * ((_x) & 0x7fff)))
#define   NFP_NBI_TM_TMDESCSRAM_TM_DESC_ENTRY_DESC_SRAM_DATA(_x) (((_x) & 0xffffffffffffffff) << 0)
#define   NFP_NBI_TM_TMDESCSRAM_TM_DESC_ENTRY_DESC_SRAM_DATA_of(_x) (((_x) >> 0) & 0xffffffffffffffff)
/*----------------------------------------------------------------
  Register: TMBDSram_NbiBDSramEntry[4096] - NBI TM Buffer Descriptor SRAM Entry
    Bits: 28:0  MuAddr(rw) - Memory Unit Address of the buffer (internal/external MU). Maps to CPP Address[29;11] Refer to the NBI top-level Master Commands section for memory unit address translation.
*/
#define NFP_NBI_TM_TMBDSram_TM_BDSRAM_ENTRY(_x)              (0x00040000 + (0x8 * ((_x) & 0xfff)))
#define   NFP_NBI_TM_TMBDSRAM_TM_BDSRAM_ENTRY_MUADDR(_x)     (((_x) & 0x1fffffff) << 0)
#define   NFP_NBI_TM_TMBDSRAM_TM_BDSRAM_ENTRY_MUADDR_of(_x)  (((_x) >> 0) & 0x1fffffff)
/*----------------------------------------------------------------
  Register: TMBufferList_BLQueCtrl0 - Configures the buffer list circular queue and provides status
    Bits: 55:48 BLQEventFullCnt(rc) - Debug counter that increments when a BLQ Event is dropped due to a full BLQ Event FIFO. This counter saturates at all 1's
    Bits: 42:40 BDFifoEntryCnt(ro) - Number of buffer descriptors resident in the buffer descriptor register FIFO
    Bits: 39    BLQFull(ro) - The Queue is full when set.
    Bits: 38    BLQEmpty(ro) - The queue is empty when set.
    Bits: 37:36 BLQSize(rw) - Defines the Size of the Circular Buffer List Queue
    Bits: 35:24 BLEntryCnt(ro) - Hardware calculated value of the number of entries on the Buffer List Queue. A full Buffer List Queue has an entry count equal to zero, with the BLQFull flag set. An empty Buffer List Queue has an entry count equal to zero with the BLQEmpty flag set.
    Bits: 23:12 BLHdPtr(rw) - Buffer List Queue Head Pointer. Points to the next entry to be read by hardware.
    Bits: 11:0  BLTlPtr(rw) - Buffer List Queue Tail Pointer. Points to the next entry to be written at the tail.
*/
#define NFP_NBI_TM_TMBufferList_BL_QUE_CTRL0                 0x00048000
#define   NFP_NBI_TM_TMBUFFERLIST_BL_QUE_CTRL0_BLQ_EVENT_FULL_CNT(_x) (((_x) & 0xff) << 48)
#define   NFP_NBI_TM_TMBUFFERLIST_BL_QUE_CTRL0_BLQ_EVENT_FULL_CNT_of(_x) (((_x) >> 48) & 0xff)
#define   NFP_NBI_TM_TMBUFFERLIST_BL_QUE_CTRL0_BD_FIFO_ENTRY_CNT(_x) (((_x) & 0x7) << 40)
#define   NFP_NBI_TM_TMBUFFERLIST_BL_QUE_CTRL0_BD_FIFO_ENTRY_CNT_of(_x) (((_x) >> 40) & 0x7)
#define   NFP_NBI_TM_TMBUFFERLIST_BL_QUE_CTRL0_BL_QUE_FULL   BIT_ULL(39)
#define   NFP_NBI_TM_TMBUFFERLIST_BL_QUE_CTRL0_BL_QUE_EMPTY  BIT_ULL(38)
#define   NFP_NBI_TM_TMBUFFERLIST_BL_QUE_CTRL0_BL_QUE_SIZE(_x) (((_x) & 0x3) << 36)
#define   NFP_NBI_TM_TMBUFFERLIST_BL_QUE_CTRL0_BL_QUE_SIZE_of(_x) (((_x) >> 36) & 0x3)
#define     NFP_NBI_TM_TMBUFFERLIST_BL_QUE_CTRL0_BL_QUE_SIZE_512 (0)
#define     NFP_NBI_TM_TMBUFFERLIST_BL_QUE_CTRL0_BL_QUE_SIZE_1K (1)
#define     NFP_NBI_TM_TMBUFFERLIST_BL_QUE_CTRL0_BL_QUE_SIZE_2K (2)
#define     NFP_NBI_TM_TMBUFFERLIST_BL_QUE_CTRL0_BL_QUE_SIZE_4K (3)
#define   NFP_NBI_TM_TMBUFFERLIST_BL_QUE_CTRL0_BL_ENTRY_CNT(_x) (((_x) & 0xfff) << 24)
#define   NFP_NBI_TM_TMBUFFERLIST_BL_QUE_CTRL0_BL_ENTRY_CNT_of(_x) (((_x) >> 24) & 0xfff)
#define   NFP_NBI_TM_TMBUFFERLIST_BL_QUE_CTRL0_BL_HEAD_PTR(_x) (((_x) & 0xfff) << 12)
#define   NFP_NBI_TM_TMBUFFERLIST_BL_QUE_CTRL0_BL_HEAD_PTR_of(_x) (((_x) >> 12) & 0xfff)
#define   NFP_NBI_TM_TMBUFFERLIST_BL_QUE_CTRL0_BL_TAIL_PTR(_x) (((_x) & 0xfff) << 0)
#define   NFP_NBI_TM_TMBUFFERLIST_BL_QUE_CTRL0_BL_TAIL_PTR_of(_x) (((_x) >> 0) & 0xfff)
/*----------------------------------------------------------------
  Register: TMBufferList_BLQueCtrl1 - Configures the buffer list circular queue and provides status
    Bits: 55:48 BLQEventFullCnt(rc) - Debug counter that increments when a BLQ Event is dropped due to a full BLQ Event FIFO. This counter saturates at all 1's
    Bits: 42:40 BDFifoEntryCnt(ro) - Number of buffer descriptors resident in the buffer descriptor register FIFO
    Bits: 39    BLQFull(ro) - The Queue is full when set.
    Bits: 38    BLQEmpty(ro) - The queue is empty when set.
    Bits: 37:36 BLQSize(rw) - Defines the Size of the Circular Buffer List Queue
    Bits: 35:24 BLEntryCnt(ro) - Hardware calculated value of the number of entries on the Buffer List Queue. A full Buffer List Queue has an entry count equal to zero, with the BLQFull flag set. An empty Buffer List Queue has an entry count equal to zero with the BLQEmpty flag set.
    Bits: 23:12 BLHdPtr(rw) - Buffer List Queue Head Pointer. Points to the next entry to be read by hardware.
    Bits: 11:0  BLTlPtr(rw) - Buffer List Queue Tail Pointer. Points to the next entry to be written at the tail.
*/
#define NFP_NBI_TM_TMBufferList_BL_QUE_CTRL1                 0x00048008
#define   NFP_NBI_TM_TMBUFFERLIST_BL_QUE_CTRL1_BLQ_EVENT_FULL_CNT(_x) (((_x) & 0xff) << 48)
#define   NFP_NBI_TM_TMBUFFERLIST_BL_QUE_CTRL1_BLQ_EVENT_FULL_CNT_of(_x) (((_x) >> 48) & 0xff)
#define   NFP_NBI_TM_TMBUFFERLIST_BL_QUE_CTRL1_BD_FIFO_ENTRY_CNT(_x) (((_x) & 0x7) << 40)
#define   NFP_NBI_TM_TMBUFFERLIST_BL_QUE_CTRL1_BD_FIFO_ENTRY_CNT_of(_x) (((_x) >> 40) & 0x7)
#define   NFP_NBI_TM_TMBUFFERLIST_BL_QUE_CTRL1_BL_QUE_FULL   BIT_ULL(39)
#define   NFP_NBI_TM_TMBUFFERLIST_BL_QUE_CTRL1_BL_QUE_EMPTY  BIT_ULL(38)
#define   NFP_NBI_TM_TMBUFFERLIST_BL_QUE_CTRL1_BL_QUE_SIZE(_x) (((_x) & 0x3) << 36)
#define   NFP_NBI_TM_TMBUFFERLIST_BL_QUE_CTRL1_BL_QUE_SIZE_of(_x) (((_x) >> 36) & 0x3)
#define     NFP_NBI_TM_TMBUFFERLIST_BL_QUE_CTRL1_BL_QUE_SIZE_512 (0)
#define     NFP_NBI_TM_TMBUFFERLIST_BL_QUE_CTRL1_BL_QUE_SIZE_1K (1)
#define     NFP_NBI_TM_TMBUFFERLIST_BL_QUE_CTRL1_BL_QUE_SIZE_2K (2)
#define     NFP_NBI_TM_TMBUFFERLIST_BL_QUE_CTRL1_BL_QUE_SIZE_4K (3)
#define   NFP_NBI_TM_TMBUFFERLIST_BL_QUE_CTRL1_BL_ENTRY_CNT(_x) (((_x) & 0xfff) << 24)
#define   NFP_NBI_TM_TMBUFFERLIST_BL_QUE_CTRL1_BL_ENTRY_CNT_of(_x) (((_x) >> 24) & 0xfff)
#define   NFP_NBI_TM_TMBUFFERLIST_BL_QUE_CTRL1_BL_HEAD_PTR(_x) (((_x) & 0xfff) << 12)
#define   NFP_NBI_TM_TMBUFFERLIST_BL_QUE_CTRL1_BL_HEAD_PTR_of(_x) (((_x) >> 12) & 0xfff)
#define   NFP_NBI_TM_TMBUFFERLIST_BL_QUE_CTRL1_BL_TAIL_PTR(_x) (((_x) & 0xfff) << 0)
#define   NFP_NBI_TM_TMBUFFERLIST_BL_QUE_CTRL1_BL_TAIL_PTR_of(_x) (((_x) >> 0) & 0xfff)
/*----------------------------------------------------------------
  Register: TMBufferList_BLQueCtrl2 - Configures the buffer list circular queue and provides status
    Bits: 55:48 BLQEventFullCnt(rc) - Debug counter that increments when a BLQ Event is dropped due to a full BLQ Event FIFO. This counter saturates at all 1's
    Bits: 42:40 BDFifoEntryCnt(ro) - Number of buffer descriptors resident in the buffer descriptor register FIFO
    Bits: 39    BLQFull(ro) - The Queue is full when set.
    Bits: 38    BLQEmpty(ro) - The queue is empty when set.
    Bits: 37:36 BLQSize(rw) - Defines the Size of the Circular Buffer List Queue
    Bits: 35:24 BLEntryCnt(ro) - Hardware calculated value of the number of entries on the Buffer List Queue. A full Buffer List Queue has an entry count equal to zero, with the BLQFull flag set. An empty Buffer List Queue has an entry count equal to zero with the BLQEmpty flag set.
    Bits: 23:12 BLHdPtr(rw) - Buffer List Queue Head Pointer. Points to the next entry to be read by hardware.
    Bits: 11:0  BLTlPtr(rw) - Buffer List Queue Tail Pointer. Points to the next entry to be written at the tail.
*/
#define NFP_NBI_TM_TMBufferList_BL_QUE_CTRL2                 0x00048010
#define   NFP_NBI_TM_TMBUFFERLIST_BL_QUE_CTRL2_BLQ_EVENT_FULL_CNT(_x) (((_x) & 0xff) << 48)
#define   NFP_NBI_TM_TMBUFFERLIST_BL_QUE_CTRL2_BLQ_EVENT_FULL_CNT_of(_x) (((_x) >> 48) & 0xff)
#define   NFP_NBI_TM_TMBUFFERLIST_BL_QUE_CTRL2_BD_FIFO_ENTRY_CNT(_x) (((_x) & 0x7) << 40)
#define   NFP_NBI_TM_TMBUFFERLIST_BL_QUE_CTRL2_BD_FIFO_ENTRY_CNT_of(_x) (((_x) >> 40) & 0x7)
#define   NFP_NBI_TM_TMBUFFERLIST_BL_QUE_CTRL2_BL_QUE_FULL   BIT_ULL(39)
#define   NFP_NBI_TM_TMBUFFERLIST_BL_QUE_CTRL2_BL_QUE_EMPTY  BIT_ULL(38)
#define   NFP_NBI_TM_TMBUFFERLIST_BL_QUE_CTRL2_BL_QUE_SIZE(_x) (((_x) & 0x3) << 36)
#define   NFP_NBI_TM_TMBUFFERLIST_BL_QUE_CTRL2_BL_QUE_SIZE_of(_x) (((_x) >> 36) & 0x3)
#define     NFP_NBI_TM_TMBUFFERLIST_BL_QUE_CTRL2_BL_QUE_SIZE_512 (0)
#define     NFP_NBI_TM_TMBUFFERLIST_BL_QUE_CTRL2_BL_QUE_SIZE_1K (1)
#define     NFP_NBI_TM_TMBUFFERLIST_BL_QUE_CTRL2_BL_QUE_SIZE_2K (2)
#define     NFP_NBI_TM_TMBUFFERLIST_BL_QUE_CTRL2_BL_QUE_SIZE_4K (3)
#define   NFP_NBI_TM_TMBUFFERLIST_BL_QUE_CTRL2_BL_ENTRY_CNT(_x) (((_x) & 0xfff) << 24)
#define   NFP_NBI_TM_TMBUFFERLIST_BL_QUE_CTRL2_BL_ENTRY_CNT_of(_x) (((_x) >> 24) & 0xfff)
#define   NFP_NBI_TM_TMBUFFERLIST_BL_QUE_CTRL2_BL_HEAD_PTR(_x) (((_x) & 0xfff) << 12)
#define   NFP_NBI_TM_TMBUFFERLIST_BL_QUE_CTRL2_BL_HEAD_PTR_of(_x) (((_x) >> 12) & 0xfff)
#define   NFP_NBI_TM_TMBUFFERLIST_BL_QUE_CTRL2_BL_TAIL_PTR(_x) (((_x) & 0xfff) << 0)
#define   NFP_NBI_TM_TMBUFFERLIST_BL_QUE_CTRL2_BL_TAIL_PTR_of(_x) (((_x) >> 0) & 0xfff)
/*----------------------------------------------------------------
  Register: TMBufferList_BLQueCtrl3 - Configures the buffer list circular queue and provides status
    Bits: 55:48 BLQEventFullCnt(rc) - Debug counter that increments when a BLQ Event is dropped due to a full BLQ Event FIFO. This counter saturates at all 1's
    Bits: 42:40 BDFifoEntryCnt(ro) - Number of buffer descriptors resident in the buffer descriptor register FIFO
    Bits: 39    BLQFull(ro) - The Queue is full when set.
    Bits: 38    BLQEmpty(ro) - The queue is empty when set.
    Bits: 37:36 BLQSize(rw) - Defines the Size of the Circular Buffer List Queue
    Bits: 35:24 BLEntryCnt(ro) - Hardware calculated value of the number of entries on the Buffer List Queue. A full Buffer List Queue has an entry count equal to zero, with the BLQFull flag set. An empty Buffer List Queue has an entry count equal to zero with the BLQEmpty flag set.
    Bits: 23:12 BLHdPtr(rw) - Buffer List Queue Head Pointer. Points to the next entry to be read by hardware.
    Bits: 11:0  BLTlPtr(rw) - Buffer List Queue Tail Pointer. Points to the next entry to be written at the tail.
*/
#define NFP_NBI_TM_TMBufferList_BL_QUE_CTRL3                 0x00048018
#define   NFP_NBI_TM_TMBUFFERLIST_BL_QUE_CTRL3_BLQ_EVENT_FULL_CNT(_x) (((_x) & 0xff) << 48)
#define   NFP_NBI_TM_TMBUFFERLIST_BL_QUE_CTRL3_BLQ_EVENT_FULL_CNT_of(_x) (((_x) >> 48) & 0xff)
#define   NFP_NBI_TM_TMBUFFERLIST_BL_QUE_CTRL3_BD_FIFO_ENTRY_CNT(_x) (((_x) & 0x7) << 40)
#define   NFP_NBI_TM_TMBUFFERLIST_BL_QUE_CTRL3_BD_FIFO_ENTRY_CNT_of(_x) (((_x) >> 40) & 0x7)
#define   NFP_NBI_TM_TMBUFFERLIST_BL_QUE_CTRL3_BL_QUE_FULL   BIT_ULL(39)
#define   NFP_NBI_TM_TMBUFFERLIST_BL_QUE_CTRL3_BL_QUE_EMPTY  BIT_ULL(38)
#define   NFP_NBI_TM_TMBUFFERLIST_BL_QUE_CTRL3_BL_QUE_SIZE(_x) (((_x) & 0x3) << 36)
#define   NFP_NBI_TM_TMBUFFERLIST_BL_QUE_CTRL3_BL_QUE_SIZE_of(_x) (((_x) >> 36) & 0x3)
#define     NFP_NBI_TM_TMBUFFERLIST_BL_QUE_CTRL3_BL_QUE_SIZE_512 (0)
#define     NFP_NBI_TM_TMBUFFERLIST_BL_QUE_CTRL3_BL_QUE_SIZE_1K (1)
#define     NFP_NBI_TM_TMBUFFERLIST_BL_QUE_CTRL3_BL_QUE_SIZE_2K (2)
#define     NFP_NBI_TM_TMBUFFERLIST_BL_QUE_CTRL3_BL_QUE_SIZE_4K (3)
#define   NFP_NBI_TM_TMBUFFERLIST_BL_QUE_CTRL3_BL_ENTRY_CNT(_x) (((_x) & 0xfff) << 24)
#define   NFP_NBI_TM_TMBUFFERLIST_BL_QUE_CTRL3_BL_ENTRY_CNT_of(_x) (((_x) >> 24) & 0xfff)
#define   NFP_NBI_TM_TMBUFFERLIST_BL_QUE_CTRL3_BL_HEAD_PTR(_x) (((_x) & 0xfff) << 12)
#define   NFP_NBI_TM_TMBUFFERLIST_BL_QUE_CTRL3_BL_HEAD_PTR_of(_x) (((_x) >> 12) & 0xfff)
#define   NFP_NBI_TM_TMBUFFERLIST_BL_QUE_CTRL3_BL_TAIL_PTR(_x) (((_x) & 0xfff) << 0)
#define   NFP_NBI_TM_TMBUFFERLIST_BL_QUE_CTRL3_BL_TAIL_PTR_of(_x) (((_x) >> 0) & 0xfff)
/*----------------------------------------------------------------
  Register: TMSlowDescSram_TMSlowDescSramEntry[2048] - NBI TM Slow Descriptor Entry
    Bits: 63:0  TmSlowDescData(rw) - 64-bits of the 128-bit descriptor in the TM Slow Descriptor SRAM. Refer to the Packet Descriptor Details table for details on bit definitions
*/
#define NFP_NBI_TM_TMSlowDescSram_TM_SLOW_DESC_ENTRY(_x)     (0x00050000 + (0x8 * ((_x) & 0x7ff)))
#define   NFP_NBI_TM_TMSLOWDESCSRAM_TM_SLOW_DESC_ENTRY_TM_SLOW_DESC_DATA(_x) (((_x) & 0xffffffffffffffff) << 0)
#define   NFP_NBI_TM_TMSLOWDESCSRAM_TM_SLOW_DESC_ENTRY_TM_SLOW_DESC_DATA_of(_x) (((_x) >> 0) & 0xffffffffffffffff)
/*----------------------------------------------------------------
  Register: TMReorderBuf_TMReorderBufEntry[2048] - NBI TM Reorder Buffer Entry
    Bits: 63:0  TmReorderBufData(rw) - 64-bits of the 128-bit descriptor in the TM Reorder Buffer SRAM. Refer to the Packet Descriptor Details table for details on bit definitions
*/
#define NFP_NBI_TM_TMReorderBuf_TM_RORDR_BUF_ENTRY(_x)       (0x00058000 + (0x8 * ((_x) & 0x7ff)))
#define   NFP_NBI_TM_TMREORDERBUF_TM_RORDR_BUF_ENTRY_TM_RE_BUF_DATA(_x) (((_x) & 0xffffffffffffffff) << 0)
#define   NFP_NBI_TM_TMREORDERBUF_TM_RORDR_BUF_ENTRY_TM_RE_BUF_DATA_of(_x) (((_x) >> 0) & 0xffffffffffffffff)
/*----------------------------------------------------------------
  Register: TMPktSram_TMPktSramEntry[2048] - NBI TM Packet SRAM Entry
    Bits: 63:0  TMPktSRAMData(rw) - 64-bits of packet data within the TM Packet Buffer SRAM. Refer to Packet Buffer Entry for details
*/
#define NFP_NBI_TM_TMPktSram_TM_PKT_SRAM_ENTRY(_x)           (0x00060000 + (0x8 * ((_x) & 0x7ff)))
#define   NFP_NBI_TM_TMPKTSRAM_TM_PKT_SRAM_ENTRY_TM_PKT_SRAM_DATA(_x) (((_x) & 0xffffffffffffffff) << 0)
#define   NFP_NBI_TM_TMPKTSRAM_TM_PKT_SRAM_ENTRY_TM_PKT_SRAM_DATA_of(_x) (((_x) >> 0) & 0xffffffffffffffff)
/*----------------------------------------------------------------
  Register: TMHeadTailSram_TMHeadTailEntry[1024] - NBI TM Head and Tail SRAM Entry
    Bits: 27:14 HeadPointer(rw) - 14-bit Descriptor Queue Head Pointer
    Bits: 13:0  TailPointer(rw) - 14-bit Descriptor Queue Tail Pointer
*/
#define NFP_NBI_TM_TMHeadTailSram_TM_HEAD_TAIL_ENTRY(_x)     (0x00068000 + (0x8 * ((_x) & 0x3ff)))
#define   NFP_NBI_TM_TMHEADTAILSRAM_TM_HEAD_TAIL_ENTRY_HEAD_POINTER(_x) (((_x) & 0x3fff) << 14)
#define   NFP_NBI_TM_TMHEADTAILSRAM_TM_HEAD_TAIL_ENTRY_HEAD_POINTER_of(_x) (((_x) >> 14) & 0x3fff)
#define   NFP_NBI_TM_TMHEADTAILSRAM_TM_HEAD_TAIL_ENTRY_TAIL_POINTER(_x) (((_x) & 0x3fff) << 0)
#define   NFP_NBI_TM_TMHEADTAILSRAM_TM_HEAD_TAIL_ENTRY_TAIL_POINTER_of(_x) (((_x) >> 0) & 0x3fff)
/*----------------------------------------------------------------
  Register: TMFlowControl_TMFlowControlEntry[18] - Reads 64-bits at a time of Traffic Manager Flow Control information
    Bits: 63:0  FlowControlData(ro) - Read only access to the 1024 bits of Out of Band Flow control, and 128-bits of XOFF flow control from the MAC.
*/
#define NFP_NBI_TM_TMFlowControl_FLOW_CONTROL(_x)            (0x00070000 + (0x8 * ((_x) & 0x1f)))
#define   NFP_NBI_TM_TMFLOWCONTROL_FLOW_CONTROL_FLOW_CONTROL_DATA(_x) (((_x) & 0xffffffffffffffff) << 0)
#define   NFP_NBI_TM_TMFLOWCONTROL_FLOW_CONTROL_FLOW_CONTROL_DATA_of(_x) (((_x) >> 0) & 0xffffffffffffffff)
/*----------------------------------------------------------------
  Register: TMBLQRead_BLQRead[4] - Reads a pair of entries from Buffer List Queue
    Bits: 60:32 MuAddrHi(ro) - Pointer to the memory buffer. Refer to NbiBDSramEntry register description.
    Bits: 28:0  MuAddrLo(ro) - Pointer to the memory buffer. Refer to NbiBDSramEntry register description.
*/
#define NFP_NBI_TM_TMBLQRead_BL_QUE_READ(_x)                 (0x00080000 + (0x80 * ((_x) & 0x3)))
#define   NFP_NBI_TM_TMBLQREAD_BL_QUE_READ_MU_PTR_HI(_x)     (((_x) & 0x1fffffff) << 32)
#define   NFP_NBI_TM_TMBLQREAD_BL_QUE_READ_MU_PTR_HI_of(_x)  (((_x) >> 32) & 0x1fffffff)
#define   NFP_NBI_TM_TMBLQREAD_BL_QUE_READ_MU_PTR_LO(_x)     (((_x) & 0x1fffffff) << 0)
#define   NFP_NBI_TM_TMBLQREAD_BL_QUE_READ_MU_PTR_LO_of(_x)  (((_x) >> 0) & 0x1fffffff)
/*----------------------------------------------------------------
  Register: TrafficManagerConfig - Traffic manager configuration register. Contains global read/write configuration information.
    Bits: 16    OOBFCEnable(rw) - MAC Out of Band Flow Control Enable. Enable the Out of Band Flow Control from the MAC external interface to shut off the 1024 level 2 scheduler ports.
    Bits: 15:13 L0ChannelMap(rw) - MAC channel number to Level 0 Scheduler port number mapping register. This mapping register is valid only when L1InputSelect = 1 and ChannelLevelSelect = 0. In this configuration there are only 16 L0 Scheduler ports used. One MAC channel may map to multiple L0 Scheduler ports, and an offset of 64 can be used to support multiple Hydras.
    Bits: 10    Sequencer0Enable(rw) - Sequencer 0 Enable bit. Set this bit to enable reordering for sequencer 0, bit is set by default. Reordering for sequencer zero will be disabled when this bit is cleared.
    Bits:  9    ErrPoisonEna(rw) - It enables the poisoning of data and/or enables the other error porpagation mechanisms in case of errors.
    Bits:  8    SchedulerEnable(rw) - Scheduler Enable bit. Set this bit to enable the scheduling of traffic, clear to disable.
    Bits:  7    ShaperEnable(rw) - Shaper Enable bit. Enables the Rate Shaper to start rate limiting traffic based on the configured values. The default is off so that the shaper is disabled prior to configuration.
    Bits:  6    MiniPacketFCEnable(rw) - Minipacket Credit based Flow Control enable bit. Clear this bit to disable minipacket flow control. This bit is set to enable flow control by default.
    Bits:  5    ChannelLevelSelect(rw) - Channel level select bit. Select the definition of the MAC channel number and the MAC XOFF Flow control to be either the 128 level 0 scheduler ports, or 128 ports on the first 16 level 1 schedulers (or level 2 schedulers depending on the value of L1InputSelect). Note: When L1InputSelect=0 and ChannelLevelSelect=1, only 128 queues are allowed to be active in this configuration.
    Bits:  3:1  NumSequencers(rw) - Number of active sequencers. Specifies the number of active sequencers in the traffic manager.
    Bits:  0    L1InputSelect(rw) - Select the operation of the Level 1 schedulers. This bit disables or enables the Level 1 schedulers, and allows for two or three levels of scheduling in the Traffic Manager. When disabled, the Level 2 scheduler outputs are connected directly to the Level 0 scheduler inputs. When enabled, Level 2 outputs connect to Level 1 inputs, and Level 1 outputs connects to Level 0 inputs
*/
#define NFP_NBI_TMX_CSR_TRAFFIC_MANAGER_CONFIG               0x00000000
#define   NFP_NBI_TMX_CSR_TRAFFIC_MANAGER_CONFIG_OOBFCENABLE BIT(16)
#define     NFP_NBI_TMX_CSR_TRAFFIC_MANAGER_CONFIG_OOBFCENABLE_DISABLE (0 << 16)
#define     NFP_NBI_TMX_CSR_TRAFFIC_MANAGER_CONFIG_OOBFCENABLE_ENABLE BIT(16)
#define   NFP_NBI_TMX_CSR_TRAFFIC_MANAGER_CONFIG_L0CHANNELMAP(_x) (((_x) & 0x7) << 13)
#define   NFP_NBI_TMX_CSR_TRAFFIC_MANAGER_CONFIG_L0CHANNELMAP_of(_x) (((_x) >> 13) & 0x7)
#define     NFP_NBI_TMX_CSR_TRAFFIC_MANAGER_CONFIG_L0CHANNELMAP_0 (0)
#define     NFP_NBI_TMX_CSR_TRAFFIC_MANAGER_CONFIG_L0CHANNELMAP_1 (1)
#define     NFP_NBI_TMX_CSR_TRAFFIC_MANAGER_CONFIG_L0CHANNELMAP_2 (2)
#define     NFP_NBI_TMX_CSR_TRAFFIC_MANAGER_CONFIG_L0CHANNELMAP_3 (3)
#define     NFP_NBI_TMX_CSR_TRAFFIC_MANAGER_CONFIG_L0CHANNELMAP_4 (4)
#define     NFP_NBI_TMX_CSR_TRAFFIC_MANAGER_CONFIG_L0CHANNELMAP_5 (5)
#define     NFP_NBI_TMX_CSR_TRAFFIC_MANAGER_CONFIG_L0CHANNELMAP_6 (6)
#define     NFP_NBI_TMX_CSR_TRAFFIC_MANAGER_CONFIG_L0CHANNELMAP_7 (7)
#define   NFP_NBI_TMX_CSR_TRAFFIC_MANAGER_CONFIG_SEQUENCER0ENABLE BIT(10)
#define     NFP_NBI_TMX_CSR_TRAFFIC_MANAGER_CONFIG_SEQUENCER0ENABLE_DISABLE (0 << 10)
#define     NFP_NBI_TMX_CSR_TRAFFIC_MANAGER_CONFIG_SEQUENCER0ENABLE_ENABLE BIT(10)
#define   NFP_NBI_TMX_CSR_TRAFFIC_MANAGER_CONFIG_ERR_POISON_ENA BIT(9)
#define     NFP_NBI_TMX_CSR_TRAFFIC_MANAGER_CONFIG_ERR_POISON_ENA_DISABLE (0 << 9)
#define     NFP_NBI_TMX_CSR_TRAFFIC_MANAGER_CONFIG_ERR_POISON_ENA_ENABLE BIT(9)
#define   NFP_NBI_TMX_CSR_TRAFFIC_MANAGER_CONFIG_SCHEDULERENABLE BIT(8)
#define     NFP_NBI_TMX_CSR_TRAFFIC_MANAGER_CONFIG_SCHEDULERENABLE_DISABLE (0 << 8)
#define     NFP_NBI_TMX_CSR_TRAFFIC_MANAGER_CONFIG_SCHEDULERENABLE_ENABLE BIT(8)
#define   NFP_NBI_TMX_CSR_TRAFFIC_MANAGER_CONFIG_SHAPERENABLE BIT(7)
#define     NFP_NBI_TMX_CSR_TRAFFIC_MANAGER_CONFIG_SHAPERENABLE_DISABLE (0 << 7)
#define     NFP_NBI_TMX_CSR_TRAFFIC_MANAGER_CONFIG_SHAPERENABLE_ENABLE BIT(7)
#define   NFP_NBI_TMX_CSR_TRAFFIC_MANAGER_CONFIG_MINIPACKETFCENABLE BIT(6)
#define     NFP_NBI_TMX_CSR_TRAFFIC_MANAGER_CONFIG_MINIPACKETFCENABLE_DISABLE (0 << 6)
#define     NFP_NBI_TMX_CSR_TRAFFIC_MANAGER_CONFIG_MINIPACKETFCENABLE_ENABLE BIT(6)
#define   NFP_NBI_TMX_CSR_TRAFFIC_MANAGER_CONFIG_CHANNELLEVELSELECT BIT(5)
#define     NFP_NBI_TMX_CSR_TRAFFIC_MANAGER_CONFIG_CHANNELLEVELSELECT_LEVEL_0 (0 << 5)
#define     NFP_NBI_TMX_CSR_TRAFFIC_MANAGER_CONFIG_CHANNELLEVELSELECT_LEVEL_1 BIT(5)
#define   NFP_NBI_TMX_CSR_TRAFFIC_MANAGER_CONFIG_NUMSEQUENCERS(_x) (((_x) & 0x7) << 1)
#define   NFP_NBI_TMX_CSR_TRAFFIC_MANAGER_CONFIG_NUMSEQUENCERS_of(_x) (((_x) >> 1) & 0x7)
#define     NFP_NBI_TMX_CSR_TRAFFIC_MANAGER_CONFIG_NUMSEQUENCERS_0 (0)
#define     NFP_NBI_TMX_CSR_TRAFFIC_MANAGER_CONFIG_NUMSEQUENCERS_1 (1)
#define     NFP_NBI_TMX_CSR_TRAFFIC_MANAGER_CONFIG_NUMSEQUENCERS_2 (2)
#define     NFP_NBI_TMX_CSR_TRAFFIC_MANAGER_CONFIG_NUMSEQUENCERS_4 (3)
#define     NFP_NBI_TMX_CSR_TRAFFIC_MANAGER_CONFIG_NUMSEQUENCERS_8 (4)
#define     NFP_NBI_TMX_CSR_TRAFFIC_MANAGER_CONFIG_NUMSEQUENCERS_16 (5)
#define     NFP_NBI_TMX_CSR_TRAFFIC_MANAGER_CONFIG_NUMSEQUENCERS_32 (6)
#define     NFP_NBI_TMX_CSR_TRAFFIC_MANAGER_CONFIG_NUMSEQUENCERS_NA (7)
#define   NFP_NBI_TMX_CSR_TRAFFIC_MANAGER_CONFIG_L1INPUTSELECT BIT(0)
#define     NFP_NBI_TMX_CSR_TRAFFIC_MANAGER_CONFIG_L1INPUTSELECT_L1_DISABLE (0 << 0)
#define     NFP_NBI_TMX_CSR_TRAFFIC_MANAGER_CONFIG_L1INPUTSELECT_L1_ENABLE BIT(0)
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
#define NFP_NBI_TMX_CSR_DROP_RATE                            0x00000004
#define   NFP_NBI_TMX_CSR_DROP_RATE_DROPRATERANGEB3(_x)      (((_x) & 0x7) << 21)
#define   NFP_NBI_TMX_CSR_DROP_RATE_DROPRATERANGEB3_of(_x)   (((_x) >> 21) & 0x7)
#define     NFP_NBI_TMX_CSR_DROP_RATE_DROPRATERANGEB3_0      (0)
#define     NFP_NBI_TMX_CSR_DROP_RATE_DROPRATERANGEB3_12_5   (1)
#define     NFP_NBI_TMX_CSR_DROP_RATE_DROPRATERANGEB3_25     (2)
#define     NFP_NBI_TMX_CSR_DROP_RATE_DROPRATERANGEB3_37_5   (3)
#define     NFP_NBI_TMX_CSR_DROP_RATE_DROPRATERANGEB3_50     (4)
#define     NFP_NBI_TMX_CSR_DROP_RATE_DROPRATERANGEB3_62_5   (5)
#define     NFP_NBI_TMX_CSR_DROP_RATE_DROPRATERANGEB3_75     (6)
#define     NFP_NBI_TMX_CSR_DROP_RATE_DROPRATERANGEB3_87_5   (7)
#define   NFP_NBI_TMX_CSR_DROP_RATE_DROPRATERANGEB2(_x)      (((_x) & 0x7) << 18)
#define   NFP_NBI_TMX_CSR_DROP_RATE_DROPRATERANGEB2_of(_x)   (((_x) >> 18) & 0x7)
#define     NFP_NBI_TMX_CSR_DROP_RATE_DROPRATERANGEB2_0      (0)
#define     NFP_NBI_TMX_CSR_DROP_RATE_DROPRATERANGEB2_12_5   (1)
#define     NFP_NBI_TMX_CSR_DROP_RATE_DROPRATERANGEB2_25     (2)
#define     NFP_NBI_TMX_CSR_DROP_RATE_DROPRATERANGEB2_37_5   (3)
#define     NFP_NBI_TMX_CSR_DROP_RATE_DROPRATERANGEB2_50     (4)
#define     NFP_NBI_TMX_CSR_DROP_RATE_DROPRATERANGEB2_62_5   (5)
#define     NFP_NBI_TMX_CSR_DROP_RATE_DROPRATERANGEB2_75     (6)
#define     NFP_NBI_TMX_CSR_DROP_RATE_DROPRATERANGEB2_87_5   (7)
#define   NFP_NBI_TMX_CSR_DROP_RATE_DROPRATERANGEB1(_x)      (((_x) & 0x7) << 15)
#define   NFP_NBI_TMX_CSR_DROP_RATE_DROPRATERANGEB1_of(_x)   (((_x) >> 15) & 0x7)
#define     NFP_NBI_TMX_CSR_DROP_RATE_DROPRATERANGEB1_0      (0)
#define     NFP_NBI_TMX_CSR_DROP_RATE_DROPRATERANGEB1_12_5   (1)
#define     NFP_NBI_TMX_CSR_DROP_RATE_DROPRATERANGEB1_25     (2)
#define     NFP_NBI_TMX_CSR_DROP_RATE_DROPRATERANGEB1_37_5   (3)
#define     NFP_NBI_TMX_CSR_DROP_RATE_DROPRATERANGEB1_50     (4)
#define     NFP_NBI_TMX_CSR_DROP_RATE_DROPRATERANGEB1_62_5   (5)
#define     NFP_NBI_TMX_CSR_DROP_RATE_DROPRATERANGEB1_75     (6)
#define     NFP_NBI_TMX_CSR_DROP_RATE_DROPRATERANGEB1_87_5   (7)
#define   NFP_NBI_TMX_CSR_DROP_RATE_DROPRATERANGEB0(_x)      (((_x) & 0x7) << 12)
#define   NFP_NBI_TMX_CSR_DROP_RATE_DROPRATERANGEB0_of(_x)   (((_x) >> 12) & 0x7)
#define     NFP_NBI_TMX_CSR_DROP_RATE_DROPRATERANGEB0_0      (0)
#define     NFP_NBI_TMX_CSR_DROP_RATE_DROPRATERANGEB0_12_5   (1)
#define     NFP_NBI_TMX_CSR_DROP_RATE_DROPRATERANGEB0_25     (2)
#define     NFP_NBI_TMX_CSR_DROP_RATE_DROPRATERANGEB0_37_5   (3)
#define     NFP_NBI_TMX_CSR_DROP_RATE_DROPRATERANGEB0_50     (4)
#define     NFP_NBI_TMX_CSR_DROP_RATE_DROPRATERANGEB0_62_5   (5)
#define     NFP_NBI_TMX_CSR_DROP_RATE_DROPRATERANGEB0_75     (6)
#define     NFP_NBI_TMX_CSR_DROP_RATE_DROPRATERANGEB0_87_5   (7)
#define   NFP_NBI_TMX_CSR_DROP_RATE_DROPRATERANGEA3(_x)      (((_x) & 0x7) << 9)
#define   NFP_NBI_TMX_CSR_DROP_RATE_DROPRATERANGEA3_of(_x)   (((_x) >> 9) & 0x7)
#define     NFP_NBI_TMX_CSR_DROP_RATE_DROPRATERANGEA3_0      (0)
#define     NFP_NBI_TMX_CSR_DROP_RATE_DROPRATERANGEA3_12_5   (1)
#define     NFP_NBI_TMX_CSR_DROP_RATE_DROPRATERANGEA3_25     (2)
#define     NFP_NBI_TMX_CSR_DROP_RATE_DROPRATERANGEA3_37_5   (3)
#define     NFP_NBI_TMX_CSR_DROP_RATE_DROPRATERANGEA3_50     (4)
#define     NFP_NBI_TMX_CSR_DROP_RATE_DROPRATERANGEA3_62_5   (5)
#define     NFP_NBI_TMX_CSR_DROP_RATE_DROPRATERANGEA3_75     (6)
#define     NFP_NBI_TMX_CSR_DROP_RATE_DROPRATERANGEA3_87_5   (7)
#define   NFP_NBI_TMX_CSR_DROP_RATE_DROPRATERANGEA2(_x)      (((_x) & 0x7) << 6)
#define   NFP_NBI_TMX_CSR_DROP_RATE_DROPRATERANGEA2_of(_x)   (((_x) >> 6) & 0x7)
#define     NFP_NBI_TMX_CSR_DROP_RATE_DROPRATERANGEA2_0      (0)
#define     NFP_NBI_TMX_CSR_DROP_RATE_DROPRATERANGEA2_12_5   (1)
#define     NFP_NBI_TMX_CSR_DROP_RATE_DROPRATERANGEA2_25     (2)
#define     NFP_NBI_TMX_CSR_DROP_RATE_DROPRATERANGEA2_37_5   (3)
#define     NFP_NBI_TMX_CSR_DROP_RATE_DROPRATERANGEA2_50     (4)
#define     NFP_NBI_TMX_CSR_DROP_RATE_DROPRATERANGEA2_62_5   (5)
#define     NFP_NBI_TMX_CSR_DROP_RATE_DROPRATERANGEA2_75     (6)
#define     NFP_NBI_TMX_CSR_DROP_RATE_DROPRATERANGEA2_87_5   (7)
#define   NFP_NBI_TMX_CSR_DROP_RATE_DROPRATERANGEA1(_x)      (((_x) & 0x7) << 3)
#define   NFP_NBI_TMX_CSR_DROP_RATE_DROPRATERANGEA1_of(_x)   (((_x) >> 3) & 0x7)
#define     NFP_NBI_TMX_CSR_DROP_RATE_DROPRATERANGEA1_0      (0)
#define     NFP_NBI_TMX_CSR_DROP_RATE_DROPRATERANGEA1_12_5   (1)
#define     NFP_NBI_TMX_CSR_DROP_RATE_DROPRATERANGEA1_25     (2)
#define     NFP_NBI_TMX_CSR_DROP_RATE_DROPRATERANGEA1_37_5   (3)
#define     NFP_NBI_TMX_CSR_DROP_RATE_DROPRATERANGEA1_50     (4)
#define     NFP_NBI_TMX_CSR_DROP_RATE_DROPRATERANGEA1_62_5   (5)
#define     NFP_NBI_TMX_CSR_DROP_RATE_DROPRATERANGEA1_75     (6)
#define     NFP_NBI_TMX_CSR_DROP_RATE_DROPRATERANGEA1_87_5   (7)
#define   NFP_NBI_TMX_CSR_DROP_RATE_DROPRATERANGEA0(_x)      (((_x) & 0x7) << 0)
#define   NFP_NBI_TMX_CSR_DROP_RATE_DROPRATERANGEA0_of(_x)   (((_x) >> 0) & 0x7)
#define     NFP_NBI_TMX_CSR_DROP_RATE_DROPRATERANGEA0_0      (0)
#define     NFP_NBI_TMX_CSR_DROP_RATE_DROPRATERANGEA0_12_5   (1)
#define     NFP_NBI_TMX_CSR_DROP_RATE_DROPRATERANGEA0_25     (2)
#define     NFP_NBI_TMX_CSR_DROP_RATE_DROPRATERANGEA0_37_5   (3)
#define     NFP_NBI_TMX_CSR_DROP_RATE_DROPRATERANGEA0_50     (4)
#define     NFP_NBI_TMX_CSR_DROP_RATE_DROPRATERANGEA0_62_5   (5)
#define     NFP_NBI_TMX_CSR_DROP_RATE_DROPRATERANGEA0_75     (6)
#define     NFP_NBI_TMX_CSR_DROP_RATE_DROPRATERANGEA0_87_5   (7)
/*----------------------------------------------------------------
  Register: BlqEvent - Configures thresholds, event type and enables/disables for the buffer list queues
    Bits: 20    DisBLQEventFifoFullAssert(rw) - Set this bit to disable the BLQ Event FIFO overflow assertion
    Bits: 19:18 BLQEventThreshold3(rw) - An event is generated every N buffer descriptors written to buffer list queue3, where N is configurable from 16 to 128.
    Bits: 17:16 BLQEventThreshold2(rw) - An event is generated every N buffer descriptors written to buffer list queue2, where N is configurable from 16 to 128.
    Bits: 15:14 BLQEventThreshold1(rw) - An event is generated every N buffer descriptors written to buffer list queue1, where N is configurable from 16 to 128.
    Bits: 13:12 BLQEventThreshold0(rw) - An event is generated every N buffer descriptors written to buffer list queue0, where N is configurable from 16 to 128.
    Bits:  7:4  BLQEventNotEmptyEnable(rw) - One bit per buffer list queue. Enables the generation of BLQ Events when the queue transitions from empty to not empty. Disabled when clear. The event type value used is 0x0 (event_type_fifo_not_empty)
    Bits:  3:0  BLQEventStatusEnable(rw) - One bit per buffer list queue. Enables BLQ Status Events when set. Disabled when clear. A Status Event is sent when the number of entries written to the queue reaches the programmed value in the BLQEventThreshold register. The event type value used is 0x5 (event_type_value_update)
*/
#define NFP_NBI_TMX_CSR_BLQ_EVENT                            0x00000008
#define   NFP_NBI_TMX_CSR_BLQ_EVENT_DIS_BLQ_EVENT_FIFO_FULL_ASSERT BIT(20)
#define   NFP_NBI_TMX_CSR_BLQ_EVENT_BLQEVENT_THRESHOLD3(_x)  (((_x) & 0x3) << 18)
#define   NFP_NBI_TMX_CSR_BLQ_EVENT_BLQEVENT_THRESHOLD3_of(_x) (((_x) >> 18) & 0x3)
#define     NFP_NBI_TMX_CSR_BLQ_EVENT_BLQEVENT_THRESHOLD3_16 (0)
#define     NFP_NBI_TMX_CSR_BLQ_EVENT_BLQEVENT_THRESHOLD3_32 (1)
#define     NFP_NBI_TMX_CSR_BLQ_EVENT_BLQEVENT_THRESHOLD3_64 (2)
#define     NFP_NBI_TMX_CSR_BLQ_EVENT_BLQEVENT_THRESHOLD3_128 (3)
#define   NFP_NBI_TMX_CSR_BLQ_EVENT_BLQEVENT_THRESHOLD2(_x)  (((_x) & 0x3) << 16)
#define   NFP_NBI_TMX_CSR_BLQ_EVENT_BLQEVENT_THRESHOLD2_of(_x) (((_x) >> 16) & 0x3)
#define     NFP_NBI_TMX_CSR_BLQ_EVENT_BLQEVENT_THRESHOLD2_16 (0)
#define     NFP_NBI_TMX_CSR_BLQ_EVENT_BLQEVENT_THRESHOLD2_32 (1)
#define     NFP_NBI_TMX_CSR_BLQ_EVENT_BLQEVENT_THRESHOLD2_64 (2)
#define     NFP_NBI_TMX_CSR_BLQ_EVENT_BLQEVENT_THRESHOLD2_128 (3)
#define   NFP_NBI_TMX_CSR_BLQ_EVENT_BLQEVENT_THRESHOLD1(_x)  (((_x) & 0x3) << 14)
#define   NFP_NBI_TMX_CSR_BLQ_EVENT_BLQEVENT_THRESHOLD1_of(_x) (((_x) >> 14) & 0x3)
#define     NFP_NBI_TMX_CSR_BLQ_EVENT_BLQEVENT_THRESHOLD1_16 (0)
#define     NFP_NBI_TMX_CSR_BLQ_EVENT_BLQEVENT_THRESHOLD1_32 (1)
#define     NFP_NBI_TMX_CSR_BLQ_EVENT_BLQEVENT_THRESHOLD1_64 (2)
#define     NFP_NBI_TMX_CSR_BLQ_EVENT_BLQEVENT_THRESHOLD1_128 (3)
#define   NFP_NBI_TMX_CSR_BLQ_EVENT_BLQEVENT_THRESHOLD0(_x)  (((_x) & 0x3) << 12)
#define   NFP_NBI_TMX_CSR_BLQ_EVENT_BLQEVENT_THRESHOLD0_of(_x) (((_x) >> 12) & 0x3)
#define     NFP_NBI_TMX_CSR_BLQ_EVENT_BLQEVENT_THRESHOLD0_16 (0)
#define     NFP_NBI_TMX_CSR_BLQ_EVENT_BLQEVENT_THRESHOLD0_32 (1)
#define     NFP_NBI_TMX_CSR_BLQ_EVENT_BLQEVENT_THRESHOLD0_64 (2)
#define     NFP_NBI_TMX_CSR_BLQ_EVENT_BLQEVENT_THRESHOLD0_128 (3)
#define   NFP_NBI_TMX_CSR_BLQ_EVENT_BLQ_EVENT_NOT_EMPTY_ENABLE(_x) (((_x) & 0xf) << 4)
#define   NFP_NBI_TMX_CSR_BLQ_EVENT_BLQ_EVENT_NOT_EMPTY_ENABLE_of(_x) (((_x) >> 4) & 0xf)
#define     NFP_NBI_TMX_CSR_BLQ_EVENT_BLQ_EVENT_NOT_EMPTY_ENABLE_DISABLE (0)
#define     NFP_NBI_TMX_CSR_BLQ_EVENT_BLQ_EVENT_NOT_EMPTY_ENABLE_ENABLE (1)
#define   NFP_NBI_TMX_CSR_BLQ_EVENT_BLQ_EVENT_STATUS_ENABLE(_x) (((_x) & 0xf) << 0)
#define   NFP_NBI_TMX_CSR_BLQ_EVENT_BLQ_EVENT_STATUS_ENABLE_of(_x) (((_x) >> 0) & 0xf)
#define     NFP_NBI_TMX_CSR_BLQ_EVENT_BLQ_EVENT_STATUS_ENABLE_DISABLE (0)
#define     NFP_NBI_TMX_CSR_BLQ_EVENT_BLQ_EVENT_STATUS_ENABLE_ENABLE (1)
/*----------------------------------------------------------------
  Register: EgressRateLimit - Rate limiting CSR for the Egress NBI Command/PullId ports.
    Bits:  2    EgressRateLimitOff(rw) - Turns the rate limiting off for both Ports 6 and 7 (P6 and P7).
    Bits:  1:0  EgressRate(rw) - Rate limiting control for the Command/PullId Ports 6 and 7 (P6 and P7).
*/
#define NFP_NBI_TMX_CSR_EGRESS_RATE_LIMIT                    0x00000074
#define   NFP_NBI_TMX_CSR_EGRESS_RATE_LIMIT_EGRESS_RATE_LIMIT_OFF BIT(2)
#define     NFP_NBI_TMX_CSR_EGRESS_RATE_LIMIT_EGRESS_RATE_LIMIT_OFF_ON (0 << 2)
#define     NFP_NBI_TMX_CSR_EGRESS_RATE_LIMIT_EGRESS_RATE_LIMIT_OFF_OFF BIT(2)
#define   NFP_NBI_TMX_CSR_EGRESS_RATE_LIMIT_EGRESS_RATE(_x)  (((_x) & 0x3) << 0)
#define   NFP_NBI_TMX_CSR_EGRESS_RATE_LIMIT_EGRESS_RATE_of(_x) (((_x) >> 0) & 0x3)
#define     NFP_NBI_TMX_CSR_EGRESS_RATE_LIMIT_EGRESS_RATE_64G (0)
#define     NFP_NBI_TMX_CSR_EGRESS_RATE_LIMIT_EGRESS_RATE_32G (1)
#define     NFP_NBI_TMX_CSR_EGRESS_RATE_LIMIT_EGRESS_RATE_16G (2)
#define     NFP_NBI_TMX_CSR_EGRESS_RATE_LIMIT_EGRESS_RATE_8G (3)
/*----------------------------------------------------------------
  Register: EgressPullIdPortEnable - Enables ports for transmitting Pull IDs.
    Bits:  1:0  EgressPullIdPortEnable(rw) - Egress Port enable for transmitting PullIDs.
*/
#define NFP_NBI_TMX_CSR_EGRESS_PULL_ID_PORT_ENABLE           0x00000078
#define   NFP_NBI_TMX_CSR_EGRESS_PULL_ID_PORT_ENABLE_EGRESS_PULL_ID_PORT_ENABLE(_x) (((_x) & 0x3) << 0)
#define   NFP_NBI_TMX_CSR_EGRESS_PULL_ID_PORT_ENABLE_EGRESS_PULL_ID_PORT_ENABLE_of(_x) (((_x) >> 0) & 0x3)
#define     NFP_NBI_TMX_CSR_EGRESS_PULL_ID_PORT_ENABLE_EGRESS_PULL_ID_PORT_ENABLE_P6_AND_P7 (0)
#define     NFP_NBI_TMX_CSR_EGRESS_PULL_ID_PORT_ENABLE_EGRESS_PULL_ID_PORT_ENABLE_P6 (1)
#define     NFP_NBI_TMX_CSR_EGRESS_PULL_ID_PORT_ENABLE_EGRESS_PULL_ID_PORT_ENABLE_P7 (2)
#define     NFP_NBI_TMX_CSR_EGRESS_PULL_ID_PORT_ENABLE_EGRESS_PULL_ID_PORT_ENABLE_UNUSED (3)
/*----------------------------------------------------------------
  Register: OutOfOrderCount[32] - Out of order counter for each sequencer. There are a total of 32 out of order counters in the traffic manager, one for each active sequencer.
    Bits: 15:0  OutOfOrderCount(ro) - Out-of-order counter to keep track of the number of packets sent out of order for this sequencer. The counter will saturate when the maximum number is reached.
*/
#define NFP_NBI_TMX_CSR_OUT_OF_ORDER_COUNT(_x)               (0x00000100 + (0x4 * ((_x) & 0x1f)))
#define   NFP_NBI_TMX_CSR_OUT_OF_ORDER_COUNT_OUTOFORDERCOUNT(_x) (((_x) & 0xffff) << 0)
#define   NFP_NBI_TMX_CSR_OUT_OF_ORDER_COUNT_OUTOFORDERCOUNT_of(_x) (((_x) >> 0) & 0xffff)
/*----------------------------------------------------------------
  Register: OutOfOrderCountClear[32] - Clear on read out of order counter for each sequencer. One for each active sequencer.
    Bits: 15:0  OutOfOrderCountClear(rc) - Clear on read out-of-order counter to keep track of the number of packets sent out of order for this sequencer. The counter will saturate when the maximum number is reached. The read value for this register is identical to the OUTOFORDERCOUNT register value, however when this register is read the out-of-order counter is cleared.
*/
#define NFP_NBI_TMX_CSR_OUT_OF_ORDER_COUNT_CLEAR(_x)         (0x00000200 + (0x4 * ((_x) & 0x1f)))
#define   NFP_NBI_TMX_CSR_OUT_OF_ORDER_COUNT_CLEAR_OUTOFORDERCOUNTCLEAR(_x) (((_x) & 0xffff) << 0)
#define   NFP_NBI_TMX_CSR_OUT_OF_ORDER_COUNT_CLEAR_OUTOFORDERCOUNTCLEAR_of(_x) (((_x) >> 0) & 0xffff)
/*----------------------------------------------------------------
  Register: MiniPktCreditConfig - MiniPacket Credit Flow Control Configuration Register
    Bits: 24    MiniPacketFCMode(rw) - MiniPacket Credit Flow Control Mode bit. Select the free pool crediting mode to use.
    Bits: 21:12 FPCreditLimit(rw) - MiniPacket Transmit Free Pool Credit Counter Limit. Programmable limit for both of the free pool credit counters. The default value of 512 credits should be used for most applications using one free pool. The free pool has a reserve of 40 credits and should always be programmed to a value greater than 40.
    Bits:  9:0  CreditLimit(rw) - MiniPacket Transmit Channel Credit Counter Limit. Programmable limit for all of the channel credit counters
*/
#define NFP_NBI_TMX_CSR_MINIPKT_CREDIT_CONFIG                0x00000300
#define   NFP_NBI_TMX_CSR_MINIPKT_CREDIT_CONFIG_MINIPACKETFCMODE BIT(24)
#define     NFP_NBI_TMX_CSR_MINIPKT_CREDIT_CONFIG_MINIPACKETFCMODE_MODE_0 (0 << 24)
#define     NFP_NBI_TMX_CSR_MINIPKT_CREDIT_CONFIG_MINIPACKETFCMODE_MODE_1 BIT(24)
#define   NFP_NBI_TMX_CSR_MINIPKT_CREDIT_CONFIG_FPCREDITLIMIT(_x) (((_x) & 0x3ff) << 12)
#define   NFP_NBI_TMX_CSR_MINIPKT_CREDIT_CONFIG_FPCREDITLIMIT_of(_x) (((_x) >> 12) & 0x3ff)
#define   NFP_NBI_TMX_CSR_MINIPKT_CREDIT_CONFIG_CREDITLIMIT(_x) (((_x) & 0x3ff) << 0)
#define   NFP_NBI_TMX_CSR_MINIPKT_CREDIT_CONFIG_CREDITLIMIT_of(_x) (((_x) >> 0) & 0x3ff)
/*----------------------------------------------------------------
  Register: MiniPktFreePoolCredit[2] - MiniPacket Transmit Free Pool Credit Counter. Read only free pool credit counters
    Bits:  9:0  FPCreditCounter(ro) - MiniPacket Transmit Free Pool Credit Counter. Read only counter of the free pool credits remaining
*/
#define NFP_NBI_TMX_CSR_MINIPKT_FREE_POOL_CREDIT(_x)         (0x00000304 + (0x4 * ((_x) & 0x1)))
#define   NFP_NBI_TMX_CSR_MINIPKT_FREE_POOL_CREDIT_FPCREDITCOUNT(_x) (((_x) & 0x3ff) << 0)
#define   NFP_NBI_TMX_CSR_MINIPKT_FREE_POOL_CREDIT_FPCREDITCOUNT_of(_x) (((_x) >> 0) & 0x3ff)
/*----------------------------------------------------------------
  Register: MiniPktChannelCredit[128] - MiniPacket Transmit Channel Credit Counter. Read only credit counter, one for each channel
    Bits:  9:0  CreditCounter(ro) - MiniPacket Transmit Channel Credit Counter. Read only counter of the channel credits remaining
*/
#define NFP_NBI_TMX_CSR_MINIPKT_CHANNEL_CREDIT(_x)           (0x00000400 + (0x4 * ((_x) & 0x7f)))
#define   NFP_NBI_TMX_CSR_MINIPKT_CHANNEL_CREDIT_CREDITCOUNT(_x) (((_x) & 0x3ff) << 0)
#define   NFP_NBI_TMX_CSR_MINIPKT_CHANNEL_CREDIT_CREDITCOUNT_of(_x) (((_x) >> 0) & 0x3ff)
/*----------------------------------------------------------------
  Register: ReorderActivity - Activity indication flag for each sequencer in the reorder buffer.
    Bits: 31:0  ReorderActivity(ro) - Clear on read activity flag for each sequencer in the reorder buffer. One bit for each active sequencer in the reorder buffer.
*/
#define NFP_NBI_TMX_CSR_REORDER_ACTIVITY                     0x00000600
#define   NFP_NBI_TMX_CSR_REORDER_ACTIVITY_REORDERACTIVITY(_x) (((_x) & 0xffffffff) << 0)
#define   NFP_NBI_TMX_CSR_REORDER_ACTIVITY_REORDERACTIVITY_of(_x) (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: QueueStatus[1024] - Status information stored for each queue. There are a total of 1024 queue status registers in the traffic manager.
    Bits: 13    QueueFull(ro) - Queue Full register bit indicating the Descriptor Queue is Full or Almost Full
    Bits: 12:0  QueueLevel(ro) - Queue level register indicating the fill level for this particular queue. One 13-bit register for each descriptor queue.
*/
#define NFP_NBI_TM_Q_QUEUE_STATUS(_x)                        (0x00000000 + (0x4 * ((_x) & 0x3ff)))
#define   NFP_NBI_TM_Q_QUEUE_STATUS_QUEUEFULL                BIT(13)
#define   NFP_NBI_TM_Q_QUEUE_STATUS_QUEUELEVEL(_x)           (((_x) & 0x1fff) << 0)
#define   NFP_NBI_TM_Q_QUEUE_STATUS_QUEUELEVEL_of(_x)        (((_x) >> 0) & 0x1fff)
/*----------------------------------------------------------------
  Register: QueueConfig[1024] - Configuration settings for each queue. There are a total of 1024 queue configuration registers in the traffic manager.
    Bits: 10    DropRateRangeSelect(rw) - Select the Drop Rate Range set to use for this queue. There are two range sets A and B that may be used and are selectable per queue.
    Bits:  9:6  QueueSize(rw) - Configures the maximum size of this particular queue
    Bits:  5:3  DMAPacketThreshold(rw) - Queue level percentage threshold to determine when the packet data should be moved from CTM memory to main memory. One register each descriptor queue. The percentage threshold maintains an accuracy of plus or minus one descriptor
    Bits:  2    REDEnable(rw) - Enables random early detection for packet dropping for this particular queue. Packets will be discarded for this queue based on the globally configured drop rates for each queue range. The queue is divided into four ranges, the drop percentage is configurable for each range.
    Bits:  1    DropEnable(rw) - Enables packet dropping for this particular queue. Packets will be discarded for this queue when it is full or almost full. If REDEnable is set to 1, DropEnable should also be set to 1
    Bits:  0    QueueEnable(rw) - Enable bit for this particular queue. Set this bit to enable the queue, clear to disable.
*/
#define NFP_NBI_TM_Q_QUEUE_CONFIG(_x)                        (0x00001000 + (0x4 * ((_x) & 0x3ff)))
#define   NFP_NBI_TM_Q_QUEUE_CONFIG_DROPRATERANGESELECT      BIT(10)
#define     NFP_NBI_TM_Q_QUEUE_CONFIG_DROPRATERANGESELECT_A  (0 << 10)
#define     NFP_NBI_TM_Q_QUEUE_CONFIG_DROPRATERANGESELECT_B  BIT(10)
#define   NFP_NBI_TM_Q_QUEUE_CONFIG_QUEUESIZE(_x)            (((_x) & 0xf) << 6)
#define   NFP_NBI_TM_Q_QUEUE_CONFIG_QUEUESIZE_of(_x)         (((_x) >> 6) & 0xf)
#define     NFP_NBI_TM_Q_QUEUE_CONFIG_QUEUESIZE_8            (3)
#define     NFP_NBI_TM_Q_QUEUE_CONFIG_QUEUESIZE_16           (4)
#define     NFP_NBI_TM_Q_QUEUE_CONFIG_QUEUESIZE_32           (5)
#define     NFP_NBI_TM_Q_QUEUE_CONFIG_QUEUESIZE_64           (6)
#define     NFP_NBI_TM_Q_QUEUE_CONFIG_QUEUESIZE_128          (7)
#define     NFP_NBI_TM_Q_QUEUE_CONFIG_QUEUESIZE_256          (8)
#define     NFP_NBI_TM_Q_QUEUE_CONFIG_QUEUESIZE_512          (9)
#define     NFP_NBI_TM_Q_QUEUE_CONFIG_QUEUESIZE_1024         (10)
#define     NFP_NBI_TM_Q_QUEUE_CONFIG_QUEUESIZE_2048         (11)
#define     NFP_NBI_TM_Q_QUEUE_CONFIG_QUEUESIZE_4096         (12)
#define     NFP_NBI_TM_Q_QUEUE_CONFIG_QUEUESIZE_8192         (13)
#define   NFP_NBI_TM_Q_QUEUE_CONFIG_DMAPACKETTHRESHOLD(_x)   (((_x) & 0x7) << 3)
#define   NFP_NBI_TM_Q_QUEUE_CONFIG_DMAPACKETTHRESHOLD_of(_x) (((_x) >> 3) & 0x7)
#define     NFP_NBI_TM_Q_QUEUE_CONFIG_DMAPACKETTHRESHOLD_DEFAULT (0)
#define     NFP_NBI_TM_Q_QUEUE_CONFIG_DMAPACKETTHRESHOLD_12_5 (1)
#define     NFP_NBI_TM_Q_QUEUE_CONFIG_DMAPACKETTHRESHOLD_25  (2)
#define     NFP_NBI_TM_Q_QUEUE_CONFIG_DMAPACKETTHRESHOLD_37_5 (3)
#define     NFP_NBI_TM_Q_QUEUE_CONFIG_DMAPACKETTHRESHOLD_50  (4)
#define     NFP_NBI_TM_Q_QUEUE_CONFIG_DMAPACKETTHRESHOLD_62_5 (5)
#define     NFP_NBI_TM_Q_QUEUE_CONFIG_DMAPACKETTHRESHOLD_75  (6)
#define     NFP_NBI_TM_Q_QUEUE_CONFIG_DMAPACKETTHRESHOLD_87_5 (7)
#define   NFP_NBI_TM_Q_QUEUE_CONFIG_REDENABLE                BIT(2)
#define     NFP_NBI_TM_Q_QUEUE_CONFIG_REDENABLE_DISABLED     (0 << 2)
#define     NFP_NBI_TM_Q_QUEUE_CONFIG_REDENABLE_ENABLED      BIT(2)
#define   NFP_NBI_TM_Q_QUEUE_CONFIG_DROPENABLE               BIT(1)
#define     NFP_NBI_TM_Q_QUEUE_CONFIG_DROPENABLE_DISABLED    (0 << 1)
#define     NFP_NBI_TM_Q_QUEUE_CONFIG_DROPENABLE_ENABLED     BIT(1)
#define   NFP_NBI_TM_Q_QUEUE_CONFIG_QUEUEENABLE              BIT(0)
#define     NFP_NBI_TM_Q_QUEUE_CONFIG_QUEUEENABLE_DISABLE    (0 << 0)
#define     NFP_NBI_TM_Q_QUEUE_CONFIG_QUEUEENABLE_ENABLE     BIT(0)
/*----------------------------------------------------------------
  Register: QueueDropCount[1024] - Drop counter for each queue. There are a total of 1024 drop counters in the traffic manager.
    Bits: 31:0  DropCount(ro) - 32-bit drop counter to keep track of the number of packets dropped for this queue. The counter will saturate when the maximum number is reached.
*/
#define NFP_NBI_TM_Q_QUEUE_DROP_COUNT(_x)                    (0x00002000 + (0x4 * ((_x) & 0x3ff)))
#define   NFP_NBI_TM_Q_QUEUE_DROP_COUNT_DROPCOUNT(_x)        (((_x) & 0xffffffff) << 0)
#define   NFP_NBI_TM_Q_QUEUE_DROP_COUNT_DROPCOUNT_of(_x)     (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: QueueDropCountClear[1024] - Clear on read drop counter for each queue. There are a total of 1024 clear on read drop counters in the traffic manager.
    Bits: 31:0  DropCountClear(rc) - Clear on read 32-bit drop counter to keep track of the number of packets dropped for this queue. The counter will saturate when the maximum number is reached. The read value for this register is identical to the DROPCOUNT register value, however when DROPCOUNTCLEAR is read the drop counter is cleared. The drop count memory is undefined at start-up. This register should be read for all active queues by software prior to device operation.
*/
#define NFP_NBI_TM_Q_QUEUE_DROP_COUNT_CLEAR(_x)              (0x00003000 + (0x4 * ((_x) & 0x3ff)))
#define   NFP_NBI_TM_Q_QUEUE_DROP_COUNT_CLEAR_DROPCOUNTCLEAR(_x) (((_x) & 0xffffffff) << 0)
#define   NFP_NBI_TM_Q_QUEUE_DROP_COUNT_CLEAR_DROPCOUNTCLEAR_of(_x) (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: SchedulerConfig[145] - Configuration register for setup of each scheduler. There are a total of 145 scheduler configuration registers in the traffic manager. The level 2 schedulers are at offsets 0-127, the level 1 schedulers are at offsets 128-143, and the level 0 scheduler is at offset 144.
    Bits:  2    SP1Enable(rw) - Strict Priority One Enable bit. Set this bit to enable scheduler port 1 as the strict priority 1 port. SP1 will have priority over all other ports except for SP0.
    Bits:  1    SP0Enable(rw) - Strict Priority Zero Enable bit. Set this bit to enable scheduler port 0 as the strict priority 0 port. SP0 will have priority over all other ports, including SP1.
    Bits:  0    DWRREnable(rw) - Deficit Weighted Round Robin Enable bit. Set this bit to enable deficit weighted round operation for the particular scheduler.
*/
#define NFP_NBI_TMX_SCH_CSR_SCHEDULER_CONFIG(_x)             (0x00000000 + (0x4 * ((_x) & 0xff)))
#define   NFP_NBI_TMX_SCH_CSR_SCHEDULER_CONFIG_SP1ENABLE     BIT(2)
#define     NFP_NBI_TMX_SCH_CSR_SCHEDULER_CONFIG_SP1ENABLE_DISABLE (0 << 2)
#define     NFP_NBI_TMX_SCH_CSR_SCHEDULER_CONFIG_SP1ENABLE_ENABLE BIT(2)
#define   NFP_NBI_TMX_SCH_CSR_SCHEDULER_CONFIG_SP0ENABLE     BIT(1)
#define     NFP_NBI_TMX_SCH_CSR_SCHEDULER_CONFIG_SP0ENABLE_DISABLE (0 << 1)
#define     NFP_NBI_TMX_SCH_CSR_SCHEDULER_CONFIG_SP0ENABLE_ENABLE BIT(1)
#define   NFP_NBI_TMX_SCH_CSR_SCHEDULER_CONFIG_DWRRENABLE    BIT(0)
#define     NFP_NBI_TMX_SCH_CSR_SCHEDULER_CONFIG_DWRRENABLE_DISABLE (0 << 0)
#define     NFP_NBI_TMX_SCH_CSR_SCHEDULER_CONFIG_DWRRENABLE_ENABLE BIT(0)
/*----------------------------------------------------------------
  Register: SchedulerWeight[128] - L0 Scheduler weight value to configure the relative bandwidth for each scheduler port. There are a total of 128 weight values to program in the level 0 scheduler.
    Bits: 23:0  Weight(rw) - Scheduler weight value. Program this memory with the desired weight value for the particular scheduler port. This memory is undefined at start-up and should be programmed or set to zero by software. If all weights for all ports of a particular port are set to zero, the scheduler will operate in round robin fashion. The weight value will configure the relative bandwidth for the port and is used to update the deficit value. The weight should be programmed to the desired relative bandwidth for this port as compared to the other ports on this scheduler.
*/
#define NFP_NBI_TMX_SCH_CSR_SCHEDULER_WEIGHT(_x)             (0x00000800 + (0x4 * ((_x) & 0x7f)))
#define   NFP_NBI_TMX_SCH_CSR_SCHEDULER_WEIGHT_WEIGHT(_x)    (((_x) & 0xffffff) << 0)
#define   NFP_NBI_TMX_SCH_CSR_SCHEDULER_WEIGHT_WEIGHT_of(_x) (((_x) >> 0) & 0xffffff)
/*----------------------------------------------------------------
  Register: SchedulerDeficit[128] - L0 Scheduler deficit value for each scheduler port. There are a total of 128 deficit values in the level 0 scheduler.
    Bits: 24:0  Deficit(rw) - Scheduler deficit value. This memory provides the signed value of the deficit for the particular scheduler port. This memory is undefined at start-up and should be programmed to zero by software. If all of the ports either have a negative deficit or do not have data to send, the deficit values will be updated for all ports. During the update if the port does not have data to send the deficit will be set to the weight value. If the deficit has gone negative, the weight value will be added to the current deficit.
*/
#define NFP_NBI_TMX_SCH_CSR_SCHEDULER_DEFICIT(_x)            (0x00001000 + (0x4 * ((_x) & 0x7f)))
#define   NFP_NBI_TMX_SCH_CSR_SCHEDULER_DEFICIT_DEFICIT(_x)  (((_x) & 0x1ffffff) << 0)
#define   NFP_NBI_TMX_SCH_CSR_SCHEDULER_DEFICIT_DEFICIT_of(_x) (((_x) >> 0) & 0x1ffffff)
/*----------------------------------------------------------------
  Register: ShaperRate[145] - Shaper rate register. Configures the maximum allowed rate. There are a total of 145 shaper rate registers in the traffic manager. The level 2 shaper registers are at offsets 0-127, the level 1 shaper registers are at offsets 128-143, and the level 0 shaper register is at offset 144.
    Bits: 13:0  Rate(rw) - The shaper rate register. This register is programmed with the desired maximum rate in 10Megabit per second increments for the associated port or queue. Valid values may range in size from 1 to 12,000 and correspond to data rates of 10Mbs to 120Gbps when using a 1.0GHz PCLK. Values outside that range are not supported (i.e. 0, or above 12,000). If other PCLK frequencies are used, the shaper rate value should be multiplied by (PCLK frequency/1.0GHz) for proper shaper accuracy.
*/
#define NFP_NBI_TMX_SHP_SHAPER_RATE(_x)                      (0x00000000 + (0x4 * ((_x) & 0xff)))
#define   NFP_NBI_TMX_SHP_SHAPER_RATE_RATE(_x)               (((_x) & 0x3fff) << 0)
#define   NFP_NBI_TMX_SHP_SHAPER_RATE_RATE_of(_x)            (((_x) >> 0) & 0x3fff)
/*----------------------------------------------------------------
  Register: ShaperThreshold[145] - Rate limiting shaper threshold register. Configures the desired threshold level of the leaky bucket. There are a total of 145 shaper threshold registers in the traffic manager. The level 2 shaper registers are at offsets 0-127, the level 1 shaper registers are at offsets 128-143, and the level 0 shaper register is at offset 144.
    Bits:  2:0  Threshold(rw) - The rate limiting shaper threshold register. This register configures the threshold in bytes used for comparison to the instantaneous level of the leaky bucket. When the number of bytes sent is below this threshold the shaper will be open, when the bucket level equals or exceeds the threshold the shaper will be closed.
*/
#define NFP_NBI_TMX_SHP_SHAPER_THRESHOLD(_x)                 (0x00000800 + (0x4 * ((_x) & 0xff)))
#define   NFP_NBI_TMX_SHP_SHAPER_THRESHOLD_THRESHOLD(_x)     (((_x) & 0x7) << 0)
#define   NFP_NBI_TMX_SHP_SHAPER_THRESHOLD_THRESHOLD_of(_x)  (((_x) >> 0) & 0x7)
#define     NFP_NBI_TMX_SHP_SHAPER_THRESHOLD_THRESHOLD_8192  (0)
#define     NFP_NBI_TMX_SHP_SHAPER_THRESHOLD_THRESHOLD_16384 (1)
#define     NFP_NBI_TMX_SHP_SHAPER_THRESHOLD_THRESHOLD_24576 (2)
#define     NFP_NBI_TMX_SHP_SHAPER_THRESHOLD_THRESHOLD_32768 (3)
#define     NFP_NBI_TMX_SHP_SHAPER_THRESHOLD_THRESHOLD_40960 (4)
#define     NFP_NBI_TMX_SHP_SHAPER_THRESHOLD_THRESHOLD_49152 (5)
#define     NFP_NBI_TMX_SHP_SHAPER_THRESHOLD_THRESHOLD_57344 (6)
#define     NFP_NBI_TMX_SHP_SHAPER_THRESHOLD_THRESHOLD_65535 (7)
/*----------------------------------------------------------------
  Register: ShaperMaxOvershoot[145] - Rate limiting shaper maximum overshoot register. Configures the maximum allowed overshoot for the leaky bucket. There are a total of 145 shaper maximum overshoot registers in the traffic manager. The level 2 shaper registers are at offsets 0-127, the level 1 shaper registers are at offsets 128-143, and the level 0 shaper register is at offset 144
    Bits:  2:0  MaxOvershoot(rw) - The rate limiting shaper maximum overshoot register. This register configures the maximum allowed overshoot in bytes for the leaky bucket. Data that is sent beyond this limit will not be accounted for in the leaky bucket algorithm.
*/
#define NFP_NBI_TMX_SHP_SHAPER_MAX_OVERSHOOT(_x)             (0x00001000 + (0x4 * ((_x) & 0xff)))
#define   NFP_NBI_TMX_SHP_SHAPER_MAX_OVERSHOOT_MAXOVERSHOOT(_x) (((_x) & 0x7) << 0)
#define   NFP_NBI_TMX_SHP_SHAPER_MAX_OVERSHOOT_MAXOVERSHOOT_of(_x) (((_x) >> 0) & 0x7)
#define     NFP_NBI_TMX_SHP_SHAPER_MAX_OVERSHOOT_MAXOVERSHOOT_16384 (0)
#define     NFP_NBI_TMX_SHP_SHAPER_MAX_OVERSHOOT_MAXOVERSHOOT_24576 (1)
#define     NFP_NBI_TMX_SHP_SHAPER_MAX_OVERSHOOT_MAXOVERSHOOT_32768 (2)
#define     NFP_NBI_TMX_SHP_SHAPER_MAX_OVERSHOOT_MAXOVERSHOOT_40960 (3)
#define     NFP_NBI_TMX_SHP_SHAPER_MAX_OVERSHOOT_MAXOVERSHOOT_49152 (4)
#define     NFP_NBI_TMX_SHP_SHAPER_MAX_OVERSHOOT_MAXOVERSHOOT_57344 (5)
#define     NFP_NBI_TMX_SHP_SHAPER_MAX_OVERSHOOT_MAXOVERSHOOT_65536 (6)
#define     NFP_NBI_TMX_SHP_SHAPER_MAX_OVERSHOOT_MAXOVERSHOOT_131071 (7)
/*----------------------------------------------------------------
  Register: ShaperRateAdjust[145] - Shaper rate adjust register. Configures the fixed data rate adjustment for packet overhead. There are a total of 145 shaper rate adjust registers in the traffic manager. The level 2 shaper registers are at offsets 0-127, the level 1 shaper registers are at offsets 128-143, and the level 0 shaper register is at offset 144.
    Bits:  9:0  RateAdjust(rw) - The shaper rate adjust register. This register contains the standard number of bytes to be subtracted for every packet that is transferred. This is used to account for packet overhead and metadata that is not to be included in the shaper rate calculation. In cases where the rate adjust value is larger than the packet size, the difference will be deducted from the shaper bucket.
*/
#define NFP_NBI_TMX_SHP_SHAPER_RATE_ADJUST(_x)               (0x00001800 + (0x4 * ((_x) & 0xff)))
#define   NFP_NBI_TMX_SHP_SHAPER_RATE_ADJUST_RATE(_x)        (((_x) & 0x3ff) << 0)
#define   NFP_NBI_TMX_SHP_SHAPER_RATE_ADJUST_RATE_of(_x)     (((_x) >> 0) & 0x3ff)
/*----------------------------------------------------------------
  Register: ShaperStatus[145] - Shaper status register. Contains read-only status information for the leaky bucket rate liming shaper. There are a total of 145 shaper status registers in the traffic manager. The level 2 shaper registers are at offsets 0-127, the level 1 shaper registers are at offsets 128-143, and the level 0 shaper register is at offset 144.
    Bits: 17    ShaperOpen(ro) - Indicates the state of the leaky bucket
    Bits: 16:0  Level(ro) - The current leaky bucket level of the rate limiting shaper. This register contains the present level of the leaky bucket that is used to determine open or closed state. If this value is below the threshold the shaper will be open, if it is above the threshold the shaper will be closed.
*/
#define NFP_NBI_TMX_SHP_SHAPER_STATUS(_x)                    (0x00002000 + (0x4 * ((_x) & 0xff)))
#define   NFP_NBI_TMX_SHP_SHAPER_STATUS_SHAPEROPEN           BIT(17)
#define     NFP_NBI_TMX_SHP_SHAPER_STATUS_SHAPEROPEN_CLOSED  (0 << 17)
#define     NFP_NBI_TMX_SHP_SHAPER_STATUS_SHAPEROPEN_OPEN    BIT(17)
#define   NFP_NBI_TMX_SHP_SHAPER_STATUS_LEVEL(_x)            (((_x) & 0x1ffff) << 0)
#define   NFP_NBI_TMX_SHP_SHAPER_STATUS_LEVEL_of(_x)         (((_x) >> 0) & 0x1ffff)

#else /* NFP6000_LONGNAMES */

/*----------------------------------------------------------------
  Register: TMDescSram_TMDescSramEntry[32768] - NBI TM Descriptor SRAM Entry
    Bits: 63:0  DescSRAMData(rw) - 64-bits of the 128-bit descriptor in the TM Descriptor SRAM. Refer to the Packet Descriptor Details table for details on bit definitions
*/
#define NFP_NBI_TM_TMDescSram_TMDescSramEntry(_x)            (0x00000000 + (0x8 * ((_x) & 0x7fff)))
#define   NFP_NBI_TM_TMDescSram_TMDescSramEntry_DescSRAMData(_x) (((_x) & 0xffffffffffffffff) << 0)
#define   NFP_NBI_TM_TMDescSram_TMDescSramEntry_DescSRAMData_of(_x) (((_x) >> 0) & 0xffffffffffffffff)
/*----------------------------------------------------------------
  Register: TMBDSram_NbiBDSramEntry[4096] - NBI TM Buffer Descriptor SRAM Entry
    Bits: 28:0  MuAddr(rw) - Memory Unit Address of the buffer (internal/external MU). Maps to CPP Address[29;11] Refer to the NBI top-level Master Commands section for memory unit address translation.
*/
#define NFP_NBI_TM_TMBDSram_NbiBDSramEntry(_x)               (0x00040000 + (0x8 * ((_x) & 0xfff)))
#define   NFP_NBI_TM_TMBDSram_NbiBDSramEntry_MuAddr(_x)      (((_x) & 0x1fffffff) << 0)
#define   NFP_NBI_TM_TMBDSram_NbiBDSramEntry_MuAddr_of(_x)   (((_x) >> 0) & 0x1fffffff)
/*----------------------------------------------------------------
  Register: TMBufferList_BLQueCtrl0 - Configures the buffer list circular queue and provides status
    Bits: 55:48 BLQEventFullCnt(rc) - Debug counter that increments when a BLQ Event is dropped due to a full BLQ Event FIFO. This counter saturates at all 1's
    Bits: 42:40 BDFifoEntryCnt(ro) - Number of buffer descriptors resident in the buffer descriptor register FIFO
    Bits: 39    BLQFull(ro) - The Queue is full when set.
    Bits: 38    BLQEmpty(ro) - The queue is empty when set.
    Bits: 37:36 BLQSize(rw) - Defines the Size of the Circular Buffer List Queue
    Bits: 35:24 BLEntryCnt(ro) - Hardware calculated value of the number of entries on the Buffer List Queue. A full Buffer List Queue has an entry count equal to zero, with the BLQFull flag set. An empty Buffer List Queue has an entry count equal to zero with the BLQEmpty flag set.
    Bits: 23:12 BLHdPtr(rw) - Buffer List Queue Head Pointer. Points to the next entry to be read by hardware.
    Bits: 11:0  BLTlPtr(rw) - Buffer List Queue Tail Pointer. Points to the next entry to be written at the tail.
*/
#define NFP_NBI_TM_TMBufferList_BLQueCtrl0                   0x00048000
#define   NFP_NBI_TM_TMBufferList_BLQueCtrl0_BLQEventFullCnt(_x) (((_x) & 0xff) << 48)
#define   NFP_NBI_TM_TMBufferList_BLQueCtrl0_BLQEventFullCnt_of(_x) (((_x) >> 48) & 0xff)
#define   NFP_NBI_TM_TMBufferList_BLQueCtrl0_BDFifoEntryCnt(_x) (((_x) & 0x7) << 40)
#define   NFP_NBI_TM_TMBufferList_BLQueCtrl0_BDFifoEntryCnt_of(_x) (((_x) >> 40) & 0x7)
#define   NFP_NBI_TM_TMBufferList_BLQueCtrl0_BLQFull         BIT_ULL(39)
#define   NFP_NBI_TM_TMBufferList_BLQueCtrl0_BLQEmpty        BIT_ULL(38)
#define   NFP_NBI_TM_TMBufferList_BLQueCtrl0_BLQSize(_x)     (((_x) & 0x3) << 36)
#define   NFP_NBI_TM_TMBufferList_BLQueCtrl0_BLQSize_of(_x)  (((_x) >> 36) & 0x3)
#define     NFP_NBI_TM_TMBufferList_BLQueCtrl0_BLQSize_512   (0)
#define     NFP_NBI_TM_TMBufferList_BLQueCtrl0_BLQSize_1K    (1)
#define     NFP_NBI_TM_TMBufferList_BLQueCtrl0_BLQSize_2K    (2)
#define     NFP_NBI_TM_TMBufferList_BLQueCtrl0_BLQSize_4K    (3)
#define   NFP_NBI_TM_TMBufferList_BLQueCtrl0_BLEntryCnt(_x)  (((_x) & 0xfff) << 24)
#define   NFP_NBI_TM_TMBufferList_BLQueCtrl0_BLEntryCnt_of(_x) (((_x) >> 24) & 0xfff)
#define   NFP_NBI_TM_TMBufferList_BLQueCtrl0_BLHdPtr(_x)     (((_x) & 0xfff) << 12)
#define   NFP_NBI_TM_TMBufferList_BLQueCtrl0_BLHdPtr_of(_x)  (((_x) >> 12) & 0xfff)
#define   NFP_NBI_TM_TMBufferList_BLQueCtrl0_BLTlPtr(_x)     (((_x) & 0xfff) << 0)
#define   NFP_NBI_TM_TMBufferList_BLQueCtrl0_BLTlPtr_of(_x)  (((_x) >> 0) & 0xfff)
/*----------------------------------------------------------------
  Register: TMBufferList_BLQueCtrl1 - Configures the buffer list circular queue and provides status
    Bits: 55:48 BLQEventFullCnt(rc) - Debug counter that increments when a BLQ Event is dropped due to a full BLQ Event FIFO. This counter saturates at all 1's
    Bits: 42:40 BDFifoEntryCnt(ro) - Number of buffer descriptors resident in the buffer descriptor register FIFO
    Bits: 39    BLQFull(ro) - The Queue is full when set.
    Bits: 38    BLQEmpty(ro) - The queue is empty when set.
    Bits: 37:36 BLQSize(rw) - Defines the Size of the Circular Buffer List Queue
    Bits: 35:24 BLEntryCnt(ro) - Hardware calculated value of the number of entries on the Buffer List Queue. A full Buffer List Queue has an entry count equal to zero, with the BLQFull flag set. An empty Buffer List Queue has an entry count equal to zero with the BLQEmpty flag set.
    Bits: 23:12 BLHdPtr(rw) - Buffer List Queue Head Pointer. Points to the next entry to be read by hardware.
    Bits: 11:0  BLTlPtr(rw) - Buffer List Queue Tail Pointer. Points to the next entry to be written at the tail.
*/
#define NFP_NBI_TM_TMBufferList_BLQueCtrl1                   0x00048008
#define   NFP_NBI_TM_TMBufferList_BLQueCtrl1_BLQEventFullCnt(_x) (((_x) & 0xff) << 48)
#define   NFP_NBI_TM_TMBufferList_BLQueCtrl1_BLQEventFullCnt_of(_x) (((_x) >> 48) & 0xff)
#define   NFP_NBI_TM_TMBufferList_BLQueCtrl1_BDFifoEntryCnt(_x) (((_x) & 0x7) << 40)
#define   NFP_NBI_TM_TMBufferList_BLQueCtrl1_BDFifoEntryCnt_of(_x) (((_x) >> 40) & 0x7)
#define   NFP_NBI_TM_TMBufferList_BLQueCtrl1_BLQFull         BIT_ULL(39)
#define   NFP_NBI_TM_TMBufferList_BLQueCtrl1_BLQEmpty        BIT_ULL(38)
#define   NFP_NBI_TM_TMBufferList_BLQueCtrl1_BLQSize(_x)     (((_x) & 0x3) << 36)
#define   NFP_NBI_TM_TMBufferList_BLQueCtrl1_BLQSize_of(_x)  (((_x) >> 36) & 0x3)
#define     NFP_NBI_TM_TMBufferList_BLQueCtrl1_BLQSize_512   (0)
#define     NFP_NBI_TM_TMBufferList_BLQueCtrl1_BLQSize_1K    (1)
#define     NFP_NBI_TM_TMBufferList_BLQueCtrl1_BLQSize_2K    (2)
#define     NFP_NBI_TM_TMBufferList_BLQueCtrl1_BLQSize_4K    (3)
#define   NFP_NBI_TM_TMBufferList_BLQueCtrl1_BLEntryCnt(_x)  (((_x) & 0xfff) << 24)
#define   NFP_NBI_TM_TMBufferList_BLQueCtrl1_BLEntryCnt_of(_x) (((_x) >> 24) & 0xfff)
#define   NFP_NBI_TM_TMBufferList_BLQueCtrl1_BLHdPtr(_x)     (((_x) & 0xfff) << 12)
#define   NFP_NBI_TM_TMBufferList_BLQueCtrl1_BLHdPtr_of(_x)  (((_x) >> 12) & 0xfff)
#define   NFP_NBI_TM_TMBufferList_BLQueCtrl1_BLTlPtr(_x)     (((_x) & 0xfff) << 0)
#define   NFP_NBI_TM_TMBufferList_BLQueCtrl1_BLTlPtr_of(_x)  (((_x) >> 0) & 0xfff)
/*----------------------------------------------------------------
  Register: TMBufferList_BLQueCtrl2 - Configures the buffer list circular queue and provides status
    Bits: 55:48 BLQEventFullCnt(rc) - Debug counter that increments when a BLQ Event is dropped due to a full BLQ Event FIFO. This counter saturates at all 1's
    Bits: 42:40 BDFifoEntryCnt(ro) - Number of buffer descriptors resident in the buffer descriptor register FIFO
    Bits: 39    BLQFull(ro) - The Queue is full when set.
    Bits: 38    BLQEmpty(ro) - The queue is empty when set.
    Bits: 37:36 BLQSize(rw) - Defines the Size of the Circular Buffer List Queue
    Bits: 35:24 BLEntryCnt(ro) - Hardware calculated value of the number of entries on the Buffer List Queue. A full Buffer List Queue has an entry count equal to zero, with the BLQFull flag set. An empty Buffer List Queue has an entry count equal to zero with the BLQEmpty flag set.
    Bits: 23:12 BLHdPtr(rw) - Buffer List Queue Head Pointer. Points to the next entry to be read by hardware.
    Bits: 11:0  BLTlPtr(rw) - Buffer List Queue Tail Pointer. Points to the next entry to be written at the tail.
*/
#define NFP_NBI_TM_TMBufferList_BLQueCtrl2                   0x00048010
#define   NFP_NBI_TM_TMBufferList_BLQueCtrl2_BLQEventFullCnt(_x) (((_x) & 0xff) << 48)
#define   NFP_NBI_TM_TMBufferList_BLQueCtrl2_BLQEventFullCnt_of(_x) (((_x) >> 48) & 0xff)
#define   NFP_NBI_TM_TMBufferList_BLQueCtrl2_BDFifoEntryCnt(_x) (((_x) & 0x7) << 40)
#define   NFP_NBI_TM_TMBufferList_BLQueCtrl2_BDFifoEntryCnt_of(_x) (((_x) >> 40) & 0x7)
#define   NFP_NBI_TM_TMBufferList_BLQueCtrl2_BLQFull         BIT_ULL(39)
#define   NFP_NBI_TM_TMBufferList_BLQueCtrl2_BLQEmpty        BIT_ULL(38)
#define   NFP_NBI_TM_TMBufferList_BLQueCtrl2_BLQSize(_x)     (((_x) & 0x3) << 36)
#define   NFP_NBI_TM_TMBufferList_BLQueCtrl2_BLQSize_of(_x)  (((_x) >> 36) & 0x3)
#define     NFP_NBI_TM_TMBufferList_BLQueCtrl2_BLQSize_512   (0)
#define     NFP_NBI_TM_TMBufferList_BLQueCtrl2_BLQSize_1K    (1)
#define     NFP_NBI_TM_TMBufferList_BLQueCtrl2_BLQSize_2K    (2)
#define     NFP_NBI_TM_TMBufferList_BLQueCtrl2_BLQSize_4K    (3)
#define   NFP_NBI_TM_TMBufferList_BLQueCtrl2_BLEntryCnt(_x)  (((_x) & 0xfff) << 24)
#define   NFP_NBI_TM_TMBufferList_BLQueCtrl2_BLEntryCnt_of(_x) (((_x) >> 24) & 0xfff)
#define   NFP_NBI_TM_TMBufferList_BLQueCtrl2_BLHdPtr(_x)     (((_x) & 0xfff) << 12)
#define   NFP_NBI_TM_TMBufferList_BLQueCtrl2_BLHdPtr_of(_x)  (((_x) >> 12) & 0xfff)
#define   NFP_NBI_TM_TMBufferList_BLQueCtrl2_BLTlPtr(_x)     (((_x) & 0xfff) << 0)
#define   NFP_NBI_TM_TMBufferList_BLQueCtrl2_BLTlPtr_of(_x)  (((_x) >> 0) & 0xfff)
/*----------------------------------------------------------------
  Register: TMBufferList_BLQueCtrl3 - Configures the buffer list circular queue and provides status
    Bits: 55:48 BLQEventFullCnt(rc) - Debug counter that increments when a BLQ Event is dropped due to a full BLQ Event FIFO. This counter saturates at all 1's
    Bits: 42:40 BDFifoEntryCnt(ro) - Number of buffer descriptors resident in the buffer descriptor register FIFO
    Bits: 39    BLQFull(ro) - The Queue is full when set.
    Bits: 38    BLQEmpty(ro) - The queue is empty when set.
    Bits: 37:36 BLQSize(rw) - Defines the Size of the Circular Buffer List Queue
    Bits: 35:24 BLEntryCnt(ro) - Hardware calculated value of the number of entries on the Buffer List Queue. A full Buffer List Queue has an entry count equal to zero, with the BLQFull flag set. An empty Buffer List Queue has an entry count equal to zero with the BLQEmpty flag set.
    Bits: 23:12 BLHdPtr(rw) - Buffer List Queue Head Pointer. Points to the next entry to be read by hardware.
    Bits: 11:0  BLTlPtr(rw) - Buffer List Queue Tail Pointer. Points to the next entry to be written at the tail.
*/
#define NFP_NBI_TM_TMBufferList_BLQueCtrl3                   0x00048018
#define   NFP_NBI_TM_TMBufferList_BLQueCtrl3_BLQEventFullCnt(_x) (((_x) & 0xff) << 48)
#define   NFP_NBI_TM_TMBufferList_BLQueCtrl3_BLQEventFullCnt_of(_x) (((_x) >> 48) & 0xff)
#define   NFP_NBI_TM_TMBufferList_BLQueCtrl3_BDFifoEntryCnt(_x) (((_x) & 0x7) << 40)
#define   NFP_NBI_TM_TMBufferList_BLQueCtrl3_BDFifoEntryCnt_of(_x) (((_x) >> 40) & 0x7)
#define   NFP_NBI_TM_TMBufferList_BLQueCtrl3_BLQFull         BIT_ULL(39)
#define   NFP_NBI_TM_TMBufferList_BLQueCtrl3_BLQEmpty        BIT_ULL(38)
#define   NFP_NBI_TM_TMBufferList_BLQueCtrl3_BLQSize(_x)     (((_x) & 0x3) << 36)
#define   NFP_NBI_TM_TMBufferList_BLQueCtrl3_BLQSize_of(_x)  (((_x) >> 36) & 0x3)
#define     NFP_NBI_TM_TMBufferList_BLQueCtrl3_BLQSize_512   (0)
#define     NFP_NBI_TM_TMBufferList_BLQueCtrl3_BLQSize_1K    (1)
#define     NFP_NBI_TM_TMBufferList_BLQueCtrl3_BLQSize_2K    (2)
#define     NFP_NBI_TM_TMBufferList_BLQueCtrl3_BLQSize_4K    (3)
#define   NFP_NBI_TM_TMBufferList_BLQueCtrl3_BLEntryCnt(_x)  (((_x) & 0xfff) << 24)
#define   NFP_NBI_TM_TMBufferList_BLQueCtrl3_BLEntryCnt_of(_x) (((_x) >> 24) & 0xfff)
#define   NFP_NBI_TM_TMBufferList_BLQueCtrl3_BLHdPtr(_x)     (((_x) & 0xfff) << 12)
#define   NFP_NBI_TM_TMBufferList_BLQueCtrl3_BLHdPtr_of(_x)  (((_x) >> 12) & 0xfff)
#define   NFP_NBI_TM_TMBufferList_BLQueCtrl3_BLTlPtr(_x)     (((_x) & 0xfff) << 0)
#define   NFP_NBI_TM_TMBufferList_BLQueCtrl3_BLTlPtr_of(_x)  (((_x) >> 0) & 0xfff)
/*----------------------------------------------------------------
  Register: TMSlowDescSram_TMSlowDescSramEntry[2048] - NBI TM Slow Descriptor Entry
    Bits: 63:0  TmSlowDescData(rw) - 64-bits of the 128-bit descriptor in the TM Slow Descriptor SRAM. Refer to the Packet Descriptor Details table for details on bit definitions
*/
#define NFP_NBI_TM_TMSlowDescSram_TMSlowDescSramEntry(_x)    (0x00050000 + (0x8 * ((_x) & 0x7ff)))
#define   NFP_NBI_TM_TMSlowDescSram_TMSlowDescSramEntry_TmSlowDescData(_x) (((_x) & 0xffffffffffffffff) << 0)
#define   NFP_NBI_TM_TMSlowDescSram_TMSlowDescSramEntry_TmSlowDescData_of(_x) (((_x) >> 0) & 0xffffffffffffffff)
/*----------------------------------------------------------------
  Register: TMReorderBuf_TMReorderBufEntry[2048] - NBI TM Reorder Buffer Entry
    Bits: 63:0  TmReorderBufData(rw) - 64-bits of the 128-bit descriptor in the TM Reorder Buffer SRAM. Refer to the Packet Descriptor Details table for details on bit definitions
*/
#define NFP_NBI_TM_TMReorderBuf_TMReorderBufEntry(_x)        (0x00058000 + (0x8 * ((_x) & 0x7ff)))
#define   NFP_NBI_TM_TMReorderBuf_TMReorderBufEntry_TmReorderBufData(_x) (((_x) & 0xffffffffffffffff) << 0)
#define   NFP_NBI_TM_TMReorderBuf_TMReorderBufEntry_TmReorderBufData_of(_x) (((_x) >> 0) & 0xffffffffffffffff)
/*----------------------------------------------------------------
  Register: TMPktSram_TMPktSramEntry[2048] - NBI TM Packet SRAM Entry
    Bits: 63:0  TMPktSRAMData(rw) - 64-bits of packet data within the TM Packet Buffer SRAM. Refer to Packet Buffer Entry for details
*/
#define NFP_NBI_TM_TMPktSram_TMPktSramEntry(_x)              (0x00060000 + (0x8 * ((_x) & 0x7ff)))
#define   NFP_NBI_TM_TMPktSram_TMPktSramEntry_TMPktSRAMData(_x) (((_x) & 0xffffffffffffffff) << 0)
#define   NFP_NBI_TM_TMPktSram_TMPktSramEntry_TMPktSRAMData_of(_x) (((_x) >> 0) & 0xffffffffffffffff)
/*----------------------------------------------------------------
  Register: TMHeadTailSram_TMHeadTailEntry[1024] - NBI TM Head and Tail SRAM Entry
    Bits: 27:14 HeadPointer(rw) - 14-bit Descriptor Queue Head Pointer
    Bits: 13:0  TailPointer(rw) - 14-bit Descriptor Queue Tail Pointer
*/
#define NFP_NBI_TM_TMHeadTailSram_TMHeadTailEntry(_x)        (0x00068000 + (0x8 * ((_x) & 0x3ff)))
#define   NFP_NBI_TM_TMHeadTailSram_TMHeadTailEntry_HeadPointer(_x) (((_x) & 0x3fff) << 14)
#define   NFP_NBI_TM_TMHeadTailSram_TMHeadTailEntry_HeadPointer_of(_x) (((_x) >> 14) & 0x3fff)
#define   NFP_NBI_TM_TMHeadTailSram_TMHeadTailEntry_TailPointer(_x) (((_x) & 0x3fff) << 0)
#define   NFP_NBI_TM_TMHeadTailSram_TMHeadTailEntry_TailPointer_of(_x) (((_x) >> 0) & 0x3fff)
/*----------------------------------------------------------------
  Register: TMFlowControl_TMFlowControlEntry[18] - Reads 64-bits at a time of Traffic Manager Flow Control information
    Bits: 63:0  FlowControlData(ro) - Read only access to the 1024 bits of Out of Band Flow control, and 128-bits of XOFF flow control from the MAC.
*/
#define NFP_NBI_TM_TMFlowControl_TMFlowControlEntry(_x)      (0x00070000 + (0x8 * ((_x) & 0x1f)))
#define   NFP_NBI_TM_TMFlowControl_TMFlowControlEntry_FlowControlData(_x) (((_x) & 0xffffffffffffffff) << 0)
#define   NFP_NBI_TM_TMFlowControl_TMFlowControlEntry_FlowControlData_of(_x) (((_x) >> 0) & 0xffffffffffffffff)
/*----------------------------------------------------------------
  Register: TMBLQRead_BLQRead[4] - Reads a pair of entries from Buffer List Queue
    Bits: 60:32 MuAddrHi(ro) - Pointer to the memory buffer. Refer to NbiBDSramEntry register description.
    Bits: 28:0  MuAddrLo(ro) - Pointer to the memory buffer. Refer to NbiBDSramEntry register description.
*/
#define NFP_NBI_TM_TMBLQRead_BLQRead(_x)                     (0x00080000 + (0x80 * ((_x) & 0x3)))
#define   NFP_NBI_TM_TMBLQRead_BLQRead_MuAddrHi(_x)          (((_x) & 0x1fffffff) << 32)
#define   NFP_NBI_TM_TMBLQRead_BLQRead_MuAddrHi_of(_x)       (((_x) >> 32) & 0x1fffffff)
#define   NFP_NBI_TM_TMBLQRead_BLQRead_MuAddrLo(_x)          (((_x) & 0x1fffffff) << 0)
#define   NFP_NBI_TM_TMBLQRead_BLQRead_MuAddrLo_of(_x)       (((_x) >> 0) & 0x1fffffff)
/*----------------------------------------------------------------
  Register: TrafficManagerConfig - Traffic manager configuration register. Contains global read/write configuration information.
    Bits: 16    OOBFCEnable(rw) - MAC Out of Band Flow Control Enable. Enable the Out of Band Flow Control from the MAC external interface to shut off the 1024 level 2 scheduler ports.
    Bits: 15:13 L0ChannelMap(rw) - MAC channel number to Level 0 Scheduler port number mapping register. This mapping register is valid only when L1InputSelect = 1 and ChannelLevelSelect = 0. In this configuration there are only 16 L0 Scheduler ports used. One MAC channel may map to multiple L0 Scheduler ports, and an offset of 64 can be used to support multiple Hydras.
    Bits: 10    Sequencer0Enable(rw) - Sequencer 0 Enable bit. Set this bit to enable reordering for sequencer 0, bit is set by default. Reordering for sequencer zero will be disabled when this bit is cleared.
    Bits:  9    ErrPoisonEna(rw) - It enables the poisoning of data and/or enables the other error porpagation mechanisms in case of errors.
    Bits:  8    SchedulerEnable(rw) - Scheduler Enable bit. Set this bit to enable the scheduling of traffic, clear to disable.
    Bits:  7    ShaperEnable(rw) - Shaper Enable bit. Enables the Rate Shaper to start rate limiting traffic based on the configured values. The default is off so that the shaper is disabled prior to configuration.
    Bits:  6    MiniPacketFCEnable(rw) - Minipacket Credit based Flow Control enable bit. Clear this bit to disable minipacket flow control. This bit is set to enable flow control by default.
    Bits:  5    ChannelLevelSelect(rw) - Channel level select bit. Select the definition of the MAC channel number and the MAC XOFF Flow control to be either the 128 level 0 scheduler ports, or 128 ports on the first 16 level 1 schedulers (or level 2 schedulers depending on the value of L1InputSelect). Note: When L1InputSelect=0 and ChannelLevelSelect=1, only 128 queues are allowed to be active in this configuration.
    Bits:  3:1  NumSequencers(rw) - Number of active sequencers. Specifies the number of active sequencers in the traffic manager.
    Bits:  0    L1InputSelect(rw) - Select the operation of the Level 1 schedulers. This bit disables or enables the Level 1 schedulers, and allows for two or three levels of scheduling in the Traffic Manager. When disabled, the Level 2 scheduler outputs are connected directly to the Level 0 scheduler inputs. When enabled, Level 2 outputs connect to Level 1 inputs, and Level 1 outputs connects to Level 0 inputs
*/
#define NFP_NBI_TMX_CSR_TrafficManagerConfig                 0x00000000
#define   NFP_NBI_TMX_CSR_TrafficManagerConfig_OOBFCEnable   BIT(16)
#define     NFP_NBI_TMX_CSR_TrafficManagerConfig_OOBFCEnable_Disable (0 << 16)
#define     NFP_NBI_TMX_CSR_TrafficManagerConfig_OOBFCEnable_Enable BIT(16)
#define   NFP_NBI_TMX_CSR_TrafficManagerConfig_L0ChannelMap(_x) (((_x) & 0x7) << 13)
#define   NFP_NBI_TMX_CSR_TrafficManagerConfig_L0ChannelMap_of(_x) (((_x) >> 13) & 0x7)
#define     NFP_NBI_TMX_CSR_TrafficManagerConfig_L0ChannelMap_0 (0)
#define     NFP_NBI_TMX_CSR_TrafficManagerConfig_L0ChannelMap_1 (1)
#define     NFP_NBI_TMX_CSR_TrafficManagerConfig_L0ChannelMap_2 (2)
#define     NFP_NBI_TMX_CSR_TrafficManagerConfig_L0ChannelMap_3 (3)
#define     NFP_NBI_TMX_CSR_TrafficManagerConfig_L0ChannelMap_4 (4)
#define     NFP_NBI_TMX_CSR_TrafficManagerConfig_L0ChannelMap_5 (5)
#define     NFP_NBI_TMX_CSR_TrafficManagerConfig_L0ChannelMap_6 (6)
#define     NFP_NBI_TMX_CSR_TrafficManagerConfig_L0ChannelMap_7 (7)
#define   NFP_NBI_TMX_CSR_TrafficManagerConfig_Sequencer0Enable BIT(10)
#define     NFP_NBI_TMX_CSR_TrafficManagerConfig_Sequencer0Enable_Disable (0 << 10)
#define     NFP_NBI_TMX_CSR_TrafficManagerConfig_Sequencer0Enable_Enable BIT(10)
#define   NFP_NBI_TMX_CSR_TrafficManagerConfig_ErrPoisonEna  BIT(9)
#define     NFP_NBI_TMX_CSR_TrafficManagerConfig_ErrPoisonEna_Disable (0 << 9)
#define     NFP_NBI_TMX_CSR_TrafficManagerConfig_ErrPoisonEna_Enable BIT(9)
#define   NFP_NBI_TMX_CSR_TrafficManagerConfig_SchedulerEnable BIT(8)
#define     NFP_NBI_TMX_CSR_TrafficManagerConfig_SchedulerEnable_Disable (0 << 8)
#define     NFP_NBI_TMX_CSR_TrafficManagerConfig_SchedulerEnable_Enable BIT(8)
#define   NFP_NBI_TMX_CSR_TrafficManagerConfig_ShaperEnable  BIT(7)
#define     NFP_NBI_TMX_CSR_TrafficManagerConfig_ShaperEnable_Disable (0 << 7)
#define     NFP_NBI_TMX_CSR_TrafficManagerConfig_ShaperEnable_Enable BIT(7)
#define   NFP_NBI_TMX_CSR_TrafficManagerConfig_MiniPacketFCEnable BIT(6)
#define     NFP_NBI_TMX_CSR_TrafficManagerConfig_MiniPacketFCEnable_Disable (0 << 6)
#define     NFP_NBI_TMX_CSR_TrafficManagerConfig_MiniPacketFCEnable_Enable BIT(6)
#define   NFP_NBI_TMX_CSR_TrafficManagerConfig_ChannelLevelSelect BIT(5)
#define     NFP_NBI_TMX_CSR_TrafficManagerConfig_ChannelLevelSelect_Level_0 (0 << 5)
#define     NFP_NBI_TMX_CSR_TrafficManagerConfig_ChannelLevelSelect_Level_1 BIT(5)
#define   NFP_NBI_TMX_CSR_TrafficManagerConfig_NumSequencers(_x) (((_x) & 0x7) << 1)
#define   NFP_NBI_TMX_CSR_TrafficManagerConfig_NumSequencers_of(_x) (((_x) >> 1) & 0x7)
#define     NFP_NBI_TMX_CSR_TrafficManagerConfig_NumSequencers_0 (0)
#define     NFP_NBI_TMX_CSR_TrafficManagerConfig_NumSequencers_1 (1)
#define     NFP_NBI_TMX_CSR_TrafficManagerConfig_NumSequencers_2 (2)
#define     NFP_NBI_TMX_CSR_TrafficManagerConfig_NumSequencers_4 (3)
#define     NFP_NBI_TMX_CSR_TrafficManagerConfig_NumSequencers_8 (4)
#define     NFP_NBI_TMX_CSR_TrafficManagerConfig_NumSequencers_16 (5)
#define     NFP_NBI_TMX_CSR_TrafficManagerConfig_NumSequencers_32 (6)
#define     NFP_NBI_TMX_CSR_TrafficManagerConfig_NumSequencers_NA (7)
#define   NFP_NBI_TMX_CSR_TrafficManagerConfig_L1InputSelect BIT(0)
#define     NFP_NBI_TMX_CSR_TrafficManagerConfig_L1InputSelect_L1_Disable (0 << 0)
#define     NFP_NBI_TMX_CSR_TrafficManagerConfig_L1InputSelect_L1_Enable BIT(0)
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
#define NFP_NBI_TMX_CSR_DropRate                             0x00000004
#define   NFP_NBI_TMX_CSR_DropRate_DropRateRangeB3(_x)       (((_x) & 0x7) << 21)
#define   NFP_NBI_TMX_CSR_DropRate_DropRateRangeB3_of(_x)    (((_x) >> 21) & 0x7)
#define     NFP_NBI_TMX_CSR_DropRate_DropRateRangeB3_0       (0)
#define     NFP_NBI_TMX_CSR_DropRate_DropRateRangeB3_12_5    (1)
#define     NFP_NBI_TMX_CSR_DropRate_DropRateRangeB3_25      (2)
#define     NFP_NBI_TMX_CSR_DropRate_DropRateRangeB3_37_5    (3)
#define     NFP_NBI_TMX_CSR_DropRate_DropRateRangeB3_50      (4)
#define     NFP_NBI_TMX_CSR_DropRate_DropRateRangeB3_62_5    (5)
#define     NFP_NBI_TMX_CSR_DropRate_DropRateRangeB3_75      (6)
#define     NFP_NBI_TMX_CSR_DropRate_DropRateRangeB3_87_5    (7)
#define   NFP_NBI_TMX_CSR_DropRate_DropRateRangeB2(_x)       (((_x) & 0x7) << 18)
#define   NFP_NBI_TMX_CSR_DropRate_DropRateRangeB2_of(_x)    (((_x) >> 18) & 0x7)
#define     NFP_NBI_TMX_CSR_DropRate_DropRateRangeB2_0       (0)
#define     NFP_NBI_TMX_CSR_DropRate_DropRateRangeB2_12_5    (1)
#define     NFP_NBI_TMX_CSR_DropRate_DropRateRangeB2_25      (2)
#define     NFP_NBI_TMX_CSR_DropRate_DropRateRangeB2_37_5    (3)
#define     NFP_NBI_TMX_CSR_DropRate_DropRateRangeB2_50      (4)
#define     NFP_NBI_TMX_CSR_DropRate_DropRateRangeB2_62_5    (5)
#define     NFP_NBI_TMX_CSR_DropRate_DropRateRangeB2_75      (6)
#define     NFP_NBI_TMX_CSR_DropRate_DropRateRangeB2_87_5    (7)
#define   NFP_NBI_TMX_CSR_DropRate_DropRateRangeB1(_x)       (((_x) & 0x7) << 15)
#define   NFP_NBI_TMX_CSR_DropRate_DropRateRangeB1_of(_x)    (((_x) >> 15) & 0x7)
#define     NFP_NBI_TMX_CSR_DropRate_DropRateRangeB1_0       (0)
#define     NFP_NBI_TMX_CSR_DropRate_DropRateRangeB1_12_5    (1)
#define     NFP_NBI_TMX_CSR_DropRate_DropRateRangeB1_25      (2)
#define     NFP_NBI_TMX_CSR_DropRate_DropRateRangeB1_37_5    (3)
#define     NFP_NBI_TMX_CSR_DropRate_DropRateRangeB1_50      (4)
#define     NFP_NBI_TMX_CSR_DropRate_DropRateRangeB1_62_5    (5)
#define     NFP_NBI_TMX_CSR_DropRate_DropRateRangeB1_75      (6)
#define     NFP_NBI_TMX_CSR_DropRate_DropRateRangeB1_87_5    (7)
#define   NFP_NBI_TMX_CSR_DropRate_DropRateRangeB0(_x)       (((_x) & 0x7) << 12)
#define   NFP_NBI_TMX_CSR_DropRate_DropRateRangeB0_of(_x)    (((_x) >> 12) & 0x7)
#define     NFP_NBI_TMX_CSR_DropRate_DropRateRangeB0_0       (0)
#define     NFP_NBI_TMX_CSR_DropRate_DropRateRangeB0_12_5    (1)
#define     NFP_NBI_TMX_CSR_DropRate_DropRateRangeB0_25      (2)
#define     NFP_NBI_TMX_CSR_DropRate_DropRateRangeB0_37_5    (3)
#define     NFP_NBI_TMX_CSR_DropRate_DropRateRangeB0_50      (4)
#define     NFP_NBI_TMX_CSR_DropRate_DropRateRangeB0_62_5    (5)
#define     NFP_NBI_TMX_CSR_DropRate_DropRateRangeB0_75      (6)
#define     NFP_NBI_TMX_CSR_DropRate_DropRateRangeB0_87_5    (7)
#define   NFP_NBI_TMX_CSR_DropRate_DropRateRangeA3(_x)       (((_x) & 0x7) << 9)
#define   NFP_NBI_TMX_CSR_DropRate_DropRateRangeA3_of(_x)    (((_x) >> 9) & 0x7)
#define     NFP_NBI_TMX_CSR_DropRate_DropRateRangeA3_0       (0)
#define     NFP_NBI_TMX_CSR_DropRate_DropRateRangeA3_12_5    (1)
#define     NFP_NBI_TMX_CSR_DropRate_DropRateRangeA3_25      (2)
#define     NFP_NBI_TMX_CSR_DropRate_DropRateRangeA3_37_5    (3)
#define     NFP_NBI_TMX_CSR_DropRate_DropRateRangeA3_50      (4)
#define     NFP_NBI_TMX_CSR_DropRate_DropRateRangeA3_62_5    (5)
#define     NFP_NBI_TMX_CSR_DropRate_DropRateRangeA3_75      (6)
#define     NFP_NBI_TMX_CSR_DropRate_DropRateRangeA3_87_5    (7)
#define   NFP_NBI_TMX_CSR_DropRate_DropRateRangeA2(_x)       (((_x) & 0x7) << 6)
#define   NFP_NBI_TMX_CSR_DropRate_DropRateRangeA2_of(_x)    (((_x) >> 6) & 0x7)
#define     NFP_NBI_TMX_CSR_DropRate_DropRateRangeA2_0       (0)
#define     NFP_NBI_TMX_CSR_DropRate_DropRateRangeA2_12_5    (1)
#define     NFP_NBI_TMX_CSR_DropRate_DropRateRangeA2_25      (2)
#define     NFP_NBI_TMX_CSR_DropRate_DropRateRangeA2_37_5    (3)
#define     NFP_NBI_TMX_CSR_DropRate_DropRateRangeA2_50      (4)
#define     NFP_NBI_TMX_CSR_DropRate_DropRateRangeA2_62_5    (5)
#define     NFP_NBI_TMX_CSR_DropRate_DropRateRangeA2_75      (6)
#define     NFP_NBI_TMX_CSR_DropRate_DropRateRangeA2_87_5    (7)
#define   NFP_NBI_TMX_CSR_DropRate_DropRateRangeA1(_x)       (((_x) & 0x7) << 3)
#define   NFP_NBI_TMX_CSR_DropRate_DropRateRangeA1_of(_x)    (((_x) >> 3) & 0x7)
#define     NFP_NBI_TMX_CSR_DropRate_DropRateRangeA1_0       (0)
#define     NFP_NBI_TMX_CSR_DropRate_DropRateRangeA1_12_5    (1)
#define     NFP_NBI_TMX_CSR_DropRate_DropRateRangeA1_25      (2)
#define     NFP_NBI_TMX_CSR_DropRate_DropRateRangeA1_37_5    (3)
#define     NFP_NBI_TMX_CSR_DropRate_DropRateRangeA1_50      (4)
#define     NFP_NBI_TMX_CSR_DropRate_DropRateRangeA1_62_5    (5)
#define     NFP_NBI_TMX_CSR_DropRate_DropRateRangeA1_75      (6)
#define     NFP_NBI_TMX_CSR_DropRate_DropRateRangeA1_87_5    (7)
#define   NFP_NBI_TMX_CSR_DropRate_DropRateRangeA0(_x)       (((_x) & 0x7) << 0)
#define   NFP_NBI_TMX_CSR_DropRate_DropRateRangeA0_of(_x)    (((_x) >> 0) & 0x7)
#define     NFP_NBI_TMX_CSR_DropRate_DropRateRangeA0_0       (0)
#define     NFP_NBI_TMX_CSR_DropRate_DropRateRangeA0_12_5    (1)
#define     NFP_NBI_TMX_CSR_DropRate_DropRateRangeA0_25      (2)
#define     NFP_NBI_TMX_CSR_DropRate_DropRateRangeA0_37_5    (3)
#define     NFP_NBI_TMX_CSR_DropRate_DropRateRangeA0_50      (4)
#define     NFP_NBI_TMX_CSR_DropRate_DropRateRangeA0_62_5    (5)
#define     NFP_NBI_TMX_CSR_DropRate_DropRateRangeA0_75      (6)
#define     NFP_NBI_TMX_CSR_DropRate_DropRateRangeA0_87_5    (7)
/*----------------------------------------------------------------
  Register: BlqEvent - Configures thresholds, event type and enables/disables for the buffer list queues
    Bits: 20    DisBLQEventFifoFullAssert(rw) - Set this bit to disable the BLQ Event FIFO overflow assertion
    Bits: 19:18 BLQEventThreshold3(rw) - An event is generated every N buffer descriptors written to buffer list queue3, where N is configurable from 16 to 128.
    Bits: 17:16 BLQEventThreshold2(rw) - An event is generated every N buffer descriptors written to buffer list queue2, where N is configurable from 16 to 128.
    Bits: 15:14 BLQEventThreshold1(rw) - An event is generated every N buffer descriptors written to buffer list queue1, where N is configurable from 16 to 128.
    Bits: 13:12 BLQEventThreshold0(rw) - An event is generated every N buffer descriptors written to buffer list queue0, where N is configurable from 16 to 128.
    Bits:  7:4  BLQEventNotEmptyEnable(rw) - One bit per buffer list queue. Enables the generation of BLQ Events when the queue transitions from empty to not empty. Disabled when clear. The event type value used is 0x0 (event_type_fifo_not_empty)
    Bits:  3:0  BLQEventStatusEnable(rw) - One bit per buffer list queue. Enables BLQ Status Events when set. Disabled when clear. A Status Event is sent when the number of entries written to the queue reaches the programmed value in the BLQEventThreshold register. The event type value used is 0x5 (event_type_value_update)
*/
#define NFP_NBI_TMX_CSR_BlqEvent                             0x00000008
#define   NFP_NBI_TMX_CSR_BlqEvent_DisBLQEventFifoFullAssert BIT(20)
#define   NFP_NBI_TMX_CSR_BlqEvent_BLQEventThreshold3(_x)    (((_x) & 0x3) << 18)
#define   NFP_NBI_TMX_CSR_BlqEvent_BLQEventThreshold3_of(_x) (((_x) >> 18) & 0x3)
#define     NFP_NBI_TMX_CSR_BlqEvent_BLQEventThreshold3_16   (0)
#define     NFP_NBI_TMX_CSR_BlqEvent_BLQEventThreshold3_32   (1)
#define     NFP_NBI_TMX_CSR_BlqEvent_BLQEventThreshold3_64   (2)
#define     NFP_NBI_TMX_CSR_BlqEvent_BLQEventThreshold3_128  (3)
#define   NFP_NBI_TMX_CSR_BlqEvent_BLQEventThreshold2(_x)    (((_x) & 0x3) << 16)
#define   NFP_NBI_TMX_CSR_BlqEvent_BLQEventThreshold2_of(_x) (((_x) >> 16) & 0x3)
#define     NFP_NBI_TMX_CSR_BlqEvent_BLQEventThreshold2_16   (0)
#define     NFP_NBI_TMX_CSR_BlqEvent_BLQEventThreshold2_32   (1)
#define     NFP_NBI_TMX_CSR_BlqEvent_BLQEventThreshold2_64   (2)
#define     NFP_NBI_TMX_CSR_BlqEvent_BLQEventThreshold2_128  (3)
#define   NFP_NBI_TMX_CSR_BlqEvent_BLQEventThreshold1(_x)    (((_x) & 0x3) << 14)
#define   NFP_NBI_TMX_CSR_BlqEvent_BLQEventThreshold1_of(_x) (((_x) >> 14) & 0x3)
#define     NFP_NBI_TMX_CSR_BlqEvent_BLQEventThreshold1_16   (0)
#define     NFP_NBI_TMX_CSR_BlqEvent_BLQEventThreshold1_32   (1)
#define     NFP_NBI_TMX_CSR_BlqEvent_BLQEventThreshold1_64   (2)
#define     NFP_NBI_TMX_CSR_BlqEvent_BLQEventThreshold1_128  (3)
#define   NFP_NBI_TMX_CSR_BlqEvent_BLQEventThreshold0(_x)    (((_x) & 0x3) << 12)
#define   NFP_NBI_TMX_CSR_BlqEvent_BLQEventThreshold0_of(_x) (((_x) >> 12) & 0x3)
#define     NFP_NBI_TMX_CSR_BlqEvent_BLQEventThreshold0_16   (0)
#define     NFP_NBI_TMX_CSR_BlqEvent_BLQEventThreshold0_32   (1)
#define     NFP_NBI_TMX_CSR_BlqEvent_BLQEventThreshold0_64   (2)
#define     NFP_NBI_TMX_CSR_BlqEvent_BLQEventThreshold0_128  (3)
#define   NFP_NBI_TMX_CSR_BlqEvent_BLQEventNotEmptyEnable(_x) (((_x) & 0xf) << 4)
#define   NFP_NBI_TMX_CSR_BlqEvent_BLQEventNotEmptyEnable_of(_x) (((_x) >> 4) & 0xf)
#define     NFP_NBI_TMX_CSR_BlqEvent_BLQEventNotEmptyEnable_Disable (0)
#define     NFP_NBI_TMX_CSR_BlqEvent_BLQEventNotEmptyEnable_Enable (1)
#define   NFP_NBI_TMX_CSR_BlqEvent_BLQEventStatusEnable(_x)  (((_x) & 0xf) << 0)
#define   NFP_NBI_TMX_CSR_BlqEvent_BLQEventStatusEnable_of(_x) (((_x) >> 0) & 0xf)
#define     NFP_NBI_TMX_CSR_BlqEvent_BLQEventStatusEnable_Disable (0)
#define     NFP_NBI_TMX_CSR_BlqEvent_BLQEventStatusEnable_Enable (1)
/*----------------------------------------------------------------
  Register: EgressRateLimit - Rate limiting CSR for the Egress NBI Command/PullId ports.
    Bits:  2    EgressRateLimitOff(rw) - Turns the rate limiting off for both Ports 6 and 7 (P6 and P7).
    Bits:  1:0  EgressRate(rw) - Rate limiting control for the Command/PullId Ports 6 and 7 (P6 and P7).
*/
#define NFP_NBI_TMX_CSR_EgressRateLimit                      0x00000074
#define   NFP_NBI_TMX_CSR_EgressRateLimit_EgressRateLimitOff BIT(2)
#define     NFP_NBI_TMX_CSR_EgressRateLimit_EgressRateLimitOff_on (0 << 2)
#define     NFP_NBI_TMX_CSR_EgressRateLimit_EgressRateLimitOff_off BIT(2)
#define   NFP_NBI_TMX_CSR_EgressRateLimit_EgressRate(_x)     (((_x) & 0x3) << 0)
#define   NFP_NBI_TMX_CSR_EgressRateLimit_EgressRate_of(_x)  (((_x) >> 0) & 0x3)
#define     NFP_NBI_TMX_CSR_EgressRateLimit_EgressRate_64G   (0)
#define     NFP_NBI_TMX_CSR_EgressRateLimit_EgressRate_32G   (1)
#define     NFP_NBI_TMX_CSR_EgressRateLimit_EgressRate_16G   (2)
#define     NFP_NBI_TMX_CSR_EgressRateLimit_EgressRate_8G    (3)
/*----------------------------------------------------------------
  Register: EgressPullIdPortEnable - Enables ports for transmitting Pull IDs.
    Bits:  1:0  EgressPullIdPortEnable(rw) - Egress Port enable for transmitting PullIDs.
*/
#define NFP_NBI_TMX_CSR_EgressPullIdPortEnable               0x00000078
#define   NFP_NBI_TMX_CSR_EgressPullIdPortEnable_EgressPullIdPortEnable(_x) (((_x) & 0x3) << 0)
#define   NFP_NBI_TMX_CSR_EgressPullIdPortEnable_EgressPullIdPortEnable_of(_x) (((_x) >> 0) & 0x3)
#define     NFP_NBI_TMX_CSR_EgressPullIdPortEnable_EgressPullIdPortEnable_P6_and_P7 (0)
#define     NFP_NBI_TMX_CSR_EgressPullIdPortEnable_EgressPullIdPortEnable_P6 (1)
#define     NFP_NBI_TMX_CSR_EgressPullIdPortEnable_EgressPullIdPortEnable_P7 (2)
#define     NFP_NBI_TMX_CSR_EgressPullIdPortEnable_EgressPullIdPortEnable_unused (3)
/*----------------------------------------------------------------
  Register: OutOfOrderCount[32] - Out of order counter for each sequencer. There are a total of 32 out of order counters in the traffic manager, one for each active sequencer.
    Bits: 15:0  OutOfOrderCount(ro) - Out-of-order counter to keep track of the number of packets sent out of order for this sequencer. The counter will saturate when the maximum number is reached.
*/
#define NFP_NBI_TMX_CSR_OutOfOrderCount(_x)                  (0x00000100 + (0x4 * ((_x) & 0x1f)))
#define   NFP_NBI_TMX_CSR_OutOfOrderCount_OutOfOrderCount(_x) (((_x) & 0xffff) << 0)
#define   NFP_NBI_TMX_CSR_OutOfOrderCount_OutOfOrderCount_of(_x) (((_x) >> 0) & 0xffff)
/*----------------------------------------------------------------
  Register: OutOfOrderCountClear[32] - Clear on read out of order counter for each sequencer. One for each active sequencer.
    Bits: 15:0  OutOfOrderCountClear(rc) - Clear on read out-of-order counter to keep track of the number of packets sent out of order for this sequencer. The counter will saturate when the maximum number is reached. The read value for this register is identical to the OUTOFORDERCOUNT register value, however when this register is read the out-of-order counter is cleared.
*/
#define NFP_NBI_TMX_CSR_OutOfOrderCountClear(_x)             (0x00000200 + (0x4 * ((_x) & 0x1f)))
#define   NFP_NBI_TMX_CSR_OutOfOrderCountClear_OutOfOrderCountClear(_x) (((_x) & 0xffff) << 0)
#define   NFP_NBI_TMX_CSR_OutOfOrderCountClear_OutOfOrderCountClear_of(_x) (((_x) >> 0) & 0xffff)
/*----------------------------------------------------------------
  Register: MiniPktCreditConfig - MiniPacket Credit Flow Control Configuration Register
    Bits: 24    MiniPacketFCMode(rw) - MiniPacket Credit Flow Control Mode bit. Select the free pool crediting mode to use.
    Bits: 21:12 FPCreditLimit(rw) - MiniPacket Transmit Free Pool Credit Counter Limit. Programmable limit for both of the free pool credit counters. The default value of 512 credits should be used for most applications using one free pool. The free pool has a reserve of 40 credits and should always be programmed to a value greater than 40.
    Bits:  9:0  CreditLimit(rw) - MiniPacket Transmit Channel Credit Counter Limit. Programmable limit for all of the channel credit counters
*/
#define NFP_NBI_TMX_CSR_MiniPktCreditConfig                  0x00000300
#define   NFP_NBI_TMX_CSR_MiniPktCreditConfig_MiniPacketFCMode BIT(24)
#define     NFP_NBI_TMX_CSR_MiniPktCreditConfig_MiniPacketFCMode_Mode_0 (0 << 24)
#define     NFP_NBI_TMX_CSR_MiniPktCreditConfig_MiniPacketFCMode_Mode_1 BIT(24)
#define   NFP_NBI_TMX_CSR_MiniPktCreditConfig_FPCreditLimit(_x) (((_x) & 0x3ff) << 12)
#define   NFP_NBI_TMX_CSR_MiniPktCreditConfig_FPCreditLimit_of(_x) (((_x) >> 12) & 0x3ff)
#define   NFP_NBI_TMX_CSR_MiniPktCreditConfig_CreditLimit(_x) (((_x) & 0x3ff) << 0)
#define   NFP_NBI_TMX_CSR_MiniPktCreditConfig_CreditLimit_of(_x) (((_x) >> 0) & 0x3ff)
/*----------------------------------------------------------------
  Register: MiniPktFreePoolCredit[2] - MiniPacket Transmit Free Pool Credit Counter. Read only free pool credit counters
    Bits:  9:0  FPCreditCounter(ro) - MiniPacket Transmit Free Pool Credit Counter. Read only counter of the free pool credits remaining
*/
#define NFP_NBI_TMX_CSR_MiniPktFreePoolCredit(_x)            (0x00000304 + (0x4 * ((_x) & 0x1)))
#define   NFP_NBI_TMX_CSR_MiniPktFreePoolCredit_FPCreditCounter(_x) (((_x) & 0x3ff) << 0)
#define   NFP_NBI_TMX_CSR_MiniPktFreePoolCredit_FPCreditCounter_of(_x) (((_x) >> 0) & 0x3ff)
/*----------------------------------------------------------------
  Register: MiniPktChannelCredit[128] - MiniPacket Transmit Channel Credit Counter. Read only credit counter, one for each channel
    Bits:  9:0  CreditCounter(ro) - MiniPacket Transmit Channel Credit Counter. Read only counter of the channel credits remaining
*/
#define NFP_NBI_TMX_CSR_MiniPktChannelCredit(_x)             (0x00000400 + (0x4 * ((_x) & 0x7f)))
#define   NFP_NBI_TMX_CSR_MiniPktChannelCredit_CreditCounter(_x) (((_x) & 0x3ff) << 0)
#define   NFP_NBI_TMX_CSR_MiniPktChannelCredit_CreditCounter_of(_x) (((_x) >> 0) & 0x3ff)
/*----------------------------------------------------------------
  Register: ReorderActivity - Activity indication flag for each sequencer in the reorder buffer.
    Bits: 31:0  ReorderActivity(ro) - Clear on read activity flag for each sequencer in the reorder buffer. One bit for each active sequencer in the reorder buffer.
*/
#define NFP_NBI_TMX_CSR_ReorderActivity                      0x00000600
#define   NFP_NBI_TMX_CSR_ReorderActivity_ReorderActivity(_x) (((_x) & 0xffffffff) << 0)
#define   NFP_NBI_TMX_CSR_ReorderActivity_ReorderActivity_of(_x) (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: QueueStatus[1024] - Status information stored for each queue. There are a total of 1024 queue status registers in the traffic manager.
    Bits: 13    QueueFull(ro) - Queue Full register bit indicating the Descriptor Queue is Full or Almost Full
    Bits: 12:0  QueueLevel(ro) - Queue level register indicating the fill level for this particular queue. One 13-bit register for each descriptor queue.
*/
#define NFP_NBI_TM_Q_QueueStatus(_x)                         (0x00000000 + (0x4 * ((_x) & 0x3ff)))
#define   NFP_NBI_TM_Q_QueueStatus_QueueFull                 BIT(13)
#define   NFP_NBI_TM_Q_QueueStatus_QueueLevel(_x)            (((_x) & 0x1fff) << 0)
#define   NFP_NBI_TM_Q_QueueStatus_QueueLevel_of(_x)         (((_x) >> 0) & 0x1fff)
/*----------------------------------------------------------------
  Register: QueueConfig[1024] - Configuration settings for each queue. There are a total of 1024 queue configuration registers in the traffic manager.
    Bits: 10    DropRateRangeSelect(rw) - Select the Drop Rate Range set to use for this queue. There are two range sets A and B that may be used and are selectable per queue.
    Bits:  9:6  QueueSize(rw) - Configures the maximum size of this particular queue
    Bits:  5:3  DMAPacketThreshold(rw) - Queue level percentage threshold to determine when the packet data should be moved from CTM memory to main memory. One register each descriptor queue. The percentage threshold maintains an accuracy of plus or minus one descriptor
    Bits:  2    REDEnable(rw) - Enables random early detection for packet dropping for this particular queue. Packets will be discarded for this queue based on the globally configured drop rates for each queue range. The queue is divided into four ranges, the drop percentage is configurable for each range.
    Bits:  1    DropEnable(rw) - Enables packet dropping for this particular queue. Packets will be discarded for this queue when it is full or almost full. If REDEnable is set to 1, DropEnable should also be set to 1
    Bits:  0    QueueEnable(rw) - Enable bit for this particular queue. Set this bit to enable the queue, clear to disable.
*/
#define NFP_NBI_TM_Q_QueueConfig(_x)                         (0x00001000 + (0x4 * ((_x) & 0x3ff)))
#define   NFP_NBI_TM_Q_QueueConfig_DropRateRangeSelect       BIT(10)
#define     NFP_NBI_TM_Q_QueueConfig_DropRateRangeSelect_A   (0 << 10)
#define     NFP_NBI_TM_Q_QueueConfig_DropRateRangeSelect_B   BIT(10)
#define   NFP_NBI_TM_Q_QueueConfig_QueueSize(_x)             (((_x) & 0xf) << 6)
#define   NFP_NBI_TM_Q_QueueConfig_QueueSize_of(_x)          (((_x) >> 6) & 0xf)
#define     NFP_NBI_TM_Q_QueueConfig_QueueSize_8             (3)
#define     NFP_NBI_TM_Q_QueueConfig_QueueSize_16            (4)
#define     NFP_NBI_TM_Q_QueueConfig_QueueSize_32            (5)
#define     NFP_NBI_TM_Q_QueueConfig_QueueSize_64            (6)
#define     NFP_NBI_TM_Q_QueueConfig_QueueSize_128           (7)
#define     NFP_NBI_TM_Q_QueueConfig_QueueSize_256           (8)
#define     NFP_NBI_TM_Q_QueueConfig_QueueSize_512           (9)
#define     NFP_NBI_TM_Q_QueueConfig_QueueSize_1024          (10)
#define     NFP_NBI_TM_Q_QueueConfig_QueueSize_2048          (11)
#define     NFP_NBI_TM_Q_QueueConfig_QueueSize_4096          (12)
#define     NFP_NBI_TM_Q_QueueConfig_QueueSize_8192          (13)
#define   NFP_NBI_TM_Q_QueueConfig_DMAPacketThreshold(_x)    (((_x) & 0x7) << 3)
#define   NFP_NBI_TM_Q_QueueConfig_DMAPacketThreshold_of(_x) (((_x) >> 3) & 0x7)
#define     NFP_NBI_TM_Q_QueueConfig_DMAPacketThreshold_default (0)
#define     NFP_NBI_TM_Q_QueueConfig_DMAPacketThreshold_12_5 (1)
#define     NFP_NBI_TM_Q_QueueConfig_DMAPacketThreshold_25   (2)
#define     NFP_NBI_TM_Q_QueueConfig_DMAPacketThreshold_37_5 (3)
#define     NFP_NBI_TM_Q_QueueConfig_DMAPacketThreshold_50   (4)
#define     NFP_NBI_TM_Q_QueueConfig_DMAPacketThreshold_62_5 (5)
#define     NFP_NBI_TM_Q_QueueConfig_DMAPacketThreshold_75   (6)
#define     NFP_NBI_TM_Q_QueueConfig_DMAPacketThreshold_87_5 (7)
#define   NFP_NBI_TM_Q_QueueConfig_REDEnable                 BIT(2)
#define     NFP_NBI_TM_Q_QueueConfig_REDEnable_Disabled      (0 << 2)
#define     NFP_NBI_TM_Q_QueueConfig_REDEnable_Enabled       BIT(2)
#define   NFP_NBI_TM_Q_QueueConfig_DropEnable                BIT(1)
#define     NFP_NBI_TM_Q_QueueConfig_DropEnable_Disabled     (0 << 1)
#define     NFP_NBI_TM_Q_QueueConfig_DropEnable_Enabled      BIT(1)
#define   NFP_NBI_TM_Q_QueueConfig_QueueEnable               BIT(0)
#define     NFP_NBI_TM_Q_QueueConfig_QueueEnable_Disable     (0 << 0)
#define     NFP_NBI_TM_Q_QueueConfig_QueueEnable_Enable      BIT(0)
/*----------------------------------------------------------------
  Register: QueueDropCount[1024] - Drop counter for each queue. There are a total of 1024 drop counters in the traffic manager.
    Bits: 31:0  DropCount(ro) - 32-bit drop counter to keep track of the number of packets dropped for this queue. The counter will saturate when the maximum number is reached.
*/
#define NFP_NBI_TM_Q_QueueDropCount(_x)                      (0x00002000 + (0x4 * ((_x) & 0x3ff)))
#define   NFP_NBI_TM_Q_QueueDropCount_DropCount(_x)          (((_x) & 0xffffffff) << 0)
#define   NFP_NBI_TM_Q_QueueDropCount_DropCount_of(_x)       (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: QueueDropCountClear[1024] - Clear on read drop counter for each queue. There are a total of 1024 clear on read drop counters in the traffic manager.
    Bits: 31:0  DropCountClear(rc) - Clear on read 32-bit drop counter to keep track of the number of packets dropped for this queue. The counter will saturate when the maximum number is reached. The read value for this register is identical to the DROPCOUNT register value, however when DROPCOUNTCLEAR is read the drop counter is cleared. The drop count memory is undefined at start-up. This register should be read for all active queues by software prior to device operation.
*/
#define NFP_NBI_TM_Q_QueueDropCountClear(_x)                 (0x00003000 + (0x4 * ((_x) & 0x3ff)))
#define   NFP_NBI_TM_Q_QueueDropCountClear_DropCountClear(_x) (((_x) & 0xffffffff) << 0)
#define   NFP_NBI_TM_Q_QueueDropCountClear_DropCountClear_of(_x) (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: SchedulerConfig[145] - Configuration register for setup of each scheduler. There are a total of 145 scheduler configuration registers in the traffic manager. The level 2 schedulers are at offsets 0-127, the level 1 schedulers are at offsets 128-143, and the level 0 scheduler is at offset 144.
    Bits:  2    SP1Enable(rw) - Strict Priority One Enable bit. Set this bit to enable scheduler port 1 as the strict priority 1 port. SP1 will have priority over all other ports except for SP0.
    Bits:  1    SP0Enable(rw) - Strict Priority Zero Enable bit. Set this bit to enable scheduler port 0 as the strict priority 0 port. SP0 will have priority over all other ports, including SP1.
    Bits:  0    DWRREnable(rw) - Deficit Weighted Round Robin Enable bit. Set this bit to enable deficit weighted round operation for the particular scheduler.
*/
#define NFP_NBI_TMX_SCH_CSR_SchedulerConfig(_x)              (0x00000000 + (0x4 * ((_x) & 0xff)))
#define   NFP_NBI_TMX_SCH_CSR_SchedulerConfig_SP1Enable      BIT(2)
#define     NFP_NBI_TMX_SCH_CSR_SchedulerConfig_SP1Enable_Disable (0 << 2)
#define     NFP_NBI_TMX_SCH_CSR_SchedulerConfig_SP1Enable_Enable BIT(2)
#define   NFP_NBI_TMX_SCH_CSR_SchedulerConfig_SP0Enable      BIT(1)
#define     NFP_NBI_TMX_SCH_CSR_SchedulerConfig_SP0Enable_Disable (0 << 1)
#define     NFP_NBI_TMX_SCH_CSR_SchedulerConfig_SP0Enable_Enable BIT(1)
#define   NFP_NBI_TMX_SCH_CSR_SchedulerConfig_DWRREnable     BIT(0)
#define     NFP_NBI_TMX_SCH_CSR_SchedulerConfig_DWRREnable_Disable (0 << 0)
#define     NFP_NBI_TMX_SCH_CSR_SchedulerConfig_DWRREnable_Enable BIT(0)
/*----------------------------------------------------------------
  Register: SchedulerWeight[128] - L0 Scheduler weight value to configure the relative bandwidth for each scheduler port. There are a total of 128 weight values to program in the level 0 scheduler.
    Bits: 23:0  Weight(rw) - Scheduler weight value. Program this memory with the desired weight value for the particular scheduler port. This memory is undefined at start-up and should be programmed or set to zero by software. If all weights for all ports of a particular port are set to zero, the scheduler will operate in round robin fashion. The weight value will configure the relative bandwidth for the port and is used to update the deficit value. The weight should be programmed to the desired relative bandwidth for this port as compared to the other ports on this scheduler.
*/
#define NFP_NBI_TMX_SCH_CSR_SchedulerWeight(_x)              (0x00000800 + (0x4 * ((_x) & 0x7f)))
#define   NFP_NBI_TMX_SCH_CSR_SchedulerWeight_Weight(_x)     (((_x) & 0xffffff) << 0)
#define   NFP_NBI_TMX_SCH_CSR_SchedulerWeight_Weight_of(_x)  (((_x) >> 0) & 0xffffff)
/*----------------------------------------------------------------
  Register: SchedulerDeficit[128] - L0 Scheduler deficit value for each scheduler port. There are a total of 128 deficit values in the level 0 scheduler.
    Bits: 24:0  Deficit(rw) - Scheduler deficit value. This memory provides the signed value of the deficit for the particular scheduler port. This memory is undefined at start-up and should be programmed to zero by software. If all of the ports either have a negative deficit or do not have data to send, the deficit values will be updated for all ports. During the update if the port does not have data to send the deficit will be set to the weight value. If the deficit has gone negative, the weight value will be added to the current deficit.
*/
#define NFP_NBI_TMX_SCH_CSR_SchedulerDeficit(_x)             (0x00001000 + (0x4 * ((_x) & 0x7f)))
#define   NFP_NBI_TMX_SCH_CSR_SchedulerDeficit_Deficit(_x)   (((_x) & 0x1ffffff) << 0)
#define   NFP_NBI_TMX_SCH_CSR_SchedulerDeficit_Deficit_of(_x) (((_x) >> 0) & 0x1ffffff)
/*----------------------------------------------------------------
  Register: ShaperRate[145] - Shaper rate register. Configures the maximum allowed rate. There are a total of 145 shaper rate registers in the traffic manager. The level 2 shaper registers are at offsets 0-127, the level 1 shaper registers are at offsets 128-143, and the level 0 shaper register is at offset 144.
    Bits: 13:0  Rate(rw) - The shaper rate register. This register is programmed with the desired maximum rate in 10Megabit per second increments for the associated port or queue. Valid values may range in size from 1 to 12,000 and correspond to data rates of 10Mbs to 120Gbps when using a 1.0GHz PCLK. Values outside that range are not supported (i.e. 0, or above 12,000). If other PCLK frequencies are used, the shaper rate value should be multiplied by (PCLK frequency/1.0GHz) for proper shaper accuracy.
*/
#define NFP_NBI_TMX_SHP_ShaperRate(_x)                       (0x00000000 + (0x4 * ((_x) & 0xff)))
#define   NFP_NBI_TMX_SHP_ShaperRate_Rate(_x)                (((_x) & 0x3fff) << 0)
#define   NFP_NBI_TMX_SHP_ShaperRate_Rate_of(_x)             (((_x) >> 0) & 0x3fff)
/*----------------------------------------------------------------
  Register: ShaperThreshold[145] - Rate limiting shaper threshold register. Configures the desired threshold level of the leaky bucket. There are a total of 145 shaper threshold registers in the traffic manager. The level 2 shaper registers are at offsets 0-127, the level 1 shaper registers are at offsets 128-143, and the level 0 shaper register is at offset 144.
    Bits:  2:0  Threshold(rw) - The rate limiting shaper threshold register. This register configures the threshold in bytes used for comparison to the instantaneous level of the leaky bucket. When the number of bytes sent is below this threshold the shaper will be open, when the bucket level equals or exceeds the threshold the shaper will be closed.
*/
#define NFP_NBI_TMX_SHP_ShaperThreshold(_x)                  (0x00000800 + (0x4 * ((_x) & 0xff)))
#define   NFP_NBI_TMX_SHP_ShaperThreshold_Threshold(_x)      (((_x) & 0x7) << 0)
#define   NFP_NBI_TMX_SHP_ShaperThreshold_Threshold_of(_x)   (((_x) >> 0) & 0x7)
#define     NFP_NBI_TMX_SHP_ShaperThreshold_Threshold_8192   (0)
#define     NFP_NBI_TMX_SHP_ShaperThreshold_Threshold_16384  (1)
#define     NFP_NBI_TMX_SHP_ShaperThreshold_Threshold_24576  (2)
#define     NFP_NBI_TMX_SHP_ShaperThreshold_Threshold_32768  (3)
#define     NFP_NBI_TMX_SHP_ShaperThreshold_Threshold_40960  (4)
#define     NFP_NBI_TMX_SHP_ShaperThreshold_Threshold_49152  (5)
#define     NFP_NBI_TMX_SHP_ShaperThreshold_Threshold_57344  (6)
#define     NFP_NBI_TMX_SHP_ShaperThreshold_Threshold_65535  (7)
/*----------------------------------------------------------------
  Register: ShaperMaxOvershoot[145] - Rate limiting shaper maximum overshoot register. Configures the maximum allowed overshoot for the leaky bucket. There are a total of 145 shaper maximum overshoot registers in the traffic manager. The level 2 shaper registers are at offsets 0-127, the level 1 shaper registers are at offsets 128-143, and the level 0 shaper register is at offset 144
    Bits:  2:0  MaxOvershoot(rw) - The rate limiting shaper maximum overshoot register. This register configures the maximum allowed overshoot in bytes for the leaky bucket. Data that is sent beyond this limit will not be accounted for in the leaky bucket algorithm.
*/
#define NFP_NBI_TMX_SHP_ShaperMaxOvershoot(_x)               (0x00001000 + (0x4 * ((_x) & 0xff)))
#define   NFP_NBI_TMX_SHP_ShaperMaxOvershoot_MaxOvershoot(_x) (((_x) & 0x7) << 0)
#define   NFP_NBI_TMX_SHP_ShaperMaxOvershoot_MaxOvershoot_of(_x) (((_x) >> 0) & 0x7)
#define     NFP_NBI_TMX_SHP_ShaperMaxOvershoot_MaxOvershoot_16384 (0)
#define     NFP_NBI_TMX_SHP_ShaperMaxOvershoot_MaxOvershoot_24576 (1)
#define     NFP_NBI_TMX_SHP_ShaperMaxOvershoot_MaxOvershoot_32768 (2)
#define     NFP_NBI_TMX_SHP_ShaperMaxOvershoot_MaxOvershoot_40960 (3)
#define     NFP_NBI_TMX_SHP_ShaperMaxOvershoot_MaxOvershoot_49152 (4)
#define     NFP_NBI_TMX_SHP_ShaperMaxOvershoot_MaxOvershoot_57344 (5)
#define     NFP_NBI_TMX_SHP_ShaperMaxOvershoot_MaxOvershoot_65536 (6)
#define     NFP_NBI_TMX_SHP_ShaperMaxOvershoot_MaxOvershoot_131071 (7)
/*----------------------------------------------------------------
  Register: ShaperRateAdjust[145] - Shaper rate adjust register. Configures the fixed data rate adjustment for packet overhead. There are a total of 145 shaper rate adjust registers in the traffic manager. The level 2 shaper registers are at offsets 0-127, the level 1 shaper registers are at offsets 128-143, and the level 0 shaper register is at offset 144.
    Bits:  9:0  RateAdjust(rw) - The shaper rate adjust register. This register contains the standard number of bytes to be subtracted for every packet that is transferred. This is used to account for packet overhead and metadata that is not to be included in the shaper rate calculation. In cases where the rate adjust value is larger than the packet size, the difference will be deducted from the shaper bucket.
*/
#define NFP_NBI_TMX_SHP_ShaperRateAdjust(_x)                 (0x00001800 + (0x4 * ((_x) & 0xff)))
#define   NFP_NBI_TMX_SHP_ShaperRateAdjust_RateAdjust(_x)    (((_x) & 0x3ff) << 0)
#define   NFP_NBI_TMX_SHP_ShaperRateAdjust_RateAdjust_of(_x) (((_x) >> 0) & 0x3ff)
/*----------------------------------------------------------------
  Register: ShaperStatus[145] - Shaper status register. Contains read-only status information for the leaky bucket rate liming shaper. There are a total of 145 shaper status registers in the traffic manager. The level 2 shaper registers are at offsets 0-127, the level 1 shaper registers are at offsets 128-143, and the level 0 shaper register is at offset 144.
    Bits: 17    ShaperOpen(ro) - Indicates the state of the leaky bucket
    Bits: 16:0  Level(ro) - The current leaky bucket level of the rate limiting shaper. This register contains the present level of the leaky bucket that is used to determine open or closed state. If this value is below the threshold the shaper will be open, if it is above the threshold the shaper will be closed.
*/
#define NFP_NBI_TMX_SHP_ShaperStatus(_x)                     (0x00002000 + (0x4 * ((_x) & 0xff)))
#define   NFP_NBI_TMX_SHP_ShaperStatus_ShaperOpen            BIT(17)
#define     NFP_NBI_TMX_SHP_ShaperStatus_ShaperOpen_Closed   (0 << 17)
#define     NFP_NBI_TMX_SHP_ShaperStatus_ShaperOpen_Open     BIT(17)
#define   NFP_NBI_TMX_SHP_ShaperStatus_Level(_x)             (((_x) & 0x1ffff) << 0)
#define   NFP_NBI_TMX_SHP_ShaperStatus_Level_of(_x)          (((_x) >> 0) & 0x1ffff)

#endif /* NFP6000_LONGNAMES */


#endif /* NFP6000_NBI_TM_H */
