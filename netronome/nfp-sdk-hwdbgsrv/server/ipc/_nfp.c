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
#include "nfp.h"

#include "nfp_handles.h"
#include "ipc_support.h"
#ifdef _MSC_VER    /* This block is specific to MS VC++. */
    #include <winerror.h>
    #define EPROTO WSAEPROTOTYPE
#endif


static int local_handle_type = 0;

 /* Handle Declaration
  * Name:    nfp_device
  * #:       0
  * Type:    struct nfp_device *
  * Count:   256
  * Release: nfp_device_close
  */
#define HANDLE_TYPE_NFP_DEVICE ((local_handle_type << 16) | 0)

static void __nfp_device_mask(ns_ipc_server_conn_handle_t con,
                              ns_ipc_buf_t *b_out,
                              ns_ipc_buf_t *b_in)
{
    int err = 0;
    uint64_t retval = 0;

    ns_ipc_buf_unpack(b_in,
                      UP_END);

    ipcclient_trace(con, "nfp_device_mask");
    retval = nfp_device_mask();
    err = errno;

    ns_ipc_buf_pack(b_out,
                    PK_INT32(err),
                    PK_UINT64(retval),
                    PK_END);

}

static void __nfp_num_devices(ns_ipc_server_conn_handle_t con,
                              ns_ipc_buf_t *b_out,
                              ns_ipc_buf_t *b_in)
{
    int err = 0;
    int retval = 0;

    ns_ipc_buf_unpack(b_in,
                      UP_END);

    ipcclient_trace(con, "nfp_num_devices");
    retval = nfp_num_devices();
    err = errno;

    ns_ipc_buf_pack(b_out,
                    PK_INT32(err),
                    PK_INT32(retval),
                    PK_END);

}

static void __nfp_device_open(ns_ipc_server_conn_handle_t con,
                              ns_ipc_buf_t *b_out,
                              ns_ipc_buf_t *b_in)
{
    int err = 0;
    uint32_t p_devnum;
    struct nfp_device * retval = NULL;
    uint32_t _cln_api_version = 0, _srv_api_version = 0;

    ns_ipc_buf_unpack(b_in,
                      UP_UINT32(_cln_api_version), /* client api version */
                      UP_UINT32(p_devnum),
                      UP_END);

    ipcclient_trace(con, "nfp_device_open");
    retval = nfp_device_open(p_devnum);
    err = errno;

    if (!ipcclient_version_check("nfp.h", 1, 1, _cln_api_version, &_srv_api_version)) {
        if (retval) {
            if (ipcclient_add_handle(con, (void *)retval,
                                     HANDLE_TYPE_NFP_DEVICE,
                                     _init_nfp_device_open,
                                     _free_nfp_device_open)) {
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

static void __nfp_device_from_cpp(ns_ipc_server_conn_handle_t con,
                                  ns_ipc_buf_t *b_out,
                                  ns_ipc_buf_t *b_in)
{
    int err = 0;
    struct nfp_cpp * p_cpp;
    uint64_t h_cpp;
    struct nfp_device * retval = NULL;
    uint32_t _cln_api_version = 0, _srv_api_version = 0;

    ns_ipc_buf_unpack(b_in,
                      UP_UINT32(_cln_api_version), /* client api version */
                      UP_UINT64(h_cpp),
                      UP_END);
    p_cpp = (struct nfp_cpp *) h_cpp;

    if (!ipcclient_check_handle(con, (void *)p_cpp)) {
        NS_LOG(NS_LOG_LVL_WARN, "%s: got bad handle %p", __func__, p_cpp);
        p_cpp = NULL;
    }

    ipcclient_trace(con, "nfp_device_from_cpp");
    retval = nfp_device_from_cpp(p_cpp);
    err = errno;

    if (!ipcclient_version_check("nfp.h", 1, 1, _cln_api_version, &_srv_api_version)) {
        if (retval) {
            if (ipcclient_add_handle(con, (void *)retval,
                                     HANDLE_TYPE_NFP_DEVICE,
                                     _init_nfp_device_from_cpp,
                                     _free_nfp_device_from_cpp)) {
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

static void __nfp_device_close(ns_ipc_server_conn_handle_t con,
                               ns_ipc_buf_t *b_out,
                               ns_ipc_buf_t *b_in)
{
    int err = 0;
    struct nfp_device * p_dev;
    uint64_t h_dev;

    ns_ipc_buf_unpack(b_in,
                      UP_UINT64(h_dev),
                      UP_END);
    p_dev = (struct nfp_device *) h_dev;

    if (!ipcclient_check_handle(con, (void *)p_dev)) {
        NS_LOG(NS_LOG_LVL_WARN, "%s: got bad handle %p", __func__, p_dev);
        p_dev = NULL;
    }

    ipcclient_del_handle(con, (void *)p_dev);
    ns_ipc_buf_pack(b_out,
                    PK_INT32(err),
                    PK_END);

}

static void __nfp_sram_read(ns_ipc_server_conn_handle_t con,
                            ns_ipc_buf_t *b_out,
                            ns_ipc_buf_t *b_in)
{
    int err = 0;
    struct nfp_device * p_dev;
    uint64_t h_dev;
    void * p_buffer = NULL;
    uint64_t p_length;
    uint64_t p_offset;
    ssize_t retval = 0;

    ns_ipc_buf_unpack(b_in,
                      UP_UINT64(h_dev),
                      UP_UINT64(p_length),
                      UP_UINT64(p_offset),
                      UP_END);
    p_dev = (struct nfp_device *) h_dev;

    if (!ipcclient_check_handle(con, (void *)p_dev)) {
        NS_LOG(NS_LOG_LVL_WARN, "%s: got bad handle %p", __func__, p_dev);
        p_dev = NULL;
    }

    p_buffer = malloc(p_length);
    if (!p_buffer)
        err = ENOMEM;

    ipcclient_trace(con, "nfp_sram_read");
    retval = nfp_sram_read(p_dev, p_buffer, p_length, p_offset);
    err = errno;

    ns_ipc_buf_pack(b_out,
                    PK_INT32(err),
                    PK_INT64(retval),
                    PK_BYTES(p_buffer, p_length),
                    PK_END);

    if (p_buffer)
        free(p_buffer);
}

static void __nfp_sram_write(ns_ipc_server_conn_handle_t con,
                             ns_ipc_buf_t *b_out,
                             ns_ipc_buf_t *b_in)
{
    int err = 0;
    struct nfp_device * p_dev;
    uint64_t h_dev;
    const void * p_buffer = NULL;
    uint64_t p_length;
    uint64_t p_offset;
    ssize_t retval = 0;

    ns_ipc_buf_unpack(b_in,
                      UP_UINT64(h_dev),
                      UP_BYTES(p_buffer, p_length),
                      UP_UINT64(p_length),
                      UP_UINT64(p_offset),
                      UP_END);
    p_dev = (struct nfp_device *) h_dev;

    if (!ipcclient_check_handle(con, (void *)p_dev)) {
        NS_LOG(NS_LOG_LVL_WARN, "%s: got bad handle %p", __func__, p_dev);
        p_dev = NULL;
    }

    ipcclient_trace(con, "nfp_sram_write");
    retval = nfp_sram_write(p_dev, p_buffer, p_length, p_offset);
    err = errno;

    ns_ipc_buf_pack(b_out,
                    PK_INT32(err),
                    PK_INT64(retval),
                    PK_END);

}

static void __nfp_sram_fill(ns_ipc_server_conn_handle_t con,
                            ns_ipc_buf_t *b_out,
                            ns_ipc_buf_t *b_in)
{
    int err = 0;
    struct nfp_device * p_dev;
    uint64_t h_dev;
    const void * p_buffer = NULL;
    uint64_t p_buf_length;
    uint64_t p_offset;
    uint64_t p_write_length;
    ssize_t retval = 0;

    ns_ipc_buf_unpack(b_in,
                      UP_UINT64(h_dev),
                      UP_BYTES(p_buffer, p_buf_length),
                      UP_UINT64(p_buf_length),
                      UP_UINT64(p_offset),
                      UP_UINT64(p_write_length),
                      UP_END);
    p_dev = (struct nfp_device *) h_dev;

    if (!ipcclient_check_handle(con, (void *)p_dev)) {
        NS_LOG(NS_LOG_LVL_WARN, "%s: got bad handle %p", __func__, p_dev);
        p_dev = NULL;
    }

    ipcclient_trace(con, "nfp_sram_fill");
    retval = nfp_sram_fill(p_dev, p_buffer, p_buf_length, p_offset, p_write_length);
    err = errno;

    ns_ipc_buf_pack(b_out,
                    PK_INT32(err),
                    PK_INT64(retval),
                    PK_END);

}

static void __nfp_emem_read(ns_ipc_server_conn_handle_t con,
                            ns_ipc_buf_t *b_out,
                            ns_ipc_buf_t *b_in)
{
    int err = 0;
    struct nfp_device * p_dev;
    uint64_t h_dev;
    int32_t p_islnum;
    void * p_buffer = NULL;
    uint64_t p_length;
    uint64_t p_offset;
    ssize_t retval = 0;

    ns_ipc_buf_unpack(b_in,
                      UP_UINT64(h_dev),
                      UP_INT32(p_islnum),
                      UP_UINT64(p_length),
                      UP_UINT64(p_offset),
                      UP_END);
    p_dev = (struct nfp_device *) h_dev;

    if (!ipcclient_check_handle(con, (void *)p_dev)) {
        NS_LOG(NS_LOG_LVL_WARN, "%s: got bad handle %p", __func__, p_dev);
        p_dev = NULL;
    }

    p_buffer = malloc(p_length);
    if (!p_buffer)
        err = ENOMEM;

    ipcclient_trace(con, "nfp_emem_read");
    retval = nfp_emem_read(p_dev, p_islnum, p_buffer, p_length, p_offset);
    err = errno;

    ns_ipc_buf_pack(b_out,
                    PK_INT32(err),
                    PK_INT64(retval),
                    PK_BYTES(p_buffer, p_length),
                    PK_END);

    if (p_buffer)
        free(p_buffer);
}

static void __nfp_emem_write(ns_ipc_server_conn_handle_t con,
                             ns_ipc_buf_t *b_out,
                             ns_ipc_buf_t *b_in)
{
    int err = 0;
    struct nfp_device * p_dev;
    uint64_t h_dev;
    int32_t p_islnum;
    const void * p_buffer = NULL;
    uint64_t p_length;
    uint64_t p_offset;
    ssize_t retval = 0;

    ns_ipc_buf_unpack(b_in,
                      UP_UINT64(h_dev),
                      UP_INT32(p_islnum),
                      UP_BYTES(p_buffer, p_length),
                      UP_UINT64(p_length),
                      UP_UINT64(p_offset),
                      UP_END);
    p_dev = (struct nfp_device *) h_dev;

    if (!ipcclient_check_handle(con, (void *)p_dev)) {
        NS_LOG(NS_LOG_LVL_WARN, "%s: got bad handle %p", __func__, p_dev);
        p_dev = NULL;
    }

    ipcclient_trace(con, "nfp_emem_write");
    retval = nfp_emem_write(p_dev, p_islnum, p_buffer, p_length, p_offset);
    err = errno;

    ns_ipc_buf_pack(b_out,
                    PK_INT32(err),
                    PK_INT64(retval),
                    PK_END);

}

static void __nfp_emem_fill(ns_ipc_server_conn_handle_t con,
                            ns_ipc_buf_t *b_out,
                            ns_ipc_buf_t *b_in)
{
    int err = 0;
    struct nfp_device * p_dev;
    uint64_t h_dev;
    int32_t p_islnum;
    const void * p_buffer = NULL;
    uint64_t p_buf_length;
    uint64_t p_offset;
    uint64_t p_write_length;
    ssize_t retval = 0;

    ns_ipc_buf_unpack(b_in,
                      UP_UINT64(h_dev),
                      UP_INT32(p_islnum),
                      UP_BYTES(p_buffer, p_buf_length),
                      UP_UINT64(p_buf_length),
                      UP_UINT64(p_offset),
                      UP_UINT64(p_write_length),
                      UP_END);
    p_dev = (struct nfp_device *) h_dev;

    if (!ipcclient_check_handle(con, (void *)p_dev)) {
        NS_LOG(NS_LOG_LVL_WARN, "%s: got bad handle %p", __func__, p_dev);
        p_dev = NULL;
    }

    ipcclient_trace(con, "nfp_emem_fill");
    retval = nfp_emem_fill(p_dev, p_islnum, p_buffer, p_buf_length, p_offset, p_write_length);
    err = errno;

    ns_ipc_buf_pack(b_out,
                    PK_INT32(err),
                    PK_INT64(retval),
                    PK_END);

}

static void __nfp_emem64_read(ns_ipc_server_conn_handle_t con,
                              ns_ipc_buf_t *b_out,
                              ns_ipc_buf_t *b_in)
{
    int err = 0;
    struct nfp_device * p_dev;
    uint64_t h_dev;
    int32_t p_islnum;
    void * p_buffer = NULL;
    uint64_t p_length;
    uint64_t p_offset;
    ssize_t retval = 0;

    ns_ipc_buf_unpack(b_in,
                      UP_UINT64(h_dev),
                      UP_INT32(p_islnum),
                      UP_UINT64(p_length),
                      UP_UINT64(p_offset),
                      UP_END);
    p_dev = (struct nfp_device *) h_dev;

    if (!ipcclient_check_handle(con, (void *)p_dev)) {
        NS_LOG(NS_LOG_LVL_WARN, "%s: got bad handle %p", __func__, p_dev);
        p_dev = NULL;
    }

    p_buffer = malloc(p_length);
    if (!p_buffer)
        err = ENOMEM;

    ipcclient_trace(con, "nfp_emem64_read");
    retval = nfp_emem64_read(p_dev, p_islnum, p_buffer, p_length, p_offset);
    err = errno;

    ns_ipc_buf_pack(b_out,
                    PK_INT32(err),
                    PK_INT64(retval),
                    PK_BYTES(p_buffer, p_length),
                    PK_END);

    if (p_buffer)
        free(p_buffer);
}

static void __nfp_emem64_write(ns_ipc_server_conn_handle_t con,
                               ns_ipc_buf_t *b_out,
                               ns_ipc_buf_t *b_in)
{
    int err = 0;
    struct nfp_device * p_dev;
    uint64_t h_dev;
    int32_t p_islnum;
    const void * p_buffer = NULL;
    uint64_t p_length;
    uint64_t p_offset;
    ssize_t retval = 0;

    ns_ipc_buf_unpack(b_in,
                      UP_UINT64(h_dev),
                      UP_INT32(p_islnum),
                      UP_BYTES(p_buffer, p_length),
                      UP_UINT64(p_length),
                      UP_UINT64(p_offset),
                      UP_END);
    p_dev = (struct nfp_device *) h_dev;

    if (!ipcclient_check_handle(con, (void *)p_dev)) {
        NS_LOG(NS_LOG_LVL_WARN, "%s: got bad handle %p", __func__, p_dev);
        p_dev = NULL;
    }

    ipcclient_trace(con, "nfp_emem64_write");
    retval = nfp_emem64_write(p_dev, p_islnum, p_buffer, p_length, p_offset);
    err = errno;

    ns_ipc_buf_pack(b_out,
                    PK_INT32(err),
                    PK_INT64(retval),
                    PK_END);

}

static void __nfp_emem64_fill(ns_ipc_server_conn_handle_t con,
                              ns_ipc_buf_t *b_out,
                              ns_ipc_buf_t *b_in)
{
    int err = 0;
    struct nfp_device * p_dev;
    uint64_t h_dev;
    int32_t p_islnum;
    const void * p_buffer = NULL;
    uint64_t p_buf_length;
    uint64_t p_offset;
    uint64_t p_write_length;
    ssize_t retval = 0;

    ns_ipc_buf_unpack(b_in,
                      UP_UINT64(h_dev),
                      UP_INT32(p_islnum),
                      UP_BYTES(p_buffer, p_buf_length),
                      UP_UINT64(p_buf_length),
                      UP_UINT64(p_offset),
                      UP_UINT64(p_write_length),
                      UP_END);
    p_dev = (struct nfp_device *) h_dev;

    if (!ipcclient_check_handle(con, (void *)p_dev)) {
        NS_LOG(NS_LOG_LVL_WARN, "%s: got bad handle %p", __func__, p_dev);
        p_dev = NULL;
    }

    ipcclient_trace(con, "nfp_emem64_fill");
    retval = nfp_emem64_fill(p_dev, p_islnum, p_buffer, p_buf_length, p_offset, p_write_length);
    err = errno;

    ns_ipc_buf_pack(b_out,
                    PK_INT32(err),
                    PK_INT64(retval),
                    PK_END);

}

static void __nfp_imem_read(ns_ipc_server_conn_handle_t con,
                            ns_ipc_buf_t *b_out,
                            ns_ipc_buf_t *b_in)
{
    int err = 0;
    struct nfp_device * p_dev;
    uint64_t h_dev;
    int32_t p_islnum;
    void * p_buffer = NULL;
    uint64_t p_length;
    uint64_t p_offset;
    ssize_t retval = 0;

    ns_ipc_buf_unpack(b_in,
                      UP_UINT64(h_dev),
                      UP_INT32(p_islnum),
                      UP_UINT64(p_length),
                      UP_UINT64(p_offset),
                      UP_END);
    p_dev = (struct nfp_device *) h_dev;

    if (!ipcclient_check_handle(con, (void *)p_dev)) {
        NS_LOG(NS_LOG_LVL_WARN, "%s: got bad handle %p", __func__, p_dev);
        p_dev = NULL;
    }

    p_buffer = malloc(p_length);
    if (!p_buffer)
        err = ENOMEM;

    ipcclient_trace(con, "nfp_imem_read");
    retval = nfp_imem_read(p_dev, p_islnum, p_buffer, p_length, p_offset);
    err = errno;

    ns_ipc_buf_pack(b_out,
                    PK_INT32(err),
                    PK_INT64(retval),
                    PK_BYTES(p_buffer, p_length),
                    PK_END);

    if (p_buffer)
        free(p_buffer);
}

static void __nfp_imem_write(ns_ipc_server_conn_handle_t con,
                             ns_ipc_buf_t *b_out,
                             ns_ipc_buf_t *b_in)
{
    int err = 0;
    struct nfp_device * p_dev;
    uint64_t h_dev;
    int32_t p_islnum;
    const void * p_buffer = NULL;
    uint64_t p_length;
    uint64_t p_offset;
    ssize_t retval = 0;

    ns_ipc_buf_unpack(b_in,
                      UP_UINT64(h_dev),
                      UP_INT32(p_islnum),
                      UP_BYTES(p_buffer, p_length),
                      UP_UINT64(p_length),
                      UP_UINT64(p_offset),
                      UP_END);
    p_dev = (struct nfp_device *) h_dev;

    if (!ipcclient_check_handle(con, (void *)p_dev)) {
        NS_LOG(NS_LOG_LVL_WARN, "%s: got bad handle %p", __func__, p_dev);
        p_dev = NULL;
    }

    ipcclient_trace(con, "nfp_imem_write");
    retval = nfp_imem_write(p_dev, p_islnum, p_buffer, p_length, p_offset);
    err = errno;

    ns_ipc_buf_pack(b_out,
                    PK_INT32(err),
                    PK_INT64(retval),
                    PK_END);

}

static void __nfp_imem_fill(ns_ipc_server_conn_handle_t con,
                            ns_ipc_buf_t *b_out,
                            ns_ipc_buf_t *b_in)
{
    int err = 0;
    struct nfp_device * p_dev;
    uint64_t h_dev;
    int32_t p_islnum;
    const void * p_buffer = NULL;
    uint64_t p_buf_length;
    uint64_t p_offset;
    uint64_t p_write_length;
    ssize_t retval = 0;

    ns_ipc_buf_unpack(b_in,
                      UP_UINT64(h_dev),
                      UP_INT32(p_islnum),
                      UP_BYTES(p_buffer, p_buf_length),
                      UP_UINT64(p_buf_length),
                      UP_UINT64(p_offset),
                      UP_UINT64(p_write_length),
                      UP_END);
    p_dev = (struct nfp_device *) h_dev;

    if (!ipcclient_check_handle(con, (void *)p_dev)) {
        NS_LOG(NS_LOG_LVL_WARN, "%s: got bad handle %p", __func__, p_dev);
        p_dev = NULL;
    }

    ipcclient_trace(con, "nfp_imem_fill");
    retval = nfp_imem_fill(p_dev, p_islnum, p_buffer, p_buf_length, p_offset, p_write_length);
    err = errno;

    ns_ipc_buf_pack(b_out,
                    PK_INT32(err),
                    PK_INT64(retval),
                    PK_END);

}

static void __nfp_imem64_read(ns_ipc_server_conn_handle_t con,
                              ns_ipc_buf_t *b_out,
                              ns_ipc_buf_t *b_in)
{
    int err = 0;
    struct nfp_device * p_dev;
    uint64_t h_dev;
    int32_t p_islnum;
    void * p_buffer = NULL;
    uint64_t p_length;
    uint64_t p_offset;
    ssize_t retval = 0;

    ns_ipc_buf_unpack(b_in,
                      UP_UINT64(h_dev),
                      UP_INT32(p_islnum),
                      UP_UINT64(p_length),
                      UP_UINT64(p_offset),
                      UP_END);
    p_dev = (struct nfp_device *) h_dev;

    if (!ipcclient_check_handle(con, (void *)p_dev)) {
        NS_LOG(NS_LOG_LVL_WARN, "%s: got bad handle %p", __func__, p_dev);
        p_dev = NULL;
    }

    p_buffer = malloc(p_length);
    if (!p_buffer)
        err = ENOMEM;

    ipcclient_trace(con, "nfp_imem64_read");
    retval = nfp_imem64_read(p_dev, p_islnum, p_buffer, p_length, p_offset);
    err = errno;

    ns_ipc_buf_pack(b_out,
                    PK_INT32(err),
                    PK_INT64(retval),
                    PK_BYTES(p_buffer, p_length),
                    PK_END);

    if (p_buffer)
        free(p_buffer);
}

static void __nfp_imem64_write(ns_ipc_server_conn_handle_t con,
                               ns_ipc_buf_t *b_out,
                               ns_ipc_buf_t *b_in)
{
    int err = 0;
    struct nfp_device * p_dev;
    uint64_t h_dev;
    int32_t p_islnum;
    const void * p_buffer = NULL;
    uint64_t p_length;
    uint64_t p_offset;
    ssize_t retval = 0;

    ns_ipc_buf_unpack(b_in,
                      UP_UINT64(h_dev),
                      UP_INT32(p_islnum),
                      UP_BYTES(p_buffer, p_length),
                      UP_UINT64(p_length),
                      UP_UINT64(p_offset),
                      UP_END);
    p_dev = (struct nfp_device *) h_dev;

    if (!ipcclient_check_handle(con, (void *)p_dev)) {
        NS_LOG(NS_LOG_LVL_WARN, "%s: got bad handle %p", __func__, p_dev);
        p_dev = NULL;
    }

    ipcclient_trace(con, "nfp_imem64_write");
    retval = nfp_imem64_write(p_dev, p_islnum, p_buffer, p_length, p_offset);
    err = errno;

    ns_ipc_buf_pack(b_out,
                    PK_INT32(err),
                    PK_INT64(retval),
                    PK_END);

}

static void __nfp_imem64_fill(ns_ipc_server_conn_handle_t con,
                              ns_ipc_buf_t *b_out,
                              ns_ipc_buf_t *b_in)
{
    int err = 0;
    struct nfp_device * p_dev;
    uint64_t h_dev;
    int32_t p_islnum;
    const void * p_buffer = NULL;
    uint64_t p_buf_length;
    uint64_t p_offset;
    uint64_t p_write_length;
    ssize_t retval = 0;

    ns_ipc_buf_unpack(b_in,
                      UP_UINT64(h_dev),
                      UP_INT32(p_islnum),
                      UP_BYTES(p_buffer, p_buf_length),
                      UP_UINT64(p_buf_length),
                      UP_UINT64(p_offset),
                      UP_UINT64(p_write_length),
                      UP_END);
    p_dev = (struct nfp_device *) h_dev;

    if (!ipcclient_check_handle(con, (void *)p_dev)) {
        NS_LOG(NS_LOG_LVL_WARN, "%s: got bad handle %p", __func__, p_dev);
        p_dev = NULL;
    }

    ipcclient_trace(con, "nfp_imem64_fill");
    retval = nfp_imem64_fill(p_dev, p_islnum, p_buffer, p_buf_length, p_offset, p_write_length);
    err = errno;

    ns_ipc_buf_pack(b_out,
                    PK_INT32(err),
                    PK_INT64(retval),
                    PK_END);

}

static void __nfp_ctm_read(ns_ipc_server_conn_handle_t con,
                           ns_ipc_buf_t *b_out,
                           ns_ipc_buf_t *b_in)
{
    int err = 0;
    struct nfp_device * p_dev;
    uint64_t h_dev;
    int32_t p_islnum;
    void * p_buffer = NULL;
    uint64_t p_length;
    uint64_t p_offset;
    ssize_t retval = 0;

    ns_ipc_buf_unpack(b_in,
                      UP_UINT64(h_dev),
                      UP_INT32(p_islnum),
                      UP_UINT64(p_length),
                      UP_UINT64(p_offset),
                      UP_END);
    p_dev = (struct nfp_device *) h_dev;

    if (!ipcclient_check_handle(con, (void *)p_dev)) {
        NS_LOG(NS_LOG_LVL_WARN, "%s: got bad handle %p", __func__, p_dev);
        p_dev = NULL;
    }

    p_buffer = malloc(p_length);
    if (!p_buffer)
        err = ENOMEM;

    ipcclient_trace(con, "nfp_ctm_read");
    retval = nfp_ctm_read(p_dev, p_islnum, p_buffer, p_length, p_offset);
    err = errno;

    ns_ipc_buf_pack(b_out,
                    PK_INT32(err),
                    PK_INT64(retval),
                    PK_BYTES(p_buffer, p_length),
                    PK_END);

    if (p_buffer)
        free(p_buffer);
}

static void __nfp_ctm_write(ns_ipc_server_conn_handle_t con,
                            ns_ipc_buf_t *b_out,
                            ns_ipc_buf_t *b_in)
{
    int err = 0;
    struct nfp_device * p_dev;
    uint64_t h_dev;
    int32_t p_islnum;
    const void * p_buffer = NULL;
    uint64_t p_length;
    uint64_t p_offset;
    ssize_t retval = 0;

    ns_ipc_buf_unpack(b_in,
                      UP_UINT64(h_dev),
                      UP_INT32(p_islnum),
                      UP_BYTES(p_buffer, p_length),
                      UP_UINT64(p_length),
                      UP_UINT64(p_offset),
                      UP_END);
    p_dev = (struct nfp_device *) h_dev;

    if (!ipcclient_check_handle(con, (void *)p_dev)) {
        NS_LOG(NS_LOG_LVL_WARN, "%s: got bad handle %p", __func__, p_dev);
        p_dev = NULL;
    }

    ipcclient_trace(con, "nfp_ctm_write");
    retval = nfp_ctm_write(p_dev, p_islnum, p_buffer, p_length, p_offset);
    err = errno;

    ns_ipc_buf_pack(b_out,
                    PK_INT32(err),
                    PK_INT64(retval),
                    PK_END);

}

static void __nfp_ctm_fill(ns_ipc_server_conn_handle_t con,
                           ns_ipc_buf_t *b_out,
                           ns_ipc_buf_t *b_in)
{
    int err = 0;
    struct nfp_device * p_dev;
    uint64_t h_dev;
    int32_t p_islnum;
    const void * p_buffer = NULL;
    uint64_t p_buf_length;
    uint64_t p_offset;
    uint64_t p_write_length;
    ssize_t retval = 0;

    ns_ipc_buf_unpack(b_in,
                      UP_UINT64(h_dev),
                      UP_INT32(p_islnum),
                      UP_BYTES(p_buffer, p_buf_length),
                      UP_UINT64(p_buf_length),
                      UP_UINT64(p_offset),
                      UP_UINT64(p_write_length),
                      UP_END);
    p_dev = (struct nfp_device *) h_dev;

    if (!ipcclient_check_handle(con, (void *)p_dev)) {
        NS_LOG(NS_LOG_LVL_WARN, "%s: got bad handle %p", __func__, p_dev);
        p_dev = NULL;
    }

    ipcclient_trace(con, "nfp_ctm_fill");
    retval = nfp_ctm_fill(p_dev, p_islnum, p_buffer, p_buf_length, p_offset, p_write_length);
    err = errno;

    ns_ipc_buf_pack(b_out,
                    PK_INT32(err),
                    PK_INT64(retval),
                    PK_END);

}

static void __nfp_ctm64_read(ns_ipc_server_conn_handle_t con,
                             ns_ipc_buf_t *b_out,
                             ns_ipc_buf_t *b_in)
{
    int err = 0;
    struct nfp_device * p_dev;
    uint64_t h_dev;
    int32_t p_islnum;
    void * p_buffer = NULL;
    uint64_t p_length;
    uint64_t p_offset;
    ssize_t retval = 0;

    ns_ipc_buf_unpack(b_in,
                      UP_UINT64(h_dev),
                      UP_INT32(p_islnum),
                      UP_UINT64(p_length),
                      UP_UINT64(p_offset),
                      UP_END);
    p_dev = (struct nfp_device *) h_dev;

    if (!ipcclient_check_handle(con, (void *)p_dev)) {
        NS_LOG(NS_LOG_LVL_WARN, "%s: got bad handle %p", __func__, p_dev);
        p_dev = NULL;
    }

    p_buffer = malloc(p_length);
    if (!p_buffer)
        err = ENOMEM;

    ipcclient_trace(con, "nfp_ctm64_read");
    retval = nfp_ctm64_read(p_dev, p_islnum, p_buffer, p_length, p_offset);
    err = errno;

    ns_ipc_buf_pack(b_out,
                    PK_INT32(err),
                    PK_INT64(retval),
                    PK_BYTES(p_buffer, p_length),
                    PK_END);

    if (p_buffer)
        free(p_buffer);
}

static void __nfp_ctm64_write(ns_ipc_server_conn_handle_t con,
                              ns_ipc_buf_t *b_out,
                              ns_ipc_buf_t *b_in)
{
    int err = 0;
    struct nfp_device * p_dev;
    uint64_t h_dev;
    int32_t p_islnum;
    const void * p_buffer = NULL;
    uint64_t p_length;
    uint64_t p_offset;
    ssize_t retval = 0;

    ns_ipc_buf_unpack(b_in,
                      UP_UINT64(h_dev),
                      UP_INT32(p_islnum),
                      UP_BYTES(p_buffer, p_length),
                      UP_UINT64(p_length),
                      UP_UINT64(p_offset),
                      UP_END);
    p_dev = (struct nfp_device *) h_dev;

    if (!ipcclient_check_handle(con, (void *)p_dev)) {
        NS_LOG(NS_LOG_LVL_WARN, "%s: got bad handle %p", __func__, p_dev);
        p_dev = NULL;
    }

    ipcclient_trace(con, "nfp_ctm64_write");
    retval = nfp_ctm64_write(p_dev, p_islnum, p_buffer, p_length, p_offset);
    err = errno;

    ns_ipc_buf_pack(b_out,
                    PK_INT32(err),
                    PK_INT64(retval),
                    PK_END);

}

static void __nfp_ctm64_fill(ns_ipc_server_conn_handle_t con,
                             ns_ipc_buf_t *b_out,
                             ns_ipc_buf_t *b_in)
{
    int err = 0;
    struct nfp_device * p_dev;
    uint64_t h_dev;
    int32_t p_islnum;
    const void * p_buffer = NULL;
    uint64_t p_buf_length;
    uint64_t p_offset;
    uint64_t p_write_length;
    ssize_t retval = 0;

    ns_ipc_buf_unpack(b_in,
                      UP_UINT64(h_dev),
                      UP_INT32(p_islnum),
                      UP_BYTES(p_buffer, p_buf_length),
                      UP_UINT64(p_buf_length),
                      UP_UINT64(p_offset),
                      UP_UINT64(p_write_length),
                      UP_END);
    p_dev = (struct nfp_device *) h_dev;

    if (!ipcclient_check_handle(con, (void *)p_dev)) {
        NS_LOG(NS_LOG_LVL_WARN, "%s: got bad handle %p", __func__, p_dev);
        p_dev = NULL;
    }

    ipcclient_trace(con, "nfp_ctm64_fill");
    retval = nfp_ctm64_fill(p_dev, p_islnum, p_buffer, p_buf_length, p_offset, p_write_length);
    err = errno;

    ns_ipc_buf_pack(b_out,
                    PK_INT32(err),
                    PK_INT64(retval),
                    PK_END);

}

static void __nfp_cls_read(ns_ipc_server_conn_handle_t con,
                           ns_ipc_buf_t *b_out,
                           ns_ipc_buf_t *b_in)
{
    int err = 0;
    struct nfp_device * p_dev;
    uint64_t h_dev;
    int32_t p_clnum;
    void * p_buffer = NULL;
    uint64_t p_length;
    uint64_t p_offset;
    ssize_t retval = 0;

    ns_ipc_buf_unpack(b_in,
                      UP_UINT64(h_dev),
                      UP_INT32(p_clnum),
                      UP_UINT64(p_length),
                      UP_UINT64(p_offset),
                      UP_END);
    p_dev = (struct nfp_device *) h_dev;

    if (!ipcclient_check_handle(con, (void *)p_dev)) {
        NS_LOG(NS_LOG_LVL_WARN, "%s: got bad handle %p", __func__, p_dev);
        p_dev = NULL;
    }

    p_buffer = malloc(p_length);
    if (!p_buffer)
        err = ENOMEM;

    ipcclient_trace(con, "nfp_cls_read");
    retval = nfp_cls_read(p_dev, p_clnum, p_buffer, p_length, p_offset);
    err = errno;

    ns_ipc_buf_pack(b_out,
                    PK_INT32(err),
                    PK_INT64(retval),
                    PK_BYTES(p_buffer, p_length),
                    PK_END);

    if (p_buffer)
        free(p_buffer);
}

static void __nfp_cls_write(ns_ipc_server_conn_handle_t con,
                            ns_ipc_buf_t *b_out,
                            ns_ipc_buf_t *b_in)
{
    int err = 0;
    struct nfp_device * p_dev;
    uint64_t h_dev;
    int32_t p_clnum;
    const void * p_buffer = NULL;
    uint64_t p_length;
    uint64_t p_offset;
    ssize_t retval = 0;

    ns_ipc_buf_unpack(b_in,
                      UP_UINT64(h_dev),
                      UP_INT32(p_clnum),
                      UP_BYTES(p_buffer, p_length),
                      UP_UINT64(p_length),
                      UP_UINT64(p_offset),
                      UP_END);
    p_dev = (struct nfp_device *) h_dev;

    if (!ipcclient_check_handle(con, (void *)p_dev)) {
        NS_LOG(NS_LOG_LVL_WARN, "%s: got bad handle %p", __func__, p_dev);
        p_dev = NULL;
    }

    ipcclient_trace(con, "nfp_cls_write");
    retval = nfp_cls_write(p_dev, p_clnum, p_buffer, p_length, p_offset);
    err = errno;

    ns_ipc_buf_pack(b_out,
                    PK_INT32(err),
                    PK_INT64(retval),
                    PK_END);

}

static void __nfp_cls_fill(ns_ipc_server_conn_handle_t con,
                           ns_ipc_buf_t *b_out,
                           ns_ipc_buf_t *b_in)
{
    int err = 0;
    struct nfp_device * p_dev;
    uint64_t h_dev;
    int32_t p_clnum;
    const void * p_buffer = NULL;
    uint64_t p_buf_length;
    uint64_t p_offset;
    uint64_t p_write_length;
    ssize_t retval = 0;

    ns_ipc_buf_unpack(b_in,
                      UP_UINT64(h_dev),
                      UP_INT32(p_clnum),
                      UP_BYTES(p_buffer, p_buf_length),
                      UP_UINT64(p_buf_length),
                      UP_UINT64(p_offset),
                      UP_UINT64(p_write_length),
                      UP_END);
    p_dev = (struct nfp_device *) h_dev;

    if (!ipcclient_check_handle(con, (void *)p_dev)) {
        NS_LOG(NS_LOG_LVL_WARN, "%s: got bad handle %p", __func__, p_dev);
        p_dev = NULL;
    }

    ipcclient_trace(con, "nfp_cls_fill");
    retval = nfp_cls_fill(p_dev, p_clnum, p_buffer, p_buf_length, p_offset, p_write_length);
    err = errno;

    ns_ipc_buf_pack(b_out,
                    PK_INT32(err),
                    PK_INT64(retval),
                    PK_END);

}

static void __nfp_lmem_read(ns_ipc_server_conn_handle_t con,
                            ns_ipc_buf_t *b_out,
                            ns_ipc_buf_t *b_in)
{
    int err = 0;
    struct nfp_device * p_dev;
    uint64_t h_dev;
    int32_t p_meid;
    void * p_buffer = NULL;
    uint64_t p_length;
    uint64_t p_offset;
    ssize_t retval = 0;

    ns_ipc_buf_unpack(b_in,
                      UP_UINT64(h_dev),
                      UP_INT32(p_meid),
                      UP_UINT64(p_length),
                      UP_UINT64(p_offset),
                      UP_END);
    p_dev = (struct nfp_device *) h_dev;

    if (!ipcclient_check_handle(con, (void *)p_dev)) {
        NS_LOG(NS_LOG_LVL_WARN, "%s: got bad handle %p", __func__, p_dev);
        p_dev = NULL;
    }

    p_buffer = malloc(p_length);
    if (!p_buffer)
        err = ENOMEM;

    ipcclient_trace(con, "nfp_lmem_read");
    retval = nfp_lmem_read(p_dev, p_meid, p_buffer, p_length, p_offset);
    err = errno;

    ns_ipc_buf_pack(b_out,
                    PK_INT32(err),
                    PK_INT64(retval),
                    PK_BYTES(p_buffer, p_length),
                    PK_END);

    if (p_buffer)
        free(p_buffer);
}

static void __nfp_lmem_write(ns_ipc_server_conn_handle_t con,
                             ns_ipc_buf_t *b_out,
                             ns_ipc_buf_t *b_in)
{
    int err = 0;
    struct nfp_device * p_dev;
    uint64_t h_dev;
    int32_t p_meid;
    const void * p_buffer = NULL;
    uint64_t p_length;
    uint64_t p_offset;
    ssize_t retval = 0;

    ns_ipc_buf_unpack(b_in,
                      UP_UINT64(h_dev),
                      UP_INT32(p_meid),
                      UP_BYTES(p_buffer, p_length),
                      UP_UINT64(p_length),
                      UP_UINT64(p_offset),
                      UP_END);
    p_dev = (struct nfp_device *) h_dev;

    if (!ipcclient_check_handle(con, (void *)p_dev)) {
        NS_LOG(NS_LOG_LVL_WARN, "%s: got bad handle %p", __func__, p_dev);
        p_dev = NULL;
    }

    ipcclient_trace(con, "nfp_lmem_write");
    retval = nfp_lmem_write(p_dev, p_meid, p_buffer, p_length, p_offset);
    err = errno;

    ns_ipc_buf_pack(b_out,
                    PK_INT32(err),
                    PK_INT64(retval),
                    PK_END);

}

static void __nfp_lmem_fill(ns_ipc_server_conn_handle_t con,
                            ns_ipc_buf_t *b_out,
                            ns_ipc_buf_t *b_in)
{
    int err = 0;
    struct nfp_device * p_dev;
    uint64_t h_dev;
    int32_t p_meid;
    const void * p_buffer = NULL;
    uint64_t p_buf_length;
    uint64_t p_offset;
    uint64_t p_write_length;
    ssize_t retval = 0;

    ns_ipc_buf_unpack(b_in,
                      UP_UINT64(h_dev),
                      UP_INT32(p_meid),
                      UP_BYTES(p_buffer, p_buf_length),
                      UP_UINT64(p_buf_length),
                      UP_UINT64(p_offset),
                      UP_UINT64(p_write_length),
                      UP_END);
    p_dev = (struct nfp_device *) h_dev;

    if (!ipcclient_check_handle(con, (void *)p_dev)) {
        NS_LOG(NS_LOG_LVL_WARN, "%s: got bad handle %p", __func__, p_dev);
        p_dev = NULL;
    }

    ipcclient_trace(con, "nfp_lmem_fill");
    retval = nfp_lmem_fill(p_dev, p_meid, p_buffer, p_buf_length, p_offset, p_write_length);
    err = errno;

    ns_ipc_buf_pack(b_out,
                    PK_INT32(err),
                    PK_INT64(retval),
                    PK_END);

}

static void __nfp_ustore_read(ns_ipc_server_conn_handle_t con,
                              ns_ipc_buf_t *b_out,
                              ns_ipc_buf_t *b_in)
{
    int err = 0;
    struct nfp_device * p_dev;
    uint64_t h_dev;
    int32_t p_meid;
    void * p_buffer = NULL;
    uint64_t p_length;
    uint64_t p_offset;
    ssize_t retval = 0;

    ns_ipc_buf_unpack(b_in,
                      UP_UINT64(h_dev),
                      UP_INT32(p_meid),
                      UP_UINT64(p_length),
                      UP_UINT64(p_offset),
                      UP_END);
    p_dev = (struct nfp_device *) h_dev;

    if (!ipcclient_check_handle(con, (void *)p_dev)) {
        NS_LOG(NS_LOG_LVL_WARN, "%s: got bad handle %p", __func__, p_dev);
        p_dev = NULL;
    }

    p_buffer = malloc(p_length);
    if (!p_buffer)
        err = ENOMEM;

    ipcclient_trace(con, "nfp_ustore_read");
    retval = nfp_ustore_read(p_dev, p_meid, p_buffer, p_length, p_offset);
    err = errno;

    ns_ipc_buf_pack(b_out,
                    PK_INT32(err),
                    PK_INT64(retval),
                    PK_BYTES(p_buffer, p_length),
                    PK_END);

    if (p_buffer)
        free(p_buffer);
}

static void __nfp_ustore_write(ns_ipc_server_conn_handle_t con,
                               ns_ipc_buf_t *b_out,
                               ns_ipc_buf_t *b_in)
{
    int err = 0;
    struct nfp_device * p_dev;
    uint64_t h_dev;
    int32_t p_meid;
    const void * p_buffer = NULL;
    uint64_t p_length;
    uint64_t p_offset;
    ssize_t retval = 0;

    ns_ipc_buf_unpack(b_in,
                      UP_UINT64(h_dev),
                      UP_INT32(p_meid),
                      UP_BYTES(p_buffer, p_length),
                      UP_UINT64(p_length),
                      UP_UINT64(p_offset),
                      UP_END);
    p_dev = (struct nfp_device *) h_dev;

    if (!ipcclient_check_handle(con, (void *)p_dev)) {
        NS_LOG(NS_LOG_LVL_WARN, "%s: got bad handle %p", __func__, p_dev);
        p_dev = NULL;
    }

    ipcclient_trace(con, "nfp_ustore_write");
    retval = nfp_ustore_write(p_dev, p_meid, p_buffer, p_length, p_offset);
    err = errno;

    ns_ipc_buf_pack(b_out,
                    PK_INT32(err),
                    PK_INT64(retval),
                    PK_END);

}

static void __nfp_ustore_fill(ns_ipc_server_conn_handle_t con,
                              ns_ipc_buf_t *b_out,
                              ns_ipc_buf_t *b_in)
{
    int err = 0;
    struct nfp_device * p_dev;
    uint64_t h_dev;
    int32_t p_meid;
    const void * p_buffer = NULL;
    uint64_t p_buf_length;
    uint64_t p_offset;
    uint64_t p_write_length;
    ssize_t retval = 0;

    ns_ipc_buf_unpack(b_in,
                      UP_UINT64(h_dev),
                      UP_INT32(p_meid),
                      UP_BYTES(p_buffer, p_buf_length),
                      UP_UINT64(p_buf_length),
                      UP_UINT64(p_offset),
                      UP_UINT64(p_write_length),
                      UP_END);
    p_dev = (struct nfp_device *) h_dev;

    if (!ipcclient_check_handle(con, (void *)p_dev)) {
        NS_LOG(NS_LOG_LVL_WARN, "%s: got bad handle %p", __func__, p_dev);
        p_dev = NULL;
    }

    ipcclient_trace(con, "nfp_ustore_fill");
    retval = nfp_ustore_fill(p_dev, p_meid, p_buffer, p_buf_length, p_offset, p_write_length);
    err = errno;

    ns_ipc_buf_pack(b_out,
                    PK_INT32(err),
                    PK_INT64(retval),
                    PK_END);

}

static void __nfp_crypto_sram_read(ns_ipc_server_conn_handle_t con,
                                   ns_ipc_buf_t *b_out,
                                   ns_ipc_buf_t *b_in)
{
    int err = 0;
    struct nfp_device * p_dev;
    uint64_t h_dev;
    int32_t p_islnum;
    void * p_buffer = NULL;
    uint64_t p_length;
    uint64_t p_offset;
    ssize_t retval = 0;

    ns_ipc_buf_unpack(b_in,
                      UP_UINT64(h_dev),
                      UP_INT32(p_islnum),
                      UP_UINT64(p_length),
                      UP_UINT64(p_offset),
                      UP_END);
    p_dev = (struct nfp_device *) h_dev;

    if (!ipcclient_check_handle(con, (void *)p_dev)) {
        NS_LOG(NS_LOG_LVL_WARN, "%s: got bad handle %p", __func__, p_dev);
        p_dev = NULL;
    }

    p_buffer = malloc(p_length);
    if (!p_buffer)
        err = ENOMEM;

    ipcclient_trace(con, "nfp_crypto_sram_read");
    retval = nfp_crypto_sram_read(p_dev, p_islnum, p_buffer, p_length, p_offset);
    err = errno;

    ns_ipc_buf_pack(b_out,
                    PK_INT32(err),
                    PK_INT64(retval),
                    PK_BYTES(p_buffer, p_length),
                    PK_END);

    if (p_buffer)
        free(p_buffer);
}

static void __nfp_crypto_sram_write(ns_ipc_server_conn_handle_t con,
                                    ns_ipc_buf_t *b_out,
                                    ns_ipc_buf_t *b_in)
{
    int err = 0;
    struct nfp_device * p_dev;
    uint64_t h_dev;
    int32_t p_islnum;
    const void * p_buffer = NULL;
    uint64_t p_length;
    uint64_t p_offset;
    ssize_t retval = 0;

    ns_ipc_buf_unpack(b_in,
                      UP_UINT64(h_dev),
                      UP_INT32(p_islnum),
                      UP_BYTES(p_buffer, p_length),
                      UP_UINT64(p_length),
                      UP_UINT64(p_offset),
                      UP_END);
    p_dev = (struct nfp_device *) h_dev;

    if (!ipcclient_check_handle(con, (void *)p_dev)) {
        NS_LOG(NS_LOG_LVL_WARN, "%s: got bad handle %p", __func__, p_dev);
        p_dev = NULL;
    }

    ipcclient_trace(con, "nfp_crypto_sram_write");
    retval = nfp_crypto_sram_write(p_dev, p_islnum, p_buffer, p_length, p_offset);
    err = errno;

    ns_ipc_buf_pack(b_out,
                    PK_INT32(err),
                    PK_INT64(retval),
                    PK_END);

}

static void __nfp_crypto_sram_fill(ns_ipc_server_conn_handle_t con,
                                   ns_ipc_buf_t *b_out,
                                   ns_ipc_buf_t *b_in)
{
    int err = 0;
    struct nfp_device * p_dev;
    uint64_t h_dev;
    int32_t p_islnum;
    const void * p_buffer = NULL;
    uint64_t p_buf_length;
    uint64_t p_offset;
    uint64_t p_write_length;
    ssize_t retval = 0;

    ns_ipc_buf_unpack(b_in,
                      UP_UINT64(h_dev),
                      UP_INT32(p_islnum),
                      UP_BYTES(p_buffer, p_buf_length),
                      UP_UINT64(p_buf_length),
                      UP_UINT64(p_offset),
                      UP_UINT64(p_write_length),
                      UP_END);
    p_dev = (struct nfp_device *) h_dev;

    if (!ipcclient_check_handle(con, (void *)p_dev)) {
        NS_LOG(NS_LOG_LVL_WARN, "%s: got bad handle %p", __func__, p_dev);
        p_dev = NULL;
    }

    ipcclient_trace(con, "nfp_crypto_sram_fill");
    retval = nfp_crypto_sram_fill(p_dev, p_islnum, p_buffer, p_buf_length, p_offset, p_write_length);
    err = errno;

    ns_ipc_buf_pack(b_out,
                    PK_INT32(err),
                    PK_INT64(retval),
                    PK_END);

}

static void __nfp_mecsr_read(ns_ipc_server_conn_handle_t con,
                             ns_ipc_buf_t *b_out,
                             ns_ipc_buf_t *b_in)
{
    int err = 0;
    struct nfp_device * p_dev;
    uint64_t h_dev;
    int32_t p_meid;
    int32_t p_ctx;
    uint32_t p_csr;
    uint32_t _p_value[1];
    uint32_t * p_value = _p_value;
    int retval = 0;

    ns_ipc_buf_unpack(b_in,
                      UP_UINT64(h_dev),
                      UP_INT32(p_meid),
                      UP_INT32(p_ctx),
                      UP_UINT32(p_csr),
                      UP_END);
    p_dev = (struct nfp_device *) h_dev;

    if (!ipcclient_check_handle(con, (void *)p_dev)) {
        NS_LOG(NS_LOG_LVL_WARN, "%s: got bad handle %p", __func__, p_dev);
        p_dev = NULL;
    }

    ipcclient_trace(con, "nfp_mecsr_read");
    retval = nfp_mecsr_read(p_dev, p_meid, p_ctx, p_csr, p_value);
    err = errno;

    ns_ipc_buf_pack(b_out,
                    PK_INT32(err),
                    PK_INT32(retval),
                    PK_BYTES(p_value, sizeof(_p_value)),
                    PK_END);

}

static void __nfp_mecsr_write(ns_ipc_server_conn_handle_t con,
                              ns_ipc_buf_t *b_out,
                              ns_ipc_buf_t *b_in)
{
    int err = 0;
    struct nfp_device * p_dev;
    uint64_t h_dev;
    int32_t p_meid;
    int32_t p_ctx;
    uint32_t p_csr;
    uint32_t p_value;
    int retval = 0;

    ns_ipc_buf_unpack(b_in,
                      UP_UINT64(h_dev),
                      UP_INT32(p_meid),
                      UP_INT32(p_ctx),
                      UP_UINT32(p_csr),
                      UP_UINT32(p_value),
                      UP_END);
    p_dev = (struct nfp_device *) h_dev;

    if (!ipcclient_check_handle(con, (void *)p_dev)) {
        NS_LOG(NS_LOG_LVL_WARN, "%s: got bad handle %p", __func__, p_dev);
        p_dev = NULL;
    }

    ipcclient_trace(con, "nfp_mecsr_write");
    retval = nfp_mecsr_write(p_dev, p_meid, p_ctx, p_csr, p_value);
    err = errno;

    ns_ipc_buf_pack(b_out,
                    PK_INT32(err),
                    PK_INT32(retval),
                    PK_END);

}

static void __nfp_me_register_read(ns_ipc_server_conn_handle_t con,
                                   ns_ipc_buf_t *b_out,
                                   ns_ipc_buf_t *b_in)
{
    int err = 0;
    struct nfp_device * p_dev;
    uint64_t h_dev;
    int32_t p_meid;
    uint32_t p_reg;
    uint32_t _p_value[1];
    uint32_t * p_value = _p_value;
    int retval = 0;

    ns_ipc_buf_unpack(b_in,
                      UP_UINT64(h_dev),
                      UP_INT32(p_meid),
                      UP_UINT32(p_reg),
                      UP_END);
    p_dev = (struct nfp_device *) h_dev;

    if (!ipcclient_check_handle(con, (void *)p_dev)) {
        NS_LOG(NS_LOG_LVL_WARN, "%s: got bad handle %p", __func__, p_dev);
        p_dev = NULL;
    }

    ipcclient_trace(con, "nfp_me_register_read");
    retval = nfp_me_register_read(p_dev, p_meid, p_reg, p_value);
    err = errno;

    ns_ipc_buf_pack(b_out,
                    PK_INT32(err),
                    PK_INT32(retval),
                    PK_BYTES(p_value, sizeof(_p_value)),
                    PK_END);

}

static void __nfp_me_register_write(ns_ipc_server_conn_handle_t con,
                                    ns_ipc_buf_t *b_out,
                                    ns_ipc_buf_t *b_in)
{
    int err = 0;
    struct nfp_device * p_dev;
    uint64_t h_dev;
    int32_t p_meid;
    uint32_t p_reg;
    uint32_t p_value;
    int retval = 0;

    ns_ipc_buf_unpack(b_in,
                      UP_UINT64(h_dev),
                      UP_INT32(p_meid),
                      UP_UINT32(p_reg),
                      UP_UINT32(p_value),
                      UP_END);
    p_dev = (struct nfp_device *) h_dev;

    if (!ipcclient_check_handle(con, (void *)p_dev)) {
        NS_LOG(NS_LOG_LVL_WARN, "%s: got bad handle %p", __func__, p_dev);
        p_dev = NULL;
    }

    ipcclient_trace(con, "nfp_me_register_write");
    retval = nfp_me_register_write(p_dev, p_meid, p_reg, p_value);
    err = errno;

    ns_ipc_buf_pack(b_out,
                    PK_INT32(err),
                    PK_INT32(retval),
                    PK_END);

}

static void __nfp_temp(ns_ipc_server_conn_handle_t con,
                       ns_ipc_buf_t *b_out,
                       ns_ipc_buf_t *b_in)
{
    int err = 0;
    struct nfp_device * p_dev;
    uint64_t h_dev;
    uint32_t _p_raw_value[1];
    uint32_t * p_raw_value = _p_raw_value;
    float _p_temp_c[1];
    float * p_temp_c = _p_temp_c;
    float _p_temp_err[1];
    float * p_temp_err = _p_temp_err;
    int retval = 0;

    ns_ipc_buf_unpack(b_in,
                      UP_UINT64(h_dev),
                      UP_END);
    p_dev = (struct nfp_device *) h_dev;

    if (!ipcclient_check_handle(con, (void *)p_dev)) {
        NS_LOG(NS_LOG_LVL_WARN, "%s: got bad handle %p", __func__, p_dev);
        p_dev = NULL;
    }

    ipcclient_trace(con, "nfp_temp");
    retval = nfp_temp(p_dev, p_raw_value, p_temp_c, p_temp_err);
    err = errno;

    ns_ipc_buf_pack(b_out,
                    PK_INT32(err),
                    PK_INT32(retval),
                    PK_BYTES(p_raw_value, sizeof(_p_raw_value)),
                    PK_BYTES(p_temp_c, sizeof(_p_temp_c)),
                    PK_BYTES(p_temp_err, sizeof(_p_temp_err)),
                    PK_END);

}

static void __nfp_rpc_version(ns_ipc_server_conn_handle_t con,
                              ns_ipc_buf_t *b_out,
                              ns_ipc_buf_t *b_in)
{
    ns_ipc_buf_pack(b_out,
                    PK_UINT32(1),
                    PK_UINT32(1),
                    PK_END);
}
struct ipc_server_fmap ipc_nfp_fmaps[] = {
        {"nfp_device_mask", __nfp_device_mask},
        {"nfp_num_devices", __nfp_num_devices},
        {"nfp_device_open", __nfp_device_open},
        {"nfp_device_from_cpp", __nfp_device_from_cpp},
        {"nfp_device_close", __nfp_device_close},
        {"nfp_sram_read", __nfp_sram_read},
        {"nfp_sram_write", __nfp_sram_write},
        {"nfp_sram_fill", __nfp_sram_fill},
        {"nfp_emem_read", __nfp_emem_read},
        {"nfp_emem_write", __nfp_emem_write},
        {"nfp_emem_fill", __nfp_emem_fill},
        {"nfp_emem64_read", __nfp_emem64_read},
        {"nfp_emem64_write", __nfp_emem64_write},
        {"nfp_emem64_fill", __nfp_emem64_fill},
        {"nfp_imem_read", __nfp_imem_read},
        {"nfp_imem_write", __nfp_imem_write},
        {"nfp_imem_fill", __nfp_imem_fill},
        {"nfp_imem64_read", __nfp_imem64_read},
        {"nfp_imem64_write", __nfp_imem64_write},
        {"nfp_imem64_fill", __nfp_imem64_fill},
        {"nfp_ctm_read", __nfp_ctm_read},
        {"nfp_ctm_write", __nfp_ctm_write},
        {"nfp_ctm_fill", __nfp_ctm_fill},
        {"nfp_ctm64_read", __nfp_ctm64_read},
        {"nfp_ctm64_write", __nfp_ctm64_write},
        {"nfp_ctm64_fill", __nfp_ctm64_fill},
        {"nfp_cls_read", __nfp_cls_read},
        {"nfp_cls_write", __nfp_cls_write},
        {"nfp_cls_fill", __nfp_cls_fill},
        {"nfp_lmem_read", __nfp_lmem_read},
        {"nfp_lmem_write", __nfp_lmem_write},
        {"nfp_lmem_fill", __nfp_lmem_fill},
        {"nfp_ustore_read", __nfp_ustore_read},
        {"nfp_ustore_write", __nfp_ustore_write},
        {"nfp_ustore_fill", __nfp_ustore_fill},
        {"nfp_crypto_sram_read", __nfp_crypto_sram_read},
        {"nfp_crypto_sram_write", __nfp_crypto_sram_write},
        {"nfp_crypto_sram_fill", __nfp_crypto_sram_fill},
        {"nfp_mecsr_read", __nfp_mecsr_read},
        {"nfp_mecsr_write", __nfp_mecsr_write},
        {"nfp_me_register_read", __nfp_me_register_read},
        {"nfp_me_register_write", __nfp_me_register_write},
        {"nfp_temp", __nfp_temp},
        {"nfp_rpc_version", __nfp_rpc_version},
        {NULL, NULL}};
