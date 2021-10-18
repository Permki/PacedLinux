/*
 * Copyright (C) 2012-2017 Netronome Systems, Inc.  All rights reserved.
 */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <inttypes.h>
#include <errno.h>
#include <limits.h>
#include <stddef.h>
#include <ctype.h>

#undef EMBEDDED
#include "libnfp_common.h"

#include <nfp-common/nfp_resid.h>
#include <nfp-common/nfp_platform.h>
#include <nfp-common/nfp_util.h>
#include <nfp-common/nfp_chipdata.h>
#include <nfp-common/nfp_regdata.h>
#include <nfp-common/ns_log.h>

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4706)
#endif

#ifdef _WIN32
#define PATH_SEP_C '\\'
#define PATH_SEP_S "\\"
#define PATH_LIST_SEP_C ';'
#else
#define PATH_SEP_C '/'
#define PATH_SEP_S "/"
#define PATH_LIST_SEP_C ':'
#endif

#define _CALLOC_OOM(_dst_, _n_, _sz_)                               \
	do {                                                        \
		if (((_sz_) == 0) || ((_n_) == 0)) {                \
			_dst_ = NULL;                               \
			break;                                      \
		}                                                   \
		_dst_ = calloc((_n_), (_sz_));                      \
		if ((_dst_) == NULL) {                              \
			err = ENOMEM;                               \
			NS_LOG(NS_LOG_LVL_FATAL, "Out of memory."); \
			goto error_cleanup;                         \
		}                                                   \
	} while (0)

#define _MALLOC_OOM(_dst_, _sz_)                                    \
	do {                                                        \
		if ((_sz_) == 0) {                                  \
			_dst_ = NULL;                               \
			break;                                      \
		}                                                   \
		_dst_ = malloc((_sz_));                             \
		if ((_dst_) == NULL) {                              \
			err = ENOMEM;                               \
			NS_LOG(NS_LOG_LVL_FATAL, "Out of memory."); \
			goto error_cleanup;                         \
		}                                                   \
	} while (0)

#define _REALLOC_OOM(_dst_, _sz_)                                   \
	do {                                                        \
		void *_newmem_ = NULL;                              \
								    \
		_newmem_ = realloc((void *)(_dst_), (_sz_));        \
		if ((_newmem_) == NULL) {                           \
			err = ENOMEM;                               \
			NS_LOG(NS_LOG_LVL_FATAL, "Out of memory."); \
			goto error_cleanup;                         \
		}                                                   \
		_dst_ = _newmem_;                                   \
	} while (0)

#define _STRDUP_OOM(_dst_, _s_)                                     \
	do {                                                        \
		if ((_s_) == NULL) {                                \
			_dst_ = NULL;                               \
			break;                                      \
		}                                                   \
		_dst_ = strdup((_s_));                              \
		if ((_dst_) == NULL) {                              \
			err = ENOMEM;                               \
			NS_LOG(NS_LOG_LVL_FATAL, "Out of memory."); \
			goto error_cleanup;                         \
		}                                                   \
	} while (0)

#define _OOM_CHECK(_v_)                                             \
	do {                                                        \
		if ((_v_) == NULL) {                                \
			err = ENOMEM;                               \
			NS_LOG(NS_LOG_LVL_FATAL, "Out of memory."); \
			goto error_cleanup;                         \
		}                                                   \
	} while (0)

#define _ARRAY_DECLARE(_type_, _name_) \
	struct {                       \
		size_t count;          \
		size_t capacity;       \
		_type_ *entry;         \
	} _name_

#define _ARRAY_ALLOC(_array_, _count_)                                                 \
	do {                                                                           \
		(_array_)->count = _count_;                                            \
		(_array_)->capacity = _count_;                                         \
		_CALLOC_OOM((_array_)->entry, (_count_), sizeof((_array_)->entry[0])); \
	} while (0)

#define _ARRAY_REALLOC(_array_, _count_)                                                 \
	do {                                                                             \
		_REALLOC_OOM((_array_)->entry, (_count_) * sizeof((_array_)->entry[0])); \
		if ((_array_)->count > (_count_))                                        \
			(_array_)->count = _count_;                                      \
		(_array_)->capacity = _count_;                                           \
	} while (0)

#define _ARRAY_EXTEND(_array_, _extra_count_)                          \
	do {                                                           \
		_REALLOC_OOM((_array_)->entry,                         \
			     ((_extra_count_) + (_array_)->capacity) * \
				     sizeof((_array_)->entry[0]));     \
		(_array_)->capacity += _extra_count_;                  \
	} while (0)

#define _ARRAY_DUP(_dst_, _src_)                                                                  \
	do {                                                                                      \
		_ARRAY_ALLOC((_dst_), (_src_)->count);                                            \
		memcpy((_dst_)->entry, (_src_)->entry, sizeof(*(_src_)->entry) * (_src_)->count); \
	} while (0)

#define _ARRAY_FREE(_array_)                                \
	do {                                                \
		if ((_array_)->entry)                       \
			free((_array_)->entry);             \
		(_array_)->count = (_array_)->capacity = 0; \
	} while (0)

#define NRD_IGN_CASE_FULLNAME(flg) ((flg) & NFP_REGDATA_FLG_IGN_CASE_FULLNAME)
#define NRD_IGN_CASE_ALTNAME(flg) ((flg) & NFP_REGDATA_FLG_IGN_CASE_ALTNAME)
#define NRD_USE_FULLNAME(flg) (~(flg) & NFP_REGDATA_MATCH_FLG_SKIP_FULLNAME)
#define NRD_USE_ALTNAME(flg) (~(flg) & NFP_REGDATA_MATCH_FLG_SKIP_ALTNAME)
#define NRD_USE_BITFIELDS(flg) (~(flg) & NFP_REGDATA_MATCH_FLG_SKIP_BITFIELDS)
#define NRD_USE_FQN_ONLY(flg) ((flg) & NFP_REGDATA_FLG_FQN_ONLY)
#define NRD_GET_FULLNAME(flg) ((flg) & NFP_REGDATA_FLG_GET_FULLNAME)
#define NRD_GET_FQN(flg) ((flg) & NFP_REGDATA_FLG_GET_FQN)
#define NRD_GET_ALTNAME(flg) ((flg) & NFP_REGDATA_FLG_GET_ALTNAME)
#define NRD_GET_DESC(flg) ((flg) & NFP_REGDATA_FLG_GET_DESC)
#define NRD_ALL_REPEATS(flg) ((flg) & NFP_REGDATA_MATCH_FLG_REPEATS_ALL)
#define NRD_REPEATS_RANGED(flg) ((flg) & NFP_REGDATA_MATCH_FLG_REPEATS_RANGED)
#define NRD_MATCH_PARTIAL(flg) ((flg) & NFP_REGDATA_MATCH_FLG_PARTIAL)
#define NRD_MATCH_NODESCEND(flg) ((flg) & NFP_REGDATA_MATCH_FLG_NO_DESCEND)
#define NRD_OFFSET_LOOKUP(flg) ((flg) & NFP_REGDATA_FLG_OFFSET_LOOKUP)

#define NRD_REG_MAPS_SUBDIR "reg_maps"
#define NRD_MAX_CTX_CNT 8
#define NRD_MEREG_TGTNAME "mereg"
#define NRD_NUM64_DEC_DIGITS 20 /* -9223372036854775808 */

#if (JANSSON_VERSION_HEX < 0x020300)
#define json_object_foreach(object, key, value)                                         \
	for (key = json_object_iter_key(json_object_iter(object));                      \
	     key && (value = json_object_iter_value(json_object_iter_at(object, key))); \
	     key = json_object_iter_key(                                                \
		     json_object_iter_next(object, json_object_iter_at(object, key))))
#endif

struct nfp_regdata_embedded_file {
	int family;
	int rev;
	const char *name;
	unsigned long usz;
	unsigned long csz;
	const unsigned char *data;
};

enum nrd_target_scope {
	NRD_TARGET_SCOPE_NONE = 0,
	NRD_TARGET_SCOPE_GLOBAL,
	NRD_TARGET_SCOPE_ISLAND,
	NRD_TARGET_SCOPE_SINGLE_ISLAND,
	NRD_TARGET_SCOPE_ME
};

struct nrd_enum_string_map {
	const char *str;
	int val;
};

struct nrd_strtab {
	size_t alloced;
	size_t offset;
	char *str;
	size_t str_cnt;
};

static const struct nrd_strtab nrd_strtab_zinit = {0, 0, NULL, 0};

struct nrd_str {
	size_t sz;
	char *str;
};

static const struct nrd_str nrd_str_zinit = {0, NULL};

struct nrd_str_alias {
	char *alias;
	char *fullname;
};

struct _nfp_reg_bitf {
	char *name;
	char *description;
	char *altname;
	uint32_t msb;
	uint32_t lsb;
	enum nfp_regfield_access access_mode;
};

struct nrd_register {
	char *id;
	uint32_t bit_len;
	size_t field_count;
	/* This struct has const fields that we malloc */
	/* and free in this file, but outside users */
	/* must see these as const. */
	struct _nfp_reg_bitf *fields;
};

struct nrd_addrmap_entry;

struct nrd_addrmap {
	char *id;

	_ARRAY_DECLARE(struct nrd_addrmap_entry, mapentries);
};

struct nrd_addrmap_entry {
	uint64_t offset;
	char *name;
	/* size_t altname_count; */
	/* char **altnames; */
	char *altname;
	char *description;
	enum nfp_regdata_node_type type;

	size_t dimensions;
	uint64_t repeat1; /* minimum of 1 */
	uint64_t repeat2;
	uint64_t offinc1;
	uint64_t offinc2;

	union {
		struct nrd_addrmap *map;
		struct nrd_register *reg;
		void *vp;
	} ptr;

	char *ptr_id;

	enum nfp_reg_type reg_type;
	union {
		struct {
			int dummy;
		} cpp;
		struct {
			int8_t latency_rd;  /* INT8_MIN for NA */
			int8_t latency_wr;  /* INT8_MIN for NA */
			int8_t latency_use; /* INT8_MIN for NA */
			uint8_t is_indirect_ctx_csr;
		} mecsr;
	} extra;

	uint8_t is_sim_backdoorable;
};

struct nrd_iid_remap {
	int iid;
	struct nrd_addrmap_entry *map;
	uint64_t mapidx1;
	uint64_t mapidx2;
};

struct nrd_mereg {
	enum nfp_mereg_type type;
	char *name;
	char *altname;
	char *description;
	uint32_t count;
	uint32_t count_mask;
	int me_lsb;
	int iid_lsb;
	int cpp_tgt;
	int8_t cpp_rd_action;
	int8_t cpp_rd_token;
	int8_t cpp_wr_action;
	int8_t cpp_wr_token;
	uint64_t cpp_rd_offset;
	uint64_t cpp_wr_offset;
	int byteaddr_lsb; /* Can be negative, means right shift. */
	uint64_t offset_cmp_mask;
};

struct nrd_target {
	int8_t cpp_tgt;
	int8_t cpp_rd_action;
	int8_t cpp_rd_token;
	int8_t cpp_wr_action;
	int8_t cpp_wr_token;
	uint64_t cpp_rd_offset;
	uint64_t cpp_wr_offset;
	uint64_t offset_cmp_mask;
	int8_t offset_iid_lsb;
	int8_t offset_me_lsb;
	enum nrd_target_scope scope;
	int single_island_iid;
	struct nrd_addrmap_entry map;
	struct nrd_strtab aliases;
	struct nrd_iid_remap *iid_remaps;
	size_t iid_remaps_cnt;
	enum nfp_reg_type reg_type;
	int byteaddr_lsb; /* Can be negative, means right shift. */
};

struct nrd_mapentry_node {
	struct nrd_addrmap_entry *this_entry;
	uint64_t this_idx1;
	uint64_t this_idx2;

	/* These are for virtual maps, -1 if the node is not the virtual map */
	/* this_entry will be set to the target map entry for virtual maps. */
	int iid;
	int menum;
	int ctx;
	int is_virtual;

	struct nrd_mapentry_node *parent;
	struct nrd_mapentry_node *child;
};

static const struct nrd_mapentry_node nrd_mapentry_node_zinit = {
	NULL, 0, 0, NFP_REGDATA_IID_NA, NFP_REGDATA_MENUM_NA, NFP_REGDATA_CTX_NA, 0, NULL, NULL};

struct nrd_map_entry_find_info {
	int lookup_depth;
	uint8_t saw_mecsr_map; /* used for handling ctxN in lookup string. */
	uint8_t saw_virtual_map;
	uint8_t saw_user_offset;
	uint64_t user_offset;
	/* uint8_t skip_first_level; */

	/* Additional info on search for caller */
	int iid;   /* IID_NA if not applicable after lookup */
	int menum; /* MENUM_NA if not applicable after lookup */
	int ctx;   /* CTX_NA */

	int ovr_iid;
	int ovr_menum;

	uint64_t mapidx1;
	uint64_t mapidx2;

	enum nfp_reg_type reg_type;
};

static const struct nrd_map_entry_find_info nrd_map_entry_find_info_zinit = {0,
									     0,
									     0,
									     0,
									     NFP_REGDATA_IID_NA,
									     NFP_REGDATA_MENUM_NA,
									     NFP_REGDATA_CTX_NA,
									     NFP_REGDATA_IID_NA,
									     NFP_REGDATA_MENUM_NA,
									     0,
									     0,
									     NFP_REG_TYPE_NONE};

struct nrd_mereg_find_info {
	int lookup_depth;
	int is_mereg;
	int matched;
	/* Additional info on search for caller */
	int iid;   /* IID_NA if not applicable after lookup */
	int menum; /* MENUM_NA if not applicable after lookup */
	int regidx;

	int ovr_iid;
	int ovr_menum;
};

static const struct nrd_mereg_find_info nrd_mereg_find_info_zinit = {0,
								     0,
								     0,
								     NFP_REGDATA_IID_NA,
								     NFP_REGDATA_MENUM_NA,
								     -1,
								     NFP_REGDATA_IID_NA,
								     NFP_REGDATA_MENUM_NA};

struct nfp_regdata {
	_ARRAY_DECLARE(struct nrd_target, tgt_coll);
	_ARRAY_DECLARE(struct nrd_addrmap, map_coll);
	_ARRAY_DECLARE(struct nrd_register, reg_coll);
	_ARRAY_DECLARE(struct nrd_mereg, mereg_coll);

	struct nrd_addrmap *empty_map;
	struct nrd_addrmap *me_local_csr_map;
	char *me_local_csr_map_id;
	struct nrd_strtab xpb_targets;
	int arch, family, rev;
	/* either xpb or xpbm, random. Only need cpp details */
	struct nrd_target *tgt_xpb;
	struct nrd_target *tgt_mecsr;
	struct nrd_str_alias *_aliases; /* internal SDK use aliases */
	size_t _aliases_cnt;

	char *json_dir;
	const struct nfp_chipdata_chip *chip;
	char *json_top_comment;
};

/* static Prototypes */
static nfp_inline char *safe_strdup(const char *s);

static int nrd_enum_from_string(const struct nrd_enum_string_map *str_map, const char *str);

static json_t *nrd_json_file_load(struct nfp_regdata *data,
				  const char *file_name,
				  int append_origin_comment);

static void nrd_resolve_ptrs(struct nfp_regdata *data, int force_all);

static int nrd_tgt_iidme_valid(struct nfp_regdata *data,
			       const struct nrd_target *tgt,
			       int iid,
			       int menum);

static int nrd_addrmap_cmp(const void *p1, const void *p2);

static void nrd_update_accessmode(struct nfp_regdata_node *node, size_t field_idx);

static int nrd_entry_offset_patch_extract(struct nfp_regdata *data,
					  const struct nrd_target *tgt,
					  const struct nrd_addrmap_entry *entry,
					  int *iid,
					  int *menum,
					  uint64_t *offset,
					  uint8_t user_offset);

static struct nrd_addrmap *nrd_addrmap_find_by_id(struct nrd_addrmap *mapentries,
						  size_t count,
						  const char *id);

static struct nrd_register *nrd_register_find_by_id(struct nrd_register *mapentries,
						    size_t count,
						    const char *id);

static int nrd_strtab_find(const struct nrd_strtab *strtab, const char *str);

static void nrd_update_reg_byte_offsets(struct nfp_regdata *data,
					struct nrd_addrmap_entry *map_entry,
					struct nfp_regdata_node *node);

static int nrd_addrmap_entry_ensure_ptr(struct nfp_regdata *data, struct nrd_addrmap_entry *entry);

static int nrd_match_prefix(const char *prefix, const char **str, int ignore_case, char suffix);

static struct nrd_register *nrd_register_collection_new_reg(struct nfp_regdata *data);

static int nrd_j_obj_strnum_get(json_t *j, const char *key, uint64_t *val);

static int nrd_strtab_add(struct nrd_strtab *strtab,
			  const char *str,
			  int allow_duplicate,
			  int case_sensitive);

static enum nfp_regfield_access nrd_access_mode_str2type(const char *s);

static struct nrd_addrmap *nrd_addrmap_collection_new_map(struct nfp_regdata *data);

static struct nrd_addrmap_entry *nrd_addrmap_new_entry(struct nrd_addrmap *map_in);

static void nrd_addrmap_entry_free(struct nrd_addrmap_entry *entry_in);

static int nrd_str_resize(struct nrd_str *str, size_t sz);

static int nrd_find_reg_in_maps(struct nfp_regdata *regdata,
				struct nrd_addrmap_entry **pmap_entry,
				uint64_t offset,
				uint64_t *pseen_offset,
				uint64_t *pmapidx1,
				uint64_t *pmapidx2,
				struct nrd_strtab *strtab,
				struct nrd_str *str);

static int nrd_strtab_find_prefix(const struct nrd_strtab *strtab,
				  const char **str,
				  int ignore_case,
				  char suffix);

static void nrd_register_free(struct nrd_register *reg_in);

static struct nrd_mereg *nrd_mereg_find(struct nfp_regdata *data,
					const char **tree_name,
					unsigned int flags,
					struct nrd_mereg_find_info *info);

static struct nrd_addrmap_entry *nrd_map_entry_find(struct nfp_regdata *data,
						    struct nrd_target *tgt,
						    struct nrd_addrmap_entry *entry,
						    const char **tree_name,
						    unsigned int flags,
						    uint64_t *offset_out,
						    struct nrd_map_entry_find_info *info);

static struct nrd_mereg *nrd_mereg_collection_new_mereg(struct nfp_regdata *data);

static int nrd_json_extract(struct nfp_regdata *data, json_t *j_root);

static int nrd_load_and_extract(struct nfp_regdata *data, const char *entry_id);

static int nrd_match_indexes(const char *wildname,
			     const char **match_name,
			     int ignore_case,
			     char suffix,
			     uint64_t *idx1,
			     uint64_t *idx2,
			     uint64_t repeat1,
			     uint64_t repeat2);

static int nrd_regs_lookup(struct nfp_regdata *regdata,
			   const char *name,
			   unsigned int flags,
			   struct nfp_regdata_node *node,
			   int local_island_id,
			   int local_menum);

static int nrd_discover_all(struct nfp_regdata *regdata,
			    unsigned int flags,
			    void *usrdata,
			    int (*cb_found_reg)(struct nfp_regdata *regdata,
						struct nfp_regdata_node *entry,
						void *usrdata),
			    struct nrd_target *tgt,
			    struct nrd_mapentry_node *this_node,
			    struct nrd_str *dynstr,
			    struct nrd_strtab *strtab);

static int nrd_matches(struct nfp_regdata *regdata,
		       const char *name,
		       unsigned int flags,
		       struct nrd_str *matchstr,
		       struct nrd_str *idstr,
		       struct nrd_strtab *strtab);

static struct nrd_target *nrd_target_collection_new_target(struct nfp_regdata *data);

static void nrd_addrmap_free(struct nrd_addrmap *map_in);

static int nrd_matches_add_str(struct nrd_strtab *strtab,
			       struct nrd_str *matchstr,
			       size_t base_name_len,
			       const char *append_name,
			       char suffix,
			       int ign_duplicate);

static int nrd_matches_add_entry(struct nrd_strtab *strtab,
				 struct nrd_str *matchstr,
				 struct nrd_str *idstr,
				 size_t base_name_len,
				 char suffix,
				 const struct nrd_addrmap_entry *entry,
				 unsigned int flags);

/* data */

static struct nrd_enum_string_map nrd_tgtscope_string_map[] = {
	{"global", (int)NRD_TARGET_SCOPE_GLOBAL},
	{"island", (int)NRD_TARGET_SCOPE_ISLAND},
	{"single_island", (int)NRD_TARGET_SCOPE_SINGLE_ISLAND},
	{"microengine", (int)NRD_TARGET_SCOPE_ME},
	{NULL, 0}
};

static struct nrd_enum_string_map nrd_mereg_string_map[] = {
	{"gpra", (int)NFP_MEREG_TYPE_GPRA},
	{"gprb", (int)NFP_MEREG_TYPE_GPRB},
	{"nn", (int)NFP_MEREG_TYPE_NN},
	{"xfer_in", (int)NFP_MEREG_TYPE_XFER_IN},
	{"xfer_out", (int)NFP_MEREG_TYPE_XFER_OUT},
	{NULL, 0}
};

/* static Implementations */

char *safe_strdup(const char *s)
{
	char *ret;

	if (!s)
		return NULL;
	ret = strdup(s);
	if (!ret)
		NS_LOG(NS_LOG_LVL_ERROR, "Out of memory.");
	return ret;
}

int nrd_json_extract(struct nfp_regdata *data, json_t *j_root)
{
	json_t *j_top, *j_topitem;
	const char *item_id;
	json_t *j_tmp;
	uint8_t is_me_csr_map = 0;
	struct nrd_strtab seen_names = nrd_strtab_zinit;
	int err = EINVAL;

	if ((!data) || (!j_root)) {
		NS_LOG(NS_LOG_LVL_FATAL, "Invalid parameters.");
		err = EINVAL;
		goto error_cleanup;
	}

	j_top = json_object_get(j_root, "regs");
	json_object_foreach(j_top, item_id, j_topitem) {
		const char *ctmp;
		json_t *j_fields;
		json_t *j_field;
		json_t *j_altname;
		struct nrd_register *reg;
		struct _nfp_reg_bitf *field;
		size_t szidx_field;
		uint64_t tmpu64;

		reg = nrd_register_collection_new_reg(data);
		_OOM_CHECK(reg);

		reg->id = strdup(item_id);

		if (nrd_j_obj_strnum_get(j_topitem, "bit_length", &tmpu64) != 0) {
			NS_LOG(NS_LOG_LVL_FATAL, "Bad bit_length for register %s", item_id);
			goto error_cleanup;
		}
		reg->bit_len = (uint32_t)tmpu64;

		j_fields = json_object_get(j_topitem, "fields");
		reg->field_count = json_array_size(j_fields);
		if (!j_fields) {
			/* May be an empty list if all fields are reserved fields, */
			/* but must be present still. */
			NS_LOG(NS_LOG_LVL_FATAL, "No fields defined for register %s", item_id);
			goto error_cleanup;
		}

		_CALLOC_OOM(reg->fields, reg->field_count, sizeof(*reg->fields));

		/* Reset the strtab */
		if (seen_names.str)
			free(seen_names.str);
		seen_names.str = NULL;
		seen_names.offset = 0;
		seen_names.alloced = 0;

		for (szidx_field = 0; szidx_field < reg->field_count; szidx_field++) {
			field = &reg->fields[szidx_field];

			j_field = json_array_get(j_fields, szidx_field);
			field->name =
				safe_strdup(json_string_value(json_object_get(j_field, "name")));
			if (!field->name) {
				NS_LOG(NS_LOG_LVL_FATAL,
				       "No field name for register %s, entry %" PRIuSZ, item_id,
				       szidx_field);
				goto error_cleanup;
			}
			switch (err = nrd_strtab_add(&seen_names, field->name, 0, 0)) {
			case 0:
				break;
			case 1:
				NS_LOG(NS_LOG_LVL_WARN, "Duplicate register field %s.%s", item_id,
				       field->name);
				err = EINVAL;
				break;
			default:
				goto error_cleanup;
			}

			j_altname = json_object_get(j_field, "altname");
			if ((j_altname) && !json_is_string(j_altname)) {
				NS_LOG(NS_LOG_LVL_FATAL, "Bad altname for register field %s.%s",
				       item_id, field->name);
				goto error_cleanup;
			}

			if (!!json_string_value(j_altname)) {
				/* If altname is case insensitive version of name, it's OK and */
				/* don't check for duplicate. This means altname may be exactly */
				/* the same as name. If it's the same, we already have a check */
				/* entry in seen_names. */
				if (strcasecmp(json_string_value(j_altname), field->name) == 0) {
					field->altname = strdup(json_string_value(j_altname));
				} else {
					switch (err = nrd_strtab_add(&seen_names,
								     json_string_value(j_altname),
								     0, 0)) {
					case 0:
						field->altname =
							strdup(json_string_value(j_altname));
						break;
					case 1:
						NS_LOG(NS_LOG_LVL_WARN,
						       "Duplicate register field %s.%s", item_id,
						       json_string_value(j_altname));
						err = EINVAL;
						break;
					default:
						goto error_cleanup;
					}
				}
			}

			j_tmp = json_object_get(j_field, "description");
			if (j_tmp) {
				if (json_is_string(j_tmp)) {
					field->description = strdup(json_string_value(j_tmp));
				} else {
					NS_LOG(NS_LOG_LVL_FATAL,
					       "Bad description for register field %s.%s", item_id,
					       field->name);
					goto error_cleanup;
				}
			}

			ctmp = json_string_value(json_object_get(j_field, "mode"));
			if (!ctmp) {
				NS_LOG(NS_LOG_LVL_FATAL, "No mode for register field %s.%s",
				       item_id, field->name);
				goto error_cleanup;
			}
			field->access_mode = nrd_access_mode_str2type(ctmp);
			if (field->access_mode == NFP_REGFIELD_ACCESS_NONE) {
				NS_LOG(NS_LOG_LVL_FATAL, "Bad access mode for register field %s.%s",
				       item_id, field->name);
				goto error_cleanup;
			}

			if (nrd_j_obj_strnum_get(j_field, "bit_msb", &tmpu64) != 0) {
				NS_LOG(NS_LOG_LVL_FATAL, "Bad bit_msb for register field %s.%s",
				       item_id, field->name);
				goto error_cleanup;
			}
			field->msb = (uint32_t)tmpu64;

			if (nrd_j_obj_strnum_get(j_field, "bit_lsb", &tmpu64) != 0) {
				NS_LOG(NS_LOG_LVL_FATAL, "Bad bit_lsb for register field %s.%s",
				       item_id, field->name);
				goto error_cleanup;
			}
			field->lsb = (uint32_t)tmpu64;
		}
	}

	j_top = json_object_get(j_root, "maps");
	json_object_foreach(j_top, item_id, j_topitem) {
		struct nrd_addrmap *address_map;
		json_t *j_entry;
		const char *entry_key;

		address_map = nrd_addrmap_collection_new_map(data);
		_OOM_CHECK(address_map);

		address_map->id = strdup(item_id);
		if (!data->me_local_csr_map)
			is_me_csr_map = (strcmp(item_id, data->me_local_csr_map_id) == 0);

		/* Reset the strtab */
		if (seen_names.str)
			free(seen_names.str);
		seen_names.str = NULL;
		seen_names.offset = 0;
		seen_names.alloced = 0;

		json_object_foreach(j_topitem, entry_key, j_entry) {
			struct nrd_addrmap_entry *map_entry;
			json_t *j_flags;
			json_t *j_latency;
			json_t *j_altname;
			const char *entry_name;
			const char *entry_altname;
			const char *node_type_str;
			enum nfp_regdata_node_type node_type;
			const char *entry_ptr_str;
			const char *entry_desc_str;
			char *c;
			uint64_t offset;
			uint64_t repeat1, repeat2, offinc1, offinc2;
			size_t dimens = 0;

			entry_name = json_string_value(json_object_get(j_entry, "name"));
			if (!entry_name) {
				NS_LOG(NS_LOG_LVL_FATAL,
				       "Missing entry name for map entry at offset %s of %s",
				       entry_key, item_id);
				goto error_cleanup;
			}
			switch (err = nrd_strtab_add(&seen_names, entry_name, 0, 0)) {
			case 0:
				break;
			case 1:
				NS_LOG(NS_LOG_LVL_WARN, "Duplicate map entry %s.%s", item_id,
				       entry_name);
				err = EINVAL;
				break;
			default:
				goto error_cleanup;
			}

			offset = nfp_strtou64(entry_key, &c, 0);
			if (*c != '\0') {
				NS_LOG(NS_LOG_LVL_FATAL, "Bad entry offset for map entry %s.%s",
				       item_id, entry_name);
				goto error_cleanup;
			}

			node_type_str = json_string_value(json_object_get(j_entry, "type"));
			if (!node_type_str) {
				NS_LOG(NS_LOG_LVL_FATAL, "Missing entry type for map entry %s.%s",
				       item_id, entry_name);
				goto error_cleanup;
			}

			if (strcmp(node_type_str, "regmap") == 0) {
				node_type = NFP_REGDATA_NODE_TYPE_ADDRMAP;
			} else if (strcmp(node_type_str, "reg") == 0) {
				node_type = NFP_REGDATA_NODE_TYPE_REG;
			} else if (strcmp(node_type_str, "multireg") == 0) {
				node_type = NFP_REGDATA_NODE_TYPE_MULTIREG;
			} else {
				NS_LOG(NS_LOG_LVL_FATAL, "Bad type for map entry %s.%s", item_id,
				       entry_name);
				goto error_cleanup;
			}

			entry_ptr_str = json_string_value(json_object_get(j_entry, "ptr"));
			if (!entry_ptr_str) {
				NS_LOG(NS_LOG_LVL_FATAL, "Missing entry ptr for map entry %s.%s",
				       item_id, entry_name);
				goto error_cleanup;
			}

			repeat1 = 1;
			repeat2 = 1; /* Makes loops simpler */
			offinc1 = 0;
			offinc2 = 0;
			j_tmp = json_object_get(j_entry, "repeat1");
			if (j_tmp) {
				if (nrd_j_obj_strnum_get(j_entry, "repeat1", &repeat1) != 0) {
					NS_LOG(NS_LOG_LVL_FATAL, "Bad repeat1 for map entry %s.%s",
					       item_id, entry_name);
					goto error_cleanup;
				}

				if (nrd_j_obj_strnum_get(j_entry, "offinc1", &offinc1) != 0) {
					NS_LOG(NS_LOG_LVL_FATAL, "Bad offinc1 for map entry %s.%s",
					       item_id, entry_name);
					goto error_cleanup;
				}

				dimens++;

				j_tmp = json_object_get(j_entry, "repeat2");
				if (j_tmp) {
					if (nrd_j_obj_strnum_get(j_entry, "repeat2", &repeat2) !=
					    0) {
						NS_LOG(NS_LOG_LVL_FATAL,
						       "Bad repeat2 for map entry %s.%s", item_id,
						       entry_name);
						goto error_cleanup;
					}

					if (nrd_j_obj_strnum_get(j_entry, "offinc2", &offinc2) !=
					    0) {
						NS_LOG(NS_LOG_LVL_FATAL,
						       "Bad offinc2 for map entry %s.%s", item_id,
						       entry_name);
						goto error_cleanup;
					}

					dimens++;
				}
			}

			j_tmp = json_object_get(j_entry, "description");
			entry_desc_str = json_string_value(j_tmp);
			if ((j_tmp) && !json_is_string(j_tmp)) {
				NS_LOG(NS_LOG_LVL_FATAL, "Bad description for map entry %s.%s",
				       item_id, entry_name);
				goto error_cleanup;
			}

			j_altname = json_object_get(j_entry, "altname");
			if ((j_altname) && !json_is_string(j_altname)) {
				NS_LOG(NS_LOG_LVL_FATAL, "Bad altname for map entry %s.%s", item_id,
				       entry_name);
				goto error_cleanup;
			}
			entry_altname = json_string_value(j_altname);
			if (entry_altname) {
				/* If altname is case insensitive version of name, it's OK and */
				/* don't check for duplicate. This means altname may be exactly */
				/* the same as name. If it's the same, we already have a check */
				/* entry in seen_names. */
				if (strcasecmp(entry_altname, entry_name) != 0) {
					switch (err = nrd_strtab_add(&seen_names, entry_altname, 0,
								     0)) {
					case 0:
						break;
					case 1:
						NS_LOG(NS_LOG_LVL_WARN, "Duplicate map entry %s.%s",
						       item_id, entry_altname);
						err = EINVAL;
						break;
					default:
						goto error_cleanup;
					}
				}
			}

			j_flags = json_object_get(j_entry, "flags");
			j_latency = json_object_get(j_entry, "me_csr_latency");

			if (!is_me_csr_map) {
				map_entry = nrd_addrmap_new_entry(address_map);
				_OOM_CHECK(map_entry);
				map_entry->offset = offset;
				map_entry->dimensions = dimens;
				map_entry->offinc1 = offinc1;
				map_entry->offinc2 = offinc2;
				map_entry->repeat1 = repeat1;
				map_entry->repeat2 = repeat2;
				map_entry->reg_type = NFP_REG_TYPE_NONE;

				map_entry->type = node_type;
				/* Going to do ptr lookups after all maps are loaded. */
				map_entry->ptr.vp = NULL;
				map_entry->ptr_id = strdup(entry_ptr_str);

				map_entry->name = safe_strdup(entry_name);
				map_entry->altname = safe_strdup(entry_altname);
				map_entry->description = safe_strdup(entry_desc_str);

				if (j_flags) {
					size_t idx;

					for (idx = 0; idx < json_array_size(j_flags); idx++) {
						const char *fstr = json_string_value(
							json_array_get(j_flags, idx));
						if (!fstr)
							continue;

						if (strcmp(fstr, "sdk_sim_backdoor") == 0)
							map_entry->is_sim_backdoorable = 1;
					}
				}
			} else {
				uint64_t mapidx1, mapidx2;
				/* Always expand the ME CSR map fully since */
				/* it's relatively small, */
				/* used a lot in nfas and keeps the mecsr */
				/* lookups much simpler. */
				/* The ME CSR map does not actually have repeating */
				/* entries at the moment. */
				for (mapidx1 = 0; mapidx1 < repeat1; mapidx1++)
					for (mapidx2 = 0; mapidx2 < repeat2; mapidx2++) {
						map_entry = nrd_addrmap_new_entry(address_map);
						_OOM_CHECK(map_entry);
						map_entry->offset = offset + (mapidx1 * offinc1) +
								    (mapidx2 * offinc2);
						map_entry->dimensions = dimens;
						map_entry->offinc1 = offinc1;
						map_entry->offinc2 = offinc2;
						map_entry->repeat1 = repeat1;
						map_entry->repeat2 = repeat2;
						map_entry->reg_type = NFP_REG_TYPE_MECSR;

						map_entry->type = node_type;
						/* Going to do ptr lookups after all maps are
						 * loaded.
						 */
						map_entry->ptr.vp = NULL;
						map_entry->ptr_id = strdup(entry_ptr_str);

						_MALLOC_OOM(map_entry->name,
							    (strlen(entry_name) +
							     (NRD_NUM64_DEC_DIGITS * dimens) + 1));
						sprintf(map_entry->name, entry_name, (int)mapidx1,
							(int)mapidx2);
						if (entry_altname) {
							_MALLOC_OOM(
								map_entry->altname,
								(strlen(entry_altname) +
								 (NRD_NUM64_DEC_DIGITS * dimens) +
								 1));
							sprintf(map_entry->altname, entry_altname,
								(int)mapidx1, (int)mapidx2);
						}

						if (entry_desc_str) {
							_MALLOC_OOM(
								map_entry->description,
								(strlen(entry_desc_str) +
								 (NRD_NUM64_DEC_DIGITS * dimens) +
								 1));
							sprintf(map_entry->description,
								entry_desc_str, (int)mapidx1,
								(int)mapidx2);
						}
						/* Get some ME CSR specific values */
						if (j_flags) {
							size_t idx;

							for (idx = 0;
							     idx < json_array_size(j_flags);
							     idx++) {
								const char *fstr = json_string_value(
									json_array_get(j_flags,
										       idx));
								if (!fstr)
									continue;

								if (strcmp(fstr, "indirect_me_ctx_csr") == 0)
									map_entry->extra.mecsr.is_indirect_ctx_csr = 1;
								else if (strcmp(fstr, "sdk_sim_backdoor") == 0)
									map_entry->is_sim_backdoorable = 1;
							}
						}

						if (j_latency) {
							uint64_t val;

#define GET_LATENCY(lt)                                                                           \
	do {                                                                                      \
		j_tmp = json_object_get(j_latency, #lt);                                          \
		if (j_tmp) {                                                                      \
			if (json_is_string(j_tmp) &&                                              \
			    (strcasecmp(json_string_value(j_tmp), "NA") == 0)) {                  \
				map_entry->extra.mecsr.latency_##lt =                             \
					NFP_REGDATA_MECSR_LATENCY_NA;                             \
			} else if ((nrd_j_obj_strnum_get(j_latency, #lt, &val) == 0) &&           \
				   (val <= INT8_MAX)) {                                           \
				map_entry->extra.mecsr.latency_##lt = (int8_t)val;                \
			} else {                                                                  \
				NS_LOG(NS_LOG_LVL_FATAL,                                          \
				       "Bad me_csr_latency." #lt " for map entry %s.%s", item_id, \
				       entry_name);                                               \
				goto error_cleanup;                                               \
			}                                                                         \
		}                                                                                 \
	} while (0)

							GET_LATENCY(wr);
							GET_LATENCY(rd);
							GET_LATENCY(use);
						}
					}
			}
		} /* json_object_foreach(j_topitem, entry_key, j_entry) */

		/* Sort the map entries so we can search them */
		qsort(address_map->mapentries.entry, address_map->mapentries.count,
		      sizeof(*address_map->mapentries.entry), nrd_addrmap_cmp);
	}

	/* We got new maps and regs, resolve any references to them. */
	nrd_resolve_ptrs(data, 0);

	if (seen_names.str)
		free(seen_names.str);
	return 0;

error_cleanup:
	if (seen_names.str)
		free(seen_names.str);

	return NFP_ERRNO(err);
}

json_t *nrd_json_file_load(struct nfp_regdata *data,
			   const char *file_name,
			   int append_origin_comment)
{
	char *s = NULL;
	json_t *j = NULL, *jj;
	json_error_t j_err;
	int err = EINVAL;

	if ((!data) || (!file_name)) {
		NS_LOG(NS_LOG_LVL_FATAL, "Invalid parameters.");
		goto error_cleanup;
	}

	if (!data->json_dir) {
		const struct nfp_regdata_embedded_file *file = nfp_regdata_embedded;
		char *embedded_json_str = NULL;
		unsigned long sz;

		NS_LOG(NS_LOG_LVL_HEAVY, "Searching for %s from embedded data.", file_name);
		for (; (file) && (file->name); file++) {
			if ((file->family != data->family) || (file->rev != data->rev))
				continue;
			if (strcmp(file_name, file->name) == 0)
				break;
		}

		if (!file->name) {
			char idstr[NFP_MEID_STR_SZ];

			if (!nfp_rev2str(idstr, data->rev))
				strncpy(idstr, "<?>", sizeof(idstr) - 1);

			NS_LOG(NS_LOG_LVL_FATAL,
			       "Could not find %s revision %s %s in embedded data.",
			       nfp_chipdata_family2str(data->family), idstr, file_name);
			goto error_cleanup;
		}

		sz = file->usz;
		_MALLOC_OOM(embedded_json_str, file->usz);
		if (uncompress((Bytef *)embedded_json_str, &sz, file->data, file->csz) != Z_OK) {
			err = EBADF;
			NS_LOG(NS_LOG_LVL_FATAL, "Could not uncompress embedded data");
			free(embedded_json_str);
			goto error_cleanup;
		}

		j = json_loadb(embedded_json_str, sz, 0, &j_err);
		if (!j) {
			NS_LOG(NS_LOG_LVL_FATAL, "Jansson error: %s, source %s:%d:%d", j_err.text,
			       j_err.source, j_err.line, j_err.column);
			err = EBADF;
			free(embedded_json_str);
			goto error_cleanup;
		}

		_MALLOC_OOM(s, (strlen("<embedded>/") + strlen(file_name) + 1));
		sprintf(s, "<embedded>/%s", file_name);

		free(embedded_json_str);
	} else {
		_MALLOC_OOM(s, (strlen(data->json_dir) + strlen(file_name) + 1));

		strcpy(s, data->json_dir);
		strcat(s, file_name);
		NS_LOG(NS_LOG_LVL_HEAVY, "Loading JSON file: %s", s);
		j = json_load_file(s, 0, &j_err);
		if (!j) {
			err = EIO;
			NS_LOG(NS_LOG_LVL_FATAL, "Jansson error: %s, source %s:%d:%d", j_err.text,
			       j_err.source, j_err.line, j_err.column);
			goto error_cleanup;
		}
	}

	if (append_origin_comment) {
		jj = json_object_get(j, "_comment");
		if (!jj) {
			jj = json_array();
			json_object_set_new(j, "_comment", jj);
		}

		if (json_is_array(jj)) {
			json_array_append_new(jj, json_string(""));
			json_array_append_new(jj, json_string("Loaded from:"));
			json_array_append_new(jj, json_string(s));
		}
	}

	if (s)
		free(s);
	return j;

error_cleanup:
	if (s)
		free(s);
	return NFP_ERRPTR(err);
}

void nrd_resolve_ptrs(struct nfp_regdata *data, int force_all)
{
	size_t top_idx;

	for (top_idx = 0; top_idx < data->map_coll.count; top_idx++) {
		size_t entry_idx;
		struct nrd_addrmap *map = &data->map_coll.entry[top_idx];

		for (entry_idx = 0; entry_idx < map->mapentries.count; entry_idx++) {
			struct nrd_addrmap_entry *entry = &map->mapentries.entry[entry_idx];

			if ((entry->ptr.vp) && (!force_all))
				continue; /* already resolved */

			switch (entry->type) {
			/* No targets here */
			case NFP_REGDATA_NODE_TYPE_ADDRMAP:
				entry->ptr.map = nrd_addrmap_find_by_id(
					data->map_coll.entry, data->map_coll.count, entry->ptr_id);
				break;
			case NFP_REGDATA_NODE_TYPE_REG:
			case NFP_REGDATA_NODE_TYPE_MULTIREG:
				entry->ptr.reg = nrd_register_find_by_id(
					data->reg_coll.entry, data->reg_coll.count, entry->ptr_id);
				break;
			default:
				NS_LOG(NS_LOG_LVL_ERROR, "Internal error");
				break;
			}
		}
	}

	for (top_idx = 0; top_idx < data->tgt_coll.count; top_idx++) {
		struct nrd_target *tgt = &data->tgt_coll.entry[top_idx];
		struct nrd_addrmap_entry *entry = &tgt->map;

		if (strcmp(tgt->map.ptr_id, data->me_local_csr_map_id) == 0)
			data->tgt_mecsr = tgt;
		else if (nrd_strtab_find(&data->xpb_targets, tgt->map.name))
			data->tgt_xpb = tgt;

		if ((entry->ptr.vp) && (!force_all))
			continue; /* already resolved */

		switch (entry->type) {
		case NFP_REGDATA_NODE_TYPE_TARGET:
		case NFP_REGDATA_NODE_TYPE_ADDRMAP:
			entry->ptr.map = nrd_addrmap_find_by_id(
				data->map_coll.entry, data->map_coll.count, entry->ptr_id);
			break;
		case NFP_REGDATA_NODE_TYPE_REG:
		case NFP_REGDATA_NODE_TYPE_MULTIREG:
			entry->ptr.reg = nrd_register_find_by_id(
				data->reg_coll.entry, data->reg_coll.count, entry->ptr_id);
			break;
		default:
			NS_LOG(NS_LOG_LVL_ERROR, "Internal error, %d", __LINE__);
			break;
		}
	}

	if ((!data->me_local_csr_map) || (force_all)) {
		data->me_local_csr_map = nrd_addrmap_find_by_id(
			data->map_coll.entry, data->map_coll.count, data->me_local_csr_map_id);
	}
}

int nrd_tgt_iidme_valid(struct nfp_regdata *data, const struct nrd_target *tgt, int iid, int menum)
{
	if ((!data) || (!data->chip))
		return 0;

	/* First check if local island is valid */
	if (!tgt) {
		if ((iid == NFP_REGDATA_IID_NA) || (iid == NFP_REGDATA_IID_LOCAL))
			return 1;
	}
	/* iid is now local or real, not NA */

	if (menum >= 0) {
		if (menum >= (int)nfp_chipdata_island_me_count(data->chip, iid))
			return 0;
	}

	if (iid > NFP_REGDATA_IID_LOCAL) {
		if (!nfp_chipdata_has_island(data->chip, iid))
			return 0;

		if ((tgt) && (tgt->scope != NRD_TARGET_SCOPE_GLOBAL) &&
		    (tgt->offset_iid_lsb == -1)) {
			/* This check is a bit odd, but the idea is that any target */
			/* which has a lookup result that includes an island ID */
			/* is bound to be the CtXpb target (14) and while */
			/* the xpb maps are considered global, the mecsr maps */
			/* are not global, but are accessed via a 'global' target */
			/* and therefore we shouldn't check if the target island has */
			/* the matching CPP-target. */
			if (!nfp_chipdata_island_has_cpp_target(data->chip, iid,
								tgt->cpp_tgt))
				return 0;
		}
	}

	return 1;
}

int nrd_addrmap_cmp(const void *p1, const void *p2)
{
	const struct nrd_addrmap_entry *a = (const struct nrd_addrmap_entry *)(p1);
	const struct nrd_addrmap_entry *b = (const struct nrd_addrmap_entry *)(p2);

	return (a->offset > b->offset) - (a->offset < b->offset);
}

void nrd_update_accessmode(struct nfp_regdata_node *node, size_t field_idx)
{
	size_t idx;

	if (!node)
		return;

	if (!node->fields)
		return;

	if (field_idx == node->field_cnt) {
		idx = 0;
		if (field_idx == 0) {
			NS_LOG(NS_LOG_LVL_ERROR, "Internal error, %d", __LINE__);
			return;
		}
		field_idx--;
	} else {
		idx = field_idx;
	}

	for (; idx <= field_idx; idx++) {
		switch (node->fields[idx].access_mode) {
		case NFP_REGFIELD_ACCESS_RC: /* Read Only, Read value then Clear */
			node->is_readable = 1;
			break;
		case NFP_REGFIELD_ACCESS_RCW: /* Read value then Clear, Write */
			node->is_readable = 1;
			node->is_writable = 1;
			break;
		case NFP_REGFIELD_ACCESS_RO: /* Read Only */
			node->is_readable = 1;
			break;
		case NFP_REGFIELD_ACCESS_RW: /* Read, Write */
			node->is_readable = 1;
			node->is_writable = 1;
			break;
		case NFP_REGFIELD_ACCESS_RW0C: /* Read, Write 0 to Clear */
			node->is_readable = 1;
			node->is_writable = 1;
			break;
		case NFP_REGFIELD_ACCESS_RW1C: /* Read, Write 1 to Clear */
			node->is_readable = 1;
			node->is_writable = 1;
			break;
		case NFP_REGFIELD_ACCESS_RW1S: /* Read, Write 1 to Set */
			node->is_readable = 1;
			node->is_writable = 1;
			break;
		case NFP_REGFIELD_ACCESS_RWC: /* Read, Write any value to Clear */
			node->is_readable = 1;
			node->is_writable = 1;
			break;
		case NFP_REGFIELD_ACCESS_RWL: /* Read, Write any value to Latch internal value */
			node->is_readable = 1;
			node->is_writable = 1;
			break;
		case NFP_REGFIELD_ACCESS_W0C: /* Write Only, Write 0 to Clear */
			node->is_writable = 1;
			break;
		case NFP_REGFIELD_ACCESS_W1C: /* Write Only, Write 1 to Clear */
			node->is_writable = 1;
			break;
		case NFP_REGFIELD_ACCESS_WC: /* Write Only, Write any value to Clear */
			node->is_writable = 1;
			break;
		case NFP_REGFIELD_ACCESS_WO: /* Write Only */
			node->is_writable = 1;
			break;
		case NFP_REGFIELD_ACCESS_NONE:
			break;
			/* No default, we need to cover all enum items, compiler can warn us when we
			 * don't
			 */
		}
	}
}

void nrd_update_reg_byte_offsets(struct nfp_regdata *data,
				 struct nrd_addrmap_entry *map_entry,
				 struct nfp_regdata_node *node)
{
	uint64_t off_start, off_end;
	struct nrd_addrmap_entry *e;

	if (!node)
		return;
	if (node->offset == UINT64_MAX)
		return;

	node->reg_byte_first = UINT64_MAX;
	node->reg_byte_last = UINT64_MAX;

	e = map_entry;
	off_start = node->offset;
	while ((e->type != NFP_REGDATA_NODE_TYPE_REG) &&
	       (e->type != NFP_REGDATA_NODE_TYPE_MULTIREG)) {
		if (nrd_addrmap_entry_ensure_ptr(data, e) != 0)
			return;
		if (e->ptr.map->mapentries.count == 0)
			return;
		e = &e->ptr.map->mapentries.entry[0];
		off_start += e->offset;
	}

	e = map_entry;
	off_end = node->offset;
	while ((e->type != NFP_REGDATA_NODE_TYPE_REG) &&
	       (e->type != NFP_REGDATA_NODE_TYPE_MULTIREG)) {
		if (nrd_addrmap_entry_ensure_ptr(data, e) != 0)
			return;
		if (e->ptr.map->mapentries.count == 0)
			return;
		e = &e->ptr.map->mapentries.entry[e->ptr.map->mapentries.count - 1];
		off_end += e->offset + (e->offinc1 * (e->repeat1 - 1)) +
			   (e->offinc2 * (e->repeat2 - 1));
	}

	if (nrd_addrmap_entry_ensure_ptr(data, e) != 0)
		return;
	off_end += ((e->ptr.reg->bit_len + 7) / 8) - 1;

	if ((node->bit_len == 0) && ((map_entry->type == NFP_REGDATA_NODE_TYPE_ADDRMAP) ||
				     (map_entry->type == NFP_REGDATA_NODE_TYPE_TARGET))) {
		size_t i;
		uint32_t blen = 0;

		for (i = 0; i < map_entry->ptr.map->mapentries.count; i++) {
			e = &map_entry->ptr.map->mapentries.entry[i];
			if (nrd_addrmap_entry_ensure_ptr(data, e) != 0)
				return;
			if ((e->type != NFP_REGDATA_NODE_TYPE_REG) &&
			    (e->type != NFP_REGDATA_NODE_TYPE_MULTIREG)) {
				blen = 0;
				break;
			}

			if (blen == 0) {
				blen = e->ptr.reg->bit_len;
			} else if (blen != e->ptr.reg->bit_len) {
				blen = 0;
				break;
			}
		}
		node->bit_len = blen;
	}

	node->reg_byte_first = off_start;
	node->reg_byte_last = off_end;
}

int nrd_entry_offset_patch_extract(struct nfp_regdata *data,
				   const struct nrd_target *tgt,
				   const struct nrd_addrmap_entry *entry,
				   int *iid,
				   int *menum,
				   uint64_t *offset,
				   uint8_t user_offset)
{
	if ((entry->type != NFP_REGDATA_NODE_TYPE_TARGET) || (user_offset)) {
		switch (tgt->scope) {
		case NRD_TARGET_SCOPE_GLOBAL:
			if (tgt->offset_iid_lsb != -1) {
				switch (data->arch) {
				case NFP_CHIP_ARCH_TH:
					if (*iid == NFP_REGDATA_IID_NA)
						*iid = ((*offset) >> tgt->offset_iid_lsb) & 0x3F;
					else if (*iid == NFP_REGDATA_IID_LOCAL)
						/* The lookup string was local-island scoped, */
						/* so clear out IID bits. */
						(*offset) &=
							~(UINT64_C(0x3F) << tgt->offset_iid_lsb);
					/* else */
					/* Used iX island map alias, no action */
					break;
				default:
					NS_LOG(NS_LOG_LVL_FATAL, "Unsupported chip family");
					return -EINVAL;
				}
			}

			if (tgt->offset_me_lsb != -1) {
				switch (data->arch) {
				case NFP_CHIP_ARCH_TH:
					/* Convert master ME number to ME number - */
					/* nfp6000 ME master is menum+4. */
					if (*menum == NFP_REGDATA_MENUM_NA)
						*menum = (((*offset) >> tgt->offset_me_lsb) & 0xF) -
							 4;
					else if (*menum == NFP_REGDATA_MENUM_LOCAL)
						/* The lookup string was local-me scoped, */
						/* so clear out menum bits. */
						(*offset) &= ~(UINT64_C(0xF) << tgt->offset_me_lsb);
					break;
				default:
					NS_LOG(NS_LOG_LVL_FATAL, "Unsupported chip family");
					return -EINVAL;
				}
			}
			break;
		case NRD_TARGET_SCOPE_SINGLE_ISLAND:
			if (*iid == NFP_REGDATA_IID_NA)
				*iid = tgt->single_island_iid;

			if ((tgt->offset_iid_lsb != -1) && (*iid > 0)) {
				switch (data->arch) {
				case NFP_CHIP_ARCH_TH:
					(*offset) |= ((uint64_t)*iid << tgt->offset_iid_lsb);
					break;
				default:
					NS_LOG(NS_LOG_LVL_FATAL, "Unsupported chip family");
					return -EINVAL;
				}
			}
			break;
		case NRD_TARGET_SCOPE_ISLAND:
			/* Island scope patches up the offset, extracts the menum */
			/* (if so specified in target */
			if ((tgt->offset_iid_lsb != -1) && (*iid > 0)) {
				switch (data->arch) {
				case NFP_CHIP_ARCH_TH:
					(*offset) |= ((uint64_t)*iid << tgt->offset_iid_lsb);
					break;
				default:
					NS_LOG(NS_LOG_LVL_FATAL, "Unsupported chip family");
					return -EINVAL;
				}
			}

			if (tgt->offset_me_lsb != -1) {
				switch (data->arch) {
				case NFP_CHIP_ARCH_TH:
					/* Convert master ME number to ME number - */
					/* nfp6000 ME master is menum+4. */
					*menum = (((*offset) >> tgt->offset_me_lsb) & 0xF) - 4;
					break;
				default:
					NS_LOG(NS_LOG_LVL_FATAL, "Unsupported chip family");
					return -EINVAL;
				}
			}
			break;
		case NRD_TARGET_SCOPE_ME:
			if ((tgt->offset_iid_lsb != -1) && (*iid > 0)) {
				switch (data->arch) {
				case NFP_CHIP_ARCH_TH:
					(*offset) |= ((uint64_t)*iid << tgt->offset_iid_lsb);
					break;
				default:
					NS_LOG(NS_LOG_LVL_FATAL, "Unsupported chip family");
					return -EINVAL;
				}
			}
			/* 0 is a valid ME num */
			if ((tgt->offset_me_lsb != -1) && (*menum >= 0)) {
				switch (data->arch) {
				case NFP_CHIP_ARCH_TH:
					/* Convert to master ID */
					(*offset) |= ((uint64_t)(*menum + 4) << tgt->offset_me_lsb);
					break;
				default:
					NS_LOG(NS_LOG_LVL_FATAL, "Unsupported chip family");
					return -EINVAL;
				}
			}
			break;
		default:
			break;
		}
	}

	if (!nrd_tgt_iidme_valid(data, tgt, *iid, *menum))
		return -ENODEV;

	return 0;
}

int nrd_strtab_add(struct nrd_strtab *strtab,
		   const char *str,
		   int allow_duplicate,
		   int case_sensitive)
{
	size_t strsz = 0;
	char *s, *end;
	size_t origsz;

	if (!str)
		return 1;

	origsz = strtab->alloced;
	strsz = strlen(str) + 1;
	end = strtab->str + strtab->offset;

	if (!allow_duplicate) {
		/* Check if string is already present. */
		for (s = strtab->str; s < end; s += strlen(s) + 1) {
			if (case_sensitive) {
				if (strcmp(s, str) == 0)
					return 1;
			} else {
				if (strcasecmp(s, str) == 0)
					return 1;
			}
		}
	}

	while (strtab->offset + strsz > strtab->alloced)
		strtab->alloced = strtab->alloced ? strtab->alloced * 2 : 256;

	if (strtab->alloced > origsz) {
		s = realloc(strtab->str, strtab->alloced);
		if (!s) {
			NS_LOG(NS_LOG_LVL_FATAL, "Out of memory.");
			return -ENOMEM;
		}
		strtab->str = s;
	}

	strncpy(&strtab->str[strtab->offset], str, strsz);
	strtab->offset += strsz;
	strtab->str_cnt++;
	return 0;
}

int nrd_str_resize(struct nrd_str *str, size_t sz)
{
	size_t origsz;
	char *s;

	if (!str)
		return -EINVAL;
	if (sz == 0)
		return 0;

	origsz = str->sz;

	while (sz > str->sz)
		str->sz = str->sz ? str->sz * 2 : 256;

	if (str->sz > origsz) {
		s = realloc(str->str, str->sz);
		if (!s) {
			NS_LOG(NS_LOG_LVL_FATAL, "Out of memory.");
			return -ENOMEM;
		}
		str->str = s;
	}
	return 0;
}

int nrd_strtab_find(const struct nrd_strtab *strtab, const char *str)
{
	char *s, *end;

	if (!str)
		return 1;

	end = strtab->str + strtab->offset;

	for (s = strtab->str; s < end; s += strlen(s) + 1) {
		if (strcmp(s, str) == 0)
			return 1;
	}

	return 0;
}

int nrd_strtab_find_prefix(const struct nrd_strtab *strtab,
			   const char **str,
			   int ignore_case,
			   char suffix)
{
	char *s, *end;

	end = strtab->str + strtab->offset;

	for (s = strtab->str; s < end; s += strlen(s) + 1) {
		if (nrd_match_prefix(s, str, ignore_case, suffix))
			return 1;
	}

	return 0;
}

/** This function handles negative values just fine, provided the caller
 *  casts 'val' to a signed type afterwards.
 * @return  May only return 0, -ENOENT or -EINVAL so that callers can check
 *          for -EINVAL where preferrable and if -ENOENT is possibly expected.
 */
int nrd_j_obj_strnum_get(json_t *j, const char *key, uint64_t *val)
{
	char *c;

	j = json_object_get(j, key);
	if (!j)
		return -ENOENT;

	if (json_is_string(j)) {
		*val = nfp_strtou64(json_string_value(j), &c, 0);
		if (*c != '\0')
			return -EINVAL;
	} else if (json_is_integer(j)) {
		*val = (uint64_t)json_integer_value(j);
	} else {
		return -EINVAL;
	}

	return 0;
}

enum nfp_regfield_access nrd_access_mode_str2type(const char *s)
{
	if (!s)
		return NFP_REGFIELD_ACCESS_NONE;

#define CHECK(NM) do {               \
	if (strcasecmp(s, #NM) == 0) \
		return NFP_REGFIELD_ACCESS_##NM; \
	} while (0)

	CHECK(RC);
	CHECK(RCW);
	CHECK(RO);
	CHECK(RW);
	CHECK(RW0C);
	CHECK(RW1C);
	CHECK(RW1S);
	CHECK(RWC);
	CHECK(RWL);
	CHECK(W0C);
	CHECK(W1C);
	CHECK(WC);
	CHECK(WO);
#undef CHECK
	return NFP_REGFIELD_ACCESS_NONE;
}

int nrd_enum_from_string(const struct nrd_enum_string_map *str_map, const char *str)
{
	const struct nrd_enum_string_map *ent;

	if (!str)
		return 0;

	for (ent = str_map; ent->str; ent++) {
		if (strcmp(ent->str, str) == 0)
			return ent->val;
	}

	return 0;
}

struct nrd_addrmap *nrd_addrmap_collection_new_map(struct nfp_regdata *data)
{
	int err = ENOMEM;
	struct nrd_addrmap *newitem = NULL;

	if (!data)
		return NULL;

	if (data->map_coll.capacity == data->map_coll.count) {
		_ARRAY_EXTEND(&data->map_coll, 1024);
		nrd_resolve_ptrs(data, 1);
	}

	newitem = &data->map_coll.entry[data->map_coll.count++];
	memset(newitem, 0, sizeof(*newitem));

	return newitem;

error_cleanup:
	return NFP_ERRPTR(err);
}

struct nrd_addrmap_entry *nrd_addrmap_new_entry(struct nrd_addrmap *map_in)
{
	int err = ENOMEM;
	struct nrd_addrmap_entry *newentry = NULL;

	if (!map_in)
		return NULL;

	if (map_in->mapentries.capacity == map_in->mapentries.count)
		_ARRAY_EXTEND(&map_in->mapentries, 1024);

	newentry = &map_in->mapentries.entry[map_in->mapentries.count++];
	memset(newentry, 0, sizeof(*newentry));

	return newentry;

error_cleanup:
	return NFP_ERRPTR(err);
}

void nrd_addrmap_free(struct nrd_addrmap *map_in)
{
	size_t i;

	if (!map_in)
		return;

	if (map_in->id) {
		free(map_in->id);
		map_in->id = NULL;
	}

	for (i = 0; i < map_in->mapentries.count; i++)
		nrd_addrmap_entry_free(&map_in->mapentries.entry[i]);

	_ARRAY_FREE(&map_in->mapentries);
}

void nrd_addrmap_entry_free(struct nrd_addrmap_entry *entry_in)
{
	if (!entry_in)
		return;

	if (entry_in->name) {
		free(entry_in->name);
		entry_in->name = NULL;
	}

	if (entry_in->description) {
		free(entry_in->description);
		entry_in->description = NULL;
	}

	if (entry_in->altname) {
		free(entry_in->altname);
		entry_in->altname = NULL;
	}

	if (entry_in->ptr_id) {
		free(entry_in->ptr_id);
		entry_in->ptr_id = NULL;
	}

	entry_in->offset = 0;
	entry_in->type = NFP_REGDATA_NODE_TYPE_NONE;
	entry_in->ptr.vp = NULL;
}

struct nrd_register *nrd_register_collection_new_reg(struct nfp_regdata *data)
{
	int err = ENOMEM;
	struct nrd_register *newitem = NULL;

	if (!data)
		return NULL;

	if (data->reg_coll.capacity == data->reg_coll.count) {
		_ARRAY_EXTEND(&data->reg_coll, 1024);
		nrd_resolve_ptrs(data, 1);
	}

	newitem = &data->reg_coll.entry[data->reg_coll.count++];
	memset(newitem, 0, sizeof(*newitem));

	return newitem;

error_cleanup:
	return NFP_ERRPTR(err);
}

void nrd_register_free(struct nrd_register *reg_in)
{
	size_t szidx_field;

	if (!reg_in)
		return;

	if (reg_in->id) {
		free(reg_in->id);
		reg_in->id = NULL;
	}

	if (reg_in->fields) {
		for (szidx_field = 0; szidx_field < reg_in->field_count; szidx_field++) {
			struct _nfp_reg_bitf *field = &reg_in->fields[szidx_field];

			if (field->name) {
				free(field->name);
				field->name = NULL;
			}

			if (field->description) {
				free(field->description);
				field->description = NULL;
			}

			if (field->altname) {
				free(field->altname);
				field->altname = NULL;
			}
		}
		free(reg_in->fields);
		reg_in->fields = NULL;
	}
}

struct nrd_mereg *nrd_mereg_collection_new_mereg(struct nfp_regdata *data)
{
	int err = ENOMEM;
	struct nrd_mereg *newitem = NULL;

	if (!data)
		return NULL;

	if (data->mereg_coll.capacity == data->mereg_coll.count)
		_ARRAY_EXTEND(&data->mereg_coll, 64);

	newitem = &data->mereg_coll.entry[data->mereg_coll.count++];
	memset(newitem, 0, sizeof(*newitem));

	return newitem;

error_cleanup:
	return NFP_ERRPTR(err);
}

struct nrd_target *nrd_target_collection_new_target(struct nfp_regdata *data)
{
	int err = ENOMEM;
	struct nrd_target *newitem = NULL;

	if (!data)
		return NULL;

	if (data->tgt_coll.capacity == data->tgt_coll.count) {
		_ARRAY_EXTEND(&data->tgt_coll, 16);
		nrd_resolve_ptrs(data, 1);
	}

	newitem = &data->tgt_coll.entry[data->tgt_coll.count++];
	memset(newitem, 0, sizeof(*newitem));

	return newitem;

error_cleanup:
	return NFP_ERRPTR(err);
}

struct nrd_addrmap *nrd_addrmap_find_by_id(struct nrd_addrmap *entries,
					   size_t count,
					   const char *id)
{
	size_t i;

	if ((!entries) || (count == 0) || (!id))
		return NULL;

	for (i = 0; i < count; i++) {
		if (strcmp(entries[i].id, id) == 0)
			return &entries[i];
	}

	return NULL;
}

struct nrd_register *nrd_register_find_by_id(struct nrd_register *entries,
					     size_t count,
					     const char *id)
{
	size_t i;

	if ((!entries) || (count == 0) || (!id))
		return NULL;

	for (i = 0; i < count; i++) {
		if (strcmp(entries[i].id, id) == 0)
			return &entries[i];
	}

	return NULL;
}

int nrd_load_and_extract(struct nfp_regdata *data, const char *entry_id)
{
	char *s_buf = NULL;
	const char *c;
	size_t idx;
	json_t *j_root = NULL;
	int err = EINVAL;

	if ((!data) || (!entry_id))
		goto error_cleanup;

	c = strchr(entry_id, '.');
	if (!c)
		goto error_cleanup;

	idx = c - entry_id;

	_MALLOC_OOM(s_buf, (idx + 5 /* strlen(".json") */ + 1));

	strncpy(s_buf, entry_id, idx);
	s_buf[idx] = '\0';
	strcat(s_buf, ".json");

	j_root = nrd_json_file_load(data, s_buf, 0);
	if (!j_root)
		goto error_cleanup;

	if (nrd_json_extract(data, j_root) != 0) {
		err = errno;
		goto error_cleanup;
	}

	json_decref(j_root);
	free(s_buf);
	return 0;

error_cleanup:

	if (j_root)
		json_decref(j_root);

	if (s_buf)
		free(s_buf);

	return NFP_ERRNO(err);
}

int nrd_addrmap_entry_ensure_ptr(struct nfp_regdata *data, struct nrd_addrmap_entry *entry)
{
	if ((!data) || (!entry))
		goto error_cleanup;

	if (!entry->ptr_id) {
		NS_LOG(NS_LOG_LVL_FATAL, "Internal error, %d", __LINE__);
		goto error_cleanup;
	}

	if (entry->ptr_id[0] == '\0') {
		entry->ptr.map = data->empty_map;
		return 0;
	}

	if (entry->ptr.vp)
		return 0;

	if (nrd_load_and_extract(data, entry->ptr_id) != 0)
		goto error_cleanup;

	if (!entry->ptr.vp) {
		NS_LOG(NS_LOG_LVL_FATAL, "Could not locate map entry: %s", entry->ptr_id);
		goto error_cleanup;
	}

	return 0;

error_cleanup:
	return -1;
}

int nrd_match_prefix(const char *prefix, const char **str, int ignore_case, char suffix)
{
	size_t prefix_len;
	int ret;

	if ((!prefix) || (!str) || (!*str))
		return 0;

	prefix_len = strlen(prefix);

	if (ignore_case) {
		if (strncasecmp(prefix, *str, prefix_len) != 0)
			return 0;
	} else {
		if (strncmp(prefix, *str, prefix_len) != 0)
			return 0;
	}

	ret = ((*str)[prefix_len] == '\0') || ((*str)[prefix_len] == suffix);
	if (ret)
		(*str) += prefix_len;

	return ret;
}

int nrd_match_indexes(const char *wildname,
		      const char **match_name,
		      int ignore_case,
		      char suffix,
		      uint64_t *idx1,
		      uint64_t *idx2,
		      uint64_t repeat1,
		      uint64_t repeat2)
{
	size_t fmtcnt = 0;
	const char *fmt = NULL;
	const char *name;

	/* This is essentially a specialised version of sscanf, */
	/* but it can compare case-insensitive. */
	if ((!wildname) || (!match_name) || (!*match_name))
		return 0;

	name = *match_name;

	*idx1 = *idx2 = UINT64_MAX;
	while (*wildname) {
		if ((!fmt) && (*wildname == '%')) {
			wildname++;
			fmt = wildname;
			if (*fmt == '\0')
				return 0;
		} else if ((!fmt) || (*fmt == '%')) {
			if (*name == suffix)
				break;

			if ((*wildname == *name) ||
			    (ignore_case && (tolower(*wildname) == tolower(*name)))) {
				wildname++;
				name++;
				continue;
			}

			return 0;
		}

		if (*fmt == 'd') {
			uint64_t v = 0;
			char *c;

			v = nfp_strtou64(name, &c, 10);
			if (c == name)
				return 0;
			name = c;

			switch (fmtcnt) {
			case 0:
				*idx1 = v;
				break;
			case 1:
				*idx2 = v;
				break;
			default:
				return 0;
			}
			fmt = NULL;
			wildname++;
			fmtcnt++;
		} else {
			return 0;
		}
	}

	if ((!fmt) && (*wildname == '\0') && ((*name == '\0') || (*name == suffix))) {
		if ((*idx1 != UINT64_MAX) && (*idx1 >= repeat1))
			return 0;
		if ((*idx2 != UINT64_MAX) && (*idx2 >= repeat2))
			return 0;
		*match_name = name;
		return 1;
	}

	return 0;
}

/**
 * @param tgt               Can be NULL, but generally the target holding
 *                          the top level map
 * @param recusrion_ctx     User must set info->lookup_depth to 0 before
 *                          calling and zero the struct
 */
struct nrd_addrmap_entry *nrd_map_entry_find(struct nfp_regdata *data,
					     struct nrd_target *tgt,
					     struct nrd_addrmap_entry *entry,
					     const char **tree_name,
					     unsigned int flags,
					     uint64_t *offset_out,
					     struct nrd_map_entry_find_info *info)
{
	size_t idx;
	char name_suffix = '.';
	const char *c;
	int idval;
	int found_match = 0;
	int scopeable_match = 0;
	int add_offset = 0;
	uint64_t mapidx1 = UINT64_MAX;
	uint64_t mapidx2 = UINT64_MAX;
	uint64_t user_offset = UINT64_MAX;
	char *end_str;

	if ((!data) || (!tgt) || (!entry) || (!tree_name) || (!*tree_name) || (!offset_out) ||
	    (!info))
		return NULL;

	if (info->lookup_depth == 0) {
		name_suffix = ':'; /* tgt:map.map.reg */
		info->iid = NFP_REGDATA_IID_NA;
		info->menum = NFP_REGDATA_MENUM_NA;
		info->ctx = NFP_REGDATA_CTX_NA;
		info->reg_type = tgt->reg_type;

		found_match = nrd_match_prefix(tgt->map.name, tree_name,
					       NRD_IGN_CASE_FULLNAME(flags), name_suffix) ||
			      nrd_strtab_find_prefix(&tgt->aliases, tree_name,
						     NRD_IGN_CASE_ALTNAME(flags), name_suffix);

		if (found_match) {
			add_offset = 1;
			info->lookup_depth++;
			entry = &tgt->map;
			if ((*tree_name)[0] == name_suffix)
				(*tree_name)++;
		} else {
			return NULL;
		}
	} else {
		if (nrd_addrmap_entry_ensure_ptr(data, entry) != 0)
			return NULL;
	}

	/* Check if this is .ctxN. */
	if ((!found_match) && info->saw_mecsr_map && (info->ctx == NFP_REGDATA_CTX_NA)) {
		/* We are a child of an ME CSR map and have not yet seen ctxN. */
		/* We only check for ctxN if no map actually had that name. */
		if (strncasecmp(*tree_name, "ctx", 3) == 0) {
			/* So far so good, now check if the digit makes sense. */
			if (((*tree_name)[3] >= '0') &&
			    ((*tree_name)[3] <= ('0' + NRD_MAX_CTX_CNT))) {
				/* We only handle one digit now and expect something to */
				/* follow ctxN, */
				/* so the next char should be a dot.
				 */
				if ((*tree_name)[4] == '.') {
					info->ctx = ((*tree_name)[3] - '0');
					(*tree_name) += 5;
					info->lookup_depth++;
					info->saw_virtual_map = 1;
					scopeable_match = 1;
					found_match = 1;
				} else if ((*tree_name)[4] == '\0') {
					info->ctx = ((*tree_name)[3] - '0');
					info->saw_virtual_map = 1;
					return &tgt->map;
				} else {
					return NULL;
				}
			}
		}
	}
	/* First check the map entries for a match, then see if it was some */
	/* of the magic prefixes. */
	if (!found_match) {
		for (idx = 0; idx < entry->ptr.map->mapentries.count; idx++) {
			struct nrd_addrmap_entry *e = &entry->ptr.map->mapentries.entry[idx];

			if (nrd_addrmap_entry_ensure_ptr(data, e) != 0)
				return NULL;

			if (nrd_match_indexes(e->name, tree_name, NRD_IGN_CASE_FULLNAME(flags),
					      name_suffix, &mapidx1, &mapidx2, e->repeat1,
					      e->repeat2)) {
				if ((*tree_name)[0] == name_suffix)
					(*tree_name)++;
				found_match = 1;
				scopeable_match = 1;
				add_offset = 1;
				info->lookup_depth++;
				entry = e;
				break;
			} else if (nrd_match_indexes(e->altname, tree_name,
						     NRD_IGN_CASE_FULLNAME(flags), name_suffix,
						     &mapidx1, &mapidx2, e->repeat1, e->repeat2)) {
				if ((*tree_name)[0] == name_suffix)
					(*tree_name)++;
				found_match = 1;
				scopeable_match = 1;
				add_offset = 1;
				info->lookup_depth++;
				entry = e;
				break;
			}
		}
	}

	if ((!found_match) && NRD_OFFSET_LOOKUP(flags)) {
		user_offset = nfp_strtou64(*tree_name, &end_str, 0);
		if (*end_str == '\0') {
			*offset_out += user_offset;
			*tree_name += strlen(*tree_name);
			found_match = 1;
			scopeable_match = 1;
			info->saw_user_offset = 1;
			info->user_offset = user_offset;
		}
	}

	if (found_match && scopeable_match) {
		/* We matched a map entry above, so validate FQN here */
		/* and if scope omitted, make local */
		switch (tgt->scope) {
		case NRD_TARGET_SCOPE_GLOBAL:
			break;
		case NRD_TARGET_SCOPE_SINGLE_ISLAND:
			/* Omitted iX must become the single island ID. */
			if (info->iid == NFP_REGDATA_IID_NA)
				info->iid = tgt->single_island_iid;

			if ((info->iid == NFP_REGDATA_IID_LOCAL) &&
			    (info->ovr_iid != NFP_REGDATA_IID_NA))
				info->iid = info->ovr_iid;

			if (NRD_USE_FQN_ONLY(flags) && (info->iid == NFP_REGDATA_IID_LOCAL))
				return NULL;
			break;
		case NRD_TARGET_SCOPE_ISLAND:
			if (info->iid == NFP_REGDATA_IID_NA)
				info->iid = NFP_REGDATA_IID_LOCAL;

			if ((info->iid == NFP_REGDATA_IID_LOCAL) &&
			    (info->ovr_iid != NFP_REGDATA_IID_NA))
				info->iid = info->ovr_iid;

			if (NRD_USE_FQN_ONLY(flags) && (info->iid == NFP_REGDATA_IID_LOCAL))
				return NULL;
			break;
		case NRD_TARGET_SCOPE_ME:
			if (info->iid == NFP_REGDATA_IID_NA)
				info->iid = NFP_REGDATA_IID_LOCAL;
			if (info->menum == NFP_REGDATA_MENUM_NA)
				info->menum = NFP_REGDATA_MENUM_LOCAL;

			if ((info->iid == NFP_REGDATA_IID_LOCAL) &&
			    (info->ovr_iid != NFP_REGDATA_IID_NA))
				info->iid = info->ovr_iid;
			if ((info->menum == NFP_REGDATA_MENUM_LOCAL) &&
			    (info->ovr_menum != NFP_REGDATA_MENUM_NA))
				info->menum = info->ovr_menum;

			if (NRD_USE_FQN_ONLY(flags) && (info->iid == NFP_REGDATA_IID_LOCAL))
				return NULL;
			if (NRD_USE_FQN_ONLY(flags) && (info->menum == NFP_REGDATA_MENUM_LOCAL))
				return NULL;
			break;
		default:
			break;
		}
	}

	if ((!found_match) && (info->lookup_depth == 1)) {
		switch (tgt->scope) {
		case NRD_TARGET_SCOPE_GLOBAL:
			idval = nfp_idstr2island(data->family, *tree_name, &c);

			if (info->ovr_iid != NFP_REGDATA_IID_NA) {
				if (idval == 0) {
					idval = info->ovr_iid;
				} else if (idval == -1) {
					idval = info->ovr_iid;
					c = *tree_name;
				}
			}

			if (idval != -1) {
				if (idval == 0) {
					if (NRD_USE_FQN_ONLY(flags))
						return NULL;
					info->iid = NFP_REGDATA_IID_LOCAL;
					(*tree_name) = c;
					info->lookup_depth++;
					info->saw_virtual_map = 1;
					if ((*tree_name)[0] == '\0')
						return &tgt->map;

					found_match = 1;
				} else {
					for (idx = 0; idx < tgt->iid_remaps_cnt; idx++) {
						if (idval == tgt->iid_remaps[idx].iid) {
							info->iid = idval;
							(*tree_name) = c;
							info->lookup_depth++;
							entry = tgt->iid_remaps[idx].map;
							mapidx1 = tgt->iid_remaps[idx].mapidx1;
							mapidx2 = tgt->iid_remaps[idx].mapidx2;
							found_match = 1;
							add_offset = 1;
							break;
						}
					}
				}
			} else {
				if (NRD_USE_FQN_ONLY(flags))
					return NULL;
				idval = 0;
				info->iid = NFP_REGDATA_IID_LOCAL;
				info->lookup_depth++;
			}

			if (info->iid == NFP_REGDATA_IID_LOCAL) {
				/* Might be xpb:i0.map or xpb:map, where map is a */
				/* map within an island. */
				for (idx = 0; idx < tgt->map.ptr.map->mapentries.count; idx++) {
					const char *found_name = *tree_name;
					uint64_t found_offset = *offset_out;
					struct nrd_map_entry_find_info found_info = *info;

					if (nrd_addrmap_entry_ensure_ptr(
						    data,
						    &tgt->map.ptr.map->mapentries.entry[idx]) != 0)
						return NULL;

					entry = nrd_map_entry_find(
						data, tgt, &tgt->map.ptr.map->mapentries.entry[idx],
						&found_name, flags, &found_offset, &found_info);
					if (found_info.saw_user_offset && (entry))
						entry = &tgt->map;

					if (entry) {
						(*tree_name) = found_name;
						if (found_info.saw_user_offset)
							*offset_out = found_offset;
						else
							*offset_out = found_offset +
								      tgt->map.ptr.map->mapentries
									      .entry[idx]
									      .offset;
						*info = found_info;
						found_match = 1;
						break;
					}
				}
			}
			break;
		case NRD_TARGET_SCOPE_SINGLE_ISLAND:
			idval = nfp_idstr2island(data->family, *tree_name, &c);

			if ((idval <= 0) && NRD_USE_FQN_ONLY(flags))
				return NULL;

			if (idval != -1) {
				if ((idval != 0) && (idval != tgt->single_island_iid))
					return NULL;
				info->iid = idval;
				(*tree_name) = c;
				info->lookup_depth++;
				found_match = 1;
				info->saw_virtual_map = 1;
				if ((*tree_name)[0] == '\0')
					return &tgt->map;
			}
			break;
		case NRD_TARGET_SCOPE_ISLAND:
			idval = nfp_idstr2island(data->family, *tree_name, &c);

			if ((idval <= 0) && NRD_USE_FQN_ONLY(flags))
				return NULL;

			if (idval != -1) {
				info->iid = idval;
				(*tree_name) = c;
				info->lookup_depth++;
				found_match = 1;
				info->saw_virtual_map = 1;
				if ((*tree_name)[0] == '\0')
					return &tgt->map;
			}
			break;
		case NRD_TARGET_SCOPE_ME:
			/* Accept iX, meY or iX.meY as virtual maps. */
			idval = nfp_idstr2island(data->family, *tree_name, &c);

			if ((idval <= 0) && NRD_USE_FQN_ONLY(flags))
				return NULL;

			if (idval != -1) {
				info->iid = idval;
				(*tree_name) = c;
				info->lookup_depth++;
				info->saw_virtual_map = 1;
				found_match = 1;
				if ((*tree_name)[0] == '\0')
					return &tgt->map;
			}

			idval = nfp_idstr2menum(data->family, *tree_name, &c);

			if (idval != -1) {
				info->menum = idval;
				(*tree_name) = c;
				info->lookup_depth++;
				info->saw_virtual_map = 1;
				found_match = 1;
				if (info->iid == NFP_REGDATA_IID_NA)
					info->iid = NFP_REGDATA_IID_LOCAL;
				if ((*tree_name)[0] == '\0')
					return &tgt->map;
			} else if (info->iid != NFP_REGDATA_IID_NA) {
				/* We matched iX.something, not allowed for this scope */
				return NULL;
			}

			break;
		default:
			break;
		}
	}

	if (found_match) {
		if (add_offset) {
			*offset_out += entry->offset;
			if (mapidx1 != UINT64_MAX) {
				if ((entry->repeat1 == 0) || (mapidx1 >= entry->repeat1))
					return NULL;
				*offset_out += (entry->offinc1 * mapidx1);
				info->mapidx1 = mapidx1;
			}

			if (mapidx2 != UINT64_MAX) {
				if ((entry->repeat2 == 0) || (mapidx2 >= entry->repeat2))
					return NULL;
				*offset_out += (entry->offinc2 * mapidx2);
				info->mapidx2 = mapidx2;
			}
		}
		/* It is important to note that nrd_addrmap_entry_ensure_ptr made sure */
		/* that data->me_local_csr_map is valid at this point. */
		if (entry->ptr.map == data->me_local_csr_map)
			info->saw_mecsr_map = 1;

		if ((*tree_name)[0] == '\0') {
			/* End of search string and we matched, maybe somewhere */
			/* in the middle of a map tree, maybe on a register. */
			return entry;
		}

		if ((entry->type == NFP_REGDATA_NODE_TYPE_REG) ||
		    (entry->type == NFP_REGDATA_NODE_TYPE_MULTIREG)) {
			/* If entry is not a map, the remaining string could be */
			/* a bit field name, the caller must check the returned */
			/* map entry type and search in the register if needed. */
			return entry;
		}

		return nrd_map_entry_find(data, tgt, entry, tree_name, flags, offset_out, info);
	}

	return NULL;
}

struct nrd_mereg *nrd_mereg_find(struct nfp_regdata *data,
				 const char **tree_name,
				 unsigned int flags,
				 struct nrd_mereg_find_info *info)
{
	size_t idx;
	char name_suffix = '.';
	const char *c;
	const char *nm;
	int idval;
	struct nrd_mereg *mereg = NULL;
	uint64_t v1 = 0, v2 = 0;
	int saw_iid = 0;

	if ((!data) || (!tree_name) || (!*tree_name) || (!info))
		return NULL;

	nm = *tree_name;

	/* First check for "mereg:" */
	name_suffix = ':'; /* tgt:map.map.reg */
	info->iid = NFP_REGDATA_IID_NA;
	info->menum = NFP_REGDATA_MENUM_NA;
	info->regidx = -1;
	info->is_mereg = 0;
	info->lookup_depth = 0;
	info->matched = 0;

	switch (data->arch) {
	case NFP_CHIP_ARCH_TH:
		break;
	default:
		NS_LOG(NS_LOG_LVL_FATAL, "Unsupported chip family");
		return NULL;
	}

	info->matched = nrd_match_prefix(
		NRD_MEREG_TGTNAME, &nm, NRD_IGN_CASE_FULLNAME(flags) || NRD_IGN_CASE_ALTNAME(flags),
		name_suffix);

	if (!info->matched)
		return NULL;

	info->lookup_depth++;
	info->is_mereg = 1;

	if (info->matched && (nm[0] == name_suffix))
		nm++;
	if (nm[0] == '\0') {
		*tree_name = nm;
		return NULL;
	}
	/* Now check for mereg:iX.meY */
	name_suffix = '.';
	idval = nfp_idstr2island(data->family, nm, &c);
	if (NRD_USE_FQN_ONLY(flags) && (idval <= 0)) {
		info->matched = 0;
		return NULL;
	}

	if (idval != -1) {
		if ((idval == 0) && (info->ovr_iid != NFP_REGDATA_IID_NA))
			idval = info->ovr_iid;
		saw_iid = 1;
		info->iid = idval;
		nm = c;
		info->lookup_depth++;
		if (nm[0] == '\0') {
			*tree_name = nm;
			return NULL;
		}
	} else {
		if (info->ovr_iid != NFP_REGDATA_IID_NA)
			info->iid = info->ovr_iid;
		else
			info->iid = NFP_REGDATA_IID_LOCAL;
	}

	idval = nfp_idstr2menum(data->family, nm, &c);
	if ((idval == -1) && ((NRD_USE_FQN_ONLY(flags)) || (saw_iid))) {
		info->matched = 0;
		return NULL;
	}

	if (idval != -1) {
		info->menum = idval;
		nm = c;
		info->lookup_depth++;
		if (nm[0] == '\0') {
			*tree_name = nm;
			return NULL;
		}
	} else {
		if (info->ovr_menum != NFP_REGDATA_IID_NA)
			info->menum = info->ovr_menum;
		else
			info->menum = NFP_REGDATA_MENUM_LOCAL;
	}

	/* nm now points to a register name */
	info->matched = 0;
	for (idx = 0; idx < data->mereg_coll.count; idx++) {
		mereg = &data->mereg_coll.entry[idx];
		info->matched = nrd_match_indexes(mereg->name, &nm, NRD_IGN_CASE_FULLNAME(flags),
						  '\0', &v1, &v2, mereg->count, 0) ||
				nrd_match_indexes(mereg->altname, &nm, NRD_IGN_CASE_ALTNAME(flags),
						  '\0', &v1, &v2, mereg->count, 0);

		if (info->matched) {
			if (v1 >= mereg->count)
				info->matched = 0;
			break;
		}
	}

	if (!info->matched)
		return NULL;

	if (v2 != UINT64_MAX) {
		info->matched = 0;
		return NULL;
	}

	info->lookup_depth++;
	info->regidx = (int)v1;
	return mereg;
}

int nrd_matches_add_str(struct nrd_strtab *strtab,
			struct nrd_str *matchstr,
			size_t base_name_len,
			const char *append_name,
			char suffix,
			int ign_duplicate)
{
	size_t s_idx;
	size_t appendlen;
	int err;

	if (!append_name)
		return 0;

	appendlen = strlen(append_name);

	err = nrd_str_resize(matchstr, (base_name_len + 1 + appendlen));
	if (err != 0)
		return err;

	s_idx = base_name_len;

	/* Truncate any previous string after the base name */
	matchstr->str[base_name_len] = '\0';

	if (append_name[0] != '\0') {
		if (s_idx > 0) {
			matchstr->str[s_idx++] = suffix;
			matchstr->str[s_idx] = '\0';
		}
		strcat(matchstr->str, append_name);
	}

	err = nrd_strtab_add(strtab, matchstr->str, (!ign_duplicate), 1);
	switch (err) {
	case -ENOMEM:
		NS_LOG(NS_LOG_LVL_FATAL, "Out of memory.");
		return -ENOMEM;
	case 0:
		break;
	case 1:
		if (!ign_duplicate)
			NS_LOG(NS_LOG_LVL_WARN, "Duplicate match: \"%s\"", matchstr->str);
		break;
	default:
		NS_LOG(NS_LOG_LVL_FATAL, "Internal error.");
		return -EFAULT;
	}

	return 0;
}

int nrd_matches_add_entry(struct nrd_strtab *strtab,
			  struct nrd_str *matchstr,
			  struct nrd_str *idstr,
			  size_t base_name_len,
			  char suffix,
			  const struct nrd_addrmap_entry *entry,
			  unsigned int flags)
{
	int err = 0;
	uint64_t mapidx1, mapidx2;

	if (NRD_USE_FULLNAME(flags)) {
		if (entry->dimensions == 0) {
			err = nrd_matches_add_str(strtab, matchstr, base_name_len, entry->name,
						  suffix, 0);
			if (err != 0)
				return err;
		} else {
			/* Reserve at least this much for nfp_resid calls */
			err = nrd_str_resize(idstr,
					     strlen(entry->name) +
						     (NRD_NUM64_DEC_DIGITS * entry->dimensions) +
						     1);
			if (err != 0)
				return err;

			if (NRD_ALL_REPEATS(flags)) {
				for (mapidx1 = 0; mapidx1 < entry->repeat1; mapidx1++)
					for (mapidx2 = 0; mapidx2 < entry->repeat2; mapidx2++) {
						sprintf(idstr->str, entry->name, (int)mapidx1,
							(int)mapidx2);
						err = nrd_matches_add_str(strtab, matchstr,
									  base_name_len, idstr->str,
									  suffix, 0);
						if (err != 0)
							return err;
					}
			} else {
				mapidx1 = 0;
				mapidx2 = 0;
				sprintf(idstr->str, entry->name, (int)mapidx1, (int)mapidx2);
				err = nrd_matches_add_str(strtab, matchstr, base_name_len,
							  idstr->str, suffix, 0);
				if (err != 0)
					return err;

				mapidx1 = entry->repeat1 - 1;
				mapidx2 = entry->repeat2 - 1;
				if ((mapidx1 > 0) || (mapidx2 > 0)) {
					sprintf(idstr->str, entry->name, (int)mapidx1,
						(int)mapidx2);
					err = nrd_matches_add_str(strtab, matchstr, base_name_len,
								  idstr->str, suffix, 0);
					if (err != 0)
						return err;
				}
			}
		}
	}

	if (NRD_USE_ALTNAME(flags) && (entry->altname)) {
		if (entry->dimensions == 0) {
			err = nrd_matches_add_str(strtab, matchstr, base_name_len, entry->altname,
						  suffix, 0);
			if (err != 0)
				return err;
		} else {
			/* Reserve at least this much for nfp_resid calls */
			err = nrd_str_resize(idstr,
					     strlen(entry->altname) +
						     (NRD_NUM64_DEC_DIGITS * entry->dimensions) +
						     1);
			if (err != 0)
				return err;

			if (NRD_ALL_REPEATS(flags)) {
				for (mapidx1 = 0; mapidx1 < entry->repeat1; mapidx1++)
					for (mapidx2 = 0; mapidx2 < entry->repeat2; mapidx2++) {
						sprintf(idstr->str, entry->altname, (int)mapidx1,
							(int)mapidx2);
						err = nrd_matches_add_str(strtab, matchstr,
									  base_name_len, idstr->str,
									  suffix, 0);
						if (err != 0)
							return err;
					}
			} else {
				mapidx1 = 0;
				mapidx2 = 0;
				sprintf(idstr->str, entry->altname, (int)mapidx1, (int)mapidx2);
				err = nrd_matches_add_str(strtab, matchstr, base_name_len,
							  idstr->str, suffix, 0);
				if (err != 0)
					return err;

				mapidx1 = entry->repeat1 - 1;
				mapidx2 = entry->repeat2 - 1;
				if ((mapidx1 > 0) || (mapidx2 > 0)) {
					sprintf(idstr->str, entry->altname, (int)mapidx1,
						(int)mapidx2);
					err = nrd_matches_add_str(strtab, matchstr, base_name_len,
								  idstr->str, suffix, 0);
					if (err != 0)
						return err;
				}
			}
		}
	}

	return 0;
}

int nrd_regs_lookup(struct nfp_regdata *regdata,
		    const char *name,
		    unsigned int flags,
		    struct nfp_regdata_node *node,
		    int local_island_id,
		    int local_menum)
{
	size_t idx, nm_idx;
	struct nrd_addrmap_entry *entry = NULL;
	const char *name_trailer = name;
	const char *names[2] = {NULL, NULL};
	uint64_t offset = 0;
	int err = 0;
	struct nrd_target *tgt = NULL;
	struct nrd_mereg *mereg = NULL;
	struct nrd_map_entry_find_info find_info = nrd_map_entry_find_info_zinit;
	struct nrd_mereg_find_info mefind_info = nrd_mereg_find_info_zinit;

	if (node) /* Clear first so error_cleanup doesn't segfault */
		memset(node, 0, sizeof(*node));

	if ((!regdata) || (!name) || (!node)) {
		NS_LOG(NS_LOG_LVL_FATAL, "Invalid parameters.");
		err = EINVAL;
		goto error_cleanup;
	}

	/* First check if we need to redirect to alias */
	if (strncmp(name, "_alias:", 7) == 0) {
		name_trailer = name + 7;
		for (idx = 0; idx < regdata->_aliases_cnt; idx++) {
			if (nrd_match_prefix(
				    regdata->_aliases[idx].alias, &name_trailer,
				    (NRD_IGN_CASE_FULLNAME(flags) || NRD_IGN_CASE_ALTNAME(flags)),
				    '.')) {
				if (*name_trailer == '\0')
					names[1] = NULL;
				else
					names[1] = name_trailer + 1;
				names[0] = regdata->_aliases[idx].fullname;
				break;
			}
		}
		if (!names[0]) {
			NS_LOG(NS_LOG_LVL_FATAL, "Lookup alias not found.");
			err = ENOENT;
			goto error_cleanup;
		}
	} else {
		names[0] = name;
	}

	/* First check for mereg */
	name_trailer = names[0];
	mefind_info.ovr_iid = local_island_id;
	mefind_info.ovr_menum = local_menum;

	if (regdata->arch == NFP_CHIP_ARCH_TH)
		mereg = nrd_mereg_find(regdata, &name_trailer, flags, &mefind_info);

	if (mefind_info.is_mereg) {
		if (!mefind_info.matched) {
			err = ENOENT;
			goto error_cleanup;
		}

		if (!nrd_tgt_iidme_valid(regdata, NULL, mefind_info.iid, mefind_info.menum)) {
			err = ENODEV;
			goto error_cleanup;
		}

		node->ctx = NFP_REGDATA_CTX_NA;
		node->reg_type = NFP_REG_TYPE_MEREG;
		node->offset_iid_lsb = -1;
		node->offset_me_lsb = -1;
		node->is_sim_backdoorable = 1;

		/* mereg:, mereg:iX, mereg:meY and mereg:iX.meY */
		/* will all have the same name and other info. */
		node->type = NFP_REGDATA_NODE_TYPE_TARGET;
		node->offset = UINT64_MAX; /* override later */
		node->cpp_tgt = -1;

		node->iid = mefind_info.iid;
		node->menum = mefind_info.menum;

		if (!mereg) {
			if (NRD_GET_FULLNAME(flags))
				node->name = strdup(NRD_MEREG_TGTNAME);
			if (NRD_GET_ALTNAME(flags))
				node->altname = strdup(NRD_MEREG_TGTNAME);
			if (NRD_GET_DESC(flags))
				node->description = strdup("Microengine Registers");
		} else {
			node->type = NFP_REGDATA_NODE_TYPE_REG;

			node->data.mereg.mereg_type = mereg->type;
			node->data.mereg.reg_idx = mefind_info.regidx;

			/* Not printing 64-bit values yet, */
			/* nothing has that many repeats yet. */
			if (NRD_GET_FULLNAME(flags) && (mereg->name)) {
				_MALLOC_OOM(node->name,
					    (strlen(mereg->name) + NRD_NUM64_DEC_DIGITS + 1));
				sprintf(node->name, mereg->name, (int)mefind_info.regidx);
			}

			if (NRD_GET_ALTNAME(flags) && (mereg->altname)) {
				_MALLOC_OOM(node->altname,
					    (strlen(mereg->altname) + NRD_NUM64_DEC_DIGITS + 1));
				sprintf(node->altname, mereg->altname, (int)mefind_info.regidx);
			}

			if (NRD_GET_DESC(flags) && (mereg->description)) {
				_MALLOC_OOM(node->description, (strlen(mereg->description) +
								NRD_NUM64_DEC_DIGITS + 1));
				sprintf(node->description, mereg->description,
					(int)mefind_info.regidx);
			}
			/* All NFP ME data registers are 32-bit */
			node->bit_len = 32;
			node->is_readable = 1;
			node->is_writable = 1;
			node->cpp_tgt = (int8_t)mereg->cpp_tgt;

			offset = 0;
			node->offset_iid_lsb = mereg->iid_lsb;
			node->offset_me_lsb = mereg->me_lsb;

			if ((mereg->iid_lsb != -1) && (mefind_info.iid >= 0)) {
				/* Even if the iid is 0, we still "set" it in the offset */
				/* and node reflects that. */
				offset |= mefind_info.iid << mereg->iid_lsb;
			}
			if ((mereg->me_lsb != -1) && (mefind_info.menum >= 0)) {
				/* NFP_CHIP_ARCH_TH specific */
				offset |= (mefind_info.menum + 4) << mereg->me_lsb;
			}

			if (mefind_info.regidx < 0) {
				err = ENODEV;
				goto error_cleanup;
			}

			if (mereg->byteaddr_lsb >= 0)
				offset |= (mefind_info.regidx * 4) << mereg->byteaddr_lsb;
			else
				offset |= (mefind_info.regidx * 4) >> (-mereg->byteaddr_lsb);

			node->cpp_rd_action = mereg->cpp_rd_action;
			node->cpp_rd_token = mereg->cpp_rd_token;
			node->cpp_rd_offset = offset + mereg->cpp_rd_offset;
			node->cpp_wr_action = mereg->cpp_wr_action;
			node->cpp_wr_token = mereg->cpp_wr_token;
			node->offset = offset + mereg->cpp_wr_offset;
		}

		goto success;
	}

	tgt = NULL;

	for (idx = 0; idx < regdata->tgt_coll.count; idx++) {
		tgt = &regdata->tgt_coll.entry[idx];
		find_info = nrd_map_entry_find_info_zinit;
		find_info.ovr_iid = local_island_id;
		find_info.ovr_menum = local_menum;

		entry = &tgt->map;
		for (nm_idx = 0; nm_idx < ARRAY_SIZE(names); nm_idx++) {
			const char *nm = names[nm_idx];

			if (!nm)
				break;
			name_trailer = nm;
			entry = nrd_map_entry_find(regdata, tgt, entry, &name_trailer, flags,
						   &offset, &find_info);
			if (!entry)
				break;
		}

		if (entry)
			break;
	}

	if (!entry) {
		err = ENOENT;
		goto error_cleanup;
	}

	if (entry->reg_type != NFP_REG_TYPE_NONE)
		find_info.reg_type = entry->reg_type;

	/* Handle offset iid/meid patching/extraction before */
	/* adding it to cpp_offsets */
	err = -nrd_entry_offset_patch_extract(regdata, tgt, entry, &find_info.iid, &find_info.menum,
					      &offset, find_info.saw_user_offset);
	if (err != 0)
		goto error_cleanup;

	node->iid = find_info.iid;
	node->menum = find_info.menum;
	node->ctx = find_info.ctx;
	node->reg_type = find_info.reg_type;

	node->offset_iid_lsb = tgt->offset_iid_lsb;
	node->offset_me_lsb = tgt->offset_me_lsb;

	node->is_sim_backdoorable = entry->is_sim_backdoorable;

	if (entry->type == NFP_REGDATA_NODE_TYPE_TARGET && !find_info.saw_user_offset) {
		node->type = NFP_REGDATA_NODE_TYPE_TARGET;
		if (NRD_GET_FULLNAME(flags))
			node->name = safe_strdup(entry->name);
		if (NRD_GET_ALTNAME(flags))
			node->altname = safe_strdup(entry->altname);
		if (NRD_GET_DESC(flags))
			node->description = safe_strdup(entry->description);
		node->offset = UINT64_MAX; /* override later */

		node->cpp_tgt = tgt->cpp_tgt;

		node->cpp_rd_action = tgt->cpp_rd_action;
		node->cpp_rd_token = tgt->cpp_rd_token;
		node->cpp_rd_offset = tgt->cpp_rd_offset + offset;

		node->cpp_wr_action = tgt->cpp_wr_action;
		node->cpp_wr_token = tgt->cpp_wr_token;
		node->offset = tgt->cpp_wr_offset + offset;

		nrd_update_reg_byte_offsets(regdata, entry, node);
	} else if ((entry->type == NFP_REGDATA_NODE_TYPE_REG) ||
		   (entry->type == NFP_REGDATA_NODE_TYPE_MULTIREG) || (find_info.saw_user_offset)) {
		if (find_info.saw_user_offset)
			node->type = NFP_REGDATA_NODE_TYPE_REG;
		else
			node->type = entry->type;

		if (NRD_GET_FULLNAME(flags) && (entry->name)) {
			_MALLOC_OOM(node->name, (strlen(entry->name) +
						 (NRD_NUM64_DEC_DIGITS * entry->dimensions) + 1));
			sprintf(node->name, entry->name, (int)find_info.mapidx1,
				(int)find_info.mapidx2);
		}

		if (NRD_GET_ALTNAME(flags) && (entry->altname)) {
			_MALLOC_OOM(node->altname,
				    (strlen(entry->altname) +
				     (NRD_NUM64_DEC_DIGITS * entry->dimensions) + 1));
			sprintf(node->altname, entry->altname, (int)find_info.mapidx1,
				(int)find_info.mapidx2);
		}

		if (NRD_GET_DESC(flags) && (entry->description)) {
			_MALLOC_OOM(node->description,
				    (strlen(entry->description) +
				     (NRD_NUM64_DEC_DIGITS * entry->dimensions) + 1));
			sprintf(node->description, entry->description, (int)find_info.mapidx1,
				(int)find_info.mapidx2);
		}

		node->offset = UINT64_MAX; /* override later */
		if (find_info.saw_user_offset) {
			node->bit_len = 0;
			node->fields = NULL;
			node->field_cnt = 0;
		} else {
			node->bit_len = entry->ptr.reg->bit_len;
			node->fields = (const struct nfp_reg_bitf *)entry->ptr.reg->fields;
			node->field_cnt = entry->ptr.reg->field_count;
		}
		node->matched_field = NULL;

		node->cpp_tgt = tgt->cpp_tgt;

		node->cpp_rd_action = tgt->cpp_rd_action;
		node->cpp_rd_token = tgt->cpp_rd_token;
		node->cpp_rd_offset = tgt->cpp_rd_offset + offset;

		node->cpp_wr_action = tgt->cpp_wr_action;
		node->cpp_wr_token = tgt->cpp_wr_token;
		node->offset = tgt->cpp_wr_offset + offset;

		if (find_info.saw_user_offset) {
			node->reg_byte_first = offset;
			node->reg_byte_last = offset;
		} else {
			nrd_update_reg_byte_offsets(regdata, entry, node);
		}

		switch (node->reg_type) {
		case NFP_REG_TYPE_CPP:
		case NFP_REG_TYPE_XPB:
			if (node->cpp_rd_offset != node->offset) {
				NS_LOG(NS_LOG_LVL_WARN,
				       "CPP/XPB Register with different read and write offset may not be handled correctly: %s",
				       name);
			}
			break;
		case NFP_REG_TYPE_MECSR:
			if (node->cpp_rd_offset != node->offset) {
				NS_LOG(NS_LOG_LVL_WARN,
				       "ME CSR with different read and write offset may not be handled correctly: %s",
				       name);
			}
			node->data.mecsr.latency_wr = entry->extra.mecsr.latency_wr;
			node->data.mecsr.latency_rd = entry->extra.mecsr.latency_rd;
			node->data.mecsr.latency_use = entry->extra.mecsr.latency_use;
			if (find_info.saw_user_offset)
				node->data.mecsr.offset = (uint16_t)find_info.user_offset;
			else
				node->data.mecsr.offset = (uint16_t)entry->offset;
			if (entry->extra.mecsr.is_indirect_ctx_csr) {
				if (node->ctx < 0)
					node->ctx = NFP_REGDATA_CTX_ANY;
			} else {
				if (node->ctx >= 0) {
					NS_LOG(NS_LOG_LVL_DEBUG,
					       "ME register '%s' is not a context indirect register.",
					       entry->name);
					err = ENOENT;
					goto error_cleanup;
				}
				node->ctx = NFP_REGDATA_CTX_NA;
			}
			break;
		default:
			NS_LOG(NS_LOG_LVL_FATAL, "Invalid register type for '%s'.", name);
			err = EINVAL;
			goto error_cleanup;
		}

		if (*name_trailer != '\0') {
			/* bit field */
			for (idx = 0; idx < node->field_cnt; idx++) {
				if (NRD_IGN_CASE_FULLNAME(flags)) {
					if (strcasecmp(node->fields[idx].name, name_trailer) == 0) {
						node->type = NFP_REGDATA_NODE_TYPE_BITF;
						node->matched_field = &node->fields[idx];
						break;
					}
				} else {
					if (strcmp(node->fields[idx].name, name_trailer) == 0) {
						node->type = NFP_REGDATA_NODE_TYPE_BITF;
						node->matched_field = &node->fields[idx];
						break;
					}
				}

				if (node->fields[idx].altname) {
					if (NRD_IGN_CASE_ALTNAME(flags)) {
						if (strcasecmp(node->fields[idx].altname,
							       name_trailer) == 0) {
							node->type = NFP_REGDATA_NODE_TYPE_BITF;
							node->matched_field = &node->fields[idx];
							break;
						}
					} else {
						if (strcmp(node->fields[idx].altname,
							   name_trailer) == 0) {
							node->type = NFP_REGDATA_NODE_TYPE_BITF;
							node->matched_field = &node->fields[idx];
							break;
						}
					}
				}
			}

			if (!node->matched_field) {
				/* Trailing tree name did not point to a bit field */
				err = ENOENT;
				goto error_cleanup;
			}

			nrd_update_accessmode(node, idx);
		} else {
			/* bit field */
			if (find_info.saw_user_offset) {
				node->is_readable = 1;
				node->is_writable = 1;
			} else {
				nrd_update_accessmode(node, node->field_cnt);
			}
		}
	} else if (entry->type == NFP_REGDATA_NODE_TYPE_ADDRMAP) {
		node->type = entry->type;

		if (NRD_GET_FULLNAME(flags) && (entry->name)) {
			_MALLOC_OOM(node->name, (strlen(entry->name) +
						 (NRD_NUM64_DEC_DIGITS * entry->dimensions) + 1));
			sprintf(node->name, entry->name, (int)find_info.mapidx1,
				(int)find_info.mapidx2);
		}

		if (NRD_GET_ALTNAME(flags) && (entry->altname)) {
			_MALLOC_OOM(node->altname,
				    (strlen(entry->altname) +
				     (NRD_NUM64_DEC_DIGITS * entry->dimensions) + 1));
			sprintf(node->altname, entry->altname, (int)find_info.mapidx1,
				(int)find_info.mapidx2);
		}

		if (NRD_GET_DESC(flags) && (entry->description)) {
			_MALLOC_OOM(node->description,
				    (strlen(entry->description) +
				     (NRD_NUM64_DEC_DIGITS * entry->dimensions) + 1));
			sprintf(node->description, entry->description, (int)find_info.mapidx1,
				(int)find_info.mapidx2);
		}
		node->offset = UINT64_MAX; /* override later */

		node->cpp_tgt = tgt->cpp_tgt;

		node->cpp_rd_action = tgt->cpp_rd_action;
		node->cpp_rd_token = tgt->cpp_rd_token;
		node->cpp_rd_offset = tgt->cpp_rd_offset + offset;

		node->cpp_wr_action = tgt->cpp_wr_action;
		node->cpp_wr_token = tgt->cpp_wr_token;
		node->offset = tgt->cpp_wr_offset + offset;

		nrd_update_reg_byte_offsets(regdata, entry, node);
	} else {
		err = EINVAL;
		goto error_cleanup;
	}

success:

	return 0;

error_cleanup:

	if (node)
		nfp_regdata_node_free_data(node);

	return NFP_ERRNO(err);
}

int nrd_discover_all(struct nfp_regdata *regdata,
		     unsigned int flags,
		     void *usrdata,
		     int (*cb_found_reg)(struct nfp_regdata *regdata,
					 struct nfp_regdata_node *entry,
					 void *usrdata),
		     struct nrd_target *tgt,
		     struct nrd_mapentry_node *this_node,
		     struct nrd_str *dynstr,
		     struct nrd_strtab *strtab)
{
	uint64_t mapidx1 = 0;
	uint64_t mapidx2 = 0;
	size_t idx;
	int iid, menum, ctx;
	int err = -EINVAL;

	if (nrd_addrmap_entry_ensure_ptr(regdata, this_node->this_entry) != 0)
		goto error_cleanup;

	switch (this_node->this_entry->type) {
	case NFP_REGDATA_NODE_TYPE_TARGET:
		switch (tgt->scope) {
		case NRD_TARGET_SCOPE_GLOBAL:
			for (idx = 0; idx < tgt->map.ptr.map->mapentries.count; idx++) {
				struct nrd_mapentry_node next_node = nrd_mapentry_node_zinit;

				next_node.parent = this_node;
				next_node.child = NULL;
				next_node.this_entry = &tgt->map.ptr.map->mapentries.entry[idx];
				next_node.is_virtual = 0;
				this_node->child = &next_node;

				for (mapidx1 = 0; mapidx1 < next_node.this_entry->repeat1;
				     mapidx1++)
					for (mapidx2 = 0; mapidx2 < next_node.this_entry->repeat2;
					     mapidx2++) {
						next_node.this_idx1 = mapidx1;
						next_node.this_idx2 = mapidx2;
						if (nrd_discover_all(regdata, flags, usrdata,
								     cb_found_reg, tgt, &next_node,
								     dynstr, strtab) != 0)
							goto error_cleanup;
					}
			}
			break;
		case NRD_TARGET_SCOPE_SINGLE_ISLAND:
			if (this_node->iid <= 0) {
				struct nrd_mapentry_node next_node = nrd_mapentry_node_zinit;

				if (!nfp_chipdata_island_has_cpp_target(
					    regdata->chip, tgt->single_island_iid, tgt->cpp_tgt))
					break;
				next_node.parent = this_node;
				next_node.child = NULL;
				next_node.this_entry = this_node->this_entry;
				next_node.iid = tgt->single_island_iid;
				next_node.is_virtual = 1;
				this_node->child = &next_node;
				if (nrd_discover_all(regdata, flags, usrdata, cb_found_reg, tgt,
						     &next_node, dynstr, strtab) != 0)
					goto error_cleanup;
			} else {
				for (idx = 0; idx < tgt->map.ptr.map->mapentries.count; idx++) {
					struct nrd_mapentry_node next_node =
						nrd_mapentry_node_zinit;

					next_node.parent = this_node;
					next_node.child = NULL;
					next_node.this_entry =
						&tgt->map.ptr.map->mapentries.entry[idx];
					next_node.is_virtual = 0;
					this_node->child = &next_node;
					for (mapidx1 = 0; mapidx1 < next_node.this_entry->repeat1;
					     mapidx1++)
						for (mapidx2 = 0;
						     mapidx2 < next_node.this_entry->repeat2;
						     mapidx2++) {
							next_node.this_idx1 = mapidx1;
							next_node.this_idx2 = mapidx2;
							if (nrd_discover_all(regdata, flags,
									     usrdata, cb_found_reg,
									     tgt, &next_node,
									     dynstr, strtab) != 0)
								goto error_cleanup;
						}
				}
			}
			break;
		case NRD_TARGET_SCOPE_ISLAND:
			if (this_node->iid <= 0) {
				for (iid = nfp_chipdata_island_id_first(regdata->chip); iid != -1;
				     iid = nfp_chipdata_island_id_next(regdata->chip, iid)) {
					struct nrd_mapentry_node next_node =
						nrd_mapentry_node_zinit;

					if (!nfp_chipdata_island_has_cpp_target(regdata->chip, iid,
										tgt->cpp_tgt))
						continue;
					next_node.parent = this_node;
					next_node.child = NULL;
					next_node.this_entry = this_node->this_entry;
					next_node.iid = iid;
					next_node.is_virtual = 1;
					this_node->child = &next_node;
					if (nrd_discover_all(regdata, flags, usrdata, cb_found_reg,
							     tgt, &next_node, dynstr, strtab) != 0)
						goto error_cleanup;
				}
			} else {
				for (idx = 0; idx < tgt->map.ptr.map->mapentries.count; idx++) {
					struct nrd_mapentry_node next_node =
						nrd_mapentry_node_zinit;

					next_node.parent = this_node;
					next_node.child = NULL;
					next_node.this_entry =
						&tgt->map.ptr.map->mapentries.entry[idx];
					this_node->child = &next_node;
					for (mapidx1 = 0; mapidx1 < next_node.this_entry->repeat1;
					     mapidx1++)
						for (mapidx2 = 0;
						     mapidx2 < next_node.this_entry->repeat2;
						     mapidx2++) {
							next_node.this_idx1 = mapidx1;
							next_node.this_idx2 = mapidx2;
							if (nrd_discover_all(regdata, flags,
									     usrdata, cb_found_reg,
									     tgt, &next_node,
									     dynstr, strtab) != 0)
								goto error_cleanup;
						}
				}
			}
			break;
		case NRD_TARGET_SCOPE_ME:
			if (this_node->iid <= 0) {
				for (iid = nfp_chipdata_island_id_first(regdata->chip); iid != -1;
				     iid = nfp_chipdata_island_id_next(regdata->chip, iid)) {
					struct nrd_mapentry_node next_node =
						nrd_mapentry_node_zinit;

					if (nfp_chipdata_island_me_count(regdata->chip, iid) == 0)
						continue;
					next_node.parent = this_node;
					next_node.child = NULL;
					next_node.this_entry = this_node->this_entry;
					next_node.iid = iid;
					next_node.is_virtual = 1;
					this_node->child = &next_node;
					if (nrd_discover_all(regdata, flags, usrdata, cb_found_reg,
							     tgt, &next_node, dynstr, strtab) != 0)
						goto error_cleanup;
				}
			} else if (this_node->menum < 0) {
				for (menum = 0; menum < (int)nfp_chipdata_island_me_count(
								regdata->chip, this_node->iid);
				     menum++) {
					struct nrd_mapentry_node next_node =
						nrd_mapentry_node_zinit;

					next_node.parent = this_node;
					next_node.child = NULL;
					next_node.this_entry = this_node->this_entry;
					next_node.iid = this_node->iid;
					next_node.menum = menum;
					next_node.is_virtual = 1;
					this_node->child = &next_node;
					if (nrd_discover_all(regdata, flags, usrdata, cb_found_reg,
							     tgt, &next_node, dynstr, strtab) != 0)
						goto error_cleanup;
				}
			} else {
				if (tgt->map.ptr.map == regdata->me_local_csr_map) {
					if (this_node->ctx < 0) {
						for (ctx = 0; ctx < NRD_MAX_CTX_CNT; ctx++) {
							struct nrd_mapentry_node next_node =
								nrd_mapentry_node_zinit;

							next_node.parent = this_node;
							next_node.child = NULL;
							next_node.this_entry =
								this_node->this_entry;
							next_node.iid = this_node->iid;
							next_node.menum = this_node->menum;
							next_node.ctx = ctx;
							next_node.is_virtual = 1;
							this_node->child = &next_node;
							if (nrd_discover_all(regdata, flags,
									     usrdata, cb_found_reg,
									     tgt, &next_node,
									     dynstr, strtab) != 0)
								goto error_cleanup;
						}
					}

					for (idx = 0; idx < tgt->map.ptr.map->mapentries.count;
					     idx++) {
						struct nrd_mapentry_node next_node =
							nrd_mapentry_node_zinit;

						next_node.parent = this_node;
						next_node.child = NULL;
						next_node.this_entry =
							&tgt->map.ptr.map->mapentries.entry[idx];
						next_node.is_virtual = 0;
						this_node->child = &next_node;
						if ((this_node->ctx >= 0) &&
						    (!next_node.this_entry->extra.mecsr
							      .is_indirect_ctx_csr))
							continue;
						for (mapidx1 = 0;
						     mapidx1 < next_node.this_entry->repeat1;
						     mapidx1++)
							for (mapidx2 = 0;
							     mapidx2 <
							     next_node.this_entry->repeat2;
							     mapidx2++) {
								next_node.this_idx1 = mapidx1;
								next_node.this_idx2 = mapidx2;
								if (nrd_discover_all(
									    regdata, flags, usrdata,
									    cb_found_reg, tgt,
									    &next_node, dynstr,
									    strtab) != 0)
									goto error_cleanup;
							}
					}
				} else {
					for (idx = 0; idx < tgt->map.ptr.map->mapentries.count;
					     idx++) {
						struct nrd_mapentry_node next_node =
							nrd_mapentry_node_zinit;

						next_node.parent = this_node;
						next_node.child = NULL;
						next_node.this_entry =
							&tgt->map.ptr.map->mapentries.entry[idx];
						next_node.is_virtual = 0;
						this_node->child = &next_node;
						for (mapidx1 = 0;
						     mapidx1 < next_node.this_entry->repeat1;
						     mapidx1++)
							for (mapidx2 = 0;
							     mapidx2 <
							     next_node.this_entry->repeat2;
							     mapidx2++) {
								next_node.this_idx1 = mapidx1;
								next_node.this_idx2 = mapidx2;
								if (nrd_discover_all(
									    regdata, flags, usrdata,
									    cb_found_reg, tgt,
									    &next_node, dynstr,
									    strtab) != 0)
									goto error_cleanup;
							}
					}
				}
			}
			break;
		default:
			break;
		}
		break;
	case NFP_REGDATA_NODE_TYPE_ADDRMAP:
		if (this_node->this_entry->ptr.map == regdata->me_local_csr_map) {
			if (this_node->ctx < 0) {
				for (ctx = 0; ctx < NRD_MAX_CTX_CNT; ctx++) {
					struct nrd_mapentry_node next_node =
						nrd_mapentry_node_zinit;

					next_node.parent = this_node;
					next_node.child = NULL;
					next_node.this_entry = this_node->this_entry;
					next_node.ctx = ctx;
					next_node.is_virtual = 1;
					this_node->child = &next_node;
					if (nrd_discover_all(regdata, flags, usrdata, cb_found_reg,
							     tgt, &next_node, dynstr, strtab) != 0)
						goto error_cleanup;
				}
			}

			for (idx = 0; idx < this_node->this_entry->ptr.map->mapentries.count;
			     idx++) {
				struct nrd_mapentry_node next_node = nrd_mapentry_node_zinit;

				next_node.parent = this_node;
				next_node.child = NULL;
				next_node.this_entry =
					&this_node->this_entry->ptr.map->mapentries.entry[idx];
				next_node.is_virtual = 0;
				this_node->child = &next_node;
				if ((this_node->ctx >= 0) &&
				    (!next_node.this_entry->extra.mecsr.is_indirect_ctx_csr))
					continue;
				for (mapidx1 = 0; mapidx1 < next_node.this_entry->repeat1;
				     mapidx1++)
					for (mapidx2 = 0; mapidx2 < next_node.this_entry->repeat2;
					     mapidx2++) {
						next_node.this_idx1 = mapidx1;
						next_node.this_idx2 = mapidx2;
						if (nrd_discover_all(regdata, flags, usrdata,
								     cb_found_reg, tgt, &next_node,
								     dynstr, strtab) != 0)
							goto error_cleanup;
					}
			}
		} else {
			for (idx = 0; idx < this_node->this_entry->ptr.map->mapentries.count;
			     idx++) {
				struct nrd_mapentry_node next_node = nrd_mapentry_node_zinit;

				next_node.parent = this_node;
				next_node.child = NULL;
				next_node.this_entry =
					&this_node->this_entry->ptr.map->mapentries.entry[idx];
				next_node.is_virtual = 0;
				this_node->child = &next_node;
				for (mapidx1 = 0; mapidx1 < next_node.this_entry->repeat1;
				     mapidx1++)
					for (mapidx2 = 0; mapidx2 < next_node.this_entry->repeat2;
					     mapidx2++) {
						next_node.this_idx1 = mapidx1;
						next_node.this_idx2 = mapidx2;
						if (nrd_discover_all(regdata, flags, usrdata,
								     cb_found_reg, tgt, &next_node,
								     dynstr, strtab) != 0)
							goto error_cleanup;
					}
			}
		}
		break;
	case NFP_REGDATA_NODE_TYPE_REG:
	case NFP_REGDATA_NODE_TYPE_MULTIREG: {
		struct nrd_mapentry_node *node;
		uint64_t offset = 0;
		char *cp;
		struct nfp_regdata_node node_out;
		struct nrd_addrmap_entry *entry = this_node->this_entry;

		strtab->offset = 0;
		strtab->str_cnt = 0;

		memset(&node_out, 0, sizeof(node_out));

		/* Find top parent */
		iid = -1;
		menum = -1;
		ctx = -1;
		for (node = this_node; node->parent; node = node->parent) {
			if (node->iid > 0)
				iid = node->iid;
			if (node->menum >= 0)
				menum = node->menum;
			if (node->ctx >= 0)
				ctx = node->ctx;
		}
		/* Now walk down, building fqn string and calculating offset */
		for (; node; node = node->child) {
			int addstr = 0;

			if (!node->is_virtual)
				offset += node->this_entry->offset +
					  (node->this_idx1 * node->this_entry->offinc1) +
					  (node->this_idx2 * node->this_entry->offinc2);

			if (NRD_GET_FQN(flags)) {
				if (node->ctx >= 0) {
					err = nrd_str_resize(dynstr,
							     strlen(node->this_entry->name) +
								     (2 * NRD_NUM64_DEC_DIGITS) +
								     1);
					if (err != 0)
						goto error_cleanup;
					sprintf(dynstr->str, "ctx%d", node->ctx);
					addstr = 1;
				} else if (node->menum >= 0) {
					if (nfp_menum2str(regdata->family, dynstr->str,
							  node->menum) == NULL) {
						NS_LOG(NS_LOG_LVL_FATAL,
						       "Could not build ID string.");
						goto error_cleanup;
					}
					addstr = 1;
				} else if (node->iid >= 0) {
					switch (tgt->scope) {
					case NRD_TARGET_SCOPE_ISLAND:
					case NRD_TARGET_SCOPE_ME:
						/* Not for single island scope */
						if (nfp_island2str(regdata->family, dynstr->str,
								   node->iid) == NULL) {
							NS_LOG(NS_LOG_LVL_FATAL,
							       "Could not build ID string.");
							goto error_cleanup;
						}
						addstr = 1;
						break;
					default:
						break;
					}
				} else {
					sprintf(dynstr->str, node->this_entry->name,
						(int)node->this_idx1, (int)node->this_idx2);
					addstr = 1;
				}

				if (addstr) {
					err = nrd_strtab_add(strtab, dynstr->str, 1, 0);
					if (err != 0)
						goto error_cleanup;
				}
			}
		}

		if (NRD_GET_FQN(flags)) {
			cp = strtab->str + strlen(strtab->str);
			*cp = ':';
			cp++;

			for (idx = 1; idx < strtab->str_cnt - 1; idx++) {
				cp += strlen(cp);
				*cp = '.';
				cp++;
			}

			node_out.fqn = strdup(strtab->str);
		}

		if (nrd_entry_offset_patch_extract(regdata, tgt, entry, &iid, &menum, &offset, 0) !=
		    0)
			break;

		node_out.type = entry->type;
		if (entry->reg_type != NFP_REG_TYPE_NONE)
			node_out.reg_type = entry->reg_type;
		else
			node_out.reg_type = tgt->reg_type;

		node_out.iid = iid;
		node_out.menum = menum;
		node_out.ctx = ctx;

		node_out.offset_iid_lsb = tgt->offset_iid_lsb;
		node_out.offset_me_lsb = tgt->offset_me_lsb;

		node_out.is_sim_backdoorable = entry->is_sim_backdoorable;

		if (NRD_GET_FULLNAME(flags) && (entry->name)) {
			_MALLOC_OOM(node_out.name,
				    (strlen(entry->name) +
				     (NRD_NUM64_DEC_DIGITS * entry->dimensions) + 1));
			sprintf(node_out.name, entry->name, (int)this_node->this_idx1,
				(int)this_node->this_idx2);
		}

		if (NRD_GET_ALTNAME(flags) && (entry->altname)) {
			_MALLOC_OOM(node_out.altname,
				    (strlen(entry->altname) +
				     (NRD_NUM64_DEC_DIGITS * entry->dimensions) + 1));
			sprintf(node_out.altname, entry->altname, (int)this_node->this_idx1,
				(int)this_node->this_idx2);
		}

		if (NRD_GET_DESC(flags) && (entry->description)) {
			_MALLOC_OOM(node_out.description,
				    (strlen(entry->description) +
				     (NRD_NUM64_DEC_DIGITS * entry->dimensions) + 1));
			sprintf(node_out.description, entry->description, (int)this_node->this_idx1,
				(int)this_node->this_idx2);
		}

		node_out.offset = UINT64_MAX; /* override later */
		node_out.bit_len = entry->ptr.reg->bit_len;
		node_out.fields = (const struct nfp_reg_bitf *)entry->ptr.reg->fields;
		node_out.field_cnt = entry->ptr.reg->field_count;
		node_out.matched_field = NULL;

		node_out.cpp_tgt = tgt->cpp_tgt;

		node_out.cpp_rd_action = tgt->cpp_rd_action;
		node_out.cpp_rd_token = tgt->cpp_rd_token;
		node_out.cpp_rd_offset = tgt->cpp_rd_offset + offset;

		node_out.cpp_wr_action = tgt->cpp_wr_action;
		node_out.cpp_wr_token = tgt->cpp_wr_token;
		node_out.offset = tgt->cpp_wr_offset + offset;

		nrd_update_accessmode(&node_out, node_out.field_cnt);
		nrd_update_reg_byte_offsets(regdata, entry, &node_out);

		switch (node_out.reg_type) {
		case NFP_REG_TYPE_CPP:
		case NFP_REG_TYPE_XPB:
			if (node_out.cpp_rd_offset != node_out.offset) {
				NS_LOG(NS_LOG_LVL_WARN,
				       "CPP/XPB Register with different read and write offset may not be handled correctly: %s",
				       entry->name);
			}
			break;
		case NFP_REG_TYPE_MECSR:
			if (node_out.cpp_rd_offset != node_out.offset) {
				NS_LOG(NS_LOG_LVL_WARN,
				       "ME CSR with different read and write offset may not be handled correctly: %s",
				       entry->name);
			}
			node_out.data.mecsr.latency_wr = entry->extra.mecsr.latency_wr;
			node_out.data.mecsr.latency_rd = entry->extra.mecsr.latency_rd;
			node_out.data.mecsr.latency_use = entry->extra.mecsr.latency_use;
			node_out.data.mecsr.offset = (uint16_t)entry->offset;
			if (entry->extra.mecsr.is_indirect_ctx_csr) {
				if (node_out.ctx < 0)
					node_out.ctx = NFP_REGDATA_CTX_ANY;
			} else {
				if (node_out.ctx >= 0) {
					NS_LOG(NS_LOG_LVL_DEBUG,
					       "ME register '%s' is not a context indirect register.",
					       entry->name);
					err = ENOENT;
					goto error_cleanup;
				}
				node_out.ctx = NFP_REGDATA_CTX_NA;
			}
			break;
		default:
			NS_LOG(NS_LOG_LVL_FATAL, "Invalid register type for '%s'.", entry->name);
			err = EINVAL;
			goto error_cleanup;
		}

		switch (cb_found_reg(regdata, &node_out, usrdata)) {
		case 0:
			nfp_regdata_node_free_data(&node_out);
			goto error_cleanup;
		case 1:
			/* callback took the strings */
			break;
		case 2:
			nfp_regdata_node_free_data(&node_out);
			break;
		default:
			nfp_regdata_node_free_data(&node_out);
			break;
		}
	} break;
	default:
		break;
	}

	return 0;

error_cleanup:
	return err;
}

int nrd_matches(struct nfp_regdata *regdata,
		const char *name,
		unsigned int flags,
		struct nrd_str *matchstr,
		struct nrd_str *idstr,
		struct nrd_strtab *strtab)
{
	size_t tgt_idx, e_idx, idx;
	const struct nrd_addrmap_entry *entry = NULL;
	const char *name_trailer = name;
	uint64_t offset = 0;
	int err = 0;
	const char *empty_str = "";
	struct nrd_map_entry_find_info find_info = nrd_map_entry_find_info_zinit;
	struct nrd_mereg_find_info mefind_info = nrd_mereg_find_info_zinit;
	struct nrd_target *tgt = NULL;
	struct nrd_mereg *mereg = NULL;
	int iid, menum;
	char suffix = '.';
	size_t name_len;
	char *c, *d, closer = ']';

	name_len = strlen(name);

	c = strchr(name, '[');
	d = strchr(name, '{');
	if ((c) && (d)) {
		NS_LOG(NS_LOG_LVL_ERROR, "Mixed group pattern format.");
		err = EINVAL;
		goto error_cleanup;
	}

	if (!c) {
		c = d;
		closer = '}';
	}

	if (c) {
		char *dup = strdup(name);
		char *out_start = strchr(dup, *c);
		char *grp_end = strchr(c, closer);
		char *feed;
		uint32_t v1, v2, vi;
		char *endp1, *endp2;

		if (grp_end++ == NULL) {
			NS_LOG(NS_LOG_LVL_ERROR, "Badly formatted group pattern.");
			err = EINVAL;
			free(dup);
			goto error_cleanup;
		}

		while ((*c != closer) && (*c != '\0')) {
			feed = out_start;
			c++;

			v1 = nfp_strtou32(c, &endp1, 10);
			/* We accept {a:b}, but it should really be [a:b]. No reason to be overly
			 * strict at this point.
			 */
			if ((endp1 > c) &&
			    ((endp1[0] == ':') || ((endp1[0] == '.') && (endp1[1] == '.')))) {
				endp1 += (endp1[0] == ':') ? 1 : 2;
				v2 = nfp_strtou32(endp1, &endp2, 10);
				if ((endp2 > endp1) &&
				    ((endp2[0] == ',') || (endp2[0] == closer))) {
					c = endp2;

					for (vi = v1; (v2 > v1) ? (vi <= v2) : (vi >= v2);
					     (v2 > v1) ? vi++ : vi--) {
						sprintf(feed, "%u%s", vi, grp_end);
						err = nrd_matches(regdata, dup, flags, matchstr,
								  idstr, strtab);
						if (err) {
							free(dup);
							goto error_cleanup;
						}

						if (v1 == v2)
							break;
					}

					continue;
				}
			}

			while ((*c != closer) && (*c != ',') && (*c != '\0')) {
				*feed++ = *c;
				c++;
			}

			if (*c == '\0') {
				NS_LOG(NS_LOG_LVL_ERROR, "Badly formatted group pattern.");
				err = EINVAL;
				free(dup);
				goto error_cleanup;
			}

			strcpy(feed, grp_end);
			err = nrd_matches(regdata, dup, flags, matchstr, idstr, strtab);
			if (err) {
				free(dup);
				goto error_cleanup;
			}
		}

		free(dup);
		return 0;
	}
	/* Reserve at least this much for nfp_resid calls */
	err = -nrd_str_resize(idstr, NFP_MEID_STR_SZ);
	if (err != 0)
		goto error_cleanup;

	/* Reserve at least this much, arbitrary magic number */
	/* doesn't matter much, size can grow at any time. */
	err = -nrd_str_resize(matchstr, name_len + 256);
	if (err != 0)
		goto error_cleanup;

	/* Add the base name to matchstr, no need to do that */
	/* for every match we add. */
	strcpy(matchstr->str, name);
	if ((name_len > 0) &&
	    ((matchstr->str[name_len - 1] == '.') || (matchstr->str[name_len - 1] == ':'))) {
		name_len--;
		matchstr->str[name_len] = '\0';
	}

	if (regdata->tgt_coll.count == 0)
		/* nothing will ever match, so go to end and return zero matches */
		goto success;

#define ADD_NAME(_nm_, _sfx_, _igd_)                                                             \
	do {                                                                                     \
		err = nrd_matches_add_str(strtab, matchstr, name_len, (_nm_), (_sfx_), (_igd_)); \
		if (err != 0) {                                                                  \
			goto error_cleanup;                                                      \
		}                                                                                \
	} while (0)

#define ADD_ENTRY_NAME(_ent_, _sfx_)                                                             \
	do {                                                                                     \
		err = nrd_matches_add_entry(strtab, matchstr, idstr, name_len, (_sfx_), (_ent_), \
					    flags);                                              \
		if (err != 0) {                                                                  \
			goto error_cleanup;                                                      \
		}                                                                                \
	} while (0)

	if (name[0] == '\0') {
		for (tgt_idx = 0; tgt_idx < regdata->tgt_coll.count; tgt_idx++) {
			char *end, *s;

			tgt = &regdata->tgt_coll.entry[tgt_idx];
			if (!nfp_chipdata_has_cpp_target(regdata->chip, tgt->cpp_tgt))
				continue;
			end = tgt->aliases.str + tgt->aliases.offset;

			/* First alias entry is normal name */
			if (NRD_USE_FULLNAME(flags))
				ADD_NAME(tgt->map.name, ':', 0);

			if (NRD_USE_ALTNAME(flags)) {
				for (s = tgt->aliases.str; s < end; s += strlen(s) + 1)
					ADD_NAME(s, ':', 0);
			}
		}

		name_trailer = NRD_MEREG_TGTNAME;
		ADD_NAME(name_trailer, ':', 0);

		goto success;
	}
	/* First check for mereg */
	name_trailer = name;
	if (regdata->arch == NFP_CHIP_ARCH_TH)
		mereg = nrd_mereg_find(regdata, &name_trailer, flags, &mefind_info);
	if (mefind_info.is_mereg) {
		if (!mefind_info.matched)
			goto success;

		if (!nrd_tgt_iidme_valid(regdata, NULL, mefind_info.iid, mefind_info.menum))
			goto success;

		if (NRD_MATCH_NODESCEND(flags)) {
			ADD_NAME("", suffix, 0);
			goto success;
		}

		if (mefind_info.lookup_depth == 1)
			suffix = ':';

		if (mefind_info.iid == NFP_REGDATA_IID_NA) {
			for (iid = nfp_chipdata_island_id_first(regdata->chip); iid != -1;
			     iid = nfp_chipdata_island_id_next(regdata->chip, iid)) {
				if (nfp_chipdata_island_me_count(regdata->chip, iid) == 0)
					continue;
				if (NRD_USE_FULLNAME(flags))
					ADD_NAME(nfp_island2str(regdata->family, idstr->str, iid),
						 suffix, 0);
				if (NRD_USE_ALTNAME(flags))
					ADD_NAME(
						nfp_island2altstr(regdata->family, idstr->str, iid),
						suffix, 0);
			}

			if (NRD_USE_FQN_ONLY(flags))
				goto success;

			if (NRD_USE_FULLNAME(flags))
				ADD_NAME(nfp_island2str(regdata->family, idstr->str, 0), suffix, 0);
		}

		if (mefind_info.menum == NFP_REGDATA_MENUM_NA) {
			int i;

			iid = mefind_info.iid;
			if (iid == NFP_REGDATA_IID_NA)
				iid = NFP_REGDATA_IID_LOCAL;

			/* mereg:iX - always only add ME numbers for this */
			menum = nfp_chipdata_island_me_count(regdata->chip, iid);
			for (i = 0; i < menum; i++) {
				if (NRD_USE_FULLNAME(flags) || NRD_USE_ALTNAME(flags)) {
					ADD_NAME(nfp_menum2str(regdata->family, idstr->str, i),
						 suffix, 0);
				}
			}
			if (mefind_info.iid != NFP_REGDATA_IID_NA)
				goto success;
			if (NRD_USE_FQN_ONLY(flags))
				goto success;
		}
		/* if ((mefind_info.menum >= 0) || */
		/* (!NRD_USE_ALTNAME(flags) && */
		/* (mefind_info.iid == NFP_REGDATA_IID_NA))) */
		if (!mereg) {
			for (e_idx = 0; e_idx < regdata->mereg_coll.count; e_idx++) {
				mereg = &regdata->mereg_coll.entry[e_idx];
				for (idx = 0; idx < (size_t)mereg->count; idx++) {
					if (NRD_USE_FULLNAME(flags)) {
						/* Reserve at least this much for nfp_resid calls */
						err = -nrd_str_resize(idstr,
								      strlen(mereg->name) +
									      NRD_NUM64_DEC_DIGITS +
									      1);
						if (err != 0)
							goto error_cleanup;
						/* Not printing 64-bit values yet, nothing has that
						 */
						/* many repeats yet. */
						sprintf(idstr->str, mereg->name, (int)idx);
						ADD_NAME(idstr->str, suffix, 0);
					}

					if (NRD_USE_ALTNAME(flags) && (mereg->altname)) {
						err = -nrd_str_resize(idstr,
								      strlen(mereg->altname) +
									      NRD_NUM64_DEC_DIGITS +
									      1);
						if (err != 0)
							goto error_cleanup;
						sprintf(idstr->str, mereg->altname, (int)idx);
						ADD_NAME(idstr->str, suffix, 0);
					}
				}
			}
			goto success;
		}

		if (NRD_USE_FULLNAME(flags) || NRD_USE_ALTNAME(flags))
			ADD_NAME(empty_str, suffix, 0);
		goto success;
	}

	for (tgt_idx = 0; tgt_idx < regdata->tgt_coll.count; tgt_idx++) {
		tgt = &regdata->tgt_coll.entry[tgt_idx];

		name_trailer = name;
		find_info = nrd_map_entry_find_info_zinit;

		entry = nrd_map_entry_find(regdata, tgt, &tgt->map, &name_trailer, flags, &offset,
					   &find_info);

		if (entry)
			break;
	}

	/* If it didn't match, return zero matches, successful. */
	if (!entry)
		goto success;

	/* tgt will be non-NULL, (regdata->tgt_coll.count == 0) checked earlier */

	err = -nrd_entry_offset_patch_extract(regdata, tgt, entry, &find_info.iid, &find_info.menum,
					      &offset, find_info.saw_user_offset);

	/* If no such island, or local island not allowed: */
	if (err == ENODEV)
		goto success;

	if (err != 0)
		goto error_cleanup;

	if ((entry->type == NFP_REGDATA_NODE_TYPE_TARGET) && (!find_info.saw_virtual_map))
		suffix = ':';

	if (find_info.saw_user_offset) {
		ADD_NAME("", suffix, 0);
		goto success;
	}

	switch (entry->type) {
	case NFP_REGDATA_NODE_TYPE_TARGET:
		switch (tgt->scope) {
		case NRD_TARGET_SCOPE_GLOBAL:
			if (NRD_MATCH_NODESCEND(flags)) {
				ADD_NAME("", suffix, 0);
				goto success;
			}

			if (find_info.iid == NFP_REGDATA_IID_NA) {
				for (e_idx = 0; e_idx < entry->ptr.map->mapentries.count; e_idx++) {
					struct nrd_addrmap_entry *eentry =
						&entry->ptr.map->mapentries.entry[e_idx];
					uint64_t eoffset = offset + eentry->offset;
					uint64_t mapidx1, mapidx2;

					if (nrd_addrmap_entry_ensure_ptr(regdata, eentry) != 0) {
						err = EINVAL;
						goto error_cleanup;
					}

					for (mapidx1 = 0; mapidx1 < eentry->repeat1; mapidx1++) {
						for (mapidx2 = 0; mapidx2 < eentry->repeat2;
						     mapidx2++) {
							uint64_t off = eoffset;
							int ii = find_info.iid;
							int mm = find_info.menum;

							off += (mapidx1 * eentry->offinc1);
							off += (mapidx2 * eentry->offinc2);
							if (nrd_entry_offset_patch_extract(
								    regdata, tgt, eentry, &ii, &mm,
								    &off, 0) != 0)
								continue;
							if (NRD_USE_FULLNAME(flags))
								ADD_ENTRY_NAME(eentry, suffix);

							if (NRD_USE_ALTNAME(flags)) {
								for (idx = 0;
								     idx < tgt->iid_remaps_cnt;
								     idx++) {
									if (tgt->iid_remaps[idx]
										    .iid == ii) {
										ADD_NAME(
											nfp_island2str(
												regdata->family,
												idstr->str,
												ii),
											suffix, 0);
										ADD_NAME(
											nfp_island2altstr(
												regdata->family,
												idstr->str,
												ii),
											suffix, 0);
										break;
									}
								}
							}
						}
					}
				}
			}

			if ((find_info.iid <= NFP_REGDATA_IID_LOCAL) &&
			    (!NRD_USE_FQN_ONLY(flags))) {
				/* xpb:submap, xpb:i0.submap */
				for (e_idx = 0; e_idx < entry->ptr.map->mapentries.count; e_idx++) {
					struct nrd_addrmap_entry *eentry =
						&entry->ptr.map->mapentries.entry[e_idx];
					uint64_t eoffset = offset + eentry->offset;
					int eiid = find_info.iid;
					int emenum = find_info.menum;

					if (nrd_addrmap_entry_ensure_ptr(regdata, eentry) != 0) {
						err = EINVAL;
						goto error_cleanup;
					}

					if (nrd_entry_offset_patch_extract(regdata, tgt, eentry,
									   &eiid, &emenum, &eoffset,
									   0) != 0)
						continue;

					for (idx = 0; idx < eentry->ptr.map->mapentries.count;
					     idx++) {
						const struct nrd_addrmap_entry *ientry =
							&eentry->ptr.map->mapentries.entry[idx];

						if (NRD_USE_FULLNAME(flags))
							ADD_NAME(ientry->name, suffix, 1);

						if (NRD_USE_ALTNAME(flags) && (ientry->altname))
							ADD_NAME(ientry->altname, suffix, 1);
					}
				}
			}
			break;
		case NRD_TARGET_SCOPE_SINGLE_ISLAND:
			if (NRD_MATCH_NODESCEND(flags)) {
				ADD_NAME("", suffix, 0);
				goto success;
			}

			for (e_idx = 0; e_idx < entry->ptr.map->mapentries.count; e_idx++) {
				const struct nrd_addrmap_entry *eentry =
					&entry->ptr.map->mapentries.entry[e_idx];
				uint64_t eoffset = offset + eentry->offset;
				int eiid = find_info.iid;
				int emenum = find_info.menum;

				if (nrd_entry_offset_patch_extract(regdata, tgt, eentry, &eiid,
								   &emenum, &eoffset, 0) != 0)
					continue;

				ADD_ENTRY_NAME(eentry, suffix);
			}
			break;
		case NRD_TARGET_SCOPE_ISLAND:
			if (NRD_MATCH_NODESCEND(flags)) {
				ADD_NAME("", suffix, 0);
				goto success;
			}

			if (find_info.iid == NFP_REGDATA_IID_NA) {
				for (iid = nfp_chipdata_island_id_first(regdata->chip); iid != -1;
				     iid = nfp_chipdata_island_id_next(regdata->chip, iid)) {
					if (!nfp_chipdata_island_has_cpp_target(regdata->chip, iid,
										tgt->cpp_tgt))
						continue;
					if (NRD_USE_FULLNAME(flags))
						ADD_NAME(nfp_island2str(regdata->family, idstr->str,
									iid),
							 suffix, 0);
					if (NRD_USE_ALTNAME(flags))
						ADD_NAME(nfp_island2altstr(regdata->family,
									   idstr->str, iid),
							 suffix, 0);
				}

				if (NRD_USE_FQN_ONLY(flags))
					break;

				ADD_NAME(nfp_island2str(regdata->family, idstr->str, 0), suffix, 0);
				find_info.iid = NFP_REGDATA_IID_LOCAL;
			} else if (find_info.iid == NFP_REGDATA_IID_LOCAL) {
				if (NRD_USE_FQN_ONLY(flags))
					break;
			}

			for (e_idx = 0; e_idx < entry->ptr.map->mapentries.count; e_idx++) {
				const struct nrd_addrmap_entry *eentry =
					&entry->ptr.map->mapentries.entry[e_idx];
				uint64_t eoffset = offset + eentry->offset;
				int eiid = find_info.iid;
				int emenum = find_info.menum;

				if (nrd_entry_offset_patch_extract(regdata, tgt, eentry, &eiid,
								   &emenum, &eoffset, 0) != 0)
					continue;

				ADD_ENTRY_NAME(eentry, suffix);
			}

			break;
		case NRD_TARGET_SCOPE_ME:
			if (NRD_MATCH_NODESCEND(flags)) {
				ADD_NAME("", suffix, 0);
				goto success;
			}

			if (find_info.iid == NFP_REGDATA_IID_NA) {
				if ((find_info.menum == NFP_REGDATA_MENUM_NA) &&
				    (find_info.ctx == NFP_REGDATA_CTX_NA)) {
					for (iid = nfp_chipdata_island_id_first(regdata->chip);
					     iid != -1; iid = nfp_chipdata_island_id_next(
								regdata->chip, iid)) {
						if (nfp_chipdata_island_me_count(regdata->chip,
										 iid) == 0)
							continue;
						if (NRD_USE_FULLNAME(flags))
							ADD_NAME(nfp_island2str(regdata->family,
										idstr->str, iid),
								 suffix, 0);
						if (NRD_USE_ALTNAME(flags))
							ADD_NAME(nfp_island2altstr(regdata->family,
										   idstr->str, iid),
								 suffix, 0);
					}

					if (!NRD_USE_FQN_ONLY(flags))
						ADD_NAME(nfp_island2str(regdata->family, idstr->str,
									0),
							 suffix, 0);
				}

				if (NRD_USE_FQN_ONLY(flags))
					break;

			} else if (find_info.iid == NFP_REGDATA_IID_LOCAL) {
				if (NRD_USE_FQN_ONLY(flags))
					break;
			}

			if ((find_info.menum == NFP_REGDATA_MENUM_NA) &&
			    (find_info.ctx == NFP_REGDATA_CTX_NA)) {
				int specified_iid = find_info.iid;

				if (find_info.iid == NFP_REGDATA_IID_NA)
					find_info.iid = NFP_REGDATA_IID_LOCAL;

				for (menum = 0; menum < (int)nfp_chipdata_island_me_count(
								regdata->chip, find_info.iid);
				     menum++) {
					ADD_NAME(nfp_menum2str(regdata->family, idstr->str, menum),
						 suffix, 0);
				}

				if (NRD_USE_FQN_ONLY(flags) ||
				    (specified_iid != NFP_REGDATA_IID_NA))
					break;

				find_info.menum = NFP_REGDATA_MENUM_LOCAL;
			}

			if (entry->ptr.map == regdata->me_local_csr_map) {
				if (find_info.ctx == NFP_REGDATA_CTX_NA) {
					for (e_idx = 0; e_idx < NRD_MAX_CTX_CNT; e_idx++) {
						err = -nrd_str_resize(idstr, 8);
						if (err != 0)
							goto error_cleanup;
						sprintf(idstr->str, "ctx%d", (int)e_idx);
						ADD_NAME(idstr->str, suffix, 0);
					}
				}

				for (e_idx = 0; e_idx < entry->ptr.map->mapentries.count; e_idx++) {
					const struct nrd_addrmap_entry *eentry =
						&entry->ptr.map->mapentries.entry[e_idx];
					uint64_t eoffset = offset + eentry->offset;
					int eiid = find_info.iid;
					int emenum = find_info.menum;

					if ((find_info.ctx >= 0) &&
					    (!eentry->extra.mecsr.is_indirect_ctx_csr))
						continue;

					if (nrd_entry_offset_patch_extract(regdata, tgt, eentry,
									   &eiid, &emenum, &eoffset,
									   0) != 0)
						continue;

					ADD_ENTRY_NAME(eentry, suffix);
				}
			} else {
				for (e_idx = 0; e_idx < entry->ptr.map->mapentries.count; e_idx++) {
					const struct nrd_addrmap_entry *eentry =
						&entry->ptr.map->mapentries.entry[e_idx];
					uint64_t eoffset = offset + eentry->offset;
					int eiid = find_info.iid;
					int emenum = find_info.menum;

					if (nrd_entry_offset_patch_extract(regdata, tgt, eentry,
									   &eiid, &emenum, &eoffset,
									   0) != 0)
						continue;

					ADD_ENTRY_NAME(eentry, suffix);
				}
			}
			break;
		default:
			break;
		}
		break;
	case NFP_REGDATA_NODE_TYPE_ADDRMAP:
		switch (tgt->scope) {
		case NRD_TARGET_SCOPE_GLOBAL:
			if (NRD_MATCH_NODESCEND(flags)) {
				ADD_NAME("", suffix, 0);
				goto success;
			}

			for (e_idx = 0; e_idx < entry->ptr.map->mapentries.count; e_idx++) {
				const struct nrd_addrmap_entry *eentry =
					&entry->ptr.map->mapentries.entry[e_idx];
				uint64_t eoffset = offset + eentry->offset;
				int eiid = find_info.iid;
				int emenum = find_info.menum;

				if (nrd_entry_offset_patch_extract(regdata, tgt, eentry, &eiid,
								   &emenum, &eoffset, 0) != 0)
					continue;

				ADD_ENTRY_NAME(eentry, suffix);
			}
			break;
		case NRD_TARGET_SCOPE_SINGLE_ISLAND:
		case NRD_TARGET_SCOPE_ISLAND:
			/* (find_info.iid > 0) == tgt:iX, X > 0 */
			/* (!NRD_USE_FQN_ONLY(flags)) == tgt:i0 */
			if (NRD_MATCH_NODESCEND(flags)) {
				ADD_NAME("", suffix, 0);
				goto success;
			}

			if ((find_info.iid > 0) || (!NRD_USE_FQN_ONLY(flags))) {
				for (e_idx = 0; e_idx < entry->ptr.map->mapentries.count; e_idx++) {
					const struct nrd_addrmap_entry *eentry =
						&entry->ptr.map->mapentries.entry[e_idx];
					uint64_t eoffset = offset + eentry->offset;
					int eiid = find_info.iid;
					int emenum = find_info.menum;

					if (nrd_entry_offset_patch_extract(regdata, tgt, eentry,
									   &eiid, &emenum, &eoffset,
									   0) != 0)
						continue;

					ADD_ENTRY_NAME(eentry, suffix);
				}
			}
			break;
		default:
			break;
		}

		break;
	case NFP_REGDATA_NODE_TYPE_MULTIREG:
	case NFP_REGDATA_NODE_TYPE_REG:

		if (*name_trailer == '\0') {
			if (NRD_MATCH_NODESCEND(flags)) {
				ADD_NAME("", suffix, 0);
				goto success;
			}

			if (NRD_USE_BITFIELDS(flags)) {
				for (e_idx = 0; e_idx < entry->ptr.reg->field_count; e_idx++) {
					if (NRD_USE_FULLNAME(flags))
						ADD_NAME(entry->ptr.reg->fields[e_idx].name, suffix,
							 0);

					if (NRD_USE_ALTNAME(flags) &&
					    (entry->ptr.reg->fields[e_idx].altname))
						ADD_NAME(entry->ptr.reg->fields[e_idx].altname,
							 suffix, 0);
				}
			} else {
				if (NRD_USE_FULLNAME(flags) || NRD_USE_ALTNAME(flags))
					ADD_NAME(empty_str, suffix, 0);
			}
		} else {
			for (e_idx = 0; e_idx < entry->ptr.reg->field_count; e_idx++) {
				if (NRD_USE_FULLNAME(flags)) {
					if (NRD_IGN_CASE_FULLNAME(flags)) {
						if (strcasecmp(entry->ptr.reg->fields[e_idx].name,
							       name_trailer) == 0)
							ADD_NAME(empty_str, suffix, 0);
					} else {
						if (strcmp(entry->ptr.reg->fields[e_idx].name,
							   name_trailer) == 0)
							ADD_NAME(empty_str, suffix, 0);
					}
				}

				if (NRD_USE_ALTNAME(flags) &&
				    (!!entry->ptr.reg->fields[e_idx].altname)) {
					if (NRD_IGN_CASE_ALTNAME(flags)) {
						if (strcasecmp(
							    entry->ptr.reg->fields[e_idx].altname,
							    name_trailer) == 0)
							ADD_NAME(empty_str, suffix, 0);
					} else {
						if (strcmp(entry->ptr.reg->fields[e_idx].altname,
							   name_trailer) == 0)
							ADD_NAME(empty_str, suffix, 0);
					}
				}
			}
		}
		break;

	default:
		break;
	}

#undef ADD_NAME

success:
	return 0;

error_cleanup:
	return err;
}

int nrd_find_reg_in_maps(struct nfp_regdata *regdata,
			 struct nrd_addrmap_entry **pmap_entry,
			 uint64_t offset,
			 uint64_t *pseen_offset,
			 uint64_t *pmapidx1,
			 uint64_t *pmapidx2,
			 struct nrd_strtab *strtab,
			 struct nrd_str *str)
{
	struct nrd_addrmap_entry *map_entry = *pmap_entry;
	uint64_t seen_offset = *pseen_offset;
	uint64_t mapidx1 = 0;
	uint64_t mapidx2 = 0;
	size_t idx;

	while ((map_entry) && (map_entry->type != NFP_REGDATA_NODE_TYPE_REG) &&
	       (map_entry->type != NFP_REGDATA_NODE_TYPE_MULTIREG)) {
		struct nrd_addrmap_entry *next_entry = NULL;
		uint64_t next_offset = seen_offset;

		if (nrd_addrmap_entry_ensure_ptr(regdata, map_entry) != 0)
			return -EINVAL;

		for (idx = 0; idx < map_entry->ptr.map->mapentries.count; idx++) {
			struct nrd_addrmap_entry *this_entry =
				&map_entry->ptr.map->mapentries.entry[idx];

			next_offset = seen_offset + this_entry->offset;

			if (next_offset > offset)
				break;

			next_entry = this_entry;
		}

		if (!next_entry)
			return 1;

		seen_offset += next_entry->offset;
		next_offset = seen_offset;

		switch (next_entry->dimensions) {
		case 0:
			if ((strtab) && (nrd_strtab_add(strtab, next_entry->name, 1, 0) != 0)) {
				NS_LOG(NS_LOG_LVL_ERROR, "Error building lookup string.");
				return -ENOMEM;
			}
			break;
		case 1:
			for (mapidx1 = 0; mapidx1 < next_entry->repeat1; mapidx1++) {
				if ((next_offset + next_entry->offinc1) > offset)
					break;
				next_offset += next_entry->offinc1;
			}
			if (mapidx1 == next_entry->repeat1)
				return 1;

			if (strtab) {
				if (nrd_str_resize(str,
						   strlen(next_entry->name) +
							   (NRD_NUM64_DEC_DIGITS *
							    next_entry->dimensions) +
							   1)) {
					NS_LOG(NS_LOG_LVL_ERROR, "Error building lookup string.");
					return -ENOMEM;
				}
				sprintf(str->str, next_entry->name, mapidx1);
				if (nrd_strtab_add(strtab, str->str, 1, 0) != 0) {
					NS_LOG(NS_LOG_LVL_ERROR, "Error building lookup string.");
					return -ENOMEM;
				}
			}
			break;
		case 2:
			for (mapidx1 = 0; mapidx1 < next_entry->repeat1; mapidx1++) {
				if ((next_offset + next_entry->offinc1) > offset)
					break;
				next_offset += next_entry->offinc1;
			}
			if (mapidx1 == next_entry->repeat1)
				return 1;

			for (mapidx2 = 0; mapidx2 < next_entry->repeat2; mapidx2++) {
				if ((next_offset + next_entry->offinc2) > offset)
					break;
				next_offset += next_entry->offinc2;
			}
			if (mapidx2 == next_entry->repeat2)
				return 1;

			if (strtab) {
				if (nrd_str_resize(str,
						   strlen(next_entry->name) +
							   (NRD_NUM64_DEC_DIGITS *
							    next_entry->dimensions) +
							   1)) {
					NS_LOG(NS_LOG_LVL_ERROR, "Error building lookup string.");
					return -ENOMEM;
				}
				sprintf(str->str, next_entry->name, (int)mapidx1, (int)mapidx2);
				if (nrd_strtab_add(strtab, str->str, 1, 0) != 0) {
					NS_LOG(NS_LOG_LVL_ERROR, "Error building lookup string.");
					return -ENOMEM;
				}
			}
			break;
		default:
			return 1;
		}

		seen_offset = next_offset;
		map_entry = next_entry;
	}

	if (nrd_addrmap_entry_ensure_ptr(regdata, map_entry) != 0)
		return -EINVAL;

	*pmap_entry = map_entry;
	*pseen_offset = seen_offset;
	*pmapidx1 = mapidx1;
	*pmapidx2 = mapidx2;
	return 0;
}

/* API implementation */

struct nfp_regdata *nfp_regdata_load(const struct nfp_chipdata_chip *chip,
				     int revision,
				     const char *regdata_path)
{
	json_t *j_root = NULL;
	json_t *j_top, *j_tmp, *j_loaded = NULL;
	struct nfp_regdata *data = NULL;
	const char *jstr;
	size_t sz, idx;
	uint64_t tmpu64;
	int err = EINVAL;
	struct nrd_strtab seen_names = nrd_strtab_zinit;
	int file_family = 0;

	if (!chip) {
		NS_LOG(NS_LOG_LVL_FATAL, "Invalid parameters.");
		return NFP_ERRPTR(EINVAL);
	}

	if (!nfp_chipdata_has_revision(chip, revision)) {
		NS_LOG(NS_LOG_LVL_FATAL, "Chip %s has no revision 0x%02X.",
		       nfp_chipdata_chip_id(chip), revision);
		return NFP_ERRPTR(ENODEV);
	}

	_CALLOC_OOM(data, 1, sizeof(struct nfp_regdata));

	data->chip = chip;
	data->arch = nfp_chipdata_chip_arch(chip);
	data->family = nfp_chipdata_chip_family(chip);
	data->rev = revision;
	data->empty_map = nrd_addrmap_collection_new_map(data);
	_OOM_CHECK(data->empty_map);
	_STRDUP_OOM(data->empty_map->id, "");

	if (!regdata_path)
		regdata_path = getenv(NFP_REGDATA_PATH_ENVVAR);

	if (regdata_path) {
		char s[4];

		if (!nfp_rev2str(s, revision)) {
			NS_LOG(NS_LOG_LVL_FATAL, "Invalid revision: 0x%X", revision);
			goto error_cleanup;
		}
		s[0] = (char)tolower(s[0]);
		s[1] = (char)tolower(s[1]);

		sz = strlen(regdata_path);
		_MALLOC_OOM(data->json_dir, (sz + 1 + 2 + 1 + 1));

		strcpy(data->json_dir, regdata_path);
		if (data->json_dir[sz - 1] != PATH_SEP_C) {
			data->json_dir[sz++] = PATH_SEP_C;
			data->json_dir[sz++] = '\0';
		}
		strcat(data->json_dir, s);
		strcat(data->json_dir, PATH_SEP_S);
	}

	j_loaded = nrd_json_file_load(data, NFP_REGDATA_FILE_NAME_TOP, 1);
	if (!j_loaded)
		goto error_cleanup;
	j_root = j_loaded;

	j_top = json_object_get(j_root, "_comment");
	if (json_is_array(j_top)) {
		size_t sz = 32;
		char s[32];
		char r[4];

		strcpy(s, "Chip revision: ");
		if (!!nfp_rev2str(r, revision))
			strcat(s, r);
		else
			strcat(s, "?");
		strcat(s, "\n");

		for (idx = 0; idx < json_array_size(j_top); idx++) {
			jstr = json_string_value(json_array_get(j_top, idx));
			if (!jstr)
				continue;
			sz += strlen(jstr) + 1;
		}
		sz++;
		_CALLOC_OOM(data->json_top_comment, sz, 1);
		for (idx = 0; idx < json_array_size(j_top); idx++) {
			jstr = json_string_value(json_array_get(j_top, idx));
			if (!jstr)
				continue;
			strcat(data->json_top_comment, jstr);
			strcat(data->json_top_comment, "\n");
		}
		strcat(data->json_top_comment, s);
	} else {
		_CALLOC_OOM(data->json_top_comment, 1, 1);
	}

	/* Load some stuff from info */
	j_top = json_object_get(j_root, "info");
	jstr = json_string_value(json_object_get(j_top, "me_csrs_id"));
	if (!jstr) {
		NS_LOG(NS_LOG_LVL_FATAL, "No valid info.me_csrs_id specified");
		goto error_cleanup;
	}
	data->me_local_csr_map_id = strdup(jstr);

	/* _aliases is a more generic version of "me_csrs_id" where */
	/* the SDK can keep the shortcuts/aliases constant in case the */
	/* register map FQN ever changes. */
	j_tmp = json_object_get(j_top, "_aliases");
	data->_aliases_cnt = 0;
	_CALLOC_OOM(data->_aliases, json_object_size(j_tmp), sizeof(*data->_aliases));

	{
		const char *alias_name;
		json_t *j_fqn;
		const char *alias_fqn;

		json_object_foreach(j_tmp, alias_name, j_fqn) {
			alias_fqn = json_string_value(j_fqn);
			if (!alias_fqn)
				continue;
			data->_aliases[data->_aliases_cnt].alias = safe_strdup(alias_name);
			data->_aliases[data->_aliases_cnt].fullname = safe_strdup(alias_fqn);
			if ((!data->_aliases[data->_aliases_cnt].alias) ||
			    (!data->_aliases[data->_aliases_cnt].fullname))
				goto error_cleanup;
			data->_aliases_cnt++;
		}
	}

	j_tmp = json_object_get(j_top, "xpb_targets");
	if (!json_is_array(j_tmp)) {
		NS_LOG(NS_LOG_LVL_FATAL, "No valid info.xpb_targets specified");
		goto error_cleanup;
	}

	for (idx = 0; idx < json_array_size(j_tmp); idx++) {
		jstr = json_string_value(json_array_get(j_tmp, idx));
		if (!jstr) {
			NS_LOG(NS_LOG_LVL_FATAL, "No valid info.xpb_targets specified");
			goto error_cleanup;
		}
		if (nrd_strtab_add(&data->xpb_targets, jstr, 0, 1) != 0) {
			NS_LOG(NS_LOG_LVL_FATAL, "Invalid entry in info.xpb_targets specified");
			goto error_cleanup;
		}
	}

	jstr = json_string_value(json_object_get(j_top, "chip_family"));
	file_family = nfp_chipdata_str2family(jstr);

	if (file_family == 0) {
		NS_LOG(NS_LOG_LVL_FATAL, "No valid info.chip_family specified");
		goto error_cleanup;
	}

	if ((data->chip) && (file_family != data->family)) {
		NS_LOG(NS_LOG_LVL_FATAL, "Target chip family does not match info.chip_family");
		goto error_cleanup;
	}

	/* Now load targets as top level maps */
	{
		const char *target_name;
		json_t *j_target;
		struct nrd_target *tgt;
		struct nrd_target *tgt_prev;

		j_top = json_object_get(j_root, "targets");
		json_object_foreach(j_top, target_name, j_target) {
			tgt = nrd_target_collection_new_target(data);
			_OOM_CHECK(tgt);

			if (nrd_strtab_find(&data->xpb_targets, target_name))
				tgt->reg_type = NFP_REG_TYPE_XPB;
			else
				tgt->reg_type = NFP_REG_TYPE_CPP;

			switch (nrd_j_obj_strnum_get(j_target, "offset_iid_lsb", &tmpu64)) {
			case 0:
				tgt->offset_iid_lsb = (uint8_t)tmpu64;
				break;
			case -ENOENT:
				tgt->offset_iid_lsb = -1;
				break;
			default:
				NS_LOG(NS_LOG_LVL_FATAL,
				       "Bad offset_iid_lsb for top level target %s", target_name);
				goto error_cleanup;
			}

			switch (nrd_j_obj_strnum_get(j_target, "offset_me_lsb", &tmpu64)) {
			case 0:
				tgt->offset_me_lsb = (uint8_t)tmpu64;
				break;
			case -ENOENT:
				tgt->offset_me_lsb = -1;
				break;
			default:
				NS_LOG(NS_LOG_LVL_FATAL,
				       "Bad offset_me_lsb for top level target %s", target_name);
				goto error_cleanup;
			}

			if (nrd_j_obj_strnum_get(j_target, "cpp_tgt", &tmpu64) != 0) {
				NS_LOG(NS_LOG_LVL_FATAL, "Bad cpp_tgt for top level target %s",
				       target_name);
				goto error_cleanup;
			}
			tgt->cpp_tgt = (int8_t)tmpu64;

			j_tmp = json_object_get(j_target, "cpp_read");
			if (nrd_j_obj_strnum_get(j_tmp, "action", &tmpu64) != 0) {
				NS_LOG(NS_LOG_LVL_FATAL,
				       "Bad cpp_read.action for top level target %s", target_name);
				goto error_cleanup;
			}
			tgt->cpp_rd_action = (int8_t)tmpu64;

			if (nrd_j_obj_strnum_get(j_tmp, "token", &tmpu64) != 0) {
				NS_LOG(NS_LOG_LVL_FATAL,
				       "Bad cpp_read.token for top level target %s", target_name);
				goto error_cleanup;
			}
			tgt->cpp_rd_token = (int8_t)tmpu64;

			if (nrd_j_obj_strnum_get(j_tmp, "offset", &tgt->cpp_rd_offset) == -EINVAL) {
				NS_LOG(NS_LOG_LVL_FATAL,
				       "Bad cpp_read.offset for top level target %s", target_name);
				goto error_cleanup;
			}

			j_tmp = json_object_get(j_target, "cpp_write");
			if (nrd_j_obj_strnum_get(j_tmp, "action", &tmpu64) != 0) {
				NS_LOG(NS_LOG_LVL_FATAL,
				       "Bad cpp_write.action for top level target %s", target_name);
				goto error_cleanup;
			}
			tgt->cpp_wr_action = (int8_t)tmpu64;

			if (nrd_j_obj_strnum_get(j_tmp, "token", &tmpu64) != 0) {
				NS_LOG(NS_LOG_LVL_FATAL,
				       "Bad cpp_write.token for top level target %s", target_name);
				goto error_cleanup;
			}
			tgt->cpp_wr_token = (int8_t)tmpu64;

			if (nrd_j_obj_strnum_get(j_tmp, "offset", &tgt->cpp_wr_offset) == -EINVAL) {
				NS_LOG(NS_LOG_LVL_FATAL,
				       "Bad cpp_write.offset for top level target %s", target_name);
				goto error_cleanup;
			}

			if (nrd_j_obj_strnum_get(j_target, "offset_cmp_mask",
						 &tgt->offset_cmp_mask) == -EINVAL) {
				NS_LOG(NS_LOG_LVL_FATAL,
				       "Bad offset_cmp_mask for top level target %s", target_name);
				goto error_cleanup;
			}

			tmpu64 = 0;
			if (nrd_j_obj_strnum_get(j_target, "byteaddr_lsb", &tmpu64) == -EINVAL) {
				NS_LOG(NS_LOG_LVL_FATAL, "Bad byteaddr_lsb for top level target %s",
				       target_name);
				goto error_cleanup;
			}
			tgt->byteaddr_lsb = (int)tmpu64;

			tgt->map.name = strdup(target_name);
			tgt->map.type = NFP_REGDATA_NODE_TYPE_TARGET;

			jstr = json_string_value(json_object_get(j_target, "ptr"));
			if (!jstr) {
				NS_LOG(NS_LOG_LVL_FATAL, "Bad ptr for top level target %s",
				       target_name);
				goto error_cleanup;
			}
			tgt->map.ptr_id = strdup(jstr);

			if (strcmp(tgt->map.ptr_id, data->me_local_csr_map_id) == 0)
				tgt->reg_type = NFP_REG_TYPE_MECSR;

			jstr = json_string_value(json_object_get(j_target, "scope"));
			tgt->scope = nrd_enum_from_string(nrd_tgtscope_string_map, jstr);
			if (tgt->scope == NRD_TARGET_SCOPE_NONE) {
				NS_LOG(NS_LOG_LVL_FATAL, "Bad scope for top level target %s",
				       target_name);
				goto error_cleanup;
			}
			/* Can resolve this only when loading for chip */
			tgt->single_island_iid = NFP_REGDATA_IID_NA;

			if (nrd_j_obj_strnum_get(j_target, "offset", &tmpu64) != 0)
				tgt->map.offset = 0;
			else
				tgt->map.offset = tmpu64;

			if (data->json_dir) {
				if (nrd_addrmap_entry_ensure_ptr(data, &tgt->map) != 0)
					goto error_cleanup;
			} else {
				nrd_resolve_ptrs(data, 0);
			}
		}

		/* Quick sanity check on XPBs */
		tgt_prev = NULL;
		for (idx = 0; idx < data->tgt_coll.count; idx++) {
			tgt = &data->tgt_coll.entry[idx];
			if (tgt->reg_type != NFP_REG_TYPE_XPB)
				continue;
			if (!tgt_prev) {
				tgt_prev = tgt;
			} else {
				if ((tgt_prev->cpp_tgt != tgt->cpp_tgt) ||
				    (tgt_prev->cpp_rd_action != tgt->cpp_rd_action) ||
				    (tgt_prev->cpp_rd_token != tgt->cpp_rd_token) ||
				    (tgt_prev->cpp_wr_action != tgt->cpp_wr_action) ||
				    (tgt_prev->cpp_wr_token != tgt->cpp_wr_token) ||
				    (tgt_prev->scope != tgt->scope)) {
					NS_LOG(NS_LOG_LVL_FATAL,
					       "All XPB lookup targets do not have the same CPP access details.");
					goto error_cleanup;
				}
			}
		}
	}

	/* Now load target aliases */
	{
		const char *alias;
		json_t *j_tgt;

		j_top = json_object_get(json_object_get(j_root, "info"), "target_aliases");
		json_object_foreach(j_top, alias, j_tgt) {
			struct nrd_target *tgt;
			const char *tgtname = json_string_value(j_tgt);

			if (!tgtname) {
				NS_LOG(NS_LOG_LVL_FATAL, "Bad value for top info.target_aliases.%s",
				       tgtname);
				goto error_cleanup;
			}

			tgt = NULL;
			for (idx = 0; idx < data->tgt_coll.count; idx++) {
				tgt = &data->tgt_coll.entry[idx];
				if (strcmp(tgt->map.name, tgtname) == 0)
					break;
			}

			if (!tgt) {
				NS_LOG(NS_LOG_LVL_FATAL,
				       "Non-existent alias for top info.target_aliases.%s",
				       tgtname);
				goto error_cleanup;
			}

			err = nrd_strtab_add(&tgt->aliases, alias, 0, 1);
			switch (err) {
			case -ENOMEM:
				NS_LOG(NS_LOG_LVL_FATAL, "Out of memory.");
				err = ENOMEM;
				goto error_cleanup;
			case 0:
				break;
			case 1:
				NS_LOG(NS_LOG_LVL_FATAL,
				       "Non-existent alias info.target_aliases.%s has case-insensitive duplicate.",
				       tgtname);
				err = EINVAL;
				break;
			default:
				NS_LOG(NS_LOG_LVL_FATAL, "Internal error.");
				err = EFAULT;
				goto error_cleanup;
			}
		}
	}

	/* Now load target island remappings */
	{
		const char *tgtname;
		const char *iidname;
		json_t *j_remaps;
		json_t *j_remap;

		j_top = json_object_get(json_object_get(j_root, "info"), "islandmap_aliases");
		json_object_foreach(j_top, tgtname, j_remaps) {
			struct nrd_target *tgt;

			tgt = NULL;
			for (idx = 0; idx < data->tgt_coll.count; idx++) {
				tgt = &data->tgt_coll.entry[idx];
				if (strcmp(tgt->map.name, tgtname) == 0)
					break;
			}

			if (!tgt) {
				NS_LOG(NS_LOG_LVL_FATAL,
				       "Non-existent target info.islandmap_aliases.%s", tgtname);
				goto error_cleanup;
			}

			if (tgt->scope != NRD_TARGET_SCOPE_GLOBAL) {
				NS_LOG(NS_LOG_LVL_FATAL,
				       "Target specified by info.islandmap_aliases.%s is not a global scope target.",
				       tgtname);
				goto error_cleanup;
			}

			tgt->iid_remaps_cnt = 0;
			_CALLOC_OOM(tgt->iid_remaps, json_object_size(j_remaps),
				    sizeof(*tgt->iid_remaps));
			json_object_foreach(j_remaps, iidname, j_remap) {
				int iid = nfp_idstr2island(data->family, iidname, NULL);
				const char *mapname = json_string_value(j_remap);

				if (iid == -1) {
					NS_LOG(NS_LOG_LVL_FATAL,
					       "Bad island ID for info.islandmap_aliases.%s.%s",
					       tgtname, iidname);
					goto error_cleanup;
				}

				if (!mapname) {
					NS_LOG(NS_LOG_LVL_FATAL,
					       "Bad island map alias for info.islandmap_aliases.%s.%s",
					       tgtname, iidname);
					goto error_cleanup;
				}

				tgt->iid_remaps[tgt->iid_remaps_cnt].iid = iid;
				tgt->iid_remaps[tgt->iid_remaps_cnt].map = NULL;

				if (nrd_addrmap_entry_ensure_ptr(data, &tgt->map) != 0)
					goto error_cleanup;

				for (idx = 0; idx < tgt->map.ptr.map->mapentries.count; idx++) {
					uint64_t mapidx1, mapidx2;
					struct nrd_addrmap_entry *entry =
						&tgt->map.ptr.map->mapentries.entry[idx];
					const char *c = mapname;

					if (nrd_match_indexes(entry->name, &c, 0, '\0', &mapidx1,
							      &mapidx2, entry->repeat1,
							      entry->repeat2)) {
						tgt->iid_remaps[tgt->iid_remaps_cnt].map = entry;
						tgt->iid_remaps[tgt->iid_remaps_cnt].mapidx1 =
							mapidx1;
						tgt->iid_remaps[tgt->iid_remaps_cnt].mapidx2 =
							mapidx2;
						break;
					}
				}

				if (!tgt->iid_remaps[tgt->iid_remaps_cnt].map) {
					NS_LOG(NS_LOG_LVL_FATAL,
					       "Map %s does not exist, info.islandmap_aliases.%s.%s",
					       mapname, tgtname, iidname);
					goto error_cleanup;
				}

				tgt->iid_remaps_cnt++;
			}
		}
	}

	/* Now load mereg special info */
	{
		const char *mereg_id;
		json_t *j_mereg;

		j_top = json_object_get(j_root, NRD_MEREG_TGTNAME);

		/* Reset the strtab */
		if (seen_names.str)
			free(seen_names.str);
		seen_names.str = NULL;
		seen_names.offset = 0;
		seen_names.alloced = 0;

		json_object_foreach(j_top, mereg_id, j_mereg) {
			struct nrd_mereg *mereg;

			mereg = nrd_mereg_collection_new_mereg(data);
			if (!mereg) {
				err = ENOMEM;
				NS_LOG(NS_LOG_LVL_FATAL, "Out of memory.");
				goto error_cleanup;
			}

			mereg->type = nrd_enum_from_string(nrd_mereg_string_map, mereg_id);
			if (mereg->type == NFP_MEREG_TYPE_NONE) {
				NS_LOG(NS_LOG_LVL_FATAL,
				       "Invalid mereg type: " NRD_MEREG_TGTNAME ":%s", mereg_id);
				goto error_cleanup;
			}

			mereg->name =
				safe_strdup(json_string_value(json_object_get(j_mereg, "name")));
			if (!mereg->name) {
				NS_LOG(NS_LOG_LVL_FATAL,
				       "No name for register " NRD_MEREG_TGTNAME ":%s", mereg_id);
				goto error_cleanup;
			}
			switch (err = nrd_strtab_add(&seen_names, mereg->name, 0, 0)) {
			case 0:
				break;
			case 1:
				NS_LOG(NS_LOG_LVL_WARN,
				       "Duplicate register name " NRD_MEREG_TGTNAME ".%s.%s",
				       mereg_id, mereg->name);
				err = EINVAL;
				break;
			default:
				goto error_cleanup;
			}

			mereg->altname =
				safe_strdup(json_string_value(json_object_get(j_mereg, "altname")));
			if (!mereg->altname) {
				NS_LOG(NS_LOG_LVL_FATAL,
				       "No altname for register " NRD_MEREG_TGTNAME ":%s",
				       mereg_id);
				goto error_cleanup;
			}

			if (strcasecmp(mereg->altname, mereg->name) != 0) {
				switch (err = nrd_strtab_add(&seen_names, mereg->altname, 0, 0)) {
				case 0:
					break;
				case 1:
					err = EINVAL;
					NS_LOG(NS_LOG_LVL_WARN,
					       "Duplicate register name " NRD_MEREG_TGTNAME
					       ".%s.%s",
					       mereg_id, mereg->altname);
					break;
				default:
					goto error_cleanup;
				}
			}

			mereg->description = safe_strdup(
				json_string_value(json_object_get(j_mereg, "description")));
			if (!mereg->description) {
				NS_LOG(NS_LOG_LVL_FATAL,
				       "No description for register " NRD_MEREG_TGTNAME ":%s",
				       mereg_id);
				goto error_cleanup;
			}

			if (nrd_j_obj_strnum_get(j_mereg, "cpp_tgt", &tmpu64) != 0) {
				NS_LOG(NS_LOG_LVL_FATAL,
				       "Bad cpp_tgt for register " NRD_MEREG_TGTNAME ":%s",
				       mereg_id);
				goto error_cleanup;
			}
			mereg->cpp_tgt = (int)tmpu64;

			if (nrd_j_obj_strnum_get(j_mereg, "offset_iid_lsb", &tmpu64) == 0)
				mereg->iid_lsb = (int)tmpu64;
			else
				mereg->iid_lsb = -1;

			if (nrd_j_obj_strnum_get(j_mereg, "offset_me_lsb", &tmpu64) != 0) {
				NS_LOG(NS_LOG_LVL_FATAL,
				       "Bad me_lsb for register " NRD_MEREG_TGTNAME ":%s",
				       mereg_id);
				goto error_cleanup;
			}
			mereg->me_lsb = (int)tmpu64;

			if (nrd_j_obj_strnum_get(j_mereg, "byteaddr_lsb", &tmpu64) != 0) {
				NS_LOG(NS_LOG_LVL_FATAL,
				       "Bad byteaddr_lsb for " NRD_MEREG_TGTNAME ":%s", mereg_id);
				goto error_cleanup;
			}
			mereg->byteaddr_lsb = (int)tmpu64;

			if (nrd_j_obj_strnum_get(j_mereg, "offset_cmp_mask", &tmpu64) != 0) {
				NS_LOG(NS_LOG_LVL_FATAL,
				       "Bad offset_cmp_mask for " NRD_MEREG_TGTNAME ":%s",
				       mereg_id);
				goto error_cleanup;
			}
			mereg->offset_cmp_mask = tmpu64;

			if (nrd_j_obj_strnum_get(j_mereg, "count", &tmpu64) != 0) {
				NS_LOG(NS_LOG_LVL_FATAL, "Bad count for " NRD_MEREG_TGTNAME ":%s",
				       mereg_id);
				goto error_cleanup;
			}
			mereg->count = (uint32_t)tmpu64;

			/* We calculate the bit mask for count so we can use it later */
			/* to extract the register index safely. */
			/* First round up to the next highest power of 2, since we */
			/* will need at least that many bits to hold the value. */
			/* http://graphics.stanford.edu/~seander/ */
			/* bithacks.html#RoundUpPowerOf2 */
			mereg->count_mask = mereg->count - 1;
			mereg->count_mask |= mereg->count_mask >> 1;
			mereg->count_mask |= mereg->count_mask >> 2;
			mereg->count_mask |= mereg->count_mask >> 4;
			mereg->count_mask |= mereg->count_mask >> 8;
			mereg->count_mask |= mereg->count_mask >> 16;

			j_tmp = json_object_get(j_mereg, "cpp_read");
			if (nrd_j_obj_strnum_get(j_tmp, "action", &tmpu64) != 0) {
				NS_LOG(NS_LOG_LVL_FATAL,
				       "Bad action for " NRD_MEREG_TGTNAME ":%s.cpp_read",
				       mereg_id);
				goto error_cleanup;
			}
			mereg->cpp_rd_action = (int8_t)tmpu64;
			if (nrd_j_obj_strnum_get(j_tmp, "token", &tmpu64) != 0) {
				NS_LOG(NS_LOG_LVL_FATAL,
				       "Bad token for " NRD_MEREG_TGTNAME ":%s.cpp_read", mereg_id);
				goto error_cleanup;
			}
			mereg->cpp_rd_token = (int8_t)tmpu64;
			if (nrd_j_obj_strnum_get(j_tmp, "offset", &tmpu64) != 0) {
				NS_LOG(NS_LOG_LVL_FATAL,
				       "Bad offset for " NRD_MEREG_TGTNAME ":%s.cpp_read",
				       mereg_id);
				goto error_cleanup;
			}
			mereg->cpp_rd_offset = tmpu64;

			j_tmp = json_object_get(j_mereg, "cpp_write");
			if (nrd_j_obj_strnum_get(j_tmp, "action", &tmpu64) != 0) {
				NS_LOG(NS_LOG_LVL_FATAL,
				       "Bad action for " NRD_MEREG_TGTNAME ":%s.cpp_write",
				       mereg_id);
				goto error_cleanup;
			}
			mereg->cpp_wr_action = (int8_t)tmpu64;
			if (nrd_j_obj_strnum_get(j_tmp, "token", &tmpu64) != 0) {
				NS_LOG(NS_LOG_LVL_FATAL,
				       "Bad token for " NRD_MEREG_TGTNAME ":%s.cpp_write",
				       mereg_id);
				goto error_cleanup;
			}
			mereg->cpp_wr_token = (int8_t)tmpu64;
			if (nrd_j_obj_strnum_get(j_tmp, "offset", &tmpu64) != 0) {
				NS_LOG(NS_LOG_LVL_FATAL,
				       "Bad offset for " NRD_MEREG_TGTNAME ":%s.cpp_write",
				       mereg_id);
				goto error_cleanup;
			}
			mereg->cpp_wr_offset = tmpu64;
		}
	}

	if (seen_names.str)
		free(seen_names.str);

	for (idx = 0; idx < data->tgt_coll.count; idx++) {
		int iid;
		struct nrd_target *tgt = &data->tgt_coll.entry[idx];

		if (tgt->scope != NRD_TARGET_SCOPE_SINGLE_ISLAND)
			continue;

		for (iid = nfp_chipdata_island_id_first(data->chip); iid != -1;
		     iid = nfp_chipdata_island_id_next(data->chip, iid)) {
			if (!nfp_chipdata_island_has_cpp_target(data->chip, iid,
								tgt->cpp_tgt))
				continue;
			if (tgt->single_island_iid == NFP_REGDATA_IID_NA) {
				tgt->single_island_iid = iid;
			} else {
				/* We found a second island hosting the CPP target, */
				/* so don't pick a default. */
				{
					NS_LOG(NS_LOG_LVL_FATAL,
					       "Target \"%s\" with single_island scope found in multiple islands",
					       tgt->map.name);
					goto error_cleanup;
				}
			}
		}
	}

	if (j_loaded)
		json_decref(j_loaded);

	NS_LOG(NS_LOG_LVL_EXTRA, "Register data loaded.");
	return data;

error_cleanup:
	if (j_loaded)
		json_decref(j_loaded);

	if (seen_names.str)
		free(seen_names.str);

	if (data)
		nfp_regdata_free(data);

	return NFP_ERRPTR(err);
}

const char *nfp_regdata_comment(struct nfp_regdata *regdata)
{
	return regdata->json_top_comment;
}

void nfp_regdata_free(struct nfp_regdata *regdata)
{
	size_t idx;

	if (!regdata)
		return;

	for (idx = 0; idx < regdata->map_coll.count; idx++)
		nrd_addrmap_free(&regdata->map_coll.entry[idx]);
	_ARRAY_FREE(&regdata->map_coll);

	for (idx = 0; idx < regdata->reg_coll.count; idx++)
		nrd_register_free(&regdata->reg_coll.entry[idx]);
	_ARRAY_FREE(&regdata->reg_coll);

	for (idx = 0; idx < regdata->tgt_coll.count; idx++) {
		struct nrd_target *item = &regdata->tgt_coll.entry[idx];

		nrd_addrmap_entry_free(&item->map);
		if (item->aliases.str)
			free(item->aliases.str);
		if (item->iid_remaps)
			free(item->iid_remaps);
	}
	_ARRAY_FREE(&regdata->tgt_coll);

	for (idx = 0; idx < regdata->mereg_coll.count; idx++) {
		struct nrd_mereg *item = &regdata->mereg_coll.entry[idx];

		if (item->altname)
			free(item->altname);
		if (item->name)
			free(item->name);
		if (item->description)
			free(item->description);
	}
	_ARRAY_FREE(&regdata->mereg_coll);

	if (regdata->json_dir)
		free(regdata->json_dir);

	if (regdata->me_local_csr_map_id)
		free(regdata->me_local_csr_map_id);

	if (regdata->xpb_targets.str)
		free(regdata->xpb_targets.str);

	if (regdata->_aliases) {
		for (idx = 0; idx < regdata->_aliases_cnt; idx++) {
			if (regdata->_aliases[idx].alias)
				free(regdata->_aliases[idx].alias);
			if (regdata->_aliases[idx].fullname)
				free(regdata->_aliases[idx].fullname);
		}
		free(regdata->_aliases);
	}

	if (regdata->json_top_comment)
		free(regdata->json_top_comment);

	free(regdata);
}

void nfp_regdata_node_free_data(struct nfp_regdata_node *node)
{
	if (!node)
		return;

	if (node->fqn) {
		free(node->fqn);
		node->fqn = NULL;
	}

	if (node->name) {
		free(node->name);
		node->name = NULL;
	}

	if (node->altname) {
		free(node->altname);
		node->altname = NULL;
	}

	if (node->description) {
		free(node->description);
		node->description = NULL;
	}
}

int nfp_regdata_family(struct nfp_regdata *regdata)
{
	return regdata->family;
}

int nfp_regdata_lookup(struct nfp_regdata *regdata,
		       const char *name,
		       unsigned int flags,
		       struct nfp_regdata_node *node)
{
	return nrd_regs_lookup(regdata, name, flags, node, NFP_REGDATA_IID_NA,
			       NFP_REGDATA_MENUM_NA);
}

int nfp_regdata_lookup_local(struct nfp_regdata *regdata,
			     const char *name,
			     unsigned int flags,
			     struct nfp_regdata_node *node,
			     int local_iid,
			     int local_menum)
{
	flags &= ~NFP_REGDATA_FLG_FQN_ONLY;
	return nrd_regs_lookup(regdata, name, flags, node, local_iid, local_menum);
}

int nfp_regdata_discover(struct nfp_regdata *regdata,
			 const char *name,
			 unsigned int flags,
			 void *usrdata,
			 int (*cb_found_reg)(struct nfp_regdata *regdata,
					     struct nfp_regdata_node *node,
					     void *usrdata))
{
	int err = EINVAL;
	size_t idx;
	struct nrd_str dynstr = nrd_str_zinit;
	struct nrd_strtab strtab = nrd_strtab_zinit;
	int iid, menum;
	char idstr[NFP_MEID_STR_SZ];

	if ((!regdata) || (!cb_found_reg)) {
		NS_LOG(NS_LOG_LVL_FATAL, "Invalid parameters.");
		err = EINVAL;
		goto error_cleanup;
	}

	err = nrd_str_resize(&dynstr, NFP_MEID_STR_SZ);
	if (err != 0)
		goto error_cleanup;

	if ((!name) || (name[0] == '\0')) {
		for (idx = 0; idx < regdata->tgt_coll.count; idx++) {
			struct nrd_target *tgt = &regdata->tgt_coll.entry[idx];
			struct nrd_mapentry_node next_node = nrd_mapentry_node_zinit;

			next_node.parent = NULL;
			next_node.this_idx1 = 0;
			next_node.this_idx2 = 0;
			next_node.this_entry = &tgt->map;

			if (nrd_discover_all(regdata, flags, usrdata, cb_found_reg, tgt, &next_node,
					     &dynstr, &strtab) != 0)
				goto error_cleanup;
		}

		for (iid = nfp_chipdata_island_id_first(regdata->chip); iid != -1;
		     iid = nfp_chipdata_island_id_next(regdata->chip, iid)) {
			for (menum = 0;
			     menum < (int)nfp_chipdata_island_me_count(regdata->chip, iid);
			     menum++) {
				for (idx = 0; idx < regdata->mereg_coll.count; idx++) {
					char *rncp = NULL;
					int ridx;
					struct nrd_mereg *mereg = &regdata->mereg_coll.entry[idx];

					if (NRD_GET_FQN(flags)) {
						err = nrd_str_resize(&dynstr,
								     strlen(NRD_MEREG_TGTNAME) + 1 +
								     NFP_MEID_STR_SZ + strlen(mereg->name) +
								     NRD_NUM64_DEC_DIGITS + 1);
						if (err != 0)
							goto error_cleanup;
						rncp = dynstr.str;
						rncp += sprintf(dynstr.str, "%s:%s.", NRD_MEREG_TGTNAME,
								nfp_meid2str(regdata->family, idstr,
									     NFP_MEID(regdata->family, iid,
										      menum)));
					}

					for (ridx = 0; ridx < (int)mereg->count; ridx++) {
						struct nfp_regdata_node node;
						uint64_t offset = 0;

						memset(&node, 0, sizeof(node));

						if (NRD_GET_FQN(flags)) {
							sprintf(rncp, mereg->name, ridx);
							node.fqn = strdup(dynstr.str);
						}

						node.ctx = NFP_REGDATA_CTX_NA;
						node.reg_type = NFP_REG_TYPE_MEREG;
						node.offset_iid_lsb = -1;
						node.offset_me_lsb = -1;
						node.is_sim_backdoorable = 1;

						/* mereg:, mereg:iX, mereg:meY and mereg:iX.meY */
						/* will all have the same name and other info. */
						node.type = NFP_REGDATA_NODE_TYPE_TARGET;
						node.offset = UINT64_MAX; /* override later */
						node.cpp_tgt = -1;

						node.iid = iid;
						node.menum = menum;

						node.type = NFP_REGDATA_NODE_TYPE_REG;

						node.data.mereg.mereg_type = mereg->type;
						node.data.mereg.reg_idx = ridx;

						/* Not printing 64-bit values yet, nothing has */
						/* that many repeats yet. */
						if (NRD_GET_FULLNAME(flags) && (mereg->name)) {
							_MALLOC_OOM(node.name,
								    (strlen(mereg->name) +
								     NRD_NUM64_DEC_DIGITS + 1));
							sprintf(node.name, mereg->name, ridx);
						}

						if (NRD_GET_ALTNAME(flags) && (mereg->altname)) {
							_MALLOC_OOM(node.altname,
								    (strlen(mereg->altname) +
								     NRD_NUM64_DEC_DIGITS + 1));
							sprintf(node.altname, mereg->altname, ridx);
						}

						if (NRD_GET_DESC(flags) && (mereg->description)) {
							_MALLOC_OOM(node.description,
								    (strlen(mereg->description) +
								     NRD_NUM64_DEC_DIGITS + 1));
							sprintf(node.description,
								mereg->description, ridx);
						}
						/* All NFP ME data registers are 32-bit */
						node.bit_len = 32;
						node.is_readable = 1;
						node.is_writable = 1;
						node.cpp_tgt = (int8_t)mereg->cpp_tgt;

						offset = 0;
						node.offset_iid_lsb = mereg->iid_lsb;
						node.offset_me_lsb = mereg->me_lsb;

						if (mereg->iid_lsb != -1) {
							/* Even if the iid is 0, we still "set" */
							/* it in the offset */
							/* and node reflects that. */
							offset |= iid << mereg->iid_lsb;
						}
						if (mereg->me_lsb != -1) {
							/* NFP_CHIP_FAMILY_NFP6000/ARCH_TH specific */
							offset |= (menum + 4) << mereg->me_lsb;
						}

						if (mereg->byteaddr_lsb >= 0)
							offset |= (ridx * 4) << mereg->byteaddr_lsb;
						else
							offset |= (ridx * 4) >>
								  (-mereg->byteaddr_lsb);

						node.cpp_rd_action = mereg->cpp_rd_action;
						node.cpp_rd_token = mereg->cpp_rd_token;
						node.cpp_rd_offset = offset + mereg->cpp_rd_offset;
						node.cpp_wr_action = mereg->cpp_wr_action;
						node.cpp_wr_token = mereg->cpp_wr_token;
						node.offset = offset + mereg->cpp_wr_offset;

						switch (cb_found_reg(regdata, &node, usrdata)) {
						case NFP_REGDATA_DISCOVER_RET_STOP:
							nfp_regdata_node_free_data(&node);
							goto success;
						case NFP_REGDATA_DISCOVER_RET_ABORT:
							nfp_regdata_node_free_data(&node);
							goto error_cleanup;
						case NFP_REGDATA_DISCOVER_RET_OWN:
							/* callback took the strings */
							break;
						case NFP_REGDATA_DISCOVER_RET_FREE:
							nfp_regdata_node_free_data(&node);
							break;
						default:
							nfp_regdata_node_free_data(&node /**/);
							break;
						}
					}
				}
			}
		}
	}

success:
	if (dynstr.str)
		free(dynstr.str);
	if (strtab.str)
		free(strtab.str);

	return 0;
error_cleanup:
	if (dynstr.str)
		free(dynstr.str);
	if (strtab.str)
		free(strtab.str);
	return NFP_ERRNO(err);
}

ssize_t nfp_regdata_matches(struct nfp_regdata *regdata,
			    const char *name,
			    char **matches,
			    unsigned int flags)
{
	struct nrd_str matchstr = nrd_str_zinit;
	struct nrd_str idstr = nrd_str_zinit;
	struct nrd_strtab strtab = nrd_strtab_zinit;
	int err = EINVAL;

	if (matches)
		*matches = NULL;

	if ((!regdata) || (!name) || (!matches)) {
		NS_LOG(NS_LOG_LVL_FATAL, "Invalid parameters.");
		err = EINVAL;
		goto error_cleanup;
	}

	err = nrd_matches(regdata, name, flags, &matchstr, &idstr, &strtab);
	if (err < 0)
		goto error_cleanup;

	if (idstr.str)
		free(idstr.str);
	if (matchstr.str)
		free(matchstr.str);

	*matches = strtab.str;

	return (ssize_t)strtab.str_cnt;

error_cleanup:

	if (idstr.str)
		free(idstr.str);
	if (strtab.str)
		free(strtab.str);

	return NFP_ERRNO(err);
}

int nfp_regdata_node2regid(struct nfp_regdata *regdata,
			   const struct nfp_regdata_node *node,
			   struct nfp_reg_id *regid)
{
	(void)regdata;

	if ((!node) || (!regid)) {
		NS_LOG(NS_LOG_LVL_FATAL, "Invalid parameters.");
		return -1;
	}

	memset(regid, 0, sizeof(*regid));

	switch (node->type) {
	case NFP_REGDATA_NODE_TYPE_REG:
	case NFP_REGDATA_NODE_TYPE_MULTIREG:
	case NFP_REGDATA_NODE_TYPE_BITF:
		break;
	default:
		NS_LOG(NS_LOG_LVL_ERROR, "Invalid node type.");
		return -1;
	}

	regid->ctx = NFP_REGDATA_CTX_NA;
	if (node->reg_type == NFP_REG_TYPE_XPB)
		regid->iid = -1;
	else
		regid->iid = (int16_t)node->iid;
	regid->ctx = (int8_t)node->ctx;
	regid->bit_len = (uint16_t)node->bit_len;
	regid->cpp_tgt = node->cpp_tgt;
	regid->cpp_offset = node->offset; /* Only ID by write offset */
	regid->cpp_rd_action = node->cpp_rd_action;
	regid->cpp_rd_token = node->cpp_rd_token;
	regid->cpp_wr_action = node->cpp_wr_action;
	regid->cpp_wr_token = node->cpp_wr_token;
	return 0;
}

enum nfp_reg_type nfp_regdata_regid2type(struct nfp_regdata *regdata,
					 const struct nfp_reg_id *regid)
{
	size_t idx;
	const struct nrd_target *tgt;
	const struct nrd_target *ftgt = NULL;
	const struct nrd_mereg *mereg;
	uint64_t tgt_off;

	if ((!regdata) || (!regid)) {
		NS_LOG(NS_LOG_LVL_FATAL, "Invalid parameters.");
		return NFP_REG_TYPE_NONE;
	}
	/* See if it matched any normal target */
	for (idx = 0, tgt = regdata->tgt_coll.entry; idx < regdata->tgt_coll.count; idx++, tgt++) {
		if (tgt->cpp_tgt != regid->cpp_tgt)
			continue;
		if (tgt->cpp_wr_action != regid->cpp_wr_action)
			continue;
		if (tgt->cpp_wr_token != regid->cpp_wr_token)
			continue;
		if (tgt->cpp_rd_action != regid->cpp_rd_action)
			continue;
		if (tgt->cpp_rd_token != regid->cpp_rd_token)
			continue;

		tgt_off = tgt->cpp_wr_offset + tgt->map.offset;

		/* offset cmp mask is used to select between */
		/* xpb and xpbm and if we ever use cls reflect */
		/* for mecsrs, that too. */
		if ((tgt_off & tgt->offset_cmp_mask) ==
		    (regid->cpp_offset & tgt->offset_cmp_mask)) {
			ftgt = tgt;
			break;
		}
	}

	if (ftgt)
		return ftgt->reg_type;

	/* Might be an ME register then */
	for (idx = 0, mereg = regdata->mereg_coll.entry; idx < regdata->mereg_coll.count;
	     idx++, mereg++) {
		if (mereg->cpp_tgt != regid->cpp_tgt)
			continue;
		if (mereg->cpp_wr_action != regid->cpp_wr_action)
			continue;
		if (mereg->cpp_rd_action != regid->cpp_rd_action)
			continue;
		if (mereg->cpp_wr_token != regid->cpp_wr_token)
			continue;
		if (mereg->cpp_rd_token != regid->cpp_rd_token)
			continue;
		if ((mereg->cpp_wr_offset & mereg->offset_cmp_mask) ==
		    (regid->cpp_offset & mereg->offset_cmp_mask)) {
			return NFP_REG_TYPE_MEREG;
		}
	}

	return NFP_REG_TYPE_NONE;
}

int nfp_regdata_regid2mereg(struct nfp_regdata *regdata,
			    const struct nfp_reg_id *regid,
			    enum nfp_mereg_type *mereg_type,
			    int *meid,
			    uint16_t *reg_idx)
{
	size_t idx;
	const struct nrd_mereg *mereg;
	int menum;

	if ((!regdata) || (!regid) || (!mereg_type) || (!meid) || (!reg_idx)) {
		NS_LOG(NS_LOG_LVL_FATAL, "Invalid parameters.");
		return -1;
	}

	if (regdata->arch != NFP_CHIP_ARCH_TH) {
		NS_LOG(NS_LOG_LVL_FATAL, "Invalid chip family.");
		return -1;
	}

	for (idx = 0; idx < regdata->mereg_coll.count; idx++) {
		mereg = &regdata->mereg_coll.entry[idx];
		if (mereg->cpp_tgt != regid->cpp_tgt)
			continue;
		if (mereg->cpp_wr_action != regid->cpp_wr_action)
			continue;
		if (mereg->cpp_rd_action != regid->cpp_rd_action)
			continue;
		if (mereg->cpp_wr_token != regid->cpp_wr_token)
			continue;
		if (mereg->cpp_rd_token != regid->cpp_rd_token)
			continue;
		if ((mereg->cpp_wr_offset & mereg->offset_cmp_mask) ==
		    (regid->cpp_offset & mereg->offset_cmp_mask)) {
			/* We ID on the write-offset only, so we found it */
			*mereg_type = mereg->type;
			menum = ((regid->cpp_offset >> mereg->me_lsb) & 0xF) - 4;
			*meid = NFP_MEID(regdata->family, regid->iid, menum);
			if (mereg->byteaddr_lsb < 0)
				*reg_idx = (uint16_t)(regid->cpp_offset << -mereg->byteaddr_lsb);
			else
				*reg_idx = (uint16_t)(regid->cpp_offset >> mereg->byteaddr_lsb);

			*reg_idx &= (mereg->count_mask << 2);
			*reg_idx /= 4;
			return 0;
		}
	}

	return -1;
}

int nfp_regdata_regid2mecsr(struct nfp_regdata *regdata,
			    const struct nfp_reg_id *regid,
			    int *meid,
			    uint16_t *offset)
{
	int menum, iid;
	uint64_t off;

	if ((!regdata) || (!regid) || (!meid) || (!offset)) {
		NS_LOG(NS_LOG_LVL_FATAL, "Invalid parameters.");
		return -1;
	}

	if (regdata->arch != NFP_CHIP_ARCH_TH) {
		NS_LOG(NS_LOG_LVL_FATAL, "Invalid chip family.");
		return -1;
	}

	if (!regdata->tgt_mecsr)
		return -1;

	if (regdata->tgt_mecsr->cpp_tgt != regid->cpp_tgt)
		return -1;
	if (regdata->tgt_mecsr->cpp_rd_action != regid->cpp_rd_action)
		return -1;
	if (regdata->tgt_mecsr->cpp_rd_token != regid->cpp_rd_token)
		return -1;
	if (regdata->tgt_mecsr->cpp_wr_action != regid->cpp_wr_action)
		return -1;
	if (regdata->tgt_mecsr->cpp_wr_token != regid->cpp_wr_token)
		return -1;

	off = regdata->tgt_mecsr->cpp_wr_offset + regdata->tgt_mecsr->map.offset;
	if ((off & regdata->tgt_mecsr->offset_cmp_mask) !=
	    (regid->cpp_offset & regdata->tgt_mecsr->offset_cmp_mask))
		return -1;

	if (regdata->tgt_mecsr->offset_me_lsb == -1)
		return -1;
	menum = (regid->cpp_offset >> regdata->tgt_mecsr->offset_me_lsb) & 0xF;
	menum -= 4;

	if (regdata->tgt_mecsr->offset_iid_lsb == -1)
		iid = regid->iid;
	else
		iid = (regid->cpp_offset >> regdata->tgt_mecsr->offset_iid_lsb) & 0x3F;

	off |= (0xF << regdata->tgt_mecsr->offset_me_lsb);
	if (regdata->tgt_mecsr->offset_iid_lsb != -1)
		off |= (0x3F << regdata->tgt_mecsr->offset_iid_lsb);

	off |= regdata->tgt_mecsr->offset_cmp_mask;
	off = regid->cpp_offset & ~off;
	/* We should now have an ME-relative offset, */
	/* masked off the fixed and iid/menum bits. */

	*meid = NFP_MEID(regdata->family, iid, menum);
	if (regdata->tgt_mecsr->byteaddr_lsb < 0)
		*offset = (uint16_t)(off << -regdata->tgt_mecsr->byteaddr_lsb);
	else
		*offset = (uint16_t)(off >> regdata->tgt_mecsr->byteaddr_lsb);

	return 0;
}

const char *nfp_reg_id_tgtname(struct nfp_regdata *regdata, const struct nfp_reg_id *regid)
{
	size_t idx;
	const struct nrd_target *tgt;
	const struct nrd_target *ftgt = NULL;
	const struct nrd_mereg *mereg;
	uint64_t tgt_off;

	if ((!regdata) || (!regid)) {
		NS_LOG(NS_LOG_LVL_FATAL, "Invalid parameters.");
		return NULL;
	}
	/* See if it matched any normal target */
	for (idx = 0, tgt = regdata->tgt_coll.entry; idx < regdata->tgt_coll.count; idx++, tgt++) {
		if (tgt->cpp_tgt != regid->cpp_tgt)
			continue;
		if (tgt->cpp_wr_action != regid->cpp_wr_action)
			continue;
		if (tgt->cpp_wr_token != regid->cpp_wr_token)
			continue;
		if (tgt->cpp_rd_action != regid->cpp_rd_action)
			continue;
		if (tgt->cpp_rd_token != regid->cpp_rd_token)
			continue;

		tgt_off = tgt->cpp_wr_offset + tgt->map.offset;

		/* offset cmp mask is used to select between */
		/* xpb and xpbm and if we ever use cls reflect */
		/* for mecsrs, that too. */
		if ((tgt_off & tgt->offset_cmp_mask) ==
		    (regid->cpp_offset & tgt->offset_cmp_mask)) {
			ftgt = tgt;
			break;
		}
	}

	if (ftgt)
		return ftgt->map.name;

	/* Might be an ME register then */
	for (idx = 0, mereg = regdata->mereg_coll.entry; idx < regdata->mereg_coll.count;
	     idx++, mereg++) {
		if (mereg->cpp_tgt != regid->cpp_tgt)
			continue;
		if (mereg->cpp_wr_action != regid->cpp_wr_action)
			continue;
		if (mereg->cpp_rd_action != regid->cpp_rd_action)
			continue;
		if (mereg->cpp_wr_token != regid->cpp_wr_token)
			continue;
		if (mereg->cpp_rd_token != regid->cpp_rd_token)
			continue;
		if ((mereg->cpp_wr_offset & mereg->offset_cmp_mask) ==
		    (regid->cpp_offset & mereg->offset_cmp_mask)) {
			return NRD_MEREG_TGTNAME;
		}
	}

	return NULL;
}

int nfp_regdata_node_cmp(struct nfp_regdata *regdata,
			 const struct nfp_regdata_node *node1,
			 const struct nfp_regdata_node *node2,
			 int cmp_str)
{
	int cmp;

	(void)regdata; /* Unused */

	if ((!node1) || (!node2)) {
		NS_LOG(NS_LOG_LVL_FATAL, "Invalid parameters.");
		return NFP_REG_TYPE_NONE;
	}
#define _CMP(_f_)                                                                    \
	do {                                                                         \
		cmp = ((node1->_f_) > (node2->_f_)) - ((node1->_f_) < (node2->_f_)); \
		if (cmp != 0)                                                        \
			return cmp;                                                  \
	} while (0)

#define _CMPSTR(_sa_, _sb_)                                   \
	do {                                                  \
		cmp = (((_sa_) == NULL) > ((_sb_) == NULL)) - \
		      (((_sa_) == NULL) < ((_sb_) == NULL));  \
		if (cmp != 0)                                 \
			return cmp;                           \
		if ((_sa_) == NULL)                           \
			break;                                \
		cmp = strcmp((_sa_), (_sb_));                 \
		if (cmp != 0)                                 \
			return cmp;                           \
	} while (0)

#define _CMPFIELD(_a_, _b_)                                                          \
	do {                                                                         \
		cmp = (((_a_)->msb) > ((_b_)->msb)) - (((_a_)->msb) < ((_b_)->msb)); \
		if (cmp != 0)                                                        \
			return cmp;                                                  \
		cmp = (((_a_)->lsb) > ((_b_)->lsb)) - (((_a_)->lsb) < ((_b_)->lsb)); \
		if (cmp != 0)                                                        \
			return cmp;                                                  \
		cmp = (((_a_)->access_mode) > ((_b_)->access_mode)) -                \
		      (((_a_)->access_mode) < ((_b_)->access_mode));                 \
		if (cmp != 0)                                                        \
			return cmp;                                                  \
		if (cmp_str) {                                                       \
			_CMPSTR((_a_)->name, (_b_)->name);                           \
			_CMPSTR((_a_)->altname, (_b_)->altname);                     \
		}                                                                    \
	} while (0)

	_CMP(type);
	_CMP(reg_type);
	_CMP(cpp_tgt);
	_CMP(cpp_rd_action);
	_CMP(cpp_rd_token);
	_CMP(cpp_wr_action);
	_CMP(cpp_wr_token);
	_CMP(iid);
	_CMP(menum);
	_CMP(ctx);
	_CMP(cpp_rd_offset);
	_CMP(offset);
	_CMP(bit_len);
	_CMP(reg_byte_first);
	_CMP(reg_byte_last);
	_CMP(offset_iid_lsb);
	_CMP(offset_me_lsb);

	cmp = ((!node1->matched_field) > (!node2->matched_field)) -
	      ((!node1->matched_field) < (!node2->matched_field));
	if (cmp != 0)
		return cmp;

	if (node1->matched_field) {
		/* Only compare matched field, we don't care about other */
		/* fields in this case. */
		/* Since we already found the field by name in both lookups, */
		/* we don't really need to compare the name/altname attributes. */
		int tmp = cmp_str;

		cmp_str = 0;
		_CMPFIELD(node1->matched_field, node2->matched_field);
		cmp_str = tmp;
	} else {
		size_t i;

		_CMP(field_cnt);
		/* Looked up whole register, compare all fields. */
		for (i = 0; i < node1->field_cnt; i++)
			_CMPFIELD(&node1->fields[i], &node2->fields[i]);
	}

	if (cmp_str) {
		_CMPSTR(node1->fqn, node2->fqn);
		_CMPSTR(node1->name, node2->name);
		_CMPSTR(node1->altname, node2->altname);

		/* Don't compare description, it doesn't affect access */
		/* _CMPSTR(node1->description, node2->description); */
	}
#undef _CMP
#undef _CMPFIELD
#undef _CMPSTR
	return 0;
}

int nfp_reg_id_cmp(struct nfp_regdata *regdata,
		   const struct nfp_reg_id *regid1,
		   const struct nfp_reg_id *regid2)
{
	int cmp = 0;
	enum nfp_reg_type type1;
	enum nfp_reg_type type2;

	if ((!regid1) || (!regid2))
		return -1;

#define _REGCMP(_f_)                                                                     \
	do {                                                                             \
		cmp = ((regid1->_f_) > (regid2->_f_)) - ((regid1->_f_) < (regid2->_f_)); \
		if (cmp != 0)                                                            \
			return cmp;                                                      \
	} while (0)

	type1 = nfp_regdata_regid2type(regdata, regid1);
	type2 = nfp_regdata_regid2type(regdata, regid2);

	if (type1 != type2)
		return ((type1) > (type2)) - ((type1) < (type2));

	/* _REGCMP(bit_len); // Don't compare bit_len, the other fields */
	/* are identity enough */
	/* and letting users keep bit_len unknown makes some things easier. */
	if (type1 == NFP_REG_TYPE_MECSR)
		_REGCMP(ctx);
	if (type1 != NFP_REG_TYPE_XPB)
		_REGCMP(iid);
	_REGCMP(cpp_tgt);
	_REGCMP(cpp_rd_action);
	_REGCMP(cpp_rd_token);
	_REGCMP(cpp_wr_action);
	_REGCMP(cpp_wr_token);

	if (regdata->arch == NFP_CHIP_ARCH_TH) {
		/* Deem XPB and XPBM offset to the same register as */
		/* pointing to the same physical register. */
		if (type1 == NFP_REG_TYPE_XPB)
			_REGCMP(cpp_offset & ~(1 << 30));
		else
			_REGCMP(cpp_offset);
	} else {
		_REGCMP(cpp_offset);
	}

#undef _REGCMP
	return 0;
}

size_t nfp_regdata_mereg_cnt(struct nfp_regdata *regdata, enum nfp_mereg_type mereg_type)
{
	size_t idx;
	const struct nrd_mereg *mereg;

	if (!regdata) {
		NS_LOG(NS_LOG_LVL_FATAL, "Invalid parameters.");
		return 0;
	}

	for (idx = 0; idx < regdata->mereg_coll.count; idx++) {
		mereg = (const struct nrd_mereg *)&regdata->mereg_coll.entry[idx];
		if (mereg->type == mereg_type)
			return mereg->count;
	}

	return 0;
}

int nfp_regdata_mecsr_lookup(struct nfp_regdata *regdata,
			     const char *name,
			     unsigned int flags,
			     struct nfp_regdata_node *node)
{
	int err = 0;
	size_t idx;
	struct nrd_addrmap_entry *entry = NULL;
	int8_t ctxN = NFP_REGDATA_CTX_NA;

	/* Use regdata->me_local_csr_map */

	if (node) /* Clear first so error_cleanup doesn't segfault */
		memset(node, 0, sizeof(*node));

	if ((!regdata) || (!name) || (!node)) {
		NS_LOG(NS_LOG_LVL_FATAL, "Invalid parameters.");
		err = EINVAL;
		goto error_cleanup;
	}

	if (!regdata->me_local_csr_map) {
		if (nrd_load_and_extract(regdata, regdata->me_local_csr_map_id) != 0)
			goto error_cleanup;

		if (!regdata->me_local_csr_map) {
			NS_LOG(NS_LOG_LVL_FATAL, "Internal error: No ME CSR map found.");
			err = EFAULT;
			goto error_cleanup;
		}
	}
	/* Similar block in nrd_map_entry_find */
	if (strncasecmp(name, "ctx", 3) == 0) {
		if ((name[3] >= '0') && (name[3] <= ('0' + NRD_MAX_CTX_CNT))) {
			if (name[4] == '.') {
				ctxN = (int8_t)(name[3] - '0');
				name += 5;
			}
		}
	}

	for (idx = 0; idx < regdata->me_local_csr_map->mapentries.count; idx++) {
		entry = &regdata->me_local_csr_map->mapentries.entry[idx];
		if ((entry->type != NFP_REGDATA_NODE_TYPE_REG) &&
		    (entry->type != NFP_REGDATA_NODE_TYPE_MULTIREG))
			continue;
		if (entry->reg_type != NFP_REG_TYPE_MECSR)
			continue;

		if (NRD_IGN_CASE_FULLNAME(flags)) {
			if (strcasecmp(entry->name, name) == 0)
				break;
		} else {
			if (strcmp(entry->name, name) == 0)
				break;
		}

		if (entry->altname) {
			if (NRD_IGN_CASE_ALTNAME(flags)) {
				if (strcasecmp(entry->altname, name) == 0)
					break;
			} else {
				if (strcmp(entry->altname, name) == 0)
					break;
			}
		}
	}

	if ((idx >= regdata->me_local_csr_map->mapentries.count) || (!entry)) {
		err = ENOENT;
		goto error_cleanup;
	}

	node->type = entry->type;

	if (NRD_GET_FULLNAME(flags))
		node->name = safe_strdup(entry->name);
	if (NRD_GET_ALTNAME(flags))
		node->altname = safe_strdup(entry->altname);
	if (NRD_GET_DESC(flags))
		node->description = safe_strdup(entry->description);
	node->bit_len = entry->ptr.reg->bit_len;
	node->cpp_rd_action = 0;
	node->cpp_rd_token = 0;
	node->cpp_rd_offset = entry->offset;
	node->cpp_wr_action = 0;
	node->cpp_wr_token = 0;
	node->offset = entry->offset;
	node->cpp_tgt = -1;
	node->field_cnt = entry->ptr.reg->field_count;
	node->fields = (const struct nfp_reg_bitf *)entry->ptr.reg->fields;
	node->matched_field = NULL;
	node->iid = NFP_REGDATA_IID_NA;
	node->menum = NFP_REGDATA_MENUM_LOCAL;
	node->offset_iid_lsb = -1;
	node->offset_me_lsb = -1;
	node->is_sim_backdoorable = entry->is_sim_backdoorable;

	node->reg_type = NFP_REG_TYPE_MECSR;
	node->data.mecsr.latency_wr = entry->extra.mecsr.latency_wr;
	node->data.mecsr.latency_rd = entry->extra.mecsr.latency_rd;
	node->data.mecsr.latency_use = entry->extra.mecsr.latency_use;
	if (entry->extra.mecsr.is_indirect_ctx_csr) {
		if (ctxN >= 0)
			node->ctx = ctxN;
		else
			node->ctx = NFP_REGDATA_CTX_ANY;
	} else {
		if (ctxN >= 0) {
			NS_LOG(NS_LOG_LVL_DEBUG,
			       "ME register '%s' is not a context indirect register.", entry->name);
			err = ENOENT;
			goto error_cleanup;
		}
		node->ctx = NFP_REGDATA_CTX_NA;
	}

	nrd_update_accessmode(node, node->field_cnt);
	nrd_update_reg_byte_offsets(regdata, entry, node);

	return 0;

error_cleanup:
	nfp_regdata_node_free_data(node);
	return NFP_ERRNO(err);
}

ssize_t nfp_regdata_mecsr_list(struct nfp_regdata *regdata, struct nfp_regdata_node **nodes)
{
	size_t idx;
	int err = 0;
	struct nfp_regdata_node *reglist = NULL;
	struct nrd_addrmap_entry *entry = NULL;
	ssize_t cnt = 0;

	if ((!regdata) || (!nodes)) {
		NS_LOG(NS_LOG_LVL_FATAL, "Invalid parameters.");
		err = EINVAL;
		goto error_cleanup;
	}

	*nodes = NULL;

	if (!regdata->me_local_csr_map) {
		if (nrd_load_and_extract(regdata, regdata->me_local_csr_map_id) != 0)
			goto error_cleanup;

		if (!regdata->me_local_csr_map) {
			NS_LOG(NS_LOG_LVL_FATAL, "Internal error: No ME CSR map found.");
			err = EFAULT;
			goto error_cleanup;
		}
	}

	if (regdata->me_local_csr_map->mapentries.count == 0)
		return 0;

	_CALLOC_OOM(reglist, regdata->me_local_csr_map->mapentries.count,
		    sizeof(struct nfp_regdata_node));

	cnt = 0;
	for (idx = 0; idx < regdata->me_local_csr_map->mapentries.count; idx++) {
		entry = &regdata->me_local_csr_map->mapentries.entry[idx];
		if ((entry->type != NFP_REGDATA_NODE_TYPE_REG) &&
		    (entry->type != NFP_REGDATA_NODE_TYPE_MULTIREG))
			continue;
		if (entry->reg_type != NFP_REG_TYPE_MECSR)
			continue;

		reglist[cnt].name = safe_strdup(entry->name);
		reglist[cnt].altname = safe_strdup(entry->altname);
		reglist[cnt].description = safe_strdup(entry->description);

		reglist[cnt].bit_len = entry->ptr.reg->bit_len;
		reglist[cnt].cpp_rd_action = 0;
		reglist[cnt].cpp_rd_token = 0;
		reglist[cnt].cpp_rd_offset = entry->offset;
		reglist[cnt].cpp_wr_action = 0;
		reglist[cnt].cpp_wr_token = 0;
		reglist[cnt].offset = entry->offset;
		reglist[cnt].cpp_tgt = -1;
		reglist[cnt].field_cnt = entry->ptr.reg->field_count;
		reglist[cnt].fields = (const struct nfp_reg_bitf *)entry->ptr.reg->fields;
		reglist[cnt].matched_field = NULL;
		reglist[cnt].iid = NFP_REGDATA_IID_NA;
		reglist[cnt].menum = NFP_REGDATA_MENUM_LOCAL;
		reglist[cnt].offset_iid_lsb = -1;
		reglist[cnt].offset_me_lsb = -1;
		reglist[cnt].is_sim_backdoorable = entry->is_sim_backdoorable;

		reglist[cnt].reg_type = NFP_REG_TYPE_MECSR;
		reglist[cnt].data.mecsr.offset = (uint16_t)entry->offset;
		reglist[cnt].data.mecsr.latency_wr = entry->extra.mecsr.latency_wr;
		reglist[cnt].data.mecsr.latency_rd = entry->extra.mecsr.latency_rd;
		reglist[cnt].data.mecsr.latency_use = entry->extra.mecsr.latency_use;
		if (entry->extra.mecsr.is_indirect_ctx_csr)
			reglist[cnt].ctx = NFP_REGDATA_CTX_ANY;
		else
			reglist[cnt].ctx = NFP_REGDATA_CTX_NA;

		nrd_update_accessmode(&reglist[cnt], reglist[cnt].field_cnt);
		nrd_update_reg_byte_offsets(regdata, entry, &reglist[cnt]);

		cnt++;
	}

	*nodes = reglist;
	return cnt;

error_cleanup:

	if (reglist) {
		for (idx = 0; idx < (size_t)cnt; idx++)
			nfp_regdata_node_free_data(&reglist[cnt]);
		free(reglist);
		reglist = NULL;
	}

	return NFP_ERRNO(err);
}

int nfp_regdata_find_regid(struct nfp_regdata *regdata,
			   const struct nfp_reg_id *regid,
			   unsigned int flags,
			   struct nfp_regdata_node *node)
{
	int err = EINVAL;

	if ((!regdata) || (!regid) || (!node)) {
		NS_LOG(NS_LOG_LVL_FATAL, "Invalid parameters.");
		err = EINVAL;
		goto error_cleanup;
	}

	switch (nfp_regdata_regid2type(regdata, regid)) {
	case NFP_REG_TYPE_CPP:
		return nfp_regdata_find_cpp(regdata, regid->cpp_tgt, regid->iid, regid->cpp_offset,
					    flags, node);
	case NFP_REG_TYPE_XPB:
		return nfp_regdata_find_xpb(regdata, regid->cpp_offset, flags, node);
	case NFP_REG_TYPE_MECSR: {
		int meid = 0;
		uint16_t csr_off = 0;

		if (nfp_regdata_regid2mecsr(regdata, regid, &meid, &csr_off) != 0)
			goto error_cleanup;

		return nfp_regdata_find_mecsr(regdata, NFP_MEID_ISLAND_of(regdata->family, meid),
					      NFP_MEID_MENUM_of(regdata->family, meid), regid->ctx,
					      (uint64_t)csr_off, flags, node);
	}
	case NFP_REG_TYPE_MEREG: {
		int meid = 0;
		uint16_t reg_idx;
		enum nfp_mereg_type mert;

		if (nfp_regdata_regid2mereg(regdata, regid, &mert, &meid, &reg_idx) != 0)
			goto error_cleanup;

		return nfp_regdata_find_mereg(regdata, NFP_MEID_ISLAND_of(regdata->family, meid),
					      NFP_MEID_MENUM_of(regdata->family, meid), mert,
					      reg_idx * 4, flags, node);
	}
	default:
		goto error_cleanup;
	}

error_cleanup:
	return NFP_ERRNO(err);
}

int nfp_regdata_find_cpp(struct nfp_regdata *regdata,
			 int8_t cpp_tgt,
			 int island_id,
			 uint64_t offset,
			 unsigned int flags,
			 struct nfp_regdata_node *node)
{
	int err = EINVAL;
	int found_partial = 0;
	struct nrd_strtab strtab = nrd_strtab_zinit;
	struct nrd_str str = nrd_str_zinit;
	size_t idx;
	uint64_t mapidx1 = 0, mapidx2 = 0;
	struct nrd_target *tgt = NULL;
	uint64_t seen_offset = 0;
	struct nrd_addrmap_entry *map_entry = NULL;
	char *cp;
	char idstr[NFP_MEID_STR_SZ];
	int fmenum = NFP_REGDATA_MENUM_NA;
	int fisland_id = island_id;
	uint64_t foffset = offset;
	uint64_t tgt_off = 0;

	if (node) /* Clear first so error_cleanup doesn't segfault */
		memset(node, 0, sizeof(*node));

	if ((!regdata) || (!node) || (!regdata->chip)) {
		NS_LOG(NS_LOG_LVL_FATAL, "Invalid parameters.");
		err = EINVAL;
		goto error_cleanup;
	}

	if (island_id > 0) {
		if (!nfp_chipdata_island_has_cpp_target(regdata->chip, island_id, cpp_tgt))
			goto not_found;
	} else {
		if (!nfp_chipdata_has_cpp_target(regdata->chip, cpp_tgt))
			goto not_found;
	}

	for (idx = 0; idx < regdata->tgt_coll.count; idx++) {
		struct nrd_target *thistgt = &regdata->tgt_coll.entry[idx];

		if ((thistgt->cpp_tgt != cpp_tgt) || (thistgt->reg_type != NFP_REG_TYPE_CPP))
			continue;
		tgt_off = (thistgt->cpp_wr_offset + thistgt->map.offset);
		if ((tgt_off & thistgt->offset_cmp_mask) != (offset & thistgt->offset_cmp_mask))
			continue;

		if (tgt) {
			tgt = NULL;
			break;
		}
		tgt = thistgt;
	}

	if (!tgt)
		goto not_found;

	if (tgt->cpp_rd_offset != tgt->cpp_wr_offset) {
		NS_LOG(NS_LOG_LVL_ERROR,
		       "CPP Register with different read and write offset cannot be identified with CPP target %d and offset 0x%" PRIx64,
		       cpp_tgt, offset);
		goto error_cleanup;
	}

	seen_offset += tgt->cpp_rd_offset + tgt->map.offset;
	map_entry = &tgt->map;
	if ((NRD_GET_FQN(flags)) && nrd_strtab_add(&strtab, tgt->map.name, 1, 0) != 0) {
		NS_LOG(NS_LOG_LVL_ERROR, "Error building lookup string.");
		goto error_cleanup;
	}

	switch (tgt->scope) {
	case NRD_TARGET_SCOPE_GLOBAL:
		break;
	case NRD_TARGET_SCOPE_SINGLE_ISLAND:
		if (island_id >= 0) {
			if ((NRD_GET_FQN(flags)) &&
			    nrd_strtab_add(&strtab, nfp_island2str(regdata->family, idstr, island_id),
					   1, 0) != 0) {
				NS_LOG(NS_LOG_LVL_ERROR, "Error building lookup string.");
				goto error_cleanup;
			}
		} else {
			fisland_id = tgt->single_island_iid;
		}
		break;
	case NRD_TARGET_SCOPE_ISLAND:
		if (island_id > 0) {
			if ((NRD_GET_FQN(flags)) &&
			    nrd_strtab_add(&strtab, nfp_island2str(regdata->family, idstr, island_id),
					   1, 0) != 0) {
				NS_LOG(NS_LOG_LVL_ERROR, "Error building lookup string.");
				goto error_cleanup;
			}
		} else {
			fisland_id = 0;
		}
		break;
	default:
		break;
	}

	err = nrd_find_reg_in_maps(regdata, &map_entry, offset, &seen_offset, &mapidx1, &mapidx2,
				   (NRD_GET_FQN(flags)) ? &strtab : NULL,
				   (NRD_GET_FQN(flags)) ? &str : NULL);
	if (err < 0)
		goto error_cleanup;
	if (!map_entry)
		goto not_found;
	if (err == 1)
		found_partial = 1;

	err = EINVAL;

	if ((seen_offset == offset) && ((map_entry->type == NFP_REGDATA_NODE_TYPE_REG) ||
					(map_entry->type == NFP_REGDATA_NODE_TYPE_MULTIREG)))
		found_partial = 0;

	if (strtab.str) {
		if (!NRD_GET_FQN(flags)) {
			free(strtab.str);
		} else {
			if (strtab.str_cnt == 1)
				nrd_strtab_add(&strtab, "", 1, 1);

			cp = strtab.str + strlen(strtab.str);
			*cp = ':';
			cp++;

			for (idx = 1; idx < strtab.str_cnt - 1; idx++) {
				cp += strlen(cp);
				*cp = '.';
				cp++;
			}

			node->fqn = strtab.str;
			strtab.str = NULL;
		}
	}

	err = -nrd_entry_offset_patch_extract(regdata, tgt, map_entry, &fisland_id, &fmenum,
					      &foffset, 0);
	if (err != 0)
		goto error_cleanup;

	if (NRD_GET_FULLNAME(flags) && (map_entry->name)) {
		_MALLOC_OOM(node->name, (strlen(map_entry->name) +
					 (NRD_NUM64_DEC_DIGITS * map_entry->dimensions) + 1));
		sprintf(node->name, map_entry->name, (int)mapidx1, (int)mapidx2);
	}

	if (NRD_GET_ALTNAME(flags) && (map_entry->altname)) {
		_MALLOC_OOM(node->altname, (strlen(map_entry->altname) +
					    (NRD_NUM64_DEC_DIGITS * map_entry->dimensions) + 1));
		sprintf(node->altname, map_entry->altname, (int)mapidx1, (int)mapidx2);
	}

	if (NRD_GET_DESC(flags) && (map_entry->description)) {
		_MALLOC_OOM(node->description,
			    (strlen(map_entry->description) +
			     (NRD_NUM64_DEC_DIGITS * map_entry->dimensions) + 1));
		sprintf(node->description, map_entry->description, (int)mapidx1, (int)mapidx2);
	}

	node->cpp_tgt = cpp_tgt;
	node->bit_len = map_entry->ptr.reg->bit_len;
	node->cpp_rd_action = tgt->cpp_rd_action;
	node->cpp_rd_token = tgt->cpp_rd_token;
	node->cpp_rd_offset = foffset;
	node->cpp_wr_action = tgt->cpp_wr_action;
	node->cpp_wr_token = tgt->cpp_wr_token;
	node->offset = foffset;
	node->ctx = NFP_REGDATA_CTX_NA;
	node->field_cnt = map_entry->ptr.reg->field_count;
	node->fields = (const struct nfp_reg_bitf *)map_entry->ptr.reg->fields;
	node->iid = fisland_id;
	node->menum = fmenum;
	if (map_entry->reg_type != NFP_REG_TYPE_NONE)
		node->reg_type = map_entry->reg_type;
	else
		node->reg_type = tgt->reg_type;
	node->type = map_entry->type;
	node->offset_iid_lsb = tgt->offset_iid_lsb;
	node->offset_me_lsb = tgt->offset_me_lsb;
	node->is_sim_backdoorable = map_entry->is_sim_backdoorable;

	nrd_update_accessmode(node, node->field_cnt);
	nrd_update_reg_byte_offsets(regdata, map_entry, node);

	if (str.str)
		free(str.str);

	return (found_partial) ? 2 : 0;
not_found:
	if (strtab.str)
		free(strtab.str);
	if (str.str)
		free(str.str);
	return 1;

error_cleanup:

	nfp_regdata_node_free_data(node);

	if (strtab.str)
		free(strtab.str);
	if (str.str)
		free(str.str);

	return NFP_ERRNO(err);
}

int nfp_regdata_find_xpb(struct nfp_regdata *regdata,
			 uint64_t offset,
			 unsigned int flags,
			 struct nfp_regdata_node *node)
{
	int err = EINVAL;
	int found_partial = 0;
	struct nrd_strtab strtab = nrd_strtab_zinit;
	struct nrd_str str = nrd_str_zinit;
	size_t idx;
	uint64_t mapidx1 = 0, mapidx2 = 0;
	struct nrd_target *tgt = NULL;
	uint64_t seen_offset = 0;
	struct nrd_addrmap_entry *map_entry = NULL;
	char *cp;
	int fmenum = NFP_REGDATA_MENUM_NA;
	int fisland_id = 0;
	uint64_t foffset = offset;
	uint64_t tgt_off = 0;

	if (node) /* Clear first so error_cleanup doesn't segfault */
		memset(node, 0, sizeof(*node));

	if ((!regdata) || (!node)) {
		NS_LOG(NS_LOG_LVL_FATAL, "Invalid parameters.");
		err = EINVAL;
		goto error_cleanup;
	}

	for (idx = 0; idx < regdata->tgt_coll.count; idx++) {
		struct nrd_target *thistgt = &regdata->tgt_coll.entry[idx];

		if (thistgt->reg_type != NFP_REG_TYPE_XPB)
			continue;
		tgt_off = (thistgt->cpp_wr_offset + thistgt->map.offset);
		if ((tgt_off & thistgt->offset_cmp_mask) != (offset & thistgt->offset_cmp_mask))
			continue;

		if (tgt) {
			tgt = NULL;
			break;
		}
		tgt = thistgt;
	}

	if (!tgt)
		goto not_found;

	if (tgt->cpp_rd_offset != tgt->cpp_wr_offset) {
		NS_LOG(NS_LOG_LVL_ERROR,
		       "XPB Register with different read and write offset cannot be identified with offset 0x%" PRIx64,
		       offset);
		goto error_cleanup;
	}

	/* Patch up IID if local */
	if (tgt->offset_iid_lsb != -1) {
		switch (regdata->arch) {
		case NFP_CHIP_ARCH_TH:
			fisland_id = (offset >> tgt->offset_iid_lsb) & 0x3F;
			if (fisland_id == 0) {
				/* We can't do local-island reverse lookups on XPBM, so
				 * don't try.
				 */
				goto not_found;
			}
			break;
		default:
			NS_LOG(NS_LOG_LVL_FATAL, "Unsupported chip family");
			return -EINVAL;
		}
	}

	seen_offset = tgt->cpp_rd_offset + tgt->map.offset;
	map_entry = &tgt->map;
	if ((NRD_GET_FQN(flags)) && nrd_strtab_add(&strtab, tgt->map.name, 1, 0) != 0) {
		NS_LOG(NS_LOG_LVL_ERROR, "Error building lookup string.");
		goto error_cleanup;
	}

	err = nrd_find_reg_in_maps(regdata, &map_entry, offset, &seen_offset, &mapidx1, &mapidx2,
				   (NRD_GET_FQN(flags)) ? &strtab : NULL,
				   (NRD_GET_FQN(flags)) ? &str : NULL);
	if (err < 0)
		goto error_cleanup;
	if (!map_entry)
		goto not_found;
	if (err == 1)
		found_partial = 1;

	err = EINVAL;

	if ((seen_offset == offset) && ((map_entry->type == NFP_REGDATA_NODE_TYPE_REG) ||
					(map_entry->type == NFP_REGDATA_NODE_TYPE_MULTIREG)))
		found_partial = 0;

	if (strtab.str) {
		if (!NRD_GET_FQN(flags)) {
			free(strtab.str);
		} else {
			if (strtab.str_cnt == 1)
				nrd_strtab_add(&strtab, "", 1, 1);

			cp = strtab.str + strlen(strtab.str);
			*cp = ':';
			cp++;

			for (idx = 1; idx < strtab.str_cnt - 1; idx++) {
				cp += strlen(cp);
				*cp = '.';
				cp++;
			}

			node->fqn = strtab.str;
			strtab.str = NULL; /* Prevent double free() */
		}
	}

	err = -nrd_entry_offset_patch_extract(regdata, tgt, map_entry, &fisland_id, &fmenum,
					      &foffset, 0);
	if (err != 0)
		goto error_cleanup;

	if (NRD_GET_FULLNAME(flags) && (map_entry->name)) {
		_MALLOC_OOM(node->name, (strlen(map_entry->name) +
					 (NRD_NUM64_DEC_DIGITS * map_entry->dimensions) + 1));
		sprintf(node->name, map_entry->name, (int)mapidx1, (int)mapidx2);
	}

	if (NRD_GET_ALTNAME(flags) && (map_entry->altname)) {
		_MALLOC_OOM(node->altname, (strlen(map_entry->altname) +
					    (NRD_NUM64_DEC_DIGITS * map_entry->dimensions) + 1));
		sprintf(node->altname, map_entry->altname, (int)mapidx1, (int)mapidx2);
	}

	if (NRD_GET_DESC(flags) && (map_entry->description)) {
		_MALLOC_OOM(node->description,
			    (strlen(map_entry->description) +
			     (NRD_NUM64_DEC_DIGITS * map_entry->dimensions) + 1));
		sprintf(node->description, map_entry->description, (int)mapidx1, (int)mapidx2);
	}

	node->cpp_tgt = tgt->cpp_tgt;
	node->bit_len = map_entry->ptr.reg->bit_len;
	node->cpp_rd_action = tgt->cpp_rd_action;
	node->cpp_rd_token = tgt->cpp_rd_token;
	node->cpp_rd_offset = foffset;
	node->cpp_wr_action = tgt->cpp_wr_action;
	node->cpp_wr_token = tgt->cpp_wr_token;
	node->offset = foffset;
	node->ctx = NFP_REGDATA_CTX_NA;
	node->field_cnt = map_entry->ptr.reg->field_count;
	node->fields = (const struct nfp_reg_bitf *)map_entry->ptr.reg->fields;
	node->iid = fisland_id;
	node->menum = fmenum;
	if (map_entry->reg_type != NFP_REG_TYPE_NONE)
		node->reg_type = map_entry->reg_type;
	else
		node->reg_type = tgt->reg_type;
	node->type = map_entry->type;
	node->offset_iid_lsb = tgt->offset_iid_lsb;
	node->offset_me_lsb = tgt->offset_me_lsb;
	node->is_sim_backdoorable = map_entry->is_sim_backdoorable;

	nrd_update_accessmode(node, node->field_cnt);
	nrd_update_reg_byte_offsets(regdata, map_entry, node);

	if (str.str)
		free(str.str);
	return (found_partial) ? 2 : 0;

not_found:
	if (strtab.str)
		free(strtab.str);
	if (str.str)
		free(str.str);
	return 1;

error_cleanup:
	nfp_regdata_node_free_data(node);
	if (strtab.str)
		free(strtab.str);
	if (str.str)
		free(str.str);

	return NFP_ERRNO(err);
}

int nfp_regdata_find_mecsr(struct nfp_regdata *regdata,
			   int island_id,
			   int menum,
			   int ctx,
			   uint64_t mecsr_offset,
			   unsigned int flags,
			   struct nfp_regdata_node *node)
{
	int err = EINVAL;
	struct nrd_strtab strtab = nrd_strtab_zinit;
	size_t idx;
	uint64_t mapidx1 = 0, mapidx2 = 0;
	struct nrd_target *tgt = NULL;
	struct nrd_addrmap_entry *map_entry = NULL;
	char *cp;
	int fmenum = NFP_REGDATA_MENUM_NA;
	int fisland_id = 0;
	uint64_t foffset = mecsr_offset;
	char idstr[NFP_MEID_STR_SZ];

	if (node) /* Clear first so error_cleanup doesn't segfault */
		memset(node, 0, sizeof(*node));

	if ((!regdata) || (!node)) {
		NS_LOG(NS_LOG_LVL_FATAL, "Invalid parameters.");
		err = EINVAL;
		goto error_cleanup;
	}

	for (idx = 0; idx < regdata->tgt_coll.count; idx++) {
		struct nrd_target *thistgt = &regdata->tgt_coll.entry[idx];

		if (thistgt->reg_type != NFP_REG_TYPE_MECSR)
			continue;
		if (tgt) {
			tgt = NULL;
			break;
		}
		tgt = thistgt;
	}

	if (!tgt)
		goto not_found;

	foffset += tgt->map.offset;

	if ((NRD_GET_FQN(flags)) && nrd_strtab_add(&strtab, tgt->map.name, 1, 0) != 0) {
		NS_LOG(NS_LOG_LVL_ERROR, "Error building lookup string.");
		goto error_cleanup;
	}

	if (island_id > 0) {
		fisland_id = island_id;
		if ((NRD_GET_FQN(flags)) &&
		    nrd_strtab_add(&strtab, nfp_island2str(regdata->family, idstr, island_id), 1, 0) !=
			    0) {
			NS_LOG(NS_LOG_LVL_ERROR, "Error building lookup string.");
			goto error_cleanup;
		}
	} else {
		fisland_id = NFP_REGDATA_IID_LOCAL;
	}

	if (menum >= 0) {
		fmenum = menum;
		if ((NRD_GET_FQN(flags)) &&
		    nrd_strtab_add(&strtab, nfp_menum2str(regdata->family, idstr, menum), 1, 0) != 0) {
			NS_LOG(NS_LOG_LVL_ERROR, "Error building lookup string.");
			goto error_cleanup;
		}
	} else if (menum < 0) {
		if (island_id > 0)
			goto not_found;
		fmenum = NFP_REGDATA_MENUM_LOCAL;
	} else {
		goto not_found;
	}

	map_entry = NULL;
	if (nrd_addrmap_entry_ensure_ptr(regdata, &tgt->map) != 0) {
		err = errno;
		goto error_cleanup;
	}
	for (idx = 0; idx < tgt->map.ptr.map->mapentries.count; idx++) {
		struct nrd_addrmap_entry *this_entry = &tgt->map.ptr.map->mapentries.entry[idx];

		if (this_entry->offset == mecsr_offset) {
			map_entry = this_entry;
			break;
		}
	}

	if ((!map_entry) || ((map_entry->type != NFP_REGDATA_NODE_TYPE_REG) &&
			     (map_entry->type != NFP_REGDATA_NODE_TYPE_MULTIREG) &&
			     (map_entry->reg_type != NFP_REG_TYPE_MECSR)))
		goto not_found;

	if ((ctx >= 0) && (map_entry->extra.mecsr.is_indirect_ctx_csr)) {
		if (ctx >= NRD_MAX_CTX_CNT)
			goto not_found;

		sprintf(idstr, "ctx%d", ctx);
		if ((NRD_GET_FQN(flags)) && (nrd_strtab_add(&strtab, idstr, 1, 0) != 0)) {
			NS_LOG(NS_LOG_LVL_ERROR, "Error building lookup string.");
			goto error_cleanup;
		}
	}

	if ((NRD_GET_FQN(flags)) && (nrd_strtab_add(&strtab, map_entry->name, 1, 0) != 0)) {
		NS_LOG(NS_LOG_LVL_ERROR, "Error building lookup string.");
		goto error_cleanup;
	}

	if (strtab.str) {
		if (!NRD_GET_FQN(flags)) {
			free(strtab.str);
		} else {
			cp = strtab.str + strlen(strtab.str);
			*cp = ':';
			cp++;

			for (idx = 1; idx < strtab.str_cnt - 1; idx++) {
				cp += strlen(cp);
				*cp = '.';
				cp++;
			}

			node->fqn = strtab.str;
			strtab.str = NULL;
		}
	}

	err = -nrd_entry_offset_patch_extract(regdata, tgt, map_entry, &fisland_id, &fmenum,
					      &foffset, 0);
	if (err != 0)
		goto error_cleanup;

	if (NRD_GET_FULLNAME(flags) && (map_entry->name)) {
		_MALLOC_OOM(node->name, (strlen(map_entry->name) +
					 (NRD_NUM64_DEC_DIGITS * map_entry->dimensions) + 1));
		sprintf(node->name, map_entry->name, (int)mapidx1, (int)mapidx2);
	}

	if (NRD_GET_ALTNAME(flags) && (map_entry->altname)) {
		_MALLOC_OOM(node->altname, (strlen(map_entry->altname) +
					    (NRD_NUM64_DEC_DIGITS * map_entry->dimensions) + 1));
		sprintf(node->altname, map_entry->altname, (int)mapidx1, (int)mapidx2);
	}

	if (NRD_GET_DESC(flags) && (map_entry->description)) {
		_MALLOC_OOM(node->description,
			    (strlen(map_entry->description) +
			     (NRD_NUM64_DEC_DIGITS * map_entry->dimensions) + 1));
		sprintf(node->description, map_entry->description, (int)mapidx1, (int)mapidx2);
	}

	node->cpp_tgt = tgt->cpp_tgt;
	node->bit_len = map_entry->ptr.reg->bit_len;
	node->cpp_rd_action = tgt->cpp_rd_action;
	node->cpp_rd_token = tgt->cpp_rd_token;
	node->cpp_rd_offset = foffset;
	node->cpp_wr_action = tgt->cpp_wr_action;
	node->cpp_wr_token = tgt->cpp_wr_token;
	node->offset = foffset;

	node->data.mecsr.offset = (uint16_t)map_entry->offset;
	node->data.mecsr.latency_wr = map_entry->extra.mecsr.latency_wr;
	node->data.mecsr.latency_rd = map_entry->extra.mecsr.latency_rd;
	node->data.mecsr.latency_use = map_entry->extra.mecsr.latency_use;
	if (map_entry->extra.mecsr.is_indirect_ctx_csr)
		node->ctx = (ctx >= 0) ? ctx : NFP_REGDATA_CTX_ANY;
	else
		node->ctx = NFP_REGDATA_CTX_NA;

	node->field_cnt = map_entry->ptr.reg->field_count;
	node->fields = (const struct nfp_reg_bitf *)map_entry->ptr.reg->fields;
	node->iid = fisland_id;
	node->menum = fmenum;
	if (map_entry->reg_type != NFP_REG_TYPE_NONE)
		node->reg_type = map_entry->reg_type;
	else
		node->reg_type = tgt->reg_type;
	node->type = map_entry->type;
	node->offset_iid_lsb = tgt->offset_iid_lsb;
	node->offset_me_lsb = tgt->offset_me_lsb;
	node->is_sim_backdoorable = map_entry->is_sim_backdoorable;

	nrd_update_accessmode(node, node->field_cnt);
	nrd_update_reg_byte_offsets(regdata, map_entry, node);

	return 0;
not_found:
	if (strtab.str)
		free(strtab.str);
	return 1;

error_cleanup:
	nfp_regdata_node_free_data(node);
	if (strtab.str)
		free(strtab.str);

	return NFP_ERRNO(err);
}

int nfp_regdata_find_mereg(struct nfp_regdata *regdata,
			   int island_id,
			   int menum,
			   enum nfp_mereg_type mereg_type,
			   uint64_t mereg_offset,
			   unsigned int flags,
			   struct nfp_regdata_node *node)
{
	int err = EINVAL;
	struct nrd_strtab strtab = nrd_strtab_zinit;
	struct nrd_str str = nrd_str_zinit;
	size_t idx;
	const struct nrd_mereg *mereg = NULL;
	char *cp;
	int fmenum = NFP_REGDATA_MENUM_NA;
	int fisland_id = 0;
	uint64_t foffset = 0;
	char idstr[NFP_MEID_STR_SZ];
	uint32_t regidx = 0;

	if (node) /* Clear first so error_cleanup doesn't segfault */
		memset(node, 0, sizeof(*node));

	if ((!regdata) || (!node)) {
		NS_LOG(NS_LOG_LVL_FATAL, "Invalid parameters.");
		err = EINVAL;
		goto error_cleanup;
	}

	if (regdata->arch != NFP_CHIP_ARCH_TH) {
		NS_LOG(NS_LOG_LVL_FATAL, "Unsupported chip family.");
		goto error_cleanup;
	}

	for (idx = 0; idx < regdata->mereg_coll.count; idx++) {
		struct nrd_mereg *thismereg = &regdata->mereg_coll.entry[idx];

		if (thismereg->type == mereg_type) {
			mereg = thismereg;
			break;
		}
	}

	if (!mereg)
		goto not_found;

	regidx = (int)(mereg_offset / 4);
	if ((mereg_offset & 0x3) || /* not 32-bit aligned */
	    (regidx >= mereg->count))
		goto not_found;

	if ((NRD_GET_FQN(flags)) && nrd_strtab_add(&strtab, NRD_MEREG_TGTNAME, 1, 0) != 0) {
		NS_LOG(NS_LOG_LVL_ERROR, "Error building lookup string.");
		goto error_cleanup;
	}

	if (island_id > 0) {
		fisland_id = island_id;
		if ((NRD_GET_FQN(flags)) &&
		    nrd_strtab_add(&strtab, nfp_island2str(regdata->family, idstr, island_id), 1, 0) !=
			    0) {
			NS_LOG(NS_LOG_LVL_ERROR, "Error building lookup string.");
			goto error_cleanup;
		}
	} else {
		fisland_id = NFP_REGDATA_IID_LOCAL;
	}

	if (menum >= 0) {
		fmenum = menum;
		if ((NRD_GET_FQN(flags)) &&
		    nrd_strtab_add(&strtab, nfp_menum2str(regdata->family, idstr, menum), 1, 0) != 0) {
			NS_LOG(NS_LOG_LVL_ERROR, "Error building lookup string.");
			goto error_cleanup;
		}
	} else if (menum < 0) {
		if (island_id > 0)
			goto not_found;
		fmenum = NFP_REGDATA_MENUM_LOCAL;
	} else {
		goto not_found;
	}

	if (!nrd_tgt_iidme_valid(regdata, NULL, fisland_id, fmenum))
		goto not_found;

	if (NRD_GET_FQN(flags)) {
		if (nrd_str_resize(&str, strlen(mereg->name) + NRD_NUM64_DEC_DIGITS + 1)) {
			NS_LOG(NS_LOG_LVL_ERROR, "Error building lookup string.");
			goto error_cleanup;
		}
		sprintf(str.str, mereg->name, (int)(mereg_offset / sizeof(uint32_t)));
		if (nrd_strtab_add(&strtab, str.str, 1, 0) != 0) {
			NS_LOG(NS_LOG_LVL_ERROR, "Error building lookup string.");
			goto error_cleanup;
		}
	}

	if (strtab.str) {
		if (!NRD_GET_FQN(flags)) {
			free(strtab.str);
		} else {
			cp = strtab.str + strlen(strtab.str);
			*cp = ':';
			cp++;

			for (idx = 1; idx < strtab.str_cnt - 1; idx++) {
				cp += strlen(cp);
				*cp = '.';
				cp++;
			}

			node->fqn = strtab.str;
			strtab.str = NULL;
		}
	}

	if ((mereg->iid_lsb != -1) && (fisland_id >= 0))
		foffset |= (fisland_id << mereg->iid_lsb);
	if ((mereg->me_lsb != -1) && (fmenum >= 0))
		/* NFP_CHIP_FAMILY_NFP6000/ARCH_TH specific */
		foffset |= ((fmenum + 4) << mereg->me_lsb);

	if (mereg->byteaddr_lsb >= 0)
		foffset |= (mereg_offset << mereg->byteaddr_lsb);
	else
		foffset |= (mereg_offset >> (-mereg->byteaddr_lsb));

	if (NRD_GET_FULLNAME(flags) && (mereg->name)) {
		_MALLOC_OOM(node->name, (strlen(mereg->name) + NRD_NUM64_DEC_DIGITS + 1));
		sprintf(node->name, mereg->name, regidx);
	}

	if (NRD_GET_ALTNAME(flags) && (mereg->altname)) {
		_MALLOC_OOM(node->altname, (strlen(mereg->altname) + NRD_NUM64_DEC_DIGITS + 1));
		sprintf(node->altname, mereg->altname, regidx);
	}

	if (NRD_GET_DESC(flags) && (mereg->description)) {
		_MALLOC_OOM(node->description,
			    (strlen(mereg->description) + NRD_NUM64_DEC_DIGITS + 1));
		sprintf(node->description, mereg->description, regidx);
	}

	node->cpp_rd_action = mereg->cpp_rd_action;
	node->cpp_rd_token = mereg->cpp_rd_token;
	node->cpp_rd_offset = foffset + mereg->cpp_rd_offset;
	node->cpp_wr_action = mereg->cpp_wr_action;
	node->cpp_wr_token = mereg->cpp_wr_token;
	node->offset = foffset + mereg->cpp_wr_offset;

	node->cpp_tgt = (int8_t)mereg->cpp_tgt;
	node->bit_len = 32;

	node->data.mereg.mereg_type = mereg_type;
	node->data.mereg.reg_idx = (int)(mereg_offset / 4);

	node->ctx = NFP_REGDATA_CTX_NA;

	node->field_cnt = 0;
	node->fields = NULL;
	node->iid = fisland_id;
	node->menum = fmenum;
	node->reg_type = NFP_REG_TYPE_MEREG;
	node->type = NFP_REGDATA_NODE_TYPE_REG;
	node->offset_iid_lsb = mereg->iid_lsb;
	node->offset_me_lsb = mereg->me_lsb;
	node->is_sim_backdoorable = 1;
	node->is_readable = 1;
	node->is_writable = 1;

	if (str.str)
		free(str.str);

	return 0;
not_found:
	if (strtab.str)
		free(strtab.str);
	if (str.str)
		free(str.str);
	return 1;

error_cleanup:
	nfp_regdata_node_free_data(node);
	if (strtab.str)
		free(strtab.str);
	if (str.str)
		free(str.str);

	return NFP_ERRNO(err);
}

#ifdef _MSC_VER
#pragma warning(pop)
#endif
