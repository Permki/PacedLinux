/*
 * Copyright (C) 2012, Netronome Systems, Inc.  All rights reserved.
 *
 * @file        nfp3200/nfp_event.h
 * @brief       Definitions for event bus and event managers
 *
 * Disclaimer: this file is provided without any warranty
 * as part of an early-access program, and the content is
 * bound to change before the final release.
 */

#ifndef __NFP_SDK__NFP3200_NFP_EVENT_H__
#define __NFP_SDK__NFP3200_NFP_EVENT_H__

/* Event bus providers */
#define NFP_EVENT_PROVIDER_CLS(cluster)     (cluster)
#define NFP_EVENT_PROVIDER_MSF0             8
#define NFP_EVENT_PROVIDER_PCIE             9
#define NFP_EVENT_PROVIDER_MSF1             10
#define NFP_EVENT_PROVIDER_CRYPTO           11
#define NFP_EVENT_PROVIDER_ARM              12
#define NFP_EVENT_PROVIDER_DDR              14
#define NFP_EVENT_PROVIDER_SHAC             15

/* Standard event bus types */
#define NFP_EVENT_TYPE_FIFO_NOT_EMPTY           0
#define NFP_EVENT_TYPE_FIFO_NOT_FULL            1
#define NFP_EVENT_TYPE_DMA                      2
#define NFP_EVENT_TYPE_PROCESS                  3
#define NFP_EVENT_TYPE_STATUS                   4
#define NFP_EVENT_TYPE_FIFO_UNDERFLOW           8
#define NFP_EVENT_TYPE_FIFO_OVERFLOW            9
#define NFP_EVENT_TYPE_ECC_SINGLE_CORRECTION    10
#define NFP_EVENT_TYPE_ECC_MULTI_ERROR          11
#define NFP_EVENT_TYPE_ECC_SINGLE_ERROR         12

/* Event data as seen on the event bus */
#define NFP_EVENT_TYPE(type)                    ((type) & 0xf)
#define NFP_EVENT_TYPE_of(ev)                   ((ev) & 0xf)
#define NFP_EVENT_TYPE_bf                       0, 3, 0
#define NFP_EVENT_SOURCE(src)                   (((src) & 0xfff) << 4)
#define NFP_EVENT_SOURCE_of(ev)                 (((ev) >> 4) & 0xfff)
#define NFP_EVENT_SOURCE_bf                     0, 15, 4
#define NFP_EVENT_PROVIDER(prv)                 (((prv) & 0xf) << 16)
#define NFP_EVENT_PROVIDER_of(ev)               (((ev) >> 16) & 0xf)
#define NFP_EVENT_PROVIDER_bf                   0, 19, 16
#define NFP_EVENT_MATCH(prv, src, type)         (NFP_EVENT_PROVIDER(prv) | \
                                                 NFP_EVENT_SOURCE(src) | \
                                                 NFP_EVENT_TYPE(type))

/* HGID: nfp3200/evntmgr.desc = eaa9b989b828 */
#define NFP_EM_FILTER(_f)              (0x0 + (0x20 * ((_f) & 0x1f)))
/* Register Type: EventFilterStatus */
#define NFP_EM_FILTER_STATUS           0x0000
#define NFP_EM_FILTER_ACK              0x0010
/* Register Type: EventFilterFlag */
#define NFP_EM_FILTER_FLAGS            0x0004
#define   NFP_EM_FILTER_FLAGS_PENDING_STATUS            (0x1 << 1)
#define   NFP_EM_FILTER_FLAGS_PENDING_STATUS_bf         0, 1, 1
#define   NFP_EM_FILTER_FLAGS_PENDING_STATUS_mask       (0x1)
#define   NFP_EM_FILTER_FLAGS_PENDING_STATUS_bit        (1)
#define   NFP_EM_FILTER_FLAGS_STATUS                    (0x1)
#define   NFP_EM_FILTER_FLAGS_STATUS_bf                 0, 0, 0
#define   NFP_EM_FILTER_FLAGS_STATUS_mask               (0x1)
#define   NFP_EM_FILTER_FLAGS_STATUS_bit                (0)
/* Register Type: EventFilterMask */
#define NFP_EM_FILTER_MASK             0x0008
#define   NFP_EM_FILTER_MASK_TYPE(_x)                   (((_x) & 0x7) << 24)
#define   NFP_EM_FILTER_MASK_TYPE_bf                    0, 26, 24
#define   NFP_EM_FILTER_MASK_TYPE_mask                  (0x7)
#define   NFP_EM_FILTER_MASK_TYPE_shift                 (24)
#define     NFP_EM_FILTER_MASK_TYPE_COUNT32             (0)
#define     NFP_EM_FILTER_MASK_TYPE_COUNT16             (1)
#define     NFP_EM_FILTER_MASK_TYPE_MASK32              (2)
#define     NFP_EM_FILTER_MASK_TYPE_MASK16              (3)
#define     NFP_EM_FILTER_MASK_TYPE_FIRSTEV             (4)
#define     NFP_EM_FILTER_MASK_TYPE_LASTEV              (5)
#define   NFP_EM_FILTER_MASK_EVENT(_x)                  ((_x) & 0xfffff)
#define   NFP_EM_FILTER_MASK_EVENT_bf                   0, 19, 0
#define   NFP_EM_FILTER_MASK_EVENT_mask                 (0xfffff)
#define   NFP_EM_FILTER_MASK_EVENT_shift                (0)
/* Register Type: EventFilterMatch */
#define NFP_EM_FILTER_MATCH            0x000c
#define   NFP_EM_FILTER_MATCH_EVENT(_x)                 ((_x) & 0xfffff)
#define   NFP_EM_FILTER_MATCH_EVENT_bf                  0, 19, 0
#define   NFP_EM_FILTER_MATCH_EVENT_mask                (0xfffff)
#define   NFP_EM_FILTER_MATCH_EVENT_shift               (0)
/* Register Type: EventCombinedStatus */
#define NFP_EM_ALL_STATUS              0x0400
/* Register Type: EventCombinedPendingStatus */
#define NFP_EM_ALL_PENDING             0x0404
/* Register Type: EventConfig */
#define NFP_EM_CONFIG                  0x0408
#define NFP_EMX8_FILTER(_f)            (0x0 + (0x40 * ((_f) & 0x1f)))
/* Register Type: EventFilterStatus */
#define NFP_EMX8_FILTER_STATUS         0x0000
#define NFP_EMX8_FILTER_ACK            0x0020
/* Register Type: EventFilterFlag */
#define NFP_EMX8_FILTER_FLAGS          0x0008
/* Register Type: EventFilterMask */
#define NFP_EMX8_FILTER_MASK           0x0010
/* Register Type: EventFilterMatch */
#define NFP_EMX8_FILTER_MATCH          0x0018
/* Register Type: EventCombinedStatus */
#define NFP_EMX8_ALL_STATUS            0x0800
/* Register Type: EventCombinedPendingStatus */
#define NFP_EMX8_ALL_PENDING           0x0808
/* Register Type: EventConfig */
#define NFP_EMX8_CONFIG                0x0810
/* Register Type: EventFilterStatusCount32 */
#define NFP_EVENT_TYPE_COUNT32         0x0000
#define   NFP_EVENT_COUNT32_CNT32_bf                    0, 31, 0
#define   NFP_EVENT_COUNT32_CNT32_mask                  (0xffffffff)
#define   NFP_EVENT_COUNT32_CNT32_shift                 (0)
/* Register Type: EventFilterStatusCount16 */
#define NFP_EVENT_TYPE_COUNT16         0x0001
#define   NFP_EVENT_COUNT16_TMOUT(_x)                   (((_x) & 0x7) << 29)
#define   NFP_EVENT_COUNT16_TMOUT_bf                    0, 31, 29
#define   NFP_EVENT_COUNT16_TMOUT_mask                  (0x7)
#define   NFP_EVENT_COUNT16_TMOUT_shift                 (29)
#define   NFP_EVENT_COUNT16_UPCNT_bf                    0, 28, 23
#define   NFP_EVENT_COUNT16_UPCNT_mask                  (0x3f)
#define   NFP_EVENT_COUNT16_UPCNT_shift                 (23)
#define   NFP_EVENT_COUNT16_OVERRIDE(_x)                (((_x) & 0x3f) << 16)
#define   NFP_EVENT_COUNT16_OVERRIDE_bf                 0, 21, 16
#define   NFP_EVENT_COUNT16_OVERRIDE_mask               (0x3f)
#define   NFP_EVENT_COUNT16_OVERRIDE_shift              (16)
#define   NFP_EVENT_COUNT16_CNT16_bf                    0, 15, 0
#define   NFP_EVENT_COUNT16_CNT16_mask                  (0xffff)
#define   NFP_EVENT_COUNT16_CNT16_shift                 (0)
/* Register Type: EventFilterStatusBitmask32 */
#define NFP_EVENT_TYPE_MASK32          0x0002
/* Register Type: EventFilterStatusBitmask16 */
#define NFP_EVENT_TYPE_MASK16          0x0003
#define   NFP_EVENT_MASK16_TMOUT(_x)                    (((_x) & 0x7) << 29)
#define   NFP_EVENT_MASK16_TMOUT_bf                     0, 31, 29
#define   NFP_EVENT_MASK16_TMOUT_mask                   (0x7)
#define   NFP_EVENT_MASK16_TMOUT_shift                  (29)
#define   NFP_EVENT_MASK16_UPCNT_bf                     0, 28, 23
#define   NFP_EVENT_MASK16_UPCNT_mask                   (0x3f)
#define   NFP_EVENT_MASK16_UPCNT_shift                  (23)
#define   NFP_EVENT_MASK16_OVERRIDE(_x)                 (((_x) & 0x7) << 20)
#define   NFP_EVENT_MASK16_OVERRIDE_bf                  0, 22, 20
#define   NFP_EVENT_MASK16_OVERRIDE_mask                (0x7)
#define   NFP_EVENT_MASK16_OVERRIDE_shift               (20)
#define   NFP_EVENT_MASK16_CNT4(_x)                     (((_x) & 0xf) << 16)
#define   NFP_EVENT_MASK16_CNT4_bf                      0, 19, 16
#define   NFP_EVENT_MASK16_CNT4_mask                    (0xf)
#define   NFP_EVENT_MASK16_CNT4_shift                   (16)
#define   NFP_EVENT_MASK16_MASK16_bf                    0, 15, 0
#define   NFP_EVENT_MASK16_MASK16_mask                  (0xffff)
#define   NFP_EVENT_MASK16_MASK16_shift                 (0)
/* Register Type: EventFilterStatusEvent */
#define NFP_EVENT_TYPE_FIRSTEV         0x0004
#define NFP_EVENT_TYPE_LASTEV          0x0005
#define   NFP_EVENT_EVENT_TMOUT(_x)                     (((_x) & 0x7) << 29)
#define   NFP_EVENT_EVENT_TMOUT_bf                      0, 31, 29
#define   NFP_EVENT_EVENT_TMOUT_mask                    (0x7)
#define   NFP_EVENT_EVENT_TMOUT_shift                   (29)
#define   NFP_EVENT_EVENT_UPCNT_bf                      0, 28, 23
#define   NFP_EVENT_EVENT_UPCNT_mask                    (0x3f)
#define   NFP_EVENT_EVENT_UPCNT_shift                   (23)
#define   NFP_EVENT_EVENT_CNT2(_x)                      (((_x) & 0x3) << 20)
#define   NFP_EVENT_EVENT_CNT2_bf                       0, 21, 20
#define   NFP_EVENT_EVENT_CNT2_mask                     (0x3)
#define   NFP_EVENT_EVENT_CNT2_shift                    (20)
#define   NFP_EVENT_EVENT_EVENT_bf                      0, 19, 0
#define   NFP_EVENT_EVENT_EVENT_mask                    (0xfffff)
#define   NFP_EVENT_EVENT_EVENT_shift                   (0)


#endif /* __NFP_SDK__NFP3200_NFP_EVENT_H__ */
