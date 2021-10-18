/*
 * Copyright (C) 2014, Netronome Systems, Inc.  All rights reserved.
 */

#include <errno.h>
#include <string.h>
#include <nfp.h>
#include <nfp_internal.h>
#include <nfp_bulk.h>
#include <nfp_explicit.h>
#include <nfp_cpp.h>
#include <nfp_cpp_internal.h>
#include <nfp_hwinfo.h>
#include "logging.h"

/* support functions for initializing and freeing nfp + nfp_cpp handles */

int _init_nfp_device_open(void *h)
{
    return 0;
}
void _free_nfp_device_open(void *h)
{
    nfp_device_close((struct nfp_device *)h);
}

int _init_nfp_cpp_open(void *h)
{
    return 0;
}
void _free_nfp_cpp_open(void *h)
{
    nfp_cpp_free((struct nfp_cpp *)h);
}

int _init_nfp_cpp_from_device_id(void *h)
{
    return 0;
}
void _free_nfp_cpp_from_device_id(void *h)
{
    nfp_cpp_free((struct nfp_cpp *)h);
}

int _init_nfp_cpp_area_alloc(void *h)
{
    return 0;
}
void _free_nfp_cpp_area_alloc(void *h)
{
    nfp_cpp_area_release_free((struct nfp_cpp_area *)h);
}

int _init_nfp_cpp_area_alloc_with_name(void *h)
{
    return 0;
}
void _free_nfp_cpp_area_alloc_with_name(void *h)
{
    nfp_cpp_area_release_free((struct nfp_cpp_area *)h);
}

int _init_nfp_cpp_area_alloc_acquire(void *h)
{
    return 0;
}
void _free_nfp_cpp_area_alloc_acquire(void *h)
{
    nfp_cpp_area_release_free((struct nfp_cpp_area *)h);
}

int _init_nfp_device_from_cpp(void *h)
{
    return 0;
}

void _free_nfp_device_from_cpp(void *h)
{
    nfp_device_close((struct nfp_device *)h);
}

int _init_nfp_cpp_event_alloc(void *h)
{
    return 0;
}

void _free_nfp_cpp_event_alloc(void *h)
{
    nfp_cpp_event_free((struct nfp_cpp_event *)h);
}

int _init_nfp_cpp_mutex_alloc(void *h)
{
    return 0;
}

void _free_nfp_cpp_mutex_alloc(void *h)
{
    nfp_cpp_mutex_free((struct nfp_cpp_mutex *)h);
}

/* Bulk operations */

int nfp_bulk_mecsr_read(struct nfp_device *dev,
                        uint32_t *mecsrs,
                        uint32_t *values,
                        int len)
{
    int i;
    int ret;
    int ctx, csr, meid;

    if (len <= 0 || !mecsrs || !values) {
        errno = EINVAL;
        return -1;
    }

    for (i = 0; i < len; i++) {
        meid = NFP_BULK_MECSR_MEID_of(mecsrs[i]);
        ctx = NFP_BULK_MECSR_CTX_of(mecsrs[i]);
        csr = NFP_BULK_MECSR_CSR_of(mecsrs[i]);
        ret = nfp_mecsr_read(dev, meid, ctx, csr, values + i);
        if (ret < 0) /* errno set already */
            return -1;
    }
    return 0;
}

int nfp_bulk_mecsr_write(struct nfp_device *dev,
                         uint32_t *mecsrs,
                         uint32_t *values,
                         int len)
{
    int i;
    int ret;
    int ctx, csr, meid;

    if (len <= 0 || !mecsrs || !values) {
        errno = EINVAL;
        return -1;
    }

    for (i = 0; i < len; i++) {
        meid = NFP_BULK_MECSR_MEID_of(mecsrs[i]);
        ctx = NFP_BULK_MECSR_CTX_of(mecsrs[i]);
        csr = NFP_BULK_MECSR_CSR_of(mecsrs[i]);
        ret = nfp_mecsr_write(dev, meid, ctx, csr, *(values + i));
        if (ret < 0) /* errno set already */
            return -1;
    }
    return 0;
}

int nfp_bulk_me_register_read(struct nfp_device *dev,
                              unsigned int meid,
                              unsigned int reg,
                              uint32_t *values,
                              int count)
{
    int type, off;
    int i;
    int ret;

    if (count <= 0 || !values) {
        errno = EINVAL;
        return -1;
    }

    type = (reg >> 16) & 0xff;
    off = (reg & 0xffff) >> 2;
    for (i = 0; i < count; i++, off++, values++) {
        ret = nfp_me_register_read(dev, meid, nfp_mereg(type, off), values);
        if (ret < 0) /* errno set already */
            return -1;
    }
    return 0;
}

int nfp_bulk_me_register_write(struct nfp_device *dev,
                               unsigned int meid,
                               unsigned int reg,
                               uint32_t *values,
                               int count)
{
    int type, off;
    int i;
    int ret;

    if (count <= 0 || !values) {
        errno = EINVAL;
        return -1;
    }

    type = (reg >> 16) & 0xff;
    off = (reg & 0xffff) >> 2;
    for (i = 0; i < count; i++, off++, values++) {
        ret = nfp_me_register_write(dev, meid, nfp_mereg(type, off), *values);
        if (ret < 0) /* errno set already */
            return -1;
    }
    return 0;
}

/* bulk apis */
int nfp_bulk_xpb_read(struct nfp_cpp *cpp,
                      uint32_t *xpb_tgts,
                      uint32_t *values,
                      int len)
{
    int i;
    if (!cpp || !xpb_tgts || !values || len <= 0) {
        errno = EINVAL;
        return -1;
    }

    for (i = 0; i < len; i++) {
        if (nfp_xpb_readl(cpp, xpb_tgts[i], &values[i]) < 0) {
            NS_LOG(NS_LOG_LVL_WARN,
                   "xpb bulk read failed on target %x", xpb_tgts[i]);
            return -1;
        }
    }
    return 0;
}

int nfp_bulk_xpb_write(struct nfp_cpp *cpp,
                       uint32_t *xpb_tgts,
                       uint32_t *values,
                       int len)
{
    int i;
    if (!cpp || !xpb_tgts || !values || len <= 0) {
        errno = EINVAL;
        return -1;
    }

    for (i = 0; i < len; i++) {
        if (nfp_xpb_writel(cpp, xpb_tgts[i], values[i]) < 0) {
            NS_LOG(NS_LOG_LVL_WARN,
                   "xpb bulk write failed on target %x", xpb_tgts[i]);
            return -1;
        }
    }
    return 0;
}

#ifndef SDK_SIM

/* Privated RPC'ed wrapper functions */

/* we cant implement nfp_hwinfo_* apis with RPC, so we have a local
 * which uses this rpc friendly implementation
 */
int nfp_get_hwinfo(struct nfp_device *dev, void *buffer, int length)
{
    const char *key, *value;
    char *s = buffer;
    int entries;
    int i, n;

    entries = nfp_hwinfo_entries(dev);

    if (length <= 0) {
        n = 0;
        for (i = 0; i < entries; i++) {
            if (nfp_hwinfo_entry(dev, i, &key, &value) < 0) {
                NS_LOG(NS_LOG_LVL_WARN, "failed to get hwinfo");
                errno = EIO;
                return -1;
            }
            n += strlen(key) + strlen(value) + 2;
        }
        return n;
    }

    for (i = 0; i < entries; i++) {
        if (nfp_hwinfo_entry(dev, i, &key, &value) < 0) {
            NS_LOG(NS_LOG_LVL_WARN, "failed to get hwinfo");
            errno = EIO;
            return -1;
        }

        if (length < strlen(key) + strlen(value) + 2) {
            NS_LOG(NS_LOG_LVL_WARN, "truncated hwinfo");
            break;
        }
        memcpy(s, key, strlen(key) + 1);
        s += strlen(key) + 1;
        length -= strlen(key) + 1;
        memcpy(s, value, strlen(value) + 1);
        s += strlen(value) + 1;
        length -= strlen(value) + 1;
    }
    /* we return the number of entries populated */
    return i;
}

/* */
int nfp_cpp_explicit_do_internal(struct nfp_cpp *cpp,
                                 struct nfp_cpp_explicit_data_internal *cppdata,
                                 void *push_buf,
                                 size_t buf_len,
                                 int *push_len)
{
    struct nfp_cpp_explicit *expl;
    int ret;

    expl = nfp_cpp_explicit_acquire(cpp);
    if (!expl) {
        NS_LOG(NS_LOG_LVL_WARN,
                "error: internal nfp_cpp_explicit_acquire failed (%s)",
                strerror(errno));
        return -1;
    }

    ret = nfp_cpp_explicit_set_target(expl,
                                      cppdata->cpp_id,
                                      cppdata->length,
                                      cppdata->bytemask);
    if (ret < 0) {
        NS_LOG(NS_LOG_LVL_WARN,
                "error: internal nfp_cpp_explicit_set_target failed (%s)",
                strerror(errno));
        nfp_cpp_explicit_release(expl);
        return -1;
    }

    ret = nfp_cpp_explicit_set_data(expl,
                                    cppdata->data_master,
                                    cppdata->data_ref);
    if (ret < 0) {
        NS_LOG(NS_LOG_LVL_WARN,
                "error: internal nfp_cpp_explicit_set_data failed (%s)",
                strerror(errno));
        nfp_cpp_explicit_release(expl);
        return -1;
    }

    ret = nfp_cpp_explicit_set_signal(expl,
                                      cppdata->signal_master,
                                      cppdata->signal_ref);
    if (ret < 0) {
        NS_LOG(NS_LOG_LVL_WARN,
                "error: internal nfp_cpp_explicit_set_signal failed (%s)",
                strerror(errno));
        return -1;
    }

    ret = nfp_cpp_explicit_set_posted(expl,
                                      cppdata->posted,
                                      cppdata->siga,
                                      cppdata->siga_mode,
                                      cppdata->sigb,
                                      cppdata->sigb_mode);
    if (ret < 0) {
        NS_LOG(NS_LOG_LVL_WARN,
                "error: internal nfp_cpp_explicit_set_posted failed (%s)",
                strerror(errno));
        return -1;
    }

    ret = nfp_cpp_explicit_put(expl, cppdata->pull_data, cppdata->pull_len);
    if (ret < 0) {
        NS_LOG(NS_LOG_LVL_WARN,
                "error: internal nfp_cpp_explicit_put failed (%s)",
                strerror(errno));
        return -1;
    }

    ret = nfp_cpp_explicit_get(expl, push_buf, buf_len);
    if (ret < 0) {
        NS_LOG(NS_LOG_LVL_WARN,
                "error: internal nfp_cpp_explicit_get failed (%s)",
                strerror(errno));
        return -1;
    }

    *push_len = buf_len;

    return 0;
}

/* some older BSP didn't have the 64bit implementations
 * define BSP_NO_MEM64 to support those
 */
#ifdef BSP_NO_MEM64

ssize_t nfp_imem64_read(struct nfp_device * dev,
                      int islnum,
                      void * buffer,
                      unsigned long long length,
                      unsigned long long offset)
{
    return nfp_imem_read(dev, islnum, buffer, length, offset);
}

ssize_t nfp_imem64_write(struct nfp_device * dev,
                         int islnum,
                         const void * buffer,
                         unsigned long long length,
                         unsigned long long offset)
{
    return nfp_imem_write(dev, islnum, buffer, length, offset);
}

ssize_t nfp_imem64_fill(struct nfp_device * dev,
                        int islnum,
                        const void * buffer,
                        unsigned long long buf_length,
                        unsigned long long offset,
                        unsigned long long write_length)
{
    return nfp_imem_fill(dev, islnum, buffer, buf_length, offset, write_length);
}

ssize_t nfp_ctm64_read(struct nfp_device * dev,
                      int islnum,
                      void * buffer,
                      unsigned long long length,
                      unsigned long long offset)
{
    return nfp_ctm_read(dev, islnum, buffer, length, offset);
}

ssize_t nfp_ctm64_write(struct nfp_device * dev,
                         int islnum,
                         const void * buffer,
                         unsigned long long length,
                         unsigned long long offset)
{
    return nfp_ctm_write(dev, islnum, buffer, length, offset);
}

ssize_t nfp_ctm64_fill(struct nfp_device * dev,
                        int islnum,
                        const void * buffer,
                        unsigned long long buf_length,
                        unsigned long long offset,
                        unsigned long long write_length)
{
    return nfp_ctm_fill(dev, islnum, buffer, buf_length, offset, write_length);
}

#endif

#endif /* SDK_SIM */
