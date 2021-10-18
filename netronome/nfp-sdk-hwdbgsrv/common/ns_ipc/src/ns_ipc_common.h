/*
 * Copyright (C) 2014, Netronome Systems, Inc.  All rights reserved.
 */

#ifndef __NS_IPC_COMMON_H__
#define __NS_IPC_COMMON_H__

#ifdef __cplusplus
extern "C" {
#endif

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>

// Need to link with Ws2_32.lib
#pragma comment (lib, "Ws2_32.lib")
#ifndef _WINDOWS_
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#undef WIN32_LEAN_AND_MEAN
#endif
#else
#include <sys/socket.h>
#include <netinet/in.h>
#endif

#pragma pack(1)
typedef struct ns_ip_addr {
    uint32_t af;                  /* AF_INET or AF_INET6 */
    union {
        uint8_t buf[16];
        struct in_addr ip4;
        struct in6_addr ip6;
    };
} ns_ip_addr_t;
#pragma pack()

//#define NS_IPC_DEBUG

#ifdef NS_IPC_DEBUG
#define NS_IPC_TEST_PRINT(...) printf(__VA_ARGS__)
#define NS_IPC_TEST_HEXDUMP(_buf_, _len_) \
    do { \
        int _i_; \
        for (_i_=0; _i_ < (_len_); _i_++) { \
            printf("%02x ", ((uint8_t *)(_buf_))[_i_]); \
            if(_i_ % 16 == 15) printf("\n"); \
        } \
        printf(NS_IPC_EOL); \
    } while (0)
#else
#define NS_IPC_TEST_HEXDUMP(_buf_, _len_)
#endif

#define LISTEN_BACKLOG 16
#define LISTEN_DEFAULT_PORT 23009
#define NS_IPC_VERSION (0x123)

typedef int socket_t;

int ns_ipc_receive(int ns_ipc_fd,
                   ns_ipc_buf_t *b,
                   int initial_timeout,
                   int busy_timeout);

int ns_ipc_send(int ns_ipc_fd, int sequence, int type, ns_ipc_buf_t *b);

#ifdef _WIN32
/* WSAStartup + cleanup management */
int ns_ipc_wsa_startup(void);

void ns_ipc_wsa_cleanup(void);
#endif

#ifdef __cplusplus
};
#endif

#endif /* __IPC_COMMON_H__ */
