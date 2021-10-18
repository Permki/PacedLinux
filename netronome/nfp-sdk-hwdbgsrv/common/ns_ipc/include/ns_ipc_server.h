/**
* Copyright (C) 2012 Netronome Systems, Inc.  All rights reserved.
*
* @file ns_ipc_server.h
* IPC server API.
*/
#ifndef _NS_IPC_SERVER_H_
#define _NS_IPC_SERVER_H_

#ifdef __cplusplus
extern "C" {
#endif

struct ns_ipc_server_handle {
    void *data;
};

typedef void * ns_ipc_server_conn_handle_t;

typedef void (*ns_ipc_server_stubfunc_t)(ns_ipc_server_conn_handle_t,
                                         ns_ipc_buf_t *,
                                         ns_ipc_buf_t *);

/* either specify the client handle or NULL for all clients */
int ns_ipc_server_send_event(struct ns_ipc_server_handle *h,
                             ns_ipc_server_conn_handle_t ch,
                             ns_ipc_buf_t *buf);

int ns_ipc_server_register_method(struct ns_ipc_server_handle *h,
                                  const char *name,
                                  ns_ipc_server_stubfunc_t);

int ns_ipc_server_accept_client(struct ns_ipc_server_handle *h);

/**
 * Register callbacks for client connect and disconnect, along with context to
 * pass with callbacks
 **/
int ns_ipc_server_set_callbacks(struct ns_ipc_server_handle *h,
                                void *opaque_context,
                                void (*connect)
                                    (ns_ipc_server_conn_handle_t h, void *ctx),
                                void (*disconnect)
                                    (ns_ipc_server_conn_handle_t h, void *ctx));

unsigned int ns_ipc_server_clientcnt(struct ns_ipc_server_handle *h);

int ns_ipc_server_get_fd(struct ns_ipc_server_handle *h);

int ns_ipc_server_init(struct ns_ipc_server_handle *h, const char *srcip, int port);
void ns_ipc_server_close(struct ns_ipc_server_handle *h);

#ifdef __cplusplus
};
#endif

#endif /* __IPC_SERVER_H__ */
