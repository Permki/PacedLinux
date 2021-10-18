/*
 * Copyright 2015-2017 Netronome Systems, Inc. All rights reserved.
 *
 *
 * @file          macflushuser.h
 * @brief         Interface to the mac flush polling function
 *
 * OVERVIEW
 *
 *
 */
#ifndef __MACFLUSHUSESR_H__
#define __MACFLUSHUSESR_H__
#include <assert.h>
#include <nfp.h>
#include <stdint.h>


/* A bitmap of ports which are paused.
 * Bits 0-23 correspond to ports 0-23 on NBI0.
 * Bits 24-47 correspond to ports 0-23 on NBI1.
 * Store this bitmap in the upper 8 bytes of CLS on every worker island
 */

#define MAC_PORT_PAUSED_BITMAP_ADDR 0xfff8
__asm {
    .alloc_mem MAC_PORT_PAUSED_BITMAP cls+MAC_PORT_PAUSED_BITMAP_ADDR \
                island 0x8 addr40
    .init MAC_PORT_PAUSED_BITMAP 0 0
}

/**
 * Check whether the NBI egress port is paused (i.e. has received a pause frame
 * from the remote port it is linked to).
 *
 * @param port  The port number to check.
 * @return      1 if paused or 0 otherwise
 */
__intrinsic int mac_is_port_paused(uint32_t subsys, uint32_t port);

/**
 * Check whether the NBI egress port is paused (i.e. has received a pause frame
 * from the remote port it is linked to).
 *
 * @param port_subsys        Subsystem from port (PKT_PORT_SUBSYS_of)
 * @param port_q             Queue from port (PKT_PORT_QUEUE_of)
 * @param xfr_port_bitmap    Read xfer for port to use in IO
 * @param sig                Signal to use in IO
 *
 * @return                   Port bit to check
 */
__intrinsic uint64_t __mac_is_port_paused(int port_subsys, int port_q,
                                          __xread uint64_t *xfr_port_bitmap,
                                          SIGNAL *sig);


/**
 * Main loop to continuously check the pause status (EthRxPauseStatus) bit
 * for every MAC/NBI ports. If non-zero, flush them (EthCmdConfig.EthTxFlush)
 * if they have been paused for PAUSE_COUNT_THRES polling iterations.
 * 
 * Call this function from main and run it on one context:
 * i.e.
 *      if (ctx() == 0) mac_poll();
 */
void mac_poll();

#endif /* __MACFLUSHUSESR_H__ */
