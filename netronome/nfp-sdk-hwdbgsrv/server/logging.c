/*
 * Copyright (C) 2014, Netronome Systems, Inc.  All rights reserved.
 */

#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>

#include <syslog.h>
#include <pthread.h>

#include <nfp-common/ns_log.h>

#define LOG_ID "HWD"

static pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;

static FILE *logfile = NULL;

static void logcallback(int lvl,
                        const char* file,
                        unsigned int line,
                        const char* func,
                        const char* logstr)
{
    if (pthread_mutex_lock(&mtx) < 0) {
        fprintf(stderr, "failed to lock mutex\n");
        return;
    }

    /* if we are not logging to a file that means we must log to syslog */
    if (!logfile) {
        int sysloglevel;

        switch (lvl) {
            case NS_LOG_LVL_NONE:
                sysloglevel = -1;
                break;
            case NS_LOG_LVL_FATAL:
                sysloglevel = LOG_ERR;
                break;
            case NS_LOG_LVL_ERROR:
                sysloglevel = LOG_ERR;
                break;
            case NS_LOG_LVL_WARN:
                sysloglevel = LOG_WARNING;
                break;
            case NS_LOG_LVL_INFO:
                sysloglevel = LOG_INFO;
                break;
            case NS_LOG_LVL_DEBUG:
                sysloglevel = LOG_DEBUG;
                break;
            case NS_LOG_LVL_EXTRA:
                sysloglevel = LOG_NOTICE;
                break;
            case NS_LOG_LVL_HEAVY:
                sysloglevel = LOG_NOTICE;
                break;
            case NS_LOG_LVL_VERBOSE:
                sysloglevel = LOG_NOTICE;
                break;
            default:
                sysloglevel = -1;
                break;
        }
        if (sysloglevel != -1)
            syslog(sysloglevel, "%s", logstr);
        pthread_mutex_unlock(&mtx);
        return;
    }
    {
        /* log output to a file */
        struct tm tm;
        struct timeval tv;
        gettimeofday(&tv, NULL);
        tm = *localtime(&tv.tv_sec);

        fprintf(logfile, "%s: %d-%02d-%02d %02d:%02d:%02d:%03d - %s:%d %s\n",
                         LOG_ID,
                         tm.tm_year + 1900,
                         tm.tm_mon + 1,
                         tm.tm_mday,
                         tm.tm_hour,
                         tm.tm_min,
                         tm.tm_sec,
                         (int)(tv.tv_usec / 1000),
                         file,
                         line,
                         logstr);
        fflush(logfile);
    }
    pthread_mutex_unlock(&mtx);
}

void logging_set_level(int lvl)
{
    /* set log level from command line */
    NS_LOG(NS_LOG_LVL_DEBUG, "setting log level to %d", lvl);
    NS_LOG_SET_LEVEL(lvl);
}

void logging_free(void)
{
    if (logfile)
        fclose(logfile);
}

void logging_init(int lvl, char *logfilename, int use_syslog, int force_stdout)
{
    /* init logging, strip the path out of the __file__ contents */
    ns_log_init(NS_LOG_FLAG_STRIPFILE | NS_LOG_FLAG_TIME | NS_LOG_FLAG_IDENT |
                (force_stdout ? NS_LOG_FLAG_FORCESTDOUT : 0));

    ns_log_set_identifier(LOG_ID);

    if (logfilename && strlen(logfilename) > 0) {
        /* we open this with appending just in case it was written to earlier */
        logfile = fopen(logfilename, "a");
    }

    if (use_syslog && !logfile) {
        openlog(LOG_ID, LOG_ODELAY | LOG_PID, LOG_USER);
        setlogmask(LOG_UPTO(LOG_DEBUG));
    }

    if (logfile || use_syslog) {
        ns_log_set_callback(logcallback);
    }

    logging_set_level(lvl);
}
