/*
 * Copyright (C) 2008-2014 Netronome Systems, Inc.  All rights reserved.
 */

/* Extended mode Memory functions */

#ifndef __NFP_MEM_C__
#define __NFP_MEM_C__

#include <nfp.h>
#include <nfp_intrinsic.h>
#include <nfp_mem.h>

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


#define _MEM_IMMEDIATE_RANGE_CHECK_SIGNED(value, length, max_length)   \
    if (__is_ct_const(value))                                   \
    {                                                           \
        CT_ASSERT(value >= -0x8000 && value <= 0x7fff);         \
    }                                                           \
    else                                                        \
    {                                                           \
        RT_RANGE_ASSERT(value >= -0x8000 && value <= 0x7fff);   \
    }                                                           \
    if (__is_ct_const(length))                                  \
    {                                                           \
        CT_ASSERT(1 <= length && length <= max_length);         \
    }                                                           \
    else                                                        \
    {                                                           \
        RT_RANGE_ASSERT(1 <= length && length <= max_length);   \
    }

#define _MEM_IMMEDIATE_RANGE_CHECK_UNSIGNED(value, length, max_length)   \
    if (__is_ct_const(value))                                   \
    {                                                           \
        CT_ASSERT(value <= 0xffff);                             \
    }                                                           \
    else                                                        \
    {                                                           \
        RT_RANGE_ASSERT(value <= 0xffff);                       \
    }                                                           \
    if (__is_ct_const(length))                                  \
    {                                                           \
        CT_ASSERT(1 <= length && length <= max_length);         \
    }                                                           \
    else                                                        \
    {                                                           \
        RT_RANGE_ASSERT(1 <= length && length <= max_length);   \
    }


#define _MEM_IMPLEMENT_IMMEDIATE_COMMAND(command)                                                       \
{                                                                                                       \
     generic_ind_t ind;                                                                                 \
                                                                                                        \
    _INTRINSIC_OVERRIDE_IMM_DATA_AND_LENGTH(ind, (data & 0xffff), (((count - 1) & 7) | (1 << 3)));      \
                                                                                                        \
    __asm alu[--, --, B, ALU_INDIRECT_TYPE(ind)]                                                        \
    __asm   mem[command, --, address, 0], indirect_ref                                                  \
}


#define _MEM_IMPLEMENT_IMMEDIATE_COMMAND_PTR40(command)                                                 \
{                                                                                                       \
    generic_ind_t ind;                                                                                  \
    uint32_t hi_addr, low_addr;                                                                         \
    _INTRINSIC_CONVERT_HI_LO_ADDRESS(address);                                                          \
                                                                                                        \
    _INTRINSIC_OVERRIDE_IMM_DATA_AND_LENGTH(ind, (data & 0xffff), (((count - 1) & 7) | (1 << 3)));      \
    __asm alu[--, --, B, ALU_INDIRECT_TYPE(ind)]                                                        \
    __asm mem[command, --, hi_addr, <<8, low_addr], indirect_ref                                        \
}

#define _MEM_IMPLEMENT_IMMEDIATE_64BIT_COMMAND(command)                                                 \
{                                                                                                       \
     generic_ind_t ind;                                                                                 \
                                                                                                        \
    _INTRINSIC_OVERRIDE_IMM_DATA_AND_LENGTH(ind, (data & 0xffff), (((count - 1) & 7) | (1 << 3) | (1 << 2)));      \
                                                                                                        \
    __asm alu[--, --, B, ALU_INDIRECT_TYPE(ind)]                                                        \
    __asm   mem[command, --, address, 0], indirect_ref                                                  \
}


#define _MEM_IMPLEMENT_IMMEDIATE_64BIT_COMMAND_PTR40(command)                                           \
{                                                                                                       \
    generic_ind_t ind;                                                                                  \
    uint32_t hi_addr, low_addr;                                                                         \
    _INTRINSIC_CONVERT_HI_LO_ADDRESS(address);                                                          \
                                                                                                        \
    _INTRINSIC_OVERRIDE_IMM_DATA_AND_LENGTH(ind, (data & 0xffff), (((count - 1) & 7) | (1 << 3) | (1 << 2)));      \
    __asm alu[--, --, B, ALU_INDIRECT_TYPE(ind)]                                                        \
    __asm mem[command, --, hi_addr, <<8, low_addr], indirect_ref                                        \
}


/* Count override for 64 bit arithmetic*/
#define _MEM_ARITHMETIC_64_IND_ALU_COUNT_MIN_MAX(count, min_value, max_value)   \
    generic_ind_t ind;                                                          \
    if (__is_ct_const(count))                                                   \
    {                                                                           \
        CT_ASSERT((count) >= (min_value) && (count) <= (max_value));            \
    }                                                                           \
    else                                                                        \
    {                                                                           \
        RT_RANGE_ASSERT((count) >= (min_value) && (count) <= (max_value));      \
    }                                                                           \
    _INTRINSIC_OVERRIDE_LENGTH(ind, (count-1) | 0x04);                          \
    __asm { alu[--, --, B, ind.alu_ind.value] }


#ifdef __NFP_SINGLE_DRAM_SIGNAL
#define _MEM_IMPLEMENT_COMMAND(mem_opr, mem_fname, min_count, max_count)                                        \
    CT_ASSERT(__is_ct_const(sync));                                                                             \
    CT_ASSERT(sync == sig_done || sync == ctx_swap);                                                            \
    if (__is_ct_const(count) && count <= 8)                                                                     \
    {                                                                                                           \
        CT_ASSERT(count != 0);                                                                                  \
        if (sync == sig_done)                                                                                   \
        {                                                                                                       \
            __asm mem[mem_opr, *data, address, 0, __ct_const_val(count)], sig_done[*sig_ptr]                    \
        }                                                                                                       \
        else                                                                                                    \
        {                                                                                                       \
            __asm mem[mem_opr, *data, address, 0, __ct_const_val(count)], ctx_swap[*sig_ptr]                    \
        }                                                                                                       \
    }                                                                                                           \
    else                                                                                                        \
    {                                                                                                           \
        if (!__is_ct_const(count))                                                                              \
        {                                                                                                       \
            CT_QPERFINFO_INDIRECT_REF(mem_fname);                                                               \
        }                                                                                                       \
        {                                                                                                       \
            _INTRINSIC_IND_ALU_COUNT_MIN_MAX(count,min_count,max_count);                                        \
            if (sync == sig_done)                                                                               \
            {                                                                                                   \
                __asm mem[mem_opr, *data, address, 0, __ct_const_val(count)], sig_done[*sig_ptr], indirect_ref  \
            }                                                                                                   \
            else                                                                                                \
            {                                                                                                   \
                __asm mem[mem_opr, *data, address, 0, __ct_const_val(count)], ctx_swap[*sig_ptr], indirect_ref  \
            }                                                                                                   \
        }                                                                                                       \
    }

#define _MEM_IMPLEMENT_COMMAND_PTR40(mem_opr, mem_fname, min_count, max_count)                                  \
{                                                                                                               \
    uint32_t hi_addr, low_addr;                                                                                 \
    _INTRINSIC_CONVERT_HI_LO_ADDRESS(address);                                                                  \
    CT_ASSERT(__is_ct_const(sync));                                                                             \
    CT_ASSERT(sync == sig_done || sync == ctx_swap);                                                            \
    if (__is_ct_const(count) && count <= 8)                                                                     \
    {                                                                                                           \
        CT_ASSERT(count != 0);                                                                                  \
        if (sync == sig_done)                                                                                   \
        {                                                                                                       \
            __asm {mem[mem_opr, *data, hi_addr, <<8, low_addr, __ct_const_val(count)], sig_done[*sig_ptr]}      \
        }                                                                                                       \
        else                                                                                                    \
        {                                                                                                       \
            __asm {mem[mem_opr, *data, hi_addr, <<8, low_addr, __ct_const_val(count)], ctx_swap[*sig_ptr]}      \
    }                                                                                                           \
    }                                                                                                           \
    else                                                                                                        \
    {                                                                                                           \
        if (!__is_ct_const(count))                                                                              \
        {                                                                                                       \
            CT_QPERFINFO_INDIRECT_REF(mem_fname);                                                               \
        }                                                                                                       \
        {                                                                                                       \
            _INTRINSIC_IND_ALU_COUNT_MIN_MAX(count,min_count,max_count);                                        \
            if (sync == sig_done)                                                                               \
            {                                                                                                   \
                __asm mem[mem_opr, *data, hi_addr, <<8, low_addr, __ct_const_val(count)], sig_done[*sig_ptr], indirect_ref  \
            }                                                                                                   \
            else                                                                                                \
            {                                                                                                   \
                __asm mem[mem_opr, *data, hi_addr, <<8, low_addr, __ct_const_val(count)], ctx_swap[*sig_ptr], indirect_ref  \
            }                                                                                                   \
        }                                                                                                       \
    }                                                                                                           \
}


#define _MEM_IMPLEMENT_ARITHMETIC64_COMMAND(mem_opr, mem_fname, min_count, max_count)                           \
{                                                                                                               \
    CT_ASSERT(__is_ct_const(sync));                                                                             \
    CT_ASSERT(sync == sig_done || sync == ctx_swap);                                                            \
    if (__is_ct_const(count) && count <= 4)                                                                     \
    {                                                                                                           \
        CT_ASSERT(count != 0);                                                                                  \
        if (sync == sig_done)                                                                                   \
        {                                                                                                       \
            __asm mem[mem_opr, *data, address, 0, __ct_const_val(count)], sig_done[*sig_ptr]                    \
        }                                                                                                       \
        else                                                                                                    \
        {                                                                                                       \
            __asm mem[mem_opr, *data, address, 0, __ct_const_val(count)], ctx_swap[*sig_ptr]                    \
        }                                                                                                       \
    }                                                                                                           \
    else                                                                                                        \
    {                                                                                                           \
        if (!__is_ct_const(count))                                                                              \
        {                                                                                                       \
            CT_QPERFINFO_INDIRECT_REF(mem_fname);                                                               \
        }                                                                                                       \
        {                                                                                                       \
            _MEM_ARITHMETIC_64_IND_ALU_COUNT_MIN_MAX(count,min_count,max_count);                                \
            if (sync == sig_done)                                                                               \
            {                                                                                                   \
                __asm mem[mem_opr, *data, address, 0, __ct_const_val(count)], sig_done[*sig_ptr], indirect_ref  \
            }                                                                                                   \
            else                                                                                                \
            {                                                                                                   \
                __asm mem[mem_opr, *data, address, 0, __ct_const_val(count)], ctx_swap[*sig_ptr], indirect_ref  \
            }                                                                                                   \
        }                                                                                                       \
    }                                                                                                           \
}

#define _MEM_IMPLEMENT_ARITHMETIC64_COMMAND_PTR40(mem_opr, mem_fname, min_count, max_count)                     \
{                                                                                                               \
    uint32_t hi_addr, low_addr;                                                                                 \
    _INTRINSIC_CONVERT_HI_LO_ADDRESS(address);                                                                  \
    CT_ASSERT(__is_ct_const(sync));                                                                             \
    CT_ASSERT(sync == sig_done || sync == ctx_swap);                                                            \
    if (__is_ct_const(count) && count <= 4)                                                                     \
    {                                                                                                           \
        CT_ASSERT(count != 0);                                                                                  \
        if (sync == sig_done)                                                                                   \
        {                                                                                                       \
            __asm {mem[mem_opr, *data, hi_addr, <<8, low_addr, __ct_const_val(count)], sig_done[*sig_ptr]}      \
        }                                                                                                       \
        else                                                                                                    \
        {                                                                                                       \
            __asm {mem[mem_opr, *data, hi_addr, <<8, low_addr, __ct_const_val(count)], ctx_swap[*sig_ptr]}      \
        }                                                                                                       \
    }                                                                                                           \
    else                                                                                                        \
    {                                                                                                           \
        if (!__is_ct_const(count))                                                                              \
        {                                                                                                       \
            CT_QPERFINFO_INDIRECT_REF(mem_fname);                                                               \
        }                                                                                                       \
        {                                                                                                       \
            _MEM_ARITHMETIC_64_IND_ALU_COUNT_MIN_MAX(count,min_count,max_count);                                \
            if (sync == sig_done)                                                                               \
            {                                                                                                   \
                __asm mem[mem_opr, *data, hi_addr, <<8, low_addr, __ct_const_val(count)], sig_done[*sig_ptr], indirect_ref  \
            }                                                                                                   \
            else                                                                                                \
            {                                                                                                   \
                __asm mem[mem_opr, *data, hi_addr, <<8, low_addr, __ct_const_val(count)], ctx_swap[*sig_ptr], indirect_ref  \
            }                                                                                                   \
        }                                                                                                       \
    }                                                                                                           \
}

#endif  /* __NFP_SINGLE_DRAM_SIGNAL */


#ifdef __NFP_SINGLE_DRAM_SIGNAL
#define _MEM_IMPLEMENT_INDIRECT_COMMAND(mem_opr, min_count, max_count)                                          \
    CT_ASSERT(__is_ct_const(sync));                                                                             \
    CT_ASSERT(sync == sig_done || sync == ctx_swap);                                                            \
    CT_ASSERT(__is_ct_const(max_nn));                                                                           \
    CT_ASSERT(max_nn >= (min_count) && max_nn <= (max_count));                                                  \
    if (sync == sig_done)                                                                                       \
    {                                                                                                           \
        __asm { alu[--, --, B, ALU_INDIRECT_TYPE(ind)] }                                                        \
        __asm { mem[mem_opr, *data, address, 0, __ct_const_val(max_nn)], sig_done[*sig_ptr], indirect_ref }     \
    }                                                                                                           \
    else                                                                                                        \
    {                                                                                                           \
        __asm { alu[--, --, B, ALU_INDIRECT_TYPE(ind)] }                                                        \
        __asm { mem[mem_opr, *data, address, 0, __ct_const_val(max_nn)], ctx_swap[*sig_ptr], indirect_ref }     \
    }

#define _MEM_IMPLEMENT_INDIRECT_COMMAND_PTR40(mem_opr, min_count, max_count)                                    \
{                                                                                                               \
    uint32_t hi_addr, low_addr;                                                                                 \
    _INTRINSIC_CONVERT_HI_LO_ADDRESS(address);                                                                  \
    CT_ASSERT(__is_ct_const(sync));                                                                             \
    CT_ASSERT(sync == sig_done || sync == ctx_swap);                                                            \
    CT_ASSERT(__is_ct_const(max_nn));                                                                           \
    CT_ASSERT(max_nn >= (min_count) && max_nn <= (max_count));                                                  \
    if (sync == sig_done)                                                                                       \
    {                                                                                                           \
        __asm { alu[--, --, B, ALU_INDIRECT_TYPE(ind)] }                                                        \
        __asm { mem[mem_opr, *data, hi_addr, <<8, low_addr, __ct_const_val(max_nn)], sig_done[*sig_ptr], indirect_ref }     \
    }                                                                                                           \
    else                                                                                                        \
    {                                                                                                           \
        __asm { alu[--, --, B, ALU_INDIRECT_TYPE(ind)] }                                                        \
        __asm { mem[mem_opr, *data, hi_addr, <<8, low_addr, __ct_const_val(max_nn)], ctx_swap[*sig_ptr], indirect_ref }     \
    }                                                                                                           \
}
#endif

#define _MEM_MIN_MAX_CHECK(ref, min, max)                           \
        if (__is_ct_const(ref))                                     \
        {                                                           \
            CT_ASSERT(ref <= max && ref >= min);                    \
        }                                                           \
        else                                                        \
        {                                                           \
            RT_RANGE_ASSERT(ref <= max && ref >= min);              \
        }

#define _MEM_TEST_IMMEDIATE_RANGE_CHECK_SIGNED(value, length, max_length)   \
    if (__is_ct_const(data))                                                \
    {                                                                       \
        CT_ASSERT(-0x80 <= value && value <= 0x7f);                         \
    }                                                                       \
    else                                                                    \
    {                                                                       \
        RT_RANGE_ASSERT(-0x80 <= value && value <= 0x7f);                   \
    }                                                                       \
    if (__is_ct_const(count))                                               \
    {                                                                       \
        CT_ASSERT(1 <= length && length <= max_length);                     \
    }                                                                       \
    else                                                                    \
    {                                                                       \
        RT_RANGE_ASSERT(1 <= length && length <= max_length);               \
    }


#define _MEM_TEST_IMMEDIATE_RANGE_CHECK_UNSIGNED(value, length, max_length) \
    if (__is_ct_const(data))                                                \
    {                                                                       \
        CT_ASSERT(value <= 0x7f);                                           \
    }                                                                       \
    else                                                                    \
    {                                                                       \
        RT_RANGE_ASSERT(value <= 0x7f);                                     \
    }                                                                       \
    if (__is_ct_const(count))                                               \
    {                                                                       \
        CT_ASSERT(1 <= length && length <= max_length);                     \
    }                                                                       \
    else                                                                    \
    {                                                                       \
        RT_RANGE_ASSERT(1 <= length && length <= max_length);               \
    }


/* Maximum reference counts */
#define MAX_REF_COUNT_16 16     /* 16 for 64 bit operations 6xxx */
#define MAX_REF_COUNT_8  8      /* 8 for some operations 6xxx */
#define MAX_REF_COUNT_32 32     /* 32 for some operations 6xxx */




/* DRAM read */
__intrinsic
void cmd_mem_read64_ptr32(
    __xread void *data,                     /* returns data from operation          */
    volatile void __addr32 __mem *address,  /* address to read from                 */
    uint32_t count,                         /* number of quadwords to read          */
    sync_t sync,                            /* sig_done or ctx_swap                 */
    SIGNAL *sig_ptr                         /* signal to raise upon completion      */
)
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_read_reg(data));
        _MEM_IMPLEMENT_COMMAND(read, cmd_mem_read64_ptr32, 1, MAX_REF_COUNT_16);
    }
    _INTRINSIC_END;
}


__intrinsic
void cmd_mem_read64_ptr40(
    __xread void *data,                     /* returns data from operation          */
    volatile void __addr40 __mem *address,  /* address to read from                 */
    uint32_t count,                         /* number of quadwords to read          */
    sync_t sync,                            /* sig_done or ctx_swap                 */
    SIGNAL *sig_ptr                         /* signal to raise upon completion      */
)
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_read_reg(data));
        _MEM_IMPLEMENT_COMMAND_PTR40(read, cmd_mem_read64_ptr40, 1, MAX_REF_COUNT_16);
    }
    _INTRINSIC_END;
}



/* DRAM read indirect */
__intrinsic
void cmd_mem_read64_ind_ptr32(
    __xread void *data,                     /* returns data from operation          */
    volatile void __addr32 __mem *address,  /* address to read from                 */
            uint32_t max_nn,                /* max. number of quadwords to read     */
            generic_ind_t ind,              /* indirect word                        */
            sync_t sync,                    /* sig_done or ctx_swap                 */
            SIGNAL *sig_ptr                 /* signal to raise upon completion      */
    )
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_read_reg(data));
        __asm local_csr_wr[CMD_INDIRECT_REF_0, CSR_INDIRECT_TYPE(ind)]
        _MEM_IMPLEMENT_INDIRECT_COMMAND(read, 1, 16);
    }
    _INTRINSIC_END;
}


__intrinsic
void cmd_mem_read64_ind_ptr40(
    __xread void *data,                     /* returns data from operation          */
    volatile void __addr40 __mem *address,  /* address to read from                 */
    uint32_t max_nn,                        /* max. number of quadwords to read     */
    generic_ind_t ind,                      /* indirect word                        */
    sync_t sync,                            /* sig_done or ctx_swap                 */
    SIGNAL *sig_ptr                         /* signal to raise upon completion      */
    )
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_read_reg(data));
        __asm local_csr_wr[CMD_INDIRECT_REF_0, CSR_INDIRECT_TYPE(ind)]
        _MEM_IMPLEMENT_INDIRECT_COMMAND_PTR40(read, 1, 16);
    }
    _INTRINSIC_END;
}



/* DRAM read swap */
__intrinsic
void cmd_mem_read64_swap_ptr32(
    __xread void *data,                     /* returns data from operation          */
    volatile void __addr32 __mem *address,  /* address to read from                 */
    uint32_t count,                         /* number of quadwords to read          */
    sync_t sync,                            /* sig_done or ctx_swap                 */
    SIGNAL *sig_ptr                         /* signal to raise upon completion      */
    )
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_read_reg(data));
        _MEM_IMPLEMENT_COMMAND(read_swap, cmd_mem_read64_swap_ptr32, 1, MAX_REF_COUNT_16);
    }
    _INTRINSIC_END;
}


__intrinsic
void cmd_mem_read64_swap_ptr40(
    __xread void *data,                     /* returns data from operation          */
    volatile void __addr40 __mem *address,  /* address to read from                 */
    uint32_t count,                         /* number of quadwords to read          */
    sync_t sync,                            /* sig_done or ctx_swap                 */
    SIGNAL *sig_ptr                         /* signal to raise upon completion      */
    )
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_read_reg(data));
        _MEM_IMPLEMENT_COMMAND_PTR40(read_swap, cmd_mem_read64_swap_ptr40, 1, MAX_REF_COUNT_16);
    }
    _INTRINSIC_END;
}


/* DRAM read swap indirect */
__intrinsic
void cmd_mem_read64_swap_ind_ptr32(
    __xread void *data,                     /* returns data from operation          */
    volatile void __addr32 __mem *address,  /* address to read from                 */
    uint32_t max_nn,                        /* max. number of quadwords to read     */
    generic_ind_t ind,                      /* indirect word                        */
    sync_t sync,                            /* sig_done or ctx_swap                 */
    SIGNAL *sig_ptr                         /* signal to raise upon completion      */
    )
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_read_reg(data));
        __asm local_csr_wr[CMD_INDIRECT_REF_0, CSR_INDIRECT_TYPE(ind)]
        _MEM_IMPLEMENT_INDIRECT_COMMAND(read_swap, 1, 16);
    }
    _INTRINSIC_END;
}


__intrinsic
void cmd_mem_read64_swap_ind_ptr40(
    __xread void *data,                     /* returns data from operation          */
    volatile void __addr40 __mem *address,  /* address to read from                 */
    uint32_t max_nn,                        /* max. number of quadwords to read     */
    generic_ind_t ind,                      /* indirect word                        */
    sync_t sync,                            /* sig_done or ctx_swap                 */
    SIGNAL *sig_ptr                         /* signal to raise upon completion      */
    )
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_read_reg(data));
        __asm local_csr_wr[CMD_INDIRECT_REF_0, CSR_INDIRECT_TYPE(ind)]
        _MEM_IMPLEMENT_INDIRECT_COMMAND_PTR40(read_swap, 1, 16);
    }
    _INTRINSIC_END;
}


/* DRAM read swap le */
__intrinsic
void cmd_mem_read64_swap_le_ptr32(
    __xread void *data,                     /* returns data from operation          */
    volatile void __addr32 __mem *address,  /* address to read from                 */
    uint32_t count,                         /* number of quadwords to read          */
    sync_t sync,                            /* sig_done or ctx_swap                 */
    SIGNAL *sig_ptr                         /* signal to raise upon completion      */
    )
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_read_reg(data));
        _MEM_IMPLEMENT_COMMAND(read_swap_le, cmd_mem_read64_swap_le_ptr32, 1, MAX_REF_COUNT_16);
    }
    _INTRINSIC_END;
}


__intrinsic
void cmd_mem_read64_swap_le_ptr40(
    __xread void *data,                     /* returns data from operation          */
    volatile void __addr40 __mem *address,  /* address to read from                 */
    uint32_t count,                         /* number of quadwords to read          */
    sync_t sync,                            /* sig_done or ctx_swap                 */
    SIGNAL *sig_ptr                         /* signal to raise upon completion      */
    )
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_read_reg(data));
        _MEM_IMPLEMENT_COMMAND_PTR40(read_swap_le, cmd_mem_read64_swap_le_ptr40, 1, MAX_REF_COUNT_16);
    }
    _INTRINSIC_END;
}


/* DRAM read swap le indirect */
__intrinsic
void cmd_mem_read64_swap_le_ind_ptr32(
    __xread void *data,                     /* returns data from operation          */
    volatile void __addr32 __mem *address,  /* address to read from                 */
    uint32_t max_nn,                        /* max. number of quadwords to read     */
    generic_ind_t ind,                      /* indirect word                        */
    sync_t sync,                            /* sig_done or ctx_swap                 */
    SIGNAL *sig_ptr                         /* signal to raise upon completion      */
    )
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_read_reg(data));
        __asm local_csr_wr[CMD_INDIRECT_REF_0, CSR_INDIRECT_TYPE(ind)]
        _MEM_IMPLEMENT_INDIRECT_COMMAND(read_swap_le, 1, 16);
    }
    _INTRINSIC_END;
}


__intrinsic
void cmd_mem_read64_swap_le_ind_ptr40(
    __xread void *data,                     /* returns data from operation          */
    volatile void __addr40 __mem *address,  /* address to read from                 */
    uint32_t max_nn,                        /* max. number of quadwords to read     */
    generic_ind_t ind,                      /* indirect word                        */
    sync_t sync,                            /* sig_done or ctx_swap                 */
    SIGNAL *sig_ptr                         /* signal to raise upon completion      */
    )
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_read_reg(data));
        __asm local_csr_wr[CMD_INDIRECT_REF_0, CSR_INDIRECT_TYPE(ind)]
        _MEM_IMPLEMENT_INDIRECT_COMMAND_PTR40(read_swap_le, 1, 16);
    }
    _INTRINSIC_END;
}


/* DRAM write */
__intrinsic
void cmd_mem_write64_ptr32(
    __xwrite void *data,                    /* data to write                        */
    volatile void __addr32 __mem *address,  /* address to operate on                */
    uint32_t count,                         /* number of quadwords to write         */
    sync_t sync,                            /* sig_done or ctx_swap                 */
    SIGNAL *sig_ptr                         /* signal to raise upon completion      */
    )
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_write_reg(data));
        _MEM_IMPLEMENT_COMMAND(write, cmd_mem_write64_ptr32, 1, MAX_REF_COUNT_16);
    }
    _INTRINSIC_END;
}


__intrinsic
void cmd_mem_write64_ptr40(
    __xwrite void *data,                    /* data to write                        */
    volatile void __addr40 __mem *address,  /* address to operate on                */
    uint32_t count,                         /* number of quadwords to write         */
    sync_t sync,                            /* sig_done or ctx_swap                 */
    SIGNAL *sig_ptr                         /* signal to raise upon completion      */
    )
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_write_reg(data));
        _MEM_IMPLEMENT_COMMAND_PTR40(write, cmd_mem_write64_ptr40, 1, MAX_REF_COUNT_16);
    }
    _INTRINSIC_END;
}


/* DRAM write indirect */
__intrinsic
void cmd_mem_write64_ind_ptr32(
    __xwrite void *data,                    /* data to write                        */
    volatile void __addr32 __mem *address,  /* address to operate on                */
    uint32_t max_nn,                        /* max. number of quadwords to write    */
    generic_ind_t ind,                      /* indirect word                        */
    sync_t sync,                            /* sig_done or ctx_swap                 */
    SIGNAL *sig_ptr                         /* signal to raise upon completion      */
    )
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_write_reg(data));
        __asm local_csr_wr[CMD_INDIRECT_REF_0, CSR_INDIRECT_TYPE(ind)]
        _MEM_IMPLEMENT_INDIRECT_COMMAND(write, 1, 16);
    }
    _INTRINSIC_END;
}


__intrinsic
void cmd_mem_write64_ind_ptr40(
    __xwrite void *data,                    /* data to write                        */
    volatile void __addr40 __mem *address,  /* address to operate on                */
    uint32_t max_nn,                        /* max. number of quadwords to write    */
    generic_ind_t ind,                      /* indirect word                        */
    sync_t sync,                            /* sig_done or ctx_swap                 */
    SIGNAL *sig_ptr                         /* signal to raise upon completion      */
    )
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_write_reg(data));
        __asm local_csr_wr[CMD_INDIRECT_REF_0, CSR_INDIRECT_TYPE(ind)]
        _MEM_IMPLEMENT_INDIRECT_COMMAND_PTR40(write, 1, 16);
    }
    _INTRINSIC_END;
}


/* Little endian read */
__intrinsic
void cmd_mem_read64_le_ptr32(
    __xread void *data,                     /* returns data from operation          */
    volatile void __addr32 __mem *address,  /* address to read from                 */
    uint32_t count,                         /* number of quadwords to read          */
    sync_t sync,                            /* sig_done or ctx_swap                 */
    SIGNAL *sig_ptr                         /* signal to raise upon completion      */
    )
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_read_reg(data));
        _MEM_IMPLEMENT_COMMAND(read_le, cmd_mem_read64_le_ptr32, 1, MAX_REF_COUNT_16);
    }
    _INTRINSIC_END;
}


__intrinsic
void cmd_mem_read64_le_ptr40(
    __xread void *data,                     /* returns data from operation          */
    volatile void __addr40 __mem *address,  /* address to read from                 */
    uint32_t count,                         /* number of quadwords to read          */
    sync_t sync,                            /* sig_done or ctx_swap                 */
    SIGNAL *sig_ptr                         /* signal to raise upon completion      */
    )
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_read_reg(data));
        _MEM_IMPLEMENT_COMMAND_PTR40(read_le, cmd_mem_read64_le_ptr40, 1, MAX_REF_COUNT_16);
    }
    _INTRINSIC_END;
}


__intrinsic
void cmd_mem_read64_le_ind_ptr32(
    __xread void *data,                     /* returns data from operation          */
    volatile void __addr32 __mem *address,  /* address to read from                 */
    uint32_t max_nn,                        /* max. number of quadwords to read     */
    generic_ind_t ind,                      /* indirect word                        */
    sync_t sync,                            /* sig_done or ctx_swap                 */
    SIGNAL *sig_ptr                         /* signal to raise upon completion      */
    )
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_read_reg(data));
        __asm local_csr_wr[CMD_INDIRECT_REF_0, CSR_INDIRECT_TYPE(ind)]
        _MEM_IMPLEMENT_INDIRECT_COMMAND(read_le, 1, 16);
    }
    _INTRINSIC_END;
}

__intrinsic
void cmd_mem_read64_le_ind_ptr40(
    __xread void *data,                     /* returns data from operation          */
    volatile void __addr40 __mem *address,  /* address to read from                 */
    uint32_t max_nn,                        /* max. number of quadwords to read     */
    generic_ind_t ind,                      /* indirect word                        */
    sync_t sync,                            /* sig_done or ctx_swap                 */
    SIGNAL *sig_ptr                         /* signal to raise upon completion      */
    )
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_read_reg(data));
        __asm local_csr_wr[CMD_INDIRECT_REF_0, CSR_INDIRECT_TYPE(ind)]
        _MEM_IMPLEMENT_INDIRECT_COMMAND_PTR40(read_le, 1, 16);
    }
    _INTRINSIC_END;
}


__intrinsic
void cmd_mem_write64_le_ptr32(
    __xwrite void *data,                    /* data to write                        */
    volatile void __addr32 __mem *address,  /* address to operate on                */
    uint32_t count,                         /* number of quadwords to write         */
    sync_t sync,                            /* sig_done or ctx_swap                 */
    SIGNAL *sig_ptr                         /* signal to raise upon completion      */
    )
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_write_reg(data));
        _MEM_IMPLEMENT_COMMAND(write_le, cmd_mem_write64_le_ptr32, 1, MAX_REF_COUNT_16);
    }
    _INTRINSIC_END;
}

__intrinsic
void cmd_mem_write64_le_ptr40(
    __xwrite void *data,                    /* data to write                        */
    volatile void __addr40 __mem *address,  /* address to operate on                */
    uint32_t count,                         /* number of quadwords to write         */
    sync_t sync,                            /* sig_done or ctx_swap                 */
    SIGNAL *sig_ptr                         /* signal to raise upon completion      */
    )
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_write_reg(data));
        _MEM_IMPLEMENT_COMMAND_PTR40(write_le, cmd_mem_write64_le_ptr40, 1, MAX_REF_COUNT_16);
    }
    _INTRINSIC_END;
}


__intrinsic
void cmd_mem_write64_le_ind_ptr32(
    __xwrite void *data,                    /* data to write                        */
    volatile void __addr32 __mem *address,  /* address to operate on                */
    uint32_t max_nn,                        /* max. number of quadwords to write    */
    generic_ind_t ind,                      /* indirect word                        */
    sync_t sync,                            /* sig_done or ctx_swap                 */
    SIGNAL *sig_ptr                         /* signal to raise upon completion      */
    )
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_write_reg(data));
        __asm local_csr_wr[CMD_INDIRECT_REF_0, CSR_INDIRECT_TYPE(ind)]
        _MEM_IMPLEMENT_INDIRECT_COMMAND(write_le, 1, 16);
    }
    _INTRINSIC_END;
}

__intrinsic
void cmd_mem_write64_le_ind_ptr40(
                                                                                                                                                                                                                                                                __xwrite void *data,                        /* data to write                        */
    volatile void __addr40 __mem *address,  /* address to operate on                */
    uint32_t max_nn,                        /* max. number of quadwords to write    */
    generic_ind_t ind,                      /* indirect word                        */
    sync_t sync,                            /* sig_done or ctx_swap                 */
    SIGNAL *sig_ptr                         /* signal to raise upon completion      */
    )
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_write_reg(data));
        __asm local_csr_wr[CMD_INDIRECT_REF_0, CSR_INDIRECT_TYPE(ind)]
        _MEM_IMPLEMENT_INDIRECT_COMMAND_PTR40(write_le, 1, 16);
    }
    _INTRINSIC_END;
}


__intrinsic
void cmd_mem_write64_swap_ptr32(
    __xwrite void *data,                    /* data to write                        */
    volatile void __addr32 __mem *address,  /* address to operate on                */
    uint32_t count,                         /* number of quadwords to write         */
    sync_t sync,                            /* sig_done or ctx_swap                 */
    SIGNAL *sig_ptr                         /* signal to raise upon completion      */
    )
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_write_reg(data));
        _MEM_IMPLEMENT_COMMAND(write_swap, cmd_mem_write64_swap_ptr32, 1, MAX_REF_COUNT_16);
    }
    _INTRINSIC_END;
}

__intrinsic
void cmd_mem_write64_swap_ptr40(
    __xwrite void *data,                    /* data to write                        */
    volatile void __addr40 __mem *address,  /* address to operate on                */
    uint32_t count,                         /* number of quadwords to write         */
    sync_t sync,                            /* sig_done or ctx_swap                 */
    SIGNAL *sig_ptr                         /* signal to raise upon completion      */
    )
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_write_reg(data));
        _MEM_IMPLEMENT_COMMAND_PTR40(write_swap, cmd_mem_write64_swap_ptr40, 1, MAX_REF_COUNT_16);
    }
    _INTRINSIC_END;
}


__intrinsic
void cmd_mem_write64_swap_ind_ptr32(
    __xwrite void *data,                    /* data to write                        */
    volatile void __addr32 __mem *address,  /* address to operate on                */
    uint32_t max_nn,                        /* max. number of 64-bit words to write */
    generic_ind_t ind,                      /* indirect word                        */
    sync_t sync,                            /* sig_done or ctx_swap                 */
    SIGNAL *sig_ptr                         /* signal to raise upon completion      */
    )
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_write_reg(data));
        __asm local_csr_wr[CMD_INDIRECT_REF_0, CSR_INDIRECT_TYPE(ind)]
        _MEM_IMPLEMENT_INDIRECT_COMMAND(write_swap, 1, 16);
    }
    _INTRINSIC_END;
}

__intrinsic
void cmd_mem_write64_swap_ind_ptr40(
    __xwrite void *data,                    /* data to write                        */
    volatile void __addr40 __mem *address,  /* address to operate on                */
    uint32_t max_nn,                        /* max. number of 64-bit words to write */
    generic_ind_t ind,                      /* indirect word                        */
    sync_t sync,                            /* sig_done or ctx_swap                 */
    SIGNAL *sig_ptr                         /* signal to raise upon completion      */
    )
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_write_reg(data));
        __asm local_csr_wr[CMD_INDIRECT_REF_0, CSR_INDIRECT_TYPE(ind)]
        _MEM_IMPLEMENT_INDIRECT_COMMAND_PTR40(write_swap, 1, 16);
    }
    _INTRINSIC_END;
}


__intrinsic
void cmd_mem_write64_swap_le_ptr32(
    __xwrite void *data,                    /* data to write                        */
    volatile void __addr32 __mem *address,  /* address to operate on                */
    uint32_t count,                         /* number of quadwords to write         */
    sync_t sync,                            /* sig_done or ctx_swap                 */
    SIGNAL *sig_ptr                         /* signal to raise upon completion      */
    )
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_write_reg(data));
        _MEM_IMPLEMENT_COMMAND(write_swap_le,cmd_mem_write64_swap_le_ptr32, 1, MAX_REF_COUNT_16);
    }
    _INTRINSIC_END;
}

__intrinsic
void cmd_mem_write64_swap_le_ptr40(
    __xwrite void *data,                    /* data to write                        */
    volatile void __addr40 __mem *address,  /* address to operate on                */
    uint32_t count,                         /* number of quadwords to write         */
    sync_t sync,                            /* sig_done or ctx_swap                 */
    SIGNAL *sig_ptr                         /* signal to raise upon completion      */
    )
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_write_reg(data));
        _MEM_IMPLEMENT_COMMAND_PTR40(write_swap_le,cmd_mem_write64_swap_le_ptr40, 1, MAX_REF_COUNT_16);
    }
    _INTRINSIC_END;
}


__intrinsic
void cmd_mem_write64_swap_le_ind_ptr32(
    __xwrite void *data,                    /* data to write                        */
    volatile void __addr32 __mem *address,  /* address to operate on                */
    uint32_t max_nn,                        /* max. number of 32-bit words to write */
    generic_ind_t ind,                      /* indirect word                        */
    sync_t sync,                            /* sig_done or ctx_swap                 */
    SIGNAL *sig_ptr                         /* signal to raise upon completion      */
    )
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_write_reg(data));
        __asm local_csr_wr[CMD_INDIRECT_REF_0, CSR_INDIRECT_TYPE(ind)]
        _MEM_IMPLEMENT_INDIRECT_COMMAND(write_swap_le, 1 ,16);
    }
    _INTRINSIC_END;
}

__intrinsic
void cmd_mem_write64_swap_le_ind_ptr40(
    __xwrite void *data,                    /* data to write                        */
    volatile void __addr40 __mem *address,  /* address to operate on                */
    uint32_t max_nn,                        /* max. number of 32-bit words to write */
    generic_ind_t ind,                      /* indirect word                        */
    sync_t sync,                            /* sig_done or ctx_swap                 */
    SIGNAL *sig_ptr                         /* signal to raise upon completion      */
    )
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_write_reg(data));
        __asm local_csr_wr[CMD_INDIRECT_REF_0, CSR_INDIRECT_TYPE(ind)]
        _MEM_IMPLEMENT_INDIRECT_COMMAND_PTR40(write_swap_le, 1 ,16);
    }
    _INTRINSIC_END;
}

__intrinsic
void cmd_mem_write8_ptr32(
    __xwrite void *data,                    /* data to write                        */
    volatile void __addr32 __mem *address,  /* address to operate on                */
    uint32_t count,                         /* number of bytes to write             */
    sync_t sync,                            /* sig_done or ctx_swap                 */
    SIGNAL *sig_ptr                         /* signal to raise upon completion      */
    )
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_write_reg(data));
        _MEM_IMPLEMENT_COMMAND(write8, cmd_mem_write8_ptr32, 1, INTRINSIC_IND_ALU_COUNT_MAX);
    }
    _INTRINSIC_END;
}

__intrinsic
void cmd_mem_write8_ptr40(
    __xwrite void *data,                    /* data to write                        */
    volatile void __addr40 __mem *address,  /* address to operate on                */
    uint32_t count,                         /* number of bytes to write             */
    sync_t sync,                            /* sig_done or ctx_swap                 */
    SIGNAL *sig_ptr                         /* signal to raise upon completion      */
    )
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_write_reg(data));
        _MEM_IMPLEMENT_COMMAND_PTR40(write8, cmd_mem_write8_ptr40, 1, INTRINSIC_IND_ALU_COUNT_MAX);
    }
    _INTRINSIC_END;
}

__intrinsic
void cmd_mem_write8_ind_ptr32(
    __xwrite void *data,                    /* data to write                        */
    volatile void __addr32 __mem *address,  /* address to operate on                */
    uint32_t max_nn,                        /* max. number of bytes to write        */
    generic_ind_t ind,                      /* indirect word                        */
    sync_t sync,                            /* sig_done or ctx_swap                 */
    SIGNAL *sig_ptr                         /* signal to raise upon completion      */
    )
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_write_reg(data));
        __asm local_csr_wr[CMD_INDIRECT_REF_0, CSR_INDIRECT_TYPE(ind)]
        _MEM_IMPLEMENT_INDIRECT_COMMAND(write8, 1, INTRINSIC_IND_ALU_COUNT_MAX);
    }
    _INTRINSIC_END;
}

__intrinsic
void cmd_mem_write8_ind_ptr40(
    __xwrite void *data,                    /* data to write                        */
    volatile void __addr40 __mem *address,  /* address to operate on                */
    uint32_t max_nn,                        /* max. number of bytes to write        */
    generic_ind_t ind,                      /* indirect word                        */
    sync_t sync,                            /* sig_done or ctx_swap                 */
    SIGNAL *sig_ptr                         /* signal to raise upon completion      */
    )
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_write_reg(data));
        __asm local_csr_wr[CMD_INDIRECT_REF_0, CSR_INDIRECT_TYPE(ind)]
        _MEM_IMPLEMENT_INDIRECT_COMMAND_PTR40(write8, 1, INTRINSIC_IND_ALU_COUNT_MAX);
    }
    _INTRINSIC_END;
}

__intrinsic
void cmd_mem_write8_le_ptr32(
    __xwrite void *data,                    /* data to write                        */
    volatile void __addr32 __mem *address,  /* address to operate on                */
    uint32_t count,                         /* number of bytes to write             */
    sync_t sync,                            /* sig_done or ctx_swap                 */
    SIGNAL *sig_ptr                         /* signal to raise upon completion      */
    )
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_write_reg(data));
        _MEM_IMPLEMENT_COMMAND(write8_le,cmd_mem_write8_le_ptr32, 1, INTRINSIC_IND_ALU_COUNT_MAX);
    }
    _INTRINSIC_END;
}

__intrinsic
void cmd_mem_write8_le_ptr40(
    __xwrite void *data,                    /* data to write                        */
    volatile void __addr40 __mem *address,  /* address to operate on                */
    uint32_t count,                         /* number of bytes to write             */
    sync_t sync,                            /* sig_done or ctx_swap                 */
    SIGNAL *sig_ptr                         /* signal to raise upon completion      */
    )
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_write_reg(data));
        _MEM_IMPLEMENT_COMMAND_PTR40(write8_le,cmd_mem_write8_le_ptr40, 1, INTRINSIC_IND_ALU_COUNT_MAX);
    }
    _INTRINSIC_END;
}


__intrinsic
void cmd_mem_write8_le_ind_ptr32(
    __xwrite void *data,                    /* data to write                        */
    volatile void __addr32 __mem *address,  /* address to operate on                */
    uint32_t max_nn,                        /* max. number of bytes to write        */
    generic_ind_t ind,                      /* indirect word                        */
    sync_t sync,                            /* sig_done or ctx_swap                 */
    SIGNAL *sig_ptr                         /* signal to raise upon completion      */
    )
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_write_reg(data));
        __asm local_csr_wr[CMD_INDIRECT_REF_0, CSR_INDIRECT_TYPE(ind)]
        _MEM_IMPLEMENT_INDIRECT_COMMAND(write8_le, 1, INTRINSIC_IND_ALU_COUNT_MAX);
    }
    _INTRINSIC_END;
}

__intrinsic
void cmd_mem_write8_le_ind_ptr40(
    __xwrite void *data,                    /* data to write                        */
    volatile void __addr40 __mem *address,  /* address to operate on                */
    uint32_t max_nn,                        /* max. number of bytes to write        */
    generic_ind_t ind,                      /* indirect word                        */
    sync_t sync,                            /* sig_done or ctx_swap                 */
    SIGNAL *sig_ptr                         /* signal to raise upon completion      */
    )
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_write_reg(data));
        __asm local_csr_wr[CMD_INDIRECT_REF_0, CSR_INDIRECT_TYPE(ind)]
        _MEM_IMPLEMENT_INDIRECT_COMMAND_PTR40(write8_le, 1, INTRINSIC_IND_ALU_COUNT_MAX);
    }
    _INTRINSIC_END;
}


__intrinsic
void cmd_mem_write8_swap_ptr32(
    __xwrite void *data,                    /* data to write                        */
    volatile void __addr32 __mem *address,  /* address to operate on                */
    uint32_t count,                         /* number of bytes to write             */
    sync_t sync,                            /* sig_done or ctx_swap                 */
    SIGNAL *sig_ptr                         /* signal to raise upon completion      */
    )
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_write_reg(data));
        _MEM_IMPLEMENT_COMMAND(write8_swap, cmd_mem_write8_swap_ptr32, 1, INTRINSIC_IND_ALU_COUNT_MAX);
    }
    _INTRINSIC_END;
}

__intrinsic
void cmd_mem_write8_swap_ptr40(
    __xwrite void *data,                    /* data to write                        */
    volatile void __addr40 __mem *address,  /* address to operate on                */
    uint32_t count,                         /* number of bytes to write             */
    sync_t sync,                            /* sig_done or ctx_swap                 */
    SIGNAL *sig_ptr                         /* signal to raise upon completion      */
    )
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_write_reg(data));
        _MEM_IMPLEMENT_COMMAND_PTR40(write8_swap, cmd_mem_write8_swap_ptr40, 1, INTRINSIC_IND_ALU_COUNT_MAX);
    }
    _INTRINSIC_END;
}


__intrinsic
void cmd_mem_write8_swap_ind_ptr32(
    __xwrite void *data,                    /* data to write                        */
    volatile void __addr32 __mem *address,  /* address to operate on                */
    uint32_t max_nn,                        /* max. number of bytes to write        */
    generic_ind_t ind,                      /* indirect word                        */
    sync_t sync,                            /* sig_done or ctx_swap                 */
    SIGNAL *sig_ptr                         /* signal to raise upon completion      */
    )
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_write_reg(data));

    __asm local_csr_wr[CMD_INDIRECT_REF_0, CSR_INDIRECT_TYPE(ind)]

        _MEM_IMPLEMENT_INDIRECT_COMMAND(write8_swap, 1, INTRINSIC_IND_ALU_COUNT_MAX);
    }
    _INTRINSIC_END;
}

__intrinsic
void cmd_mem_write8_swap_ind_ptr40(
    __xwrite void *data,                    /* data to write                        */
    volatile void __addr40 __mem *address,  /* address to operate on                */
    uint32_t max_nn,                        /* max. number of bytes to write        */
    generic_ind_t ind,                      /* indirect word                        */
    sync_t sync,                            /* sig_done or ctx_swap                 */
    SIGNAL *sig_ptr                         /* signal to raise upon completion      */
    )
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_write_reg(data));
        __asm local_csr_wr[CMD_INDIRECT_REF_0, CSR_INDIRECT_TYPE(ind)]
        _MEM_IMPLEMENT_INDIRECT_COMMAND_PTR40(write8_swap, 1, INTRINSIC_IND_ALU_COUNT_MAX);
    }
    _INTRINSIC_END;
}


__intrinsic
void cmd_mem_write8_swap_le_ptr32(
    __xwrite void *data,                    /* data to write                        */
    volatile void __addr32 __mem *address,  /* address to operate on                */
    uint32_t count,                         /* number of bytes to write             */
    sync_t sync,                            /* sig_done or ctx_swap                 */
    SIGNAL *sig_ptr                         /* signal to raise upon completion      */
    )
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_write_reg(data));
        _MEM_IMPLEMENT_COMMAND(write8_swap_le, cmd_mem_write8_swap_le_ptr32, 1, INTRINSIC_IND_ALU_COUNT_MAX);
    }
    _INTRINSIC_END;
}

__intrinsic
void cmd_mem_write8_swap_le_ptr40(
    __xwrite void *data,                    /* data to write                        */
    volatile void __addr40 __mem *address,  /* address to operate on                */
    uint32_t count,                         /* number of bytes to write             */
    sync_t sync,                            /* sig_done or ctx_swap                 */
    SIGNAL *sig_ptr                         /* signal to raise upon completion      */
    )
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_write_reg(data));
        _MEM_IMPLEMENT_COMMAND_PTR40(write8_swap_le, cmd_mem_write8_swap_le_ptr40, 1, INTRINSIC_IND_ALU_COUNT_MAX);
    }
    _INTRINSIC_END;
}


__intrinsic
void cmd_mem_write8_swap_le_ind_ptr32(
    __xwrite void *data,                    /* data to write                        */
    volatile void __addr32 __mem *address,  /* address to operate on                */
    uint32_t max_nn,                        /* max. number of bytes to write        */
    generic_ind_t ind,                      /* indirect word                        */
    sync_t sync,                            /* sig_done or ctx_swap                 */
    SIGNAL *sig_ptr                         /* signal to raise upon completion      */
    )
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_write_reg(data));
        __asm local_csr_wr[CMD_INDIRECT_REF_0, CSR_INDIRECT_TYPE(ind)]
        _MEM_IMPLEMENT_INDIRECT_COMMAND(write8_swap_le, 1, INTRINSIC_IND_ALU_COUNT_MAX);
    }
    _INTRINSIC_END;
}


__intrinsic
void cmd_mem_write8_swap_le_ind_ptr40(
    __xwrite void *data,                    /* data to write                        */
    volatile void __addr40 __mem *address,  /* address to operate on                */
    uint32_t max_nn,                        /* max. number of bytes to write        */
    generic_ind_t ind,                      /* indirect word                        */
    sync_t sync,                            /* sig_done or ctx_swap                 */
    SIGNAL *sig_ptr                         /* signal to raise upon completion      */
    )
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_write_reg(data));
        __asm local_csr_wr[CMD_INDIRECT_REF_0, CSR_INDIRECT_TYPE(ind)]
        _MEM_IMPLEMENT_INDIRECT_COMMAND_PTR40(write8_swap_le, 1, INTRINSIC_IND_ALU_COUNT_MAX);
    }
    _INTRINSIC_END;
}


__intrinsic
void cmd_mem_read8_ptr32(
    __xread void *data,                     /* returns data from operation          */
    volatile void __addr32 __mem *address,  /* address to read from                 */
    uint32_t count,                         /* number of bytes to read              */
    sync_t sync,                            /* sig_done or ctx_swap                 */
    SIGNAL *sig_ptr                         /* signal to raise upon completion      */
    )
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_read_reg(data));
        _MEM_IMPLEMENT_COMMAND(read8, cmd_mem_read8_ptr32, 1, INTRINSIC_IND_ALU_COUNT_MAX);
    }
    _INTRINSIC_END;
}


__intrinsic
void cmd_mem_read8_ptr40(
    __xread void *data,                     /* returns data from operation          */
    volatile void __addr40 __mem *address,  /* address to read from                 */
    uint32_t count,                         /* number of bytes to read              */
    sync_t sync,                            /* sig_done or ctx_swap                 */
    SIGNAL *sig_ptr                         /* signal to raise upon completion      */
    )
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_read_reg(data));
        _MEM_IMPLEMENT_COMMAND_PTR40(read8, cmd_mem_read8_ptr40, 1, INTRINSIC_IND_ALU_COUNT_MAX);
    }
    _INTRINSIC_END;
}


__intrinsic
void cmd_mem_read8_ind_ptr32(
    __xread void *data,                     /* returns data from operation          */
    volatile void __addr32 __mem *address,  /* address to read from                 */
    uint32_t max_nn,                        /* max. number of bytes to read         */
    generic_ind_t ind,                      /* indirect word                        */
    sync_t sync,                            /* sig_done or ctx_swap                 */
    SIGNAL *sig_ptr                         /* signal to raise upon completion      */
    )
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_read_reg(data));
        __asm local_csr_wr[CMD_INDIRECT_REF_0, CSR_INDIRECT_TYPE(ind)]
        _MEM_IMPLEMENT_INDIRECT_COMMAND(read8, 1, INTRINSIC_IND_ALU_COUNT_MAX);
    }
    _INTRINSIC_END;
}

__intrinsic
void cmd_mem_read8_ind_ptr40(
    __xread void *data,                     /* returns data from operation          */
    volatile void __addr40 __mem *address,  /* address to read from                 */
    uint32_t max_nn,                        /* max. number of bytes to read         */
    generic_ind_t ind,                      /* indirect word                        */
    sync_t sync,                            /* sig_done or ctx_swap                 */
    SIGNAL *sig_ptr                         /* signal to raise upon completion      */
    )
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_read_reg(data));
        __asm local_csr_wr[CMD_INDIRECT_REF_0, CSR_INDIRECT_TYPE(ind)]
        _MEM_IMPLEMENT_INDIRECT_COMMAND_PTR40(read8, 1, INTRINSIC_IND_ALU_COUNT_MAX);
    }
    _INTRINSIC_END;
}


__intrinsic
void cmd_mem_read_atomic_ptr32(
    __xread void *data,                     /* returns data from operation          */
    volatile void __addr32 __mem *address,  /* address to read from                 */
    uint32_t count,                         /* number of 32-bit words to read       */
    sync_t sync,                            /* sig_done or ctx_swap                 */
    SIGNAL *sig_ptr                         /* signal to raise upon completion      */
    )
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_read_reg(data));
        _MEM_IMPLEMENT_COMMAND(atomic_read, cmd_mem_read_atomic_ptr32, 1, 8);
    }
    _INTRINSIC_END;
}

__intrinsic
void cmd_mem_read_atomic_ptr40(
    __xread void *data,                     /* returns data from operation          */
    volatile void __addr40 __mem *address,  /* address to read from                 */
    uint32_t count,                         /* number of 32-bit words to read       */
    sync_t sync,                            /* sig_done or ctx_swap                 */
    SIGNAL *sig_ptr                         /* signal to raise upon completion      */
    )
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_read_reg(data));
        _MEM_IMPLEMENT_COMMAND_PTR40(atomic_read, cmd_mem_read_atomic_ptr40, 1, 8);
    }
    _INTRINSIC_END;
}


__intrinsic
void cmd_mem_read_atomic_ind_ptr32(
    __xread void *data,                     /* returns data from operation          */
    volatile void __addr32 __mem *address,  /* address to read from                 */
    uint32_t max_nn,                        /* max. number of 32-bit words to read  */
    generic_ind_t ind,                      /* indirect word                        */
    sync_t sync,                            /* sig_done or ctx_swap                 */
    SIGNAL *sig_ptr                         /* signal to raise upon completion      */
    )
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_read_reg(data));
        __asm local_csr_wr[CMD_INDIRECT_REF_0, CSR_INDIRECT_TYPE(ind)]
        _MEM_IMPLEMENT_INDIRECT_COMMAND(atomic_read, 1, 8);
    }
    _INTRINSIC_END;
}

__intrinsic
void cmd_mem_read_atomic_ind_ptr40(
    __xread void *data,                     /* returns data from operation          */
    volatile void __addr40 __mem *address,  /* address to read from                 */
    uint32_t max_nn,                        /* max. number of 32-bit words to read  */
        generic_ind_t ind,                  /* indirect word                        */
        sync_t sync,                        /* sig_done or ctx_swap                 */
        SIGNAL *sig_ptr                     /* signal to raise upon completion      */
    )
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_read_reg(data));
        __asm local_csr_wr[CMD_INDIRECT_REF_0, CSR_INDIRECT_TYPE(ind)]
        _MEM_IMPLEMENT_INDIRECT_COMMAND_PTR40(atomic_read, 1, 8);
    }
    _INTRINSIC_END;
}


__intrinsic
void cmd_mem_compare_write_ptr32(
    __xwrite void *data,                    /* data to write                        */
    volatile void __addr32 __mem *address,  /* address to operate on                */
    uint32_t count,                         /* number of 32-bit words to write      */
    sync_t sync,                            /* sig_done or ctx_swap                 */
    SIGNAL *sig_ptr                         /* signal  to raise upon completion */
    )
{
    _INTRINSIC_BEGIN;
    {
        // cmd_mem_compare_write_ptr32() function requires silicon revision C0 or higher
        CT_ASSERT(__is_nfp_arch_or_above(__nfp6000, __REVISION_C0));
        CT_ASSERT(__is_write_reg(data));
        _MEM_IMPLEMENT_COMMAND(compare_write_or_incr, cmd_mem_compare_write_ptr32, 1, 8);
    }
    _INTRINSIC_END;
}

__intrinsic
void cmd_mem_compare_write_ptr40(
    __xwrite void *data,                        /* data to write                        */
    volatile void __addr40 __mem *address,      /* address to operate on                */
    uint32_t count,                             /* number of 32-bit words to write      */
    sync_t sync,                                /* sig_done or ctx_swap                 */
    SIGNAL *sig_ptr                             /* signal to raise upon completion      */
    )
{
    _INTRINSIC_BEGIN;
    {
        // cmd_mem_compare_write_ptr40() function requires silicon revision C0 or higher
        CT_ASSERT(__is_nfp_arch_or_above(__nfp6000, __REVISION_C0));
        CT_ASSERT(__is_write_reg(data));
        _MEM_IMPLEMENT_COMMAND_PTR40(compare_write_or_incr, cmd_mem_compare_write_ptr40, 1, 8);

    }
    _INTRINSIC_END;
}


__intrinsic
void cmd_mem_compare_write_ind_ptr32(
    __xwrite void *data,                    /* data to write                        */
    volatile void __addr32 __mem *address,  /* address to operate on                */
    uint32_t max_nn,                        /* max. number 32-bit words to write    */
    generic_ind_t ind,                      /* indirect word                        */
    sync_t sync,                            /* sig_done or ctx_swap                 */
    SIGNAL_PAIR *sig_pair_ptr               /* signal pair to raise upon completion */
    )
{
    _INTRINSIC_BEGIN;
    {
        // cmd_mem_compare_write_ind_ptr32() function requires silicon revision C0 or higher
        CT_ASSERT(__is_nfp_arch_or_above(__nfp6000, __REVISION_C0));
        CT_ASSERT(__is_write_reg(data));
        CT_ASSERT(__is_ct_const(max_nn));
        CT_ASSERT(max_nn >= 1 && max_nn <= 8);
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done || sync == ctx_swap);

        __asm local_csr_wr[CMD_INDIRECT_REF_0, CSR_INDIRECT_TYPE(ind)]
        __asm alu[--, --, B, ind]
        if (sync == sig_done)
        {
            __asm mem[compare_write_or_incr, *data, address, 0, __ct_const_val(max_nn)], sig_done[*__ct_sig_pair(sig_pair_ptr)], indirect_ref
        }
        else
        {
            __asm mem[compare_write_or_incr, *data, address, 0, __ct_const_val(max_nn)], ctx_swap[*__ct_sig_pair(sig_pair_ptr)], indirect_ref
        }
    }
    _INTRINSIC_END;
}


__intrinsic
void cmd_mem_compare_write_ind_ptr40(
    __xwrite void *data,                    /* data to write                        */
    volatile void __addr32 __mem *address,  /* address to operate on                */
    uint32_t max_nn,                        /* max. number 32-bit words to write    */
    generic_ind_t ind,                      /* indirect word                        */
    sync_t sync,                            /* sig_done or ctx_swap                 */
    SIGNAL_PAIR *sig_pair_ptr               /* signal pair to raise upon completion */
    )
{
    _INTRINSIC_BEGIN;
    {
        uint32_t hi_addr, low_addr;

        // cmd_mem_compare_write_ind_ptr40() function requires silicon revision C0 or higher
        CT_ASSERT(__is_nfp_arch_or_above(__nfp6000, __REVISION_C0));
        _INTRINSIC_CONVERT_HI_LO_ADDRESS(address);
        CT_ASSERT(__is_write_reg(data));
        CT_ASSERT(__is_ct_const(max_nn));
        CT_ASSERT(max_nn >= 1 && max_nn <= 8);
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done || sync == ctx_swap);

        __asm local_csr_wr[CMD_INDIRECT_REF_0, CSR_INDIRECT_TYPE(ind)]
        __asm alu[--, --, B, ind]
        if (sync == sig_done)
        {
            __asm mem[compare_write_or_incr, *data, hi_addr, <<8, low_addr, __ct_const_val(max_nn)], sig_done[*__ct_sig_pair(sig_pair_ptr)], indirect_ref
        }
        else
        {
            __asm mem[compare_write_or_incr, *data, hi_addr, <<8, low_addr, __ct_const_val(max_nn)], ctx_swap[*__ct_sig_pair(sig_pair_ptr)], indirect_ref
        }
    }
    _INTRINSIC_END;
}


__intrinsic
void cmd_mem_test_and_compare_write_ptr32(
    __xread uint32_t *val,                  /* returned pre-modified value          */
    __xwrite void *data,                    /* data to write                        */
    volatile void __addr32 __mem *address,  /* address to operate on                */
    uint32_t count,                         /* number of 32-bit words to write      */
    sync_t sync,                            /* must be sig_done                     */
    SIGNAL_PAIR *sig_pair_ptr               /* signal pair to raise upon completion */
    )
{
    _INTRINSIC_BEGIN;
    {
        // cmd_mem_test_and_compare_write_ptr32() function requires silicon revision C0 or higher
        CT_ASSERT(__is_nfp_arch_or_above(__nfp6000, __REVISION_C0));
        CT_ASSERT(__is_read_reg(val));
        CT_ASSERT(__is_write_reg(data));
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done);
        __associate_read_write_reg_pair_no_spill(val, data);

        if (__is_ct_const(count) && count <= 8)
        {
            CT_ASSERT(count != 0);
            __asm mem[test_compare_write_or_incr, *val, address, 0, __ct_const_val(count)], sig_done[*__ct_sig_pair(sig_pair_ptr)]
        }
        else
        {
            if (!__is_ct_const(count))
            {
                CT_QPERFINFO_INDIRECT_REF(cmd_mem_test_and_compare_write_ptr40);
            }
            {
                _INTRINSIC_IND_ALU_COUNT_MIN_MAX(count, 1, MAX_REF_COUNT_8);
                __asm mem[test_compare_write_or_incr, *val, address, 0, __ct_const_val(count)], sig_done[*__ct_sig_pair(sig_pair_ptr)], indirect_ref
            }
        }
    }
    _INTRINSIC_END;
}

__intrinsic
void cmd_mem_test_and_compare_write_ptr40(
    __xread uint32_t *val,                  /* returned pre-modified value          */
    __xwrite void *data,                    /* data to write                        */
    volatile void __addr40 __mem *address,  /* address to operate on                */
    uint32_t count,                         /* number of 32-bit words to write      */
    sync_t sync,                            /* must be sig_done                     */
    SIGNAL_PAIR *sig_pair_ptr               /* signal pair to raise upon completion */
    )
{
    _INTRINSIC_BEGIN;
    {
        uint32_t hi_addr, low_addr;

        // cmd_mem_test_and_compare_write_ptr40() function requires silicon revision C0 or higher
       CT_ASSERT(__is_nfp_arch_or_above(__nfp6000, __REVISION_C0));
        _INTRINSIC_CONVERT_HI_LO_ADDRESS(address);
        CT_ASSERT(__is_read_reg(val));
        CT_ASSERT(__is_write_reg(data));
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done);
        __associate_read_write_reg_pair_no_spill(val, data);

        if (__is_ct_const(count) && count <= 8)
        {
            CT_ASSERT(count != 0);
            __asm mem[test_compare_write_or_incr, *val, hi_addr, <<8, low_addr, __ct_const_val(count)], sig_done[*__ct_sig_pair(sig_pair_ptr)]
        }
        else
        {
            if (!__is_ct_const(count))
            {
                CT_QPERFINFO_INDIRECT_REF(cmd_mem_test_and_compare_write_ptr40);
            }
            {
                _INTRINSIC_IND_ALU_COUNT_MIN_MAX(count, 1, MAX_REF_COUNT_8);
                __asm mem[test_compare_write_or_incr, *val, hi_addr, <<8, low_addr, __ct_const_val(count)], sig_done[*__ct_sig_pair(sig_pair_ptr)], indirect_ref
            }
        }
    }
    _INTRINSIC_END;
}


__intrinsic
void cmd_mem_test_and_compare_write_ind_ptr32(
    __xread uint32_t *val,                  /* returned pre-modified value          */
    __xwrite void *data,                    /* data to write                        */
    volatile void __addr32 __mem *address,  /* address to operate on                */
    uint32_t max_nn,                        /* max. number 32-bit words to write    */
    generic_ind_t ind,                      /* indirect word                        */
    sync_t sync,                            /* must be sig_done                     */
    SIGNAL_PAIR *sig_pair_ptr               /* signal pair to raise upon completion */
    )
{
    _INTRINSIC_BEGIN;
    {
        // cmd_mem_test_and_compare_write_ind_ptr32() function requires silicon revision C0 or higher
        CT_ASSERT(__is_nfp_arch_or_above(__nfp6000, __REVISION_C0));
        CT_ASSERT(__is_read_reg(val));
        CT_ASSERT(__is_write_reg(data));
        CT_ASSERT(__is_ct_const(max_nn));
        CT_ASSERT(max_nn >= 1 && max_nn <= 8);
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done);
        __associate_read_write_reg_pair_no_spill(val, data);

        __asm local_csr_wr[CMD_INDIRECT_REF_0, CSR_INDIRECT_TYPE(ind)]
        __asm
        {
            alu[--, --, B, ind]
            mem[test_compare_write_or_incr, *val, address, 0, __ct_const_val(max_nn)], sig_done[*__ct_sig_pair(sig_pair_ptr)], indirect_ref
        }
    }
    _INTRINSIC_END;
}

__intrinsic
void cmd_mem_test_and_compare_write_ind_ptr40(
    __xread uint32_t *val,                  /* returned pre-modified value          */
    __xwrite void *data,                    /* data to write                        */
    volatile void __addr40 __mem *address,  /* address to operate on                */
    uint32_t max_nn,                        /* max. number 32-bit words to write    */
    generic_ind_t ind,                      /* indirect word                        */
    sync_t sync,                            /* must be sig_done                     */
    SIGNAL_PAIR *sig_pair_ptr               /* signal pair to raise upon completion */
    )
{
    _INTRINSIC_BEGIN;
    {
        uint32_t hi_addr, low_addr;

        // cmd_mem_test_and_compare_write_ind_ptr40() function requires silicon revision C0 or higher
        CT_ASSERT(__is_nfp_arch_or_above(__nfp6000, __REVISION_C0));
        _INTRINSIC_CONVERT_HI_LO_ADDRESS(address);
        CT_ASSERT(__is_read_reg(val));
        CT_ASSERT(__is_write_reg(data));
        CT_ASSERT(__is_ct_const(max_nn));
        CT_ASSERT(max_nn >= 1 && max_nn <= 8);
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done);
        __associate_read_write_reg_pair_no_spill(val, data);


        __asm local_csr_wr[CMD_INDIRECT_REF_0, CSR_INDIRECT_TYPE(ind)]
        __asm
        {
            alu[--, --, B, ind]
            mem[test_compare_write_or_incr, *val, hi_addr, <<8, low_addr, __ct_const_val(max_nn)], sig_done[*__ct_sig_pair(sig_pair_ptr)], indirect_ref
        }
    }
    _INTRINSIC_END;
}


__intrinsic
void cmd_mem_write_atomic_ptr32(
    __xwrite void *data,                    /* data to write                        */
    volatile void __addr32 __mem *address,  /* address to operate on                */
    uint32_t count,                         /* number of 32-bit words to write      */
    sync_t sync,                            /* sig_done or ctx_swap                 */
    SIGNAL *sig_ptr                         /* signal to raise upon completion      */
    )
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_write_reg(data));
        _MEM_IMPLEMENT_COMMAND(atomic_write, cmd_mem_write_atomic_ptr32, 1, 8);
    }
    _INTRINSIC_END;
}

__intrinsic
void cmd_mem_write_atomic_ptr40(
    __xwrite void *data,                    /* data to write                        */
    volatile void __addr40 __mem *address,  /* address to operate on                */
    uint32_t count,                         /* number of 32-bit words to write      */
    sync_t sync,                            /* sig_done or ctx_swap                 */
    SIGNAL *sig_ptr                         /* signal to raise upon completion      */
    )
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_write_reg(data));
        _MEM_IMPLEMENT_COMMAND_PTR40(atomic_write, cmd_mem_write_atomic_ptr40, 1, 8);
    }
    _INTRINSIC_END;
}


__intrinsic
void cmd_mem_write_atomic_ind_ptr32(
    __xwrite void *data,                    /* data to write                        */
    volatile void __addr32 __mem *address,  /* address to operate on                */
    uint32_t max_nn,                        /* max. number of 32-bit words to write */
    generic_ind_t ind,                      /* indirect word                        */
    sync_t sync,                            /* sig_done or ctx_swap                 */
    SIGNAL *sig_ptr                         /* signal to raise upon completion      */
    )
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_write_reg(data));
        __asm local_csr_wr[CMD_INDIRECT_REF_0, CSR_INDIRECT_TYPE(ind)]
        _MEM_IMPLEMENT_INDIRECT_COMMAND(atomic_write, 1, MAX_REF_COUNT_8);
    }
    _INTRINSIC_END;
}


__intrinsic
void cmd_mem_write_atomic_ind_ptr40(
    __xwrite void *data,                    /* data to write                        */
    volatile void __addr40 __mem *address,  /* address to operate on                */
    uint32_t max_nn,                        /* max. number of 32-bit words to write */
    generic_ind_t ind,                      /* indirect word                        */
    sync_t sync,                            /* sig_done or ctx_swap                 */
    SIGNAL *sig_ptr                         /* signal to raise upon completion      */
    )
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_write_reg(data));
        __asm local_csr_wr[CMD_INDIRECT_REF_0, CSR_INDIRECT_TYPE(ind)]
        _MEM_IMPLEMENT_INDIRECT_COMMAND_PTR40(atomic_write, 1, MAX_REF_COUNT_8);
    }
    _INTRINSIC_END;
}


__intrinsic
void cmd_mem_swap_ptr32(
    __xread uint32_t *val,                  /* returned value                       */
    __xwrite void *data,                    /* data to swap                         */
    volatile void __addr32 __mem *address,  /* address to operate on                */
    uint32_t count,                         /* number of 32-bit words to write      */
    sync_t sync,                            /* must be sig_done                     */
    SIGNAL_PAIR *sig_pair_ptr               /* signal pair to raise upon completion */
    )
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_read_reg(val));
        CT_ASSERT(__is_write_reg(data));
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done);

        __associate_read_write_reg_pair_no_spill(val, data);
        if (__is_ct_const(count))
        {
            CT_ASSERT(count >= 1 && count <= 8);
            __asm mem[swap, *val, address, 0, __ct_const_val(count)], sig_done[*__ct_sig_pair(sig_pair_ptr)]
        }
        else
        {
            CT_QPERFINFO_INDIRECT_REF(cmd_mem_swap_ptr32);
            {
                _INTRINSIC_IND_ALU_COUNT_MIN_MAX(count, 1, 8);
                __asm mem[swap, *val, address, 0, __ct_const_val(count)], sig_done[*__ct_sig_pair(sig_pair_ptr)], indirect_ref
            }
        }
    }
    _INTRINSIC_END;
}

__intrinsic
void cmd_mem_swap_ptr40(
    __xread uint32_t *val,                  /* returned value                       */
    __xwrite void *data,                    /* data to swap                         */
    volatile void __addr40 __mem *address,  /* address to operate on                */
    uint32_t count,                         /* number of 32-bit words to write      */
    sync_t sync,                            /* must be sig_done                     */
    SIGNAL_PAIR *sig_pair_ptr               /* signal pair to raise upon completion */
    )
{
    _INTRINSIC_BEGIN;
    {
        uint32_t hi_addr, low_addr;
    _INTRINSIC_CONVERT_HI_LO_ADDRESS(address);
        CT_ASSERT(__is_read_reg(val));
        CT_ASSERT(__is_write_reg(data));
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done);

        __associate_read_write_reg_pair_no_spill(val, data);
        if (__is_ct_const(count))
        {
            CT_ASSERT(count >= 1 && count <= 8);
            __asm mem[swap, *val, hi_addr, <<8, low_addr, __ct_const_val(count)], sig_done[*__ct_sig_pair(sig_pair_ptr)]
        }
        else
        {
            CT_QPERFINFO_INDIRECT_REF(cmd_mem_swap_ptr40);
            {
                _INTRINSIC_IND_ALU_COUNT_MIN_MAX(count, 1, 8);
                __asm mem[swap, *val, hi_addr, <<8, low_addr, __ct_const_val(count)], sig_done[*__ct_sig_pair(sig_pair_ptr)], indirect_ref
            }
        }
    }
    _INTRINSIC_END;
}


__intrinsic
void cmd_mem_swap_ind_ptr32(
    __xread uint32_t *val,                  /* returned value                       */
    __xwrite void *data,                    /* data to swap                         */
    volatile void __addr32 __mem *address,  /* address to operate on                */
    uint32_t max_nn,                        /* max. number of 32-bit words to write */
    generic_ind_t ind,                      /* indirect word                        */
    sync_t sync,                            /* must be sig_done                     */
    SIGNAL_PAIR *sig_pair_ptr               /* signal pair to raise upon completion */
    )
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_read_reg(val));
        CT_ASSERT(__is_write_reg(data));
        CT_ASSERT(__is_ct_const(max_nn));
        CT_ASSERT(max_nn >= 1 && max_nn <= 8);
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done);
        __associate_read_write_reg_pair_no_spill(val, data);


        __asm local_csr_wr[CMD_INDIRECT_REF_0, CSR_INDIRECT_TYPE(ind)]
        __asm
        {
            alu[--, --, B, ind]
            mem[swap, *val, address, 0, __ct_const_val(max_nn)], sig_done[*__ct_sig_pair(sig_pair_ptr)], indirect_ref
        }
    }
    _INTRINSIC_END;
}

__intrinsic
void cmd_mem_swap_ind_ptr40(
    __xread uint32_t *val,                  /* returned value                       */
    __xwrite void *data,                    /* data to swap                         */
    volatile void __addr40 __mem *address,  /* address to operate on                */
    uint32_t max_nn,                        /* max. number of 32-bit words to write */
    generic_ind_t ind,                      /* indirect word                        */
    sync_t sync,                            /* must be sig_done                     */
    SIGNAL_PAIR *sig_pair_ptr               /* signal pair to raise upon completion */
    )
{
    _INTRINSIC_BEGIN;
    {
        uint32_t hi_addr, low_addr;
        _INTRINSIC_CONVERT_HI_LO_ADDRESS(address);
        CT_ASSERT(__is_read_reg(val));
        CT_ASSERT(__is_write_reg(data));
        CT_ASSERT(__is_ct_const(max_nn));
        CT_ASSERT(max_nn >= 1 && max_nn <= 8);
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done);
        __associate_read_write_reg_pair_no_spill(val, data);


        __asm local_csr_wr[CMD_INDIRECT_REF_0, CSR_INDIRECT_TYPE(ind)]
        __asm
        {
            alu[--, --, B, ind]
            mem[swap, *val, hi_addr, <<8, low_addr, __ct_const_val(max_nn)], sig_done[*__ct_sig_pair(sig_pair_ptr)], indirect_ref
        }
    }
    _INTRINSIC_END;
}


__intrinsic
void cmd_mem_write_atomic_imm_ptr32(
    uint32_t data,                          /* immediate data                       */
    volatile void __addr32 __mem *address,  /* address to operate on                */
    uint32_t count                          /* number of 32-bit words to write      */
    )
{
    _INTRINSIC_BEGIN;
    _MEM_IMMEDIATE_RANGE_CHECK_UNSIGNED(data, count, 8);
    _MEM_IMPLEMENT_IMMEDIATE_COMMAND(atomic_write_imm);
    _INTRINSIC_END;
}


__intrinsic
void cmd_mem_write_atomic_imm_ptr40(
    uint32_t data,                          /* immediate data                       */
    volatile void __addr40 __mem *address,  /* address to operate on                */
    uint32_t count                          /* number of 32-bit words to write      */
    )
{
    _INTRINSIC_BEGIN;
    _MEM_IMMEDIATE_RANGE_CHECK_UNSIGNED(data, count, 8);
    _MEM_IMPLEMENT_IMMEDIATE_COMMAND_PTR40(atomic_write_imm);
#ifndef __NFP_THIRD_PARTY_ADDRESSING_40_BIT
    _INTRINSIC_NEED_40_BIT_THIRD_PARTY();
#endif

    _INTRINSIC_END;
}


__intrinsic
void cmd_mem_swap_imm_ptr32(
    uint32_t data,                          /* immediate data                       */
    __xread void *rd_back,                  /* returned value                       */
    volatile void __addr32 __mem *address,  /* address to operate on                */
    uint32_t count,                         /* number of 32-bit words to write      */
    sync_t sync,                            /* sig_done or ctx_swap                 */
    SIGNAL *sig_ptr                         /* signal to raise upon completion      */
    )
{
    _INTRINSIC_BEGIN;

    {
        _MEM_TEST_IMMEDIATE_RANGE_CHECK_UNSIGNED(data, count, 8);
        CT_ASSERT(__is_read_reg(rd_back));
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done);
        {
            generic_ind_t ind;
            // override length and bytemask in ALU
            _INTRINSIC_OVERRIDE_LENGTH_AND_BYTEMASK_IN_ALU(ind, ((count - 1) & 7), (data & 0x7f));
            __asm
            {
                alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
                mem[swap_imm, *rd_back, address, 0, __ct_const_val(count)], sig_done[*sig_ptr], indirect_ref
            }
        }
    }
    _INTRINSIC_END;
}

__intrinsic
void cmd_mem_swap_imm_ptr40(
    uint32_t data,                          /* immediate data                       */
    __xread void *rd_back,                  /* returned value                       */
    volatile void __addr40 __mem *address,  /* address to operate on                */
    uint32_t count,                         /* number of 32-bit words to write      */
    sync_t sync,                            /* sig_done or ctx_swap                 */
    SIGNAL *sig_ptr                         /* signal to raise upon completion      */
    )
{
    _INTRINSIC_BEGIN;
    {
        _MEM_TEST_IMMEDIATE_RANGE_CHECK_UNSIGNED(data, count, 8);
        CT_ASSERT(__is_read_reg(rd_back));
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done);

        {
            generic_ind_t ind;
            uint32_t hi_addr, low_addr;
            _INTRINSIC_CONVERT_HI_LO_ADDRESS(address);

            // override length and byte mask in ALU
            _INTRINSIC_OVERRIDE_LENGTH_AND_BYTEMASK_IN_ALU(ind, ((count - 1) & 7), (data & 0x7f));

            __asm
            {
                alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
                mem[swap_imm, *rd_back, hi_addr, <<8, low_addr, __ct_const_val(count)], sig_done[*sig_ptr], indirect_ref
            }
        }
    }
    _INTRINSIC_END;
}

/* Bit setting operations */
__intrinsic
void cmd_mem_set_ptr32(
    __xwrite void *data,                    /* data to write                        */
    volatile void __addr32 __mem *address,  /* address to operate on                */
    uint32_t count,                         /* number of 32-bit words to write      */
    sync_t sync,                            /* sig_done or ctx_swap                 */
    SIGNAL *sig_ptr                         /* signal to raise upon completion      */
    )
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_write_reg(data));
        _MEM_IMPLEMENT_COMMAND(set, cmd_mem_set_ptr32, 1, 8);
    }
    _INTRINSIC_END;
}

__intrinsic
void cmd_mem_set_ptr40(
    __xwrite void *data,                    /* data to write                        */
    volatile void __addr40 __mem *address,  /* address to operate on                */
    uint32_t count,                         /* number of 32-bit words to write      */
    sync_t sync,                            /* sig_done or ctx_swap                 */
    SIGNAL *sig_ptr                         /* signal to raise upon completion      */
    )
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_write_reg(data));
        _MEM_IMPLEMENT_COMMAND_PTR40(set, cmd_mem_set_ptr40, 1, 8);
    }
    _INTRINSIC_END;
}


__intrinsic
void cmd_mem_set_ind_ptr32(
    __xwrite void *data,                    /* data to write                        */
    volatile void __addr32 __mem *address,  /* address to operate on                */
    uint32_t max_nn,                        /* max. number of 32-bit words to write */
    generic_ind_t ind,                      /* indirect word                        */
    sync_t sync,                            /* sig_done or ctx_swap                 */
    SIGNAL *sig_ptr                         /* signal to raise upon completion      */
    )
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_write_reg(data));
        __asm local_csr_wr[CMD_INDIRECT_REF_0, CSR_INDIRECT_TYPE(ind)]
        _MEM_IMPLEMENT_INDIRECT_COMMAND(set, 1, 8);
    }
    _INTRINSIC_END;
}

__intrinsic
void cmd_mem_set_ind_ptr40(
    __xwrite void *data,                    /* data to write                        */
    volatile void __addr40 __mem *address,  /* address to operate on                */
    uint32_t max_nn,                        /* max. number of 32-bit words to write */
    generic_ind_t ind,                      /* indirect word                        */
    sync_t sync,                            /* sig_done or ctx_swap                 */
    SIGNAL *sig_ptr                         /* signal to raise upon completion      */
    )
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_write_reg(data));
        __asm local_csr_wr[CMD_INDIRECT_REF_0, CSR_INDIRECT_TYPE(ind)]
        _MEM_IMPLEMENT_INDIRECT_COMMAND_PTR40(set, 1, 8);
    }
    _INTRINSIC_END;
}

__intrinsic
void cmd_mem_test_and_set_ptr32(
    __xread uint32_t *val,                  /* returned pre-modified value          */
    __xwrite void *data,                    /* mask of bits to set                  */
    volatile void __addr32 __mem *address,  /* address to operate on                */
    uint32_t count,                         /* number of 32-bit words to write      */
    sync_t sync,                            /* must be sig_done                     */
    SIGNAL_PAIR *sig_pair_ptr               /* signal pair to raise upon completion */
    )
{
    _INTRINSIC_BEGIN;
    {

        CT_ASSERT(__is_read_reg(val));
        CT_ASSERT(__is_write_reg(data));
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done);
        __associate_read_write_reg_pair_no_spill(val, data);

        if (__is_ct_const(count))
        {
            CT_ASSERT(count >= 1 && count <= 8);
            __asm mem[test_set, *val, address, 0, __ct_const_val(count)], sig_done[*__ct_sig_pair(sig_pair_ptr)]
        }
        else
        {
            CT_QPERFINFO_INDIRECT_REF(cmd_mem_test_and_set_ptr40);
            {
                _INTRINSIC_IND_ALU_COUNT_MIN_MAX(count, 1, 8);
                __asm mem[test_set, *val, address, 0, __ct_const_val(count)], sig_done[*__ct_sig_pair(sig_pair_ptr)], indirect_ref
            }
        }
    }
    _INTRINSIC_END;
}

__intrinsic
void cmd_mem_test_and_set_ptr40(
    __xread uint32_t *val,                  /* returned pre-modified value          */
    __xwrite void *data,                    /* mask of bits to set                  */
    volatile void __addr40 __mem *address,  /* address to operate on                */
    uint32_t count,                         /* number of 32-bit words to write      */
    sync_t sync,                            /* must be sig_done                     */
    SIGNAL_PAIR *sig_pair_ptr               /* signal pair to raise upon completion */
    )
{
    _INTRINSIC_BEGIN;
    {
        uint32_t hi_addr, low_addr;
        _INTRINSIC_CONVERT_HI_LO_ADDRESS(address);
        CT_ASSERT(__is_read_reg(val));
        CT_ASSERT(__is_write_reg(data));
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done);
        __associate_read_write_reg_pair_no_spill(val, data);

        if (__is_ct_const(count))
        {
            CT_ASSERT(count >= 1 && count <= 8);
            __asm mem[test_set, *val, hi_addr, <<8, low_addr, __ct_const_val(count)], sig_done[*__ct_sig_pair(sig_pair_ptr)]
        }
        else
        {
            CT_QPERFINFO_INDIRECT_REF(mem_test_set);
            {
                _INTRINSIC_IND_ALU_COUNT_MIN_MAX(count, 1, 8);
                __asm mem[test_set, *val, hi_addr, <<8, low_addr, __ct_const_val(count)], sig_done[*__ct_sig_pair(sig_pair_ptr)], indirect_ref
            }
        }
    }
    _INTRINSIC_END;
}


__intrinsic
void cmd_mem_test_and_set_ind_ptr32(
    __xread uint32_t *val,                  /* returned pre-modified value          */
    __xwrite void *data,                    /* mask of bits to set                  */
    volatile void __addr32 __mem *address,  /* address to operate on                */
    uint32_t max_nn,                        /* max. number of 32-bit words to write */
    generic_ind_t ind,                      /* indirect word                        */
    sync_t sync,                            /* must be sig_done                     */
    SIGNAL_PAIR *sig_pair_ptr               /* signal pair to raise upon completion */
    )
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_read_reg(val));
        CT_ASSERT(__is_write_reg(data));
        CT_ASSERT(__is_ct_const(max_nn));
        CT_ASSERT(max_nn >= 1 && max_nn <= 8);
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done);
        __associate_read_write_reg_pair_no_spill(val, data);

        __asm local_csr_wr[CMD_INDIRECT_REF_0, CSR_INDIRECT_TYPE(ind)]
        __asm
        {
            alu[--, --, B, ind]
            mem[test_set, *val, address, 0, __ct_const_val(max_nn)], sig_done[*__ct_sig_pair(sig_pair_ptr)], indirect_ref
        }
    }
    _INTRINSIC_END;
}

__intrinsic
void cmd_mem_test_and_set_ind_ptr40(
    __xread uint32_t *val,                  /* returned pre-modified value          */
    __xwrite void *data,                    /* mask of bits to set                  */
    volatile void __addr40 __mem *address,  /* address to operate on                */
    uint32_t max_nn,                        /* max. number of 32-bit words to write */
    generic_ind_t ind,                      /* indirect word                        */
    sync_t sync,                            /* must be sig_done                     */
    SIGNAL_PAIR *sig_pair_ptr               /* signal pair to raise upon completion */
    )
{
    _INTRINSIC_BEGIN;
    {
        uint32_t hi_addr, low_addr;
        _INTRINSIC_CONVERT_HI_LO_ADDRESS(address);
        CT_ASSERT(__is_read_reg(val));
        CT_ASSERT(__is_write_reg(data));
        CT_ASSERT(__is_ct_const(max_nn));
        CT_ASSERT(max_nn >= 1 && max_nn <= 8);
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done);
        __associate_read_write_reg_pair_no_spill(val, data);

        __asm local_csr_wr[CMD_INDIRECT_REF_0, CSR_INDIRECT_TYPE(ind)]
        __asm
        {
            alu[--, --, B, ind]
            mem[test_set, *val, hi_addr, <<8, low_addr, __ct_const_val(max_nn)], sig_done[*__ct_sig_pair(sig_pair_ptr)], indirect_ref
        }
    }
    _INTRINSIC_END;
}


__intrinsic
void cmd_mem_set_imm_ptr32(
    uint32_t data,                          /* immediate data                       */
    volatile void __addr32 __mem *address,  /* address to operate on                */
    uint32_t count                          /* number of 32-bit words to write      */
    )
{
    _INTRINSIC_BEGIN;
    {
        _MEM_IMMEDIATE_RANGE_CHECK_UNSIGNED(data, count, 8);
        _MEM_IMPLEMENT_IMMEDIATE_COMMAND(set_imm);
    }
    _INTRINSIC_END;
}

__intrinsic
void cmd_mem_set_imm_ptr40(
    uint32_t data,                          /* immediate data                       */
    volatile void __addr40 __mem *address,  /* address to operate on                */
    uint32_t count                          /* number of 32-bit words to write      */
    )
{
    _INTRINSIC_BEGIN;
    {
        _MEM_IMMEDIATE_RANGE_CHECK_UNSIGNED(data, count, 8);
        _MEM_IMPLEMENT_IMMEDIATE_COMMAND_PTR40(set_imm);
    }
#ifndef __NFP_THIRD_PARTY_ADDRESSING_40_BIT
_INTRINSIC_NEED_40_BIT_THIRD_PARTY();
#endif

    _INTRINSIC_END;
}

__intrinsic
void cmd_mem_test_and_set_imm_ptr32(
    uint32_t data,                          /* immediate data to set                */
    __xread void *rd_back,                  /* holds read back data                 */
    volatile void __addr32 __mem *address,  /* address to operate on                */
    uint32_t count,                         /* number of 32-bit words to write      */
    sync_t sync,                            /* must be sig_done                     */
    SIGNAL *sig_ptr                         /* signal to raise upon completion      */
    )
{
    _INTRINSIC_BEGIN;

    _MEM_TEST_IMMEDIATE_RANGE_CHECK_UNSIGNED(data, count, 8);
    CT_ASSERT(__is_read_reg(rd_back));
    CT_ASSERT(__is_ct_const(sync));
    CT_ASSERT(sync == sig_done);

    {
        generic_ind_t ind;
        // override length and byte mask
        _INTRINSIC_OVERRIDE_LENGTH_AND_BYTEMASK_IN_ALU(ind, ((count - 1) & 7), (data & 0x7f));

        __asm
        {
            alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
            mem[test_set_imm, *rd_back, address, 0, --], sig_done[*sig_ptr], num_sigs[1], indirect_ref
        }
    }

    _INTRINSIC_END;
}

__intrinsic
void cmd_mem_test_and_set_imm_ptr40(
    uint32_t data,                          /* immediate data to set                */
    __xread void *rd_back,                  /* holds read back data                 */
    volatile void __addr40 __mem *address,  /* address to operate on                */
    uint32_t count,                         /* number of 32-bit words to write      */
    sync_t sync,                            /* must be sig_done                     */
    SIGNAL *sig_ptr                         /* signal to raise upon completion      */
    )
{
    _INTRINSIC_BEGIN;

    _MEM_TEST_IMMEDIATE_RANGE_CHECK_UNSIGNED(data, count, 8);

    CT_ASSERT(__is_ct_const(sync));
    CT_ASSERT(sync == sig_done);

    {
        generic_ind_t ind;
        uint32_t hi_addr, low_addr;
        _INTRINSIC_CONVERT_HI_LO_ADDRESS(address);

        // override length and byte mask
        _INTRINSIC_OVERRIDE_LENGTH_AND_BYTEMASK_IN_ALU(ind, ((count - 1) & 7), (data & 0x7f));
        __asm
        {
            alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
            mem[test_set_imm, *rd_back, hi_addr, <<8, low_addr, --], sig_done[*sig_ptr], num_sigs[1], indirect_ref
    }
    }

    _INTRINSIC_END;
}

/* Bit clearing operations */
__intrinsic
void cmd_mem_clr_ptr32(
    __xwrite void *data,                    /* data to write                        */
    volatile void __addr32 __mem *address,  /* address to operate on                */
    uint32_t count,                         /* number of 32-bit words to write      */
    sync_t sync,                            /* sig_done or ctx_swap                 */
    SIGNAL *sig_ptr                         /* signal to raise upon completion      */
    )
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_write_reg(data));
        _MEM_IMPLEMENT_COMMAND(clr, cmd_mem_clr_ptr32, 1, 8);
    }
    _INTRINSIC_END;
}

__intrinsic
void cmd_mem_clr_ptr40(
    __xwrite void *data,                    /* data to write                        */
    volatile void __addr40 __mem *address,  /* address to operate on                */
    uint32_t count,                         /* number of 32-bit words to write      */
    sync_t sync,                            /* sig_done or ctx_swap                 */
    SIGNAL *sig_ptr                         /* signal to raise upon completion      */
    )
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_write_reg(data));
        _MEM_IMPLEMENT_COMMAND_PTR40(clr, cmd_mem_clr_ptr40, 1, 8);
    }
    _INTRINSIC_END;
}


__intrinsic
void cmd_mem_clr_ind_ptr32(
    __xwrite void *data,                    /* data to write                        */
    volatile void __addr32 __mem *address,  /* address to operate on                */
    uint32_t max_nn,                        /* max. number of 32-bit words to write */
    generic_ind_t ind,                      /* indirect word                        */
    sync_t sync,                            /* sig_done or ctx_swap                 */
    SIGNAL *sig_ptr                         /* signal to raise upon completion      */
    )
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_write_reg(data));
        __asm local_csr_wr[CMD_INDIRECT_REF_0, CSR_INDIRECT_TYPE(ind)]
        _MEM_IMPLEMENT_INDIRECT_COMMAND(clr, 1, 8);
    }
    _INTRINSIC_END;
}

__intrinsic
void cmd_mem_clr_ind_ptr40(
    __xwrite void *data,                    /* data to write                        */
    volatile void __addr40 __mem *address,  /* address to operate on                */
    uint32_t max_nn,                        /* max. number of 32-bit words to write */
    generic_ind_t ind,                      /* indirect word                        */
    sync_t sync,                            /* sig_done or ctx_swap                 */
    SIGNAL *sig_ptr                         /* signal to raise upon completion      */
    )
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_write_reg(data));
        __asm local_csr_wr[CMD_INDIRECT_REF_0, CSR_INDIRECT_TYPE(ind)]
        _MEM_IMPLEMENT_INDIRECT_COMMAND_PTR40(clr, 1, 8);
    }
    _INTRINSIC_END;
}


__intrinsic
void cmd_mem_test_and_clr_ptr32(
    __xread uint32_t *val,                  /* returned pre-modified value          */
    __xwrite void *data,                    /* mask of bits to set                  */
    volatile void __addr32 __mem *address,  /* address to operate on                */
    uint32_t count,                         /* number of 32-bit words to write      */
    sync_t sync,                            /* must be sig_done                     */
    SIGNAL_PAIR *sig_pair_ptr               /* signal pair to raise upon completion */
    )
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_read_reg(val));
        CT_ASSERT(__is_write_reg(data));
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done);
        __associate_read_write_reg_pair_no_spill(val, data);
        if (__is_ct_const(count))
        {
            CT_ASSERT(count >= 1 && count <= 8);
            __asm mem[test_clr, *val, address, 0, __ct_const_val(count)], sig_done[*__ct_sig_pair(sig_pair_ptr)]
        }
        else
        {
            CT_QPERFINFO_INDIRECT_REF(cmd_mem_test_and_clr_ptr32);
            {
                _INTRINSIC_IND_ALU_COUNT_MIN_MAX(count, 1, 8);
                __asm mem[test_clr, *val, address, 0, __ct_const_val(count)], sig_done[*__ct_sig_pair(sig_pair_ptr)], indirect_ref
            }
        }
    }
    _INTRINSIC_END;
}

__intrinsic
void cmd_mem_test_and_clr_ptr40(
    __xread uint32_t *val,                  /* returned pre-modified value          */
    __xwrite void *data,                    /* mask of bits to set                  */
    volatile void __addr40 __mem *address,  /* address to operate on                */
    uint32_t count,                         /* number of 32-bit words to write      */
    sync_t sync,                            /* must be sig_done                     */
    SIGNAL_PAIR *sig_pair_ptr               /* signal pair to raise upon completion */
    )
{
    _INTRINSIC_BEGIN;
    {
        uint32_t hi_addr, low_addr;
        _INTRINSIC_CONVERT_HI_LO_ADDRESS(address);
        CT_ASSERT(__is_read_reg(val));
        CT_ASSERT(__is_write_reg(data));
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done);
        __associate_read_write_reg_pair_no_spill(val, data);
        if (__is_ct_const(count))
        {
            CT_ASSERT(count >= 1 && count <= 8);
            __asm mem[test_clr, *val, hi_addr, <<8, low_addr, __ct_const_val(count)], sig_done[*__ct_sig_pair(sig_pair_ptr)]
        }
        else
        {
            CT_QPERFINFO_INDIRECT_REF(cmd_mem_test_and_clr_ptr40);
            {
                _INTRINSIC_IND_ALU_COUNT_MIN_MAX(count, 1, 8);
                __asm mem[test_clr, *val, hi_addr, <<8, low_addr, __ct_const_val(count)], sig_done[*__ct_sig_pair(sig_pair_ptr)], indirect_ref
            }
        }
    }
    _INTRINSIC_END;
}


__intrinsic
void cmd_mem_test_and_clr_ind_ptr32(
    __xread uint32_t *val,                  /* returned pre-modified value          */
    __xwrite void *data,                    /* mask of bits to clear                */
    volatile void __addr32 __mem *address,  /* address to operate on                */
    uint32_t max_nn,                        /* max. number of 32-bit words to write */
    generic_ind_t ind,                      /* indirect word                        */
    sync_t sync,                            /* must be sig_done                     */
    SIGNAL_PAIR *sig_pair_ptr               /* signal pair to raise upon completion */
    )
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_read_reg(val));
        CT_ASSERT(__is_write_reg(data));
        CT_ASSERT(__is_ct_const(max_nn));
        CT_ASSERT(max_nn >= 1 && max_nn <= 8);
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done);
        __associate_read_write_reg_pair_no_spill(val, data);

        __asm local_csr_wr[CMD_INDIRECT_REF_0, CSR_INDIRECT_TYPE(ind)]
        __asm
        {
            alu[--, --, B, ind]
            mem[test_clr, *val, address, 0, __ct_const_val(max_nn)], sig_done[*__ct_sig_pair(sig_pair_ptr)], indirect_ref
        }
    }
    _INTRINSIC_END;
}

__intrinsic
void cmd_mem_test_and_clr_ind_ptr40(
    __xread uint32_t *val,                  /* returned pre-modified value          */
    __xwrite void *data,                    /* mask of bits to clear                */
    volatile void __addr40 __mem *address,  /* address to operate on                */
    uint32_t max_nn,                        /* max. number of 32-bit words to write */
    generic_ind_t ind,                      /* indirect word                        */
    sync_t sync,                            /* must be sig_done                     */
    SIGNAL_PAIR *sig_pair_ptr               /* signal pair to raise upon completion */
    )
{
    _INTRINSIC_BEGIN;
    {
        uint32_t hi_addr, low_addr;
        _INTRINSIC_CONVERT_HI_LO_ADDRESS(address);
        CT_ASSERT(__is_read_reg(val));
        CT_ASSERT(__is_write_reg(data));
        CT_ASSERT(__is_ct_const(max_nn));
        CT_ASSERT(max_nn >= 1 && max_nn <= 8);
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done);
        __associate_read_write_reg_pair_no_spill(val, data);

        __asm local_csr_wr[CMD_INDIRECT_REF_0, CSR_INDIRECT_TYPE(ind)]
        __asm
        {
            alu[--, --, B, ind]
            mem[test_clr, *val, hi_addr, <<8, low_addr, __ct_const_val(max_nn)], sig_done[*__ct_sig_pair(sig_pair_ptr)], indirect_ref
        }
    }
    _INTRINSIC_END;
}


__intrinsic
void cmd_mem_clr_imm_ptr32(
    uint32_t data,                          /* immediate data                       */
    volatile void __addr32 __mem *address,  /* address to operate on                */
    uint32_t count                          /* number of 32-bit words to write      */
    )
{
    _INTRINSIC_BEGIN;
    {
        _MEM_IMMEDIATE_RANGE_CHECK_UNSIGNED(data, count, 8);
        _MEM_IMPLEMENT_IMMEDIATE_COMMAND(clr_imm);
    }
    _INTRINSIC_END;
}


__intrinsic
void cmd_mem_clr_imm_ptr40(
    uint32_t data,                          /* immediate data                       */
    volatile void __addr40 __mem *address,  /* address to operate on                */
    uint32_t count                          /* number of 32-bit words to write      */
    )
{
    _INTRINSIC_BEGIN;
    {
        _MEM_IMMEDIATE_RANGE_CHECK_UNSIGNED(data, count, 8);
        _MEM_IMPLEMENT_IMMEDIATE_COMMAND_PTR40(clr_imm);
    }

#ifndef __NFP_THIRD_PARTY_ADDRESSING_40_BIT
_INTRINSIC_NEED_40_BIT_THIRD_PARTY();
#endif

    _INTRINSIC_END;
}

__intrinsic
void cmd_mem_test_and_clr_imm_ptr32(
    uint32_t data,                          /* immediate data                       */
    __xread void *rd_back,                  /* holds read back data                 */
    volatile void __addr32 __mem *address,  /* address to operate on                */
    uint32_t count,                         /* number of 32-bit words to write      */
    sync_t sync,                            /* must be sig_done                     */
    SIGNAL *sig_ptr                         /* signal to raise upon completion      */
    )
{
    _INTRINSIC_BEGIN;
    _MEM_TEST_IMMEDIATE_RANGE_CHECK_UNSIGNED(data, count, 8);
    CT_ASSERT(__is_ct_const(sync));
    CT_ASSERT(sync == sig_done);

    {
        generic_ind_t ind;

        // override length and bytemask
        _INTRINSIC_OVERRIDE_LENGTH_AND_BYTEMASK_IN_ALU(ind, ((count - 1) & 7), (data & 0x7f));
        __asm
        {
            alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
            mem[test_clr_imm, *rd_back, address, 0, --], sig_done[*sig_ptr], num_sigs[1], indirect_ref
        }
    }

    _INTRINSIC_END;
}

__intrinsic
void cmd_mem_test_and_clr_imm_ptr40(
    uint32_t data,                          /* immediate data                       */
    __xread void *rd_back,                  /* holds read back data                 */
    volatile void __addr40 __mem *address,  /* address to operate on                */
    uint32_t count,                         /* number of 32-bit words to write      */
    sync_t sync,                            /* must be sig_done                     */
    SIGNAL *sig_ptr                         /* signal to raise upon completion      */
    )
{
    _INTRINSIC_BEGIN;

    _MEM_TEST_IMMEDIATE_RANGE_CHECK_UNSIGNED(data, count, 8);
    CT_ASSERT(__is_read_reg(rd_back));
    CT_ASSERT(__is_ct_const(sync));
    CT_ASSERT(sync == sig_done);

    {
        generic_ind_t ind;
        uint32_t hi_addr, low_addr;
        _INTRINSIC_CONVERT_HI_LO_ADDRESS(address);

        // override length and byte mask
        _INTRINSIC_OVERRIDE_LENGTH_AND_BYTEMASK_IN_ALU(ind, ((count - 1) & 7), (data & 0x7f));
        __asm
        {
            alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
            mem[test_clr_imm, *rd_back, hi_addr, <<8, low_addr, --], sig_done[*sig_ptr], num_sigs[1], indirect_ref
        }
    }

    _INTRINSIC_END;
}

/* Add operations */
__intrinsic
void cmd_mem_add32_ptr32(
    __xwrite void *data,                    /* data to write                        */
    volatile void __addr32 __mem *address,  /* address to operate on                */
    uint32_t count,                         /* number of 32-bit words to write      */
    sync_t sync,                            /* sig_done or ctx_swap                 */
    SIGNAL *sig_ptr                         /* signal to raise upon completion      */
    )
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_write_reg(data));
        _MEM_IMPLEMENT_COMMAND(add, cmd_mem_add32_ptr32, 1, 4);
    }
    _INTRINSIC_END;
}

__intrinsic
void cmd_mem_add32_ptr40(
    __xwrite void *data,                    /* data to write                        */
    volatile void __addr40 __mem *address,  /* address to operate on                */
    uint32_t count,                         /* number of 32-bit words to write      */
    sync_t sync,                            /* sig_done or ctx_swap                 */
    SIGNAL *sig_ptr                         /* signal to raise upon completion      */
    )
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_write_reg(data));
        _MEM_IMPLEMENT_COMMAND_PTR40(add, cmd_mem_add32_ptr40, 1, 4);
    }
    _INTRINSIC_END;
}


__intrinsic
void cmd_mem_add32_ind_ptr32(
    __xwrite void *data,                    /* data to write                        */
    volatile void __addr32 __mem *address,  /* address to operate on                */
    uint32_t max_nn,                        /* number of 32-bit words to write      */
    generic_ind_t ind,                      /* indirect word                        */
    sync_t sync,                            /* sig_done or ctx_swap                 */
    SIGNAL *sig_ptr                         /* signal to raise upon completion      */
    )
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_write_reg(data));
        __asm local_csr_wr[CMD_INDIRECT_REF_0, CSR_INDIRECT_TYPE(ind)]
        CT_ASSERT(__is_write_reg(data));
        _MEM_IMPLEMENT_INDIRECT_COMMAND(add, 1, 4);
    }
    _INTRINSIC_END;
}


__intrinsic
void cmd_mem_add32_ind_ptr40(
    __xwrite void *data,                    /* data to write                        */
    volatile void __addr40 __mem *address,  /* address to operate on                */
    uint32_t max_nn,                        /* number of 32-bit words to write      */
    generic_ind_t ind,                      /* indirect word                        */
    sync_t sync,                            /* sig_done or ctx_swap                 */
    SIGNAL *sig_ptr                         /* signal to raise upon completion      */
    )
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_write_reg(data));
        __asm local_csr_wr[CMD_INDIRECT_REF_0, CSR_INDIRECT_TYPE(ind)]
        CT_ASSERT(__is_write_reg(data));
        _MEM_IMPLEMENT_INDIRECT_COMMAND_PTR40(add, 1, 4);
    }
    _INTRINSIC_END;
}


__intrinsic
void cmd_mem_add64_ptr32(
    __xwrite void *data,                    /* data to write                        */
    volatile void __addr32 __mem *address,  /* address to operate on                */
    uint32_t count,                         /* number of 32-bit words to write      */
    sync_t sync,                            /* sig_done or ctx_swap                 */
    SIGNAL *sig_ptr                         /* signal to raise upon completion      */
    )
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_write_reg(data));
        _MEM_IMPLEMENT_ARITHMETIC64_COMMAND(add64, cmd_mem_add64_ptr32, 1, 4);
    }
    _INTRINSIC_END;
}

__intrinsic
void cmd_mem_add64_ptr40(
    __xwrite void *data,                    /* data to write                        */
    volatile void __addr40 __mem *address,  /* address to operate on                */
    uint32_t count,                         /* number of 32-bit words to write      */
    sync_t sync,                            /* sig_done or ctx_swap                 */
    SIGNAL *sig_ptr                         /* signal to raise upon completion      */
    )
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_write_reg(data));
        _MEM_IMPLEMENT_ARITHMETIC64_COMMAND_PTR40(add64, cmd_mem_add64_ptr40, 1, 4);
    }
    _INTRINSIC_END;
}


__intrinsic
void cmd_mem_add64_ind_ptr32(
    __xwrite void *data,                    /* data to write                        */
    volatile void __addr32 __mem *address,  /* address to operate on                */
    uint32_t max_nn,                        /* number of 64-bit words to write      */
    generic_ind_t ind,                      /* indirect word                        */
    sync_t sync,                            /* sig_done or ctx_swap                 */
    SIGNAL *sig_ptr                         /* signal to raise upon completion      */
    )
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_write_reg(data));
        __asm local_csr_wr[CMD_INDIRECT_REF_0, CSR_INDIRECT_TYPE(ind)]
        CT_ASSERT(__is_write_reg(data));
        _MEM_IMPLEMENT_INDIRECT_COMMAND(add64, 1, 4);
    }
    _INTRINSIC_END;
}


__intrinsic
void cmd_mem_add64_ind_ptr40(
    __xwrite void *data,                    /* data to write                        */
    volatile void __addr40 __mem *address,  /* address to operate on                */
    uint32_t max_nn,                        /* number of 64-bit words to write      */
    generic_ind_t ind,                      /* indirect word                        */
    sync_t sync,                            /* sig_done or ctx_swap                 */
    SIGNAL *sig_ptr                         /* signal to raise upon completion      */
    )
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_write_reg(data));
        __asm local_csr_wr[CMD_INDIRECT_REF_0, CSR_INDIRECT_TYPE(ind)]
        CT_ASSERT(__is_write_reg(data));
        _MEM_IMPLEMENT_INDIRECT_COMMAND_PTR40(add64, 1, 4);
    }
    _INTRINSIC_END;
}


/* Test and add operations */
__intrinsic
void cmd_mem_test_and_add32_ptr32(
    __xread uint32_t *val,                  /* returned pre-modified value          */
    __xwrite void *data,                    /* data to add                          */
    volatile void __addr32 __mem *address,  /* address to operate on                */
    uint32_t count,                         /* number of 32-bit words to write      */
    sync_t sync,                            /* must be sig_done                     */
    SIGNAL_PAIR *sig_pair_ptr               /* signal pair to raise upon completion */
    )
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_read_reg(val));
        CT_ASSERT(__is_write_reg(data));
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done);
        __associate_read_write_reg_pair_no_spill(val, data);

        if (__is_ct_const(count))
        {
            CT_ASSERT(count >= 1 && count <= 4);
            __asm mem[test_add, *val, address, 0, __ct_const_val(count)], sig_done[*__ct_sig_pair(sig_pair_ptr)]
        }
        else
        {
            CT_QPERFINFO_INDIRECT_REF(cmd_mem_test_and_add32_ptr32);
            {
                _INTRINSIC_IND_ALU_COUNT_MIN_MAX(count, 1, 4);
                __asm mem[test_add, *val, address, 0, __ct_const_val(count)], sig_done[*__ct_sig_pair(sig_pair_ptr)], indirect_ref
            }
        }
    }
    _INTRINSIC_END;
}

__intrinsic
void cmd_mem_test_and_add32_ptr40(
    __xread uint32_t *val,                  /* returned pre-modified value          */
    __xwrite void *data,                    /* data to add                          */
    volatile void __addr40 __mem *address,  /* address to operate on                */
    uint32_t count,                         /* number of 32-bit words to write      */
    sync_t sync,                            /* must be sig_done                     */
    SIGNAL_PAIR *sig_pair_ptr               /* signal pair to raise upon completion */
    )
{
    _INTRINSIC_BEGIN;
    {
        uint32_t hi_addr, low_addr;
        _INTRINSIC_CONVERT_HI_LO_ADDRESS(address);
        CT_ASSERT(__is_read_reg(val));
        CT_ASSERT(__is_write_reg(data));
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done);
        __associate_read_write_reg_pair_no_spill(val, data);

        if (__is_ct_const(count))
        {
            CT_ASSERT(count >= 1 && count <= 4);
            __asm mem[test_add, *val, hi_addr, <<8, low_addr, __ct_const_val(count)], sig_done[*__ct_sig_pair(sig_pair_ptr)]
        }
        else
        {
            CT_QPERFINFO_INDIRECT_REF(cmd_mem_test_and_add32_ptr40);
            {
                _INTRINSIC_IND_ALU_COUNT_MIN_MAX(count, 1, 4);
                __asm mem[test_add, *val, hi_addr, <<8, low_addr, __ct_const_val(count)], sig_done[*__ct_sig_pair(sig_pair_ptr)], indirect_ref
            }
        }
    }
    _INTRINSIC_END;
}


__intrinsic
void cmd_mem_test_and_add32_ind_ptr32(
    __xread uint32_t *val,                  /* returned pre-modified value          */
    __xwrite void *data,                    /* data to add                          */
    volatile void __addr32 __mem *address,  /* address to operate on                */
    uint32_t max_nn,                        /* max. number of 32-bit words to write */
    generic_ind_t ind,                      /* indirect word                        */
    sync_t sync,                            /* must be sig_done                     */
    SIGNAL_PAIR *sig_pair_ptr               /* signal pair to raise upon completion */
    )
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_read_reg(val));
        CT_ASSERT(__is_write_reg(data));
        CT_ASSERT(__is_ct_const(max_nn));
        CT_ASSERT(max_nn >= 1 && max_nn <= 4);
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done);
        __associate_read_write_reg_pair_no_spill(val, data);
        __asm local_csr_wr[CMD_INDIRECT_REF_0, CSR_INDIRECT_TYPE(ind)]

        __asm
        {
            alu[--, --, B, ind]
            mem[test_add, *val, address, 0, __ct_const_val(max_nn)], sig_done[*__ct_sig_pair(sig_pair_ptr)], indirect_ref
        }
    }
    _INTRINSIC_END;
}

__intrinsic
void cmd_mem_test_and_add32_ind_ptr40(
    __xread uint32_t *val,                  /* returned pre-modified value          */
    __xwrite void *data,                    /* data to add                          */
    volatile void __addr40 __mem *address,  /* address to operate on                */
    uint32_t max_nn,                        /* max. number of 32-bit words to write */
    generic_ind_t ind,                      /* indirect word                        */
    sync_t sync,                            /* must be sig_done                     */
    SIGNAL_PAIR *sig_pair_ptr               /* signal pair to raise upon completion */
    )
{
    _INTRINSIC_BEGIN;
    {
        uint32_t hi_addr, low_addr;
        _INTRINSIC_CONVERT_HI_LO_ADDRESS(address);
        CT_ASSERT(__is_read_reg(val));
        CT_ASSERT(__is_write_reg(data));
        CT_ASSERT(__is_ct_const(max_nn));
        CT_ASSERT(max_nn >= 1 && max_nn <= 4);
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done);
        __associate_read_write_reg_pair_no_spill(val, data);

        __asm local_csr_wr[CMD_INDIRECT_REF_0, CSR_INDIRECT_TYPE(ind)]
        __asm
        {
            alu[--, --, B, ind]
            mem[test_add, *val, hi_addr, <<8, low_addr, __ct_const_val(max_nn)], sig_done[*__ct_sig_pair(sig_pair_ptr)], indirect_ref
        }
    }
    _INTRINSIC_END;
}


__intrinsic
void cmd_mem_test_and_add64_ptr32(
    __xread uint32_t *val,                  /* returned pre-modified value          */
    __xwrite void *data,                    /* data to add                          */
    volatile void __addr32 __mem *address,  /* address to operate on                */
    uint32_t count,                         /* number of 32-bit words to write      */
    sync_t sync,                            /* must be sig_done                     */
    SIGNAL_PAIR *sig_pair_ptr               /* signal pair to raise upon completion */
    )
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_read_reg(val));
        CT_ASSERT(__is_write_reg(data));
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done);
        __associate_read_write_reg_pair_no_spill(val, data);

        if (__is_ct_const(count))
        {
            CT_ASSERT(count >= 1 && count <= 4);
            __asm mem[test_add64, *val, address, 0, __ct_const_val(count)], sig_done[*__ct_sig_pair(sig_pair_ptr)]
        }
        else
        {
            CT_QPERFINFO_INDIRECT_REF(cmd_mem_add64_ptr32);
            {
                _MEM_ARITHMETIC_64_IND_ALU_COUNT_MIN_MAX(count, 1, 4);
                __asm mem[test_add64, *val, address, 0, __ct_const_val(count)], sig_done[*__ct_sig_pair(sig_pair_ptr)], indirect_ref
            }
        }
    }
    _INTRINSIC_END;
}

__intrinsic
void cmd_mem_test_and_add64_ptr40(
    __xread uint32_t *val,                  /* returned pre-modified value          */
    __xwrite void *data,                    /* data to add                          */
    volatile void __addr40 __mem *address,  /* address to operate on                */
    uint32_t count,                         /* number of 32-bit words to write      */
    sync_t sync,                            /* must be sig_done                     */
    SIGNAL_PAIR *sig_pair_ptr               /* signal pair to raise upon completion */
    )
{
    _INTRINSIC_BEGIN;
    {
        uint32_t hi_addr, low_addr;
        _INTRINSIC_CONVERT_HI_LO_ADDRESS(address);
        CT_ASSERT(__is_read_reg(val));
        CT_ASSERT(__is_write_reg(data));
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done);
        __associate_read_write_reg_pair_no_spill(val, data);

        if (__is_ct_const(count))
        {
            CT_ASSERT(count >= 1 && count <= 4);
            __asm mem[test_add64, *val, hi_addr, <<8, low_addr, __ct_const_val(count)], sig_done[*__ct_sig_pair(sig_pair_ptr)]
        }
        else
        {
            CT_QPERFINFO_INDIRECT_REF(cmd_mem_add64_ptr32);
            {
                _MEM_ARITHMETIC_64_IND_ALU_COUNT_MIN_MAX(count, 1, 4);
                __asm mem[test_add64, *val, hi_addr, <<8, low_addr, __ct_const_val(count)], sig_done[*__ct_sig_pair(sig_pair_ptr)], indirect_ref
            }
        }
    }
    _INTRINSIC_END;
}


__intrinsic
void cmd_mem_test_and_add64_ind_ptr32(
    __xread uint32_t *val,                  /* returned pre-modified value          */
    __xwrite void *data,                    /* data to add                          */
    volatile void __addr32 __mem *address,  /* address to operate on                */
    uint32_t max_nn,                        /* max. number of 32-bit words to write */
    generic_ind_t ind,                      /* indirect word                        */
    sync_t sync,                            /* must be sig_done                     */
    SIGNAL_PAIR *sig_pair_ptr               /* signal pair to raise upon completion */
    )
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_read_reg(val));
        CT_ASSERT(__is_write_reg(data));
        CT_ASSERT(__is_ct_const(max_nn));
        CT_ASSERT(max_nn >= 1 && max_nn <= 4);
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done);
        __associate_read_write_reg_pair_no_spill(val, data);

        __asm local_csr_wr[CMD_INDIRECT_REF_0, CSR_INDIRECT_TYPE(ind)]
        __asm
        {
            alu[--, --, B, ind]
            mem[test_add64, *val, address, 0, __ct_const_val(max_nn)], sig_done[*__ct_sig_pair(sig_pair_ptr)], indirect_ref
        }
    }
    _INTRINSIC_END;
}

__intrinsic
void cmd_mem_test_and_add64_ind_ptr40(
    __xread uint32_t *val,                  /* returned pre-modified value          */
    __xwrite void *data,                    /* data to add                          */
    volatile void __addr40 __mem *address,  /* address to operate on                */
    uint32_t max_nn,                        /* max. number of 32-bit words to write */
    generic_ind_t ind,                      /* indirect word                        */
    sync_t sync,                            /* must be sig_done                     */
    SIGNAL_PAIR *sig_pair_ptr               /* signal pair to raise upon completion */
    )
{
    _INTRINSIC_BEGIN;
    {
        uint32_t hi_addr, low_addr;
        _INTRINSIC_CONVERT_HI_LO_ADDRESS(address);
        CT_ASSERT(__is_read_reg(val));
        CT_ASSERT(__is_write_reg(data));
        CT_ASSERT(__is_ct_const(max_nn));
        CT_ASSERT(max_nn >= 1 && max_nn <= 4);
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done);
        __associate_read_write_reg_pair_no_spill(val, data);

        __asm local_csr_wr[CMD_INDIRECT_REF_0, CSR_INDIRECT_TYPE(ind)]
        __asm
        {
            alu[--, --, B, ind]
            mem[test_add64, *val, hi_addr, <<8, low_addr, __ct_const_val(max_nn)], sig_done[*__ct_sig_pair(sig_pair_ptr)], indirect_ref
        }
    }
    _INTRINSIC_END;
}


/* Add immediate operations */
__intrinsic
void cmd_mem_add32_imm_ptr32(
    int data,                               /* immediate data                       */
    volatile void __addr32 __mem *address,  /* address to operate on                */
    uint32_t count                          /* number of 32-bit words to write      */
    )
{
    _INTRINSIC_BEGIN;
    {
        _MEM_IMMEDIATE_RANGE_CHECK_SIGNED(data, count, 4);
        _MEM_IMPLEMENT_IMMEDIATE_COMMAND(add_imm);
    }
    _INTRINSIC_END;
}


__intrinsic
void cmd_mem_add32_imm_ptr40(
    int data,                               /* immediate data                       */
    volatile void __addr40 __mem *address,  /* address to operate on                */
    uint32_t count                          /* number of 32-bit words to write      */
    )
{
    _INTRINSIC_BEGIN;
    {
        _MEM_IMMEDIATE_RANGE_CHECK_SIGNED(data, count, 4);
        _MEM_IMPLEMENT_IMMEDIATE_COMMAND_PTR40(add_imm);
    }
    _INTRINSIC_END;
}


/* Increment 32 integer */
__intrinsic
void cmd_mem_incr32_ptr32(
    volatile void __addr32 __mem *address           /* address to increment                 */
    )
{
    _INTRINSIC_BEGIN;
    {
        __asm
        {
            mem[incr, --, address, 0]
        }
    }
    _INTRINSIC_END;
}



/* Increment 32 integer */
__intrinsic
void cmd_mem_incr32_ptr40(
    volatile void __addr40 __mem *address     /* address to increment                 */
    )
{
    _INTRINSIC_BEGIN;
    {
        uint32_t hi_addr, low_addr;
        _INTRINSIC_CONVERT_HI_LO_ADDRESS(address);
        __asm
        {
            mem[incr, --, hi_addr, <<8, low_addr]
        }
    }
    _INTRINSIC_END;
}

__intrinsic
void cmd_mem_test_and_add32_imm_ptr32(
    int data,                               /* immediate data                       */
    __xread void *rd_back,                  /* holds read back data                 */
    volatile void __addr32 __mem *address,  /* address to operate on                */
    uint32_t count,                         /* number of 32-bit words to write      */
    sync_t sync,                            /* must be sig_done                     */
    SIGNAL *sig_ptr                         /* signal to raise upon completion      */
    )
{
    _INTRINSIC_BEGIN;
    _MEM_TEST_IMMEDIATE_RANGE_CHECK_SIGNED(data, count, 4);
    CT_ASSERT(__is_read_reg(rd_back));
    CT_ASSERT(__is_ct_const(sync));
    CT_ASSERT(sync == sig_done);

    {
        generic_ind_t ind;

        // override length and bytemask
        _INTRINSIC_OVERRIDE_LENGTH_AND_BYTEMASK_IN_ALU(ind, ((count - 1) & 3), (data & 0xff));

        __asm
        {
            alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
            mem[test_add_imm, *rd_back, address, 0], sig_done[*sig_ptr], num_sigs[1], indirect_ref
        }
    }
    _INTRINSIC_END;
}

__intrinsic
void cmd_mem_test_and_add32_imm_ptr40(
    int data,                               /* immediate data                       */
    __xread void *rd_back,                  /* holds read back data                 */
    volatile void __addr40 __mem *address,  /* address to operate on                */
    uint32_t count,                         /* number of 32-bit words to write      */
    sync_t sync,                            /* must be sig_done                     */
    SIGNAL *sig_ptr                         /* signal to raise upon completion      */
    )
{
    _INTRINSIC_BEGIN;
    _MEM_TEST_IMMEDIATE_RANGE_CHECK_SIGNED(data, count, 4);
    CT_ASSERT(__is_read_reg(rd_back));
    CT_ASSERT(__is_ct_const(sync));
    CT_ASSERT(sync == sig_done);

    {
        generic_ind_t ind;
        uint32_t hi_addr, low_addr;
        _INTRINSIC_CONVERT_HI_LO_ADDRESS(address);

        // override length and bytemask
        _INTRINSIC_OVERRIDE_LENGTH_AND_BYTEMASK_IN_ALU(ind, ((count - 1) & 3), (data & 0xff));
        __asm
        {
            alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
            mem[test_add_imm, *rd_back, hi_addr, <<8, low_addr], sig_done[*sig_ptr], num_sigs[1], indirect_ref
        }
    }

    _INTRINSIC_END;
}

__intrinsic
void cmd_mem_add64_imm_ptr32(
    int data,                               /* immediate data                       */
    volatile void __addr32 __mem *address,  /* address to operate on                */
    uint32_t count                          /* number of 64-bit words to write      */
    )
{
    _INTRINSIC_BEGIN;
    {
        _MEM_IMMEDIATE_RANGE_CHECK_SIGNED(data, count, 4);
        _MEM_IMPLEMENT_IMMEDIATE_64BIT_COMMAND(add64_imm);
    }
    _INTRINSIC_END;
}


__intrinsic
void cmd_mem_add64_imm_ptr40(
    int data,                               /* immediate data                       */
    volatile void __addr40 __mem *address,  /* address to operate on                */
    uint32_t count                          /* number of 64-bit words to write      */
    )
{
    _INTRINSIC_BEGIN;
    {
        _MEM_IMMEDIATE_RANGE_CHECK_SIGNED(data, count, 4);
        _MEM_IMPLEMENT_IMMEDIATE_64BIT_COMMAND_PTR40(add64_imm);
    }
    _INTRINSIC_END;
}


/* Increment 64 integer */
__intrinsic
void cmd_mem_incr64_ptr32(
    volatile void __addr32 __mem *address           /* address to increment                 */
    )
{
    _INTRINSIC_BEGIN;
    {
        __asm
        {
            mem[incr64, --, address, 0]
        }
    }
    _INTRINSIC_END;
}


/* Increment 64 integer */
__intrinsic
void cmd_mem_incr64_ptr40(
    volatile void __addr40 __mem *address     /* address to increment                 */
    )
{
    _INTRINSIC_BEGIN;
    {
        uint32_t hi_addr, low_addr;
        _INTRINSIC_CONVERT_HI_LO_ADDRESS(address);
        __asm
        {
            mem[incr64, --, hi_addr, <<8, low_addr]
        }
    }
    _INTRINSIC_END;
}

__intrinsic
void cmd_mem_test_and_add64_imm_ptr32(
    int data,                               /* immediate data                       */
    __xread void *rd_back,                  /* holds read back data                 */
    volatile void __addr32 __mem *address,  /* address to operate on                */
    uint32_t count,                         /* number of 32-bit words to write      */
    sync_t sync,                            /* must be sig_done                     */
    SIGNAL *sig_ptr                         /* signal to raise upon completion      */
    )
{
    _INTRINSIC_BEGIN;

    _MEM_TEST_IMMEDIATE_RANGE_CHECK_SIGNED(data, count, 4);

    CT_ASSERT(__is_read_reg(rd_back));
    CT_ASSERT(__is_ct_const(sync));
    CT_ASSERT(sync == sig_done);
    {
        generic_ind_t ind;

        // override length and bytemask
        _INTRINSIC_OVERRIDE_LENGTH_AND_BYTEMASK_IN_ALU(ind, (((count - 1) & 3) | (1 << 2)), (data & 0xff));

        __asm
        {
            alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
            mem[test_add64_imm, *rd_back, address, 0], sig_done[*sig_ptr], num_sigs[1], indirect_ref
        }
    }

    _INTRINSIC_END;
}

__intrinsic
void cmd_mem_test_and_add64_imm_ptr40(
    int data,                               /* immediate data                       */
    __xread void *rd_back,                  /* holds read back data                 */
    volatile void __addr40 __mem *address,  /* address to operate on                */
    uint32_t count,                         /* number of 32-bit words to write      */
    sync_t sync,                            /* must be sig_done                     */
    SIGNAL *sig_ptr                         /* signal to raise upon completion      */
    )
{
    _INTRINSIC_BEGIN;
    _MEM_TEST_IMMEDIATE_RANGE_CHECK_SIGNED(data, count, 4);
    {
        generic_ind_t ind;
        uint32_t hi_addr, low_addr;
        _INTRINSIC_CONVERT_HI_LO_ADDRESS(address);
        _INTRINSIC_OVERRIDE_LENGTH_AND_BYTEMASK_IN_ALU(ind, ((count - 1) & 3) | (1 << 2), (data & 0xff));
        __asm
        {
            alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
            mem[test_add64_imm, *rd_back, hi_addr, <<8, low_addr], sig_done[*sig_ptr], num_sigs[1], indirect_ref
        }
    }

    _INTRINSIC_END;
}


/* Add with saturation operations */
__intrinsic
void cmd_mem_add32_sat_ptr32(
    __xwrite void *data,                    /* data to write                        */
    volatile void __addr32 __mem *address,  /* address to operate on                */
    uint32_t count,                         /* number of 32-bit words to write      */
    sync_t sync,                            /* sig_done or ctx_swap                 */
    SIGNAL *sig_ptr                         /* signal to raise upon completion      */
    )
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_write_reg(data));
        _MEM_IMPLEMENT_COMMAND(addsat, cmd_mem_add32_sat_ptr32, 1, 4);
    }
    _INTRINSIC_END;
}

__intrinsic
void cmd_mem_add32_sat_ptr40(
    __xwrite void *data,                    /* data to write                        */
    volatile void __addr40 __mem *address,  /* address to operate on                */
    uint32_t count,                         /* number of 32-bit words to write      */
    sync_t sync,                            /* sig_done or ctx_swap                 */
    SIGNAL *sig_ptr                         /* signal to raise upon completion      */
    )
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_write_reg(data));
        _MEM_IMPLEMENT_COMMAND_PTR40(addsat, cmd_mem_add32_sat_ptr40, 1, 4);
    }
    _INTRINSIC_END;
}


__intrinsic
void cmd_mem_add32_sat_ind_ptr32(
    __xwrite void *data,                    /* data to write                        */
    volatile void __addr32 __mem *address,  /* address to operate on                */
    uint32_t max_nn,                        /* max. number of 32-bit words to write */
    generic_ind_t ind,                      /* indirect word                        */
    sync_t sync,                            /* sig_done or ctx_swap                 */
    SIGNAL *sig_ptr                         /* signal to raise upon completion      */
    )
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_write_reg(data));
        _MEM_IMPLEMENT_INDIRECT_COMMAND(addsat, 1, 4);
    }
    _INTRINSIC_END;
}


__intrinsic
void cmd_mem_add32_sat_ind_ptr40(
    __xwrite void *data,                    /* data to write                        */
    volatile void __addr40 __mem *address,  /* address to operate on                */
    uint32_t max_nn,                        /* max. number of 32-bit words to write */
    generic_ind_t ind,                      /* indirect word                        */
    sync_t sync,                            /* sig_done or ctx_swap                 */
    SIGNAL *sig_ptr                         /* signal to raise upon completion      */
)
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_write_reg(data));
        __asm local_csr_wr[CMD_INDIRECT_REF_0, CSR_INDIRECT_TYPE(ind)]
        _MEM_IMPLEMENT_INDIRECT_COMMAND_PTR40(addsat, 1, 4);
    }
    _INTRINSIC_END;
}


__intrinsic
void cmd_mem_add64_sat_ptr32(
    __xwrite void *data,                    /* data to write                        */
    volatile void __addr32 __mem *address,  /* address to operate on                */
    uint32_t count,                         /* number of 32-bit words to write      */
    sync_t sync,                            /* sig_done or ctx_swap                 */
    SIGNAL *sig_ptr                         /* signal to raise upon completion      */
    )
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_write_reg(data));
        _MEM_IMPLEMENT_ARITHMETIC64_COMMAND(addsat64, cmd_mem_add64_sat_ptr40, 1, 4);
    }
    _INTRINSIC_END;
}


__intrinsic
void cmd_mem_add64_sat_ptr40(
    __xwrite void *data,                    /* data to write                        */
    volatile void __addr40 __mem *address,  /* address to operate on                */
    uint32_t count,                         /* number of 32-bit words to write      */
    sync_t sync,                            /* sig_done or ctx_swap                 */
    SIGNAL *sig_ptr                         /* signal to raise upon completion      */
    )
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_write_reg(data));
        _MEM_IMPLEMENT_ARITHMETIC64_COMMAND_PTR40(addsat64, cmd_mem_add64_sat_ptr40, 1, 4);
    }
    _INTRINSIC_END;
}


__intrinsic
void cmd_mem_add64_sat_ind_ptr32(
    __xwrite void *data,                    /* data to write                        */
    volatile void __addr32 __mem *address,  /* address to operate on                */
    uint32_t max_nn,                        /* max. number of 32-bit words to write */
    generic_ind_t ind,                      /* indirect word                        */
    sync_t sync,                            /* sig_done or ctx_swap                 */
    SIGNAL *sig_ptr                         /* signal to raise upon completion      */
    )
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_write_reg(data));
        _MEM_IMPLEMENT_INDIRECT_COMMAND(addsat64, 1, 4);
    }
    _INTRINSIC_END;
}


__intrinsic
void cmd_mem_add64_sat_ind_ptr40(
    __xwrite void *data,                    /* data to write                        */
    volatile void __addr40 __mem *address,  /* address to operate on                */
    uint32_t max_nn,                        /* max. number of 32-bit words to write */
    generic_ind_t ind,                      /* indirect word                        */
    sync_t sync,                            /* sig_done or ctx_swap                 */
    SIGNAL *sig_ptr                         /* signal to raise upon completion      */
    )
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_write_reg(data));
        __asm local_csr_wr[CMD_INDIRECT_REF_0, CSR_INDIRECT_TYPE(ind)]
        _MEM_IMPLEMENT_INDIRECT_COMMAND_PTR40(addsat64, 1, 4);
    }
    _INTRINSIC_END;
}


/* Test and add operations */
__intrinsic
void cmd_mem_test_and_add32_sat_ptr32(
    __xread uint32_t *val,                  /* returned pre-modified value          */
    __xwrite void *data,                    /* data to add                          */
    volatile void __addr32 __mem *address,  /* address to operate on                */
    uint32_t count,                         /* number of 32-bit words to write      */
    sync_t sync,                            /* must be sig_done                     */
    SIGNAL_PAIR *sig_pair_ptr               /* signal pair to raise upon completion */
    )
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_read_reg(val));
        CT_ASSERT(__is_write_reg(data));
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done);
        __associate_read_write_reg_pair_no_spill(val, data);

        if (__is_ct_const(count))
        {
            CT_ASSERT(count >= 1 && count <= 4);
            __asm mem[test_addsat, *val, address, 0, __ct_const_val(count)], sig_done[*__ct_sig_pair(sig_pair_ptr)]
        }
        else
        {
            CT_QPERFINFO_INDIRECT_REF(cmd_mem_test_and_add32_sat_ptr32);
            {
                _INTRINSIC_IND_ALU_COUNT_MIN_MAX(count, 1, 4);
                __asm mem[test_addsat, *val, address, 0, __ct_const_val(count)], sig_done[*__ct_sig_pair(sig_pair_ptr)], indirect_ref
            }
        }
    }
    _INTRINSIC_END;
}

__intrinsic
void cmd_mem_test_and_add32_sat_ptr40(
    __xread uint32_t *val,                  /* returned pre-modified value          */
    __xwrite void *data,                    /* data to add                          */
    volatile void __addr40 __mem *address,  /* address to operate on                    */
    uint32_t count,                         /* number of 32-bit words to write      */
    sync_t sync,                            /* must be sig_done                     */
    SIGNAL_PAIR *sig_pair_ptr               /* signal pair to raise upon completion */
    )
{
    _INTRINSIC_BEGIN;
    {
        uint32_t hi_addr, low_addr;
        _INTRINSIC_CONVERT_HI_LO_ADDRESS(address);
        CT_ASSERT(__is_read_reg(val));
        CT_ASSERT(__is_write_reg(data));
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done);
        __associate_read_write_reg_pair_no_spill(val, data);

        if (__is_ct_const(count))
        {
            CT_ASSERT(count >= 1 && count <= 4);
            __asm mem[test_addsat, *val, hi_addr, <<8, low_addr, __ct_const_val(count)], sig_done[*__ct_sig_pair(sig_pair_ptr)]
        }
        else
        {
            CT_QPERFINFO_INDIRECT_REF(cmd_mem_test_and_add32_sat_ptr40);
            {
                _INTRINSIC_IND_ALU_COUNT_MIN_MAX(count, 1, 4);
                __asm mem[test_addsat, *val, hi_addr, <<8, low_addr, __ct_const_val(count)], sig_done[*__ct_sig_pair(sig_pair_ptr)], indirect_ref
            }
        }
    }
    _INTRINSIC_END;
}


__intrinsic
void cmd_mem_test_and_add32_sat_ind_ptr32(
    __xread uint32_t *val,                  /* returned pre-modified value          */
    __xwrite void *data,                    /* data to add                          */
    volatile void __addr32 __mem *address,  /* address to operate on                */
    uint32_t max_nn,                        /* max. number of 32-bit words to write */
    generic_ind_t ind,                      /* indirect word                        */
    sync_t sync,                            /* must be sig_done                     */
    SIGNAL_PAIR *sig_pair_ptr               /* signal pair to raise upon completion */
    )
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_read_reg(val));
        CT_ASSERT(__is_write_reg(data));
        CT_ASSERT(__is_ct_const(max_nn));
        CT_ASSERT(max_nn >= 1 && max_nn <= 4);
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done);
        __associate_read_write_reg_pair_no_spill(val, data);
        __asm local_csr_wr[CMD_INDIRECT_REF_0, CSR_INDIRECT_TYPE(ind)]
        __asm
        {
            alu[--, --, B, ind]
            mem[test_addsat, *val, address, 0, __ct_const_val(max_nn)], sig_done[*__ct_sig_pair(sig_pair_ptr)], indirect_ref
        }
    }
    _INTRINSIC_END;
}

__intrinsic
void cmd_mem_test_and_add32_sat_ind_ptr40(
    __xread uint32_t *val,                  /* returned pre-modified value          */
    __xwrite void *data,                    /* data to add                          */
    volatile void __addr40 __mem *address,  /* address to operate on                */
    uint32_t max_nn,                        /* max. number of 32-bit words to write */
    generic_ind_t ind,                      /* indirect word                        */
    sync_t sync,                            /* must be sig_done                     */
    SIGNAL_PAIR *sig_pair_ptr               /* signal pair to raise upon completion */
    )
{
    _INTRINSIC_BEGIN;
    {
        uint32_t hi_addr, low_addr;
        _INTRINSIC_CONVERT_HI_LO_ADDRESS(address);
        CT_ASSERT(__is_read_reg(val));
        CT_ASSERT(__is_write_reg(data));
        CT_ASSERT(__is_ct_const(max_nn));
        CT_ASSERT(max_nn >= 1 && max_nn <= 4);
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done);
        __associate_read_write_reg_pair_no_spill(val, data);
        __asm local_csr_wr[CMD_INDIRECT_REF_0, CSR_INDIRECT_TYPE(ind)]
        __asm
        {
            alu[--, --, B, ind]
            mem[test_addsat, *val, hi_addr, <<8, low_addr, __ct_const_val(max_nn)], sig_done[*__ct_sig_pair(sig_pair_ptr)], indirect_ref
        }
    }
    _INTRINSIC_END;
}


__intrinsic
void cmd_mem_test_and_add64_sat_ptr32(
    __xread uint32_t *val,                  /* returned pre-modified value          */
    __xwrite void *data,                    /* data to add                          */
    volatile void __addr32 __mem *address,  /* address to operate on                */
    uint32_t count,                         /* number of 32-bit words to write      */
    sync_t sync,                            /* must be sig_done                     */
    SIGNAL_PAIR *sig_pair_ptr               /* signal pair to raise upon completion */
    )
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_read_reg(val));
        CT_ASSERT(__is_write_reg(data));
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done);
        __associate_read_write_reg_pair_no_spill(val, data);

        if (__is_ct_const(count))
        {
            CT_ASSERT(count >= 1 && count <= 4);
            __asm mem[test_addsat64, *val, address, 0, __ct_const_val(count)], sig_done[*__ct_sig_pair(sig_pair_ptr)]
        }
        else
        {
            CT_QPERFINFO_INDIRECT_REF(cmd_mem_test_and_add64_sat_ptr32);
            {
                _MEM_ARITHMETIC_64_IND_ALU_COUNT_MIN_MAX(count, 1, 4);
                __asm mem[test_addsat64, *val, address, 0, __ct_const_val(count)], sig_done[*__ct_sig_pair(sig_pair_ptr)], indirect_ref
            }
        }
    }
    _INTRINSIC_END;
}

__intrinsic
void cmd_mem_test_and_add64_sat_ptr40(
    __xread uint32_t *val,                  /* returned pre-modified value          */
    __xwrite void *data,                    /* data to add                          */
    volatile void __addr40 __mem *address,  /* address to operate on                */
    uint32_t count,                         /* number of 32-bit words to write      */
    sync_t sync,                            /* must be sig_done                     */
    SIGNAL_PAIR *sig_pair_ptr               /* signal pair to raise upon completion */
    )
{
    _INTRINSIC_BEGIN;
    {
        uint32_t hi_addr, low_addr;
        _INTRINSIC_CONVERT_HI_LO_ADDRESS(address);
        CT_ASSERT(__is_read_reg(val));
        CT_ASSERT(__is_write_reg(data));
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done);
        __associate_read_write_reg_pair_no_spill(val, data);

        if (__is_ct_const(count))
        {
            CT_ASSERT(count >= 1 && count <= 4);
            __asm mem[test_addsat64, *val, hi_addr, <<8, low_addr, __ct_const_val(count)], sig_done[*__ct_sig_pair(sig_pair_ptr)]
        }
        else
        {
            CT_QPERFINFO_INDIRECT_REF(cmd_mem_test_and_add64_sat_ptr40);
            {
                _MEM_ARITHMETIC_64_IND_ALU_COUNT_MIN_MAX(count, 1, 4);
                __asm mem[test_addsat64, *val, hi_addr, <<8, low_addr, __ct_const_val(count)], sig_done[*__ct_sig_pair(sig_pair_ptr)], indirect_ref
            }
        }
    }
    _INTRINSIC_END;
}


__intrinsic
void cmd_mem_test_and_add64_sat_ind_ptr32(
    __xread uint32_t *val,                  /* returned pre-modified value          */
    __xwrite void *data,                    /* data to add                          */
    volatile void __addr32 __mem *address,  /* address to operate on                */
    uint32_t max_nn,                        /* max. number of 32-bit words to write */
    generic_ind_t ind,                      /* indirect word                        */
    sync_t sync,                            /* must be sig_done                     */
    SIGNAL_PAIR *sig_pair_ptr               /* signal pair to raise upon completion */
    )
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_read_reg(val));
        CT_ASSERT(__is_write_reg(data));
        CT_ASSERT(__is_ct_const(max_nn));
        CT_ASSERT(max_nn >= 1 && max_nn <= 4);
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done);
        __associate_read_write_reg_pair_no_spill(val, data);
        __asm local_csr_wr[CMD_INDIRECT_REF_0, CSR_INDIRECT_TYPE(ind)]
        __asm
        {
            alu[--, --, B, ind]
            mem[test_addsat64, *val, address, 0, __ct_const_val(max_nn)], sig_done[*__ct_sig_pair(sig_pair_ptr)], indirect_ref
        }
    }
    _INTRINSIC_END;
}

__intrinsic
void cmd_mem_test_and_add64_sat_ind_ptr40(
    __xread uint32_t *val,                  /* returned pre-modified value          */
    __xwrite void *data,                    /* data to add                          */
    volatile void __addr40 __mem *address,  /* address to operate on                */
    uint32_t max_nn,                        /* max. number of 32-bit words to write */
    generic_ind_t ind,                      /* indirect word                        */
    sync_t sync,                            /* must be sig_done                     */
    SIGNAL_PAIR *sig_pair_ptr               /* signal pair to raise upon completion */
    )
{
    _INTRINSIC_BEGIN;
    {
        uint32_t hi_addr, low_addr;
        _INTRINSIC_CONVERT_HI_LO_ADDRESS(address);
        CT_ASSERT(__is_read_reg(val));
        CT_ASSERT(__is_write_reg(data));
        CT_ASSERT(__is_ct_const(max_nn));
        CT_ASSERT(max_nn >= 1 && max_nn <= 4);
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done);
        __associate_read_write_reg_pair_no_spill(val, data);
        __asm local_csr_wr[CMD_INDIRECT_REF_0, CSR_INDIRECT_TYPE(ind)]
        __asm
        {
            alu[--, --, B, ind]
            mem[test_addsat64, *val, hi_addr, <<8, low_addr, __ct_const_val(max_nn)], sig_done[*__ct_sig_pair(sig_pair_ptr)], indirect_ref
        }
    }
    _INTRINSIC_END;
}


/* Add immediate operations */
__intrinsic
void cmd_mem_add32_imm_sat_ptr32(
    int data,                               /* immediate data                       */
    volatile void __addr32 __mem *address,  /* address to operate on                */
    uint32_t count                          /* number of 32-bit words to write      */
    )
{
    _INTRINSIC_BEGIN;
    {
        _MEM_IMMEDIATE_RANGE_CHECK_SIGNED(data, count, 4);
        _MEM_IMPLEMENT_IMMEDIATE_COMMAND(addsat_imm);
    }
    _INTRINSIC_END;
}

/* Add immediate operations */
__intrinsic
void cmd_mem_add32_imm_sat_ptr40(
    int data,                               /* immediate data                       */
    volatile void __addr40 __mem *address,  /* address to operate on                */
    uint32_t count                          /* number of 32-bit words to write      */
    )
{
    _INTRINSIC_BEGIN;
    {
        _MEM_IMMEDIATE_RANGE_CHECK_SIGNED(data, count, 4);
        _MEM_IMPLEMENT_IMMEDIATE_COMMAND_PTR40(addsat_imm);
    }


#ifndef __NFP_THIRD_PARTY_ADDRESSING_40_BIT
_INTRINSIC_NEED_40_BIT_THIRD_PARTY();
#endif

    _INTRINSIC_END;
}

__intrinsic
void cmd_mem_test_and_add32_imm_sat_ptr32(
    int data,                               /* immediate data                       */
    __xread void *rd_back,                  /* holds read back data                 */
    volatile void __addr32 __mem *address,  /* address to operate on                */
    uint32_t count,                         /* number of 32-bit words to write      */
    sync_t sync,                            /* must be sig_done                     */
    SIGNAL *sig_ptr                         /* signal to raise upon completion      */
    )
{
    _INTRINSIC_BEGIN;
    _MEM_TEST_IMMEDIATE_RANGE_CHECK_SIGNED(data, count, 4);
    CT_ASSERT(__is_read_reg(rd_back));
    CT_ASSERT(__is_ct_const(sync));
    CT_ASSERT(sync == sig_done);
    {
        generic_ind_t ind;

        // override length and bytemask
        _INTRINSIC_OVERRIDE_LENGTH_AND_BYTEMASK_IN_ALU(ind, ((count - 1) & 3), (data & 0xff));
        __asm
        {
            alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
            mem[test_addsat_imm, *rd_back, address, 0], sig_done[*sig_ptr], num_sigs[1], indirect_ref
        }
    }

    _INTRINSIC_END;
}

__intrinsic
void cmd_mem_test_and_add32_imm_sat_ptr40(
    int data,                               /* immediate data                       */
    __xread void *rd_back,                  /* holds read back data                 */
    volatile void __addr40 __mem *address,  /* address to operate on                */
    uint32_t count,                         /* number of 32-bit words to write      */
    sync_t sync,                            /* must be sig_done                     */
    SIGNAL *sig_ptr                         /* signal to raise upon completion      */
    )
{
    _INTRINSIC_BEGIN;
    _MEM_TEST_IMMEDIATE_RANGE_CHECK_SIGNED(data, count, 4);

    CT_ASSERT(__is_read_reg(rd_back));
    CT_ASSERT(__is_ct_const(sync));
    CT_ASSERT(sync == sig_done);
    {
        generic_ind_t ind;
        uint32_t hi_addr, low_addr;
        _INTRINSIC_CONVERT_HI_LO_ADDRESS(address);

        // override length and bytemask
        _INTRINSIC_OVERRIDE_LENGTH_AND_BYTEMASK_IN_ALU(ind, ((count - 1) & 3), (data & 0xff));
        __asm
        {
            alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
            mem[test_addsat_imm, *rd_back, hi_addr, <<8, low_addr], sig_done[*sig_ptr], num_sigs[1], indirect_ref
        }
    }

    _INTRINSIC_END;
}

__intrinsic
void cmd_mem_add64_imm_sat_ptr32(
    int data,                               /* immediate data                       */
    volatile void __addr32 __mem *address,  /* address to operate on                */
    uint32_t count                          /* number of 32-bit words to write      */
    )
{
    _INTRINSIC_BEGIN;
    {
        _MEM_IMMEDIATE_RANGE_CHECK_SIGNED(data, count, 4);
        _MEM_IMPLEMENT_IMMEDIATE_64BIT_COMMAND(addsat64_imm);
    }
    _INTRINSIC_END;
}

__intrinsic
void cmd_mem_add64_imm_sat_ptr40(
    int data,                               /* immediate data                       */
    volatile void __addr40 __mem *address,  /* address to operate on                */
    uint32_t count                          /* number of 32-bit words to write      */
    )
{
    _INTRINSIC_BEGIN;
    {
        _MEM_IMMEDIATE_RANGE_CHECK_SIGNED(data, count, 4);
        _MEM_IMPLEMENT_IMMEDIATE_64BIT_COMMAND_PTR40(addsat64_imm);
    }


    _INTRINSIC_END;
}

__intrinsic
void cmd_mem_test_and_add64_imm_sat_ptr32(
    int data,                               /* immediate data                       */
    __xread void *rd_back,                  /* holds read back data                 */
    volatile void __addr32 __mem *address,  /* address to operate on                */
    uint32_t count,                         /* number of 32-bit words to write      */
    sync_t sync,                            /* must be sig_done                     */
    SIGNAL *sig_ptr                         /* signal to raise upon completion      */
    )
{
    _INTRINSIC_BEGIN;

    _MEM_TEST_IMMEDIATE_RANGE_CHECK_SIGNED(data, count, 4);

    CT_ASSERT(__is_read_reg(rd_back));
    CT_ASSERT(__is_ct_const(sync));
    CT_ASSERT(sync == sig_done);

    {
        generic_ind_t ind;

        // override length and bytemask
        _INTRINSIC_OVERRIDE_LENGTH_AND_BYTEMASK_IN_ALU(ind, ((count - 1) & 3) | (1 << 2), (data & 0xff));
        __asm
        {
            alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
            mem[test_addsat64_imm, *rd_back, address, 0], sig_done[*sig_ptr], num_sigs[1], indirect_ref
        }
    }

    _INTRINSIC_END;
}

__intrinsic
void cmd_mem_test_and_add64_imm_sat_ptr40(
    int data,                               /* immediate data                       */
    __xread void *rd_back,                  /* holds read back data                 */
    volatile void __addr40 __mem *address,  /* address to operate on                */
    uint32_t count,                         /* number of 32-bit words to write      */
    sync_t sync,                            /* must be sig_done                     */
    SIGNAL *sig_ptr                         /* signal to raise upon completion      */
    )
{
    _INTRINSIC_BEGIN;

    _MEM_TEST_IMMEDIATE_RANGE_CHECK_SIGNED(data, count, 4);

    CT_ASSERT(__is_read_reg(rd_back));
    CT_ASSERT(__is_ct_const(sync));
    CT_ASSERT(sync == sig_done);

    {
        generic_ind_t ind;
        uint32_t hi_addr, low_addr;
        _INTRINSIC_CONVERT_HI_LO_ADDRESS(address);

        // override length and bytemask
        _INTRINSIC_OVERRIDE_LENGTH_AND_BYTEMASK_IN_ALU(ind, ((count - 1) & 3) | (1 << 2), data & 0xff);
        __asm
        {
            alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
            mem[test_addsat64_imm, *rd_back, hi_addr,<<8, low_addr], sig_done[*sig_ptr], num_sigs[1], indirect_ref
        }
    }

    _INTRINSIC_END;
}

/* Subtract operations */
__intrinsic
void cmd_mem_sub32_ptr32(
    __xwrite void *data,                    /* data to write                        */
    volatile void __addr32 __mem *address,  /* address to operate on                */
    uint32_t count,                         /* number of 32-bit words to write      */
    sync_t sync,                            /* sig_done or ctx_swap                 */
    SIGNAL *sig_ptr                         /* signal to raise upon completion      */
    )
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_write_reg(data));
        _MEM_IMPLEMENT_COMMAND(sub, cmd_mem_sub32_ptr32, 1, 4);
    }
    _INTRINSIC_END;
}

__intrinsic
void cmd_mem_sub32_ptr40(
    __xwrite void *data,                    /* data to write                        */
    volatile void __addr40 __mem *address,  /* address to operate on                */
    uint32_t count,                         /* number of 32-bit words to write      */
    sync_t sync,                            /* sig_done or ctx_swap                 */
    SIGNAL *sig_ptr                         /* signal to raise upon completion      */
    )
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_write_reg(data));
        _MEM_IMPLEMENT_COMMAND_PTR40(sub, cmd_mem_sub32_ptr40, 1, 4);
    }
    _INTRINSIC_END;
}

__intrinsic
void cmd_mem_sub32_ind_ptr32(
    __xwrite void *data,                    /* data to write                        */
    volatile void __addr32 __mem *address,  /* address to operate on                */
    uint32_t max_nn,                        /* max. number of 32-bit words to write */
    generic_ind_t ind,                      /* indirect word                        */
    sync_t sync,                            /* sig_done or ctx_swap                 */
    SIGNAL *sig_ptr                         /* signal to raise upon completion      */
    )
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_write_reg(data));
        _MEM_IMPLEMENT_INDIRECT_COMMAND(sub, 1, 4);
    }
    _INTRINSIC_END;
}


__intrinsic
void cmd_mem_sub32_ind_ptr40(
    __xwrite void *data,                    /* data to write                        */
    volatile void __addr40 __mem *address,  /* address to operate on                */
    uint32_t max_nn,                        /* max. number of 32-bit words to write */
    generic_ind_t ind,                      /* indirect word                        */
    sync_t sync,                            /* sig_done or ctx_swap                 */
    SIGNAL *sig_ptr                         /* signal to raise upon completion      */
    )
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_write_reg(data));
        __asm local_csr_wr[CMD_INDIRECT_REF_0, CSR_INDIRECT_TYPE(ind)]
        _MEM_IMPLEMENT_INDIRECT_COMMAND_PTR40(sub, 1, 4);
    }
    _INTRINSIC_END;
}


__intrinsic
void cmd_mem_sub64_ptr32(
    __xwrite void *data,                    /* data to write                        */
    volatile void __addr32 __mem *address,  /* address to operate on                */
    uint32_t count,                         /* number of 32-bit words to write      */
    sync_t sync,                            /* sig_done or ctx_swap                 */
    SIGNAL *sig_ptr                         /* signal to raise upon completion      */
    )
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_write_reg(data));
        _MEM_IMPLEMENT_ARITHMETIC64_COMMAND(sub64, cmd_mem_sub64_ptr32, 1, 4);
    }
    _INTRINSIC_END;
}

__intrinsic
void cmd_mem_sub64_ptr40(
    __xwrite void *data,                    /* data to write                        */
    volatile void __addr40 __mem *address,  /* address to operate on                */
    uint32_t count,                         /* number of 32-bit words to write      */
    sync_t sync,                            /* sig_done or ctx_swap                 */
    SIGNAL *sig_ptr                         /* signal to raise upon completion      */
    )
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_write_reg(data));
        _MEM_IMPLEMENT_ARITHMETIC64_COMMAND_PTR40(sub64, cmd_mem_sub64_ptr40, 1, 4);
    }
    _INTRINSIC_END;
}


__intrinsic
void cmd_mem_sub64_ind_ptr32(
    __xwrite void *data,                    /* data to write                        */
    volatile void __addr32 __mem *address,  /* address to operate on                */
    uint32_t max_nn,                        /* max. number of 32-bit words to write */
    generic_ind_t ind,                      /* indirect word                        */
    sync_t sync,                            /* sig_done or ctx_swap                 */
    SIGNAL *sig_ptr                         /* signal to raise upon completion      */
    )
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_write_reg(data));
        _MEM_IMPLEMENT_INDIRECT_COMMAND(sub64, 1, 4);
    }
    _INTRINSIC_END;
}


__intrinsic
void cmd_mem_sub64_ind_ptr40(
    __xwrite void *data,                    /* data to write                        */
    volatile void __addr40 __mem *address,  /* address to operate on                */
    uint32_t max_nn,                        /* max. number of 32-bit words to write */
    generic_ind_t ind,                      /* indirect word                        */
    sync_t sync,                            /* sig_done or ctx_swap                 */
    SIGNAL *sig_ptr                         /* signal to raise upon completion      */
    )
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_write_reg(data));

        __asm local_csr_wr[CMD_INDIRECT_REF_0, CSR_INDIRECT_TYPE(ind)]

        _MEM_IMPLEMENT_INDIRECT_COMMAND_PTR40(sub64, 1, 4);
    }
    _INTRINSIC_END;
}


/* Test and sub operations */
__intrinsic
void cmd_mem_test_and_sub32_ptr32(
    __xread uint32_t *val,                  /* returned pre-modified value          */
    __xwrite void *data,                    /* data to add                          */
    volatile void __addr32 __mem *address,  /* address to operate on                */
    uint32_t count,                         /* number of 32-bit words to write      */
    sync_t sync,                            /* must be sig_done                     */
    SIGNAL_PAIR *sig_pair_ptr               /* signal pair to raise upon completion */
    )
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_read_reg(val));
        CT_ASSERT(__is_write_reg(data));
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done);
        __associate_read_write_reg_pair_no_spill(val, data);

        if (__is_ct_const(count))
        {
            CT_ASSERT(count >= 1 && count <= 4);
            __asm mem[test_sub, *val, address, 0, __ct_const_val(count)], sig_done[*__ct_sig_pair(sig_pair_ptr)]
        }
        else
        {
            CT_QPERFINFO_INDIRECT_REF(cmd_mem_test_and_sub32_ptr32);
            {
                _INTRINSIC_IND_ALU_COUNT_MIN_MAX(count, 1, 4);
                __asm mem[test_sub, *val, address, 0, __ct_const_val(count)], sig_done[*__ct_sig_pair(sig_pair_ptr)], indirect_ref
            }
        }
    }
    _INTRINSIC_END;
}

__intrinsic
void cmd_mem_test_and_sub32_ptr40(
    __xread uint32_t *val,                  /* returned pre-modified value          */
    __xwrite void *data,                    /* data to add                          */
    volatile void __addr40 __mem *address,  /* address to operate on                */
    uint32_t count,                         /* number of 32-bit words to write      */
    sync_t sync,                            /* must be sig_done                     */
    SIGNAL_PAIR *sig_pair_ptr               /* signal pair to raise upon completion */
    )
{
    _INTRINSIC_BEGIN;
    {
        uint32_t hi_addr, low_addr;
        _INTRINSIC_CONVERT_HI_LO_ADDRESS(address);
        CT_ASSERT(__is_read_reg(val));
        CT_ASSERT(__is_write_reg(data));
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done);
        __associate_read_write_reg_pair_no_spill(val, data);

        if (__is_ct_const(count))
        {
            CT_ASSERT(count >= 1 && count <= 4);
            __asm mem[test_sub, *val, hi_addr, <<8, low_addr, __ct_const_val(count)], sig_done[*__ct_sig_pair(sig_pair_ptr)]
        }
        else
        {
            CT_QPERFINFO_INDIRECT_REF(cmd_mem_test_and_sub32_ptr40);
            {
                _INTRINSIC_IND_ALU_COUNT_MIN_MAX(count, 1, 4);
                __asm mem[test_sub, *val, hi_addr, <<8, low_addr, __ct_const_val(count)], sig_done[*__ct_sig_pair(sig_pair_ptr)], indirect_ref
            }
        }
    }
    _INTRINSIC_END;
}


__intrinsic
void cmd_mem_test_and_sub32_ind_ptr32(
    __xread uint32_t *val,                  /* returned pre-modified value          */
    __xwrite void *data,                    /* data to subtract                     */
    volatile void __addr32 __mem *address,  /* address to operate on                */
    uint32_t max_nn,                        /* max. number of 32-bit words to write */
    generic_ind_t ind,                      /* indirect word                        */
    sync_t sync,                            /* must be sig_done                     */
    SIGNAL_PAIR *sig_pair_ptr               /* signal pair to raise upon completion */
    )
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_read_reg(val));
        CT_ASSERT(__is_write_reg(data));
        CT_ASSERT(__is_ct_const(max_nn));
        CT_ASSERT(max_nn >= 1 && max_nn <= 4);
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done);
        __associate_read_write_reg_pair_no_spill(val, data);


        __asm local_csr_wr[CMD_INDIRECT_REF_0, CSR_INDIRECT_TYPE(ind)]

        __asm
        {
            alu[--, --, B, ind]
            mem[test_sub, *val, address, 0, __ct_const_val(max_nn)], sig_done[*__ct_sig_pair(sig_pair_ptr)], indirect_ref
        }
    }
    _INTRINSIC_END;
}

__intrinsic
void cmd_mem_test_and_sub32_ind_ptr40(
    __xread uint32_t *val,                  /* returned pre-modified value          */
    __xwrite void *data,                    /* data to subtract                     */
    volatile void __addr40 __mem *address,  /* address to operate on                */
    uint32_t max_nn,                        /* max. number of 32-bit words to write */
    generic_ind_t ind,                      /* indirect word                        */
    sync_t sync,                            /* must be sig_done                     */
    SIGNAL_PAIR *sig_pair_ptr               /* signal pair to raise upon completion */
    )
{
    _INTRINSIC_BEGIN;
    {
        uint32_t hi_addr, low_addr;
        _INTRINSIC_CONVERT_HI_LO_ADDRESS(address);
        CT_ASSERT(__is_read_reg(val));
        CT_ASSERT(__is_write_reg(data));
        CT_ASSERT(__is_ct_const(max_nn));
        CT_ASSERT(max_nn >= 1 && max_nn <= 4);
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done);
        __associate_read_write_reg_pair_no_spill(val, data);
        __asm local_csr_wr[CMD_INDIRECT_REF_0, CSR_INDIRECT_TYPE(ind)]
        __asm
        {
            alu[--, --, B, ind]
            mem[test_sub, *val, hi_addr, <<8, low_addr, __ct_const_val(max_nn)], sig_done[*__ct_sig_pair(sig_pair_ptr)], indirect_ref
        }
    }
    _INTRINSIC_END;
}


__intrinsic
void cmd_mem_test_and_sub64_ptr32(
    __xread uint32_t *val,                  /* returned pre-modified value          */
    __xwrite void *data,                    /* data to subtract                     */
    volatile void __addr32 __mem *address,  /* address to operate on                */
    uint32_t count,                         /* number of 32-bit words to write      */
    sync_t sync,                            /* must be sig_done                     */
    SIGNAL_PAIR *sig_pair_ptr               /* signal pair to raise upon completion */
    )
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_read_reg(val));
        CT_ASSERT(__is_write_reg(data));
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done);
        __associate_read_write_reg_pair_no_spill(val, data);

        if (__is_ct_const(count))
        {
            CT_ASSERT(count >= 1 && count <= 4);
            __asm mem[test_sub64, *val, address, 0, __ct_const_val(count)], sig_done[*__ct_sig_pair(sig_pair_ptr)]
        }
        else
        {
            CT_QPERFINFO_INDIRECT_REF(cmd_mem_test_and_sub64_ptr32);
            {
                _MEM_ARITHMETIC_64_IND_ALU_COUNT_MIN_MAX(count, 1, 4);
                __asm mem[test_sub64, *val, address, 0, __ct_const_val(count)], sig_done[*__ct_sig_pair(sig_pair_ptr)], indirect_ref
            }
        }
    }
    _INTRINSIC_END;
}

__intrinsic
void cmd_mem_test_and_sub64_ptr40(
    __xread uint32_t *val,                  /* returned pre-modified value          */
    __xwrite void *data,                    /* data to subtract                     */
    volatile void __addr40 __mem *address,  /* address to operate on                */
    uint32_t count,                         /* number of 32-bit words to write      */
    sync_t sync,                            /* must be sig_done                     */
    SIGNAL_PAIR *sig_pair_ptr               /* signal pair to raise upon completion */
    )
{
    _INTRINSIC_BEGIN;
    {
        uint32_t hi_addr, low_addr;
        _INTRINSIC_CONVERT_HI_LO_ADDRESS(address);
        CT_ASSERT(__is_read_reg(val));
        CT_ASSERT(__is_write_reg(data));
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done);
        __associate_read_write_reg_pair_no_spill(val, data);

        if (__is_ct_const(count))
        {
            CT_ASSERT(count >= 1 && count <= 4);
            __asm mem[test_sub64, *val, hi_addr, <<8, low_addr, __ct_const_val(count)], sig_done[*__ct_sig_pair(sig_pair_ptr)]
        }
        else
        {
            CT_QPERFINFO_INDIRECT_REF(cmd_mem_test_and_sub64_ptr40);
            {
                _MEM_ARITHMETIC_64_IND_ALU_COUNT_MIN_MAX(count, 1, 4);
                __asm mem[test_sub64, *val, hi_addr, <<8, low_addr, __ct_const_val(count)], sig_done[*__ct_sig_pair(sig_pair_ptr)], indirect_ref
            }
        }
    }
    _INTRINSIC_END;
}


__intrinsic
void cmd_mem_test_and_sub64_ind_ptr32(
    __xread uint32_t *val,                  /* returned pre-modified value          */
    __xwrite void *data,                    /* data to subtract                     */
    volatile void __addr32 __mem *address,  /* address to operate on                */
    uint32_t max_nn,                        /* max. number of 32-bit words to write */
    generic_ind_t ind,                      /* indirect word                        */
    sync_t sync,                            /* must be sig_done                     */
    SIGNAL_PAIR *sig_pair_ptr               /* signal pair to raise upon completion */
    )
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_read_reg(val));
        CT_ASSERT(__is_write_reg(data));
        CT_ASSERT(__is_ct_const(max_nn));
        CT_ASSERT(max_nn >= 1 && max_nn <= 4);
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done);
        __associate_read_write_reg_pair_no_spill(val, data);
        __asm local_csr_wr[CMD_INDIRECT_REF_0, CSR_INDIRECT_TYPE(ind)]
        __asm
        {
            alu[--, --, B, ind]
            mem[test_sub64, *val, address, 0, __ct_const_val(max_nn)], sig_done[*__ct_sig_pair(sig_pair_ptr)], indirect_ref
        }
    }
    _INTRINSIC_END;
}

__intrinsic
void cmd_mem_test_and_sub64_ind_ptr40(
    __xread uint32_t *val,                  /* returned pre-modified value          */
    __xwrite void *data,                    /* data to subtract                     */
    volatile void __addr40 __mem *address,  /* address to operate on                */
    uint32_t max_nn,                        /* max. number of 32-bit words to write */
    generic_ind_t ind,                      /* indirect word                        */
    sync_t sync,                            /* must be sig_done                     */
    SIGNAL_PAIR *sig_pair_ptr               /* signal pair to raise upon completion */
    )
{
    _INTRINSIC_BEGIN;
    {
        uint32_t hi_addr, low_addr;
        _INTRINSIC_CONVERT_HI_LO_ADDRESS(address);
        CT_ASSERT(__is_read_reg(val));
        CT_ASSERT(__is_write_reg(data));
        CT_ASSERT(__is_ct_const(max_nn));
        CT_ASSERT(max_nn >= 1 && max_nn <= 4);
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done);
        __associate_read_write_reg_pair_no_spill(val, data);
        __asm local_csr_wr[CMD_INDIRECT_REF_0, CSR_INDIRECT_TYPE(ind)]
        __asm
        {
            alu[--, --, B, ind]
            mem[test_sub64, *val, hi_addr, <<8, low_addr, __ct_const_val(max_nn)], sig_done[*__ct_sig_pair(sig_pair_ptr)], indirect_ref
        }
    }
    _INTRINSIC_END;
}


/* Subtract immediate operations */
__intrinsic
void cmd_mem_sub32_imm_ptr32(
    int data,                               /* immediate data                       */
    volatile void __addr32 __mem *address,  /* address to operate on                */
    uint32_t count                          /* number of 32-bit words to write      */
    )
{
    _INTRINSIC_BEGIN;
    {
        _MEM_IMMEDIATE_RANGE_CHECK_SIGNED(data, count, 4);
        _MEM_IMPLEMENT_IMMEDIATE_COMMAND(sub_imm);
    }
    _INTRINSIC_END;
}


__intrinsic
void cmd_mem_sub32_imm_ptr40(
    int data,                               /* immediate data                       */
    volatile void __addr40 __mem *address,  /* address to operate on                */
    uint32_t count                          /* number of 32-bit words to write      */
    )
{
    _INTRINSIC_BEGIN;
    {
        _MEM_IMMEDIATE_RANGE_CHECK_SIGNED(data, count, 4);
        _MEM_IMPLEMENT_IMMEDIATE_COMMAND_PTR40(sub_imm);
    }
    _INTRINSIC_END;
}


/* Decrement 32 integer */
__intrinsic
void cmd_mem_decr32_ptr32(
    volatile void __addr32 __mem *address           /* address to decrement                 */
    )
{
    _INTRINSIC_BEGIN;
    {
        __asm
        {
            mem[decr, --, address, 0]
        }
    }
    _INTRINSIC_END;
}



__intrinsic
void cmd_mem_decr32_ptr40(
    volatile void __addr40 __mem *address     /* address to decrement                 */
)
{
    _INTRINSIC_BEGIN;
    {
        uint32_t hi_addr, low_addr;
        _INTRINSIC_CONVERT_HI_LO_ADDRESS(address);
        __asm
        {
            mem[decr, --, hi_addr, <<8, low_addr]
        }
    }
    _INTRINSIC_END;
}


__intrinsic
void cmd_mem_test_and_sub32_imm_ptr32(
    int data,                               /* immediate data                       */
    __xread void *rd_back,                  /* returned pre-modified value          */
    volatile void __addr32 __mem *address,  /* address to operate on                */
    uint32_t count,                         /* number of 32-bit words to write      */
    sync_t sync,                            /* must be sig_done                     */
    SIGNAL *sig_ptr                         /* signal to raise upon completion      */
    )
{
    _INTRINSIC_BEGIN;

    _MEM_TEST_IMMEDIATE_RANGE_CHECK_SIGNED(data, count, 4);

    CT_ASSERT(__is_read_reg(rd_back));
    CT_ASSERT(__is_ct_const(sync));
    CT_ASSERT(sync == sig_done);

    {
        generic_ind_t ind;
        // override length and bytemask
        _INTRINSIC_OVERRIDE_LENGTH_AND_BYTEMASK_IN_ALU(ind, ((count - 1) & 3), (data & 0xff));
        __asm
        {
            alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
            mem[test_sub_imm, *rd_back, address, 0], sig_done[*sig_ptr], num_sigs[1], indirect_ref
        }
    }

    _INTRINSIC_END;
}

__intrinsic
void cmd_mem_test_and_sub32_imm_ptr40(
    int data,                               /* immediate data                       */
    __xread void *rd_back,                  /* returned pre-modified value          */
    volatile void __addr40 __mem *address,  /* address to operate on                */
    uint32_t count,                         /* number of 32-bit words to write      */
    sync_t sync,                            /* must be sig_done                     */
    SIGNAL *sig_ptr                         /* signal to raise upon completion      */
    )
{
    _INTRINSIC_BEGIN;

    _MEM_TEST_IMMEDIATE_RANGE_CHECK_SIGNED(data, count, 4);

    {
        generic_ind_t ind;
        uint32_t hi_addr, low_addr;
        _INTRINSIC_CONVERT_HI_LO_ADDRESS(address);

        // override length and bytemask
        _INTRINSIC_OVERRIDE_LENGTH_AND_BYTEMASK_IN_ALU(ind, ((count - 1) & 3), (data & 0xff));
        __asm
        {
            alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
            mem[test_sub_imm, *rd_back, hi_addr, <<8, low_addr], sig_done[*sig_ptr], num_sigs[1], indirect_ref
        }
    }

    _INTRINSIC_END;
}


__intrinsic
void cmd_mem_sub64_imm_ptr32(
    int data,                               /* immediate data                       */
    volatile void __addr32 __mem *address,  /* address to operate on                */
    uint32_t count                          /* number of 64-bit words to subtract from  */
    )
{
    _INTRINSIC_BEGIN;
    {
        _MEM_IMMEDIATE_RANGE_CHECK_SIGNED(data, count, 4);
        _MEM_IMPLEMENT_IMMEDIATE_64BIT_COMMAND(sub64_imm);
    }
    _INTRINSIC_END;
}


__intrinsic
void cmd_mem_sub64_imm_ptr40(
    int data,                               /* immediate data                       */
    volatile void __addr40 __mem *address,  /* address to operate on                */
    uint32_t count                          /* number of 32-bit words to write      */
    )
{
    _INTRINSIC_BEGIN;
    {
        _MEM_IMMEDIATE_RANGE_CHECK_SIGNED(data, count, 4);
        _MEM_IMPLEMENT_IMMEDIATE_64BIT_COMMAND_PTR40(sub64_imm);
    }
    _INTRINSIC_END;
}


/* Decrement 64 integer */
__intrinsic
void cmd_mem_decr64_ptr32(
    volatile void __addr32 __mem *address           /* address to decrement                 */
    )
{
    _INTRINSIC_BEGIN;
    {
        __asm
        {
            mem[decr64, --, address, 0]
        }
    }
    _INTRINSIC_END;
}


__intrinsic
void cmd_mem_decr64_ptr40(
    volatile void __addr40 __mem *address     /* address to decrement                 */
    )
{
    _INTRINSIC_BEGIN;
    {
        uint32_t hi_addr, low_addr;
        _INTRINSIC_CONVERT_HI_LO_ADDRESS(address);
        __asm
        {
            mem[decr64, --, hi_addr, <<8, low_addr]
        }
    }
    _INTRINSIC_END;
}


__intrinsic
void cmd_mem_test_and_sub64_imm_ptr32(
    int data,                               /* immediate data                       */
    __xread void *rd_back,                  /* returned pre-modified value          */
    volatile void __addr32 __mem *address,  /* address to operate on                */
    uint32_t count,                         /* number of 32-bit words to write      */
    sync_t sync,                            /* must be sig_done                     */
    SIGNAL *sig_ptr                         /* signal to raise upon completion      */
    )
{
    _INTRINSIC_BEGIN;
    _MEM_TEST_IMMEDIATE_RANGE_CHECK_SIGNED(data, count, 4);
    {
        generic_ind_t ind;

        // override length and bytemask
        _INTRINSIC_OVERRIDE_LENGTH_AND_BYTEMASK_IN_ALU(ind,  ((count - 1) & 3) | (1 << 2), data & 0xff);

        __asm
        {
            alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
            mem[test_sub64_imm, *rd_back, address, 0], sig_done[*sig_ptr], num_sigs[1], indirect_ref
        }
    }


    _INTRINSIC_END;
}

__intrinsic
void cmd_mem_test_and_sub64_imm_ptr40(
    int data,                               /* immediate data                       */
    __xread void *rd_back,                  /* returned pre-modified value          */
    volatile void __addr40 __mem *address,  /* address to operate on                */
    uint32_t count,                         /* number of 32-bit words to write      */
    sync_t sync,                            /* must be sig_done                     */
    SIGNAL *sig_ptr                         /* signal to raise upon completion      */
    )
{
    _INTRINSIC_BEGIN;

    _MEM_TEST_IMMEDIATE_RANGE_CHECK_SIGNED(data, count, 4);

    {
        generic_ind_t ind;
        uint32_t hi_addr, low_addr;
        _INTRINSIC_CONVERT_HI_LO_ADDRESS(address);

        // override length and bytemask
        _INTRINSIC_OVERRIDE_LENGTH_AND_BYTEMASK_IN_ALU(ind, ((count - 1) & 3) | (1 << 2), data & 0xff);
        __asm
        {
            alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
            mem[test_sub64_imm, *rd_back, hi_addr, <<8, low_addr], sig_done[*sig_ptr], num_sigs[1], indirect_ref
        }
    }

    _INTRINSIC_END;
}

/* Subtract with saturation */
__intrinsic
void cmd_mem_sub32_sat_ptr32(
    __xwrite void *data,                    /* data to write                        */
    volatile void __addr32 __mem *address,  /* address to operate on                */
    uint32_t count,                         /* number of 32-bit words to write      */
    sync_t sync,                            /* sig_done or ctx_swap                 */
    SIGNAL *sig_ptr                         /* signal to raise upon completion      */
    )
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_write_reg(data));
        _MEM_IMPLEMENT_COMMAND(subsat, cmd_mem_sub32_sat_ptr32, 1, 4);
    }
    _INTRINSIC_END;
}

__intrinsic
void cmd_mem_sub32_sat_ptr40(
    __xwrite void *data,                    /* data to write                        */
    volatile void __addr40 __mem *address,  /* address to operate on                */
    uint32_t count,                         /* number of 32-bit words to write      */
    sync_t sync,                            /* sig_done or ctx_swap                 */
    SIGNAL *sig_ptr                         /* signal to raise upon completion      */
    )
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_write_reg(data));
        _MEM_IMPLEMENT_COMMAND_PTR40(subsat, cmd_mem_sub32_sat_ptr40, 1, 4);
    }
    _INTRINSIC_END;
}


__intrinsic
void cmd_mem_sub32_sat_ind_ptr32(
    __xwrite void *data,                    /* data to write                        */
    volatile void __addr32 __mem *address,  /* address to operate on                */
    uint32_t max_nn,                        /* max. number of 32-bit words to write */
    generic_ind_t ind,                      /* indirect word                        */
    sync_t sync,                            /* sig_done or ctx_swap                 */
    SIGNAL *sig_ptr                         /* signal to raise upon completion      */
    )
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_write_reg(data));
        _MEM_IMPLEMENT_INDIRECT_COMMAND(subsat, 1, 4);
    }
    _INTRINSIC_END;
}

__intrinsic
void cmd_mem_sub32_sat_ind_ptr40(
    __xwrite void *data,                    /* data to write                        */
    volatile void __addr40 __mem *address,  /* address to operate on                */
    uint32_t max_nn,                        /* max. number of 32-bit words to write */
    generic_ind_t ind,                      /* indirect word                        */
    sync_t sync,                            /* sig_done or ctx_swap                 */
    SIGNAL *sig_ptr                         /* signal to raise upon completion      */
    )
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_write_reg(data));
        __asm local_csr_wr[CMD_INDIRECT_REF_0, CSR_INDIRECT_TYPE(ind)]
        _MEM_IMPLEMENT_INDIRECT_COMMAND_PTR40(subsat, 1, 4);
    }
    _INTRINSIC_END;
}


__intrinsic
void cmd_mem_sub64_sat_ptr32(
    __xwrite void *data,                    /* data to write                        */
    volatile void __addr32 __mem *address,  /* address to operate on                */
    uint32_t count,                         /* number of 32-bit words to write      */
    sync_t sync,                            /* sig_done or ctx_swap                 */
    SIGNAL *sig_ptr                         /* signal to raise upon completion      */
    )
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_write_reg(data));
        _MEM_IMPLEMENT_ARITHMETIC64_COMMAND(subsat64, cmd_mem_sub64_sat_ptr32, 1, 4);
    }
    _INTRINSIC_END;
}

__intrinsic
void cmd_mem_sub64_sat_ptr40(
    __xwrite void *data,                    /* data to write                        */
    volatile void __addr40 __mem *address,  /* address to operate on                */
    uint32_t count,                         /* number of 32-bit words to write      */
    sync_t sync,                            /* sig_done or ctx_swap                 */
    SIGNAL *sig_ptr                         /* signal to raise upon completion      */
    )
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_write_reg(data));
        _MEM_IMPLEMENT_ARITHMETIC64_COMMAND_PTR40(subsat64, cmd_mem_sub64_sat_ptr40, 1, 4);
    }
    _INTRINSIC_END;
}


__intrinsic
void cmd_mem_sub64_sat_ind_ptr32(
    __xwrite void *data,                    /* data to write                        */
    volatile void __addr32 __mem *address,  /* address to operate on                */
    uint32_t max_nn,                        /* max. number of 32-bit words to write */
    generic_ind_t ind,                      /* indirect word                        */
    sync_t sync,                            /* sig_done or ctx_swap                 */
    SIGNAL *sig_ptr                         /* signal to raise upon completion      */
    )
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_write_reg(data));
        _MEM_IMPLEMENT_INDIRECT_COMMAND(subsat64, 1, 4);
    }
    _INTRINSIC_END;
}

__intrinsic
void cmd_mem_sub64_sat_ind_ptr40(
    __xwrite void *data,                    /* data to write                        */
    volatile void __addr40 __mem *address,  /* address to operate on                */
    uint32_t max_nn,                        /* max. number of 32-bit words to write */
    generic_ind_t ind,                      /* indirect word                        */
    sync_t sync,                            /* sig_done or ctx_swap                 */
    SIGNAL *sig_ptr                         /* signal to raise upon completion      */
    )
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_write_reg(data));

        __asm local_csr_wr[CMD_INDIRECT_REF_0, CSR_INDIRECT_TYPE(ind)]

        _MEM_IMPLEMENT_INDIRECT_COMMAND_PTR40(subsat64, 1, 4);
    }
    _INTRINSIC_END;
}


/* Test and sub operations */
__intrinsic
void cmd_mem_test_and_sub32_sat_ptr32(
    __xread uint32_t *val,                  /* returned pre-modified value          */
    __xwrite void *data,                    /* data to subtract                     */
    volatile void __addr32 __mem *address,  /* address to operate on                */
    uint32_t count,                         /* number of 32-bit words to write      */
    sync_t sync,                            /* must be sig_done                     */
    SIGNAL_PAIR *sig_pair_ptr               /* signal pair to raise upon completion */
    )
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_read_reg(val));
        CT_ASSERT(__is_write_reg(data));
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done);
        __associate_read_write_reg_pair_no_spill(val, data);

        if (__is_ct_const(count))
        {
            CT_ASSERT(count >= 1 && count <= 4);
            __asm mem[test_subsat, *val, address, 0, __ct_const_val(count)], sig_done[*__ct_sig_pair(sig_pair_ptr)]
        }
        else
        {
            CT_QPERFINFO_INDIRECT_REF(cmd_mem_test_and_sub32_sat_ptr32);
            {
                _INTRINSIC_IND_ALU_COUNT_MIN_MAX(count, 1, 4);
                __asm mem[test_subsat, *val, address, 0, __ct_const_val(count)], sig_done[*__ct_sig_pair(sig_pair_ptr)], indirect_ref
            }
        }
    }
    _INTRINSIC_END;
}

__intrinsic
void cmd_mem_test_and_sub32_sat_ptr40(
    __xread uint32_t *val,                  /* returned pre-modified value          */
    __xwrite void *data,                    /* data to subtract                     */
    volatile void __addr40 __mem *address,  /* address to operate on                */
    uint32_t count,                         /* number of 32-bit words to write      */
    sync_t sync,                            /* must be sig_done                     */
    SIGNAL_PAIR *sig_pair_ptr               /* signal pair to raise upon completion */
    )
{
    _INTRINSIC_BEGIN;
    {
        uint32_t hi_addr, low_addr;
        _INTRINSIC_CONVERT_HI_LO_ADDRESS(address);
        CT_ASSERT(__is_read_reg(val));
        CT_ASSERT(__is_write_reg(data));
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done);
        __associate_read_write_reg_pair_no_spill(val, data);

        if (__is_ct_const(count))
        {
            CT_ASSERT(count >= 1 && count <= 4);
            __asm mem[test_subsat, *val, hi_addr, <<8, low_addr, __ct_const_val(count)], sig_done[*__ct_sig_pair(sig_pair_ptr)]
        }
        else
        {
            CT_QPERFINFO_INDIRECT_REF(cmd_mem_test_and_sub32_sat_ptr40);
            {
                _INTRINSIC_IND_ALU_COUNT_MIN_MAX(count, 1, 4);
                __asm mem[test_subsat, *val, hi_addr, <<8, low_addr, __ct_const_val(count)], sig_done[*__ct_sig_pair(sig_pair_ptr)], indirect_ref
            }
        }
    }
    _INTRINSIC_END;
}


__intrinsic
void cmd_mem_test_and_sub32_sat_ind_ptr32(
    __xread uint32_t *val,                  /* returned pre-modified value          */
    __xwrite void *data,                    /* data to subtract                     */
    volatile void __addr32 __mem *address,  /* address to operate on                */
    uint32_t max_nn,                        /* max. number of 32-bit words to write */
    generic_ind_t ind,                      /* indirect word                        */
    sync_t sync,                            /* must be sig_done                     */
    SIGNAL_PAIR *sig_pair_ptr               /* signal pair to raise upon completion */
    )
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_read_reg(val));
        CT_ASSERT(__is_write_reg(data));
        CT_ASSERT(__is_ct_const(max_nn));
        CT_ASSERT(max_nn >= 1 && max_nn <= 4);
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done);
        __associate_read_write_reg_pair_no_spill(val, data);
        __asm local_csr_wr[CMD_INDIRECT_REF_0, CSR_INDIRECT_TYPE(ind)]
        __asm
        {
            alu[--, --, B, ind]
            mem[test_subsat, *val, address, 0, __ct_const_val(max_nn)], sig_done[*__ct_sig_pair(sig_pair_ptr)], indirect_ref
        }
    }
    _INTRINSIC_END;
}

__intrinsic
void cmd_mem_test_and_sub32_sat_ind_ptr40(
    __xread uint32_t *val,                  /* returned pre-modified value          */
    __xwrite void *data,                    /* data to subtract                     */
    volatile void __addr40 __mem *address,  /* address to operate on                */
    uint32_t max_nn,                        /* max. number of 32-bit words to write */
    generic_ind_t ind,                      /* indirect word                        */
    sync_t sync,                            /* must be sig_done                     */
    SIGNAL_PAIR *sig_pair_ptr               /* signal pair to raise upon completion */
    )
{
    _INTRINSIC_BEGIN;
    {
        uint32_t hi_addr, low_addr;
        _INTRINSIC_CONVERT_HI_LO_ADDRESS(address);
        CT_ASSERT(__is_read_reg(val));
        CT_ASSERT(__is_write_reg(data));
        CT_ASSERT(__is_ct_const(max_nn));
        CT_ASSERT(max_nn >= 1 && max_nn <= 4);
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done);
        __associate_read_write_reg_pair_no_spill(val, data);
        __asm local_csr_wr[CMD_INDIRECT_REF_0, CSR_INDIRECT_TYPE(ind)]
        __asm
        {
            alu[--, --, B, ind]
            mem[test_subsat, *val, hi_addr, <<8, low_addr, __ct_const_val(max_nn)], sig_done[*__ct_sig_pair(sig_pair_ptr)], indirect_ref
        }
    }
    _INTRINSIC_END;
}


__intrinsic
void cmd_mem_test_and_sub64_sat_ptr32(
    __xread uint32_t *val,                  /* returned pre-modified value          */
    __xwrite void *data,                    /* data to subtract                     */
    volatile void __addr32 __mem *address,  /* address to operate on                */
    uint32_t count,                         /* number of 32-bit words to write      */
    sync_t sync,                            /* must be sig_done                     */
    SIGNAL_PAIR *sig_pair_ptr               /* signal pair to raise upon completion */
    )
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_read_reg(val));
        CT_ASSERT(__is_write_reg(data));
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done);
        __associate_read_write_reg_pair_no_spill(val, data);

        if (__is_ct_const(count))
        {
            CT_ASSERT(count >= 1 && count <= 4);
            __asm mem[test_subsat64, *val, address, 0, __ct_const_val(count)], sig_done[*__ct_sig_pair(sig_pair_ptr)]
        }
        else
        {
            CT_QPERFINFO_INDIRECT_REF(cmd_mem_test_and_sub64_sat_ptr32);
            {
                _MEM_ARITHMETIC_64_IND_ALU_COUNT_MIN_MAX(count, 1, 4);
                __asm mem[test_subsat64, *val, address, 0, __ct_const_val(count)], sig_done[*__ct_sig_pair(sig_pair_ptr)], indirect_ref
            }
        }
    }
    _INTRINSIC_END;
}

__intrinsic
void cmd_mem_test_and_sub64_sat_ptr40(
    __xread uint32_t *val,                  /* returned pre-modified value          */
    __xwrite void *data,                    /* data to subtract                     */
    volatile void __addr40 __mem *address,  /* address to operate on                */
    uint32_t count,                         /* number of 32-bit words to write      */
    sync_t sync,                            /* must be sig_done                     */
    SIGNAL_PAIR *sig_pair_ptr               /* signal pair to raise upon completion */
    )
{
    _INTRINSIC_BEGIN;
    {
        uint32_t hi_addr, low_addr;
        _INTRINSIC_CONVERT_HI_LO_ADDRESS(address);
        CT_ASSERT(__is_read_reg(val));
        CT_ASSERT(__is_write_reg(data));
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done);
        __associate_read_write_reg_pair_no_spill(val, data);

        if (__is_ct_const(count))
        {
            CT_ASSERT(count >= 1 && count <= 4);
            __asm mem[test_subsat64, *val, hi_addr, <<8, low_addr, __ct_const_val(count)], sig_done[*__ct_sig_pair(sig_pair_ptr)]
        }
        else
        {
            CT_QPERFINFO_INDIRECT_REF(cmd_mem_test_and_sub64_sat_ptr40);
            {
                _MEM_ARITHMETIC_64_IND_ALU_COUNT_MIN_MAX(count, 1, 4);
                __asm mem[test_subsat64, *val, hi_addr, <<8, low_addr, __ct_const_val(count)], sig_done[*__ct_sig_pair(sig_pair_ptr)], indirect_ref
            }
        }
    }
    _INTRINSIC_END;
}


__intrinsic
void cmd_mem_test_and_sub64_sat_ind_ptr32(
    __xread uint32_t *val,                  /* returned pre-modified value          */
    __xwrite void *data,                    /* data to subtract                     */
    volatile void __addr32 __mem *address,  /* address to operate on                */
    uint32_t max_nn,                        /* max. number of 32-bit words to write */
    generic_ind_t ind,                      /* indirect word                        */
    sync_t sync,                            /* must be sig_done                     */
    SIGNAL_PAIR *sig_pair_ptr               /* signal pair to raise upon completion */
    )
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_read_reg(val));
        CT_ASSERT(__is_write_reg(data));
        CT_ASSERT(__is_ct_const(max_nn));
        CT_ASSERT(max_nn >= 1 && max_nn <= 4);
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done);
        __associate_read_write_reg_pair_no_spill(val, data);
        __asm local_csr_wr[CMD_INDIRECT_REF_0, CSR_INDIRECT_TYPE(ind)]
        __asm
        {
            alu[--, --, B, ind]
            mem[test_subsat64, *val, address, 0, __ct_const_val(max_nn)], sig_done[*__ct_sig_pair(sig_pair_ptr)], indirect_ref
        }
    }
    _INTRINSIC_END;
}

__intrinsic
void cmd_mem_test_and_sub64_sat_ind_ptr40(
    __xread uint32_t *val,                  /* returned pre-modified value          */
    __xwrite void *data,                    /* data to subtract                     */
    volatile void __addr40 __mem *address,  /* address to operate on                */
    uint32_t max_nn,                        /* max. number of 32-bit words to write */
    generic_ind_t ind,                      /* indirect word                        */
    sync_t sync,                            /* must be sig_done                     */
    SIGNAL_PAIR *sig_pair_ptr               /* signal pair to raise upon completion */
    )
{
    _INTRINSIC_BEGIN;
    {
        uint32_t hi_addr, low_addr;
        _INTRINSIC_CONVERT_HI_LO_ADDRESS(address);
        CT_ASSERT(__is_read_reg(val));
        CT_ASSERT(__is_write_reg(data));
        CT_ASSERT(__is_ct_const(max_nn));
        CT_ASSERT(max_nn >= 1 && max_nn <= 4);
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done);
        __associate_read_write_reg_pair_no_spill(val, data);
        __asm local_csr_wr[CMD_INDIRECT_REF_0, CSR_INDIRECT_TYPE(ind)]
        __asm
        {
            alu[--, --, B, ind]
            mem[test_subsat64, *val, hi_addr, <<8, low_addr, __ct_const_val(max_nn)], sig_done[*__ct_sig_pair(sig_pair_ptr)], indirect_ref
        }
    }
    _INTRINSIC_END;
}


/* Subtract immediate operations */
__intrinsic
void cmd_mem_sub32_imm_sat_ptr32(
    int data,                               /* immediate data                       */
    volatile void __addr32 __mem *address,  /* address to operate on                */
    uint32_t count                          /* number of 32-bit words to write      */
    )
{
    _INTRINSIC_BEGIN;
    {
        _MEM_IMMEDIATE_RANGE_CHECK_SIGNED(data, count, 4);
        _MEM_IMPLEMENT_IMMEDIATE_COMMAND(subsat_imm);
    }
    _INTRINSIC_END;
}

__intrinsic
void cmd_mem_sub32_imm_sat_ptr40(
    int data,                               /* immediate data                       */
    volatile void __addr40 __mem *address,  /* address to operate on                */
    uint32_t count                          /* number of 32-bit words to write      */
    )
{
    _INTRINSIC_BEGIN;
    {
        _MEM_IMMEDIATE_RANGE_CHECK_SIGNED(data, count, 4);
        _MEM_IMPLEMENT_IMMEDIATE_COMMAND_PTR40(subsat_imm);
#ifndef __NFP_THIRD_PARTY_ADDRESSING_40_BIT
_INTRINSIC_NEED_40_BIT_THIRD_PARTY();
#endif
    }
    _INTRINSIC_END;
}

__intrinsic
void cmd_mem_test_and_sub32_imm_sat_ptr32(
    int data,                               /* immediate data                       */
    __xread void *rd_back,                  /* returned pre-modified value          */
    volatile void __addr32 __mem *address,  /* address to operate on                */
    uint32_t count,                         /* number of 32-bit words to write      */
    sync_t sync,                            /* must be sig_done                     */
    SIGNAL *sig_ptr                         /* signal to raise upon completion      */
    )
{
    _MEM_TEST_IMMEDIATE_RANGE_CHECK_SIGNED(data, count, 4);

    CT_ASSERT(__is_read_reg(rd_back));
    CT_ASSERT(__is_ct_const(sync));
    CT_ASSERT(sync == sig_done);

    {
        generic_ind_t ind;

        // override length and bytemask
        _INTRINSIC_OVERRIDE_LENGTH_AND_BYTEMASK_IN_ALU(ind,  (count - 1), (data & 0xff));

        __asm
        {
            alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
            mem[test_subsat_imm, *rd_back, address, 0], sig_done[*sig_ptr], num_sigs[1], indirect_ref
        }
    }

    _INTRINSIC_END;
}

__intrinsic
void cmd_mem_test_and_sub32_imm_sat_ptr40(
    int data,                               /* immediate data                       */
    __xread void *rd_back,                  /* returned pre-modified value          */
    volatile void __addr40 __mem *address,  /* address to operate on                */
    uint32_t count,                         /* number of 32-bit words to write      */
    sync_t sync,                            /* must be sig_done                     */
    SIGNAL *sig_ptr                         /* signal to raise upon completion      */
    )
{
    _INTRINSIC_BEGIN;

    _MEM_TEST_IMMEDIATE_RANGE_CHECK_SIGNED(data, count, 4);

    CT_ASSERT(__is_read_reg(rd_back));
    CT_ASSERT(__is_ct_const(sync));
    CT_ASSERT(sync == sig_done);

    {
        generic_ind_t ind;
        uint32_t hi_addr, low_addr;
        _INTRINSIC_CONVERT_HI_LO_ADDRESS(address);

        // override length and bytemask
        _INTRINSIC_OVERRIDE_LENGTH_AND_BYTEMASK_IN_ALU(ind,  (count - 1), (data & 0xff));

        __asm
        {
            alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
            mem[test_subsat_imm, *rd_back, hi_addr, <<8, low_addr], sig_done[*sig_ptr], num_sigs[1], indirect_ref
    }
    }

    _INTRINSIC_END;
}

__intrinsic
void cmd_mem_sub64_imm_sat_ptr32(
    int data,                               /* immediate data                       */
    volatile void __addr32 __mem *address,  /* address to operate on                */
    uint32_t count                          /* number of 32-bit words to write      */
    )
{
    _INTRINSIC_BEGIN;
    {
        _MEM_IMMEDIATE_RANGE_CHECK_SIGNED(data, count, 4);

        _MEM_IMPLEMENT_IMMEDIATE_64BIT_COMMAND(subsat64_imm);



    }
    _INTRINSIC_END;
}

__intrinsic
void cmd_mem_sub64_imm_sat_ptr40(
    int data,                               /* immediate data                       */
    volatile void __addr40 __mem *address,  /* address to operate on                */
    uint32_t count                          /* number of 32-bit words to write      */
    )
{
    _INTRINSIC_BEGIN;
    {
        _MEM_IMMEDIATE_RANGE_CHECK_SIGNED(data, count, 4);
        _MEM_IMPLEMENT_IMMEDIATE_64BIT_COMMAND_PTR40(subsat64_imm);
    }

#ifndef __NFP_THIRD_PARTY_ADDRESSING_40_BIT
_INTRINSIC_NEED_40_BIT_THIRD_PARTY();
#endif

    _INTRINSIC_END;
}

__intrinsic
void cmd_mem_test_and_sub64_imm_sat_ptr32(
    int data,                               /* immediate data                       */
    __xread void *rd_back,                  /* returned pre-modified value          */
    volatile void __addr32 __mem *address,  /* address to operate on                */
    uint32_t count,                         /* number of 32-bit words to write      */
    sync_t sync,                            /* must be sig_done                     */
    SIGNAL *sig_ptr                         /* signal to raise upon completion      */
    )
{
    _INTRINSIC_BEGIN;
    _MEM_TEST_IMMEDIATE_RANGE_CHECK_SIGNED(data, count, 4);

    CT_ASSERT(__is_read_reg(rd_back));
    CT_ASSERT(__is_ct_const(sync));
    CT_ASSERT(sync == sig_done);
    {
        generic_ind_t ind;
        // override length and bytemask
        _INTRINSIC_OVERRIDE_LENGTH_AND_BYTEMASK_IN_ALU(ind,  (((count - 1) & 3) | (1 << 2)),  (data & 0xff));
        __asm
        {
            alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
            mem[test_subsat64_imm, *rd_back, address, 0], sig_done[*sig_ptr], num_sigs[1], indirect_ref
        }
    }

    _INTRINSIC_END;
}

__intrinsic
void cmd_mem_test_and_sub64_imm_sat_ptr40(
    int data,                               /* immediate data                       */
    __xread void *rd_back,                  /* returned pre-modified value          */
    volatile void __addr40 __mem *address,  /* address to operate on                */
    uint32_t count,                         /* number of 32-bit words to write      */
    sync_t sync,                            /* must be sig_done                     */
    SIGNAL *sig_ptr                         /* signal to raise upon completion      */
    )
{
    _INTRINSIC_BEGIN;
    _MEM_TEST_IMMEDIATE_RANGE_CHECK_SIGNED(data, count, 4);
    {
        generic_ind_t ind;
        uint32_t hi_addr, low_addr;
        _INTRINSIC_CONVERT_HI_LO_ADDRESS(address);

        // override length and bytemask
        _INTRINSIC_OVERRIDE_LENGTH_AND_BYTEMASK_IN_ALU(ind,  (((count - 1) & 3) | (1 << 2)),  (data & 0xff));
        __asm
        {
            alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
            mem[test_subsat64_imm, *rd_back, hi_addr, <<8, low_addr], sig_done[*sig_ptr], num_sigs[1], indirect_ref
        }
    }

    _INTRINSIC_END;
}

/* Exclusive or operations */
__intrinsic
void cmd_mem_xor_ptr32(
    __xwrite void *data,                    /* data to write                        */
    volatile void __addr32 __mem *address,  /* address to operate on                */
    uint32_t count,                         /* number of 32-bit words to write      */
    sync_t sync,                            /* sig_done or ctx_swap                 */
    SIGNAL *sig_ptr                         /* signal to raise upon completion      */
    )
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_write_reg(data));
        _MEM_IMPLEMENT_COMMAND(xor, cmd_mem_xor_ptr32, 1, 8);
    }
    _INTRINSIC_END;
}


__intrinsic
void cmd_mem_xor_ptr40(
    __xwrite void *data,                    /* data to write                        */
    volatile void __addr40 __mem *address,  /* address to operate on                */
    uint32_t count,                         /* number of 32-bit words to write      */
    sync_t sync,                            /* sig_done or ctx_swap                 */
    SIGNAL *sig_ptr                         /* signal to raise upon completion      */
    )
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_write_reg(data));
        _MEM_IMPLEMENT_COMMAND_PTR40(xor, cmd_mem_xor_ptr40, 1, 8);
    }
    _INTRINSIC_END;
}


__intrinsic
void cmd_mem_xor_ind_ptr32(
    __xwrite void *data,                    /* data to write                        */
    volatile void __addr32 __mem *address,  /* address to operate on                */
    uint32_t max_nn,                        /* max. number of 32-bit words to write */
    generic_ind_t ind,                      /* indirect word                        */
    sync_t sync,                            /* sig_done or ctx_swap                 */
    SIGNAL *sig_ptr                         /* signal to raise upon completion      */
    )
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_write_reg(data));
        _MEM_IMPLEMENT_INDIRECT_COMMAND(xor, 1, 8);
    }
    _INTRINSIC_END;
}


__intrinsic
void cmd_mem_xor_ind_ptr40(
    __xwrite void *data,                    /* data to write                        */
    volatile void __addr40 __mem *address,  /* address to operate on                */
    uint32_t max_nn,                        /* max. number of 32-bit words to write */
    generic_ind_t ind,                      /* indirect word                        */
    sync_t sync,                            /* sig_done or ctx_swap                 */
    SIGNAL *sig_ptr                         /* signal to raise upon completion      */
    )
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_write_reg(data));
        __asm local_csr_wr[CMD_INDIRECT_REF_0, CSR_INDIRECT_TYPE(ind)]
        _MEM_IMPLEMENT_INDIRECT_COMMAND_PTR40(xor, 1, 8);
    }
    _INTRINSIC_END;
}


__intrinsic
void cmd_mem_test_and_xor_ptr32(
    __xread uint32_t *val,                  /* returned pre-modified value          */
    __xwrite void *data,                    /* data to xor                          */
    volatile void __addr32 __mem *address,  /* address to operate on                */
    uint32_t count,                         /* number of 32-bit words to write      */
    sync_t sync,                            /* must be sig_done                     */
    SIGNAL_PAIR *sig_pair_ptr               /* signal pair to raise upon completion */
    )
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_read_reg(val));
        CT_ASSERT(__is_write_reg(data));
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done);
        __associate_read_write_reg_pair_no_spill(val, data);

        if (__is_ct_const(count))
        {
            CT_ASSERT(count >= 1 && count <= 8);
            __asm mem[test_xor, *val, address, 0, __ct_const_val(count)], sig_done[*__ct_sig_pair(sig_pair_ptr)]
        }
        else
        {
            CT_QPERFINFO_INDIRECT_REF(cmd_mem_test_and_xor_ptr32);
            {
                _INTRINSIC_IND_ALU_COUNT_MIN_MAX(count, 1, 8);
                __asm mem[test_xor, *val, address, 0, __ct_const_val(count)], sig_done[*__ct_sig_pair(sig_pair_ptr)], indirect_ref
            }
        }
    }
    _INTRINSIC_END;
}

__intrinsic
void cmd_mem_test_and_xor_ptr40(
    __xread uint32_t *val,                  /* returned pre-modified value          */
    __xwrite void *data,                    /* data to xor                          */
    volatile void __addr40 __mem *address,  /* address to operate on                */
    uint32_t count,                         /* number of 32-bit words to write      */
    sync_t sync,                            /* must be sig_done                     */
    SIGNAL_PAIR *sig_pair_ptr               /* signal pair to raise upon completion */
    )
{
    _INTRINSIC_BEGIN;
    {
        uint32_t hi_addr, low_addr;
        _INTRINSIC_CONVERT_HI_LO_ADDRESS(address);
        CT_ASSERT(__is_read_reg(val));
        CT_ASSERT(__is_write_reg(data));
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done);
        __associate_read_write_reg_pair_no_spill(val, data);

        if (__is_ct_const(count))
        {
            CT_ASSERT(count >= 1 && count <= 8);
            __asm mem[test_xor, *val, hi_addr, <<8, low_addr, __ct_const_val(count)], sig_done[*__ct_sig_pair(sig_pair_ptr)]
        }
        else
        {
            CT_QPERFINFO_INDIRECT_REF(cmd_mem_test_and_xor_ptr40);
            {
                _INTRINSIC_IND_ALU_COUNT_MIN_MAX(count, 1, 8);
                __asm mem[test_xor, *val, hi_addr, <<8, low_addr, __ct_const_val(count)], sig_done[*__ct_sig_pair(sig_pair_ptr)], indirect_ref
            }
        }
    }
    _INTRINSIC_END;
}


__intrinsic
void cmd_mem_test_and_xor_ind_ptr32(
    __xread uint32_t *val,                  /* returned pre-modified value          */
    __xwrite void *data,                    /* data to xor                          */
    volatile void __addr32 __mem *address,  /* address to operate on                */
    uint32_t max_nn,                        /* max. number of 32-bit words to write */
    generic_ind_t ind,                      /* indirect word                        */
    sync_t sync,                            /* must be sig_done                     */
    SIGNAL_PAIR *sig_pair_ptr               /* signal pair to raise upon completion */
    )
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_read_reg(val));
        CT_ASSERT(__is_write_reg(data));
        CT_ASSERT(__is_ct_const(max_nn));
        CT_ASSERT(max_nn >= 1 && max_nn <= 8);
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done);
        __associate_read_write_reg_pair_no_spill(val, data);
        __asm local_csr_wr[CMD_INDIRECT_REF_0, CSR_INDIRECT_TYPE(ind)]
        __asm
        {
            alu[--, --, B, ind]
            mem[test_xor, *val, address, 0, __ct_const_val(max_nn)], sig_done[*__ct_sig_pair(sig_pair_ptr)], indirect_ref
        }
    }
    _INTRINSIC_END;
}

__intrinsic
void cmd_mem_test_and_xor_ind_ptr40(
    __xread uint32_t *val,                  /* returned pre-modified value          */
    __xwrite void *data,                    /* data to xor                          */
    volatile void __addr40 __mem *address,  /* address to operate on                */
    uint32_t max_nn,                        /* max. number of 32-bit words to write */
    generic_ind_t ind,                      /* indirect word                        */
    sync_t sync,                            /* must be sig_done                     */
    SIGNAL_PAIR *sig_pair_ptr               /* signal pair to raise upon completion */
    )
{
    _INTRINSIC_BEGIN;
    {
        uint32_t hi_addr, low_addr;
        _INTRINSIC_CONVERT_HI_LO_ADDRESS(address);
        CT_ASSERT(__is_read_reg(val));
        CT_ASSERT(__is_write_reg(data));
        CT_ASSERT(__is_ct_const(max_nn));
        CT_ASSERT(max_nn >= 1 && max_nn <= 8);
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done);
        __associate_read_write_reg_pair_no_spill(val, data);
        __asm local_csr_wr[CMD_INDIRECT_REF_0, CSR_INDIRECT_TYPE(ind)]
        __asm
        {
            alu[--, --, B, ind]
            mem[test_xor, *val, hi_addr, <<8, low_addr, __ct_const_val(max_nn)], sig_done[*__ct_sig_pair(sig_pair_ptr)], indirect_ref
        }
    }
    _INTRINSIC_END;
}



__intrinsic
void cmd_mem_xor_imm_ptr32(
    uint32_t data,                          /* immediate data                       */
    volatile void __addr32 __mem *address,  /* address to operate on                */
    uint32_t count                          /* number of 32-bit words to write      */
    )
{
    _INTRINSIC_BEGIN;
    {
        _MEM_IMMEDIATE_RANGE_CHECK_UNSIGNED(data, count, 8);
        _MEM_IMPLEMENT_IMMEDIATE_COMMAND(xor_imm);
    }
    _INTRINSIC_END;
}

__intrinsic
void cmd_mem_xor_imm_ptr40(
    uint32_t data,                          /* immediate data                       */
    volatile void __addr40 __mem *address,  /* address to operate on                */
    uint32_t count                          /* number of 32-bit words to write      */
    )
{
    _INTRINSIC_BEGIN;
    {
        _MEM_IMMEDIATE_RANGE_CHECK_UNSIGNED(data, count, 8);


        _MEM_IMPLEMENT_IMMEDIATE_COMMAND_PTR40(xor_imm);


#ifndef __NFP_THIRD_PARTY_ADDRESSING_40_BIT
        _INTRINSIC_NEED_40_BIT_THIRD_PARTY();
#endif
    }
    _INTRINSIC_END;
}

__intrinsic
void cmd_mem_test_and_xor_imm_ptr32(
    uint32_t data,                          /* immediate data                       */
    __xread void *rd_back,                  /* returned pre-modified value          */
    volatile void __addr32 __mem *address,  /* address to operate on                */
    uint32_t count,                         /* number of 32-bit words to write      */
    sync_t sync,                            /* must be sig_done                     */
    SIGNAL *sig_ptr                         /* signal to raise upon completion      */
    )
{
    _INTRINSIC_BEGIN;
    _MEM_TEST_IMMEDIATE_RANGE_CHECK_UNSIGNED(data, count, 8);
    CT_ASSERT(__is_read_reg(rd_back));
    CT_ASSERT(__is_ct_const(sync));
    CT_ASSERT(sync == sig_done);

    {
        generic_ind_t ind;
        // override length and bytemask
        _INTRINSIC_OVERRIDE_LENGTH_AND_BYTEMASK_IN_ALU(ind, ((count - 1) & 7), (data & 0x7f));

        __asm
        {
            alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
            mem[test_xor_imm, *rd_back, address, 0, --], sig_done[*sig_ptr], num_sigs[1], indirect_ref
        }
    }

    _INTRINSIC_END;
}


__intrinsic
void cmd_mem_test_and_xor_imm_ptr40(
    uint32_t data,                          /* immediate data                       */
    __xread void *rd_back,                  /* returned pre-modified value          */
    volatile void __addr40 __mem *address,  /* address to operate on                */
    uint32_t count,                         /* number of 32-bit words to write      */
    sync_t sync,                            /* must be sig_done                     */
    SIGNAL *sig_ptr                         /* signal to raise upon completion      */
    )
{
    _INTRINSIC_BEGIN;

    _MEM_TEST_IMMEDIATE_RANGE_CHECK_UNSIGNED(data, count, 8);

    CT_ASSERT(__is_read_reg(rd_back));
    CT_ASSERT(__is_ct_const(sync));
    CT_ASSERT(sync == sig_done);

    {
        generic_ind_t ind;
        uint32_t hi_addr, low_addr;
        _INTRINSIC_CONVERT_HI_LO_ADDRESS(address);
        // override length and bytemask
        _INTRINSIC_OVERRIDE_LENGTH_AND_BYTEMASK_IN_ALU(ind, ((count - 1) & 7), (data & 0x7f));

        __asm
        {
            alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
            mem[test_xor_imm, *rd_back, hi_addr, <<8, low_addr, --], sig_done[*sig_ptr], num_sigs[1], indirect_ref
    }
    }

    _INTRINSIC_END;
}


__intrinsic
void cmd_mem_mask_compare_write_ptr32(
    __xwrite void *data,                    /* data to write                        */
    volatile void __addr32 __mem *address,  /* address to operate on                */
    uint32_t mask,                          /* 4-bit mask of which bytes to compare */
    uint32_t count,                         /* number of 32-bit words to write      */
    sync_t sync,                            /* sig_done or ctx_swap                 */
    SIGNAL *sig_ptr                         /* signal to raise upon completion      */
    )
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_write_reg(data));
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done || sync == ctx_swap);

        _MEM_MIN_MAX_CHECK(count, 1, 8);
        _MEM_MIN_MAX_CHECK(mask, 1, 0xe);           // 0xf is not allowed,
        {
            generic_ind_t ind;
            _INTRINSIC_OVERRIDE_LENGTH_AND_BYTEMASK_IN_ALU(ind, ((count - 1) |0x8), (mask & 0xf));

            if (sync == sig_done)
            {
                __asm
                {
                    alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
                    mem[mask_compare_write, *data, address, 0, --], sig_done[*sig_ptr], indirect_ref
                }
            }
            else
            {
                __asm
                {
                    alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
                    mem[mask_compare_write, *data, address, 0, --], ctx_swap[*sig_ptr], indirect_ref
                }
            }
        }
    }
    _INTRINSIC_END;
}



__intrinsic
void cmd_mem_mask_compare_write_ptr40(
    __xwrite void *data,                    /* data to write                        */
    volatile void __addr40 __mem *address,  /* address to operate on                */
    uint32_t mask,                      /* 4-bit mask of which bytes to compare */
    uint32_t count,                         /* number of 32-bit words to write      */
    sync_t sync,                            /* sig_done or ctx_swap                 */
    SIGNAL *sig_ptr                         /* signal to raise upon completion */
    )
{
    _INTRINSIC_BEGIN;
    {
        // asserts
        CT_ASSERT(__is_write_reg(data));
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done || sync == ctx_swap);

        _MEM_MIN_MAX_CHECK(count, 1, 8);
        _MEM_MIN_MAX_CHECK(mask, 1, 0xe);           // 0xf is not allowed,

        {
            generic_ind_t ind;
            uint32_t hi_addr, low_addr;

            _INTRINSIC_CONVERT_HI_LO_ADDRESS(address);
            _INTRINSIC_OVERRIDE_LENGTH_AND_BYTEMASK_IN_ALU(ind, ((count - 1) |0x8), (mask & 0xf));
            if (sync == sig_done)
            {
                __asm
                {
                    alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
                    mem[mask_compare_write, *data, hi_addr, <<8, low_addr, --], sig_done[*sig_ptr], indirect_ref
                }
            }
            else
            {
                __asm
                {
                    alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
                    mem[mask_compare_write, *data, hi_addr, <<8, low_addr, --], ctx_swap[*sig_ptr], indirect_ref
                }
            }
        }
    }
    _INTRINSIC_END;
}


__intrinsic
void cmd_mem_test_mask_and_compare_write_ptr32(
    __xread void *val,                      /* returned pre-modified value          */
    __xwrite void *data,                    /* data to write                        */
    volatile void __addr32 __mem *address,  /* address to operate on                */
    uint32_t mask,                          /* 4-bit mask of which bytes to compare */
    uint32_t count,                         /* number of 32-bit words to write      */
    sync_t sync,                            /* sig_done or ctx_swap                 */
    SIGNAL_PAIR *sig_pair_ptr               /* signal pair to raise upon completion */
    )
{
    _INTRINSIC_BEGIN;
    {
        // asserts
        CT_ASSERT(__is_read_reg(val));
        CT_ASSERT(__is_write_reg(data));
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done);

        _MEM_MIN_MAX_CHECK(count, 1, 8);
        _MEM_MIN_MAX_CHECK(mask, 1, 0xe);           // 0xf is not allowed,

        {
            generic_ind_t ind;

            _INTRINSIC_OVERRIDE_LENGTH_AND_BYTEMASK_IN_ALU(ind, ((count - 1) | 0x8), (mask & 0xf));
            __associate_read_write_reg_pair_no_spill(val, data);

            if (__is_ct_const(count) && count <= 8)
            {
                __asm
                {
                    alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
                    mem[test_mask_compare_write, *val, address, 0, __ct_const_val(count)], sig_done[*__ct_sig_pair(sig_pair_ptr)], indirect_ref
                }
            }
            else
            {
                if (!__is_ct_const(count))
                {
                    CT_QPERFINFO_INDIRECT_REF(mem_test_mask_compare_write);
                }
                {
                    __asm
                    {
                        alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
                        mem[test_mask_compare_write, *val, address, 0, __ct_const_val(count)], sig_done[*__ct_sig_pair(sig_pair_ptr)], indirect_ref
                    }
                }
            }
        }
    }
    _INTRINSIC_END;
}

__intrinsic
void cmd_mem_test_mask_and_compare_write_ptr40(
    __xread void *val,                      /* returned pre-modified value          */
    __xwrite void *data,                    /* data to write                        */
    volatile void __addr40 __mem *address,  /* address to operate on                */
    uint32_t mask,                          /* 4-bit mask of which bytes to compare */
    uint32_t count,                         /* number of 32-bit words to write      */
    sync_t sync,                            /* must be sig_done                     */
    SIGNAL_PAIR *sig_pair_ptr               /* signal pair to raise upon completion */
    )
{
    _INTRINSIC_BEGIN;
    {
        // asserts
        CT_ASSERT(__is_read_reg(val));
        CT_ASSERT(__is_write_reg(data));
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done);

        _MEM_MIN_MAX_CHECK(count, 1, 8);
        _MEM_MIN_MAX_CHECK(mask, 1, 0xe);           // 0xf is not allowed,

        {
            uint32_t hi_addr, low_addr;
            generic_ind_t ind;

            __associate_read_write_reg_pair_no_spill(val, data);
            _INTRINSIC_CONVERT_HI_LO_ADDRESS(address);
            _INTRINSIC_OVERRIDE_LENGTH_AND_BYTEMASK_IN_ALU(ind, ((count - 1) |0x8), (mask & 0xf));

            if (__is_ct_const(count) && count <= 8)
            {
                __asm
                {
                    alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
                    mem[test_mask_compare_write, *val, hi_addr, <<8, low_addr, __ct_const_val(count)], sig_done[*__ct_sig_pair(sig_pair_ptr)], indirect_ref
                }
            }
            else
            {
                if (!__is_ct_const(count))
                {
                    CT_QPERFINFO_INDIRECT_REF(mem_test_mask_compare_write);
                }
                {
                    __asm
                    {
                        alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
                        mem[test_mask_compare_write, *val, hi_addr, << 8, low_addr, __ct_const_val(count)], sig_done[*__ct_sig_pair(sig_pair_ptr)], indirect_ref
                    }
                }
            }
        }
    }
    _INTRINSIC_END;
}



/* Bulk 32 bit write*/
__intrinsic
void cmd_mem_write32_ptr32(
    __xwrite void *data,                    /* data to write                        */
    volatile void __addr32 __mem *address,  /* address to operate on                */
    uint32_t count,                         /* number of 32-bit words to write      */
    sync_t sync,                            /* sig_done or ctx_swap                 */
    SIGNAL *sig_ptr                         /* signal to raise upon completion      */
    )
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_write_reg(data));
        _MEM_IMPLEMENT_COMMAND(write32, cmd_mem_write32_ptr32, 1, MAX_REF_COUNT_32);
    }
    _INTRINSIC_END;
}

__intrinsic
void cmd_mem_write32_ptr40(
    __xwrite void *data,                        /* data to write                        */
    volatile void __addr40 __mem *address,      /* address to operate on                */
    uint32_t count,                             /* number of 32-bit words to write      */
    sync_t sync,                                /* sig_done or ctx_swap                 */
    SIGNAL *sig_ptr                             /* signal to raise upon completion      */
    )
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_write_reg(data));
        _MEM_IMPLEMENT_COMMAND_PTR40(write32, cmd_mem_write32_ptr40, 1, MAX_REF_COUNT_32);
    }
    _INTRINSIC_END;
}


__intrinsic
 void cmd_mem_write32_ind_ptr32(
    __xwrite void *data,                    /* data to write                        */
    volatile void __addr32 __mem *address,  /* address to operate on                */
    uint32_t max_nn,                        /* max. number of 32-bit words to write */
    generic_ind_t ind,                      /* indirect word                        */
    sync_t sync,                            /* sig_done or ctx_swap                 */
    SIGNAL *sig_ptr                         /* signal to raise upon completion      */
    )
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_write_reg(data));
        _MEM_IMPLEMENT_INDIRECT_COMMAND(write32, 1, INTRINSIC_IND_ALU_COUNT_MAX);
    }
    _INTRINSIC_END;
}


__intrinsic
 void cmd_mem_write32_ind_ptr40(
    __xwrite void *data,                    /* data to write                        */
    volatile void __addr40 __mem *address,  /* address to operate on                */
    uint32_t max_nn,                        /* max. number of 32-bit words to write */
    generic_ind_t ind,                      /* indirect word                        */
    sync_t sync,                            /* sig_done or ctx_swap                 */
    SIGNAL *sig_ptr                         /* signal to raise upon completion      */
    )
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_write_reg(data));
        __asm local_csr_wr[CMD_INDIRECT_REF_0, CSR_INDIRECT_TYPE(ind)]
        _MEM_IMPLEMENT_INDIRECT_COMMAND_PTR40(write32, 1, INTRINSIC_IND_ALU_COUNT_MAX);
    }
    _INTRINSIC_END;
}


/* Bulk 32 bit write*/
__intrinsic
void cmd_mem_write32_le_ptr32(
    __xwrite void *data,                    /* data to write                        */
    volatile void __addr32 __mem *address,  /* address to operate on                */
    uint32_t count,                         /* number of 32-bit words to write      */
    sync_t sync,                            /* sig_done or ctx_swap                 */
    SIGNAL *sig_ptr                         /* signal to raise upon completion      */
    )
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_write_reg(data));
        _MEM_IMPLEMENT_COMMAND(write32_le, cmd_mem_write32_le_ptr32, 1, MAX_REF_COUNT_32);
    }
    _INTRINSIC_END;
}

__intrinsic
void cmd_mem_write32_le_ptr40(
    __xwrite void *data,                        /* data to write                        */
    volatile void __addr40 __mem *address,      /* address to operate on                */
    uint32_t count,                             /* number of 32-bit words to write      */
    sync_t sync,                                /* sig_done or ctx_swap                 */
    SIGNAL *sig_ptr                             /* signal to raise upon completion      */
    )
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_write_reg(data));
        _MEM_IMPLEMENT_COMMAND_PTR40(write32_le, cmd_mem_write32_le_ptr40, 1, MAX_REF_COUNT_32);
    }
    _INTRINSIC_END;
}



__intrinsic
void cmd_mem_read32_ptr32(
    __xread void *data,                     /* returns data from operation          */
    volatile void __addr32 __mem *address,  /* address to read from                 */
    uint32_t count,                         /* number of quadwords to read          */
    sync_t sync,                            /* sig_done or ctx_swap                 */
    SIGNAL *sig_ptr                         /* signal to raise upon completion      */
    )
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_read_reg(data));
        _MEM_IMPLEMENT_COMMAND(read32, cmd_mem_read32_ptr32, 1, MAX_REF_COUNT_32);
    }
    _INTRINSIC_END;
}

__intrinsic
void cmd_mem_read32_ptr40(
    __xread void *data,                     /* returns data from operation          */
    volatile void __addr40 __mem *address,  /* address to read from                 */
    uint32_t count,                         /* number of quadwords to read          */
    sync_t sync,                            /* sig_done or ctx_swap                 */
    SIGNAL *sig_ptr                         /* signal to raise upon completion      */
    )
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_read_reg(data));
        _MEM_IMPLEMENT_COMMAND_PTR40(read32, cmd_mem_read32_ptr40, 1, MAX_REF_COUNT_32);
    }
    _INTRINSIC_END;
}


__intrinsic
 void cmd_mem_read32_ind_ptr32(
    __xread void *data,                     /* data to read                         */
    volatile void __addr32 __mem *address,  /* address to operate on                */
    uint32_t max_nn,                        /* max. number of 32-bit words to read  */
    generic_ind_t ind,                      /* indirect word                        */
    sync_t sync,                            /* sig_done or ctx_swap                 */
    SIGNAL *sig_ptr                         /* signal to raise upon completion      */
    )
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_read_reg(data));
        _MEM_IMPLEMENT_INDIRECT_COMMAND(read32, 1, INTRINSIC_IND_ALU_COUNT_MAX);
    }
    _INTRINSIC_END;
}


__intrinsic
 void cmd_mem_read32_ind_ptr40(
    __xread void *data,                     /* data to read                         */
    volatile void __addr40 __mem *address,  /* address to operate on                */
    uint32_t max_nn,                        /* max. number of 32-bit words to read  */
    generic_ind_t ind,                      /* indirect word                        */
    sync_t sync,                            /* sig_done or ctx_swap                 */
    SIGNAL *sig_ptr                         /* signal to raise upon completion      */
    )
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_read_reg(data));
        __asm local_csr_wr[CMD_INDIRECT_REF_0, CSR_INDIRECT_TYPE(ind)]
        _MEM_IMPLEMENT_INDIRECT_COMMAND_PTR40(read32, 1, INTRINSIC_IND_ALU_COUNT_MAX);
    }
    _INTRINSIC_END;
}


/* Little endian read */
__intrinsic
void cmd_mem_read32_le_ptr32(
    __xread void *data,                     /* returns data from operation          */
    volatile void __addr32 __mem *address,  /* address to read from                 */
    uint32_t count,                         /* number of quadwords to read          */
    sync_t sync,                            /* sig_done or ctx_swap                 */
    SIGNAL *sig_ptr                         /* signal to raise upon completion      */
    )
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_read_reg(data));
        _MEM_IMPLEMENT_COMMAND(read32_le, cmd_mem_read32_le_ptr32, 1, MAX_REF_COUNT_32);
    }
    _INTRINSIC_END;
}

__intrinsic
void cmd_mem_read32_le_ptr40(
    __xread void *data,                     /* returns data from operation          */
    volatile void __addr40 __mem *address,  /* address to read from                 */
    uint32_t count,                         /* number of quadwords to read          */
    sync_t sync,                            /* sig_done or ctx_swap                 */
    SIGNAL *sig_ptr                         /* signal to raise upon completion      */
    )
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_read_reg(data));
        _MEM_IMPLEMENT_COMMAND_PTR40(read32_le, cmd_mem_read32_le_ptr40, 1, MAX_REF_COUNT_32);
    }
    _INTRINSIC_END;
}


__intrinsic
void cmd_mem_read32_swap_ptr32(
    __xread void *data,                     /* returns data from operation          */
    volatile void __addr32 __mem *address,  /* address to read from                 */
    uint32_t count,                         /* number of quadwords to read          */
    sync_t sync,                            /* sig_done or ctx_swap                 */
    SIGNAL *sig_ptr                         /* signal to raise upon completion      */
    )
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_read_reg(data));
        _MEM_IMPLEMENT_COMMAND(read32_swap, cmd_mem_read32_swap_ptr32, 1, MAX_REF_COUNT_32);
    }
    _INTRINSIC_END;
}

__intrinsic
void cmd_mem_read32_swap_ptr40(
    __xread void *data,                     /* returns data from operation          */
    volatile void __addr40 __mem *address,  /* address to read from                 */
    uint32_t count,                         /* number of quadwords to read          */
    sync_t sync,                            /* sig_done or ctx_swap                 */
    SIGNAL *sig_ptr                         /* signal to raise upon completion      */
    )
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_read_reg(data));
        _MEM_IMPLEMENT_COMMAND_PTR40(read32_swap, cmd_mem_read32_swap_ptr40, 1, MAX_REF_COUNT_32);
    }
    _INTRINSIC_END;
}


__intrinsic
 void cmd_mem_read32_swap_ind_ptr32(
    __xread void *data,                     /* data to read                         */
    volatile void __addr32 __mem *address,  /* address to operate on                */
    uint32_t max_nn,                        /* max. number of 32-bit words to read  */
    generic_ind_t ind,                      /* indirect word                        */
    sync_t sync,                            /* sig_done or ctx_swap                 */
    SIGNAL *sig_ptr                         /* signal to raise upon completion      */
    )
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_read_reg(data));
        _MEM_IMPLEMENT_INDIRECT_COMMAND(read32_swap, 1, INTRINSIC_IND_ALU_COUNT_MAX);
    }
    _INTRINSIC_END;
}


__intrinsic
 void cmd_mem_read32_swap_ind_ptr40(
    __xread void *data,                     /* data to read                         */
    volatile void __addr40 __mem *address,  /* address to operate on                */
    uint32_t max_nn,                        /* max. number of 32-bit words to read  */
    generic_ind_t ind,                      /* indirect word                        */
    sync_t sync,                            /* sig_done or ctx_swap                 */
    SIGNAL *sig_ptr                         /* signal to raise upon completion      */
    )
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_read_reg(data));

        __asm local_csr_wr[CMD_INDIRECT_REF_0, CSR_INDIRECT_TYPE(ind)]

        _MEM_IMPLEMENT_INDIRECT_COMMAND_PTR40(read32_swap, 1, INTRINSIC_IND_ALU_COUNT_MAX);
    }
    _INTRINSIC_END;
}


__intrinsic
void cmd_mem_read32_swap_le_ptr32(
    __xread void *data,                         /* returns data from operation          */
    volatile void __addr32 __mem *address,      /* address to read from                 */
    uint32_t count,                             /* number of quadwords to read          */
    sync_t sync,                                /* sig_done or ctx_swap                 */
    SIGNAL *sig_ptr                             /* signal to raise upon completion      */
    )
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_read_reg(data));
        _MEM_IMPLEMENT_COMMAND(read32_swap_le, cmd_mem_read32_swap_le_ptr32, 1, MAX_REF_COUNT_32);
    }
    _INTRINSIC_END;
}

__intrinsic
void cmd_mem_read32_swap_le_ptr40(
    __xread void *data,                        /* returns data from operation          */
    volatile void __addr40 __mem *address,      /* address to read from                 */
    uint32_t count,                             /* number of quadwords to read          */
    sync_t sync,                                /* sig_done or ctx_swap                 */
    SIGNAL *sig_ptr                             /* signal to raise upon completion      */
    )
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_read_reg(data));
        _MEM_IMPLEMENT_COMMAND_PTR40(read32_swap_le, cmd_mem_read32_swap_le_ptr40, 1, MAX_REF_COUNT_32);
    }
    _INTRINSIC_END;
}


/* Bulk 32 bit write with byte swap */
__intrinsic
void cmd_mem_write32_swap_ptr32(
    __xwrite void *data,                        /* data to write                        */
    volatile void __addr32 __mem *address,      /* address to operate on                */
    uint32_t count,                             /* number of 32-bit words to write      */
    sync_t sync,                                /* sig_done or ctx_swap                 */
    SIGNAL *sig_ptr                             /* signal to raise upon completion      */
    )
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_write_reg(data));
        _MEM_IMPLEMENT_COMMAND(write32_swap, cmd_mem_write32_swap_ptr32, 1, MAX_REF_COUNT_32);
    }
    _INTRINSIC_END;
}

__intrinsic
void cmd_mem_write32_swap_ptr40(
    __xwrite void *data,                        /* data to write                        */
    volatile void __addr40 __mem *address,      /* address to operate on                */
    uint32_t count,                             /* number of 32-bit words to write      */
    sync_t sync,                                /* sig_done or ctx_swap                 */
    SIGNAL *sig_ptr                             /* signal to raise upon completion      */
    )
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_write_reg(data));
        _MEM_IMPLEMENT_COMMAND_PTR40(write32_swap, cmd_mem_write32_swap_ptr40, 1, MAX_REF_COUNT_32);
    }
    _INTRINSIC_END;
}


__intrinsic
void cmd_mem_write32_swap_ind_ptr32(
    __xwrite void *data,                    /* data to write                        */
    volatile void __addr32 __mem *address,  /* address to operate on                */
    uint32_t max_nn,                        /* max. number of 32-bit words to write */
    generic_ind_t ind,                      /* indirect word                        */
    sync_t sync,                            /* sig_done or ctx_swap                 */
    SIGNAL *sig_ptr                         /* signal to raise upon completion      */
    )
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_write_reg(data));
        _MEM_IMPLEMENT_INDIRECT_COMMAND(write32_swap, 1, INTRINSIC_IND_ALU_COUNT_MAX);
    }
    _INTRINSIC_END;
}


__intrinsic
void cmd_mem_write32_swap_ind_ptr40(
    __xwrite void *data,                    /* data to write                        */
    volatile void __addr40 __mem *address,  /* address to operate on                */
    uint32_t max_nn,                        /* max. number of 32-bit words to write */
    generic_ind_t ind,                      /* indirect word                        */
    sync_t sync,                            /* sig_done or ctx_swap                 */
    SIGNAL *sig_ptr                         /* signal to raise upon completion      */
    )
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_write_reg(data));
        __asm local_csr_wr[CMD_INDIRECT_REF_0, CSR_INDIRECT_TYPE(ind)]
        _MEM_IMPLEMENT_INDIRECT_COMMAND_PTR40(write32_swap, 1, INTRINSIC_IND_ALU_COUNT_MAX);
    }
    _INTRINSIC_END;
}


/* Bulk 32 bit write with byte swap */
__intrinsic
void cmd_mem_write32_swap_le_ptr32(
    __xwrite void *data,                        /* data to write                        */
    volatile void __addr32 __mem *address,      /* address to operate on                */
    uint32_t count,                             /* number of 32-bit words to write      */
    sync_t sync,                                /* sig_done or ctx_swap                 */
    SIGNAL *sig_ptr                             /* signal to raise upon completion      */
    )
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_write_reg(data));
        _MEM_IMPLEMENT_COMMAND(write32_swap_le, cmd_mem_write32_swap_le_ptr32, 1, MAX_REF_COUNT_32);
    }
    _INTRINSIC_END;
}

__intrinsic
void cmd_mem_write32_swap_le_ptr40(
    __xwrite void *data,                        /* data to write                        */
    volatile void __addr40 __mem *address,      /* address to operate on                */
    uint32_t count,                             /* number of 32-bit words to write      */
    sync_t sync,                                /* sig_done or ctx_swap                 */
    SIGNAL *sig_ptr                             /* signal to raise upon completion      */
    )
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_write_reg(data));
        _MEM_IMPLEMENT_COMMAND_PTR40(write32_swap_le, cmd_mem_write32_swap_le_ptr40, 1, MAX_REF_COUNT_32);
    }
    _INTRINSIC_END;
}


#endif /* __NFP_MEM_C__ */


