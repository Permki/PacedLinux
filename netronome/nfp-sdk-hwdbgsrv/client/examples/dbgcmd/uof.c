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
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <gelf.h>

#include <nfp_hwdbg.h>

#include "uof_dbg.h"

#include "dbgcmd.h"

/* debug section name in elf */
#define DEBUGSECSTR ".udebug"
/* debug info for an ME */
#define DBG_IMAG_STR "DBG_IMAG"
/* string table */
#define DBG_STRT_STR "DBG_STRT"

#ifdef DEBUG
static void print_objhdr(uof_objHdr_T *hdr)
{
    printf("uof header\n");
    printf("cpuType          = %d\n", hdr->cpuType);
    printf("minCpuVer        = %d\n", hdr->minCpuVer);
    printf("maxCpuVer        = %d\n", hdr->maxCpuVer);
    printf("maxChunks        = %d\n", hdr->maxChunks);
    printf("numChunks        = %d\n", hdr->numChunks);
    printf("nn_chain_modes   = %d\n", hdr->nn_chain_modes);
    printf("uof_dbg_version  = %d\n", hdr->uof_dbg_version);
}

static void print_chunkhdr(dbg_chunkHdr_T *chunk)
{
    int i;
    printf("chunkid = ");
    for (i = 0; i < DBGUOF_OBJID_LEN; i++) {
        if (chunk->chunkId[i] == '\0')
            break;
        printf("%c", chunk->chunkId[i]);
    }
    printf("\n");
    printf("offset = %d\n", chunk->offset);
    printf("size = %d\n", chunk->size);
}

static void print_dbgimg(dbg_Image_T *dbgimg, uof_strTab_T *strt)
{
    printf("dbgimage\n");
    printf("lstFileName        = %s\n", get_strt_str(strt, dbgimg->lstFileName));
    printf("meid               = 0x%lx\n", (int64_t)dbgimg->meid);
    printf("ctxAssigned        = 0x%lx\n", (int64_t)dbgimg->ctxAssigned);
    printf("lstFileCreatedBy   = 0x%lx\n", (int64_t)dbgimg->lstFileCreatedBy);
    printf("reserved1          = 0x%lx\n", (int64_t)dbgimg->reserved1);
    printf("ctxMode            = 0x%lx\n", (int64_t)dbgimg->ctxMode);
    printf("endianMode         = 0x%lx\n", (int64_t)dbgimg->endianMode);
    printf("scopeTabOffset     = 0x%lx\n", (int64_t)dbgimg->scopeTabOffset);
    printf("regTabSize         = 0x%lx\n", (int64_t)dbgimg->regTabSize);
    printf("lblTabSize         = 0x%lx\n", (int64_t)dbgimg->lblTabSize);
    printf("srcTabSize         = 0x%lx\n", (int64_t)dbgimg->srcTabSize);
    printf("regTabOffset       = 0x%lx\n", (int64_t)dbgimg->regTabOffset);
    printf("lblTabOffset       = 0x%lx\n", (int64_t)dbgimg->lblTabOffset);
    printf("srcTabOffset       = 0x%lx\n", (int64_t)dbgimg->srcTabOffset);
    printf("typTabSize         = 0x%lx\n", (int64_t)dbgimg->typTabSize);
    printf("scopeTabSize       = 0x%lx\n", (int64_t)dbgimg->scopeTabSize);
    printf("typTabOffset       = 0x%lx\n", (int64_t)dbgimg->typTabOffset);
    printf("instOprndTabSize   = 0x%lx\n", (int64_t)dbgimg->instOprndTabSize);
    printf("instOprndTabOffset = 0x%lx\n", (int64_t)dbgimg->instOprndTabOffset);
    printf("reserved2          = 0x%lx\n", (int64_t)dbgimg->reserved2);
}

#endif /* DEBUG */

typedef struct uof_strTab_S{
    unsigned int tableLen;               /* length of table */
    /* strings follow */
}uof_strTab_T;

#define CHUNKOFF(hdr, ch) (((char *)(hdr)) + chunk->offset)

static int get_strt_cnt(uof_strTab_T *strt)
{
    char *s = (((char *)strt) + sizeof(strt->tableLen));
    int l = strt->tableLen - 1;
    int n = 0;

    while (1) {
        /* look for next string */
        while (l) {
            s++;
            l--;
            if (*s == '\0') {
                n++;
                break;
            }
        }
        if (!l)
            break;
    }
    return n;
}

static char *get_strt_str(uof_strTab_T *strt, int ind)
{
    char *s = (((char *)strt) + sizeof(strt->tableLen));
    int l = strt->tableLen - 1;
    int n = 0;

    while (1) {
        if (n == ind)
            return s;
        /* look for next string */
        while (l) {
            s++;
            l--;
            if (*s == '\0') {
                s++;
                n++;
                break;
            }
        }
        if (!l)
            break;
    }
    return NULL;
}

static uof_strTab_T *get_strt(uof_objHdr_T *hdr, dbg_chunkHdr_T *chunk)
{
    uof_strTab_T *ret;
    int cnt;

    ret = (uof_strTab_T *)(CHUNKOFF(hdr, chunk));

#ifdef DEBUG
    printf("strt : tableLen = %d\n", ret->tableLen);
#endif
    cnt = get_strt_cnt(ret);
    if (cnt == 0)
        fprintf(stderr, "warning: no entries found in string table\n");
#ifdef DEBUG
    printf("strt : count = %d\n", cnt);
#endif

    return ret;
}

static void load_dbginfo(dbg_Source_T *dbgsrc,
                         int srccnt,
                         struct uof_dbg_entry *entry)
{
    int64_t tmp;
    int lastpc;
    int a;
    int i, ii;

    lastpc = 0xffff;
    ii = 0;

    for (i = 0; i < srccnt; i++) {
        if ((lastpc != 0xffff) && (lastpc != dbgsrc[i].addr))
            ii++;

        lastpc = dbgsrc[i].addr;
        a = dbgsrc[i].addr;
        entry->vld[a] = 1;

        if (dbgsrc[i].validBkPt == 1)
            entry->data[a] |= NFP_HWDBG_DBGINFO_BRKPTALLOWED;

        if (dbgsrc[i].validBkPt == 2) {
            entry->data[a] |= NFP_HWDBG_DBGINFO_SWBRKPT;
            /* lets allow breaks where there are swbpts, I dont see why not */
            entry->data[a] |= NFP_HWDBG_DBGINFO_BRKPTALLOWED;
        }

        tmp = NFP_HWDBG_DBGINFO_DEFERCNT_of(entry->data[a]);
        if (dbgsrc[i].deferCount > tmp)
            tmp = dbgsrc[i].deferCount; /* use bigger - might not be needed */

        entry->data[a] &= ~(NFP_HWDBG_DBGINFO_DEFERCNT(-1LL));
        entry->data[a] |= NFP_HWDBG_DBGINFO_DEFERCNT(tmp);

        if (dbgsrc[i].brAddr != -1) {
            tmp = dbgsrc[i].brAddr;
            if ((tmp != dbgsrc[i].brAddr) && (tmp != -1)) {
                fprintf(stderr,
                        "conflicting branch information in list file\n");
            } else {
                entry->data[a] &= ~(NFP_HWDBG_DBGINFO_BRADDR(-1LL));
                entry->data[a] |= NFP_HWDBG_DBGINFO_BRADDR(tmp);
            }
        }

        tmp = dbgsrc[i].regAddr;
        entry->data[a] &= ~(NFP_HWDBG_DBGINFO_REGADDR(-1LL));
        entry->data[a] |= NFP_HWDBG_DBGINFO_REGADDR(tmp);

        if (dbgsrc[i].regType != 0) {
            tmp = dbgsrc[i].regType;
            entry->data[a] &= ~(NFP_HWDBG_DBGINFO_REGTYPE(-1LL));
            entry->data[a] |= NFP_HWDBG_DBGINFO_REGTYPE(tmp);
        }

        if (dbgsrc[i].ctxArbKill)
            entry->data[a] |= NFP_HWDBG_DBGINFO_CTXARBKILL;
    }
}

/* reads the ucode debug section from an elf */
static int getdebugdata(const char *filename, char **buf, int *len)
{
    Elf *elf;
    Elf_Scn *scn = NULL;
    Elf_Data *edata = NULL;
    GElf_Shdr shdr;
    size_t shstrndx;
    int fd;
    int found;
    int n;

    fd = open(filename, O_RDWR);
    if (fd < 0) {
        fprintf(stderr, "failed to open elf %s (%s)\n",
                filename, strerror(errno));
        return -1;
    }

    if (elf_version(EV_CURRENT) == EV_NONE)
        fprintf(stderr, "warning: elf library is out of date\n");

    elf = elf_begin(fd, ELF_C_READ, NULL);
    if (!elf) {
        fprintf(stderr, "elf begin failed (%s)\n", elf_errmsg(-1));
        close(fd);
    }

    if (elf_kind(elf) != ELF_K_ELF) {
        fprintf(stderr, "unsupported elf kind\n");
        elf_end(elf);
        close(fd);
        return -1;
    }

    if (elf_getshdrstrndx(elf, &shstrndx)) {
        fprintf(stderr, "failed to get header string info\n");
        elf_end(elf);
        close(fd);
        return -1;
    }

    found = 0;
    while (1) {
        scn = elf_nextscn(elf, scn);
        if (!scn)
            break;

        if (gelf_getshdr(scn, &shdr) != &shdr) {
            fprintf(stderr, "gelf_getshdr failed\n");
            break;
        }

        if (strcmp(DEBUGSECSTR, elf_strptr(elf, shstrndx, shdr.sh_name)))
            continue;

        found = 1;
        break;

    }

    if (!found) {
        fprintf(stderr, "debug data not found in elf %s\n", filename);
        elf_end(elf);
        close(fd);
        return -1;
    }

    *buf = malloc(shdr.sh_size);
    if (!*buf) {
        fprintf(stderr, "failed to allocate memory for debug data\n");
        elf_end(elf);
        close(fd);
        return -1;
    }

    //printf("found debug section (size %ld)\n", shdr.sh_size);
    n = 0;
    while (n < shdr.sh_size) {
        edata = elf_getdata(scn, edata);
        if (!edata) /* no more data */
            break;

        memcpy(*buf + n, (char *) edata->d_buf, edata->d_size);
        n += edata->d_size;
    }

    if (n != shdr.sh_size)
        fprintf(stderr, "warning: only copied %d/%d bytes\n",
                n, (int) shdr.sh_size);

    *len = n;
    elf_end(elf);
    close(fd);

    return 0;
}

int uof_parse(const char *elffile, struct uof_dbg_entry **dbgdat)
{
    char *elfdbgdat;
    int elfdbglen;
    uof_strTab_T *strt = NULL;
    uof_objHdr_T *hdr = NULL;
    dbg_chunkHdr_T *chunks = NULL;
    dbg_Image_T *dbgimg = NULL;
    dgb_ObjTable_T *objtab = NULL;
    dbg_Source_T *dbgsrc = NULL;
    struct uof_dbg_entry *entry;
    int meid;
    int i, j;

    if (getdebugdata(elffile, &elfdbgdat, &elfdbglen) < 0) {
        fprintf(stderr, "failed to get debug data from %s\n", elffile);
        return -1;
    }

    *dbgdat = NULL;

    hdr = (uof_objHdr_T *) elfdbgdat;
#ifdef DEBUG
    print_objhdr(hdr);
#endif

    chunks = (dbg_chunkHdr_T *) (elfdbgdat + sizeof(*hdr));

#ifdef DEBUG
    for (i = 0; i < hdr->numChunks; i++) {
        print_chunkhdr(&chunks[i]);
    }
#endif

    for (i = 0; i < hdr->numChunks; i++) {
        if (memcmp(DBG_STRT_STR, chunks[i].chunkId, strlen(DBG_STRT_STR)))
            continue;

        strt = get_strt(hdr, &chunks[i]);
        if (!strt) {
            fprintf(stderr, "failed to find string table\n");
            free(elfdbgdat);
            return -1;
        }
        break;
    }

    for (i = 0; i < hdr->numChunks; i++) {
        char *s;
        if (memcmp(DBG_IMAG_STR, chunks[i].chunkId, strlen(DBG_IMAG_STR)))
            continue;

        dbgimg = (dbg_Image_T *)(((char *)hdr) + chunks[i].offset);

#ifdef DEBUG
        print_dbgimg(dbgimg, strt);
#endif

        objtab = (dgb_ObjTable_T *) (((char *)hdr) + dbgimg->srcTabOffset);
        dbgsrc = (dbg_Source_T *) (((char *) objtab) + sizeof(dgb_ObjTable_T));

        /* count the number of useful dbg info */
        meid = ((dbgimg->meid & 0xf) + 4) | ((dbgimg->meid & 0xff00) >> 4);
        if (objtab->numEntries == 0) {
            fprintf(stderr, "no src dbginfo for me%x\n", meid);
            continue;
        }
#ifdef DEBUG
        printf("handling debug data for me%x (%s)\n",
               meid,  get_strt_str(strt, dbgimg->lstFileName));
#endif
        entry = malloc(sizeof(*entry));
        memset(entry, '\0', sizeof(*entry));
        s = get_strt_str(strt, dbgimg->lstFileName);
        if (!s)
            s = "unknown listfile";
        entry->listfile_name = strdup(s);
        entry->meid = meid;
        entry->next = *dbgdat;
        *dbgdat = entry;
        for (j = 0; j < 16*1024; j++)
            entry->data[j] |= NFP_HWDBG_DBGINFO_BRADDR(-1);

        load_dbginfo(dbgsrc, objtab->numEntries, entry);
    }

    free(elfdbgdat);
    return 0;
}
