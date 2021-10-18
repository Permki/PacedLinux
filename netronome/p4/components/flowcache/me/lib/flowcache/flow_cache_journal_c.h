/*
 * Copyright (C) 2014-2016,  Netronome Systems, Inc.  All rights reserved.
 *
 * @file        flow_cache_journal_c.h
 * @brief       Used by flow_cache.c when journalling (for troubleshooting) is enabled.
 */

#ifndef _FLOW_CACHE_JOURNAL_C_H_
#define _FLOW_CACHE_JOURNAL_C_H_

#include <nfp.h>
#include <stdint.h>
#include <stdlib.h>



typedef enum FC_JOURNAL_ERROR_CODES
{
    JOURNAL_XLOCK_RELEASE = 0xa0,
    JOURNAL_SHARED_LOCK_RELEASE,
    JOURNAL_LOCK_NOT_IN_CAM,
    JOURNAL_LOCK_CORRUPT,

    JOURNAL_PAYLOAD_OUT_OF_BOUNDS = 0xb0,
    JOURNAL_CAM_ENTRY_OUT_OF_BOUNDS,
    JOURNAL_RING_ENTRY_OUT_OF_BOUND
};

typedef union fc_journal_t
{
    struct
    {
        enum FC_JOURNAL_ERROR_CODES error_code      : 8;
        uint32_t param_1                            : 8;
        uint32_t param_2                            : 8;
        uint32_t param_3                            : 8;
    };
    uint32_t value;
} fc_journal_t;


#endif
