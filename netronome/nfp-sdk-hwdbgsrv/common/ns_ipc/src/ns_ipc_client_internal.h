/*
 * Copyright (C) 2014, Netronome Systems, Inc.  All rights reserved.
 */

#ifndef _NS_IPC_CLIENT_INTERNAL_H_
#define _NS_IPC_CLIENT_INTERNAL_H_

#include <sys/queue.h>

#ifdef __cplusplus
extern "C" {
#endif

#define MAX_PENDING_CALLS (16)

struct call_matcher {
    sem_t sem;
    uint32_t sequence;
    ns_ipc_buf_t *b_in;
};

#ifndef DISABLE_EVENT_QUEUE
struct eq_entry {
    STAILQ_ENTRY(eq_entry) next;
    ns_ipc_buf_t *buf;
};
#endif

struct ns_ipc_client_connection {
    socket_t ipc_socket;
    uint32_t running;
    uint32_t done;
    pthread_t socket_thread;
    pthread_mutex_t send_mutex;
    struct call_matcher cm[MAX_PENDING_CALLS];
    uint32_t sequence;
    void *(*event_handler)(ns_ipc_buf_t *, void *opaque);
    void *opaque;
#ifndef DISABLE_EVENT_QUEUE
    pthread_t event_thread;
    pthread_mutex_t eq_lock;
    pthread_cond_t  eq_not_empty;
    STAILQ_HEAD(event_queue, eq_entry) eq_head;
    uint32_t eq_level;
#endif
};

#ifdef __cplusplus
};
#endif

#endif  /* __NS_IPC_CLIENT_INTERNAL_H__ */

