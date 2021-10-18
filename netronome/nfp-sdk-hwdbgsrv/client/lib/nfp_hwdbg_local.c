/**
 * Copyright (C) 2012 Netronome Systems, Inc.  All rights reserved.
 *
 * Implementation of [LOCAL] functions from nfp_hwdbg.interface.h
 *
 */
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <nfp_hwdbg.h>
#include <pthread.h>
#include <nfp-common/ns_log.h>
#include <nfp_hwdbg.h>
#include <ns_ipc_buf.h>
#include <ns_ipc_client.h>
#include "nfp_hwdbg_handles.h"

#define MAX_SUBS_PER_EVENT_TYPE 8
#define EVENTLAST NFP_HWDBG_EVENT_TYPE_LAST

int nfp_hwdbg_open_local(struct nfp_hwdbg *hwdbg, unsigned int devnum)
{
    return 0;
}

void nfp_hwdbg_close_local(struct nfp_hwdbg *hwdbg)
{
    handle_common_free(&hwdbg->h_common);
    return;
}

/******** Events stuff *********/

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)

#define HANDLE_EVENT(NM,VN)                                                 \
        case NM:                                                            \
            if (ev_data_len >= sizeof(struct VN)) {                         \
                for (i = (NM + 1)* MAX_SUBS_PER_EVENT_TYPE; i < (NM + 2) * MAX_SUBS_PER_EVENT_TYPE; i++) { \
                    if (dev->h_common.callbacks[i] != NULL) {               \
                        ((void (*)(struct nfp_hwdbg *, struct VN *))(dev->h_common.callbacks[i]))(dev, (struct VN *)ev_data); \
                    }                                                       \
                }                                                           \
            } else {                                                        \
                NS_LOG(NS_LOG_LVL_WARN,"Event data mismatch for event: " TOSTRING(NM)); \
                NS_LOG(NS_LOG_LVL_INFO, "Using struct " TOSTRING(VN)); \
                NS_LOG(NS_LOG_LVL_INFO, "Data length=%d, struct size=%d", (int) ev_data_len, (int) sizeof(struct VN));   \
            }                                                               \
        break

static void callbacks_init(struct nfp_hwdbg *dev)
{
    int l = MAX_SUBS_PER_EVENT_TYPE * (EVENTLAST + 1);
    if (dev->h_common.callbacks)
        return;
    dev->h_common.callbacks = malloc(sizeof(*(dev->h_common.callbacks)) * l);
    if (!dev->h_common.callbacks) {
        NS_LOG(NS_LOG_LVL_ERROR, "Failure while allocating ipc callbacks");
        return;
    }
    memset(dev->h_common.callbacks, '\0', sizeof(*(dev->h_common.callbacks)) * l);
}

/* TODO - standardize callback calling convention */
static void *ipc_callback(ns_ipc_buf_t *buf, void *opaque)
{
    uint32_t seq, ev_type;
    uint8_t *ev_data;
    size_t ev_data_len;
    struct nfp_hwdbg *dev;
    int i;

    if (opaque) {
        dev = (struct nfp_hwdbg *)opaque;
    } else {
        NS_LOG(NS_LOG_LVL_FATAL,"Device handle is NULL - not processing any callbacks");
        return NULL;
    }
    ns_ipc_buf_unpack(buf, UP_UINT32(seq), UP_UINT32(ev_type),
                           UP_BYTES(ev_data, ev_data_len), UP_END);
    NS_LOG(NS_LOG_LVL_DEBUG,"Received event Seq:%d Type:%d Parameter size:%zd dev %p\n", seq, ev_type, ev_data_len, dev);
    pthread_mutex_lock(&dev->h_common.callbacks_mutex); /* Keep lock during callback processing - change when this becomes and issue for anyone */
    callbacks_init(dev);
    switch (ev_type) {
        HANDLE_EVENT(NFP_HWDBG_EVENT_TYPE_BREAK, nfp_hwdbg_break_data);
        HANDLE_EVENT(NFP_HWDBG_EVENT_TYPE_GROUPSTOP, nfp_hwdbg_event_pl_groupstop);
        HANDLE_EVENT(NFP_HWDBG_EVENT_TYPE_SHUTDOWN, nfp_hwdbg_event_pl_shutdown);
        HANDLE_EVENT(NFP_HWDBG_EVENT_TYPE_RESET, nfp_hwdbg_event_pl_reset);
        default:
            NS_LOG(NS_LOG_LVL_WARN,"Unknown event received: %d",ev_type);
    }
    for (i = 0; i < MAX_SUBS_PER_EVENT_TYPE; i++) {
        if (dev->h_common.callbacks[i] != NULL) {
            ((void (*)(struct nfp_hwdbg *, uint32_t, void *, uint32_t ))dev->h_common.callbacks[i])(dev, ev_type, ev_data, ev_data_len);
        }
    }
    pthread_mutex_unlock(&dev->h_common.callbacks_mutex);
    return NULL;
}

int nfp_hwdbg_subs_events_all(struct nfp_hwdbg *dev, void (*callback)(struct nfp_hwdbg *dev, uint32_t ev_type, void *ev_data, uint32_t ev_data_len))
{
    struct ns_ipc_client_handle *ipc = dev->ipc_h;
    int i, rc;
    if (dev == NULL) {
        return -1;
    }

    rc = ns_ipc_client_set_event_handler(ipc, ipc_callback, (void*)dev);
    if (rc < 0) {
        return rc;
    }
    pthread_mutex_lock(&dev->h_common.callbacks_mutex);
    callbacks_init(dev);
    for (i = 0; i < MAX_SUBS_PER_EVENT_TYPE; i++) {
        if (dev->h_common.callbacks[i] == NULL) {
            dev->h_common.callbacks[i] = (void (*)(void*))callback;
            break;
        }
    }
    pthread_mutex_unlock(&dev->h_common.callbacks_mutex);
    if (i == MAX_SUBS_PER_EVENT_TYPE) {
        return -2;
    } else {
        return i;
    }
}

#define HANDLE_EVENT_SUBS(NM,VN)                                                            \
int nfp_hwdbg_subs_##VN(struct nfp_hwdbg *dev, void (*callback)(struct nfp_hwdbg *, struct VN *)) \
{                                                                                           \
    int i, rc;                                                                              \
    if (dev == NULL) {                                                                      \
        return -1;                                                                          \
    }                                                                                       \
    rc = ns_ipc_client_set_event_handler(dev->ipc_h, ipc_callback, (void*)dev);            \
    if (rc < 0) {                                                                           \
        return rc;                                                                          \
    }                                                                                       \
    pthread_mutex_lock(&dev->h_common.callbacks_mutex);                                     \
    callbacks_init(dev); \
    for (i = (NM + 1) * MAX_SUBS_PER_EVENT_TYPE; i < (NM + 2) * MAX_SUBS_PER_EVENT_TYPE; i++) { \
        if (dev->h_common.callbacks[i] == NULL) {                                           \
            dev->h_common.callbacks[i] = (void (*)(void*))callback;                         \
            break;                                                                          \
        }                                                                                   \
    }                                                                                       \
    pthread_mutex_unlock(&dev->h_common.callbacks_mutex);                                   \
    if (i ==  (NM + 2) * MAX_SUBS_PER_EVENT_TYPE) {                                         \
        return -2;                                                                          \
    } else {                                                                                \
        return i;                                                                           \
    }                                                                                       \
}

int nfp_hwdbg_unsubs_event(struct nfp_hwdbg *dev, int handler_no) {
    int rc = 0;
    if ((handler_no < 0) || (handler_no >= (EVENTLAST + 1) * MAX_SUBS_PER_EVENT_TYPE)) {
        return -1; /* Invalid handle */
    }
    pthread_mutex_lock(&dev->h_common.callbacks_mutex);
    callbacks_init(dev);
    /* All events handles */
    if (dev->h_common.callbacks[handler_no] != NULL) {
        dev->h_common.callbacks[handler_no] = NULL;
    } else {
        rc = -2;
    }
    pthread_mutex_unlock(&dev->h_common.callbacks_mutex);
    return rc;
}
