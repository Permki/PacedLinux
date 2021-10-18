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

#include <nfp_hwdbg.h>
#include <nfp-common/nfp_resid.h>

#include "dbgcmd.h"

static void print_mestatus_bar(int menum,
                               const char *mestr, const char *liststr,
                               int stopped, int ctx, int pc)
{
    char statstr[64];
    if (stopped)
        snprintf(statstr, 64, "pc %d   ctx %d   |   status: stopped", pc, ctx);
    else
        snprintf(statstr, 64, "status: running");

    move(0, 0);
    clrtoeol();
    mvprintw(0, 0, "%3d: Debugging ME %s : %s",
             menum, mestr, liststr ? liststr : "no list file");
    mvprintw(0, COLS - strlen(statstr), "%s", statstr);
    mvchgat(0, 0, -1, A_STANDOUT, 0, NULL);
}

static int calc_lines(const char *msg, int width)
{
    const char *w = msg;
    int n, o = 0;
    int cnt = 1;
    int done = 0;

    while (!done) {
        /* find the end of the word */
        n = 0;
        while (1) {
            if (w[n] == '\0' || w[n] == ' ' || w[n] == '\n' || w[n] == '\r')
                break;
            n++;
        }

        if (w[n] == '\0') {
            if (n == 0)
                break; /* nothing to do */
            done = 1;
        }

        if (n + o == width || n + o == width - 1) {
            /* just fits - dont account for white space */
            w += n;
            o = 0;
            cnt++;
            continue;
        }

        if (n + o > width) {
            /* doesn't fit */
            if (n > width) {
                /* this word wont fit in the line */
                w += width - o;
            } /* otherwise we start a new line and place that word there */
            o = 0;
            cnt ++;
            done = 0; /* override the done */
            continue;
        }

        /* fits */
        if (w[n] == ' ') {
            o += n + 1;
        } else if (w[n] == '\r' || w[n] == '\n' || w[n] == '\0') {
            o = 0;
            cnt++;
        }

        w += n + 1;
    }

    /* if there is nothing after a newline, report one less */
    if (o == 0 && cnt > 1)
        return cnt - 1;

    return cnt;
}

static WINDOW *create_msg_win(int height, int width, int starty, int startx)
{
    WINDOW *win;
    win = newwin(height, width, starty, startx);
    box(win, 0 , 0);
    /* no border on the window */
    wborder(win, ' ', ' ', ' ', ' ', '-', '-', ' ', ' ');
    wrefresh(win);
    return win;
}

static void destroy_win(WINDOW *win)
{
    wborder(win, ' ', ' ', ' ',' ',' ',' ',' ',' ');
    wclear(win);
    wrefresh(win);
    delwin(win);
}

static void print_status_msg(struct dbgcmd_data *dd, const char *msg, int error)
{
    WINDOW *win;
    int width = COLS;
    int height;

    height = calc_lines(msg, width) + 1;

    win = create_msg_win(height, width, LINES - height, 0);
    if (error)
        wattron(win, A_BOLD);
    mvwprintw(win, 1, 0, "%s", msg);
    wrefresh(win);
    if (error)
        wattroff(win, A_BOLD);
    /* wait for input */
    timeout(-1);
    dd->forcechar = getch();
    timeout(0);
    destroy_win(win);
}

static WINDOW *create_cmd_win(int height, int width, int starty, int startx)
{
    WINDOW *win;
    win = newwin(height, width, starty, startx);
    box(win, 0 , 0);
    /* no border on the window */
    wborder(win, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
    wrefresh(win);
    return win;
}

static void get_cmd_str(char cmdchar, char *out_str, int len)
{
    WINDOW *win;
    int n = 0;
    int ch;
    int done = 0;
    int off;
    int width = COLS;

    out_str[0] = '\0';
    win = create_cmd_win(1, width, LINES - 1, 0);
    wprintw(win, "%c", cmdchar);
    curs_set(1);
    wrefresh(win);
    timeout(-1);
    while (1) {
        ch = getch();
#if 0
        if (ch == 27) { /* escape */
            ch = getch();
            if (ch == ERR)
                ch = 27;
        }
#endif
        switch (ch) {
        case 27:
            out_str[0] = '\0';
            done = 1;
            break;
        case KEY_BACKSPACE:
        case '\b':
            if (strlen(out_str) == 0) {
                done = 1;
                break;
            }

            mvwprintw(win, 0, 1 + strlen(out_str) - 1, " ");
            out_str[--n] = '\0';
            break;
        case '\n':
        case '\r':
            out_str[n] = '\0';
            done = 1;
            break;
        default:
            if (!isprint(ch)) /* ignore non-printables */
                break;
            if (n >= len - 1)
                n--; /* overwrite last char */
            out_str[n++] = ch;
            out_str[n] = '\0';
        }
        if (done)
            break;

        if (strlen(out_str) < width - 1)
            off = 0;
        else
            off = strlen(out_str) - width + 1;

        mvwprintw(win, 0, 1, "%s", out_str + off);
        wrefresh(win);
    }
    timeout(0);
    curs_set(0);
    destroy_win(win);
}

static WINDOW *create_event_win(int height, int width, int starty, int startx)
{
    WINDOW *win;
    win = newwin(height, width, starty, startx);
    box(win, 0 , 0);
    /* no border on the window */
    wborder(win, '*', '*', '*', '*', '/', '\\', '\\', '/');
    wrefresh(win);
    return win;
}

static void print_event_msg(const char *type, const char *info)
{
    WINDOW *win;

    win = create_event_win(9, 40, LINES / 2 - 9 / 2, COLS / 2 - 40 / 2);

    wattron(win, A_BOLD);
    mvwprintw(win, 1, 40/2 - 6, "Debug event");
    mvwprintw(win, 3, 3, "%s", type);
    mvwprintw(win, 5, 3, "%s", info);
    wrefresh(win);
    wattroff(win, A_BOLD);

    /* wait for input */
    timeout(-1);
    getch();
    timeout(0);
    destroy_win(win);
}

/* same as above but also prints yn text and returns 1 if y or Y was pushed */
static int print_event_msg_yn(const char *type,
                              const char *info,
                              const char *ynprompt)
{
    WINDOW *win;
    int ret = 0;
    int c;

    win = create_event_win(11, 40, LINES / 2 - 11 / 2, COLS / 2 - 40 / 2);

    wattron(win, A_BOLD);
    mvwprintw(win, 1, 40/2 - 6, "Debug event");
    mvwprintw(win, 3, 3, "%s", type);
    mvwprintw(win, 5, 3, "%s", info);
    mvwprintw(win, 7, 3, "%s", ynprompt);
    wrefresh(win);
    wattroff(win, A_BOLD);

    /* wait for input */
    timeout(-1);
    c = getch();
    if (c == 'y' || c == 'Y')
        ret = 1;

    timeout(0);
    destroy_win(win);
    return ret;
}

static void text_jump_to_index(struct dbgcmd_medata *md, int index)
{
    int i;
    int last_index = -1, last_line = -1;
    int first_index = -1;

    /* if this was called with a running ME, index will be zero
     * just ignore this case
     */
    if (index < 0)
        return;

    for (i = 0; md->listdata[i].data; i++) {
        if (i == 0)
            first_index = md->listdata[i].index;

        if (md->listdata[i].index == index) {
            md->cur_line = i;
            return;
        } else {
            if (last_index != md->listdata[i].index)
                last_line = i;
            last_index = md->listdata[i].index;
        }
    }

    if (last_index >= 0 && last_index < index) {
        md->cur_line = last_line;
        return;
    }

    if (first_index >= 0 && first_index > index) {
        md->cur_line = first_index;
        return;
    }
}

static int text_index_shows(struct text_line *text, int off, int index)
{
    int first_line = -1;
    int last_line = -1;
    int i;

    for (i = 0; text[i].data; i++) {
        if (text[i].index == index) {
            if (first_line < 0)
                first_line = i;
            last_line = i;
        }
    }

    /* don't bother trying to jump to the pc if it isn't represented */
    if (first_line < 0 || last_line < 0)
        return 1;

    /* check that off makes sense */
    for (i = 0; i <= off; i++) {
        if (text[i].data)
            continue;

        if (i == 0) /* no code to put in */
            return 1;

        off = i - 1;
        break;
    }

    if (off > first_line || off + LINES - 2 < last_line)
        return 0; /* it wont show */

    return 1;
}

static void draw_text(struct text_line *text, int off, int show_cursor)
{
    int i;
    int prev = -1;

    for (i = 0; i < LINES - 1; i++) {
        move(1 + i, 0);
        clrtoeol();
    }

    /* check that off makes sense */
    for (i = 0; i <= off; i++) {
        if (text[i].data)
            continue;

        if (i == 0) /* no code to put in */
            return;

        off = i - 1;
        break;
    }

    for (i = 0; text[i + off].data; i++) {
        if (i >= LINES - 1)
            break;

        if (prev != text[i + off].index) {
            init_pair(2, COLOR_YELLOW, COLOR_BLACK);
            attron(COLOR_PAIR(2));
            mvprintw(1 + i, 0, "%5d", text[i + off].index);
            attroff(COLOR_PAIR(2));
            prev = text[i + off].index;
        }

        mvprintw(1 + i, 6, "%s", text[i + off].data);
        if (text[i + off].attribute) {
            int use_colour = text[i + off].attribute & TEXT_ATTR_COLOUR ? 1 : 0;
            int attr = 0;
            init_pair(1, COLOR_GREEN, COLOR_BLACK);

            if (text[i + off].attribute & TEXT_ATTR_BOLD)
                attr |= A_BOLD;
            if (text[i + off].attribute & TEXT_ATTR_HIGHLIGHT)
                attr |= A_BLINK;
            mvchgat(1 + i, 6, -1, attr, use_colour, NULL);
        }
    }
    move(1, 0);
    if (show_cursor)
        curs_set(1);
    else
        curs_set(0);
}

static void print_help(struct dbgcmd_data *dd)
{
    const char *s;
    s = ":h, :help, :?        - help\n"
        ":q, :quit            - quit\n"
        ":v, :version         - print client and server version\n"
        ":e ctx               - step a context\n"
        ":s me                - stop an ME (-1 for all loaded MEs)\n"
        ":r me                - run an ME (-1 for all loaded MEs)\n"
        ":# [meno,meid,mestr] - debug another ME\n"
        ":[pcno]              - jump to a PC location\n"
        "/[str]               - search down for string\n"
        "?[str]               - search up for string\n"
        "k                    - move up a line\n"
        "j                    - move down a line\n"
        "<HOME>               - move to first line\n"
        "<END>                - move to last line\n"
        "<PGUP>               - move up a page\n"
        "<PGDOWN>             - move down a page\n"
        "b, <SPACE>           - toggle code breakpoint\n"
        "r                    - run ME\n"
        "s                    - stop ME\n"
        "e                    - step ME\n"
        "C                    - clear all outstanding breaks\n"
        "L                    - prime the server for firmware loading\n"
        "R                    - reset debug server data (removes code breaks)\n"
        "S                    - reset debug server data (keeps code breaks)\n";
    print_status_msg(dd, s, 0);
}

static void assign_line_attibutes(struct dbgcmd_data *dd,
                                  struct dbgcmd_medata *md)
{
    struct break_list_entry *brent;
    int i;

    for (i = 0; md->listdata[i].data; i++) {
        md->listdata[i].attribute = 0;

        /* this isn't very efficient, but that doesn't really matter */
        for (brent = dd->breaks; brent; brent = brent->next) {
            if (md->listdata[i].index != brent->pc)
                continue;

            if (md->meid != brent->meid)
                continue;

            md->listdata[i].attribute |= TEXT_ATTR_COLOUR;
        }

        if (md->me_stopped && md->me_cur_pc == md->listdata[i].index) {
            md->listdata[i].attribute |= TEXT_ATTR_BOLD;
        }

        if (md->brk_pc == md->listdata[i].index) {
            md->listdata[i].attribute |= TEXT_ATTR_HIGHLIGHT;
        }
    }
}

static int mestr2id(const char *s)
{
    if (!s)
        return 0;

    if (isdigit(s[0]))
        return strtol(s, NULL, 0);

    return nfp6000_idstr2meid(s, NULL);
}


static void handle_cmd(struct dbgcmd_data *dd, const char *cmdstr)
{
    struct dbgcmd_medata *md = &dd->medata[dd->cur_me];
    char buf[1024];
    char *arg;
    int isnum = 1;
    int found;
    int i;
    int err = 0;
    int ret;

    /* make arg point to the next arg, make sure cmdstr is nulled */
    arg = strchr(cmdstr, ' ');
    if (arg) {
        *arg = '\0';
        arg++;
        while (1) {
            if ((*arg) == '\0') {
                arg = NULL;
                break;
            }
            if ((*arg) != ' ')
                break;
            arg++;
        }
    }

    if (!strcmp(cmdstr, "q") ||
           !strcmp(cmdstr, "quit")) {
        dd->quit = 1;
        return;
    }

    if (!strcmp(cmdstr, "v") ||
           !strcmp(cmdstr, "version")) {
        i = sprintf(buf, "client version: %s\n", dd->version);
        i += sprintf(buf + i, "server version: ");
        if (dbgcmd_get_version(dd, buf + i, sizeof(buf) - i) < 0) {
            sprintf(buf + i,
                    "could not retrieve server version (%s)",
                    strerror(errno));
            err = 1;
        }

        print_status_msg(dd, buf, err);
        return;
    }

    if (!strcmp(cmdstr, "#") || !strcmp(cmdstr, "me")) {
        if (!arg) {
            /* cycle through the MEs */
            dd->cur_me = (dd->cur_me + 1) % dd->mecnt;
        } else if (!isdigit(arg[0])) {
            /* meid string */
            found = 0;
            for (i = 0; i < dd->mecnt; i++) {
                if (strcmp(arg, dd->medata[i].meid_str))
                    continue;
                found = 1;
                break;
            }
            if (!found) {
                print_status_msg(dd, "ME not found", 1);
                return;
            }
            dd->cur_me = i;
        } else if (strlen(arg) >= 2 && arg[1] == 'x') { /* hex MEID */
            int meid = strtol(arg, NULL, 0);

            found = 0;
            for (i = 0; i < dd->mecnt; i++) {
                if (meid != dd->medata[i].meid)
                    continue;
                found = 1;
                break;
            }
            if (!found) {
                print_status_msg(dd, "ME not found", 1);
                return;
            }
            dd->cur_me = i;
        } else {
            int menum = strtol(arg, NULL, 0);
            if (menum >= dd->mecnt || menum < 0) {
                print_status_msg(dd, "ME not found", 1);
                return;
            }
            dd->cur_me = menum;
        }
        /* force redraw */
        dd->norefresh = 0;
        dbgcmd_update_me_status(dd, -1);
        return;
    }

    if (!strcmp(cmdstr, "r") || !strcmp(cmdstr, "run")) {
        ret = dbgcmd_run(dd, mestr2id(arg));
        if (ret < 0) {
            snprintf(buf, sizeof(buf),
                     "failed to stop me (%s)", strerror(errno));
            print_status_msg(dd, buf, 1);
            return;
        }
        if (!text_index_shows(md->listdata,
                              md->cur_line, md->me_cur_pc)) {
            text_jump_to_index(md, md->me_cur_pc);
        }
        return;
    }

    if (!strcmp(cmdstr, "s") || !strcmp(cmdstr, "stop")) {
        ret = dbgcmd_stop(dd, mestr2id(arg));
        if (ret < 0) {
            snprintf(buf, sizeof(buf),
                     "failed to stop me (%s)", strerror(errno));
            print_status_msg(dd, buf, 1);
            return;
        }
        if (!text_index_shows(md->listdata,
                              md->cur_line, md->me_cur_pc)) {
            text_jump_to_index(md, md->me_cur_pc);
        }
        return;
    }

    if (!strcmp(cmdstr, "c") || !strcmp(cmdstr, "ctx")) {
        dbgcmd_update_me_status(dd, !arg ? -1 : atoi(arg));

        if (!text_index_shows(md->listdata,
                              md->cur_line, md->me_cur_pc)) {
            text_jump_to_index(md, md->me_cur_pc);
        }
        return;
    }

    if (!strcmp(cmdstr, "e") || !strcmp(cmdstr, "step")) {
        /* XXX: duplicate code here */
        if (!md->me_stopped) {
            print_status_msg(dd,
                             "cannot step while ME is running",
                             1);
            return;
        }
        ret = dbgcmd_step(dd, !arg ? -1 : atoi(arg));
        if (ret < 0) {
            if (errno == EBUSY) /* make this error more clear */
                snprintf(buf, sizeof(buf),
                         "failed to step me, ME has uncleared breaks");
            else
                snprintf(buf, sizeof(buf),
                         "failed to step me (%s)", strerror(errno));
            print_status_msg(dd, buf, 1);
            return;
        }
        if (!text_index_shows(md->listdata,
                              md->cur_line, md->me_cur_pc)) {
            text_jump_to_index(md, md->me_cur_pc);
        }
        return;
    }

    if (!strcmp(cmdstr, "q") ||
           !strcmp(cmdstr, "quit")) {
        dd->quit = 1;
        return;
    }

    if (!strcmp(cmdstr, "h") ||
           !strcmp(cmdstr, "?") ||
           !strcmp(cmdstr, "help")) {
        print_help(dd);
        return;
    }

    /* check if the string is a number */
    for (i = 0; cmdstr[i] != '\0'; i++) {
        if (!isdigit(cmdstr[i])) {
            isnum = 0;
            break;
        }
    }

    if (isnum) {
        /* jump to that PC */
        isnum = atoi(cmdstr);
        for (i = 0; i < md->linecount; i++) {
            if (md->listdata[i].index == isnum) {
                md->cur_line = i;
                return;
            }
        }
        snprintf(buf, sizeof(buf), "could not find entry for pc %d", isnum);
        print_status_msg(dd, buf, 1);
        return;
    }

    snprintf(buf, sizeof(buf),
             "bad command: %s\n"
             "press :h for help",
             cmdstr);
    print_status_msg(dd, buf, 1);
}

static void search_jump(struct dbgcmd_data *dd)
{
    struct dbgcmd_medata *md = &dd->medata[dd->cur_me];
    int startline = md->cur_line;
    int n;

    if (strlen(dd->searchstr) == 0)
        return;

    /* if no list file is loaded */
    if (md->linecount == 0)
        return;

    n = startline;
    while (1) {
        if (dd->searchdir)
            n++;
        else
            n--;

        if (n < 0)
            n = md->linecount - 1;
        if (n >= md->linecount)
            n = 0;
        if (strstr(md->listdata[n].data, dd->searchstr)) {
            md->cur_line = n;
            return;
        }

        if (n == startline) {
            print_status_msg(dd, "could not find search string", 1);
            return;
        }
    }
}

static struct dbgcmd_medata *
find_medata(struct dbgcmd_data *dd, int meid)
{
    int i;

    for (i = 0; i < dd->mecnt; i++) {
        if (dd->medata[i].meid != meid)
            continue;
        return &dd->medata[i];
    }
    return NULL;
}

static void handle_events(struct dbgcmd_data *dd)
{
    struct dbgcmd_medata *cur_md = &dd->medata[dd->cur_me];
    struct dbgcmd_medata *ev_md = NULL;
    struct event_list_entry *e, *n;
    char buf[1024];
    int activity = 0;
    int clear_break;
    int i;

    /* mark all the brk pc's as invalid */
    for (i = 0; i < dd->mecnt; i++)
        dd->medata[i].brk_pc = -1;

    pthread_mutex_lock(&dd->ev_mtx);

    for (e = dd->events; e; e = n) {
        activity = 1;
        clear_break = 0;
        n = e->next;
        switch (e->type) {
        case EVENT_TYPE_RESET:
            print_event_msg("Debug server data reset",
                            "press any key to continue");
            break;
        case EVENT_TYPE_SHUTDOWN:
            print_event_msg("Debug server shut down", "press any key to exit");
            dd->quit = 1;
            break;
        case EVENT_TYPE_CODEBP:
            ev_md = find_medata(dd, e->meid);
            snprintf(buf, sizeof(buf),
                     "ME %s : Context %d : PC %d",
                     ev_md ? ev_md->meid_str : "UNKNOWN", e->ctx, e->pc);
            if (ev_md == cur_md) {
                print_event_msg("Code breakpoint", buf);
            } else {
                if (print_event_msg_yn("Code breakpoint",
                                       buf,
                                       "Press Y to jump to ME")) {
                    cur_md = ev_md;
                    dd->cur_me = cur_md->index;
                }
            }
            clear_break = 1;
            break;
        case EVENT_TYPE_SWBP:
            /* for SWBPT we want to highlight the bpt that hit as the
             * PC points to the instruction after
             */
            ev_md = find_medata(dd, e->meid);
            if (ev_md)
                ev_md->brk_pc = e->pc;
            snprintf(buf, sizeof(buf),
                     "ME %s: Context %d : PC %d",
                     ev_md ? ev_md->meid_str : "UNKNOWN", e->ctx, e->pc);
            if (ev_md == cur_md) {
                print_event_msg("Software breakpoint", buf);
            } else {
                if (print_event_msg_yn("Software breakpoint",
                                       buf,
                                       "Press Y to jump to ME")) {
                    cur_md = ev_md;
                    dd->cur_me = cur_md->index;
                }
            }
            clear_break = 1;
            break;
        case EVENT_TYPE_MESTOP:
            print_event_msg("MicroEngine group stop", "");
            break;
        }
        if (clear_break) {
            if (dbgcmd_break_clear(dd, e->breakid) < 0) {
                snprintf(buf, sizeof(buf),
                         "failed to clear break (%s)", strerror(errno));
                print_status_msg(dd, buf, 1);
            }
        }
        free(e);
    }
    dd->events = NULL;
    pthread_mutex_unlock(&dd->ev_mtx);

    if (activity) {
        dbgcmd_update_me_status(dd, -1);
        if (cur_md->brk_pc >= 0) {
            /* SW BPT detected so jump to where the bpt hit not where the next
             * instruction to be executed is
             */
            text_jump_to_index(cur_md, cur_md->brk_pc);
        } else {
            text_jump_to_index(cur_md, cur_md->me_cur_pc);
        }
        dd->norefresh = 0; /* force redraw */
    }
}

int gui_loop(struct dbgcmd_data *dbgdat)
{
    struct dbgcmd_medata *md;
    char buf[1024];
    int ch;
    int ret = 0;
    int pc;

    md = &dbgdat->medata[dbgdat->cur_me];

    if (dbgdat->first) {
        text_jump_to_index(md, md->me_cur_pc);
        dbgdat->first = 0;
    }

    handle_events(dbgdat);

    /* current ME can change in handle_events */
    md = &dbgdat->medata[dbgdat->cur_me];

    /* make the list text colourful/bold if needed */
    assign_line_attibutes(dbgdat, md);

    if (!dbgdat->norefresh) {
        print_mestatus_bar(md->index,
                           md->meid_str, md->listfile,
                           md->me_stopped,
                           md->me_cur_ctx,
                           md->me_cur_pc);
        draw_text(md->listdata, md->cur_line, 1);
        refresh();
    }
    dbgdat->norefresh = 0;

    /* we may take a previously entered/injected char */
    if (dbgdat->forcechar >= 0) {
        ch = dbgdat->forcechar;
        dbgdat->forcechar = -1;
    } else
        ch = getch();

    if (ch == 27) { /* escape */
        ch = getch();
        if (ch == ERR)
            ch = 27;
    }

    switch (ch) {
    case 3 : /* ctrl-c */
        dbgdat->quit = 1;
        break;
    case ':' :
        get_cmd_str(':', buf, sizeof(buf));
        handle_cmd(dbgdat, buf);
        break;
    case '/' :
    case '?' :
        get_cmd_str(ch, buf, sizeof(buf));
        if (ch == '/')
            dbgdat->searchdir = 1;
        else
            dbgdat->searchdir = 0;
        if (strlen(buf) > 0)
            strcpy(dbgdat->searchstr, buf);
    case 'n':
        search_jump(dbgdat);
        break;
    case 339: /* PGUP */
        md->cur_line = MAX(0, md->cur_line - (LINES - 1));
        break;
    case 338: /* PGDOWN */
        md->cur_line = MIN(md->linecount - 1,
                           md->cur_line + (LINES - 1));
        break;
    case 262: /* HOME */
        md->cur_line = 0;
        break;
    case 360: /* END */
        md->cur_line = md->linecount - 1;
        break;
    case 259 : /* UP */
    case 'k' :
        md->cur_line = MAX(0, md->cur_line - 1);
        break;
    case 258 : /* DOWN */
    case 'j' :
        md->cur_line = MIN(md->linecount - 1, md->cur_line + 1);
        break;
    case '#':
        dbgdat->cur_me = (dbgdat->cur_me + 1) % dbgdat->mecnt;
        dbgdat->norefresh = 0;
        dbgcmd_update_me_status(dbgdat, -1);
        break;
    case 'b' :
    case ' ' :
        if (!md->me_stopped) {
            print_status_msg(dbgdat,
                             "cannot add or remove breaks while ME is running",
                             1);
            break;
        }

        /* cant add breaks if we dont have code */
        if (md->linecount == 0)
            break;

        pc = md->listdata[md->cur_line].index;
        if (dbgcmd_break_exists(dbgdat, pc, -1)) {
            if (dbgcmd_break_del(dbgdat, pc, -1) < 0) {
                snprintf(buf, sizeof(buf),
                         "failed to delete break (%s)", strerror(errno));
                print_status_msg(dbgdat, buf, 1);
            }
        } else {
            if (dbgcmd_break_add(dbgdat, pc, -1) < 0) {
                snprintf(buf, sizeof(buf),
                         "failed to add break (%s)", strerror(errno));
                print_status_msg(dbgdat, buf, 1);
            }
        }
        break;
    case 'r':
        ret = dbgcmd_run(dbgdat, 0);
        if (ret < 0) {
            if (errno == EBUSY) /* make this error more clear */
                snprintf(buf, sizeof(buf),
                         "failed to run me, ME has uncleared breaks");
            else
                snprintf(buf, sizeof(buf),
                         "failed to run me (%s)", strerror(errno));
            print_status_msg(dbgdat, buf, 1);
            break;
        }
        break;
    case 's':
        ret = dbgcmd_stop(dbgdat, 0);
        if (ret < 0) {
            snprintf(buf, sizeof(buf),
                     "failed to stop me (%s)", strerror(errno));
            print_status_msg(dbgdat, buf, 1);
            break;
        } else {
            text_jump_to_index(md, md->me_cur_pc);
        }
        break;
    case 'e':
        if (!md->me_stopped) {
            print_status_msg(dbgdat,
                             "cannot step while ME is running",
                             1);
            break;
        }
        print_mestatus_bar(md->index,
                           md->meid_str, md->listfile,
                           0, -1, -1);
        refresh();
        ret = dbgcmd_step(dbgdat, md->me_cur_ctx);
        if (ret < 0) {
            if (errno == EBUSY) /* make this error more clear */
                snprintf(buf, sizeof(buf),
                         "failed to step me, ME has uncleared breaks");
            else
                snprintf(buf, sizeof(buf),
                         "failed to step me (%s)", strerror(errno));
            print_status_msg(dbgdat, buf, 1);
            break;
        }
        if (!text_index_shows(md->listdata,
                              md->cur_line, md->me_cur_pc)) {
            text_jump_to_index(md, md->me_cur_pc);
        }
        break;
    case 'C':
        ret = dbgcmd_break_clearall(dbgdat);
        if (ret < 0) {
            snprintf(buf, sizeof(buf),
                     "failed to clear outstanding bugs (%s)", strerror(errno));
            print_status_msg(dbgdat, buf, 1);
        }
        break;
    case 'L':
        ret = dbgcmd_srvreset(dbgdat, NFP_HWDBG_SRV_RESET_TYPE_LOADER);
        if (ret < 0) {
            snprintf(buf, sizeof(buf),
                     "failed to reset debug server data (%s)", strerror(errno));
            print_status_msg(dbgdat, buf, 1);
        }
        text_jump_to_index(md, md->me_cur_pc);
        break;
    case 'S':
        ret = dbgcmd_srvreset(dbgdat, NFP_HWDBG_SRV_RESET_TYPE_SOFTRESET);
        if (ret < 0) {
            snprintf(buf, sizeof(buf),
                     "failed to reset debug server data (%s)", strerror(errno));
            print_status_msg(dbgdat, buf, 1);
        }
        text_jump_to_index(md, md->me_cur_pc);
        break;
    case 'R':
        ret = dbgcmd_srvreset(dbgdat, NFP_HWDBG_SRV_RESET_TYPE_HARDRESET);
        if (ret < 0) {
            snprintf(buf, sizeof(buf),
                     "failed to reset debug server data (%s)", strerror(errno));
            print_status_msg(dbgdat, buf, 1);
        }
        text_jump_to_index(md, md->me_cur_pc);
        break;
    case '\r' :
    case '\n' :
        text_jump_to_index(md, md->me_cur_pc);
        break;
    case 27:
    case KEY_BACKSPACE:
    case '\b':
        break;
    case ERR: /* timeout */
        ret = 1;
        dbgdat->norefresh = 1;
        break;
    default:
        snprintf(buf, sizeof(buf), "unrecognized shorcut %c(%d)\n"
                                   "type :h for help", ch, ch);
        print_status_msg(dbgdat, buf, 1);
    }
    return ret;
}

int gui_init(struct dbgcmd_data *dbgdat)
{
    initscr();          /* Start curses mode        */
    timeout(0); /* make non-blocking */

    if(has_colors() == FALSE) {
        endwin();
        printf("Your terminal does not support color\n");
        return -1;
    } else
        start_color();      /* Colour mode */

    curs_set(0);
    raw();              /* Line buffering disabled  */
    keypad(stdscr, TRUE);       /* We get F1, F2 etc..      */
    noecho();           /* Don't echo() while we do getch */
    return 0;
}

void gui_free(struct dbgcmd_data *dbgdat)
{
    endwin();           /* End curses mode */
}
