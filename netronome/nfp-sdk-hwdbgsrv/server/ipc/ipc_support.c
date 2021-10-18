/*
 * Copyright (C) 2014, Netronome Systems, Inc.  All rights reserved.
 */

#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ns_ipc_buf.h"
#include "ns_ipc_server.h"

#include "logging.h"

#include "dbgsrv.h"

#if 0
/* TODO: version checking disabled at the moment, enable soon */
int do_api_version_checks = 1; //<----- set to 1 to enable again
#else
int do_api_version_checks = -1;
#endif
static int warned_once = 0;

int ipcclient_version_check(const char *api_name,
                            int server_version,
                            int server_compat,
                            int client_version,
                            uint32_t *version_reported)
{
    int err = 0;

    if (server_compat) {
        /* if compat is sent, client must be in the range of compat & version */
        if (client_version < server_compat)
            err = 1;

        if (client_version > server_version)
            err = 1;
    } else {
        if (client_version != server_version)
            err = 1;
    }

    if (!err) {
        /* version in compat range */
        *version_reported = client_version;
        return 0;
    }

    if (do_api_version_checks <= 0) {
        if (!warned_once)
            NS_LOG(NS_LOG_LVL_WARN,
                    "\n\tRPC client version mismatch for API %s detected:"
                    "\n\tclient version %d, server compat %d, server version %d."
                    "\n\tIgnoring mismatch, no further warnings will be generated",
                    api_name, client_version, server_compat, server_version);
        warned_once = 1;

        if (do_api_version_checks == -1) /* client will always see success */
            *version_reported = client_version;
        else /* server succeeds, but client may report failure or warning */
            *version_reported = server_version;

        return 0; /* return success, even on mismatch */
    }

    *version_reported = server_version;

    if (!warned_once)
        NS_LOG(NS_LOG_LVL_ERROR,
                "\n\tRPC client version mismatch for API %s detected:"
                "\n\tclient version %d, server compat %d, server version %d."
                "\n\tNo further error messages will be generated",
                api_name, client_version, server_compat, server_version);

    warned_once = 1;

    return -1;
}


int ipcclient_check_handle(ns_ipc_server_conn_handle_t con, void *h)
{
    return ipc_client_check_handle(dbgsrv_glob->ipcclient, con, h);
}

int ipcclient_add_handle(ns_ipc_server_conn_handle_t con,
                         void *h, int type,
                         int (*init_cb)(void *data),
                         void (*free_cb)(void *data))
{
    return ipc_client_add_handle(dbgsrv_glob->ipcclient, con, h,
                                 init_cb, free_cb);
}


int ipcclient_del_handle(ns_ipc_server_conn_handle_t con, void *h)
{
    return ipc_client_del_handle(dbgsrv_glob->ipcclient, con, h);
}

void ipcclient_trace(void *handle, const char *funcname)
{
    NS_LOG(NS_LOG_LVL_DEBUG, "client %p called %s", handle, funcname);
}
