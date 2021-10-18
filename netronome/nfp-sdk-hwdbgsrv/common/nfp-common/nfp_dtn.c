/*
 * Copyright (C) 2014, Netronome Systems, Inc.
 * All right reserved.
 *
 */

#include <errno.h>
#include <string.h>
#include <malloc.h>
#include <stdint.h>
#include <stdarg.h>

#include <nfp-common/nfp_platform.h>
#include <nfp-common/nfp_dtn.h>

#define OF_DT_HEADER            0xd00dfeed
#define OF_DT_BEGIN_NODE        1
#define OF_DT_END_NODE          2
#define OF_DT_PROP              3
#define OF_DT_END               9

struct fdt_header {
	uint32_t magic;
	uint32_t totalsize;
	uint32_t off_dt_struct;
	uint32_t off_dt_strings;
	uint32_t off_mem_resvmap;
	uint32_t version;
	uint32_t last_comp_version;
	uint32_t boot_cpuid_phys;	/* version 2+ */
	uint32_t size_dt_strings;	/* version 3+ */
	uint32_t size_dt_struct;	/* version 17+ */
};

struct of_resvmap {
	int entries;
	struct of_resvmap_entry {
		uint64_t addr;
		uint64_t size;
	} *entry;
};

struct nfp_dtn {
	char *name;
	unsigned int props;
	struct nfp_dtn_prop {
		char *name;
		uint32_t size;
		char *data;
	} **prop;
	int children;
	struct nfp_dtn **child;
	struct nfp_dtn *root;
	/* Only valid for the root node.. */
	struct of_resvmap *resvmap;
};

static const void *_dtn_prop_unflatten(struct nfp_dtn *node, uint32_t version, const void *dtb, const char *ds)
{
	uint32_t size;
	uint32_t name_offset;
	int err;
	const char *dt = dtb;

	if (NFP_BETOH32(*(const uint32_t *)dt) != OF_DT_PROP)
		return NFP_ERRPTR(EINVAL);

	dt += sizeof(uint32_t);

	size = NFP_BETOH32(*(const uint32_t *)dt);
	dt += sizeof(uint32_t);

	name_offset = NFP_BETOH32(*(const uint32_t *)dt);
	dt += sizeof(uint32_t);

	err = nfp_dtn_set_prop_value(node, &ds[name_offset], size, dt);
	if (err < 0)
		return NULL;

	dt += (size + 3) & ~3;

	return dt;
}

int nfp_dtn_reserve(struct nfp_dtn *root, uint64_t addr, uint64_t size)
{
	struct of_resvmap *rm = root->root->resvmap;
	struct of_resvmap_entry *etmp;

	if (!rm)
		return NFP_ERRNO(EINVAL);

	etmp = realloc(rm->entry, sizeof(rm->entry[0]) * (rm->entries + 1));
	if (!etmp)
		return -1;

	rm->entry = etmp;
	rm->entry[rm->entries].addr = addr;
	rm->entry[rm->entries].size = size;
	rm->entries++;

	return 0;
}

static const void *_dtn_unflatten(struct nfp_dtn *parent, uint32_t version, const void *dtb, const char *ds)
{
	int done;
	struct nfp_dtn *node;
	const char *dt = dtb;

	if (NFP_BETOH32(*(const uint32_t *)dt) != OF_DT_BEGIN_NODE)
		return NFP_ERRPTR(EINVAL);

	dt += sizeof(uint32_t);

	node = nfp_dtn_create(version < 16 ? parent->root : parent, dt);
	if (!node)
		return NULL;

	dt += (strlen(dt) + 1 + 3) & ~3;

	for (done = 0; !done;) {
		switch (NFP_BETOH32(*(const uint32_t *)dt)) {
		case OF_DT_BEGIN_NODE:
			dt = _dtn_unflatten(node, version, dt, ds);
			break;
		case OF_DT_PROP:
			dt = _dtn_prop_unflatten(node, version, dt, ds);
			break;
		case OF_DT_END_NODE:
			dt += sizeof(uint32_t);
			done = 1;
			break;
		default:
			dt = NULL;
			done = 1;
			break;
		}
	}

	return dt;
}

struct nfp_dtn *nfp_dtn_unflatten(const void *dtb, size_t dtb_size)
{
	struct nfp_dtn *root;
	const struct fdt_header *fdt;
	uint32_t version;
	const char *ds;
	const char *dt;

	fdt = dtb;
	if (NFP_BETOH32(fdt->magic) != OF_DT_HEADER)
		return NFP_ERRPTR(ENOENT);

	if (NFP_BETOH32(fdt->totalsize) > dtb_size)
		return NFP_ERRPTR(EFBIG);

	version = NFP_BETOH32(fdt->version);
	dt = (const char *)dtb + NFP_BETOH32(fdt->off_dt_struct);
	ds = (const char *)dtb + NFP_BETOH32(fdt->off_dt_strings);

	root = nfp_dtn_create(NULL, NULL);
	dt = _dtn_unflatten(root, version, dt, ds);
	if (dt) {
		/* Verify the trailer */
		if (NFP_BETOH32(*(uint32_t *)dt) != OF_DT_END) {
			errno = EINVAL;
			dt = NULL;
		}
	}
	if (!dt) {
		nfp_dtn_free(root);
		return NULL;
	}

	return root;
}

struct of_strtab {
	int offsets;
	uint32_t *offset;
	size_t bytes;
	char *byte;
};

static int _dtn_strtab_insert(struct of_strtab *strtab, const char *str)
{
	int i;
	char *ctmp;
	uint32_t *otmp;

	for (i = 0; i < strtab->offsets; i++) {
		if (strcmp(&strtab->byte[strtab->offset[i]], str) == 0)
			return strtab->offset[i];
	}

	otmp = realloc(strtab->offset, (strtab->offsets + 1) * sizeof(strtab->offset[0]));
	if (!otmp)
		return -1;
	strtab->offset = otmp;

	ctmp = realloc(strtab->byte, strtab->bytes + strlen(str) + 1);
	if (!ctmp)
		return -1;
	strtab->byte = ctmp;

	strcpy(strtab->byte + strtab->bytes, str);
	strtab->offset[strtab->offsets] = strtab->bytes;
	strtab->bytes += strlen(str) + 1;

	return strtab->offsets++;
}

static size_t _dtn_flatten(struct nfp_dtn *node, struct of_strtab *strtab, void *dtb, size_t size)
{
	int i;
	char *dt = dtb;
	const char *name = node->name ? node->name : "";

	if (dt)
		*((uint32_t *)(dt + size)) = NFP_HTOBE32(OF_DT_BEGIN_NODE);
	size += 4;		/* OF_DT_BEGIN_NODE */

	if (dt)
		strcpy(dt + size, name);
	size += (strlen(name) + 1 + 3) & ~3;

	for (i = 0; i < node->props; i++) {
		int offset;

		if (dt)
			*((uint32_t *)(dt + size)) = NFP_HTOBE32(OF_DT_PROP);
		size += 4;	/* OF_DT_PROP */

		if (dt)
			*((uint32_t *)(dt + size)) = NFP_HTOBE32(node->prop[i]->size);
		size += 4;	/* size of prop */

		offset = _dtn_strtab_insert(strtab, node->prop[i]->name);
		if (dt)
			*((uint32_t *)(dt + size)) = NFP_HTOBE32(offset);
		size += 4;	/* prop-name offset */

		if (dt)
			memcpy(dt + size, node->prop[i]->data, node->prop[i]->size);
		size += (node->prop[i]->size + 3) & ~3;
	}

	for (i = 0; i < node->children; i++)
		size = _dtn_flatten(node->child[i], strtab, dt, size);

	if (dt)
		*((uint32_t *)(dt + size)) = NFP_HTOBE32(OF_DT_END_NODE);
	size += 4;

	return size;
}

void *nfp_dtn_flatten(struct nfp_dtn *root, uint64_t phys_address, size_t *dtb_size)
{
	struct of_strtab strtab = { 0 };
	struct fdt_header fdt;
	size_t fdt_size = (sizeof(fdt) + 3) & ~3;
	size_t resvmap_size, dts_size;
	char *dt;
	int i;

	resvmap_size = sizeof(uint64_t) * (2 * (root->resvmap->entries + 1) + 2);

	dts_size = _dtn_flatten(root, &strtab, NULL, 0) + 4;

	*dtb_size = fdt_size + resvmap_size + dts_size + ((strtab.bytes + 3) & ~3);

	dt = calloc(1, *dtb_size);

	_dtn_flatten(root, &strtab, dt + fdt_size + resvmap_size, 0);
	*((uint32_t *)(dt + fdt_size + resvmap_size + dts_size - 4)) = NFP_HTOBE32(OF_DT_END);

	fdt.magic = NFP_HTOBE32(OF_DT_HEADER);
	fdt.totalsize = NFP_HTOBE32(*dtb_size);
	fdt.off_dt_struct = NFP_HTOBE32(fdt_size + resvmap_size);
	fdt.off_dt_strings = NFP_HTOBE32(fdt_size + resvmap_size + dts_size);
	fdt.off_mem_resvmap = NFP_HTOBE32(fdt_size);
	fdt.version = NFP_HTOBE32(17);
	fdt.last_comp_version = NFP_HTOBE32(16);
	fdt.boot_cpuid_phys = NFP_HTOBE32(0);
	fdt.size_dt_strings = NFP_HTOBE32(strtab.bytes);
	fdt.size_dt_struct = NFP_HTOBE32(dts_size);

	memcpy(dt, &fdt, sizeof(fdt));
	for (i = 0; i < root->resvmap->entries; i++) {
		((uint64_t *) (dt + fdt_size + i * (sizeof(uint64_t) * 2)))[0] = NFP_BETOH64(root->resvmap->entry[i].addr);
		((uint64_t *) (dt + fdt_size + i * (sizeof(uint64_t) * 2)))[1] = NFP_BETOH64(root->resvmap->entry[i].size);
	}
	((uint64_t *) (dt + fdt_size + i * (sizeof(uint64_t) * 2)))[0] = NFP_BETOH64(phys_address);
	((uint64_t *) (dt + fdt_size + i * (sizeof(uint64_t) * 2)))[1] = NFP_BETOH64(*dtb_size);
	i++;
	((uint64_t *) (dt + fdt_size + i * (sizeof(uint64_t) * 2)))[0] = NFP_BETOH64(0);
	((uint64_t *) (dt + fdt_size + i * (sizeof(uint64_t) * 2)))[1] = NFP_BETOH64(0);

	memcpy(dt + fdt_size + resvmap_size + dts_size, strtab.byte, strtab.bytes);

	if (strtab.offset)
		free(strtab.offset);
	if (strtab.byte)
		free(strtab.byte);

	return dt;
}

static struct nfp_dtn *_dtn_locate(struct nfp_dtn *parent, const char *path, int create)
{
	struct nfp_dtn *node, **tmp;

	if (parent) {
		const char *cp;
		int clen, i;

		if (!path)
			return parent;

		while (*path && *path == '/')
			path++;

		if (*path == 0)
			return parent;

		cp = strchr(path, '/');
		if (!cp)
			clen = strlen(path);
		else
			clen = cp - path;

		for (i = 0; i < parent->children; i++) {
			if (strlen(parent->child[i]->name) != clen)
				continue;
			if (strncmp(parent->child[i]->name, path, clen) == 0)
				return _dtn_locate(parent->child[i], cp, create);
		}

		if (!create)
			return NFP_ERRPTR(ENOENT);

		node = calloc(1, sizeof(*node) + clen + 1);
		if (!node)
			return NULL;

		node->root = parent->root;
		node->name = (char *)&node[1];
		memcpy(node->name, path, clen);
		node->name[clen] = 0;

		tmp = realloc(parent->child, sizeof(parent->child[0]) * (parent->children + 1));
		if (!tmp)
			return NULL;

		parent->child = tmp;
		parent->child[parent->children++] = node;

		node = nfp_dtn_create(parent->child[parent->children - 1], cp);
	} else {
		if (!create || path)
			return NFP_ERRPTR(EINVAL);

		node = calloc(1, sizeof(*node) + sizeof(struct of_resvmap));
		node->root = node;
		node->name = NULL;
		node->resvmap = (struct of_resvmap *)&node[1];
	}

	return node;
}

struct nfp_dtn *nfp_dtn_find(struct nfp_dtn *parent, const char *path)
{
	return _dtn_locate(parent, path, 0);
}

struct nfp_dtn *nfp_dtn_create(struct nfp_dtn *parent, const char *path)
{
	return _dtn_locate(parent, path, 1);
}

void nfp_dtn_free(struct nfp_dtn *node)
{
	int i;

	for (i = 0; i < node->props; i++)
		free(node->prop[i]);

	if (node->prop)
		free(node->prop);

	for (i = 0; i < node->children; i++)
		nfp_dtn_free(node->child[i]);

	if (node->child)
		free(node->child);

	free(node);
}

struct nfp_dtn *nfp_dtn_child_next(struct nfp_dtn *node, void **tmp)
{
	struct nfp_dtn **next = NULL, **prev = *tmp;

	if (!prev) {
		if (node->children > 0)
			next = &node->child[0];
	} else {
		next = prev + 1;
		if ((next - &node->child[0]) >= node->children)
			next = NULL;
	}

	*tmp = next;
	return next ? *next : NULL;
}

static struct nfp_dtn_prop *_dtn_prop_find(struct nfp_dtn *node, const char *prop_name)
{
	int i;

	for (i = 0; i < node->props; i++) {
		if (strcmp(node->prop[i]->name, prop_name) == 0)
			return node->prop[i];
	}

	return NULL;
}

int nfp_dtn_set_prop_value(struct nfp_dtn *node, const char *prop_name, size_t size, const void *data)
{
	struct nfp_dtn_prop *prop;
	int i;

	prop = calloc(1, sizeof(*prop) + size + strlen(prop_name) + 1);
	if (!prop)
		return -1;

	prop->size = size;
	prop->data = (char *)&prop[1];
	prop->name = prop->data + size;
	strcpy(prop->name, prop_name);

	memcpy(prop->data, data, size);

	for (i = 0; i < node->props; i++) {
		if (strcmp(node->prop[i]->name, prop_name) == 0) {
			free(node->prop[i]);
			break;
		}
	}

	if (i == node->props) {
		struct nfp_dtn_prop **tmp;

		tmp = realloc(node->prop, sizeof(*node->prop[0]) * (node->props + 1));
		if (!tmp) {
			free(prop);
			return -1;
		}
		node->prop = tmp;
		node->props++;
	}

	node->prop[i] = prop;

	return 0;
}

int nfp_dtn_set_prop(struct nfp_dtn *node, const char *prop_name)
{
	return nfp_dtn_set_prop_value(node, prop_name, 0, NULL);
}

int nfp_dtn_set_prop_u32(struct nfp_dtn *node, const char *prop_name, int values, uint32_t value, ...)
{
	va_list args;
	int cell, err;
	uint32_t *data;

	if (values < 1)
		return NFP_ERRNO(EINVAL);

	data = malloc(values * sizeof(data[0]));

	va_start(args, value);
	for (cell = 0; cell < values; value = va_arg(args, uint32_t), cell++)
		data[cell] = NFP_HTOBE32(value);
	va_end(args);

	err = nfp_dtn_set_prop_value(node, prop_name, values * sizeof(uint32_t), data);
	free(data);

	return err;
}

int nfp_dtn_set_prop_str(struct nfp_dtn *node, const char *prop_name, int values, const char *value, ...)
{
	va_list args;
	int cell, err;
	size_t len;
	const char **strs;
	char *strval;

	if (values < 1)
		return NFP_ERRNO(EINVAL);

	strs = malloc(values * sizeof(strs[0]));
	if (!strs)
		return -1;

	va_start(args, value);
	len = 0;
	for (cell = 0; cell < values; value = va_arg(args, char *), cell++) {
		strs[cell] = value;
		len += strlen(value) + 1;
	}
	va_end(args);

	strval = malloc(len);
	if (!strval) {
		free(strs);
		return -1;
	}

	len = 0;
	for (cell = 0; cell < values; cell++) {
		strcpy(strval + len, strs[cell]);
		len += strlen(strs[cell]) + 1;
	}
	free(strs);

	err = nfp_dtn_set_prop_value(node, prop_name, len, strval);
	free(strval);

	return err;
}

int nfp_dtn_get_prop_size(struct nfp_dtn *node, const char *prop_name)
{
	struct nfp_dtn_prop *prop;

	prop = _dtn_prop_find(node, prop_name);
	if (prop)
		return prop->size;

	return NFP_ERRNO(ENOENT);
}

int nfp_dtn_get_prop_cells(struct nfp_dtn *node, const char *prop_name, const void **value)
{
	struct nfp_dtn_prop *prop;

	prop = _dtn_prop_find(node, prop_name);
	if (prop) {
		if (value)
			*value = prop->data;
		return prop->size;
	}

	return NFP_ERRNO(ENOENT);
}

int nfp_dtn_get_prop_u32(struct nfp_dtn *node, const char *prop_name, int cell, uint32_t *value)
{
	struct nfp_dtn_prop *prop;

	prop = _dtn_prop_find(node, prop_name);
	if (prop) {
		cell *= sizeof(uint32_t);
		if (cell < prop->size) {
			*value = NFP_BETOH32(*(uint32_t *)(prop->data + cell));
			return 0;
		}
		return NFP_ERRNO(EINVAL);
	}

	return NFP_ERRNO(ENOENT);
}

int nfp_dtn_get_prop_str(struct nfp_dtn *node, const char *prop_name, int cell, const char **value)
{
	struct nfp_dtn_prop *prop;

	prop = _dtn_prop_find(node, prop_name);
	if (prop) {
		const char *cp;

		for (cp = prop->data;
		     cell && ((cp - (char *)prop->data) < prop->size);
		     cp += strlen(cp) + 1, cell--)
			;

		if ((cp - (char *)prop->data) < prop->size) {
			*value = cp;
			return 0;
		}
		return NFP_ERRNO(EINVAL);
	}

	return NFP_ERRNO(ENOENT);
}
