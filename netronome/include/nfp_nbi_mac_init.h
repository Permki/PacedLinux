/**
 * Copyright (C) 2013-2017 Netronome Systems, Inc.  All rights reserved.
 *
 * @file nfp_nbi_mac_init.h
 * nfp6000 MAC initialization functions
 *
 */
#ifndef __NFP_NBI_MAC_INIT_H__
#define __NFP_NBI_MAC_INIT_H__

#include "nfp_nbi_mac_config.h"
#include "nfp_nbi_mac_misc.h"
#include "nfp_nbi_phy_config.h"
#include "nfp_nbi_phymod.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Initialize the MAC.
 * @ingroup nfp6000-only
 *
 * This function uses the configuration information in
 * <code>mcfg</code> to initialize the MAC specified by
 * <code>nbi</code> and <code>corepair</code>.  <code>Corepair</code>
 * can be 0 or 1 indicating core-pair 0 or 1 respectively or it can be
 * 2 indicating both core-pairs.  If <code>hold</code> == 0 this
 * function will poll and wait for link up status before returning.
 * If <code>hold</code> == 1, this function will leave the MAC in a
 * reset state and return without waiting for link up.  This allows
 * further configuration of the MAC.  In this case the function
 * <code>init_release()</code> must be called to release the resets
 * and complete initialization.
 *
 * @param nbi NBI device handle
 * @param mcfg Pointer to the MAC configuration structure.
 * @param phy_config Pointer to the PHY configuration structure.
 * @param corepair MAC core-pair: [0,1,2]
 * @param hold hold=0 wait for link up status before returning.
 *        hold=1 return with resets held to allow further configuration.
 *        hold=2 return with resets released. Link state should be checked
 *        later.
 * @return 0 on success
 * @return -1 on failure
 */
NFP_NBI_API
int nfp_nbi_mac_init(struct nfp_nbi_dev *nbi,
		     struct nfp_nbi_mac_config *mcfg,
		     const struct nfp_nbi_phy_config *phy_config,
		     int corepair,
		     int hold);

NFP_NBI_API
int nfp_nbi_mac_phy_minimal_init(struct nfp_nbi_dev *nbi,
				 struct nfp_nbi_mac_config *mcfg,
				 const struct nfp_nbi_phy_config *phy_config);

NFP_NBI_API
int nfp_nbi_mac_reset_phymod(struct nfp_nbi_dev *nbi,
			     struct nfp_nbi_mac_config *mcfg,
			     const struct nfp_nbi_phy_config *phy_config,
			     struct nfp_phymod_eth *eth);

NFP_NBI_API
int nfp_nbi_mac_rxadapt(struct nfp_nbi_dev *nbi, uint32_t serdes, int lane_mode);

NFP_NBI_API
int nfp_nbi_mac_ext_phy5113_platform(struct nfp_nbi_dev *nbi);

NFP_NBI_API
int nfp_nbi_mac_ext_phy5113_mode(struct nfp_nbi_dev *nbi, int port, int lp, int first);

NFP_NBI_API
int nfp_nbi_mac_ext_phy5113_post_init(struct nfp_nbi_dev *nbi);

NFP_NBI_API
int nfp_nbi_mac_rxadapt_check(struct nfp_nbi_dev *nbi, struct nfp_phymod_eth *eth);

NFP_NBI_API
int nfp_nbi_mac_use_static(struct nfp_nbi_dev *nbi, struct nfp_phymod_eth *eth);

NFP_NBI_API
int nfp_nbi_mac_rxctl(struct nfp_nbi_dev *nbi, int port, int state);

#ifdef __cplusplus
}
#endif

#endif
