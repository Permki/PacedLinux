/*
 * Copyright (C) 2009 - 2012  Netronome Systems, Inc.  All rights reserved.
 *
 * @file        nfp3200/nfp_cap.h
 * @brief       Register definitions for CSR Access Proxy (CAP)
 *
 * Disclaimer: this file is provided without any warranty
 * as part of an early-access program, and the content is
 * bound to change before the final release.
 */

#ifndef __NFP_SDK__NFP3200_NFP_CAP_H__
#define __NFP_SDK__NFP3200_NFP_CAP_H__

#define NFP_CAP_CSR             0x00004000

#define NFP_CAP_MEXFER(mecl,me) (0x00008000 | \
                                (((mecl) & 0xe) << 16) | \
                                (((mecl) & 1) << 14) | \
                                (((me) & 0x7) << 10))

#define NFP_CAP_MECSR(mecl,me)  (0x00018000 | \
                                (((mecl) & 0xe) << 16) | \
                                (((mecl) & 1) << 14) | \
                                (((me) & 0x7) << 10))

#define NFP_CAP_XPB             0x2000000

/* HGID: nfp3200/cap.desc = 5a64ad6e9b74 */
/* Register Type: ThreadMessageGeneric */
#define NFP_SHAC_THD_MSG_GEN           0x0000
#define   NFP_SHAC_THD_MSG_GEN_THD_STS(_x)              ((_x) & 0xff)
#define   NFP_SHAC_THD_MSG_GEN_THD_STS_bf               0, 7, 0
#define   NFP_SHAC_THD_MSG_GEN_THD_STS_mask             (0xff)
#define   NFP_SHAC_THD_MSG_GEN_THD_STS_shift            (0)
/* Register Type: ThreadMessageSummary */
#define NFP_SHAC_THD_MSG_SUM_0_0       0x0004
#define NFP_SHAC_THD_MSG_SUM_0_1       0x0008
#define NFP_SHAC_THD_MSG_SUM_1_0       0x000c
#define NFP_SHAC_THD_MSG_SUM_1_1       0x0010
#define   NFP_SHAC_THD_MSG_SUM_0_0_VALID_ME3_bf         0, 31, 24
#define   NFP_SHAC_THD_MSG_SUM_0_0_VALID_ME3_mask       (0xff)
#define   NFP_SHAC_THD_MSG_SUM_0_0_VALID_ME3_shift      (24)
#define   NFP_SHAC_THD_MSG_SUM_0_0_VALID_ME2_bf         0, 23, 16
#define   NFP_SHAC_THD_MSG_SUM_0_0_VALID_ME2_mask       (0xff)
#define   NFP_SHAC_THD_MSG_SUM_0_0_VALID_ME2_shift      (16)
#define   NFP_SHAC_THD_MSG_SUM_0_0_VALID_ME1_bf         0, 15, 8
#define   NFP_SHAC_THD_MSG_SUM_0_0_VALID_ME1_mask       (0xff)
#define   NFP_SHAC_THD_MSG_SUM_0_0_VALID_ME1_shift      (8)
#define   NFP_SHAC_THD_MSG_SUM_0_0_VALID_ME0_bf         0, 7, 0
#define   NFP_SHAC_THD_MSG_SUM_0_0_VALID_ME0_mask       (0xff)
#define   NFP_SHAC_THD_MSG_SUM_0_0_VALID_ME0_shift      (0)
/* Register Type: SelfDestruct */
#define NFP_SHAC_SELF_DESTRUCT0        0x0014
#define NFP_SHAC_SELF_DESTRUCT1        0x0018
#define   NFP_SHAC_SELF_DESTRUCT_DATA(_x)               (_x)
#define   NFP_SHAC_SELF_DESTRUCT_DATA_bf                0, 31, 0
#define   NFP_SHAC_SELF_DESTRUCT_DATA_mask              (0xffffffff)
#define   NFP_SHAC_SELF_DESTRUCT_DATA_shift             (0)
/* Register Type: InterThreadSignal */
#define NFP_SHAC_INTR_THD_SIG          0x001c
#define   NFP_SHAC_INTR_THD_SIG_ME_NO(_x)               (((_x) & 0xff) << 7)
#define   NFP_SHAC_INTR_THD_SIG_ME_NO_mask              (0xff)
#define   NFP_SHAC_INTR_THD_SIG_ME_NO_shift             (7)
#define   NFP_SHAC_INTR_THD_SIG_THD_NO(_x)              (((_x) & 0x7) << 4)
#define   NFP_SHAC_INTR_THD_SIG_THD_NO_mask             (0x7)
#define   NFP_SHAC_INTR_THD_SIG_THD_NO_shift            (4)
#define   NFP_SHAC_INTR_THD_SIG_SIG(_x)                 ((_x) & 0xf)
#define   NFP_SHAC_INTR_THD_SIG_SIG_mask                (0xf)
#define   NFP_SHAC_INTR_THD_SIG_SIG_shift               (0)
/* Register Type: ThreadMessageClear */
#define NFP_SHAC_THD_MSG_CLR(_me,_ctx) (0x100 + (0x20 * ((_me) & 0xf)) + (0x4 * ((_ctx) & 0x7)))
#define   NFP_SHAC_THD_MSG_CLR_THD_MSG(_x)              ((_x) & 0xff)
#define   NFP_SHAC_THD_MSG_CLR_THD_MSG_bf               0, 7, 0
#define   NFP_SHAC_THD_MSG_CLR_THD_MSG_mask             (0xff)
#define   NFP_SHAC_THD_MSG_CLR_THD_MSG_shift            (0)
/* Register Type: ThreadMessage */
#define NFP_SHAC_THD_MSG(_me,_ctx)     (0x500 + (0x20 * ((_me) & 0xf)) + (0x4 * ((_ctx) & 0x7)))
#define   NFP_SHAC_THD_MSG_THD_MSG(_x)                  ((_x) & 0xff)
#define   NFP_SHAC_THD_MSG_THD_MSG_bf                   0, 7, 0
#define   NFP_SHAC_THD_MSG_THD_MSG_mask                 (0xff)
#define   NFP_SHAC_THD_MSG_THD_MSG_shift                (0)
/* Register Type: RingBase */
#define NFP_SHAC_GS_RING_BASE(_ring)   (0x800 + (0x10 * ((_ring) & 0xf)))
#define   NFP_SHAC_GS_RING_BASE_SIZE(_x)                (((_x) & 0x3) << 30)
#define   NFP_SHAC_GS_RING_BASE_SIZE_bf                 0, 31, 30
#define   NFP_SHAC_GS_RING_BASE_SIZE_mask               (0x3)
#define   NFP_SHAC_GS_RING_BASE_SIZE_shift              (30)
#define     NFP_SHAC_GS_RING_BASE_SIZE_128              (0)
#define     NFP_SHAC_GS_RING_BASE_SIZE_256              (1)
#define     NFP_SHAC_GS_RING_BASE_SIZE_512              (2)
#define     NFP_SHAC_GS_RING_BASE_SIZE_1024             (3)
#define   NFP_SHAC_GS_RING_BASE_STS                     (0x1 << 26)
#define   NFP_SHAC_GS_RING_BASE_STS_bf                  0, 26, 26
#define   NFP_SHAC_GS_RING_BASE_STS_mask                (0x1)
#define   NFP_SHAC_GS_RING_BASE_STS_bit                 (26)
#define     NFP_SHAC_GS_RING_BASE_STS_FULL              (0x0)
#define     NFP_SHAC_GS_RING_BASE_STS_EMPTY             (0x4000000)
#define   NFP_SHAC_GS_RING_BASE_BASE(_x)                (((_x) & 0x1f) << 9)
#define   NFP_SHAC_GS_RING_BASE_BASE_bf                 0, 13, 9
#define   NFP_SHAC_GS_RING_BASE_BASE_mask               (0x1f)
#define   NFP_SHAC_GS_RING_BASE_BASE_shift              (9)
/* Register Type: RingHead */
#define NFP_SHAC_GS_RING_HEAD(_ring)   (0x804 + (0x10 * ((_ring) & 0xf)))
#define   NFP_SHAC_GS_RING_HEAD_OFF(_x)                 (((_x) & 0x3ff) << 2)
#define   NFP_SHAC_GS_RING_HEAD_OFF_bf                  0, 11, 2
#define   NFP_SHAC_GS_RING_HEAD_OFF_mask                (0x3ff)
#define   NFP_SHAC_GS_RING_HEAD_OFF_shift               (2)
/* Register Type: RingTail */
#define NFP_SHAC_GS_RING_TAIL(_ring)   (0x808 + (0x10 * ((_ring) & 0xf)))
#define   NFP_SHAC_GS_RING_TAIL_OFF(_x)                 (((_x) & 0x3ff) << 2)
#define   NFP_SHAC_GS_RING_TAIL_OFF_bf                  0, 11, 2
#define   NFP_SHAC_GS_RING_TAIL_OFF_mask                (0x3ff)
#define   NFP_SHAC_GS_RING_TAIL_OFF_shift               (2)
/* Register Type: HashMultiplier48_0 */
#define NFP_SHAC_HASH_MULT48_0         0x0900
#define   NFP_SHAC_HASH_MULT48_0_MULT48_0(_x)           (_x)
#define   NFP_SHAC_HASH_MULT48_0_MULT48_0_bf            0, 31, 0
#define   NFP_SHAC_HASH_MULT48_0_MULT48_0_mask          (0xffffffff)
#define   NFP_SHAC_HASH_MULT48_0_MULT48_0_shift         (0)
/* Register Type: HashMultiplier48_1 */
#define NFP_SHAC_HASH_MULT48_1         0x0904
#define   NFP_SHAC_HASH_MULT48_1_MULT48_1(_x)           ((_x) & 0xffff)
#define   NFP_SHAC_HASH_MULT48_1_MULT48_1_bf            0, 15, 0
#define   NFP_SHAC_HASH_MULT48_1_MULT48_1_mask          (0xffff)
#define   NFP_SHAC_HASH_MULT48_1_MULT48_1_shift         (0)
/* Register Type: HashMultiplier64_0 */
#define NFP_SHAC_HASH_MULT64_0         0x0908
#define   NFP_SHAC_HASH_MULT64_0_MULT64_0(_x)           (_x)
#define   NFP_SHAC_HASH_MULT64_0_MULT64_0_bf            0, 31, 0
#define   NFP_SHAC_HASH_MULT64_0_MULT64_0_mask          (0xffffffff)
#define   NFP_SHAC_HASH_MULT64_0_MULT64_0_shift         (0)
/* Register Type: HashMultiplier64_1 */
#define NFP_SHAC_HASH_MULT64_1         0x090c
#define   NFP_SHAC_HASH_MULT64_1_MULT64_1(_x)           (_x)
#define   NFP_SHAC_HASH_MULT64_1_MULT64_1_bf            0, 31, 0
#define   NFP_SHAC_HASH_MULT64_1_MULT64_1_mask          (0xffffffff)
#define   NFP_SHAC_HASH_MULT64_1_MULT64_1_shift         (0)
/* Register Type: HashMultiplier128_0 */
#define NFP_SHAC_HASH_MULT128_0        0x0910
#define   NFP_SHAC_HASH_MULT128_0_MULT128_0(_x)         (_x)
#define   NFP_SHAC_HASH_MULT128_0_MULT128_0_bf          0, 31, 0
#define   NFP_SHAC_HASH_MULT128_0_MULT128_0_mask        (0xffffffff)
#define   NFP_SHAC_HASH_MULT128_0_MULT128_0_shift       (0)
/* Register Type: HashMultiplier128_1 */
#define NFP_SHAC_HASH_MULT128_1        0x0914
#define   NFP_SHAC_HASH_MULT128_1_MULT128_1(_x)         (_x)
#define   NFP_SHAC_HASH_MULT128_1_MULT128_1_bf          0, 31, 0
#define   NFP_SHAC_HASH_MULT128_1_MULT128_1_mask        (0xffffffff)
#define   NFP_SHAC_HASH_MULT128_1_MULT128_1_shift       (0)
/* Register Type: HashMultiplier128_2 */
#define NFP_SHAC_HASH_MULT128_2        0x0918
#define   NFP_SHAC_HASH_MULT128_2_MULT128_2(_x)         (_x)
#define   NFP_SHAC_HASH_MULT128_2_MULT128_2_bf          0, 31, 0
#define   NFP_SHAC_HASH_MULT128_2_MULT128_2_mask        (0xffffffff)
#define   NFP_SHAC_HASH_MULT128_2_MULT128_2_shift       (0)
/* Register Type: HashMultiplier128_3 */
#define NFP_SHAC_HASH_MULT128_3        0x091c
#define   NFP_SHAC_HASH_MULT128_3_MULT128_3(_x)         (_x)
#define   NFP_SHAC_HASH_MULT128_3_MULT128_3_bf          0, 31, 0
#define   NFP_SHAC_HASH_MULT128_3_MULT128_3_mask        (0xffffffff)
#define   NFP_SHAC_HASH_MULT128_3_MULT128_3_shift       (0)
/* Register Type: ProductID */
#define NFP_SHAC_YLD_PRODUCT_ID        0x0a00
#define   NFP_SHAC_YLD_PRODUCT_ID_MAJOR_TYPE_bf         0, 20, 16
#define   NFP_SHAC_YLD_PRODUCT_ID_MAJOR_TYPE_mask       (0x1f)
#define   NFP_SHAC_YLD_PRODUCT_ID_MAJOR_TYPE_shift      (16)
#define     NFP_SHAC_YLD_PRODUCT_ID_MAJOR_TYPE_NFP3200  (0)
#define   NFP_SHAC_YLD_PRODUCT_ID_MINOR_TYPE_bf         0, 15, 8
#define   NFP_SHAC_YLD_PRODUCT_ID_MINOR_TYPE_mask       (0xff)
#define   NFP_SHAC_YLD_PRODUCT_ID_MINOR_TYPE_shift      (8)
#define     NFP_SHAC_YLD_PRODUCT_ID_MINOR_TYPE_NFP3200  (0)
#define   NFP_SHAC_YLD_PRODUCT_ID_MAJOR_REV_bf          0, 7, 4
#define   NFP_SHAC_YLD_PRODUCT_ID_MAJOR_REV_mask        (0xf)
#define   NFP_SHAC_YLD_PRODUCT_ID_MAJOR_REV_shift       (4)
#define     NFP_SHAC_YLD_PRODUCT_ID_MAJOR_REV_REV_A     (0)
#define   NFP_SHAC_YLD_PRODUCT_ID_MINOR_REV_bf          0, 3, 0
#define   NFP_SHAC_YLD_PRODUCT_ID_MINOR_REV_mask        (0xf)
#define   NFP_SHAC_YLD_PRODUCT_ID_MINOR_REV_shift       (0)
#define     NFP_SHAC_YLD_PRODUCT_ID_MINOR_REV_REV_0     (0)
/* Register Type: MiscControl */
#define NFP_SHAC_YLD_MISC_CTRL         0x0a04
#define   NFP_SHAC_YLD_MISC_CTRL_RAM_READ_WRITE_MARGIN_WRITE (0x1 << 30)
#define   NFP_SHAC_YLD_MISC_CTRL_RAM_READ_WRITE_MARGIN_WRITE_bf 0, 30, 30
#define   NFP_SHAC_YLD_MISC_CTRL_RAM_READ_WRITE_MARGIN_WRITE_mask (0x1)
#define   NFP_SHAC_YLD_MISC_CTRL_RAM_READ_WRITE_MARGIN_WRITE_bit (30)
#define     NFP_SHAC_YLD_MISC_CTRL_RAM_READ_WRITE_MARGIN_WRITE_DISABLE (0x0)
#define     NFP_SHAC_YLD_MISC_CTRL_RAM_READ_WRITE_MARGIN_WRITE_ENABLE (0x40000000)
#define   NFP_SHAC_YLD_MISC_CTRL_SERIAL_DEV3_MDIO       (0x1 << 29)
#define   NFP_SHAC_YLD_MISC_CTRL_SERIAL_DEV3_MDIO_bf    0, 29, 29
#define   NFP_SHAC_YLD_MISC_CTRL_SERIAL_DEV3_MDIO_mask  (0x1)
#define   NFP_SHAC_YLD_MISC_CTRL_SERIAL_DEV3_MDIO_bit   (29)
#define     NFP_SHAC_YLD_MISC_CTRL_SERIAL_DEV3_MDIO_NOT_PRESET (0x0)
#define     NFP_SHAC_YLD_MISC_CTRL_SERIAL_DEV3_MDIO_PRESENT (0x20000000)
#define   NFP_SHAC_YLD_MISC_CTRL_SERIAL_DEV2_MDIO       (0x1 << 28)
#define   NFP_SHAC_YLD_MISC_CTRL_SERIAL_DEV2_MDIO_bf    0, 28, 28
#define   NFP_SHAC_YLD_MISC_CTRL_SERIAL_DEV2_MDIO_mask  (0x1)
#define   NFP_SHAC_YLD_MISC_CTRL_SERIAL_DEV2_MDIO_bit   (28)
#define     NFP_SHAC_YLD_MISC_CTRL_SERIAL_DEV2_MDIO_NOT_PRESET (0x0)
#define     NFP_SHAC_YLD_MISC_CTRL_SERIAL_DEV2_MDIO_PRESENT (0x10000000)
#define   NFP_SHAC_YLD_MISC_CTRL_GLOBAL_SHAC_T_RWM_DELAY(_x) (((_x) & 0x7) << 20)
#define   NFP_SHAC_YLD_MISC_CTRL_GLOBAL_SHAC_T_RWM_DELAY_bf 0, 22, 20
#define   NFP_SHAC_YLD_MISC_CTRL_GLOBAL_SHAC_T_RWM_DELAY_mask (0x7)
#define   NFP_SHAC_YLD_MISC_CTRL_GLOBAL_SHAC_T_RWM_DELAY_shift (20)
#define   NFP_SHAC_YLD_MISC_CTRL_SCRATCH_RAM_T_RWM_DELAY(_x) (((_x) & 0x7) << 16)
#define   NFP_SHAC_YLD_MISC_CTRL_SCRATCH_RAM_T_RWM_DELAY_bf 0, 18, 16
#define   NFP_SHAC_YLD_MISC_CTRL_SCRATCH_RAM_T_RWM_DELAY_mask (0x7)
#define   NFP_SHAC_YLD_MISC_CTRL_SCRATCH_RAM_T_RWM_DELAY_shift (16)
#define   NFP_SHAC_YLD_MISC_CTRL_SERIAL_DEV3_RD_CAPTURE_DELAY_CTRL(_x) (((_x) & 0x3) << 14)
#define   NFP_SHAC_YLD_MISC_CTRL_SERIAL_DEV3_RD_CAPTURE_DELAY_CTRL_bf 0, 15, 14
#define   NFP_SHAC_YLD_MISC_CTRL_SERIAL_DEV3_RD_CAPTURE_DELAY_CTRL_mask (0x3)
#define   NFP_SHAC_YLD_MISC_CTRL_SERIAL_DEV3_RD_CAPTURE_DELAY_CTRL_shift (14)
#define     NFP_SHAC_YLD_MISC_CTRL_SERIAL_DEV3_RD_CAPTURE_DELAY_CTRL_NO_DELAY (0)
#define     NFP_SHAC_YLD_MISC_CTRL_SERIAL_DEV3_RD_CAPTURE_DELAY_CTRL_2CYCLES (1)
#define     NFP_SHAC_YLD_MISC_CTRL_SERIAL_DEV3_RD_CAPTURE_DELAY_CTRL_3CYCLES (2)
#define     NFP_SHAC_YLD_MISC_CTRL_SERIAL_DEV3_RD_CAPTURE_DELAY_CTRL_4CYCLES (3)
#define   NFP_SHAC_YLD_MISC_CTRL_SERIAL_DEV2_RD_CAPTURE_DELAY_CTRL(_x) (((_x) & 0x3) << 12)
#define   NFP_SHAC_YLD_MISC_CTRL_SERIAL_DEV2_RD_CAPTURE_DELAY_CTRL_bf 0, 13, 12
#define   NFP_SHAC_YLD_MISC_CTRL_SERIAL_DEV2_RD_CAPTURE_DELAY_CTRL_mask (0x3)
#define   NFP_SHAC_YLD_MISC_CTRL_SERIAL_DEV2_RD_CAPTURE_DELAY_CTRL_shift (12)
#define     NFP_SHAC_YLD_MISC_CTRL_SERIAL_DEV2_RD_CAPTURE_DELAY_CTRL_NO_DELAY (0)
#define     NFP_SHAC_YLD_MISC_CTRL_SERIAL_DEV2_RD_CAPTURE_DELAY_CTRL_2CYCLES (1)
#define     NFP_SHAC_YLD_MISC_CTRL_SERIAL_DEV2_RD_CAPTURE_DELAY_CTRL_3CYCLES (2)
#define     NFP_SHAC_YLD_MISC_CTRL_SERIAL_DEV2_RD_CAPTURE_DELAY_CTRL_4CYCLES (3)
#define   NFP_SHAC_YLD_MISC_CTRL_SERIAL_DEV1_RD_CAPTURE_DELAY_CTRL(_x) (((_x) & 0x3) << 10)
#define   NFP_SHAC_YLD_MISC_CTRL_SERIAL_DEV1_RD_CAPTURE_DELAY_CTRL_bf 0, 11, 10
#define   NFP_SHAC_YLD_MISC_CTRL_SERIAL_DEV1_RD_CAPTURE_DELAY_CTRL_mask (0x3)
#define   NFP_SHAC_YLD_MISC_CTRL_SERIAL_DEV1_RD_CAPTURE_DELAY_CTRL_shift (10)
#define     NFP_SHAC_YLD_MISC_CTRL_SERIAL_DEV1_RD_CAPTURE_DELAY_CTRL_NO_DELAY (0)
#define     NFP_SHAC_YLD_MISC_CTRL_SERIAL_DEV1_RD_CAPTURE_DELAY_CTRL_2CYCLES (1)
#define     NFP_SHAC_YLD_MISC_CTRL_SERIAL_DEV1_RD_CAPTURE_DELAY_CTRL_3CYCLES (2)
#define     NFP_SHAC_YLD_MISC_CTRL_SERIAL_DEV1_RD_CAPTURE_DELAY_CTRL_4CYCLES (3)
#define   NFP_SHAC_YLD_MISC_CTRL_SERIAL_DEV0_RD_CAPTURE_DELAY_CTRL(_x) (((_x) & 0x3) << 8)
#define   NFP_SHAC_YLD_MISC_CTRL_SERIAL_DEV0_RD_CAPTURE_DELAY_CTRL_bf 0, 9, 8
#define   NFP_SHAC_YLD_MISC_CTRL_SERIAL_DEV0_RD_CAPTURE_DELAY_CTRL_mask (0x3)
#define   NFP_SHAC_YLD_MISC_CTRL_SERIAL_DEV0_RD_CAPTURE_DELAY_CTRL_shift (8)
#define     NFP_SHAC_YLD_MISC_CTRL_SERIAL_DEV0_RD_CAPTURE_DELAY_CTRL_NO_DELAY (0)
#define     NFP_SHAC_YLD_MISC_CTRL_SERIAL_DEV0_RD_CAPTURE_DELAY_CTRL_2CYCLES (1)
#define     NFP_SHAC_YLD_MISC_CTRL_SERIAL_DEV0_RD_CAPTURE_DELAY_CTRL_3CYCLES (2)
#define     NFP_SHAC_YLD_MISC_CTRL_SERIAL_DEV0_RD_CAPTURE_DELAY_CTRL_4CYCLES (3)
#define   NFP_SHAC_YLD_MISC_CTRL_ME_TIMESTAMP           (0x1 << 7)
#define   NFP_SHAC_YLD_MISC_CTRL_ME_TIMESTAMP_bf        0, 7, 7
#define   NFP_SHAC_YLD_MISC_CTRL_ME_TIMESTAMP_mask      (0x1)
#define   NFP_SHAC_YLD_MISC_CTRL_ME_TIMESTAMP_bit       (7)
#define     NFP_SHAC_YLD_MISC_CTRL_ME_TIMESTAMP_DISABLE (0x0)
#define     NFP_SHAC_YLD_MISC_CTRL_ME_TIMESTAMP_ENABLE  (0x80)
#define   NFP_SHAC_YLD_MISC_CTRL_ME_TIMESTAMP_RESTART   (0x1 << 6)
#define   NFP_SHAC_YLD_MISC_CTRL_ME_TIMESTAMP_RESTART_bf 0, 6, 6
#define   NFP_SHAC_YLD_MISC_CTRL_ME_TIMESTAMP_RESTART_mask (0x1)
#define   NFP_SHAC_YLD_MISC_CTRL_ME_TIMESTAMP_RESTART_bit (6)
#define     NFP_SHAC_YLD_MISC_CTRL_ME_TIMESTAMP_RESTART_RESTART (0x40)
/* Register Type: ShacBistCtrl */
#define NFP_SHAC_YLD_BIST_CTRL         0x0a08
#define   NFP_SHAC_YLD_BIST_CTRL_CSR_BISTFAIL_bf        0, 11, 8
#define   NFP_SHAC_YLD_BIST_CTRL_CSR_BISTFAIL_mask      (0xf)
#define   NFP_SHAC_YLD_BIST_CTRL_CSR_BISTFAIL_shift     (8)
#define   NFP_SHAC_YLD_BIST_CTRL_CSR_BISTDONE_bf        0, 7, 4
#define   NFP_SHAC_YLD_BIST_CTRL_CSR_BISTDONE_mask      (0xf)
#define   NFP_SHAC_YLD_BIST_CTRL_CSR_BISTDONE_shift     (4)
#define   NFP_SHAC_YLD_BIST_CTRL_CSR_BISTRUN(_x)        ((_x) & 0x3)
#define   NFP_SHAC_YLD_BIST_CTRL_CSR_BISTRUN_bf         0, 1, 0
#define   NFP_SHAC_YLD_BIST_CTRL_CSR_BISTRUN_mask       (0x3)
#define   NFP_SHAC_YLD_BIST_CTRL_CSR_BISTRUN_shift      (0)
/* Register Type: CppShErrCtrl */
#define NFP_SHAC_YLD_CPP_ERR_CTRL      0x0c00
#define   NFP_SHAC_YLD_CPP_ERR_CTRL_PARITY              (0x1 << 2)
#define   NFP_SHAC_YLD_CPP_ERR_CTRL_PARITY_bf           0, 2, 2
#define   NFP_SHAC_YLD_CPP_ERR_CTRL_PARITY_mask         (0x1)
#define   NFP_SHAC_YLD_CPP_ERR_CTRL_PARITY_bit          (2)
#define     NFP_SHAC_YLD_CPP_ERR_CTRL_PARITY_EVEN       (0x0)
#define     NFP_SHAC_YLD_CPP_ERR_CTRL_PARITY_ODD        (0x4)
#define   NFP_SHAC_YLD_CPP_ERR_CTRL_INTR                (0x1 << 1)
#define   NFP_SHAC_YLD_CPP_ERR_CTRL_INTR_bf             0, 1, 1
#define   NFP_SHAC_YLD_CPP_ERR_CTRL_INTR_mask           (0x1)
#define   NFP_SHAC_YLD_CPP_ERR_CTRL_INTR_bit            (1)
#define     NFP_SHAC_YLD_CPP_ERR_CTRL_INTR_ENABLE       (0x0)
#define     NFP_SHAC_YLD_CPP_ERR_CTRL_INTR_DISABLE      (0x2)
#define   NFP_SHAC_YLD_CPP_ERR_CTRL_PP_ERR              (0x1)
#define   NFP_SHAC_YLD_CPP_ERR_CTRL_PP_ERR_bf           0, 0, 0
#define   NFP_SHAC_YLD_CPP_ERR_CTRL_PP_ERR_mask         (0x1)
#define   NFP_SHAC_YLD_CPP_ERR_CTRL_PP_ERR_bit          (0)
#define     NFP_SHAC_YLD_CPP_ERR_CTRL_PP_ERR_DISABLE    (0x0)
#define     NFP_SHAC_YLD_CPP_ERR_CTRL_PP_ERR_ENABLE     (0x1)
/* Register Type: CppShErrStat */
#define NFP_SHAC_YLD_CPP_ERR_STS       0x0c04
#define   NFP_SHAC_YLD_CPP_ERR_STS_RDWR_ADDR_bf         0, 17, 6
#define   NFP_SHAC_YLD_CPP_ERR_STS_RDWR_ADDR_mask       (0xfff)
#define   NFP_SHAC_YLD_CPP_ERR_STS_RDWR_ADDR_shift      (6)
#define   NFP_SHAC_YLD_CPP_ERR_STS_ERR_TGT_bf           0, 5, 4
#define   NFP_SHAC_YLD_CPP_ERR_STS_ERR_TGT_mask         (0x3)
#define   NFP_SHAC_YLD_CPP_ERR_STS_ERR_TGT_shift        (4)
#define     NFP_SHAC_YLD_CPP_ERR_STS_ERR_TGT_SCRATCH    (0)
#define     NFP_SHAC_YLD_CPP_ERR_STS_ERR_TGT_HASH       (1)
#define     NFP_SHAC_YLD_CPP_ERR_STS_ERR_TGT_CAP        (2)
#define     NFP_SHAC_YLD_CPP_ERR_STS_ERR_TGT_XPB        (3)
#define   NFP_SHAC_YLD_CPP_ERR_STS_ERR_TYPE             (0x1 << 3)
#define   NFP_SHAC_YLD_CPP_ERR_STS_ERR_TYPE_bf          0, 3, 3
#define   NFP_SHAC_YLD_CPP_ERR_STS_ERR_TYPE_mask        (0x1)
#define   NFP_SHAC_YLD_CPP_ERR_STS_ERR_TYPE_bit         (3)
#define     NFP_SHAC_YLD_CPP_ERR_STS_ERR_TYPE_PUSH      (0x0)
#define     NFP_SHAC_YLD_CPP_ERR_STS_ERR_TYPE_PULL      (0x8)
#define   NFP_SHAC_YLD_CPP_ERR_STS_INT                  (0x1 << 2)
#define   NFP_SHAC_YLD_CPP_ERR_STS_INT_bf               0, 2, 2
#define   NFP_SHAC_YLD_CPP_ERR_STS_INT_mask             (0x1)
#define   NFP_SHAC_YLD_CPP_ERR_STS_INT_bit              (2)
#define   NFP_SHAC_YLD_CPP_ERR_STS_MULTI_ERR            (0x1 << 1)
#define   NFP_SHAC_YLD_CPP_ERR_STS_MULTI_ERR_bf         0, 1, 1
#define   NFP_SHAC_YLD_CPP_ERR_STS_MULTI_ERR_mask       (0x1)
#define   NFP_SHAC_YLD_CPP_ERR_STS_MULTI_ERR_bit        (1)
#define   NFP_SHAC_YLD_CPP_ERR_STS_ERR                  (0x1)
#define   NFP_SHAC_YLD_CPP_ERR_STS_ERR_bf               0, 0, 0
#define   NFP_SHAC_YLD_CPP_ERR_STS_ERR_mask             (0x1)
#define   NFP_SHAC_YLD_CPP_ERR_STS_ERR_bit              (0)
/* Register Type: CppShErrStCl */
#define NFP_SHAC_YLD_CPP_ERR_CLR       0x0c08
#define   NFP_SHAC_YLD_CPP_ERR_CLR_INT                  (0x1 << 2)
#define   NFP_SHAC_YLD_CPP_ERR_CLR_INT_bf               0, 2, 2
#define   NFP_SHAC_YLD_CPP_ERR_CLR_INT_mask             (0x1)
#define   NFP_SHAC_YLD_CPP_ERR_CLR_INT_bit              (2)
#define   NFP_SHAC_YLD_CPP_ERR_CLR_MERR                 (0x1 << 1)
#define   NFP_SHAC_YLD_CPP_ERR_CLR_MERR_bf              0, 1, 1
#define   NFP_SHAC_YLD_CPP_ERR_CLR_MERR_mask            (0x1)
#define   NFP_SHAC_YLD_CPP_ERR_CLR_MERR_bit             (1)
#define   NFP_SHAC_YLD_CPP_ERR_CLR_ERR                  (0x1)
#define   NFP_SHAC_YLD_CPP_ERR_CLR_ERR_bf               0, 0, 0
#define   NFP_SHAC_YLD_CPP_ERR_CLR_ERR_mask             (0x1)
#define   NFP_SHAC_YLD_CPP_ERR_CLR_ERR_bit              (0)
/* Register Type: CppShErrPpid */
#define NFP_SHAC_YLD_CPP_ERR_PPID      0x0c0c
#define   NFP_SHAC_YLD_CPP_ERR_PPID_PPID_bf             0, 31, 0
#define   NFP_SHAC_YLD_CPP_ERR_PPID_PPID_mask           (0xffffffff)
#define   NFP_SHAC_YLD_CPP_ERR_PPID_PPID_shift          (0)
/* Register Type: XpbToErrStat */
#define NFP_SHAC_YLD_XPB_TIMEOUT       0x0c80
#define   NFP_SHAC_YLD_XPB_TIMEOUT_SPI_HOLD_TIMEOUT_ERR (0x1 << 17)
#define   NFP_SHAC_YLD_XPB_TIMEOUT_SPI_HOLD_TIMEOUT_ERR_bf 0, 17, 17
#define   NFP_SHAC_YLD_XPB_TIMEOUT_SPI_HOLD_TIMEOUT_ERR_mask (0x1)
#define   NFP_SHAC_YLD_XPB_TIMEOUT_SPI_HOLD_TIMEOUT_ERR_bit (17)
#define   NFP_SHAC_YLD_XPB_TIMEOUT_XPB_TIMEOUT_ERR      (0x1 << 16)
#define   NFP_SHAC_YLD_XPB_TIMEOUT_XPB_TIMEOUT_ERR_bf   0, 16, 16
#define   NFP_SHAC_YLD_XPB_TIMEOUT_XPB_TIMEOUT_ERR_mask (0x1)
#define   NFP_SHAC_YLD_XPB_TIMEOUT_XPB_TIMEOUT_ERR_bit  (16)
#define   NFP_SHAC_YLD_XPB_TIMEOUT_XPB_CLUSTER_ID(_x)   (((_x) & 0xff) << 8)
#define   NFP_SHAC_YLD_XPB_TIMEOUT_XPB_CLUSTER_ID_bf    0, 15, 8
#define   NFP_SHAC_YLD_XPB_TIMEOUT_XPB_CLUSTER_ID_mask  (0xff)
#define   NFP_SHAC_YLD_XPB_TIMEOUT_XPB_CLUSTER_ID_shift (8)
#define   NFP_SHAC_YLD_XPB_TIMEOUT_XPB_DEVICE_ID(_x)    ((_x) & 0xff)
#define   NFP_SHAC_YLD_XPB_TIMEOUT_XPB_DEVICE_ID_bf     0, 7, 0
#define   NFP_SHAC_YLD_XPB_TIMEOUT_XPB_DEVICE_ID_mask   (0xff)
#define   NFP_SHAC_YLD_XPB_TIMEOUT_XPB_DEVICE_ID_shift  (0)
/* Register Type: ShPerfAnCtrl */
#define NFP_SHAC_YLD_PERF_ANAL_CTRL    0x0c84
#define   NFP_SHAC_YLD_PERF_ANAL_CTRL_CL_MOD5_SEL(_x)   (((_x) & 0xf) << 26)
#define   NFP_SHAC_YLD_PERF_ANAL_CTRL_CL_MOD5_SEL_bf    0, 29, 26
#define   NFP_SHAC_YLD_PERF_ANAL_CTRL_CL_MOD5_SEL_mask  (0xf)
#define   NFP_SHAC_YLD_PERF_ANAL_CTRL_CL_MOD5_SEL_shift (26)
#define   NFP_SHAC_YLD_PERF_ANAL_CTRL_CL_MOD4_SEL(_x)   (((_x) & 0xf) << 22)
#define   NFP_SHAC_YLD_PERF_ANAL_CTRL_CL_MOD4_SEL_bf    0, 25, 22
#define   NFP_SHAC_YLD_PERF_ANAL_CTRL_CL_MOD4_SEL_mask  (0xf)
#define   NFP_SHAC_YLD_PERF_ANAL_CTRL_CL_MOD4_SEL_shift (22)
#define   NFP_SHAC_YLD_PERF_ANAL_CTRL_CL_MOD3_SEL(_x)   (((_x) & 0xf) << 18)
#define   NFP_SHAC_YLD_PERF_ANAL_CTRL_CL_MOD3_SEL_bf    0, 21, 18
#define   NFP_SHAC_YLD_PERF_ANAL_CTRL_CL_MOD3_SEL_mask  (0xf)
#define   NFP_SHAC_YLD_PERF_ANAL_CTRL_CL_MOD3_SEL_shift (18)
#define   NFP_SHAC_YLD_PERF_ANAL_CTRL_CL_MOD2_SEL(_x)   (((_x) & 0xf) << 14)
#define   NFP_SHAC_YLD_PERF_ANAL_CTRL_CL_MOD2_SEL_bf    0, 17, 14
#define   NFP_SHAC_YLD_PERF_ANAL_CTRL_CL_MOD2_SEL_mask  (0xf)
#define   NFP_SHAC_YLD_PERF_ANAL_CTRL_CL_MOD2_SEL_shift (14)
#define   NFP_SHAC_YLD_PERF_ANAL_CTRL_CL_MOD1_SEL(_x)   (((_x) & 0xf) << 10)
#define   NFP_SHAC_YLD_PERF_ANAL_CTRL_CL_MOD1_SEL_bf    0, 13, 10
#define   NFP_SHAC_YLD_PERF_ANAL_CTRL_CL_MOD1_SEL_mask  (0xf)
#define   NFP_SHAC_YLD_PERF_ANAL_CTRL_CL_MOD1_SEL_shift (10)
#define   NFP_SHAC_YLD_PERF_ANAL_CTRL_CL_MOD0_SEL(_x)   (((_x) & 0xf) << 6)
#define   NFP_SHAC_YLD_PERF_ANAL_CTRL_CL_MOD0_SEL_bf    0, 9, 6
#define   NFP_SHAC_YLD_PERF_ANAL_CTRL_CL_MOD0_SEL_mask  (0xf)
#define   NFP_SHAC_YLD_PERF_ANAL_CTRL_CL_MOD0_SEL_shift (6)
#define   NFP_SHAC_YLD_PERF_ANAL_CTRL_HI_LANE_SEL(_x)   (((_x) & 0x3) << 4)
#define   NFP_SHAC_YLD_PERF_ANAL_CTRL_HI_LANE_SEL_bf    0, 5, 4
#define   NFP_SHAC_YLD_PERF_ANAL_CTRL_HI_LANE_SEL_mask  (0x3)
#define   NFP_SHAC_YLD_PERF_ANAL_CTRL_HI_LANE_SEL_shift (4)
#define   NFP_SHAC_YLD_PERF_ANAL_CTRL_MID_LANE_SEL(_x)  (((_x) & 0x3) << 2)
#define   NFP_SHAC_YLD_PERF_ANAL_CTRL_MID_LANE_SEL_bf   0, 3, 2
#define   NFP_SHAC_YLD_PERF_ANAL_CTRL_MID_LANE_SEL_mask (0x3)
#define   NFP_SHAC_YLD_PERF_ANAL_CTRL_MID_LANE_SEL_shift (2)
#define   NFP_SHAC_YLD_PERF_ANAL_CTRL_LO_LANE_SEL(_x)   ((_x) & 0x3)
#define   NFP_SHAC_YLD_PERF_ANAL_CTRL_LO_LANE_SEL_bf    0, 1, 0
#define   NFP_SHAC_YLD_PERF_ANAL_CTRL_LO_LANE_SEL_mask  (0x3)
#define   NFP_SHAC_YLD_PERF_ANAL_CTRL_LO_LANE_SEL_shift (0)

#endif /* __NFP_SDK__NFP3200_NFP_CAP_H__ */
