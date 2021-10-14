/* Copyright (C) 2015-2016,  Netronome Systems, Inc.  All rights reserved. */

#include <nfp.h>
#include <nfp/me.h>
#include <pkt/pkt.h>
#include <modscript/modscript.h>
#ifdef PKTIO_GRO_ENABLED
#include <gro.h>
#endif
#include "nfd_user_cfg.h"
#include "pif_common.h"
#include <pif_counters.h>

__forceinline static int handle_tier_0(__lmem uint32_t *parrep, PIF_PKT_INFO_TYPE struct pif_pkt_info *pktinfo, int *pkt_byteoff, uint32_t pkt_min_off)
{
    __lmem struct pif_parrep_ctldata *ctldata = (__lmem struct pif_parrep_ctldata *)(parrep + PIF_PARREP_CTLDATA_OFF_LW);
    uint32_t orig_len = PIF_PARREP_T0_ORIG_LEN(ctldata);
    uint32_t curr_len = 0;

    if (!PIF_PARREP_T0_DIRTY(ctldata)) {
        *pkt_byteoff -= orig_len;
        if (*pkt_byteoff < 0)
            return -PIF_DEPARSE_ERROR_BUFFER_OVERRUN;

        return 0;
    }

    if (PIF_PARREP_T0_VALID(ctldata)) {
        curr_len = PIF_PARREP_eth_LEN_B;
    }

    if (curr_len) {
        *pkt_byteoff -= curr_len;
        if (*pkt_byteoff < 0)
            return -PIF_DEPARSE_ERROR_BUFFER_OVERRUN;

        pif_pkt_write_header(*pkt_byteoff,
                             parrep,
                             PIF_PARREP_T0_OFF_LW,
                             curr_len);
    }

    return 0;
}

__forceinline static int handle_tier_1(__lmem uint32_t *parrep, PIF_PKT_INFO_TYPE struct pif_pkt_info *pktinfo, int *pkt_byteoff, uint32_t pkt_min_off)
{
    __lmem struct pif_parrep_ctldata *ctldata = (__lmem struct pif_parrep_ctldata *)(parrep + PIF_PARREP_CTLDATA_OFF_LW);
    uint32_t orig_len = PIF_PARREP_T1_ORIG_LEN(ctldata);
    uint32_t curr_len = 0;

    if (!PIF_PARREP_T1_DIRTY(ctldata)) {
        *pkt_byteoff -= orig_len;
        if (*pkt_byteoff < 0)
            return -PIF_DEPARSE_ERROR_BUFFER_OVERRUN;

        return 0;
    }

    if (PIF_PARREP_T1_VALID(ctldata)) {
        curr_len = PIF_PARREP_xlan_LEN_B;
    }

    if (curr_len < orig_len) {
        unsigned int delta = orig_len - curr_len;

        /*
         * Remove header space ending at *pkt_byteoff of length 'delta'.
         * Adjust pkt_byteoff to make provision for change in pkt_buf (buffer pointer)
         * because pkt_byteoff is relative to start of pkt_buf
         */
        *pkt_byteoff -= delta;
        if (*pkt_byteoff < 0)
            return -PIF_DEPARSE_ERROR_BUFFER_OVERRUN;

        pif_pkt_remove_header(*pkt_byteoff, delta);

        pif_pkt_info_spec.pkt_pl_off -= delta;
    } else if (curr_len > orig_len) {
        unsigned int delta = curr_len - orig_len;
        unsigned int num_bytes;
        int space_needed;

        num_bytes = pktinfo->p_offset;
        space_needed = pkt_min_off + delta;
        /*
         * Insert header space of length 'delta' up to current *pkt_byteoff.
         * Then increase *pkt_byteoff to make provision for change in pkt_buf (buffer pointer)
         * because pkt_byteoff is relative to start of pkt_buf
         */
        if (num_bytes >= space_needed) {
            pif_pkt_insert_header(*pkt_byteoff, delta);
        } else {
            pif_pkt_make_space(*pkt_byteoff, delta);
        }

        *pkt_byteoff += delta;
        pif_pkt_info_spec.pkt_pl_off += delta;
    }

    if (curr_len) {
        *pkt_byteoff -= curr_len;
        if (*pkt_byteoff < 0)
            return -PIF_DEPARSE_ERROR_BUFFER_OVERRUN;

        pif_pkt_write_header(*pkt_byteoff,
                             parrep,
                             PIF_PARREP_T1_OFF_LW,
                             curr_len);
    }

    return 0;
}

__forceinline extern int pif_deparse(__lmem uint32_t *parrep, PIF_PKT_INFO_TYPE struct pif_pkt_info *pktinfo)
{
    __gpr uint32_t pkt_byteoff = pif_pkt_info_spec.pkt_pl_off;
    __gpr uint32_t pkt_min_off;
    int ret;

    /* Packet minimum offset depends on packet destination - NBI/PCIe */
    pkt_min_off = PKTIO_MIN_NBI_TX_OFFSET; /* apply the nbi min to nfd too */
    ret = handle_tier_1(parrep, pktinfo, (uint32_t *)&pkt_byteoff, pkt_min_off);
    if (ret < 0)
        return ret;
    ret = handle_tier_0(parrep, pktinfo, (uint32_t *)&pkt_byteoff, pkt_min_off);
    if (ret < 0)
        return ret;

    pkt_byteoff = pktinfo->p_offset;

    /* If packet offset more than maximum allowed for NBI, return error */
    if (PKT_PORT_TYPE_of(pif_pkt_info_global.p_dst) == PKT_PTYPE_WIRE) {
        if (pkt_byteoff > PKTIO_MAX_NBI_TX_OFFSET) {
            uint32_t move_len = pkt_byteoff - PKTIO_MAX_NBI_TX_OFFSET;
            pif_pkt_move_pkt_up(pkt_byteoff, move_len);
            pkt_byteoff -= move_len;
            pktinfo->p_offset -= move_len;
            PIF_COUNT(OFFSET_TOO_LARGE_SHIFT);

        }
    } else {
#ifdef PKTIO_GRO_ENABLED
        if (pkt_byteoff > GRO_NFD_MAX_OFFSET) {
            uint32_t move_len = pkt_byteoff - GRO_NFD_MAX_OFFSET;
            pif_pkt_move_pkt_up(pkt_byteoff, move_len);
            pkt_byteoff -= move_len;
            pktinfo->p_offset -= move_len;
            PIF_COUNT(OFFSET_TOO_LARGE_SHIFT);

        }
#endif
    }
    if (pif_pkt_info_spec.trunc_len != 0 && pif_pkt_info_spec.trunc_len < pif_pkt_info_global.p_len)
        pif_pkt_info_global.p_len = pif_pkt_info_spec.trunc_len;

    return pkt_byteoff;
}
