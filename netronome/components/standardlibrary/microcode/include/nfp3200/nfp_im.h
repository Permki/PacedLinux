/*
 * Copyright (C) 2012  Netronome Systems, Inc.  All rights reserved.
 *
 * @file        nfp3200/im.h
 * @brief       Register definitions for Interrupt Manager
 *
 * Disclaimer: this file is provided without any warranty
 * as part of an early-access program, and the content is
 * bound to change before the final release.
 */

#ifndef __NFP_SDK__NFP3200_NFP_IM_H__
#define __NFP_SDK__NFP3200_NFP_IM_H__

/* HGID: nfp3200/intmgr.desc = 5702fafbfb92 */
/* Register Type: IMgrStatus */
#define NFP_IM_PIN_STATUS              0x0000
/* Register Type: IMgrEnable */
#define NFP_IM_PIN_DISABLE             0x0004
#define NFP_IM_PIN_ENABLE_LOW          0x000c
#define NFP_IM_PIN_ENABLE_MED          0x0014
#define NFP_IM_PIN_ENABLE_HIGH         0x001c
/* Register Type: IMgrEnabledStatus */
#define NFP_IM_PIN_STATUS_LOW          0x0008
#define NFP_IM_INTR_STS_MI             0x0010
#define NFP_IM_PIN_STATUS_HIGH         0x0018
/* Register Type: IMgrConfig */
#define NFP_IM_MODULE_CONFIG           0x0020
/* Register Type: IMgrStatusEventConfig */
#define NFP_IM_EVENT_CONFIG_0          0x0028
#define NFP_IM_EVENT_CONFIG_1          0x002c
#define   NFP_IM_EVENT_CONFIG_EDGE_15(_x)               (((_x) & 0x3) << 30)
#define   NFP_IM_EVENT_CONFIG_EDGE_15_bf                0, 31, 30
#define   NFP_IM_EVENT_CONFIG_EDGE_15_mask              (0x3)
#define   NFP_IM_EVENT_CONFIG_EDGE_15_shift             (30)
#define   NFP_IM_EVENT_CONFIG_EDGE_14(_x)               (((_x) & 0x3) << 28)
#define   NFP_IM_EVENT_CONFIG_EDGE_14_bf                0, 29, 28
#define   NFP_IM_EVENT_CONFIG_EDGE_14_mask              (0x3)
#define   NFP_IM_EVENT_CONFIG_EDGE_14_shift             (28)
#define   NFP_IM_EVENT_CONFIG_EDGE_13(_x)               (((_x) & 0x3) << 26)
#define   NFP_IM_EVENT_CONFIG_EDGE_13_bf                0, 27, 26
#define   NFP_IM_EVENT_CONFIG_EDGE_13_mask              (0x3)
#define   NFP_IM_EVENT_CONFIG_EDGE_13_shift             (26)
#define   NFP_IM_EVENT_CONFIG_EDGE_12(_x)               (((_x) & 0x3) << 24)
#define   NFP_IM_EVENT_CONFIG_EDGE_12_bf                0, 25, 24
#define   NFP_IM_EVENT_CONFIG_EDGE_12_mask              (0x3)
#define   NFP_IM_EVENT_CONFIG_EDGE_12_shift             (24)
#define   NFP_IM_EVENT_CONFIG_EDGE_11(_x)               (((_x) & 0x3) << 22)
#define   NFP_IM_EVENT_CONFIG_EDGE_11_bf                0, 23, 22
#define   NFP_IM_EVENT_CONFIG_EDGE_11_mask              (0x3)
#define   NFP_IM_EVENT_CONFIG_EDGE_11_shift             (22)
#define   NFP_IM_EVENT_CONFIG_EDGE_10(_x)               (((_x) & 0x3) << 20)
#define   NFP_IM_EVENT_CONFIG_EDGE_10_bf                0, 21, 20
#define   NFP_IM_EVENT_CONFIG_EDGE_10_mask              (0x3)
#define   NFP_IM_EVENT_CONFIG_EDGE_10_shift             (20)
#define   NFP_IM_EVENT_CONFIG_EDGE_9(_x)                (((_x) & 0x3) << 18)
#define   NFP_IM_EVENT_CONFIG_EDGE_9_bf                 0, 19, 18
#define   NFP_IM_EVENT_CONFIG_EDGE_9_mask               (0x3)
#define   NFP_IM_EVENT_CONFIG_EDGE_9_shift              (18)
#define   NFP_IM_EVENT_CONFIG_EDGE_8(_x)                (((_x) & 0x3) << 16)
#define   NFP_IM_EVENT_CONFIG_EDGE_8_bf                 0, 17, 16
#define   NFP_IM_EVENT_CONFIG_EDGE_8_mask               (0x3)
#define   NFP_IM_EVENT_CONFIG_EDGE_8_shift              (16)
#define   NFP_IM_EVENT_CONFIG_EDGE_7(_x)                (((_x) & 0x3) << 14)
#define   NFP_IM_EVENT_CONFIG_EDGE_7_bf                 0, 15, 14
#define   NFP_IM_EVENT_CONFIG_EDGE_7_mask               (0x3)
#define   NFP_IM_EVENT_CONFIG_EDGE_7_shift              (14)
#define   NFP_IM_EVENT_CONFIG_EDGE_6(_x)                (((_x) & 0x3) << 12)
#define   NFP_IM_EVENT_CONFIG_EDGE_6_bf                 0, 13, 12
#define   NFP_IM_EVENT_CONFIG_EDGE_6_mask               (0x3)
#define   NFP_IM_EVENT_CONFIG_EDGE_6_shift              (12)
#define   NFP_IM_EVENT_CONFIG_EDGE_5(_x)                (((_x) & 0x3) << 10)
#define   NFP_IM_EVENT_CONFIG_EDGE_5_bf                 0, 11, 10
#define   NFP_IM_EVENT_CONFIG_EDGE_5_mask               (0x3)
#define   NFP_IM_EVENT_CONFIG_EDGE_5_shift              (10)
#define   NFP_IM_EVENT_CONFIG_EDGE_4(_x)                (((_x) & 0x3) << 8)
#define   NFP_IM_EVENT_CONFIG_EDGE_4_bf                 0, 9, 8
#define   NFP_IM_EVENT_CONFIG_EDGE_4_mask               (0x3)
#define   NFP_IM_EVENT_CONFIG_EDGE_4_shift              (8)
#define   NFP_IM_EVENT_CONFIG_EDGE_3(_x)                (((_x) & 0x3) << 6)
#define   NFP_IM_EVENT_CONFIG_EDGE_3_bf                 0, 7, 6
#define   NFP_IM_EVENT_CONFIG_EDGE_3_mask               (0x3)
#define   NFP_IM_EVENT_CONFIG_EDGE_3_shift              (6)
#define   NFP_IM_EVENT_CONFIG_EDGE_2(_x)                (((_x) & 0x3) << 4)
#define   NFP_IM_EVENT_CONFIG_EDGE_2_bf                 0, 5, 4
#define   NFP_IM_EVENT_CONFIG_EDGE_2_mask               (0x3)
#define   NFP_IM_EVENT_CONFIG_EDGE_2_shift              (4)
#define   NFP_IM_EVENT_CONFIG_EDGE_1(_x)                (((_x) & 0x3) << 2)
#define   NFP_IM_EVENT_CONFIG_EDGE_1_bf                 0, 3, 2
#define   NFP_IM_EVENT_CONFIG_EDGE_1_mask               (0x3)
#define   NFP_IM_EVENT_CONFIG_EDGE_1_shift              (2)
#define   NFP_IM_EVENT_CONFIG_EDGE_0(_x)                ((_x) & 0x3)
#define   NFP_IM_EVENT_CONFIG_EDGE_0_bf                 0, 1, 0
#define   NFP_IM_EVENT_CONFIG_EDGE_0_mask               (0x3)
#define   NFP_IM_EVENT_CONFIG_EDGE_0_shift              (0)
/* Register Type: IMgrEventOut */
#define NFP_IM_EVENT_TEST              0x0030
#define   NFP_IM_EVENT_TEST_SOURCE(_x)                  (((_x) & 0xfff) << 4)
#define   NFP_IM_EVENT_TEST_SOURCE_mask                 (0xfff)
#define   NFP_IM_EVENT_TEST_SOURCE_shift                (4)
#define   NFP_IM_EVENT_TEST_TYPE(_x)                    ((_x) & 0xf)
#define   NFP_IM_EVENT_TEST_TYPE_mask                   (0xf)
#define   NFP_IM_EVENT_TEST_TYPE_shift                  (0)
/* Register Type: IMgrPerformanceAnalyzerControl */
#define NFP_IM_PERF_CONTROL            0x0034
/* Register Type: IMgrCaptureTimerStatus */
#define NFP_IM_CAP_TIMER_STATUS        0x0038
/* Register Type: IMgrCaptureTimerValue */
#define NFP_IM_CAP_TIMER_VALUE         0x003c
#define   NFP_IM_CAP_TIMER_VALUE_VALID                  (0x1 << 31)
#define   NFP_IM_CAP_TIMER_VALUE_VALID_bf               0, 31, 31
#define   NFP_IM_CAP_TIMER_VALUE_VALID_mask             (0x1)
#define   NFP_IM_CAP_TIMER_VALUE_VALID_bit              (31)
#define   NFP_IM_CAP_TIMER_VALUE_VALUE_bf               0, 30, 0
#define   NFP_IM_CAP_TIMER_VALUE_VALUE_mask             (0x7fffffff)
#define   NFP_IM_CAP_TIMER_VALUE_VALUE_shift            (0)
/* Register Type: IMgrStatus */
#define NFP_IMX8_PIN_STATUS            0x0000
/* Register Type: IMgrEnable */
#define NFP_IMX8_PIN_DISABLE           0x0008
#define NFP_IMX8_PIN_ENABLE_LOW        0x0018
#define NFP_IMX8_PIN_ENABLE_MED        0x0028
#define NFP_IMX8_PIN_ENABLE_HIGH       0x0038
/* Register Type: IMgrEnabledStatus */
#define NFP_IMX8_PIN_STATUS_LOW        0x0010
#define NFP_IMX8_INTR_STS_MI           0x0020
#define NFP_IMX8_PIN_STATUS_HIGH       0x0030
/* Register Type: IMgrConfig */
#define NFP_IMX8_MODULE_CONFIG         0x0040
/* Register Type: IMgrStatusEventConfig */
#define NFP_IMX8_EVENT_CONFIG_0        0x0050
#define NFP_IMX8_EVENT_CONFIG_1        0x0058
/* Register Type: IMgrEventOut */
#define NFP_IMX8_EVENT_TEST            0x0060
/* Register Type: IMgrPerformanceAnalyzerControl */
#define NFP_IMX8_PERF_CONTROL          0x0068
/* Register Type: IMgrCaptureTimerStatus */
#define NFP_IMX8_CAP_TIMER_STATUS      0x0070
/* Register Type: IMgrCaptureTimerValue */
#define NFP_IMX8_CAP_TIMER_VALUE       0x0078

#endif /* __NFP_SDK__NFP3200_NFP_IM_H__ */
