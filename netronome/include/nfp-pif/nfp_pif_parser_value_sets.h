/*
 * Copyright (C) 2017  Netronome Systems, Inc.  All rights reserved.
 *
 * @file          nfp_pif_parser_value_sets.h
 * @brief         Parser value sets interface for NFP PIF library 
 *
 */

#ifndef __NFP_PIF_PARSER_VALUE_SETS_H__
#define __NFP_PIF_PARSER_VALUE_SETS_H__


#include <stdint.h>
#include <sys/cdefs.h>

#include "_nfp_pif_apiex.h"
#include "arbint.h"

__BEGIN_DECLS

typedef struct {
    char *name;
    int width;
} nfp_pif_pvs_field;

typedef struct {
    char *name;
    int key_width;
    int table_size;
    nfp_pif_pvs_field *fields;
    int fields_len;
} nfp_pif_pvs_set;

typedef struct {
    char *value_arbstr;
    char *mask_arbstr;
} nfp_pif_pvs_set_entry;


/**
* Retrieve parser value set at specified index design configuration from
* design data structure for the device specified by the given handle
*
* @param handle  Handle for nfp_pif target as returned by nfp_pif_attach
* @param set_cnt Pointer to the number of sets being returned
*
* @returns  Pointer to structure with the parser value set data, caller the 
            free nfp_pif_pvs_set structure. Helper function nfp_pif_free_pvs_set
            created for this task. In case of error return NULL and set errno to 
*           pif_error code
*/
PIF_PUBLIC
nfp_pif_pvs_set *nfp_pif_parser_value_sets_get(uint64_t handle,
                                               int *set_cnt);

/**
* Add entry to parser value set 
*
* @param handle  Handle for nfp_pif target as returned by nfp_pif_attach
* @param set_id  ID os set to whihc entry must be added
* @param value   String with entry value
* @param mask    String with entry mask
*
* @returns  0 if successful otherwise return -1 and errno set to pif_error code
*/
PIF_PUBLIC
int nfp_pif_parser_value_sets_add_entry(uint64_t handle,
                                        int set_id,
                                        const char *value,
                                        const char *mask);

/**
* Clears parser values set's entries 
*
* @param handle  Handle for nfp_pif target as returned by nfp_pif_attach
* @param set_id   ID of set which must be cleared
*
* @returns  0 if successful otherwise  return negative error code
*/
PIF_PUBLIC
int nfp_pif_parser_value_sets_clear_set(uint64_t handle, int set_id);

/**
* Retrieve all entries from specified set at specified index
*
* @param handle    Handle for nfp_pif target as returned by nfp_pif_attach
* @param set_id    Target parser value set's ID
* @param entry_idx Index of entry to return
*
* @returns  Pointer to the parser value set entry data will be returned, caller 
            must free nfp_pif_pvs_set_entry structure. Helper function 
            nfp_pif_free_pvs_entry created for this task. In case of error
            NULL will be returnedand errno populated with pif_error code
*/
PIF_PUBLIC
nfp_pif_pvs_set_entry *nfp_pif_parser_value_sets_get_entries(uint64_t handle,
                                                             int *entry_cnt,
                                                             int set_id);


/**
 * Function to free all memory allocated to fields in specified nfp_pif_pvs_set 
 * structure, BUT NOT THE STRUCTURE ITSELF
 *
 * @param set    Sets structure of which the content fields must be freed
 */
PIF_PUBLIC
void nfp_pif_free_pvs_sets(nfp_pif_pvs_set *sets, int set_cnt);

PIF_PUBLIC
void nfp_pif_free_pvs_entry_list(nfp_pif_pvs_set_entry *entries, int entry_cnt);

__END_DECLS

#endif /* !__NFP_PIF_PARSER_VALUE_SETS_H__ */