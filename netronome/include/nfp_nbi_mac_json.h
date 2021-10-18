/**
 * Copyright (C) 2013-2017 Netronome Systems, Inc.  All rights reserved.
 *
 * @file nfp_nbi_mac_misc.h
 * nfp6000 MAC API functions
 *
 */

#ifndef _NFP_NBI_MAC_JSON_H_
#define _NFP_NBI_MAC_JSON_H_

#include <jansson.h>
#include "nfp_nbi_mac_config.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Load the JSON text from a file into memory
 * @ingroup nfp6000-only
 *
 * Read the JSON file into an internal JSON MAC configuration object
 * created by the JSON library.  The library allocates the space
 * required automatically.
 *
 *  @param fp File pointer for the JSON file
 *  @return Pointer to the JSON MAC configuration object.
 *  @return NULL on error
 */
NFP_NBI_API
json_t *nfp_nbi_mac_json_load(FILE *fp);

/**
 * Decode a JSON configuration object into a configuration structure.
 * @ingroup nfp6000-only
 *
 * Extract the MAC configuration parameters from JSON object created
 * by the jansson library into the MAC configuration structure.
 *
 *  @param jobj Pointer to the JSON MAC configuration object.
 *  @param config Pointer to the MAC configuration structure.
 *  @param core Core to be configured. [0, 1, 2]
 *  @return Error code
 */
NFP_NBI_API
int nfp_nbi_mac_json_decode(json_t *jobj,
			    struct nfp_nbi_mac_config *config,
			    int core);

#ifdef __cplusplus
}
#endif

#endif
