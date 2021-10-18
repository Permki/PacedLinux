/*
 * Copyright (C) 2014, Netronome Systems, Inc.  All rights reserved.
 */

#ifndef __DBGSRV_H__
#define __DBGSRV_H__

#include <pthread.h>
#include <nfp-common/nfp_chipdata.h>
#include <nfp.h>
#include <nfp_hwdbg.h>
#include "ns_ipc_buf.h"
#include "ns_ipc_server.h"
#include "ipc/ipc_client.h"
#include "dbgsrv_env.h"
#include "logging.h"
#include "hwdbg.h"
#include "hwdbg_mgmt.h"

/* Application defaults */
#define DBGSRV_DEFAULT_PORT 20406
#define DBGSRV_DEFAULT_NFPDEVICENO 0

#ifdef SDK_SIM
#define DBGSRV_DEFAULT_SWBRKPOLL 200000 /* 200ms */
#else
#define DBGSRV_DEFAULT_SWBRKPOLL 5000 /* 5ms */
#endif

#define DBGSRV_DEFAULT_PIDFILE "/tmp/dbgsrv.%d.pid"

struct dbgsrv_handle {
    struct ipc_client *ipcclient;
    struct ns_ipc_server_handle ipc_h;
    int ipcsrv_fd;
    const struct nfp_chipdata_chip *chip;
    struct nfp_chipdata *chipdata;
    struct nfp_device *nfp;
    pthread_mutex_t *nfp_mtx;
    struct hwdbg_handle *hwdbg;
    struct hwdbg_mgmt_handle *hwdbg_mgmt;
    int pidfd;
    char *pidfilename;
    char *version;

    pthread_mutex_t *load_mtx;
    int load_busy;

    /* command line options */
    char *opt_ipc_listen_ip;
    int   opt_ipc_listen_port;
    int   opt_nfp_device_no;
    int   opt_daemonize;
    int   opt_forcestdout;
    int   opt_usesyslog;
    int   opt_stopmes;
    int   opt_loglevel;
    int   opt_nostdin;
    char *opt_logfile;
    char *opt_pidfile;
    int  *opt_enabled_mes;
};

struct dbgsrv_handle *dbgsrv_init(void);
void dbgsrv_free(struct dbgsrv_handle *dh);

int dbgsrv_load_options(struct dbgsrv_handle *dh, int argc, char **argv);
void dbgsrv_print_usage(char *app);
int dbgsrv_do_setup(struct dbgsrv_handle *dh);

int dbgsrv_debugger_attached_set(int is_set);

extern struct dbgsrv_handle *dbgsrv_glob;


#endif /* __DBGSRV_H__ */
