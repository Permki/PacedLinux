/*
 * Copyright (C) 2008-2014 Netronome Systems, Inc.  All rights reserved.
 */

#ifndef __NFP_MEM_LOOKUP_ENGINE_C__
#define __NFP_MEM_LOOKUP_ENGINE_C__

#include <nfp.h>
#include <nfp_intrinsic.h>
#include <nfp_mem_lookup_engine.h>

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

#define _MEM_LOOKUP_SIG_DONE_CHECK(sig)                                 \
        if (__is_ct_const(sig))                                         \
        {                                                               \
            CT_ASSERT(sig == sig_done);                                 \
        }

#define _MEM_LOOKUP_SIGNAL_PAIR_CHECK(sig_pair)                         \
        if (__is_ct_const(sig_pair))                                    \
        {                                                               \
            CT_ASSERT(sig_pair == sig_done);                            \
        }




__intrinsic
mem_lookup_result_in_read_reg_t *cmd_mem_lookup(
    __xwrite void *xfer,
    volatile void __addr40 __mem *address,
    uint32_t count,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
)
{
    _INTRINSIC_BEGIN;
    {
        uint32_t hi_addr, low_addr;

        _INTRINSIC_CONVERT_HI_LO_ADDRESS(address);
        _MEM_LOOKUP_SIGNAL_PAIR_CHECK(sync);
        CT_ASSERT(__is_write_reg(xfer));

        if (__is_ct_const(count) && count <= 2)
        {
            CT_ASSERT(count != 0);
            if (sync == sig_done)
            {
                __asm mem[lookup, *xfer, hi_addr, << 8, low_addr, __ct_const_val(count)], sig_done[*sig_pair_ptr]
            }
        }
        else
        {
            if (!__is_ct_const(count))
            {
                CT_QPERFINFO_INDIRECT_REF(cmd_mem_lookup);
            }
            if (sync == sig_done)
            {
                __asm mem[lookup, *xfer, hi_addr, << 8, low_addr,__ct_const_val(count)], sig_done[*sig_pair_ptr]
            }
        }
        return (mem_lookup_result_in_read_reg_t *)(xfer);
    }
    _INTRINSIC_END;
}

#endif /* __NFP_MEM_LOOKUP_C__ */
