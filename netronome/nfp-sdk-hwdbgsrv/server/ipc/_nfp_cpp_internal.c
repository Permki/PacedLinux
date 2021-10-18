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
#include "nfp_cpp_internal.h"

#include "nfp_cpp_handles.h"
#include "ipc_support.h"
#ifdef _MSC_VER    /* This block is specific to MS VC++. */
    #include <winerror.h>
    #define EPROTO WSAEPROTOTYPE
#endif

static void __nfp_cpp_explicit_do_internal(ns_ipc_server_conn_handle_t con,
                                           ns_ipc_buf_t *b_out,
                                           ns_ipc_buf_t *b_in)
{
    int err = 0;
    struct nfp_cpp * p_cpp;
    uint64_t h_cpp;
    struct nfp_cpp_explicit_data_internal * p_cppdata = NULL;
    size_t _l_p_cppdata;
    void * p_push_buf = NULL;
    int64_t p_buf_len;
    int _p_push_len[1];
    int * p_push_len = _p_push_len;
    int retval = 0;

    ns_ipc_buf_unpack(b_in,
                      UP_UINT64(h_cpp),
                      UP_BYTES(p_cppdata, _l_p_cppdata),
                      UP_INT64(p_buf_len),
                      UP_END);
    p_cpp = (struct nfp_cpp *) h_cpp;

    if (!ipcclient_check_handle(con, (void *)p_cpp)) {
        NS_LOG(NS_LOG_LVL_WARN, "%s: got bad handle %p", __func__, p_cpp);
        p_cpp = NULL;
    }

    p_push_buf = malloc(p_buf_len);
    if (!p_push_buf)
        err = ENOMEM;

    ipcclient_trace(con, "nfp_cpp_explicit_do_internal");
    retval = nfp_cpp_explicit_do_internal(p_cpp, p_cppdata, p_push_buf, p_buf_len, p_push_len);
    err = errno;

    ns_ipc_buf_pack(b_out,
                    PK_INT32(err),
                    PK_INT32(retval),
                    PK_BYTES(p_push_buf, p_buf_len),
                    PK_BYTES(p_push_len, sizeof(_p_push_len)),
                    PK_END);

    if (p_push_buf)
        free(p_push_buf);
}

static void __nfp_cpp_internal_rpc_version(ns_ipc_server_conn_handle_t con,
                              ns_ipc_buf_t *b_out,
                              ns_ipc_buf_t *b_in)
{
    ns_ipc_buf_pack(b_out,
                    PK_UINT32(1),
                    PK_UINT32(1),
                    PK_END);
}
struct ipc_server_fmap ipc_nfp_cpp_internal_fmaps[] = {
        {"nfp_cpp_explicit_do_internal", __nfp_cpp_explicit_do_internal},
        {"nfp_cpp_internal_rpc_version", __nfp_cpp_internal_rpc_version},
        {NULL, NULL}};
