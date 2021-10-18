/*
 * Copyright (C) 2014, Netronome Systems, Inc.  All rights reserved.
 */

#ifndef __LOGGING_H__
#define __LOGGING_H__

#include <nfp-common/ns_log.h>

void logging_init(int lvl, char *logfilename, int use_syslog, int force_stdout);
void logging_free(void);

void logging_set_level(int lvl);

#define LOG_ERROR(...) NS_LOG(NS_LOG_LVL_ERROR, __VA_ARGS__)
#define LOG_WARN(...)  NS_LOG(NS_LOG_LVL_WARN, __VA_ARGS__)
#define LOG_INFO(...)  NS_LOG(NS_LOG_LVL_INFO, __VA_ARGS__)
#define LOG_DEBUG(...)  NS_LOG(NS_LOG_LVL_DEBUG, __VA_ARGS__)
#define LOG_EXTRA(...)  NS_LOG(NS_LOG_LVL_EXTRA, __VA_ARGS__)
#define LOG_HEAVY(...)  NS_LOG(NS_LOG_LVL_HEAVY, __VA_ARGS__)

#endif /* __LOGGING_H__ */
