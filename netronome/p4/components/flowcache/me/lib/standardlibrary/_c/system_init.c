/*
 * Copyright (C) 2014-2016,  Netronome Systems, Inc.  All rights reserved.
 *
 * @file          system_init.c
 * @brief         System startup reordering of threads.
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


__asm .alloc_mem _system_init_done emem0 global 8
volatile __declspec(emem0, shared, import) unsigned int *system_init_done_flag;


 //.alloc_mem _system_init_done emem0 global 8
void system_init_wait_done()
{
    __declspec(read_reg) unsigned int   xr_done;

    system_init_done_flag  = (__declspec(emem0, shared, import) unsigned int *)__link_sym("_system_init_done");
    do
    {
        mem_read32(&xr_done, (__mem __addr40 void *)system_init_done_flag, 1 << 2);
    }
    while (xr_done != SYSTEM_DONE_FLAG);

}

void system_init_done()
{
    __declspec(write_reg) unsigned int  xr_done = SYSTEM_DONE_FLAG;

    system_init_done_flag  = (__declspec(emem0, shared, import) unsigned int *)__link_sym("_system_init_done");
    mem_write32(&xr_done, (__mem __addr40 void *)system_init_done_flag, 1 << 2);
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
