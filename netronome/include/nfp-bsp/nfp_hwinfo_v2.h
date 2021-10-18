/*
 * Copyright (C) 2009-2010, Netronome Systems, Inc.  All rights reserved.
 *
 */

#ifndef NFP_BSP_NFP_HWINFO_V2_H
#define NFP_BSP_NFP_HWINFO_V2_H

#include <stdint.h>
#include <string.h>
#include <errno.h>

#include <nfp-bsp/nfp_hwinfo.h>
#include <nfp-common/nfp_platform.h>

#ifdef _MSC_VER
#define __le32_to_cpu(X) X
#define __cpu_to_le32(X) X
#endif

/* Hardware information database */
static nfp_inline void hwinfo_v2_init(void *hwinfo_db, size_t db_len)
{
	memset(hwinfo_db, 0xff, db_len);

	NFP_HWINFO_VERSION_set(hwinfo_db, NFP_HWINFO_VERSION_2 | 1);
	NFP_HWINFO_SIZE_set(hwinfo_db, (4 + 1) * sizeof(uint32_t));
	NFP_HWINFO_V2_LIMIT_set(hwinfo_db, db_len);
	NFP_HWINFO_V2_RESERVED_set(hwinfo_db, 0);
	NFP_HWINFO_CRC32_set(hwinfo_db, 0);
}

static nfp_inline void hwinfo_v2_finish(void *hwinfo_db)
{
	uint32_t version, crc;

	/* Calculate the CRC, assuming the new version
	 */
	NFP_HWINFO_VERSION_set(&version, NFP_HWINFO_VERSION_2);
	crc = crc32_posix_add(0, &version, sizeof(version));
	crc = crc32_posix_add(crc, (char *)hwinfo_db + sizeof(version),
			      NFP_HWINFO_SIZE_in(hwinfo_db) - sizeof(uint32_t) * 2);
	crc = crc32_posix_end(crc, NFP_HWINFO_SIZE_in(hwinfo_db) - sizeof(uint32_t));
	NFP_HWINFO_CRC32_set(hwinfo_db, crc);

	/* Finally, place in the new version ID */
	NFP_HWINFO_VERSION_set(hwinfo_db, NFP_HWINFO_VERSION_2);
}

static nfp_inline int hwinfo_v2_set_at(void *hwinfo_db,
				       const char *prefix,
				       const char *key,
				       const char *value)
{
	char *kv_start = NFP_HWINFO_DATA_START(hwinfo_db);
	char *kv_end = NFP_HWINFO_DATA_END(hwinfo_db);
	char *k, *next_k, *v;
	uint32_t version;
	uint32_t matched;
	const unsigned int base_size = sizeof(uint32_t) * (4 + 1);
	int p_len;
	int rv = 0;

	p_len = (prefix && prefix[0]) ? (strlen(prefix) + 1) : 0;

	/* Mark hwinfo as modified (CRC32 invalid) */
	version = NFP_HWINFO_VERSION_in(hwinfo_db);
	if ((version & 1) == 0)
		NFP_HWINFO_VERSION_set(hwinfo_db, version | 1);

	k = kv_start;
	while (*k != 0 && k < kv_end) {
		v = k + strlen(k) + 1;
		next_k = v + strlen(v) + 1;
		if ((prefix) && (strncmp(k, prefix, p_len - 1) == 0) && (k[p_len - 1] == '.') &&
		    (strcmp(&k[p_len], key) == 0))
			matched = 1;
		else
			matched = ((!prefix) && (strcmp(k, key) == 0)) ? 1 : 0;
		if (matched) {
			memmove(k, next_k, kv_end - next_k);
			kv_end -= (next_k - k);
			next_k = k;
		}
		k = next_k;
	}
	kv_end = k;

	if (value && value[0]) {
		int k_len = strlen(key);
		int v_len = strlen(value);

		/* Make sure it'll fit */
		if (NFP_HWINFO_V2_LIMIT_in(hwinfo_db) >=
		    (base_size + (kv_end - kv_start) + p_len + k_len + 1 + v_len + 1 + 1)) {
			if (p_len) {
				memcpy(k, prefix, p_len - 1);
				k += p_len - 1;
				*(k++) = '.';
			}

			memcpy(k, key, k_len + 1);
			k += k_len + 1;
			memcpy(k, value, v_len + 1);
			k += v_len + 1;
			kv_end += k_len + 1 + p_len + v_len + 1;
			/* Zero-pad */
			*(kv_end++) = 0;
		} else {
			rv = -1;
		}
	}
	*(kv_end++) = 0;

	/* Zero-pad until word aligned */
	while ((kv_end - kv_start) & 3)
		*(kv_end++) = 0;

	if (!*kv_start)
		kv_end = kv_start;

	/* Adjust the size */
	NFP_HWINFO_SIZE_set(hwinfo_db, base_size + (kv_end - kv_start));

	/* Rebuild the key table if opened, crc32 is closed */
	if ((version & 1) == 0)
		hwinfo_v2_finish(hwinfo_db);

	return rv;
}

static nfp_inline int hwinfo_v2_set(void *hwinfo_db, const char *key, const char *value)
{
	return hwinfo_v2_set_at(hwinfo_db, NULL, key, value);
}

static nfp_inline const char *hwinfo_v2_get(const void *hwinfo_db, const char *key)
{
	char *kv_start = NFP_HWINFO_DATA_START(hwinfo_db);
	char *kv_end = NFP_HWINFO_DATA_END(hwinfo_db);
	char *k, *v;

	k = kv_start;
	while (*k != 0 && k < kv_end) {
		v = k + strlen(k) + 1;
		if (strcmp(k, key) == 0)
			return v;
		k = v + strlen(v) + 1;
	}

	return NULL;
}

static nfp_inline const char *hwinfo_v2_key_next(const void *hwinfo_db, void **ptr)
{
	const char *cp;

	if (!*ptr) {
		*ptr = NFP_HWINFO_DATA_START(hwinfo_db);
		return (*ptr >= NFP_HWINFO_DATA_END(hwinfo_db)) ? NULL : *ptr;
	}

	if (*ptr == 0 || *ptr >= NFP_HWINFO_DATA_END(hwinfo_db))
		return NULL;

	/* Skip past current attr/value */
	cp = (char *)(*ptr) + strlen(*ptr) + 1;
	cp += strlen(cp) + 1;

	*ptr = (void *)cp;

	return (*cp) ? cp : NULL;
}

static nfp_inline unsigned int hwinfo_v2_avail(const void *hwinfo_db)
{
	return NFP_HWINFO_V2_LIMIT_in(hwinfo_db) - NFP_HWINFO_SIZE_in(hwinfo_db);
}

static nfp_inline int hwinfo_v2_load_at(void *hwinfo_db, const void *def, const char *prefix)
{
	uint32_t crc;
	const char *k;

	if (!def)
		return -EFAULT;

	/* Both HWInfo v1 and HWInfo v2 can be loaded in with the same code.
	 * HWInfo v2 drops the key/value table.
	 */
	if ((NFP_HWINFO_VERSION_in(def) & 0xffffff00) != (NFP_HWINFO_VERSION_1 & 0xffffff00) &&
	    (NFP_HWINFO_VERSION_in(def) & 0xffffff00) != (NFP_HWINFO_VERSION_2 & 0xffffff00)) {
		return -EINVAL;
	}

	/* hwinfo tables greater than 64k are unsupported
	 */
	if (NFP_HWINFO_SIZE_in(def) < sizeof(uint32_t) || NFP_HWINFO_SIZE_in(def) > 0x10000)
		return -EFBIG;

	/* Verify that the table is consistent */
	crc = crc32_posix(def, NFP_HWINFO_SIZE_in(def) - sizeof(uint32_t));
	if (crc != NFP_HWINFO_CRC32_in(def))
		return -EINVAL;

	/* Only checking? Then don't try to load. */
	if (!hwinfo_db)
		return 0;

	/* Load the old keys into the new table */
	for (k = NFP_HWINFO_DATA_START(def); *k && k < (const char *)NFP_HWINFO_DATA_END(def);) {
		const char *v = k + strlen(k) + 1;

		hwinfo_v2_set_at(hwinfo_db, prefix, k, v);
		k = v + strlen(v) + 1;
	}

	return 0;
}

static nfp_inline int hwinfo_v2_load(void *hwinfo_db, const void *def)
{
	return hwinfo_v2_load_at(hwinfo_db, def, NULL);
}

#endif /* NFP_BSP_NFP_HWINFO_V2_H */
