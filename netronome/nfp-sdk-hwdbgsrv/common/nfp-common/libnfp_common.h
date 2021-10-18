/*
 * Copyright (C) 2014 Netronome Systems, Inc. All rights reserved.
 * Author: Jason McMullan <jason.mcmullan@netronome.com>
 *
 */

#ifndef LIBNFP_COMMON_H
#define LIBNFP_COMMON_H

#if !defined(EMBEDDED)
/* If we're building for embedded, just don't do any of the stuff below. */

#ifdef HAVE_CONFIG_H
#  include "config.h"
#else /* Makefile.netro */
#  define HAVE_JANSSON
#  define HAVE_ZLIB
#endif

#ifdef HAVE_JANSSON
#include <jansson.h>
#else

#warning ***** BUILDING WITHOUT libjansson SUPPORT - FIRMWARE LOADING DISABLED ****

#define JANSSON_VERSION_HEX  0x020300

typedef struct {
	const char *text;
	const char *source;
	int line;
	int column;
} json_error_t;

typedef struct {
} json_t;

static inline json_t *json_object_get(const json_t *obj, const char *key)
{
	return NULL;
}

static inline int json_is_integer(const json_t *obj)
{
	return 0;
}

static inline int json_is_array(const json_t *obj)
{
	return 0;
}

static inline const char *json_string_value(const json_t *obj)
{
	return NULL;
}

static inline const char *json_object_iter_key(void *iter)
{
	return NULL;
}

static inline void *json_object_iter_next(json_t *obj, void *iter)
{
	return NULL;
}

static inline json_t *json_object_iter_value(void *iter)
{
	return NULL;
}

static inline int json_is_string(const json_t *obj)
{
	return 0;
}

static inline json_t *json_array_get(json_t *item, int index)
{
	return NULL;
}

static inline int json_array_size(json_t *arr)
{
	return 0;
}

static inline int json_integer_value(const json_t *iv)
{
	return 0;
}

static inline void json_decref(json_t *j)
{
}

static inline json_t *json_incref(json_t *j)
{
	return j;
}

static inline json_t *json_load_file(const char *path,
				     size_t flags,
				     json_error_t *err)
{
	return NULL;
}

static inline json_t *json_loads(const char *input,
				 size_t flags,
				 json_error_t *err)
{
	return NULL;
}

static inline json_t *json_loadb(const char *binary,
				 size_t len,
				 size_t flags,
				 json_error_t *err)
{
	return NULL;
}

static inline size_t json_object_size(const json_t *obj)
{
	return 0;
}

static inline void *json_object_iter(json_t *object)
{
	return NULL;
}
#endif

/* json_object_foreach() in newer libjansson uses the
 * v2.3+ only json_object_key_to_iter() function.
 * Redefine json_object_foreach() to use 2.2 and lower compatible APIs
 */
#undef json_object_foreach
#define json_object_foreach(object, key, value)                    \
	for (key = json_object_iter_key(json_object_iter(object)); \
	     key && (value = json_object_iter_value(               \
			     json_object_iter_at(object, key)));   \
	     key = json_object_iter_key(json_object_iter_next(     \
		     object, json_object_iter_at(object, key))))

#ifdef HAVE_ZLIB
#include <zlib.h>
#else
#warning ***** BUILDING WITHOUT libz SUPPORT - FIRMWARE LOADING DISABLED ****
#define Z_OK 0
#define Z_VERSION_ERROR (-6)
typedef unsigned char Byte;
typedef unsigned char Bytef;
typedef unsigned long uLong;
typedef unsigned long uLongf;
static inline int uncompress(Bytef *dest,
			     uLongf *destLen,
			     const Bytef *source,
			     uLong sourceLen)
{
	return Z_VERSION_ERROR;
}
#endif

#endif /* !defined(EMBEDDED) */

extern const unsigned long nfp_chipdata_embedded_usz;
extern const unsigned long nfp_chipdata_embedded_sz;
extern const unsigned char *nfp_chipdata_embedded;

extern const struct nfp_regdata_embedded_file *nfp_regdata_embedded;

#endif /* LIBNFP_COMMON_H */
