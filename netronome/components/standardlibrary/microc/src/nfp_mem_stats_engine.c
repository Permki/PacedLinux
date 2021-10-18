/*
 * Copyright (C) 2008-2014 Netronome Systems, Inc.  All rights reserved.
 */

#ifndef __NFP_MEM_STATS_ENGINE_C__
#define __NFP_MEM_STATS_ENGINE_C__

#include <nfp.h>
#include <nfp_intrinsic.h>
#include <nfp_mem_stats_engine.h>
#include <nfp_override.h>

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

#define _MEM_STATS_SIGNAL_CHECK(sig)                                    \
        if (__is_ct_const(sig))                                         \
        {                                                               \
            CT_ASSERT(sig == sig_done || sig == ctx_swap);              \
        }


#define _MEM_STATS_READ_IMPLEMENT_COMMAND(command)                                                              \
{                                                                                                               \
    uint32_t hi_addr, low_addr;                                                                                 \
    CT_ASSERT(__is_read_reg(xfer));                                                                             \
    _MEM_STATS_SIGNAL_CHECK(sync);                                                                              \
    _INTRINSIC_CONVERT_HI_LO_ADDRESS(address);                                                                  \
    if (sync == sig_done)                                                                                       \
    {                                                                                                           \
        if (__is_ct_const(count) && count <= 8)                                                                 \
        {                                                                                                       \
            __asm mem[command, *xfer, hi_addr, << 8, low_addr, __ct_const_val(count)], sig_done[*sig_ptr]       \
        }                                                                                                       \
        else                                                                                                    \
        {                                                                                                       \
            _INTRINSIC_IND_ALU_COUNT_MIN_MAX(count, 1, 16);                                                     \
            __asm mem[command, *xfer, hi_addr, << 8, low_addr, __ct_const_val(count)], sig_done[*sig_ptr], indirect_ref \
        }                                                                                                       \
    }                                                                                                           \
    else                                                                                                        \
    {                                                                                                           \
        if (__is_ct_const(count) && count <= 8)                                                                 \
        {                                                                                                       \
            __asm mem[command, *xfer, hi_addr, << 8, low_addr, __ct_const_val(count)], ctx_swap[*sig_ptr]       \
        }                                                                                                       \
        else                                                                                                    \
        {                                                                                                       \
            _INTRINSIC_IND_ALU_COUNT_MIN_MAX(count, 1, 16);                                                     \
            __asm mem[command, *xfer, hi_addr, << 8, low_addr, __ct_const_val(count)], ctx_swap[*sig_ptr], indirect_ref \
        }                                                                                                       \
    }                                                                                                           \
}

__intrinsic
void cmd_mem_stats_read(
    __xread void *xfer,
    volatile void __addr40 __mem *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
)
{
    _INTRINSIC_BEGIN;
    {
        _MEM_STATS_READ_IMPLEMENT_COMMAND(stats_push);
    }
    _INTRINSIC_END;
}


__intrinsic
void cmd_mem_stats_read_and_clear(
    __xread void *xfer,
    volatile void __addr40 __mem *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
)
{
    _INTRINSIC_BEGIN;
    {
        _MEM_STATS_READ_IMPLEMENT_COMMAND(stats_push_clear);
    }
    _INTRINSIC_END;
}

#define _MEM_STATS_LOG_IMPLEMENT_COMMAND(command)                                                                       \
{                                                                                                                       \
    uint32_t hi_addr, low_addr;                                                                                         \
    CT_ASSERT(__is_write_reg(xfer));                                                                                    \
    _MEM_STATS_SIGNAL_CHECK(sync);                                                                                      \
    _INTRINSIC_CONVERT_HI_LO_ADDRESS(data);                                                                             \
    if (sync == sig_done)                                                                                               \
    {                                                                                                                   \
        if (__is_ct_const(count) && count <= 8)                                                                         \
        {                                                                                                               \
            __asm mem[command, *xfer, hi_addr, << 8, low_addr, __ct_const_val(count)], sig_done[*sig_ptr]               \
        }                                                                                                               \
        else                                                                                                            \
        {                                                                                                               \
            _INTRINSIC_IND_ALU_COUNT_MIN_MAX(count, 1, 16);                                                             \
            __asm mem[command, *xfer, hi_addr, << 8, low_addr, __ct_const_val(count)], sig_done[*sig_ptr], indirect_ref \
        }                                                                                                               \
    }                                                                                                                   \
    else                                                                                                                \
    {                                                                                                                   \
        if (__is_ct_const(count) && count <= 8)                                                                         \
        {                                                                                                               \
            __asm mem[command, *xfer, hi_addr, << 8, low_addr, __ct_const_val(count)], ctx_swap[*sig_ptr]               \
        }                                                                                                               \
        else                                                                                                            \
        {                                                                                                               \
            _INTRINSIC_IND_ALU_COUNT_MIN_MAX(count, 1, 16);                                                             \
            __asm mem[command, *xfer, hi_addr, << 8, low_addr, __ct_const_val(count)], ctx_swap[*sig_ptr], indirect_ref \
        }                                                                                                               \
     }                                                                                                                  \
}


__intrinsic
void cmd_mem_stats_log(
    __xwrite void *xfer,
    volatile void __addr40 __mem *data,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
)
{
    _INTRINSIC_BEGIN;
    {
        _MEM_STATS_LOG_IMPLEMENT_COMMAND(stats_log);
    }
    _INTRINSIC_END;
}

__intrinsic
void cmd_mem_stats_log_saturate(
    __xwrite void *xfer,
    volatile void __addr40 __mem *data,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
)
{
    _INTRINSIC_BEGIN;
    {
        _MEM_STATS_LOG_IMPLEMENT_COMMAND(stats_log_sat);
    }
    _INTRINSIC_END;
}

__intrinsic
void cmd_mem_stats_log_event(
    __xwrite void *xfer,
    volatile void __addr40 __mem *data,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
)
{
    _INTRINSIC_BEGIN;
    {
        _MEM_STATS_LOG_IMPLEMENT_COMMAND(stats_log_event);
    }
    _INTRINSIC_END;
}

__intrinsic
void cmd_mem_stats_log_event_saturate(
    __xwrite void *xfer,
    volatile void __addr40 __mem *data,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
)
{
    _INTRINSIC_BEGIN;
    {
        _MEM_STATS_LOG_IMPLEMENT_COMMAND(stats_log_sat_event);
    }
    _INTRINSIC_END;
}

#endif /* __NFP_MEM_STATS_ENGINE_C__ */
