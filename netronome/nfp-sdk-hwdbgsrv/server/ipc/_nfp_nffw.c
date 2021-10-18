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
#include "nfp_nffw.h"

#include "nfp_nffw_handles.h"
#include "ipc_support.h"
#ifdef _MSC_VER    /* This block is specific to MS VC++. */
    #include <winerror.h>
    #define EPROTO WSAEPROTOTYPE
#endif

static void __nfp_nffw_load(ns_ipc_server_conn_handle_t con,
                            ns_ipc_buf_t *b_out,
                            ns_ipc_buf_t *b_in)
{
    int err = 0;
    struct nfp_device * p_nfp;
    uint64_t h_nfp;
    const void * p_data = NULL;
    int64_t p_size;
    uint8_t _p_fwid[1];
    uint8_t * p_fwid = _p_fwid;
    int retval = 0;

    ns_ipc_buf_unpack(b_in,
                      UP_UINT64(h_nfp),
                      UP_BYTES(p_data, p_size),
                      UP_INT64(p_size),
                      UP_END);
    p_nfp = (struct nfp_device *) h_nfp;

    if (!ipcclient_check_handle(con, (void *)p_nfp)) {
        NS_LOG(NS_LOG_LVL_WARN, "%s: got bad handle %p", __func__, p_nfp);
        p_nfp = NULL;
    }

    ipcclient_trace(con, "nfp_nffw_load");
    retval = nfp_nffw_load(p_nfp, p_data, p_size, p_fwid);
    err = errno;

    ns_ipc_buf_pack(b_out,
                    PK_INT32(err),
                    PK_INT32(retval),
                    PK_BYTES(p_fwid, sizeof(_p_fwid)),
                    PK_END);

}

static void __nfp_nffw_load_mutable(ns_ipc_server_conn_handle_t con,
                                    ns_ipc_buf_t *b_out,
                                    ns_ipc_buf_t *b_in)
{
    int err = 0;
    struct nfp_device * p_nfp;
    uint64_t h_nfp;
    void * p_data = NULL;
    int64_t p_size;
    uint8_t _p_fwid[1];
    uint8_t * p_fwid = _p_fwid;
    int retval = 0;

    ns_ipc_buf_unpack(b_in,
                      UP_UINT64(h_nfp),
                      UP_BYTES(p_data, p_size),
                      UP_INT64(p_size),
                      UP_END);
    p_nfp = (struct nfp_device *) h_nfp;

    if (!ipcclient_check_handle(con, (void *)p_nfp)) {
        NS_LOG(NS_LOG_LVL_WARN, "%s: got bad handle %p", __func__, p_nfp);
        p_nfp = NULL;
    }

    ipcclient_trace(con, "nfp_nffw_load_mutable");
    retval = nfp_nffw_load_mutable(p_nfp, p_data, p_size, p_fwid);
    err = errno;

    ns_ipc_buf_pack(b_out,
                    PK_INT32(err),
                    PK_INT32(retval),
                    PK_BYTES(p_fwid, sizeof(_p_fwid)),
                    PK_END);

}

static void __nfp_nffw_start(ns_ipc_server_conn_handle_t con,
                             ns_ipc_buf_t *b_out,
                             ns_ipc_buf_t *b_in)
{
    int err = 0;
    struct nfp_device * p_nfp;
    uint64_t h_nfp;
    uint32_t p_fwid;
    int retval = 0;

    ns_ipc_buf_unpack(b_in,
                      UP_UINT64(h_nfp),
                      UP_UINT32(p_fwid),
                      UP_END);
    p_nfp = (struct nfp_device *) h_nfp;

    if (!ipcclient_check_handle(con, (void *)p_nfp)) {
        NS_LOG(NS_LOG_LVL_WARN, "%s: got bad handle %p", __func__, p_nfp);
        p_nfp = NULL;
    }

    ipcclient_trace(con, "nfp_nffw_start");
    retval = nfp_nffw_start(p_nfp, p_fwid);
    err = errno;

    ns_ipc_buf_pack(b_out,
                    PK_INT32(err),
                    PK_INT32(retval),
                    PK_END);

}

static void __nfp_nffw_unload(ns_ipc_server_conn_handle_t con,
                              ns_ipc_buf_t *b_out,
                              ns_ipc_buf_t *b_in)
{
    int err = 0;
    struct nfp_device * p_nfp;
    uint64_t h_nfp;
    uint32_t p_fwid;
    int retval = 0;

    ns_ipc_buf_unpack(b_in,
                      UP_UINT64(h_nfp),
                      UP_UINT32(p_fwid),
                      UP_END);
    p_nfp = (struct nfp_device *) h_nfp;

    if (!ipcclient_check_handle(con, (void *)p_nfp)) {
        NS_LOG(NS_LOG_LVL_WARN, "%s: got bad handle %p", __func__, p_nfp);
        p_nfp = NULL;
    }

    ipcclient_trace(con, "nfp_nffw_unload");
    retval = nfp_nffw_unload(p_nfp, p_fwid);
    err = errno;

    ns_ipc_buf_pack(b_out,
                    PK_INT32(err),
                    PK_INT32(retval),
                    PK_END);

}

static void __nfp_nffw_info_acquire(ns_ipc_server_conn_handle_t con,
                                    ns_ipc_buf_t *b_out,
                                    ns_ipc_buf_t *b_in)
{
    int err = 0;
    struct nfp_device * p_nfp;
    uint64_t h_nfp;
    int retval = 0;

    ns_ipc_buf_unpack(b_in,
                      UP_UINT64(h_nfp),
                      UP_END);
    p_nfp = (struct nfp_device *) h_nfp;

    if (!ipcclient_check_handle(con, (void *)p_nfp)) {
        NS_LOG(NS_LOG_LVL_WARN, "%s: got bad handle %p", __func__, p_nfp);
        p_nfp = NULL;
    }

    ipcclient_trace(con, "nfp_nffw_info_acquire");
    retval = nfp_nffw_info_acquire(p_nfp);
    err = errno;

    ns_ipc_buf_pack(b_out,
                    PK_INT32(err),
                    PK_INT32(retval),
                    PK_END);

}

static void __nfp_nffw_info_release(ns_ipc_server_conn_handle_t con,
                                    ns_ipc_buf_t *b_out,
                                    ns_ipc_buf_t *b_in)
{
    int err = 0;
    struct nfp_device * p_nfp;
    uint64_t h_nfp;
    int retval = 0;

    ns_ipc_buf_unpack(b_in,
                      UP_UINT64(h_nfp),
                      UP_END);
    p_nfp = (struct nfp_device *) h_nfp;

    if (!ipcclient_check_handle(con, (void *)p_nfp)) {
        NS_LOG(NS_LOG_LVL_WARN, "%s: got bad handle %p", __func__, p_nfp);
        p_nfp = NULL;
    }

    ipcclient_trace(con, "nfp_nffw_info_release");
    retval = nfp_nffw_info_release(p_nfp);
    err = errno;

    ns_ipc_buf_pack(b_out,
                    PK_INT32(err),
                    PK_INT32(retval),
                    PK_END);

}

static void __nfp_nffw_info_fw_loaded(ns_ipc_server_conn_handle_t con,
                                      ns_ipc_buf_t *b_out,
                                      ns_ipc_buf_t *b_in)
{
    int err = 0;
    struct nfp_device * p_nfp;
    uint64_t h_nfp;
    int retval = 0;

    ns_ipc_buf_unpack(b_in,
                      UP_UINT64(h_nfp),
                      UP_END);
    p_nfp = (struct nfp_device *) h_nfp;

    if (!ipcclient_check_handle(con, (void *)p_nfp)) {
        NS_LOG(NS_LOG_LVL_WARN, "%s: got bad handle %p", __func__, p_nfp);
        p_nfp = NULL;
    }

    ipcclient_trace(con, "nfp_nffw_info_fw_loaded");
    retval = nfp_nffw_info_fw_loaded(p_nfp);
    err = errno;

    ns_ipc_buf_pack(b_out,
                    PK_INT32(err),
                    PK_INT32(retval),
                    PK_END);

}

static void __nfp_nffw_info_fw_debugger_attached(ns_ipc_server_conn_handle_t con,
                                                 ns_ipc_buf_t *b_out,
                                                 ns_ipc_buf_t *b_in)
{
    int err = 0;
    struct nfp_device * p_nfp;
    uint64_t h_nfp;
    int retval = 0;

    ns_ipc_buf_unpack(b_in,
                      UP_UINT64(h_nfp),
                      UP_END);
    p_nfp = (struct nfp_device *) h_nfp;

    if (!ipcclient_check_handle(con, (void *)p_nfp)) {
        NS_LOG(NS_LOG_LVL_WARN, "%s: got bad handle %p", __func__, p_nfp);
        p_nfp = NULL;
    }

    ipcclient_trace(con, "nfp_nffw_info_fw_debugger_attached");
    retval = nfp_nffw_info_fw_debugger_attached(p_nfp);
    err = errno;

    ns_ipc_buf_pack(b_out,
                    PK_INT32(err),
                    PK_INT32(retval),
                    PK_END);

}

static void __nfp_nffw_info_fw_modular(ns_ipc_server_conn_handle_t con,
                                       ns_ipc_buf_t *b_out,
                                       ns_ipc_buf_t *b_in)
{
    int err = 0;
    struct nfp_device * p_nfp;
    uint64_t h_nfp;
    int retval = 0;

    ns_ipc_buf_unpack(b_in,
                      UP_UINT64(h_nfp),
                      UP_END);
    p_nfp = (struct nfp_device *) h_nfp;

    if (!ipcclient_check_handle(con, (void *)p_nfp)) {
        NS_LOG(NS_LOG_LVL_WARN, "%s: got bad handle %p", __func__, p_nfp);
        p_nfp = NULL;
    }

    ipcclient_trace(con, "nfp_nffw_info_fw_modular");
    retval = nfp_nffw_info_fw_modular(p_nfp);
    err = errno;

    ns_ipc_buf_pack(b_out,
                    PK_INT32(err),
                    PK_INT32(retval),
                    PK_END);

}

static void __nfp_nffw_info_me_ctxmask(ns_ipc_server_conn_handle_t con,
                                       ns_ipc_buf_t *b_out,
                                       ns_ipc_buf_t *b_in)
{
    int err = 0;
    struct nfp_device * p_nfp;
    uint64_t h_nfp;
    int32_t p_meid;
    uint8_t retval = 0;

    ns_ipc_buf_unpack(b_in,
                      UP_UINT64(h_nfp),
                      UP_INT32(p_meid),
                      UP_END);
    p_nfp = (struct nfp_device *) h_nfp;

    if (!ipcclient_check_handle(con, (void *)p_nfp)) {
        NS_LOG(NS_LOG_LVL_WARN, "%s: got bad handle %p", __func__, p_nfp);
        p_nfp = NULL;
    }

    ipcclient_trace(con, "nfp_nffw_info_me_ctxmask");
    retval = nfp_nffw_info_me_ctxmask(p_nfp, p_meid);
    err = errno;

    ns_ipc_buf_pack(b_out,
                    PK_INT32(err),
                    PK_UINT32(retval),
                    PK_END);

}

static void __nfp_nffw_info_me_ctxmask_set(ns_ipc_server_conn_handle_t con,
                                           ns_ipc_buf_t *b_out,
                                           ns_ipc_buf_t *b_in)
{
    int err = 0;
    struct nfp_device * p_nfp;
    uint64_t h_nfp;
    int32_t p_meid;
    uint32_t p_ctxmask;
    int retval = 0;

    ns_ipc_buf_unpack(b_in,
                      UP_UINT64(h_nfp),
                      UP_INT32(p_meid),
                      UP_UINT32(p_ctxmask),
                      UP_END);
    p_nfp = (struct nfp_device *) h_nfp;

    if (!ipcclient_check_handle(con, (void *)p_nfp)) {
        NS_LOG(NS_LOG_LVL_WARN, "%s: got bad handle %p", __func__, p_nfp);
        p_nfp = NULL;
    }

    ipcclient_trace(con, "nfp_nffw_info_me_ctxmask_set");
    retval = nfp_nffw_info_me_ctxmask_set(p_nfp, p_meid, p_ctxmask);
    err = errno;

    ns_ipc_buf_pack(b_out,
                    PK_INT32(err),
                    PK_INT32(retval),
                    PK_END);

}

static void __nfp_nffw_info_me_fwid(ns_ipc_server_conn_handle_t con,
                                    ns_ipc_buf_t *b_out,
                                    ns_ipc_buf_t *b_in)
{
    int err = 0;
    struct nfp_device * p_nfp;
    uint64_t h_nfp;
    int32_t p_meid;
    uint8_t retval = 0;

    ns_ipc_buf_unpack(b_in,
                      UP_UINT64(h_nfp),
                      UP_INT32(p_meid),
                      UP_END);
    p_nfp = (struct nfp_device *) h_nfp;

    if (!ipcclient_check_handle(con, (void *)p_nfp)) {
        NS_LOG(NS_LOG_LVL_WARN, "%s: got bad handle %p", __func__, p_nfp);
        p_nfp = NULL;
    }

    ipcclient_trace(con, "nfp_nffw_info_me_fwid");
    retval = nfp_nffw_info_me_fwid(p_nfp, p_meid);
    err = errno;

    ns_ipc_buf_pack(b_out,
                    PK_INT32(err),
                    PK_UINT32(retval),
                    PK_END);

}

static void __nfp_nffw_info_me_fwid_set(ns_ipc_server_conn_handle_t con,
                                        ns_ipc_buf_t *b_out,
                                        ns_ipc_buf_t *b_in)
{
    int err = 0;
    struct nfp_device * p_nfp;
    uint64_t h_nfp;
    int32_t p_meid;
    uint32_t p_fwid;
    int retval = 0;

    ns_ipc_buf_unpack(b_in,
                      UP_UINT64(h_nfp),
                      UP_INT32(p_meid),
                      UP_UINT32(p_fwid),
                      UP_END);
    p_nfp = (struct nfp_device *) h_nfp;

    if (!ipcclient_check_handle(con, (void *)p_nfp)) {
        NS_LOG(NS_LOG_LVL_WARN, "%s: got bad handle %p", __func__, p_nfp);
        p_nfp = NULL;
    }

    ipcclient_trace(con, "nfp_nffw_info_me_fwid_set");
    retval = nfp_nffw_info_me_fwid_set(p_nfp, p_meid, p_fwid);
    err = errno;

    ns_ipc_buf_pack(b_out,
                    PK_INT32(err),
                    PK_INT32(retval),
                    PK_END);

}

static void __nfp_nffw_info_fwid_first(ns_ipc_server_conn_handle_t con,
                                       ns_ipc_buf_t *b_out,
                                       ns_ipc_buf_t *b_in)
{
    int err = 0;
    struct nfp_device * p_nfp;
    uint64_t h_nfp;
    uint8_t retval = 0;

    ns_ipc_buf_unpack(b_in,
                      UP_UINT64(h_nfp),
                      UP_END);
    p_nfp = (struct nfp_device *) h_nfp;

    if (!ipcclient_check_handle(con, (void *)p_nfp)) {
        NS_LOG(NS_LOG_LVL_WARN, "%s: got bad handle %p", __func__, p_nfp);
        p_nfp = NULL;
    }

    ipcclient_trace(con, "nfp_nffw_info_fwid_first");
    retval = nfp_nffw_info_fwid_first(p_nfp);
    err = errno;

    ns_ipc_buf_pack(b_out,
                    PK_INT32(err),
                    PK_UINT32(retval),
                    PK_END);

}

static void __nfp_nffw_info_fwid_next(ns_ipc_server_conn_handle_t con,
                                      ns_ipc_buf_t *b_out,
                                      ns_ipc_buf_t *b_in)
{
    int err = 0;
    struct nfp_device * p_nfp;
    uint64_t h_nfp;
    uint32_t p_fwid;
    uint8_t retval = 0;

    ns_ipc_buf_unpack(b_in,
                      UP_UINT64(h_nfp),
                      UP_UINT32(p_fwid),
                      UP_END);
    p_nfp = (struct nfp_device *) h_nfp;

    if (!ipcclient_check_handle(con, (void *)p_nfp)) {
        NS_LOG(NS_LOG_LVL_WARN, "%s: got bad handle %p", __func__, p_nfp);
        p_nfp = NULL;
    }

    ipcclient_trace(con, "nfp_nffw_info_fwid_next");
    retval = nfp_nffw_info_fwid_next(p_nfp, p_fwid);
    err = errno;

    ns_ipc_buf_pack(b_out,
                    PK_INT32(err),
                    PK_UINT32(retval),
                    PK_END);

}

static void __nfp_nffw_info_fw_mip(ns_ipc_server_conn_handle_t con,
                                   ns_ipc_buf_t *b_out,
                                   ns_ipc_buf_t *b_in)
{
    int err = 0;
    struct nfp_device * p_nfp;
    uint64_t h_nfp;
    uint32_t p_fwid;
    uint32_t _p_cpp_id[1];
    uint32_t * p_cpp_id = _p_cpp_id;
    uint64_t _p_off[1];
    uint64_t * p_off = _p_off;
    int retval = 0;

    ns_ipc_buf_unpack(b_in,
                      UP_UINT64(h_nfp),
                      UP_UINT32(p_fwid),
                      UP_END);
    p_nfp = (struct nfp_device *) h_nfp;

    if (!ipcclient_check_handle(con, (void *)p_nfp)) {
        NS_LOG(NS_LOG_LVL_WARN, "%s: got bad handle %p", __func__, p_nfp);
        p_nfp = NULL;
    }

    ipcclient_trace(con, "nfp_nffw_info_fw_mip");
    retval = nfp_nffw_info_fw_mip(p_nfp, p_fwid, p_cpp_id, p_off);
    err = errno;

    ns_ipc_buf_pack(b_out,
                    PK_INT32(err),
                    PK_INT32(retval),
                    PK_BYTES(p_cpp_id, sizeof(_p_cpp_id)),
                    PK_BYTES(p_off, sizeof(_p_off)),
                    PK_END);

}

static void __nfp_nffw_rpc_version(ns_ipc_server_conn_handle_t con,
                              ns_ipc_buf_t *b_out,
                              ns_ipc_buf_t *b_in)
{
    ns_ipc_buf_pack(b_out,
                    PK_UINT32(1),
                    PK_UINT32(1),
                    PK_END);
}
struct ipc_server_fmap ipc_nfp_nffw_fmaps[] = {
        {"nfp_nffw_load", __nfp_nffw_load},
        {"nfp_nffw_load_mutable", __nfp_nffw_load_mutable},
        {"nfp_nffw_start", __nfp_nffw_start},
        {"nfp_nffw_unload", __nfp_nffw_unload},
        {"nfp_nffw_info_acquire", __nfp_nffw_info_acquire},
        {"nfp_nffw_info_release", __nfp_nffw_info_release},
        {"nfp_nffw_info_fw_loaded", __nfp_nffw_info_fw_loaded},
        {"nfp_nffw_info_fw_debugger_attached", __nfp_nffw_info_fw_debugger_attached},
        {"nfp_nffw_info_fw_modular", __nfp_nffw_info_fw_modular},
        {"nfp_nffw_info_me_ctxmask", __nfp_nffw_info_me_ctxmask},
        {"nfp_nffw_info_me_ctxmask_set", __nfp_nffw_info_me_ctxmask_set},
        {"nfp_nffw_info_me_fwid", __nfp_nffw_info_me_fwid},
        {"nfp_nffw_info_me_fwid_set", __nfp_nffw_info_me_fwid_set},
        {"nfp_nffw_info_fwid_first", __nfp_nffw_info_fwid_first},
        {"nfp_nffw_info_fwid_next", __nfp_nffw_info_fwid_next},
        {"nfp_nffw_info_fw_mip", __nfp_nffw_info_fw_mip},
        {"nfp_nffw_rpc_version", __nfp_nffw_rpc_version},
        {NULL, NULL}};
