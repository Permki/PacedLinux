/* Copyright (C) 2015-2016,  Netronome Systems, Inc.  All rights reserved. */

#include <pif_common.h>

/* Table related defines */

/* Table 0 : encap_tbl */
#define PIF_TABLE_ACTIONDATA_SIZE_0 16
#define PIF_TABLE_MAX_ENTRIES_0 1
#define PIF_TABLE_SHIFT_0 4

/* DCFL related table storage */
__export __emem __align256K uint32_t dcfl_ct_0[DCFL_TABLE_SIZE / 4];

/* PIF related table storage */
__export __emem uint32_t pif_actiondata_table_0_encap_tbl[2 * PIF_TABLE_MAX_ENTRIES_0 * (PIF_TABLE_ACTIONDATA_SIZE_0 / 4)];

__export __emem struct pif_actiondata_table_desc pif_actiondata_table_descs[PIF_TABLE_COUNT] = {
        {
            /* .table_address = */ 0,
            /* .rule_count = */ 0,
            /* .reserved = */ 0,
            /* .record_shift = */  PIF_TABLE_SHIFT_0,
            /* .flags = */ 0,
        },
    };
