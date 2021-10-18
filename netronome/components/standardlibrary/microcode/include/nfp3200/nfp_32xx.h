/*
 * Copyright (C) 2009-2013 Netronome Systems, Inc.  All rights reserved.
 *
 * @file        nfp3200/nfp3200.h
 * @brief       NFP-32xx family definitions
 *
 */

#ifndef __NFP_SDK__NFP3200_NFP_32XX_H__
#define __NFP_SDK__NFP3200_NFP_32XX_H__


/* NOTE: 'addr' is byte-offset; least significant 2 bits must be 0 */
#define NFP_XPB_CAP_ADDR(cl, dev, addr)  ((1<<25) | (((cl)&0x1f)<<20) | (((dev)&0x3f)<<14) | ((addr)&0x3ffc))

#define NFP_XPB_CLUSTER_of(xpb_cap_addr)   (((xpb_cap_addr) >> 20) & 0x1f)
#define NFP_XPB_DEVICE_of(xpb_cap_addr)    (((xpb_cap_addr) >> 14) & 0x3f)
#define NFP_XPB_ADDR_of(xpb_cap_addr)      ((xpb_cap_addr) & 0x3ffc)


/* CPP Push-Pull IDs */
#define NFP_CPP_PPID_INVALID    0x00
#define NFP_CPP_PPID_MSF0       0x01
#define NFP_CPP_PPID_ME(x)      ((((x) >> 3) * 16 + 8) + ((x) & 7))
#define NFP_CPP_PPID_PCIE       0x11
#define NFP_CPP_PPID_MSF1       0x21
#define NFP_CPP_PPID_CRYPTO     0x31
#define NFP_CPP_PPID_ARM        0x41


/* CPP Target IDs */
#define NFP_CPP_TARGET_INVALID          0
#define NFP_CPP_TARGET_MSF0             1
#define NFP_CPP_TARGET_QDR              2
#define NFP_CPP_TARGET_MSF1             3
#define NFP_CPP_TARGET_HASH             4
#define NFP_CPP_TARGET_DDR              7
#define NFP_CPP_TARGET_GLOBAL_SCRATCH   8
#define NFP_CPP_TARGET_PCIE             9
#define NFP_CPP_TARGET_ARM              10
#define NFP_CPP_TARGET_CRYPTO           12
#define NFP_CPP_TARGET_CAP              13
#define NFP_CPP_TARGET_CLUSTER          14
#define NFP_CPP_TARGET_LOCAL_SCRATCH    15


/* XPB Cluster,Device table */

/* Microengines */
#define NFP_XPB_ME_CLUSTER(me)         (me), 1 /* Cluster Config */
#define NFP_XPB_ME_LSCRATCH(me)        (me), 2 /* Local scratch Config */
#define NFP_XPB_ME_LSCRATCH_ECC(me)    (me), 3 /* Local scratch ECC Monitor */

/* MSF 0 */
#define NFP_XPB_MSF0_GLOBAL            16,16 /* MSF0 Global CSR */
#define NFP_XPB_MSF0_CONFIG            16,17 /* MSF0 Configuration */
#define NFP_XPB_MSF0_ECC               16,18 /* MSF0 SPI4.2 ECC */
#define NFP_XPB_MSF0_MSF_ECC           16,19 /* MSF0 MSF ECC */
#define NFP_XPB_MSF0_INTMGR            16,20 /* MSF0 Interrupt Manager */

/* PCI Express */
#define NFP_XPB_PCIE_CSR               17,1  /* PCIe Cluster Config */
#define NFP_XPB_PCIE_MGMT              17,2  /* PCIe Management Config */
#define NFP_XPB_PCIE_QCTL_ECC          17,3  /* PCIe Queue Controller ECC */
#define NFP_XPB_PCIE_SRAM_ECC          17,4  /* PCIe Shared Memory ECC */
#define NFP_XPB_PCIE_INTMGR            17,5  /* PCIe Interrupt Manager */

/* MSF 1 */
#define NFP_XPB_MSF1_GLOBAL            18,16 /* MSF1 Global CSR */
#define NFP_XPB_MSF1_CONFIG            18,17 /* MSF1 Configuration */
#define NFP_XPB_MSF1_ECC               18,18 /* MSF1 XAUI ECC */
#define NFP_XPB_MSF1_MSF_ECC           18,19 /* MSF1 MSF ECC */
#define NFP_XPB_MSF1_INTMGR            18,20 /* MSF1 Interrupt Manager */

/* Cryptography */
#define NFP_XPB_CRYPTO_CIF             19,1  /* CIF CSR */
#define NFP_XPB_CRYPTO_DISPATCH_ECC    19,2  /* Dispatcher ECC */
#define NFP_XPB_CRYPTO_MASTER_ECC      19,3  /* CPP Master ECC */
#define NFP_XPB_CRYPTO_PKI_MPU_ECC     19,4  /* PKI MPU ECC */
#define NFP_XPB_CRYPTO_PKI_DISTR_ECC   19,5  /* PKI Distributer ECC */
#define NFP_XPB_CRYPTO_PKI             19,6  /* PKI CSR */
#define NFP_XPB_CRYPTO_BULK            19,7  /* Bulk CSR */
#define NFP_XPB_CRYPTO_BULK0           19,8  /* Bulk Engine_0 CSR */
#define NFP_XPB_CRYPTO_BULK1           19,9  /* Bulk Engine_1 CSR */
#define NFP_XPB_CRYPTO_BULK2           19,10 /* Bulk Engine_2 CSR */
#define NFP_XPB_CRYPTO_BULK3           19,11 /* Bulk Engine_3 CSR */

/* ARM11 */
#define NFP_XPB_ARM_QCTL_ECC           20,3  /* Queue Controller ECC Monitor */
#define NFP_XPB_ARM_SRAM_ECC           20,4  /* Shared Memory ECC */
#define NFP_XPB_ARM_INTMGR             20,5  /* Interrupt Manager */
#define NFP_XPB_ARM_CSR                20,6  /* Local CSR */

/* Memory Unit */
#define NFP_XPB_MU_CSR                 21,1  /* Memory Unit CSR */
#define NFP_XPB_MU_PCTL0_APB           21,2  /* Synopsys PCTL0 APB Port */
#define NFP_XPB_MU_PCTL1_APB           21,3  /* Synopsys PCTL1 APB Port */
#define NFP_XPB_MU_MOVER_ECC           21,8  /* Data Mover ECC */
#define NFP_XPB_MU_QUEUE_ECC           21,8  /* Queue ECC */
#define NFP_XPB_MU_DCACHE_ECC          21,10 /* DCache ECC */
#define NFP_XPB_MU_CACHE_TAG_0         21,16 /* Cache Tag Bank0 */
#define NFP_XPB_MU_CACHE_TAG_0_ECC     21,17 /* Cache Tag Bank0 ECC */
#define NFP_XPB_MU_CACHE_TAG_1         21,18 /* Cache Tag Bank1 */
#define NFP_XPB_MU_CACHE_TAG_1_ECC     21,19 /* Cache Tag Bank1 ECC */
#define NFP_XPB_MU_CACHE_TAG_2         21,20 /* Cache Tag Bank2 */
#define NFP_XPB_MU_CACHE_TAG_2_ECC     21,21 /* Cache Tag Bank2 ECC */
#define NFP_XPB_MU_CACHE_TAG_3         21,22 /* Cache Tag Bank3 */
#define NFP_XPB_MU_CACHE_TAG_3_ECC     21,23 /* Cache Tag Bank3 ECC */
#define NFP_XPB_MU_CACHE_TAG_4         21,24 /* Cache Tag Bank4 */
#define NFP_XPB_MU_CACHE_TAG_4_ECC     21,25 /* Cache Tag Bank4 ECC */
#define NFP_XPB_MU_CACHE_TAG_5         21,26 /* Cache Tag Bank5 */
#define NFP_XPB_MU_CACHE_TAG_5_ECC     21,27 /* Cache Tag Bank5 ECC */
#define NFP_XPB_MU_CACHE_TAG_6         21,28 /* Cache Tag Bank6 */
#define NFP_XPB_MU_CACHE_TAG_6_ECC     21,29 /* Cache Tag Bank6 ECC */
#define NFP_XPB_MU_CACHE_TAG_7         21,30 /* Cache Tag Bank7 */
#define NFP_XPB_MU_CACHE_TAG_7_ECC     21,31 /* Cache Tag Bank7 ECC */

/* QDR */
#define NFP_XPB_QDR0_CSR               22,1  /* QDR Channel-0 CSR */
#define NFP_XPB_QDR1_CSR               22,2  /* QDR Channel-1 CSR */

/* System Support */
#define NFP_XPB_GPIO_CSR               31,1  /* GPIO CSR */
#define NFP_XPB_UART_CSR               31,2  /* UART CSR */
#define NFP_XPB_TIMER_CSR              31,3  /* Timer CSR */
#define NFP_XPB_SHAC_INTMGR            31,5  /* SHaC Cluster Interrupt Manager */
#define NFP_XPB_SHAC_PERF              31,6  /* SHaC Cluster Performance Analyzer */
#define NFP_XPB_SPI_DEV0_TGT           31,8  /* SPI serial I/F Device0 24-bit Addr TGT */
#define NFP_XPB_SPI_DEV1_TGT           31,9  /* SPI serial I/F Device1 24-bit Addr TGT */
#define NFP_XPB_SPI_DEV2_TGT           31,10 /* SPI/MDIO serial I/F Device2 24-bit Addr TGT */
#define NFP_XPB_SPI_DEV3_TGT           31,11 /* SPI/MDIO serial I/F Device3 24-bit Addr TGT */
#define NFP_XPB_SPI_DEV0_CFG0          31,12 /* SPI serial I/F Device0 CSR CFG0 */
#define NFP_XPB_SPI_DEV1_CFG0          31,13 /* SPI serial I/F Device1 CSR CFG0 */
#define NFP_XPB_SPI_DEV2_CFG0          31,14 /* SPI/MDIO serial I/F Device2 CSR CFG0 */
#define NFP_XPB_SPI_DEV3_CFG0          31,15 /* SPI/MDIO serial I/F Device3 CSR CFG0 */
#define NFP_XPB_SPI_DEV0_CFG1          31,28 /* SPI serial I/F Device0 CSR CFG1 */
#define NFP_XPB_SPI_DEV1_CFG1          31,29 /* SPI serial I/F Device1 CSR CFG1 */
#define NFP_XPB_SPI_DEV2_CFG1          31,30 /* SPI/MDIO serial I/F Device2 CSR CFG1 */
#define NFP_XPB_SPI_DEV3_CFG1          31,31 /* SPI/MDIO serial I/F Device3 CSR CFG1 */
#define NFP_XPB_SPI_DEV0_CFG2          31,44 /* SPI serial I/F Device0 CSR CFG2 */
#define NFP_XPB_SPI_DEV1_CFG2          31,45 /* SPI serial I/F Device1 CSR CFG2 */
#define NFP_XPB_SPI_DEV2_CFG2          31,46 /* SPI/MDIO serial I/F Device2 CSR CFG2 */
#define NFP_XPB_SPI_DEV3_CFG2          31,47 /* SPI/MDIO serial I/F Device3 CSR CFG2 */
#define NFP_XPB_CRU_CSR                31,48 /* Clocks and Resets (CRU) CSR */
#define NFP_XPB_FUSE_CSR               31,49 /* Fuse CSR */
#define NFP_XPB_THERMAL_CSR            31,50 /* Thermal Sensor CSR */

#endif /* __NFP_SDK__NFP3200_NFP_32XX_H__ */
