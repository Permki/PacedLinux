/*
 * Copyright (C) 2014, Netronome Systems, Inc.  All rights reserved.
 */

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include <nfp-common/nfp_resid.h>
#include <nfp.h>
#include <nfp_nffw.h>

#include "dbgsrv.h"
#include "ipc/ipc_support.h"
/* stub headers */
#include "_nfp.h"
#include "_nfp_bulk.h"
#include "_nfp_power.h"
#include "_nfp_nffw.h"
#include "_nfp_internal.h"
#include "_nfp_cpp.h"
#include "_nfp_cpp_internal.h"
#include "_nfp_hwdbg.h"

/* build version and date */
extern char *build_dbgsrv_version;
extern char *build_number;
extern char *build_date;

static pthread_mutex_t nfp_mtx = PTHREAD_MUTEX_INITIALIZER;

/* Use the NFFW Debugger Flag to indicate debugger attached,
 * enforces cooperative loading with nfp-nffw tool
 */
#define NFFW_DEBUGGER_FLAG

/* set the nffw debugger attached flag, this can be used to block
 * firmware loading while the debugger is loaded and not primed to
 * allow loading
 */
int dbgsrv_debugger_attached_set(int is_set)
{
#ifdef NFFW_DEBUGGER_FLAG
    int ret;
    if (pthread_mutex_lock(dbgsrv_glob->nfp_mtx) < 0) {
        NS_LOG(NS_LOG_LVL_ERROR, "Failed to acquire nfp lock");
        return -1;
    }

    if (nfp_nffw_info_acquire(dbgsrv_glob->nfp) < 0) {
        pthread_mutex_unlock(dbgsrv_glob->nfp_mtx);
        NS_LOG(NS_LOG_LVL_ERROR, "Failed to acquire nffw info");
        return -1;
    }

    ret = nfp_nffw_info_fw_debugger_attached_set(dbgsrv_glob->nfp, is_set);
    nfp_nffw_info_release(dbgsrv_glob->nfp);
    pthread_mutex_unlock(dbgsrv_glob->nfp_mtx);

    if (ret < 0) {
        NS_LOG(NS_LOG_LVL_WARN, "Failed to set nffw debugger attached flag");
        return -1;
    }

    return 0;
#else
    return 0; /* return success if disabled */
#endif
}

int dbgsrv_debugger_attached_get(void)
{
#ifdef NFFW_DEBUGGER_FLAG
    int ret;
    if (pthread_mutex_lock(dbgsrv_glob->nfp_mtx) < 0) {
        NS_LOG(NS_LOG_LVL_ERROR, "Failed to acquire nfp lock");
        return -1;
    }

    if (nfp_nffw_info_acquire(dbgsrv_glob->nfp) < 0) {
        pthread_mutex_unlock(dbgsrv_glob->nfp_mtx);
        NS_LOG(NS_LOG_LVL_ERROR, "Failed to acquire nffw info");
        return -1;
    }

    ret = nfp_nffw_info_fw_debugger_attached(dbgsrv_glob->nfp);

    nfp_nffw_info_release(dbgsrv_glob->nfp);
    pthread_mutex_unlock(dbgsrv_glob->nfp_mtx);

    return ret;
#else
    return 0; /* return not attached */
#endif
}


struct dbgsrv_handle *dbgsrv_init(void)
{
    struct dbgsrv_handle *h;

    h = malloc(sizeof(*h));
    if (!h) {
        NS_LOG(NS_LOG_LVL_ERROR, "Failed to allocate debug handle");
        return NULL;
    }

    h->ipcclient = NULL;
    h->ipcsrv_fd = -1;
    h->nfp = NULL;
    h->chip = NULL;
    h->hwdbg = NULL;
    h->hwdbg_mgmt = NULL;
    h->version = NULL;
    h->nfp_mtx = &nfp_mtx;
    h->pidfd = -1;
    h->pidfilename = NULL;

    return h;
}

void dbgsrv_free(struct dbgsrv_handle *dh)
{
    int i;

    if (dh->hwdbg_mgmt) {
        struct nfp_hwdbg_event_pl_shutdown pl = {0};
        /* Tell client we are shutting down */
        hwdbg_mgmt_send_event(dh->hwdbg_mgmt,
                              NFP_HWDBG_EVENT_TYPE_SHUTDOWN, &pl, sizeof(pl));
    }

    if (dh->ipcsrv_fd >= 0) {
        if (ns_ipc_server_clientcnt(&dh->ipc_h) > 0) {
            NS_LOG(NS_LOG_LVL_INFO, "Waiting for client to disconnect");
            /* wait 5s, with 100ms poll interval for client to disconnect */
            for (i = 0; i < 5*10; i++) {
                usleep(100000);
                if (ns_ipc_server_clientcnt(&dh->ipc_h) == 0)
                    break;
            }
            i = ns_ipc_server_clientcnt(&dh->ipc_h);
            if (i != 0)
                NS_LOG(NS_LOG_LVL_WARN,
                       "warning: %d client(s) still connected", i);
        }

        NS_LOG(NS_LOG_LVL_INFO, "Shutting down server");
        ns_ipc_server_close(&dh->ipc_h);
    }

    if (dh->ipcclient)
        ipc_client_free(dh->ipcclient);

    if (dh->hwdbg_mgmt)
        hwdbg_mgmt_free(dh->hwdbg_mgmt);

    if (dh->hwdbg)
        hwdbg_free(dh->hwdbg);

    if (dh->nfp)
        dbgsrv_debugger_attached_set(0);

    if (dh->nfp)
        nfp_device_close(dh->nfp);

    if (dh->version)
        free(dh->version);

    if (dh->opt_enabled_mes)
        free(dh->opt_enabled_mes);

    if (dh->pidfd) {
        close(dh->pidfd);
        unlink(dh->pidfilename);
        free(dh->pidfilename);
    }

    if (dh->load_mtx) {
        pthread_mutex_destroy(dh->load_mtx);
        free(dh->load_mtx);
    }

    free(dh);
}

static int parse_enabled_mes(const char *mestr, int **mes)
{
    char *buf;
    char *s, *e;
    int *m;
    int end = 0;
    int cnt = 0;
    int ret;

    *mes = NULL;

    buf = strdup(mestr);
    if (!buf) {
        printf("out of memory while handling enable MEs\n");
        return -1;
    }

    s = buf;
    do {
        e = strchr(s, ',');
        if (e) {
            *e = '\0';
        } else
            end = 1;

        m = realloc(*mes, sizeof(int) * (cnt + 1));
        if (!m) {
            printf("out of memory while parsing enable MEs\n");
            free(buf);
            free(*mes);
            *mes = NULL;
            return -1;
        }
        *mes = m;

        ret = nfp6000_idstr2meid(s, NULL);
        if (ret < 0)
            (*mes)[cnt] = strtol(s, NULL, 0);
        else
            (*mes)[cnt] = ret;

        cnt++;
        s = e + 1;
    } while (!end);

    free(buf);

    /* add a -1 at the end */
    m = realloc(*mes, sizeof(int) * (cnt + 1));
    if (!m) {
        free(*mes);
        *mes = NULL;
        printf("out of memory while parsing enable MEs\n");
        return -1;
    }
    *mes = m;
    (*mes)[cnt] = -1;

    return cnt;
}

static char *gen_version_str(void)
{
#define VERSTR_LEN 1024
    char *ret;
    ret = malloc(VERSTR_LEN);
    if (!ret) {
        NS_LOG(NS_LOG_LVL_FATAL,
               "Failed to allocate memory for version string");
        return NULL;
    }
    snprintf(ret, VERSTR_LEN, "build: %s revision: %s date: %s",
             build_number, build_dbgsrv_version, build_date);
    return ret;
}

static void print_version(const char *app, const char *ver)
{
    printf("%s version info: %s\n", app, ver);

}

void dbgsrv_print_usage(char *app)
{
    printf("usage : %s [options]\n"
           "\tbuild version: %s\n"
           "\tbuild date: %s\n"
           "\tWhere options are:\n"
           "\t-h              - this message\n"
           "\t-v              - print version\n"
           "\t-n [nfp]        - NFP device number (default: %d)\n"
           "\t-p [port]       - specify RPC port (default: %d)\n"
           "\t-s [srcip]      - specify RPC source IP\n"
           "\t-D              - run as daemon\n"
           "\t-O              - stop MEs on startup\n"
           "\t-m [mes]        - list of MEs to debug\n"
           "\t-l [level]      - set log level (default: %d)\n"
           "\t-f [filename]   - log output to a file\n"
           "\t-S              - log output to syslog\n"
           "\t-A              - always log to stdout\n"
           "\t-P [pidfile]    - specify pidfile name\n",
           app, build_dbgsrv_version, build_date,
           DBGSRV_DEFAULT_NFPDEVICENO, DBGSRV_DEFAULT_PORT, NS_LOG_LVL_INFO);
}

void dbgsrv_load_env(struct dbgsrv_handle *dh)
{
    char *var;
    var = getenv(RPC_DO_VERSION_CHECK);
    if (var)
        do_api_version_checks = atoi(var);
}

int dbgsrv_load_options(struct dbgsrv_handle *dh, int argc, char **argv)
{
    int o;
    dh->version = gen_version_str();
    /* set defaults */
    dh->opt_enabled_mes = NULL;
    dh->opt_nfp_device_no = DBGSRV_DEFAULT_NFPDEVICENO;
    dh->opt_ipc_listen_port = DBGSRV_DEFAULT_PORT;
    dh->opt_ipc_listen_ip = NULL;
    dh->opt_stopmes = 0;
    dh->opt_daemonize = 0;
    dh->opt_forcestdout = 0;
    dh->opt_usesyslog = 0;
    dh->opt_loglevel = NS_LOG_LVL_INFO;
    dh->opt_logfile = NULL;
    dh->opt_pidfile = NULL;
    dh->opt_nostdin = 0;

    while ((o = getopt(argc, argv, "hvon:m:p:s:Dl:f:SAP:I")) != -1) {
        switch (o) {
        case 'h' :
            return 1;
        case 'v' :
            print_version(argv[0], dh->version);
            exit(0);
        case 'o' :
            dh->opt_stopmes = 1;
            break;
        case 'n' :
            dh->opt_nfp_device_no = atoi(optarg);
            break;
        case 'm' :
            parse_enabled_mes(optarg, &dh->opt_enabled_mes);
            break;
        case 'p' :
            dh->opt_ipc_listen_port = atoi(optarg);
            break;
        case 's' :
            dh->opt_ipc_listen_ip = optarg;
            break;
        case 'D' :
            dh->opt_daemonize = 1;
            dh->opt_usesyslog = 1; /* always log to syslog in daemon mode */
            break;
        case 'l' :
            dh->opt_loglevel = atoi(optarg);
            printf("loglevel = %d\n", dh->opt_loglevel);
            break;
        case 'f' :
            dh->opt_logfile = optarg;
            break;
        case 'S' :
            dh->opt_usesyslog = 1;
            break;
        case 'I' :
            dh->opt_nostdin = 1;
            break;
        case 'A' :
            dh->opt_forcestdout = 1;
            break;
        case 'P' :
            dh->opt_pidfile = optarg;
            break;
        default:
            NS_LOG(NS_LOG_LVL_FATAL, "Unrecognized option %c", o);
            return -1;
        }
    }
    return 0;
}

int dbgsrv_do_setup(struct dbgsrv_handle *dh)
{
    int i, n;
    int ret;

    dh->load_mtx = malloc(sizeof(*dh->load_mtx));
    if (!dh->load_mtx) {
        NS_LOG(NS_LOG_LVL_FATAL, "failed to allocate loader rwlock");
        return -1;
    }

    ret = pthread_mutex_init(dh->load_mtx, NULL);
    if (ret) {
        NS_LOG(NS_LOG_LVL_FATAL,
               "pthread_mutex_init failed (%s)",
               strerror(ret));
        return -1;
    }

    dh->load_busy = 0;

    dh->ipcclient = ipc_client_init();
    if (!dh->ipcclient) {
        NS_LOG(NS_LOG_LVL_FATAL, "failed to allocate IPC Client data");
        return -1;
    }

    /* initialize the ipc server, starts listening but not accepting */
    ret = ns_ipc_server_init(&dh->ipc_h,
                             dh->opt_ipc_listen_ip,
                             dh->opt_ipc_listen_port);
    if (ret) {
        NS_LOG(NS_LOG_LVL_FATAL, "Fatal error: IPC initialization failed");
        return -1;
    }

    /* Get file descriptor for IPC and set up callback functions for
     * client connect and disconnect
     */
    dh->ipcsrv_fd = ns_ipc_server_get_fd(&dh->ipc_h);
    ns_ipc_server_set_callbacks(&dh->ipc_h,
                                NULL,
                                &ipc_client_connect,
                                &ipc_client_disconnect);

    /* Register NFP functions */
    n = 0;
    for (i = 0; ; i++) {
        if (!ipc_nfp_fmaps[i].name)
            break;

        NS_LOG(NS_LOG_LVL_DEBUG,
               "Registering NFP function %s",
               ipc_nfp_fmaps[i].name);

        ret = ns_ipc_server_register_method(&dh->ipc_h,
                                            ipc_nfp_fmaps[i].name,
                                            ipc_nfp_fmaps[i].func);
        if (ret) {
            NS_LOG(NS_LOG_LVL_ERROR,
                   "failed to add NFP/NFHAL function %s",
                   ipc_nfp_fmaps[i].name);
        }
        n++;
    }
    /* Register NFP Internal functions */
    for (i = 0; ; i++) {
        if (!ipc_nfp_internal_fmaps[i].name)
            break;

        NS_LOG(NS_LOG_LVL_DEBUG,
               "Registering NFP Internal function %s",
               ipc_nfp_internal_fmaps[i].name);
        ret = ns_ipc_server_register_method(&dh->ipc_h,
                                            ipc_nfp_internal_fmaps[i].name,
                                            ipc_nfp_internal_fmaps[i].func);
        if (ret)
            NS_LOG(NS_LOG_LVL_ERROR,
                   "failed to add NFP Internal function %s",
                   ipc_nfp_internal_fmaps[i].name);
        n++;
    }
    /* Register NFP Power functions */
    for (i = 0; ; i++) {
        if (!ipc_nfp_power_fmaps[i].name)
            break;

        NS_LOG(NS_LOG_LVL_DEBUG,
               "Registering NFP Power function %s",
               ipc_nfp_power_fmaps[i].name);
        ret = ns_ipc_server_register_method(&dh->ipc_h,
                                            ipc_nfp_power_fmaps[i].name,
                                            ipc_nfp_power_fmaps[i].func);
        if (ret)
            NS_LOG(NS_LOG_LVL_ERROR,
                   "failed to add NFP Power function %s",
                   ipc_nfp_power_fmaps[i].name);
        n++;
    }
    /* Register NFP Bulk functions */
    for (i = 0; ; i++) {
        if (!ipc_nfp_bulk_fmaps[i].name)
            break;

        NS_LOG(NS_LOG_LVL_DEBUG,
               "Registering NFP Bulk function %s",
               ipc_nfp_bulk_fmaps[i].name);
        ret = ns_ipc_server_register_method(&dh->ipc_h,
                                            ipc_nfp_bulk_fmaps[i].name,
                                            ipc_nfp_bulk_fmaps[i].func);
        if (ret)
            NS_LOG(NS_LOG_LVL_ERROR,
                   "failed to add NFP Bulk function %s",
                   ipc_nfp_bulk_fmaps[i].name);
        n++;
    }
    NS_LOG(NS_LOG_LVL_INFO, "Registered %d NFP functions", n);

    /* Register NFP NFFW functions */
    for (i = 0; ; i++) {
        if (!ipc_nfp_nffw_fmaps[i].name)
            break;

        NS_LOG(NS_LOG_LVL_DEBUG,
               "Registering NFP NFFW function %s",
               ipc_nfp_nffw_fmaps[i].name);
        ret = ns_ipc_server_register_method(&dh->ipc_h,
                                            ipc_nfp_nffw_fmaps[i].name,
                                            ipc_nfp_nffw_fmaps[i].func);
        if (ret)
            NS_LOG(NS_LOG_LVL_ERROR,
                   "failed to add NFP NFFW function %s",
                   ipc_nfp_nffw_fmaps[i].name);
    }
    NS_LOG(NS_LOG_LVL_INFO, "Registered %d NFP NFFW functions", i);

    /* Register NFP_CPP functions */
    n = 0;
    for (i = 0; ; i++) {
        if (!ipc_nfp_cpp_fmaps[i].name)
            break;

        NS_LOG(NS_LOG_LVL_DEBUG,
               "Registering NFP CPP function %s",
               ipc_nfp_cpp_fmaps[i].name);

        ret = ns_ipc_server_register_method(&dh->ipc_h,
                                            ipc_nfp_cpp_fmaps[i].name,
                                            ipc_nfp_cpp_fmaps[i].func);
        if (ret) {
            NS_LOG(NS_LOG_LVL_ERROR,
                   "failed to add NFP CPP function %s",
                   ipc_nfp_cpp_fmaps[i].name);
        }
        n++;
    }
    for (i = 0; ; i++) {
        if (!ipc_nfp_cpp_internal_fmaps[i].name)
            break;

        NS_LOG(NS_LOG_LVL_DEBUG,
               "Registering NFP CPP function %s",
               ipc_nfp_cpp_internal_fmaps[i].name);

        ret = ns_ipc_server_register_method(&dh->ipc_h,
                                            ipc_nfp_cpp_internal_fmaps[i].name,
                                            ipc_nfp_cpp_internal_fmaps[i].func);
        if (ret) {
            NS_LOG(NS_LOG_LVL_ERROR,
                   "failed to add NFP CPP function %s",
                   ipc_nfp_cpp_internal_fmaps[i].name);
        }
        n++;
    }
    NS_LOG(NS_LOG_LVL_INFO, "Registered %d NFP CPP functions", n);

    /* Register NFP_HWDBG functions */
    for (i = 0; ; i++) {
        if (!ipc_nfp_hwdbg_fmaps[i].name)
            break;

        NS_LOG(NS_LOG_LVL_DEBUG,
               "Registering NFP HWDBG function %s",
               ipc_nfp_hwdbg_fmaps[i].name);

        ret = ns_ipc_server_register_method(&dh->ipc_h,
                                            ipc_nfp_hwdbg_fmaps[i].name,
                                            ipc_nfp_hwdbg_fmaps[i].func);
        if (ret) {
            NS_LOG(NS_LOG_LVL_ERROR,
                   "failed to add NFP HWDBG function %s",
                   ipc_nfp_hwdbg_fmaps[i].name);
        }
    }
    NS_LOG(NS_LOG_LVL_INFO, "Registered %d NFP HWDBG functions", i);

    /* Open an nfp handle */
    dh->nfp = nfp_device_open(dh->opt_nfp_device_no);
    if (!dh->nfp) {
        NS_LOG(NS_LOG_LVL_FATAL,
               "failed to open nfp device (%s)",
               strerror(errno));
        return -1;
    }

    dh->chip = nfp_device_chip(dh->nfp);
    if (!dh->chip) {
        NS_LOG(NS_LOG_LVL_FATAL,
               "nfp_device_chip failed (%s)",
               strerror(errno));
        return -1;
    }
    NS_LOG(NS_LOG_LVL_INFO, "loaded nfp chipdata");

    dh->hwdbg_mgmt = hwdbg_mgmt_init();
    if (!dh->hwdbg_mgmt) {
        NS_LOG(NS_LOG_LVL_FATAL, "failed to initialize hwdbg management");
        return -1;
    }

    /* start up hwdbg - this will also start the hwdbg thread */
    dh->hwdbg = hwdbg_init();
    if (!dh->hwdbg) {
        NS_LOG(NS_LOG_LVL_FATAL, "hwdbg init failed");
        return -1;
    }

    if (dbgsrv_debugger_attached_get())
        NS_LOG(NS_LOG_LVL_WARN, "Debugger attached flag is set, overriding");

    if (dbgsrv_debugger_attached_set(1) < 0)
        NS_LOG(NS_LOG_LVL_WARN, "Failed to set nffw debugger flag");

    NS_LOG(NS_LOG_LVL_INFO, "Debug server setup complete");

    return 0;
}
