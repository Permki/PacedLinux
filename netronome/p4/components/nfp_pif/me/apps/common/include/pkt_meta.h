/* Copyright (C) 2015,  Netronome Systems, Inc.  All rights reserved.
 *
 * This header contians information for reading and manipulating the managed-C
 * packet metadata.
 *
 */

#ifndef __PKT_META_H__
#define __PKT_META_H__

#include <pkt/pkt.h>


/*
 * Packet meta data mapping
 */

/* Place the thread local packet meta data in Next Neighbour registers */
#define PKTIO_META_TYPE __nnr

#include <pktio/pktio.h>

#define PIF_PKT_INFO_TYPE       PKTIO_META_TYPE
#define pif_pkt_info            pktio_meta
#define pif_pkt_info_global     pkt

#define PKT_META_TYPE           PKTIO_META_TYPE
#define pkt_meta                pktio_meta

/* extern - packet metadata is declared elsewhere and is global */
extern PKTIO_META_TYPE struct pktio_meta pif_pkt_info_global;


/* return codes for mc_main() */

#define MC_DROP    -1
#define MC_FORWARD 0


/* source/destination of the packet */
/* physical port */
#define PKT_META_SRC_NBI 0
#define PKT_META_DST_NBI PKT_META_SRC_NBI
/* from host/pcie */
#define PKT_META_SRC_PCIE 1
#define PKT_META_DST_PCIE PKT_META_SRC_PCIE





/* helper macro for declaring pointer to packet metadata */
#define PKT_META_PTR (&pif_pkt_info_global)

/* function for setting how packet are forwarded */
__forceinline void pkt_meta_forward_nbi(PKT_META_TYPE struct pktio_meta *pmeta,
                                        int nbi, int port)
{
     /* there is a 1:4 mapping of port # */
    pmeta->p_dst = (PKT_META_SRC_NBI << 13) | (nbi << 10) | (port << 2);
}

__forceinline void pkt_meta_forward_pcie(PKT_META_TYPE struct pktio_meta *pmeta,
                                        int vf)
{
    pmeta->p_dst = (PKT_META_SRC_PCIE << 13) | (0 << 10) | vf;
}


#endif /* __PKT_META_H__ */
