/*
 * Copyright (C) 2014-2016,  Netronome Systems, Inc.  All rights reserved.
 *
 * @file          system_init_c.h
 * @brief         Synchronize context on system up
 */

#ifndef _SYSTEM_INIT_C_H_
#define _SYSTEM_INIT_C_H_


#define ME_INIT_SIGNAL                  15  /* System init done signal */


/*
 * Set a flag when initialisation complete, i.e. flow cache free ring entries
 */
void system_init_done(void);

/*
 * Wait for the flag to indicate initialisation complete (normally first ctx)
 */
void system_init_wait_done(void);

/*
 * All contexts wait for init to complete (above). Highest context start
 * signalling lower contexts.
 */
void system_reorder_contexts(void);

#endif //_SYSTEM_INIT_C_H_