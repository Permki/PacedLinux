/*
 * Copyright (C) 2014,2017 Netronome Systems, Inc.  All rights reserved.
 *
 * NFP Register database API for Init-CSR validation.
 *
 */

#ifndef __NFP_REGDB_H__
#define __NFP_REGDB_H__

#include <stdint.h>
#include <nfp-common/nfp_platform.h>
#include <nfp-common/nfp_resid.h>

#include "_nfp_common_apiex.h"

__BEGIN_DECLS

#define NFP_REGDB_TABLE_ID_ISLAND_of(_a_) (((_a_) >> 26) & 0x3F)
#define NFP_REGDB_TABLE_ID_TARGET_of(_a_) (((_a_) >> 22) & 0xF)
#define NFP_REGDB_TABLE_ID_WRACT_of(_a_) (((_a_) >> 17) & 0x1F)
#define NFP_REGDB_TABLE_ID_WRTOK_of(_a_) (((_a_) >> 15) & 0x3)
#define NFP_REGDB_TABLE_ID_RDACT_of(_a_) (((_a_) >> 10) & 0x1F)
#define NFP_REGDB_TABLE_ID_RDTOK_of(_a_) (((_a_) >> 8) & 0x3)
#define NFP_REGDB_TABLE_ID_OWNER_of(_a_) ((_a_) & 0xFF)
#define NFP_REGDB_TABLE_ID_ISLAND(_v_) (((_v_) & 0x3F) << 26)
#define NFP_REGDB_TABLE_ID_TARGET(_v_) (((_v_) & 0xF) << 22)
#define NFP_REGDB_TABLE_ID_WRACT(_v_) (((_v_) & 0x1F) << 17)
#define NFP_REGDB_TABLE_ID_WRTOK(_v_) (((_v_) & 0x3) << 15)
#define NFP_REGDB_TABLE_ID_RDACT(_v_) (((_v_) & 0x1F) << 10)
#define NFP_REGDB_TABLE_ID_RDTOK(_v_) (((_v_) & 0x3) << 8)
#define NFP_REGDB_TABLE_ID_OWNER(_v_) ((_v_) & 0xff)
#define NFP_REGDB_TABLE_ID(_island_, _target_, _wr_act_, _wr_tok_, _rd_act_, \
			   _rd_tok_, _owner_)                                \
	(NFP_REGDB_TABLE_ID_ISLAND(_island_) |                               \
	 NFP_REGDB_TABLE_ID_TARGET(_target_) |                               \
	 NFP_REGDB_TABLE_ID_WRACT(_wr_act_) |                                \
	 NFP_REGDB_TABLE_ID_WRTOK(_wr_tok_) |                                \
	 NFP_REGDB_TABLE_ID_RDACT(_rd_act_) |                                \
	 NFP_REGDB_TABLE_ID_RDTOK(_rd_tok_) |                                \
	 NFP_REGDB_TABLE_ID_OWNER(_owner_))

#define NFP_REGDB_TABLE_ID_NO_OWNER(_a_) ((_a_) & 0xFFFFFF00)
#define NFP_REGDB_TABLE_ID_OWNER_ANY 0xFF
#define NFP_REGDB_CAN_MERGE_RET_YES 1
#define NFP_REGDB_CAN_MERGE_RET_NO 0
#define NFP_REGDB_CAN_MERGE_RET_ERR -1
#define NFP_REGDB_USR_CONFLICT_RET_YES 1
#define NFP_REGDB_USR_CONFLICT_RET_NO 0
#define NFP_REGDB_USR_CONFLICT_RET_ERR -1

/** Entry value types, also called entry "mode".
 * The value types are enumerated in order of strictness
 * here, with 'const' being the most strict, but
 * @c nfp_regdb_val_type_cmp should preferrably be used for comparisons.
 */
enum nfp_regdb_valtype {
	NFP_REGDB_VALTYPE_NONE = 0,
	NFP_REGDB_VALTYPE_CONST = 1,
	NFP_REGDB_VALTYPE_REQUIRED = 2,
	NFP_REGDB_VALTYPE_VOLATILE_INIT = 3,
	NFP_REGDB_VALTYPE_VOLATILE_NOINIT = 4,
	NFP_REGDB_VALTYPE_INVALID = 5
};

/* Opaque register database handle */
struct nfp_regdb;

/* offset_hi = w0<31:24>  (8)
 * rsvd      = w0<23>     (1)
 * ctx       = w0<22:20>  (3)
 * val_undef = w0<19>     (1)
 * last_word = w0<18>     (1)
 * rsvd      = w0<17:0>  (18)
 * val_type  = w0<2:0>    (3)
 */
#define NFP_REGDB_ENTRY_WORD_W0_OHI_of(_w0_) (((_w0_) >> 24) & 0xFF)
#define NFP_REGDB_ENTRY_WORD_W0_VTP_of(_w0_) \
	((enum nfp_regdb_valtype)(((_w0_) >> 0) & 0x7))
#define NFP_REGDB_ENTRY_WORD_W0_LWO_of(_w0_) (((_w0_) >> 18) & 0x1)
#define NFP_REGDB_ENTRY_WORD_W0_VUN_of(_w0_) (((_w0_) >> 19) & 0x1)
#define NFP_REGDB_ENTRY_WORD_W0_CTX_of(_w0_) (((_w0_) >> 20) & 0x7)

#define NFP_REGDB_ENTRY_WORD_W0_OHI(v) (((v) & 0xFF) << 24)
#define NFP_REGDB_ENTRY_WORD_W0_VTP(v) (((uint32_t)(v) & 0x7) << 0)
#define NFP_REGDB_ENTRY_WORD_W0_LWO(v) (((v) & 0x1) << 18)
#define NFP_REGDB_ENTRY_WORD_W0_VUN(v) (((v) & 0x1) << 19)
#define NFP_REGDB_ENTRY_WORD_W0_CTX(v) (((v) & 0x7) << 20)

#define NFP_REGDB_ENTRY_WORD_W0(_offset_hi_, _ctx_, _val_type_, _last_word_, \
				_value_is_undef_)                            \
	(NFP_REGDB_ENTRY_WORD_W0_OHI(_offset_hi_) |                          \
	 NFP_REGDB_ENTRY_WORD_W0_CTX(_ctx_) |                                \
	 NFP_REGDB_ENTRY_WORD_W0_VUN(_value_is_undef_) |                     \
	 NFP_REGDB_ENTRY_WORD_W0_LWO(_last_word_) |                          \
	 NFP_REGDB_ENTRY_WORD_W0_VTP(_val_type_))

struct nfp_regdb_entry_word {
	/* w0 and offset_lo must be the same for all higher words of a
	 * multi-word entry (register larger than 32 bits), except for
	 * "last_word" which will only be set for the last 32-bit word.
	 */
	uint32_t w0;
	uint32_t offset_lo;
	uint32_t value;
	uint32_t mask;
};

struct nfp_regdb_entry {
	/* words[0] is bits <31:0> */
	struct nfp_regdb_entry_word *words;
	uint8_t word_count;
	uint8_t internal_flags;
	void *usrdata;
};

/** Create a new empty register database.
 *
 * @return      Pointer to new database, NULL on error with errno set
 *              to ENOMEM.
 */
NFP_COMMON_API
struct nfp_regdb *nfp_regdb_new();

/** Free a register database.
 *
 * @param db        Database
 * @param removing  If non-NULL, will be called for each entry before it is
 *                  freed. Users are responsible for freeing entry->usrdata.
 */
NFP_COMMON_API
void nfp_regdb_free(struct nfp_regdb *db,
		    void (*removing)(uint32_t table_id,
				     struct nfp_regdb_entry *entry));

/** Create a new register database from a serialized buffer.
 *
 * @param buf       Serialized buffer
 * @param buf_len   Byte size of buffer
 * @return          New register database, NULL on error.
 */
NFP_COMMON_API
struct nfp_regdb *nfp_regdb_from_buf(const void *buf,
				     size_t buf_len);

/** Create a serialized buffer from a register database.
 *
 * @param db        Register database
 * @param buf_out   Will be set to new serialized buffer, unless database empty
 * @return          Byte size of @p buf_out, 0 for empty database, -1 on error
 */
NFP_COMMON_API
ssize_t nfp_regdb_to_buf(struct nfp_regdb *db, void **buf_out);

/** Remove table from database.
 *
 * @param db        Database
 * @param table_id  Table ID of table to remove
 * @param removing  If non-NULL, called for each entry before it is removed
 *                  from the table. Users are responsible for freeing
 *                  entry->usrdata;
 */
NFP_COMMON_API
void nfp_regdb_table_remove(struct nfp_regdb *db,
			    uint32_t table_id,
			    void (*removing)(uint32_t table_id,
					     struct nfp_regdb_entry *entry));

/** Remove all owner's tables from database.
 *
 * @param db            Database
 * @param table_owner   Table owner of tables to remove. If set to
 *                      NFP_REGDB_TABLE_ID_OWNER_ANY, all tables will be
 *                      removed.
 * @param removing      If non-NULL, called for each entry before it is removed
 *                      from the table. Users are responsible for freeing
 *                      entry->usrdata;
 */
NFP_COMMON_API
void nfp_regdb_table_remove_by_owner(
	struct nfp_regdb *db,
	uint32_t table_owner,
	void (*removing)(uint32_t table_id, struct nfp_regdb_entry *entry));

/** Compact a table by merging or dropping entries on the same register.
 *
 * The typical use-case for this is when entries are added for each bit-field
 * or a register with the same value type, for example setting several fields
 * of a register to 'const' values, those entries can instead be combined into
 * one entry for the whole register. This is also important for registers
 * with Write-Only fields that will not support read-modify-write access as
 * the user may expect.
 *
 * Another use-case is when a 'const' entry on certain bits effectively
 * ensures a certain value for those bits, so any 'required' or 'invalid'
 * entries on those bits are not necessary to keep around.
 *
 * When one entry has a defined value and another has an undefined value,
 * they cannot be merged and @p can_merge will not be called. When both
 * entries have defined values they will be merged if @p can_merge allows it,
 * but it is recommended that @p can_merge allows this for defined entries.
 * For two entries with undefined values @p can_merge will be the deciding
 * factor in deciding if they can be merged or not. If two undefined entries
 * are merged, entry->words[n].value will be undefined, so if a user used this
 * field for any purpose, it cannot be used after merging.
 *
 * @param db        Database
 * @param table_id  Table ID of the table to compact
 * @param removing  Callback to let the caller know when entries should be
 *                  considered removed from the table. The user must free
 *                  entry-usrdata and set it to NULL, otherwise stale usrdata
 *                  in entries may cause problems. Some entries may be
 *                  considered removed when merged, but one of them actually
 *                  stays in the table (keeping any old @p entry->usrdata
 *                  value). This is for internal convenience when
 *                  moving entries down in the table, rather than creating
 *                  a completely new table.
 * @param can_merge When the compactor deems two entries mergeable, it will
 *                  call this callback to let the user have a final say. The
 *                  function should return NFP_REGDB_CAN_MERGE_RET_YES if they
 *                  can merge, NFP_REGDB_CAN_MERGE_RET_NO if not and
 *                  NFP_REGDB_CAN_MERGE_RET_ERR on error.
 *                  An error return will abort compaction. The
 *                  callback may assume that @p entry1 and @p entry2 point to
 *                  the same register and are both defined or both undefined.
 *                  Masks may overlap, be the same or not overlap. The intended
 *                  use-case for this callback is to let users decide when
 *                  two undefined entries can merge based on usrdata.
 * @return          0 on success, -1 on error with errno set.
 */
NFP_COMMON_API
int nfp_regdb_table_compact(
	struct nfp_regdb *db,
	uint32_t table_id,
	void (*removing)(uint32_t table_id, struct nfp_regdb_entry *entry),
	int (*can_merge)(uint32_t table_id,
			 const struct nfp_regdb_entry *entry1,
			 const struct nfp_regdb_entry *entry2));

/** Check all entries in the table for conflicts.
 *
 * Entries added to a table should ideally never conflict with existing entries
 * in the table, but this can be used to do a sanity check on a table that
 * has been populated with data which is expected to be valid.
 *
 * @param db            Database
 * @param table_id      Table ID of table to validate
 * @param usr_conflicts If non-NULL, this is called for entries to the same
 *                      register that have no conflict on value/mask/mode, but
 *                      may have a conflict on 'usrdata'. The @c usr_conflicts
 *                      callback must return NFP_REGDB_USR_CONFLICT_RET_NO
 *                      if there is no conflict, NFP_REGDB_USR_CONFLICT_RET_YES
 *                      if there is a conflict, NFP_REGDB_USR_CONFLICT_RET_ERR
 *                      if there is an error.
 *                      The callback may assume that @p a and @p b are
 *                      non-NULL, point to the same register, have an equal
 *                      number of words and that all words are valid.
 * @return              If the table is valid, returns 0. If there are
 *                      conflicts, returns 1. If there is an error, returns -1
 *                      and sets errno.
 */
NFP_COMMON_API
int nfp_regdb_table_validate(
	struct nfp_regdb *db,
	uint32_t table_id,
	int (*usr_conflicts)(const struct nfp_regdb_entry *a,
			     const struct nfp_regdb_entry *b,
			     int same_owner));

/** Get a list of table IDs.
 *
 * @param db        Database
 * @param table_ids If non-NULL, *table_ids will be set to a newly allocated
 *                  array containing a list of table IDs. Caller must free().
 * @return          The number of tables in the database.
 */
NFP_COMMON_API
size_t nfp_regdb_table_ids(struct nfp_regdb *db,
			   uint32_t **table_ids);

/** Get all entries in a table.
 *
 * If the table is not found in the database, 0 will be returned.
 * When 0 is returned, *entries is undefined.
 *
 * Entries are sorted by:
 * - Register offset
 * - ctx
 * - Value Undefined (Defined values first)
 * - Value Type
 *
 * This means iterators over entries can use @c nfp_regdb_entry_regcmp to
 * detect when entries for a new register starts.
 *
 * @param db        Databse
 * @param table_id  Table ID to get entries of.
 * @param entries   If non-NULL, *entries will be set to the array of entries.
 * @return          The number of entries in the table.
 */
NFP_COMMON_API
size_t nfp_regdb_table_entries(struct nfp_regdb *db,
			       uint32_t table_id,
			       const struct nfp_regdb_entry **entries);

/** Check two register's entry words for a conflict.
 *
 * @param existing      An existing entry's words. This is akin to a target
 *                      CSR reservation.
 * @param incoming      An incoming entry's words. This is akin to a firmware
 *                      CSR reservation.
 * @param same_owner    1 if the two entries are reserved by the same entity,
 *                      0 if not. If the two entries are owned by the same
 *                      entity, conflict checking is slightly different.
 * @return              1 if conflicting on value or mode, 0 if not, -1 if
 *                      error with errno set.
 */
NFP_COMMON_API
int nfp_regdb_word_conflicts(const struct nfp_regdb_entry_word *existing,
			     const struct nfp_regdb_entry_word *incoming,
			     int same_owner);

/** Check two entries for a conflict.
 *
 * Similar to @c nfp_regdb_word_conflicts, but for a complete table entry.
 *
 * @param existing      An existing entry. This is akin to a target
 *                      CSR reservation.
 * @param incoming      An incoming entry. This is akin to a firmware
 *                      CSR reservation.
 * @param same_owner    1 if the two entries are reserved by the same entity,
 *                      0 if not. If the two entries are owned by the same
 *                      entity, conflict checking is slightly different.
 * @param usr_conflicts If non-NULL, this is called for entries to the same
 *                      register that have no conflict on value/mask/mode, but
 *                      may have a conflict on 'usrdata'. The @c usr_conflicts
 *                      callback must return NFP_REGDB_USR_CONFLICT_RET_NO
 *                      if there is no conflict, NFP_REGDB_USR_CONFLICT_RET_YES
 *                      if there is a conflict, NFP_REGDB_USR_CONFLICT_RET_ERR
 *                      if there is an error.
 *                      The callback may assume that @p a and @p b are
 *                      non-NULL, point to the same register, have an equal
 *                      number of words and that all words are valid.
 * @return              1 if conflicting on value, mode or usrdata,
 *                      0 if not, -1 if error with errno set.
 */
NFP_COMMON_API
int nfp_regdb_entry_conflicts(
	const struct nfp_regdb_entry *existing,
	const struct nfp_regdb_entry *incoming,
	int same_owner,
	int (*usr_conflicts)(const struct nfp_regdb_entry *existing,
			     const struct nfp_regdb_entry *incoming,
			     int same_owner));

/** Validate register entry words against a table.
 *
 * Similar to @c nfp_regdb_table_validate, but validates words that may
 * not yet be in any table.
 *
 * @param db            Database
 * @param table_id      Table ID. If the owner of @p table_id
 *                      is NFP_REGDB_TABLE_ID_OWNER_ANY, @p all_tables is
 *                      implicitly true.
 * @param all_tables    If 0, only the table with ID @p table_id is checked for
 *                      conflicts. If non-zero, all tables that have the same
 *                      ID are checked, ignoring the table owner field.
 *                      In this case, @p same_owner passed to @p usr_conflicts
 *                      will be set if the table owner is the same as the
 *                      requested @p table_id. If the owner of @p table_id
 *                      is NFP_REGDB_TABLE_ID_OWNER_ANY, @p same_owner is
 *                      always 0.
 * @param words         Register entry words.
 * @param word_count    The number of items in @p words.
 * @param usrdata       Optional user data that would form part of the words
 *                      as an entry if added to the table.
 * @param usr_conflicts If non-NULL, this is called for entries to the same
 *                      register that have no conflict on value/mask/mode, but
 *                      may have a conflict on 'usrdata'. The @c usr_conflicts
 *                      callback must return NFP_REGDB_USR_CONFLICT_RET_NO
 *                      if there is no conflict, NFP_REGDB_USR_CONFLICT_RET_YES
 *                      if there is a conflict, NFP_REGDB_USR_CONFLICT_RET_ERR
 *                      if there is an error.
 *                      The callback may assume that @p a and @p b are
 *                      non-NULL, point to the same register, have an equal
 *                      number of words and that all words are valid.
 * @param conflict      If non-NULL, *conflict will point to the first entry
 *                      found to conflict with incoming words. *conflict will
 *                      be NULL if there was no conflict.
 * @return              1 if conflicting on value, mode or usrdata,
 *                      0 if not, -1 if error with errno set.
 */
NFP_COMMON_API
int nfp_regdb_entry_validate(
	struct nfp_regdb *db,
	uint32_t table_id,
	int all_tables,
	const struct nfp_regdb_entry_word *words,
	uint8_t word_count,
	void *usrdata,
	int (*usr_conflicts)(const struct nfp_regdb_entry *existing,
			     const struct nfp_regdb_entry *incoming,
			     int same_owner),
	const struct nfp_regdb_entry **conflict,
	int *conflict_owner);

/** Add register entry words to a table as a new entry.
 *
 * If the table does not yet exist it is created, if the table exists the
 * entry is added and the table is sorted.
 *
 * The caller must ensure that:
 *   NFP_REGDB_ENTRY_WORD_W0_CTX_of(words[x].w0) == 0
 * for registers that are not context indirect ME CSRs, otherwise
 * comparison on offset and ctx will fail to correctly identify registers.
 *
 * The caller does not need to set NFP_REGDB_ENTRY_WORD_W0_LWO, this function
 * will set the las-word bits for each word correctly.
 *
 * @param db            Database
 * @param table_id      Table ID
 * @param words         Register entry words.
 * @param word_count    The number of items in @p words.
 * @param usrdata       Optional user data that form part of the words
 *                      as an entry.
 *
 * @return 0 on success, -1 on error.
 */
NFP_COMMON_API
int nfp_regdb_entry_add(struct nfp_regdb *db,
			uint32_t table_id,
			const struct nfp_regdb_entry_word *words,
			uint8_t word_count,
			void *usrdata);

/** Get the value and value mode for a given register.
 *
 * @param db            Database
 * @param table_id      Table ID. If the table owner is
 *                      NFP_REGDB_TABLE_ID_OWNER_ANY, all tables will be
 *                      searched for entries matching the register offset
 *                      and base table_id (no owner comparison).
 * @param offset        The CPP offset of the register to search for.
 * @param ctx           The context of the register to search for. Only
 *                      applies to indirect ME CSRs and should be <= 0 if
 *                      not applicable.
 * @param masks         An array of 32-bit masks. The value and value mode
 *                      gathered will be only for bits set in @p masks.
 *                      Must be at least @word_cnt words long.
 * @param vals          If NULL, ignored. If non-NULL, must be at least
 *                      @word_cnt words long and will hold the value
 *                      for all "const" and "required" mode entries.
 *                      Therefore, if only "volatile" entries were found, this
 *                      will be 0 even though there may be volatile-init
 *                      entries.
 * @param word_cnt      The number of 32-bit words in @p masks and @p vals.
 *                      This should be at least as many as the register
 *                      will need.
 * @param set_type      If NULL, ignored. If non-NULL, this will be set to the
 *                      most strict value mode found for any of the bits in
 *                      @p masks.
 * @param is_undefined  If NULL, ignored. If non-NULL, will be set to 1 if
 *                      any value bits were "set", but with an undefined
 *                      value (NFP_REGDB_ENTRY_WORD_W0_VUN). This is only
 *                      expected to be useful in the SDK toolchain.
 *
 * @return 0 on success, -1 on error.
 */
NFP_COMMON_API
int nfp_regdb_val_get(struct nfp_regdb *db,
		      uint32_t table_id,
		      uint64_t offset,
		      int8_t ctx,
		      const uint32_t *masks,
		      uint32_t *vals,
		      uint8_t word_cnt,
		      enum nfp_regdb_valtype *set_type,
		      int *is_undefined);

/** Compare two value types
 *
 * @param a Value type A
 * @param b Value type B
 * @return  0 if equal, 1 if @p b is more strict than @a or -1 if @p a is more
 *          strict than @p b
 */
NFP_COMMON_API
int nfp_regdb_val_type_cmp(enum nfp_regdb_valtype a,
			   enum nfp_regdb_valtype b);

/** Compare two entries by register ID.
 *
 * Comparison is, in sequence, on:
 * - offset
 * - ctx
 *
 * @param a Entry A
 * @param b Entry B
 * @return  0 if equal, -1 if (a < b), 1 if (a > b)
 */
NFP_COMMON_API
int nfp_regdb_entry_regcmp(const struct nfp_regdb_entry *a,
			   const struct nfp_regdb_entry *b);

__END_DECLS

#endif /* __NFP_REGDB_H__ */
