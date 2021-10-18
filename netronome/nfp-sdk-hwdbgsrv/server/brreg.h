/*
 * Copyright (C) 2014, Netronome Systems, Inc.  All rights reserved.
 */

/* header file for branch based on register
 * this is required for single stepping
 */
#ifndef __BRREG_H__
#define __BRREG_H__

/* XXX this ENUM is hopelessly out of date for TH, what about LMEM 2/3 */
enum BRREG_TYPES {
    IXP_NO_DEST,
    IXP_GPA_REL,
    IXP_GPA_ABS,
    IXP_GPB_REL,
    IXP_GPB_ABS,
    IXP_SR_REL,
    IXP_SR_RD_REL,
    IXP_SR_WR_REL,
    IXP_SR_ABS,
    IXP_SR_RD_ABS,
    IXP_SR_WR_ABS,
    IXP_SR0_SPILL,
    IXP_SR1_SPILL,
    IXP_SR2_SPILL,
    IXP_SR3_SPILL,
    IXP_R0_MEM_ADDR,
    IXP_SR1_MEM_ADDR,
    IXP_SR2_MEM_ADDR,
    IXP_SR3_MEM_ADDR,
    IXP_DR_REL,
    IXP_DR_RD_REL,
    IXP_DR_WR_REL,
    IXP_DR_ABS,
    IXP_DR_RD_ABS,
    IXP_DR_WR_ABS,
    IXP_DR_MEM_ADDR,
    IXP_LMEM,
    IXP_LMEM0,
    IXP_LMEM1,
    IXP_LMEM_SPILL,
    IXP_LMEM_ADDR,
    IXP_NEIGH_REL,
    IXP_NEIGH_INDX,
    IXP_SIG_REL,
    IXP_SIG_INDX,
    IXP_SIG_DOUBLE,
    IXP_SIG_SINGLE,
    IXP_SCRATCH_MEM_ADDR,
    IXP_UMEM0,
    IXP_UMEM1,
    IXP_UMEM_SPILL,
    IXP_UMEM_ADDR,
    IXP_DR1_MEM_ADDR,
    IXP_SR0_IMPORTED,
    IXP_SR1_IMPORTED,
    IXP_SR2_IMPORTED,
    IXP_SR3_IMPORTED,
    IXP_DR_IMPORTED,
    IXP_DR1_IMPORTED,
    IXP_SCRATCH_IMPORTED,
    NFP_LSCRATCH_MEM_ADDR,
    NFP_LSCRATCH_0_MEM_ADDR,	// cluster 0 scratch
    NFP_LSCRATCH_1_MEM_ADDR,	// cluster 1 scratch
    NFP_LSCRATCH_2_MEM_ADDR,	// cluster 2 scratch
    NFP_LSCRATCH_3_MEM_ADDR,	// cluster 3 scratch
    NFP_LSCRATCH_4_MEM_ADDR,	// cluster 4 scratch
    IXP_ANY_REG = 0xffff
};

int brreg_get_offset(struct nfp_device *nfp, int meid, int ctx,
                     int regtype, int regaddr, uint32_t *regdata);

#endif /* __BRREG_H__ */
