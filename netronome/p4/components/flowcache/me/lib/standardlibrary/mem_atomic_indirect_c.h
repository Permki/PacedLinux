/*
 * Copyright (C) 2014-2016,  Netronome Systems, Inc.  All rights reserved.
 *
 * @file          mem_atomic_indirect_c.h
 * @brief         Interface for common ME related functions and types
 */

#ifndef _MEM_ATOMIC_INDIRECT_C_H_
#define _MEM_ATOMIC_INDIRECT_C_H_

#include <nfp.h>




__intrinsic void mem_write_atomic_with_bytemask (__xwrite void *data, __mem __addr40 void *addr,
             size_t size, const size_t max_size, sync_t sync, SIGNAL *sig, unsigned int byte_mask);





#endif /* _MEM_ATOMIC_INDIRECT_C_H_ */
