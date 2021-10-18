/*
 * Copyright (C) 2013, Netronome Systems, Inc.  All rights reserved.
 *
 * Disclaimer: this file is provided without any warranty
 * as part of an early-access program, and the content is
 * bound to change before the final release.
 */

#ifndef NFP3800_NBI_DMA_H
#define NFP3800_NBI_DMA_H

#include "nfp_nbi.h"

/*----------------------------------------------------------------*/
/* CPP Address Map (CPP_BASE = IMB translation: island + indexes) */

/* CPP_BASE + NFP3800_NBI_DMA + NFP3800_NBI_DMA_* */
#define NFP3800_NBI_DMA                                          (0x000000)

/*----------------------------------------------------------------*/
/* XPB Address Map (XPB_BASE = NFP3800_XPB_ISLAND(island))          */

#define NFP3800_NBI_DMAX                                         (NBIX_BASE + 0x000000)

/* XPB_BASE + NFP3800_NBI_DMAX_CSR + NFP3800_NBI_DMAX_CSR_* */
#define NFP3800_NBI_DMAX_CSR                                     (NFP3800_NBI_DMAX + 0x00000)
/* XPB_BASE + NFP3800_NBI_DMAX_BD_ECC + (nfp_ecc.h) */
#define NFP3800_NBI_DMAX_BD_ECC                                  (NFP3800_NBI_DMAX + 0x10000)
/* XPB_BASE + NFP3800_NBI_DMAX_BC_ECC + (nfp_ecc.h) */
#define NFP3800_NBI_DMAX_BC_ECC                                  (NFP3800_NBI_DMAX + 0x20000)
/* XPB_BASE + NFP3800_NBI_DMAX_DS_ECC + (nfp_ecc.h) */
#define NFP3800_NBI_DMAX_DS_ECC                                  (NFP3800_NBI_DMAX + 0x30000)

/*================================================================*/


/* HGID: c3e5162fe280 */

#ifndef NFP3800_LONGNAMES

/*----------------------------------------------------------------
  Register: NbiDmaBDSramEntry[2048] - NBI DMA Buffer Descriptor SRAM Entry X
    Bits: 28:0  MuAddr(rw) - Memory Unit Address of the buffer (internal/external MU). Maps to CPP Address[29;11] Refer to the NBI top-level Master Commands section for memory unit address translation.
*/
#define NFP3800_NBI_DMA_BDSRAM_NBIDMA_BDSRAM_ENTRY(_x)       (0x00000000 + (0x8 * ((_x) & 0x7ff)))
#define   NFP3800_NBI_DMA_BDSRAM_NBIDMA_BDSRAM_ENTRY_MUADDR(_x) (((_x) & 0x1fffffff) << 0)
#define   NFP3800_NBI_DMA_BDSRAM_NBIDMA_BDSRAM_ENTRY_MUADDR_of(_x) (((_x) >> 0) & 0x1fffffff)
/*----------------------------------------------------------------
  Register: BLQueCtrl0 - Configures the buffer list circular queue and provides status
    Bits: 55:48 BLQEventFullCnt(rc) - Debug counter that increments when a BLQ Event is dropped due to a full BLQ Event FIFO. This counter saturates at all 1's
    Bits: 42:40 BDFifoEntryCnt(ro) - Number of buffer descriptors resident in the buffer descriptor register FIFO
    Bits: 39    BLQFull(ro) - The Queue is full when set.
    Bits: 38    BLQEmpty(ro) - The queue is empty when set.
    Bits: 37:36 BLQSize(rw) - Defines the Size of the Circular Buffer List Queue
    Bits: 34:24 BLEntryCnt(ro) - Hardware calculated value of the number of entries on the Buffer List Queue. A full Buffer List Queue has an entry count equal to zero, with the BLQFull flag set. An empty Buffer List Queue has an entry count equal to zero with the BLQEmpty flag set.
    Bits: 22:12 BLHdPtr(rw) - Buffer List Queue Head Pointer. Points to the next entry to be read by hardware.
    Bits: 10:0  BLTlPtr(rw) - Buffer List Queue Tail Pointer. Points to the next entry to be written at the tail.
*/
#define NFP3800_NBI_DMA_BufferList_BL_QUE_CTRL0              0x00008000
#define   NFP3800_NBI_DMA_BUFFERLIST_BL_QUE_CTRL0_BLQ_EVENT_FULL_CNT(_x) (((_x) & 0xff) << 48)
#define   NFP3800_NBI_DMA_BUFFERLIST_BL_QUE_CTRL0_BLQ_EVENT_FULL_CNT_of(_x) (((_x) >> 48) & 0xff)
#define   NFP3800_NBI_DMA_BUFFERLIST_BL_QUE_CTRL0_BD_FIFO_ENTRY_CNT(_x) (((_x) & 0x7) << 40)
#define   NFP3800_NBI_DMA_BUFFERLIST_BL_QUE_CTRL0_BD_FIFO_ENTRY_CNT_of(_x) (((_x) >> 40) & 0x7)
#define   NFP3800_NBI_DMA_BUFFERLIST_BL_QUE_CTRL0_BL_QUE_FULL BIT_ULL(39)
#define   NFP3800_NBI_DMA_BUFFERLIST_BL_QUE_CTRL0_BL_QUE_EMPTY BIT_ULL(38)
#define   NFP3800_NBI_DMA_BUFFERLIST_BL_QUE_CTRL0_BL_QUE_SIZE(_x) (((_x) & 0x3) << 36)
#define   NFP3800_NBI_DMA_BUFFERLIST_BL_QUE_CTRL0_BL_QUE_SIZE_of(_x) (((_x) >> 36) & 0x3)
#define     NFP3800_NBI_DMA_BUFFERLIST_BL_QUE_CTRL0_BL_QUE_SIZE_256 (0)
#define     NFP3800_NBI_DMA_BUFFERLIST_BL_QUE_CTRL0_BL_QUE_SIZE_512 (1)
#define     NFP3800_NBI_DMA_BUFFERLIST_BL_QUE_CTRL0_BL_QUE_SIZE_1K (2)
#define     NFP3800_NBI_DMA_BUFFERLIST_BL_QUE_CTRL0_BL_QUE_SIZE_2K (3)
#define   NFP3800_NBI_DMA_BUFFERLIST_BL_QUE_CTRL0_BL_ENTRY_CNT(_x) (((_x) & 0x7ff) << 24)
#define   NFP3800_NBI_DMA_BUFFERLIST_BL_QUE_CTRL0_BL_ENTRY_CNT_of(_x) (((_x) >> 24) & 0x7ff)
#define   NFP3800_NBI_DMA_BUFFERLIST_BL_QUE_CTRL0_BL_HEAD_PTR(_x) (((_x) & 0x7ff) << 12)
#define   NFP3800_NBI_DMA_BUFFERLIST_BL_QUE_CTRL0_BL_HEAD_PTR_of(_x) (((_x) >> 12) & 0x7ff)
#define   NFP3800_NBI_DMA_BUFFERLIST_BL_QUE_CTRL0_BL_TAIL_PTR(_x) (((_x) & 0x7ff) << 0)
#define   NFP3800_NBI_DMA_BUFFERLIST_BL_QUE_CTRL0_BL_TAIL_PTR_of(_x) (((_x) >> 0) & 0x7ff)
/*----------------------------------------------------------------
  Register: BLQueCtrl1 - Configures the buffer list circular queue and provides status
    Bits: 55:48 BLQEventFullCnt(rc) - Debug counter that increments when a BLQ Event is dropped due to a full BLQ Event FIFO. This counter saturates at all 1's
    Bits: 42:40 BDFifoEntryCnt(ro) - Number of buffer descriptors resident in the buffer descriptor register FIFO
    Bits: 39    BLQFull(ro) - The Queue is full when set.
    Bits: 38    BLQEmpty(ro) - The queue is empty when set.
    Bits: 37:36 BLQSize(rw) - Defines the Size of the Circular Buffer List Queue
    Bits: 34:24 BLEntryCnt(ro) - Hardware calculated value of the number of entries on the Buffer List Queue. A full Buffer List Queue has an entry count equal to zero, with the BLQFull flag set. An empty Buffer List Queue has an entry count equal to zero with the BLQEmpty flag set.
    Bits: 22:12 BLHdPtr(rw) - Buffer List Queue Head Pointer. Points to the next entry to be read by hardware.
    Bits: 10:0  BLTlPtr(rw) - Buffer List Queue Tail Pointer. Points to the next entry to be written at the tail.
*/
#define NFP3800_NBI_DMA_BufferList_BL_QUE_CTRL1              0x00008008
#define   NFP3800_NBI_DMA_BUFFERLIST_BL_QUE_CTRL1_BLQ_EVENT_FULL_CNT(_x) (((_x) & 0xff) << 48)
#define   NFP3800_NBI_DMA_BUFFERLIST_BL_QUE_CTRL1_BLQ_EVENT_FULL_CNT_of(_x) (((_x) >> 48) & 0xff)
#define   NFP3800_NBI_DMA_BUFFERLIST_BL_QUE_CTRL1_BD_FIFO_ENTRY_CNT(_x) (((_x) & 0x7) << 40)
#define   NFP3800_NBI_DMA_BUFFERLIST_BL_QUE_CTRL1_BD_FIFO_ENTRY_CNT_of(_x) (((_x) >> 40) & 0x7)
#define   NFP3800_NBI_DMA_BUFFERLIST_BL_QUE_CTRL1_BL_QUE_FULL BIT_ULL(39)
#define   NFP3800_NBI_DMA_BUFFERLIST_BL_QUE_CTRL1_BL_QUE_EMPTY BIT_ULL(38)
#define   NFP3800_NBI_DMA_BUFFERLIST_BL_QUE_CTRL1_BL_QUE_SIZE(_x) (((_x) & 0x3) << 36)
#define   NFP3800_NBI_DMA_BUFFERLIST_BL_QUE_CTRL1_BL_QUE_SIZE_of(_x) (((_x) >> 36) & 0x3)
#define     NFP3800_NBI_DMA_BUFFERLIST_BL_QUE_CTRL1_BL_QUE_SIZE_256 (0)
#define     NFP3800_NBI_DMA_BUFFERLIST_BL_QUE_CTRL1_BL_QUE_SIZE_512 (1)
#define     NFP3800_NBI_DMA_BUFFERLIST_BL_QUE_CTRL1_BL_QUE_SIZE_1K (2)
#define     NFP3800_NBI_DMA_BUFFERLIST_BL_QUE_CTRL1_BL_QUE_SIZE_2K (3)
#define   NFP3800_NBI_DMA_BUFFERLIST_BL_QUE_CTRL1_BL_ENTRY_CNT(_x) (((_x) & 0x7ff) << 24)
#define   NFP3800_NBI_DMA_BUFFERLIST_BL_QUE_CTRL1_BL_ENTRY_CNT_of(_x) (((_x) >> 24) & 0x7ff)
#define   NFP3800_NBI_DMA_BUFFERLIST_BL_QUE_CTRL1_BL_HEAD_PTR(_x) (((_x) & 0x7ff) << 12)
#define   NFP3800_NBI_DMA_BUFFERLIST_BL_QUE_CTRL1_BL_HEAD_PTR_of(_x) (((_x) >> 12) & 0x7ff)
#define   NFP3800_NBI_DMA_BUFFERLIST_BL_QUE_CTRL1_BL_TAIL_PTR(_x) (((_x) & 0x7ff) << 0)
#define   NFP3800_NBI_DMA_BUFFERLIST_BL_QUE_CTRL1_BL_TAIL_PTR_of(_x) (((_x) >> 0) & 0x7ff)
/*----------------------------------------------------------------
  Register: BLQueCtrl2 - Configures the buffer list circular queue and provides status
    Bits: 55:48 BLQEventFullCnt(rc) - Debug counter that increments when a BLQ Event is dropped due to a full BLQ Event FIFO. This counter saturates at all 1's
    Bits: 42:40 BDFifoEntryCnt(ro) - Number of buffer descriptors resident in the buffer descriptor register FIFO
    Bits: 39    BLQFull(ro) - The Queue is full when set.
    Bits: 38    BLQEmpty(ro) - The queue is empty when set.
    Bits: 37:36 BLQSize(rw) - Defines the Size of the Circular Buffer List Queue
    Bits: 34:24 BLEntryCnt(ro) - Hardware calculated value of the number of entries on the Buffer List Queue. A full Buffer List Queue has an entry count equal to zero, with the BLQFull flag set. An empty Buffer List Queue has an entry count equal to zero with the BLQEmpty flag set.
    Bits: 22:12 BLHdPtr(rw) - Buffer List Queue Head Pointer. Points to the next entry to be read by hardware.
    Bits: 10:0  BLTlPtr(rw) - Buffer List Queue Tail Pointer. Points to the next entry to be written at the tail.
*/
#define NFP3800_NBI_DMA_BufferList_BL_QUE_CTRL2              0x00008010
#define   NFP3800_NBI_DMA_BUFFERLIST_BL_QUE_CTRL2_BLQ_EVENT_FULL_CNT(_x) (((_x) & 0xff) << 48)
#define   NFP3800_NBI_DMA_BUFFERLIST_BL_QUE_CTRL2_BLQ_EVENT_FULL_CNT_of(_x) (((_x) >> 48) & 0xff)
#define   NFP3800_NBI_DMA_BUFFERLIST_BL_QUE_CTRL2_BD_FIFO_ENTRY_CNT(_x) (((_x) & 0x7) << 40)
#define   NFP3800_NBI_DMA_BUFFERLIST_BL_QUE_CTRL2_BD_FIFO_ENTRY_CNT_of(_x) (((_x) >> 40) & 0x7)
#define   NFP3800_NBI_DMA_BUFFERLIST_BL_QUE_CTRL2_BL_QUE_FULL BIT_ULL(39)
#define   NFP3800_NBI_DMA_BUFFERLIST_BL_QUE_CTRL2_BL_QUE_EMPTY BIT_ULL(38)
#define   NFP3800_NBI_DMA_BUFFERLIST_BL_QUE_CTRL2_BL_QUE_SIZE(_x) (((_x) & 0x3) << 36)
#define   NFP3800_NBI_DMA_BUFFERLIST_BL_QUE_CTRL2_BL_QUE_SIZE_of(_x) (((_x) >> 36) & 0x3)
#define     NFP3800_NBI_DMA_BUFFERLIST_BL_QUE_CTRL2_BL_QUE_SIZE_256 (0)
#define     NFP3800_NBI_DMA_BUFFERLIST_BL_QUE_CTRL2_BL_QUE_SIZE_512 (1)
#define     NFP3800_NBI_DMA_BUFFERLIST_BL_QUE_CTRL2_BL_QUE_SIZE_1K (2)
#define     NFP3800_NBI_DMA_BUFFERLIST_BL_QUE_CTRL2_BL_QUE_SIZE_2K (3)
#define   NFP3800_NBI_DMA_BUFFERLIST_BL_QUE_CTRL2_BL_ENTRY_CNT(_x) (((_x) & 0x7ff) << 24)
#define   NFP3800_NBI_DMA_BUFFERLIST_BL_QUE_CTRL2_BL_ENTRY_CNT_of(_x) (((_x) >> 24) & 0x7ff)
#define   NFP3800_NBI_DMA_BUFFERLIST_BL_QUE_CTRL2_BL_HEAD_PTR(_x) (((_x) & 0x7ff) << 12)
#define   NFP3800_NBI_DMA_BUFFERLIST_BL_QUE_CTRL2_BL_HEAD_PTR_of(_x) (((_x) >> 12) & 0x7ff)
#define   NFP3800_NBI_DMA_BUFFERLIST_BL_QUE_CTRL2_BL_TAIL_PTR(_x) (((_x) & 0x7ff) << 0)
#define   NFP3800_NBI_DMA_BUFFERLIST_BL_QUE_CTRL2_BL_TAIL_PTR_of(_x) (((_x) >> 0) & 0x7ff)
/*----------------------------------------------------------------
  Register: BLQueCtrl3 - Configures the buffer list circular queue and provides status
    Bits: 55:48 BLQEventFullCnt(rc) - Debug counter that increments when a BLQ Event is dropped due to a full BLQ Event FIFO. This counter saturates at all 1's
    Bits: 42:40 BDFifoEntryCnt(ro) - Number of buffer descriptors resident in the buffer descriptor register FIFO
    Bits: 39    BLQFull(ro) - The Queue is full when set.
    Bits: 38    BLQEmpty(ro) - The queue is empty when set.
    Bits: 37:36 BLQSize(rw) - Defines the Size of the Circular Buffer List Queue
    Bits: 34:24 BLEntryCnt(ro) - Hardware calculated value of the number of entries on the Buffer List Queue. A full Buffer List Queue has an entry count equal to zero, with the BLQFull flag set. An empty Buffer List Queue has an entry count equal to zero with the BLQEmpty flag set.
    Bits: 22:12 BLHdPtr(rw) - Buffer List Queue Head Pointer. Points to the next entry to be read by hardware.
    Bits: 10:0  BLTlPtr(rw) - Buffer List Queue Tail Pointer. Points to the next entry to be written at the tail.
*/
#define NFP3800_NBI_DMA_BufferList_BL_QUE_CTRL3              0x00008018
#define   NFP3800_NBI_DMA_BUFFERLIST_BL_QUE_CTRL3_BLQ_EVENT_FULL_CNT(_x) (((_x) & 0xff) << 48)
#define   NFP3800_NBI_DMA_BUFFERLIST_BL_QUE_CTRL3_BLQ_EVENT_FULL_CNT_of(_x) (((_x) >> 48) & 0xff)
#define   NFP3800_NBI_DMA_BUFFERLIST_BL_QUE_CTRL3_BD_FIFO_ENTRY_CNT(_x) (((_x) & 0x7) << 40)
#define   NFP3800_NBI_DMA_BUFFERLIST_BL_QUE_CTRL3_BD_FIFO_ENTRY_CNT_of(_x) (((_x) >> 40) & 0x7)
#define   NFP3800_NBI_DMA_BUFFERLIST_BL_QUE_CTRL3_BL_QUE_FULL BIT_ULL(39)
#define   NFP3800_NBI_DMA_BUFFERLIST_BL_QUE_CTRL3_BL_QUE_EMPTY BIT_ULL(38)
#define   NFP3800_NBI_DMA_BUFFERLIST_BL_QUE_CTRL3_BL_QUE_SIZE(_x) (((_x) & 0x3) << 36)
#define   NFP3800_NBI_DMA_BUFFERLIST_BL_QUE_CTRL3_BL_QUE_SIZE_of(_x) (((_x) >> 36) & 0x3)
#define     NFP3800_NBI_DMA_BUFFERLIST_BL_QUE_CTRL3_BL_QUE_SIZE_256 (0)
#define     NFP3800_NBI_DMA_BUFFERLIST_BL_QUE_CTRL3_BL_QUE_SIZE_512 (1)
#define     NFP3800_NBI_DMA_BUFFERLIST_BL_QUE_CTRL3_BL_QUE_SIZE_1K (2)
#define     NFP3800_NBI_DMA_BUFFERLIST_BL_QUE_CTRL3_BL_QUE_SIZE_2K (3)
#define   NFP3800_NBI_DMA_BUFFERLIST_BL_QUE_CTRL3_BL_ENTRY_CNT(_x) (((_x) & 0x7ff) << 24)
#define   NFP3800_NBI_DMA_BUFFERLIST_BL_QUE_CTRL3_BL_ENTRY_CNT_of(_x) (((_x) >> 24) & 0x7ff)
#define   NFP3800_NBI_DMA_BUFFERLIST_BL_QUE_CTRL3_BL_HEAD_PTR(_x) (((_x) & 0x7ff) << 12)
#define   NFP3800_NBI_DMA_BUFFERLIST_BL_QUE_CTRL3_BL_HEAD_PTR_of(_x) (((_x) >> 12) & 0x7ff)
#define   NFP3800_NBI_DMA_BUFFERLIST_BL_QUE_CTRL3_BL_TAIL_PTR(_x) (((_x) & 0x7ff) << 0)
#define   NFP3800_NBI_DMA_BUFFERLIST_BL_QUE_CTRL3_BL_TAIL_PTR_of(_x) (((_x) >> 0) & 0x7ff)
/*----------------------------------------------------------------
  Register: NbiDmaBCSramEntry[1024] - NBI DMA Buffer Completion SRAM Entry X
    Bits: 31:24 bufNum3(rw) - Buffer number 3
    Bits: 23:16 bufNum2(rw) - Buffer number 2
    Bits: 15:8  bufNum1(rw) - Buffer number 1
    Bits:  7:0  bufNum0(rw) - Buffer number 0
*/
#define NFP3800_NBI_DMA_BCSRAM_NBIDMA_BCSRAM_ENTRY(_x)       (0x0000a000 + (0x8 * ((_x) & 0x3ff)))
#define   NFP3800_NBI_DMA_BCSRAM_NBIDMA_BCSRAM_ENTRY_BUFNUM3(_x) (((_x) & 0xff) << 24)
#define   NFP3800_NBI_DMA_BCSRAM_NBIDMA_BCSRAM_ENTRY_BUFNUM3_of(_x) (((_x) >> 24) & 0xff)
#define   NFP3800_NBI_DMA_BCSRAM_NBIDMA_BCSRAM_ENTRY_BUFNUM2(_x) (((_x) & 0xff) << 16)
#define   NFP3800_NBI_DMA_BCSRAM_NBIDMA_BCSRAM_ENTRY_BUFNUM2_of(_x) (((_x) >> 16) & 0xff)
#define   NFP3800_NBI_DMA_BCSRAM_NBIDMA_BCSRAM_ENTRY_BUFNUM1(_x) (((_x) & 0xff) << 8)
#define   NFP3800_NBI_DMA_BCSRAM_NBIDMA_BCSRAM_ENTRY_BUFNUM1_of(_x) (((_x) >> 8) & 0xff)
#define   NFP3800_NBI_DMA_BCSRAM_NBIDMA_BCSRAM_ENTRY_BUFNUM0(_x) (((_x) & 0xff) << 0)
#define   NFP3800_NBI_DMA_BCSRAM_NBIDMA_BCSRAM_ENTRY_BUFNUM0_of(_x) (((_x) >> 0) & 0xff)
/*----------------------------------------------------------------
  Register: NbiDmaCfg - NBI DMA Configuration register
    Bits: 30:29 NegativeAlloc256BCr(rw) - In Allocation Lookahead mode, this is the threshold for the threshold for the outstanding 256B credits. Each credit is 256B.
    Bits: 28:27 BufferCreditThresh(rw) - In Allocation Lookahead mode, the Buffer Credit is allowed to go negative by this number.
    Bits: 26    AllocLookAheadEna(rw) - When set, it enables the Allocation Lookahead mode allowing the Buffer Credit to go 4 negative.
    Bits: 25    PacketColorEnable(rw) - When set, the NBI DMA adds 2-bit packet color in Picoengine metadata based on the CTM's credit level.
    Bits: 24    SeqNumOverride(rw) - When set, the NBI DMA overrides the Sequence Numbers in the PicoEngine metadata to avoid holes due to drops
    Bits: 23    PollRespNoWaitEna(rw) - When set, CTM Polling command responses are processed as they arrive. When cleared, CTM Polling command responses are processed after all outstanding responses have arrived
    Bits: 21    EnaEnhCtmOffset(rw) - Controls how the CtmOffset field of the NbiDmaBPCfg registers operates.
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
    Bits:  5:4  CtmPollIntvl(rw) - Configures the minimum amount of time between packet allocation poll commands in clock cycles.
    Bits:  3:2  PollCmdIssueRate(rw) - Configures the minimum space between CPP Polling commands issued after the polling interval has expired
    Bits:  1    RateLimitEnable(rw) - Selects whether to limit the bandwidth on the four Push Buses.
    Bits:  0    PollSearchEna(rw) - Enable searching for next CTM to poll
*/
#define NFP3800_NBI_DMAX_CSR_NBI_DMA_CFG                     0x00000000
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_CFG_NEGATIVE_ALLOC_256B_CR(_x) (((_x) & 0x3) << 29)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_CFG_NEGATIVE_ALLOC_256B_CR_of(_x) (((_x) >> 29) & 0x3)
#define     NFP3800_NBI_DMAX_CSR_NBI_DMA_CFG_NEGATIVE_ALLOC_256B_CR_16 (0)
#define     NFP3800_NBI_DMAX_CSR_NBI_DMA_CFG_NEGATIVE_ALLOC_256B_CR_32 (1)
#define     NFP3800_NBI_DMAX_CSR_NBI_DMA_CFG_NEGATIVE_ALLOC_256B_CR_48 (2)
#define     NFP3800_NBI_DMAX_CSR_NBI_DMA_CFG_NEGATIVE_ALLOC_256B_CR_64 (3)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_CFG_BUFFER_CREDIT_THRESH(_x) (((_x) & 0x3) << 27)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_CFG_BUFFER_CREDIT_THRESH_of(_x) (((_x) >> 27) & 0x3)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_CFG_ALLOC_LOOKAHEAD_ENA BIT(26)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_CFG_PACKET_COLOR_ENABLE BIT(25)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_CFG_SEQ_NUM_OVERRIDE  BIT(24)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_CFG_POLL_RESP_NO_WAIT_ENA BIT(23)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_CFG_ENA_ENH_CTM_OFFSET BIT(21)
#define     NFP3800_NBI_DMAX_CSR_NBI_DMA_CFG_ENA_ENH_CTM_OFFSET_CTMOFFSETCOMPATABILITYMODE (0 << 21)
#define     NFP3800_NBI_DMAX_CSR_NBI_DMA_CFG_ENA_ENH_CTM_OFFSET_CTMOFFSETENHANCEDMODE BIT(21)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_CFG_DIS_PUSHBUS_SELECT(_x) (((_x) & 0xf) << 17)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_CFG_DIS_PUSHBUS_SELECT_of(_x) (((_x) >> 17) & 0xf)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_CFG_DIS_BCRAM_ERR     BIT(16)
#define     NFP3800_NBI_DMAX_CSR_NBI_DMA_CFG_DIS_BCRAM_ERR_ENABLE (0 << 16)
#define     NFP3800_NBI_DMAX_CSR_NBI_DMA_CFG_DIS_BCRAM_ERR_DISABLE BIT(16)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_CFG_DIS_DSRAM_ERR     BIT(15)
#define     NFP3800_NBI_DMAX_CSR_NBI_DMA_CFG_DIS_DSRAM_ERR_ENABLE (0 << 15)
#define     NFP3800_NBI_DMAX_CSR_NBI_DMA_CFG_DIS_DSRAM_ERR_DISABLE BIT(15)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_CFG_DIS_BDRAM_ERR     BIT(14)
#define     NFP3800_NBI_DMAX_CSR_NBI_DMA_CFG_DIS_BDRAM_ERR_ENABLE (0 << 14)
#define     NFP3800_NBI_DMAX_CSR_NBI_DMA_CFG_DIS_BDRAM_ERR_DISABLE BIT(14)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_CFG_DIS_BUFRD_ERR     BIT(13)
#define     NFP3800_NBI_DMAX_CSR_NBI_DMA_CFG_DIS_BUFRD_ERR_ENABLE (0 << 13)
#define     NFP3800_NBI_DMAX_CSR_NBI_DMA_CFG_DIS_BUFRD_ERR_DISABLE BIT(13)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_CFG_DIS_RXPUSH_LAST_ERR BIT(12)
#define     NFP3800_NBI_DMAX_CSR_NBI_DMA_CFG_DIS_RXPUSH_LAST_ERR_ENABLE (0 << 12)
#define     NFP3800_NBI_DMAX_CSR_NBI_DMA_CFG_DIS_RXPUSH_LAST_ERR_DISABLE BIT(12)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_CFG_DIS_RXALLOC_IN_ERR BIT(11)
#define     NFP3800_NBI_DMAX_CSR_NBI_DMA_CFG_DIS_RXALLOC_IN_ERR_ENABLE (0 << 11)
#define     NFP3800_NBI_DMAX_CSR_NBI_DMA_CFG_DIS_RXALLOC_IN_ERR_DISABLE BIT(11)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_CFG_DIS_RXBLQWR_IN_ERR BIT(10)
#define     NFP3800_NBI_DMAX_CSR_NBI_DMA_CFG_DIS_RXBLQWR_IN_ERR_ENABLE (0 << 10)
#define     NFP3800_NBI_DMAX_CSR_NBI_DMA_CFG_DIS_RXBLQWR_IN_ERR_DISABLE BIT(10)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_CFG_NBI_NUM(_x)       (((_x) & 0x7) << 7)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_CFG_NBI_NUM_of(_x)    (((_x) >> 7) & 0x7)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_CFG_CTM_POLL_ENA      BIT(6)
#define     NFP3800_NBI_DMAX_CSR_NBI_DMA_CFG_CTM_POLL_ENA_DISABLE (0 << 6)
#define     NFP3800_NBI_DMAX_CSR_NBI_DMA_CFG_CTM_POLL_ENA_ENABLE BIT(6)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_CFG_CTM_POLL_INTVL(_x) (((_x) & 0x3) << 4)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_CFG_CTM_POLL_INTVL_of(_x) (((_x) >> 4) & 0x3)
#define     NFP3800_NBI_DMAX_CSR_NBI_DMA_CFG_CTM_POLL_INTVL_128 (0)
#define     NFP3800_NBI_DMAX_CSR_NBI_DMA_CFG_CTM_POLL_INTVL_256 (1)
#define     NFP3800_NBI_DMAX_CSR_NBI_DMA_CFG_CTM_POLL_INTVL_512 (2)
#define     NFP3800_NBI_DMAX_CSR_NBI_DMA_CFG_CTM_POLL_INTVL_1024 (3)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_CFG_POLL_CMD_ISSUE_RATE(_x) (((_x) & 0x3) << 2)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_CFG_POLL_CMD_ISSUE_RATE_of(_x) (((_x) >> 2) & 0x3)
#define     NFP3800_NBI_DMAX_CSR_NBI_DMA_CFG_POLL_CMD_ISSUE_RATE_4 (0)
#define     NFP3800_NBI_DMAX_CSR_NBI_DMA_CFG_POLL_CMD_ISSUE_RATE_8 (1)
#define     NFP3800_NBI_DMAX_CSR_NBI_DMA_CFG_POLL_CMD_ISSUE_RATE_12 (2)
#define     NFP3800_NBI_DMAX_CSR_NBI_DMA_CFG_POLL_CMD_ISSUE_RATE_16 (3)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_CFG_RATE_LIMIT_ENABLE BIT(1)
#define     NFP3800_NBI_DMAX_CSR_NBI_DMA_CFG_RATE_LIMIT_ENABLE_DISABLE (0 << 1)
#define     NFP3800_NBI_DMAX_CSR_NBI_DMA_CFG_RATE_LIMIT_ENABLE_ENABLE BIT(1)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_CFG_POLL_SEARCH_ENA   BIT(0)
/*----------------------------------------------------------------
  Register: NbiDmaBlqEvent - NBI DMA Buffer List Queue Event Control
    Bits: 22    ReducedThresh(rw) - If set lowers the blq event thresholds to 4, 8, 16 and 32.
    Bits: 20    DisBLQEventFifoFullAssert(rw) - Set this bit to disable the BLQ Event FIFO overflow assertion
    Bits: 19:18 BLQEventThreshold3(rw) - An event is generated every N buffer descriptors written to buffer list queue3, where N is configurable from 16 to 128 or 4 to 32 if Reduced BLQ Threshold is set..
    Bits: 17:16 BLQEventThreshold2(rw) - An event is generated every N buffer descriptors written to buffer list queue3, where N is configurable from 16 to 128 or 4 to 32 if Reduced BLQ Threshold is set..
    Bits: 15:14 BLQEventThreshold1(rw) - An event is generated every N buffer descriptors written to buffer list queue3, where N is configurable from 16 to 128 or 4 to 32 if Reduced BLQ Threshold is set..
    Bits: 13:12 BLQEventThreshold0(rw) - An event is generated every N buffer descriptors written to buffer list queue3, where N is configurable from 16 to 128 or 4 to 32 if Reduced BLQ Threshold is set..
    Bits:  7:4  BLQEventNotFullEnable(rw) - One bit per buffer list queue. Enables the generation of BLQ Events when the queue transitions from full to not full. Disabled when clear. The event type value used is 0x1 (event_type_fifo_not_full)
    Bits:  3:0  BLQEventStatusEnable(rw) - One bit per buffer list queue. Enables BLQ Status Events when set. Disabled when clear. A Status Event is sent when the number of entries read from the queue reaches the programmed value in the BLQEventThreshold register. The event type value used is 0x5 (event_type_value_update)
*/
#define NFP3800_NBI_DMAX_CSR_NBI_DMA_BLQ_EVENT               0x0000000c
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_BLQ_EVENT_REDUCED_THRESH BIT(22)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_BLQ_EVENT_DIS_BLQ_EVENT_FIFO_FULL_ASSERT BIT(20)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_BLQ_EVENT_BLQEVENT_THRESHOLD3(_x) (((_x) & 0x3) << 18)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_BLQ_EVENT_BLQEVENT_THRESHOLD3_of(_x) (((_x) >> 18) & 0x3)
#define     NFP3800_NBI_DMAX_CSR_NBI_DMA_BLQ_EVENT_BLQEVENT_THRESHOLD3_16_OR_4 (0)
#define     NFP3800_NBI_DMAX_CSR_NBI_DMA_BLQ_EVENT_BLQEVENT_THRESHOLD3_32_OR_8 (1)
#define     NFP3800_NBI_DMAX_CSR_NBI_DMA_BLQ_EVENT_BLQEVENT_THRESHOLD3_64_OR_16 (2)
#define     NFP3800_NBI_DMAX_CSR_NBI_DMA_BLQ_EVENT_BLQEVENT_THRESHOLD3_128_OR_32 (3)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_BLQ_EVENT_BLQEVENT_THRESHOLD2(_x) (((_x) & 0x3) << 16)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_BLQ_EVENT_BLQEVENT_THRESHOLD2_of(_x) (((_x) >> 16) & 0x3)
#define     NFP3800_NBI_DMAX_CSR_NBI_DMA_BLQ_EVENT_BLQEVENT_THRESHOLD2_16_OR_4 (0)
#define     NFP3800_NBI_DMAX_CSR_NBI_DMA_BLQ_EVENT_BLQEVENT_THRESHOLD2_32_OR_8 (1)
#define     NFP3800_NBI_DMAX_CSR_NBI_DMA_BLQ_EVENT_BLQEVENT_THRESHOLD2_64_OR_16 (2)
#define     NFP3800_NBI_DMAX_CSR_NBI_DMA_BLQ_EVENT_BLQEVENT_THRESHOLD2_128_OR_32 (3)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_BLQ_EVENT_BLQEVENT_THRESHOLD1(_x) (((_x) & 0x3) << 14)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_BLQ_EVENT_BLQEVENT_THRESHOLD1_of(_x) (((_x) >> 14) & 0x3)
#define     NFP3800_NBI_DMAX_CSR_NBI_DMA_BLQ_EVENT_BLQEVENT_THRESHOLD1_16_OR_4 (0)
#define     NFP3800_NBI_DMAX_CSR_NBI_DMA_BLQ_EVENT_BLQEVENT_THRESHOLD1_32_OR_8 (1)
#define     NFP3800_NBI_DMAX_CSR_NBI_DMA_BLQ_EVENT_BLQEVENT_THRESHOLD1_64_OR_16 (2)
#define     NFP3800_NBI_DMAX_CSR_NBI_DMA_BLQ_EVENT_BLQEVENT_THRESHOLD1_128_OR_32 (3)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_BLQ_EVENT_BLQEVENT_THRESHOLD0(_x) (((_x) & 0x3) << 12)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_BLQ_EVENT_BLQEVENT_THRESHOLD0_of(_x) (((_x) >> 12) & 0x3)
#define     NFP3800_NBI_DMAX_CSR_NBI_DMA_BLQ_EVENT_BLQEVENT_THRESHOLD0_16_OR_4 (0)
#define     NFP3800_NBI_DMAX_CSR_NBI_DMA_BLQ_EVENT_BLQEVENT_THRESHOLD0_32_OR_8 (1)
#define     NFP3800_NBI_DMAX_CSR_NBI_DMA_BLQ_EVENT_BLQEVENT_THRESHOLD0_64_OR_16 (2)
#define     NFP3800_NBI_DMAX_CSR_NBI_DMA_BLQ_EVENT_BLQEVENT_THRESHOLD0_128_OR_32 (3)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_BLQ_EVENT_BLQ_EVENT_NOT_FULL_ENABLE(_x) (((_x) & 0xf) << 4)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_BLQ_EVENT_BLQ_EVENT_NOT_FULL_ENABLE_of(_x) (((_x) >> 4) & 0xf)
#define     NFP3800_NBI_DMAX_CSR_NBI_DMA_BLQ_EVENT_BLQ_EVENT_NOT_FULL_ENABLE_DISABLE (0)
#define     NFP3800_NBI_DMAX_CSR_NBI_DMA_BLQ_EVENT_BLQ_EVENT_NOT_FULL_ENABLE_ENABLE (1)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_BLQ_EVENT_BLQ_EVENT_STATUS_ENABLE(_x) (((_x) & 0xf) << 0)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_BLQ_EVENT_BLQ_EVENT_STATUS_ENABLE_of(_x) (((_x) >> 0) & 0xf)
#define     NFP3800_NBI_DMAX_CSR_NBI_DMA_BLQ_EVENT_BLQ_EVENT_STATUS_ENABLE_DISABLE (0)
#define     NFP3800_NBI_DMAX_CSR_NBI_DMA_BLQ_EVENT_BLQ_EVENT_STATUS_ENABLE_ENABLE (1)
/*----------------------------------------------------------------
  Register: NbiDmaRate - NBI DMA Token Refresh Rate Register
    Bits:  7:6  CreditRate3(rw) - Sets the credit rate for push bus 3. Values are in 4 cycle increments.
    Bits:  5:4  CreditRate2(rw) - Sets the credit rate for push bus 2. Values are in 4 cycle increments.
    Bits:  3:2  CreditRate1(rw) - Sets the credit rate for push bus 1. Values are in 4 cycle increments.
    Bits:  1:0  CreditRate0(rw) - Sets the credit rate for push bus 0. Values are in 4 cycle increments.
*/
#define NFP3800_NBI_DMAX_CSR_NBI_DMA_RATE                    0x00000010
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_RATE_TOKEN_RATE3(_x)  (((_x) & 0x3) << 6)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_RATE_TOKEN_RATE3_of(_x) (((_x) >> 6) & 0x3)
#define     NFP3800_NBI_DMAX_CSR_NBI_DMA_RATE_TOKEN_RATE3_4  (0)
#define     NFP3800_NBI_DMAX_CSR_NBI_DMA_RATE_TOKEN_RATE3_8  (1)
#define     NFP3800_NBI_DMAX_CSR_NBI_DMA_RATE_TOKEN_RATE3_12 (2)
#define     NFP3800_NBI_DMAX_CSR_NBI_DMA_RATE_TOKEN_RATE3_16 (3)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_RATE_TOKEN_RATE2(_x)  (((_x) & 0x3) << 4)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_RATE_TOKEN_RATE2_of(_x) (((_x) >> 4) & 0x3)
#define     NFP3800_NBI_DMAX_CSR_NBI_DMA_RATE_TOKEN_RATE2_4  (0)
#define     NFP3800_NBI_DMAX_CSR_NBI_DMA_RATE_TOKEN_RATE2_8  (1)
#define     NFP3800_NBI_DMAX_CSR_NBI_DMA_RATE_TOKEN_RATE2_12 (2)
#define     NFP3800_NBI_DMAX_CSR_NBI_DMA_RATE_TOKEN_RATE2_16 (3)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_RATE_TOKEN_RATE1(_x)  (((_x) & 0x3) << 2)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_RATE_TOKEN_RATE1_of(_x) (((_x) >> 2) & 0x3)
#define     NFP3800_NBI_DMAX_CSR_NBI_DMA_RATE_TOKEN_RATE1_4  (0)
#define     NFP3800_NBI_DMAX_CSR_NBI_DMA_RATE_TOKEN_RATE1_8  (1)
#define     NFP3800_NBI_DMAX_CSR_NBI_DMA_RATE_TOKEN_RATE1_12 (2)
#define     NFP3800_NBI_DMAX_CSR_NBI_DMA_RATE_TOKEN_RATE1_16 (3)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_RATE_TOKEN_RATE0(_x)  (((_x) & 0x3) << 0)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_RATE_TOKEN_RATE0_of(_x) (((_x) >> 0) & 0x3)
#define     NFP3800_NBI_DMAX_CSR_NBI_DMA_RATE_TOKEN_RATE0_4  (0)
#define     NFP3800_NBI_DMAX_CSR_NBI_DMA_RATE_TOKEN_RATE0_8  (1)
#define     NFP3800_NBI_DMAX_CSR_NBI_DMA_RATE_TOKEN_RATE0_12 (2)
#define     NFP3800_NBI_DMAX_CSR_NBI_DMA_RATE_TOKEN_RATE0_16 (3)
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
#define NFP3800_NBI_DMAX_CSR_NBI_DMA_CREDIT                  0x00000014
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_CREDIT_THRESHOLD3(_x) (((_x) & 0x3) << 14)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_CREDIT_THRESHOLD3_of(_x) (((_x) >> 14) & 0x3)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_CREDIT_THRESHOLD2(_x) (((_x) & 0x3) << 12)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_CREDIT_THRESHOLD2_of(_x) (((_x) >> 12) & 0x3)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_CREDIT_THRESHOLD1(_x) (((_x) & 0x3) << 10)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_CREDIT_THRESHOLD1_of(_x) (((_x) >> 10) & 0x3)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_CREDIT_THRESHOLD0(_x) (((_x) & 0x3) << 8)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_CREDIT_THRESHOLD0_of(_x) (((_x) >> 8) & 0x3)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_CREDIT_CREDIT3(_x)    (((_x) & 0x3) << 6)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_CREDIT_CREDIT3_of(_x) (((_x) >> 6) & 0x3)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_CREDIT_CREDIT2(_x)    (((_x) & 0x3) << 4)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_CREDIT_CREDIT2_of(_x) (((_x) >> 4) & 0x3)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_CREDIT_CREDIT1(_x)    (((_x) & 0x3) << 2)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_CREDIT_CREDIT1_of(_x) (((_x) >> 2) & 0x3)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_CREDIT_CREDIT0(_x)    (((_x) & 0x3) << 0)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_CREDIT_CREDIT0_of(_x) (((_x) >> 0) & 0x3)
/*----------------------------------------------------------------
  Register: NbiDmaBpeChainEnd - NBI DMA BPE Chain End Register
    Bits: 31:0  BpeChainEnd(rw) - Configures BPE module 'n' as the end of the buffer pool entry chain, where 'n' = 0 to 31.
*/
#define NFP3800_NBI_DMAX_CSR_BPE_CHAIN_END                   0x00000018
#define   NFP3800_NBI_DMAX_CSR_BPE_CHAIN_END_BPE_CHAIN_END(_x) (((_x) & 0xffffffff) << 0)
#define   NFP3800_NBI_DMAX_CSR_BPE_CHAIN_END_BPE_CHAIN_END_of(_x) (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: NbiDmaBP[8]Cfg - NBI DMA Buffer Pool X Configuration
    Bits: 24    BlqBypassEna(rw) - When set, selects to use a null buffer descriptor if the packet is not split. The selection can also be made per packet using the preclassifier metadata_0 bits [1;6] when the BlOverrideEna is set.
    Bits: 23    BlOverrideEna(rw) - When set, Preclassifier metadata_0 bits [3;3] (PcBlistOverride) are used to select a buffer list override and metadata_0 bits [1;6] (PcBlqBypassEna) selects to use a null buffer descriptor if the packet is not split.
    Bits: 22:19 CtmBufPad(rw) - Selects the number of 32 bytes to add to the allocation size of the packet. Allows padding to the allocated space in the CTM for smaller packets in increments of 32B. 0=32B, 1=64B, 0xf=512B.
    Bits: 18    CtmBufPadEna(rw) - Enables adding CtmBufPad bytes to the allocation size of the packet.
    Bits: 17:15 CtmOffsetMsb(rw) - When the EnaEnhCtmOffset bit of the NbiDmaCfg register is a zero, this field is not used. When EnaEnhCtmOffset is a one, this field represents the MSbits of CtmOffset and defines the offset to add to the data reference when starting to write packet data to the CTM. The space in front of the packet is for the rewrite script used by the packet modifier. Encoding of {CtmOffsetMsb[3;0],CtmOffset} are: 0000 = 32B, 0001=64B, 0010=96B, 0011 = 128B, 0100 = 160B, ... 1111 = 512B
    Bits: 14    DropEn(rw) - Enables a packet to be dropped when buffer or packet credits run out or there are no MU buffers left on the selected buffer list. When cleared packets are not dropped and the logic backs up waiting for credits to arrive.
    Bits: 13    CtmOffset(rw) - When the EnaEnhCtmOffset bit of the NbiDmaCfg register is a zero, this field controls the CtmOffset in Compatability mode. In Compatability mode a CtmOffset = 0 equates to a CTM offset of 128 Bytes and a CtmOffset = 1 equates to a CTM offset of 64 Bytes. When the EnaEnhCtmOffset is a one this field contains the LSbit of CtmOffset. See description of CtmOffsetMsb.
    Bits: 12:11 PriBList(rw) - Primary Buffer list number associated. There are 4 buffer lists available, which contain buffer descriptors that are used to store packet data in internal/external MUs.
    Bits: 10:9  SecBList(rw) - Secondary Buffer list number associated. There are 4 buffer lists available, which contain buffer descriptors that are used to store packet data in internal/external MUs.
    Bits:  8:5  SplitLength(rw) - Encoded packet size used when deciding to split packets between CTM and internal/external MUs. Values 7,12-15 are reserved
    Bits:  4:0  BpeHead(rw) - Pointer to the first Buffer Pool Entry register in the list associated with this buffer pool.
*/
#define NFP3800_NBI_DMAX_CSR_NBI_DMA_BP_CFG(_x)              (0x00000020 + (0x4 * ((_x) & 0x7)))
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_BP_CFG_BLQ_BYPASS_ENA BIT(24)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_BP_CFG_OVERRIDE_ENA   BIT(23)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_BP_CFG_CTM_BUF_PAD(_x) (((_x) & 0xf) << 19)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_BP_CFG_CTM_BUF_PAD_of(_x) (((_x) >> 19) & 0xf)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_BP_CFG_CTM_BUF_PAD_ENA BIT(18)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_BP_CFG_CTM_OFFSET_MSB(_x) (((_x) & 0x7) << 15)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_BP_CFG_CTM_OFFSET_MSB_of(_x) (((_x) >> 15) & 0x7)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_BP_CFG_DROP_EN        BIT(14)
#define     NFP3800_NBI_DMAX_CSR_NBI_DMA_BP_CFG_DROP_EN_NODROP (0 << 14)
#define     NFP3800_NBI_DMAX_CSR_NBI_DMA_BP_CFG_DROP_EN_DROP BIT(14)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_BP_CFG_CTM_OFFSET     BIT(13)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_BP_CFG_PRI_BP_LIST(_x) (((_x) & 0x3) << 11)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_BP_CFG_PRI_BP_LIST_of(_x) (((_x) >> 11) & 0x3)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_BP_CFG_SEC_BP_LIST(_x) (((_x) & 0x3) << 9)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_BP_CFG_SEC_BP_LIST_of(_x) (((_x) >> 9) & 0x3)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_BP_CFG_SPLIT_LENGTH(_x) (((_x) & 0xf) << 5)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_BP_CFG_SPLIT_LENGTH_of(_x) (((_x) >> 5) & 0xf)
#define     NFP3800_NBI_DMAX_CSR_NBI_DMA_BP_CFG_SPLIT_LENGTH_256 (0)
#define     NFP3800_NBI_DMAX_CSR_NBI_DMA_BP_CFG_SPLIT_LENGTH_512 (1)
#define     NFP3800_NBI_DMAX_CSR_NBI_DMA_BP_CFG_SPLIT_LENGTH_1K (2)
#define     NFP3800_NBI_DMAX_CSR_NBI_DMA_BP_CFG_SPLIT_LENGTH_2K (3)
#define     NFP3800_NBI_DMAX_CSR_NBI_DMA_BP_CFG_SPLIT_LENGTH_4K (4)
#define     NFP3800_NBI_DMAX_CSR_NBI_DMA_BP_CFG_SPLIT_LENGTH_8K (5)
#define     NFP3800_NBI_DMAX_CSR_NBI_DMA_BP_CFG_SPLIT_LENGTH_16K (6)
#define     NFP3800_NBI_DMAX_CSR_NBI_DMA_BP_CFG_SPLIT_LENGTH_6K (8)
#define     NFP3800_NBI_DMAX_CSR_NBI_DMA_BP_CFG_SPLIT_LENGTH_10K (9)
#define     NFP3800_NBI_DMAX_CSR_NBI_DMA_BP_CFG_SPLIT_LENGTH_12K (10)
#define     NFP3800_NBI_DMAX_CSR_NBI_DMA_BP_CFG_SPLIT_LENGTH_14K (11)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_BP_CFG_BPE_HEAD(_x)   (((_x) & 0x1f) << 0)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_BP_CFG_BPE_HEAD_of(_x) (((_x) >> 0) & 0x1f)
/*----------------------------------------------------------------
  Register: NbiDmaBpe[32]Cfg - NBI DMA Buffer Pool Entry X Configuration
    Bits: 31:27 BpeNum(rwl) - Buffer Pool Entry Number. When the register is written this value is the latched version of the buffer pool register number 0-31. Used for debug purposes.
    Bits: 26:21 Ctm(rw) - CTM number to use
    Bits: 20:10 PktCredit(rw) - Configures the number of packets allocated to the CTM
    Bits:  9:0  BufCredit(rw) - Configures the number of buffer credits in 2K byte increments associated with the CTM. The msbit is the sign bit and must be cleared during configuration.
*/
#define NFP3800_NBI_DMAX_CSR_NBI_DMA_BPE_CFG(_x)             (0x00000040 + (0x4 * ((_x) & 0x1f)))
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_BPE_CFG_BPE_NUM(_x)   (((_x) & 0x1f) << 27)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_BPE_CFG_BPE_NUM_of(_x) (((_x) >> 27) & 0x1f)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_BPE_CFG_CTM(_x)       (((_x) & 0x3f) << 21)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_BPE_CFG_CTM_of(_x)    (((_x) >> 21) & 0x3f)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_BPE_CFG_PKT_CREDIT(_x) (((_x) & 0x7ff) << 10)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_BPE_CFG_PKT_CREDIT_of(_x) (((_x) >> 10) & 0x7ff)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_BPE_CFG_BUF_CREDIT(_x) (((_x) & 0x3ff) << 0)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_BPE_CFG_BUF_CREDIT_of(_x) (((_x) >> 0) & 0x3ff)
/*----------------------------------------------------------------
  Register: NbiDmaPktCntLo - Nbi Dma Pkt Counter Low 32-bits
    Bits: 31:0  NbiDmaPktCntLo(rc) - Lower 32-bits of 48-bit NBI DMA Tx Packet count. Reading this register latches the value in NbiDmaPktCntHi register
*/
#define NFP3800_NBI_DMAX_CSR_NBI_DMA_PKT_CNT_LO              0x000000c0
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_PKT_CNT_LO_NBI_DMA_PKT_CNT_LO(_x) (((_x) & 0xffffffff) << 0)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_PKT_CNT_LO_NBI_DMA_PKT_CNT_LO_of(_x) (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: NbiDmaPktCntHi - Nbi Dma Pkt Counter High 16-bits
    Bits: 15:0  NbiDmaPktCntHi(rc) - Upper 16-bits of 48-bit NBI Tx Packet count. Read this register after reading the NbiDmaPktCntLo register
*/
#define NFP3800_NBI_DMAX_CSR_NBI_DMA_PKT_CNT_HI              0x000000c4
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_PKT_CNT_HI_NBI_DMA_PKT_CNT_HI(_x) (((_x) & 0xffff) << 0)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_PKT_CNT_HI_NBI_DMA_PKT_CNT_HI_of(_x) (((_x) >> 0) & 0xffff)
/*----------------------------------------------------------------
  Register: NbiDmaByteCntLo - Nbi Dma Byte Counter Low 32-bits
    Bits: 31:0  NbiDmaByteCntLo(rc) - Lower 32-bits of 64-bit NBI DMA Tx Byte count. Reading this register latches the value in NbiDmaByteCntHi register
*/
#define NFP3800_NBI_DMAX_CSR_NBI_DMA_BYTE_CNT_LO             0x000000c8
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_BYTE_CNT_LO_NBI_DMA_BYTE_CNT_LO(_x) (((_x) & 0xffffffff) << 0)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_BYTE_CNT_LO_NBI_DMA_BYTE_CNT_LO_of(_x) (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: NbiDmaByteCntHi - Nbi Dma Byte Counter High 32-bits
    Bits: 31:0  NbiDmaByteCntHi(rc) - Upper 32-bits of 64-bit NBI DMA Tx Byte count. Read this register after reading the NbiDmaByteCntLo register
*/
#define NFP3800_NBI_DMAX_CSR_NBI_DMA_BYTE_CNT_HI             0x000000cc
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_BYTE_CNT_HI_NBI_DMA_BYTE_CNT_HI(_x) (((_x) & 0xffffffff) << 0)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_BYTE_CNT_HI_NBI_DMA_BYTE_CNT_HI_of(_x) (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: NbiDmaDropPktCntLo - Nbi Dma Drop Pkt Counter Low 32-bits
    Bits: 31:0  NbiDmaDropPktCntLo(rc) - Lower 32-bits of 48-bit NBI DMA Drop Packet count. Reading this register latches the value in NbiDmaDropPktCntHi register
*/
#define NFP3800_NBI_DMAX_CSR_NBI_DMA_DROP_PKT_CNT_LO         0x000000d0
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_DROP_PKT_CNT_LO_NBI_DMA_DROP_PKT_CNT_LO(_x) (((_x) & 0xffffffff) << 0)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_DROP_PKT_CNT_LO_NBI_DMA_DROP_PKT_CNT_LO_of(_x) (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: NbiDmaDropPktCntHi - Nbi Dma Drop Pkt Counter High 16-bits
    Bits: 15:0  NbiDmaDropPktCntHi(rc) - Upper 16-bits of 48-bit NBI Drop Packet count. Read this register after reading the NbiDmaDropPktCntLo register
*/
#define NFP3800_NBI_DMAX_CSR_NBI_DMA_DROP_PKT_CNT_HI         0x000000d4
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_DROP_PKT_CNT_HI_NBI_DMA_DROP_PKT_CNT_HI(_x) (((_x) & 0xffff) << 0)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_DROP_PKT_CNT_HI_NBI_DMA_DROP_PKT_CNT_HI_of(_x) (((_x) >> 0) & 0xffff)
/*----------------------------------------------------------------
  Register: NbiDmaDropByteCntLo - Nbi Dma Drop Byte Counter Low 32-bits
    Bits: 31:0  NbiDmaDropByteCntLo(rc) - Lower 32-bits of 64-bit NBI DMA Drop Byte count. Reading this register latches the value in NbiDmaDropByteCntHi register
*/
#define NFP3800_NBI_DMAX_CSR_NBI_DMA_DROP_BYTE_CNT_LO        0x000000d8
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_DROP_BYTE_CNT_LO_NBI_DMA_DROP_BYTE_CNT_LO(_x) (((_x) & 0xffffffff) << 0)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_DROP_BYTE_CNT_LO_NBI_DMA_DROP_BYTE_CNT_LO_of(_x) (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: NbiDmaDropByteCntHi - Nbi Dma Drop Byte Counter High 32-bits
    Bits: 31:0  NbiDmaDropByteCntHi(rc) - Upper 32-bits of 64-bit NBI DMA Drop Byte count. Read this register after reading the NbiDmaDropByteCntLo register
*/
#define NFP3800_NBI_DMAX_CSR_NBI_DMA_DROP_BYTE_CNT_HI        0x000000dc
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_DROP_BYTE_CNT_HI_NBI_DMA_DROP_BYTE_CNT_HI(_x) (((_x) & 0xffffffff) << 0)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_DROP_BYTE_CNT_HI_NBI_DMA_DROP_BYTE_CNT_HI_of(_x) (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: NbiDmaBP[8]CtmCrThresh - NBI DMA Buffer Pool CTM Credit Thresholds.
    Bits: 29:20 Threshold2(rw) - Configures Threshold2 for the CTM credits. The packet color value is 2b11 if: credit < Threshold2. Only lower 9 bits are used, the top bit should be cleared.
    Bits: 19:10 Threshold1(rw) - Configures Threshold1 for the CTM credits. The packet color value is 2b10 if: Threshold2 <= credit < Threshold1. Only lower 9 bits are used, the top bit should be cleared.
    Bits:  9:0  Threshold0(rw) - Configures Threshold0 for the CTM credits. The packet color value is 2b01 if : Threshold1 <= credit < Threshold0, else it remains 2b00 if credit >= Threshold0. Only lower 9 bits are used, the top bit should be cleared.
*/
#define NFP3800_NBI_DMAX_CSR_NBI_DMA_BP_CTMCR_THRESH(_x)     (0x00000148 + (0x4 * ((_x) & 0x7)))
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_BP_CTMCR_THRESH_THRESHOLD2(_x) (((_x) & 0x3ff) << 20)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_BP_CTMCR_THRESH_THRESHOLD2_of(_x) (((_x) >> 20) & 0x3ff)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_BP_CTMCR_THRESH_THRESHOLD1(_x) (((_x) & 0x3ff) << 10)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_BP_CTMCR_THRESH_THRESHOLD1_of(_x) (((_x) >> 10) & 0x3ff)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_BP_CTMCR_THRESH_THRESHOLD0(_x) (((_x) & 0x3ff) << 0)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_BP_CTMCR_THRESH_THRESHOLD0_of(_x) (((_x) >> 0) & 0x3ff)
/*----------------------------------------------------------------
  Register: BlqNullMuPtr - Programmable Null Memory Buffer Pointer.
    Bits: 28:0  BlqNullMuPtr(rw) - Programmable Null Memory Buffer Pointer.
*/
#define NFP3800_NBI_DMAX_CSR_BLQ_NULL_MU_PTR                 0x00000168
#define   NFP3800_NBI_DMAX_CSR_BLQ_NULL_MU_PTR_BLQ_NULL_MU_PTR(_x) (((_x) & 0x1fffffff) << 0)
#define   NFP3800_NBI_DMAX_CSR_BLQ_NULL_MU_PTR_BLQ_NULL_MU_PTR_of(_x) (((_x) >> 0) & 0x1fffffff)
/*----------------------------------------------------------------
  Register: NbiDmaSeqNum[8] - Sequence Numbers for Sequencer X
    Bits: 11:0  NbiDmaSeqNum(rw1c) - Sequence Numbers (0..4095)
*/
#define NFP3800_NBI_DMAX_CSR_NBI_DMA_SEQ_NUM(_x)             (0x0000016c + (0x4 * ((_x) & 0x7)))
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_SEQ_NUM_NBI_DMA_SEQ_NUM(_x) (((_x) & 0xfff) << 0)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_SEQ_NUM_NBI_DMA_SEQ_NUM_of(_x) (((_x) >> 0) & 0xfff)
/*----------------------------------------------------------------
  Register: NbiDmaFlowControlManualThreshold[20] - Port based Flow Control Threshold X
    Bits: 15:0  Threshold(rw) - When AutoThresholdEnable = 1, this register is read only and reflects the current value of the port threshold set by the high-level algorithm. When AutoThresholdEnable = 0, the programmed value is used as the port threshold.
*/
#define NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_MANUAL_THRESHOLD(_x) (0x0000018c + (0x4 * ((_x) & 0x1f)))
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_MANUAL_THRESHOLD_THRESHOLD(_x) (((_x) & 0xffff) << 0)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_MANUAL_THRESHOLD_THRESHOLD_of(_x) (((_x) >> 0) & 0xffff)
/*----------------------------------------------------------------
  Register: NbiDmaFlowControlConfig0 - Port based Flow Control Config
    Bits: 23:21 T0fraction(rw) - Programmed value is threshold 0 in terms of a fraction of CTM Capacity: When the Total Packet Bucket climbs to this threshold the Auto Threshold State Machine transitions from State 0 to State 1. This value must be largest of the three T#fractions.
    Bits: 20:18 T1fraction(rw) - Programmed value is threshold 1 in terms of a fraction of CTM Capacity: When the Total Packet Bucket falls to this threshold the Auto Threshold State Machine transitions from State 1 to State 0. This value must be between T0fraction and T2fraction.
    Bits: 17:15 T2fraction(rw) - Programmed value is threshold 2 in terms of a fraction of CTM Capacity: When the Total Packet Bucket falls to this threshold the Auto Threshold State Machine transitions from State 2 to State 0. This value must be smallest of the three T#fractions.
    Bits: 14:12 AutoThresholdTimeout(rw) - Programmed value controls the number of packets while in State 1 after which the Auto Threshold State Machine will transition from State 1 to State 2
    Bits: 11    AutoThresholdEnable(rw) - When programmed to 1, enables the Auto Threshold State Machine to execute the high-level algorithm that periodically re-adjusts the individual port bucket thresholds that govern the per port XON/XOFF flow control
    Bits: 10:7  XonHysteresis(rw) - Programmed value controls the XON Hysteresis. If a port is XOFF'd, the port bucket value must fall this amount below its port bucket threshold before being XON'd
    Bits:  6:4  XoffTimer(rw) - Programmed value controls the maximum number of clocks a port may be XOFF'd. When a port has been off for this duration, it will be XON'd until one packet is seen, after which it will be XOFF'd again if the original XOFF condition exists. This maximum only has effect while the Auto Threshold State Machine is in State 2.
    Bits:  3    RateMeterEnable(rw) - When programmed to 1, enables all the rate meters. Disabling the rate meters will freeze the values so that they can be read one at a time, but represent a snapshot in time.
    Bits:  2:0  RateMeterResolution(rw) - When programmed to a value of N, the rate meters will have a resolution of 1/(2**(15-N)*(CLK PERIOD in secs)*(10**6)) Million Packets Per Sec
*/
#define NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CONFIG0    0x000001dc
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CONFIG0_T0_FRACTION(_x) (((_x) & 0x7) << 21)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CONFIG0_T0_FRACTION_of(_x) (((_x) >> 21) & 0x7)
#define     NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CONFIG0_T0_FRACTION_8 (0)
#define     NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CONFIG0_T0_FRACTION_9 (1)
#define     NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CONFIG0_T0_FRACTION_10 (2)
#define     NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CONFIG0_T0_FRACTION_11 (3)
#define     NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CONFIG0_T0_FRACTION_12 (4)
#define     NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CONFIG0_T0_FRACTION_13 (5)
#define     NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CONFIG0_T0_FRACTION_14 (6)
#define     NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CONFIG0_T0_FRACTION_15 (7)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CONFIG0_T1_FRACTION(_x) (((_x) & 0x7) << 18)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CONFIG0_T1_FRACTION_of(_x) (((_x) >> 18) & 0x7)
#define     NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CONFIG0_T1_FRACTION_8 (0)
#define     NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CONFIG0_T1_FRACTION_9 (1)
#define     NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CONFIG0_T1_FRACTION_10 (2)
#define     NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CONFIG0_T1_FRACTION_11 (3)
#define     NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CONFIG0_T1_FRACTION_12 (4)
#define     NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CONFIG0_T1_FRACTION_13 (5)
#define     NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CONFIG0_T1_FRACTION_14 (6)
#define     NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CONFIG0_T1_FRACTION_15 (7)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CONFIG0_T2_FRACTION(_x) (((_x) & 0x7) << 15)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CONFIG0_T2_FRACTION_of(_x) (((_x) >> 15) & 0x7)
#define     NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CONFIG0_T2_FRACTION_8 (0)
#define     NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CONFIG0_T2_FRACTION_9 (1)
#define     NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CONFIG0_T2_FRACTION_10 (2)
#define     NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CONFIG0_T2_FRACTION_11 (3)
#define     NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CONFIG0_T2_FRACTION_12 (4)
#define     NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CONFIG0_T2_FRACTION_13 (5)
#define     NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CONFIG0_T2_FRACTION_14 (6)
#define     NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CONFIG0_T2_FRACTION_15 (7)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CONFIG0_AUTO_THRESHOLD_TIMEOUT(_x) (((_x) & 0x7) << 12)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CONFIG0_AUTO_THRESHOLD_TIMEOUT_of(_x) (((_x) >> 12) & 0x7)
#define     NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CONFIG0_AUTO_THRESHOLD_TIMEOUT_256 (0)
#define     NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CONFIG0_AUTO_THRESHOLD_TIMEOUT_512 (1)
#define     NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CONFIG0_AUTO_THRESHOLD_TIMEOUT_1024 (2)
#define     NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CONFIG0_AUTO_THRESHOLD_TIMEOUT_2048 (3)
#define     NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CONFIG0_AUTO_THRESHOLD_TIMEOUT_4096 (4)
#define     NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CONFIG0_AUTO_THRESHOLD_TIMEOUT_8192 (5)
#define     NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CONFIG0_AUTO_THRESHOLD_TIMEOUT_16384 (6)
#define     NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CONFIG0_AUTO_THRESHOLD_TIMEOUT_32768 (7)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CONFIG0_AUTO_THRESHOLD_ENABLE BIT(11)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CONFIG0_XON_HYSTERESIS(_x) (((_x) & 0xf) << 7)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CONFIG0_XON_HYSTERESIS_of(_x) (((_x) >> 7) & 0xf)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CONFIG0_XOFF_TIMER(_x) (((_x) & 0x7) << 4)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CONFIG0_XOFF_TIMER_of(_x) (((_x) >> 4) & 0x7)
#define     NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CONFIG0_XOFF_TIMER_256 (0)
#define     NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CONFIG0_XOFF_TIMER_512 (1)
#define     NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CONFIG0_XOFF_TIMER_1024 (2)
#define     NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CONFIG0_XOFF_TIMER_2048 (3)
#define     NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CONFIG0_XOFF_TIMER_4096 (4)
#define     NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CONFIG0_XOFF_TIMER_8192 (5)
#define     NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CONFIG0_XOFF_TIMER_16384 (6)
#define     NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CONFIG0_XOFF_TIMER_32768 (7)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CONFIG0_RATE_METER_ENABLE BIT(3)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CONFIG0_RATE_METER_RESOLUTIION(_x) (((_x) & 0x7) << 0)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CONFIG0_RATE_METER_RESOLUTIION_of(_x) (((_x) >> 0) & 0x7)
/*----------------------------------------------------------------
  Register: NbiDmaFlowControlConfig1 - Port based Flow Control Config
    Bits: 15:0  CtmCapacity(rw) - Programmed value is the maximum number of CTM packet credits usable by this NBI
*/
#define NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CONFIG1    0x000001e0
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CONFIG1_CTM_CAPACITY(_x) (((_x) & 0xffff) << 0)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CONFIG1_CTM_CAPACITY_of(_x) (((_x) >> 0) & 0xffff)
/*----------------------------------------------------------------
  Register: NbiDmaFlowControlStatus0 - Port based Flow Control status
    Bits: 31:30 AutoThresholdState(ro) - State of the Auto Threshold Algorithm
    Bits: 29:16 TimeoutPacketCounter(ro) - Current Value of the TimeoutPacketCounter associated with the AutoThresholdTimeOut
    Bits: 15:0  TotalPacketsBucket(ro) - Indicates the total number of packets that are currently being processed in the chip from this NBI
*/
#define NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_STATUS0    0x000001e4
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_STATUS0_AUTO_THRESHOLD_STATE(_x) (((_x) & 0x3) << 30)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_STATUS0_AUTO_THRESHOLD_STATE_of(_x) (((_x) >> 30) & 0x3)
#define     NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_STATUS0_AUTO_THRESHOLD_STATE_0 (0)
#define     NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_STATUS0_AUTO_THRESHOLD_STATE_1 (1)
#define     NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_STATUS0_AUTO_THRESHOLD_STATE_2 (2)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_STATUS0_TIMEOUT_PACKET_COUNTER(_x) (((_x) & 0x3fff) << 16)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_STATUS0_TIMEOUT_PACKET_COUNTER_of(_x) (((_x) >> 16) & 0x3fff)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_STATUS0_TOTAL_PACKETS_BUCKET(_x) (((_x) & 0xffff) << 0)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_STATUS0_TOTAL_PACKETS_BUCKET_of(_x) (((_x) >> 0) & 0xffff)
/*----------------------------------------------------------------
  Register: NbiDmaFlowControlStatus1 - Port based Flow Control Status
    Bits: 31:16 ReturnCreditRate(ro) - Measured rate of packet credit returns; effectively processing rate. Value/(32768*(CLK PERIOD in secs)*(10**6)) = Millions of packets/sec
    Bits: 15:0  NewPacketRate(ro) - Measured rate of packets entering the chip through this NBI. Value/(32768*(CLK PERIOD in secs)*(10**6)) = Millions of packets/sec
*/
#define NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_STATUS1    0x000001e8
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_STATUS1_RETURN_CREDIT_RATE(_x) (((_x) & 0xffff) << 16)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_STATUS1_RETURN_CREDIT_RATE_of(_x) (((_x) >> 16) & 0xffff)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_STATUS1_NEW_PACKET_RATE(_x) (((_x) & 0xffff) << 0)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_STATUS1_NEW_PACKET_RATE_of(_x) (((_x) >> 0) & 0xffff)
/*----------------------------------------------------------------
  Register: NbiDmaFlowControlPortBucketStatus0 - Port based Flow Control Status
    Bits: 20:16 Port(rw) - Programmed value provides port index information presented in the three NbiDmaFlowControlPortBucketStatusN registers.
    Bits: 15:0  Rate(ro) - Measured rate of packets entering the chip through this port. Value/(32768*(CLK PERIOD in secs)*(10**6)) = Millions of packets/sec
*/
#define NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_PORT_BUCKET_STATUS0 0x000001ec
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_PORT_BUCKET_STATUS0_PORT(_x) (((_x) & 0x1f) << 16)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_PORT_BUCKET_STATUS0_PORT_of(_x) (((_x) >> 16) & 0x1f)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_PORT_BUCKET_STATUS0_RATE(_x) (((_x) & 0xffff) << 0)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_PORT_BUCKET_STATUS0_RATE_of(_x) (((_x) >> 0) & 0xffff)
/*----------------------------------------------------------------
  Register: NbiDmaFlowControlPortBucketStatus1 - Port based Flow Control Status
    Bits: 15:0  Bucket(ro) - Current number of packets being processed that entered through this port.
*/
#define NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_PORT_BUCKET_STATUS1 0x000001f0
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_PORT_BUCKET_STATUS1_BUCKET(_x) (((_x) & 0xffff) << 0)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_PORT_BUCKET_STATUS1_BUCKET_of(_x) (((_x) >> 0) & 0xffff)
/*----------------------------------------------------------------
  Register: NbiDmaFlowControlPortBucketStatus2 - Port based Flow Control Status
    Bits: 29:14 Accumulator(ro) - Value of the credit return accumulator for the indexed port.
    Bits: 13:0  XoffTimer(ro) - Value of XoffTimer for indexed port associated with the XoffTimer programmable threshold in NbiDmaFlowControlConfig0.
*/
#define NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_PORT_BUCKET_STATUS2 0x000001f4
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_PORT_BUCKET_STATUS2_ACCUMULATOR(_x) (((_x) & 0xffff) << 14)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_PORT_BUCKET_STATUS2_ACCUMULATOR_of(_x) (((_x) >> 14) & 0xffff)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_PORT_BUCKET_STATUS2_XOFF_TIMER(_x) (((_x) & 0x3fff) << 0)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_PORT_BUCKET_STATUS2_XOFF_TIMER_of(_x) (((_x) >> 0) & 0x3fff)
/*----------------------------------------------------------------
  Register: NbiDmaFlowControlChannelPortMap0 - Channel to Port Map
    Bits: 29:25 Channel5(rw) - Defines the Channel to Port Mapping
    Bits: 24:20 Channel4(rw) - Defines the Channel to Port Mapping
    Bits: 19:15 Channel3(rw) - Defines the Channel to Port Mapping
    Bits: 14:10 Channel2(rw) - Defines the Channel to Port Mapping
    Bits:  9:5  Channel1(rw) - Defines the Channel to Port Mapping
    Bits:  4:0  Channel0(rw) - Defines the Channel to Port Mapping
*/
#define NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP0 0x000001f8
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP0_CHANNEL5(_x) (((_x) & 0x1f) << 25)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP0_CHANNEL5_of(_x) (((_x) >> 25) & 0x1f)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP0_CHANNEL4(_x) (((_x) & 0x1f) << 20)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP0_CHANNEL4_of(_x) (((_x) >> 20) & 0x1f)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP0_CHANNEL3(_x) (((_x) & 0x1f) << 15)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP0_CHANNEL3_of(_x) (((_x) >> 15) & 0x1f)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP0_CHANNEL2(_x) (((_x) & 0x1f) << 10)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP0_CHANNEL2_of(_x) (((_x) >> 10) & 0x1f)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP0_CHANNEL1(_x) (((_x) & 0x1f) << 5)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP0_CHANNEL1_of(_x) (((_x) >> 5) & 0x1f)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP0_CHANNEL0(_x) (((_x) & 0x1f) << 0)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP0_CHANNEL0_of(_x) (((_x) >> 0) & 0x1f)
/*----------------------------------------------------------------
  Register: NbiDmaFlowControlChannelPortMap1 - Channel to Port Map
    Bits: 29:25 Channel11(rw) - Defines the Channel to Port Mapping
    Bits: 24:20 Channel10(rw) - Defines the Channel to Port Mapping
    Bits: 19:15 Channel9(rw) - Defines the Channel to Port Mapping
    Bits: 14:10 Channel8(rw) - Defines the Channel to Port Mapping
    Bits:  9:5  Channel7(rw) - Defines the Channel to Port Mapping
    Bits:  4:0  Channel6(rw) - Defines the Channel to Port Mapping
*/
#define NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP1 0x000001fc
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP1_CHANNEL11(_x) (((_x) & 0x1f) << 25)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP1_CHANNEL11_of(_x) (((_x) >> 25) & 0x1f)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP1_CHANNEL10(_x) (((_x) & 0x1f) << 20)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP1_CHANNEL10_of(_x) (((_x) >> 20) & 0x1f)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP1_CHANNEL9(_x) (((_x) & 0x1f) << 15)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP1_CHANNEL9_of(_x) (((_x) >> 15) & 0x1f)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP1_CHANNEL8(_x) (((_x) & 0x1f) << 10)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP1_CHANNEL8_of(_x) (((_x) >> 10) & 0x1f)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP1_CHANNEL7(_x) (((_x) & 0x1f) << 5)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP1_CHANNEL7_of(_x) (((_x) >> 5) & 0x1f)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP1_CHANNEL6(_x) (((_x) & 0x1f) << 0)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP1_CHANNEL6_of(_x) (((_x) >> 0) & 0x1f)
/*----------------------------------------------------------------
  Register: NbiDmaFlowControlChannelPortMap2 - Channel to Port Map
    Bits: 29:25 Channel17(rw) - Defines the Channel to Port Mapping
    Bits: 24:20 Channel16(rw) - Defines the Channel to Port Mapping
    Bits: 19:15 Channel15(rw) - Defines the Channel to Port Mapping
    Bits: 14:10 Channel14(rw) - Defines the Channel to Port Mapping
    Bits:  9:5  Channel13(rw) - Defines the Channel to Port Mapping
    Bits:  4:0  Channel12(rw) - Defines the Channel to Port Mapping
*/
#define NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP2 0x00000200
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP2_CHANNEL17(_x) (((_x) & 0x1f) << 25)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP2_CHANNEL17_of(_x) (((_x) >> 25) & 0x1f)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP2_CHANNEL16(_x) (((_x) & 0x1f) << 20)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP2_CHANNEL16_of(_x) (((_x) >> 20) & 0x1f)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP2_CHANNEL15(_x) (((_x) & 0x1f) << 15)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP2_CHANNEL15_of(_x) (((_x) >> 15) & 0x1f)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP2_CHANNEL14(_x) (((_x) & 0x1f) << 10)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP2_CHANNEL14_of(_x) (((_x) >> 10) & 0x1f)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP2_CHANNEL13(_x) (((_x) & 0x1f) << 5)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP2_CHANNEL13_of(_x) (((_x) >> 5) & 0x1f)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP2_CHANNEL12(_x) (((_x) & 0x1f) << 0)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP2_CHANNEL12_of(_x) (((_x) >> 0) & 0x1f)
/*----------------------------------------------------------------
  Register: NbiDmaFlowControlChannelPortMap3 - Channel to Port Map
    Bits: 29:25 Channel23(rw) - Defines the Channel to Port Mapping
    Bits: 24:20 Channel22(rw) - Defines the Channel to Port Mapping
    Bits: 19:15 Channel21(rw) - Defines the Channel to Port Mapping
    Bits: 14:10 Channel20(rw) - Defines the Channel to Port Mapping
    Bits:  9:5  Channel19(rw) - Defines the Channel to Port Mapping
    Bits:  4:0  Channel18(rw) - Defines the Channel to Port Mapping
*/
#define NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP3 0x00000204
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP3_CHANNEL23(_x) (((_x) & 0x1f) << 25)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP3_CHANNEL23_of(_x) (((_x) >> 25) & 0x1f)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP3_CHANNEL22(_x) (((_x) & 0x1f) << 20)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP3_CHANNEL22_of(_x) (((_x) >> 20) & 0x1f)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP3_CHANNEL21(_x) (((_x) & 0x1f) << 15)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP3_CHANNEL21_of(_x) (((_x) >> 15) & 0x1f)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP3_CHANNEL20(_x) (((_x) & 0x1f) << 10)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP3_CHANNEL20_of(_x) (((_x) >> 10) & 0x1f)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP3_CHANNEL19(_x) (((_x) & 0x1f) << 5)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP3_CHANNEL19_of(_x) (((_x) >> 5) & 0x1f)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP3_CHANNEL18(_x) (((_x) & 0x1f) << 0)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP3_CHANNEL18_of(_x) (((_x) >> 0) & 0x1f)
/*----------------------------------------------------------------
  Register: NbiDmaFlowControlChannelPortMap4 - Channel to Port Map
    Bits: 29:25 Channel29(rw) - Defines the Channel to Port Mapping
    Bits: 24:20 Channel28(rw) - Defines the Channel to Port Mapping
    Bits: 19:15 Channel27(rw) - Defines the Channel to Port Mapping
    Bits: 14:10 Channel26(rw) - Defines the Channel to Port Mapping
    Bits:  9:5  Channel25(rw) - Defines the Channel to Port Mapping
    Bits:  4:0  Channel24(rw) - Defines the Channel to Port Mapping
*/
#define NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP4 0x00000208
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP4_CHANNEL29(_x) (((_x) & 0x1f) << 25)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP4_CHANNEL29_of(_x) (((_x) >> 25) & 0x1f)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP4_CHANNEL28(_x) (((_x) & 0x1f) << 20)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP4_CHANNEL28_of(_x) (((_x) >> 20) & 0x1f)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP4_CHANNEL27(_x) (((_x) & 0x1f) << 15)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP4_CHANNEL27_of(_x) (((_x) >> 15) & 0x1f)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP4_CHANNEL26(_x) (((_x) & 0x1f) << 10)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP4_CHANNEL26_of(_x) (((_x) >> 10) & 0x1f)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP4_CHANNEL25(_x) (((_x) & 0x1f) << 5)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP4_CHANNEL25_of(_x) (((_x) >> 5) & 0x1f)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP4_CHANNEL24(_x) (((_x) & 0x1f) << 0)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP4_CHANNEL24_of(_x) (((_x) >> 0) & 0x1f)
/*----------------------------------------------------------------
  Register: NbiDmaFlowControlChannelPortMap5 - Channel to Port Map
    Bits: 29:25 Channel35(rw) - Defines the Channel to Port Mapping
    Bits: 24:20 Channel34(rw) - Defines the Channel to Port Mapping
    Bits: 19:15 Channel33(rw) - Defines the Channel to Port Mapping
    Bits: 14:10 Channel32(rw) - Defines the Channel to Port Mapping
    Bits:  9:5  Channel31(rw) - Defines the Channel to Port Mapping
    Bits:  4:0  Channel30(rw) - Defines the Channel to Port Mapping
*/
#define NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP5 0x0000020c
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP5_CHANNEL35(_x) (((_x) & 0x1f) << 25)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP5_CHANNEL35_of(_x) (((_x) >> 25) & 0x1f)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP5_CHANNEL34(_x) (((_x) & 0x1f) << 20)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP5_CHANNEL34_of(_x) (((_x) >> 20) & 0x1f)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP5_CHANNEL33(_x) (((_x) & 0x1f) << 15)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP5_CHANNEL33_of(_x) (((_x) >> 15) & 0x1f)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP5_CHANNEL32(_x) (((_x) & 0x1f) << 10)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP5_CHANNEL32_of(_x) (((_x) >> 10) & 0x1f)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP5_CHANNEL31(_x) (((_x) & 0x1f) << 5)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP5_CHANNEL31_of(_x) (((_x) >> 5) & 0x1f)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP5_CHANNEL30(_x) (((_x) & 0x1f) << 0)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP5_CHANNEL30_of(_x) (((_x) >> 0) & 0x1f)
/*----------------------------------------------------------------
  Register: NbiDmaFlowControlChannelPortMap6 - Channel to Port Map
    Bits: 29:25 Channel41(rw) - Defines the Channel to Port Mapping
    Bits: 24:20 Channel40(rw) - Defines the Channel to Port Mapping
    Bits: 19:15 Channel39(rw) - Defines the Channel to Port Mapping
    Bits: 14:10 Channel38(rw) - Defines the Channel to Port Mapping
    Bits:  9:5  Channel37(rw) - Defines the Channel to Port Mapping
    Bits:  4:0  Channel36(rw) - Defines the Channel to Port Mapping
*/
#define NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP6 0x00000210
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP6_CHANNEL41(_x) (((_x) & 0x1f) << 25)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP6_CHANNEL41_of(_x) (((_x) >> 25) & 0x1f)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP6_CHANNEL40(_x) (((_x) & 0x1f) << 20)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP6_CHANNEL40_of(_x) (((_x) >> 20) & 0x1f)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP6_CHANNEL39(_x) (((_x) & 0x1f) << 15)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP6_CHANNEL39_of(_x) (((_x) >> 15) & 0x1f)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP6_CHANNEL38(_x) (((_x) & 0x1f) << 10)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP6_CHANNEL38_of(_x) (((_x) >> 10) & 0x1f)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP6_CHANNEL37(_x) (((_x) & 0x1f) << 5)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP6_CHANNEL37_of(_x) (((_x) >> 5) & 0x1f)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP6_CHANNEL36(_x) (((_x) & 0x1f) << 0)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP6_CHANNEL36_of(_x) (((_x) >> 0) & 0x1f)
/*----------------------------------------------------------------
  Register: NbiDmaFlowControlChannelPortMap7 - Channel to Port Map
    Bits: 29:25 Channel47(rw) - Defines the Channel to Port Mapping
    Bits: 24:20 Channel46(rw) - Defines the Channel to Port Mapping
    Bits: 19:15 Channel45(rw) - Defines the Channel to Port Mapping
    Bits: 14:10 Channel44(rw) - Defines the Channel to Port Mapping
    Bits:  9:5  Channel43(rw) - Defines the Channel to Port Mapping
    Bits:  4:0  Channel42(rw) - Defines the Channel to Port Mapping
*/
#define NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP7 0x00000214
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP7_CHANNEL47(_x) (((_x) & 0x1f) << 25)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP7_CHANNEL47_of(_x) (((_x) >> 25) & 0x1f)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP7_CHANNEL46(_x) (((_x) & 0x1f) << 20)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP7_CHANNEL46_of(_x) (((_x) >> 20) & 0x1f)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP7_CHANNEL45(_x) (((_x) & 0x1f) << 15)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP7_CHANNEL45_of(_x) (((_x) >> 15) & 0x1f)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP7_CHANNEL44(_x) (((_x) & 0x1f) << 10)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP7_CHANNEL44_of(_x) (((_x) >> 10) & 0x1f)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP7_CHANNEL43(_x) (((_x) & 0x1f) << 5)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP7_CHANNEL43_of(_x) (((_x) >> 5) & 0x1f)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP7_CHANNEL42(_x) (((_x) & 0x1f) << 0)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP7_CHANNEL42_of(_x) (((_x) >> 0) & 0x1f)
/*----------------------------------------------------------------
  Register: NbiDmaFlowControlChannelPortMap8 - Channel to Port Map
    Bits: 29:25 Channel53(rw) - Defines the Channel to Port Mapping
    Bits: 24:20 Channel52(rw) - Defines the Channel to Port Mapping
    Bits: 19:15 Channel51(rw) - Defines the Channel to Port Mapping
    Bits: 14:10 Channel50(rw) - Defines the Channel to Port Mapping
    Bits:  9:5  Channel49(rw) - Defines the Channel to Port Mapping
    Bits:  4:0  Channel48(rw) - Defines the Channel to Port Mapping
*/
#define NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP8 0x00000218
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP8_CHANNEL53(_x) (((_x) & 0x1f) << 25)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP8_CHANNEL53_of(_x) (((_x) >> 25) & 0x1f)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP8_CHANNEL52(_x) (((_x) & 0x1f) << 20)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP8_CHANNEL52_of(_x) (((_x) >> 20) & 0x1f)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP8_CHANNEL51(_x) (((_x) & 0x1f) << 15)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP8_CHANNEL51_of(_x) (((_x) >> 15) & 0x1f)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP8_CHANNEL50(_x) (((_x) & 0x1f) << 10)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP8_CHANNEL50_of(_x) (((_x) >> 10) & 0x1f)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP8_CHANNEL49(_x) (((_x) & 0x1f) << 5)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP8_CHANNEL49_of(_x) (((_x) >> 5) & 0x1f)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP8_CHANNEL48(_x) (((_x) & 0x1f) << 0)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP8_CHANNEL48_of(_x) (((_x) >> 0) & 0x1f)
/*----------------------------------------------------------------
  Register: NbiDmaFlowControlChannelPortMap9 - Channel to Port Map
    Bits: 29:25 Channel59(rw) - Defines the Channel to Port Mapping
    Bits: 24:20 Channel58(rw) - Defines the Channel to Port Mapping
    Bits: 19:15 Channel57(rw) - Defines the Channel to Port Mapping
    Bits: 14:10 Channel56(rw) - Defines the Channel to Port Mapping
    Bits:  9:5  Channel55(rw) - Defines the Channel to Port Mapping
    Bits:  4:0  Channel54(rw) - Defines the Channel to Port Mapping
*/
#define NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP9 0x0000021c
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP9_CHANNEL59(_x) (((_x) & 0x1f) << 25)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP9_CHANNEL59_of(_x) (((_x) >> 25) & 0x1f)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP9_CHANNEL58(_x) (((_x) & 0x1f) << 20)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP9_CHANNEL58_of(_x) (((_x) >> 20) & 0x1f)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP9_CHANNEL57(_x) (((_x) & 0x1f) << 15)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP9_CHANNEL57_of(_x) (((_x) >> 15) & 0x1f)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP9_CHANNEL56(_x) (((_x) & 0x1f) << 10)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP9_CHANNEL56_of(_x) (((_x) >> 10) & 0x1f)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP9_CHANNEL55(_x) (((_x) & 0x1f) << 5)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP9_CHANNEL55_of(_x) (((_x) >> 5) & 0x1f)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP9_CHANNEL54(_x) (((_x) & 0x1f) << 0)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP9_CHANNEL54_of(_x) (((_x) >> 0) & 0x1f)
/*----------------------------------------------------------------
  Register: NbiDmaFlowControlChannelPortMap10 - Channel to Port Map
    Bits: 29:25 Channel65(rw) - Defines the Channel to Port Mapping
    Bits: 24:20 Channel64(rw) - Defines the Channel to Port Mapping
    Bits: 19:15 Channel63(rw) - Defines the Channel to Port Mapping
    Bits: 14:10 Channel62(rw) - Defines the Channel to Port Mapping
    Bits:  9:5  Channel61(rw) - Defines the Channel to Port Mapping
    Bits:  4:0  Channel60(rw) - Defines the Channel to Port Mapping
*/
#define NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP10 0x00000220
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP10_CHANNEL65(_x) (((_x) & 0x1f) << 25)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP10_CHANNEL65_of(_x) (((_x) >> 25) & 0x1f)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP10_CHANNEL64(_x) (((_x) & 0x1f) << 20)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP10_CHANNEL64_of(_x) (((_x) >> 20) & 0x1f)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP10_CHANNEL63(_x) (((_x) & 0x1f) << 15)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP10_CHANNEL63_of(_x) (((_x) >> 15) & 0x1f)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP10_CHANNEL62(_x) (((_x) & 0x1f) << 10)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP10_CHANNEL62_of(_x) (((_x) >> 10) & 0x1f)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP10_CHANNEL61(_x) (((_x) & 0x1f) << 5)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP10_CHANNEL61_of(_x) (((_x) >> 5) & 0x1f)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP10_CHANNEL60(_x) (((_x) & 0x1f) << 0)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP10_CHANNEL60_of(_x) (((_x) >> 0) & 0x1f)
/*----------------------------------------------------------------
  Register: NbiDmaFlowControlChannelPortMap11 - Channel to Port Map
    Bits: 29:25 Channel71(rw) - Defines the Channel to Port Mapping
    Bits: 24:20 Channel70(rw) - Defines the Channel to Port Mapping
    Bits: 19:15 Channel69(rw) - Defines the Channel to Port Mapping
    Bits: 14:10 Channel68(rw) - Defines the Channel to Port Mapping
    Bits:  9:5  Channel67(rw) - Defines the Channel to Port Mapping
    Bits:  4:0  Channel66(rw) - Defines the Channel to Port Mapping
*/
#define NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP11 0x00000224
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP11_CHANNEL71(_x) (((_x) & 0x1f) << 25)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP11_CHANNEL71_of(_x) (((_x) >> 25) & 0x1f)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP11_CHANNEL70(_x) (((_x) & 0x1f) << 20)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP11_CHANNEL70_of(_x) (((_x) >> 20) & 0x1f)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP11_CHANNEL69(_x) (((_x) & 0x1f) << 15)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP11_CHANNEL69_of(_x) (((_x) >> 15) & 0x1f)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP11_CHANNEL68(_x) (((_x) & 0x1f) << 10)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP11_CHANNEL68_of(_x) (((_x) >> 10) & 0x1f)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP11_CHANNEL67(_x) (((_x) & 0x1f) << 5)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP11_CHANNEL67_of(_x) (((_x) >> 5) & 0x1f)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP11_CHANNEL66(_x) (((_x) & 0x1f) << 0)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP11_CHANNEL66_of(_x) (((_x) >> 0) & 0x1f)
/*----------------------------------------------------------------
  Register: NbiDmaFlowControlChannelPortMap12 - Channel to Port Map
    Bits: 29:25 Channel77(rw) - Defines the Channel to Port Mapping
    Bits: 24:20 Channel76(rw) - Defines the Channel to Port Mapping
    Bits: 19:15 Channel75(rw) - Defines the Channel to Port Mapping
    Bits: 14:10 Channel74(rw) - Defines the Channel to Port Mapping
    Bits:  9:5  Channel73(rw) - Defines the Channel to Port Mapping
    Bits:  4:0  Channel72(rw) - Defines the Channel to Port Mapping
*/
#define NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP12 0x00000228
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP12_CHANNEL77(_x) (((_x) & 0x1f) << 25)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP12_CHANNEL77_of(_x) (((_x) >> 25) & 0x1f)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP12_CHANNEL76(_x) (((_x) & 0x1f) << 20)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP12_CHANNEL76_of(_x) (((_x) >> 20) & 0x1f)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP12_CHANNEL75(_x) (((_x) & 0x1f) << 15)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP12_CHANNEL75_of(_x) (((_x) >> 15) & 0x1f)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP12_CHANNEL74(_x) (((_x) & 0x1f) << 10)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP12_CHANNEL74_of(_x) (((_x) >> 10) & 0x1f)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP12_CHANNEL73(_x) (((_x) & 0x1f) << 5)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP12_CHANNEL73_of(_x) (((_x) >> 5) & 0x1f)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP12_CHANNEL72(_x) (((_x) & 0x1f) << 0)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP12_CHANNEL72_of(_x) (((_x) >> 0) & 0x1f)
/*----------------------------------------------------------------
  Register: NbiDmaFlowControlChannelPortMap13 - Channel to Port Map
    Bits: 29:25 Channel83(rw) - Defines the Channel to Port Mapping
    Bits: 24:20 Channel82(rw) - Defines the Channel to Port Mapping
    Bits: 19:15 Channel81(rw) - Defines the Channel to Port Mapping
    Bits: 14:10 Channel80(rw) - Defines the Channel to Port Mapping
    Bits:  9:5  Channel79(rw) - Defines the Channel to Port Mapping
    Bits:  4:0  Channel78(rw) - Defines the Channel to Port Mapping
*/
#define NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP13 0x0000022c
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP13_CHANNEL83(_x) (((_x) & 0x1f) << 25)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP13_CHANNEL83_of(_x) (((_x) >> 25) & 0x1f)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP13_CHANNEL82(_x) (((_x) & 0x1f) << 20)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP13_CHANNEL82_of(_x) (((_x) >> 20) & 0x1f)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP13_CHANNEL81(_x) (((_x) & 0x1f) << 15)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP13_CHANNEL81_of(_x) (((_x) >> 15) & 0x1f)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP13_CHANNEL80(_x) (((_x) & 0x1f) << 10)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP13_CHANNEL80_of(_x) (((_x) >> 10) & 0x1f)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP13_CHANNEL79(_x) (((_x) & 0x1f) << 5)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP13_CHANNEL79_of(_x) (((_x) >> 5) & 0x1f)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP13_CHANNEL78(_x) (((_x) & 0x1f) << 0)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP13_CHANNEL78_of(_x) (((_x) >> 0) & 0x1f)
/*----------------------------------------------------------------
  Register: NbiDmaFlowControlChannelPortMap14 - Channel to Port Map
    Bits: 29:25 Channel89(rw) - Defines the Channel to Port Mapping
    Bits: 24:20 Channel88(rw) - Defines the Channel to Port Mapping
    Bits: 19:15 Channel87(rw) - Defines the Channel to Port Mapping
    Bits: 14:10 Channel86(rw) - Defines the Channel to Port Mapping
    Bits:  9:5  Channel85(rw) - Defines the Channel to Port Mapping
    Bits:  4:0  Channel84(rw) - Defines the Channel to Port Mapping
*/
#define NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP14 0x00000230
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP14_CHANNEL89(_x) (((_x) & 0x1f) << 25)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP14_CHANNEL89_of(_x) (((_x) >> 25) & 0x1f)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP14_CHANNEL88(_x) (((_x) & 0x1f) << 20)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP14_CHANNEL88_of(_x) (((_x) >> 20) & 0x1f)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP14_CHANNEL87(_x) (((_x) & 0x1f) << 15)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP14_CHANNEL87_of(_x) (((_x) >> 15) & 0x1f)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP14_CHANNEL86(_x) (((_x) & 0x1f) << 10)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP14_CHANNEL86_of(_x) (((_x) >> 10) & 0x1f)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP14_CHANNEL85(_x) (((_x) & 0x1f) << 5)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP14_CHANNEL85_of(_x) (((_x) >> 5) & 0x1f)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP14_CHANNEL84(_x) (((_x) & 0x1f) << 0)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP14_CHANNEL84_of(_x) (((_x) >> 0) & 0x1f)
/*----------------------------------------------------------------
  Register: NbiDmaFlowControlChannelPortMap15 - Channel to Port Map
    Bits: 29:25 Channel95(rw) - Defines the Channel to Port Mapping
    Bits: 24:20 Channel94(rw) - Defines the Channel to Port Mapping
    Bits: 19:15 Channel93(rw) - Defines the Channel to Port Mapping
    Bits: 14:10 Channel92(rw) - Defines the Channel to Port Mapping
    Bits:  9:5  Channel91(rw) - Defines the Channel to Port Mapping
    Bits:  4:0  Channel90(rw) - Defines the Channel to Port Mapping
*/
#define NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP15 0x00000234
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP15_CHANNEL95(_x) (((_x) & 0x1f) << 25)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP15_CHANNEL95_of(_x) (((_x) >> 25) & 0x1f)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP15_CHANNEL94(_x) (((_x) & 0x1f) << 20)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP15_CHANNEL94_of(_x) (((_x) >> 20) & 0x1f)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP15_CHANNEL93(_x) (((_x) & 0x1f) << 15)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP15_CHANNEL93_of(_x) (((_x) >> 15) & 0x1f)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP15_CHANNEL92(_x) (((_x) & 0x1f) << 10)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP15_CHANNEL92_of(_x) (((_x) >> 10) & 0x1f)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP15_CHANNEL91(_x) (((_x) & 0x1f) << 5)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP15_CHANNEL91_of(_x) (((_x) >> 5) & 0x1f)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP15_CHANNEL90(_x) (((_x) & 0x1f) << 0)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP15_CHANNEL90_of(_x) (((_x) >> 0) & 0x1f)
/*----------------------------------------------------------------
  Register: NbiDmaFlowControlChannelPortMap16 - Channel to Port Map
    Bits: 29:25 Channel101(rw) - Defines the Channel to Port Mapping
    Bits: 24:20 Channel100(rw) - Defines the Channel to Port Mapping
    Bits: 19:15 Channel99(rw) - Defines the Channel to Port Mapping
    Bits: 14:10 Channel98(rw) - Defines the Channel to Port Mapping
    Bits:  9:5  Channel97(rw) - Defines the Channel to Port Mapping
    Bits:  4:0  Channel96(rw) - Defines the Channel to Port Mapping
*/
#define NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP16 0x00000238
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP16_CHANNEL101(_x) (((_x) & 0x1f) << 25)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP16_CHANNEL101_of(_x) (((_x) >> 25) & 0x1f)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP16_CHANNEL100(_x) (((_x) & 0x1f) << 20)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP16_CHANNEL100_of(_x) (((_x) >> 20) & 0x1f)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP16_CHANNEL99(_x) (((_x) & 0x1f) << 15)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP16_CHANNEL99_of(_x) (((_x) >> 15) & 0x1f)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP16_CHANNEL98(_x) (((_x) & 0x1f) << 10)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP16_CHANNEL98_of(_x) (((_x) >> 10) & 0x1f)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP16_CHANNEL97(_x) (((_x) & 0x1f) << 5)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP16_CHANNEL97_of(_x) (((_x) >> 5) & 0x1f)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP16_CHANNEL96(_x) (((_x) & 0x1f) << 0)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP16_CHANNEL96_of(_x) (((_x) >> 0) & 0x1f)
/*----------------------------------------------------------------
  Register: NbiDmaFlowControlChannelPortMap17 - Channel to Port Map
    Bits: 29:25 Channel107(rw) - Defines the Channel to Port Mapping
    Bits: 24:20 Channel106(rw) - Defines the Channel to Port Mapping
    Bits: 19:15 Channel105(rw) - Defines the Channel to Port Mapping
    Bits: 14:10 Channel104(rw) - Defines the Channel to Port Mapping
    Bits:  9:5  Channel103(rw) - Defines the Channel to Port Mapping
    Bits:  4:0  Channel102(rw) - Defines the Channel to Port Mapping
*/
#define NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP17 0x0000023c
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP17_CHANNEL107(_x) (((_x) & 0x1f) << 25)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP17_CHANNEL107_of(_x) (((_x) >> 25) & 0x1f)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP17_CHANNEL106(_x) (((_x) & 0x1f) << 20)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP17_CHANNEL106_of(_x) (((_x) >> 20) & 0x1f)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP17_CHANNEL105(_x) (((_x) & 0x1f) << 15)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP17_CHANNEL105_of(_x) (((_x) >> 15) & 0x1f)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP17_CHANNEL104(_x) (((_x) & 0x1f) << 10)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP17_CHANNEL104_of(_x) (((_x) >> 10) & 0x1f)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP17_CHANNEL103(_x) (((_x) & 0x1f) << 5)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP17_CHANNEL103_of(_x) (((_x) >> 5) & 0x1f)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP17_CHANNEL102(_x) (((_x) & 0x1f) << 0)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP17_CHANNEL102_of(_x) (((_x) >> 0) & 0x1f)
/*----------------------------------------------------------------
  Register: NbiDmaFlowControlChannelPortMap18 - Channel to Port Map
    Bits: 29:25 Channel113(rw) - Defines the Channel to Port Mapping
    Bits: 24:20 Channel112(rw) - Defines the Channel to Port Mapping
    Bits: 19:15 Channel111(rw) - Defines the Channel to Port Mapping
    Bits: 14:10 Channel110(rw) - Defines the Channel to Port Mapping
    Bits:  9:5  Channel109(rw) - Defines the Channel to Port Mapping
    Bits:  4:0  Channel108(rw) - Defines the Channel to Port Mapping
*/
#define NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP18 0x00000240
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP18_CHANNEL113(_x) (((_x) & 0x1f) << 25)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP18_CHANNEL113_of(_x) (((_x) >> 25) & 0x1f)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP18_CHANNEL112(_x) (((_x) & 0x1f) << 20)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP18_CHANNEL112_of(_x) (((_x) >> 20) & 0x1f)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP18_CHANNEL111(_x) (((_x) & 0x1f) << 15)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP18_CHANNEL111_of(_x) (((_x) >> 15) & 0x1f)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP18_CHANNEL110(_x) (((_x) & 0x1f) << 10)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP18_CHANNEL110_of(_x) (((_x) >> 10) & 0x1f)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP18_CHANNEL109(_x) (((_x) & 0x1f) << 5)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP18_CHANNEL109_of(_x) (((_x) >> 5) & 0x1f)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP18_CHANNEL108(_x) (((_x) & 0x1f) << 0)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP18_CHANNEL108_of(_x) (((_x) >> 0) & 0x1f)
/*----------------------------------------------------------------
  Register: NbiDmaFlowControlChannelPortMap19 - Channel to Port Map
    Bits: 29:25 Channel119(rw) - Defines the Channel to Port Mapping
    Bits: 24:20 Channel118(rw) - Defines the Channel to Port Mapping
    Bits: 19:15 Channel117(rw) - Defines the Channel to Port Mapping
    Bits: 14:10 Channel116(rw) - Defines the Channel to Port Mapping
    Bits:  9:5  Channel115(rw) - Defines the Channel to Port Mapping
    Bits:  4:0  Channel114(rw) - Defines the Channel to Port Mapping
*/
#define NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP19 0x00000244
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP19_CHANNEL119(_x) (((_x) & 0x1f) << 25)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP19_CHANNEL119_of(_x) (((_x) >> 25) & 0x1f)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP19_CHANNEL118(_x) (((_x) & 0x1f) << 20)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP19_CHANNEL118_of(_x) (((_x) >> 20) & 0x1f)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP19_CHANNEL117(_x) (((_x) & 0x1f) << 15)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP19_CHANNEL117_of(_x) (((_x) >> 15) & 0x1f)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP19_CHANNEL116(_x) (((_x) & 0x1f) << 10)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP19_CHANNEL116_of(_x) (((_x) >> 10) & 0x1f)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP19_CHANNEL115(_x) (((_x) & 0x1f) << 5)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP19_CHANNEL115_of(_x) (((_x) >> 5) & 0x1f)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP19_CHANNEL114(_x) (((_x) & 0x1f) << 0)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP19_CHANNEL114_of(_x) (((_x) >> 0) & 0x1f)
/*----------------------------------------------------------------
  Register: NbiDmaFlowControlChannelPortMap20 - Channel to Port Map
    Bits: 29:25 Channel125(rw) - Defines the Channel to Port Mapping
    Bits: 24:20 Channel124(rw) - Defines the Channel to Port Mapping
    Bits: 19:15 Channel123(rw) - Defines the Channel to Port Mapping
    Bits: 14:10 Channel122(rw) - Defines the Channel to Port Mapping
    Bits:  9:5  Channel121(rw) - Defines the Channel to Port Mapping
    Bits:  4:0  Channel120(rw) - Defines the Channel to Port Mapping
*/
#define NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP20 0x00000248
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP20_CHANNEL125(_x) (((_x) & 0x1f) << 25)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP20_CHANNEL125_of(_x) (((_x) >> 25) & 0x1f)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP20_CHANNEL124(_x) (((_x) & 0x1f) << 20)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP20_CHANNEL124_of(_x) (((_x) >> 20) & 0x1f)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP20_CHANNEL123(_x) (((_x) & 0x1f) << 15)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP20_CHANNEL123_of(_x) (((_x) >> 15) & 0x1f)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP20_CHANNEL122(_x) (((_x) & 0x1f) << 10)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP20_CHANNEL122_of(_x) (((_x) >> 10) & 0x1f)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP20_CHANNEL121(_x) (((_x) & 0x1f) << 5)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP20_CHANNEL121_of(_x) (((_x) >> 5) & 0x1f)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP20_CHANNEL120(_x) (((_x) & 0x1f) << 0)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP20_CHANNEL120_of(_x) (((_x) >> 0) & 0x1f)
/*----------------------------------------------------------------
  Register: NbiDmaFlowControlChannelPortMap21 - Channel to Port Map
    Bits:  9:5  Channel127(rw) - Defines the Channel to Port Mapping
    Bits:  4:0  Channel126(rw) - Defines the Channel to Port Mapping
*/
#define NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP21 0x0000024c
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP21_CHANNEL127(_x) (((_x) & 0x1f) << 5)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP21_CHANNEL127_of(_x) (((_x) >> 5) & 0x1f)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP21_CHANNEL126(_x) (((_x) & 0x1f) << 0)
#define   NFP3800_NBI_DMAX_CSR_NBI_DMA_FLOW_CONTROL_CHANNEL_PORT_MAP21_CHANNEL126_of(_x) (((_x) >> 0) & 0x1f)

#else /* NFP3800_LONGNAMES */

/*----------------------------------------------------------------
  Register: NbiDmaBDSramEntry[2048] - NBI DMA Buffer Descriptor SRAM Entry X
    Bits: 28:0  MuAddr(rw) - Memory Unit Address of the buffer (internal/external MU). Maps to CPP Address[29;11] Refer to the NBI top-level Master Commands section for memory unit address translation.
*/
#define NFP3800_NBI_DMA_BDSRAM_NbiDmaBDSramEntry(_x)         (0x00000000 + (0x8 * ((_x) & 0x7ff)))
#define   NFP3800_NBI_DMA_BDSRAM_NbiDmaBDSramEntry_MuAddr(_x) (((_x) & 0x1fffffff) << 0)
#define   NFP3800_NBI_DMA_BDSRAM_NbiDmaBDSramEntry_MuAddr_of(_x) (((_x) >> 0) & 0x1fffffff)
/*----------------------------------------------------------------
  Register: BLQueCtrl0 - Configures the buffer list circular queue and provides status
    Bits: 55:48 BLQEventFullCnt(rc) - Debug counter that increments when a BLQ Event is dropped due to a full BLQ Event FIFO. This counter saturates at all 1's
    Bits: 42:40 BDFifoEntryCnt(ro) - Number of buffer descriptors resident in the buffer descriptor register FIFO
    Bits: 39    BLQFull(ro) - The Queue is full when set.
    Bits: 38    BLQEmpty(ro) - The queue is empty when set.
    Bits: 37:36 BLQSize(rw) - Defines the Size of the Circular Buffer List Queue
    Bits: 34:24 BLEntryCnt(ro) - Hardware calculated value of the number of entries on the Buffer List Queue. A full Buffer List Queue has an entry count equal to zero, with the BLQFull flag set. An empty Buffer List Queue has an entry count equal to zero with the BLQEmpty flag set.
    Bits: 22:12 BLHdPtr(rw) - Buffer List Queue Head Pointer. Points to the next entry to be read by hardware.
    Bits: 10:0  BLTlPtr(rw) - Buffer List Queue Tail Pointer. Points to the next entry to be written at the tail.
*/
#define NFP3800_NBI_DMA_BufferList_BLQueCtrl0                0x00008000
#define   NFP3800_NBI_DMA_BufferList_BLQueCtrl0_BLQEventFullCnt(_x) (((_x) & 0xff) << 48)
#define   NFP3800_NBI_DMA_BufferList_BLQueCtrl0_BLQEventFullCnt_of(_x) (((_x) >> 48) & 0xff)
#define   NFP3800_NBI_DMA_BufferList_BLQueCtrl0_BDFifoEntryCnt(_x) (((_x) & 0x7) << 40)
#define   NFP3800_NBI_DMA_BufferList_BLQueCtrl0_BDFifoEntryCnt_of(_x) (((_x) >> 40) & 0x7)
#define   NFP3800_NBI_DMA_BufferList_BLQueCtrl0_BLQFull      BIT_ULL(39)
#define   NFP3800_NBI_DMA_BufferList_BLQueCtrl0_BLQEmpty     BIT_ULL(38)
#define   NFP3800_NBI_DMA_BufferList_BLQueCtrl0_BLQSize(_x)  (((_x) & 0x3) << 36)
#define   NFP3800_NBI_DMA_BufferList_BLQueCtrl0_BLQSize_of(_x) (((_x) >> 36) & 0x3)
#define     NFP3800_NBI_DMA_BufferList_BLQueCtrl0_BLQSize_256 (0)
#define     NFP3800_NBI_DMA_BufferList_BLQueCtrl0_BLQSize_512 (1)
#define     NFP3800_NBI_DMA_BufferList_BLQueCtrl0_BLQSize_1K (2)
#define     NFP3800_NBI_DMA_BufferList_BLQueCtrl0_BLQSize_2K (3)
#define   NFP3800_NBI_DMA_BufferList_BLQueCtrl0_BLEntryCnt(_x) (((_x) & 0x7ff) << 24)
#define   NFP3800_NBI_DMA_BufferList_BLQueCtrl0_BLEntryCnt_of(_x) (((_x) >> 24) & 0x7ff)
#define   NFP3800_NBI_DMA_BufferList_BLQueCtrl0_BLHdPtr(_x)  (((_x) & 0x7ff) << 12)
#define   NFP3800_NBI_DMA_BufferList_BLQueCtrl0_BLHdPtr_of(_x) (((_x) >> 12) & 0x7ff)
#define   NFP3800_NBI_DMA_BufferList_BLQueCtrl0_BLTlPtr(_x)  (((_x) & 0x7ff) << 0)
#define   NFP3800_NBI_DMA_BufferList_BLQueCtrl0_BLTlPtr_of(_x) (((_x) >> 0) & 0x7ff)
/*----------------------------------------------------------------
  Register: BLQueCtrl1 - Configures the buffer list circular queue and provides status
    Bits: 55:48 BLQEventFullCnt(rc) - Debug counter that increments when a BLQ Event is dropped due to a full BLQ Event FIFO. This counter saturates at all 1's
    Bits: 42:40 BDFifoEntryCnt(ro) - Number of buffer descriptors resident in the buffer descriptor register FIFO
    Bits: 39    BLQFull(ro) - The Queue is full when set.
    Bits: 38    BLQEmpty(ro) - The queue is empty when set.
    Bits: 37:36 BLQSize(rw) - Defines the Size of the Circular Buffer List Queue
    Bits: 34:24 BLEntryCnt(ro) - Hardware calculated value of the number of entries on the Buffer List Queue. A full Buffer List Queue has an entry count equal to zero, with the BLQFull flag set. An empty Buffer List Queue has an entry count equal to zero with the BLQEmpty flag set.
    Bits: 22:12 BLHdPtr(rw) - Buffer List Queue Head Pointer. Points to the next entry to be read by hardware.
    Bits: 10:0  BLTlPtr(rw) - Buffer List Queue Tail Pointer. Points to the next entry to be written at the tail.
*/
#define NFP3800_NBI_DMA_BufferList_BLQueCtrl1                0x00008008
#define   NFP3800_NBI_DMA_BufferList_BLQueCtrl1_BLQEventFullCnt(_x) (((_x) & 0xff) << 48)
#define   NFP3800_NBI_DMA_BufferList_BLQueCtrl1_BLQEventFullCnt_of(_x) (((_x) >> 48) & 0xff)
#define   NFP3800_NBI_DMA_BufferList_BLQueCtrl1_BDFifoEntryCnt(_x) (((_x) & 0x7) << 40)
#define   NFP3800_NBI_DMA_BufferList_BLQueCtrl1_BDFifoEntryCnt_of(_x) (((_x) >> 40) & 0x7)
#define   NFP3800_NBI_DMA_BufferList_BLQueCtrl1_BLQFull      BIT_ULL(39)
#define   NFP3800_NBI_DMA_BufferList_BLQueCtrl1_BLQEmpty     BIT_ULL(38)
#define   NFP3800_NBI_DMA_BufferList_BLQueCtrl1_BLQSize(_x)  (((_x) & 0x3) << 36)
#define   NFP3800_NBI_DMA_BufferList_BLQueCtrl1_BLQSize_of(_x) (((_x) >> 36) & 0x3)
#define     NFP3800_NBI_DMA_BufferList_BLQueCtrl1_BLQSize_256 (0)
#define     NFP3800_NBI_DMA_BufferList_BLQueCtrl1_BLQSize_512 (1)
#define     NFP3800_NBI_DMA_BufferList_BLQueCtrl1_BLQSize_1K (2)
#define     NFP3800_NBI_DMA_BufferList_BLQueCtrl1_BLQSize_2K (3)
#define   NFP3800_NBI_DMA_BufferList_BLQueCtrl1_BLEntryCnt(_x) (((_x) & 0x7ff) << 24)
#define   NFP3800_NBI_DMA_BufferList_BLQueCtrl1_BLEntryCnt_of(_x) (((_x) >> 24) & 0x7ff)
#define   NFP3800_NBI_DMA_BufferList_BLQueCtrl1_BLHdPtr(_x)  (((_x) & 0x7ff) << 12)
#define   NFP3800_NBI_DMA_BufferList_BLQueCtrl1_BLHdPtr_of(_x) (((_x) >> 12) & 0x7ff)
#define   NFP3800_NBI_DMA_BufferList_BLQueCtrl1_BLTlPtr(_x)  (((_x) & 0x7ff) << 0)
#define   NFP3800_NBI_DMA_BufferList_BLQueCtrl1_BLTlPtr_of(_x) (((_x) >> 0) & 0x7ff)
/*----------------------------------------------------------------
  Register: BLQueCtrl2 - Configures the buffer list circular queue and provides status
    Bits: 55:48 BLQEventFullCnt(rc) - Debug counter that increments when a BLQ Event is dropped due to a full BLQ Event FIFO. This counter saturates at all 1's
    Bits: 42:40 BDFifoEntryCnt(ro) - Number of buffer descriptors resident in the buffer descriptor register FIFO
    Bits: 39    BLQFull(ro) - The Queue is full when set.
    Bits: 38    BLQEmpty(ro) - The queue is empty when set.
    Bits: 37:36 BLQSize(rw) - Defines the Size of the Circular Buffer List Queue
    Bits: 34:24 BLEntryCnt(ro) - Hardware calculated value of the number of entries on the Buffer List Queue. A full Buffer List Queue has an entry count equal to zero, with the BLQFull flag set. An empty Buffer List Queue has an entry count equal to zero with the BLQEmpty flag set.
    Bits: 22:12 BLHdPtr(rw) - Buffer List Queue Head Pointer. Points to the next entry to be read by hardware.
    Bits: 10:0  BLTlPtr(rw) - Buffer List Queue Tail Pointer. Points to the next entry to be written at the tail.
*/
#define NFP3800_NBI_DMA_BufferList_BLQueCtrl2                0x00008010
#define   NFP3800_NBI_DMA_BufferList_BLQueCtrl2_BLQEventFullCnt(_x) (((_x) & 0xff) << 48)
#define   NFP3800_NBI_DMA_BufferList_BLQueCtrl2_BLQEventFullCnt_of(_x) (((_x) >> 48) & 0xff)
#define   NFP3800_NBI_DMA_BufferList_BLQueCtrl2_BDFifoEntryCnt(_x) (((_x) & 0x7) << 40)
#define   NFP3800_NBI_DMA_BufferList_BLQueCtrl2_BDFifoEntryCnt_of(_x) (((_x) >> 40) & 0x7)
#define   NFP3800_NBI_DMA_BufferList_BLQueCtrl2_BLQFull      BIT_ULL(39)
#define   NFP3800_NBI_DMA_BufferList_BLQueCtrl2_BLQEmpty     BIT_ULL(38)
#define   NFP3800_NBI_DMA_BufferList_BLQueCtrl2_BLQSize(_x)  (((_x) & 0x3) << 36)
#define   NFP3800_NBI_DMA_BufferList_BLQueCtrl2_BLQSize_of(_x) (((_x) >> 36) & 0x3)
#define     NFP3800_NBI_DMA_BufferList_BLQueCtrl2_BLQSize_256 (0)
#define     NFP3800_NBI_DMA_BufferList_BLQueCtrl2_BLQSize_512 (1)
#define     NFP3800_NBI_DMA_BufferList_BLQueCtrl2_BLQSize_1K (2)
#define     NFP3800_NBI_DMA_BufferList_BLQueCtrl2_BLQSize_2K (3)
#define   NFP3800_NBI_DMA_BufferList_BLQueCtrl2_BLEntryCnt(_x) (((_x) & 0x7ff) << 24)
#define   NFP3800_NBI_DMA_BufferList_BLQueCtrl2_BLEntryCnt_of(_x) (((_x) >> 24) & 0x7ff)
#define   NFP3800_NBI_DMA_BufferList_BLQueCtrl2_BLHdPtr(_x)  (((_x) & 0x7ff) << 12)
#define   NFP3800_NBI_DMA_BufferList_BLQueCtrl2_BLHdPtr_of(_x) (((_x) >> 12) & 0x7ff)
#define   NFP3800_NBI_DMA_BufferList_BLQueCtrl2_BLTlPtr(_x)  (((_x) & 0x7ff) << 0)
#define   NFP3800_NBI_DMA_BufferList_BLQueCtrl2_BLTlPtr_of(_x) (((_x) >> 0) & 0x7ff)
/*----------------------------------------------------------------
  Register: BLQueCtrl3 - Configures the buffer list circular queue and provides status
    Bits: 55:48 BLQEventFullCnt(rc) - Debug counter that increments when a BLQ Event is dropped due to a full BLQ Event FIFO. This counter saturates at all 1's
    Bits: 42:40 BDFifoEntryCnt(ro) - Number of buffer descriptors resident in the buffer descriptor register FIFO
    Bits: 39    BLQFull(ro) - The Queue is full when set.
    Bits: 38    BLQEmpty(ro) - The queue is empty when set.
    Bits: 37:36 BLQSize(rw) - Defines the Size of the Circular Buffer List Queue
    Bits: 34:24 BLEntryCnt(ro) - Hardware calculated value of the number of entries on the Buffer List Queue. A full Buffer List Queue has an entry count equal to zero, with the BLQFull flag set. An empty Buffer List Queue has an entry count equal to zero with the BLQEmpty flag set.
    Bits: 22:12 BLHdPtr(rw) - Buffer List Queue Head Pointer. Points to the next entry to be read by hardware.
    Bits: 10:0  BLTlPtr(rw) - Buffer List Queue Tail Pointer. Points to the next entry to be written at the tail.
*/
#define NFP3800_NBI_DMA_BufferList_BLQueCtrl3                0x00008018
#define   NFP3800_NBI_DMA_BufferList_BLQueCtrl3_BLQEventFullCnt(_x) (((_x) & 0xff) << 48)
#define   NFP3800_NBI_DMA_BufferList_BLQueCtrl3_BLQEventFullCnt_of(_x) (((_x) >> 48) & 0xff)
#define   NFP3800_NBI_DMA_BufferList_BLQueCtrl3_BDFifoEntryCnt(_x) (((_x) & 0x7) << 40)
#define   NFP3800_NBI_DMA_BufferList_BLQueCtrl3_BDFifoEntryCnt_of(_x) (((_x) >> 40) & 0x7)
#define   NFP3800_NBI_DMA_BufferList_BLQueCtrl3_BLQFull      BIT_ULL(39)
#define   NFP3800_NBI_DMA_BufferList_BLQueCtrl3_BLQEmpty     BIT_ULL(38)
#define   NFP3800_NBI_DMA_BufferList_BLQueCtrl3_BLQSize(_x)  (((_x) & 0x3) << 36)
#define   NFP3800_NBI_DMA_BufferList_BLQueCtrl3_BLQSize_of(_x) (((_x) >> 36) & 0x3)
#define     NFP3800_NBI_DMA_BufferList_BLQueCtrl3_BLQSize_256 (0)
#define     NFP3800_NBI_DMA_BufferList_BLQueCtrl3_BLQSize_512 (1)
#define     NFP3800_NBI_DMA_BufferList_BLQueCtrl3_BLQSize_1K (2)
#define     NFP3800_NBI_DMA_BufferList_BLQueCtrl3_BLQSize_2K (3)
#define   NFP3800_NBI_DMA_BufferList_BLQueCtrl3_BLEntryCnt(_x) (((_x) & 0x7ff) << 24)
#define   NFP3800_NBI_DMA_BufferList_BLQueCtrl3_BLEntryCnt_of(_x) (((_x) >> 24) & 0x7ff)
#define   NFP3800_NBI_DMA_BufferList_BLQueCtrl3_BLHdPtr(_x)  (((_x) & 0x7ff) << 12)
#define   NFP3800_NBI_DMA_BufferList_BLQueCtrl3_BLHdPtr_of(_x) (((_x) >> 12) & 0x7ff)
#define   NFP3800_NBI_DMA_BufferList_BLQueCtrl3_BLTlPtr(_x)  (((_x) & 0x7ff) << 0)
#define   NFP3800_NBI_DMA_BufferList_BLQueCtrl3_BLTlPtr_of(_x) (((_x) >> 0) & 0x7ff)
/*----------------------------------------------------------------
  Register: NbiDmaBCSramEntry[1024] - NBI DMA Buffer Completion SRAM Entry X
    Bits: 31:24 bufNum3(rw) - Buffer number 3
    Bits: 23:16 bufNum2(rw) - Buffer number 2
    Bits: 15:8  bufNum1(rw) - Buffer number 1
    Bits:  7:0  bufNum0(rw) - Buffer number 0
*/
#define NFP3800_NBI_DMA_BCSRAM_NbiDmaBCSramEntry(_x)         (0x0000a000 + (0x8 * ((_x) & 0x3ff)))
#define   NFP3800_NBI_DMA_BCSRAM_NbiDmaBCSramEntry_bufNum3(_x) (((_x) & 0xff) << 24)
#define   NFP3800_NBI_DMA_BCSRAM_NbiDmaBCSramEntry_bufNum3_of(_x) (((_x) >> 24) & 0xff)
#define   NFP3800_NBI_DMA_BCSRAM_NbiDmaBCSramEntry_bufNum2(_x) (((_x) & 0xff) << 16)
#define   NFP3800_NBI_DMA_BCSRAM_NbiDmaBCSramEntry_bufNum2_of(_x) (((_x) >> 16) & 0xff)
#define   NFP3800_NBI_DMA_BCSRAM_NbiDmaBCSramEntry_bufNum1(_x) (((_x) & 0xff) << 8)
#define   NFP3800_NBI_DMA_BCSRAM_NbiDmaBCSramEntry_bufNum1_of(_x) (((_x) >> 8) & 0xff)
#define   NFP3800_NBI_DMA_BCSRAM_NbiDmaBCSramEntry_bufNum0(_x) (((_x) & 0xff) << 0)
#define   NFP3800_NBI_DMA_BCSRAM_NbiDmaBCSramEntry_bufNum0_of(_x) (((_x) >> 0) & 0xff)
/*----------------------------------------------------------------
  Register: NbiDmaCfg - NBI DMA Configuration register
    Bits: 30:29 NegativeAlloc256BCr(rw) - In Allocation Lookahead mode, this is the threshold for the threshold for the outstanding 256B credits. Each credit is 256B.
    Bits: 28:27 BufferCreditThresh(rw) - In Allocation Lookahead mode, the Buffer Credit is allowed to go negative by this number.
    Bits: 26    AllocLookAheadEna(rw) - When set, it enables the Allocation Lookahead mode allowing the Buffer Credit to go 4 negative.
    Bits: 25    PacketColorEnable(rw) - When set, the NBI DMA adds 2-bit packet color in Picoengine metadata based on the CTM's credit level.
    Bits: 24    SeqNumOverride(rw) - When set, the NBI DMA overrides the Sequence Numbers in the PicoEngine metadata to avoid holes due to drops
    Bits: 23    PollRespNoWaitEna(rw) - When set, CTM Polling command responses are processed as they arrive. When cleared, CTM Polling command responses are processed after all outstanding responses have arrived
    Bits: 21    EnaEnhCtmOffset(rw) - Controls how the CtmOffset field of the NbiDmaBPCfg registers operates.
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
    Bits:  5:4  CtmPollIntvl(rw) - Configures the minimum amount of time between packet allocation poll commands in clock cycles.
    Bits:  3:2  PollCmdIssueRate(rw) - Configures the minimum space between CPP Polling commands issued after the polling interval has expired
    Bits:  1    RateLimitEnable(rw) - Selects whether to limit the bandwidth on the four Push Buses.
    Bits:  0    PollSearchEna(rw) - Enable searching for next CTM to poll
*/
#define NFP3800_NBI_DMAX_CSR_NbiDmaCfg                       0x00000000
#define   NFP3800_NBI_DMAX_CSR_NbiDmaCfg_NegativeAlloc256BCr(_x) (((_x) & 0x3) << 29)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaCfg_NegativeAlloc256BCr_of(_x) (((_x) >> 29) & 0x3)
#define     NFP3800_NBI_DMAX_CSR_NbiDmaCfg_NegativeAlloc256BCr_16 (0)
#define     NFP3800_NBI_DMAX_CSR_NbiDmaCfg_NegativeAlloc256BCr_32 (1)
#define     NFP3800_NBI_DMAX_CSR_NbiDmaCfg_NegativeAlloc256BCr_48 (2)
#define     NFP3800_NBI_DMAX_CSR_NbiDmaCfg_NegativeAlloc256BCr_64 (3)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaCfg_BufferCreditThresh(_x) (((_x) & 0x3) << 27)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaCfg_BufferCreditThresh_of(_x) (((_x) >> 27) & 0x3)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaCfg_AllocLookAheadEna   BIT(26)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaCfg_PacketColorEnable   BIT(25)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaCfg_SeqNumOverride      BIT(24)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaCfg_PollRespNoWaitEna   BIT(23)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaCfg_EnaEnhCtmOffset     BIT(21)
#define     NFP3800_NBI_DMAX_CSR_NbiDmaCfg_EnaEnhCtmOffset_CtmOffsetCompatabilityMode (0 << 21)
#define     NFP3800_NBI_DMAX_CSR_NbiDmaCfg_EnaEnhCtmOffset_CtmOffsetEnhancedMode BIT(21)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaCfg_DisPushBusSelect(_x) (((_x) & 0xf) << 17)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaCfg_DisPushBusSelect_of(_x) (((_x) >> 17) & 0xf)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaCfg_DisBcRamErr         BIT(16)
#define     NFP3800_NBI_DMAX_CSR_NbiDmaCfg_DisBcRamErr_Enable (0 << 16)
#define     NFP3800_NBI_DMAX_CSR_NbiDmaCfg_DisBcRamErr_Disable BIT(16)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaCfg_DisDsRamErr         BIT(15)
#define     NFP3800_NBI_DMAX_CSR_NbiDmaCfg_DisDsRamErr_Enable (0 << 15)
#define     NFP3800_NBI_DMAX_CSR_NbiDmaCfg_DisDsRamErr_Disable BIT(15)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaCfg_DisBdRamErr         BIT(14)
#define     NFP3800_NBI_DMAX_CSR_NbiDmaCfg_DisBdRamErr_Enable (0 << 14)
#define     NFP3800_NBI_DMAX_CSR_NbiDmaCfg_DisBdRamErr_Disable BIT(14)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaCfg_DisBufRdErr         BIT(13)
#define     NFP3800_NBI_DMAX_CSR_NbiDmaCfg_DisBufRdErr_Enable (0 << 13)
#define     NFP3800_NBI_DMAX_CSR_NbiDmaCfg_DisBufRdErr_Disable BIT(13)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaCfg_DisRxPushLastErr    BIT(12)
#define     NFP3800_NBI_DMAX_CSR_NbiDmaCfg_DisRxPushLastErr_Enable (0 << 12)
#define     NFP3800_NBI_DMAX_CSR_NbiDmaCfg_DisRxPushLastErr_Disable BIT(12)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaCfg_DisRxAllocInErr     BIT(11)
#define     NFP3800_NBI_DMAX_CSR_NbiDmaCfg_DisRxAllocInErr_Enable (0 << 11)
#define     NFP3800_NBI_DMAX_CSR_NbiDmaCfg_DisRxAllocInErr_Disable BIT(11)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaCfg_DisRxBlqWrInErr     BIT(10)
#define     NFP3800_NBI_DMAX_CSR_NbiDmaCfg_DisRxBlqWrInErr_Enable (0 << 10)
#define     NFP3800_NBI_DMAX_CSR_NbiDmaCfg_DisRxBlqWrInErr_Disable BIT(10)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaCfg_NbiNum(_x)          (((_x) & 0x7) << 7)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaCfg_NbiNum_of(_x)       (((_x) >> 7) & 0x7)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaCfg_CtmPollEna          BIT(6)
#define     NFP3800_NBI_DMAX_CSR_NbiDmaCfg_CtmPollEna_Disable (0 << 6)
#define     NFP3800_NBI_DMAX_CSR_NbiDmaCfg_CtmPollEna_Enable BIT(6)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaCfg_CtmPollIntvl(_x)    (((_x) & 0x3) << 4)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaCfg_CtmPollIntvl_of(_x) (((_x) >> 4) & 0x3)
#define     NFP3800_NBI_DMAX_CSR_NbiDmaCfg_CtmPollIntvl_128  (0)
#define     NFP3800_NBI_DMAX_CSR_NbiDmaCfg_CtmPollIntvl_256  (1)
#define     NFP3800_NBI_DMAX_CSR_NbiDmaCfg_CtmPollIntvl_512  (2)
#define     NFP3800_NBI_DMAX_CSR_NbiDmaCfg_CtmPollIntvl_1024 (3)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaCfg_PollCmdIssueRate(_x) (((_x) & 0x3) << 2)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaCfg_PollCmdIssueRate_of(_x) (((_x) >> 2) & 0x3)
#define     NFP3800_NBI_DMAX_CSR_NbiDmaCfg_PollCmdIssueRate_4 (0)
#define     NFP3800_NBI_DMAX_CSR_NbiDmaCfg_PollCmdIssueRate_8 (1)
#define     NFP3800_NBI_DMAX_CSR_NbiDmaCfg_PollCmdIssueRate_12 (2)
#define     NFP3800_NBI_DMAX_CSR_NbiDmaCfg_PollCmdIssueRate_16 (3)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaCfg_RateLimitEnable     BIT(1)
#define     NFP3800_NBI_DMAX_CSR_NbiDmaCfg_RateLimitEnable_Disable (0 << 1)
#define     NFP3800_NBI_DMAX_CSR_NbiDmaCfg_RateLimitEnable_Enable BIT(1)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaCfg_PollSearchEna       BIT(0)
/*----------------------------------------------------------------
  Register: NbiDmaBlqEvent - NBI DMA Buffer List Queue Event Control
    Bits: 22    ReducedThresh(rw) - If set lowers the blq event thresholds to 4, 8, 16 and 32.
    Bits: 20    DisBLQEventFifoFullAssert(rw) - Set this bit to disable the BLQ Event FIFO overflow assertion
    Bits: 19:18 BLQEventThreshold3(rw) - An event is generated every N buffer descriptors written to buffer list queue3, where N is configurable from 16 to 128 or 4 to 32 if Reduced BLQ Threshold is set..
    Bits: 17:16 BLQEventThreshold2(rw) - An event is generated every N buffer descriptors written to buffer list queue3, where N is configurable from 16 to 128 or 4 to 32 if Reduced BLQ Threshold is set..
    Bits: 15:14 BLQEventThreshold1(rw) - An event is generated every N buffer descriptors written to buffer list queue3, where N is configurable from 16 to 128 or 4 to 32 if Reduced BLQ Threshold is set..
    Bits: 13:12 BLQEventThreshold0(rw) - An event is generated every N buffer descriptors written to buffer list queue3, where N is configurable from 16 to 128 or 4 to 32 if Reduced BLQ Threshold is set..
    Bits:  7:4  BLQEventNotFullEnable(rw) - One bit per buffer list queue. Enables the generation of BLQ Events when the queue transitions from full to not full. Disabled when clear. The event type value used is 0x1 (event_type_fifo_not_full)
    Bits:  3:0  BLQEventStatusEnable(rw) - One bit per buffer list queue. Enables BLQ Status Events when set. Disabled when clear. A Status Event is sent when the number of entries read from the queue reaches the programmed value in the BLQEventThreshold register. The event type value used is 0x5 (event_type_value_update)
*/
#define NFP3800_NBI_DMAX_CSR_NbiDmaBlqEvent                  0x0000000c
#define   NFP3800_NBI_DMAX_CSR_NbiDmaBlqEvent_ReducedThresh  BIT(22)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaBlqEvent_DisBLQEventFifoFullAssert BIT(20)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaBlqEvent_BLQEventThreshold3(_x) (((_x) & 0x3) << 18)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaBlqEvent_BLQEventThreshold3_of(_x) (((_x) >> 18) & 0x3)
#define     NFP3800_NBI_DMAX_CSR_NbiDmaBlqEvent_BLQEventThreshold3_16_or_4 (0)
#define     NFP3800_NBI_DMAX_CSR_NbiDmaBlqEvent_BLQEventThreshold3_32_or_8 (1)
#define     NFP3800_NBI_DMAX_CSR_NbiDmaBlqEvent_BLQEventThreshold3_64_or_16 (2)
#define     NFP3800_NBI_DMAX_CSR_NbiDmaBlqEvent_BLQEventThreshold3_128_or_32 (3)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaBlqEvent_BLQEventThreshold2(_x) (((_x) & 0x3) << 16)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaBlqEvent_BLQEventThreshold2_of(_x) (((_x) >> 16) & 0x3)
#define     NFP3800_NBI_DMAX_CSR_NbiDmaBlqEvent_BLQEventThreshold2_16_or_4 (0)
#define     NFP3800_NBI_DMAX_CSR_NbiDmaBlqEvent_BLQEventThreshold2_32_or_8 (1)
#define     NFP3800_NBI_DMAX_CSR_NbiDmaBlqEvent_BLQEventThreshold2_64_or_16 (2)
#define     NFP3800_NBI_DMAX_CSR_NbiDmaBlqEvent_BLQEventThreshold2_128_or_32 (3)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaBlqEvent_BLQEventThreshold1(_x) (((_x) & 0x3) << 14)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaBlqEvent_BLQEventThreshold1_of(_x) (((_x) >> 14) & 0x3)
#define     NFP3800_NBI_DMAX_CSR_NbiDmaBlqEvent_BLQEventThreshold1_16_or_4 (0)
#define     NFP3800_NBI_DMAX_CSR_NbiDmaBlqEvent_BLQEventThreshold1_32_or_8 (1)
#define     NFP3800_NBI_DMAX_CSR_NbiDmaBlqEvent_BLQEventThreshold1_64_or_16 (2)
#define     NFP3800_NBI_DMAX_CSR_NbiDmaBlqEvent_BLQEventThreshold1_128_or_32 (3)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaBlqEvent_BLQEventThreshold0(_x) (((_x) & 0x3) << 12)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaBlqEvent_BLQEventThreshold0_of(_x) (((_x) >> 12) & 0x3)
#define     NFP3800_NBI_DMAX_CSR_NbiDmaBlqEvent_BLQEventThreshold0_16_or_4 (0)
#define     NFP3800_NBI_DMAX_CSR_NbiDmaBlqEvent_BLQEventThreshold0_32_or_8 (1)
#define     NFP3800_NBI_DMAX_CSR_NbiDmaBlqEvent_BLQEventThreshold0_64_or_16 (2)
#define     NFP3800_NBI_DMAX_CSR_NbiDmaBlqEvent_BLQEventThreshold0_128_or_32 (3)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaBlqEvent_BLQEventNotFullEnable(_x) (((_x) & 0xf) << 4)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaBlqEvent_BLQEventNotFullEnable_of(_x) (((_x) >> 4) & 0xf)
#define     NFP3800_NBI_DMAX_CSR_NbiDmaBlqEvent_BLQEventNotFullEnable_Disable (0)
#define     NFP3800_NBI_DMAX_CSR_NbiDmaBlqEvent_BLQEventNotFullEnable_Enable (1)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaBlqEvent_BLQEventStatusEnable(_x) (((_x) & 0xf) << 0)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaBlqEvent_BLQEventStatusEnable_of(_x) (((_x) >> 0) & 0xf)
#define     NFP3800_NBI_DMAX_CSR_NbiDmaBlqEvent_BLQEventStatusEnable_Disable (0)
#define     NFP3800_NBI_DMAX_CSR_NbiDmaBlqEvent_BLQEventStatusEnable_Enable (1)
/*----------------------------------------------------------------
  Register: NbiDmaRate - NBI DMA Token Refresh Rate Register
    Bits:  7:6  CreditRate3(rw) - Sets the credit rate for push bus 3. Values are in 4 cycle increments.
    Bits:  5:4  CreditRate2(rw) - Sets the credit rate for push bus 2. Values are in 4 cycle increments.
    Bits:  3:2  CreditRate1(rw) - Sets the credit rate for push bus 1. Values are in 4 cycle increments.
    Bits:  1:0  CreditRate0(rw) - Sets the credit rate for push bus 0. Values are in 4 cycle increments.
*/
#define NFP3800_NBI_DMAX_CSR_NbiDmaRate                      0x00000010
#define   NFP3800_NBI_DMAX_CSR_NbiDmaRate_CreditRate3(_x)    (((_x) & 0x3) << 6)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaRate_CreditRate3_of(_x) (((_x) >> 6) & 0x3)
#define     NFP3800_NBI_DMAX_CSR_NbiDmaRate_CreditRate3_4    (0)
#define     NFP3800_NBI_DMAX_CSR_NbiDmaRate_CreditRate3_8    (1)
#define     NFP3800_NBI_DMAX_CSR_NbiDmaRate_CreditRate3_12   (2)
#define     NFP3800_NBI_DMAX_CSR_NbiDmaRate_CreditRate3_16   (3)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaRate_CreditRate2(_x)    (((_x) & 0x3) << 4)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaRate_CreditRate2_of(_x) (((_x) >> 4) & 0x3)
#define     NFP3800_NBI_DMAX_CSR_NbiDmaRate_CreditRate2_4    (0)
#define     NFP3800_NBI_DMAX_CSR_NbiDmaRate_CreditRate2_8    (1)
#define     NFP3800_NBI_DMAX_CSR_NbiDmaRate_CreditRate2_12   (2)
#define     NFP3800_NBI_DMAX_CSR_NbiDmaRate_CreditRate2_16   (3)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaRate_CreditRate1(_x)    (((_x) & 0x3) << 2)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaRate_CreditRate1_of(_x) (((_x) >> 2) & 0x3)
#define     NFP3800_NBI_DMAX_CSR_NbiDmaRate_CreditRate1_4    (0)
#define     NFP3800_NBI_DMAX_CSR_NbiDmaRate_CreditRate1_8    (1)
#define     NFP3800_NBI_DMAX_CSR_NbiDmaRate_CreditRate1_12   (2)
#define     NFP3800_NBI_DMAX_CSR_NbiDmaRate_CreditRate1_16   (3)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaRate_CreditRate0(_x)    (((_x) & 0x3) << 0)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaRate_CreditRate0_of(_x) (((_x) >> 0) & 0x3)
#define     NFP3800_NBI_DMAX_CSR_NbiDmaRate_CreditRate0_4    (0)
#define     NFP3800_NBI_DMAX_CSR_NbiDmaRate_CreditRate0_8    (1)
#define     NFP3800_NBI_DMAX_CSR_NbiDmaRate_CreditRate0_12   (2)
#define     NFP3800_NBI_DMAX_CSR_NbiDmaRate_CreditRate0_16   (3)
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
#define NFP3800_NBI_DMAX_CSR_NbiDmaCredit                    0x00000014
#define   NFP3800_NBI_DMAX_CSR_NbiDmaCredit_Threshold3(_x)   (((_x) & 0x3) << 14)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaCredit_Threshold3_of(_x) (((_x) >> 14) & 0x3)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaCredit_Threshold2(_x)   (((_x) & 0x3) << 12)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaCredit_Threshold2_of(_x) (((_x) >> 12) & 0x3)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaCredit_Threshold1(_x)   (((_x) & 0x3) << 10)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaCredit_Threshold1_of(_x) (((_x) >> 10) & 0x3)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaCredit_Threshold0(_x)   (((_x) & 0x3) << 8)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaCredit_Threshold0_of(_x) (((_x) >> 8) & 0x3)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaCredit_Credit3(_x)      (((_x) & 0x3) << 6)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaCredit_Credit3_of(_x)   (((_x) >> 6) & 0x3)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaCredit_Credit2(_x)      (((_x) & 0x3) << 4)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaCredit_Credit2_of(_x)   (((_x) >> 4) & 0x3)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaCredit_Credit1(_x)      (((_x) & 0x3) << 2)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaCredit_Credit1_of(_x)   (((_x) >> 2) & 0x3)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaCredit_Credit0(_x)      (((_x) & 0x3) << 0)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaCredit_Credit0_of(_x)   (((_x) >> 0) & 0x3)
/*----------------------------------------------------------------
  Register: NbiDmaBpeChainEnd - NBI DMA BPE Chain End Register
    Bits: 31:0  BpeChainEnd(rw) - Configures BPE module 'n' as the end of the buffer pool entry chain, where 'n' = 0 to 31.
*/
#define NFP3800_NBI_DMAX_CSR_NbiDmaBpeChainEnd               0x00000018
#define   NFP3800_NBI_DMAX_CSR_NbiDmaBpeChainEnd_BpeChainEnd(_x) (((_x) & 0xffffffff) << 0)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaBpeChainEnd_BpeChainEnd_of(_x) (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: NbiDmaBP[8]Cfg - NBI DMA Buffer Pool X Configuration
    Bits: 24    BlqBypassEna(rw) - When set, selects to use a null buffer descriptor if the packet is not split. The selection can also be made per packet using the preclassifier metadata_0 bits [1;6] when the BlOverrideEna is set.
    Bits: 23    BlOverrideEna(rw) - When set, Preclassifier metadata_0 bits [3;3] (PcBlistOverride) are used to select a buffer list override and metadata_0 bits [1;6] (PcBlqBypassEna) selects to use a null buffer descriptor if the packet is not split.
    Bits: 22:19 CtmBufPad(rw) - Selects the number of 32 bytes to add to the allocation size of the packet. Allows padding to the allocated space in the CTM for smaller packets in increments of 32B. 0=32B, 1=64B, 0xf=512B.
    Bits: 18    CtmBufPadEna(rw) - Enables adding CtmBufPad bytes to the allocation size of the packet.
    Bits: 17:15 CtmOffsetMsb(rw) - When the EnaEnhCtmOffset bit of the NbiDmaCfg register is a zero, this field is not used. When EnaEnhCtmOffset is a one, this field represents the MSbits of CtmOffset and defines the offset to add to the data reference when starting to write packet data to the CTM. The space in front of the packet is for the rewrite script used by the packet modifier. Encoding of {CtmOffsetMsb[3;0],CtmOffset} are: 0000 = 32B, 0001=64B, 0010=96B, 0011 = 128B, 0100 = 160B, ... 1111 = 512B
    Bits: 14    DropEn(rw) - Enables a packet to be dropped when buffer or packet credits run out or there are no MU buffers left on the selected buffer list. When cleared packets are not dropped and the logic backs up waiting for credits to arrive.
    Bits: 13    CtmOffset(rw) - When the EnaEnhCtmOffset bit of the NbiDmaCfg register is a zero, this field controls the CtmOffset in Compatability mode. In Compatability mode a CtmOffset = 0 equates to a CTM offset of 128 Bytes and a CtmOffset = 1 equates to a CTM offset of 64 Bytes. When the EnaEnhCtmOffset is a one this field contains the LSbit of CtmOffset. See description of CtmOffsetMsb.
    Bits: 12:11 PriBList(rw) - Primary Buffer list number associated. There are 4 buffer lists available, which contain buffer descriptors that are used to store packet data in internal/external MUs.
    Bits: 10:9  SecBList(rw) - Secondary Buffer list number associated. There are 4 buffer lists available, which contain buffer descriptors that are used to store packet data in internal/external MUs.
    Bits:  8:5  SplitLength(rw) - Encoded packet size used when deciding to split packets between CTM and internal/external MUs. Values 7,12-15 are reserved
    Bits:  4:0  BpeHead(rw) - Pointer to the first Buffer Pool Entry register in the list associated with this buffer pool.
*/
#define NFP3800_NBI_DMAX_CSR_NbiDmaBPCfg(_x)                 (0x00000020 + (0x4 * ((_x) & 0x7)))
#define   NFP3800_NBI_DMAX_CSR_NbiDmaBPCfg_BlqBypassEna      BIT(24)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaBPCfg_BlOverrideEna     BIT(23)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaBPCfg_CtmBufPad(_x)     (((_x) & 0xf) << 19)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaBPCfg_CtmBufPad_of(_x)  (((_x) >> 19) & 0xf)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaBPCfg_CtmBufPadEna      BIT(18)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaBPCfg_CtmOffsetMsb(_x)  (((_x) & 0x7) << 15)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaBPCfg_CtmOffsetMsb_of(_x) (((_x) >> 15) & 0x7)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaBPCfg_DropEn            BIT(14)
#define     NFP3800_NBI_DMAX_CSR_NbiDmaBPCfg_DropEn_NoDrop   (0 << 14)
#define     NFP3800_NBI_DMAX_CSR_NbiDmaBPCfg_DropEn_Drop     BIT(14)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaBPCfg_CtmOffset         BIT(13)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaBPCfg_PriBList(_x)      (((_x) & 0x3) << 11)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaBPCfg_PriBList_of(_x)   (((_x) >> 11) & 0x3)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaBPCfg_SecBList(_x)      (((_x) & 0x3) << 9)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaBPCfg_SecBList_of(_x)   (((_x) >> 9) & 0x3)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaBPCfg_SplitLength(_x)   (((_x) & 0xf) << 5)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaBPCfg_SplitLength_of(_x) (((_x) >> 5) & 0xf)
#define     NFP3800_NBI_DMAX_CSR_NbiDmaBPCfg_SplitLength_256 (0)
#define     NFP3800_NBI_DMAX_CSR_NbiDmaBPCfg_SplitLength_512 (1)
#define     NFP3800_NBI_DMAX_CSR_NbiDmaBPCfg_SplitLength_1K  (2)
#define     NFP3800_NBI_DMAX_CSR_NbiDmaBPCfg_SplitLength_2K  (3)
#define     NFP3800_NBI_DMAX_CSR_NbiDmaBPCfg_SplitLength_4K  (4)
#define     NFP3800_NBI_DMAX_CSR_NbiDmaBPCfg_SplitLength_8K  (5)
#define     NFP3800_NBI_DMAX_CSR_NbiDmaBPCfg_SplitLength_16K (6)
#define     NFP3800_NBI_DMAX_CSR_NbiDmaBPCfg_SplitLength_6K  (8)
#define     NFP3800_NBI_DMAX_CSR_NbiDmaBPCfg_SplitLength_10K (9)
#define     NFP3800_NBI_DMAX_CSR_NbiDmaBPCfg_SplitLength_12K (10)
#define     NFP3800_NBI_DMAX_CSR_NbiDmaBPCfg_SplitLength_14K (11)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaBPCfg_BpeHead(_x)       (((_x) & 0x1f) << 0)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaBPCfg_BpeHead_of(_x)    (((_x) >> 0) & 0x1f)
/*----------------------------------------------------------------
  Register: NbiDmaBpe[32]Cfg - NBI DMA Buffer Pool Entry X Configuration
    Bits: 31:27 BpeNum(rwl) - Buffer Pool Entry Number. When the register is written this value is the latched version of the buffer pool register number 0-31. Used for debug purposes.
    Bits: 26:21 Ctm(rw) - CTM number to use
    Bits: 20:10 PktCredit(rw) - Configures the number of packets allocated to the CTM
    Bits:  9:0  BufCredit(rw) - Configures the number of buffer credits in 2K byte increments associated with the CTM. The msbit is the sign bit and must be cleared during configuration.
*/
#define NFP3800_NBI_DMAX_CSR_NbiDmaBpeCfg(_x)                (0x00000040 + (0x4 * ((_x) & 0x1f)))
#define   NFP3800_NBI_DMAX_CSR_NbiDmaBpeCfg_BpeNum(_x)       (((_x) & 0x1f) << 27)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaBpeCfg_BpeNum_of(_x)    (((_x) >> 27) & 0x1f)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaBpeCfg_Ctm(_x)          (((_x) & 0x3f) << 21)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaBpeCfg_Ctm_of(_x)       (((_x) >> 21) & 0x3f)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaBpeCfg_PktCredit(_x)    (((_x) & 0x7ff) << 10)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaBpeCfg_PktCredit_of(_x) (((_x) >> 10) & 0x7ff)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaBpeCfg_BufCredit(_x)    (((_x) & 0x3ff) << 0)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaBpeCfg_BufCredit_of(_x) (((_x) >> 0) & 0x3ff)
/*----------------------------------------------------------------
  Register: NbiDmaPktCntLo - Nbi Dma Pkt Counter Low 32-bits
    Bits: 31:0  NbiDmaPktCntLo(rc) - Lower 32-bits of 48-bit NBI DMA Tx Packet count. Reading this register latches the value in NbiDmaPktCntHi register
*/
#define NFP3800_NBI_DMAX_CSR_NbiDmaPktCntLo                  0x000000c0
#define   NFP3800_NBI_DMAX_CSR_NbiDmaPktCntLo_NbiDmaPktCntLo(_x) (((_x) & 0xffffffff) << 0)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaPktCntLo_NbiDmaPktCntLo_of(_x) (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: NbiDmaPktCntHi - Nbi Dma Pkt Counter High 16-bits
    Bits: 15:0  NbiDmaPktCntHi(rc) - Upper 16-bits of 48-bit NBI Tx Packet count. Read this register after reading the NbiDmaPktCntLo register
*/
#define NFP3800_NBI_DMAX_CSR_NbiDmaPktCntHi                  0x000000c4
#define   NFP3800_NBI_DMAX_CSR_NbiDmaPktCntHi_NbiDmaPktCntHi(_x) (((_x) & 0xffff) << 0)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaPktCntHi_NbiDmaPktCntHi_of(_x) (((_x) >> 0) & 0xffff)
/*----------------------------------------------------------------
  Register: NbiDmaByteCntLo - Nbi Dma Byte Counter Low 32-bits
    Bits: 31:0  NbiDmaByteCntLo(rc) - Lower 32-bits of 64-bit NBI DMA Tx Byte count. Reading this register latches the value in NbiDmaByteCntHi register
*/
#define NFP3800_NBI_DMAX_CSR_NbiDmaByteCntLo                 0x000000c8
#define   NFP3800_NBI_DMAX_CSR_NbiDmaByteCntLo_NbiDmaByteCntLo(_x) (((_x) & 0xffffffff) << 0)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaByteCntLo_NbiDmaByteCntLo_of(_x) (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: NbiDmaByteCntHi - Nbi Dma Byte Counter High 32-bits
    Bits: 31:0  NbiDmaByteCntHi(rc) - Upper 32-bits of 64-bit NBI DMA Tx Byte count. Read this register after reading the NbiDmaByteCntLo register
*/
#define NFP3800_NBI_DMAX_CSR_NbiDmaByteCntHi                 0x000000cc
#define   NFP3800_NBI_DMAX_CSR_NbiDmaByteCntHi_NbiDmaByteCntHi(_x) (((_x) & 0xffffffff) << 0)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaByteCntHi_NbiDmaByteCntHi_of(_x) (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: NbiDmaDropPktCntLo - Nbi Dma Drop Pkt Counter Low 32-bits
    Bits: 31:0  NbiDmaDropPktCntLo(rc) - Lower 32-bits of 48-bit NBI DMA Drop Packet count. Reading this register latches the value in NbiDmaDropPktCntHi register
*/
#define NFP3800_NBI_DMAX_CSR_NbiDmaDropPktCntLo              0x000000d0
#define   NFP3800_NBI_DMAX_CSR_NbiDmaDropPktCntLo_NbiDmaDropPktCntLo(_x) (((_x) & 0xffffffff) << 0)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaDropPktCntLo_NbiDmaDropPktCntLo_of(_x) (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: NbiDmaDropPktCntHi - Nbi Dma Drop Pkt Counter High 16-bits
    Bits: 15:0  NbiDmaDropPktCntHi(rc) - Upper 16-bits of 48-bit NBI Drop Packet count. Read this register after reading the NbiDmaDropPktCntLo register
*/
#define NFP3800_NBI_DMAX_CSR_NbiDmaDropPktCntHi              0x000000d4
#define   NFP3800_NBI_DMAX_CSR_NbiDmaDropPktCntHi_NbiDmaDropPktCntHi(_x) (((_x) & 0xffff) << 0)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaDropPktCntHi_NbiDmaDropPktCntHi_of(_x) (((_x) >> 0) & 0xffff)
/*----------------------------------------------------------------
  Register: NbiDmaDropByteCntLo - Nbi Dma Drop Byte Counter Low 32-bits
    Bits: 31:0  NbiDmaDropByteCntLo(rc) - Lower 32-bits of 64-bit NBI DMA Drop Byte count. Reading this register latches the value in NbiDmaDropByteCntHi register
*/
#define NFP3800_NBI_DMAX_CSR_NbiDmaDropByteCntLo             0x000000d8
#define   NFP3800_NBI_DMAX_CSR_NbiDmaDropByteCntLo_NbiDmaDropByteCntLo(_x) (((_x) & 0xffffffff) << 0)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaDropByteCntLo_NbiDmaDropByteCntLo_of(_x) (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: NbiDmaDropByteCntHi - Nbi Dma Drop Byte Counter High 32-bits
    Bits: 31:0  NbiDmaDropByteCntHi(rc) - Upper 32-bits of 64-bit NBI DMA Drop Byte count. Read this register after reading the NbiDmaDropByteCntLo register
*/
#define NFP3800_NBI_DMAX_CSR_NbiDmaDropByteCntHi             0x000000dc
#define   NFP3800_NBI_DMAX_CSR_NbiDmaDropByteCntHi_NbiDmaDropByteCntHi(_x) (((_x) & 0xffffffff) << 0)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaDropByteCntHi_NbiDmaDropByteCntHi_of(_x) (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: NbiDmaBP[8]CtmCrThresh - NBI DMA Buffer Pool CTM Credit Thresholds.
    Bits: 29:20 Threshold2(rw) - Configures Threshold2 for the CTM credits. The packet color value is 2b11 if: credit < Threshold2. Only lower 9 bits are used, the top bit should be cleared.
    Bits: 19:10 Threshold1(rw) - Configures Threshold1 for the CTM credits. The packet color value is 2b10 if: Threshold2 <= credit < Threshold1. Only lower 9 bits are used, the top bit should be cleared.
    Bits:  9:0  Threshold0(rw) - Configures Threshold0 for the CTM credits. The packet color value is 2b01 if : Threshold1 <= credit < Threshold0, else it remains 2b00 if credit >= Threshold0. Only lower 9 bits are used, the top bit should be cleared.
*/
#define NFP3800_NBI_DMAX_CSR_NbiDmaBPCtmCrThresh(_x)         (0x00000148 + (0x4 * ((_x) & 0x7)))
#define   NFP3800_NBI_DMAX_CSR_NbiDmaBPCtmCrThresh_Threshold2(_x) (((_x) & 0x3ff) << 20)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaBPCtmCrThresh_Threshold2_of(_x) (((_x) >> 20) & 0x3ff)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaBPCtmCrThresh_Threshold1(_x) (((_x) & 0x3ff) << 10)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaBPCtmCrThresh_Threshold1_of(_x) (((_x) >> 10) & 0x3ff)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaBPCtmCrThresh_Threshold0(_x) (((_x) & 0x3ff) << 0)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaBPCtmCrThresh_Threshold0_of(_x) (((_x) >> 0) & 0x3ff)
/*----------------------------------------------------------------
  Register: BlqNullMuPtr - Programmable Null Memory Buffer Pointer.
    Bits: 28:0  BlqNullMuPtr(rw) - Programmable Null Memory Buffer Pointer.
*/
#define NFP3800_NBI_DMAX_CSR_BlqNullMuPtr                    0x00000168
#define   NFP3800_NBI_DMAX_CSR_BlqNullMuPtr_BlqNullMuPtr(_x) (((_x) & 0x1fffffff) << 0)
#define   NFP3800_NBI_DMAX_CSR_BlqNullMuPtr_BlqNullMuPtr_of(_x) (((_x) >> 0) & 0x1fffffff)
/*----------------------------------------------------------------
  Register: NbiDmaSeqNum[8] - Sequence Numbers for Sequencer X
    Bits: 11:0  NbiDmaSeqNum(rw1c) - Sequence Numbers (0..4095)
*/
#define NFP3800_NBI_DMAX_CSR_NbiDmaSeqNum(_x)                (0x0000016c + (0x4 * ((_x) & 0x7)))
#define   NFP3800_NBI_DMAX_CSR_NbiDmaSeqNum_NbiDmaSeqNum(_x) (((_x) & 0xfff) << 0)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaSeqNum_NbiDmaSeqNum_of(_x) (((_x) >> 0) & 0xfff)
/*----------------------------------------------------------------
  Register: NbiDmaFlowControlManualThreshold[20] - Port based Flow Control Threshold X
    Bits: 15:0  Threshold(rw) - When AutoThresholdEnable = 1, this register is read only and reflects the current value of the port threshold set by the high-level algorithm. When AutoThresholdEnable = 0, the programmed value is used as the port threshold.
*/
#define NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlManualThreshold(_x) (0x0000018c + (0x4 * ((_x) & 0x1f)))
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlManualThreshold_Threshold(_x) (((_x) & 0xffff) << 0)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlManualThreshold_Threshold_of(_x) (((_x) >> 0) & 0xffff)
/*----------------------------------------------------------------
  Register: NbiDmaFlowControlConfig0 - Port based Flow Control Config
    Bits: 23:21 T0fraction(rw) - Programmed value is threshold 0 in terms of a fraction of CTM Capacity: When the Total Packet Bucket climbs to this threshold the Auto Threshold State Machine transitions from State 0 to State 1. This value must be largest of the three T#fractions.
    Bits: 20:18 T1fraction(rw) - Programmed value is threshold 1 in terms of a fraction of CTM Capacity: When the Total Packet Bucket falls to this threshold the Auto Threshold State Machine transitions from State 1 to State 0. This value must be between T0fraction and T2fraction.
    Bits: 17:15 T2fraction(rw) - Programmed value is threshold 2 in terms of a fraction of CTM Capacity: When the Total Packet Bucket falls to this threshold the Auto Threshold State Machine transitions from State 2 to State 0. This value must be smallest of the three T#fractions.
    Bits: 14:12 AutoThresholdTimeout(rw) - Programmed value controls the number of packets while in State 1 after which the Auto Threshold State Machine will transition from State 1 to State 2
    Bits: 11    AutoThresholdEnable(rw) - When programmed to 1, enables the Auto Threshold State Machine to execute the high-level algorithm that periodically re-adjusts the individual port bucket thresholds that govern the per port XON/XOFF flow control
    Bits: 10:7  XonHysteresis(rw) - Programmed value controls the XON Hysteresis. If a port is XOFF'd, the port bucket value must fall this amount below its port bucket threshold before being XON'd
    Bits:  6:4  XoffTimer(rw) - Programmed value controls the maximum number of clocks a port may be XOFF'd. When a port has been off for this duration, it will be XON'd until one packet is seen, after which it will be XOFF'd again if the original XOFF condition exists. This maximum only has effect while the Auto Threshold State Machine is in State 2.
    Bits:  3    RateMeterEnable(rw) - When programmed to 1, enables all the rate meters. Disabling the rate meters will freeze the values so that they can be read one at a time, but represent a snapshot in time.
    Bits:  2:0  RateMeterResolution(rw) - When programmed to a value of N, the rate meters will have a resolution of 1/(2**(15-N)*(CLK PERIOD in secs)*(10**6)) Million Packets Per Sec
*/
#define NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlConfig0        0x000001dc
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlConfig0_T0fraction(_x) (((_x) & 0x7) << 21)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlConfig0_T0fraction_of(_x) (((_x) >> 21) & 0x7)
#define     NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlConfig0_T0fraction_8 (0)
#define     NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlConfig0_T0fraction_9 (1)
#define     NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlConfig0_T0fraction_10 (2)
#define     NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlConfig0_T0fraction_11 (3)
#define     NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlConfig0_T0fraction_12 (4)
#define     NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlConfig0_T0fraction_13 (5)
#define     NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlConfig0_T0fraction_14 (6)
#define     NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlConfig0_T0fraction_15 (7)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlConfig0_T1fraction(_x) (((_x) & 0x7) << 18)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlConfig0_T1fraction_of(_x) (((_x) >> 18) & 0x7)
#define     NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlConfig0_T1fraction_8 (0)
#define     NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlConfig0_T1fraction_9 (1)
#define     NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlConfig0_T1fraction_10 (2)
#define     NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlConfig0_T1fraction_11 (3)
#define     NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlConfig0_T1fraction_12 (4)
#define     NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlConfig0_T1fraction_13 (5)
#define     NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlConfig0_T1fraction_14 (6)
#define     NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlConfig0_T1fraction_15 (7)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlConfig0_T2fraction(_x) (((_x) & 0x7) << 15)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlConfig0_T2fraction_of(_x) (((_x) >> 15) & 0x7)
#define     NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlConfig0_T2fraction_8 (0)
#define     NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlConfig0_T2fraction_9 (1)
#define     NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlConfig0_T2fraction_10 (2)
#define     NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlConfig0_T2fraction_11 (3)
#define     NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlConfig0_T2fraction_12 (4)
#define     NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlConfig0_T2fraction_13 (5)
#define     NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlConfig0_T2fraction_14 (6)
#define     NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlConfig0_T2fraction_15 (7)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlConfig0_AutoThresholdTimeout(_x) (((_x) & 0x7) << 12)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlConfig0_AutoThresholdTimeout_of(_x) (((_x) >> 12) & 0x7)
#define     NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlConfig0_AutoThresholdTimeout_256 (0)
#define     NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlConfig0_AutoThresholdTimeout_512 (1)
#define     NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlConfig0_AutoThresholdTimeout_1024 (2)
#define     NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlConfig0_AutoThresholdTimeout_2048 (3)
#define     NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlConfig0_AutoThresholdTimeout_4096 (4)
#define     NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlConfig0_AutoThresholdTimeout_8192 (5)
#define     NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlConfig0_AutoThresholdTimeout_16384 (6)
#define     NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlConfig0_AutoThresholdTimeout_32768 (7)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlConfig0_AutoThresholdEnable BIT(11)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlConfig0_XonHysteresis(_x) (((_x) & 0xf) << 7)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlConfig0_XonHysteresis_of(_x) (((_x) >> 7) & 0xf)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlConfig0_XoffTimer(_x) (((_x) & 0x7) << 4)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlConfig0_XoffTimer_of(_x) (((_x) >> 4) & 0x7)
#define     NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlConfig0_XoffTimer_256 (0)
#define     NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlConfig0_XoffTimer_512 (1)
#define     NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlConfig0_XoffTimer_1024 (2)
#define     NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlConfig0_XoffTimer_2048 (3)
#define     NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlConfig0_XoffTimer_4096 (4)
#define     NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlConfig0_XoffTimer_8192 (5)
#define     NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlConfig0_XoffTimer_16384 (6)
#define     NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlConfig0_XoffTimer_32768 (7)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlConfig0_RateMeterEnable BIT(3)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlConfig0_RateMeterResolution(_x) (((_x) & 0x7) << 0)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlConfig0_RateMeterResolution_of(_x) (((_x) >> 0) & 0x7)
/*----------------------------------------------------------------
  Register: NbiDmaFlowControlConfig1 - Port based Flow Control Config
    Bits: 15:0  CtmCapacity(rw) - Programmed value is the maximum number of CTM packet credits usable by this NBI
*/
#define NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlConfig1        0x000001e0
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlConfig1_CtmCapacity(_x) (((_x) & 0xffff) << 0)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlConfig1_CtmCapacity_of(_x) (((_x) >> 0) & 0xffff)
/*----------------------------------------------------------------
  Register: NbiDmaFlowControlStatus0 - Port based Flow Control status
    Bits: 31:30 AutoThresholdState(ro) - State of the Auto Threshold Algorithm
    Bits: 29:16 TimeoutPacketCounter(ro) - Current Value of the TimeoutPacketCounter associated with the AutoThresholdTimeOut
    Bits: 15:0  TotalPacketsBucket(ro) - Indicates the total number of packets that are currently being processed in the chip from this NBI
*/
#define NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlStatus0        0x000001e4
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlStatus0_AutoThresholdState(_x) (((_x) & 0x3) << 30)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlStatus0_AutoThresholdState_of(_x) (((_x) >> 30) & 0x3)
#define     NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlStatus0_AutoThresholdState_0 (0)
#define     NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlStatus0_AutoThresholdState_1 (1)
#define     NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlStatus0_AutoThresholdState_2 (2)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlStatus0_TimeoutPacketCounter(_x) (((_x) & 0x3fff) << 16)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlStatus0_TimeoutPacketCounter_of(_x) (((_x) >> 16) & 0x3fff)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlStatus0_TotalPacketsBucket(_x) (((_x) & 0xffff) << 0)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlStatus0_TotalPacketsBucket_of(_x) (((_x) >> 0) & 0xffff)
/*----------------------------------------------------------------
  Register: NbiDmaFlowControlStatus1 - Port based Flow Control Status
    Bits: 31:16 ReturnCreditRate(ro) - Measured rate of packet credit returns; effectively processing rate. Value/(32768*(CLK PERIOD in secs)*(10**6)) = Millions of packets/sec
    Bits: 15:0  NewPacketRate(ro) - Measured rate of packets entering the chip through this NBI. Value/(32768*(CLK PERIOD in secs)*(10**6)) = Millions of packets/sec
*/
#define NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlStatus1        0x000001e8
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlStatus1_ReturnCreditRate(_x) (((_x) & 0xffff) << 16)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlStatus1_ReturnCreditRate_of(_x) (((_x) >> 16) & 0xffff)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlStatus1_NewPacketRate(_x) (((_x) & 0xffff) << 0)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlStatus1_NewPacketRate_of(_x) (((_x) >> 0) & 0xffff)
/*----------------------------------------------------------------
  Register: NbiDmaFlowControlPortBucketStatus0 - Port based Flow Control Status
    Bits: 20:16 Port(rw) - Programmed value provides port index information presented in the three NbiDmaFlowControlPortBucketStatusN registers.
    Bits: 15:0  Rate(ro) - Measured rate of packets entering the chip through this port. Value/(32768*(CLK PERIOD in secs)*(10**6)) = Millions of packets/sec
*/
#define NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlPortBucketStatus0 0x000001ec
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlPortBucketStatus0_Port(_x) (((_x) & 0x1f) << 16)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlPortBucketStatus0_Port_of(_x) (((_x) >> 16) & 0x1f)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlPortBucketStatus0_Rate(_x) (((_x) & 0xffff) << 0)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlPortBucketStatus0_Rate_of(_x) (((_x) >> 0) & 0xffff)
/*----------------------------------------------------------------
  Register: NbiDmaFlowControlPortBucketStatus1 - Port based Flow Control Status
    Bits: 15:0  Bucket(ro) - Current number of packets being processed that entered through this port.
*/
#define NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlPortBucketStatus1 0x000001f0
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlPortBucketStatus1_Bucket(_x) (((_x) & 0xffff) << 0)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlPortBucketStatus1_Bucket_of(_x) (((_x) >> 0) & 0xffff)
/*----------------------------------------------------------------
  Register: NbiDmaFlowControlPortBucketStatus2 - Port based Flow Control Status
    Bits: 29:14 Accumulator(ro) - Value of the credit return accumulator for the indexed port.
    Bits: 13:0  XoffTimer(ro) - Value of XoffTimer for indexed port associated with the XoffTimer programmable threshold in NbiDmaFlowControlConfig0.
*/
#define NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlPortBucketStatus2 0x000001f4
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlPortBucketStatus2_Accumulator(_x) (((_x) & 0xffff) << 14)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlPortBucketStatus2_Accumulator_of(_x) (((_x) >> 14) & 0xffff)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlPortBucketStatus2_XoffTimer(_x) (((_x) & 0x3fff) << 0)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlPortBucketStatus2_XoffTimer_of(_x) (((_x) >> 0) & 0x3fff)
/*----------------------------------------------------------------
  Register: NbiDmaFlowControlChannelPortMap0 - Channel to Port Map
    Bits: 29:25 Channel5(rw) - Defines the Channel to Port Mapping
    Bits: 24:20 Channel4(rw) - Defines the Channel to Port Mapping
    Bits: 19:15 Channel3(rw) - Defines the Channel to Port Mapping
    Bits: 14:10 Channel2(rw) - Defines the Channel to Port Mapping
    Bits:  9:5  Channel1(rw) - Defines the Channel to Port Mapping
    Bits:  4:0  Channel0(rw) - Defines the Channel to Port Mapping
*/
#define NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap0 0x000001f8
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap0_Channel5(_x) (((_x) & 0x1f) << 25)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap0_Channel5_of(_x) (((_x) >> 25) & 0x1f)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap0_Channel4(_x) (((_x) & 0x1f) << 20)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap0_Channel4_of(_x) (((_x) >> 20) & 0x1f)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap0_Channel3(_x) (((_x) & 0x1f) << 15)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap0_Channel3_of(_x) (((_x) >> 15) & 0x1f)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap0_Channel2(_x) (((_x) & 0x1f) << 10)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap0_Channel2_of(_x) (((_x) >> 10) & 0x1f)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap0_Channel1(_x) (((_x) & 0x1f) << 5)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap0_Channel1_of(_x) (((_x) >> 5) & 0x1f)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap0_Channel0(_x) (((_x) & 0x1f) << 0)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap0_Channel0_of(_x) (((_x) >> 0) & 0x1f)
/*----------------------------------------------------------------
  Register: NbiDmaFlowControlChannelPortMap1 - Channel to Port Map
    Bits: 29:25 Channel11(rw) - Defines the Channel to Port Mapping
    Bits: 24:20 Channel10(rw) - Defines the Channel to Port Mapping
    Bits: 19:15 Channel9(rw) - Defines the Channel to Port Mapping
    Bits: 14:10 Channel8(rw) - Defines the Channel to Port Mapping
    Bits:  9:5  Channel7(rw) - Defines the Channel to Port Mapping
    Bits:  4:0  Channel6(rw) - Defines the Channel to Port Mapping
*/
#define NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap1 0x000001fc
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap1_Channel11(_x) (((_x) & 0x1f) << 25)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap1_Channel11_of(_x) (((_x) >> 25) & 0x1f)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap1_Channel10(_x) (((_x) & 0x1f) << 20)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap1_Channel10_of(_x) (((_x) >> 20) & 0x1f)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap1_Channel9(_x) (((_x) & 0x1f) << 15)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap1_Channel9_of(_x) (((_x) >> 15) & 0x1f)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap1_Channel8(_x) (((_x) & 0x1f) << 10)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap1_Channel8_of(_x) (((_x) >> 10) & 0x1f)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap1_Channel7(_x) (((_x) & 0x1f) << 5)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap1_Channel7_of(_x) (((_x) >> 5) & 0x1f)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap1_Channel6(_x) (((_x) & 0x1f) << 0)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap1_Channel6_of(_x) (((_x) >> 0) & 0x1f)
/*----------------------------------------------------------------
  Register: NbiDmaFlowControlChannelPortMap2 - Channel to Port Map
    Bits: 29:25 Channel17(rw) - Defines the Channel to Port Mapping
    Bits: 24:20 Channel16(rw) - Defines the Channel to Port Mapping
    Bits: 19:15 Channel15(rw) - Defines the Channel to Port Mapping
    Bits: 14:10 Channel14(rw) - Defines the Channel to Port Mapping
    Bits:  9:5  Channel13(rw) - Defines the Channel to Port Mapping
    Bits:  4:0  Channel12(rw) - Defines the Channel to Port Mapping
*/
#define NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap2 0x00000200
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap2_Channel17(_x) (((_x) & 0x1f) << 25)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap2_Channel17_of(_x) (((_x) >> 25) & 0x1f)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap2_Channel16(_x) (((_x) & 0x1f) << 20)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap2_Channel16_of(_x) (((_x) >> 20) & 0x1f)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap2_Channel15(_x) (((_x) & 0x1f) << 15)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap2_Channel15_of(_x) (((_x) >> 15) & 0x1f)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap2_Channel14(_x) (((_x) & 0x1f) << 10)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap2_Channel14_of(_x) (((_x) >> 10) & 0x1f)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap2_Channel13(_x) (((_x) & 0x1f) << 5)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap2_Channel13_of(_x) (((_x) >> 5) & 0x1f)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap2_Channel12(_x) (((_x) & 0x1f) << 0)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap2_Channel12_of(_x) (((_x) >> 0) & 0x1f)
/*----------------------------------------------------------------
  Register: NbiDmaFlowControlChannelPortMap3 - Channel to Port Map
    Bits: 29:25 Channel23(rw) - Defines the Channel to Port Mapping
    Bits: 24:20 Channel22(rw) - Defines the Channel to Port Mapping
    Bits: 19:15 Channel21(rw) - Defines the Channel to Port Mapping
    Bits: 14:10 Channel20(rw) - Defines the Channel to Port Mapping
    Bits:  9:5  Channel19(rw) - Defines the Channel to Port Mapping
    Bits:  4:0  Channel18(rw) - Defines the Channel to Port Mapping
*/
#define NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap3 0x00000204
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap3_Channel23(_x) (((_x) & 0x1f) << 25)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap3_Channel23_of(_x) (((_x) >> 25) & 0x1f)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap3_Channel22(_x) (((_x) & 0x1f) << 20)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap3_Channel22_of(_x) (((_x) >> 20) & 0x1f)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap3_Channel21(_x) (((_x) & 0x1f) << 15)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap3_Channel21_of(_x) (((_x) >> 15) & 0x1f)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap3_Channel20(_x) (((_x) & 0x1f) << 10)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap3_Channel20_of(_x) (((_x) >> 10) & 0x1f)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap3_Channel19(_x) (((_x) & 0x1f) << 5)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap3_Channel19_of(_x) (((_x) >> 5) & 0x1f)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap3_Channel18(_x) (((_x) & 0x1f) << 0)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap3_Channel18_of(_x) (((_x) >> 0) & 0x1f)
/*----------------------------------------------------------------
  Register: NbiDmaFlowControlChannelPortMap4 - Channel to Port Map
    Bits: 29:25 Channel29(rw) - Defines the Channel to Port Mapping
    Bits: 24:20 Channel28(rw) - Defines the Channel to Port Mapping
    Bits: 19:15 Channel27(rw) - Defines the Channel to Port Mapping
    Bits: 14:10 Channel26(rw) - Defines the Channel to Port Mapping
    Bits:  9:5  Channel25(rw) - Defines the Channel to Port Mapping
    Bits:  4:0  Channel24(rw) - Defines the Channel to Port Mapping
*/
#define NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap4 0x00000208
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap4_Channel29(_x) (((_x) & 0x1f) << 25)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap4_Channel29_of(_x) (((_x) >> 25) & 0x1f)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap4_Channel28(_x) (((_x) & 0x1f) << 20)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap4_Channel28_of(_x) (((_x) >> 20) & 0x1f)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap4_Channel27(_x) (((_x) & 0x1f) << 15)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap4_Channel27_of(_x) (((_x) >> 15) & 0x1f)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap4_Channel26(_x) (((_x) & 0x1f) << 10)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap4_Channel26_of(_x) (((_x) >> 10) & 0x1f)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap4_Channel25(_x) (((_x) & 0x1f) << 5)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap4_Channel25_of(_x) (((_x) >> 5) & 0x1f)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap4_Channel24(_x) (((_x) & 0x1f) << 0)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap4_Channel24_of(_x) (((_x) >> 0) & 0x1f)
/*----------------------------------------------------------------
  Register: NbiDmaFlowControlChannelPortMap5 - Channel to Port Map
    Bits: 29:25 Channel35(rw) - Defines the Channel to Port Mapping
    Bits: 24:20 Channel34(rw) - Defines the Channel to Port Mapping
    Bits: 19:15 Channel33(rw) - Defines the Channel to Port Mapping
    Bits: 14:10 Channel32(rw) - Defines the Channel to Port Mapping
    Bits:  9:5  Channel31(rw) - Defines the Channel to Port Mapping
    Bits:  4:0  Channel30(rw) - Defines the Channel to Port Mapping
*/
#define NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap5 0x0000020c
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap5_Channel35(_x) (((_x) & 0x1f) << 25)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap5_Channel35_of(_x) (((_x) >> 25) & 0x1f)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap5_Channel34(_x) (((_x) & 0x1f) << 20)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap5_Channel34_of(_x) (((_x) >> 20) & 0x1f)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap5_Channel33(_x) (((_x) & 0x1f) << 15)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap5_Channel33_of(_x) (((_x) >> 15) & 0x1f)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap5_Channel32(_x) (((_x) & 0x1f) << 10)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap5_Channel32_of(_x) (((_x) >> 10) & 0x1f)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap5_Channel31(_x) (((_x) & 0x1f) << 5)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap5_Channel31_of(_x) (((_x) >> 5) & 0x1f)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap5_Channel30(_x) (((_x) & 0x1f) << 0)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap5_Channel30_of(_x) (((_x) >> 0) & 0x1f)
/*----------------------------------------------------------------
  Register: NbiDmaFlowControlChannelPortMap6 - Channel to Port Map
    Bits: 29:25 Channel41(rw) - Defines the Channel to Port Mapping
    Bits: 24:20 Channel40(rw) - Defines the Channel to Port Mapping
    Bits: 19:15 Channel39(rw) - Defines the Channel to Port Mapping
    Bits: 14:10 Channel38(rw) - Defines the Channel to Port Mapping
    Bits:  9:5  Channel37(rw) - Defines the Channel to Port Mapping
    Bits:  4:0  Channel36(rw) - Defines the Channel to Port Mapping
*/
#define NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap6 0x00000210
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap6_Channel41(_x) (((_x) & 0x1f) << 25)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap6_Channel41_of(_x) (((_x) >> 25) & 0x1f)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap6_Channel40(_x) (((_x) & 0x1f) << 20)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap6_Channel40_of(_x) (((_x) >> 20) & 0x1f)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap6_Channel39(_x) (((_x) & 0x1f) << 15)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap6_Channel39_of(_x) (((_x) >> 15) & 0x1f)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap6_Channel38(_x) (((_x) & 0x1f) << 10)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap6_Channel38_of(_x) (((_x) >> 10) & 0x1f)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap6_Channel37(_x) (((_x) & 0x1f) << 5)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap6_Channel37_of(_x) (((_x) >> 5) & 0x1f)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap6_Channel36(_x) (((_x) & 0x1f) << 0)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap6_Channel36_of(_x) (((_x) >> 0) & 0x1f)
/*----------------------------------------------------------------
  Register: NbiDmaFlowControlChannelPortMap7 - Channel to Port Map
    Bits: 29:25 Channel47(rw) - Defines the Channel to Port Mapping
    Bits: 24:20 Channel46(rw) - Defines the Channel to Port Mapping
    Bits: 19:15 Channel45(rw) - Defines the Channel to Port Mapping
    Bits: 14:10 Channel44(rw) - Defines the Channel to Port Mapping
    Bits:  9:5  Channel43(rw) - Defines the Channel to Port Mapping
    Bits:  4:0  Channel42(rw) - Defines the Channel to Port Mapping
*/
#define NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap7 0x00000214
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap7_Channel47(_x) (((_x) & 0x1f) << 25)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap7_Channel47_of(_x) (((_x) >> 25) & 0x1f)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap7_Channel46(_x) (((_x) & 0x1f) << 20)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap7_Channel46_of(_x) (((_x) >> 20) & 0x1f)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap7_Channel45(_x) (((_x) & 0x1f) << 15)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap7_Channel45_of(_x) (((_x) >> 15) & 0x1f)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap7_Channel44(_x) (((_x) & 0x1f) << 10)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap7_Channel44_of(_x) (((_x) >> 10) & 0x1f)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap7_Channel43(_x) (((_x) & 0x1f) << 5)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap7_Channel43_of(_x) (((_x) >> 5) & 0x1f)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap7_Channel42(_x) (((_x) & 0x1f) << 0)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap7_Channel42_of(_x) (((_x) >> 0) & 0x1f)
/*----------------------------------------------------------------
  Register: NbiDmaFlowControlChannelPortMap8 - Channel to Port Map
    Bits: 29:25 Channel53(rw) - Defines the Channel to Port Mapping
    Bits: 24:20 Channel52(rw) - Defines the Channel to Port Mapping
    Bits: 19:15 Channel51(rw) - Defines the Channel to Port Mapping
    Bits: 14:10 Channel50(rw) - Defines the Channel to Port Mapping
    Bits:  9:5  Channel49(rw) - Defines the Channel to Port Mapping
    Bits:  4:0  Channel48(rw) - Defines the Channel to Port Mapping
*/
#define NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap8 0x00000218
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap8_Channel53(_x) (((_x) & 0x1f) << 25)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap8_Channel53_of(_x) (((_x) >> 25) & 0x1f)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap8_Channel52(_x) (((_x) & 0x1f) << 20)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap8_Channel52_of(_x) (((_x) >> 20) & 0x1f)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap8_Channel51(_x) (((_x) & 0x1f) << 15)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap8_Channel51_of(_x) (((_x) >> 15) & 0x1f)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap8_Channel50(_x) (((_x) & 0x1f) << 10)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap8_Channel50_of(_x) (((_x) >> 10) & 0x1f)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap8_Channel49(_x) (((_x) & 0x1f) << 5)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap8_Channel49_of(_x) (((_x) >> 5) & 0x1f)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap8_Channel48(_x) (((_x) & 0x1f) << 0)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap8_Channel48_of(_x) (((_x) >> 0) & 0x1f)
/*----------------------------------------------------------------
  Register: NbiDmaFlowControlChannelPortMap9 - Channel to Port Map
    Bits: 29:25 Channel59(rw) - Defines the Channel to Port Mapping
    Bits: 24:20 Channel58(rw) - Defines the Channel to Port Mapping
    Bits: 19:15 Channel57(rw) - Defines the Channel to Port Mapping
    Bits: 14:10 Channel56(rw) - Defines the Channel to Port Mapping
    Bits:  9:5  Channel55(rw) - Defines the Channel to Port Mapping
    Bits:  4:0  Channel54(rw) - Defines the Channel to Port Mapping
*/
#define NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap9 0x0000021c
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap9_Channel59(_x) (((_x) & 0x1f) << 25)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap9_Channel59_of(_x) (((_x) >> 25) & 0x1f)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap9_Channel58(_x) (((_x) & 0x1f) << 20)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap9_Channel58_of(_x) (((_x) >> 20) & 0x1f)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap9_Channel57(_x) (((_x) & 0x1f) << 15)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap9_Channel57_of(_x) (((_x) >> 15) & 0x1f)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap9_Channel56(_x) (((_x) & 0x1f) << 10)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap9_Channel56_of(_x) (((_x) >> 10) & 0x1f)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap9_Channel55(_x) (((_x) & 0x1f) << 5)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap9_Channel55_of(_x) (((_x) >> 5) & 0x1f)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap9_Channel54(_x) (((_x) & 0x1f) << 0)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap9_Channel54_of(_x) (((_x) >> 0) & 0x1f)
/*----------------------------------------------------------------
  Register: NbiDmaFlowControlChannelPortMap10 - Channel to Port Map
    Bits: 29:25 Channel65(rw) - Defines the Channel to Port Mapping
    Bits: 24:20 Channel64(rw) - Defines the Channel to Port Mapping
    Bits: 19:15 Channel63(rw) - Defines the Channel to Port Mapping
    Bits: 14:10 Channel62(rw) - Defines the Channel to Port Mapping
    Bits:  9:5  Channel61(rw) - Defines the Channel to Port Mapping
    Bits:  4:0  Channel60(rw) - Defines the Channel to Port Mapping
*/
#define NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap10 0x00000220
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap10_Channel65(_x) (((_x) & 0x1f) << 25)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap10_Channel65_of(_x) (((_x) >> 25) & 0x1f)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap10_Channel64(_x) (((_x) & 0x1f) << 20)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap10_Channel64_of(_x) (((_x) >> 20) & 0x1f)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap10_Channel63(_x) (((_x) & 0x1f) << 15)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap10_Channel63_of(_x) (((_x) >> 15) & 0x1f)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap10_Channel62(_x) (((_x) & 0x1f) << 10)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap10_Channel62_of(_x) (((_x) >> 10) & 0x1f)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap10_Channel61(_x) (((_x) & 0x1f) << 5)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap10_Channel61_of(_x) (((_x) >> 5) & 0x1f)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap10_Channel60(_x) (((_x) & 0x1f) << 0)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap10_Channel60_of(_x) (((_x) >> 0) & 0x1f)
/*----------------------------------------------------------------
  Register: NbiDmaFlowControlChannelPortMap11 - Channel to Port Map
    Bits: 29:25 Channel71(rw) - Defines the Channel to Port Mapping
    Bits: 24:20 Channel70(rw) - Defines the Channel to Port Mapping
    Bits: 19:15 Channel69(rw) - Defines the Channel to Port Mapping
    Bits: 14:10 Channel68(rw) - Defines the Channel to Port Mapping
    Bits:  9:5  Channel67(rw) - Defines the Channel to Port Mapping
    Bits:  4:0  Channel66(rw) - Defines the Channel to Port Mapping
*/
#define NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap11 0x00000224
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap11_Channel71(_x) (((_x) & 0x1f) << 25)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap11_Channel71_of(_x) (((_x) >> 25) & 0x1f)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap11_Channel70(_x) (((_x) & 0x1f) << 20)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap11_Channel70_of(_x) (((_x) >> 20) & 0x1f)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap11_Channel69(_x) (((_x) & 0x1f) << 15)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap11_Channel69_of(_x) (((_x) >> 15) & 0x1f)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap11_Channel68(_x) (((_x) & 0x1f) << 10)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap11_Channel68_of(_x) (((_x) >> 10) & 0x1f)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap11_Channel67(_x) (((_x) & 0x1f) << 5)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap11_Channel67_of(_x) (((_x) >> 5) & 0x1f)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap11_Channel66(_x) (((_x) & 0x1f) << 0)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap11_Channel66_of(_x) (((_x) >> 0) & 0x1f)
/*----------------------------------------------------------------
  Register: NbiDmaFlowControlChannelPortMap12 - Channel to Port Map
    Bits: 29:25 Channel77(rw) - Defines the Channel to Port Mapping
    Bits: 24:20 Channel76(rw) - Defines the Channel to Port Mapping
    Bits: 19:15 Channel75(rw) - Defines the Channel to Port Mapping
    Bits: 14:10 Channel74(rw) - Defines the Channel to Port Mapping
    Bits:  9:5  Channel73(rw) - Defines the Channel to Port Mapping
    Bits:  4:0  Channel72(rw) - Defines the Channel to Port Mapping
*/
#define NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap12 0x00000228
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap12_Channel77(_x) (((_x) & 0x1f) << 25)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap12_Channel77_of(_x) (((_x) >> 25) & 0x1f)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap12_Channel76(_x) (((_x) & 0x1f) << 20)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap12_Channel76_of(_x) (((_x) >> 20) & 0x1f)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap12_Channel75(_x) (((_x) & 0x1f) << 15)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap12_Channel75_of(_x) (((_x) >> 15) & 0x1f)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap12_Channel74(_x) (((_x) & 0x1f) << 10)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap12_Channel74_of(_x) (((_x) >> 10) & 0x1f)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap12_Channel73(_x) (((_x) & 0x1f) << 5)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap12_Channel73_of(_x) (((_x) >> 5) & 0x1f)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap12_Channel72(_x) (((_x) & 0x1f) << 0)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap12_Channel72_of(_x) (((_x) >> 0) & 0x1f)
/*----------------------------------------------------------------
  Register: NbiDmaFlowControlChannelPortMap13 - Channel to Port Map
    Bits: 29:25 Channel83(rw) - Defines the Channel to Port Mapping
    Bits: 24:20 Channel82(rw) - Defines the Channel to Port Mapping
    Bits: 19:15 Channel81(rw) - Defines the Channel to Port Mapping
    Bits: 14:10 Channel80(rw) - Defines the Channel to Port Mapping
    Bits:  9:5  Channel79(rw) - Defines the Channel to Port Mapping
    Bits:  4:0  Channel78(rw) - Defines the Channel to Port Mapping
*/
#define NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap13 0x0000022c
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap13_Channel83(_x) (((_x) & 0x1f) << 25)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap13_Channel83_of(_x) (((_x) >> 25) & 0x1f)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap13_Channel82(_x) (((_x) & 0x1f) << 20)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap13_Channel82_of(_x) (((_x) >> 20) & 0x1f)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap13_Channel81(_x) (((_x) & 0x1f) << 15)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap13_Channel81_of(_x) (((_x) >> 15) & 0x1f)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap13_Channel80(_x) (((_x) & 0x1f) << 10)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap13_Channel80_of(_x) (((_x) >> 10) & 0x1f)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap13_Channel79(_x) (((_x) & 0x1f) << 5)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap13_Channel79_of(_x) (((_x) >> 5) & 0x1f)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap13_Channel78(_x) (((_x) & 0x1f) << 0)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap13_Channel78_of(_x) (((_x) >> 0) & 0x1f)
/*----------------------------------------------------------------
  Register: NbiDmaFlowControlChannelPortMap14 - Channel to Port Map
    Bits: 29:25 Channel89(rw) - Defines the Channel to Port Mapping
    Bits: 24:20 Channel88(rw) - Defines the Channel to Port Mapping
    Bits: 19:15 Channel87(rw) - Defines the Channel to Port Mapping
    Bits: 14:10 Channel86(rw) - Defines the Channel to Port Mapping
    Bits:  9:5  Channel85(rw) - Defines the Channel to Port Mapping
    Bits:  4:0  Channel84(rw) - Defines the Channel to Port Mapping
*/
#define NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap14 0x00000230
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap14_Channel89(_x) (((_x) & 0x1f) << 25)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap14_Channel89_of(_x) (((_x) >> 25) & 0x1f)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap14_Channel88(_x) (((_x) & 0x1f) << 20)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap14_Channel88_of(_x) (((_x) >> 20) & 0x1f)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap14_Channel87(_x) (((_x) & 0x1f) << 15)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap14_Channel87_of(_x) (((_x) >> 15) & 0x1f)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap14_Channel86(_x) (((_x) & 0x1f) << 10)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap14_Channel86_of(_x) (((_x) >> 10) & 0x1f)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap14_Channel85(_x) (((_x) & 0x1f) << 5)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap14_Channel85_of(_x) (((_x) >> 5) & 0x1f)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap14_Channel84(_x) (((_x) & 0x1f) << 0)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap14_Channel84_of(_x) (((_x) >> 0) & 0x1f)
/*----------------------------------------------------------------
  Register: NbiDmaFlowControlChannelPortMap15 - Channel to Port Map
    Bits: 29:25 Channel95(rw) - Defines the Channel to Port Mapping
    Bits: 24:20 Channel94(rw) - Defines the Channel to Port Mapping
    Bits: 19:15 Channel93(rw) - Defines the Channel to Port Mapping
    Bits: 14:10 Channel92(rw) - Defines the Channel to Port Mapping
    Bits:  9:5  Channel91(rw) - Defines the Channel to Port Mapping
    Bits:  4:0  Channel90(rw) - Defines the Channel to Port Mapping
*/
#define NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap15 0x00000234
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap15_Channel95(_x) (((_x) & 0x1f) << 25)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap15_Channel95_of(_x) (((_x) >> 25) & 0x1f)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap15_Channel94(_x) (((_x) & 0x1f) << 20)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap15_Channel94_of(_x) (((_x) >> 20) & 0x1f)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap15_Channel93(_x) (((_x) & 0x1f) << 15)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap15_Channel93_of(_x) (((_x) >> 15) & 0x1f)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap15_Channel92(_x) (((_x) & 0x1f) << 10)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap15_Channel92_of(_x) (((_x) >> 10) & 0x1f)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap15_Channel91(_x) (((_x) & 0x1f) << 5)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap15_Channel91_of(_x) (((_x) >> 5) & 0x1f)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap15_Channel90(_x) (((_x) & 0x1f) << 0)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap15_Channel90_of(_x) (((_x) >> 0) & 0x1f)
/*----------------------------------------------------------------
  Register: NbiDmaFlowControlChannelPortMap16 - Channel to Port Map
    Bits: 29:25 Channel101(rw) - Defines the Channel to Port Mapping
    Bits: 24:20 Channel100(rw) - Defines the Channel to Port Mapping
    Bits: 19:15 Channel99(rw) - Defines the Channel to Port Mapping
    Bits: 14:10 Channel98(rw) - Defines the Channel to Port Mapping
    Bits:  9:5  Channel97(rw) - Defines the Channel to Port Mapping
    Bits:  4:0  Channel96(rw) - Defines the Channel to Port Mapping
*/
#define NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap16 0x00000238
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap16_Channel101(_x) (((_x) & 0x1f) << 25)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap16_Channel101_of(_x) (((_x) >> 25) & 0x1f)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap16_Channel100(_x) (((_x) & 0x1f) << 20)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap16_Channel100_of(_x) (((_x) >> 20) & 0x1f)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap16_Channel99(_x) (((_x) & 0x1f) << 15)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap16_Channel99_of(_x) (((_x) >> 15) & 0x1f)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap16_Channel98(_x) (((_x) & 0x1f) << 10)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap16_Channel98_of(_x) (((_x) >> 10) & 0x1f)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap16_Channel97(_x) (((_x) & 0x1f) << 5)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap16_Channel97_of(_x) (((_x) >> 5) & 0x1f)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap16_Channel96(_x) (((_x) & 0x1f) << 0)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap16_Channel96_of(_x) (((_x) >> 0) & 0x1f)
/*----------------------------------------------------------------
  Register: NbiDmaFlowControlChannelPortMap17 - Channel to Port Map
    Bits: 29:25 Channel107(rw) - Defines the Channel to Port Mapping
    Bits: 24:20 Channel106(rw) - Defines the Channel to Port Mapping
    Bits: 19:15 Channel105(rw) - Defines the Channel to Port Mapping
    Bits: 14:10 Channel104(rw) - Defines the Channel to Port Mapping
    Bits:  9:5  Channel103(rw) - Defines the Channel to Port Mapping
    Bits:  4:0  Channel102(rw) - Defines the Channel to Port Mapping
*/
#define NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap17 0x0000023c
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap17_Channel107(_x) (((_x) & 0x1f) << 25)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap17_Channel107_of(_x) (((_x) >> 25) & 0x1f)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap17_Channel106(_x) (((_x) & 0x1f) << 20)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap17_Channel106_of(_x) (((_x) >> 20) & 0x1f)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap17_Channel105(_x) (((_x) & 0x1f) << 15)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap17_Channel105_of(_x) (((_x) >> 15) & 0x1f)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap17_Channel104(_x) (((_x) & 0x1f) << 10)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap17_Channel104_of(_x) (((_x) >> 10) & 0x1f)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap17_Channel103(_x) (((_x) & 0x1f) << 5)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap17_Channel103_of(_x) (((_x) >> 5) & 0x1f)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap17_Channel102(_x) (((_x) & 0x1f) << 0)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap17_Channel102_of(_x) (((_x) >> 0) & 0x1f)
/*----------------------------------------------------------------
  Register: NbiDmaFlowControlChannelPortMap18 - Channel to Port Map
    Bits: 29:25 Channel113(rw) - Defines the Channel to Port Mapping
    Bits: 24:20 Channel112(rw) - Defines the Channel to Port Mapping
    Bits: 19:15 Channel111(rw) - Defines the Channel to Port Mapping
    Bits: 14:10 Channel110(rw) - Defines the Channel to Port Mapping
    Bits:  9:5  Channel109(rw) - Defines the Channel to Port Mapping
    Bits:  4:0  Channel108(rw) - Defines the Channel to Port Mapping
*/
#define NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap18 0x00000240
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap18_Channel113(_x) (((_x) & 0x1f) << 25)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap18_Channel113_of(_x) (((_x) >> 25) & 0x1f)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap18_Channel112(_x) (((_x) & 0x1f) << 20)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap18_Channel112_of(_x) (((_x) >> 20) & 0x1f)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap18_Channel111(_x) (((_x) & 0x1f) << 15)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap18_Channel111_of(_x) (((_x) >> 15) & 0x1f)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap18_Channel110(_x) (((_x) & 0x1f) << 10)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap18_Channel110_of(_x) (((_x) >> 10) & 0x1f)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap18_Channel109(_x) (((_x) & 0x1f) << 5)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap18_Channel109_of(_x) (((_x) >> 5) & 0x1f)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap18_Channel108(_x) (((_x) & 0x1f) << 0)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap18_Channel108_of(_x) (((_x) >> 0) & 0x1f)
/*----------------------------------------------------------------
  Register: NbiDmaFlowControlChannelPortMap19 - Channel to Port Map
    Bits: 29:25 Channel119(rw) - Defines the Channel to Port Mapping
    Bits: 24:20 Channel118(rw) - Defines the Channel to Port Mapping
    Bits: 19:15 Channel117(rw) - Defines the Channel to Port Mapping
    Bits: 14:10 Channel116(rw) - Defines the Channel to Port Mapping
    Bits:  9:5  Channel115(rw) - Defines the Channel to Port Mapping
    Bits:  4:0  Channel114(rw) - Defines the Channel to Port Mapping
*/
#define NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap19 0x00000244
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap19_Channel119(_x) (((_x) & 0x1f) << 25)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap19_Channel119_of(_x) (((_x) >> 25) & 0x1f)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap19_Channel118(_x) (((_x) & 0x1f) << 20)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap19_Channel118_of(_x) (((_x) >> 20) & 0x1f)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap19_Channel117(_x) (((_x) & 0x1f) << 15)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap19_Channel117_of(_x) (((_x) >> 15) & 0x1f)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap19_Channel116(_x) (((_x) & 0x1f) << 10)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap19_Channel116_of(_x) (((_x) >> 10) & 0x1f)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap19_Channel115(_x) (((_x) & 0x1f) << 5)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap19_Channel115_of(_x) (((_x) >> 5) & 0x1f)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap19_Channel114(_x) (((_x) & 0x1f) << 0)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap19_Channel114_of(_x) (((_x) >> 0) & 0x1f)
/*----------------------------------------------------------------
  Register: NbiDmaFlowControlChannelPortMap20 - Channel to Port Map
    Bits: 29:25 Channel125(rw) - Defines the Channel to Port Mapping
    Bits: 24:20 Channel124(rw) - Defines the Channel to Port Mapping
    Bits: 19:15 Channel123(rw) - Defines the Channel to Port Mapping
    Bits: 14:10 Channel122(rw) - Defines the Channel to Port Mapping
    Bits:  9:5  Channel121(rw) - Defines the Channel to Port Mapping
    Bits:  4:0  Channel120(rw) - Defines the Channel to Port Mapping
*/
#define NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap20 0x00000248
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap20_Channel125(_x) (((_x) & 0x1f) << 25)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap20_Channel125_of(_x) (((_x) >> 25) & 0x1f)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap20_Channel124(_x) (((_x) & 0x1f) << 20)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap20_Channel124_of(_x) (((_x) >> 20) & 0x1f)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap20_Channel123(_x) (((_x) & 0x1f) << 15)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap20_Channel123_of(_x) (((_x) >> 15) & 0x1f)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap20_Channel122(_x) (((_x) & 0x1f) << 10)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap20_Channel122_of(_x) (((_x) >> 10) & 0x1f)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap20_Channel121(_x) (((_x) & 0x1f) << 5)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap20_Channel121_of(_x) (((_x) >> 5) & 0x1f)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap20_Channel120(_x) (((_x) & 0x1f) << 0)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap20_Channel120_of(_x) (((_x) >> 0) & 0x1f)
/*----------------------------------------------------------------
  Register: NbiDmaFlowControlChannelPortMap21 - Channel to Port Map
    Bits:  9:5  Channel127(rw) - Defines the Channel to Port Mapping
    Bits:  4:0  Channel126(rw) - Defines the Channel to Port Mapping
*/
#define NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap21 0x0000024c
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap21_Channel127(_x) (((_x) & 0x1f) << 5)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap21_Channel127_of(_x) (((_x) >> 5) & 0x1f)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap21_Channel126(_x) (((_x) & 0x1f) << 0)
#define   NFP3800_NBI_DMAX_CSR_NbiDmaFlowControlChannelPortMap21_Channel126_of(_x) (((_x) >> 0) & 0x1f)

#endif /* NFP3800_LONGNAMES */


#endif /* NFP3800_NBI_DMA_H */
