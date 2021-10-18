/*
 * Copyright (C) 2014, Netronome Systems, Inc.  All rights reserved.
 */
#ifndef __DBGCMD_H__
#define __DBGCMD_H__

#define DEFAULT_NFP 0
#define DEFAULT_MEID 0x204
#define DEFAULT_MEID_STR "i32.me0"

/* debug data for an ME as pulled from an elf */
struct uof_dbg_entry {
    int meid;
    uint64_t data[1024*16];
    uint8_t vld[1024*16];
    char *listfile_name;
    struct uof_dbg_entry *next;
};

struct break_list_entry {
    int breakid;
    int meid;
    int pc;
    int ctx;
    struct break_list_entry *next;
};

struct event_list_entry {
#define EVENT_TYPE_CODEBP 0
#define EVENT_TYPE_SWBP 1
#define EVENT_TYPE_MESTOP 2
#define EVENT_TYPE_SHUTDOWN 3
#define EVENT_TYPE_RESET 4
    int type;
    int meid;
    int pc;
    int ctx;
    int breakid;
    struct event_list_entry *next;
};

struct text_line {
    char *data;
#define TEXT_ATTR_BOLD      (1 << 0)
#define TEXT_ATTR_COLOUR    (1 << 1)
#define TEXT_ATTR_HIGHLIGHT (1 << 2)
    int attribute;
    int index;
};

#define MAX_MES 128
struct dbgcmd_medata {
    int index;
    char *meid_str;
    int meid;
    char *listfile;

    /* list file line state */
    struct text_line *listdata;
    int linecount;
    int cur_line;

    /* me status */
    int me_stopped;
    int me_cur_pc;
    int me_cur_ctx;

    /* brkpoint pc */
    int brk_pc;
};

struct dbgcmd_data {
    /* cmd line opts */
    int opt_nfp;
    char *opt_elffile;

    struct dbgcmd_medata medata[MAX_MES];
    int mecnt;

    int cur_me;

    /* executable name (argv[0]) */
    const char *progname;

    char *version;

    /* hwdbg handle */
    struct nfp_hwdbg *hwdbg;

    int quit;

    /* list of active code breaks */
    struct break_list_entry *breaks;
    /* ticker for allocating code break ids */
    int64_t breakid;
    /* list of pending events */
    struct event_list_entry *events;
    /* the event list requires exclusive access */
    pthread_mutex_t ev_mtx;

    /* search state, common for all MEs */
    int searchdir;
    char searchstr[1024];

    /* dont redraw gui in the next gui loop */
    int norefresh;
    /* force a character to be read in place of next getch */
    int forcechar;
    /* first gui loop */
    int first;
};

int dbgcmd_get_version(struct dbgcmd_data *dd, char *buf, int len);
int dbgcmd_break_exists(struct dbgcmd_data *dd, int pc, int ctx);
int dbgcmd_break_add(struct dbgcmd_data *dd, int pc, int ctx);
int dbgcmd_break_del(struct dbgcmd_data *dd, int pc, int ctx);
int dbgcmd_break_clear(struct dbgcmd_data *dd, int breakid);
int dbgcmd_break_clearall(struct dbgcmd_data *dd);
void dbgcmd_update_me_status(struct dbgcmd_data *dd, int ctx);
int dbgcmd_srvreset(struct dbgcmd_data *dd, int type);

int dbgcmd_step(struct dbgcmd_data *dd, int ctx);
int dbgcmd_run(struct dbgcmd_data *dd, int me);
int dbgcmd_stop(struct dbgcmd_data *dd, int me);

int gui_init(struct dbgcmd_data *dbgdat);
void gui_free(struct dbgcmd_data *dbgdat);
int gui_loop(struct dbgcmd_data *dbgdat);

int listfile_parse(struct dbgcmd_medata *md);

int uof_parse(const char *elffile, struct uof_dbg_entry **dbgdat);

#endif /* __DBGCMD_H__ */
