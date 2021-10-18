/**
* Copyright (C) 2012 Netronome Systems, Inc.  All rights reserved.
*
* @file ns_error.c
* IPC error message instantiation.
*/
#include <ns_error.h>

char *ns_error_str[NS_ERR_FINAL] = {
        "Success",                     /* NS_ERR_SUCCESS */
        "General failure",             /* NS_ERR_FAIL */
        "Invalid parameter",           /* NS_ERR_INVALID_PARAMETER */
        "Out of memory",               /* NS_ERR_OUT_OF_MEMORY */
        "Entry not found",             /* NS_ERR_NOT_FOUND */
        "Client communication error",  /* NS_ERR_CLIENT_COMMS */
        "Server communication error",  /* NS_ERR_SERVER_COMMS */
    };
