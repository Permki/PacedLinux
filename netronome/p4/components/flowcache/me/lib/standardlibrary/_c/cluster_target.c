/*
 * Copyright (C) 2014-2016,  Netronome Systems, Inc.  All rights reserved.
 *
 * @file          cluster_target.c
 * @brief         Cluster target related functions
 */

#include <assert.h>
#include <nfp.h>

#include <nfp6000/nfp_me.h>

#include <cluster_target_c.h>


__intrinsic void __signal_me(unsigned int dst_island, unsigned int dst_me, unsigned int dst_ctx, unsigned int sig_no)
{
    unsigned int addr;

    addr = ((dst_island & 0x3f) << 24 | (dst_me & 15) << 9 | (dst_ctx & 7) << 6 | (sig_no & 15) << 2);

    __asm ct[interthread_signal, --, addr, 0, --];
}