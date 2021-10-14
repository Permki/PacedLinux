/* Copyright (C) 2015-2016,  Netronome Systems, Inc.  All rights reserved. */

#ifndef __PIF_PLUGIN_XLAN_H__
#define __PIF_PLUGIN_XLAN_H__

/* This file is generated, edit at your peril */

/*
 * Header type definition
 */

/* xlan (6B) */
struct pif_plugin_xlan {
    unsigned int tag:32;
    unsigned int etype:16;
};

/* xlan field accessor macros */
#define PIF_HEADER_GET_xlan___tag(_hdr_p) (((_hdr_p)->tag)) /* xlan.tag [32;0] */

#define PIF_HEADER_SET_xlan___tag(_hdr_p, _val) \
    do { \
        (_hdr_p)->tag = (unsigned)(((_val))); \
    } while (0) /* xlan.tag[32;0] */

#define PIF_HEADER_GET_xlan___etype(_hdr_p) (((_hdr_p)->etype)) /* xlan.etype [16;0] */

#define PIF_HEADER_SET_xlan___etype(_hdr_p, _val) \
    do { \
        (_hdr_p)->etype = (unsigned)(((_val))); \
    } while (0) /* xlan.etype[16;0] */



#define PIF_PLUGIN_xlan_T __lmem struct pif_plugin_xlan

/*
 * Access function prototypes
 */

int pif_plugin_hdr_xlan_present(EXTRACTED_HEADERS_T *extracted_headers);

PIF_PLUGIN_xlan_T *pif_plugin_hdr_get_xlan(EXTRACTED_HEADERS_T *extracted_headers);

PIF_PLUGIN_xlan_T *pif_plugin_hdr_readonly_get_xlan(EXTRACTED_HEADERS_T *extracted_headers);

int pif_plugin_hdr_xlan_add(EXTRACTED_HEADERS_T *extracted_headers);

int pif_plugin_hdr_xlan_remove(EXTRACTED_HEADERS_T *extracted_headers);






/*
 * Access function implementations
 */

#include "pif_parrep.h"

__forceinline int pif_plugin_hdr_xlan_present(EXTRACTED_HEADERS_T *extracted_headers)
{
    __lmem struct pif_parrep_ctldata *_ctl = (__lmem struct pif_parrep_ctldata *)extracted_headers;
    return PIF_PARREP_xlan_VALID(_ctl);
}

__forceinline PIF_PLUGIN_xlan_T *pif_plugin_hdr_get_xlan(EXTRACTED_HEADERS_T *extracted_headers)
{
    __lmem struct pif_parrep_ctldata *_ctl = (__lmem struct pif_parrep_ctldata *)extracted_headers;
    PIF_PARREP_SET_xlan_DIRTY(_ctl);
    return (PIF_PLUGIN_xlan_T *)(((__lmem uint32_t *)extracted_headers) + PIF_PARREP_xlan_OFF_LW);
}

__forceinline PIF_PLUGIN_xlan_T *pif_plugin_hdr_readonly_get_xlan(EXTRACTED_HEADERS_T *extracted_headers)
{
    __lmem struct pif_parrep_ctldata *_ctl = (__lmem struct pif_parrep_ctldata *)extracted_headers;
    return (PIF_PLUGIN_xlan_T *)(((__lmem uint32_t *)extracted_headers) + PIF_PARREP_xlan_OFF_LW);
}

__forceinline int pif_plugin_hdr_xlan_add(EXTRACTED_HEADERS_T *extracted_headers)
{
    __lmem struct pif_parrep_ctldata *_ctl = (__lmem struct pif_parrep_ctldata *)extracted_headers;
    if (PIF_PARREP_T1_VALID(_ctl))
        return -1; /* either already present or incompatible header combination */
    PIF_PARREP_SET_xlan_VALID(_ctl);
    return 0;
}

__forceinline int pif_plugin_hdr_xlan_remove(EXTRACTED_HEADERS_T *extracted_headers)
{
    return -1; /* this header is not removable in the P4 design */
}

#endif /* __PIF_PLUGIN_XLAN_H__ */
