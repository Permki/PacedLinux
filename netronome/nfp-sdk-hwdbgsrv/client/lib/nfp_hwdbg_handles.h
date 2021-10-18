/**
 * Copyright (C) 2012 Netronome Systems, Inc.  All rights reserved.
 *
 * Interim NFP device handle header
 *
 */

#ifndef __NFP_HWDBG_HANDLES_H__
#define __NFP_HWDBG_HANDLES_H__

#include "nfp_client_common.h"

#ifdef __cplusplus
extern "C" {
#endif

struct nfp_hwdbg {
    struct ns_ipc_client_handle ipc_handle;
    struct ns_ipc_client_handle *ipc_h;
    int own_handle;
    uint64_t rh; /* remote handle */
    struct handle_common h_common;
    int id;
};

int nfp_hwdbg_open_local(struct nfp_hwdbg *hwdbg, unsigned int devnum);
void nfp_hwdbg_close_local(struct nfp_hwdbg *hwdbg);

#ifdef __cplusplus
};
#endif

#endif /* __NFP_HWDBG_HANDLES_H__ */
