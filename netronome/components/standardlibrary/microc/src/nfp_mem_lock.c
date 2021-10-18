/*
 * Copyright (C) 2008-2014 Netronome Systems, Inc.  All rights reserved.
 */

#ifndef __NFP_MEM_LOCK_C__
#define __NFP_MEM_LOCK_C__

#include <nfp.h>
#include <nfp_intrinsic.h>
#include <nfp_mem.h>
#include <nfp_mem_lock.h>

#ifndef NFP_LIB_ANY_NFCC_VERSION
    #if (!defined(__NFP_TOOL_NFCC) ||                       \
        (__NFP_TOOL_NFCC < NFP_SW_VERSION(5, 0, 0, 0)) ||   \
        (__NFP_TOOL_NFCC > NFP_SW_VERSION(6, 1, 0, 255)))
        #error "This standard library is not supported for the version of the SDK currently in use."
    #endif
#endif

/*
 * NOTE: When writing inline-asm, it's important not to use reserved words
 *       for variables.  Common ones are 'a','b', 'csr', 'state', and
 *       'inp_state'.
 */

__intrinsic
void cmd_mem_lock128_init_ptr32(__addr32 __mem __align16 mem_lock128_t *mem_lock)
{
    cmd_mem_write_atomic_imm_ptr32(0, (void __addr32 __mem *)mem_lock, sizeof(*mem_lock) / sizeof(uint32_t));
}

__intrinsic
void cmd_mem_lock128_init_ptr40(__addr40 __mem __align16 mem_lock128_t *mem_lock)
{
    cmd_mem_write_atomic_imm_ptr40(0, mem_lock, sizeof(*mem_lock) / sizeof(uint32_t));
}

__intrinsic
void cmd_mem_lock256_init_ptr32(__addr32 __mem __align16 mem_lock256_t *mem_lock)
{
    cmd_mem_write_atomic_imm_ptr32(0, (void __addr32 __mem *)mem_lock, sizeof(*mem_lock) / sizeof(uint32_t));
}

__intrinsic
void cmd_mem_lock256_init_ptr40(__addr40 __mem __align16 mem_lock256_t *mem_lock)
{
    cmd_mem_write_atomic_imm_ptr40(0, mem_lock, sizeof(*mem_lock) / sizeof(uint32_t));
}

__intrinsic
void cmd_mem_lock384_init_ptr32(__addr32 __mem __align16 mem_lock384_t *mem_lock)
{
    __addr32 __mem uint32_t *fp = (__addr32 __mem uint32_t *)mem_lock;
    cmd_mem_write_atomic_imm_ptr32(0, (void __addr32 __mem *)mem_lock, 8);
    cmd_mem_write_atomic_imm_ptr32(0, fp + 8, 4);
}

__intrinsic
void cmd_mem_lock384_init_ptr40(__addr40 __mem __align16 mem_lock384_t *mem_lock)
{
    __addr40 __mem uint32_t *fp = (__addr40 __mem uint32_t *)mem_lock;
    cmd_mem_write_atomic_imm_ptr40(0, mem_lock, 8);
    cmd_mem_write_atomic_imm_ptr40(0, fp + 8, 4);
}

__intrinsic
void cmd_mem_lock512_init_ptr32(__addr32 __mem __align16 mem_lock512_t *mem_lock)
{
    __addr32 __mem uint32_t *fp = (__addr32 __mem uint32_t *)mem_lock;
    cmd_mem_write_atomic_imm_ptr32(0, (void __addr32 __mem *)mem_lock, sizeof(*mem_lock) / sizeof(uint32_t) / 2);
    cmd_mem_write_atomic_imm_ptr32(0, fp + sizeof(*mem_lock) / sizeof(uint32_t) / 2, sizeof(*mem_lock) / sizeof(uint32_t) / 2);
}

__intrinsic
void cmd_mem_lock512_init_ptr40(__addr40 __mem __align16 mem_lock512_t *mem_lock)
{
    __addr40 __mem uint32_t *fp = (__addr40 __mem uint32_t *)mem_lock;
    cmd_mem_write_atomic_imm_ptr40(0, mem_lock, sizeof(*mem_lock) / sizeof(uint32_t) / 2);
    cmd_mem_write_atomic_imm_ptr40(0, fp + sizeof(*mem_lock) / sizeof(uint32_t) / 2, sizeof(*mem_lock) / sizeof(uint32_t) / 2);
}

enum
{
    MEM_LOCK_REF_COUNT_128 = 0x3,
    MEM_LOCK_REF_COUNT_256 = 0x7,
    MEM_LOCK_REF_COUNT_384 = 0xB,
    MEM_LOCK_REF_COUNT_512 = 0xF
};


__intrinsic
__xread mem_lock_out_t *cmd_mem_lock128_ptr32(
    __addr32 __mem __align16 mem_lock128_t  *mem_lock,
    __xwrite mem_lock_in_t      *xfer,
    uint32_t                    lsbzeroes,
    sync_t                      sync,
    SIGNAL_PAIR                 *sig_pair_ptr)
{
    _INTRINSIC_BEGIN;
    {
        generic_ind_t ind;
        CT_ASSERT(__is_write_reg(xfer));
        _INTRINSIC_OVERRIDE_LENGTH_AND_BYTEMASK_IN_ALU(ind, MEM_LOCK_REF_COUNT_128, lsbzeroes);
        __asm alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
        __asm mem[lock128, *xfer, mem_lock, 0 ], sig_done[*sig_pair_ptr], indirect_ref
    }
    return (__xread mem_lock_out_t *)(xfer);
    _INTRINSIC_END;
}

__intrinsic
__xread mem_lock_out_t *cmd_mem_lock256_ptr32(
    __addr32 __mem __align16 mem_lock256_t  *mem_lock,
    __xwrite mem_lock_in_t      *xfer,
    uint32_t                    lsbzeroes,
    sync_t                      sync,
    SIGNAL_PAIR                 *sig_pair_ptr)
{
    _INTRINSIC_BEGIN;
    {
        generic_ind_t ind;
        CT_ASSERT(__is_write_reg(xfer));
        _INTRINSIC_OVERRIDE_LENGTH_AND_BYTEMASK_IN_ALU(ind, MEM_LOCK_REF_COUNT_256, lsbzeroes);
        __asm alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
        __asm mem[lock256, *xfer, mem_lock, 0 ], sig_done[*sig_pair_ptr], indirect_ref
    }
    return (__xread mem_lock_out_t *)(xfer);
    _INTRINSIC_END;
}

__intrinsic
__xread mem_lock_out_t *cmd_mem_lock384_ptr32(
    __addr32 __mem __align16 mem_lock384_t  *mem_lock,
    __xwrite mem_lock_in_t      *xfer,
    uint32_t                    lsbzeroes,
    sync_t                      sync,
    SIGNAL_PAIR                 *sig_pair_ptr)
{
    _INTRINSIC_BEGIN;
    {
        generic_ind_t ind;
        CT_ASSERT(__is_write_reg(xfer));
        _INTRINSIC_OVERRIDE_LENGTH_AND_BYTEMASK_IN_ALU(ind, MEM_LOCK_REF_COUNT_384, lsbzeroes);
        __asm alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
        __asm mem[lock384, *xfer, mem_lock, 0 ], sig_done[*sig_pair_ptr], indirect_ref
    }
    return (__xread mem_lock_out_t *)(xfer);
    _INTRINSIC_END;
}

__intrinsic
__xread mem_lock_out_t *cmd_mem_lock512_ptr32(
    __addr32 __mem __align16 mem_lock512_t  *mem_lock,
    __xwrite mem_lock_in_t      *xfer,
    uint32_t                    lsbzeroes,
    sync_t                      sync,
    SIGNAL_PAIR                 *sig_pair_ptr)
{
    _INTRINSIC_BEGIN;
    {
        generic_ind_t ind;
        CT_ASSERT(__is_write_reg(xfer));
        _INTRINSIC_OVERRIDE_LENGTH_AND_BYTEMASK_IN_ALU(ind, MEM_LOCK_REF_COUNT_512, lsbzeroes);
        __asm alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
        __asm mem[lock512, *xfer, mem_lock, 0 ], sig_done[*sig_pair_ptr], indirect_ref
    }
    return (__xread mem_lock_out_t *)(xfer);
    _INTRINSIC_END;
}


__intrinsic
__xread mem_lock_out_t *cmd_mem_lock128_ptr40(
    __addr40 __mem __align16 mem_lock128_t      *mem_lock,
    __xwrite mem_lock_in_t                      *xfer,
    uint32_t                                    lsbzeroes,
    sync_t                                      sync,
    SIGNAL_PAIR                                 *sig_pair_ptr)
{
    _INTRINSIC_BEGIN;
    {
        generic_ind_t   ind;
        uint32_t        hi_addr, low_addr;
        _INTRINSIC_CONVERT_HI_LO_ADDRESS(mem_lock);
        CT_ASSERT(__is_write_reg(xfer));
        _INTRINSIC_OVERRIDE_LENGTH_AND_BYTEMASK_IN_ALU(ind, MEM_LOCK_REF_COUNT_128, lsbzeroes);
        __asm alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
        __asm mem[lock128, *xfer, hi_addr, <<8, low_addr], sig_done[*sig_pair_ptr], indirect_ref
    }
    return (__xread mem_lock_out_t *)(xfer);
    _INTRINSIC_END;
}

__intrinsic
__xread mem_lock_out_t *cmd_mem_lock256_ptr40(
    __addr40 __mem __align16 mem_lock256_t      *mem_lock,
    __xwrite mem_lock_in_t                      *xfer,
    uint32_t                                    lsbzeroes,
    sync_t                                      sync,
    SIGNAL_PAIR                                 *sig_pair_ptr)
{
    _INTRINSIC_BEGIN;
    {
        generic_ind_t   ind;
        uint32_t        hi_addr, low_addr;
        _INTRINSIC_CONVERT_HI_LO_ADDRESS(mem_lock);
        CT_ASSERT(__is_write_reg(xfer));
        _INTRINSIC_OVERRIDE_LENGTH_AND_BYTEMASK_IN_ALU(ind, MEM_LOCK_REF_COUNT_256, lsbzeroes);
        __asm alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
        __asm mem[lock256, *xfer, hi_addr, <<8, low_addr], sig_done[*sig_pair_ptr], indirect_ref
    }
    return (__xread mem_lock_out_t *)(xfer);
    _INTRINSIC_END;
}

__intrinsic
__xread mem_lock_out_t *cmd_mem_lock384_ptr40(
    __addr40 __mem __align16 mem_lock384_t      *mem_lock,
    __xwrite mem_lock_in_t                      *xfer,
    uint32_t                                    lsbzeroes,
    sync_t                                      sync,
    SIGNAL_PAIR                                 *sig_pair_ptr)
{
    _INTRINSIC_BEGIN;
    {
        generic_ind_t   ind;
        uint32_t        hi_addr, low_addr;
        _INTRINSIC_CONVERT_HI_LO_ADDRESS(mem_lock);
        CT_ASSERT(__is_write_reg(xfer));
        _INTRINSIC_OVERRIDE_LENGTH_AND_BYTEMASK_IN_ALU(ind, MEM_LOCK_REF_COUNT_384, lsbzeroes);
        __asm alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
        __asm mem[lock384, *xfer, hi_addr, <<8, low_addr], sig_done[*sig_pair_ptr], indirect_ref
    }
    return (__xread mem_lock_out_t *)(xfer);
    _INTRINSIC_END;
}

__intrinsic
__xread mem_lock_out_t *cmd_mem_lock512_ptr40(
    __addr40 __mem __align16 mem_lock512_t      *mem_lock,
    __xwrite mem_lock_in_t                      *xfer,
    uint32_t                                    lsbzeroes,
    sync_t                                      sync,
    SIGNAL_PAIR                                 *sig_pair_ptr)
{
    _INTRINSIC_BEGIN;
    {
        generic_ind_t   ind;
        uint32_t        hi_addr, low_addr;
        _INTRINSIC_CONVERT_HI_LO_ADDRESS(mem_lock);
        CT_ASSERT(__is_write_reg(xfer));
        _INTRINSIC_OVERRIDE_LENGTH_AND_BYTEMASK_IN_ALU(ind, MEM_LOCK_REF_COUNT_512, lsbzeroes);
        __asm alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
        __asm mem[lock512, *xfer, hi_addr, <<8, low_addr], sig_done[*sig_pair_ptr], indirect_ref
    }
    return (__xread mem_lock_out_t *)(xfer);
    _INTRINSIC_END;
}


__intrinsic
void cmd_mem_unlock128_ptr32(
    __addr32 __mem __align16 mem_lock128_t *mem_lock,
    uint32_t index,
    sync_t sync,
    SIGNAL *sig_ptr
    )
{
    _INTRINSIC_BEGIN;
    {
        __xwrite uint32_t xfer = 0;
        cmd_mem_write_atomic_ptr32(&xfer,(__addr32 __mem void *)(((__addr32 __mem uint32_t *)mem_lock) + index), 1, sync, sig_ptr);
    }
    _INTRINSIC_END;
}

__intrinsic
void cmd_mem_unlock256_ptr32(
    __addr32 __mem __align16 mem_lock256_t *mem_lock,
    uint32_t index,
    sync_t sync,
    SIGNAL *sig_ptr
    )
{
    _INTRINSIC_BEGIN;
    {
        __xwrite uint32_t xfer = 0;
        cmd_mem_write_atomic_ptr32(&xfer,(__addr32 __mem void *)(((__addr32 __mem uint32_t *)mem_lock) + index), 1, sync, sig_ptr);
    }
    _INTRINSIC_END;
}

__intrinsic
void cmd_mem_unlock384_ptr32(
    __addr32 __mem __align16 mem_lock384_t *mem_lock,
    uint32_t index,
    sync_t sync,
    SIGNAL *sig_ptr
    )
{
    _INTRINSIC_BEGIN;
    {
        __xwrite uint32_t xfer = 0;
        cmd_mem_write_atomic_ptr32(&xfer,(__addr32 __mem void *)(((__addr32 __mem uint32_t *)mem_lock) + index), 1, sync, sig_ptr);
    }
    _INTRINSIC_END;
}

__intrinsic
void cmd_mem_unlock512_ptr32(
    __addr32 __mem __align16 mem_lock512_t *mem_lock,
    uint32_t index,
    sync_t sync,
    SIGNAL *sig_ptr
    )
{
    _INTRINSIC_BEGIN;
    {
        __xwrite uint32_t xfer = 0;
        cmd_mem_write_atomic_ptr32(&xfer,(__addr32 __mem void *)(((__addr32 __mem uint32_t *)mem_lock) + index), 1, sync, sig_ptr);
    }
    _INTRINSIC_END;
}


__intrinsic
void cmd_mem_unlock128_ptr40(
    __addr40 __mem __align16 mem_lock128_t  *mem_lock,
    uint32_t                                index,
    sync_t                                  sync,
    SIGNAL                                  *sig_ptr
    )
{
    _INTRINSIC_BEGIN;
    {
        __xwrite uint32_t xfer = 0;
        cmd_mem_write_atomic_ptr40(&xfer,(__addr40 __mem void *)(((__addr40 __mem uint32_t *)mem_lock) + index), 1, sync, sig_ptr);
    }
    _INTRINSIC_END;
}

__intrinsic
void cmd_mem_unlock256_ptr40(
    __addr40 __mem __align16 mem_lock256_t  *mem_lock,
    uint32_t                                index,
    sync_t                                  sync,
    SIGNAL                                  *sig_ptr
    )
{
    _INTRINSIC_BEGIN;
    {
        __xwrite uint32_t xfer = 0;
        cmd_mem_write_atomic_ptr40(&xfer,(__addr40 __mem void *)(((__addr40 __mem uint32_t *)mem_lock) + index), 1, sync, sig_ptr);
    }
    _INTRINSIC_END;
}

__intrinsic
void cmd_mem_unlock384_ptr40(
    __addr40 __mem __align16 mem_lock384_t  *mem_lock,
    uint32_t                                index,
    sync_t                                  sync,
    SIGNAL                                  *sig_ptr
    )
{
    _INTRINSIC_BEGIN;
    {
        __xwrite uint32_t xfer = 0;
        cmd_mem_write_atomic_ptr40(&xfer,(__addr40 __mem void *)(((__addr40 __mem uint32_t *)mem_lock) + index), 1, sync, sig_ptr);
    }
    _INTRINSIC_END;
}

__intrinsic
void cmd_mem_unlock512_ptr40(
    __addr40 __mem __align16 mem_lock512_t  *mem_lock,
    uint32_t                                index,
    sync_t                                  sync,
    SIGNAL                                  *sig_ptr
    )
{
    _INTRINSIC_BEGIN;
    {
        __xwrite uint32_t xfer = 0;
        cmd_mem_write_atomic_ptr40(&xfer,(__addr40 __mem void *)(((__addr40 __mem uint32_t *)mem_lock) + index), 1, sync, sig_ptr);
    }
    _INTRINSIC_END;
}


#endif /* __NFP_MEM_LOCK_C__ */
