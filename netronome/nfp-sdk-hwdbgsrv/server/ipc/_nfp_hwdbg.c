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
#include "nfp_hwdbg.h"

#include "nfp_hwdbg_handles.h"
#include "ipc_support.h"
#ifdef _MSC_VER    /* This block is specific to MS VC++. */
    #include <winerror.h>
    #define EPROTO WSAEPROTOTYPE
#endif


static int local_handle_type = 0;

 /* Handle Declaration
  * Name:    nfp_hwdbg
  * #:       0
  * Type:    struct nfp_hwdbg *
  * Count:   256
  * Release: nfp_hwdbg_close
  */
#define HANDLE_TYPE_NFP_HWDBG ((local_handle_type << 16) | 0)

static void __nfp_hwdbg_open(ns_ipc_server_conn_handle_t con,
                             ns_ipc_buf_t *b_out,
                             ns_ipc_buf_t *b_in)
{
    int err = 0;
    uint32_t p_devnum;
    struct nfp_hwdbg * retval = NULL;
    uint32_t _cln_api_version = 0, _srv_api_version = 0;

    ns_ipc_buf_unpack(b_in,
                      UP_UINT32(_cln_api_version), /* client api version */
                      UP_UINT32(p_devnum),
                      UP_END);

    ipcclient_trace(con, "nfp_hwdbg_open");
    retval = nfp_hwdbg_open(p_devnum);
    err = errno;

    if (!ipcclient_version_check("nfp_hwdbg.h", 2, 2, _cln_api_version, &_srv_api_version)) {
        if (retval) {
            if (ipcclient_add_handle(con, (void *)retval,
                                     HANDLE_TYPE_NFP_HWDBG,
                                     _init_nfp_hwdbg_open,
                                     _free_nfp_hwdbg_open)) {
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

static void __nfp_hwdbg_close(ns_ipc_server_conn_handle_t con,
                              ns_ipc_buf_t *b_out,
                              ns_ipc_buf_t *b_in)
{
    int err = 0;
    struct nfp_hwdbg * p_hwdbg;
    uint64_t h_hwdbg;

    ns_ipc_buf_unpack(b_in,
                      UP_UINT64(h_hwdbg),
                      UP_END);
    p_hwdbg = (struct nfp_hwdbg *) h_hwdbg;

    if (!ipcclient_check_handle(con, (void *)p_hwdbg)) {
        NS_LOG(NS_LOG_LVL_WARN, "%s: got bad handle %p", __func__, p_hwdbg);
        p_hwdbg = NULL;
    }

    ipcclient_del_handle(con, (void *)p_hwdbg);
    ns_ipc_buf_pack(b_out,
                    PK_INT32(err),
                    PK_END);

}

static void __nfp_hwdbg_client_count(ns_ipc_server_conn_handle_t con,
                                     ns_ipc_buf_t *b_out,
                                     ns_ipc_buf_t *b_in)
{
    int err = 0;
    struct nfp_hwdbg * p_hwdbg;
    uint64_t h_hwdbg;
    int retval = 0;

    ns_ipc_buf_unpack(b_in,
                      UP_UINT64(h_hwdbg),
                      UP_END);
    p_hwdbg = (struct nfp_hwdbg *) h_hwdbg;

    if (!ipcclient_check_handle(con, (void *)p_hwdbg)) {
        NS_LOG(NS_LOG_LVL_WARN, "%s: got bad handle %p", __func__, p_hwdbg);
        p_hwdbg = NULL;
    }

    ipcclient_trace(con, "nfp_hwdbg_client_count");
    retval = nfp_hwdbg_client_count(p_hwdbg);
    err = errno;

    ns_ipc_buf_pack(b_out,
                    PK_INT32(err),
                    PK_INT32(retval),
                    PK_END);

}

static void __nfp_hwdbg_srv_reset(ns_ipc_server_conn_handle_t con,
                                  ns_ipc_buf_t *b_out,
                                  ns_ipc_buf_t *b_in)
{
    int err = 0;
    struct nfp_hwdbg * p_hwdbg;
    uint64_t h_hwdbg;
    int32_t p_resettype;

    ns_ipc_buf_unpack(b_in,
                      UP_UINT64(h_hwdbg),
                      UP_INT32(p_resettype),
                      UP_END);
    p_hwdbg = (struct nfp_hwdbg *) h_hwdbg;

    if (!ipcclient_check_handle(con, (void *)p_hwdbg)) {
        NS_LOG(NS_LOG_LVL_WARN, "%s: got bad handle %p", __func__, p_hwdbg);
        p_hwdbg = NULL;
    }

    ipcclient_trace(con, "nfp_hwdbg_srv_reset");
    nfp_hwdbg_srv_reset(p_hwdbg, p_resettype);
    err = errno;

    ns_ipc_buf_pack(b_out,
                    PK_INT32(err),
                    PK_END);

}

static void __nfp_hwdbg_set_enabled_mes(ns_ipc_server_conn_handle_t con,
                                        ns_ipc_buf_t *b_out,
                                        ns_ipc_buf_t *b_in)
{
    int err = 0;
    struct nfp_hwdbg * p_hwdbg;
    uint64_t h_hwdbg;
    int * p_meids = NULL;
    size_t _l_p_meids;
    int32_t p_len;
    int retval = 0;

    ns_ipc_buf_unpack(b_in,
                      UP_UINT64(h_hwdbg),
                      UP_BYTES(p_meids, _l_p_meids),
                      UP_INT32(p_len),
                      UP_END);
    p_hwdbg = (struct nfp_hwdbg *) h_hwdbg;

    if (!ipcclient_check_handle(con, (void *)p_hwdbg)) {
        NS_LOG(NS_LOG_LVL_WARN, "%s: got bad handle %p", __func__, p_hwdbg);
        p_hwdbg = NULL;
    }

    ipcclient_trace(con, "nfp_hwdbg_set_enabled_mes");
    retval = nfp_hwdbg_set_enabled_mes(p_hwdbg, p_meids, p_len);
    err = errno;

    ns_ipc_buf_pack(b_out,
                    PK_INT32(err),
                    PK_INT32(retval),
                    PK_END);

}

static void __nfp_hwdbg_me_status(ns_ipc_server_conn_handle_t con,
                                  ns_ipc_buf_t *b_out,
                                  ns_ipc_buf_t *b_in)
{
    int err = 0;
    struct nfp_hwdbg * p_hwdbg;
    uint64_t h_hwdbg;
    int * p_meids = NULL;
    size_t _l_p_meids;
    struct nfp_hwdbg_me_status_data * p_status = NULL;
    int32_t p_len;
    int retval = 0;

    ns_ipc_buf_unpack(b_in,
                      UP_UINT64(h_hwdbg),
                      UP_BYTES(p_meids, _l_p_meids),
                      UP_INT32(p_len),
                      UP_END);
    p_hwdbg = (struct nfp_hwdbg *) h_hwdbg;

    if (!ipcclient_check_handle(con, (void *)p_hwdbg)) {
        NS_LOG(NS_LOG_LVL_WARN, "%s: got bad handle %p", __func__, p_hwdbg);
        p_hwdbg = NULL;
    }

    p_status = malloc(sizeof(struct nfp_hwdbg_me_status_data ) * (p_len));
    if (!p_status)
        err = ENOMEM;

    ipcclient_trace(con, "nfp_hwdbg_me_status");
    retval = nfp_hwdbg_me_status(p_hwdbg, p_meids, p_status, p_len);
    err = errno;

    ns_ipc_buf_pack(b_out,
                    PK_INT32(err),
                    PK_INT32(retval),
                    PK_BYTES(p_status, sizeof(struct nfp_hwdbg_me_status_data ) * (p_len)),
                    PK_END);

    if (p_status)
        free(p_status);
}

static void __nfp_hwdbg_me_stop(ns_ipc_server_conn_handle_t con,
                                ns_ipc_buf_t *b_out,
                                ns_ipc_buf_t *b_in)
{
    int err = 0;
    struct nfp_hwdbg * p_hwdbg;
    uint64_t h_hwdbg;
    int32_t p_meid;
    int retval = 0;

    ns_ipc_buf_unpack(b_in,
                      UP_UINT64(h_hwdbg),
                      UP_INT32(p_meid),
                      UP_END);
    p_hwdbg = (struct nfp_hwdbg *) h_hwdbg;

    if (!ipcclient_check_handle(con, (void *)p_hwdbg)) {
        NS_LOG(NS_LOG_LVL_WARN, "%s: got bad handle %p", __func__, p_hwdbg);
        p_hwdbg = NULL;
    }

    ipcclient_trace(con, "nfp_hwdbg_me_stop");
    retval = nfp_hwdbg_me_stop(p_hwdbg, p_meid);
    err = errno;

    ns_ipc_buf_pack(b_out,
                    PK_INT32(err),
                    PK_INT32(retval),
                    PK_END);

}

static void __nfp_hwdbg_me_run(ns_ipc_server_conn_handle_t con,
                               ns_ipc_buf_t *b_out,
                               ns_ipc_buf_t *b_in)
{
    int err = 0;
    struct nfp_hwdbg * p_hwdbg;
    uint64_t h_hwdbg;
    int32_t p_meid;
    int32_t p_resume;
    int retval = 0;

    ns_ipc_buf_unpack(b_in,
                      UP_UINT64(h_hwdbg),
                      UP_INT32(p_meid),
                      UP_INT32(p_resume),
                      UP_END);
    p_hwdbg = (struct nfp_hwdbg *) h_hwdbg;

    if (!ipcclient_check_handle(con, (void *)p_hwdbg)) {
        NS_LOG(NS_LOG_LVL_WARN, "%s: got bad handle %p", __func__, p_hwdbg);
        p_hwdbg = NULL;
    }

    ipcclient_trace(con, "nfp_hwdbg_me_run");
    retval = nfp_hwdbg_me_run(p_hwdbg, p_meid, p_resume);
    err = errno;

    ns_ipc_buf_pack(b_out,
                    PK_INT32(err),
                    PK_INT32(retval),
                    PK_END);

}

static void __nfp_hwdbg_me_step(ns_ipc_server_conn_handle_t con,
                                ns_ipc_buf_t *b_out,
                                ns_ipc_buf_t *b_in)
{
    int err = 0;
    struct nfp_hwdbg * p_hwdbg;
    uint64_t h_hwdbg;
    int32_t p_meid;
    int32_t p_ctx;
    int retval = 0;

    ns_ipc_buf_unpack(b_in,
                      UP_UINT64(h_hwdbg),
                      UP_INT32(p_meid),
                      UP_INT32(p_ctx),
                      UP_END);
    p_hwdbg = (struct nfp_hwdbg *) h_hwdbg;

    if (!ipcclient_check_handle(con, (void *)p_hwdbg)) {
        NS_LOG(NS_LOG_LVL_WARN, "%s: got bad handle %p", __func__, p_hwdbg);
        p_hwdbg = NULL;
    }

    ipcclient_trace(con, "nfp_hwdbg_me_step");
    retval = nfp_hwdbg_me_step(p_hwdbg, p_meid, p_ctx);
    err = errno;

    ns_ipc_buf_pack(b_out,
                    PK_INT32(err),
                    PK_INT32(retval),
                    PK_END);

}

static void __nfp_hwdbg_brkgrp_add(ns_ipc_server_conn_handle_t con,
                                   ns_ipc_buf_t *b_out,
                                   ns_ipc_buf_t *b_in)
{
    int err = 0;
    struct nfp_hwdbg * p_hwdbg;
    uint64_t h_hwdbg;
    int32_t p_meid;
    int * p_breakmeids = NULL;
    size_t _l_p_breakmeids;
    int32_t p_len;
    int retval = 0;

    ns_ipc_buf_unpack(b_in,
                      UP_UINT64(h_hwdbg),
                      UP_INT32(p_meid),
                      UP_BYTES(p_breakmeids, _l_p_breakmeids),
                      UP_INT32(p_len),
                      UP_END);
    p_hwdbg = (struct nfp_hwdbg *) h_hwdbg;

    if (!ipcclient_check_handle(con, (void *)p_hwdbg)) {
        NS_LOG(NS_LOG_LVL_WARN, "%s: got bad handle %p", __func__, p_hwdbg);
        p_hwdbg = NULL;
    }

    ipcclient_trace(con, "nfp_hwdbg_brkgrp_add");
    retval = nfp_hwdbg_brkgrp_add(p_hwdbg, p_meid, p_breakmeids, p_len);
    err = errno;

    ns_ipc_buf_pack(b_out,
                    PK_INT32(err),
                    PK_INT32(retval),
                    PK_END);

}

static void __nfp_hwdbg_brkgrp_del(ns_ipc_server_conn_handle_t con,
                                   ns_ipc_buf_t *b_out,
                                   ns_ipc_buf_t *b_in)
{
    int err = 0;
    struct nfp_hwdbg * p_hwdbg;
    uint64_t h_hwdbg;
    int32_t p_meid;
    int retval = 0;

    ns_ipc_buf_unpack(b_in,
                      UP_UINT64(h_hwdbg),
                      UP_INT32(p_meid),
                      UP_END);
    p_hwdbg = (struct nfp_hwdbg *) h_hwdbg;

    if (!ipcclient_check_handle(con, (void *)p_hwdbg)) {
        NS_LOG(NS_LOG_LVL_WARN, "%s: got bad handle %p", __func__, p_hwdbg);
        p_hwdbg = NULL;
    }

    ipcclient_trace(con, "nfp_hwdbg_brkgrp_del");
    retval = nfp_hwdbg_brkgrp_del(p_hwdbg, p_meid);
    err = errno;

    ns_ipc_buf_pack(b_out,
                    PK_INT32(err),
                    PK_INT32(retval),
                    PK_END);

}

static void __nfp_hwdbg_brk_add_code(ns_ipc_server_conn_handle_t con,
                                     ns_ipc_buf_t *b_out,
                                     ns_ipc_buf_t *b_in)
{
    int err = 0;
    struct nfp_hwdbg * p_hwdbg;
    uint64_t h_hwdbg;
    int64_t p_breakid;
    int32_t p_meid;
    int32_t p_pc;
    uint32_t p_ctxmask;
    int retval = 0;

    ns_ipc_buf_unpack(b_in,
                      UP_UINT64(h_hwdbg),
                      UP_INT64(p_breakid),
                      UP_INT32(p_meid),
                      UP_INT32(p_pc),
                      UP_UINT32(p_ctxmask),
                      UP_END);
    p_hwdbg = (struct nfp_hwdbg *) h_hwdbg;

    if (!ipcclient_check_handle(con, (void *)p_hwdbg)) {
        NS_LOG(NS_LOG_LVL_WARN, "%s: got bad handle %p", __func__, p_hwdbg);
        p_hwdbg = NULL;
    }

    ipcclient_trace(con, "nfp_hwdbg_brk_add_code");
    retval = nfp_hwdbg_brk_add_code(p_hwdbg, p_breakid, p_meid, p_pc, p_ctxmask);
    err = errno;

    ns_ipc_buf_pack(b_out,
                    PK_INT32(err),
                    PK_INT32(retval),
                    PK_END);

}

static void __nfp_hwdbg_brk_del(ns_ipc_server_conn_handle_t con,
                                ns_ipc_buf_t *b_out,
                                ns_ipc_buf_t *b_in)
{
    int err = 0;
    struct nfp_hwdbg * p_hwdbg;
    uint64_t h_hwdbg;
    int64_t p_breakid;
    int retval = 0;

    ns_ipc_buf_unpack(b_in,
                      UP_UINT64(h_hwdbg),
                      UP_INT64(p_breakid),
                      UP_END);
    p_hwdbg = (struct nfp_hwdbg *) h_hwdbg;

    if (!ipcclient_check_handle(con, (void *)p_hwdbg)) {
        NS_LOG(NS_LOG_LVL_WARN, "%s: got bad handle %p", __func__, p_hwdbg);
        p_hwdbg = NULL;
    }

    ipcclient_trace(con, "nfp_hwdbg_brk_del");
    retval = nfp_hwdbg_brk_del(p_hwdbg, p_breakid);
    err = errno;

    ns_ipc_buf_pack(b_out,
                    PK_INT32(err),
                    PK_INT32(retval),
                    PK_END);

}

static void __nfp_hwdbg_brk_flush(ns_ipc_server_conn_handle_t con,
                                  ns_ipc_buf_t *b_out,
                                  ns_ipc_buf_t *b_in)
{
    int err = 0;
    struct nfp_hwdbg * p_hwdbg;
    uint64_t h_hwdbg;
    int32_t p_flushtype;
    int retval = 0;

    ns_ipc_buf_unpack(b_in,
                      UP_UINT64(h_hwdbg),
                      UP_INT32(p_flushtype),
                      UP_END);
    p_hwdbg = (struct nfp_hwdbg *) h_hwdbg;

    if (!ipcclient_check_handle(con, (void *)p_hwdbg)) {
        NS_LOG(NS_LOG_LVL_WARN, "%s: got bad handle %p", __func__, p_hwdbg);
        p_hwdbg = NULL;
    }

    ipcclient_trace(con, "nfp_hwdbg_brk_flush");
    retval = nfp_hwdbg_brk_flush(p_hwdbg, p_flushtype);
    err = errno;

    ns_ipc_buf_pack(b_out,
                    PK_INT32(err),
                    PK_INT32(retval),
                    PK_END);

}

static void __nfp_hwdbg_brk_check(ns_ipc_server_conn_handle_t con,
                                  ns_ipc_buf_t *b_out,
                                  ns_ipc_buf_t *b_in)
{
    int err = 0;
    struct nfp_hwdbg * p_hwdbg;
    uint64_t h_hwdbg;
    struct nfp_hwdbg_break_data * p_data = NULL;
    int32_t p_len;
    int _p_cnt[1];
    int * p_cnt = _p_cnt;
    int retval = 0;

    ns_ipc_buf_unpack(b_in,
                      UP_UINT64(h_hwdbg),
                      UP_INT32(p_len),
                      UP_END);
    p_hwdbg = (struct nfp_hwdbg *) h_hwdbg;

    if (!ipcclient_check_handle(con, (void *)p_hwdbg)) {
        NS_LOG(NS_LOG_LVL_WARN, "%s: got bad handle %p", __func__, p_hwdbg);
        p_hwdbg = NULL;
    }

    p_data = malloc(sizeof(struct nfp_hwdbg_break_data ) * (p_len));
    if (!p_data)
        err = ENOMEM;

    ipcclient_trace(con, "nfp_hwdbg_brk_check");
    retval = nfp_hwdbg_brk_check(p_hwdbg, p_data, p_len, p_cnt);
    err = errno;

    ns_ipc_buf_pack(b_out,
                    PK_INT32(err),
                    PK_INT32(retval),
                    PK_BYTES(p_data, sizeof(struct nfp_hwdbg_break_data ) * (p_len)),
                    PK_BYTES(p_cnt, sizeof(_p_cnt)),
                    PK_END);

    if (p_data)
        free(p_data);
}

static void __nfp_hwdbg_brk_clear(ns_ipc_server_conn_handle_t con,
                                  ns_ipc_buf_t *b_out,
                                  ns_ipc_buf_t *b_in)
{
    int err = 0;
    struct nfp_hwdbg * p_hwdbg;
    uint64_t h_hwdbg;
    int64_t p_breakid;
    int retval = 0;

    ns_ipc_buf_unpack(b_in,
                      UP_UINT64(h_hwdbg),
                      UP_INT64(p_breakid),
                      UP_END);
    p_hwdbg = (struct nfp_hwdbg *) h_hwdbg;

    if (!ipcclient_check_handle(con, (void *)p_hwdbg)) {
        NS_LOG(NS_LOG_LVL_WARN, "%s: got bad handle %p", __func__, p_hwdbg);
        p_hwdbg = NULL;
    }

    ipcclient_trace(con, "nfp_hwdbg_brk_clear");
    retval = nfp_hwdbg_brk_clear(p_hwdbg, p_breakid);
    err = errno;

    ns_ipc_buf_pack(b_out,
                    PK_INT32(err),
                    PK_INT32(retval),
                    PK_END);

}

static void __nfp_hwdbg_load_dbginfo(ns_ipc_server_conn_handle_t con,
                                     ns_ipc_buf_t *b_out,
                                     ns_ipc_buf_t *b_in)
{
    int err = 0;
    struct nfp_hwdbg * p_hwdbg;
    uint64_t h_hwdbg;
    int32_t p_meid;
    uint64_t * p_data = NULL;
    size_t _l_p_data;
    int32_t p_len;
    int32_t p_off;
    int retval = 0;

    ns_ipc_buf_unpack(b_in,
                      UP_UINT64(h_hwdbg),
                      UP_INT32(p_meid),
                      UP_BYTES(p_data, _l_p_data),
                      UP_INT32(p_len),
                      UP_INT32(p_off),
                      UP_END);
    p_hwdbg = (struct nfp_hwdbg *) h_hwdbg;

    if (!ipcclient_check_handle(con, (void *)p_hwdbg)) {
        NS_LOG(NS_LOG_LVL_WARN, "%s: got bad handle %p", __func__, p_hwdbg);
        p_hwdbg = NULL;
    }

    ipcclient_trace(con, "nfp_hwdbg_load_dbginfo");
    retval = nfp_hwdbg_load_dbginfo(p_hwdbg, p_meid, p_data, p_len, p_off);
    err = errno;

    ns_ipc_buf_pack(b_out,
                    PK_INT32(err),
                    PK_INT32(retval),
                    PK_END);

}

static void __nfp_hwdbg_server_version(ns_ipc_server_conn_handle_t con,
                                       ns_ipc_buf_t *b_out,
                                       ns_ipc_buf_t *b_in)
{
    int err = 0;
    struct nfp_hwdbg * p_hwdbg;
    uint64_t h_hwdbg;
    char * p_buf = NULL;
    int32_t p_len;
    int retval = 0;

    ns_ipc_buf_unpack(b_in,
                      UP_UINT64(h_hwdbg),
                      UP_INT32(p_len),
                      UP_END);
    p_hwdbg = (struct nfp_hwdbg *) h_hwdbg;

    if (!ipcclient_check_handle(con, (void *)p_hwdbg)) {
        NS_LOG(NS_LOG_LVL_WARN, "%s: got bad handle %p", __func__, p_hwdbg);
        p_hwdbg = NULL;
    }

    p_buf = malloc(sizeof(char ) * (p_len));
    if (!p_buf)
        err = ENOMEM;

    ipcclient_trace(con, "nfp_hwdbg_server_version");
    retval = nfp_hwdbg_server_version(p_hwdbg, p_buf, p_len);
    err = errno;

    ns_ipc_buf_pack(b_out,
                    PK_INT32(err),
                    PK_INT32(retval),
                    PK_BYTES(p_buf, sizeof(char ) * (p_len)),
                    PK_END);

    if (p_buf)
        free(p_buf);
}

static void __nfp_hwdbg_rpc_version(ns_ipc_server_conn_handle_t con,
                              ns_ipc_buf_t *b_out,
                              ns_ipc_buf_t *b_in)
{
    ns_ipc_buf_pack(b_out,
                    PK_UINT32(2),
                    PK_UINT32(2),
                    PK_END);
}
struct ipc_server_fmap ipc_nfp_hwdbg_fmaps[] = {
        {"nfp_hwdbg_open", __nfp_hwdbg_open},
        {"nfp_hwdbg_close", __nfp_hwdbg_close},
        {"nfp_hwdbg_client_count", __nfp_hwdbg_client_count},
        {"nfp_hwdbg_srv_reset", __nfp_hwdbg_srv_reset},
        {"nfp_hwdbg_set_enabled_mes", __nfp_hwdbg_set_enabled_mes},
        {"nfp_hwdbg_me_status", __nfp_hwdbg_me_status},
        {"nfp_hwdbg_me_stop", __nfp_hwdbg_me_stop},
        {"nfp_hwdbg_me_run", __nfp_hwdbg_me_run},
        {"nfp_hwdbg_me_step", __nfp_hwdbg_me_step},
        {"nfp_hwdbg_brkgrp_add", __nfp_hwdbg_brkgrp_add},
        {"nfp_hwdbg_brkgrp_del", __nfp_hwdbg_brkgrp_del},
        {"nfp_hwdbg_brk_add_code", __nfp_hwdbg_brk_add_code},
        {"nfp_hwdbg_brk_del", __nfp_hwdbg_brk_del},
        {"nfp_hwdbg_brk_flush", __nfp_hwdbg_brk_flush},
        {"nfp_hwdbg_brk_check", __nfp_hwdbg_brk_check},
        {"nfp_hwdbg_brk_clear", __nfp_hwdbg_brk_clear},
        {"nfp_hwdbg_load_dbginfo", __nfp_hwdbg_load_dbginfo},
        {"nfp_hwdbg_server_version", __nfp_hwdbg_server_version},
        {"nfp_hwdbg_rpc_version", __nfp_hwdbg_rpc_version},
        {NULL, NULL}};
