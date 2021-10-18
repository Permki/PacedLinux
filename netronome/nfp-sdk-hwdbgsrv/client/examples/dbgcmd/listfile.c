/*
 * Copyright (C) 2014, Netronome Systems, Inc.  All rights reserved.
 */
#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "dbgcmd.h"

int listfile_parse(struct dbgcmd_medata *md)
{
    struct text_line *tlarr = NULL;
    FILE *fd;
    char buf[1024];
    char *s;
    char *lst;
    int pad;
    int pc = -1;
    int i, n = 0;

    /* if no list file, make null entry */
    if (!md->listfile || strlen(md->listfile) == 0) {
        tlarr = realloc(tlarr, sizeof(struct text_line) * (n + 1));
        if (!tlarr) {
            fprintf(stderr, "out of memory while parsing list file %s\n",
                    md->listfile);
            return -1;
        }
        tlarr[n].data = NULL; /* end of array indicator */
        md->listdata = tlarr;
        md->linecount = n;
        return 0;
    }

    fd = fopen(md->listfile, "r");
    if (!fd) {
        fprintf(stderr, "could not open list file %s (%s)\n",
                md->listfile, strerror(errno));
        return -1;
    }
    while (1) {
        s = fgets(buf, 1024, fd);
        if (!s) {
            fprintf(stderr, "failed to read list file %s (%s)\n",
                    md->listfile, strerror(errno));
            fclose(fd);
            return -1;
        }

        if (strlen(s) == 0)
            break;

        /* kill the new line */
        if (s[strlen(s) - 1] == '\n')
            s[strlen(s) - 1] = '\0';

        /* code start with .number xxxx */
        if (s[0] == '.' && isdigit(s[1])) {
            lst = strchr(s + 1, ' ');
            if (!lst) {
                fprintf(stderr, "failed to parse list file %s\n",
                        md->listfile);
                fclose(fd);
                return -1;
            }
            pad = lst - s;
            *lst = '\0';
            pc = atoi(s + 1);
            *lst = ' ';
        } else {
            pad = 0;
            for (i = 0; s[i] != '\0'; i++) {
                if (s[i] != ' ')
                    break;
                pad++;
            }
        }
        if (pc < 0)
            continue;

        if (strstr(s, "+ucode_end"))
            break;
        tlarr = realloc(tlarr, sizeof(struct text_line) * (n + 1));
        if (!tlarr) {
            fprintf(stderr, "out of memory while parsing list file %s\n",
                    md->listfile);
            fclose(fd);
            return -1;
        }
        tlarr[n].data = strdup(s + pad);
        if (!tlarr[n].data) {
            fprintf(stderr, "out of memory while parsing list file %s\n",
                    md->listfile);
            fclose(fd);
            return -1;
        }
        tlarr[n].index = pc;
        n++;
    }
    tlarr = realloc(tlarr, sizeof(struct text_line) * (n + 1));
    if (!tlarr) {
        fprintf(stderr, "out of memory while parsing list file %s\n",
                md->listfile);
        fclose(fd);
        return -1;
    }
    tlarr[n].data = NULL; /* end of array indicator */
    fclose(fd);
    md->listdata = tlarr;
    md->linecount = n;
    return 0;
}
