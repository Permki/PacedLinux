/*
 * Copyright (C) 2014, Netronome Systems, Inc.  All rights reserved.
 */

#ifndef __IPC_CLIENT_H__
#define __IPC_CLIENT_H__

/* data relating to a registered client handle */
struct ipc_client_handle {
    void *data;
    void (*free_cb)(void *data);
    struct ipc_client_handle *next;
};

/* data relating to a registered client interest */
struct ipc_client_interest {
    void *data;
    void (*disconnect_cb)(void *data, ns_ipc_server_conn_handle_t con);
    struct ipc_client_interest *next;
};

/* IPC client connection data */
struct ipc_client_data {
    /* connection handle for the client */
    ns_ipc_server_conn_handle_t con;
    /* list of registered client handles */
    struct ipc_client_handle *h_list;
    /* list of registered interests */
    struct ipc_client_interest *i_list;
    struct ipc_client_data *next;
};

/* IPC client handle data */
struct ipc_client {
    /* list of all clients connected */
    struct ipc_client_data *cl_list;
    pthread_mutex_t cl_mtx;
};

/************ IPC Client API ************/

/* Initialize an ipc client handle */
struct ipc_client *ipc_client_init();
/* Free an ipc client handle */
void ipc_client_free(struct ipc_client *ic);
/* Register a new client */
int ipc_client_add_client(struct ipc_client *ic,
                          ns_ipc_server_conn_handle_t con);
/* Delete a client */
int ipc_client_del_client(struct ipc_client *ic,
                          ns_ipc_server_conn_handle_t con);

/* add a client handle, also register destructor and init callbacks */
int ipc_client_add_handle(struct ipc_client *ic,
                          ns_ipc_server_conn_handle_t con,
                          void *handle,
                          int (*init_cb)(void *h),
                          void (*free_cb)(void *data));

/* delete a specific client handle (calls the destructor) */
int ipc_client_del_handle(struct ipc_client *ic,
                          ns_ipc_server_conn_handle_t con,
                          void *h);

/* Register an action to perform when a client disconnects */
int ipc_client_add_interest(struct ipc_client *ic,
                            ns_ipc_server_conn_handle_t con,
                            void *handle,
                            void (*disc_cb) (void *handle,
                                             ns_ipc_server_conn_handle_t con));
/* Delete an interest callback */
int ipc_client_del_interest(struct ipc_client *ic,
                            ns_ipc_server_conn_handle_t con,
                            void *handle);

/* check if a handle is already associated with a connection
 * returns 1 if found, 0 if not
 */
int ipc_client_check_handle(struct ipc_client *ic,
                            ns_ipc_server_conn_handle_t con,
                            void *handle);

/* retrieve the connection handle for a client handle */
ns_ipc_server_conn_handle_t
ipc_client_get_con(struct ipc_client *ic, void *handle);

/************ Connection call-backs ************/

/* function for connect to be register as callbacks with IPC server handle */
void ipc_client_connect(ns_ipc_server_conn_handle_t h, void *ctx);

/* function for disconnect to be register as callbacks with IPC server handle */
void ipc_client_disconnect(ns_ipc_server_conn_handle_t h, void *ctx);

#endif
