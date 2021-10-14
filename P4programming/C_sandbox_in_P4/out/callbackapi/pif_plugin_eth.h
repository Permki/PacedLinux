/* Copyright (C) 2015-2016,  Netronome Systems, Inc.  All rights reserved. */

#ifndef __PIF_PLUGIN_ETH_H__
#define __PIF_PLUGIN_ETH_H__

/* This file is generated, edit at your peril */

/*
 * Header type definition
 */

/* eth (14B) */
struct pif_plugin_eth {
    /* dst [32;16] */
    unsigned int dst:32;
    /* dst [16;0] */
    unsigned int __dst_1:16;
    /* src [16;32] */
    unsigned int src:16;
    /* src [32;0] */
    unsigned int __src_1:32;
    unsigned int etype:16;
};

/* eth field accessor macros */
#define PIF_HEADER_GET_eth___dst___0(_hdr_p) ((((_hdr_p)->dst & 0xffff) << 16) | ((_hdr_p)->__dst_1)) /* eth.dst [32;0] */

#define PIF_HEADER_SET_eth___dst___0(_hdr_p, _val) \
    do { \
        (_hdr_p)->dst &= (unsigned)(0xffff0000); \
        (_hdr_p)->dst |= (unsigned)((((_val) >> 16) & 0xffff)); \
        (_hdr_p)->__dst_1 = (unsigned)(((_val))); \
    } while (0) /* eth.dst[32;0] */

#define PIF_HEADER_GET_eth___dst___1(_hdr_p) ((((_hdr_p)->dst >> 16) & 0xffff)) /* eth.dst [16;32] */

#define PIF_HEADER_SET_eth___dst___1(_hdr_p, _val) \
    do { \
        (_hdr_p)->dst &= (unsigned)(0xffff); \
        (_hdr_p)->dst |= (unsigned)((((_val) & 0xffff) << 16)); \
    } while (0) /* eth.dst[16;32] */

#define PIF_HEADER_GET_eth___src___0(_hdr_p) (((_hdr_p)->__src_1)) /* eth.src [32;0] */

#define PIF_HEADER_SET_eth___src___0(_hdr_p, _val) \
    do { \
        (_hdr_p)->__src_1 = (unsigned)(((_val))); \
    } while (0) /* eth.src[32;0] */

#define PIF_HEADER_GET_eth___src___1(_hdr_p) (((_hdr_p)->src)) /* eth.src [16;32] */

#define PIF_HEADER_SET_eth___src___1(_hdr_p, _val) \
    do { \
        (_hdr_p)->src = (unsigned)(((_val))); \
    } while (0) /* eth.src[16;32] */

#define PIF_HEADER_GET_eth___etype(_hdr_p) (((_hdr_p)->etype)) /* eth.etype [16;0] */

#define PIF_HEADER_SET_eth___etype(_hdr_p, _val) \
    do { \
        (_hdr_p)->etype = (unsigned)(((_val))); \
    } while (0) /* eth.etype[16;0] */



#define PIF_PLUGIN_eth_T __lmem struct pif_plugin_eth

/*
 * Access function prototypes
 */

int pif_plugin_hdr_eth_present(EXTRACTED_HEADERS_T *extracted_headers);

PIF_PLUGIN_eth_T *pif_plugin_hdr_get_eth(EXTRACTED_HEADERS_T *extracted_headers);

PIF_PLUGIN_eth_T *pif_plugin_hdr_readonly_get_eth(EXTRACTED_HEADERS_T *extracted_headers);

int pif_plugin_hdr_eth_add(EXTRACTED_HEADERS_T *extracted_headers);

int pif_plugin_hdr_eth_remove(EXTRACTED_HEADERS_T *extracted_headers);






/*
 * Access function implementations
 */

#include "pif_parrep.h"

__forceinline int pif_plugin_hdr_eth_present(EXTRACTED_HEADERS_T *extracted_headers)
{
    __lmem struct pif_parrep_ctldata *_ctl = (__lmem struct pif_parrep_ctldata *)extracted_headers;
    return PIF_PARREP_eth_VALID(_ctl);
}

__forceinline PIF_PLUGIN_eth_T *pif_plugin_hdr_get_eth(EXTRACTED_HEADERS_T *extracted_headers)
{
    __lmem struct pif_parrep_ctldata *_ctl = (__lmem struct pif_parrep_ctldata *)extracted_headers;
    PIF_PARREP_SET_eth_DIRTY(_ctl);
    return (PIF_PLUGIN_eth_T *)(((__lmem uint32_t *)extracted_headers) + PIF_PARREP_eth_OFF_LW);
}

__forceinline PIF_PLUGIN_eth_T *pif_plugin_hdr_readonly_get_eth(EXTRACTED_HEADERS_T *extracted_headers)
{
    __lmem struct pif_parrep_ctldata *_ctl = (__lmem struct pif_parrep_ctldata *)extracted_headers;
    return (PIF_PLUGIN_eth_T *)(((__lmem uint32_t *)extracted_headers) + PIF_PARREP_eth_OFF_LW);
}

__forceinline int pif_plugin_hdr_eth_add(EXTRACTED_HEADERS_T *extracted_headers)
{
    return -1; /* this header is not addable in the P4 design */
}

__forceinline int pif_plugin_hdr_eth_remove(EXTRACTED_HEADERS_T *extracted_headers)
{
    return -1; /* this header is not removable in the P4 design */
}

#endif /* __PIF_PLUGIN_ETH_H__ */
