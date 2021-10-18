/*
 * Copyright (C) 2017  Netronome Systems, Inc.  All rights reserved.
 *
 * @file          nfp_pif_sys_counters.h
 * @brief         Interface for system counters for NFP PIF library 
 *
 */

#ifndef __NFP_PIF_SYS_COUNTERS_H__
#define __NFP_PIF_SYS_COUNTERS_H__


#include <stdint.h>
#include <sys/cdefs.h>

#include "_nfp_pif_apiex.h"

__BEGIN_DECLS

typedef struct {
    char *name;
    uint64_t val;
} nfp_pif_sys_counter;


PIF_PUBLIC
int nfp_pif_sys_counters_clear(uint64_t handle);

PIF_PUBLIC
nfp_pif_sys_counter * nfp_pif_sys_counters_retrieve(uint64_t handle,
                                                    int *sys_cntr_cnt);

PIF_PUBLIC
void nfp_pif_sys_counters_free_cntr_list(nfp_pif_sys_counter *sys_cntrs,
                                         int sys_cntr_cnt);

__END_DECLS

#endif /* !__NFP_PIF_SYS_COUNTERS_H__ */