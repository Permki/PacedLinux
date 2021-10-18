/*
 * Copyright (C) 2012  Netronome Systems, Inc.  All rights reserved.
 *
 * @file        nfp3200/nfp_arm.h
 * @brief       Register definitions for ARM
 *
 * Disclaimer: this file is provided without any warranty
 * as part of an early-access program, and the content is
 * bound to change before the final release.
 */

#ifndef __NFP_SDK__NFP3200_NFP_ARM_H__
#define __NFP_SDK__NFP3200_NFP_ARM_H__

/* HGID: nfp3200/arm.desc = dcf7ec592c9f */
#define NFP_ARM_QUEUE(_q)              (0x100000 + (0x800 * ((_q) & 0xff)))
#define NFP_ARM_IM                     0x200000
#define NFP_ARM_EM                     0x300000
#define NFP_ARM_GCSR                   0x400000
#define NFP_ARM_MPCORE                 0x800000
#define NFP_ARM_PL310                  0xa00000
/* Register Type: BulkBARConfig */
#define NFP_ARM_GCSR_BULK_BAR(_bar)    (0x0 + (0x4 * ((_bar) & 0x7)))
#define   NFP_ARM_GCSR_BULK_BAR_TYPE                    (0x1 << 31)
#define   NFP_ARM_GCSR_BULK_BAR_TYPE_bf                 0, 31, 31
#define   NFP_ARM_GCSR_BULK_BAR_TYPE_mask               (0x1)
#define   NFP_ARM_GCSR_BULK_BAR_TYPE_bit                (31)
#define     NFP_ARM_GCSR_BULK_BAR_TYPE_BULK             (0x0)
#define     NFP_ARM_GCSR_BULK_BAR_TYPE_EXPA             (0x80000000)
#define   NFP_ARM_GCSR_BULK_BAR_TGT(_x)                 (((_x) & 0xf) << 27)
#define   NFP_ARM_GCSR_BULK_BAR_TGT_bf                  0, 30, 27
#define   NFP_ARM_GCSR_BULK_BAR_TGT_mask                (0xf)
#define   NFP_ARM_GCSR_BULK_BAR_TGT_shift               (27)
#define   NFP_ARM_GCSR_BULK_BAR_TOK(_x)                 (((_x) & 0x3) << 25)
#define   NFP_ARM_GCSR_BULK_BAR_TOK_bf                  0, 26, 25
#define   NFP_ARM_GCSR_BULK_BAR_TOK_mask                (0x3)
#define   NFP_ARM_GCSR_BULK_BAR_TOK_shift               (25)
#define   NFP_ARM_GCSR_BULK_BAR_LEN                     (0x1 << 24)
#define   NFP_ARM_GCSR_BULK_BAR_LEN_bf                  0, 24, 24
#define   NFP_ARM_GCSR_BULK_BAR_LEN_mask                (0x1)
#define   NFP_ARM_GCSR_BULK_BAR_LEN_bit                 (24)
#define     NFP_ARM_GCSR_BULK_BAR_LEN_32BIT             (0x0)
#define     NFP_ARM_GCSR_BULK_BAR_LEN_64BIT             (0x1000000)
#define   NFP_ARM_GCSR_BULK_BAR_ADDR(_x)                ((_x) & 0x7ff)
#define   NFP_ARM_GCSR_BULK_BAR_ADDR_bf                 0, 10, 0
#define   NFP_ARM_GCSR_BULK_BAR_ADDR_mask               (0x7ff)
#define   NFP_ARM_GCSR_BULK_BAR_ADDR_shift              (0)
/* Register Type: ExpansionBARConfig */
#define NFP_ARM_GCSR_EXPA_BAR(_bar)    (0x20 + (0x4 * ((_bar) & 0xf)))
#define   NFP_ARM_GCSR_EXPA_BAR_TYPE                    (0x1 << 31)
#define   NFP_ARM_GCSR_EXPA_BAR_TYPE_bf                 0, 31, 31
#define   NFP_ARM_GCSR_EXPA_BAR_TYPE_mask               (0x1)
#define   NFP_ARM_GCSR_EXPA_BAR_TYPE_bit                (31)
#define     NFP_ARM_GCSR_EXPA_BAR_TYPE_EXPA             (0x0)
#define     NFP_ARM_GCSR_EXPA_BAR_TYPE_EXPL             (0x80000000)
#define   NFP_ARM_GCSR_EXPA_BAR_TGT(_x)                 (((_x) & 0xf) << 27)
#define   NFP_ARM_GCSR_EXPA_BAR_TGT_bf                  0, 30, 27
#define   NFP_ARM_GCSR_EXPA_BAR_TGT_mask                (0xf)
#define   NFP_ARM_GCSR_EXPA_BAR_TGT_shift               (27)
#define   NFP_ARM_GCSR_EXPA_BAR_TOK(_x)                 (((_x) & 0x3) << 25)
#define   NFP_ARM_GCSR_EXPA_BAR_TOK_bf                  0, 26, 25
#define   NFP_ARM_GCSR_EXPA_BAR_TOK_mask                (0x3)
#define   NFP_ARM_GCSR_EXPA_BAR_TOK_shift               (25)
#define   NFP_ARM_GCSR_EXPA_BAR_LEN                     (0x1 << 24)
#define   NFP_ARM_GCSR_EXPA_BAR_LEN_bf                  0, 24, 24
#define   NFP_ARM_GCSR_EXPA_BAR_LEN_mask                (0x1)
#define   NFP_ARM_GCSR_EXPA_BAR_LEN_bit                 (24)
#define     NFP_ARM_GCSR_EXPA_BAR_LEN_32BIT             (0x0)
#define     NFP_ARM_GCSR_EXPA_BAR_LEN_64BIT             (0x1000000)
#define   NFP_ARM_GCSR_EXPA_BAR_ACT(_x)                 (((_x) & 0x1f) << 19)
#define   NFP_ARM_GCSR_EXPA_BAR_ACT_bf                  0, 23, 19
#define   NFP_ARM_GCSR_EXPA_BAR_ACT_mask                (0x1f)
#define   NFP_ARM_GCSR_EXPA_BAR_ACT_shift               (19)
#define     NFP_ARM_GCSR_EXPA_BAR_ACT_DERIVED           (0)
#define   NFP_ARM_GCSR_EXPA_BAR_ADDR(_x)                ((_x) & 0x7fff)
#define   NFP_ARM_GCSR_EXPA_BAR_ADDR_bf                 0, 14, 0
#define   NFP_ARM_GCSR_EXPA_BAR_ADDR_mask               (0x7fff)
#define   NFP_ARM_GCSR_EXPA_BAR_ADDR_shift              (0)
/* Register Type: ExplicitBARConfig0_Reg */
#define NFP_ARM_GCSR_EXPL0_BAR(_bar)   (0x60 + (0x4 * ((_bar) & 0x7)))
#define   NFP_ARM_GCSR_EXPL0_BAR_ADDR(_x)               ((_x) & 0x3ffff)
#define   NFP_ARM_GCSR_EXPL0_BAR_ADDR_bf                0, 17, 0
#define   NFP_ARM_GCSR_EXPL0_BAR_ADDR_mask              (0x3ffff)
#define   NFP_ARM_GCSR_EXPL0_BAR_ADDR_shift             (0)
/* Register Type: ExplicitBARConfig1_Reg */
#define NFP_ARM_GCSR_EXPL1_BAR(_bar)   (0x80 + (0x4 * ((_bar) & 0x7)))
#define   NFP_ARM_GCSR_EXPL1_BAR_POSTED                 (0x1 << 31)
#define   NFP_ARM_GCSR_EXPL1_BAR_POSTED_bf              0, 31, 31
#define   NFP_ARM_GCSR_EXPL1_BAR_POSTED_mask            (0x1)
#define   NFP_ARM_GCSR_EXPL1_BAR_POSTED_bit             (31)
#define   NFP_ARM_GCSR_EXPL1_BAR_SIGNAL_REF(_x)         (((_x) & 0x7f) << 24)
#define   NFP_ARM_GCSR_EXPL1_BAR_SIGNAL_REF_bf          0, 30, 24
#define   NFP_ARM_GCSR_EXPL1_BAR_SIGNAL_REF_mask        (0x7f)
#define   NFP_ARM_GCSR_EXPL1_BAR_SIGNAL_REF_shift       (24)
#define   NFP_ARM_GCSR_EXPL1_BAR_DATA_MASTER(_x)        (((_x) & 0xff) << 16)
#define   NFP_ARM_GCSR_EXPL1_BAR_DATA_MASTER_bf         0, 23, 16
#define   NFP_ARM_GCSR_EXPL1_BAR_DATA_MASTER_mask       (0xff)
#define   NFP_ARM_GCSR_EXPL1_BAR_DATA_MASTER_shift      (16)
#define   NFP_ARM_GCSR_EXPL1_BAR_DATA_REF(_x)           ((_x) & 0x3fff)
#define   NFP_ARM_GCSR_EXPL1_BAR_DATA_REF_bf            0, 13, 0
#define   NFP_ARM_GCSR_EXPL1_BAR_DATA_REF_mask          (0x3fff)
#define   NFP_ARM_GCSR_EXPL1_BAR_DATA_REF_shift         (0)
/* Register Type: ExplicitBARConfig2_Reg */
#define NFP_ARM_GCSR_EXPL2_BAR(_bar)   (0xa0 + (0x4 * ((_bar) & 0x7)))
#define   NFP_ARM_GCSR_EXPL2_BAR_TGT(_x)                (((_x) & 0xf) << 28)
#define   NFP_ARM_GCSR_EXPL2_BAR_TGT_bf                 0, 31, 28
#define   NFP_ARM_GCSR_EXPL2_BAR_TGT_mask               (0xf)
#define   NFP_ARM_GCSR_EXPL2_BAR_TGT_shift              (28)
#define   NFP_ARM_GCSR_EXPL2_BAR_ACT(_x)                (((_x) & 0x1f) << 23)
#define   NFP_ARM_GCSR_EXPL2_BAR_ACT_bf                 0, 27, 23
#define   NFP_ARM_GCSR_EXPL2_BAR_ACT_mask               (0x1f)
#define   NFP_ARM_GCSR_EXPL2_BAR_ACT_shift              (23)
#define   NFP_ARM_GCSR_EXPL2_BAR_LEN(_x)                (((_x) & 0x1f) << 18)
#define   NFP_ARM_GCSR_EXPL2_BAR_LEN_bf                 0, 22, 18
#define   NFP_ARM_GCSR_EXPL2_BAR_LEN_mask               (0x1f)
#define   NFP_ARM_GCSR_EXPL2_BAR_LEN_shift              (18)
#define   NFP_ARM_GCSR_EXPL2_BAR_BYTE_MASK(_x)          (((_x) & 0xff) << 10)
#define   NFP_ARM_GCSR_EXPL2_BAR_BYTE_MASK_bf           0, 17, 10
#define   NFP_ARM_GCSR_EXPL2_BAR_BYTE_MASK_mask         (0xff)
#define   NFP_ARM_GCSR_EXPL2_BAR_BYTE_MASK_shift        (10)
#define   NFP_ARM_GCSR_EXPL2_BAR_TOK(_x)                (((_x) & 0x3) << 8)
#define   NFP_ARM_GCSR_EXPL2_BAR_TOK_bf                 0, 9, 8
#define   NFP_ARM_GCSR_EXPL2_BAR_TOK_mask               (0x3)
#define   NFP_ARM_GCSR_EXPL2_BAR_TOK_shift              (8)
#define   NFP_ARM_GCSR_EXPL2_BAR_SIGNAL_MASTER(_x)      ((_x) & 0xff)
#define   NFP_ARM_GCSR_EXPL2_BAR_SIGNAL_MASTER_bf       0, 7, 0
#define   NFP_ARM_GCSR_EXPL2_BAR_SIGNAL_MASTER_mask     (0xff)
#define   NFP_ARM_GCSR_EXPL2_BAR_SIGNAL_MASTER_shift    (0)
/* Register Type: PostedCommandSignal */
#define NFP_ARM_GCSR_EXPL_POST(_bar)   (0xc0 + (0x4 * ((_bar) & 0x7)))
#define   NFP_ARM_GCSR_EXPL_POST_SIG_B(_x)              (((_x) & 0x7f) << 25)
#define   NFP_ARM_GCSR_EXPL_POST_SIG_B_bf               0, 31, 25
#define   NFP_ARM_GCSR_EXPL_POST_SIG_B_mask             (0x7f)
#define   NFP_ARM_GCSR_EXPL_POST_SIG_B_shift            (25)
#define   NFP_ARM_GCSR_EXPL_POST_SIG_B_BUS              (0x1 << 24)
#define   NFP_ARM_GCSR_EXPL_POST_SIG_B_BUS_bf           0, 24, 24
#define   NFP_ARM_GCSR_EXPL_POST_SIG_B_BUS_mask         (0x1)
#define   NFP_ARM_GCSR_EXPL_POST_SIG_B_BUS_bit          (24)
#define     NFP_ARM_GCSR_EXPL_POST_SIG_B_BUS_PULL       (0x0)
#define     NFP_ARM_GCSR_EXPL_POST_SIG_B_BUS_PUSH       (0x1000000)
#define   NFP_ARM_GCSR_EXPL_POST_SIG_A(_x)              (((_x) & 0x7f) << 17)
#define   NFP_ARM_GCSR_EXPL_POST_SIG_A_bf               0, 23, 17
#define   NFP_ARM_GCSR_EXPL_POST_SIG_A_mask             (0x7f)
#define   NFP_ARM_GCSR_EXPL_POST_SIG_A_shift            (17)
#define   NFP_ARM_GCSR_EXPL_POST_SIG_A_BUS              (0x1 << 16)
#define   NFP_ARM_GCSR_EXPL_POST_SIG_A_BUS_bf           0, 16, 16
#define   NFP_ARM_GCSR_EXPL_POST_SIG_A_BUS_mask         (0x1)
#define   NFP_ARM_GCSR_EXPL_POST_SIG_A_BUS_bit          (16)
#define     NFP_ARM_GCSR_EXPL_POST_SIG_A_BUS_PULL       (0x0)
#define     NFP_ARM_GCSR_EXPL_POST_SIG_A_BUS_PUSH       (0x10000)
#define   NFP_ARM_GCSR_EXPL_POST_SIG_B_RCVD             (0x1 << 7)
#define   NFP_ARM_GCSR_EXPL_POST_SIG_B_RCVD_bf          0, 7, 7
#define   NFP_ARM_GCSR_EXPL_POST_SIG_B_RCVD_mask        (0x1)
#define   NFP_ARM_GCSR_EXPL_POST_SIG_B_RCVD_bit         (7)
#define   NFP_ARM_GCSR_EXPL_POST_SIG_B_VALID            (0x1 << 6)
#define   NFP_ARM_GCSR_EXPL_POST_SIG_B_VALID_bf         0, 6, 6
#define   NFP_ARM_GCSR_EXPL_POST_SIG_B_VALID_mask       (0x1)
#define   NFP_ARM_GCSR_EXPL_POST_SIG_B_VALID_bit        (6)
#define   NFP_ARM_GCSR_EXPL_POST_SIG_A_RCVD             (0x1 << 5)
#define   NFP_ARM_GCSR_EXPL_POST_SIG_A_RCVD_bf          0, 5, 5
#define   NFP_ARM_GCSR_EXPL_POST_SIG_A_RCVD_mask        (0x1)
#define   NFP_ARM_GCSR_EXPL_POST_SIG_A_RCVD_bit         (5)
#define   NFP_ARM_GCSR_EXPL_POST_SIG_A_VALID            (0x1 << 4)
#define   NFP_ARM_GCSR_EXPL_POST_SIG_A_VALID_bf         0, 4, 4
#define   NFP_ARM_GCSR_EXPL_POST_SIG_A_VALID_mask       (0x1)
#define   NFP_ARM_GCSR_EXPL_POST_SIG_A_VALID_bit        (4)
#define   NFP_ARM_GCSR_EXPL_POST_CMD_COMPLETE           (0x1)
#define   NFP_ARM_GCSR_EXPL_POST_CMD_COMPLETE_bf        0, 0, 0
#define   NFP_ARM_GCSR_EXPL_POST_CMD_COMPLETE_mask      (0x1)
#define   NFP_ARM_GCSR_EXPL_POST_CMD_COMPLETE_bit       (0)
/* Register Type: MPCoreBaseAddress */
#define NFP_ARM_GCSR_MPCORE_BASE       0x00e0
#define   NFP_ARM_GCSR_MPCORE_BASE_ADDR(_x)             (((_x) & 0x7ffff) << 13)
#define   NFP_ARM_GCSR_MPCORE_BASE_ADDR_bf              0, 31, 13
#define   NFP_ARM_GCSR_MPCORE_BASE_ADDR_mask            (0x7ffff)
#define   NFP_ARM_GCSR_MPCORE_BASE_ADDR_shift           (13)
/* Register Type: PL310BaseAddress */
#define NFP_ARM_GCSR_PL310_BASE        0x00e4
#define   NFP_ARM_GCSR_PL310_BASE_ADDR(_x)              (((_x) & 0xfffff) << 12)
#define   NFP_ARM_GCSR_PL310_BASE_ADDR_bf               0, 31, 12
#define   NFP_ARM_GCSR_PL310_BASE_ADDR_mask             (0xfffff)
#define   NFP_ARM_GCSR_PL310_BASE_ADDR_shift            (12)
/* Register Type: MPCoreConfig */
#define NFP_ARM_GCSR_MP0_CFG           0x00e8
#define   NFP_ARM_GCSR_MP0_CFG_SPI_BOOT                 (0x1 << 14)
#define   NFP_ARM_GCSR_MP0_CFG_SPI_BOOT_bf              0, 14, 14
#define   NFP_ARM_GCSR_MP0_CFG_SPI_BOOT_mask            (0x1)
#define   NFP_ARM_GCSR_MP0_CFG_SPI_BOOT_bit             (14)
#define   NFP_ARM_GCSR_MP0_CFG_ENDIAN(_x)               (((_x) & 0x3) << 12)
#define   NFP_ARM_GCSR_MP0_CFG_ENDIAN_bf                0, 13, 12
#define   NFP_ARM_GCSR_MP0_CFG_ENDIAN_mask              (0x3)
#define   NFP_ARM_GCSR_MP0_CFG_ENDIAN_shift             (12)
#define     NFP_ARM_GCSR_MP0_CFG_ENDIAN_LITTLE          (0)
#define     NFP_ARM_GCSR_MP0_CFG_ENDIAN_BIG             (1)
#define   NFP_ARM_GCSR_MP0_CFG_RESET_VECTOR             (0x1 << 8)
#define   NFP_ARM_GCSR_MP0_CFG_RESET_VECTOR_bf          0, 8, 8
#define   NFP_ARM_GCSR_MP0_CFG_RESET_VECTOR_mask        (0x1)
#define   NFP_ARM_GCSR_MP0_CFG_RESET_VECTOR_bit         (8)
#define     NFP_ARM_GCSR_MP0_CFG_RESET_VECTOR_LO        (0x0)
#define     NFP_ARM_GCSR_MP0_CFG_RESET_VECTOR_HI        (0x100)
#define   NFP_ARM_GCSR_MP0_CFG_OUTCLK_EN(_x)            (((_x) & 0xf) << 4)
#define   NFP_ARM_GCSR_MP0_CFG_OUTCLK_EN_bf             0, 7, 4
#define   NFP_ARM_GCSR_MP0_CFG_OUTCLK_EN_mask           (0xf)
#define   NFP_ARM_GCSR_MP0_CFG_OUTCLK_EN_shift          (4)
#define   NFP_ARM_GCSR_MP0_CFG_ARMID(_x)                ((_x) & 0xf)
#define   NFP_ARM_GCSR_MP0_CFG_ARMID_bf                 0, 3, 0
#define   NFP_ARM_GCSR_MP0_CFG_ARMID_mask               (0xf)
#define   NFP_ARM_GCSR_MP0_CFG_ARMID_shift              (0)
/* Register Type: MPCoreIDCacheDataError */
#define NFP_ARM_GCSR_MP0_CACHE_ERR     0x00ec
#define   NFP_ARM_GCSR_MP0_CACHE_ERR_MP0_D7             (0x1 << 15)
#define   NFP_ARM_GCSR_MP0_CACHE_ERR_MP0_D7_bf          0, 15, 15
#define   NFP_ARM_GCSR_MP0_CACHE_ERR_MP0_D7_mask        (0x1)
#define   NFP_ARM_GCSR_MP0_CACHE_ERR_MP0_D7_bit         (15)
#define   NFP_ARM_GCSR_MP0_CACHE_ERR_MP0_D6             (0x1 << 14)
#define   NFP_ARM_GCSR_MP0_CACHE_ERR_MP0_D6_bf          0, 14, 14
#define   NFP_ARM_GCSR_MP0_CACHE_ERR_MP0_D6_mask        (0x1)
#define   NFP_ARM_GCSR_MP0_CACHE_ERR_MP0_D6_bit         (14)
#define   NFP_ARM_GCSR_MP0_CACHE_ERR_MP0_D5             (0x1 << 13)
#define   NFP_ARM_GCSR_MP0_CACHE_ERR_MP0_D5_bf          0, 13, 13
#define   NFP_ARM_GCSR_MP0_CACHE_ERR_MP0_D5_mask        (0x1)
#define   NFP_ARM_GCSR_MP0_CACHE_ERR_MP0_D5_bit         (13)
#define   NFP_ARM_GCSR_MP0_CACHE_ERR_MP0_D4             (0x1 << 12)
#define   NFP_ARM_GCSR_MP0_CACHE_ERR_MP0_D4_bf          0, 12, 12
#define   NFP_ARM_GCSR_MP0_CACHE_ERR_MP0_D4_mask        (0x1)
#define   NFP_ARM_GCSR_MP0_CACHE_ERR_MP0_D4_bit         (12)
#define   NFP_ARM_GCSR_MP0_CACHE_ERR_MP0_D3             (0x1 << 11)
#define   NFP_ARM_GCSR_MP0_CACHE_ERR_MP0_D3_bf          0, 11, 11
#define   NFP_ARM_GCSR_MP0_CACHE_ERR_MP0_D3_mask        (0x1)
#define   NFP_ARM_GCSR_MP0_CACHE_ERR_MP0_D3_bit         (11)
#define   NFP_ARM_GCSR_MP0_CACHE_ERR_MP0_D2             (0x1 << 10)
#define   NFP_ARM_GCSR_MP0_CACHE_ERR_MP0_D2_bf          0, 10, 10
#define   NFP_ARM_GCSR_MP0_CACHE_ERR_MP0_D2_mask        (0x1)
#define   NFP_ARM_GCSR_MP0_CACHE_ERR_MP0_D2_bit         (10)
#define   NFP_ARM_GCSR_MP0_CACHE_ERR_MP0_D1             (0x1 << 9)
#define   NFP_ARM_GCSR_MP0_CACHE_ERR_MP0_D1_bf          0, 9, 9
#define   NFP_ARM_GCSR_MP0_CACHE_ERR_MP0_D1_mask        (0x1)
#define   NFP_ARM_GCSR_MP0_CACHE_ERR_MP0_D1_bit         (9)
#define   NFP_ARM_GCSR_MP0_CACHE_ERR_MP0_D0             (0x1 << 8)
#define   NFP_ARM_GCSR_MP0_CACHE_ERR_MP0_D0_bf          0, 8, 8
#define   NFP_ARM_GCSR_MP0_CACHE_ERR_MP0_D0_mask        (0x1)
#define   NFP_ARM_GCSR_MP0_CACHE_ERR_MP0_D0_bit         (8)
#define   NFP_ARM_GCSR_MP0_CACHE_ERR_MP0_I7             (0x1 << 7)
#define   NFP_ARM_GCSR_MP0_CACHE_ERR_MP0_I7_bf          0, 7, 7
#define   NFP_ARM_GCSR_MP0_CACHE_ERR_MP0_I7_mask        (0x1)
#define   NFP_ARM_GCSR_MP0_CACHE_ERR_MP0_I7_bit         (7)
#define   NFP_ARM_GCSR_MP0_CACHE_ERR_MP0_I6             (0x1 << 6)
#define   NFP_ARM_GCSR_MP0_CACHE_ERR_MP0_I6_bf          0, 6, 6
#define   NFP_ARM_GCSR_MP0_CACHE_ERR_MP0_I6_mask        (0x1)
#define   NFP_ARM_GCSR_MP0_CACHE_ERR_MP0_I6_bit         (6)
#define   NFP_ARM_GCSR_MP0_CACHE_ERR_MP0_I5             (0x1 << 5)
#define   NFP_ARM_GCSR_MP0_CACHE_ERR_MP0_I5_bf          0, 5, 5
#define   NFP_ARM_GCSR_MP0_CACHE_ERR_MP0_I5_mask        (0x1)
#define   NFP_ARM_GCSR_MP0_CACHE_ERR_MP0_I5_bit         (5)
#define   NFP_ARM_GCSR_MP0_CACHE_ERR_MP0_I4             (0x1 << 4)
#define   NFP_ARM_GCSR_MP0_CACHE_ERR_MP0_I4_bf          0, 4, 4
#define   NFP_ARM_GCSR_MP0_CACHE_ERR_MP0_I4_mask        (0x1)
#define   NFP_ARM_GCSR_MP0_CACHE_ERR_MP0_I4_bit         (4)
#define   NFP_ARM_GCSR_MP0_CACHE_ERR_MP0_I3             (0x1 << 3)
#define   NFP_ARM_GCSR_MP0_CACHE_ERR_MP0_I3_bf          0, 3, 3
#define   NFP_ARM_GCSR_MP0_CACHE_ERR_MP0_I3_mask        (0x1)
#define   NFP_ARM_GCSR_MP0_CACHE_ERR_MP0_I3_bit         (3)
#define   NFP_ARM_GCSR_MP0_CACHE_ERR_MP0_I2             (0x1 << 2)
#define   NFP_ARM_GCSR_MP0_CACHE_ERR_MP0_I2_bf          0, 2, 2
#define   NFP_ARM_GCSR_MP0_CACHE_ERR_MP0_I2_mask        (0x1)
#define   NFP_ARM_GCSR_MP0_CACHE_ERR_MP0_I2_bit         (2)
#define   NFP_ARM_GCSR_MP0_CACHE_ERR_MP0_I1             (0x1 << 1)
#define   NFP_ARM_GCSR_MP0_CACHE_ERR_MP0_I1_bf          0, 1, 1
#define   NFP_ARM_GCSR_MP0_CACHE_ERR_MP0_I1_mask        (0x1)
#define   NFP_ARM_GCSR_MP0_CACHE_ERR_MP0_I1_bit         (1)
#define   NFP_ARM_GCSR_MP0_CACHE_ERR_MP0_I0             (0x1)
#define   NFP_ARM_GCSR_MP0_CACHE_ERR_MP0_I0_bf          0, 0, 0
#define   NFP_ARM_GCSR_MP0_CACHE_ERR_MP0_I0_mask        (0x1)
#define   NFP_ARM_GCSR_MP0_CACHE_ERR_MP0_I0_bit         (0)
/* Register Type: ARMDFT */
#define NFP_ARM_GCSR_DFT               0x0100
#define   NFP_ARM_GCSR_DFT_DBG_REQ                      (0x1 << 20)
#define   NFP_ARM_GCSR_DFT_DBG_REQ_bf                   0, 20, 20
#define   NFP_ARM_GCSR_DFT_DBG_REQ_mask                 (0x1)
#define   NFP_ARM_GCSR_DFT_DBG_REQ_bit                  (20)
#define   NFP_ARM_GCSR_DFT_DBG_EN                       (0x1 << 19)
#define   NFP_ARM_GCSR_DFT_DBG_EN_bf                    0, 19, 19
#define   NFP_ARM_GCSR_DFT_DBG_EN_mask                  (0x1)
#define   NFP_ARM_GCSR_DFT_DBG_EN_bit                   (19)
#define   NFP_ARM_GCSR_DFT_WFE_EVT_TRG                  (0x1 << 18)
#define   NFP_ARM_GCSR_DFT_WFE_EVT_TRG_bf               0, 18, 18
#define   NFP_ARM_GCSR_DFT_WFE_EVT_TRG_mask             (0x1)
#define   NFP_ARM_GCSR_DFT_WFE_EVT_TRG_bit              (18)
#define   NFP_ARM_GCSR_DFT_ETM_WFI_RDY                  (0x1 << 17)
#define   NFP_ARM_GCSR_DFT_ETM_WFI_RDY_bf               0, 17, 17
#define   NFP_ARM_GCSR_DFT_ETM_WFI_RDY_mask             (0x1)
#define   NFP_ARM_GCSR_DFT_ETM_WFI_RDY_bit              (17)
#define   NFP_ARM_GCSR_DFT_ETM_PWR_ON                   (0x1 << 16)
#define   NFP_ARM_GCSR_DFT_ETM_PWR_ON_bf                0, 16, 16
#define   NFP_ARM_GCSR_DFT_ETM_PWR_ON_mask              (0x1)
#define   NFP_ARM_GCSR_DFT_ETM_PWR_ON_bit               (16)
#define   NFP_ARM_GCSR_DFT_BIST_FAIL_bf                 0, 11, 8
#define   NFP_ARM_GCSR_DFT_BIST_FAIL_mask               (0xf)
#define   NFP_ARM_GCSR_DFT_BIST_FAIL_shift              (8)
#define   NFP_ARM_GCSR_DFT_BIST_DONE_bf                 0, 7, 4
#define   NFP_ARM_GCSR_DFT_BIST_DONE_mask               (0xf)
#define   NFP_ARM_GCSR_DFT_BIST_DONE_shift              (4)
#define   NFP_ARM_GCSR_DFT_BIST_RUN(_x)                 ((_x) & 0x7)
#define   NFP_ARM_GCSR_DFT_BIST_RUN_bf                  0, 2, 0
#define   NFP_ARM_GCSR_DFT_BIST_RUN_mask                (0x7)
#define   NFP_ARM_GCSR_DFT_BIST_RUN_shift               (0)

#endif /* __NFP_SDK__NFP3200_NFP_ARM_H__ */
