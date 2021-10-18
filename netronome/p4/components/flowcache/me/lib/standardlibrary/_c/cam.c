/*
 * Copyright (C) 2014-2016,  Netronome Systems, Inc.  All rights reserved.
 *
 * @file          cam.c
 * @brief         Interface for ME CAM functions
 */

#include <assert.h>
#include <nfp.h>

#include <nfp6000/nfp_me.h>

#include <cam_c.h>


__intrinsic void
cam_clear()
{
    __asm nop;      // Just in case previous instruction is conditional p3-branch
                    // Will be removed by scheduler if not
    __asm cam_clear;
}


__intrinsic void
cam_write(unsigned int entry_num, unsigned int tag, unsigned int cam_state)
{
    ctassert(__is_ct_const(cam_state));
    if (__is_ct_const(entry_num))
    {
        ctassert(entry_num <= 15);
    }
    __asm nop;      // Just in case previous instruction is conditional p3-branch
                    // Will be removed by scheduler if not
    __asm cam_write[entry_num, tag, __ct_const_val(cam_state)];
}

__intrinsic cam_lookup_t
cam_lookup(unsigned int tag)
{
    cam_lookup_t result;

    __asm nop;      // Just in case previous instruction is conditional p3-branch
                    // Will be removed by scheduler if not
    __asm cam_lookup[result, tag]
    return result;
}