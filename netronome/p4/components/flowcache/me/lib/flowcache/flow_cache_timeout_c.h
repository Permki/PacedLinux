/*
 * Copyright (C) 2014-2016,  Netronome Systems, Inc.  All rights reserved.
 *
 * @file        flow_cache_timeout_c.h
 * @brief       Definitions, macros and protypes for flow_cache_timeout.c
 */


#ifndef _FLOW_CACHE_TIMEOUT_C_H_
#define _FLOW_CACHE_TIMEOUT_C_H_

/**
 * Defines
 */
#ifndef __fc_addr40
#define __fc_addr40                     __declspec(mem addr40)
#endif

 #ifndef MU_LOCALITY_HIGH
    #define MU_LOCALITY_HIGH                0
#endif
#ifndef MU_LOCALITY_LOW
    #define MU_LOCALITY_LOW                 1
#endif
#ifndef MU_LOCALITY_DIRECT_ACCESS
    #define MU_LOCALITY_DIRECT_ACCESS       2
#endif
#ifndef MU_LOCALITY_DISCARD_AFTER_READ
    #define MU_LOCALITY_DISCARD_AFTER_READ  3
#endif





/**
 * Macro's
 */

/**
 * Changes the locality bits
 */
#define fc_change_mu_locality(io_upper8, NEW_MU_LOCALITY) \
    io_upper8 = io_upper8 & ~(3 << 30);                   \
    io_upper8 = io_upper8 | (NEW_MU_LOCALITY## << 30);




/**
 * Function prototypes
 */


/**
 * fc_timeout
 * For each entry CAM bucket, verify if entry timed out and delete the entry.
 * If the timeout is 0, the entry is not invalidated.
 *
 */
static void fc_timeout(__gpr uint32_t bucket_mu);

#endif