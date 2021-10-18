/**
 * Copyright (C) 2012 Netronome Systems, Inc.  All rights reserved.
 *
 * Interim NFP device handle header
 *
 */

#ifndef __NFP_CPP_HANDLES_H__
#define __NFP_CPP_HANDLES_H__

#include "nfp_client_common.h"

#ifdef __cplusplus
extern "C" {
#endif

struct nfp_cpp {
    struct ns_ipc_client_handle ipc_handle;
    struct ns_ipc_client_handle *ipc_h;
    int own_handle;
    uint64_t rh; /* remote handle */
    struct handle_common h_common;
    int id;
};

struct nfp_cpp_area {
    struct ns_ipc_client_handle ipc_handle;
    struct ns_ipc_client_handle *ipc_h;
    int own_handle;
    uint64_t rh; /* remote handle */
    struct handle_common h_common;
    int id;
    struct nfp_cpp *cpp;
    char *name;
};

struct nfp_cpp_mutex {
    struct ns_ipc_client_handle ipc_handle;
    struct ns_ipc_client_handle *ipc_h;
    int own_handle;
    uint64_t rh; /* remote handle */
    struct handle_common h_common;
    int id;
    struct nfp_cpp *cpp;
};

struct nfp_cpp_event {
    struct ns_ipc_client_handle ipc_handle;
    struct ns_ipc_client_handle *ipc_h;
    int own_handle;
    uint64_t rh; /* remote handle */
    struct handle_common h_common;
    int id;
    struct nfp_cpp *cpp;
};

int nfp_cpp_from_device_id_local(struct nfp_cpp *nfp_cpp, int id);
void nfp_cpp_free_local(struct nfp_cpp *nfp_cpp);

int nfp_cpp_area_alloc_local(struct nfp_cpp_area *area,
                             struct nfp_cpp *cpp,
                             uint32_t cpp_id,
                             unsigned long long address,
                             unsigned long size);
int nfp_cpp_area_alloc_with_name_local(struct nfp_cpp_area *area,
                                       struct nfp_cpp *cpp,
                                       uint32_t cpp_id,
                                       const char *name,
                                       unsigned long long address,
                                       unsigned long size);
void nfp_cpp_area_free_local(struct nfp_cpp_area *area);

int nfp_cpp_area_alloc_acquire_local(struct nfp_cpp_area *area,
                                     struct nfp_cpp *cpp,
                                     uint32_t cpp_id,
                                     unsigned long long address,
                                     unsigned long size);


void nfp_cpp_area_release_free_local(struct nfp_cpp_area *area);

int nfp_cpp_event_alloc_local(struct nfp_cpp_event *area,
                              struct nfp_cpp *cpp,
                              uint32_t event_match,
                              uint32_t event_mask,
                              int type);

void nfp_cpp_event_free_local(struct nfp_cpp_event *event);
int nfp_cpp_mutex_alloc_local(struct nfp_cpp_mutex *mutex,
                              struct nfp_cpp *cpp,
                              int target,
                              unsigned long long address,
                              uint32_t key_id);

void nfp_cpp_mutex_free_local(struct nfp_cpp_mutex *mutex);

#ifdef __cplusplus
};
#endif

#endif /* __NFP_CPP_HANDLES_H__ */
