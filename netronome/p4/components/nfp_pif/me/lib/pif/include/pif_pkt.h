/* Copyright (C) 2015,  Netronome Systems, Inc.  All rights reserved. */

#ifndef __PIF_PKT_H__
#define __PIF_PKT_H__

#include <pif_port_spec.h>
#include <pkt/pkt.h>


/*
 * Packet meta data mapping
 */

/* Place the thread local packet meta data in Next Neighbour registers */
#ifdef PIF_APP_REDUCED_THREADS
#define PKTIO_META_TYPE __gpr
#else
#define PKTIO_META_TYPE __nnr
#endif

#include <pktio/pktio.h>

/*
 * This maps to what is defined in pktio
 */
#define PIF_PKT_INFO_TYPE       PKTIO_META_TYPE
#define pif_pkt_info            pktio_meta
#define pif_pkt_info_global     pkt

/*
 * Application specific data for pif
 */
struct pif_pkt_specific {
    union {
        struct {
            uint32_t resv3:8;
            uint32_t trunc_len:14;
            uint32_t pkt_pl_off:10;         /* payload offset after parsing */

            uint32_t calc_fld_bmsk;   // bit mask for toggling incr calculated field updates
        };
        uint32_t _raw[2];
    };
};

extern __nnr struct pif_pkt_specific           pif_pkt_info_spec;

struct pif_pkt_combined {
    struct pif_pkt_specific specific;
    struct pif_pkt_info pkt;
};


/*
 * returned port is :
 * type: 2 .. pkt_port_type
 * port: 8
 */
#define PIF_PKT_P4PORT(_pktmd) \
    (PKT_PORT_TYPE_of((_pktmd)->p_src) == PKT_PTYPE_HOST) ? \
    PIF_PORT_SPEC(PKT_PORT_QUEUE_of((_pktmd)->p_src) | (PKT_PORT_SUBSYS_of((_pktmd)->p_src) << 6), PIF_PORT_SPEC_TYPE_HOST, 0) : \
    PIF_PORT_SPEC((PKT_PORT_QUEUE_of((_pktmd)->p_src)) | (PKT_PORT_SUBSYS_of((_pktmd)->p_src) << 6), PIF_PORT_SPEC_TYPE_PHYSICAL, 0)

/* get the nbi number out of the port spec */
#define PIF_PKT_PSPEC_NBI_of(pspec) ((PIF_PORT_SPEC_PORT_of(pspec) >> 6) & 0x1)
#define PIF_PKT_PSPEC_PORT_of(pspec) ((PIF_PORT_SPEC_PORT_of(pspec) >> 0) & 0x3f)

#define PIF_PKT_PSPEC_NBI_of(pspec) ((PIF_PORT_SPEC_PORT_of(pspec) >> 6) & 0x1)

/* get the nbi mac channel out of the port spec */
/* note we mulitply be 4 due to the channel->port fan in inside the mac
 * this fan in is configuration dependent, so care must be taken to get it right
 */
#define PIF_PKT_PSPEC_CHAN_of(port, q) (port * 64) | (q)


/** pif_pkt_write_header
 *
 * Insert hdrlen bytes by copying data at SOP to (SOP - hdrlen) for copylen
 *
 * @param pktoff         Offset at which to write header (in bytes)
 * @param lmem           Source to write to header (in local mem)
 * @param lmoff          Offset in local mem of header
 * @param hdrlen         Length to insert in bytes
 * @returns              Offset in packet after write (in bytes)
 */
unsigned int
pif_pkt_write_header(unsigned int pktoff,
                     __lmem uint32_t *lmem,
                     unsigned int lmoff,
                     unsigned int hdrlen);

/** pif_pkt_write_vheader
 *
 * Write variable length header to packet buffer.
 *
 * @param pktoff         Offset at which to write header (in bytes)
 * @param lmptr           Source to write to header (in local mem)
 * @param lmoff          Offset in local mem of header
 * @param lmsz         Length to insert in bytes
 * @param hdrlen         Length to insert in bytes
 * @param varptr         Length to insert in bytes
 * @param varoff         Length to insert in bytes
 */
void
pif_pkt_write_vheader(unsigned int pktoff,
                      __lmem uint32_t *lmptr,
                      unsigned int lmsz,
                      unsigned int hdrlen,
                      __cls uint32_t *varptr,
                      unsigned int varoff_b);


/** pif_pkt_free_space
 *
 * Remove hdrlen bytes by moving all packet data from pktoff up/left
 * This is done with bulk engine commands.
 *
 * @param pktoff         Offset at which to remove hdr (in bytes)
 * @param hdrlen         Length to delete in bytes
 *
 */
void
pif_pkt_free_space(unsigned int pktoff,
                   unsigned int hdrlen);

/** pif_pkt_move_pkt_up
 *
 * Move all packet data from pktoff up/left with removelen.
 * This is done with bulk engine commands.
 *
 * @param pktoff         Offset at which to remove hdr (in bytes)
 * @param removelen         Length to delete in bytes
 *
 */
void
pif_pkt_move_pkt_up(unsigned int pktoff,
                    unsigned int removelen);

/** pif_pkt_make_space
 *
 * Insert hdrlen bytes (space) by moving all packet from pktoff down/right.
 * This is done with bulk engine commands.
 *
 * @param pktoff         Offset at which to insert space (in bytes)
 * @param hdrlen         Length to insert in bytes
 *
 */
void
pif_pkt_make_space( unsigned int pktoff,
                   unsigned int hdrlen);



/** pif_pkt_insert_header
 *
 * Insert hdrlen bytes by copying data at SOP to (SOP - hdrlen) for copylen
 *
 * @param copylen        Length to copy in bytes (from start of packet)
 * @param hdrlen         Length to insert in bytes
 *
 */
void
pif_pkt_insert_header(unsigned int copylen,
                   unsigned int hdrlen);



/** pif_pkt_remove_header
 *
 * Remove hdrlen bytes by copying data at SOP to (SOP + hdrlen) for copylen
 *
 * @param copylen        Length to copy in bytes (from start of packet)
 * @param hdrlen         Length to remove in bytes
 *
 */
void
pif_pkt_remove_header(unsigned int copylen,
                   unsigned int hdrlen);

/** pif_pkt_dma_packet_down
 *
 * Insert space in a packet by shifting the packet down/right using DMA.
 * Use only CTM buffer if it is enough space, otherwise data in MU buffer
 * is also moved. If new length is more than MU buffer, allocate a larger buffer.
 * Error is returned if larger buffer is not available.
 *
 * @param insertlen         Length to insert in bytes, 64B aligned

 * @return 1 if success otherwise 0
 *
 */
int
pif_pkt_dma_packet_down(unsigned int insertlen);


#endif /* __PIF_PKT_H__ */
