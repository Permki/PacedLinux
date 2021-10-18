/* Generated ns_ipc client dddstubs */
#include <errno.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include <nfp-common/nfp_platform.h>
#include <nfp-common/ns_log.h>
#include "ns_ipc_buf.h"
#include "ns_ipc_client.h"

#include "nfp_bulk.h"
#include "nfp_bulk_handles.h"

#ifdef _MSC_VER    /* This block is specific to MS VC++. */
    #include <Windows.h>
    #include <winerror.h>
    #define EPROTO WSAEPROTOTYPE
    #define SET_ERROR(x) do { SetLastError(x); errno = x; } while(0)
#else
    #define SET_ERROR(x) do { errno = x; } while(0)
#endif

int nfp_bulk_mecsr_read(struct nfp_device * dev, uint32_t * mecsrs, uint32_t * values, int len)
{
    ns_ipc_buf_t *b_in = NULL, b_out;
    int _error_code = 0;
    int32_t retval = 0;
    uint32_t * o_values;
    size_t olen_values;
    int32_t inconv_len = len;
    size_t _l_p_mecsrs = sizeof(uint32_t) * len;

    ns_ipc_buf_init(&b_out);
    ns_ipc_buf_pack(&b_out,
                    PK_STR(__func__),
                    PK_UINT64(dev->rh),
                    PK_BYTES(mecsrs, _l_p_mecsrs),
                    PK_INT32(inconv_len),
                    PK_END);
    if (ns_ipc_client_send_receive(dev->ipc_h, &b_in, &b_out) < 0)
        b_in = NULL;
    if (b_in) {
        ns_ipc_buf_unpack(b_in,
                          UP_INT32(_error_code),
                          UP_INT32(retval),
                          UP_BYTES(o_values, olen_values),
                          UP_END);

        if (b_in->error == 0) {
            if (values) {
                memcpy(values, o_values, olen_values);
            }
        } else
            NS_LOG(NS_LOG_LVL_ERROR, "error occurred during buffer unpack for nfp_bulk_mecsr_read");
        ns_ipc_buf_done2(b_in, &b_out);
        free(b_in);
    } else {
        _error_code = EIO;
    }

    SET_ERROR(_error_code);
    return retval;
}

int nfp_bulk_mecsr_write(struct nfp_device * dev, uint32_t * mecsrs, uint32_t * values, int len)
{
    ns_ipc_buf_t *b_in = NULL, b_out;
    int _error_code = 0;
    int32_t retval = 0;
    int32_t inconv_len = len;
    size_t _l_p_mecsrs = sizeof(uint32_t) * len;
    size_t _l_p_values = sizeof(uint32_t) * len;

    ns_ipc_buf_init(&b_out);
    ns_ipc_buf_pack(&b_out,
                    PK_STR(__func__),
                    PK_UINT64(dev->rh),
                    PK_BYTES(mecsrs, _l_p_mecsrs),
                    PK_BYTES(values, _l_p_values),
                    PK_INT32(inconv_len),
                    PK_END);
    if (ns_ipc_client_send_receive(dev->ipc_h, &b_in, &b_out) < 0)
        b_in = NULL;
    if (b_in) {
        ns_ipc_buf_unpack(b_in,
                          UP_INT32(_error_code),
                          UP_INT32(retval),
                          UP_END);

        ns_ipc_buf_done2(b_in, &b_out);
        free(b_in);
    } else {
        _error_code = EIO;
    }

    SET_ERROR(_error_code);
    return retval;
}

int nfp_bulk_me_register_read(struct nfp_device * dev, unsigned int meid, unsigned int reg, uint32_t * values, int count)
{
    ns_ipc_buf_t *b_in = NULL, b_out;
    int _error_code = 0;
    int32_t retval = 0;
    uint32_t * o_values;
    size_t olen_values;
    uint32_t inconv_meid = meid;
    uint32_t inconv_reg = reg;
    int32_t inconv_count = count;

    ns_ipc_buf_init(&b_out);
    ns_ipc_buf_pack(&b_out,
                    PK_STR(__func__),
                    PK_UINT64(dev->rh),
                    PK_UINT32(inconv_meid),
                    PK_UINT32(inconv_reg),
                    PK_INT32(inconv_count),
                    PK_END);
    if (ns_ipc_client_send_receive(dev->ipc_h, &b_in, &b_out) < 0)
        b_in = NULL;
    if (b_in) {
        ns_ipc_buf_unpack(b_in,
                          UP_INT32(_error_code),
                          UP_INT32(retval),
                          UP_BYTES(o_values, olen_values),
                          UP_END);

        if (b_in->error == 0) {
            if (values) {
                memcpy(values, o_values, olen_values);
            }
        } else
            NS_LOG(NS_LOG_LVL_ERROR, "error occurred during buffer unpack for nfp_bulk_me_register_read");
        ns_ipc_buf_done2(b_in, &b_out);
        free(b_in);
    } else {
        _error_code = EIO;
    }

    SET_ERROR(_error_code);
    return retval;
}

int nfp_bulk_me_register_write(struct nfp_device * dev, unsigned int meid, unsigned int reg, uint32_t * values, int count)
{
    ns_ipc_buf_t *b_in = NULL, b_out;
    int _error_code = 0;
    int32_t retval = 0;
    uint32_t inconv_meid = meid;
    uint32_t inconv_reg = reg;
    int32_t inconv_count = count;
    size_t _l_p_values = sizeof(uint32_t) * count;

    ns_ipc_buf_init(&b_out);
    ns_ipc_buf_pack(&b_out,
                    PK_STR(__func__),
                    PK_UINT64(dev->rh),
                    PK_UINT32(inconv_meid),
                    PK_UINT32(inconv_reg),
                    PK_BYTES(values, _l_p_values),
                    PK_INT32(inconv_count),
                    PK_END);
    if (ns_ipc_client_send_receive(dev->ipc_h, &b_in, &b_out) < 0)
        b_in = NULL;
    if (b_in) {
        ns_ipc_buf_unpack(b_in,
                          UP_INT32(_error_code),
                          UP_INT32(retval),
                          UP_END);

        ns_ipc_buf_done2(b_in, &b_out);
        free(b_in);
    } else {
        _error_code = EIO;
    }

    SET_ERROR(_error_code);
    return retval;
}

int nfp_bulk_xpb_read(struct nfp_cpp * cpp, uint32_t * xpb_tgts, uint32_t * values, int len)
{
    ns_ipc_buf_t *b_in = NULL, b_out;
    int _error_code = 0;
    int32_t retval = 0;
    uint32_t * o_values;
    size_t olen_values;
    int32_t inconv_len = len;
    size_t _l_p_xpb_tgts = sizeof(uint32_t) * len;

    ns_ipc_buf_init(&b_out);
    ns_ipc_buf_pack(&b_out,
                    PK_STR(__func__),
                    PK_UINT64(cpp->rh),
                    PK_BYTES(xpb_tgts, _l_p_xpb_tgts),
                    PK_INT32(inconv_len),
                    PK_END);
    if (ns_ipc_client_send_receive(cpp->ipc_h, &b_in, &b_out) < 0)
        b_in = NULL;
    if (b_in) {
        ns_ipc_buf_unpack(b_in,
                          UP_INT32(_error_code),
                          UP_INT32(retval),
                          UP_BYTES(o_values, olen_values),
                          UP_END);

        if (b_in->error == 0) {
            if (values) {
                memcpy(values, o_values, olen_values);
            }
        } else
            NS_LOG(NS_LOG_LVL_ERROR, "error occurred during buffer unpack for nfp_bulk_xpb_read");
        ns_ipc_buf_done2(b_in, &b_out);
        free(b_in);
    } else {
        _error_code = EIO;
    }

    SET_ERROR(_error_code);
    return retval;
}

int nfp_bulk_xpb_write(struct nfp_cpp * cpp, uint32_t * xpb_tgts, uint32_t * values, int len)
{
    ns_ipc_buf_t *b_in = NULL, b_out;
    int _error_code = 0;
    int32_t retval = 0;
    int32_t inconv_len = len;
    size_t _l_p_xpb_tgts = sizeof(uint32_t) * len;
    size_t _l_p_values = sizeof(uint32_t) * len;

    ns_ipc_buf_init(&b_out);
    ns_ipc_buf_pack(&b_out,
                    PK_STR(__func__),
                    PK_UINT64(cpp->rh),
                    PK_BYTES(xpb_tgts, _l_p_xpb_tgts),
                    PK_BYTES(values, _l_p_values),
                    PK_INT32(inconv_len),
                    PK_END);
    if (ns_ipc_client_send_receive(cpp->ipc_h, &b_in, &b_out) < 0)
        b_in = NULL;
    if (b_in) {
        ns_ipc_buf_unpack(b_in,
                          UP_INT32(_error_code),
                          UP_INT32(retval),
                          UP_END);

        ns_ipc_buf_done2(b_in, &b_out);
        free(b_in);
    } else {
        _error_code = EIO;
    }

    SET_ERROR(_error_code);
    return retval;
}

int nfp_bulk_rpc_version(int *cln_ver, int *cln_compat, int *srv_ver, int *srv_compat)
{
    char *url;
    ns_ipc_buf_t *b_in = NULL, b_out;
    struct ns_ipc_client_handle ipc_h;
    uint32_t sver = 0, scmp = 0;
    int ret = 0;

    url = nfp_get_server_url(-1);
    ret = ns_ipc_client_init(&ipc_h, url);
    if (url)
      free(url);
    if (ret) {
        NS_LOG(NS_LOG_LVL_ERROR, "failed to create IPC server connection");
        return -1;
    }
    ns_ipc_buf_init(&b_out);
    ns_ipc_buf_pack(&b_out,
                    PK_STR(__func__),
                    PK_END);
    if (ns_ipc_client_send_receive(&ipc_h, &b_in, &b_out) < 0)
        b_in = NULL;
    ns_ipc_client_close(&ipc_h);
    if (b_in) {
        ns_ipc_buf_unpack(b_in,
                          UP_UINT32(sver),
                          UP_UINT32(scmp),
                          UP_END);
        ns_ipc_buf_done2(b_in, &b_out);
        free(b_in);
    } else {
        NS_LOG(NS_LOG_LVL_ERROR, "failed to read from IPC server");
        ns_ipc_buf_done(&b_out);
        return -1;
    }
    *srv_ver = sver;
    *srv_compat = scmp;
    *cln_ver = 1;
    *cln_compat = 1;
    if (*cln_compat) {
        if (*cln_ver > *srv_ver)
            return 1;
        if (*srv_ver < *cln_compat)
            return 1;
    } else if (*srv_ver != *cln_ver)
        return 1;
    return 0;
}
