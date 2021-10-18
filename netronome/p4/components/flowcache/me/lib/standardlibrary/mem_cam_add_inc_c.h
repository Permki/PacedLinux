/*
 * Copyright (C) 2014-2016,  Netronome Systems, Inc.  All rights reserved.
 *
 * @file          mem_cam_add_inc_c.h
 * @brief         Interface for ME CAM increment functions
 */

#ifndef _NFP__MEM_CAM_ADD_INC_C_H_
#define _NFP__MEM_CAM_ADD_INC_C_H_

#include <nfp.h>



/**
 * CAM search and result data for 32bit CAM lookup add inc.
 */
struct mem_cam_add_inc_24bit
{
    union
    {
        struct {
            unsigned int value;
        } search;

        struct {
            unsigned int mask:16;       /* Bits are set on matching entries */
            unsigned int data:8;        /* Upper 8-bits of the matched CAM entry is returned in this field.*/
            unsigned int added:1;       /* When set, this field indicates that an entry was added. */
            unsigned int match:7;       /* First matched entry number, or on a CAM miss where the entry was added.*/
        } result;
    };
};


__intrinsic void __mem_cam128_lookup24_add_inc(__xrw struct mem_cam_add_inc_24bit *data,
                                       __mem __addr40 __align64 void *addr,
                                       sync_t sync, SIGNAL_PAIR *sigpair);

__intrinsic void mem_cam128_lookup24_add_inc(__xrw struct mem_cam_add_inc_24bit *data,
                                       __mem __addr40 __align64 void *addr);


#endif /* _NFP__MEM_CAM_ADD_INC_C_H_ */
