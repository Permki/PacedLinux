/*
 * Copyright (C) 2014,  Netronome Systems, Inc.  All rights reserved.
 *
 * @file          _nfp_nffw_apiex.h
 *
 */
#ifndef __NFP_NBI_APIEX_H__
#define __NFP_NBI_APIEX_H__

/* Import/Export symbols for MSC and set visibility for GCC */
#if defined(NO_NFP_NBI_API_EXPORT)
#  define NFP_NBI_API
#elif defined(_MSC_VER)
#  ifdef NFP_NBI_API_EXPORT
#    define NFP_NBI_API __declspec(dllexport)
#  else
#    define NFP_NBI_API __declspec(dllimport)
#  endif
#else
#  if __GNUC__ >= 4
#    define NFP_NBI_API __attribute__((visibility("default")))
#  else
#    define NFP_NBI_API
#  endif
#endif

#endif /* __NFP_NBI_APIEX_H__ */
