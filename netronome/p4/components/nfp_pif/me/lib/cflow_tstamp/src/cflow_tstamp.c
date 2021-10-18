/*
 * Copyright (C) 2014-2017,  Netronome Systems, Inc.  All rights reserved.
 *
 * @file   cflow_time_stamp.c
 *
 * @brief  Performance analysis of code execution by monitoring
 *         the duration of various code segments in the piff_app_nfd.c file.
 *
 *         Include CFLOW_TSTAMP in the build options to enable the
 *         timestamp journalling
 *
 */

/* define to enable code, used to remove while keeping source file in project */
#ifdef CFLOW_TSTAMP

#include <nfp.h>
#include <me.h>
#include <nfp/mem_ring.h>
#include "cflow_tstamp.h"

/* internal function format */
#ifdef CFLOW_TSTAMP_USE_INTRINSIC
#define CFLOW_TSTAMP_FNTYPE_INT __intrinsic
#else
#define CFLOW_TSTAMP_FNTYPE_INT extern __forceinline
#endif

/***********************************
 * Definitions                     *
 ***********************************/

union tstamp_journal {
    struct {
        unsigned int sequence_no : 16;
        /* the user defined monitoring point, 0 - 15 */
        unsigned int tstamp_pos  : 4;
        unsigned int tstamp      : 12;
    };
    unsigned int value;
};

union code_path_journal {
    struct {
        unsigned int sequence_no : 16;
        unsigned int code_path   : 16;
    };
    unsigned int value;
};

/***********************************
 * Declarations                    *
 ***********************************/

/* User of cflow tstamp library needs to declare this variable */
__gpr unsigned int current_code_path;

union tstamp_journal tstamp_data;
union code_path_journal code_path_data;

MEM_RING_INIT_MU(cflow_code_path_ring,
                 CFLOW_TSTAMP_CPATH_JOURNAL_SIZE_LW,
                 emem0);
MEM_RING_INIT_MU(cflow_timestamp_ring,
                 CFLOW_TSTAMP_TSTAMP_JOURNAL_SIZE_LW,
                 emem0);

/***********************************
 * Code path journal               *
 ***********************************/

/* Update the current code path string:
 * The timestamp position is used to flip a corresponding bit in the
 * current_code_path string to indicate which positions were used / hit
 * by the packet
 */
CFLOW_TSTAMP_FNTYPE_INT void
update_code_path(unsigned int stamp_pos)
{
    current_code_path |= 1 << stamp_pos;
}

/* Log the code path to the journal */
CFLOW_TSTAMP_FNTYPE_INT void
log_code_path(unsigned int sequence_no)
{
   code_path_data.value = 0x00;
   code_path_data.sequence_no = sequence_no;
   code_path_data.code_path = current_code_path;
   /* Log the data to the ring */
   JDBG(cflow_code_path_ring, code_path_data.value);
}

/***********************************
 * Primary timestamp journal       *
 ***********************************/

/* Write the timestamp to the primary journal */
CFLOW_TSTAMP_FNTYPE_INT void
cflow_tstamp(unsigned int tstamp_pos, unsigned int sequence_no)
{
    /* initialize the string */
    tstamp_data.value = 0x00;
    tstamp_data.sequence_no = sequence_no;
    tstamp_data.tstamp_pos = tstamp_pos;
    /* Return the current 64bit ME timestamp counter value.*/
    tstamp_data.tstamp = me_tsc_read();
    /* Log the data to the ring */
    JDBG(cflow_timestamp_ring, tstamp_data.value);
    /* Update the code path string with the current timestamp position */
    update_code_path(tstamp_pos);
}

/* Fixed timestamp locations */
CFLOW_TSTAMP_FNTYPE_INT void
cflow_tstamp_start(unsigned int sequence_no)
{
    /* Initialise the local variable to contain the code path */
    current_code_path = 0;
    cflow_tstamp(0, sequence_no);
}

CFLOW_TSTAMP_FNTYPE_INT void
cflow_tstamp_end(unsigned int tstamp_pos, unsigned int sequence_no)
{
    cflow_tstamp(tstamp_pos, sequence_no);
    log_code_path(sequence_no);
}

#endif
