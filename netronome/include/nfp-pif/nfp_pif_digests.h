/*
 * Copyright (C) 2017  Netronome Systems, Inc.  All rights reserved.
 *
 * @file          nfp_pif_digests.h
 * @brief         Digests interface for NFP PIF library 
 *
 */

#ifndef __NFP_PIF_DIGESTS_H__
#define __NFP_PIF_DIGESTS_H__

#include <stdint.h>
#include <sys/cdefs.h>

#include "_nfp_pif_apiex.h"

__BEGIN_DECLS

typedef struct {
    char *name;
    int width;
} nfp_pif_dsgn_dgst_field;

typedef struct {
    char *name;
    char *field_list_str;
    int id;

    nfp_pif_dsgn_dgst_field *fields;
    int fields_len;
} nfp_pif_digest;


/**
* Function to run cleanup of digests for specifed ahndle after client 
* disconnects
*/
PIF_PUBLIC
int nfp_pif_digests_client_cleanup(uint64_t handle);

PIF_PUBLIC
int64_t nfp_pif_digests_client_register(uint64_t handle,
                                        int digest_id);

PIF_PUBLIC
int nfp_pif_digests_client_deregister(uint64_t handle,
                                      int64_t digest_regid);

PIF_PUBLIC
int nfp_pif_digests_retrieve(uint64_t handle,
                             int64_t digest_regid,
                             char ***values);

/**
* Retrieve a list of digests for the specified handle
*
* @param handle    Handle for nfp_pif target as returned by nfp_pif_attach
* @param dgst_len  Pointer for returning the length of the digest list
*
* @returns    Returns address to the list of digests. This function will 
              allocate the needed memory for the list, caller must free the list
              and  subfields. nfp_pif_digest_free is a helper function created
*             for freeing digets list. Otherwise return NULL and set errno 
              to pif_error.
*/
PIF_PUBLIC
nfp_pif_digest *nfp_pif_digests_list(uint64_t handle, int *dgst_len);

/**
* Helper function to free given digest list
*
* @param dgsts     Pointer to the digest list that need to be freed
* @param dgst_len  Length of the digests list
*/
PIF_PUBLIC
void nfp_pif_digest_free(nfp_pif_digest *dgsts, int dgst_len);

__END_DECLS

#endif /* __NFP_PIF_DIGESTS_H__ */