/*
 * Copyright (C) 2009 - 2012  Netronome Systems, Inc.  All rights reserved.
 *
 * @file        nfp3200/nfp_pcie.h
 * @brief       Register definitions for PCIe block
 *
 * Disclaimer: this file is provided without any warranty
 * as part of an early-access program, and the content is
 * bound to change before the final release.
 */

#ifndef __NFP_SDK__NFP3200_NFP_PCIE_H__
#define __NFP_SDK__NFP3200_NFP_PCIE_H__


#define PCIE_CFGSPACE_DEVICE_ID             0x000
#define   PCIE_CFGSPACE_DEVICE_ID_v(_x)     (((_x) & 0xffff) << 16)
#define   PCIE_CFGSPACE_DEVICE_ID_bf        0, 31, 16
#define   PCIE_CFGSPACE_DEVICE_ID_mask      (0xffff)
#define   PCIE_CFGSPACE_DEVICE_ID_shift     (16)

#define PCIE_CFGSPACE_VENDOR_ID             0x000
#define   PCIE_CFGSPACE_VENDOR_ID_v(_x)     ((_x) & 0xffff)
#define   PCIE_CFGSPACE_VENDOR_ID_bf        0, 15, 0
#define   PCIE_CFGSPACE_VENDOR_ID_mask      (0xffff)
#define   PCIE_CFGSPACE_VENDOR_ID_shift     (0)

#define PCIE_CFGSPACE_STATUS                0x004
#define   PCIE_CFGSPACE_STATUS_v(_x)        (((_x) & 0xffff) << 16)
#define   PCIE_CFGSPACE_STATUS_bf           0, 31, 16
#define   PCIE_CFGSPACE_STATUS_mask         (0xffff)
#define   PCIE_CFGSPACE_STATUS_shift        (16)

#define PCIE_CFGSPACE_COMMAND               0x004
#define   PCIE_CFGSPACE__v(_x)              ((_x) & 0xffff)
#define   PCIE_CFGSPACE__bf                 0, 15, 0
#define   PCIE_CFGSPACE__mask               (0xffff)
#define   PCIE_CFGSPACE__shift              (0)

#define PCIE_CFGSPACE_CLASS                 0x008
#define   PCIE_CFGSPACE_CLASS_v(_x)         (((_x) & 0xffffff) << 8)
#define   PCIE_CFGSPACE_CLASS_bf            0, 31, 8
#define   PCIE_CFGSPACE_CLASS_mask          (0xffffff)
#define   PCIE_CFGSPACE_CLASS_shift         (8)

#define PCIE_CFGSPACE_REV_ID                0x008
#define   PCIE_CFGSPACE_REV_ID_v(_x)        ((_x) & 0xff)
#define   PCIE_CFGSPACE_REV_ID_bf           0, 7, 0
#define   PCIE_CFGSPACE_REV_ID_mask         (0xff)
#define   PCIE_CFGSPACE_REV_ID_shift        (0)

#define PCIE_CFGSPACE_BIST                  0x00c
#define   PCIE_CFGSPACE_BIST_v(_x)          (((_x) & 0xff) << 24)
#define   PCIE_CFGSPACE_BIST_bf             0, 31, 24
#define   PCIE_CFGSPACE_BIST_mask           (0xff)
#define   PCIE_CFGSPACE_BIST_shift          (24)

#define PCIE_CFGSPACE_HDRTYPE               0x00c
#define   PCIE_CFGSPACE_HDRTYPE_v(_x)       (((_x) & 0xff) << 16)
#define   PCIE_CFGSPACE_HDRTYPE_bf          0, 23, 16
#define   PCIE_CFGSPACE_HDRTYPE_mask        (0xff)
#define   PCIE_CFGSPACE_HDRTYPE_shift       (16)

#define PCIE_CFGSPACE_LATTIM                0x00c
#define   PCIE_CFGSPACE_LATTIM_v(_x)        (((_x) & 0xff) << 8)
#define   PCIE_CFGSPACE_LATTIM_bf           0, 15, 8
#define   PCIE_CFGSPACE_LATTIM_mask         (0xff)
#define   PCIE_CFGSPACE_LATTIM_shift        (8)

#define PCIE_CFGSPACE_CACHELNSZ             0x00c
#define   PCIE_CFGSPACE_CACHELNSZ_v(_x)     ((_x) & 0xff)
#define   PCIE_CFGSPACE_CACHELNSZ_bf        0, 7, 0
#define   PCIE_CFGSPACE_CACHELNSZ_mask      (0xff)
#define   PCIE_CFGSPACE_CACHELNSZ_shift     (0)

#define PCIE_CFGSPACE_BAR0                  0x010
#define   PCIE_CFGSPACE_BAR0_bf             0, 31, 0
#define   PCIE_CFGSPACE_BAR0_mask           (0xffffffff)
#define   PCIE_CFGSPACE_BAR0_shift          (0)
#define PCIE_CFGSPACE_BAR1                  0x014
#define   PCIE_CFGSPACE_BAR1_bf             0, 31, 0
#define   PCIE_CFGSPACE_BAR1_mask           (0xffffffff)
#define   PCIE_CFGSPACE_BAR1_shift          (0)
#define PCIE_CFGSPACE_BAR2                  0x018
#define   PCIE_CFGSPACE_BAR2_bf             0, 31, 0
#define   PCIE_CFGSPACE_BAR2_mask           (0xffffffff)
#define   PCIE_CFGSPACE_BAR2_shift          (0)
#define PCIE_CFGSPACE_BAR3                  0x01c
#define   PCIE_CFGSPACE_BAR3_bf             0, 31, 0
#define   PCIE_CFGSPACE_BAR3_mask           (0xffffffff)
#define   PCIE_CFGSPACE_BAR3_shift          (0)
#define PCIE_CFGSPACE_BAR4                  0x020
#define   PCIE_CFGSPACE_BAR4_bf             0, 31, 0
#define   PCIE_CFGSPACE_BAR4_mask           (0xffffffff)
#define   PCIE_CFGSPACE_BAR4_shift          (0)
#define PCIE_CFGSPACE_BAR5                  0x024
#define   PCIE_CFGSPACE_BAR5_bf             0, 31, 0
#define   PCIE_CFGSPACE_BAR5_mask           (0xffffffff)
#define   PCIE_CFGSPACE_BAR5_shift          (0)

#define PCIE_CFGSPACE_CARDBUS_CIS           0x028
#define   PCIE_CFGSPACE_CARDBUS_CIS_bf      0, 31, 0
#define   PCIE_CFGSPACE_CARDBUS_CIS_mask    (0xffffffff)
#define   PCIE_CFGSPACE_CARDBUS_CIS_shift   (0)

#define PCIE_CFGSPACE_SUBSYS_ID             0x02c
#define   PCIE_CFGSPACE_SUBSYS_ID_v(_x)     (((_x) & 0xffff) << 16)
#define   PCIE_CFGSPACE_SUBSYS_ID_bf        0, 31, 16
#define   PCIE_CFGSPACE_SUBSYS_ID_mask      (0xffff)
#define   PCIE_CFGSPACE_SUBSYS_ID_shift     (16)

#define PCIE_CFGSPACE_SUBSYS_VID            0x02c
#define   PCIE_CFGSPACE__v(_x)              ((_x) & 0xffff)
#define   PCIE_CFGSPACE__bf                 0, 15, 0
#define   PCIE_CFGSPACE__mask               (0xffff)
#define   PCIE_CFGSPACE__shift              (0)

#define PCIE_CFGSPACE_ROMBASE               0x030
#define   PCIE_CFGSPACE_ROMBASE_bf          0, 31, 0
#define   PCIE_CFGSPACE_ROMBASE_mask        (0xffffffff)
#define   PCIE_CFGSPACE_ROMBASE_shift       (0)

#define PCIE_CFGSPACE_CAPPTR                0x034
#define   PCIE_CFGSPACE_CAPPTR_v(_x)        ((_x) & 0xff)
#define   PCIE_CFGSPACE_CAPPTR_bf           0, 7, 0
#define   PCIE_CFGSPACE_CAPPTR_mask         (0xff)
#define   PCIE_CFGSPACE_CAPPTR_shift        (0)

#define PCIE_CFGSPACE_MAXLAT                0x03c
#define   PCIE_CFGSPACE_MAXLAT_v(_x)        (((_x) & 0xff) << 24)
#define   PCIE_CFGSPACE_MAXLAT_bf           0, 31, 24
#define   PCIE_CFGSPACE_MAXLAT_mask         (0xff)
#define   PCIE_CFGSPACE_MAXLAT_shift        (24)

#define PCIE_CFGSPACE_MINGNT                0x03c
#define   PCIE_CFGSPACE_MINGNT_v(_x)        (((_x) & 0xff) << 16)
#define   PCIE_CFGSPACE_MINGNT_bf           0, 23, 16
#define   PCIE_CFGSPACE_MINGNT_mask         (0xff)
#define   PCIE_CFGSPACE_MINGNT_shift        (16)

#define PCIE_CFGSPACE_INTPIN                0x03c
#define   PCIE_CFGSPACE_INTPIN_v(_x)        (((_x) & 0xff) << 8)
#define   PCIE_CFGSPACE_INTPIN_bf           0, 15, 8
#define   PCIE_CFGSPACE_INTPIN_mask         (0xff)
#define   PCIE_CFGSPACE_INTPIN_shift        (8)

#define PCIE_CFGSPACE_INTLINE               0x03c
#define   PCIE_CFGSPACE_INTLINE_v(_x)       ((_x) & 0xff)
#define   PCIE_CFGSPACE_INTLINE_bf          0, 7, 0
#define   PCIE_CFGSPACE_INTLINE_mask        (0xff)
#define   PCIE_CFGSPACE_INTLINE_shift       (0)


#define NFP_PCIE_DMA_TPCI_HI    0
#define NFP_PCIE_DMA_TPCI_LO    1
#define NFP_PCIE_DMA_FPCI_HI    2
#define NFP_PCIE_DMA_FPCI_LO    3


/* HGID: nfp3200/pcie.desc = 9c49f40378dd */
#define NFP_PCIE_SRAM_BASE             0x0000
#define NFP_PCIE_SRAM_LEN              0x10000
#define NFP_PCIE_IM                    0x10000
#define NFP_PCIE_EM                    0x20000
/* Register Type: PcieToCppBar */
#define NFP_PCIE_BARCFG_P2C(_bar)      (0x30000 + (0x4 * (_bar)))
#define   NFP_PCIE_BARCFG_P2C_MAPTYPE(_x)               (((_x) & 0x3) << 30)
#define   NFP_PCIE_BARCFG_P2C_MAPTYPE_bf                0, 31, 30
#define   NFP_PCIE_BARCFG_P2C_MAPTYPE_mask              (0x3)
#define   NFP_PCIE_BARCFG_P2C_MAPTYPE_shift             (30)
#define     NFP_PCIE_BARCFG_P2C_MAPTYPE_BULK            (0)
#define     NFP_PCIE_BARCFG_P2C_MAPTYPE_CPP             (1)
#define     NFP_PCIE_BARCFG_P2C_MAPTYPE_MIXED           (2)
#define   NFP_PCIE_BARCFG_P2C_TGTACT(_x)                (((_x) & 0x1f) << 25)
#define   NFP_PCIE_BARCFG_P2C_TGTACT_bf                 0, 29, 25
#define   NFP_PCIE_BARCFG_P2C_TGTACT_mask               (0x1f)
#define   NFP_PCIE_BARCFG_P2C_TGTACT_shift              (25)
#define   NFP_PCIE_BARCFG_P2C_TOKACTSEL(_x)             (((_x) & 0x3) << 23)
#define   NFP_PCIE_BARCFG_P2C_TOKACTSEL_bf              0, 24, 23
#define   NFP_PCIE_BARCFG_P2C_TOKACTSEL_mask            (0x3)
#define   NFP_PCIE_BARCFG_P2C_TOKACTSEL_shift           (23)
#define     NFP_PCIE_BARCFG_P2C_TOKACTSEL_NONE          (0)
#define     NFP_PCIE_BARCFG_P2C_TOKACTSEL_ADDR          (1)
#define     NFP_PCIE_BARCFG_P2C_TOKACTSEL_BARCFG        (2)
#define   NFP_PCIE_BARCFG_P2C_LEN                       (0x1 << 22)
#define   NFP_PCIE_BARCFG_P2C_LEN_bf                    0, 22, 22
#define   NFP_PCIE_BARCFG_P2C_LEN_mask                  (0x1)
#define   NFP_PCIE_BARCFG_P2C_LEN_bit                   (22)
#define     NFP_PCIE_BARCFG_P2C_LEN_32BIT               (0x0)
#define     NFP_PCIE_BARCFG_P2C_LEN_64BIT               (0x400000)
#define   NFP_PCIE_BARCFG_P2C_BASE(_x)                  ((_x) & 0x3fffff)
#define   NFP_PCIE_BARCFG_P2C_BASE_bf                   0, 21, 0
#define   NFP_PCIE_BARCFG_P2C_BASE_mask                 (0x3fffff)
#define   NFP_PCIE_BARCFG_P2C_BASE_shift                (0)
/* Register Type: CppToPcieBar */
#define NFP_PCIE_BARCFG_C2P(_bar)      (0x30020 + (0x4 * ((_bar) & 0x7)))
#define   NFP_PCIE_BARCFG_C2P_TYPE(_x)                  (((_x) & 0x3) << 30)
#define   NFP_PCIE_BARCFG_C2P_TYPE_bf                   0, 31, 30
#define   NFP_PCIE_BARCFG_C2P_TYPE_mask                 (0x3)
#define   NFP_PCIE_BARCFG_C2P_TYPE_shift                (30)
#define     NFP_PCIE_BARCFG_C2P_TYPE_MEM                (0)
#define     NFP_PCIE_BARCFG_C2P_TYPE_IO                 (1)
#define     NFP_PCIE_BARCFG_C2P_TYPE_CFG                (2)
#define     NFP_PCIE_BARCFG_C2P_TYPE_VIRT               (3)
#define   NFP_PCIE_BARCFG_C2P_ARI_ENABLE                (0x1 << 28)
#define   NFP_PCIE_BARCFG_C2P_ARI_ENABLE_bf             0, 28, 28
#define   NFP_PCIE_BARCFG_C2P_ARI_ENABLE_mask           (0x1)
#define   NFP_PCIE_BARCFG_C2P_ARI_ENABLE_bit            (28)
#define   NFP_PCIE_BARCFG_C2P_ARI(_x)                   (((_x) & 0xff) << 20)
#define   NFP_PCIE_BARCFG_C2P_ARI_bf                    0, 27, 20
#define   NFP_PCIE_BARCFG_C2P_ARI_mask                  (0xff)
#define   NFP_PCIE_BARCFG_C2P_ARI_shift                 (20)
#define   NFP_PCIE_BARCFG_C2P_ADDR(_x)                  ((_x) & 0x7ffff)
#define   NFP_PCIE_BARCFG_C2P_ADDR_bf                   0, 18, 0
#define   NFP_PCIE_BARCFG_C2P_ADDR_mask                 (0x7ffff)
#define   NFP_PCIE_BARCFG_C2P_ADDR_shift                (0)
/* Register Type: DMAWritePtr */
#define NFP_PCIE_DMA_WRITEPTR_TPCI_HI  0x40000
#define NFP_PCIE_DMA_WRITEPTR_TPCI_LO  0x40010
#define NFP_PCIE_DMA_WRITEPTR_FPCI_HI  0x40020
#define NFP_PCIE_DMA_WRITEPTR_FPCI_LO  0x40030
#define   NFP_PCIE_DMA_WRITEPTR_PTR_bf                  0, 31, 0
#define   NFP_PCIE_DMA_WRITEPTR_PTR_mask                (0xffffffff)
#define   NFP_PCIE_DMA_WRITEPTR_PTR_shift               (0)
/* Register Type: DMACommand */
#define NFP_PCIE_DMA_CMD_TPCI_HI       0x40000
#define NFP_PCIE_DMA_CMD_TPCI_LO       0x40010
#define NFP_PCIE_DMA_CMD_FPCI_HI       0x40020
#define NFP_PCIE_DMA_CMD_FPCI_LO       0x40030
/* Word 3 */
#define   NFP_PCIE_DMA_CMD_LENGTH(_x)                   (((_x) & 0xfff) << 20)
#define   NFP_PCIE_DMA_CMD_LENGTH_bf                    3, 31, 20
#define   NFP_PCIE_DMA_CMD_LENGTH_mask                  (0xfff)
#define   NFP_PCIE_DMA_CMD_LENGTH_shift                 (20)
#define   NFP_PCIE_DMA_CMD_RID(_x)                      (((_x) & 0xff) << 12)
#define   NFP_PCIE_DMA_CMD_RID_bf                       3, 19, 12
#define   NFP_PCIE_DMA_CMD_RID_mask                     (0xff)
#define   NFP_PCIE_DMA_CMD_RID_shift                    (12)
#define   NFP_PCIE_DMA_CMD_RID_OVERRIDE                 (0x1 << 11)
#define   NFP_PCIE_DMA_CMD_RID_OVERRIDE_bf              3, 11, 11
#define   NFP_PCIE_DMA_CMD_RID_OVERRIDE_mask            (0x1)
#define   NFP_PCIE_DMA_CMD_RID_OVERRIDE_bit             (11)
#define     NFP_PCIE_DMA_CMD_RID_OVERRIDE_ENABLE        (0x800)
#define   NFP_PCIE_DMA_CMD_NO_SNOOP                     (0x1 << 10)
#define   NFP_PCIE_DMA_CMD_NO_SNOOP_bf                  3, 10, 10
#define   NFP_PCIE_DMA_CMD_NO_SNOOP_mask                (0x1)
#define   NFP_PCIE_DMA_CMD_NO_SNOOP_bit                 (10)
#define     NFP_PCIE_DMA_CMD_NO_SNOOP_ENABLE            (0x400)
#define   NFP_PCIE_DMA_CMD_RELAXED_ORDER                (0x1 << 9)
#define   NFP_PCIE_DMA_CMD_RELAXED_ORDER_bf             3, 9, 9
#define   NFP_PCIE_DMA_CMD_RELAXED_ORDER_mask           (0x1)
#define   NFP_PCIE_DMA_CMD_RELAXED_ORDER_bit            (9)
#define     NFP_PCIE_DMA_CMD_RELAXED_ORDER_ENABLE       (0x200)
#define   NFP_PCIE_DMA_CMD_PCIE_ADDR_HI(_x)             ((_x) & 0xff)
#define   NFP_PCIE_DMA_CMD_PCIE_ADDR_HI_bf              3, 7, 0
#define   NFP_PCIE_DMA_CMD_PCIE_ADDR_HI_mask            (0xff)
#define   NFP_PCIE_DMA_CMD_PCIE_ADDR_HI_shift           (0)
/* Word 2 */
#define   NFP_PCIE_DMA_CMD_PCIE_ADDR_LO(_x)             (_x)
#define   NFP_PCIE_DMA_CMD_PCIE_ADDR_LO_bf              2, 31, 0
#define   NFP_PCIE_DMA_CMD_PCIE_ADDR_LO_mask            (0xffffffff)
#define   NFP_PCIE_DMA_CMD_PCIE_ADDR_LO_shift           (0)
/* Word 1 */
#define   NFP_PCIE_DMA_CMD_CPL(_x)                      (((_x) & 0xffff) << 16)
#define   NFP_PCIE_DMA_CMD_CPL_bf                       1, 31, 16
#define   NFP_PCIE_DMA_CMD_CPL_mask                     (0xffff)
#define   NFP_PCIE_DMA_CMD_CPL_shift                    (16)
#define   NFP_PCIE_DMA_CMD_TOKEN(_x)                    (((_x) & 0x3) << 14)
#define   NFP_PCIE_DMA_CMD_TOKEN_bf                     1, 15, 14
#define   NFP_PCIE_DMA_CMD_TOKEN_mask                   (0x3)
#define   NFP_PCIE_DMA_CMD_TOKEN_shift                  (14)
#define   NFP_PCIE_DMA_CMD_WORD_ONLY                    (0x1 << 13)
#define   NFP_PCIE_DMA_CMD_WORD_ONLY_bf                 1, 13, 13
#define   NFP_PCIE_DMA_CMD_WORD_ONLY_mask               (0x1)
#define   NFP_PCIE_DMA_CMD_WORD_ONLY_bit                (13)
#define     NFP_PCIE_DMA_CMD_WORD_ONLY_ENABLE           (0x2000)
#define   NFP_PCIE_DMA_CMD_TARGET64                     (0x1 << 12)
#define   NFP_PCIE_DMA_CMD_TARGET64_bf                  1, 12, 12
#define   NFP_PCIE_DMA_CMD_TARGET64_mask                (0x1)
#define   NFP_PCIE_DMA_CMD_TARGET64_bit                 (12)
#define     NFP_PCIE_DMA_CMD_TARGET64_ENABLE            (0x1000)
#define   NFP_PCIE_DMA_CMD_CPP_TARGET(_x)               (((_x) & 0xf) << 8)
#define   NFP_PCIE_DMA_CMD_CPP_TARGET_bf                1, 11, 8
#define   NFP_PCIE_DMA_CMD_CPP_TARGET_mask              (0xf)
#define   NFP_PCIE_DMA_CMD_CPP_TARGET_shift             (8)
#define   NFP_PCIE_DMA_CMD_CPP_ADDR_HI(_x)              ((_x) & 0xff)
#define   NFP_PCIE_DMA_CMD_CPP_ADDR_HI_bf               1, 7, 0
#define   NFP_PCIE_DMA_CMD_CPP_ADDR_HI_mask             (0xff)
#define   NFP_PCIE_DMA_CMD_CPP_ADDR_HI_shift            (0)
/* Word 0 */
#define   NFP_PCIE_DMA_CMD_CPP_ADDR_LO(_x)              (_x)
#define   NFP_PCIE_DMA_CMD_CPP_ADDR_LO_bf               0, 31, 0
#define   NFP_PCIE_DMA_CMD_CPP_ADDR_LO_mask             (0xffffffff)
#define   NFP_PCIE_DMA_CMD_CPP_ADDR_LO_shift            (0)
/* Register Type: DMAControl */
#define NFP_PCIE_DMA_CTRL_TPCI_HI      0x40040
#define NFP_PCIE_DMA_CTRL_TPCI_LO      0x40044
#define NFP_PCIE_DMA_CTRL_FPCI_HI      0x40048
#define NFP_PCIE_DMA_CTRL_FPCI_LO      0x4004c
#define   NFP_PCIE_DMA_CTRL_DESC_ERROR                  (0x1 << 5)
#define   NFP_PCIE_DMA_CTRL_DESC_ERROR_bf               0, 5, 5
#define   NFP_PCIE_DMA_CTRL_DESC_ERROR_mask             (0x1)
#define   NFP_PCIE_DMA_CTRL_DESC_ERROR_bit              (5)
#define     NFP_PCIE_DMA_CTRL_DESC_ERROR_CLEAR          (0x20)
#define   NFP_PCIE_DMA_CTRL_PCIE_ERROR                  (0x1 << 4)
#define   NFP_PCIE_DMA_CTRL_PCIE_ERROR_bf               0, 4, 4
#define   NFP_PCIE_DMA_CTRL_PCIE_ERROR_mask             (0x1)
#define   NFP_PCIE_DMA_CTRL_PCIE_ERROR_bit              (4)
#define     NFP_PCIE_DMA_CTRL_PCIE_ERROR_CLEAR          (0x10)
#define   NFP_PCIE_DMA_CTRL_CPP_ERROR                   (0x1 << 3)
#define   NFP_PCIE_DMA_CTRL_CPP_ERROR_bf                0, 3, 3
#define   NFP_PCIE_DMA_CTRL_CPP_ERROR_mask              (0x1)
#define   NFP_PCIE_DMA_CTRL_CPP_ERROR_bit               (3)
#define     NFP_PCIE_DMA_CTRL_CPP_ERROR_CLEAR           (0x8)
#define   NFP_PCIE_DMA_CTRL_QUEUE_OVERFLOW              (0x1 << 2)
#define   NFP_PCIE_DMA_CTRL_QUEUE_OVERFLOW_bf           0, 2, 2
#define   NFP_PCIE_DMA_CTRL_QUEUE_OVERFLOW_mask         (0x1)
#define   NFP_PCIE_DMA_CTRL_QUEUE_OVERFLOW_bit          (2)
#define     NFP_PCIE_DMA_CTRL_QUEUE_OVERFLOW_CLEAR      (0x4)
#define   NFP_PCIE_DMA_CTRL_CONT_ON_ERROR               (0x1 << 1)
#define   NFP_PCIE_DMA_CTRL_CONT_ON_ERROR_bf            0, 1, 1
#define   NFP_PCIE_DMA_CTRL_CONT_ON_ERROR_mask          (0x1)
#define   NFP_PCIE_DMA_CTRL_CONT_ON_ERROR_bit           (1)
#define     NFP_PCIE_DMA_CTRL_CONT_ON_ERROR_ENABLE      (0x2)
#define   NFP_PCIE_DMA_CTRL_QUEUE_STOP                  (0x1)
#define   NFP_PCIE_DMA_CTRL_QUEUE_STOP_bf               0, 0, 0
#define   NFP_PCIE_DMA_CTRL_QUEUE_STOP_mask             (0x1)
#define   NFP_PCIE_DMA_CTRL_QUEUE_STOP_bit              (0)
#define     NFP_PCIE_DMA_CTRL_QUEUE_STOP_ENABLE         (0x1)
/* Register Type: DMAQueueStatus */
#define NFP_PCIE_DMA_QSTS_TPCI_HI      0x40040
#define NFP_PCIE_DMA_QSTS_TPCI_LO      0x40044
#define NFP_PCIE_DMA_QSTS_FPCI_HI      0x40048
#define NFP_PCIE_DMA_QSTS_FPCI_LO      0x4004c
#define   NFP_PCIE_DMA_QSTS_AVAIL_bf                    0, 31, 24
#define   NFP_PCIE_DMA_QSTS_AVAIL_mask                  (0xff)
#define   NFP_PCIE_DMA_QSTS_AVAIL_shift                 (24)
#define   NFP_PCIE_DMA_QSTS_READ_PTR_bf                 0, 23, 16
#define   NFP_PCIE_DMA_QSTS_READ_PTR_mask               (0xff)
#define   NFP_PCIE_DMA_QSTS_READ_PTR_shift              (16)
#define   NFP_PCIE_DMA_QSTS_POP_PTR_bf                  0, 15, 8
#define   NFP_PCIE_DMA_QSTS_POP_PTR_mask                (0xff)
#define   NFP_PCIE_DMA_QSTS_POP_PTR_shift               (8)
#define   NFP_PCIE_DMA_QSTS_QUEUE_FULL                  (0x1 << 7)
#define   NFP_PCIE_DMA_QSTS_QUEUE_FULL_bf               0, 7, 7
#define   NFP_PCIE_DMA_QSTS_QUEUE_FULL_mask             (0x1)
#define   NFP_PCIE_DMA_QSTS_QUEUE_FULL_bit              (7)
#define   NFP_PCIE_DMA_QSTS_QUEUE_ACTIVE                (0x1 << 6)
#define   NFP_PCIE_DMA_QSTS_QUEUE_ACTIVE_bf             0, 6, 6
#define   NFP_PCIE_DMA_QSTS_QUEUE_ACTIVE_mask           (0x1)
#define   NFP_PCIE_DMA_QSTS_QUEUE_ACTIVE_bit            (6)
#define   NFP_PCIE_DMA_QSTS_DESC_ERROR                  (0x1 << 5)
#define   NFP_PCIE_DMA_QSTS_DESC_ERROR_bf               0, 5, 5
#define   NFP_PCIE_DMA_QSTS_DESC_ERROR_mask             (0x1)
#define   NFP_PCIE_DMA_QSTS_DESC_ERROR_bit              (5)
#define   NFP_PCIE_DMA_QSTS_PCIE_ERROR                  (0x1 << 4)
#define   NFP_PCIE_DMA_QSTS_PCIE_ERROR_bf               0, 4, 4
#define   NFP_PCIE_DMA_QSTS_PCIE_ERROR_mask             (0x1)
#define   NFP_PCIE_DMA_QSTS_PCIE_ERROR_bit              (4)
#define   NFP_PCIE_DMA_QSTS_CPP_ERROR                   (0x1 << 3)
#define   NFP_PCIE_DMA_QSTS_CPP_ERROR_bf                0, 3, 3
#define   NFP_PCIE_DMA_QSTS_CPP_ERROR_mask              (0x1)
#define   NFP_PCIE_DMA_QSTS_CPP_ERROR_bit               (3)
#define   NFP_PCIE_DMA_QSTS_QUEUE_OVERFLOW              (0x1 << 2)
#define   NFP_PCIE_DMA_QSTS_QUEUE_OVERFLOW_bf           0, 2, 2
#define   NFP_PCIE_DMA_QSTS_QUEUE_OVERFLOW_mask         (0x1)
#define   NFP_PCIE_DMA_QSTS_QUEUE_OVERFLOW_bit          (2)
#define   NFP_PCIE_DMA_QSTS_CONT_ON_ERROR               (0x1 << 1)
#define   NFP_PCIE_DMA_QSTS_CONT_ON_ERROR_bf            0, 1, 1
#define   NFP_PCIE_DMA_QSTS_CONT_ON_ERROR_mask          (0x1)
#define   NFP_PCIE_DMA_QSTS_CONT_ON_ERROR_bit           (1)
#define   NFP_PCIE_DMA_QSTS_QUEUE_STOP                  (0x1)
#define   NFP_PCIE_DMA_QSTS_QUEUE_STOP_bf               0, 0, 0
#define   NFP_PCIE_DMA_QSTS_QUEUE_STOP_mask             (0x1)
#define   NFP_PCIE_DMA_QSTS_QUEUE_STOP_bit              (0)
#define NFP_PCIE_QUEUE(_q)             (0x80000 + (0x800 * ((_q) & 0xff)))

/* HGID: nfp3200/pcie_csr.desc = 3371527b1976 */
/* Register Type: PCIECntrlrConfig0 */
#define NFP_PCIE_CSR_CFG0              0x0000
#define   NFP_PCIE_CSR_CFG0_XXX_A                       (0x1 << 31)
#define   NFP_PCIE_CSR_CFG0_XXX_A_bf                    0, 31, 31
#define   NFP_PCIE_CSR_CFG0_XXX_A_mask                  (0x1)
#define   NFP_PCIE_CSR_CFG0_XXX_A_bit                   (31)
#define     NFP_PCIE_CSR_CFG0_XXX_A_XXX_SHORT_0A        (0x0)
#define     NFP_PCIE_CSR_CFG0_XXX_A_XXX_SHORT_1A        (0x80000000)
#define   NFP_PCIE_CSR_CFG0_XXX_B                       (0x1 << 30)
#define   NFP_PCIE_CSR_CFG0_XXX_B_bf                    0, 30, 30
#define   NFP_PCIE_CSR_CFG0_XXX_B_mask                  (0x1)
#define   NFP_PCIE_CSR_CFG0_XXX_B_bit                   (30)
#define     NFP_PCIE_CSR_CFG0_XXX_B_XXX_SHORT_0B        (0x0)
#define     NFP_PCIE_CSR_CFG0_XXX_B_XXX_SHORT_1B        (0x40000000)
#define   NFP_PCIE_CSR_CFG0_XXX_C                       (0x1 << 29)
#define   NFP_PCIE_CSR_CFG0_XXX_C_bf                    0, 29, 29
#define   NFP_PCIE_CSR_CFG0_XXX_C_mask                  (0x1)
#define   NFP_PCIE_CSR_CFG0_XXX_C_bit                   (29)
#define     NFP_PCIE_CSR_CFG0_XXX_C_XXX_SHORT_0C        (0x0)
#define     NFP_PCIE_CSR_CFG0_XXX_C_XXX_SHORT_1C        (0x20000000)
#define   NFP_PCIE_CSR_CFG0_MGMT_REG_ENABLE             (0x1 << 27)
#define   NFP_PCIE_CSR_CFG0_MGMT_REG_ENABLE_bf          0, 27, 27
#define   NFP_PCIE_CSR_CFG0_MGMT_REG_ENABLE_mask        (0x1)
#define   NFP_PCIE_CSR_CFG0_MGMT_REG_ENABLE_bit         (27)
#define     NFP_PCIE_CSR_CFG0_MGMT_REG_ENABLE_XXX_SHORT_0D (0x0)
#define     NFP_PCIE_CSR_CFG0_MGMT_REG_ENABLE_XXX_SHORT_1D (0x8000000)
#define   NFP_PCIE_CSR_CFG0_TARGET_DEVICE(_x)           (((_x) & 0x1f) << 16)
#define   NFP_PCIE_CSR_CFG0_TARGET_DEVICE_bf            0, 20, 16
#define   NFP_PCIE_CSR_CFG0_TARGET_DEVICE_mask          (0x1f)
#define   NFP_PCIE_CSR_CFG0_TARGET_DEVICE_shift         (16)
#define   NFP_PCIE_CSR_CFG0_TARGET_BUS(_x)              (((_x) & 0xff) << 8)
#define   NFP_PCIE_CSR_CFG0_TARGET_BUS_bf               0, 15, 8
#define   NFP_PCIE_CSR_CFG0_TARGET_BUS_mask             (0xff)
#define   NFP_PCIE_CSR_CFG0_TARGET_BUS_shift            (8)
#define   NFP_PCIE_CSR_CFG0_XXX_READS_IN_FLIGHT_LIMIT(_x) (((_x) & 0x3) << 2)
#define   NFP_PCIE_CSR_CFG0_XXX_READS_IN_FLIGHT_LIMIT_bf 0, 3, 2
#define   NFP_PCIE_CSR_CFG0_XXX_READS_IN_FLIGHT_LIMIT_mask (0x3)
#define   NFP_PCIE_CSR_CFG0_XXX_READS_IN_FLIGHT_LIMIT_shift (2)
#define     NFP_PCIE_CSR_CFG0_XXX_READS_IN_FLIGHT_LIMIT_32_READS_IN_FLIGHT (0)
#define     NFP_PCIE_CSR_CFG0_XXX_READS_IN_FLIGHT_LIMIT_8_READS_IN_FLIGHT (1)
#define     NFP_PCIE_CSR_CFG0_XXX_READS_IN_FLIGHT_LIMIT_16_READS_IN_FLIGHT (2)
#define     NFP_PCIE_CSR_CFG0_XXX_READS_IN_FLIGHT_LIMIT_XXX_RESERVED_XXX (3)
#define   NFP_PCIE_CSR_CFG0_PCIE_ADDR_MODE              (0x1 << 1)
#define   NFP_PCIE_CSR_CFG0_PCIE_ADDR_MODE_bf           0, 1, 1
#define   NFP_PCIE_CSR_CFG0_PCIE_ADDR_MODE_mask         (0x1)
#define   NFP_PCIE_CSR_CFG0_PCIE_ADDR_MODE_bit          (1)
#define     NFP_PCIE_CSR_CFG0_PCIE_ADDR_MODE_32         (0x0)
#define     NFP_PCIE_CSR_CFG0_PCIE_ADDR_MODE_64         (0x2)
#define   NFP_PCIE_CSR_CFG0_CPP_ADDR_MODE               (0x1)
#define   NFP_PCIE_CSR_CFG0_CPP_ADDR_MODE_bf            0, 0, 0
#define   NFP_PCIE_CSR_CFG0_CPP_ADDR_MODE_mask          (0x1)
#define   NFP_PCIE_CSR_CFG0_CPP_ADDR_MODE_bit           (0)
#define     NFP_PCIE_CSR_CFG0_CPP_ADDR_MODE_40          (0x0)
#define     NFP_PCIE_CSR_CFG0_CPP_ADDR_MODE_32          (0x1)
/* Register Type: PCIESPARE */
#define NFP_PCIE_CSR_SPARE             0x0004
#define   NFP_PCIE_CSR_SPARE_DATA(_x)                   (_x)
#define   NFP_PCIE_CSR_SPARE_DATA_bf                    0, 31, 0
#define   NFP_PCIE_CSR_SPARE_DATA_mask                  (0xffffffff)
#define   NFP_PCIE_CSR_SPARE_DATA_shift                 (0)
/* Register Type: PCIECntrlrConfig1 */
#define NFP_PCIE_CSR_CFG1              0x0008
#define   NFP_PCIE_CSR_CFG1_INT_SEL(_x)                 (((_x) & 0x3) << 10)
#define   NFP_PCIE_CSR_CFG1_INT_SEL_bf                  0, 11, 10
#define   NFP_PCIE_CSR_CFG1_INT_SEL_mask                (0x3)
#define   NFP_PCIE_CSR_CFG1_INT_SEL_shift               (10)
#define     NFP_PCIE_CSR_CFG1_INT_SEL_INTA              (0)
#define     NFP_PCIE_CSR_CFG1_INT_SEL_INTB              (1)
#define     NFP_PCIE_CSR_CFG1_INT_SEL_INTC              (2)
#define     NFP_PCIE_CSR_CFG1_INT_SEL_INTD              (3)
#define   NFP_PCIE_CSR_CFG1_PCIE_LANES(_x)              (((_x) & 0x3) << 8)
#define   NFP_PCIE_CSR_CFG1_PCIE_LANES_bf               0, 9, 8
#define   NFP_PCIE_CSR_CFG1_PCIE_LANES_mask             (0x3)
#define   NFP_PCIE_CSR_CFG1_PCIE_LANES_shift            (8)
#define     NFP_PCIE_CSR_CFG1_PCIE_LANES_X1             (0)
#define     NFP_PCIE_CSR_CFG1_PCIE_LANES_X2             (1)
#define     NFP_PCIE_CSR_CFG1_PCIE_LANES_X4             (2)
#define     NFP_PCIE_CSR_CFG1_PCIE_LANES_X8             (3)
#define   NFP_PCIE_CSR_CFG1_GEN2_ENABLE                 (0x1 << 7)
#define   NFP_PCIE_CSR_CFG1_GEN2_ENABLE_bf              0, 7, 7
#define   NFP_PCIE_CSR_CFG1_GEN2_ENABLE_mask            (0x1)
#define   NFP_PCIE_CSR_CFG1_GEN2_ENABLE_bit             (7)
#define   NFP_PCIE_CSR_CFG1_BAR2_WINDOW                 (0x1 << 2)
#define   NFP_PCIE_CSR_CFG1_BAR2_WINDOW_bf              0, 2, 2
#define   NFP_PCIE_CSR_CFG1_BAR2_WINDOW_mask            (0x1)
#define   NFP_PCIE_CSR_CFG1_BAR2_WINDOW_bit             (2)
#define     NFP_PCIE_CSR_CFG1_BAR2_WINDOW_30            (0x0)
#define     NFP_PCIE_CSR_CFG1_BAR2_WINDOW_24            (0x4)
#define   NFP_PCIE_CSR_CFG1_BAR1_WINDOW                 (0x1 << 1)
#define   NFP_PCIE_CSR_CFG1_BAR1_WINDOW_bf              0, 1, 1
#define   NFP_PCIE_CSR_CFG1_BAR1_WINDOW_mask            (0x1)
#define   NFP_PCIE_CSR_CFG1_BAR1_WINDOW_bit             (1)
#define     NFP_PCIE_CSR_CFG1_BAR1_WINDOW_30            (0x0)
#define     NFP_PCIE_CSR_CFG1_BAR1_WINDOW_24            (0x2)
#define   NFP_PCIE_CSR_CFG1_BAR0_WINDOW                 (0x1)
#define   NFP_PCIE_CSR_CFG1_BAR0_WINDOW_bf              0, 0, 0
#define   NFP_PCIE_CSR_CFG1_BAR0_WINDOW_mask            (0x1)
#define   NFP_PCIE_CSR_CFG1_BAR0_WINDOW_bit             (0)
#define     NFP_PCIE_CSR_CFG1_BAR0_WINDOW_30            (0x0)
#define     NFP_PCIE_CSR_CFG1_BAR0_WINDOW_24            (0x1)
/* Register Type: PCIECntrlrStat */
#define NFP_PCIE_CSR_STATUS            0x000c
#define   NFP_PCIE_CSR_STATUS_INTERRUPT                 (0x1 << 31)
#define   NFP_PCIE_CSR_STATUS_INTERRUPT_bf              0, 31, 31
#define   NFP_PCIE_CSR_STATUS_INTERRUPT_mask            (0x1)
#define   NFP_PCIE_CSR_STATUS_INTERRUPT_bit             (31)
#define   NFP_PCIE_CSR_STATUS_RCIOCFG_WRCPL             (0x1 << 30)
#define   NFP_PCIE_CSR_STATUS_RCIOCFG_WRCPL_bf          0, 30, 30
#define   NFP_PCIE_CSR_STATUS_RCIOCFG_WRCPL_mask        (0x1)
#define   NFP_PCIE_CSR_STATUS_RCIOCFG_WRCPL_bit         (30)
#define   NFP_PCIE_CSR_STATUS_ERROR_CORRECTABLE         (0x1 << 23)
#define   NFP_PCIE_CSR_STATUS_ERROR_CORRECTABLE_bf      0, 23, 23
#define   NFP_PCIE_CSR_STATUS_ERROR_CORRECTABLE_mask    (0x1)
#define   NFP_PCIE_CSR_STATUS_ERROR_CORRECTABLE_bit     (23)
#define   NFP_PCIE_CSR_STATUS_ERROR_NON_FATAL           (0x1 << 22)
#define   NFP_PCIE_CSR_STATUS_ERROR_NON_FATAL_bf        0, 22, 22
#define   NFP_PCIE_CSR_STATUS_ERROR_NON_FATAL_mask      (0x1)
#define   NFP_PCIE_CSR_STATUS_ERROR_NON_FATAL_bit       (22)
#define   NFP_PCIE_CSR_STATUS_ERROR_FATAL               (0x1 << 21)
#define   NFP_PCIE_CSR_STATUS_ERROR_FATAL_bf            0, 21, 21
#define   NFP_PCIE_CSR_STATUS_ERROR_FATAL_mask          (0x1)
#define   NFP_PCIE_CSR_STATUS_ERROR_FATAL_bit           (21)
#define   NFP_PCIE_CSR_STATUS_MODE_IS_RC                (0x1 << 20)
#define   NFP_PCIE_CSR_STATUS_MODE_IS_RC_bf             0, 20, 20
#define   NFP_PCIE_CSR_STATUS_MODE_IS_RC_mask           (0x1)
#define   NFP_PCIE_CSR_STATUS_MODE_IS_RC_bit            (20)
#define     NFP_PCIE_CSR_STATUS_MODE_IS_RC_FIXME        (0x0)
#define     NFP_PCIE_CSR_STATUS_MODE_IS_RC_FIXME1       (0x100000)
#define   NFP_PCIE_CSR_STATUS_LINK_STATE_bf             0, 19, 16
#define   NFP_PCIE_CSR_STATUS_LINK_STATE_mask           (0xf)
#define   NFP_PCIE_CSR_STATUS_LINK_STATE_shift          (16)
#define     NFP_PCIE_CSR_STATUS_LINK_STATE_L0           (0)
#define     NFP_PCIE_CSR_STATUS_LINK_STATE_L0S          (1)
#define     NFP_PCIE_CSR_STATUS_LINK_STATE_L1           (2)
#define     NFP_PCIE_CSR_STATUS_LINK_STATE_L2           (3)
#define   NFP_PCIE_CSR_STATUS_POWER_STATE_bf            0, 15, 13
#define   NFP_PCIE_CSR_STATUS_POWER_STATE_mask          (0x7)
#define   NFP_PCIE_CSR_STATUS_POWER_STATE_shift         (13)
#define     NFP_PCIE_CSR_STATUS_POWER_STATE_DO_UNINITIALIZED (0)
#define     NFP_PCIE_CSR_STATUS_POWER_STATE_D0_ACTIVE   (1)
#define     NFP_PCIE_CSR_STATUS_POWER_STATE_D1          (2)
#define     NFP_PCIE_CSR_STATUS_POWER_STATE_D2          (3)
#define     NFP_PCIE_CSR_STATUS_POWER_STATE_D3_HOT      (4)
#define     NFP_PCIE_CSR_STATUS_POWER_STATE_D3_COLD     (5)
#define   NFP_PCIE_CSR_STATUS_F0_CONFIG_ENABLED         (0x1 << 12)
#define   NFP_PCIE_CSR_STATUS_F0_CONFIG_ENABLED_bf      0, 12, 12
#define   NFP_PCIE_CSR_STATUS_F0_CONFIG_ENABLED_mask    (0x1)
#define   NFP_PCIE_CSR_STATUS_F0_CONFIG_ENABLED_bit     (12)
#define   NFP_PCIE_CSR_STATUS_F0_BUS_MASTER_ENABLED     (0x1 << 11)
#define   NFP_PCIE_CSR_STATUS_F0_BUS_MASTER_ENABLED_bf  0, 11, 11
#define   NFP_PCIE_CSR_STATUS_F0_BUS_MASTER_ENABLED_mask (0x1)
#define   NFP_PCIE_CSR_STATUS_F0_BUS_MASTER_ENABLED_bit (11)
#define   NFP_PCIE_CSR_STATUS_F0_MMIO_ENABLED           (0x1 << 10)
#define   NFP_PCIE_CSR_STATUS_F0_MMIO_ENABLED_bf        0, 10, 10
#define   NFP_PCIE_CSR_STATUS_F0_MMIO_ENABLED_mask      (0x1)
#define   NFP_PCIE_CSR_STATUS_F0_MMIO_ENABLED_bit       (10)
#define   NFP_PCIE_CSR_STATUS_F0_IO_ENABLED             (0x1 << 9)
#define   NFP_PCIE_CSR_STATUS_F0_IO_ENABLED_bf          0, 9, 9
#define   NFP_PCIE_CSR_STATUS_F0_IO_ENABLED_mask        (0x1)
#define   NFP_PCIE_CSR_STATUS_F0_IO_ENABLED_bit         (9)
#define   NFP_PCIE_CSR_STATUS_MAX_PAYLOAD_bf            0, 8, 6
#define   NFP_PCIE_CSR_STATUS_MAX_PAYLOAD_mask          (0x7)
#define   NFP_PCIE_CSR_STATUS_MAX_PAYLOAD_shift         (6)
#define     NFP_PCIE_CSR_STATUS_MAX_PAYLOAD_128         (0)
#define     NFP_PCIE_CSR_STATUS_MAX_PAYLOAD_256         (1)
#define     NFP_PCIE_CSR_STATUS_MAX_PAYLOAD_FIXME2      (2)
#define     NFP_PCIE_CSR_STATUS_MAX_PAYLOAD_FIXME3      (3)
#define     NFP_PCIE_CSR_STATUS_MAX_PAYLOAD_FIXME4      (4)
#define     NFP_PCIE_CSR_STATUS_MAX_PAYLOAD_FIXME5      (5)
#define     NFP_PCIE_CSR_STATUS_MAX_PAYLOAD_FIXME6      (6)
#define     NFP_PCIE_CSR_STATUS_MAX_PAYLOAD_FIXME7      (7)
#define   NFP_PCIE_CSR_STATUS_MAX_READ_REQ_bf           0, 5, 3
#define   NFP_PCIE_CSR_STATUS_MAX_READ_REQ_mask         (0x7)
#define   NFP_PCIE_CSR_STATUS_MAX_READ_REQ_shift        (3)
#define     NFP_PCIE_CSR_STATUS_MAX_READ_REQ_128        (0)
#define     NFP_PCIE_CSR_STATUS_MAX_READ_REQ_256        (1)
#define     NFP_PCIE_CSR_STATUS_MAX_READ_REQ_512        (2)
#define     NFP_PCIE_CSR_STATUS_MAX_READ_REQ_1024       (3)
#define     NFP_PCIE_CSR_STATUS_MAX_READ_REQ_2048       (4)
#define     NFP_PCIE_CSR_STATUS_MAX_READ_REQ_4096       (5)
#define     NFP_PCIE_CSR_STATUS_MAX_READ_REQ_FIXME8     (6)
#define     NFP_PCIE_CSR_STATUS_MAX_READ_REQ_FIXME9     (7)
#define   NFP_PCIE_CSR_STATUS_HIGH_SPEED                (0x1 << 2)
#define   NFP_PCIE_CSR_STATUS_HIGH_SPEED_bf             0, 2, 2
#define   NFP_PCIE_CSR_STATUS_HIGH_SPEED_mask           (0x1)
#define   NFP_PCIE_CSR_STATUS_HIGH_SPEED_bit            (2)
#define   NFP_PCIE_CSR_STATUS_LINK_STATUS_bf            0, 1, 0
#define   NFP_PCIE_CSR_STATUS_LINK_STATUS_mask          (0x3)
#define   NFP_PCIE_CSR_STATUS_LINK_STATUS_shift         (0)
#define     NFP_PCIE_CSR_STATUS_LINK_STATUS_DOWN        (0)
#define     NFP_PCIE_CSR_STATUS_LINK_STATUS_TRAIN       (1)
#define     NFP_PCIE_CSR_STATUS_LINK_STATUS_DL_INIT     (2)
#define     NFP_PCIE_CSR_STATUS_LINK_STATUS_UP          (3)
/* Register Type: PCIEPHYConfig */
#define NFP_PCIE_CSR_PHYCFG0           0x0010
#define NFP_PCIE_CSR_PHYCFG1           0x0014
#define NFP_PCIE_CSR_PHYCFG2           0x0018
#define NFP_PCIE_CSR_PHYCFG3           0x001c
#define NFP_PCIE_CSR_PHYCFG4           0x0020
#define NFP_PCIE_CSR_PHYCFG5           0x0024
#define NFP_PCIE_CSR_PHYCFG6           0x0028
#define NFP_PCIE_CSR_PHYCFG7           0x002c
#define NFP_PCIE_CSR_PHYCFG8           0x0030
#define NFP_PCIE_CSR_PHYCFG9           0x0034
#define NFP_PCIE_CSR_PHYCFG10          0x0038
#define NFP_PCIE_CSR_PHYCFG11          0x003c
#define NFP_PCIE_CSR_PHYCFG12          0x0040
#define NFP_PCIE_CSR_PHYCFG13          0x0044
#define NFP_PCIE_CSR_PHYCFG14          0x0048
#define   NFP_PCIE_CSR_PHYCFG_DATA(_x)                  (_x)
#define   NFP_PCIE_CSR_PHYCFG_DATA_bf                   0, 31, 0
#define   NFP_PCIE_CSR_PHYCFG_DATA_mask                 (0xffffffff)
#define   NFP_PCIE_CSR_PHYCFG_DATA_shift                (0)
/* Register Type: PCIEPHYTest */
#define NFP_PCIE_CSR_PHYTST0           0x004c
#define NFP_PCIE_CSR_PHYTST1           0x0050
#define NFP_PCIE_CSR_PHYTST2           0x0054
#define NFP_PCIE_CSR_PHYTST3           0x0058
#define NFP_PCIE_CSR_PHYTST4           0x005c
#define NFP_PCIE_CSR_PHYTST5           0x0060
#define NFP_PCIE_CSR_PHYTST6           0x0064
#define NFP_PCIE_CSR_PHYTST7           0x0068
#define NFP_PCIE_CSR_PHYTST8           0x006c
#define NFP_PCIE_CSR_PHYTST9           0x0070
#define   NFP_PCIE_CSR_PHYTST_DATA(_x)                  (_x)
#define   NFP_PCIE_CSR_PHYTST_DATA_bf                   0, 31, 0
#define   NFP_PCIE_CSR_PHYTST_DATA_mask                 (0xffffffff)
#define   NFP_PCIE_CSR_PHYTST_DATA_shift                (0)

#endif /* __NFP_SDK__NFP3200_NFP_PCIE_H__ */
