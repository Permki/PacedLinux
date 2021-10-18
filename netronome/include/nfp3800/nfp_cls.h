/*
 * Copyright (C) 2013, Netronome Systems, Inc.  All rights reserved.
 *
 * Disclaimer: this file is provided without any warranty
 * as part of an early-access program, and the content is
 * bound to change before the final release.
 */

#ifndef NFP3800_CLS_H
#define NFP3800_CLS_H

/*================================================================*/
/* TGT 15                                                         */

/*----------------------------------------------------------------*/
/* CPP Address Map (CPP_BASE = IMB translation: island + indexes) */

/* CPP_BASE + NFP3800_CLS_MEM */
#define NFP3800_CLS_MEM                                          (0x000000)
/* CPP_BASE + NFP3800_CLS_RINGS + NFP3800_CLS_RING_* */
#define NFP3800_CLS_RINGS                                        (0x010000)
/* CPP_BASE + NFP3800_CLS_EM + (nfp_em.h) */
#define NFP3800_CLS_EM                                           (0x020000)
/* CPP_BASE + NFP3800_CLS_AUTOPUSH + NFP3800_CLS_AUTOPUSH_* */
#define NFP3800_CLS_AUTOPUSH                                     (0x030000)
/* CPP_BASE + NFP3800_CLS_HASH + NFP3800_CLS_HASH_* */
#define NFP3800_CLS_HASH                                         (0x040000)
/* CPP_BASE + NFP3800_CLS_IM + (nfp_im.h) */
#define NFP3800_CLS_IM                                           (0x050000)
/* CPP_BASE + NFP3800_CLS_TRNG + NFP3800_CLS_TRNG_* */
#define NFP3800_CLS_TRNG                                         (0x060000)

/*----------------------------------------------------------------*/
/* XPB Address Map (XPB_BASE = NFP3800_XPB_ISLAND(island))          */

#define CLSX_BASE                                            (0x50000)

/* XPB_BASE + NFP3800_CLSX_IM + (nfp_im.h) */
#define NFP3800_CLSX_IM                                     (CLSX_BASE + 0x000000)
/* XPB_BASE + NFP3800_CLSX_TRNG + NFP3800_CLSX_TRNG_* */
#define NFP3800_CLSX_TRNG                                        (CLSX_BASE + 0x010000)
/* XPB_BASE + NFP3800_CLSX_ECC + (nfp_ecc.h) */
#define NFP3800_CLSX_ECC                                         (CLSX_BASE + 0x020000)
/* XPB_BASE + NFP3800_CLSX_ASSERT + (nfp_assert.h) */
#define NFP3800_CLSX_ASSERT                                      (CLSX_BASE + 0x030000)
/* XPB_BASE + NFP3800_CLSX_PA + (nfp_pa.h) */
#define NFP3800_CLSX_PA                                          (CLSX_BASE + 0x040000)

/*================================================================*/


/* HGID: 6ea1af6fca88 */

#ifndef NFP3800_LONGNAMES

/*----------------------------------------------------------------
  Register: RingBase[16] - Base, size, etc. of ring X
    Bits: 31    Full(rw) - Asserted if the ring is >= 3/4 full (almost full); should be initialized to zero. Watermark is at 3/4 full.
    Bits: 30    NotEmpty(rw) - Asserted if the ring is not empty; should be initialized to zero
    Bits: 28    ContainsWork(rw) - Asserted for work queues if the ring contains work and not threads
    Bits: 27:24 Report(rw) - Bitmap of which ring events should generate system events on the event bus
    Bits: 18:16 Size(rw) - Size of the ring
    Bits:  7:0  Base(rw) - Base address (anywhere within the cluster local scratch RAM) of the ring; note the alignment required by the size above
*/
#define NFP3800_CLS_RINGS_RING_BASE(_x)                      (0x00000000 + (0x8 * ((_x) & 0xf)))
#define   NFP3800_CLS_RINGS_RING_BASE_FULL                   BIT(31)
#define   NFP3800_CLS_RINGS_RING_BASE_NOT_EMPTY              BIT(30)
#define   NFP3800_CLS_RINGS_RING_BASE_WORK                   BIT(28)
#define   NFP3800_CLS_RINGS_RING_BASE_REPORT(_x)             (((_x) & 0xf) << 24)
#define   NFP3800_CLS_RINGS_RING_BASE_REPORT_of(_x)          (((_x) >> 24) & 0xf)
#define     NFP3800_CLS_RINGS_RING_BASE_REPORT_NOT_FULL      (8)
#define     NFP3800_CLS_RINGS_RING_BASE_REPORT_UNDERFLOW     (1)
#define     NFP3800_CLS_RINGS_RING_BASE_REPORT_OVERFLOW      (2)
#define     NFP3800_CLS_RINGS_RING_BASE_REPORT_NOT_EMPTY     (4)
#define   NFP3800_CLS_RINGS_RING_BASE_SIZE(_x)               (((_x) & 0x7) << 16)
#define   NFP3800_CLS_RINGS_RING_BASE_SIZE_of(_x)            (((_x) >> 16) & 0x7)
#define     NFP3800_CLS_RINGS_RING_BASE_SIZE_32              (0)
#define     NFP3800_CLS_RINGS_RING_BASE_SIZE_64              (1)
#define     NFP3800_CLS_RINGS_RING_BASE_SIZE_128             (2)
#define     NFP3800_CLS_RINGS_RING_BASE_SIZE_256             (3)
#define     NFP3800_CLS_RINGS_RING_BASE_SIZE_512             (4)
#define     NFP3800_CLS_RINGS_RING_BASE_SIZE_1024            (5)
#define     NFP3800_CLS_RINGS_RING_BASE_SIZE_2048            (6)
#define     NFP3800_CLS_RINGS_RING_BASE_SIZE_4096            (7)
#define   NFP3800_CLS_RINGS_RING_BASE_BASE(_x)               (((_x) & 0xff) << 0)
#define   NFP3800_CLS_RINGS_RING_BASE_BASE_of(_x)            (((_x) >> 0) & 0xff)
/*----------------------------------------------------------------
  Register: RingPtrs[16] - Pointers for ring X
    Bits: 28:16 TailPointer(rw) - Word offset from base address to tail of ring; when written it must be bounded by the ring size first.
    Bits: 12:0  HeadPointer(rw) - Word offset from base address to head of ring; when written it must be bounded by the ring size first.
*/
#define NFP3800_CLS_RINGS_RING_PTRS(_x)                      (0x00000080 + (0x8 * ((_x) & 0xf)))
#define   NFP3800_CLS_RINGS_RING_PTRS_TAIL_POINTER(_x)       (((_x) & 0x1fff) << 16)
#define   NFP3800_CLS_RINGS_RING_PTRS_TAIL_POINTER_of(_x)    (((_x) >> 16) & 0x1fff)
#define   NFP3800_CLS_RINGS_RING_PTRS_HEAD_POINTER(_x)       (((_x) & 0x1fff) << 0)
#define   NFP3800_CLS_RINGS_RING_PTRS_HEAD_POINTER_of(_x)    (((_x) >> 0) & 0x1fff)
/*----------------------------------------------------------------
  Register: FilterStatusMonitor[16] - Status Monitor for Event manager filter
    Bits: 17    LastStatus(rw) - Last status value; a write clears this, to ensure that a new event will be generated if the filter status monitor is reconfigured
    Bits: 16    EdgeDetected(rw) - Asserted if a rising edge of the filter status has been detected; a write clears this, to ensure that a new event will be generated if the filter status monitor is reconfigured
    Bits:  9:8  Monitor(rw) - Type of monitoring to perform
    Bits:  3:0  Autopush(rw) - Which autopush configuration register to use when the monitor fires (unless Monitor is 'Off')
*/
#define NFP3800_CLS_AUTOPUSH_STATUS(_x)                      (0x00000000 + (0x8 * ((_x) & 0xf)))
#define   NFP3800_CLS_AUTOPUSH_STATUS_LAST_STATUS            BIT(17)
#define   NFP3800_CLS_AUTOPUSH_STATUS_EDGE_DETECTED          BIT(16)
#define   NFP3800_CLS_AUTOPUSH_STATUS_MONITOR(_x)            (((_x) & 0x3) << 8)
#define   NFP3800_CLS_AUTOPUSH_STATUS_MONITOR_of(_x)         (((_x) >> 8) & 0x3)
#define     NFP3800_CLS_AUTOPUSH_STATUS_MONITOR_OFF          (0)
#define     NFP3800_CLS_AUTOPUSH_STATUS_MONITOR_STATUS       (1)
#define     NFP3800_CLS_AUTOPUSH_STATUS_MONITOR_ONE_SHOT_STATUS (2)
#define     NFP3800_CLS_AUTOPUSH_STATUS_MONITOR_ONE_SHOT_ACK (3)
#define   NFP3800_CLS_AUTOPUSH_STATUS_AUTOPUSH(_x)           (((_x) & 0xf) << 0)
#define   NFP3800_CLS_AUTOPUSH_STATUS_AUTOPUSH_of(_x)        (((_x) >> 0) & 0xf)
/*----------------------------------------------------------------
  Register: AutopushSignal[16] - Autopush signal configuration
    Bits: 31:28 MasterIslandLow(rw) - The bottom 4 bits of the CPP master island number to signal; zero in the CPP master island number implies the island the CLS is in
    Bits: 27:24 Master(rw) - The CPP master number to signal and push data to when a FilterStatusMonitor fires with this AutoPushSignal referenced.
    Bits: 22:16 SignalRef(rw) - Signal reference to signal.
    Bits: 15:14 MasterIslandHigh(rw) - The top 2 bits of the CPP master island number to signal; zero in the CPP master island number implies the island the CLS is in
    Bits: 13:0  DataRef(rw) - Data reference for data to be pushed to.
*/
#define NFP3800_CLS_AUTOPUSH_SIGNAL(_x)                      (0x00000200 + (0x8 * ((_x) & 0xf)))
#define   NFP3800_CLS_AUTOPUSH_SIGNAL_MASTER_ISL_LO(_x)      (((_x) & 0xf) << 28)
#define   NFP3800_CLS_AUTOPUSH_SIGNAL_MASTER_ISL_LO_of(_x)   (((_x) >> 28) & 0xf)
#define   NFP3800_CLS_AUTOPUSH_SIGNAL_MASTER(_x)             (((_x) & 0xf) << 24)
#define   NFP3800_CLS_AUTOPUSH_SIGNAL_MASTER_of(_x)          (((_x) >> 24) & 0xf)
#define   NFP3800_CLS_AUTOPUSH_SIGNAL_SIGNAL_REF(_x)         (((_x) & 0x7f) << 16)
#define   NFP3800_CLS_AUTOPUSH_SIGNAL_SIGNAL_REF_of(_x)      (((_x) >> 16) & 0x7f)
#define   NFP3800_CLS_AUTOPUSH_SIGNAL_MASTER_ISL_HI(_x)      (((_x) & 0x3) << 14)
#define   NFP3800_CLS_AUTOPUSH_SIGNAL_MASTER_ISL_HI_of(_x)   (((_x) >> 14) & 0x3)
#define   NFP3800_CLS_AUTOPUSH_SIGNAL_DATA_REF(_x)           (((_x) & 0x3fff) << 0)
#define   NFP3800_CLS_AUTOPUSH_SIGNAL_DATA_REF_of(_x)        (((_x) >> 0) & 0x3fff)
/*----------------------------------------------------------------
  Register: UserEventGrp_UserEvent - Push user event in to event bus system
    Bits: 15:4  Source(wo) - Apparent event source of the event.
    Bits:  3:0  Type(wo) - Apparent type of the event.
*/
#define NFP3800_CLS_AUTOPUSH_UserEventGrp_USER_EVENT         0x00000400
#define   NFP3800_CLS_AUTOPUSH_USEREVENTGRP_USER_EVENT_SOURCE(_x) (((_x) & 0xfff) << 4)
#define   NFP3800_CLS_AUTOPUSH_USEREVENTGRP_USER_EVENT_SOURCE_of(_x) (((_x) >> 4) & 0xfff)
#define   NFP3800_CLS_AUTOPUSH_USEREVENTGRP_USER_EVENT_TYPE(_x) (((_x) & 0xf) << 0)
#define   NFP3800_CLS_AUTOPUSH_USEREVENTGRP_USER_EVENT_TYPE_of(_x) (((_x) >> 0) & 0xf)
/*----------------------------------------------------------------
  Register: UserEventGrp_UserEventStatus - Get status of user event FIFO
    Bits: 20    Overflow(ro) - Set if the user event FIFO has overflowed since last read of status; cleared on read
    Bits: 19:16 HWM(ro) - Set to the largest number of events that have been in the user event FIFO at any one time since last read of the status
    Bits: 11:8  Read(ro) - Current read pointer of the user event FIFO; for test use only
    Bits:  3:0  Write(ro) - Current write pointer of the user event FIFO; for test use only
*/
#define NFP3800_CLS_AUTOPUSH_UserEventGrp_USER_EVENT_STATUS  0x00000400
#define   NFP3800_CLS_AUTOPUSH_USEREVENTGRP_USER_EVENT_STATUS_OVERFLOW BIT(20)
#define   NFP3800_CLS_AUTOPUSH_USEREVENTGRP_USER_EVENT_STATUS_HWM(_x) (((_x) & 0xf) << 16)
#define   NFP3800_CLS_AUTOPUSH_USEREVENTGRP_USER_EVENT_STATUS_HWM_of(_x) (((_x) >> 16) & 0xf)
#define   NFP3800_CLS_AUTOPUSH_USEREVENTGRP_USER_EVENT_STATUS_READ(_x) (((_x) & 0xf) << 8)
#define   NFP3800_CLS_AUTOPUSH_USEREVENTGRP_USER_EVENT_STATUS_READ_of(_x) (((_x) >> 8) & 0xf)
#define   NFP3800_CLS_AUTOPUSH_USEREVENTGRP_USER_EVENT_STATUS_WRITE(_x) (((_x) & 0xf) << 0)
#define   NFP3800_CLS_AUTOPUSH_USEREVENTGRP_USER_EVENT_STATUS_WRITE_of(_x) (((_x) >> 0) & 0xf)
/*----------------------------------------------------------------
  Register: WriteAlertConfig - Write alert configuration
    Bits: 23:16 AddressMask(rw) - Address mask for SRAM address [8;8]
    Bits: 15:8  AddressMatch(rw) - Address match for SRAM address [8;8]
    Bits:  6:4  Region(rw) - Bottom bit number of SRAM address to use as alert number
    Bits:  2    GenerateEvents(rw) - If asserted, write alerts generate events (when the pending_events bit becomes set)
    Bits:  1    RequireData63(rw) - If asserted, a write alert requires bit 63 of the SRAM write data to be set to generate an alert
    Bits:  0    Enable(rw) - If deasserted, the write alert logic is disabled
*/
#define NFP3800_CLS_AUTOPUSH_WRITE_ALERT_CFG                 0x00000600
#define   NFP3800_CLS_AUTOPUSH_WRITE_ALERT_CFG_ADDR_MASK(_x) (((_x) & 0xff) << 16)
#define   NFP3800_CLS_AUTOPUSH_WRITE_ALERT_CFG_ADDR_MASK_of(_x) (((_x) >> 16) & 0xff)
#define   NFP3800_CLS_AUTOPUSH_WRITE_ALERT_CFG_ADDR_MATCH(_x) (((_x) & 0xff) << 8)
#define   NFP3800_CLS_AUTOPUSH_WRITE_ALERT_CFG_ADDR_MATCH_of(_x) (((_x) >> 8) & 0xff)
#define   NFP3800_CLS_AUTOPUSH_WRITE_ALERT_CFG_REGION(_x)    (((_x) & 0x7) << 4)
#define   NFP3800_CLS_AUTOPUSH_WRITE_ALERT_CFG_REGION_of(_x) (((_x) >> 4) & 0x7)
#define     NFP3800_CLS_AUTOPUSH_WRITE_ALERT_CFG_REGION_1WORD (0)
#define     NFP3800_CLS_AUTOPUSH_WRITE_ALERT_CFG_REGION_2WORD (1)
#define     NFP3800_CLS_AUTOPUSH_WRITE_ALERT_CFG_REGION_4WORD (2)
#define     NFP3800_CLS_AUTOPUSH_WRITE_ALERT_CFG_REGION_8WORD (3)
#define     NFP3800_CLS_AUTOPUSH_WRITE_ALERT_CFG_REGION_16WORD (4)
#define     NFP3800_CLS_AUTOPUSH_WRITE_ALERT_CFG_REGION_32WORD (5)
#define     NFP3800_CLS_AUTOPUSH_WRITE_ALERT_CFG_REGION_64WORD (6)
#define     NFP3800_CLS_AUTOPUSH_WRITE_ALERT_CFG_REGION_RESVD (7)
#define   NFP3800_CLS_AUTOPUSH_WRITE_ALERT_CFG_EVENTS        BIT(2)
#define   NFP3800_CLS_AUTOPUSH_WRITE_ALERT_CFG_DATA63        BIT(1)
#define   NFP3800_CLS_AUTOPUSH_WRITE_ALERT_CFG_ENABLE        BIT(0)
/*----------------------------------------------------------------
  Register: WriteAlertsPending - Pending write alerts
    Bits: 63:0  Alerts(rc) - Pending alerts; clears after read
*/
#define NFP3800_CLS_AUTOPUSH_WRITE_ALERT_PEND                0x00000608
#define   NFP3800_CLS_AUTOPUSH_WRITE_ALERT_PEND_ALERTS(_x)   (((_x) & 0xffffffffffffffff) << 0)
#define   NFP3800_CLS_AUTOPUSH_WRITE_ALERT_PEND_ALERTS_of(_x) (((_x) >> 0) & 0xffffffffffffffff)
/*----------------------------------------------------------------
  Register: VFTranslation - VF translation
    Bits: 25:16 BaseAddress(rw) - Base address to be used for SRAM address [10;6]
    Bits: 11:10 OffsetSize(rw) - Virtual translation offset size
    Bits:  9:8  VFOffset(rw) - Virtual translation offset
    Bits:  7:6  VFAlert(rw) - Bit to use for write_alert enabling for virtual function translation
    Bits:  5:4  Alignment(rw) - Virtual translation offset size
    Bits:  1    ProtectVfs(rw) - Set to if virtual functions should have half the space per VF to protect them from each other
    Bits:  0    Enable(rw) - Set to enable virtual address translation (only for atomics and read/write)
*/
#define NFP3800_CLS_AUTOPUSH_VF_TRANSLATION                  0x00000620
#define   NFP3800_CLS_AUTOPUSH_VF_TRANSLATION_BASE(_x)       (((_x) & 0x3ff) << 16)
#define   NFP3800_CLS_AUTOPUSH_VF_TRANSLATION_BASE_of(_x)    (((_x) >> 16) & 0x3ff)
#define   NFP3800_CLS_AUTOPUSH_VF_TRANSLATION_OFFSET_SIZE(_x) (((_x) & 0x3) << 10)
#define   NFP3800_CLS_AUTOPUSH_VF_TRANSLATION_OFFSET_SIZE_of(_x) (((_x) >> 10) & 0x3)
#define     NFP3800_CLS_AUTOPUSH_VF_TRANSLATION_OFFSET_SIZE_OS64B (0)
#define     NFP3800_CLS_AUTOPUSH_VF_TRANSLATION_OFFSET_SIZE_OS1KB (1)
#define     NFP3800_CLS_AUTOPUSH_VF_TRANSLATION_OFFSET_SIZE_OS4KB (2)
#define     NFP3800_CLS_AUTOPUSH_VF_TRANSLATION_OFFSET_SIZE_OS16KB (3)
#define   NFP3800_CLS_AUTOPUSH_VF_TRANSLATION_VF_OFS(_x)     (((_x) & 0x3) << 8)
#define   NFP3800_CLS_AUTOPUSH_VF_TRANSLATION_VF_OFS_of(_x)  (((_x) >> 8) & 0x3)
#define     NFP3800_CLS_AUTOPUSH_VF_TRANSLATION_VF_OFS_VF4KB (0)
#define     NFP3800_CLS_AUTOPUSH_VF_TRANSLATION_VF_OFS_VF32KB (1)
#define     NFP3800_CLS_AUTOPUSH_VF_TRANSLATION_VF_OFS_VF256KB (2)
#define     NFP3800_CLS_AUTOPUSH_VF_TRANSLATION_VF_OFS_VF2MB (3)
#define   NFP3800_CLS_AUTOPUSH_VF_TRANSLATION_VF_ALERT(_x)   (((_x) & 0x3) << 6)
#define   NFP3800_CLS_AUTOPUSH_VF_TRANSLATION_VF_ALERT_of(_x) (((_x) >> 6) & 0x3)
#define     NFP3800_CLS_AUTOPUSH_VF_TRANSLATION_VF_ALERT_ALERT11 (0)
#define     NFP3800_CLS_AUTOPUSH_VF_TRANSLATION_VF_ALERT_ALERT12 (1)
#define     NFP3800_CLS_AUTOPUSH_VF_TRANSLATION_VF_ALERT_ALERT14 (2)
#define     NFP3800_CLS_AUTOPUSH_VF_TRANSLATION_VF_ALERT_ALERT20 (3)
#define   NFP3800_CLS_AUTOPUSH_VF_TRANSLATION_ALIGN(_x)      (((_x) & 0x3) << 4)
#define   NFP3800_CLS_AUTOPUSH_VF_TRANSLATION_ALIGN_of(_x)   (((_x) >> 4) & 0x3)
#define     NFP3800_CLS_AUTOPUSH_VF_TRANSLATION_ALIGN_NONE   (0)
#define     NFP3800_CLS_AUTOPUSH_VF_TRANSLATION_ALIGN_RESVD  (1)
#define     NFP3800_CLS_AUTOPUSH_VF_TRANSLATION_ALIGN_WORD   (2)
#define     NFP3800_CLS_AUTOPUSH_VF_TRANSLATION_ALIGN_WORD64 (3)
#define   NFP3800_CLS_AUTOPUSH_VF_TRANSLATION_PROT_VFS       BIT(1)
#define   NFP3800_CLS_AUTOPUSH_VF_TRANSLATION_ENABLE         BIT(0)
/*----------------------------------------------------------------
  Register: HashMultiply - Multiplier configuration for all hash indices
    Bits:  9    Premultiply(rw) - If set, then XOR the residue with the incoming data
    Bits:  8    SboxEnable(rw) - If set, then the residue is passed through SBOXes prior to remainder calculation; if clear, the remainder is performed without the SBOXes.
    Bits:  7:4  NumSboxes(rw) - Number of SBOXes to use, if enabled by SboxEnable; the sboxes are applied to 4-bit groups starting at the bottom 4 bits of the residue
    Bits:  3    Mult63(rw) - Value of 'm63' for hash multiplication equation m63.x^63 + m53.x^53 + m36.x^36 + m4.x^4
    Bits:  2    Mult53(rw) - Value of 'm53' for hash multiplication equation m63.x^63 + m53.x^53 + m36.x^36 + m4.x^4
    Bits:  1    Mult36(rw) - Value of 'm36' for hash multiplication equation m63.x^63 + m53.x^53 + m36.x^36 + m4.x^4
    Bits:  0    Mult4(rw) - Value of 'm4' for hash multiplication equation m63.x^63 + m53.x^53 + m36.x^36 + m4.x^4
*/
#define NFP3800_CLS_HASH_HASH_MULTIPLY                       0x00000000
#define   NFP3800_CLS_HASH_HASH_MULTIPLY_PREMULTIPLY         BIT(9)
#define   NFP3800_CLS_HASH_HASH_MULTIPLY_SBOXEN              BIT(8)
#define   NFP3800_CLS_HASH_HASH_MULTIPLY_NUMSBOXES(_x)       (((_x) & 0xf) << 4)
#define   NFP3800_CLS_HASH_HASH_MULTIPLY_NUMSBOXES_of(_x)    (((_x) >> 4) & 0xf)
#define   NFP3800_CLS_HASH_HASH_MULTIPLY_MULT63              BIT(3)
#define   NFP3800_CLS_HASH_HASH_MULTIPLY_MULT53              BIT(2)
#define   NFP3800_CLS_HASH_HASH_MULTIPLY_MULT36              BIT(1)
#define   NFP3800_CLS_HASH_HASH_MULTIPLY_MULT4               BIT(0)
/*----------------------------------------------------------------
  Register: HashIndexLow[16] - Bottom 32-bits of hash index
    Bits: 31:0  IndexLow(ro) - Bottom 32-bits of hash index value
*/
#define NFP3800_CLS_HASH_HASH_INDEX_LOW(_x)                  (0x00000010 + (0x100 * ((_x) & 0xf)))
#define   NFP3800_CLS_HASH_HASH_INDEX_LOW_INDEX_LOW(_x)      (((_x) & 0xffffffff) << 0)
#define   NFP3800_CLS_HASH_HASH_INDEX_LOW_INDEX_LOW_of(_x)   (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: HashIndexHigh[16] - Top 32-bits of hash index
    Bits: 31:0  IndexHigh(ro) - Top 32-bits of hash index value
*/
#define NFP3800_CLS_HASH_HASH_INDEX_HIGH(_x)                 (0x00000018 + (0x100 * ((_x) & 0xf)))
#define   NFP3800_CLS_HASH_HASH_INDEX_HIGH_INDEX_HIGH(_x)    (((_x) & 0xffffffff) << 0)
#define   NFP3800_CLS_HASH_HASH_INDEX_HIGH_INDEX_HIGH_of(_x) (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: HashIndex[16] - Full 64-bits of hash index
    Bits: 63:0  Index(ro) - Full 64-bits of hash index value
*/
#define NFP3800_CLS_HASH_HASH_INDEX64(_x)                    (0x00000800 + (0x8 * ((_x) & 0xf)))
#define   NFP3800_CLS_HASH_HASH_INDEX64_INDEX(_x)            (((_x) & 0xffffffffffffffff) << 0)
#define   NFP3800_CLS_HASH_HASH_INDEX64_INDEX_of(_x)         (((_x) >> 0) & 0xffffffffffffffff)
/*----------------------------------------------------------------
  Register: HashInitialResidueLow - Bottom 32-bits of initial residue
    Bits: 31:0  InitialResidueLow(ro) - Bottom 32-bits of initial residue
*/
#define NFP3800_CLS_HASH_HASH_INITIAL_RESIDUE_LOW            0x00002000
#define   NFP3800_CLS_HASH_HASH_INITIAL_RESIDUE_LOW_INITIAL_RESIDUE_LOW(_x) (((_x) & 0xffffffff) << 0)
#define   NFP3800_CLS_HASH_HASH_INITIAL_RESIDUE_LOW_INITIAL_RESIDUE_LOW_of(_x) (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: HashInitialResidueHigh - Top 32-bits of initial residue
    Bits: 31:0  InitialResidueHigh(ro) - Top 32-bits of initial residue
*/
#define NFP3800_CLS_HASH_HASH_INITIAL_RESIDUE_HIGH           0x00002008
#define   NFP3800_CLS_HASH_HASH_INITIAL_RESIDUE_HIGH_INITIAL_RESIDUE_HIGH(_x) (((_x) & 0xffffffff) << 0)
#define   NFP3800_CLS_HASH_HASH_INITIAL_RESIDUE_HIGH_INITIAL_RESIDUE_HIGH_of(_x) (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: Data - Random number data
  Register: DataAlt - Random number data (either or both registers may be read to retrieve data)
    Bits: 63:0  Data(ro) - Next 64 bits of random data; if zero, then there is no valid data ready.
*/
#define NFP3800_CLS_TRNG_DATA                                0x00000000
#define NFP3800_CLS_TRNG_DATA_ALT                            0x00000001
#define   NFP3800_CLS_TRNG_TRNGDATA_DATA(_x)                 (((_x) & 0xffffffffffffffff) << 0)
#define   NFP3800_CLS_TRNG_TRNGDATA_DATA_of(_x)              (((_x) >> 0) & 0xffffffffffffffff)
/*----------------------------------------------------------------
  Register: AsyncRing - Async ring configuration
    Bits: 17    OverrideData(rw) - Data to override ring input with if 'OverrideRing' is set
    Bits: 16    OverrideRing(rw) - Override the ring input and use 'OverrideData' instead as input to ring
    Bits:  7:4  FeedbackEnables(rw) - Enable feedback to internal ring segments (from inverted last ring segment output)
    Bits:  2    SyncEnable(rw) - Enable synchronizer
    Bits:  1    RingEnable(rw) - Enable ring oscillator (feedback from inverted last ring segment output to first ring segment)
    Bits:  0    EntropyEnable(rw) - Enable entropy from async LFSR (or override) to feed in to first ring segment
*/
#define NFP3800_CLSX_TRNG_ASYNC_RING                         0x00000000
#define   NFP3800_CLSX_TRNG_ASYNC_RING_OVERRIDE_DATA         BIT(17)
#define   NFP3800_CLSX_TRNG_ASYNC_RING_OVERRIDE_RING         BIT(16)
#define   NFP3800_CLSX_TRNG_ASYNC_RING_FEEDBACK_ENABLES(_x)  (((_x) & 0xf) << 4)
#define   NFP3800_CLSX_TRNG_ASYNC_RING_FEEDBACK_ENABLES_of(_x) (((_x) >> 4) & 0xf)
#define   NFP3800_CLSX_TRNG_ASYNC_RING_SYNC_ENABLE           BIT(2)
#define   NFP3800_CLSX_TRNG_ASYNC_RING_RING_ENABLE           BIT(1)
#define   NFP3800_CLSX_TRNG_ASYNC_RING_ENTROPY_ENABLE        BIT(0)
/*----------------------------------------------------------------
  Register: AsyncTest - Async LFSR test data
    Bits: 31:16 LFSRTestData(ro) - Shift register capture for LFSR test, shifted in from the bottom bit upwards; for internal test purposes only
    Bits:  0    LFSRTestEnable(rw) - Test enable for LFSR; set to enable shift register to capture LFSR output
*/
#define NFP3800_CLSX_TRNG_ASYNC_TEST                         0x00000004
#define   NFP3800_CLSX_TRNG_ASYNC_TEST_LFSR_TST_DATA(_x)     (((_x) & 0xffff) << 16)
#define   NFP3800_CLSX_TRNG_ASYNC_TEST_LFSR_TST_DATA_of(_x)  (((_x) >> 16) & 0xffff)
#define   NFP3800_CLSX_TRNG_ASYNC_TEST_LFSR_TEST_ENABLE      BIT(0)
/*----------------------------------------------------------------
  Register: AsyncCommand - Async LFSR command
    Bits:  2:0  Operation(wo) - Run an operation in the async entropy generator
*/
#define NFP3800_CLSX_TRNG_ASYNC_COMMAND                      0x00000008
#define   NFP3800_CLSX_TRNG_ASYNC_COMMAND_OPERATION(_x)      (((_x) & 0x7) << 0)
#define   NFP3800_CLSX_TRNG_ASYNC_COMMAND_OPERATION_of(_x)   (((_x) >> 0) & 0x7)
#define     NFP3800_CLSX_TRNG_ASYNC_COMMAND_OPERATION_NONE   (0)
#define     NFP3800_CLSX_TRNG_ASYNC_COMMAND_OPERATION_RST    (1)
#define     NFP3800_CLSX_TRNG_ASYNC_COMMAND_OPERATION_INIT   (2)
#define     NFP3800_CLSX_TRNG_ASYNC_COMMAND_OPERATION_LFSR   (3)
#define     NFP3800_CLSX_TRNG_ASYNC_COMMAND_OPERATION_INCR   (4)
#define     NFP3800_CLSX_TRNG_ASYNC_COMMAND_OPERATION_ONCE   (5)
#define     NFP3800_CLSX_TRNG_ASYNC_COMMAND_OPERATION_RUN    (6)
#define     NFP3800_CLSX_TRNG_ASYNC_COMMAND_OPERATION_RESVD  (7)
/*----------------------------------------------------------------
  Register: AsyncStatus - Async LFSR Status
    Bits: 11:8  RTCErrors(rw) - RTC error count - for internal purposes only
    Bits:  6    RTCDone(rw) - Set if the async generator RTC is done
    Bits:  5    RTCReady(rw) - Set if the async generator RTC is ready
    Bits:  4    RTCRunning(rw) - Set if the async generator RTC is running
    Bits:  3:0  FSMState(ro) - State of the async generator control FSM
*/
#define NFP3800_CLSX_TRNG_ASYNC_STATUS                       0x0000000c
#define   NFP3800_CLSX_TRNG_ASYNC_STATUS_RTC_ERRORS(_x)      (((_x) & 0xf) << 8)
#define   NFP3800_CLSX_TRNG_ASYNC_STATUS_RTC_ERRORS_of(_x)   (((_x) >> 8) & 0xf)
#define   NFP3800_CLSX_TRNG_ASYNC_STATUS_RTC_DONE            BIT(6)
#define   NFP3800_CLSX_TRNG_ASYNC_STATUS_RTC_READY           BIT(5)
#define   NFP3800_CLSX_TRNG_ASYNC_STATUS_RTC_RUNNING         BIT(4)
#define   NFP3800_CLSX_TRNG_ASYNC_STATUS_FSM_STATE(_x)       (((_x) & 0xf) << 0)
#define   NFP3800_CLSX_TRNG_ASYNC_STATUS_FSM_STATE_of(_x)    (((_x) >> 0) & 0xf)
/*----------------------------------------------------------------
  Register: AsyncConfig - Async LFSR Config
    Bits: 30:16 Data(rw) - Data value to load to the incrementer or LFSR
    Bits:  2    RepeatIncr(rw) - Internal use - set while the async generator is performing INCR in a continual run
    Bits:  1    Repeat(rw1c) - Set if runs will be repeating; write a 1 to this bit to stop a continual run
*/
#define NFP3800_CLSX_TRNG_ASYNC_CONFIG                       0x00000010
#define   NFP3800_CLSX_TRNG_ASYNC_CONFIG_DATA(_x)            (((_x) & 0x7fff) << 16)
#define   NFP3800_CLSX_TRNG_ASYNC_CONFIG_DATA_of(_x)         (((_x) >> 16) & 0x7fff)
#define   NFP3800_CLSX_TRNG_ASYNC_CONFIG_REPEAT_INCR         BIT(2)
#define   NFP3800_CLSX_TRNG_ASYNC_CONFIG_REPEAT              BIT(1)
/*----------------------------------------------------------------
  Register: LfsrConfig - PRNG LFSR Config
    Bits: 31:16 ReseedTimerRestart(rw) - Restart value for the LFSR reseed timer. Zero if PRNG LFSRs are not to be reseeded.
    Bits: 15:12 ClockOnce(rw) - One bit per PRNG LFSR to force them to clock only once; if this bit is set, then 'Enable' for the PRNG is cleared on the next clock cycle.
    Bits: 11:10 Reseed3(rw) - Method to reseed PRNG LFSR3
    Bits:  9:8  Reseed2(rw) - Method to reseed PRNG LFSR2
    Bits:  7:6  Reseed1(rw) - Method to reseed PRNG LFSR1
    Bits:  5:4  Reseed0(rw) - Method to reseed PRNG LFSR0
    Bits:  3:0  Enable(rw) - One bit per PRNG LFSR to enable them to clock. If zero, the relevant LFSR will not change.
*/
#define NFP3800_CLSX_TRNG_LFSR_CONFIG                        0x00000020
#define   NFP3800_CLSX_TRNG_LFSR_CONFIG_RESEED_TIMER_RESTART(_x) (((_x) & 0xffff) << 16)
#define   NFP3800_CLSX_TRNG_LFSR_CONFIG_RESEED_TIMER_RESTART_of(_x) (((_x) >> 16) & 0xffff)
#define   NFP3800_CLSX_TRNG_LFSR_CONFIG_CLK_ONCE(_x)         (((_x) & 0xf) << 12)
#define   NFP3800_CLSX_TRNG_LFSR_CONFIG_CLK_ONCE_of(_x)      (((_x) >> 12) & 0xf)
#define   NFP3800_CLSX_TRNG_LFSR_CONFIG_RESEED3(_x)          (((_x) & 0x3) << 10)
#define   NFP3800_CLSX_TRNG_LFSR_CONFIG_RESEED3_of(_x)       (((_x) >> 10) & 0x3)
#define     NFP3800_CLSX_TRNG_LFSR_CONFIG_RESEED3_NONE       (0)
#define     NFP3800_CLSX_TRNG_LFSR_CONFIG_RESEED3_HOLD       (1)
#define     NFP3800_CLSX_TRNG_LFSR_CONFIG_RESEED3_XOR        (2)
#define     NFP3800_CLSX_TRNG_LFSR_CONFIG_RESEED3_COPY       (3)
#define   NFP3800_CLSX_TRNG_LFSR_CONFIG_RESEED2(_x)          (((_x) & 0x3) << 8)
#define   NFP3800_CLSX_TRNG_LFSR_CONFIG_RESEED2_of(_x)       (((_x) >> 8) & 0x3)
#define     NFP3800_CLSX_TRNG_LFSR_CONFIG_RESEED2_NONE       (0)
#define     NFP3800_CLSX_TRNG_LFSR_CONFIG_RESEED2_HOLD       (1)
#define     NFP3800_CLSX_TRNG_LFSR_CONFIG_RESEED2_XOR        (2)
#define     NFP3800_CLSX_TRNG_LFSR_CONFIG_RESEED2_COPY       (3)
#define   NFP3800_CLSX_TRNG_LFSR_CONFIG_RESEED1(_x)          (((_x) & 0x3) << 6)
#define   NFP3800_CLSX_TRNG_LFSR_CONFIG_RESEED1_of(_x)       (((_x) >> 6) & 0x3)
#define     NFP3800_CLSX_TRNG_LFSR_CONFIG_RESEED1_NONE       (0)
#define     NFP3800_CLSX_TRNG_LFSR_CONFIG_RESEED1_HOLD       (1)
#define     NFP3800_CLSX_TRNG_LFSR_CONFIG_RESEED1_XOR        (2)
#define     NFP3800_CLSX_TRNG_LFSR_CONFIG_RESEED1_COPY       (3)
#define   NFP3800_CLSX_TRNG_LFSR_CONFIG_RESEED0(_x)          (((_x) & 0x3) << 4)
#define   NFP3800_CLSX_TRNG_LFSR_CONFIG_RESEED0_of(_x)       (((_x) >> 4) & 0x3)
#define     NFP3800_CLSX_TRNG_LFSR_CONFIG_RESEED0_NONE       (0)
#define     NFP3800_CLSX_TRNG_LFSR_CONFIG_RESEED0_HOLD       (1)
#define     NFP3800_CLSX_TRNG_LFSR_CONFIG_RESEED0_XOR        (2)
#define     NFP3800_CLSX_TRNG_LFSR_CONFIG_RESEED0_COPY       (3)
#define   NFP3800_CLSX_TRNG_LFSR_CONFIG_ENABLE(_x)           (((_x) & 0xf) << 0)
#define   NFP3800_CLSX_TRNG_LFSR_CONFIG_ENABLE_of(_x)        (((_x) >> 0) & 0xf)
/*----------------------------------------------------------------
  Register: WhitenControl - TRNG Whitener control
    Bits: 31:16 GeneratorTimeout(rw) - Timeout value for determining if data is being produced too slowly
    Bits:  2    MonitorAsync(rw) - If set then monitor the async generator bitstream for randomness; if zero, then monitor valid whitener output.
    Bits:  1    Enable(rw) - Enable the whitener - if disabled, all the data out and shift registers are also reset
    Bits:  0    PassThrough(rw) - Pass PRNG LFSR 0 output through whitener as valid on every cycle
*/
#define NFP3800_CLSX_TRNG_WHITEN_CONTROL                     0x00000024
#define   NFP3800_CLSX_TRNG_WHITEN_CONTROL_GEN_TIMEOUT(_x)   (((_x) & 0xffff) << 16)
#define   NFP3800_CLSX_TRNG_WHITEN_CONTROL_GEN_TIMEOUT_of(_x) (((_x) >> 16) & 0xffff)
#define   NFP3800_CLSX_TRNG_WHITEN_CONTROL_MONITOR_ASYNC     BIT(2)
#define   NFP3800_CLSX_TRNG_WHITEN_CONTROL_ENABLE            BIT(1)
#define   NFP3800_CLSX_TRNG_WHITEN_CONTROL_PASS_THROUGH      BIT(0)
/*----------------------------------------------------------------
  Register: WhitenConfig - TRNG Whitener configuration
    Bits: 31:0  Map(rw) - Sixteen two-bit entries to map from the four bits of PRNG LFSR output to whitened data; lower bit is the 'data' bit, upper bit is 'valid'
*/
#define NFP3800_CLSX_TRNG_WHITEN_CONFIG                      0x00000028
#define   NFP3800_CLSX_TRNG_WHITEN_CONFIG_MAP(_x)            (((_x) & 0xffffffff) << 0)
#define   NFP3800_CLSX_TRNG_WHITEN_CONFIG_MAP_of(_x)         (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: MonitorPeriod - TRNG Monitor period
    Bits: 31:0  Value(rw) - Whiteness period monitoring timer restart value
*/
#define NFP3800_CLSX_TRNG_MONITOR_PERIOD                     0x00000030
#define   NFP3800_CLSX_TRNG_MONITOR_PERIOD_VALUE(_x)         (((_x) & 0xffffffff) << 0)
#define   NFP3800_CLSX_TRNG_MONITOR_PERIOD_VALUE_of(_x)      (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: MonitorOnesCount - Count of valid ones from the whitener in the last completed monitoring period
  Register: MonitorMinOnes - Minimum permitted number of valid ones from the whitener in any monitoring period
  Register: MonitorMaxOnes - Maximum permitted number of valid ones from the whitener in any monitoring period
    Bits: 31:0  Value(rw) - Number of ones in a whiteness monitoring period
*/
#define NFP3800_CLSX_TRNG_MONITOR_ONES_COUNT                 0x00000034
#define NFP3800_CLSX_TRNG_MONITOR_MIN_ONES                   0x00000038
#define NFP3800_CLSX_TRNG_MONITOR_MAX_ONES                   0x0000003c
#define   NFP3800_CLSX_TRNG_TRNGMONITORONESCOUNT_VALUE(_x)   (((_x) & 0xffffffff) << 0)
#define   NFP3800_CLSX_TRNG_TRNGMONITORONESCOUNT_VALUE_of(_x) (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: MonitorRunLength - Longest permitted and actual consecutive same-bit valid data from the whitener
    Bits: 31:16 Max(rw) - Maximum run length permitted within a whiteness monitoring period without generating an alert
    Bits: 15:0  Value(ro) - Maximum run length seen within the last whiteness monitoring period
*/
#define NFP3800_CLSX_TRNG_MONITOR_RUN_LENGTH                 0x00000040
#define   NFP3800_CLSX_TRNG_MONITOR_RUN_LENGTH_MAX(_x)       (((_x) & 0xffff) << 16)
#define   NFP3800_CLSX_TRNG_MONITOR_RUN_LENGTH_MAX_of(_x)    (((_x) >> 16) & 0xffff)
#define   NFP3800_CLSX_TRNG_MONITOR_RUN_LENGTH_VALUE(_x)     (((_x) & 0xffff) << 0)
#define   NFP3800_CLSX_TRNG_MONITOR_RUN_LENGTH_VALUE_of(_x)  (((_x) >> 0) & 0xffff)
/*----------------------------------------------------------------
  Register: Lock - Control write access to the TRNG through XPB
    Bits:  1    WriteWarn(wo) - Enable the alert on writes; this bit can only be cleared by reset
    Bits:  0    WriteLock(wo) - Lockout all XPB writes; any subsequent write will generate an alert, and the content will be ignored
*/
#define NFP3800_CLSX_TRNG_LOCK                               0x00000050
#define   NFP3800_CLSX_TRNG_LOCK_WRITE_WARN                  BIT(1)
#define   NFP3800_CLSX_TRNG_LOCK_WRITE_LOCK                  BIT(0)
/*----------------------------------------------------------------
  Register: Alerts - Readable alert status - contents clear only on reset
    Bits: 11    DataTooSlow(ro) - Set to indicate that the whitener did not produce 64 valid data bits within the GeneratorTimeout period
    Bits: 10    WhitenessRunLength(ro) - The maximum run length of zeros or ones has been exceeded in the whiteness monitor
    Bits:  9    TooManyOnes(ro) - Set if too many valid ones were seen by the whiteness monitor
    Bits:  8    TooFewOnes(ro) - Set if too few valid ones were seen by the whiteness monitor
    Bits:  2    RtcErrored(ro) - Indicates the async RTC was not ready when it was expected to be; this should never set, and is here for sanity
    Bits:  1    WriteWarned(ro) - Indicatest a write was attempted after the WriteWarn bit was set.
    Bits:  0    LockedOut(ro) - Indicates a write was attempted after writes were locked out. The write will have been ignored.
*/
#define NFP3800_CLSX_TRNG_ALERTS                             0x00000054
#define   NFP3800_CLSX_TRNG_ALERTS_DATA_TOO_SLOW             BIT(11)
#define   NFP3800_CLSX_TRNG_ALERTS_WHITENESS_RUN_LENGTH      BIT(10)
#define   NFP3800_CLSX_TRNG_ALERTS_TOO_MANY_ONES             BIT(9)
#define   NFP3800_CLSX_TRNG_ALERTS_TOO_FEW_ONES              BIT(8)
#define   NFP3800_CLSX_TRNG_ALERTS_RTC_ERRORED               BIT(2)
#define   NFP3800_CLSX_TRNG_ALERTS_WRITE_WARNED              BIT(1)
#define   NFP3800_CLSX_TRNG_ALERTS_LOCKED_OUT                BIT(0)

#else /* NFP3800_LONGNAMES */

/*----------------------------------------------------------------
  Register: RingBase[16] - Base, size, etc. of ring X
    Bits: 31    Full(rw) - Asserted if the ring is >= 3/4 full (almost full); should be initialized to zero. Watermark is at 3/4 full.
    Bits: 30    NotEmpty(rw) - Asserted if the ring is not empty; should be initialized to zero
    Bits: 28    ContainsWork(rw) - Asserted for work queues if the ring contains work and not threads
    Bits: 27:24 Report(rw) - Bitmap of which ring events should generate system events on the event bus
    Bits: 18:16 Size(rw) - Size of the ring
    Bits:  7:0  Base(rw) - Base address (anywhere within the cluster local scratch RAM) of the ring; note the alignment required by the size above
*/
#define NFP3800_CLS_RINGS_RingBase(_x)                       (0x00000000 + (0x8 * ((_x) & 0xf)))
#define   NFP3800_CLS_RINGS_RingBase_Full                    BIT(31)
#define   NFP3800_CLS_RINGS_RingBase_NotEmpty                BIT(30)
#define   NFP3800_CLS_RINGS_RingBase_ContainsWork            BIT(28)
#define   NFP3800_CLS_RINGS_RingBase_Report(_x)              (((_x) & 0xf) << 24)
#define   NFP3800_CLS_RINGS_RingBase_Report_of(_x)           (((_x) >> 24) & 0xf)
#define     NFP3800_CLS_RINGS_RingBase_Report_NOT_FULL       (8)
#define     NFP3800_CLS_RINGS_RingBase_Report_UNDERFLOW      (1)
#define     NFP3800_CLS_RINGS_RingBase_Report_OVERFLOW       (2)
#define     NFP3800_CLS_RINGS_RingBase_Report_NOT_EMPTY      (4)
#define   NFP3800_CLS_RINGS_RingBase_Size(_x)                (((_x) & 0x7) << 16)
#define   NFP3800_CLS_RINGS_RingBase_Size_of(_x)             (((_x) >> 16) & 0x7)
#define     NFP3800_CLS_RINGS_RingBase_Size_32               (0)
#define     NFP3800_CLS_RINGS_RingBase_Size_64               (1)
#define     NFP3800_CLS_RINGS_RingBase_Size_128              (2)
#define     NFP3800_CLS_RINGS_RingBase_Size_256              (3)
#define     NFP3800_CLS_RINGS_RingBase_Size_512              (4)
#define     NFP3800_CLS_RINGS_RingBase_Size_1024             (5)
#define     NFP3800_CLS_RINGS_RingBase_Size_2048             (6)
#define     NFP3800_CLS_RINGS_RingBase_Size_4096             (7)
#define   NFP3800_CLS_RINGS_RingBase_Base(_x)                (((_x) & 0xff) << 0)
#define   NFP3800_CLS_RINGS_RingBase_Base_of(_x)             (((_x) >> 0) & 0xff)
/*----------------------------------------------------------------
  Register: RingPtrs[16] - Pointers for ring X
    Bits: 28:16 TailPointer(rw) - Word offset from base address to tail of ring; when written it must be bounded by the ring size first.
    Bits: 12:0  HeadPointer(rw) - Word offset from base address to head of ring; when written it must be bounded by the ring size first.
*/
#define NFP3800_CLS_RINGS_RingPtrs(_x)                       (0x00000080 + (0x8 * ((_x) & 0xf)))
#define   NFP3800_CLS_RINGS_RingPtrs_TailPointer(_x)         (((_x) & 0x1fff) << 16)
#define   NFP3800_CLS_RINGS_RingPtrs_TailPointer_of(_x)      (((_x) >> 16) & 0x1fff)
#define   NFP3800_CLS_RINGS_RingPtrs_HeadPointer(_x)         (((_x) & 0x1fff) << 0)
#define   NFP3800_CLS_RINGS_RingPtrs_HeadPointer_of(_x)      (((_x) >> 0) & 0x1fff)
/*----------------------------------------------------------------
  Register: FilterStatusMonitor[16] - Status Monitor for Event manager filter
    Bits: 17    LastStatus(rw) - Last status value; a write clears this, to ensure that a new event will be generated if the filter status monitor is reconfigured
    Bits: 16    EdgeDetected(rw) - Asserted if a rising edge of the filter status has been detected; a write clears this, to ensure that a new event will be generated if the filter status monitor is reconfigured
    Bits:  9:8  Monitor(rw) - Type of monitoring to perform
    Bits:  3:0  Autopush(rw) - Which autopush configuration register to use when the monitor fires (unless Monitor is 'Off')
*/
#define NFP3800_CLS_AUTOPUSH_FilterStatusMonitor(_x)         (0x00000000 + (0x8 * ((_x) & 0xf)))
#define   NFP3800_CLS_AUTOPUSH_FilterStatusMonitor_LastStatus BIT(17)
#define   NFP3800_CLS_AUTOPUSH_FilterStatusMonitor_EdgeDetected BIT(16)
#define   NFP3800_CLS_AUTOPUSH_FilterStatusMonitor_Monitor(_x) (((_x) & 0x3) << 8)
#define   NFP3800_CLS_AUTOPUSH_FilterStatusMonitor_Monitor_of(_x) (((_x) >> 8) & 0x3)
#define     NFP3800_CLS_AUTOPUSH_FilterStatusMonitor_Monitor_OFF (0)
#define     NFP3800_CLS_AUTOPUSH_FilterStatusMonitor_Monitor_STATUS (1)
#define     NFP3800_CLS_AUTOPUSH_FilterStatusMonitor_Monitor_ONE_SHOT_STATUS (2)
#define     NFP3800_CLS_AUTOPUSH_FilterStatusMonitor_Monitor_ONE_SHOT_ACK (3)
#define   NFP3800_CLS_AUTOPUSH_FilterStatusMonitor_Autopush(_x) (((_x) & 0xf) << 0)
#define   NFP3800_CLS_AUTOPUSH_FilterStatusMonitor_Autopush_of(_x) (((_x) >> 0) & 0xf)
/*----------------------------------------------------------------
  Register: AutopushSignal[16] - Autopush signal configuration
    Bits: 31:28 MasterIslandLow(rw) - The bottom 4 bits of the CPP master island number to signal; zero in the CPP master island number implies the island the CLS is in
    Bits: 27:24 Master(rw) - The CPP master number to signal and push data to when a FilterStatusMonitor fires with this AutoPushSignal referenced.
    Bits: 22:16 SignalRef(rw) - Signal reference to signal.
    Bits: 15:14 MasterIslandHigh(rw) - The top 2 bits of the CPP master island number to signal; zero in the CPP master island number implies the island the CLS is in
    Bits: 13:0  DataRef(rw) - Data reference for data to be pushed to.
*/
#define NFP3800_CLS_AUTOPUSH_AutopushSignal(_x)              (0x00000200 + (0x8 * ((_x) & 0xf)))
#define   NFP3800_CLS_AUTOPUSH_AutopushSignal_MasterIslandLow(_x) (((_x) & 0xf) << 28)
#define   NFP3800_CLS_AUTOPUSH_AutopushSignal_MasterIslandLow_of(_x) (((_x) >> 28) & 0xf)
#define   NFP3800_CLS_AUTOPUSH_AutopushSignal_Master(_x)     (((_x) & 0xf) << 24)
#define   NFP3800_CLS_AUTOPUSH_AutopushSignal_Master_of(_x)  (((_x) >> 24) & 0xf)
#define   NFP3800_CLS_AUTOPUSH_AutopushSignal_SignalRef(_x)  (((_x) & 0x7f) << 16)
#define   NFP3800_CLS_AUTOPUSH_AutopushSignal_SignalRef_of(_x) (((_x) >> 16) & 0x7f)
#define   NFP3800_CLS_AUTOPUSH_AutopushSignal_MasterIslandHigh(_x) (((_x) & 0x3) << 14)
#define   NFP3800_CLS_AUTOPUSH_AutopushSignal_MasterIslandHigh_of(_x) (((_x) >> 14) & 0x3)
#define   NFP3800_CLS_AUTOPUSH_AutopushSignal_DataRef(_x)    (((_x) & 0x3fff) << 0)
#define   NFP3800_CLS_AUTOPUSH_AutopushSignal_DataRef_of(_x) (((_x) >> 0) & 0x3fff)
/*----------------------------------------------------------------
  Register: UserEventGrp_UserEvent - Push user event in to event bus system
    Bits: 15:4  Source(wo) - Apparent event source of the event.
    Bits:  3:0  Type(wo) - Apparent type of the event.
*/
#define NFP3800_CLS_AUTOPUSH_UserEventGrp_UserEvent          0x00000400
#define   NFP3800_CLS_AUTOPUSH_UserEventGrp_UserEvent_Source(_x) (((_x) & 0xfff) << 4)
#define   NFP3800_CLS_AUTOPUSH_UserEventGrp_UserEvent_Source_of(_x) (((_x) >> 4) & 0xfff)
#define   NFP3800_CLS_AUTOPUSH_UserEventGrp_UserEvent_Type(_x) (((_x) & 0xf) << 0)
#define   NFP3800_CLS_AUTOPUSH_UserEventGrp_UserEvent_Type_of(_x) (((_x) >> 0) & 0xf)
/*----------------------------------------------------------------
  Register: UserEventGrp_UserEventStatus - Get status of user event FIFO
    Bits: 20    Overflow(ro) - Set if the user event FIFO has overflowed since last read of status; cleared on read
    Bits: 19:16 HWM(ro) - Set to the largest number of events that have been in the user event FIFO at any one time since last read of the status
    Bits: 11:8  Read(ro) - Current read pointer of the user event FIFO; for test use only
    Bits:  3:0  Write(ro) - Current write pointer of the user event FIFO; for test use only
*/
#define NFP3800_CLS_AUTOPUSH_UserEventGrp_UserEventStatus    0x00000400
#define   NFP3800_CLS_AUTOPUSH_UserEventGrp_UserEventStatus_Overflow BIT(20)
#define   NFP3800_CLS_AUTOPUSH_UserEventGrp_UserEventStatus_HWM(_x) (((_x) & 0xf) << 16)
#define   NFP3800_CLS_AUTOPUSH_UserEventGrp_UserEventStatus_HWM_of(_x) (((_x) >> 16) & 0xf)
#define   NFP3800_CLS_AUTOPUSH_UserEventGrp_UserEventStatus_Read(_x) (((_x) & 0xf) << 8)
#define   NFP3800_CLS_AUTOPUSH_UserEventGrp_UserEventStatus_Read_of(_x) (((_x) >> 8) & 0xf)
#define   NFP3800_CLS_AUTOPUSH_UserEventGrp_UserEventStatus_Write(_x) (((_x) & 0xf) << 0)
#define   NFP3800_CLS_AUTOPUSH_UserEventGrp_UserEventStatus_Write_of(_x) (((_x) >> 0) & 0xf)
/*----------------------------------------------------------------
  Register: WriteAlertConfig - Write alert configuration
    Bits: 23:16 AddressMask(rw) - Address mask for SRAM address [8;8]
    Bits: 15:8  AddressMatch(rw) - Address match for SRAM address [8;8]
    Bits:  6:4  Region(rw) - Bottom bit number of SRAM address to use as alert number
    Bits:  2    GenerateEvents(rw) - If asserted, write alerts generate events (when the pending_events bit becomes set)
    Bits:  1    RequireData63(rw) - If asserted, a write alert requires bit 63 of the SRAM write data to be set to generate an alert
    Bits:  0    Enable(rw) - If deasserted, the write alert logic is disabled
*/
#define NFP3800_CLS_AUTOPUSH_WriteAlertConfig                0x00000600
#define   NFP3800_CLS_AUTOPUSH_WriteAlertConfig_AddressMask(_x) (((_x) & 0xff) << 16)
#define   NFP3800_CLS_AUTOPUSH_WriteAlertConfig_AddressMask_of(_x) (((_x) >> 16) & 0xff)
#define   NFP3800_CLS_AUTOPUSH_WriteAlertConfig_AddressMatch(_x) (((_x) & 0xff) << 8)
#define   NFP3800_CLS_AUTOPUSH_WriteAlertConfig_AddressMatch_of(_x) (((_x) >> 8) & 0xff)
#define   NFP3800_CLS_AUTOPUSH_WriteAlertConfig_Region(_x)   (((_x) & 0x7) << 4)
#define   NFP3800_CLS_AUTOPUSH_WriteAlertConfig_Region_of(_x) (((_x) >> 4) & 0x7)
#define     NFP3800_CLS_AUTOPUSH_WriteAlertConfig_Region_1WORD (0)
#define     NFP3800_CLS_AUTOPUSH_WriteAlertConfig_Region_2WORD (1)
#define     NFP3800_CLS_AUTOPUSH_WriteAlertConfig_Region_4WORD (2)
#define     NFP3800_CLS_AUTOPUSH_WriteAlertConfig_Region_8WORD (3)
#define     NFP3800_CLS_AUTOPUSH_WriteAlertConfig_Region_16WORD (4)
#define     NFP3800_CLS_AUTOPUSH_WriteAlertConfig_Region_32WORD (5)
#define     NFP3800_CLS_AUTOPUSH_WriteAlertConfig_Region_64WORD (6)
#define     NFP3800_CLS_AUTOPUSH_WriteAlertConfig_Region_RESVD (7)
#define   NFP3800_CLS_AUTOPUSH_WriteAlertConfig_GenerateEvents BIT(2)
#define   NFP3800_CLS_AUTOPUSH_WriteAlertConfig_RequireData63 BIT(1)
#define   NFP3800_CLS_AUTOPUSH_WriteAlertConfig_Enable       BIT(0)
/*----------------------------------------------------------------
  Register: WriteAlertsPending - Pending write alerts
    Bits: 63:0  Alerts(rc) - Pending alerts; clears after read
*/
#define NFP3800_CLS_AUTOPUSH_WriteAlertsPending              0x00000608
#define   NFP3800_CLS_AUTOPUSH_WriteAlertsPending_Alerts(_x) (((_x) & 0xffffffffffffffff) << 0)
#define   NFP3800_CLS_AUTOPUSH_WriteAlertsPending_Alerts_of(_x) (((_x) >> 0) & 0xffffffffffffffff)
/*----------------------------------------------------------------
  Register: VFTranslation - VF translation
    Bits: 25:16 BaseAddress(rw) - Base address to be used for SRAM address [10;6]
    Bits: 11:10 OffsetSize(rw) - Virtual translation offset size
    Bits:  9:8  VFOffset(rw) - Virtual translation offset
    Bits:  7:6  VFAlert(rw) - Bit to use for write_alert enabling for virtual function translation
    Bits:  5:4  Alignment(rw) - Virtual translation offset size
    Bits:  1    ProtectVfs(rw) - Set to if virtual functions should have half the space per VF to protect them from each other
    Bits:  0    Enable(rw) - Set to enable virtual address translation (only for atomics and read/write)
*/
#define NFP3800_CLS_AUTOPUSH_VFTranslation                   0x00000620
#define   NFP3800_CLS_AUTOPUSH_VFTranslation_BaseAddress(_x) (((_x) & 0x3ff) << 16)
#define   NFP3800_CLS_AUTOPUSH_VFTranslation_BaseAddress_of(_x) (((_x) >> 16) & 0x3ff)
#define   NFP3800_CLS_AUTOPUSH_VFTranslation_OffsetSize(_x)  (((_x) & 0x3) << 10)
#define   NFP3800_CLS_AUTOPUSH_VFTranslation_OffsetSize_of(_x) (((_x) >> 10) & 0x3)
#define     NFP3800_CLS_AUTOPUSH_VFTranslation_OffsetSize_OS64B (0)
#define     NFP3800_CLS_AUTOPUSH_VFTranslation_OffsetSize_OS1kB (1)
#define     NFP3800_CLS_AUTOPUSH_VFTranslation_OffsetSize_OS4kB (2)
#define     NFP3800_CLS_AUTOPUSH_VFTranslation_OffsetSize_OS16kB (3)
#define   NFP3800_CLS_AUTOPUSH_VFTranslation_VFOffset(_x)    (((_x) & 0x3) << 8)
#define   NFP3800_CLS_AUTOPUSH_VFTranslation_VFOffset_of(_x) (((_x) >> 8) & 0x3)
#define     NFP3800_CLS_AUTOPUSH_VFTranslation_VFOffset_VF4kB (0)
#define     NFP3800_CLS_AUTOPUSH_VFTranslation_VFOffset_VF32kB (1)
#define     NFP3800_CLS_AUTOPUSH_VFTranslation_VFOffset_VF256kB (2)
#define     NFP3800_CLS_AUTOPUSH_VFTranslation_VFOffset_VF2MB (3)
#define   NFP3800_CLS_AUTOPUSH_VFTranslation_VFAlert(_x)     (((_x) & 0x3) << 6)
#define   NFP3800_CLS_AUTOPUSH_VFTranslation_VFAlert_of(_x)  (((_x) >> 6) & 0x3)
#define     NFP3800_CLS_AUTOPUSH_VFTranslation_VFAlert_ALERT11 (0)
#define     NFP3800_CLS_AUTOPUSH_VFTranslation_VFAlert_ALERT12 (1)
#define     NFP3800_CLS_AUTOPUSH_VFTranslation_VFAlert_ALERT14 (2)
#define     NFP3800_CLS_AUTOPUSH_VFTranslation_VFAlert_ALERT20 (3)
#define   NFP3800_CLS_AUTOPUSH_VFTranslation_Alignment(_x)   (((_x) & 0x3) << 4)
#define   NFP3800_CLS_AUTOPUSH_VFTranslation_Alignment_of(_x) (((_x) >> 4) & 0x3)
#define     NFP3800_CLS_AUTOPUSH_VFTranslation_Alignment_NONE (0)
#define     NFP3800_CLS_AUTOPUSH_VFTranslation_Alignment_RESVD (1)
#define     NFP3800_CLS_AUTOPUSH_VFTranslation_Alignment_WORD (2)
#define     NFP3800_CLS_AUTOPUSH_VFTranslation_Alignment_WORD64 (3)
#define   NFP3800_CLS_AUTOPUSH_VFTranslation_ProtectVfs      BIT(1)
#define   NFP3800_CLS_AUTOPUSH_VFTranslation_Enable          BIT(0)
/*----------------------------------------------------------------
  Register: HashMultiply - Multiplier configuration for all hash indices
    Bits:  9    Premultiply(rw) - If set, then XOR the residue with the incoming data
    Bits:  8    SboxEnable(rw) - If set, then the residue is passed through SBOXes prior to remainder calculation; if clear, the remainder is performed without the SBOXes.
    Bits:  7:4  NumSboxes(rw) - Number of SBOXes to use, if enabled by SboxEnable; the sboxes are applied to 4-bit groups starting at the bottom 4 bits of the residue
    Bits:  3    Mult63(rw) - Value of 'm63' for hash multiplication equation m63.x^63 + m53.x^53 + m36.x^36 + m4.x^4
    Bits:  2    Mult53(rw) - Value of 'm53' for hash multiplication equation m63.x^63 + m53.x^53 + m36.x^36 + m4.x^4
    Bits:  1    Mult36(rw) - Value of 'm36' for hash multiplication equation m63.x^63 + m53.x^53 + m36.x^36 + m4.x^4
    Bits:  0    Mult4(rw) - Value of 'm4' for hash multiplication equation m63.x^63 + m53.x^53 + m36.x^36 + m4.x^4
*/
#define NFP3800_CLS_HASH_HashMultiply                        0x00000000
#define   NFP3800_CLS_HASH_HashMultiply_Premultiply          BIT(9)
#define   NFP3800_CLS_HASH_HashMultiply_SboxEnable           BIT(8)
#define   NFP3800_CLS_HASH_HashMultiply_NumSboxes(_x)        (((_x) & 0xf) << 4)
#define   NFP3800_CLS_HASH_HashMultiply_NumSboxes_of(_x)     (((_x) >> 4) & 0xf)
#define   NFP3800_CLS_HASH_HashMultiply_Mult63               BIT(3)
#define   NFP3800_CLS_HASH_HashMultiply_Mult53               BIT(2)
#define   NFP3800_CLS_HASH_HashMultiply_Mult36               BIT(1)
#define   NFP3800_CLS_HASH_HashMultiply_Mult4                BIT(0)
/*----------------------------------------------------------------
  Register: HashIndexLow[16] - Bottom 32-bits of hash index
    Bits: 31:0  IndexLow(ro) - Bottom 32-bits of hash index value
*/
#define NFP3800_CLS_HASH_HashIndexLow(_x)                    (0x00000010 + (0x100 * ((_x) & 0xf)))
#define   NFP3800_CLS_HASH_HashIndexLow_IndexLow(_x)         (((_x) & 0xffffffff) << 0)
#define   NFP3800_CLS_HASH_HashIndexLow_IndexLow_of(_x)      (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: HashIndexHigh[16] - Top 32-bits of hash index
    Bits: 31:0  IndexHigh(ro) - Top 32-bits of hash index value
*/
#define NFP3800_CLS_HASH_HashIndexHigh(_x)                   (0x00000018 + (0x100 * ((_x) & 0xf)))
#define   NFP3800_CLS_HASH_HashIndexHigh_IndexHigh(_x)       (((_x) & 0xffffffff) << 0)
#define   NFP3800_CLS_HASH_HashIndexHigh_IndexHigh_of(_x)    (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: HashIndex[16] - Full 64-bits of hash index
    Bits: 63:0  Index(ro) - Full 64-bits of hash index value
*/
#define NFP3800_CLS_HASH_HashIndex(_x)                       (0x00000800 + (0x8 * ((_x) & 0xf)))
#define   NFP3800_CLS_HASH_HashIndex_Index(_x)               (((_x) & 0xffffffffffffffff) << 0)
#define   NFP3800_CLS_HASH_HashIndex_Index_of(_x)            (((_x) >> 0) & 0xffffffffffffffff)
/*----------------------------------------------------------------
  Register: HashInitialResidueLow - Bottom 32-bits of initial residue
    Bits: 31:0  InitialResidueLow(ro) - Bottom 32-bits of initial residue
*/
#define NFP3800_CLS_HASH_HashInitialResidueLow               0x00002000
#define   NFP3800_CLS_HASH_HashInitialResidueLow_InitialResidueLow(_x) (((_x) & 0xffffffff) << 0)
#define   NFP3800_CLS_HASH_HashInitialResidueLow_InitialResidueLow_of(_x) (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: HashInitialResidueHigh - Top 32-bits of initial residue
    Bits: 31:0  InitialResidueHigh(ro) - Top 32-bits of initial residue
*/
#define NFP3800_CLS_HASH_HashInitialResidueHigh              0x00002008
#define   NFP3800_CLS_HASH_HashInitialResidueHigh_InitialResidueHigh(_x) (((_x) & 0xffffffff) << 0)
#define   NFP3800_CLS_HASH_HashInitialResidueHigh_InitialResidueHigh_of(_x) (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: Data - Random number data
  Register: DataAlt - Random number data (either or both registers may be read to retrieve data)
    Bits: 63:0  Data(ro) - Next 64 bits of random data; if zero, then there is no valid data ready.
*/
#define NFP3800_CLS_TRNG_Data                                0x00000000
#define NFP3800_CLS_TRNG_DataAlt                             0x00000001
#define   NFP3800_CLS_TRNG_TRNGData_Data(_x)                 (((_x) & 0xffffffffffffffff) << 0)
#define   NFP3800_CLS_TRNG_TRNGData_Data_of(_x)              (((_x) >> 0) & 0xffffffffffffffff)
/*----------------------------------------------------------------
  Register: AsyncRing - Async ring configuration
    Bits: 17    OverrideData(rw) - Data to override ring input with if 'OverrideRing' is set
    Bits: 16    OverrideRing(rw) - Override the ring input and use 'OverrideData' instead as input to ring
    Bits:  7:4  FeedbackEnables(rw) - Enable feedback to internal ring segments (from inverted last ring segment output)
    Bits:  2    SyncEnable(rw) - Enable synchronizer
    Bits:  1    RingEnable(rw) - Enable ring oscillator (feedback from inverted last ring segment output to first ring segment)
    Bits:  0    EntropyEnable(rw) - Enable entropy from async LFSR (or override) to feed in to first ring segment
*/
#define NFP3800_CLSX_TRNG_AsyncRing                          0x00000000
#define   NFP3800_CLSX_TRNG_AsyncRing_OverrideData           BIT(17)
#define   NFP3800_CLSX_TRNG_AsyncRing_OverrideRing           BIT(16)
#define   NFP3800_CLSX_TRNG_AsyncRing_FeedbackEnables(_x)    (((_x) & 0xf) << 4)
#define   NFP3800_CLSX_TRNG_AsyncRing_FeedbackEnables_of(_x) (((_x) >> 4) & 0xf)
#define   NFP3800_CLSX_TRNG_AsyncRing_SyncEnable             BIT(2)
#define   NFP3800_CLSX_TRNG_AsyncRing_RingEnable             BIT(1)
#define   NFP3800_CLSX_TRNG_AsyncRing_EntropyEnable          BIT(0)
/*----------------------------------------------------------------
  Register: AsyncTest - Async LFSR test data
    Bits: 31:16 LFSRTestData(ro) - Shift register capture for LFSR test, shifted in from the bottom bit upwards; for internal test purposes only
    Bits:  0    LFSRTestEnable(rw) - Test enable for LFSR; set to enable shift register to capture LFSR output
*/
#define NFP3800_CLSX_TRNG_AsyncTest                          0x00000004
#define   NFP3800_CLSX_TRNG_AsyncTest_LFSRTestData(_x)       (((_x) & 0xffff) << 16)
#define   NFP3800_CLSX_TRNG_AsyncTest_LFSRTestData_of(_x)    (((_x) >> 16) & 0xffff)
#define   NFP3800_CLSX_TRNG_AsyncTest_LFSRTestEnable         BIT(0)
/*----------------------------------------------------------------
  Register: AsyncCommand - Async LFSR command
    Bits:  2:0  Operation(wo) - Run an operation in the async entropy generator
*/
#define NFP3800_CLSX_TRNG_AsyncCommand                       0x00000008
#define   NFP3800_CLSX_TRNG_AsyncCommand_Operation(_x)       (((_x) & 0x7) << 0)
#define   NFP3800_CLSX_TRNG_AsyncCommand_Operation_of(_x)    (((_x) >> 0) & 0x7)
#define     NFP3800_CLSX_TRNG_AsyncCommand_Operation_NONE    (0)
#define     NFP3800_CLSX_TRNG_AsyncCommand_Operation_RST     (1)
#define     NFP3800_CLSX_TRNG_AsyncCommand_Operation_INIT    (2)
#define     NFP3800_CLSX_TRNG_AsyncCommand_Operation_LFSR    (3)
#define     NFP3800_CLSX_TRNG_AsyncCommand_Operation_INCR    (4)
#define     NFP3800_CLSX_TRNG_AsyncCommand_Operation_ONCE    (5)
#define     NFP3800_CLSX_TRNG_AsyncCommand_Operation_RUN     (6)
#define     NFP3800_CLSX_TRNG_AsyncCommand_Operation_RESVD   (7)
/*----------------------------------------------------------------
  Register: AsyncStatus - Async LFSR Status
    Bits: 11:8  RTCErrors(rw) - RTC error count - for internal purposes only
    Bits:  6    RTCDone(rw) - Set if the async generator RTC is done
    Bits:  5    RTCReady(rw) - Set if the async generator RTC is ready
    Bits:  4    RTCRunning(rw) - Set if the async generator RTC is running
    Bits:  3:0  FSMState(ro) - State of the async generator control FSM
*/
#define NFP3800_CLSX_TRNG_AsyncStatus                        0x0000000c
#define   NFP3800_CLSX_TRNG_AsyncStatus_RTCErrors(_x)        (((_x) & 0xf) << 8)
#define   NFP3800_CLSX_TRNG_AsyncStatus_RTCErrors_of(_x)     (((_x) >> 8) & 0xf)
#define   NFP3800_CLSX_TRNG_AsyncStatus_RTCDone              BIT(6)
#define   NFP3800_CLSX_TRNG_AsyncStatus_RTCReady             BIT(5)
#define   NFP3800_CLSX_TRNG_AsyncStatus_RTCRunning           BIT(4)
#define   NFP3800_CLSX_TRNG_AsyncStatus_FSMState(_x)         (((_x) & 0xf) << 0)
#define   NFP3800_CLSX_TRNG_AsyncStatus_FSMState_of(_x)      (((_x) >> 0) & 0xf)
/*----------------------------------------------------------------
  Register: AsyncConfig - Async LFSR Config
    Bits: 30:16 Data(rw) - Data value to load to the incrementer or LFSR
    Bits:  2    RepeatIncr(rw) - Internal use - set while the async generator is performing INCR in a continual run
    Bits:  1    Repeat(rw1c) - Set if runs will be repeating; write a 1 to this bit to stop a continual run
*/
#define NFP3800_CLSX_TRNG_AsyncConfig                        0x00000010
#define   NFP3800_CLSX_TRNG_AsyncConfig_Data(_x)             (((_x) & 0x7fff) << 16)
#define   NFP3800_CLSX_TRNG_AsyncConfig_Data_of(_x)          (((_x) >> 16) & 0x7fff)
#define   NFP3800_CLSX_TRNG_AsyncConfig_RepeatIncr           BIT(2)
#define   NFP3800_CLSX_TRNG_AsyncConfig_Repeat               BIT(1)
/*----------------------------------------------------------------
  Register: LfsrConfig - PRNG LFSR Config
    Bits: 31:16 ReseedTimerRestart(rw) - Restart value for the LFSR reseed timer. Zero if PRNG LFSRs are not to be reseeded.
    Bits: 15:12 ClockOnce(rw) - One bit per PRNG LFSR to force them to clock only once; if this bit is set, then 'Enable' for the PRNG is cleared on the next clock cycle.
    Bits: 11:10 Reseed3(rw) - Method to reseed PRNG LFSR3
    Bits:  9:8  Reseed2(rw) - Method to reseed PRNG LFSR2
    Bits:  7:6  Reseed1(rw) - Method to reseed PRNG LFSR1
    Bits:  5:4  Reseed0(rw) - Method to reseed PRNG LFSR0
    Bits:  3:0  Enable(rw) - One bit per PRNG LFSR to enable them to clock. If zero, the relevant LFSR will not change.
*/
#define NFP3800_CLSX_TRNG_LfsrConfig                         0x00000020
#define   NFP3800_CLSX_TRNG_LfsrConfig_ReseedTimerRestart(_x) (((_x) & 0xffff) << 16)
#define   NFP3800_CLSX_TRNG_LfsrConfig_ReseedTimerRestart_of(_x) (((_x) >> 16) & 0xffff)
#define   NFP3800_CLSX_TRNG_LfsrConfig_ClockOnce(_x)         (((_x) & 0xf) << 12)
#define   NFP3800_CLSX_TRNG_LfsrConfig_ClockOnce_of(_x)      (((_x) >> 12) & 0xf)
#define   NFP3800_CLSX_TRNG_LfsrConfig_Reseed3(_x)           (((_x) & 0x3) << 10)
#define   NFP3800_CLSX_TRNG_LfsrConfig_Reseed3_of(_x)        (((_x) >> 10) & 0x3)
#define     NFP3800_CLSX_TRNG_LfsrConfig_Reseed3_NONE        (0)
#define     NFP3800_CLSX_TRNG_LfsrConfig_Reseed3_HOLD        (1)
#define     NFP3800_CLSX_TRNG_LfsrConfig_Reseed3_XOR         (2)
#define     NFP3800_CLSX_TRNG_LfsrConfig_Reseed3_COPY        (3)
#define   NFP3800_CLSX_TRNG_LfsrConfig_Reseed2(_x)           (((_x) & 0x3) << 8)
#define   NFP3800_CLSX_TRNG_LfsrConfig_Reseed2_of(_x)        (((_x) >> 8) & 0x3)
#define     NFP3800_CLSX_TRNG_LfsrConfig_Reseed2_NONE        (0)
#define     NFP3800_CLSX_TRNG_LfsrConfig_Reseed2_HOLD        (1)
#define     NFP3800_CLSX_TRNG_LfsrConfig_Reseed2_XOR         (2)
#define     NFP3800_CLSX_TRNG_LfsrConfig_Reseed2_COPY        (3)
#define   NFP3800_CLSX_TRNG_LfsrConfig_Reseed1(_x)           (((_x) & 0x3) << 6)
#define   NFP3800_CLSX_TRNG_LfsrConfig_Reseed1_of(_x)        (((_x) >> 6) & 0x3)
#define     NFP3800_CLSX_TRNG_LfsrConfig_Reseed1_NONE        (0)
#define     NFP3800_CLSX_TRNG_LfsrConfig_Reseed1_HOLD        (1)
#define     NFP3800_CLSX_TRNG_LfsrConfig_Reseed1_XOR         (2)
#define     NFP3800_CLSX_TRNG_LfsrConfig_Reseed1_COPY        (3)
#define   NFP3800_CLSX_TRNG_LfsrConfig_Reseed0(_x)           (((_x) & 0x3) << 4)
#define   NFP3800_CLSX_TRNG_LfsrConfig_Reseed0_of(_x)        (((_x) >> 4) & 0x3)
#define     NFP3800_CLSX_TRNG_LfsrConfig_Reseed0_NONE        (0)
#define     NFP3800_CLSX_TRNG_LfsrConfig_Reseed0_HOLD        (1)
#define     NFP3800_CLSX_TRNG_LfsrConfig_Reseed0_XOR         (2)
#define     NFP3800_CLSX_TRNG_LfsrConfig_Reseed0_COPY        (3)
#define   NFP3800_CLSX_TRNG_LfsrConfig_Enable(_x)            (((_x) & 0xf) << 0)
#define   NFP3800_CLSX_TRNG_LfsrConfig_Enable_of(_x)         (((_x) >> 0) & 0xf)
/*----------------------------------------------------------------
  Register: WhitenControl - TRNG Whitener control
    Bits: 31:16 GeneratorTimeout(rw) - Timeout value for determining if data is being produced too slowly
    Bits:  2    MonitorAsync(rw) - If set then monitor the async generator bitstream for randomness; if zero, then monitor valid whitener output.
    Bits:  1    Enable(rw) - Enable the whitener - if disabled, all the data out and shift registers are also reset
    Bits:  0    PassThrough(rw) - Pass PRNG LFSR 0 output through whitener as valid on every cycle
*/
#define NFP3800_CLSX_TRNG_WhitenControl                      0x00000024
#define   NFP3800_CLSX_TRNG_WhitenControl_GeneratorTimeout(_x) (((_x) & 0xffff) << 16)
#define   NFP3800_CLSX_TRNG_WhitenControl_GeneratorTimeout_of(_x) (((_x) >> 16) & 0xffff)
#define   NFP3800_CLSX_TRNG_WhitenControl_MonitorAsync       BIT(2)
#define   NFP3800_CLSX_TRNG_WhitenControl_Enable             BIT(1)
#define   NFP3800_CLSX_TRNG_WhitenControl_PassThrough        BIT(0)
/*----------------------------------------------------------------
  Register: WhitenConfig - TRNG Whitener configuration
    Bits: 31:0  Map(rw) - Sixteen two-bit entries to map from the four bits of PRNG LFSR output to whitened data; lower bit is the 'data' bit, upper bit is 'valid'
*/
#define NFP3800_CLSX_TRNG_WhitenConfig                       0x00000028
#define   NFP3800_CLSX_TRNG_WhitenConfig_Map(_x)             (((_x) & 0xffffffff) << 0)
#define   NFP3800_CLSX_TRNG_WhitenConfig_Map_of(_x)          (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: MonitorPeriod - TRNG Monitor period
    Bits: 31:0  Value(rw) - Whiteness period monitoring timer restart value
*/
#define NFP3800_CLSX_TRNG_MonitorPeriod                      0x00000030
#define   NFP3800_CLSX_TRNG_MonitorPeriod_Value(_x)          (((_x) & 0xffffffff) << 0)
#define   NFP3800_CLSX_TRNG_MonitorPeriod_Value_of(_x)       (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: MonitorOnesCount - Count of valid ones from the whitener in the last completed monitoring period
  Register: MonitorMinOnes - Minimum permitted number of valid ones from the whitener in any monitoring period
  Register: MonitorMaxOnes - Maximum permitted number of valid ones from the whitener in any monitoring period
    Bits: 31:0  Value(rw) - Number of ones in a whiteness monitoring period
*/
#define NFP3800_CLSX_TRNG_MonitorOnesCount                   0x00000034
#define NFP3800_CLSX_TRNG_MonitorMinOnes                     0x00000038
#define NFP3800_CLSX_TRNG_MonitorMaxOnes                     0x0000003c
#define   NFP3800_CLSX_TRNG_TRNGMonitorOnesCount_Value(_x)   (((_x) & 0xffffffff) << 0)
#define   NFP3800_CLSX_TRNG_TRNGMonitorOnesCount_Value_of(_x) (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: MonitorRunLength - Longest permitted and actual consecutive same-bit valid data from the whitener
    Bits: 31:16 Max(rw) - Maximum run length permitted within a whiteness monitoring period without generating an alert
    Bits: 15:0  Value(ro) - Maximum run length seen within the last whiteness monitoring period
*/
#define NFP3800_CLSX_TRNG_MonitorRunLength                   0x00000040
#define   NFP3800_CLSX_TRNG_MonitorRunLength_Max(_x)         (((_x) & 0xffff) << 16)
#define   NFP3800_CLSX_TRNG_MonitorRunLength_Max_of(_x)      (((_x) >> 16) & 0xffff)
#define   NFP3800_CLSX_TRNG_MonitorRunLength_Value(_x)       (((_x) & 0xffff) << 0)
#define   NFP3800_CLSX_TRNG_MonitorRunLength_Value_of(_x)    (((_x) >> 0) & 0xffff)
/*----------------------------------------------------------------
  Register: Lock - Control write access to the TRNG through XPB
    Bits:  1    WriteWarn(wo) - Enable the alert on writes; this bit can only be cleared by reset
    Bits:  0    WriteLock(wo) - Lockout all XPB writes; any subsequent write will generate an alert, and the content will be ignored
*/
#define NFP3800_CLSX_TRNG_Lock                               0x00000050
#define   NFP3800_CLSX_TRNG_Lock_WriteWarn                   BIT(1)
#define   NFP3800_CLSX_TRNG_Lock_WriteLock                   BIT(0)
/*----------------------------------------------------------------
  Register: Alerts - Readable alert status - contents clear only on reset
    Bits: 11    DataTooSlow(ro) - Set to indicate that the whitener did not produce 64 valid data bits within the GeneratorTimeout period
    Bits: 10    WhitenessRunLength(ro) - The maximum run length of zeros or ones has been exceeded in the whiteness monitor
    Bits:  9    TooManyOnes(ro) - Set if too many valid ones were seen by the whiteness monitor
    Bits:  8    TooFewOnes(ro) - Set if too few valid ones were seen by the whiteness monitor
    Bits:  2    RtcErrored(ro) - Indicates the async RTC was not ready when it was expected to be; this should never set, and is here for sanity
    Bits:  1    WriteWarned(ro) - Indicatest a write was attempted after the WriteWarn bit was set.
    Bits:  0    LockedOut(ro) - Indicates a write was attempted after writes were locked out. The write will have been ignored.
*/
#define NFP3800_CLSX_TRNG_Alerts                             0x00000054
#define   NFP3800_CLSX_TRNG_Alerts_DataTooSlow               BIT(11)
#define   NFP3800_CLSX_TRNG_Alerts_WhitenessRunLength        BIT(10)
#define   NFP3800_CLSX_TRNG_Alerts_TooManyOnes               BIT(9)
#define   NFP3800_CLSX_TRNG_Alerts_TooFewOnes                BIT(8)
#define   NFP3800_CLSX_TRNG_Alerts_RtcErrored                BIT(2)
#define   NFP3800_CLSX_TRNG_Alerts_WriteWarned               BIT(1)
#define   NFP3800_CLSX_TRNG_Alerts_LockedOut                 BIT(0)

#endif /* NFP3800_LONGNAMES */


#endif /* NFP3800_CLS_H */
