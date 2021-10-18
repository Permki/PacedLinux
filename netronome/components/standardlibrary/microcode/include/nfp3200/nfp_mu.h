/*
 * Copyright (C) 2012  Netronome Systems, Inc.  All rights reserved.
 *
 * @file        nfp3200/nfp_mu.h
 * @brief       Register definitions for Memory Unit
 *
 * Disclaimer: this file is provided without any warranty
 * as part of an early-access program, and the content is
 * bound to change before the final release.
 */

#ifndef __NFP_SDK__NFP3200_NFP_MU_H__
#define __NFP_SDK__NFP3200_NFP_MU_H__

/* HGID: nfp3200/mu.desc = 2ff0540d883f */
/* Register Type: MUConfigCPP */
#define NFP_MU_CFG                     0x0000
#define   NFP_MU_CFG_WAYS(_x)                           (((_x) & 0xff) << 8)
#define   NFP_MU_CFG_WAYS_bf                            0, 15, 8
#define   NFP_MU_CFG_WAYS_mask                          (0xff)
#define   NFP_MU_CFG_WAYS_shift                         (8)
#define   NFP_MU_CFG_TWO_SIGNALS                        (0x1 << 3)
#define   NFP_MU_CFG_TWO_SIGNALS_bf                     0, 3, 3
#define   NFP_MU_CFG_TWO_SIGNALS_mask                   (0x1)
#define   NFP_MU_CFG_TWO_SIGNALS_bit                    (3)
#define     NFP_MU_CFG_TWO_SIGNALS_ENABLE               (0x8)
#define   NFP_MU_CFG_ADDRESSING(_x)                     (((_x) & 0x3) << 1)
#define   NFP_MU_CFG_ADDRESSING_bf                      0, 2, 1
#define   NFP_MU_CFG_ADDRESSING_mask                    (0x3)
#define   NFP_MU_CFG_ADDRESSING_shift                   (1)
#define     NFP_MU_CFG_ADDRESSING_40BIT                 (0)
#define     NFP_MU_CFG_ADDRESSING_32BIT                 (1)
#define     NFP_MU_CFG_ADDRESSING_32BITHLR              (2)
#define     NFP_MU_CFG_ADDRESSING_32BITLLR              (3)
#define   NFP_MU_CFG_BULK_ALIGN                         (0x1)
#define   NFP_MU_CFG_BULK_ALIGN_bf                      0, 0, 0
#define   NFP_MU_CFG_BULK_ALIGN_mask                    (0x1)
#define   NFP_MU_CFG_BULK_ALIGN_bit                     (0)
#define     NFP_MU_CFG_BULK_ALIGN_ENFORCE               (0x0)
#define     NFP_MU_CFG_BULK_ALIGN_IGNORE                (0x1)
/* Register Type: MUConfigVQDR */
#define NFP_MU_VQDR_0                  0x0004
#define NFP_MU_VQDR_1                  0x0008
#define NFP_MU_VQDR_2                  0x000c
#define NFP_MU_VQDR_3                  0x0010
#define   NFP_MU_VQDR_QUEUE_TYPE(_x)                    (((_x) & 0x3) << 20)
#define   NFP_MU_VQDR_QUEUE_TYPE_bf                     0, 21, 20
#define   NFP_MU_VQDR_QUEUE_TYPE_mask                   (0x3)
#define   NFP_MU_VQDR_QUEUE_TYPE_shift                  (20)
#define     NFP_MU_VQDR_QUEUE_TYPE_SEGMENTS             (0)
#define     NFP_MU_VQDR_QUEUE_TYPE_PACKETS              (1)
#define     NFP_MU_VQDR_QUEUE_TYPE_PACKETS32            (2)
#define     NFP_MU_VQDR_QUEUE_TYPE_BUFFERS              (3)
#define   NFP_MU_VQDR_ENABLE                            (0x1 << 16)
#define   NFP_MU_VQDR_ENABLE_bf                         0, 16, 16
#define   NFP_MU_VQDR_ENABLE_mask                       (0x1)
#define   NFP_MU_VQDR_ENABLE_bit                        (16)
#define     NFP_MU_VQDR_ENABLE_SET                      (0x10000)
#define   NFP_MU_VQDR_BASE(_x)                          ((_x) & 0xffff)
#define   NFP_MU_VQDR_BASE_bf                           0, 15, 0
#define   NFP_MU_VQDR_BASE_mask                         (0xffff)
#define   NFP_MU_VQDR_BASE_shift                        (0)
/* Register Type: MUConfigDrams */
#define NFP_MU_DATA_MOVER              0x0014
#define   NFP_MU_DATA_MOVER_CH1_PANIC_FNP               (0x1 << 27)
#define   NFP_MU_DATA_MOVER_CH1_PANIC_FNP_bf            0, 27, 27
#define   NFP_MU_DATA_MOVER_CH1_PANIC_FNP_mask          (0x1)
#define   NFP_MU_DATA_MOVER_CH1_PANIC_FNP_bit           (27)
#define     NFP_MU_DATA_MOVER_CH1_PANIC_FNP_SET         (0x8000000)
#define   NFP_MU_DATA_MOVER_CH1_PANIC_FULL              (0x1 << 26)
#define   NFP_MU_DATA_MOVER_CH1_PANIC_FULL_bf           0, 26, 26
#define   NFP_MU_DATA_MOVER_CH1_PANIC_FULL_mask         (0x1)
#define   NFP_MU_DATA_MOVER_CH1_PANIC_FULL_bit          (26)
#define     NFP_MU_DATA_MOVER_CH1_PANIC_FULL_SET        (0x4000000)
#define   NFP_MU_DATA_MOVER_CH0_PANIC_FNP               (0x1 << 25)
#define   NFP_MU_DATA_MOVER_CH0_PANIC_FNP_bf            0, 25, 25
#define   NFP_MU_DATA_MOVER_CH0_PANIC_FNP_mask          (0x1)
#define   NFP_MU_DATA_MOVER_CH0_PANIC_FNP_bit           (25)
#define     NFP_MU_DATA_MOVER_CH0_PANIC_FNP_SET         (0x2000000)
#define   NFP_MU_DATA_MOVER_CH0_PANIC_FULL              (0x1 << 24)
#define   NFP_MU_DATA_MOVER_CH0_PANIC_FULL_bf           0, 24, 24
#define   NFP_MU_DATA_MOVER_CH0_PANIC_FULL_mask         (0x1)
#define   NFP_MU_DATA_MOVER_CH0_PANIC_FULL_bit          (24)
#define     NFP_MU_DATA_MOVER_CH0_PANIC_FULL_SET        (0x1000000)
#define   NFP_MU_DATA_MOVER_PACE_MODE(_x)               (((_x) & 0x3) << 20)
#define   NFP_MU_DATA_MOVER_PACE_MODE_bf                0, 21, 20
#define   NFP_MU_DATA_MOVER_PACE_MODE_mask              (0x3)
#define   NFP_MU_DATA_MOVER_PACE_MODE_shift             (20)
#define     NFP_MU_DATA_MOVER_PACE_MODE_OFF             (0)
#define     NFP_MU_DATA_MOVER_PACE_MODE_4               (1)
#define     NFP_MU_DATA_MOVER_PACE_MODE_5               (2)
#define     NFP_MU_DATA_MOVER_PACE_MODE_6               (3)
#define   NFP_MU_DATA_MOVER_DCACHE_DEPTH(_x)            (((_x) & 0x7) << 16)
#define   NFP_MU_DATA_MOVER_DCACHE_DEPTH_bf             0, 18, 16
#define   NFP_MU_DATA_MOVER_DCACHE_DEPTH_mask           (0x7)
#define   NFP_MU_DATA_MOVER_DCACHE_DEPTH_shift          (16)
#define     NFP_MU_DATA_MOVER_DCACHE_DEPTH_16           (0)
#define     NFP_MU_DATA_MOVER_DCACHE_DEPTH_14           (1)
#define     NFP_MU_DATA_MOVER_DCACHE_DEPTH_12           (2)
#define     NFP_MU_DATA_MOVER_DCACHE_DEPTH_10           (3)
#define     NFP_MU_DATA_MOVER_DCACHE_DEPTH_8            (4)
#define     NFP_MU_DATA_MOVER_DCACHE_DEPTH_6            (5)
#define     NFP_MU_DATA_MOVER_DCACHE_DEPTH_4            (6)
#define     NFP_MU_DATA_MOVER_DCACHE_DEPTH_2            (7)
#define   NFP_MU_DATA_MOVER_DIMMS                       (0x1 << 13)
#define   NFP_MU_DATA_MOVER_DIMMS_bf                    0, 13, 13
#define   NFP_MU_DATA_MOVER_DIMMS_mask                  (0x1)
#define   NFP_MU_DATA_MOVER_DIMMS_bit                   (13)
#define     NFP_MU_DATA_MOVER_DIMMS_1                   (0x0)
#define     NFP_MU_DATA_MOVER_DIMMS_2                   (0x2000)
#define   NFP_MU_DATA_MOVER_RANKS(_x)                   (((_x) & 0x3) << 11)
#define   NFP_MU_DATA_MOVER_RANKS_bf                    0, 12, 11
#define   NFP_MU_DATA_MOVER_RANKS_mask                  (0x3)
#define   NFP_MU_DATA_MOVER_RANKS_shift                 (11)
#define     NFP_MU_DATA_MOVER_RANKS_1                   (0)
#define     NFP_MU_DATA_MOVER_RANKS_2                   (1)
#define     NFP_MU_DATA_MOVER_RANKS_4                   (2)
#define   NFP_MU_DATA_MOVER_ROW_BITS(_x)                (((_x) & 0x7) << 8)
#define   NFP_MU_DATA_MOVER_ROW_BITS_bf                 0, 10, 8
#define   NFP_MU_DATA_MOVER_ROW_BITS_mask               (0x7)
#define   NFP_MU_DATA_MOVER_ROW_BITS_shift              (8)
#define     NFP_MU_DATA_MOVER_ROW_BITS_12               (0)
#define     NFP_MU_DATA_MOVER_ROW_BITS_13               (1)
#define     NFP_MU_DATA_MOVER_ROW_BITS_14               (2)
#define     NFP_MU_DATA_MOVER_ROW_BITS_15               (3)
#define     NFP_MU_DATA_MOVER_ROW_BITS_16               (4)
#define   NFP_MU_DATA_MOVER_TRC_TIMER(_x)               (((_x) & 0x7) << 5)
#define   NFP_MU_DATA_MOVER_TRC_TIMER_bf                0, 7, 5
#define   NFP_MU_DATA_MOVER_TRC_TIMER_mask              (0x7)
#define   NFP_MU_DATA_MOVER_TRC_TIMER_shift             (5)
#define     NFP_MU_DATA_MOVER_TRC_TIMER_40              (0)
#define     NFP_MU_DATA_MOVER_TRC_TIMER_39              (1)
#define     NFP_MU_DATA_MOVER_TRC_TIMER_38              (2)
#define     NFP_MU_DATA_MOVER_TRC_TIMER_37              (3)
#define     NFP_MU_DATA_MOVER_TRC_TIMER_36              (4)
#define     NFP_MU_DATA_MOVER_TRC_TIMER_35              (5)
#define     NFP_MU_DATA_MOVER_TRC_TIMER_34              (6)
#define     NFP_MU_DATA_MOVER_TRC_TIMER_33              (7)
#define   NFP_MU_DATA_MOVER_SNAPSHOT_TIMER(_x)          ((_x) & 0x1f)
#define   NFP_MU_DATA_MOVER_SNAPSHOT_TIMER_bf           0, 4, 0
#define   NFP_MU_DATA_MOVER_SNAPSHOT_TIMER_mask         (0x1f)
#define   NFP_MU_DATA_MOVER_SNAPSHOT_TIMER_shift        (0)
/* Register Type: MUConfigPerformanceBus */
#define NFP_MU_PERF_CFG                0x0018
#define   NFP_MU_PERF_CFG_RFU(_x)                       (((_x) & 0x3) << 30)
#define   NFP_MU_PERF_CFG_RFU_bf                        0, 31, 30
#define   NFP_MU_PERF_CFG_RFU_mask                      (0x3)
#define   NFP_MU_PERF_CFG_RFU_shift                     (30)
#define   NFP_MU_PERF_CFG_MOD_5_SEL(_x)                 (((_x) & 0xf) << 26)
#define   NFP_MU_PERF_CFG_MOD_5_SEL_bf                  0, 29, 26
#define   NFP_MU_PERF_CFG_MOD_5_SEL_mask                (0xf)
#define   NFP_MU_PERF_CFG_MOD_5_SEL_shift               (26)
#define   NFP_MU_PERF_CFG_MOD_4_SEL(_x)                 (((_x) & 0xf) << 22)
#define   NFP_MU_PERF_CFG_MOD_4_SEL_bf                  0, 25, 22
#define   NFP_MU_PERF_CFG_MOD_4_SEL_mask                (0xf)
#define   NFP_MU_PERF_CFG_MOD_4_SEL_shift               (22)
#define   NFP_MU_PERF_CFG_MOD_3_SEL(_x)                 (((_x) & 0xf) << 18)
#define   NFP_MU_PERF_CFG_MOD_3_SEL_bf                  0, 21, 18
#define   NFP_MU_PERF_CFG_MOD_3_SEL_mask                (0xf)
#define   NFP_MU_PERF_CFG_MOD_3_SEL_shift               (18)
#define   NFP_MU_PERF_CFG_MOD_2_SEL(_x)                 (((_x) & 0xf) << 14)
#define   NFP_MU_PERF_CFG_MOD_2_SEL_bf                  0, 17, 14
#define   NFP_MU_PERF_CFG_MOD_2_SEL_mask                (0xf)
#define   NFP_MU_PERF_CFG_MOD_2_SEL_shift               (14)
#define   NFP_MU_PERF_CFG_MOD_1_SEL(_x)                 (((_x) & 0xf) << 10)
#define   NFP_MU_PERF_CFG_MOD_1_SEL_bf                  0, 13, 10
#define   NFP_MU_PERF_CFG_MOD_1_SEL_mask                (0xf)
#define   NFP_MU_PERF_CFG_MOD_1_SEL_shift               (10)
#define   NFP_MU_PERF_CFG_MOD_0_SEL(_x)                 (((_x) & 0xf) << 6)
#define   NFP_MU_PERF_CFG_MOD_0_SEL_bf                  0, 9, 6
#define   NFP_MU_PERF_CFG_MOD_0_SEL_mask                (0xf)
#define   NFP_MU_PERF_CFG_MOD_0_SEL_shift               (6)
#define   NFP_MU_PERF_CFG_LANE_SEL_HI(_x)               (((_x) & 0x3) << 4)
#define   NFP_MU_PERF_CFG_LANE_SEL_HI_bf                0, 5, 4
#define   NFP_MU_PERF_CFG_LANE_SEL_HI_mask              (0x3)
#define   NFP_MU_PERF_CFG_LANE_SEL_HI_shift             (4)
#define     NFP_MU_PERF_CFG_LANE_SEL_HI_ZERO            (0)
#define     NFP_MU_PERF_CFG_LANE_SEL_HI_LOW             (1)
#define     NFP_MU_PERF_CFG_LANE_SEL_HI_MID             (2)
#define     NFP_MU_PERF_CFG_LANE_SEL_HI_HIGH            (3)
#define   NFP_MU_PERF_CFG_LANE_SEL_MID(_x)              (((_x) & 0x3) << 2)
#define   NFP_MU_PERF_CFG_LANE_SEL_MID_bf               0, 3, 2
#define   NFP_MU_PERF_CFG_LANE_SEL_MID_mask             (0x3)
#define   NFP_MU_PERF_CFG_LANE_SEL_MID_shift            (2)
#define     NFP_MU_PERF_CFG_LANE_SEL_MID_ZERO           (0)
#define     NFP_MU_PERF_CFG_LANE_SEL_MID_LOW            (1)
#define     NFP_MU_PERF_CFG_LANE_SEL_MID_MID            (2)
#define     NFP_MU_PERF_CFG_LANE_SEL_MID_HIGH           (3)
#define   NFP_MU_PERF_CFG_LANE_SEL_LO(_x)               ((_x) & 0x3)
#define   NFP_MU_PERF_CFG_LANE_SEL_LO_bf                0, 1, 0
#define   NFP_MU_PERF_CFG_LANE_SEL_LO_mask              (0x3)
#define   NFP_MU_PERF_CFG_LANE_SEL_LO_shift             (0)
#define     NFP_MU_PERF_CFG_LANE_SEL_LO_ZERO            (0)
#define     NFP_MU_PERF_CFG_LANE_SEL_LO_LOW             (1)
#define     NFP_MU_PERF_CFG_LANE_SEL_LO_MID             (2)
#define     NFP_MU_PERF_CFG_LANE_SEL_LO_HIGH            (3)
/* Register Type: MUConfigPerformanceTweaks */
#define NFP_MU_TCACHE_CFG              0x001c
#define   NFP_MU_TCACHE_CFG_CPP_LB_INV                  (0x1 << 23)
#define   NFP_MU_TCACHE_CFG_CPP_LB_INV_bf               0, 23, 23
#define   NFP_MU_TCACHE_CFG_CPP_LB_INV_mask             (0x1)
#define   NFP_MU_TCACHE_CFG_CPP_LB_INV_bit              (23)
#define   NFP_MU_TCACHE_CFG_CPP_LB_MASK(_x)             (((_x) & 0x7f) << 16)
#define   NFP_MU_TCACHE_CFG_CPP_LB_MASK_bf              0, 22, 16
#define   NFP_MU_TCACHE_CFG_CPP_LB_MASK_mask            (0x7f)
#define   NFP_MU_TCACHE_CFG_CPP_LB_MASK_shift           (16)
#define   NFP_MU_TCACHE_CFG_LEVEL_SETS                  (0x1 << 2)
#define   NFP_MU_TCACHE_CFG_LEVEL_SETS_bf               0, 2, 2
#define   NFP_MU_TCACHE_CFG_LEVEL_SETS_mask             (0x1)
#define   NFP_MU_TCACHE_CFG_LEVEL_SETS_bit              (2)
#define     NFP_MU_TCACHE_CFG_LEVEL_SETS_SET            (0x4)
#define   NFP_MU_TCACHE_CFG_CHANNEL_LEVEL               (0x1 << 1)
#define   NFP_MU_TCACHE_CFG_CHANNEL_LEVEL_bf            0, 1, 1
#define   NFP_MU_TCACHE_CFG_CHANNEL_LEVEL_mask          (0x1)
#define   NFP_MU_TCACHE_CFG_CHANNEL_LEVEL_bit           (1)
#define     NFP_MU_TCACHE_CFG_CHANNEL_LEVEL_SET         (0x2)
#define   NFP_MU_TCACHE_CFG_BANK_LEVEL                  (0x1)
#define   NFP_MU_TCACHE_CFG_BANK_LEVEL_bf               0, 0, 0
#define   NFP_MU_TCACHE_CFG_BANK_LEVEL_mask             (0x1)
#define   NFP_MU_TCACHE_CFG_BANK_LEVEL_bit              (0)
#define     NFP_MU_TCACHE_CFG_BANK_LEVEL_SET            (0x1)
/* Register Type: MUConfigRealQDR */
#define NFP_MU_REAL_QDR_CFG            0x0020
#define   NFP_MU_REAL_QDR_CFG_ENABLE1                   (0x1 << 15)
#define   NFP_MU_REAL_QDR_CFG_ENABLE1_bf                0, 15, 15
#define   NFP_MU_REAL_QDR_CFG_ENABLE1_mask              (0x1)
#define   NFP_MU_REAL_QDR_CFG_ENABLE1_bit               (15)
#define     NFP_MU_REAL_QDR_CFG_ENABLE1_SET             (0x8000)
#define   NFP_MU_REAL_QDR_CFG_ID1(_x)                   (((_x) & 0x3) << 8)
#define   NFP_MU_REAL_QDR_CFG_ID1_bf                    0, 9, 8
#define   NFP_MU_REAL_QDR_CFG_ID1_mask                  (0x3)
#define   NFP_MU_REAL_QDR_CFG_ID1_shift                 (8)
#define   NFP_MU_REAL_QDR_CFG_ENABLE0                   (0x1 << 7)
#define   NFP_MU_REAL_QDR_CFG_ENABLE0_bf                0, 7, 7
#define   NFP_MU_REAL_QDR_CFG_ENABLE0_mask              (0x1)
#define   NFP_MU_REAL_QDR_CFG_ENABLE0_bit               (7)
#define     NFP_MU_REAL_QDR_CFG_ENABLE0_SET             (0x80)
#define   NFP_MU_REAL_QDR_CFG_ID0(_x)                   ((_x) & 0x3)
#define   NFP_MU_REAL_QDR_CFG_ID0_bf                    0, 1, 0
#define   NFP_MU_REAL_QDR_CFG_ID0_mask                  (0x3)
#define   NFP_MU_REAL_QDR_CFG_ID0_shift                 (0)
/* Register Type: MUDcacheTestPortDisable */
#define NFP_MU_DCACHE_TEST             0x0024
#define   NFP_MU_DCACHE_TEST_CFG_TP_DISABLE             (0x1)
#define   NFP_MU_DCACHE_TEST_CFG_TP_DISABLE_bf          0, 0, 0
#define   NFP_MU_DCACHE_TEST_CFG_TP_DISABLE_mask        (0x1)
#define   NFP_MU_DCACHE_TEST_CFG_TP_DISABLE_bit         (0)
#define     NFP_MU_DCACHE_TEST_CFG_TP_DISABLE_SET       (0x1)
/* Register Type: MUMemoryBISTTrigger */
#define NFP_MU_BIST_TRIGGER            0x0028
#define   NFP_MU_BIST_TRIGGER_CSR_BISTRUN(_x)           ((_x) & 0xff)
#define   NFP_MU_BIST_TRIGGER_CSR_BISTRUN_bf            0, 7, 0
#define   NFP_MU_BIST_TRIGGER_CSR_BISTRUN_mask          (0xff)
#define   NFP_MU_BIST_TRIGGER_CSR_BISTRUN_shift         (0)
/* Register Type: MUMemoryBISTStatus */
#define NFP_MU_BIST_STATUS             0x002c
#define   NFP_MU_BIST_STATUS_CSR_BISTFAIL_bf            0, 23, 16
#define   NFP_MU_BIST_STATUS_CSR_BISTFAIL_mask          (0xff)
#define   NFP_MU_BIST_STATUS_CSR_BISTFAIL_shift         (16)
#define   NFP_MU_BIST_STATUS_CSR_BISTDONE_bf            0, 7, 0
#define   NFP_MU_BIST_STATUS_CSR_BISTDONE_mask          (0xff)
#define   NFP_MU_BIST_STATUS_CSR_BISTDONE_shift         (0)

/* HGID: nfp3200/mu_pctl.desc = 8f30fff6b984 */
/* Register Type: MUPCtlSCfg */
#define NFP_MU_PCTL_SCFG               0x0000
#define   NFP_MU_PCTL_SCFG_RKINF_EN                     (0x1 << 5)
#define   NFP_MU_PCTL_SCFG_RKINF_EN_bf                  0, 5, 5
#define   NFP_MU_PCTL_SCFG_RKINF_EN_mask                (0x1)
#define   NFP_MU_PCTL_SCFG_RKINF_EN_bit                 (5)
#define   NFP_MU_PCTL_SCFG_DUALPCTLEN                   (0x1 << 4)
#define   NFP_MU_PCTL_SCFG_DUALPCTLEN_bf                0, 4, 4
#define   NFP_MU_PCTL_SCFG_DUALPCTLEN_mask              (0x1)
#define   NFP_MU_PCTL_SCFG_DUALPCTLEN_bit               (4)
#define   NFP_MU_PCTL_SCFG_MODE                         (0x1 << 3)
#define   NFP_MU_PCTL_SCFG_MODE_bf                      0, 3, 3
#define   NFP_MU_PCTL_SCFG_MODE_mask                    (0x1)
#define   NFP_MU_PCTL_SCFG_MODE_bit                     (3)
#define     NFP_MU_PCTL_SCFG_MODE_MASTER                (0x0)
#define     NFP_MU_PCTL_SCFG_MODE_SLAVE                 (0x8)
#define   NFP_MU_PCTL_SCFG_LOOPBACK_EN                  (0x1 << 1)
#define   NFP_MU_PCTL_SCFG_LOOPBACK_EN_bf               0, 1, 1
#define   NFP_MU_PCTL_SCFG_LOOPBACK_EN_mask             (0x1)
#define   NFP_MU_PCTL_SCFG_LOOPBACK_EN_bit              (1)
#define   NFP_MU_PCTL_SCFG_HW_LOW_POWER_EN              (0x1)
#define   NFP_MU_PCTL_SCFG_HW_LOW_POWER_EN_bf           0, 0, 0
#define   NFP_MU_PCTL_SCFG_HW_LOW_POWER_EN_mask         (0x1)
#define   NFP_MU_PCTL_SCFG_HW_LOW_POWER_EN_bit          (0)
/* Register Type: MUPCtlSCtl */
#define NFP_MU_PCTL_SCTL               0x0004
#define   NFP_MU_PCTL_SCTL_STATE(_x)                    ((_x) & 0x7)
#define   NFP_MU_PCTL_SCTL_STATE_bf                     0, 2, 0
#define   NFP_MU_PCTL_SCTL_STATE_mask                   (0x7)
#define   NFP_MU_PCTL_SCTL_STATE_shift                  (0)
#define     NFP_MU_PCTL_SCTL_STATE_INIT                 (0)
#define     NFP_MU_PCTL_SCTL_STATE_CFG                  (1)
#define     NFP_MU_PCTL_SCTL_STATE_GO                   (2)
#define     NFP_MU_PCTL_SCTL_STATE_SLEEP                (3)
#define     NFP_MU_PCTL_SCTL_STATE_WAKEUP               (4)
/* Register Type: MUPCtlStat */
#define NFP_MU_PCTL_STAT               0x0008
#define   NFP_MU_PCTL_STAT_CTL_bf                       0, 2, 0
#define   NFP_MU_PCTL_STAT_CTL_mask                     (0x7)
#define   NFP_MU_PCTL_STAT_CTL_shift                    (0)
#define     NFP_MU_PCTL_STAT_CTL_INIT_MEM               (0)
#define     NFP_MU_PCTL_STAT_CTL_CONFIG                 (1)
#define     NFP_MU_PCTL_STAT_CTL_CONFIG_REQ             (2)
#define     NFP_MU_PCTL_STAT_CTL_ACCESS                 (3)
#define     NFP_MU_PCTL_STAT_CTL_ACCESS_REQ             (4)
#define     NFP_MU_PCTL_STAT_CTL_LOW_POWER              (5)
#define     NFP_MU_PCTL_STAT_CTL_LOW_POWER_ENTRY_REQ    (6)
#define     NFP_MU_PCTL_STAT_CTL_LOW_POWER_EXIT_REQ     (7)
/* Register Type: MUPCtlMCmd */
#define NFP_MU_PCTL_MCMD               0x0040
#define   NFP_MU_PCTL_MCMD_START                        (0x1 << 31)
#define   NFP_MU_PCTL_MCMD_START_bf                     0, 31, 31
#define   NFP_MU_PCTL_MCMD_START_mask                   (0x1)
#define   NFP_MU_PCTL_MCMD_START_bit                    (31)
#define   NFP_MU_PCTL_MCMD_DELAY(_x)                    (((_x) & 0xf) << 24)
#define   NFP_MU_PCTL_MCMD_DELAY_bf                     0, 27, 24
#define   NFP_MU_PCTL_MCMD_DELAY_mask                   (0xf)
#define   NFP_MU_PCTL_MCMD_DELAY_shift                  (24)
#define   NFP_MU_PCTL_MCMD_RANKS(_x)                    (((_x) & 0xf) << 20)
#define   NFP_MU_PCTL_MCMD_RANKS_bf                     0, 23, 20
#define   NFP_MU_PCTL_MCMD_RANKS_mask                   (0xf)
#define   NFP_MU_PCTL_MCMD_RANKS_shift                  (20)
#define     NFP_MU_PCTL_MCMD_RANKS_RANK_0               (1)
#define     NFP_MU_PCTL_MCMD_RANKS_RANK_1               (2)
#define     NFP_MU_PCTL_MCMD_RANKS_RANK_2               (4)
#define     NFP_MU_PCTL_MCMD_RANKS_RANK_3               (8)
#define     NFP_MU_PCTL_MCMD_RANKS_ALL                  (15)
#define   NFP_MU_PCTL_MCMD_DDR3(_x)                     (((_x) & 0x7) << 17)
#define   NFP_MU_PCTL_MCMD_DDR3_bf                      0, 19, 17
#define   NFP_MU_PCTL_MCMD_DDR3_mask                    (0x7)
#define   NFP_MU_PCTL_MCMD_DDR3_shift                   (17)
#define     NFP_MU_PCTL_MCMD_DDR3_MR0                   (0)
#define     NFP_MU_PCTL_MCMD_DDR3_MR1                   (1)
#define     NFP_MU_PCTL_MCMD_DDR3_MR2                   (2)
#define     NFP_MU_PCTL_MCMD_DDR3_MR3                   (3)
#define   NFP_MU_PCTL_MCMD_ADDR(_x)                     (((_x) & 0x1fff) << 4)
#define   NFP_MU_PCTL_MCMD_ADDR_bf                      0, 16, 4
#define   NFP_MU_PCTL_MCMD_ADDR_mask                    (0x1fff)
#define   NFP_MU_PCTL_MCMD_ADDR_shift                   (4)
#define   NFP_MU_PCTL_MCMD_CMD(_x)                      ((_x) & 0x7)
#define   NFP_MU_PCTL_MCMD_CMD_bf                       0, 2, 0
#define   NFP_MU_PCTL_MCMD_CMD_mask                     (0x7)
#define   NFP_MU_PCTL_MCMD_CMD_shift                    (0)
#define     NFP_MU_PCTL_MCMD_CMD_NOP                    (0)
#define     NFP_MU_PCTL_MCMD_CMD_PREA                   (1)
#define     NFP_MU_PCTL_MCMD_CMD_REF                    (2)
#define     NFP_MU_PCTL_MCMD_CMD_MRS                    (3)
#define     NFP_MU_PCTL_MCMD_CMD_ZQCS                   (4)
#define     NFP_MU_PCTL_MCMD_CMD_ZQCL                   (5)
#define     NFP_MU_PCTL_MCMD_CMD_RSTL                   (6)
#define     NFP_MU_PCTL_MCMD_CMD_CKEL                   (7)
/* Register Type: MUPCtlPowCtl */
#define NFP_MU_PCTL_POWCTL             0x0044
#define   NFP_MU_PCTL_POWCTL_START                      (0x1)
#define   NFP_MU_PCTL_POWCTL_START_bf                   0, 0, 0
#define   NFP_MU_PCTL_POWCTL_START_mask                 (0x1)
#define   NFP_MU_PCTL_POWCTL_START_bit                  (0)
/* Register Type: MUPCtlPowStat */
#define NFP_MU_PCTL_POWSTAT            0x0048
#define   NFP_MU_PCTL_POWSTAT_PHY_DONE                  (0x1 << 1)
#define   NFP_MU_PCTL_POWSTAT_PHY_DONE_bf               0, 1, 1
#define   NFP_MU_PCTL_POWSTAT_PHY_DONE_mask             (0x1)
#define   NFP_MU_PCTL_POWSTAT_PHY_DONE_bit              (1)
#define   NFP_MU_PCTL_POWSTAT_POWER_DONE                (0x1)
#define   NFP_MU_PCTL_POWSTAT_POWER_DONE_bf             0, 0, 0
#define   NFP_MU_PCTL_POWSTAT_POWER_DONE_mask           (0x1)
#define   NFP_MU_PCTL_POWSTAT_POWER_DONE_bit            (0)
#define     NFP_MU_PCTL_POWSTAT_POWER_DONE_NO           (0x0)
#define     NFP_MU_PCTL_POWSTAT_POWER_DONE_YES          (0x1)
/* Register Type: MUPCtlMCfg */
#define NFP_MU_PCTL_MCFG               0x0080
#define   NFP_MU_PCTL_MCFG_TFAW(_x)                     (((_x) & 0x3) << 18)
#define   NFP_MU_PCTL_MCFG_TFAW_bf                      0, 19, 18
#define   NFP_MU_PCTL_MCFG_TFAW_mask                    (0x3)
#define   NFP_MU_PCTL_MCFG_TFAW_shift                   (18)
#define     NFP_MU_PCTL_MCFG_TFAW_4                     (0)
#define     NFP_MU_PCTL_MCFG_TFAW_5                     (1)
#define     NFP_MU_PCTL_MCFG_TFAW_6                     (2)
#define   NFP_MU_PCTL_MCFG_PD_MODE                      (0x1 << 17)
#define   NFP_MU_PCTL_MCFG_PD_MODE_bf                   0, 17, 17
#define   NFP_MU_PCTL_MCFG_PD_MODE_mask                 (0x1)
#define   NFP_MU_PCTL_MCFG_PD_MODE_bit                  (17)
#define     NFP_MU_PCTL_MCFG_PD_MODE_SLOW               (0x0)
#define     NFP_MU_PCTL_MCFG_PD_MODE_FAST               (0x20000)
#define   NFP_MU_PCTL_MCFG_PD_TYPE                      (0x1 << 16)
#define   NFP_MU_PCTL_MCFG_PD_TYPE_bf                   0, 16, 16
#define   NFP_MU_PCTL_MCFG_PD_TYPE_mask                 (0x1)
#define   NFP_MU_PCTL_MCFG_PD_TYPE_bit                  (16)
#define     NFP_MU_PCTL_MCFG_PD_TYPE_PRECHARGE          (0x0)
#define     NFP_MU_PCTL_MCFG_PD_TYPE_ACTIVE             (0x10000)
#define   NFP_MU_PCTL_MCFG_PD_IDLE(_x)                  (((_x) & 0xff) << 8)
#define   NFP_MU_PCTL_MCFG_PD_IDLE_bf                   0, 15, 8
#define   NFP_MU_PCTL_MCFG_PD_IDLE_mask                 (0xff)
#define   NFP_MU_PCTL_MCFG_PD_IDLE_shift                (8)
#define   NFP_MU_PCTL_MCFG_PAGE_POLICY(_x)              (((_x) & 0x3) << 6)
#define   NFP_MU_PCTL_MCFG_PAGE_POLICY_bf               0, 7, 6
#define   NFP_MU_PCTL_MCFG_PAGE_POLICY_mask             (0x3)
#define   NFP_MU_PCTL_MCFG_PAGE_POLICY_shift            (6)
#define     NFP_MU_PCTL_MCFG_PAGE_POLICY_OPEN           (0)
#define     NFP_MU_PCTL_MCFG_PAGE_POLICY_CLOSE          (1)
#define   NFP_MU_PCTL_MCFG_DDR3_EN                      (0x1 << 5)
#define   NFP_MU_PCTL_MCFG_DDR3_EN_bf                   0, 5, 5
#define   NFP_MU_PCTL_MCFG_DDR3_EN_mask                 (0x1)
#define   NFP_MU_PCTL_MCFG_DDR3_EN_bit                  (5)
#define   NFP_MU_PCTL_MCFG_TWO_T_EN                     (0x1 << 3)
#define   NFP_MU_PCTL_MCFG_TWO_T_EN_bf                  0, 3, 3
#define   NFP_MU_PCTL_MCFG_TWO_T_EN_mask                (0x1)
#define   NFP_MU_PCTL_MCFG_TWO_T_EN_bit                 (3)
#define   NFP_MU_PCTL_MCFG_MEM_BL                       (0x1)
#define   NFP_MU_PCTL_MCFG_MEM_BL_bf                    0, 0, 0
#define   NFP_MU_PCTL_MCFG_MEM_BL_mask                  (0x1)
#define   NFP_MU_PCTL_MCFG_MEM_BL_bit                   (0)
#define     NFP_MU_PCTL_MCFG_MEM_BL_BL4                 (0x0)
#define     NFP_MU_PCTL_MCFG_MEM_BL_BL8                 (0x1)
/* Register Type: MUPCtlPPCfg */
#define NFP_MU_PCTL_PPCFG              0x0084
#define   NFP_MU_PCTL_PPCFG_RPMEM_DIS(_x)               (((_x) & 0xff) << 1)
#define   NFP_MU_PCTL_PPCFG_RPMEM_DIS_bf                0, 8, 1
#define   NFP_MU_PCTL_PPCFG_RPMEM_DIS_mask              (0xff)
#define   NFP_MU_PCTL_PPCFG_RPMEM_DIS_shift             (1)
#define   NFP_MU_PCTL_PPCFG_PPMEM_EN                    (0x1)
#define   NFP_MU_PCTL_PPCFG_PPMEM_EN_bf                 0, 0, 0
#define   NFP_MU_PCTL_PPCFG_PPMEM_EN_mask               (0x1)
#define   NFP_MU_PCTL_PPCFG_PPMEM_EN_bit                (0)
/* Register Type: MUPCtlMStat */
#define NFP_MU_PCTL_MSTAT              0x0088
#define   NFP_MU_PCTL_MSTAT_POWER_DOWN                  (0x1)
#define   NFP_MU_PCTL_MSTAT_POWER_DOWN_bf               0, 0, 0
#define   NFP_MU_PCTL_MSTAT_POWER_DOWN_mask             (0x1)
#define   NFP_MU_PCTL_MSTAT_POWER_DOWN_bit              (0)
/* Register Type: MUPCtlODTCfg */
#define NFP_MU_PCTL_ODTCFG             0x008c
#define   NFP_MU_PCTL_ODTCFG_RANK3_ODT_DEFAULT          (0x1 << 28)
#define   NFP_MU_PCTL_ODTCFG_RANK3_ODT_DEFAULT_bf       0, 28, 28
#define   NFP_MU_PCTL_ODTCFG_RANK3_ODT_DEFAULT_mask     (0x1)
#define   NFP_MU_PCTL_ODTCFG_RANK3_ODT_DEFAULT_bit      (28)
#define   NFP_MU_PCTL_ODTCFG_RANK3_ODT_WRITE_SEL        (0x1 << 27)
#define   NFP_MU_PCTL_ODTCFG_RANK3_ODT_WRITE_SEL_bf     0, 27, 27
#define   NFP_MU_PCTL_ODTCFG_RANK3_ODT_WRITE_SEL_mask   (0x1)
#define   NFP_MU_PCTL_ODTCFG_RANK3_ODT_WRITE_SEL_bit    (27)
#define   NFP_MU_PCTL_ODTCFG_RANK3_ODT_WRITE_NSEL       (0x1 << 26)
#define   NFP_MU_PCTL_ODTCFG_RANK3_ODT_WRITE_NSEL_bf    0, 26, 26
#define   NFP_MU_PCTL_ODTCFG_RANK3_ODT_WRITE_NSEL_mask  (0x1)
#define   NFP_MU_PCTL_ODTCFG_RANK3_ODT_WRITE_NSEL_bit   (26)
#define   NFP_MU_PCTL_ODTCFG_RANK3_ODT_READ_SEL         (0x1 << 25)
#define   NFP_MU_PCTL_ODTCFG_RANK3_ODT_READ_SEL_bf      0, 25, 25
#define   NFP_MU_PCTL_ODTCFG_RANK3_ODT_READ_SEL_mask    (0x1)
#define   NFP_MU_PCTL_ODTCFG_RANK3_ODT_READ_SEL_bit     (25)
#define   NFP_MU_PCTL_ODTCFG_RANK3_ODT_READ_NSEL        (0x1 << 24)
#define   NFP_MU_PCTL_ODTCFG_RANK3_ODT_READ_NSEL_bf     0, 24, 24
#define   NFP_MU_PCTL_ODTCFG_RANK3_ODT_READ_NSEL_mask   (0x1)
#define   NFP_MU_PCTL_ODTCFG_RANK3_ODT_READ_NSEL_bit    (24)
#define   NFP_MU_PCTL_ODTCFG_RANK2_ODT_DEFAULT          (0x1 << 20)
#define   NFP_MU_PCTL_ODTCFG_RANK2_ODT_DEFAULT_bf       0, 20, 20
#define   NFP_MU_PCTL_ODTCFG_RANK2_ODT_DEFAULT_mask     (0x1)
#define   NFP_MU_PCTL_ODTCFG_RANK2_ODT_DEFAULT_bit      (20)
#define   NFP_MU_PCTL_ODTCFG_RANK2_ODT_WRITE_SEL        (0x1 << 19)
#define   NFP_MU_PCTL_ODTCFG_RANK2_ODT_WRITE_SEL_bf     0, 19, 19
#define   NFP_MU_PCTL_ODTCFG_RANK2_ODT_WRITE_SEL_mask   (0x1)
#define   NFP_MU_PCTL_ODTCFG_RANK2_ODT_WRITE_SEL_bit    (19)
#define   NFP_MU_PCTL_ODTCFG_RANK2_ODT_WRITE_NSEL       (0x1 << 18)
#define   NFP_MU_PCTL_ODTCFG_RANK2_ODT_WRITE_NSEL_bf    0, 18, 18
#define   NFP_MU_PCTL_ODTCFG_RANK2_ODT_WRITE_NSEL_mask  (0x1)
#define   NFP_MU_PCTL_ODTCFG_RANK2_ODT_WRITE_NSEL_bit   (18)
#define   NFP_MU_PCTL_ODTCFG_RANK2_ODT_READ_SEL         (0x1 << 17)
#define   NFP_MU_PCTL_ODTCFG_RANK2_ODT_READ_SEL_bf      0, 17, 17
#define   NFP_MU_PCTL_ODTCFG_RANK2_ODT_READ_SEL_mask    (0x1)
#define   NFP_MU_PCTL_ODTCFG_RANK2_ODT_READ_SEL_bit     (17)
#define   NFP_MU_PCTL_ODTCFG_RANK2_ODT_READ_NSEL        (0x1 << 16)
#define   NFP_MU_PCTL_ODTCFG_RANK2_ODT_READ_NSEL_bf     0, 16, 16
#define   NFP_MU_PCTL_ODTCFG_RANK2_ODT_READ_NSEL_mask   (0x1)
#define   NFP_MU_PCTL_ODTCFG_RANK2_ODT_READ_NSEL_bit    (16)
#define   NFP_MU_PCTL_ODTCFG_RANK1_ODT_DEFAULT          (0x1 << 12)
#define   NFP_MU_PCTL_ODTCFG_RANK1_ODT_DEFAULT_bf       0, 12, 12
#define   NFP_MU_PCTL_ODTCFG_RANK1_ODT_DEFAULT_mask     (0x1)
#define   NFP_MU_PCTL_ODTCFG_RANK1_ODT_DEFAULT_bit      (12)
#define   NFP_MU_PCTL_ODTCFG_RANK1_ODT_WRITE_SEL        (0x1 << 11)
#define   NFP_MU_PCTL_ODTCFG_RANK1_ODT_WRITE_SEL_bf     0, 11, 11
#define   NFP_MU_PCTL_ODTCFG_RANK1_ODT_WRITE_SEL_mask   (0x1)
#define   NFP_MU_PCTL_ODTCFG_RANK1_ODT_WRITE_SEL_bit    (11)
#define   NFP_MU_PCTL_ODTCFG_RANK1_ODT_WRITE_NSEL       (0x1 << 10)
#define   NFP_MU_PCTL_ODTCFG_RANK1_ODT_WRITE_NSEL_bf    0, 10, 10
#define   NFP_MU_PCTL_ODTCFG_RANK1_ODT_WRITE_NSEL_mask  (0x1)
#define   NFP_MU_PCTL_ODTCFG_RANK1_ODT_WRITE_NSEL_bit   (10)
#define   NFP_MU_PCTL_ODTCFG_RANK1_ODT_READ_SEL         (0x1 << 9)
#define   NFP_MU_PCTL_ODTCFG_RANK1_ODT_READ_SEL_bf      0, 9, 9
#define   NFP_MU_PCTL_ODTCFG_RANK1_ODT_READ_SEL_mask    (0x1)
#define   NFP_MU_PCTL_ODTCFG_RANK1_ODT_READ_SEL_bit     (9)
#define   NFP_MU_PCTL_ODTCFG_RANK1_ODT_READ_NSEL        (0x1 << 8)
#define   NFP_MU_PCTL_ODTCFG_RANK1_ODT_READ_NSEL_bf     0, 8, 8
#define   NFP_MU_PCTL_ODTCFG_RANK1_ODT_READ_NSEL_mask   (0x1)
#define   NFP_MU_PCTL_ODTCFG_RANK1_ODT_READ_NSEL_bit    (8)
#define   NFP_MU_PCTL_ODTCFG_RANK0_ODT_DEFAULT          (0x1 << 4)
#define   NFP_MU_PCTL_ODTCFG_RANK0_ODT_DEFAULT_bf       0, 4, 4
#define   NFP_MU_PCTL_ODTCFG_RANK0_ODT_DEFAULT_mask     (0x1)
#define   NFP_MU_PCTL_ODTCFG_RANK0_ODT_DEFAULT_bit      (4)
#define   NFP_MU_PCTL_ODTCFG_RANK0_ODT_WRITE_SEL        (0x1 << 3)
#define   NFP_MU_PCTL_ODTCFG_RANK0_ODT_WRITE_SEL_bf     0, 3, 3
#define   NFP_MU_PCTL_ODTCFG_RANK0_ODT_WRITE_SEL_mask   (0x1)
#define   NFP_MU_PCTL_ODTCFG_RANK0_ODT_WRITE_SEL_bit    (3)
#define   NFP_MU_PCTL_ODTCFG_RANK0_ODT_WRITE_NSEL       (0x1 << 2)
#define   NFP_MU_PCTL_ODTCFG_RANK0_ODT_WRITE_NSEL_bf    0, 2, 2
#define   NFP_MU_PCTL_ODTCFG_RANK0_ODT_WRITE_NSEL_mask  (0x1)
#define   NFP_MU_PCTL_ODTCFG_RANK0_ODT_WRITE_NSEL_bit   (2)
#define   NFP_MU_PCTL_ODTCFG_RANK0_ODT_READ_SEL         (0x1 << 1)
#define   NFP_MU_PCTL_ODTCFG_RANK0_ODT_READ_SEL_bf      0, 1, 1
#define   NFP_MU_PCTL_ODTCFG_RANK0_ODT_READ_SEL_mask    (0x1)
#define   NFP_MU_PCTL_ODTCFG_RANK0_ODT_READ_SEL_bit     (1)
#define   NFP_MU_PCTL_ODTCFG_RANK0_ODT_READ_NSEL        (0x1)
#define   NFP_MU_PCTL_ODTCFG_RANK0_ODT_READ_NSEL_bf     0, 0, 0
#define   NFP_MU_PCTL_ODTCFG_RANK0_ODT_READ_NSEL_mask   (0x1)
#define   NFP_MU_PCTL_ODTCFG_RANK0_ODT_READ_NSEL_bit    (0)
/* Register Type: MUPCtlDQSECfg */
#define NFP_MU_PCTL_DQSECFG            0x0090
#define   NFP_MU_PCTL_DQSECFG_DV_ALAT(_x)               (((_x) & 0xf) << 20)
#define   NFP_MU_PCTL_DQSECFG_DV_ALAT_bf                0, 23, 20
#define   NFP_MU_PCTL_DQSECFG_DV_ALAT_mask              (0xf)
#define   NFP_MU_PCTL_DQSECFG_DV_ALAT_shift             (20)
#define   NFP_MU_PCTL_DQSECFG_DV_ALEN(_x)               (((_x) & 0x3) << 16)
#define   NFP_MU_PCTL_DQSECFG_DV_ALEN_bf                0, 17, 16
#define   NFP_MU_PCTL_DQSECFG_DV_ALEN_mask              (0x3)
#define   NFP_MU_PCTL_DQSECFG_DV_ALEN_shift             (16)
#define   NFP_MU_PCTL_DQSECFG_DSE_ALAT(_x)              (((_x) & 0xf) << 12)
#define   NFP_MU_PCTL_DQSECFG_DSE_ALAT_bf               0, 15, 12
#define   NFP_MU_PCTL_DQSECFG_DSE_ALAT_mask             (0xf)
#define   NFP_MU_PCTL_DQSECFG_DSE_ALAT_shift            (12)
#define   NFP_MU_PCTL_DQSECFG_DSE_ALEN(_x)              (((_x) & 0x3) << 8)
#define   NFP_MU_PCTL_DQSECFG_DSE_ALEN_bf               0, 9, 8
#define   NFP_MU_PCTL_DQSECFG_DSE_ALEN_mask             (0x3)
#define   NFP_MU_PCTL_DQSECFG_DSE_ALEN_shift            (8)
#define   NFP_MU_PCTL_DQSECFG_QSE_ALAT(_x)              (((_x) & 0xf) << 4)
#define   NFP_MU_PCTL_DQSECFG_QSE_ALAT_bf               0, 7, 4
#define   NFP_MU_PCTL_DQSECFG_QSE_ALAT_mask             (0xf)
#define   NFP_MU_PCTL_DQSECFG_QSE_ALAT_shift            (4)
#define   NFP_MU_PCTL_DQSECFG_QSE_ALEN(_x)              ((_x) & 0x3)
#define   NFP_MU_PCTL_DQSECFG_QSE_ALEN_bf               0, 1, 0
#define   NFP_MU_PCTL_DQSECFG_QSE_ALEN_mask             (0x3)
#define   NFP_MU_PCTL_DQSECFG_QSE_ALEN_shift            (0)
/* Register Type: MUPCtlDtuStat */
#define NFP_MU_PCTL_DTUPDES            0x0094
#define   NFP_MU_PCTL_DTUPDES_DTU_RD_MISSING            (0x1 << 13)
#define   NFP_MU_PCTL_DTUPDES_DTU_RD_MISSING_bf         0, 13, 13
#define   NFP_MU_PCTL_DTUPDES_DTU_RD_MISSING_mask       (0x1)
#define   NFP_MU_PCTL_DTUPDES_DTU_RD_MISSING_bit        (13)
#define   NFP_MU_PCTL_DTUPDES_DTU_EAFFL_bf              0, 12, 9
#define   NFP_MU_PCTL_DTUPDES_DTU_EAFFL_mask            (0xf)
#define   NFP_MU_PCTL_DTUPDES_DTU_EAFFL_shift           (9)
#define   NFP_MU_PCTL_DTUPDES_DTU_RANDOM_ERROR          (0x1 << 8)
#define   NFP_MU_PCTL_DTUPDES_DTU_RANDOM_ERROR_bf       0, 8, 8
#define   NFP_MU_PCTL_DTUPDES_DTU_RANDOM_ERROR_mask     (0x1)
#define   NFP_MU_PCTL_DTUPDES_DTU_RANDOM_ERROR_bit      (8)
#define   NFP_MU_PCTL_DTUPDES_DTU_ERR_B7                (0x1 << 7)
#define   NFP_MU_PCTL_DTUPDES_DTU_ERR_B7_bf             0, 7, 7
#define   NFP_MU_PCTL_DTUPDES_DTU_ERR_B7_mask           (0x1)
#define   NFP_MU_PCTL_DTUPDES_DTU_ERR_B7_bit            (7)
#define   NFP_MU_PCTL_DTUPDES_DTU_ERR_B6                (0x1 << 6)
#define   NFP_MU_PCTL_DTUPDES_DTU_ERR_B6_bf             0, 6, 6
#define   NFP_MU_PCTL_DTUPDES_DTU_ERR_B6_mask           (0x1)
#define   NFP_MU_PCTL_DTUPDES_DTU_ERR_B6_bit            (6)
#define   NFP_MU_PCTL_DTUPDES_DTU_ERR_B5                (0x1 << 5)
#define   NFP_MU_PCTL_DTUPDES_DTU_ERR_B5_bf             0, 5, 5
#define   NFP_MU_PCTL_DTUPDES_DTU_ERR_B5_mask           (0x1)
#define   NFP_MU_PCTL_DTUPDES_DTU_ERR_B5_bit            (5)
#define   NFP_MU_PCTL_DTUPDES_DTU_ERR_B4                (0x1 << 4)
#define   NFP_MU_PCTL_DTUPDES_DTU_ERR_B4_bf             0, 4, 4
#define   NFP_MU_PCTL_DTUPDES_DTU_ERR_B4_mask           (0x1)
#define   NFP_MU_PCTL_DTUPDES_DTU_ERR_B4_bit            (4)
#define   NFP_MU_PCTL_DTUPDES_DTU_ERR_B3                (0x1 << 3)
#define   NFP_MU_PCTL_DTUPDES_DTU_ERR_B3_bf             0, 3, 3
#define   NFP_MU_PCTL_DTUPDES_DTU_ERR_B3_mask           (0x1)
#define   NFP_MU_PCTL_DTUPDES_DTU_ERR_B3_bit            (3)
#define   NFP_MU_PCTL_DTUPDES_DTU_ERR_B2                (0x1 << 2)
#define   NFP_MU_PCTL_DTUPDES_DTU_ERR_B2_bf             0, 2, 2
#define   NFP_MU_PCTL_DTUPDES_DTU_ERR_B2_mask           (0x1)
#define   NFP_MU_PCTL_DTUPDES_DTU_ERR_B2_bit            (2)
#define   NFP_MU_PCTL_DTUPDES_DTU_ERR_B1                (0x1 << 1)
#define   NFP_MU_PCTL_DTUPDES_DTU_ERR_B1_bf             0, 1, 1
#define   NFP_MU_PCTL_DTUPDES_DTU_ERR_B1_mask           (0x1)
#define   NFP_MU_PCTL_DTUPDES_DTU_ERR_B1_bit            (1)
#define   NFP_MU_PCTL_DTUPDES_DTU_ERR_B0                (0x1)
#define   NFP_MU_PCTL_DTUPDES_DTU_ERR_B0_bf             0, 0, 0
#define   NFP_MU_PCTL_DTUPDES_DTU_ERR_B0_mask           (0x1)
#define   NFP_MU_PCTL_DTUPDES_DTU_ERR_B0_bit            (0)
/* Register Type: MUPCtlDtuNA */
#define NFP_MU_PCTL_DTUNA              0x0098
#define   NFP_MU_PCTL_DTUNA_DTU_NUM_ADDRESS_bf          0, 31, 0
#define   NFP_MU_PCTL_DTUNA_DTU_NUM_ADDRESS_mask        (0xffffffff)
#define   NFP_MU_PCTL_DTUNA_DTU_NUM_ADDRESS_shift       (0)
/* Register Type: MUPCtlDtuNE */
#define NFP_MU_PCTL_DTUNE              0x009c
#define   NFP_MU_PCTL_DTUNE_DTU_NUM_ERRORS_bf           0, 31, 0
#define   NFP_MU_PCTL_DTUNE_DTU_NUM_ERRORS_mask         (0xffffffff)
#define   NFP_MU_PCTL_DTUNE_DTU_NUM_ERRORS_shift        (0)
/* Register Type: MUPCtlDtuPRd */
#define NFP_MU_PCTL_DTUPRD0            0x00a0
#define NFP_MU_PCTL_DTUPRD1            0x00a4
#define NFP_MU_PCTL_DTUPRD2            0x00a8
#define NFP_MU_PCTL_DTUPRD3            0x00ac
#define   NFP_MU_PCTL_DTUPRD_DTU_ALLBITS_1_bf           0, 31, 16
#define   NFP_MU_PCTL_DTUPRD_DTU_ALLBITS_1_mask         (0xffff)
#define   NFP_MU_PCTL_DTUPRD_DTU_ALLBITS_1_shift        (16)
#define   NFP_MU_PCTL_DTUPRD_DTU_ALLBITS_0_bf           0, 15, 0
#define   NFP_MU_PCTL_DTUPRD_DTU_ALLBITS_0_mask         (0xffff)
#define   NFP_MU_PCTL_DTUPRD_DTU_ALLBITS_0_shift        (0)
/* Register Type: MUPCtlDtuAWdt */
#define NFP_MU_PCTL_DTUAWDT            0x00b0
#define   NFP_MU_PCTL_DTUAWDT_NUMBER_RANKS(_x)          (((_x) & 0x3) << 9)
#define   NFP_MU_PCTL_DTUAWDT_NUMBER_RANKS_bf           0, 10, 9
#define   NFP_MU_PCTL_DTUAWDT_NUMBER_RANKS_mask         (0x3)
#define   NFP_MU_PCTL_DTUAWDT_NUMBER_RANKS_shift        (9)
#define     NFP_MU_PCTL_DTUAWDT_NUMBER_RANKS_1          (0)
#define     NFP_MU_PCTL_DTUAWDT_NUMBER_RANKS_2          (1)
#define     NFP_MU_PCTL_DTUAWDT_NUMBER_RANKS_3          (2)
#define     NFP_MU_PCTL_DTUAWDT_NUMBER_RANKS_4          (3)
#define   NFP_MU_PCTL_DTUAWDT_ROW_ADDR_WIDTH(_x)        (((_x) & 0x3) << 6)
#define   NFP_MU_PCTL_DTUAWDT_ROW_ADDR_WIDTH_bf         0, 7, 6
#define   NFP_MU_PCTL_DTUAWDT_ROW_ADDR_WIDTH_mask       (0x3)
#define   NFP_MU_PCTL_DTUAWDT_ROW_ADDR_WIDTH_shift      (6)
#define     NFP_MU_PCTL_DTUAWDT_ROW_ADDR_WIDTH_13_BIT   (0)
#define     NFP_MU_PCTL_DTUAWDT_ROW_ADDR_WIDTH_14_BIT   (1)
#define     NFP_MU_PCTL_DTUAWDT_ROW_ADDR_WIDTH_15_BIT   (2)
#define     NFP_MU_PCTL_DTUAWDT_ROW_ADDR_WIDTH_16_BIT   (3)
#define   NFP_MU_PCTL_DTUAWDT_BANK_ADDR_WIDTH(_x)       (((_x) & 0x3) << 3)
#define   NFP_MU_PCTL_DTUAWDT_BANK_ADDR_WIDTH_bf        0, 4, 3
#define   NFP_MU_PCTL_DTUAWDT_BANK_ADDR_WIDTH_mask      (0x3)
#define   NFP_MU_PCTL_DTUAWDT_BANK_ADDR_WIDTH_shift     (3)
#define     NFP_MU_PCTL_DTUAWDT_BANK_ADDR_WIDTH_2_BIT   (0)
#define     NFP_MU_PCTL_DTUAWDT_BANK_ADDR_WIDTH_3_BIT   (1)
#define   NFP_MU_PCTL_DTUAWDT_COLUMN_ADDR_WIDTH(_x)     ((_x) & 0x3)
#define   NFP_MU_PCTL_DTUAWDT_COLUMN_ADDR_WIDTH_bf      0, 1, 0
#define   NFP_MU_PCTL_DTUAWDT_COLUMN_ADDR_WIDTH_mask    (0x3)
#define   NFP_MU_PCTL_DTUAWDT_COLUMN_ADDR_WIDTH_shift   (0)
#define     NFP_MU_PCTL_DTUAWDT_COLUMN_ADDR_WIDTH_9_BIT (0)
#define     NFP_MU_PCTL_DTUAWDT_COLUMN_ADDR_WIDTH_10_BIT (1)
/* Register Type: MUPCtlTogCnt1U */
#define NFP_MU_PCTL_TOGCNT1U           0x00c0
#define   NFP_MU_PCTL_TOGCNT1U_CYCLES(_x)               ((_x) & 0x3ff)
#define   NFP_MU_PCTL_TOGCNT1U_CYCLES_bf                0, 9, 0
#define   NFP_MU_PCTL_TOGCNT1U_CYCLES_mask              (0x3ff)
#define   NFP_MU_PCTL_TOGCNT1U_CYCLES_shift             (0)
/* Register Type: MUPCtlTInit */
#define NFP_MU_PCTL_TINIT              0x00c4
#define   NFP_MU_PCTL_TINIT_TIMER(_x)                   ((_x) & 0x1ff)
#define   NFP_MU_PCTL_TINIT_TIMER_bf                    0, 8, 0
#define   NFP_MU_PCTL_TINIT_TIMER_mask                  (0x1ff)
#define   NFP_MU_PCTL_TINIT_TIMER_shift                 (0)
/* Register Type: MUPCtlTRstH */
#define NFP_MU_PCTL_TRSTH              0x00c8
#define   NFP_MU_PCTL_TRSTH_TIMER(_x)                   ((_x) & 0x3ff)
#define   NFP_MU_PCTL_TRSTH_TIMER_bf                    0, 9, 0
#define   NFP_MU_PCTL_TRSTH_TIMER_mask                  (0x3ff)
#define   NFP_MU_PCTL_TRSTH_TIMER_shift                 (0)
/* Register Type: MUPCtlTogCnt100N */
#define NFP_MU_PCTL_TOGCNT100N         0x00cc
#define   NFP_MU_PCTL_TOGCNT100N_CYCLES(_x)             ((_x) & 0x7f)
#define   NFP_MU_PCTL_TOGCNT100N_CYCLES_bf              0, 6, 0
#define   NFP_MU_PCTL_TOGCNT100N_CYCLES_mask            (0x7f)
#define   NFP_MU_PCTL_TOGCNT100N_CYCLES_shift           (0)
/* Register Type: MUPCtlTRefI */
#define NFP_MU_PCTL_TREFI              0x00d0
#define   NFP_MU_PCTL_TREFI_DATA(_x)                    ((_x) & 0xff)
#define   NFP_MU_PCTL_TREFI_DATA_bf                     0, 7, 0
#define   NFP_MU_PCTL_TREFI_DATA_mask                   (0xff)
#define   NFP_MU_PCTL_TREFI_DATA_shift                  (0)
/* Register Type: MUPCtlTMRD */
#define NFP_MU_PCTL_TMRD               0x00d4
#define   NFP_MU_PCTL_TMRD_DATA(_x)                     ((_x) & 0x7)
#define   NFP_MU_PCTL_TMRD_DATA_bf                      0, 2, 0
#define   NFP_MU_PCTL_TMRD_DATA_mask                    (0x7)
#define   NFP_MU_PCTL_TMRD_DATA_shift                   (0)
/* Register Type: MUPCtlTRFC */
#define NFP_MU_PCTL_TRFC               0x00d8
#define   NFP_MU_PCTL_TRFC_DATA(_x)                     ((_x) & 0x1ff)
#define   NFP_MU_PCTL_TRFC_DATA_bf                      0, 8, 0
#define   NFP_MU_PCTL_TRFC_DATA_mask                    (0x1ff)
#define   NFP_MU_PCTL_TRFC_DATA_shift                   (0)
/* Register Type: MUPCtlTRP */
#define NFP_MU_PCTL_TRP                0x00dc
#define   NFP_MU_PCTL_TRP_DATA(_x)                      ((_x) & 0xf)
#define   NFP_MU_PCTL_TRP_DATA_bf                       0, 3, 0
#define   NFP_MU_PCTL_TRP_DATA_mask                     (0xf)
#define   NFP_MU_PCTL_TRP_DATA_shift                    (0)
/* Register Type: MUPCtlTRTW */
#define NFP_MU_PCTL_TRTW               0x00e0
#define   NFP_MU_PCTL_TRTW_DATA(_x)                     ((_x) & 0x7)
#define   NFP_MU_PCTL_TRTW_DATA_bf                      0, 2, 0
#define   NFP_MU_PCTL_TRTW_DATA_mask                    (0x7)
#define   NFP_MU_PCTL_TRTW_DATA_shift                   (0)
/* Register Type: MUPCtlTAL */
#define NFP_MU_PCTL_TAL                0x00e4
#define   NFP_MU_PCTL_TAL_DATA(_x)                      ((_x) & 0xf)
#define   NFP_MU_PCTL_TAL_DATA_bf                       0, 3, 0
#define   NFP_MU_PCTL_TAL_DATA_mask                     (0xf)
#define   NFP_MU_PCTL_TAL_DATA_shift                    (0)
/* Register Type: MUPCtlTCL */
#define NFP_MU_PCTL_TCL                0x00e8
#define   NFP_MU_PCTL_TCL_DATA(_x)                      ((_x) & 0xf)
#define   NFP_MU_PCTL_TCL_DATA_bf                       0, 3, 0
#define   NFP_MU_PCTL_TCL_DATA_mask                     (0xf)
#define   NFP_MU_PCTL_TCL_DATA_shift                    (0)
/* Register Type: MUPCtlTCWL */
#define NFP_MU_PCTL_TCWL               0x00ec
#define   NFP_MU_PCTL_TCWL_DATA(_x)                     ((_x) & 0xf)
#define   NFP_MU_PCTL_TCWL_DATA_bf                      0, 3, 0
#define   NFP_MU_PCTL_TCWL_DATA_mask                    (0xf)
#define   NFP_MU_PCTL_TCWL_DATA_shift                   (0)
/* Register Type: MUPCtlTRAS */
#define NFP_MU_PCTL_TRAS               0x00f0
#define   NFP_MU_PCTL_TRAS_DATA(_x)                     ((_x) & 0x1f)
#define   NFP_MU_PCTL_TRAS_DATA_bf                      0, 4, 0
#define   NFP_MU_PCTL_TRAS_DATA_mask                    (0x1f)
#define   NFP_MU_PCTL_TRAS_DATA_shift                   (0)
/* Register Type: MUPCtlTRC */
#define NFP_MU_PCTL_TRC                0x00f4
#define   NFP_MU_PCTL_TRC_DATA(_x)                      ((_x) & 0x3f)
#define   NFP_MU_PCTL_TRC_DATA_bf                       0, 5, 0
#define   NFP_MU_PCTL_TRC_DATA_mask                     (0x3f)
#define   NFP_MU_PCTL_TRC_DATA_shift                    (0)
/* Register Type: MUPCtlTRCD */
#define NFP_MU_PCTL_TRCD               0x00f8
#define   NFP_MU_PCTL_TRCD_DATA(_x)                     ((_x) & 0xf)
#define   NFP_MU_PCTL_TRCD_DATA_bf                      0, 3, 0
#define   NFP_MU_PCTL_TRCD_DATA_mask                    (0xf)
#define   NFP_MU_PCTL_TRCD_DATA_shift                   (0)
/* Register Type: MUPCtlTRRD */
#define NFP_MU_PCTL_TRRD               0x00fc
#define   NFP_MU_PCTL_TRRD_DATA(_x)                     ((_x) & 0x7)
#define   NFP_MU_PCTL_TRRD_DATA_bf                      0, 2, 0
#define   NFP_MU_PCTL_TRRD_DATA_mask                    (0x7)
#define   NFP_MU_PCTL_TRRD_DATA_shift                   (0)
/* Register Type: MUPCtlTRTP */
#define NFP_MU_PCTL_TRTP               0x0100
#define   NFP_MU_PCTL_TRTP_DATA(_x)                     ((_x) & 0xf)
#define   NFP_MU_PCTL_TRTP_DATA_bf                      0, 3, 0
#define   NFP_MU_PCTL_TRTP_DATA_mask                    (0xf)
#define   NFP_MU_PCTL_TRTP_DATA_shift                   (0)
/* Register Type: MUPCtlTWR */
#define NFP_MU_PCTL_TWR                0x0104
#define   NFP_MU_PCTL_TWR_DATA(_x)                      ((_x) & 0xf)
#define   NFP_MU_PCTL_TWR_DATA_bf                       0, 3, 0
#define   NFP_MU_PCTL_TWR_DATA_mask                     (0xf)
#define   NFP_MU_PCTL_TWR_DATA_shift                    (0)
/* Register Type: MUPCtlTWTR */
#define NFP_MU_PCTL_TWTR               0x0108
#define   NFP_MU_PCTL_TWTR_DATA(_x)                     ((_x) & 0x7)
#define   NFP_MU_PCTL_TWTR_DATA_bf                      0, 2, 0
#define   NFP_MU_PCTL_TWTR_DATA_mask                    (0x7)
#define   NFP_MU_PCTL_TWTR_DATA_shift                   (0)
/* Register Type: MUPCtlTEXSR */
#define NFP_MU_PCTL_TEXSR              0x010c
#define   NFP_MU_PCTL_TEXSR_DATA(_x)                    ((_x) & 0x3ff)
#define   NFP_MU_PCTL_TEXSR_DATA_bf                     0, 9, 0
#define   NFP_MU_PCTL_TEXSR_DATA_mask                   (0x3ff)
#define   NFP_MU_PCTL_TEXSR_DATA_shift                  (0)
/* Register Type: MUPCtlTXP */
#define NFP_MU_PCTL_TXP                0x0110
#define   NFP_MU_PCTL_TXP_DATA(_x)                      ((_x) & 0x7)
#define   NFP_MU_PCTL_TXP_DATA_bf                       0, 2, 0
#define   NFP_MU_PCTL_TXP_DATA_mask                     (0x7)
#define   NFP_MU_PCTL_TXP_DATA_shift                    (0)
/* Register Type: MUPCtlTXPDLL */
#define NFP_MU_PCTL_TXPDLL             0x0114
#define   NFP_MU_PCTL_TXPDLL_DATA(_x)                   ((_x) & 0x3f)
#define   NFP_MU_PCTL_TXPDLL_DATA_bf                    0, 5, 0
#define   NFP_MU_PCTL_TXPDLL_DATA_mask                  (0x3f)
#define   NFP_MU_PCTL_TXPDLL_DATA_shift                 (0)
/* Register Type: MUPCtlTZQCS */
#define NFP_MU_PCTL_TZQCS              0x0118
#define   NFP_MU_PCTL_TZQCS_DATA(_x)                    ((_x) & 0x7f)
#define   NFP_MU_PCTL_TZQCS_DATA_bf                     0, 6, 0
#define   NFP_MU_PCTL_TZQCS_DATA_mask                   (0x7f)
#define   NFP_MU_PCTL_TZQCS_DATA_shift                  (0)
/* Register Type: MUPCtlTZQCSI */
#define NFP_MU_PCTL_TZQCSI             0x011c
#define   NFP_MU_PCTL_TZQCSI_DATA(_x)                   (_x)
#define   NFP_MU_PCTL_TZQCSI_DATA_bf                    0, 31, 0
#define   NFP_MU_PCTL_TZQCSI_DATA_mask                  (0xffffffff)
#define   NFP_MU_PCTL_TZQCSI_DATA_shift                 (0)
/* Register Type: MUPCtlTDQS */
#define NFP_MU_PCTL_TDQS               0x0120
#define   NFP_MU_PCTL_TDQS_DATA(_x)                     ((_x) & 0x7)
#define   NFP_MU_PCTL_TDQS_DATA_bf                      0, 2, 0
#define   NFP_MU_PCTL_TDQS_DATA_mask                    (0x7)
#define   NFP_MU_PCTL_TDQS_DATA_shift                   (0)
/* Register Type: MUPCtlTCKSRE */
#define NFP_MU_PCTL_TCKSRE             0x0124
#define   NFP_MU_PCTL_TCKSRE_DATA(_x)                   ((_x) & 0xf)
#define   NFP_MU_PCTL_TCKSRE_DATA_bf                    0, 3, 0
#define   NFP_MU_PCTL_TCKSRE_DATA_mask                  (0xf)
#define   NFP_MU_PCTL_TCKSRE_DATA_shift                 (0)
/* Register Type: MUPCtlTCKSRX */
#define NFP_MU_PCTL_TCKSRX             0x0128
#define   NFP_MU_PCTL_TCKSRX_DATA(_x)                   ((_x) & 0xf)
#define   NFP_MU_PCTL_TCKSRX_DATA_bf                    0, 3, 0
#define   NFP_MU_PCTL_TCKSRX_DATA_mask                  (0xf)
#define   NFP_MU_PCTL_TCKSRX_DATA_shift                 (0)
/* Register Type: MUPCtlTCKE */
#define NFP_MU_PCTL_TCKE               0x012c
#define   NFP_MU_PCTL_TCKE_DATA(_x)                     ((_x) & 0x7)
#define   NFP_MU_PCTL_TCKE_DATA_bf                      0, 2, 0
#define   NFP_MU_PCTL_TCKE_DATA_mask                    (0x7)
#define   NFP_MU_PCTL_TCKE_DATA_shift                   (0)
/* Register Type: MUPCtlTMOD */
#define NFP_MU_PCTL_TMOD               0x0130
#define   NFP_MU_PCTL_TMOD_DATA(_x)                     ((_x) & 0xf)
#define   NFP_MU_PCTL_TMOD_DATA_bf                      0, 3, 0
#define   NFP_MU_PCTL_TMOD_DATA_mask                    (0xf)
#define   NFP_MU_PCTL_TMOD_DATA_shift                   (0)
/* Register Type: MUPCtlTRSTL */
#define NFP_MU_PCTL_TRSTL              0x0134
#define   NFP_MU_PCTL_TRSTL_DATA(_x)                    ((_x) & 0x7f)
#define   NFP_MU_PCTL_TRSTL_DATA_bf                     0, 6, 0
#define   NFP_MU_PCTL_TRSTL_DATA_mask                   (0x7f)
#define   NFP_MU_PCTL_TRSTL_DATA_shift                  (0)
/* Register Type: MUPCtlTZQCL */
#define NFP_MU_PCTL_TZQCL              0x0138
#define   NFP_MU_PCTL_TZQCL_DATA(_x)                    ((_x) & 0x3ff)
#define   NFP_MU_PCTL_TZQCL_DATA_bf                     0, 9, 0
#define   NFP_MU_PCTL_TZQCL_DATA_mask                   (0x3ff)
#define   NFP_MU_PCTL_TZQCL_DATA_shift                  (0)
/* Register Type: MUPCtlDWLCFG0 */
#define NFP_MU_PCTL_DWLCFG0            0x0170
#define   NFP_MU_PCTL_DWLCFG0_T_ADWL_VEC(_x)            ((_x) & 0x1ff)
#define   NFP_MU_PCTL_DWLCFG0_T_ADWL_VEC_bf             0, 8, 0
#define   NFP_MU_PCTL_DWLCFG0_T_ADWL_VEC_mask           (0x1ff)
#define   NFP_MU_PCTL_DWLCFG0_T_ADWL_VEC_shift          (0)
/* Register Type: MUPCtlDWLCFG1 */
#define NFP_MU_PCTL_DWLCFG1            0x0174
#define   NFP_MU_PCTL_DWLCFG1_T_ADWL_VEC(_x)            ((_x) & 0x1ff)
#define   NFP_MU_PCTL_DWLCFG1_T_ADWL_VEC_bf             0, 8, 0
#define   NFP_MU_PCTL_DWLCFG1_T_ADWL_VEC_mask           (0x1ff)
#define   NFP_MU_PCTL_DWLCFG1_T_ADWL_VEC_shift          (0)
/* Register Type: MUPCtlDWLCFG2 */
#define NFP_MU_PCTL_DWLCFG2            0x0178
#define   NFP_MU_PCTL_DWLCFG2_T_ADWL_VEC(_x)            ((_x) & 0x1ff)
#define   NFP_MU_PCTL_DWLCFG2_T_ADWL_VEC_bf             0, 8, 0
#define   NFP_MU_PCTL_DWLCFG2_T_ADWL_VEC_mask           (0x1ff)
#define   NFP_MU_PCTL_DWLCFG2_T_ADWL_VEC_shift          (0)
/* Register Type: MUPCtlDWLCFG3 */
#define NFP_MU_PCTL_DWLCFG3            0x017c
#define   NFP_MU_PCTL_DWLCFG3_T_ADWL_VEC(_x)            ((_x) & 0x1ff)
#define   NFP_MU_PCTL_DWLCFG3_T_ADWL_VEC_bf             0, 8, 0
#define   NFP_MU_PCTL_DWLCFG3_T_ADWL_VEC_mask           (0x1ff)
#define   NFP_MU_PCTL_DWLCFG3_T_ADWL_VEC_shift          (0)
/* Register Type: MUPCtlDtuWACtl */
#define NFP_MU_PCTL_DTUWACTL           0x0200
#define   NFP_MU_PCTL_DTUWACTL_DTU_WR_RANK(_x)          (((_x) & 0x3) << 30)
#define   NFP_MU_PCTL_DTUWACTL_DTU_WR_RANK_bf           0, 31, 30
#define   NFP_MU_PCTL_DTUWACTL_DTU_WR_RANK_mask         (0x3)
#define   NFP_MU_PCTL_DTUWACTL_DTU_WR_RANK_shift        (30)
#define   NFP_MU_PCTL_DTUWACTL_DTU_WR_ROW(_x)           (((_x) & 0x1ffff) << 13)
#define   NFP_MU_PCTL_DTUWACTL_DTU_WR_ROW_bf            0, 29, 13
#define   NFP_MU_PCTL_DTUWACTL_DTU_WR_ROW_mask          (0x1ffff)
#define   NFP_MU_PCTL_DTUWACTL_DTU_WR_ROW_shift         (13)
#define   NFP_MU_PCTL_DTUWACTL_DTU_WR_BANK(_x)          (((_x) & 0x7) << 10)
#define   NFP_MU_PCTL_DTUWACTL_DTU_WR_BANK_bf           0, 12, 10
#define   NFP_MU_PCTL_DTUWACTL_DTU_WR_BANK_mask         (0x7)
#define   NFP_MU_PCTL_DTUWACTL_DTU_WR_BANK_shift        (10)
#define   NFP_MU_PCTL_DTUWACTL_DTU_WR_COL(_x)           ((_x) & 0x3ff)
#define   NFP_MU_PCTL_DTUWACTL_DTU_WR_COL_bf            0, 9, 0
#define   NFP_MU_PCTL_DTUWACTL_DTU_WR_COL_mask          (0x3ff)
#define   NFP_MU_PCTL_DTUWACTL_DTU_WR_COL_shift         (0)
/* Register Type: MUPCtlDtuRACtl */
#define NFP_MU_PCTL_DTURACTL           0x0204
#define   NFP_MU_PCTL_DTURACTL_DTU_RD_RANK(_x)          (((_x) & 0x3) << 30)
#define   NFP_MU_PCTL_DTURACTL_DTU_RD_RANK_bf           0, 31, 30
#define   NFP_MU_PCTL_DTURACTL_DTU_RD_RANK_mask         (0x3)
#define   NFP_MU_PCTL_DTURACTL_DTU_RD_RANK_shift        (30)
#define   NFP_MU_PCTL_DTURACTL_DTU_RD_ROW(_x)           (((_x) & 0x1ffff) << 13)
#define   NFP_MU_PCTL_DTURACTL_DTU_RD_ROW_bf            0, 29, 13
#define   NFP_MU_PCTL_DTURACTL_DTU_RD_ROW_mask          (0x1ffff)
#define   NFP_MU_PCTL_DTURACTL_DTU_RD_ROW_shift         (13)
#define   NFP_MU_PCTL_DTURACTL_DTU_RD_BANK(_x)          (((_x) & 0x7) << 10)
#define   NFP_MU_PCTL_DTURACTL_DTU_RD_BANK_bf           0, 12, 10
#define   NFP_MU_PCTL_DTURACTL_DTU_RD_BANK_mask         (0x7)
#define   NFP_MU_PCTL_DTURACTL_DTU_RD_BANK_shift        (10)
#define   NFP_MU_PCTL_DTURACTL_DTU_RD_COL(_x)           ((_x) & 0x3ff)
#define   NFP_MU_PCTL_DTURACTL_DTU_RD_COL_bf            0, 9, 0
#define   NFP_MU_PCTL_DTURACTL_DTU_RD_COL_mask          (0x3ff)
#define   NFP_MU_PCTL_DTURACTL_DTU_RD_COL_shift         (0)
/* Register Type: MUPCtlDtuCfg */
#define NFP_MU_PCTL_DTUCFG             0x0208
#define   NFP_MU_PCTL_DTUCFG_DTU_ROW_INCREMENTS(_x)     (((_x) & 0x7f) << 16)
#define   NFP_MU_PCTL_DTUCFG_DTU_ROW_INCREMENTS_bf      0, 22, 16
#define   NFP_MU_PCTL_DTUCFG_DTU_ROW_INCREMENTS_mask    (0x7f)
#define   NFP_MU_PCTL_DTUCFG_DTU_ROW_INCREMENTS_shift   (16)
#define   NFP_MU_PCTL_DTUCFG_DTU_WR_MULTI_RD            (0x1 << 15)
#define   NFP_MU_PCTL_DTUCFG_DTU_WR_MULTI_RD_bf         0, 15, 15
#define   NFP_MU_PCTL_DTUCFG_DTU_WR_MULTI_RD_mask       (0x1)
#define   NFP_MU_PCTL_DTUCFG_DTU_WR_MULTI_RD_bit        (15)
#define   NFP_MU_PCTL_DTUCFG_DTU_DATA_MASK_EN           (0x1 << 14)
#define   NFP_MU_PCTL_DTUCFG_DTU_DATA_MASK_EN_bf        0, 14, 14
#define   NFP_MU_PCTL_DTUCFG_DTU_DATA_MASK_EN_mask      (0x1)
#define   NFP_MU_PCTL_DTUCFG_DTU_DATA_MASK_EN_bit       (14)
#define   NFP_MU_PCTL_DTUCFG_DTU_TARGET_LANE(_x)        (((_x) & 0xf) << 10)
#define   NFP_MU_PCTL_DTUCFG_DTU_TARGET_LANE_bf         0, 13, 10
#define   NFP_MU_PCTL_DTUCFG_DTU_TARGET_LANE_mask       (0xf)
#define   NFP_MU_PCTL_DTUCFG_DTU_TARGET_LANE_shift      (10)
#define   NFP_MU_PCTL_DTUCFG_DTU_GENERATE_RANDOM        (0x1 << 9)
#define   NFP_MU_PCTL_DTUCFG_DTU_GENERATE_RANDOM_bf     0, 9, 9
#define   NFP_MU_PCTL_DTUCFG_DTU_GENERATE_RANDOM_mask   (0x1)
#define   NFP_MU_PCTL_DTUCFG_DTU_GENERATE_RANDOM_bit    (9)
#define   NFP_MU_PCTL_DTUCFG_DTU_INCR_BANKS             (0x1 << 8)
#define   NFP_MU_PCTL_DTUCFG_DTU_INCR_BANKS_bf          0, 8, 8
#define   NFP_MU_PCTL_DTUCFG_DTU_INCR_BANKS_mask        (0x1)
#define   NFP_MU_PCTL_DTUCFG_DTU_INCR_BANKS_bit         (8)
#define   NFP_MU_PCTL_DTUCFG_DTU_INCR_COLS              (0x1 << 7)
#define   NFP_MU_PCTL_DTUCFG_DTU_INCR_COLS_bf           0, 7, 7
#define   NFP_MU_PCTL_DTUCFG_DTU_INCR_COLS_mask         (0x1)
#define   NFP_MU_PCTL_DTUCFG_DTU_INCR_COLS_bit          (7)
#define   NFP_MU_PCTL_DTUCFG_DTU_NALEN(_x)              (((_x) & 0x3f) << 1)
#define   NFP_MU_PCTL_DTUCFG_DTU_NALEN_bf               0, 6, 1
#define   NFP_MU_PCTL_DTUCFG_DTU_NALEN_mask             (0x3f)
#define   NFP_MU_PCTL_DTUCFG_DTU_NALEN_shift            (1)
#define   NFP_MU_PCTL_DTUCFG_DTU_ENABLE                 (0x1)
#define   NFP_MU_PCTL_DTUCFG_DTU_ENABLE_bf              0, 0, 0
#define   NFP_MU_PCTL_DTUCFG_DTU_ENABLE_mask            (0x1)
#define   NFP_MU_PCTL_DTUCFG_DTU_ENABLE_bit             (0)
#define     NFP_MU_PCTL_DTUCFG_DTU_ENABLE_EN            (0x0)
#define     NFP_MU_PCTL_DTUCFG_DTU_ENABLE_DIS           (0x1)
/* Register Type: MUPCtlDtuECtl */
#define NFP_MU_PCTL_DTUECTL            0x020c
#define   NFP_MU_PCTL_DTUECTL_WR_MULTI_RD_RST           (0x1 << 2)
#define   NFP_MU_PCTL_DTUECTL_WR_MULTI_RD_RST_bf        0, 2, 2
#define   NFP_MU_PCTL_DTUECTL_WR_MULTI_RD_RST_mask      (0x1)
#define   NFP_MU_PCTL_DTUECTL_WR_MULTI_RD_RST_bit       (2)
#define   NFP_MU_PCTL_DTUECTL_RUN_ERROR_REPORTS         (0x1 << 1)
#define   NFP_MU_PCTL_DTUECTL_RUN_ERROR_REPORTS_bf      0, 1, 1
#define   NFP_MU_PCTL_DTUECTL_RUN_ERROR_REPORTS_mask    (0x1)
#define   NFP_MU_PCTL_DTUECTL_RUN_ERROR_REPORTS_bit     (1)
#define   NFP_MU_PCTL_DTUECTL_RUN_DTU                   (0x1)
#define   NFP_MU_PCTL_DTUECTL_RUN_DTU_bf                0, 0, 0
#define   NFP_MU_PCTL_DTUECTL_RUN_DTU_mask              (0x1)
#define   NFP_MU_PCTL_DTUECTL_RUN_DTU_bit               (0)
/* Register Type: MUPCtlDtuWD */
#define NFP_MU_PCTL_DTUWD0             0x0210
#define NFP_MU_PCTL_DTUWD1             0x0214
#define NFP_MU_PCTL_DTUWD2             0x0218
#define NFP_MU_PCTL_DTUWD3             0x021c
#define   NFP_MU_PCTL_DTUWD_DTU_WR_BYTE3(_x)            (((_x) & 0xff) << 24)
#define   NFP_MU_PCTL_DTUWD_DTU_WR_BYTE3_bf             0, 31, 24
#define   NFP_MU_PCTL_DTUWD_DTU_WR_BYTE3_mask           (0xff)
#define   NFP_MU_PCTL_DTUWD_DTU_WR_BYTE3_shift          (24)
#define   NFP_MU_PCTL_DTUWD_DTU_WR_BYTE2(_x)            (((_x) & 0xff) << 16)
#define   NFP_MU_PCTL_DTUWD_DTU_WR_BYTE2_bf             0, 23, 16
#define   NFP_MU_PCTL_DTUWD_DTU_WR_BYTE2_mask           (0xff)
#define   NFP_MU_PCTL_DTUWD_DTU_WR_BYTE2_shift          (16)
#define   NFP_MU_PCTL_DTUWD_DTU_WR_BYTE1(_x)            (((_x) & 0xff) << 8)
#define   NFP_MU_PCTL_DTUWD_DTU_WR_BYTE1_bf             0, 15, 8
#define   NFP_MU_PCTL_DTUWD_DTU_WR_BYTE1_mask           (0xff)
#define   NFP_MU_PCTL_DTUWD_DTU_WR_BYTE1_shift          (8)
#define   NFP_MU_PCTL_DTUWD_DTU_WR_BYTE0(_x)            ((_x) & 0xff)
#define   NFP_MU_PCTL_DTUWD_DTU_WR_BYTE0_bf             0, 7, 0
#define   NFP_MU_PCTL_DTUWD_DTU_WR_BYTE0_mask           (0xff)
#define   NFP_MU_PCTL_DTUWD_DTU_WR_BYTE0_shift          (0)
/* Register Type: MUPCtlDtuWDM */
#define NFP_MU_PCTL_DTUWDM             0x0220
#define   NFP_MU_PCTL_DTUWDM_DM_WR_BYTE0(_x)            ((_x) & 0xffff)
#define   NFP_MU_PCTL_DTUWDM_DM_WR_BYTE0_bf             0, 15, 0
#define   NFP_MU_PCTL_DTUWDM_DM_WR_BYTE0_mask           (0xffff)
#define   NFP_MU_PCTL_DTUWDM_DM_WR_BYTE0_shift          (0)
/* Register Type: MUPCtlDtuRD */
#define NFP_MU_PCTL_DTURD0             0x0224
#define NFP_MU_PCTL_DTURD1             0x0228
#define NFP_MU_PCTL_DTURD2             0x022c
#define NFP_MU_PCTL_DTURD3             0x0230
#define   NFP_MU_PCTL_DTURD_DTU_RD_BYTE3_bf             0, 31, 24
#define   NFP_MU_PCTL_DTURD_DTU_RD_BYTE3_mask           (0xff)
#define   NFP_MU_PCTL_DTURD_DTU_RD_BYTE3_shift          (24)
#define   NFP_MU_PCTL_DTURD_DTU_RD_BYTE2_bf             0, 23, 16
#define   NFP_MU_PCTL_DTURD_DTU_RD_BYTE2_mask           (0xff)
#define   NFP_MU_PCTL_DTURD_DTU_RD_BYTE2_shift          (16)
#define   NFP_MU_PCTL_DTURD_DTU_RD_BYTE1_bf             0, 15, 8
#define   NFP_MU_PCTL_DTURD_DTU_RD_BYTE1_mask           (0xff)
#define   NFP_MU_PCTL_DTURD_DTU_RD_BYTE1_shift          (8)
#define   NFP_MU_PCTL_DTURD_DTU_RD_BYTE0_bf             0, 7, 0
#define   NFP_MU_PCTL_DTURD_DTU_RD_BYTE0_mask           (0xff)
#define   NFP_MU_PCTL_DTURD_DTU_RD_BYTE0_shift          (0)
/* Register Type: MUPCtlDtuLfsrWD */
#define NFP_MU_PCTL_DTULFSRWD          0x0234
#define   NFP_MU_PCTL_DTULFSRWD_DTU_LFSR_WSEED(_x)      (_x)
#define   NFP_MU_PCTL_DTULFSRWD_DTU_LFSR_WSEED_bf       0, 31, 0
#define   NFP_MU_PCTL_DTULFSRWD_DTU_LFSR_WSEED_mask     (0xffffffff)
#define   NFP_MU_PCTL_DTULFSRWD_DTU_LFSR_WSEED_shift    (0)
/* Register Type: MUPCtlDtuLfsrRD */
#define NFP_MU_PCTL_DTULFSRRD          0x0238
#define   NFP_MU_PCTL_DTULFSRRD_DTU_LFSR_RSEED(_x)      (_x)
#define   NFP_MU_PCTL_DTULFSRRD_DTU_LFSR_RSEED_bf       0, 31, 0
#define   NFP_MU_PCTL_DTULFSRRD_DTU_LFSR_RSEED_mask     (0xffffffff)
#define   NFP_MU_PCTL_DTULFSRRD_DTU_LFSR_RSEED_shift    (0)
/* Register Type: MUPCtlDtuEAF */
#define NFP_MU_PCTL_DTUEAF             0x023c
#define   NFP_MU_PCTL_DTUEAF_EA_RANK_bf                 0, 31, 30
#define   NFP_MU_PCTL_DTUEAF_EA_RANK_mask               (0x3)
#define   NFP_MU_PCTL_DTUEAF_EA_RANK_shift              (30)
#define   NFP_MU_PCTL_DTUEAF_EA_ROW_bf                  0, 29, 13
#define   NFP_MU_PCTL_DTUEAF_EA_ROW_mask                (0x1ffff)
#define   NFP_MU_PCTL_DTUEAF_EA_ROW_shift               (13)
#define   NFP_MU_PCTL_DTUEAF_EA_BANK_bf                 0, 12, 10
#define   NFP_MU_PCTL_DTUEAF_EA_BANK_mask               (0x7)
#define   NFP_MU_PCTL_DTUEAF_EA_BANK_shift              (10)
#define   NFP_MU_PCTL_DTUEAF_EA_COLUMN_bf               0, 9, 0
#define   NFP_MU_PCTL_DTUEAF_EA_COLUMN_mask             (0x3ff)
#define   NFP_MU_PCTL_DTUEAF_EA_COLUMN_shift            (0)
/* Register Type: MUPCtlPHYPVTCFG */
#define NFP_MU_PCTL_PHYPVTCFG          0x0300
#define   NFP_MU_PCTL_PHYPVTCFG_PV_UPD_REQ_EN           (0x1 << 15)
#define   NFP_MU_PCTL_PHYPVTCFG_PV_UPD_REQ_EN_bf        0, 15, 15
#define   NFP_MU_PCTL_PHYPVTCFG_PV_UPD_REQ_EN_mask      (0x1)
#define   NFP_MU_PCTL_PHYPVTCFG_PV_UPD_REQ_EN_bit       (15)
#define   NFP_MU_PCTL_PHYPVTCFG_PVT_UPD_TRIG_POL        (0x1 << 14)
#define   NFP_MU_PCTL_PHYPVTCFG_PVT_UPD_TRIG_POL_bf     0, 14, 14
#define   NFP_MU_PCTL_PHYPVTCFG_PVT_UPD_TRIG_POL_mask   (0x1)
#define   NFP_MU_PCTL_PHYPVTCFG_PVT_UPD_TRIG_POL_bit    (14)
#define     NFP_MU_PCTL_PHYPVTCFG_PVT_UPD_TRIG_POL_LOW  (0x0)
#define     NFP_MU_PCTL_PHYPVTCFG_PVT_UPD_TRIG_POL_HIGH (0x4000)
#define   NFP_MU_PCTL_PHYPVTCFG_PVT_UPD_TRIG_TYPE       (0x1 << 12)
#define   NFP_MU_PCTL_PHYPVTCFG_PVT_UPD_TRIG_TYPE_bf    0, 12, 12
#define   NFP_MU_PCTL_PHYPVTCFG_PVT_UPD_TRIG_TYPE_mask  (0x1)
#define   NFP_MU_PCTL_PHYPVTCFG_PVT_UPD_TRIG_TYPE_bit   (12)
#define     NFP_MU_PCTL_PHYPVTCFG_PVT_UPD_TRIG_TYPE_ALWAYS (0x0)
#define     NFP_MU_PCTL_PHYPVTCFG_PVT_UPD_TRIG_TYPE_PBT_UPDATE (0x1000)
#define   NFP_MU_PCTL_PHYPVTCFG_PVT_UPD_DONE_POL        (0x1 << 10)
#define   NFP_MU_PCTL_PHYPVTCFG_PVT_UPD_DONE_POL_bf     0, 10, 10
#define   NFP_MU_PCTL_PHYPVTCFG_PVT_UPD_DONE_POL_mask   (0x1)
#define   NFP_MU_PCTL_PHYPVTCFG_PVT_UPD_DONE_POL_bit    (10)
#define     NFP_MU_PCTL_PHYPVTCFG_PVT_UPD_DONE_POL_LOW  (0x0)
#define     NFP_MU_PCTL_PHYPVTCFG_PVT_UPD_DONE_POL_HIGH (0x400)
#define   NFP_MU_PCTL_PHYPVTCFG_PVT_UPD_DONE_TYPE(_x)   (((_x) & 0x3) << 8)
#define   NFP_MU_PCTL_PHYPVTCFG_PVT_UPD_DONE_TYPE_bf    0, 9, 8
#define   NFP_MU_PCTL_PHYPVTCFG_PVT_UPD_DONE_TYPE_mask  (0x3)
#define   NFP_MU_PCTL_PHYPVTCFG_PVT_UPD_DONE_TYPE_shift (8)
#define     NFP_MU_PCTL_PHYPVTCFG_PVT_UPD_DONE_TYPE_FIXME1 (0)
#define     NFP_MU_PCTL_PHYPVTCFG_PVT_UPD_DONE_TYPE_FIXME2 (1)
#define     NFP_MU_PCTL_PHYPVTCFG_PVT_UPD_DONE_TYPE_FIXME3 (2)
#define   NFP_MU_PCTL_PHYPVTCFG_PHY_UPD_REQ_EN          (0x1 << 7)
#define   NFP_MU_PCTL_PHYPVTCFG_PHY_UPD_REQ_EN_bf       0, 7, 7
#define   NFP_MU_PCTL_PHYPVTCFG_PHY_UPD_REQ_EN_mask     (0x1)
#define   NFP_MU_PCTL_PHYPVTCFG_PHY_UPD_REQ_EN_bit      (7)
#define     NFP_MU_PCTL_PHYPVTCFG_PHY_UPD_REQ_EN_DIS    (0x0)
#define     NFP_MU_PCTL_PHYPVTCFG_PHY_UPD_REQ_EN_EN     (0x80)
#define   NFP_MU_PCTL_PHYPVTCFG_PHY_UPD_TRIG_POL        (0x1 << 6)
#define   NFP_MU_PCTL_PHYPVTCFG_PHY_UPD_TRIG_POL_bf     0, 6, 6
#define   NFP_MU_PCTL_PHYPVTCFG_PHY_UPD_TRIG_POL_mask   (0x1)
#define   NFP_MU_PCTL_PHYPVTCFG_PHY_UPD_TRIG_POL_bit    (6)
#define     NFP_MU_PCTL_PHYPVTCFG_PHY_UPD_TRIG_POL_LOW  (0x0)
#define     NFP_MU_PCTL_PHYPVTCFG_PHY_UPD_TRIG_POL_HIGH (0x40)
#define   NFP_MU_PCTL_PHYPVTCFG_PHY_UPD_TRIG_TYPE(_x)   (((_x) & 0x3) << 4)
#define   NFP_MU_PCTL_PHYPVTCFG_PHY_UPD_TRIG_TYPE_bf    0, 5, 4
#define   NFP_MU_PCTL_PHYPVTCFG_PHY_UPD_TRIG_TYPE_mask  (0x3)
#define   NFP_MU_PCTL_PHYPVTCFG_PHY_UPD_TRIG_TYPE_shift (4)
#define     NFP_MU_PCTL_PHYPVTCFG_PHY_UPD_TRIG_TYPE_ALWAYS (0)
#define     NFP_MU_PCTL_PHYPVTCFG_PHY_UPD_TRIG_TYPE_PHY_UPDATE (1)
#define     NFP_MU_PCTL_PHYPVTCFG_PHY_UPD_TRIG_TYPE_PHY_UPDATE_IMM (3)
#define   NFP_MU_PCTL_PHYPVTCFG_PHY_UPD_DONE_POL        (0x1 << 2)
#define   NFP_MU_PCTL_PHYPVTCFG_PHY_UPD_DONE_POL_bf     0, 2, 2
#define   NFP_MU_PCTL_PHYPVTCFG_PHY_UPD_DONE_POL_mask   (0x1)
#define   NFP_MU_PCTL_PHYPVTCFG_PHY_UPD_DONE_POL_bit    (2)
#define     NFP_MU_PCTL_PHYPVTCFG_PHY_UPD_DONE_POL_LOW  (0x0)
#define     NFP_MU_PCTL_PHYPVTCFG_PHY_UPD_DONE_POL_HIGH (0x4)
#define   NFP_MU_PCTL_PHYPVTCFG_PHY_UPD_DONE_TYPE(_x)   ((_x) & 0x3)
#define   NFP_MU_PCTL_PHYPVTCFG_PHY_UPD_DONE_TYPE_bf    0, 1, 0
#define   NFP_MU_PCTL_PHYPVTCFG_PHY_UPD_DONE_TYPE_mask  (0x3)
#define   NFP_MU_PCTL_PHYPVTCFG_PHY_UPD_DONE_TYPE_shift (0)
#define     NFP_MU_PCTL_PHYPVTCFG_PHY_UPD_DONE_TYPE_FIXME1 (0)
#define     NFP_MU_PCTL_PHYPVTCFG_PHY_UPD_DONE_TYPE_FIXME2 (1)
#define     NFP_MU_PCTL_PHYPVTCFG_PHY_UPD_DONE_TYPE_FIXME3 (2)
/* Register Type: MUPCtlPHYPVTSTAT */
#define NFP_MU_PCTL_PHYPVTSTAT         0x0304
#define   NFP_MU_PCTL_PHYPVTSTAT_I_PVT_UPD_TRIG         (0x1 << 5)
#define   NFP_MU_PCTL_PHYPVTSTAT_I_PVT_UPD_TRIG_bf      0, 5, 5
#define   NFP_MU_PCTL_PHYPVTSTAT_I_PVT_UPD_TRIG_mask    (0x1)
#define   NFP_MU_PCTL_PHYPVTSTAT_I_PVT_UPD_TRIG_bit     (5)
#define   NFP_MU_PCTL_PHYPVTSTAT_I_PVT_UPD_DONE         (0x1 << 4)
#define   NFP_MU_PCTL_PHYPVTSTAT_I_PVT_UPD_DONE_bf      0, 4, 4
#define   NFP_MU_PCTL_PHYPVTSTAT_I_PVT_UPD_DONE_mask    (0x1)
#define   NFP_MU_PCTL_PHYPVTSTAT_I_PVT_UPD_DONE_bit     (4)
#define   NFP_MU_PCTL_PHYPVTSTAT_I_PHY_UPD_TRIG         (0x1 << 1)
#define   NFP_MU_PCTL_PHYPVTSTAT_I_PHY_UPD_TRIG_bf      0, 1, 1
#define   NFP_MU_PCTL_PHYPVTSTAT_I_PHY_UPD_TRIG_mask    (0x1)
#define   NFP_MU_PCTL_PHYPVTSTAT_I_PHY_UPD_TRIG_bit     (1)
#define   NFP_MU_PCTL_PHYPVTSTAT_I_PHY_UPD_DONE         (0x1)
#define   NFP_MU_PCTL_PHYPVTSTAT_I_PHY_UPD_DONE_bf      0, 0, 0
#define   NFP_MU_PCTL_PHYPVTSTAT_I_PHY_UPD_DONE_mask    (0x1)
#define   NFP_MU_PCTL_PHYPVTSTAT_I_PHY_UPD_DONE_bit     (0)
/* Register Type: MUPCtlPHYTUPDON */
#define NFP_MU_PCTL_PHYTUPDON          0x0308
#define   NFP_MU_PCTL_PHYTUPDON_PHY_T_UPDON(_x)         ((_x) & 0xff)
#define   NFP_MU_PCTL_PHYTUPDON_PHY_T_UPDON_bf          0, 7, 0
#define   NFP_MU_PCTL_PHYTUPDON_PHY_T_UPDON_mask        (0xff)
#define   NFP_MU_PCTL_PHYTUPDON_PHY_T_UPDON_shift       (0)
/* Register Type: MUPCtlPHYTUPDDLY */
#define NFP_MU_PCTL_PHYTUPDDLY         0x030c
#define   NFP_MU_PCTL_PHYTUPDDLY_PHY_T_UPDDLY(_x)       ((_x) & 0xf)
#define   NFP_MU_PCTL_PHYTUPDDLY_PHY_T_UPDDLY_bf        0, 3, 0
#define   NFP_MU_PCTL_PHYTUPDDLY_PHY_T_UPDDLY_mask      (0xf)
#define   NFP_MU_PCTL_PHYTUPDDLY_PHY_T_UPDDLY_shift     (0)
/* Register Type: MUPCtlPVTTUPDON */
#define NFP_MU_PCTL_PVTTUPDON          0x0310
#define   NFP_MU_PCTL_PVTTUPDON_PVT_T_UPDON(_x)         ((_x) & 0xff)
#define   NFP_MU_PCTL_PVTTUPDON_PVT_T_UPDON_bf          0, 7, 0
#define   NFP_MU_PCTL_PVTTUPDON_PVT_T_UPDON_mask        (0xff)
#define   NFP_MU_PCTL_PVTTUPDON_PVT_T_UPDON_shift       (0)
/* Register Type: MUPCtlPVTTUPDDLY */
#define NFP_MU_PCTL_PVTTUPDDLY         0x0314
#define   NFP_MU_PCTL_PVTTUPDDLY_PVT_T_UPDDLY(_x)       ((_x) & 0xf)
#define   NFP_MU_PCTL_PVTTUPDDLY_PVT_T_UPDDLY_bf        0, 3, 0
#define   NFP_MU_PCTL_PVTTUPDDLY_PVT_T_UPDDLY_mask      (0xf)
#define   NFP_MU_PCTL_PVTTUPDDLY_PVT_T_UPDDLY_shift     (0)
/* Register Type: MUPCtlPHYPVTUPDI */
#define NFP_MU_PCTL_PHYPVTUPDI         0x0318
#define   NFP_MU_PCTL_PHYPVTUPDI_PHYPVT_T_UPDI(_x)      ((_x) & 0xff)
#define   NFP_MU_PCTL_PHYPVTUPDI_PHYPVT_T_UPDI_bf       0, 7, 0
#define   NFP_MU_PCTL_PHYPVTUPDI_PHYPVT_T_UPDI_mask     (0xff)
#define   NFP_MU_PCTL_PHYPVTUPDI_PHYPVT_T_UPDI_shift    (0)
/* Register Type: MUPCtlPHYIOCRV1 */
#define NFP_MU_PCTL_PHYIOCRV1          0x031c
#define   NFP_MU_PCTL_PHYIOCRV1_AUTO_CMD_PDD(_x)        (((_x) & 0x3) << 28)
#define   NFP_MU_PCTL_PHYIOCRV1_AUTO_CMD_PDD_bf         0, 29, 28
#define   NFP_MU_PCTL_PHYIOCRV1_AUTO_CMD_PDD_mask       (0x3)
#define   NFP_MU_PCTL_PHYIOCRV1_AUTO_CMD_PDD_shift      (28)
#define     NFP_MU_PCTL_PHYIOCRV1_AUTO_CMD_PDD_FIXME1   (0)
#define     NFP_MU_PCTL_PHYIOCRV1_AUTO_CMD_PDD_FIXME2   (1)
#define     NFP_MU_PCTL_PHYIOCRV1_AUTO_CMD_PDD_FIXME3   (2)
#define     NFP_MU_PCTL_PHYIOCRV1_AUTO_CMD_PDD_FIXME4   (3)
#define   NFP_MU_PCTL_PHYIOCRV1_AUTO_DATA_PDD(_x)       (((_x) & 0x3) << 26)
#define   NFP_MU_PCTL_PHYIOCRV1_AUTO_DATA_PDD_bf        0, 27, 26
#define   NFP_MU_PCTL_PHYIOCRV1_AUTO_DATA_PDD_mask      (0x3)
#define   NFP_MU_PCTL_PHYIOCRV1_AUTO_DATA_PDD_shift     (26)
#define     NFP_MU_PCTL_PHYIOCRV1_AUTO_DATA_PDD_FIXME1  (0)
#define     NFP_MU_PCTL_PHYIOCRV1_AUTO_DATA_PDD_FIXME2  (1)
#define     NFP_MU_PCTL_PHYIOCRV1_AUTO_DATA_PDD_FIXME3  (2)
#define     NFP_MU_PCTL_PHYIOCRV1_AUTO_DATA_PDD_FIXME4  (3)
#define   NFP_MU_PCTL_PHYIOCRV1_BYTE_OE_CTL(_x)         (((_x) & 0x3) << 16)
#define   NFP_MU_PCTL_PHYIOCRV1_BYTE_OE_CTL_bf          0, 17, 16
#define   NFP_MU_PCTL_PHYIOCRV1_BYTE_OE_CTL_mask        (0x3)
#define   NFP_MU_PCTL_PHYIOCRV1_BYTE_OE_CTL_shift       (16)
#define     NFP_MU_PCTL_PHYIOCRV1_BYTE_OE_CTL_FIXME1    (0)
#define     NFP_MU_PCTL_PHYIOCRV1_BYTE_OE_CTL_FIXME2    (1)
#define     NFP_MU_PCTL_PHYIOCRV1_BYTE_OE_CTL_FIXME3    (2)
#define     NFP_MU_PCTL_PHYIOCRV1_BYTE_OE_CTL_FIXME4    (3)
#define   NFP_MU_PCTL_PHYIOCRV1_DYN_SOC_ODT_ALAT(_x)    (((_x) & 0xf) << 12)
#define   NFP_MU_PCTL_PHYIOCRV1_DYN_SOC_ODT_ALAT_bf     0, 15, 12
#define   NFP_MU_PCTL_PHYIOCRV1_DYN_SOC_ODT_ALAT_mask   (0xf)
#define   NFP_MU_PCTL_PHYIOCRV1_DYN_SOC_ODT_ALAT_shift  (12)
#define   NFP_MU_PCTL_PHYIOCRV1_DYN_SOC_ODT_ALEN(_x)    (((_x) & 0x3) << 8)
#define   NFP_MU_PCTL_PHYIOCRV1_DYN_SOC_ODT_ALEN_bf     0, 9, 8
#define   NFP_MU_PCTL_PHYIOCRV1_DYN_SOC_ODT_ALEN_mask   (0x3)
#define   NFP_MU_PCTL_PHYIOCRV1_DYN_SOC_ODT_ALEN_shift  (8)
#define   NFP_MU_PCTL_PHYIOCRV1_DYN_SOC_ODT             (0x1 << 2)
#define   NFP_MU_PCTL_PHYIOCRV1_DYN_SOC_ODT_bf          0, 2, 2
#define   NFP_MU_PCTL_PHYIOCRV1_DYN_SOC_ODT_mask        (0x1)
#define   NFP_MU_PCTL_PHYIOCRV1_DYN_SOC_ODT_bit         (2)
#define   NFP_MU_PCTL_PHYIOCRV1_SOC_ODT_EN              (0x1)
#define   NFP_MU_PCTL_PHYIOCRV1_SOC_ODT_EN_bf           0, 0, 0
#define   NFP_MU_PCTL_PHYIOCRV1_SOC_ODT_EN_mask         (0x1)
#define   NFP_MU_PCTL_PHYIOCRV1_SOC_ODT_EN_bit          (0)
/* Register Type: MUPCtlPHYTUpdWait */
#define NFP_MU_PCTL_PHYTUPDWAIT        0x0320
#define   NFP_MU_PCTL_PHYTUPDWAIT_PHY_T_UPDWAIT(_x)     ((_x) & 0x3f)
#define   NFP_MU_PCTL_PHYTUPDWAIT_PHY_T_UPDWAIT_bf      0, 5, 0
#define   NFP_MU_PCTL_PHYTUPDWAIT_PHY_T_UPDWAIT_mask    (0x3f)
#define   NFP_MU_PCTL_PHYTUPDWAIT_PHY_T_UPDWAIT_shift   (0)
/* Register Type: MUPCtlPVTTUpdWait */
#define NFP_MU_PCTL_PVTTUPDWAIT        0x0324
#define   NFP_MU_PCTL_PVTTUPDWAIT_PVT_T_UPDWAIT(_x)     ((_x) & 0x3f)
#define   NFP_MU_PCTL_PVTTUPDWAIT_PVT_T_UPDWAIT_bf      0, 5, 0
#define   NFP_MU_PCTL_PVTTUPDWAIT_PVT_T_UPDWAIT_mask    (0x3f)
#define   NFP_MU_PCTL_PVTTUPDWAIT_PVT_T_UPDWAIT_shift   (0)
/* Register Type: MUPCtlPVTUpdI */
#define NFP_MU_PCTL_MUPCTLPVTUPDI      0x0328
#define   NFP_MU_PCTL_MUPCTLPVTUPDI_PVT_T_UPDI(_x)      (_x)
#define   NFP_MU_PCTL_MUPCTLPVTUPDI_PVT_T_UPDI_bf       0, 31, 0
#define   NFP_MU_PCTL_MUPCTLPVTUPDI_PVT_T_UPDI_mask     (0xffffffff)
#define   NFP_MU_PCTL_MUPCTLPVTUPDI_PVT_T_UPDI_shift    (0)
/* Register Type: MUPCtlIPVR */
#define NFP_MU_PCTL_IPVR               0x03f8
#define   NFP_MU_PCTL_IPVR_IP_VERSION_bf                0, 31, 0
#define   NFP_MU_PCTL_IPVR_IP_VERSION_mask              (0xffffffff)
#define   NFP_MU_PCTL_IPVR_IP_VERSION_shift             (0)
/* Register Type: MUPCtlIPTR */
#define NFP_MU_PCTL_IPTR               0x03fc
#define   NFP_MU_PCTL_IPTR_IP_TYPE_bf                   0, 31, 0
#define   NFP_MU_PCTL_IPTR_IP_TYPE_mask                 (0xffffffff)
#define   NFP_MU_PCTL_IPTR_IP_TYPE_shift                (0)

/* HGID: nfp3200/mu_phy.desc = 5702fafbfb92 */
/* Register Type: MUPhyRIDR */
#define NFP_MU_PCTL_PHY_RIDR           0x0400
#define   NFP_MU_PCTL_PHY_RIDR_MJREV_bf                 0, 15, 8
#define   NFP_MU_PCTL_PHY_RIDR_MJREV_mask               (0xff)
#define   NFP_MU_PCTL_PHY_RIDR_MJREV_shift              (8)
#define   NFP_MU_PCTL_PHY_RIDR_MNREV_bf                 0, 7, 0
#define   NFP_MU_PCTL_PHY_RIDR_MNREV_mask               (0xff)
#define   NFP_MU_PCTL_PHY_RIDR_MNREV_shift              (0)
/* Register Type: MUPhyPIR */
#define NFP_MU_PCTL_PHY_PIR            0x0404
#define   NFP_MU_PCTL_PHY_PIR_INHVT                     (0x1 << 28)
#define   NFP_MU_PCTL_PHY_PIR_INHVT_bf                  0, 28, 28
#define   NFP_MU_PCTL_PHY_PIR_INHVT_mask                (0x1)
#define   NFP_MU_PCTL_PHY_PIR_INHVT_bit                 (28)
#define   NFP_MU_PCTL_PHY_PIR_ZCALUPD                   (0x1 << 27)
#define   NFP_MU_PCTL_PHY_PIR_ZCALUPD_bf                0, 27, 27
#define   NFP_MU_PCTL_PHY_PIR_ZCALUPD_mask              (0x1)
#define   NFP_MU_PCTL_PHY_PIR_ZCALUPD_bit               (27)
#define   NFP_MU_PCTL_PHY_PIR_ZCAL                      (0x1 << 26)
#define   NFP_MU_PCTL_PHY_PIR_ZCAL_bf                   0, 26, 26
#define   NFP_MU_PCTL_PHY_PIR_ZCAL_mask                 (0x1)
#define   NFP_MU_PCTL_PHY_PIR_ZCAL_bit                  (26)
#define   NFP_MU_PCTL_PHY_PIR_ZCALBYP                   (0x1 << 25)
#define   NFP_MU_PCTL_PHY_PIR_ZCALBYP_bf                0, 25, 25
#define   NFP_MU_PCTL_PHY_PIR_ZCALBYP_mask              (0x1)
#define   NFP_MU_PCTL_PHY_PIR_ZCALBYP_bit               (25)
#define   NFP_MU_PCTL_PHY_PIR_ZCKSEL(_x)                (((_x) & 0x3) << 23)
#define   NFP_MU_PCTL_PHY_PIR_ZCKSEL_bf                 0, 24, 23
#define   NFP_MU_PCTL_PHY_PIR_ZCKSEL_mask               (0x3)
#define   NFP_MU_PCTL_PHY_PIR_ZCKSEL_shift              (23)
#define     NFP_MU_PCTL_PHY_PIR_ZCKSEL_DIV_1            (0)
#define     NFP_MU_PCTL_PHY_PIR_ZCKSEL_DIV_8            (1)
#define     NFP_MU_PCTL_PHY_PIR_ZCKSEL_DIV_32           (2)
#define     NFP_MU_PCTL_PHY_PIR_ZCKSEL_DIV_64           (3)
#define   NFP_MU_PCTL_PHY_PIR_DLDLMT(_x)                (((_x) & 0xff) << 15)
#define   NFP_MU_PCTL_PHY_PIR_DLDLMT_bf                 0, 22, 15
#define   NFP_MU_PCTL_PHY_PIR_DLDLMT_mask               (0xff)
#define   NFP_MU_PCTL_PHY_PIR_DLDLMT_shift              (15)
#define   NFP_MU_PCTL_PHY_PIR_FDEPTH(_x)                (((_x) & 0x3) << 13)
#define   NFP_MU_PCTL_PHY_PIR_FDEPTH_bf                 0, 14, 13
#define   NFP_MU_PCTL_PHY_PIR_FDEPTH_mask               (0x3)
#define   NFP_MU_PCTL_PHY_PIR_FDEPTH_shift              (13)
#define     NFP_MU_PCTL_PHY_PIR_FDEPTH_2                (0)
#define     NFP_MU_PCTL_PHY_PIR_FDEPTH_4                (1)
#define     NFP_MU_PCTL_PHY_PIR_FDEPTH_8                (2)
#define     NFP_MU_PCTL_PHY_PIR_FDEPTH_16               (3)
#define   NFP_MU_PCTL_PHY_PIR_LPFDEPTH(_x)              (((_x) & 0x3) << 11)
#define   NFP_MU_PCTL_PHY_PIR_LPFDEPTH_bf               0, 12, 11
#define   NFP_MU_PCTL_PHY_PIR_LPFDEPTH_mask             (0x3)
#define   NFP_MU_PCTL_PHY_PIR_LPFDEPTH_shift            (11)
#define     NFP_MU_PCTL_PHY_PIR_LPFDEPTH_2              (0)
#define     NFP_MU_PCTL_PHY_PIR_LPFDEPTH_4              (1)
#define     NFP_MU_PCTL_PHY_PIR_LPFDEPTH_8              (2)
#define     NFP_MU_PCTL_PHY_PIR_LPFDEPTH_16             (3)
#define   NFP_MU_PCTL_PHY_PIR_LPFEN                     (0x1 << 10)
#define   NFP_MU_PCTL_PHY_PIR_LPFEN_bf                  0, 10, 10
#define   NFP_MU_PCTL_PHY_PIR_LPFEN_mask                (0x1)
#define   NFP_MU_PCTL_PHY_PIR_LPFEN_bit                 (10)
#define   NFP_MU_PCTL_PHY_PIR_MDLEN                     (0x1 << 9)
#define   NFP_MU_PCTL_PHY_PIR_MDLEN_bf                  0, 9, 9
#define   NFP_MU_PCTL_PHY_PIR_MDLEN_mask                (0x1)
#define   NFP_MU_PCTL_PHY_PIR_MDLEN_bit                 (9)
#define   NFP_MU_PCTL_PHY_PIR_PLLPD                     (0x1 << 8)
#define   NFP_MU_PCTL_PHY_PIR_PLLPD_bf                  0, 8, 8
#define   NFP_MU_PCTL_PHY_PIR_PLLPD_mask                (0x1)
#define   NFP_MU_PCTL_PHY_PIR_PLLPD_bit                 (8)
#define   NFP_MU_PCTL_PHY_PIR_PLLRST                    (0x1 << 7)
#define   NFP_MU_PCTL_PHY_PIR_PLLRST_bf                 0, 7, 7
#define   NFP_MU_PCTL_PHY_PIR_PLLRST_mask               (0x1)
#define   NFP_MU_PCTL_PHY_PIR_PLLRST_bit                (7)
#define   NFP_MU_PCTL_PHY_PIR_PHYHRST                   (0x1 << 6)
#define   NFP_MU_PCTL_PHY_PIR_PHYHRST_bf                0, 6, 6
#define   NFP_MU_PCTL_PHY_PIR_PHYHRST_mask              (0x1)
#define   NFP_MU_PCTL_PHY_PIR_PHYHRST_bit               (6)
#define   NFP_MU_PCTL_PHY_PIR_PHYRST                    (0x1 << 5)
#define   NFP_MU_PCTL_PHY_PIR_PHYRST_bf                 0, 5, 5
#define   NFP_MU_PCTL_PHY_PIR_PHYRST_mask               (0x1)
#define   NFP_MU_PCTL_PHY_PIR_PHYRST_bit                (5)
#define   NFP_MU_PCTL_PHY_PIR_CALBYP                    (0x1 << 3)
#define   NFP_MU_PCTL_PHY_PIR_CALBYP_bf                 0, 3, 3
#define   NFP_MU_PCTL_PHY_PIR_CALBYP_mask               (0x1)
#define   NFP_MU_PCTL_PHY_PIR_CALBYP_bit                (3)
#define   NFP_MU_PCTL_PHY_PIR_CAL                       (0x1 << 2)
#define   NFP_MU_PCTL_PHY_PIR_CAL_bf                    0, 2, 2
#define   NFP_MU_PCTL_PHY_PIR_CAL_mask                  (0x1)
#define   NFP_MU_PCTL_PHY_PIR_CAL_bit                   (2)
#define   NFP_MU_PCTL_PHY_PIR_INITBYP                   (0x1 << 1)
#define   NFP_MU_PCTL_PHY_PIR_INITBYP_bf                0, 1, 1
#define   NFP_MU_PCTL_PHY_PIR_INITBYP_mask              (0x1)
#define   NFP_MU_PCTL_PHY_PIR_INITBYP_bit               (1)
#define   NFP_MU_PCTL_PHY_PIR_PHYINIT                   (0x1)
#define   NFP_MU_PCTL_PHY_PIR_PHYINIT_bf                0, 0, 0
#define   NFP_MU_PCTL_PHY_PIR_PHYINIT_mask              (0x1)
#define   NFP_MU_PCTL_PHY_PIR_PHYINIT_bit               (0)
/* Register Type: MUPhyPGCR0 */
#define NFP_MU_PCTL_PHY_PGCR0          0x0408
#define   NFP_MU_PCTL_PHY_PGCR0_CKEN(_x)                (((_x) & 0x3f) << 26)
#define   NFP_MU_PCTL_PHY_PGCR0_CKEN_bf                 0, 31, 26
#define   NFP_MU_PCTL_PHY_PGCR0_CKEN_mask               (0x3f)
#define   NFP_MU_PCTL_PHY_PGCR0_CKEN_shift              (26)
#define   NFP_MU_PCTL_PHY_PGCR0_LBMODE                  (0x1 << 25)
#define   NFP_MU_PCTL_PHY_PGCR0_LBMODE_bf               0, 25, 25
#define   NFP_MU_PCTL_PHY_PGCR0_LBMODE_mask             (0x1)
#define   NFP_MU_PCTL_PHY_PGCR0_LBMODE_bit              (25)
#define   NFP_MU_PCTL_PHY_PGCR0_LBCKSEL(_x)             (((_x) & 0x3) << 23)
#define   NFP_MU_PCTL_PHY_PGCR0_LBCKSEL_bf              0, 24, 23
#define   NFP_MU_PCTL_PHY_PGCR0_LBCKSEL_mask            (0x3)
#define   NFP_MU_PCTL_PHY_PGCR0_LBCKSEL_shift           (23)
#define     NFP_MU_PCTL_PHY_PGCR0_LBCKSEL_CK0           (0)
#define     NFP_MU_PCTL_PHY_PGCR0_LBCKSEL_CK1           (1)
#define     NFP_MU_PCTL_PHY_PGCR0_LBCKSEL_CK2           (2)
#define   NFP_MU_PCTL_PHY_PGCR0_LBSEL(_x)               (((_x) & 0x3) << 21)
#define   NFP_MU_PCTL_PHY_PGCR0_LBSEL_bf                0, 22, 21
#define   NFP_MU_PCTL_PHY_PGCR0_LBSEL_mask              (0x3)
#define   NFP_MU_PCTL_PHY_PGCR0_LBSEL_shift             (21)
#define     NFP_MU_PCTL_PHY_PGCR0_LBSEL_FIXME1          (0)
#define     NFP_MU_PCTL_PHY_PGCR0_LBSEL_FIXME2          (1)
#define     NFP_MU_PCTL_PHY_PGCR0_LBSEL_FIXME3          (2)
#define     NFP_MU_PCTL_PHY_PGCR0_LBSEL_FIXME4          (3)
#define   NFP_MU_PCTL_PHY_PGCR0_IOLB                    (0x1 << 20)
#define   NFP_MU_PCTL_PHY_PGCR0_IOLB_bf                 0, 20, 20
#define   NFP_MU_PCTL_PHY_PGCR0_IOLB_mask               (0x1)
#define   NFP_MU_PCTL_PHY_PGCR0_IOLB_bit                (20)
#define     NFP_MU_PCTL_PHY_PGCR0_IOLB_IOLB0            (0x0)
#define     NFP_MU_PCTL_PHY_PGCR0_IOLB_IOLB1            (0x100000)
#define   NFP_MU_PCTL_PHY_PGCR0_PLCKSEL                 (0x1 << 19)
#define   NFP_MU_PCTL_PHY_PGCR0_PLCKSEL_bf              0, 19, 19
#define   NFP_MU_PCTL_PHY_PGCR0_PLCKSEL_mask            (0x1)
#define   NFP_MU_PCTL_PHY_PGCR0_PLCKSEL_bit             (19)
#define     NFP_MU_PCTL_PHY_PGCR0_PLCKSEL_PLCKSEL0      (0x0)
#define     NFP_MU_PCTL_PHY_PGCR0_PLCKSEL_PLCKSEL1      (0x80000)
#define   NFP_MU_PCTL_PHY_PGCR0_DTOSEL(_x)              (((_x) & 0x1f) << 14)
#define   NFP_MU_PCTL_PHY_PGCR0_DTOSEL_bf               0, 18, 14
#define   NFP_MU_PCTL_PHY_PGCR0_DTOSEL_mask             (0x1f)
#define   NFP_MU_PCTL_PHY_PGCR0_DTOSEL_shift            (14)
#define     NFP_MU_PCTL_PHY_PGCR0_DTOSEL_FIXME1         (0)
#define     NFP_MU_PCTL_PHY_PGCR0_DTOSEL_FIXME2         (1)
#define     NFP_MU_PCTL_PHY_PGCR0_DTOSEL_FIXME3         (2)
#define     NFP_MU_PCTL_PHY_PGCR0_DTOSEL_FIXME4         (3)
#define     NFP_MU_PCTL_PHY_PGCR0_DTOSEL_FIXME5         (4)
#define     NFP_MU_PCTL_PHY_PGCR0_DTOSEL_FIXME6         (5)
#define     NFP_MU_PCTL_PHY_PGCR0_DTOSEL_FIXME7         (6)
#define     NFP_MU_PCTL_PHY_PGCR0_DTOSEL_FIXME8         (7)
#define     NFP_MU_PCTL_PHY_PGCR0_DTOSEL_FIXME9         (8)
#define     NFP_MU_PCTL_PHY_PGCR0_DTOSEL_FIXME10        (9)
#define     NFP_MU_PCTL_PHY_PGCR0_DTOSEL_FIXME11        (10)
#define     NFP_MU_PCTL_PHY_PGCR0_DTOSEL_FIXME12        (11)
#define     NFP_MU_PCTL_PHY_PGCR0_DTOSEL_FIXME13        (12)
#define     NFP_MU_PCTL_PHY_PGCR0_DTOSEL_FIXME14        (13)
#define     NFP_MU_PCTL_PHY_PGCR0_DTOSEL_FIXME15        (14)
#define     NFP_MU_PCTL_PHY_PGCR0_DTOSEL_FIXME16        (15)
#define     NFP_MU_PCTL_PHY_PGCR0_DTOSEL_FIXME17        (16)
#define     NFP_MU_PCTL_PHY_PGCR0_DTOSEL_FIXME18        (17)
#define     NFP_MU_PCTL_PHY_PGCR0_DTOSEL_FIXME19        (18)
#define     NFP_MU_PCTL_PHY_PGCR0_DTOSEL_FIXME20        (19)
#define   NFP_MU_PCTL_PHY_PGCR0_OSCWDL(_x)              (((_x) & 0x3) << 12)
#define   NFP_MU_PCTL_PHY_PGCR0_OSCWDL_bf               0, 13, 12
#define   NFP_MU_PCTL_PHY_PGCR0_OSCWDL_mask             (0x3)
#define   NFP_MU_PCTL_PHY_PGCR0_OSCWDL_shift            (12)
#define     NFP_MU_PCTL_PHY_PGCR0_OSCWDL_NONE           (0)
#define     NFP_MU_PCTL_PHY_PGCR0_OSCWDL_DDR            (1)
#define     NFP_MU_PCTL_PHY_PGCR0_OSCWDL_SDR            (2)
#define     NFP_MU_PCTL_PHY_PGCR0_OSCWDL_BOTH           (3)
#define   NFP_MU_PCTL_PHY_PGCR0_OSCDIV(_x)              (((_x) & 0x7) << 9)
#define   NFP_MU_PCTL_PHY_PGCR0_OSCDIV_bf               0, 11, 9
#define   NFP_MU_PCTL_PHY_PGCR0_OSCDIV_mask             (0x7)
#define   NFP_MU_PCTL_PHY_PGCR0_OSCDIV_shift            (9)
#define     NFP_MU_PCTL_PHY_PGCR0_OSCDIV_1              (0)
#define     NFP_MU_PCTL_PHY_PGCR0_OSCDIV_256            (1)
#define     NFP_MU_PCTL_PHY_PGCR0_OSCDIV_512            (2)
#define     NFP_MU_PCTL_PHY_PGCR0_OSCDIV_1024           (3)
#define     NFP_MU_PCTL_PHY_PGCR0_OSCDIV_2048           (4)
#define     NFP_MU_PCTL_PHY_PGCR0_OSCDIV_4096           (5)
#define     NFP_MU_PCTL_PHY_PGCR0_OSCDIV_8192           (6)
#define     NFP_MU_PCTL_PHY_PGCR0_OSCDIV_65536          (7)
#define   NFP_MU_PCTL_PHY_PGCR0_OSCEN                   (0x1 << 8)
#define   NFP_MU_PCTL_PHY_PGCR0_OSCEN_bf                0, 8, 8
#define   NFP_MU_PCTL_PHY_PGCR0_OSCEN_mask              (0x1)
#define   NFP_MU_PCTL_PHY_PGCR0_OSCEN_bit               (8)
#define   NFP_MU_PCTL_PHY_PGCR0_DLTST                   (0x1 << 7)
#define   NFP_MU_PCTL_PHY_PGCR0_DLTST_bf                0, 7, 7
#define   NFP_MU_PCTL_PHY_PGCR0_DLTST_mask              (0x1)
#define   NFP_MU_PCTL_PHY_PGCR0_DLTST_bit               (7)
#define   NFP_MU_PCTL_PHY_PGCR0_DLTMODE                 (0x1 << 6)
#define   NFP_MU_PCTL_PHY_PGCR0_DLTMODE_bf              0, 6, 6
#define   NFP_MU_PCTL_PHY_PGCR0_DLTMODE_mask            (0x1)
#define   NFP_MU_PCTL_PHY_PGCR0_DLTMODE_bit             (6)
#define   NFP_MU_PCTL_PHY_PGCR0_RDBVT                   (0x1 << 5)
#define   NFP_MU_PCTL_PHY_PGCR0_RDBVT_bf                0, 5, 5
#define   NFP_MU_PCTL_PHY_PGCR0_RDBVT_mask              (0x1)
#define   NFP_MU_PCTL_PHY_PGCR0_RDBVT_bit               (5)
#define   NFP_MU_PCTL_PHY_PGCR0_WDBVT                   (0x1 << 4)
#define   NFP_MU_PCTL_PHY_PGCR0_WDBVT_bf                0, 4, 4
#define   NFP_MU_PCTL_PHY_PGCR0_WDBVT_mask              (0x1)
#define   NFP_MU_PCTL_PHY_PGCR0_WDBVT_bit               (4)
#define   NFP_MU_PCTL_PHY_PGCR0_RGLVT                   (0x1 << 3)
#define   NFP_MU_PCTL_PHY_PGCR0_RGLVT_bf                0, 3, 3
#define   NFP_MU_PCTL_PHY_PGCR0_RGLVT_mask              (0x1)
#define   NFP_MU_PCTL_PHY_PGCR0_RGLVT_bit               (3)
#define   NFP_MU_PCTL_PHY_PGCR0_RDLVT                   (0x1 << 2)
#define   NFP_MU_PCTL_PHY_PGCR0_RDLVT_bf                0, 2, 2
#define   NFP_MU_PCTL_PHY_PGCR0_RDLVT_mask              (0x1)
#define   NFP_MU_PCTL_PHY_PGCR0_RDLVT_bit               (2)
#define   NFP_MU_PCTL_PHY_PGCR0_WDLVT                   (0x1 << 1)
#define   NFP_MU_PCTL_PHY_PGCR0_WDLVT_bf                0, 1, 1
#define   NFP_MU_PCTL_PHY_PGCR0_WDLVT_mask              (0x1)
#define   NFP_MU_PCTL_PHY_PGCR0_WDLVT_bit               (1)
#define   NFP_MU_PCTL_PHY_PGCR0_WLLVT                   (0x1)
#define   NFP_MU_PCTL_PHY_PGCR0_WLLVT_bf                0, 0, 0
#define   NFP_MU_PCTL_PHY_PGCR0_WLLVT_mask              (0x1)
#define   NFP_MU_PCTL_PHY_PGCR0_WLLVT_bit               (0)
/* Register Type: MUPhyPGCR1 */
#define NFP_MU_PCTL_PHY_PGCR1          0x040c
#define   NFP_MU_PCTL_PHY_PGCR1_WLSELT                  (0x1 << 6)
#define   NFP_MU_PCTL_PHY_PGCR1_WLSELT_bf               0, 6, 6
#define   NFP_MU_PCTL_PHY_PGCR1_WLSELT_mask             (0x1)
#define   NFP_MU_PCTL_PHY_PGCR1_WLSELT_bit              (6)
#define     NFP_MU_PCTL_PHY_PGCR1_WLSELT_1              (0x0)
#define     NFP_MU_PCTL_PHY_PGCR1_WLSELT_2              (0x40)
#define   NFP_MU_PCTL_PHY_PGCR1_WLRANK(_x)              (((_x) & 0x3) << 4)
#define   NFP_MU_PCTL_PHY_PGCR1_WLRANK_bf               0, 5, 4
#define   NFP_MU_PCTL_PHY_PGCR1_WLRANK_mask             (0x3)
#define   NFP_MU_PCTL_PHY_PGCR1_WLRANK_shift            (4)
#define     NFP_MU_PCTL_PHY_PGCR1_WLRANK_1              (0)
#define     NFP_MU_PCTL_PHY_PGCR1_WLRANK_2              (1)
#define     NFP_MU_PCTL_PHY_PGCR1_WLRANK_3              (2)
#define     NFP_MU_PCTL_PHY_PGCR1_WLRANK_4              (3)
#define   NFP_MU_PCTL_PHY_PGCR1_WLUNCRT                 (0x1 << 3)
#define   NFP_MU_PCTL_PHY_PGCR1_WLUNCRT_bf              0, 3, 3
#define   NFP_MU_PCTL_PHY_PGCR1_WLUNCRT_mask            (0x1)
#define   NFP_MU_PCTL_PHY_PGCR1_WLUNCRT_bit             (3)
#define   NFP_MU_PCTL_PHY_PGCR1_WLSTEP                  (0x1 << 2)
#define   NFP_MU_PCTL_PHY_PGCR1_WLSTEP_bf               0, 2, 2
#define   NFP_MU_PCTL_PHY_PGCR1_WLSTEP_mask             (0x1)
#define   NFP_MU_PCTL_PHY_PGCR1_WLSTEP_bit              (2)
#define     NFP_MU_PCTL_PHY_PGCR1_WLSTEP_32             (0x0)
#define     NFP_MU_PCTL_PHY_PGCR1_WLSTEP_1              (0x4)
#define   NFP_MU_PCTL_PHY_PGCR1_WFULL                   (0x1 << 1)
#define   NFP_MU_PCTL_PHY_PGCR1_WFULL_bf                0, 1, 1
#define   NFP_MU_PCTL_PHY_PGCR1_WFULL_mask              (0x1)
#define   NFP_MU_PCTL_PHY_PGCR1_WFULL_bit               (1)
#define   NFP_MU_PCTL_PHY_PGCR1_WLEN                    (0x1)
#define   NFP_MU_PCTL_PHY_PGCR1_WLEN_bf                 0, 0, 0
#define   NFP_MU_PCTL_PHY_PGCR1_WLEN_mask               (0x1)
#define   NFP_MU_PCTL_PHY_PGCR1_WLEN_bit                (0)
/* Register Type: MUPhyPGSR0 */
#define NFP_MU_PCTL_PHY_PGSR0          0x0410
#define   NFP_MU_PCTL_PHY_PGSR0_APLOCK                  (0x1 << 5)
#define   NFP_MU_PCTL_PHY_PGSR0_APLOCK_bf               0, 5, 5
#define   NFP_MU_PCTL_PHY_PGSR0_APLOCK_mask             (0x1)
#define   NFP_MU_PCTL_PHY_PGSR0_APLOCK_bit              (5)
#define   NFP_MU_PCTL_PHY_PGSR0_WLERR                   (0x1 << 4)
#define   NFP_MU_PCTL_PHY_PGSR0_WLERR_bf                0, 4, 4
#define   NFP_MU_PCTL_PHY_PGSR0_WLERR_mask              (0x1)
#define   NFP_MU_PCTL_PHY_PGSR0_WLERR_bit               (4)
#define   NFP_MU_PCTL_PHY_PGSR0_WL                      (0x1 << 3)
#define   NFP_MU_PCTL_PHY_PGSR0_WL_bf                   0, 3, 3
#define   NFP_MU_PCTL_PHY_PGSR0_WL_mask                 (0x1)
#define   NFP_MU_PCTL_PHY_PGSR0_WL_bit                  (3)
#define   NFP_MU_PCTL_PHY_PGSR0_CAL                     (0x1 << 2)
#define   NFP_MU_PCTL_PHY_PGSR0_CAL_bf                  0, 2, 2
#define   NFP_MU_PCTL_PHY_PGSR0_CAL_mask                (0x1)
#define   NFP_MU_PCTL_PHY_PGSR0_CAL_bit                 (2)
#define   NFP_MU_PCTL_PHY_PGSR0_PLLINIT                 (0x1 << 1)
#define   NFP_MU_PCTL_PHY_PGSR0_PLLINIT_bf              0, 1, 1
#define   NFP_MU_PCTL_PHY_PGSR0_PLLINIT_mask            (0x1)
#define   NFP_MU_PCTL_PHY_PGSR0_PLLINIT_bit             (1)
#define   NFP_MU_PCTL_PHY_PGSR0_INIT                    (0x1)
#define   NFP_MU_PCTL_PHY_PGSR0_INIT_bf                 0, 0, 0
#define   NFP_MU_PCTL_PHY_PGSR0_INIT_mask               (0x1)
#define   NFP_MU_PCTL_PHY_PGSR0_INIT_bit                (0)
/* Register Type: MUPhyPGSR1 */
#define NFP_MU_PCTL_PHY_PGSR1          0x0414
#define   NFP_MU_PCTL_PHY_PGSR1_DLTCODE_bf              0, 24, 1
#define   NFP_MU_PCTL_PHY_PGSR1_DLTCODE_mask            (0xffffff)
#define   NFP_MU_PCTL_PHY_PGSR1_DLTCODE_shift           (1)
#define   NFP_MU_PCTL_PHY_PGSR1_DLTDONE                 (0x1)
#define   NFP_MU_PCTL_PHY_PGSR1_DLTDONE_bf              0, 0, 0
#define   NFP_MU_PCTL_PHY_PGSR1_DLTDONE_mask            (0x1)
#define   NFP_MU_PCTL_PHY_PGSR1_DLTDONE_bit             (0)
/* Register Type: MUPhyPLLCR */
#define NFP_MU_PCTL_PHY_PLLCR          0x0418
#define   NFP_MU_PCTL_PHY_PLLCR_BYP                     (0x1 << 31)
#define   NFP_MU_PCTL_PHY_PLLCR_BYP_bf                  0, 31, 31
#define   NFP_MU_PCTL_PHY_PLLCR_BYP_mask                (0x1)
#define   NFP_MU_PCTL_PHY_PLLCR_BYP_bit                 (31)
#define   NFP_MU_PCTL_PHY_PLLCR_FRQSEL                  (0x1 << 18)
#define   NFP_MU_PCTL_PHY_PLLCR_FRQSEL_bf               0, 18, 18
#define   NFP_MU_PCTL_PHY_PLLCR_FRQSEL_mask             (0x1)
#define   NFP_MU_PCTL_PHY_PLLCR_FRQSEL_bit              (18)
#define     NFP_MU_PCTL_PHY_PLLCR_FRQSEL_FIXME1         (0x0)
#define     NFP_MU_PCTL_PHY_PLLCR_FRQSEL_FIXME2         (0x40000)
#define   NFP_MU_PCTL_PHY_PLLCR_QPMODE                  (0x1 << 17)
#define   NFP_MU_PCTL_PHY_PLLCR_QPMODE_bf               0, 17, 17
#define   NFP_MU_PCTL_PHY_PLLCR_QPMODE_mask             (0x1)
#define   NFP_MU_PCTL_PHY_PLLCR_QPMODE_bit              (17)
#define   NFP_MU_PCTL_PHY_PLLCR_CPPC(_x)                (((_x) & 0xf) << 13)
#define   NFP_MU_PCTL_PHY_PLLCR_CPPC_bf                 0, 16, 13
#define   NFP_MU_PCTL_PHY_PLLCR_CPPC_mask               (0xf)
#define   NFP_MU_PCTL_PHY_PLLCR_CPPC_shift              (13)
#define   NFP_MU_PCTL_PHY_PLLCR_CPIC(_x)                (((_x) & 0x3) << 11)
#define   NFP_MU_PCTL_PHY_PLLCR_CPIC_bf                 0, 12, 11
#define   NFP_MU_PCTL_PHY_PLLCR_CPIC_mask               (0x3)
#define   NFP_MU_PCTL_PHY_PLLCR_CPIC_shift              (11)
#define   NFP_MU_PCTL_PHY_PLLCR_GSHIFT                  (0x1 << 10)
#define   NFP_MU_PCTL_PHY_PLLCR_GSHIFT_bf               0, 10, 10
#define   NFP_MU_PCTL_PHY_PLLCR_GSHIFT_mask             (0x1)
#define   NFP_MU_PCTL_PHY_PLLCR_GSHIFT_bit              (10)
#define   NFP_MU_PCTL_PHY_PLLCR_ATOEN(_x)               (((_x) & 0xf) << 6)
#define   NFP_MU_PCTL_PHY_PLLCR_ATOEN_bf                0, 9, 6
#define   NFP_MU_PCTL_PHY_PLLCR_ATOEN_mask              (0xf)
#define   NFP_MU_PCTL_PHY_PLLCR_ATOEN_shift             (6)
#define     NFP_MU_PCTL_PHY_PLLCR_ATOEN_FIXME1          (0)
#define     NFP_MU_PCTL_PHY_PLLCR_ATOEN_FIXME2          (1)
#define     NFP_MU_PCTL_PHY_PLLCR_ATOEN_FIXME3          (2)
#define     NFP_MU_PCTL_PHY_PLLCR_ATOEN_FIXME4          (3)
#define     NFP_MU_PCTL_PHY_PLLCR_ATOEN_FIXME5          (4)
#define     NFP_MU_PCTL_PHY_PLLCR_ATOEN_FIXME6          (5)
#define     NFP_MU_PCTL_PHY_PLLCR_ATOEN_FIXME7          (6)
#define     NFP_MU_PCTL_PHY_PLLCR_ATOEN_FIXME8          (7)
#define     NFP_MU_PCTL_PHY_PLLCR_ATOEN_FIXME9          (8)
#define     NFP_MU_PCTL_PHY_PLLCR_ATOEN_FIXME10         (9)
#define     NFP_MU_PCTL_PHY_PLLCR_ATOEN_FIXME11         (10)
#define   NFP_MU_PCTL_PHY_PLLCR_ATC(_x)                 (((_x) & 0xf) << 2)
#define   NFP_MU_PCTL_PHY_PLLCR_ATC_bf                  0, 5, 2
#define   NFP_MU_PCTL_PHY_PLLCR_ATC_mask                (0xf)
#define   NFP_MU_PCTL_PHY_PLLCR_ATC_shift               (2)
#define   NFP_MU_PCTL_PHY_PLLCR_DTC(_x)                 ((_x) & 0x3)
#define   NFP_MU_PCTL_PHY_PLLCR_DTC_bf                  0, 1, 0
#define   NFP_MU_PCTL_PHY_PLLCR_DTC_mask                (0x3)
#define   NFP_MU_PCTL_PHY_PLLCR_DTC_shift               (0)
#define     NFP_MU_PCTL_PHY_PLLCR_DTC_DISABLE           (0)
#define     NFP_MU_PCTL_PHY_PLLCR_DTC_X1                (1)
#define     NFP_MU_PCTL_PHY_PLLCR_DTC_FB_X1             (2)
#define     NFP_MU_PCTL_PHY_PLLCR_DTC_REF_CLK           (3)
/* Register Type: MUPhyPTR0 */
#define NFP_MU_PCTL_PHY_PTR0           0x041c
#define   NFP_MU_PCTL_PHY_PTR0_T_PLLPD(_x)              (((_x) & 0x7ff) << 21)
#define   NFP_MU_PCTL_PHY_PTR0_T_PLLPD_bf               0, 31, 21
#define   NFP_MU_PCTL_PHY_PTR0_T_PLLPD_mask             (0x7ff)
#define   NFP_MU_PCTL_PHY_PTR0_T_PLLPD_shift            (21)
#define   NFP_MU_PCTL_PHY_PTR0_T_PHYRST(_x)             ((_x) & 0x3f)
#define   NFP_MU_PCTL_PHY_PTR0_T_PHYRST_bf              0, 5, 0
#define   NFP_MU_PCTL_PHY_PTR0_T_PHYRST_mask            (0x3f)
#define   NFP_MU_PCTL_PHY_PTR0_T_PHYRST_shift           (0)
/* Register Type: MUPhyPTR1 */
#define NFP_MU_PCTL_PHY_PTR1           0x0420
#define   NFP_MU_PCTL_PHY_PTR1_T_PLLLOCK(_x)            (((_x) & 0xffff) << 16)
#define   NFP_MU_PCTL_PHY_PTR1_T_PLLLOCK_bf             0, 31, 16
#define   NFP_MU_PCTL_PHY_PTR1_T_PLLLOCK_mask           (0xffff)
#define   NFP_MU_PCTL_PHY_PTR1_T_PLLLOCK_shift          (16)
#define   NFP_MU_PCTL_PHY_PTR1_T_PLLRST(_x)             ((_x) & 0x1fff)
#define   NFP_MU_PCTL_PHY_PTR1_T_PLLRST_bf              0, 12, 0
#define   NFP_MU_PCTL_PHY_PTR1_T_PLLRST_mask            (0x1fff)
#define   NFP_MU_PCTL_PHY_PTR1_T_PLLRST_shift           (0)
/* Register Type: MUPhyPTR2 */
#define NFP_MU_PCTL_PHY_PTR2           0x0424
#define   NFP_MU_PCTL_PHY_PTR2_T_WLDLYS(_x)             (((_x) & 0x1f) << 16)
#define   NFP_MU_PCTL_PHY_PTR2_T_WLDLYS_bf              0, 20, 16
#define   NFP_MU_PCTL_PHY_PTR2_T_WLDLYS_mask            (0x1f)
#define   NFP_MU_PCTL_PHY_PTR2_T_WLDLYS_shift           (16)
#define   NFP_MU_PCTL_PHY_PTR2_T_WLO(_x)                (((_x) & 0xf) << 12)
#define   NFP_MU_PCTL_PHY_PTR2_T_WLO_bf                 0, 15, 12
#define   NFP_MU_PCTL_PHY_PTR2_T_WLO_mask               (0xf)
#define   NFP_MU_PCTL_PHY_PTR2_T_WLO_shift              (12)
#define   NFP_MU_PCTL_PHY_PTR2_T_CALH(_x)               (((_x) & 0xf) << 8)
#define   NFP_MU_PCTL_PHY_PTR2_T_CALH_bf                0, 11, 8
#define   NFP_MU_PCTL_PHY_PTR2_T_CALH_mask              (0xf)
#define   NFP_MU_PCTL_PHY_PTR2_T_CALH_shift             (8)
#define   NFP_MU_PCTL_PHY_PTR2_T_CALS(_x)               (((_x) & 0xf) << 4)
#define   NFP_MU_PCTL_PHY_PTR2_T_CALS_bf                0, 7, 4
#define   NFP_MU_PCTL_PHY_PTR2_T_CALS_mask              (0xf)
#define   NFP_MU_PCTL_PHY_PTR2_T_CALS_shift             (4)
#define   NFP_MU_PCTL_PHY_PTR2_T_CALON(_x)              ((_x) & 0xf)
#define   NFP_MU_PCTL_PHY_PTR2_T_CALON_bf               0, 3, 0
#define   NFP_MU_PCTL_PHY_PTR2_T_CALON_mask             (0xf)
#define   NFP_MU_PCTL_PHY_PTR2_T_CALON_shift            (0)
/* Register Type: MUPhyACMDLR */
#define NFP_MU_PCTL_PHY_ACMDLR         0x0428
#define   NFP_MU_PCTL_PHY_ACMDLR_MDLD(_x)               (((_x) & 0xff) << 16)
#define   NFP_MU_PCTL_PHY_ACMDLR_MDLD_bf                0, 23, 16
#define   NFP_MU_PCTL_PHY_ACMDLR_MDLD_mask              (0xff)
#define   NFP_MU_PCTL_PHY_ACMDLR_MDLD_shift             (16)
#define   NFP_MU_PCTL_PHY_ACMDLR_TPRD(_x)               (((_x) & 0xff) << 8)
#define   NFP_MU_PCTL_PHY_ACMDLR_TPRD_bf                0, 15, 8
#define   NFP_MU_PCTL_PHY_ACMDLR_TPRD_mask              (0xff)
#define   NFP_MU_PCTL_PHY_ACMDLR_TPRD_shift             (8)
#define   NFP_MU_PCTL_PHY_ACMDLR_IPRD(_x)               ((_x) & 0xff)
#define   NFP_MU_PCTL_PHY_ACMDLR_IPRD_bf                0, 7, 0
#define   NFP_MU_PCTL_PHY_ACMDLR_IPRD_mask              (0xff)
#define   NFP_MU_PCTL_PHY_ACMDLR_IPRD_shift             (0)
/* Register Type: MUPhyACBDLR */
#define NFP_MU_PCTL_PHY_ACBDLR         0x042c
#define   NFP_MU_PCTL_PHY_ACBDLR_ACBD(_x)               (((_x) & 0x3f) << 18)
#define   NFP_MU_PCTL_PHY_ACBDLR_ACBD_bf                0, 23, 18
#define   NFP_MU_PCTL_PHY_ACBDLR_ACBD_mask              (0x3f)
#define   NFP_MU_PCTL_PHY_ACBDLR_ACBD_shift             (18)
#define   NFP_MU_PCTL_PHY_ACBDLR_CK2BD(_x)              (((_x) & 0x3f) << 12)
#define   NFP_MU_PCTL_PHY_ACBDLR_CK2BD_bf               0, 17, 12
#define   NFP_MU_PCTL_PHY_ACBDLR_CK2BD_mask             (0x3f)
#define   NFP_MU_PCTL_PHY_ACBDLR_CK2BD_shift            (12)
#define   NFP_MU_PCTL_PHY_ACBDLR_CK1BD(_x)              (((_x) & 0x3f) << 6)
#define   NFP_MU_PCTL_PHY_ACBDLR_CK1BD_bf               0, 11, 6
#define   NFP_MU_PCTL_PHY_ACBDLR_CK1BD_mask             (0x3f)
#define   NFP_MU_PCTL_PHY_ACBDLR_CK1BD_shift            (6)
#define   NFP_MU_PCTL_PHY_ACBDLR_CK0BD(_x)              ((_x) & 0x3f)
#define   NFP_MU_PCTL_PHY_ACBDLR_CK0BD_bf               0, 5, 0
#define   NFP_MU_PCTL_PHY_ACBDLR_CK0BD_mask             (0x3f)
#define   NFP_MU_PCTL_PHY_ACBDLR_CK0BD_shift            (0)
/* Register Type: MUPhyACIOCR */
#define NFP_MU_PCTL_PHY_ACIOCR         0x0430
#define   NFP_MU_PCTL_PHY_ACIOCR_RSTPDR                 (0x1 << 28)
#define   NFP_MU_PCTL_PHY_ACIOCR_RSTPDR_bf              0, 28, 28
#define   NFP_MU_PCTL_PHY_ACIOCR_RSTPDR_mask            (0x1)
#define   NFP_MU_PCTL_PHY_ACIOCR_RSTPDR_bit             (28)
#define   NFP_MU_PCTL_PHY_ACIOCR_RSTPDD                 (0x1 << 27)
#define   NFP_MU_PCTL_PHY_ACIOCR_RSTPDD_bf              0, 27, 27
#define   NFP_MU_PCTL_PHY_ACIOCR_RSTPDD_mask            (0x1)
#define   NFP_MU_PCTL_PHY_ACIOCR_RSTPDD_bit             (27)
#define   NFP_MU_PCTL_PHY_ACIOCR_RSTODT                 (0x1 << 26)
#define   NFP_MU_PCTL_PHY_ACIOCR_RSTODT_bf              0, 26, 26
#define   NFP_MU_PCTL_PHY_ACIOCR_RSTODT_mask            (0x1)
#define   NFP_MU_PCTL_PHY_ACIOCR_RSTODT_bit             (26)
#define   NFP_MU_PCTL_PHY_ACIOCR_RANKPDR(_x)            (((_x) & 0xf) << 22)
#define   NFP_MU_PCTL_PHY_ACIOCR_RANKPDR_bf             0, 25, 22
#define   NFP_MU_PCTL_PHY_ACIOCR_RANKPDR_mask           (0xf)
#define   NFP_MU_PCTL_PHY_ACIOCR_RANKPDR_shift          (22)
#define   NFP_MU_PCTL_PHY_ACIOCR_RANKPDD(_x)            (((_x) & 0xf) << 18)
#define   NFP_MU_PCTL_PHY_ACIOCR_RANKPDD_bf             0, 21, 18
#define   NFP_MU_PCTL_PHY_ACIOCR_RANKPDD_mask           (0xf)
#define   NFP_MU_PCTL_PHY_ACIOCR_RANKPDD_shift          (18)
#define   NFP_MU_PCTL_PHY_ACIOCR_RANKODT(_x)            (((_x) & 0xf) << 14)
#define   NFP_MU_PCTL_PHY_ACIOCR_RANKODT_bf             0, 17, 14
#define   NFP_MU_PCTL_PHY_ACIOCR_RANKODT_mask           (0xf)
#define   NFP_MU_PCTL_PHY_ACIOCR_RANKODT_shift          (14)
#define   NFP_MU_PCTL_PHY_ACIOCR_CKPDR(_x)              (((_x) & 0x7) << 11)
#define   NFP_MU_PCTL_PHY_ACIOCR_CKPDR_bf               0, 13, 11
#define   NFP_MU_PCTL_PHY_ACIOCR_CKPDR_mask             (0x7)
#define   NFP_MU_PCTL_PHY_ACIOCR_CKPDR_shift            (11)
#define   NFP_MU_PCTL_PHY_ACIOCR_CKPDD(_x)              (((_x) & 0x7) << 8)
#define   NFP_MU_PCTL_PHY_ACIOCR_CKPDD_bf               0, 10, 8
#define   NFP_MU_PCTL_PHY_ACIOCR_CKPDD_mask             (0x7)
#define   NFP_MU_PCTL_PHY_ACIOCR_CKPDD_shift            (8)
#define   NFP_MU_PCTL_PHY_ACIOCR_CKODT(_x)              (((_x) & 0x7) << 5)
#define   NFP_MU_PCTL_PHY_ACIOCR_CKODT_bf               0, 7, 5
#define   NFP_MU_PCTL_PHY_ACIOCR_CKODT_mask             (0x7)
#define   NFP_MU_PCTL_PHY_ACIOCR_CKODT_shift            (5)
#define   NFP_MU_PCTL_PHY_ACIOCR_ACPDR                  (0x1 << 4)
#define   NFP_MU_PCTL_PHY_ACIOCR_ACPDR_bf               0, 4, 4
#define   NFP_MU_PCTL_PHY_ACIOCR_ACPDR_mask             (0x1)
#define   NFP_MU_PCTL_PHY_ACIOCR_ACPDR_bit              (4)
#define   NFP_MU_PCTL_PHY_ACIOCR_ACPDD                  (0x1 << 3)
#define   NFP_MU_PCTL_PHY_ACIOCR_ACPDD_bf               0, 3, 3
#define   NFP_MU_PCTL_PHY_ACIOCR_ACPDD_mask             (0x1)
#define   NFP_MU_PCTL_PHY_ACIOCR_ACPDD_bit              (3)
#define   NFP_MU_PCTL_PHY_ACIOCR_ACODT                  (0x1 << 2)
#define   NFP_MU_PCTL_PHY_ACIOCR_ACODT_bf               0, 2, 2
#define   NFP_MU_PCTL_PHY_ACIOCR_ACODT_mask             (0x1)
#define   NFP_MU_PCTL_PHY_ACIOCR_ACODT_bit              (2)
#define   NFP_MU_PCTL_PHY_ACIOCR_ACOE                   (0x1 << 1)
#define   NFP_MU_PCTL_PHY_ACIOCR_ACOE_bf                0, 1, 1
#define   NFP_MU_PCTL_PHY_ACIOCR_ACOE_mask              (0x1)
#define   NFP_MU_PCTL_PHY_ACIOCR_ACOE_bit               (1)
#define   NFP_MU_PCTL_PHY_ACIOCR_ACIOM                  (0x1)
#define   NFP_MU_PCTL_PHY_ACIOCR_ACIOM_bf               0, 0, 0
#define   NFP_MU_PCTL_PHY_ACIOCR_ACIOM_mask             (0x1)
#define   NFP_MU_PCTL_PHY_ACIOCR_ACIOM_bit              (0)
/* Register Type: MUPhyDXCCR */
#define NFP_MU_PCTL_PHY_DXCCR          0x0434
#define   NFP_MU_PCTL_PHY_DXCCR_DQSNRES(_x)             (((_x) & 0xf) << 9)
#define   NFP_MU_PCTL_PHY_DXCCR_DQSNRES_bf              0, 12, 9
#define   NFP_MU_PCTL_PHY_DXCCR_DQSNRES_mask            (0xf)
#define   NFP_MU_PCTL_PHY_DXCCR_DQSNRES_shift           (9)
#define   NFP_MU_PCTL_PHY_DXCCR_DQRES(_x)               (((_x) & 0xf) << 5)
#define   NFP_MU_PCTL_PHY_DXCCR_DQRES_bf                0, 8, 5
#define   NFP_MU_PCTL_PHY_DXCCR_DQRES_mask              (0xf)
#define   NFP_MU_PCTL_PHY_DXCCR_DQRES_shift             (5)
#define     NFP_MU_PCTL_PHY_DXCCR_DQRES_OPEN            (0)
#define     NFP_MU_PCTL_PHY_DXCCR_DQRES_688             (1)
#define     NFP_MU_PCTL_PHY_DXCCR_DQRES_611             (2)
#define     NFP_MU_PCTL_PHY_DXCCR_DQRES_550             (3)
#define     NFP_MU_PCTL_PHY_DXCCR_DQRES_500             (4)
#define     NFP_MU_PCTL_PHY_DXCCR_DQRES_458             (5)
#define     NFP_MU_PCTL_PHY_DXCCR_DQRES_393             (6)
#define     NFP_MU_PCTL_PHY_DXCCR_DQRES_344             (7)
#define   NFP_MU_PCTL_PHY_DXCCR_DXPDR                   (0x1 << 4)
#define   NFP_MU_PCTL_PHY_DXCCR_DXPDR_bf                0, 4, 4
#define   NFP_MU_PCTL_PHY_DXCCR_DXPDR_mask              (0x1)
#define   NFP_MU_PCTL_PHY_DXCCR_DXPDR_bit               (4)
#define   NFP_MU_PCTL_PHY_DXCCR_DXPDD                   (0x1 << 3)
#define   NFP_MU_PCTL_PHY_DXCCR_DXPDD_bf                0, 3, 3
#define   NFP_MU_PCTL_PHY_DXCCR_DXPDD_mask              (0x1)
#define   NFP_MU_PCTL_PHY_DXCCR_DXPDD_bit               (3)
#define   NFP_MU_PCTL_PHY_DXCCR_MDLEN                   (0x1 << 2)
#define   NFP_MU_PCTL_PHY_DXCCR_MDLEN_bf                0, 2, 2
#define   NFP_MU_PCTL_PHY_DXCCR_MDLEN_mask              (0x1)
#define   NFP_MU_PCTL_PHY_DXCCR_MDLEN_bit               (2)
#define   NFP_MU_PCTL_PHY_DXCCR_DXIOM                   (0x1 << 1)
#define   NFP_MU_PCTL_PHY_DXCCR_DXIOM_bf                0, 1, 1
#define   NFP_MU_PCTL_PHY_DXCCR_DXIOM_mask              (0x1)
#define   NFP_MU_PCTL_PHY_DXCCR_DXIOM_bit               (1)
#define   NFP_MU_PCTL_PHY_DXCCR_DXODT                   (0x1)
#define   NFP_MU_PCTL_PHY_DXCCR_DXODT_bf                0, 0, 0
#define   NFP_MU_PCTL_PHY_DXCCR_DXODT_mask              (0x1)
#define   NFP_MU_PCTL_PHY_DXCCR_DXODT_bit               (0)
/* Register Type: MUPhyZQCR */
#define NFP_MU_PCTL_PHY_ZQCR0          0x05a0
#define NFP_MU_PCTL_PHY_ZQCR1          0x05a8
#define   NFP_MU_PCTL_PHY_ZQCR_IMP_CAL_TRIG             (0x1 << 30)
#define   NFP_MU_PCTL_PHY_ZQCR_IMP_CAL_TRIG_bf          0, 30, 30
#define   NFP_MU_PCTL_PHY_ZQCR_IMP_CAL_TRIG_mask        (0x1)
#define   NFP_MU_PCTL_PHY_ZQCR_IMP_CAL_TRIG_bit         (30)
#define   NFP_MU_PCTL_PHY_ZQCR_IMP_CAL_BYP              (0x1 << 29)
#define   NFP_MU_PCTL_PHY_ZQCR_IMP_CAL_BYP_bf           0, 29, 29
#define   NFP_MU_PCTL_PHY_ZQCR_IMP_CAL_BYP_mask         (0x1)
#define   NFP_MU_PCTL_PHY_ZQCR_IMP_CAL_BYP_bit          (29)
#define   NFP_MU_PCTL_PHY_ZQCR_IMP_OVERRIDE_EN          (0x1 << 28)
#define   NFP_MU_PCTL_PHY_ZQCR_IMP_OVERRIDE_EN_bf       0, 28, 28
#define   NFP_MU_PCTL_PHY_ZQCR_IMP_OVERRIDE_EN_mask     (0x1)
#define   NFP_MU_PCTL_PHY_ZQCR_IMP_OVERRIDE_EN_bit      (28)
#define   NFP_MU_PCTL_PHY_ZQCR_IMP_DIV(_x)              (((_x) & 0xff) << 20)
#define   NFP_MU_PCTL_PHY_ZQCR_IMP_DIV_bf               0, 27, 20
#define   NFP_MU_PCTL_PHY_ZQCR_IMP_DIV_mask             (0xff)
#define   NFP_MU_PCTL_PHY_ZQCR_IMP_DIV_shift            (20)
#define   NFP_MU_PCTL_PHY_ZQCR_IMP_OVERRIDE_DATA(_x)    ((_x) & 0xfffff)
#define   NFP_MU_PCTL_PHY_ZQCR_IMP_OVERRIDE_DATA_bf     0, 19, 0
#define   NFP_MU_PCTL_PHY_ZQCR_IMP_OVERRIDE_DATA_mask   (0xfffff)
#define   NFP_MU_PCTL_PHY_ZQCR_IMP_OVERRIDE_DATA_shift  (0)
/* Register Type: MUPhyZQSR */
#define NFP_MU_PCTL_PHY_ZQSR0          0x05a4
#define NFP_MU_PCTL_PHY_ZQSR1          0x05ac
#define   NFP_MU_PCTL_PHY_ZQSR_IMP_CAL_DONE             (0x1 << 31)
#define   NFP_MU_PCTL_PHY_ZQSR_IMP_CAL_DONE_bf          0, 31, 31
#define   NFP_MU_PCTL_PHY_ZQSR_IMP_CAL_DONE_mask        (0x1)
#define   NFP_MU_PCTL_PHY_ZQSR_IMP_CAL_DONE_bit         (31)
#define   NFP_MU_PCTL_PHY_ZQSR_IMP_CAL_ERROR            (0x1 << 30)
#define   NFP_MU_PCTL_PHY_ZQSR_IMP_CAL_ERROR_bf         0, 30, 30
#define   NFP_MU_PCTL_PHY_ZQSR_IMP_CAL_ERROR_mask       (0x1)
#define   NFP_MU_PCTL_PHY_ZQSR_IMP_CAL_ERROR_bit        (30)
#define   NFP_MU_PCTL_PHY_ZQSR_TERM_PULLUP_CAL_STATUS_bf 0, 27, 26
#define   NFP_MU_PCTL_PHY_ZQSR_TERM_PULLUP_CAL_STATUS_mask (0x3)
#define   NFP_MU_PCTL_PHY_ZQSR_TERM_PULLUP_CAL_STATUS_shift (26)
#define   NFP_MU_PCTL_PHY_ZQSR_TERM_PULLDOWN_CAL_STATUS_bf 0, 25, 24
#define   NFP_MU_PCTL_PHY_ZQSR_TERM_PULLDOWN_CAL_STATUS_mask (0x3)
#define   NFP_MU_PCTL_PHY_ZQSR_TERM_PULLDOWN_CAL_STATUS_shift (24)
#define   NFP_MU_PCTL_PHY_ZQSR_IMP_PULLUP_CAL_STATUS_bf 0, 23, 22
#define   NFP_MU_PCTL_PHY_ZQSR_IMP_PULLUP_CAL_STATUS_mask (0x3)
#define   NFP_MU_PCTL_PHY_ZQSR_IMP_PULLUP_CAL_STATUS_shift (22)
#define     NFP_MU_PCTL_PHY_ZQSR_IMP_PULLUP_CAL_STATUS_OK (0)
#define     NFP_MU_PCTL_PHY_ZQSR_IMP_PULLUP_CAL_STATUS_OVERFLOW (1)
#define     NFP_MU_PCTL_PHY_ZQSR_IMP_PULLUP_CAL_STATUS_UNDEFLOW (2)
#define     NFP_MU_PCTL_PHY_ZQSR_IMP_PULLUP_CAL_STATUS_CALIBRATING (3)
#define   NFP_MU_PCTL_PHY_ZQSR_IMP_PULLDOWN_CAL_STATUS_bf 0, 21, 20
#define   NFP_MU_PCTL_PHY_ZQSR_IMP_PULLDOWN_CAL_STATUS_mask (0x3)
#define   NFP_MU_PCTL_PHY_ZQSR_IMP_PULLDOWN_CAL_STATUS_shift (20)
#define     NFP_MU_PCTL_PHY_ZQSR_IMP_PULLDOWN_CAL_STATUS_OK (0)
#define     NFP_MU_PCTL_PHY_ZQSR_IMP_PULLDOWN_CAL_STATUS_OVERFLOW (1)
#define     NFP_MU_PCTL_PHY_ZQSR_IMP_PULLDOWN_CAL_STATUS_UNDERFLOW (2)
#define     NFP_MU_PCTL_PHY_ZQSR_IMP_PULLDOWN_CAL_STATUS_CALIBRATING (3)
#define   NFP_MU_PCTL_PHY_ZQSR_IMP_CTRL_bf              0, 19, 0
#define   NFP_MU_PCTL_PHY_ZQSR_IMP_CTRL_mask            (0xfffff)
#define   NFP_MU_PCTL_PHY_ZQSR_IMP_CTRL_shift           (0)
/* Register Type: MUPhyDXGCR */
#define NFP_MU_PCTL_PHY_DXGCR0         0x05c0
#define NFP_MU_PCTL_PHY_DXGCR1         0x0600
#define NFP_MU_PCTL_PHY_DXGCR2         0x0640
#define NFP_MU_PCTL_PHY_DXGCR3         0x0680
#define NFP_MU_PCTL_PHY_DXGCR4         0x06c0
#define NFP_MU_PCTL_PHY_DXGCR5         0x0700
#define NFP_MU_PCTL_PHY_DXGCR6         0x0740
#define NFP_MU_PCTL_PHY_DXGCR7         0x0780
#define NFP_MU_PCTL_PHY_DXGCR8         0x07c0
#define   NFP_MU_PCTL_PHY_DXGCR_POWER_DOWN_RECEIVER     (0x1 << 10)
#define   NFP_MU_PCTL_PHY_DXGCR_POWER_DOWN_RECEIVER_bf  0, 10, 10
#define   NFP_MU_PCTL_PHY_DXGCR_POWER_DOWN_RECEIVER_mask (0x1)
#define   NFP_MU_PCTL_PHY_DXGCR_POWER_DOWN_RECEIVER_bit (10)
#define   NFP_MU_PCTL_PHY_DXGCR_POWER_DOWN_DRIVER       (0x1 << 9)
#define   NFP_MU_PCTL_PHY_DXGCR_POWER_DOWN_DRIVER_bf    0, 9, 9
#define   NFP_MU_PCTL_PHY_DXGCR_POWER_DOWN_DRIVER_mask  (0x1)
#define   NFP_MU_PCTL_PHY_DXGCR_POWER_DOWN_DRIVER_bit   (9)
#define   NFP_MU_PCTL_PHY_DXGCR_MASTER_DELAY_LINE_EN    (0x1 << 8)
#define   NFP_MU_PCTL_PHY_DXGCR_MASTER_DELAY_LINE_EN_bf 0, 8, 8
#define   NFP_MU_PCTL_PHY_DXGCR_MASTER_DELAY_LINE_EN_mask (0x1)
#define   NFP_MU_PCTL_PHY_DXGCR_MASTER_DELAY_LINE_EN_bit (8)
#define   NFP_MU_PCTL_PHY_DXGCR_WRITE_LEVEL_RANK_EN(_x) (((_x) & 0xf) << 4)
#define   NFP_MU_PCTL_PHY_DXGCR_WRITE_LEVEL_RANK_EN_bf  0, 7, 4
#define   NFP_MU_PCTL_PHY_DXGCR_WRITE_LEVEL_RANK_EN_mask (0xf)
#define   NFP_MU_PCTL_PHY_DXGCR_WRITE_LEVEL_RANK_EN_shift (4)
#define   NFP_MU_PCTL_PHY_DXGCR_IDDQ_TEST               (0x1 << 3)
#define   NFP_MU_PCTL_PHY_DXGCR_IDDQ_TEST_bf            0, 3, 3
#define   NFP_MU_PCTL_PHY_DXGCR_IDDQ_TEST_mask          (0x1)
#define   NFP_MU_PCTL_PHY_DXGCR_IDDQ_TEST_bit           (3)
#define   NFP_MU_PCTL_PHY_DXGCR_TERM                    (0x1 << 2)
#define   NFP_MU_PCTL_PHY_DXGCR_TERM_bf                 0, 2, 2
#define   NFP_MU_PCTL_PHY_DXGCR_TERM_mask               (0x1)
#define   NFP_MU_PCTL_PHY_DXGCR_TERM_bit                (2)
#define   NFP_MU_PCTL_PHY_DXGCR_CAL_BYP                 (0x1 << 1)
#define   NFP_MU_PCTL_PHY_DXGCR_CAL_BYP_bf              0, 1, 1
#define   NFP_MU_PCTL_PHY_DXGCR_CAL_BYP_mask            (0x1)
#define   NFP_MU_PCTL_PHY_DXGCR_CAL_BYP_bit             (1)
#define   NFP_MU_PCTL_PHY_DXGCR_DATA_EN                 (0x1)
#define   NFP_MU_PCTL_PHY_DXGCR_DATA_EN_bf              0, 0, 0
#define   NFP_MU_PCTL_PHY_DXGCR_DATA_EN_mask            (0x1)
#define   NFP_MU_PCTL_PHY_DXGCR_DATA_EN_bit             (0)
/* Register Type: MUPhyDXGSR0 */
#define NFP_MU_PCTL_PHY_DXGSR00        0x05c4
#define NFP_MU_PCTL_PHY_DXGSR01        0x0604
#define NFP_MU_PCTL_PHY_DXGSR02        0x0644
#define NFP_MU_PCTL_PHY_DXGSR03        0x0684
#define NFP_MU_PCTL_PHY_DXGSR04        0x06c4
#define NFP_MU_PCTL_PHY_DXGSR05        0x0704
#define NFP_MU_PCTL_PHY_DXGSR06        0x0744
#define NFP_MU_PCTL_PHY_DXGSR07        0x0784
#define NFP_MU_PCTL_PHY_DXGSR08        0x07c4
#define   NFP_MU_PCTL_PHY_DXGSR0_READ_DQS_PERIOD_bf     0, 22, 15
#define   NFP_MU_PCTL_PHY_DXGSR0_READ_DQS_PERIOD_mask   (0xff)
#define   NFP_MU_PCTL_PHY_DXGSR0_READ_DQS_PERIOD_shift  (15)
#define   NFP_MU_PCTL_PHY_DXGSR0_DATX8_PLL_LOCK         (0x1 << 14)
#define   NFP_MU_PCTL_PHY_DXGSR0_DATX8_PLL_LOCK_bf      0, 14, 14
#define   NFP_MU_PCTL_PHY_DXGSR0_DATX8_PLL_LOCK_mask    (0x1)
#define   NFP_MU_PCTL_PHY_DXGSR0_DATX8_PLL_LOCK_bit     (14)
#define   NFP_MU_PCTL_PHY_DXGSR0_WRITE_LVL_PERIOD_bf    0, 13, 6
#define   NFP_MU_PCTL_PHY_DXGSR0_WRITE_LVL_PERIOD_mask  (0xff)
#define   NFP_MU_PCTL_PHY_DXGSR0_WRITE_LVL_PERIOD_shift (6)
#define   NFP_MU_PCTL_PHY_DXGSR0_WRITE_LVL_ERROR        (0x1 << 5)
#define   NFP_MU_PCTL_PHY_DXGSR0_WRITE_LVL_ERROR_bf     0, 5, 5
#define   NFP_MU_PCTL_PHY_DXGSR0_WRITE_LVL_ERROR_mask   (0x1)
#define   NFP_MU_PCTL_PHY_DXGSR0_WRITE_LVL_ERROR_bit    (5)
#define   NFP_MU_PCTL_PHY_DXGSR0_WRITE_LVL_DONE         (0x1 << 4)
#define   NFP_MU_PCTL_PHY_DXGSR0_WRITE_LVL_DONE_bf      0, 4, 4
#define   NFP_MU_PCTL_PHY_DXGSR0_WRITE_LVL_DONE_mask    (0x1)
#define   NFP_MU_PCTL_PHY_DXGSR0_WRITE_LVL_DONE_bit     (4)
#define   NFP_MU_PCTL_PHY_DXGSR0_WRITE_LVL_CAL          (0x1 << 3)
#define   NFP_MU_PCTL_PHY_DXGSR0_WRITE_LVL_CAL_bf       0, 3, 3
#define   NFP_MU_PCTL_PHY_DXGSR0_WRITE_LVL_CAL_mask     (0x1)
#define   NFP_MU_PCTL_PHY_DXGSR0_WRITE_LVL_CAL_bit      (3)
#define   NFP_MU_PCTL_PHY_DXGSR0_READ_DQS_GATING_CAL    (0x1 << 2)
#define   NFP_MU_PCTL_PHY_DXGSR0_READ_DQS_GATING_CAL_bf 0, 2, 2
#define   NFP_MU_PCTL_PHY_DXGSR0_READ_DQS_GATING_CAL_mask (0x1)
#define   NFP_MU_PCTL_PHY_DXGSR0_READ_DQS_GATING_CAL_bit (2)
#define   NFP_MU_PCTL_PHY_DXGSR0_READ_DQS_CAL           (0x1 << 1)
#define   NFP_MU_PCTL_PHY_DXGSR0_READ_DQS_CAL_bf        0, 1, 1
#define   NFP_MU_PCTL_PHY_DXGSR0_READ_DQS_CAL_mask      (0x1)
#define   NFP_MU_PCTL_PHY_DXGSR0_READ_DQS_CAL_bit       (1)
#define   NFP_MU_PCTL_PHY_DXGSR0_WRITE_DQ_CAL           (0x1)
#define   NFP_MU_PCTL_PHY_DXGSR0_WRITE_DQ_CAL_bf        0, 0, 0
#define   NFP_MU_PCTL_PHY_DXGSR0_WRITE_DQ_CAL_mask      (0x1)
#define   NFP_MU_PCTL_PHY_DXGSR0_WRITE_DQ_CAL_bit       (0)
/* Register Type: MUPhyDXGSR1 */
#define NFP_MU_PCTL_PHY_DXGSR10        0x05c8
#define NFP_MU_PCTL_PHY_DXGSR11        0x0608
#define NFP_MU_PCTL_PHY_DXGSR12        0x0648
#define NFP_MU_PCTL_PHY_DXGSR13        0x0688
#define NFP_MU_PCTL_PHY_DXGSR14        0x06c8
#define NFP_MU_PCTL_PHY_DXGSR15        0x0708
#define NFP_MU_PCTL_PHY_DXGSR16        0x0748
#define NFP_MU_PCTL_PHY_DXGSR17        0x0788
#define NFP_MU_PCTL_PHY_DXGSR18        0x07c8
#define   NFP_MU_PCTL_PHY_DXGSR1_DELAY_LINE_TEST_CODE_bf 0, 24, 1
#define   NFP_MU_PCTL_PHY_DXGSR1_DELAY_LINE_TEST_CODE_mask (0xffffff)
#define   NFP_MU_PCTL_PHY_DXGSR1_DELAY_LINE_TEST_CODE_shift (1)
#define   NFP_MU_PCTL_PHY_DXGSR1_DELAY_LINE_TEST_DONE   (0x1)
#define   NFP_MU_PCTL_PHY_DXGSR1_DELAY_LINE_TEST_DONE_bf 0, 0, 0
#define   NFP_MU_PCTL_PHY_DXGSR1_DELAY_LINE_TEST_DONE_mask (0x1)
#define   NFP_MU_PCTL_PHY_DXGSR1_DELAY_LINE_TEST_DONE_bit (0)
/* Register Type: MUPhyDXBDLR0 */
#define NFP_MU_PCTL_PHY_DXBDLR00       0x05cc
#define NFP_MU_PCTL_PHY_DXBDLR01       0x060c
#define NFP_MU_PCTL_PHY_DXBDLR02       0x064c
#define NFP_MU_PCTL_PHY_DXBDLR03       0x068c
#define NFP_MU_PCTL_PHY_DXBDLR04       0x06cc
#define NFP_MU_PCTL_PHY_DXBDLR05       0x070c
#define NFP_MU_PCTL_PHY_DXBDLR06       0x074c
#define NFP_MU_PCTL_PHY_DXBDLR07       0x078c
#define NFP_MU_PCTL_PHY_DXBDLR08       0x07cc
#define   NFP_MU_PCTL_PHY_DXBDLR0_DQ4_WRITE_DELAY(_x)   (((_x) & 0x3f) << 24)
#define   NFP_MU_PCTL_PHY_DXBDLR0_DQ4_WRITE_DELAY_bf    0, 29, 24
#define   NFP_MU_PCTL_PHY_DXBDLR0_DQ4_WRITE_DELAY_mask  (0x3f)
#define   NFP_MU_PCTL_PHY_DXBDLR0_DQ4_WRITE_DELAY_shift (24)
#define   NFP_MU_PCTL_PHY_DXBDLR0_DQ3_WRITE_DELAY(_x)   (((_x) & 0x3f) << 18)
#define   NFP_MU_PCTL_PHY_DXBDLR0_DQ3_WRITE_DELAY_bf    0, 23, 18
#define   NFP_MU_PCTL_PHY_DXBDLR0_DQ3_WRITE_DELAY_mask  (0x3f)
#define   NFP_MU_PCTL_PHY_DXBDLR0_DQ3_WRITE_DELAY_shift (18)
#define   NFP_MU_PCTL_PHY_DXBDLR0_DQ2_WRITE_DELAY(_x)   (((_x) & 0x3f) << 12)
#define   NFP_MU_PCTL_PHY_DXBDLR0_DQ2_WRITE_DELAY_bf    0, 17, 12
#define   NFP_MU_PCTL_PHY_DXBDLR0_DQ2_WRITE_DELAY_mask  (0x3f)
#define   NFP_MU_PCTL_PHY_DXBDLR0_DQ2_WRITE_DELAY_shift (12)
#define   NFP_MU_PCTL_PHY_DXBDLR0_DQ1_WRITE_DELAY(_x)   (((_x) & 0x3f) << 6)
#define   NFP_MU_PCTL_PHY_DXBDLR0_DQ1_WRITE_DELAY_bf    0, 11, 6
#define   NFP_MU_PCTL_PHY_DXBDLR0_DQ1_WRITE_DELAY_mask  (0x3f)
#define   NFP_MU_PCTL_PHY_DXBDLR0_DQ1_WRITE_DELAY_shift (6)
#define   NFP_MU_PCTL_PHY_DXBDLR0_DQ0_WRITE_DELAY(_x)   ((_x) & 0x3f)
#define   NFP_MU_PCTL_PHY_DXBDLR0_DQ0_WRITE_DELAY_bf    0, 5, 0
#define   NFP_MU_PCTL_PHY_DXBDLR0_DQ0_WRITE_DELAY_mask  (0x3f)
#define   NFP_MU_PCTL_PHY_DXBDLR0_DQ0_WRITE_DELAY_shift (0)
/* Register Type: MUPhyDXBDLR1 */
#define NFP_MU_PCTL_PHY_DXBDLR10       0x05d0
#define NFP_MU_PCTL_PHY_DXBDLR11       0x0610
#define NFP_MU_PCTL_PHY_DXBDLR12       0x0650
#define NFP_MU_PCTL_PHY_DXBDLR13       0x0690
#define NFP_MU_PCTL_PHY_DXBDLR14       0x06d0
#define NFP_MU_PCTL_PHY_DXBDLR15       0x0710
#define NFP_MU_PCTL_PHY_DXBDLR16       0x0750
#define NFP_MU_PCTL_PHY_DXBDLR17       0x0790
#define NFP_MU_PCTL_PHY_DXBDLR18       0x07d0
#define   NFP_MU_PCTL_PHY_DXBDLR1_DQS_WRITE_DELAY(_x)   (((_x) & 0x3f) << 24)
#define   NFP_MU_PCTL_PHY_DXBDLR1_DQS_WRITE_DELAY_bf    0, 29, 24
#define   NFP_MU_PCTL_PHY_DXBDLR1_DQS_WRITE_DELAY_mask  (0x3f)
#define   NFP_MU_PCTL_PHY_DXBDLR1_DQS_WRITE_DELAY_shift (24)
#define   NFP_MU_PCTL_PHY_DXBDLR1_DQM_WRITE_DELAY(_x)   (((_x) & 0x3f) << 18)
#define   NFP_MU_PCTL_PHY_DXBDLR1_DQM_WRITE_DELAY_bf    0, 23, 18
#define   NFP_MU_PCTL_PHY_DXBDLR1_DQM_WRITE_DELAY_mask  (0x3f)
#define   NFP_MU_PCTL_PHY_DXBDLR1_DQM_WRITE_DELAY_shift (18)
#define   NFP_MU_PCTL_PHY_DXBDLR1_DQ7_WRITE_DELAY(_x)   (((_x) & 0x3f) << 12)
#define   NFP_MU_PCTL_PHY_DXBDLR1_DQ7_WRITE_DELAY_bf    0, 17, 12
#define   NFP_MU_PCTL_PHY_DXBDLR1_DQ7_WRITE_DELAY_mask  (0x3f)
#define   NFP_MU_PCTL_PHY_DXBDLR1_DQ7_WRITE_DELAY_shift (12)
#define   NFP_MU_PCTL_PHY_DXBDLR1_DQ6_WRITE_DELAY(_x)   (((_x) & 0x3f) << 6)
#define   NFP_MU_PCTL_PHY_DXBDLR1_DQ6_WRITE_DELAY_bf    0, 11, 6
#define   NFP_MU_PCTL_PHY_DXBDLR1_DQ6_WRITE_DELAY_mask  (0x3f)
#define   NFP_MU_PCTL_PHY_DXBDLR1_DQ6_WRITE_DELAY_shift (6)
#define   NFP_MU_PCTL_PHY_DXBDLR1_DQ5_WRITE_DELAY(_x)   ((_x) & 0x3f)
#define   NFP_MU_PCTL_PHY_DXBDLR1_DQ5_WRITE_DELAY_bf    0, 5, 0
#define   NFP_MU_PCTL_PHY_DXBDLR1_DQ5_WRITE_DELAY_mask  (0x3f)
#define   NFP_MU_PCTL_PHY_DXBDLR1_DQ5_WRITE_DELAY_shift (0)
/* Register Type: MUPhyDXBDLR2 */
#define NFP_MU_PCTL_PHY_DXBDLR20       0x05d4
#define NFP_MU_PCTL_PHY_DXBDLR21       0x0614
#define NFP_MU_PCTL_PHY_DXBDLR22       0x0654
#define NFP_MU_PCTL_PHY_DXBDLR23       0x0694
#define NFP_MU_PCTL_PHY_DXBDLR24       0x06d4
#define NFP_MU_PCTL_PHY_DXBDLR25       0x0714
#define NFP_MU_PCTL_PHY_DXBDLR26       0x0754
#define NFP_MU_PCTL_PHY_DXBDLR27       0x0794
#define NFP_MU_PCTL_PHY_DXBDLR28       0x07d4
#define   NFP_MU_PCTL_PHY_DXBDLR2_DQ_OUT_ENABLE_DELAY(_x) (((_x) & 0x3f) << 6)
#define   NFP_MU_PCTL_PHY_DXBDLR2_DQ_OUT_ENABLE_DELAY_bf 0, 11, 6
#define   NFP_MU_PCTL_PHY_DXBDLR2_DQ_OUT_ENABLE_DELAY_mask (0x3f)
#define   NFP_MU_PCTL_PHY_DXBDLR2_DQ_OUT_ENABLE_DELAY_shift (6)
#define   NFP_MU_PCTL_PHY_DXBDLR2_DQS_OUT_ENABLE_DELAY(_x) ((_x) & 0x3f)
#define   NFP_MU_PCTL_PHY_DXBDLR2_DQS_OUT_ENABLE_DELAY_bf 0, 5, 0
#define   NFP_MU_PCTL_PHY_DXBDLR2_DQS_OUT_ENABLE_DELAY_mask (0x3f)
#define   NFP_MU_PCTL_PHY_DXBDLR2_DQS_OUT_ENABLE_DELAY_shift (0)
/* Register Type: MUPhyDXBDLR3 */
#define NFP_MU_PCTL_PHY_DXBDLR30       0x05d8
#define NFP_MU_PCTL_PHY_DXBDLR31       0x0618
#define NFP_MU_PCTL_PHY_DXBDLR32       0x0658
#define NFP_MU_PCTL_PHY_DXBDLR33       0x0698
#define NFP_MU_PCTL_PHY_DXBDLR34       0x06d8
#define NFP_MU_PCTL_PHY_DXBDLR35       0x0718
#define NFP_MU_PCTL_PHY_DXBDLR36       0x0758
#define NFP_MU_PCTL_PHY_DXBDLR37       0x0798
#define NFP_MU_PCTL_PHY_DXBDLR38       0x07d8
#define   NFP_MU_PCTL_PHY_DXBDLR3_DQ4_READ_DELAY(_x)    (((_x) & 0x3f) << 24)
#define   NFP_MU_PCTL_PHY_DXBDLR3_DQ4_READ_DELAY_bf     0, 29, 24
#define   NFP_MU_PCTL_PHY_DXBDLR3_DQ4_READ_DELAY_mask   (0x3f)
#define   NFP_MU_PCTL_PHY_DXBDLR3_DQ4_READ_DELAY_shift  (24)
#define   NFP_MU_PCTL_PHY_DXBDLR3_DQ3_READ_DELAY(_x)    (((_x) & 0x3f) << 18)
#define   NFP_MU_PCTL_PHY_DXBDLR3_DQ3_READ_DELAY_bf     0, 23, 18
#define   NFP_MU_PCTL_PHY_DXBDLR3_DQ3_READ_DELAY_mask   (0x3f)
#define   NFP_MU_PCTL_PHY_DXBDLR3_DQ3_READ_DELAY_shift  (18)
#define   NFP_MU_PCTL_PHY_DXBDLR3_DQ2_READ_DELAY(_x)    (((_x) & 0x3f) << 12)
#define   NFP_MU_PCTL_PHY_DXBDLR3_DQ2_READ_DELAY_bf     0, 17, 12
#define   NFP_MU_PCTL_PHY_DXBDLR3_DQ2_READ_DELAY_mask   (0x3f)
#define   NFP_MU_PCTL_PHY_DXBDLR3_DQ2_READ_DELAY_shift  (12)
#define   NFP_MU_PCTL_PHY_DXBDLR3_DQ1_READ_DELAY(_x)    (((_x) & 0x3f) << 6)
#define   NFP_MU_PCTL_PHY_DXBDLR3_DQ1_READ_DELAY_bf     0, 11, 6
#define   NFP_MU_PCTL_PHY_DXBDLR3_DQ1_READ_DELAY_mask   (0x3f)
#define   NFP_MU_PCTL_PHY_DXBDLR3_DQ1_READ_DELAY_shift  (6)
#define   NFP_MU_PCTL_PHY_DXBDLR3_DQ0_READ_DELAY(_x)    ((_x) & 0x3f)
#define   NFP_MU_PCTL_PHY_DXBDLR3_DQ0_READ_DELAY_bf     0, 5, 0
#define   NFP_MU_PCTL_PHY_DXBDLR3_DQ0_READ_DELAY_mask   (0x3f)
#define   NFP_MU_PCTL_PHY_DXBDLR3_DQ0_READ_DELAY_shift  (0)
/* Register Type: MUPhyDXBDLR4 */
#define NFP_MU_PCTL_PHY_DXBDLR40       0x05dc
#define NFP_MU_PCTL_PHY_DXBDLR41       0x061c
#define NFP_MU_PCTL_PHY_DXBDLR42       0x065c
#define NFP_MU_PCTL_PHY_DXBDLR43       0x069c
#define NFP_MU_PCTL_PHY_DXBDLR44       0x06dc
#define NFP_MU_PCTL_PHY_DXBDLR45       0x071c
#define NFP_MU_PCTL_PHY_DXBDLR46       0x075c
#define NFP_MU_PCTL_PHY_DXBDLR47       0x079c
#define NFP_MU_PCTL_PHY_DXBDLR48       0x07dc
#define   NFP_MU_PCTL_PHY_DXBDLR4_DQS_READ_DELAY(_x)    (((_x) & 0x3f) << 24)
#define   NFP_MU_PCTL_PHY_DXBDLR4_DQS_READ_DELAY_bf     0, 29, 24
#define   NFP_MU_PCTL_PHY_DXBDLR4_DQS_READ_DELAY_mask   (0x3f)
#define   NFP_MU_PCTL_PHY_DXBDLR4_DQS_READ_DELAY_shift  (24)
#define   NFP_MU_PCTL_PHY_DXBDLR4_DQM_READ_DELAY(_x)    (((_x) & 0x3f) << 18)
#define   NFP_MU_PCTL_PHY_DXBDLR4_DQM_READ_DELAY_bf     0, 23, 18
#define   NFP_MU_PCTL_PHY_DXBDLR4_DQM_READ_DELAY_mask   (0x3f)
#define   NFP_MU_PCTL_PHY_DXBDLR4_DQM_READ_DELAY_shift  (18)
#define   NFP_MU_PCTL_PHY_DXBDLR4_DQ7_READ_DELAY(_x)    (((_x) & 0x3f) << 12)
#define   NFP_MU_PCTL_PHY_DXBDLR4_DQ7_READ_DELAY_bf     0, 17, 12
#define   NFP_MU_PCTL_PHY_DXBDLR4_DQ7_READ_DELAY_mask   (0x3f)
#define   NFP_MU_PCTL_PHY_DXBDLR4_DQ7_READ_DELAY_shift  (12)
#define   NFP_MU_PCTL_PHY_DXBDLR4_DQ6_READ_DELAY(_x)    (((_x) & 0x3f) << 6)
#define   NFP_MU_PCTL_PHY_DXBDLR4_DQ6_READ_DELAY_bf     0, 11, 6
#define   NFP_MU_PCTL_PHY_DXBDLR4_DQ6_READ_DELAY_mask   (0x3f)
#define   NFP_MU_PCTL_PHY_DXBDLR4_DQ6_READ_DELAY_shift  (6)
#define   NFP_MU_PCTL_PHY_DXBDLR4_DQ5_READ_DELAY(_x)    ((_x) & 0x3f)
#define   NFP_MU_PCTL_PHY_DXBDLR4_DQ5_READ_DELAY_bf     0, 5, 0
#define   NFP_MU_PCTL_PHY_DXBDLR4_DQ5_READ_DELAY_mask   (0x3f)
#define   NFP_MU_PCTL_PHY_DXBDLR4_DQ5_READ_DELAY_shift  (0)
/* Register Type: MUPhyDXLCDLR0 */
#define NFP_MU_PCTL_PHY_DXLCDLR0_0     0x05e0
#define NFP_MU_PCTL_PHY_DXLCDLR0_1     0x0620
#define NFP_MU_PCTL_PHY_DXLCDLR0_2     0x0660
#define NFP_MU_PCTL_PHY_DXLCDLR0_3     0x06a0
#define NFP_MU_PCTL_PHY_DXLCDLR0_4     0x06e0
#define NFP_MU_PCTL_PHY_DXLCDLR0_5     0x0720
#define NFP_MU_PCTL_PHY_DXLCDLR0_6     0x0760
#define NFP_MU_PCTL_PHY_DXLCDLR0_7     0x07a0
#define NFP_MU_PCTL_PHY_DXLCDLR0_8     0x07e0
#define   NFP_MU_PCTL_PHY_DXLCDLR0__RANK3_WRITE_DELAY(_x) (((_x) & 0xff) << 24)
#define   NFP_MU_PCTL_PHY_DXLCDLR0__RANK3_WRITE_DELAY_bf 0, 31, 24
#define   NFP_MU_PCTL_PHY_DXLCDLR0__RANK3_WRITE_DELAY_mask (0xff)
#define   NFP_MU_PCTL_PHY_DXLCDLR0__RANK3_WRITE_DELAY_shift (24)
#define   NFP_MU_PCTL_PHY_DXLCDLR0__RANK2_WRITE_DELAY(_x) (((_x) & 0xff) << 16)
#define   NFP_MU_PCTL_PHY_DXLCDLR0__RANK2_WRITE_DELAY_bf 0, 23, 16
#define   NFP_MU_PCTL_PHY_DXLCDLR0__RANK2_WRITE_DELAY_mask (0xff)
#define   NFP_MU_PCTL_PHY_DXLCDLR0__RANK2_WRITE_DELAY_shift (16)
#define   NFP_MU_PCTL_PHY_DXLCDLR0__RANK1_WRITE_DELAY(_x) (((_x) & 0xff) << 8)
#define   NFP_MU_PCTL_PHY_DXLCDLR0__RANK1_WRITE_DELAY_bf 0, 15, 8
#define   NFP_MU_PCTL_PHY_DXLCDLR0__RANK1_WRITE_DELAY_mask (0xff)
#define   NFP_MU_PCTL_PHY_DXLCDLR0__RANK1_WRITE_DELAY_shift (8)
#define   NFP_MU_PCTL_PHY_DXLCDLR0__RANK0_WRITE_DELAY(_x) ((_x) & 0xff)
#define   NFP_MU_PCTL_PHY_DXLCDLR0__RANK0_WRITE_DELAY_bf 0, 7, 0
#define   NFP_MU_PCTL_PHY_DXLCDLR0__RANK0_WRITE_DELAY_mask (0xff)
#define   NFP_MU_PCTL_PHY_DXLCDLR0__RANK0_WRITE_DELAY_shift (0)
/* Register Type: MUPhyDXLCDLR1 */
#define NFP_MU_PCTL_PHY_DXLCDLR1_0     0x05e4
#define NFP_MU_PCTL_PHY_DXLCDLR1_1     0x0624
#define NFP_MU_PCTL_PHY_DXLCDLR1_2     0x0664
#define NFP_MU_PCTL_PHY_DXLCDLR1_3     0x06a4
#define NFP_MU_PCTL_PHY_DXLCDLR1_4     0x06e4
#define NFP_MU_PCTL_PHY_DXLCDLR1_5     0x0724
#define NFP_MU_PCTL_PHY_DXLCDLR1_6     0x0764
#define NFP_MU_PCTL_PHY_DXLCDLR1_7     0x07a4
#define NFP_MU_PCTL_PHY_DXLCDLR1_8     0x07e4
#define   NFP_MU_PCTL_PHY_DXLCDLR1__READ_DQS_DELAY(_x)  (((_x) & 0xff) << 8)
#define   NFP_MU_PCTL_PHY_DXLCDLR1__READ_DQS_DELAY_bf   0, 15, 8
#define   NFP_MU_PCTL_PHY_DXLCDLR1__READ_DQS_DELAY_mask (0xff)
#define   NFP_MU_PCTL_PHY_DXLCDLR1__READ_DQS_DELAY_shift (8)
#define   NFP_MU_PCTL_PHY_DXLCDLR1__WRITE_DELAY(_x)     ((_x) & 0xff)
#define   NFP_MU_PCTL_PHY_DXLCDLR1__WRITE_DELAY_bf      0, 7, 0
#define   NFP_MU_PCTL_PHY_DXLCDLR1__WRITE_DELAY_mask    (0xff)
#define   NFP_MU_PCTL_PHY_DXLCDLR1__WRITE_DELAY_shift   (0)
/* Register Type: MUPhyDXLCDLR2 */
#define NFP_MU_PCTL_PHY_DXLCDLR2_0     0x05e8
#define NFP_MU_PCTL_PHY_DXLCDLR2_1     0x0628
#define NFP_MU_PCTL_PHY_DXLCDLR2_2     0x0668
#define NFP_MU_PCTL_PHY_DXLCDLR2_3     0x06a8
#define NFP_MU_PCTL_PHY_DXLCDLR2_4     0x06e8
#define NFP_MU_PCTL_PHY_DXLCDLR2_5     0x0728
#define NFP_MU_PCTL_PHY_DXLCDLR2_6     0x0768
#define NFP_MU_PCTL_PHY_DXLCDLR2_7     0x07a8
#define NFP_MU_PCTL_PHY_DXLCDLR2_8     0x07e8
#define   NFP_MU_PCTL_PHY_DXLCDLR2__RNK3_DQS_GATING_DELAY(_x) (((_x) & 0xff) << 24)
#define   NFP_MU_PCTL_PHY_DXLCDLR2__RNK3_DQS_GATING_DELAY_bf 0, 31, 24
#define   NFP_MU_PCTL_PHY_DXLCDLR2__RNK3_DQS_GATING_DELAY_mask (0xff)
#define   NFP_MU_PCTL_PHY_DXLCDLR2__RNK3_DQS_GATING_DELAY_shift (24)
#define   NFP_MU_PCTL_PHY_DXLCDLR2__RANK2_DQS_GATING_DELAY(_x) (((_x) & 0xff) << 16)
#define   NFP_MU_PCTL_PHY_DXLCDLR2__RANK2_DQS_GATING_DELAY_bf 0, 23, 16
#define   NFP_MU_PCTL_PHY_DXLCDLR2__RANK2_DQS_GATING_DELAY_mask (0xff)
#define   NFP_MU_PCTL_PHY_DXLCDLR2__RANK2_DQS_GATING_DELAY_shift (16)
#define   NFP_MU_PCTL_PHY_DXLCDLR2__RANK1_DQS_GATING_DELAY(_x) (((_x) & 0xff) << 8)
#define   NFP_MU_PCTL_PHY_DXLCDLR2__RANK1_DQS_GATING_DELAY_bf 0, 15, 8
#define   NFP_MU_PCTL_PHY_DXLCDLR2__RANK1_DQS_GATING_DELAY_mask (0xff)
#define   NFP_MU_PCTL_PHY_DXLCDLR2__RANK1_DQS_GATING_DELAY_shift (8)
#define   NFP_MU_PCTL_PHY_DXLCDLR2__RANK0_DQS_GATING_DELAY(_x) ((_x) & 0xff)
#define   NFP_MU_PCTL_PHY_DXLCDLR2__RANK0_DQS_GATING_DELAY_bf 0, 7, 0
#define   NFP_MU_PCTL_PHY_DXLCDLR2__RANK0_DQS_GATING_DELAY_mask (0xff)
#define   NFP_MU_PCTL_PHY_DXLCDLR2__RANK0_DQS_GATING_DELAY_shift (0)
/* Register Type: MUPhyDXMDLR */
#define NFP_MU_PCTL_PHY_DXMDLR0        0x05ec
#define NFP_MU_PCTL_PHY_DXMDLR1        0x062c
#define NFP_MU_PCTL_PHY_DXMDLR2        0x066c
#define NFP_MU_PCTL_PHY_DXMDLR3        0x06ac
#define NFP_MU_PCTL_PHY_DXMDLR4        0x06ec
#define NFP_MU_PCTL_PHY_DXMDLR5        0x072c
#define NFP_MU_PCTL_PHY_DXMDLR6        0x076c
#define NFP_MU_PCTL_PHY_DXMDLR7        0x07ac
#define NFP_MU_PCTL_PHY_DXMDLR8        0x07ec
#define   NFP_MU_PCTL_PHY_DXMDLR_FIXME1(_x)             (((_x) & 0xff) << 16)
#define   NFP_MU_PCTL_PHY_DXMDLR_FIXME1_bf              0, 23, 16
#define   NFP_MU_PCTL_PHY_DXMDLR_FIXME1_mask            (0xff)
#define   NFP_MU_PCTL_PHY_DXMDLR_FIXME1_shift           (16)
#define   NFP_MU_PCTL_PHY_DXMDLR_FIXME2(_x)             (((_x) & 0xff) << 8)
#define   NFP_MU_PCTL_PHY_DXMDLR_FIXME2_bf              0, 15, 8
#define   NFP_MU_PCTL_PHY_DXMDLR_FIXME2_mask            (0xff)
#define   NFP_MU_PCTL_PHY_DXMDLR_FIXME2_shift           (8)
#define   NFP_MU_PCTL_PHY_DXMDLR_FIXME3(_x)             ((_x) & 0xff)
#define   NFP_MU_PCTL_PHY_DXMDLR_FIXME3_bf              0, 7, 0
#define   NFP_MU_PCTL_PHY_DXMDLR_FIXME3_mask            (0xff)
#define   NFP_MU_PCTL_PHY_DXMDLR_FIXME3_shift           (0)

/* HGID: nfp3200/mu_pub.desc = 5702fafbfb92 */
/* Register Type: MUPubMisc */
#define NFP_MU_PCTL_PUB_MISC           0x0480
#define   NFP_MU_PCTL_PUB_MISC_FIXME1                   (0x1 << 2)
#define   NFP_MU_PCTL_PUB_MISC_FIXME1_bf                0, 2, 2
#define   NFP_MU_PCTL_PUB_MISC_FIXME1_mask              (0x1)
#define   NFP_MU_PCTL_PUB_MISC_FIXME1_bit               (2)
#define   NFP_MU_PCTL_PUB_MISC_RESET                    (0x1 << 1)
#define   NFP_MU_PCTL_PUB_MISC_RESET_bf                 0, 1, 1
#define   NFP_MU_PCTL_PUB_MISC_RESET_mask               (0x1)
#define   NFP_MU_PCTL_PUB_MISC_RESET_bit                (1)
#define   NFP_MU_PCTL_PUB_MISC_TR_EN                    (0x1)
#define   NFP_MU_PCTL_PUB_MISC_TR_EN_bf                 0, 0, 0
#define   NFP_MU_PCTL_PUB_MISC_TR_EN_mask               (0x1)
#define   NFP_MU_PCTL_PUB_MISC_TR_EN_bit                (0)
/* Register Type: MUPubSMCtl */
#define NFP_MU_PCTL_PUB_SMCTL          0x0484
#define   NFP_MU_PCTL_PUB_SMCTL_MSCMP(_x)               (((_x) & 0xffff) << 16)
#define   NFP_MU_PCTL_PUB_SMCTL_MSCMP_bf                0, 31, 16
#define   NFP_MU_PCTL_PUB_SMCTL_MSCMP_mask              (0xffff)
#define   NFP_MU_PCTL_PUB_SMCTL_MSCMP_shift             (16)
#define   NFP_MU_PCTL_PUB_SMCTL_MSWAIT(_x)              ((_x) & 0xffff)
#define   NFP_MU_PCTL_PUB_SMCTL_MSWAIT_bf               0, 15, 0
#define   NFP_MU_PCTL_PUB_SMCTL_MSWAIT_mask             (0xffff)
#define   NFP_MU_PCTL_PUB_SMCTL_MSWAIT_shift            (0)
/* Register Type: MUPubSMStat */
#define NFP_MU_PCTL_PUB_SMSTAT         0x0488
#define   NFP_MU_PCTL_PUB_SMSTAT_MSE_bf                 0, 31, 16
#define   NFP_MU_PCTL_PUB_SMSTAT_MSE_mask               (0xffff)
#define   NFP_MU_PCTL_PUB_SMSTAT_MSE_shift              (16)
#define   NFP_MU_PCTL_PUB_SMSTAT_MSS_bf                 0, 15, 0
#define   NFP_MU_PCTL_PUB_SMSTAT_MSS_mask               (0xffff)
#define   NFP_MU_PCTL_PUB_SMSTAT_MSS_shift              (0)
/* Register Type: MUPubMR1 */
#define NFP_MU_PCTL_PUB_MR1            0x048c
#define   NFP_MU_PCTL_PUB_MR1_MR1(_x)                   ((_x) & 0xffff)
#define   NFP_MU_PCTL_PUB_MR1_MR1_bf                    0, 15, 0
#define   NFP_MU_PCTL_PUB_MR1_MR1_mask                  (0xffff)
#define   NFP_MU_PCTL_PUB_MR1_MR1_shift                 (0)
/* Register Type: MUPubTWLMRD */
#define NFP_MU_PCTL_PUB_TWLMRD         0x0490
#define   NFP_MU_PCTL_PUB_TWLMRD_TWLMRD(_x)             ((_x) & 0x3f)
#define   NFP_MU_PCTL_PUB_TWLMRD_TWLMRD_bf              0, 5, 0
#define   NFP_MU_PCTL_PUB_TWLMRD_TWLMRD_mask            (0x3f)
#define   NFP_MU_PCTL_PUB_TWLMRD_TWLMRD_shift           (0)
/* Register Type: MUPubTR_ADDR */
#define NFP_MU_PCTL_PUB_TR_ADDR0       0x0494
#define NFP_MU_PCTL_PUB_TR_ADDR1       0x0498
#define NFP_MU_PCTL_PUB_TR_ADDR2       0x049c
#define NFP_MU_PCTL_PUB_TR_ADDR3       0x04a0
#define   NFP_MU_PCTL_PUB_TR_ADDR_ROW_ADDR(_x)          (((_x) & 0x1ffff) << 13)
#define   NFP_MU_PCTL_PUB_TR_ADDR_ROW_ADDR_bf           0, 29, 13
#define   NFP_MU_PCTL_PUB_TR_ADDR_ROW_ADDR_mask         (0x1ffff)
#define   NFP_MU_PCTL_PUB_TR_ADDR_ROW_ADDR_shift        (13)
#define   NFP_MU_PCTL_PUB_TR_ADDR_BANK_ADDR(_x)         (((_x) & 0x7) << 10)
#define   NFP_MU_PCTL_PUB_TR_ADDR_BANK_ADDR_bf          0, 12, 10
#define   NFP_MU_PCTL_PUB_TR_ADDR_BANK_ADDR_mask        (0x7)
#define   NFP_MU_PCTL_PUB_TR_ADDR_BANK_ADDR_shift       (10)
#define   NFP_MU_PCTL_PUB_TR_ADDR_COL_ADDR(_x)          ((_x) & 0x3ff)
#define   NFP_MU_PCTL_PUB_TR_ADDR_COL_ADDR_bf           0, 9, 0
#define   NFP_MU_PCTL_PUB_TR_ADDR_COL_ADDR_mask         (0x3ff)
#define   NFP_MU_PCTL_PUB_TR_ADDR_COL_ADDR_shift        (0)
/* Register Type: MUPubEYE_TR_LCDL */
#define NFP_MU_PCTL_PUB_EYE_TR_LCDL    0x04a4
#define   NFP_MU_PCTL_PUB_EYE_TR_LCDL_INCR_DLY(_x)      (((_x) & 0xff) << 8)
#define   NFP_MU_PCTL_PUB_EYE_TR_LCDL_INCR_DLY_bf       0, 15, 8
#define   NFP_MU_PCTL_PUB_EYE_TR_LCDL_INCR_DLY_mask     (0xff)
#define   NFP_MU_PCTL_PUB_EYE_TR_LCDL_INCR_DLY_shift    (8)
#define   NFP_MU_PCTL_PUB_EYE_TR_LCDL_QRTR_DLY(_x)      ((_x) & 0xff)
#define   NFP_MU_PCTL_PUB_EYE_TR_LCDL_QRTR_DLY_bf       0, 7, 0
#define   NFP_MU_PCTL_PUB_EYE_TR_LCDL_QRTR_DLY_mask     (0xff)
#define   NFP_MU_PCTL_PUB_EYE_TR_LCDL_QRTR_DLY_shift    (0)
/* Register Type: MUPubPPMCFG */
#define NFP_MU_PCTL_PUB_PPMCFG         0x04a8
#define   NFP_MU_PCTL_PUB_PPMCFG_FIXME2(_x)             (((_x) & 0xf) << 9)
#define   NFP_MU_PCTL_PUB_PPMCFG_FIXME2_bf              0, 12, 9
#define   NFP_MU_PCTL_PUB_PPMCFG_FIXME2_mask            (0xf)
#define   NFP_MU_PCTL_PUB_PPMCFG_FIXME2_shift           (9)
#define     NFP_MU_PCTL_PUB_PPMCFG_FIXME2_FIXME9        (14)
#define     NFP_MU_PCTL_PUB_PPMCFG_FIXME2_FIXME10       (12)
#define     NFP_MU_PCTL_PUB_PPMCFG_FIXME2_FIXME11       (8)
#define     NFP_MU_PCTL_PUB_PPMCFG_FIXME2_FIXME12       (0)
#define   NFP_MU_PCTL_PUB_PPMCFG_FIXME5(_x)             (((_x) & 0xff) << 1)
#define   NFP_MU_PCTL_PUB_PPMCFG_FIXME5_bf              0, 8, 1
#define   NFP_MU_PCTL_PUB_PPMCFG_FIXME5_mask            (0xff)
#define   NFP_MU_PCTL_PUB_PPMCFG_FIXME5_shift           (1)
#define   NFP_MU_PCTL_PUB_PPMCFG_FIXME8                 (0x1)
#define   NFP_MU_PCTL_PUB_PPMCFG_FIXME8_bf              0, 0, 0
#define   NFP_MU_PCTL_PUB_PPMCFG_FIXME8_mask            (0x1)
#define   NFP_MU_PCTL_PUB_PPMCFG_FIXME8_bit             (0)
#define     NFP_MU_PCTL_PUB_PPMCFG_FIXME8_FIXME3        (0x0)
#define     NFP_MU_PCTL_PUB_PPMCFG_FIXME8_FIXME4        (0x1)
/* Register Type: MUPubSLCR */
#define NFP_MU_PCTL_PUB_FIXME0         0x04ac
#define   NFP_MU_PCTL_PUB_FIXME0_FIXME13(_x)            (((_x) & 0xf) << 20)
#define   NFP_MU_PCTL_PUB_FIXME0_FIXME13_bf             0, 23, 20
#define   NFP_MU_PCTL_PUB_FIXME0_FIXME13_mask           (0xf)
#define   NFP_MU_PCTL_PUB_FIXME0_FIXME13_shift          (20)
#define   NFP_MU_PCTL_PUB_FIXME0_FIXME14(_x)            (((_x) & 0x3) << 16)
#define   NFP_MU_PCTL_PUB_FIXME0_FIXME14_bf             0, 17, 16
#define   NFP_MU_PCTL_PUB_FIXME0_FIXME14_mask           (0x3)
#define   NFP_MU_PCTL_PUB_FIXME0_FIXME14_shift          (16)
#define   NFP_MU_PCTL_PUB_FIXME0_FIXME15(_x)            (((_x) & 0xf) << 12)
#define   NFP_MU_PCTL_PUB_FIXME0_FIXME15_bf             0, 15, 12
#define   NFP_MU_PCTL_PUB_FIXME0_FIXME15_mask           (0xf)
#define   NFP_MU_PCTL_PUB_FIXME0_FIXME15_shift          (12)
#define   NFP_MU_PCTL_PUB_FIXME0_FIXME16(_x)            (((_x) & 0x3) << 8)
#define   NFP_MU_PCTL_PUB_FIXME0_FIXME16_bf             0, 9, 8
#define   NFP_MU_PCTL_PUB_FIXME0_FIXME16_mask           (0x3)
#define   NFP_MU_PCTL_PUB_FIXME0_FIXME16_shift          (8)
#define   NFP_MU_PCTL_PUB_FIXME0_FIXME17(_x)            (((_x) & 0xf) << 4)
#define   NFP_MU_PCTL_PUB_FIXME0_FIXME17_bf             0, 7, 4
#define   NFP_MU_PCTL_PUB_FIXME0_FIXME17_mask           (0xf)
#define   NFP_MU_PCTL_PUB_FIXME0_FIXME17_shift          (4)
#define   NFP_MU_PCTL_PUB_FIXME0_FIXME18(_x)            ((_x) & 0x3)
#define   NFP_MU_PCTL_PUB_FIXME0_FIXME18_bf             0, 1, 0
#define   NFP_MU_PCTL_PUB_FIXME0_FIXME18_mask           (0x3)
#define   NFP_MU_PCTL_PUB_FIXME0_FIXME18_shift          (0)
/* Register Type: MUPubBSTCTL */
#define NFP_MU_PCTL_PUB_BSTCTL         0x04c0
#define   NFP_MU_PCTL_PUB_BSTCTL_BIST_DATA_LOOPBACK_SEL(_x) (((_x) & 0xf) << 10)
#define   NFP_MU_PCTL_PUB_BSTCTL_BIST_DATA_LOOPBACK_SEL_bf 0, 13, 10
#define   NFP_MU_PCTL_PUB_BSTCTL_BIST_DATA_LOOPBACK_SEL_mask (0xf)
#define   NFP_MU_PCTL_PUB_BSTCTL_BIST_DATA_LOOPBACK_SEL_shift (10)
#define   NFP_MU_PCTL_PUB_BSTCTL_BIST_RUN               (0x1 << 9)
#define   NFP_MU_PCTL_PUB_BSTCTL_BIST_RUN_bf            0, 9, 9
#define   NFP_MU_PCTL_PUB_BSTCTL_BIST_RUN_mask          (0x1)
#define   NFP_MU_PCTL_PUB_BSTCTL_BIST_RUN_bit           (9)
#define   NFP_MU_PCTL_PUB_BSTCTL_CLR_ERROR_COUNTERS     (0x1 << 8)
#define   NFP_MU_PCTL_PUB_BSTCTL_CLR_ERROR_COUNTERS_bf  0, 8, 8
#define   NFP_MU_PCTL_PUB_BSTCTL_CLR_ERROR_COUNTERS_mask (0x1)
#define   NFP_MU_PCTL_PUB_BSTCTL_CLR_ERROR_COUNTERS_bit (8)
#define   NFP_MU_PCTL_PUB_BSTCTL_WALKING_BIT_SEL        (0x1 << 7)
#define   NFP_MU_PCTL_PUB_BSTCTL_WALKING_BIT_SEL_bf     0, 7, 7
#define   NFP_MU_PCTL_PUB_BSTCTL_WALKING_BIT_SEL_mask   (0x1)
#define   NFP_MU_PCTL_PUB_BSTCTL_WALKING_BIT_SEL_bit    (7)
#define   NFP_MU_PCTL_PUB_BSTCTL_FIXME19                (0x1 << 6)
#define   NFP_MU_PCTL_PUB_BSTCTL_FIXME19_bf             0, 6, 6
#define   NFP_MU_PCTL_PUB_BSTCTL_FIXME19_mask           (0x1)
#define   NFP_MU_PCTL_PUB_BSTCTL_FIXME19_bit            (6)
#define   NFP_MU_PCTL_PUB_BSTCTL_FIXME20                (0x1 << 5)
#define   NFP_MU_PCTL_PUB_BSTCTL_FIXME20_bf             0, 5, 5
#define   NFP_MU_PCTL_PUB_BSTCTL_FIXME20_mask           (0x1)
#define   NFP_MU_PCTL_PUB_BSTCTL_FIXME20_bit            (5)
#define   NFP_MU_PCTL_PUB_BSTCTL_WORD_LINE_A_SEL        (0x1 << 4)
#define   NFP_MU_PCTL_PUB_BSTCTL_WORD_LINE_A_SEL_bf     0, 4, 4
#define   NFP_MU_PCTL_PUB_BSTCTL_WORD_LINE_A_SEL_mask   (0x1)
#define   NFP_MU_PCTL_PUB_BSTCTL_WORD_LINE_A_SEL_bit    (4)
#define   NFP_MU_PCTL_PUB_BSTCTL_WORD_LINE_B_SEL        (0x1 << 3)
#define   NFP_MU_PCTL_PUB_BSTCTL_WORD_LINE_B_SEL_bf     0, 3, 3
#define   NFP_MU_PCTL_PUB_BSTCTL_WORD_LINE_B_SEL_mask   (0x1)
#define   NFP_MU_PCTL_PUB_BSTCTL_WORD_LINE_B_SEL_bit    (3)
#define   NFP_MU_PCTL_PUB_BSTCTL_WORD_LINE_DATA_MASK_SEL (0x1 << 2)
#define   NFP_MU_PCTL_PUB_BSTCTL_WORD_LINE_DATA_MASK_SEL_bf 0, 2, 2
#define   NFP_MU_PCTL_PUB_BSTCTL_WORD_LINE_DATA_MASK_SEL_mask (0x1)
#define   NFP_MU_PCTL_PUB_BSTCTL_WORD_LINE_DATA_MASK_SEL_bit (2)
#define   NFP_MU_PCTL_PUB_BSTCTL_BIST_MODE              (0x1 << 1)
#define   NFP_MU_PCTL_PUB_BSTCTL_BIST_MODE_bf           0, 1, 1
#define   NFP_MU_PCTL_PUB_BSTCTL_BIST_MODE_mask         (0x1)
#define   NFP_MU_PCTL_PUB_BSTCTL_BIST_MODE_bit          (1)
#define   NFP_MU_PCTL_PUB_BSTCTL_BIST_EN                (0x1)
#define   NFP_MU_PCTL_PUB_BSTCTL_BIST_EN_bf             0, 0, 0
#define   NFP_MU_PCTL_PUB_BSTCTL_BIST_EN_mask           (0x1)
#define   NFP_MU_PCTL_PUB_BSTCTL_BIST_EN_bit            (0)
/* Register Type: MUPubBSTSTAT */
#define NFP_MU_PCTL_PUB_BSTSTAT        0x04c4
#define   NFP_MU_PCTL_PUB_BSTSTAT_ERROR_DATA_MASK       (0x1 << 5)
#define   NFP_MU_PCTL_PUB_BSTSTAT_ERROR_DATA_MASK_bf    0, 5, 5
#define   NFP_MU_PCTL_PUB_BSTSTAT_ERROR_DATA_MASK_mask  (0x1)
#define   NFP_MU_PCTL_PUB_BSTSTAT_ERROR_DATA_MASK_bit   (5)
#define   NFP_MU_PCTL_PUB_BSTSTAT_ERROR_DATA_CMP        (0x1 << 4)
#define   NFP_MU_PCTL_PUB_BSTSTAT_ERROR_DATA_CMP_bf     0, 4, 4
#define   NFP_MU_PCTL_PUB_BSTSTAT_ERROR_DATA_CMP_mask   (0x1)
#define   NFP_MU_PCTL_PUB_BSTSTAT_ERROR_DATA_CMP_bit    (4)
#define   NFP_MU_PCTL_PUB_BSTSTAT_ERROR_ADDR_CMP        (0x1 << 3)
#define   NFP_MU_PCTL_PUB_BSTSTAT_ERROR_ADDR_CMP_bf     0, 3, 3
#define   NFP_MU_PCTL_PUB_BSTSTAT_ERROR_ADDR_CMP_mask   (0x1)
#define   NFP_MU_PCTL_PUB_BSTSTAT_ERROR_ADDR_CMP_bit    (3)
#define   NFP_MU_PCTL_PUB_BSTSTAT_BIST_RUNNING          (0x1)
#define   NFP_MU_PCTL_PUB_BSTSTAT_BIST_RUNNING_bf       0, 0, 0
#define   NFP_MU_PCTL_PUB_BSTSTAT_BIST_RUNNING_mask     (0x1)
#define   NFP_MU_PCTL_PUB_BSTSTAT_BIST_RUNNING_bit      (0)
/* Register Type: MUPubBSTCNT */
#define NFP_MU_PCTL_PUB_BSTCNT         0x04c8
#define   NFP_MU_PCTL_PUB_BSTCNT_COUNT(_x)              ((_x) & 0xffff)
#define   NFP_MU_PCTL_PUB_BSTCNT_COUNT_bf               0, 15, 0
#define   NFP_MU_PCTL_PUB_BSTCNT_COUNT_mask             (0xffff)
#define   NFP_MU_PCTL_PUB_BSTCNT_COUNT_shift            (0)
/* Register Type: MUPubBSTWRD */
#define NFP_MU_PCTL_PUB_BSTWRD         0x04cc
#define   NFP_MU_PCTL_PUB_BSTWRD_LINE_B(_x)             (((_x) & 0xffff) << 16)
#define   NFP_MU_PCTL_PUB_BSTWRD_LINE_B_bf              0, 31, 16
#define   NFP_MU_PCTL_PUB_BSTWRD_LINE_B_mask            (0xffff)
#define   NFP_MU_PCTL_PUB_BSTWRD_LINE_B_shift           (16)
#define   NFP_MU_PCTL_PUB_BSTWRD_LINE_A(_x)             ((_x) & 0xffff)
#define   NFP_MU_PCTL_PUB_BSTWRD_LINE_A_bf              0, 15, 0
#define   NFP_MU_PCTL_PUB_BSTWRD_LINE_A_mask            (0xffff)
#define   NFP_MU_PCTL_PUB_BSTWRD_LINE_A_shift           (0)
/* Register Type: MUPubBSTMSK */
#define NFP_MU_PCTL_PUB_BSTMSK         0x04d0
#define   NFP_MU_PCTL_PUB_BSTMSK_MASK(_x)               ((_x) & 0xffff)
#define   NFP_MU_PCTL_PUB_BSTMSK_MASK_bf                0, 15, 0
#define   NFP_MU_PCTL_PUB_BSTMSK_MASK_mask              (0xffff)
#define   NFP_MU_PCTL_PUB_BSTMSK_MASK_shift             (0)
/* Register Type: MUPubBSTSEED */
#define NFP_MU_PCTL_PUB_BSTSEED        0x04d4
#define   NFP_MU_PCTL_PUB_BSTSEED_FIXME21(_x)           (_x)
#define   NFP_MU_PCTL_PUB_BSTSEED_FIXME21_bf            0, 31, 0
#define   NFP_MU_PCTL_PUB_BSTSEED_FIXME21_mask          (0xffffffff)
#define   NFP_MU_PCTL_PUB_BSTSEED_FIXME21_shift         (0)
/* Register Type: MUPubBSTDWEC */
#define NFP_MU_PCTL_PUB_BSTDWEC        0x04d8
#define   NFP_MU_PCTL_PUB_BSTDWEC_MASK_COUNT_bf         0, 31, 16
#define   NFP_MU_PCTL_PUB_BSTDWEC_MASK_COUNT_mask       (0xffff)
#define   NFP_MU_PCTL_PUB_BSTDWEC_MASK_COUNT_shift      (16)
#define   NFP_MU_PCTL_PUB_BSTDWEC_DATA_COUNT_bf         0, 15, 0
#define   NFP_MU_PCTL_PUB_BSTDWEC_DATA_COUNT_mask       (0xffff)
#define   NFP_MU_PCTL_PUB_BSTDWEC_DATA_COUNT_shift      (0)
/* Register Type: MUPubBSTAWEC */
#define NFP_MU_PCTL_PUB_BSTAWEC        0x04dc
#define   NFP_MU_PCTL_PUB_BSTAWEC_ADDR_bf               0, 15, 0
#define   NFP_MU_PCTL_PUB_BSTAWEC_ADDR_mask             (0xffff)
#define   NFP_MU_PCTL_PUB_BSTAWEC_ADDR_shift            (0)
/* Register Type: MUPubIPVR */
#define NFP_MU_PCTL_PUB_PUB_IPVR       0x04f8
#define   NFP_MU_PCTL_PUB_PUB_IPVR_PUB_IP_VERSION_bf    0, 31, 0
#define   NFP_MU_PCTL_PUB_PUB_IPVR_PUB_IP_VERSION_mask  (0xffffffff)
#define   NFP_MU_PCTL_PUB_PUB_IPVR_PUB_IP_VERSION_shift (0)
/* Register Type: MUPubIPTR */
#define NFP_MU_PCTL_PUB_PUB_IPTR       0x04fc
#define   NFP_MU_PCTL_PUB_PUB_IPTR_PUB_IP_TYPE_bf       0, 31, 0
#define   NFP_MU_PCTL_PUB_PUB_IPTR_PUB_IP_TYPE_mask     (0xffffffff)
#define   NFP_MU_PCTL_PUB_PUB_IPTR_PUB_IP_TYPE_shift    (0)

/* HGID: nfp3200/mu_tcache_bank.desc = 5702fafbfb92 */
/* Register Type: MUTCacheConfig */
#define NFP_MU_TCACHE_CONF             0x0000
#define   NFP_MU_TCACHE_CONF_REALLY_DIRTY_WM(_x)        (((_x) & 0xf) << 28)
#define   NFP_MU_TCACHE_CONF_REALLY_DIRTY_WM_bf         0, 31, 28
#define   NFP_MU_TCACHE_CONF_REALLY_DIRTY_WM_mask       (0xf)
#define   NFP_MU_TCACHE_CONF_REALLY_DIRTY_WM_shift      (28)
#define   NFP_MU_TCACHE_CONF_DIRTY_WM(_x)               (((_x) & 0xf) << 24)
#define   NFP_MU_TCACHE_CONF_DIRTY_WM_bf                0, 27, 24
#define   NFP_MU_TCACHE_CONF_DIRTY_WM_mask              (0xf)
#define   NFP_MU_TCACHE_CONF_DIRTY_WM_shift             (24)
#define   NFP_MU_TCACHE_CONF_HOLDOFF_TIMER(_x)          (((_x) & 0xff) << 16)
#define   NFP_MU_TCACHE_CONF_HOLDOFF_TIMER_bf           0, 23, 16
#define   NFP_MU_TCACHE_CONF_HOLDOFF_TIMER_mask         (0xff)
#define   NFP_MU_TCACHE_CONF_HOLDOFF_TIMER_shift        (16)
#define   NFP_MU_TCACHE_CONF_LLR_MASK(_x)               (((_x) & 0xff) << 8)
#define   NFP_MU_TCACHE_CONF_LLR_MASK_bf                0, 15, 8
#define   NFP_MU_TCACHE_CONF_LLR_MASK_mask              (0xff)
#define   NFP_MU_TCACHE_CONF_LLR_MASK_shift             (8)
#define   NFP_MU_TCACHE_CONF_HLR_MASK(_x)               ((_x) & 0xff)
#define   NFP_MU_TCACHE_CONF_HLR_MASK_bf                0, 7, 0
#define   NFP_MU_TCACHE_CONF_HLR_MASK_mask              (0xff)
#define   NFP_MU_TCACHE_CONF_HLR_MASK_shift             (0)
/* Register Type: MUTCacheConfigClean */
#define NFP_MU_TCACHE_CLEANCONF        0x0004
#define   NFP_MU_TCACHE_CLEANCONF_CLK_DIV(_x)           (((_x) & 0xff) << 8)
#define   NFP_MU_TCACHE_CLEANCONF_CLK_DIV_bf            0, 15, 8
#define   NFP_MU_TCACHE_CLEANCONF_CLK_DIV_mask          (0xff)
#define   NFP_MU_TCACHE_CLEANCONF_CLK_DIV_shift         (8)
#define   NFP_MU_TCACHE_CLEANCONF_ON_ALLOC              (0x1 << 3)
#define   NFP_MU_TCACHE_CLEANCONF_ON_ALLOC_bf           0, 3, 3
#define   NFP_MU_TCACHE_CLEANCONF_ON_ALLOC_mask         (0x1)
#define   NFP_MU_TCACHE_CLEANCONF_ON_ALLOC_bit          (3)
#define   NFP_MU_TCACHE_CLEANCONF_ON_HLR_RELEASE        (0x1 << 2)
#define   NFP_MU_TCACHE_CLEANCONF_ON_HLR_RELEASE_bf     0, 2, 2
#define   NFP_MU_TCACHE_CLEANCONF_ON_HLR_RELEASE_mask   (0x1)
#define   NFP_MU_TCACHE_CLEANCONF_ON_HLR_RELEASE_bit    (2)
#define   NFP_MU_TCACHE_CLEANCONF_ON_RELEASE            (0x1 << 1)
#define   NFP_MU_TCACHE_CLEANCONF_ON_RELEASE_bf         0, 1, 1
#define   NFP_MU_TCACHE_CLEANCONF_ON_RELEASE_mask       (0x1)
#define   NFP_MU_TCACHE_CLEANCONF_ON_RELEASE_bit        (1)
#define   NFP_MU_TCACHE_CLEANCONF_ENABLE                (0x1)
#define   NFP_MU_TCACHE_CLEANCONF_ENABLE_bf             0, 0, 0
#define   NFP_MU_TCACHE_CLEANCONF_ENABLE_mask           (0x1)
#define   NFP_MU_TCACHE_CLEANCONF_ENABLE_bit            (0)
/* Register Type: MUTCacheCommandStatus */
#define NFP_MU_TCACHE_CMDST            0x0008
#define   NFP_MU_TCACHE_CMDST_ABORTED                   (0x1 << 2)
#define   NFP_MU_TCACHE_CMDST_ABORTED_bf                0, 2, 2
#define   NFP_MU_TCACHE_CMDST_ABORTED_mask              (0x1)
#define   NFP_MU_TCACHE_CMDST_ABORTED_bit               (2)
#define   NFP_MU_TCACHE_CMDST_VALID                     (0x1 << 1)
#define   NFP_MU_TCACHE_CMDST_VALID_bf                  0, 1, 1
#define   NFP_MU_TCACHE_CMDST_VALID_mask                (0x1)
#define   NFP_MU_TCACHE_CMDST_VALID_bit                 (1)
#define   NFP_MU_TCACHE_CMDST_ERROR                     (0x1)
#define   NFP_MU_TCACHE_CMDST_ERROR_bf                  0, 0, 0
#define   NFP_MU_TCACHE_CMDST_ERROR_mask                (0x1)
#define   NFP_MU_TCACHE_CMDST_ERROR_bit                 (0)
/* Register Type: MUTCacheCommandControl */
#define NFP_MU_TCACHE_CMDCTL           0x000c
#define   NFP_MU_TCACHE_CMDCTL_SET(_x)                  (((_x) & 0x1ff) << 16)
#define   NFP_MU_TCACHE_CMDCTL_SET_mask                 (0x1ff)
#define   NFP_MU_TCACHE_CMDCTL_SET_shift                (16)
#define   NFP_MU_TCACHE_CMDCTL_WAYS(_x)                 (((_x) & 0xff) << 8)
#define   NFP_MU_TCACHE_CMDCTL_WAYS_mask                (0xff)
#define   NFP_MU_TCACHE_CMDCTL_WAYS_shift               (8)
#define   NFP_MU_TCACHE_CMDCTL_TYPE(_x)                 ((_x) & 0x7)
#define   NFP_MU_TCACHE_CMDCTL_TYPE_mask                (0x7)
#define   NFP_MU_TCACHE_CMDCTL_TYPE_shift               (0)
#define     NFP_MU_TCACHE_CMDCTL_TYPE_WRWAYS            (4)
#define     NFP_MU_TCACHE_CMDCTL_TYPE_YES               (5)
/* Register Type: MUTCacheWriteData */
#define NFP_MU_TCACHE_WDATA            0x0010
#define   NFP_MU_TCACHE_WDATA_STATE(_x)                 (((_x) & 0x7) << 16)
#define   NFP_MU_TCACHE_WDATA_STATE_bf                  0, 18, 16
#define   NFP_MU_TCACHE_WDATA_STATE_mask                (0x7)
#define   NFP_MU_TCACHE_WDATA_STATE_shift               (16)
#define     NFP_MU_TCACHE_WDATA_STATE_LOCKED            (0)
#define     NFP_MU_TCACHE_WDATA_STATE_INUSE             (1)
#define     NFP_MU_TCACHE_WDATA_STATE_DIRTYHLR          (2)
#define     NFP_MU_TCACHE_WDATA_STATE_DIRTYLLR          (3)
#define     NFP_MU_TCACHE_WDATA_STATE_CLEANHLR          (4)
#define     NFP_MU_TCACHE_WDATA_STATE_CLEANLLR          (5)
#define   NFP_MU_TCACHE_WDATA_TAG(_x)                   ((_x) & 0x7fff)
#define   NFP_MU_TCACHE_WDATA_TAG_bf                    0, 14, 0
#define   NFP_MU_TCACHE_WDATA_TAG_mask                  (0x7fff)
#define   NFP_MU_TCACHE_WDATA_TAG_shift                 (0)
/* Register Type: MUTCacheCommandResponse */
#define NFP_MU_TCACHE_CMDRESP          0x0014
#define   NFP_MU_TCACHE_CMDRESP_VALID                   (0x1 << 31)
#define   NFP_MU_TCACHE_CMDRESP_VALID_bf                0, 31, 31
#define   NFP_MU_TCACHE_CMDRESP_VALID_mask              (0x1)
#define   NFP_MU_TCACHE_CMDRESP_VALID_bit               (31)
#define   NFP_MU_TCACHE_CMDRESP_TYPE_bf                 0, 30, 28
#define   NFP_MU_TCACHE_CMDRESP_TYPE_mask               (0x7)
#define   NFP_MU_TCACHE_CMDRESP_TYPE_shift              (28)
#define   NFP_MU_TCACHE_CMDRESP_WAY_bf                  0, 22, 20
#define   NFP_MU_TCACHE_CMDRESP_WAY_mask                (0x7)
#define   NFP_MU_TCACHE_CMDRESP_WAY_shift               (20)
#define   NFP_MU_TCACHE_CMDRESP_STATE_bf                0, 18, 16
#define   NFP_MU_TCACHE_CMDRESP_STATE_mask              (0x7)
#define   NFP_MU_TCACHE_CMDRESP_STATE_shift             (16)
#define   NFP_MU_TCACHE_CMDRESP_TAG_bf                  0, 14, 0
#define   NFP_MU_TCACHE_CMDRESP_TAG_mask                (0x7fff)
#define   NFP_MU_TCACHE_CMDRESP_TAG_shift               (0)
/* Register Type: MUTCacheReadData */
#define NFP_MU_TCACHE_RDATA            0x0018
#define   NFP_MU_TCACHE_RDATA_STATE_bf                  0, 18, 16
#define   NFP_MU_TCACHE_RDATA_STATE_mask                (0x7)
#define   NFP_MU_TCACHE_RDATA_STATE_shift               (16)
#define   NFP_MU_TCACHE_RDATA_TAG_bf                    0, 14, 0
#define   NFP_MU_TCACHE_RDATA_TAG_mask                  (0x7fff)
#define   NFP_MU_TCACHE_RDATA_TAG_shift                 (0)
/* Register Type: MUTCacheCleanMarkAll */
#define NFP_MU_TCACHE_CLEANALL         0x0020
#define   NFP_MU_TCACHE_CLEANALL_DATA(_x)               ((_x) & 0x3)
#define   NFP_MU_TCACHE_CLEANALL_DATA_mask              (0x3)
#define   NFP_MU_TCACHE_CLEANALL_DATA_shift             (0)
#define     NFP_MU_TCACHE_CLEANALL_DATA_CLEAN           (0)
#define     NFP_MU_TCACHE_CLEANALL_DATA_DIRTY           (1)
#define     NFP_MU_TCACHE_CLEANALL_DATA_URGENT          (2)
/* Register Type: MUTCacheCleanGetDirtiness */
#define NFP_MU_TCACHE_CLEANDIRTY       0x0028
#define   NFP_MU_TCACHE_CLEANDIRTY_TYPE                 (0x1 << 4)
#define   NFP_MU_TCACHE_CLEANDIRTY_TYPE_bf              0, 4, 4
#define   NFP_MU_TCACHE_CLEANDIRTY_TYPE_mask            (0x1)
#define   NFP_MU_TCACHE_CLEANDIRTY_TYPE_bit             (4)
#define     NFP_MU_TCACHE_CLEANDIRTY_TYPE_URGENT        (0x0)
#define     NFP_MU_TCACHE_CLEANDIRTY_TYPE_DIRTY         (0x10)
#define   NFP_MU_TCACHE_CLEANDIRTY_GROUP(_x)            ((_x) & 0xf)
#define   NFP_MU_TCACHE_CLEANDIRTY_GROUP_mask           (0xf)
#define   NFP_MU_TCACHE_CLEANDIRTY_GROUP_shift          (0)
/* Register Type: MUTCacheCleanMarkSet */
#define NFP_MU_TCACHE_CLEANSET         0x002c
#define   NFP_MU_TCACHE_CLEANSET_GROUP(_x)              (((_x) & 0xf) << 9)
#define   NFP_MU_TCACHE_CLEANSET_GROUP_mask             (0xf)
#define   NFP_MU_TCACHE_CLEANSET_GROUP_shift            (9)
#define   NFP_MU_TCACHE_CLEANSET_SET(_x)                (((_x) & 0x1f) << 4)
#define   NFP_MU_TCACHE_CLEANSET_SET_mask               (0x1f)
#define   NFP_MU_TCACHE_CLEANSET_SET_shift              (4)
#define   NFP_MU_TCACHE_CLEANSET_DATA(_x)               ((_x) & 0x3)
#define   NFP_MU_TCACHE_CLEANSET_DATA_mask              (0x3)
#define   NFP_MU_TCACHE_CLEANSET_DATA_shift             (0)
#define     NFP_MU_TCACHE_CLEANSET_DATA_CLEAN           (0)
#define     NFP_MU_TCACHE_CLEANSET_DATA_DIRTY           (1)
#define     NFP_MU_TCACHE_CLEANSET_DATA_URGENT          (2)
/* Register Type: MUTCacheDirtiness */
#define NFP_MU_TCACHE_CLEANNESS        0x0030
#define   NFP_MU_TCACHE_CLEANNESS_DATA(_x)              ((_x) & 0xff)
#define   NFP_MU_TCACHE_CLEANNESS_DATA_mask             (0xff)
#define   NFP_MU_TCACHE_CLEANNESS_DATA_shift            (0)
/* Register Type: MUTCachePerformanceCounterMode */
#define NFP_MU_TCACHE_PERFMODE         0x0034
#define   NFP_MU_TCACHE_PERFMODE_DATA(_x)               ((_x) & 0xf)
#define   NFP_MU_TCACHE_PERFMODE_DATA_bf                0, 3, 0
#define   NFP_MU_TCACHE_PERFMODE_DATA_mask              (0xf)
#define   NFP_MU_TCACHE_PERFMODE_DATA_shift             (0)
#define     NFP_MU_TCACHE_PERFMODE_DATA_HOLD            (0)
#define     NFP_MU_TCACHE_PERFMODE_DATA_RESET           (1)
#define     NFP_MU_TCACHE_PERFMODE_DATA_STATS           (2)
#define     NFP_MU_TCACHE_PERFMODE_DATA_REQ0MON         (3)
#define     NFP_MU_TCACHE_PERFMODE_DATA_ALLOCCLEAN      (4)
#define     NFP_MU_TCACHE_PERFMODE_DATA_ALLOCRDY        (5)
#define     NFP_MU_TCACHE_PERFMODE_DATA_ALLOCEVICT      (6)
#define     NFP_MU_TCACHE_PERFMODE_DATA_DIRTY           (7)
#define     NFP_MU_TCACHE_PERFMODE_DATA_SETREQ          (8)
#define     NFP_MU_TCACHE_PERFMODE_DATA_FULLALL         (9)
#define     NFP_MU_TCACHE_PERFMODE_DATA_FULLHIGH        (10)
#define     NFP_MU_TCACHE_PERFMODE_DATA_FULLPAIRS       (11)
/* Register Type: MUTCachePerformanceCounterValue */
#define NFP_MU_TCACHE_PERFCOUNTER0     0x0040
#define NFP_MU_TCACHE_PERFCOUNTER1     0x0044
#define NFP_MU_TCACHE_PERFCOUNTER2     0x0048
#define NFP_MU_TCACHE_PERFCOUNTER3     0x004c
#define NFP_MU_TCACHE_PERFCOUNTER4     0x0050
#define NFP_MU_TCACHE_PERFCOUNTER5     0x0054
#define NFP_MU_TCACHE_PERFCOUNTER6     0x0058
#define NFP_MU_TCACHE_PERFCOUNTER7     0x005c
#define   NFP_MU_TCACHE_PERFCOUNTER_DATA_bf             0, 31, 0
#define   NFP_MU_TCACHE_PERFCOUNTER_DATA_mask           (0xffffffff)
#define   NFP_MU_TCACHE_PERFCOUNTER_DATA_shift          (0)

#endif /* __NFP_SDK__NFP3200_NFP_MU_H__ */
