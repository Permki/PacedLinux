/*
 * Copyright (C) 2017  Netronome Systems, Inc.  All rights reserved.
 *
 * @file          nfp_pif_tbl_entries.h
 * @brief         Table entry interface for NFP PIF library 
 *
 */

#ifndef __NFP_PIF_TBL_ENTRIES_H__
#define __NFP_PIF_TBL_ENTRIES_H__

#include <stdint.h>
#include <sys/cdefs.h>

#include "_nfp_pif_apiex.h"

__BEGIN_DECLS


/* Following structures are abridged versions of the the user_data and 
 * design_data  structures, more fields can be added if users of the nfp_pif 
 * library needs them */
typedef struct {
    char *name;
    char *value_str;
} nfp_pif_usr_action;

typedef struct {
    char *name;
    char *value_str;
    char *mask_str;
    int dontcare;
} nfp_pif_usr_match;

typedef struct {
    char *name;
} nfp_pif_usr_action_design;

typedef struct {
    char *name;
    char *action;

    int id;

    /* <= 0 mean no priority, 1 is highest priority, 2 is lower etc. */
    int user_priority;

    /* effective priority */
    int priority;

    nfp_pif_usr_action *actiondata;
    int actiondata_len;

    nfp_pif_usr_match *matches;
    int matches_len;

    nfp_pif_usr_action_design *design_action;

    int breakpoint_enabled;

    /* timeout in seconds, 0 means never timeout */
    int timeout;

    /* ID allocated when rule is added */
    int timeout_id;

    /* flag indicating default rule */
    int default_rule;
} nfp_pif_user_rule;

/**
* Add user table entry, if rule already exists with the same ID it is over
* written with specified values. In the case where entry already exists if match
* or action data is not supplied old data is retained but new entry is being
* created an error will be generated if both match and action data is not
* supplied. 
*
* @param handle         Handle for nfp_pif target as returned by nfp_pif_attach
* @param tbl_id         ID of table where entry must be added
* @param default_rule   Flag indicating if rule is a default rule
* @param rule_name      Name of new rule
* @param match          String buffer containing match data in JSON format
* @param mlen           Length of match buffer
* @action               String buffer containing action data in JSON format
* @act_len              Length of action buffer
* @priority             Rule priority
* @timeout              Rule timeout in seconds

* @returns  Returns 0 if no error occurs and rule is added successfully
*           otherwise return -1 and set errno to pif_error code
*/
PIF_PUBLIC
int nfp_pif_tbl_entries_add(uint64_t handle,
                            int tbl_id,
                            int default_rule,
                            const char *rule_name,
                            const char *match,
                            int mlen,
                            const char *action,
                            int act_len,
                            int priority,
                            int timeout);

/**
* Edit user table entry, an existing rule match is found using rule name only
* and default rule is only checked if default_rule flag is set where again rule
* names must match. If match or action data is not supplied old data is
* retained. If an existing matching name rule is not found no data is changed
* and error code PIF_ERROR_TBL_ENT_RULE_NOT_FOUND is returned.
*
* @param handle       Handle for nfp_pif target as returned by nfp_pif_attach
* @param tbl_id       ID of table where entry must be edited
* @param default_rule Flag indicating if rule is a default rule
* @param rule_name    Name of new rule
* @param match        String buffer containing match data in JSON format
* @param mlen         Length of match buffer
* @action             String buffer containing action data in JSON format
* @act_len            Length of action buffer
* @priority           Rule priority
* @timeout            Rule timeout in seconds

* @returns  Returns 0 if no error occurs and rule is edited
*           successfully otherwise return -1 and set errno to pif_error code
*/
PIF_PUBLIC
int nfp_pif_tbl_entries_edit(uint64_t handle,
                             int tbl_id,
                             int default_rule,
                             const char *rule_name,
                             const char *match,
                             int mlen,
                             const char *action,
                             int act_len,
                             int priority,
                             int seconds);

/**
* Delete user table entry, an existing rule match is found using rule name only
* and default rule is checked first. If match or action data is not supplied old
* data is retained. If an existing matching name rule is not found no data is
* changed and error code PIF_ERROR_TBL_ENT_RULE_NOT_FOUND is returned.
*
* @param handle    Handle for nfp_pif target as returned by nfp_pif_attach
* @param tbl_id    ID of table from which the entry must be deleted
* @param rule_name Name of new rule
*
* @returns  Returns 0 if no error occurs and rule is removed
*           successfully otherwise return -1 and set errno to pif_error code
*/
PIF_PUBLIC
int nfp_pif_tbl_entries_delete(uint64_t handle,
                               int tbl_id,
                               const char *rule_name);

/**
 * Funtion to process timed-out rules
 *
 * @param handle    Handle for nfp_pif target as returned by nfp_pif_attach
 * @param tbl_id    ID of table from which the entry must be deleted
 */
PIF_PUBLIC
void nfp_pif_tbl_entries_process_timedout(uint64_t handle, int tbl_id);

/**
* Function returns list of table entries
*
* @param handle       Handle for nfp_pif target as returned by nfp_pif_attach
* @param tbl_id       ID of table which entries must be listed
* @param rule_cnt     Pointer to varible passed on in which the entries count
                      will be returned
*
* @returns  Returns pointer to list of table entries (must be freed by caller,
            nfp_pif_tbl_entries_free() was created for this purpose), if 
            error occurs return NULL and poplate errno.
*
*
*/
PIF_PUBLIC
nfp_pif_user_rule *nfp_pif_tbl_entries_list(uint64_t handle,
                                            int tbl_id,
                                            int *rule_cnt);

/**
* Function to be used to list entries in a table in string format.  Destination
* string specified by dst is populated by the rule data at the specified index.
* If table empty or if there isn't rule at the index (end of table reached)
* PIF_ERROR_TBL_ENT_RULE_NOT_FOUND error code is returned.
*
* @param handle   Handle for nfp_pif target as returned by nfp_pif_attach
* @param tbl_name Name of table which entries must be listed
* @dst            Destination string where rule data will be written
* @rule_idx       Index of rule to be returned

* @returns  Returns 0 if no error occurs and rule data is written to the buffer
*           successfully otherwise return -1 and set errno to pif_error code
*/
PIF_PUBLIC
int  nfp_pif_tbl_entries_str_list(uint64_t handle,
                                  const char *tbl_name,
                                  char **dst,
                                  int rule_idx);

/**
 * Function to free all memory allocated to fields in specified nfp_pif_user_rule 
 * structure or list of rules. Rule list struct also freed.
 *
 * @param rule    Rule structure which must be freed
 */
PIF_PUBLIC
void nfp_pif_tbl_entries_free(nfp_pif_user_rule *rule_list, int rule_cnt);

__END_DECLS

#endif /* __NFP_PIF_TBL_ENTRIES_H__ */
