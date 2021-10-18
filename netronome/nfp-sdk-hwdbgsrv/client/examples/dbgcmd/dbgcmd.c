/*
 * Copyright (C) 2014, Netronome Systems, Inc.  All rights reserved.
 */
#include <errno.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>

#include <pthread.h>
#include <ncurses.h>

#include <nfp6000/nfp_me.h>
#include <nfp-common/nfp_resid.h>
#include <nfp.h>
#include <nfp_hwdbg.h>

#include "dbgcmd.h"

/**** Defines *****/

/* number of bits per ustore word (minus ECC), for listfile verification */
#define USTORE_ECC_WORDSIZE 45

/**** Globals *****/

/* pointer to global state, used from server event callback */
struct dbgcmd_data *dbgcmd_glob = NULL;

/**** Helper functions for interacting with Debug Server *****/

/* check if a local break exists */
int dbgcmd_break_exists(struct dbgcmd_data *dd, int pc, int ctx)
{
    struct break_list_entry *e;
    int meid = dd->medata[dd->cur_me].meid;

    for (e = dd->breaks; e; e = e->next) {
        if (e->pc != pc)
            continue;

        if (e->meid != meid)
            continue;

        if (e->ctx != -1) {
            if (e->ctx != ctx) {
                continue;
            }
        }

        /* break exists */
        return 1;
    }

    /* break doesn't exist */
    return 0;
}

/* register a break on the server and save a local reference */
int dbgcmd_break_add(struct dbgcmd_data *dd, int pc, int ctx)
{
    struct break_list_entry *e;
    int meid = dd->medata[dd->cur_me].meid;
    int brctx;

    if (ctx < 0)
        brctx = 0xff;
    else
        brctx = 1 << ctx;

    if (nfp_hwdbg_brk_add_code(dd->hwdbg, dd->breakid, meid, pc, brctx) < 0)
        return -1;

    e = malloc(sizeof(*e));

    if (!e) {
        errno = ENOMEM;
        return -1;
    }

    e->meid = meid;
    e->pc = pc;
    e->breakid = dd->breakid++;
    e->ctx = ctx;
    e->next = dd->breaks;
    dd->breaks = e;

    return 0;
}

/* Delete a break */
int dbgcmd_break_del(struct dbgcmd_data *dd, int pc, int ctx)
{
    struct break_list_entry **e, *n;
    int meid = dd->medata[dd->cur_me].meid;
    int found = 0;
    int64_t breakid;

    e = &dd->breaks;
    while (*e) {
        if ((*e)->pc == pc && (*e)->meid == meid) {
            if (ctx == -1 || (*e)->ctx == ctx) {
                n = (*e)->next;
                breakid = (*e)->breakid;
                free(*e);
                *e = n;
                found = 1;
                break;
            }
        } else
            e = &((*e)->next);
    }

    if (!found) {
        errno = ENOENT;
        return -1;
    }

    if (nfp_hwdbg_brk_del(dd->hwdbg, breakid) < 0)
        return -1;

    return 0;
}

/* Clear (not delete) a server break;
 * we need to do this to acknowledge a break so run controls work again */
int dbgcmd_break_clear(struct dbgcmd_data *dd, int breakid)
{
    return nfp_hwdbg_brk_clear(dd->hwdbg, breakid);
}

/* Clear all breaks */
int dbgcmd_break_clearall(struct dbgcmd_data *dd)
{
    struct nfp_hwdbg_break_data data[256];
    int i, cnt;
    int ret;
    ret = nfp_hwdbg_brk_check(dd->hwdbg, data, 256, &cnt);
    if (ret < 0)
        return -1;

    for (i = 0; i < cnt; i++)
        nfp_hwdbg_brk_clear(dd->hwdbg, data[i].breakid);

    return 0;
}

/* Get the server revision string */
int dbgcmd_get_version(struct dbgcmd_data *dd, char *buf, int len)
{
    return nfp_hwdbg_server_version(dd->hwdbg, buf, len);
}

/* Get an ME's status from the server */
void dbgcmd_update_me_status(struct dbgcmd_data *dd, int ctx)
{
    struct nfp_hwdbg_me_status_data data;
    struct dbgcmd_medata *md;
    int meid;

    md = &dd->medata[dd->cur_me];

    meid = md->meid;

    if (nfp_hwdbg_me_status(dd->hwdbg, &meid, &data, 1) < 0)
        return;

    md->me_stopped = data.state != NFP_HWDBG_ME_STATUS_ACTIVE;
    if (md->me_stopped) {
        if (ctx < 0 || ctx >= 8) {
            md->me_cur_pc = data.pc;
            md->me_cur_ctx = data.ctx;
        } else {
            md->me_cur_pc = data.ctxpc[ctx];
            md->me_cur_ctx = ctx;
        }
    } else {
        md->me_cur_pc = -1;
        md->me_cur_ctx = -1;
    }
}

/* Reset server data, normally done around a firmware load */
int dbgcmd_srvreset(struct dbgcmd_data *dd, int type)
{
    nfp_hwdbg_srv_reset(dd->hwdbg, type);

    dbgcmd_update_me_status(dd, -1);

    if (type == NFP_HWDBG_SRV_RESET_TYPE_HARDRESET) {
        struct break_list_entry *e, *n;
        for (e = dd->breaks; e; e = n) {
            n = e->next;
            free(e);
        }
        dd->breaks = NULL;
    }

    return 0;
}

/* Step an ME context */
int dbgcmd_step(struct dbgcmd_data *dd, int ctx)
{
    int ret, err;
    int meid = dd->medata[dd->cur_me].meid;
    ret = nfp_hwdbg_me_step(dd->hwdbg, meid, ctx);
    err = errno;
    dbgcmd_update_me_status(dd, -1);
    errno = err;
    return ret;
}

/* Run an ME */
int dbgcmd_run(struct dbgcmd_data *dd, int meid)
{
    int ret, err;

    if (meid == 0)
        meid = dd->medata[dd->cur_me].meid;

    ret = nfp_hwdbg_me_run(dd->hwdbg, meid, 0);
    err = errno;
    dbgcmd_update_me_status(dd, -1);
    errno = err;
    return ret;
}

/* Stop an ME */
int dbgcmd_stop(struct dbgcmd_data *dd, int meid)
{
    int ret, err;

    if (meid == 0)
        meid = dd->medata[dd->cur_me].meid;

    ret = nfp_hwdbg_me_stop(dd->hwdbg, meid);
    err = errno;
    dbgcmd_update_me_status(dd, -1);
    errno = err;
    return ret;
}

/**** Event callback, asynchronous interactions with Debug Server ****/

static void hwdbg_event_handler(struct nfp_hwdbg *hwdbg,
                                uint32_t ev_type,
                                void *ev_data,
                                uint32_t ev_data_len)
{
    struct nfp_hwdbg_break_data *bd = ev_data;
    struct nfp_hwdbg_event_pl_groupstop *gs = ev_data;
    struct event_list_entry *e;
    int type, meid = -1, pc = -1, ctx = -1, id = -1;
    int found, i;

    switch (ev_type) {
    case NFP_HWDBG_EVENT_TYPE_RESET:
        type = EVENT_TYPE_RESET;
        break;
    case NFP_HWDBG_EVENT_TYPE_SHUTDOWN:
        type = EVENT_TYPE_SHUTDOWN;
        break;
    case NFP_HWDBG_EVENT_TYPE_GROUPSTOP:
        meid = gs->meid;
        type = EVENT_TYPE_MESTOP;
        break;
    case NFP_HWDBG_EVENT_TYPE_BREAK:
        meid = bd->meid;
        if (bd->breakid < 0)
            type = EVENT_TYPE_SWBP;
        else
            type = EVENT_TYPE_CODEBP;
        ctx = bd->ctx;
        pc = bd->pc;
        id = bd->breakid;
        break;
    default:
        return;
    }

    /* we are only interested in the events for the me we are debugging */
    found = 0;
    for (i = 0; i < dbgcmd_glob->mecnt; i++) {
        if (dbgcmd_glob->medata[i].meid != meid)
            continue;
        found = 1;
        break;
    }
    if (!found)
        return;

    /* queue up an event for the main thread to process */
    e = malloc(sizeof(*e));
    if (!e)
        return;

    e->breakid = id;
    e->meid = meid;
    e->type = type;
    e->pc = pc;
    e->ctx = ctx;

    pthread_mutex_lock(&dbgcmd_glob->ev_mtx);
    e->next = dbgcmd_glob->events;
    dbgcmd_glob->events = e;
    pthread_mutex_unlock(&dbgcmd_glob->ev_mtx);
}

/**** Command line argument handling ****/

static void print_usage(struct dbgcmd_data *dd)
{
    printf("usage %s\n", dd->progname);
    printf("-h    : this message\n"
           "-n    : nfp device number (default: 0)\n"
           "-m    : meid (default: mei0.me0)\n"
           "-e    : elffile with debug enabled\n"
           "-l    : listfile\n");
    return;
}

static int alloc_medata(struct dbgcmd_data *dd, const char *mestr)
{
    char buf[NFP_MEID_STR_SZ];
    struct dbgcmd_medata *md;

    if (dd->mecnt == MAX_MES) {
        fprintf(stderr, "Too many MEs specified\n");
        return -1;
    }

    md = &dd->medata[dd->mecnt];

    if (isdigit(mestr[0])) { /* meid is a number */
        md->meid = strtol(mestr, NULL, 0);
        md->meid_str = strdup(nfp6000_meid2str(buf, md->meid));
    } else { /* meid is a string */
        md->meid_str = strdup(mestr);
        md->meid = nfp6000_idstr2meid(mestr, NULL);
    }

    if (!md->meid_str) {
        fprintf(stderr, "failed to allocate MEID string\n");
        return -1;
    }

    md->index = dd->mecnt;
    md->cur_line = 0;
    md->linecount = 0;
    md->listfile = NULL;
    md->me_stopped = 0;
    md->me_cur_pc = 0;
    md->me_cur_ctx = 0;

    md->brk_pc = 0;

    dd->mecnt++;

    return 0;
}

static int parseopts(struct dbgcmd_data *dd, int argc, char **argv)
{
    int o;
    dd->progname = argv[0];

    dd->opt_nfp = DEFAULT_NFP;
    dd->opt_elffile = NULL;
    while ((o = getopt(argc, argv, "hn:m:l:e:")) != -1) {
        switch (o) {
        case 'h':
            print_usage(dd);
            exit(EXIT_SUCCESS);
            break;
        case 'n':
            dd->opt_nfp = strtol(optarg, NULL, 0);
            break;
        case 'm':
            alloc_medata(dd, optarg);
            break;
        case 'e':
            dd->opt_elffile = optarg;
            break;
        case 'l':
            /* if no me has been allocated yet, allocate the default */
            if (dd->mecnt == 0) {
                if (alloc_medata(dd, DEFAULT_MEID_STR) < 0) {
                    exit(EXIT_FAILURE);
                }
            }
            if (dd->medata[dd->mecnt - 1].listfile) {
                fprintf(stderr, "cannot assign multiple list files to an ME\n");
                print_usage(dd);
                exit(EXIT_FAILURE);
            }
            /* tack the list file onto the head */
            dd->medata[dd->mecnt - 1].listfile = optarg;
            break;
        default:
            fprintf(stderr, "unrecognized option\n");
            print_usage(dd);
            exit(EXIT_FAILURE);
        }
    }

    /* if no me has been allocated yet, allocate the default */
    if (dd->mecnt == 0) {
        if (alloc_medata(dd, DEFAULT_MEID_STR) < 0) {
            exit(EXIT_FAILURE);
        }
    }

    return 0;
}

/**** Free up the debug command handle ****/

static void dbgcmd_free(struct dbgcmd_data *dd)
{
    struct break_list_entry *be, *bn;
    struct event_list_entry *ee, *en;
    int i, m;

    for (m = 0; m < dd->mecnt; m++) {
        struct dbgcmd_medata *md;
        md = &dd->medata[m];
        for (i = 0; md->listdata[i].data; i++)
            free(md->listdata[i].data);

        free(md->listdata);
        free(md->meid_str);
    }

    for (be = dd->breaks; be; be = bn) {
        bn = be->next;
        nfp_hwdbg_brk_del(dd->hwdbg, be->breakid);
        free(be);
    }

    for (ee = dd->events; ee; ee = en) {
        en = ee->next;
        free(ee);
    }

    nfp_hwdbg_close(dd->hwdbg);

    pthread_mutex_destroy(&dd->ev_mtx);
}

/**** ELF Debug data loading ****/

static int load_debug_data(struct dbgcmd_data *dd)
{
    struct uof_dbg_entry *uofdbgdat, *e;
    int startpc = -1, endpc = -1;
    int found;
    int i, n = 0;

    if (uof_parse(dd->opt_elffile, &uofdbgdat) < 0)
        return -1;

    for (e = uofdbgdat; e; e = e->next) {

        /* check if we are debugging the ME */
        found = 0;
        for (i = 0; i < dd->mecnt; i++) {
            if (dd->medata[i].meid != e->meid)
                continue;
            found = 1;
            break;
        }

        if (!found)
            continue; /* elf data not for a selected ME */

        /* figure out the start and end pc */
        for (i = 0; i < 1024*16; i++) {
            if (!e->vld[i])
                continue;
            startpc = i;
            break;
        }
        for (i = 1024*16 - 1; i >= 0; i--) {
            if (!e->vld[i])
                continue;
            endpc = i;
            break;
        }
        if (startpc < 0 || endpc < 0) {
            fprintf(stderr, "invalid pc range for me%x: %d -> %d\n",
                    e->meid, startpc, endpc);
            return -1;
        }
        if (nfp_hwdbg_load_dbginfo(dd->hwdbg, e->meid,
                                   e->data + startpc,
                                   endpc - startpc + 1,
                                   startpc)) {
            fprintf(stderr,
                    "failed to load dbginfo for me%x (%s)\n",
                    e->meid, strerror(errno));
            return -1;
        }
        n++;
    }

    if (n != dd->mecnt) {
        fprintf(stderr,
                "debug data not present for selected MEs\n");
        return -1;
    }

    free(uofdbgdat);

    return 0;
}

/**** main ****/

int main(int argc, char **argv)
{
    struct dbgcmd_data dbgdat;
    int i;

    dbgcmd_glob = &dbgdat;

    dbgdat.quit = 0;
    dbgdat.searchstr[0] = '\0';
    dbgdat.breaks = NULL;
    dbgdat.events = NULL;
    dbgdat.norefresh = 0;
    dbgdat.first = 1;
    dbgdat.breakid = 0;
    dbgdat.forcechar = -1;
    dbgdat.version = "Unknown";

    if (pthread_mutex_init(&dbgdat.ev_mtx, NULL) < 0) {
        fprintf(stderr, "failed to initialize mutex");
        return EXIT_FAILURE;
    }

    dbgdat.mecnt = 0;
    dbgdat.cur_me = 0; /* always start debugging the first ME */

    parseopts(&dbgdat, argc, argv);

    for (i = 0; i < dbgdat.mecnt; i++) {
        if (listfile_parse(&dbgdat.medata[i]) < 0) {
            fprintf(stderr, "failed to load list file");
            return EXIT_FAILURE;
        }
    }

    dbgdat.hwdbg = nfp_hwdbg_open(dbgdat.opt_nfp);
    if (!dbgdat.hwdbg) {
        fprintf(stderr,
                "failed to open hw debug handle (%s)\n",
                strerror(errno));
        return EXIT_FAILURE;
    }

    /* attempt to load up the debug info from an elf file */
    if (dbgdat.opt_elffile) {
        if (load_debug_data(&dbgdat) < 0)
            return EXIT_FAILURE;
    }
    nfp_hwdbg_subs_events_all(dbgdat.hwdbg, &hwdbg_event_handler);

    dbgcmd_update_me_status(&dbgdat, -1);

    gui_init(&dbgdat);

    /* sometimes breaks have hit before we connected */
    dbgcmd_break_clearall(&dbgdat);

    /* run the main loop */
    while (!dbgdat.quit) {
        if (gui_loop(&dbgdat) == 1)
            usleep(10000);
    }

    gui_free(&dbgdat);
    dbgcmd_free(&dbgdat);

    return EXIT_SUCCESS;
}
