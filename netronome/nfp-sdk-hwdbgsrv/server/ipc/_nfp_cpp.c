/* Generated ns_ipc server stubs */
#include <errno.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

#include <nfp-common/nfp_platform.h>
#include <nfp-common/ns_log.h>
#include "ns_ipc_buf.h"
#include "ns_ipc_server.h"
#include "nfp_cpp.h"

#include "nfp_cpp_handles.h"
#include "ipc_support.h"
#ifdef _MSC_VER    /* This block is specific to MS VC++. */
    #include <winerror.h>
    #define EPROTO WSAEPROTOTYPE
#endif


static int local_handle_type = 0;

 /* Handle Declaration
  * Name:    nfp_cpp
  * #:       0
  * Type:    struct nfp_cpp *
  * Count:   1024
  * Release: nfp_cpp_free
  */
#define HANDLE_TYPE_NFP_CPP ((local_handle_type << 16) | 0)

 /* Handle Declaration
  * Name:    cpp_area
  * #:       1
  * Type:    struct nfp_cpp_area *
  * Count:   1024
  * Release: nfp_cpp_area_release_free
  */
#define HANDLE_TYPE_CPP_AREA ((local_handle_type << 16) | 1)

 /* Handle Declaration
  * Name:    nfp_cpp_mutex
  * #:       2
  * Type:    struct nfp_cpp_mutex *
  * Count:   1024
  * Release: nfp_cpp_mutex_free
  */
#define HANDLE_TYPE_NFP_CPP_MUTEX ((local_handle_type << 16) | 2)

 /* Handle Declaration
  * Name:    nfp_cpp_event
  * #:       3
  * Type:    struct nfp_cpp_event *
  * Count:   1024
  * Release: nfp_cpp_event_free
  */
#define HANDLE_TYPE_NFP_CPP_EVENT ((local_handle_type << 16) | 3)

static void __nfp_cpp_from_device_id(ns_ipc_server_conn_handle_t con,
                                     ns_ipc_buf_t *b_out,
                                     ns_ipc_buf_t *b_in)
{
    int err = 0;
    int32_t p_id;
    struct nfp_cpp * retval = NULL;
    uint32_t _cln_api_version = 0, _srv_api_version = 0;

    ns_ipc_buf_unpack(b_in,
                      UP_UINT32(_cln_api_version), /* client api version */
                      UP_INT32(p_id),
                      UP_END);

    ipcclient_trace(con, "nfp_cpp_from_device_id");
    retval = nfp_cpp_from_device_id(p_id);
    err = errno;

    if (!ipcclient_version_check("nfp_cpp.h", 0, 0, _cln_api_version, &_srv_api_version)) {
        if (retval) {
            if (ipcclient_add_handle(con, (void *)retval,
                                     HANDLE_TYPE_NFP_CPP,
                                     _init_nfp_cpp_from_device_id,
                                     _free_nfp_cpp_from_device_id)) {
                retval = NULL;
                err = ENOMEM;
            }
        }
    } else {
        err = EPROTO;
        retval = NULL;
    }

    ns_ipc_buf_pack(b_out,
                    PK_UINT32(_srv_api_version), /* server api version */
                    PK_INT32(err),
                    PK_UINT64(retval),
                    PK_END);

}

static void __nfp_cpp_free(ns_ipc_server_conn_handle_t con,
                           ns_ipc_buf_t *b_out,
                           ns_ipc_buf_t *b_in)
{
    int err = 0;
    struct nfp_cpp * p_cpp;
    uint64_t h_cpp;

    ns_ipc_buf_unpack(b_in,
                      UP_UINT64(h_cpp),
                      UP_END);
    p_cpp = (struct nfp_cpp *) h_cpp;

    if (!ipcclient_check_handle(con, (void *)p_cpp)) {
        NS_LOG(NS_LOG_LVL_WARN, "%s: got bad handle %p", __func__, p_cpp);
        p_cpp = NULL;
    }

    ipcclient_del_handle(con, (void *)p_cpp);
    ns_ipc_buf_pack(b_out,
                    PK_INT32(err),
                    PK_END);

}

static void __nfp_cpp_model(ns_ipc_server_conn_handle_t con,
                            ns_ipc_buf_t *b_out,
                            ns_ipc_buf_t *b_in)
{
    int err = 0;
    struct nfp_cpp * p_cpp;
    uint64_t h_cpp;
    uint32_t retval = 0;

    ns_ipc_buf_unpack(b_in,
                      UP_UINT64(h_cpp),
                      UP_END);
    p_cpp = (struct nfp_cpp *) h_cpp;

    if (!ipcclient_check_handle(con, (void *)p_cpp)) {
        NS_LOG(NS_LOG_LVL_WARN, "%s: got bad handle %p", __func__, p_cpp);
        p_cpp = NULL;
    }

    ipcclient_trace(con, "nfp_cpp_model");
    retval = nfp_cpp_model(p_cpp);
    err = errno;

    ns_ipc_buf_pack(b_out,
                    PK_INT32(err),
                    PK_UINT32(retval),
                    PK_END);

}

static void __nfp_cpp_interface(ns_ipc_server_conn_handle_t con,
                                ns_ipc_buf_t *b_out,
                                ns_ipc_buf_t *b_in)
{
    int err = 0;
    struct nfp_cpp * p_cpp;
    uint64_t h_cpp;
    uint16_t retval = 0;

    ns_ipc_buf_unpack(b_in,
                      UP_UINT64(h_cpp),
                      UP_END);
    p_cpp = (struct nfp_cpp *) h_cpp;

    if (!ipcclient_check_handle(con, (void *)p_cpp)) {
        NS_LOG(NS_LOG_LVL_WARN, "%s: got bad handle %p", __func__, p_cpp);
        p_cpp = NULL;
    }

    ipcclient_trace(con, "nfp_cpp_interface");
    retval = nfp_cpp_interface(p_cpp);
    err = errno;

    ns_ipc_buf_pack(b_out,
                    PK_INT32(err),
                    PK_UINT32(retval),
                    PK_END);

}

static void __nfp_cpp_area_alloc(ns_ipc_server_conn_handle_t con,
                                 ns_ipc_buf_t *b_out,
                                 ns_ipc_buf_t *b_in)
{
    int err = 0;
    struct nfp_cpp * p_cpp;
    uint64_t h_cpp;
    uint32_t p_cpp_id;
    uint64_t p_address;
    uint64_t p_size;
    struct nfp_cpp_area * retval = NULL;
    uint32_t _cln_api_version = 0, _srv_api_version = 0;

    ns_ipc_buf_unpack(b_in,
                      UP_UINT32(_cln_api_version), /* client api version */
                      UP_UINT64(h_cpp),
                      UP_UINT32(p_cpp_id),
                      UP_UINT64(p_address),
                      UP_UINT64(p_size),
                      UP_END);
    p_cpp = (struct nfp_cpp *) h_cpp;

    if (!ipcclient_check_handle(con, (void *)p_cpp)) {
        NS_LOG(NS_LOG_LVL_WARN, "%s: got bad handle %p", __func__, p_cpp);
        p_cpp = NULL;
    }

    ipcclient_trace(con, "nfp_cpp_area_alloc");
    retval = nfp_cpp_area_alloc(p_cpp, p_cpp_id, p_address, p_size);
    err = errno;

    if (!ipcclient_version_check("nfp_cpp.h", 0, 0, _cln_api_version, &_srv_api_version)) {
        if (retval) {
            if (ipcclient_add_handle(con, (void *)retval,
                                     HANDLE_TYPE_CPP_AREA,
                                     _init_nfp_cpp_area_alloc,
                                     _free_nfp_cpp_area_alloc)) {
                retval = NULL;
                err = ENOMEM;
            }
        }
    } else {
        err = EPROTO;
        retval = NULL;
    }

    ns_ipc_buf_pack(b_out,
                    PK_UINT32(_srv_api_version), /* server api version */
                    PK_INT32(err),
                    PK_UINT64(retval),
                    PK_END);

}

static void __nfp_cpp_area_alloc_with_name(ns_ipc_server_conn_handle_t con,
                                           ns_ipc_buf_t *b_out,
                                           ns_ipc_buf_t *b_in)
{
    int err = 0;
    struct nfp_cpp * p_cpp;
    uint64_t h_cpp;
    uint32_t p_cpp_id;
    const char * p_name;
    size_t _len_name = 0;
    uint64_t p_address;
    uint64_t p_size;
    struct nfp_cpp_area * retval = NULL;
    uint32_t _cln_api_version = 0, _srv_api_version = 0;

    ns_ipc_buf_unpack(b_in,
                      UP_UINT32(_cln_api_version), /* client api version */
                      UP_UINT64(h_cpp),
                      UP_UINT32(p_cpp_id),
                      UP_STR(p_name, &_len_name),
                      UP_UINT64(p_address),
                      UP_UINT64(p_size),
                      UP_END);
    p_cpp = (struct nfp_cpp *) h_cpp;

    if (!ipcclient_check_handle(con, (void *)p_cpp)) {
        NS_LOG(NS_LOG_LVL_WARN, "%s: got bad handle %p", __func__, p_cpp);
        p_cpp = NULL;
    }

    ipcclient_trace(con, "nfp_cpp_area_alloc_with_name");
    retval = nfp_cpp_area_alloc_with_name(p_cpp, p_cpp_id, p_name, p_address, p_size);
    err = errno;

    if (!ipcclient_version_check("nfp_cpp.h", 0, 0, _cln_api_version, &_srv_api_version)) {
        if (retval) {
            if (ipcclient_add_handle(con, (void *)retval,
                                     HANDLE_TYPE_CPP_AREA,
                                     _init_nfp_cpp_area_alloc_with_name,
                                     _free_nfp_cpp_area_alloc_with_name)) {
                retval = NULL;
                err = ENOMEM;
            }
        }
    } else {
        err = EPROTO;
        retval = NULL;
    }

    ns_ipc_buf_pack(b_out,
                    PK_UINT32(_srv_api_version), /* server api version */
                    PK_INT32(err),
                    PK_UINT64(retval),
                    PK_END);

}

static void __nfp_cpp_area_free(ns_ipc_server_conn_handle_t con,
                                ns_ipc_buf_t *b_out,
                                ns_ipc_buf_t *b_in)
{
    int err = 0;
    struct nfp_cpp_area * p_area;
    uint64_t h_area;

    ns_ipc_buf_unpack(b_in,
                      UP_UINT64(h_area),
                      UP_END);
    p_area = (struct nfp_cpp_area *) h_area;

    if (!ipcclient_check_handle(con, (void *)p_area)) {
        NS_LOG(NS_LOG_LVL_WARN, "%s: got bad handle %p", __func__, p_area);
        p_area = NULL;
    }

    ipcclient_del_handle(con, (void *)p_area);
    ns_ipc_buf_pack(b_out,
                    PK_INT32(err),
                    PK_END);

}

static void __nfp_cpp_area_acquire(ns_ipc_server_conn_handle_t con,
                                   ns_ipc_buf_t *b_out,
                                   ns_ipc_buf_t *b_in)
{
    int err = 0;
    struct nfp_cpp_area * p_area;
    uint64_t h_area;
    int retval = 0;

    ns_ipc_buf_unpack(b_in,
                      UP_UINT64(h_area),
                      UP_END);
    p_area = (struct nfp_cpp_area *) h_area;

    if (!ipcclient_check_handle(con, (void *)p_area)) {
        NS_LOG(NS_LOG_LVL_WARN, "%s: got bad handle %p", __func__, p_area);
        p_area = NULL;
    }

    ipcclient_trace(con, "nfp_cpp_area_acquire");
    retval = nfp_cpp_area_acquire(p_area);
    err = errno;

    ns_ipc_buf_pack(b_out,
                    PK_INT32(err),
                    PK_INT32(retval),
                    PK_END);

}

static void __nfp_cpp_area_release(ns_ipc_server_conn_handle_t con,
                                   ns_ipc_buf_t *b_out,
                                   ns_ipc_buf_t *b_in)
{
    int err = 0;
    struct nfp_cpp_area * p_area;
    uint64_t h_area;

    ns_ipc_buf_unpack(b_in,
                      UP_UINT64(h_area),
                      UP_END);
    p_area = (struct nfp_cpp_area *) h_area;

    if (!ipcclient_check_handle(con, (void *)p_area)) {
        NS_LOG(NS_LOG_LVL_WARN, "%s: got bad handle %p", __func__, p_area);
        p_area = NULL;
    }

    ipcclient_trace(con, "nfp_cpp_area_release");
    nfp_cpp_area_release(p_area);
    err = errno;

    ns_ipc_buf_pack(b_out,
                    PK_INT32(err),
                    PK_END);

}

static void __nfp_cpp_area_alloc_acquire(ns_ipc_server_conn_handle_t con,
                                         ns_ipc_buf_t *b_out,
                                         ns_ipc_buf_t *b_in)
{
    int err = 0;
    struct nfp_cpp * p_cpp;
    uint64_t h_cpp;
    uint32_t p_cpp_id;
    uint64_t p_address;
    uint64_t p_size;
    struct nfp_cpp_area * retval = NULL;
    uint32_t _cln_api_version = 0, _srv_api_version = 0;

    ns_ipc_buf_unpack(b_in,
                      UP_UINT32(_cln_api_version), /* client api version */
                      UP_UINT64(h_cpp),
                      UP_UINT32(p_cpp_id),
                      UP_UINT64(p_address),
                      UP_UINT64(p_size),
                      UP_END);
    p_cpp = (struct nfp_cpp *) h_cpp;

    if (!ipcclient_check_handle(con, (void *)p_cpp)) {
        NS_LOG(NS_LOG_LVL_WARN, "%s: got bad handle %p", __func__, p_cpp);
        p_cpp = NULL;
    }

    ipcclient_trace(con, "nfp_cpp_area_alloc_acquire");
    retval = nfp_cpp_area_alloc_acquire(p_cpp, p_cpp_id, p_address, p_size);
    err = errno;

    if (!ipcclient_version_check("nfp_cpp.h", 0, 0, _cln_api_version, &_srv_api_version)) {
        if (retval) {
            if (ipcclient_add_handle(con, (void *)retval,
                                     HANDLE_TYPE_CPP_AREA,
                                     _init_nfp_cpp_area_alloc_acquire,
                                     _free_nfp_cpp_area_alloc_acquire)) {
                retval = NULL;
                err = ENOMEM;
            }
        }
    } else {
        err = EPROTO;
        retval = NULL;
    }

    ns_ipc_buf_pack(b_out,
                    PK_UINT32(_srv_api_version), /* server api version */
                    PK_INT32(err),
                    PK_UINT64(retval),
                    PK_END);

}

static void __nfp_cpp_area_release_free(ns_ipc_server_conn_handle_t con,
                                        ns_ipc_buf_t *b_out,
                                        ns_ipc_buf_t *b_in)
{
    int err = 0;
    struct nfp_cpp_area * p_area;
    uint64_t h_area;

    ns_ipc_buf_unpack(b_in,
                      UP_UINT64(h_area),
                      UP_END);
    p_area = (struct nfp_cpp_area *) h_area;

    if (!ipcclient_check_handle(con, (void *)p_area)) {
        NS_LOG(NS_LOG_LVL_WARN, "%s: got bad handle %p", __func__, p_area);
        p_area = NULL;
    }

    ipcclient_del_handle(con, (void *)p_area);
    ns_ipc_buf_pack(b_out,
                    PK_INT32(err),
                    PK_END);

}

static void __nfp_cpp_area_read(ns_ipc_server_conn_handle_t con,
                                ns_ipc_buf_t *b_out,
                                ns_ipc_buf_t *b_in)
{
    int err = 0;
    struct nfp_cpp_area * p_area;
    uint64_t h_area;
    uint64_t p_offset;
    void * p_buffer = NULL;
    int64_t p_length;
    int retval = 0;

    ns_ipc_buf_unpack(b_in,
                      UP_UINT64(h_area),
                      UP_UINT64(p_offset),
                      UP_INT64(p_length),
                      UP_END);
    p_area = (struct nfp_cpp_area *) h_area;

    if (!ipcclient_check_handle(con, (void *)p_area)) {
        NS_LOG(NS_LOG_LVL_WARN, "%s: got bad handle %p", __func__, p_area);
        p_area = NULL;
    }

    p_buffer = malloc(p_length);
    if (!p_buffer)
        err = ENOMEM;

    ipcclient_trace(con, "nfp_cpp_area_read");
    retval = nfp_cpp_area_read(p_area, p_offset, p_buffer, p_length);
    err = errno;

    ns_ipc_buf_pack(b_out,
                    PK_INT32(err),
                    PK_INT32(retval),
                    PK_BYTES(p_buffer, p_length),
                    PK_END);

    if (p_buffer)
        free(p_buffer);
}

static void __nfp_cpp_area_write(ns_ipc_server_conn_handle_t con,
                                 ns_ipc_buf_t *b_out,
                                 ns_ipc_buf_t *b_in)
{
    int err = 0;
    struct nfp_cpp_area * p_area;
    uint64_t h_area;
    uint64_t p_offset;
    const void * p_buffer = NULL;
    int64_t p_length;
    int retval = 0;

    ns_ipc_buf_unpack(b_in,
                      UP_UINT64(h_area),
                      UP_UINT64(p_offset),
                      UP_BYTES(p_buffer, p_length),
                      UP_INT64(p_length),
                      UP_END);
    p_area = (struct nfp_cpp_area *) h_area;

    if (!ipcclient_check_handle(con, (void *)p_area)) {
        NS_LOG(NS_LOG_LVL_WARN, "%s: got bad handle %p", __func__, p_area);
        p_area = NULL;
    }

    ipcclient_trace(con, "nfp_cpp_area_write");
    retval = nfp_cpp_area_write(p_area, p_offset, p_buffer, p_length);
    err = errno;

    ns_ipc_buf_pack(b_out,
                    PK_INT32(err),
                    PK_INT32(retval),
                    PK_END);

}

static void __nfp_cpp_area_check_range(ns_ipc_server_conn_handle_t con,
                                       ns_ipc_buf_t *b_out,
                                       ns_ipc_buf_t *b_in)
{
    int err = 0;
    struct nfp_cpp_area * p_area;
    uint64_t h_area;
    uint64_t p_offset;
    uint64_t p_size;
    int retval = 0;

    ns_ipc_buf_unpack(b_in,
                      UP_UINT64(h_area),
                      UP_UINT64(p_offset),
                      UP_UINT64(p_size),
                      UP_END);
    p_area = (struct nfp_cpp_area *) h_area;

    if (!ipcclient_check_handle(con, (void *)p_area)) {
        NS_LOG(NS_LOG_LVL_WARN, "%s: got bad handle %p", __func__, p_area);
        p_area = NULL;
    }

    ipcclient_trace(con, "nfp_cpp_area_check_range");
    retval = nfp_cpp_area_check_range(p_area, p_offset, p_size);
    err = errno;

    ns_ipc_buf_pack(b_out,
                    PK_INT32(err),
                    PK_INT32(retval),
                    PK_END);

}

static void __nfp_cpp_read(ns_ipc_server_conn_handle_t con,
                           ns_ipc_buf_t *b_out,
                           ns_ipc_buf_t *b_in)
{
    int err = 0;
    struct nfp_cpp * p_cpp;
    uint64_t h_cpp;
    uint32_t p_cpp_id;
    uint64_t p_address;
    void * p_kernel_vaddr = NULL;
    int64_t p_length;
    int retval = 0;

    ns_ipc_buf_unpack(b_in,
                      UP_UINT64(h_cpp),
                      UP_UINT32(p_cpp_id),
                      UP_UINT64(p_address),
                      UP_INT64(p_length),
                      UP_END);
    p_cpp = (struct nfp_cpp *) h_cpp;

    if (!ipcclient_check_handle(con, (void *)p_cpp)) {
        NS_LOG(NS_LOG_LVL_WARN, "%s: got bad handle %p", __func__, p_cpp);
        p_cpp = NULL;
    }

    p_kernel_vaddr = malloc(p_length);
    if (!p_kernel_vaddr)
        err = ENOMEM;

    ipcclient_trace(con, "nfp_cpp_read");
    retval = nfp_cpp_read(p_cpp, p_cpp_id, p_address, p_kernel_vaddr, p_length);
    err = errno;

    ns_ipc_buf_pack(b_out,
                    PK_INT32(err),
                    PK_INT32(retval),
                    PK_BYTES(p_kernel_vaddr, p_length),
                    PK_END);

    if (p_kernel_vaddr)
        free(p_kernel_vaddr);
}

static void __nfp_cpp_write(ns_ipc_server_conn_handle_t con,
                            ns_ipc_buf_t *b_out,
                            ns_ipc_buf_t *b_in)
{
    int err = 0;
    struct nfp_cpp * p_cpp;
    uint64_t h_cpp;
    uint32_t p_cpp_id;
    uint64_t p_address;
    const void * p_kernel_vaddr = NULL;
    int64_t p_length;
    int retval = 0;

    ns_ipc_buf_unpack(b_in,
                      UP_UINT64(h_cpp),
                      UP_UINT32(p_cpp_id),
                      UP_UINT64(p_address),
                      UP_BYTES(p_kernel_vaddr, p_length),
                      UP_INT64(p_length),
                      UP_END);
    p_cpp = (struct nfp_cpp *) h_cpp;

    if (!ipcclient_check_handle(con, (void *)p_cpp)) {
        NS_LOG(NS_LOG_LVL_WARN, "%s: got bad handle %p", __func__, p_cpp);
        p_cpp = NULL;
    }

    ipcclient_trace(con, "nfp_cpp_write");
    retval = nfp_cpp_write(p_cpp, p_cpp_id, p_address, p_kernel_vaddr, p_length);
    err = errno;

    ns_ipc_buf_pack(b_out,
                    PK_INT32(err),
                    PK_INT32(retval),
                    PK_END);

}

static void __nfp_cpp_area_fill(ns_ipc_server_conn_handle_t con,
                                ns_ipc_buf_t *b_out,
                                ns_ipc_buf_t *b_in)
{
    int err = 0;
    struct nfp_cpp_area * p_area;
    uint64_t h_area;
    uint64_t p_offset;
    uint32_t p_value;
    int64_t p_length;
    int retval = 0;

    ns_ipc_buf_unpack(b_in,
                      UP_UINT64(h_area),
                      UP_UINT64(p_offset),
                      UP_UINT32(p_value),
                      UP_INT64(p_length),
                      UP_END);
    p_area = (struct nfp_cpp_area *) h_area;

    if (!ipcclient_check_handle(con, (void *)p_area)) {
        NS_LOG(NS_LOG_LVL_WARN, "%s: got bad handle %p", __func__, p_area);
        p_area = NULL;
    }

    ipcclient_trace(con, "nfp_cpp_area_fill");
    retval = nfp_cpp_area_fill(p_area, p_offset, p_value, p_length);
    err = errno;

    ns_ipc_buf_pack(b_out,
                    PK_INT32(err),
                    PK_INT32(retval),
                    PK_END);

}

static void __nfp_cpp_area_readl(ns_ipc_server_conn_handle_t con,
                                 ns_ipc_buf_t *b_out,
                                 ns_ipc_buf_t *b_in)
{
    int err = 0;
    struct nfp_cpp_area * p_area;
    uint64_t h_area;
    uint64_t p_offset;
    uint32_t _p_value[1];
    uint32_t * p_value = _p_value;
    int retval = 0;

    ns_ipc_buf_unpack(b_in,
                      UP_UINT64(h_area),
                      UP_UINT64(p_offset),
                      UP_END);
    p_area = (struct nfp_cpp_area *) h_area;

    if (!ipcclient_check_handle(con, (void *)p_area)) {
        NS_LOG(NS_LOG_LVL_WARN, "%s: got bad handle %p", __func__, p_area);
        p_area = NULL;
    }

    ipcclient_trace(con, "nfp_cpp_area_readl");
    retval = nfp_cpp_area_readl(p_area, p_offset, p_value);
    err = errno;

    ns_ipc_buf_pack(b_out,
                    PK_INT32(err),
                    PK_INT32(retval),
                    PK_BYTES(p_value, sizeof(_p_value)),
                    PK_END);

}

static void __nfp_cpp_area_writel(ns_ipc_server_conn_handle_t con,
                                  ns_ipc_buf_t *b_out,
                                  ns_ipc_buf_t *b_in)
{
    int err = 0;
    struct nfp_cpp_area * p_area;
    uint64_t h_area;
    uint64_t p_offset;
    uint32_t p_value;
    int retval = 0;

    ns_ipc_buf_unpack(b_in,
                      UP_UINT64(h_area),
                      UP_UINT64(p_offset),
                      UP_UINT32(p_value),
                      UP_END);
    p_area = (struct nfp_cpp_area *) h_area;

    if (!ipcclient_check_handle(con, (void *)p_area)) {
        NS_LOG(NS_LOG_LVL_WARN, "%s: got bad handle %p", __func__, p_area);
        p_area = NULL;
    }

    ipcclient_trace(con, "nfp_cpp_area_writel");
    retval = nfp_cpp_area_writel(p_area, p_offset, p_value);
    err = errno;

    ns_ipc_buf_pack(b_out,
                    PK_INT32(err),
                    PK_INT32(retval),
                    PK_END);

}

static void __nfp_cpp_area_readq(ns_ipc_server_conn_handle_t con,
                                 ns_ipc_buf_t *b_out,
                                 ns_ipc_buf_t *b_in)
{
    int err = 0;
    struct nfp_cpp_area * p_area;
    uint64_t h_area;
    uint64_t p_offset;
    uint64_t _p_value[1];
    uint64_t * p_value = _p_value;
    int retval = 0;

    ns_ipc_buf_unpack(b_in,
                      UP_UINT64(h_area),
                      UP_UINT64(p_offset),
                      UP_END);
    p_area = (struct nfp_cpp_area *) h_area;

    if (!ipcclient_check_handle(con, (void *)p_area)) {
        NS_LOG(NS_LOG_LVL_WARN, "%s: got bad handle %p", __func__, p_area);
        p_area = NULL;
    }

    ipcclient_trace(con, "nfp_cpp_area_readq");
    retval = nfp_cpp_area_readq(p_area, p_offset, p_value);
    err = errno;

    ns_ipc_buf_pack(b_out,
                    PK_INT32(err),
                    PK_INT32(retval),
                    PK_BYTES(p_value, sizeof(_p_value)),
                    PK_END);

}

static void __nfp_cpp_area_writeq(ns_ipc_server_conn_handle_t con,
                                  ns_ipc_buf_t *b_out,
                                  ns_ipc_buf_t *b_in)
{
    int err = 0;
    struct nfp_cpp_area * p_area;
    uint64_t h_area;
    uint64_t p_offset;
    uint64_t p_value;
    int retval = 0;

    ns_ipc_buf_unpack(b_in,
                      UP_UINT64(h_area),
                      UP_UINT64(p_offset),
                      UP_UINT64(p_value),
                      UP_END);
    p_area = (struct nfp_cpp_area *) h_area;

    if (!ipcclient_check_handle(con, (void *)p_area)) {
        NS_LOG(NS_LOG_LVL_WARN, "%s: got bad handle %p", __func__, p_area);
        p_area = NULL;
    }

    ipcclient_trace(con, "nfp_cpp_area_writeq");
    retval = nfp_cpp_area_writeq(p_area, p_offset, p_value);
    err = errno;

    ns_ipc_buf_pack(b_out,
                    PK_INT32(err),
                    PK_INT32(retval),
                    PK_END);

}

static void __nfp_xpb_writel(ns_ipc_server_conn_handle_t con,
                             ns_ipc_buf_t *b_out,
                             ns_ipc_buf_t *b_in)
{
    int err = 0;
    struct nfp_cpp * p_cpp;
    uint64_t h_cpp;
    uint32_t p_xpb_tgt;
    uint32_t p_value;
    int retval = 0;

    ns_ipc_buf_unpack(b_in,
                      UP_UINT64(h_cpp),
                      UP_UINT32(p_xpb_tgt),
                      UP_UINT32(p_value),
                      UP_END);
    p_cpp = (struct nfp_cpp *) h_cpp;

    if (!ipcclient_check_handle(con, (void *)p_cpp)) {
        NS_LOG(NS_LOG_LVL_WARN, "%s: got bad handle %p", __func__, p_cpp);
        p_cpp = NULL;
    }

    ipcclient_trace(con, "nfp_xpb_writel");
    retval = nfp_xpb_writel(p_cpp, p_xpb_tgt, p_value);
    err = errno;

    ns_ipc_buf_pack(b_out,
                    PK_INT32(err),
                    PK_INT32(retval),
                    PK_END);

}

static void __nfp_xpb_readl(ns_ipc_server_conn_handle_t con,
                            ns_ipc_buf_t *b_out,
                            ns_ipc_buf_t *b_in)
{
    int err = 0;
    struct nfp_cpp * p_cpp;
    uint64_t h_cpp;
    uint32_t p_xpb_tgt;
    uint32_t _p_value[1];
    uint32_t * p_value = _p_value;
    int retval = 0;

    ns_ipc_buf_unpack(b_in,
                      UP_UINT64(h_cpp),
                      UP_UINT32(p_xpb_tgt),
                      UP_END);
    p_cpp = (struct nfp_cpp *) h_cpp;

    if (!ipcclient_check_handle(con, (void *)p_cpp)) {
        NS_LOG(NS_LOG_LVL_WARN, "%s: got bad handle %p", __func__, p_cpp);
        p_cpp = NULL;
    }

    ipcclient_trace(con, "nfp_xpb_readl");
    retval = nfp_xpb_readl(p_cpp, p_xpb_tgt, p_value);
    err = errno;

    ns_ipc_buf_pack(b_out,
                    PK_INT32(err),
                    PK_INT32(retval),
                    PK_BYTES(p_value, sizeof(_p_value)),
                    PK_END);

}

static void __nfp_xpb_writelm(ns_ipc_server_conn_handle_t con,
                              ns_ipc_buf_t *b_out,
                              ns_ipc_buf_t *b_in)
{
    int err = 0;
    struct nfp_cpp * p_cpp;
    uint64_t h_cpp;
    uint32_t p_xpb_tgt;
    uint32_t p_mask;
    uint32_t p_value;
    int retval = 0;

    ns_ipc_buf_unpack(b_in,
                      UP_UINT64(h_cpp),
                      UP_UINT32(p_xpb_tgt),
                      UP_UINT32(p_mask),
                      UP_UINT32(p_value),
                      UP_END);
    p_cpp = (struct nfp_cpp *) h_cpp;

    if (!ipcclient_check_handle(con, (void *)p_cpp)) {
        NS_LOG(NS_LOG_LVL_WARN, "%s: got bad handle %p", __func__, p_cpp);
        p_cpp = NULL;
    }

    ipcclient_trace(con, "nfp_xpb_writelm");
    retval = nfp_xpb_writelm(p_cpp, p_xpb_tgt, p_mask, p_value);
    err = errno;

    ns_ipc_buf_pack(b_out,
                    PK_INT32(err),
                    PK_INT32(retval),
                    PK_END);

}

static void __nfp_xpb_waitlm(ns_ipc_server_conn_handle_t con,
                             ns_ipc_buf_t *b_out,
                             ns_ipc_buf_t *b_in)
{
    int err = 0;
    struct nfp_cpp * p_cpp;
    uint64_t h_cpp;
    uint32_t p_xpb_tgt;
    uint32_t p_mask;
    uint32_t p_value;
    int32_t p_timeout_us;
    int retval = 0;

    ns_ipc_buf_unpack(b_in,
                      UP_UINT64(h_cpp),
                      UP_UINT32(p_xpb_tgt),
                      UP_UINT32(p_mask),
                      UP_UINT32(p_value),
                      UP_INT32(p_timeout_us),
                      UP_END);
    p_cpp = (struct nfp_cpp *) h_cpp;

    if (!ipcclient_check_handle(con, (void *)p_cpp)) {
        NS_LOG(NS_LOG_LVL_WARN, "%s: got bad handle %p", __func__, p_cpp);
        p_cpp = NULL;
    }

    ipcclient_trace(con, "nfp_xpb_waitlm");
    retval = nfp_xpb_waitlm(p_cpp, p_xpb_tgt, p_mask, p_value, p_timeout_us);
    err = errno;

    ns_ipc_buf_pack(b_out,
                    PK_INT32(err),
                    PK_INT32(retval),
                    PK_END);

}

static void __nfp_cpp_readl(ns_ipc_server_conn_handle_t con,
                            ns_ipc_buf_t *b_out,
                            ns_ipc_buf_t *b_in)
{
    int err = 0;
    struct nfp_cpp * p_cpp;
    uint64_t h_cpp;
    uint32_t p_cpp_id;
    uint64_t p_address;
    uint32_t _p_value[1];
    uint32_t * p_value = _p_value;
    int retval = 0;

    ns_ipc_buf_unpack(b_in,
                      UP_UINT64(h_cpp),
                      UP_UINT32(p_cpp_id),
                      UP_UINT64(p_address),
                      UP_END);
    p_cpp = (struct nfp_cpp *) h_cpp;

    if (!ipcclient_check_handle(con, (void *)p_cpp)) {
        NS_LOG(NS_LOG_LVL_WARN, "%s: got bad handle %p", __func__, p_cpp);
        p_cpp = NULL;
    }

    ipcclient_trace(con, "nfp_cpp_readl");
    retval = nfp_cpp_readl(p_cpp, p_cpp_id, p_address, p_value);
    err = errno;

    ns_ipc_buf_pack(b_out,
                    PK_INT32(err),
                    PK_INT32(retval),
                    PK_BYTES(p_value, sizeof(_p_value)),
                    PK_END);

}

static void __nfp_cpp_writel(ns_ipc_server_conn_handle_t con,
                             ns_ipc_buf_t *b_out,
                             ns_ipc_buf_t *b_in)
{
    int err = 0;
    struct nfp_cpp * p_cpp;
    uint64_t h_cpp;
    uint32_t p_cpp_id;
    uint64_t p_address;
    uint32_t p_value;
    int retval = 0;

    ns_ipc_buf_unpack(b_in,
                      UP_UINT64(h_cpp),
                      UP_UINT32(p_cpp_id),
                      UP_UINT64(p_address),
                      UP_UINT32(p_value),
                      UP_END);
    p_cpp = (struct nfp_cpp *) h_cpp;

    if (!ipcclient_check_handle(con, (void *)p_cpp)) {
        NS_LOG(NS_LOG_LVL_WARN, "%s: got bad handle %p", __func__, p_cpp);
        p_cpp = NULL;
    }

    ipcclient_trace(con, "nfp_cpp_writel");
    retval = nfp_cpp_writel(p_cpp, p_cpp_id, p_address, p_value);
    err = errno;

    ns_ipc_buf_pack(b_out,
                    PK_INT32(err),
                    PK_INT32(retval),
                    PK_END);

}

static void __nfp_cpp_readq(ns_ipc_server_conn_handle_t con,
                            ns_ipc_buf_t *b_out,
                            ns_ipc_buf_t *b_in)
{
    int err = 0;
    struct nfp_cpp * p_cpp;
    uint64_t h_cpp;
    uint32_t p_cpp_id;
    uint64_t p_address;
    uint64_t _p_value[1];
    uint64_t * p_value = _p_value;
    int retval = 0;

    ns_ipc_buf_unpack(b_in,
                      UP_UINT64(h_cpp),
                      UP_UINT32(p_cpp_id),
                      UP_UINT64(p_address),
                      UP_END);
    p_cpp = (struct nfp_cpp *) h_cpp;

    if (!ipcclient_check_handle(con, (void *)p_cpp)) {
        NS_LOG(NS_LOG_LVL_WARN, "%s: got bad handle %p", __func__, p_cpp);
        p_cpp = NULL;
    }

    ipcclient_trace(con, "nfp_cpp_readq");
    retval = nfp_cpp_readq(p_cpp, p_cpp_id, p_address, p_value);
    err = errno;

    ns_ipc_buf_pack(b_out,
                    PK_INT32(err),
                    PK_INT32(retval),
                    PK_BYTES(p_value, sizeof(_p_value)),
                    PK_END);

}

static void __nfp_cpp_writeq(ns_ipc_server_conn_handle_t con,
                             ns_ipc_buf_t *b_out,
                             ns_ipc_buf_t *b_in)
{
    int err = 0;
    struct nfp_cpp * p_cpp;
    uint64_t h_cpp;
    uint32_t p_cpp_id;
    uint64_t p_address;
    uint64_t p_value;
    int retval = 0;

    ns_ipc_buf_unpack(b_in,
                      UP_UINT64(h_cpp),
                      UP_UINT32(p_cpp_id),
                      UP_UINT64(p_address),
                      UP_UINT64(p_value),
                      UP_END);
    p_cpp = (struct nfp_cpp *) h_cpp;

    if (!ipcclient_check_handle(con, (void *)p_cpp)) {
        NS_LOG(NS_LOG_LVL_WARN, "%s: got bad handle %p", __func__, p_cpp);
        p_cpp = NULL;
    }

    ipcclient_trace(con, "nfp_cpp_writeq");
    retval = nfp_cpp_writeq(p_cpp, p_cpp_id, p_address, p_value);
    err = errno;

    ns_ipc_buf_pack(b_out,
                    PK_INT32(err),
                    PK_INT32(retval),
                    PK_END);

}

static void __nfp_cpp_mutex_init(ns_ipc_server_conn_handle_t con,
                                 ns_ipc_buf_t *b_out,
                                 ns_ipc_buf_t *b_in)
{
    int err = 0;
    struct nfp_cpp * p_cpp;
    uint64_t h_cpp;
    int32_t p_target;
    uint64_t p_address;
    uint32_t p_key_id;
    int retval = 0;

    ns_ipc_buf_unpack(b_in,
                      UP_UINT64(h_cpp),
                      UP_INT32(p_target),
                      UP_UINT64(p_address),
                      UP_UINT32(p_key_id),
                      UP_END);
    p_cpp = (struct nfp_cpp *) h_cpp;

    if (!ipcclient_check_handle(con, (void *)p_cpp)) {
        NS_LOG(NS_LOG_LVL_WARN, "%s: got bad handle %p", __func__, p_cpp);
        p_cpp = NULL;
    }

    ipcclient_trace(con, "nfp_cpp_mutex_init");
    retval = nfp_cpp_mutex_init(p_cpp, p_target, p_address, p_key_id);
    err = errno;

    ns_ipc_buf_pack(b_out,
                    PK_INT32(err),
                    PK_INT32(retval),
                    PK_END);

}

static void __nfp_cpp_mutex_alloc(ns_ipc_server_conn_handle_t con,
                                  ns_ipc_buf_t *b_out,
                                  ns_ipc_buf_t *b_in)
{
    int err = 0;
    struct nfp_cpp * p_cpp;
    uint64_t h_cpp;
    int32_t p_target;
    uint64_t p_address;
    uint32_t p_key_id;
    struct nfp_cpp_mutex * retval = NULL;
    uint32_t _cln_api_version = 0, _srv_api_version = 0;

    ns_ipc_buf_unpack(b_in,
                      UP_UINT32(_cln_api_version), /* client api version */
                      UP_UINT64(h_cpp),
                      UP_INT32(p_target),
                      UP_UINT64(p_address),
                      UP_UINT32(p_key_id),
                      UP_END);
    p_cpp = (struct nfp_cpp *) h_cpp;

    if (!ipcclient_check_handle(con, (void *)p_cpp)) {
        NS_LOG(NS_LOG_LVL_WARN, "%s: got bad handle %p", __func__, p_cpp);
        p_cpp = NULL;
    }

    ipcclient_trace(con, "nfp_cpp_mutex_alloc");
    retval = nfp_cpp_mutex_alloc(p_cpp, p_target, p_address, p_key_id);
    err = errno;

    if (!ipcclient_version_check("nfp_cpp.h", 0, 0, _cln_api_version, &_srv_api_version)) {
        if (retval) {
            if (ipcclient_add_handle(con, (void *)retval,
                                     HANDLE_TYPE_NFP_CPP_MUTEX,
                                     _init_nfp_cpp_mutex_alloc,
                                     _free_nfp_cpp_mutex_alloc)) {
                retval = NULL;
                err = ENOMEM;
            }
        }
    } else {
        err = EPROTO;
        retval = NULL;
    }

    ns_ipc_buf_pack(b_out,
                    PK_UINT32(_srv_api_version), /* server api version */
                    PK_INT32(err),
                    PK_UINT64(retval),
                    PK_END);

}

static void __nfp_cpp_mutex_free(ns_ipc_server_conn_handle_t con,
                                 ns_ipc_buf_t *b_out,
                                 ns_ipc_buf_t *b_in)
{
    int err = 0;
    struct nfp_cpp_mutex * p_mutex;
    uint64_t h_mutex;

    ns_ipc_buf_unpack(b_in,
                      UP_UINT64(h_mutex),
                      UP_END);
    p_mutex = (struct nfp_cpp_mutex *) h_mutex;

    if (!ipcclient_check_handle(con, (void *)p_mutex)) {
        NS_LOG(NS_LOG_LVL_WARN, "%s: got bad handle %p", __func__, p_mutex);
        p_mutex = NULL;
    }

    ipcclient_del_handle(con, (void *)p_mutex);
    ns_ipc_buf_pack(b_out,
                    PK_INT32(err),
                    PK_END);

}

static void __nfp_cpp_mutex_lock(ns_ipc_server_conn_handle_t con,
                                 ns_ipc_buf_t *b_out,
                                 ns_ipc_buf_t *b_in)
{
    int err = 0;
    struct nfp_cpp_mutex * p_mutex;
    uint64_t h_mutex;
    int retval = 0;

    ns_ipc_buf_unpack(b_in,
                      UP_UINT64(h_mutex),
                      UP_END);
    p_mutex = (struct nfp_cpp_mutex *) h_mutex;

    if (!ipcclient_check_handle(con, (void *)p_mutex)) {
        NS_LOG(NS_LOG_LVL_WARN, "%s: got bad handle %p", __func__, p_mutex);
        p_mutex = NULL;
    }

    ipcclient_trace(con, "nfp_cpp_mutex_lock");
    retval = nfp_cpp_mutex_lock(p_mutex);
    err = errno;

    ns_ipc_buf_pack(b_out,
                    PK_INT32(err),
                    PK_INT32(retval),
                    PK_END);

}

static void __nfp_cpp_mutex_unlock(ns_ipc_server_conn_handle_t con,
                                   ns_ipc_buf_t *b_out,
                                   ns_ipc_buf_t *b_in)
{
    int err = 0;
    struct nfp_cpp_mutex * p_mutex;
    uint64_t h_mutex;
    int retval = 0;

    ns_ipc_buf_unpack(b_in,
                      UP_UINT64(h_mutex),
                      UP_END);
    p_mutex = (struct nfp_cpp_mutex *) h_mutex;

    if (!ipcclient_check_handle(con, (void *)p_mutex)) {
        NS_LOG(NS_LOG_LVL_WARN, "%s: got bad handle %p", __func__, p_mutex);
        p_mutex = NULL;
    }

    ipcclient_trace(con, "nfp_cpp_mutex_unlock");
    retval = nfp_cpp_mutex_unlock(p_mutex);
    err = errno;

    ns_ipc_buf_pack(b_out,
                    PK_INT32(err),
                    PK_INT32(retval),
                    PK_END);

}

static void __nfp_cpp_mutex_trylock(ns_ipc_server_conn_handle_t con,
                                    ns_ipc_buf_t *b_out,
                                    ns_ipc_buf_t *b_in)
{
    int err = 0;
    struct nfp_cpp_mutex * p_mutex;
    uint64_t h_mutex;
    int retval = 0;

    ns_ipc_buf_unpack(b_in,
                      UP_UINT64(h_mutex),
                      UP_END);
    p_mutex = (struct nfp_cpp_mutex *) h_mutex;

    if (!ipcclient_check_handle(con, (void *)p_mutex)) {
        NS_LOG(NS_LOG_LVL_WARN, "%s: got bad handle %p", __func__, p_mutex);
        p_mutex = NULL;
    }

    ipcclient_trace(con, "nfp_cpp_mutex_trylock");
    retval = nfp_cpp_mutex_trylock(p_mutex);
    err = errno;

    ns_ipc_buf_pack(b_out,
                    PK_INT32(err),
                    PK_INT32(retval),
                    PK_END);

}

static void __nfp_cpp_event_alloc(ns_ipc_server_conn_handle_t con,
                                  ns_ipc_buf_t *b_out,
                                  ns_ipc_buf_t *b_in)
{
    int err = 0;
    struct nfp_cpp * p_cpp;
    uint64_t h_cpp;
    uint32_t p_event_match;
    uint32_t p_event_mask;
    int32_t p_type;
    struct nfp_cpp_event * retval = NULL;
    uint32_t _cln_api_version = 0, _srv_api_version = 0;

    ns_ipc_buf_unpack(b_in,
                      UP_UINT32(_cln_api_version), /* client api version */
                      UP_UINT64(h_cpp),
                      UP_UINT32(p_event_match),
                      UP_UINT32(p_event_mask),
                      UP_INT32(p_type),
                      UP_END);
    p_cpp = (struct nfp_cpp *) h_cpp;

    if (!ipcclient_check_handle(con, (void *)p_cpp)) {
        NS_LOG(NS_LOG_LVL_WARN, "%s: got bad handle %p", __func__, p_cpp);
        p_cpp = NULL;
    }

    ipcclient_trace(con, "nfp_cpp_event_alloc");
    retval = nfp_cpp_event_alloc(p_cpp, p_event_match, p_event_mask, p_type);
    err = errno;

    if (!ipcclient_version_check("nfp_cpp.h", 0, 0, _cln_api_version, &_srv_api_version)) {
        if (retval) {
            if (ipcclient_add_handle(con, (void *)retval,
                                     HANDLE_TYPE_NFP_CPP_EVENT,
                                     _init_nfp_cpp_event_alloc,
                                     _free_nfp_cpp_event_alloc)) {
                retval = NULL;
                err = ENOMEM;
            }
        }
    } else {
        err = EPROTO;
        retval = NULL;
    }

    ns_ipc_buf_pack(b_out,
                    PK_UINT32(_srv_api_version), /* server api version */
                    PK_INT32(err),
                    PK_UINT64(retval),
                    PK_END);

}

static void __nfp_cpp_event_free(ns_ipc_server_conn_handle_t con,
                                 ns_ipc_buf_t *b_out,
                                 ns_ipc_buf_t *b_in)
{
    int err = 0;
    struct nfp_cpp_event * p_event;
    uint64_t h_event;

    ns_ipc_buf_unpack(b_in,
                      UP_UINT64(h_event),
                      UP_END);
    p_event = (struct nfp_cpp_event *) h_event;

    if (!ipcclient_check_handle(con, (void *)p_event)) {
        NS_LOG(NS_LOG_LVL_WARN, "%s: got bad handle %p", __func__, p_event);
        p_event = NULL;
    }

    ipcclient_del_handle(con, (void *)p_event);
    ns_ipc_buf_pack(b_out,
                    PK_INT32(err),
                    PK_END);

}

static void __nfp_cpp_rpc_version(ns_ipc_server_conn_handle_t con,
                              ns_ipc_buf_t *b_out,
                              ns_ipc_buf_t *b_in)
{
    ns_ipc_buf_pack(b_out,
                    PK_UINT32(0),
                    PK_UINT32(0),
                    PK_END);
}
struct ipc_server_fmap ipc_nfp_cpp_fmaps[] = {
        {"nfp_cpp_from_device_id", __nfp_cpp_from_device_id},
        {"nfp_cpp_free", __nfp_cpp_free},
        {"nfp_cpp_model", __nfp_cpp_model},
        {"nfp_cpp_interface", __nfp_cpp_interface},
        {"nfp_cpp_area_alloc", __nfp_cpp_area_alloc},
        {"nfp_cpp_area_alloc_with_name", __nfp_cpp_area_alloc_with_name},
        {"nfp_cpp_area_free", __nfp_cpp_area_free},
        {"nfp_cpp_area_acquire", __nfp_cpp_area_acquire},
        {"nfp_cpp_area_release", __nfp_cpp_area_release},
        {"nfp_cpp_area_alloc_acquire", __nfp_cpp_area_alloc_acquire},
        {"nfp_cpp_area_release_free", __nfp_cpp_area_release_free},
        {"nfp_cpp_area_read", __nfp_cpp_area_read},
        {"nfp_cpp_area_write", __nfp_cpp_area_write},
        {"nfp_cpp_area_check_range", __nfp_cpp_area_check_range},
        {"nfp_cpp_read", __nfp_cpp_read},
        {"nfp_cpp_write", __nfp_cpp_write},
        {"nfp_cpp_area_fill", __nfp_cpp_area_fill},
        {"nfp_cpp_area_readl", __nfp_cpp_area_readl},
        {"nfp_cpp_area_writel", __nfp_cpp_area_writel},
        {"nfp_cpp_area_readq", __nfp_cpp_area_readq},
        {"nfp_cpp_area_writeq", __nfp_cpp_area_writeq},
        {"nfp_xpb_writel", __nfp_xpb_writel},
        {"nfp_xpb_readl", __nfp_xpb_readl},
        {"nfp_xpb_writelm", __nfp_xpb_writelm},
        {"nfp_xpb_waitlm", __nfp_xpb_waitlm},
        {"nfp_cpp_readl", __nfp_cpp_readl},
        {"nfp_cpp_writel", __nfp_cpp_writel},
        {"nfp_cpp_readq", __nfp_cpp_readq},
        {"nfp_cpp_writeq", __nfp_cpp_writeq},
        {"nfp_cpp_mutex_init", __nfp_cpp_mutex_init},
        {"nfp_cpp_mutex_alloc", __nfp_cpp_mutex_alloc},
        {"nfp_cpp_mutex_free", __nfp_cpp_mutex_free},
        {"nfp_cpp_mutex_lock", __nfp_cpp_mutex_lock},
        {"nfp_cpp_mutex_unlock", __nfp_cpp_mutex_unlock},
        {"nfp_cpp_mutex_trylock", __nfp_cpp_mutex_trylock},
        {"nfp_cpp_event_alloc", __nfp_cpp_event_alloc},
        {"nfp_cpp_event_free", __nfp_cpp_event_free},
        {"nfp_cpp_rpc_version", __nfp_cpp_rpc_version},
        {NULL, NULL}};
