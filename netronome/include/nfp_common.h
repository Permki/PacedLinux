/*
 * Copyright (C) 2009-2014, Netronome Systems, Inc.  All rights reserved.
 *
 */

#ifndef NFP_COMMON_H
#define NFP_COMMON_H

struct nfp_device;

/** Retrieve system uptime in milliseconds
 *
 *  implemented via CLOCK_MONOTONIC_RAW or similar hardware based timer
 *  guaranteed to always increase
 */
uint64_t tickcount_ms(void);

/** Retrieve system uptime in microseconds
 *
 *  implemented via CLOCK_MONOTONIC_RAW or similar hardware based timer
 *  guaranteed to always increase
 */
uint64_t tickcount_us(void);

/** Retrieve an NFP clock frequency in Hz
 *
 *  @param nfp          NFP device context
 *  @param pll          target pll
 *  @param div          target divisor
 *  @param hz           clock frequency in Hz
 */
int nfp_clock(struct nfp_device *nfp, int pll, int div, uint32_t *hz);

/** Retrieve NFP P_CLK frequency in Hz
 *
 *  @param nfp          NFP device context
 *  @param hz           clock frequency in Hz
 */
int nfp_pclk(struct nfp_device *nfp, uint32_t *hz);

/** Retrieve ARM processor clock frequency in Hz
 *
 *  @param nfp          NFP device context
 *  @param hz           clock frequency in Hz
 */
int nfp_arm_clock(struct nfp_device *nfp, uint32_t *hz);

/** Check that current platform is of a specific type
 *
 *  @param nfp          NFP device context
 *  @param plat         specific platform name to be checked for
 *
 *  returns 1 on match, 0 otherwise. -1 on error
 */
int nfp_platform_is(struct nfp_device *nfp, const char *plat);

uint16_t cpp_interface(struct nfp_device *nfp);

#endif /* NFP_COMMON_H */
