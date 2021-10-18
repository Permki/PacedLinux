/**
* Copyright (C) 2012 Netronome Systems, Inc.  All rights reserved.
*
* @file ns_ipc_common.c
* Implementation of IPC common functions.
*/

#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stddef.h>

#ifdef _WIN32
#include <time.h>
#else
#include <string.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/time.h>
#endif
#include <sys/types.h>

#include <pthread.h>

#include <nfp-common/nfp_platform.h>
#include <nfp-common/ns_log.h>
#include "ns_ipc_buf.h"
#include "ns_ipc_common.h"


#ifdef _WIN32
static pthread_mutex_t wsa_mtx = PTHREAD_MUTEX_INITIALIZER;

/* count the number of active connections for wsa management */
static unsigned int wsa_active = 0;

int ns_ipc_wsa_startup(void)
{
    int ret;
    WSADATA wsaData;

    pthread_mutex_lock(&wsa_mtx);
    if (!wsa_active) {
        ret = WSAStartup(0x202, &wsaData);
        if (ret != 0) {
            NS_LOG(NS_LOG_LVL_ERROR,
                   "WSAStartup() failed with error %d\n", ret);
            pthread_mutex_unlock(&wsa_mtx);
            return -1;
        }
    }
    wsa_active++;
    pthread_mutex_unlock(&wsa_mtx);
    return 0;
}

void ns_ipc_wsa_cleanup(void)
{
    pthread_mutex_lock(&wsa_mtx);
    if (wsa_active == 0) {
            NS_LOG(NS_LOG_LVL_WARN,
                   "WSA cleanup initiates without a startup active");
        pthread_mutex_unlock(&wsa_mtx);
        return;
    }
    if (wsa_active == 1) /* only clean up when one socket is active */
        WSACleanup();
    wsa_active--;
    pthread_mutex_unlock(&wsa_mtx);
}
#endif

static int select_fd(int fd, int timeout_ms)
{
    fd_set rfds;
    struct timeval tv;
    int retval;
    FD_ZERO(&rfds);
    FD_SET(fd, &rfds);
    tv.tv_sec = timeout_ms / 1000;
    tv.tv_usec = (timeout_ms % 1000)*1000;
    retval = select(fd + 1, &rfds, NULL, NULL, &tv);
    return retval;
}

/* XXX: this function is out of hand - it should be really neat and simple
 *      suggest have a windows and a linux function
 */
int ns_ipc_receive(int ns_ipc_fd,
                   ns_ipc_buf_t *b,
                   int initial_timeout,
                   int busy_timeout) {
    struct ns_ipc_header *hdr;
    int got_activity = 0;
    int timeout;
    size_t received_len = 0;
    size_t want_len = sizeof(struct ns_ipc_header);
    int read_len = 0, remain_len;
    int ret = 0;
    uint8_t *p;
#ifdef _WIN32
    u_long iMode;
    int ret_val = 0;
#endif

    ns_ipc_buf_clear(b);
    p = b->buf;
    while((remain_len = want_len - received_len) > 0) {
        if (got_activity)
            timeout = busy_timeout;
        else
            timeout = initial_timeout;

        if (timeout) {
            ret = select_fd(ns_ipc_fd, timeout);
            if (ret == 0) {
#ifdef _WIN32
                errno = WSAEWOULDBLOCK;
#else
                errno = EWOULDBLOCK;
#endif
                return -1;
            } else if (ret == -1) {
#ifdef _WIN32
                errno = WSAGetLastError();
                NS_LOG(NS_LOG_LVL_ERROR, "select failed error: %d", WSAGetLastError());
#else
                NS_LOG(NS_LOG_LVL_ERROR, "select failed error: %s", strerror(errno));
#endif
                return -1;
            }
            if (read_len >= 0)
#ifdef _WIN32
            {
                /* Make socket non-blocking */
                iMode = 1;
                ret_val = ioctlsocket(ns_ipc_fd,FIONBIO,&iMode);
                if (ret_val == SOCKET_ERROR) {
                    errno = WSAGetLastError();
                    NS_LOG(NS_LOG_LVL_DEBUG,"ioctlsocket returns error %d", errno);
                }
                read_len = recv(ns_ipc_fd, (char *)p, remain_len, 0); // non-blocking receive
                if (read_len == SOCKET_ERROR) {
                    errno = WSAGetLastError();
                    if (errno == WSAEWOULDBLOCK) {
                        Sleep(10);
                        goto restart;
                    } else if (errno == WSAECONNRESET) {
                        NS_LOG(NS_LOG_LVL_DEBUG, "client disconnected abruptly");
                        return 0; /* just report client shut down */
                    } else {
                        NS_LOG(NS_LOG_LVL_DEBUG,"recv returns error %d", errno);
                    }
                }
            }
#else
                read_len = recv(ns_ipc_fd, (char*)p, remain_len, MSG_DONTWAIT);
#endif

        } else {
            restart:
#ifdef _WIN32
            iMode = 0;
            /* Make socket blocking */
            ret_val = ioctlsocket(ns_ipc_fd,FIONBIO,&iMode);
            if (ret_val == SOCKET_ERROR) {
                errno = WSAGetLastError();
                NS_LOG(NS_LOG_LVL_DEBUG,"ioctlsocket returns error %d", errno);
            }
            read_len = recv(ns_ipc_fd, (char *)p, remain_len, 0); // blocking receive
            if ((read_len < 0) && ((errno = WSAGetLastError()) == WSAEINTR)) {
#else
            read_len = recv(ns_ipc_fd, p, remain_len, 0);
            if ((read_len < 0) && (errno == EINTR)) {
#endif
                goto restart; // Not all OSes restart system calls
            }
        }

        if (read_len == 0) {
            return 0;
        }
        if (read_len < 0) {
            /* caller should do error checking */
            return -1;
        }
        got_activity = 1;
        NS_IPC_TEST_HEXDUMP(p, read_len);
        received_len += read_len;
        p += read_len;
        if (received_len >= sizeof(struct ns_ipc_header)) {
            want_len = ((struct ns_ipc_header*)(b->buf))->length;
            if ((want_len > NS_IPC_BUF_MAX) || (want_len < sizeof(struct ns_ipc_header))) {
                NS_LOG(NS_LOG_LVL_FATAL,"Receive error: wl %"PRIuSZ, want_len);
                ++b->error;
                return -2;
            }
            if (want_len > b->alloc_size) {
                ptrdiff_t pd = p - b->buf; /* ns_ipc_buf_grow_size() may invalidate p */
                ns_ipc_buf_grow_size(b, want_len - b->alloc_size);
                p = b->buf + pd;
                if (b->error) {
                    return -3;
                }
            }
        }
    }
    b->head = b->buf + sizeof(struct ns_ipc_header);
    b->tail = b->buf + want_len;

    hdr = (struct ns_ipc_header*) b->buf;
    if (hdr->version != NS_IPC_VERSION) {
        NS_LOG(NS_LOG_LVL_ERROR, "IPC protocol version mismatch");
#ifdef _WIN32
        errno = WSAEPROTOTYPE;
#else
        errno = EPROTO;
#endif
        return -1;
    }
    NS_LOG(NS_LOG_LVL_EXTRA,"Received message (length %"PRIuSZ" errors %d)", want_len, b->error);
    NS_IPC_TEST_HEXDUMP(b->buf, want_len);
    return 1;
}

int ns_ipc_send(int ns_ipc_fd, int sequence, int type, ns_ipc_buf_t *b) {
    int sent_len = 0;
    int remain_len, written_len;
    int want_len = b->tail - b->buf;
    uint8_t *p;
    struct ns_ipc_header *hdr = (struct ns_ipc_header*)b->buf;

    hdr->length = want_len;
    hdr->version = NS_IPC_VERSION;
    hdr->sequence = sequence;
    hdr->type = type;
    p = b->buf;

    while ((remain_len = want_len - sent_len) > 0) {
        NS_LOG(NS_LOG_LVL_HEAVY,"Total sent = %d, want to send = %d - remain = %d",
                                            sent_len, want_len, remain_len);
        restart:
#ifdef _WIN32
        written_len = send(ns_ipc_fd, (const char*)p, remain_len, 0);
        if (written_len == SOCKET_ERROR) {
            errno = WSAGetLastError();
            if (errno == WSAEWOULDBLOCK) {
                Sleep(10);
                goto restart;
            } else {
                NS_LOG(NS_LOG_LVL_DEBUG,"Couldn't write to server, errno = %d", errno);
                return -1;
            }
        }
        if ((written_len < 0) && ((errno = WSAGetLastError()) == WSAEINTR)) {
#else
        written_len = write(ns_ipc_fd, p, remain_len);
        if ((written_len < 0) && (errno == EINTR)) {
#endif
            goto restart; // Not all OSes restart system calls
        }
        NS_LOG(NS_LOG_LVL_HEAVY,"Wrote %d bytes, errno = %d", written_len, errno);
        if (written_len < 0) {
            // Caller should do error checking
            return -1;
        }
        NS_IPC_TEST_HEXDUMP(p, written_len);
        sent_len += written_len;
        p += written_len;
    }
    return 0;
}
