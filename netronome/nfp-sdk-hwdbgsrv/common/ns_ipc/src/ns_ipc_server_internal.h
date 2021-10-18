/*
 * Copyright (C) 2014, Netronome Systems, Inc.  All rights reserved.
 */

#ifndef __NS_IPC_SERVER_INTERNAL_H__
#define __NS_IPC_SERVER_INTERNAL_H__

/* IPC method structure */

struct ns_ipc_server; /* forward declaration for ns_ipc_server_connection */

struct ns_ipc_server_connection {
    struct ns_ipc_server *srv; /* parent server data */
    socket_t ipc_socket;
    struct sockaddr_in client_addr;
    int running;
    int done;
    int exit;
    unsigned int id;
    pthread_t thread;
    pthread_mutex_t send_mutex;

    LIST_ENTRY(ns_ipc_server_connection) next;
};

struct server_method_struct {
    const char *name;          /* key */
    ns_ipc_server_stubfunc_t func;
    UT_hash_handle hh;         /* makes this struct hashable */
};

/* data for server instance */

struct ns_ipc_server {
    socket_t listen_socket;
    int port;
    unsigned int clientid;
    unsigned int clientcnt;

    /* client connect/disconnect callbacks */
    void *cb_ctx; /* context to send to callbacks */
    void (*cb_connect)(ns_ipc_server_conn_handle_t h, void *ctx);
    void (*cb_disconnect)(ns_ipc_server_conn_handle_t h, void *ctx);

    pthread_mutex_t server_method_mutex;
    struct server_method_struct *methods;

    pthread_mutex_t server_connection_list_mutex;
    LIST_HEAD(a, ns_ipc_server_connection) connection_list;
};

#endif /* __NS_IPC_SERVER_INTERNAL_H__ */

