/*
 * Copyright (C) 2009 - 2012  Netronome Systems, Inc.  All rights reserved.
 *
 * @file        nfp3200/nfp_pl.h
 * @brief       Register definitions for PL unit
 *
 * Disclaimer: this file is provided without any warranty
 * as part of an early-access program, and the content is
 * bound to change before the final release.
 */

#ifndef __NFP_SDK__NFP3200_NFP_PL_H__
#define __NFP_SDK__NFP3200_NFP_PL_H__

/* HGID: nfp3200/pl.desc = 03600216d5c8 */
/* Register Type: PluResetsAndEnables */
#define NFP_PL_RE                      0x0008
#define   NFP_PL_RE_ARM_GASKET_RESET                    (0x1 << 31)
#define   NFP_PL_RE_ARM_GASKET_RESET_bf                 0, 31, 31
#define   NFP_PL_RE_ARM_GASKET_RESET_mask               (0x1)
#define   NFP_PL_RE_ARM_GASKET_RESET_bit                (31)
#define   NFP_PL_RE_ARM_GASKET_ENABLE                   (0x1 << 30)
#define   NFP_PL_RE_ARM_GASKET_ENABLE_bf                0, 30, 30
#define   NFP_PL_RE_ARM_GASKET_ENABLE_mask              (0x1)
#define   NFP_PL_RE_ARM_GASKET_ENABLE_bit               (30)
#define   NFP_PL_RE_MECL_ME_RESET(_x)                   (((_x) & 0x1f) << 25)
#define   NFP_PL_RE_MECL_ME_RESET_bf                    0, 29, 25
#define   NFP_PL_RE_MECL_ME_RESET_mask                  (0x1f)
#define   NFP_PL_RE_MECL_ME_RESET_shift                 (25)
#define   NFP_PL_RE_MECL_ME_ENABLE(_x)                  (((_x) & 0x1f) << 20)
#define   NFP_PL_RE_MECL_ME_ENABLE_bf                   0, 24, 20
#define   NFP_PL_RE_MECL_ME_ENABLE_mask                 (0x1f)
#define   NFP_PL_RE_MECL_ME_ENABLE_shift                (20)
#define   NFP_PL_RE_PCIE_ENABLE                         (0x1 << 19)
#define   NFP_PL_RE_PCIE_ENABLE_bf                      0, 19, 19
#define   NFP_PL_RE_PCIE_ENABLE_mask                    (0x1)
#define   NFP_PL_RE_PCIE_ENABLE_bit                     (19)
#define   NFP_PL_RE_ARM_CORE_ENABLE                     (0x1 << 18)
#define   NFP_PL_RE_ARM_CORE_ENABLE_bf                  0, 18, 18
#define   NFP_PL_RE_ARM_CORE_ENABLE_mask                (0x1)
#define   NFP_PL_RE_ARM_CORE_ENABLE_bit                 (18)
#define   NFP_PL_RE_PCIE_RESET                          (0x1 << 17)
#define   NFP_PL_RE_PCIE_RESET_bf                       0, 17, 17
#define   NFP_PL_RE_PCIE_RESET_mask                     (0x1)
#define   NFP_PL_RE_PCIE_RESET_bit                      (17)
#define   NFP_PL_RE_ARM_CORE_RESET                      (0x1 << 16)
#define   NFP_PL_RE_ARM_CORE_RESET_bf                   0, 16, 16
#define   NFP_PL_RE_ARM_CORE_RESET_mask                 (0x1)
#define   NFP_PL_RE_ARM_CORE_RESET_bit                  (16)
#define   NFP_PL_RE_MSF0_RESET                          (0x1 << 15)
#define   NFP_PL_RE_MSF0_RESET_bf                       0, 15, 15
#define   NFP_PL_RE_MSF0_RESET_mask                     (0x1)
#define   NFP_PL_RE_MSF0_RESET_bit                      (15)
#define   NFP_PL_RE_MSF1_RESET                          (0x1 << 14)
#define   NFP_PL_RE_MSF1_RESET_bf                       0, 14, 14
#define   NFP_PL_RE_MSF1_RESET_mask                     (0x1)
#define   NFP_PL_RE_MSF1_RESET_bit                      (14)
#define   NFP_PL_RE_CRYPTO_RESET                        (0x1 << 13)
#define   NFP_PL_RE_CRYPTO_RESET_bf                     0, 13, 13
#define   NFP_PL_RE_CRYPTO_RESET_mask                   (0x1)
#define   NFP_PL_RE_CRYPTO_RESET_bit                    (13)
#define   NFP_PL_RE_QDR0_RESET                          (0x1 << 12)
#define   NFP_PL_RE_QDR0_RESET_bf                       0, 12, 12
#define   NFP_PL_RE_QDR0_RESET_mask                     (0x1)
#define   NFP_PL_RE_QDR0_RESET_bit                      (12)
#define   NFP_PL_RE_QDR1_RESET                          (0x1 << 11)
#define   NFP_PL_RE_QDR1_RESET_bf                       0, 11, 11
#define   NFP_PL_RE_QDR1_RESET_mask                     (0x1)
#define   NFP_PL_RE_QDR1_RESET_bit                      (11)
#define   NFP_PL_RE_DDR0_RESET                          (0x1 << 10)
#define   NFP_PL_RE_DDR0_RESET_bf                       0, 10, 10
#define   NFP_PL_RE_DDR0_RESET_mask                     (0x1)
#define   NFP_PL_RE_DDR0_RESET_bit                      (10)
#define   NFP_PL_RE_DDR1_RESET                          (0x1 << 9)
#define   NFP_PL_RE_DDR1_RESET_bf                       0, 9, 9
#define   NFP_PL_RE_DDR1_RESET_mask                     (0x1)
#define   NFP_PL_RE_DDR1_RESET_bit                      (9)
#define   NFP_PL_RE_MU_RESET                            (0x1 << 8)
#define   NFP_PL_RE_MU_RESET_bf                         0, 8, 8
#define   NFP_PL_RE_MU_RESET_mask                       (0x1)
#define   NFP_PL_RE_MU_RESET_bit                        (8)
#define   NFP_PL_RE_MSF0_ENABLE                         (0x1 << 7)
#define   NFP_PL_RE_MSF0_ENABLE_bf                      0, 7, 7
#define   NFP_PL_RE_MSF0_ENABLE_mask                    (0x1)
#define   NFP_PL_RE_MSF0_ENABLE_bit                     (7)
#define   NFP_PL_RE_MSF1_ENABLE                         (0x1 << 6)
#define   NFP_PL_RE_MSF1_ENABLE_bf                      0, 6, 6
#define   NFP_PL_RE_MSF1_ENABLE_mask                    (0x1)
#define   NFP_PL_RE_MSF1_ENABLE_bit                     (6)
#define   NFP_PL_RE_CRYPTO_ENABLE                       (0x1 << 5)
#define   NFP_PL_RE_CRYPTO_ENABLE_bf                    0, 5, 5
#define   NFP_PL_RE_CRYPTO_ENABLE_mask                  (0x1)
#define   NFP_PL_RE_CRYPTO_ENABLE_bit                   (5)
#define   NFP_PL_RE_QDR0_ENABLE                         (0x1 << 4)
#define   NFP_PL_RE_QDR0_ENABLE_bf                      0, 4, 4
#define   NFP_PL_RE_QDR0_ENABLE_mask                    (0x1)
#define   NFP_PL_RE_QDR0_ENABLE_bit                     (4)
#define   NFP_PL_RE_QDR1_ENABLE                         (0x1 << 3)
#define   NFP_PL_RE_QDR1_ENABLE_bf                      0, 3, 3
#define   NFP_PL_RE_QDR1_ENABLE_mask                    (0x1)
#define   NFP_PL_RE_QDR1_ENABLE_bit                     (3)
#define   NFP_PL_RE_DDR0_ENABLE                         (0x1 << 2)
#define   NFP_PL_RE_DDR0_ENABLE_bf                      0, 2, 2
#define   NFP_PL_RE_DDR0_ENABLE_mask                    (0x1)
#define   NFP_PL_RE_DDR0_ENABLE_bit                     (2)
#define   NFP_PL_RE_DDR1_ENABLE                         (0x1 << 1)
#define   NFP_PL_RE_DDR1_ENABLE_bf                      0, 1, 1
#define   NFP_PL_RE_DDR1_ENABLE_mask                    (0x1)
#define   NFP_PL_RE_DDR1_ENABLE_bit                     (1)
#define   NFP_PL_RE_MU_ENABLE                           (0x1)
#define   NFP_PL_RE_MU_ENABLE_bf                        0, 0, 0
#define   NFP_PL_RE_MU_ENABLE_mask                      (0x1)
#define   NFP_PL_RE_MU_ENABLE_bit                       (0)
/* Register Type: PluConfig0 */
#define NFP_PL_CFG0                    0x000c
#define   NFP_PL_CFG0_DIV_STG1_bf                       0, 24, 20
#define   NFP_PL_CFG0_DIV_STG1_mask                     (0x1f)
#define   NFP_PL_CFG0_DIV_STG1_shift                    (20)
#define   NFP_PL_CFG0_DIV_STG2_bf                       0, 19, 15
#define   NFP_PL_CFG0_DIV_STG2_mask                     (0x1f)
#define   NFP_PL_CFG0_DIV_STG2_shift                    (15)
#define   NFP_PL_CFG0_MUL_PLL0_bf                       0, 14, 10
#define   NFP_PL_CFG0_MUL_PLL0_mask                     (0x1f)
#define   NFP_PL_CFG0_MUL_PLL0_shift                    (10)
#define   NFP_PL_CFG0_DIV_ME_bf                         0, 9, 5
#define   NFP_PL_CFG0_DIV_ME_mask                       (0x1f)
#define   NFP_PL_CFG0_DIV_ME_shift                      (5)
#define   NFP_PL_CFG0_DIV_SRAM(_x)                      ((_x) & 0x1f)
#define   NFP_PL_CFG0_DIV_SRAM_bf                       0, 4, 0
#define   NFP_PL_CFG0_DIV_SRAM_mask                     (0x1f)
#define   NFP_PL_CFG0_DIV_SRAM_shift                    (0)
/* Register Type: PluFuses */
#define NFP_PL_FUSE                    0x0010
#define   NFP_PL_FUSE_MSF1_ENABLE                       (0x1 << 9)
#define   NFP_PL_FUSE_MSF1_ENABLE_bf                    0, 9, 9
#define   NFP_PL_FUSE_MSF1_ENABLE_mask                  (0x1)
#define   NFP_PL_FUSE_MSF1_ENABLE_bit                   (9)
#define   NFP_PL_FUSE_CRYPTO_ENABLE                     (0x1 << 8)
#define   NFP_PL_FUSE_CRYPTO_ENABLE_bf                  0, 8, 8
#define   NFP_PL_FUSE_CRYPTO_ENABLE_mask                (0x1)
#define   NFP_PL_FUSE_CRYPTO_ENABLE_bit                 (8)
#define   NFP_PL_FUSE_QDR0_ENABLE                       (0x1 << 7)
#define   NFP_PL_FUSE_QDR0_ENABLE_bf                    0, 7, 7
#define   NFP_PL_FUSE_QDR0_ENABLE_mask                  (0x1)
#define   NFP_PL_FUSE_QDR0_ENABLE_bit                   (7)
#define   NFP_PL_FUSE_QDR1_ENABLE                       (0x1 << 6)
#define   NFP_PL_FUSE_QDR1_ENABLE_bf                    0, 6, 6
#define   NFP_PL_FUSE_QDR1_ENABLE_mask                  (0x1)
#define   NFP_PL_FUSE_QDR1_ENABLE_bit                   (6)
#define   NFP_PL_FUSE_DDR1_ENABLE                       (0x1 << 5)
#define   NFP_PL_FUSE_DDR1_ENABLE_bf                    0, 5, 5
#define   NFP_PL_FUSE_DDR1_ENABLE_mask                  (0x1)
#define   NFP_PL_FUSE_DDR1_ENABLE_bit                   (5)
#define   NFP_PL_FUSE_MECL_ME_ENABLE_bf                 0, 4, 2
#define   NFP_PL_FUSE_MECL_ME_ENABLE_mask               (0x7)
#define   NFP_PL_FUSE_MECL_ME_ENABLE_shift              (2)
#define     NFP_PL_FUSE_MECL_ME_ENABLE_MES_8            (0)
#define     NFP_PL_FUSE_MECL_ME_ENABLE_MES_16           (1)
#define     NFP_PL_FUSE_MECL_ME_ENABLE_MES_24           (2)
#define     NFP_PL_FUSE_MECL_ME_ENABLE_MES_32           (3)
#define     NFP_PL_FUSE_MECL_ME_ENABLE_MES_40           (4)
#define   NFP_PL_FUSE_SPEED_bf                          0, 1, 0
#define   NFP_PL_FUSE_SPEED_mask                        (0x3)
#define   NFP_PL_FUSE_SPEED_shift                       (0)
#define     NFP_PL_FUSE_SPEED_SLOW                      (0)
#define     NFP_PL_FUSE_SPEED_MEDIUM                    (1)
#define     NFP_PL_FUSE_SPEED_FAST                      (2)
#define     NFP_PL_FUSE_SPEED_UNLIMITED                 (3)
/* Register Type: PluConfig1 */
#define NFP_PL_CFG1                    0x0014
#define   NFP_PL_CFG1_DIV_SPICLK0(_x)                   (((_x) & 0x1f) << 25)
#define   NFP_PL_CFG1_DIV_SPICLK0_bf                    0, 29, 25
#define   NFP_PL_CFG1_DIV_SPICLK0_mask                  (0x1f)
#define   NFP_PL_CFG1_DIV_SPICLK0_shift                 (25)
#define   NFP_PL_CFG1_DIV_SPICLK1(_x)                   (((_x) & 0x1f) << 20)
#define   NFP_PL_CFG1_DIV_SPICLK1_bf                    0, 24, 20
#define   NFP_PL_CFG1_DIV_SPICLK1_mask                  (0x1f)
#define   NFP_PL_CFG1_DIV_SPICLK1_shift                 (20)
#define   NFP_PL_CFG1_DIV_CPP(_x)                       (((_x) & 0x1f) << 15)
#define   NFP_PL_CFG1_DIV_CPP_bf                        0, 19, 15
#define   NFP_PL_CFG1_DIV_CPP_mask                      (0x1f)
#define   NFP_PL_CFG1_DIV_CPP_shift                     (15)
#define   NFP_PL_CFG1_DIV_DCLK(_x)                      (((_x) & 0x1f) << 10)
#define   NFP_PL_CFG1_DIV_DCLK_bf                       0, 14, 10
#define   NFP_PL_CFG1_DIV_DCLK_mask                     (0x1f)
#define   NFP_PL_CFG1_DIV_DCLK_shift                    (10)
#define   NFP_PL_CFG1_DIV_XCLK0(_x)                     (((_x) & 0x1f) << 5)
#define   NFP_PL_CFG1_DIV_XCLK0_bf                      0, 9, 5
#define   NFP_PL_CFG1_DIV_XCLK0_mask                    (0x1f)
#define   NFP_PL_CFG1_DIV_XCLK0_shift                   (5)
#define   NFP_PL_CFG1_DIV_XCLK1(_x)                     ((_x) & 0x1f)
#define   NFP_PL_CFG1_DIV_XCLK1_bf                      0, 4, 0
#define   NFP_PL_CFG1_DIV_XCLK1_mask                    (0x1f)
#define   NFP_PL_CFG1_DIV_XCLK1_shift                   (0)
/* Register Type: PluStraps */
#define NFP_PL_STRAPS                  0x0018
#define   NFP_PL_STRAPS_CFG_RST_DIR(_x)                 (((_x) & 0x3) << 7)
#define   NFP_PL_STRAPS_CFG_RST_DIR_bf                  0, 8, 7
#define   NFP_PL_STRAPS_CFG_RST_DIR_mask                (0x3)
#define   NFP_PL_STRAPS_CFG_RST_DIR_shift               (7)
#define     NFP_PL_STRAPS_CFG_RST_DIR_INPUT_RESET_CHIP  (0)
#define     NFP_PL_STRAPS_CFG_RST_DIR_INPUT_RESET_PCIE  (1)
#define     NFP_PL_STRAPS_CFG_RST_DIR_OUTPUT            (2)
#define     NFP_PL_STRAPS_CFG_RST_DIR_INPUT_OUTPUT_RESET_PCIE (3)
#define   NFP_PL_STRAPS_CFG_PCI_ROOT                    (0x1 << 6)
#define   NFP_PL_STRAPS_CFG_PCI_ROOT_bf                 0, 6, 6
#define   NFP_PL_STRAPS_CFG_PCI_ROOT_mask               (0x1)
#define   NFP_PL_STRAPS_CFG_PCI_ROOT_bit                (6)
#define     NFP_PL_STRAPS_CFG_PCI_ROOT_EP               (0x0)
#define     NFP_PL_STRAPS_CFG_PCI_ROOT_RC               (0x40)
#define   NFP_PL_STRAPS_CFG_PROM_BOOT                   (0x1 << 5)
#define   NFP_PL_STRAPS_CFG_PROM_BOOT_bf                0, 5, 5
#define   NFP_PL_STRAPS_CFG_PROM_BOOT_mask              (0x1)
#define   NFP_PL_STRAPS_CFG_PROM_BOOT_bit               (5)
#define     NFP_PL_STRAPS_CFG_PROM_BOOT_XXX_1           (0x0)
#define     NFP_PL_STRAPS_CFG_PROM_BOOT_XXX_2           (0x20)
#define   NFP_PL_STRAPS_PLL0_MULIPLIER(_x)              ((_x) & 0x1f)
#define   NFP_PL_STRAPS_PLL0_MULIPLIER_bf               0, 4, 0
#define   NFP_PL_STRAPS_PLL0_MULIPLIER_mask             (0x1f)
#define   NFP_PL_STRAPS_PLL0_MULIPLIER_shift            (0)
/* Register Type: PluPassword */
#define NFP_PL_PASSWORD1               0x001c
#define NFP_PL_PASSWORD2               0x0020
/* Register Type: PluShacAndMEResetsAndEnables */
#define NFP_PL_RE2                     0x0024
#define   NFP_PL_RE2_PCIE_RESET                         (0x1 << 12)
#define   NFP_PL_RE2_PCIE_RESET_bf                      0, 12, 12
#define   NFP_PL_RE2_PCIE_RESET_mask                    (0x1)
#define   NFP_PL_RE2_PCIE_RESET_bit                     (12)
#define   NFP_PL_RE2_SHAC_RESET                         (0x1 << 11)
#define   NFP_PL_RE2_SHAC_RESET_bf                      0, 11, 11
#define   NFP_PL_RE2_SHAC_RESET_mask                    (0x1)
#define   NFP_PL_RE2_SHAC_RESET_bit                     (11)
#define   NFP_PL_RE2_SHAC_ENABLE                        (0x1 << 10)
#define   NFP_PL_RE2_SHAC_ENABLE_bf                     0, 10, 10
#define   NFP_PL_RE2_SHAC_ENABLE_mask                   (0x1)
#define   NFP_PL_RE2_SHAC_ENABLE_bit                    (10)
#define   NFP_PL_RE2_MECL_CTL_RESET(_x)                 (((_x) & 0x1f) << 5)
#define   NFP_PL_RE2_MECL_CTL_RESET_bf                  0, 9, 5
#define   NFP_PL_RE2_MECL_CTL_RESET_mask                (0x1f)
#define   NFP_PL_RE2_MECL_CTL_RESET_shift               (5)
#define   NFP_PL_RE2_MECL_CTL_ENABLE(_x)                ((_x) & 0x1f)
#define   NFP_PL_RE2_MECL_CTL_ENABLE_bf                 0, 4, 0
#define   NFP_PL_RE2_MECL_CTL_ENABLE_mask               (0x1f)
#define   NFP_PL_RE2_MECL_CTL_ENABLE_shift              (0)
/* Register Type: PluConfig2 */
#define NFP_PL_CFG2                    0x0028
#define   NFP_PL_CFG2_SOFT_RESET                        (0x1 << 15)
#define   NFP_PL_CFG2_SOFT_RESET_bf                     0, 15, 15
#define   NFP_PL_CFG2_SOFT_RESET_mask                   (0x1)
#define   NFP_PL_CFG2_SOFT_RESET_bit                    (15)
#define   NFP_PL_CFG2_DIV_OCLK(_x)                      (((_x) & 0x1f) << 10)
#define   NFP_PL_CFG2_DIV_OCLK_bf                       0, 14, 10
#define   NFP_PL_CFG2_DIV_OCLK_mask                     (0x1f)
#define   NFP_PL_CFG2_DIV_OCLK_shift                    (10)
#define   NFP_PL_CFG2_MUL_PLL1(_x)                      (((_x) & 0x1f) << 5)
#define   NFP_PL_CFG2_MUL_PLL1_bf                       0, 9, 5
#define   NFP_PL_CFG2_MUL_PLL1_mask                     (0x1f)
#define   NFP_PL_CFG2_MUL_PLL1_shift                    (5)
#define   NFP_PL_CFG2_MUL_PLL2(_x)                      ((_x) & 0x1f)
#define   NFP_PL_CFG2_MUL_PLL2_bf                       0, 4, 0
#define   NFP_PL_CFG2_MUL_PLL2_mask                     (0x1f)
#define   NFP_PL_CFG2_MUL_PLL2_shift                    (0)
/* Register Type: PluPLLTuning */
#define NFP_PL_PLL0_TUNING             0x0030
#define NFP_PL_PLL1_TUNING             0x0034
#define NFP_PL_PLL2_TUNING             0x0038
/* Register Type: PLLEnsat */
#define NFP_PL_PLL0_ENSAT              0x003c
#define NFP_PL_PLL1_ENSAT              0x0040
#define NFP_PL_PLL2_ENSAT              0x0044
/* Register Type: PluBisr */
#define NFP_PL_BISR                    0x0048
/* Register Type: PluJTagIdCode */
#define NFP_PL_JTAG_ID_CODE            0x004c
#define   NFP_PL_JTAG_ID_CODE_REV_ID_bf                 0, 31, 28
#define   NFP_PL_JTAG_ID_CODE_REV_ID_mask               (0xf)
#define   NFP_PL_JTAG_ID_CODE_REV_ID_shift              (28)
#define   NFP_PL_JTAG_ID_CODE_PART_NUM_bf               0, 27, 12
#define   NFP_PL_JTAG_ID_CODE_PART_NUM_mask             (0xffff)
#define   NFP_PL_JTAG_ID_CODE_PART_NUM_shift            (12)
#define   NFP_PL_JTAG_ID_CODE_MFR_ID_bf                 0, 11, 1
#define   NFP_PL_JTAG_ID_CODE_MFR_ID_mask               (0x7ff)
#define   NFP_PL_JTAG_ID_CODE_MFR_ID_shift              (1)

#endif /* __NFP_SDK__NFP3200_NFP_PL_H__ */
