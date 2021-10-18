/*
 * Copyright (C) 2008-2014 Netronome Systems, Inc.  All rights reserved.
 */

/*
 * ISO C99: 7.18 Integer types <stdint.h>
 */

#ifndef __STDINT_H__
#define __STDINT_H__

#ifndef NFP_LIB_ANY_NFCC_VERSION
    #if (!defined(__NFP_TOOL_NFCC) ||                       \
        (__NFP_TOOL_NFCC < NFP_SW_VERSION(5, 0, 0, 0)) ||   \
        (__NFP_TOOL_NFCC > NFP_SW_VERSION(6, 1, 0, 255)))
        #error "This standard library is not supported for the version of the SDK currently in use."
    #endif
#endif

/* Exact integral types. */

/* Integral types */
typedef signed char             int8_t;
typedef short int               int16_t;
typedef int                     int32_t;
typedef long long int           int64_t;

typedef unsigned char           uint8_t;
typedef unsigned short int      uint16_t;
typedef unsigned int            uint32_t;
typedef unsigned long long int  uint64_t;

/* Limits of integral types */
#define INT8_MIN                (-128)
#define INT16_MIN               (-32767-1)
#define INT32_MIN               (-2147483647-1)

#define INT8_MAX                (127)
#define INT16_MAX               (32767)
#define INT32_MAX               (2147483647)

#define UINT8_MAX               (255)
#define UINT16_MAX              (65535)
#define UINT32_MAX              (4294967295U)

#endif /* __STDINT_H__ */
