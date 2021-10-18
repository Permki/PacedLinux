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

#include "libnfp_common.h"

#include <nfp-common/nfp_resid.h>
#include <nfp-common/nfp_util.h>
#include <nfp-common/nfp_chipdata.h>
#include <nfp-common/nfp_cppat.h>
#include <nfp-common/ns_log.h>

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4706)
#endif

#ifdef _WIN32
#  define PATH_SEP_C '\\'
#  define PATH_SEP_S "\\"
#  define PATH_LIST_SEP_C ';'
#  ifndef NFP_CHIPDATA_BIN_REL_PATH
#    define NFP_CHIPDATA_BIN_REL_PATH "..\\nfp_chipdata"
#  endif
#else
#  define PATH_SEP_C '/'
#  define PATH_SEP_S "/"
#  define PATH_LIST_SEP_C ':'
#  ifndef NFP_CHIPDATA_BIN_REL_PATH
#    define NFP_CHIPDATA_BIN_REL_PATH "../nfp_chipdata"
#  endif
#endif

#define NFP_CHIP_FAMILY_NFP4000 0x4000	/* ARCH_TH */
#define NFP_CHIP_FAMILY_NFP5000 0x5000	/* ARCH_TH */

#define _NFP_ARCH(x) ((x == NFP_CHIP_FAMILY_NFP4000 || \
	x == NFP_CHIP_FAMILY_NFP5000) ? \
	NFP_CHIP_ARCH_TH : NFP_FAMILY_ARCH(x))

static nfp_inline char *safe_strdup(const char *s)
{
	char *ret;

	if (!s)
		return NULL;
	ret = strdup(s);
	if (!ret)
		NS_LOG(NS_LOG_LVL_ERROR, "Out of memory.");
	return ret;
}

#define _CALLOC_OOM(_dst_, _n_, _sz_)                               \
	do {                                                        \
		if (((_sz_) == 0) || ((_n_) == 0)) {                \
			_dst_ = NULL;                               \
			break;                                      \
		}                                                   \
		_dst_ = calloc((_n_), (_sz_));                      \
		if (!(_dst_)) {                              \
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
		if (!(_dst_)) {                              \
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
		if (!(_newmem_)) {                           \
			err = ENOMEM;                               \
			NS_LOG(NS_LOG_LVL_FATAL, "Out of memory."); \
			goto error_cleanup;                         \
		}                                                   \
		_dst_ = _newmem_;                                   \
	} while (0)

#define _STRDUP_OOM(_dst_, _s_)                                     \
	do {                                                        \
		if (!(_s_)) {                                \
			_dst_ = NULL;                               \
			break;                                      \
		}                                                   \
		_dst_ = strdup((_s_));                              \
		if (!(_dst_)) {                              \
			err = ENOMEM;                               \
			NS_LOG(NS_LOG_LVL_FATAL, "Out of memory."); \
			goto error_cleanup;                         \
		}                                                   \
	} while (0)

#define _STRTAB_ADD_OOM(_dst_, _strtab_, _s_)                       \
	do {                                                        \
		size_t n = nsc_strtab_add(_strtab_, _s_);           \
		if (n == SIZE_MAX) {                                \
			err = ENOMEM;                               \
			NS_LOG(NS_LOG_LVL_FATAL, "Out of memory."); \
			goto error_cleanup;                         \
		}                                                   \
		_dst_ = n;                                          \
	} while (0)

#define _OOM_CHECK(_v_)                                             \
	do {                                                        \
		if (!(_v_)) {                                \
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
		if ((_array_)->capacity > (_array_)->count)                              \
			memset(&(_array_)->entry[(_array_)->count], 0,                   \
			       sizeof(*(_array_)->entry) *                               \
				       ((_array_)->capacity - (_array_)->count));        \
	} while (0)

#define _ARRAY_EXTEND(_array_, _extra_count_)                                     \
	do {                                                                      \
		_REALLOC_OOM((_array_)->entry,                                    \
			     ((_extra_count_) + (_array_)->capacity) *            \
				     sizeof((_array_)->entry[0]));                \
		(_array_)->capacity += _extra_count_;                             \
		if ((_array_)->capacity > (_array_)->count)                       \
			memset(&(_array_)->entry[(_array_)->count], 0,            \
			       sizeof(*(_array_)->entry) *                        \
				       ((_array_)->capacity - (_array_)->count)); \
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

#if !defined(EMBEDDED)
#define NSC_JOBJ_strdup(_dst_, _obj_, _key_) \
	_STRDUP_OOM((_dst_), json_string_value(json_object_get((_obj_), (_key_))))

#if (JANSSON_VERSION_HEX < 0x020300)
#define json_object_foreach(object, key, value)                                         \
	for (key = json_object_iter_key(json_object_iter(object));                      \
	     key && (value = json_object_iter_value(json_object_iter_at(object, key))); \
	     key = json_object_iter_key(                                                \
		     json_object_iter_next(object, json_object_iter_at(object, key))))
#endif

#define NSC_JOBJ_FOREACH(object, key, value, num)                                              \
	for (num = 0, key = json_object_iter_key(json_object_iter(object));                    \
	     key && (value = json_object_iter_value(json_object_iter_at(object, key))); num++, \
	    key = json_object_iter_key(                                                        \
		    json_object_iter_next(object, json_object_iter_at(object, key))))

#define NSC_FILE_NAME_CHIPS_INTERNAL "chips.json"
#define NSC_FILE_NAME_DESC_INTERNAL "desc.json"

#endif

#define NSC_STR(s) (&data->strtab.str[s])

#define NSC_NFP3200_NUM_MES 40
#define NSC_NFP3200_ME_PER_CLUSTER 8
#define NSC_NFP3200_CLUSTER_CNT_MAX 5

struct nsc_strtab {
	size_t alloced, offset;
	char *str;
};

struct nsc_llist_node {
	struct nsc_llist_node *next;
};

struct nsc_llist {
	size_t cnt;
	struct nsc_llist_node *node;
};

struct nsc_chip_nfp3200 {
	_ARRAY_DECLARE(int, cpp_targets);
	size_t me_cnt;
};

struct nsc_chip_nfp6000 {
	_ARRAY_DECLARE(int, islands);
	size_t serdes_cnt;
	size_t max_me_cnt; // Highest island ME count chip has
};

struct nsc_desc_cpptgt_nfpxxxx {
	int tgt_id;
	size_t name;
	size_t name_long;
};

struct nsc_desc_island_mem_cfg {
	uint64_t base;
	uint64_t alloc_base;
	uint64_t size;
};

struct nsc_desc_island_nfp6000 {
	int id;
	enum nfp_chipdata_isld_type island_type;
	size_t name;

	int *cpp_tgt_ids;
	size_t cpp_tgt_id_count;

	enum nfp_chipdata_mem_type *mem_types;
	size_t mem_types_count;
	struct nsc_desc_island_mem_cfg *mem_cfgs;

	enum nfp_chipdata_mu_type mu_type; // for CPP tgt 7

	size_t me_cnt;
};

struct _nfp_chipdata_memory {
	int cpp_tgt; /* -1 if NA (ME local mem) */
	enum nfp_chipdata_mem_type type;
	size_t name;

	/* An absolute resource base (e.g. QDR channel offsets) */
	uint64_t base;

	/* Base in resoure from which to start allocating symbols for the chip family as a whole. */
	uint64_t alloc_base;

	uint64_t size_max;
};

struct nsc_chipdata_alloc_alias {
	size_t alias;
	size_t name;
};

struct nsc_chip_desc_nfp6000 {
	_ARRAY_DECLARE(struct nsc_desc_cpptgt_nfpxxxx, cpp_targets);
	_ARRAY_DECLARE(struct nsc_desc_island_nfp6000, islands);
	_ARRAY_DECLARE(struct _nfp_chipdata_memory, memories);
	_ARRAY_DECLARE(struct _nfp_chipdata_alloc_type, alloc_types);
	_ARRAY_DECLARE(struct nsc_chipdata_alloc_alias, alloc_aliases);
};

struct nsc_chip_desc_nfp3200 {
	_ARRAY_DECLARE(struct nsc_desc_cpptgt_nfpxxxx, cpp_targets);
	_ARRAY_DECLARE(struct _nfp_chipdata_memory, memories);
	_ARRAY_DECLARE(struct _nfp_chipdata_alloc_type, alloc_types);
	_ARRAY_DECLARE(struct nsc_chipdata_alloc_alias, alloc_aliases);
	int cluster_count;
};

struct nsc_chip_desc {
	struct nsc_llist_node node; /* Must be first in struct */

	char *file_path; // NULL for default Netronome-created desc.json
	int arch;
	int family;
	int rev_min;
	int rev_max;
	union {
		struct nsc_chip_desc_nfp3200 nfp3200;
		struct nsc_chip_desc_nfp6000 nfp6000;
	} desc;
};

struct nsc_chip_cfg {
	size_t k;
	size_t v;
};

struct nfp_chipdata_chip {
	struct nsc_llist_node node; /* Must be first in struct */

	size_t source_file;
	struct nfp_chipdata *chipdata;
	char *id;
	int arch;	/* #defines in nfp_resid.h */
	int family;	/* #defines in nfp_resid.h */
	char *description;
	int rev_min;
	int rev_max;

	_ARRAY_DECLARE(int, revisions);
	_ARRAY_DECLARE(uint32_t, revision_models);

	char *desc_override;
	char *inherit;
	int is_board;
	struct nfp_chipdata_chip *parent_chip;

	_ARRAY_DECLARE(union nfp_chipdata_cppat_entry, cppats);

	_ARRAY_DECLARE(struct nsc_chip_cfg, cfgs);

	union {
		struct nsc_chip_nfp3200 nfp3200;
		struct nsc_chip_nfp6000 nfp6000;
	} details;
	union {
		void *vp;
		struct nsc_chip_desc_nfp3200 *nfp3200;
		struct nsc_chip_desc_nfp6000 *nfp6000;
	} chip_desc;
};

struct nfp_chipdata {
	char *chipdata_path;
	int embedded_loaded;
	struct nsc_strtab search_paths;
	struct nsc_llist chip_decls; /* struct nfp_chipdata_chip */
	struct nsc_llist chip_descs; /* struct nsc_chip_desc */
	char *json_comment;
	struct nsc_strtab strtab; /* Easier inheritance with global strtab */
};

/* These structs MUST always be the same as their counterparts
 * in the header file, except for dropping 'const'.
 */
struct _nfp_chipdata_alloc {
	enum nfp_chipdata_mem_type type;
	enum nfp_chipdata_alloc_policy policy;
	size_t islands_count;
	int *islands;
	size_t targets_count;
	int *targets;
	uint64_t base; /* This is relative to nfp_chipdata_memory.base */
	uint64_t size_max;
};

struct _nfp_chipdata_alloc_type {
	size_t name;
	int local_alloc;
	size_t entries_count;
	struct _nfp_chipdata_alloc *entries;
};

struct nsc_enum_string_map {
	const char *str;
	int val;
};

static int nsc_comp_int_asc(const void *p1, const void *p2);

static void nsc_strtab_free(struct nsc_strtab *strtab);

#if !defined(EMBEDDED)
static void *nsc_llist_add(struct nsc_llist *llist, size_t data_sz);
static size_t nsc_strtab_add(struct nsc_strtab *strtab, const char *str);
static int nsc_sanity_check();
static int nsc_comp_chipdecl_asc(const void *p1, const void *p2);
static int nsc_comp_nfp6000_cppat_asc(const void *p1, const void *p2);
static int nsc_paths_to_strtab(struct nsc_strtab *strtab, const char *search_paths);

static int nsc_j_obj_strnum_get(json_t *j, const char *key, uint64_t *val);

static json_t *nsc_json_file_load(struct nfp_chipdata *data, const char *file_name);

static json_t *nsc_json_file_load_embedded();

static int nsc_json_extract_chip(struct nfp_chipdata *data,
				 const char *id,
				 json_t *j_chip,
				 int chip_num,
				 const char *source_file);

static int nsc_json_extract_chips(struct nfp_chipdata *data,
				  json_t *j_root,
				  const char *source_file,
				  int extract_boards);

static int nsc_json_extract_desc_nfp6000_top(struct nfp_chipdata *data,
					     json_t *j_root,
					     const char *source_file,
					     int subfamily);

static int nsc_json_extract_desc_nfp6000(struct nfp_chipdata *data,
					 json_t *j_root,
					 const char *source_file,
					 int subfamily,
					 int rev_min,
					 int rev_max);

static int nsc_json_extract_desc_nfp3200_top(struct nfp_chipdata *data,
					     json_t *j_root,
					     const char *source_file);

static int nsc_json_extract_desc_nfp3200(struct nfp_chipdata *data,
					 json_t *j_root,
					 const char *source_file,
					 int rev_min,
					 int rev_max);

static struct nfp_chipdata_chip *nsc_chip_decl_find_by_id(struct nfp_chipdata *data,
							  const char *id);

static struct nsc_chip_desc *nsc_chip_desc_find_by_path(struct nfp_chipdata *data,
							const char *path,
							int rev_min,
							int rev_max);
static int nsc_chip_desc_dup(struct nfp_chipdata_chip *chip);
static int nsc_chip_apply_cfg(struct nfp_chipdata_chip *chip);

#define NSC6000_EXPAND_SIID "{siid}"
#define NSC6000_EXPAND_NIID "{niid}"

#define NSC6000_EXPAND_FLAG_SIID BIT(0)
#define NSC6000_EXPAND_FLAG_NIID BIT(1)

static char *nsc6000_expand_fmtstr(const char *fmtstr, int iid, int altstr);

static int nsc_nfp3200_chip_apply_cfg(struct nfp_chipdata_chip *chip);
static int nsc_nfp6000_chip_apply_cfg(struct nfp_chipdata_chip *chip);

static struct nsc_desc_island_nfp6000 *nsc_nfp6000_island_find_by_iid_nc(
	struct nfp_chipdata_chip *chip,
	int iid);
#endif /* !defined(EMBEDDED) */

/* For default, non-override descs */
static struct nsc_chip_desc *nsc_chip_desc_find_by_family(struct nfp_chipdata *data,
							  int family,
							  int rev_min,
							  int rev_max);

static const struct nsc_desc_island_nfp6000 *nsc_nfp6000_island_find_by_iid(
	const struct nfp_chipdata_chip *chip,
	int iid);

static int nsc_enum_from_string(const struct nsc_enum_string_map *str_map, const char *str);

static int nsc_enum_from_stringn(const struct nsc_enum_string_map *str_map,
				 const char *str,
				 size_t n);

static const char *nsc_enum_to_string(const struct nsc_enum_string_map *str_map,
				      int val,
				      const char *default_str);

static void nsc_chip_desc_free(struct nsc_chip_desc *desc);

static void _nfp_chipdata_chip_free(struct nfp_chipdata_chip *chip);

// These should match the strings in the JSON files.
static struct nsc_enum_string_map nsc_family_string_map[] = {
	{"nfp3200", NFP_CHIP_FAMILY_NFP3200},
	{"nfp6000", NFP_CHIP_FAMILY_NFP6000},
	{"nfp5000", NFP_CHIP_FAMILY_NFP5000},
	{"nfp4000", NFP_CHIP_FAMILY_NFP4000},
	{"nfp3800", NFP_CHIP_FAMILY_NFP3800},
	{NULL, 0}
};

// static struct nsc_enum_string_map nsc_nfp3200_mu_type_string_map[] = {
//    {"DDR", (int)NFP_CHIPDATA_MU_TYPE_DDR},
//    {NULL,  (int)NFP_CHIPDATA_MU_TYPE_NONE}
//};

// The map is searched case-insensitively and first match is taken,
// so if we want to show "SRAM0" rather than "QDR0", that should be first.
static struct nsc_enum_string_map nsc_nfp6000_mem_type_string_map[] = {
	{"CTM", (int)NFP_CHIPDATA_MEM_TYPE_CTM},
	{"IMEM", (int)NFP_CHIPDATA_MEM_TYPE_IMEM},
	{"EMEM", (int)NFP_CHIPDATA_MEM_TYPE_EMEM},
	{"EMEM_CACHE", (int)NFP_CHIPDATA_MEM_TYPE_EMEM_CACHE},
	{"SRAM0", (int)NFP_CHIPDATA_MEM_TYPE_QDR0},
	{"SRAM1", (int)NFP_CHIPDATA_MEM_TYPE_QDR1},
	{"SRAM2", (int)NFP_CHIPDATA_MEM_TYPE_QDR2},
	{"SRAM3", (int)NFP_CHIPDATA_MEM_TYPE_QDR3},
	{"QDR0", (int)NFP_CHIPDATA_MEM_TYPE_QDR0},
	{"QDR1", (int)NFP_CHIPDATA_MEM_TYPE_QDR1},
	{"QDR2", (int)NFP_CHIPDATA_MEM_TYPE_QDR2},
	{"QDR3", (int)NFP_CHIPDATA_MEM_TYPE_QDR3},
	{"CLS", (int)NFP_CHIPDATA_MEM_TYPE_CLS},
	{"LMEM", (int)NFP_CHIPDATA_MEM_TYPE_LM},
	{"LM", (int)NFP_CHIPDATA_MEM_TYPE_LM}, // Microengine local memory
	{"USTORE", (int)NFP_CHIPDATA_MEM_TYPE_USTORE},
	{"CRYPTO", (int)NFP_CHIPDATA_MEM_TYPE_CRYPTO},
	{"ILA_SRAM", (int)NFP_CHIPDATA_MEM_TYPE_ILA},
	{NULL, (int)NFP_CHIPDATA_MEM_TYPE_NONE}
};

static struct nsc_enum_string_map nsc_nfp3200_mem_type_string_map[] = {
	{"SRAM0", (int)NFP_CHIPDATA_MEM_TYPE_QDR0},
	{"SRAM1", (int)NFP_CHIPDATA_MEM_TYPE_QDR1},
	{"SRAM2", (int)NFP_CHIPDATA_MEM_TYPE_QDR2},
	{"SRAM3", (int)NFP_CHIPDATA_MEM_TYPE_QDR3},
	{"QDR0", (int)NFP_CHIPDATA_MEM_TYPE_QDR0},
	{"QDR1", (int)NFP_CHIPDATA_MEM_TYPE_QDR1},
	{"QDR2", (int)NFP_CHIPDATA_MEM_TYPE_QDR2},
	{"QDR3", (int)NFP_CHIPDATA_MEM_TYPE_QDR3},
	{"DRAM", (int)NFP_CHIPDATA_MEM_TYPE_DDR},
	{"DDR", (int)NFP_CHIPDATA_MEM_TYPE_DDR},
	{"GS", (int)NFP_CHIPDATA_MEM_TYPE_GS},
	{"SCRATCH", (int)NFP_CHIPDATA_MEM_TYPE_GS},
	{"CLS", (int)NFP_CHIPDATA_MEM_TYPE_CLS},
	{"LMEM", (int)NFP_CHIPDATA_MEM_TYPE_LM},
	{"LM", (int)NFP_CHIPDATA_MEM_TYPE_LM}, // Microengine local memory
	{"USTORE", (int)NFP_CHIPDATA_MEM_TYPE_USTORE},
	{"CRYPTO", (int)NFP_CHIPDATA_MEM_TYPE_CRYPTO},
	{NULL, (int)NFP_CHIPDATA_MEM_TYPE_NONE}
};

#if !defined(EMBEDDED)
static struct nsc_enum_string_map nsc_nfp6000_island_string_map[] = {
	{"ARM", (int)NFP_CHIPDATA_ISLD_TYPE_ARM},
	{"PCIE", (int)NFP_CHIPDATA_ISLD_TYPE_PCIE},
	{"NBI", (int)NFP_CHIPDATA_ISLD_TYPE_NBI},
	{"Crypto", (int)NFP_CHIPDATA_ISLD_TYPE_CRYPTO},
	{"ExtMuEng", (int)NFP_CHIPDATA_ISLD_TYPE_MU_ENG_EXT},
	{"IntMuEng", (int)NFP_CHIPDATA_ISLD_TYPE_MU_ENG_INT},
	{"MeIsld", (int)NFP_CHIPDATA_ISLD_TYPE_MECL},
	{"ILA", (int)NFP_CHIPDATA_ISLD_TYPE_ILA},
	{NULL, (int)NFP_CHIPDATA_ISLD_TYPE_NONE}
};

static struct nsc_enum_string_map nsc_nfp6000_mu_type_string_map[] = {
	{"CTM", NFP_CHIPDATA_MU_TYPE_CTM},
	{"IMEM", NFP_CHIPDATA_MU_TYPE_IMEM},
	{"EMEM", NFP_CHIPDATA_MU_TYPE_EMEM},
	{NULL, NFP_CHIPDATA_MU_TYPE_NONE}
};

static struct nsc_enum_string_map nsc_alloc_type_policy_string_map[] = {
	{"spread", (int)NFP_CHIPDATA_ALLOC_POLICY_SPREAD},
	{"exhaust", (int)NFP_CHIPDATA_ALLOC_POLICY_EXHAUST},
	{NULL, (int)NFP_CHIPDATA_ALLOC_POLICY_INVALID},
};

int nsc_sanity_check()
{
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4127)
// Disabled warning: "conditional expression is constant"
// which, for some reason, only appears after modifying ns_log...
#endif

#define NSC_CHECK(_s_, _m_) do {                                       \
	if (offsetof(struct _s_, _m_) != offsetof(struct _##_s_, _m_)) \
		return 0; \
	} while (0)

	NSC_CHECK(nfp_chipdata_alloc, type);
	NSC_CHECK(nfp_chipdata_alloc, policy);
	NSC_CHECK(nfp_chipdata_alloc, islands);
	NSC_CHECK(nfp_chipdata_alloc, islands_count);
	NSC_CHECK(nfp_chipdata_alloc, targets);
	NSC_CHECK(nfp_chipdata_alloc, targets_count);

	NSC_CHECK(nfp_chipdata_alloc_type, name);
	NSC_CHECK(nfp_chipdata_alloc_type, local_alloc);
	NSC_CHECK(nfp_chipdata_alloc_type, entries);
	NSC_CHECK(nfp_chipdata_alloc_type, entries_count);

#undef NSC_CHECK

	if (offsetof(struct nfp_chipdata_chip, node.next) != 0)
		return 0;

#ifdef _MSC_VER
#pragma warning(pop)
#endif

	return 1;
}

json_t *nsc_json_file_load(struct nfp_chipdata *data, const char *path)
{
	json_t *j = NULL;
	json_error_t j_err;

	if ((!data) || (!path)) {
		NS_LOG(NS_LOG_LVL_FATAL, "Invalid parameters.");
		return NULL;
	}

	NS_LOG(NS_LOG_LVL_EXTRA, "Loading JSON file: %s", path);
	j = json_load_file(path, 0, &j_err);
	if (!j) {
		NS_LOG(NS_LOG_LVL_FATAL, "Jansson error: %s, source %s:%d:%d", j_err.text,
		       j_err.source, j_err.line, j_err.column);
	}

	return j;
}

json_t *nsc_json_file_load_embedded()
{
	char *embedded_json_str = NULL;
	json_t *j = NULL;
	json_error_t j_err;
	unsigned long sz = nfp_chipdata_embedded_usz;
	int err = EINVAL;

	NS_LOG(NS_LOG_LVL_EXTRA, "Loading embedded JSON data");

	_CALLOC_OOM(embedded_json_str, nfp_chipdata_embedded_usz + 1, 1);
	if (uncompress((Bytef *)embedded_json_str, &sz, nfp_chipdata_embedded,
		       nfp_chipdata_embedded_sz) != Z_OK) {
		err = EBADF;
		NS_LOG(NS_LOG_LVL_FATAL, "Could not uncompress embedded data");
		goto error_cleanup;
	}

	j = json_loads(embedded_json_str, 0, &j_err);
	if (!j) {
		NS_LOG(NS_LOG_LVL_FATAL, "Jansson error: %s, source %s:%d:%d", j_err.text,
		       j_err.source, j_err.line, j_err.column);
		err = EBADF;
		goto error_cleanup;
	}

	free(embedded_json_str);
	return j;

error_cleanup:
	if (!!embedded_json_str)
		free(embedded_json_str);
	if (!!j)
		json_decref(j);
	return NFP_ERRPTR(err);
}

struct _nsc_json_gather_cfg_ctx {
	/* Unique to each recursion */
	const char *name;
	struct _nsc_json_gather_cfg_ctx *parent;
	struct _nsc_json_gather_cfg_ctx *root;
	size_t fqn_mark;

	/* Root only */
	char *fqn;
	size_t fqn_len; /* excl. '\0' */
	size_t fqn_cap;
};

int nsc_json_gather_cfg(struct nfp_chipdata_chip *chip,
			json_t *j_cfg,
			struct _nsc_json_gather_cfg_ctx *parent_ctx)
{
	struct _nsc_json_gather_cfg_ctx ctx;
	json_t *j_val;
	int err = EINVAL;

	ctx.name = NULL;
	ctx.parent = parent_ctx;
	ctx.root = &ctx;
	ctx.fqn_mark = 0;
	ctx.fqn = NULL;
	ctx.fqn_cap = 0;
	ctx.fqn_len = 0;

	if (parent_ctx) {
		ctx.root = parent_ctx->root;
	} else {
		_CALLOC_OOM(ctx.fqn, 8, 1);
		ctx.fqn_len = 0;
		ctx.fqn_cap = 8;
	}

	ctx.fqn_mark = ctx.root->fqn_len;

	json_object_foreach(j_cfg, ctx.name, j_val) {
		size_t nm_len;

		if (ctx.name[0] == '_')
			continue;

		nm_len = strlen(ctx.name);

		ctx.root->fqn_len = ctx.fqn_mark;
		while ((ctx.root->fqn_len + 1 + nm_len + 1) >= ctx.root->fqn_cap) {
			_REALLOC_OOM(ctx.root->fqn, ctx.root->fqn_cap * 2);
			ctx.root->fqn_cap *= 2;
		}

		if (ctx.root->fqn_len)
			ctx.root->fqn[ctx.root->fqn_len++] = '.';
		strncpy(&ctx.root->fqn[ctx.root->fqn_len], ctx.name, nm_len + 1);
		ctx.root->fqn_len += nm_len;

		if (json_is_object(j_val)) {
			err = nsc_json_gather_cfg(chip, j_val, &ctx);
			if (err)
				goto error_cleanup;
		} else {
			struct nsc_chip_cfg *cfg = NULL;
			const char *v = json_string_value(j_val);

			if (!v) {
				err = EINVAL;
				goto error_cleanup;
			}

			if (chip->cfgs.count == chip->cfgs.capacity)
				_ARRAY_EXTEND(&chip->cfgs, 4);
			cfg = &chip->cfgs.entry[chip->cfgs.count++];
			_STRTAB_ADD_OOM(cfg->k, &chip->chipdata->strtab, ctx.root->fqn);
			_STRTAB_ADD_OOM(cfg->v, &chip->chipdata->strtab, v);
		}
	}
	if (!parent_ctx)
		free(ctx.fqn);
	return 0;

error_cleanup:
	NS_LOG(NS_LOG_LVL_FATAL, "Error processing config for chip '%s': %s", chip->id,
	       (ctx.root) ? ctx.root->fqn : "<bad tree>");
	return NFP_ERRNO(err);
}

int nsc_json_extract_chip(struct nfp_chipdata *data,
			  const char *id,
			  json_t *j_chip,
			  int chip_num,
			  const char *source_file)
{
	json_t *j_topitem = j_chip;
	json_t *j_tmp, *j_tmp2;
	uint64_t tmpu64;
	int err = EINVAL;
	size_t cnt;
	const char *iidstr;
	union nfp_chipdata_cppat_entry *cppat;
	struct nfp_chipdata_chip *decl = NULL;

	if (!!nsc_chip_decl_find_by_id(data, id))
		return 0;

	decl = nsc_llist_add(&data->chip_decls, sizeof(*decl));
	_OOM_CHECK(decl);

	decl->rev_min = INT32_MAX;
	decl->rev_max = -1;
	decl->is_board = (chip_num >= 0);
	_STRTAB_ADD_OOM(decl->source_file, &data->strtab, source_file);

	decl->chipdata = data;
	if (!!strchr(id, ':')) {
		err = EINVAL;
		NS_LOG(NS_LOG_LVL_FATAL,
		       "Invalid name for chip '%s' in '%s', may not contain ':'",
		       decl->id, source_file);
		goto error_cleanup;
	}

	if (chip_num < 0) {
		_STRDUP_OOM(decl->id, id);
	} else {
		// Space for NFP_MAX_DEVICE_NUM in "name:%d"
		cnt = strlen(id) + 1 + 2 + 1;
		_MALLOC_OOM(decl->id, cnt);
		snprintf(decl->id, cnt, "%s:%d", id, chip_num);
	}

	decl->family = nsc_enum_from_string(
		nsc_family_string_map, json_string_value(json_object_get(j_topitem, "family")));
	if (decl->family == 0) {
		err = ENOENT;
		NS_LOG(NS_LOG_LVL_FATAL, "Invalid family for chip '%s' in '%s'", decl->id,
		       source_file);
		goto error_cleanup;
	}
	decl->arch = _NFP_ARCH(decl->family);

	NSC_JOBJ_strdup(decl->description, j_topitem, "description");
	{
		const char *cs = json_string_value(json_object_get(j_topitem, "chip_desc"));
		char *c;

		if (!!cs) {
			_MALLOC_OOM(decl->desc_override, strlen(source_file) + 1 + strlen(cs) + 1);
			strcpy(decl->desc_override, source_file);
			c = strrchr(decl->desc_override, PATH_SEP_C);
			if (!c) {
				// The caller had to ensure that PATH_SEP_C is
				// preceeding the file name.
				NS_LOG(NS_LOG_LVL_FATAL, "Internal error.");
				goto error_cleanup;
			}
			c++;
			strcpy(c, cs);

			if (!nfp_util_normalize_abspath(decl->desc_override)) {
				NS_LOG(NS_LOG_LVL_FATAL, "Error cleaning up path: %s",
				       decl->desc_override);
				goto error_cleanup;
			}
		}
	}

	NSC_JOBJ_strdup(decl->inherit, j_topitem, "inherit");

	j_tmp = json_object_get(j_topitem, "revisions");
	j_tmp2 = json_object_get(j_topitem, "models");
	if (json_array_size(j_tmp) > 0) {
		size_t idx;

		_ARRAY_ALLOC(&decl->revisions, json_array_size(j_tmp));
		_ARRAY_ALLOC(&decl->revision_models, json_array_size(j_tmp));

		for (idx = 0; idx < decl->revisions.count; idx++) {
			const char *s;
			int rev;

			s = json_string_value(json_array_get(j_tmp, idx));
			rev = nfp_idstr2rev(s);
			if (rev == -1) {
				if (!s)
					s = "<null>";
				NS_LOG(NS_LOG_LVL_FATAL,
				       "Bad revision ('%s') for chip '%s' in '%s'",
				       s, decl->id, source_file);
				goto error_cleanup;
			}
			decl->revisions.entry[idx] = rev;
			decl->rev_min = NFP_MIN(decl->rev_min, rev);
			decl->rev_max = NFP_MAX(decl->rev_max, rev);
		}

		qsort(decl->revisions.entry, decl->revisions.count, sizeof(int), nsc_comp_int_asc);

		for (idx = 0; idx < decl->revisions.count; idx++) {
			int rev = decl->revisions.entry[idx];
			char revstr[NFP_MEID_STR_SZ];
			size_t i;

			if (!nfp_rev2str(revstr, rev)) {
				NS_LOG(NS_LOG_LVL_FATAL,
				       "Bad revision ('0x%x') for chip '%s' in '%s'",
				       rev, decl->id, source_file);
				goto error_cleanup;
			}

			for (i = 0; i < strlen(revstr); i++)
				revstr[i] = (char)tolower(revstr[i]);

			switch (nsc_j_obj_strnum_get(j_tmp2, revstr, &tmpu64)) {
			case 0:
				decl->revision_models.entry[idx] = (uint32_t)tmpu64;
				break;
			case -ENOENT:
				decl->revision_models.entry[idx] = NFP_CHIPDATA_CPP_MODEL_INVALID;
				break;
			default:
				NS_LOG(NS_LOG_LVL_FATAL,
				       "Invalid model IDs value for chip '%s', revision '%s' in '%s'",
				       decl->id, revstr, source_file);
				goto error_cleanup;
			}
		}
	}

	switch (decl->arch) {
	case NFP_CHIP_ARCH_YD:
		j_tmp = json_object_get(j_topitem, "cpp_targets");
		if (json_array_size(j_tmp) > 0) {
			size_t idx;

			_ARRAY_ALLOC(&decl->details.nfp3200.cpp_targets, json_array_size(j_tmp));

			for (idx = 0; idx < decl->details.nfp3200.cpp_targets.count; idx++)
				decl->details.nfp3200.cpp_targets.entry[idx] =
					(int)json_integer_value(json_array_get(j_tmp, idx));

			qsort(decl->details.nfp3200.cpp_targets.entry,
			      decl->details.nfp3200.cpp_targets.count, sizeof(int),
			      nsc_comp_int_asc);
		}

		switch (nsc_j_obj_strnum_get(j_topitem, "microengine_cluster_count", &tmpu64)) {
		case 0:
			if (tmpu64 == 0) {
				NS_LOG(NS_LOG_LVL_FATAL,
				       "Invalid microengine_cluster_count value for chip '%s' in '%s'",
				       decl->id, source_file);
				goto error_cleanup;
			}
			decl->details.nfp3200.me_cnt =
				(size_t)(tmpu64 * NSC_NFP3200_ME_PER_CLUSTER);
			break;
		case -ENOENT:
			decl->details.nfp3200.me_cnt = 0;
			break;
		default:
			NS_LOG(NS_LOG_LVL_FATAL,
			       "Invalid microengine_cluster_count value for chip '%s' in '%s'",
			       decl->id, source_file);
			goto error_cleanup;
		}
		break;
	case NFP_CHIP_ARCH_TH:
		switch (nsc_j_obj_strnum_get(j_topitem, "serdes_cnt", &tmpu64)) {
		case 0:
			decl->details.nfp6000.serdes_cnt = (size_t)(tmpu64);
			break;
		case -ENOENT:
			decl->details.nfp6000.serdes_cnt = SIZE_MAX;
			break;
		default:
			NS_LOG(NS_LOG_LVL_FATAL,
			       "Invalid serdes_cnt value for chip '%s' in '%s'",
			       decl->id, source_file);
			goto error_cleanup;
		}

		j_tmp = json_object_get(j_topitem, "islands");
		if (json_array_size(j_tmp) > 0) {
			size_t idx, idx2;

			_ARRAY_ALLOC(&decl->details.nfp6000.islands, json_array_size(j_tmp));

			for (idx = 0; idx < decl->details.nfp6000.islands.count; idx++) {
				int v = (int)json_integer_value(json_array_get(j_tmp, idx));

				for (idx2 = 0; idx2 < idx; idx2++) {
					if (decl->details.nfp6000.islands.entry[idx2] == v) {
						NS_LOG(NS_LOG_LVL_FATAL,
						       "Duplicate island ID for chip '%s' in '%s'",
						       decl->id, source_file);
						goto error_cleanup;
					}
				}
				decl->details.nfp6000.islands.entry[idx] = v;
			}

			qsort(decl->details.nfp6000.islands.entry,
			      decl->details.nfp6000.islands.count, sizeof(int), nsc_comp_int_asc);
		}

		cnt = 0;
		j_tmp = json_object_get(j_topitem, "imbcppats");
		json_object_foreach(j_tmp, iidstr, j_tmp2)
			cnt += json_object_size(j_tmp2);

		j_tmp2 = json_object_get(j_topitem, "nbimuats");
		cnt += json_object_size(j_tmp2);

		_ARRAY_ALLOC(&decl->cppats, cnt);
		cppat = decl->cppats.entry;

		{
			json_t *j_island_catt;
			json_t *j_cpp_catt;
			const char *cppstr;
			int iid, cpp_tgt;
			char *c;

			json_object_foreach(j_tmp, iidstr, j_island_catt) {
				iid = (int)nfp_strtou64(iidstr, &c, 0);
				if (*c != '\0') {
					NS_LOG(NS_LOG_LVL_FATAL,
					       "Invalid Island ID (imbcppats.%s) in '%s'",
					       iidstr, source_file);
					goto error_cleanup;
				}

				json_object_foreach(j_island_catt, cppstr, j_cpp_catt) {
					cpp_tgt = (int)nfp_strtou64(cppstr, &c, 0);
					if (*c != '\0') {
						NS_LOG(NS_LOG_LVL_FATAL,
						       "Invalid imbcppats.%d.cpp_tgt (%s) in '%s'",
						       iid, iidstr, source_file);
						goto error_cleanup;
					}

					cppat->nfp6000.from_iid = (uint8_t)iid;
					cppat->nfp6000.cpp_tgt = (uint8_t)cpp_tgt;

					switch (nsc_j_obj_strnum_get(j_cpp_catt, "addr40",
								     &tmpu64)) {
					case 0:
						if (tmpu64 <= 1) {
							cppat->nfp6000.addr40 = (uint8_t)tmpu64;
							break;
						}
					default:
						NS_LOG(NS_LOG_LVL_FATAL,
						       "Invalid imbcppat.%d.%d.addr40 value for chip '%s' in '%s'",
						       iid, cpp_tgt, decl->id, source_file);
						goto error_cleanup;
					}

					switch (nsc_j_obj_strnum_get(j_cpp_catt, "mode", &tmpu64)) {
					case 0:
						if (tmpu64 <= 0x7) {
							cppat->nfp6000.mode = (uint8_t)tmpu64;
							break;
						}
					// fall through for error message
					default:
						NS_LOG(NS_LOG_LVL_FATAL,
						       "Invalid imbcppat.%d.%d.mode value for chip '%s' in '%s'",
						       iid, cpp_tgt, decl->id, source_file);
						goto error_cleanup;
					}

					switch (nsc_j_obj_strnum_get(j_cpp_catt, "isld0",
								     &tmpu64)) {
					case 0:
						if (tmpu64 <= 0x3F) {
							cppat->nfp6000.isld0 = (uint8_t)tmpu64;
							break;
						}
					// fall through for error message
					default:
						NS_LOG(NS_LOG_LVL_FATAL,
						       "Invalid imbcppat.%d.%d.isld0 value for chip '%s' in '%s'",
						       iid, cpp_tgt, decl->id, source_file);
						goto error_cleanup;
					}

					switch (nsc_j_obj_strnum_get(j_cpp_catt, "isld1",
								     &tmpu64)) {
					case 0:
						if (tmpu64 <= 0x3F) {
							cppat->nfp6000.isld1 = (uint8_t)tmpu64;
							break;
						}
					// fall through for error message
					default:
						NS_LOG(NS_LOG_LVL_FATAL,
						       "Invalid imbcppat.%d.%d.isld1 value for chip '%s' in '%s'",
						       iid, cpp_tgt, decl->id, source_file);
						goto error_cleanup;
					}

					cppat++;
				}
			}
		}

		// Now NBI MU Xlate
		{
			json_t *j_catt;
			int iid;
			char *c;

			json_object_foreach(j_tmp2, iidstr, j_catt) {
				iid = (int)nfp_strtou64(iidstr, &c, 0);
				if (*c != '\0') {
					NS_LOG(NS_LOG_LVL_FATAL,
					       "Invalid Island ID (nbimuats.%s) in '%s'",
					       iidstr, source_file);
					goto error_cleanup;
				}

				cppat->nfp6000.addr40 = 1; // Only 40-bit from NBI
				cppat->nfp6000.from_iid = (uint8_t)iid;
				cppat->nfp6000.cpp_tgt = (uint8_t)NFP6000_CPPTGT_MU;

				switch (nsc_j_obj_strnum_get(j_catt, "mode", &tmpu64)) {
				case 0:
					if (tmpu64 <= 0x7) {
						cppat->nfp6000.mode = (uint8_t)tmpu64;
						break;
					}
				// fall through for error message
				default:
					NS_LOG(NS_LOG_LVL_FATAL,
					       "Invalid nbimuat.%d.mode value for chip '%s' in '%s'",
					       iid, decl->id, source_file);
					goto error_cleanup;
				}

				switch (nsc_j_obj_strnum_get(j_catt, "isld0", &tmpu64)) {
				case 0:
					if (tmpu64 <= 0x3F) {
						cppat->nfp6000.isld0 = (uint8_t)tmpu64;
						break;
					}
				// fall through for error message
				default:
					NS_LOG(NS_LOG_LVL_FATAL,
					       "Invalid nbimuat.%d.isld0 value for chip '%s' in '%s'",
					       iid, decl->id, source_file);
					goto error_cleanup;
				}

				switch (nsc_j_obj_strnum_get(j_catt, "isld1", &tmpu64)) {
				case 0:
					if (tmpu64 <= 0x3F) {
						cppat->nfp6000.isld1 = (uint8_t)tmpu64;
						break;
					}
				// fall through for error message
				default:
					NS_LOG(NS_LOG_LVL_FATAL,
					       "Invalid nbimuat.%d.isld1 value for chip '%s' in '%s'",
					       iid, decl->id, source_file);
					goto error_cleanup;
				}

				cppat++;
			}
		}
		break;
	default:
		NS_LOG(NS_LOG_LVL_FATAL, "Invalid family for chip '%s' in '%s'", decl->id,
		       source_file);
		goto error_cleanup;
	}

	j_tmp = json_object_get(j_topitem, "config");
	if (!!j_tmp)
		return nsc_json_gather_cfg(decl, j_tmp, NULL);

	return 0;

error_cleanup:
	return NFP_ERRNO(err);
}

int nsc_json_extract_chips(struct nfp_chipdata *data,
			   json_t *j_root,
			   const char *source_file,
			   int extract_boards)
{
	json_t *j_top, *j_topitem;
	const char *item_id;
	int ret;
	size_t i;

	if ((!data) || (!j_root) || (!source_file)) {
		NS_LOG(NS_LOG_LVL_FATAL, "Invalid parameters.");
		return NFP_ERRNO(EINVAL);
	}

	if (extract_boards) {
		json_t *j_arr;

		j_top = json_object_get(j_root, "boards");
		json_object_foreach(j_top, item_id, j_arr) {
			if (!strcmp(item_id, "_comment"))
				continue;
			for (i = 0; i < json_array_size(j_arr); i++) {
				j_topitem = json_array_get(j_arr, i);
				ret = nsc_json_extract_chip(data, item_id, j_topitem, i,
							    source_file);
				if (ret)
					return ret;
			}
		}
	} else {
		j_top = json_object_get(j_root, "chips");
		json_object_foreach(j_top, item_id, j_topitem) {
			if (!strcmp(item_id, "_comment"))
				continue;
			ret = nsc_json_extract_chip(data, item_id, j_topitem, -1, source_file);
			if (ret)
				return ret;
		}
	}

	return 0;
}

int nsc_json_extract_desc_nfp6000_top(struct nfp_chipdata *data,
				      json_t *j_root,
				      const char *source_file,
				      int subfamily)
{
	json_t *j_topitem;
	const char *item_id;
	const char *c = NULL;
	int err = EINVAL;
	size_t n;

	if ((!data) || (!j_root)) {
		NS_LOG(NS_LOG_LVL_FATAL, "Invalid parameters.");
		return NFP_ERRNO(EINVAL);
	}

	if (!!json_object_get(j_root, "cpp_targets"))
		return nsc_json_extract_desc_nfp6000(data, j_root, source_file, subfamily,
						     NFP_CHIP_REVISION_A0, NFP_CHIP_REVISION_PF);

	NSC_JOBJ_FOREACH(j_root, item_id, j_topitem, n)
	{
		int rev_min, rev_max;

		if (item_id[0] == '_')
			continue;
		c = item_id;
		rev_min = nfp_idstr2rev(c);
		if (rev_min < 0)
			continue;
		c += 2;
		if (*c != '-')
			continue;
		c++;
		rev_max = nfp_idstr2rev(c);
		if (rev_max < 0)
			continue;
		err = nsc_json_extract_desc_nfp6000(data, j_topitem, source_file, subfamily,
						    rev_min, rev_max);
		if (err)
			return err;
	}

	return 0;
}

int nsc_json_extract_desc_nfp6000(struct nfp_chipdata *data,
				  json_t *j_root,
				  const char *source_file,
				  int subfamily,
				  int rev_min,
				  int rev_max)
{
	json_t *j_top, *j_topitem;
	const char *item_id;
	json_t *j_tmp;
	char *c = NULL;
	struct nsc_chip_desc *gen_desc = NULL;
	struct nsc_chip_desc_nfp6000 *desc = NULL;
	uint64_t tmpu64;
	int has_cpp_mu = 0;
	size_t n;
	const char *log_src_str = NULL;
	int err = EINVAL;

	if ((!data) || (!j_root)) {
		NS_LOG(NS_LOG_LVL_FATAL, "Invalid parameters.");
		err = EINVAL;
		goto error_cleanup;
	}

	gen_desc = nsc_llist_add(&data->chip_descs, sizeof(*gen_desc));
	_OOM_CHECK(gen_desc);

	gen_desc->family = subfamily;
	gen_desc->arch = _NFP_ARCH(subfamily);
	gen_desc->rev_min = rev_min;
	gen_desc->rev_max = rev_max;

	desc = &gen_desc->desc.nfp6000;

	_STRDUP_OOM(gen_desc->file_path, source_file);
	if (!!source_file)
		log_src_str = source_file;
	else
		log_src_str =
			nsc_enum_to_string(nsc_family_string_map, gen_desc->family, "<unknown>");

	// If cpp_targets is missing or an empty object,
	// no array_alloc and no foreach is done.
	// desc->cpp_targets.count will be 0 in that case.
	// So no explicit error check needs to be done on the
	// existence of cpp_targets since it may be empty or
	// not present. This applies to most objects.
	j_top = json_object_get(j_root, "cpp_targets");
	_ARRAY_ALLOC(&desc->cpp_targets, json_object_size(j_top));
	NSC_JOBJ_FOREACH(j_top, item_id, j_topitem, n)
	{
		struct nsc_desc_cpptgt_nfpxxxx *cpptgt;

		cpptgt = &desc->cpp_targets.entry[n];

		cpptgt->tgt_id = (int)nfp_strtou64(item_id, &c, 0);
		if (*c != '\0') {
			NS_LOG(NS_LOG_LVL_FATAL, "Invalid CPP Target ID (%s) in '%s'", item_id,
			       log_src_str);
			goto error_cleanup;
		}

		_STRTAB_ADD_OOM(cpptgt->name, &data->strtab,
				json_string_value(json_object_get((j_topitem), ("name"))));
		_STRTAB_ADD_OOM(cpptgt->name_long, &data->strtab,
				json_string_value(json_object_get((j_topitem), ("name_long"))));

		if (cpptgt->name == 0) {
			NS_LOG(NS_LOG_LVL_FATAL, "No name for CPP Target ID (%s) in '%s'", item_id,
			       log_src_str);
			goto error_cleanup;
		}
	}

	j_top = json_object_get(j_root, "memories");
	_ARRAY_ALLOC(&desc->memories, json_object_size(j_top));
	NSC_JOBJ_FOREACH(j_top, item_id, j_topitem, n)
	{
		struct _nfp_chipdata_memory *mem;

		mem = &desc->memories.entry[n];

		mem->type = (enum nfp_chipdata_mem_type)nsc_enum_from_string(
			nsc_nfp6000_mem_type_string_map, item_id);

		if (mem->type == NFP_CHIPDATA_MEM_TYPE_NONE) {
			NS_LOG(NS_LOG_LVL_ERROR,
			       "Invalid memory name (%s) in '%s', ignoring.",
			       item_id, log_src_str);
			continue;
		}

		_STRTAB_ADD_OOM(mem->name, &data->strtab, item_id);

		switch (nsc_j_obj_strnum_get(j_topitem, "size_max", &tmpu64)) {
		case 0:
			mem->size_max = tmpu64;
			break;
		case -ENOENT:
			mem->size_max = UINT64_MAX;
			break;
		default:
			NS_LOG(NS_LOG_LVL_FATAL,
			       "Invalid size_max value for memory %s in '%s'",
			       item_id, log_src_str);
			goto error_cleanup;
		}

		switch (nsc_j_obj_strnum_get(j_topitem, "base", &tmpu64)) {
		case 0:
			mem->base = tmpu64;
			break;
		case -ENOENT:
			mem->base = 0;
			break;
		default:
			NS_LOG(NS_LOG_LVL_FATAL,
			       "Invalid base value for memory %s in '%s'",
			       item_id, log_src_str);
			goto error_cleanup;
		}

		switch (nsc_j_obj_strnum_get(j_topitem, "alloc_base", &tmpu64)) {
		case 0:
			mem->alloc_base = tmpu64;
			break;
		case -ENOENT:
			mem->alloc_base = 0;
			break;
		default:
			NS_LOG(NS_LOG_LVL_FATAL,
			       "Invalid base value for memory %s in '%s'",
			       item_id, log_src_str);
			goto error_cleanup;
		}

		switch (nsc_j_obj_strnum_get(j_topitem, "cpp_id", &tmpu64)) {
		case 0:
			mem->cpp_tgt = (int)tmpu64;
			break;
		case -ENOENT:
			mem->cpp_tgt = -1;
			break;
		default:
			NS_LOG(NS_LOG_LVL_FATAL,
			       "Invalid cpp_id value for memory %s in '%s'",
			       item_id, log_src_str);
			goto error_cleanup;
		}
	}

	j_top = json_object_get(j_root, "islands");
	_ARRAY_ALLOC(&desc->islands, json_object_size(j_top));
	NSC_JOBJ_FOREACH(j_top, item_id, j_topitem, n)
	{
		struct nsc_desc_island_nfp6000 *island;

		island = &desc->islands.entry[n];

		island->id = (int)nfp_strtou64(item_id, &c, 0);
		if (*c != '\0') {
			NS_LOG(NS_LOG_LVL_FATAL, "Invalid Island ID (%s) in '%s'", item_id,
			       log_src_str);
			goto error_cleanup;
		}

		_STRTAB_ADD_OOM(island->name, &data->strtab,
				json_string_value(json_object_get((j_topitem), ("name"))));
		if (island->name == 0) {
			NS_LOG(NS_LOG_LVL_FATAL, "No name for Island ID %d in '%s'", island->id,
			       log_src_str);
			goto error_cleanup;
		}

		has_cpp_mu = 0;
		j_tmp = json_object_get(j_topitem, "cpp_targets");
		if (json_array_size(j_tmp) > 0) {
			size_t idx1, idx2;

			island->cpp_tgt_id_count = json_array_size(j_tmp);
			_CALLOC_OOM(island->cpp_tgt_ids, island->cpp_tgt_id_count, sizeof(int));
			for (idx1 = 0; idx1 < island->cpp_tgt_id_count; idx1++) {
				int cpptgtid = (int)json_integer_value(json_array_get(j_tmp, idx1));

				island->cpp_tgt_ids[idx1] = -1;
				for (idx2 = 0; idx2 < desc->cpp_targets.count; idx2++) {
					struct nsc_desc_cpptgt_nfpxxxx *cpptgt =
						&desc->cpp_targets.entry[idx2];
					if (cpptgt->tgt_id == cpptgtid) {
						island->cpp_tgt_ids[idx1] = cpptgtid;
						break;
					}
				}

				if (island->cpp_tgt_ids[idx1] == -1) {
					NS_LOG(NS_LOG_LVL_FATAL,
					       "Invalid CPP target %d for Island ID %d in '%s'",
					       cpptgtid, island->id, log_src_str);
					goto error_cleanup;
				}

				has_cpp_mu |= (cpptgtid == NFP6000_CPPTGT_MU);
			}
		}

		j_tmp = json_object_get(j_topitem, "memories");
		if (json_array_size(j_tmp) > 0) {
			size_t idx1;
			enum nfp_chipdata_mem_type memtype;

			island->mem_types_count = json_array_size(j_tmp);
			_CALLOC_OOM(island->mem_types, island->mem_types_count,
				    sizeof(*island->mem_types));
			_CALLOC_OOM(island->mem_cfgs, island->mem_types_count,
				    sizeof(*island->mem_cfgs));
			for (idx1 = 0; idx1 < island->mem_types_count; idx1++) {
				const char *s = json_string_value(json_array_get(j_tmp, idx1));

				memtype = (enum nfp_chipdata_mem_type)nsc_enum_from_string(
					nsc_nfp6000_mem_type_string_map, s);
				if (!s)
					s = "<null>";
				if (memtype == NFP_CHIPDATA_MEM_TYPE_NONE) {
					NS_LOG(NS_LOG_LVL_FATAL,
					       "Invalid memory name '%s' for Island ID %d in '%s'",
					       s, island->id, log_src_str);
					goto error_cleanup;
				}

				island->mem_types[idx1] = memtype;
				island->mem_cfgs[idx1].base = 0;
				island->mem_cfgs[idx1].alloc_base = 0;
				island->mem_cfgs[idx1].size = UINT64_MAX;
			}
		}

		island->island_type = (enum nfp_chipdata_isld_type)nsc_enum_from_string(
			nsc_nfp6000_island_string_map,
			json_string_value(json_object_get(j_topitem, "type")));

		switch (nsc_j_obj_strnum_get(j_topitem, "microengines", &tmpu64)) {
		case 0:
			island->me_cnt = (int)tmpu64;
			break;
		case -ENOENT:
			break;
		default:
			NS_LOG(NS_LOG_LVL_FATAL,
			       "Invalid microengines value for Island ID %d in '%s'",
			       island->id, log_src_str);
			goto error_cleanup;
		}

		island->mu_type = (enum nfp_chipdata_mu_type)nsc_enum_from_string(
			nsc_nfp6000_mu_type_string_map,
			json_string_value(json_object_get(j_topitem, "mu_type")));

		if ((island->mu_type == NFP_CHIPDATA_MU_TYPE_NONE) && (has_cpp_mu)) {
			NS_LOG(NS_LOG_LVL_FATAL,
			       "Invalid mu_type value for Island ID %d in '%s'",
			       island->id, log_src_str);
			goto error_cleanup;
		} else if ((island->mu_type != NFP_CHIPDATA_MU_TYPE_NONE) && (!has_cpp_mu)) {
			NS_LOG(NS_LOG_LVL_FATAL,
			       "Island ID %d has CPP target ID 7 and no mu_type in '%s'",
			       island->id, log_src_str);
			goto error_cleanup;
		}
	}

	// These depend on the above, so never move it to before the
	// basic island info parsing.
	j_top = json_object_get(j_root, "alloc_types");

	// First iterate and check for expanding entries and also
	// do all the primary error checks so that the second loop
	// only needs to do processing.
	// Currently supported:
	// * aaa{iid}ee : {iid} is replace with the island IDs of
	// islands that contain the memory type
	_ARRAY_ALLOC(&desc->alloc_types, 128);
	desc->alloc_types.count = 0;

	json_object_foreach(j_top, item_id, j_topitem) {
		struct _nfp_chipdata_alloc_type *alloc_type;
		struct _nfp_chipdata_alloc *entry;
		int expand_flags = 0;
		enum nfp_chipdata_mem_type memtype;
		const char *memstr;
		size_t idx_ent, idx_iid, idx_mem;
		json_t *j_ent;

		if (json_array_size(j_topitem) == 0) {
			NS_LOG(NS_LOG_LVL_FATAL, "No allocation specs for type '%s'", item_id);
			goto error_cleanup;
		}

		if (!!strstr(item_id, NSC6000_EXPAND_SIID))
			expand_flags |= NSC6000_EXPAND_FLAG_SIID;
		if (!!strstr(item_id, NSC6000_EXPAND_NIID))
			expand_flags |= NSC6000_EXPAND_FLAG_NIID;

		if (expand_flags) {
			for (idx_ent = 0; idx_ent < json_array_size(j_topitem); idx_ent++) {
				int iid = -1;

				j_ent = json_array_get(j_topitem, idx_ent);

				memstr = json_string_value(json_object_get(j_ent, "memory"));
				memtype = (enum nfp_chipdata_mem_type)nsc_enum_from_string(
					nsc_nfp6000_mem_type_string_map, memstr);
				if (memtype == NFP_CHIPDATA_MEM_TYPE_NONE) {
					NS_LOG(NS_LOG_LVL_FATAL,
					       "Invalid memory name (%s) for allocation type '%s'",
					       memstr, item_id);
					goto error_cleanup;
				}

				j_tmp = json_object_get(j_ent, "targets");
				if (!!j_tmp) {
					NS_LOG(NS_LOG_LVL_WARN,
					       "'targets' field ignored for allocation type '%s'",
					       item_id);
				}

				for (idx_iid = 0; idx_iid < desc->islands.count; idx_iid++) {
					enum nfp_chipdata_mem_type mt = memtype;

					/* For any alloc_type that uses EMEM, we match any island
					 * that is an External MU island. This is to allow us to
					 * later "fake" a 3MiB EMEM resource in the
					 * linker if the user allows it.
					 */
					if (mt == NFP_CHIPDATA_MEM_TYPE_EMEM)
						mt = NFP_CHIPDATA_MEM_TYPE_EMEM_CACHE;

					for (idx_mem = 0;
					     idx_mem < desc->islands.entry[idx_iid].mem_types_count;
					     idx_mem++) {
						if (desc->islands.entry[idx_iid]
							    .mem_types[idx_mem] == mt)
							break;
					}

					if (idx_mem == desc->islands.entry[idx_iid].mem_types_count)
						continue;

					iid = desc->islands.entry[idx_iid].id;

					if (desc->alloc_types.count == desc->alloc_types.capacity)
						_ARRAY_EXTEND(&desc->alloc_types, 128);
					alloc_type =
						&desc->alloc_types.entry[desc->alloc_types.count++];

					alloc_type->entries_count = 1;
					_CALLOC_OOM(alloc_type->entries, 1,
						    sizeof(*alloc_type->entries));
					entry = alloc_type->entries;
					entry->type = memtype;

					j_tmp = json_object_get(j_ent, "islands");
					entry->islands_count = json_array_size(j_tmp);
					_CALLOC_OOM(entry->islands, entry->islands_count,
						    sizeof(*entry->islands));
					for (n = 0; n < entry->islands_count; n++) {
						entry->islands[n] = (int)json_integer_value(
							json_array_get(j_tmp, n));
					}

					entry->targets_count = 1;
					_CALLOC_OOM(entry->targets, entry->targets_count,
						    sizeof(*entry->targets));
					entry->targets[0] = iid;

					entry->policy = NFP_CHIPDATA_ALLOC_POLICY_DEFAULT;
					j_tmp = json_object_get(j_ent, "policy");
					if (j_tmp) {
						const char *tmpstr = json_string_value(j_tmp);

						entry->policy = (enum nfp_chipdata_alloc_policy)
							nsc_enum_from_string(
								nsc_alloc_type_policy_string_map,
								tmpstr);

						if (entry->policy == NFP_CHIPDATA_ALLOC_POLICY_INVALID) {
							NS_LOG(NS_LOG_LVL_FATAL,
							       "Invalid allocation policy (%s) for allocation type '%s'",
							       tmpstr, NSC_STR(alloc_type->name));
							goto error_cleanup;
						}
					}

					switch (nsc_j_obj_strnum_get(j_ent, "size_max", &tmpu64)) {
					case 0:
						entry->size_max = tmpu64;
						break;
					case -ENOENT:
						entry->size_max = UINT64_MAX;
						break;
					default:
						NS_LOG(NS_LOG_LVL_FATAL,
						       "Invalid size_max value for alloc_type %s in '%s'",
						       item_id, log_src_str);
						goto error_cleanup;
					}

					switch (nsc_j_obj_strnum_get(j_ent, "base", &tmpu64)) {
					case 0:
						entry->base = tmpu64;
						break;
					case -ENOENT:
						entry->base = 0;
						break;
					default:
						NS_LOG(NS_LOG_LVL_FATAL,
						       "Invalid base value for alloc_type %s in '%s'",
						       item_id, log_src_str);
						goto error_cleanup;
					}

					c = nsc6000_expand_fmtstr(item_id, iid, 0);
					if (!c) {
						err = errno;
						goto error_cleanup;
					}

					_STRTAB_ADD_OOM(alloc_type->name, &data->strtab, c);
					free(c);

					/* If we have a string iid format,
					 * we duplicate the entry
					 * for an island alias.
					 */
					if (expand_flags & NSC6000_EXPAND_FLAG_SIID) {
						size_t n2;
						struct _nfp_chipdata_alloc_type *prev_alloc_type =
							NULL;

						if (desc->alloc_types.count ==
						    desc->alloc_types.capacity)
							_ARRAY_EXTEND(&desc->alloc_types, 128);

						prev_alloc_type =
							&desc->alloc_types
								 .entry[desc->alloc_types.count - 1];
						alloc_type =
							&desc->alloc_types
								 .entry[desc->alloc_types.count++];
						alloc_type->entries_count =
							prev_alloc_type->entries_count;
						_CALLOC_OOM(alloc_type->entries,
							    alloc_type->entries_count,
							    sizeof(*alloc_type->entries));

						for (n2 = 0; n2 < alloc_type->entries_count; n2++) {
							entry = &alloc_type->entries[n2];
							entry->type =
								prev_alloc_type->entries[n2].type;
							entry->policy =
								prev_alloc_type->entries[n2].policy;
							entry->base =
								prev_alloc_type->entries[n2].base;
							entry->size_max =
								prev_alloc_type->entries[n2]
									.size_max;

							entry->islands_count =
								prev_alloc_type->entries[n2]
									.islands_count;
							_CALLOC_OOM(entry->islands,
								    entry->islands_count,
								    sizeof(*entry->islands));
							memcpy(entry->islands,
							       prev_alloc_type->entries[n2].islands,
							       (sizeof(entry->islands[0]) *
								entry->islands_count));

							entry->targets_count = 1;
							_CALLOC_OOM(entry->targets,
								    entry->targets_count,
								    sizeof(*entry->targets));
							entry->targets[0] = iid;
						}

						c = nsc6000_expand_fmtstr(item_id, iid, 1);
						if (!c) {
							err = errno;
							goto error_cleanup;
						}
						_STRTAB_ADD_OOM(alloc_type->name, &data->strtab, c);
						free(c);
					}
				}

				if (iid == -1) {
					NS_LOG(NS_LOG_LVL_FATAL,
					       "No expansion for allocation type '%s' - no islands with memory type %s",
					       item_id, memstr);
					goto error_cleanup;
				}
			}
		} else {
			int la = 1;

			if (desc->alloc_types.count == desc->alloc_types.capacity)
				_ARRAY_EXTEND(&desc->alloc_types, 128);
			alloc_type = &desc->alloc_types.entry[desc->alloc_types.count++];

			_STRTAB_ADD_OOM(alloc_type->name, &data->strtab, item_id);

			alloc_type->entries_count = json_array_size(j_topitem);
			_CALLOC_OOM(alloc_type->entries, alloc_type->entries_count,
				    sizeof(*alloc_type->entries));

			for (idx_ent = 0; idx_ent < alloc_type->entries_count; idx_ent++) {
				entry = &alloc_type->entries[idx_ent];
				j_ent = json_array_get(j_topitem, idx_ent);
				memstr = json_string_value(json_object_get(j_ent, "memory"));

				memtype = (enum nfp_chipdata_mem_type)nsc_enum_from_string(
					nsc_nfp6000_mem_type_string_map, memstr);

				entry->type = NFP_CHIPDATA_MEM_TYPE_NONE;
				for (idx_mem = 0; idx_mem < desc->memories.count; idx_mem++) {
					struct _nfp_chipdata_memory *mem =
						&desc->memories.entry[idx_mem];
					if (memtype == mem->type) {
						entry->type = memtype;
						break;
					}
				}
				if (entry->type == NFP_CHIPDATA_MEM_TYPE_NONE) {
					NS_LOG(NS_LOG_LVL_FATAL,
					       "Memory type (%s) does not exist for allocation type '%s'",
					       memstr, NSC_STR(alloc_type->name));
					goto error_cleanup;
				}

				j_tmp = json_object_get(j_ent, "islands");
				entry->islands_count = json_array_size(j_tmp);
				_CALLOC_OOM(entry->islands, entry->islands_count,
					    sizeof(*entry->islands));
				for (idx_iid = 0; idx_iid < entry->islands_count; idx_iid++) {
					entry->islands[idx_iid] = (int)json_integer_value(
						json_array_get(j_tmp, idx_iid));
				}

				j_tmp = json_object_get(j_ent, "targets");
				entry->targets_count = json_array_size(j_tmp);
				_CALLOC_OOM(entry->targets, entry->targets_count,
					    sizeof(*entry->targets));
				for (idx_iid = 0; idx_iid < entry->targets_count; idx_iid++) {
					entry->targets[idx_iid] = (int)json_integer_value(
						json_array_get(j_tmp, idx_iid));
				}

				entry->policy = NFP_CHIPDATA_ALLOC_POLICY_DEFAULT;
				j_tmp = json_object_get(j_ent, "policy");
				if (j_tmp) {
					const char *tmpstr = json_string_value(j_tmp);

					entry->policy = (enum nfp_chipdata_alloc_policy)
						nsc_enum_from_string(
							nsc_alloc_type_policy_string_map, tmpstr);

					if (entry->policy == NFP_CHIPDATA_ALLOC_POLICY_INVALID) {
						NS_LOG(NS_LOG_LVL_FATAL,
						       "Invalid allocation policy (%s) for allocation type '%s'",
						       tmpstr, NSC_STR(alloc_type->name));
						goto error_cleanup;
					}
				}

				switch (nsc_j_obj_strnum_get(j_ent, "size_max", &tmpu64)) {
				case 0:
					entry->size_max = tmpu64;
					break;
				case -ENOENT:
					entry->size_max = UINT64_MAX;
					break;
				default:
					NS_LOG(NS_LOG_LVL_FATAL,
					       "Invalid size_max value for alloc_type %s in '%s'",
					       item_id, log_src_str);
					goto error_cleanup;
				}

				switch (nsc_j_obj_strnum_get(j_ent, "base", &tmpu64)) {
				case 0:
					entry->base = tmpu64;
					break;
				case -ENOENT:
					entry->base = 0;
					break;
				default:
					NS_LOG(NS_LOG_LVL_FATAL,
					       "Invalid base value for alloc_type %s in '%s'",
					       item_id, log_src_str);
					goto error_cleanup;
				}

				la = la && (entry->islands_count == 1) &&
				     (entry->targets_count == 1) &&
				     (entry->islands[0] == entry->targets[0]);
			}

			alloc_type->local_alloc = la;
			if (la) {
				size_t n2;
				struct _nfp_chipdata_alloc_type *prev_alloc_type = NULL;

				if (desc->alloc_types.count == desc->alloc_types.capacity)
					_ARRAY_EXTEND(&desc->alloc_types, 128);

				prev_alloc_type =
					&desc->alloc_types.entry[desc->alloc_types.count - 1];
				alloc_type = &desc->alloc_types.entry[desc->alloc_types.count++];
				alloc_type->entries_count = prev_alloc_type->entries_count;
				_CALLOC_OOM(alloc_type->entries, alloc_type->entries_count,
					    sizeof(*alloc_type->entries));

				for (n2 = 0; n2 < alloc_type->entries_count; n2++) {
					entry = &alloc_type->entries[n2];
					entry->type = prev_alloc_type->entries[n2].type;
					entry->policy = prev_alloc_type->entries[n2].policy;
					entry->base = prev_alloc_type->entries[n2].base;
					entry->size_max = prev_alloc_type->entries[n2].size_max;

					entry->islands_count = 1;
					_CALLOC_OOM(entry->islands, entry->islands_count,
						    sizeof(*entry->islands));
					entry->islands[0] = prev_alloc_type->entries[n2].islands[0];

					entry->targets_count = 1;
					_CALLOC_OOM(entry->targets, entry->targets_count,
						    sizeof(*entry->targets));
					entry->targets[0] = prev_alloc_type->entries[n2].targets[0];
				}

				_MALLOC_OOM(
					c, NFP_MAX(NFP_MEID_STR_SZ,
						   strlen(NSC_STR(prev_alloc_type->name)) + 1 + 3));
				(void)nfp6000_island2str(c, 0);
				strcat(c, ".");
				strcat(c, NSC_STR(prev_alloc_type->name));
				alloc_type->name = nsc_strtab_add(&data->strtab, c);
				free(c);
				if (alloc_type->name == SIZE_MAX) {
					err = ENOMEM;
					goto error_cleanup;
				}
				/* "i0.ctm/cls" should expand to a full address */
				/* "ctm/cls" should first try to expand as just 0 */
				alloc_type->local_alloc = 0;
			}
		}
	}

	j_top = json_object_get(j_root, "alloc_aliases");
	_ARRAY_ALLOC(&desc->alloc_aliases, json_object_size(j_top));
	NSC_JOBJ_FOREACH(j_top, item_id, j_topitem, n)
	{
		struct nsc_chipdata_alloc_alias *alloc_alias;
		size_t idx;

		alloc_alias = &desc->alloc_aliases.entry[n];
		_STRTAB_ADD_OOM(alloc_alias->alias, &data->strtab, item_id);

		if (!json_string_value(j_topitem)) {
			NS_LOG(NS_LOG_LVL_FATAL,
			       "No allocation type specified for allocation alias '%s'",
			       alloc_alias->alias);
			goto error_cleanup;
		}

		_STRTAB_ADD_OOM(alloc_alias->name, &data->strtab, json_string_value(j_topitem));

		for (idx = 0; idx < desc->alloc_types.count; idx++) {
			struct _nfp_chipdata_alloc_type *atype = &desc->alloc_types.entry[idx];

			if (strcmp(NSC_STR(atype->name), NSC_STR(alloc_alias->name)) == 0)
				break;
		}
		if (idx >= desc->alloc_types.count) {
			NS_LOG(NS_LOG_LVL_FATAL,
			       "Invalid allocation type '%s' specified for allocation alias '%s'",
			       NSC_STR(alloc_alias->name), NSC_STR(alloc_alias->alias));
			goto error_cleanup;
		}
	}

	return 0;

error_cleanup:
	return NFP_ERRNO(err);
}

int nsc_json_extract_desc_nfp3200_top(struct nfp_chipdata *data,
				      json_t *j_root,
				      const char *source_file)
{
	json_t *j_topitem;
	const char *item_id;
	const char *c = NULL;
	int err = EINVAL;
	size_t n;

	if ((!data) || (!j_root)) {
		NS_LOG(NS_LOG_LVL_FATAL, "Invalid parameters.");
		return NFP_ERRNO(EINVAL);
	}

	if (!!json_object_get(j_root, "cpp_targets"))
		return nsc_json_extract_desc_nfp3200(data, j_root, source_file,
						     NFP_CHIP_REVISION_A0, NFP_CHIP_REVISION_PF);

	NSC_JOBJ_FOREACH(j_root, item_id, j_topitem, n)
	{
		int rev_min, rev_max;

		if (item_id[0] == '_')
			continue;
		c = item_id;
		rev_min = nfp_idstr2rev(c);
		if (rev_min < 0)
			continue;
		c += 2;
		if (*c != '-')
			continue;
		c++;
		rev_max = nfp_idstr2rev(c);
		if (rev_max < 0)
			continue;
		err = nsc_json_extract_desc_nfp3200(data, j_topitem, source_file, rev_min, rev_max);
		if (err)
			return err;
	}

	return 0;
}

int nsc_json_extract_desc_nfp3200(struct nfp_chipdata *data,
				  json_t *j_root,
				  const char *source_file,
				  int rev_min,
				  int rev_max)
{
	json_t *j_top, *j_topitem, *j_tmp;
	const char *item_id;
	char *c = NULL;
	struct nsc_chip_desc *gen_desc = NULL;
	struct nsc_chip_desc_nfp3200 *desc = NULL;
	uint64_t tmpu64;
	size_t n;
	const char *log_src_str = NULL;
	int err = EINVAL;

	if ((!data) || (!j_root)) {
		NS_LOG(NS_LOG_LVL_FATAL, "Invalid parameters.");
		err = EINVAL;
		goto error_cleanup;
	}

	gen_desc = nsc_llist_add(&data->chip_descs, sizeof(*gen_desc));
	_OOM_CHECK(gen_desc);

	gen_desc->family = NFP_CHIP_FAMILY_NFP3200;
	gen_desc->arch = _NFP_ARCH(NFP_CHIP_FAMILY_NFP3200);
	desc = &gen_desc->desc.nfp3200;

	gen_desc->rev_min = rev_min;
	gen_desc->rev_max = rev_max;

	_STRDUP_OOM(gen_desc->file_path, source_file);
	if (!!source_file)
		log_src_str = source_file;
	else
		log_src_str =
			nsc_enum_to_string(nsc_family_string_map, gen_desc->family, "<unknown>");

	switch (nsc_j_obj_strnum_get(j_root, "microengines_per_clusters", &tmpu64)) {
	case 0:
		break;
	case -ENOENT:
		tmpu64 = NSC_NFP3200_ME_PER_CLUSTER;
		break;
	default:
		tmpu64 = 0;
		break;
	}

	if (tmpu64 != NSC_NFP3200_ME_PER_CLUSTER) {
		NS_LOG(NS_LOG_LVL_FATAL,
		       "Invalid microengines_per_clusters in '%s'",
		       log_src_str);
		goto error_cleanup;
	}

	j_top = json_object_get(j_root, "cpp_targets");
	_ARRAY_ALLOC(&desc->cpp_targets, json_object_size(j_top));
	NSC_JOBJ_FOREACH(j_top, item_id, j_topitem, n)
	{
		struct nsc_desc_cpptgt_nfpxxxx *cpptgt;

		cpptgt = &desc->cpp_targets.entry[n];

		cpptgt->tgt_id = (int)nfp_strtou64(item_id, &c, 0);
		if (*c != '\0') {
			NS_LOG(NS_LOG_LVL_FATAL,
			       "Invalid CPP Target ID (%s) in '%s'", item_id,
			       log_src_str);
			goto error_cleanup;
		}

		_STRTAB_ADD_OOM(cpptgt->name, &data->strtab,
				json_string_value(json_object_get(j_topitem, "name")));
		_STRTAB_ADD_OOM(cpptgt->name_long, &data->strtab,
				json_string_value(json_object_get(j_topitem, "name_long")));

		if (cpptgt->name == 0) {
			NS_LOG(NS_LOG_LVL_FATAL,
			       "No name for CPP Target ID (%s) in '%s'", item_id,
			       log_src_str);
			goto error_cleanup;
		}
	}

	j_top = json_object_get(j_root, "memories");
	_ARRAY_ALLOC(&desc->memories, json_object_size(j_top));
	NSC_JOBJ_FOREACH(j_top, item_id, j_topitem, n)
	{
		struct _nfp_chipdata_memory *mem;

		mem = &desc->memories.entry[n];

		mem->type = (enum nfp_chipdata_mem_type)nsc_enum_from_string(
			nsc_nfp3200_mem_type_string_map, item_id);

		if (mem->type == NFP_CHIPDATA_MEM_TYPE_NONE) {
			NS_LOG(NS_LOG_LVL_FATAL, "Invalid memory name (%s) in '%s'", item_id,
			       log_src_str);
			goto error_cleanup;
		}

		_STRTAB_ADD_OOM(mem->name, &data->strtab, item_id);

		switch (nsc_j_obj_strnum_get(j_topitem, "size_max", &tmpu64)) {
		case 0:
			mem->size_max = tmpu64;
			break;
		case -ENOENT:
			mem->size_max = UINT64_MAX;
			break;
		default:
			NS_LOG(NS_LOG_LVL_FATAL,
			       "Invalid size_max value for memory %s in '%s'",
			       item_id, log_src_str);
			goto error_cleanup;
		}

		switch (nsc_j_obj_strnum_get(j_topitem, "base", &tmpu64)) {
		case 0:
			mem->base = tmpu64;
			break;
		case -ENOENT:
			mem->base = 0;
			break;
		default:
			NS_LOG(NS_LOG_LVL_FATAL,
			       "Invalid base value for memory %s in '%s'",
			       item_id, log_src_str);
			goto error_cleanup;
		}

		switch (nsc_j_obj_strnum_get(j_topitem, "alloc_base", &tmpu64)) {
		case 0:
			mem->alloc_base = tmpu64;
			break;
		case -ENOENT:
			mem->alloc_base = 0;
			break;
		default:
			NS_LOG(NS_LOG_LVL_FATAL,
			       "Invalid base value for memory %s in '%s'",
			       item_id, log_src_str);
			goto error_cleanup;
		}

		switch (nsc_j_obj_strnum_get(j_topitem, "cpp_id", &tmpu64)) {
		case 0:
			mem->cpp_tgt = (int)tmpu64;
			break;
		case -ENOENT:
			mem->cpp_tgt = -1;
			break;
		default:
			NS_LOG(NS_LOG_LVL_FATAL,
			       "Invalid cpp_id value for memory %s in '%s'",
			       item_id, log_src_str);
			goto error_cleanup;
		}
	}

	/* This can only have one value and therefore should not really even
	 * need to be in the JSON file, but it makes reading the file provide
	 * a more complete view. We then just need to at least verify that
	 * it is valid.
	 */
	switch (nsc_j_obj_strnum_get(j_root, "cluster_count", &tmpu64)) {
	case 0:
		break;
	case -ENOENT:
		tmpu64 = NSC_NFP3200_CLUSTER_CNT_MAX;
		break;
	default:
		tmpu64 = 0;
		break;
	}

	if (tmpu64 != NSC_NFP3200_CLUSTER_CNT_MAX) {
		NS_LOG(NS_LOG_LVL_FATAL, "Invalid cluster_count in '%s'", log_src_str);
		goto error_cleanup;
	}
	desc->cluster_count = (int)tmpu64;

	j_top = json_object_get(j_root, "alloc_types");
	_ARRAY_ALLOC(&desc->alloc_types, json_object_size(j_top));
	NSC_JOBJ_FOREACH(j_top, item_id, j_topitem, n)
	{
		struct _nfp_chipdata_alloc_type *alloc_type;
		struct _nfp_chipdata_alloc *entry;
		enum nfp_chipdata_mem_type memtype;
		const char *tmpstr;
		size_t idx1, idx2;
		json_t *j_ent;
		int la = 1;

		alloc_type = &desc->alloc_types.entry[n];
		_STRTAB_ADD_OOM(alloc_type->name, &data->strtab, item_id);

		if (json_array_size(j_topitem) == 0) {
			NS_LOG(NS_LOG_LVL_FATAL, "No allocation specs for type '%s'",
			       NSC_STR(alloc_type->name));
			goto error_cleanup;
		}

		alloc_type->entries_count = json_array_size(j_topitem);
		_CALLOC_OOM(alloc_type->entries, alloc_type->entries_count,
			    sizeof(*alloc_type->entries));

		for (idx1 = 0; idx1 < alloc_type->entries_count; idx1++) {
			entry = &alloc_type->entries[idx1];
			j_ent = json_array_get(j_topitem, idx1);
			tmpstr = json_string_value(json_object_get(j_ent, "memory"));

			memtype = (enum nfp_chipdata_mem_type)nsc_enum_from_string(
				nsc_nfp3200_mem_type_string_map, tmpstr);
			if (memtype == NFP_CHIPDATA_MEM_TYPE_NONE) {
				NS_LOG(NS_LOG_LVL_FATAL,
				       "Invalid memory name (%s) for allocation type '%s'", tmpstr,
				       NSC_STR(alloc_type->name));
				goto error_cleanup;
			}

			entry->type = NFP_CHIPDATA_MEM_TYPE_NONE;
			for (idx2 = 0; idx2 < desc->memories.count; idx2++) {
				struct _nfp_chipdata_memory *mem = &desc->memories.entry[idx2];

				if (memtype == mem->type) {
					entry->type = memtype;
					break;
				}
			}
			if (entry->type == NFP_CHIPDATA_MEM_TYPE_NONE) {
				NS_LOG(NS_LOG_LVL_FATAL,
				       "Memory type (%s) does not exist for allocation type '%s'",
				       tmpstr, NSC_STR(alloc_type->name));
				goto error_cleanup;
			}

			j_tmp = json_object_get(j_ent, "islands");
			entry->islands_count = json_array_size(j_tmp);
			_CALLOC_OOM(entry->islands, entry->islands_count, sizeof(*entry->islands));
			for (idx2 = 0; idx2 < entry->islands_count; idx2++) {
				entry->islands[idx2] =
					(int)json_integer_value(json_array_get(j_tmp, idx2));
			}

			j_tmp = json_object_get(j_ent, "targets");
			entry->targets_count = json_array_size(j_tmp);
			_CALLOC_OOM(entry->targets, entry->targets_count, sizeof(*entry->targets));
			for (idx2 = 0; idx2 < entry->targets_count; idx2++) {
				entry->targets[idx2] =
					(int)json_integer_value(json_array_get(j_tmp, idx2));
			}

			entry->policy = NFP_CHIPDATA_ALLOC_POLICY_DEFAULT;
			j_tmp = json_object_get(j_ent, "policy");
			if (j_tmp) {
				tmpstr = json_string_value(j_tmp);
				entry->policy =
					(enum nfp_chipdata_alloc_policy)nsc_enum_from_string(
						nsc_alloc_type_policy_string_map, tmpstr);
				if (entry->policy == NFP_CHIPDATA_ALLOC_POLICY_INVALID) {
					NS_LOG(NS_LOG_LVL_FATAL,
					       "Invalid allocation policy (%s) for allocation type '%s'",
					       tmpstr, NSC_STR(alloc_type->name));
					goto error_cleanup;
				}
			}

			switch (nsc_j_obj_strnum_get(j_ent, "size_max", &tmpu64)) {
			case 0:
				entry->size_max = tmpu64;
				break;
			case -ENOENT:
				entry->size_max = UINT64_MAX;
				break;
			default:
				NS_LOG(NS_LOG_LVL_FATAL,
				       "Invalid size_max value for alloc_type %s in '%s'", item_id,
				       log_src_str);
				goto error_cleanup;
			}

			switch (nsc_j_obj_strnum_get(j_ent, "base", &tmpu64)) {
			case 0:
				entry->base = tmpu64;
				break;
			case -ENOENT:
				entry->base = 0;
				break;
			default:
				NS_LOG(NS_LOG_LVL_FATAL,
				       "Invalid base value for alloc_type %s in '%s'", item_id,
				       log_src_str);
				goto error_cleanup;
			}

			la = la && (entry->islands_count == 1) && (entry->targets_count == 1) &&
			     (entry->islands[0] == entry->targets[0]);
		}

		alloc_type->local_alloc = la;
	}

	j_top = json_object_get(j_root, "alloc_aliases");
	_ARRAY_ALLOC(&desc->alloc_aliases, json_object_size(j_top));
	NSC_JOBJ_FOREACH(j_top, item_id, j_topitem, n)
	{
		struct nsc_chipdata_alloc_alias *alloc_alias;
		size_t idx;

		alloc_alias = &desc->alloc_aliases.entry[n];
		_STRTAB_ADD_OOM(alloc_alias->alias, &data->strtab, item_id);

		if (!json_string_value(j_topitem)) {
			NS_LOG(NS_LOG_LVL_FATAL,
			       "No allocation type specified for allocation alias '%s'",
			       alloc_alias->alias);
			goto error_cleanup;
		}
		_STRTAB_ADD_OOM(alloc_alias->name, &data->strtab, json_string_value(j_topitem));

		for (idx = 0; idx < desc->alloc_types.count; idx++) {
			struct _nfp_chipdata_alloc_type *atype = &desc->alloc_types.entry[idx];

			if (strcmp(NSC_STR(atype->name), NSC_STR(alloc_alias->name)) == 0)
				break;
		}
		if (idx >= desc->alloc_types.count) {
			NS_LOG(NS_LOG_LVL_FATAL,
			       "Invalid allocation type '%s' specified for allocation alias '%s'",
			       NSC_STR(alloc_alias->name), NSC_STR(alloc_alias->alias));
			goto error_cleanup;
		}
	}

	return 0;

error_cleanup:
	return NFP_ERRNO(err);
}

int nsc_j_obj_strnum_get(json_t *j, const char *key, uint64_t *val)
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

struct nfp_chipdata_chip *nsc_chip_decl_find_by_id(struct nfp_chipdata *data, const char *id)
{
	struct nfp_chipdata_chip *decl;

	if ((!data) || (!id))
		return NULL;

	for (decl = (struct nfp_chipdata_chip *)data->chip_decls.node; !!decl;
	     decl = (struct nfp_chipdata_chip *)decl->node.next) {
		if (strcmp(decl->id, id) == 0)
			return decl;
	}

	return NULL;
}

struct nsc_chip_desc *nsc_chip_desc_find_by_path(struct nfp_chipdata *data,
						 const char *path,
						 int rev_min,
						 int rev_max)
{
	struct nsc_chip_desc *desc;

	if ((!data) || (!path))
		return NULL;

	for (desc = (struct nsc_chip_desc *)data->chip_descs.node; !!desc;
	     desc = (struct nsc_chip_desc *)desc->node.next) {
		if (!desc->file_path)
			continue;

		if (strcmp(desc->file_path, path) != 0)
			continue;

		if ((rev_min >= desc->rev_min) && (rev_max <= desc->rev_max))
			return desc;
	}

	return NULL;
}

#endif /* !defined(EMBEDDED) */

struct nsc_chip_desc *nsc_chip_desc_find_by_family(struct nfp_chipdata *data,
						   int family,
						   int rev_min,
						   int rev_max)
{
	struct nsc_chip_desc *desc;

	if (!data)
		return NULL;

	for (desc = (struct nsc_chip_desc *)data->chip_descs.node; !!desc;
	     desc = (struct nsc_chip_desc *)desc->node.next) {
		if (!!desc->file_path)
			continue;

		if (desc->family != family)
			continue;

		if ((rev_min >= desc->rev_min) && (rev_max <= desc->rev_max))
			return desc;
	}

	return NULL;
}

const struct nsc_desc_island_nfp6000 *nsc_nfp6000_island_find_by_iid(
	const struct nfp_chipdata_chip *chip,
	int iid)
{
	size_t idx;

	if ((!chip) || (chip->arch != NFP_CHIP_ARCH_TH))
		return NULL;

	if (!nfp_chipdata_has_island(chip, iid))
		return NULL;

#define _NSC_ISLAS chip->chip_desc.nfp6000->islands
	for (idx = 0; idx < _NSC_ISLAS.count; idx++) {
		struct nsc_desc_island_nfp6000 *island = &_NSC_ISLAS.entry[idx];

		if (island->id == iid)
			return island;
	}
#undef _NSC_ISLAS

	return NULL;
}

#if !defined(EMBEDDED)

struct nsc_desc_island_nfp6000 *nsc_nfp6000_island_find_by_iid_nc(struct nfp_chipdata_chip *chip,
								  int iid)
{
	size_t idx;

	if ((!chip) || (chip->arch != NFP_CHIP_ARCH_TH))
		return NULL;

	if (!nfp_chipdata_has_island(chip, iid))
		return NULL;

#define _NSC_ISLAS chip->chip_desc.nfp6000->islands
	for (idx = 0; idx < _NSC_ISLAS.count; idx++) {
		struct nsc_desc_island_nfp6000 *island = &_NSC_ISLAS.entry[idx];

		if (island->id == iid)
			return island;
	}
#undef _NSC_ISLAS

	return NULL;
}

#endif

int nsc_enum_from_string(const struct nsc_enum_string_map *str_map, const char *str)
{
	const struct nsc_enum_string_map *ent;

	if (!str)
		return 0;

	for (ent = str_map; !!ent->str; ent++) {
		if (strcasecmp(ent->str, str) == 0)
			return ent->val;
	}

	return 0;
}

int nsc_enum_from_stringn(const struct nsc_enum_string_map *str_map, const char *str, size_t n)
{
	const struct nsc_enum_string_map *ent;

	if (!str)
		return 0;

	for (ent = str_map; !!ent->str; ent++) {
		if ((strncasecmp(ent->str, str, n) == 0) && (ent->str[n] == '\0'))
			return ent->val;
	}

	return 0;
}

static const char *nsc_enum_to_string(const struct nsc_enum_string_map *str_map,
				      int val,
				      const char *default_str)
{
	const struct nsc_enum_string_map *ent;

	if (val == 0)
		return default_str;

	for (ent = str_map; !!ent->str; ent++) {
		if (ent->val == val)
			return ent->str;
	}

	return default_str;
}

void _nfp_chipdata_chip_free(struct nfp_chipdata_chip *chip)
{
	if (!chip)
		return;

	if (!!chip->desc_override)
		free(chip->desc_override);
	if (!!chip->description)
		free(chip->description);
	if (!!chip->id)
		free(chip->id);
	if (!!chip->inherit)
		free(chip->inherit);

	_ARRAY_FREE(&chip->revisions);
	_ARRAY_FREE(&chip->revision_models);

	_ARRAY_FREE(&chip->cfgs);

	switch (chip->arch) {
	case NFP_CHIP_ARCH_YD:
		_ARRAY_FREE(&chip->details.nfp3200.cpp_targets);
		break;
	case NFP_CHIP_ARCH_TH:
		_ARRAY_FREE(&chip->details.nfp6000.islands);
		_ARRAY_FREE(&chip->cppats);
		break;
	default:
		break;
	}
}

void nfp_chipdata_chip_free(struct nfp_chipdata_chip *chip)
{
	if (!chip)
		return;
	_nfp_chipdata_chip_free(chip);
	free(chip);
}

#if !defined(EMBEDDED)

int nsc_chip_desc_dup(struct nfp_chipdata_chip *chip)
{
	struct nfp_chipdata *data = chip->chipdata;
	struct nsc_chip_desc *desc = NULL;
	int err = EINVAL;
	const struct nsc_chip_desc_nfp6000 *orig_nfp6000;
	const struct nsc_chip_desc_nfp3200 *orig_nfp3200;
	size_t idx, jdx, sz, alloc_type_cnt;
	struct _nfp_chipdata_alloc_type *alloc_types = NULL;
	struct _nfp_chipdata_alloc_type *orig_alloc_types = NULL;

	if (!chip) {
		NS_LOG(NS_LOG_LVL_FATAL, "Invalid parameters.");
		err = EINVAL;
		goto error_cleanup;
	}

	desc = nsc_llist_add(&data->chip_descs, sizeof(*desc));
	_OOM_CHECK(desc);

	desc->family = chip->family;
	desc->arch = _NFP_ARCH(desc->family);

	switch (desc->arch) {
	case NFP_CHIP_ARCH_YD:
		orig_nfp3200 = chip->chip_desc.nfp3200;
		desc->desc.nfp3200.cluster_count = orig_nfp3200->cluster_count;
		_ARRAY_DUP(&desc->desc.nfp3200.cpp_targets, &orig_nfp3200->cpp_targets);
		_ARRAY_DUP(&desc->desc.nfp3200.memories, &orig_nfp3200->memories);
		_ARRAY_DUP(&desc->desc.nfp3200.alloc_aliases, &orig_nfp3200->alloc_aliases);

		orig_alloc_types = orig_nfp3200->alloc_types.entry;
		_ARRAY_DUP(&desc->desc.nfp3200.alloc_types, &orig_nfp3200->alloc_types);
		alloc_types = desc->desc.nfp3200.alloc_types.entry;
		alloc_type_cnt = orig_nfp3200->alloc_types.count;
		break;
	case NFP_CHIP_ARCH_TH:
		orig_nfp6000 = chip->chip_desc.nfp6000;
		_ARRAY_DUP(&desc->desc.nfp6000.cpp_targets, &orig_nfp6000->cpp_targets);
		_ARRAY_DUP(&desc->desc.nfp6000.memories, &orig_nfp6000->memories);
		_ARRAY_DUP(&desc->desc.nfp6000.alloc_aliases, &orig_nfp6000->alloc_aliases);

		orig_alloc_types = orig_nfp6000->alloc_types.entry;
		_ARRAY_DUP(&desc->desc.nfp6000.alloc_types, &orig_nfp6000->alloc_types);
		alloc_types = desc->desc.nfp6000.alloc_types.entry;
		alloc_type_cnt = orig_nfp6000->alloc_types.count;

		_ARRAY_DUP(&desc->desc.nfp6000.islands, &orig_nfp6000->islands);
		for (idx = 0; idx < orig_nfp6000->islands.count; idx++) {
			struct nsc_desc_island_nfp6000 *orig_isld =
				&orig_nfp6000->islands.entry[idx];
			struct nsc_desc_island_nfp6000 *isld =
				&desc->desc.nfp6000.islands.entry[idx];

			sz = sizeof(*isld->cpp_tgt_ids) * isld->cpp_tgt_id_count;
			_MALLOC_OOM(isld->cpp_tgt_ids, sz);
			memcpy(isld->cpp_tgt_ids, orig_isld->cpp_tgt_ids, sz);

			sz = sizeof(*isld->mem_types) * isld->mem_types_count;
			_MALLOC_OOM(isld->mem_types, sz);
			memcpy(isld->mem_types, orig_isld->mem_types, sz);

			sz = sizeof(*isld->mem_cfgs) * isld->mem_types_count;
			_MALLOC_OOM(isld->mem_cfgs, sz);
			memcpy(isld->mem_cfgs, orig_isld->mem_cfgs, sz);
		}
		break;
	default:
		err = EINVAL;
		goto error_cleanup;
	}

	for (idx = 0; idx < alloc_type_cnt; idx++, alloc_types++, orig_alloc_types++) {
		struct _nfp_chipdata_alloc *alloc = NULL;
		struct _nfp_chipdata_alloc *orig_alloc = orig_alloc_types->entries;

		sz = alloc_types->entries_count * sizeof(*alloc_types->entries);
		_MALLOC_OOM(alloc_types->entries, sz);
		memcpy(alloc_types->entries, orig_alloc_types->entries, sz);
		alloc = alloc_types->entries;

		for (jdx = 0; jdx < orig_alloc_types->entries_count; jdx++, alloc++, orig_alloc++) {
			sz = orig_alloc->islands_count * sizeof(*orig_alloc->islands);
			_MALLOC_OOM(alloc->islands, sz);
			memcpy(alloc->islands, orig_alloc->islands, sz);

			sz = orig_alloc->targets_count * sizeof(*orig_alloc->targets);
			_MALLOC_OOM(alloc->targets, sz);
			memcpy(alloc->targets, orig_alloc->targets, sz);
		}
	}

	chip->chip_desc.vp = (void *)&desc->desc;
	return 0;

error_cleanup:
	return NFP_ERRNO(err);
}

int nsc_chip_apply_cfg(struct nfp_chipdata_chip *chip)
{
	if (!chip) {
		NS_LOG(NS_LOG_LVL_FATAL, "Invalid parameters.");
		return NFP_ERRNO(EINVAL);
	}

	switch (chip->arch) {
	case NFP_CHIP_ARCH_YD:
		return nsc_nfp3200_chip_apply_cfg(chip);
	case NFP_CHIP_ARCH_TH:
		return nsc_nfp6000_chip_apply_cfg(chip);
	default:
		break;
	}

	NS_LOG(NS_LOG_LVL_FATAL, "Invalid chip family.");
	return NFP_ERRNO(EINVAL);
}

int nsc_nfp3200_chip_apply_cfg(struct nfp_chipdata_chip *chip)
{
	(void)chip;
	return 0;
}

int nsc_nfp6000_chip_apply_cfg(struct nfp_chipdata_chip *chip)
{
	int err = EINVAL;
	size_t idx, jdx;
	struct nfp_chipdata *data = NULL;

	data = chip->chipdata;

	for (idx = 0; idx < chip->cfgs.count; idx++) {
		uint64_t v64;
		const char *key = NSC_STR(chip->cfgs.entry[idx].k);
		const char *val = NSC_STR(chip->cfgs.entry[idx].v);
		const char *c;
		char *c2;
		int iid, menum;
		struct nsc_desc_island_nfp6000 *island = NULL;
		enum nfp_chipdata_mem_type mem_type;
		struct nsc_desc_island_mem_cfg *mem_cfg = NULL;

		/* Ignore empty values. */
		if (*val == '\0')
			continue;

		/* i24.emem.base */
		mem_type = nfp_chipdata_idstr2mem_type(chip->family, key, &c, &iid, &menum);
		if (mem_type == NFP_CHIPDATA_MEM_TYPE_NONE)
			continue;

		island = nsc_nfp6000_island_find_by_iid_nc(chip, iid);
		if (!island) {
			NS_LOG(NS_LOG_LVL_EXTRA, "Ignoring config %s.%s.", chip->id, key);
			continue;
		}

		for (jdx = 0; jdx < island->mem_types_count; jdx++) {
			if (island->mem_types[jdx] == mem_type) {
				mem_cfg = &island->mem_cfgs[jdx];
				break;
			}
		}

		if (jdx == island->mem_types_count) {
			NS_LOG(NS_LOG_LVL_EXTRA, "Ignoring config %s.%s.", chip->id, key);
			continue;
		}

		if (!strcmp(c, "base")) {
			v64 = nfp_strtou64(val, &c2, 0);
			if (*c2 != '\0') {
				NS_LOG(NS_LOG_LVL_FATAL, "Invalid config value for %s.%s", chip->id,
				       key);
				err = EINVAL;
				goto error_cleanup;
			}
			mem_cfg->base = v64;

		} else if (!strcmp(c, "alloc_base")) {
			v64 = nfp_strtou64(val, &c2, 0);
			if (*c2 != '\0') {
				NS_LOG(NS_LOG_LVL_FATAL, "Invalid config value for %s.%s", chip->id,
				       key);
				err = EINVAL;
				goto error_cleanup;
			}
			mem_cfg->alloc_base = v64;

		} else if (!strcmp(c, "size")) {
			if (!strcmp(val, "max")) {
				v64 = UINT64_MAX;
			} else {
				v64 = nfp_strtou64(val, &c2, 0);
				if (*c2 != '\0') {
					NS_LOG(NS_LOG_LVL_FATAL, "Invalid config value for %s.%s",
					       chip->id, key);
					err = EINVAL;
					goto error_cleanup;
				}
			}
			mem_cfg->size = v64;
		} else if (!strcmp(c, "cache_as_ddr")) {
			if (mem_type != NFP_CHIPDATA_MEM_TYPE_EMEM_CACHE) {
				NS_LOG(NS_LOG_LVL_FATAL, "Invalid config mem-type for %s.%s",
				       chip->id, key);
				err = EINVAL;
				goto error_cleanup;
			}

			if ((val[0] | 0x20) == 'y') {
				for (jdx = 0; jdx < island->mem_types_count; jdx++) {
					if (island->mem_types[jdx] == NFP_CHIPDATA_MEM_TYPE_EMEM) {
						island->mem_types_count--;
						break;
					}
				}

				for (; jdx < island->mem_types_count; jdx++) {
					island->mem_types[jdx] = island->mem_types[jdx + 1];
					island->mem_cfgs[jdx] = island->mem_cfgs[jdx + 1];
				}
			}
		}
	}

	return 0;
error_cleanup:
	return NFP_ERRNO(err);
}

#endif /* !defined(EMBEDDED) */

void nsc_chip_desc_free(struct nsc_chip_desc *desc)
{
	size_t idx, idx2;
	struct nsc_chip_desc_nfp6000 *desc_nfp6000;
	struct nsc_chip_desc_nfp3200 *desc_nfp3200;

	if (!desc)
		return;

	if (!!desc->file_path)
		free(desc->file_path);

	switch (desc->arch) {
	case NFP_CHIP_ARCH_YD:
		desc_nfp3200 = &desc->desc.nfp3200;

		_ARRAY_FREE(&desc_nfp3200->cpp_targets);
		_ARRAY_FREE(&desc_nfp3200->memories);
		_ARRAY_FREE(&desc_nfp3200->alloc_aliases);

		for (idx = 0; idx < desc_nfp3200->alloc_types.count; idx++) {
			struct _nfp_chipdata_alloc_type *alloctype =
				&desc_nfp3200->alloc_types.entry[idx];

			for (idx2 = 0; idx2 < alloctype->entries_count; idx2++) {
				struct _nfp_chipdata_alloc *alloc = &alloctype->entries[idx2];

				if (!!alloc->islands)
					free(alloc->islands);
				if (!!alloc->targets)
					free(alloc->targets);
			}

			if (!!alloctype->entries)
				free(alloctype->entries);
		}
		_ARRAY_FREE(&desc_nfp3200->alloc_types);

		break;
	case NFP_CHIP_ARCH_TH:
		desc_nfp6000 = &desc->desc.nfp6000;

		_ARRAY_FREE(&desc_nfp6000->cpp_targets);

		for (idx = 0; idx < desc_nfp6000->islands.count; idx++) {
			struct nsc_desc_island_nfp6000 *island = &desc_nfp6000->islands.entry[idx];

			if (!!island->cpp_tgt_ids)
				free(island->cpp_tgt_ids);
			if (!!island->mem_types)
				free(island->mem_types);
			if (!!island->mem_cfgs)
				free(island->mem_cfgs);
			memset(island, 0, sizeof(*island));
		}
		_ARRAY_FREE(&desc_nfp6000->islands);
		_ARRAY_FREE(&desc_nfp6000->memories);
		_ARRAY_FREE(&desc_nfp6000->alloc_aliases);

		for (idx = 0; idx < desc_nfp6000->alloc_types.count; idx++) {
			struct _nfp_chipdata_alloc_type *alloctype =
				&desc_nfp6000->alloc_types.entry[idx];

			for (idx2 = 0; idx2 < alloctype->entries_count; idx2++) {
				struct _nfp_chipdata_alloc *alloc = &alloctype->entries[idx2];

				if (!!alloc->islands)
					free(alloc->islands);
				if (!!alloc->targets)
					free(alloc->targets);
			}

			if (!!alloctype->entries)
				free(alloctype->entries);
		}
		_ARRAY_FREE(&desc_nfp6000->alloc_types);

		break;
	default:
		break;
	}

	free(desc);
}

#if !defined(EMBEDDED)

char *nsc6000_expand_fmtstr(const char *fmtstr, int iid, int altstr)
{
	const char *inm = fmtstr;
	char *onm;
	char idstr[NFP_MEID_STR_SZ + 32]; // space for numbers and id strings
	char *s = NULL;
	size_t ssz = 128;
	size_t slen = 0;
	int err = EINVAL;

	_CALLOC_OOM(s, ssz, 1);
	onm = s;

	while (*inm) {
		if (*inm == '{') {
			if (strncmp(inm, "{siid}", 6) == 0) {
				inm += 6;
				idstr[0] = 0;
				if (altstr)
					nfp6000_island2altstr(idstr, iid);
				else
					nfp6000_island2str(idstr, iid);
			} else if (strncmp(inm, "{niid}", 6) == 0) {
				inm += 6;
				snprintf(idstr, sizeof(idstr), "%d", iid);
				idstr[sizeof(idstr) - 1] = 0;
			}

			if ((slen + strlen(idstr) + 1) >= ssz) {
				_REALLOC_OOM(s, (ssz + strlen(idstr) + 128));
				ssz += strlen(idstr) + 128;
			}
			strcat(onm, idstr);
			onm += strlen(idstr);
		} else {
			if ((slen + 1) >= ssz) {
				_REALLOC_OOM(s, (ssz + 128));
				ssz += 128;
			}
			*onm++ = *inm++;
			slen++;
		}
	}

	return s;

error_cleanup:
	if (s)
		free(s);
	return NFP_ERRPTR(err);
}
#endif /* !defined(EMBEDDED) */

int nsc_comp_int_asc(const void *p1, const void *p2)
{
	int a = *((const int *)p1);
	int b = *((const int *)p2);

	return (a > b) - (a < b);
}

#if !defined(EMBEDDED)
int nsc_comp_chipdecl_asc(const void *p1, const void *p2)
{
	const struct nfp_chipdata_chip *a = *((const struct nfp_chipdata_chip **)p1);
	const struct nfp_chipdata_chip *b = *((const struct nfp_chipdata_chip **)p2);

	return strcmp(a->id, b->id);
}

int nsc_comp_nfp6000_cppat_asc(const void *p1, const void *p2)
{
	int d;
	const union nfp_chipdata_cppat_entry *a = (const union nfp_chipdata_cppat_entry *)p1;
	const union nfp_chipdata_cppat_entry *b = (const union nfp_chipdata_cppat_entry *)p2;

	d = (a->nfp6000.from_iid > b->nfp6000.from_iid) -
	    (a->nfp6000.from_iid < b->nfp6000.from_iid);
	if (d != 0)
		return d;
	return (a->nfp6000.cpp_tgt > b->nfp6000.cpp_tgt) -
	       (a->nfp6000.cpp_tgt < b->nfp6000.cpp_tgt);
}

size_t nsc_strtab_add(struct nsc_strtab *strtab, const char *str)
{
	size_t strsz = 0, origsz;
	char *s, *end;

	if (!str)
		return SIZE_MAX;

	strsz = strlen(str) + 1;
	end = strtab->str + strtab->offset;

	/* Check if string is already present. */
	for (s = strtab->str; s < end; s += strlen(s) + 1) {
		if (strcmp(s, str) == 0)
			return (s - strtab->str);
	}

	origsz = strtab->alloced;
	if (!origsz)
		strsz++;
	while (strtab->offset + strsz > strtab->alloced)
		strtab->alloced = strtab->alloced ? strtab->alloced * 2 : 256;

	if (strtab->alloced > origsz) {
		s = realloc(strtab->str, strtab->alloced);
		if (!s)
			return SIZE_MAX;
		strtab->str = s;
	}

	if (!origsz)
		strtab->str[strtab->offset++] = '\0', strsz--;

	s = &strtab->str[strtab->offset];
	strncpy(s, str, strsz);
	strtab->offset += strsz;
	return (s - strtab->str);
}

#endif /* !defined(EMBEDDED) */

void nsc_strtab_free(struct nsc_strtab *strtab)
{
	if (!strtab)
		return;
	if (!!strtab->str)
		free(strtab->str);
	memset(strtab, 0, sizeof(*strtab));
}

#if !defined(EMBEDDED)
void *nsc_llist_add(struct nsc_llist *llist, size_t data_sz)
{
	struct nsc_llist_node *new_node;
	struct nsc_llist_node *node;

	if (!llist->node) {
		llist->node = calloc(1, data_sz);
		if (llist->node)
			llist->cnt++;
		return llist->node;
	}

	new_node = calloc(1, data_sz);
	if (!new_node)
		return NULL;

	node = llist->node;
	while (node->next)
		node = node->next;
	node->next = new_node;
	llist->cnt++;

	return new_node;
}

int nsc_paths_to_strtab(struct nsc_strtab *strtab, const char *search_paths)
{
	char *tokpath = NULL;
	const char *cur = NULL;
	char *next = NULL;

	if ((!strtab) || (!search_paths))
		return 0;

	tokpath = safe_strdup(search_paths);
	next = tokpath;
	cur = tokpath;
	if (!tokpath) {
		NS_LOG(NS_LOG_LVL_FATAL, "Out of memory.");
		return -ENOMEM;
	}

	while (!!(next = strchr(cur, PATH_LIST_SEP_C))) {
		*next = '\0';
		next++;

		if (nsc_strtab_add(strtab, cur) == SIZE_MAX) {
			free(tokpath);
			return -ENOMEM;
		}
		cur = next;
	}

	if (*cur == '\0')
		cur = ".";

	if (!nsc_strtab_add(strtab, cur)) {
		free(tokpath);
		return -ENOMEM;
	}

	free(tokpath);
	return 0;
}

#endif /* !defined(EMBEDDED) */

int nsc_has_cppat(const struct nfp_chipdata_chip *chip, int island_id, int cpp_tgt)
{
	size_t idx;
	union nfp_chipdata_cppat_entry *cppat;

	if ((!chip) || (island_id < 0)) {
		NS_LOG(NS_LOG_LVL_ERROR, "Invalid parameters.");
		return 0;
	}

	switch (chip->arch) {
	case NFP_CHIP_ARCH_YD:
		return 0;
	case NFP_CHIP_ARCH_TH:
		for (idx = 0, cppat = chip->cppats.entry; idx < chip->cppats.count;
		     idx++, cppat++) {
			if ((island_id == cppat->nfp6000.from_iid) &&
			    (cpp_tgt == cppat->nfp6000.cpp_tgt))
				return 1;
		}
		break;
	default:
		break;
	}

	return 0;
}

// ==== API implementation below ====
const char *nfp_chipdata_path(const struct nfp_chipdata *data, int default_path)
{
	if (data->embedded_loaded) {
		if (!default_path)
			return NULL;
	}
	return data->chipdata_path;
}

const char *nfp_chipdata_rel_path()
{
	return NFP_CHIPDATA_BIN_REL_PATH;
}

const char *nfp_chipdata_family2str(int family)
{
	return nsc_enum_to_string(nsc_family_string_map, family, NULL);
}

int nfp_chipdata_str2family(const char *family_str)
{
	return nsc_enum_from_string(nsc_family_string_map, family_str);
}

const char *nfp_chipdata_str(const struct nfp_chipdata *data, size_t offset)
{
	if ((!data) || (offset >= data->strtab.offset)) {
		NS_LOG(NS_LOG_LVL_FATAL, "Invalid parameters.");
		return NFP_ERRPTR(EINVAL);
	}

	return &data->strtab.str[offset];
}

#if !defined(EMBEDDED)

struct nfp_chipdata *nfp_chipdata_load(const char *include_paths, const char *chipdata_path)
{
	struct nfp_chipdata *data = NULL;
	const char *pathc;
	size_t idxa;
	int ret;
	int done;
	int err = EINVAL;
	json_t *embedded_json = NULL;
	struct nsc_llist_node *node;

	if (!nsc_sanity_check()) {
		NS_LOG(NS_LOG_LVL_FATAL, "Internal sanity check failed.");
		return NFP_ERRPTR(err);
	}

	embedded_json = nsc_json_file_load_embedded();
	if (!embedded_json)
		return NULL;

	_CALLOC_OOM(data, 1, sizeof(struct nfp_chipdata));

	if (!chipdata_path)
		chipdata_path = getenv(NFP_CHIPDATA_OVR_PATH_ENVVAR);

	if (!!chipdata_path) {
		size_t sz = strlen(chipdata_path);

		// Keep space for trailing separator after path cleanup
		_MALLOC_OOM(data->chipdata_path, (sz + 2));
		strncpy(data->chipdata_path, chipdata_path, sz);
		data->chipdata_path[sz] = '\0';
	} else {
		data->chipdata_path = nfp_util_bin_dir();
		if (!data->chipdata_path) {
			NS_LOG(NS_LOG_LVL_FATAL, "Could not locate bin directory.");
			goto error_cleanup;
		}

		{
			// ensure space for path separator
			char *s = realloc(data->chipdata_path,
					  strlen(data->chipdata_path) +
						  strlen(NFP_CHIPDATA_BIN_REL_PATH) + 1 + 1);
			_OOM_CHECK(s);

			data->chipdata_path = s;

			strcat(data->chipdata_path, NFP_CHIPDATA_BIN_REL_PATH);
		}

		data->embedded_loaded = 1;
	}

	if (!nfp_util_normalize_abspath(data->chipdata_path)) {
		NS_LOG(NS_LOG_LVL_FATAL, "Error cleaning up path: %s", data->chipdata_path);
		goto error_cleanup;
	}
	strcat(data->chipdata_path, PATH_SEP_S);

	if ((!!include_paths) && (include_paths[0] != '\0')) {
		err = -nsc_paths_to_strtab(&data->search_paths, include_paths);
		if (err != 0)
			goto error_cleanup;
	}

	include_paths = getenv(NFP_CHIPDATA_INCLUDE_PATH_ENVVAR);
	if (!!include_paths) {
		err = -nsc_paths_to_strtab(&data->search_paths, include_paths);
		if (err != 0)
			goto error_cleanup;
	}
	// Now look for user chip definitions.
	for (pathc = data->search_paths.str;
	     pathc < (data->search_paths.str + data->search_paths.offset);
	     pathc += (strlen(pathc) + 1)) {
		char *file_path;
		json_t *j_root = NULL;
		FILE *f = NULL;

		_MALLOC_OOM(file_path,
			    strlen(pathc) + 1 + strlen(NFP_CHIPDATA_FILE_NAME_CHIPS) + 1);
		strcpy(file_path, pathc);
		strcat(file_path, PATH_SEP_S);
		strcat(file_path, NFP_CHIPDATA_FILE_NAME_CHIPS);

		if (!nfp_util_normalize_abspath(file_path)) {
			NS_LOG(NS_LOG_LVL_FATAL, "Error cleaning up path: %s", file_path);
			free(file_path);
			goto error_cleanup;
		}

		f = fopen(file_path, "r");
		if (!!f) {
			fclose(f);
			j_root = nsc_json_file_load(data, file_path);
			if (!j_root) {
				err = EIO;
				NS_LOG(NS_LOG_LVL_FATAL, "Could not load required file: %s",
				       file_path);
				free(file_path);
				goto error_cleanup;
			}

			if ((nsc_json_extract_chips(data, j_root, file_path, 0) != 0) ||
			    (nsc_json_extract_chips(data, j_root, file_path, 1) != 0)) {
				err = errno;
				json_decref(j_root);
				free(file_path);
				goto error_cleanup;
			}

			json_decref(j_root);
		}

		free(file_path);
	}

	{
		char *file_path = NULL;
		json_t *j_loaded = NULL;
		json_t *j_root = NULL, *j_top;
		size_t idx;

		if (!chipdata_path) {
			j_root = json_object_get(embedded_json, NSC_FILE_NAME_CHIPS_INTERNAL);
			if (!j_root) {
				err = EIO;
				NS_LOG(NS_LOG_LVL_FATAL, "Embedded JSON has no %s",
				       NSC_FILE_NAME_CHIPS_INTERNAL);
				free(file_path);
				goto error_cleanup;
			}
			_MALLOC_OOM(file_path,
				    strlen("<embedded>/" NSC_FILE_NAME_CHIPS_INTERNAL) + 1);
			strcpy(file_path, "<embedded>/" NSC_FILE_NAME_CHIPS_INTERNAL);
		} else {
			_MALLOC_OOM(file_path,
				    strlen(data->chipdata_path) +
					    strlen(NSC_FILE_NAME_CHIPS_INTERNAL) + 1);
			strcpy(file_path, data->chipdata_path);
			strcat(file_path, NSC_FILE_NAME_CHIPS_INTERNAL);

			j_loaded = nsc_json_file_load(data, file_path);
			if (!j_loaded) {
				err = EIO;
				NS_LOG(NS_LOG_LVL_FATAL, "Could not load required file: %s",
				       file_path);
				free(file_path);
				goto error_cleanup;
			}
			j_root = j_loaded;
		}

		j_top = json_object_get(j_root, "_comment");
		if (json_is_array(j_top)) {
			size_t sz = 0;
			const char *jstr;

			sz = strlen("\nLoaded from:\n") + strlen(file_path) + 1;

			for (idx = 0; idx < json_array_size(j_top); idx++) {
				jstr = json_string_value(json_array_get(j_top, idx));
				if (!jstr)
					continue;
				sz += strlen(jstr) + 1;
			}
			sz++;
			_CALLOC_OOM(data->json_comment, sz, 1);
			for (idx = 0; idx < json_array_size(j_top); idx++) {
				jstr = json_string_value(json_array_get(j_top, idx));
				if (!jstr)
					continue;
				strcat(data->json_comment, jstr);
				strcat(data->json_comment, "\n");
			}
			strcat(data->json_comment, "\nLoaded from:\n");
			strcat(data->json_comment, file_path);
			strcat(data->json_comment, "\n");
		} else {
			_CALLOC_OOM(data->json_comment, 1, 1);
		}

		if ((nsc_json_extract_chips(data, j_root, file_path, 0) != 0) ||
		    (nsc_json_extract_chips(data, j_root, file_path, 1) != 0)) {
			err = errno;
			json_decref(j_root);
			free(file_path);
			goto error_cleanup;
		}

		if (!!j_loaded)
			json_decref(j_loaded);
		free(file_path);
	}

	// Sort decls, qsort on temporary array
	if (data->chip_decls.cnt > 0) {
		void **arr = calloc(data->chip_decls.cnt, sizeof(void *));
		size_t idx;

		for (node = data->chip_decls.node, idx = 0; idx < data->chip_decls.cnt;
		     node = node->next, idx++) {
			arr[idx] = node;
		}

		qsort(arr, data->chip_decls.cnt, sizeof(void *), nsc_comp_chipdecl_asc);

		data->chip_decls.node = arr[0];
		node = data->chip_decls.node;

		for (idx = 1; idx < data->chip_decls.cnt; idx++) {
			node->next = arr[idx];
			node = node->next;
		}
		node->next = NULL;
		free(arr);
	}
	// Chip definitions loaded. Get chip descriptions (desc).
	{
		struct nsc_enum_string_map *ent;

		for (ent = nsc_family_string_map; !!ent->str; ent++) {
			char *file_path = NULL;
			json_t *j_root = NULL;
			json_t *j_loaded = NULL;

			if (!chipdata_path) {
				j_root = json_object_get(embedded_json, ent->str);
				j_root = json_object_get(j_root, NSC_FILE_NAME_DESC_INTERNAL);
				if (!j_root) {
				// This family has no embedded data, this is not an error if
				// it was intentional.
					continue;
				}
			} else {
				FILE *f = NULL;

				_MALLOC_OOM(file_path,
					    strlen(data->chipdata_path) + strlen(ent->str) + 1 +
						    strlen(NSC_FILE_NAME_DESC_INTERNAL) + 1);
				strcpy(file_path, data->chipdata_path);
				strcat(file_path, ent->str);
				strcat(file_path, PATH_SEP_S);
				strcat(file_path, NSC_FILE_NAME_DESC_INTERNAL);

				f = fopen(file_path, "r");
				if (!f) {
					free(file_path);
					continue;
				}
				fclose(f);

				j_loaded = nsc_json_file_load(data, file_path);
				if (!j_loaded) {
					err = EIO;
					NS_LOG(NS_LOG_LVL_FATAL, "Could not load required file: %s",
					       file_path);
					free(file_path);
					goto error_cleanup;
				}
				j_root = j_loaded;
			}

			switch (ent->val) {
			case NFP_CHIP_FAMILY_NFP3200:
				ret = nsc_json_extract_desc_nfp3200_top(data, j_root, NULL);
				if (ret != 0)
					err = errno;
				break;
			case NFP_CHIP_FAMILY_NFP6000:
			case NFP_CHIP_FAMILY_NFP5000:
			case NFP_CHIP_FAMILY_NFP4000:
			case NFP_CHIP_FAMILY_NFP3800:
				ret = nsc_json_extract_desc_nfp6000_top(data, j_root, NULL, ent->val);
				if (ret != 0)
					err = errno;
				break;
			default:
				NS_LOG(NS_LOG_LVL_FATAL, "Internal error, invalid chip family");
				if (!!file_path)
					free(file_path);
				json_decref(j_root);
				err = EINVAL;
				goto error_cleanup;
			}

			if (!!j_loaded)
				json_decref(j_loaded);

			if (!!file_path)
				free(file_path);
			if (ret != 0)
				goto error_cleanup;
		}
	}

	// Sanity check on alloc_type size/base values.
	for (node = data->chip_descs.node; (!!node); node = node->next) {
		struct nsc_chip_desc *desc = (struct nsc_chip_desc *)node;
		struct _nfp_chipdata_alloc_type *alloc_types = NULL;
		struct _nfp_chipdata_memory *memories = NULL;
		size_t mem_cnt = 0;
		size_t alloc_cnt = 0;
		size_t ent_idx, alloc_idx;

		switch (desc->arch) {
		case NFP_CHIP_ARCH_YD:
			alloc_types = &desc->desc.nfp3200.alloc_types.entry[0];
			alloc_cnt = desc->desc.nfp3200.alloc_types.count;
			memories = &desc->desc.nfp3200.memories.entry[0];
			mem_cnt = desc->desc.nfp3200.memories.count;
			break;
		case NFP_CHIP_ARCH_TH:
			alloc_types = &desc->desc.nfp6000.alloc_types.entry[0];
			alloc_cnt = desc->desc.nfp6000.alloc_types.count;
			memories = &desc->desc.nfp6000.memories.entry[0];
			mem_cnt = desc->desc.nfp6000.memories.count;
			break;
		default:
			break;
		}

		for (alloc_idx = 0; alloc_idx < alloc_cnt; alloc_idx++, alloc_types++)
			for (ent_idx = 0; ent_idx < alloc_types->entries_count; ent_idx++) {
				struct _nfp_chipdata_alloc *alloc = &alloc_types->entries[ent_idx];
				size_t mem_idx;

				for (mem_idx = 0; mem_idx < mem_cnt; mem_idx++) {
					if (memories[mem_idx].type == alloc->type)
						break;
				}
				if (mem_idx == mem_cnt) {
					NS_LOG(NS_LOG_LVL_FATAL, "Invalid alloc_type memory type.");
					goto error_cleanup;
				}

				if (alloc->size_max == UINT64_MAX) {
					alloc->size_max = memories[mem_idx].size_max - alloc->base;
				} else if ((alloc->base + alloc->size_max) >
					   memories[mem_idx].size_max) {
					NS_LOG(NS_LOG_LVL_FATAL,
					       "Invalid alloc_type base and size_max values, out of range for memory type.");
					goto error_cleanup;
				}
			}
	}

	for (node = data->chip_decls.node; (!!node); node = node->next) {
		json_t *j_root = NULL;
		struct nfp_chipdata_chip *decl = (struct nfp_chipdata_chip *)node;
		struct nsc_chip_desc *desc = NULL;

		if (!decl->desc_override) {
			if (!decl->inherit) {
				desc = nsc_chip_desc_find_by_family(data, decl->family,
								    decl->rev_min, decl->rev_max);
				if (!desc) {
					NS_LOG(NS_LOG_LVL_FATAL,
					       "Default family chip desc not found for '%s'",
					       decl->id);
					goto error_cleanup;
				}
				switch (decl->arch) {
				case NFP_CHIP_ARCH_YD:
					decl->chip_desc.nfp3200 = &desc->desc.nfp3200;
					break;
				case NFP_CHIP_ARCH_TH:
					decl->chip_desc.nfp6000 = &desc->desc.nfp6000;
					break;
				default:
					NS_LOG(NS_LOG_LVL_FATAL, "Internal error on chip '%s'",
					       decl->id);
					goto error_cleanup;
				}
			}
			continue;
		}

		desc = nsc_chip_desc_find_by_path(data, decl->desc_override, decl->rev_min,
						  decl->rev_max);
		if (!!desc) {
			switch (decl->arch) {
			case NFP_CHIP_ARCH_YD:
				decl->chip_desc.nfp3200 = &desc->desc.nfp3200;
				break;
			case NFP_CHIP_ARCH_TH:
				decl->chip_desc.nfp6000 = &desc->desc.nfp6000;
				break;
			default:
				NS_LOG(NS_LOG_LVL_FATAL, "Internal error on chip '%s'", decl->id);
				goto error_cleanup;
			}
			continue; // Already loaded this desc file.
		}

		j_root = nsc_json_file_load(data, decl->desc_override);
		if (!j_root) {
			err = EIO;
			NS_LOG(NS_LOG_LVL_FATAL, "Could not load required file: %s",
			       decl->desc_override);
			goto error_cleanup;
		}

		switch (decl->arch) {
		case NFP_CHIP_ARCH_YD:
			ret = nsc_json_extract_desc_nfp3200_top(data, j_root, decl->desc_override);
			if (ret != 0)
				err = errno;
			break;
		case NFP_CHIP_ARCH_TH:
			ret = nsc_json_extract_desc_nfp6000_top(data, j_root, decl->desc_override, decl->family);
			if (ret != 0)
				err = errno;
			break;
		default:
			NS_LOG(NS_LOG_LVL_FATAL, "Internal error loading file: %s",
			       decl->desc_override);
			json_decref(j_root);
			err = EINVAL;
			goto error_cleanup;
		}
		json_decref(j_root);

		if (ret != 0)
			goto error_cleanup;

		desc = nsc_chip_desc_find_by_path(data, decl->desc_override, decl->rev_min,
						  decl->rev_max);
		switch (decl->arch) {
		case NFP_CHIP_ARCH_YD:
			decl->chip_desc.nfp3200 = &desc->desc.nfp3200;
			break;
		case NFP_CHIP_ARCH_TH:
			decl->chip_desc.nfp6000 = &desc->desc.nfp6000;
			break;
		default:
			NS_LOG(NS_LOG_LVL_FATAL, "Internal error on chip '%s'", decl->id);
			goto error_cleanup;
		}

		free(decl->desc_override);
		decl->desc_override = NULL;
	}

	// Resolve inheritance.
	done = 0;
	while (!done) {
		done = 1;
		for (node = data->chip_decls.node; (!!node); node = node->next) {
			struct nfp_chipdata_chip *decl = (struct nfp_chipdata_chip *)node;
			struct nfp_chipdata_chip *decl_parent = NULL;

			if (!!decl->inherit) {
				done = 0;
				decl_parent = nsc_chip_decl_find_by_id(data, decl->inherit);
				if (!decl_parent) {
					NS_LOG(NS_LOG_LVL_FATAL,
					       "Chip '%s' inherits from non-existent '%s'",
					       decl->id, decl->inherit);
					goto error_cleanup;
				}
				if (!decl_parent->inherit) {
					/* Parent is resolved, copy properties down. */
					if (decl->family != decl_parent->family) {
						NS_LOG(NS_LOG_LVL_FATAL,
						       "Chip '%s' inherits from '%s' of different chip family",
						       decl->id, decl->inherit);
						goto error_cleanup;
					}

					if (!decl->chip_desc.vp)
						decl->chip_desc.vp = decl_parent->chip_desc.vp;
					if (!decl->description)
						_STRDUP_OOM(decl->description,
							    decl_parent->description);

					if ((decl->revisions.count == 0) &&
					    (decl_parent->revisions.count > 0)) {
						_ARRAY_DUP(&decl->revisions,
							   &decl_parent->revisions);
						_ARRAY_DUP(&decl->revision_models,
							   &decl_parent->revision_models);
					} else {
						/* For each revision model that is invalid, get one
						 * from the parent.
						 */
						size_t myi, pari;

						for (myi = 0; myi < decl->revisions.count; myi++) {
							if (decl->revision_models.entry[myi] !=
							    NFP_CHIPDATA_CPP_MODEL_INVALID)
								continue;
							for (pari = 0;
							     pari < decl_parent->revisions.count;
							     pari++) {
								if (decl_parent->revisions
									    .entry[pari] ==
								    decl->revisions.entry[myi]) {
									decl->revision_models
										.entry[myi] =
										decl_parent
											->revision_models
											.entry[pari];
									break;
								}
							}
						}
					}

					switch (decl->arch) {
					case NFP_CHIP_ARCH_YD:
						if (decl->details.nfp3200.cpp_targets.count == 0) {
							_ARRAY_DUP(
								&decl->details.nfp3200.cpp_targets,
								&decl_parent->details.nfp3200
									 .cpp_targets);
						}

						if (decl->details.nfp3200.me_cnt == 0)
							decl->details.nfp3200.me_cnt =
								decl_parent->details.nfp3200.me_cnt;

						done = 1;
						break;
					case NFP_CHIP_ARCH_TH:
						done = 1;
						if (decl->details.nfp6000.serdes_cnt == SIZE_MAX) {
							if (decl_parent->details.nfp6000
								    .serdes_cnt != SIZE_MAX) {
								decl->details.nfp6000.serdes_cnt =
									decl_parent->details.nfp6000
										.serdes_cnt;
								done = done && 1;
							} else {
								done = 0;
							}
						}

						if (decl->details.nfp6000.islands.count == 0) {
							_ARRAY_DUP(&decl->details.nfp6000.islands,
								   &decl_parent->details.nfp6000
									    .islands);
							done = 0;
						} else {
							// We know which islands we have, so inherit
							// cppats
							// we don't have yet - fine-grained
							// overrides
							union nfp_chipdata_cppat_entry *cppat;
							union nfp_chipdata_cppat_entry *parent_cppat;
							size_t cnt = 0, ncnt = 0;

							for (idxa = 0, parent_cppat = decl_parent->cppats.entry;
							     idxa < decl_parent->cppats.count; idxa++, parent_cppat++) {
								if (nfp_chipdata_has_island(
									    decl, parent_cppat->nfp6000.from_iid) &&
								    !nsc_has_cppat(decl, parent_cppat->nfp6000.from_iid,
										   parent_cppat->nfp6000.cpp_tgt))
									cnt++;
							}

							if (cnt > 0) {
								ncnt = decl->cppats.count + cnt;
								cnt = decl->cppats.count;
								if (ncnt > cnt) {
									_ARRAY_REALLOC(
										&decl->cppats,
										ncnt);
									// Pretend we didn't grow
									// yet
									// (don't access random new
									// memory)
									decl->cppats.count = cnt;
								}
								// point to previous end
								cppat = &decl->cppats.entry[cnt];
								for (idxa = 0,
								     parent_cppat =
									     decl_parent->cppats
										     .entry;
								     idxa <
								     decl_parent->cppats.count;
								     idxa++, parent_cppat++) {
									if (nfp_chipdata_has_island(
										    decl,
										    parent_cppat
											    ->nfp6000
											    .from_iid) &&
									    !nsc_has_cppat(
										    decl,
										    parent_cppat
											    ->nfp6000
											    .from_iid,
										    parent_cppat
											    ->nfp6000
											    .cpp_tgt)) {
										*cppat =
											*parent_cppat;
										cppat++;
									}
								}
								decl->cppats.count = ncnt;
							}
							done = done && 1;
						}
						break;
					default:
						NS_LOG(NS_LOG_LVL_FATAL,
						       "Internal error with chip '%s'", decl->id);
						goto error_cleanup;
					}

					if (done) {
						free(decl->inherit);
						decl->inherit = NULL;
						done = 0; // stole 'done' for a moment
					}
					decl->parent_chip = decl_parent;
				}
			} else {
				switch (decl->arch) {
				case NFP_CHIP_ARCH_YD:
					break;
				case NFP_CHIP_ARCH_TH:
					// If this decl doesn't inherit and it didn't
					// have a serdes setting in json data, set it
					// to zero.
					if (decl->details.nfp6000.serdes_cnt == SIZE_MAX) {
						decl->details.nfp6000.serdes_cnt = 0;
						done = 0;
					}
					break;
				default:
					break;
				}

				if (decl->cfgs.count) {
					err = nsc_chip_desc_dup(decl);
					if (err)
						goto error_cleanup;
					err = nsc_chip_apply_cfg(decl);
					if (err)
						goto error_cleanup;
				}
			}
		}
	} // End of inheritance

	// Do some sorting

	for (node = data->chip_decls.node; (!!node); node = node->next) {
		struct nfp_chipdata_chip *decl = (struct nfp_chipdata_chip *)node;

		switch (decl->arch) {
		case NFP_CHIP_ARCH_TH:
			qsort(decl->cppats.entry, decl->cppats.count, sizeof(*decl->cppats.entry),
			      nsc_comp_nfp6000_cppat_asc);
			break;
		default:
			break;
		}
	}

	// Verify chip definitions.
	for (node = data->chip_decls.node; (!!node); node = node->next) {
		struct nfp_chipdata_chip *decl = (struct nfp_chipdata_chip *)node;
		size_t idx1, idx2;
		int check;

		switch (decl->arch) {
		case NFP_CHIP_ARCH_YD:
			for (idx1 = 0; idx1 < decl->details.nfp3200.cpp_targets.count; idx1++) {
				check = 0;
				for (idx2 = 0; idx2 < decl->chip_desc.nfp3200->cpp_targets.count;
				     idx2++) {
					struct nsc_desc_cpptgt_nfpxxxx *cpptgt =
						&decl->chip_desc.nfp3200->cpp_targets.entry[idx2];
					if (cpptgt->tgt_id ==
					    decl->details.nfp3200.cpp_targets.entry[idx1]) {
						check = 1;
						break;
					}
				}

				if (!check) {
					NS_LOG(NS_LOG_LVL_FATAL,
					       "Chip '%s' definition uses invalid CPP target %d",
					       decl->id,
					       decl->details.nfp3200.cpp_targets.entry[idx1]);
					goto error_cleanup;
				}
			}
			break;
		case NFP_CHIP_ARCH_TH:
			decl->details.nfp6000.max_me_cnt = 0;
			for (idx1 = 0; idx1 < decl->details.nfp6000.islands.count; idx1++) {
				int iid = decl->details.nfp6000.islands.entry[idx1];

				check = 0;
				for (idx2 = 0; idx2 < decl->chip_desc.nfp6000->islands.count;
				     idx2++) {
					struct nsc_desc_island_nfp6000 *island =
						&decl->chip_desc.nfp6000->islands.entry[idx2];
					if (island->id == iid) {
						check = 1;
						if (island->me_cnt >
						    decl->details.nfp6000.max_me_cnt)
							decl->details.nfp6000.max_me_cnt =
								island->me_cnt;
						break;
					}
				}

				if (!check) {
					NS_LOG(NS_LOG_LVL_FATAL,
					       "Chip '%s' definition uses invalid island ID %d",
					       decl->id, iid);
					goto error_cleanup;
				}
			}
			break;
		default:
			NS_LOG(NS_LOG_LVL_FATAL, "Internal error with chip '%s'", decl->id);
			goto error_cleanup;
		}
	}

	NS_LOG(NS_LOG_LVL_EXTRA, "Data extracted.");

	if (!!embedded_json)
		json_decref(embedded_json);

	return data;

error_cleanup:

	if (!!data)
		nfp_chipdata_free(data);

	if (!!embedded_json)
		json_decref(embedded_json);

	if (err)
		errno = err;
	else
		errno = EINVAL;
	return NULL;
}

#else /* !defined(EMBEDDED) */
struct nfp_chipdata *nfp_chipdata_load(const char *include_paths, const char *chipdata_path)
{
	return NULL;
}
#endif /* !defined(EMBEDDED) */

const char *nfp_chipdata_comment(const struct nfp_chipdata *data)
{
	return data->json_comment;
}

const struct nfp_chipdata_chip *nfp_chipdata_select(const struct nfp_chipdata *data,
						    const char *chip_id)
{
	struct nsc_llist_node *node;

	if (!data)
		return NULL;

	if (!chip_id) {
		chip_id = getenv(NFP_CHIPDATA_CHIP_ID_ENVVAR);
		if (!chip_id)
			return NULL;
	}

	for (node = data->chip_decls.node; (!!node); node = node->next) {
		struct nfp_chipdata_chip *ent = (struct nfp_chipdata_chip *)node;

		if (strcasecmp(ent->id, chip_id) == 0)
			return ent;
	}

	return NULL;
}

int nfp_chipdata_on_board(const struct nfp_chipdata_chip *chip)
{
	if (!chip) {
		NS_LOG(NS_LOG_LVL_ERROR, "Invalid parameters.");
		errno = EINVAL;
		return 0;
	}

	return chip->is_board;
}

const char *nfp_chipdata_board_chip_id(const struct nfp_chipdata_chip *chip)
{
	if (!chip) {
		NS_LOG(NS_LOG_LVL_ERROR, "Invalid parameters.");
		errno = EINVAL;
		return "";
	}

	return (chip->parent_chip) ? chip->parent_chip->id : "";
}

struct nfp_chipdata_chip *nfp_chipdata_chip_new(struct nfp_chipdata *data,
						int chip_family,
						int chip_rev,
						const char *new_id,
						const char *inherit_chip)
{
	int err = EINVAL;
	const struct nfp_chipdata_chip *inherit = NULL;
	struct nfp_chipdata_chip *chip = NULL;
	struct nsc_llist_node *node;

	if ((!data) || (!new_id)) {
		NS_LOG(NS_LOG_LVL_FATAL, "Invalid parameters.");
		goto error_cleanup;
	}

	switch (chip_family) {
	case NFP_CHIP_FAMILY_NFP3200:
	case NFP_CHIP_FAMILY_NFP3800:
	case NFP_CHIP_FAMILY_NFP6000:
	case NFP_CHIP_FAMILY_NFP5000:
	case NFP_CHIP_FAMILY_NFP4000:
		break;
	default:
		NS_LOG(NS_LOG_LVL_FATAL, "Invalid chip family.");
		goto error_cleanup;
	}

	if (!!inherit_chip) {
		for (node = data->chip_decls.node; (!!node); node = node->next) {
			struct nfp_chipdata_chip *ent = (struct nfp_chipdata_chip *)node;

			if (strcasecmp(ent->id, inherit_chip) == 0) {
				inherit = ent;
				break;
			}
		}

		if (!inherit) {
			NS_LOG(NS_LOG_LVL_FATAL, "Chip '%s' not found", inherit_chip);
			err = ENODEV;
			goto error_cleanup;
		}

		if (inherit->family != chip_family) {
			NS_LOG(NS_LOG_LVL_FATAL,
			       "Chip '%s' has different family from new chip",
			       inherit_chip);
			err = EINVAL;
			goto error_cleanup;
		}

		if (chip_rev < 0) {
			chip_rev = inherit->rev_max;
		} else if (!nfp_chipdata_has_revision(inherit, chip_rev)) {
			NS_LOG(NS_LOG_LVL_FATAL,
			       "Chip '%s' has does not have required revision",
			       inherit_chip);
			err = EINVAL;
			goto error_cleanup;
		}
	}

	_CALLOC_OOM(chip, 1, sizeof(struct nfp_chipdata_chip));
	chip->id = strdup(new_id);
	chip->family = chip_family;
	chip->arch = _NFP_ARCH(chip_family);
	chip->rev_min = chip_rev;
	chip->rev_max = chip_rev;
	chip->chipdata = data;
	if (nfp_chipdata_revision_add(chip, chip_rev) != 0) {
		err = errno;
		goto error_cleanup;
	}

	if (!!inherit) {
		chip->chip_desc.vp = inherit->chip_desc.vp;
		chip->description = safe_strdup(inherit->description);

		if (inherit->cppats.count > 0)
			_ARRAY_DUP(&chip->cppats, &inherit->cppats);

		switch (_NFP_ARCH(chip_family)) {
		case NFP_CHIP_ARCH_YD:
			chip->details.nfp3200.me_cnt = inherit->details.nfp3200.me_cnt;
			_ARRAY_DUP(&chip->details.nfp3200.cpp_targets,
				   &inherit->details.nfp3200.cpp_targets);
			break;
		case NFP_CHIP_ARCH_TH:
			chip->details.nfp6000.max_me_cnt = inherit->details.nfp6000.max_me_cnt;
			chip->details.nfp6000.serdes_cnt = inherit->details.nfp6000.serdes_cnt;
			_ARRAY_DUP(&chip->details.nfp6000.islands,
				   &inherit->details.nfp6000.islands);
			break;
		default:
			break;
		}
	} else {
		chip->chip_desc.vp = &(
			nsc_chip_desc_find_by_family(data, chip_family, chip_rev, chip_rev)->desc);
	}

	return chip;

error_cleanup:
	nfp_chipdata_chip_free(chip);
	return NFP_ERRPTR(err);
}

int nfp_chipdata_island_add(struct nfp_chipdata_chip *chip, int island_id)
{
	int err = EINVAL;
	size_t idx;
	struct nsc_desc_island_nfp6000 *island = NULL;

	if (!chip) {
		NS_LOG(NS_LOG_LVL_ERROR, "Invalid parameters.");
		return NFP_ERRNO(EINVAL);
	}

	switch (chip->arch) {
	case NFP_CHIP_ARCH_TH:
#define _NSC_ISLAS chip->chip_desc.nfp6000->islands
		for (idx = 0; idx < _NSC_ISLAS.count; idx++) {
			island = &_NSC_ISLAS.entry[idx];
			if (island->id == island_id)
				break;
		}

		if (idx == _NSC_ISLAS.count) {
			NS_LOG(NS_LOG_LVL_ERROR, "Invalid island ID (%d) for chip family.",
			       island_id);
			return NFP_ERRNO(ENODEV);
		}

		for (idx = 0; idx < chip->details.nfp6000.islands.count; idx++)
			if (chip->details.nfp6000.islands.entry[idx] == island_id)
				return 0;

#undef _NSC_ISLAS
		if (chip->details.nfp6000.islands.capacity == chip->details.nfp6000.islands.count)
			_ARRAY_EXTEND(&chip->details.nfp6000.islands, 64);
		chip->details.nfp6000.islands.entry[chip->details.nfp6000.islands.count++] =
			island_id;
		qsort(chip->details.nfp6000.islands.entry, chip->details.nfp6000.islands.count,
		      sizeof(*chip->details.nfp6000.islands.entry), nsc_comp_int_asc);

		if (island->me_cnt > chip->details.nfp6000.max_me_cnt)
			chip->details.nfp6000.max_me_cnt = island->me_cnt;
		break;
	default:
		NS_LOG(NS_LOG_LVL_ERROR, "Invalid chip family.");
		return NFP_ERRNO(EINVAL);
	}

	return 0;

error_cleanup:
	return NFP_ERRNO(err);
}

int nfp_chipdata_island_rem(struct nfp_chipdata_chip *chip, int island_id)
{
	size_t idx;

	if (!chip) {
		NS_LOG(NS_LOG_LVL_ERROR, "Invalid parameters.");
		return NFP_ERRNO(EINVAL);
	}

	switch (chip->arch) {
	case NFP_CHIP_ARCH_TH:
		for (idx = 0; idx < chip->details.nfp6000.islands.count; idx++) {
			if (chip->details.nfp6000.islands.entry[idx] == island_id)
				break;
		}

		if (idx == chip->details.nfp6000.islands.count)
			return 0;

		for (idx++; idx < chip->details.nfp6000.islands.count; idx++) {
			chip->details.nfp6000.islands.entry[idx - 1] =
				chip->details.nfp6000.islands.entry[idx];
		}

		chip->details.nfp6000.islands.count--;
		qsort(chip->details.nfp6000.islands.entry, chip->details.nfp6000.islands.count,
		      sizeof(*chip->details.nfp6000.islands.entry), nsc_comp_int_asc);
		break;
	default:
		NS_LOG(NS_LOG_LVL_ERROR, "Invalid chip family.");
		return NFP_ERRNO(EINVAL);
	}

	return 0;
}

int nfp_chipdata_revision_add(struct nfp_chipdata_chip *chip, int revision)
{
	size_t idx;
	int err = EINVAL;

	if (!chip) {
		NS_LOG(NS_LOG_LVL_ERROR, "Invalid parameters.");
		return NFP_ERRNO(EINVAL);
	}

	for (idx = 0; idx < chip->revisions.count; idx++)
		if (chip->revisions.entry[idx] == revision)
			return 0;

	if (chip->revisions.capacity == chip->revisions.count) {
		_ARRAY_EXTEND(&chip->revisions, 8);
		_ARRAY_EXTEND(&chip->revision_models, 8);
	}
	chip->revisions.entry[chip->revisions.count++] = revision;
	qsort(chip->revisions.entry, chip->revisions.count, sizeof(*chip->revisions.entry),
	      nsc_comp_int_asc);

	return 0;

error_cleanup:
	return NFP_ERRNO(err);
}

int nfp_chipdata_revision_rem(struct nfp_chipdata_chip *chip, int revision)
{
	size_t idx;

	if (!chip) {
		NS_LOG(NS_LOG_LVL_ERROR, "Invalid parameters.");
		return NFP_ERRNO(EINVAL);
	}

	for (idx = 0; idx < chip->revisions.count; idx++) {
		if (chip->revisions.entry[idx] == revision)
			break;
	}

	if (idx == chip->revisions.count)
		return 0;

	for (idx++; idx < chip->revisions.count; idx++)
		chip->revisions.entry[idx - 1] = chip->revisions.entry[idx];

	chip->revisions.count--;

	return 0;
}

int nfp_chipdata_model_set(struct nfp_chipdata_chip *chip, int revision, uint32_t model)
{
	size_t idx;

	if (!chip) {
		NS_LOG(NS_LOG_LVL_ERROR, "Invalid parameters.");
		return NFP_ERRNO(EINVAL);
	}

	for (idx = 0; idx < chip->revisions.count; idx++) {
		if (chip->revisions.entry[idx] == revision) {
			chip->revision_models.entry[idx] = model;
			return 0;
		}
	}

	NS_LOG(NS_LOG_LVL_ERROR, "Invalid parameters.");
	return NFP_ERRNO(EINVAL);
}

int nfp_chipdata_mecnt_set(struct nfp_chipdata_chip *chip, size_t me_cnt)
{
	if (!chip) {
		NS_LOG(NS_LOG_LVL_ERROR, "Invalid parameters.");
		return NFP_ERRNO(EINVAL);
	}

	switch (chip->arch) {
	case NFP_CHIP_ARCH_YD:
		if (me_cnt > NSC_NFP3200_NUM_MES) {
			NS_LOG(NS_LOG_LVL_ERROR,
			       "Maximum microengine count for NFP-32xx is %d.",
			       NSC_NFP3200_NUM_MES);
			return NFP_ERRNO(ERANGE);
		}

		if (me_cnt % NSC_NFP3200_ME_PER_CLUSTER) {
			NS_LOG(NS_LOG_LVL_ERROR,
			       "Microengine count for NFP-32xx must be a multiple of %d.",
			       NSC_NFP3200_ME_PER_CLUSTER);
			return NFP_ERRNO(EINVAL);
		}

		chip->details.nfp3200.me_cnt = me_cnt;
		break;
	default:
		NS_LOG(NS_LOG_LVL_ERROR, "Invalid chip family.");
		return NFP_ERRNO(EINVAL);
	}

	return 0;
}

int nfp_chipdata_cpptgt_add(struct nfp_chipdata_chip *chip, int cpp_tgt)
{
	int err = EINVAL;
	size_t idx;

	if (!chip) {
		NS_LOG(NS_LOG_LVL_ERROR, "Invalid parameters.");
		return NFP_ERRNO(EINVAL);
	}

	switch (chip->arch) {
	case NFP_CHIP_ARCH_YD:
#define _NSC_CTS chip->chip_desc.nfp3200->cpp_targets
		for (idx = 0; idx < _NSC_CTS.count; idx++) {
			struct nsc_desc_cpptgt_nfpxxxx *ctgt = &_NSC_CTS.entry[idx];

			if (ctgt->tgt_id == cpp_tgt)
				break;
		}

		if (idx == _NSC_CTS.count) {
			NS_LOG(NS_LOG_LVL_ERROR,
			       "Invalid CPP Target ID (%d) for chip family.",
			       cpp_tgt);
			return NFP_ERRNO(ENODEV);
		}

		for (idx = 0; idx < chip->details.nfp3200.cpp_targets.count; idx++)
			if (chip->details.nfp3200.cpp_targets.entry[idx] == cpp_tgt)
				return 0;

#undef _NSC_CTS
		if (chip->details.nfp3200.cpp_targets.capacity ==
		    chip->details.nfp3200.cpp_targets.count)
			_ARRAY_EXTEND(&chip->details.nfp3200.cpp_targets, 16);
		chip->details.nfp3200.cpp_targets.entry[chip->details.nfp3200.cpp_targets.count++] =
			cpp_tgt;
		qsort(chip->details.nfp3200.cpp_targets.entry,
		      chip->details.nfp3200.cpp_targets.count,
		      sizeof(*chip->details.nfp3200.cpp_targets.entry), nsc_comp_int_asc);
		break;
	default:
		NS_LOG(NS_LOG_LVL_ERROR, "Invalid chip family.");
		return NFP_ERRNO(EINVAL);
	}

	return 0;

error_cleanup:
	return NFP_ERRNO(err);
}

int nfp_chipdata_cpptgt_rem(struct nfp_chipdata_chip *chip, int cpp_tgt)
{
	size_t idx;

	if (!chip) {
		NS_LOG(NS_LOG_LVL_ERROR, "Invalid parameters.");
		return NFP_ERRNO(EINVAL);
	}

	switch (chip->arch) {
	case NFP_CHIP_ARCH_YD:
		for (idx = 0; idx < chip->details.nfp3200.cpp_targets.count; idx++) {
			if (chip->details.nfp3200.cpp_targets.entry[idx] == cpp_tgt)
				break;
		}

		if (idx == chip->details.nfp3200.cpp_targets.count)
			return 0;

		for (idx++; idx < chip->details.nfp3200.cpp_targets.count; idx++) {
			chip->details.nfp3200.cpp_targets.entry[idx - 1] =
				chip->details.nfp3200.cpp_targets.entry[idx];
		}

		chip->details.nfp3200.cpp_targets.count--;
		break;
	default:
		NS_LOG(NS_LOG_LVL_ERROR, "Invalid chip family.");
		return NFP_ERRNO(EINVAL);
	}

	return 0;
}

const char *nfp_chipdata_cpptgt2str(const struct nfp_chipdata_chip *chip, int cpp_tgt)
{
	size_t idx;
	const struct nsc_desc_cpptgt_nfpxxxx *cpptgt = NULL;
	size_t cpptgt_sz = 0;
	struct nfp_chipdata *data = NULL;

	if (!chip) {
		NS_LOG(NS_LOG_LVL_ERROR, "Invalid parameters.");
		return NFP_ERRPTR(EINVAL);
	}

	data = chip->chipdata;

	switch (chip->arch) {
	case NFP_CHIP_ARCH_YD:
		cpptgt_sz = chip->chip_desc.nfp3200->cpp_targets.count;
		cpptgt = chip->chip_desc.nfp3200->cpp_targets.entry;
		break;
	case NFP_CHIP_ARCH_TH:
		cpptgt_sz = chip->chip_desc.nfp6000->cpp_targets.count;
		cpptgt = chip->chip_desc.nfp6000->cpp_targets.entry;
		break;
	default:
		NS_LOG(NS_LOG_LVL_ERROR, "Invalid chip family.");
		return NFP_ERRPTR(EINVAL);
	}

	for (idx = 0; idx < cpptgt_sz; idx++, cpptgt++) {
		if (cpptgt->tgt_id == cpp_tgt)
			return NSC_STR(cpptgt->name);
	}

	return NFP_ERRPTR(ENOENT);
}

const struct nfp_chipdata *nfp_chipdata_from_chip(const struct nfp_chipdata_chip *chip)
{
	if (!chip)
		return NULL;
	return chip->chipdata;
}

const struct nfp_chipdata_chip *nfp_chipdata_chip_first(const struct nfp_chipdata *data)
{
	if (!data)
		return NULL;
	return (const struct nfp_chipdata_chip *)data->chip_decls.node;
}

const struct nfp_chipdata_chip *nfp_chipdata_chip_next(const struct nfp_chipdata *data,
						       const struct nfp_chipdata_chip *chip)
{
	if ((!data) || (!chip))
		return NULL;

	return (const struct nfp_chipdata_chip *)chip->node.next;
}

int nfp_chipdata_chip_arch(const struct nfp_chipdata_chip *chip)
{
	if (!chip) {
		NS_LOG(NS_LOG_LVL_ERROR, "Invalid parameters.");
		return 0;
	}

	return chip->arch;
}

int nfp_chipdata_chip_family(const struct nfp_chipdata_chip *chip)
{
	if (!chip) {
		NS_LOG(NS_LOG_LVL_ERROR, "Invalid parameters.");
		return 0;
	}

	if (chip->family == NFP_CHIP_FAMILY_NFP4000 ||
	    chip->family == NFP_CHIP_FAMILY_NFP5000)
		return NFP_CHIP_FAMILY_NFP6000;
	return chip->family;
}

const char *nfp_chipdata_chip_file(const struct nfp_chipdata_chip *chip)
{
	struct nfp_chipdata *data = NULL;

	if (!chip)
		return NULL;
	data = chip->chipdata;
	return NSC_STR(chip->source_file);
}

const char *nfp_chipdata_chip_id(const struct nfp_chipdata_chip *chip)
{
	if (!chip)
		return NULL;
	return chip->id;
}

const char *nfp_chipdata_chip_description(const struct nfp_chipdata_chip *chip)
{
	if (!chip)
		return NULL;
	return chip->description;
}

size_t nfp_chipdata_chip_revisions(const struct nfp_chipdata_chip *chip, const int **revisions)
{
	if (!chip)
		return 0;
	if (!!revisions)
		*revisions = chip->revisions.entry;
	return chip->revisions.count;
}

int nfp_chipdata_has_revision(const struct nfp_chipdata_chip *chip, int revision)
{
	size_t i;

	if (!chip)
		return 0;
	for (i = 0; i < chip->revisions.count; i++)
		if (chip->revisions.entry[i] == revision)
			return 1;
	return 0;
}

int nfp_chipdata_revision_min(const struct nfp_chipdata_chip *chip)
{
	if ((!chip) || (chip->revisions.count == 0))
		return -1;
	return chip->revisions.entry[0];
}

int nfp_chipdata_revision_max(const struct nfp_chipdata_chip *chip)
{
	if ((!chip) || (chip->revisions.count == 0))
		return -1;
	return chip->revisions.entry[chip->revisions.count - 1];
}

int nfp_chipdata_me_version(const struct nfp_chipdata_chip *chip)
{
	if (!chip) {
		NS_LOG(NS_LOG_LVL_ERROR, "Invalid parameters.");
		return -1;
	}

	switch (chip->family) {
	case NFP_CHIP_FAMILY_NFP3200:
		return NFP_CHIP_ME_VERSION_2_7;
	case NFP_CHIP_FAMILY_NFP6000:
		if (nfp_chipdata_revision_max(chip) < NFP_CHIP_REVISION_C0)
			return NFP_CHIP_ME_VERSION_2_8;
		if (nfp_chipdata_revision_min(chip) >= NFP_CHIP_REVISION_C0)
			return NFP_CHIP_ME_VERSION_2_9;
		break;
	case NFP_CHIP_FAMILY_NFP4000:
	case NFP_CHIP_FAMILY_NFP5000:
		return NFP_CHIP_ME_VERSION_2_8;
	case NFP_CHIP_FAMILY_NFP3800:
		return NFP_CHIP_ME_VERSION_2_9;
	default:
		break;
	}

	NS_LOG(NS_LOG_LVL_ERROR, "Unsupported chip family and revision.");
	return -1;
}

uint32_t nfp_chipdata_model(const struct nfp_chipdata_chip *chip, int revision)
{
	size_t i;

	if (!chip)
		return NFP_CHIPDATA_CPP_MODEL_INVALID;

	if ((chip->revisions.count == 1) && (revision < 0))
		return chip->revision_models.entry[0];

	for (i = 0; i < chip->revisions.count; i++)
		if (chip->revisions.entry[i] == revision)
			return chip->revision_models.entry[i];

	return NFP_CHIPDATA_CPP_MODEL_INVALID;
}

int nfp_chipdata_has_island(const struct nfp_chipdata_chip *chip, int island_id)
{
	size_t idx;

	if ((!chip) || (island_id < 0)) {
		NS_LOG(NS_LOG_LVL_ERROR, "Invalid parameters.");
		return 0;
	}

	switch (chip->arch) {
	case NFP_CHIP_ARCH_YD:
		return (island_id <
			(int)(chip->details.nfp3200.me_cnt / NSC_NFP3200_ME_PER_CLUSTER));
	case NFP_CHIP_ARCH_TH:
		for (idx = 0; idx < chip->details.nfp6000.islands.count; idx++) {
			if (island_id == chip->details.nfp6000.islands.entry[idx])
				return 1;
		}
		break;
	default:
		break;
	}

	return 0;
}

size_t nfp_chipdata_island_ids(const struct nfp_chipdata_chip *chip, const int **island_ids)
{
	if (!chip)
		return 0;

	switch (chip->arch) {
	case NFP_CHIP_ARCH_TH:
		if (!!island_ids)
			*island_ids = chip->details.nfp6000.islands.entry;
		return chip->details.nfp6000.islands.count;
	default:
		break;
	}

	return 0;
}

int nfp_chipdata_island_id_first(const struct nfp_chipdata_chip *chip)
{
	if (!chip) {
		NS_LOG(NS_LOG_LVL_ERROR, "Invalid parameters.");
		return -1;
	}

	switch (chip->arch) {
	case NFP_CHIP_ARCH_TH:
		if (chip->details.nfp6000.islands.count > 0)
			return chip->details.nfp6000.islands.entry[0];
		break;
	case NFP_CHIP_ARCH_YD:
		return 0;
	default:
		break;
	}

	return -1;
}

int nfp_chipdata_island_id_last(const struct nfp_chipdata_chip *chip)
{
	if (!chip) {
		NS_LOG(NS_LOG_LVL_ERROR, "Invalid parameters.");
		return -1;
	}

	switch (chip->arch) {
	case NFP_CHIP_ARCH_TH:
		if (chip->details.nfp6000.islands.count > 0)
			return chip->details.nfp6000.islands
				.entry[chip->details.nfp6000.islands.count - 1];
		break;
	case NFP_CHIP_ARCH_YD: {
		size_t cl_cnt = (chip->details.nfp3200.me_cnt / NSC_NFP3200_ME_PER_CLUSTER);

		if (cl_cnt == 0)
			return -1;

		return cl_cnt - 1;
	}
	default:
		break;
	}

	return -1;
}

int nfp_chipdata_island_id_next(const struct nfp_chipdata_chip *chip, int island_id)
{
	size_t idx;

	if ((!chip) || (island_id < 0)) {
		NS_LOG(NS_LOG_LVL_ERROR, "Invalid parameters.");
		return -1;
	}

	switch (chip->arch) {
	case NFP_CHIP_ARCH_TH:
		// This works because we sort the loaded island list.
		for (idx = 0; idx < chip->details.nfp6000.islands.count; idx++)
			if (chip->details.nfp6000.islands.entry[idx] == island_id)
				break;
		idx++;
		if (idx < chip->details.nfp6000.islands.count)
			return chip->details.nfp6000.islands.entry[idx];
		break;
	case NFP_CHIP_ARCH_YD:
		if (++island_id < chip->chip_desc.nfp3200->cluster_count)
			return island_id;
		break;
	default:
		break;
	}

	return -1;
}

int nfp_chipdata_island_id_prev(const struct nfp_chipdata_chip *chip, int island_id)
{
	size_t idx;

	if ((!chip) || (island_id < 0)) {
		NS_LOG(NS_LOG_LVL_ERROR, "Invalid parameters.");
		return -1;
	}

	switch (chip->arch) {
	case NFP_CHIP_ARCH_TH:
		// This works because we sort the loaded island list.
		for (idx = 0; idx < chip->details.nfp6000.islands.count; idx++)
			if (chip->details.nfp6000.islands.entry[idx] == island_id)
				break;
		if (idx == 0)
			return -1;
		idx--;
		return chip->details.nfp6000.islands.entry[idx];
	case NFP_CHIP_ARCH_YD:
		if (--island_id >= 0)
			return island_id;
		break;
	default:
		break;
	}

	return -1;
}

int nfp_chipdata_island_id_type_first(const struct nfp_chipdata_chip *chip,
				      enum nfp_chipdata_isld_type island_type)
{
	int island_id;

	island_id = nfp_chipdata_island_id_first(chip);

	if ((island_id == -1) || (island_type == NFP_CHIPDATA_ISLD_TYPE_NONE) ||
	    (chip->arch == NFP_CHIP_ARCH_YD))
		return island_id;

	for (island_id = nfp_chipdata_island_id_first(chip); island_id != -1;
	     island_id = nfp_chipdata_island_id_next(chip, island_id)) {
		if (nfp_chipdata_island_type(chip, island_id) == island_type)
			return island_id;
	}

	return -1;
}

int nfp_chipdata_island_id_type_next(const struct nfp_chipdata_chip *chip,
				     int island_id,
				     enum nfp_chipdata_isld_type island_type)
{
	if ((island_type == NFP_CHIPDATA_ISLD_TYPE_NONE) ||
	    (chip->arch == NFP_CHIP_ARCH_YD))
		return nfp_chipdata_island_id_next(chip, island_id);

	for (island_id = nfp_chipdata_island_id_next(chip, island_id); island_id != -1;
	     island_id = nfp_chipdata_island_id_next(chip, island_id)) {
		if (nfp_chipdata_island_type(chip, island_id) == island_type)
			return island_id;
	}

	return -1;
}

int nfp_chipdata_island_id_type_prev(const struct nfp_chipdata_chip *chip,
				     int island_id,
				     enum nfp_chipdata_isld_type island_type)
{
	if ((island_type == NFP_CHIPDATA_ISLD_TYPE_NONE) ||
	    (chip->arch == NFP_CHIP_ARCH_YD))
		return nfp_chipdata_island_id_prev(chip, island_id);

	for (island_id = nfp_chipdata_island_id_prev(chip, island_id); island_id != -1;
	     island_id = nfp_chipdata_island_id_prev(chip, island_id)) {
		if (nfp_chipdata_island_type(chip, island_id) == island_type)
			return island_id;
	}

	return -1;
}

size_t nfp_chipdata_island_count(const struct nfp_chipdata_chip *chip,
				 enum nfp_chipdata_isld_type island_type)
{
	size_t idx_iid = 0;
	size_t idx = 0;
	size_t cnt = 0;

	if (!chip)
		return 0;

	switch (chip->arch) {
	case NFP_CHIP_ARCH_YD:
		return (chip->details.nfp3200.me_cnt / NSC_NFP3200_ME_PER_CLUSTER);

	case NFP_CHIP_ARCH_TH:
		if (island_type == NFP_CHIPDATA_ISLD_TYPE_NONE)
			return chip->details.nfp6000.islands.count;

		for (idx_iid = 0; idx_iid < chip->details.nfp6000.islands.count; idx_iid++) {
#define _NSC_ISLAS chip->chip_desc.nfp6000->islands
			int iid = chip->details.nfp6000.islands.entry[idx_iid];

			for (idx = 0; idx < _NSC_ISLAS.count; idx++) {
				struct nsc_desc_island_nfp6000 *island = &_NSC_ISLAS.entry[idx];

				if ((island->id == iid) && (island->island_type == island_type))
					cnt++;
			}
#undef _NSC_ISLAS
		}

		break;
	default:
		return 0;
	}

	return cnt;
}

int nfp_chipdata_has_crypto(const struct nfp_chipdata_chip *chip)
{
	if (!chip) {
		NS_LOG(NS_LOG_LVL_ERROR, "Invalid parameters.");
		return 0;
	}

	switch (chip->arch) {
	case NFP_CHIP_ARCH_YD:
		return nfp_chipdata_has_cpp_target(chip, NFP3200_CPPTGT_CRYPTO);
	case NFP_CHIP_ARCH_TH:
		return nfp_chipdata_has_cpp_target(chip, NFP6000_CPPTGT_CRYPTO);
	default:
		break;
	}

	return 0;
}

const char *nfp_chipdata_island_name(const struct nfp_chipdata_chip *chip, int island_id)
{
	struct nfp_chipdata *data = NULL;

	if (!chip) {
		NS_LOG(NS_LOG_LVL_ERROR, "Invalid parameters.");
		return 0;
	}

	data = chip->chipdata;

	switch (chip->arch) {
	case NFP_CHIP_ARCH_TH: {
		const struct nsc_desc_island_nfp6000 *island =
			nsc_nfp6000_island_find_by_iid(chip, island_id);
		if (!island)
			return NULL;
		return NSC_STR(island->name);
	}
	default:
		break;
	}

	return NULL;
}

enum nfp_chipdata_isld_type nfp_chipdata_island_type(const struct nfp_chipdata_chip *chip,
						     int island_id)
{
	if (!chip) {
		NS_LOG(NS_LOG_LVL_ERROR, "Invalid parameters.");
		return NFP_CHIPDATA_ISLD_TYPE_NONE;
	}

	switch (chip->arch) {
	case NFP_CHIP_ARCH_TH: {
		const struct nsc_desc_island_nfp6000 *island =
			nsc_nfp6000_island_find_by_iid(chip, island_id);
		if (!island)
			return NFP_CHIPDATA_ISLD_TYPE_NONE;
		return island->island_type;
	}
	default:
		break;
	}

	return NFP_CHIPDATA_ISLD_TYPE_NONE;
}

size_t nfp_chipdata_island_cpp_targets(const struct nfp_chipdata_chip *chip,
				       const int **cpp_targets,
				       int island_id)
{
	if (!chip) {
		NS_LOG(NS_LOG_LVL_ERROR, "Invalid parameters.");
		return 0;
	}

	switch (chip->arch) {
	case NFP_CHIP_ARCH_TH: {
		const struct nsc_desc_island_nfp6000 *island =
			nsc_nfp6000_island_find_by_iid(chip, island_id);
		if (!island)
			return 0;
		if (!!cpp_targets)
			*cpp_targets = island->cpp_tgt_ids;
		return island->cpp_tgt_id_count;
	}
	default:
		break;
	}

	return 0;
}

int nfp_chipdata_has_cpp_target(const struct nfp_chipdata_chip *chip, int cpp_tgt_id)
{
	size_t idx_cpp;
	int iid;

	if (!chip) {
		NS_LOG(NS_LOG_LVL_ERROR, "Invalid parameters.");
		return 0;
	}

	switch (chip->arch) {
	case NFP_CHIP_ARCH_YD:
		for (idx_cpp = 0; idx_cpp < chip->details.nfp3200.cpp_targets.count; idx_cpp++) {
			if (chip->details.nfp3200.cpp_targets.entry[idx_cpp] == cpp_tgt_id)
				return 1;
		}
		break;
	case NFP_CHIP_ARCH_TH: {
		for (iid = nfp_chipdata_island_id_first(chip); iid != -1;
		     iid = nfp_chipdata_island_id_next(chip, iid)) {
			const struct nsc_desc_island_nfp6000 *island =
				nsc_nfp6000_island_find_by_iid(chip, iid);
			if (!island)
				continue;
			for (idx_cpp = 0; idx_cpp < island->cpp_tgt_id_count; idx_cpp++) {
				if (island->cpp_tgt_ids[idx_cpp] == cpp_tgt_id)
					return 1;
			}
		}
		break;
	}
	default:
		break;
	}

	return 0;
}

int nfp_chipdata_island_has_cpp_target(const struct nfp_chipdata_chip *chip,
				       int island_id,
				       int cpp_tgt_id)
{
	size_t idx;

	if (!chip) {
		NS_LOG(NS_LOG_LVL_ERROR, "Invalid parameters.");
		return 0;
	}

	switch (chip->arch) {
	case NFP_CHIP_ARCH_TH: {
		const struct nsc_desc_island_nfp6000 *island =
			nsc_nfp6000_island_find_by_iid(chip, island_id);
		if (!island)
			return 0;
		for (idx = 0; idx < island->cpp_tgt_id_count; idx++) {
			if (island->cpp_tgt_ids[idx] == cpp_tgt_id)
				return 1;
		}
		break;
	}
	default:
		break;
	}

	return 0;
}

size_t nfp_chipdata_me_count(const struct nfp_chipdata_chip *chip)
{
	size_t me_cnt = 0;
	int iid;

	if (!chip)
		return 0;

	switch (chip->arch) {
	case NFP_CHIP_ARCH_YD:
		return chip->details.nfp3200.me_cnt;
	case NFP_CHIP_ARCH_TH:
		for (iid = nfp_chipdata_island_id_first(chip); iid != -1;
		     iid = nfp_chipdata_island_id_next(chip, iid)) {
			me_cnt += nfp_chipdata_island_me_count(chip, iid);
		}
		return me_cnt;
	default:
		break;
	}

	return 0;
}

size_t nfp_chipdata_serdes_count(const struct nfp_chipdata_chip *chip)
{
	if (!chip)
		return 0;

	switch (chip->arch) {
	case NFP_CHIP_ARCH_YD:
		return 0;
	case NFP_CHIP_ARCH_TH:
		return chip->details.nfp6000.serdes_cnt;
	default:
		break;
	}

	return 0;
}

size_t nfp_chipdata_island_me_count(const struct nfp_chipdata_chip *chip, int island_id)
{
	if (!chip)
		return 0;

	switch (chip->arch) {
	case NFP_CHIP_ARCH_YD:
		return NSC_NFP3200_ME_PER_CLUSTER;
	case NFP_CHIP_ARCH_TH: {
		const struct nsc_desc_island_nfp6000 *island = NULL;

		if (island_id == 0)
			return chip->details.nfp6000.max_me_cnt;

		island = nsc_nfp6000_island_find_by_iid(chip, island_id);
		if (!island)
			return 0;
		return island->me_cnt;
	}
	default:
		break;
	}

	return 0;
}

int nfp_chipdata_meid_first(const struct nfp_chipdata_chip *chip)
{
	int i;

	if (!chip) {
		NS_LOG(NS_LOG_LVL_ERROR, "Invalid parameters.");
		return -1;
	}

	switch (chip->arch) {
	case NFP_CHIP_ARCH_YD:
		if (chip->details.nfp3200.me_cnt == 0)
			return -1;
		return NFP3200_MEID(0, 0);
	case NFP_CHIP_ARCH_TH:
		for (i = nfp_chipdata_island_id_first(chip); i != -1;
		     i = nfp_chipdata_island_id_next(chip, i)) {
			if (nfp_chipdata_island_me_count(chip, i) > 0)
				return NFP6000_MEID(i, 0);
		}
		break;
	default:
		break;
	}

	return -1;
}

int nfp_chipdata_meid_last(const struct nfp_chipdata_chip *chip)
{
	int i;

	if (!chip) {
		NS_LOG(NS_LOG_LVL_ERROR, "Invalid parameters.");
		return -1;
	}

	switch (chip->arch) {
	case NFP_CHIP_ARCH_YD:
		if (chip->details.nfp3200.me_cnt == 0)
			return -1;
		return NFP3200_MELIN2MEID(chip->details.nfp3200.me_cnt - 1);
	case NFP_CHIP_ARCH_TH:
		for (i = nfp_chipdata_island_id_last(chip); i != -1;
		     i = nfp_chipdata_island_id_prev(chip, i)) {
			size_t cnt = nfp_chipdata_island_me_count(chip, i);

			if (cnt > 0)
				return NFP6000_MEID(i, (cnt - 1));
		}
		break;
	default:
		break;
	}

	return -1;
}

int nfp_chipdata_meid_next(const struct nfp_chipdata_chip *chip, int meid)
{
	int i;
	const struct nsc_desc_island_nfp6000 *island6000;

	if ((!chip) || (meid < 0)) {
		NS_LOG(NS_LOG_LVL_ERROR, "Invalid parameters.");
		return -1;
	}

	switch (chip->arch) {
	case NFP_CHIP_ARCH_YD:
		i = NFP3200_MEID2MELIN(meid);
		if ((i >= 0) && ((i + 1) < (int)chip->details.nfp3200.me_cnt))
			return NFP3200_MELIN2MEID(i + 1);
		return -1;
	case NFP_CHIP_ARCH_TH:
		island6000 = nsc_nfp6000_island_find_by_iid(chip, NFP6000_MEID_ISLAND_of(meid));
		if (!island6000)
			return -1;
		i = NFP6000_MEID_MENUM_of(meid);
		if (i < 0)
			return -1;

		if ((i + 1) >= (int)island6000->me_cnt) {
			// Was last ME of island, find next island with MEs.
			for (i = nfp_chipdata_island_id_next(chip, island6000->id); i != -1;
			     i = nfp_chipdata_island_id_next(chip, i)) {
				island6000 = nsc_nfp6000_island_find_by_iid(chip, i);
				if (!island6000)
					return -1;
				if (nfp_chipdata_island_me_count(chip, i) > 0)
					return NFP6000_MEID(i, 0);
			}
		} else {
			return (meid + 1);
		}
		break;
	default:
		break;
	}

	return -1;
}

int nfp_chipdata_meid_prev(const struct nfp_chipdata_chip *chip, int meid)
{
	int i;
	const struct nsc_desc_island_nfp6000 *island6000;

	if ((!chip) || (meid < 0)) {
		NS_LOG(NS_LOG_LVL_ERROR, "Invalid parameters.");
		return -1;
	}

	switch (chip->arch) {
	case NFP_CHIP_ARCH_YD:
		i = NFP3200_MEID2MELIN(meid);
		if ((i - 1) >= 0)
			return NFP3200_MELIN2MEID(i - 1);
		return -1;
	case NFP_CHIP_ARCH_TH:
		i = NFP6000_MEID_MENUM_of(meid);
		if (i < 0)
			return -1;
		if (i > 0)
			return NFP6000_MEID(NFP6000_MEID_ISLAND_of(meid), (i - 1));

		island6000 = nsc_nfp6000_island_find_by_iid(chip, NFP6000_MEID_ISLAND_of(meid));
		if (!island6000)
			return -1;
		// Was first ME of island, find previous island with MEs.
		for (i = nfp_chipdata_island_id_prev(chip, island6000->id); i != -1;
		     i = nfp_chipdata_island_id_prev(chip, i)) {
			island6000 = nsc_nfp6000_island_find_by_iid(chip, i);
			if (!island6000)
				return -1;
			if (nfp_chipdata_island_me_count(chip, i) > 0)
				return NFP6000_MEID(i, nfp_chipdata_island_me_count(chip, i) - 1);
		}
		break;
	default:
		break;
	}

	return -1;
}

int nfp_chipdata_has_me(const struct nfp_chipdata_chip *chip, int meid)
{
	int iid, menum;
	const struct nsc_desc_island_nfp6000 *island;

	if ((!chip) || (meid < 0)) {
		NS_LOG(NS_LOG_LVL_ERROR, "Invalid parameters.");
		return 0;
	}

	switch (chip->arch) {
	case NFP_CHIP_ARCH_YD:
		menum = NFP3200_MEID2MELIN(meid);
		return (menum >= 0) && (menum < (int)chip->details.nfp3200.me_cnt);
	case NFP_CHIP_ARCH_TH:
		iid = NFP6000_MEID_ISLAND_of(meid);
		menum = NFP6000_MEID_MENUM_of(meid);

		island = nsc_nfp6000_island_find_by_iid(chip, iid);
		if (!island)
			return 0;

		return ((menum >= 0) && (menum < (int)island->me_cnt));
	default:
		break;
	}

	return 0;
}

enum nfp_chipdata_mu_type nfp_chipdata_island_mu_type(const struct nfp_chipdata_chip *chip,
						      int island_id)
{
	if (!chip) {
		NS_LOG(NS_LOG_LVL_ERROR, "Invalid parameters.");
		return NFP_CHIPDATA_MU_TYPE_NONE;
	}

	switch (chip->arch) {
	case NFP_CHIP_ARCH_YD:
		return NFP_CHIPDATA_MU_TYPE_DDR;
	case NFP_CHIP_ARCH_TH: {
		const struct nsc_desc_island_nfp6000 *island =
			nsc_nfp6000_island_find_by_iid(chip, island_id);
		if (!island)
			return 0;
		return island->mu_type;
	}
	default:
		break;
	}

	return 0;
}

uint64_t nfp_chipdata_mu_max_size(const struct nfp_chipdata_chip *chip, int island_id)
{
	enum nfp_chipdata_mem_type mem_type;

	if (!chip) {
		NS_LOG(NS_LOG_LVL_ERROR, "Invalid parameters.");
		return 0;
	}

	switch (chip->arch) {
	case NFP_CHIP_ARCH_YD:
		return nfp_chipdata_mem_max_size(chip, NFP_CHIPDATA_MEM_TYPE_DDR, -1);
	case NFP_CHIP_ARCH_TH: {
		const struct nsc_desc_island_nfp6000 *island =
			nsc_nfp6000_island_find_by_iid(chip, island_id);
		if (!island)
			return 0;

		switch (island->mu_type) {
		case NFP_CHIPDATA_MU_TYPE_CTM:
			mem_type = NFP_CHIPDATA_MEM_TYPE_CTM;
			break;
		case NFP_CHIPDATA_MU_TYPE_IMEM:
			mem_type = NFP_CHIPDATA_MEM_TYPE_IMEM;
			break;
		case NFP_CHIPDATA_MU_TYPE_EMEM:
			mem_type = NFP_CHIPDATA_MEM_TYPE_EMEM;
			break;
		default:
			return 0;
		}

		return nfp_chipdata_mem_max_size(chip, mem_type, island_id);
	}
	default:
		break;
	}

	return 0;
}

uint64_t nfp_chipdata_mem_base(const struct nfp_chipdata_chip *chip,
			       enum nfp_chipdata_mem_type mem_type,
			       int island_id)
{
	size_t idx;
	int iid;

	if (!chip) {
		NS_LOG(NS_LOG_LVL_ERROR, "Invalid parameters.");
		return 0;
	}

	switch (chip->arch) {
	case NFP_CHIP_ARCH_YD:
		switch (mem_type) {
		case NFP_CHIPDATA_MEM_TYPE_EMEM:
			mem_type = NFP_CHIPDATA_MEM_TYPE_DDR;
			break;
		default:
			break;
		}

#define _NSC_MEMS chip->chip_desc.nfp3200->memories
		for (idx = 0; idx < _NSC_MEMS.count; idx++) {
			struct _nfp_chipdata_memory *mem = &_NSC_MEMS.entry[idx];

			if (mem->type == mem_type)
				return mem->base;
		}
#undef _NSC_MEMS
		break;
	case NFP_CHIP_ARCH_TH:
#define _NSC_MEMS chip->chip_desc.nfp6000->memories
		for (idx = 0; idx < _NSC_MEMS.count; idx++) {
			const struct nsc_desc_island_nfp6000 *island = NULL;
			struct _nfp_chipdata_memory *mem = &_NSC_MEMS.entry[idx];

			if (mem->type != mem_type)
				continue;
			iid = island_id;

			if (mem->cpp_tgt == -1)
				iid = -1; // Ignore IID if mem is not a CPP target.

			if (iid <= 0)
				return mem->base;

			island = nsc_nfp6000_island_find_by_iid(chip, island_id);
			if (!island)
				return 0;

			for (idx = 0; idx < island->mem_types_count; idx++) {
				if (island->mem_types[idx] == mem_type)
					return NFP_MAX(mem->base, island->mem_cfgs[idx].base);
			}
		}
#undef _NSC_MEMS
		break;
	default:
		break;
	}

	return 0;
}

uint64_t nfp_chipdata_mem_alloc_base(const struct nfp_chipdata_chip *chip,
				     enum nfp_chipdata_mem_type mem_type,
				     int island_id)
{
	size_t idx;
	int iid;

	if (!chip) {
		NS_LOG(NS_LOG_LVL_ERROR, "Invalid parameters.");
		return 0;
	}

	switch (chip->arch) {
	case NFP_CHIP_ARCH_YD:
		switch (mem_type) {
		case NFP_CHIPDATA_MEM_TYPE_EMEM:
			mem_type = NFP_CHIPDATA_MEM_TYPE_DDR;
			break;
		default:
			break;
		}

#define _NSC_MEMS chip->chip_desc.nfp3200->memories
		for (idx = 0; idx < _NSC_MEMS.count; idx++) {
			struct _nfp_chipdata_memory *mem = &_NSC_MEMS.entry[idx];

			if (mem->type == mem_type)
				return mem->alloc_base;
		}
#undef _NSC_MEMS
		break;
	case NFP_CHIP_ARCH_TH:
#define _NSC_MEMS chip->chip_desc.nfp6000->memories
		for (idx = 0; idx < _NSC_MEMS.count; idx++) {
			const struct nsc_desc_island_nfp6000 *island = NULL;
			struct _nfp_chipdata_memory *mem = &_NSC_MEMS.entry[idx];

			if (mem->type != mem_type)
				continue;
			iid = island_id;

			if (mem->cpp_tgt == -1)
				iid = -1; // Ignore IID if mem is not a CPP target.

			if (iid <= 0)
				return mem->alloc_base;

			island = nsc_nfp6000_island_find_by_iid(chip, island_id);
			if (!island)
				return 0;

			for (idx = 0; idx < island->mem_types_count; idx++) {
				if (island->mem_types[idx] == mem_type)
					return NFP_MAX(mem->alloc_base,
						       island->mem_cfgs[idx].alloc_base);
			}
		}
#undef _NSC_MEMS
		break;
	default:
		break;
	}

	return 0;
}

uint64_t nfp_chipdata_mem_max_size(const struct nfp_chipdata_chip *chip,
				   enum nfp_chipdata_mem_type mem_type,
				   int island_id)
{
	size_t idx, jdx;
	int iid;

	if (!chip) {
		NS_LOG(NS_LOG_LVL_ERROR, "Invalid parameters.");
		return 0;
	}

	switch (chip->arch) {
	case NFP_CHIP_ARCH_YD:
		switch (mem_type) {
		case NFP_CHIPDATA_MEM_TYPE_EMEM:
			mem_type = NFP_CHIPDATA_MEM_TYPE_DDR;
			break;
		default:
			break;
		}

#define _NSC_MEMS chip->chip_desc.nfp3200->memories
		for (idx = 0; idx < _NSC_MEMS.count; idx++) {
			struct _nfp_chipdata_memory *mem = &_NSC_MEMS.entry[idx];

			if (mem->type == mem_type)
				return mem->size_max;
		}
#undef _NSC_MEMS
		break;
	case NFP_CHIP_ARCH_TH:
#define _NSC_MEMS chip->chip_desc.nfp6000->memories
		for (idx = 0; idx < _NSC_MEMS.count; idx++) {
			const struct nsc_desc_island_nfp6000 *island = NULL;
			struct _nfp_chipdata_memory *mem = &_NSC_MEMS.entry[idx];

			if (mem->type != mem_type)
				continue;
			iid = island_id;

			if (mem->cpp_tgt == -1)
				iid = -1; // Ignore IID if mem is not a CPP target.

			if (iid <= 0)
				return mem->size_max;

			island = nsc_nfp6000_island_find_by_iid(chip, island_id);
			if (!island)
				return 0;

			for (jdx = 0; jdx < island->mem_types_count; jdx++) {
				if (island->mem_types[jdx] == mem_type)
					return NFP_MIN(mem->size_max, island->mem_cfgs[jdx].size);
			}
		}
#undef _NSC_MEMS
		break;
	default:
		break;
	}

	return 0;
}

int nfp_chipdata_island_has_mem_type(const struct nfp_chipdata_chip *chip,
				     enum nfp_chipdata_mem_type mem_type,
				     int island_id)
{
	size_t idx;
	const struct nsc_desc_island_nfp6000 *island = NULL;

	if (!chip) {
		NS_LOG(NS_LOG_LVL_ERROR, "Invalid parameters.");
		return 0;
	}

	switch (chip->arch) {
	case NFP_CHIP_ARCH_YD:
		break;
	case NFP_CHIP_ARCH_TH:
		island = nsc_nfp6000_island_find_by_iid(chip, island_id);
		if (!island)
			break;

		for (idx = 0; idx < island->mem_types_count; idx++) {
			if (island->mem_types[idx] == mem_type)
				return 1;
		}
		break;
	default:
		break;
	}

	return 0;
}

const char *nfp_chipdata_mem_name(const struct nfp_chipdata_chip *chip,
				  enum nfp_chipdata_mem_type mem_type)
{
	size_t idx;
	struct nfp_chipdata *data = NULL;

	if (!chip) {
		NS_LOG(NS_LOG_LVL_ERROR, "Invalid parameters.");
		return NULL;
	}

	data = chip->chipdata;

	switch (chip->arch) {
	case NFP_CHIP_ARCH_YD:
		switch (mem_type) {
		case NFP_CHIPDATA_MEM_TYPE_EMEM:
			mem_type = NFP_CHIPDATA_MEM_TYPE_DDR;
			break;
		default:
			break;
		}

#define _NSC_MEMS chip->chip_desc.nfp3200->memories
		for (idx = 0; idx < _NSC_MEMS.count; idx++) {
			struct _nfp_chipdata_memory *mem = &_NSC_MEMS.entry[idx];

			if (mem->type == mem_type)
				return NSC_STR(mem->name);
		}
#undef _NSC_MEMS
		break;

	case NFP_CHIP_ARCH_TH:
#define _NSC_MEMS chip->chip_desc.nfp6000->memories
		for (idx = 0; idx < _NSC_MEMS.count; idx++) {
			struct _nfp_chipdata_memory *mem = &_NSC_MEMS.entry[idx];

			if (mem->type == mem_type)
				return NSC_STR(mem->name);
		}
#undef _NSC_MEMS
		break;

	default:
		break;
	}

	return NULL;
}

int nfp_chipdata_mem_cpp_tgt(const struct nfp_chipdata_chip *chip,
			     enum nfp_chipdata_mem_type mem_type)
{
	size_t idx;

	if (!chip) {
		NS_LOG(NS_LOG_LVL_ERROR, "Invalid parameters.");
		return -1;
	}

	switch (chip->arch) {
	case NFP_CHIP_ARCH_YD:
		switch (mem_type) {
		case NFP_CHIPDATA_MEM_TYPE_EMEM:
			mem_type = NFP_CHIPDATA_MEM_TYPE_DDR;
			break;
		default:
			break;
		}

#define _NSC_MEMS chip->chip_desc.nfp3200->memories
		for (idx = 0; idx < _NSC_MEMS.count; idx++) {
			struct _nfp_chipdata_memory *mem = &_NSC_MEMS.entry[idx];

			if (mem->type == mem_type)
				return mem->cpp_tgt;
		}
#undef _NSC_MEMS
		break;

	case NFP_CHIP_ARCH_TH:
#define _NSC_MEMS chip->chip_desc.nfp6000->memories
		for (idx = 0; idx < _NSC_MEMS.count; idx++) {
			struct _nfp_chipdata_memory *mem = &_NSC_MEMS.entry[idx];

			if (mem->type == mem_type)
				return mem->cpp_tgt;
		}
#undef _NSC_MEMS
		break;

	default:
		break;
	}

	return -1;
}

size_t nfp_chipdata_memories(const struct nfp_chipdata_chip *chip,
			     const struct nfp_chipdata_memory **memories)
{
	if (!chip) {
		NS_LOG(NS_LOG_LVL_ERROR, "Invalid parameters.");
		return 0;
	}

	switch (chip->arch) {
	case NFP_CHIP_ARCH_YD:
		if (!!memories)
			*memories = (const struct nfp_chipdata_memory *)&chip->chip_desc.nfp3200
					    ->memories.entry[0];
		return chip->chip_desc.nfp3200->memories.count;
	case NFP_CHIP_ARCH_TH:
		if (!!memories)
			*memories = (const struct nfp_chipdata_memory *)&chip->chip_desc.nfp6000
					    ->memories.entry[0];
		return chip->chip_desc.nfp6000->memories.count;
	default:
		break;
	}

	return 0;
}

size_t nfp_chipdata_island_mem_types(const struct nfp_chipdata_chip *chip,
				     const enum nfp_chipdata_mem_type **mem_types,
				     int island_id)
{
	static enum nfp_chipdata_mem_type nfp3200_mem_types[] = {NFP_CHIPDATA_MEM_TYPE_CLS};

	if (!chip) {
		NS_LOG(NS_LOG_LVL_ERROR, "Invalid parameters.");
		return 0;
	}

	switch (chip->arch) {
	case NFP_CHIP_ARCH_YD:
		if ((island_id < 0) || (island_id >= chip->chip_desc.nfp3200->cluster_count))
			return 0;
		if (!!mem_types)
			*mem_types = nfp3200_mem_types;
		return ARRAY_SIZE(nfp3200_mem_types);
	case NFP_CHIP_ARCH_TH: {
		const struct nsc_desc_island_nfp6000 *island =
			nsc_nfp6000_island_find_by_iid(chip, island_id);
		if (!island)
			return 0;
		if (!!mem_types)
			*mem_types = island->mem_types;
		return island->mem_types_count;
	}
	default:
		break;
	}

	return 0;
}

const char *nfp_chipdata_alloc_alias(const struct nfp_chipdata_chip *chip, const char *name)
{
	struct nsc_chipdata_alloc_alias *aliases;
	size_t count, idx;
	struct nfp_chipdata *data = NULL;

	if ((!chip) || (!name)) {
		NS_LOG(NS_LOG_LVL_ERROR, "Invalid parameters.");
		return NULL;
	}

	data = chip->chipdata;

	switch (chip->arch) {
	case NFP_CHIP_ARCH_YD:
		count = chip->chip_desc.nfp3200->alloc_aliases.count;
		aliases = chip->chip_desc.nfp3200->alloc_aliases.entry;
		break;
	case NFP_CHIP_ARCH_TH:
		count = chip->chip_desc.nfp6000->alloc_aliases.count;
		aliases = chip->chip_desc.nfp6000->alloc_aliases.entry;
		break;
	default:
		return NULL;
	}

	for (idx = 0; idx < count; idx++) {
		if (strcmp(NSC_STR(aliases[idx].alias), name) == 0)
			return NSC_STR(aliases[idx].name);
	}

	return name;
}

const struct nfp_chipdata_alloc_type *nfp_chipdata_find_alloc_type(
	const struct nfp_chipdata_chip *chip,
	const char *alloc_type)
{
	struct _nfp_chipdata_alloc_type *types;
	size_t count, idx;
	struct nfp_chipdata *data = NULL;

	if ((!chip) || (!alloc_type)) {
		NS_LOG(NS_LOG_LVL_ERROR, "Invalid parameters.");
		return NULL;
	}

	data = chip->chipdata;

	/* Determine which set of alloc types to use */
	switch (chip->arch) {
	case NFP_CHIP_ARCH_YD:
		count = chip->chip_desc.nfp3200->alloc_types.count;
		types = chip->chip_desc.nfp3200->alloc_types.entry;
		break;
	case NFP_CHIP_ARCH_TH:
		count = chip->chip_desc.nfp6000->alloc_types.count;
		types = chip->chip_desc.nfp6000->alloc_types.entry;
		break;
	default:
		return NULL;
	}

	/* First check if we have an alias for the alloc_type */
	alloc_type = nfp_chipdata_alloc_alias(chip, alloc_type);

	/* Find the allocation type */
	for (idx = 0; idx < count; idx++) {
		if (strcmp(NSC_STR(types[idx].name), alloc_type) == 0)
			return (const struct nfp_chipdata_alloc_type *)&types[idx];
	}

	return NULL;
}

const struct nfp_chipdata_alloc *nfp_chipdata_find_alloc(const struct nfp_chipdata_chip *chip,
							 const char *alloc_type,
							 int island_id)
{
	const struct nfp_chipdata_alloc_type *type;
	size_t idx1, idx2;

	if ((!chip) || (!alloc_type)) {
		NS_LOG(NS_LOG_LVL_ERROR, "Invalid parameters.");
		return NULL;
	}

	/* Find the allocation type */
	type = nfp_chipdata_find_alloc_type(chip, alloc_type);

	if (!type)
		return NULL;

	/* Locate the specific allocation struct within the alloc type */
	for (idx1 = 0; idx1 < type->entries_count; idx1++) {
		const struct _nfp_chipdata_alloc *alloc =
			(const struct _nfp_chipdata_alloc *)&type->entries[idx1];

		if (alloc->islands_count == 0)
			return (const struct nfp_chipdata_alloc *)alloc;

		for (idx2 = 0; idx2 < alloc->islands_count; idx2++) {
			if (alloc->islands[idx2] == island_id)
				return (const struct nfp_chipdata_alloc *)alloc;
		}
	}

	return NULL;
}

size_t nfp_chipdata_alloc_types(const struct nfp_chipdata_chip *chip,
				const struct nfp_chipdata_alloc_type **types)
{
	if (!chip) {
		NS_LOG(NS_LOG_LVL_ERROR, "Invalid parameters.");
		return 0;
	}

	switch (chip->arch) {
	case NFP_CHIP_ARCH_YD:
		if (!!types)
			*types = (const struct nfp_chipdata_alloc_type *)&chip->chip_desc.nfp3200
					 ->alloc_types.entry[0];
		return chip->chip_desc.nfp3200->alloc_types.count;
	case NFP_CHIP_ARCH_TH:
		if (!!types)
			*types = (const struct nfp_chipdata_alloc_type *)&chip->chip_desc.nfp6000
					 ->alloc_types.entry[0];
		return chip->chip_desc.nfp6000->alloc_types.count;
	default:
		break;
	}

	return 0;
}

size_t nfp_chipdata_cppat_entries(const struct nfp_chipdata_chip *chip,
				  const union nfp_chipdata_cppat_entry **entries)
{
	if (!chip) {
		NS_LOG(NS_LOG_LVL_ERROR, "Invalid parameters.");
		return 0;
	}

	switch (chip->arch) {
	case NFP_CHIP_ARCH_YD:
		if (!!entries)
			*entries = NULL;
		return 0;
	case NFP_CHIP_ARCH_TH:
		if (!!entries)
			*entries = &chip->cppats.entry[0];
		return chip->cppats.count;
	default:
		break;
	}

	return 0;
}

const union nfp_chipdata_cppat_entry *nfp_chipdata_cppat_entry_find(
	int chip_family,
	const union nfp_chipdata_cppat_entry *entries,
	size_t entry_cnt,
	int from_island,
	int cpp_tgt)
{
	size_t idx;
	const union nfp_chipdata_cppat_entry *entry;

	if (!entries) {
		NS_LOG(NS_LOG_LVL_ERROR, "Invalid parameters.");
		return NULL;
	}

	switch (_NFP_ARCH(chip_family)) {
	case NFP_CHIP_ARCH_TH:
		entry = entries;
		for (idx = 0; idx < entry_cnt; idx++, entry++) {
			if ((entry->nfp6000.from_iid == from_island) &&
			    (entry->nfp6000.cpp_tgt == cpp_tgt))
				return entry;
		}
	default:
		break;
	}

	return NULL;
}

int nfp_chipdata_cppat_encode(int chip_family,
			      const union nfp_chipdata_cppat_entry *entries,
			      size_t entry_cnt,
			      int from_island,
			      int cpp_tgt,
			      int to_island,
			      uint64_t *addr)
{
	const union nfp_chipdata_cppat_entry *entry;

	if ((!entries) || (!addr)) {
		NS_LOG(NS_LOG_LVL_ERROR, "Invalid parameters.");
		return -EINVAL;
	}

	switch (_NFP_ARCH(chip_family)) {
	case NFP_CHIP_ARCH_YD:
		return 0;
	case NFP_CHIP_ARCH_TH:
		entry = nfp_chipdata_cppat_entry_find(chip_family, entries, entry_cnt, from_island,
						      cpp_tgt);
		if (!entry) {
			NS_LOG(NS_LOG_LVL_ERROR,
			       "No IMB CPPAT Table entry for island %d CPP Target %d.",
			       from_island, cpp_tgt);
			return -ENODEV;
		}

		return _nfp6000_cppat_addr_encode(addr, to_island, cpp_tgt, entry->nfp6000.mode,
						  entry->nfp6000.addr40, entry->nfp6000.isld1,
						  entry->nfp6000.isld0);
	default:
		break;
	}

	return -EINVAL;
}

int nfp_chipdata_cppat_decode(int chip_family,
			      const union nfp_chipdata_cppat_entry *entries,
			      size_t entry_cnt,
			      int from_island,
			      int cpp_tgt,
			      int *to_island,
			      uint64_t addr)
{
	const union nfp_chipdata_cppat_entry *entry;

	if ((!entries) || (!to_island)) {
		NS_LOG(NS_LOG_LVL_ERROR, "Invalid parameters.");
		return -EINVAL;
	}

	switch (_NFP_ARCH(chip_family)) {
	case NFP_CHIP_ARCH_YD:
		return 0;
	case NFP_CHIP_ARCH_TH:
		entry = nfp_chipdata_cppat_entry_find(chip_family, entries, entry_cnt, from_island,
						      cpp_tgt);
		if (!entry) {
			NS_LOG(NS_LOG_LVL_ERROR,
			       "No IMB CPPAT Table entry for island %d CPP Target %d.",
			       from_island, cpp_tgt);
			return -ENODEV;
		}

		return _nfp6000_cppat_addr_decode(addr, to_island, cpp_tgt, entry->nfp6000.mode,
						  entry->nfp6000.addr40, entry->nfp6000.isld1,
						  entry->nfp6000.isld0);
	default:
		break;
	}

	return -EINVAL;
}

int nfp_chipdata_cppat_mu_locality_lsb(int chip_family,
				       const union nfp_chipdata_cppat_entry *entries,
				       size_t entry_cnt,
				       int from_island)
{
	const union nfp_chipdata_cppat_entry *entry;

	if (!entries) {
		NS_LOG(NS_LOG_LVL_ERROR, "Invalid parameters.");
		return -1;
	}

	switch (_NFP_ARCH(chip_family)) {
	case NFP_CHIP_ARCH_YD:
		return -1;
	case NFP_CHIP_ARCH_TH:
		entry = nfp_chipdata_cppat_entry_find(chip_family, entries, entry_cnt, from_island,
						      NFP6000_CPPTGT_MU);
		if (!entry) {
			NS_LOG(NS_LOG_LVL_ERROR,
			       "No IMB CPPAT Table entry for island %d CPP Target %d.",
			       from_island, NFP6000_CPPTGT_MU);
			return -1;
		}

		return _nfp6000_cppat_mu_locality_lsb(entry->nfp6000.mode, entry->nfp6000.addr40);
	default:
		break;
	}

	return -1;
}

enum nfp_chipdata_mem_class nfp_chipdata_mem_type2class(const struct nfp_chipdata_chip *chip,
							enum nfp_chipdata_mem_type mem_type)
{
	if (!chip) {
		NS_LOG(NS_LOG_LVL_ERROR, "Invalid parameters.");
		return -1;
	}

	switch (chip->arch) {
	case NFP_CHIP_ARCH_YD:
		switch (mem_type) {
		case NFP_CHIPDATA_MEM_TYPE_DDR:
		case NFP_CHIPDATA_MEM_TYPE_EMEM:
			return NFP_CHIPDATA_MEM_CLASS_MU;
		case NFP_CHIPDATA_MEM_TYPE_QDR0:
		case NFP_CHIPDATA_MEM_TYPE_QDR1:
		case NFP_CHIPDATA_MEM_TYPE_QDR2:
		case NFP_CHIPDATA_MEM_TYPE_QDR3:
			return NFP_CHIPDATA_MEM_CLASS_QDR;
		case NFP_CHIPDATA_MEM_TYPE_CLS:
			return NFP_CHIPDATA_MEM_CLASS_CLS;
		case NFP_CHIPDATA_MEM_TYPE_GS:
			return NFP_CHIPDATA_MEM_CLASS_GS;
		case NFP_CHIPDATA_MEM_TYPE_LM:
			return NFP_CHIPDATA_MEM_CLASS_LM;
		case NFP_CHIPDATA_MEM_TYPE_USTORE:
			return NFP_CHIPDATA_MEM_CLASS_USTORE;
		case NFP_CHIPDATA_MEM_TYPE_CRYPTO:
			return NFP_CHIPDATA_MEM_CLASS_CRYPTO;
		default:
			break;
		}
		break;
	case NFP_CHIP_ARCH_TH:
		switch (mem_type) {
		case NFP_CHIPDATA_MEM_TYPE_DDR:
		case NFP_CHIPDATA_MEM_TYPE_EMEM:
		case NFP_CHIPDATA_MEM_TYPE_EMEM_CACHE:
		case NFP_CHIPDATA_MEM_TYPE_IMEM:
		case NFP_CHIPDATA_MEM_TYPE_CTM:
			return NFP_CHIPDATA_MEM_CLASS_MU;
		case NFP_CHIPDATA_MEM_TYPE_QDR0:
		case NFP_CHIPDATA_MEM_TYPE_QDR1:
		case NFP_CHIPDATA_MEM_TYPE_QDR2:
		case NFP_CHIPDATA_MEM_TYPE_QDR3:
			return NFP_CHIPDATA_MEM_CLASS_QDR;
		case NFP_CHIPDATA_MEM_TYPE_CLS:
			return NFP_CHIPDATA_MEM_CLASS_CLS;
		case NFP_CHIPDATA_MEM_TYPE_LM:
			return NFP_CHIPDATA_MEM_CLASS_LM;
		case NFP_CHIPDATA_MEM_TYPE_USTORE:
			return NFP_CHIPDATA_MEM_CLASS_USTORE;
		case NFP_CHIPDATA_MEM_TYPE_CRYPTO:
			return NFP_CHIPDATA_MEM_CLASS_CRYPTO;
		case NFP_CHIPDATA_MEM_TYPE_ILA:
			return NFP_CHIPDATA_MEM_CLASS_ILA;
		default:
			break;
		}
		break;
	default:
		break;
	}

	return NFP_CHIPDATA_MEM_CLASS_NONE;
}

const char *nfp_chipdata_mem_type2str(int chip_family,
				      char *s,
				      enum nfp_chipdata_mem_type mem_type,
				      int island_id,
				      int menum)
{
	const char *c;
	const struct nsc_enum_string_map *str_map = NULL;

	switch (_NFP_ARCH(chip_family)) {
	case NFP_CHIP_ARCH_YD:
		str_map = nsc_nfp3200_mem_type_string_map;
		switch (mem_type) {
		case NFP_CHIPDATA_MEM_TYPE_DDR:
		case NFP_CHIPDATA_MEM_TYPE_EMEM:
		case NFP_CHIPDATA_MEM_TYPE_QDR0:
		case NFP_CHIPDATA_MEM_TYPE_QDR1:
		case NFP_CHIPDATA_MEM_TYPE_QDR2:
		case NFP_CHIPDATA_MEM_TYPE_QDR3:
		case NFP_CHIPDATA_MEM_TYPE_GS:
			s[0] = '\0';
			break;
		case NFP_CHIPDATA_MEM_TYPE_CLS:
			c = nfp3200_cluster2str(s, island_id);
			if (!c)
				return NULL;
			break;
		case NFP_CHIPDATA_MEM_TYPE_LM:
		case NFP_CHIPDATA_MEM_TYPE_USTORE:
			c = nfp3200_meid2str(s, NFP3200_MEID(island_id, menum));
			if (!c)
				return NULL;
			break;
		default:
			return NULL;
		}
		break;
	case NFP_CHIP_ARCH_TH:
		str_map = nsc_nfp6000_mem_type_string_map;
		switch (mem_type) {
		case NFP_CHIPDATA_MEM_TYPE_QDR0:
		case NFP_CHIPDATA_MEM_TYPE_QDR1:
		case NFP_CHIPDATA_MEM_TYPE_QDR2:
		case NFP_CHIPDATA_MEM_TYPE_QDR3:
			s[0] = '\0';
			break;
		case NFP_CHIPDATA_MEM_TYPE_DDR:
			mem_type = NFP_CHIPDATA_MEM_TYPE_EMEM;
		// Fall through
		case NFP_CHIPDATA_MEM_TYPE_EMEM:
		case NFP_CHIPDATA_MEM_TYPE_EMEM_CACHE:
		case NFP_CHIPDATA_MEM_TYPE_IMEM:
		case NFP_CHIPDATA_MEM_TYPE_CTM:
		case NFP_CHIPDATA_MEM_TYPE_CLS:
			c = nfp6000_island2str(s, island_id);
			if (!c)
				return NULL;
			strcat(s, ".");
			break;
		case NFP_CHIPDATA_MEM_TYPE_LM:
		case NFP_CHIPDATA_MEM_TYPE_USTORE:
			c = nfp6000_meid2str(s, NFP6000_MEID(island_id, menum));
			if (!c)
				return NULL;
			strcat(s, ".");
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}

	strcat(s, nsc_enum_to_string(str_map, (int)mem_type, "<unknown>"));
	return s;
}

enum nfp_chipdata_mem_type nfp_chipdata_idstr2mem_type(int chip_family,
						       const char *s,
						       const char **endptr,
						       int *island_id,
						       int *menum)
{
	const char *c;
	char *c2;
	int iid = -1, me = -1;
	enum nfp_chipdata_mem_type mem_type = NFP_CHIPDATA_MEM_TYPE_NONE;
	const struct nsc_enum_string_map *str_map = NULL;
	size_t n;

	switch (chip_family) {
	case NFP_CHIP_FAMILY_NFP3200:
		str_map = nsc_nfp3200_mem_type_string_map;
		break;
	case NFP_CHIP_FAMILY_NFP4000:
	case NFP_CHIP_FAMILY_NFP5000:
		/* Remap before using nfp_idstr2*() */
		chip_family = NFP_CHIP_FAMILY_NFP6000;
	case NFP_CHIP_FAMILY_NFP6000:
	case NFP_CHIP_FAMILY_NFP3800:
		str_map = nsc_nfp6000_mem_type_string_map;
		break;
	default:
		return NFP_CHIPDATA_MEM_TYPE_NONE;
	}

	iid = nfp_idstr2island(chip_family, s, &c);
	if (iid >= 0)
		me = nfp_idstr2menum(chip_family, c, &c);

	if (island_id)
		*island_id = iid;
	if (menum)
		*menum = me;

	n = strlen(c);
	c2 = strchr(c, '.');
	if (c2)
		n = c2 - c;
	mem_type = nsc_enum_from_stringn(str_map, c, n);
	if (endptr) {
		if (c2)
			*endptr = &c[n + 1];
		else
			*endptr = &c[n];
	}

	if (mem_type == NFP_CHIPDATA_MEM_TYPE_NONE)
		return NFP_CHIPDATA_MEM_TYPE_NONE;

	switch (chip_family) {
	case NFP_CHIP_FAMILY_NFP3200:
		switch (mem_type) {
		case NFP_CHIPDATA_MEM_TYPE_DDR:
		case NFP_CHIPDATA_MEM_TYPE_EMEM:
		case NFP_CHIPDATA_MEM_TYPE_QDR0:
		case NFP_CHIPDATA_MEM_TYPE_QDR1:
		case NFP_CHIPDATA_MEM_TYPE_QDR2:
		case NFP_CHIPDATA_MEM_TYPE_QDR3:
		case NFP_CHIPDATA_MEM_TYPE_GS:
			if ((iid >= 0) || (me >= 0))
				return NFP_CHIPDATA_MEM_TYPE_NONE;
			break;
		case NFP_CHIPDATA_MEM_TYPE_CLS:
			if ((iid < 0) || (me >= 0))
				return NFP_CHIPDATA_MEM_TYPE_NONE;
			break;
		case NFP_CHIPDATA_MEM_TYPE_LM:
		case NFP_CHIPDATA_MEM_TYPE_USTORE:
			if ((iid < 0) || (me < 0))
				return NFP_CHIPDATA_MEM_TYPE_NONE;
			break;
			break;
		default:
			return NFP_CHIPDATA_MEM_TYPE_NONE;
		}
		break;
	case NFP_CHIP_FAMILY_NFP6000:
	case NFP_CHIP_FAMILY_NFP3800:
		switch (mem_type) {
		case NFP_CHIPDATA_MEM_TYPE_QDR0:
		case NFP_CHIPDATA_MEM_TYPE_QDR1:
		case NFP_CHIPDATA_MEM_TYPE_QDR2:
		case NFP_CHIPDATA_MEM_TYPE_QDR3:
			if ((iid >= 0) || (me >= 0))
				return NFP_CHIPDATA_MEM_TYPE_NONE;
			break;
		case NFP_CHIPDATA_MEM_TYPE_DDR:
			mem_type = NFP_CHIPDATA_MEM_TYPE_EMEM;
		// Fall through
		case NFP_CHIPDATA_MEM_TYPE_EMEM:
		case NFP_CHIPDATA_MEM_TYPE_EMEM_CACHE:
		case NFP_CHIPDATA_MEM_TYPE_IMEM:
		case NFP_CHIPDATA_MEM_TYPE_CTM:
		case NFP_CHIPDATA_MEM_TYPE_CLS:
			if ((iid < 0) || (me >= 0))
				return NFP_CHIPDATA_MEM_TYPE_NONE;
			break;
		case NFP_CHIPDATA_MEM_TYPE_LM:
		case NFP_CHIPDATA_MEM_TYPE_USTORE:
			if ((iid < 0) || (me < 0))
				return NFP_CHIPDATA_MEM_TYPE_NONE;
			break;
		default:
			return NFP_CHIPDATA_MEM_TYPE_NONE;
		}
		break;
	default:
		return NFP_CHIPDATA_MEM_TYPE_NONE;
	}

	return mem_type;
}

const char *nfp_chipdata_cfg_val(const struct nfp_chipdata_chip *chip, const char *key)
{
	struct nsc_chip_cfg *cfg = NULL;
	size_t idx;
	struct nfp_chipdata *data = NULL;

	if ((!chip) || (!key)) {
		NS_LOG(NS_LOG_LVL_ERROR, "Invalid parameters.");
		return NULL;
	}

	data = chip->chipdata;

	while (chip) {
		for (idx = 0, cfg = chip->cfgs.entry; idx < chip->cfgs.count; idx++, cfg++) {
			if (!strcmp(NSC_STR(cfg->k), key))
				return NSC_STR(cfg->v);
		}
		chip = chip->parent_chip;
	}

	return NULL;
}

void nfp_chipdata_free(struct nfp_chipdata *data)
{
	struct nsc_llist_node *node;

	if (!data)
		return;

	if (!!data->chipdata_path)
		free(data->chipdata_path);

	nsc_strtab_free(&data->search_paths);

	node = data->chip_decls.node;
	while (!!node) {
		struct nsc_llist_node *next_node = node->next;

		nfp_chipdata_chip_free((struct nfp_chipdata_chip *)node);
		node = next_node;
	}

	node = data->chip_descs.node;
	while (!!node) {
		struct nsc_llist_node *next_node = node->next;

		nsc_chip_desc_free((struct nsc_chip_desc *)node);
		node = next_node;
	}

	if (!!data->json_comment)
		free(data->json_comment);

	nsc_strtab_free(&data->strtab);

	memset(data, 0, sizeof(*data));
	free(data);
}

#ifdef _MSC_VER
#pragma warning(pop)
#endif
