/**
* Copyright (C) 2012 Netronome Systems, Inc.  All rights reserved.
*
* @file ns_ipc_client.c
* Implementation of IPC client functions.
*/
#include <ctype.h>
#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#ifdef _WIN32
#include <winerror.h>
#else
#include <sys/prctl.h>
#include <sys/syscall.h>
#include <sys/fcntl.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <netdb.h>
#endif

#include <semaphore.h>
#include <pthread.h>

#include <nfp-common/ns_log.h>
#include "ns_error.h"
#include "ns_ipc_buf.h"
#include "ns_ipc_common.h"
#include "ns_ipc_client.h"
#include "ns_ipc_client_internal.h"

/******* Client message call *******/

static int wait_response(struct ns_ipc_client_connection *con,
                         uint32_t seq,
                         ns_ipc_buf_t **b_in)
{
    /* Register for callback */
    struct call_matcher *cm = &con->cm[seq % MAX_PENDING_CALLS];

    if (con->done) /* FIXME: race condition here */
        return -1;

    NS_LOG(NS_LOG_LVL_EXTRA,"Waiting for server response on seq: %d", seq);

    /* Deschedule */
    sem_wait(&cm->sem);

    NS_LOG(NS_LOG_LVL_EXTRA,"Got server response on seq: %d", seq);

    /* Get data */
    *b_in = cm->b_in;

    if (!cm->b_in)
        return -1;

    return 0;
}

int ns_ipc_client_send_receive(struct ns_ipc_client_handle *h,
                               ns_ipc_buf_t **b_in,
                               ns_ipc_buf_t *b_out)
{
    struct ns_ipc_client_connection *con = (struct ns_ipc_client_connection *)(h->data);
    uint32_t seq;
    int ret;
    int ret_val;

    pthread_mutex_lock(&con->send_mutex);
    seq = con->sequence++;
    ret_val = ns_ipc_send(con->ipc_socket, seq, NS_IPC_MSG_CLIENT_CALL, b_out);
    pthread_mutex_unlock(&con->send_mutex);

    if (ret_val == -1)
        return ret_val;

    ret = wait_response(con, seq, b_in); /* TODO - add timeout */
    return ret;
}

/******* Receive handler thread *********/

static int response_received(struct ns_ipc_client_connection *con,
                             uint32_t seq,
                             ns_ipc_buf_t *b_in)
{
    /* Look for caller */
    struct call_matcher *cm = &con->cm[seq % MAX_PENDING_CALLS];

    /* Pass buffer */
    cm->b_in = b_in;
    sem_post(&cm->sem);
    return 0;
}

#ifndef DISABLE_EVENT_QUEUE

static int deliver_event(struct ns_ipc_client_connection *con, ns_ipc_buf_t *buf)
{
    struct eq_entry *e = (struct eq_entry* )malloc(sizeof(struct eq_entry));
    if (e == NULL) {
        NS_LOG(NS_LOG_LVL_FATAL,"Could not allocate memory for event delivery\n");
        return 0;
    }
    pthread_mutex_lock(&con->eq_lock);
    e->buf = buf;
    STAILQ_INSERT_TAIL(&con->eq_head, e, next);
    pthread_cond_signal(&con->eq_not_empty);
    con->eq_level++;
    pthread_mutex_unlock(&con->eq_lock);
    return 1;
}

static void handle_event(struct ns_ipc_client_connection *con)
{
    struct eq_entry *entry;

    entry = STAILQ_FIRST(&con->eq_head);
    STAILQ_REMOVE_HEAD(&con->eq_head, next);
    if (entry)
        con->eq_level--;
    pthread_mutex_unlock(&con->eq_lock);

    if (entry) {
        if (con->event_handler)
            con->event_handler(entry->buf, con->opaque);

        if (entry->buf) {
            ns_ipc_buf_done(entry->buf);
            free(entry->buf);
        }

        free(entry);
    }
}

static void *event_notifier(void* arg)
{
    struct ns_ipc_client_connection *con = (struct ns_ipc_client_connection *)arg;
#ifndef _WIN32
    char strbuf[128];
    snprintf(strbuf, 128, "ipc_client:evt");
    prctl(PR_SET_NAME, strbuf, 0, 0, 0);
#endif
    /* Wait for events in queue to deliver */
    while(1) {
        pthread_mutex_lock(&con->eq_lock);

        if (STAILQ_EMPTY(&con->eq_head) && con->running) {
            /* wait for data or thread kill wakeup */
            pthread_cond_wait(&con->eq_not_empty, &con->eq_lock);
        }

        /* thread kill */
        if (!con->running) {
            pthread_mutex_unlock(&con->eq_lock);
            break;
        }

        /* handle data */
        handle_event(con);
    }
    return NULL;
}
#endif


/* time-outs in ms */
#define RECV_INIT_TIMEOUT 10
/* time-outs in ms */
#define RECV_BUSY_TIMEOUT (20 * 1000)
static void *receive_handler(void *arg)
{
    struct ns_ipc_client_connection *con = (struct ns_ipc_client_connection *)arg;
    ns_ipc_buf_t *in_buf = NULL;
    struct ns_ipc_header *hdr;
    int ret;
    int client_shutdown = 0;
#ifndef _WIN32
    char strbuf[128];
    snprintf(strbuf, 128, "ipc_client:recv");
    prctl(PR_SET_NAME, strbuf, 0, 0, 0);
#endif

    con->done = 0;

    NS_LOG(NS_LOG_LVL_DEBUG, "Client Thread Starting: %p on socket %d", con, con->ipc_socket);
    while (con->running) {
        /* client will free in case of resp */
        in_buf = (ns_ipc_buf_t *) malloc(sizeof(*in_buf));
        if (in_buf == NULL) {
            NS_LOG(NS_LOG_LVL_FATAL,"Out of memory while allocating receive buffer");
            break;
        }
        ns_ipc_buf_init(in_buf);

        while (1) {
            if (!con->running) {
                client_shutdown = 1;
                break;
            }

            ret = ns_ipc_receive(con->ipc_socket,
                                 in_buf,
                                 RECV_INIT_TIMEOUT,
                                 RECV_BUSY_TIMEOUT);
#ifndef _WIN32
            if (ret < 0 && errno == EWOULDBLOCK)
#else
            if (ret < 0 && errno == WSAEWOULDBLOCK)
#endif
                continue;

            break;
        }

        if (client_shutdown)
            break;

        if (ret < 0) {
            NS_LOG(NS_LOG_LVL_ERROR,"ns_ipc_receive failed %d", ret);
            break;
        } else if (ret == 0) {
            NS_LOG(NS_LOG_LVL_WARN,"connection closed by server");
            break;
        }

        hdr = (struct ns_ipc_header*)in_buf->buf;

        if (hdr->type == NS_IPC_MSG_SERVER_RESP) {
            /* Process response */
            ret = response_received(con, hdr->sequence, in_buf);
            in_buf = NULL;
            if (ret < 0) {
                /* Unexpected sequence */
                NS_LOG(NS_LOG_LVL_FATAL,"Protocol error - "
                          "Unexpected sequence: %u", con->sequence);
                break;
            }
        } else if (hdr->type == NS_IPC_MSG_SERVER_EVENT) {
#ifdef DISABLE_EVENT_QUEUE
            if (con->event_handler) {
                con->event_handler(in_buf, con->opaque);
            }
            ns_ipc_buf_done(in_buf);
            free(in_buf);
#else
            if (con->event_handler) {
                deliver_event(con, in_buf);
            } else {
                ns_ipc_buf_done(in_buf);
                free(in_buf);
            }
#endif
            in_buf = NULL;
        } else {
            /* Error */
            NS_LOG(NS_LOG_LVL_FATAL,"Protocol error - Unexpected type: %u", hdr->type);
            break;
        }
    }
    { /* make sure all existing calls are interrupted */
        int i;
        for (i = 0; i < MAX_PENDING_CALLS; i++)
            response_received(con, i, NULL);
    }
#ifdef _WIN32
    shutdown(con->ipc_socket, SD_BOTH);
    closesocket(con->ipc_socket);
#else
    close(con->ipc_socket);
#endif
    NS_LOG(NS_LOG_LVL_DEBUG,"Client Thread Stopping: %p", con);
    if (in_buf) { /* XXX: how safe is this? */
        ns_ipc_buf_done(in_buf);
        free(in_buf);
    }
    con->done = 1;
    return NULL;
}
/***** Connect *****/

int check_url(char *url, struct sockaddr_in *server_addr)
{
    char *cp;
    struct hostent *he;
    struct in_addr **pptr;

    cp = strtok(url,":");

    if (cp) {
        if (strcmp(cp,"tcp") != 0) {
            NS_LOG(NS_LOG_LVL_ERROR,"URL should start with tcp");
            return -1;
        }

        cp = strtok(NULL,":");
        if (cp) {
            while (*cp && (*cp == '/')) { /* Step over */
                cp++;
            }

            he = gethostbyname(cp);
            if (he == NULL) {
                NS_LOG(NS_LOG_LVL_FATAL,"Could not find host: %s", cp);
                return -1;
            }
            pptr = (struct in_addr**)he->h_addr_list;

            memset(server_addr, '\0', sizeof(*server_addr));
            memcpy(&server_addr->sin_addr, *pptr, sizeof(struct in_addr));
            cp = strtok(NULL,":");
            if (cp) {
                server_addr->sin_port = htons(atoi(cp));
                NS_LOG(NS_LOG_LVL_DEBUG,"Using port: %s", cp);
            }
        }
    }
    return 0;
}

#define TIMEOUT_ENV "NS_IPC_CONNECT_TIMEOUT_MS"

static int get_timeout_env(void)
{
    char *var;
    const char *env_str = TIMEOUT_ENV;
#ifdef _WIN32
    char env_buf[1024];
    int ret;
    ret = GetEnvironmentVariableA(env_str, env_buf, sizeof(env_buf));
    if (ret)
        var = env_buf;
    else
        var = NULL;
#else
    var = getenv(env_str);
#endif
    if (!var)
        return -1;

    return atoi(var);
}

/* standard error references for Linux and Windows */
#ifdef _WIN32
#define ERRNO_ISCONN WSAEISCONN
#define ERRNO_CONNREFUSED WSAECONNREFUSED
#else
#define ERRNO_ISCONN EISCONN
#define ERRNO_CONNREFUSED ECONNREFUSED
#endif

/* connect poll delay is 10ms */
#define LOOPDELAY 10
static int do_connect(struct ns_ipc_client_connection *con,
                      const char *url,
                      int timeout_ms)
{
    struct sockaddr_in server_addr;
    int flags;
    char *url2 = NULL;
    int to_env, loop_delay, fatal_error;
    int ret = -1;

    /* set defaults */
    memset(&server_addr, '\0', sizeof(server_addr));
    server_addr.sin_addr.s_addr = INADDR_LOOPBACK;
    server_addr.sin_port = htons(LISTEN_DEFAULT_PORT);

    to_env = get_timeout_env();

    if (!url) {
        NS_LOG(NS_LOG_LVL_ERROR, "got NULL URL");
        return -1;
    }

    url2 = strdup(url);
    if (!url2) {
        NS_LOG(NS_LOG_LVL_ERROR, "Could not strdup()");
        return -1;
    }

#ifdef _WIN32
    if (ns_ipc_wsa_startup() == -1)
        return -1;
#endif

    ret = check_url(url2, &server_addr);
    if (ret){
        NS_LOG(NS_LOG_LVL_ERROR, "invalid URL");
        goto error;
    }

    NS_LOG(NS_LOG_LVL_DEBUG, "Connecting to Server: %s:%d",
                inet_ntoa(server_addr.sin_addr),
                htons(server_addr.sin_port));

    con->ipc_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (con->ipc_socket < 0) {
        NS_LOG(NS_LOG_LVL_ERROR, "socket() failed: %s", LOG_PERROR);
        goto error;
    }

    /* Make socket non-blocking */
#ifndef _WIN32
    flags = fcntl(con->ipc_socket, F_GETFL);
    if (fcntl(con->ipc_socket, F_SETFL, flags | O_NONBLOCK) != 0) {
        NS_LOG(NS_LOG_LVL_WARN, "Could not fcntl: %s", LOG_PERROR);
    }
#else
    {
        unsigned long p = 1;
        ret = ioctlsocket (con->ipc_socket, FIONBIO, &p);
        if (ret == SOCKET_ERROR) {
            NS_LOG(NS_LOG_LVL_WARN,
                   "FIONBIO failed: "
                   "Need to set socket to be non-blocking: %s", LOG_PERROR);
        }
    }
#endif

    server_addr.sin_family = AF_INET;

    if (to_env > 0)
        timeout_ms = to_env;

    /* we try to connect until the timeout is reached */
    while (1) {
        ret = connect(con->ipc_socket,
                      (struct sockaddr*)&server_addr,
                      sizeof(server_addr));

        if (!ret) /* success */
            break;

        if (ret < 0) {
#ifdef _WIN32
            errno = WSAGetLastError();
#endif
            /* check that the errno indicates success */
            if (errno == ERRNO_ISCONN) {
                ret = 0;
                break;
            }
        }

        loop_delay = 10; /* by default try again after 10ms */

        /* some errors we want to retry on */
        fatal_error = 0; /* default is non-fatal */
        switch (errno) {
        case ERRNO_CONNREFUSED:
            if (to_env < 0) {
                /* we only treat this as non-fatal if we have the
                 * envvar in place
                 */
                 fatal_error = 1;
                 break;
            }
            /* wait a while longer if we are retrying a refused connection */
            loop_delay = 1000;
#ifndef _WIN32 /* unix errcodes to keep trying on */
        case EAGAIN:
        case EALREADY:
        case EINPROGRESS:
        case EINTR:
        case ETIMEDOUT:
            break;
#else /* windows errcodes to keep trying on */
        case WSAEWOULDBLOCK:
        case WSAEINPROGRESS:
        case WSAEALREADY:
        case WSAEINVAL:
            break;
#endif
        default: /* fatal */
            fatal_error = 1;
        }

        /* dont retry on a fatal error */
        if (fatal_error)
            break;

        /* we are done if the timeout value is reached */
        if (timeout_ms <= 0)
            break;

        if (loop_delay > timeout_ms)
            loop_delay = timeout_ms;

        timeout_ms -= loop_delay;

        /* wait a while before trying to connect again */
#ifdef _WIN32
        Sleep(loop_delay);
#else
        usleep(loop_delay * 1000);
#endif
    }

    if (ret < 0) {
        NS_LOG(NS_LOG_LVL_ERROR,
               "Could not connect to server: %s [%d]",
               strerror(errno), errno);
        goto error;
    }

    /* Change socket back to blocking */
#ifndef _WIN32
    if (fcntl(con->ipc_socket, F_SETFL, flags) != 0) {
        NS_LOG(NS_LOG_LVL_FATAL,"Could not fcntl: %s", LOG_PERROR);
        goto error;
    }
#else
    {
        unsigned long p = 1;

        ret = ioctlsocket (con->ipc_socket, FIONBIO, &p);
        if (ret == SOCKET_ERROR) {
            NS_LOG(NS_LOG_LVL_FATAL,
                   "Failed to set socket to blocking: %s", LOG_PERROR);
            goto error;
        }

        /* TCP_NODELAY for windows - attempt to get around latency issue */
        flags = 1;
        setsockopt(con->ipc_socket,
                   IPPROTO_TCP, TCP_NODELAY,
                   (char *) &flags, sizeof(int));
    }
#endif

    free(url2);
    return 0;

error:
#ifdef _WIN32
    ns_ipc_wsa_cleanup();
#endif
    if (url2)
        free(url2);

    return -1;
}

/***** API bits *****/

int ns_ipc_client_set_event_handler(struct ns_ipc_client_handle *h,
                                    void *(*event)(ns_ipc_buf_t *, void *opaque),
                                    void *opaque)
{
    struct ns_ipc_client_connection *con = (struct ns_ipc_client_connection *)(h->data);
    con->event_handler = event;
    con->opaque = opaque;
    return 0;
}

int ns_ipc_client_event_queue_level(struct ns_ipc_client_handle *h)
{
#ifndef DISABLE_EVENT_QUEUE
    struct ns_ipc_client_connection *con = (struct ns_ipc_client_connection *)(h->data);
    return con->eq_level;
#else
    return 0;
#endif
}

/**** Init/free ****/

static void connection_init(struct ns_ipc_client_connection *con)
{
    int i;

    memset(con, '\0', sizeof(*con));
    for (i = 0; i < MAX_PENDING_CALLS; i++) {
        sem_init(&con->cm[i].sem, 0, 0);
    }
    pthread_mutex_init(&con->send_mutex, NULL);
}

/* by default we wait 10s for a connection */
int ns_ipc_client_init(struct ns_ipc_client_handle *h, char *url)
{
    struct ns_ipc_client_connection *con;
    int ret = -1;

    if (!h) {
        NS_LOG(NS_LOG_LVL_FATAL, "passed NULL handle pointer");
        return NS_ERR_INVALID_PARAMETER;
    }

    con = (struct ns_ipc_client_connection *)malloc(sizeof(struct ns_ipc_client_connection));
    if (con == NULL) {
        NS_LOG(NS_LOG_LVL_FATAL, "could not allocate handle data");
        return NS_ERR_OUT_OF_MEMORY;
    }
    h->data = con;

    con->event_handler = NULL;

    connection_init(con);

    /* we try to connect for a while */
    ret = do_connect(con, url, 10000 /* 10s to connect */);
    if (ret < 0) { /* could not connect */
        NS_LOG(NS_LOG_LVL_FATAL, "connection failed");
        h->data = NULL;
        free(con);
        return NS_ERR_FAIL;
    }

    con->running = 1;
    ret = pthread_create(&con->socket_thread, NULL, receive_handler, (void *)con);
    if(ret) {
        NS_LOG(NS_LOG_LVL_FATAL, "receive_handler pthread_create failed %d", ret);
#ifdef _WIN32
        // Clean up
        ns_ipc_wsa_cleanup();
#endif
        free(con);
        return NS_ERR_FAIL;
    }

#ifndef DISABLE_EVENT_QUEUE
    con->eq_level = 0;
    STAILQ_INIT(&con->eq_head);
    pthread_cond_init(&con->eq_not_empty, NULL);
    pthread_mutex_init(&con->eq_lock, NULL);
    ret = pthread_create(&con->event_thread, NULL, event_notifier, (void *)con);
    if (ret) {
        NS_LOG(NS_LOG_LVL_FATAL, "event_notifier pthread_create failed %d", ret);
        return NS_ERR_FAIL;
    }
#endif

    return 0;
}

void ns_ipc_client_close(struct ns_ipc_client_handle *h)
{
    int i;
    struct ns_ipc_client_connection *con = (struct ns_ipc_client_connection *)(h->data);

    con->running = 0;

#ifndef DISABLE_EVENT_QUEUE
    /* we disable the event queue first
     * it is possible that the event handler may be using the ipc handle
     * this avoids race conditions with the event handler and shutting
     * down the ipc handle
     */
    pthread_mutex_lock(&con->eq_lock);
    pthread_cond_signal(&con->eq_not_empty);
    pthread_mutex_unlock(&con->eq_lock);
    pthread_join(con->event_thread, NULL);
#endif

    pthread_join(con->socket_thread, NULL);
    for (i = 0; i < MAX_PENDING_CALLS; i++)
        sem_destroy(&con->cm[i].sem);
    pthread_mutex_destroy(&con->send_mutex);

#ifndef DISABLE_EVENT_QUEUE
    /* drain the event queue - there may be some left overs
     * this is important as the ipc bufs can be very big
     */
    while (!STAILQ_EMPTY(&con->eq_head)) {
        struct eq_entry *entry;
        entry = STAILQ_FIRST(&con->eq_head);
        STAILQ_REMOVE_HEAD(&con->eq_head, next);
        if (entry && entry->buf) {
            ns_ipc_buf_done(entry->buf);
            free(entry->buf);
        }
        if (entry)
            free(entry);
    }

    pthread_cond_destroy(&con->eq_not_empty);
    pthread_mutex_destroy(&con->eq_lock);
#endif

#ifdef _WIN32
    ns_ipc_wsa_cleanup();
#endif
    free(con);
}
