/*
 * Copyright (C) 2014, Netronome Systems, Inc.  All rights reserved.
 */

#ifndef __NFP_CPP_HANDLES_H__
#define __NFP_CPP_HANDLES_H__

#include "ns_ipc_buf.h"
#include "ns_ipc_server.h"

/* These functions are called automatically when a client connects and
 * disconnects; they perform extra init or free a handle
 */
int _init_nfp_cpp_open(void *);
void _free_nfp_cpp_open(void *);

int _init_nfp_cpp_from_device_id(void *);
void _free_nfp_cpp_from_device_id(void *);

int _init_nfp_cpp_area_alloc(void *);
void _free_nfp_cpp_area_alloc(void *);

int _init_nfp_cpp_area_alloc_with_name(void *);
void _free_nfp_cpp_area_alloc_with_name(void *);

int _init_nfp_cpp_area_alloc_acquire(void *);
void _free_nfp_cpp_area_alloc_acquire(void *);

int _init_nfp_cpp_mutex_alloc(void *h);
void _free_nfp_cpp_mutex_alloc(void *h);

int _init_nfp_cpp_event_alloc(void *h);
void _free_nfp_cpp_event_alloc(void *h);

#endif /* __NFP_CPP_HANDLES_H__ */
