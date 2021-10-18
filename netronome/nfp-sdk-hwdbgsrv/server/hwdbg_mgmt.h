/*
 * Copyright (C) 2014, Netronome Systems, Inc.  All rights reserved.
 */

#ifndef __HWDBH_MGMT_H__
#define __HWDBH_MGMT_H__

#include <nfp_hwdbg.h>

/*** Data for hardware debug management ***/

struct hwdbg_mgmt_break_entry {
    int64_t breakid;
    int ctx;
    int meid;
    int pc;
    void *client_handle;
    struct hwdbg_mgmt_break_entry *next;
};

struct hwdbg_mgmt_client_handle_entry {
    void *client_handle;
    void *client_conn_handle;
    struct hwdbg_mgmt_client_handle_entry *next;
};

struct hwdbg_mgmt_handle {
    /* list of client connections - we use this to send events */
    struct hwdbg_mgmt_client_handle_entry *client_handles;
    /* code break points that the client has added */
    struct hwdbg_mgmt_break_entry *code_breaks;
    /* active software and code breaks */
    struct hwdbg_mgmt_break_entry *active_breaks;
    /* breaks that are cleared, but have not had the clear action applied
     * the action will be applied when step or run is called
     */
    struct hwdbg_mgmt_break_entry *cleared_breaks;
    /* active software break number */
    uint64_t softbreakno;
    /* event sequence number */
    uint32_t ipc_seq;
    /* buffer for ipc */
    ns_ipc_buf_t ipc_buf;
    /* lock */
    pthread_mutex_t *mtx;
};

/*** Internal API ***/

/* initialize hardware debug management handle */
struct hwdbg_mgmt_handle *hwdbg_mgmt_init(void);

/* free a hardware debug management handle */
void hwdbg_mgmt_free(struct hwdbg_mgmt_handle *hm);

/* reset all debug data, software breaks are optionally retained */
int hwdbg_mgmt_reset(struct hwdbg_mgmt_handle *hm, int resettype);

int hwdbg_mgmt_set_enabled_mes(struct hwdbg_mgmt_handle *hm,
                               int *meids,
                               int len);

int hwdbg_mgmt_run(struct hwdbg_mgmt_handle *hm, int meid, int doclear);
int hwdbg_mgmt_stop(struct hwdbg_mgmt_handle *hm, int meid);

/* add a client connection handle */
int hwdbg_mgmt_add_client(struct hwdbg_mgmt_handle *hm,
                          void *client_handle); /* client connection handle */
/* add a client connection handle */
int hwdbg_mgmt_del_client(struct hwdbg_mgmt_handle *hm,
                          void *client_handle); /* client connection handle */

int hwdbg_mgmt_client_count(struct hwdbg_mgmt_handle *hm);

/* add a code breakpoint */
int hwdbg_mgmt_add_codebp(struct hwdbg_mgmt_handle *hm,
                          void *client_handle, /* client connection handle */
                          int64_t breakid,
                          int meid, int pc, unsigned int ctxmask);

/* delete a code breakpoint */
int hwdbg_mgmt_del_codebp(struct hwdbg_mgmt_handle *hm,
                          void *client_handle, /* client connection handle */
                          int64_t breakid);

/* flush all code breakpoints and also clear any active code breakpoints */
int hwdbg_mgmt_flush_codebp(struct hwdbg_mgmt_handle *hm, void *client_handle);

/* retrieve active breaks */
int hwdbg_mgmt_retrieve(struct hwdbg_mgmt_handle *hm,
                        struct nfp_hwdbg_break_data *data, int len, int *cnt);
/* clear an active break */
int hwdbg_mgmt_clear(struct hwdbg_mgmt_handle *hm, int64_t breakid);

/* handle a detected sw break - will add to active break list and send
 * notifications
 * returns the number of breakpoint matches, or 0 is no code breakpoint
 * was matched
 */
int hwdbg_mgmt_handle_swbrk(struct hwdbg_mgmt_handle *hm,
                            int meid, int ctx, int pc, int swbrk);

/* return 1 if there is a code breaks for a given me + ctx + pc */
int hwdbg_mgmt_codebp_matches(struct hwdbg_mgmt_handle *hm,
                              int meid, int ctx, int pc);

/* send an event */
void hwdbg_mgmt_send_event(struct hwdbg_mgmt_handle *hm,
                           int type, void *data, int len);

#endif /* __HWDBH_MGMT_H__ */
