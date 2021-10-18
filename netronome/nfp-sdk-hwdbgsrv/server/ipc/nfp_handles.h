/*
 * Copyright (C) 2014, Netronome Systems, Inc.  All rights reserved.
 */

#ifndef __NFP_HANDLES_H__
#define __NFP_HANDLES_H__

#include "ns_ipc_buf.h"
#include "ns_ipc_server.h"

int _init_nfp_device_open(void *);
void _free_nfp_device_open(void *);

int _init_nfp_device_from_cpp(void *);
void _free_nfp_device_from_cpp(void *);

#endif /* __NFP_HANDLES_H__ */
