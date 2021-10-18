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
#include "nfp_internal.h"

#include "nfp_handles.h"
#include "ipc_support.h"
#ifdef _MSC_VER    /* This block is specific to MS VC++. */
    #include <winerror.h>
    #define EPROTO WSAEPROTOTYPE
#endif

static void __nfp_get_hwinfo(ns_ipc_server_conn_handle_t con,
                             ns_ipc_buf_t *b_out,
                             ns_ipc_buf_t *b_in)
{
    int err = 0;
    struct nfp_device * p_dev;
    uint64_t h_dev;
    void * p_buffer = NULL;
    int32_t p_length;
    int retval = 0;

    ns_ipc_buf_unpack(b_in,
                      UP_UINT64(h_dev),
                      UP_INT32(p_length),
                      UP_END);
    p_dev = (struct nfp_device *) h_dev;

    if (!ipcclient_check_handle(con, (void *)p_dev)) {
        NS_LOG(NS_LOG_LVL_WARN, "%s: got bad handle %p", __func__, p_dev);
        p_dev = NULL;
    }

    p_buffer = malloc(p_length);
    if (!p_buffer)
        err = ENOMEM;

    ipcclient_trace(con, "nfp_get_hwinfo");
    retval = nfp_get_hwinfo(p_dev, p_buffer, p_length);
    err = errno;

    ns_ipc_buf_pack(b_out,
                    PK_INT32(err),
                    PK_INT32(retval),
                    PK_BYTES(p_buffer, p_length),
                    PK_END);

    if (p_buffer)
        free(p_buffer);
}

static void __nfp_internal_rpc_version(ns_ipc_server_conn_handle_t con,
                              ns_ipc_buf_t *b_out,
                              ns_ipc_buf_t *b_in)
{
    ns_ipc_buf_pack(b_out,
                    PK_UINT32(1),
                    PK_UINT32(1),
                    PK_END);
}
struct ipc_server_fmap ipc_nfp_internal_fmaps[] = {
        {"nfp_get_hwinfo", __nfp_get_hwinfo},
        {"nfp_internal_rpc_version", __nfp_internal_rpc_version},
        {NULL, NULL}};
