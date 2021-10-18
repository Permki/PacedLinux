/**
 * Copyright (C) 2013-2017 Netronome Systems, Inc.  All rights reserved.
 *
 * nfp6000 MAC initialization
 *
 */
#ifndef __NFP_NBI_PHY_JSON__
#define __NFP_NBI_PHY_JSON__

#include <jansson.h>
#include "nfp_nbi_phy_config.h"

#define NFP_NBI_PHY_CONFIG_MAX_KEY 34

int nfp_nbi_phy_lane_config_jinit(json_t *jobj,
				  struct nfp_nbi_phy_lane_config *config);

int nfp_nbi_phy_config_jinit(json_t *jobj, struct nfp_nbi_phy_config *config);

json_t *nfp_nbi_phy_json_load(FILE *fp);

#endif /* __NFP_NBI_PHY_JSON__ */
