/*
 * Copyright (C) 2017  Netronome Systems, Inc.  All rights reserved.
 *
 * @file          nfp_pif_mcast.h
 * @brief         Interface for multicast feature for NFP PIF library 
 *
 */

#ifndef __NFP_PIF_MCAST_H__
#define __NFP_PIF_MCAST_H__


#include <stdint.h>
#include <sys/cdefs.h>

#include "_nfp_pif_apiex.h"

typedef struct {
    int id;
    int max_entries;
    int32_t *ports;
    int cnt;
} nfp_pif_mcast_group;

__BEGIN_DECLS

/* return -1 or error; number of port present on success */
PIF_PUBLIC
nfp_pif_mcast_group *nfp_pif_mcast_group_list(uint64_t handle, int *grpcnt);

PIF_PUBLIC
nfp_pif_mcast_group *nfp_pif_mcast_group_read(uint64_t handle,
                             const int32_t grp_id);

/* return -1 or error; number of ports loaded on success */
PIF_PUBLIC
int nfp_pif_mcast_group_write(uint64_t handle,
                              int grp_id,
                              const int *ports,
                              int cnt);

PIF_PUBLIC
void nfp_pif_free_groups(nfp_pif_mcast_group *groups, int grpcnt);

__END_DECLS

#endif /* !__NFP_PIF_MCAST_H__ */