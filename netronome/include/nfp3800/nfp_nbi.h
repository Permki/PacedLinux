/*
 * Copyright (C) 2013, Netronome Systems, Inc.  All rights reserved.
 *
 * Disclaimer: this file is provided without any warranty
 * as part of an early-access program, and the content is
 * bound to change before the final release.
 */

#ifndef NFP3800_NBI_H
#define NFP3800_NBI_H

/*================================================================*/
/* TGT 1                                                          */

/*----------------------------------------------------------------*/
/* CPP Address Map (CPP_BASE = IMB translation: island + indexes) */

/* CPP_BASE + (nfp_nbi_dma.h) */
/* CPP_BASE + (nfp_nbi_mac.h) */
/* CPP_BASE + (nfp_nbi_tm.h) */
/* CPP_BASE + (nfp_nbi_pc.h) */

/*----------------------------------------------------------------*/
/* XPB Address Map (XPB_BASE = NFP3800_XPB_ISLAND(island))          */

#define NBIX_BASE                                            (0xa0000)

/* XPB_BASE + (nfp_nbi_dma.h) */
/* XPB_BASE + (nfp_nbi_tm.h) */
/* XPB_BASE + (nfp_nbi_pc.h) */
/* XPB_BASE + (nfp_nbi_pm.h) */
/* XPB_BASE + NFP3800_NBIX_IM + (nfp_nbi_im.h) */
#define NFP3800_NBIX_IM                                          (NBIX_BASE + 0x2e0000)
/* XPB_BASE + NFP3800_NBIX_CSR + NFP3800_NBI_CSR_* */
#define NFP3800_NBIX_CSR                                         (NBIX_BASE + 0x2f0000)
/* XPB_BASE + (nfp_nbi_mac.h) */

/*================================================================*/


/* HGID: 6ea1af6fca88 */

#ifndef NFP3800_LONGNAMES

/*----------------------------------------------------------------
  Register: NbiMuXlate - NBI Memory Unit Address Translation register
    Bits: 15:13 AccMode(rw) - The AccMode field is used to determine how the destination island number is formed from the address of the MU Write or MU Read command.
    Bits: 11:6  Island1(rw) - The Island1 field is one of 2 selections made during MU address translation
    Bits:  5:0  Island0(rw) - The Island0 field is one of 2 selections made during MU address translation
*/
#define NFP3800_NBIX_CSR_NBI_MU_XLATE                        0x00000000
#define   NFP3800_NBIX_CSR_NBI_MU_XLATE_ACCESS_MODE(_x)      (((_x) & 0x7) << 13)
#define   NFP3800_NBIX_CSR_NBI_MU_XLATE_ACCESS_MODE_of(_x)   (((_x) >> 13) & 0x7)
#define   NFP3800_NBIX_CSR_NBI_MU_XLATE_ISLAND1(_x)          (((_x) & 0x3f) << 6)
#define   NFP3800_NBIX_CSR_NBI_MU_XLATE_ISLAND1_of(_x)       (((_x) >> 6) & 0x3f)
#define   NFP3800_NBIX_CSR_NBI_MU_XLATE_ISLAND0(_x)          (((_x) & 0x3f) << 0)
#define   NFP3800_NBIX_CSR_NBI_MU_XLATE_ISLAND0_of(_x)       (((_x) >> 0) & 0x3f)
/*----------------------------------------------------------------
  Register: NbiStat - NBI Interrupt Status Register
    Bits: 28    NbiXpbMstrTimeOutE(rw1c) - Indicates a xpb master timeout error occurred
    Bits: 27    NbiPushPullP7BE(rw1c) - Indicates a push/pull error detected on port7b
    Bits: 26    NbiPushPullP7AE(rw1c) - Indicates a push/pull error detected on port7a
    Bits: 25    NbiPushPullP6BE(rw1c) - Indicates a push/pull error detected on port6b
    Bits: 24    NbiPushPullP6AE(rw1c) - Indicates a push/pull error detected on port6a
    Bits: 23    NbiMstrPullidP7E(rw1c) - Indicates a master pullid error detected on port7
    Bits: 22    NbiMstrPullidP6E(rw1c) - Indicates a master pullid error detected on port6
    Bits: 21    NbiTgtCmdP7E(rw1c) - Indicates a target command error detected on port7
    Bits: 20    NbiTgtCmdP6E(rw1c) - Indicates a target command error detected on port6
    Bits: 19:16 BLQFull(rw1c) - Indicates the queue became full. One bit per Egress Buffer List Queue.
    Bits: 15:12 BLQEmpty(rw1c) - Indicates the queue went empty. One bit per Ingress Buffer List Queue.
    Bits: 11    NbiPmPktErrE(rw1c) - This is an event register which is set if two consecutive 'sop' are seen without an 'eop' in-between them.
    Bits: 10    NbiPmModscrDecodeErrE(rw1c) - This is an event register which is set if the modification script that is prepended to the packet has errors, the packet modifier can not decode the script correctly. Following are the contributors: 1. 'mod_instruct' == del and 'rdata_len' != 0 for direct script, 2. 'mod_instruct' == mrep and 'rdata_len' != 2*'num_bytes'+1 for direct script, 3. rdata_len > 16 for direct script, 4. rdata_len != 'num_bytes'+1 for direct script 5. offset_len+1 doesn't match the number of offsets provided in the offset field, 6. rdata_len doesn't match the number of bytes in the rdata field, 7. offsets are not in the ascending order, 8. an opcode of 0xe0 found in between the other valid opcodes
    Bits:  9    CppDataOutErr(rw1c) - An error from the NBI propogated onto the DSF CPP output data bus
    Bits:  8    CppDataInErr(rw1c) - Indicates data received from the DSF CPP contained an error.
    Bits:  7:0  BPDrop(rw1c) - Indicates a packet was dropped due to lack of buffer or packet credits. One bit per buffer pool
*/
#define NFP3800_NBIX_CSR_NBI_STAT                            0x00000004
#define   NFP3800_NBIX_CSR_NBI_STAT_NBI_XPB_MSTR_TIMEOUT_ERR_E BIT(28)
#define   NFP3800_NBIX_CSR_NBI_STAT_NBI_PUSHPULL_P7B_ERR_E   BIT(27)
#define   NFP3800_NBIX_CSR_NBI_STAT_NBI_PUSHPULL_P7A_ERR_E   BIT(26)
#define   NFP3800_NBIX_CSR_NBI_STAT_NBI_PUSHPULL_P6B_ERR_E   BIT(25)
#define   NFP3800_NBIX_CSR_NBI_STAT_NBI_PUSHPULL_P6A_ERR_E   BIT(24)
#define   NFP3800_NBIX_CSR_NBI_STAT_NBI_MSTR_PULLID_P7_ERR_E BIT(23)
#define   NFP3800_NBIX_CSR_NBI_STAT_NBI_MSTR_PULLID_P6_ERR_E BIT(22)
#define   NFP3800_NBIX_CSR_NBI_STAT_NBI_TGT_CMD_P7_ERR_E     BIT(21)
#define   NFP3800_NBIX_CSR_NBI_STAT_NBI_TGT_CMD_P6_ERR_E     BIT(20)
#define   NFP3800_NBIX_CSR_NBI_STAT_BL_QUE_FULL(_x)          (((_x) & 0xf) << 16)
#define   NFP3800_NBIX_CSR_NBI_STAT_BL_QUE_FULL_of(_x)       (((_x) >> 16) & 0xf)
#define   NFP3800_NBIX_CSR_NBI_STAT_BL_QUE_EMPTY(_x)         (((_x) & 0xf) << 12)
#define   NFP3800_NBIX_CSR_NBI_STAT_BL_QUE_EMPTY_of(_x)      (((_x) >> 12) & 0xf)
#define   NFP3800_NBIX_CSR_NBI_STAT_NBI_PM_PKT_ERR_E         BIT(11)
#define   NFP3800_NBIX_CSR_NBI_STAT_NBI_PM_MODSCR_DECODE_ERR_E BIT(10)
#define   NFP3800_NBIX_CSR_NBI_STAT_CPP_DATA_OUT_ERR         BIT(9)
#define   NFP3800_NBIX_CSR_NBI_STAT_CPP_DATA_IN_ERR          BIT(8)
#define   NFP3800_NBIX_CSR_NBI_STAT_BP_DROP(_x)              (((_x) & 0xff) << 0)
#define   NFP3800_NBIX_CSR_NBI_STAT_BP_DROP_of(_x)           (((_x) >> 0) & 0xff)
/*----------------------------------------------------------------
  Register: NbiCtrl - NBI Control Register
    Bits: 10    NoMuptrOffset(rw) - During Split packets - this bit determines where to DMA the packet to/from MU.
    Bits:  9    NbiIgrsDataRateEna(rw) - Enables Data Rate monitoring on the Ingress Minipkt interface.
    Bits:  8    NbiIgrsPktRateEna(rw) - Enables Packet Rate monitoring on the Ingress Minipkt interface.
    Bits:  7    NbiEgrsDataRateEna(rw) - Enables Data Rate monitoring on the Egress Minipkt interface.
    Bits:  6    NbiEgrsPktRateEna(rw) - Enables Packet Rate monitoring on the Egress Minipkt interface.
    Bits:  5    CppTargetCppTxErrEna(rw) - When set, CPP Data errors from the CPP Target Logic are forwarded to the DSF CPP bus. When cleared, CPP Data errors from the CPP Target Logic are not forwarded to the DSF CPP bus.
    Bits:  4    CppTargetCppRxErrEna(rw) - When set, received DSF CPP Data errors are forwarded to the CPP Target logic. When cleared, CPP Data errors are not forwarded to the CPP Target logic
    Bits:  3    IngDmaCppTxErrEna(rw) - When set, CPP Data errors from the Ingress DMA engine are forwarded to the DSF CPP bus. When cleared, CPP Data errors from the Ingress DMA engine are not forwarded to the DSF CPP bus.
    Bits:  2    IngDmaCppRxErrEna(rw) - When set, received DSF CPP Data errors are forwarded to the Ingress DMA engine. When cleared, CPP Data errors are not forwarded to the Ingress DMA engine
    Bits:  1    EgrDmaCppTxErrEna(rw) - When set, CPP Data errors from the Egress DMA engine are forwarded to the DSF CPP bus. When cleared, CPP Data errors from the Egress DMA engine are not forwarded to the DSF CPP bus.
    Bits:  0    EgrDmaCppRxErrEna(rw) - When set, received DSF CPP Data errors are forwarded to the Egress DMA engine. When cleared, CPP Data errors are not forwarded to the Egress DMA engine
*/
#define NFP3800_NBIX_CSR_NBI_CTRL                            0x00000008
#define   NFP3800_NBIX_CSR_NBI_CTRL_NO_MUPTR_OFFSET          BIT(10)
#define     NFP3800_NBIX_CSR_NBI_CTRL_NO_MUPTR_OFFSET_0      (0 << 10)
#define     NFP3800_NBIX_CSR_NBI_CTRL_NO_MUPTR_OFFSET_1      BIT(10)
#define   NFP3800_NBIX_CSR_NBI_CTRL_NBI_IGRS_DATA_RATE_ENA   BIT(9)
#define   NFP3800_NBIX_CSR_NBI_CTRL_NBI_IGRS_PKT_RATE_ENA    BIT(8)
#define   NFP3800_NBIX_CSR_NBI_CTRL_NBI_EGRS_DATA_RATE_ENA   BIT(7)
#define   NFP3800_NBIX_CSR_NBI_CTRL_NBI_EGRS_PKT_RATE_ENA    BIT(6)
#define   NFP3800_NBIX_CSR_NBI_CTRL_CPP_TARGET_CPP_TX_ERR_ENA BIT(5)
#define   NFP3800_NBIX_CSR_NBI_CTRL_CPP_TARGET_CPP_RX_ERR_ENA BIT(4)
#define   NFP3800_NBIX_CSR_NBI_CTRL_ING_DMA_CPP_TX_ERR_ENA   BIT(3)
#define   NFP3800_NBIX_CSR_NBI_CTRL_ING_DMA_CPP_RX_ERR_ENA   BIT(2)
#define   NFP3800_NBIX_CSR_NBI_CTRL_EGR_DMA_CPP_TX_ERR_ENA   BIT(1)
#define   NFP3800_NBIX_CSR_NBI_CTRL_EGR_DMA_CPP_RX_ERR_ENA   BIT(0)
/*----------------------------------------------------------------
  Register: NbiXpbMaxTo - XPB Maximum Timeout Limit
    Bits: 14:0  XpbMaxTimeoutCnt(rw) - Configures the allowed maximum number of clock cycles for an XPB target to respond in before a timeout is declared.
*/
#define NFP3800_NBIX_CSR_NBI_XPB_MAXTO                       0x0000000c
#define   NFP3800_NBIX_CSR_NBI_XPB_MAXTO_XPB_MAX_TO_CNT(_x)  (((_x) & 0x7fff) << 0)
#define   NFP3800_NBIX_CSR_NBI_XPB_MAXTO_XPB_MAX_TO_CNT_of(_x) (((_x) >> 0) & 0x7fff)
/*----------------------------------------------------------------
  Register: NbiMacRd256XpbMaxTo - MAC Read 256 XPB Maximum Timeout Limit
    Bits: 14:0  XpbRd256MaxTimeoutCnt(rw) - Configures the allowed maximum number of clock cycles for the XPB RD256 target to respond in before a timeout is declared.
*/
#define NFP3800_NBIX_CSR_NBI_MACRD256_XPB_MAXTO              0x00000010
#define   NFP3800_NBIX_CSR_NBI_MACRD256_XPB_MAXTO_XPB_RD256_MAX_TO_CNT(_x) (((_x) & 0x7fff) << 0)
#define   NFP3800_NBIX_CSR_NBI_MACRD256_XPB_MAXTO_XPB_RD256_MAX_TO_CNT_of(_x) (((_x) >> 0) & 0x7fff)
/*----------------------------------------------------------------
  Register: NbiXpbToStat - Provides information about the last XPB timeout error
    Bits: 16    TimeOutError(rw1c) - When set, a timeout error occurred during an XPB access.
    Bits: 15:8  ClusterId(ro) - Cluster ID of the target being accessed during the last XPB timout.
    Bits:  7:0  DeviceId(ro) - Device ID of the XPB Target being accessed during the last XPB timout.
*/
#define NFP3800_NBIX_CSR_NBI_XPB_TOSTAT                      0x00000014
#define   NFP3800_NBIX_CSR_NBI_XPB_TOSTAT_TO_ERROR           BIT(16)
#define   NFP3800_NBIX_CSR_NBI_XPB_TOSTAT_CLUSTER_ID(_x)     (((_x) & 0xff) << 8)
#define   NFP3800_NBIX_CSR_NBI_XPB_TOSTAT_CLUSTER_ID_of(_x)  (((_x) >> 8) & 0xff)
#define   NFP3800_NBIX_CSR_NBI_XPB_TOSTAT_DEVID(_x)          (((_x) & 0xff) << 0)
#define   NFP3800_NBIX_CSR_NBI_XPB_TOSTAT_DEVID_of(_x)       (((_x) >> 0) & 0xff)
/*----------------------------------------------------------------
  Register: NbiMacRd256XpbToStat - Provides information about the last MAC RD256 XPB timeout error
    Bits: 16    TimeOutError(rw1c) - When set, a timeout error occurred during an XPB RD256 access.
    Bits: 15:8  ClusterId(ro) - Cluster ID of the target being accessed during the last XPB RD256 timout.
    Bits:  7:0  DeviceId(ro) - Device ID of the XPB Target being accessed during the last XPB RD256 timout.
*/
#define NFP3800_NBIX_CSR_NBI_MACRD256_XPB_TOSTAT             0x00000018
#define   NFP3800_NBIX_CSR_NBI_MACRD256_XPB_TOSTAT_TO_ERROR  BIT(16)
#define   NFP3800_NBIX_CSR_NBI_MACRD256_XPB_TOSTAT_CLUSTER_ID(_x) (((_x) & 0xff) << 8)
#define   NFP3800_NBIX_CSR_NBI_MACRD256_XPB_TOSTAT_CLUSTER_ID_of(_x) (((_x) >> 8) & 0xff)
#define   NFP3800_NBIX_CSR_NBI_MACRD256_XPB_TOSTAT_DEVID(_x) (((_x) & 0xff) << 0)
#define   NFP3800_NBIX_CSR_NBI_MACRD256_XPB_TOSTAT_DEVID_of(_x) (((_x) >> 0) & 0xff)
/*----------------------------------------------------------------
  Register: NbiDebugCsr - NBI Debug CSR
    Bits:  1:0  CppPktEndianness(rw) - DEBUG Only register: Determines the packet endianess sent and expected endianess received on the DSF CPP data buses.
*/
#define NFP3800_NBIX_CSR_NBI_DEBUG_CSR                       0x0000005c
#define   NFP3800_NBIX_CSR_NBI_DEBUG_CSR_CPP_PKT_ENDIANNESS(_x) (((_x) & 0x3) << 0)
#define   NFP3800_NBIX_CSR_NBI_DEBUG_CSR_CPP_PKT_ENDIANNESS_of(_x) (((_x) >> 0) & 0x3)
#define     NFP3800_NBIX_CSR_NBI_DEBUG_CSR_CPP_PKT_ENDIANNESS_LWBE (0)
#define     NFP3800_NBIX_CSR_NBI_DEBUG_CSR_CPP_PKT_ENDIANNESS_LE (1)
#define     NFP3800_NBIX_CSR_NBI_DEBUG_CSR_CPP_PKT_ENDIANNESS_BE (2)
#define     NFP3800_NBIX_CSR_NBI_DEBUG_CSR_CPP_PKT_ENDIANNESS_BWLE (3)
/*----------------------------------------------------------------
  Register: NbiEgrsRate - NBI Egress Packet/Data Rate Monitor
    Bits: 31:16 EgrsDataRate(ro) - DEBUG Only register: Provides Egress Data rate when enabled.
    Bits: 15:0  EgrsPktRate(ro) - DEBUG Only register: Provides Egress Packet rate when enabled.
*/
#define NFP3800_NBIX_CSR_NBI_EGRS_RATE                       0x00000060
#define   NFP3800_NBIX_CSR_NBI_EGRS_RATE_EGRS_DATA_RATE(_x)  (((_x) & 0xffff) << 16)
#define   NFP3800_NBIX_CSR_NBI_EGRS_RATE_EGRS_DATA_RATE_of(_x) (((_x) >> 16) & 0xffff)
#define   NFP3800_NBIX_CSR_NBI_EGRS_RATE_EGRS_PKT_RATE(_x)   (((_x) & 0xffff) << 0)
#define   NFP3800_NBIX_CSR_NBI_EGRS_RATE_EGRS_PKT_RATE_of(_x) (((_x) >> 0) & 0xffff)
/*----------------------------------------------------------------
  Register: NbiIgrsRate - NBI Ingress Packet/Data Rate Monitor
    Bits: 31:16 IgrsDataRate(ro) - DEBUG Only register: Provides Igress Data rate when enabled.
    Bits: 15:0  IgrsPktRate(ro) - DEBUG Only register: Provides Ingress Packet rate when enabled.
*/
#define NFP3800_NBIX_CSR_NBI_IGRS_RATE                       0x00000064
#define   NFP3800_NBIX_CSR_NBI_IGRS_RATE_IGRS_DATA_RATE(_x)  (((_x) & 0xffff) << 16)
#define   NFP3800_NBIX_CSR_NBI_IGRS_RATE_IGRS_DATA_RATE_of(_x) (((_x) >> 16) & 0xffff)
#define   NFP3800_NBIX_CSR_NBI_IGRS_RATE_IGRS_PKT_RATE(_x)   (((_x) & 0xffff) << 0)
#define   NFP3800_NBIX_CSR_NBI_IGRS_RATE_IGRS_PKT_RATE_of(_x) (((_x) >> 0) & 0xffff)

#else /* NFP3800_LONGNAMES */

/*----------------------------------------------------------------
  Register: NbiMuXlate - NBI Memory Unit Address Translation register
    Bits: 15:13 AccMode(rw) - The AccMode field is used to determine how the destination island number is formed from the address of the MU Write or MU Read command.
    Bits: 11:6  Island1(rw) - The Island1 field is one of 2 selections made during MU address translation
    Bits:  5:0  Island0(rw) - The Island0 field is one of 2 selections made during MU address translation
*/
#define NFP3800_NBIX_CSR_NbiMuXlate                          0x00000000
#define   NFP3800_NBIX_CSR_NbiMuXlate_AccMode(_x)            (((_x) & 0x7) << 13)
#define   NFP3800_NBIX_CSR_NbiMuXlate_AccMode_of(_x)         (((_x) >> 13) & 0x7)
#define   NFP3800_NBIX_CSR_NbiMuXlate_Island1(_x)            (((_x) & 0x3f) << 6)
#define   NFP3800_NBIX_CSR_NbiMuXlate_Island1_of(_x)         (((_x) >> 6) & 0x3f)
#define   NFP3800_NBIX_CSR_NbiMuXlate_Island0(_x)            (((_x) & 0x3f) << 0)
#define   NFP3800_NBIX_CSR_NbiMuXlate_Island0_of(_x)         (((_x) >> 0) & 0x3f)
/*----------------------------------------------------------------
  Register: NbiStat - NBI Interrupt Status Register
    Bits: 28    NbiXpbMstrTimeOutE(rw1c) - Indicates a xpb master timeout error occurred
    Bits: 27    NbiPushPullP7BE(rw1c) - Indicates a push/pull error detected on port7b
    Bits: 26    NbiPushPullP7AE(rw1c) - Indicates a push/pull error detected on port7a
    Bits: 25    NbiPushPullP6BE(rw1c) - Indicates a push/pull error detected on port6b
    Bits: 24    NbiPushPullP6AE(rw1c) - Indicates a push/pull error detected on port6a
    Bits: 23    NbiMstrPullidP7E(rw1c) - Indicates a master pullid error detected on port7
    Bits: 22    NbiMstrPullidP6E(rw1c) - Indicates a master pullid error detected on port6
    Bits: 21    NbiTgtCmdP7E(rw1c) - Indicates a target command error detected on port7
    Bits: 20    NbiTgtCmdP6E(rw1c) - Indicates a target command error detected on port6
    Bits: 19:16 BLQFull(rw1c) - Indicates the queue became full. One bit per Egress Buffer List Queue.
    Bits: 15:12 BLQEmpty(rw1c) - Indicates the queue went empty. One bit per Ingress Buffer List Queue.
    Bits: 11    NbiPmPktErrE(rw1c) - This is an event register which is set if two consecutive 'sop' are seen without an 'eop' in-between them.
    Bits: 10    NbiPmModscrDecodeErrE(rw1c) - This is an event register which is set if the modification script that is prepended to the packet has errors, the packet modifier can not decode the script correctly. Following are the contributors: 1. 'mod_instruct' == del and 'rdata_len' != 0 for direct script, 2. 'mod_instruct' == mrep and 'rdata_len' != 2*'num_bytes'+1 for direct script, 3. rdata_len > 16 for direct script, 4. rdata_len != 'num_bytes'+1 for direct script 5. offset_len+1 doesn't match the number of offsets provided in the offset field, 6. rdata_len doesn't match the number of bytes in the rdata field, 7. offsets are not in the ascending order, 8. an opcode of 0xe0 found in between the other valid opcodes
    Bits:  9    CppDataOutErr(rw1c) - An error from the NBI propogated onto the DSF CPP output data bus
    Bits:  8    CppDataInErr(rw1c) - Indicates data received from the DSF CPP contained an error.
    Bits:  7:0  BPDrop(rw1c) - Indicates a packet was dropped due to lack of buffer or packet credits. One bit per buffer pool
*/
#define NFP3800_NBIX_CSR_NbiStat                             0x00000004
#define   NFP3800_NBIX_CSR_NbiStat_NbiXpbMstrTimeOutE        BIT(28)
#define   NFP3800_NBIX_CSR_NbiStat_NbiPushPullP7BE           BIT(27)
#define   NFP3800_NBIX_CSR_NbiStat_NbiPushPullP7AE           BIT(26)
#define   NFP3800_NBIX_CSR_NbiStat_NbiPushPullP6BE           BIT(25)
#define   NFP3800_NBIX_CSR_NbiStat_NbiPushPullP6AE           BIT(24)
#define   NFP3800_NBIX_CSR_NbiStat_NbiMstrPullidP7E          BIT(23)
#define   NFP3800_NBIX_CSR_NbiStat_NbiMstrPullidP6E          BIT(22)
#define   NFP3800_NBIX_CSR_NbiStat_NbiTgtCmdP7E              BIT(21)
#define   NFP3800_NBIX_CSR_NbiStat_NbiTgtCmdP6E              BIT(20)
#define   NFP3800_NBIX_CSR_NbiStat_BLQFull(_x)               (((_x) & 0xf) << 16)
#define   NFP3800_NBIX_CSR_NbiStat_BLQFull_of(_x)            (((_x) >> 16) & 0xf)
#define   NFP3800_NBIX_CSR_NbiStat_BLQEmpty(_x)              (((_x) & 0xf) << 12)
#define   NFP3800_NBIX_CSR_NbiStat_BLQEmpty_of(_x)           (((_x) >> 12) & 0xf)
#define   NFP3800_NBIX_CSR_NbiStat_NbiPmPktErrE              BIT(11)
#define   NFP3800_NBIX_CSR_NbiStat_NbiPmModscrDecodeErrE     BIT(10)
#define   NFP3800_NBIX_CSR_NbiStat_CppDataOutErr             BIT(9)
#define   NFP3800_NBIX_CSR_NbiStat_CppDataInErr              BIT(8)
#define   NFP3800_NBIX_CSR_NbiStat_BPDrop(_x)                (((_x) & 0xff) << 0)
#define   NFP3800_NBIX_CSR_NbiStat_BPDrop_of(_x)             (((_x) >> 0) & 0xff)
/*----------------------------------------------------------------
  Register: NbiCtrl - NBI Control Register
    Bits: 10    NoMuptrOffset(rw) - During Split packets - this bit determines where to DMA the packet to/from MU.
    Bits:  9    NbiIgrsDataRateEna(rw) - Enables Data Rate monitoring on the Ingress Minipkt interface.
    Bits:  8    NbiIgrsPktRateEna(rw) - Enables Packet Rate monitoring on the Ingress Minipkt interface.
    Bits:  7    NbiEgrsDataRateEna(rw) - Enables Data Rate monitoring on the Egress Minipkt interface.
    Bits:  6    NbiEgrsPktRateEna(rw) - Enables Packet Rate monitoring on the Egress Minipkt interface.
    Bits:  5    CppTargetCppTxErrEna(rw) - When set, CPP Data errors from the CPP Target Logic are forwarded to the DSF CPP bus. When cleared, CPP Data errors from the CPP Target Logic are not forwarded to the DSF CPP bus.
    Bits:  4    CppTargetCppRxErrEna(rw) - When set, received DSF CPP Data errors are forwarded to the CPP Target logic. When cleared, CPP Data errors are not forwarded to the CPP Target logic
    Bits:  3    IngDmaCppTxErrEna(rw) - When set, CPP Data errors from the Ingress DMA engine are forwarded to the DSF CPP bus. When cleared, CPP Data errors from the Ingress DMA engine are not forwarded to the DSF CPP bus.
    Bits:  2    IngDmaCppRxErrEna(rw) - When set, received DSF CPP Data errors are forwarded to the Ingress DMA engine. When cleared, CPP Data errors are not forwarded to the Ingress DMA engine
    Bits:  1    EgrDmaCppTxErrEna(rw) - When set, CPP Data errors from the Egress DMA engine are forwarded to the DSF CPP bus. When cleared, CPP Data errors from the Egress DMA engine are not forwarded to the DSF CPP bus.
    Bits:  0    EgrDmaCppRxErrEna(rw) - When set, received DSF CPP Data errors are forwarded to the Egress DMA engine. When cleared, CPP Data errors are not forwarded to the Egress DMA engine
*/
#define NFP3800_NBIX_CSR_NbiCtrl                             0x00000008
#define   NFP3800_NBIX_CSR_NbiCtrl_NoMuptrOffset             BIT(10)
#define     NFP3800_NBIX_CSR_NbiCtrl_NoMuptrOffset_0         (0 << 10)
#define     NFP3800_NBIX_CSR_NbiCtrl_NoMuptrOffset_1         BIT(10)
#define   NFP3800_NBIX_CSR_NbiCtrl_NbiIgrsDataRateEna        BIT(9)
#define   NFP3800_NBIX_CSR_NbiCtrl_NbiIgrsPktRateEna         BIT(8)
#define   NFP3800_NBIX_CSR_NbiCtrl_NbiEgrsDataRateEna        BIT(7)
#define   NFP3800_NBIX_CSR_NbiCtrl_NbiEgrsPktRateEna         BIT(6)
#define   NFP3800_NBIX_CSR_NbiCtrl_CppTargetCppTxErrEna      BIT(5)
#define   NFP3800_NBIX_CSR_NbiCtrl_CppTargetCppRxErrEna      BIT(4)
#define   NFP3800_NBIX_CSR_NbiCtrl_IngDmaCppTxErrEna         BIT(3)
#define   NFP3800_NBIX_CSR_NbiCtrl_IngDmaCppRxErrEna         BIT(2)
#define   NFP3800_NBIX_CSR_NbiCtrl_EgrDmaCppTxErrEna         BIT(1)
#define   NFP3800_NBIX_CSR_NbiCtrl_EgrDmaCppRxErrEna         BIT(0)
/*----------------------------------------------------------------
  Register: NbiXpbMaxTo - XPB Maximum Timeout Limit
    Bits: 14:0  XpbMaxTimeoutCnt(rw) - Configures the allowed maximum number of clock cycles for an XPB target to respond in before a timeout is declared.
*/
#define NFP3800_NBIX_CSR_NbiXpbMaxTo                         0x0000000c
#define   NFP3800_NBIX_CSR_NbiXpbMaxTo_XpbMaxTimeoutCnt(_x)  (((_x) & 0x7fff) << 0)
#define   NFP3800_NBIX_CSR_NbiXpbMaxTo_XpbMaxTimeoutCnt_of(_x) (((_x) >> 0) & 0x7fff)
/*----------------------------------------------------------------
  Register: NbiMacRd256XpbMaxTo - MAC Read 256 XPB Maximum Timeout Limit
    Bits: 14:0  XpbRd256MaxTimeoutCnt(rw) - Configures the allowed maximum number of clock cycles for the XPB RD256 target to respond in before a timeout is declared.
*/
#define NFP3800_NBIX_CSR_NbiMacRd256XpbMaxTo                 0x00000010
#define   NFP3800_NBIX_CSR_NbiMacRd256XpbMaxTo_XpbRd256MaxTimeoutCnt(_x) (((_x) & 0x7fff) << 0)
#define   NFP3800_NBIX_CSR_NbiMacRd256XpbMaxTo_XpbRd256MaxTimeoutCnt_of(_x) (((_x) >> 0) & 0x7fff)
/*----------------------------------------------------------------
  Register: NbiXpbToStat - Provides information about the last XPB timeout error
    Bits: 16    TimeOutError(rw1c) - When set, a timeout error occurred during an XPB access.
    Bits: 15:8  ClusterId(ro) - Cluster ID of the target being accessed during the last XPB timout.
    Bits:  7:0  DeviceId(ro) - Device ID of the XPB Target being accessed during the last XPB timout.
*/
#define NFP3800_NBIX_CSR_NbiXpbToStat                        0x00000014
#define   NFP3800_NBIX_CSR_NbiXpbToStat_TimeOutError         BIT(16)
#define   NFP3800_NBIX_CSR_NbiXpbToStat_ClusterId(_x)        (((_x) & 0xff) << 8)
#define   NFP3800_NBIX_CSR_NbiXpbToStat_ClusterId_of(_x)     (((_x) >> 8) & 0xff)
#define   NFP3800_NBIX_CSR_NbiXpbToStat_DeviceId(_x)         (((_x) & 0xff) << 0)
#define   NFP3800_NBIX_CSR_NbiXpbToStat_DeviceId_of(_x)      (((_x) >> 0) & 0xff)
/*----------------------------------------------------------------
  Register: NbiMacRd256XpbToStat - Provides information about the last MAC RD256 XPB timeout error
    Bits: 16    TimeOutError(rw1c) - When set, a timeout error occurred during an XPB RD256 access.
    Bits: 15:8  ClusterId(ro) - Cluster ID of the target being accessed during the last XPB RD256 timout.
    Bits:  7:0  DeviceId(ro) - Device ID of the XPB Target being accessed during the last XPB RD256 timout.
*/
#define NFP3800_NBIX_CSR_NbiMacRd256XpbToStat                0x00000018
#define   NFP3800_NBIX_CSR_NbiMacRd256XpbToStat_TimeOutError BIT(16)
#define   NFP3800_NBIX_CSR_NbiMacRd256XpbToStat_ClusterId(_x) (((_x) & 0xff) << 8)
#define   NFP3800_NBIX_CSR_NbiMacRd256XpbToStat_ClusterId_of(_x) (((_x) >> 8) & 0xff)
#define   NFP3800_NBIX_CSR_NbiMacRd256XpbToStat_DeviceId(_x) (((_x) & 0xff) << 0)
#define   NFP3800_NBIX_CSR_NbiMacRd256XpbToStat_DeviceId_of(_x) (((_x) >> 0) & 0xff)
/*----------------------------------------------------------------
  Register: NbiDebugCsr - NBI Debug CSR
    Bits:  1:0  CppPktEndianness(rw) - DEBUG Only register: Determines the packet endianess sent and expected endianess received on the DSF CPP data buses.
*/
#define NFP3800_NBIX_CSR_NbiDebugCsr                         0x0000005c
#define   NFP3800_NBIX_CSR_NbiDebugCsr_CppPktEndianness(_x)  (((_x) & 0x3) << 0)
#define   NFP3800_NBIX_CSR_NbiDebugCsr_CppPktEndianness_of(_x) (((_x) >> 0) & 0x3)
#define     NFP3800_NBIX_CSR_NbiDebugCsr_CppPktEndianness_LWBE (0)
#define     NFP3800_NBIX_CSR_NbiDebugCsr_CppPktEndianness_LE (1)
#define     NFP3800_NBIX_CSR_NbiDebugCsr_CppPktEndianness_BE (2)
#define     NFP3800_NBIX_CSR_NbiDebugCsr_CppPktEndianness_BWLE (3)
/*----------------------------------------------------------------
  Register: NbiEgrsRate - NBI Egress Packet/Data Rate Monitor
    Bits: 31:16 EgrsDataRate(ro) - DEBUG Only register: Provides Egress Data rate when enabled.
    Bits: 15:0  EgrsPktRate(ro) - DEBUG Only register: Provides Egress Packet rate when enabled.
*/
#define NFP3800_NBIX_CSR_NbiEgrsRate                         0x00000060
#define   NFP3800_NBIX_CSR_NbiEgrsRate_EgrsDataRate(_x)      (((_x) & 0xffff) << 16)
#define   NFP3800_NBIX_CSR_NbiEgrsRate_EgrsDataRate_of(_x)   (((_x) >> 16) & 0xffff)
#define   NFP3800_NBIX_CSR_NbiEgrsRate_EgrsPktRate(_x)       (((_x) & 0xffff) << 0)
#define   NFP3800_NBIX_CSR_NbiEgrsRate_EgrsPktRate_of(_x)    (((_x) >> 0) & 0xffff)
/*----------------------------------------------------------------
  Register: NbiIgrsRate - NBI Ingress Packet/Data Rate Monitor
    Bits: 31:16 IgrsDataRate(ro) - DEBUG Only register: Provides Igress Data rate when enabled.
    Bits: 15:0  IgrsPktRate(ro) - DEBUG Only register: Provides Ingress Packet rate when enabled.
*/
#define NFP3800_NBIX_CSR_NbiIgrsRate                         0x00000064
#define   NFP3800_NBIX_CSR_NbiIgrsRate_IgrsDataRate(_x)      (((_x) & 0xffff) << 16)
#define   NFP3800_NBIX_CSR_NbiIgrsRate_IgrsDataRate_of(_x)   (((_x) >> 16) & 0xffff)
#define   NFP3800_NBIX_CSR_NbiIgrsRate_IgrsPktRate(_x)       (((_x) & 0xffff) << 0)
#define   NFP3800_NBIX_CSR_NbiIgrsRate_IgrsPktRate_of(_x)    (((_x) >> 0) & 0xffff)

#endif /* NFP3800_LONGNAMES */


#endif /* NFP3800_NBI_H */
