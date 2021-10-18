/**
* Copyright (C) 2012 Netronome Systems, Inc.  All rights reserved.
*
* @file ns_ipc_buf.c
* IPC Data marshalling implementation.
*/
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

#include <nfp-common/ns_log.h>
#include "ns_ipc_buf.h"
#include "ns_ipc_common.h"

#ifdef _MSC_VER
  #define INLINE __forceinline /* use __forceinline (VC++ specific) */
#else
  #define INLINE inline        /* use standard inline */
#endif

void ns_ipc_buf_grow_size(ns_ipc_buf_t *b, size_t additional_size)
{
    ptrdiff_t bh = b->head - b->buf;
    ptrdiff_t bt = b->tail - b->buf;
    b->alloc_size += additional_size;
    if (b->alloc_size > NS_IPC_BUF_MAX) {
        ++b->error;
		return;
	}
    b->buf = (uint8_t*)realloc(b->buf, b->alloc_size);
    if(b->buf == NULL) {
        NS_LOG(NS_LOG_LVL_ERROR,"Fatal error: out of memory");
        exit(1);
    }
    b->head = b->buf + bh;
    b->tail = b->buf + bt;
    b->error = 0;
}

INLINE void ns_ipc_buf_clear(ns_ipc_buf_t *b)
{
    if (b->buf == NULL) {
        if((b->buf = (uint8_t*)malloc(NS_IPC_BUF_INITIAL_SIZE)) == NULL) {
            NS_LOG(NS_LOG_LVL_ERROR,"Fatal error: out of memory");
            exit(1);
        }
    }
    b->alloc_size = NS_IPC_BUF_INITIAL_SIZE;
    b->head = b->tail = b->buf + sizeof(struct ns_ipc_header);
    b->error = 0;
}

void ns_ipc_buf_init(ns_ipc_buf_t *b) {
    b->buf = NULL;
    ns_ipc_buf_clear(b);
}

void ns_ipc_buf_init2(ns_ipc_buf_t *b1, ns_ipc_buf_t *b2)
{
    b1->buf = b2->buf = NULL;
    ns_ipc_buf_clear(b1);
    ns_ipc_buf_clear(b2);
}

void ns_ipc_buf_done(ns_ipc_buf_t *b) {
    if (b->buf != NULL) {
        free(b->buf);
        b->buf = NULL;
    }
}

void ns_ipc_buf_done2(ns_ipc_buf_t *b1, ns_ipc_buf_t *b2) {
    if (b1->buf != NULL) {
        free(b1->buf);
        b1->buf = NULL;
    }
    if (b2->buf != NULL) {
        free(b2->buf);
        b2->buf = NULL;
    }
}

INLINE void ns_ipc_buf_pack_data(ns_ipc_buf_t *b, void *data, size_t datalen)
{
    size_t e = (b->tail - b->buf) + datalen;
    if (e > b->alloc_size) {
        size_t m = e - b->alloc_size;
        if (m < 1024) {
            m = 1024;
        }
        ns_ipc_buf_grow_size(b, m);
    }
	if (!b->error) {
        memcpy(b->tail, data, datalen);
        b->tail += datalen;
    }
}

void ns_ipc_buf_pack_tlv(ns_ipc_buf_t *b, uint32_t type, size_t val_len, void *val)
{
    // Format: LEN_OF_TYPE_AND_VAL, TYPE, VAL
    uint32_t pack_len = 2 * sizeof(uint32_t) + val_len;
    ns_ipc_buf_pack_data(b, &pack_len, sizeof(pack_len));
    ns_ipc_buf_pack_data(b, &type, sizeof(type));
    ns_ipc_buf_pack_data(b, val, val_len);
}

void ns_ipc_buf_pack(ns_ipc_buf_t *b, ...)
{
    uint32_t len;
    uint32_t type;
    int32_t int32_val;
    uint32_t uint32_val;
    int64_t int64_val;
    uint64_t uint64_val;
    char *str_val;
    uint8_t *bytes_val;
    ns_ip_addr_t *ip_ptr;
    ns_ipc_buf_t *ipc_buf;
    va_list ap;


    va_start(ap, b);

    if (b->buf == NULL) {
        ns_ipc_buf_clear(b);
    }

    for (;;) {
        type = va_arg(ap, uint32_t);
        if (type == NS_IPC_END) break;
        switch (type) {
            case NS_IPC_INT32:
                int32_val = va_arg(ap, int32_t);
                ns_ipc_buf_pack_tlv(b, type, sizeof(int32_val), &int32_val);
                break;
            case NS_IPC_UINT32:
                uint32_val = va_arg(ap, uint32_t);
                ns_ipc_buf_pack_tlv(b, type, sizeof(uint32_val), &uint32_val);
                break;
            case NS_IPC_INT64:
                int64_val = va_arg(ap, int64_t);
                ns_ipc_buf_pack_tlv(b, type, sizeof(int64_val), &int64_val);
                break;
            case NS_IPC_UINT64:
                uint64_val = va_arg(ap, uint64_t);
                ns_ipc_buf_pack_tlv(b, type, sizeof(uint64_val), &uint64_val);
                break;
            case NS_IPC_IP4: // TODO reject IP6
            case NS_IPC_IP6: // TODO reject IP4
            case NS_IPC_IP:
                ip_ptr = va_arg(ap, ns_ip_addr_t *);
                ns_ipc_buf_pack_tlv(b,
                    (ip_ptr->af == AF_INET6) ? NS_IPC_IP6 : NS_IPC_IP4,
                    (ip_ptr->af == AF_INET6) ? sizeof(ip_ptr->ip6) : sizeof(ip_ptr->ip4),
                    ip_ptr->buf);
                break;
            case NS_IPC_STR:
                str_val = va_arg(ap, char *);
                ns_ipc_buf_pack_tlv(b, type, strlen(str_val) + 1, str_val);
                break;
            case NS_IPC_MAC:
                bytes_val = va_arg(ap, uint8_t *);
                ns_ipc_buf_pack_tlv(b, type, 6, bytes_val);
                break;
            case NS_IPC_BYTES:
                bytes_val = va_arg(ap, uint8_t *);
                len = va_arg(ap, size_t);
                if (!bytes_val)
                    len = 0;
                ns_ipc_buf_pack_tlv(b, type, len, bytes_val);
                break;
            case NS_IPC_BUF:
                ipc_buf = va_arg(ap, ns_ipc_buf_t *);
                ns_ipc_buf_pack_tlv(b, type, ipc_buf->tail - ipc_buf->head, ipc_buf->head);
                break;
        }
    }
    va_end(ap);
}

void ns_ipc_buf_unpack_data(ns_ipc_buf_t *b, void *data, size_t datalen)
{
    if (b->head + datalen > b->tail) {
        ++b->error;
    } else if (!b->error) {
        memcpy(data, b->head, datalen);
        b->head += datalen;
    }
}

void ns_ipc_buf_unpack_data_no_cpy(ns_ipc_buf_t *b, void *data, size_t datalen)
{
    if (b->head + datalen > b->tail) {
        ++b->error;
    } else if (!b->error) {
        //memcpy(data, b->head, datalen);
        b->head += datalen;
    }
}

void ns_ipc_buf_unpack_ipc_buf(ns_ipc_buf_t *b, ns_ipc_buf_t *ipc_buf, size_t datalen)
{
    if (b->head + datalen > b->tail) {
        ++b->error;
    } else if (!b->error) {
        memcpy(ipc_buf->head, b->head, datalen);
        ipc_buf->tail = ipc_buf->head + datalen;
        b->head += datalen;
    }
}

void ns_ipc_buf_unpack_tl(ns_ipc_buf_t *b, uint32_t *type, size_t *val_len)
{
    // Format: LEN_OF_TYPE_AND_VAL, TYPE, VAL
    uint32_t tmp;
    tmp = 0;
    ns_ipc_buf_unpack_data(b, &tmp, sizeof(tmp));
    if ((!b->error) && (tmp >= sizeof(uint32_t))) {
        *val_len = tmp - 2 * sizeof(uint32_t);
        ns_ipc_buf_unpack_data(b, &tmp, sizeof(tmp));
        *type = tmp;
    } else {
        *val_len = 0;
        *type = (uint32_t)-1;
    }
}

void ns_ipc_buf_unpack(ns_ipc_buf_t *b, ...)
{
    size_t found_len, *max_len_ptr;
    uint32_t found_type, need_type, len_err = 0;
    int32_t *int32_ptr;
    uint32_t *uint32_ptr;
    int64_t *int64_ptr;
    uint64_t *uint64_ptr;
    char **str_ptr_ptr;
    uint8_t *bytes_ptr;
    uint8_t **bytes_ptr_ptr;
    ns_ip_addr_t *ip_ptr;
    ns_ipc_buf_t *ipc_buf;
    va_list ap;

    if (b->buf == NULL) ++b->error;
    if (b->error) return;

    va_start(ap, b);
    while(!len_err) {
        need_type = va_arg(ap, uint32_t);
        if (need_type == NS_IPC_END) break;
        ns_ipc_buf_unpack_tl(b, &found_type, &found_len);
        if (!((found_type == need_type) ||
              ((found_type == NS_IPC_IP4 || found_type == NS_IPC_IP6) &&
               (need_type == NS_IPC_IP)))) {
            if (found_type != (uint32_t)-1) {
                NS_LOG(NS_LOG_LVL_ERROR,"ipc: protocol mismatch -"
                          " expected type %u, found type %u",
                          need_type, found_type);
            }
            ++b->error;
            return;
        }
        switch (need_type) {
            case NS_IPC_INT32:
                int32_ptr = va_arg(ap, int32_t *);
                if (found_len > sizeof(*int32_ptr)) {
                    len_err = 1;
                } else {
                    ns_ipc_buf_unpack_data(b, int32_ptr, sizeof(*int32_ptr));
                }
                break;
            case NS_IPC_UINT32:
                uint32_ptr = va_arg(ap, uint32_t *);
                if (found_len > sizeof(*uint32_ptr)) {
                    len_err = 1;
                } else {
                    ns_ipc_buf_unpack_data(b, uint32_ptr, sizeof(*uint32_ptr));
                }
                break;
            case NS_IPC_INT64:
                int64_ptr = va_arg(ap, int64_t *);
                if (found_len > sizeof(*int64_ptr)) {
                    len_err = 1;
                } else {
                    ns_ipc_buf_unpack_data(b, int64_ptr, sizeof(*int64_ptr));
                }
                break;
            case NS_IPC_UINT64:
                uint64_ptr = va_arg(ap, uint64_t *);
                if (found_len > sizeof(*uint64_ptr)) {
                    len_err = 1;
                } else {
                    ns_ipc_buf_unpack_data(b, uint64_ptr, sizeof(*uint64_ptr));
                }
                break;
            case NS_IPC_STR:
                str_ptr_ptr = va_arg(ap, char **);
                *str_ptr_ptr = (char*)b->head;
                max_len_ptr = va_arg(ap, size_t*);
                *max_len_ptr = found_len;
                ns_ipc_buf_unpack_data_no_cpy(b, NULL, found_len);
#ifdef COPY_STRINGS
                if (found_len >= max_len) {
                    len_err = 1;
                } else {
                    ns_ipc_buf_unpack_data_no_cpy(b,NULL, found_len);
                    //ns_ipc_buf_unpack_data(b, str_ptr, found_len);
                    str_ptr[found_len]='\0';
                }
#endif
                break;
            case NS_IPC_IP4:
                ip_ptr = va_arg(ap, ns_ip_addr_t *);
                if (found_len != sizeof(ip_ptr->ip4)) {
                    len_err = 1;
                } else {
                    ip_ptr->af = AF_INET;
                    ns_ipc_buf_unpack_data(b, ip_ptr->buf, found_len);
                }
                break;
            case NS_IPC_IP6:
                ip_ptr = va_arg(ap, ns_ip_addr_t *);
                if (found_len != sizeof(ip_ptr->ip6)) {
                    len_err = 1;
                } else {
                    ip_ptr->af = AF_INET6;
                    ns_ipc_buf_unpack_data(b, ip_ptr->buf, found_len);
                }
                break;
            case NS_IPC_IP:
                ip_ptr = va_arg(ap, ns_ip_addr_t *);
                if (found_type == NS_IPC_IP6) {
                    if (found_len != sizeof(ip_ptr->ip6)) {
                        len_err = 1;
                    } else {
                        ip_ptr->af = AF_INET6;
                        ns_ipc_buf_unpack_data(b, ip_ptr->buf, found_len);
                    }
                } else if (found_type == NS_IPC_IP4) {
                    if (found_len != sizeof(ip_ptr->ip4)) {
                        len_err = 1;
                    } else {
                        ip_ptr->af = AF_INET;
                        ns_ipc_buf_unpack_data(b, ip_ptr->buf, found_len);
                    }
                } else {
                    // Should never occur as we ensure type compatibility
                    // before the switch(), but we program defensively.
                    len_err = 1;
                }
                break;
            case NS_IPC_MAC:
                bytes_ptr = va_arg(ap, uint8_t *);
                if (found_len != 6) {
                    len_err = 1;
                } else {
                    ns_ipc_buf_unpack_data(b, bytes_ptr, found_len);
                }
                break;
            case NS_IPC_BYTES:
                bytes_ptr_ptr = va_arg(ap, uint8_t **);
                *bytes_ptr_ptr = (uint8_t*)b->head;
                max_len_ptr = va_arg(ap, size_t*);
                *max_len_ptr = found_len;
                ns_ipc_buf_unpack_data_no_cpy(b, NULL, found_len);
                /* if the buffer size was zero, the pointer is NULL */
                if (found_len == 0)
                    *bytes_ptr_ptr = NULL;
                break;
            case NS_IPC_BUF:
                ipc_buf = va_arg(ap, ns_ipc_buf_t *);
                ns_ipc_buf_clear(ipc_buf);
                ns_ipc_buf_unpack_ipc_buf(b, ipc_buf, found_len);
                break;
        }
    }

    if (len_err) {
        ++b->error;
        return;
    }

    va_end(ap);
}

//
// Print a ns_ipc_buf_t for debug
//
void ns_ipc_buf_print(ns_ipc_buf_t *b) {
    int end;
    uint32_t type;
    size_t val_len;
    uint8_t *head_save = b->head; // Save the head
    // Parse message
    NS_LOG(NS_LOG_LVL_DEBUG, "{");
    for (end = 0; !end;) {
        ns_ipc_buf_unpack_tl(b, &type, &val_len);
        switch (type) {
            //case NS_IPC_NONE:
            case NS_IPC_BYTES:
            case NS_IPC_STR:
            {
                char *string = (char*)malloc(val_len);
                ns_ipc_buf_unpack_data(b, string, val_len);
                NS_LOG(NS_LOG_LVL_DEBUG, "string:%s", string);
                free(string);
            }
            break;

            case NS_IPC_INT32:
            {
                if (val_len == sizeof(int32_t)) {
                    int32_t val;
                    ns_ipc_buf_unpack_data(b, &val, sizeof(int32_t));
                    NS_LOG(NS_LOG_LVL_DEBUG,"int32:%d", val);
                } else {
                    NS_LOG(NS_LOG_LVL_ERROR,"Length does not match type");
                }
            }
            break;
            case NS_IPC_UINT32:
            {
                if (val_len == sizeof(uint32_t)) {
                    uint32_t val;
                    ns_ipc_buf_unpack_data(b, &val, sizeof(uint32_t));
                    NS_LOG(NS_LOG_LVL_DEBUG, "uint32:%u", val);
                } else {
                    NS_LOG(NS_LOG_LVL_ERROR,"Length does not match type");
                }
            }
            break;
            case NS_IPC_INT64:
            {
                if (val_len == sizeof(int64_t)) {
                    int64_t val;
                    ns_ipc_buf_unpack_data(b, &val, sizeof(int64_t));
                    NS_LOG(NS_LOG_LVL_DEBUG, "int64:%ld", val);
                } else {
                    NS_LOG(NS_LOG_LVL_ERROR,"Length does not match type");
                }
            }
            case NS_IPC_UINT64:
            {
                if (val_len == sizeof(uint64_t)) {
                    uint64_t val;
                    ns_ipc_buf_unpack_data(b, &val, sizeof(uint64_t));
                    NS_LOG(NS_LOG_LVL_DEBUG, "uint64:%lu", val);
                } else {
                    NS_LOG(NS_LOG_LVL_ERROR,"Length does not match type");
                }
            }
            case NS_IPC_IP:
            case NS_IPC_IP4:
            case NS_IPC_IP6:
            case NS_IPC_MAC:
                NS_LOG(NS_LOG_LVL_ERROR,"Not implemented");
                break;

            case NS_IPC_BUF:
            {
                ns_ipc_buf_t ipc_buf;
                ns_ipc_buf_init(&ipc_buf);
                ns_ipc_buf_unpack_ipc_buf(b, &ipc_buf, val_len);
                printf("ns_ipc_buf:");
                ns_ipc_buf_print(&ipc_buf);
                break;
            }
            case NS_IPC_END:
                end = 1;
                break;

            case -1:
                end = 1;
                break;

            default:
                NS_LOG(NS_LOG_LVL_ERROR,"Unexpected type");
                end = 1;
        }
    }
    NS_LOG(NS_LOG_LVL_DEBUG,"}");
    b->head = head_save; // Restore the head
}
