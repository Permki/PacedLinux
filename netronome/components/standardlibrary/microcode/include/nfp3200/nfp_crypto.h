/*
 * Copyright (C) 2012  Netronome Systems, Inc.  All rights reserved.
 *
 * @file        nfp3200/nfp_crypto.h
 * @brief       Register definitions for Crypto
 *
 * Disclaimer: this file is provided without any warranty
 * as part of an early-access program, and the content is
 * bound to change before the final release.
 */

#ifndef __NFP_SDK__NFP3200_NFP_CRYPTO_H__
#define __NFP_SDK__NFP3200_NFP_CRYPTO_H__

/* HGID: nfp3200/crypto_cif.desc = 30d128ef7055 */
/* Register Type: CIFConfig */
#define NFP_CRYPTO_CIF_CFG             0x0000
/* Register Type: CIFErrFrst1 */
#define NFP_CRYPTO_CIF_FERR1           0x0010
#define   NFP_CRYPTO_CIF_FERR1_CDS_BLK_UNDFLW           (0x1 << 31)
#define   NFP_CRYPTO_CIF_FERR1_CDS_BLK_UNDFLW_bf        0, 31, 31
#define   NFP_CRYPTO_CIF_FERR1_CDS_BLK_UNDFLW_mask      (0x1)
#define   NFP_CRYPTO_CIF_FERR1_CDS_BLK_UNDFLW_bit       (31)
#define   NFP_CRYPTO_CIF_FERR1_CDS_BLK_OVRFLW           (0x1 << 30)
#define   NFP_CRYPTO_CIF_FERR1_CDS_BLK_OVRFLW_bf        0, 30, 30
#define   NFP_CRYPTO_CIF_FERR1_CDS_BLK_OVRFLW_mask      (0x1)
#define   NFP_CRYPTO_CIF_FERR1_CDS_BLK_OVRFLW_bit       (30)
#define   NFP_CRYPTO_CIF_FERR1_CDS_PKI_UNDFLW           (0x1 << 29)
#define   NFP_CRYPTO_CIF_FERR1_CDS_PKI_UNDFLW_bf        0, 29, 29
#define   NFP_CRYPTO_CIF_FERR1_CDS_PKI_UNDFLW_mask      (0x1)
#define   NFP_CRYPTO_CIF_FERR1_CDS_PKI_UNDFLW_bit       (29)
#define   NFP_CRYPTO_CIF_FERR1_CDS_PKI_OVRFLW           (0x1 << 28)
#define   NFP_CRYPTO_CIF_FERR1_CDS_PKI_OVRFLW_bf        0, 28, 28
#define   NFP_CRYPTO_CIF_FERR1_CDS_PKI_OVRFLW_mask      (0x1)
#define   NFP_CRYPTO_CIF_FERR1_CDS_PKI_OVRFLW_bit       (28)
#define   NFP_CRYPTO_CIF_FERR1_CDS_DMA_UNDFLW           (0x1 << 27)
#define   NFP_CRYPTO_CIF_FERR1_CDS_DMA_UNDFLW_bf        0, 27, 27
#define   NFP_CRYPTO_CIF_FERR1_CDS_DMA_UNDFLW_mask      (0x1)
#define   NFP_CRYPTO_CIF_FERR1_CDS_DMA_UNDFLW_bit       (27)
#define   NFP_CRYPTO_CIF_FERR1_CDS_DMA_OVRFLW           (0x1 << 26)
#define   NFP_CRYPTO_CIF_FERR1_CDS_DMA_OVRFLW_bf        0, 26, 26
#define   NFP_CRYPTO_CIF_FERR1_CDS_DMA_OVRFLW_mask      (0x1)
#define   NFP_CRYPTO_CIF_FERR1_CDS_DMA_OVRFLW_bit       (26)
#define   NFP_CRYPTO_CIF_FERR1_CDS_CBG_UNDFLW           (0x1 << 25)
#define   NFP_CRYPTO_CIF_FERR1_CDS_CBG_UNDFLW_bf        0, 25, 25
#define   NFP_CRYPTO_CIF_FERR1_CDS_CBG_UNDFLW_mask      (0x1)
#define   NFP_CRYPTO_CIF_FERR1_CDS_CBG_UNDFLW_bit       (25)
#define   NFP_CRYPTO_CIF_FERR1_CDS_CBG_OVRFLW           (0x1 << 24)
#define   NFP_CRYPTO_CIF_FERR1_CDS_CBG_OVRFLW_bf        0, 24, 24
#define   NFP_CRYPTO_CIF_FERR1_CDS_CBG_OVRFLW_mask      (0x1)
#define   NFP_CRYPTO_CIF_FERR1_CDS_CBG_OVRFLW_bit       (24)
#define   NFP_CRYPTO_CIF_FERR1_CDS_EVT_UNDFLW           (0x1 << 23)
#define   NFP_CRYPTO_CIF_FERR1_CDS_EVT_UNDFLW_bf        0, 23, 23
#define   NFP_CRYPTO_CIF_FERR1_CDS_EVT_UNDFLW_mask      (0x1)
#define   NFP_CRYPTO_CIF_FERR1_CDS_EVT_UNDFLW_bit       (23)
#define   NFP_CRYPTO_CIF_FERR1_CDS_EVT_OVRFLW           (0x1 << 22)
#define   NFP_CRYPTO_CIF_FERR1_CDS_EVT_OVRFLW_bf        0, 22, 22
#define   NFP_CRYPTO_CIF_FERR1_CDS_EVT_OVRFLW_mask      (0x1)
#define   NFP_CRYPTO_CIF_FERR1_CDS_EVT_OVRFLW_bit       (22)
#define   NFP_CRYPTO_CIF_FERR1_CDS_PRE_UNDFLW           (0x1 << 21)
#define   NFP_CRYPTO_CIF_FERR1_CDS_PRE_UNDFLW_bf        0, 21, 21
#define   NFP_CRYPTO_CIF_FERR1_CDS_PRE_UNDFLW_mask      (0x1)
#define   NFP_CRYPTO_CIF_FERR1_CDS_PRE_UNDFLW_bit       (21)
#define   NFP_CRYPTO_CIF_FERR1_CDS_PRE_OVRFLW           (0x1 << 20)
#define   NFP_CRYPTO_CIF_FERR1_CDS_PRE_OVRFLW_bf        0, 20, 20
#define   NFP_CRYPTO_CIF_FERR1_CDS_PRE_OVRFLW_mask      (0x1)
#define   NFP_CRYPTO_CIF_FERR1_CDS_PRE_OVRFLW_bit       (20)
#define   NFP_CRYPTO_CIF_FERR1_CDS_SGL_UNDFLW           (0x1 << 19)
#define   NFP_CRYPTO_CIF_FERR1_CDS_SGL_UNDFLW_bf        0, 19, 19
#define   NFP_CRYPTO_CIF_FERR1_CDS_SGL_UNDFLW_mask      (0x1)
#define   NFP_CRYPTO_CIF_FERR1_CDS_SGL_UNDFLW_bit       (19)
#define   NFP_CRYPTO_CIF_FERR1_CDS_SGL_OVRFLW           (0x1 << 18)
#define   NFP_CRYPTO_CIF_FERR1_CDS_SGL_OVRFLW_bf        0, 18, 18
#define   NFP_CRYPTO_CIF_FERR1_CDS_SGL_OVRFLW_mask      (0x1)
#define   NFP_CRYPTO_CIF_FERR1_CDS_SGL_OVRFLW_bit       (18)
#define   NFP_CRYPTO_CIF_FERR1_CBG_GRD_UNDFLW           (0x1 << 17)
#define   NFP_CRYPTO_CIF_FERR1_CBG_GRD_UNDFLW_bf        0, 17, 17
#define   NFP_CRYPTO_CIF_FERR1_CBG_GRD_UNDFLW_mask      (0x1)
#define   NFP_CRYPTO_CIF_FERR1_CBG_GRD_UNDFLW_bit       (17)
#define   NFP_CRYPTO_CIF_FERR1_CBG_GRD_OVRFLW           (0x1 << 16)
#define   NFP_CRYPTO_CIF_FERR1_CBG_GRD_OVRFLW_bf        0, 16, 16
#define   NFP_CRYPTO_CIF_FERR1_CBG_GRD_OVRFLW_mask      (0x1)
#define   NFP_CRYPTO_CIF_FERR1_CBG_GRD_OVRFLW_bit       (16)
#define   NFP_CRYPTO_CIF_FERR1_CBG_GPKI_UNDFLW          (0x1 << 15)
#define   NFP_CRYPTO_CIF_FERR1_CBG_GPKI_UNDFLW_bf       0, 15, 15
#define   NFP_CRYPTO_CIF_FERR1_CBG_GPKI_UNDFLW_mask     (0x1)
#define   NFP_CRYPTO_CIF_FERR1_CBG_GPKI_UNDFLW_bit      (15)
#define   NFP_CRYPTO_CIF_FERR1_CBG_GPKI_OVRFLW          (0x1 << 14)
#define   NFP_CRYPTO_CIF_FERR1_CBG_GPKI_OVRFLW_bf       0, 14, 14
#define   NFP_CRYPTO_CIF_FERR1_CBG_GPKI_OVRFLW_mask     (0x1)
#define   NFP_CRYPTO_CIF_FERR1_CBG_GPKI_OVRFLW_bit      (14)
#define   NFP_CRYPTO_CIF_FERR1_CBG_GBLK_UNDFLW          (0x1 << 13)
#define   NFP_CRYPTO_CIF_FERR1_CBG_GBLK_UNDFLW_bf       0, 13, 13
#define   NFP_CRYPTO_CIF_FERR1_CBG_GBLK_UNDFLW_mask     (0x1)
#define   NFP_CRYPTO_CIF_FERR1_CBG_GBLK_UNDFLW_bit      (13)
#define   NFP_CRYPTO_CIF_FERR1_CBG_GBLK_OVRFLW          (0x1 << 12)
#define   NFP_CRYPTO_CIF_FERR1_CBG_GBLK_OVRFLW_bf       0, 12, 12
#define   NFP_CRYPTO_CIF_FERR1_CBG_GBLK_OVRFLW_mask     (0x1)
#define   NFP_CRYPTO_CIF_FERR1_CBG_GBLK_OVRFLW_bit      (12)
#define   NFP_CRYPTO_CIF_FERR1_CBG_GCPP_UNDFLW          (0x1 << 11)
#define   NFP_CRYPTO_CIF_FERR1_CBG_GCPP_UNDFLW_bf       0, 11, 11
#define   NFP_CRYPTO_CIF_FERR1_CBG_GCPP_UNDFLW_mask     (0x1)
#define   NFP_CRYPTO_CIF_FERR1_CBG_GCPP_UNDFLW_bit      (11)
#define   NFP_CRYPTO_CIF_FERR1_CBG_GCPP_OVRFLW          (0x1 << 10)
#define   NFP_CRYPTO_CIF_FERR1_CBG_GCPP_OVRFLW_bf       0, 10, 10
#define   NFP_CRYPTO_CIF_FERR1_CBG_GCPP_OVRFLW_mask     (0x1)
#define   NFP_CRYPTO_CIF_FERR1_CBG_GCPP_OVRFLW_bit      (10)
#define   NFP_CRYPTO_CIF_FERR1_CBG_GDSP_UNDFLW          (0x1 << 9)
#define   NFP_CRYPTO_CIF_FERR1_CBG_GDSP_UNDFLW_bf       0, 9, 9
#define   NFP_CRYPTO_CIF_FERR1_CBG_GDSP_UNDFLW_mask     (0x1)
#define   NFP_CRYPTO_CIF_FERR1_CBG_GDSP_UNDFLW_bit      (9)
#define   NFP_CRYPTO_CIF_FERR1_CBG_GDSP_OVRFLW          (0x1 << 8)
#define   NFP_CRYPTO_CIF_FERR1_CBG_GDSP_OVRFLW_bf       0, 8, 8
#define   NFP_CRYPTO_CIF_FERR1_CBG_GDSP_OVRFLW_mask     (0x1)
#define   NFP_CRYPTO_CIF_FERR1_CBG_GDSP_OVRFLW_bit      (8)
#define   NFP_CRYPTO_CIF_FERR1_CDC_PKI_UNDFLW           (0x1 << 7)
#define   NFP_CRYPTO_CIF_FERR1_CDC_PKI_UNDFLW_bf        0, 7, 7
#define   NFP_CRYPTO_CIF_FERR1_CDC_PKI_UNDFLW_mask      (0x1)
#define   NFP_CRYPTO_CIF_FERR1_CDC_PKI_UNDFLW_bit       (7)
#define   NFP_CRYPTO_CIF_FERR1_CDC_PKI_OVRFLW           (0x1 << 6)
#define   NFP_CRYPTO_CIF_FERR1_CDC_PKI_OVRFLW_bf        0, 6, 6
#define   NFP_CRYPTO_CIF_FERR1_CDC_PKI_OVRFLW_mask      (0x1)
#define   NFP_CRYPTO_CIF_FERR1_CDC_PKI_OVRFLW_bit       (6)
#define   NFP_CRYPTO_CIF_FERR1_CDC_DSP_UNDFLW           (0x1 << 5)
#define   NFP_CRYPTO_CIF_FERR1_CDC_DSP_UNDFLW_bf        0, 5, 5
#define   NFP_CRYPTO_CIF_FERR1_CDC_DSP_UNDFLW_mask      (0x1)
#define   NFP_CRYPTO_CIF_FERR1_CDC_DSP_UNDFLW_bit       (5)
#define   NFP_CRYPTO_CIF_FERR1_CDC_DSP_OVRFLW           (0x1 << 4)
#define   NFP_CRYPTO_CIF_FERR1_CDC_DSP_OVRFLW_bf        0, 4, 4
#define   NFP_CRYPTO_CIF_FERR1_CDC_DSP_OVRFLW_mask      (0x1)
#define   NFP_CRYPTO_CIF_FERR1_CDC_DSP_OVRFLW_bit       (4)
#define   NFP_CRYPTO_CIF_FERR1_CPM_PUSH_UNDFLW          (0x1 << 3)
#define   NFP_CRYPTO_CIF_FERR1_CPM_PUSH_UNDFLW_bf       0, 3, 3
#define   NFP_CRYPTO_CIF_FERR1_CPM_PUSH_UNDFLW_mask     (0x1)
#define   NFP_CRYPTO_CIF_FERR1_CPM_PUSH_UNDFLW_bit      (3)
#define   NFP_CRYPTO_CIF_FERR1_CPM_PUSH_OVRFLW          (0x1 << 2)
#define   NFP_CRYPTO_CIF_FERR1_CPM_PUSH_OVRFLW_bf       0, 2, 2
#define   NFP_CRYPTO_CIF_FERR1_CPM_PUSH_OVRFLW_mask     (0x1)
#define   NFP_CRYPTO_CIF_FERR1_CPM_PUSH_OVRFLW_bit      (2)
#define   NFP_CRYPTO_CIF_FERR1_CPM_PULL_UNDFLW          (0x1 << 1)
#define   NFP_CRYPTO_CIF_FERR1_CPM_PULL_UNDFLW_bf       0, 1, 1
#define   NFP_CRYPTO_CIF_FERR1_CPM_PULL_UNDFLW_mask     (0x1)
#define   NFP_CRYPTO_CIF_FERR1_CPM_PULL_UNDFLW_bit      (1)
#define   NFP_CRYPTO_CIF_FERR1_CPM_PULL_OVRFLW          (0x1)
#define   NFP_CRYPTO_CIF_FERR1_CPM_PULL_OVRFLW_bf       0, 0, 0
#define   NFP_CRYPTO_CIF_FERR1_CPM_PULL_OVRFLW_mask     (0x1)
#define   NFP_CRYPTO_CIF_FERR1_CPM_PULL_OVRFLW_bit      (0)
/* Register Type: CIFErrFrst2 */
#define NFP_CRYPTO_CIF_FERR2           0x0014
#define   NFP_CRYPTO_CIF_FERR2_CTX_UNDFLW_bf            0, 31, 16
#define   NFP_CRYPTO_CIF_FERR2_CTX_UNDFLW_mask          (0xffff)
#define   NFP_CRYPTO_CIF_FERR2_CTX_UNDFLW_shift         (16)
#define   NFP_CRYPTO_CIF_FERR2_CTX_OVRFLW_bf            0, 15, 0
#define   NFP_CRYPTO_CIF_FERR2_CTX_OVRFLW_mask          (0xffff)
#define   NFP_CRYPTO_CIF_FERR2_CTX_OVRFLW_shift         (0)
/* Register Type: CIFErrFrst3 */
#define NFP_CRYPTO_CIF_FERR3           0x0018
#define   NFP_CRYPTO_CIF_FERR3_BLK_SYNC_ERR             (0x1 << 11)
#define   NFP_CRYPTO_CIF_FERR3_BLK_SYNC_ERR_bf          0, 11, 11
#define   NFP_CRYPTO_CIF_FERR3_BLK_SYNC_ERR_mask        (0x1)
#define   NFP_CRYPTO_CIF_FERR3_BLK_SYNC_ERR_bit         (11)
#define   NFP_CRYPTO_CIF_FERR3_DEC_ERR                  (0x1 << 10)
#define   NFP_CRYPTO_CIF_FERR3_DEC_ERR_bf               0, 10, 10
#define   NFP_CRYPTO_CIF_FERR3_DEC_ERR_mask             (0x1)
#define   NFP_CRYPTO_CIF_FERR3_DEC_ERR_bit              (10)
#define   NFP_CRYPTO_CIF_FERR3_CPP_PS_UNDFLW            (0x1 << 9)
#define   NFP_CRYPTO_CIF_FERR3_CPP_PS_UNDFLW_bf         0, 9, 9
#define   NFP_CRYPTO_CIF_FERR3_CPP_PS_UNDFLW_mask       (0x1)
#define   NFP_CRYPTO_CIF_FERR3_CPP_PS_UNDFLW_bit        (9)
#define   NFP_CRYPTO_CIF_FERR3_CPP_PS_OVRFLW            (0x1 << 8)
#define   NFP_CRYPTO_CIF_FERR3_CPP_PS_OVRFLW_bf         0, 8, 8
#define   NFP_CRYPTO_CIF_FERR3_CPP_PS_OVRFLW_mask       (0x1)
#define   NFP_CRYPTO_CIF_FERR3_CPP_PS_OVRFLW_bit        (8)
#define   NFP_CRYPTO_CIF_FERR3_CPP_DN_UNDFLW            (0x1 << 7)
#define   NFP_CRYPTO_CIF_FERR3_CPP_DN_UNDFLW_bf         0, 7, 7
#define   NFP_CRYPTO_CIF_FERR3_CPP_DN_UNDFLW_mask       (0x1)
#define   NFP_CRYPTO_CIF_FERR3_CPP_DN_UNDFLW_bit        (7)
#define   NFP_CRYPTO_CIF_FERR3_CPP_DN_OVRFLW            (0x1 << 6)
#define   NFP_CRYPTO_CIF_FERR3_CPP_DN_OVRFLW_bf         0, 6, 6
#define   NFP_CRYPTO_CIF_FERR3_CPP_DN_OVRFLW_mask       (0x1)
#define   NFP_CRYPTO_CIF_FERR3_CPP_DN_OVRFLW_bit        (6)
#define   NFP_CRYPTO_CIF_FERR3_CPP_PH_UNDFLW            (0x1 << 5)
#define   NFP_CRYPTO_CIF_FERR3_CPP_PH_UNDFLW_bf         0, 5, 5
#define   NFP_CRYPTO_CIF_FERR3_CPP_PH_UNDFLW_mask       (0x1)
#define   NFP_CRYPTO_CIF_FERR3_CPP_PH_UNDFLW_bit        (5)
#define   NFP_CRYPTO_CIF_FERR3_CPP_PH_OVRFLW            (0x1 << 4)
#define   NFP_CRYPTO_CIF_FERR3_CPP_PH_OVRFLW_bf         0, 4, 4
#define   NFP_CRYPTO_CIF_FERR3_CPP_PH_OVRFLW_mask       (0x1)
#define   NFP_CRYPTO_CIF_FERR3_CPP_PH_OVRFLW_bit        (4)
#define   NFP_CRYPTO_CIF_FERR3_CPP_DTA_ERR              (0x1 << 3)
#define   NFP_CRYPTO_CIF_FERR3_CPP_DTA_ERR_bf           0, 3, 3
#define   NFP_CRYPTO_CIF_FERR3_CPP_DTA_ERR_mask         (0x1)
#define   NFP_CRYPTO_CIF_FERR3_CPP_DTA_ERR_bit          (3)
#define   NFP_CRYPTO_CIF_FERR3_CMD_ERR                  (0x1 << 2)
#define   NFP_CRYPTO_CIF_FERR3_CMD_ERR_bf               0, 2, 2
#define   NFP_CRYPTO_CIF_FERR3_CMD_ERR_mask             (0x1)
#define   NFP_CRYPTO_CIF_FERR3_CMD_ERR_bit              (2)
#define   NFP_CRYPTO_CIF_FERR3_CBG_ERR_bf               0, 1, 0
#define   NFP_CRYPTO_CIF_FERR3_CBG_ERR_mask             (0x3)
#define   NFP_CRYPTO_CIF_FERR3_CBG_ERR_shift            (0)
/* Register Type: CIFErr1 */
#define NFP_CRYPTO_CIF_ERR1            0x0020
#define   NFP_CRYPTO_CIF_ERR1_CDS_ERR_bf                0, 31, 18
#define   NFP_CRYPTO_CIF_ERR1_CDS_ERR_mask              (0x3fff)
#define   NFP_CRYPTO_CIF_ERR1_CDS_ERR_shift             (18)
#define   NFP_CRYPTO_CIF_ERR1_CBG_ERR_bf                0, 17, 8
#define   NFP_CRYPTO_CIF_ERR1_CBG_ERR_mask              (0x3ff)
#define   NFP_CRYPTO_CIF_ERR1_CBG_ERR_shift             (8)
#define   NFP_CRYPTO_CIF_ERR1_CDC_ERR_bf                0, 7, 4
#define   NFP_CRYPTO_CIF_ERR1_CDC_ERR_mask              (0xf)
#define   NFP_CRYPTO_CIF_ERR1_CDC_ERR_shift             (4)
#define   NFP_CRYPTO_CIF_ERR1_CPM_ERR_bf                0, 3, 0
#define   NFP_CRYPTO_CIF_ERR1_CPM_ERR_mask              (0xf)
#define   NFP_CRYPTO_CIF_ERR1_CPM_ERR_shift             (0)
/* Register Type: CIFErr2 */
#define NFP_CRYPTO_CIF_ERR2            0x0024
#define   NFP_CRYPTO_CIF_ERR2_CTX_UNDFLW_bf             0, 31, 16
#define   NFP_CRYPTO_CIF_ERR2_CTX_UNDFLW_mask           (0xffff)
#define   NFP_CRYPTO_CIF_ERR2_CTX_UNDFLW_shift          (16)
#define   NFP_CRYPTO_CIF_ERR2_CTX_OVRFLW_bf             0, 15, 0
#define   NFP_CRYPTO_CIF_ERR2_CTX_OVRFLW_mask           (0xffff)
#define   NFP_CRYPTO_CIF_ERR2_CTX_OVRFLW_shift          (0)
/* Register Type: CIFErr3 */
#define NFP_CRYPTO_CIF_ERR3            0x0028
#define   NFP_CRYPTO_CIF_ERR3_BLK_SYNC_ERR              (0x1 << 11)
#define   NFP_CRYPTO_CIF_ERR3_BLK_SYNC_ERR_bf           0, 11, 11
#define   NFP_CRYPTO_CIF_ERR3_BLK_SYNC_ERR_mask         (0x1)
#define   NFP_CRYPTO_CIF_ERR3_BLK_SYNC_ERR_bit          (11)
#define   NFP_CRYPTO_CIF_ERR3_DEC_ERR                   (0x1 << 10)
#define   NFP_CRYPTO_CIF_ERR3_DEC_ERR_bf                0, 10, 10
#define   NFP_CRYPTO_CIF_ERR3_DEC_ERR_mask              (0x1)
#define   NFP_CRYPTO_CIF_ERR3_DEC_ERR_bit               (10)
#define   NFP_CRYPTO_CIF_ERR3_CPP_PS_UNDFLW             (0x1 << 9)
#define   NFP_CRYPTO_CIF_ERR3_CPP_PS_UNDFLW_bf          0, 9, 9
#define   NFP_CRYPTO_CIF_ERR3_CPP_PS_UNDFLW_mask        (0x1)
#define   NFP_CRYPTO_CIF_ERR3_CPP_PS_UNDFLW_bit         (9)
#define   NFP_CRYPTO_CIF_ERR3_CPP_PS_OVRFLW             (0x1 << 8)
#define   NFP_CRYPTO_CIF_ERR3_CPP_PS_OVRFLW_bf          0, 8, 8
#define   NFP_CRYPTO_CIF_ERR3_CPP_PS_OVRFLW_mask        (0x1)
#define   NFP_CRYPTO_CIF_ERR3_CPP_PS_OVRFLW_bit         (8)
#define   NFP_CRYPTO_CIF_ERR3_CPP_DN_UNDFLW             (0x1 << 7)
#define   NFP_CRYPTO_CIF_ERR3_CPP_DN_UNDFLW_bf          0, 7, 7
#define   NFP_CRYPTO_CIF_ERR3_CPP_DN_UNDFLW_mask        (0x1)
#define   NFP_CRYPTO_CIF_ERR3_CPP_DN_UNDFLW_bit         (7)
#define   NFP_CRYPTO_CIF_ERR3_CPP_DN_OVRFLW             (0x1 << 6)
#define   NFP_CRYPTO_CIF_ERR3_CPP_DN_OVRFLW_bf          0, 6, 6
#define   NFP_CRYPTO_CIF_ERR3_CPP_DN_OVRFLW_mask        (0x1)
#define   NFP_CRYPTO_CIF_ERR3_CPP_DN_OVRFLW_bit         (6)
#define   NFP_CRYPTO_CIF_ERR3_CPP_PH_UNDFLW             (0x1 << 5)
#define   NFP_CRYPTO_CIF_ERR3_CPP_PH_UNDFLW_bf          0, 5, 5
#define   NFP_CRYPTO_CIF_ERR3_CPP_PH_UNDFLW_mask        (0x1)
#define   NFP_CRYPTO_CIF_ERR3_CPP_PH_UNDFLW_bit         (5)
#define   NFP_CRYPTO_CIF_ERR3_CPP_PH_OVRFLW             (0x1 << 4)
#define   NFP_CRYPTO_CIF_ERR3_CPP_PH_OVRFLW_bf          0, 4, 4
#define   NFP_CRYPTO_CIF_ERR3_CPP_PH_OVRFLW_mask        (0x1)
#define   NFP_CRYPTO_CIF_ERR3_CPP_PH_OVRFLW_bit         (4)
#define   NFP_CRYPTO_CIF_ERR3_CPP_DTA_ERR               (0x1 << 3)
#define   NFP_CRYPTO_CIF_ERR3_CPP_DTA_ERR_bf            0, 3, 3
#define   NFP_CRYPTO_CIF_ERR3_CPP_DTA_ERR_mask          (0x1)
#define   NFP_CRYPTO_CIF_ERR3_CPP_DTA_ERR_bit           (3)
#define   NFP_CRYPTO_CIF_ERR3_CMD_ERR                   (0x1 << 2)
#define   NFP_CRYPTO_CIF_ERR3_CMD_ERR_bf                0, 2, 2
#define   NFP_CRYPTO_CIF_ERR3_CMD_ERR_mask              (0x1)
#define   NFP_CRYPTO_CIF_ERR3_CMD_ERR_bit               (2)
#define   NFP_CRYPTO_CIF_ERR3_CBG_ERR_bf                0, 1, 0
#define   NFP_CRYPTO_CIF_ERR3_CBG_ERR_mask              (0x3)
#define   NFP_CRYPTO_CIF_ERR3_CBG_ERR_shift             (0)
/* Register Type: CIFErrMask1 */
#define NFP_CRYPTO_CIF_ERRMASK1        0x0030
#define   NFP_CRYPTO_CIF_ERRMASK1_CDS_ERR_bf            0, 31, 18
#define   NFP_CRYPTO_CIF_ERRMASK1_CDS_ERR_mask          (0x3fff)
#define   NFP_CRYPTO_CIF_ERRMASK1_CDS_ERR_shift         (18)
#define   NFP_CRYPTO_CIF_ERRMASK1_CBG_ERR_bf            0, 17, 8
#define   NFP_CRYPTO_CIF_ERRMASK1_CBG_ERR_mask          (0x3ff)
#define   NFP_CRYPTO_CIF_ERRMASK1_CBG_ERR_shift         (8)
#define   NFP_CRYPTO_CIF_ERRMASK1_CDC_ERR_bf            0, 7, 4
#define   NFP_CRYPTO_CIF_ERRMASK1_CDC_ERR_mask          (0xf)
#define   NFP_CRYPTO_CIF_ERRMASK1_CDC_ERR_shift         (4)
#define   NFP_CRYPTO_CIF_ERRMASK1_CPM_ERR_bf            0, 3, 0
#define   NFP_CRYPTO_CIF_ERRMASK1_CPM_ERR_mask          (0xf)
#define   NFP_CRYPTO_CIF_ERRMASK1_CPM_ERR_shift         (0)
/* Register Type: CIFErrMask2 */
#define NFP_CRYPTO_CIF_ERRMASK2        0x0034
#define   NFP_CRYPTO_CIF_ERRMASK2_CTX_UNDFLW_bf         0, 31, 16
#define   NFP_CRYPTO_CIF_ERRMASK2_CTX_UNDFLW_mask       (0xffff)
#define   NFP_CRYPTO_CIF_ERRMASK2_CTX_UNDFLW_shift      (16)
#define   NFP_CRYPTO_CIF_ERRMASK2_CTX_OVRFLW_bf         0, 15, 0
#define   NFP_CRYPTO_CIF_ERRMASK2_CTX_OVRFLW_mask       (0xffff)
#define   NFP_CRYPTO_CIF_ERRMASK2_CTX_OVRFLW_shift      (0)
/* Register Type: CIFErrMask3 */
#define NFP_CRYPTO_CIF_ERRMASK3        0x0038
#define   NFP_CRYPTO_CIF_ERRMASK3_BLK_SYNC_ERR          (0x1 << 11)
#define   NFP_CRYPTO_CIF_ERRMASK3_BLK_SYNC_ERR_bf       0, 11, 11
#define   NFP_CRYPTO_CIF_ERRMASK3_BLK_SYNC_ERR_mask     (0x1)
#define   NFP_CRYPTO_CIF_ERRMASK3_BLK_SYNC_ERR_bit      (11)
#define   NFP_CRYPTO_CIF_ERRMASK3_DEC_ERR               (0x1 << 10)
#define   NFP_CRYPTO_CIF_ERRMASK3_DEC_ERR_bf            0, 10, 10
#define   NFP_CRYPTO_CIF_ERRMASK3_DEC_ERR_mask          (0x1)
#define   NFP_CRYPTO_CIF_ERRMASK3_DEC_ERR_bit           (10)
#define   NFP_CRYPTO_CIF_ERRMASK3_CPP_PS_UNDFLW         (0x1 << 9)
#define   NFP_CRYPTO_CIF_ERRMASK3_CPP_PS_UNDFLW_bf      0, 9, 9
#define   NFP_CRYPTO_CIF_ERRMASK3_CPP_PS_UNDFLW_mask    (0x1)
#define   NFP_CRYPTO_CIF_ERRMASK3_CPP_PS_UNDFLW_bit     (9)
#define   NFP_CRYPTO_CIF_ERRMASK3_CPP_PS_OVRFLW         (0x1 << 8)
#define   NFP_CRYPTO_CIF_ERRMASK3_CPP_PS_OVRFLW_bf      0, 8, 8
#define   NFP_CRYPTO_CIF_ERRMASK3_CPP_PS_OVRFLW_mask    (0x1)
#define   NFP_CRYPTO_CIF_ERRMASK3_CPP_PS_OVRFLW_bit     (8)
#define   NFP_CRYPTO_CIF_ERRMASK3_CPP_DN_UNDFLW         (0x1 << 7)
#define   NFP_CRYPTO_CIF_ERRMASK3_CPP_DN_UNDFLW_bf      0, 7, 7
#define   NFP_CRYPTO_CIF_ERRMASK3_CPP_DN_UNDFLW_mask    (0x1)
#define   NFP_CRYPTO_CIF_ERRMASK3_CPP_DN_UNDFLW_bit     (7)
#define   NFP_CRYPTO_CIF_ERRMASK3_CPP_DN_OVRFLW         (0x1 << 6)
#define   NFP_CRYPTO_CIF_ERRMASK3_CPP_DN_OVRFLW_bf      0, 6, 6
#define   NFP_CRYPTO_CIF_ERRMASK3_CPP_DN_OVRFLW_mask    (0x1)
#define   NFP_CRYPTO_CIF_ERRMASK3_CPP_DN_OVRFLW_bit     (6)
#define   NFP_CRYPTO_CIF_ERRMASK3_CPP_PH_UNDFLW         (0x1 << 5)
#define   NFP_CRYPTO_CIF_ERRMASK3_CPP_PH_UNDFLW_bf      0, 5, 5
#define   NFP_CRYPTO_CIF_ERRMASK3_CPP_PH_UNDFLW_mask    (0x1)
#define   NFP_CRYPTO_CIF_ERRMASK3_CPP_PH_UNDFLW_bit     (5)
#define   NFP_CRYPTO_CIF_ERRMASK3_CPP_PH_OVRFLW         (0x1 << 4)
#define   NFP_CRYPTO_CIF_ERRMASK3_CPP_PH_OVRFLW_bf      0, 4, 4
#define   NFP_CRYPTO_CIF_ERRMASK3_CPP_PH_OVRFLW_mask    (0x1)
#define   NFP_CRYPTO_CIF_ERRMASK3_CPP_PH_OVRFLW_bit     (4)
#define   NFP_CRYPTO_CIF_ERRMASK3_CPP_DTA_ERR           (0x1 << 3)
#define   NFP_CRYPTO_CIF_ERRMASK3_CPP_DTA_ERR_bf        0, 3, 3
#define   NFP_CRYPTO_CIF_ERRMASK3_CPP_DTA_ERR_mask      (0x1)
#define   NFP_CRYPTO_CIF_ERRMASK3_CPP_DTA_ERR_bit       (3)
#define   NFP_CRYPTO_CIF_ERRMASK3_CMD_ERR               (0x1 << 2)
#define   NFP_CRYPTO_CIF_ERRMASK3_CMD_ERR_bf            0, 2, 2
#define   NFP_CRYPTO_CIF_ERRMASK3_CMD_ERR_mask          (0x1)
#define   NFP_CRYPTO_CIF_ERRMASK3_CMD_ERR_bit           (2)
#define   NFP_CRYPTO_CIF_ERRMASK3_CBG_ERR_bf            0, 1, 0
#define   NFP_CRYPTO_CIF_ERRMASK3_CBG_ERR_mask          (0x3)
#define   NFP_CRYPTO_CIF_ERRMASK3_CBG_ERR_shift         (0)
/* Register Type: CIFErrClear1 */
#define NFP_CRYPTO_CIF_ERRCLR1         0x0040
#define   NFP_CRYPTO_CIF_ERRCLR1_CDS_ERR_bf             0, 31, 18
#define   NFP_CRYPTO_CIF_ERRCLR1_CDS_ERR_mask           (0x3fff)
#define   NFP_CRYPTO_CIF_ERRCLR1_CDS_ERR_shift          (18)
#define   NFP_CRYPTO_CIF_ERRCLR1_CBG_ERR_bf             0, 17, 8
#define   NFP_CRYPTO_CIF_ERRCLR1_CBG_ERR_mask           (0x3ff)
#define   NFP_CRYPTO_CIF_ERRCLR1_CBG_ERR_shift          (8)
#define   NFP_CRYPTO_CIF_ERRCLR1_CDC_ERR_bf             0, 7, 4
#define   NFP_CRYPTO_CIF_ERRCLR1_CDC_ERR_mask           (0xf)
#define   NFP_CRYPTO_CIF_ERRCLR1_CDC_ERR_shift          (4)
#define   NFP_CRYPTO_CIF_ERRCLR1_CPM_ERR_bf             0, 3, 0
#define   NFP_CRYPTO_CIF_ERRCLR1_CPM_ERR_mask           (0xf)
#define   NFP_CRYPTO_CIF_ERRCLR1_CPM_ERR_shift          (0)
/* Register Type: CIFErrClear2 */
#define NFP_CRYPTO_CIF_ERRCLR2         0x0044
#define   NFP_CRYPTO_CIF_ERRCLR2_CTX_UNDFLW(_x)         (((_x) & 0xffff) << 16)
#define   NFP_CRYPTO_CIF_ERRCLR2_CTX_UNDFLW_bf          0, 31, 16
#define   NFP_CRYPTO_CIF_ERRCLR2_CTX_UNDFLW_mask        (0xffff)
#define   NFP_CRYPTO_CIF_ERRCLR2_CTX_UNDFLW_shift       (16)
#define   NFP_CRYPTO_CIF_ERRCLR2_CTX_OVRFLW(_x)         ((_x) & 0xffff)
#define   NFP_CRYPTO_CIF_ERRCLR2_CTX_OVRFLW_bf          0, 15, 0
#define   NFP_CRYPTO_CIF_ERRCLR2_CTX_OVRFLW_mask        (0xffff)
#define   NFP_CRYPTO_CIF_ERRCLR2_CTX_OVRFLW_shift       (0)
/* Register Type: CIFErrClear3 */
#define NFP_CRYPTO_CIF_ERRCLR3         0x0048
#define   NFP_CRYPTO_CIF_ERRCLR3_BLK_SYNC_ERR           (0x1 << 11)
#define   NFP_CRYPTO_CIF_ERRCLR3_BLK_SYNC_ERR_bf        0, 11, 11
#define   NFP_CRYPTO_CIF_ERRCLR3_BLK_SYNC_ERR_mask      (0x1)
#define   NFP_CRYPTO_CIF_ERRCLR3_BLK_SYNC_ERR_bit       (11)
#define   NFP_CRYPTO_CIF_ERRCLR3_DEC_ERR                (0x1 << 10)
#define   NFP_CRYPTO_CIF_ERRCLR3_DEC_ERR_bf             0, 10, 10
#define   NFP_CRYPTO_CIF_ERRCLR3_DEC_ERR_mask           (0x1)
#define   NFP_CRYPTO_CIF_ERRCLR3_DEC_ERR_bit            (10)
#define   NFP_CRYPTO_CIF_ERRCLR3_CPP_PS_UNDFLW          (0x1 << 9)
#define   NFP_CRYPTO_CIF_ERRCLR3_CPP_PS_UNDFLW_bf       0, 9, 9
#define   NFP_CRYPTO_CIF_ERRCLR3_CPP_PS_UNDFLW_mask     (0x1)
#define   NFP_CRYPTO_CIF_ERRCLR3_CPP_PS_UNDFLW_bit      (9)
#define   NFP_CRYPTO_CIF_ERRCLR3_CPP_PS_OVRFLW          (0x1 << 8)
#define   NFP_CRYPTO_CIF_ERRCLR3_CPP_PS_OVRFLW_bf       0, 8, 8
#define   NFP_CRYPTO_CIF_ERRCLR3_CPP_PS_OVRFLW_mask     (0x1)
#define   NFP_CRYPTO_CIF_ERRCLR3_CPP_PS_OVRFLW_bit      (8)
#define   NFP_CRYPTO_CIF_ERRCLR3_CPP_DN_UNDFLW          (0x1 << 7)
#define   NFP_CRYPTO_CIF_ERRCLR3_CPP_DN_UNDFLW_bf       0, 7, 7
#define   NFP_CRYPTO_CIF_ERRCLR3_CPP_DN_UNDFLW_mask     (0x1)
#define   NFP_CRYPTO_CIF_ERRCLR3_CPP_DN_UNDFLW_bit      (7)
#define   NFP_CRYPTO_CIF_ERRCLR3_CPP_DN_OVRFLW          (0x1 << 6)
#define   NFP_CRYPTO_CIF_ERRCLR3_CPP_DN_OVRFLW_bf       0, 6, 6
#define   NFP_CRYPTO_CIF_ERRCLR3_CPP_DN_OVRFLW_mask     (0x1)
#define   NFP_CRYPTO_CIF_ERRCLR3_CPP_DN_OVRFLW_bit      (6)
#define   NFP_CRYPTO_CIF_ERRCLR3_CPP_PH_UNDFLW          (0x1 << 5)
#define   NFP_CRYPTO_CIF_ERRCLR3_CPP_PH_UNDFLW_bf       0, 5, 5
#define   NFP_CRYPTO_CIF_ERRCLR3_CPP_PH_UNDFLW_mask     (0x1)
#define   NFP_CRYPTO_CIF_ERRCLR3_CPP_PH_UNDFLW_bit      (5)
#define   NFP_CRYPTO_CIF_ERRCLR3_CPP_PH_OVRFLW          (0x1 << 4)
#define   NFP_CRYPTO_CIF_ERRCLR3_CPP_PH_OVRFLW_bf       0, 4, 4
#define   NFP_CRYPTO_CIF_ERRCLR3_CPP_PH_OVRFLW_mask     (0x1)
#define   NFP_CRYPTO_CIF_ERRCLR3_CPP_PH_OVRFLW_bit      (4)
#define   NFP_CRYPTO_CIF_ERRCLR3_CPP_DTA_ERR            (0x1 << 3)
#define   NFP_CRYPTO_CIF_ERRCLR3_CPP_DTA_ERR_bf         0, 3, 3
#define   NFP_CRYPTO_CIF_ERRCLR3_CPP_DTA_ERR_mask       (0x1)
#define   NFP_CRYPTO_CIF_ERRCLR3_CPP_DTA_ERR_bit        (3)
#define   NFP_CRYPTO_CIF_ERRCLR3_CMD_ERR                (0x1 << 2)
#define   NFP_CRYPTO_CIF_ERRCLR3_CMD_ERR_bf             0, 2, 2
#define   NFP_CRYPTO_CIF_ERRCLR3_CMD_ERR_mask           (0x1)
#define   NFP_CRYPTO_CIF_ERRCLR3_CMD_ERR_bit            (2)
#define   NFP_CRYPTO_CIF_ERRCLR3_CTX_ERR(_x)            ((_x) & 0x3)
#define   NFP_CRYPTO_CIF_ERRCLR3_CTX_ERR_bf             0, 1, 0
#define   NFP_CRYPTO_CIF_ERRCLR3_CTX_ERR_mask           (0x3)
#define   NFP_CRYPTO_CIF_ERRCLR3_CTX_ERR_shift          (0)
/* Register Type: CIFPerfSel */
#define NFP_CRYPTO_CIF_PERFSEL         0x0050
#define   NFP_CRYPTO_CIF_PERFSEL_SELECT(_x)             ((_x) & 0xf)
#define   NFP_CRYPTO_CIF_PERFSEL_SELECT_bf              0, 3, 0
#define   NFP_CRYPTO_CIF_PERFSEL_SELECT_mask            (0xf)
#define   NFP_CRYPTO_CIF_PERFSEL_SELECT_shift           (0)
#define     NFP_CRYPTO_CIF_PERFSEL_SELECT_NONE          (0)
#define     NFP_CRYPTO_CIF_PERFSEL_SELECT_FIFO          (1)
#define     NFP_CRYPTO_CIF_PERFSEL_SELECT_DISP          (2)
#define     NFP_CRYPTO_CIF_PERFSEL_SELECT_BUFF          (3)
#define     NFP_CRYPTO_CIF_PERFSEL_SELECT_CPPM          (4)
#define     NFP_CRYPTO_CIF_PERFSEL_SELECT_CPPT          (5)
/* Register Type: CIFDbgRdCtrl */
#define NFP_CRYPTO_CIF_DBGRC           0x0054
#define   NFP_CRYPTO_CIF_DBGRC_BUSY                     (0x1 << 31)
#define   NFP_CRYPTO_CIF_DBGRC_BUSY_bf                  0, 31, 31
#define   NFP_CRYPTO_CIF_DBGRC_BUSY_mask                (0x1)
#define   NFP_CRYPTO_CIF_DBGRC_BUSY_bit                 (31)
#define     NFP_CRYPTO_CIF_DBGRC_BUSY_READING           (0x80000000)
#define     NFP_CRYPTO_CIF_DBGRC_BUSY_READ_COMPLETE     (0x0)
#define   NFP_CRYPTO_CIF_DBGRC_SRC                      (0x1 << 16)
#define   NFP_CRYPTO_CIF_DBGRC_SRC_bf                   0, 16, 16
#define   NFP_CRYPTO_CIF_DBGRC_SRC_mask                 (0x1)
#define   NFP_CRYPTO_CIF_DBGRC_SRC_bit                  (16)
#define     NFP_CRYPTO_CIF_DBGRC_SRC_DISP               (0x0)
#define     NFP_CRYPTO_CIF_DBGRC_SRC_DMAC               (0x10000)
#define   NFP_CRYPTO_CIF_DBGRC_CNT(_x)                  ((_x) & 0xff)
#define   NFP_CRYPTO_CIF_DBGRC_CNT_bf                   0, 7, 0
#define   NFP_CRYPTO_CIF_DBGRC_CNT_mask                 (0xff)
#define   NFP_CRYPTO_CIF_DBGRC_CNT_shift                (0)
/* Register Type: CIFDbgRdAddr */
#define NFP_CRYPTO_CIF_DBGRA           0x0058
#define   NFP_CRYPTO_CIF_DBGRA_CRYPTO_ADDR(_x)          (((_x) & 0x1fff) << 16)
#define   NFP_CRYPTO_CIF_DBGRA_CRYPTO_ADDR_bf           0, 28, 16
#define   NFP_CRYPTO_CIF_DBGRA_CRYPTO_ADDR_mask         (0x1fff)
#define   NFP_CRYPTO_CIF_DBGRA_CRYPTO_ADDR_shift        (16)
#define   NFP_CRYPTO_CIF_DBGRA_CTX_INC                  (0x1 << 15)
#define   NFP_CRYPTO_CIF_DBGRA_CTX_INC_bf               0, 15, 15
#define   NFP_CRYPTO_CIF_DBGRA_CTX_INC_mask             (0x1)
#define   NFP_CRYPTO_CIF_DBGRA_CTX_INC_bit              (15)
#define     NFP_CRYPTO_CIF_DBGRA_CTX_INC_ENTRY          (0x0)
#define     NFP_CRYPTO_CIF_DBGRA_CTX_INC_FULL           (0x8000)
#define   NFP_CRYPTO_CIF_DBGRA_SRC_ADDR(_x)             ((_x) & 0xff)
#define   NFP_CRYPTO_CIF_DBGRA_SRC_ADDR_bf              0, 7, 0
#define   NFP_CRYPTO_CIF_DBGRA_SRC_ADDR_mask            (0xff)
#define   NFP_CRYPTO_CIF_DBGRA_SRC_ADDR_shift           (0)
/* Register Type: CIFDalBstRun */
#define NFP_CRYPTO_CIF_DALBSTRN        0x005c
#define   NFP_CRYPTO_CIF_DALBSTRN_RUN(_x)               ((_x) & 0x3)
#define   NFP_CRYPTO_CIF_DALBSTRN_RUN_bf                0, 1, 0
#define   NFP_CRYPTO_CIF_DALBSTRN_RUN_mask              (0x3)
#define   NFP_CRYPTO_CIF_DALBSTRN_RUN_shift             (0)
/* Register Type: CIFDalBstSt */
#define NFP_CRYPTO_CIF_DALBSTST        0x0060
#define   NFP_CRYPTO_CIF_DALBSTST_DONE(_x)              (((_x) & 0xf) << 4)
#define   NFP_CRYPTO_CIF_DALBSTST_DONE_bf               0, 7, 4
#define   NFP_CRYPTO_CIF_DALBSTST_DONE_mask             (0xf)
#define   NFP_CRYPTO_CIF_DALBSTST_DONE_shift            (4)
#define   NFP_CRYPTO_CIF_DALBSTST_FAIL(_x)              ((_x) & 0xf)
#define   NFP_CRYPTO_CIF_DALBSTST_FAIL_bf               0, 3, 0
#define   NFP_CRYPTO_CIF_DALBSTST_FAIL_mask             (0xf)
#define   NFP_CRYPTO_CIF_DALBSTST_FAIL_shift            (0)

/* HGID: nfp3200/crypto_pki.desc = a0ae124dec15 */
/* Register Type: MpuTmoutCtrl */
#define NFP_CRYPTO_PKI_TIMECTRL        0x0000
#define   NFP_CRYPTO_PKI_TIMECTRL_ENABLE_TIMEOUT        (0x1 << 16)
#define   NFP_CRYPTO_PKI_TIMECTRL_ENABLE_TIMEOUT_bf     0, 16, 16
#define   NFP_CRYPTO_PKI_TIMECTRL_ENABLE_TIMEOUT_mask   (0x1)
#define   NFP_CRYPTO_PKI_TIMECTRL_ENABLE_TIMEOUT_bit    (16)
#define   NFP_CRYPTO_PKI_TIMECTRL_TIMEOUT_LIMIT(_x)     ((_x) & 0xffff)
#define   NFP_CRYPTO_PKI_TIMECTRL_TIMEOUT_LIMIT_bf      0, 15, 0
#define   NFP_CRYPTO_PKI_TIMECTRL_TIMEOUT_LIMIT_mask    (0xffff)
#define   NFP_CRYPTO_PKI_TIMECTRL_TIMEOUT_LIMIT_shift   (0)
/* Register Type: MpuTmdOutMsk */
#define NFP_CRYPTO_PKI_TIMEDOUT        0x0004
#define   NFP_CRYPTO_PKI_TIMEDOUT_TIMED_OUT(_x)         ((_x) & 0xfff)
#define   NFP_CRYPTO_PKI_TIMEDOUT_TIMED_OUT_bf          0, 11, 0
#define   NFP_CRYPTO_PKI_TIMEDOUT_TIMED_OUT_mask        (0xfff)
#define   NFP_CRYPTO_PKI_TIMEDOUT_TIMED_OUT_shift       (0)
/* Register Type: MpuExcldMsk */
#define NFP_CRYPTO_PKI_EXCLUDE         0x0008
#define   NFP_CRYPTO_PKI_EXCLUDE_EXCLUDED(_x)           ((_x) & 0xfff)
#define   NFP_CRYPTO_PKI_EXCLUDE_EXCLUDED_bf            0, 11, 0
#define   NFP_CRYPTO_PKI_EXCLUDE_EXCLUDED_mask          (0xfff)
#define   NFP_CRYPTO_PKI_EXCLUDE_EXCLUDED_shift         (0)
/* Register Type: MpuRestart */
#define NFP_CRYPTO_PKI_RESTART         0x000c
#define   NFP_CRYPTO_PKI_RESTART_RESTART(_x)            ((_x) & 0xfff)
#define   NFP_CRYPTO_PKI_RESTART_RESTART_bf             0, 11, 0
#define   NFP_CRYPTO_PKI_RESTART_RESTART_mask           (0xfff)
#define   NFP_CRYPTO_PKI_RESTART_RESTART_shift          (0)
/* Register Type: MpuBusy */
#define NFP_CRYPTO_PKI_BUSY            0x0010
#define   NFP_CRYPTO_PKI_BUSY_SLOT_CNT_bf               0, 28, 24
#define   NFP_CRYPTO_PKI_BUSY_SLOT_CNT_mask             (0x1f)
#define   NFP_CRYPTO_PKI_BUSY_SLOT_CNT_shift            (24)
#define   NFP_CRYPTO_PKI_BUSY_FREE_bf                   0, 23, 12
#define   NFP_CRYPTO_PKI_BUSY_FREE_mask                 (0xfff)
#define   NFP_CRYPTO_PKI_BUSY_FREE_shift                (12)
#define   NFP_CRYPTO_PKI_BUSY_BUSY_bf                   0, 11, 0
#define   NFP_CRYPTO_PKI_BUSY_BUSY_mask                 (0xfff)
#define   NFP_CRYPTO_PKI_BUSY_BUSY_shift                (0)
/* Register Type: DbgMpuIndx */
#define NFP_CRYPTO_PKI_DBG             0x0014
#define   NFP_CRYPTO_PKI_DBG_BLK_IRM_FAULTS             (0x1 << 17)
#define   NFP_CRYPTO_PKI_DBG_BLK_IRM_FAULTS_bf          0, 17, 17
#define   NFP_CRYPTO_PKI_DBG_BLK_IRM_FAULTS_mask        (0x1)
#define   NFP_CRYPTO_PKI_DBG_BLK_IRM_FAULTS_bit         (17)
#define   NFP_CRYPTO_PKI_DBG_HALT_FAULT                 (0x1 << 16)
#define   NFP_CRYPTO_PKI_DBG_HALT_FAULT_bf              0, 16, 16
#define   NFP_CRYPTO_PKI_DBG_HALT_FAULT_mask            (0x1)
#define   NFP_CRYPTO_PKI_DBG_HALT_FAULT_bit             (16)
#define   NFP_CRYPTO_PKI_DBG_PERFBUS_SELECT(_x)         (((_x) & 0xff) << 8)
#define   NFP_CRYPTO_PKI_DBG_PERFBUS_SELECT_bf          0, 15, 8
#define   NFP_CRYPTO_PKI_DBG_PERFBUS_SELECT_mask        (0xff)
#define   NFP_CRYPTO_PKI_DBG_PERFBUS_SELECT_shift       (8)
#define   NFP_CRYPTO_PKI_DBG_ENABLE_IDX                 (0x1 << 7)
#define   NFP_CRYPTO_PKI_DBG_ENABLE_IDX_bf              0, 7, 7
#define   NFP_CRYPTO_PKI_DBG_ENABLE_IDX_mask            (0x1)
#define   NFP_CRYPTO_PKI_DBG_ENABLE_IDX_bit             (7)
#define   NFP_CRYPTO_PKI_DBG_DISABLE_POISON_CB          (0x1 << 6)
#define   NFP_CRYPTO_PKI_DBG_DISABLE_POISON_CB_bf       0, 6, 6
#define   NFP_CRYPTO_PKI_DBG_DISABLE_POISON_CB_mask     (0x1)
#define   NFP_CRYPTO_PKI_DBG_DISABLE_POISON_CB_bit      (6)
#define   NFP_CRYPTO_PKI_DBG_DISABLE_DST_SCRUBBING      (0x1 << 5)
#define   NFP_CRYPTO_PKI_DBG_DISABLE_DST_SCRUBBING_bf   0, 5, 5
#define   NFP_CRYPTO_PKI_DBG_DISABLE_DST_SCRUBBING_mask (0x1)
#define   NFP_CRYPTO_PKI_DBG_DISABLE_DST_SCRUBBING_bit  (5)
#define   NFP_CRYPTO_PKI_DBG_DISABLE_MPU_SCRUBBING      (0x1 << 4)
#define   NFP_CRYPTO_PKI_DBG_DISABLE_MPU_SCRUBBING_bf   0, 4, 4
#define   NFP_CRYPTO_PKI_DBG_DISABLE_MPU_SCRUBBING_mask (0x1)
#define   NFP_CRYPTO_PKI_DBG_DISABLE_MPU_SCRUBBING_bit  (4)
#define   NFP_CRYPTO_PKI_DBG_DBG_IDX(_x)                ((_x) & 0xf)
#define   NFP_CRYPTO_PKI_DBG_DBG_IDX_bf                 0, 3, 0
#define   NFP_CRYPTO_PKI_DBG_DBG_IDX_mask               (0xf)
#define   NFP_CRYPTO_PKI_DBG_DBG_IDX_shift              (0)
/* Register Type: PrgmCntr */
#define NFP_CRYPTO_PKI_PROGCNTR        0x0018
#define   NFP_CRYPTO_PKI_PROGCNTR_MEM_FAULY_bf          0, 31, 23
#define   NFP_CRYPTO_PKI_PROGCNTR_MEM_FAULY_mask        (0x1ff)
#define   NFP_CRYPTO_PKI_PROGCNTR_MEM_FAULY_shift       (23)
#define   NFP_CRYPTO_PKI_PROGCNTR_PROG_COUNT_bf         0, 8, 0
#define   NFP_CRYPTO_PKI_PROGCNTR_PROG_COUNT_mask       (0x1ff)
#define   NFP_CRYPTO_PKI_PROGCNTR_PROG_COUNT_shift      (0)
/* Register Type: CurrInstr */
#define NFP_CRYPTO_PKI_INSTR           0x001c
#define   NFP_CRYPTO_PKI_INSTR_CURR_INSTR_bf            0, 31, 0
#define   NFP_CRYPTO_PKI_INSTR_CURR_INSTR_mask          (0xffffffff)
#define   NFP_CRYPTO_PKI_INSTR_CURR_INSTR_shift         (0)
/* Register Type: MpuMemPkCtrl */
#define NFP_CRYPTO_PKI_MPUCTRL         0x0020
#define   NFP_CRYPTO_PKI_MPUCTRL_START_PEEK             (0x1 << 31)
#define   NFP_CRYPTO_PKI_MPUCTRL_START_PEEK_bf          0, 31, 31
#define   NFP_CRYPTO_PKI_MPUCTRL_START_PEEK_mask        (0x1)
#define   NFP_CRYPTO_PKI_MPUCTRL_START_PEEK_bit         (31)
#define     NFP_CRYPTO_PKI_MPUCTRL_START_PEEK_IN_PROGRESS (0x80000000)
#define     NFP_CRYPTO_PKI_MPUCTRL_START_PEEK_DONE      (0x0)
#define   NFP_CRYPTO_PKI_MPUCTRL_UNCORR_ERR             (0x1 << 25)
#define   NFP_CRYPTO_PKI_MPUCTRL_UNCORR_ERR_bf          0, 25, 25
#define   NFP_CRYPTO_PKI_MPUCTRL_UNCORR_ERR_mask        (0x1)
#define   NFP_CRYPTO_PKI_MPUCTRL_UNCORR_ERR_bit         (25)
#define   NFP_CRYPTO_PKI_MPUCTRL_CORR_ERR               (0x1 << 24)
#define   NFP_CRYPTO_PKI_MPUCTRL_CORR_ERR_bf            0, 24, 24
#define   NFP_CRYPTO_PKI_MPUCTRL_CORR_ERR_mask          (0x1)
#define   NFP_CRYPTO_PKI_MPUCTRL_CORR_ERR_bit           (24)
#define   NFP_CRYPTO_PKI_MPUCTRL_PEEK_ADDR(_x)          ((_x) & 0x1ff)
#define   NFP_CRYPTO_PKI_MPUCTRL_PEEK_ADDR_bf           0, 8, 0
#define   NFP_CRYPTO_PKI_MPUCTRL_PEEK_ADDR_mask         (0x1ff)
#define   NFP_CRYPTO_PKI_MPUCTRL_PEEK_ADDR_shift        (0)
/* Register Type: MpuMemPkData */
#define NFP_CRYPTO_PKI_MPUDATA         0x0024
#define   NFP_CRYPTO_PKI_MPUDATA_PEEK_DATA_bf           0, 31, 0
#define   NFP_CRYPTO_PKI_MPUDATA_PEEK_DATA_mask         (0xffffffff)
#define   NFP_CRYPTO_PKI_MPUDATA_PEEK_DATA_shift        (0)
/* Register Type: DstTblPkCtrl */
#define NFP_CRYPTO_PKI_DISTCTRL        0x0028
#define   NFP_CRYPTO_PKI_DISTCTRL_START_PEEK            (0x1 << 31)
#define   NFP_CRYPTO_PKI_DISTCTRL_START_PEEK_bf         0, 31, 31
#define   NFP_CRYPTO_PKI_DISTCTRL_START_PEEK_mask       (0x1)
#define   NFP_CRYPTO_PKI_DISTCTRL_START_PEEK_bit        (31)
#define     NFP_CRYPTO_PKI_DISTCTRL_START_PEEK_IN_PROGRESS (0x80000000)
#define     NFP_CRYPTO_PKI_DISTCTRL_START_PEEK_DONE     (0x0)
#define   NFP_CRYPTO_PKI_DISTCTRL_UNCORR_ERR            (0x1 << 25)
#define   NFP_CRYPTO_PKI_DISTCTRL_UNCORR_ERR_bf         0, 25, 25
#define   NFP_CRYPTO_PKI_DISTCTRL_UNCORR_ERR_mask       (0x1)
#define   NFP_CRYPTO_PKI_DISTCTRL_UNCORR_ERR_bit        (25)
#define   NFP_CRYPTO_PKI_DISTCTRL_CORR_ERR              (0x1 << 24)
#define   NFP_CRYPTO_PKI_DISTCTRL_CORR_ERR_bf           0, 24, 24
#define   NFP_CRYPTO_PKI_DISTCTRL_CORR_ERR_mask         (0x1)
#define   NFP_CRYPTO_PKI_DISTCTRL_CORR_ERR_bit          (24)
#define   NFP_CRYPTO_PKI_DISTCTRL_PEEK_ADDR(_x)         ((_x) & 0x3ff)
#define   NFP_CRYPTO_PKI_DISTCTRL_PEEK_ADDR_bf          0, 9, 0
#define   NFP_CRYPTO_PKI_DISTCTRL_PEEK_ADDR_mask        (0x3ff)
#define   NFP_CRYPTO_PKI_DISTCTRL_PEEK_ADDR_shift       (0)
/* Register Type: DstTblPkDat0 */
#define NFP_CRYPTO_PKI_DISTDATA0       0x002c
#define   NFP_CRYPTO_PKI_DISTDATA0_DATA_bf              0, 31, 0
#define   NFP_CRYPTO_PKI_DISTDATA0_DATA_mask            (0xffffffff)
#define   NFP_CRYPTO_PKI_DISTDATA0_DATA_shift           (0)
/* Register Type: DstTblPkDat1 */
#define NFP_CRYPTO_PKI_DISTDATA1       0x0030
#define   NFP_CRYPTO_PKI_DISTDATA1_DATA_bf              0, 31, 0
#define   NFP_CRYPTO_PKI_DISTDATA1_DATA_mask            (0xffffffff)
#define   NFP_CRYPTO_PKI_DISTDATA1_DATA_shift           (0)
/* Register Type: PwrsavDis */
#define NFP_CRYPTO_PKI_POWER           0x0034
#define   NFP_CRYPTO_PKI_POWER_DISABLE_POWER_SAVE(_x)   ((_x) & 0xfff)
#define   NFP_CRYPTO_PKI_POWER_DISABLE_POWER_SAVE_bf    0, 11, 0
#define   NFP_CRYPTO_PKI_POWER_DISABLE_POWER_SAVE_mask  (0xfff)
#define   NFP_CRYPTO_PKI_POWER_DISABLE_POWER_SAVE_shift (0)
/* Register Type: PkiErrFrst1 */
#define NFP_CRYPTO_PKI_FERR1           0x0040
#define   NFP_CRYPTO_PKI_FERR1_UNCORR_MEM               (0x1 << 31)
#define   NFP_CRYPTO_PKI_FERR1_UNCORR_MEM_bf            0, 31, 31
#define   NFP_CRYPTO_PKI_FERR1_UNCORR_MEM_mask          (0x1)
#define   NFP_CRYPTO_PKI_FERR1_UNCORR_MEM_bit           (31)
#define   NFP_CRYPTO_PKI_FERR1_REL_FREE                 (0x1 << 30)
#define   NFP_CRYPTO_PKI_FERR1_REL_FREE_bf              0, 30, 30
#define   NFP_CRYPTO_PKI_FERR1_REL_FREE_mask            (0x1)
#define   NFP_CRYPTO_PKI_FERR1_REL_FREE_bit             (30)
#define   NFP_CRYPTO_PKI_FERR1_ALLOC_NOT_FREE           (0x1 << 29)
#define   NFP_CRYPTO_PKI_FERR1_ALLOC_NOT_FREE_bf        0, 29, 29
#define   NFP_CRYPTO_PKI_FERR1_ALLOC_NOT_FREE_mask      (0x1)
#define   NFP_CRYPTO_PKI_FERR1_ALLOC_NOT_FREE_bit       (29)
#define   NFP_CRYPTO_PKI_FERR1_CLEANUP_NOT_DONE         (0x1 << 28)
#define   NFP_CRYPTO_PKI_FERR1_CLEANUP_NOT_DONE_bf      0, 28, 28
#define   NFP_CRYPTO_PKI_FERR1_CLEANUP_NOT_DONE_mask    (0x1)
#define   NFP_CRYPTO_PKI_FERR1_CLEANUP_NOT_DONE_bit     (28)
#define   NFP_CRYPTO_PKI_FERR1_POP_DONE                 (0x1 << 27)
#define   NFP_CRYPTO_PKI_FERR1_POP_DONE_bf              0, 27, 27
#define   NFP_CRYPTO_PKI_FERR1_POP_DONE_mask            (0x1)
#define   NFP_CRYPTO_PKI_FERR1_POP_DONE_bit             (27)
#define   NFP_CRYPTO_PKI_FERR1_PATH_CNT_UNDFLW          (0x1 << 26)
#define   NFP_CRYPTO_PKI_FERR1_PATH_CNT_UNDFLW_bf       0, 26, 26
#define   NFP_CRYPTO_PKI_FERR1_PATH_CNT_UNDFLW_mask     (0x1)
#define   NFP_CRYPTO_PKI_FERR1_PATH_CNT_UNDFLW_bit      (26)
#define   NFP_CRYPTO_PKI_FERR1_PATH_CNT_OVRFLW          (0x1 << 25)
#define   NFP_CRYPTO_PKI_FERR1_PATH_CNT_OVRFLW_bf       0, 25, 25
#define   NFP_CRYPTO_PKI_FERR1_PATH_CNT_OVRFLW_mask     (0x1)
#define   NFP_CRYPTO_PKI_FERR1_PATH_CNT_OVRFLW_bit      (25)
#define   NFP_CRYPTO_PKI_FERR1_XFER_NOREQ               (0x1 << 23)
#define   NFP_CRYPTO_PKI_FERR1_XFER_NOREQ_bf            0, 23, 23
#define   NFP_CRYPTO_PKI_FERR1_XFER_NOREQ_mask          (0x1)
#define   NFP_CRYPTO_PKI_FERR1_XFER_NOREQ_bit           (23)
#define   NFP_CRYPTO_PKI_FERR1_BAD_PKI_CBG              (0x1 << 21)
#define   NFP_CRYPTO_PKI_FERR1_BAD_PKI_CBG_bf           0, 21, 21
#define   NFP_CRYPTO_PKI_FERR1_BAD_PKI_CBG_mask         (0x1)
#define   NFP_CRYPTO_PKI_FERR1_BAD_PKI_CBG_bit          (21)
#define   NFP_CRYPTO_PKI_FERR1_ACK_NOREQ                (0x1 << 20)
#define   NFP_CRYPTO_PKI_FERR1_ACK_NOREQ_bf             0, 20, 20
#define   NFP_CRYPTO_PKI_FERR1_ACK_NOREQ_mask           (0x1)
#define   NFP_CRYPTO_PKI_FERR1_ACK_NOREQ_bit            (20)
#define   NFP_CRYPTO_PKI_FERR1_RMT_EXEC_NORET           (0x1 << 19)
#define   NFP_CRYPTO_PKI_FERR1_RMT_EXEC_NORET_bf        0, 19, 19
#define   NFP_CRYPTO_PKI_FERR1_RMT_EXEC_NORET_mask      (0x1)
#define   NFP_CRYPTO_PKI_FERR1_RMT_EXEC_NORET_bit       (19)
#define   NFP_CRYPTO_PKI_FERR1_RMT_EXEC_NOEXE           (0x1 << 18)
#define   NFP_CRYPTO_PKI_FERR1_RMT_EXEC_NOEXE_bf        0, 18, 18
#define   NFP_CRYPTO_PKI_FERR1_RMT_EXEC_NOEXE_mask      (0x1)
#define   NFP_CRYPTO_PKI_FERR1_RMT_EXEC_NOEXE_bit       (18)
#define   NFP_CRYPTO_PKI_FERR1_HEAD_GT_TAIL             (0x1 << 17)
#define   NFP_CRYPTO_PKI_FERR1_HEAD_GT_TAIL_bf          0, 17, 17
#define   NFP_CRYPTO_PKI_FERR1_HEAD_GT_TAIL_mask        (0x1)
#define   NFP_CRYPTO_PKI_FERR1_HEAD_GT_TAIL_bit         (17)
#define   NFP_CRYPTO_PKI_FERR1_HEAD_OVRFLW              (0x1 << 16)
#define   NFP_CRYPTO_PKI_FERR1_HEAD_OVRFLW_bf           0, 16, 16
#define   NFP_CRYPTO_PKI_FERR1_HEAD_OVRFLW_mask         (0x1)
#define   NFP_CRYPTO_PKI_FERR1_HEAD_OVRFLW_bit          (16)
#define   NFP_CRYPTO_PKI_FERR1_TAIL_OVERFLW             (0x1 << 15)
#define   NFP_CRYPTO_PKI_FERR1_TAIL_OVERFLW_bf          0, 15, 15
#define   NFP_CRYPTO_PKI_FERR1_TAIL_OVERFLW_mask        (0x1)
#define   NFP_CRYPTO_PKI_FERR1_TAIL_OVERFLW_bit         (15)
#define   NFP_CRYPTO_PKI_FERR1_UNDEF_RMT_INSTR          (0x1 << 14)
#define   NFP_CRYPTO_PKI_FERR1_UNDEF_RMT_INSTR_bf       0, 14, 14
#define   NFP_CRYPTO_PKI_FERR1_UNDEF_RMT_INSTR_mask     (0x1)
#define   NFP_CRYPTO_PKI_FERR1_UNDEF_RMT_INSTR_bit      (14)
#define   NFP_CRYPTO_PKI_FERR1_UNDEF_INSTR              (0x1 << 13)
#define   NFP_CRYPTO_PKI_FERR1_UNDEF_INSTR_bf           0, 13, 13
#define   NFP_CRYPTO_PKI_FERR1_UNDEF_INSTR_mask         (0x1)
#define   NFP_CRYPTO_PKI_FERR1_UNDEF_INSTR_bit          (13)
#define   NFP_CRYPTO_PKI_FERR1_MPU_ASSIGN_ERR           (0x1 << 12)
#define   NFP_CRYPTO_PKI_FERR1_MPU_ASSIGN_ERR_bf        0, 12, 12
#define   NFP_CRYPTO_PKI_FERR1_MPU_ASSIGN_ERR_mask      (0x1)
#define   NFP_CRYPTO_PKI_FERR1_MPU_ASSIGN_ERR_bit       (12)
#define   NFP_CRYPTO_PKI_FERR1_CDS_REP_UNDFLW           (0x1 << 11)
#define   NFP_CRYPTO_PKI_FERR1_CDS_REP_UNDFLW_bf        0, 11, 11
#define   NFP_CRYPTO_PKI_FERR1_CDS_REP_UNDFLW_mask      (0x1)
#define   NFP_CRYPTO_PKI_FERR1_CDS_REP_UNDFLW_bit       (11)
#define   NFP_CRYPTO_PKI_FERR1_CDS_REP_OVRFLW           (0x1 << 10)
#define   NFP_CRYPTO_PKI_FERR1_CDS_REP_OVRFLW_bf        0, 10, 10
#define   NFP_CRYPTO_PKI_FERR1_CDS_REP_OVRFLW_mask      (0x1)
#define   NFP_CRYPTO_PKI_FERR1_CDS_REP_OVRFLW_bit       (10)
#define   NFP_CRYPTO_PKI_FERR1_DMA_REQ_UNDFLW           (0x1 << 9)
#define   NFP_CRYPTO_PKI_FERR1_DMA_REQ_UNDFLW_bf        0, 9, 9
#define   NFP_CRYPTO_PKI_FERR1_DMA_REQ_UNDFLW_mask      (0x1)
#define   NFP_CRYPTO_PKI_FERR1_DMA_REQ_UNDFLW_bit       (9)
#define   NFP_CRYPTO_PKI_FERR1_DMA_REQ_OVRFLW           (0x1 << 8)
#define   NFP_CRYPTO_PKI_FERR1_DMA_REQ_OVRFLW_bf        0, 8, 8
#define   NFP_CRYPTO_PKI_FERR1_DMA_REQ_OVRFLW_mask      (0x1)
#define   NFP_CRYPTO_PKI_FERR1_DMA_REQ_OVRFLW_bit       (8)
#define   NFP_CRYPTO_PKI_FERR1_SIG_REQ_UNDFLW           (0x1 << 7)
#define   NFP_CRYPTO_PKI_FERR1_SIG_REQ_UNDFLW_bf        0, 7, 7
#define   NFP_CRYPTO_PKI_FERR1_SIG_REQ_UNDFLW_mask      (0x1)
#define   NFP_CRYPTO_PKI_FERR1_SIG_REQ_UNDFLW_bit       (7)
#define   NFP_CRYPTO_PKI_FERR1_SIG_REQ_OVRFLW           (0x1 << 6)
#define   NFP_CRYPTO_PKI_FERR1_SIG_REQ_OVRFLW_bf        0, 6, 6
#define   NFP_CRYPTO_PKI_FERR1_SIG_REQ_OVRFLW_mask      (0x1)
#define   NFP_CRYPTO_PKI_FERR1_SIG_REQ_OVRFLW_bit       (6)
#define   NFP_CRYPTO_PKI_FERR1_EVT_REQ_UNDFLW           (0x1 << 5)
#define   NFP_CRYPTO_PKI_FERR1_EVT_REQ_UNDFLW_bf        0, 5, 5
#define   NFP_CRYPTO_PKI_FERR1_EVT_REQ_UNDFLW_mask      (0x1)
#define   NFP_CRYPTO_PKI_FERR1_EVT_REQ_UNDFLW_bit       (5)
#define   NFP_CRYPTO_PKI_FERR1_EVT_REQ_OVRFLW           (0x1 << 4)
#define   NFP_CRYPTO_PKI_FERR1_EVT_REQ_OVRFLW_bf        0, 4, 4
#define   NFP_CRYPTO_PKI_FERR1_EVT_REQ_OVRFLW_mask      (0x1)
#define   NFP_CRYPTO_PKI_FERR1_EVT_REQ_OVRFLW_bit       (4)
#define   NFP_CRYPTO_PKI_FERR1_PROG_UNDFLW              (0x1 << 3)
#define   NFP_CRYPTO_PKI_FERR1_PROG_UNDFLW_bf           0, 3, 3
#define   NFP_CRYPTO_PKI_FERR1_PROG_UNDFLW_mask         (0x1)
#define   NFP_CRYPTO_PKI_FERR1_PROG_UNDFLW_bit          (3)
#define   NFP_CRYPTO_PKI_FERR1_PROG_OVRFLW              (0x1 << 2)
#define   NFP_CRYPTO_PKI_FERR1_PROG_OVRFLW_bf           0, 2, 2
#define   NFP_CRYPTO_PKI_FERR1_PROG_OVRFLW_mask         (0x1)
#define   NFP_CRYPTO_PKI_FERR1_PROG_OVRFLW_bit          (2)
#define   NFP_CRYPTO_PKI_FERR1_CBD_UNDFLW               (0x1 << 1)
#define   NFP_CRYPTO_PKI_FERR1_CBD_UNDFLW_bf            0, 1, 1
#define   NFP_CRYPTO_PKI_FERR1_CBD_UNDFLW_mask          (0x1)
#define   NFP_CRYPTO_PKI_FERR1_CBD_UNDFLW_bit           (1)
#define   NFP_CRYPTO_PKI_FERR1_CBD_OVRFLW               (0x1)
#define   NFP_CRYPTO_PKI_FERR1_CBD_OVRFLW_bf            0, 0, 0
#define   NFP_CRYPTO_PKI_FERR1_CBD_OVRFLW_mask          (0x1)
#define   NFP_CRYPTO_PKI_FERR1_CBD_OVRFLW_bit           (0)
/* Register Type: PkiErr1 */
#define NFP_CRYPTO_PKI_ERR1            0x0044
#define   NFP_CRYPTO_PKI_ERR1_DIST_ERR_bf               0, 31, 0
#define   NFP_CRYPTO_PKI_ERR1_DIST_ERR_mask             (0xffffffff)
#define   NFP_CRYPTO_PKI_ERR1_DIST_ERR_shift            (0)
/* Register Type: PkiErrMsk1 */
#define NFP_CRYPTO_PKI_ERRMASK1        0x0048
#define   NFP_CRYPTO_PKI_ERRMASK1_DIST_ERR_MASK(_x)     (_x)
#define   NFP_CRYPTO_PKI_ERRMASK1_DIST_ERR_MASK_bf      0, 31, 0
#define   NFP_CRYPTO_PKI_ERRMASK1_DIST_ERR_MASK_mask    (0xffffffff)
#define   NFP_CRYPTO_PKI_ERRMASK1_DIST_ERR_MASK_shift   (0)
/* Register Type: PkiErrClr1 */
#define NFP_CRYPTO_PKI_ERRCLR1         0x004c
#define   NFP_CRYPTO_PKI_ERRCLR1_DIST_ERR_CLR(_x)       (_x)
#define   NFP_CRYPTO_PKI_ERRCLR1_DIST_ERR_CLR_bf        0, 31, 0
#define   NFP_CRYPTO_PKI_ERRCLR1_DIST_ERR_CLR_mask      (0xffffffff)
#define   NFP_CRYPTO_PKI_ERRCLR1_DIST_ERR_CLR_shift     (0)
/* Register Type: BistRun */
#define NFP_CRYPTO_PKI_BISTRUN         0x0050
#define   NFP_CRYPTO_PKI_BISTRUN_BIST_RUN               (0x1)
#define   NFP_CRYPTO_PKI_BISTRUN_BIST_RUN_bf            0, 0, 0
#define   NFP_CRYPTO_PKI_BISTRUN_BIST_RUN_mask          (0x1)
#define   NFP_CRYPTO_PKI_BISTRUN_BIST_RUN_bit           (0)
/* Register Type: BistStatus */
#define NFP_CRYPTO_PKI_BISTSTAT        0x0054
#define   NFP_CRYPTO_PKI_BISTSTAT_BIST_DONE1            (0x1 << 3)
#define   NFP_CRYPTO_PKI_BISTSTAT_BIST_DONE1_bf         0, 3, 3
#define   NFP_CRYPTO_PKI_BISTSTAT_BIST_DONE1_mask       (0x1)
#define   NFP_CRYPTO_PKI_BISTSTAT_BIST_DONE1_bit        (3)
#define   NFP_CRYPTO_PKI_BISTSTAT_BIST_DONE0            (0x1 << 2)
#define   NFP_CRYPTO_PKI_BISTSTAT_BIST_DONE0_bf         0, 2, 2
#define   NFP_CRYPTO_PKI_BISTSTAT_BIST_DONE0_mask       (0x1)
#define   NFP_CRYPTO_PKI_BISTSTAT_BIST_DONE0_bit        (2)
#define   NFP_CRYPTO_PKI_BISTSTAT_BIST_FL_DONE1         (0x1 << 1)
#define   NFP_CRYPTO_PKI_BISTSTAT_BIST_FL_DONE1_bf      0, 1, 1
#define   NFP_CRYPTO_PKI_BISTSTAT_BIST_FL_DONE1_mask    (0x1)
#define   NFP_CRYPTO_PKI_BISTSTAT_BIST_FL_DONE1_bit     (1)
#define   NFP_CRYPTO_PKI_BISTSTAT_BIST_FL_DONE0         (0x1)
#define   NFP_CRYPTO_PKI_BISTSTAT_BIST_FL_DONE0_bf      0, 0, 0
#define   NFP_CRYPTO_PKI_BISTSTAT_BIST_FL_DONE0_mask    (0x1)
#define   NFP_CRYPTO_PKI_BISTSTAT_BIST_FL_DONE0_bit     (0)

/* HGID: nfp3200/crypto_bulk.desc = a0ae124dec15 */
/* Register Type: BasicConfig */
#define NFP_CRYPTO_BULK_CFG            0x0000
#define   NFP_CRYPTO_BULK_CFG_DES_MCP(_x)               (((_x) & 0x3) << 4)
#define   NFP_CRYPTO_BULK_CFG_DES_MCP_bf                0, 5, 4
#define   NFP_CRYPTO_BULK_CFG_DES_MCP_mask              (0x3)
#define   NFP_CRYPTO_BULK_CFG_DES_MCP_shift             (4)
#define     NFP_CRYPTO_BULK_CFG_DES_MCP_5X              (0)
#define     NFP_CRYPTO_BULK_CFG_DES_MCP_5               (1)
#define     NFP_CRYPTO_BULK_CFG_DES_MCP_4               (2)
#define     NFP_CRYPTO_BULK_CFG_DES_MCP_6               (3)
#define   NFP_CRYPTO_BULK_CFG_ENABLE(_x)                ((_x) & 0xf)
#define   NFP_CRYPTO_BULK_CFG_ENABLE_bf                 0, 3, 0
#define   NFP_CRYPTO_BULK_CFG_ENABLE_mask               (0xf)
#define   NFP_CRYPTO_BULK_CFG_ENABLE_shift              (0)
/* Register Type: PerfBusSel */
#define NFP_CRYPTO_BULK_PERF_BUS_SEL   0x0004
#define   NFP_CRYPTO_BULK_PERF_BUS_SEL_HI_BLK(_x)       (((_x) & 0xf) << 20)
#define   NFP_CRYPTO_BULK_PERF_BUS_SEL_HI_BLK_bf        0, 23, 20
#define   NFP_CRYPTO_BULK_PERF_BUS_SEL_HI_BLK_mask      (0xf)
#define   NFP_CRYPTO_BULK_PERF_BUS_SEL_HI_BLK_shift     (20)
#define     NFP_CRYPTO_BULK_PERF_BUS_SEL_HI_BLK_DIRECTOR (8)
#define     NFP_CRYPTO_BULK_PERF_BUS_SEL_HI_BLK_ENG1    (12)
#define     NFP_CRYPTO_BULK_PERF_BUS_SEL_HI_BLK_ENG2    (13)
#define     NFP_CRYPTO_BULK_PERF_BUS_SEL_HI_BLK_ENG3    (14)
#define     NFP_CRYPTO_BULK_PERF_BUS_SEL_HI_BLK_ENG4    (15)
#define   NFP_CRYPTO_BULK_PERF_BUS_SEL_MID_BLK(_x)      (((_x) & 0xf) << 16)
#define   NFP_CRYPTO_BULK_PERF_BUS_SEL_MID_BLK_bf       0, 19, 16
#define   NFP_CRYPTO_BULK_PERF_BUS_SEL_MID_BLK_mask     (0xf)
#define   NFP_CRYPTO_BULK_PERF_BUS_SEL_MID_BLK_shift    (16)
#define     NFP_CRYPTO_BULK_PERF_BUS_SEL_MID_BLK_DIRECTOR (8)
#define     NFP_CRYPTO_BULK_PERF_BUS_SEL_MID_BLK_ENG0   (12)
#define     NFP_CRYPTO_BULK_PERF_BUS_SEL_MID_BLK_ENG1   (13)
#define     NFP_CRYPTO_BULK_PERF_BUS_SEL_MID_BLK_ENG2   (14)
#define     NFP_CRYPTO_BULK_PERF_BUS_SEL_MID_BLK_ENG3   (15)
#define   NFP_CRYPTO_BULK_PERF_BUS_SEL_LO_BLK(_x)       (((_x) & 0xf) << 12)
#define   NFP_CRYPTO_BULK_PERF_BUS_SEL_LO_BLK_bf        0, 15, 12
#define   NFP_CRYPTO_BULK_PERF_BUS_SEL_LO_BLK_mask      (0xf)
#define   NFP_CRYPTO_BULK_PERF_BUS_SEL_LO_BLK_shift     (12)
#define     NFP_CRYPTO_BULK_PERF_BUS_SEL_LO_BLK_DIRECTOR (8)
#define     NFP_CRYPTO_BULK_PERF_BUS_SEL_LO_BLK_ENG0    (12)
#define     NFP_CRYPTO_BULK_PERF_BUS_SEL_LO_BLK_ENG1    (13)
#define     NFP_CRYPTO_BULK_PERF_BUS_SEL_LO_BLK_ENG2    (14)
#define     NFP_CRYPTO_BULK_PERF_BUS_SEL_LO_BLK_ENG3    (15)
#define   NFP_CRYPTO_BULK_PERF_BUS_SEL_HI_SEL(_x)       (((_x) & 0xf) << 8)
#define   NFP_CRYPTO_BULK_PERF_BUS_SEL_HI_SEL_bf        0, 11, 8
#define   NFP_CRYPTO_BULK_PERF_BUS_SEL_HI_SEL_mask      (0xf)
#define   NFP_CRYPTO_BULK_PERF_BUS_SEL_HI_SEL_shift     (8)
#define   NFP_CRYPTO_BULK_PERF_BUS_SEL_MID_SEL(_x)      (((_x) & 0xf) << 4)
#define   NFP_CRYPTO_BULK_PERF_BUS_SEL_MID_SEL_bf       0, 7, 4
#define   NFP_CRYPTO_BULK_PERF_BUS_SEL_MID_SEL_mask     (0xf)
#define   NFP_CRYPTO_BULK_PERF_BUS_SEL_MID_SEL_shift    (4)
#define   NFP_CRYPTO_BULK_PERF_BUS_SEL_LO_SEL(_x)       ((_x) & 0xf)
#define   NFP_CRYPTO_BULK_PERF_BUS_SEL_LO_SEL_bf        0, 3, 0
#define   NFP_CRYPTO_BULK_PERF_BUS_SEL_LO_SEL_mask      (0xf)
#define   NFP_CRYPTO_BULK_PERF_BUS_SEL_LO_SEL_shift     (0)
/* Register Type: WtchDgTmrCnf */
#define NFP_CRYPTO_BULK_WATCHDOG_TIMER_CFG 0x0008
#define   NFP_CRYPTO_BULK_WATCHDOG_TIMER_CFG_ENABLE     (0x1 << 30)
#define   NFP_CRYPTO_BULK_WATCHDOG_TIMER_CFG_ENABLE_bf  0, 30, 30
#define   NFP_CRYPTO_BULK_WATCHDOG_TIMER_CFG_ENABLE_mask (0x1)
#define   NFP_CRYPTO_BULK_WATCHDOG_TIMER_CFG_ENABLE_bit (30)
#define   NFP_CRYPTO_BULK_WATCHDOG_TIMER_CFG_SIZE(_x)   (((_x) & 0x3) << 28)
#define   NFP_CRYPTO_BULK_WATCHDOG_TIMER_CFG_SIZE_bf    0, 29, 28
#define   NFP_CRYPTO_BULK_WATCHDOG_TIMER_CFG_SIZE_mask  (0x3)
#define   NFP_CRYPTO_BULK_WATCHDOG_TIMER_CFG_SIZE_shift (28)
#define     NFP_CRYPTO_BULK_WATCHDOG_TIMER_CFG_SIZE_28  (0)
#define     NFP_CRYPTO_BULK_WATCHDOG_TIMER_CFG_SIZE_32  (1)
#define     NFP_CRYPTO_BULK_WATCHDOG_TIMER_CFG_SIZE_36  (2)
#define     NFP_CRYPTO_BULK_WATCHDOG_TIMER_CFG_SIZE_40  (3)
#define   NFP_CRYPTO_BULK_WATCHDOG_TIMER_CFG_TRIP(_x)   ((_x) & 0xfffffff)
#define   NFP_CRYPTO_BULK_WATCHDOG_TIMER_CFG_TRIP_bf    0, 27, 0
#define   NFP_CRYPTO_BULK_WATCHDOG_TIMER_CFG_TRIP_mask  (0xfffffff)
#define   NFP_CRYPTO_BULK_WATCHDOG_TIMER_CFG_TRIP_shift (0)
/* Register Type: ErrorReg */
#define NFP_CRYPTO_BULK_ERR            0x0010
#define   NFP_CRYPTO_BULK_ERR_ENG3                      (0x1 << 22)
#define   NFP_CRYPTO_BULK_ERR_ENG3_bf                   0, 22, 22
#define   NFP_CRYPTO_BULK_ERR_ENG3_mask                 (0x1)
#define   NFP_CRYPTO_BULK_ERR_ENG3_bit                  (22)
#define   NFP_CRYPTO_BULK_ERR_ENG2                      (0x1 << 21)
#define   NFP_CRYPTO_BULK_ERR_ENG2_bf                   0, 21, 21
#define   NFP_CRYPTO_BULK_ERR_ENG2_mask                 (0x1)
#define   NFP_CRYPTO_BULK_ERR_ENG2_bit                  (21)
#define   NFP_CRYPTO_BULK_ERR_ENG1                      (0x1 << 20)
#define   NFP_CRYPTO_BULK_ERR_ENG1_bf                   0, 20, 20
#define   NFP_CRYPTO_BULK_ERR_ENG1_mask                 (0x1)
#define   NFP_CRYPTO_BULK_ERR_ENG1_bit                  (20)
#define   NFP_CRYPTO_BULK_ERR_ENG0                      (0x1 << 19)
#define   NFP_CRYPTO_BULK_ERR_ENG0_bf                   0, 19, 19
#define   NFP_CRYPTO_BULK_ERR_ENG0_mask                 (0x1)
#define   NFP_CRYPTO_BULK_ERR_ENG0_bit                  (19)
#define   NFP_CRYPTO_BULK_ERR_SEQNUM                    (0x1 << 18)
#define   NFP_CRYPTO_BULK_ERR_SEQNUM_bf                 0, 18, 18
#define   NFP_CRYPTO_BULK_ERR_SEQNUM_mask               (0x1)
#define   NFP_CRYPTO_BULK_ERR_SEQNUM_bit                (18)
#define   NFP_CRYPTO_BULK_ERR_FIFO_READY                (0x1 << 17)
#define   NFP_CRYPTO_BULK_ERR_FIFO_READY_bf             0, 17, 17
#define   NFP_CRYPTO_BULK_ERR_FIFO_READY_mask           (0x1)
#define   NFP_CRYPTO_BULK_ERR_FIFO_READY_bit            (17)
#define   NFP_CRYPTO_BULK_ERR_FIFO_PARITY               (0x1 << 16)
#define   NFP_CRYPTO_BULK_ERR_FIFO_PARITY_bf            0, 16, 16
#define   NFP_CRYPTO_BULK_ERR_FIFO_PARITY_mask          (0x1)
#define   NFP_CRYPTO_BULK_ERR_FIFO_PARITY_bit           (16)
#define   NFP_CRYPTO_BULK_ERR_FIFO_OVRFLW               (0x1 << 15)
#define   NFP_CRYPTO_BULK_ERR_FIFO_OVRFLW_bf            0, 15, 15
#define   NFP_CRYPTO_BULK_ERR_FIFO_OVRFLW_mask          (0x1)
#define   NFP_CRYPTO_BULK_ERR_FIFO_OVRFLW_bit           (15)
#define   NFP_CRYPTO_BULK_ERR_WRT_FIFO_PARITY           (0x1 << 14)
#define   NFP_CRYPTO_BULK_ERR_WRT_FIFO_PARITY_bf        0, 14, 14
#define   NFP_CRYPTO_BULK_ERR_WRT_FIFO_PARITY_mask      (0x1)
#define   NFP_CRYPTO_BULK_ERR_WRT_FIFO_PARITY_bit       (14)
#define   NFP_CRYPTO_BULK_ERR_WRT_FIFO_OVRFLW           (0x1 << 13)
#define   NFP_CRYPTO_BULK_ERR_WRT_FIFO_OVRFLW_bf        0, 13, 13
#define   NFP_CRYPTO_BULK_ERR_WRT_FIFO_OVRFLW_mask      (0x1)
#define   NFP_CRYPTO_BULK_ERR_WRT_FIFO_OVRFLW_bit       (13)
#define   NFP_CRYPTO_BULK_ERR_WRT_ARB                   (0x1 << 12)
#define   NFP_CRYPTO_BULK_ERR_WRT_ARB_bf                0, 12, 12
#define   NFP_CRYPTO_BULK_ERR_WRT_ARB_mask              (0x1)
#define   NFP_CRYPTO_BULK_ERR_WRT_ARB_bit               (12)
#define   NFP_CRYPTO_BULK_ERR_ALLOC_WATCHDOG3           (0x1 << 11)
#define   NFP_CRYPTO_BULK_ERR_ALLOC_WATCHDOG3_bf        0, 11, 11
#define   NFP_CRYPTO_BULK_ERR_ALLOC_WATCHDOG3_mask      (0x1)
#define   NFP_CRYPTO_BULK_ERR_ALLOC_WATCHDOG3_bit       (11)
#define   NFP_CRYPTO_BULK_ERR_ALLOC_WATCHDOG2           (0x1 << 10)
#define   NFP_CRYPTO_BULK_ERR_ALLOC_WATCHDOG2_bf        0, 10, 10
#define   NFP_CRYPTO_BULK_ERR_ALLOC_WATCHDOG2_mask      (0x1)
#define   NFP_CRYPTO_BULK_ERR_ALLOC_WATCHDOG2_bit       (10)
#define   NFP_CRYPTO_BULK_ERR_ALLOC_WATCHDOG1           (0x1 << 9)
#define   NFP_CRYPTO_BULK_ERR_ALLOC_WATCHDOG1_bf        0, 9, 9
#define   NFP_CRYPTO_BULK_ERR_ALLOC_WATCHDOG1_mask      (0x1)
#define   NFP_CRYPTO_BULK_ERR_ALLOC_WATCHDOG1_bit       (9)
#define   NFP_CRYPTO_BULK_ERR_ALLOC_WATCHDOG0           (0x1 << 8)
#define   NFP_CRYPTO_BULK_ERR_ALLOC_WATCHDOG0_bf        0, 8, 8
#define   NFP_CRYPTO_BULK_ERR_ALLOC_WATCHDOG0_mask      (0x1)
#define   NFP_CRYPTO_BULK_ERR_ALLOC_WATCHDOG0_bit       (8)
#define   NFP_CRYPTO_BULK_ERR_ALLOC_ARB                 (0x1 << 7)
#define   NFP_CRYPTO_BULK_ERR_ALLOC_ARB_bf              0, 7, 7
#define   NFP_CRYPTO_BULK_ERR_ALLOC_ARB_mask            (0x1)
#define   NFP_CRYPTO_BULK_ERR_ALLOC_ARB_bit             (7)
#define   NFP_CRYPTO_BULK_ERR_ALLOC_FIFO_PARITY         (0x1 << 6)
#define   NFP_CRYPTO_BULK_ERR_ALLOC_FIFO_PARITY_bf      0, 6, 6
#define   NFP_CRYPTO_BULK_ERR_ALLOC_FIFO_PARITY_mask    (0x1)
#define   NFP_CRYPTO_BULK_ERR_ALLOC_FIFO_PARITY_bit     (6)
#define   NFP_CRYPTO_BULK_ERR_ALLOC_MEM_PARITY          (0x1 << 5)
#define   NFP_CRYPTO_BULK_ERR_ALLOC_MEM_PARITY_bf       0, 5, 5
#define   NFP_CRYPTO_BULK_ERR_ALLOC_MEM_PARITY_mask     (0x1)
#define   NFP_CRYPTO_BULK_ERR_ALLOC_MEM_PARITY_bit      (5)
#define   NFP_CRYPTO_BULK_ERR_ALLOC_BUSY                (0x1 << 4)
#define   NFP_CRYPTO_BULK_ERR_ALLOC_BUSY_bf             0, 4, 4
#define   NFP_CRYPTO_BULK_ERR_ALLOC_BUSY_mask           (0x1)
#define   NFP_CRYPTO_BULK_ERR_ALLOC_BUSY_bit            (4)
#define   NFP_CRYPTO_BULK_ERR_ALLOC_GET_CTX             (0x1 << 3)
#define   NFP_CRYPTO_BULK_ERR_ALLOC_GET_CTX_bf          0, 3, 3
#define   NFP_CRYPTO_BULK_ERR_ALLOC_GET_CTX_mask        (0x1)
#define   NFP_CRYPTO_BULK_ERR_ALLOC_GET_CTX_bit         (3)
#define   NFP_CRYPTO_BULK_ERR_ALLOC_FREE_CTX            (0x1 << 2)
#define   NFP_CRYPTO_BULK_ERR_ALLOC_FREE_CTX_bf         0, 2, 2
#define   NFP_CRYPTO_BULK_ERR_ALLOC_FREE_CTX_mask       (0x1)
#define   NFP_CRYPTO_BULK_ERR_ALLOC_FREE_CTX_bit        (2)
#define   NFP_CRYPTO_BULK_ERR_ALLOC_GET                 (0x1 << 1)
#define   NFP_CRYPTO_BULK_ERR_ALLOC_GET_bf              0, 1, 1
#define   NFP_CRYPTO_BULK_ERR_ALLOC_GET_mask            (0x1)
#define   NFP_CRYPTO_BULK_ERR_ALLOC_GET_bit             (1)
#define   NFP_CRYPTO_BULK_ERR_ALLOC_CTX                 (0x1)
#define   NFP_CRYPTO_BULK_ERR_ALLOC_CTX_bf              0, 0, 0
#define   NFP_CRYPTO_BULK_ERR_ALLOC_CTX_mask            (0x1)
#define   NFP_CRYPTO_BULK_ERR_ALLOC_CTX_bit             (0)
/* Register Type: WhsOnFrstErr */
#define NFP_CRYPTO_BULK_FERR           0x0014
/* Register Type: ErrorMask */
#define NFP_CRYPTO_BULK_ERRMASK        0x0018
/* Register Type: ClkStopMask */
#define NFP_CRYPTO_BULK_CLK_STOPMASK   0x001c
/* Register Type: TestMode */
#define NFP_CRYPTO_BULK_TESTMODE       0x0020
#define   NFP_CRYPTO_BULK_TESTMODE_TARGET(_x)           (((_x) & 0xff) << 24)
#define   NFP_CRYPTO_BULK_TESTMODE_TARGET_bf            0, 31, 24
#define   NFP_CRYPTO_BULK_TESTMODE_TARGET_mask          (0xff)
#define   NFP_CRYPTO_BULK_TESTMODE_TARGET_shift         (24)
#define     NFP_CRYPTO_BULK_TESTMODE_TARGET_ALLOC       (129)
#define     NFP_CRYPTO_BULK_TESTMODE_TARGET_WRT         (130)
#define     NFP_CRYPTO_BULK_TESTMODE_TARGET_OTHER       (131)
#define   NFP_CRYPTO_BULK_TESTMODE_DATA(_x)             ((_x) & 0xffffff)
#define   NFP_CRYPTO_BULK_TESTMODE_DATA_bf              0, 23, 0
#define   NFP_CRYPTO_BULK_TESTMODE_DATA_mask            (0xffffff)
#define   NFP_CRYPTO_BULK_TESTMODE_DATA_shift           (0)

/* HGID: nfp3200/crypto_bulkeng.desc = c5fdc5bce1f6 */
/* Register Type: ErrRegLo */
#define NFP_CRYPTO_BULKENG_ERR_LO      0x0000
#define   NFP_CRYPTO_BULKENG_ERR_LO_SER_PARITY          (0x1 << 25)
#define   NFP_CRYPTO_BULKENG_ERR_LO_SER_PARITY_bf       0, 25, 25
#define   NFP_CRYPTO_BULKENG_ERR_LO_SER_PARITY_mask     (0x1)
#define   NFP_CRYPTO_BULKENG_ERR_LO_SER_PARITY_bit      (25)
#define   NFP_CRYPTO_BULKENG_ERR_LO_SER_OVRFLW          (0x1 << 24)
#define   NFP_CRYPTO_BULKENG_ERR_LO_SER_OVRFLW_bf       0, 24, 24
#define   NFP_CRYPTO_BULKENG_ERR_LO_SER_OVRFLW_mask     (0x1)
#define   NFP_CRYPTO_BULKENG_ERR_LO_SER_OVRFLW_bit      (24)
#define   NFP_CRYPTO_BULKENG_ERR_LO_DAT_OVRFLOW         (0x1 << 23)
#define   NFP_CRYPTO_BULKENG_ERR_LO_DAT_OVRFLOW_bf      0, 23, 23
#define   NFP_CRYPTO_BULKENG_ERR_LO_DAT_OVRFLOW_mask    (0x1)
#define   NFP_CRYPTO_BULKENG_ERR_LO_DAT_OVRFLOW_bit     (23)
#define   NFP_CRYPTO_BULKENG_ERR_LO_RDYDAT_PARITY       (0x1 << 22)
#define   NFP_CRYPTO_BULKENG_ERR_LO_RDYDAT_PARITY_bf    0, 22, 22
#define   NFP_CRYPTO_BULKENG_ERR_LO_RDYDAT_PARITY_mask  (0x1)
#define   NFP_CRYPTO_BULKENG_ERR_LO_RDYDAT_PARITY_bit   (22)
#define   NFP_CRYPTO_BULKENG_ERR_LO_CFGDAT_PARITY       (0x1 << 21)
#define   NFP_CRYPTO_BULKENG_ERR_LO_CFGDAT_PARITY_bf    0, 21, 21
#define   NFP_CRYPTO_BULKENG_ERR_LO_CFGDAT_PARITY_mask  (0x1)
#define   NFP_CRYPTO_BULKENG_ERR_LO_CFGDAT_PARITY_bit   (21)
#define   NFP_CRYPTO_BULKENG_ERR_LO_CFGDAT_SIZE         (0x1 << 20)
#define   NFP_CRYPTO_BULKENG_ERR_LO_CFGDAT_SIZE_bf      0, 20, 20
#define   NFP_CRYPTO_BULKENG_ERR_LO_CFGDAT_SIZE_mask    (0x1)
#define   NFP_CRYPTO_BULKENG_ERR_LO_CFGDAT_SIZE_bit     (20)
#define   NFP_CRYPTO_BULKENG_ERR_LO_DES_OVRFLW_MASK     (0x1 << 19)
#define   NFP_CRYPTO_BULKENG_ERR_LO_DES_OVRFLW_MASK_bf  0, 19, 19
#define   NFP_CRYPTO_BULKENG_ERR_LO_DES_OVRFLW_MASK_mask (0x1)
#define   NFP_CRYPTO_BULKENG_ERR_LO_DES_OVRFLW_MASK_bit (19)
#define   NFP_CRYPTO_BULKENG_ERR_LO_DES_OVRFLW_FLAG     (0x1 << 18)
#define   NFP_CRYPTO_BULKENG_ERR_LO_DES_OVRFLW_FLAG_bf  0, 18, 18
#define   NFP_CRYPTO_BULKENG_ERR_LO_DES_OVRFLW_FLAG_mask (0x1)
#define   NFP_CRYPTO_BULKENG_ERR_LO_DES_OVRFLW_FLAG_bit (18)
#define   NFP_CRYPTO_BULKENG_ERR_LO_DES_OVERFLW_DOUT    (0x1 << 17)
#define   NFP_CRYPTO_BULKENG_ERR_LO_DES_OVERFLW_DOUT_bf 0, 17, 17
#define   NFP_CRYPTO_BULKENG_ERR_LO_DES_OVERFLW_DOUT_mask (0x1)
#define   NFP_CRYPTO_BULKENG_ERR_LO_DES_OVERFLW_DOUT_bit (17)
#define   NFP_CRYPTO_BULKENG_ERR_LO_DES_OVRFLW_DIN      (0x1 << 16)
#define   NFP_CRYPTO_BULKENG_ERR_LO_DES_OVRFLW_DIN_bf   0, 16, 16
#define   NFP_CRYPTO_BULKENG_ERR_LO_DES_OVRFLW_DIN_mask (0x1)
#define   NFP_CRYPTO_BULKENG_ERR_LO_DES_OVRFLW_DIN_bit  (16)
#define   NFP_CRYPTO_BULKENG_ERR_LO_DES_CFG_PARTIY      (0x1 << 15)
#define   NFP_CRYPTO_BULKENG_ERR_LO_DES_CFG_PARTIY_bf   0, 15, 15
#define   NFP_CRYPTO_BULKENG_ERR_LO_DES_CFG_PARTIY_mask (0x1)
#define   NFP_CRYPTO_BULKENG_ERR_LO_DES_CFG_PARTIY_bit  (15)
#define   NFP_CRYPTO_BULKENG_ERR_LO_DES_CFG_SIZE        (0x1 << 14)
#define   NFP_CRYPTO_BULKENG_ERR_LO_DES_CFG_SIZE_bf     0, 14, 14
#define   NFP_CRYPTO_BULKENG_ERR_LO_DES_CFG_SIZE_mask   (0x1)
#define   NFP_CRYPTO_BULKENG_ERR_LO_DES_CFG_SIZE_bit    (14)
#define   NFP_CRYPTO_BULKENG_ERR_LO_AES_OVRFLW_FLAG     (0x1 << 13)
#define   NFP_CRYPTO_BULKENG_ERR_LO_AES_OVRFLW_FLAG_bf  0, 13, 13
#define   NFP_CRYPTO_BULKENG_ERR_LO_AES_OVRFLW_FLAG_mask (0x1)
#define   NFP_CRYPTO_BULKENG_ERR_LO_AES_OVRFLW_FLAG_bit (13)
#define   NFP_CRYPTO_BULKENG_ERR_LO_AES_OVRFLW_DATA     (0x1 << 12)
#define   NFP_CRYPTO_BULKENG_ERR_LO_AES_OVRFLW_DATA_bf  0, 12, 12
#define   NFP_CRYPTO_BULKENG_ERR_LO_AES_OVRFLW_DATA_mask (0x1)
#define   NFP_CRYPTO_BULKENG_ERR_LO_AES_OVRFLW_DATA_bit (12)
#define   NFP_CRYPTO_BULKENG_ERR_LO_AES_OVRFLW_MASK     (0x1 << 11)
#define   NFP_CRYPTO_BULKENG_ERR_LO_AES_OVRFLW_MASK_bf  0, 11, 11
#define   NFP_CRYPTO_BULKENG_ERR_LO_AES_OVRFLW_MASK_mask (0x1)
#define   NFP_CRYPTO_BULKENG_ERR_LO_AES_OVRFLW_MASK_bit (11)
#define   NFP_CRYPTO_BULKENG_ERR_LO_AES_FDF_OVRFLW_FLAG (0x1 << 10)
#define   NFP_CRYPTO_BULKENG_ERR_LO_AES_FDF_OVRFLW_FLAG_bf 0, 10, 10
#define   NFP_CRYPTO_BULKENG_ERR_LO_AES_FDF_OVRFLW_FLAG_mask (0x1)
#define   NFP_CRYPTO_BULKENG_ERR_LO_AES_FDF_OVRFLW_FLAG_bit (10)
#define   NFP_CRYPTO_BULKENG_ERR_LO_AES_FDF_OVRFLW_DOUT (0x1 << 9)
#define   NFP_CRYPTO_BULKENG_ERR_LO_AES_FDF_OVRFLW_DOUT_bf 0, 9, 9
#define   NFP_CRYPTO_BULKENG_ERR_LO_AES_FDF_OVRFLW_DOUT_mask (0x1)
#define   NFP_CRYPTO_BULKENG_ERR_LO_AES_FDF_OVRFLW_DOUT_bit (9)
#define   NFP_CRYPTO_BULKENG_ERR_LO_AES_FDF_OVRFLW_DIN  (0x1 << 8)
#define   NFP_CRYPTO_BULKENG_ERR_LO_AES_FDF_OVRFLW_DIN_bf 0, 8, 8
#define   NFP_CRYPTO_BULKENG_ERR_LO_AES_FDF_OVRFLW_DIN_mask (0x1)
#define   NFP_CRYPTO_BULKENG_ERR_LO_AES_FDF_OVRFLW_DIN_bit (8)
#define   NFP_CRYPTO_BULKENG_ERR_LO_AES_KEYRD_PARITY    (0x1 << 7)
#define   NFP_CRYPTO_BULKENG_ERR_LO_AES_KEYRD_PARITY_bf 0, 7, 7
#define   NFP_CRYPTO_BULKENG_ERR_LO_AES_KEYRD_PARITY_mask (0x1)
#define   NFP_CRYPTO_BULKENG_ERR_LO_AES_KEYRD_PARITY_bit (7)
#define   NFP_CRYPTO_BULKENG_ERR_LO_AES_KEYIN_PARITY    (0x1 << 6)
#define   NFP_CRYPTO_BULKENG_ERR_LO_AES_KEYIN_PARITY_bf 0, 6, 6
#define   NFP_CRYPTO_BULKENG_ERR_LO_AES_KEYIN_PARITY_mask (0x1)
#define   NFP_CRYPTO_BULKENG_ERR_LO_AES_KEYIN_PARITY_bit (6)
#define   NFP_CRYPTO_BULKENG_ERR_LO_AES_KEY_SIZE        (0x1 << 5)
#define   NFP_CRYPTO_BULKENG_ERR_LO_AES_KEY_SIZE_bf     0, 5, 5
#define   NFP_CRYPTO_BULKENG_ERR_LO_AES_KEY_SIZE_mask   (0x1)
#define   NFP_CRYPTO_BULKENG_ERR_LO_AES_KEY_SIZE_bit    (5)
#define   NFP_CRYPTO_BULKENG_ERR_LO_AES_CFG_PARITY      (0x1 << 4)
#define   NFP_CRYPTO_BULKENG_ERR_LO_AES_CFG_PARITY_bf   0, 4, 4
#define   NFP_CRYPTO_BULKENG_ERR_LO_AES_CFG_PARITY_mask (0x1)
#define   NFP_CRYPTO_BULKENG_ERR_LO_AES_CFG_PARITY_bit  (4)
#define   NFP_CRYPTO_BULKENG_ERR_LO_AES_CFG_SIZE        (0x1 << 3)
#define   NFP_CRYPTO_BULKENG_ERR_LO_AES_CFG_SIZE_bf     0, 3, 3
#define   NFP_CRYPTO_BULKENG_ERR_LO_AES_CFG_SIZE_mask   (0x1)
#define   NFP_CRYPTO_BULKENG_ERR_LO_AES_CFG_SIZE_bit    (3)
#define   NFP_CRYPTO_BULKENG_ERR_LO_CPC_PKG_HOLE        (0x1 << 2)
#define   NFP_CRYPTO_BULKENG_ERR_LO_CPC_PKG_HOLE_bf     0, 2, 2
#define   NFP_CRYPTO_BULKENG_ERR_LO_CPC_PKG_HOLE_mask   (0x1)
#define   NFP_CRYPTO_BULKENG_ERR_LO_CPC_PKG_HOLE_bit    (2)
#define   NFP_CRYPTO_BULKENG_ERR_LO_CPC_OVRFLW          (0x1 << 1)
#define   NFP_CRYPTO_BULKENG_ERR_LO_CPC_OVRFLW_bf       0, 1, 1
#define   NFP_CRYPTO_BULKENG_ERR_LO_CPC_OVRFLW_mask     (0x1)
#define   NFP_CRYPTO_BULKENG_ERR_LO_CPC_OVRFLW_bit      (1)
#define   NFP_CRYPTO_BULKENG_ERR_LO_CPC_PARITY          (0x1)
#define   NFP_CRYPTO_BULKENG_ERR_LO_CPC_PARITY_bf       0, 0, 0
#define   NFP_CRYPTO_BULKENG_ERR_LO_CPC_PARITY_mask     (0x1)
#define   NFP_CRYPTO_BULKENG_ERR_LO_CPC_PARITY_bit      (0)
/* Register Type: ErrRegHi */
#define NFP_CRYPTO_BULKENG_ERR_HI      0x0004
#define   NFP_CRYPTO_BULKENG_ERR_HI_CRC_MEMRD           (0x1 << 27)
#define   NFP_CRYPTO_BULKENG_ERR_HI_CRC_MEMRD_bf        0, 27, 27
#define   NFP_CRYPTO_BULKENG_ERR_HI_CRC_MEMRD_mask      (0x1)
#define   NFP_CRYPTO_BULKENG_ERR_HI_CRC_MEMRD_bit       (27)
#define   NFP_CRYPTO_BULKENG_ERR_HI_CRC_INSTR_COLLIDE   (0x1 << 26)
#define   NFP_CRYPTO_BULKENG_ERR_HI_CRC_INSTR_COLLIDE_bf 0, 26, 26
#define   NFP_CRYPTO_BULKENG_ERR_HI_CRC_INSTR_COLLIDE_mask (0x1)
#define   NFP_CRYPTO_BULKENG_ERR_HI_CRC_INSTR_COLLIDE_bit (26)
#define   NFP_CRYPTO_BULKENG_ERR_HI_CPS_FLAG            (0x1 << 25)
#define   NFP_CRYPTO_BULKENG_ERR_HI_CPS_FLAG_bf         0, 25, 25
#define   NFP_CRYPTO_BULKENG_ERR_HI_CPS_FLAG_mask       (0x1)
#define   NFP_CRYPTO_BULKENG_ERR_HI_CPS_FLAG_bit        (25)
#define   NFP_CRYPTO_BULKENG_ERR_HI_CPS_PKG_HOLE        (0x1 << 24)
#define   NFP_CRYPTO_BULKENG_ERR_HI_CPS_PKG_HOLE_bf     0, 24, 24
#define   NFP_CRYPTO_BULKENG_ERR_HI_CPS_PKG_HOLE_mask   (0x1)
#define   NFP_CRYPTO_BULKENG_ERR_HI_CPS_PKG_HOLE_bit    (24)
#define   NFP_CRYPTO_BULKENG_ERR_HI_CDI_PARITY          (0x1 << 23)
#define   NFP_CRYPTO_BULKENG_ERR_HI_CDI_PARITY_bf       0, 23, 23
#define   NFP_CRYPTO_BULKENG_ERR_HI_CDI_PARITY_mask     (0x1)
#define   NFP_CRYPTO_BULKENG_ERR_HI_CDI_PARITY_bit      (23)
#define   NFP_CRYPTO_BULKENG_ERR_HI_CDI_OVRFLW          (0x1 << 22)
#define   NFP_CRYPTO_BULKENG_ERR_HI_CDI_OVRFLW_bf       0, 22, 22
#define   NFP_CRYPTO_BULKENG_ERR_HI_CDI_OVRFLW_mask     (0x1)
#define   NFP_CRYPTO_BULKENG_ERR_HI_CDI_OVRFLW_bit      (22)
#define   NFP_CRYPTO_BULKENG_ERR_HI_ICF_PTR1_PARITY     (0x1 << 21)
#define   NFP_CRYPTO_BULKENG_ERR_HI_ICF_PTR1_PARITY_bf  0, 21, 21
#define   NFP_CRYPTO_BULKENG_ERR_HI_ICF_PTR1_PARITY_mask (0x1)
#define   NFP_CRYPTO_BULKENG_ERR_HI_ICF_PTR1_PARITY_bit (21)
#define   NFP_CRYPTO_BULKENG_ERR_HI_ICF_PTR0_PARITY     (0x1 << 20)
#define   NFP_CRYPTO_BULKENG_ERR_HI_ICF_PTR0_PARITY_bf  0, 20, 20
#define   NFP_CRYPTO_BULKENG_ERR_HI_ICF_PTR0_PARITY_mask (0x1)
#define   NFP_CRYPTO_BULKENG_ERR_HI_ICF_PTR0_PARITY_bit (20)
#define   NFP_CRYPTO_BULKENG_ERR_HI_ICF_DATA_PARITY     (0x1 << 19)
#define   NFP_CRYPTO_BULKENG_ERR_HI_ICF_DATA_PARITY_bf  0, 19, 19
#define   NFP_CRYPTO_BULKENG_ERR_HI_ICF_DATA_PARITY_mask (0x1)
#define   NFP_CRYPTO_BULKENG_ERR_HI_ICF_DATA_PARITY_bit (19)
#define   NFP_CRYPTO_BULKENG_ERR_HI_RDR_BUSY            (0x1 << 18)
#define   NFP_CRYPTO_BULKENG_ERR_HI_RDR_BUSY_bf         0, 18, 18
#define   NFP_CRYPTO_BULKENG_ERR_HI_RDR_BUSY_mask       (0x1)
#define   NFP_CRYPTO_BULKENG_ERR_HI_RDR_BUSY_bit        (18)
#define   NFP_CRYPTO_BULKENG_ERR_HI_RDR_PARITY          (0x1 << 17)
#define   NFP_CRYPTO_BULKENG_ERR_HI_RDR_PARITY_bf       0, 17, 17
#define   NFP_CRYPTO_BULKENG_ERR_HI_RDR_PARITY_mask     (0x1)
#define   NFP_CRYPTO_BULKENG_ERR_HI_RDR_PARITY_bit      (17)
#define   NFP_CRYPTO_BULKENG_ERR_HI_RDR_OVRFLW          (0x1 << 16)
#define   NFP_CRYPTO_BULKENG_ERR_HI_RDR_OVRFLW_bf       0, 16, 16
#define   NFP_CRYPTO_BULKENG_ERR_HI_RDR_OVRFLW_mask     (0x1)
#define   NFP_CRYPTO_BULKENG_ERR_HI_RDR_OVRFLW_bit      (16)
#define   NFP_CRYPTO_BULKENG_ERR_HI_HASH_OVRFLW         (0x1 << 8)
#define   NFP_CRYPTO_BULKENG_ERR_HI_HASH_OVRFLW_bf      0, 8, 8
#define   NFP_CRYPTO_BULKENG_ERR_HI_HASH_OVRFLW_mask    (0x1)
#define   NFP_CRYPTO_BULKENG_ERR_HI_HASH_OVRFLW_bit     (8)
#define   NFP_CRYPTO_BULKENG_ERR_HI_SHA_CFG_SIZE        (0x1 << 7)
#define   NFP_CRYPTO_BULKENG_ERR_HI_SHA_CFG_SIZE_bf     0, 7, 7
#define   NFP_CRYPTO_BULKENG_ERR_HI_SHA_CFG_SIZE_mask   (0x1)
#define   NFP_CRYPTO_BULKENG_ERR_HI_SHA_CFG_SIZE_bit    (7)
#define   NFP_CRYPTO_BULKENG_ERR_HI_SHA_CFG_PARITY      (0x1 << 6)
#define   NFP_CRYPTO_BULKENG_ERR_HI_SHA_CFG_PARITY_bf   0, 6, 6
#define   NFP_CRYPTO_BULKENG_ERR_HI_SHA_CFG_PARITY_mask (0x1)
#define   NFP_CRYPTO_BULKENG_ERR_HI_SHA_CFG_PARITY_bit  (6)
#define   NFP_CRYPTO_BULKENG_ERR_HI_GF_FLAGTYPE         (0x1 << 5)
#define   NFP_CRYPTO_BULKENG_ERR_HI_GF_FLAGTYPE_bf      0, 5, 5
#define   NFP_CRYPTO_BULKENG_ERR_HI_GF_FLAGTYPE_mask    (0x1)
#define   NFP_CRYPTO_BULKENG_ERR_HI_GF_FLAGTYPE_bit     (5)
#define   NFP_CRYPTO_BULKENG_ERR_HI_GF_CFG_SIZE         (0x1 << 4)
#define   NFP_CRYPTO_BULKENG_ERR_HI_GF_CFG_SIZE_bf      0, 4, 4
#define   NFP_CRYPTO_BULKENG_ERR_HI_GF_CFG_SIZE_mask    (0x1)
#define   NFP_CRYPTO_BULKENG_ERR_HI_GF_CFG_SIZE_bit     (4)
#define   NFP_CRYPTO_BULKENG_ERR_HI_GF_CFG_PARITY       (0x1 << 3)
#define   NFP_CRYPTO_BULKENG_ERR_HI_GF_CFG_PARITY_bf    0, 3, 3
#define   NFP_CRYPTO_BULKENG_ERR_HI_GF_CFG_PARITY_mask  (0x1)
#define   NFP_CRYPTO_BULKENG_ERR_HI_GF_CFG_PARITY_bit   (3)
#define   NFP_CRYPTO_BULKENG_ERR_HI_CPH_PKG_HOLE        (0x1 << 2)
#define   NFP_CRYPTO_BULKENG_ERR_HI_CPH_PKG_HOLE_bf     0, 2, 2
#define   NFP_CRYPTO_BULKENG_ERR_HI_CPH_PKG_HOLE_mask   (0x1)
#define   NFP_CRYPTO_BULKENG_ERR_HI_CPH_PKG_HOLE_bit    (2)
#define   NFP_CRYPTO_BULKENG_ERR_HI_CPH_OVRFLW          (0x1 << 1)
#define   NFP_CRYPTO_BULKENG_ERR_HI_CPH_OVRFLW_bf       0, 1, 1
#define   NFP_CRYPTO_BULKENG_ERR_HI_CPH_OVRFLW_mask     (0x1)
#define   NFP_CRYPTO_BULKENG_ERR_HI_CPH_OVRFLW_bit      (1)
#define   NFP_CRYPTO_BULKENG_ERR_HI_CPH_PARITY          (0x1)
#define   NFP_CRYPTO_BULKENG_ERR_HI_CPH_PARITY_bf       0, 0, 0
#define   NFP_CRYPTO_BULKENG_ERR_HI_CPH_PARITY_mask     (0x1)
#define   NFP_CRYPTO_BULKENG_ERR_HI_CPH_PARITY_bit      (0)
/* Register Type: WhosOnFrstLo */
#define NFP_CRYPTO_BULKENG_FERR_LO     0x0008
#define   NFP_CRYPTO_BULKENG_FERR_LO_FERR_LO(_x)        (_x)
#define   NFP_CRYPTO_BULKENG_FERR_LO_FERR_LO_bf         0, 31, 0
#define   NFP_CRYPTO_BULKENG_FERR_LO_FERR_LO_mask       (0xffffffff)
#define   NFP_CRYPTO_BULKENG_FERR_LO_FERR_LO_shift      (0)
/* Register Type: WhosOnFrstHi */
#define NFP_CRYPTO_BULKENG_FERR_HI     0x000c
#define   NFP_CRYPTO_BULKENG_FERR_HI_FERR_HI(_x)        (_x)
#define   NFP_CRYPTO_BULKENG_FERR_HI_FERR_HI_bf         0, 31, 0
#define   NFP_CRYPTO_BULKENG_FERR_HI_FERR_HI_mask       (0xffffffff)
#define   NFP_CRYPTO_BULKENG_FERR_HI_FERR_HI_shift      (0)
/* Register Type: ErrMaskLo */
#define NFP_CRYPTO_BULKENG_ERRMASK_LO  0x0010
#define   NFP_CRYPTO_BULKENG_ERRMASK_LO_ERRMASK_LO(_x)  (_x)
#define   NFP_CRYPTO_BULKENG_ERRMASK_LO_ERRMASK_LO_bf   0, 31, 0
#define   NFP_CRYPTO_BULKENG_ERRMASK_LO_ERRMASK_LO_mask (0xffffffff)
#define   NFP_CRYPTO_BULKENG_ERRMASK_LO_ERRMASK_LO_shift (0)
/* Register Type: ErrMaskHi */
#define NFP_CRYPTO_BULKENG_ERRMASK_HI  0x0014
#define   NFP_CRYPTO_BULKENG_ERRMASK_HI_ERRMASK_HI(_x)  (_x)
#define   NFP_CRYPTO_BULKENG_ERRMASK_HI_ERRMASK_HI_bf   0, 31, 0
#define   NFP_CRYPTO_BULKENG_ERRMASK_HI_ERRMASK_HI_mask (0xffffffff)
#define   NFP_CRYPTO_BULKENG_ERRMASK_HI_ERRMASK_HI_shift (0)
/* Register Type: ClkStpMskLo */
#define NFP_CRYPTO_BULKENG_CLK_STOPMASK_LO 0x0018
#define   NFP_CRYPTO_BULKENG_CLK_STOPMASK_LO_CLK_STOPMASK_LO(_x) (_x)
#define   NFP_CRYPTO_BULKENG_CLK_STOPMASK_LO_CLK_STOPMASK_LO_bf 0, 31, 0
#define   NFP_CRYPTO_BULKENG_CLK_STOPMASK_LO_CLK_STOPMASK_LO_mask (0xffffffff)
#define   NFP_CRYPTO_BULKENG_CLK_STOPMASK_LO_CLK_STOPMASK_LO_shift (0)
/* Register Type: ClkStpMskHi */
#define NFP_CRYPTO_BULKENG_CLK_STOPMASK_HI 0x001c
#define   NFP_CRYPTO_BULKENG_CLK_STOPMASK_HI_CLK_STOPMASK_HI(_x) (_x)
#define   NFP_CRYPTO_BULKENG_CLK_STOPMASK_HI_CLK_STOPMASK_HI_bf 0, 31, 0
#define   NFP_CRYPTO_BULKENG_CLK_STOPMASK_HI_CLK_STOPMASK_HI_mask (0xffffffff)
#define   NFP_CRYPTO_BULKENG_CLK_STOPMASK_HI_CLK_STOPMASK_HI_shift (0)
/* Register Type: TstMdErrInj */
#define NFP_CRYPTO_BULKENG_TESTMODE_INJ 0x0020
#define   NFP_CRYPTO_BULKENG_TESTMODE_INJ_TARGET(_x)    (((_x) & 0xff) << 24)
#define   NFP_CRYPTO_BULKENG_TESTMODE_INJ_TARGET_bf     0, 31, 24
#define   NFP_CRYPTO_BULKENG_TESTMODE_INJ_TARGET_mask   (0xff)
#define   NFP_CRYPTO_BULKENG_TESTMODE_INJ_TARGET_shift  (24)
#define     NFP_CRYPTO_BULKENG_TESTMODE_INJ_TARGET_RDR  (129)
#define     NFP_CRYPTO_BULKENG_TESTMODE_INJ_TARGET_ICF_DATA (130)
#define     NFP_CRYPTO_BULKENG_TESTMODE_INJ_TARGET_ICF_PTR0 (131)
#define     NFP_CRYPTO_BULKENG_TESTMODE_INJ_TARGET_ICF_PTR1 (132)
#define     NFP_CRYPTO_BULKENG_TESTMODE_INJ_TARGET_CDI  (133)
#define     NFP_CRYPTO_BULKENG_TESTMODE_INJ_TARGET_AES_KEY (136)
#define     NFP_CRYPTO_BULKENG_TESTMODE_INJ_TARGET_AES_CFG (137)
#define     NFP_CRYPTO_BULKENG_TESTMODE_INJ_TARGET_DES  (138)
#define     NFP_CRYPTO_BULKENG_TESTMODE_INJ_TARGET_CPC  (139)
#define     NFP_CRYPTO_BULKENG_TESTMODE_INJ_TARGET_GF   (140)
#define   NFP_CRYPTO_BULKENG_TESTMODE_INJ_DATA(_x)      ((_x) & 0xffffff)
#define   NFP_CRYPTO_BULKENG_TESTMODE_INJ_DATA_bf       0, 23, 0
#define   NFP_CRYPTO_BULKENG_TESTMODE_INJ_DATA_mask     (0xffffff)
#define   NFP_CRYPTO_BULKENG_TESTMODE_INJ_DATA_shift    (0)


#endif /* __NFP_SDK__NFP3200_NFP_CRYPTO_H__ */
