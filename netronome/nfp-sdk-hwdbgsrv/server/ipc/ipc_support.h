/*
 * Copyright (C) 2014, Netronome Systems, Inc.  All rights reserved.
 */

#ifndef __IPC_SUPPORT_H__
#define __IPC_SUPPORT_H__

extern int do_api_version_checks;

void ipcclient_trace(void *handle, const char *funcname);

int ipcclient_version_check(const char *api_name,
                            int server_version,
                            int server_compat,
                            int client_version,
                            uint32_t *version_reported);

struct ipc_server_fmap {
    char *name;
    ns_ipc_server_stubfunc_t func;
};

int ipcclient_check_handle(ns_ipc_server_conn_handle_t con, void *h);
int ipcclient_add_handle(ns_ipc_server_conn_handle_t con,
                            void *h, int type,
                            int (*init_cb)(void *h),
                            void (*free_cb)(void *h));
int ipcclient_del_handle(ns_ipc_server_conn_handle_t con, void *h);

#endif /* __IPC_SUPPORT_H__ */
