/**
* Copyright (C) 2012 Netronome Systems, Inc.  All rights reserved.
*
* @file ns_ipc_client.h
* IPC client API.
*/
#ifndef _NS_IPC_CLIENT_H_
#define _NS_IPC_CLIENT_H_

#ifdef __cplusplus
extern "C" {
#endif

struct ns_ipc_client_handle {
    void *data;
    void *opaque;
};

int ns_ipc_client_send_receive(struct ns_ipc_client_handle *h,
                               ns_ipc_buf_t **b_in,
                               ns_ipc_buf_t *b_out);

int ns_ipc_client_set_event_handler(struct ns_ipc_client_handle *h,
                                    void *(*event)(ns_ipc_buf_t *, void *opaque),
                                    void *opaque);

int ns_ipc_client_event_queue_level(struct ns_ipc_client_handle *h);

int ns_ipc_client_init(struct ns_ipc_client_handle *h, char *url);
void ns_ipc_client_close(struct ns_ipc_client_handle *h);

#ifdef __cplusplus
};
#endif

#endif  /* __IPC_CLIENT_H__ */
