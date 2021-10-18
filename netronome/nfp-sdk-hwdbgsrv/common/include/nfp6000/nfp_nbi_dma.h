/*
 * Copyright (C) 2013, Netronome Systems, Inc.  All rights reserved.
 *
 * Disclaimer: this file is provided without any warranty
 * as part of an early-access program, and the content is
 * bound to change before the final release.
 */

#ifndef NFP6000_NBI_DMA_H
#define NFP6000_NBI_DMA_H

#include "nfp_nbi.h"

/*----------------------------------------------------------------*/
/* CPP Address Map (CPP_BASE = IMB translation: island + indexes) */

/* CPP_BASE + NFP_NBI_DMA + NFP_NBI_DMA_* */
#define NFP_NBI_DMA                                          (0x000000)

/*----------------------------------------------------------------*/
/* XPB Address Map (XPB_BASE = NFP_XPB_ISLAND(island))          */

#define NFP_NBI_DMAX                                         (NBIX_BASE + 0x000000)

/* XPB_BASE + NFP_NBI_DMAX_CSR + NFP_NBI_DMAX_CSR_* */
#define NFP_NBI_DMAX_CSR                                     (NFP_NBI_DMAX + 0x00000)
/* XPB_BASE + NFP_NBI_DMAX_BD_ECC + (nfp_ecc.h) */
#define NFP_NBI_DMAX_BD_ECC                                  (NFP_NBI_DMAX + 0x10000)
/* XPB_BASE + NFP_NBI_DMAX_BC_ECC + (nfp_ecc.h) */
#define NFP_NBI_DMAX_BC_ECC                                  (NFP_NBI_DMAX + 0x20000)
/* XPB_BASE + NFP_NBI_DMAX_DS_ECC + (nfp_ecc.h) */
#define NFP_NBI_DMAX_DS_ECC                                  (NFP_NBI_DMAX + 0x30000)

/*================================================================*/


/* HGID: 60515d2e075a */

#ifndef NFP6000_LONGNAMES

/*----------------------------------------------------------------
  Register: NbiDmaBDSramEntry[4096] - NBI DMA Buffer Descriptor SRAM Entry X
    Bits: 28:0  MuAddr(rw) - Memory Unit Address of the buffer (internal/external MU). Maps to CPP Address[29;11] Refer to the NBI top-level Master Commands section for memory unit address translation.
*/
#define NFP_NBI_DMA_BDSRAM_NBIDMA_BDSRAM_ENTRY(_x)           (0x00000000 + (0x8 * ((_x) & 0xfff)))
#define   NFP_NBI_DMA_BDSRAM_NBIDMA_BDSRAM_ENTRY_MUADDR(_x)  (((_x) & 0x1fffffff) << 0)
#define   NFP_NBI_DMA_BDSRAM_NBIDMA_BDSRAM_ENTRY_MUADDR_of(_x) (((_x) >> 0) & 0x1fffffff)
/*----------------------------------------------------------------
  Register: BLQueCtrl0 - Configures the buffer list circular queue and provides status
    Bits: 55:48 BLQEventFullCnt(rc) - Debug counter that increments when a BLQ Event is dropped due to a full BLQ Event FIFO. This counter saturates at all 1's
    Bits: 42:40 BDFifoEntryCnt(ro) - Number of buffer descriptors resident in the buffer descriptor register FIFO
    Bits: 39    BLQFull(ro) - The Queue is full when set.
    Bits: 38    BLQEmpty(ro) - The queue is empty when set.
    Bits: 37:36 BLQSize(rw) - Defines the Size of the Circular Buffer List Queue
    Bits: 35:24 BLEntryCnt(ro) - Hardware calculated value of the number of entries on the Buffer List Queue. A full Buffer List Queue has an entry count equal to zero, with the BLQFull flag set. An empty Buffer List Queue has an entry count equal to zero with the BLQEmpty flag set.
    Bits: 23:12 BLHdPtr(rw) - Buffer List Queue Head Pointer. Points to the next entry to be read by hardware.
    Bits: 11:0  BLTlPtr(rw) - Buffer List Queue Tail Pointer. Points to the next entry to be written at the tail.
*/
#define NFP_NBI_DMA_BufferList_BL_QUE_CTRL0                  0x00008000
#define   NFP_NBI_DMA_BUFFERLIST_BL_QUE_CTRL0_BLQ_EVENT_FULL_CNT(_x) (((_x) & 0xff) << 48)
#define   NFP_NBI_DMA_BUFFERLIST_BL_QUE_CTRL0_BLQ_EVENT_FULL_CNT_of(_x) (((_x) >> 48) & 0xff)
#define   NFP_NBI_DMA_BUFFERLIST_BL_QUE_CTRL0_BD_FIFO_ENTRY_CNT(_x) (((_x) & 0x7) << 40)
#define   NFP_NBI_DMA_BUFFERLIST_BL_QUE_CTRL0_BD_FIFO_ENTRY_CNT_of(_x) (((_x) >> 40) & 0x7)
#define   NFP_NBI_DMA_BUFFERLIST_BL_QUE_CTRL0_BL_QUE_FULL    BIT_ULL(39)
#define   NFP_NBI_DMA_BUFFERLIST_BL_QUE_CTRL0_BL_QUE_EMPTY   BIT_ULL(38)
#define   NFP_NBI_DMA_BUFFERLIST_BL_QUE_CTRL0_BL_QUE_SIZE(_x) (((_x) & 0x3) << 36)
#define   NFP_NBI_DMA_BUFFERLIST_BL_QUE_CTRL0_BL_QUE_SIZE_of(_x) (((_x) >> 36) & 0x3)
#define     NFP_NBI_DMA_BUFFERLIST_BL_QUE_CTRL0_BL_QUE_SIZE_512 (0)
#define     NFP_NBI_DMA_BUFFERLIST_BL_QUE_CTRL0_BL_QUE_SIZE_1K (1)
#define     NFP_NBI_DMA_BUFFERLIST_BL_QUE_CTRL0_BL_QUE_SIZE_2K (2)
#define     NFP_NBI_DMA_BUFFERLIST_BL_QUE_CTRL0_BL_QUE_SIZE_4K (3)
#define   NFP_NBI_DMA_BUFFERLIST_BL_QUE_CTRL0_BL_ENTRY_CNT(_x) (((_x) & 0xfff) << 24)
#define   NFP_NBI_DMA_BUFFERLIST_BL_QUE_CTRL0_BL_ENTRY_CNT_of(_x) (((_x) >> 24) & 0xfff)
#define   NFP_NBI_DMA_BUFFERLIST_BL_QUE_CTRL0_BL_HEAD_PTR(_x) (((_x) & 0xfff) << 12)
#define   NFP_NBI_DMA_BUFFERLIST_BL_QUE_CTRL0_BL_HEAD_PTR_of(_x) (((_x) >> 12) & 0xfff)
#define   NFP_NBI_DMA_BUFFERLIST_BL_QUE_CTRL0_BL_TAIL_PTR(_x) (((_x) & 0xfff) << 0)
#define   NFP_NBI_DMA_BUFFERLIST_BL_QUE_CTRL0_BL_TAIL_PTR_of(_x) (((_x) >> 0) & 0xfff)
/*----------------------------------------------------------------
  Register: BLQueCtrl1 - Configures the buffer list circular queue and provides status
    Bits: 55:48 BLQEventFullCnt(rc) - Debug counter that increments when a BLQ Event is dropped due to a full BLQ Event FIFO. This counter saturates at all 1's
    Bits: 42:40 BDFifoEntryCnt(ro) - Number of buffer descriptors resident in the buffer descriptor register FIFO
    Bits: 39    BLQFull(ro) - The Queue is full when set.
    Bits: 38    BLQEmpty(ro) - The queue is empty when set.
    Bits: 37:36 BLQSize(rw) - Defines the Size of the Circular Buffer List Queue
    Bits: 35:24 BLEntryCnt(ro) - Hardware calculated value of the number of entries on the Buffer List Queue. A full Buffer List Queue has an entry count equal to zero, with the BLQFull flag set. An empty Buffer List Queue has an entry count equal to zero with the BLQEmpty flag set.
    Bits: 23:12 BLHdPtr(rw) - Buffer List Queue Head Pointer. Points to the next entry to be read by hardware.
    Bits: 11:0  BLTlPtr(rw) - Buffer List Queue Tail Pointer. Points to the next entry to be written at the tail.
*/
#define NFP_NBI_DMA_BufferList_BL_QUE_CTRL1                  0x00008008
#define   NFP_NBI_DMA_BUFFERLIST_BL_QUE_CTRL1_BLQ_EVENT_FULL_CNT(_x) (((_x) & 0xff) << 48)
#define   NFP_NBI_DMA_BUFFERLIST_BL_QUE_CTRL1_BLQ_EVENT_FULL_CNT_of(_x) (((_x) >> 48) & 0xff)
#define   NFP_NBI_DMA_BUFFERLIST_BL_QUE_CTRL1_BD_FIFO_ENTRY_CNT(_x) (((_x) & 0x7) << 40)
#define   NFP_NBI_DMA_BUFFERLIST_BL_QUE_CTRL1_BD_FIFO_ENTRY_CNT_of(_x) (((_x) >> 40) & 0x7)
#define   NFP_NBI_DMA_BUFFERLIST_BL_QUE_CTRL1_BL_QUE_FULL    BIT_ULL(39)
#define   NFP_NBI_DMA_BUFFERLIST_BL_QUE_CTRL1_BL_QUE_EMPTY   BIT_ULL(38)
#define   NFP_NBI_DMA_BUFFERLIST_BL_QUE_CTRL1_BL_QUE_SIZE(_x) (((_x) & 0x3) << 36)
#define   NFP_NBI_DMA_BUFFERLIST_BL_QUE_CTRL1_BL_QUE_SIZE_of(_x) (((_x) >> 36) & 0x3)
#define     NFP_NBI_DMA_BUFFERLIST_BL_QUE_CTRL1_BL_QUE_SIZE_512 (0)
#define     NFP_NBI_DMA_BUFFERLIST_BL_QUE_CTRL1_BL_QUE_SIZE_1K (1)
#define     NFP_NBI_DMA_BUFFERLIST_BL_QUE_CTRL1_BL_QUE_SIZE_2K (2)
#define     NFP_NBI_DMA_BUFFERLIST_BL_QUE_CTRL1_BL_QUE_SIZE_4K (3)
#define   NFP_NBI_DMA_BUFFERLIST_BL_QUE_CTRL1_BL_ENTRY_CNT(_x) (((_x) & 0xfff) << 24)
#define   NFP_NBI_DMA_BUFFERLIST_BL_QUE_CTRL1_BL_ENTRY_CNT_of(_x) (((_x) >> 24) & 0xfff)
#define   NFP_NBI_DMA_BUFFERLIST_BL_QUE_CTRL1_BL_HEAD_PTR(_x) (((_x) & 0xfff) << 12)
#define   NFP_NBI_DMA_BUFFERLIST_BL_QUE_CTRL1_BL_HEAD_PTR_of(_x) (((_x) >> 12) & 0xfff)
#define   NFP_NBI_DMA_BUFFERLIST_BL_QUE_CTRL1_BL_TAIL_PTR(_x) (((_x) & 0xfff) << 0)
#define   NFP_NBI_DMA_BUFFERLIST_BL_QUE_CTRL1_BL_TAIL_PTR_of(_x) (((_x) >> 0) & 0xfff)
/*----------------------------------------------------------------
  Register: BLQueCtrl2 - Configures the buffer list circular queue and provides status
    Bits: 55:48 BLQEventFullCnt(rc) - Debug counter that increments when a BLQ Event is dropped due to a full BLQ Event FIFO. This counter saturates at all 1's
    Bits: 42:40 BDFifoEntryCnt(ro) - Number of buffer descriptors resident in the buffer descriptor register FIFO
    Bits: 39    BLQFull(ro) - The Queue is full when set.
    Bits: 38    BLQEmpty(ro) - The queue is empty when set.
    Bits: 37:36 BLQSize(rw) - Defines the Size of the Circular Buffer List Queue
    Bits: 35:24 BLEntryCnt(ro) - Hardware calculated value of the number of entries on the Buffer List Queue. A full Buffer List Queue has an entry count equal to zero, with the BLQFull flag set. An empty Buffer List Queue has an entry count equal to zero with the BLQEmpty flag set.
    Bits: 23:12 BLHdPtr(rw) - Buffer List Queue Head Pointer. Points to the next entry to be read by hardware.
    Bits: 11:0  BLTlPtr(rw) - Buffer List Queue Tail Pointer. Points to the next entry to be written at the tail.
*/
#define NFP_NBI_DMA_BufferList_BL_QUE_CTRL2                  0x00008010
#define   NFP_NBI_DMA_BUFFERLIST_BL_QUE_CTRL2_BLQ_EVENT_FULL_CNT(_x) (((_x) & 0xff) << 48)
#define   NFP_NBI_DMA_BUFFERLIST_BL_QUE_CTRL2_BLQ_EVENT_FULL_CNT_of(_x) (((_x) >> 48) & 0xff)
#define   NFP_NBI_DMA_BUFFERLIST_BL_QUE_CTRL2_BD_FIFO_ENTRY_CNT(_x) (((_x) & 0x7) << 40)
#define   NFP_NBI_DMA_BUFFERLIST_BL_QUE_CTRL2_BD_FIFO_ENTRY_CNT_of(_x) (((_x) >> 40) & 0x7)
#define   NFP_NBI_DMA_BUFFERLIST_BL_QUE_CTRL2_BL_QUE_FULL    BIT_ULL(39)
#define   NFP_NBI_DMA_BUFFERLIST_BL_QUE_CTRL2_BL_QUE_EMPTY   BIT_ULL(38)
#define   NFP_NBI_DMA_BUFFERLIST_BL_QUE_CTRL2_BL_QUE_SIZE(_x) (((_x) & 0x3) << 36)
#define   NFP_NBI_DMA_BUFFERLIST_BL_QUE_CTRL2_BL_QUE_SIZE_of(_x) (((_x) >> 36) & 0x3)
#define     NFP_NBI_DMA_BUFFERLIST_BL_QUE_CTRL2_BL_QUE_SIZE_512 (0)
#define     NFP_NBI_DMA_BUFFERLIST_BL_QUE_CTRL2_BL_QUE_SIZE_1K (1)
#define     NFP_NBI_DMA_BUFFERLIST_BL_QUE_CTRL2_BL_QUE_SIZE_2K (2)
#define     NFP_NBI_DMA_BUFFERLIST_BL_QUE_CTRL2_BL_QUE_SIZE_4K (3)
#define   NFP_NBI_DMA_BUFFERLIST_BL_QUE_CTRL2_BL_ENTRY_CNT(_x) (((_x) & 0xfff) << 24)
#define   NFP_NBI_DMA_BUFFERLIST_BL_QUE_CTRL2_BL_ENTRY_CNT_of(_x) (((_x) >> 24) & 0xfff)
#define   NFP_NBI_DMA_BUFFERLIST_BL_QUE_CTRL2_BL_HEAD_PTR(_x) (((_x) & 0xfff) << 12)
#define   NFP_NBI_DMA_BUFFERLIST_BL_QUE_CTRL2_BL_HEAD_PTR_of(_x) (((_x) >> 12) & 0xfff)
#define   NFP_NBI_DMA_BUFFERLIST_BL_QUE_CTRL2_BL_TAIL_PTR(_x) (((_x) & 0xfff) << 0)
#define   NFP_NBI_DMA_BUFFERLIST_BL_QUE_CTRL2_BL_TAIL_PTR_of(_x) (((_x) >> 0) & 0xfff)
/*----------------------------------------------------------------
  Register: BLQueCtrl3 - Configures the buffer list circular queue and provides status
    Bits: 55:48 BLQEventFullCnt(rc) - Debug counter that increments when a BLQ Event is dropped due to a full BLQ Event FIFO. This counter saturates at all 1's
    Bits: 42:40 BDFifoEntryCnt(ro) - Number of buffer descriptors resident in the buffer descriptor register FIFO
    Bits: 39    BLQFull(ro) - The Queue is full when set.
    Bits: 38    BLQEmpty(ro) - The queue is empty when set.
    Bits: 37:36 BLQSize(rw) - Defines the Size of the Circular Buffer List Queue
    Bits: 35:24 BLEntryCnt(ro) - Hardware calculated value of the number of entries on the Buffer List Queue. A full Buffer List Queue has an entry count equal to zero, with the BLQFull flag set. An empty Buffer List Queue has an entry count equal to zero with the BLQEmpty flag set.
    Bits: 23:12 BLHdPtr(rw) - Buffer List Queue Head Pointer. Points to the next entry to be read by hardware.
    Bits: 11:0  BLTlPtr(rw) - Buffer List Queue Tail Pointer. Points to the next entry to be written at the tail.
*/
#define NFP_NBI_DMA_BufferList_BL_QUE_CTRL3                  0x00008018
#define   NFP_NBI_DMA_BUFFERLIST_BL_QUE_CTRL3_BLQ_EVENT_FULL_CNT(_x) (((_x) & 0xff) << 48)
#define   NFP_NBI_DMA_BUFFERLIST_BL_QUE_CTRL3_BLQ_EVENT_FULL_CNT_of(_x) (((_x) >> 48) & 0xff)
#define   NFP_NBI_DMA_BUFFERLIST_BL_QUE_CTRL3_BD_FIFO_ENTRY_CNT(_x) (((_x) & 0x7) << 40)
#define   NFP_NBI_DMA_BUFFERLIST_BL_QUE_CTRL3_BD_FIFO_ENTRY_CNT_of(_x) (((_x) >> 40) & 0x7)
#define   NFP_NBI_DMA_BUFFERLIST_BL_QUE_CTRL3_BL_QUE_FULL    BIT_ULL(39)
#define   NFP_NBI_DMA_BUFFERLIST_BL_QUE_CTRL3_BL_QUE_EMPTY   BIT_ULL(38)
#define   NFP_NBI_DMA_BUFFERLIST_BL_QUE_CTRL3_BL_QUE_SIZE(_x) (((_x) & 0x3) << 36)
#define   NFP_NBI_DMA_BUFFERLIST_BL_QUE_CTRL3_BL_QUE_SIZE_of(_x) (((_x) >> 36) & 0x3)
#define     NFP_NBI_DMA_BUFFERLIST_BL_QUE_CTRL3_BL_QUE_SIZE_512 (0)
#define     NFP_NBI_DMA_BUFFERLIST_BL_QUE_CTRL3_BL_QUE_SIZE_1K (1)
#define     NFP_NBI_DMA_BUFFERLIST_BL_QUE_CTRL3_BL_QUE_SIZE_2K (2)
#define     NFP_NBI_DMA_BUFFERLIST_BL_QUE_CTRL3_BL_QUE_SIZE_4K (3)
#define   NFP_NBI_DMA_BUFFERLIST_BL_QUE_CTRL3_BL_ENTRY_CNT(_x) (((_x) & 0xfff) << 24)
#define   NFP_NBI_DMA_BUFFERLIST_BL_QUE_CTRL3_BL_ENTRY_CNT_of(_x) (((_x) >> 24) & 0xfff)
#define   NFP_NBI_DMA_BUFFERLIST_BL_QUE_CTRL3_BL_HEAD_PTR(_x) (((_x) & 0xfff) << 12)
#define   NFP_NBI_DMA_BUFFERLIST_BL_QUE_CTRL3_BL_HEAD_PTR_of(_x) (((_x) >> 12) & 0xfff)
#define   NFP_NBI_DMA_BUFFERLIST_BL_QUE_CTRL3_BL_TAIL_PTR(_x) (((_x) & 0xfff) << 0)
#define   NFP_NBI_DMA_BUFFERLIST_BL_QUE_CTRL3_BL_TAIL_PTR_of(_x) (((_x) >> 0) & 0xfff)
/*----------------------------------------------------------------
  Register: NbiDmaBCSramEntry[1024] - NBI DMA Buffer Completion SRAM Entry X
    Bits: 31:24 bufNum3(rw) - Buffer number 3
    Bits: 23:16 bufNum2(rw) - Buffer number 2
    Bits: 15:8  bufNum1(rw) - Buffer number 1
    Bits:  7:0  bufNum0(rw) - Buffer number 0
*/
#define NFP_NBI_DMA_BCSRAM_NBIDMA_BCSRAM_ENTRY(_x)           (0x0000a000 + (0x8 * ((_x) & 0x3ff)))
#define   NFP_NBI_DMA_BCSRAM_NBIDMA_BCSRAM_ENTRY_BUFNUM3(_x) (((_x) & 0xff) << 24)
#define   NFP_NBI_DMA_BCSRAM_NBIDMA_BCSRAM_ENTRY_BUFNUM3_of(_x) (((_x) >> 24) & 0xff)
#define   NFP_NBI_DMA_BCSRAM_NBIDMA_BCSRAM_ENTRY_BUFNUM2(_x) (((_x) & 0xff) << 16)
#define   NFP_NBI_DMA_BCSRAM_NBIDMA_BCSRAM_ENTRY_BUFNUM2_of(_x) (((_x) >> 16) & 0xff)
#define   NFP_NBI_DMA_BCSRAM_NBIDMA_BCSRAM_ENTRY_BUFNUM1(_x) (((_x) & 0xff) << 8)
#define   NFP_NBI_DMA_BCSRAM_NBIDMA_BCSRAM_ENTRY_BUFNUM1_of(_x) (((_x) >> 8) & 0xff)
#define   NFP_NBI_DMA_BCSRAM_NBIDMA_BCSRAM_ENTRY_BUFNUM0(_x) (((_x) & 0xff) << 0)
#define   NFP_NBI_DMA_BCSRAM_NBIDMA_BCSRAM_ENTRY_BUFNUM0_of(_x) (((_x) >> 0) & 0xff)
/*----------------------------------------------------------------
  Register: NbiDmaCfg - NBI DMA Configuration register
    Bits: 20:17 DisPushBusSelect(rw) - Disables a push bus from being selected by the read arbiter when set. Used to force packet traffic to not use a particular push bus.
    Bits: 16    DisBcRamErr(rw) - Controls how the DMA engine handles multi-bit ECC errors received from the Buffer Completion RAM.
    Bits: 15    DisDsRamErr(rw) - Controls how the DMA engine handles multi-bit ECC errors received from the DMA State RAM.
    Bits: 14    DisBdRamErr(rw) - Controls how the BLQ logic handles multi-bit ECC errors received from the Buffer Descriptor RAM
    Bits: 13    DisBufRdErr(rw) - Controls how the hardware forwards the received NBI Buffer Memory Read Error to the output data buses.
    Bits: 12    DisRxPushLastErr(rw) - Controls how the hardware responds to a push bus input protocol error for allocation response transactions
    Bits: 11    DisRxAllocInErr(rw) - Controls how the hardware responds to a push bus input error for allocation response transactions.
    Bits: 10    DisRxBlqWrInErr(rw) - Controls how the hardware responds to a push bus input error for BLQ writes transactions.
    Bits:  9:7  NbiNum(rw) - This field is placed in the 3 lsbits of the Packet Allocation Command Address field. It is used by the CTM to identify which NBI sent the command.
    Bits:  6    CtmPollEna(rw) - Configures whether the polling function is enabled or disabled.
    Bits:  5:4  CtmPollIntvl(rw) - Configures the amount of time between packet allocation poll commands to a CTM. If CtmPollSearchEna = 0, the polling interval is defined by the CtmPollIntvl. If the CtmPollSearchEna = 1, the poll interval is defined by the following equation: (CtmPollIntvl/64)*(# of active CTMs).
    Bits:  1    RateLimitEnable(rw) - Selects whether to limit the bandwidth on the four Push Buses.
    Bits:  0    CtmPollSearchEna(rw) - Controls how CTM polling is performed. When set the CTM polling logic skips to the next active CTM every poll interval. When clear the CTM polling logic increments the CTM counter by one every poll interval.
*/
#define NFP_NBI_DMAX_CSR_NBI_DMA_CFG                         0x00000000
#define   NFP_NBI_DMAX_CSR_NBI_DMA_CFG_DIS_PUSHBUS_SELECT(_x) (((_x) & 0xf) << 17)
#define   NFP_NBI_DMAX_CSR_NBI_DMA_CFG_DIS_PUSHBUS_SELECT_of(_x) (((_x) >> 17) & 0xf)
#define   NFP_NBI_DMAX_CSR_NBI_DMA_CFG_DIS_BCRAM_ERR         BIT(16)
#define     NFP_NBI_DMAX_CSR_NBI_DMA_CFG_DIS_BCRAM_ERR_ENABLE (0 << 16)
#define     NFP_NBI_DMAX_CSR_NBI_DMA_CFG_DIS_BCRAM_ERR_DISABLE BIT(16)
#define   NFP_NBI_DMAX_CSR_NBI_DMA_CFG_DIS_DSRAM_ERR         BIT(15)
#define     NFP_NBI_DMAX_CSR_NBI_DMA_CFG_DIS_DSRAM_ERR_ENABLE (0 << 15)
#define     NFP_NBI_DMAX_CSR_NBI_DMA_CFG_DIS_DSRAM_ERR_DISABLE BIT(15)
#define   NFP_NBI_DMAX_CSR_NBI_DMA_CFG_DIS_BDRAM_ERR         BIT(14)
#define     NFP_NBI_DMAX_CSR_NBI_DMA_CFG_DIS_BDRAM_ERR_ENABLE (0 << 14)
#define     NFP_NBI_DMAX_CSR_NBI_DMA_CFG_DIS_BDRAM_ERR_DISABLE BIT(14)
#define   NFP_NBI_DMAX_CSR_NBI_DMA_CFG_DIS_BUFRD_ERR         BIT(13)
#define     NFP_NBI_DMAX_CSR_NBI_DMA_CFG_DIS_BUFRD_ERR_ENABLE (0 << 13)
#define     NFP_NBI_DMAX_CSR_NBI_DMA_CFG_DIS_BUFRD_ERR_DISABLE BIT(13)
#define   NFP_NBI_DMAX_CSR_NBI_DMA_CFG_DIS_RXPUSH_LAST_ERR   BIT(12)
#define     NFP_NBI_DMAX_CSR_NBI_DMA_CFG_DIS_RXPUSH_LAST_ERR_ENABLE (0 << 12)
#define     NFP_NBI_DMAX_CSR_NBI_DMA_CFG_DIS_RXPUSH_LAST_ERR_DISABLE BIT(12)
#define   NFP_NBI_DMAX_CSR_NBI_DMA_CFG_DIS_RXALLOC_IN_ERR    BIT(11)
#define     NFP_NBI_DMAX_CSR_NBI_DMA_CFG_DIS_RXALLOC_IN_ERR_ENABLE (0 << 11)
#define     NFP_NBI_DMAX_CSR_NBI_DMA_CFG_DIS_RXALLOC_IN_ERR_DISABLE BIT(11)
#define   NFP_NBI_DMAX_CSR_NBI_DMA_CFG_DIS_RXBLQWR_IN_ERR    BIT(10)
#define     NFP_NBI_DMAX_CSR_NBI_DMA_CFG_DIS_RXBLQWR_IN_ERR_ENABLE (0 << 10)
#define     NFP_NBI_DMAX_CSR_NBI_DMA_CFG_DIS_RXBLQWR_IN_ERR_DISABLE BIT(10)
#define   NFP_NBI_DMAX_CSR_NBI_DMA_CFG_NBI_NUM(_x)           (((_x) & 0x7) << 7)
#define   NFP_NBI_DMAX_CSR_NBI_DMA_CFG_NBI_NUM_of(_x)        (((_x) >> 7) & 0x7)
#define   NFP_NBI_DMAX_CSR_NBI_DMA_CFG_CTM_POLL_ENA          BIT(6)
#define     NFP_NBI_DMAX_CSR_NBI_DMA_CFG_CTM_POLL_ENA_DISABLE (0 << 6)
#define     NFP_NBI_DMAX_CSR_NBI_DMA_CFG_CTM_POLL_ENA_ENABLE BIT(6)
#define   NFP_NBI_DMAX_CSR_NBI_DMA_CFG_CTM_POLL_INTVL(_x)    (((_x) & 0x3) << 4)
#define   NFP_NBI_DMAX_CSR_NBI_DMA_CFG_CTM_POLL_INTVL_of(_x) (((_x) >> 4) & 0x3)
#define     NFP_NBI_DMAX_CSR_NBI_DMA_CFG_CTM_POLL_INTVL_64US (0)
#define     NFP_NBI_DMAX_CSR_NBI_DMA_CFG_CTM_POLL_INTVL_128US (1)
#define     NFP_NBI_DMAX_CSR_NBI_DMA_CFG_CTM_POLL_INTVL_256US (2)
#define     NFP_NBI_DMAX_CSR_NBI_DMA_CFG_CTM_POLL_INTVL_512US (3)
#define   NFP_NBI_DMAX_CSR_NBI_DMA_CFG_RATE_LIMIT_ENABLE     BIT(1)
#define     NFP_NBI_DMAX_CSR_NBI_DMA_CFG_RATE_LIMIT_ENABLE_DISABLE (0 << 1)
#define     NFP_NBI_DMAX_CSR_NBI_DMA_CFG_RATE_LIMIT_ENABLE_ENABLE BIT(1)
#define   NFP_NBI_DMAX_CSR_NBI_DMA_CFG_CTM_POLL_SEARCH_ENA   BIT(0)
#define     NFP_NBI_DMAX_CSR_NBI_DMA_CFG_CTM_POLL_SEARCH_ENA_DISABLE (0 << 0)
#define     NFP_NBI_DMAX_CSR_NBI_DMA_CFG_CTM_POLL_SEARCH_ENA_ENABLE BIT(0)
/*----------------------------------------------------------------
  Register: NbiDmaSpare - General Purpose Register
    Bits: 31:0  NbiDmaSpare(rw) - General use register.
*/
#define NFP_NBI_DMAX_CSR_NBI_DMA_SPARE                       0x00000004
#define   NFP_NBI_DMAX_CSR_NBI_DMA_SPARE_NBI_DMA_SPARE(_x)   (((_x) & 0xffffffff) << 0)
#define   NFP_NBI_DMAX_CSR_NBI_DMA_SPARE_NBI_DMA_SPARE_of(_x) (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: NbiDmaBlqEvent - NBI DMA Buffer List Queue Event Control
    Bits: 20    DisBLQEventFifoFullAssert(rw) - Set this bit to disable the BLQ Event FIFO overflow assertion
    Bits: 19:18 BLQEventThreshold3(rw) - An event is generated every N buffer descriptors read from buffer list queue3, where N is configurable from 16 to 128.
    Bits: 17:16 BLQEventThreshold2(rw) - An event is generated every N buffer descriptors read from buffer list queue2, where N is configurable from 16 to 128.
    Bits: 15:14 BLQEventThreshold1(rw) - An event is generated every N buffer descriptors read from buffer list queue1, where N is configurable from 16 to 128.
    Bits: 13:12 BLQEventThreshold0(rw) - An event is generated every N buffer descriptors read from buffer list queue0, where N is configurable from 16 to 128.
    Bits:  7:4  BLQEventNotFullEnable(rw) - One bit per buffer list queue. Enables the generation of BLQ Events when the queue transitions from full to not full. Disabled when clear. The event type value used is 0x1 (event_type_fifo_not_full)
    Bits:  3:0  BLQEventStatusEnable(rw) - One bit per buffer list queue. Enables BLQ Status Events when set. Disabled when clear. A Status Event is sent when the number of entries read from the queue reaches the programmed value in the BLQEventThreshold register. The event type value used is 0x5 (event_type_value_update)
*/
#define NFP_NBI_DMAX_CSR_NBI_DMA_BLQ_EVENT                   0x0000000c
#define   NFP_NBI_DMAX_CSR_NBI_DMA_BLQ_EVENT_DIS_BLQ_EVENT_FIFO_FULL_ASSERT BIT(20)
#define   NFP_NBI_DMAX_CSR_NBI_DMA_BLQ_EVENT_BLQEVENT_THRESHOLD3(_x) (((_x) & 0x3) << 18)
#define   NFP_NBI_DMAX_CSR_NBI_DMA_BLQ_EVENT_BLQEVENT_THRESHOLD3_of(_x) (((_x) >> 18) & 0x3)
#define     NFP_NBI_DMAX_CSR_NBI_DMA_BLQ_EVENT_BLQEVENT_THRESHOLD3_16 (0)
#define     NFP_NBI_DMAX_CSR_NBI_DMA_BLQ_EVENT_BLQEVENT_THRESHOLD3_32 (1)
#define     NFP_NBI_DMAX_CSR_NBI_DMA_BLQ_EVENT_BLQEVENT_THRESHOLD3_64 (2)
#define     NFP_NBI_DMAX_CSR_NBI_DMA_BLQ_EVENT_BLQEVENT_THRESHOLD3_128 (3)
#define   NFP_NBI_DMAX_CSR_NBI_DMA_BLQ_EVENT_BLQEVENT_THRESHOLD2(_x) (((_x) & 0x3) << 16)
#define   NFP_NBI_DMAX_CSR_NBI_DMA_BLQ_EVENT_BLQEVENT_THRESHOLD2_of(_x) (((_x) >> 16) & 0x3)
#define     NFP_NBI_DMAX_CSR_NBI_DMA_BLQ_EVENT_BLQEVENT_THRESHOLD2_16 (0)
#define     NFP_NBI_DMAX_CSR_NBI_DMA_BLQ_EVENT_BLQEVENT_THRESHOLD2_32 (1)
#define     NFP_NBI_DMAX_CSR_NBI_DMA_BLQ_EVENT_BLQEVENT_THRESHOLD2_64 (2)
#define     NFP_NBI_DMAX_CSR_NBI_DMA_BLQ_EVENT_BLQEVENT_THRESHOLD2_128 (3)
#define   NFP_NBI_DMAX_CSR_NBI_DMA_BLQ_EVENT_BLQEVENT_THRESHOLD1(_x) (((_x) & 0x3) << 14)
#define   NFP_NBI_DMAX_CSR_NBI_DMA_BLQ_EVENT_BLQEVENT_THRESHOLD1_of(_x) (((_x) >> 14) & 0x3)
#define     NFP_NBI_DMAX_CSR_NBI_DMA_BLQ_EVENT_BLQEVENT_THRESHOLD1_16 (0)
#define     NFP_NBI_DMAX_CSR_NBI_DMA_BLQ_EVENT_BLQEVENT_THRESHOLD1_32 (1)
#define     NFP_NBI_DMAX_CSR_NBI_DMA_BLQ_EVENT_BLQEVENT_THRESHOLD1_64 (2)
#define     NFP_NBI_DMAX_CSR_NBI_DMA_BLQ_EVENT_BLQEVENT_THRESHOLD1_128 (3)
#define   NFP_NBI_DMAX_CSR_NBI_DMA_BLQ_EVENT_BLQEVENT_THRESHOLD0(_x) (((_x) & 0x3) << 12)
#define   NFP_NBI_DMAX_CSR_NBI_DMA_BLQ_EVENT_BLQEVENT_THRESHOLD0_of(_x) (((_x) >> 12) & 0x3)
#define     NFP_NBI_DMAX_CSR_NBI_DMA_BLQ_EVENT_BLQEVENT_THRESHOLD0_16 (0)
#define     NFP_NBI_DMAX_CSR_NBI_DMA_BLQ_EVENT_BLQEVENT_THRESHOLD0_32 (1)
#define     NFP_NBI_DMAX_CSR_NBI_DMA_BLQ_EVENT_BLQEVENT_THRESHOLD0_64 (2)
#define     NFP_NBI_DMAX_CSR_NBI_DMA_BLQ_EVENT_BLQEVENT_THRESHOLD0_128 (3)
#define   NFP_NBI_DMAX_CSR_NBI_DMA_BLQ_EVENT_BLQ_EVENT_NOT_FULL_ENABLE(_x) (((_x) & 0xf) << 4)
#define   NFP_NBI_DMAX_CSR_NBI_DMA_BLQ_EVENT_BLQ_EVENT_NOT_FULL_ENABLE_of(_x) (((_x) >> 4) & 0xf)
#define     NFP_NBI_DMAX_CSR_NBI_DMA_BLQ_EVENT_BLQ_EVENT_NOT_FULL_ENABLE_DISABLE (0)
#define     NFP_NBI_DMAX_CSR_NBI_DMA_BLQ_EVENT_BLQ_EVENT_NOT_FULL_ENABLE_ENABLE (1)
#define   NFP_NBI_DMAX_CSR_NBI_DMA_BLQ_EVENT_BLQ_EVENT_STATUS_ENABLE(_x) (((_x) & 0xf) << 0)
#define   NFP_NBI_DMAX_CSR_NBI_DMA_BLQ_EVENT_BLQ_EVENT_STATUS_ENABLE_of(_x) (((_x) >> 0) & 0xf)
#define     NFP_NBI_DMAX_CSR_NBI_DMA_BLQ_EVENT_BLQ_EVENT_STATUS_ENABLE_DISABLE (0)
#define     NFP_NBI_DMAX_CSR_NBI_DMA_BLQ_EVENT_BLQ_EVENT_STATUS_ENABLE_ENABLE (1)
/*----------------------------------------------------------------
  Register: NbiDmaRate - NBI DMA Token Refresh Rate Register
    Bits:  7:6  CreditRate3(rw) - Sets the credit rate for push bus 3. Values are in 4 cycle increments.
    Bits:  5:4  CreditRate2(rw) - Sets the credit rate for push bus 2. Values are in 4 cycle increments.
    Bits:  3:2  CreditRate1(rw) - Sets the credit rate for push bus 1. Values are in 4 cycle increments.
    Bits:  1:0  CreditRate0(rw) - Sets the credit rate for push bus 0. Values are in 4 cycle increments.
*/
#define NFP_NBI_DMAX_CSR_NBI_DMA_RATE                        0x00000010
#define   NFP_NBI_DMAX_CSR_NBI_DMA_RATE_TOKEN_RATE3(_x)      (((_x) & 0x3) << 6)
#define   NFP_NBI_DMAX_CSR_NBI_DMA_RATE_TOKEN_RATE3_of(_x)   (((_x) >> 6) & 0x3)
#define     NFP_NBI_DMAX_CSR_NBI_DMA_RATE_TOKEN_RATE3_4      (0)
#define     NFP_NBI_DMAX_CSR_NBI_DMA_RATE_TOKEN_RATE3_8      (1)
#define     NFP_NBI_DMAX_CSR_NBI_DMA_RATE_TOKEN_RATE3_12     (2)
#define     NFP_NBI_DMAX_CSR_NBI_DMA_RATE_TOKEN_RATE3_16     (3)
#define   NFP_NBI_DMAX_CSR_NBI_DMA_RATE_TOKEN_RATE2(_x)      (((_x) & 0x3) << 4)
#define   NFP_NBI_DMAX_CSR_NBI_DMA_RATE_TOKEN_RATE2_of(_x)   (((_x) >> 4) & 0x3)
#define     NFP_NBI_DMAX_CSR_NBI_DMA_RATE_TOKEN_RATE2_4      (0)
#define     NFP_NBI_DMAX_CSR_NBI_DMA_RATE_TOKEN_RATE2_8      (1)
#define     NFP_NBI_DMAX_CSR_NBI_DMA_RATE_TOKEN_RATE2_12     (2)
#define     NFP_NBI_DMAX_CSR_NBI_DMA_RATE_TOKEN_RATE2_16     (3)
#define   NFP_NBI_DMAX_CSR_NBI_DMA_RATE_TOKEN_RATE1(_x)      (((_x) & 0x3) << 2)
#define   NFP_NBI_DMAX_CSR_NBI_DMA_RATE_TOKEN_RATE1_of(_x)   (((_x) >> 2) & 0x3)
#define     NFP_NBI_DMAX_CSR_NBI_DMA_RATE_TOKEN_RATE1_4      (0)
#define     NFP_NBI_DMAX_CSR_NBI_DMA_RATE_TOKEN_RATE1_8      (1)
#define     NFP_NBI_DMAX_CSR_NBI_DMA_RATE_TOKEN_RATE1_12     (2)
#define     NFP_NBI_DMAX_CSR_NBI_DMA_RATE_TOKEN_RATE1_16     (3)
#define   NFP_NBI_DMAX_CSR_NBI_DMA_RATE_TOKEN_RATE0(_x)      (((_x) & 0x3) << 0)
#define   NFP_NBI_DMAX_CSR_NBI_DMA_RATE_TOKEN_RATE0_of(_x)   (((_x) >> 0) & 0x3)
#define     NFP_NBI_DMAX_CSR_NBI_DMA_RATE_TOKEN_RATE0_4      (0)
#define     NFP_NBI_DMAX_CSR_NBI_DMA_RATE_TOKEN_RATE0_8      (1)
#define     NFP_NBI_DMAX_CSR_NBI_DMA_RATE_TOKEN_RATE0_12     (2)
#define     NFP_NBI_DMAX_CSR_NBI_DMA_RATE_TOKEN_RATE0_16     (3)
/*----------------------------------------------------------------
  Register: NbiDmaCredit - NBI DMA Token Credit Registers
    Bits: 15:14 Threshold3(rw) - Sets the threshold that the leaky bucket is closed. Values are in increments of 16 Credits. A value of 0 equates to 16 credits or 128 bytes. A value of 3 equates to 64 credits or 512 bytes.
    Bits: 13:12 Threshold2(rw) - Sets the threshold that the leaky bucket is closed. Values are in increments of 16 Credits. A value of 0 equates to 16 credits or 128 bytes. A value of 3 equates to 64 credits or 512 bytes.
    Bits: 11:10 Threshold1(rw) - Sets the threshold that the leaky bucket is closed. Values are in increments of 16 Credits. A value of 0 equates to 16 credits or 128 bytes. A value of 3 equates to 64 credits or 512 bytes.
    Bits:  9:8  Threshold0(rw) - Sets the threshold that the leaky bucket is closed. Values are in increments of 16 Credits. A value of 0 equates to 16 credits or 128 bytes. A value of 3 equates to 64 credits or 512 bytes.
    Bits:  7:6  Credit3(rw) - Sets the number of credits to subtract from the leaky bucket for push bus 3. A credit equates to 8-bytes or 1 cycle on the bus. A value of zero equates to 1 credits. A value of 3 equates to 4 credits.
    Bits:  5:4  Credit2(rw) - Sets the number of credits to subtract from the leaky bucket for push bus 2. A credit equates to 8-bytes or 1 cycle on the bus. A value of zero equates to 1 credits. A value of 3 equates to 4 credits.
    Bits:  3:2  Credit1(rw) - Sets the number of credits to subtract from the leaky bucket for push bus 1. A credit equates to 8-bytes or 1 cycle on the bus. A value of zero equates to 1 credits. A value of 3 equates to 4 credits.
    Bits:  1:0  Credit0(rw) - Sets the number of credits to subtract from the leaky bucket for push bus 0. A credit equates to 8-bytes or 1 cycle on the bus. A value of zero equates to 1 credits. A value of 3 equates to 4 credits.
*/
#define NFP_NBI_DMAX_CSR_NBI_DMA_CREDIT                      0x00000014
#define   NFP_NBI_DMAX_CSR_NBI_DMA_CREDIT_THRESHOLD3(_x)     (((_x) & 0x3) << 14)
#define   NFP_NBI_DMAX_CSR_NBI_DMA_CREDIT_THRESHOLD3_of(_x)  (((_x) >> 14) & 0x3)
#define   NFP_NBI_DMAX_CSR_NBI_DMA_CREDIT_THRESHOLD2(_x)     (((_x) & 0x3) << 12)
#define   NFP_NBI_DMAX_CSR_NBI_DMA_CREDIT_THRESHOLD2_of(_x)  (((_x) >> 12) & 0x3)
#define   NFP_NBI_DMAX_CSR_NBI_DMA_CREDIT_THRESHOLD1(_x)     (((_x) & 0x3) << 10)
#define   NFP_NBI_DMAX_CSR_NBI_DMA_CREDIT_THRESHOLD1_of(_x)  (((_x) >> 10) & 0x3)
#define   NFP_NBI_DMAX_CSR_NBI_DMA_CREDIT_THRESHOLD0(_x)     (((_x) & 0x3) << 8)
#define   NFP_NBI_DMAX_CSR_NBI_DMA_CREDIT_THRESHOLD0_of(_x)  (((_x) >> 8) & 0x3)
#define   NFP_NBI_DMAX_CSR_NBI_DMA_CREDIT_CREDIT3(_x)        (((_x) & 0x3) << 6)
#define   NFP_NBI_DMAX_CSR_NBI_DMA_CREDIT_CREDIT3_of(_x)     (((_x) >> 6) & 0x3)
#define   NFP_NBI_DMAX_CSR_NBI_DMA_CREDIT_CREDIT2(_x)        (((_x) & 0x3) << 4)
#define   NFP_NBI_DMAX_CSR_NBI_DMA_CREDIT_CREDIT2_of(_x)     (((_x) >> 4) & 0x3)
#define   NFP_NBI_DMAX_CSR_NBI_DMA_CREDIT_CREDIT1(_x)        (((_x) & 0x3) << 2)
#define   NFP_NBI_DMAX_CSR_NBI_DMA_CREDIT_CREDIT1_of(_x)     (((_x) >> 2) & 0x3)
#define   NFP_NBI_DMAX_CSR_NBI_DMA_CREDIT_CREDIT0(_x)        (((_x) & 0x3) << 0)
#define   NFP_NBI_DMAX_CSR_NBI_DMA_CREDIT_CREDIT0_of(_x)     (((_x) >> 0) & 0x3)
/*----------------------------------------------------------------
  Register: NbiDmaBpeChainEnd - NBI DMA BPE Chain End Register
    Bits: 31:0  BpeChainEnd(rw) - Configures BPE module 'n' as the end of the buffer pool entry chain, where 'n' = 0 to 31.
*/
#define NFP_NBI_DMAX_CSR_BPE_CHAIN_END                       0x00000018
#define   NFP_NBI_DMAX_CSR_BPE_CHAIN_END_BPE_CHAIN_END(_x)   (((_x) & 0xffffffff) << 0)
#define   NFP_NBI_DMAX_CSR_BPE_CHAIN_END_BPE_CHAIN_END_of(_x) (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: NbiDmaBP[8]Cfg - NBI DMA Buffer Pool X Configuration
    Bits: 13    DropEn(rw) - Enables a packet to be dropped when buffer or packet credits run out or there are no MU buffers left on the selected buffer list. When cleared packets are not dropped and the logic backs up waiting for credits to arrive.
    Bits: 12    CtmOffset(rw) - Defines the offset to add to the data reference when starting to write packet data to the CTM. The space in front of the packet is for the rewrite script used by the packet modifier.
    Bits: 11:10 PriBList(rw) - Primary Buffer list number associated. There are 4 buffer lists available, which contain buffer descriptors that are used to store packet data in internal/external MUs.
    Bits:  9:8  SecBList(rw) - Secondary Buffer list number associated. There are 4 buffer lists available, which contain buffer descriptors that are used to store packet data in internal/external MUs.
    Bits:  6:5  SplitLength(rw) - Encoded packet size used when deciding to split packets between CTM and internal/external MUs. The encoding is in powers of 2 starting at 256 bytes.
    Bits:  4:0  BpeHead(rw) - Pointer to the first Buffer Pool Entry register in the list associated with this buffer pool.
*/
#define NFP_NBI_DMAX_CSR_NBI_DMA_BP_CFG(_x)                  (0x00000020 + (0x4 * ((_x) & 0x7)))
#define   NFP_NBI_DMAX_CSR_NBI_DMA_BP_CFG_DROP_EN            BIT(13)
#define     NFP_NBI_DMAX_CSR_NBI_DMA_BP_CFG_DROP_EN_NODROP   (0 << 13)
#define     NFP_NBI_DMAX_CSR_NBI_DMA_BP_CFG_DROP_EN_DROP     BIT(13)
#define   NFP_NBI_DMAX_CSR_NBI_DMA_BP_CFG_CTM_OFFSET         BIT(12)
#define     NFP_NBI_DMAX_CSR_NBI_DMA_BP_CFG_CTM_OFFSET_128   (0 << 12)
#define     NFP_NBI_DMAX_CSR_NBI_DMA_BP_CFG_CTM_OFFSET_64    BIT(12)
#define   NFP_NBI_DMAX_CSR_NBI_DMA_BP_CFG_PRI_BP_LIST(_x)    (((_x) & 0x3) << 10)
#define   NFP_NBI_DMAX_CSR_NBI_DMA_BP_CFG_PRI_BP_LIST_of(_x) (((_x) >> 10) & 0x3)
#define   NFP_NBI_DMAX_CSR_NBI_DMA_BP_CFG_SEC_BP_LIST(_x)    (((_x) & 0x3) << 8)
#define   NFP_NBI_DMAX_CSR_NBI_DMA_BP_CFG_SEC_BP_LIST_of(_x) (((_x) >> 8) & 0x3)
#define   NFP_NBI_DMAX_CSR_NBI_DMA_BP_CFG_SPLIT_LENGTH(_x)   (((_x) & 0x3) << 5)
#define   NFP_NBI_DMAX_CSR_NBI_DMA_BP_CFG_SPLIT_LENGTH_of(_x) (((_x) >> 5) & 0x3)
#define     NFP_NBI_DMAX_CSR_NBI_DMA_BP_CFG_SPLIT_LENGTH_256 (0)
#define     NFP_NBI_DMAX_CSR_NBI_DMA_BP_CFG_SPLIT_LENGTH_512 (1)
#define     NFP_NBI_DMAX_CSR_NBI_DMA_BP_CFG_SPLIT_LENGTH_1K  (2)
#define     NFP_NBI_DMAX_CSR_NBI_DMA_BP_CFG_SPLIT_LENGTH_2K  (3)
#define   NFP_NBI_DMAX_CSR_NBI_DMA_BP_CFG_BPE_HEAD(_x)       (((_x) & 0x1f) << 0)
#define   NFP_NBI_DMAX_CSR_NBI_DMA_BP_CFG_BPE_HEAD_of(_x)    (((_x) >> 0) & 0x1f)
/*----------------------------------------------------------------
  Register: NbiDmaBpe[32]Cfg - NBI DMA Buffer Pool Entry X Configuration
    Bits: 31:27 BpeNum(rwl) - Buffer Pool Entry Number. When the register is written this value is the latched version of the buffer pool register number 0-31. Used for debug purposes.
    Bits: 26:21 Ctm(rw) - CTM number to use
    Bits: 20:10 PktCredit(rw) - Configures the number of packets allocated to the CTM
    Bits:  9:0  BufCredit(rw) - Configures the number of buffer credits in 2K byte increments associated with the CTM. The msbit is the sign bit and must be cleared during configuration.
*/
#define NFP_NBI_DMAX_CSR_NBI_DMA_BPE_CFG(_x)                 (0x00000040 + (0x4 * ((_x) & 0x1f)))
#define   NFP_NBI_DMAX_CSR_NBI_DMA_BPE_CFG_BPE_NUM(_x)       (((_x) & 0x1f) << 27)
#define   NFP_NBI_DMAX_CSR_NBI_DMA_BPE_CFG_BPE_NUM_of(_x)    (((_x) >> 27) & 0x1f)
#define   NFP_NBI_DMAX_CSR_NBI_DMA_BPE_CFG_CTM(_x)           (((_x) & 0x3f) << 21)
#define   NFP_NBI_DMAX_CSR_NBI_DMA_BPE_CFG_CTM_of(_x)        (((_x) >> 21) & 0x3f)
#define   NFP_NBI_DMAX_CSR_NBI_DMA_BPE_CFG_PKT_CREDIT(_x)    (((_x) & 0x7ff) << 10)
#define   NFP_NBI_DMAX_CSR_NBI_DMA_BPE_CFG_PKT_CREDIT_of(_x) (((_x) >> 10) & 0x7ff)
#define   NFP_NBI_DMAX_CSR_NBI_DMA_BPE_CFG_BUF_CREDIT(_x)    (((_x) & 0x3ff) << 0)
#define   NFP_NBI_DMAX_CSR_NBI_DMA_BPE_CFG_BUF_CREDIT_of(_x) (((_x) >> 0) & 0x3ff)
/*----------------------------------------------------------------
  Register: NbiDmaPktCntLo - Nbi Dma Pkt Counter Low 32-bits
    Bits: 31:0  NbiDmaPktCntLo(rc) - Lower 32-bits of 48-bit NBI DMA Tx Packet count. Reading this register latches the value in NbiDmaPktCntHi register
*/
#define NFP_NBI_DMAX_CSR_NBI_DMA_PKT_CNT_LO                  0x000000c0
#define   NFP_NBI_DMAX_CSR_NBI_DMA_PKT_CNT_LO_NBI_DMA_PKT_CNT_LO(_x) (((_x) & 0xffffffff) << 0)
#define   NFP_NBI_DMAX_CSR_NBI_DMA_PKT_CNT_LO_NBI_DMA_PKT_CNT_LO_of(_x) (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: NbiDmaPktCntHi - Nbi Dma Pkt Counter High 16-bits
    Bits: 15:0  NbiDmaPktCntHi(rc) - Upper 16-bits of 48-bit NBI Tx Packet count. Read this register after reading the NbiDmaPktCntLo register
*/
#define NFP_NBI_DMAX_CSR_NBI_DMA_PKT_CNT_HI                  0x000000c4
#define   NFP_NBI_DMAX_CSR_NBI_DMA_PKT_CNT_HI_NBI_DMA_PKT_CNT_HI(_x) (((_x) & 0xffff) << 0)
#define   NFP_NBI_DMAX_CSR_NBI_DMA_PKT_CNT_HI_NBI_DMA_PKT_CNT_HI_of(_x) (((_x) >> 0) & 0xffff)
/*----------------------------------------------------------------
  Register: NbiDmaByteCntLo - Nbi Dma Byte Counter Low 32-bits
    Bits: 31:0  NbiDmaByteCntLo(rc) - Lower 32-bits of 64-bit NBI DMA Tx Byte count. Reading this register latches the value in NbiDmaByteCntHi register
*/
#define NFP_NBI_DMAX_CSR_NBI_DMA_BYTE_CNT_LO                 0x000000c8
#define   NFP_NBI_DMAX_CSR_NBI_DMA_BYTE_CNT_LO_NBI_DMA_BYTE_CNT_LO(_x) (((_x) & 0xffffffff) << 0)
#define   NFP_NBI_DMAX_CSR_NBI_DMA_BYTE_CNT_LO_NBI_DMA_BYTE_CNT_LO_of(_x) (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: NbiDmaByteCntHi - Nbi Dma Byte Counter High 32-bits
    Bits: 31:0  NbiDmaByteCntHi(rc) - Upper 32-bits of 64-bit NBI DMA Tx Byte count. Read this register after reading the NbiDmaByteCntLo register
*/
#define NFP_NBI_DMAX_CSR_NBI_DMA_BYTE_CNT_HI                 0x000000cc
#define   NFP_NBI_DMAX_CSR_NBI_DMA_BYTE_CNT_HI_NBI_DMA_BYTE_CNT_HI(_x) (((_x) & 0xffffffff) << 0)
#define   NFP_NBI_DMAX_CSR_NBI_DMA_BYTE_CNT_HI_NBI_DMA_BYTE_CNT_HI_of(_x) (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: NbiDmaDropPktCntLo - Nbi Dma Drop Pkt Counter Low 32-bits
    Bits: 31:0  NbiDmaDropPktCntLo(rc) - Lower 32-bits of 48-bit NBI DMA Drop Packet count. Reading this register latches the value in NbiDmaDropPktCntHi register
*/
#define NFP_NBI_DMAX_CSR_NBI_DMA_DROP_PKT_CNT_LO             0x000000d0
#define   NFP_NBI_DMAX_CSR_NBI_DMA_DROP_PKT_CNT_LO_NBI_DMA_DROP_PKT_CNT_LO(_x) (((_x) & 0xffffffff) << 0)
#define   NFP_NBI_DMAX_CSR_NBI_DMA_DROP_PKT_CNT_LO_NBI_DMA_DROP_PKT_CNT_LO_of(_x) (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: NbiDmaDropPktCntHi - Nbi Dma Drop Pkt Counter High 16-bits
    Bits: 15:0  NbiDmaDropPktCntHi(rc) - Upper 16-bits of 48-bit NBI Drop Packet count. Read this register after reading the NbiDmaDropPktCntLo register
*/
#define NFP_NBI_DMAX_CSR_NBI_DMA_DROP_PKT_CNT_HI             0x000000d4
#define   NFP_NBI_DMAX_CSR_NBI_DMA_DROP_PKT_CNT_HI_NBI_DMA_DROP_PKT_CNT_HI(_x) (((_x) & 0xffff) << 0)
#define   NFP_NBI_DMAX_CSR_NBI_DMA_DROP_PKT_CNT_HI_NBI_DMA_DROP_PKT_CNT_HI_of(_x) (((_x) >> 0) & 0xffff)
/*----------------------------------------------------------------
  Register: NbiDmaDropByteCntLo - Nbi Dma Drop Byte Counter Low 32-bits
    Bits: 31:0  NbiDmaDropByteCntLo(rc) - Lower 32-bits of 64-bit NBI DMA Drop Byte count. Reading this register latches the value in NbiDmaDropByteCntHi register
*/
#define NFP_NBI_DMAX_CSR_NBI_DMA_DROP_BYTE_CNT_LO            0x000000d8
#define   NFP_NBI_DMAX_CSR_NBI_DMA_DROP_BYTE_CNT_LO_NBI_DMA_DROP_BYTE_CNT_LO(_x) (((_x) & 0xffffffff) << 0)
#define   NFP_NBI_DMAX_CSR_NBI_DMA_DROP_BYTE_CNT_LO_NBI_DMA_DROP_BYTE_CNT_LO_of(_x) (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: NbiDmaDropByteCntHi - Nbi Dma Drop Byte Counter High 32-bits
    Bits: 31:0  NbiDmaDropByteCntHi(rc) - Upper 32-bits of 64-bit NBI DMA Drop Byte count. Read this register after reading the NbiDmaDropByteCntLo register
*/
#define NFP_NBI_DMAX_CSR_NBI_DMA_DROP_BYTE_CNT_HI            0x000000dc
#define   NFP_NBI_DMAX_CSR_NBI_DMA_DROP_BYTE_CNT_HI_NBI_DMA_DROP_BYTE_CNT_HI(_x) (((_x) & 0xffffffff) << 0)
#define   NFP_NBI_DMAX_CSR_NBI_DMA_DROP_BYTE_CNT_HI_NBI_DMA_DROP_BYTE_CNT_HI_of(_x) (((_x) >> 0) & 0xffffffff)

#else /* NFP6000_LONGNAMES */

/*----------------------------------------------------------------
  Register: NbiDmaBDSramEntry[4096] - NBI DMA Buffer Descriptor SRAM Entry X
    Bits: 28:0  MuAddr(rw) - Memory Unit Address of the buffer (internal/external MU). Maps to CPP Address[29;11] Refer to the NBI top-level Master Commands section for memory unit address translation.
*/
#define NFP_NBI_DMA_BDSRAM_NbiDmaBDSramEntry(_x)             (0x00000000 + (0x8 * ((_x) & 0xfff)))
#define   NFP_NBI_DMA_BDSRAM_NbiDmaBDSramEntry_MuAddr(_x)    (((_x) & 0x1fffffff) << 0)
#define   NFP_NBI_DMA_BDSRAM_NbiDmaBDSramEntry_MuAddr_of(_x) (((_x) >> 0) & 0x1fffffff)
/*----------------------------------------------------------------
  Register: BLQueCtrl0 - Configures the buffer list circular queue and provides status
    Bits: 55:48 BLQEventFullCnt(rc) - Debug counter that increments when a BLQ Event is dropped due to a full BLQ Event FIFO. This counter saturates at all 1's
    Bits: 42:40 BDFifoEntryCnt(ro) - Number of buffer descriptors resident in the buffer descriptor register FIFO
    Bits: 39    BLQFull(ro) - The Queue is full when set.
    Bits: 38    BLQEmpty(ro) - The queue is empty when set.
    Bits: 37:36 BLQSize(rw) - Defines the Size of the Circular Buffer List Queue
    Bits: 35:24 BLEntryCnt(ro) - Hardware calculated value of the number of entries on the Buffer List Queue. A full Buffer List Queue has an entry count equal to zero, with the BLQFull flag set. An empty Buffer List Queue has an entry count equal to zero with the BLQEmpty flag set.
    Bits: 23:12 BLHdPtr(rw) - Buffer List Queue Head Pointer. Points to the next entry to be read by hardware.
    Bits: 11:0  BLTlPtr(rw) - Buffer List Queue Tail Pointer. Points to the next entry to be written at the tail.
*/
#define NFP_NBI_DMA_BufferList_BLQueCtrl0                    0x00008000
#define   NFP_NBI_DMA_BufferList_BLQueCtrl0_BLQEventFullCnt(_x) (((_x) & 0xff) << 48)
#define   NFP_NBI_DMA_BufferList_BLQueCtrl0_BLQEventFullCnt_of(_x) (((_x) >> 48) & 0xff)
#define   NFP_NBI_DMA_BufferList_BLQueCtrl0_BDFifoEntryCnt(_x) (((_x) & 0x7) << 40)
#define   NFP_NBI_DMA_BufferList_BLQueCtrl0_BDFifoEntryCnt_of(_x) (((_x) >> 40) & 0x7)
#define   NFP_NBI_DMA_BufferList_BLQueCtrl0_BLQFull          BIT_ULL(39)
#define   NFP_NBI_DMA_BufferList_BLQueCtrl0_BLQEmpty         BIT_ULL(38)
#define   NFP_NBI_DMA_BufferList_BLQueCtrl0_BLQSize(_x)      (((_x) & 0x3) << 36)
#define   NFP_NBI_DMA_BufferList_BLQueCtrl0_BLQSize_of(_x)   (((_x) >> 36) & 0x3)
#define     NFP_NBI_DMA_BufferList_BLQueCtrl0_BLQSize_512    (0)
#define     NFP_NBI_DMA_BufferList_BLQueCtrl0_BLQSize_1K     (1)
#define     NFP_NBI_DMA_BufferList_BLQueCtrl0_BLQSize_2K     (2)
#define     NFP_NBI_DMA_BufferList_BLQueCtrl0_BLQSize_4K     (3)
#define   NFP_NBI_DMA_BufferList_BLQueCtrl0_BLEntryCnt(_x)   (((_x) & 0xfff) << 24)
#define   NFP_NBI_DMA_BufferList_BLQueCtrl0_BLEntryCnt_of(_x) (((_x) >> 24) & 0xfff)
#define   NFP_NBI_DMA_BufferList_BLQueCtrl0_BLHdPtr(_x)      (((_x) & 0xfff) << 12)
#define   NFP_NBI_DMA_BufferList_BLQueCtrl0_BLHdPtr_of(_x)   (((_x) >> 12) & 0xfff)
#define   NFP_NBI_DMA_BufferList_BLQueCtrl0_BLTlPtr(_x)      (((_x) & 0xfff) << 0)
#define   NFP_NBI_DMA_BufferList_BLQueCtrl0_BLTlPtr_of(_x)   (((_x) >> 0) & 0xfff)
/*----------------------------------------------------------------
  Register: BLQueCtrl1 - Configures the buffer list circular queue and provides status
    Bits: 55:48 BLQEventFullCnt(rc) - Debug counter that increments when a BLQ Event is dropped due to a full BLQ Event FIFO. This counter saturates at all 1's
    Bits: 42:40 BDFifoEntryCnt(ro) - Number of buffer descriptors resident in the buffer descriptor register FIFO
    Bits: 39    BLQFull(ro) - The Queue is full when set.
    Bits: 38    BLQEmpty(ro) - The queue is empty when set.
    Bits: 37:36 BLQSize(rw) - Defines the Size of the Circular Buffer List Queue
    Bits: 35:24 BLEntryCnt(ro) - Hardware calculated value of the number of entries on the Buffer List Queue. A full Buffer List Queue has an entry count equal to zero, with the BLQFull flag set. An empty Buffer List Queue has an entry count equal to zero with the BLQEmpty flag set.
    Bits: 23:12 BLHdPtr(rw) - Buffer List Queue Head Pointer. Points to the next entry to be read by hardware.
    Bits: 11:0  BLTlPtr(rw) - Buffer List Queue Tail Pointer. Points to the next entry to be written at the tail.
*/
#define NFP_NBI_DMA_BufferList_BLQueCtrl1                    0x00008008
#define   NFP_NBI_DMA_BufferList_BLQueCtrl1_BLQEventFullCnt(_x) (((_x) & 0xff) << 48)
#define   NFP_NBI_DMA_BufferList_BLQueCtrl1_BLQEventFullCnt_of(_x) (((_x) >> 48) & 0xff)
#define   NFP_NBI_DMA_BufferList_BLQueCtrl1_BDFifoEntryCnt(_x) (((_x) & 0x7) << 40)
#define   NFP_NBI_DMA_BufferList_BLQueCtrl1_BDFifoEntryCnt_of(_x) (((_x) >> 40) & 0x7)
#define   NFP_NBI_DMA_BufferList_BLQueCtrl1_BLQFull          BIT_ULL(39)
#define   NFP_NBI_DMA_BufferList_BLQueCtrl1_BLQEmpty         BIT_ULL(38)
#define   NFP_NBI_DMA_BufferList_BLQueCtrl1_BLQSize(_x)      (((_x) & 0x3) << 36)
#define   NFP_NBI_DMA_BufferList_BLQueCtrl1_BLQSize_of(_x)   (((_x) >> 36) & 0x3)
#define     NFP_NBI_DMA_BufferList_BLQueCtrl1_BLQSize_512    (0)
#define     NFP_NBI_DMA_BufferList_BLQueCtrl1_BLQSize_1K     (1)
#define     NFP_NBI_DMA_BufferList_BLQueCtrl1_BLQSize_2K     (2)
#define     NFP_NBI_DMA_BufferList_BLQueCtrl1_BLQSize_4K     (3)
#define   NFP_NBI_DMA_BufferList_BLQueCtrl1_BLEntryCnt(_x)   (((_x) & 0xfff) << 24)
#define   NFP_NBI_DMA_BufferList_BLQueCtrl1_BLEntryCnt_of(_x) (((_x) >> 24) & 0xfff)
#define   NFP_NBI_DMA_BufferList_BLQueCtrl1_BLHdPtr(_x)      (((_x) & 0xfff) << 12)
#define   NFP_NBI_DMA_BufferList_BLQueCtrl1_BLHdPtr_of(_x)   (((_x) >> 12) & 0xfff)
#define   NFP_NBI_DMA_BufferList_BLQueCtrl1_BLTlPtr(_x)      (((_x) & 0xfff) << 0)
#define   NFP_NBI_DMA_BufferList_BLQueCtrl1_BLTlPtr_of(_x)   (((_x) >> 0) & 0xfff)
/*----------------------------------------------------------------
  Register: BLQueCtrl2 - Configures the buffer list circular queue and provides status
    Bits: 55:48 BLQEventFullCnt(rc) - Debug counter that increments when a BLQ Event is dropped due to a full BLQ Event FIFO. This counter saturates at all 1's
    Bits: 42:40 BDFifoEntryCnt(ro) - Number of buffer descriptors resident in the buffer descriptor register FIFO
    Bits: 39    BLQFull(ro) - The Queue is full when set.
    Bits: 38    BLQEmpty(ro) - The queue is empty when set.
    Bits: 37:36 BLQSize(rw) - Defines the Size of the Circular Buffer List Queue
    Bits: 35:24 BLEntryCnt(ro) - Hardware calculated value of the number of entries on the Buffer List Queue. A full Buffer List Queue has an entry count equal to zero, with the BLQFull flag set. An empty Buffer List Queue has an entry count equal to zero with the BLQEmpty flag set.
    Bits: 23:12 BLHdPtr(rw) - Buffer List Queue Head Pointer. Points to the next entry to be read by hardware.
    Bits: 11:0  BLTlPtr(rw) - Buffer List Queue Tail Pointer. Points to the next entry to be written at the tail.
*/
#define NFP_NBI_DMA_BufferList_BLQueCtrl2                    0x00008010
#define   NFP_NBI_DMA_BufferList_BLQueCtrl2_BLQEventFullCnt(_x) (((_x) & 0xff) << 48)
#define   NFP_NBI_DMA_BufferList_BLQueCtrl2_BLQEventFullCnt_of(_x) (((_x) >> 48) & 0xff)
#define   NFP_NBI_DMA_BufferList_BLQueCtrl2_BDFifoEntryCnt(_x) (((_x) & 0x7) << 40)
#define   NFP_NBI_DMA_BufferList_BLQueCtrl2_BDFifoEntryCnt_of(_x) (((_x) >> 40) & 0x7)
#define   NFP_NBI_DMA_BufferList_BLQueCtrl2_BLQFull          BIT_ULL(39)
#define   NFP_NBI_DMA_BufferList_BLQueCtrl2_BLQEmpty         BIT_ULL(38)
#define   NFP_NBI_DMA_BufferList_BLQueCtrl2_BLQSize(_x)      (((_x) & 0x3) << 36)
#define   NFP_NBI_DMA_BufferList_BLQueCtrl2_BLQSize_of(_x)   (((_x) >> 36) & 0x3)
#define     NFP_NBI_DMA_BufferList_BLQueCtrl2_BLQSize_512    (0)
#define     NFP_NBI_DMA_BufferList_BLQueCtrl2_BLQSize_1K     (1)
#define     NFP_NBI_DMA_BufferList_BLQueCtrl2_BLQSize_2K     (2)
#define     NFP_NBI_DMA_BufferList_BLQueCtrl2_BLQSize_4K     (3)
#define   NFP_NBI_DMA_BufferList_BLQueCtrl2_BLEntryCnt(_x)   (((_x) & 0xfff) << 24)
#define   NFP_NBI_DMA_BufferList_BLQueCtrl2_BLEntryCnt_of(_x) (((_x) >> 24) & 0xfff)
#define   NFP_NBI_DMA_BufferList_BLQueCtrl2_BLHdPtr(_x)      (((_x) & 0xfff) << 12)
#define   NFP_NBI_DMA_BufferList_BLQueCtrl2_BLHdPtr_of(_x)   (((_x) >> 12) & 0xfff)
#define   NFP_NBI_DMA_BufferList_BLQueCtrl2_BLTlPtr(_x)      (((_x) & 0xfff) << 0)
#define   NFP_NBI_DMA_BufferList_BLQueCtrl2_BLTlPtr_of(_x)   (((_x) >> 0) & 0xfff)
/*----------------------------------------------------------------
  Register: BLQueCtrl3 - Configures the buffer list circular queue and provides status
    Bits: 55:48 BLQEventFullCnt(rc) - Debug counter that increments when a BLQ Event is dropped due to a full BLQ Event FIFO. This counter saturates at all 1's
    Bits: 42:40 BDFifoEntryCnt(ro) - Number of buffer descriptors resident in the buffer descriptor register FIFO
    Bits: 39    BLQFull(ro) - The Queue is full when set.
    Bits: 38    BLQEmpty(ro) - The queue is empty when set.
    Bits: 37:36 BLQSize(rw) - Defines the Size of the Circular Buffer List Queue
    Bits: 35:24 BLEntryCnt(ro) - Hardware calculated value of the number of entries on the Buffer List Queue. A full Buffer List Queue has an entry count equal to zero, with the BLQFull flag set. An empty Buffer List Queue has an entry count equal to zero with the BLQEmpty flag set.
    Bits: 23:12 BLHdPtr(rw) - Buffer List Queue Head Pointer. Points to the next entry to be read by hardware.
    Bits: 11:0  BLTlPtr(rw) - Buffer List Queue Tail Pointer. Points to the next entry to be written at the tail.
*/
#define NFP_NBI_DMA_BufferList_BLQueCtrl3                    0x00008018
#define   NFP_NBI_DMA_BufferList_BLQueCtrl3_BLQEventFullCnt(_x) (((_x) & 0xff) << 48)
#define   NFP_NBI_DMA_BufferList_BLQueCtrl3_BLQEventFullCnt_of(_x) (((_x) >> 48) & 0xff)
#define   NFP_NBI_DMA_BufferList_BLQueCtrl3_BDFifoEntryCnt(_x) (((_x) & 0x7) << 40)
#define   NFP_NBI_DMA_BufferList_BLQueCtrl3_BDFifoEntryCnt_of(_x) (((_x) >> 40) & 0x7)
#define   NFP_NBI_DMA_BufferList_BLQueCtrl3_BLQFull          BIT_ULL(39)
#define   NFP_NBI_DMA_BufferList_BLQueCtrl3_BLQEmpty         BIT_ULL(38)
#define   NFP_NBI_DMA_BufferList_BLQueCtrl3_BLQSize(_x)      (((_x) & 0x3) << 36)
#define   NFP_NBI_DMA_BufferList_BLQueCtrl3_BLQSize_of(_x)   (((_x) >> 36) & 0x3)
#define     NFP_NBI_DMA_BufferList_BLQueCtrl3_BLQSize_512    (0)
#define     NFP_NBI_DMA_BufferList_BLQueCtrl3_BLQSize_1K     (1)
#define     NFP_NBI_DMA_BufferList_BLQueCtrl3_BLQSize_2K     (2)
#define     NFP_NBI_DMA_BufferList_BLQueCtrl3_BLQSize_4K     (3)
#define   NFP_NBI_DMA_BufferList_BLQueCtrl3_BLEntryCnt(_x)   (((_x) & 0xfff) << 24)
#define   NFP_NBI_DMA_BufferList_BLQueCtrl3_BLEntryCnt_of(_x) (((_x) >> 24) & 0xfff)
#define   NFP_NBI_DMA_BufferList_BLQueCtrl3_BLHdPtr(_x)      (((_x) & 0xfff) << 12)
#define   NFP_NBI_DMA_BufferList_BLQueCtrl3_BLHdPtr_of(_x)   (((_x) >> 12) & 0xfff)
#define   NFP_NBI_DMA_BufferList_BLQueCtrl3_BLTlPtr(_x)      (((_x) & 0xfff) << 0)
#define   NFP_NBI_DMA_BufferList_BLQueCtrl3_BLTlPtr_of(_x)   (((_x) >> 0) & 0xfff)
/*----------------------------------------------------------------
  Register: NbiDmaBCSramEntry[1024] - NBI DMA Buffer Completion SRAM Entry X
    Bits: 31:24 bufNum3(rw) - Buffer number 3
    Bits: 23:16 bufNum2(rw) - Buffer number 2
    Bits: 15:8  bufNum1(rw) - Buffer number 1
    Bits:  7:0  bufNum0(rw) - Buffer number 0
*/
#define NFP_NBI_DMA_BCSRAM_NbiDmaBCSramEntry(_x)             (0x0000a000 + (0x8 * ((_x) & 0x3ff)))
#define   NFP_NBI_DMA_BCSRAM_NbiDmaBCSramEntry_bufNum3(_x)   (((_x) & 0xff) << 24)
#define   NFP_NBI_DMA_BCSRAM_NbiDmaBCSramEntry_bufNum3_of(_x) (((_x) >> 24) & 0xff)
#define   NFP_NBI_DMA_BCSRAM_NbiDmaBCSramEntry_bufNum2(_x)   (((_x) & 0xff) << 16)
#define   NFP_NBI_DMA_BCSRAM_NbiDmaBCSramEntry_bufNum2_of(_x) (((_x) >> 16) & 0xff)
#define   NFP_NBI_DMA_BCSRAM_NbiDmaBCSramEntry_bufNum1(_x)   (((_x) & 0xff) << 8)
#define   NFP_NBI_DMA_BCSRAM_NbiDmaBCSramEntry_bufNum1_of(_x) (((_x) >> 8) & 0xff)
#define   NFP_NBI_DMA_BCSRAM_NbiDmaBCSramEntry_bufNum0(_x)   (((_x) & 0xff) << 0)
#define   NFP_NBI_DMA_BCSRAM_NbiDmaBCSramEntry_bufNum0_of(_x) (((_x) >> 0) & 0xff)
/*----------------------------------------------------------------
  Register: NbiDmaCfg - NBI DMA Configuration register
    Bits: 20:17 DisPushBusSelect(rw) - Disables a push bus from being selected by the read arbiter when set. Used to force packet traffic to not use a particular push bus.
    Bits: 16    DisBcRamErr(rw) - Controls how the DMA engine handles multi-bit ECC errors received from the Buffer Completion RAM.
    Bits: 15    DisDsRamErr(rw) - Controls how the DMA engine handles multi-bit ECC errors received from the DMA State RAM.
    Bits: 14    DisBdRamErr(rw) - Controls how the BLQ logic handles multi-bit ECC errors received from the Buffer Descriptor RAM
    Bits: 13    DisBufRdErr(rw) - Controls how the hardware forwards the received NBI Buffer Memory Read Error to the output data buses.
    Bits: 12    DisRxPushLastErr(rw) - Controls how the hardware responds to a push bus input protocol error for allocation response transactions
    Bits: 11    DisRxAllocInErr(rw) - Controls how the hardware responds to a push bus input error for allocation response transactions.
    Bits: 10    DisRxBlqWrInErr(rw) - Controls how the hardware responds to a push bus input error for BLQ writes transactions.
    Bits:  9:7  NbiNum(rw) - This field is placed in the 3 lsbits of the Packet Allocation Command Address field. It is used by the CTM to identify which NBI sent the command.
    Bits:  6    CtmPollEna(rw) - Configures whether the polling function is enabled or disabled.
    Bits:  5:4  CtmPollIntvl(rw) - Configures the amount of time between packet allocation poll commands to a CTM. If CtmPollSearchEna = 0, the polling interval is defined by the CtmPollIntvl. If the CtmPollSearchEna = 1, the poll interval is defined by the following equation: (CtmPollIntvl/64)*(# of active CTMs).
    Bits:  1    RateLimitEnable(rw) - Selects whether to limit the bandwidth on the four Push Buses.
    Bits:  0    CtmPollSearchEna(rw) - Controls how CTM polling is performed. When set the CTM polling logic skips to the next active CTM every poll interval. When clear the CTM polling logic increments the CTM counter by one every poll interval.
*/
#define NFP_NBI_DMAX_CSR_NbiDmaCfg                           0x00000000
#define   NFP_NBI_DMAX_CSR_NbiDmaCfg_DisPushBusSelect(_x)    (((_x) & 0xf) << 17)
#define   NFP_NBI_DMAX_CSR_NbiDmaCfg_DisPushBusSelect_of(_x) (((_x) >> 17) & 0xf)
#define   NFP_NBI_DMAX_CSR_NbiDmaCfg_DisBcRamErr             BIT(16)
#define     NFP_NBI_DMAX_CSR_NbiDmaCfg_DisBcRamErr_Enable    (0 << 16)
#define     NFP_NBI_DMAX_CSR_NbiDmaCfg_DisBcRamErr_Disable   BIT(16)
#define   NFP_NBI_DMAX_CSR_NbiDmaCfg_DisDsRamErr             BIT(15)
#define     NFP_NBI_DMAX_CSR_NbiDmaCfg_DisDsRamErr_Enable    (0 << 15)
#define     NFP_NBI_DMAX_CSR_NbiDmaCfg_DisDsRamErr_Disable   BIT(15)
#define   NFP_NBI_DMAX_CSR_NbiDmaCfg_DisBdRamErr             BIT(14)
#define     NFP_NBI_DMAX_CSR_NbiDmaCfg_DisBdRamErr_Enable    (0 << 14)
#define     NFP_NBI_DMAX_CSR_NbiDmaCfg_DisBdRamErr_Disable   BIT(14)
#define   NFP_NBI_DMAX_CSR_NbiDmaCfg_DisBufRdErr             BIT(13)
#define     NFP_NBI_DMAX_CSR_NbiDmaCfg_DisBufRdErr_Enable    (0 << 13)
#define     NFP_NBI_DMAX_CSR_NbiDmaCfg_DisBufRdErr_Disable   BIT(13)
#define   NFP_NBI_DMAX_CSR_NbiDmaCfg_DisRxPushLastErr        BIT(12)
#define     NFP_NBI_DMAX_CSR_NbiDmaCfg_DisRxPushLastErr_Enable (0 << 12)
#define     NFP_NBI_DMAX_CSR_NbiDmaCfg_DisRxPushLastErr_Disable BIT(12)
#define   NFP_NBI_DMAX_CSR_NbiDmaCfg_DisRxAllocInErr         BIT(11)
#define     NFP_NBI_DMAX_CSR_NbiDmaCfg_DisRxAllocInErr_Enable (0 << 11)
#define     NFP_NBI_DMAX_CSR_NbiDmaCfg_DisRxAllocInErr_Disable BIT(11)
#define   NFP_NBI_DMAX_CSR_NbiDmaCfg_DisRxBlqWrInErr         BIT(10)
#define     NFP_NBI_DMAX_CSR_NbiDmaCfg_DisRxBlqWrInErr_Enable (0 << 10)
#define     NFP_NBI_DMAX_CSR_NbiDmaCfg_DisRxBlqWrInErr_Disable BIT(10)
#define   NFP_NBI_DMAX_CSR_NbiDmaCfg_NbiNum(_x)              (((_x) & 0x7) << 7)
#define   NFP_NBI_DMAX_CSR_NbiDmaCfg_NbiNum_of(_x)           (((_x) >> 7) & 0x7)
#define   NFP_NBI_DMAX_CSR_NbiDmaCfg_CtmPollEna              BIT(6)
#define     NFP_NBI_DMAX_CSR_NbiDmaCfg_CtmPollEna_Disable    (0 << 6)
#define     NFP_NBI_DMAX_CSR_NbiDmaCfg_CtmPollEna_Enable     BIT(6)
#define   NFP_NBI_DMAX_CSR_NbiDmaCfg_CtmPollIntvl(_x)        (((_x) & 0x3) << 4)
#define   NFP_NBI_DMAX_CSR_NbiDmaCfg_CtmPollIntvl_of(_x)     (((_x) >> 4) & 0x3)
#define     NFP_NBI_DMAX_CSR_NbiDmaCfg_CtmPollIntvl_64us     (0)
#define     NFP_NBI_DMAX_CSR_NbiDmaCfg_CtmPollIntvl_128us    (1)
#define     NFP_NBI_DMAX_CSR_NbiDmaCfg_CtmPollIntvl_256us    (2)
#define     NFP_NBI_DMAX_CSR_NbiDmaCfg_CtmPollIntvl_512us    (3)
#define   NFP_NBI_DMAX_CSR_NbiDmaCfg_RateLimitEnable         BIT(1)
#define     NFP_NBI_DMAX_CSR_NbiDmaCfg_RateLimitEnable_Disable (0 << 1)
#define     NFP_NBI_DMAX_CSR_NbiDmaCfg_RateLimitEnable_Enable BIT(1)
#define   NFP_NBI_DMAX_CSR_NbiDmaCfg_CtmPollSearchEna        BIT(0)
#define     NFP_NBI_DMAX_CSR_NbiDmaCfg_CtmPollSearchEna_Disable (0 << 0)
#define     NFP_NBI_DMAX_CSR_NbiDmaCfg_CtmPollSearchEna_Enable BIT(0)
/*----------------------------------------------------------------
  Register: NbiDmaSpare - General Purpose Register
    Bits: 31:0  NbiDmaSpare(rw) - General use register.
*/
#define NFP_NBI_DMAX_CSR_NbiDmaSpare                         0x00000004
#define   NFP_NBI_DMAX_CSR_NbiDmaSpare_NbiDmaSpare(_x)       (((_x) & 0xffffffff) << 0)
#define   NFP_NBI_DMAX_CSR_NbiDmaSpare_NbiDmaSpare_of(_x)    (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: NbiDmaBlqEvent - NBI DMA Buffer List Queue Event Control
    Bits: 20    DisBLQEventFifoFullAssert(rw) - Set this bit to disable the BLQ Event FIFO overflow assertion
    Bits: 19:18 BLQEventThreshold3(rw) - An event is generated every N buffer descriptors read from buffer list queue3, where N is configurable from 16 to 128.
    Bits: 17:16 BLQEventThreshold2(rw) - An event is generated every N buffer descriptors read from buffer list queue2, where N is configurable from 16 to 128.
    Bits: 15:14 BLQEventThreshold1(rw) - An event is generated every N buffer descriptors read from buffer list queue1, where N is configurable from 16 to 128.
    Bits: 13:12 BLQEventThreshold0(rw) - An event is generated every N buffer descriptors read from buffer list queue0, where N is configurable from 16 to 128.
    Bits:  7:4  BLQEventNotFullEnable(rw) - One bit per buffer list queue. Enables the generation of BLQ Events when the queue transitions from full to not full. Disabled when clear. The event type value used is 0x1 (event_type_fifo_not_full)
    Bits:  3:0  BLQEventStatusEnable(rw) - One bit per buffer list queue. Enables BLQ Status Events when set. Disabled when clear. A Status Event is sent when the number of entries read from the queue reaches the programmed value in the BLQEventThreshold register. The event type value used is 0x5 (event_type_value_update)
*/
#define NFP_NBI_DMAX_CSR_NbiDmaBlqEvent                      0x0000000c
#define   NFP_NBI_DMAX_CSR_NbiDmaBlqEvent_DisBLQEventFifoFullAssert BIT(20)
#define   NFP_NBI_DMAX_CSR_NbiDmaBlqEvent_BLQEventThreshold3(_x) (((_x) & 0x3) << 18)
#define   NFP_NBI_DMAX_CSR_NbiDmaBlqEvent_BLQEventThreshold3_of(_x) (((_x) >> 18) & 0x3)
#define     NFP_NBI_DMAX_CSR_NbiDmaBlqEvent_BLQEventThreshold3_16 (0)
#define     NFP_NBI_DMAX_CSR_NbiDmaBlqEvent_BLQEventThreshold3_32 (1)
#define     NFP_NBI_DMAX_CSR_NbiDmaBlqEvent_BLQEventThreshold3_64 (2)
#define     NFP_NBI_DMAX_CSR_NbiDmaBlqEvent_BLQEventThreshold3_128 (3)
#define   NFP_NBI_DMAX_CSR_NbiDmaBlqEvent_BLQEventThreshold2(_x) (((_x) & 0x3) << 16)
#define   NFP_NBI_DMAX_CSR_NbiDmaBlqEvent_BLQEventThreshold2_of(_x) (((_x) >> 16) & 0x3)
#define     NFP_NBI_DMAX_CSR_NbiDmaBlqEvent_BLQEventThreshold2_16 (0)
#define     NFP_NBI_DMAX_CSR_NbiDmaBlqEvent_BLQEventThreshold2_32 (1)
#define     NFP_NBI_DMAX_CSR_NbiDmaBlqEvent_BLQEventThreshold2_64 (2)
#define     NFP_NBI_DMAX_CSR_NbiDmaBlqEvent_BLQEventThreshold2_128 (3)
#define   NFP_NBI_DMAX_CSR_NbiDmaBlqEvent_BLQEventThreshold1(_x) (((_x) & 0x3) << 14)
#define   NFP_NBI_DMAX_CSR_NbiDmaBlqEvent_BLQEventThreshold1_of(_x) (((_x) >> 14) & 0x3)
#define     NFP_NBI_DMAX_CSR_NbiDmaBlqEvent_BLQEventThreshold1_16 (0)
#define     NFP_NBI_DMAX_CSR_NbiDmaBlqEvent_BLQEventThreshold1_32 (1)
#define     NFP_NBI_DMAX_CSR_NbiDmaBlqEvent_BLQEventThreshold1_64 (2)
#define     NFP_NBI_DMAX_CSR_NbiDmaBlqEvent_BLQEventThreshold1_128 (3)
#define   NFP_NBI_DMAX_CSR_NbiDmaBlqEvent_BLQEventThreshold0(_x) (((_x) & 0x3) << 12)
#define   NFP_NBI_DMAX_CSR_NbiDmaBlqEvent_BLQEventThreshold0_of(_x) (((_x) >> 12) & 0x3)
#define     NFP_NBI_DMAX_CSR_NbiDmaBlqEvent_BLQEventThreshold0_16 (0)
#define     NFP_NBI_DMAX_CSR_NbiDmaBlqEvent_BLQEventThreshold0_32 (1)
#define     NFP_NBI_DMAX_CSR_NbiDmaBlqEvent_BLQEventThreshold0_64 (2)
#define     NFP_NBI_DMAX_CSR_NbiDmaBlqEvent_BLQEventThreshold0_128 (3)
#define   NFP_NBI_DMAX_CSR_NbiDmaBlqEvent_BLQEventNotFullEnable(_x) (((_x) & 0xf) << 4)
#define   NFP_NBI_DMAX_CSR_NbiDmaBlqEvent_BLQEventNotFullEnable_of(_x) (((_x) >> 4) & 0xf)
#define     NFP_NBI_DMAX_CSR_NbiDmaBlqEvent_BLQEventNotFullEnable_Disable (0)
#define     NFP_NBI_DMAX_CSR_NbiDmaBlqEvent_BLQEventNotFullEnable_Enable (1)
#define   NFP_NBI_DMAX_CSR_NbiDmaBlqEvent_BLQEventStatusEnable(_x) (((_x) & 0xf) << 0)
#define   NFP_NBI_DMAX_CSR_NbiDmaBlqEvent_BLQEventStatusEnable_of(_x) (((_x) >> 0) & 0xf)
#define     NFP_NBI_DMAX_CSR_NbiDmaBlqEvent_BLQEventStatusEnable_Disable (0)
#define     NFP_NBI_DMAX_CSR_NbiDmaBlqEvent_BLQEventStatusEnable_Enable (1)
/*----------------------------------------------------------------
  Register: NbiDmaRate - NBI DMA Token Refresh Rate Register
    Bits:  7:6  CreditRate3(rw) - Sets the credit rate for push bus 3. Values are in 4 cycle increments.
    Bits:  5:4  CreditRate2(rw) - Sets the credit rate for push bus 2. Values are in 4 cycle increments.
    Bits:  3:2  CreditRate1(rw) - Sets the credit rate for push bus 1. Values are in 4 cycle increments.
    Bits:  1:0  CreditRate0(rw) - Sets the credit rate for push bus 0. Values are in 4 cycle increments.
*/
#define NFP_NBI_DMAX_CSR_NbiDmaRate                          0x00000010
#define   NFP_NBI_DMAX_CSR_NbiDmaRate_CreditRate3(_x)        (((_x) & 0x3) << 6)
#define   NFP_NBI_DMAX_CSR_NbiDmaRate_CreditRate3_of(_x)     (((_x) >> 6) & 0x3)
#define     NFP_NBI_DMAX_CSR_NbiDmaRate_CreditRate3_4        (0)
#define     NFP_NBI_DMAX_CSR_NbiDmaRate_CreditRate3_8        (1)
#define     NFP_NBI_DMAX_CSR_NbiDmaRate_CreditRate3_12       (2)
#define     NFP_NBI_DMAX_CSR_NbiDmaRate_CreditRate3_16       (3)
#define   NFP_NBI_DMAX_CSR_NbiDmaRate_CreditRate2(_x)        (((_x) & 0x3) << 4)
#define   NFP_NBI_DMAX_CSR_NbiDmaRate_CreditRate2_of(_x)     (((_x) >> 4) & 0x3)
#define     NFP_NBI_DMAX_CSR_NbiDmaRate_CreditRate2_4        (0)
#define     NFP_NBI_DMAX_CSR_NbiDmaRate_CreditRate2_8        (1)
#define     NFP_NBI_DMAX_CSR_NbiDmaRate_CreditRate2_12       (2)
#define     NFP_NBI_DMAX_CSR_NbiDmaRate_CreditRate2_16       (3)
#define   NFP_NBI_DMAX_CSR_NbiDmaRate_CreditRate1(_x)        (((_x) & 0x3) << 2)
#define   NFP_NBI_DMAX_CSR_NbiDmaRate_CreditRate1_of(_x)     (((_x) >> 2) & 0x3)
#define     NFP_NBI_DMAX_CSR_NbiDmaRate_CreditRate1_4        (0)
#define     NFP_NBI_DMAX_CSR_NbiDmaRate_CreditRate1_8        (1)
#define     NFP_NBI_DMAX_CSR_NbiDmaRate_CreditRate1_12       (2)
#define     NFP_NBI_DMAX_CSR_NbiDmaRate_CreditRate1_16       (3)
#define   NFP_NBI_DMAX_CSR_NbiDmaRate_CreditRate0(_x)        (((_x) & 0x3) << 0)
#define   NFP_NBI_DMAX_CSR_NbiDmaRate_CreditRate0_of(_x)     (((_x) >> 0) & 0x3)
#define     NFP_NBI_DMAX_CSR_NbiDmaRate_CreditRate0_4        (0)
#define     NFP_NBI_DMAX_CSR_NbiDmaRate_CreditRate0_8        (1)
#define     NFP_NBI_DMAX_CSR_NbiDmaRate_CreditRate0_12       (2)
#define     NFP_NBI_DMAX_CSR_NbiDmaRate_CreditRate0_16       (3)
/*----------------------------------------------------------------
  Register: NbiDmaCredit - NBI DMA Token Credit Registers
    Bits: 15:14 Threshold3(rw) - Sets the threshold that the leaky bucket is closed. Values are in increments of 16 Credits. A value of 0 equates to 16 credits or 128 bytes. A value of 3 equates to 64 credits or 512 bytes.
    Bits: 13:12 Threshold2(rw) - Sets the threshold that the leaky bucket is closed. Values are in increments of 16 Credits. A value of 0 equates to 16 credits or 128 bytes. A value of 3 equates to 64 credits or 512 bytes.
    Bits: 11:10 Threshold1(rw) - Sets the threshold that the leaky bucket is closed. Values are in increments of 16 Credits. A value of 0 equates to 16 credits or 128 bytes. A value of 3 equates to 64 credits or 512 bytes.
    Bits:  9:8  Threshold0(rw) - Sets the threshold that the leaky bucket is closed. Values are in increments of 16 Credits. A value of 0 equates to 16 credits or 128 bytes. A value of 3 equates to 64 credits or 512 bytes.
    Bits:  7:6  Credit3(rw) - Sets the number of credits to subtract from the leaky bucket for push bus 3. A credit equates to 8-bytes or 1 cycle on the bus. A value of zero equates to 1 credits. A value of 3 equates to 4 credits.
    Bits:  5:4  Credit2(rw) - Sets the number of credits to subtract from the leaky bucket for push bus 2. A credit equates to 8-bytes or 1 cycle on the bus. A value of zero equates to 1 credits. A value of 3 equates to 4 credits.
    Bits:  3:2  Credit1(rw) - Sets the number of credits to subtract from the leaky bucket for push bus 1. A credit equates to 8-bytes or 1 cycle on the bus. A value of zero equates to 1 credits. A value of 3 equates to 4 credits.
    Bits:  1:0  Credit0(rw) - Sets the number of credits to subtract from the leaky bucket for push bus 0. A credit equates to 8-bytes or 1 cycle on the bus. A value of zero equates to 1 credits. A value of 3 equates to 4 credits.
*/
#define NFP_NBI_DMAX_CSR_NbiDmaCredit                        0x00000014
#define   NFP_NBI_DMAX_CSR_NbiDmaCredit_Threshold3(_x)       (((_x) & 0x3) << 14)
#define   NFP_NBI_DMAX_CSR_NbiDmaCredit_Threshold3_of(_x)    (((_x) >> 14) & 0x3)
#define   NFP_NBI_DMAX_CSR_NbiDmaCredit_Threshold2(_x)       (((_x) & 0x3) << 12)
#define   NFP_NBI_DMAX_CSR_NbiDmaCredit_Threshold2_of(_x)    (((_x) >> 12) & 0x3)
#define   NFP_NBI_DMAX_CSR_NbiDmaCredit_Threshold1(_x)       (((_x) & 0x3) << 10)
#define   NFP_NBI_DMAX_CSR_NbiDmaCredit_Threshold1_of(_x)    (((_x) >> 10) & 0x3)
#define   NFP_NBI_DMAX_CSR_NbiDmaCredit_Threshold0(_x)       (((_x) & 0x3) << 8)
#define   NFP_NBI_DMAX_CSR_NbiDmaCredit_Threshold0_of(_x)    (((_x) >> 8) & 0x3)
#define   NFP_NBI_DMAX_CSR_NbiDmaCredit_Credit3(_x)          (((_x) & 0x3) << 6)
#define   NFP_NBI_DMAX_CSR_NbiDmaCredit_Credit3_of(_x)       (((_x) >> 6) & 0x3)
#define   NFP_NBI_DMAX_CSR_NbiDmaCredit_Credit2(_x)          (((_x) & 0x3) << 4)
#define   NFP_NBI_DMAX_CSR_NbiDmaCredit_Credit2_of(_x)       (((_x) >> 4) & 0x3)
#define   NFP_NBI_DMAX_CSR_NbiDmaCredit_Credit1(_x)          (((_x) & 0x3) << 2)
#define   NFP_NBI_DMAX_CSR_NbiDmaCredit_Credit1_of(_x)       (((_x) >> 2) & 0x3)
#define   NFP_NBI_DMAX_CSR_NbiDmaCredit_Credit0(_x)          (((_x) & 0x3) << 0)
#define   NFP_NBI_DMAX_CSR_NbiDmaCredit_Credit0_of(_x)       (((_x) >> 0) & 0x3)
/*----------------------------------------------------------------
  Register: NbiDmaBpeChainEnd - NBI DMA BPE Chain End Register
    Bits: 31:0  BpeChainEnd(rw) - Configures BPE module 'n' as the end of the buffer pool entry chain, where 'n' = 0 to 31.
*/
#define NFP_NBI_DMAX_CSR_NbiDmaBpeChainEnd                   0x00000018
#define   NFP_NBI_DMAX_CSR_NbiDmaBpeChainEnd_BpeChainEnd(_x) (((_x) & 0xffffffff) << 0)
#define   NFP_NBI_DMAX_CSR_NbiDmaBpeChainEnd_BpeChainEnd_of(_x) (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: NbiDmaBP[8]Cfg - NBI DMA Buffer Pool X Configuration
    Bits: 13    DropEn(rw) - Enables a packet to be dropped when buffer or packet credits run out or there are no MU buffers left on the selected buffer list. When cleared packets are not dropped and the logic backs up waiting for credits to arrive.
    Bits: 12    CtmOffset(rw) - Defines the offset to add to the data reference when starting to write packet data to the CTM. The space in front of the packet is for the rewrite script used by the packet modifier.
    Bits: 11:10 PriBList(rw) - Primary Buffer list number associated. There are 4 buffer lists available, which contain buffer descriptors that are used to store packet data in internal/external MUs.
    Bits:  9:8  SecBList(rw) - Secondary Buffer list number associated. There are 4 buffer lists available, which contain buffer descriptors that are used to store packet data in internal/external MUs.
    Bits:  6:5  SplitLength(rw) - Encoded packet size used when deciding to split packets between CTM and internal/external MUs. The encoding is in powers of 2 starting at 256 bytes.
    Bits:  4:0  BpeHead(rw) - Pointer to the first Buffer Pool Entry register in the list associated with this buffer pool.
*/
#define NFP_NBI_DMAX_CSR_NbiDmaBPCfg(_x)                     (0x00000020 + (0x4 * ((_x) & 0x7)))
#define   NFP_NBI_DMAX_CSR_NbiDmaBPCfg_DropEn                BIT(13)
#define     NFP_NBI_DMAX_CSR_NbiDmaBPCfg_DropEn_NoDrop       (0 << 13)
#define     NFP_NBI_DMAX_CSR_NbiDmaBPCfg_DropEn_Drop         BIT(13)
#define   NFP_NBI_DMAX_CSR_NbiDmaBPCfg_CtmOffset             BIT(12)
#define     NFP_NBI_DMAX_CSR_NbiDmaBPCfg_CtmOffset_128       (0 << 12)
#define     NFP_NBI_DMAX_CSR_NbiDmaBPCfg_CtmOffset_64        BIT(12)
#define   NFP_NBI_DMAX_CSR_NbiDmaBPCfg_PriBList(_x)          (((_x) & 0x3) << 10)
#define   NFP_NBI_DMAX_CSR_NbiDmaBPCfg_PriBList_of(_x)       (((_x) >> 10) & 0x3)
#define   NFP_NBI_DMAX_CSR_NbiDmaBPCfg_SecBList(_x)          (((_x) & 0x3) << 8)
#define   NFP_NBI_DMAX_CSR_NbiDmaBPCfg_SecBList_of(_x)       (((_x) >> 8) & 0x3)
#define   NFP_NBI_DMAX_CSR_NbiDmaBPCfg_SplitLength(_x)       (((_x) & 0x3) << 5)
#define   NFP_NBI_DMAX_CSR_NbiDmaBPCfg_SplitLength_of(_x)    (((_x) >> 5) & 0x3)
#define     NFP_NBI_DMAX_CSR_NbiDmaBPCfg_SplitLength_256     (0)
#define     NFP_NBI_DMAX_CSR_NbiDmaBPCfg_SplitLength_512     (1)
#define     NFP_NBI_DMAX_CSR_NbiDmaBPCfg_SplitLength_1K      (2)
#define     NFP_NBI_DMAX_CSR_NbiDmaBPCfg_SplitLength_2K      (3)
#define   NFP_NBI_DMAX_CSR_NbiDmaBPCfg_BpeHead(_x)           (((_x) & 0x1f) << 0)
#define   NFP_NBI_DMAX_CSR_NbiDmaBPCfg_BpeHead_of(_x)        (((_x) >> 0) & 0x1f)
/*----------------------------------------------------------------
  Register: NbiDmaBpe[32]Cfg - NBI DMA Buffer Pool Entry X Configuration
    Bits: 31:27 BpeNum(rwl) - Buffer Pool Entry Number. When the register is written this value is the latched version of the buffer pool register number 0-31. Used for debug purposes.
    Bits: 26:21 Ctm(rw) - CTM number to use
    Bits: 20:10 PktCredit(rw) - Configures the number of packets allocated to the CTM
    Bits:  9:0  BufCredit(rw) - Configures the number of buffer credits in 2K byte increments associated with the CTM. The msbit is the sign bit and must be cleared during configuration.
*/
#define NFP_NBI_DMAX_CSR_NbiDmaBpeCfg(_x)                    (0x00000040 + (0x4 * ((_x) & 0x1f)))
#define   NFP_NBI_DMAX_CSR_NbiDmaBpeCfg_BpeNum(_x)           (((_x) & 0x1f) << 27)
#define   NFP_NBI_DMAX_CSR_NbiDmaBpeCfg_BpeNum_of(_x)        (((_x) >> 27) & 0x1f)
#define   NFP_NBI_DMAX_CSR_NbiDmaBpeCfg_Ctm(_x)              (((_x) & 0x3f) << 21)
#define   NFP_NBI_DMAX_CSR_NbiDmaBpeCfg_Ctm_of(_x)           (((_x) >> 21) & 0x3f)
#define   NFP_NBI_DMAX_CSR_NbiDmaBpeCfg_PktCredit(_x)        (((_x) & 0x7ff) << 10)
#define   NFP_NBI_DMAX_CSR_NbiDmaBpeCfg_PktCredit_of(_x)     (((_x) >> 10) & 0x7ff)
#define   NFP_NBI_DMAX_CSR_NbiDmaBpeCfg_BufCredit(_x)        (((_x) & 0x3ff) << 0)
#define   NFP_NBI_DMAX_CSR_NbiDmaBpeCfg_BufCredit_of(_x)     (((_x) >> 0) & 0x3ff)
/*----------------------------------------------------------------
  Register: NbiDmaPktCntLo - Nbi Dma Pkt Counter Low 32-bits
    Bits: 31:0  NbiDmaPktCntLo(rc) - Lower 32-bits of 48-bit NBI DMA Tx Packet count. Reading this register latches the value in NbiDmaPktCntHi register
*/
#define NFP_NBI_DMAX_CSR_NbiDmaPktCntLo                      0x000000c0
#define   NFP_NBI_DMAX_CSR_NbiDmaPktCntLo_NbiDmaPktCntLo(_x) (((_x) & 0xffffffff) << 0)
#define   NFP_NBI_DMAX_CSR_NbiDmaPktCntLo_NbiDmaPktCntLo_of(_x) (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: NbiDmaPktCntHi - Nbi Dma Pkt Counter High 16-bits
    Bits: 15:0  NbiDmaPktCntHi(rc) - Upper 16-bits of 48-bit NBI Tx Packet count. Read this register after reading the NbiDmaPktCntLo register
*/
#define NFP_NBI_DMAX_CSR_NbiDmaPktCntHi                      0x000000c4
#define   NFP_NBI_DMAX_CSR_NbiDmaPktCntHi_NbiDmaPktCntHi(_x) (((_x) & 0xffff) << 0)
#define   NFP_NBI_DMAX_CSR_NbiDmaPktCntHi_NbiDmaPktCntHi_of(_x) (((_x) >> 0) & 0xffff)
/*----------------------------------------------------------------
  Register: NbiDmaByteCntLo - Nbi Dma Byte Counter Low 32-bits
    Bits: 31:0  NbiDmaByteCntLo(rc) - Lower 32-bits of 64-bit NBI DMA Tx Byte count. Reading this register latches the value in NbiDmaByteCntHi register
*/
#define NFP_NBI_DMAX_CSR_NbiDmaByteCntLo                     0x000000c8
#define   NFP_NBI_DMAX_CSR_NbiDmaByteCntLo_NbiDmaByteCntLo(_x) (((_x) & 0xffffffff) << 0)
#define   NFP_NBI_DMAX_CSR_NbiDmaByteCntLo_NbiDmaByteCntLo_of(_x) (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: NbiDmaByteCntHi - Nbi Dma Byte Counter High 32-bits
    Bits: 31:0  NbiDmaByteCntHi(rc) - Upper 32-bits of 64-bit NBI DMA Tx Byte count. Read this register after reading the NbiDmaByteCntLo register
*/
#define NFP_NBI_DMAX_CSR_NbiDmaByteCntHi                     0x000000cc
#define   NFP_NBI_DMAX_CSR_NbiDmaByteCntHi_NbiDmaByteCntHi(_x) (((_x) & 0xffffffff) << 0)
#define   NFP_NBI_DMAX_CSR_NbiDmaByteCntHi_NbiDmaByteCntHi_of(_x) (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: NbiDmaDropPktCntLo - Nbi Dma Drop Pkt Counter Low 32-bits
    Bits: 31:0  NbiDmaDropPktCntLo(rc) - Lower 32-bits of 48-bit NBI DMA Drop Packet count. Reading this register latches the value in NbiDmaDropPktCntHi register
*/
#define NFP_NBI_DMAX_CSR_NbiDmaDropPktCntLo                  0x000000d0
#define   NFP_NBI_DMAX_CSR_NbiDmaDropPktCntLo_NbiDmaDropPktCntLo(_x) (((_x) & 0xffffffff) << 0)
#define   NFP_NBI_DMAX_CSR_NbiDmaDropPktCntLo_NbiDmaDropPktCntLo_of(_x) (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: NbiDmaDropPktCntHi - Nbi Dma Drop Pkt Counter High 16-bits
    Bits: 15:0  NbiDmaDropPktCntHi(rc) - Upper 16-bits of 48-bit NBI Drop Packet count. Read this register after reading the NbiDmaDropPktCntLo register
*/
#define NFP_NBI_DMAX_CSR_NbiDmaDropPktCntHi                  0x000000d4
#define   NFP_NBI_DMAX_CSR_NbiDmaDropPktCntHi_NbiDmaDropPktCntHi(_x) (((_x) & 0xffff) << 0)
#define   NFP_NBI_DMAX_CSR_NbiDmaDropPktCntHi_NbiDmaDropPktCntHi_of(_x) (((_x) >> 0) & 0xffff)
/*----------------------------------------------------------------
  Register: NbiDmaDropByteCntLo - Nbi Dma Drop Byte Counter Low 32-bits
    Bits: 31:0  NbiDmaDropByteCntLo(rc) - Lower 32-bits of 64-bit NBI DMA Drop Byte count. Reading this register latches the value in NbiDmaDropByteCntHi register
*/
#define NFP_NBI_DMAX_CSR_NbiDmaDropByteCntLo                 0x000000d8
#define   NFP_NBI_DMAX_CSR_NbiDmaDropByteCntLo_NbiDmaDropByteCntLo(_x) (((_x) & 0xffffffff) << 0)
#define   NFP_NBI_DMAX_CSR_NbiDmaDropByteCntLo_NbiDmaDropByteCntLo_of(_x) (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: NbiDmaDropByteCntHi - Nbi Dma Drop Byte Counter High 32-bits
    Bits: 31:0  NbiDmaDropByteCntHi(rc) - Upper 32-bits of 64-bit NBI DMA Drop Byte count. Read this register after reading the NbiDmaDropByteCntLo register
*/
#define NFP_NBI_DMAX_CSR_NbiDmaDropByteCntHi                 0x000000dc
#define   NFP_NBI_DMAX_CSR_NbiDmaDropByteCntHi_NbiDmaDropByteCntHi(_x) (((_x) & 0xffffffff) << 0)
#define   NFP_NBI_DMAX_CSR_NbiDmaDropByteCntHi_NbiDmaDropByteCntHi_of(_x) (((_x) >> 0) & 0xffffffff)

#endif /* NFP6000_LONGNAMES */


#endif /* NFP6000_NBI_DMA_H */
