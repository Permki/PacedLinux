 /*
 * Copyright (C) 2014-2015 Netronome Systems, Inc.  All rights reserved.
 *
 * File:        system_init_c.h
 *
 */

#ifndef _SYSTEM_INIT_C_H_
#define _SYSTEM_INIT_C_H_

#define ME_INIT_SIGNAL 15

void system_init_wait_done(void);

void system_init_done(void);

void system_reorder_contexts(void);

#endif /* _SYSTEM_INIT_C_H_ */
