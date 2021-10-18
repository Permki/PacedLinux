/*
 * Copyright (C) 2014, Netronome Systems, Inc.  All rights reserved.
 */

#ifndef __NFP_HWDBG_HANDLES_H__
#define __NFP_HWDBG_HANDLES_H__

#include "ns_ipc_buf.h"
#include "ns_ipc_server.h"

int _init_nfp_hwdbg_open(void *);
void _free_nfp_hwdbg_open(void *);

struct nfp_hwdbg {
    int dummy; /* nothing in here */
};

#endif /* __NFP_HWDBG_HANDLES_H__ */
