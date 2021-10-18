/*
 * Copyright (C) 2014,2017 Netronome Systems, Inc.
 * All right reserved.
 *
 * Device Tree Node functions, based on the Linux Flattened Device Tree
 * specification.
 */

#ifndef NFP_DTN_H
#define NFP_DTN_H

struct nfp_dtn;

struct nfp_dtn *nfp_dtn_unflatten(const void *dtb, size_t dtb_size);

int nfp_dtn_reserve(struct nfp_dtn *root, uint64_t addr, uint64_t size);

void *nfp_dtn_flatten(struct nfp_dtn *root, uint64_t base, size_t *dtb_size);

struct nfp_dtn *nfp_dtn_find(struct nfp_dtn *parent, const char *path);

struct nfp_dtn *nfp_dtn_create(struct nfp_dtn *parent, const char *path);

void nfp_dtn_free(struct nfp_dtn *node);

struct nfp_dtn *nfp_dtn_child_next(struct nfp_dtn *node, void **tmp);

int nfp_dtn_set_prop_value(struct nfp_dtn *node,
			   const char *prop_name,
			   size_t size,
			   const void *data);

int nfp_dtn_set_prop(struct nfp_dtn *node, const char *prop_name);

int nfp_dtn_set_prop_u32(struct nfp_dtn *node,
			 const char *prop_name,
			 int values,
			 uint32_t value_0,
			 ...);

int nfp_dtn_set_prop_str(struct nfp_dtn *node,
			 const char *prop_name,
			 int values,
			 const char *value_0,
			 ...);

int nfp_dtn_get_prop_cells(struct nfp_dtn *node,
			   const char *prop_name,
			   const void **value);

int nfp_dtn_get_prop_u32(struct nfp_dtn *node,
			 const char *prop_name,
			 int cell,
			 uint32_t *value);

int nfp_dtn_get_prop_str(struct nfp_dtn *node,
			 const char *prop_name,
			 int cell,
			 const char **value);

#endif /* NFP_DTN_H */
