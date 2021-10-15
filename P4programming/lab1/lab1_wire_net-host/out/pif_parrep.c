/* Copyright (C) 2015-2016,  Netronome Systems, Inc.  All rights reserved. */

#include <nfp/me.h>
#include <hashmap.h>
#include "pif_common.h"

extern __forceinline
pif_parrep_extract(__lmem uint32_t *_pif_parrep, __mem __addr40 uint8_t *_pif_parrep_pktdata, unsigned int _pif_parrep_pktlen, __lmem uint32_t *_pif_parrep_fk_ptr, __gpr uint32_t *_pif_parrep_fk_len)
{
    __lmem struct pif_parrep_ctldata *_pif_ctldata = (__lmem struct pif_parrep_ctldata *)(_pif_parrep + PIF_PARREP_CTLDATA_OFF_LW);
    PIF_PKT_INFO_TYPE struct pif_pkt_info *pkt_info = &pif_pkt_info_global;
    unsigned int _pif_read_xbuf_off = 0;
    __gpr unsigned int _pif_parrep_pktoff = 0;
    __gpr int _pif_parrep_state = 0;
    __gpr int _pif_parrep_error = 0;
    int i;

    /* First zero the parsed representation control data fields */
    for (i = 0; i < PIF_PARREP_CTLDATA_LEN_LW; i++)
        _pif_parrep[PIF_PARREP_CTLDATA_OFF_LW + i] = 0;

__label_start:
    { /* implementation of parse node start */
        __lmem struct pif_parrep_ctldata *_pif_ctldata = (__lmem struct pif_parrep_ctldata *)(_pif_parrep + PIF_PARREP_CTLDATA_OFF_LW);
#ifdef PIF_DEBUG
        __debug_label("pif_parrep_state_start");
#endif

        __critical_path(); /* prioritize the fall through */
    } /* end of implementation of parse node start */
__label_eth_parse:
    { /* implementation of parse node eth_parse */
        __lmem struct pif_parrep_ctldata *_pif_ctldata = (__lmem struct pif_parrep_ctldata *)(_pif_parrep + PIF_PARREP_CTLDATA_OFF_LW);
        __lmem struct pif_header_eth *eth;
#ifdef PIF_DEBUG
        __debug_label("pif_parrep_state_eth_parse");
#endif

        eth = (__lmem struct pif_header_eth *) (_pif_parrep + PIF_PARREP_eth_OFF_LW);
        {
            /* extract(eth) */

            /* primitive body */
            {
                __gpr uint32_t _pif_parrep_hdrsz = 0;

                mem2lmem_copy(_pif_parrep_pktdata + _pif_read_xbuf_off, _pif_parrep,
                            PIF_PARREP_eth_OFF_LW,
                            PIF_PARREP_eth_LEN_LW);
                _pif_parrep_hdrsz = PIF_PARREP_eth_LEN_B;
                _pif_read_xbuf_off += _pif_parrep_hdrsz;

                if (_pif_parrep_pktoff + _pif_read_xbuf_off > _pif_parrep_pktlen) {
                    _pif_parrep_error = PIF_PARSE_ERROR_OUT_OF_PACKET;
                    _pif_parrep_state = PIF_PARREP_STATE_eth_parse;
                    goto _pif_parrep_label_error;
                }
                __critical_path();
                PIF_PARREP_SET_eth_VALID(_pif_ctldata);
                PIF_PARREP_SET_eth_ORIG_LEN(_pif_ctldata, _pif_parrep_hdrsz);
            }
        }
        goto _pif_parrep_label_exit;
    } /* end of implementation of parse node eth_parse */
_pif_parrep_label_exit:
    pif_pkt_info_spec.pkt_pl_off = _pif_parrep_pktoff + _pif_read_xbuf_off;
    PIF_PARREP_SET_VALID(_pif_ctldata);
#ifndef PIF_GLOBAL_FLOWCACHE_DISABLED
    _pif_parrep_fk_ptr[(*_pif_parrep_fk_len) + 0] = _pif_parrep[PIF_PARREP_CTLDATA_OFF_LW + 0] & 0x80000000;
    (*_pif_parrep_fk_len) += PIF_PARREP_CTLDATA_LEN_LW;
#endif /* !PIF_GLOBAL_FLOWCACHE_DISABLED */
    return 0;
_pif_parrep_label_error:
    pif_pkt_info_spec.pkt_pl_off = _pif_parrep_pktoff + _pif_read_xbuf_off;
    pif_global_set_stdmd_parser_error_location(_pif_parrep, _pif_parrep_state);
    return -_pif_parrep_error;
}

extern __forceinline int
pif_parrep_exceptions(__lmem uint32_t *_pif_parrep, int _pif_pe_error, __lmem uint32_t *_pif_parrep_fk_ptr, __gpr uint32_t *_pif_parrep_fk_len)
{
    return -1; /* no exceptions, always fail */
}
