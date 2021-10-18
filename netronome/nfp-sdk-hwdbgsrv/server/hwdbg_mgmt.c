/*
 * Copyright (C) 2014, Netronome Systems, Inc.  All rights reserved.
 */

#include <errno.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <pthread.h>

#include "dbgsrv.h"

/********** Support functions ************/

static int codebp_exists(struct hwdbg_mgmt_handle *hm, int64_t breakid)
{
    struct hwdbg_mgmt_break_entry *e;
    /* lock must be active */

    for (e = hm->code_breaks; e; e = e->next) {
        if (e->breakid == breakid)
            return 1;
    }
    return 0;
}

static int send_event(struct hwdbg_mgmt_handle *hm,
                      void *client_handle,
                      int type,
                      void *data,
                      int len)
{
    /* lock must be active */
    ns_ipc_buf_clear(&hm->ipc_buf);
    ns_ipc_buf_pack(&hm->ipc_buf,
            PK_UINT32(hm->ipc_seq++),
            PK_UINT32(type),
            PK_BYTES(data, len), PK_END);
    return ns_ipc_server_send_event(&dbgsrv_glob->ipc_h,
                                    client_handle,
                                    &hm->ipc_buf);
}

static void send_event_all(struct hwdbg_mgmt_handle *hm,
                           int type,
                           void *data,
                           int len)
{
    struct hwdbg_mgmt_client_handle_entry *e;
    /* lock must be active */

    for (e = hm->client_handles; e; e = e->next) {
        send_event(hm, e->client_conn_handle, type, data, len);
    }
}

void hwdbg_mgmt_send_event(struct hwdbg_mgmt_handle *hm,
                          int type, void *data, int len)
{
    send_event_all(hm, type, data, len);
}

/************ Add/Clear breaks **************/

static int add_codebreak(struct hwdbg_mgmt_handle *hm,
                         struct hwdbg_mgmt_break_entry *br)
{
    struct hwdbg_mgmt_break_entry *b;
    /* check if there are any other code breaks which use the same pc */
    for (b = hm->code_breaks; b; b = b->next) {
        if (b->meid != br->meid)
            continue;
        if (b->pc != br->pc)
            continue;
        /* there is already a sw bpt at that location so do nothing */
        return 0;
    }
    return hwdbg_add_codebp(dbgsrv_glob->hwdbg, br->meid, br->pc);
}

static int del_codebreak(struct hwdbg_mgmt_handle *hm,
                         struct hwdbg_mgmt_break_entry *br)
{
    struct hwdbg_mgmt_break_entry *b;

    /* check if there are any other code breaks which use the same pc */
    for (b = hm->code_breaks; b; b = b->next) {
        if (b->meid != br->meid)
            continue;
        if (b->pc != br->pc)
            continue;
        if (b == br) /* dont count the one we are deleting */
            continue;

        /* there is another codebp at that location so don't touch the ucode */
        return 0;
    }
    return hwdbg_del_codebp(dbgsrv_glob->hwdbg, br->meid, br->pc);
}

static int clear_activebreak(struct hwdbg_mgmt_handle *hm,
                             struct hwdbg_mgmt_break_entry *br)
{
    struct hwdbg_mgmt_break_entry **e, *p;
    int found = 0;
    for (e = &hm->active_breaks; *e; e = &(*e)->next) {
        if ((*e)->breakid != br->breakid)
            continue;
        found = 1;
        p = *e;
        *e = (*e)->next;
        break;
    }

    if (!found) {
        /* we don't log anything here as this can be done speculatively */
        return -1;
    }

    /* insert the active break into the cleared breaks list */
    p->next = hm->cleared_breaks;
    hm->cleared_breaks = p;

    return 0;
}

static void add_active_break(struct hwdbg_mgmt_handle *hm,
                             struct nfp_hwdbg_break_data *bdata)
{
    /* lock must be active */
    struct hwdbg_mgmt_break_entry *e;

    e = malloc(sizeof(*e));

    if (!e) {
        NS_LOG(NS_LOG_LVL_ERROR,
               "Failed to allocate active break entry");
        return;
    }

    e->breakid = bdata->breakid;
    e->meid = bdata->meid;
    e->ctx = bdata->ctx;
    e->pc = bdata->pc;
    e->client_handle = NULL;
    /* add to front */
    e->next = hm->active_breaks;
    hm->active_breaks = e;
}

int hwdbg_mgmt_codebp_matches(struct hwdbg_mgmt_handle *hm,
                              int meid, int ctx, int pc)
{
    struct hwdbg_mgmt_break_entry *e;
    int found = 0;
    /* note: hm->mtx mutex must be locked */

    for (e = hm->code_breaks; e; e = e->next) {
        if (e->meid != meid)
            continue;
        /* check against the mask */
        if (!(e->ctx & (1 << ctx)))
            continue;
        if (e->pc != pc)
            continue;
        found = 1;
        break;
    }

    return found;
}

int hwdbg_mgmt_handle_swbrk(struct hwdbg_mgmt_handle *hm,
                            int meid,
                            int ctx,
                            int pc,
                            int isswbrk)
{
    struct nfp_hwdbg_break_data bdata;
    struct hwdbg_mgmt_break_entry *e;
    int n = 0; /* count the number of matched breaks */
    /* note: hm->mtx mutex must be locked */

    NS_LOG(NS_LOG_LVL_DEBUG,
            "Handling breakpoint on me%x:ctx%d:pc%d",
            meid, ctx, pc);

    if (isswbrk) { /* the bpt was a user code ctx_arb[bpt] */
        bdata.meid = meid;
        bdata.ctx = ctx;
        bdata.pc = pc;
        bdata.breakid = -1LL - (hm->softbreakno++);
        send_event_all(hm, NFP_HWDBG_EVENT_TYPE_BREAK, &bdata, sizeof(bdata));
        add_active_break(hm, &bdata);
        n++;
        NS_LOG(NS_LOG_LVL_DEBUG,
               "Software breakpoint ID 0x%lx me%x:ctx%d:pc%d",
               bdata.breakid, meid, ctx, pc);
    }

    for (e = hm->code_breaks; e; e = e->next) {
        if (e->meid != meid)
            continue;
        /* check against the mask */
        if (!(e->ctx & (1 << ctx)))
            continue;
        if (e->pc != pc)
            continue;

        bdata.meid = meid;
        bdata.ctx = ctx;
        bdata.pc = pc;
        bdata.breakid = e->breakid;
        send_event_all(hm, NFP_HWDBG_EVENT_TYPE_BREAK, &bdata, sizeof(bdata));
        add_active_break(hm, &bdata);
        n++;
        NS_LOG(NS_LOG_LVL_DEBUG,
               "Code breakpoint ID 0x%lx me%x:ctx%d:pc%d",
               bdata.breakid, meid, ctx, pc);
    }

    return n;
}

/******* Code break add / delete ********/

/* add a code breakpoint */
int hwdbg_mgmt_add_codebp(struct hwdbg_mgmt_handle *hm,
                          void *client_handle, /* client connection handle */
                          int64_t breakid,
                          int meid, int pc, unsigned int ctxmask)
{
    struct hwdbg_mgmt_break_entry *e;
    int ret;
    if (breakid < 0) {
        NS_LOG(NS_LOG_LVL_WARN,
               "Attempted to add breakpoint with invalid ID 0x%lx", breakid);
        return -EINVAL;
    }

    if (!nfp_chipdata_has_me(dbgsrv_glob->chip, meid)) {
        NS_LOG(NS_LOG_LVL_WARN,
               "Couldn't find MEID %x for breakpoint ID 0x%lx",
               meid, breakid);
        return -ENOENT;
    }

    if (pc < 0 || pc >= CSLEN * 2) {
        NS_LOG(NS_LOG_LVL_WARN,
               "Attempted to add breakpoint ID 0x%lx with invalid pc %d",
               breakid, pc);
        return -EINVAL;
    }

    /* 0 means all contexts too */
    if (ctxmask == 0)
        ctxmask = 0xff;

    if (ctxmask > 0xff) {
        NS_LOG(NS_LOG_LVL_WARN,
               "Attempted to add breakpoint ID 0x%lx with invalid ctxmask 0x%x",
               breakid, ctxmask);
        return -EINVAL;
    }

    if (pthread_mutex_lock(hm->mtx) < 0) {
        NS_LOG(NS_LOG_LVL_ERROR, "Failed to acquire lock while adding client");
        return -EIO;
    }

    if (codebp_exists(hm, breakid)) {
        pthread_mutex_unlock(hm->mtx);
        NS_LOG(NS_LOG_LVL_ERROR,
               "Attempted to add breakpoint with duplicate ID 0x%lx", breakid);
        return -EEXIST;
    }

    e = malloc(sizeof(*e));
    if (!e) {
        pthread_mutex_unlock(hm->mtx);
        NS_LOG(NS_LOG_LVL_ERROR,
               "Failed to allocate breakpoint with ID 0x%lx", breakid);
        return -ENOMEM;
    }

    e->breakid = breakid;
    e->meid = meid;
    e->ctx = ctxmask;
    e->pc = pc;
    e->client_handle = client_handle;
    e->next = hm->code_breaks;

    ret = add_codebreak(hm, e);
    if (ret < 0) {
        free(e);
        pthread_mutex_unlock(hm->mtx);
        NS_LOG(NS_LOG_LVL_ERROR,
               "Failed to add code breakpoint with ID 0x%lx", breakid);
        return ret;
    }

    hm->code_breaks = e;

    pthread_mutex_unlock(hm->mtx);
    NS_LOG(NS_LOG_LVL_DEBUG,
           "Added breakpoint with ID 0x%lx me%x:pc%d:ctx%x",
            breakid, meid, pc, ctxmask);
    return 0;
}

/* delete a code breakpoint */
int hwdbg_mgmt_del_codebp(struct hwdbg_mgmt_handle *hm,
                          void *client_handle, /* client connection handle */
                          int64_t breakid)
{
    struct hwdbg_mgmt_break_entry **e, *p;

    if (pthread_mutex_lock(hm->mtx) < 0) {
        NS_LOG(NS_LOG_LVL_ERROR, "Failed to acquire lock while adding client");
        return -EIO;
    }

    for (e = &hm->code_breaks; *e; e = &(*e)->next) {
        if ((*e)->breakid != breakid)
            continue;

        if (hwdbg_me_running(dbgsrv_glob->hwdbg, (*e)->meid)) {
            pthread_mutex_unlock(hm->mtx);
            NS_LOG(NS_LOG_LVL_ERROR,
                   "Could not delete codeBP on running me%x", (*e)->meid);
            return -EBUSY;
        }

        /* speculatively clear any active break associated with the code bp */
        clear_activebreak(hm, *e);
        /* delete the code breakpoint */
        del_codebreak(hm, *e);
        /* remove from list */
        p = *e;
        *e = ((*e)->next);
        free(p);
        pthread_mutex_unlock(hm->mtx);
        NS_LOG(NS_LOG_LVL_DEBUG, "Deleted breakpoint with ID 0x%lx", breakid);
        return 0;
    }

    pthread_mutex_unlock(hm->mtx);
    NS_LOG(NS_LOG_LVL_ERROR,
            "Attempted to delete non-existant breakpoint with ID 0x%lx",
            breakid);
    return -ENOENT;
}

/****** Retrieve and clear breaks *******/

int hwdbg_mgmt_flush_codebp(struct hwdbg_mgmt_handle *hm, void *client_handle)
{
    struct hwdbg_mgmt_break_entry **b, *p;
    int n = 0;

    if (pthread_mutex_lock(hm->mtx) < 0) {
        NS_LOG(NS_LOG_LVL_ERROR, "Failed to acquire lock while adding client");
        return -EIO;
    }

    b = &hm->code_breaks;
    while (*b) {
        /* if we are matching client_handles, check the match */
        if (client_handle && client_handle != (*b)->client_handle) {
            /* dont delete */
            b = &(*b)->next;
            continue;
        }
        clear_activebreak(hm, *b);
        if (hwdbg_me_running(dbgsrv_glob->hwdbg, (*b)->meid)) {
            NS_LOG(NS_LOG_LVL_DEBUG, "Stopping ME to remove codeBP");
            if (hwdbg_stop_me(dbgsrv_glob->hwdbg, (*b)->meid, 0) < 0) {
                NS_LOG(NS_LOG_LVL_WARN, "Failed to stop ME");
            } else {
                del_codebreak(hm, *b);
                hwdbg_run_me(dbgsrv_glob->hwdbg, (*b)->meid, 0);
            }
        } else
            del_codebreak(hm, *b);
        /* remove the entry */
        p = *b;
        *b = p->next;
        free(p);
        n++;
    }
    pthread_mutex_unlock(hm->mtx);
    NS_LOG(NS_LOG_LVL_DEBUG, "Flushed %d code breaks", n);
    if (n == 0)
        return -ENOENT;

    return 0;
}

int hwdbg_mgmt_retrieve(struct hwdbg_mgmt_handle *hm,
                        struct nfp_hwdbg_break_data *data, int len, int *cnt)
{
    struct hwdbg_mgmt_break_entry *e;
    int n;
    int ret = 0;

    *cnt = 0;

    if (pthread_mutex_lock(hm->mtx) < 0) {
        NS_LOG(NS_LOG_LVL_ERROR, "Failed to acquire lock while adding client");
        return -EIO;
    }

    /* if a zero sized buffer is presented, just return the count */
    if (len == 0) {
        n = 0;
        for (e = hm->active_breaks; e; e = e->next) {
            n++;
        }
        *cnt = n;
        pthread_mutex_unlock(hm->mtx);
        return 0;
    }

    n = 0;
    for (e = hm->active_breaks; e; e = e->next, n++) {
        if (n >= len) {
            ret = 1;
            break;
        }
        data[n].breakid = e->breakid;
        data[n].meid = e->meid;
        data[n].ctx = e->ctx;
        data[n].pc = e->pc;
    }

    *cnt = n;
    pthread_mutex_unlock(hm->mtx);

    return ret;
}

/* clear an active break */
int hwdbg_mgmt_clear(struct hwdbg_mgmt_handle *hm, int64_t breakid)
{
    struct hwdbg_mgmt_break_entry *e, *en;
    int ret = 0;
    int found = 0;

    NS_LOG(NS_LOG_LVL_DEBUG, "Clearing break with ID %lx", breakid);

    if (pthread_mutex_lock(hm->mtx) < 0) {
        NS_LOG(NS_LOG_LVL_ERROR, "Failed to acquire lock while clearing break");
        return -EIO;
    }

    for (e = hm->active_breaks; e; e = en) {
        en = e->next;
        if (e->breakid != breakid)
            continue;
        if (clear_activebreak(hm, e) < 0) {
            NS_LOG(NS_LOG_LVL_WARN,
                   "Error while clearing break with id %lx", breakid);
            ret = -EIO;
        }
        found = 1;
        break;
    }
    pthread_mutex_unlock(hm->mtx);

    if (!found)
        ret = -ENOENT;

    return ret;
}

/************** Run control ****************/

int hwdbg_mgmt_stop(struct hwdbg_mgmt_handle *hm, int meid)
{
    int f, i;
    int ret = 0;

    /* if meid <= 0 we stop all MEs */
    f = meid > 0 ? meid : nfp_chipdata_meid_first(dbgsrv_glob->chip);
    for (i = f; i > 0;
         i = meid >= 0 ? 0 : nfp_chipdata_meid_next(dbgsrv_glob->chip, i)) {
        ret = hwdbg_stop_me(dbgsrv_glob->hwdbg, i, meid <= 0);
        if (ret > 0) {
            ret = -EBUSY;
        }
        if (ret < 0)
            break;
    }
    return ret;
}

static void remove_clear_by_meid(struct hwdbg_mgmt_handle *hm, int meid)
{
    struct hwdbg_mgmt_break_entry **e, *p;

    e = &hm->cleared_breaks;
    while (*e) {
        if ((*e)->meid == meid) {
            p = *e;
            *e = (*e)->next;
            free(p);
        } else
            e = &(*e)->next;
    }
}

static int me_has_active_break(struct hwdbg_mgmt_handle *hm, int meid)
{
    struct hwdbg_mgmt_break_entry *e;
    for (e = hm->active_breaks; e; e = e->next) {
        if (e->meid != meid)
            continue;
        return 1;
    }
    return 0;
}

int hwdbg_mgmt_run(struct hwdbg_mgmt_handle *hm, int meid, int doclear)
{
    struct hwdbg_mgmt_break_entry *e, *en;
    int f, i, n;

    if (meid > 0 && !nfp_chipdata_has_me(dbgsrv_glob->chip, meid)) {
        NS_LOG(NS_LOG_LVL_WARN, "me%x doesn't exist on chip", meid);
        return -ENOENT;
    }

    if (pthread_mutex_lock(hm->mtx) < 0) {
        NS_LOG(NS_LOG_LVL_ERROR, "Failed to acquire lock while running ME");
        return -EIO;
    }

    /* go through either all the MEs or just the one specified */
    n = 0;
    f = meid > 0 ? meid : nfp_chipdata_meid_first(dbgsrv_glob->chip);
    for (i = f; i > 0;
            i = meid >= 0 ? 0 : nfp_chipdata_meid_next(dbgsrv_glob->chip, i)) {
        if (me_has_active_break(hm, i)) {
            if (!doclear)
                continue;
            /* put all matching active breaks on the clear list */
            for (e = hm->active_breaks; e; e = en) {
                en = e->next;
                if (e->meid != i)
                    continue;
                clear_activebreak(hm, e);
            }
        }
        NS_LOG(NS_LOG_LVL_DEBUG, "running me%x", i);
        hwdbg_run_me(dbgsrv_glob->hwdbg, i, meid <= 0);
        remove_clear_by_meid(hm, i);
        n++;
    }

    pthread_mutex_unlock(hm->mtx);
    if (!n) {
        if (meid < 0)
            NS_LOG(NS_LOG_LVL_WARN, "Could not resume any MEs");
        else
            NS_LOG(NS_LOG_LVL_WARN,
                   "Attempted to resume me%x with active breaks", meid);
        return -EBUSY;
    }

    return 0;
}

/********* Client management ************/

int hwdbg_mgmt_add_client(struct hwdbg_mgmt_handle *hm,
                          void *client_handle)
{
    struct hwdbg_mgmt_client_handle_entry *e;

    if (pthread_mutex_lock(hm->mtx) < 0) {
        NS_LOG(NS_LOG_LVL_ERROR, "Failed to acquire lock while adding client");
        return -1;
    }

    for (e = hm->client_handles; e; e = e->next) {
        if (e->client_handle == client_handle) {
            pthread_mutex_unlock(hm->mtx);
            return -1;
        }
    }

    e = malloc(sizeof(*e));
    if (!e) {
        pthread_mutex_unlock(hm->mtx);
        NS_LOG(NS_LOG_LVL_ERROR, "Failed to allocate memory for client data");
        return -1;
    }
    e->client_handle = client_handle;
    /* store the client connection info too - use this to send events */
    e->client_conn_handle = ipc_client_get_con(dbgsrv_glob->ipcclient,
                                               client_handle);
    e->next = hm->client_handles;
    hm->client_handles = e;

    pthread_mutex_unlock(hm->mtx);
    NS_LOG(NS_LOG_LVL_DEBUG, "Added client %p", client_handle);
    return 0;
}

int hwdbg_mgmt_del_client(struct hwdbg_mgmt_handle *hm,
                          void *client_handle)
{
    struct hwdbg_mgmt_client_handle_entry **he, *hp;
    struct hwdbg_mgmt_break_entry **be, *bp;
    int64_t breakid;
    int found = 0;

    if (pthread_mutex_lock(hm->mtx) < 0) {
        NS_LOG(NS_LOG_LVL_ERROR, "Failed to acquire lock while adding client");
        return -1;
    }

    for (he = &hm->client_handles; *he; he = &(*he)->next) {
        if ((*he)->client_handle == client_handle) {
            hp = *he;
            *he = (*he)->next;
            free(hp);
            found = 1;
            break;
        }
    }

    if (!found) {
        pthread_mutex_unlock(hm->mtx);
        NS_LOG(NS_LOG_LVL_WARN, "Failed to find client handle");
        return -1;
    }

    /* delete all break points associated with the client */
    be = &hm->code_breaks;
    while (*be) {
        if ((*be)->client_handle == client_handle) {
            breakid = (*be)->breakid;
            /* clear any active break associated with the code bp */
            clear_activebreak(hm, *be);
            /* delete the code breakpoint */
            if (hwdbg_me_running(dbgsrv_glob->hwdbg, (*be)->meid)) {
                NS_LOG(NS_LOG_LVL_DEBUG, "Stopping ME to remove codeBP");
                if (hwdbg_stop_me(dbgsrv_glob->hwdbg, (*be)->meid, 0) < 0) {
                    NS_LOG(NS_LOG_LVL_WARN, "Failed to stop ME");
                } else {
                    del_codebreak(hm, *be);
                    hwdbg_run_me(dbgsrv_glob->hwdbg, (*be)->meid, 0);
                }
            } else
                del_codebreak(hm, *be);
            bp = *be;
            *be = bp->next;
            free(bp);
            NS_LOG(NS_LOG_LVL_DEBUG,
                   "Automatically removed breakpoint with ID 0x%lx", breakid);
        } else
            be = &(*be)->next;
    }
    pthread_mutex_unlock(hm->mtx);
    NS_LOG(NS_LOG_LVL_DEBUG, "Deleting client %p", client_handle);
    return 0;
}

int hwdbg_mgmt_client_count(struct hwdbg_mgmt_handle *hm)
{
    struct hwdbg_mgmt_client_handle_entry *e;
    int cnt = 0;

    if (pthread_mutex_lock(hm->mtx) < 0) {
        NS_LOG(NS_LOG_LVL_ERROR, "Failed to acquire lock while adding client");
        return -1;
    }

    for (e = hm->client_handles; e; e = e->next)
        cnt++;

    pthread_mutex_unlock(hm->mtx);

    return cnt;
}

/************* Init + Free **************/
static void send_reset_event(struct hwdbg_mgmt_handle *hm, int resettype)
{
    struct nfp_hwdbg_event_pl_reset pl;

    pl.type = resettype;
    pthread_mutex_lock(hm->mtx);
    send_event_all(hm, NFP_HWDBG_EVENT_TYPE_RESET, &pl, sizeof(pl));
    pthread_mutex_unlock(hm->mtx);
}

int hwdbg_mgmt_reset(struct hwdbg_mgmt_handle *hm, int resettype)
{
    struct hwdbg_mgmt_break_entry *b, *bn;
    int clearcodebp = 0;

    /* Reset for loader synchronization */
    if (resettype == NFP_HWDBG_SRV_RESET_TYPE_LOADER) {
        if (pthread_mutex_lock(dbgsrv_glob->load_mtx) < 0) {
            NS_LOG(NS_LOG_LVL_ERROR, "Failed to acquire load lock");
            return -EIO;
        }

        /* check if we already have a load pending */
        if (dbgsrv_glob->load_busy) {
            pthread_mutex_unlock(dbgsrv_glob->load_mtx);
            NS_LOG(NS_LOG_LVL_DEBUG,
                   "Attempted to synchronize load while load in progress");
            return -EBUSY;
        }

        /* detach the debugger */
        if (dbgsrv_debugger_attached_set(0))
            NS_LOG(NS_LOG_LVL_WARN, "Failed to set nffw debugger flag");

        dbgsrv_glob->load_busy = 1;

        pthread_mutex_unlock(dbgsrv_glob->load_mtx);
        send_reset_event(hm, resettype);
        NS_LOG(NS_LOG_LVL_DEBUG, "Server primed for firware loading");
        return 0;
    }

    switch (resettype) {
    case NFP_HWDBG_SRV_RESET_TYPE_HARDRESET:
        clearcodebp = 1;
    case NFP_HWDBG_SRV_RESET_TYPE_SOFTRESET:
        break;
    default:
        NS_LOG(NS_LOG_LVL_WARN, "Invalid debug reset type %d", resettype);
        return -EINVAL;
    }

    NS_LOG(NS_LOG_LVL_INFO, "Resetting Debug Server data");

    if (pthread_mutex_lock(hm->mtx) < 0) {
        NS_LOG(NS_LOG_LVL_ERROR,
               "Failed to acquire lock while resetting debug");
        return -EIO;
    }

    hwdbg_reset(dbgsrv_glob->hwdbg);

    hm->softbreakno = 0;

    for (b = hm->active_breaks; b; b = bn) {
        bn = b->next;
        free(b);
    }
    hm->active_breaks = NULL;

    for (b = hm->cleared_breaks; b; b = bn) {
        bn = b->next;
        free(b);
    }
    hm->cleared_breaks = NULL;

    for (b = hm->code_breaks; b; b = bn) {
        bn = b->next;
        if (clearcodebp) {
            /* FIXME: I think restoring original instruction for the
             * code breakpoint is not needed, consider removing
             */
            hwdbg_del_codebp(dbgsrv_glob->hwdbg, b->meid, b->pc);
            free(b);
        } else {
            /* restore the code break in ustore */
            hwdbg_add_codebp(dbgsrv_glob->hwdbg, b->meid, b->pc);
        }
    }
    if (clearcodebp)
        hm->code_breaks = NULL;

    pthread_mutex_unlock(hm->mtx);

    if (pthread_mutex_lock(dbgsrv_glob->load_mtx) < 0) {
        NS_LOG(NS_LOG_LVL_ERROR, "Failed to acquire load lock");
        return -EIO;
    }

    /* reattach the debugger */
    if (dbgsrv_debugger_attached_set(1))
        NS_LOG(NS_LOG_LVL_WARN, "Failed to set nffw debugger flag");

    if (dbgsrv_glob->load_busy) {
        dbgsrv_glob->load_busy = 0;
        NS_LOG(NS_LOG_LVL_DEBUG, "Server ready for debugging");
    }

    pthread_mutex_unlock(dbgsrv_glob->load_mtx);

    send_reset_event(hm, resettype);

    return 0;
}

/* set/override the enabled MEs list */
int hwdbg_mgmt_set_enabled_mes(struct hwdbg_mgmt_handle *hm,
                               int *meids,
                               int len)
{
    int i;

    if (pthread_mutex_lock(dbgsrv_glob->load_mtx) < 0) {
        NS_LOG(NS_LOG_LVL_ERROR, "Failed to acquire load lock");
        return -EIO;
    }

    if (!dbgsrv_glob->load_busy) {
        pthread_mutex_unlock(dbgsrv_glob->load_mtx);
        NS_LOG(NS_LOG_LVL_ERROR, "Failed to acquire load lock");
        return -EIO;
    }

    if (dbgsrv_glob->opt_enabled_mes) {
        free(dbgsrv_glob->opt_enabled_mes);
        dbgsrv_glob->opt_enabled_mes = NULL;
    }

    if (len) {
        dbgsrv_glob->opt_enabled_mes = malloc((len + 1) * sizeof(int));
        if (!dbgsrv_glob->opt_enabled_mes) {
            pthread_mutex_unlock(dbgsrv_glob->load_mtx);
            NS_LOG(NS_LOG_LVL_ERROR,
                   "Failed to allocate memory for enabled me list");
            return -ENOMEM;
        }
        dbgsrv_glob->opt_enabled_mes[len] = -1;

        NS_LOG(NS_LOG_LVL_DEBUG, "Enabled MEs:");
        for (i = 0; i < len; i++) {
            char buf[NFP_MEID_STR_SZ];

            dbgsrv_glob->opt_enabled_mes[i] = meids[i];

            if (!nfp6000_meid2str(buf, meids[i]))
                NS_LOG(NS_LOG_LVL_DEBUG, "\t\t%s", buf);
            else
                NS_LOG(NS_LOG_LVL_DEBUG, "\t\tmeid 0x%x", meids[i]);
        }
    } else {
        NS_LOG(NS_LOG_LVL_DEBUG, "Enabled all MEs");
    }

    pthread_mutex_unlock(dbgsrv_glob->load_mtx);

    return 0;
}

struct hwdbg_mgmt_handle *hwdbg_mgmt_init(void)
{
    struct hwdbg_mgmt_handle *ret;

    ret = malloc(sizeof(*ret));

    if (!ret) {
        NS_LOG(NS_LOG_LVL_ERROR, "Failed to alloced debug management handle");
        return NULL;
    }

    ret->softbreakno = 0;
    ret->ipc_seq = 0;
    ns_ipc_buf_init(&ret->ipc_buf);
    ret->client_handles = NULL;
    ret->code_breaks = NULL;
    ret->active_breaks = NULL;
    ret->cleared_breaks = NULL;
    ret->mtx = dbgsrv_glob->nfp_mtx;
    return ret;
}

void hwdbg_mgmt_free(struct hwdbg_mgmt_handle *hm)
{
    struct hwdbg_mgmt_client_handle_entry *ch, *chn;
    struct hwdbg_mgmt_break_entry *b, *bn;

    pthread_mutex_lock(hm->mtx);

    for (ch = hm->client_handles; ch; ch = chn) {
        chn = ch->next;
        free(ch);
    }

    for (b = hm->active_breaks; b; b = bn) {
        bn = b->next;
        free(b);
    }

    for (b = hm->cleared_breaks; b; b = bn) {
        bn = b->next;
        free(b);
    }

    for (b = hm->code_breaks; b; b = bn) {
        bn = b->next;
        del_codebreak(hm, b);
        free(b);
    }

    pthread_mutex_unlock(hm->mtx);
    ns_ipc_buf_done(&hm->ipc_buf);
    free(hm);
}
