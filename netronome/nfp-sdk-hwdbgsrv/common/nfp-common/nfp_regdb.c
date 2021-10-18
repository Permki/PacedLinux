/*
 * Copyright (C) 2014,2017 Netronome Systems, Inc.  All rights reserved.
 */

#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <errno.h>
#include <limits.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>

#include <nfp-common/nfp_platform.h>
#include <nfp-common/nfp_resid.h>
#include <nfp-common/ns_log.h>
#include <nfp-common/nfp_regdb.h>

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4706)
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

#define _NRDB_FLG_REMOVE BIT(0)
#define _NRDB_FLG_CHANGED BIT(1)

struct _nfp_regdb_table {
	uint32_t table_id;

	_ARRAY_DECLARE(struct nfp_regdb_entry, tblents);
};

struct nfp_regdb {
	_ARRAY_DECLARE(struct _nfp_regdb_table, tables);
};

static int _nfp_regdb_table_cmp(const void *a, const void *b)
{
	struct _nfp_regdb_table *tbla = (struct _nfp_regdb_table *)a;
	struct _nfp_regdb_table *tblb = (struct _nfp_regdb_table *)b;

	return (tbla->table_id > tblb->table_id) - (tbla->table_id < tblb->table_id);
}

static int _nfp_regdb_table_cmp_noowner(const void *a, const void *b)
{
	struct _nfp_regdb_table *tbla = (struct _nfp_regdb_table *)a;
	struct _nfp_regdb_table *tblb = (struct _nfp_regdb_table *)b;

	return (NFP_REGDB_TABLE_ID_NO_OWNER(tbla->table_id) >
		NFP_REGDB_TABLE_ID_NO_OWNER(tblb->table_id)) -
	       (NFP_REGDB_TABLE_ID_NO_OWNER(tbla->table_id) <
		NFP_REGDB_TABLE_ID_NO_OWNER(tblb->table_id));
}

static int _nfp_regdb_entry_sortcmp(const void *a, const void *b)
{
	const struct nfp_regdb_entry *enta = (const struct nfp_regdb_entry *)a;
	const struct nfp_regdb_entry *entb = (const struct nfp_regdb_entry *)b;
	uint32_t va, vb;
	uint8_t widx;
	int ret;

	if ((!enta->words) || (!entb->words))
		return (enta->words > entb->words) - (enta->words < entb->words);

	/* First sort by offsets */
	va = NFP_REGDB_ENTRY_WORD_W0_OHI_of(enta->words[0].w0);
	vb = NFP_REGDB_ENTRY_WORD_W0_OHI_of(entb->words[0].w0);
	if (va != vb)
		return (va > vb) - (va < vb);

	va = enta->words[0].offset_lo;
	vb = entb->words[0].offset_lo;
	if (va != vb)
		return (va > vb) - (va < vb);

	/* Then by ctx */
	va = NFP_REGDB_ENTRY_WORD_W0_CTX_of(enta->words[0].w0);
	vb = NFP_REGDB_ENTRY_WORD_W0_CTX_of(entb->words[0].w0);
	if (va != vb)
		return (va > vb) - (va < vb);

	/* Keep undefined values at the back */
	va = NFP_REGDB_ENTRY_WORD_W0_VUN_of(enta->words[0].w0);
	vb = NFP_REGDB_ENTRY_WORD_W0_VUN_of(entb->words[0].w0);
	if (va != vb)
		return (va > vb) - (va < vb);

	/* Then by value type */
	va = NFP_REGDB_ENTRY_WORD_W0_VTP_of(enta->words[0].w0);
	vb = NFP_REGDB_ENTRY_WORD_W0_VTP_of(entb->words[0].w0);
	ret = nfp_regdb_val_type_cmp((enum nfp_regdb_valtype)va, (enum nfp_regdb_valtype)vb);
	if (ret != 0)
		return ret;

	for (widx = enta->word_count; widx > 0; widx--) {
		va = enta->words[widx - 1].mask;
		vb = entb->words[widx - 1].mask;
		if (va != vb)
			return (va > vb) - (va < vb);
	}

	return 0;
}

static nfp_inline int _nfp_regdb_entry_idcmp(const void *a, const void *b)
{
	return nfp_regdb_entry_regcmp(a, b);
}

/** Find the spot where a new entry must be inserted to keep the array sorted.
 *
 *  - If the array is empty, we insert at n = 0.
 *  - Then do a binary search.
 *  -- If the new entry goes after the tested one (cmp_res < 0):
 *  --- Increment n onto the next entry, a possible insertion point.
 *  --- Set lower barrier to this new n
 *  -- If the new entry goes before the tested one (cmp_res > 0):
 *  --- Keep n, possible insertion point.
 *  --- Set higher barrier to below n
 *  -- If the new entry matches the tested one (cmp_res == 0):
 *  --- Increment n until we reach the end of the array or an entry that does
 *      not match. This is a definite insertion point.
 *  -- While the search range is not empty, repeat
 */
static size_t _find_insert(const void *key, const void *base, size_t nmemb,
			   size_t size, int (*compar)(const void *, const void *))
{
	size_t n = SIZE_MAX;
	size_t lo = 0, hi = nmemb - 1;

	if (nmemb == 0)
		return 0;

	while (lo <= hi) {
		int cmp_res;

		n = (lo + hi) / 2;

		cmp_res = compar((uint8_t *)base + (size * n), key);

		if (cmp_res < 0) {
			n++;
			lo = n;
		} else if (cmp_res > 0) {
			if (n == 0)
				break;
			hi = n - 1;
		} else if (cmp_res == 0) {
			n++;
			while ((n < nmemb) &&
			       (compar((uint8_t *)base + (size * n), key) == 0))
				n++;
			break;
		}
	}

	return n;
}

static void _nfp_regdb_entry_free(struct nfp_regdb_entry *entry)
{
	if (!entry)
		return;
	if (entry->words)
		free(entry->words);
}

static void _nfp_regdb_table_free(struct _nfp_regdb_table *table,
				  void (*removing)(uint32_t table_id,
						   struct nfp_regdb_entry *entry))
{
	size_t idx;
	struct nfp_regdb_entry *ent;

	if (!table)
		return;

	if (!table->tblents.entry)
		return;

	for (idx = 0, ent = table->tblents.entry; idx < table->tblents.count; idx++, ent++) {
		if (removing)
			removing(table->table_id, ent);
		_nfp_regdb_entry_free(ent);
	}

	free(table->tblents.entry);
};

struct nfp_regdb *nfp_regdb_new()
{
	struct nfp_regdb *db = NULL;
	int err = EINVAL;

	_CALLOC_OOM(db, 1, sizeof(*db));

	return db;

error_cleanup:
	if (db)
		nfp_regdb_free(db, NULL);
	return NFP_ERRPTR(err);
};

void nfp_regdb_free(struct nfp_regdb *db,
		    void (*removing)(uint32_t table_id, struct nfp_regdb_entry *entry))
{
	size_t idx;
	struct _nfp_regdb_table *tbl;

	if (!db)
		return;

	if (db->tables.entry) {
		for (idx = 0, tbl = db->tables.entry; idx < db->tables.count; idx++, tbl++)
			_nfp_regdb_table_free(tbl, removing);
		free(db->tables.entry);
	}

	free(db);
}

struct nfp_regdb *nfp_regdb_from_buf(const void *buf, size_t buf_len)
{
	struct nfp_regdb *db = NULL;
	int err = EINVAL;
	const uint32_t *p32 = (uint32_t *)buf;
	const uint32_t *buf_end32 = p32 + (buf_len / 4);
	struct nfp_regdb_entry_word *words = NULL;
	uint8_t words_cnt = 0;

	db = nfp_regdb_new();

	if (buf_len == 0)
		return db;

	while (p32 < buf_end32) {
		uint32_t table_id, tblsz;
		const uint32_t *table_end32;
		uint8_t wcnt = 0, widx;

		if ((buf_end32 - p32) < 2) {
			err = EFAULT;
			goto error_cleanup;
		}

		table_id = NFP_LETOH32(*p32++);
		tblsz = NFP_LETOH32(*p32++);
		if (tblsz % 4) {
			NS_LOG(NS_LOG_LVL_ERROR,
			       "Bad table size (0x%" PRIx32 ") at offset 0x%" PRIx32,
			       tblsz, ((uint32_t)(ptrdiff_t)(p32 - 1) * 4));
			err = EFAULT;
			goto error_cleanup;
		}

		table_end32 = p32 + (tblsz / 4);
		while (p32 < table_end32) {
			if (!NFP_REGDB_ENTRY_WORD_W0_LWO_of(NFP_LETOH32(*p32))) {
				p32 += 4;
				wcnt++;
				if (wcnt == UINT8_MAX) {
					NS_LOG(NS_LOG_LVL_ERROR,
					       "Too many words for table entry at offset 0x%" PRIx32,
					       ((uint32_t)(ptrdiff_t)(p32 - (wcnt * 4)) * 4));
					err = EFAULT;
					goto error_cleanup;
				}
				continue;
			}
			wcnt++;
			p32 += 4;

			if (wcnt > words_cnt) {
				_REALLOC_OOM(words, (wcnt * (sizeof(*words))));
				words_cnt = wcnt;
			}

			for (p32 = p32 - (4 * wcnt), widx = 0; widx < wcnt; widx++) {
				words[widx].w0 = NFP_LETOH32(*p32++);
				words[widx].offset_lo = NFP_LETOH32(*p32++);
				words[widx].value = NFP_LETOH32(*p32++);
				words[widx].mask = NFP_LETOH32(*p32++);
			}

			if (nfp_regdb_entry_add(db, table_id, words, wcnt, NULL) != 0) {
				NS_LOG(NS_LOG_LVL_ERROR,
				       "Could not add entries to database, at offset 0x%" PRIx32,
				       ((uint32_t)(ptrdiff_t)(p32 - (wcnt * 4)) * 4));
				err = errno;
				goto error_cleanup;
			}

			wcnt = 0;
		}
	}

	return db;
error_cleanup:
	return NFP_ERRPTR(err);
}

ssize_t nfp_regdb_to_buf(struct nfp_regdb *db, void **buf_out)
{
	uint32_t *table_ids = NULL;
	size_t tblcnt = 0, tblidx;
	size_t entcnt = 0, entidx;
	uint8_t widx;
	uint32_t *buf = NULL, *p32;
	ssize_t bufsz = 0;
	int err = EINVAL;

	if (buf_out)
		*buf_out = NULL;

	if ((!db) || (!db->tables.count)) {
		err = ENOENT;
		return -1;
	}

	tblcnt = nfp_regdb_table_ids(db, &table_ids);
	for (tblidx = 0; tblidx < tblcnt; tblidx++) {
		const struct nfp_regdb_entry *entry;

		/* table_hdr.table_id */
		/* table_hdr.byte_size */
		bufsz += 8;
		entcnt = nfp_regdb_table_entries(db, table_ids[tblidx], &entry);
		for (entidx = 0; entidx < entcnt; entidx++, entry++)
			bufsz += (ssize_t)entry->word_count * sizeof(*entry->words);
	}

	_MALLOC_OOM(buf, bufsz);
	p32 = buf;

	for (tblidx = 0; tblidx < tblcnt; tblidx++) {
		const struct nfp_regdb_entry *entry;
		uint32_t *p_tbl_sz = &p32[1];
		uint32_t tblsz = 0;

		*p32++ = NFP_HTOLE32(table_ids[tblidx]);
		*p32++ = 0;
		entcnt = nfp_regdb_table_entries(db, table_ids[tblidx], &entry);
		for (entidx = 0; entidx < entcnt; entidx++, entry++) {
			tblsz += (ssize_t)entry->word_count * sizeof(*entry->words);
			for (widx = 0; widx < entry->word_count; widx++) {
				*p32++ = NFP_HTOLE32(entry->words[widx].w0);
				*p32++ = NFP_HTOLE32(entry->words[widx].offset_lo);
				*p32++ = NFP_HTOLE32(entry->words[widx].value);
				*p32++ = NFP_HTOLE32(entry->words[widx].mask);
			}
		}
		*p_tbl_sz = NFP_HTOLE32(tblsz);
	}

	if (buf_out)
		*buf_out = buf;
	else
		free(buf);
	if (table_ids)
		free(table_ids);
	return bufsz;

error_cleanup:
	if (buf)
		free(buf);
	if (table_ids)
		free(table_ids);
	return NFP_ERRNO(err);
}

int nfp_regdb_table_compact(struct nfp_regdb *db,
			    uint32_t table_id,
			    void (*removing)(uint32_t table_id, struct nfp_regdb_entry *entry),
			    int (*can_merge)(uint32_t table_id,
					     const struct nfp_regdb_entry *entry1,
					     const struct nfp_regdb_entry *entry2))
{
	size_t i, j, k;
	struct _nfp_regdb_table *tbl;
	struct _nfp_regdb_table bkey;
	struct nfp_regdb_entry *a, *b;
	struct nfp_regdb_entry_word *wa, *wb;
	enum nfp_regdb_valtype vtp_a, vtp_b;
	int amasks_clear, bmasks_clear, merge, vundef;
	uint32_t m;

	if ((!db) || (!db->tables.count))
		return NFP_ERRNO(ENOENT);

	bkey.table_id = table_id;
	tbl = bsearch(&bkey, db->tables.entry, db->tables.count, sizeof(*db->tables.entry),
		      _nfp_regdb_table_cmp);

	if (!tbl)
		return NFP_ERRNO(ENOENT);

	if (tbl->tblents.count <= 1)
		return 0;

	/* Assumes sorted entries */
	a = tbl->tblents.entry;
	for (i = 0; i < (tbl->tblents.count - 1); i++, a++) {
		if (a->internal_flags & _NRDB_FLG_REMOVE)
			continue;
		b = a + 1;
		for (j = i + 1; j < tbl->tblents.count; j++, b++) {
			if (_nfp_regdb_entry_idcmp(a, b) != 0)
				break;
			if (b->internal_flags & _NRDB_FLG_REMOVE)
				continue;

			merge = 0;
			vundef = NFP_REGDB_ENTRY_WORD_W0_VUN_of(a->words[0].w0) |
				 (NFP_REGDB_ENTRY_WORD_W0_VUN_of(b->words[0].w0) << 1);
			vtp_a = NFP_REGDB_ENTRY_WORD_W0_VTP_of(a->words[0].w0);
			vtp_b = NFP_REGDB_ENTRY_WORD_W0_VTP_of(b->words[0].w0);
			amasks_clear = 1;
			bmasks_clear = 1;
			switch (vtp_a) {
			case NFP_REGDB_VALTYPE_CONST:
				switch (vtp_b) {
				case NFP_REGDB_VALTYPE_CONST:
					if ((vundef == 0x1) || (vundef == 0x2)) {
						amasks_clear = 0;
						bmasks_clear = 0;
						/* Don't merge defined and undefined values */
						break;
					}

					for (k = 0, wa = a->words, wb = b->words; k < a->word_count;
					     k++, wa++, wb++) {
						m = wa->mask & wb->mask;

						if ((!vundef) && (m) &&
						    ((wa->value & m) != (wb->value & m))) {
							/* Conflict, user should not have allowed
							 * this
							 */
							return NFP_ERRNO(EINVAL);
						}

						if (k == 0) {
							if (can_merge)
								merge = can_merge(table_id, a, b);
							else
								merge = 1;

							if (merge < 0)
								return NFP_ERRNO(EPERM);

							if (merge == NFP_REGDB_CAN_MERGE_RET_NO)
								break;
						}

						/* Move all B bits to A */
						wa->mask |= wb->mask;
						wa->value |= wb->value;
						wb->mask = 0;
						a->internal_flags |= _NRDB_FLG_CHANGED;
						b->internal_flags |= _NRDB_FLG_CHANGED;
						/* bmasks_clear = 1 */
						amasks_clear = 0;
					}
					break;
				case NFP_REGDB_VALTYPE_REQUIRED:
					if ((vundef == 0x1) || (vundef == 0x2)) {
						amasks_clear = 0;
						bmasks_clear = 0;
						/* Don't merge defined and undefined values */
						break;
					}

					for (k = 0, wa = a->words, wb = b->words; k < a->word_count;
					     k++, wa++, wb++) {
						m = wa->mask & wb->mask;
						if ((!vundef) && (m) &&
						    ((wa->value & m) != (wb->value & m))) {
							/* Conflict, user should not have allowed
							 * this
							 */
							return NFP_ERRNO(EINVAL);
						}

						if (k == 0) {
							if (can_merge)
								merge = can_merge(table_id, a, b);
							else
								merge = 1;

							if (merge < 0)
								return NFP_ERRNO(EPERM);

							if (!merge)
								break;
						}

						/* Clear bits that the const ensures */
						wb->mask &= ~m;
						wb->value &= ~m;
						b->internal_flags |= _NRDB_FLG_CHANGED;
						bmasks_clear = bmasks_clear && (!wb->mask);
						amasks_clear = 0;
					}
					break;
				case NFP_REGDB_VALTYPE_VOLATILE_INIT:
				case NFP_REGDB_VALTYPE_VOLATILE_NOINIT:
					for (k = 0, wa = a->words, wb = b->words; k < a->word_count;
					     k++, wa++, wb++) {
						m = wa->mask & wb->mask;
						if (m) {
							/* Conflict, user should not have allowed
							 * this
							 */
							return NFP_ERRNO(EINVAL);
						}
						bmasks_clear = 0;
						amasks_clear = 0;
					}
					break;
				case NFP_REGDB_VALTYPE_INVALID:
					if ((vundef == 0x1) || (vundef == 0x2)) {
						amasks_clear = 0;
						bmasks_clear = 0;
						/* Don't merge defined and undefined values */
						break;
					}

					for (k = 0, wa = a->words, wb = b->words; k < a->word_count;
					     k++, wa++, wb++) {
						m = wa->mask & wb->mask;
						if ((!vundef) && (m) &&
						    ((wa->value & m) == (wb->value & m))) {
							/* Conflict, user should not have allowed
							 * this
							 */
							return NFP_ERRNO(EINVAL);
						}

						if (k == 0) {
							if (can_merge)
								merge = can_merge(table_id, a, b);
							else
								merge = 1;

							if (merge < 0)
								return NFP_ERRNO(EPERM);

							if (!merge)
								break;
						}

						/* Clear bits that the const ensures */
						wb->mask &= ~m;
						wb->value &= ~m;
						b->internal_flags |= _NRDB_FLG_CHANGED;
						bmasks_clear = bmasks_clear && (!wb->mask);
						amasks_clear = 0;
					}
					break;
				default:
					return NFP_ERRNO(EINVAL);
				}
				break;
			case NFP_REGDB_VALTYPE_REQUIRED:
				switch (vtp_b) {
				case NFP_REGDB_VALTYPE_REQUIRED:
					if ((vundef == 0x1) || (vundef == 0x2)) {
						amasks_clear = 0;
						bmasks_clear = 0;
						/* Don't merge defined and undefined values */
						break;
					}

					for (k = 0, wa = a->words, wb = b->words; k < a->word_count;
					     k++, wa++, wb++) {
						m = wa->mask & wb->mask;
						if ((!vundef) && (m) &&
						    ((wa->value & m) != (wb->value & m))) {
							/* Conflict, user should not have allowed
							 * this
							 */
							return NFP_ERRNO(EINVAL);
						}

						if (k == 0) {
							if (can_merge)
								merge = can_merge(table_id, a, b);
							else
								merge = 1;

							if (merge < 0)
								return NFP_ERRNO(EPERM);

							if (!merge)
								break;
						}

						/* Move all B bits to A */
						wa->mask |= wb->mask;
						wa->value |= wb->value;
						wb->mask = 0;
						a->internal_flags |= _NRDB_FLG_CHANGED;
						b->internal_flags |= _NRDB_FLG_CHANGED;
						/* bmasks_clear = 1 */
						amasks_clear = 0;
					}
					break;
				case NFP_REGDB_VALTYPE_VOLATILE_INIT:
				case NFP_REGDB_VALTYPE_VOLATILE_NOINIT:
					for (k = 0, wa = a->words, wb = b->words; k < a->word_count;
					     k++, wa++, wb++) {
						m = wa->mask & wb->mask;
						if (m) {
							/* Conflict, user should not have allowed
							 * this
							 */
							return NFP_ERRNO(EINVAL);
						}
						bmasks_clear = 0;
						amasks_clear = 0;
					}
					break;
				case NFP_REGDB_VALTYPE_INVALID:
					if ((vundef == 0x1) || (vundef == 0x2)) {
						amasks_clear = 0;
						bmasks_clear = 0;
						/* Don't merge defined and undefined values */
						break;
					}

					for (k = 0, wa = a->words, wb = b->words; k < a->word_count;
					     k++, wa++, wb++) {
						m = wa->mask & wb->mask;
						if ((!vundef) && (m) &&
						    ((wa->value & m) == (wb->value & m))) {
							/* Conflict, user should not have allowed
							 * this
							 */
							return NFP_ERRNO(EINVAL);
						}

						if (k == 0) {
							if (can_merge)
								merge = can_merge(table_id, a, b);
							else
								merge = 1;

							if (merge < 0)
								return NFP_ERRNO(EPERM);

							if (!merge)
								break;
						}

						/* Clear bits that the const ensures */
						wb->mask &= ~m;
						wb->value &= ~m;
						b->internal_flags |= _NRDB_FLG_CHANGED;
						bmasks_clear = bmasks_clear && (!wb->mask);
						amasks_clear = 0;
					}
					break;
				default:
					return NFP_ERRNO(EINVAL);
				}
				break;
			case NFP_REGDB_VALTYPE_VOLATILE_INIT:
				switch (vtp_b) {
				case NFP_REGDB_VALTYPE_VOLATILE_INIT:
					if ((vundef == 0x1) || (vundef == 0x2)) {
						amasks_clear = 0;
						bmasks_clear = 0;
						/* Don't merge defined and undefined values */
						break;
					}

					for (k = 0, wa = a->words, wb = b->words; k < a->word_count;
					     k++, wa++, wb++) {
						m = wa->mask & wb->mask;
						if ((!vundef) && (m) &&
						    ((wa->value & m) != (wb->value & m))) {
							/* Conflict, user should not have allowed
							 * this
							 */
							return NFP_ERRNO(EINVAL);
						}

						if (k == 0) {
							if (can_merge)
								merge = can_merge(table_id, a, b);
							else
								merge = 1;

							if (merge < 0)
								return NFP_ERRNO(EPERM);

							if (!merge)
								break;
						}

						/* Move all B bits to A */
						wa->mask |= wb->mask;
						wa->value |= wb->value;
						wb->mask = 0;
						a->internal_flags |= _NRDB_FLG_CHANGED;
						b->internal_flags |= _NRDB_FLG_CHANGED;
						/* bmasks_clear = 1 */
						amasks_clear = 0;
					}
					break;
				case NFP_REGDB_VALTYPE_VOLATILE_NOINIT:
					/* B is noinit, so no value, so A can be defined or
					 * undefined for a merge.
					 */
					for (k = 0, wa = a->words, wb = b->words; k < a->word_count;
					     k++, wa++, wb++) {
						m = wa->mask & wb->mask;

						if (k == 0) {
							if (can_merge)
								merge = can_merge(table_id, a, b);
							else
								merge = 1;

							if (merge < 0)
								return NFP_ERRNO(EPERM);

							if (!merge)
								break;
						}

						/* Clear bits that the const ensures */
						wb->mask &= ~m;
						wb->value &= ~m;
						b->internal_flags |= _NRDB_FLG_CHANGED;
						bmasks_clear = bmasks_clear && (!wb->mask);
						amasks_clear = 0;
					}
					break;
				case NFP_REGDB_VALTYPE_INVALID:
					for (k = 0, wa = a->words, wb = b->words; k < a->word_count;
					     k++, wa++, wb++) {
						m = wa->mask & wb->mask;
						if (m) {
							/* Conflict, user should not have allowed
							 * this
							 */
							return NFP_ERRNO(EINVAL);
						}
						bmasks_clear = 0;
						amasks_clear = 0;
					}
					break;
				default:
					return NFP_ERRNO(EINVAL);
				}
				break;
			case NFP_REGDB_VALTYPE_VOLATILE_NOINIT:
				switch (vtp_b) {
				case NFP_REGDB_VALTYPE_VOLATILE_NOINIT:
					for (k = 0, wa = a->words, wb = b->words; k < a->word_count;
					     k++, wa++, wb++) {
						m = wa->mask & wb->mask;

						if (k == 0) {
							if (can_merge)
								merge = can_merge(table_id, a, b);
							else
								merge = 1;

							if (merge < 0)
								return NFP_ERRNO(EPERM);

							if (!merge)
								break;
						}

						/* Move all B bits to A */
						wa->mask |= wb->mask;
						wa->value |= wb->value;
						wb->mask = 0;
						a->internal_flags |= _NRDB_FLG_CHANGED;
						b->internal_flags |= _NRDB_FLG_CHANGED;
						/* bmasks_clear = 1 */
						amasks_clear = 0;
					}
					break;
				case NFP_REGDB_VALTYPE_INVALID:
					for (k = 0, wa = a->words, wb = b->words; k < a->word_count;
					     k++, wa++, wb++) {
						m = wa->mask & wb->mask;
						if (m) {
							/* Conflict, user should not have allowed
							 * this
							 */
							return NFP_ERRNO(EINVAL);
						}
						bmasks_clear = 0;
						amasks_clear = 0;
					}
					break;
				default:
					return NFP_ERRNO(EINVAL);
				}
				break;
			case NFP_REGDB_VALTYPE_INVALID:
				switch (vtp_b) {
				case NFP_REGDB_VALTYPE_INVALID:
					if ((vundef == 0x1) || (vundef == 0x2)) {
						amasks_clear = 0;
						bmasks_clear = 0;
						/* Don't merge defined and undefined values */
						break;
					}

					{
						int no_overlap = 1;
						int a_in_b = 1;
						int b_in_a = 1;

						for (k = 0, wa = a->words, wb = b->words;
						     k < a->word_count; k++, wa++, wb++) {
							m = wa->mask & wb->mask;
							no_overlap = no_overlap && (!m);
							b_in_a = b_in_a && (m == wb->mask) &&
								 ((vundef) ||
								  ((wa->value & m) == wb->value));
							a_in_b = a_in_b && (m == wa->mask) &&
								 ((vundef) ||
								  ((wb->value & m) == wa->value));
						}

						if (no_overlap || b_in_a || a_in_b) {
							if (can_merge)
								merge = can_merge(table_id, a, b);
							else
								merge = 1;

							if (merge < 0)
								return NFP_ERRNO(EPERM);
						}

						if (merge) {
							for (k = 0, wa = a->words, wb = b->words;
							     k < a->word_count; k++, wa++, wb++) {
								if (no_overlap) {
									wa->mask |= wb->mask;
									wa->value |= wb->value;
									wb->mask = 0;
									a->internal_flags |=
										_NRDB_FLG_CHANGED;
									b->internal_flags |=
										_NRDB_FLG_CHANGED;
									amasks_clear = 0;
								} else if (b_in_a) {
									wb->mask = 0;
									wb->value = 0;
									b->internal_flags |=
										_NRDB_FLG_CHANGED;
									amasks_clear = 0;
									/* bmasks_clear = 0; */
								} else if (a_in_b) {
									wa->mask = 0;
									wa->value = 0;
									a->internal_flags |=
										_NRDB_FLG_CHANGED;
									bmasks_clear = 0;
									/* amasks_clear = 0; */
								}
							}
						}
					}
					break;
				default:
					return NFP_ERRNO(EINVAL);
				}
				break;
				break;
			default:
				return NFP_ERRNO(EINVAL);
			}

			if (merge) {
				if (amasks_clear)
					a->internal_flags |= _NRDB_FLG_REMOVE;
				if (bmasks_clear)
					b->internal_flags |= _NRDB_FLG_REMOVE;
			}
		}
	}

	if (removing) {
		a = tbl->tblents.entry;
		for (i = 0; i < (tbl->tblents.count - 1); i++, a++) {
			if (!(a->internal_flags & _NRDB_FLG_CHANGED))
				continue;
			removing(table_id, a);
		}
	}

	a = tbl->tblents.entry;
	k = 0;
	for (i = 0, b = a; i < tbl->tblents.count; i++, b++) {
		if (b->internal_flags & _NRDB_FLG_REMOVE) {
			k++;
			_nfp_regdb_entry_free(b);
			continue;
		};

		if (a == b) {
			a++;
			continue;
		}

		memcpy(a, b, sizeof(*a));
		a++;
	}
	tbl->tblents.count -= k;

	/* We should still be sorted, but make sure we are */
	qsort(tbl->tblents.entry, tbl->tblents.count, sizeof(*tbl->tblents.entry),
	      _nfp_regdb_entry_sortcmp);

	return 0;
}

void nfp_regdb_table_remove(struct nfp_regdb *db,
			    uint32_t table_id,
			    void (*removing)(uint32_t table_id, struct nfp_regdb_entry *entry))
{
	size_t idx;
	struct _nfp_regdb_table *tbl;
	struct _nfp_regdb_table bkey;

	if ((!db) || (!db->tables.count))
		return;

	bkey.table_id = table_id;
	tbl = bsearch(&bkey, db->tables.entry, db->tables.count, sizeof(*db->tables.entry),
		      _nfp_regdb_table_cmp);

	if (!tbl)
		return;
	idx = (tbl - db->tables.entry) + 1;
	NS_LOG(NS_LOG_LVL_VERBOSE, "Removing table 0x%08" PRIx32 " at index %" PRIuSZ, table_id,
	       (idx - 1));

	_nfp_regdb_table_free(tbl, removing);

	if (idx < db->tables.count)
		memmove(tbl, tbl + 1, (db->tables.count - idx) * sizeof(*tbl));
	db->tables.count--;
}

void nfp_regdb_table_remove_by_owner(struct nfp_regdb *db,
				     uint32_t table_owner,
				     void (*removing)(uint32_t table_id,
						      struct nfp_regdb_entry *entry))
{
	uint32_t *table_ids = NULL;
	size_t cnt = nfp_regdb_table_ids(db, &table_ids);
	size_t i;

	for (i = 0; i < cnt; i++) {
		uint32_t tblid = table_ids[i];

		if ((table_owner == NFP_REGDB_TABLE_ID_OWNER_ANY) ||
		    (table_owner == NFP_REGDB_TABLE_ID_OWNER_of(tblid))) {
			/* This table should be removed. */
			nfp_regdb_table_remove(db, tblid, removing);
		}
	}

	if (table_ids)
		free(table_ids);
}

int nfp_regdb_table_validate(struct nfp_regdb *db,
			     uint32_t table_id,
			     int (*usr_conflicts)(const struct nfp_regdb_entry *a,
						  const struct nfp_regdb_entry *b,
						  int same_owner))
{
	size_t i, j;
	struct _nfp_regdb_table *tbl;
	struct _nfp_regdb_table bkey;
	const struct nfp_regdb_entry *a, *b;
	int ret;

	if ((!db) || (!db->tables.count))
		return NFP_ERRNO(ENOENT);

	bkey.table_id = table_id;
	tbl = bsearch(&bkey, db->tables.entry, db->tables.count, sizeof(*db->tables.entry),
		      _nfp_regdb_table_cmp);

	if (!tbl)
		return NFP_ERRNO(ENOENT);

	if (tbl->tblents.count <= 1)
		return 0;

	a = tbl->tblents.entry;

	/* Assumes sorted entries */
	for (i = 0; i < (tbl->tblents.count - 1); i++, a++) {
		b = a + 1;
		if (_nfp_regdb_entry_idcmp(a, b) != 0)
			continue;
		for (j = i + 1; j < tbl->tblents.count; j++, b++) {
			if (_nfp_regdb_entry_idcmp(a, b) != 0)
				break;
			ret = nfp_regdb_entry_conflicts(a, b, 1, usr_conflicts);
			if (ret)
				return ret;
		}
	}

	return 0;
}

size_t nfp_regdb_table_ids(struct nfp_regdb *db, uint32_t **table_ids)
{
	if (table_ids)
		*table_ids = NULL;

	if (!db)
		return 0;

	if (table_ids && (db->tables.count > 0)) {
		size_t idx;
		*table_ids = calloc(db->tables.count, sizeof(**table_ids));
		if (!*table_ids)
			return 0;

		for (idx = 0; idx < db->tables.count; idx++)
			(*table_ids)[idx] = db->tables.entry[idx].table_id;
	}

	return db->tables.count;
}

size_t nfp_regdb_table_entries(struct nfp_regdb *db,
			       uint32_t table_id,
			       const struct nfp_regdb_entry **entries)
{
	struct _nfp_regdb_table *tbl;
	struct _nfp_regdb_table bkey;

	if ((!db) || (!db->tables.count))
		return 0;

	bkey.table_id = table_id;
	tbl = bsearch(&bkey, db->tables.entry, db->tables.count, sizeof(*db->tables.entry),
		      _nfp_regdb_table_cmp);

	if (!tbl)
		return 0;

	if (entries)
		*entries = tbl->tblents.entry;

	return tbl->tblents.count;
}

int nfp_regdb_word_conflicts(const struct nfp_regdb_entry_word *existing,
			     const struct nfp_regdb_entry_word *incoming,
			     int same_owner)
{
	uint32_t vex, vin;
	uint32_t mask, val_match;
	enum nfp_regdb_valtype vtp_ex, vtp_in;
	uint32_t vundef;

	if ((!existing) || (!incoming))
		return NFP_ERRNO(EINVAL);

	vex = NFP_REGDB_ENTRY_WORD_W0_OHI_of(existing->w0);
	vin = NFP_REGDB_ENTRY_WORD_W0_OHI_of(incoming->w0);
	if (vex != vin)
		return 0;

	if (existing->offset_lo != incoming->offset_lo)
		return 0;

	vex = NFP_REGDB_ENTRY_WORD_W0_CTX_of(existing->w0);
	vin = NFP_REGDB_ENTRY_WORD_W0_CTX_of(incoming->w0);
	if (vex != vin)
		return 0;

	mask = existing->mask & incoming->mask;
	if (!mask)
		return 0;
	val_match = (existing->value & mask) == (incoming->value & mask);

	vundef = (NFP_REGDB_ENTRY_WORD_W0_VUN_of(existing->w0) ||
		  NFP_REGDB_ENTRY_WORD_W0_VUN_of(incoming->w0));

	vtp_ex = (enum nfp_regdb_valtype)NFP_REGDB_ENTRY_WORD_W0_VTP_of(existing->w0);
	vtp_in = (enum nfp_regdb_valtype)NFP_REGDB_ENTRY_WORD_W0_VTP_of(incoming->w0);

	switch (vtp_ex) {
	case NFP_REGDB_VALTYPE_CONST:
		switch (vtp_in) {
		case NFP_REGDB_VALTYPE_CONST:
		case NFP_REGDB_VALTYPE_REQUIRED:
			return (!vundef) && (!val_match);
		case NFP_REGDB_VALTYPE_VOLATILE_INIT:
		case NFP_REGDB_VALTYPE_VOLATILE_NOINIT:
			return 1;
		case NFP_REGDB_VALTYPE_INVALID:
			return (!vundef) && (!!val_match);
		default:
			return 1;
		}
		break;
	case NFP_REGDB_VALTYPE_REQUIRED:
		switch (vtp_in) {
		case NFP_REGDB_VALTYPE_CONST:
			/* Incoming cannot take 'const' ownership if an existing entry claimed
			 * 'required'.
			 */
			if (!same_owner)
				return 1;
			return (!vundef) && (!val_match);
		case NFP_REGDB_VALTYPE_REQUIRED:
			return (!vundef) && (!val_match);
		case NFP_REGDB_VALTYPE_VOLATILE_INIT:
		case NFP_REGDB_VALTYPE_VOLATILE_NOINIT:
			return 1;
		case NFP_REGDB_VALTYPE_INVALID:
			return (!vundef) && (!!val_match);
		default:
			return 1;
		}
		break;
	case NFP_REGDB_VALTYPE_VOLATILE_INIT:
		switch (vtp_in) {
		case NFP_REGDB_VALTYPE_CONST:
		case NFP_REGDB_VALTYPE_REQUIRED:
			return 1;
		case NFP_REGDB_VALTYPE_VOLATILE_INIT:
			if (!same_owner)
				return 1;
			return (!vundef) && (!val_match);
		case NFP_REGDB_VALTYPE_VOLATILE_NOINIT:
			if (!same_owner)
				return 1;
			return 0;
		case NFP_REGDB_VALTYPE_INVALID:
			return 1;
		default:
			return 1;
		}
		break;
	case NFP_REGDB_VALTYPE_VOLATILE_NOINIT:
		switch (vtp_in) {
		case NFP_REGDB_VALTYPE_CONST:
		case NFP_REGDB_VALTYPE_REQUIRED:
			return 1;
		case NFP_REGDB_VALTYPE_VOLATILE_INIT:
			if (!same_owner)
				return 1;
			return 0;
		case NFP_REGDB_VALTYPE_VOLATILE_NOINIT:
			if (!same_owner)
				return 1;
			return 0;
		case NFP_REGDB_VALTYPE_INVALID:
			return 1;
		default:
			return 1;
		}
		break;
	case NFP_REGDB_VALTYPE_INVALID:
		switch (vtp_in) {
		case NFP_REGDB_VALTYPE_CONST:
		case NFP_REGDB_VALTYPE_REQUIRED:
			return (!vundef) && (!!val_match);
		case NFP_REGDB_VALTYPE_VOLATILE_INIT:
		case NFP_REGDB_VALTYPE_VOLATILE_NOINIT:
			return 1;
		case NFP_REGDB_VALTYPE_INVALID:
			return 0;
		default:
			return 1;
		}
		break;
	default:
		break;
	}

	return 1;
}

int nfp_regdb_entry_conflicts(const struct nfp_regdb_entry *existing,
			      const struct nfp_regdb_entry *incoming,
			      int same_owner,
			      int (*usr_conflicts)(const struct nfp_regdb_entry *existing,
						   const struct nfp_regdb_entry *incoming,
						   int same_owner))
{
	int ret;
	int mask_overlap = 0;
	size_t widx;

	if ((!existing) || (!incoming))
		return NFP_ERRNO(EINVAL);

	if (_nfp_regdb_entry_idcmp(existing, incoming) != 0)
		return 0;

	if (existing->word_count != incoming->word_count)
		return 1;

	for (widx = 0; widx < existing->word_count; widx++) {
		mask_overlap = (existing->words[widx].mask & incoming->words[widx].mask) != 0;
		ret = nfp_regdb_word_conflicts(&existing->words[widx], &incoming->words[widx],
					       same_owner);
		if (ret)
			return ret;
	}

	if (usr_conflicts && mask_overlap && usr_conflicts(existing, incoming, same_owner))
		return 1;

	return 0;
}

static int _nfp_regdb_entry_validate(struct _nfp_regdb_table *tbl,
				     int same_owner,
				     const struct nfp_regdb_entry_word *words,
				     uint8_t word_count,
				     void *usrdata,
				     int (*usr_conflicts)(const struct nfp_regdb_entry *existing,
							  const struct nfp_regdb_entry *incoming,
							  int same_owner),
				     const struct nfp_regdb_entry **conflict)
{
	const struct nfp_regdb_entry *enta = NULL;
	struct nfp_regdb_entry entkey;
	int ret;

	entkey.words = (struct nfp_regdb_entry_word *)words;
	entkey.word_count = word_count;
	entkey.internal_flags = 0;
	entkey.usrdata = usrdata;
	if (tbl->tblents.count) {
		enta = bsearch(&entkey, tbl->tblents.entry, tbl->tblents.count,
			       sizeof(*tbl->tblents.entry), _nfp_regdb_entry_idcmp);
	}

	if (!enta)
		return 0; /* No existing entries for register */

	/* Go to first entry for register */
	while ((enta > tbl->tblents.entry) && (_nfp_regdb_entry_idcmp(enta, enta - 1) == 0))
		enta--;

	/* Assumes sorted entries */
	for (; enta < (tbl->tblents.entry + tbl->tblents.count); enta++) {
		if (_nfp_regdb_entry_idcmp(enta, &entkey) != 0)
			break;
		ret = nfp_regdb_entry_conflicts(enta, &entkey, same_owner, usr_conflicts);
		if (ret) {
			if ((conflict) && (ret == 1))
				*conflict = enta;
			return ret;
		}
	}

	return 0;
}

int nfp_regdb_entry_validate(struct nfp_regdb *db,
			     uint32_t table_id,
			     int all_tables,
			     const struct nfp_regdb_entry_word *words,
			     uint8_t word_count,
			     void *usrdata,
			     int (*usr_conflicts)(const struct nfp_regdb_entry *existing,
						  const struct nfp_regdb_entry *incoming,
						  int same_owner),
			     const struct nfp_regdb_entry **conflict,
			     int *conflict_owner)
{
	int ret;
	struct _nfp_regdb_table *tbl = NULL;
	struct _nfp_regdb_table bkey;
	uint32_t in_owner = NFP_REGDB_TABLE_ID_OWNER_of(table_id);

	if (conflict)
		*conflict = NULL;

	if (!db)
		return NFP_ERRNO(EINVAL);

	if (db->tables.count == 0)
		return 0;

	if (in_owner == NFP_REGDB_TABLE_ID_OWNER_ANY)
		all_tables = 1;

	bkey.table_id = table_id;
	/**** Single table only ****/
	if (!all_tables) {
		tbl = bsearch(&bkey, db->tables.entry, db->tables.count, sizeof(*db->tables.entry),
			      _nfp_regdb_table_cmp);
		if (!tbl)
			return 0;
		if (conflict_owner)
			*conflict_owner = NFP_REGDB_TABLE_ID_OWNER_of(tbl->table_id);
		return _nfp_regdb_entry_validate(tbl, 1, words, word_count, usrdata, usr_conflicts,
						 conflict);
	}

	/**** All matching tables regardless of owner ****/
	tbl = bsearch(&bkey, db->tables.entry, db->tables.count, sizeof(*db->tables.entry),
		      _nfp_regdb_table_cmp_noowner);
	if (!tbl)
		return 0;

	/* Go to the first table with the same ID, ignoring owner */
	while ((tbl > db->tables.entry) && (NFP_REGDB_TABLE_ID_NO_OWNER((tbl - 1)->table_id) ==
					    NFP_REGDB_TABLE_ID_NO_OWNER(table_id)))
		tbl--;

	/* Then validate for each matching table */
	ret = 0;
	for (;
	     (tbl < (db->tables.entry + db->tables.count)) &&
	     (NFP_REGDB_TABLE_ID_NO_OWNER(tbl->table_id) == NFP_REGDB_TABLE_ID_NO_OWNER(table_id));
	     tbl++) {
		int same_owner = (in_owner != NFP_REGDB_TABLE_ID_OWNER_ANY) &&
				 (NFP_REGDB_TABLE_ID_OWNER_of(tbl->table_id) == in_owner);

		if (conflict_owner)
			*conflict_owner = NFP_REGDB_TABLE_ID_OWNER_of(tbl->table_id);
		ret = _nfp_regdb_entry_validate(tbl, same_owner, words, word_count, usrdata,
						usr_conflicts, conflict);
		if (ret != 0)
			return ret;
	}

	return 0;
}

int nfp_regdb_entry_add(struct nfp_regdb *db,
			uint32_t table_id,
			const struct nfp_regdb_entry_word *words,
			uint8_t word_count,
			void *usrdata)
{
	int err = EINVAL;
	uint8_t widx;
	struct _nfp_regdb_table *tbl = NULL;
	struct _nfp_regdb_table bkey;
	size_t ent_insert;
	struct nfp_regdb_entry ent;
	struct nfp_regdb_entry *tblent;

	if (!db)
		goto error_cleanup;

	bkey.table_id = table_id;
	if (db->tables.count) {
		tbl = bsearch(&bkey, db->tables.entry, db->tables.count, sizeof(*db->tables.entry),
			      _nfp_regdb_table_cmp);
	}

	if (!tbl) {
		if (db->tables.count == db->tables.capacity)
			_ARRAY_EXTEND(&db->tables, 8);

		tbl = &db->tables.entry[db->tables.count++];
		memset(tbl, 0, sizeof(*tbl));
		tbl->table_id = table_id;
		qsort(db->tables.entry, db->tables.count, sizeof(*db->tables.entry),
		      _nfp_regdb_table_cmp);
		tbl = bsearch(&bkey, db->tables.entry, db->tables.count, sizeof(*db->tables.entry),
			      _nfp_regdb_table_cmp);
		if (!tbl) {
			err = EFAULT;
			goto error_cleanup;
		}
		NS_LOG(NS_LOG_LVL_VERBOSE, "Created new table 0x%08" PRIx32, table_id);
	}

	/* Make sure the array can take a new entry */
	if (tbl->tblents.count == tbl->tblents.capacity)
		_ARRAY_EXTEND(&tbl->tblents, 32);


	/* Prepare the new entry */
	memset(&ent, 0, sizeof(ent));
	ent.usrdata = usrdata;
	ent.word_count = word_count;
	_CALLOC_OOM(ent.words, word_count, sizeof(*ent.words));

	memcpy(ent.words, words, sizeof(*words) * word_count);

	ent.words[0].w0 &= ~NFP_REGDB_ENTRY_WORD_W0_LWO(1);
	ent.words[0].w0 |= NFP_REGDB_ENTRY_WORD_W0_LWO(word_count == 1);
	ent.words[0].value &= ent.words[0].mask;
	for (widx = 1; widx < word_count; widx++) {
		ent.words[widx].w0 = ent.words[0].w0;
		if (widx == word_count - 1)
			ent.words[widx].w0 |= NFP_REGDB_ENTRY_WORD_W0_LWO(1);
		ent.words[widx].offset_lo = ent.words[0].offset_lo;
		ent.words[widx].value &= ent.words[widx].mask;
	}

	/* Find insertion point, which may be past the last entry */
	ent_insert = _find_insert(&ent, tbl->tblents.entry, tbl->tblents.count,
				  sizeof(*tbl->tblents.entry),
				  _nfp_regdb_entry_sortcmp);

	tblent = tbl->tblents.entry + ent_insert;

	/* If we are not appending, move the right side of the array up a slot.
	 * This is overlapping, so memmove and not memcpy.
	 */
	if (ent_insert < tbl->tblents.count)
		memmove(tblent + 1, tblent,
			sizeof(*tblent) * (tbl->tblents.count - ent_insert));

	tbl->tblents.count++;
	memcpy(tblent, &ent, sizeof(ent));

	return 0;

error_cleanup:
	return NFP_ERRNO(err);
}

static int _nfp_regdb_val_get(struct _nfp_regdb_table *tbl,
			      uint64_t offset,
			      int8_t ctx,
			      const uint32_t *masks,
			      uint32_t *vals,
			      uint32_t *undefined_masks,
			      uint8_t word_cnt,
			      enum nfp_regdb_valtype *set_type)
{
	int err = EINVAL;
	uint8_t widx;
	struct nfp_regdb_entry *enta;
	struct nfp_regdb_entry entkey;

	if (tbl->tblents.count == 0)
		return 0;

	entkey.word_count = word_cnt;
	entkey.usrdata = NULL;
	entkey.internal_flags = 0;
	_CALLOC_OOM(entkey.words, word_cnt, sizeof(*entkey.words));
	for (widx = 0; widx < word_cnt; widx++) {
		entkey.words[widx].mask = masks[widx];
		entkey.words[widx].offset_lo = (uint32_t)offset;
		entkey.words[widx].w0 = NFP_REGDB_ENTRY_WORD_W0(
			(uint32_t)((offset >> 32) & 0xFF), (ctx > 0) ? ctx : 0,
			NFP_REGDB_VALTYPE_NONE, (widx == (word_cnt - 1)), 0);
	}

	enta = bsearch(&entkey, tbl->tblents.entry, tbl->tblents.count, sizeof(*tbl->tblents.entry),
		       _nfp_regdb_entry_idcmp);

	if (!enta) {
		free(entkey.words);
		return 0; /* No existing entries for register */
	}

	/* Go to first entry for register */
	while ((enta > tbl->tblents.entry) && (_nfp_regdb_entry_idcmp(enta, enta - 1) == 0))
		enta--;

	/* Assumes sorted entries */
	for (; enta < (tbl->tblents.entry + tbl->tblents.count); enta++) {
		enum nfp_regdb_valtype set_type_ent;
		int vundef;
		int mask_overlap = 0;
		uint8_t wcnt = NFP_MIN(enta->word_count, word_cnt);

		if (_nfp_regdb_entry_idcmp(enta, &entkey) != 0)
			break;

		for (widx = 0; widx < wcnt; widx++) {
			if (enta->words[widx].mask & masks[widx]) {
				mask_overlap = 1;
				break;
			}
		}

		if (!mask_overlap)
			continue;

		set_type_ent = NFP_REGDB_ENTRY_WORD_W0_VTP_of(enta->words[0].w0);
		vundef = NFP_REGDB_ENTRY_WORD_W0_VUN_of(enta->words[0].w0);
		if ((set_type) && (nfp_regdb_val_type_cmp(*set_type, set_type_ent) == 1))
			*set_type = set_type_ent;

		if (((set_type_ent == NFP_REGDB_VALTYPE_CONST) ||
		     (set_type_ent == NFP_REGDB_VALTYPE_REQUIRED)) &&
		    (!vundef)) {
			for (widx = 0; widx < wcnt; widx++) {
				if (vals)
					vals[widx] |= enta->words[widx].value;
				if (undefined_masks) {
					/* Clear mask bits for defined values so we know which bits
					 * are set const/required with undefined values.
					 */
					undefined_masks[widx] &= ~(enta->words[widx].mask);
				}
			}
		}
	}

	free(entkey.words);
	return 0;

error_cleanup:
	if (entkey.words)
		free(entkey.words);
	return NFP_ERRNO(err);
}

int nfp_regdb_val_get(struct nfp_regdb *db,
		      uint32_t table_id,
		      uint64_t offset,
		      int8_t ctx,
		      const uint32_t *masks,
		      uint32_t *vals,
		      uint8_t word_cnt,
		      enum nfp_regdb_valtype *set_type,
		      int *is_undefined)
{
	int err = EINVAL;
	struct _nfp_regdb_table *tbl = NULL;
	struct _nfp_regdb_table bkey;
	int ret;
	uint32_t *undefined_masks = NULL;
	size_t widx;

	if (set_type)
		*set_type = NFP_REGDB_VALTYPE_NONE;
	if (is_undefined)
		*is_undefined = 0;

	if (vals)
		for (widx = 0; widx < word_cnt; widx++)
			vals[widx] = 0;

	if ((!db) || (!masks))
		goto error_cleanup;
	if (db->tables.count == 0)
		goto success_cleanup;

	if (is_undefined) {
		_CALLOC_OOM(undefined_masks, word_cnt, sizeof(*undefined_masks));
		memcpy(undefined_masks, masks, (word_cnt * sizeof(*undefined_masks)));
	}

	bkey.table_id = table_id;
	/**** Single table only ****/
	if (NFP_REGDB_TABLE_ID_OWNER_of(table_id) != NFP_REGDB_TABLE_ID_OWNER_ANY) {
		tbl = bsearch(&bkey, db->tables.entry, db->tables.count, sizeof(*db->tables.entry),
			      _nfp_regdb_table_cmp);
		if (!tbl)
			goto success_cleanup;
		ret = _nfp_regdb_val_get(tbl, offset, ctx, masks, vals, undefined_masks, word_cnt,
					 set_type);
		if (ret != 0) {
			err = errno;
			goto error_cleanup;
		}
		goto success_cleanup;
	}

	/**** All matching tables regardless of owner ****/
	tbl = bsearch(&bkey, db->tables.entry, db->tables.count, sizeof(*db->tables.entry),
		      _nfp_regdb_table_cmp_noowner);
	if (!tbl)
		return 0;

	/* Go to the first table with the same ID, ignoring owner */
	while ((tbl > db->tables.entry) && (NFP_REGDB_TABLE_ID_NO_OWNER((tbl - 1)->table_id) ==
					    NFP_REGDB_TABLE_ID_NO_OWNER(table_id)))
		tbl--;

	/* Then val_get for each matching table */
	ret = 0;
	for (;
	     (tbl < (db->tables.entry + db->tables.count)) &&
	     (NFP_REGDB_TABLE_ID_NO_OWNER(tbl->table_id) == NFP_REGDB_TABLE_ID_NO_OWNER(table_id));
	     tbl++) {
		ret = _nfp_regdb_val_get(tbl, offset, ctx, masks, vals, undefined_masks, word_cnt,
					 set_type);
		if (ret != 0) {
			err = errno;
			goto error_cleanup;
		}
	}

success_cleanup:
	if (is_undefined) {
		/* Check if we have any bits left that are set but with undefined values. */
		for (widx = 0; widx < word_cnt; widx++) {
			if (undefined_masks[widx]) {
				*is_undefined = 1;
				break;
			}
		}

		free(undefined_masks);
	}
	return 0;

error_cleanup:
	if (undefined_masks)
		free(undefined_masks);
	return NFP_ERRNO(err);
}

int nfp_regdb_val_type_cmp(enum nfp_regdb_valtype a, enum nfp_regdb_valtype b)
{
	/* This cmp function make no assumption about the value of the enum names */
	if (a == b)
		return 0;

	switch (a) {
	case NFP_REGDB_VALTYPE_CONST:
		switch (b) {
		case NFP_REGDB_VALTYPE_REQUIRED:
		case NFP_REGDB_VALTYPE_VOLATILE_INIT:
		case NFP_REGDB_VALTYPE_VOLATILE_NOINIT:
		case NFP_REGDB_VALTYPE_INVALID:
			return -1;
		default:
			return 0;
		}
	case NFP_REGDB_VALTYPE_REQUIRED:
		switch (b) {
		case NFP_REGDB_VALTYPE_CONST:
			return 1;
		case NFP_REGDB_VALTYPE_VOLATILE_INIT:
		case NFP_REGDB_VALTYPE_VOLATILE_NOINIT:
		case NFP_REGDB_VALTYPE_INVALID:
			return -1;
		default:
			return 0;
		}
	case NFP_REGDB_VALTYPE_VOLATILE_INIT:
		switch (b) {
		case NFP_REGDB_VALTYPE_CONST:
		case NFP_REGDB_VALTYPE_REQUIRED:
			return 1;
		case NFP_REGDB_VALTYPE_VOLATILE_NOINIT:
		case NFP_REGDB_VALTYPE_INVALID:
			return -1;
		default:
			return 0;
		}
	case NFP_REGDB_VALTYPE_VOLATILE_NOINIT:
		switch (b) {
		case NFP_REGDB_VALTYPE_CONST:
		case NFP_REGDB_VALTYPE_REQUIRED:
		case NFP_REGDB_VALTYPE_VOLATILE_INIT:
			return 1;
		case NFP_REGDB_VALTYPE_INVALID:
			return -1;
		default:
			return 0;
		}
	case NFP_REGDB_VALTYPE_INVALID:
		switch (b) {
		case NFP_REGDB_VALTYPE_CONST:
		case NFP_REGDB_VALTYPE_REQUIRED:
		case NFP_REGDB_VALTYPE_VOLATILE_INIT:
		case NFP_REGDB_VALTYPE_VOLATILE_NOINIT:
			return 1;
		default:
			return 0;
		}
	case NFP_REGDB_VALTYPE_NONE:
		switch (b) {
		case NFP_REGDB_VALTYPE_CONST:
		case NFP_REGDB_VALTYPE_REQUIRED:
		case NFP_REGDB_VALTYPE_VOLATILE_INIT:
		case NFP_REGDB_VALTYPE_VOLATILE_NOINIT:
		case NFP_REGDB_VALTYPE_INVALID:
			return 1;
		default:
			return 0;
		}
	default:
		return 0;
	}
}

int nfp_regdb_entry_regcmp(const struct nfp_regdb_entry *a, const struct nfp_regdb_entry *b)
{
	uint32_t va, vb;

	if ((!a->words) || (!b->words))
		return (a->words > b->words) - (a->words < b->words);

	va = NFP_REGDB_ENTRY_WORD_W0_OHI_of(a->words[0].w0);
	vb = NFP_REGDB_ENTRY_WORD_W0_OHI_of(b->words[0].w0);
	if (va != vb)
		return (va > vb) - (va < vb);

	va = a->words[0].offset_lo;
	vb = b->words[0].offset_lo;
	if (va != vb)
		return (va > vb) - (va < vb);

	va = NFP_REGDB_ENTRY_WORD_W0_CTX_of(a->words[0].w0);
	vb = NFP_REGDB_ENTRY_WORD_W0_CTX_of(b->words[0].w0);
	if (va != vb)
		return (va > vb) - (va < vb);

	return 0;
}

#ifdef _MSC_VER
#pragma warning(pop)
#endif
