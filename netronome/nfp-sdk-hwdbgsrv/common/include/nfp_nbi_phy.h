/**
 * Copyright (C) 2013-2014 Netronome Systems, Inc.  All rights reserved.
 *
 * @file nfp_nbi_phy.h
 *
 */

#ifndef __NFP_NBI_PHY_H__
#define __NFP_NBI_PHY_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <nfp.h>
#include <nfp_cpp.h>
#include <nfp_serdes.h>

#include "nfp_nbi.h"
#include "nfp_nbi_phy_init.h"
#include "nfp_nbi_phy_config.h"


/**
 * Enable PMA Rx-Tx loopback at the Serdes receiver
 */
#define NFP_NBI_MAC_LOOP_LINEPMA1  0x10
/**
 * Enable PMA Tx-Rx loopback at the Serdes Serializer
 */
#define NFP_NBI_MAC_LOOP_SYSPMA1   0x20
/**
 * Enable PMA Tx-Rx loopback at the Serdes Tx driver
 */
#define NFP_NBI_MAC_LOOP_SYSPMA2   0x40
/**
 * Enable PMA Parallel Rx-Tx loopback at the Serdes data port
 */
#define NFP_NBI_MAC_LOOP_LINEPMA2  0x80


/**
 * Initialize one serdes (4 lanes)
 * @ingroup nfp6000-only
 *
 * Executes the sequence which writes the Tx & Rx tuning values to the
 * specified serdes.
 *
 *  @param serdes Serdes handle
 *  @param params selection parameters
 *
 *  @return zero on success. Set errno and return -1 on error.
 */
NFP_NBI_API
int nfp_nbi_phy_init_group(struct nfp_serdes *serdes,
                           struct nfp_nbi_phy_params *params);

/**
 * Initialize RX adaptation
 * @ingroup nfp6000-only
 *
 * Executes the sequence which initializes Rx adaptation on the
 * specified serdes.
 *
 *  @param serdes Serdes handle
 *  @param params selection parameters
 *
 *  @return zero on success. Set errno and return -1 on error.
 */
NFP_NBI_API
int nfp_nbi_phy_rxadapt(struct nfp_serdes *serdes,
                         struct nfp_nbi_phy_params *params);

/**
 * Start RX adaptation
 * @ingroup nfp6000-only
 *
 * Executes the sequence which initiates Rx adaptation on the
 * specified serdes lane.
 *
 *  @param serdes Serdes handle
 *  @param params selection parameters
 *
 *  @return zero on success. Set errno and return -1 on error.
 */
NFP_NBI_API
int nfp_nbi_phy_rxadapt_start(struct nfp_serdes *serdes,
                              struct nfp_nbi_phy_params *params);

/**
 * Check if RX adaptation finished
 * @ingroup nfp6000-only
 *
 * Determine if Rx Adaptation is finished
 *  @param serdes Serdes handle
 *  @param params selection parameters
 *
 *  @return zero on success. Set errno and return -1 on error.
 */
NFP_NBI_API
int nfp_nbi_phy_rxadapt_done(struct nfp_serdes *serdes,
                              struct nfp_nbi_phy_params *params);


/**
 *  Load the serdes tuning values from a file
 * @ingroup nfp6000-only
 *
 *  Tuning values are maintained in a JSON file.
 *  @param[out] phy_config PHY configuration to load into
 *  @param fp  file handle
 *
 *  @return zero on success. Set errno and return -1 on error.
 */
NFP_NBI_API
int nfp_nbi_phy_load_config(struct nfp_nbi_phy_config *phy_config, FILE *fp);

/**
 * Write PHY loopback modes for lanes specified by lanemask
 * @ingroup nfp6000-only
 *
 * Each SerDes lane supports two system side loopbacks and two line-side
 * loopbacks.
 *
 * These loopbacks may be enabled/disabled in various combinations for
 * testing.  The loopback codes are defined in nfp_nbi_phy.h and can be
 * ORed together to enable multiple loopbacks on a per-lane basis.
 *
 * @param nbi NBI device
 * @param lanemask 24-bit value - each bit selects a lane, 23-0
 * @param mode loopback mode
 * @return 0 on success. Set errno and return -1 on error.
 */
NFP_NBI_API
int nfp_nbi_phy_write_loopback_mode(struct nfp_nbi_dev *nbi, uint32_t lanemask,
                                    uint32_t mode);

/**
 * Read status of PHY loopbacks for a single lane
 * @ingroup nfp6000-only
 *
 * Each SerDes lane supports two system side loopbacks and two line-side
 * loopbacks.
 *
 * These loopbacks may be enabled/disabled in various combinations for
 * testing.  The loopback codes are defined in nfp_nbi_phy.h and can be
 * ORed together to enable multiple loopbacks on a per-lane basis.
 *
 * The return value will be the result of ORing the states of the four
 * loopbacks for the specified lane.
 *
 * @param nbi NBI device
 * @param lane integer lane number in range 0..23
 * @return loopback state on success. Set errno and return -1 on error.
 */
NFP_NBI_API
int nfp_nbi_phy_read_loopback_mode(struct nfp_nbi_dev *nbi, int lane);

#ifdef __cplusplus
}
#endif

#endif
