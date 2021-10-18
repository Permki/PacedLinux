/*
 * Copyright (C) 2008-2014 Netronome Systems, Inc.  All rights reserved.
 */

#ifndef __NFP_ILA_C__
#define __NFP_ILA_C__

#include <nfp.h>
#include <nfp_intrinsic.h>
#include <nfp_ila.h>
#include <nfp_override.h>

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
 *       for variables.  Common ones are 'a','b', 'csr', 'state', and
 *       'inp_state'.
 */
#define _ILA_SEARCH_KEY_LENGTH_CHECK(length)                            \
        if (__is_ct_const(length))                                      \
        {                                                               \
            CT_ASSERT(length <= 3);                                     \
        }                                                               \
        else                                                            \
        {                                                               \
            RT_RANGE_ASSERT(length <= 3);                               \
        }


#define _ILA_IMPLEMENT_COMMAND_PTR40(command, max_ref_count, is_read)                                                           \
{                                                                                                                               \
    if (is_read == 1) {                                                                                                         \
        CT_ASSERT(__is_read_reg(data));                                                                                         \
    }                                                                                                                           \
    else                                                                                                                        \
    {                                                                                                                           \
        CT_ASSERT(__is_write_reg(data));                                                                                        \
    }                                                                                                                           \
    {                                                                                                                           \
       uint32_t hi_addr, low_addr;                                                                                          \
       _INTRINSIC_CONVERT_HI_LO_ADDRESS(address);                                                                               \
       CT_ASSERT(__is_ct_const(sync));                                                                                          \
        CT_ASSERT(sync == sig_done || sync == ctx_swap);                                                                        \
        if (__is_ct_const(count) && count <= 8)                                                                                 \
        {                                                                                                                       \
            CT_ASSERT(count != 0);                                                                                              \
            if (sync == sig_done)                                                                                               \
            {                                                                                                                   \
                __asm ila[command, *data, hi_addr, <<8, low_addr, __ct_const_val(count)], sig_done[*sig_ptr]                    \
            }                                                                                                                   \
            else                                                                                                                \
            {                                                                                                                   \
                __asm ila[command, *data, hi_addr, <<8, low_addr, __ct_const_val(count)], ctx_swap[*sig_ptr]                    \
            }                                                                                                                   \
        }                                                                                                                       \
        else                                                                                                                    \
        {                                                                                                                       \
            if (!__is_ct_const(count))                                                                                          \
            {                                                                                                                   \
                CT_QPERFINFO_INDIRECT_REF(command);                                                                             \
            }                                                                                                                   \
            {                                                                                                                   \
                _INTRINSIC_IND_ALU_COUNT_MIN_MAX(count, 1,  max_ref_count);                                                     \
                if (sync == sig_done)                                                                                           \
                {                                                                                                               \
                    __asm ila[command, *data, hi_addr, <<8, low_addr, __ct_const_val(count)], sig_done[*sig_ptr], indirect_ref  \
                }                                                                                                               \
                else                                                                                                            \
                {                                                                                                               \
                    __asm ila[command, *data, hi_addr, <<8, low_addr, __ct_const_val(count)], ctx_swap[*sig_ptr], indirect_ref  \
                }                                                                                                               \
            }                                                                                                                   \
        }                                                                                                                       \
    }                                                                                                                           \
}

 __intrinsic
void cmd_ila_write_ptr40(
    __xwrite void *data,
    volatile void __addr40 __mem *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
)
{
    _INTRINSIC_BEGIN;
    {
        __ct_assert( !( __is_nfp_arch(__nfp3800, __REVISION_A0)),"cmd_ila_write_ptr40 is not supported with the selected chip");
        _ILA_IMPLEMENT_COMMAND_PTR40(write, 16, 0);
    }
    _INTRINSIC_END;
}


 __intrinsic
void cmd_ila_read_ptr40(
    __xread void *data,
    volatile void __addr40 __mem *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
)
{
    _INTRINSIC_BEGIN;
    {
        __ct_assert( !( __is_nfp_arch(__nfp3800, __REVISION_A0)),"cmd_ila_read_ptr40 is not supported with the selected chip");
        _ILA_IMPLEMENT_COMMAND_PTR40(read, 16, 1);
    }
    _INTRINSIC_END;
}

 __intrinsic
void cmd_ila_write_internal_ptr40(
    __xwrite void *data,
    volatile void __addr40 __mem *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
)
{
    _INTRINSIC_BEGIN;
    {
        __ct_assert( !( __is_nfp_arch(__nfp3800, __REVISION_A0)),"cmd_ila_write_internal_ptr40 is not supported with the selected chip");
        _ILA_IMPLEMENT_COMMAND_PTR40(write_int, 16, 0);
    }
    _INTRINSIC_END;
}


 __intrinsic
void cmd_ila_read_internal_ptr40(
    __xread void *data,
    volatile void __addr40 __mem *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
)
{
    _INTRINSIC_BEGIN;
    {
        __ct_assert( !( __is_nfp_arch(__nfp3800, __REVISION_A0)),"cmd_ila_read_internal_ptr40 is not supported with the selected chip");
        _ILA_IMPLEMENT_COMMAND_PTR40(read_int, 16, 1);
    }
    _INTRINSIC_END;
}


#define _ILA_IMPLEMENT_COMMAND_SIG_PAIR_PTR40(command, max_ref_count, is_read)                                                  \
{                                                                                                                               \
    if (is_read == 1) {                                                                                                         \
        CT_ASSERT(__is_read_reg(data));                                                                                         \
    }                                                                                                                           \
    else                                                                                                                        \
    {                                                                                                                           \
        CT_ASSERT(__is_write_reg(data));                                                                                        \
    }                                                                                                                           \
    {                                                                                                                           \
       uint32_t hi_addr, low_addr;                                                                                          \
       _INTRINSIC_CONVERT_HI_LO_ADDRESS(address);                                                                               \
       CT_ASSERT(__is_ct_const(sync));                                                                                          \
        CT_ASSERT(sync == sig_done);                                                                                            \
        if (__is_ct_const(count) && count <= 8)                                                                                 \
        {                                                                                                                       \
            CT_ASSERT(count != 0);                                                                                              \
            if (sync == sig_done)                                                                                               \
            {                                                                                                                   \
                __asm ila[command, *data, hi_addr, <<8, low_addr, __ct_const_val(count)],  sig_done[*sig_pair_ptr]              \
            }                                                                                                                   \
        }                                                                                                                       \
        else                                                                                                                    \
        {                                                                                                                       \
            if (!__is_ct_const(count))                                                                                          \
            {                                                                                                                   \
                CT_QPERFINFO_INDIRECT_REF(command);                                                                             \
            }                                                                                                                   \
            {                                                                                                                   \
                _INTRINSIC_IND_ALU_COUNT_MIN_MAX(count, 1,  max_ref_count);                                                     \
                if (sync == sig_done)                                                                                           \
                {                                                                                                               \
                    __asm ila[command, *data, hi_addr, <<8, low_addr, __ct_const_val(count)], sig_done[*sig_pair_ptr], indirect_ref  \
                }                                                                                                               \
            }                                                                                                                   \
        }                                                                                                                       \
    }                                                                                                                           \
}

__intrinsic
void cmd_ila_write_check_error_ptr40(
    __xwrite void *data,
    volatile void __addr40 __mem *address,
    uint32_t count,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
)
{
    _INTRINSIC_BEGIN;
    {
        __ct_assert( !( __is_nfp_arch(__nfp3800, __REVISION_A0)),"cmd_ila_write_check_error_ptr40 is not supported with the selected chip");
        _ILA_IMPLEMENT_COMMAND_SIG_PAIR_PTR40(write_check_error, 32, 0);
    }
    _INTRINSIC_END;
}

__intrinsic
void cmd_ila_read_check_error_ptr40(
    __xread void *data,
    volatile void __addr40 __mem *address,
    uint32_t count,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
)
{
    _INTRINSIC_BEGIN;
    {
        __ct_assert( !( __is_nfp_arch(__nfp3800, __REVISION_A0)),"cmd_ila_read_check_error_ptr40 is not supported with the selected chip");
        _ILA_IMPLEMENT_COMMAND_SIG_PAIR_PTR40(read_check_error, 32, 1);
    }
    _INTRINSIC_END;
}

#pragma diag_default 279  /* suppressed above */

#endif
