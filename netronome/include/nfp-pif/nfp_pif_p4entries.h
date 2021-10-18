/*
 * Copyright (C) 2017  Netronome Systems, Inc.  All rights reserved.
 *
 * @file          nfp_pif_p4entries.h
 * @brief         Functions to read, write and modify tables entries. These 
                  functions were deduced from the Write and Read messages in 
                  p4runtime.proto from the the P4Lang PI project on 
 *                Github (https://github.com/p4lang/PI)
 *
 */

#ifndef __NFP_PIF_P4ENTRIES_H__
#define __NFP_PIF_P4ENTRIES_H__

#include <stdint.h>
#include <sys/cdefs.h>

#include "_nfp_pif_apiex.h"
#include "nfp_pif_p4info_defs.h"

__BEGIN_DECLS

typedef enum {
    RAW = 0,
    AUTO
} p4_value_format_t;

typedef struct {
    uint8_t *value;        // big endian byte string
    int value_len;
} p4_match_exact_t;

typedef struct {
    uint8_t *value;        // big endian byte string
    int value_len;
    uint8_t *mask;         // big endian byte string
    int mask_len;
} p4_match_ternary_t;

typedef struct {
    uint8_t *value;        // big endian byte string
    int value_len;
    int32_t prefix_len;
} p4_match_lpm_t;

typedef struct {
    uint8_t *low;         // big endian byte string
    int low_len;
    uint8_t *high;        // big endian byte string
    int high_len;
} p4_match_range_t;

typedef struct {
    bool value;
} p4_match_valid_t;

typedef union {
    p4_match_exact_t exact;
    p4_match_ternary_t ternary;
    p4_match_lpm_t lpm;
    p4_match_range_t range;
    p4_match_valid_t valid;
    p4_match_ternary_t unspec;
} p4_match_data_u;

typedef struct {
    uint32_t field_id;
    p4_value_format_t format;
    p4_match_data_u data;
    p4info_matchtype_t type;
} p4_entry_match_t;

typedef struct {
    uint32_t param_id;
    uint8_t *value;
    int value_len;
    p4_value_format_t format;
} p4_action_param_t;

typedef struct {
    uint32_t action_id;
    p4_action_param_t *params;
    int num_params;
}p4_action_t;

typedef union {
    p4_action_t action;
    uint32_t profile_member_id;
    uint32_t profile_group_id;
} p4_action_data_u;

typedef enum {
    ACTION,
    PROFILE_MEMBER_ID,
    PROFILE_GROUP_ID
} p4_action_type_t;

typedef struct {
    p4_action_data_u data;
    p4_action_type_t type;
} p4_entry_action_t;

typedef struct {
    uint32_t table_id;
    p4_entry_match_t *match;
    int num_matches;
    p4_entry_action_t action;
    int32_t priority;
    // Metadata (cookie) opaque to the target. There is no requirement of where
    // this is stored, as long as it is returned with the rest of the entry in
    // a Read RPC.
    uint64_t metadata;
} p4_table_entry_t;



/*
 * Insert given set of p4runtime table entries into table
 *
 * @param handle         Handle for nfp_pif target as returned by nfp_pif_attach
 * @param entries        Pointer to block of memory with seqeuntial entries, all 
 *                       fields must be populates to add entry to table
 * @param num_entries    Number of entries in the given entries list
 *
 * @return  Return 0 on successfull addition of at least some of the entries, 
 *          return -1 when no entries could be inserted and populate errno
 */
PIF_PUBLIC
int nfp_pif_p4entries_insert(uint64_t handle, 
                             p4_table_entry_t *entries,
                             int num_entries);

/*
 * Modify given set of p4runtime table entries into table
 *
 * @param handle         Handle for nfp_pif target as returned by nfp_pif_attach
 * @param entries        Pointer to block of memory with seqeuntial entries, all 
 *                       fields must be populates to add entry to table
 * @param num_entries    Number of entries in the given entries list
 *
 * @return  Return 0 on successfull modification of at least some of the entries, 
 *          return -1 when no entries could be inserted and populate errno
 */
PIF_PUBLIC
int nfp_pif_p4entries_modify(uint64_t handle, 
                             p4_table_entry_t *entries,
                             int num_entries);

/*
 * Delete given set of p4runtime table entries into table
 *
 * @param handle         Handle for nfp_pif target as returned by nfp_pif_attach
 * @param entries        Pointer to block of memory with seqeuntial entries, all 
 *                       fields must be populates to add entry to table
 * @param num_entries    Number of entries in the given entries list
 *
 * @return  Return 0 on successfull deletion of at least some of the entries, 
 *          return -1 when no entries could be inserted and populate errno
 */
PIF_PUBLIC
int nfp_pif_p4entries_delete(uint64_t handle, 
                             p4_table_entry_t *entries,
                             int num_entries);

/*
 * Read all entries from table with specified table p4id
 *
 * @param handle         Handle for nfp_pif target as returned by nfp_pif_attach
 * @param table_id       P4ID of the table which entries must be returned.
 *                       Callers responsibility to free memory allocated to for
 *                       this list. nfp_pif_p4entries_free was created for this
 *                       purpose.
 * @param num_entries    Pointer to variable in which the number of entries
 *                       returned will be written
 *
 * @return  Return block of memory populate with sequential list of table 
 *          entries, if error occurs return NULL and populate errno
 */
PIF_PUBLIC
p4_table_entry_t *nfp_pif_p4entries_read(uint64_t handle, 
                                         uint32_t table_id,
                                         int *num_entries,
                                         p4_value_format_t format);

/*
 * Free memory allocated to given list of table entrires returned by
 * nfp_pif_p4entries_read
 *
 * @param entries        Pointer to list of entries that must be freed
 * @param num_entries    Number of entries in the list
 */
PIF_PUBLIC
void nfp_pif_p4entries_free(p4_table_entry_t *entries, int num_entries);


__END_DECLS

#endif /* !__NFP_PIF_P4ENTRIES_H__ */
