/*
 * Copyright (C) 2014-2016,  Netronome Systems, Inc.  All rights reserved.
 *
 * @file          mem_atomic_indirect.c
 * @brief         MEM atomic indirect function.
 */

#include <assert.h>
#include <nfp.h>
#include <stdint.h>
#include <nfp6000/nfp_me.h>
#include <mem_atomic_indirect_c.h>


__intrinsic void mem_write_atomic_with_bytemask (__xwrite void *data, __mem __addr40 void *addr,
             size_t size, const size_t max_size, sync_t sync, SIGNAL *sig, unsigned int byte_mask)
{

    __gpr uint32_t                              addr_hi;
    __gpr uint32_t                              addr_lo;
    struct nfp_mecsr_prev_alu                   prev_alu;
    struct nfp_mecsr_cmd_indirect_ref_0         ind_csr;
    unsigned int                                count = (size >> 2);
    unsigned int                                max_count = (max_size >> 2);

    ctassert(__is_write_reg(data));
    try_ctassert(__is_aligned(size, 4));
    try_ctassert(size <= 128);

    prev_alu.__raw = 0;
    prev_alu.ov_bm_csr = 1;

    ind_csr.__raw = 0;
    ind_csr.byte_mask = byte_mask;

    addr_hi = ((unsigned long long int)addr >> 8) & 0xff000000;
    addr_lo = (unsigned long long int)addr & 0xffffffff;

    if (sync == sig_done) {
        __asm
        {
            local_csr_wr[CMD_INDIRECT_REF_0, ind_csr.__raw]
            alu[--, --, B, prev_alu.__raw]
            mem[write_atomic, *data, addr_hi, << 8,   addr_lo, __ct_const_val(count)], sig_done[*sig], indirect_ref
        }
    } else {
        __asm
        {
            local_csr_wr[CMD_INDIRECT_REF_0, ind_csr.__raw]
            alu[--, --, B, prev_alu.__raw]
            mem[write_atomic, *data, addr_hi, << 8,   addr_lo, __ct_const_val(count)], ctx_swap[*sig], indirect_ref
        }
    }
}

