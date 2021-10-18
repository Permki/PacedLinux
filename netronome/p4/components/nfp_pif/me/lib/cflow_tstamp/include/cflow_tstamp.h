/*
 * Copyright (C) 2016-2017,  Netronome Systems, Inc.  All rights reserved.
 *
 * @file        cflow_tstamp.h
 * @brief       Timestamp various locations and write to memory ring journal
 */

#ifndef __CFLOW_TSTAMP_H__
#define __CFLOW_TSTAMP_H__

#ifdef CFLOW_TSTAMP_USE_INTRINSIC
#define CFLOW_TSTAMP_FNTYPE __intrinsic
#else
#define CFLOW_TSTAMP_FNTYPE
#endif

/* Journal size definitions
 * The default journal sizes can be changed to any desired power of 2 value
 */
#ifndef CFLOW_TSTAMP_TSTAMP_JOURNAL_SIZE_LW
#define CFLOW_TSTAMP_TSTAMP_JOURNAL_SIZE_LW 2097152 /* 2^21 */
#endif

#ifndef CFLOW_TSTAMP_CPATH_JOURNAL_SIZE_LW
#define CFLOW_TSTAMP_CPATH_JOURNAL_SIZE_LW  262144  /* 2^18 */
#endif

CFLOW_TSTAMP_FNTYPE void
cflow_tstamp_start(unsigned int sequence_no);

CFLOW_TSTAMP_FNTYPE void
cflow_tstamp(unsigned int tstamp_pos, unsigned int sequence_no);

CFLOW_TSTAMP_FNTYPE void
cflow_tstamp_end(unsigned int tstamp_pos, unsigned int sequence_no);

#endif /* __CFLOW_TSTAMP_H__ */
