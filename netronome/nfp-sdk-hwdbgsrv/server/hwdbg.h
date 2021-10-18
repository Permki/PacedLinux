/*
 * Copyright (C) 2014, Netronome Systems, Inc.  All rights reserved.
 */

#ifndef __HWDBG_H__
#define __HWDBG_H__

#include <nfp-common/nfp_resid.h>

/* number of 64-bit words in CS */
#define CSLEN (1024*8)

/* number of MEs that share control store in QSC mode.
 * first ME is already part of hwdbg_me_data
 */
#define QCSMENUM  (4 - 1)

struct hwdbg_me_data {
    int meid;
    /* is the ME in the stopped state */
    int stopped;
    /* is SCS enabled for this ME */
    int scsenabled;
    /* is QCS enabled for this ME */
    int qcsenabled;
    /* ME does not support PC breakpoints */
    int nopcb;
    /* set to 0 if this ME has had debug disabled */
    int disabled;
    /* code store with our modifications - we allocate as required */
    uint64_t *base_cs;
    /* pointer to "buddy ME" for SCS */
    struct hwdbg_me_data *scs_me_data;
    /* pointer to "buddy MEs" for QCS */
    struct hwdbg_me_data *qcs_me_data[QCSMENUM];

    /* debug info */
    uint64_t *dbginfo; /* data */
    int dbginfo_len; /* length of data */
    int dbginfo_off; /* pc offset where data starts */

    /* me break group */
    int *brkgrp_meids;
    int brkgrp_len;

    /* there is an active codebreak to resume */
    int active_codebp;
    int routine_primed;
    /* if a break occurred, these contain the pc + ctx */
    int resume_pc;
    int resume_ctx;
    char meid_str[NFP_MEID_STR_SZ];

    /* Pre-determined PC where debugger side-routing starts.
     * For SCS ME pairs, each ME needs its own block at the end of the virtual
     * address space.
     *
     * This will be set to -1 if the loaded app code uses the last few
     * instructions and we cannot support breakpoints for that ME or SCS MEs.
     */
    int routine_addr;
};

struct hwdbg_handle {
    int me_count;
    struct hwdbg_me_data *me_data;

    int stop_thread;
    pthread_t pthread;

    /* data from parent */
    struct nfp_device *nfp;
    pthread_mutex_t *nfp_mtx;
};

/* internal API */

struct hwdbg_handle *hwdbg_init(void);
void hwdbg_free(struct hwdbg_handle *hd);

/* reset all state */
void hwdbg_reset(struct hwdbg_handle *hd);

/* interaction with hwdbg_mgmt */
int hwdbg_del_codebp(struct hwdbg_handle *hd, int meid, int pc);
int hwdbg_add_codebp(struct hwdbg_handle *hd, int meid, int pc);
/* run an me, this will automatically resume from the active breakd */
int hwdbg_run_me(struct hwdbg_handle *hd, int meid, int suppress);
/* stop an ME - return 0 if stopped ok, -1 if error or 1 if timed out */
int hwdbg_stop_me(struct hwdbg_handle *hd, int meid, int suppress);
/* check if an me is running, 1 when running otherwise 0 */
int hwdbg_me_running(struct hwdbg_handle *hd, int meid);

struct hwdbg_me_data *hwdbg_get_me_data(struct hwdbg_handle *hd, int meid);

/* rpc calls */
int hwdbg_add_dbginfo(struct hwdbg_handle *hd,
                      int meid, uint64_t *data, int len, int off);
int hwdbg_step_me(struct hwdbg_handle *hd, int meid, int ctx);

int hwdbg_me_status(struct hwdbg_handle *hd, int meid,
                    struct nfp_hwdbg_me_status_data *status);

int hwdbg_brkgrp_add(struct hwdbg_handle *hd, int meid, int *meids, int len);
int hwdbg_brkgrp_del(struct hwdbg_handle *hd, int meid);
#endif /* __HWDBG_H__ */
