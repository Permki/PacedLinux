/**
* Copyright (C) 2012 Netronome Systems, Inc.  All rights reserved.
*
* @file ns_ipc_buf.h
* IPC Data marshalling API.
*/
#ifndef _NS_IPC_BUF_H_
#define _NS_IPC_BUF_H_

#ifdef __cplusplus
extern "C" {
#endif

// Use a 1GiB maximum. 2GiB was a 32-bit signed integer overflow
// and 1GiB is more than enough.
#define NS_IPC_BUF_MAX (1U*1024*1024*1024)
#define NS_IPC_BUF_INITIAL_SIZE (1500)

enum {
    NS_IPC_NONE = 0,
    NS_IPC_BYTES,
    NS_IPC_STR,
    NS_IPC_INT32,
    NS_IPC_UINT32,
    NS_IPC_INT64,
    NS_IPC_UINT64,
    NS_IPC_IP,
    NS_IPC_IP4,
    NS_IPC_IP6,
    NS_IPC_MAC,
    NS_IPC_BUF,
    NS_IPC_END
};

struct ns_ipc_header {
    uint32_t length;
    uint32_t version;
    uint32_t sequence;
    uint32_t type;
};

enum {
    NS_IPC_MSG_CLIENT_CALL,
    NS_IPC_MSG_SERVER_RESP,
    NS_IPC_MSG_SERVER_EVENT
};

typedef struct {
    size_t alloc_size;
    uint8_t *head;
    uint8_t *tail;
    uint8_t *buf;
    uint32_t error;
} ns_ipc_buf_t;

void ns_ipc_buf_init(ns_ipc_buf_t *b);
void ns_ipc_buf_init2(ns_ipc_buf_t *, ns_ipc_buf_t *);
void ns_ipc_buf_done(ns_ipc_buf_t *b);
void ns_ipc_buf_done2(ns_ipc_buf_t *, ns_ipc_buf_t *);
void ns_ipc_buf_clear(ns_ipc_buf_t *b);
void ns_ipc_buf_grow_size(ns_ipc_buf_t *b, size_t additional_size);
void ns_ipc_buf_pack_data(ns_ipc_buf_t *b, void *data, size_t datalen);
void ns_ipc_buf_pack_tlv(ns_ipc_buf_t *b, uint32_t type, size_t val_len, void *val);
void ns_ipc_buf_pack(ns_ipc_buf_t *b, ...);
void ns_ipc_buf_unpack_data(ns_ipc_buf_t *b, void *data, size_t datalen);
void ns_ipc_buf_unpack_tl(ns_ipc_buf_t *b, uint32_t *type, size_t *val_len);
void ns_ipc_buf_unpack(ns_ipc_buf_t *b, ...);
void ns_ipc_buf_print(ns_ipc_buf_t *b);

#define PK_INT32(_val_)        NS_IPC_INT32,(_val_)
#define PK_UINT32(_val_)       NS_IPC_UINT32,(_val_)
#define PK_INT64(_val_)        NS_IPC_INT64,(_val_)
#define PK_UINT64(_val_)       NS_IPC_UINT64,(_val_)
#define PK_STR(_str_)          NS_IPC_STR,(_str_)
#define PK_BYTES(_val_,_len_)  NS_IPC_BYTES,(_val_),(_len_)
#define PK_ID(_id_)            PK_UINT32(_id_)
#define PK_IP(_ip_)            NS_IPC_IP,&(_ip_)
#define PK_IP4(_ip_)           NS_IPC_IP4,&(_ip_)
#define PK_IP6(_ip_)           NS_IPC_IP6,&(_ip_)
#define PK_MAC(_mac_)          NS_IPC_MAC,(_mac_)
#define PK_IPC_BUF(_ipc_buf_)  NS_IPC_BUF,(_ipc_buf_)
#define PK_END NS_IPC_END

#define UP_INT32(_var_)         NS_IPC_INT32,&(_var_)
#define UP_UINT32(_var_)        NS_IPC_UINT32,&(_var_)
#define UP_INT64(_var_)         NS_IPC_INT64,&(_var_)
#define UP_UINT64(_var_)        NS_IPC_UINT64,&(_var_)
#define UP_STR(_buf_,_len_)     NS_IPC_STR,(&_buf_),(_len_)
#define UP_BYTES(_buf_,_len_)   NS_IPC_BYTES,(&_buf_),&(_len_)
#define UP_ID(_id_)             UP_UINT32(_id_)
#define UP_IP(_ip_)             NS_IPC_IP,&(_ip_)
#define UP_IP4(_ip_)            NS_IPC_IP4,&(_ip_)
#define UP_IP6(_ip_)            NS_IPC_IP6,&(_ip_)
#define UP_MAC(_mac_)           NS_IPC_MAC,&(_mac_)
#define UP_IPC_BUF(_ipc_buf_)   NS_IPC_BUF,&(_ipc_buf_)
#define UP_END NS_IPC_END

#ifdef __cplusplus
};
#endif

#endif /* __NS_IPC_BUF_H__ */
