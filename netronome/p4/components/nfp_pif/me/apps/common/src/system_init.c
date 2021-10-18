/*
 * Copyright (C) 2014-2015 Netronome Systems, Inc.  All rights reserved.
 *
 * File:        system_init.c
 *
 */

#ifndef _SYSTEM_INIT_C_
#define _SYSTEM_INIT_C_

#include <assert.h>
#include <nfp.h>
#include <stdint.h>

#include <nfp6000/nfp_me.h>
#include <nfp/me.h>
#include <nfp/mem_bulk.h>
#include "system_init_c.h"

#define SYSTEM_DONE_FLAG        0x01

__emem __shared __export unsigned int system_init_done_flag = 0;
static __shared __gpr int init_done = 0;

void system_init_wait_done()
{
    __xread unsigned int xr_done;

    if (ctx() == 0) {
        do {
            sleep(2000);
            mem_read32(&xr_done,
                       (__mem __addr40 void *)&system_init_done_flag,
                       4);
        } while (xr_done != SYSTEM_DONE_FLAG);

        init_done = 1;
    } else {
        while (!init_done) {
            __asm ctx_arb[voluntary];
        }
    }

}

void system_init_done()
{
    __xwrite unsigned int xr_done = SYSTEM_DONE_FLAG;

    mem_write32(&xr_done,
                (__mem __addr40 void *)&system_init_done_flag,
                4);
}



void system_reorder_contexts(void)
{
    __gpr uint32_t      last_context_number = 7;
    SIGNAL              sig;

    __assign_relative_register((void*)&sig, ME_INIT_SIGNAL);


    if (__nctx_mode() == 4)
    {
        last_context_number = 6;
    }

    if (ctx() == last_context_number)
    {
        /*
        * If last context, signal context 0
        */
        local_csr_write(local_csr_same_me_signal, ((1 << 7) | (ME_INIT_SIGNAL << 3)));
        wait_for_all(&sig);
    }
    else
    {
        wait_for_all(&sig);
        local_csr_write(local_csr_same_me_signal, ((1 << 7) | (ME_INIT_SIGNAL << 3)));
    }
    __implicit_write(&sig);
}


#endif //_SYSTEM_INIT_C_
