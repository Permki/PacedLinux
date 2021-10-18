/*
/*
 * Copyright (C) 2014-2016,  Netronome Systems, Inc.  All rights reserved.
 *
 * @file        flow_cache_timeout_journal_c.h
 * @brief       Used by flow_cache_timeout.c when journalling (for troubleshooting) is enabled.
 */

#ifndef _FLOW_CACHE_TIMEOUT_JOURNAL_C_H_
#define _FLOW_CACHE_TIMEOUT_JOURNAL_C_H_

#include <nfp.h>
#include <stdint.h>
#include <stdlib.h>




    typedef enum TIMEOUT_JOURNAL_ERROR_CODES
    {
        JOURNAL_CHECK_HEAD_SHARED_FLAG = 0,
        JOURNAL_CHECK_NEXT_ENTRY_IDX_ZERO = 1,
        JOURNAL_CHECK_HEAD_ENTRY_IDX_ZERO = 2,
        JOURNAL_CHECK_NEXT_SHARED_FLAG = 3,
        JOURNAL_CHECK_NEXT_HEAD_ZERO = 4,
        JOURNAL_CLEAR_HASH_LOCK_CORRUPT = 5,
        JOURNAL_XLOCK_RELEASE = 6,
        JOURNAL_SHARED_LOCK_RELEASE = 7,
        JOURNAL_REMOVE_OVF_HASH_EMPTY = 8,
        JOURNAL_FREE_ENTRY_IDX_ZERO = 9,
        JOURNAL_NEW_HEAD_ENTRY_IDX_ZERO = 10,

        JOURNAL_ENTRY_PTR_OUT_OF_BOUNDS = 20,
        JOURNAL_PAYLOAD_PTR_OUT_OF_BOUNDS = 21,

        JOURNAL_LOCK_NOT_IN_CAM = 100
    };

    typedef union fc_timeout_journal_t
    {
        struct
        {
            enum TIMEOUT_JOURNAL_ERROR_CODES error_code : 8;
            uint32_t param_1                    : 8;
            uint32_t param_2                    : 8;
            uint32_t param_3                    : 8;
        };
        uint32_t value;
    } fc_timeout_journal_t;


#endif
