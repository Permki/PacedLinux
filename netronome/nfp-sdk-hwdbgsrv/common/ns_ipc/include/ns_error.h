/**
* Copyright (C) 2012 Netronome Systems, Inc.  All rights reserved.
*
* @file ns_error.h
* IPC error messages.
*/
#ifndef _NS_ERROR_H_
#define _NS_ERROR_H_

#ifdef __cplusplus
extern "C" {
#endif

enum {
    NS_ERR_SUCCESS = 0,
    NS_ERR_FAIL,
    NS_ERR_INVALID_PARAMETER,
    NS_ERR_OUT_OF_MEMORY,
    NS_ERR_NOT_FOUND,
    NS_ERR_CLIENT_COMMS,
    NS_ERR_SERVER_COMMS,
    NS_ERR_FINAL
};

extern char *ns_error_str[NS_ERR_FINAL];

#define NS_ERR_STR(x) (ns_error_str[(x)])

#ifdef __cplusplus
};
#endif

#endif /* __NS_ERROR_H__ */
