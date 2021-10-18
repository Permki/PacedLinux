/*
 * Copyright (C) 2008-2014 Netronome Systems, Inc.  All rights reserved.
 */

#ifndef __NFP_MEM_RING_C__
#define __NFP_MEM_RING_C__

#include <nfp.h>
#include <nfp_intrinsic.h>
#include <nfp_mem.h>
#include <nfp_mem_ring.h>

#ifndef NFP_LIB_ANY_NFCC_VERSION
    #if (!defined(__NFP_TOOL_NFCC) ||                       \
        (__NFP_TOOL_NFCC < NFP_SW_VERSION(5, 0, 0, 0)) ||   \
        (__NFP_TOOL_NFCC > NFP_SW_VERSION(6, 1, 0, 255)))
        #error "This standard library is not supported for the version of the SDK currently in use."
    #endif
#endif

#pragma diag_suppress 279  /* ignore while(0) etc in macros */

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

#define _MEM_RING_IMMEDIATE_WORD_CHECK(_word)                           \
        if (__is_ct_const(_word))                                       \
        {                                                               \
            CT_ASSERT(_word < (1 << 10));                               \
        }                                                               \
        else                                                            \
        {                                                               \
            RT_RANGE_ASSERT(_word < (1 << 10));                         \
        }

#define _MEM_RING_ALIGN_16_CHECK(addr)                                  \
        if (__is_ct_const(addr))                                        \
        {                                                               \
            CT_ASSERT((addr & 0xf) == 0);                               \
        }                                                               \
        else                                                            \
        {                                                               \
            RT_ALIGN_ASSERT((addr & 0xf) == 0);                         \
        }

#define _MEM_RING_SIGNAL_CHECK(sig)                                     \
        if (__is_ct_const(sig))                                         \
        {                                                               \
            CT_ASSERT(sig == sig_done || sig == ctx_swap);              \
        }

#define _MEM_RING_SIGNAL_PAIR_CHECK(sig_pair)                           \
        if (__is_ct_const(sig_pair))                                    \
        {                                                               \
            CT_ASSERT(sig_pair == sig_done);                            \
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


#define _MEM_RING_NUMBER_OVERRIDE(ring_number)                          \
{                                                                       \
       generic_ind_t ind;                                               \
       _INTRINSIC_OVERRIDE_FULL_DATA_REF(ind, ring_number);             \
       __asm { alu[--, --, B, ALU_INDIRECT_TYPE(ind)] }                 \
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
void cmd_mem_ring_read_desc(
    uint32_t ring_number,
    mem_ring_desc_in_mem_t *mem_ring_desc_in_mem
)
{
    _INTRINSIC_BEGIN;
    {
        uint32_t hi_addr, low_addr;
        _INTRINSIC_CONVERT_HI_LO_ADDRESS(mem_ring_desc_in_mem);
        _MEM_RING_NUMBER_CHECK(ring_number);
        _MEM_RING_ALIGN_16_CHECK((uint32_t)mem_ring_desc_in_mem);
        {
            _MEM_RING_NUMBER_OVERRIDE(ring_number & 0x3ff);
            __asm mem[rd_qdesc, --, hi_addr, <<8, low_addr], indirect_ref
        }
    }
    _INTRINSIC_END;
}

__intrinsic
void cmd_mem_ring_write_desc(
    uint32_t ring_number,
    mem_ring_desc_in_mem_t *mem_ring_desc_in_mem
)
{
    _INTRINSIC_BEGIN;
    {
        uint32_t hi_addr, low_addr;
        _INTRINSIC_CONVERT_HI_LO_ADDRESS(mem_ring_desc_in_mem);
        _MEM_RING_NUMBER_CHECK(ring_number);
        _MEM_RING_ALIGN_16_CHECK((uint32_t)mem_ring_desc_in_mem);
        {
            _MEM_RING_NUMBER_OVERRIDE(ring_number & 0x3ff);
            __asm mem[wr_qdesc, --, hi_addr, <<8, low_addr], indirect_ref
        }
    }
    _INTRINSIC_END;
}


__intrinsic
void cmd_mem_ring_push_desc(
    uint32_t ring_number,
    __xread void *xfer,
    sync_t sync,
    SIGNAL *sig_ptr
)
{
    _INTRINSIC_BEGIN;
    {
        __gpr uint32_t ring_index;
        uint32_t hi_addr;
        _MEM_RING_NUMBER_CHECK(ring_number);
        _MEM_RING_CONVERT_RING_TO_HI_ADDRESS(ring_number);

        CT_ASSERT(__is_read_reg(xfer));
        __implicit_write(xfer); /* Ensure xfer initialisations are not skipped in caller */

        if (sync == sig_done)
        {
            __asm
            {
                mem[push_qdesc, *xfer, hi_addr, << 8, ring_index ], sig_done[*sig_ptr]
            }
        }
        else
        {
            __asm
            {
                mem[push_qdesc, *xfer, hi_addr, << 8, ring_index ], ctx_swap[*sig_ptr]
            }
        }
    }
    _INTRINSIC_END;
}


#define _MEM_RING_IMPLEMENT_READ_WRITE_QUEUE_COMMAND(command, is_read)                                                      \
{                                                                                                                           \
        __gpr uint32_t address;                                                                                             \
        uint32_t hi_addr, ring_index;                                                                                       \
        _MEM_RING_NUMBER_CHECK(ring_number);                                                                                \
        if (is_read == 1) {                                                                                                 \
            CT_ASSERT(__is_read_reg(xfer));                                                                                 \
        }                                                                                                                   \
        else                                                                                                                \
        {                                                                                                                   \
            CT_ASSERT(__is_write_reg(xfer));                                                                                \
        }                                                                                                                   \
        _MEM_RING_COUNT_CHECK(count);                                                                                       \
        _MEM_RING_SIGNAL_CHECK(sync);                                                                                       \
        _MEM_RING_CONVERT_RING_TO_HI_ADDRESS(ring_number);                                                                  \
        if (__is_ct_const(offset))                                                                                          \
        {                                                                                                                   \
            CT_ASSERT(offset <= 0xfffff);                                                                                   \
            address = (ring_index << 22) | (offset << 2);                                                                   \
        }                                                                                                                   \
        else                                                                                                                \
        {                                                                                                                   \
            address = (ring_index << 22) | ((offset & 0xfffff) << 2);                                                       \
        }                                                                                                                   \
        if (sync == sig_done)                                                                                               \
        {                                                                                                                   \
            if (__is_ct_const(count) && count <= 8)                                                                         \
            {                                                                                                               \
                __asm mem[command, *xfer, hi_addr, << 8, address, __ct_const_val(count)], sig_done[*sig_ptr]                \
            }                                                                                                               \
            else                                                                                                            \
            {                                                                                                               \
                _INTRINSIC_IND_ALU_COUNT_MIN_MAX(count, 1, 16);                                                             \
                __asm mem[command, *xfer, hi_addr, << 8, address, __ct_const_val(count)], sig_done[*sig_ptr], indirect_ref  \
            }                                                                                                               \
        }                                                                                                                   \
        else                                                                                                                \
        {                                                                                                                   \
            if (__is_ct_const(count) && count <= 8)                                                                         \
            {                                                                                                               \
                __asm mem[command, *xfer, hi_addr, << 8, address, __ct_const_val(count)], ctx_swap[*sig_ptr]                \
            }                                                                                                               \
            else                                                                                                            \
            {                                                                                                               \
                _INTRINSIC_IND_ALU_COUNT_MIN_MAX(count, 1, 16);                                                             \
                __asm mem[command, *xfer, hi_addr, << 8, address, __ct_const_val(count)], ctx_swap[*sig_ptr], indirect_ref  \
            }                                                                                                               \
        }                                                                                                                   \
    }


__intrinsic
void cmd_mem_ring_read_buffer_unbounded(
    uint32_t ring_number,
    __xread uint32_t *xfer,
    uint32_t count,
    uint32_t offset,
    sync_t sync,
    SIGNAL *sig_ptr
)
{
    _INTRINSIC_BEGIN;
    {
        _MEM_RING_IMPLEMENT_READ_WRITE_QUEUE_COMMAND(read_queue, 1);
    }
    _INTRINSIC_END;
}

__intrinsic
void cmd_mem_ring_read_buffer(
    uint32_t ring_number,
    __xread uint32_t *xfer,
    uint32_t count,
    uint32_t offset,
    sync_t sync,
    SIGNAL *sig_ptr
)
{
    _INTRINSIC_BEGIN;
    {
        _MEM_RING_IMPLEMENT_READ_WRITE_QUEUE_COMMAND(read_queue_ring, 1);
    }
    _INTRINSIC_END;
}

__intrinsic
void cmd_mem_ring_write_buffer_unbounded(
    uint32_t ring_number,
    __xwrite void *xfer,
    uint32_t count,
    uint32_t offset,
    sync_t sync,
    SIGNAL *sig_ptr
)
{
    _INTRINSIC_BEGIN;
    {
        _MEM_RING_IMPLEMENT_READ_WRITE_QUEUE_COMMAND(write_queue, 0);
    }
    _INTRINSIC_END;
}

__intrinsic
void cmd_mem_ring_write_buffer(
    uint32_t ring_number,
    __xwrite void *xfer,
    uint32_t count,
    uint32_t offset,
    sync_t sync,
    SIGNAL *sig_ptr
)
{
    _INTRINSIC_BEGIN;
    {
        _MEM_RING_IMPLEMENT_READ_WRITE_QUEUE_COMMAND(write_queue_ring, 0);
    }
    _INTRINSIC_END;
}


__intrinsic
void cmd_mem_ring_add_tail(
    uint32_t ring_number,
    uint32_t count
)
{
    _INTRINSIC_BEGIN;
    {
        uint32_t hi_addr, ring_index;

        _MEM_RING_NUMBER_CHECK(ring_number);
        _MEM_RING_CONVERT_RING_TO_HI_ADDRESS(ring_number);

        if (__is_ct_const(count))
        {
            CT_ASSERT(count < (1 << 24));
        }
        {
            _MEM_RING_NUMBER_OVERRIDE(ring_index);
            __asm mem[add_tail, --, hi_addr, << 8, count], indirect_ref
        }
    }
    _INTRINSIC_END;
}

__intrinsic
__xread mem_ring_put_status_t* cmd_mem_ring_put_buffer(
    uint32_t ring_number,
    __xrw void *xfer,
    uint32_t count,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
)
{
    _INTRINSIC_BEGIN;
    {
        uint32_t hi_addr, ring_index;

        _MEM_RING_NUMBER_CHECK(ring_number);
        CT_ASSERT(__is_write_reg(xfer));
        _MEM_RING_COUNT_CHECK(count);
        _MEM_RING_SIGNAL_PAIR_CHECK(sync);
        __implicit_read(xfer); /* Ensure xfer initialisations are not skipped in caller */

        _MEM_RING_CONVERT_RING_TO_HI_ADDRESS(ring_number);

        if (__is_ct_const(count) && count <= 8)
        {
            __asm mem[put, *xfer, hi_addr, << 8, ring_index, __ct_const_val(count)], sig_done[*__ct_sig_pair(sig_pair_ptr)]
        }
        else
        {
            _INTRINSIC_IND_ALU_COUNT_MIN_MAX(count, 1, 16);
            __asm mem[put, *xfer, hi_addr, << 8, ring_index, __ct_const_val(count)], sig_done[*__ct_sig_pair(sig_pair_ptr)], indirect_ref
        }
    }
    return (__xread mem_ring_put_status_t *)(xfer);
    _INTRINSIC_END;
}

__intrinsic
void cmd_mem_ring_journal_buffer(
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
        _MEM_RING_CONVERT_RING_TO_HI_ADDRESS(ring_number);

        if (sync == sig_done)
        {
            if (__is_ct_const(count) && count <= 8)
            {
                __asm mem[journal, *xfer, hi_addr, << 8, ring_index, __ct_const_val(count)], sig_done[*__ct_sig_pair(sig_ptr)]
            }
            else
            {
                _INTRINSIC_IND_ALU_COUNT_MIN_MAX(count, 1, 16);
                __asm mem[journal, *xfer, hi_addr, << 8, ring_index, __ct_const_val(count)], sig_done[*__ct_sig_pair(sig_ptr)], indirect_ref
            }
        }
        else
        {
            if (__is_ct_const(count) && count <= 8)
            {
                __asm mem[journal, *xfer, hi_addr, << 8, ring_index, __ct_const_val(count)], ctx_swap[*__ct_sig_pair(sig_ptr)]
            }
            else
            {
                _INTRINSIC_IND_ALU_COUNT_MIN_MAX(count, 1, 16);
                __asm mem[journal, *xfer, hi_addr, << 8, ring_index, __ct_const_val(count)], ctx_swap[*__ct_sig_pair(sig_ptr)], indirect_ref
            }
        }
    }
    _INTRINSIC_END;
}


#define _MEM_RING_IMPLEMENT_RING_GET_POP_COMMAND(command)                                                                                       \
{                                                                                                                                               \
        uint32_t hi_addr, ring_index;                                                                                                           \
        _MEM_RING_NUMBER_CHECK(ring_number);                                                                                                    \
        CT_ASSERT(__is_read_reg(xfer));                                                                                                         \
        _MEM_RING_COUNT_CHECK(count);                                                                                                           \
        _MEM_RING_SIGNAL_PAIR_CHECK(sync);                                                                                                      \
        _MEM_RING_CONVERT_RING_TO_HI_ADDRESS(ring_number);                                                                                      \
        if (__is_ct_const(count) && count <= 8)                                                                                                 \
        {                                                                                                                                       \
            __asm mem[command, *xfer,  hi_addr, << 8, ring_index, __ct_const_val(count)], sig_done[*__ct_sig_pair(sig_pair_ptr)]                \
        }                                                                                                                                       \
        else                                                                                                                                    \
        {                                                                                                                                       \
            _INTRINSIC_IND_ALU_COUNT_MIN_MAX(count, 1, 16);                                                                                     \
            __asm mem[command, *xfer,  hi_addr, << 8, ring_index, __ct_const_val(count)], sig_done[*__ct_sig_pair(sig_pair_ptr)], indirect_ref  \
        }                                                                                                                                       \
}


__intrinsic
void cmd_mem_ring_get_buffer(
    uint32_t ring_number,
    __xread uint32_t *xfer,
    uint32_t count,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
)
{
    _INTRINSIC_BEGIN;
    {
        _MEM_RING_IMPLEMENT_RING_GET_POP_COMMAND(get);
    }
    _INTRINSIC_END;
}

__intrinsic
void cmd_mem_ring_get_buffer_eop(
    uint32_t ring_number,
    __xread uint32_t *xfer,
    uint32_t count,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
)
{
    _INTRINSIC_BEGIN;
    {
        _MEM_RING_IMPLEMENT_RING_GET_POP_COMMAND(get_eop);
    }
    _INTRINSIC_END;
}


__intrinsic
void cmd_mem_ring_get_buffer_freely(
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

        _MEM_RING_NUMBER_CHECK(ring_number);
        CT_ASSERT(__is_read_reg(xfer));
        _MEM_RING_COUNT_CHECK(count);
        _MEM_RING_SIGNAL_CHECK(sync);

        _MEM_RING_CONVERT_RING_TO_HI_ADDRESS(ring_number);

        if (sync == sig_done)
        {
            if (__is_ct_const(count) && count <= 8)
            {
                __asm mem[get_freely, *xfer, hi_addr, << 8, ring_index, __ct_const_val(count)], sig_done[*sig_ptr]
            }
            else
            {
                _INTRINSIC_IND_ALU_COUNT_MIN_MAX(count, 1, 16);
                __asm mem[get_freely, *xfer, hi_addr, << 8, ring_index, __ct_const_val(count)], sig_done[*sig_ptr], indirect_ref
            }
        }
        else
        {
            if (__is_ct_const(count) && count <= 8)
            {
                __asm mem[get_freely, *xfer, hi_addr, << 8, ring_index, __ct_const_val(count)], ctx_swap[*sig_ptr]
            }
            else
            {
                _INTRINSIC_IND_ALU_COUNT_MIN_MAX(count, 1, 16);
                __asm mem[get_freely, *xfer, hi_addr, << 8, ring_index, __ct_const_val(count)], ctx_swap[*sig_ptr], indirect_ref
            }
        }
    }
    _INTRINSIC_END;
}


__intrinsic
void cmd_mem_ring_pop_buffer(
    uint32_t ring_number,
    __xread uint32_t *xfer,
    uint32_t count,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
)
{
    _INTRINSIC_BEGIN;
    {
        _MEM_RING_IMPLEMENT_RING_GET_POP_COMMAND(pop);
    }
    _INTRINSIC_END;
}

__intrinsic
void cmd_mem_ring_pop_buffer_eop(
    uint32_t ring_number,
    __xread uint32_t *xfer,
    uint32_t count,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
)
{
    _INTRINSIC_BEGIN;
    {
        _MEM_RING_IMPLEMENT_RING_GET_POP_COMMAND(pop_eop);
    }
    _INTRINSIC_END;
}


__intrinsic
void cmd_mem_ring_pop_buffer_freely(
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

        _MEM_RING_NUMBER_CHECK(ring_number);
        CT_ASSERT(__is_read_reg(xfer));
        _MEM_RING_COUNT_CHECK(count);
        _MEM_RING_SIGNAL_CHECK(sync);

        _MEM_RING_CONVERT_RING_TO_HI_ADDRESS(ring_number);

        if (sync == sig_done)
        {
            if (__is_ct_const(count) && count <= 8)
            {
                __asm mem[pop_freely, *xfer, hi_addr, << 8, ring_index, __ct_const_val(count)], sig_done[*sig_ptr]
            }
            else
            {
                _INTRINSIC_IND_ALU_COUNT_MIN_MAX(count, 1, 16);
                __asm mem[pop_freely, *xfer, hi_addr, << 8, ring_index, __ct_const_val(count)], sig_done[*sig_ptr], indirect_ref
            }
        }
        else
        {
            if (__is_ct_const(count) && count <= 8)
            {
                __asm mem[pop_freely, *xfer, hi_addr, << 8, ring_index, __ct_const_val(count)], ctx_swap[*sig_ptr]
            }
            else
            {
                _INTRINSIC_IND_ALU_COUNT_MIN_MAX(count, 1, 16);
                __asm mem[pop_freely, *xfer, hi_addr, << 8, ring_index, __ct_const_val(count)], ctx_swap[*sig_ptr], indirect_ref
            }
        }
    }
    _INTRINSIC_END;
}


__intrinsic
void cmd_mem_ring_fastjournal_imm(
    uint32_t ring_number,
    uint32_t longword
)
{
    _INTRINSIC_BEGIN;
    {
            uint32_t hi_addr, ring_index;
            _MEM_RING_CONVERT_RING_TO_HI_ADDRESS(ring_number);
            _MEM_RING_NUMBER_CHECK(ring_number);
        {
            generic_ind_t ind;
            _INTRINSIC_OVERRIDE_DATA_IN_ALU(ind, ring_index);
            __asm
            {
                alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
                mem[fast_journal, --, hi_addr, <<8, longword, 1], indirect_ref
            }
        }

    }
    _INTRINSIC_END;
}




/*
* Setup the descriptor data in emem
* Wait for descriptor to get to emem by reading it back
*
* Setup the Q descriptor for ring_num
*/
#define MEM_RING_DESC_TO_APPLICABLE_ISLAND(base_address)                                                        \
{                                                                                                               \
    {                                                                                                           \
        __addr40 __mem mem_ring_desc_in_mem_t *ptr = (__addr40 __mem mem_ring_desc_in_mem_t*)&base_address;     \
        __xread uint32_t read_back[4];                                                                          \
        cmd_mem_write_atomic_ptr40((void*)&mem_ring_desc, ptr , 4, ctx_swap, &sig);                             \
        cmd_mem_read_atomic_ptr40((void*)&read_back, ptr , 4, ctx_swap, &sig);                                  \
    }                                                                                                           \
    {                                                                                                           \
        mem_ring_desc_in_mem_t  *mem_ring_desc_in_mem = (mem_ring_desc_in_mem_t *)&base_address;                \
        cmd_mem_ring_read_desc(ring_number, mem_ring_desc_in_mem);                                              \
    }                                                                                                           \
}

__intrinsic
void cmd_mem_ring_init(
    uint32_t ring_number,
    enum MEM_RING_SIZE size,
    __addr40 __mem uint32_t *head,
    __addr40 __mem uint32_t *tail,
    uint32_t count
    )
{
    uint32_t                                        island_offset = (ring_number >> 10);
    volatile __xwrite mem_ring_desc_t      mem_ring_desc;           /* Ring descriptor */
    SIGNAL                                              sig;

    _MEM_RING_NUMBER_CHECK(ring_number);
    if (__is_ct_const(island_offset))
    {
        #if !(_nfp_has_island("i24.mem"))
            if (island_offset == 0)
            {
                CT_ASSERT((island_offset) == 24);       // i24.emem is not supported
            }
        #endif
        #if (!_nfp_has_island("i25.mem"))
            if (island_offset == 1)
            {
                CT_ASSERT((island_offset) == 25);       // i25.emem is not supported
            }
        #endif
        #if !(_nfp_has_island("i26.mem"))
            if (island_offset == 2)
            {
                CT_ASSERT((island_offset) == 26);       // i26.emem is not supported
            }
        #endif
    }
    else
    {
        #if !(_nfp_has_island("i24.mem"))
            if (island_offset == 0)
            {
                RT_ASSERT((island_offset) == 24);
            }
        #endif
        #if (!_nfp_has_island("i25.mem"))
            if (island_offset == 1)
            {
                RT_ASSERT((island_offset) == 25);
            }
        #endif
        #if !(_nfp_has_island("i26.mem"))
            if (island_offset == 2)
            {
                RT_ASSERT((island_offset) == 26);
            }
        #endif
    }

    /*
    * Clear and configure descriptor
    */

    mem_ring_desc.value[0] = ((((uint32_t)head >> 2) & 0xffffff) << 2) | (size << 28);
    mem_ring_desc.value[1] =  (((uint32_t)tail >> 2) << 2) | 2 ;
    mem_ring_desc.value[2] = count;
    mem_ring_desc.value[3] = 0;

    /* ring number between 0 and 1023 means first emem island (i24) */
    if (island_offset == 0)
    {
    #if (_nfp_has_island("i24.mem"))
        __emem_n(0) __addr40 __align16 mem_ring_desc_t      emem_addr;  // i24
        MEM_RING_DESC_TO_APPLICABLE_ISLAND(emem_addr) ;
    #endif
    }
    else if (island_offset == 1)      /* ring number between 1023 and 2046 means second emem island (i25) */
    {
    #if (_nfp_has_island("i25.mem"))
        __emem_n(1) __addr40 __align16 mem_ring_desc_t      emem_addr;  // i25
        MEM_RING_DESC_TO_APPLICABLE_ISLAND(emem_addr) ;
    #endif
    }
    else if (island_offset == 2)     /* ring number between 2046 and 3069 means second emem island (i26) */
    {
    #if (_nfp_has_island("i26.mem"))
        __emem_n(2) __addr40 __align16 mem_ring_desc_t      emem_addr;  // i26
        MEM_RING_DESC_TO_APPLICABLE_ISLAND(emem_addr) ;
    #endif
    }
}

__intrinsic
void cmd_mem_ring_init_with_loc_and_page(
    uint32_t ring_number,
    enum MEM_RING_SIZE size,
    __addr40 __mem uint32_t *head,
    __addr40 __mem uint32_t *tail,
    uint32_t count,
    uint32_t locality,
    uint32_t page
    )
{
    SIGNAL                                      sig;
    volatile __xwrite mem_ring_desc_t       mem_ring_desc;           /* Ring descriptor */
    uint32_t island_offset = (ring_number >> 10);

    _MEM_RING_NUMBER_CHECK(ring_number);

    if (__is_ct_const(island_offset))
    {
        #if !(_nfp_has_island("i24.mem"))
            if (island_offset == 0)
            {
                CT_ASSERT((island_offset) == 24);       // i24.emem is not supported
            }
        #endif
        #if (!_nfp_has_island("i25.mem"))
            if (island_offset == 1)
            {
                CT_ASSERT((island_offset) == 25);       // i25.emem is not supported
            }
        #endif
        #if !(_nfp_has_island("i26.mem"))
            if (island_offset == 2)
            {
                CT_ASSERT((island_offset) == 26);       // i26.emem is not supported
            }
        #endif
    }
    else
    {
        #if !(_nfp_has_island("i24.mem"))
            if (island_offset == 0)
            {
                RT_ASSERT((island_offset) == 24);
            }
        #endif
        #if (!_nfp_has_island("i25.mem"))
            if (island_offset == 1)
            {
                RT_ASSERT((island_offset) == 25);
            }
        #endif
        #if !(_nfp_has_island("i26.mem"))
            if (island_offset == 2)
            {
                RT_ASSERT((island_offset) == 26);
            }
        #endif
    }
    /*
     * Clear and configure descriptor
     */

    mem_ring_desc.value[0] = ((((uint32_t)head >> 2) & 0xffffff) << 2) | (size << 28);
    mem_ring_desc.value[1] =  (((uint32_t)tail >> 2) << 2) | 2 ;
    mem_ring_desc.value[2] = count | (locality << 30) | (page << 24);
    mem_ring_desc.value[3] = 0;

    /* ring number between 0 and 1023 means first emem island (i24) */
    if (island_offset == 0)
    {
    #if (_nfp_has_island("i24.mem"))
        __emem_n(0) __addr40 __align16 mem_ring_desc_t      emem_addr;  // i24
        MEM_RING_DESC_TO_APPLICABLE_ISLAND(emem_addr) ;
    #endif
    }
    else if (island_offset == 1)      /* ring number between 1023 and 2046 means second emem island (i25) */
    {
    #if (_nfp_has_island("i25.mem"))
        __emem_n(1) __addr40 __align16 mem_ring_desc_t      emem_addr;  // i25
        MEM_RING_DESC_TO_APPLICABLE_ISLAND(emem_addr) ;
    #endif
    }
    else   /* ring number between 2046 and 3069 means second emem island (i26) */
    {
    #if (_nfp_has_island("i26.mem"))
        __emem_n(2) __addr40 __align16 mem_ring_desc_t      emem_addr;  // i26
        MEM_RING_DESC_TO_APPLICABLE_ISLAND(emem_addr) ;
    #endif
    }
}

#pragma diag_default 279  /* suppressed above */

#endif /* __NFP_MEM_RING_C__ */
