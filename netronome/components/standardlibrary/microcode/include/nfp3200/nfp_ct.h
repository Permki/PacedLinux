/*
 * Copyright (C) 2012  Netronome Systems, Inc.  All rights reserved.
 *
 * @file        nfp3200/nfp_ct.h
 * @brief       Register definitions for Cluster Target
 *
 * Disclaimer: this file is provided without any warranty
 * as part of an early-access program, and the content is
 * bound to change before the final release.
 */

#ifndef __NFP_SDK__NFP3200_NFP_CT_H__
#define __NFP_SDK__NFP3200_NFP_CT_H__

/* HGID: nfp3200/ctarget.desc = 5702fafbfb92 */
/* Register Type: CtgtNNConfig */
#define NFP_CT_NNCFG                   0x0000
#define   NFP_CT_NNCFG_CHAIN                            (0x1 << 1)
#define   NFP_CT_NNCFG_CHAIN_bf                         0, 1, 1
#define   NFP_CT_NNCFG_CHAIN_mask                       (0x1)
#define   NFP_CT_NNCFG_CHAIN_bit                        (1)
#define   NFP_CT_NNCFG_ENABLE_CPP                       (0x1)
#define   NFP_CT_NNCFG_ENABLE_CPP_bf                    0, 0, 0
#define   NFP_CT_NNCFG_ENABLE_CPP_mask                  (0x1)
#define   NFP_CT_NNCFG_ENABLE_CPP_bit                   (0)
/* Register Type: CtgtMePerf */
#define NFP_CT_ME_PERF                 0x0004
#define   NFP_CT_ME_PERF_TG3(_x)                        (((_x) & 0xf) << 18)
#define   NFP_CT_ME_PERF_TG3_bf                         0, 21, 18
#define   NFP_CT_ME_PERF_TG3_mask                       (0xf)
#define   NFP_CT_ME_PERF_TG3_shift                      (18)
#define     NFP_CT_ME_PERF_TG3_ZERO                     (0)
#define     NFP_CT_ME_PERF_TG3_ME0                      (1)
#define     NFP_CT_ME_PERF_TG3_LOW                      (2)
#define   NFP_CT_ME_PERF_TG2(_x)                        (((_x) & 0xf) << 14)
#define   NFP_CT_ME_PERF_TG2_bf                         0, 17, 14
#define   NFP_CT_ME_PERF_TG2_mask                       (0xf)
#define   NFP_CT_ME_PERF_TG2_shift                      (14)
#define     NFP_CT_ME_PERF_TG2_ZERO                     (0)
#define     NFP_CT_ME_PERF_TG2_ME0                      (1)
#define     NFP_CT_ME_PERF_TG2_LOW                      (2)
#define   NFP_CT_ME_PERF_TG1(_x)                        (((_x) & 0xf) << 10)
#define   NFP_CT_ME_PERF_TG1_bf                         0, 13, 10
#define   NFP_CT_ME_PERF_TG1_mask                       (0xf)
#define   NFP_CT_ME_PERF_TG1_shift                      (10)
#define     NFP_CT_ME_PERF_TG1_ZERO                     (0)
#define     NFP_CT_ME_PERF_TG1_ME0                      (1)
#define     NFP_CT_ME_PERF_TG1_LOW                      (2)
#define   NFP_CT_ME_PERF_TG0(_x)                        (((_x) & 0xf) << 6)
#define   NFP_CT_ME_PERF_TG0_bf                         0, 9, 6
#define   NFP_CT_ME_PERF_TG0_mask                       (0xf)
#define   NFP_CT_ME_PERF_TG0_shift                      (6)
#define     NFP_CT_ME_PERF_TG0_ZERO                     (0)
#define     NFP_CT_ME_PERF_TG0_ME0                      (1)
#define     NFP_CT_ME_PERF_TG0_LOW                      (2)
#define   NFP_CT_ME_PERF_LANE_SEL_HI(_x)                (((_x) & 0x3) << 4)
#define   NFP_CT_ME_PERF_LANE_SEL_HI_bf                 0, 5, 4
#define   NFP_CT_ME_PERF_LANE_SEL_HI_mask               (0x3)
#define   NFP_CT_ME_PERF_LANE_SEL_HI_shift              (4)
#define     NFP_CT_ME_PERF_LANE_SEL_HI_ZERO             (0)
#define     NFP_CT_ME_PERF_LANE_SEL_HI_LOW              (1)
#define     NFP_CT_ME_PERF_LANE_SEL_HI_MID              (2)
#define     NFP_CT_ME_PERF_LANE_SEL_HI_HIGH             (3)
#define   NFP_CT_ME_PERF_LANE_SEL_MID(_x)               (((_x) & 0x3) << 2)
#define   NFP_CT_ME_PERF_LANE_SEL_MID_bf                0, 3, 2
#define   NFP_CT_ME_PERF_LANE_SEL_MID_mask              (0x3)
#define   NFP_CT_ME_PERF_LANE_SEL_MID_shift             (2)
#define     NFP_CT_ME_PERF_LANE_SEL_MID_ZERO            (0)
#define     NFP_CT_ME_PERF_LANE_SEL_MID_LOW             (1)
#define     NFP_CT_ME_PERF_LANE_SEL_MID_MID             (2)
#define     NFP_CT_ME_PERF_LANE_SEL_MID_HIGH            (3)
#define   NFP_CT_ME_PERF_LANE_SEL_LO(_x)                ((_x) & 0x3)
#define   NFP_CT_ME_PERF_LANE_SEL_LO_bf                 0, 1, 0
#define   NFP_CT_ME_PERF_LANE_SEL_LO_mask               (0x3)
#define   NFP_CT_ME_PERF_LANE_SEL_LO_shift              (0)
#define     NFP_CT_ME_PERF_LANE_SEL_LO_ZERO             (0)
#define     NFP_CT_ME_PERF_LANE_SEL_LO_LOW              (1)
#define     NFP_CT_ME_PERF_LANE_SEL_LO_MID              (2)
#define     NFP_CT_ME_PERF_LANE_SEL_LO_HIGH             (3)
/* Register Type: CtgtCppPerf */
#define NFP_CT_CPP_PERF                0x0008
#define   NFP_CT_CPP_PERF_GNT(_x)                       (((_x) & 0xf) << 26)
#define   NFP_CT_CPP_PERF_GNT_bf                        0, 29, 26
#define   NFP_CT_CPP_PERF_GNT_mask                      (0xf)
#define   NFP_CT_CPP_PERF_GNT_shift                     (26)
#define     NFP_CT_CPP_PERF_GNT_ZERO                    (0)
#define     NFP_CT_CPP_PERF_GNT_REQ_GNT                 (1)
#define     NFP_CT_CPP_PERF_GNT_MEM                     (2)
#define     NFP_CT_CPP_PERF_GNT_IO                      (3)
#define     NFP_CT_CPP_PERF_GNT_GNT                     (4)
#define   NFP_CT_CPP_PERF_PUSH(_x)                      (((_x) & 0xf) << 22)
#define   NFP_CT_CPP_PERF_PUSH_bf                       0, 25, 22
#define   NFP_CT_CPP_PERF_PUSH_mask                     (0xf)
#define   NFP_CT_CPP_PERF_PUSH_shift                    (22)
#define     NFP_CT_CPP_PERF_PUSH_ZERO                   (0)
#define     NFP_CT_CPP_PERF_PUSH_A_DATA                 (1)
#define     NFP_CT_CPP_PERF_PUSH_A_SIG                  (2)
#define     NFP_CT_CPP_PERF_PUSH_B_DATA                 (3)
#define     NFP_CT_CPP_PERF_PUSH_B_SIG                  (4)
#define   NFP_CT_CPP_PERF_PULL(_x)                      (((_x) & 0xf) << 18)
#define   NFP_CT_CPP_PERF_PULL_bf                       0, 21, 18
#define   NFP_CT_CPP_PERF_PULL_mask                     (0xf)
#define   NFP_CT_CPP_PERF_PULL_shift                    (18)
#define     NFP_CT_CPP_PERF_PULL_ZERO                   (0)
#define     NFP_CT_CPP_PERF_PULL_A_DATA                 (1)
#define     NFP_CT_CPP_PERF_PULL_A_SIG                  (2)
#define     NFP_CT_CPP_PERF_PULL_B_DATA                 (3)
#define     NFP_CT_CPP_PERF_PULL_B_SIG                  (4)
#define   NFP_CT_CPP_PERF_CMD2(_x)                      (((_x) & 0xf) << 14)
#define   NFP_CT_CPP_PERF_CMD2_bf                       0, 17, 14
#define   NFP_CT_CPP_PERF_CMD2_mask                     (0xf)
#define   NFP_CT_CPP_PERF_CMD2_shift                    (14)
#define     NFP_CT_CPP_PERF_CMD2_ZERO                   (0)
#define     NFP_CT_CPP_PERF_CMD2_ADDR_LO                (1)
#define     NFP_CT_CPP_PERF_CMD2_ADDR_HI                (2)
#define     NFP_CT_CPP_PERF_CMD2_ADDR_NONE              (2)
#define   NFP_CT_CPP_PERF_CMD1(_x)                      (((_x) & 0xf) << 10)
#define   NFP_CT_CPP_PERF_CMD1_bf                       0, 13, 10
#define   NFP_CT_CPP_PERF_CMD1_mask                     (0xf)
#define   NFP_CT_CPP_PERF_CMD1_shift                    (10)
#define     NFP_CT_CPP_PERF_CMD1_ZERO                   (0)
#define     NFP_CT_CPP_PERF_CMD1_ADDR_LO                (1)
#define     NFP_CT_CPP_PERF_CMD1_ADDR_HI                (2)
#define     NFP_CT_CPP_PERF_CMD1_ADDR_NONE              (2)
#define   NFP_CT_CPP_PERF_CMD0(_x)                      (((_x) & 0xf) << 6)
#define   NFP_CT_CPP_PERF_CMD0_bf                       0, 9, 6
#define   NFP_CT_CPP_PERF_CMD0_mask                     (0xf)
#define   NFP_CT_CPP_PERF_CMD0_shift                    (6)
#define     NFP_CT_CPP_PERF_CMD0_ZERO                   (0)
#define     NFP_CT_CPP_PERF_CMD0_ADDR_LO                (1)
#define     NFP_CT_CPP_PERF_CMD0_ADDR_HI                (2)
#define     NFP_CT_CPP_PERF_CMD0_ADDR_NONE              (2)
#define   NFP_CT_CPP_PERF_LANE_SEL_HI(_x)               (((_x) & 0x3) << 4)
#define   NFP_CT_CPP_PERF_LANE_SEL_HI_bf                0, 5, 4
#define   NFP_CT_CPP_PERF_LANE_SEL_HI_mask              (0x3)
#define   NFP_CT_CPP_PERF_LANE_SEL_HI_shift             (4)
#define     NFP_CT_CPP_PERF_LANE_SEL_HI_ZERO            (0)
#define     NFP_CT_CPP_PERF_LANE_SEL_HI_LOW             (1)
#define     NFP_CT_CPP_PERF_LANE_SEL_HI_MID             (2)
#define     NFP_CT_CPP_PERF_LANE_SEL_HI_HIGH            (3)
#define   NFP_CT_CPP_PERF_LANE_SEL_MID(_x)              (((_x) & 0x3) << 2)
#define   NFP_CT_CPP_PERF_LANE_SEL_MID_bf               0, 3, 2
#define   NFP_CT_CPP_PERF_LANE_SEL_MID_mask             (0x3)
#define   NFP_CT_CPP_PERF_LANE_SEL_MID_shift            (2)
#define     NFP_CT_CPP_PERF_LANE_SEL_MID_ZERO           (0)
#define     NFP_CT_CPP_PERF_LANE_SEL_MID_LOW            (1)
#define     NFP_CT_CPP_PERF_LANE_SEL_MID_MID            (2)
#define     NFP_CT_CPP_PERF_LANE_SEL_MID_HIGH           (3)
#define   NFP_CT_CPP_PERF_LANE_SEL_LO(_x)               ((_x) & 0x3)
#define   NFP_CT_CPP_PERF_LANE_SEL_LO_bf                0, 1, 0
#define   NFP_CT_CPP_PERF_LANE_SEL_LO_mask              (0x3)
#define   NFP_CT_CPP_PERF_LANE_SEL_LO_shift             (0)
#define     NFP_CT_CPP_PERF_LANE_SEL_LO_ZERO            (0)
#define     NFP_CT_CPP_PERF_LANE_SEL_LO_LOW             (1)
#define     NFP_CT_CPP_PERF_LANE_SEL_LO_MID             (2)
#define     NFP_CT_CPP_PERF_LANE_SEL_LO_HIGH            (3)
/* Register Type: CtgtPreempt */
#define NFP_CT_PREEMT_A                0x000c
#define NFP_CT_PREEMT_B                0x0010
#define   NFP_CT_PREEMT_A_PULL_TIMER(_x)                (((_x) & 0xff) << 10)
#define   NFP_CT_PREEMT_A_PULL_TIMER_bf                 0, 17, 10
#define   NFP_CT_PREEMT_A_PULL_TIMER_mask               (0xff)
#define   NFP_CT_PREEMT_A_PULL_TIMER_shift              (10)
#define   NFP_CT_PREEMT_A_PULL_ENABLE                   (0x1 << 9)
#define   NFP_CT_PREEMT_A_PULL_ENABLE_bf                0, 9, 9
#define   NFP_CT_PREEMT_A_PULL_ENABLE_mask              (0x1)
#define   NFP_CT_PREEMT_A_PULL_ENABLE_bit               (9)
#define   NFP_CT_PREEMT_A_PUSH_TIMER(_x)                (((_x) & 0xff) << 1)
#define   NFP_CT_PREEMT_A_PUSH_TIMER_bf                 0, 8, 1
#define   NFP_CT_PREEMT_A_PUSH_TIMER_mask               (0xff)
#define   NFP_CT_PREEMT_A_PUSH_TIMER_shift              (1)
#define   NFP_CT_PREEMT_A_PUSH_ENABLE                   (0x1)
#define   NFP_CT_PREEMT_A_PUSH_ENABLE_bf                0, 0, 0
#define   NFP_CT_PREEMT_A_PUSH_ENABLE_mask              (0x1)
#define   NFP_CT_PREEMT_A_PUSH_ENABLE_bit               (0)
/* Register Type: CtgtCppClPerf */
#define NFP_CT_CL_PERF                 0x0014
#define   NFP_CT_CL_PERF_CL5(_x)                        (((_x) & 0xf) << 26)
#define   NFP_CT_CL_PERF_CL5_bf                         0, 29, 26
#define   NFP_CT_CL_PERF_CL5_mask                       (0xf)
#define   NFP_CT_CL_PERF_CL5_shift                      (26)
#define     NFP_CT_CL_PERF_CL5_ZERO                     (0)
#define   NFP_CT_CL_PERF_CL4(_x)                        (((_x) & 0xf) << 22)
#define   NFP_CT_CL_PERF_CL4_bf                         0, 25, 22
#define   NFP_CT_CL_PERF_CL4_mask                       (0xf)
#define   NFP_CT_CL_PERF_CL4_shift                      (22)
#define     NFP_CT_CL_PERF_CL4_ZERO                     (0)
#define   NFP_CT_CL_PERF_CL3(_x)                        (((_x) & 0xf) << 18)
#define   NFP_CT_CL_PERF_CL3_bf                         0, 21, 18
#define   NFP_CT_CL_PERF_CL3_mask                       (0xf)
#define   NFP_CT_CL_PERF_CL3_shift                      (18)
#define     NFP_CT_CL_PERF_CL3_ZERO                     (0)
#define   NFP_CT_CL_PERF_CL2(_x)                        (((_x) & 0xf) << 14)
#define   NFP_CT_CL_PERF_CL2_bf                         0, 17, 14
#define   NFP_CT_CL_PERF_CL2_mask                       (0xf)
#define   NFP_CT_CL_PERF_CL2_shift                      (14)
#define     NFP_CT_CL_PERF_CL2_ZERO                     (0)
#define   NFP_CT_CL_PERF_CL1(_x)                        (((_x) & 0xf) << 10)
#define   NFP_CT_CL_PERF_CL1_bf                         0, 13, 10
#define   NFP_CT_CL_PERF_CL1_mask                       (0xf)
#define   NFP_CT_CL_PERF_CL1_shift                      (10)
#define     NFP_CT_CL_PERF_CL1_ZERO                     (0)
#define   NFP_CT_CL_PERF_CL0(_x)                        (((_x) & 0xf) << 6)
#define   NFP_CT_CL_PERF_CL0_bf                         0, 9, 6
#define   NFP_CT_CL_PERF_CL0_mask                       (0xf)
#define   NFP_CT_CL_PERF_CL0_shift                      (6)
#define     NFP_CT_CL_PERF_CL0_ZERO                     (0)
#define   NFP_CT_CL_PERF_LANE_SEL_HI(_x)                (((_x) & 0x3) << 4)
#define   NFP_CT_CL_PERF_LANE_SEL_HI_bf                 0, 5, 4
#define   NFP_CT_CL_PERF_LANE_SEL_HI_mask               (0x3)
#define   NFP_CT_CL_PERF_LANE_SEL_HI_shift              (4)
#define     NFP_CT_CL_PERF_LANE_SEL_HI_ZERO             (0)
#define     NFP_CT_CL_PERF_LANE_SEL_HI_LOW              (1)
#define     NFP_CT_CL_PERF_LANE_SEL_HI_MID              (2)
#define     NFP_CT_CL_PERF_LANE_SEL_HI_HIGH             (3)
#define   NFP_CT_CL_PERF_LANE_SEL_MID(_x)               (((_x) & 0x3) << 2)
#define   NFP_CT_CL_PERF_LANE_SEL_MID_bf                0, 3, 2
#define   NFP_CT_CL_PERF_LANE_SEL_MID_mask              (0x3)
#define   NFP_CT_CL_PERF_LANE_SEL_MID_shift             (2)
#define     NFP_CT_CL_PERF_LANE_SEL_MID_ZERO            (0)
#define     NFP_CT_CL_PERF_LANE_SEL_MID_LOW             (1)
#define     NFP_CT_CL_PERF_LANE_SEL_MID_MID             (2)
#define     NFP_CT_CL_PERF_LANE_SEL_MID_HIGH            (3)
#define   NFP_CT_CL_PERF_LANE_SEL_LO(_x)                ((_x) & 0x3)
#define   NFP_CT_CL_PERF_LANE_SEL_LO_bf                 0, 1, 0
#define   NFP_CT_CL_PERF_LANE_SEL_LO_mask               (0x3)
#define   NFP_CT_CL_PERF_LANE_SEL_LO_shift              (0)
#define     NFP_CT_CL_PERF_LANE_SEL_LO_ZERO             (0)
#define     NFP_CT_CL_PERF_LANE_SEL_LO_LOW              (1)
#define     NFP_CT_CL_PERF_LANE_SEL_LO_MID              (2)
#define     NFP_CT_CL_PERF_LANE_SEL_LO_HIGH             (3)

#endif /* __NFP_SDK__NFP3200_NFP_CT_H__ */
