/*
 * Copyright (C) 2005-2013 Netronome Systems, Inc.  All rights reserved.
 *
 * File:        lmqueue.h
 */

#ifndef _LMQUEUE_H_
#define _LMQUEUE_H_

/*
    Use LMQ_KEEP_HEAD/TAIL in queue flags to intruct the queue to keep copies
    of the current head/tail variables.

    Use LMQ_NO_COUNTER for queues where external logic and state determines
    whether the queue full/empty.
*/
#define LMQ_KEEP_HEAD           (1 << 0)    /* Keep copy of "current" head variable */
#define LMQ_KEEP_TAIL           (1 << 1)    /* Keep copy of "current" tail variable */
#define LMQ_NO_COUNTER          (1 << 2)    /* Do not keep counter variable */
#define LMQ_USE_BODY            (1 << 3)    /* "Dual" headed queue, using (head, body, tail) */
#define LMQ_ZERO                (1 << 4)    /* Zero queue contents at initialization (clobbers ACTIVE_LM_ADDR_0) */


#endif /* _LMQUEUE_H_ */
