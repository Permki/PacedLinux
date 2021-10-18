/**
 * Copyright (C) 2012 Netronome Systems, Inc.  All rights reserved.
 *
 * @file ns_log.c
 * Logging API implementation.
 */
#include <ctype.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <time.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <sys/time.h>
#endif

#include <nfp-common/nfp_platform.h>
#include <nfp-common/ns_log.h>

/* global data */

/* keep this global for now */
int ns_log_level = NS_LOG_LVL_INFO;

/* a bit per module indicating that logging has been enabled */
uint64_t ns_log_module_enabled;

uint8_t ns_log_module_log_level[NS_LOG_MODULE_MAX] = {0};

static unsigned int g_flags;

static const char *g_identifier;

static void (*g_ns_log_cb)(int lvl,
			   const char *file,
			   unsigned int line,
			   const char *func,
			   const char *str);

static int str_to_level(const char *s)
{
	if (!strcmp("NONE", s))
		return NS_LOG_LVL_NONE;
	if (!strcmp("FATAL", s))
		return NS_LOG_LVL_FATAL;
	if (!strcmp("ERROR", s))
		return NS_LOG_LVL_ERROR;
	if (!strcmp("WARN", s))
		return NS_LOG_LVL_WARN;
	if (!strcmp("INFO", s))
		return NS_LOG_LVL_INFO;
	if (!strcmp("DEBUG", s))
		return NS_LOG_LVL_DEBUG;
	if (!strcmp("EXTRA", s))
		return NS_LOG_LVL_EXTRA;
	if (!strcmp("HEAVY", s))
		return NS_LOG_LVL_HEAVY;
	if (!strcmp("VERBOSE", s))
		return NS_LOG_LVL_VERBOSE;

	return NS_LOG_LVL_INVALID;
}

static int level_valid(int lvl)
{
	switch (lvl) {
	case NS_LOG_LVL_NONE:
		return 1;
	case NS_LOG_LVL_FATAL:
		return 1;
	case NS_LOG_LVL_ERROR:
		return 1;
	case NS_LOG_LVL_WARN:
		return 1;
	case NS_LOG_LVL_INFO:
		return 1;
	case NS_LOG_LVL_DEBUG:
		return 1;
	case NS_LOG_LVL_EXTRA:
		return 1;
	case NS_LOG_LVL_HEAVY:
		return 1;
	case NS_LOG_LVL_VERBOSE:
		return 1;
	}
	return 0;
};

int ns_log_init(unsigned int flags)
{
	/* check for NS_LOG_LEVEL_ENVVAR */
	const char *var;
#ifdef _WIN32
	char env_buf[128];
	int ret;

	ret = GetEnvironmentVariable(NS_LOG_LEVEL_ENVVAR, env_buf, 128);
	if (ret && ret < 128)
		var = env_buf;
	else
		var = NULL;
#else
	var = getenv(NS_LOG_LEVEL_ENVVAR);
#endif
	if (var) {
		int lvl;

		if (isdigit(var[0])) /* numeric log level */
			lvl = atoi(var);
		else
			lvl = str_to_level(var);

		if (level_valid(lvl))
			ns_log_level = lvl;
	}
#ifdef _WIN32
	ret = GetEnvironmentVariable(NS_LOG_FLAGS_ENVVAR, env_buf, 128);
	if (ret && ret < 128)
		var = env_buf;
	else
		var = NULL;
#else
	var = getenv(NS_LOG_FLAGS_ENVVAR);
#endif
	if (var) {
		char *c;
		unsigned int evflgs = nfp_strtou32(var, &c, 0);

		if (*c == '\0')
			flags |= evflgs;
	}

	/* set flags */
	g_flags = flags;
	return 0;
}

void ns_log_set_identifier(const char *id)
{
	g_identifier = id;
}

/* FIXME: would be nice to have the module here too */
void ns_log_set_callback(void (
	*cb)(int lvl, const char *file, unsigned int line, const char *func, const char *logstr))
{
	g_ns_log_cb = cb;
}

/* log functions */

#ifdef _WIN32
#define FILE_SLASH '\\'
#else
#define FILE_SLASH '/'
#endif

#define BUFMAX 4096
void ns_log_module_print(int module,
			 int lvl,
			 const char *file,
			 unsigned int line,
			 const char *func,
			 const char *format,
			 ...)
{
	/* Note: the level check MUST happen in a macro preceding this
	 * otherwise this will be VERY slow even without printing logs
	 *
	 * Note: the size of this buffer may have stack implications on
	 * some systems - keep that in mind
	 */
	char *file_stripped;
	char buf[BUFMAX];
	char pre[BUFMAX];
	char *p;
	int n, c;
	va_list args;
	FILE *of;

	(void)module; /* Unused warning */

	if (!file)
		file = "unknown";
	if (!func)
		func = "unknown";

	if (g_flags & NS_LOG_FLAG_STRIPFILE) {
		file_stripped = (char *)strrchr(file, FILE_SLASH);
		if (file_stripped) /* found */
			file = file_stripped + 1;
		/* file now contains __file__ without any paths */
	}

	va_start(args, format);
	vsnprintf(buf, BUFMAX, format, args);

	if (g_ns_log_cb) {
		(*g_ns_log_cb)(lvl, file, line, func, buf);
		/* if we want to force the stdout output dont return */
		if (!(g_flags & NS_LOG_FLAG_FORCESTDOUT))
			return;
	}

	if (lvl < NS_LOG_LVL_INFO)
		of = stderr;
	else
		of = stdout;

	pre[0] = '\0';
	p = pre;
	n = 0;

	if ((g_flags & NS_LOG_FLAG_PRINTLEVELTAG)) {
		char levchr;

		switch (lvl) {
		case NS_LOG_LVL_NONE:
			levchr = '!';
			break;
		case NS_LOG_LVL_FATAL:
			levchr = 'F';
			break;
		case NS_LOG_LVL_ERROR:
			levchr = 'E';
			break;
		case NS_LOG_LVL_WARN:
			levchr = 'W';
			break;
		case NS_LOG_LVL_INFO:
			levchr = 'I';
			break;
		case NS_LOG_LVL_DEBUG:
			levchr = 'D';
			break;
		case NS_LOG_LVL_EXTRA:
			levchr = 'X';
			break;
		case NS_LOG_LVL_HEAVY:
			levchr = 'H';
			break;
		case NS_LOG_LVL_VERBOSE:
			levchr = 'V';
			break;
		default:
			levchr = '?';
		}
		c = snprintf(p, BUFMAX - n, "[%c] ", levchr);
		if (c >= 0) {
			p += c;
			n += c;
		}
	}

	if ((g_flags & NS_LOG_FLAG_IDENT) && g_identifier) {
		c = snprintf(p, BUFMAX - n, "%s: ", g_identifier);
		if (c >= 0) {
			p += c;
			n += c;
		}
	}

	if ((g_flags & NS_LOG_FLAG_TIME)) {
#ifdef _WIN32
		SYSTEMTIME lt;

		GetLocalTime(&lt);
		c = snprintf(p, BUFMAX - n, "%d-%02d-%02d %02d:%02d:%02d.%03d - ", lt.wYear,
			     lt.wMonth, lt.wDay, lt.wHour, lt.wMinute, lt.wSecond,
			     lt.wMilliseconds);
#else
		struct tm tm;
		struct timeval tv;
		time_t ctime;

		gettimeofday(&tv, NULL);
		ctime = (time_t)tv.tv_sec;
		tm = *localtime(&ctime);

		c = snprintf(p, BUFMAX - n, "%d-%02d-%02d %02d:%02d:%02d.%03d - ",
			     tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min,
			     tm.tm_sec, (int)(tv.tv_usec / 1000));
#endif
		if (c >= 0) {
			p += c;
			n += c;
		}
	}

	fprintf(of, "%s%s:%d: %s\n", pre, file, line, buf);
}

void ns_log_set_level(int level)
{
	if (level_valid(level))
		ns_log_level = level;
}

void ns_log_set_module_level(int module, int level)
{
	if (module >= NS_LOG_MODULE_MAX)
		return;

	/* disable module logging */
	if (module < 0) {
		ns_log_module_enabled = 0;
		return;
	}

	if (!level_valid(level))
		return;

	/* enable per-module logging */
	ns_log_module_enabled |= (UINT64_C(1) << module);

	ns_log_module_log_level[module] = (uint8_t)level;
}

int ns_log_get_level(void)
{
	return ns_log_level;
}

int ns_log_get_module_level(int module)
{
	if (module < 0 || module >= NS_LOG_MODULE_MAX)
		return ns_log_level;

	if (!(ns_log_module_enabled & (UINT64_C(1) << module)))
		return ns_log_level;

	return ns_log_module_log_level[module];
}
