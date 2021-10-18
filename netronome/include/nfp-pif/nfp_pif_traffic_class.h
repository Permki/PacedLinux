/*
 * Copyright (C) 2017  Netronome Systems, Inc.  All rights reserved.
 *
 * @file          nfp_pif_traffic.h
 * @brief         Interface for traffic classes for NFP PIF library 
 *
 */

#ifndef __NFP_PIF_TRAFFIC_H__
#define __NFP_PIF_TRAFFIC_H__


#include <stdint.h>
#include <sys/cdefs.h>

#include "_nfp_pif_apiex.h"

__BEGIN_DECLS

typedef struct {
    int class_id;
    int weight;
    int queue_no;
    int committed;
} nfp_pif_tc_config;

PIF_PUBLIC
int nfp_pif_traffic_class_config(uint64_t handle,
                                 int port_id,
                                 int class_id,
                                 int weight);

PIF_PUBLIC
nfp_pif_tc_config *nfp_pif_traffic_class_retrieve(uint64_t handle,
                                                 int port_id,
                                                 int *num_configs);

PIF_PUBLIC
int nfp_pif_traffic_class_sync(uint64_t handle, int port_id);

__END_DECLS

#endif /* !__NFP_PIF_TRAFFIC_H__ */