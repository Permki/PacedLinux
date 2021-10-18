/**
 * Copyright (C) 2013-2017 Netronome Systems, Inc.  All rights reserved.
 *
 * @file nfp_nbi_phy_config.h
 *
 */

#include <stdint.h>

#ifndef __NFP_NBI_PHY_CONFIG_H__
#define __NFP_NBI_PHY_CONFIG_H__

#define NFP_NBI_PHY_LANES 4
#define NFP_NBI_PHY_GROUPS 6
#define NFP_NBI_PHY_NUM_SPEEDS 3
#define NFP_NBI_PHY_NUM_MODES 2
#define NFP_NBI_PHY_MODNAME_LEN 20
#define NFP_NBI_PHY_MAX_MODULES 10
#define NFP_NBI_PHY_NUM_RX_VALS 10
#define NFP_NBI_PHY_NUM_TX_VALS 6
#define NFP_NBI_PHY_VEND 30
#define NFP_NBI_PHY_PROD 20
#define NFP_NBI_PHY_NETROUI 0x154d
#define NFP_NBI_PHY_MAX_WINDOW_SIZE 0xf

enum NFP_NBI_PHY_BITRATE {
	PHY_1p25,
	PHY_10p3125,
	PHY_12p5
};

enum NFP_NBI_PHY_LOOPBACK_MODE {
	NONE,
	LINE,
	SYSTEM
};

struct nfp_nbi_phy_lane_config {
	char rx[NFP_NBI_PHY_NUM_RX_VALS];
	char tx[NFP_NBI_PHY_NUM_TX_VALS];
};

struct nfp_nbi_phy_platform_module_config {
	char name[NFP_NBI_PHY_MODNAME_LEN];
	char vendor[NFP_NBI_PHY_VEND];
	char product[NFP_NBI_PHY_PROD];
	unsigned int oui;
	int length;
	struct nfp_nbi_phy_lane_config lanes[NFP_NBI_PHY_LANES *
					     NFP_NBI_PHY_GROUPS * 2];
};

struct nfp_nbi_phy_config {
	int num_modules;
	struct nfp_nbi_phy_platform_module_config module[NFP_NBI_PHY_MAX_MODULES];
};

#define PHY_CODE_VERSION 1
#define AMDASIZE 16
struct nfp_nbi_phy_hdr {
	char amda[AMDASIZE];
	int bd_rev;
	int code_ver;
	int num_modules;
	uint32_t crc_d;
	uint32_t crc_h;
};

#endif
