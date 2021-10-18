/*
 * Copyright (C) 2008-2014 Netronome Systems, Inc.  All rights reserved.
 */

#ifndef __NFP_MEM_WORKQ_C__
#define __NFP_MEM_WORKQ_C__

#include <nfp.h>
#include <nfp_intrinsic.h>
#include <nfp_mem.h>
#include <nfp_mem_workq.h>

#ifndef NFP_LIB_ANY_NFCC_VERSION
    #if (!defined(__NFP_TOOL_NFCC) ||                       \
        (__NFP_TOOL_NFCC < NFP_SW_VERSION(5, 0, 0, 0)) ||   \
        (__NFP_TOOL_NFCC > NFP_SW_VERSION(6, 1, 0, 255)))
        #error "This standard library is not supported for the version of the SDK currently in use."
    #endif
#endif

/*
 * NOTE: When writing inline-asm, it's important not to use reserved words
 * for variables.  Common ones are 'a','b', 'csr', 'state', and
 * 'inp_state'.
 */


#define _MEM_RING_NUMBER_CHECK(ring_number)                             \
        if (__is_ct_const(ring_number))                                 \
        {                                                               \
            CT_ASSERT(ring_number < 3072);                              \
        }                                                               \
        else                                                            \
        {                                                               \
            RT_RANGE_ASSERT(ring_number < 3072);                        \
        }

#define _MEM_RING_COUNT_CHECK(count)                                    \
        if (__is_ct_const(count))                                       \
        {                                                               \
            CT_ASSERT(count >= 1 && count <= 16);                       \
        }                                                               \
        else                                                            \
        {                                                               \
            RT_RANGE_ASSERT(count >= 1 && count <= 16);                 \
        }

#define _MEM_RING_SIGNAL_CHECK(sig)                                     \
        if (__is_ct_const(sig))                                         \
        {                                                               \
            CT_ASSERT(sig == sig_done || sig == ctx_swap);              \
        }

#define _MEM_RING_ISLAND_CHECK(island)                                  \
        if (__is_ct_const(island))                                      \
        {                                                               \
            CT_ASSERT(island > 0 && island <= 2);                       \
        }                                                               \
        else                                                            \
        {                                                               \
            RT_RANGE_ASSERT(island > 0 && island <= 2);                 \
        }

#if (_nfp_has_island("i24.mem") && !_nfp_has_island("i25.mem") && !_nfp_has_island("i26.mem"))
#define _MEM_RING_CONVERT_RING_TO_HI_ADDRESS(number)                            \
{                                                                               \
    switch (number >> 10)                                                       \
    {                                                                           \
    case 0:                                                                     \
        hi_addr = (uint32_t)((uint64_t)LoadTimeConstant(i24_emem_base) >> 8);   \
        break;                                                                  \
    default:                                                                    \
        _MEM_RING_ISLAND_CHECK(number >> 10);                                   \
    }                                                                           \
    ring_index = number & 0x3ff;                                                \
}
#endif
#if (_nfp_has_island("i24.mem") && _nfp_has_island("i25.mem") && !_nfp_has_island("i26.mem"))
#define _MEM_RING_CONVERT_RING_TO_HI_ADDRESS(number)                            \
{                                                                               \
    switch (number >> 10)                                                       \
    {                                                                           \
    case 0:                                                                     \
        hi_addr = (uint32_t)((uint64_t)LoadTimeConstant(i24_emem_base) >> 8);   \
        break;                                                                  \
    case 1:                                                                     \
        hi_addr = (uint32_t)((uint64_t)LoadTimeConstant(i25_emem_base) >> 8);   \
        break;                                                                  \
    default:                                                                    \
        _MEM_RING_ISLAND_CHECK(number >> 10);                                   \
    }                                                                           \
    ring_index = number & 0x3ff;                                                \
}
#endif
#if  (_nfp_has_island("i24.mem") && _nfp_has_island("i25.mem") && _nfp_has_island("i26.mem"))
#define _MEM_RING_CONVERT_RING_TO_HI_ADDRESS(number)                            \
{                                                                               \
    switch (number >> 10)                                                       \
    {                                                                           \
    case 0:                                                                     \
        hi_addr = (uint32_t)((uint64_t)LoadTimeConstant(i24_emem_base) >> 8);   \
        break;                                                                  \
    case 1:                                                                     \
        hi_addr = (uint32_t)((uint64_t)LoadTimeConstant(i25_emem_base) >> 8);   \
        break;                                                                  \
    case 2:                                                                     \
        hi_addr = (uint32_t)((uint64_t)LoadTimeConstant(i26_emem_base) >> 8);   \
        break;                                                                  \
    default:                                                                    \
        _MEM_RING_ISLAND_CHECK(number >> 10);                                   \
    }                                                                           \
    ring_index = number & 0x3ff;                                                \
}
#endif


__intrinsic
void cmd_mem_workq_add_work(
    uint32_t ring_number,
    __xwrite void *xfer,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
)
{
    _INTRINSIC_BEGIN;
    {
        uint32_t hi_addr, ring_index;

        _MEM_RING_NUMBER_CHECK(ring_number);
        CT_ASSERT(__is_write_reg(xfer));
        _MEM_RING_COUNT_CHECK(count);
        _MEM_RING_SIGNAL_CHECK(sync);
        __implicit_read(xfer); /* Ensure xfer initialisations are not skipped in caller */
        _MEM_RING_CONVERT_RING_TO_HI_ADDRESS(ring_number);

        if (__is_ct_const(count) && count <= 8)
        {
            if (sync == sig_done)
            {
                __asm mem[qadd_work, *xfer, hi_addr, << 8, ring_index, __ct_const_val(count)], sig_done[*sig_ptr]
            }
            else
            {
                __asm mem[qadd_work, *xfer, hi_addr, << 8, ring_index, __ct_const_val(count)], ctx_swap[*sig_ptr]
            }
        }
        else
        {
            _INTRINSIC_IND_ALU_COUNT_MIN_MAX(count, 1, 16);
            if (sync == sig_done)
            {
                __asm mem[qadd_work, *xfer, hi_addr, << 8, ring_index, __ct_const_val(count)], sig_done[*sig_ptr], indirect_ref
            }
            else
            {
                __asm mem[qadd_work, *xfer, hi_addr, << 8, ring_index, __ct_const_val(count)], ctx_swap[*sig_ptr], indirect_ref
            }
        }
    }
    _INTRINSIC_END;
}


__intrinsic
void cmd_mem_workq_add_work_imm(
    uint32_t ring_number,
    uint32_t work_longword
)
{
    _INTRINSIC_BEGIN;

    {
        uint32_t hi_addr, ring_index;
        _MEM_RING_CONVERT_RING_TO_HI_ADDRESS(ring_number);
        {
            generic_ind_t ind;
            _INTRINSIC_OVERRIDE_DATA_IN_ALU(ind, ring_index);

            _MEM_RING_NUMBER_CHECK(ring_number);
            __asm
            {
                alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
                mem[qadd_work_imm, --, hi_addr, <<8, work_longword, 1], indirect_ref
            }
        }
    }


    _INTRINSIC_END;
}


__intrinsic
void cmd_mem_workq_add_thread(
    uint32_t ring_number,
    __xread uint32_t *xfer,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
)
{
    _INTRINSIC_BEGIN;
    {
        uint32_t hi_addr, ring_index;
        _MEM_RING_CONVERT_RING_TO_HI_ADDRESS(ring_number);

        _MEM_RING_NUMBER_CHECK(ring_number);
        CT_ASSERT(__is_read_reg(xfer));
        _MEM_RING_COUNT_CHECK(count);
        _MEM_RING_SIGNAL_CHECK(sync);
        if (__is_ct_const(count) && count <= 8)
        {
            if (sync == sig_done)
            {
                __asm mem[qadd_thread, *xfer, hi_addr, << 8, ring_index, __ct_const_val(count)], sig_done[*sig_ptr]
            }
            else
            {
                __asm mem[qadd_thread, *xfer, hi_addr, << 8, ring_index, __ct_const_val(count)], ctx_swap[*sig_ptr]
            }
        }
        else
        {
            _INTRINSIC_IND_ALU_COUNT_MIN_MAX(count, 1, 16);
            if (sync == sig_done)
            {
                __asm mem[qadd_thread, *xfer, hi_addr, << 8, ring_index, __ct_const_val(count)], sig_done[*sig_ptr], indirect_ref
            }
            else
            {
                __asm mem[qadd_thread, *xfer, hi_addr, << 8, ring_index, __ct_const_val(count)], ctx_swap[*sig_ptr], indirect_ref
            }
        }
    }
    _INTRINSIC_END;
}

#endif /* __NFP_MEM_WORKQ_C__ */
