/*
 * Copyright (C) 2012  Netronome Systems, Inc.  All rights reserved.
 *
 * @file        nfp3200/nfp_ecc.h
 * @brief       Register definitions for ECC
 *
 * Disclaimer: this file is provided without any warranty
 * as part of an early-access program, and the content is
 * bound to change before the final release.
 */

#ifndef __NFP_SDK__NFP3200_NFP_ECC_H__
#define __NFP_SDK__NFP3200_NFP_ECC_H__

/* HGID: nfp3200/ecc.desc = 9aa6ddb03994 */
#define NFP_ECC_CTL(_x)                (0x0 + (0x40 * (_x)))
/* Register Type: ECCEnable */
#define NFP_ECC_CTL_ENABLE             0x0000
#define   NFP_ECC_CTL_ENABLE_ALL                        (0x1)
#define   NFP_ECC_CTL_ENABLE_ALL_bf                     0, 0, 0
#define   NFP_ECC_CTL_ENABLE_ALL_mask                   (0x1)
#define   NFP_ECC_CTL_ENABLE_ALL_bit                    (0)
/* Register Type: ECCInjectClear */
#define NFP_ECC_CTL_INJECT_CLEAR       0x0004
/* Register Type: ECCInjectError */
#define NFP_ECC_CTL_INJECT_ECC_PERM    0x0008
#define NFP_ECC_CTL_INJECT_DATA_PERM   0x000c
#define NFP_ECC_CTL_INJECT_ECC_ONCE    0x0010
#define NFP_ECC_CTL_INJECT_DATA_ONCE   0x0014
#define   NFP_ECC_CTL_INJECT_ADDR_MATCH(_x)             (((_x) & 0xff) << 8)
#define   NFP_ECC_CTL_INJECT_ADDR_MATCH_mask            (0xff)
#define   NFP_ECC_CTL_INJECT_ADDR_MATCH_shift           (8)
#define   NFP_ECC_CTL_INJECT_DELTA(_x)                  ((_x) & 0xff)
#define   NFP_ECC_CTL_INJECT_DELTA_mask                 (0xff)
#define   NFP_ECC_CTL_INJECT_DELTA_shift                (0)
/* Register Type: ECCError */
#define NFP_ECC_CTL_ERR_FIRST          0x0020
#define NFP_ECC_CTL_ERR_LAST           0x0024
#define   NFP_ECC_CTL_ERR_VALID                         (0x1 << 31)
#define   NFP_ECC_CTL_ERR_VALID_bf                      0, 31, 31
#define   NFP_ECC_CTL_ERR_VALID_mask                    (0x1)
#define   NFP_ECC_CTL_ERR_VALID_bit                     (31)
#define   NFP_ECC_CTL_ERR_MULTIBIT                      (0x1 << 25)
#define   NFP_ECC_CTL_ERR_MULTIBIT_bf                   0, 25, 25
#define   NFP_ECC_CTL_ERR_MULTIBIT_mask                 (0x1)
#define   NFP_ECC_CTL_ERR_MULTIBIT_bit                  (25)
#define   NFP_ECC_CTL_ERR_SINGLEBIT                     (0x1 << 24)
#define   NFP_ECC_CTL_ERR_SINGLEBIT_bf                  0, 24, 24
#define   NFP_ECC_CTL_ERR_SINGLEBIT_mask                (0x1)
#define   NFP_ECC_CTL_ERR_SINGLEBIT_bit                 (24)
#define   NFP_ECC_CTL_ERR_DELTA_bf                      0, 23, 16
#define   NFP_ECC_CTL_ERR_DELTA_mask                    (0xff)
#define   NFP_ECC_CTL_ERR_DELTA_shift                   (16)
#define   NFP_ECC_CTL_ERR_ADDR_bf                       0, 15, 0
#define   NFP_ECC_CTL_ERR_ADDR_mask                     (0xffff)
#define   NFP_ECC_CTL_ERR_ADDR_shift                    (0)
/* Register Type: ECCClearErrors */
#define NFP_ECC_CTL_CLEAR_ERR          0x0020
/* Register Type: ECCErrorCounts */
#define NFP_ECC_CTL_ERR_COUNT          0x0028
#define NFP_ECC_CTL_ERR_COUNT_RESET    0x002c
#define   NFP_ECC_CTL_ERR_COUNT_MULTIS_bf               0, 31, 16
#define   NFP_ECC_CTL_ERR_COUNT_MULTIS_mask             (0xffff)
#define   NFP_ECC_CTL_ERR_COUNT_MULTIS_shift            (16)
#define   NFP_ECC_CTL_ERR_COUNT_SINGLES_bf              0, 15, 0
#define   NFP_ECC_CTL_ERR_COUNT_SINGLES_mask            (0xffff)
#define   NFP_ECC_CTL_ERR_COUNT_SINGLES_shift           (0)
/* Register Type: ECCSRAMTiming */
#define NFP_ECC_CTL_SRAM_TIMING        0x0030
#define   NFP_ECC_CTL_SRAM_TIMING_PARC(_x)              (((_x) & 0x7) << 8)
#define   NFP_ECC_CTL_SRAM_TIMING_PARC_bf               0, 10, 8
#define   NFP_ECC_CTL_SRAM_TIMING_PARC_mask             (0x7)
#define   NFP_ECC_CTL_SRAM_TIMING_PARC_shift            (8)
#define   NFP_ECC_CTL_SRAM_TIMING_PARB(_x)              (((_x) & 0x7) << 4)
#define   NFP_ECC_CTL_SRAM_TIMING_PARB_bf               0, 6, 4
#define   NFP_ECC_CTL_SRAM_TIMING_PARB_mask             (0x7)
#define   NFP_ECC_CTL_SRAM_TIMING_PARB_shift            (4)
#define   NFP_ECC_CTL_SRAM_TIMING_PARA(_x)              ((_x) & 0x7)
#define   NFP_ECC_CTL_SRAM_TIMING_PARA_bf               0, 2, 0
#define   NFP_ECC_CTL_SRAM_TIMING_PARA_mask             (0x7)
#define   NFP_ECC_CTL_SRAM_TIMING_PARA_shift            (0)
/* Register Type: ECCLongAddress */
#define NFP_ECC_CTL_LONG_ADDR          0x0034

#endif /* __NFP_SDK__NFP3200_NFP_ECC_H__ */
