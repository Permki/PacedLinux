/*
 * Copyright (C) 2010-2015,  Netronome Systems, Inc.  All rights reserved.
 *
 * @file          nfp_mip.c
 * @brief         Interface for Microcode Information Page (MIP)
 *
 *
 * This file is compiled by BSP and SDK, with the SDK using this file from
 * the BSP directly.
 *
 */

#define NFP6000_LONGNAMES

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <errno.h>
#include <limits.h>
#include <stddef.h>
#ifdef _WIN32
#include <time.h>
#endif

#include <nfp.h>
#include <nfp_cpp.h>
#include <nfp_nffw.h>
#include <nfp_mip.h>
#include <nfp-common/nfp_resid.h>
#include <nfp-common/nfp_cppat.h>
#include <nfp-common/nfp_nffw_rtt.h>
#include <nfp6000/nfp_imb.h>

#ifndef NFP_SDK
#include "libnffw.h"
#endif

static ssize_t nfp_mip_cpp_read(struct nfp_device *dev, uint32_t cpp_id,
				unsigned long long offset, void *buffer,
				unsigned long long length);

static void __nfp_mip_reset(struct nfp_mip *mip)
{
	size_t i;

	mip->fwid = 0;
	for (i = 0; i < mip->entry_cnt; i++) {
		if (mip->entries[i].ent_data)
			free(mip->entries[i].ent_data);
	}
	mip->entry_cnt = 0;

	if (mip->fw_name)
		free(mip->fw_name);
	if (mip->fw_toolchain)
		free(mip->fw_toolchain);

	mip->fw_name = NULL;
	mip->fw_toolchain = NULL;
}

static struct nfp_mip_entry *__nfp_mip_new_entry(struct nfp_mip *mip)
{
	if (mip->entry_cnt == mip->entry_capacity) {
		size_t new_cap = mip->entry_capacity + 4;
		struct nfp_mip_entry *new_ents = realloc(mip->entries,
							 (new_cap *
							  sizeof(*new_ents)));
		if (!new_ents)
			return NULL;
		mip->entries = new_ents;
		mip->entry_capacity = new_cap;
	}

	memset(&mip->entries[mip->entry_cnt], 0, sizeof(*mip->entries));
	return &mip->entries[mip->entry_cnt++];
}

static void __nfp_mip_des(void *data)
{
	struct nfp_mip *mip = data;

	if (mip->fw_name)
		free(mip->fw_name);
	if (mip->fw_toolchain)
		free(mip->fw_toolchain);

	if (mip->entries) {
		size_t i;

		for (i = 0; i < mip->entry_cnt; i++) {
			if (mip->entries[i].ent_data)
				free(mip->entries[i].ent_data);
		}
		free(mip->entries);
	}
}

static void *__nfp_mip_con(struct nfp_device *dev)
{
	struct nfp_mip *mip =
	    nfp_device_private_alloc(dev, sizeof(struct nfp_mip),
				     __nfp_mip_des);

	if (mip) {
		mip->dev = dev;
		mip->fwid = 0;
	}
	return mip;
}

struct nfp_mip *nfp_mip(struct nfp_device *dev)
{
	struct nfp_mip *mip = nfp_device_private(dev, __nfp_mip_con);

	if (!mip)
		return NULL;

	if (mip->fwid)
		return mip;
	if (nfp_mip_probe(dev) < 0)
		return NULL;
	/* A second safety check because we can */
	return (mip->fwid) ? mip : NULL;
}

static int nfp_mip_mu_locality_lsb(struct nfp_device *dev)
{
	struct nfp_cpp *cpp = nfp_device_cpp(dev);
	uint32_t imbcppat;

	if (!cpp)
		return -1;

	imbcppat = nfp_cpp_local_imb_cppat(cpp, NFP6000_CPPTGT_MU);
	if (imbcppat == UINT32_MAX)
		return -1;

	return
	    _nfp6000_cppat_mu_locality_lsb(NFP_IMB_TgtAddressModeCfg_Mode_of
					   (imbcppat),
					   (imbcppat &
					    NFP_IMB_TgtAddressModeCfg_AddrMode)
					   ==
					   NFP_IMB_TgtAddressModeCfg_AddrMode_40_bit);
}

int __nfp_mip_location(struct nfp_device *dev, uint32_t *cpp_id, uint64_t *addr,
		       uint32_t *version, uint32_t *load_time, uint8_t *fwid)
{
	int retval;

	*cpp_id = 0;
	/* First see if we can get it from the nfp.nffw resource */
	if (nfp_nffw_info_acquire(dev) == 0) {
		*fwid = nfp_nffw_info_fwid_first(dev);
		if ((nfp_nffw_info_fw_mip(dev, *fwid, cpp_id, addr) == 0) &&
		    (*cpp_id != 0) &&
		    (NFP_CPP_ID_TARGET_of(*cpp_id) == NFP6000_CPPTGT_MU)) {
			if ((*addr >> 63) & 1) {
				int mu_lsb = nfp_mip_mu_locality_lsb(dev);
				*addr &= ~(UINT64_C(1) << 63);
				*addr &= ~(UINT64_C(0x3) << mu_lsb);
				*addr |= ((uint64_t)2 << mu_lsb);	/* Direct Access */
			}
		}
		nfp_nffw_info_release(dev);
	}

	if (*cpp_id == 0)
		goto err_probe;

	retval = nfp_mip_mem_read(dev, NFP_CPP_ID_TARGET_of(*cpp_id),
				  NFP_CPP_ID_ISLAND_of(*cpp_id), version,
				  sizeof(*version),
				  *addr + offsetof(struct nfp_mip_v0,
						   mip_version));
	if (retval != sizeof(*version))
		goto err_probe;
	*version = NFP_LETOH32(*version);

	retval = nfp_mip_mem_read(dev, NFP_CPP_ID_TARGET_of(*cpp_id),
				  NFP_CPP_ID_ISLAND_of(*cpp_id), load_time,
				  sizeof(*load_time),
				  *addr + offsetof(struct nfp_mip_v0,
						   fw_loadtime));
	if (retval != sizeof(*load_time))
		goto err_probe;
	*load_time = NFP_LETOH32(*load_time);

	switch (*version) {
	case 1:
	case 2:
		break;
	default:
		return NFP_ERRNO(EINVAL);
	}

	return 0;

err_probe:
	return NFP_ERRNO(ENODEV);
}

int nfp_mip_probe(struct nfp_device *dev)
{
	struct nfp_mip *mip = nfp_device_private(dev, __nfp_mip_con);
	uint32_t cppid, load_time, version;
	uint8_t fwid = 0;
	uint64_t addr;
	int retval;

	if (!mip)
		return -1;

	retval = __nfp_mip_location(dev,
				    &cppid, &addr, &version, &load_time, &fwid);
	if (retval != 0)
		return -1;

	if ((mip->fwid == fwid) && (mip->fw_loadtime == load_time))
		return 0;	/* No change */

	/*
	 * Once we have confirmed a MIP update we discard old MIP and read
	 * new contents.  We also discard the current symtab.
	 */

	if (mip->fwid) {
		/* Invalidate rtsym first, it may want to still look at the mip */
		nfp_rtsym_reload(dev);
		__nfp_mip_reset(mip);
	}

	switch (version) {
	case 1:{
			uint64_t ent_off = 0;
			struct nfp_mip_v1 mip_v1;
			struct nfp_mip_entry *ent;
			struct nfp_mip_v1_entry ent_v1;

			retval =
			    nfp_mip_cpp_read(dev, cppid, addr, &mip_v1,
					     sizeof(mip_v1));
			if (retval != sizeof(mip_v1)) {
				return NFP_ERRNO(EINVAL);
			}

			/* Sanity check */
			if ((NFP_LETOH32(mip_v1.signature) != NFP_MIP_SIGNATURE)
			    || (NFP_LETOH32(mip_v1.mip_version) != 1)) {
				free(mip);
				return NFP_ERRNO(EIO);
			}

			mip->mip_ver = version;
			mip->mip_cppid = cppid;
			mip->mip_addr = addr;

			mip->fw_buildnum = NFP_LETOH32(mip_v1.buildnum);
			mip->fw_buildtime = NFP_LETOH32(mip_v1.buildtime);
			mip->fw_loadtime = NFP_LETOH32(mip_v1.loadtime);
			mip->fw_version = NFP_LETOH32(mip_v1.version);
			mip->fw_name = calloc(sizeof(mip_v1.name) + 1, 1);
			mip->fw_toolchain =
			    calloc(sizeof(mip_v1.toolchain) + 1, 1);
			if (!mip->fw_name || !mip->fw_toolchain)
				return -1;
			strncpy(mip->fw_name, mip_v1.name, sizeof(mip_v1.name));
			strncpy(mip->fw_toolchain, mip_v1.toolchain,
				sizeof(mip_v1.toolchain));

			ent_off = addr + NFP_LETOH32(mip_v1.first_entry);
			retval =
			    nfp_mip_cpp_read(dev, cppid, ent_off, &ent_v1,
					     sizeof(ent_v1));
			if (retval != sizeof(ent_v1)) {
				return NFP_ERRNO(EINVAL);
			}
			while (ent_v1.type) {
				ent = __nfp_mip_new_entry(mip);
				if (!ent)
					return -1;
				ent->mem_type = NFP_CPP_ID_TARGET_of(cppid);
				ent->domain = NFP_CPP_ID_ISLAND_of(cppid);
				ent->addr = ent_off + sizeof(ent_v1);
				ent->ent_type = NFP_LETOH32(ent_v1.type);
				ent->ent_ver = NFP_LETOH32(ent_v1.version);

				switch (ent->ent_type) {
				case NFP_MIP_TYPE_QC:
					if (ent->ent_ver == 1) {
						ent->size =
						    sizeof(struct
							   nfp_mip_qc_v1);
						/* These structs actually have the header in them
						 * so move the address back a bit
						 */
						ent->addr = ent_off;
					}
					break;
				case NFP_MIP_TYPE_VPCI:
					if (ent->ent_ver == 1) {
						ent->size =
						    sizeof(struct
							   nfp_mip_vpci_v1);
						ent->addr = ent_off;
					}
					break;
				default:
					break;
				}

				retval = nfp_mip_load_entry(mip, ent);
				if (retval < 0)
					return retval;

				ent_off += NFP_LETOH32(ent_v1.offset_next);
				retval = nfp_mip_cpp_read(dev, cppid, ent_off,
							  &ent_v1,
							  sizeof(ent_v1));
				if (retval != sizeof(ent_v1)) {
					return NFP_ERRNO(EINVAL);
				}
			}

			mip_v1.symtab_addr = NFP_LETOH32(mip_v1.symtab_addr);
			mip_v1.symtab_size = NFP_LETOH32(mip_v1.symtab_size);
			mip_v1.strtab_addr = NFP_LETOH32(mip_v1.strtab_addr);
			mip_v1.strtab_size = NFP_LETOH32(mip_v1.strtab_size);

			if (mip_v1.symtab_size && mip_v1.strtab_size) {
				ent = __nfp_mip_new_entry(mip);
				if (!ent)
					return -1;
				ent->mem_type = NFP_CPP_ID_TARGET_of(cppid);
				ent->domain = NFP_CPP_ID_ISLAND_of(cppid);
				ent->addr = mip_v1.symtab_addr;
				ent->size = mip_v1.symtab_size;
				ent->ent_type = NFP_MIP_TYPE_RTSYMTAB;
				ent->ent_ver = 1;
				retval = nfp_mip_load_entry(mip, ent);
				if (retval < 0)
					return retval;

				ent = __nfp_mip_new_entry(mip);
				if (!ent)
					return -1;
				ent->mem_type = NFP_CPP_ID_TARGET_of(cppid);
				ent->domain = NFP_CPP_ID_ISLAND_of(cppid);
				ent->addr = mip_v1.strtab_addr;
				ent->size = mip_v1.strtab_size;
				ent->ent_type = NFP_MIP_TYPE_RTSTRTAB;
				ent->ent_ver = 1;
				retval = nfp_mip_load_entry(mip, ent);
				if (retval < 0)
					return retval;
			}

			break;
		}
	case 2:{
			uint64_t ent_addr;
			int8_t ent_mem_type;
			uint16_t ent_domain;
			struct nfp_mip_v2 mip_v2;
			struct nfp_mip_entry *ent;
			struct nfp_mip_v2_entry ent_v2;

			retval =
			    nfp_mip_cpp_read(dev, cppid, addr, &mip_v2,
					     sizeof(mip_v2));
			if (retval != sizeof(mip_v2)) {
				return NFP_ERRNO(EINVAL);
			}

			/* Sanity check */
			if ((NFP_LETOH32(mip_v2.signature) != NFP_MIP_SIGNATURE)
			    || (NFP_LETOH32(mip_v2.mip_version) != 2)) {
				free(mip);
				return NFP_ERRNO(EIO);
			}

			mip->mip_ver = version;
			mip->mip_cppid = cppid;
			mip->mip_addr = addr;

			mip->fw_buildnum = NFP_LETOH32(mip_v2.fw_buildnum);
			mip->fw_buildtime = NFP_LETOH32(mip_v2.fw_buildtime);
			mip->fw_loadtime = NFP_LETOH32(mip_v2.fw_loadtime);
			mip->fw_version = NFP_LETOH32(mip_v2.fw_version);
			mip->fw_name = calloc(sizeof(mip_v2.fw_name) + 1, 1);
			mip->fw_toolchain =
			    calloc(sizeof(mip_v2.fw_toolchain) + 1, 1);
			if (!mip->fw_name || !mip->fw_toolchain)
				return -1;
			strncpy(mip->fw_name, mip_v2.fw_name,
				sizeof(mip_v2.fw_name));
			strncpy(mip->fw_toolchain, mip_v2.fw_toolchain,
				sizeof(mip_v2.fw_toolchain));

			ent_mem_type = mip_v2.first_entry.mem_type;
			ent_domain = NFP_LETOH16(mip_v2.first_entry.domain);
			ent_addr = ((uint64_t)mip_v2.first_entry.addr_hi << 32) |
			    NFP_LETOH32(mip_v2.first_entry.addr_lo);

			while (ent_mem_type) {
				retval =
				    nfp_mip_mem_read(dev, ent_mem_type,
						     ent_domain, &ent_v2,
						     sizeof(ent_v2), ent_addr);
				if (retval != sizeof(ent_v2)) {
					return NFP_ERRNO(EINVAL);
				}
				if (!ent_v2.type)
					break;

				ent = __nfp_mip_new_entry(mip);
				if (!ent)
					return -1;
				ent->mem_type = ent_mem_type;
				ent->domain = ent_domain;
				ent->addr = ent_addr + sizeof(ent_v2);

				ent->size = NFP_LETOH32(ent_v2.size);
				ent->ent_type = NFP_LETOH32(ent_v2.type);
				ent->ent_ver = NFP_LETOH32(ent_v2.version);

				switch (ent->ent_type) {
				case NFP_MIP_TYPE_QC:
				case NFP_MIP_TYPE_VPCI:
				case NFP_MIP_TYPE_RTSYMTAB:
				case NFP_MIP_TYPE_RTSTRTAB:
					retval = nfp_mip_load_entry(mip, ent);
					break;
				default:
					/* For unknown entries, only load based on arbitrarily chosen
					 * size limit.
					 */
					if (ent->size <= 4096)
						retval =
						    nfp_mip_load_entry(mip,
								       ent);
					break;
				}
				if (retval < 0)
					return retval;

				ent_mem_type = ent_v2.next_entry.mem_type;
				ent_domain =
				    NFP_LETOH16(ent_v2.next_entry.domain);
				ent_addr =
				    ((uint64_t)ent_v2.next_entry.
				     addr_hi << 32) | NFP_LETOH32(ent_v2.
								  next_entry.
								  addr_lo);
			}

			break;
		}
	default:
		return -1;
	}

	mip->fwid = fwid;
	return 1;
}

struct nfp_mip_entry *nfp_mip_find_entry(struct nfp_mip *mip,
					 enum nfp_mip_entry_type type)
{
	struct nfp_mip_entry *ent = mip->entries;
	size_t idx;

	for (idx = 0; idx < mip->entry_cnt; idx++, ent++) {
		if (ent->ent_type == type)
			return ent;
	}

	return NULL;
}

int nfp_mip_load_entry(struct nfp_mip *mip, struct nfp_mip_entry *entry)
{
	if (!entry->size)	/* Has no data */
		return 0;

	if (entry->ent_data)	/* Already loaded */
		return 0;

	entry->ent_data = malloc(entry->size);
	if (!entry->ent_data)
		return -1;	/* ENOMEM */

	if (nfp_mip_mem_read(mip->dev, entry->mem_type, entry->domain,
			     entry->ent_data, entry->size,
			     entry->addr) != (ssize_t)entry->size)
		goto error_cleanup;

	switch (entry->ent_type) {
	case NFP_MIP_TYPE_QC:
		if (entry->ent_ver == 1) {
			struct nfp_mip_qc_v1 *qc =
			    (struct nfp_mip_qc_v1 *)entry->ent_data;
			qc->type_config = NFP_LETOH32(qc->type_config);
			qc->type_config_size =
			    NFP_LETOH32(qc->type_config_size);
			qc->host_config = NFP_LETOH32(qc->host_config);
			qc->host_config_size =
			    NFP_LETOH32(qc->host_config_size);
			qc->config_signal = NFP_LETOH32(qc->config_signal);
			qc->nfp_queue_size = NFP_LETOH32(qc->nfp_queue_size);
			qc->queue_base = NFP_LETOH32(qc->queue_base);
			qc->sequence_base = NFP_LETOH32(qc->sequence_base);
			qc->sequence_type = NFP_LETOH32(qc->sequence_type);
			qc->status_base = NFP_LETOH32(qc->status_base);
			qc->status_version = NFP_LETOH32(qc->status_version);
			qc->error_base = NFP_LETOH32(qc->error_base);
			entry->host_endian = 1;
		}
		break;
	case NFP_MIP_TYPE_VPCI:
		if (entry->ent_ver == 1) {
			struct nfp_mip_vpci_v1 *vpci =
			    (struct nfp_mip_vpci_v1 *)entry->ent_data;
			vpci->vpci_epconfig = NFP_LETOH32(vpci->vpci_epconfig);
			vpci->vpci_epconfig_size =
			    NFP_LETOH32(vpci->vpci_epconfig_size);
			entry->host_endian = 1;
		}
		break;
	case NFP_MIP_TYPE_RTSYMTAB:
		switch (entry->ent_ver) {
		case 1:{
				uint32_t rtsym_cnt =
				    entry->size /
				    sizeof(struct nfp_rtsymtab_v1_entry);
				struct nfp_rtsymtab_v1_entry *rtsym =
				    (struct nfp_rtsymtab_v1_entry *)entry->
				    ent_data;

				if (entry->size %
				    sizeof(struct nfp_rtsymtab_v1_entry)) {
					errno = ENXIO;
					goto error_cleanup;
				}

				for (; rtsym_cnt > 0; rtsym_cnt--, rtsym++) {
					rtsym->addr_lo =
					    NFP_LETOH32(rtsym->addr_lo);
					rtsym->name = NFP_LETOH16(rtsym->name);
					rtsym->size_lo =
					    NFP_LETOH32(rtsym->size_lo);
				}
				entry->host_endian = 1;
				break;
			}
		case 2:{
				uint32_t rtsym_cnt =
				    entry->size /
				    sizeof(struct nfp_rtsymtab_v2_entry);
				struct nfp_rtsymtab_v2_entry *rtsym =
				    (struct nfp_rtsymtab_v2_entry *)entry->
				    ent_data;

				if (entry->size %
				    sizeof(struct nfp_rtsymtab_v2_entry)) {
					errno = ENXIO;
					goto error_cleanup;
				}

				for (; rtsym_cnt > 0; rtsym_cnt--, rtsym++) {
					rtsym->domain =
					    NFP_LETOH16(rtsym->domain);
					rtsym->addr_lo =
					    NFP_LETOH32(rtsym->addr_lo);
					rtsym->size_lo =
					    NFP_LETOH32(rtsym->size_lo);
					rtsym->name = NFP_LETOH32(rtsym->name);
				}
				entry->host_endian = 1;
				break;
			}
		default:
			break;
		}
		break;
	case NFP_MIP_TYPE_RTSTRTAB:
		entry->host_endian = 1;
		break;
	default:
		break;
	}

	return 0;
error_cleanup:
	if (entry->ent_data)
		free(entry->ent_data);
	return -1;
}

ssize_t nfp_mip_cpp_read(struct nfp_device *dev, uint32_t cpp_id,
			 unsigned long long offset, void *buffer,
			 unsigned long long length)
{
	return nfp_mip_mem_read(dev, NFP_CPP_ID_TARGET_of(cpp_id),
				NFP_CPP_ID_ISLAND_of(cpp_id), buffer, length,
				offset);
}

ssize_t nfp_mip_mem_read(struct nfp_device *dev,
			 int mip_mem_type, int domain, void *buffer,
			 unsigned long long length, unsigned long long offset)
{
	struct nfp_cpp *cpp = nfp_device_cpp(dev);
	int i;
	uint32_t cpp_act = NFP_CPP_ACTION_RW;
	uint32_t cpp_tok = 0;

	if (length == 0)
		return 0;

	if (mip_mem_type == NFP_MIP_MEMTYPE_LMEM)
		return nfp_lmem_read(dev, domain, buffer, length, offset);

	if (mip_mem_type == NFP_MIP_MEMTYPE_EMU_CACHE) {
		i = nfp_mip_mu_locality_lsb(dev);
		offset &= ~(0x3ULL << i);
		offset |= (0x2ULL << i);	/* Direct Access */
		mip_mem_type = NFP6000_CPPTGT_MU;
	}

	if (mip_mem_type <= 0) {
		return NFP_ERRNO(EINVAL);
	}

	if (mip_mem_type == NFP6000_CPPTGT_MU) {
		cpp_act = 28;
		cpp_tok = 0;	/* read32_be */
	}

	return nfp_cpp_read(cpp, NFP_CPP_ISLAND_ID(mip_mem_type,
						   cpp_act, cpp_tok, domain),
			    offset, buffer, (size_t)length);
}

ssize_t nfp_mip_mem_write(struct nfp_device *dev,
			  int mip_mem_type, int domain, const void *buffer,
			  unsigned long long length, unsigned long long offset)
{
	struct nfp_cpp *cpp = nfp_device_cpp(dev);
	int i;
	uint32_t cpp_act = NFP_CPP_ACTION_RW;
	uint32_t cpp_tok = 0;

	if (length == 0)
		return 0;

	if (mip_mem_type == NFP_MIP_MEMTYPE_LMEM)
		return nfp_lmem_write(dev, domain, buffer, length, offset);

	if (mip_mem_type == NFP_MIP_MEMTYPE_EMU_CACHE) {
		i = nfp_mip_mu_locality_lsb(dev);
		offset &= ~(0x3ULL << i);
		offset |= (0x2ULL << i);	/* Direct Access */
		mip_mem_type = NFP6000_CPPTGT_MU;
	}

	if (mip_mem_type <= 0) {
		return NFP_ERRNO(EINVAL);
	}

	/* We need to access in 32-bit word sizes */
	if (mip_mem_type == NFP6000_CPPTGT_MU) {
		cpp_act = 31;
		cpp_tok = 0;	/* write32_be */
	}

	return nfp_cpp_write(cpp, NFP_CPP_ISLAND_ID(mip_mem_type,
						    cpp_act, cpp_tok, domain),
			     offset, buffer, (size_t)length);
}

ssize_t nfp_mip_mem_fill(struct nfp_device *dev,
			 int mip_mem_type, int domain, const void *buffer,
			 unsigned long long length,
			 unsigned long long offset, unsigned long long fill_len)
{
	unsigned long long total = 0;
	unsigned long long buf_offset = 0;
	unsigned long long len;
	ssize_t ret;

	while (total < fill_len) {
		len = length - buf_offset;
		if (len > (size_t) -64)	/* As much as possible, but still aligned. */
			len = (size_t) -64;	/* 16-byte alignment for good measure */

		if (len > (fill_len - total))
			len = (fill_len - total);

		ret = nfp_mip_mem_write(dev, mip_mem_type, domain,
					(uint8_t *)buffer + buf_offset,
					(size_t)len, offset);
		if (ret < 0)
			return ret;

		len = ret;
		buf_offset += len;
		if (buf_offset == length)
			buf_offset = 0;
		total += len;
		offset += len;
	}
	return (ssize_t)total;
}

void nfp_mip_reload(struct nfp_device *dev)
{
	struct nfp_mip *mip = nfp_device_private(dev, __nfp_mip_con);

	if (mip) {
		/* Invalidate rtsym first, it may want to still look at the mip */
		nfp_rtsym_reload(dev);
		__nfp_mip_reset(mip);
	}
}
