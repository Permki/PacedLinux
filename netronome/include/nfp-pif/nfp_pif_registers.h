/*
 * Copyright (C) 2017  Netronome Systems, Inc.  All rights reserved.
 *
 * @file          nfp_pif_registers.h
 * @brief         Interface for register control, register I/O and register 
 *                stored counters for NFP PIF library 
 *
 */

#ifndef __NFP_PIF_REGISTERS_H__
#define __NFP_PIF_REGISTERS_H__


#include <stdint.h>
#include <sys/cdefs.h>

#include "_nfp_pif_apiex.h"

__BEGIN_DECLS

typedef struct {
    char *name;
    int width;
} nfp_pif_dsgn_reg_field;

typedef struct {
    char *name;
    char *rclass_str;
    char *table;
    int count;

    /* when hidden is set the register should be ommitted from
     * the register listing
     */
    int hidden;

#define PIF_DESIGN_REGISTER_CLASS_GLOBAL 0
#define PIF_DESIGN_REGISTER_CLASS_STATIC 1
#define PIF_DESIGN_REGISTER_CLASS_DIRECT 2
    int rclass;

    int table_id;

    int table_size;

    /* number of 32-bit words of data per register */
    int szlw;

    /* index into register array */
    int id;

    nfp_pif_dsgn_reg_field *fields;
    int fields_len;
} nfp_pif_dsgn_reg;

typedef struct  {
    char *name;
    char *type_str;
    char *register_str;
    char *field_str;

    int width;

    /* when hidden is set the counter should be ommitted from
     * the counter listing
     */
    int hidden;

    nfp_pif_dsgn_reg *design_register;
} nfp_pif_dsgn_cntr;


/*
 * Register IO functions
 */
PIF_PUBLIC
nfp_pif_dsgn_reg *nfp_pif_registers_get_list(uint64_t handle,
                                              int *reg_cnt);

PIF_PUBLIC
int nfp_pif_registers_retrieve(uint64_t handle,
                               int regid,
                               int index,
                               int count,
                               char ***fields_out);

PIF_PUBLIC
int nfp_pif_registers_clear(uint64_t handle,
                            int regid,
                            int index,
                            int count);

PIF_PUBLIC
int nfp_pif_registers_set_field(uint64_t handle,
                                int regid,
                                int index,
                                int count,
                                int field_id,
                                const char *field_value);

PIF_PUBLIC
int nfp_pif_registers_set(uint64_t handle,
                          int regid,
                          int index,
                          int count,
                          const char ** const field_values,
                          int field_values_count);

PIF_PUBLIC
void nfp_pif_registers_free_dsgn_reg_list(nfp_pif_dsgn_reg *reg_list,
                                          int reg_cnt);

/*
 * Counter APIs
 */
PIF_PUBLIC
nfp_pif_dsgn_cntr *nfp_pif_registers_counter_get_list(uint64_t handle,
                                                      int *dcntr_cnt);

PIF_PUBLIC
int nfp_pif_registers_counter_clear(uint64_t handle, int counter_id);

PIF_PUBLIC
int nfp_pif_registers_counter_clear_all(uint64_t handle);

PIF_PUBLIC
uint64_t *nfp_pif_registers_counter_retrieve(uint64_t handle,
                                       int counter_id,
                                       int *val_len);

PIF_PUBLIC
void nfp_pif_registers_free_counter_list(nfp_pif_dsgn_cntr *dgsn_cnter,
                                         int dcntr_cnt);


__END_DECLS

#endif /* !__NFP_PIF_REGISTERS_H__ */