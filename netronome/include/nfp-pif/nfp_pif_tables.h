/*
 * Copyright (C) 2017  Netronome Systems, Inc.  All rights reserved.
 *
 * @file          nfp_pif_tables.h
 * @brief         Table interface for NFP PIF library 
 *
 */

#ifndef __NFP_PIF_TABLES_H__
#define __NFP_PIF_TABLES_H__

#include <stdint.h>
#include <sys/cdefs.h>

#include "_nfp_pif_apiex.h"

__BEGIN_DECLS

// Forward declarations
typedef struct {
    int id;
    int max_entries;
    int data_size;
    char *name;
    int support_timeout;
} nfp_pif_dsgn_tbl;

/**
* Function to be used to list tables.  Destination string specified by dst is
* populated by the table design data at the specified index.
*
* @param handle   Handle for nfp_pif target as returned by nfp_pif_attach
* @param dst      Destination string where table data will be written
* @param rule_idx Index of table to be returned

* @returns  Returns 0 if no error occurs and table data is written to the buffer
*           successfully otherwise return -1 and set errno to pif_error code
*/
PIF_PUBLIC
int  nfp_pif_tbl_list_tables(uint64_t handle, char **dst, int tbl_idx);

/**
* Retrieve design table structure of table at specified index.
*
* @param handle   Handle for nfp_pif target as returned by nfp_pif_attach
* @param dt       Pointer to list of design tables created to return all design
*                 table information
* @param dt_cnt   Pointer to the variable in which the design table count will
*                 be returned.
* @returns  Returns 0 if no error occurs and dt gets set successfully
*           otherwise return -1 and set errno to pif_error code
*/
PIF_PUBLIC
nfp_pif_dsgn_tbl *nfp_pif_tbl_get_design_tables(uint64_t handle,
                                                int *dt_cnt);

/**
 * Function to free all memory allocated to fields in specified nfp_pif_dsgn_tbl 
 * structure and the structure itself.
*
* @param dt       Pointer to nfp_pif_dsgn_tbl structure that needs to be freed
*/
PIF_PUBLIC 
void nfp_pif_tbl_free(nfp_pif_dsgn_tbl *dtbls, int dt_cnt);

__END_DECLS

#endif /* __NFP_PIF_TABLES_H__ */
