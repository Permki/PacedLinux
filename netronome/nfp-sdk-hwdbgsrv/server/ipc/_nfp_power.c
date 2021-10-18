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
#include "nfp_power.h"

#include "nfp_power_handles.h"
#include "ipc_support.h"
#ifdef _MSC_VER    /* This block is specific to MS VC++. */
    #include <winerror.h>
    #define EPROTO WSAEPROTOTYPE
#endif

static void __nfp_power_get(ns_ipc_server_conn_handle_t con,
                            ns_ipc_buf_t *b_out,
                            ns_ipc_buf_t *b_in)
{
    int err = 0;
    struct nfp_device * p_dev;
    uint64_t h_dev;
    uint32_t p_subdevice;
    int _p_state[1];
    int * p_state = _p_state;
    int retval = 0;

    ns_ipc_buf_unpack(b_in,
                      UP_UINT64(h_dev),
                      UP_UINT32(p_subdevice),
                      UP_END);
    p_dev = (struct nfp_device *) h_dev;

    if (!ipcclient_check_handle(con, (void *)p_dev)) {
        NS_LOG(NS_LOG_LVL_WARN, "%s: got bad handle %p", __func__, p_dev);
        p_dev = NULL;
    }

    ipcclient_trace(con, "nfp_power_get");
    retval = nfp_power_get(p_dev, p_subdevice, p_state);
    err = errno;

    ns_ipc_buf_pack(b_out,
                    PK_INT32(err),
                    PK_INT32(retval),
                    PK_BYTES(p_state, sizeof(_p_state)),
                    PK_END);

}

static void __nfp_power_set(ns_ipc_server_conn_handle_t con,
                            ns_ipc_buf_t *b_out,
                            ns_ipc_buf_t *b_in)
{
    int err = 0;
    struct nfp_device * p_dev;
    uint64_t h_dev;
    uint32_t p_subdevice;
    int32_t p_state;
    int retval = 0;

    ns_ipc_buf_unpack(b_in,
                      UP_UINT64(h_dev),
                      UP_UINT32(p_subdevice),
                      UP_INT32(p_state),
                      UP_END);
    p_dev = (struct nfp_device *) h_dev;

    if (!ipcclient_check_handle(con, (void *)p_dev)) {
        NS_LOG(NS_LOG_LVL_WARN, "%s: got bad handle %p", __func__, p_dev);
        p_dev = NULL;
    }

    ipcclient_trace(con, "nfp_power_set");
    retval = nfp_power_set(p_dev, p_subdevice, p_state);
    err = errno;

    ns_ipc_buf_pack(b_out,
                    PK_INT32(err),
                    PK_INT32(retval),
                    PK_END);

}

static void __nfp_reset_soft(ns_ipc_server_conn_handle_t con,
                             ns_ipc_buf_t *b_out,
                             ns_ipc_buf_t *b_in)
{
    int err = 0;
    struct nfp_device * p_dev;
    uint64_t h_dev;
    int retval = 0;

    ns_ipc_buf_unpack(b_in,
                      UP_UINT64(h_dev),
                      UP_END);
    p_dev = (struct nfp_device *) h_dev;

    if (!ipcclient_check_handle(con, (void *)p_dev)) {
        NS_LOG(NS_LOG_LVL_WARN, "%s: got bad handle %p", __func__, p_dev);
        p_dev = NULL;
    }

    ipcclient_trace(con, "nfp_reset_soft");
    retval = nfp_reset_soft(p_dev);
    err = errno;

    ns_ipc_buf_pack(b_out,
                    PK_INT32(err),
                    PK_INT32(retval),
                    PK_END);

}

static void __nfp_island_powered(ns_ipc_server_conn_handle_t con,
                                 ns_ipc_buf_t *b_out,
                                 ns_ipc_buf_t *b_in)
{
    int err = 0;
    struct nfp_device * p_nfp;
    uint64_t h_nfp;
    int32_t p_iid;
    int retval = 0;

    ns_ipc_buf_unpack(b_in,
                      UP_UINT64(h_nfp),
                      UP_INT32(p_iid),
                      UP_END);
    p_nfp = (struct nfp_device *) h_nfp;

    if (!ipcclient_check_handle(con, (void *)p_nfp)) {
        NS_LOG(NS_LOG_LVL_WARN, "%s: got bad handle %p", __func__, p_nfp);
        p_nfp = NULL;
    }

    ipcclient_trace(con, "nfp_island_powered");
    retval = nfp_island_powered(p_nfp, p_iid);
    err = errno;

    ns_ipc_buf_pack(b_out,
                    PK_INT32(err),
                    PK_INT32(retval),
                    PK_END);

}

static void __nfp_power_rpc_version(ns_ipc_server_conn_handle_t con,
                              ns_ipc_buf_t *b_out,
                              ns_ipc_buf_t *b_in)
{
    ns_ipc_buf_pack(b_out,
                    PK_UINT32(0),
                    PK_UINT32(0),
                    PK_END);
}
struct ipc_server_fmap ipc_nfp_power_fmaps[] = {
        {"nfp_power_get", __nfp_power_get},
        {"nfp_power_set", __nfp_power_set},
        {"nfp_reset_soft", __nfp_reset_soft},
        {"nfp_island_powered", __nfp_island_powered},
        {"nfp_power_rpc_version", __nfp_power_rpc_version},
        {NULL, NULL}};
