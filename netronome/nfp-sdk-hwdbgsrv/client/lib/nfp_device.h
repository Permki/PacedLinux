/**
 * Copyright (C) 2012 Netronome Systems, Inc.  All rights reserved.
 *
 * Interim NFP device handle header
 *
 */

#ifndef __NFP_DEVICE_H__
#define __NFP_DEVICE_H__

#include "nfp_client_common.h"
#include <ns_ipc_buf.h>
#include <ns_ipc_client.h>

#ifdef __cplusplus
extern "C" {
#endif

struct nfp_device_hwinfo_entry;
struct nfp_mip;
struct nfp_rtsym;
struct nfp_cpp;

struct private_handle_data {
    void (*destructor)(void *);
    void *(*constructor)(struct nfp_device *);
};

struct nfp_device {
    struct nfp_chipdata *chipdata;
    struct nfp_chipdata_chip *chip;
    struct ns_ipc_client_handle ipc_handle;
    struct ns_ipc_client_handle *ipc_h;
    int own_handle;
    uint64_t rh; /* remote handle */
    struct handle_common h_common;
    int id;
    struct nfp_device_hwinfo_entry *hwinfo;
    int hwinfo_entries;
    struct nfp_mip *mip;
    int mip_island;
    int numsyms; /* rtsyms */
    struct nfp_rtsym *rtsymtab;
    char *strtab;
    struct private_handle_data **private_handles;
    int private_handle_cnt;
};

int nfp_device_open_local(struct nfp_device *dev, unsigned int devnum);
void nfp_device_close_local(struct nfp_device *dev);

int nfp_device_from_cpp_local(struct nfp_device *dev, struct nfp_cpp *cpp);

#ifdef __cplusplus
};
#endif

#endif /* __NFP_DEVICE_H__ */
