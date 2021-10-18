/*
 * Copyright (C) 2014-2016,  Netronome Systems, Inc.  All rights reserved.
 *
 * @file          mem_cam_add_inc.c
 * @brief         MEM CAM add increment.
 */

#include <assert.h>
#include <nfp.h>
#include <nfp6000/nfp_me.h>
#include <mem_cam_add_inc_c.h>



__intrinsic void
__mem_cam_lookup_add_inc(__xrw void *data, __mem __addr40 void *addr,
                 int camsize, int cambits,
                 sync_t sync, SIGNAL_PAIR *sigpair)
{
    struct nfp_mecsr_prev_alu ind;
    unsigned int addr_hi, addr_lo;

    ctassert(__is_ct_const(camsize));
    ctassert(__is_ct_const(cambits));
    ctassert(__is_ct_const(sync));
    ctassert(camsize == 128 || camsize == 256 ||
             camsize == 384 || camsize == 512);
    ctassert(cambits == 24);
    ctassert(sync == sig_done);
    ctassert(__is_read_reg(data));
    ctassert(__is_write_reg(data));

    addr_hi = ((unsigned long long int)addr >> 8) & 0xff000000;
    addr_lo = (unsigned long long int)addr & 0xffffffff;

    if (camsize == 128) {
            __asm mem[cam128_lookup24_add_inc, *data, addr_hi, <<8, addr_lo], \
                sig_done[*__ct_sig_pair(sigpair)];

    } else if (camsize == 256) {
            __asm mem[cam256_lookup24_add_inc, *data, addr_hi, <<8, addr_lo], \
                sig_done[*__ct_sig_pair(sigpair)];

    } else if (camsize == 384) {
            __asm mem[cam384_lookup24_add_inc, *data, addr_hi, <<8, addr_lo], \
                sig_done[*__ct_sig_pair(sigpair)];

    } else if (camsize == 512) {
            __asm mem[cam512_lookup24_add_inc, *data, addr_hi, <<8, addr_lo], \
                sig_done[*__ct_sig_pair(sigpair)];
    }
}

__intrinsic void
mem_cam_lookup_add_inc(__xrw void *data, __mem __addr40 void *addr,
               int camsize, int cambits)
{
    SIGNAL_PAIR sigpair;
    __mem_cam_lookup_add_inc(data, addr, camsize, 24, sig_done, &sigpair);
    wait_for_all(&sigpair);
}


__intrinsic void
__mem_cam128_lookup24_add_inc(__xrw struct mem_cam_add_inc_24bit *data,
                                       __mem __addr40 __align64 void *addr,
                                       sync_t sync, SIGNAL_PAIR *sigpair)
{
    __mem_cam_lookup_add_inc(data, addr, 128, 24, sync, sigpair);
}

__intrinsic void
mem_cam128_lookup24_add_inc(__xrw struct mem_cam_add_inc_24bit *data,
                                       __mem __addr40 __align64 void *addr)
{
    mem_cam_lookup_add_inc(data, addr, 128, 24);
}

__intrinsic void
__mem_cam256_lookup24_add_inc(__xrw struct mem_cam_add_inc_24bit *data,
                                       __mem __addr40 __align64 void *addr,
                                       sync_t sync, SIGNAL_PAIR *sigpair)
{
    __mem_cam_lookup_add_inc(data, addr, 256, 24, sync, sigpair);
}

__intrinsic void
mem_cam256_lookup24_add_inc(__xrw struct mem_cam_add_inc_24bit *data,
                                       __mem __addr40 __align64 void *addr)
{
    mem_cam_lookup_add_inc(data, addr, 256, 24);
}


__intrinsic void
__mem_cam384_lookup24_add_inc(__xrw struct mem_cam_add_inc_24bit *data,
                                       __mem __addr40 __align64 void *addr,
                                       sync_t sync, SIGNAL_PAIR *sigpair)
{
    __mem_cam_lookup_add_inc(data, addr, 384, 24, sync, sigpair);
}

__intrinsic void
mem_cam384_lookup24_add_inc(__xrw struct mem_cam_add_inc_24bit *data,
                                       __mem __addr40 __align64 void *addr)
{
    mem_cam_lookup_add_inc(data, addr, 384, 24);
}

__intrinsic void
__mem_cam512lookup24_add_inc(__xrw struct mem_cam_add_inc_24bit *data,
                                       __mem __addr40 __align64 void *addr,
                                       sync_t sync, SIGNAL_PAIR *sigpair)
{
    __mem_cam_lookup_add_inc(data, addr, 512, 24, sync, sigpair);
}

__intrinsic void
mem_cam512_lookup24_add_inc(__xrw struct mem_cam_add_inc_24bit *data,
                                     __mem __addr40 __align64 void *addr)
{
    mem_cam_lookup_add_inc(data, addr, 512, 24);
}