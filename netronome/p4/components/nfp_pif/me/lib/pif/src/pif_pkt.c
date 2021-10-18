/* Copyright (C) 2015,  Netronome Systems, Inc.  All rights reserved. */

#include <std/reg_utils.h>
#include <pkt/pkt.h>
#include <nfp/mem_pe.h>
#include <nfp/cls.h>
#include <pktdma/pktdma.h>
#include <blm_c.h>
#include "pif_common.h"

#define _PIF_XFR_LW     8
#define _PIF_XFR_BYTES  ((_PIF_XFR_LW) * 4)

unsigned int
pif_pkt_write_header(unsigned int pktoff,
                     __lmem uint32_t *lmem,
                     unsigned int lmoff,
                     unsigned int hdrlen)
{
    SIGNAL sig;
    __xwrite uint32_t write_xfer[8];
    unsigned int i,  chunk;
    __gpr uint32_t ctm_len_max = (256 << pif_pkt_info_global.p_ctm_size) - pif_pkt_info_global.p_offset;
    __mem __addr40 uint8_t *pkt_buf = ( __mem __addr40 uint8_t *)pkt_ctm_ptr40(0, pif_pkt_info_global.p_pnum, pif_pkt_info_global.p_offset);

    __mem __addr40 uint8_t *mu_base_ptr;
    int overshoot = 0, ctm_remnants, tocopy;
    int mu_off = (256 << pif_pkt_info_global.p_ctm_size);

    /* check if the header overshoots into emem */
    if (pktoff + hdrlen > ctm_len_max)
        overshoot = pktoff + hdrlen - ctm_len_max;

    tocopy = hdrlen - overshoot;

    while (tocopy > 0) {
        if (tocopy > _PIF_XFR_BYTES)
            chunk = _PIF_XFR_BYTES;
        else
            chunk = tocopy;

#if _PIF_XFR_LW != 8
#error _PIF_XFR_LW must be 8
#endif
        write_xfer[0] = lmem[lmoff++];
        write_xfer[1] = lmem[lmoff++];
        write_xfer[2] = lmem[lmoff++];
        write_xfer[3] = lmem[lmoff++];
        write_xfer[4] = lmem[lmoff++];
        write_xfer[5] = lmem[lmoff++];
        write_xfer[6] = lmem[lmoff++];
        write_xfer[7] = lmem[lmoff++];

        __mem_write8(write_xfer,
                     pkt_buf + pktoff,
                     chunk, _PIF_XFR_BYTES, ctx_swap, &sig);

        ctm_remnants = _PIF_XFR_BYTES - chunk;
        tocopy -= chunk;
        pktoff += chunk;
    }

    if (overshoot > 0) {
        pktoff = 0;
        mu_base_ptr = (__mem __addr40 uint8_t *)(((uint64_t)pif_pkt_info_global.p_muptr) << 11);

        if (ctm_remnants) {
            int to_write = _PIF_XFR_BYTES;
            /* the previous process leaves a certain amount of
             * data in write xfers and advances lmem pointer to
             * beyond where we expect it to be for the beginning
             * of mu buffer, hence we write the previous data
             * directly into the start of the mu,
             * taking care to offset by the amount require to
             * align the remnants to where they should go
             */

            /* make sure we account for the ctm_remnants in what
             * we plan on writing to the MU
             */
            overshoot -= ctm_remnants;
            if (overshoot < 0) /* more remnants than required, so write less */
                to_write += overshoot;

            __mem_write8(write_xfer,
                         mu_base_ptr + mu_off - (8*4 - ctm_remnants),
                         to_write, _PIF_XFR_BYTES, ctx_swap, &sig);
            mu_off += ctm_remnants;
        }

        pktoff += overshoot;

        while (overshoot > 0) {
            if (overshoot > _PIF_XFR_BYTES)
                chunk = _PIF_XFR_BYTES;
            else
                chunk = overshoot;

#if _PIF_XFR_LW != 8
#error _PIF_XFR_LW must be 8
#endif
            write_xfer[0] = lmem[lmoff++];
            write_xfer[1] = lmem[lmoff++];
            write_xfer[2] = lmem[lmoff++];
            write_xfer[3] = lmem[lmoff++];
            write_xfer[4] = lmem[lmoff++];
            write_xfer[5] = lmem[lmoff++];
            write_xfer[6] = lmem[lmoff++];
            write_xfer[7] = lmem[lmoff++];

            __mem_write8(write_xfer,
                         mu_base_ptr + mu_off,
                         chunk, _PIF_XFR_BYTES, ctx_swap, &sig);

            overshoot -= chunk;
            mu_off += chunk;
        }
    }
    __critical_path();

    return pktoff;
}


void
pif_pkt_write_vheader(unsigned int pktoff,
                     __lmem uint32_t *lmptr,
                     unsigned int lmsz,
                     unsigned int hdrlen,
                     __cls uint32_t *varptr,
                     unsigned int varoff_b)
{
    SIGNAL sig;
    __mem __addr40 uint8_t *pkt_buf = ( __mem __addr40 uint8_t *)pkt_ctm_ptr40(0, pif_pkt_info_global.p_pnum, pif_pkt_info_global.p_offset);
    __xread uint32_t xfr_rd_data[_PIF_XFR_LW];
    __xrw uint32_t xfr_wr_data[_PIF_XFR_LW];
    unsigned int i, xfr_b, off_b, xfr_lw;
    unsigned int rem_b = varoff_b;

    /* Write fixed start of header first. */
    while (rem_b > 0) {
        if (rem_b > _PIF_XFR_BYTES)
            xfr_b = _PIF_XFR_BYTES;
        else
            xfr_b = rem_b;

#if _PIF_XFR_LW != 8
#error _PIF_XFR_LW must be 8
#endif
        xfr_wr_data[0] = *lmptr++;
        xfr_wr_data[1] = *lmptr++;
        xfr_wr_data[2] = *lmptr++;
        xfr_wr_data[3] = *lmptr++;
        xfr_wr_data[4] = *lmptr++;
        xfr_wr_data[5] = *lmptr++;
        xfr_wr_data[6] = *lmptr++;
        xfr_wr_data[7] = *lmptr++;

        __mem_write8(xfr_wr_data,
                     pkt_buf + pktoff,
                     xfr_b, sizeof(xfr_wr_data),
                     ctx_swap, &sig);
        rem_b -= xfr_b;
        pktoff += xfr_b;
    }

    /* Write variable part of header. */
    rem_b = hdrlen - lmsz;
    while (rem_b > 0) {
        if (rem_b > _PIF_XFR_BYTES) {
            xfr_b = _PIF_XFR_BYTES;
            xfr_lw = _PIF_XFR_LW;
        } else {
            xfr_b = rem_b;
            xfr_lw = (rem_b + 3) / 4;
        }

        cls_read(xfr_rd_data, varptr, sizeof(xfr_rd_data));
        reg_cp(xfr_wr_data, xfr_rd_data, sizeof(xfr_rd_data));
        __mem_write8(xfr_wr_data,
                     pkt_buf + pktoff,
                     xfr_b, sizeof(xfr_wr_data),
                     ctx_swap, &sig);
        rem_b -= xfr_b;
        pktoff += xfr_b;
        varptr += _PIF_XFR_LW;
    }
}


void pif_pkt_free_space(unsigned int pktoff, unsigned int hdrlen)
{
    SIGNAL sig;
    __xread uint32_t read_xfer[_PIF_XFR_LW];
    __xwrite uint32_t write_xfer[_PIF_XFR_LW];
    __mem __addr40 uint8_t *pkt_buf = ( __mem __addr40 uint8_t *)pkt_ctm_ptr40(0, pif_pkt_info_global.p_pnum, pktoff);
    __mem __addr40 void *mu_base;
    unsigned int mu_len, mu_off;
    int toshift_mu, toshift_mu2ctm, toshift_ctm;
    unsigned int i, chunk;

    if (pif_pkt_info_global.p_is_split == 0) {
        mu_len = 0;
        toshift_ctm = pif_pkt_info_global.p_len - pktoff - hdrlen;
        toshift_mu2ctm = 0;
        toshift_mu = 0;
    } else {
        mu_len = pif_pkt_info_global.p_len - (256 << pif_pkt_info_global.p_ctm_size) + pif_pkt_info_global.p_offset;

        toshift_ctm = pif_pkt_info_global.p_len - mu_len - pktoff - hdrlen;
        if (mu_len < hdrlen)
            toshift_mu2ctm = mu_len;
        else
            toshift_mu2ctm = hdrlen;
        toshift_mu = pif_pkt_info_global.p_len - pktoff - hdrlen - toshift_ctm -
            toshift_mu2ctm;
    }

    /* Update pktinfo */
    pif_pkt_info_global.p_len -= hdrlen;
    if (toshift_mu == 0)
        pif_pkt_info_global.p_is_split = 0;
    else
        pif_pkt_info_global.p_is_split = 1;

    /* Shift CTM "up" */
    while (toshift_ctm > 0) {
        if (toshift_ctm > _PIF_XFR_BYTES)
            chunk = _PIF_XFR_BYTES;
        else
            chunk = toshift_ctm;

        __mem_read8(read_xfer,
            pkt_buf + pktoff + hdrlen,
            chunk, _PIF_XFR_BYTES, ctx_swap, &sig);

        reg_cp(write_xfer, read_xfer, sizeof(read_xfer));

        __mem_write8(write_xfer,
            pkt_buf + pktoff,
            chunk, _PIF_XFR_BYTES, ctx_swap, &sig);

        pktoff += chunk;
        toshift_ctm -= chunk;
    }

    /* Shift MU to CTM */
    mu_base = (__mem __addr40 void *)((uint64_t)pif_pkt_info_global.p_muptr << 11);
    mu_off = (256 << pif_pkt_info_global.p_ctm_size) + mu_len - toshift_mu2ctm - toshift_mu;

    while (toshift_mu2ctm > 0) {
        if (toshift_mu2ctm > _PIF_XFR_BYTES)
            chunk = _PIF_XFR_BYTES;
        else
            chunk = toshift_mu2ctm;

        __mem_read8(read_xfer,
            (__mem __addr40 uint8_t *)((uint64_t)mu_base + (uint64_t)mu_off),
            chunk, _PIF_XFR_BYTES, ctx_swap, &sig);

        reg_cp(write_xfer, read_xfer, sizeof(read_xfer));

        __mem_write8(write_xfer,
            pkt_buf + pktoff,
            chunk, _PIF_XFR_BYTES, ctx_swap, &sig);

        pktoff += chunk;
        mu_off += chunk;
        toshift_mu2ctm -= chunk;
    }

    /* Shift MU "up" */
    while (toshift_mu > 0) {
        if (toshift_mu > _PIF_XFR_BYTES)
            chunk = _PIF_XFR_BYTES;
        else
            chunk = toshift_mu;

        __mem_read8(read_xfer,
            (__mem __addr40 uint8_t *)((uint64_t)((uint64_t)pif_pkt_info_global.p_muptr << 11) + (uint64_t)mu_off),
            chunk, _PIF_XFR_BYTES, ctx_swap, &sig);

        reg_cp(write_xfer, read_xfer, sizeof(read_xfer));

        __mem_write8(write_xfer,
            (__mem __addr40 uint8_t *)((uint64_t)((uint64_t)pif_pkt_info_global.p_muptr << 11) + (uint64_t)mu_off - (uint64_t)hdrlen),
            chunk, _PIF_XFR_BYTES, ctx_swap, &sig);

        mu_off += chunk;
        toshift_mu -= chunk;
    }
}



void pif_pkt_move_pkt_up(unsigned int pktoff, unsigned int removelen)
{
    SIGNAL sig;
    __xread uint32_t read_xfer[_PIF_XFR_LW];
    __xwrite uint32_t write_xfer[_PIF_XFR_LW];
    __mem __addr40 uint8_t *pkt_buf = ( __mem __addr40 uint8_t *)pkt_ctm_ptr40(0, pif_pkt_info_global.p_pnum, 0);
    __mem __addr40 void *mu_base;
    unsigned int mu_len, mu_off;
    int toshift_mu, toshift_mu2ctm, toshift_ctm;
    unsigned int i, chunk;

    if (pif_pkt_info_global.p_is_split == 0) {
        mu_len = 0;
        toshift_ctm = pif_pkt_info_global.p_len;
        toshift_mu2ctm = 0;
        toshift_mu = 0;
    } else {
        mu_len = pif_pkt_info_global.p_len - (256 << pif_pkt_info_global.p_ctm_size) + pif_pkt_info_global.p_offset;

        toshift_ctm = pif_pkt_info_global.p_len - mu_len;
        if (mu_len < removelen)
            toshift_mu2ctm = mu_len;
        else
            toshift_mu2ctm = removelen;
        toshift_mu = pif_pkt_info_global.p_len - toshift_ctm - toshift_mu2ctm;
    }

    /* Update pktinfo */
    if (toshift_mu == 0)
        pif_pkt_info_global.p_is_split = 0;
    else
        pif_pkt_info_global.p_is_split = 1;

    /* Shift CTM "up" */
    while (toshift_ctm > 0) {
        if (toshift_ctm > _PIF_XFR_BYTES)
            chunk = _PIF_XFR_BYTES;
        else
            chunk = toshift_ctm;

        __mem_read8(read_xfer,
            pkt_buf + pktoff,
            chunk, _PIF_XFR_BYTES, ctx_swap, &sig);

        reg_cp(write_xfer, read_xfer, sizeof(read_xfer));

        __mem_write8(write_xfer,
            pkt_buf + pktoff - removelen,
            chunk, _PIF_XFR_BYTES, ctx_swap, &sig);

        pktoff += chunk;
        toshift_ctm -= chunk;
    }

    /* Shift MU to CTM */
    mu_base = (__mem __addr40 void *)((uint64_t)pif_pkt_info_global.p_muptr << 11);
    mu_off = (256 << pif_pkt_info_global.p_ctm_size) + mu_len - toshift_mu2ctm - toshift_mu;

    while (toshift_mu2ctm > 0) {
        if (toshift_mu2ctm > _PIF_XFR_BYTES)
            chunk = _PIF_XFR_BYTES;
        else
            chunk = toshift_mu2ctm;

        __mem_read8(read_xfer,
            (__mem __addr40 uint8_t *)((uint64_t)mu_base + (uint64_t)mu_off),
            chunk, _PIF_XFR_BYTES, ctx_swap, &sig);

        reg_cp(write_xfer, read_xfer, sizeof(read_xfer));

        __mem_write8(write_xfer,
            pkt_buf + pktoff - removelen,
            chunk, _PIF_XFR_BYTES, ctx_swap, &sig);

        pktoff += chunk;
        mu_off += chunk;
        toshift_mu2ctm -= chunk;
    }

    /* Shift MU "up" */
    while (toshift_mu > 0) {
        if (toshift_mu > _PIF_XFR_BYTES)
            chunk = _PIF_XFR_BYTES;
        else
            chunk = toshift_mu;

        __mem_read8(read_xfer,
            (__mem __addr40 uint8_t *)((uint64_t)((uint64_t)pif_pkt_info_global.p_muptr << 11) + (uint64_t)mu_off),
            chunk, _PIF_XFR_BYTES, ctx_swap, &sig);

        reg_cp(write_xfer, read_xfer, sizeof(read_xfer));

        __mem_write8(write_xfer,
            (__mem __addr40 uint8_t *)((uint64_t)((uint64_t)pif_pkt_info_global.p_muptr << 11) + (uint64_t)mu_off - (uint64_t)removelen),
            chunk, _PIF_XFR_BYTES, ctx_swap, &sig);

        mu_off += chunk;
        toshift_mu -= chunk;
    }

}


void pif_pkt_insert_header(unsigned int copylen, unsigned int hdrlen)
{
    SIGNAL              sig;
    __mem __addr40 uint8_t    *pkt_buf = ( __mem __addr40 uint8_t *)pkt_ctm_ptr40(0, pif_pkt_info_global.p_pnum,  pif_pkt_info_global.p_offset);
    __xread uint32_t    read_xfer[_PIF_XFR_LW];
    __xwrite uint32_t   write_xfer[_PIF_XFR_LW];
    __gpr int32_t       tocopy = copylen;
    __gpr uint32_t      chunk = _PIF_XFR_BYTES;
    __gpr uint32_t      bytes_copied = 0;

    /*
     * Move packet headers left/up to make space for new header.
     * Copy data at SOH to (SOH - hdrlen) for copylen
     */

    while (tocopy)
    {
        if (tocopy < _PIF_XFR_BYTES)
            chunk = tocopy;

        __mem_read8(read_xfer,
            pkt_buf + bytes_copied,
            chunk, _PIF_XFR_BYTES, ctx_swap, &sig);

        reg_cp(write_xfer, read_xfer, sizeof(read_xfer));

        __mem_write8(write_xfer,
            pkt_buf  - hdrlen + bytes_copied,
            chunk, _PIF_XFR_BYTES, ctx_swap, &sig);

        tocopy -= chunk;
        bytes_copied += chunk;
    }

    // Update pktinfo - increase pkt_len and move pkt_buf pointer "up/left".
    pif_pkt_info_global.p_len += hdrlen;
    pif_pkt_info_global.p_offset -=  hdrlen;
}

void pif_pkt_remove_header(unsigned int copylen, unsigned int hdrlen)
{
    SIGNAL              sig;
    __mem __addr40 uint8_t    *pkt_buf = ( __mem __addr40 uint8_t *)pkt_ctm_ptr40(0, pif_pkt_info_global.p_pnum,  pif_pkt_info_global.p_offset);
    __xread uint32_t    read_xfer[_PIF_XFR_LW];
    __xwrite uint32_t   write_xfer[_PIF_XFR_LW];
    __gpr int32_t       tocopy = copylen;
    __gpr uint32_t      chunk = _PIF_XFR_BYTES;
    __gpr uint32_t      bytes_copied = 0;
    __gpr int32_t       offset = copylen;

    /*
     * Move packet headers right/down to remove headers.
     * Copy data at SOH to (SOH + hdrlen) for copylen
     */
    while (tocopy)
    {
        if (tocopy < _PIF_XFR_BYTES)
            chunk = tocopy;

        __mem_read8(read_xfer,
            pkt_buf + offset - chunk,
            chunk, _PIF_XFR_BYTES, ctx_swap, &sig);

        reg_cp(write_xfer, read_xfer, sizeof(read_xfer));

        __mem_write8(write_xfer,
            pkt_buf + hdrlen + offset - chunk,
            chunk, _PIF_XFR_BYTES, ctx_swap, &sig);

        tocopy -= chunk;
        bytes_copied += chunk;
        offset -= chunk;
    }

    /* Update pktinfo - decrease pkt_len and move pkt_buf pointer "down/right". */
    pif_pkt_info_global.p_len -= hdrlen;
    pif_pkt_info_global.p_offset += hdrlen;
}


void pif_pkt_make_space(unsigned int pktoff, unsigned int hdrlen)
{
    SIGNAL sig;
    __xread uint32_t read_xfer[_PIF_XFR_LW];
    __xwrite uint32_t write_xfer[_PIF_XFR_LW];
    __mem __addr40 uint8_t *mu_base_ptr;
    __mem __addr40 uint8_t *ctm_base_ptr = (__mem __addr40 uint8_t *)pkt_ctm_ptr40(0, pif_pkt_info_global.p_pnum,  pif_pkt_info_global.p_offset);
    unsigned int ctm_len_max, ctm_len_init, ctm_len_final, ctm_off;
    unsigned int i, chunk;
    int toshift_mu, toshift_ctm2mu, toshift_ctm, ctm_overshoot;
    unsigned int mu_off;

    /* How much data can we cram in CTM */
    ctm_len_max = (256 << pif_pkt_info_global.p_ctm_size) - pif_pkt_info_global.p_offset;

    /* How much data will end up in CTM */
    ctm_len_final = pif_pkt_info_global.p_len + hdrlen;
    if (ctm_len_final > ctm_len_max)
        ctm_len_final = ctm_len_max;

    /* get the length of data in the ctm */
    ctm_len_init = pif_pkt_info_global.p_len;
    if (ctm_len_init > ctm_len_max)
        ctm_len_init = ctm_len_max;

    /* Figure out how much data we need to move within the CTM after
     * making space
     */
    toshift_ctm = ctm_len_final - pktoff - hdrlen;
    if (toshift_ctm < 0) { /* negative means data will in fact overshoot */
        ctm_overshoot = -toshift_ctm;
        toshift_ctm = 0;
    } else {
        ctm_overshoot = 0;
        /* the offset at which we shift from ctm in th toshift_ctm case */
        ctm_off = ctm_len_final - hdrlen;
    }

    /* Now figure out how much data we need to move from ctm to mu */
    toshift_ctm2mu = ctm_len_init - pktoff - toshift_ctm;

    /* Finally figure out how much data we move into MU */
    if (pif_pkt_info_global.p_is_split == 0)
        toshift_mu = 0;
    else
        toshift_mu = pif_pkt_info_global.p_len - ctm_len_final;

    /* Shift MU "down" */
    mu_off = (256 << pif_pkt_info_global.p_ctm_size) + pif_pkt_info_global.p_len - ctm_len_final;

    mu_base_ptr = (__mem __addr40 uint8_t *)(((uint64_t)pif_pkt_info_global.p_muptr) << 11);
    while (toshift_mu > 0) {
        __mem __addr40 uint8_t *mem_ptr;

        if (toshift_mu > _PIF_XFR_BYTES)
            chunk = _PIF_XFR_BYTES;
        else
            chunk = toshift_mu;

        __mem_read8(read_xfer,
                    mu_base_ptr + (mu_off - chunk),
                    chunk, _PIF_XFR_BYTES, ctx_swap, &sig);

        reg_cp(write_xfer, read_xfer, sizeof(read_xfer));

        __mem_write8(write_xfer,
                     mu_base_ptr + (mu_off - chunk + hdrlen),
                     chunk, _PIF_XFR_BYTES, ctx_swap, &sig);

        mu_off -= chunk;
        toshift_mu -= chunk;
    }

    /* Update pktinfo */
    pif_pkt_info_global.p_len += hdrlen;
    if (toshift_ctm2mu == 0)
        pif_pkt_info_global.p_is_split = 0;
    else
        pif_pkt_info_global.p_is_split = 1;

    /* Shift CTM to MU */
    mu_base_ptr = (__mem __addr40 uint8_t *)(((uint64_t)pif_pkt_info_global.p_muptr) << 11);
    ctm_off = ctm_len_init - toshift_ctm2mu;
    mu_off = (256 << pif_pkt_info_global.p_ctm_size) + ctm_overshoot;

    while (toshift_ctm2mu > 0) {
        if (toshift_ctm2mu > _PIF_XFR_BYTES)
            chunk = _PIF_XFR_BYTES;
        else
            chunk = toshift_ctm2mu;

        __mem_read8(read_xfer,
                    ctm_base_ptr + ctm_off,
                    chunk, _PIF_XFR_BYTES, ctx_swap, &sig);

        reg_cp(write_xfer, read_xfer, sizeof(read_xfer));

        __mem_write8(write_xfer,
                     mu_base_ptr + mu_off,
                     chunk, _PIF_XFR_BYTES, ctx_swap, &sig);

        ctm_off += chunk;
        mu_off += chunk;
        toshift_ctm2mu -= chunk;
    }

    /* Shift CTM "down" */
    ctm_off = ctm_len_final - hdrlen;
    while (toshift_ctm > 0) {
        if (toshift_ctm > _PIF_XFR_BYTES)
            chunk = _PIF_XFR_BYTES;
        else
            chunk = toshift_ctm;

        __mem_read8(read_xfer,
                    ctm_base_ptr + ctm_off - chunk,
                    chunk, _PIF_XFR_BYTES, ctx_swap, &sig);

        reg_cp(write_xfer, read_xfer, sizeof(read_xfer));

        __mem_write8(write_xfer,
                     ctm_base_ptr + ctm_off - chunk + hdrlen,
                     chunk, _PIF_XFR_BYTES, ctx_swap, &sig);

        ctm_off -= chunk;
        toshift_ctm -= chunk;
    }

    return;
}

int pif_pkt_dma_packet_down(unsigned int insertlen)
{
    __gpr uint32_t pkt_len_padded;
    __gpr uint32_t pkt_len_plus_insertlen;
    __gpr uint32_t ctm_buffer_size = 256 << pif_pkt_info_global.p_ctm_size;
    __mem __addr40 uint8_t *pkt_buf = ( __mem __addr40 uint8_t *)pkt_ctm_ptr40(0, pif_pkt_info_global.p_pnum,  pif_pkt_info_global.p_offset);

    // TODO verify insertlen is 64B aligned

    /*  metadata ---------------| SOP --------------------------| EOP
     *  0                       | <-----------pkt_len---------> |
     *  |<------------------------pkt_len_padded---------------------------->| aligned to 64B
     */

    pkt_len_padded = pif_pkt_info_global.p_len + pif_pkt_info_global.p_offset;         // packet len from start of buffer
    pkt_len_padded = (pkt_len_padded + (64-1)) & (~(64-1));     // align to the next higher 64B boundary
    pkt_len_plus_insertlen = pkt_len_padded + insertlen;

    if (pkt_len_plus_insertlen <= ctm_buffer_size && pif_pkt_info_global.p_is_split == 0) {

        /*
        * shifted packet still fits into CTM buffer, copy to MU and back to CTM
        */

        __mem __addr40 uint8_t *ctm_source_ptr;
        __mem __addr40 uint8_t *ctm_dest_ptr;
        __mem __addr40 uint8_t *mu_dest_ptr;
        __xread pkt_status_t pkt_status;

        pkt_status_read(__ISLAND, pif_pkt_info_global.p_pnum, &pkt_status);
        ctm_source_ptr = (__mem __addr40 uint8_t *)(pkt_status.ctm_addr_div256 * 256);
        ctm_dest_ptr = ((__mem __addr40 uint8_t *)ctm_source_ptr) + insertlen;

        mu_dest_ptr = (__mem __addr40 uint8_t *)((uint64_t)pif_pkt_info_global.p_muptr << 11);
        mu_dest_ptr = ((__mem __addr40 uint8_t *)mu_dest_ptr) + insertlen;

        pktdma_ctm_to_mu(mu_dest_ptr, ctm_source_ptr, pkt_len_padded);
        pktdma_mu_to_ctm(ctm_dest_ptr, mu_dest_ptr, pkt_len_padded);

        // Update pktinfo - move pkt_buf pointer "down/right".
        pif_pkt_info_global.p_offset += insertlen;

        return 1;

    } else if (pkt_len_plus_insertlen > ctm_buffer_size && pif_pkt_info_global.p_is_split == 0)  {

        pif_pkt_info_global.p_is_split = 1;

    }

    {
        __gpr uint32_t mu_buffer_size = NBI8_BLQ_EMU_0_PKTBUF_SIZE;
        __gpr int32_t bytes_to_copy = pkt_len_padded;
        __gpr uint32_t offset = pkt_len_padded;
        __gpr uint32_t xfer_len;
        __mem __addr40 uint8_t *ctm_offset_0_ptr;
        __xread pkt_status_t pkt_status;
        __mem __addr40 uint8_t *mu_offset_0_ptr = (__mem __addr40 uint8_t *)((uint64_t)pif_pkt_info_global.p_muptr << 11);


        pkt_status_read(__ISLAND, pif_pkt_info_global.p_pnum, &pkt_status);
        ctm_offset_0_ptr = (__mem __addr40 uint8_t *) (pkt_status.ctm_addr_div256 * 256);

#if 0
        // TODO - find out what the MU buffer size is based on the BLQ in bls field
        //         if (pif_pkt_info_global.bls == BLM_NBI8_BLQ0_EMU_QID)
        //             mu_buffer_size = NBI8_BLQ_EMU_0_PKTBUF_SIZE;
        //         else if (pif_pkt_info_global.bls == BLM_NBI8_BLQ1_EMU_QID)
        //             mu_buffer_size = NBI8_BLQ_EMU_1_PKTBUF_SIZE;
        //         else if (pif_pkt_info_global.bls == BLM_NBI8_BLQ2_EMU_QID)
        //             mu_buffer_size = NBI8_BLQ_EMU_2_PKTBUF_SIZE;
#endif
            if (pkt_len_plus_insertlen > mu_buffer_size) {

                // TODO allocate a new buffer
                // error
                return 0;

            }
            {


                pktdma_ctm_to_mu(mu_offset_0_ptr, ctm_offset_0_ptr, ctm_buffer_size); // DMA entire CTM buffer to MU

                /*
                * shift in main mem by insertlen .. start copying from end of buffer
                *   - copy ctm_buffer_size chunk from MU buffer at offset to CTM buffer (offset 0)
                *   - copy ctm_buffer_size chunk to MU buffer with offset insert_len
                */
                do
                {
                    if (bytes_to_copy > ctm_buffer_size)
                        xfer_len = ctm_buffer_size;
                    else
                        xfer_len = bytes_to_copy;

                    offset -= xfer_len;

                    pktdma_mu_to_ctm(ctm_offset_0_ptr, mu_offset_0_ptr + offset, xfer_len);
                    pktdma_ctm_to_mu(mu_offset_0_ptr + offset + insertlen, ctm_offset_0_ptr, xfer_len);

                    bytes_to_copy -= xfer_len;

                } while (bytes_to_copy > 0);
            }

            {
                /*
                * Copy CTM slice from MU to CTM
                */
                pktdma_mu_to_ctm(ctm_offset_0_ptr, mu_offset_0_ptr, ctm_buffer_size);

                // Update pktinfo - move pkt_buf pointer "down/right".
                pif_pkt_info_global.p_offset += insertlen;
            }
    }
    return 1;
}
