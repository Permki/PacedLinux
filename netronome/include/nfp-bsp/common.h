/**
 * Copyright (C) 2008-2014 Netronome Systems, Inc.  All rights reserved.
 *
 * @file common.h
 *
 */

#ifndef _COMMON_H_
#define _COMMON_H_

/******************************************************************************/

#include <stdint.h>

typedef unsigned int      u32;
typedef unsigned short    u16;
typedef unsigned char     u8;
typedef   signed int      s32;
typedef   signed short    s16;
typedef   signed char     s8;

#define DBG(fmt, a...)    printf("%s:%d %s():  " fmt "\n",        \
                               __FILE__, __LINE__, __func__, ## a)

#define DIM(x)            (sizeof(x)/sizeof(x[0]))

#ifndef MIN
 #define MIN(x, y)        ((x < y) ? x : y)
#endif
#ifndef MAX
 #define MAX(x, y)        ((x > y) ? x : y)
#endif

#define KB(x)                   (x << 10)
#define MB(x)                   (KB(x) * KB(1))
#define GB(x)                   (KB(x) * MB(1))

char * _basename      (const char *path);
char* newstr          (const char *format, ...);
int build_string_list (int count, char ***list,
                        char * (*get_string_func)(void *, int), void *context);
int  fsize            (char *fname, unsigned int *sz);
int  term_width       (void);
void term_buffering   (void);
void no_term_buffering(void);
char _getc            (void);
void _pause           (void);
void check_for_abort  (void);
void sleep_ms         (int ms);
u32  tickcount        (void);
int  getrand          (int min, int max);
u32  bin_rand         (void);

#define RESET_CLR         "0"
#define GREY              "30"
#define RED               "31"
#define BROWN             "32"
#define YELLOW            "33"
#define BLUE              "34"
#define PURPLE            "35"
#define CYAN              "36"
#define WHITE             "37"
#define GREEN             "38"
#define BOLD              "1"
#define RESET_BOLD        "21"
#define BOLD_TXT()         textattr(BOLD)
#define RESET_BOLD_TXT()   textattr(RESET_BOLD)
#define RESET_TXT_CLR()    textattr(RESET_CLR)
#define GREEN_TXT()        textattr(GREEN)
#define GREY_TXT()         textattr(GREY)
#define RED_TXT()          textattr(RED)
#define YELLOW_TXT()       textattr(YELLOW)
#define WHITE_TXT()        textattr(WHITE)
#define BLUE_TXT()         textattr(BLUE)
#define CYAN_TXT()         textattr(CYAN)
void textattr(char *);


#define offset_of(s, e)         ((intptr_t)&((s *)NULL)->e)
#define container_of(x, s, e)   ((void*)((intptr_t)(x) - offset_of(s, e)))

#define list_for_each(p, head) for (p = (head)->next; p != (head); p = p->next)
struct dl_list {
    struct dl_list *next;
    struct dl_list *prev;
};
void  list_init     (struct dl_list *list);
void  list_add_hd   (struct dl_list *head, struct dl_list *lst);
void  list_add_tl   (struct dl_list *head, struct dl_list *lst);
void  list_del      (struct dl_list *rem);
int   list_is_empty (struct dl_list *list);

/******************************************************************************/

#endif // _COMMON_H_

