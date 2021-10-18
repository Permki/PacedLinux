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

#include "nfp_hwdbg.h"
#include "nfp_hwdbg_handles.h"

#ifdef _MSC_VER    /* This block is specific to MS VC++. */
    #include <Windows.h>
    #include <winerror.h>
    #define EPROTO WSAEPROTOTYPE
    #define SET_ERROR(x) do { SetLastError(x); errno = x; } while(0)
#else
    #define SET_ERROR(x) do { errno = x; } while(0)
#endif

struct nfp_hwdbg * nfp_hwdbg_open(unsigned int devnum)
{
    ns_ipc_buf_t *b_in = NULL, b_out;
    int _error_code = 0;
    int ret = 0;
    struct nfp_hwdbg * retval = NULL;
    uint64_t rh = 0;
    uint32_t _api_version = 0;
    uint32_t inconv_devnum = devnum;

    retval = malloc(sizeof(*retval));
    if (!retval) {
        SET_ERROR(ENOMEM);
        return NULL;
    }
    {
        char *url = nfp_get_server_url(devnum);
        retval->id = devnum;
        handle_common_init(&retval->h_common);
        ret = ns_ipc_client_init(&retval->ipc_handle, url);
        retval->ipc_h = &retval->ipc_handle;
        if (url)
            free(url);
    }
    if (ret) {
        free(retval);
        NS_LOG(NS_LOG_LVL_ERROR, "failed to open IPC client");
        SET_ERROR(ENXIO);
        return NULL;
    }
    retval->own_handle = 1;

    ns_ipc_buf_init(&b_out);
    ns_ipc_buf_pack(&b_out,
                    PK_STR(__func__),
                    PK_UINT32(2), /* api version */
                    PK_UINT32(inconv_devnum),
                    PK_END);
    if (ns_ipc_client_send_receive(retval->ipc_h, &b_in, &b_out) < 0)
        b_in = NULL;
    if (b_in) {
        ns_ipc_buf_unpack(b_in,
                          UP_UINT32(_api_version),
                          UP_INT32(_error_code),
                          UP_UINT64(rh),
                          UP_END);

        ns_ipc_buf_done2(b_in, &b_out);
        free(b_in);
    } else {
        _error_code = EIO;
    }

    if (rh && nfp_client_check_version("nfp_hwdbg.h", 2, 2, _api_version) < 0) {
        rh = (uint64_t)NULL; /* treat like remote alloc failure */
        _error_code = EPROTO;
    }
    if (!rh) {
        if (retval->own_handle)
            ns_ipc_client_close(retval->ipc_h);
        free(retval);
        retval = NULL;
        NS_LOG(NS_LOG_LVL_ERROR, "remote handle creation failed");
    } else {
        nfp_hwdbg_open_local(
                retval,
                devnum
                ); /* do local handle init at last minute */
        retval->rh = rh;
    }
    SET_ERROR(_error_code);
    return retval;
}

void nfp_hwdbg_close(struct nfp_hwdbg * hwdbg)
{
    ns_ipc_buf_t *b_in = NULL, b_out;
    int _error_code = 0;

    ns_ipc_buf_init(&b_out);
    ns_ipc_buf_pack(&b_out,
                    PK_STR(__func__),
                    PK_UINT64(hwdbg->rh),
                    PK_END);
    if (ns_ipc_client_send_receive(hwdbg->ipc_h, &b_in, &b_out) < 0)
        b_in = NULL;
    if (b_in) {
        ns_ipc_buf_unpack(b_in,
                          UP_INT32(_error_code),
                          UP_END);

        ns_ipc_buf_done2(b_in, &b_out);
        free(b_in);
    } else {
        _error_code = EIO;
    }

    if (hwdbg->own_handle)
        ns_ipc_client_close(hwdbg->ipc_h);
    nfp_hwdbg_close_local(hwdbg); /* do local handle free */
    free(hwdbg);
    SET_ERROR(_error_code);
}

int nfp_hwdbg_client_count(struct nfp_hwdbg * hwdbg)
{
    ns_ipc_buf_t *b_in = NULL, b_out;
    int _error_code = 0;
    int32_t retval = 0;

    ns_ipc_buf_init(&b_out);
    ns_ipc_buf_pack(&b_out,
                    PK_STR(__func__),
                    PK_UINT64(hwdbg->rh),
                    PK_END);
    if (ns_ipc_client_send_receive(hwdbg->ipc_h, &b_in, &b_out) < 0)
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

void nfp_hwdbg_srv_reset(struct nfp_hwdbg * hwdbg, int resettype)
{
    ns_ipc_buf_t *b_in = NULL, b_out;
    int _error_code = 0;
    int32_t inconv_resettype = resettype;

    ns_ipc_buf_init(&b_out);
    ns_ipc_buf_pack(&b_out,
                    PK_STR(__func__),
                    PK_UINT64(hwdbg->rh),
                    PK_INT32(inconv_resettype),
                    PK_END);
    if (ns_ipc_client_send_receive(hwdbg->ipc_h, &b_in, &b_out) < 0)
        b_in = NULL;
    if (b_in) {
        ns_ipc_buf_unpack(b_in,
                          UP_INT32(_error_code),
                          UP_END);

        ns_ipc_buf_done2(b_in, &b_out);
        free(b_in);
    } else {
        _error_code = EIO;
    }

    SET_ERROR(_error_code);
}

int nfp_hwdbg_set_enabled_mes(struct nfp_hwdbg * hwdbg, int * meids, int len)
{
    ns_ipc_buf_t *b_in = NULL, b_out;
    int _error_code = 0;
    int32_t retval = 0;
    int32_t inconv_len = len;
    size_t _l_p_meids = sizeof(int) * len;

    ns_ipc_buf_init(&b_out);
    ns_ipc_buf_pack(&b_out,
                    PK_STR(__func__),
                    PK_UINT64(hwdbg->rh),
                    PK_BYTES(meids, _l_p_meids),
                    PK_INT32(inconv_len),
                    PK_END);
    if (ns_ipc_client_send_receive(hwdbg->ipc_h, &b_in, &b_out) < 0)
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

int nfp_hwdbg_me_status(struct nfp_hwdbg * hwdbg, int * meids, struct nfp_hwdbg_me_status_data * status, int len)
{
    ns_ipc_buf_t *b_in = NULL, b_out;
    int _error_code = 0;
    int32_t retval = 0;
    struct nfp_hwdbg_me_status_data * o_status;
    size_t olen_status;
    int32_t inconv_len = len;
    size_t _l_p_meids = sizeof(int) * len;

    ns_ipc_buf_init(&b_out);
    ns_ipc_buf_pack(&b_out,
                    PK_STR(__func__),
                    PK_UINT64(hwdbg->rh),
                    PK_BYTES(meids, _l_p_meids),
                    PK_INT32(inconv_len),
                    PK_END);
    if (ns_ipc_client_send_receive(hwdbg->ipc_h, &b_in, &b_out) < 0)
        b_in = NULL;
    if (b_in) {
        ns_ipc_buf_unpack(b_in,
                          UP_INT32(_error_code),
                          UP_INT32(retval),
                          UP_BYTES(o_status, olen_status),
                          UP_END);

        if (b_in->error == 0) {
            if (status) {
                memcpy(status, o_status, olen_status);
            }
        } else
            NS_LOG(NS_LOG_LVL_ERROR, "error occurred during buffer unpack for nfp_hwdbg_me_status");
        ns_ipc_buf_done2(b_in, &b_out);
        free(b_in);
    } else {
        _error_code = EIO;
    }

    SET_ERROR(_error_code);
    return retval;
}

int nfp_hwdbg_me_stop(struct nfp_hwdbg * hwdbg, int meid)
{
    ns_ipc_buf_t *b_in = NULL, b_out;
    int _error_code = 0;
    int32_t retval = 0;
    int32_t inconv_meid = meid;

    ns_ipc_buf_init(&b_out);
    ns_ipc_buf_pack(&b_out,
                    PK_STR(__func__),
                    PK_UINT64(hwdbg->rh),
                    PK_INT32(inconv_meid),
                    PK_END);
    if (ns_ipc_client_send_receive(hwdbg->ipc_h, &b_in, &b_out) < 0)
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

int nfp_hwdbg_me_run(struct nfp_hwdbg * hwdbg, int meid, int resume)
{
    ns_ipc_buf_t *b_in = NULL, b_out;
    int _error_code = 0;
    int32_t retval = 0;
    int32_t inconv_meid = meid;
    int32_t inconv_resume = resume;

    ns_ipc_buf_init(&b_out);
    ns_ipc_buf_pack(&b_out,
                    PK_STR(__func__),
                    PK_UINT64(hwdbg->rh),
                    PK_INT32(inconv_meid),
                    PK_INT32(inconv_resume),
                    PK_END);
    if (ns_ipc_client_send_receive(hwdbg->ipc_h, &b_in, &b_out) < 0)
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

int nfp_hwdbg_me_step(struct nfp_hwdbg * hwdbg, int meid, int ctx)
{
    ns_ipc_buf_t *b_in = NULL, b_out;
    int _error_code = 0;
    int32_t retval = 0;
    int32_t inconv_meid = meid;
    int32_t inconv_ctx = ctx;

    ns_ipc_buf_init(&b_out);
    ns_ipc_buf_pack(&b_out,
                    PK_STR(__func__),
                    PK_UINT64(hwdbg->rh),
                    PK_INT32(inconv_meid),
                    PK_INT32(inconv_ctx),
                    PK_END);
    if (ns_ipc_client_send_receive(hwdbg->ipc_h, &b_in, &b_out) < 0)
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

int nfp_hwdbg_brkgrp_add(struct nfp_hwdbg * hwdbg, int meid, int * breakmeids, int len)
{
    ns_ipc_buf_t *b_in = NULL, b_out;
    int _error_code = 0;
    int32_t retval = 0;
    int32_t inconv_meid = meid;
    int32_t inconv_len = len;
    size_t _l_p_breakmeids = sizeof(int) * len;

    ns_ipc_buf_init(&b_out);
    ns_ipc_buf_pack(&b_out,
                    PK_STR(__func__),
                    PK_UINT64(hwdbg->rh),
                    PK_INT32(inconv_meid),
                    PK_BYTES(breakmeids, _l_p_breakmeids),
                    PK_INT32(inconv_len),
                    PK_END);
    if (ns_ipc_client_send_receive(hwdbg->ipc_h, &b_in, &b_out) < 0)
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

int nfp_hwdbg_brkgrp_del(struct nfp_hwdbg * hwdbg, int meid)
{
    ns_ipc_buf_t *b_in = NULL, b_out;
    int _error_code = 0;
    int32_t retval = 0;
    int32_t inconv_meid = meid;

    ns_ipc_buf_init(&b_out);
    ns_ipc_buf_pack(&b_out,
                    PK_STR(__func__),
                    PK_UINT64(hwdbg->rh),
                    PK_INT32(inconv_meid),
                    PK_END);
    if (ns_ipc_client_send_receive(hwdbg->ipc_h, &b_in, &b_out) < 0)
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

int nfp_hwdbg_brk_add_code(struct nfp_hwdbg * hwdbg, int64_t breakid, int meid, int pc, unsigned int ctxmask)
{
    ns_ipc_buf_t *b_in = NULL, b_out;
    int _error_code = 0;
    int32_t retval = 0;
    int32_t inconv_meid = meid;
    int32_t inconv_pc = pc;
    uint32_t inconv_ctxmask = ctxmask;

    ns_ipc_buf_init(&b_out);
    ns_ipc_buf_pack(&b_out,
                    PK_STR(__func__),
                    PK_UINT64(hwdbg->rh),
                    PK_INT64(breakid),
                    PK_INT32(inconv_meid),
                    PK_INT32(inconv_pc),
                    PK_UINT32(inconv_ctxmask),
                    PK_END);
    if (ns_ipc_client_send_receive(hwdbg->ipc_h, &b_in, &b_out) < 0)
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

int nfp_hwdbg_brk_del(struct nfp_hwdbg * hwdbg, int64_t breakid)
{
    ns_ipc_buf_t *b_in = NULL, b_out;
    int _error_code = 0;
    int32_t retval = 0;

    ns_ipc_buf_init(&b_out);
    ns_ipc_buf_pack(&b_out,
                    PK_STR(__func__),
                    PK_UINT64(hwdbg->rh),
                    PK_INT64(breakid),
                    PK_END);
    if (ns_ipc_client_send_receive(hwdbg->ipc_h, &b_in, &b_out) < 0)
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

int nfp_hwdbg_brk_flush(struct nfp_hwdbg * hwdbg, int flushtype)
{
    ns_ipc_buf_t *b_in = NULL, b_out;
    int _error_code = 0;
    int32_t retval = 0;
    int32_t inconv_flushtype = flushtype;

    ns_ipc_buf_init(&b_out);
    ns_ipc_buf_pack(&b_out,
                    PK_STR(__func__),
                    PK_UINT64(hwdbg->rh),
                    PK_INT32(inconv_flushtype),
                    PK_END);
    if (ns_ipc_client_send_receive(hwdbg->ipc_h, &b_in, &b_out) < 0)
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

int nfp_hwdbg_brk_check(struct nfp_hwdbg * hwdbg, struct nfp_hwdbg_break_data * data, int len, int * cnt)
{
    ns_ipc_buf_t *b_in = NULL, b_out;
    int _error_code = 0;
    int32_t retval = 0;
    struct nfp_hwdbg_break_data * o_data;
    size_t olen_data;
    void * o_cnt;
    size_t olen_cnt = sizeof(int) * 1;
    int32_t inconv_len = len;

    ns_ipc_buf_init(&b_out);
    ns_ipc_buf_pack(&b_out,
                    PK_STR(__func__),
                    PK_UINT64(hwdbg->rh),
                    PK_INT32(inconv_len),
                    PK_END);
    if (ns_ipc_client_send_receive(hwdbg->ipc_h, &b_in, &b_out) < 0)
        b_in = NULL;
    if (b_in) {
        ns_ipc_buf_unpack(b_in,
                          UP_INT32(_error_code),
                          UP_INT32(retval),
                          UP_BYTES(o_data, olen_data),
                          UP_BYTES(o_cnt, olen_cnt),
                          UP_END);

        if (b_in->error == 0) {
            if (data) {
                memcpy(data, o_data, olen_data);
            }
            if (cnt) {
                memcpy(cnt, o_cnt, olen_cnt);
            }
        } else
            NS_LOG(NS_LOG_LVL_ERROR, "error occurred during buffer unpack for nfp_hwdbg_brk_check");
        ns_ipc_buf_done2(b_in, &b_out);
        free(b_in);
    } else {
        _error_code = EIO;
    }

    SET_ERROR(_error_code);
    return retval;
}

int nfp_hwdbg_brk_clear(struct nfp_hwdbg * hwdbg, int64_t breakid)
{
    ns_ipc_buf_t *b_in = NULL, b_out;
    int _error_code = 0;
    int32_t retval = 0;

    ns_ipc_buf_init(&b_out);
    ns_ipc_buf_pack(&b_out,
                    PK_STR(__func__),
                    PK_UINT64(hwdbg->rh),
                    PK_INT64(breakid),
                    PK_END);
    if (ns_ipc_client_send_receive(hwdbg->ipc_h, &b_in, &b_out) < 0)
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

int nfp_hwdbg_load_dbginfo(struct nfp_hwdbg * hwdbg, int meid, uint64_t * data, int len, int off)
{
    ns_ipc_buf_t *b_in = NULL, b_out;
    int _error_code = 0;
    int32_t retval = 0;
    int32_t inconv_meid = meid;
    int32_t inconv_len = len;
    int32_t inconv_off = off;
    size_t _l_p_data = sizeof(uint64_t) * len;

    ns_ipc_buf_init(&b_out);
    ns_ipc_buf_pack(&b_out,
                    PK_STR(__func__),
                    PK_UINT64(hwdbg->rh),
                    PK_INT32(inconv_meid),
                    PK_BYTES(data, _l_p_data),
                    PK_INT32(inconv_len),
                    PK_INT32(inconv_off),
                    PK_END);
    if (ns_ipc_client_send_receive(hwdbg->ipc_h, &b_in, &b_out) < 0)
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

int nfp_hwdbg_server_version(struct nfp_hwdbg * hwdbg, char * buf, int len)
{
    ns_ipc_buf_t *b_in = NULL, b_out;
    int _error_code = 0;
    int32_t retval = 0;
    char * o_buf;
    size_t olen_buf;
    int32_t inconv_len = len;

    ns_ipc_buf_init(&b_out);
    ns_ipc_buf_pack(&b_out,
                    PK_STR(__func__),
                    PK_UINT64(hwdbg->rh),
                    PK_INT32(inconv_len),
                    PK_END);
    if (ns_ipc_client_send_receive(hwdbg->ipc_h, &b_in, &b_out) < 0)
        b_in = NULL;
    if (b_in) {
        ns_ipc_buf_unpack(b_in,
                          UP_INT32(_error_code),
                          UP_INT32(retval),
                          UP_BYTES(o_buf, olen_buf),
                          UP_END);

        if (b_in->error == 0) {
            if (buf) {
                memcpy(buf, o_buf, olen_buf);
            }
        } else
            NS_LOG(NS_LOG_LVL_ERROR, "error occurred during buffer unpack for nfp_hwdbg_server_version");
        ns_ipc_buf_done2(b_in, &b_out);
        free(b_in);
    } else {
        _error_code = EIO;
    }

    SET_ERROR(_error_code);
    return retval;
}

int nfp_hwdbg_rpc_version(int *cln_ver, int *cln_compat, int *srv_ver, int *srv_compat)
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
    *cln_ver = 2;
    *cln_compat = 2;
    if (*cln_compat) {
        if (*cln_ver > *srv_ver)
            return 1;
        if (*srv_ver < *cln_compat)
            return 1;
    } else if (*srv_ver != *cln_ver)
        return 1;
    return 0;
}
