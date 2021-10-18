/*
 * Copyright (C) 2011-2015,  Netronome Systems, Inc.  All rights reserved.
 *
 * @file          nfp_mip.h
 * @brief         Microcode Information Page (MIP) interface
 *
 */
#ifndef __NFP_MIP_H__
#define __NFP_MIP_H__

#include <nfp-common/nfp_platform.h>
#include <nfp-common/nfp_nffw_rtt.h>

#ifdef __KERNEL__
#define NO_NFP_NFFW_API_EXPORT
#endif

#include "_nfp_nffw_apiex.h"

struct nfp_device;

struct nfp_mip_entry {
	/* If the entry was recognised and endianness converted, 'host_endian'
	 * will be non-zero, otherwise it is zero and the caller needs to
	 * handle any endian conversion. Since the caller can modify the entry
	 * data, this can be done in-place after which the caller can set
	 * 'host_endian' to prevent doing so again.
	 */
	uint8_t host_endian;
	int8_t mem_type;
	uint16_t domain; /* MEID or island ID */
	uint64_t addr;   /* entry data, not entry header */
	uint32_t size;   /* Can be 0 if entry uses no data */

	uint32_t ent_type;
	uint32_t ent_ver;
	void *ent_data;
};

struct nfp_mip {
	struct nfp_device *dev;
	uint8_t fwid; /* Currently only 0 or NFFW_FWID_BASE */
	uint32_t mip_ver;
	uint32_t mip_cppid; /* cpp_tgt, NFP_CPP_ACTION_RW, 0, iid */
	uint64_t mip_addr;

	uint32_t fw_version;
	uint32_t fw_buildnum;
	uint32_t fw_buildtime;
	uint32_t fw_loadtime;
	char *fw_name;
	char *fw_toolchain;

	/* On MIP probe, all entries are traversed and details stored in this
	 * array, but the entry data is not necessarily loaded.
	 * Call nfp_mip_load_entry to load entry data if necessary. Note that if
	 * nfp_mip_entry.host_endian is true, the data was already loaded.
	 */
	size_t entry_cnt;
	size_t entry_capacity; /* size of the array, not number of entries */
	struct nfp_mip_entry *entries;
};

__BEGIN_DECLS;

/**
 * Get MIP for NFP device.
 *
 * @param dev           NFP device
 *
 * Copy MIP structure from NFP device and return it.  The returned
 * structure is handled internally by the library and should not be
 * explicitly freed by the caller.  It will be implicitly freed when
 * closing the NFP device.  Further, any subsequent call to
 * nfp_mip_probe() returning non-zero renders references to any
 * previously returned MIP structure invalid.
 *
 * If the MIP is found, the main fields of the MIP structure are
 * automatically converted to the endianness of the host CPU, as are
 * any MIP entries known to the library.  If a MIP entry is not known
 * to the library, only the 'offset_next' field of the entry structure
 * is endian converted.  The remainder of the structure is left as-is.
 * Such entries must be searched for by explicitly converting the type
 * and version to/from little-endian.
 *
 * @return MIP structure, or NULL if not found (and set errno
 * accordingly).
 */
NFP_NFFW_API
struct nfp_mip *nfp_mip(struct nfp_device *dev);

/**
 * Check if MIP has been updated.
 *
 * @param dev           NFP device
 *
 * Check if currently cached MIP has been updated on the NFP device,
 * and read potential new contents.  If a call to nfp_mip_probe()
 * returns non-zero, the old MIP structure returned by a previous call
 * to nfp_mip() is no longer guaranteed to be present and any
 * references to the old entries are invalid.
 *
 * @return 1 if MIP has been updated, 0 if no update has occurred, or
 * -1 on error (and set errno accordingly).
 */
NFP_NFFW_API
int nfp_mip_probe(struct nfp_device *dev);

/**
 * Clear the MIP and its entries.
 *
 * @param dev           NFP device
 *
 * This actually only frees and clears the MIP and entries, the next MIP
 * API call will probe the MIP again and reload it.
 */
NFP_NFFW_API
void nfp_mip_reload(struct nfp_device *dev);

/**
 * Find entry within MIP.
 *
 * @param mip           MIP structure
 * @param type          MIP entry type to locate
 *
 * @return pointer to MIP entry, or NULL if entry was not found (and
 * set errno accordingly).
 */
NFP_NFFW_API
struct nfp_mip_entry *nfp_mip_find_entry(struct nfp_mip *mip, enum nfp_mip_entry_type type);

/**
 * Load MIP entry's data if not already loaded. If the entry is a type and
 * version recognised by the current implementation, its data will be converted
 * to host endianness.
 *
 * @param mip           MIP structure
 * @param type          MIP entry
 *
 * @return 0 on success, -1 on error (and set errno accordingly).
 */
NFP_NFFW_API
int nfp_mip_load_entry(struct nfp_mip *mip, struct nfp_mip_entry *entry);

/**
 * A convenience wrapper to read a memory location using MIP-style memory
 * types.
 *
 * Ideally @p mip_mem_type and @p domain will travel as-is from the MIP
 * entry in memory to this function.
 *
 * @param dev           NFP Device
 * @param mip_mem_type  NFP_MIP_MEMTYPE_* or positive CPP target ID
 * @param domain        MEID or island ID, depends on mip_mem_type
 * @param buffer        Data buffer
 * @param length        Length
 * @param offset        Offset
 *
 */
NFP_NFFW_API
ssize_t nfp_mip_mem_read(struct nfp_device *dev,
			 int mip_mem_type,
			 int domain,
			 void *buffer,
			 unsigned long long length,
			 unsigned long long offset);

/**
 * A convenience wrapper to write a memory location using MIP-style memory
 * types.
 *
 * Ideally @p mip_mem_type and @p domain will travel as-is from the MIP
 * entry in memory to this function.
 *
 * @param dev           NFP Device
 * @param mip_mem_type  NFP_MIP_MEMTYPE_* or positive CPP target ID
 * @param domain        MEID or island ID, depends on mip_mem_type
 * @param buffer        Data buffer
 * @param length        Length
 * @param offset        Offset
 */
NFP_NFFW_API
ssize_t nfp_mip_mem_write(struct nfp_device *dev,
			  int mip_mem_type,
			  int domain,
			  const void *buffer,
			  unsigned long long length,
			  unsigned long long offset);

/**
 * A convenience wrapper to fill a memory location using MIP-style memory
 * types.
 *
 * Ideally @p mip_mem_type and @p domain will travel as-is from the MIP
 * entry in memory to this function.
 *
 * @param dev           NFP Device
 * @param mip_mem_type  NFP_MIP_MEMTYPE_* or positive CPP target ID
 * @param domain        MEID or island ID, depends on mip_mem_type
 * @param buffer        Data buffer
 * @param length        Length of buffer
 * @param offset        Offset
 * @param fill_len      Length of memory to write
 */
NFP_NFFW_API
ssize_t nfp_mip_mem_fill(struct nfp_device *dev,
			 int mip_mem_type,
			 int domain,
			 const void *buffer,
			 unsigned long long length,
			 unsigned long long offset,
			 unsigned long long fill_len);

__END_DECLS;

#endif /* !__NFP_MIP_H__ */
