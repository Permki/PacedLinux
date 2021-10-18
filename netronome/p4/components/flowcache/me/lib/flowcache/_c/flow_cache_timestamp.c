/*
 * Copyright (C) 2014-2016,  Netronome Systems, Inc.  All rights reserved.
 *
 * @file        flow_cache_timestamp.c
 * @brief       Get a timestamp depending on platform.
 */

#ifndef _FLOW_CACHE_TIMESTAMP_C_
#define _FLOW_CACHE_TIMESTAMP_C_

#include <nfp.h>
#include <stdint.h>
#include <nfp6000/nfp_me.h>
#include "flow_cache_global_c.h"
#include "flow_cache_timestamp_c.h"



/*
 * Get timestamp for entries.
 * Read timestamp_low first because whenever for internal reads a TimestampLow read instruction
 * occurs, the hardware automatically makes a copy of the TimestampHigh CSR into a TimestampHigh "shadow"
 * register; a subsequent read instruction of the TimestampHigh CSR is in effect a read of the "shadow" register.
 * See PRM.
 */
__forceinline extern __gpr uint16_t fc_get_timestamp(void)
{
    __gpr uint32_t      timestamp_hi,
                        timestamp_low;

    timestamp_low = local_csr_read(local_csr_timestamp_low);
    timestamp_hi = local_csr_read(local_csr_timestamp_high);

    return dbl_shr(timestamp_hi, timestamp_low, _FC_TS_SHIFT);
}


#endif
