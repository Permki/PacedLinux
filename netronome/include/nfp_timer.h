/*
 * Copyright (C) 2014, Netronome Systems, Inc.
 * All right reserved.
 *
 */

#ifndef NFP_TIMER_H
#define NFP_TIMER_H

/* API is part of libnfp */
#include "_nfp_apiex.h"

struct nfp_timer;

/**
 * Acquire a handle to one of the NFP Timers
 * @ingroup nfp6000-only
 *
 * @param       nfp     NFP Device
 */
NFP_API
struct nfp_timer *nfp_timer_acquire(struct nfp_device *nfp);

/**
 * Release the handle to a NFP Timer
 * @ingroup nfp6000-only
 *
 * @param       timer   NFP Timer
 */
NFP_API
void nfp_timer_release(struct nfp_timer *timer);

/**
 * Do busy-wait delay
 * @ingroup nfp6000-only
 *
 * @param       timer      NFP Timer
 * @param       nanosecs   delay period
 */
NFP_API
int nfp_timer_busy_wait(struct nfp_timer *timer, long nanosecs);

/**
 * Do busy-wait delay
 * @ingroup nfp6000-only
 *
 * @param       timer      NFP Timer
 * @param       nanosecs   default delay period
 *
 * Used to prevent internal calculation for each busy_wait call if
 *  a non dymanic wait period is sufficient
 */
NFP_API
int nfp_timer_set_default_wait(struct nfp_timer *timer, long nanosecs);

#endif /* NFP_TIMER_H */
