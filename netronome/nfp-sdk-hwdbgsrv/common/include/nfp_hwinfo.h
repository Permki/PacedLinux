/*
 * Copyright (C) 2014 Netronome Systems, Inc. All rights reserved.
 * Author: Jason McMullan <jason.mcmullan@netronome.com>
 *
 */

#ifndef NFP_HWINFO_H
#define NFP_HWINFO_H

#include <nfp-common/nfp_platform.h>

#ifdef __KERNEL__
#define NO_NFP_API_EXPORT
#endif

/* API is part of libnfp */
#include "_nfp_apiex.h"

__BEGIN_DECLS;

/**
 * Get number of hwinfo entries for NFP device.
 *
 * @param dev           NFP device
 *
 * @return number of hwinfo enries, or 0 if no entries.
 */
NFP_API
int nfp_hwinfo_entries(struct nfp_device *dev);

/**
 * Get a hwinfo (key, value) tuple for NFP device.
 *
 * @param dev           NFP device
 * @param idx           hwinfo table index
 * @param key           key output param (if valid index)
 * @param value         value output param (if valid index)
 *
 * @return 0 on success, or -1 on failure (and sets errno accordingly).
 */
NFP_API
int nfp_hwinfo_entry(struct nfp_device *dev,
		     unsigned int idx,
		     const char **key,
		     const char **value);

/**
 * Lookup hwinfo value using key.
 *
 * @param dev           NFP device
 * @param key           hwinfo key
 *
 * @return value string for key, or NULL on lookup failure (and set
 * errno accordingly).
 */
NFP_API
const char *nfp_hwinfo_lookup(struct nfp_device *dev, const char *key);

/**
 * Set hwinfo value for a key (debugging)
 *
 * @param dev           NFP device
 * @param key           hwinfo key
 * @param value         hwinfo value
 *
 * @return 0, or -1 on failure (and sets errno accordingly).
 *
 * Note that this only modifies the hwinfo table cache for this NFP device
 * handle - it does NOT modify the on-NFP hwinfo table.
 */
NFP_API
int nfp_hwinfo_alter(struct nfp_device *dev, const char *key, const char *value);

/**
 * Get a copy of the the hwinfo block (debugging)
 *
 * @param dev           NFP device
 *
 * @return copy of the hwinfo block, or NULL on failure (and sets errno accordingly).
 *
 * Allocates a copy of the hwinfo binary block, for use with the NFP_HWINFO_* macros
 * from <nfp-bsp/nfp_hwinfo.h>. Free with 'free(3)' to release.
 */
NFP_API
void *nfp_hwinfo_dup(struct nfp_device *dev);

/**
 * Replace the current cached hwinfo block (debugging)
 *
 * @param dev           NFP device
 * @param hwinfo        hwinfo block (or NULL for the NFP block)
 *
 * @return 0, or -1 on failure (and sets errno accordingly).
 *
 * Replaces the current cached hwinfo block in the NFP Device handle.
 * See <nfp-bsp/nfp_hwinfo.h> for useful macros for constructing a HWINFO block.
 *
 * Use a NULL hwinfo block to reload the default NFP block.
 */
NFP_API
int nfp_hwinfo_reload(struct nfp_device *dev, const void *hwinfo);

/**
 * Create and set hwinfo value for a new key
 *
 * @param dev           NFP device
 * @param key           hwinfo key
 * @param val           hwinfo value
 *
 * @return 0, or -1 on failure (and sets errno accordingly).
 *
 * Note that this modifies the on-NFP hwinfo table, but does not allow
 * alteration of existing keys.
 */
NFP_API
int nfp_hwinfo_createkey(struct nfp_device *dev, const char *key, const char *val);

/**
 * Get number of user info entries for NFP device.
 *
 * @param dev           NFP device
 *
 * @return number of info enries, or 0 if no entries.
 */
NFP_API
int nfp_userinfo_entries(struct nfp_device *dev);

/**
 * Get a user info (key, value) tuple for NFP device.
 *
 * @param dev           NFP device
 * @param idx           info table index
 * @param key           key output param (if valid index)
 * @param value         value output param (if valid index)
 *
 * @return 0 on success, or -1 on failure (and sets errno accordingly).
 */
NFP_API
int nfp_userinfo_entry(struct nfp_device *dev,
		       unsigned int idx,
		       const char **key,
		       const char **value);

/**
 * Lookup user info value using key.
 *
 * @param dev           NFP device
 * @param key           info key
 *
 * @return value string for key, or NULL on lookup failure (and set
 * errno accordingly).
 */
NFP_API
const char *nfp_userinfo_get(struct nfp_device *dev, const char *key);

/**
 * Retrieve any default value overridden by key.
 *
 * @param dev           NFP device
 * @param key           info key
 *
 * @return value string for key, or NULL on lookup failure (and set
 * errno accordingly).
 */
NFP_API
const char *nfp_userinfo_get_default(struct nfp_device *dev, const char *key);

/**
 * Set user hwinfo key value.
 *
 * @param dev           NFP device
 * @param key           info key
 * @param val           string value to set, NULL to clear
 *
 * @return 0 on success, or -1 on failure (and set errno accordingly).
 */
NFP_API
int nfp_userinfo_set(struct nfp_device *dev, const char *key, const char *val);

/**
 * Set list of user hwinfo key value pairs.
 *
 * @param dev           NFP device
 * @param keys          NULL terminated array of info key pointers
 * @param vals          NULL terminated array of info values
 *
 * @return 0 on success, or -1 on failure (and set errno accordingly).
 */
NFP_API
int nfp_userinfo_set_multiple(struct nfp_device *dev,
			      const char *keys[],
			      const char *vals[]);

__END_DECLS;

#endif /* NFP_HWINFO_H */
