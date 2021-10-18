/* Copyright (C) 2015,  Netronome Systems, Inc.  All rights reserved. */

#ifndef __PIF_MEMOPS_H__
#define __PIF_MEMOPS_H__

#include <nfp.h>
#include <stdint.h>
#include <nfp/mem_bulk.h>
#include <std/reg_utils.h>

#define _PIF_MEMOPS_XFR_CNT  8

/* Copy byte aligned mem data to 4B aligned lm */
__forceinline void
mem2lmem_copy(__mem __addr40 uint8_t *pktdata,
              __lmem uint32_t *lm_ptr,
              int off_lw,
              int len_lw)
{
    SIGNAL sig;
    __xread uint32_t read_xfer[8];
    unsigned int i, tocopy, chunk_lw, off, copied;

    tocopy = len_lw;
    off = off_lw;
    copied = 0;

    /* tocopy MUST be constant */
    if (tocopy * 4 <= sizeof(read_xfer) * 8) {
        /* do manual loop unrolling */

#define _DO_COPY_ONE \
        do { \
            mem_read8(read_xfer, \
                      pktdata + copied, \
                      sizeof(read_xfer)); \
            reg_cp(lm_ptr + off, read_xfer, sizeof(read_xfer)); \
            copied += sizeof(read_xfer); \
            off += sizeof(read_xfer) / 4; \
        } while (0);

        if ((tocopy * 4)/sizeof(read_xfer) > 0)
            _DO_COPY_ONE;
        if ((tocopy * 4)/sizeof(read_xfer) > 1)
            _DO_COPY_ONE;
        if ((tocopy * 4)/sizeof(read_xfer) > 2)
            _DO_COPY_ONE;
        if ((tocopy * 4)/sizeof(read_xfer) > 3)
            _DO_COPY_ONE;
        if ((tocopy * 4)/sizeof(read_xfer) > 4)
            _DO_COPY_ONE;
        if ((tocopy * 4)/sizeof(read_xfer) > 5)
            _DO_COPY_ONE;
        if ((tocopy * 4)/sizeof(read_xfer) > 6)
            _DO_COPY_ONE;
        if ((tocopy * 4)/sizeof(read_xfer) > 7)
            _DO_COPY_ONE;

        if ((tocopy * 4) % sizeof(read_xfer)) {
            /* last piece is partial */
            __mem_read8(read_xfer,
                        pktdata + copied,
                        (tocopy * 4) % sizeof(read_xfer), sizeof(read_xfer),
                        ctx_swap, &sig);

            reg_cp(lm_ptr + off, read_xfer, (tocopy * 4) % sizeof(read_xfer));
        }
#undef _DO_COPY_ONE
    } else {
        while (tocopy > 0) {
            if (tocopy > sizeof(read_xfer)/4)
                chunk_lw = sizeof(read_xfer)/4;
            else
                chunk_lw = tocopy;

            __mem_read8(read_xfer,
                        pktdata + copied,
                        chunk_lw * 4, sizeof(read_xfer),
                        ctx_swap, &sig);

            for (i = 0; i < chunk_lw; i++)
                lm_ptr[i + off] = read_xfer[i];

            tocopy -= chunk_lw;
            copied += chunk_lw * 4;
            off += chunk_lw;
        }
    }
}

/* Byte-wise copy from mem to lmem. */
void
mem2lmem_bcopy(__mem __addr40 uint8_t *pktdata,
               __lmem uint8_t *lm_ptr,
               int lm_off_b,
               int lm_en_b);

/* Byte-wise copy from mem to CLS. */
void
mem2cls_lwcopy(__mem __addr40 uint8_t *pktdata,
               __cls uint32_t *cls_ptr,
               int cls_len_lw);

void
cls2cls_lwcopy(__align4 __cls uint32_t *cls_in,
               __align4 __cls uint32_t *cls_out,
               int cls_len_lw);


#endif /* __PIF_MEMOPS_H__ */
