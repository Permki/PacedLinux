/**
 * Copyright (C) 2018 Netronome Systems, Inc.  All rights reserved.
 */

#ifndef __NFP_NBI_MAC_3800_MISC_H__
#define __NFP_NBI_MAC_3800_MISC_H__

#define NFP3800_LONGNAMES
#include <nfp3800/nfp_nbi_mac.h>

#define NFP3800_MAC_PCS8_PCS2(r0, i) ((r0) | ((i) % 2) << ((i) % 2) * 7 | (1 * ((i) / 2) + 4 * ((i) / 8)) << 8)

#define NFP3800_NBI_MACX_ETH_MacEthSgmii_EthLpcsSgmiiPCS_EthSgmiiIfMode(i) \
	NFP3800_MAC_PCS8_PCS2(NFP3800_NBI_MACX_ETH_MacEth8ChSgmii_EthLpcs8chSgmiiPCS0_EthSgmiiIfMode, (i))
#define NFP3800_NBI_MACX_ETH_MacEthSgmii_EthLpcsSgmiiPCS_EthSgmiiLinkTimerLo(i) \
	NFP3800_MAC_PCS8_PCS2(NFP3800_NBI_MACX_ETH_MacEth8ChSgmii_EthLpcs8chSgmiiPCS0_EthSgmiiLinkTimerLo, (i))
#define NFP3800_NBI_MACX_ETH_MacEthSgmii_EthLpcsSgmiiPCS_EthSgmiiLinkTimerHi(i) \
	NFP3800_MAC_PCS8_PCS2(NFP3800_NBI_MACX_ETH_MacEth8ChSgmii_EthLpcs8chSgmiiPCS0_EthSgmiiLinkTimerHi, (i))
#define NFP3800_NBI_MACX_ETH_MacEthSgmii_EthLpcsSgmiiPCS_EthSgmiiDevAbility(i) \
	NFP3800_MAC_PCS8_PCS2(NFP3800_NBI_MACX_ETH_MacEth8ChSgmii_EthLpcs8chSgmiiPCS0_EthSgmiiDevAbility, (i))
#define NFP3800_NBI_MACX_ETH_MacEthSgmii_EthLpcsSgmiiPCS_EthSgmiiPcsCtl(i) \
	NFP3800_MAC_PCS8_PCS2(NFP3800_NBI_MACX_ETH_MacEth8ChSgmii_EthLpcs8chSgmiiPCS0_EthSgmiiPcsCtl, (i))
#define NFP3800_NBI_MACX_ETH_MacEthSgmii_EthLpcsSgmiiPCS0_EthSgmiiPcsStatus(i) \
	NFP3800_MAC_PCS8_PCS2(NFP3800_NBI_MACX_ETH_MacEth8ChSgmii_EthLpcs8chSgmiiPCS0_EthSgmiiPcsStatus, (i))

#define NFP3800_NBI_MACX_ETH_MacEthSgmii_EthLpcsSgmiiGMODE(i) \
	((i) < 8 ? NFP3800_NBI_MACX_ETH_MacEth8ChSgmii_EthLpcs8chSgmiiGMODE : \
	 NFP3800_NBI_MACX_ETH_MacEth2ChSgmii_EthLpcs2ChSgmiiGMODE)

#define NFP3800_NBI_MACX_ETH_MacEth8ChSgmii_EthLpcs8chSgmiiPCS0_EthSgmiiIfMode_EthSgmiiSpeed_10M (0)
#define NFP3800_NBI_MACX_ETH_MacEth8ChSgmii_EthLpcs8chSgmiiPCS0_EthSgmiiIfMode_EthSgmiiSpeed_100M (1)
#define NFP3800_NBI_MACX_ETH_MacEth8ChSgmii_EthLpcs8chSgmiiPCS0_EthSgmiiIfMode_EthSgmiiSpeed_1G (2)

/* nfp6000 error */
#define NFP3800_NBI_MAC_MacEthPortStatsHyd_MacEthPortStatsSeg_TxPStatsPkts1518toMAXoctets \
	NFP3800_NBI_MAC_MacEthPortStatsHyd_MacEthPortStatsSeg_TxPStatsPkts1519toMAXoctets

#endif
