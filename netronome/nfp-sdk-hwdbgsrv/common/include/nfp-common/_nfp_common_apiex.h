/*
 * Copyright (C) 2014,  Netronome Systems, Inc.  All rights reserved.
 *
 * @file          _nfp_common_apiex.h
 *
 */
#ifndef __NFP_COMMON_APIEX_H__
#define __NFP_COMMON_APIEX_H__

/* Import/Export symbols for MSC and set visibility for GCC */
#if defined(NO_NFP_COMMON_API_EXPORT)
#  define NFP_COMMON_API
#elif defined(_MSC_VER)
#  ifdef NFP_COMMON_API_EXPORT
#    define NFP_COMMON_API __declspec(dllexport)
#  else
#    define NFP_COMMON_API __declspec(dllimport)
#  endif
#else
#  if __GNUC__ >= 4
#    define NFP_COMMON_API __attribute__((visibility("default")))
#  else
#    define NFP_COMMON_API
#  endif
#endif

#endif				/* __NFP_COMMON_APIEX_H__ */
