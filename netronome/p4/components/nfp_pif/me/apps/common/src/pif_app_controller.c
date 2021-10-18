
/* Copyright (C) 2017,  Netronome Systems, Inc.  All rights reserved. */

#include <pkt/pkt.h>
#include <nfp/mem_pe.h>
#include "pif_common.h"
#include "pif_app_controller.h"

__forceinline extern int decap_controller_pkt(PIF_PKT_INFO_TYPE struct pif_pkt_info *pktinfo)
{
    /* Decapsulate header without looking at info, do we need to ? TODO */
    if (pktinfo->p_src == CPU_IN_PORT) {
        if (pktinfo->p_orig_len > sizeof(struct pif_ctrl_header)) {
            pktinfo->p_orig_len -= sizeof(struct pif_ctrl_header);
            pktinfo->p_offset += sizeof(struct pif_ctrl_header);
        } else return -1;
    }

    return 0;
}

__forceinline extern void encap_controller_pkt(PIF_PKT_INFO_TYPE struct pif_pkt_info *pktinfo)
{
    __xwrite struct pif_ctrl_header ctrl_hdr;
    __mem __addr40 uint8_t *pkt_buf;
    SIGNAL sig;

    if (pktinfo->p_dst == CPU_OUT_PORT) {
        pktinfo->p_orig_len += sizeof(struct pif_ctrl_header);
        pktinfo->p_offset -= sizeof(struct pif_ctrl_header);
        pkt_buf = (__mem __addr40 uint8_t *)pkt_ctm_ptr40(0, pktinfo->p_pnum,
                                                    pktinfo->p_offset);
        /* TODO change to correct data */
        ctrl_hdr.placeholder_1 = 0x12345678;
        ctrl_hdr.placeholder_2 = 0x90123456;
        __mem_write8(&ctrl_hdr.__raw[0], pkt_buf,
                     sizeof(struct pif_ctrl_header),
                     sizeof(struct pif_ctrl_header), ctx_swap, &sig);
    }
    return;
}