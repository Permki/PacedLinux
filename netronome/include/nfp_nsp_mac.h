/*
 * Copyright (C) 2015 Netronome Systems, Inc. All rights reserved.
 * Author: Jason McMullan <jason.mcmullan@netronome.com>
 *
 */

#ifndef MAC_H
#define MAC_H

#include <nfp.h>
#include <nfp_cpp.h>

#define PLATFORM_ASCIIZ "\000"

struct platform {
	const char *hwinfo;
	const uint32_t **nbi0;
	const uint32_t **nbi1;
	const char *firmware;
};

extern int network_initialized;
extern int eth_present;
extern int rxadapt_forced;

extern struct nfp_nbi_phy_config mac_cfg_phy;
extern struct nfp_nbi_mac_config mac_cfg_nbi0;
extern struct nfp_nbi_mac_config mac_cfg_nbi1;
extern const uint32_t *port_cfg_nbi0;
extern const uint32_t *port_cfg_nbi1;

extern uint64_t eth_link_mask;
extern uint64_t eth_activity_mask;

int nfp_nsp_phy_init(struct nfp_device *nfp, int nbi_unit);
int nfp_nsp_init_mac(struct nfp_device *nfp, int allow_default, uint32_t *ovrides);
int nfp_nsp_mac_init(struct nfp_device *nfp, int nbi_unit, uint32_t *ovrides);

int nfp_mac_enabled(struct nfp_device *nfp);

int eth_rescan(struct nfp_device *nfp, struct nfp_cpp_area *table, int *entries);
int eth_control(struct nfp_device *nfp, struct nfp_cpp_area *table, int entries);
int eth_rescan_entry(struct nfp_device *nfp, struct nfp_cpp_area *table, int entries);
int eth_control_entry(struct nfp_device *nfp, struct nfp_cpp_area *table, int entry);

#endif /* MAC_H */
