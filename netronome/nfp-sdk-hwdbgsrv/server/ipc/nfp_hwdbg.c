/*
 * Copyright (C) 2014, Netronome Systems, Inc.  All rights reserved.
 */

#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <nfp_hwdbg.h>
#include "nfp_hwdbg_handles.h"

#include "dbgsrv.h"

/******************************************************************
 * Exported functions                                             *
 ******************************************************************/

/************* NFP device functions *************/

struct nfp_hwdbg *nfp_hwdbg_open(unsigned int devnum)
{
    struct nfp_hwdbg *d;

    d = (struct nfp_hwdbg *)malloc(sizeof(*d));

    if (!d) {
        errno = ENOMEM;
        return NULL;
    }

    return d;
}

/* support function for initializing + freeing nfp_hwdbg */

int _init_nfp_hwdbg_open(void *h)
{
    hwdbg_mgmt_add_client(dbgsrv_glob->hwdbg_mgmt, h);
    return 0;
}

void _free_nfp_hwdbg_open(void *h)
{
    nfp_hwdbg_close((struct nfp_hwdbg *)h);
}

void nfp_hwdbg_close(struct nfp_hwdbg *dev)
{
    hwdbg_mgmt_del_client(dbgsrv_glob->hwdbg_mgmt, dev);
    free(dev);
}

int nfp_hwdbg_client_count(struct nfp_hwdbg *dev)
{
    return hwdbg_mgmt_client_count(dbgsrv_glob->hwdbg_mgmt);
}

void nfp_hwdbg_srv_reset(struct nfp_hwdbg *dev, int resettype)
{
    hwdbg_mgmt_reset(dbgsrv_glob->hwdbg_mgmt, resettype);
}

int nfp_hwdbg_set_enabled_mes(struct nfp_hwdbg *dev,
                              int *meids,
                              int len)
{
    int ret;
    ret = hwdbg_mgmt_set_enabled_mes(dbgsrv_glob->hwdbg_mgmt, meids, len);
    if (ret < 0) {
        errno = -ret;
        return -1;
    }
    return 0;
}

int nfp_hwdbg_me_status(struct nfp_hwdbg *hwdbg,
                        int *meids,
                        struct nfp_hwdbg_me_status_data *status,
                        int len)
{
    int ret = 0;
    int i;

    if (len <= 0 || !meids || !status) {
        errno = EINVAL;
        return -1;
    }

    for (i = 0; i < len; i++) {
        ret = hwdbg_me_status(dbgsrv_glob->hwdbg, *(meids + i), status + i);
        if (ret < 0) {
            errno = -ret;
            return -1;
        }
    }

    return 0;
}

int nfp_hwdbg_me_stop(struct nfp_hwdbg *hwdbg, int meid)
{
    int ret = 0;
    ret = hwdbg_mgmt_stop(dbgsrv_glob->hwdbg_mgmt, meid);
    if (ret < 0) {
        errno = -ret;
        ret = -1;
    }
    return ret;
}

int nfp_hwdbg_me_run(struct nfp_hwdbg *hwdbg, int meid, int resume)
{
    int ret = 0;
    ret = hwdbg_mgmt_run(dbgsrv_glob->hwdbg_mgmt, meid, resume);
    if (ret < 0) {
        errno = -ret;
        ret = -1;
    }
    return ret;
}

int nfp_hwdbg_me_step(struct nfp_hwdbg *hwdbg, int meid, int ctx)
{
    int ret = 0;
    ret = hwdbg_step_me(dbgsrv_glob->hwdbg, meid, ctx);
    if (ret < 0) {
        errno = -ret;
        ret = -1;
    }
    return ret;
}

int nfp_hwdbg_brkgrp_add(struct nfp_hwdbg *hwdbg,
                         int meid,
                         int *breakmeids,
                         int len)
{
    int ret = 0;
    ret = hwdbg_brkgrp_add(dbgsrv_glob->hwdbg, meid, breakmeids, len);
    if (ret < 0) {
        errno = -ret;
        ret = -1;
    }
    return ret;
}

int nfp_hwdbg_brkgrp_del(struct nfp_hwdbg *hwdbg,
                         int meid)
{
    int ret = 0;
    ret = hwdbg_brkgrp_del(dbgsrv_glob->hwdbg, meid);
    if (ret < 0) {
        errno = -ret;
        ret = -1;
    }
    return ret;
}

int nfp_hwdbg_brk_add_code(struct nfp_hwdbg *hwdbg,
                           int64_t breakid,
                           int meid,
                           int pc,
                           unsigned int ctxmask)
{
    int ret = 0;
    ret = hwdbg_mgmt_add_codebp(dbgsrv_glob->hwdbg_mgmt, hwdbg,
                                breakid, meid, pc, ctxmask);
    if (ret < 0) {
        errno = -ret;
        ret = -1;
    }
    return ret;
}

int nfp_hwdbg_brk_del(struct nfp_hwdbg *hwdbg, int64_t breakid)
{
    int ret = 0;
    ret = hwdbg_mgmt_del_codebp(dbgsrv_glob->hwdbg_mgmt, hwdbg, breakid);
    if (ret < 0) {
        errno = -ret;
        ret = -1;
    }
    return ret;
}

int nfp_hwdbg_brk_flush(struct nfp_hwdbg *hwdbg, int flushtype)
{
    int ret = 0;
    void *h = flushtype == NFP_HWDBG_FLUSH_TYPE_ALL ? NULL : hwdbg;
    ret = hwdbg_mgmt_flush_codebp(dbgsrv_glob->hwdbg_mgmt, h);
    if (ret < 0) {
        errno = -ret;
        ret = -1;
    }
    return ret;
}

int nfp_hwdbg_brk_check(struct nfp_hwdbg *hwdbg,
                        struct nfp_hwdbg_break_data *data,
                        int len,
                        int *cnt)
{
    int ret = 0;
    ret = hwdbg_mgmt_retrieve(dbgsrv_glob->hwdbg_mgmt, data, len, cnt);
    if (ret < 0) {
        errno = -ret;
        ret = -1;
    }
    return ret;
}

int nfp_hwdbg_brk_clear(struct nfp_hwdbg *hwdbg, int64_t breakid)
{
    int ret = 0;
    ret = hwdbg_mgmt_clear(dbgsrv_glob->hwdbg_mgmt, breakid);
    if (ret < 0) {
        errno = -ret;
        ret = -1;
    }
    return ret;
}

int nfp_hwdbg_load_dbginfo(struct nfp_hwdbg *hwdbg,
                           int meid,
                           uint64_t *data,
                           int len,
                           int off)
{
    int ret = 0;
    ret = hwdbg_add_dbginfo(dbgsrv_glob->hwdbg, meid, data, len, off);
    if (ret < 0) {
        errno = -ret;
        ret = -1;
    }
    return ret;
}


int nfp_hwdbg_server_version(struct nfp_hwdbg *hwdbg, char *buf, int len)
{
    int l;
    const char *appname = "debug server:";
    if (!hwdbg || !buf) {
        errno = EINVAL;
        return -1;
    }
    if (!dbgsrv_glob->version) {
        errno = ENOENT;
        return -1;
    }
    l = strlen(appname) + 1 + strlen(dbgsrv_glob->version) + 1;
    if (len <= 0)
        return l;
    if (l > len) {
        NS_LOG(NS_LOG_LVL_WARN, "Not enough space for version string");
        errno = ENOSPC;
        return -1;
    }
    snprintf(buf, l, "%s %s", appname, dbgsrv_glob->version);
    return l;
}
