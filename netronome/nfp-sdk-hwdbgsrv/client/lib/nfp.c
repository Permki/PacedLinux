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

#include "nfp.h"
#include "nfp_handles.h"

#ifdef _MSC_VER    /* This block is specific to MS VC++. */
    #include <Windows.h>
    #include <winerror.h>
    #define EPROTO WSAEPROTOTYPE
    #define SET_ERROR(x) do { SetLastError(x); errno = x; } while(0)
#else
    #define SET_ERROR(x) do { errno = x; } while(0)
#endif

uint64_t nfp_device_mask(void)
{
    ns_ipc_buf_t *b_in = NULL, b_out;
    int _error_code = 0;
    int ret = 0;
    struct ns_ipc_client_handle ipc_h;
    uint64_t retval = 0;

    {
        char *url = nfp_get_server_url(-1);
        ret = ns_ipc_client_init(&ipc_h, url);
        if (url)
            free(url);
    }
    if (ret) {
        NS_LOG(NS_LOG_LVL_ERROR, "failed to open IPC client");
        SET_ERROR(ENXIO);
        return 0;
    }
    ns_ipc_buf_init(&b_out);
    ns_ipc_buf_pack(&b_out,
                    PK_STR(__func__),
                    PK_END);
    if (ns_ipc_client_send_receive(&ipc_h, &b_in, &b_out) < 0)
        b_in = NULL;
    if (b_in) {
        ns_ipc_buf_unpack(b_in,
                          UP_INT32(_error_code),
                          UP_UINT64(retval),
                          UP_END);

        ns_ipc_buf_done2(b_in, &b_out);
        free(b_in);
    } else {
        _error_code = EIO;
    }

    ns_ipc_client_close(&ipc_h);
    SET_ERROR(_error_code);
    return retval;
}

int nfp_num_devices(void)
{
    ns_ipc_buf_t *b_in = NULL, b_out;
    int _error_code = 0;
    int ret = 0;
    struct ns_ipc_client_handle ipc_h;
    int retval = 0;

    {
        char *url = nfp_get_server_url(-1);
        ret = ns_ipc_client_init(&ipc_h, url);
        if (url)
            free(url);
    }
    if (ret) {
        NS_LOG(NS_LOG_LVL_ERROR, "failed to open IPC client");
        SET_ERROR(ENXIO);
        return -1;
    }
    ns_ipc_buf_init(&b_out);
    ns_ipc_buf_pack(&b_out,
                    PK_STR(__func__),
                    PK_END);
    if (ns_ipc_client_send_receive(&ipc_h, &b_in, &b_out) < 0)
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

    ns_ipc_client_close(&ipc_h);
    SET_ERROR(_error_code);
    return retval;
}

struct nfp_device * nfp_device_open(unsigned int devnum)
{
    ns_ipc_buf_t *b_in = NULL, b_out;
    int _error_code = 0;
    int ret = 0;
    struct nfp_device * retval = NULL;
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
                    PK_UINT32(1), /* api version */
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

    if (rh && nfp_client_check_version("nfp.h", 1, 1, _api_version) < 0) {
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
        nfp_device_open_local(
                retval,
                devnum
                ); /* do local handle init at last minute */
        retval->rh = rh;
    }
    SET_ERROR(_error_code);
    return retval;
}

struct nfp_device * nfp_device_from_cpp(struct nfp_cpp * cpp)
{
    ns_ipc_buf_t *b_in = NULL, b_out;
    int _error_code = 0;
    struct nfp_device * retval = NULL;
    uint64_t rh = 0;
    uint32_t _api_version = 0;

    retval = malloc(sizeof(*retval));
    if (!retval) {
        SET_ERROR(ENOMEM);
        return NULL;
    }
    retval->ipc_h = cpp->ipc_h;
    retval->own_handle = 0;
    handle_common_init(&retval->h_common);
    retval->id = cpp->id;
    ns_ipc_buf_init(&b_out);
    ns_ipc_buf_pack(&b_out,
                    PK_STR(__func__),
                    PK_UINT32(1), /* api version */
                    PK_UINT64(cpp->rh),
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

    if (rh && nfp_client_check_version("nfp.h", 1, 1, _api_version) < 0) {
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
        nfp_device_from_cpp_local(
                retval,
                cpp
                ); /* do local handle init at last minute */
        retval->rh = rh;
    }
    SET_ERROR(_error_code);
    return retval;
}

void nfp_device_close(struct nfp_device * dev)
{
    ns_ipc_buf_t *b_in = NULL, b_out;
    int _error_code = 0;

    ns_ipc_buf_init(&b_out);
    ns_ipc_buf_pack(&b_out,
                    PK_STR(__func__),
                    PK_UINT64(dev->rh),
                    PK_END);
    if (ns_ipc_client_send_receive(dev->ipc_h, &b_in, &b_out) < 0)
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

    if (dev->own_handle)
        ns_ipc_client_close(dev->ipc_h);
    nfp_device_close_local(dev); /* do local handle free */
    free(dev);
    SET_ERROR(_error_code);
}

ssize_t nfp_sram_read(struct nfp_device * dev, void * buffer, unsigned long long length, unsigned long long offset)
{
    ns_ipc_buf_t *b_in = NULL, b_out;
    int _error_code = 0;
    int64_t retval = 0;
    void * o_buffer;
    size_t olen_buffer;
    uint64_t inconv_length = length;
    uint64_t inconv_offset = offset;

    ns_ipc_buf_init(&b_out);
    ns_ipc_buf_pack(&b_out,
                    PK_STR(__func__),
                    PK_UINT64(dev->rh),
                    PK_UINT64(inconv_length),
                    PK_UINT64(inconv_offset),
                    PK_END);
    if (ns_ipc_client_send_receive(dev->ipc_h, &b_in, &b_out) < 0)
        b_in = NULL;
    if (b_in) {
        ns_ipc_buf_unpack(b_in,
                          UP_INT32(_error_code),
                          UP_INT64(retval),
                          UP_BYTES(o_buffer, olen_buffer),
                          UP_END);

        if (b_in->error == 0) {
            if (buffer) {
                memcpy(buffer, o_buffer, olen_buffer);
            }
        } else
            NS_LOG(NS_LOG_LVL_ERROR, "error occurred during buffer unpack for nfp_sram_read");
        ns_ipc_buf_done2(b_in, &b_out);
        free(b_in);
    } else {
        _error_code = EIO;
    }

    SET_ERROR(_error_code);
    return retval;
}

ssize_t nfp_sram_write(struct nfp_device * dev, const void * buffer, unsigned long long length, unsigned long long offset)
{
    ns_ipc_buf_t *b_in = NULL, b_out;
    int _error_code = 0;
    int64_t retval = 0;
    uint64_t inconv_length = length;
    uint64_t inconv_offset = offset;

    ns_ipc_buf_init(&b_out);
    ns_ipc_buf_pack(&b_out,
                    PK_STR(__func__),
                    PK_UINT64(dev->rh),
                    PK_BYTES(buffer, length),
                    PK_UINT64(inconv_length),
                    PK_UINT64(inconv_offset),
                    PK_END);
    if (ns_ipc_client_send_receive(dev->ipc_h, &b_in, &b_out) < 0)
        b_in = NULL;
    if (b_in) {
        ns_ipc_buf_unpack(b_in,
                          UP_INT32(_error_code),
                          UP_INT64(retval),
                          UP_END);

        ns_ipc_buf_done2(b_in, &b_out);
        free(b_in);
    } else {
        _error_code = EIO;
    }

    SET_ERROR(_error_code);
    return retval;
}

ssize_t nfp_sram_fill(struct nfp_device * dev, const void * buffer, unsigned long long buf_length, unsigned long long offset, unsigned long long write_length)
{
    ns_ipc_buf_t *b_in = NULL, b_out;
    int _error_code = 0;
    int64_t retval = 0;
    uint64_t inconv_buf_length = buf_length;
    uint64_t inconv_offset = offset;
    uint64_t inconv_write_length = write_length;

    ns_ipc_buf_init(&b_out);
    ns_ipc_buf_pack(&b_out,
                    PK_STR(__func__),
                    PK_UINT64(dev->rh),
                    PK_BYTES(buffer, buf_length),
                    PK_UINT64(inconv_buf_length),
                    PK_UINT64(inconv_offset),
                    PK_UINT64(inconv_write_length),
                    PK_END);
    if (ns_ipc_client_send_receive(dev->ipc_h, &b_in, &b_out) < 0)
        b_in = NULL;
    if (b_in) {
        ns_ipc_buf_unpack(b_in,
                          UP_INT32(_error_code),
                          UP_INT64(retval),
                          UP_END);

        ns_ipc_buf_done2(b_in, &b_out);
        free(b_in);
    } else {
        _error_code = EIO;
    }

    SET_ERROR(_error_code);
    return retval;
}

ssize_t nfp_emem_read(struct nfp_device * dev, int islnum, void * buffer, unsigned long long length, unsigned long long offset)
{
    ns_ipc_buf_t *b_in = NULL, b_out;
    int _error_code = 0;
    int64_t retval = 0;
    void * o_buffer;
    size_t olen_buffer;
    int32_t inconv_islnum = islnum;
    uint64_t inconv_length = length;
    uint64_t inconv_offset = offset;

    ns_ipc_buf_init(&b_out);
    ns_ipc_buf_pack(&b_out,
                    PK_STR(__func__),
                    PK_UINT64(dev->rh),
                    PK_INT32(inconv_islnum),
                    PK_UINT64(inconv_length),
                    PK_UINT64(inconv_offset),
                    PK_END);
    if (ns_ipc_client_send_receive(dev->ipc_h, &b_in, &b_out) < 0)
        b_in = NULL;
    if (b_in) {
        ns_ipc_buf_unpack(b_in,
                          UP_INT32(_error_code),
                          UP_INT64(retval),
                          UP_BYTES(o_buffer, olen_buffer),
                          UP_END);

        if (b_in->error == 0) {
            if (buffer) {
                memcpy(buffer, o_buffer, olen_buffer);
            }
        } else
            NS_LOG(NS_LOG_LVL_ERROR, "error occurred during buffer unpack for nfp_emem_read");
        ns_ipc_buf_done2(b_in, &b_out);
        free(b_in);
    } else {
        _error_code = EIO;
    }

    SET_ERROR(_error_code);
    return retval;
}

ssize_t nfp_emem_write(struct nfp_device * dev, int islnum, const void * buffer, unsigned long long length, unsigned long long offset)
{
    ns_ipc_buf_t *b_in = NULL, b_out;
    int _error_code = 0;
    int64_t retval = 0;
    int32_t inconv_islnum = islnum;
    uint64_t inconv_length = length;
    uint64_t inconv_offset = offset;

    ns_ipc_buf_init(&b_out);
    ns_ipc_buf_pack(&b_out,
                    PK_STR(__func__),
                    PK_UINT64(dev->rh),
                    PK_INT32(inconv_islnum),
                    PK_BYTES(buffer, length),
                    PK_UINT64(inconv_length),
                    PK_UINT64(inconv_offset),
                    PK_END);
    if (ns_ipc_client_send_receive(dev->ipc_h, &b_in, &b_out) < 0)
        b_in = NULL;
    if (b_in) {
        ns_ipc_buf_unpack(b_in,
                          UP_INT32(_error_code),
                          UP_INT64(retval),
                          UP_END);

        ns_ipc_buf_done2(b_in, &b_out);
        free(b_in);
    } else {
        _error_code = EIO;
    }

    SET_ERROR(_error_code);
    return retval;
}

ssize_t nfp_emem_fill(struct nfp_device * dev, int islnum, const void * buffer, unsigned long long buf_length, unsigned long long offset, unsigned long long write_length)
{
    ns_ipc_buf_t *b_in = NULL, b_out;
    int _error_code = 0;
    int64_t retval = 0;
    int32_t inconv_islnum = islnum;
    uint64_t inconv_buf_length = buf_length;
    uint64_t inconv_offset = offset;
    uint64_t inconv_write_length = write_length;

    ns_ipc_buf_init(&b_out);
    ns_ipc_buf_pack(&b_out,
                    PK_STR(__func__),
                    PK_UINT64(dev->rh),
                    PK_INT32(inconv_islnum),
                    PK_BYTES(buffer, buf_length),
                    PK_UINT64(inconv_buf_length),
                    PK_UINT64(inconv_offset),
                    PK_UINT64(inconv_write_length),
                    PK_END);
    if (ns_ipc_client_send_receive(dev->ipc_h, &b_in, &b_out) < 0)
        b_in = NULL;
    if (b_in) {
        ns_ipc_buf_unpack(b_in,
                          UP_INT32(_error_code),
                          UP_INT64(retval),
                          UP_END);

        ns_ipc_buf_done2(b_in, &b_out);
        free(b_in);
    } else {
        _error_code = EIO;
    }

    SET_ERROR(_error_code);
    return retval;
}

ssize_t nfp_emem64_read(struct nfp_device * dev, int islnum, void * buffer, unsigned long long length, unsigned long long offset)
{
    ns_ipc_buf_t *b_in = NULL, b_out;
    int _error_code = 0;
    int64_t retval = 0;
    void * o_buffer;
    size_t olen_buffer;
    int32_t inconv_islnum = islnum;
    uint64_t inconv_length = length;
    uint64_t inconv_offset = offset;

    ns_ipc_buf_init(&b_out);
    ns_ipc_buf_pack(&b_out,
                    PK_STR(__func__),
                    PK_UINT64(dev->rh),
                    PK_INT32(inconv_islnum),
                    PK_UINT64(inconv_length),
                    PK_UINT64(inconv_offset),
                    PK_END);
    if (ns_ipc_client_send_receive(dev->ipc_h, &b_in, &b_out) < 0)
        b_in = NULL;
    if (b_in) {
        ns_ipc_buf_unpack(b_in,
                          UP_INT32(_error_code),
                          UP_INT64(retval),
                          UP_BYTES(o_buffer, olen_buffer),
                          UP_END);

        if (b_in->error == 0) {
            if (buffer) {
                memcpy(buffer, o_buffer, olen_buffer);
            }
        } else
            NS_LOG(NS_LOG_LVL_ERROR, "error occurred during buffer unpack for nfp_emem64_read");
        ns_ipc_buf_done2(b_in, &b_out);
        free(b_in);
    } else {
        _error_code = EIO;
    }

    SET_ERROR(_error_code);
    return retval;
}

ssize_t nfp_emem64_write(struct nfp_device * dev, int islnum, const void * buffer, unsigned long long length, unsigned long long offset)
{
    ns_ipc_buf_t *b_in = NULL, b_out;
    int _error_code = 0;
    int64_t retval = 0;
    int32_t inconv_islnum = islnum;
    uint64_t inconv_length = length;
    uint64_t inconv_offset = offset;

    ns_ipc_buf_init(&b_out);
    ns_ipc_buf_pack(&b_out,
                    PK_STR(__func__),
                    PK_UINT64(dev->rh),
                    PK_INT32(inconv_islnum),
                    PK_BYTES(buffer, length),
                    PK_UINT64(inconv_length),
                    PK_UINT64(inconv_offset),
                    PK_END);
    if (ns_ipc_client_send_receive(dev->ipc_h, &b_in, &b_out) < 0)
        b_in = NULL;
    if (b_in) {
        ns_ipc_buf_unpack(b_in,
                          UP_INT32(_error_code),
                          UP_INT64(retval),
                          UP_END);

        ns_ipc_buf_done2(b_in, &b_out);
        free(b_in);
    } else {
        _error_code = EIO;
    }

    SET_ERROR(_error_code);
    return retval;
}

ssize_t nfp_emem64_fill(struct nfp_device * dev, int islnum, const void * buffer, unsigned long long buf_length, unsigned long long offset, unsigned long long write_length)
{
    ns_ipc_buf_t *b_in = NULL, b_out;
    int _error_code = 0;
    int64_t retval = 0;
    int32_t inconv_islnum = islnum;
    uint64_t inconv_buf_length = buf_length;
    uint64_t inconv_offset = offset;
    uint64_t inconv_write_length = write_length;

    ns_ipc_buf_init(&b_out);
    ns_ipc_buf_pack(&b_out,
                    PK_STR(__func__),
                    PK_UINT64(dev->rh),
                    PK_INT32(inconv_islnum),
                    PK_BYTES(buffer, buf_length),
                    PK_UINT64(inconv_buf_length),
                    PK_UINT64(inconv_offset),
                    PK_UINT64(inconv_write_length),
                    PK_END);
    if (ns_ipc_client_send_receive(dev->ipc_h, &b_in, &b_out) < 0)
        b_in = NULL;
    if (b_in) {
        ns_ipc_buf_unpack(b_in,
                          UP_INT32(_error_code),
                          UP_INT64(retval),
                          UP_END);

        ns_ipc_buf_done2(b_in, &b_out);
        free(b_in);
    } else {
        _error_code = EIO;
    }

    SET_ERROR(_error_code);
    return retval;
}

ssize_t nfp_imem_read(struct nfp_device * dev, int islnum, void * buffer, unsigned long long length, unsigned long long offset)
{
    ns_ipc_buf_t *b_in = NULL, b_out;
    int _error_code = 0;
    int64_t retval = 0;
    void * o_buffer;
    size_t olen_buffer;
    int32_t inconv_islnum = islnum;
    uint64_t inconv_length = length;
    uint64_t inconv_offset = offset;

    ns_ipc_buf_init(&b_out);
    ns_ipc_buf_pack(&b_out,
                    PK_STR(__func__),
                    PK_UINT64(dev->rh),
                    PK_INT32(inconv_islnum),
                    PK_UINT64(inconv_length),
                    PK_UINT64(inconv_offset),
                    PK_END);
    if (ns_ipc_client_send_receive(dev->ipc_h, &b_in, &b_out) < 0)
        b_in = NULL;
    if (b_in) {
        ns_ipc_buf_unpack(b_in,
                          UP_INT32(_error_code),
                          UP_INT64(retval),
                          UP_BYTES(o_buffer, olen_buffer),
                          UP_END);

        if (b_in->error == 0) {
            if (buffer) {
                memcpy(buffer, o_buffer, olen_buffer);
            }
        } else
            NS_LOG(NS_LOG_LVL_ERROR, "error occurred during buffer unpack for nfp_imem_read");
        ns_ipc_buf_done2(b_in, &b_out);
        free(b_in);
    } else {
        _error_code = EIO;
    }

    SET_ERROR(_error_code);
    return retval;
}

ssize_t nfp_imem_write(struct nfp_device * dev, int islnum, const void * buffer, unsigned long long length, unsigned long long offset)
{
    ns_ipc_buf_t *b_in = NULL, b_out;
    int _error_code = 0;
    int64_t retval = 0;
    int32_t inconv_islnum = islnum;
    uint64_t inconv_length = length;
    uint64_t inconv_offset = offset;

    ns_ipc_buf_init(&b_out);
    ns_ipc_buf_pack(&b_out,
                    PK_STR(__func__),
                    PK_UINT64(dev->rh),
                    PK_INT32(inconv_islnum),
                    PK_BYTES(buffer, length),
                    PK_UINT64(inconv_length),
                    PK_UINT64(inconv_offset),
                    PK_END);
    if (ns_ipc_client_send_receive(dev->ipc_h, &b_in, &b_out) < 0)
        b_in = NULL;
    if (b_in) {
        ns_ipc_buf_unpack(b_in,
                          UP_INT32(_error_code),
                          UP_INT64(retval),
                          UP_END);

        ns_ipc_buf_done2(b_in, &b_out);
        free(b_in);
    } else {
        _error_code = EIO;
    }

    SET_ERROR(_error_code);
    return retval;
}

ssize_t nfp_imem_fill(struct nfp_device * dev, int islnum, const void * buffer, unsigned long long buf_length, unsigned long long offset, unsigned long long write_length)
{
    ns_ipc_buf_t *b_in = NULL, b_out;
    int _error_code = 0;
    int64_t retval = 0;
    int32_t inconv_islnum = islnum;
    uint64_t inconv_buf_length = buf_length;
    uint64_t inconv_offset = offset;
    uint64_t inconv_write_length = write_length;

    ns_ipc_buf_init(&b_out);
    ns_ipc_buf_pack(&b_out,
                    PK_STR(__func__),
                    PK_UINT64(dev->rh),
                    PK_INT32(inconv_islnum),
                    PK_BYTES(buffer, buf_length),
                    PK_UINT64(inconv_buf_length),
                    PK_UINT64(inconv_offset),
                    PK_UINT64(inconv_write_length),
                    PK_END);
    if (ns_ipc_client_send_receive(dev->ipc_h, &b_in, &b_out) < 0)
        b_in = NULL;
    if (b_in) {
        ns_ipc_buf_unpack(b_in,
                          UP_INT32(_error_code),
                          UP_INT64(retval),
                          UP_END);

        ns_ipc_buf_done2(b_in, &b_out);
        free(b_in);
    } else {
        _error_code = EIO;
    }

    SET_ERROR(_error_code);
    return retval;
}

ssize_t nfp_imem64_read(struct nfp_device * dev, int islnum, void * buffer, unsigned long long length, unsigned long long offset)
{
    ns_ipc_buf_t *b_in = NULL, b_out;
    int _error_code = 0;
    int64_t retval = 0;
    void * o_buffer;
    size_t olen_buffer;
    int32_t inconv_islnum = islnum;
    uint64_t inconv_length = length;
    uint64_t inconv_offset = offset;

    ns_ipc_buf_init(&b_out);
    ns_ipc_buf_pack(&b_out,
                    PK_STR(__func__),
                    PK_UINT64(dev->rh),
                    PK_INT32(inconv_islnum),
                    PK_UINT64(inconv_length),
                    PK_UINT64(inconv_offset),
                    PK_END);
    if (ns_ipc_client_send_receive(dev->ipc_h, &b_in, &b_out) < 0)
        b_in = NULL;
    if (b_in) {
        ns_ipc_buf_unpack(b_in,
                          UP_INT32(_error_code),
                          UP_INT64(retval),
                          UP_BYTES(o_buffer, olen_buffer),
                          UP_END);

        if (b_in->error == 0) {
            if (buffer) {
                memcpy(buffer, o_buffer, olen_buffer);
            }
        } else
            NS_LOG(NS_LOG_LVL_ERROR, "error occurred during buffer unpack for nfp_imem64_read");
        ns_ipc_buf_done2(b_in, &b_out);
        free(b_in);
    } else {
        _error_code = EIO;
    }

    SET_ERROR(_error_code);
    return retval;
}

ssize_t nfp_imem64_write(struct nfp_device * dev, int islnum, const void * buffer, unsigned long long length, unsigned long long offset)
{
    ns_ipc_buf_t *b_in = NULL, b_out;
    int _error_code = 0;
    int64_t retval = 0;
    int32_t inconv_islnum = islnum;
    uint64_t inconv_length = length;
    uint64_t inconv_offset = offset;

    ns_ipc_buf_init(&b_out);
    ns_ipc_buf_pack(&b_out,
                    PK_STR(__func__),
                    PK_UINT64(dev->rh),
                    PK_INT32(inconv_islnum),
                    PK_BYTES(buffer, length),
                    PK_UINT64(inconv_length),
                    PK_UINT64(inconv_offset),
                    PK_END);
    if (ns_ipc_client_send_receive(dev->ipc_h, &b_in, &b_out) < 0)
        b_in = NULL;
    if (b_in) {
        ns_ipc_buf_unpack(b_in,
                          UP_INT32(_error_code),
                          UP_INT64(retval),
                          UP_END);

        ns_ipc_buf_done2(b_in, &b_out);
        free(b_in);
    } else {
        _error_code = EIO;
    }

    SET_ERROR(_error_code);
    return retval;
}

ssize_t nfp_imem64_fill(struct nfp_device * dev, int islnum, const void * buffer, unsigned long long buf_length, unsigned long long offset, unsigned long long write_length)
{
    ns_ipc_buf_t *b_in = NULL, b_out;
    int _error_code = 0;
    int64_t retval = 0;
    int32_t inconv_islnum = islnum;
    uint64_t inconv_buf_length = buf_length;
    uint64_t inconv_offset = offset;
    uint64_t inconv_write_length = write_length;

    ns_ipc_buf_init(&b_out);
    ns_ipc_buf_pack(&b_out,
                    PK_STR(__func__),
                    PK_UINT64(dev->rh),
                    PK_INT32(inconv_islnum),
                    PK_BYTES(buffer, buf_length),
                    PK_UINT64(inconv_buf_length),
                    PK_UINT64(inconv_offset),
                    PK_UINT64(inconv_write_length),
                    PK_END);
    if (ns_ipc_client_send_receive(dev->ipc_h, &b_in, &b_out) < 0)
        b_in = NULL;
    if (b_in) {
        ns_ipc_buf_unpack(b_in,
                          UP_INT32(_error_code),
                          UP_INT64(retval),
                          UP_END);

        ns_ipc_buf_done2(b_in, &b_out);
        free(b_in);
    } else {
        _error_code = EIO;
    }

    SET_ERROR(_error_code);
    return retval;
}

ssize_t nfp_ctm_read(struct nfp_device * dev, int islnum, void * buffer, unsigned long long length, unsigned long long offset)
{
    ns_ipc_buf_t *b_in = NULL, b_out;
    int _error_code = 0;
    int64_t retval = 0;
    void * o_buffer;
    size_t olen_buffer;
    int32_t inconv_islnum = islnum;
    uint64_t inconv_length = length;
    uint64_t inconv_offset = offset;

    ns_ipc_buf_init(&b_out);
    ns_ipc_buf_pack(&b_out,
                    PK_STR(__func__),
                    PK_UINT64(dev->rh),
                    PK_INT32(inconv_islnum),
                    PK_UINT64(inconv_length),
                    PK_UINT64(inconv_offset),
                    PK_END);
    if (ns_ipc_client_send_receive(dev->ipc_h, &b_in, &b_out) < 0)
        b_in = NULL;
    if (b_in) {
        ns_ipc_buf_unpack(b_in,
                          UP_INT32(_error_code),
                          UP_INT64(retval),
                          UP_BYTES(o_buffer, olen_buffer),
                          UP_END);

        if (b_in->error == 0) {
            if (buffer) {
                memcpy(buffer, o_buffer, olen_buffer);
            }
        } else
            NS_LOG(NS_LOG_LVL_ERROR, "error occurred during buffer unpack for nfp_ctm_read");
        ns_ipc_buf_done2(b_in, &b_out);
        free(b_in);
    } else {
        _error_code = EIO;
    }

    SET_ERROR(_error_code);
    return retval;
}

ssize_t nfp_ctm_write(struct nfp_device * dev, int islnum, const void * buffer, unsigned long long length, unsigned long long offset)
{
    ns_ipc_buf_t *b_in = NULL, b_out;
    int _error_code = 0;
    int64_t retval = 0;
    int32_t inconv_islnum = islnum;
    uint64_t inconv_length = length;
    uint64_t inconv_offset = offset;

    ns_ipc_buf_init(&b_out);
    ns_ipc_buf_pack(&b_out,
                    PK_STR(__func__),
                    PK_UINT64(dev->rh),
                    PK_INT32(inconv_islnum),
                    PK_BYTES(buffer, length),
                    PK_UINT64(inconv_length),
                    PK_UINT64(inconv_offset),
                    PK_END);
    if (ns_ipc_client_send_receive(dev->ipc_h, &b_in, &b_out) < 0)
        b_in = NULL;
    if (b_in) {
        ns_ipc_buf_unpack(b_in,
                          UP_INT32(_error_code),
                          UP_INT64(retval),
                          UP_END);

        ns_ipc_buf_done2(b_in, &b_out);
        free(b_in);
    } else {
        _error_code = EIO;
    }

    SET_ERROR(_error_code);
    return retval;
}

ssize_t nfp_ctm_fill(struct nfp_device * dev, int islnum, const void * buffer, unsigned long long buf_length, unsigned long long offset, unsigned long long write_length)
{
    ns_ipc_buf_t *b_in = NULL, b_out;
    int _error_code = 0;
    int64_t retval = 0;
    int32_t inconv_islnum = islnum;
    uint64_t inconv_buf_length = buf_length;
    uint64_t inconv_offset = offset;
    uint64_t inconv_write_length = write_length;

    ns_ipc_buf_init(&b_out);
    ns_ipc_buf_pack(&b_out,
                    PK_STR(__func__),
                    PK_UINT64(dev->rh),
                    PK_INT32(inconv_islnum),
                    PK_BYTES(buffer, buf_length),
                    PK_UINT64(inconv_buf_length),
                    PK_UINT64(inconv_offset),
                    PK_UINT64(inconv_write_length),
                    PK_END);
    if (ns_ipc_client_send_receive(dev->ipc_h, &b_in, &b_out) < 0)
        b_in = NULL;
    if (b_in) {
        ns_ipc_buf_unpack(b_in,
                          UP_INT32(_error_code),
                          UP_INT64(retval),
                          UP_END);

        ns_ipc_buf_done2(b_in, &b_out);
        free(b_in);
    } else {
        _error_code = EIO;
    }

    SET_ERROR(_error_code);
    return retval;
}

ssize_t nfp_ctm64_read(struct nfp_device * dev, int islnum, void * buffer, unsigned long long length, unsigned long long offset)
{
    ns_ipc_buf_t *b_in = NULL, b_out;
    int _error_code = 0;
    int64_t retval = 0;
    void * o_buffer;
    size_t olen_buffer;
    int32_t inconv_islnum = islnum;
    uint64_t inconv_length = length;
    uint64_t inconv_offset = offset;

    ns_ipc_buf_init(&b_out);
    ns_ipc_buf_pack(&b_out,
                    PK_STR(__func__),
                    PK_UINT64(dev->rh),
                    PK_INT32(inconv_islnum),
                    PK_UINT64(inconv_length),
                    PK_UINT64(inconv_offset),
                    PK_END);
    if (ns_ipc_client_send_receive(dev->ipc_h, &b_in, &b_out) < 0)
        b_in = NULL;
    if (b_in) {
        ns_ipc_buf_unpack(b_in,
                          UP_INT32(_error_code),
                          UP_INT64(retval),
                          UP_BYTES(o_buffer, olen_buffer),
                          UP_END);

        if (b_in->error == 0) {
            if (buffer) {
                memcpy(buffer, o_buffer, olen_buffer);
            }
        } else
            NS_LOG(NS_LOG_LVL_ERROR, "error occurred during buffer unpack for nfp_ctm64_read");
        ns_ipc_buf_done2(b_in, &b_out);
        free(b_in);
    } else {
        _error_code = EIO;
    }

    SET_ERROR(_error_code);
    return retval;
}

ssize_t nfp_ctm64_write(struct nfp_device * dev, int islnum, const void * buffer, unsigned long long length, unsigned long long offset)
{
    ns_ipc_buf_t *b_in = NULL, b_out;
    int _error_code = 0;
    int64_t retval = 0;
    int32_t inconv_islnum = islnum;
    uint64_t inconv_length = length;
    uint64_t inconv_offset = offset;

    ns_ipc_buf_init(&b_out);
    ns_ipc_buf_pack(&b_out,
                    PK_STR(__func__),
                    PK_UINT64(dev->rh),
                    PK_INT32(inconv_islnum),
                    PK_BYTES(buffer, length),
                    PK_UINT64(inconv_length),
                    PK_UINT64(inconv_offset),
                    PK_END);
    if (ns_ipc_client_send_receive(dev->ipc_h, &b_in, &b_out) < 0)
        b_in = NULL;
    if (b_in) {
        ns_ipc_buf_unpack(b_in,
                          UP_INT32(_error_code),
                          UP_INT64(retval),
                          UP_END);

        ns_ipc_buf_done2(b_in, &b_out);
        free(b_in);
    } else {
        _error_code = EIO;
    }

    SET_ERROR(_error_code);
    return retval;
}

ssize_t nfp_ctm64_fill(struct nfp_device * dev, int islnum, const void * buffer, unsigned long long buf_length, unsigned long long offset, unsigned long long write_length)
{
    ns_ipc_buf_t *b_in = NULL, b_out;
    int _error_code = 0;
    int64_t retval = 0;
    int32_t inconv_islnum = islnum;
    uint64_t inconv_buf_length = buf_length;
    uint64_t inconv_offset = offset;
    uint64_t inconv_write_length = write_length;

    ns_ipc_buf_init(&b_out);
    ns_ipc_buf_pack(&b_out,
                    PK_STR(__func__),
                    PK_UINT64(dev->rh),
                    PK_INT32(inconv_islnum),
                    PK_BYTES(buffer, buf_length),
                    PK_UINT64(inconv_buf_length),
                    PK_UINT64(inconv_offset),
                    PK_UINT64(inconv_write_length),
                    PK_END);
    if (ns_ipc_client_send_receive(dev->ipc_h, &b_in, &b_out) < 0)
        b_in = NULL;
    if (b_in) {
        ns_ipc_buf_unpack(b_in,
                          UP_INT32(_error_code),
                          UP_INT64(retval),
                          UP_END);

        ns_ipc_buf_done2(b_in, &b_out);
        free(b_in);
    } else {
        _error_code = EIO;
    }

    SET_ERROR(_error_code);
    return retval;
}

ssize_t nfp_cls_read(struct nfp_device * dev, int clnum, void * buffer, unsigned long long length, unsigned long long offset)
{
    ns_ipc_buf_t *b_in = NULL, b_out;
    int _error_code = 0;
    int64_t retval = 0;
    void * o_buffer;
    size_t olen_buffer;
    int32_t inconv_clnum = clnum;
    uint64_t inconv_length = length;
    uint64_t inconv_offset = offset;

    ns_ipc_buf_init(&b_out);
    ns_ipc_buf_pack(&b_out,
                    PK_STR(__func__),
                    PK_UINT64(dev->rh),
                    PK_INT32(inconv_clnum),
                    PK_UINT64(inconv_length),
                    PK_UINT64(inconv_offset),
                    PK_END);
    if (ns_ipc_client_send_receive(dev->ipc_h, &b_in, &b_out) < 0)
        b_in = NULL;
    if (b_in) {
        ns_ipc_buf_unpack(b_in,
                          UP_INT32(_error_code),
                          UP_INT64(retval),
                          UP_BYTES(o_buffer, olen_buffer),
                          UP_END);

        if (b_in->error == 0) {
            if (buffer) {
                memcpy(buffer, o_buffer, olen_buffer);
            }
        } else
            NS_LOG(NS_LOG_LVL_ERROR, "error occurred during buffer unpack for nfp_cls_read");
        ns_ipc_buf_done2(b_in, &b_out);
        free(b_in);
    } else {
        _error_code = EIO;
    }

    SET_ERROR(_error_code);
    return retval;
}

ssize_t nfp_cls_write(struct nfp_device * dev, int clnum, const void * buffer, unsigned long long length, unsigned long long offset)
{
    ns_ipc_buf_t *b_in = NULL, b_out;
    int _error_code = 0;
    int64_t retval = 0;
    int32_t inconv_clnum = clnum;
    uint64_t inconv_length = length;
    uint64_t inconv_offset = offset;

    ns_ipc_buf_init(&b_out);
    ns_ipc_buf_pack(&b_out,
                    PK_STR(__func__),
                    PK_UINT64(dev->rh),
                    PK_INT32(inconv_clnum),
                    PK_BYTES(buffer, length),
                    PK_UINT64(inconv_length),
                    PK_UINT64(inconv_offset),
                    PK_END);
    if (ns_ipc_client_send_receive(dev->ipc_h, &b_in, &b_out) < 0)
        b_in = NULL;
    if (b_in) {
        ns_ipc_buf_unpack(b_in,
                          UP_INT32(_error_code),
                          UP_INT64(retval),
                          UP_END);

        ns_ipc_buf_done2(b_in, &b_out);
        free(b_in);
    } else {
        _error_code = EIO;
    }

    SET_ERROR(_error_code);
    return retval;
}

ssize_t nfp_cls_fill(struct nfp_device * dev, int clnum, const void * buffer, unsigned long long buf_length, unsigned long long offset, unsigned long long write_length)
{
    ns_ipc_buf_t *b_in = NULL, b_out;
    int _error_code = 0;
    int64_t retval = 0;
    int32_t inconv_clnum = clnum;
    uint64_t inconv_buf_length = buf_length;
    uint64_t inconv_offset = offset;
    uint64_t inconv_write_length = write_length;

    ns_ipc_buf_init(&b_out);
    ns_ipc_buf_pack(&b_out,
                    PK_STR(__func__),
                    PK_UINT64(dev->rh),
                    PK_INT32(inconv_clnum),
                    PK_BYTES(buffer, buf_length),
                    PK_UINT64(inconv_buf_length),
                    PK_UINT64(inconv_offset),
                    PK_UINT64(inconv_write_length),
                    PK_END);
    if (ns_ipc_client_send_receive(dev->ipc_h, &b_in, &b_out) < 0)
        b_in = NULL;
    if (b_in) {
        ns_ipc_buf_unpack(b_in,
                          UP_INT32(_error_code),
                          UP_INT64(retval),
                          UP_END);

        ns_ipc_buf_done2(b_in, &b_out);
        free(b_in);
    } else {
        _error_code = EIO;
    }

    SET_ERROR(_error_code);
    return retval;
}

ssize_t nfp_lmem_read(struct nfp_device * dev, int meid, void * buffer, unsigned long long length, unsigned long long offset)
{
    ns_ipc_buf_t *b_in = NULL, b_out;
    int _error_code = 0;
    int64_t retval = 0;
    void * o_buffer;
    size_t olen_buffer;
    int32_t inconv_meid = meid;
    uint64_t inconv_length = length;
    uint64_t inconv_offset = offset;

    ns_ipc_buf_init(&b_out);
    ns_ipc_buf_pack(&b_out,
                    PK_STR(__func__),
                    PK_UINT64(dev->rh),
                    PK_INT32(inconv_meid),
                    PK_UINT64(inconv_length),
                    PK_UINT64(inconv_offset),
                    PK_END);
    if (ns_ipc_client_send_receive(dev->ipc_h, &b_in, &b_out) < 0)
        b_in = NULL;
    if (b_in) {
        ns_ipc_buf_unpack(b_in,
                          UP_INT32(_error_code),
                          UP_INT64(retval),
                          UP_BYTES(o_buffer, olen_buffer),
                          UP_END);

        if (b_in->error == 0) {
            if (buffer) {
                memcpy(buffer, o_buffer, olen_buffer);
            }
        } else
            NS_LOG(NS_LOG_LVL_ERROR, "error occurred during buffer unpack for nfp_lmem_read");
        ns_ipc_buf_done2(b_in, &b_out);
        free(b_in);
    } else {
        _error_code = EIO;
    }

    SET_ERROR(_error_code);
    return retval;
}

ssize_t nfp_lmem_write(struct nfp_device * dev, int meid, const void * buffer, unsigned long long length, unsigned long long offset)
{
    ns_ipc_buf_t *b_in = NULL, b_out;
    int _error_code = 0;
    int64_t retval = 0;
    int32_t inconv_meid = meid;
    uint64_t inconv_length = length;
    uint64_t inconv_offset = offset;

    ns_ipc_buf_init(&b_out);
    ns_ipc_buf_pack(&b_out,
                    PK_STR(__func__),
                    PK_UINT64(dev->rh),
                    PK_INT32(inconv_meid),
                    PK_BYTES(buffer, length),
                    PK_UINT64(inconv_length),
                    PK_UINT64(inconv_offset),
                    PK_END);
    if (ns_ipc_client_send_receive(dev->ipc_h, &b_in, &b_out) < 0)
        b_in = NULL;
    if (b_in) {
        ns_ipc_buf_unpack(b_in,
                          UP_INT32(_error_code),
                          UP_INT64(retval),
                          UP_END);

        ns_ipc_buf_done2(b_in, &b_out);
        free(b_in);
    } else {
        _error_code = EIO;
    }

    SET_ERROR(_error_code);
    return retval;
}

ssize_t nfp_lmem_fill(struct nfp_device * dev, int meid, const void * buffer, unsigned long long buf_length, unsigned long long offset, unsigned long long write_length)
{
    ns_ipc_buf_t *b_in = NULL, b_out;
    int _error_code = 0;
    int64_t retval = 0;
    int32_t inconv_meid = meid;
    uint64_t inconv_buf_length = buf_length;
    uint64_t inconv_offset = offset;
    uint64_t inconv_write_length = write_length;

    ns_ipc_buf_init(&b_out);
    ns_ipc_buf_pack(&b_out,
                    PK_STR(__func__),
                    PK_UINT64(dev->rh),
                    PK_INT32(inconv_meid),
                    PK_BYTES(buffer, buf_length),
                    PK_UINT64(inconv_buf_length),
                    PK_UINT64(inconv_offset),
                    PK_UINT64(inconv_write_length),
                    PK_END);
    if (ns_ipc_client_send_receive(dev->ipc_h, &b_in, &b_out) < 0)
        b_in = NULL;
    if (b_in) {
        ns_ipc_buf_unpack(b_in,
                          UP_INT32(_error_code),
                          UP_INT64(retval),
                          UP_END);

        ns_ipc_buf_done2(b_in, &b_out);
        free(b_in);
    } else {
        _error_code = EIO;
    }

    SET_ERROR(_error_code);
    return retval;
}

ssize_t nfp_ustore_read(struct nfp_device * dev, int meid, void * buffer, unsigned long long length, unsigned long long offset)
{
    ns_ipc_buf_t *b_in = NULL, b_out;
    int _error_code = 0;
    int64_t retval = 0;
    void * o_buffer;
    size_t olen_buffer;
    int32_t inconv_meid = meid;
    uint64_t inconv_length = length;
    uint64_t inconv_offset = offset;

    ns_ipc_buf_init(&b_out);
    ns_ipc_buf_pack(&b_out,
                    PK_STR(__func__),
                    PK_UINT64(dev->rh),
                    PK_INT32(inconv_meid),
                    PK_UINT64(inconv_length),
                    PK_UINT64(inconv_offset),
                    PK_END);
    if (ns_ipc_client_send_receive(dev->ipc_h, &b_in, &b_out) < 0)
        b_in = NULL;
    if (b_in) {
        ns_ipc_buf_unpack(b_in,
                          UP_INT32(_error_code),
                          UP_INT64(retval),
                          UP_BYTES(o_buffer, olen_buffer),
                          UP_END);

        if (b_in->error == 0) {
            if (buffer) {
                memcpy(buffer, o_buffer, olen_buffer);
            }
        } else
            NS_LOG(NS_LOG_LVL_ERROR, "error occurred during buffer unpack for nfp_ustore_read");
        ns_ipc_buf_done2(b_in, &b_out);
        free(b_in);
    } else {
        _error_code = EIO;
    }

    SET_ERROR(_error_code);
    return retval;
}

ssize_t nfp_ustore_write(struct nfp_device * dev, int meid, const void * buffer, unsigned long long length, unsigned long long offset)
{
    ns_ipc_buf_t *b_in = NULL, b_out;
    int _error_code = 0;
    int64_t retval = 0;
    int32_t inconv_meid = meid;
    uint64_t inconv_length = length;
    uint64_t inconv_offset = offset;

    ns_ipc_buf_init(&b_out);
    ns_ipc_buf_pack(&b_out,
                    PK_STR(__func__),
                    PK_UINT64(dev->rh),
                    PK_INT32(inconv_meid),
                    PK_BYTES(buffer, length),
                    PK_UINT64(inconv_length),
                    PK_UINT64(inconv_offset),
                    PK_END);
    if (ns_ipc_client_send_receive(dev->ipc_h, &b_in, &b_out) < 0)
        b_in = NULL;
    if (b_in) {
        ns_ipc_buf_unpack(b_in,
                          UP_INT32(_error_code),
                          UP_INT64(retval),
                          UP_END);

        ns_ipc_buf_done2(b_in, &b_out);
        free(b_in);
    } else {
        _error_code = EIO;
    }

    SET_ERROR(_error_code);
    return retval;
}

ssize_t nfp_ustore_fill(struct nfp_device * dev, int meid, const void * buffer, unsigned long long buf_length, unsigned long long offset, unsigned long long write_length)
{
    ns_ipc_buf_t *b_in = NULL, b_out;
    int _error_code = 0;
    int64_t retval = 0;
    int32_t inconv_meid = meid;
    uint64_t inconv_buf_length = buf_length;
    uint64_t inconv_offset = offset;
    uint64_t inconv_write_length = write_length;

    ns_ipc_buf_init(&b_out);
    ns_ipc_buf_pack(&b_out,
                    PK_STR(__func__),
                    PK_UINT64(dev->rh),
                    PK_INT32(inconv_meid),
                    PK_BYTES(buffer, buf_length),
                    PK_UINT64(inconv_buf_length),
                    PK_UINT64(inconv_offset),
                    PK_UINT64(inconv_write_length),
                    PK_END);
    if (ns_ipc_client_send_receive(dev->ipc_h, &b_in, &b_out) < 0)
        b_in = NULL;
    if (b_in) {
        ns_ipc_buf_unpack(b_in,
                          UP_INT32(_error_code),
                          UP_INT64(retval),
                          UP_END);

        ns_ipc_buf_done2(b_in, &b_out);
        free(b_in);
    } else {
        _error_code = EIO;
    }

    SET_ERROR(_error_code);
    return retval;
}

ssize_t nfp_crypto_sram_read(struct nfp_device * dev, int islnum, void * buffer, unsigned long long length, unsigned long long offset)
{
    ns_ipc_buf_t *b_in = NULL, b_out;
    int _error_code = 0;
    int64_t retval = 0;
    void * o_buffer;
    size_t olen_buffer;
    int32_t inconv_islnum = islnum;
    uint64_t inconv_length = length;
    uint64_t inconv_offset = offset;

    ns_ipc_buf_init(&b_out);
    ns_ipc_buf_pack(&b_out,
                    PK_STR(__func__),
                    PK_UINT64(dev->rh),
                    PK_INT32(inconv_islnum),
                    PK_UINT64(inconv_length),
                    PK_UINT64(inconv_offset),
                    PK_END);
    if (ns_ipc_client_send_receive(dev->ipc_h, &b_in, &b_out) < 0)
        b_in = NULL;
    if (b_in) {
        ns_ipc_buf_unpack(b_in,
                          UP_INT32(_error_code),
                          UP_INT64(retval),
                          UP_BYTES(o_buffer, olen_buffer),
                          UP_END);

        if (b_in->error == 0) {
            if (buffer) {
                memcpy(buffer, o_buffer, olen_buffer);
            }
        } else
            NS_LOG(NS_LOG_LVL_ERROR, "error occurred during buffer unpack for nfp_crypto_sram_read");
        ns_ipc_buf_done2(b_in, &b_out);
        free(b_in);
    } else {
        _error_code = EIO;
    }

    SET_ERROR(_error_code);
    return retval;
}

ssize_t nfp_crypto_sram_write(struct nfp_device * dev, int islnum, const void * buffer, unsigned long long length, unsigned long long offset)
{
    ns_ipc_buf_t *b_in = NULL, b_out;
    int _error_code = 0;
    int64_t retval = 0;
    int32_t inconv_islnum = islnum;
    uint64_t inconv_length = length;
    uint64_t inconv_offset = offset;

    ns_ipc_buf_init(&b_out);
    ns_ipc_buf_pack(&b_out,
                    PK_STR(__func__),
                    PK_UINT64(dev->rh),
                    PK_INT32(inconv_islnum),
                    PK_BYTES(buffer, length),
                    PK_UINT64(inconv_length),
                    PK_UINT64(inconv_offset),
                    PK_END);
    if (ns_ipc_client_send_receive(dev->ipc_h, &b_in, &b_out) < 0)
        b_in = NULL;
    if (b_in) {
        ns_ipc_buf_unpack(b_in,
                          UP_INT32(_error_code),
                          UP_INT64(retval),
                          UP_END);

        ns_ipc_buf_done2(b_in, &b_out);
        free(b_in);
    } else {
        _error_code = EIO;
    }

    SET_ERROR(_error_code);
    return retval;
}

ssize_t nfp_crypto_sram_fill(struct nfp_device * dev, int islnum, const void * buffer, unsigned long long buf_length, unsigned long long offset, unsigned long long write_length)
{
    ns_ipc_buf_t *b_in = NULL, b_out;
    int _error_code = 0;
    int64_t retval = 0;
    int32_t inconv_islnum = islnum;
    uint64_t inconv_buf_length = buf_length;
    uint64_t inconv_offset = offset;
    uint64_t inconv_write_length = write_length;

    ns_ipc_buf_init(&b_out);
    ns_ipc_buf_pack(&b_out,
                    PK_STR(__func__),
                    PK_UINT64(dev->rh),
                    PK_INT32(inconv_islnum),
                    PK_BYTES(buffer, buf_length),
                    PK_UINT64(inconv_buf_length),
                    PK_UINT64(inconv_offset),
                    PK_UINT64(inconv_write_length),
                    PK_END);
    if (ns_ipc_client_send_receive(dev->ipc_h, &b_in, &b_out) < 0)
        b_in = NULL;
    if (b_in) {
        ns_ipc_buf_unpack(b_in,
                          UP_INT32(_error_code),
                          UP_INT64(retval),
                          UP_END);

        ns_ipc_buf_done2(b_in, &b_out);
        free(b_in);
    } else {
        _error_code = EIO;
    }

    SET_ERROR(_error_code);
    return retval;
}

int nfp_mecsr_read(struct nfp_device * dev, int meid, int ctx, unsigned int csr, uint32_t * value)
{
    ns_ipc_buf_t *b_in = NULL, b_out;
    int _error_code = 0;
    int32_t retval = 0;
    void * o_value;
    size_t olen_value = sizeof(uint32_t) * 1;
    int32_t inconv_meid = meid;
    int32_t inconv_ctx = ctx;
    uint32_t inconv_csr = csr;

    ns_ipc_buf_init(&b_out);
    ns_ipc_buf_pack(&b_out,
                    PK_STR(__func__),
                    PK_UINT64(dev->rh),
                    PK_INT32(inconv_meid),
                    PK_INT32(inconv_ctx),
                    PK_UINT32(inconv_csr),
                    PK_END);
    if (ns_ipc_client_send_receive(dev->ipc_h, &b_in, &b_out) < 0)
        b_in = NULL;
    if (b_in) {
        ns_ipc_buf_unpack(b_in,
                          UP_INT32(_error_code),
                          UP_INT32(retval),
                          UP_BYTES(o_value, olen_value),
                          UP_END);

        if (b_in->error == 0) {
            if (value) {
                memcpy(value, o_value, olen_value);
            }
        } else
            NS_LOG(NS_LOG_LVL_ERROR, "error occurred during buffer unpack for nfp_mecsr_read");
        ns_ipc_buf_done2(b_in, &b_out);
        free(b_in);
    } else {
        _error_code = EIO;
    }

    SET_ERROR(_error_code);
    return retval;
}

int nfp_mecsr_write(struct nfp_device * dev, int meid, int ctx, unsigned int csr, uint32_t value)
{
    ns_ipc_buf_t *b_in = NULL, b_out;
    int _error_code = 0;
    int32_t retval = 0;
    int32_t inconv_meid = meid;
    int32_t inconv_ctx = ctx;
    uint32_t inconv_csr = csr;

    ns_ipc_buf_init(&b_out);
    ns_ipc_buf_pack(&b_out,
                    PK_STR(__func__),
                    PK_UINT64(dev->rh),
                    PK_INT32(inconv_meid),
                    PK_INT32(inconv_ctx),
                    PK_UINT32(inconv_csr),
                    PK_UINT32(value),
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

int nfp_me_register_read(struct nfp_device * dev, int meid, unsigned int reg, uint32_t * value)
{
    ns_ipc_buf_t *b_in = NULL, b_out;
    int _error_code = 0;
    int32_t retval = 0;
    void * o_value;
    size_t olen_value = sizeof(uint32_t) * 1;
    int32_t inconv_meid = meid;
    uint32_t inconv_reg = reg;

    ns_ipc_buf_init(&b_out);
    ns_ipc_buf_pack(&b_out,
                    PK_STR(__func__),
                    PK_UINT64(dev->rh),
                    PK_INT32(inconv_meid),
                    PK_UINT32(inconv_reg),
                    PK_END);
    if (ns_ipc_client_send_receive(dev->ipc_h, &b_in, &b_out) < 0)
        b_in = NULL;
    if (b_in) {
        ns_ipc_buf_unpack(b_in,
                          UP_INT32(_error_code),
                          UP_INT32(retval),
                          UP_BYTES(o_value, olen_value),
                          UP_END);

        if (b_in->error == 0) {
            if (value) {
                memcpy(value, o_value, olen_value);
            }
        } else
            NS_LOG(NS_LOG_LVL_ERROR, "error occurred during buffer unpack for nfp_me_register_read");
        ns_ipc_buf_done2(b_in, &b_out);
        free(b_in);
    } else {
        _error_code = EIO;
    }

    SET_ERROR(_error_code);
    return retval;
}

int nfp_me_register_write(struct nfp_device * dev, int meid, unsigned int reg, uint32_t value)
{
    ns_ipc_buf_t *b_in = NULL, b_out;
    int _error_code = 0;
    int32_t retval = 0;
    int32_t inconv_meid = meid;
    uint32_t inconv_reg = reg;

    ns_ipc_buf_init(&b_out);
    ns_ipc_buf_pack(&b_out,
                    PK_STR(__func__),
                    PK_UINT64(dev->rh),
                    PK_INT32(inconv_meid),
                    PK_UINT32(inconv_reg),
                    PK_UINT32(value),
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

int nfp_temp(struct nfp_device * dev, uint32_t * raw_value, float * temp_c, float * temp_err)
{
    ns_ipc_buf_t *b_in = NULL, b_out;
    int _error_code = 0;
    int32_t retval = 0;
    void * o_raw_value;
    size_t olen_raw_value = sizeof(uint32_t) * 1;
    void * o_temp_c;
    size_t olen_temp_c = sizeof(float) * 1;
    void * o_temp_err;
    size_t olen_temp_err = sizeof(float) * 1;

    ns_ipc_buf_init(&b_out);
    ns_ipc_buf_pack(&b_out,
                    PK_STR(__func__),
                    PK_UINT64(dev->rh),
                    PK_END);
    if (ns_ipc_client_send_receive(dev->ipc_h, &b_in, &b_out) < 0)
        b_in = NULL;
    if (b_in) {
        ns_ipc_buf_unpack(b_in,
                          UP_INT32(_error_code),
                          UP_INT32(retval),
                          UP_BYTES(o_raw_value, olen_raw_value),
                          UP_BYTES(o_temp_c, olen_temp_c),
                          UP_BYTES(o_temp_err, olen_temp_err),
                          UP_END);

        if (b_in->error == 0) {
            if (raw_value) {
                memcpy(raw_value, o_raw_value, olen_raw_value);
            }
            if (temp_c) {
                memcpy(temp_c, o_temp_c, olen_temp_c);
            }
            if (temp_err) {
                memcpy(temp_err, o_temp_err, olen_temp_err);
            }
        } else
            NS_LOG(NS_LOG_LVL_ERROR, "error occurred during buffer unpack for nfp_temp");
        ns_ipc_buf_done2(b_in, &b_out);
        free(b_in);
    } else {
        _error_code = EIO;
    }

    SET_ERROR(_error_code);
    return retval;
}

int nfp_rpc_version(int *cln_ver, int *cln_compat, int *srv_ver, int *srv_compat)
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
