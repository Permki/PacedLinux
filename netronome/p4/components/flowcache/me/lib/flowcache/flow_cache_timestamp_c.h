/*
 * Copyright (C) 2014-2016,  Netronome Systems, Inc.  All rights reserved.
 *
 * @file        flow_cache_timestamp_c.h
 * @brief       Definitions for flow_cache_timestamp.c
 */

#ifndef _FLOW_CACHE_TIMESTAMP_C_H_
#define _FLOW_CACHE_TIMESTAMP_C_H_


/*
* Number of cycles counted as one (1) when reading timestamp.
* It counts up by one (1) every sixteen (16) cycles
*/
#define FC_CYCLES_PER_TIMESTAMP_UNIT    16

#if (SIMULATION)

    #define _FC_TS_SHIFT                    1
    #define FC_CYCLES_PER_SECOND            19          // thumbsuck
    #define FC_TIMEOUT_MULTIPLIER           1

#else

    /*
    * Depending on the cycles per second, we set the amount the timestamp should be shifted
    * by. Only 16 bits are allowed for timestamp and timeout calculations.
    *
    * For 1.2 GHz a right shift of 20 gives a range of 0 to 58 seconds.
    */
    #define _FC_TS_SHIFT                    20

    /*
    * Defines the cycles per second depending on speed of the hardware.
    */
    #define FC_CYCLES_PER_SECOND            (ME_MHZ * 1000000)

    /*
    * Value to multiply timeout value with
    * Formula = (1 / (FC_CYCLES_PER_TIMESTAMP_UNIT/(FC_CYCLES_PER_SECOND >> 20)))
    * .. should be 47 for Hydrogen and 71 for SF/CDP
    */
    #define FC_TIMEOUT_MULTIPLIER           ((FC_CYCLES_PER_SECOND >> _FC_TS_SHIFT)/FC_CYCLES_PER_TIMESTAMP_UNIT)

#endif



#define FC_TIMESTAMP_UNITS_SECOND       (FC_CYCLES_PER_SECOND/FC_CYCLES_PER_TIMESTAMP_UNIT)


/*
 * Convert the timeout value to same units as in timestamps.
 * We want to compare/add this to the timestamp which is in cycles/sec.
 * Also drop the lower _FC_TS_SHIFT bits
 */
#define FC_CONVERT_TIMEOUT(timeout)    ((timeout) * FC_TIMEOUT_MULTIPLIER)




/** fc_get_timestamp
 *
 *  Read 64-bit timestamp from local CSR (1 for each 16 cycles)
 *  and discard the lower _FC_TS_SHIFT bits.
 *  The in_timeout is added to this and the result is returned in 16-bit timestamp.
 *
 *
 * @return                      16-bit timestamp
 */
__gpr uint16_t fc_get_timestamp(void);

#endif
