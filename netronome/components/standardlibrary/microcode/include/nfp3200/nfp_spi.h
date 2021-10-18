/*
 * Copyright (C) 2012  Netronome Systems, Inc.  All rights reserved.
 *
 * @file        nfp3200/nfp_spi.h
 * @brief       Register definitions for SPI
 *
 * Disclaimer: this file is provided without any warranty
 * as part of an early-access program, and the content is
 * bound to change before the final release.
 */

#ifndef __NFP_SDK__NFP3200_NFP_SPI_H__
#define __NFP_SDK__NFP3200_NFP_SPI_H__

/* HGID: nfp3200/spi.desc = bf45aaeb59b3 */
/* Register Type: SerialConfigSpiA */
#define NFP_CAP_SPI_CTL0_DEV0_CFG0_SPI 0x0aa0
#define NFP_CAP_SPI_CTL0_DEV1_CFG0_SPI 0x0aa4
#define NFP_CAP_SPI_CTL0_DEV2_CFG0_SPI 0x0aa8
#define NFP_CAP_SPI_CTL0_DEV3_CFG0_SPI 0x0aac
#define NFP_CAP_SPI_CTL0_DEV0_CFG1_SPI 0x0ac0
#define NFP_CAP_SPI_CTL0_DEV1_CFG1_SPI 0x0ac4
#define NFP_CAP_SPI_CTL0_DEV2_CFG1_SPI 0x0ac8
#define NFP_CAP_SPI_CTL0_DEV3_CFG1_SPI 0x0acc
#define NFP_CAP_SPI_CTL0_DEV0_CFG2_SPI 0x0ae0
#define NFP_CAP_SPI_CTL0_DEV1_CFG2_SPI 0x0ae4
#define NFP_CAP_SPI_CTL0_DEV2_CFG2_SPI 0x0ae8
#define NFP_CAP_SPI_CTL0_DEV3_CFG2_SPI 0x0aec
#define   NFP_CAP_SPI_CTL0_SPI_DATA_LEN(_x)             (((_x) & 0x3f) << 26)
#define   NFP_CAP_SPI_CTL0_SPI_DATA_LEN_bf              0, 31, 26
#define   NFP_CAP_SPI_CTL0_SPI_DATA_LEN_mask            (0x3f)
#define   NFP_CAP_SPI_CTL0_SPI_DATA_LEN_shift           (26)
#define   NFP_CAP_SPI_CTL0_SPI_CMD_LEN(_x)              (((_x) & 0xf) << 22)
#define   NFP_CAP_SPI_CTL0_SPI_CMD_LEN_bf               0, 25, 22
#define   NFP_CAP_SPI_CTL0_SPI_CMD_LEN_mask             (0xf)
#define   NFP_CAP_SPI_CTL0_SPI_CMD_LEN_shift            (22)
#define   NFP_CAP_SPI_CTL0_SPI_ADDR_LEN(_x)             (((_x) & 0x1f) << 16)
#define   NFP_CAP_SPI_CTL0_SPI_ADDR_LEN_bf              0, 20, 16
#define   NFP_CAP_SPI_CTL0_SPI_ADDR_LEN_mask            (0x1f)
#define   NFP_CAP_SPI_CTL0_SPI_ADDR_LEN_shift           (16)
#define   NFP_CAP_SPI_CTL0_SPI_ALT_READ_CMD(_x)         (((_x) & 0xff) << 8)
#define   NFP_CAP_SPI_CTL0_SPI_ALT_READ_CMD_bf          0, 15, 8
#define   NFP_CAP_SPI_CTL0_SPI_ALT_READ_CMD_mask        (0xff)
#define   NFP_CAP_SPI_CTL0_SPI_ALT_READ_CMD_shift       (8)
#define   NFP_CAP_SPI_CTL0_SPI_ALT_WRITE_CMD(_x)        ((_x) & 0xff)
#define   NFP_CAP_SPI_CTL0_SPI_ALT_WRITE_CMD_bf         0, 7, 0
#define   NFP_CAP_SPI_CTL0_SPI_ALT_WRITE_CMD_mask       (0xff)
#define   NFP_CAP_SPI_CTL0_SPI_ALT_WRITE_CMD_shift      (0)
/* Register Type: SerialConfigMdioA */
#define NFP_CAP_SPI_CTL0_DEV0_CFG0_MDIO 0x0aa0
#define NFP_CAP_SPI_CTL0_DEV1_CFG0_MDIO 0x0aa4
#define NFP_CAP_SPI_CTL0_DEV2_CFG0_MDIO 0x0aa8
#define NFP_CAP_SPI_CTL0_DEV3_CFG0_MDIO 0x0aac
#define NFP_CAP_SPI_CTL0_DEV0_CFG1_MDIO 0x0ac0
#define NFP_CAP_SPI_CTL0_DEV1_CFG1_MDIO 0x0ac4
#define NFP_CAP_SPI_CTL0_DEV2_CFG1_MDIO 0x0ac8
#define NFP_CAP_SPI_CTL0_DEV3_CFG1_MDIO 0x0acc
#define NFP_CAP_SPI_CTL0_DEV0_CFG2_MDIO 0x0ae0
#define NFP_CAP_SPI_CTL0_DEV1_CFG2_MDIO 0x0ae4
#define NFP_CAP_SPI_CTL0_DEV2_CFG2_MDIO 0x0ae8
#define NFP_CAP_SPI_CTL0_DEV3_CFG2_MDIO 0x0aec
#define   NFP_CAP_SPI_CTL0_MDIO_DATA_LEN(_x)            (((_x) & 0x3f) << 26)
#define   NFP_CAP_SPI_CTL0_MDIO_DATA_LEN_bf             0, 31, 26
#define   NFP_CAP_SPI_CTL0_MDIO_DATA_LEN_mask           (0x3f)
#define   NFP_CAP_SPI_CTL0_MDIO_DATA_LEN_shift          (26)
#define   NFP_CAP_SPI_CTL0_MDIO_TA_LEN(_x)              (((_x) & 0x3) << 24)
#define   NFP_CAP_SPI_CTL0_MDIO_TA_LEN_bf               0, 25, 24
#define   NFP_CAP_SPI_CTL0_MDIO_TA_LEN_mask             (0x3)
#define   NFP_CAP_SPI_CTL0_MDIO_TA_LEN_shift            (24)
#define   NFP_CAP_SPI_CTL0_MDIO_HDR_LEN(_x)             (((_x) & 0x1f) << 16)
#define   NFP_CAP_SPI_CTL0_MDIO_HDR_LEN_bf              0, 20, 16
#define   NFP_CAP_SPI_CTL0_MDIO_HDR_LEN_mask            (0x1f)
#define   NFP_CAP_SPI_CTL0_MDIO_HDR_LEN_shift           (16)
#define   NFP_CAP_SPI_CTL0_MDIO_HDR_HI(_x)              (((_x) & 0xff) << 8)
#define   NFP_CAP_SPI_CTL0_MDIO_HDR_HI_bf               0, 15, 8
#define   NFP_CAP_SPI_CTL0_MDIO_HDR_HI_mask             (0xff)
#define   NFP_CAP_SPI_CTL0_MDIO_HDR_HI_shift            (8)
#define   NFP_CAP_SPI_CTL0_MDIO_HDR_LO(_x)              ((_x) & 0xff)
#define   NFP_CAP_SPI_CTL0_MDIO_HDR_LO_bf               0, 7, 0
#define   NFP_CAP_SPI_CTL0_MDIO_HDR_LO_mask             (0xff)
#define   NFP_CAP_SPI_CTL0_MDIO_HDR_LO_shift            (0)
/* Register Type: SerialConfigMdioB */
#define NFP_CAP_SPI_CTL1_DEV0_CFG0_MDIO 0x0ab0
#define NFP_CAP_SPI_CTL1_DEV1_CFG0_MDIO 0x0ab4
#define NFP_CAP_SPI_CTL1_DEV2_CFG0_MDIO 0x0ab8
#define NFP_CAP_SPI_CTL1_DEV3_CFG0_MDIO 0x0abc
#define NFP_CAP_SPI_CTL1_DEV0_CFG1_MDIO 0x0ad0
#define NFP_CAP_SPI_CTL1_DEV1_CFG1_MDIO 0x0ad4
#define NFP_CAP_SPI_CTL1_DEV2_CFG1_MDIO 0x0ad8
#define NFP_CAP_SPI_CTL1_DEV3_CFG1_MDIO 0x0adc
#define NFP_CAP_SPI_CTL1_DEV0_CFG2_MDIO 0x0af0
#define NFP_CAP_SPI_CTL1_DEV1_CFG2_MDIO 0x0af4
#define NFP_CAP_SPI_CTL1_DEV2_CFG2_MDIO 0x0af8
#define NFP_CAP_SPI_CTL1_DEV3_CFG2_MDIO 0x0afc
#define   NFP_CAP_SPI_CTL1_MDIO_SELECT                  (0x1 << 31)
#define   NFP_CAP_SPI_CTL1_MDIO_SELECT_bf               0, 31, 31
#define   NFP_CAP_SPI_CTL1_MDIO_SELECT_mask             (0x1)
#define   NFP_CAP_SPI_CTL1_MDIO_SELECT_bit              (31)
#define     NFP_CAP_SPI_CTL1_MDIO_SELECT_SPI            (0x0)
#define     NFP_CAP_SPI_CTL1_MDIO_SELECT_MDIO           (0x80000000)
#define   NFP_CAP_SPI_CTL1_MDIO_PREAMBLE                (0x1 << 30)
#define   NFP_CAP_SPI_CTL1_MDIO_PREAMBLE_bf             0, 30, 30
#define   NFP_CAP_SPI_CTL1_MDIO_PREAMBLE_mask           (0x1)
#define   NFP_CAP_SPI_CTL1_MDIO_PREAMBLE_bit            (30)
#define     NFP_CAP_SPI_CTL1_MDIO_PREAMBLE_DISABLE      (0x0)
#define     NFP_CAP_SPI_CTL1_MDIO_PREAMBLE_ENABLE       (0x40000000)
#define   NFP_CAP_SPI_CTL1_MDIO_TA_BITS(_x)             (((_x) & 0x7) << 24)
#define   NFP_CAP_SPI_CTL1_MDIO_TA_BITS_bf              0, 26, 24
#define   NFP_CAP_SPI_CTL1_MDIO_TA_BITS_mask            (0x7)
#define   NFP_CAP_SPI_CTL1_MDIO_TA_BITS_shift           (24)
#define   NFP_CAP_SPI_CTL1_MDIO_BUSY                    (0x1 << 21)
#define   NFP_CAP_SPI_CTL1_MDIO_BUSY_bf                 0, 21, 21
#define   NFP_CAP_SPI_CTL1_MDIO_BUSY_mask               (0x1)
#define   NFP_CAP_SPI_CTL1_MDIO_BUSY_bit                (21)
#define     NFP_CAP_SPI_CTL1_MDIO_BUSY_NOT_BUSY         (0x0)
#define     NFP_CAP_SPI_CTL1_MDIO_BUSY_BUSY             (0x200000)
#define   NFP_CAP_SPI_CTL1_MDIO_HDR_23_16(_x)           (((_x) & 0xff) << 12)
#define   NFP_CAP_SPI_CTL1_MDIO_HDR_23_16_bf            0, 19, 12
#define   NFP_CAP_SPI_CTL1_MDIO_HDR_23_16_mask          (0xff)
#define   NFP_CAP_SPI_CTL1_MDIO_HDR_23_16_shift         (12)
#define   NFP_CAP_SPI_CTL1_MDIO_DATA_DRIVE_CTRL         (0x1 << 10)
#define   NFP_CAP_SPI_CTL1_MDIO_DATA_DRIVE_CTRL_bf      0, 10, 10
#define   NFP_CAP_SPI_CTL1_MDIO_DATA_DRIVE_CTRL_mask    (0x1)
#define   NFP_CAP_SPI_CTL1_MDIO_DATA_DRIVE_CTRL_bit     (10)
#define     NFP_CAP_SPI_CTL1_MDIO_DATA_DRIVE_CTRL_OPEN_DRAIN (0x0)
#define     NFP_CAP_SPI_CTL1_MDIO_DATA_DRIVE_CTRL_PUSH_PULL (0x400)
#define   NFP_CAP_SPI_CTL1_MDIO_DEV_PCLK_DIV(_x)        ((_x) & 0x3ff)
#define   NFP_CAP_SPI_CTL1_MDIO_DEV_PCLK_DIV_bf         0, 9, 0
#define   NFP_CAP_SPI_CTL1_MDIO_DEV_PCLK_DIV_mask       (0x3ff)
#define   NFP_CAP_SPI_CTL1_MDIO_DEV_PCLK_DIV_shift      (0)
/* Register Type: SerialConfigSpiB */
#define NFP_CAP_SPI_CTL1_DEV0_CFG0_SPI 0x0ab0
#define NFP_CAP_SPI_CTL1_DEV1_CFG0_SPI 0x0ab4
#define NFP_CAP_SPI_CTL1_DEV2_CFG0_SPI 0x0ab8
#define NFP_CAP_SPI_CTL1_DEV3_CFG0_SPI 0x0abc
#define NFP_CAP_SPI_CTL1_DEV0_CFG1_SPI 0x0ad0
#define NFP_CAP_SPI_CTL1_DEV1_CFG1_SPI 0x0ad4
#define NFP_CAP_SPI_CTL1_DEV2_CFG1_SPI 0x0ad8
#define NFP_CAP_SPI_CTL1_DEV3_CFG1_SPI 0x0adc
#define NFP_CAP_SPI_CTL1_DEV0_CFG2_SPI 0x0af0
#define NFP_CAP_SPI_CTL1_DEV1_CFG2_SPI 0x0af4
#define NFP_CAP_SPI_CTL1_DEV2_CFG2_SPI 0x0af8
#define NFP_CAP_SPI_CTL1_DEV3_CFG2_SPI 0x0afc
#define   NFP_CAP_SPI_CTL1_SPI_SELECT                   (0x1 << 31)
#define   NFP_CAP_SPI_CTL1_SPI_SELECT_bf                0, 31, 31
#define   NFP_CAP_SPI_CTL1_SPI_SELECT_mask              (0x1)
#define   NFP_CAP_SPI_CTL1_SPI_SELECT_bit               (31)
#define     NFP_CAP_SPI_CTL1_SPI_SELECT_SPI             (0x0)
#define     NFP_CAP_SPI_CTL1_SPI_SELECT_MDIO            (0x80000000)
#define   NFP_CAP_SPI_CTL1_SPI_ALT_READ                 (0x1 << 30)
#define   NFP_CAP_SPI_CTL1_SPI_ALT_READ_bf              0, 30, 30
#define   NFP_CAP_SPI_CTL1_SPI_ALT_READ_mask            (0x1)
#define   NFP_CAP_SPI_CTL1_SPI_ALT_READ_bit             (30)
#define     NFP_CAP_SPI_CTL1_SPI_ALT_READ_DISABLED      (0x0)
#define     NFP_CAP_SPI_CTL1_SPI_ALT_READ_ENABLED       (0x40000000)
#define   NFP_CAP_SPI_CTL1_SPI_ALT_WRITE                (0x1 << 29)
#define   NFP_CAP_SPI_CTL1_SPI_ALT_WRITE_bf             0, 29, 29
#define   NFP_CAP_SPI_CTL1_SPI_ALT_WRITE_mask           (0x1)
#define   NFP_CAP_SPI_CTL1_SPI_ALT_WRITE_bit            (29)
#define     NFP_CAP_SPI_CTL1_SPI_ALT_WRITE_DISABLED     (0x0)
#define     NFP_CAP_SPI_CTL1_SPI_ALT_WRITE_ENABLED      (0x20000000)
#define   NFP_CAP_SPI_CTL1_SPI_ENABLE_DELAY(_x)         (((_x) & 0x1f) << 24)
#define   NFP_CAP_SPI_CTL1_SPI_ENABLE_DELAY_bf          0, 28, 24
#define   NFP_CAP_SPI_CTL1_SPI_ENABLE_DELAY_mask        (0x1f)
#define   NFP_CAP_SPI_CTL1_SPI_ENABLE_DELAY_shift       (24)
#define   NFP_CAP_SPI_CTL1_SPI_ADDR_TRANS_BITS(_x)      (((_x) & 0x3) << 22)
#define   NFP_CAP_SPI_CTL1_SPI_ADDR_TRANS_BITS_bf       0, 23, 22
#define   NFP_CAP_SPI_CTL1_SPI_ADDR_TRANS_BITS_mask     (0x3)
#define   NFP_CAP_SPI_CTL1_SPI_ADDR_TRANS_BITS_shift    (22)
#define   NFP_CAP_SPI_CTL1_SPI_BUSY                     (0x1 << 21)
#define   NFP_CAP_SPI_CTL1_SPI_BUSY_bf                  0, 21, 21
#define   NFP_CAP_SPI_CTL1_SPI_BUSY_mask                (0x1)
#define   NFP_CAP_SPI_CTL1_SPI_BUSY_bit                 (21)
#define     NFP_CAP_SPI_CTL1_SPI_BUSY_NOT_BUSY          (0x0)
#define     NFP_CAP_SPI_CTL1_SPI_BUSY_BUSY              (0x200000)
#define   NFP_CAP_SPI_CTL1_SPI_SERIAL_BOOT_PROM_WRITE   (0x1 << 20)
#define   NFP_CAP_SPI_CTL1_SPI_SERIAL_BOOT_PROM_WRITE_bf 0, 20, 20
#define   NFP_CAP_SPI_CTL1_SPI_SERIAL_BOOT_PROM_WRITE_mask (0x1)
#define   NFP_CAP_SPI_CTL1_SPI_SERIAL_BOOT_PROM_WRITE_bit (20)
#define     NFP_CAP_SPI_CTL1_SPI_SERIAL_BOOT_PROM_WRITE_DISCARD (0x0)
#define     NFP_CAP_SPI_CTL1_SPI_SERIAL_BOOT_PROM_WRITE_ENABLE (0x100000)
#define   NFP_CAP_SPI_CTL1_SPI_MODE_SELECT(_x)          (((_x) & 0x3) << 18)
#define   NFP_CAP_SPI_CTL1_SPI_MODE_SELECT_bf           0, 19, 18
#define   NFP_CAP_SPI_CTL1_SPI_MODE_SELECT_mask         (0x3)
#define   NFP_CAP_SPI_CTL1_SPI_MODE_SELECT_shift        (18)
#define     NFP_CAP_SPI_CTL1_SPI_MODE_SELECT_FIRST_EDGE_CLK0 (0)
#define     NFP_CAP_SPI_CTL1_SPI_MODE_SELECT_SECOND_EDGE_CLK0 (1)
#define     NFP_CAP_SPI_CTL1_SPI_MODE_SELECT_FIRST_EDGE_CLK1 (2)
#define     NFP_CAP_SPI_CTL1_SPI_MODE_SELECT_SECOND_EDGE_CLK1 (3)
#define   NFP_CAP_SPI_CTL1_SPI_READ_MODE(_x)            (((_x) & 0x3) << 16)
#define   NFP_CAP_SPI_CTL1_SPI_READ_MODE_bf             0, 17, 16
#define   NFP_CAP_SPI_CTL1_SPI_READ_MODE_mask           (0x3)
#define   NFP_CAP_SPI_CTL1_SPI_READ_MODE_shift          (16)
#define     NFP_CAP_SPI_CTL1_SPI_READ_MODE_SLOW         (0)
#define     NFP_CAP_SPI_CTL1_SPI_READ_MODE_FAST_SINGLE  (1)
#define     NFP_CAP_SPI_CTL1_SPI_READ_MODE_FAST_DUAL    (3)
#define   NFP_CAP_SPI_CTL1_SPI_HOLD                     (0x1 << 15)
#define   NFP_CAP_SPI_CTL1_SPI_HOLD_bf                  0, 15, 15
#define   NFP_CAP_SPI_CTL1_SPI_HOLD_mask                (0x1)
#define   NFP_CAP_SPI_CTL1_SPI_HOLD_bit                 (15)
#define     NFP_CAP_SPI_CTL1_SPI_HOLD_IGNORE            (0x0)
#define     NFP_CAP_SPI_CTL1_SPI_HOLD_ALLOW             (0x8000)
#define   NFP_CAP_SPI_CTL1_SPI_HOLD_TIMEOUT             (0x1 << 14)
#define   NFP_CAP_SPI_CTL1_SPI_HOLD_TIMEOUT_bf          0, 14, 14
#define   NFP_CAP_SPI_CTL1_SPI_HOLD_TIMEOUT_mask        (0x1)
#define   NFP_CAP_SPI_CTL1_SPI_HOLD_TIMEOUT_bit         (14)
#define     NFP_CAP_SPI_CTL1_SPI_HOLD_TIMEOUT_USE       (0x0)
#define     NFP_CAP_SPI_CTL1_SPI_HOLD_TIMEOUT_IGNORE    (0x4000)
#define   NFP_CAP_SPI_CTL1_SPI_DUMMY_LEN(_x)            (((_x) & 0x3f) << 8)
#define   NFP_CAP_SPI_CTL1_SPI_DUMMY_LEN_bf             0, 13, 8
#define   NFP_CAP_SPI_CTL1_SPI_DUMMY_LEN_mask           (0x3f)
#define   NFP_CAP_SPI_CTL1_SPI_DUMMY_LEN_shift          (8)
#define   NFP_CAP_SPI_CTL1_SPI_PCLK_DIV(_x)             ((_x) & 0xff)
#define   NFP_CAP_SPI_CTL1_SPI_PCLK_DIV_bf              0, 7, 0
#define   NFP_CAP_SPI_CTL1_SPI_PCLK_DIV_mask            (0xff)
#define   NFP_CAP_SPI_CTL1_SPI_PCLK_DIV_shift           (0)

#endif /* __NFP_SDK__NFP3200_NFP_SPI_H__ */
