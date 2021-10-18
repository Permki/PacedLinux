/**
 * Copyright (C) 2013-2017 Netronome Systems, Inc.  All rights reserved.
 * Author: Tony Egan <tony.egan@netronome.com>
 *
 * @file nfp_nbi_mac.h
 *
 */

#ifndef __NFP_NBI_MAC_H__
#define __NFP_NBI_MAC_H__

#include <nfp.h>
#include <nfp_nbi.h>
#include <nfp_nbi_mac_json.h>
#include <nfp_nbi_mac_config.h>
#include <nfp_nbi_mac_config_jsonstubs.h>
#include <nfp_nbi_mac_config.h>
#include <nfp_nbi_mac_misc.h>
#include <nfp_nbi_mac_eth.h>
#include <nfp_nbi_mac_ilk.h>
#include <nfp_nbi_mac_init.h>
#include <nfp_nbi_mac_stats.h>
#include <nfp_serdes.h>

/*
 * Print and verify the port channel assignments and channel remapping.
 * @ingroup nfp6000-only
 *
 * @param nbi NBI device
 */
NFP_NBI_API
int nfp_nbi_mac_print_portchan(struct nfp_nbi_dev *nbi);

/**
 * Print the channel pausewater marks.
 * @ingroup nfp6000-only
 *
 * @param nbi NBI device
 */
NFP_NBI_API
int nfp_nbi_mac_print_chan_pwm(struct nfp_nbi_dev *nbi);

/*
 * Print the Ethernet port highwater marks and deltas.
 * @ingroup nfp6000-only
 *
 * @param nbi NBI device
 */
NFP_NBI_API
int nfp_nbi_mac_eth_print_porthwm(struct nfp_nbi_dev *nbi);

/*
 * Print the Interlaken port highwater marks and deltas.
 * @ingroup nfp6000-only
 *
 * @param nbi NBI device
 */
NFP_NBI_API
int nfp_nbi_mac_ilk_print_porthwm(struct nfp_nbi_dev *nbi);

#endif
