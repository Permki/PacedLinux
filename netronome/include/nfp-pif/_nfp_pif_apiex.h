/*
 * Copyright (C) 2014,  Netronome Systems, Inc.  All rights reserved.
 *
 * @file          _nfp_apiex.h
 *
 */
#ifndef __NFP_PIF_APIEX_H__
#define __NFP_PIF_APIEX_H__

/* Import/Export symbols for Windows and set visibility for GCC */
#if defined(NFP_PIF_LIB_COMPILE)
#  if defined(_WIN32) || defined (__CYGWIN__)
#    ifdef COMPILING_PIF_DLL
#      define PIF_PUBLIC __declspec(dllexport)
#    else
#      define PIF_PUBLIC __declspec(dllimport)
#    endif
#    define PIF_LOCAL
#  else
#    if __GNUC__ >= 4
#      define PIF_PUBLIC __attribute__((visibility("default")))
#      define PIF_LOCAL __attribute__((visibility("hidden")))
#    else
#      define PIF_PUBLIC
#      define PIF_LOCAL
#    endif
#  endif
#else
#  define PIF_PUBLIC
#  define PIF_LOCAL
#endif

#endif /* __NFP_PIF_APIEX_H__ */
