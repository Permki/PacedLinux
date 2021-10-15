/* Copyright (C) 2015-2016,  Netronome Systems, Inc.  All rights reserved. */

#ifndef __PIF_PARREP_H__
#define __PIF_PARREP_H__

/* Generated C source defining layout of parsed representation */
/* Warning: your edits to this file may be lost */

/*
 * Parsed representation control data
 */
struct pif_parrep_ctldata {
    unsigned int valid:1;
    unsigned int t0_valid:1;
    unsigned int t0_dirty:1;
    unsigned int t0_orig_len:1;
};

#define PIF_PARREP_CTLDATA_OFF_LW 0
#define PIF_PARREP_CTLDATA_LEN_LW 1

/*
 * Parsed representation layout
 */

/* Parsed represention tier types */

/* Parse state values */
#define PIF_PARREP_STATE_exit -1
#define PIF_PARREP_STATE_encap_tbl (PIF_PARREP_STATE_exit)
#define PIF_PARREP_STATE_eth_parse 1
#define PIF_PARREP_STATE_start 0
#define PIF_PARREP_MAX_STATE 1

/* Tier 0 */
#define PIF_PARREP_T0_OFF_LW 1
#define PIF_PARREP_T0_LEN_LW 4
#define PIF_PARREP_eth_OFF_LW (PIF_PARREP_T0_OFF_LW)
#define PIF_PARREP_eth_LEN_LW 4
#define PIF_PARREP_eth_LEN_B 14

/*
 * Metadata
 */

#define PIF_PARREP_standard_metadata_OFF_LW 5
#define PIF_PARREP_standard_metadata_LEN_LW 4

#define PIF_PARREP_LEN_LW 9

/* Parsing branches to a constant control entry point */
#define PIF_PARREP_NO_VARIABLE_EXIT

/* Control data macros */
#define PIF_PARREP_VALID(_ctl) (_ctl->valid)
#define PIF_PARREP_SET_VALID(_ctl) \
    do { _ctl->valid = 1; } while (0)

/* Tier 0 */
#define PIF_PARREP_T0_TYPE(ctl) ( ((ctl)->t0_type))
#define PIF_PARREP_T0_VALID(ctl) ( ((ctl)->t0_valid))
#define PIF_PARREP_eth_VALID(ctl) ( ((ctl)->t0_valid) )
#define PIF_PARREP_SET_eth_VALID(ctl) \
    do { \
        (ctl)->t0_valid = 1; \
    } while(0);
#define PIF_PARREP_CLEAR_eth_VALID(ctl) \
    do { \
        (ctl)->t0_valid = 0; \
    } while(0);

#define PIF_PARREP_eth_DIRTY(_ctl) ((_ctl)->t0_dirty)
#define PIF_PARREP_T0_DIRTY(_ctl) ((_ctl)->t0_dirty)
#define PIF_PARREP_CLEAR_T0_DIRTY(_ctl)     do { \
        (_ctl)->t0_dirty = 0; \
    } while(0);
#define PIF_PARREP_SET_T0_DIRTY(_ctl)     do { \
        (_ctl)->t0_dirty = 0; \
    } while(0);
#define PIF_PARREP_SET_eth_DIRTY(_ctl) \
    do { \
        (_ctl)->t0_dirty = 1; \
    } while(0);

#define PIF_PARREP_T0_ORIG_LEN(ctl) (((ctl)->t0_orig_len) ? PIF_PARREP_eth_LEN_B : 0)
#define PIF_PARREP_SET_T0_ORIG_LEN(ctl, len) \
    do { \
        (ctl)->t0_orig_len = (len == 0 ? 0 : 1); \
    } while(0);
#define PIF_PARREP_eth_ORIG_LEN(ctl) (((ctl)->t0_orig_len) ? PIF_PARREP_eth_LEN_B : 0)
#define PIF_PARREP_CLEAR_eth_ORIG_LEN(ctl) \
    do { \
        (ctl)->t0_orig_len = 0; \
    } while(0);
#define PIF_PARREP_SET_eth_ORIG_LEN(ctl, len) \
    do { \
        (ctl)->t0_orig_len = 1; \
    } while(0);



void pif_value_set_scan_configs();

#endif /* __PIF_PARREP_H__ */
