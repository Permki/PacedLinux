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
#include "nfp_bulk.h"

#include "nfp_handles.h"
#include "ipc_support.h"
#ifdef _MSC_VER    /* This block is specific to MS VC++. */
    #include <winerror.h>
    #define EPROTO WSAEPROTOTYPE
#endif

static void __nfp_bulk_mecsr_read(ns_ipc_server_conn_handle_t con,
                                  ns_ipc_buf_t *b_out,
                                  ns_ipc_buf_t *b_in)
{
    int err = 0;
    struct nfp_device * p_dev;
    uint64_t h_dev;
    uint32_t * p_mecsrs = NULL;
    size_t _l_p_mecsrs;
    uint32_t * p_values = NULL;
    int32_t p_len;
    int retval = 0;

    ns_ipc_buf_unpack(b_in,
                      UP_UINT64(h_dev),
                      UP_BYTES(p_mecsrs, _l_p_mecsrs),
                      UP_INT32(p_len),
                      UP_END);
    p_dev = (struct nfp_device *) h_dev;

    if (!ipcclient_check_handle(con, (void *)p_dev)) {
        NS_LOG(NS_LOG_LVL_WARN, "%s: got bad handle %p", __func__, p_dev);
        p_dev = NULL;
    }

    p_values = malloc(sizeof(uint32_t ) * (p_len));
    if (!p_values)
        err = ENOMEM;

    ipcclient_trace(con, "nfp_bulk_mecsr_read");
    retval = nfp_bulk_mecsr_read(p_dev, p_mecsrs, p_values, p_len);
    err = errno;

    ns_ipc_buf_pack(b_out,
                    PK_INT32(err),
                    PK_INT32(retval),
                    PK_BYTES(p_values, sizeof(uint32_t ) * (p_len)),
                    PK_END);

    if (p_values)
        free(p_values);
}

static void __nfp_bulk_mecsr_write(ns_ipc_server_conn_handle_t con,
                                   ns_ipc_buf_t *b_out,
                                   ns_ipc_buf_t *b_in)
{
    int err = 0;
    struct nfp_device * p_dev;
    uint64_t h_dev;
    uint32_t * p_mecsrs = NULL;
    size_t _l_p_mecsrs;
    uint32_t * p_values = NULL;
    size_t _l_p_values;
    int32_t p_len;
    int retval = 0;

    ns_ipc_buf_unpack(b_in,
                      UP_UINT64(h_dev),
                      UP_BYTES(p_mecsrs, _l_p_mecsrs),
                      UP_BYTES(p_values, _l_p_values),
                      UP_INT32(p_len),
                      UP_END);
    p_dev = (struct nfp_device *) h_dev;

    if (!ipcclient_check_handle(con, (void *)p_dev)) {
        NS_LOG(NS_LOG_LVL_WARN, "%s: got bad handle %p", __func__, p_dev);
        p_dev = NULL;
    }

    ipcclient_trace(con, "nfp_bulk_mecsr_write");
    retval = nfp_bulk_mecsr_write(p_dev, p_mecsrs, p_values, p_len);
    err = errno;

    ns_ipc_buf_pack(b_out,
                    PK_INT32(err),
                    PK_INT32(retval),
                    PK_END);

}

static void __nfp_bulk_me_register_read(ns_ipc_server_conn_handle_t con,
                                        ns_ipc_buf_t *b_out,
                                        ns_ipc_buf_t *b_in)
{
    int err = 0;
    struct nfp_device * p_dev;
    uint64_t h_dev;
    uint32_t p_meid;
    uint32_t p_reg;
    uint32_t * p_values = NULL;
    int32_t p_count;
    int retval = 0;

    ns_ipc_buf_unpack(b_in,
                      UP_UINT64(h_dev),
                      UP_UINT32(p_meid),
                      UP_UINT32(p_reg),
                      UP_INT32(p_count),
                      UP_END);
    p_dev = (struct nfp_device *) h_dev;

    if (!ipcclient_check_handle(con, (void *)p_dev)) {
        NS_LOG(NS_LOG_LVL_WARN, "%s: got bad handle %p", __func__, p_dev);
        p_dev = NULL;
    }

    p_values = malloc(sizeof(uint32_t ) * (p_count));
    if (!p_values)
        err = ENOMEM;

    ipcclient_trace(con, "nfp_bulk_me_register_read");
    retval = nfp_bulk_me_register_read(p_dev, p_meid, p_reg, p_values, p_count);
    err = errno;

    ns_ipc_buf_pack(b_out,
                    PK_INT32(err),
                    PK_INT32(retval),
                    PK_BYTES(p_values, sizeof(uint32_t ) * (p_count)),
                    PK_END);

    if (p_values)
        free(p_values);
}

static void __nfp_bulk_me_register_write(ns_ipc_server_conn_handle_t con,
                                         ns_ipc_buf_t *b_out,
                                         ns_ipc_buf_t *b_in)
{
    int err = 0;
    struct nfp_device * p_dev;
    uint64_t h_dev;
    uint32_t p_meid;
    uint32_t p_reg;
    uint32_t * p_values = NULL;
    size_t _l_p_values;
    int32_t p_count;
    int retval = 0;

    ns_ipc_buf_unpack(b_in,
                      UP_UINT64(h_dev),
                      UP_UINT32(p_meid),
                      UP_UINT32(p_reg),
                      UP_BYTES(p_values, _l_p_values),
                      UP_INT32(p_count),
                      UP_END);
    p_dev = (struct nfp_device *) h_dev;

    if (!ipcclient_check_handle(con, (void *)p_dev)) {
        NS_LOG(NS_LOG_LVL_WARN, "%s: got bad handle %p", __func__, p_dev);
        p_dev = NULL;
    }

    ipcclient_trace(con, "nfp_bulk_me_register_write");
    retval = nfp_bulk_me_register_write(p_dev, p_meid, p_reg, p_values, p_count);
    err = errno;

    ns_ipc_buf_pack(b_out,
                    PK_INT32(err),
                    PK_INT32(retval),
                    PK_END);

}

static void __nfp_bulk_xpb_read(ns_ipc_server_conn_handle_t con,
                                ns_ipc_buf_t *b_out,
                                ns_ipc_buf_t *b_in)
{
    int err = 0;
    struct nfp_cpp * p_cpp;
    uint64_t h_cpp;
    uint32_t * p_xpb_tgts = NULL;
    size_t _l_p_xpb_tgts;
    uint32_t * p_values = NULL;
    int32_t p_len;
    int retval = 0;

    ns_ipc_buf_unpack(b_in,
                      UP_UINT64(h_cpp),
                      UP_BYTES(p_xpb_tgts, _l_p_xpb_tgts),
                      UP_INT32(p_len),
                      UP_END);
    p_cpp = (struct nfp_cpp *) h_cpp;

    if (!ipcclient_check_handle(con, (void *)p_cpp)) {
        NS_LOG(NS_LOG_LVL_WARN, "%s: got bad handle %p", __func__, p_cpp);
        p_cpp = NULL;
    }

    p_values = malloc(sizeof(uint32_t ) * (p_len));
    if (!p_values)
        err = ENOMEM;

    ipcclient_trace(con, "nfp_bulk_xpb_read");
    retval = nfp_bulk_xpb_read(p_cpp, p_xpb_tgts, p_values, p_len);
    err = errno;

    ns_ipc_buf_pack(b_out,
                    PK_INT32(err),
                    PK_INT32(retval),
                    PK_BYTES(p_values, sizeof(uint32_t ) * (p_len)),
                    PK_END);

    if (p_values)
        free(p_values);
}

static void __nfp_bulk_xpb_write(ns_ipc_server_conn_handle_t con,
                                 ns_ipc_buf_t *b_out,
                                 ns_ipc_buf_t *b_in)
{
    int err = 0;
    struct nfp_cpp * p_cpp;
    uint64_t h_cpp;
    uint32_t * p_xpb_tgts = NULL;
    size_t _l_p_xpb_tgts;
    uint32_t * p_values = NULL;
    size_t _l_p_values;
    int32_t p_len;
    int retval = 0;

    ns_ipc_buf_unpack(b_in,
                      UP_UINT64(h_cpp),
                      UP_BYTES(p_xpb_tgts, _l_p_xpb_tgts),
                      UP_BYTES(p_values, _l_p_values),
                      UP_INT32(p_len),
                      UP_END);
    p_cpp = (struct nfp_cpp *) h_cpp;

    if (!ipcclient_check_handle(con, (void *)p_cpp)) {
        NS_LOG(NS_LOG_LVL_WARN, "%s: got bad handle %p", __func__, p_cpp);
        p_cpp = NULL;
    }

    ipcclient_trace(con, "nfp_bulk_xpb_write");
    retval = nfp_bulk_xpb_write(p_cpp, p_xpb_tgts, p_values, p_len);
    err = errno;

    ns_ipc_buf_pack(b_out,
                    PK_INT32(err),
                    PK_INT32(retval),
                    PK_END);

}

static void __nfp_bulk_rpc_version(ns_ipc_server_conn_handle_t con,
                              ns_ipc_buf_t *b_out,
                              ns_ipc_buf_t *b_in)
{
    ns_ipc_buf_pack(b_out,
                    PK_UINT32(1),
                    PK_UINT32(1),
                    PK_END);
}
struct ipc_server_fmap ipc_nfp_bulk_fmaps[] = {
        {"nfp_bulk_mecsr_read", __nfp_bulk_mecsr_read},
        {"nfp_bulk_mecsr_write", __nfp_bulk_mecsr_write},
        {"nfp_bulk_me_register_read", __nfp_bulk_me_register_read},
        {"nfp_bulk_me_register_write", __nfp_bulk_me_register_write},
        {"nfp_bulk_xpb_read", __nfp_bulk_xpb_read},
        {"nfp_bulk_xpb_write", __nfp_bulk_xpb_write},
        {"nfp_bulk_rpc_version", __nfp_bulk_rpc_version},
        {NULL, NULL}};
