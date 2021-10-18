/* Copyright (C) 2016,  Netronome Systems, Inc.  All rights reserved. */

#include "pif_memops.h"
#include <std/reg_utils.h>
#include <nfp/cls.h>

/* Byte-wise copy from mem to lmem. */
void
mem2lmem_bcopy(__mem __addr40 uint8_t *pktdata,
               __lmem uint8_t *lm_ptr,
               int lm_off_b,
               int lm_len_b)
{
    SIGNAL sig;
    __xread uint8_t xfr_data[_PIF_MEMOPS_XFR_CNT * 4];
    unsigned int i, xfr_b, pkt_off;
    int rem_b = lm_len_b;

    pkt_off = 0;
    while (rem_b > 0) {
        if (rem_b > _PIF_MEMOPS_XFR_CNT*4)
            xfr_b = _PIF_MEMOPS_XFR_CNT*4;
        else
            xfr_b = rem_b;
        __mem_read8(xfr_data, pktdata + pkt_off, xfr_b, _PIF_MEMOPS_XFR_CNT*4, ctx_swap, &sig);
        for (i = 0; i < xfr_b; i++)
            lm_ptr[lm_off_b + i] = xfr_data[i];
        rem_b -= xfr_b;
        pkt_off += xfr_b;
        lm_off_b += xfr_b;
    }
}

/* Byte-wise copy from mem to CLS. */
extern __forceinline void
mem2cls_lwcopy(__mem __addr40 uint8_t *pktdata,
               __cls uint32_t *cls_ptr,
               int cls_len_lw)
{
    SIGNAL sig;
    __xread uint32_t xfr_rd_data[_PIF_MEMOPS_XFR_CNT];
    __xwrite uint32_t xfr_wr_data[_PIF_MEMOPS_XFR_CNT];
    unsigned int i, xfr_lw;
    unsigned int rem_lw = cls_len_lw;

    while (rem_lw > 0) {
        if (rem_lw > _PIF_MEMOPS_XFR_CNT)
            xfr_lw = _PIF_MEMOPS_XFR_CNT;
        else
            xfr_lw = rem_lw;

        __mem_read8(xfr_rd_data,
                    pktdata,
                    xfr_lw * 4,
                    sizeof(xfr_rd_data),
                    ctx_swap,
                    &sig);

        reg_cp(xfr_wr_data, xfr_rd_data, sizeof(xfr_rd_data));

        __cls_write(xfr_wr_data,
                    cls_ptr,
                    xfr_lw * 4,
                    sizeof(xfr_wr_data),
                    ctx_swap, &sig);

        rem_lw -= xfr_lw;
        pktdata += xfr_lw * 4;
        cls_ptr += xfr_lw;
    }
}

/* Byte-wise copy from mem to CLS. */
extern __forceinline void
cls2cls_lwcopy(__align4 __cls uint32_t *cls_in,
               __align4 __cls uint32_t *cls_out,
               int cls_len_lw)
{
    SIGNAL sig;
    __xread uint32_t xfr_rd_data[_PIF_MEMOPS_XFR_CNT];
    __xwrite uint32_t xfr_wr_data[_PIF_MEMOPS_XFR_CNT];
    unsigned int i, xfr_lw;
    unsigned int rem_lw = cls_len_lw;

    while (rem_lw > 0) {
        if (rem_lw > _PIF_MEMOPS_XFR_CNT)
            xfr_lw = _PIF_MEMOPS_XFR_CNT;
        else
            xfr_lw = rem_lw;

        __cls_read(xfr_rd_data,
                   cls_in,
                   xfr_lw * 4,
                   sizeof(xfr_rd_data),
                   ctx_swap,
                   &sig);

        reg_cp(xfr_wr_data, xfr_rd_data, sizeof(xfr_rd_data));

        __cls_write(xfr_wr_data,
                    cls_out,
                    xfr_lw * 4,
                    sizeof(xfr_wr_data),
                    ctx_swap,
                    &sig);

        rem_lw -= xfr_lw;
        cls_in += xfr_lw;
        cls_out += xfr_lw;
    }
}
