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

#include "nfp_cpp.h"
#include "nfp_cpp_handles.h"

#ifdef _MSC_VER    /* This block is specific to MS VC++. */
    #include <Windows.h>
    #include <winerror.h>
    #define EPROTO WSAEPROTOTYPE
    #define SET_ERROR(x) do { SetLastError(x); errno = x; } while(0)
#else
    #define SET_ERROR(x) do { errno = x; } while(0)
#endif

struct nfp_cpp * nfp_cpp_from_device_id(int id)
{
    ns_ipc_buf_t *b_in = NULL, b_out;
    int _error_code = 0;
    int ret = 0;
    struct nfp_cpp * retval = NULL;
    uint64_t rh = 0;
    uint32_t _api_version = 0;
    int32_t inconv_id = id;

    retval = malloc(sizeof(*retval));
    if (!retval) {
        SET_ERROR(ENOMEM);
        return NULL;
    }
    {
        char *url = nfp_get_server_url(id);
        retval->id = id;
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
                    PK_UINT32(0), /* api version */
                    PK_INT32(inconv_id),
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

    if (rh && nfp_client_check_version("nfp_cpp.h", 0, 0, _api_version) < 0) {
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
        nfp_cpp_from_device_id_local(
                retval,
                id
                ); /* do local handle init at last minute */
        retval->rh = rh;
    }
    SET_ERROR(_error_code);
    return retval;
}

void nfp_cpp_free(struct nfp_cpp * cpp)
{
    ns_ipc_buf_t *b_in = NULL, b_out;
    int _error_code = 0;

    ns_ipc_buf_init(&b_out);
    ns_ipc_buf_pack(&b_out,
                    PK_STR(__func__),
                    PK_UINT64(cpp->rh),
                    PK_END);
    if (ns_ipc_client_send_receive(cpp->ipc_h, &b_in, &b_out) < 0)
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

    if (cpp->own_handle)
        ns_ipc_client_close(cpp->ipc_h);
    nfp_cpp_free_local(cpp); /* do local handle free */
    free(cpp);
    SET_ERROR(_error_code);
}

uint32_t nfp_cpp_model(struct nfp_cpp * cpp)
{
    ns_ipc_buf_t *b_in = NULL, b_out;
    int _error_code = 0;
    uint32_t retval = 0;

    ns_ipc_buf_init(&b_out);
    ns_ipc_buf_pack(&b_out,
                    PK_STR(__func__),
                    PK_UINT64(cpp->rh),
                    PK_END);
    if (ns_ipc_client_send_receive(cpp->ipc_h, &b_in, &b_out) < 0)
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

uint16_t nfp_cpp_interface(struct nfp_cpp * cpp)
{
    ns_ipc_buf_t *b_in = NULL, b_out;
    int _error_code = 0;
    uint32_t retval = 0;

    ns_ipc_buf_init(&b_out);
    ns_ipc_buf_pack(&b_out,
                    PK_STR(__func__),
                    PK_UINT64(cpp->rh),
                    PK_END);
    if (ns_ipc_client_send_receive(cpp->ipc_h, &b_in, &b_out) < 0)
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

struct nfp_cpp_area * nfp_cpp_area_alloc(struct nfp_cpp * cpp, uint32_t cpp_id, unsigned long long address, unsigned long size)
{
    ns_ipc_buf_t *b_in = NULL, b_out;
    int _error_code = 0;
    struct nfp_cpp_area * retval = NULL;
    uint64_t rh = 0;
    uint32_t _api_version = 0;
    uint64_t inconv_address = address;
    uint64_t inconv_size = size;

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
                    PK_UINT32(0), /* api version */
                    PK_UINT64(cpp->rh),
                    PK_UINT32(cpp_id),
                    PK_UINT64(inconv_address),
                    PK_UINT64(inconv_size),
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

    if (rh && nfp_client_check_version("nfp_cpp.h", 0, 0, _api_version) < 0) {
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
        nfp_cpp_area_alloc_local(
                retval,
                cpp,
                cpp_id,
                address,
                size
                ); /* do local handle init at last minute */
        retval->rh = rh;
    }
    SET_ERROR(_error_code);
    return retval;
}

struct nfp_cpp_area * nfp_cpp_area_alloc_with_name(struct nfp_cpp * cpp, uint32_t cpp_id, const char * name, unsigned long long address, unsigned long size)
{
    ns_ipc_buf_t *b_in = NULL, b_out;
    int _error_code = 0;
    struct nfp_cpp_area * retval = NULL;
    uint64_t rh = 0;
    uint32_t _api_version = 0;
    uint64_t inconv_address = address;
    uint64_t inconv_size = size;

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
                    PK_UINT32(0), /* api version */
                    PK_UINT64(cpp->rh),
                    PK_UINT32(cpp_id),
                    PK_STR(name),
                    PK_UINT64(inconv_address),
                    PK_UINT64(inconv_size),
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

    if (rh && nfp_client_check_version("nfp_cpp.h", 0, 0, _api_version) < 0) {
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
        nfp_cpp_area_alloc_with_name_local(
                retval,
                cpp,
                cpp_id,
                name,
                address,
                size
                ); /* do local handle init at last minute */
        retval->rh = rh;
    }
    SET_ERROR(_error_code);
    return retval;
}

void nfp_cpp_area_free(struct nfp_cpp_area * area)
{
    ns_ipc_buf_t *b_in = NULL, b_out;
    int _error_code = 0;

    ns_ipc_buf_init(&b_out);
    ns_ipc_buf_pack(&b_out,
                    PK_STR(__func__),
                    PK_UINT64(area->rh),
                    PK_END);
    if (ns_ipc_client_send_receive(area->ipc_h, &b_in, &b_out) < 0)
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

    if (area->own_handle)
        ns_ipc_client_close(area->ipc_h);
    nfp_cpp_area_free_local(area); /* do local handle free */
    free(area);
    SET_ERROR(_error_code);
}

int nfp_cpp_area_acquire(struct nfp_cpp_area * area)
{
    ns_ipc_buf_t *b_in = NULL, b_out;
    int _error_code = 0;
    int32_t retval = 0;

    ns_ipc_buf_init(&b_out);
    ns_ipc_buf_pack(&b_out,
                    PK_STR(__func__),
                    PK_UINT64(area->rh),
                    PK_END);
    if (ns_ipc_client_send_receive(area->ipc_h, &b_in, &b_out) < 0)
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

void nfp_cpp_area_release(struct nfp_cpp_area * area)
{
    ns_ipc_buf_t *b_in = NULL, b_out;
    int _error_code = 0;

    ns_ipc_buf_init(&b_out);
    ns_ipc_buf_pack(&b_out,
                    PK_STR(__func__),
                    PK_UINT64(area->rh),
                    PK_END);
    if (ns_ipc_client_send_receive(area->ipc_h, &b_in, &b_out) < 0)
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

struct nfp_cpp_area * nfp_cpp_area_alloc_acquire(struct nfp_cpp * cpp, uint32_t cpp_id, unsigned long long address, unsigned long size)
{
    ns_ipc_buf_t *b_in = NULL, b_out;
    int _error_code = 0;
    struct nfp_cpp_area * retval = NULL;
    uint64_t rh = 0;
    uint32_t _api_version = 0;
    uint64_t inconv_address = address;
    uint64_t inconv_size = size;

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
                    PK_UINT32(0), /* api version */
                    PK_UINT64(cpp->rh),
                    PK_UINT32(cpp_id),
                    PK_UINT64(inconv_address),
                    PK_UINT64(inconv_size),
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

    if (rh && nfp_client_check_version("nfp_cpp.h", 0, 0, _api_version) < 0) {
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
        nfp_cpp_area_alloc_acquire_local(
                retval,
                cpp,
                cpp_id,
                address,
                size
                ); /* do local handle init at last minute */
        retval->rh = rh;
    }
    SET_ERROR(_error_code);
    return retval;
}

void nfp_cpp_area_release_free(struct nfp_cpp_area * area)
{
    ns_ipc_buf_t *b_in = NULL, b_out;
    int _error_code = 0;

    ns_ipc_buf_init(&b_out);
    ns_ipc_buf_pack(&b_out,
                    PK_STR(__func__),
                    PK_UINT64(area->rh),
                    PK_END);
    if (ns_ipc_client_send_receive(area->ipc_h, &b_in, &b_out) < 0)
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

    if (area->own_handle)
        ns_ipc_client_close(area->ipc_h);
    nfp_cpp_area_release_free_local(area); /* do local handle free */
    free(area);
    SET_ERROR(_error_code);
}

int nfp_cpp_area_read(struct nfp_cpp_area * area, unsigned long offset, void * buffer, size_t length)
{
    ns_ipc_buf_t *b_in = NULL, b_out;
    int _error_code = 0;
    int32_t retval = 0;
    void * o_buffer;
    size_t olen_buffer;
    uint64_t inconv_offset = offset;
    int64_t inconv_length = length;

    ns_ipc_buf_init(&b_out);
    ns_ipc_buf_pack(&b_out,
                    PK_STR(__func__),
                    PK_UINT64(area->rh),
                    PK_UINT64(inconv_offset),
                    PK_INT64(inconv_length),
                    PK_END);
    if (ns_ipc_client_send_receive(area->ipc_h, &b_in, &b_out) < 0)
        b_in = NULL;
    if (b_in) {
        ns_ipc_buf_unpack(b_in,
                          UP_INT32(_error_code),
                          UP_INT32(retval),
                          UP_BYTES(o_buffer, olen_buffer),
                          UP_END);

        if (b_in->error == 0) {
            if (buffer) {
                memcpy(buffer, o_buffer, olen_buffer);
            }
        } else
            NS_LOG(NS_LOG_LVL_ERROR, "error occurred during buffer unpack for nfp_cpp_area_read");
        ns_ipc_buf_done2(b_in, &b_out);
        free(b_in);
    } else {
        _error_code = EIO;
    }

    SET_ERROR(_error_code);
    return retval;
}

int nfp_cpp_area_write(struct nfp_cpp_area * area, unsigned long offset, const void * buffer, size_t length)
{
    ns_ipc_buf_t *b_in = NULL, b_out;
    int _error_code = 0;
    int32_t retval = 0;
    uint64_t inconv_offset = offset;
    int64_t inconv_length = length;

    ns_ipc_buf_init(&b_out);
    ns_ipc_buf_pack(&b_out,
                    PK_STR(__func__),
                    PK_UINT64(area->rh),
                    PK_UINT64(inconv_offset),
                    PK_BYTES(buffer, length),
                    PK_INT64(inconv_length),
                    PK_END);
    if (ns_ipc_client_send_receive(area->ipc_h, &b_in, &b_out) < 0)
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

int nfp_cpp_area_check_range(struct nfp_cpp_area * area, unsigned long long offset, unsigned long size)
{
    ns_ipc_buf_t *b_in = NULL, b_out;
    int _error_code = 0;
    int32_t retval = 0;
    uint64_t inconv_offset = offset;
    uint64_t inconv_size = size;

    ns_ipc_buf_init(&b_out);
    ns_ipc_buf_pack(&b_out,
                    PK_STR(__func__),
                    PK_UINT64(area->rh),
                    PK_UINT64(inconv_offset),
                    PK_UINT64(inconv_size),
                    PK_END);
    if (ns_ipc_client_send_receive(area->ipc_h, &b_in, &b_out) < 0)
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

int nfp_cpp_read(struct nfp_cpp * cpp, uint32_t cpp_id, unsigned long long address, void * kernel_vaddr, size_t length)
{
    ns_ipc_buf_t *b_in = NULL, b_out;
    int _error_code = 0;
    int32_t retval = 0;
    void * o_kernel_vaddr;
    size_t olen_kernel_vaddr;
    uint64_t inconv_address = address;
    int64_t inconv_length = length;

    ns_ipc_buf_init(&b_out);
    ns_ipc_buf_pack(&b_out,
                    PK_STR(__func__),
                    PK_UINT64(cpp->rh),
                    PK_UINT32(cpp_id),
                    PK_UINT64(inconv_address),
                    PK_INT64(inconv_length),
                    PK_END);
    if (ns_ipc_client_send_receive(cpp->ipc_h, &b_in, &b_out) < 0)
        b_in = NULL;
    if (b_in) {
        ns_ipc_buf_unpack(b_in,
                          UP_INT32(_error_code),
                          UP_INT32(retval),
                          UP_BYTES(o_kernel_vaddr, olen_kernel_vaddr),
                          UP_END);

        if (b_in->error == 0) {
            if (kernel_vaddr) {
                memcpy(kernel_vaddr, o_kernel_vaddr, olen_kernel_vaddr);
            }
        } else
            NS_LOG(NS_LOG_LVL_ERROR, "error occurred during buffer unpack for nfp_cpp_read");
        ns_ipc_buf_done2(b_in, &b_out);
        free(b_in);
    } else {
        _error_code = EIO;
    }

    SET_ERROR(_error_code);
    return retval;
}

int nfp_cpp_write(struct nfp_cpp * cpp, uint32_t cpp_id, unsigned long long address, const void * kernel_vaddr, size_t length)
{
    ns_ipc_buf_t *b_in = NULL, b_out;
    int _error_code = 0;
    int32_t retval = 0;
    uint64_t inconv_address = address;
    int64_t inconv_length = length;

    ns_ipc_buf_init(&b_out);
    ns_ipc_buf_pack(&b_out,
                    PK_STR(__func__),
                    PK_UINT64(cpp->rh),
                    PK_UINT32(cpp_id),
                    PK_UINT64(inconv_address),
                    PK_BYTES(kernel_vaddr, length),
                    PK_INT64(inconv_length),
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

int nfp_cpp_area_fill(struct nfp_cpp_area * area, unsigned long offset, uint32_t value, size_t length)
{
    ns_ipc_buf_t *b_in = NULL, b_out;
    int _error_code = 0;
    int32_t retval = 0;
    uint64_t inconv_offset = offset;
    int64_t inconv_length = length;

    ns_ipc_buf_init(&b_out);
    ns_ipc_buf_pack(&b_out,
                    PK_STR(__func__),
                    PK_UINT64(area->rh),
                    PK_UINT64(inconv_offset),
                    PK_UINT32(value),
                    PK_INT64(inconv_length),
                    PK_END);
    if (ns_ipc_client_send_receive(area->ipc_h, &b_in, &b_out) < 0)
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

int nfp_cpp_area_readl(struct nfp_cpp_area * area, unsigned long offset, uint32_t * value)
{
    ns_ipc_buf_t *b_in = NULL, b_out;
    int _error_code = 0;
    int32_t retval = 0;
    void * o_value;
    size_t olen_value = sizeof(uint32_t) * 1;
    uint64_t inconv_offset = offset;

    ns_ipc_buf_init(&b_out);
    ns_ipc_buf_pack(&b_out,
                    PK_STR(__func__),
                    PK_UINT64(area->rh),
                    PK_UINT64(inconv_offset),
                    PK_END);
    if (ns_ipc_client_send_receive(area->ipc_h, &b_in, &b_out) < 0)
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
            NS_LOG(NS_LOG_LVL_ERROR, "error occurred during buffer unpack for nfp_cpp_area_readl");
        ns_ipc_buf_done2(b_in, &b_out);
        free(b_in);
    } else {
        _error_code = EIO;
    }

    SET_ERROR(_error_code);
    return retval;
}

int nfp_cpp_area_writel(struct nfp_cpp_area * area, unsigned long offset, uint32_t value)
{
    ns_ipc_buf_t *b_in = NULL, b_out;
    int _error_code = 0;
    int32_t retval = 0;
    uint64_t inconv_offset = offset;

    ns_ipc_buf_init(&b_out);
    ns_ipc_buf_pack(&b_out,
                    PK_STR(__func__),
                    PK_UINT64(area->rh),
                    PK_UINT64(inconv_offset),
                    PK_UINT32(value),
                    PK_END);
    if (ns_ipc_client_send_receive(area->ipc_h, &b_in, &b_out) < 0)
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

int nfp_cpp_area_readq(struct nfp_cpp_area * area, unsigned long offset, uint64_t * value)
{
    ns_ipc_buf_t *b_in = NULL, b_out;
    int _error_code = 0;
    int32_t retval = 0;
    void * o_value;
    size_t olen_value = sizeof(uint64_t) * 1;
    uint64_t inconv_offset = offset;

    ns_ipc_buf_init(&b_out);
    ns_ipc_buf_pack(&b_out,
                    PK_STR(__func__),
                    PK_UINT64(area->rh),
                    PK_UINT64(inconv_offset),
                    PK_END);
    if (ns_ipc_client_send_receive(area->ipc_h, &b_in, &b_out) < 0)
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
            NS_LOG(NS_LOG_LVL_ERROR, "error occurred during buffer unpack for nfp_cpp_area_readq");
        ns_ipc_buf_done2(b_in, &b_out);
        free(b_in);
    } else {
        _error_code = EIO;
    }

    SET_ERROR(_error_code);
    return retval;
}

int nfp_cpp_area_writeq(struct nfp_cpp_area * area, unsigned long offset, uint64_t value)
{
    ns_ipc_buf_t *b_in = NULL, b_out;
    int _error_code = 0;
    int32_t retval = 0;
    uint64_t inconv_offset = offset;

    ns_ipc_buf_init(&b_out);
    ns_ipc_buf_pack(&b_out,
                    PK_STR(__func__),
                    PK_UINT64(area->rh),
                    PK_UINT64(inconv_offset),
                    PK_UINT64(value),
                    PK_END);
    if (ns_ipc_client_send_receive(area->ipc_h, &b_in, &b_out) < 0)
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

int nfp_xpb_writel(struct nfp_cpp * cpp, uint32_t xpb_tgt, uint32_t value)
{
    ns_ipc_buf_t *b_in = NULL, b_out;
    int _error_code = 0;
    int32_t retval = 0;

    ns_ipc_buf_init(&b_out);
    ns_ipc_buf_pack(&b_out,
                    PK_STR(__func__),
                    PK_UINT64(cpp->rh),
                    PK_UINT32(xpb_tgt),
                    PK_UINT32(value),
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

int nfp_xpb_readl(struct nfp_cpp * cpp, uint32_t xpb_tgt, uint32_t * value)
{
    ns_ipc_buf_t *b_in = NULL, b_out;
    int _error_code = 0;
    int32_t retval = 0;
    void * o_value;
    size_t olen_value = sizeof(uint32_t) * 1;

    ns_ipc_buf_init(&b_out);
    ns_ipc_buf_pack(&b_out,
                    PK_STR(__func__),
                    PK_UINT64(cpp->rh),
                    PK_UINT32(xpb_tgt),
                    PK_END);
    if (ns_ipc_client_send_receive(cpp->ipc_h, &b_in, &b_out) < 0)
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
            NS_LOG(NS_LOG_LVL_ERROR, "error occurred during buffer unpack for nfp_xpb_readl");
        ns_ipc_buf_done2(b_in, &b_out);
        free(b_in);
    } else {
        _error_code = EIO;
    }

    SET_ERROR(_error_code);
    return retval;
}

int nfp_xpb_writelm(struct nfp_cpp * cpp, uint32_t xpb_tgt, uint32_t mask, uint32_t value)
{
    ns_ipc_buf_t *b_in = NULL, b_out;
    int _error_code = 0;
    int32_t retval = 0;

    ns_ipc_buf_init(&b_out);
    ns_ipc_buf_pack(&b_out,
                    PK_STR(__func__),
                    PK_UINT64(cpp->rh),
                    PK_UINT32(xpb_tgt),
                    PK_UINT32(mask),
                    PK_UINT32(value),
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

int nfp_xpb_waitlm(struct nfp_cpp * cpp, uint32_t xpb_tgt, uint32_t mask, uint32_t value, int timeout_us)
{
    ns_ipc_buf_t *b_in = NULL, b_out;
    int _error_code = 0;
    int32_t retval = 0;
    int32_t inconv_timeout_us = timeout_us;

    ns_ipc_buf_init(&b_out);
    ns_ipc_buf_pack(&b_out,
                    PK_STR(__func__),
                    PK_UINT64(cpp->rh),
                    PK_UINT32(xpb_tgt),
                    PK_UINT32(mask),
                    PK_UINT32(value),
                    PK_INT32(inconv_timeout_us),
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

int nfp_cpp_readl(struct nfp_cpp * cpp, uint32_t cpp_id, unsigned long long address, uint32_t * value)
{
    ns_ipc_buf_t *b_in = NULL, b_out;
    int _error_code = 0;
    int32_t retval = 0;
    void * o_value;
    size_t olen_value = sizeof(uint32_t) * 1;
    uint64_t inconv_address = address;

    ns_ipc_buf_init(&b_out);
    ns_ipc_buf_pack(&b_out,
                    PK_STR(__func__),
                    PK_UINT64(cpp->rh),
                    PK_UINT32(cpp_id),
                    PK_UINT64(inconv_address),
                    PK_END);
    if (ns_ipc_client_send_receive(cpp->ipc_h, &b_in, &b_out) < 0)
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
            NS_LOG(NS_LOG_LVL_ERROR, "error occurred during buffer unpack for nfp_cpp_readl");
        ns_ipc_buf_done2(b_in, &b_out);
        free(b_in);
    } else {
        _error_code = EIO;
    }

    SET_ERROR(_error_code);
    return retval;
}

int nfp_cpp_writel(struct nfp_cpp * cpp, uint32_t cpp_id, unsigned long long address, uint32_t value)
{
    ns_ipc_buf_t *b_in = NULL, b_out;
    int _error_code = 0;
    int32_t retval = 0;
    uint64_t inconv_address = address;

    ns_ipc_buf_init(&b_out);
    ns_ipc_buf_pack(&b_out,
                    PK_STR(__func__),
                    PK_UINT64(cpp->rh),
                    PK_UINT32(cpp_id),
                    PK_UINT64(inconv_address),
                    PK_UINT32(value),
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

int nfp_cpp_readq(struct nfp_cpp * cpp, uint32_t cpp_id, unsigned long long address, uint64_t * value)
{
    ns_ipc_buf_t *b_in = NULL, b_out;
    int _error_code = 0;
    int32_t retval = 0;
    void * o_value;
    size_t olen_value = sizeof(uint64_t) * 1;
    uint64_t inconv_address = address;

    ns_ipc_buf_init(&b_out);
    ns_ipc_buf_pack(&b_out,
                    PK_STR(__func__),
                    PK_UINT64(cpp->rh),
                    PK_UINT32(cpp_id),
                    PK_UINT64(inconv_address),
                    PK_END);
    if (ns_ipc_client_send_receive(cpp->ipc_h, &b_in, &b_out) < 0)
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
            NS_LOG(NS_LOG_LVL_ERROR, "error occurred during buffer unpack for nfp_cpp_readq");
        ns_ipc_buf_done2(b_in, &b_out);
        free(b_in);
    } else {
        _error_code = EIO;
    }

    SET_ERROR(_error_code);
    return retval;
}

int nfp_cpp_writeq(struct nfp_cpp * cpp, uint32_t cpp_id, unsigned long long address, uint64_t value)
{
    ns_ipc_buf_t *b_in = NULL, b_out;
    int _error_code = 0;
    int32_t retval = 0;
    uint64_t inconv_address = address;

    ns_ipc_buf_init(&b_out);
    ns_ipc_buf_pack(&b_out,
                    PK_STR(__func__),
                    PK_UINT64(cpp->rh),
                    PK_UINT32(cpp_id),
                    PK_UINT64(inconv_address),
                    PK_UINT64(value),
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

int nfp_cpp_mutex_init(struct nfp_cpp * cpp, int target, unsigned long long address, uint32_t key_id)
{
    ns_ipc_buf_t *b_in = NULL, b_out;
    int _error_code = 0;
    int32_t retval = 0;
    int32_t inconv_target = target;
    uint64_t inconv_address = address;

    ns_ipc_buf_init(&b_out);
    ns_ipc_buf_pack(&b_out,
                    PK_STR(__func__),
                    PK_UINT64(cpp->rh),
                    PK_INT32(inconv_target),
                    PK_UINT64(inconv_address),
                    PK_UINT32(key_id),
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

struct nfp_cpp_mutex * nfp_cpp_mutex_alloc(struct nfp_cpp * cpp, int target, unsigned long long address, uint32_t key_id)
{
    ns_ipc_buf_t *b_in = NULL, b_out;
    int _error_code = 0;
    struct nfp_cpp_mutex * retval = NULL;
    uint64_t rh = 0;
    uint32_t _api_version = 0;
    int32_t inconv_target = target;
    uint64_t inconv_address = address;

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
                    PK_UINT32(0), /* api version */
                    PK_UINT64(cpp->rh),
                    PK_INT32(inconv_target),
                    PK_UINT64(inconv_address),
                    PK_UINT32(key_id),
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

    if (rh && nfp_client_check_version("nfp_cpp.h", 0, 0, _api_version) < 0) {
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
        nfp_cpp_mutex_alloc_local(
                retval,
                cpp,
                target,
                address,
                key_id
                ); /* do local handle init at last minute */
        retval->rh = rh;
    }
    SET_ERROR(_error_code);
    return retval;
}

void nfp_cpp_mutex_free(struct nfp_cpp_mutex * mutex)
{
    ns_ipc_buf_t *b_in = NULL, b_out;
    int _error_code = 0;

    ns_ipc_buf_init(&b_out);
    ns_ipc_buf_pack(&b_out,
                    PK_STR(__func__),
                    PK_UINT64(mutex->rh),
                    PK_END);
    if (ns_ipc_client_send_receive(mutex->ipc_h, &b_in, &b_out) < 0)
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

    if (mutex->own_handle)
        ns_ipc_client_close(mutex->ipc_h);
    nfp_cpp_mutex_free_local(mutex); /* do local handle free */
    free(mutex);
    SET_ERROR(_error_code);
}

int nfp_cpp_mutex_lock(struct nfp_cpp_mutex * mutex)
{
    ns_ipc_buf_t *b_in = NULL, b_out;
    int _error_code = 0;
    int32_t retval = 0;

    ns_ipc_buf_init(&b_out);
    ns_ipc_buf_pack(&b_out,
                    PK_STR(__func__),
                    PK_UINT64(mutex->rh),
                    PK_END);
    if (ns_ipc_client_send_receive(mutex->ipc_h, &b_in, &b_out) < 0)
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

int nfp_cpp_mutex_unlock(struct nfp_cpp_mutex * mutex)
{
    ns_ipc_buf_t *b_in = NULL, b_out;
    int _error_code = 0;
    int32_t retval = 0;

    ns_ipc_buf_init(&b_out);
    ns_ipc_buf_pack(&b_out,
                    PK_STR(__func__),
                    PK_UINT64(mutex->rh),
                    PK_END);
    if (ns_ipc_client_send_receive(mutex->ipc_h, &b_in, &b_out) < 0)
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

int nfp_cpp_mutex_trylock(struct nfp_cpp_mutex * mutex)
{
    ns_ipc_buf_t *b_in = NULL, b_out;
    int _error_code = 0;
    int32_t retval = 0;

    ns_ipc_buf_init(&b_out);
    ns_ipc_buf_pack(&b_out,
                    PK_STR(__func__),
                    PK_UINT64(mutex->rh),
                    PK_END);
    if (ns_ipc_client_send_receive(mutex->ipc_h, &b_in, &b_out) < 0)
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

struct nfp_cpp_event * nfp_cpp_event_alloc(struct nfp_cpp * cpp, uint32_t event_match, uint32_t event_mask, int type)
{
    ns_ipc_buf_t *b_in = NULL, b_out;
    int _error_code = 0;
    struct nfp_cpp_event * retval = NULL;
    uint64_t rh = 0;
    uint32_t _api_version = 0;
    int32_t inconv_type = type;

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
                    PK_UINT32(0), /* api version */
                    PK_UINT64(cpp->rh),
                    PK_UINT32(event_match),
                    PK_UINT32(event_mask),
                    PK_INT32(inconv_type),
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

    if (rh && nfp_client_check_version("nfp_cpp.h", 0, 0, _api_version) < 0) {
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
        nfp_cpp_event_alloc_local(
                retval,
                cpp,
                event_match,
                event_mask,
                type
                ); /* do local handle init at last minute */
        retval->rh = rh;
    }
    SET_ERROR(_error_code);
    return retval;
}

void nfp_cpp_event_free(struct nfp_cpp_event * event)
{
    ns_ipc_buf_t *b_in = NULL, b_out;
    int _error_code = 0;

    ns_ipc_buf_init(&b_out);
    ns_ipc_buf_pack(&b_out,
                    PK_STR(__func__),
                    PK_UINT64(event->rh),
                    PK_END);
    if (ns_ipc_client_send_receive(event->ipc_h, &b_in, &b_out) < 0)
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

    if (event->own_handle)
        ns_ipc_client_close(event->ipc_h);
    nfp_cpp_event_free_local(event); /* do local handle free */
    free(event);
    SET_ERROR(_error_code);
}

int nfp_cpp_rpc_version(int *cln_ver, int *cln_compat, int *srv_ver, int *srv_compat)
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
    *cln_ver = 0;
    *cln_compat = 0;
    if (*cln_compat) {
        if (*cln_ver > *srv_ver)
            return 1;
        if (*srv_ver < *cln_compat)
            return 1;
    } else if (*srv_ver != *cln_ver)
        return 1;
    return 0;
}
