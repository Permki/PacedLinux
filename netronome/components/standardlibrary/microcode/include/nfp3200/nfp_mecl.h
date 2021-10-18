/*
 * Copyright (C) 2012  Netronome Systems, Inc.  All rights reserved.
 *
 * @file        nfp3200/nfp_mecl.h
 * @brief       Register definitions for ME Cluster
 *
 * Disclaimer: this file is provided without any warranty
 * as part of an early-access program, and the content is
 * bound to change before the final release.
 */

#ifndef __NFP_SDK__NFP3200_NFP_MECL_H__
#define __NFP_SDK__NFP3200_NFP_MECL_H__

/* HGID: nfp3200/lscratch.desc = 014261175c48 */
#define NFP_MECL_SRAM_BASE             0x0000
#define NFP_MECL_SRAM_LEN              0x10000
/* Register Type: ClsRingBase */
#define NFP_MECL_RING_BASE(_r)         (0x10000 + (0x8 * ((_r) & 0xf)))
#define   NFP_MECL_RING_BASE_FULL                       (0x1 << 31)
#define   NFP_MECL_RING_BASE_FULL_bf                    0, 31, 31
#define   NFP_MECL_RING_BASE_FULL_mask                  (0x1)
#define   NFP_MECL_RING_BASE_FULL_bit                   (31)
#define   NFP_MECL_RING_BASE_NOT_EMPTY                  (0x1 << 30)
#define   NFP_MECL_RING_BASE_NOT_EMPTY_bf               0, 30, 30
#define   NFP_MECL_RING_BASE_NOT_EMPTY_mask             (0x1)
#define   NFP_MECL_RING_BASE_NOT_EMPTY_bit              (30)
#define   NFP_MECL_RING_BASE_REPORT(_x)                 (((_x) & 0xf) << 24)
#define   NFP_MECL_RING_BASE_REPORT_bf                  0, 27, 24
#define   NFP_MECL_RING_BASE_REPORT_mask                (0xf)
#define   NFP_MECL_RING_BASE_REPORT_shift               (24)
#define     NFP_MECL_RING_BASE_REPORT_UNDERFLOW         (1)
#define     NFP_MECL_RING_BASE_REPORT_OVERFLOW          (2)
#define     NFP_MECL_RING_BASE_REPORT_NOT_EMPTY         (4)
#define     NFP_MECL_RING_BASE_REPORT_NOT_FULL          (8)
#define   NFP_MECL_RING_BASE_SIZE(_x)                   (((_x) & 0x7) << 16)
#define   NFP_MECL_RING_BASE_SIZE_bf                    0, 18, 16
#define   NFP_MECL_RING_BASE_SIZE_mask                  (0x7)
#define   NFP_MECL_RING_BASE_SIZE_shift                 (16)
#define     NFP_MECL_RING_BASE_SIZE_32                  (0)
#define     NFP_MECL_RING_BASE_SIZE_64                  (1)
#define     NFP_MECL_RING_BASE_SIZE_128                 (2)
#define     NFP_MECL_RING_BASE_SIZE_256                 (3)
#define     NFP_MECL_RING_BASE_SIZE_512                 (4)
#define     NFP_MECL_RING_BASE_SIZE_1024                (5)
#define   NFP_MECL_RING_BASE_BASE(_x)                   ((_x) & 0x1ff)
#define   NFP_MECL_RING_BASE_BASE_bf                    0, 8, 0
#define   NFP_MECL_RING_BASE_BASE_mask                  (0x1ff)
#define   NFP_MECL_RING_BASE_BASE_shift                 (0)
/* Register Type: ClsRingPointers */
#define NFP_MECL_RING_POINTER(_r)      (0x10080 + (0x8 * ((_r) & 0xf)))
#define   NFP_MECL_RING_POINTER_TAIL(_x)                (((_x) & 0x7ff) << 16)
#define   NFP_MECL_RING_POINTER_TAIL_bf                 0, 26, 16
#define   NFP_MECL_RING_POINTER_TAIL_mask               (0x7ff)
#define   NFP_MECL_RING_POINTER_TAIL_shift              (16)
#define   NFP_MECL_RING_POINTER_HEAD(_x)                ((_x) & 0x7ff)
#define   NFP_MECL_RING_POINTER_HEAD_bf                 0, 10, 0
#define   NFP_MECL_RING_POINTER_HEAD_mask               (0x7ff)
#define   NFP_MECL_RING_POINTER_HEAD_shift              (0)
#define NFP_MECL_EM                    0x20000
/* Register Type: ClsAutoFilterStatusMonitor */
#define NFP_MECL_AUTOPUSH_STATUS(_m)   (0x30000 + (0x8 * ((_m) & 0xf)))
#define   NFP_MECL_AUTOPUSH_STATUS_LAST_STATUS          (0x1 << 17)
#define   NFP_MECL_AUTOPUSH_STATUS_LAST_STATUS_bf       0, 17, 17
#define   NFP_MECL_AUTOPUSH_STATUS_LAST_STATUS_mask     (0x1)
#define   NFP_MECL_AUTOPUSH_STATUS_LAST_STATUS_bit      (17)
#define   NFP_MECL_AUTOPUSH_STATUS_EDGE_DETECTED        (0x1 << 16)
#define   NFP_MECL_AUTOPUSH_STATUS_EDGE_DETECTED_bf     0, 16, 16
#define   NFP_MECL_AUTOPUSH_STATUS_EDGE_DETECTED_mask   (0x1)
#define   NFP_MECL_AUTOPUSH_STATUS_EDGE_DETECTED_bit    (16)
#define   NFP_MECL_AUTOPUSH_STATUS_MONITOR(_x)          (((_x) & 0x3) << 8)
#define   NFP_MECL_AUTOPUSH_STATUS_MONITOR_bf           0, 9, 8
#define   NFP_MECL_AUTOPUSH_STATUS_MONITOR_mask         (0x3)
#define   NFP_MECL_AUTOPUSH_STATUS_MONITOR_shift        (8)
#define     NFP_MECL_AUTOPUSH_STATUS_MONITOR_OFF        (0)
#define     NFP_MECL_AUTOPUSH_STATUS_MONITOR_STATUS     (1)
#define     NFP_MECL_AUTOPUSH_STATUS_MONITOR_ONE_SHOT_STATUS (2)
#define     NFP_MECL_AUTOPUSH_STATUS_MONITOR_ONE_SHOT_ACK (3)
#define   NFP_MECL_AUTOPUSH_STATUS_AUTOPUSH(_x)         ((_x) & 0x7)
#define   NFP_MECL_AUTOPUSH_STATUS_AUTOPUSH_bf          0, 2, 0
#define   NFP_MECL_AUTOPUSH_STATUS_AUTOPUSH_mask        (0x7)
#define   NFP_MECL_AUTOPUSH_STATUS_AUTOPUSH_shift       (0)
/* Register Type: ClsAutoSignal */
#define NFP_MECL_AUTOPUSH_SIGNAL(_s)   (0x30200 + (0x8 * ((_s) & 0x7)))
#define   NFP_MECL_AUTOPUSH_SIGNAL_MASTER(_x)           (((_x) & 0xf) << 24)
#define   NFP_MECL_AUTOPUSH_SIGNAL_MASTER_bf            0, 27, 24
#define   NFP_MECL_AUTOPUSH_SIGNAL_MASTER_mask          (0xf)
#define   NFP_MECL_AUTOPUSH_SIGNAL_MASTER_shift         (24)
#define   NFP_MECL_AUTOPUSH_SIGNAL_SIGNAL_REF(_x)       (((_x) & 0x7f) << 16)
#define   NFP_MECL_AUTOPUSH_SIGNAL_SIGNAL_REF_bf        0, 22, 16
#define   NFP_MECL_AUTOPUSH_SIGNAL_SIGNAL_REF_mask      (0x7f)
#define   NFP_MECL_AUTOPUSH_SIGNAL_SIGNAL_REF_shift     (16)
#define   NFP_MECL_AUTOPUSH_SIGNAL_DATA_REF(_x)         ((_x) & 0x3fff)
#define   NFP_MECL_AUTOPUSH_SIGNAL_DATA_REF_bf          0, 13, 0
#define   NFP_MECL_AUTOPUSH_SIGNAL_DATA_REF_mask        (0x3fff)
#define   NFP_MECL_AUTOPUSH_SIGNAL_DATA_REF_shift       (0)
/* Register Type: ClsAutoEventStatus */
#define NFP_MECL_AUTOPUSH_USER_EVENT_STATUS 0x30400
#define   NFP_MECL_AUTOPUSH_USER_EVENT_STATUS_OVERFLOW  (0x1 << 20)
#define   NFP_MECL_AUTOPUSH_USER_EVENT_STATUS_OVERFLOW_bf 0, 20, 20
#define   NFP_MECL_AUTOPUSH_USER_EVENT_STATUS_OVERFLOW_mask (0x1)
#define   NFP_MECL_AUTOPUSH_USER_EVENT_STATUS_OVERFLOW_bit (20)
#define   NFP_MECL_AUTOPUSH_USER_EVENT_STATUS_HWM_bf    0, 19, 16
#define   NFP_MECL_AUTOPUSH_USER_EVENT_STATUS_HWM_mask  (0xf)
#define   NFP_MECL_AUTOPUSH_USER_EVENT_STATUS_HWM_shift (16)
#define   NFP_MECL_AUTOPUSH_USER_EVENT_STATUS_READ_bf   0, 11, 8
#define   NFP_MECL_AUTOPUSH_USER_EVENT_STATUS_READ_mask (0xf)
#define   NFP_MECL_AUTOPUSH_USER_EVENT_STATUS_READ_shift (8)
#define   NFP_MECL_AUTOPUSH_USER_EVENT_STATUS_WRITE_bf  0, 3, 0
#define   NFP_MECL_AUTOPUSH_USER_EVENT_STATUS_WRITE_mask (0xf)
#define   NFP_MECL_AUTOPUSH_USER_EVENT_STATUS_WRITE_shift (0)
/* Register Type: ClsAutoEvent */
#define NFP_MECL_AUTOPUSH_USER_EVENT   0x30400
#define   NFP_MECL_AUTOPUSH_USER_EVENT_SOURCE(_x)       (((_x) & 0xfff) << 4)
#define   NFP_MECL_AUTOPUSH_USER_EVENT_SOURCE_mask      (0xfff)
#define   NFP_MECL_AUTOPUSH_USER_EVENT_SOURCE_shift     (4)
#define   NFP_MECL_AUTOPUSH_USER_EVENT_TYPE(_x)         ((_x) & 0xf)
#define   NFP_MECL_AUTOPUSH_USER_EVENT_TYPE_mask        (0xf)
#define   NFP_MECL_AUTOPUSH_USER_EVENT_TYPE_shift       (0)
/* Register Type: ClsHashMultiply */
#define NFP_MECL_HASH_MULT             0x40000
#define   NFP_MECL_HASH_MULT_MULT_63                    (0x1 << 3)
#define   NFP_MECL_HASH_MULT_MULT_63_bf                 0, 3, 3
#define   NFP_MECL_HASH_MULT_MULT_63_mask               (0x1)
#define   NFP_MECL_HASH_MULT_MULT_63_bit                (3)
#define   NFP_MECL_HASH_MULT_MULT_53                    (0x1 << 2)
#define   NFP_MECL_HASH_MULT_MULT_53_bf                 0, 2, 2
#define   NFP_MECL_HASH_MULT_MULT_53_mask               (0x1)
#define   NFP_MECL_HASH_MULT_MULT_53_bit                (2)
#define   NFP_MECL_HASH_MULT_MULT_36                    (0x1 << 1)
#define   NFP_MECL_HASH_MULT_MULT_36_bf                 0, 1, 1
#define   NFP_MECL_HASH_MULT_MULT_36_mask               (0x1)
#define   NFP_MECL_HASH_MULT_MULT_36_bit                (1)
#define   NFP_MECL_HASH_MULT_MULT_4                     (0x1)
#define   NFP_MECL_HASH_MULT_MULT_4_bf                  0, 0, 0
#define   NFP_MECL_HASH_MULT_MULT_4_mask                (0x1)
#define   NFP_MECL_HASH_MULT_MULT_4_bit                 (0)
/* Register Type: ClsHashIndexLow */
#define NFP_MECL_HASH_INDEX_LO(_h)     (0x40010 + (0x100 * ((_h) & 0x7)))
#define   NFP_MECL_HASH_INDEX_LO_INDEX_LO_bf            0, 31, 0
#define   NFP_MECL_HASH_INDEX_LO_INDEX_LO_mask          (0xffffffff)
#define   NFP_MECL_HASH_INDEX_LO_INDEX_LO_shift         (0)
/* Register Type: ClsHashIndexHigh */
#define NFP_MECL_HASH_INDEX_HI(_h)     (0x40018 + (0x100 * ((_h) & 0x7)))
#define   NFP_MECL_HASH_INDEX_HI_INDEX_HI_bf            0, 31, 0
#define   NFP_MECL_HASH_INDEX_HI_INDEX_HI_mask          (0xffffffff)
#define   NFP_MECL_HASH_INDEX_HI_INDEX_HI_shift         (0)
#define NFP_MECL_IM                    0x50000
/* Register Type: ClsTrngAccessRequest */
#define NFP_MECL_TRNG_REQUEST          0x60000
#define   NFP_MECL_TRNG_REQUEST_CLEAR_ERR               (0x1 << 31)
#define   NFP_MECL_TRNG_REQUEST_CLEAR_ERR_bf            0, 31, 31
#define   NFP_MECL_TRNG_REQUEST_CLEAR_ERR_mask          (0x1)
#define   NFP_MECL_TRNG_REQUEST_CLEAR_ERR_bit           (31)
#define   NFP_MECL_TRNG_REQUEST_ACCESS(_x)              (((_x) & 0xf) << 8)
#define   NFP_MECL_TRNG_REQUEST_ACCESS_mask             (0xf)
#define   NFP_MECL_TRNG_REQUEST_ACCESS_shift            (8)
#define     NFP_MECL_TRNG_REQUEST_ACCESS_READ           (0)
#define     NFP_MECL_TRNG_REQUEST_ACCESS_WRITE          (1)
#define     NFP_MECL_TRNG_REQUEST_ACCESS_FETCH          (2)
#define     NFP_MECL_TRNG_REQUEST_ACCESS_POLL           (3)
#define   NFP_MECL_TRNG_REQUEST_ADDR(_x)                (((_x) & 0x7) << 4)
#define   NFP_MECL_TRNG_REQUEST_ADDR_mask               (0x7)
#define   NFP_MECL_TRNG_REQUEST_ADDR_shift              (4)
/* Register Type: ClsTrngWriteData */
#define NFP_MECL_TRNG_REQUEST_DATA     0x60008
/* Register Type: ClsTrngData */
#define NFP_MECL_TRNG_DATA0            0x60020
#define NFP_MECL_TRNG_DATA1            0x60028
#define NFP_MECL_TRNG_DATA2            0x60030
#define NFP_MECL_TRNG_DATA3            0x60038

#define NFP_MECL_RING_BASE_SIZE_CALC(x)     (log2((x)) - 5)

#endif /* __NFP_SDK__NFP3200_NFP_MECL_H__ */
