/*
 * Copyright (C) 2014, Netronome Systems, Inc.  All rights reserved.
 */

#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#include "ns_ipc_buf.h"
#include "ns_ipc_server.h"
#include "logging.h"

#include "dbgsrv.h"

#include "ipc_client.h"

/************ *************/

void ipc_client_connect(ns_ipc_server_conn_handle_t h, void *ctx)
{
    ipc_client_add_client(dbgsrv_glob->ipcclient, h);
}

void ipc_client_disconnect(ns_ipc_server_conn_handle_t h, void *ctx)
{
    ipc_client_del_client(dbgsrv_glob->ipcclient, h);
}

/************ *************/

struct ipc_client *ipc_client_init(void)
{
    struct ipc_client *ret;

    ret = malloc(sizeof(*ret));
    if (!ret) {
        NS_LOG(NS_LOG_LVL_ERROR, "failed to allocate ipc_client handle");
        return NULL;
    }

    if (pthread_mutex_init(&ret->cl_mtx, NULL) < 0) {
        NS_LOG(NS_LOG_LVL_ERROR, "failed to initialize mutex for ipc_client");
        free(ret);
        return NULL;
    }

    ret->cl_list = NULL;

    return ret;
}

static void free_client_data(struct ipc_client_data *data)
{
    struct ipc_client_handle *h, *hn;
    struct ipc_client_interest *i, *in;

    for (h = data->h_list; h; h = hn) {
        hn = h->next;
        if (h->free_cb)
            h->free_cb(h->data);
        free(h);
    }
    data->h_list = NULL;

    for (i = data->i_list; i; i = in) {
        in = i->next;
        if (i->disconnect_cb)
            i->disconnect_cb(i->data, data->con);
        free(i);
    }
    data->i_list = NULL;
}

void ipc_client_free(struct ipc_client *ic)
{
    int n=0;
    struct ipc_client_data *d, *dn;

    for (d = ic->cl_list; d; d = dn) {
        dn = d->next;
        free_client_data(d);
        free(d);
        n++;
    }
    ic->cl_list = NULL;
    pthread_mutex_destroy(&ic->cl_mtx);
    free(ic);
    NS_LOG(NS_LOG_LVL_INFO, "Disconnected %d clients", n);
}

/************ *************/

int ipc_client_add_client(struct ipc_client *ic,
                          ns_ipc_server_conn_handle_t con)
{
    struct ipc_client_data *data;
    NS_LOG(NS_LOG_LVL_EXTRA, "%s: %p", __func__, con);

    data = malloc(sizeof(*data));
    if (!data) {
        NS_LOG(NS_LOG_LVL_ERROR, "ran out of memory when adding client");
        return -1;
    }
    memset(data, '\0', sizeof(*data));
    data->con = con;
    data->h_list = NULL;
    data->i_list = NULL;

    if (pthread_mutex_lock(&ic->cl_mtx) < 0) {
        NS_LOG(NS_LOG_LVL_ERROR, "failed to acquire lock when adding client");
        free(data);
        return -1;
    }
    /* insert at the front */
    data->next = ic->cl_list;
    ic->cl_list = data;
    pthread_mutex_unlock(&ic->cl_mtx);

    return 0;
}

int ipc_client_del_client(struct ipc_client *ic,
                          ns_ipc_server_conn_handle_t con)
{
    struct ipc_client_data **c, **cn;
    struct ipc_client_data *tofree = NULL;

    /* identify matching client, remove from list and then free */

    NS_LOG(NS_LOG_LVL_EXTRA, "%s: %p", __func__, con);
    if (pthread_mutex_lock(&ic->cl_mtx) < 0) {
        NS_LOG(NS_LOG_LVL_ERROR, "failed to acquire lock when deleting client");
        return -1;
    }

    for (c = &ic->cl_list; *c; c = cn) {
        cn = &(*c)->next;

        if ((*c)->con == con) {
            tofree = *c;
            *c = *cn;
            break;
        }
    }
    pthread_mutex_unlock(&ic->cl_mtx);

    if (!tofree) /* none found */
        return -1;

    free_client_data(tofree);
    free(tofree);

    return 0;
}

/*
 * Interests - a mechanism to notify a party about a disconnected client
 */

int ipc_client_add_interest(struct ipc_client *ic,
                            ns_ipc_server_conn_handle_t con,
                            void *handle,
                            void (*disc_cb) (void *h,
                                             ns_ipc_server_conn_handle_t con))
{
    struct ipc_client_data *c;
    struct ipc_client_interest *i_list = NULL;
    struct ipc_client_interest *interest;

    NS_LOG(NS_LOG_LVL_EXTRA, "%s: %p %p()", __func__, handle, disc_cb);

    if (pthread_mutex_lock(&ic->cl_mtx) < 0) {
        NS_LOG(NS_LOG_LVL_ERROR, "failed to acquire lock when adding interest");
        return -1;
    }

    /* find a matching client connection */
    for (c = ic->cl_list; c; c++) {
        if (c->con == con) {
            i_list = c->i_list;
            break;
        }
    }

    if (!i_list) {
        NS_LOG(NS_LOG_LVL_ERROR,
               "could not find client connectiond when adding interest");
        pthread_mutex_unlock(&ic->cl_mtx);
        return -1;
    }

    interest = malloc(sizeof(*interest));
    if (!interest) {
        NS_LOG(NS_LOG_LVL_ERROR, "failed to allocate interest for client");
        pthread_mutex_unlock(&ic->cl_mtx);
        return -1;
    }

    interest->data = handle;
    interest->disconnect_cb = disc_cb;
    /* insert at front */
    interest->next = i_list;
    i_list = interest;

    pthread_mutex_unlock(&ic->cl_mtx);

    return 0;
}

int ipc_client_del_interest(struct ipc_client *ic,
                            ns_ipc_server_conn_handle_t con,
                            void *handle)
{
    struct ipc_client_data *c;
    struct ipc_client_interest **i, **in, **i_list = NULL;
    int ret = -1;

    NS_LOG(NS_LOG_LVL_EXTRA, "%s: %p", __func__, handle);

    if (pthread_mutex_lock(&ic->cl_mtx) < 0) {
        NS_LOG(NS_LOG_LVL_ERROR,
               "failed to acquire lock when deleting interest");
        return -1;
    }

    for (c = ic->cl_list; c; c = c->next) {
        if (c->con == con) {
            i_list = &c->i_list;
            break;
        }
    }

    if (!i_list) {
        pthread_mutex_unlock(&ic->cl_mtx);
        return -1;
    }

    for (i = i_list; *i; i = in) {
        in = &(*i)->next;
        if ((*i)->data == handle) {
            free(*i);
            *i = *in;
            ret = 0;
            break;
        }
    }

    pthread_mutex_unlock(&ic->cl_mtx);

    return ret;
}


/************ *************/

int ipc_client_check_handle(struct ipc_client *ic,
                            ns_ipc_server_conn_handle_t con,
                            void *handle)
{
    struct ipc_client_data *c;
    struct ipc_client_handle *h, *h_list = NULL;
    int found = 0;

    NS_LOG(NS_LOG_LVL_HEAVY, "%s: %p", __func__, handle);

    if (pthread_mutex_lock(&ic->cl_mtx) < 0) {
        NS_LOG(NS_LOG_LVL_ERROR,
               "failed to acquire lock when checking handle");
        return -1;
    }

    /* go through connected clients */
    for (c = ic->cl_list; c; c = c->next) {
        if (c->con == con) {
            h_list = c->h_list;
            break;
        }
    }

    if (!h_list) { /* no matching connection */
        pthread_mutex_unlock(&ic->cl_mtx);
        return 0;
    }

    /* check handle against those registered to a connection */
    for (h = h_list; h; h = h->next) {
        if (h->data == handle) {
            found = 1;
            break;
        }
    }
    pthread_mutex_unlock(&ic->cl_mtx);

    return found;
}

int ipc_client_del_handle(struct ipc_client *ic,
                          ns_ipc_server_conn_handle_t con,
                          void *handle)
{
    struct ipc_client_data * c;
    struct ipc_client_handle **h_list = NULL, **h, **hn, *hp;
    int ret = -1;

    NS_LOG(NS_LOG_LVL_EXTRA, "%s: %p", __func__, handle);

    if (pthread_mutex_lock(&ic->cl_mtx) < 0) {
        NS_LOG(NS_LOG_LVL_ERROR,
               "failed to acquire lock when deleting handle");
        return -1;
    }

    /* find the matching connection */
    for (c = ic->cl_list; c; c = c->next) {
        if (c->con == con) {
            h_list = &c->h_list;
            break;
        }
    }

    if (!h_list) {
        pthread_mutex_unlock(&ic->cl_mtx);
        return -1;
    }

    /* find the matching handle,
     * call the handle destuctor,
     * then delete the entry */
    for (h = h_list; (*h); h = hn) {
        hn = &(*h)->next;
        if ((*h)->data == handle) {
            if ((*h)->free_cb)
                (*h)->free_cb(handle);
            hp = *h;
            *h = *hn;
            free(hp);
            ret = 0;
            break;
        }
    }

    pthread_mutex_unlock(&ic->cl_mtx);

    return ret;
}

int ipc_client_add_handle(struct ipc_client *ic,
                          ns_ipc_server_conn_handle_t con,
                          void *handle,
                          int (*init_cb)(void *h),
                          void (*free_cb)(void *h))
{
    struct ipc_client_data *c;
    struct ipc_client_handle **h_list = NULL;
    struct ipc_client_handle *hent;

    NS_LOG(NS_LOG_LVL_EXTRA, "%s: %p %p()", __func__, handle, free_cb);

    if (pthread_mutex_lock(&ic->cl_mtx) < 0) {
        NS_LOG(NS_LOG_LVL_ERROR,
               "failed to acquire lock when adding handle");
        return -1;
    }

    for (c = ic->cl_list; c; c = c->next) {
        if (c->con == con) {
            h_list = &c->h_list;
            break;
        }
    }

    if (!h_list) {
        NS_LOG(NS_LOG_LVL_ERROR,
               "could not find client connection when adding handle");
        pthread_mutex_unlock(&ic->cl_mtx);
        if (free_cb)
            free_cb(handle);
        return -1;
    }

    hent = malloc(sizeof(*hent));
    if (!hent) {
        NS_LOG(NS_LOG_LVL_ERROR, "failed to allocate handle for client");
        pthread_mutex_unlock(&ic->cl_mtx);
        if (free_cb)
            free_cb(handle);
        return -1;
    }

    hent->data = handle;
    hent->free_cb = free_cb;

    /* insert at front of list */
    hent->next = *h_list;
    *h_list = hent;

    pthread_mutex_unlock(&ic->cl_mtx);

    /* if there is a post client connection init, run it */
    if (init_cb)
        init_cb(handle);

    return 0;
}

ns_ipc_server_conn_handle_t ipc_client_get_con(struct ipc_client *ic,
                                               void *handle)
{
    struct ipc_client_data *c;
    struct ipc_client_handle *h;
    ns_ipc_server_conn_handle_t *con;

    if (pthread_mutex_lock(&ic->cl_mtx) < 0) {
        NS_LOG(NS_LOG_LVL_ERROR,
               "failed to acquire lock when looking up connection");
        return NULL;
    }

    for (c = ic->cl_list; c; c = c->next) {
        for (h = c->h_list; h; h = h->next) {
            if (h->data == handle) {
                con = c->con;
                pthread_mutex_unlock(&ic->cl_mtx);
                return con;
            }
        }
    }
    pthread_mutex_unlock(&ic->cl_mtx);
    return NULL;
}
