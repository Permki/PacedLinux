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

#include "nfp_nffw.h"
#include "nfp_nffw_handles.h"

#ifdef _MSC_VER    /* This block is specific to MS VC++. */
    #include <Windows.h>
    #include <winerror.h>
    #define EPROTO WSAEPROTOTYPE
    #define SET_ERROR(x) do { SetLastError(x); errno = x; } while(0)
#else
    #define SET_ERROR(x) do { errno = x; } while(0)
#endif

int nfp_nffw_load(struct nfp_device * nfp, const void * data, size_t size, uint8_t * fwid)
{
    ns_ipc_buf_t *b_in = NULL, b_out;
    int _error_code = 0;
    int32_t retval = 0;
    void * o_fwid;
    size_t olen_fwid = sizeof(uint8_t) * 1;
    int64_t inconv_size = size;

    ns_ipc_buf_init(&b_out);
    ns_ipc_buf_pack(&b_out,
                    PK_STR(__func__),
                    PK_UINT64(nfp->rh),
                    PK_BYTES(data, size),
                    PK_INT64(inconv_size),
                    PK_END);
    if (ns_ipc_client_send_receive(nfp->ipc_h, &b_in, &b_out) < 0)
        b_in = NULL;
    if (b_in) {
        ns_ipc_buf_unpack(b_in,
                          UP_INT32(_error_code),
                          UP_INT32(retval),
                          UP_BYTES(o_fwid, olen_fwid),
                          UP_END);

        if (b_in->error == 0) {
            if (fwid) {
                memcpy(fwid, o_fwid, olen_fwid);
            }
        } else
            NS_LOG(NS_LOG_LVL_ERROR, "error occurred during buffer unpack for nfp_nffw_load");
        ns_ipc_buf_done2(b_in, &b_out);
        free(b_in);
    } else {
        _error_code = EIO;
    }

    SET_ERROR(_error_code);
    return retval;
}

int nfp_nffw_load_mutable(struct nfp_device * nfp, void * data, size_t size, uint8_t * fwid)
{
    ns_ipc_buf_t *b_in = NULL, b_out;
    int _error_code = 0;
    int32_t retval = 0;
    void * o_fwid;
    size_t olen_fwid = sizeof(uint8_t) * 1;
    int64_t inconv_size = size;

    ns_ipc_buf_init(&b_out);
    ns_ipc_buf_pack(&b_out,
                    PK_STR(__func__),
                    PK_UINT64(nfp->rh),
                    PK_BYTES(data, size),
                    PK_INT64(inconv_size),
                    PK_END);
    if (ns_ipc_client_send_receive(nfp->ipc_h, &b_in, &b_out) < 0)
        b_in = NULL;
    if (b_in) {
        ns_ipc_buf_unpack(b_in,
                          UP_INT32(_error_code),
                          UP_INT32(retval),
                          UP_BYTES(o_fwid, olen_fwid),
                          UP_END);

        if (b_in->error == 0) {
            if (fwid) {
                memcpy(fwid, o_fwid, olen_fwid);
            }
        } else
            NS_LOG(NS_LOG_LVL_ERROR, "error occurred during buffer unpack for nfp_nffw_load_mutable");
        ns_ipc_buf_done2(b_in, &b_out);
        free(b_in);
    } else {
        _error_code = EIO;
    }

    SET_ERROR(_error_code);
    return retval;
}

int nfp_nffw_start(struct nfp_device * nfp, uint8_t fwid)
{
    ns_ipc_buf_t *b_in = NULL, b_out;
    int _error_code = 0;
    int32_t retval = 0;
    uint32_t inconv_fwid = fwid;

    ns_ipc_buf_init(&b_out);
    ns_ipc_buf_pack(&b_out,
                    PK_STR(__func__),
                    PK_UINT64(nfp->rh),
                    PK_UINT32(inconv_fwid),
                    PK_END);
    if (ns_ipc_client_send_receive(nfp->ipc_h, &b_in, &b_out) < 0)
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

int nfp_nffw_unload(struct nfp_device * nfp, uint8_t fwid)
{
    ns_ipc_buf_t *b_in = NULL, b_out;
    int _error_code = 0;
    int32_t retval = 0;
    uint32_t inconv_fwid = fwid;

    ns_ipc_buf_init(&b_out);
    ns_ipc_buf_pack(&b_out,
                    PK_STR(__func__),
                    PK_UINT64(nfp->rh),
                    PK_UINT32(inconv_fwid),
                    PK_END);
    if (ns_ipc_client_send_receive(nfp->ipc_h, &b_in, &b_out) < 0)
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

int nfp_nffw_info_acquire(struct nfp_device * nfp)
{
    ns_ipc_buf_t *b_in = NULL, b_out;
    int _error_code = 0;
    int32_t retval = 0;

    ns_ipc_buf_init(&b_out);
    ns_ipc_buf_pack(&b_out,
                    PK_STR(__func__),
                    PK_UINT64(nfp->rh),
                    PK_END);
    if (ns_ipc_client_send_receive(nfp->ipc_h, &b_in, &b_out) < 0)
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

int nfp_nffw_info_release(struct nfp_device * nfp)
{
    ns_ipc_buf_t *b_in = NULL, b_out;
    int _error_code = 0;
    int32_t retval = 0;

    ns_ipc_buf_init(&b_out);
    ns_ipc_buf_pack(&b_out,
                    PK_STR(__func__),
                    PK_UINT64(nfp->rh),
                    PK_END);
    if (ns_ipc_client_send_receive(nfp->ipc_h, &b_in, &b_out) < 0)
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

int nfp_nffw_info_fw_loaded(struct nfp_device * nfp)
{
    ns_ipc_buf_t *b_in = NULL, b_out;
    int _error_code = 0;
    int32_t retval = 0;

    ns_ipc_buf_init(&b_out);
    ns_ipc_buf_pack(&b_out,
                    PK_STR(__func__),
                    PK_UINT64(nfp->rh),
                    PK_END);
    if (ns_ipc_client_send_receive(nfp->ipc_h, &b_in, &b_out) < 0)
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

int nfp_nffw_info_fw_debugger_attached(struct nfp_device * nfp)
{
    ns_ipc_buf_t *b_in = NULL, b_out;
    int _error_code = 0;
    int32_t retval = 0;

    ns_ipc_buf_init(&b_out);
    ns_ipc_buf_pack(&b_out,
                    PK_STR(__func__),
                    PK_UINT64(nfp->rh),
                    PK_END);
    if (ns_ipc_client_send_receive(nfp->ipc_h, &b_in, &b_out) < 0)
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

int nfp_nffw_info_fw_modular(struct nfp_device * nfp)
{
    ns_ipc_buf_t *b_in = NULL, b_out;
    int _error_code = 0;
    int32_t retval = 0;

    ns_ipc_buf_init(&b_out);
    ns_ipc_buf_pack(&b_out,
                    PK_STR(__func__),
                    PK_UINT64(nfp->rh),
                    PK_END);
    if (ns_ipc_client_send_receive(nfp->ipc_h, &b_in, &b_out) < 0)
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

uint8_t nfp_nffw_info_me_ctxmask(struct nfp_device * nfp, int meid)
{
    ns_ipc_buf_t *b_in = NULL, b_out;
    int _error_code = 0;
    uint32_t retval = 0;
    int32_t inconv_meid = meid;

    ns_ipc_buf_init(&b_out);
    ns_ipc_buf_pack(&b_out,
                    PK_STR(__func__),
                    PK_UINT64(nfp->rh),
                    PK_INT32(inconv_meid),
                    PK_END);
    if (ns_ipc_client_send_receive(nfp->ipc_h, &b_in, &b_out) < 0)
        b_in = NULL;
    if (b_in) {
        ns_ipc_buf_unpack(b_in,
                          UP_INT32(_error_code),
                          UP_UINT32(retval),
                          UP_END);

        ns_ipc_buf_done2(b_in, &b_out);
        free(b_in);
    } else {
        _error_code = EIO;
    }

    SET_ERROR(_error_code);
    return retval;
}

int nfp_nffw_info_me_ctxmask_set(struct nfp_device * nfp, int meid, uint8_t ctxmask)
{
    ns_ipc_buf_t *b_in = NULL, b_out;
    int _error_code = 0;
    int32_t retval = 0;
    int32_t inconv_meid = meid;
    uint32_t inconv_ctxmask = ctxmask;

    ns_ipc_buf_init(&b_out);
    ns_ipc_buf_pack(&b_out,
                    PK_STR(__func__),
                    PK_UINT64(nfp->rh),
                    PK_INT32(inconv_meid),
                    PK_UINT32(inconv_ctxmask),
                    PK_END);
    if (ns_ipc_client_send_receive(nfp->ipc_h, &b_in, &b_out) < 0)
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

uint8_t nfp_nffw_info_me_fwid(struct nfp_device * nfp, int meid)
{
    ns_ipc_buf_t *b_in = NULL, b_out;
    int _error_code = 0;
    uint32_t retval = 0;
    int32_t inconv_meid = meid;

    ns_ipc_buf_init(&b_out);
    ns_ipc_buf_pack(&b_out,
                    PK_STR(__func__),
                    PK_UINT64(nfp->rh),
                    PK_INT32(inconv_meid),
                    PK_END);
    if (ns_ipc_client_send_receive(nfp->ipc_h, &b_in, &b_out) < 0)
        b_in = NULL;
    if (b_in) {
        ns_ipc_buf_unpack(b_in,
                          UP_INT32(_error_code),
                          UP_UINT32(retval),
                          UP_END);

        ns_ipc_buf_done2(b_in, &b_out);
        free(b_in);
    } else {
        _error_code = EIO;
    }

    SET_ERROR(_error_code);
    return retval;
}

int nfp_nffw_info_me_fwid_set(struct nfp_device * nfp, int meid, uint8_t fwid)
{
    ns_ipc_buf_t *b_in = NULL, b_out;
    int _error_code = 0;
    int32_t retval = 0;
    int32_t inconv_meid = meid;
    uint32_t inconv_fwid = fwid;

    ns_ipc_buf_init(&b_out);
    ns_ipc_buf_pack(&b_out,
                    PK_STR(__func__),
                    PK_UINT64(nfp->rh),
                    PK_INT32(inconv_meid),
                    PK_UINT32(inconv_fwid),
                    PK_END);
    if (ns_ipc_client_send_receive(nfp->ipc_h, &b_in, &b_out) < 0)
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

uint8_t nfp_nffw_info_fwid_first(struct nfp_device * nfp)
{
    ns_ipc_buf_t *b_in = NULL, b_out;
    int _error_code = 0;
    uint32_t retval = 0;

    ns_ipc_buf_init(&b_out);
    ns_ipc_buf_pack(&b_out,
                    PK_STR(__func__),
                    PK_UINT64(nfp->rh),
                    PK_END);
    if (ns_ipc_client_send_receive(nfp->ipc_h, &b_in, &b_out) < 0)
        b_in = NULL;
    if (b_in) {
        ns_ipc_buf_unpack(b_in,
                          UP_INT32(_error_code),
                          UP_UINT32(retval),
                          UP_END);

        ns_ipc_buf_done2(b_in, &b_out);
        free(b_in);
    } else {
        _error_code = EIO;
    }

    SET_ERROR(_error_code);
    return retval;
}

uint8_t nfp_nffw_info_fwid_next(struct nfp_device * nfp, uint8_t fwid)
{
    ns_ipc_buf_t *b_in = NULL, b_out;
    int _error_code = 0;
    uint32_t retval = 0;
    uint32_t inconv_fwid = fwid;

    ns_ipc_buf_init(&b_out);
    ns_ipc_buf_pack(&b_out,
                    PK_STR(__func__),
                    PK_UINT64(nfp->rh),
                    PK_UINT32(inconv_fwid),
                    PK_END);
    if (ns_ipc_client_send_receive(nfp->ipc_h, &b_in, &b_out) < 0)
        b_in = NULL;
    if (b_in) {
        ns_ipc_buf_unpack(b_in,
                          UP_INT32(_error_code),
                          UP_UINT32(retval),
                          UP_END);

        ns_ipc_buf_done2(b_in, &b_out);
        free(b_in);
    } else {
        _error_code = EIO;
    }

    SET_ERROR(_error_code);
    return retval;
}

int nfp_nffw_info_fw_mip(struct nfp_device * nfp, uint8_t fwid, uint32_t * cpp_id, uint64_t * off)
{
    ns_ipc_buf_t *b_in = NULL, b_out;
    int _error_code = 0;
    int32_t retval = 0;
    void * o_cpp_id;
    size_t olen_cpp_id = sizeof(uint32_t) * 1;
    void * o_off;
    size_t olen_off = sizeof(uint64_t) * 1;
    uint32_t inconv_fwid = fwid;

    ns_ipc_buf_init(&b_out);
    ns_ipc_buf_pack(&b_out,
                    PK_STR(__func__),
                    PK_UINT64(nfp->rh),
                    PK_UINT32(inconv_fwid),
                    PK_END);
    if (ns_ipc_client_send_receive(nfp->ipc_h, &b_in, &b_out) < 0)
        b_in = NULL;
    if (b_in) {
        ns_ipc_buf_unpack(b_in,
                          UP_INT32(_error_code),
                          UP_INT32(retval),
                          UP_BYTES(o_cpp_id, olen_cpp_id),
                          UP_BYTES(o_off, olen_off),
                          UP_END);

        if (b_in->error == 0) {
            if (cpp_id) {
                memcpy(cpp_id, o_cpp_id, olen_cpp_id);
            }
            if (off) {
                memcpy(off, o_off, olen_off);
            }
        } else
            NS_LOG(NS_LOG_LVL_ERROR, "error occurred during buffer unpack for nfp_nffw_info_fw_mip");
        ns_ipc_buf_done2(b_in, &b_out);
        free(b_in);
    } else {
        _error_code = EIO;
    }

    SET_ERROR(_error_code);
    return retval;
}

int nfp_nffw_rpc_version(int *cln_ver, int *cln_compat, int *srv_ver, int *srv_compat)
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
