/*
 * Copyright (C) 2012  Netronome Systems, Inc.  All rights reserved.
 *
 * @file        nfp3200/nfp_timer.h
 * @brief       Register definitions for timer peripheral
 *
 * Disclaimer: this file is provided without any warranty
 * as part of an early-access program, and the content is
 * bound to change before the final release.
 */

#ifndef __NFP_SDK__NFP3200_NFP_TIMER_H__
#define __NFP_SDK__NFP3200_NFP_TIMER_H__

/* HGID: nfp3200/timer.desc = 9aa6ddb03994 */
/* Register Type: TimerControl */
#define NFP_TIMER_CTRL0                0x0000
#define NFP_TIMER_CTRL1                0x0004
#define NFP_TIMER_CTRL2                0x0008
#define NFP_TIMER_CTRL3                0x000c
#define   NFP_TIMER_CTRL_ACT                            (0x1 << 7)
#define   NFP_TIMER_CTRL_ACT_bf                         0, 7, 7
#define   NFP_TIMER_CTRL_ACT_mask                       (0x1)
#define   NFP_TIMER_CTRL_ACT_bit                        (7)
#define     NFP_TIMER_CTRL_ACT_ACTIVATED                (0x0)
#define     NFP_TIMER_CTRL_ACT_DEACTIVATED              (0x80)
#define   NFP_TIMER_CTRL_PSS(_x)                        (((_x) & 0x3) << 2)
#define   NFP_TIMER_CTRL_PSS_bf                         0, 3, 2
#define   NFP_TIMER_CTRL_PSS_mask                       (0x3)
#define   NFP_TIMER_CTRL_PSS_shift                      (2)
#define     NFP_TIMER_CTRL_PSS_CLK                      (0)
#define     NFP_TIMER_CTRL_PSS_CLK_16                   (1)
#define     NFP_TIMER_CTRL_PSS_CLK_256                  (2)
#define     NFP_TIMER_CTRL_PSS_GPIO                     (3)
/* Register Type: TimerCounterLoading */
#define NFP_TIMER_CNT_LOAD0            0x0010
#define NFP_TIMER_CNT_LOAD1            0x0014
#define NFP_TIMER_CNT_LOAD2            0x0018
#define NFP_TIMER_CNT_LOAD3            0x001c
#define   NFP_TIMER_CNT_LOAD_INITIAL_VAL(_x)            (_x)
#define   NFP_TIMER_CNT_LOAD_INITIAL_VAL_bf             0, 31, 0
#define   NFP_TIMER_CNT_LOAD_INITIAL_VAL_mask           (0xffffffff)
#define   NFP_TIMER_CNT_LOAD_INITIAL_VAL_shift          (0)
/* Register Type: TimerCounterStatus */
#define NFP_TIMER_CNT_STS0             0x0020
#define NFP_TIMER_CNT_STS1             0x0024
#define NFP_TIMER_CNT_STS2             0x0028
#define NFP_TIMER_CNT_STS3             0x002c
#define   NFP_TIMER_CNT_STS_CURRENT_VAL_bf              0, 31, 0
#define   NFP_TIMER_CNT_STS_CURRENT_VAL_mask            (0xffffffff)
#define   NFP_TIMER_CNT_STS_CURRENT_VAL_shift           (0)
/* Register Type: TimerCounterClear */
#define NFP_TIMER_CNT_CLR0             0x0030
#define NFP_TIMER_CNT_CLR1             0x0034
#define NFP_TIMER_CNT_CLR2             0x0038
#define NFP_TIMER_CNT_CLR3             0x003c
#define   NFP_TIMER_CNT_CLR_INTR_CLR                    (0x1)
#define   NFP_TIMER_CNT_CLR_INTR_CLR_bf                 0, 0, 0
#define   NFP_TIMER_CNT_CLR_INTR_CLR_mask               (0x1)
#define   NFP_TIMER_CNT_CLR_INTR_CLR_bit                (0)
#define     NFP_TIMER_CNT_CLR_INTR_CLR_NONE             (0x0)
#define     NFP_TIMER_CNT_CLR_INTR_CLR_CLEAR            (0x1)
/* Register Type: TimerWatchdogEnable */
#define NFP_TIMER_T_WDE                0x0040
#define   NFP_TIMER_t_wde_WDE                           (0x1)
#define   NFP_TIMER_t_wde_WDE_bf                        0, 0, 0
#define   NFP_TIMER_t_wde_WDE_mask                      (0x1)
#define   NFP_TIMER_t_wde_WDE_bit                       (0)

#endif /* __NFP_SDK__NFP3200_NFP_TIMER_H__ */
