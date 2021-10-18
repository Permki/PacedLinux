/* Copyright (C) 2015,  Netronome Systems, Inc.  All rights reserved. */

#ifndef __PIF_LOOKUP_H__
#define __PIF_LOOKUP_H__

#include <nfp.h>
#include <nfp/mem_bulk.h>

#include <dcfl_ctrl.h>

/* control flow return this bit set in a negative number
 * to distinguish a drop from an error
 */
#define PIF_LOOKUP_ERROR_BIT 16

enum {
    PIF_LOOKUP_ERROR_SUCCESS = 0,
    PIF_LOOKUP_ERROR_BUFFER_OVERFLOW,
    PIF_LOOKUP_ERROR_LAST,
};

/* maximum action buffer size */
#define PIF_LOOKUP_BUFFER_MAX ((256 - 4) / 4)

/* Invalid rule identifier to indicate a miss */
#define PIF_LOOKUP_MISS_RULE           (1 << 16)

/* The size of the DCFL table in bytes
 * 20MB is the default; this can be overridden
 * from a build define
 */
#ifndef DCFL_TABLE_SIZE
#define DCFL_TABLE_SIZE (20 * 1024 * 1024)
#endif

#define PIF_ACTIONDATA_FLAG_VALID      (1 << 0)
#define PIF_ACTIONDATA_FLAG_HASDEFAULT (1 << 1)

#ifdef DCFL_SPLIT_ARCH
#define DCFL_KEYQ_NUM 19
#define PIF_LOOKUP_KEY_SIZE_LW      PIF_PARREP_LEN_LW

void pif_lookup_dcfl_init();
#endif

struct pif_actiondata_table_desc {
    __emem uint32_t *table_address;

    uint32_t rule_count;

    uint16_t reserved;
    uint8_t record_shift;
    uint8_t flags;
};

struct pif_lookup_result {
    signed int action_id;
    signed int action_len;
};

struct pif_lookup_result
pif_lookup(unsigned int tableno,
           __lmem uint32_t *parrep,
           __mem __addr40 uint32_t *action_buffer,
           uint32_t action_buffer_offset);

#endif /* __PIF_LOOKUP_H__ */
