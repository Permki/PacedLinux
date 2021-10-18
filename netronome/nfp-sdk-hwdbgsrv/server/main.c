/*
 * Copyright (C) 2014, Netronome Systems, Inc.  All rights reserved.
 */

#include <errno.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/prctl.h>
#include <sys/syscall.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "ns_ipc_buf.h"
#include "ns_ipc_server.h"

#include "logging.h"

#include "dbgsrv.h"

/***************************************************
 * Signal handlers                                 *
 ***************************************************/

/* global state pointer */
struct dbgsrv_handle *dbgsrv_glob = NULL;

/* signal handlers */
static int do_shutdown = 0;

static void shutdown_handler(int signum)
{
    do_shutdown++;
}

/***************************************************
 * PID File Creation                               *
 ***************************************************/

static int create_pidfile(char *app, char *filename)
{
    int fd;
    char pid[256];
    char *file;

    if (!filename) { /* no user specified filename */
        int len;
        /* figure out how long the string will be */
        len = snprintf(NULL, 0,
                       DBGSRV_DEFAULT_PIDFILE, dbgsrv_glob->opt_nfp_device_no);

        /* allocate a string for the pid path */
        file = malloc(len + 1);
        snprintf(file, len + 1,
                 DBGSRV_DEFAULT_PIDFILE, dbgsrv_glob->opt_nfp_device_no);
    } else {
        file = strdup(filename);
        if (!file) {
            NS_LOG(NS_LOG_LVL_ERROR,
                   "Failed to allocate memory for pidfile");
            return -1;
        }
    }

    fd = open(file, O_RDWR | O_CREAT, 0644);
    if (fd < 0) {
        free(file);
        NS_LOG(NS_LOG_LVL_ERROR,
               "Error accessing pidfile: %s", strerror(errno));
        return -1;
    }

    if (lockf(fd, F_TLOCK, 0) < 0) {
        NS_LOG(NS_LOG_LVL_ERROR,
               "Pidfile %s exists; is %s already running?", file, app);
        close(fd);
        free(file);
        return -1;
    }

    snprintf(pid, 256, "%u\n", getpid());
    if (write(fd, pid, strlen(pid)) < 0) {
        NS_LOG(NS_LOG_LVL_ERROR,
               "Failed to write to pidfile %s", file);
        close(fd);
        unlink(file);
        free(file);
        return -1;
    }
    fsync(fd);
    dbgsrv_glob->pidfd = fd;
    dbgsrv_glob->pidfilename = file;
    return 0;
}

/***************************************************
 * Input handling                                  *
 ***************************************************/

/* select on stdin and server fd */
#define ACTIVITY_STDIN (1 << 0)
#define ACTIVITY_SRV   (1 << 1)

static int select_all(int stdin_fd, int srv_fd, int timeout)
{
    fd_set rfds;
    struct timeval tv;
    int retval;
    int max_fd;

    FD_ZERO(&rfds);

    if (stdin_fd >= 0)
        FD_SET(stdin_fd, &rfds);
    if (srv_fd >= 0)
        FD_SET(srv_fd, &rfds);

    max_fd = stdin_fd > srv_fd ? stdin_fd : srv_fd;

    tv.tv_sec = timeout / 1000;
    tv.tv_usec = (timeout % 1000) * 1000;

    retval = select(max_fd + 1, &rfds, NULL, NULL, &tv);

    if (retval < 0) {
        if (errno != EINTR) {
            NS_LOG(NS_LOG_LVL_ERROR,
                   "error: select failed - %s", strerror(errno));
            return retval;
        }
        return 0;
    }

    if (!retval)
        return 0;

    retval = 0;

    if (stdin_fd >= 0 && FD_ISSET(stdin_fd, &rfds))
        retval |= ACTIVITY_STDIN;

    if (srv_fd >= 0 && FD_ISSET(srv_fd, &rfds))
        retval |= ACTIVITY_SRV;

    return retval;
}

/* handle stdin when running in foreground */
static int grab_stdin(int fd, int *quit)
{
    char buffer[1025];
    int ret;

    ret = read(fd, buffer, 1024);
    if (ret < 0) {
        return ret;
    }
    if (ret > 0) {
        buffer[ret] = '\0';
        printf("stdin: %s\n", buffer);
        switch (buffer[0]){
        case '?':
            printf("Help)\n"
                   "\tI\tSet log level to INFO\n"
                   "\tD\tSet log level to DEBUG\n"
                   "\tE\tSet log level to EXTRA\n"
                   "\tH\tSet log level to HEAVY\n"
                   "\tL\tApply LOADER server reset\n"
                   "\tR\tApply HARD server reset\n"
                   "\tS\tApply SOFT server reset (code breaks remain)\n"
                   "\tq\tQuit\n");
            break;
        case 'q':
            *quit = 1;
            break;
        case 'I':
            NS_LOG_SET_LEVEL(NS_LOG_LVL_INFO);
            NS_LOG(NS_LOG_LVL_INFO, "Log level set to INFO");
            break;
        case 'D':
            NS_LOG_SET_LEVEL(NS_LOG_LVL_DEBUG);
            NS_LOG(NS_LOG_LVL_DEBUG, "Log level set to DEBUG");
            break;
        case 'E':
            NS_LOG_SET_LEVEL(NS_LOG_LVL_EXTRA);
            NS_LOG(NS_LOG_LVL_EXTRA, "Log level set to EXTRA");
            break;
        case 'H':
            NS_LOG_SET_LEVEL(NS_LOG_LVL_HEAVY);
            NS_LOG(NS_LOG_LVL_EXTRA, "Log level set to HEAVY");
            break;
        case 'L':
            NS_LOG(NS_LOG_LVL_INFO, "Applying LOADER reset");
            hwdbg_mgmt_reset(dbgsrv_glob->hwdbg_mgmt,
                             NFP_HWDBG_SRV_RESET_TYPE_LOADER);
            break;
        case 'R':
            NS_LOG(NS_LOG_LVL_INFO, "Applying HARD reset");
            hwdbg_mgmt_reset(dbgsrv_glob->hwdbg_mgmt,
                             NFP_HWDBG_SRV_RESET_TYPE_HARDRESET);
            break;
        case 'S':
            NS_LOG(NS_LOG_LVL_INFO, "Applying SOFT reset");
            hwdbg_mgmt_reset(dbgsrv_glob->hwdbg_mgmt,
                             NFP_HWDBG_SRV_RESET_TYPE_SOFTRESET);
            break;
        case '\0':
        case '\r':
        case '\n':
            /* Ignore */
            break;
        default:
            NS_LOG(NS_LOG_LVL_INFO, "unknown stdin command %c", buffer[0]);
            break;
        }
    }
    return 0;
}

/* silly exit helper function */
static int exit_cleanup(int ret)
{
    dbgsrv_free(dbgsrv_glob);
    return ret;
}

/***************************************************
 * Main                                            *
 ***************************************************/

int main(int argc, char **argv)
{
    int ret;
    int stdin_fd = STDIN_FILENO;
    char strbuf[128];

    /* name the main thread */
    sprintf(strbuf, "dbgsrv:main");
    prctl(PR_SET_NAME, strbuf, 0, 0, 0);

    /* register signals for clean shutdown */
    signal(SIGINT, shutdown_handler);
    signal(SIGHUP, shutdown_handler);
    signal(SIGTERM, shutdown_handler);

    dbgsrv_glob = dbgsrv_init();
    if (!dbgsrv_glob) {
        NS_LOG(NS_LOG_LVL_FATAL, "Failed to allocate server data");
        return -1;
    }

    /* load command line options */
    ret = dbgsrv_load_options(dbgsrv_glob, argc, argv);
    if (ret) {
        dbgsrv_print_usage(argv[0]);
        if (ret < 0)
            return exit_cleanup(-1);
        else
            return exit_cleanup(0);
    }

    /* initialize logging */
    logging_init(dbgsrv_glob->opt_loglevel,
                 dbgsrv_glob->opt_logfile,
                 dbgsrv_glob->opt_usesyslog,
                 dbgsrv_glob->opt_forcestdout);

    NS_LOG(NS_LOG_LVL_INFO, "Starting NFP Debug Server");

    if (dbgsrv_glob->opt_daemonize) {
        int pid, sid;
        /* Fork off the parent process */
        pid = fork();
        if (pid < 0) {
            NS_LOG(NS_LOG_LVL_FATAL, "fork() failed - %s", strerror(errno));
            return exit_cleanup(-1);
        }

        /* If we got a good PID, then
           we can exit the parent process. */
        if (pid > 0) {
            exit(0);
        }

        umask(0);

        sid = setsid();
        if (sid < 0) {
            /* Log the failure */
            NS_LOG(NS_LOG_LVL_FATAL, "setsid() failed - %s", strerror(errno));
            return exit_cleanup(-1);
        }

        close(STDIN_FILENO);
        close(STDOUT_FILENO);
        close(STDERR_FILENO);

        stdin_fd = -1;
    } else {
        if (dbgsrv_glob->opt_nostdin) {
            close(STDIN_FILENO);

            stdin_fd = -1;
        }

    }

    if (create_pidfile(argv[0], dbgsrv_glob->opt_pidfile))
        return exit_cleanup(-1);


    /* TODO: spawn as daemon or not here */

    if (dbgsrv_do_setup(dbgsrv_glob) < 0) {
        NS_LOG(NS_LOG_LVL_FATAL, "failed to initialize server data");
        return exit_cleanup(-1);
    }

    while (!do_shutdown)
    {
        ret = select_all(stdin_fd, dbgsrv_glob->ipcsrv_fd, 200);
        if (ret < 0) {
            NS_LOG(NS_LOG_LVL_FATAL, "error during select()");
            break;
        }
        if (!ret) {
            /* timeout */
            continue;
        }

        if (ret & ACTIVITY_STDIN)
            grab_stdin(stdin_fd, &do_shutdown);

        if (ret & ACTIVITY_SRV) {
            /* Wait for IPC client connection */
            ret = ns_ipc_server_accept_client(&dbgsrv_glob->ipc_h);
            if (ret == 0) {
                NS_LOG(NS_LOG_LVL_DEBUG, "got new IPC client");
            } else {
                NS_LOG(NS_LOG_LVL_FATAL, "error while accepting IPC client");
                break;
            }
        }
    }

    NS_LOG(NS_LOG_LVL_INFO, "Shutting down NFP Debug Server");

    return exit_cleanup(0);
}
