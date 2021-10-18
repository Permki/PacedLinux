/**
* Copyright (C) 2012 Netronome Systems, Inc.  All rights reserved.
*
* @file ns_ipc_server.c
* Implementation of IPC server functions.
*/
#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#ifndef _WIN32
#include <sys/prctl.h>
#include <sys/syscall.h>
#include <sys/fcntl.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <netinet/tcp.h>
#else
#include <winerror.h>
#endif

#include <sys/queue.h>
#include <pthread.h>

#include "uthash.h"

#include <nfp-common/ns_log.h>
#include "ns_error.h"
#include "ns_ipc_buf.h"
#include "ns_ipc_common.h"
#include "ns_ipc_server.h"
#include "ns_ipc_server_internal.h"

static int handle_msg(struct ns_ipc_server_connection *con,
                      ns_ipc_buf_t *in_buf,
                      ns_ipc_buf_t *out_buf)
{
    struct server_method_struct *s;
    char *method;
    uint32_t sequence;
    uint32_t type;
    size_t val_len;
    int ret = 0;

    ns_ipc_buf_unpack_tl(in_buf, &type, &val_len);
    if (type != NS_IPC_STR) {
        NS_LOG(NS_LOG_LVL_ERROR,"String expected. Type %d found", type);
        return 1;
    }

    method = (char *)malloc(val_len);
    if (method == NULL) {
        NS_LOG(NS_LOG_LVL_ERROR,"Out of memory");
        return -1;
    }

    ns_ipc_buf_unpack_data(in_buf, method, val_len);

    NS_LOG(NS_LOG_LVL_HEAVY,"Method is: %s", method);

    HASH_FIND_STR(con->srv->methods, method, s);
    if (s != NULL) {
        if (s->func != NULL) {
            NS_LOG(NS_LOG_LVL_HEAVY,"Calling stub");

            /* Clear the outbuf */
            ns_ipc_buf_clear(out_buf);

            (s->func)(con, out_buf, in_buf);

            /* Reflect the sequence */
            sequence = ((struct ns_ipc_header*)in_buf->buf)->sequence;

            pthread_mutex_lock(&con->send_mutex);
            ret = ns_ipc_send(con->ipc_socket,
                              sequence,
                              NS_IPC_MSG_SERVER_RESP,
                              out_buf);
            pthread_mutex_unlock(&con->send_mutex);

            if (ret < 0) {
                NS_LOG(NS_LOG_LVL_ERROR,"Could not send: %s", LOG_PERROR);
                ret = 1;
            }
        } else {
            NS_LOG(NS_LOG_LVL_ERROR,"Callback is NULL: %s", method);
            ret = 1;
        }
    } else {
        NS_LOG(NS_LOG_LVL_ERROR,"Unknown command: %s", method);

        /* Clear the outbuf */
        ns_ipc_buf_clear(out_buf);
        ns_ipc_buf_pack(out_buf,
                    PK_INT32(ENOSYS),
                    PK_INT32(-1),
                    PK_END);

        /* Reflect the sequence */
        sequence = ((struct ns_ipc_header*)in_buf->buf)->sequence;

        pthread_mutex_lock(&con->send_mutex);
        ret = ns_ipc_send(con->ipc_socket,
                          sequence,
                          NS_IPC_MSG_SERVER_RESP,
                          out_buf);
        pthread_mutex_unlock(&con->send_mutex);

        if (ret < 0) {
            NS_LOG(NS_LOG_LVL_ERROR,"Could not send: %s", LOG_PERROR);
            ret = 1;
        }
    }

    NS_LOG(NS_LOG_LVL_EXTRA,"Method %s returned %d", method, ret);
    free(method);

    return ret;
}

/* time-outs in ms */
#define RECV_INIT_TIMEOUT 50
/* time-outs in ms */
#define RECV_BUSY_TIMEOUT (20 * 1000)

static void *connection_handler(void *arg)
{
    ns_ipc_buf_t in_buf, out_buf;
    struct ns_ipc_server_connection* con = (struct ns_ipc_server_connection*)arg;
    struct ns_ipc_server *srv = con->srv;
    ns_ipc_server_conn_handle_t cl_h = con;
    int ret = 0;
#ifndef _WIN32
    char strbuf[128];
    snprintf(strbuf, 128, "ipc_server:cl%u", con->id);
    prctl(PR_SET_NAME, strbuf, 0, 0, 0);
#endif

    con->running = 1;
    con->done = 0;
    ns_ipc_buf_init(&in_buf);
    ns_ipc_buf_init(&out_buf);

    if (srv->cb_connect)
        (*srv->cb_connect)(cl_h, srv->cb_ctx);

    NS_LOG(NS_LOG_LVL_EXTRA, "Server Thread Starting: %p on socket %d", con, con->ipc_socket);
    while (con->running) {
        ns_ipc_buf_clear(&in_buf);
        ret = ns_ipc_receive(con->ipc_socket,
                             &in_buf,
                             RECV_INIT_TIMEOUT,
                             RECV_BUSY_TIMEOUT);

        if (ret < 0) {
#ifdef _WIN32
            if (errno == WSAEWOULDBLOCK)
#else
            if (errno == EWOULDBLOCK)
#endif
                continue;
            NS_LOG(NS_LOG_LVL_ERROR,"ns_ipc_receive failed: %s", strerror(errno));
            break;
        } else if (ret == 0) {
            NS_LOG(NS_LOG_LVL_DEBUG,"connection closed by client");
            break;
        }

        if (handle_msg(con, &in_buf, &out_buf) < 0) {
            con->running = 0;
        }
    }
    NS_LOG(NS_LOG_LVL_DEBUG,"Server Thread Stopping: %p", con);

    ns_ipc_buf_done(&in_buf);
    ns_ipc_buf_done(&out_buf);

    if (srv->cb_disconnect)
        (*srv->cb_disconnect)(cl_h, srv->cb_ctx);

    con->done = 1;

    if (!con->running) {
        /* if the server is shutting down wait for handshake so we
         * can free resources safely */
        while (!con->exit) {
#ifndef _WIN32
            usleep(1000);
#else
            Sleep(1000);
#endif
        }
    } else {
        /* if the client shut down remove ourselves from the con list */
        pthread_mutex_lock(&srv->server_connection_list_mutex);
        srv->clientcnt--;
        LIST_REMOVE(con, next);
        pthread_mutex_unlock(&srv->server_connection_list_mutex);
    }

    pthread_mutex_destroy(&con->send_mutex);
#ifdef _WIN32
    shutdown(con->ipc_socket, SD_BOTH);
    closesocket(con->ipc_socket);
#else
    close(con->ipc_socket);
#endif
    free(con);

    return NULL;
}

static void connection_init(struct ns_ipc_server_connection *con,
                            struct ns_ipc_server *srv,
                            socket_t sock)
{
#ifndef _WIN32
    bzero(con, sizeof(*con));
#else
    ZeroMemory(con, sizeof(*con));
#endif
    srv->clientcnt++;
    con->id = srv->clientid++;
    con->srv = srv;
    con->ipc_socket = sock;
    con->done = 0;
    con->exit = 0;
    pthread_mutex_init(&con->send_mutex, NULL);
}

int ns_ipc_server_accept_client(struct ns_ipc_server_handle *h)
{
    struct ns_ipc_server *srv = (struct ns_ipc_server *)h->data;
    struct ns_ipc_server_connection *connection = NULL;
    struct sockaddr_in client_addr;
    int client_len = sizeof(client_addr);
    int flag = 1;
    int ret;
    int fd;

    fd = accept(srv->listen_socket,
                (struct sockaddr*) &client_addr,
                (socklen_t*) &client_len);

#ifndef _WIN32
    NS_LOG(NS_LOG_LVL_DEBUG, "New connection on socket: %d", fd);
    if (fd < 0) {
        if (errno == EAGAIN) {
            NS_LOG(NS_LOG_LVL_DEBUG, "EAGAIN: %s retrying...", LOG_PERROR);
            return 1;
        }
        NS_LOG(NS_LOG_LVL_FATAL, "Accept error: %s Exiting", LOG_PERROR);
        return -1;
    }
#else
    NS_LOG(NS_LOG_LVL_DEBUG, "New connection on socket: %d", fd);
    if (fd == INVALID_SOCKET) {
        errno = WSAGetLastError();
        if (errno == WSAEWOULDBLOCK) {
            NS_LOG(NS_LOG_LVL_DEBUG, "WSAEWOULDBLOCK: retrying...");
            return 1;
        } else if (errno != 0) {
            NS_LOG(NS_LOG_LVL_FATAL, "Accept error: %d Exiting", errno);
            return -1;
        }
    }
#endif
    setsockopt(fd, IPPROTO_TCP, TCP_NODELAY, (char *) &flag, sizeof(int));

    NS_LOG(NS_LOG_LVL_DEBUG, "Client IP: %s", inet_ntoa(client_addr.sin_addr));

    /* Create a new server connection thread */

    connection = (struct ns_ipc_server_connection *)malloc(sizeof(*connection));
    if (!connection) {
        NS_LOG(NS_LOG_LVL_FATAL, "Out of memory");
        close(fd);
        return -1;
    }

    pthread_mutex_lock(&srv->server_connection_list_mutex);
    /* do this in the mutex, otherwise clientid/cnt may be wrong */
    connection_init(connection, srv, fd);
    LIST_INSERT_HEAD(&srv->connection_list, connection, next);
    pthread_mutex_unlock(&srv->server_connection_list_mutex);

    ret = pthread_create(&connection->thread, NULL, connection_handler, (void *)connection);
    if (ret) {
        NS_LOG(NS_LOG_LVL_FATAL, "pthread_create %d", ret);
        return -1;
    }

    ret = pthread_detach(connection->thread);
    if (ret) {
        NS_LOG(NS_LOG_LVL_WARN, "pthread detach failed %d", ret);
    }

    return 0;
}

static int send_event(struct ns_ipc_server_connection *con, ns_ipc_buf_t *b) {
    int ret;
    pthread_mutex_lock(&con->send_mutex);
    ret = ns_ipc_send(con->ipc_socket, 0, NS_IPC_MSG_SERVER_EVENT, b);
    pthread_mutex_unlock(&con->send_mutex);
    return ret;
}

int ns_ipc_server_send_event(struct ns_ipc_server_handle *h,
                             ns_ipc_server_conn_handle_t ch,
                             ns_ipc_buf_t *buf)
{
    struct ns_ipc_server *srv;
    struct ns_ipc_server_connection *cref = (struct ns_ipc_server_connection *)ch;
    struct ns_ipc_server_connection *c;
    int ret;
    int err = 0, found = 0;

    if (!h) {
        NS_LOG(NS_LOG_LVL_WARN, "null server handle passed");
        return -1;
    }

    srv = (struct ns_ipc_server *)(h->data);

    /* need to lock the tree down, the handle may disappear at any time */
    pthread_mutex_lock(&srv->server_connection_list_mutex);
    LIST_FOREACH(c, &srv->connection_list, next) {
        if (!cref || cref == c) {
            NS_LOG(NS_LOG_LVL_EXTRA, "Sending event to: %p", c);
            ret = send_event(c, buf);
            if (ret < 0) {
                NS_LOG(NS_LOG_LVL_FATAL,"Send event failed");
                /* if one fails try sending to others, but record error */
                err = NS_ERR_CLIENT_COMMS;
            } else {
                found = 1;
            }
        }
    }
    pthread_mutex_unlock(&srv->server_connection_list_mutex);

    if (err)
        return err;

    return found ? 0 : NS_ERR_NOT_FOUND;
}

int ns_ipc_server_register_method(struct ns_ipc_server_handle *h,
                                  const char *name,
                                  ns_ipc_server_stubfunc_t stub)
{
    struct ns_ipc_server *srv = (struct ns_ipc_server *)(h->data);
    struct server_method_struct *sf;

    sf = (struct server_method_struct *)malloc(sizeof(struct server_method_struct));

    if (!sf) {
        NS_LOG(NS_LOG_LVL_ERROR,"Out of memory");
        return NS_ERR_OUT_OF_MEMORY;
    }

    sf->name = name;
    sf->func = stub;
    pthread_mutex_lock(&srv->server_method_mutex);
    HASH_ADD_KEYPTR(hh, srv->methods, sf->name, strlen(sf->name), sf);
    pthread_mutex_unlock(&srv->server_method_mutex);
    return 0;
}

unsigned int ns_ipc_server_clientcnt(struct ns_ipc_server_handle *h)
{
    struct ns_ipc_server *srv = (struct ns_ipc_server *)(h->data);
    return srv->clientcnt;
}

int ns_ipc_server_get_fd(struct ns_ipc_server_handle *h)
{
    struct ns_ipc_server *srv = (struct ns_ipc_server *)(h->data);
    return srv->listen_socket;
}

int ns_ipc_server_set_callbacks(struct ns_ipc_server_handle *h,
                                void *opaque_context,
                                void (*connect)
                                    (ns_ipc_server_conn_handle_t h, void *ctx),
                                void (*disconnect)
                                    (ns_ipc_server_conn_handle_t h, void *ctx))
{
    struct ns_ipc_server *srv = (struct ns_ipc_server *)(h->data);
    if (!h) {
        NS_LOG(NS_LOG_LVL_FATAL, "NULL server handle passed");
        return NS_ERR_INVALID_PARAMETER;
    }

    srv->cb_ctx = opaque_context;
    srv->cb_connect = connect;
    srv->cb_disconnect = disconnect;
    return 0;
}
#ifdef _WIN32
/* NOTE: XP doesn't have inet_pton() so in order to support it (and wine)
         we include a local implementation here */
static int local_inet_pton(int af, const char *src, void *dst)
{
    struct sockaddr_storage ss;
    int size = sizeof(ss);
    char src_copy[INET6_ADDRSTRLEN+1];

    ZeroMemory(&ss, sizeof(ss));
    /* stupid non-const API */
    strncpy (src_copy, src, INET6_ADDRSTRLEN+1);
    src_copy[INET6_ADDRSTRLEN] = 0;

    if (WSAStringToAddressA(src_copy, af, NULL, (struct sockaddr *)&ss, &size) == 0) {
        switch(af) {
            case AF_INET:
                *(struct in_addr *)dst = ((struct sockaddr_in *)&ss)->sin_addr;
                return 1;
            case AF_INET6:
                *(struct in6_addr *)dst = ((struct sockaddr_in6 *)&ss)->sin6_addr;
                return 1;
        }
    }
    return 0;
}
#define INET_PTON local_inet_pton
#else
#define INET_PTON inet_pton
#endif

int ns_ipc_server_init(struct ns_ipc_server_handle *h, const char *srcip, int port)
{
    struct ns_ipc_server *srv;
    struct sockaddr_in serv_addr;
    int opt = 1;
#ifdef _WIN32
    int ret;
#endif

    if (!h) {
        NS_LOG(NS_LOG_LVL_FATAL, "NULL server handle passed");
        return NS_ERR_INVALID_PARAMETER;
    }

    srv = (struct ns_ipc_server *)malloc(sizeof(*srv));
    if (!srv) {
        NS_LOG(NS_LOG_LVL_FATAL, "Failed to allocate server handle data");
        return NS_ERR_OUT_OF_MEMORY;
    }
    h->data = srv;

#ifdef _WIN32
    if (ns_ipc_wsa_startup())
        return NS_ERR_FAIL;
#endif

#ifndef _WIN32
    bzero(srv,sizeof(*srv));
#else
    ZeroMemory(srv,sizeof(*srv));
#endif
    srv->port = port;

    srv->cb_connect = NULL;
    srv->cb_disconnect = NULL;
    srv->clientid = 0;
    srv->clientcnt = 0;

    pthread_mutex_init(&srv->server_method_mutex, NULL);
    pthread_mutex_init(&srv->server_connection_list_mutex, NULL);

    LIST_INIT(&srv->connection_list);

#ifndef _WIN32
    bzero((char*) &serv_addr, sizeof (serv_addr));
#else
    ZeroMemory((char*) &serv_addr, sizeof (serv_addr));
#endif
    if (!srcip || strlen(srcip) == 0) {
        serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    } else {
        if (INET_PTON(AF_INET, srcip, &serv_addr.sin_addr) != 1) {
            NS_LOG(NS_LOG_LVL_WARN, "Could not listen on src IP: %s", srcip);
            serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
        }
    }
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(srv->port);

    if ((srv->listen_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {

#ifndef _WIN32
        NS_LOG(NS_LOG_LVL_FATAL, "Could not open socket: %s Exiting", LOG_PERROR);
#else
        ret = WSAGetLastError();
        NS_LOG(NS_LOG_LVL_FATAL, "Could not open socket: %d Exiting", ret);
#endif
        free(srv);
        return NS_ERR_FAIL;
    }

#ifndef _WIN32
    if (setsockopt(srv->listen_socket,
                   SOL_SOCKET, SO_REUSEADDR,
                   (int*) &opt, sizeof (opt)) != 0) {
#else
    if (setsockopt(srv->listen_socket,
                   SOL_SOCKET, SO_REUSEADDR,
                   (const char *) &opt, sizeof (opt)) != 0) {
#endif
        NS_LOG(NS_LOG_LVL_FATAL, "Could not set opt: %s", LOG_PERROR);
        free(srv);
        return NS_ERR_FAIL;
    }

    if (bind(srv->listen_socket,
                    (struct sockaddr *) &serv_addr,
                    sizeof (serv_addr)) < 0) {
        NS_LOG(NS_LOG_LVL_FATAL, "Could not bind: %s Exiting", LOG_PERROR);
        free(srv);
        return NS_ERR_FAIL;
    }

    if (listen(srv->listen_socket, LISTEN_BACKLOG) < 0) {
        NS_LOG(NS_LOG_LVL_FATAL, "Could not listen: %s Exiting", LOG_PERROR);
        free(srv);
        return NS_ERR_FAIL;
    }

    NS_LOG(NS_LOG_LVL_INFO, "server listening on port %d", srv->port);

    return 0;
}

void ns_ipc_server_close(struct ns_ipc_server_handle *h)
{
    struct ns_ipc_server *srv = (struct ns_ipc_server *)(h->data);
    struct ns_ipc_server_connection *c;
    struct server_method_struct *tmp,*s;

    if (!h) {
        NS_LOG(NS_LOG_LVL_FATAL, "NULL ns_ipc_server passed");
        return;
    }

    /* Free the hash table contents */
    pthread_mutex_lock(&srv->server_method_mutex);
    HASH_ITER(hh, srv->methods, s, tmp) {
      HASH_DEL(srv->methods, s);
      free(s);
    }
    pthread_mutex_unlock(&srv->server_method_mutex);

    /* Close the connections */
    pthread_mutex_lock(&srv->server_connection_list_mutex);
    while((c = LIST_FIRST(&srv->connection_list)) != NULL) {
        NS_LOG(NS_LOG_LVL_DEBUG, "Freeing connection: %p", c);
        LIST_REMOVE(c, next);
        c->running = 0; /* Signal thread should shutdown */
        NS_LOG(NS_LOG_LVL_INFO,"Waiting for connection to terminate");
        while (c->done == 0) {
#ifndef _WIN32
            usleep(1000);
#else
            Sleep(1000);
#endif
        }
        c->exit = 1;
        NS_LOG(NS_LOG_LVL_INFO,"Done freeing connection: %p", c);
        /* all resource freeing happens on spawned thread */
    }
    pthread_mutex_unlock(&srv->server_connection_list_mutex);
    free(h->data);

#ifdef _WIN32
    ns_ipc_wsa_cleanup();
#endif
}
