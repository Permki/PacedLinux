/*
 * Copyright (C) 2017  Netronome Systems, Inc.  All rights reserved.
 *
 * @file          nfp_pif_meters.h
 * @brief         Interface for meters for NFP PIF library 
 *
 */

#ifndef __NFP_PIF_METERS_H__
#define __NFP_PIF_METERS_H__


#include <stdint.h>
#include <sys/cdefs.h>

#include "_nfp_pif_apiex.h"

__BEGIN_DECLS

/* direct copy of struct struct meter_cfg_op in meter_cfg.h to allow direct cast*/
typedef struct {
    double rate;
    uint32_t burst;
    int offset;
    int count;
} nfp_pif_meter_cfg_op;

typedef struct {
    char *name;
    char *mclass_str;
    char *meters_str;
    int count;

#define PIF_DESIGN_METER_CLASS_GLOBAL 0
#define PIF_DESIGN_METER_CLASS_STATIC 1
#define PIF_DESIGN_METER_CLASS_DIRECT 2
    int mclass;

#define PIF_DESIGN_METER_METERS_PKTS  0
#define PIF_DESIGN_METER_METERS_BYTES 1
    int meters;

    char *table;
    int table_id;

    int table_size;

    /* total number of entries, in the case of direct count * table_size */
    int tot_count;

    int id;
} nfp_pif_dsgn_meter;


PIF_PUBLIC
int nfp_pif_meter_get_count(uint64_t handle);

PIF_PUBLIC
nfp_pif_dsgn_meter *nfp_pif_meter_get_meters_data(uint64_t handle, 
                                                  int *mtr_cnt);

PIF_PUBLIC
nfp_pif_meter_cfg_op *nfp_pif_meter_cfg_get_ops(uint64_t handle,
                                                int mindex,
                                                int *ops_sz);

PIF_PUBLIC
int nfp_pif_meter_cfg_set_ops(uint64_t handle,
                              int mindex,
                              const nfp_pif_meter_cfg_op *ops,
                              int ops_sz);

PIF_PUBLIC
void nfp_pif_meters_free_dsg_meter(nfp_pif_dsgn_meter *meters, int mtr_cnt);

__END_DECLS

#endif /* !__NFP_PIF_METERS_H__ */