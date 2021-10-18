/*
 * Copyright (C) 2009 - 2012  Netronome Systems, Inc.  All rights reserved.
 *
 * @file        nfp3200/nfp_qctl.h
 * @brief       Register definitions for Queue Controller Peripheral
 *
 * Disclaimer: this file is provided without any warranty
 * as part of an early-access program, and the content is
 * bound to change before the final release.
 */

#ifndef __NFP_SDK__NFP3200_NFP_QCTL_H__
#define __NFP_SDK__NFP3200_NFP_QCTL_H__

/* HGID: nfp3200/qctl.desc = 724a41f2f1b0 */
/* Register Type: QueueAddToReadPtr */
#define NFP_QC_QUEUE_ADD_RPTR          0x0000
#define   NFP_QC_QUEUE_ADD_RPTR_CNT(_x)                 ((_x) & 0x3ffff)
#define   NFP_QC_QUEUE_ADD_RPTR_CNT_mask                (0x3ffff)
#define   NFP_QC_QUEUE_ADD_RPTR_CNT_shift               (0)
/* Register Type: QueueAddToWritePtr */
#define NFP_QC_QUEUE_ADD_WPTR          0x0004
#define   NFP_QC_QUEUE_ADD_WPTR_CNT(_x)                 ((_x) & 0x3ffff)
#define   NFP_QC_QUEUE_ADD_WPTR_CNT_mask                (0x3ffff)
#define   NFP_QC_QUEUE_ADD_WPTR_CNT_shift               (0)
/* Register Type: QueueConfigWriteLow */
#define NFP_QC_QUEUE_CSR_LO            0x0008
#define   NFP_QC_QUEUE_CSR_LO_READPTR_ENABLE            (0x1 << 31)
#define   NFP_QC_QUEUE_CSR_LO_READPTR_ENABLE_bf         0, 31, 31
#define   NFP_QC_QUEUE_CSR_LO_READPTR_ENABLE_mask       (0x1)
#define   NFP_QC_QUEUE_CSR_LO_READPTR_ENABLE_bit        (31)
#define   NFP_QC_QUEUE_CSR_LO_EVENT_DATA(_x)            (((_x) & 0x3f) << 20)
#define   NFP_QC_QUEUE_CSR_LO_EVENT_DATA_mask           (0x3f)
#define   NFP_QC_QUEUE_CSR_LO_EVENT_DATA_shift          (20)
#define   NFP_QC_QUEUE_CSR_LO_EVENT_TYPE(_x)            (((_x) & 0x3) << 18)
#define   NFP_QC_QUEUE_CSR_LO_EVENT_TYPE_mask           (0x3)
#define   NFP_QC_QUEUE_CSR_LO_EVENT_TYPE_shift          (18)
#define     NFP_QC_QUEUE_EVENT_TYPE_NEVER               (0)
#define     NFP_QC_QUEUE_EVENT_TYPE_NOT_EMPTY           (1)
#define     NFP_QC_QUEUE_EVENT_TYPE_LO_WATERMARK        (2)
#define     NFP_QC_QUEUE_EVENT_TYPE_HI_WATERMARK        (3)
#define   NFP_QC_QUEUE_CSR_LO_READPTR(_x)               ((_x) & 0x3ffff)
#define   NFP_QC_QUEUE_CSR_LO_READPTR_mask              (0x3ffff)
#define   NFP_QC_QUEUE_CSR_LO_READPTR_shift             (0)
/* Register Type: QueueStatusLow */
#define NFP_QC_QUEUE_STS_LO            0x0008
#define   NFP_QC_QUEUE_STS_LO_OVERFLOWED                (0x1 << 30)
#define   NFP_QC_QUEUE_STS_LO_OVERFLOWED_bf             0, 30, 30
#define   NFP_QC_QUEUE_STS_LO_OVERFLOWED_mask           (0x1)
#define   NFP_QC_QUEUE_STS_LO_OVERFLOWED_bit            (30)
#define   NFP_QC_QUEUE_STS_LO_UNDERFLOWED               (0x1 << 29)
#define   NFP_QC_QUEUE_STS_LO_UNDERFLOWED_bf            0, 29, 29
#define   NFP_QC_QUEUE_STS_LO_UNDERFLOWED_mask          (0x1)
#define   NFP_QC_QUEUE_STS_LO_UNDERFLOWED_bit           (29)
#define   NFP_QC_QUEUE_STS_LO_WM                        (0x1 << 28)
#define   NFP_QC_QUEUE_STS_LO_WM_bf                     0, 28, 28
#define   NFP_QC_QUEUE_STS_LO_WM_mask                   (0x1)
#define   NFP_QC_QUEUE_STS_LO_WM_bit                    (28)
#define   NFP_QC_QUEUE_STS_LO_FULL                      (0x1 << 27)
#define   NFP_QC_QUEUE_STS_LO_FULL_bf                   0, 27, 27
#define   NFP_QC_QUEUE_STS_LO_FULL_mask                 (0x1)
#define   NFP_QC_QUEUE_STS_LO_FULL_bit                  (27)
#define   NFP_QC_QUEUE_STS_LO_EMPTY                     (0x1 << 26)
#define   NFP_QC_QUEUE_STS_LO_EMPTY_bf                  0, 26, 26
#define   NFP_QC_QUEUE_STS_LO_EMPTY_mask                (0x1)
#define   NFP_QC_QUEUE_STS_LO_EMPTY_bit                 (26)
#define   NFP_QC_QUEUE_STS_LO_EVENT_DATA_bf             0, 25, 20
#define   NFP_QC_QUEUE_STS_LO_EVENT_DATA_mask           (0x3f)
#define   NFP_QC_QUEUE_STS_LO_EVENT_DATA_shift          (20)
#define   NFP_QC_QUEUE_STS_LO_EVENT_TYPE_bf             0, 19, 18
#define   NFP_QC_QUEUE_STS_LO_EVENT_TYPE_mask           (0x3)
#define   NFP_QC_QUEUE_STS_LO_EVENT_TYPE_shift          (18)
#define   NFP_QC_QUEUE_STS_LO_READPTR_bf                0, 17, 0
#define   NFP_QC_QUEUE_STS_LO_READPTR_mask              (0x3ffff)
#define   NFP_QC_QUEUE_STS_LO_READPTR_shift             (0)
/* Register Type: QueueConfigWriteHigh */
#define NFP_QC_QUEUE_CSR_HI            0x000c
#define   NFP_QC_QUEUE_CSR_HI_CLR_OVERFLOWED            (0x1 << 30)
#define   NFP_QC_QUEUE_CSR_HI_CLR_OVERFLOWED_bf         0, 30, 30
#define   NFP_QC_QUEUE_CSR_HI_CLR_OVERFLOWED_mask       (0x1)
#define   NFP_QC_QUEUE_CSR_HI_CLR_OVERFLOWED_bit        (30)
#define   NFP_QC_QUEUE_CSR_HI_CLR_UNDERFLOWED           (0x1 << 29)
#define   NFP_QC_QUEUE_CSR_HI_CLR_UNDERFLOWED_bf        0, 29, 29
#define   NFP_QC_QUEUE_CSR_HI_CLR_UNDERFLOWED_mask      (0x1)
#define   NFP_QC_QUEUE_CSR_HI_CLR_UNDERFLOWED_bit       (29)
#define   NFP_QC_QUEUE_CSR_HI_CLR_WATERMARK             (0x1 << 28)
#define   NFP_QC_QUEUE_CSR_HI_CLR_WATERMARK_bf          0, 28, 28
#define   NFP_QC_QUEUE_CSR_HI_CLR_WATERMARK_mask        (0x1)
#define   NFP_QC_QUEUE_CSR_HI_CLR_WATERMARK_bit         (28)
#define   NFP_QC_QUEUE_CSR_HI_CLR_FULL                  (0x1 << 27)
#define   NFP_QC_QUEUE_CSR_HI_CLR_FULL_bf               0, 27, 27
#define   NFP_QC_QUEUE_CSR_HI_CLR_FULL_mask             (0x1)
#define   NFP_QC_QUEUE_CSR_HI_CLR_FULL_bit              (27)
#define   NFP_QC_QUEUE_CSR_HI_CLR_EMPTY                 (0x1 << 26)
#define   NFP_QC_QUEUE_CSR_HI_CLR_EMPTY_bf              0, 26, 26
#define   NFP_QC_QUEUE_CSR_HI_CLR_EMPTY_mask            (0x1)
#define   NFP_QC_QUEUE_CSR_HI_CLR_EMPTY_bit             (26)
#define   NFP_QC_QUEUE_CSR_HI_WATERMARK(_x)             (((_x) & 0x7) << 22)
#define   NFP_QC_QUEUE_CSR_HI_WATERMARK_mask            (0x7)
#define   NFP_QC_QUEUE_CSR_HI_WATERMARK_shift           (22)
#define     NFP_QC_QUEUE_WATERMARK_4                    (0)
#define     NFP_QC_QUEUE_WATERMARK_8                    (1)
#define     NFP_QC_QUEUE_WATERMARK_16                   (2)
#define     NFP_QC_QUEUE_WATERMARK_32                   (3)
#define     NFP_QC_QUEUE_WATERMARK_64                   (4)
#define     NFP_QC_QUEUE_WATERMARK_128                  (5)
#define     NFP_QC_QUEUE_WATERMARK_256                  (6)
#define     NFP_QC_QUEUE_WATERMARK_512                  (7)
#define   NFP_QC_QUEUE_CSR_HI_SIZE(_x)                  (((_x) & 0xf) << 18)
#define   NFP_QC_QUEUE_CSR_HI_SIZE_mask                 (0xf)
#define   NFP_QC_QUEUE_CSR_HI_SIZE_shift                (18)
#define     NFP_QC_QUEUE_SIZE_256                       (0)
#define     NFP_QC_QUEUE_SIZE_512                       (1)
#define     NFP_QC_QUEUE_SIZE_1K                        (0)
#define     NFP_QC_QUEUE_SIZE_2K                        (3)
#define     NFP_QC_QUEUE_SIZE_4K                        (4)
#define     NFP_QC_QUEUE_SIZE_8K                        (5)
#define     NFP_QC_QUEUE_SIZE_16K                       (6)
#define     NFP_QC_QUEUE_SIZE_32K                       (7)
#define     NFP_QC_QUEUE_SIZE_64K                       (8)
#define     NFP_QC_QUEUE_SIZE_128K                      (9)
#define     NFP_QC_QUEUE_SIZE_256K                      (10)
#define   NFP_QC_QUEUE_CSR_HI_WRITEPTR(_x)              ((_x) & 0x3ffff)
#define   NFP_QC_QUEUE_CSR_HI_WRITEPTR_mask             (0x3ffff)
#define   NFP_QC_QUEUE_CSR_HI_WRITEPTR_shift            (0)
/* Register Type: QueueStatusHigh */
#define NFP_QC_QUEUE_STS_HI            0x000c
#define   NFP_QC_QUEUE_STS_HI_OVERFLOWED                (0x1 << 30)
#define   NFP_QC_QUEUE_STS_HI_OVERFLOWED_bf             0, 30, 30
#define   NFP_QC_QUEUE_STS_HI_OVERFLOWED_mask           (0x1)
#define   NFP_QC_QUEUE_STS_HI_OVERFLOWED_bit            (30)
#define   NFP_QC_QUEUE_STS_HI_UNDERFLOWED               (0x1 << 29)
#define   NFP_QC_QUEUE_STS_HI_UNDERFLOWED_bf            0, 29, 29
#define   NFP_QC_QUEUE_STS_HI_UNDERFLOWED_mask          (0x1)
#define   NFP_QC_QUEUE_STS_HI_UNDERFLOWED_bit           (29)
#define   NFP_QC_QUEUE_STS_HI_WM                        (0x1 << 28)
#define   NFP_QC_QUEUE_STS_HI_WM_bf                     0, 28, 28
#define   NFP_QC_QUEUE_STS_HI_WM_mask                   (0x1)
#define   NFP_QC_QUEUE_STS_HI_WM_bit                    (28)
#define   NFP_QC_QUEUE_STS_HI_FULL                      (0x1 << 27)
#define   NFP_QC_QUEUE_STS_HI_FULL_bf                   0, 27, 27
#define   NFP_QC_QUEUE_STS_HI_FULL_mask                 (0x1)
#define   NFP_QC_QUEUE_STS_HI_FULL_bit                  (27)
#define   NFP_QC_QUEUE_STS_HI_EMPTY                     (0x1 << 26)
#define   NFP_QC_QUEUE_STS_HI_EMPTY_bf                  0, 26, 26
#define   NFP_QC_QUEUE_STS_HI_EMPTY_mask                (0x1)
#define   NFP_QC_QUEUE_STS_HI_EMPTY_bit                 (26)
#define   NFP_QC_QUEUE_STS_HI_WATERMARK_bf              0, 24, 22
#define   NFP_QC_QUEUE_STS_HI_WATERMARK_mask            (0x7)
#define   NFP_QC_QUEUE_STS_HI_WATERMARK_shift           (22)
#define   NFP_QC_QUEUE_STS_HI_SIZE_bf                   0, 21, 18
#define   NFP_QC_QUEUE_STS_HI_SIZE_mask                 (0xf)
#define   NFP_QC_QUEUE_STS_HI_SIZE_shift                (18)
#define   NFP_QC_QUEUE_STS_HI_WRITEPTR_bf               0, 17, 0
#define   NFP_QC_QUEUE_STS_HI_WRITEPTR_mask             (0x3ffff)
#define   NFP_QC_QUEUE_STS_HI_WRITEPTR_shift            (0)

#endif /* __NFP_SDK__NFP3200_NFP_QCTL_H__ */
