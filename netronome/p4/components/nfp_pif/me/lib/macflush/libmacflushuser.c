/*
 * Copyright 2015-2017 Netronome Systems, Inc. All rights reserved.
 *
 * @file          lib/macflush/libmacflushuser.c
 * @brief         Functions to interface to the check if MAC has been paused
 *
 * This file contains the interface to verify if the MAC port has been paused
 * on NBI. Check if the port is paused (i.e. has received a pause frame
 * from the remote port it is linked to).
 *
 */

#include <nfp.h>
#include <stdint.h>
#include <nfp/me.h>
#include <nfp/cls.h>
#include <pktio/pktio.h>
#include <macflush/macflushuser.h>


// TODO use MAC port settings?
#define PORT_IN_MAP(subsys, queue)  ((subsys) * 24 + (queue))

__intrinsic uint64_t __mac_is_port_paused(int port_subsys, int port_q,
                                          __xread uint64_t *xfr_port_bitmap,
                                          SIGNAL *sig)
{
    __cls __addr32 void *port_bitmap_addr = (__cls __addr32 void*)
                                        __link_sym("MAC_PORT_PAUSED_BITMAP");
    uint32_t count = sizeof(*xfr_port_bitmap)>>2;
    int temp_port = PORT_IN_MAP(port_subsys, port_q);

    __asm {
        cls[read, *xfr_port_bitmap, port_bitmap_addr, 0, \
                __ct_const_val(count)], sig_done[*sig]
    }

    return ((uint64_t) 1) << temp_port;
}

__intrinsic int mac_is_port_paused(uint32_t subsys, uint32_t port)
{
    int is_paused = 0;
    __xread uint64_t xfr_port_bitmap;
    int      temp_port;
    uint64_t port_bit;
    SIGNAL sig;

    port_bit = __mac_is_port_paused(subsys, port, &xfr_port_bitmap, &sig);

    wait_for_all(&sig);

    if (xfr_port_bitmap & port_bit)
        is_paused = 1;

    return is_paused;
}
