/** @file ns_log.h
 * @brief Logging API.
 *
 * Copyright (C) 2012,2017 Netronome Systems, Inc.  All rights reserved.
 */
#ifndef _NS_LOG_H_
#define _NS_LOG_H_

#include <stdint.h>
#include <inttypes.h>
#include <nfp-common/nfp_platform.h>

#include "_nfp_common_apiex.h"

__BEGIN_DECLS

/* This envvar will be scanned at init time for setting the level */
#define NS_LOG_LEVEL_ENVVAR "NS_LOG_LEVEL"
#define NS_LOG_FLAGS_ENVVAR "NS_LOG_FLAGS"

enum NS_LOG_LVLS {
	NS_LOG_LVL_NONE = 0, /* no logging */
	NS_LOG_LVL_FATAL,
	NS_LOG_LVL_ERROR,
	NS_LOG_LVL_WARN,
	NS_LOG_LVL_INFO,
	NS_LOG_LVL_DEBUG,
	NS_LOG_LVL_EXTRA,
	NS_LOG_LVL_HEAVY,
	NS_LOG_LVL_VERBOSE,
	NS_LOG_LVL_COUNT, /* end of enum indicator */
	NS_LOG_LVL_INVALID = NS_LOG_LVL_COUNT,
};

/**** Flags for ns_log_init ****/

/** No flags */
#define NS_LOG_FLAG_NONE (0)
/** Strip the path out of the source file */
#define NS_LOG_FLAG_STRIPFILE BIT(1)
/** Print the designator identifier */
#define NS_LOG_FLAG_IDENT BIT(2)
/** Print the data and time */
#define NS_LOG_FLAG_TIME BIT(3)
/** Force STDOUT even if a callback is present */
#define NS_LOG_FLAG_FORCESTDOUT BIT(4)
/** Print log level at the start of the line, e.g [E] for error */
#define NS_LOG_FLAG_PRINTLEVELTAG BIT(5)

/****  ****/

/** Module value to disable per module logging with ns_log_set_module_level() */
#define NS_LOG_MODULE_DISABLE_ALL (-1)

/**** ****/
/** Maximum number of log modules */
#define NS_LOG_MODULE_MAX 64

/* init logging with flags */
NFP_COMMON_API
int ns_log_init(unsigned int flags);

/* set an identifier; this will get prepended to the log messages and will
 * identify the source of the message
 */
NFP_COMMON_API
void ns_log_set_identifier(const char *id);

/* register a logging callback; logging then takes place within the registered
 * callback
 */
NFP_COMMON_API
void ns_log_set_callback(void (*cb)(int lvl,
				    const char *file,
				    unsigned int line,
				    const char *func,
				    const char *logstr));

/* print a log message */
NFP_COMMON_API
void ns_log_module_print(int module,
			 int lvl,
			 const char *file,
			 unsigned int line,
			 const char *func,
			 const char *format,
			 ...);

NFP_COMMON_API
void ns_log_set_level(int level);

NFP_COMMON_API
void ns_log_set_module_level(int module, int level);

NFP_COMMON_API
int ns_log_get_level(void);

NFP_COMMON_API
int ns_log_get_module_level(int module);

#ifdef NS_LOG_MODULE
#if (((NS_LOG_MODULE) < 0) || ((NS_LOG_MODULE >= NS_LOG_MODULE_MAX)))
#error NS_LOG_MODULE number out of range
#endif /* (((NS_LOG_MODULE) < 0) || ((NS_LOG_MODULE >= NS_LOG_MODULE_MAX))) */
#define NS_LOG(level, str, ...)                                              \
	do {                                                                 \
		if ((level) > ns_log_get_module_level(NS_LOG_MODULE))        \
			break;                                               \
		ns_log_module_print(NS_LOG_MODULE, (level), __FILE__,        \
				    __LINE__, __func__, str, ##__VA_ARGS__); \
	} while (0)

#define NS_LOG_SET_LEVEL(level)                                  \
	ns_log_set_module_level(NS_LOG_MODULE, (level))

#define NS_LOG_GET_LEVEL() ns_log_get_module_level(NS_LOG_MODULE)

#else /* NS_LOG_MODULE */
#define NS_LOG(level, str, ...)                                                \
	do {                                                                   \
		if ((level) > ns_log_get_level())                              \
			break;                                                 \
		ns_log_module_print(-1, (level), __FILE__, __LINE__, __func__, \
				    str, ##__VA_ARGS__);                       \
	} while (0)

#define NS_LOG_SET_LEVEL(level)            \
		ns_log_set_level((level))

#define NS_LOG_GET_LEVEL() ns_log_get_level()

#endif /* !NS_LOG_MODULE */

#ifndef LOG_PERROR
#define LOG_PERROR strerror(errno)
#endif

__END_DECLS

#endif /* __NS_LOG_H__ */
