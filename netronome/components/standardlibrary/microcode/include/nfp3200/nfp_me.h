/*
 * Copyright (C) 2009 - 2012  Netronome Systems, Inc.  All rights reserved.
 *
 * @file        nfp3200/nfp_me.h
 * @brief       Register definitions for microengines
 *
 * Disclaimer: this file is provided without any warranty
 * as part of an early-access program, and the content is
 * bound to change before the final release.
 */

#ifndef __NFP_SDK__NFP3200_NFP_ME_H__
#define __NFP_SDK__NFP3200_NFP_ME_H__

/* HGID: nfp3200/me.desc = dcba9b757d97 */
/* Register Type: UstorAddr */
#define NFP_MECSR_USTORE_ADDR          0x0000
#define   NFP_MECSR_USTORE_ADDR_ECS                     (0x1 << 31)
#define   NFP_MECSR_USTORE_ADDR_ECS_bf                  0, 31, 31
#define   NFP_MECSR_USTORE_ADDR_ECS_mask                (0x1)
#define   NFP_MECSR_USTORE_ADDR_ECS_bit                 (31)
#define   NFP_MECSR_USTORE_ADDR_WRITE_STROBE            (0x1 << 30)
#define   NFP_MECSR_USTORE_ADDR_WRITE_STROBE_bf         0, 30, 30
#define   NFP_MECSR_USTORE_ADDR_WRITE_STROBE_mask       (0x1)
#define   NFP_MECSR_USTORE_ADDR_WRITE_STROBE_bit        (30)
#define   NFP_MECSR_USTORE_ADDR_DATA_INVERT             (0x1 << 29)
#define   NFP_MECSR_USTORE_ADDR_DATA_INVERT_bf          0, 29, 29
#define   NFP_MECSR_USTORE_ADDR_DATA_INVERT_mask        (0x1)
#define   NFP_MECSR_USTORE_ADDR_DATA_INVERT_bit         (29)
#define   NFP_MECSR_USTORE_ADDR_ADDR(_x)                ((_x) & 0x1fff)
#define   NFP_MECSR_USTORE_ADDR_ADDR_bf                 0, 12, 0
#define   NFP_MECSR_USTORE_ADDR_ADDR_mask               (0x1fff)
#define   NFP_MECSR_USTORE_ADDR_ADDR_shift              (0)
/* Register Type: UstorDataLwr */
#define NFP_MECSR_USTORE_DATA_LO       0x0004
/* Register Type: UstorDataUpr */
#define NFP_MECSR_USTORE_DATA_HI       0x0008
#define   NFP_MECSR_USTORE_DATA_ECC(_x)                 (((_x) & 0x7f) << 13)
#define   NFP_MECSR_USTORE_DATA_ECC_bf                  0, 19, 13
#define   NFP_MECSR_USTORE_DATA_ECC_mask                (0x7f)
#define   NFP_MECSR_USTORE_DATA_ECC_shift               (13)
#define   NFP_MECSR_USTORE_DATA_HIVAL(_x)               ((_x) & 0x1fff)
#define   NFP_MECSR_USTORE_DATA_HIVAL_bf                0, 12, 0
#define   NFP_MECSR_USTORE_DATA_HIVAL_mask              (0x1fff)
#define   NFP_MECSR_USTORE_DATA_HIVAL_shift             (0)
/* Register Type: UstorErrStat */
#define NFP_MECSR_USTORE_ERR           0x000c
#define   NFP_MECSR_USTORE_ERR_UNCORRECTABLE            (0x1 << 31)
#define   NFP_MECSR_USTORE_ERR_UNCORRECTABLE_bf         0, 31, 31
#define   NFP_MECSR_USTORE_ERR_UNCORRECTABLE_mask       (0x1)
#define   NFP_MECSR_USTORE_ERR_UNCORRECTABLE_bit        (31)
#define   NFP_MECSR_USTORE_ERR_HARD                     (0x1 << 30)
#define   NFP_MECSR_USTORE_ERR_HARD_bf                  0, 30, 30
#define   NFP_MECSR_USTORE_ERR_HARD_mask                (0x1)
#define   NFP_MECSR_USTORE_ERR_HARD_bit                 (30)
#define   NFP_MECSR_USTORE_ERR_SYNDROME_bf              0, 26, 20
#define   NFP_MECSR_USTORE_ERR_SYNDROME_mask            (0x7f)
#define   NFP_MECSR_USTORE_ERR_SYNDROME_shift           (20)
#define   NFP_MECSR_USTORE_ERR_CTX_bf                   0, 18, 16
#define   NFP_MECSR_USTORE_ERR_CTX_mask                 (0x7)
#define   NFP_MECSR_USTORE_ERR_CTX_shift                (16)
#define   NFP_MECSR_USTORE_ERR_UADDR_bf                 0, 13, 0
#define   NFP_MECSR_USTORE_ERR_UADDR_mask               (0x3fff)
#define   NFP_MECSR_USTORE_ERR_UADDR_shift              (0)
/* Register Type: ALUOut */
#define NFP_MECSR_ALU_OUT              0x0010
/* Register Type: CtxArbCtrl */
#define NFP_MECSR_ARB_CTL              0x0014
#define   NFP_MECSR_ARB_CTL_PCTX_bf                     0, 6, 4
#define   NFP_MECSR_ARB_CTL_PCTX_mask                   (0x7)
#define   NFP_MECSR_ARB_CTL_PCTX_shift                  (4)
#define   NFP_MECSR_ARB_CTL_NCTX(_x)                    ((_x) & 0x7)
#define   NFP_MECSR_ARB_CTL_NCTX_bf                     0, 2, 0
#define   NFP_MECSR_ARB_CTL_NCTX_mask                   (0x7)
#define   NFP_MECSR_ARB_CTL_NCTX_shift                  (0)
/* Register Type: CtxEnables */
#define NFP_MECSR_CTX_ENABLES          0x0018
#define   NFP_MECSR_CTX_ENABLES_IN_USE                  (0x1 << 31)
#define   NFP_MECSR_CTX_ENABLES_IN_USE_bf               0, 31, 31
#define   NFP_MECSR_CTX_ENABLES_IN_USE_mask             (0x1)
#define   NFP_MECSR_CTX_ENABLES_IN_USE_bit              (31)
#define     NFP_MECSR_CTX_ENABLES_IN_USE_4CTX           (0x80000000)
#define   NFP_MECSR_CTX_ENABLES_PRN_MODE                (0x1 << 30)
#define   NFP_MECSR_CTX_ENABLES_PRN_MODE_bf             0, 30, 30
#define   NFP_MECSR_CTX_ENABLES_PRN_MODE_mask           (0x1)
#define   NFP_MECSR_CTX_ENABLES_PRN_MODE_bit            (30)
#define     NFP_MECSR_CTX_ENABLES_PRN_MODE_ME_INSTR     (0x0)
#define     NFP_MECSR_CTX_ENABLES_PRN_MODE_EVERY_CYCLE  (0x40000000)
#define   NFP_MECSR_CTX_ENABLES_USTORE_ECC_ERR          (0x1 << 29)
#define   NFP_MECSR_CTX_ENABLES_USTORE_ECC_ERR_bf       0, 29, 29
#define   NFP_MECSR_CTX_ENABLES_USTORE_ECC_ERR_mask     (0x1)
#define   NFP_MECSR_CTX_ENABLES_USTORE_ECC_ERR_bit      (29)
#define   NFP_MECSR_CTX_ENABLES_USTORE_ECC_ENABLE       (0x1 << 28)
#define   NFP_MECSR_CTX_ENABLES_USTORE_ECC_ENABLE_bf    0, 28, 28
#define   NFP_MECSR_CTX_ENABLES_USTORE_ECC_ENABLE_mask  (0x1)
#define   NFP_MECSR_CTX_ENABLES_USTORE_ECC_ENABLE_bit   (28)
#define   NFP_MECSR_CTX_ENABLES_BREAKPOINT              (0x1 << 27)
#define   NFP_MECSR_CTX_ENABLES_BREAKPOINT_bf           0, 27, 27
#define   NFP_MECSR_CTX_ENABLES_BREAKPOINT_mask         (0x1)
#define   NFP_MECSR_CTX_ENABLES_BREAKPOINT_bit          (27)
#define   NFP_MECSR_CTX_ENABLES_REG_PAR_ERR             (0x1 << 25)
#define   NFP_MECSR_CTX_ENABLES_REG_PAR_ERR_bf          0, 25, 25
#define   NFP_MECSR_CTX_ENABLES_REG_PAR_ERR_mask        (0x1)
#define   NFP_MECSR_CTX_ENABLES_REG_PAR_ERR_bit         (25)
#define   NFP_MECSR_CTX_ENABLES_NN_MODE                 (0x1 << 20)
#define   NFP_MECSR_CTX_ENABLES_NN_MODE_bf              0, 20, 20
#define   NFP_MECSR_CTX_ENABLES_NN_MODE_mask            (0x1)
#define   NFP_MECSR_CTX_ENABLES_NN_MODE_bit             (20)
#define     NFP_MECSR_CTX_ENABLES_NN_MODE_PREV          (0x0)
#define     NFP_MECSR_CTX_ENABLES_NN_MODE_THIS          (0x100000)
#define   NFP_MECSR_CTX_ENABLES_NN_RING_EMPTY(_x)       (((_x) & 0x3) << 18)
#define   NFP_MECSR_CTX_ENABLES_NN_RING_EMPTY_bf        0, 19, 18
#define   NFP_MECSR_CTX_ENABLES_NN_RING_EMPTY_mask      (0x3)
#define   NFP_MECSR_CTX_ENABLES_NN_RING_EMPTY_shift     (18)
#define   NFP_MECSR_CTX_ENABLES_LM_ADDR_1_GLOB          (0x1 << 17)
#define   NFP_MECSR_CTX_ENABLES_LM_ADDR_1_GLOB_bf       0, 17, 17
#define   NFP_MECSR_CTX_ENABLES_LM_ADDR_1_GLOB_mask     (0x1)
#define   NFP_MECSR_CTX_ENABLES_LM_ADDR_1_GLOB_bit      (17)
#define   NFP_MECSR_CTX_ENABLES_LM_ADDR_0_GLOB          (0x1 << 16)
#define   NFP_MECSR_CTX_ENABLES_LM_ADDR_0_GLOB_bf       0, 16, 16
#define   NFP_MECSR_CTX_ENABLES_LM_ADDR_0_GLOB_mask     (0x1)
#define   NFP_MECSR_CTX_ENABLES_LM_ADDR_0_GLOB_bit      (16)
#define   NFP_MECSR_CTX_ENABLES_CONTEXTS(_x)            (((_x) & 0xff) << 8)
#define   NFP_MECSR_CTX_ENABLES_CONTEXTS_bf             0, 15, 8
#define   NFP_MECSR_CTX_ENABLES_CONTEXTS_mask           (0xff)
#define   NFP_MECSR_CTX_ENABLES_CONTEXTS_shift          (8)
/* Register Type: CondCodeEn */
#define NFP_MECSR_CC_ENABLE            0x001c
#define   NFP_MECSR_CC_ENABLE_CCCE                      (0x1 << 13)
#define   NFP_MECSR_CC_ENABLE_CCCE_bf                   0, 13, 13
#define   NFP_MECSR_CC_ENABLE_CCCE_mask                 (0x1)
#define   NFP_MECSR_CC_ENABLE_CCCE_bit                  (13)
/* Register Type: CSRCtxPtr */
#define NFP_MECSR_CSR_CTX_POINTER      0x0020
#define   NFP_MECSR_CSR_CTX_POINTER_CTX(_x)             ((_x) & 0x7)
#define   NFP_MECSR_CSR_CTX_POINTER_CTX_bf              0, 2, 0
#define   NFP_MECSR_CSR_CTX_POINTER_CTX_mask            (0x7)
#define   NFP_MECSR_CSR_CTX_POINTER_CTX_shift           (0)
/* Register Type: RegErrStatus */
#define NFP_MECSR_REG_ERROR_STATUS     0x0030
#define   NFP_MECSR_REG_ERROR_STATUS_TYPE_bf            0, 17, 16
#define   NFP_MECSR_REG_ERROR_STATUS_TYPE_mask          (0x3)
#define   NFP_MECSR_REG_ERROR_STATUS_TYPE_shift         (16)
#define     NFP_MECSR_REG_ERROR_STATUS_TYPE_GPR         (0)
#define     NFP_MECSR_REG_ERROR_STATUS_TYPE_XFER        (1)
#define     NFP_MECSR_REG_ERROR_STATUS_TYPE_NN          (2)
#define     NFP_MECSR_REG_ERROR_STATUS_TYPE_LM          (3)
#define   NFP_MECSR_REG_ERROR_STATUS_REG_bf             0, 15, 0
#define   NFP_MECSR_REG_ERROR_STATUS_REG_mask           (0xffff)
#define   NFP_MECSR_REG_ERROR_STATUS_REG_shift          (0)
/* Register Type: IndCtxStatus */
#define NFP_MECSR_INDIRECT_CTX_STS     0x0040
#define   NFP_MECSR_INDIRECT_CTX_STS_RR                 (0x1 << 31)
#define   NFP_MECSR_INDIRECT_CTX_STS_RR_bf              0, 31, 31
#define   NFP_MECSR_INDIRECT_CTX_STS_RR_mask            (0x1)
#define   NFP_MECSR_INDIRECT_CTX_STS_RR_bit             (31)
#define   NFP_MECSR_INDIRECT_CTX_STS_CTX_PC(_x)         ((_x) & 0x3fff)
#define   NFP_MECSR_INDIRECT_CTX_STS_CTX_PC_bf          0, 13, 0
#define   NFP_MECSR_INDIRECT_CTX_STS_CTX_PC_mask        (0x3fff)
#define   NFP_MECSR_INDIRECT_CTX_STS_CTX_PC_shift       (0)
/* Register Type: ActCtxStatus */
#define NFP_MECSR_ACTIVE_CTX_STS       0x0044
#define   NFP_MECSR_ACTIVE_CTX_STS_AB0                  (0x1 << 31)
#define   NFP_MECSR_ACTIVE_CTX_STS_AB0_bf               0, 31, 31
#define   NFP_MECSR_ACTIVE_CTX_STS_AB0_mask             (0x1)
#define   NFP_MECSR_ACTIVE_CTX_STS_AB0_bit              (31)
#define   NFP_MECSR_ACTIVE_CTX_STS_CL_ID_bf             0, 28, 25
#define   NFP_MECSR_ACTIVE_CTX_STS_CL_ID_mask           (0xf)
#define   NFP_MECSR_ACTIVE_CTX_STS_CL_ID_shift          (25)
#define   NFP_MECSR_ACTIVE_CTX_STS_ACTXPC_bf            0, 21, 8
#define   NFP_MECSR_ACTIVE_CTX_STS_ACTXPC_mask          (0x3fff)
#define   NFP_MECSR_ACTIVE_CTX_STS_ACTXPC_shift         (8)
#define   NFP_MECSR_ACTIVE_CTX_STS_CL_ID_LSB            (0x1 << 7)
#define   NFP_MECSR_ACTIVE_CTX_STS_CL_ID_LSB_bf         0, 7, 7
#define   NFP_MECSR_ACTIVE_CTX_STS_CL_ID_LSB_mask       (0x1)
#define   NFP_MECSR_ACTIVE_CTX_STS_CL_ID_LSB_bit        (7)
#define   NFP_MECSR_ACTIVE_CTX_STS_ME_ID_bf             0, 6, 3
#define   NFP_MECSR_ACTIVE_CTX_STS_ME_ID_mask           (0xf)
#define   NFP_MECSR_ACTIVE_CTX_STS_ME_ID_shift          (3)
#define   NFP_MECSR_ACTIVE_CTX_STS_ACNO(_x)             ((_x) & 0x7)
#define   NFP_MECSR_ACTIVE_CTX_STS_ACNO_bf              0, 2, 0
#define   NFP_MECSR_ACTIVE_CTX_STS_ACNO_mask            (0x7)
#define   NFP_MECSR_ACTIVE_CTX_STS_ACNO_shift           (0)
/* Register Type: CtxSglEvt */
#define NFP_MECSR_INDIRECT_CTX_SIG_EVENTS 0x0048
#define NFP_MECSR_ACTIVE_CTX_SIG_EVENTS 0x004c
#define   NFP_MECSR_SIG_EVENTS_SIGNALS(_x)              (((_x) & 0x7fff) << 1)
#define   NFP_MECSR_SIG_EVENTS_SIGNALS_bf               0, 15, 1
#define   NFP_MECSR_SIG_EVENTS_SIGNALS_mask             (0x7fff)
#define   NFP_MECSR_SIG_EVENTS_SIGNALS_shift            (1)
#define   NFP_MECSR_SIG_EVENTS_VOLUNTARY                (0x1)
#define   NFP_MECSR_SIG_EVENTS_VOLUNTARY_bf             0, 0, 0
#define   NFP_MECSR_SIG_EVENTS_VOLUNTARY_mask           (0x1)
#define   NFP_MECSR_SIG_EVENTS_VOLUNTARY_bit            (0)
/* Register Type: CtxWkpEvt */
#define NFP_MECSR_INDIRECT_CTX_WAKEUP_EVENTS 0x0050
#define NFP_MECSR_ACTIVE_CTX_WAKEUP_EVENTS 0x0054
#define   NFP_MECSR_WAKEUP_ANY                          (0x1 << 16)
#define   NFP_MECSR_WAKEUP_ANY_bf                       0, 16, 16
#define   NFP_MECSR_WAKEUP_ANY_mask                     (0x1)
#define   NFP_MECSR_WAKEUP_ANY_bit                      (16)
#define   NFP_MECSR_WAKEUP_EVENTS(_x)                   (((_x) & 0x7fff) << 1)
#define   NFP_MECSR_WAKEUP_EVENTS_bf                    0, 15, 1
#define   NFP_MECSR_WAKEUP_EVENTS_mask                  (0x7fff)
#define   NFP_MECSR_WAKEUP_EVENTS_shift                 (1)
#define   NFP_MECSR_WAKEUP_VOLUNTARY                    (0x1)
#define   NFP_MECSR_WAKEUP_VOLUNTARY_bf                 0, 0, 0
#define   NFP_MECSR_WAKEUP_VOLUNTARY_mask               (0x1)
#define   NFP_MECSR_WAKEUP_VOLUNTARY_bit                (0)
/* Register Type: CtxFtrCnt */
#define NFP_MECSR_INDIRECT_CTX_CNT     0x0058
#define NFP_MECSR_ACTIVE_CTX_CNT       0x005c
#define   NFP_MECSR_CTX_CNT(_x)                         ((_x) & 0xffff)
#define   NFP_MECSR_CTX_CNT_bf                          0, 15, 0
#define   NFP_MECSR_CTX_CNT_mask                        (0xffff)
#define   NFP_MECSR_CTX_CNT_shift                       (0)
/* Register Type: LMAddr */
#define NFP_MECSR_INDIRECT_LM_ADDR_0   0x0060
#define NFP_MECSR_ACTIVE_LM_ADDR_0     0x0064
#define NFP_MECSR_INDIRECT_LM_ADDR_1   0x0068
#define NFP_MECSR_ACTIVE_LM_ADDR_1     0x006c
#define   NFP_MECSR_LM_ADDR(_x)                         (((_x) & 0x3ff) << 2)
#define   NFP_MECSR_LM_ADDR_bf                          0, 11, 2
#define   NFP_MECSR_LM_ADDR_mask                        (0x3ff)
#define   NFP_MECSR_LM_ADDR_shift                       (2)
/* Register Type: ByteIndex */
#define NFP_MECSR_BYTE_INDEX           0x0070
#define   NFP_MECSR_BYTE_N0(_x)                         ((_x) & 0x3)
#define   NFP_MECSR_BYTE_N0_bf                          0, 1, 0
#define   NFP_MECSR_BYTE_N0_mask                        (0x3)
#define   NFP_MECSR_BYTE_N0_shift                       (0)
/* Register Type: XferIndex */
#define NFP_MECSR_T_INDEX              0x0074
#define   NFP_MECSR_XFER_INDEX(_x)                      (((_x) & 0xff) << 2)
#define   NFP_MECSR_XFER_INDEX_bf                       0, 9, 2
#define   NFP_MECSR_XFER_INDEX_mask                     (0xff)
#define   NFP_MECSR_XFER_INDEX_shift                    (2)
/* Register Type: FtrCntSgl */
#define NFP_MECSR_INDIRECT_CTX_CNT_SIGNAL 0x0078
#define NFP_MECSR_ACTIVE_CTX_CNT_SIGNAL 0x007c
#define   NFP_MECSR_CTX_CNT_SIGNAL_NO(_x)               ((_x) & 0xf)
#define   NFP_MECSR_CTX_CNT_SIGNAL_NO_bf                0, 3, 0
#define   NFP_MECSR_CTX_CNT_SIGNAL_NO_mask              (0xf)
#define   NFP_MECSR_CTX_CNT_SIGNAL_NO_shift             (0)
/* Register Type: NNPutGet */
#define NFP_MECSR_NN_PUT               0x0080
#define NFP_MECSR_NN_GET               0x0084
#define   NFP_MECSR_NN_REG_INDEX(_x)                    ((_x) & 0x7f)
#define   NFP_MECSR_NN_REG_INDEX_bf                     0, 6, 0
#define   NFP_MECSR_NN_REG_INDEX_mask                   (0x7f)
#define   NFP_MECSR_NN_REG_INDEX_shift                  (0)
/* Register Type: TimestampLow */
#define NFP_MECSR_TIMESTAMP_LO         0x00c0
/* Register Type: TimestampHgh */
#define NFP_MECSR_TIMESTAMP_HI         0x00c4
/* Register Type: LMAddrBytIdx */
#define NFP_MECSR_INDIRECT_LM_ADDR_0_BYTE_INDEX 0x00e0
#define NFP_MECSR_ACTIVE_LM_ADDR_0_BYTE_INDEX 0x00e4
#define NFP_MECSR_INDIRECT_LM_ADDR_1_BYTE_INDEX 0x00e8
#define NFP_MECSR_ACTIVE_LM_ADDR_1_BYTE_INDEX 0x00ec
/* Register Type: XfrAndBytIdx */
#define NFP_MECSR_T_INDEX_BYTE_INDEX   0x00f4
/* Register Type: NxtNghbrSgl */
#define NFP_MECSR_SIGNAL_NN            0x0100
#define   NFP_MECSR_SIGNAL_NN_THIS_CTX                  (0x1 << 7)
#define   NFP_MECSR_SIGNAL_NN_THIS_CTX_bf               0, 7, 7
#define   NFP_MECSR_SIGNAL_NN_THIS_CTX_mask             (0x1)
#define   NFP_MECSR_SIGNAL_NN_THIS_CTX_bit              (7)
#define   NFP_MECSR_SIGNAL_NN_SIG_NO(_x)                (((_x) & 0xf) << 3)
#define   NFP_MECSR_SIGNAL_NN_SIG_NO_mask               (0xf)
#define   NFP_MECSR_SIGNAL_NN_SIG_NO_shift              (3)
#define   NFP_MECSR_SIGNAL_NN_CTX(_x)                   ((_x) & 0x7)
#define   NFP_MECSR_SIGNAL_NN_CTX_mask                  (0x7)
#define   NFP_MECSR_SIGNAL_NN_CTX_shift                 (0)
/* Register Type: PrvNghbrSgl */
#define NFP_MECSR_SIGNAL_PN            0x0104
#define   NFP_MECSR_SIGNAL_PN_THIS_CTX                  (0x1 << 7)
#define   NFP_MECSR_SIGNAL_PN_THIS_CTX_bf               0, 7, 7
#define   NFP_MECSR_SIGNAL_PN_THIS_CTX_mask             (0x1)
#define   NFP_MECSR_SIGNAL_PN_THIS_CTX_bit              (7)
#define   NFP_MECSR_SIGNAL_PN_SIG_NO(_x)                (((_x) & 0xf) << 3)
#define   NFP_MECSR_SIGNAL_PN_SIG_NO_mask               (0xf)
#define   NFP_MECSR_SIGNAL_PN_SIG_NO_shift              (3)
#define   NFP_MECSR_SIGNAL_PN_CTX(_x)                   ((_x) & 0x7)
#define   NFP_MECSR_SIGNAL_PN_CTX_mask                  (0x7)
#define   NFP_MECSR_SIGNAL_PN_CTX_shift                 (0)
/* Register Type: SameMESignal */
#define NFP_MECSR_SIGNAL_SAME          0x0108
#define   NFP_MECSR_SIGNAL_SAME_NEXT_CTX                (0x1 << 7)
#define   NFP_MECSR_SIGNAL_SAME_NEXT_CTX_bf             0, 7, 7
#define   NFP_MECSR_SIGNAL_SAME_NEXT_CTX_mask           (0x1)
#define   NFP_MECSR_SIGNAL_SAME_NEXT_CTX_bit            (7)
#define   NFP_MECSR_SIGNAL_SAME_SIG_NO(_x)              (((_x) & 0xf) << 3)
#define   NFP_MECSR_SIGNAL_SAME_SIG_NO_mask             (0xf)
#define   NFP_MECSR_SIGNAL_SAME_SIG_NO_shift            (3)
#define   NFP_MECSR_SIGNAL_SAME_CTX(_x)                 ((_x) & 0x7)
#define   NFP_MECSR_SIGNAL_SAME_CTX_mask                (0x7)
#define   NFP_MECSR_SIGNAL_SAME_CTX_shift               (0)
/* Register Type: CRCRemainder */
#define NFP_MECSR_CRC_REMAINDER        0x0140
/* Register Type: ProfileCnt */
#define NFP_MECSR_PROFILE_CNT          0x0144
/* Register Type: PseudoRndNum */
#define NFP_MECSR_RANDOM_NUMBER        0x0148
/* Register Type: MiscControl */
#define NFP_MECSR_MISC_CONTROL         0x0160
#define   NFP_MECSR_MISC_CONTROL_ECC_CORR_GATE          (0x1 << 25)
#define   NFP_MECSR_MISC_CONTROL_ECC_CORR_GATE_bf       0, 25, 25
#define   NFP_MECSR_MISC_CONTROL_ECC_CORR_GATE_mask     (0x1)
#define   NFP_MECSR_MISC_CONTROL_ECC_CORR_GATE_bit      (25)
#define     NFP_MECSR_MISC_CONTROL_ECC_CORR_GATE_DISABLE (0x0)
#define     NFP_MECSR_MISC_CONTROL_ECC_CORR_GATE_ENABLE (0x2000000)
#define   NFP_MECSR_MISC_CONTROL_PARITY_ENBLE           (0x1 << 24)
#define   NFP_MECSR_MISC_CONTROL_PARITY_ENBLE_bf        0, 24, 24
#define   NFP_MECSR_MISC_CONTROL_PARITY_ENBLE_mask      (0x1)
#define   NFP_MECSR_MISC_CONTROL_PARITY_ENBLE_bit       (24)
#define   NFP_MECSR_MISC_CONTROL_FORCE_BAR_PARITY       (0x1 << 23)
#define   NFP_MECSR_MISC_CONTROL_FORCE_BAR_PARITY_bf    0, 23, 23
#define   NFP_MECSR_MISC_CONTROL_FORCE_BAR_PARITY_mask  (0x1)
#define   NFP_MECSR_MISC_CONTROL_FORCE_BAR_PARITY_bit   (23)
#define   NFP_MECSR_MISC_CONTROL_USTORE_ECC_CORRECT     (0x1 << 12)
#define   NFP_MECSR_MISC_CONTROL_USTORE_ECC_CORRECT_bf  0, 12, 12
#define   NFP_MECSR_MISC_CONTROL_USTORE_ECC_CORRECT_mask (0x1)
#define   NFP_MECSR_MISC_CONTROL_USTORE_ECC_CORRECT_bit (12)
#define   NFP_MECSR_MISC_CONTROL_RF_RWM_SETTING(_x)     (((_x) & 0x7) << 9)
#define   NFP_MECSR_MISC_CONTROL_RF_RWM_SETTING_bf      0, 11, 9
#define   NFP_MECSR_MISC_CONTROL_RF_RWM_SETTING_mask    (0x7)
#define   NFP_MECSR_MISC_CONTROL_RF_RWM_SETTING_shift   (9)
#define   NFP_MECSR_MISC_CONTROL_CS_RWM_SETTING(_x)     (((_x) & 0x7) << 6)
#define   NFP_MECSR_MISC_CONTROL_CS_RWM_SETTING_bf      0, 8, 6
#define   NFP_MECSR_MISC_CONTROL_CS_RWM_SETTING_mask    (0x7)
#define   NFP_MECSR_MISC_CONTROL_CS_RWM_SETTING_shift   (6)
#define   NFP_MECSR_MISC_CONTROL_NN_PUSH                (0x1 << 5)
#define   NFP_MECSR_MISC_CONTROL_NN_PUSH_bf             0, 5, 5
#define   NFP_MECSR_MISC_CONTROL_NN_PUSH_mask           (0x1)
#define   NFP_MECSR_MISC_CONTROL_NN_PUSH_bit            (5)
#define     NFP_MECSR_MISC_CONTROL_NN_PUSH_FIFO         (0x0)
#define     NFP_MECSR_MISC_CONTROL_NN_PUSH_RAM          (0x20)
#define   NFP_MECSR_MISC_CONTROL_THIRD_PARTY            (0x1 << 4)
#define   NFP_MECSR_MISC_CONTROL_THIRD_PARTY_bf         0, 4, 4
#define   NFP_MECSR_MISC_CONTROL_THIRD_PARTY_mask       (0x1)
#define   NFP_MECSR_MISC_CONTROL_THIRD_PARTY_bit        (4)
#define     NFP_MECSR_MISC_CONTROL_THIRD_PARTY_40BIT    (0x0)
#define     NFP_MECSR_MISC_CONTROL_THIRD_PARTY_32BIT    (0x10)
#define   NFP_MECSR_MISC_CONTROL_INDREF_FMT             (0x1 << 3)
#define   NFP_MECSR_MISC_CONTROL_INDREF_FMT_bf          0, 3, 3
#define   NFP_MECSR_MISC_CONTROL_INDREF_FMT_mask        (0x1)
#define   NFP_MECSR_MISC_CONTROL_INDREF_FMT_bit         (3)
#define     NFP_MECSR_MISC_CONTROL_INDREF_FMT_LEGACY    (0x0)
#define     NFP_MECSR_MISC_CONTROL_INDREF_FMT_EXTENDED  (0x8)
#define   NFP_MECSR_MISC_CONTROL_SHARE_USTORE           (0x1 << 2)
#define   NFP_MECSR_MISC_CONTROL_SHARE_USTORE_bf        0, 2, 2
#define   NFP_MECSR_MISC_CONTROL_SHARE_USTORE_mask      (0x1)
#define   NFP_MECSR_MISC_CONTROL_SHARE_USTORE_bit       (2)
#define     NFP_MECSR_MISC_CONTROL_SHARE_USTORE_NOT_SHARED (0x0)
#define     NFP_MECSR_MISC_CONTROL_SHARE_USTORE_SHARED  (0x4)
/* Register Type: Mailbox */
#define NFP_MECSR_MAILBOX_0            0x0170
#define NFP_MECSR_MAILBOX_1            0x0174
#define NFP_MECSR_MAILBOX_2            0x0178
#define NFP_MECSR_MAILBOX_3            0x017c
/* Register Type: LclCSRStatus */
#define NFP_MECSR_LOCAL_CSR_STATUS     0x0180
#define   NFP_MECSR_LOCAL_CSR_STATUS_FAILED             (0x1)
#define   NFP_MECSR_LOCAL_CSR_STATUS_FAILED_bf          0, 0, 0
#define   NFP_MECSR_LOCAL_CSR_STATUS_FAILED_mask        (0x1)
#define   NFP_MECSR_LOCAL_CSR_STATUS_FAILED_bit         (0)

#endif /* __NFP_SDK__NFP3200_NFP_ME_H__ */
