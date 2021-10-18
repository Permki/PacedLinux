/*
 * Copyright (C) 2011-2015,  Netronome Systems, Inc.  All rights reserved.
 *
 * @file          nfp_syms.c
 * @brief         Interface for accessing run-time symbol table
 *
 *
 * This file is compiled by BSP and SDK, with the SDK using this file from
 * the BSP directly.
 *
 */
#define NFP6000_LONGNAMES

#include <stdint.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#ifndef NFP_SDK
#include "libnffw.h"
#endif

#include <nfp_nffw.h>
#include <nfp_mip.h>
#include <nfp_cpp.h>

#include <nfp-common/nfp_resid.h>
#include <nfp-common/nfp_chipdata.h>
#include <nfp-common/nfp_cppat.h>
#include <nfp6000/nfp_imb.h>

struct nfp_rtsym_priv {
	struct nfp_mip *mip;
	struct nfp_mip_entry *mipent_rtsymtab;
	struct nfp_mip_entry *mipent_rtstrtab;

	uint32_t rtsym_cnt;
	struct nfp_rtsym *rtsyms;
};

static void nfp_rtsym_priv_des(void *data)
{
	struct nfp_rtsym_priv *priv = data;

	if (priv->rtsyms)
		free(priv->rtsyms);
}

static void *nfp_rtsym_priv_con(struct nfp_device *dev)
{
	struct nfp_rtsym_priv *priv;

	priv = nfp_device_private_alloc(dev, sizeof(*priv), nfp_rtsym_priv_des);

	return priv;
}

int __nfp_rtsymtab_probe(struct nfp_device *dev, struct nfp_rtsym_priv *priv)
{
	uint32_t model = nfp_cpp_model(nfp_device_cpp(dev));
	const char *rtstrtab = NULL;
	uint32_t rtsym_idx;
	struct nfp_rtsym *usr_rtsym;

	if (!priv->mip) {
		priv->mip = nfp_mip(dev);
		if (!priv->mip) {
			return NFP_ERRNO(ENODEV);
		}
	}

	priv->mipent_rtsymtab = nfp_mip_find_entry(priv->mip,
						   NFP_MIP_TYPE_RTSYMTAB);
	priv->mipent_rtstrtab = nfp_mip_find_entry(priv->mip,
						   NFP_MIP_TYPE_RTSTRTAB);

	if ((!priv->mipent_rtsymtab) || (!priv->mipent_rtstrtab) ||
	    (!priv->mipent_rtsymtab->size) || (!priv->mipent_rtstrtab->size)) {
		return NFP_ERRNO(ENXIO);
	}

	if ((nfp_mip_load_entry(priv->mip, priv->mipent_rtsymtab)) ||
	    (nfp_mip_load_entry(priv->mip, priv->mipent_rtstrtab)) ||
	    (!priv->mipent_rtsymtab->host_endian) ||
	    (!priv->mipent_rtstrtab->host_endian)) {
		return NFP_ERRNO(EIO);
	}

	switch (priv->mipent_rtsymtab->ent_ver) {
	case 1:{
			struct nfp_rtsymtab_v1_entry *ent =
			    (struct nfp_rtsymtab_v1_entry *)priv->
			    mipent_rtsymtab->ent_data;

			priv->rtsym_cnt =
			    priv->mipent_rtsymtab->size /
			    sizeof(struct nfp_rtsymtab_v1_entry);
			rtstrtab =
			    (const char *)priv->mipent_rtstrtab->ent_data;
			priv->rtsyms =
			    calloc(priv->rtsym_cnt, sizeof(*priv->rtsyms));
			if (!priv->rtsyms)
				return -1;
			for (rtsym_idx = 0, usr_rtsym = priv->rtsyms;
			     rtsym_idx < priv->rtsym_cnt;
			     rtsym_idx++, ent++, usr_rtsym++) {
				if (ent->name < priv->mipent_rtstrtab->size)
					usr_rtsym->name = rtstrtab + ent->name;
				else
					usr_rtsym->name =
					    "<out of strtab range>";
				usr_rtsym->addr =
				    (((uint64_t)ent->addr_hi) << 32) | ent->addr_lo;
				usr_rtsym->size =
				    (((uint64_t)ent->size_hi) << 32) | ent->size_lo;
				usr_rtsym->type = ent->type;
				usr_rtsym->domain = -1;
				switch (ent->target) {
				case NFP_RTSYM_V1_TGT_LMEM:
					usr_rtsym->target =
					    NFP_RTSYM_TARGET_LMEM;
					break;
				case NFP_RTSYM_V1_TGT_UMEM:
					usr_rtsym->target =
					    NFP_RTSYM_TARGET_USTORE;
					break;
				case NFP_RTSYM_V1_TGT_EMU_CACHE:
					usr_rtsym->target =
					    NFP_RTSYM_TARGET_EMU_CACHE;
					break;
				default:
					usr_rtsym->target = ent->target;
					break;
				}

				if (NFP_CPP_MODEL_IS_THORNHAM(model)) {
					if (ent->domain2.nfp6000_menum != 0xff)
						usr_rtsym->domain =
						    NFP6000_MEID(ent->domain1.
								 nfp6000_island,
								 ent->domain2.
								 nfp6000_menum);
					else if (ent->domain1.nfp6000_island !=
						 0xff)
						usr_rtsym->domain =
						    ent->domain1.nfp6000_island;
				}
			}
			break;
		}
	case 2:{
			struct nfp_rtsymtab_v2_entry *ent =
			    (struct nfp_rtsymtab_v2_entry *)priv->
			    mipent_rtsymtab->ent_data;

			priv->rtsym_cnt =
			    priv->mipent_rtsymtab->size /
			    sizeof(struct nfp_rtsymtab_v2_entry);
			rtstrtab =
			    (const char *)priv->mipent_rtstrtab->ent_data;
			priv->rtsyms =
			    calloc(priv->rtsym_cnt, sizeof(*priv->rtsyms));
			if (!priv->rtsyms)
				return -1;
			for (rtsym_idx = 0, usr_rtsym = priv->rtsyms;
			     rtsym_idx < priv->rtsym_cnt;
			     rtsym_idx++, ent++, usr_rtsym++) {
				if (ent->name < priv->mipent_rtstrtab->size)
					usr_rtsym->name = rtstrtab + ent->name;
				else
					usr_rtsym->name =
					    "<out of strtab range>";
				usr_rtsym->addr =
				    (((uint64_t)ent->addr_hi) << 32) | ent->addr_lo;
				usr_rtsym->size =
				    (((uint64_t)ent->size_hi) << 32) | ent->size_lo;
				usr_rtsym->type = ent->sym_type;
				usr_rtsym->target = ent->mem_type;
				usr_rtsym->domain = ent->domain;
			}
			break;
		}
	default:
		return NFP_ERRNO(ENOTSUP);
	}

	return 0;
}

int nfp_rtsym_count(struct nfp_device *dev)
{
	struct nfp_rtsym_priv *priv =
	    nfp_device_private(dev, nfp_rtsym_priv_con);
	int err;

	if (!priv->rtsyms) {
		err = __nfp_rtsymtab_probe(dev, priv);
		if (err < 0)
			return err;
	}

	return priv->rtsym_cnt;
}

const struct nfp_rtsym *nfp_rtsym_get(struct nfp_device *dev, int idx)
{
	struct nfp_rtsym_priv *priv =
	    nfp_device_private(dev, nfp_rtsym_priv_con);
	int err;

	if (!priv->rtsyms) {
		err = __nfp_rtsymtab_probe(dev, priv);
		if (err < 0)
			return NULL;
	}

	if ((uint32_t)idx >= priv->rtsym_cnt) {
		return NFP_ERRPTR(EINVAL);
	}

	return &priv->rtsyms[idx];
}

const struct nfp_rtsym *nfp_rtsym_lookup(struct nfp_device *dev,
					 const char *name)
{
	struct nfp_rtsym_priv *priv =
	    nfp_device_private(dev, nfp_rtsym_priv_con);
	int err;
	uint32_t n;

	if (!priv->rtsyms) {
		err = __nfp_rtsymtab_probe(dev, priv);
		if (err < 0)
			return NULL;
	}

	for (n = 0; n < priv->rtsym_cnt; n++) {
		if (strcmp(name, priv->rtsyms[n].name) == 0)
			return &priv->rtsyms[n];
	}

	return NFP_ERRPTR(ENOENT);
}

ssize_t nfp_rtsym_read(struct nfp_device *dev,
		       const struct nfp_rtsym *symbol,
		       void *buffer, size_t buflen, unsigned long long offset)
{
	unsigned long long addr, len;

	if (!symbol) {
		return NFP_ERRNO(EINVAL);
	}

	if (offset >= symbol->size) {
		return NFP_ERRNO(ERANGE);
	}

	addr = symbol->addr + offset;
	len = NFP_MIN(symbol->size, buflen);
	len = NFP_MIN(len, (symbol->size - offset));

	if (symbol->type == NFP_RTSYM_TYPE_OBJECT ||
	    symbol->type == NFP_RTSYM_TYPE_FUNCTION) {
		return nfp_mip_mem_read(dev, symbol->target, symbol->domain,
					buffer, len, addr);
	} else if (symbol->type == NFP_RTSYM_TYPE_ABS) {
		*(uint64_t *)buffer = addr;
		return sizeof(uint64_t);
	}

	return NFP_ERRNO(EINVAL);
}

ssize_t nfp_rtsym_write(struct nfp_device *dev,
			const struct nfp_rtsym *symbol,
			const void *buffer, size_t buflen,
			unsigned long long offset)
{
	unsigned long long addr, len;

	if (!symbol) {
		return NFP_ERRNO(EINVAL);
	}

	if (offset >= symbol->size) {
		return NFP_ERRNO(ERANGE);
	}

	addr = symbol->addr + offset;
	len = NFP_MIN(symbol->size, buflen);
	len = NFP_MIN(len, (symbol->size - offset));

	if (symbol->type == NFP_RTSYM_TYPE_OBJECT ||
	    symbol->type == NFP_RTSYM_TYPE_FUNCTION) {
		return nfp_mip_mem_write(dev, symbol->target, symbol->domain,
					 buffer, len, addr);
	} else if (symbol->type == NFP_RTSYM_TYPE_ABS) {
		/* Can not change value of absolute symbols */
		return NFP_ERRNO(EFAULT);
	}

	return NFP_ERRNO(EINVAL);
}

ssize_t nfp_rtsym_fill(struct nfp_device *dev,
		       const struct nfp_rtsym *symbol,
		       const void *buffer, size_t buf_length,
		       unsigned long long offset, unsigned long long fill_len)
{
	unsigned long long addr, len;

	if (!symbol) {
		return NFP_ERRNO(EINVAL);
	}

	if (offset >= symbol->size) {
		return NFP_ERRNO(ERANGE);
	}

	addr = symbol->addr + offset;
	len = NFP_MIN(symbol->size, fill_len);
	len = NFP_MIN(len, (symbol->size - offset));

	if (symbol->type == NFP_RTSYM_TYPE_OBJECT ||
	    symbol->type == NFP_RTSYM_TYPE_FUNCTION) {
		return nfp_mip_mem_fill(dev, symbol->target, symbol->domain,
					buffer, buf_length, addr, len);
	} else if (symbol->type == NFP_RTSYM_TYPE_ABS) {
		/* Can not change value of absolute symbols */
		return NFP_ERRNO(EFAULT);
	}

	return NFP_ERRNO(EINVAL);
}

void nfp_rtsym_reload(struct nfp_device *dev)
{
	struct nfp_rtsym_priv *priv =
	    nfp_device_private(dev, nfp_rtsym_priv_con);

	if (priv->rtsyms)
		free(priv->rtsyms);

	priv->mip = NULL;
	priv->mipent_rtsymtab = NULL;
	priv->mipent_rtstrtab = NULL;
	priv->rtsyms = NULL;
	priv->rtsym_cnt = 0;
}
