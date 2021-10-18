/**
 * Copyright (C) 2012 Netronome Systems, Inc.  All rights reserved.
 *
 * Implementation of [LOCAL] functions from nfp_sal.interface.h
 *
 */
#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include <nfp-common/ns_log.h>
#include <nfp_cpp.h>

#include <ns_ipc_buf.h>
#include <ns_ipc_client.h>

#include "nfp_cpp_handles.h"

#ifdef _MSC_VER    /* This block is specific to MS VC++. */
    #include <Windows.h>
    #include <winerror.h>
    #define EPROTO WSAEPROTOTYPE
    #define SET_ERROR(x) do { SetLastError(x); } while(0)
#else
    #define SET_ERROR(x) do { errno = x; } while(0)
#endif

int nfp_cpp_from_device_id_local(struct nfp_cpp *nfp_cpp, int id)
{
    return 0;
}

void nfp_cpp_free_local(struct nfp_cpp *nfp_cpp)
{
    handle_common_free(&nfp_cpp->h_common);
    return;
}

int nfp_cpp_area_alloc_local(struct nfp_cpp_area *area,
                             struct nfp_cpp *cpp,
                             uint32_t cpp_id,
                             unsigned long long address,
                             unsigned long size)
{
    area->cpp = cpp;
    area->name = NULL;
    return 0;
}

int nfp_cpp_area_alloc_with_name_local(struct nfp_cpp_area *area,
                                       struct nfp_cpp *cpp,
                                       uint32_t cpp_id,
                                       const char *name,
                                       unsigned long long address,
                                       unsigned long size)
{
    area->cpp = cpp;
    area->name = strdup(name);
    return 0;
}

void nfp_cpp_area_free_local(struct nfp_cpp_area *area)
{
    if (area->name) {
        free(area->name);
        area->name = NULL;
    }
    handle_common_free(&area->h_common);
    return;
}

int nfp_cpp_area_alloc_acquire_local(struct nfp_cpp_area *area,
                                     struct nfp_cpp *cpp,
                                     uint32_t cpp_id,
                                     unsigned long long address,
                                     unsigned long size)
{
    area->cpp = cpp;
    area->name = NULL;
    return 0;
}

void nfp_cpp_area_release_free_local(struct nfp_cpp_area *area)
{
    if (area->name) {
        free(area->name);
        area->name = NULL;
    }
    handle_common_free(&area->h_common);
    return;
}

void * nfp_cpp_area_mapped(struct nfp_cpp_area *area)
{
    SET_ERROR(ENOTSUP);
    return NULL; /* this is not possible for the remoted API */
}

struct nfp_cpp *nfp_cpp_area_cpp(struct nfp_cpp_area *area)
{
    return area->cpp;
}

const char *nfp_cpp_area_name(struct nfp_cpp_area *cpp_area)
{
    return cpp_area->name;
}

/*********** CPP Event stuff *************/

struct nfp_cpp *nfp_cpp_event_cpp(struct nfp_cpp_event *cpp_event)
{
    return cpp_event->cpp;
}

struct sigaction;
int nfp_cpp_event_as_signal(struct nfp_cpp_event *event,
                            int signum,
                            const struct sigaction *act)
{
    SET_ERROR(ENOTSUP);
    return -1;
}

int nfp_cpp_event_alloc_local(struct nfp_cpp_event *event,
                              struct nfp_cpp *cpp,
                              uint32_t event_match,
                              uint32_t event_mask,
                              int type)
{
    event->cpp = cpp;
    return 0;
}

void nfp_cpp_event_free_local(struct nfp_cpp_event *event)
{
    handle_common_free(&event->h_common);
    return;
}

/*********** Mutex stuff ****************/

int nfp_cpp_mutex_alloc_local(struct nfp_cpp_mutex *mutex,
                              struct nfp_cpp *cpp,
                              int target,
                              unsigned long long address,
                              uint32_t key_id)
{
    mutex->cpp = cpp;
    return 0;
}

void nfp_cpp_mutex_free_local(struct nfp_cpp_mutex *mutex)
{
    handle_common_free(&mutex->h_common);
    return;
}

/* */

uint32_t nfp_cpp_local_imb_cppat(struct nfp_cpp *cpp, int cpp_tgt)
{
    uint32_t val;
    uint32_t xpbaddr = 0x010a0000 + (cpp_tgt * 4);
    /* lets just return what is in the arm island */

    if (cpp_tgt > 15 || cpp_tgt < 0) {
        errno = EINVAL;
        return UINT32_MAX;
    }

    if (nfp_xpb_readl(cpp, xpbaddr, &val) < 0) {
        errno = EIO;
        return UINT32_MAX;
    }

    return val;
}

/* this serial is typically stored in pcie IP csrs
 * and retrieved by the nfp driver; just hard code it
 * in theory we could implement this on the server
 */
static uint8_t cpp_serial[] = {0x0, 0x1, 0x2, 0x3, 0x4, 0x5};

int nfp_cpp_serial(struct nfp_cpp *cpp, const uint8_t **serial)
{
    *serial = cpp_serial;
    return sizeof(cpp_serial);
}

void nfp_cpp_vtrace_err(struct nfp_cpp *cpp, const char *format, va_list args)
{
}

void nfp_cpp_trace_err(struct nfp_cpp *cpp, const char *format, ...)
{
}

int nfp_cpp_vtrace(struct nfp_cpp *cpp, const char *format, va_list args)
{
    return 0;
}

int nfp_cpp_trace(struct nfp_cpp *cpp, const char *format, ...)
{
    return 0;
}
