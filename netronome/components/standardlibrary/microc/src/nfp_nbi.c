/*
 * Copyright (C) 2008-2014 Netronome Systems, Inc.  All rights reserved.
 */

#ifndef __NFP_NBI_C__
#define __NFP_NBI_C__

#include <nfp.h>
#include <nfp_intrinsic.h>
#include <nfp_nbi.h>
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

#pragma diag_suppress 279  /* ignore while(0) etc in macros */

#define _NBI_IMPLEMENT_COMMAND(command, max_ref_count, is_read)                                                                 \
{                                                                                                                               \
    if (is_read == 1) {                                                                                                         \
        CT_ASSERT(__is_read_reg(data));                                                                                         \
    }                                                                                                                           \
    else                                                                                                                        \
    {                                                                                                                           \
        CT_ASSERT(__is_write_reg(data));                                                                                        \
    }                                                                                                                           \
    {                                                                                                                           \
       uint32_t hi_addr, low_addr;                                                                                              \
       _INTRINSIC_CONVERT_HI_LO_ADDRESS(address);                                                                               \
       CT_ASSERT(__is_ct_const(sync));                                                                                          \
        CT_ASSERT(sync == sig_done || sync == ctx_swap);                                                                        \
        if (__is_ct_const(count) && count <= 8)                                                                                 \
        {                                                                                                                       \
            CT_ASSERT(count != 0);                                                                                              \
            if (sync == sig_done)                                                                                               \
            {                                                                                                                   \
                __asm nbi[command, *data, hi_addr, <<8, low_addr, __ct_const_val(count)], sig_done[*sig_ptr]                    \
            }                                                                                                                   \
            else                                                                                                                \
            {                                                                                                                   \
                __asm nbi[command, *data, hi_addr, <<8, low_addr, __ct_const_val(count)], ctx_swap[*sig_ptr]                    \
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
                    __asm nbi[command, *data, hi_addr, <<8, low_addr, __ct_const_val(count)], sig_done[*sig_ptr], indirect_ref  \
                }                                                                                                               \
                else                                                                                                            \
                {                                                                                                               \
                    __asm nbi[command, *data, hi_addr, <<8, low_addr, __ct_const_val(count)], ctx_swap[*sig_ptr], indirect_ref  \
                }                                                                                                               \
            }                                                                                                                   \
        }                                                                                                                       \
    }                                                                                                                           \
}


 __intrinsic
void cmd_nbi_write(
    __xwrite void *data,
    volatile void __addr40 __mem *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
)
{
    _INTRINSIC_BEGIN;
    {
        _NBI_IMPLEMENT_COMMAND(write, 16, 0);
    }
    _INTRINSIC_END;
}


 __intrinsic
void cmd_nbi_read(
    __xread void *data,
    volatile void __addr40 __mem *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
)
{
    _INTRINSIC_BEGIN;
    {
        _NBI_IMPLEMENT_COMMAND(read, 16, 1);
    }
    _INTRINSIC_END;
}

#pragma diag_default 279  /* suppressed above */

#endif /* __NFP_NBI_C__ */
