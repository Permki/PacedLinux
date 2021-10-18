/*
 * Copyright (C) 2008-2014 Netronome Systems, Inc.  All rights reserved.
 */

#ifndef __NFP_CLUSTER_TARGET_C__
#define __NFP_CLUSTER_TARGET_C__

#include <nfp.h>
#include <nfp_intrinsic.h>
#include <nfp_cluster_target.h>

#ifndef NFP_LIB_ANY_NFCC_VERSION
    #if (!defined(__NFP_TOOL_NFCC) ||                       \
        (__NFP_TOOL_NFCC < NFP_SW_VERSION(5, 0, 0, 0)) ||   \
        (__NFP_TOOL_NFCC > NFP_SW_VERSION(6, 1, 0, 255)))
        #error "This standard library is not supported for the version of the SDK currently in use."
    #endif
#endif


/* Cluster operations */

#pragma diag_suppress 279  /* ignore while(0) etc in macros */

/*
* NOTE: When writing inline-asm, it's important not to use reserved words
*       for variables.  Common ones are 'a','b', 'csr', 'state', and
*       'inp_state'.
*/

#define _CLUSTER_TARGET_SIGNAL_CHECK(sig)                           \
        CT_ASSERT(__is_ct_const(sync));                             \
        CT_ASSERT(sync == sig_done || sync == ctx_swap);


__intrinsic
void cmd_cluster_target_xpb_write(
    __xwrite void *xfer,
    volatile cluster_target_xpb_address_format_t *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
)
{
    _INTRINSIC_BEGIN;
    {
        __gpr uint32_t address_val =  address->value;
        CT_ASSERT(__is_write_reg(xfer));
        _CLUSTER_TARGET_SIGNAL_CHECK(sync);

        if (sync == sig_done)
        {
            if (__is_ct_const(count) && count <= 8)
            {
                __asm ct[xpb_write, *xfer, address_val, 0, __ct_const_val(count)], sig_done[*sig_ptr]
            }
            else
            {
                // max 16 allowed for revision C0 and greater - see errata
                if (__is_nfp_arch_or_above(__nfp6000, __REVISION_C0))
                {
                    _INTRINSIC_IND_ALU_COUNT_MIN_MAX(count, 1, 16);
                }
                else
                {
                    _INTRINSIC_IND_ALU_COUNT_MIN_MAX(count, 1, 14);
                }

                __asm ct[xpb_write, *xfer, address_val, 0, __ct_const_val(count)], sig_done[*sig_ptr], indirect_ref
            }
        }
        else
        {
            if (__is_ct_const(count) && count <= 8)
            {
                __asm ct[xpb_write, *xfer, address_val, 0, __ct_const_val(count)], ctx_swap[*sig_ptr]
            }
            else
            {
                // max 16 allowed for revision C0 and greater - see errata
                if (__is_nfp_arch_or_above(__nfp6000, __REVISION_C0))
                {
                    _INTRINSIC_IND_ALU_COUNT_MIN_MAX(count, 1, 16);
                }
                else
                {
                    _INTRINSIC_IND_ALU_COUNT_MIN_MAX(count, 1, 14);
                }
                __asm ct[xpb_write, *xfer, address_val, 0, __ct_const_val(count)], ctx_swap[*sig_ptr], indirect_ref
            }
        }
    }
    _INTRINSIC_END;
}


__intrinsic
void cmd_cluster_target_xpb_read(
    __xread void *xfer,
    volatile cluster_target_xpb_address_format_t *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
)
{
    _INTRINSIC_BEGIN;
    {
        __gpr uint32_t address_val =  address->value;
        CT_ASSERT(__is_read_reg(xfer));

        _CLUSTER_TARGET_SIGNAL_CHECK(sync);

        if (sync == sig_done)
        {
            if (__is_ct_const(count) && count <= 8)
            {
                __asm ct[xpb_read, *xfer, address_val, 0, __ct_const_val(count)], sig_done[*sig_ptr]
            }
            else
            {
                _INTRINSIC_IND_ALU_COUNT_MIN_MAX(count, 1, 16);
                __asm ct[xpb_read, *xfer, address_val, 0, __ct_const_val(count)], sig_done[*sig_ptr], indirect_ref
            }
        }
        else
        {
            if (__is_ct_const(count) && count <= 8)
            {
                __asm ct[xpb_read, *xfer, address_val, 0, __ct_const_val(count)], ctx_swap[*sig_ptr]
            }
            else
            {
                _INTRINSIC_IND_ALU_COUNT_MIN_MAX(count, 1, 16);
                __asm ct[xpb_read, *xfer, address_val, 0, __ct_const_val(count)], ctx_swap[*sig_ptr], indirect_ref
            }
        }
    }
    _INTRINSIC_END;
}

/*
* Implement a reflect command with no signalling
* Override the length if more than 8.
* sig_done and ctx_swap are supported.
* Set is_read flag if read operation, then read_reg is checked otherwise write_reg.
* Verify the max ref count 1 to .. max_ref_count.
*/

#define _CT_REFLECT_COMMAND_NO_SIG(command, max_ref_count, is_read)                                 \
{                                                                                                   \
    __gpr uint32_t address_val =  address->value;                                               \
    if (is_read == 1) {                                                                             \
        CT_ASSERT(__is_read_reg(xfer));                                                             \
    }                                                                                               \
    else                                                                                            \
    {                                                                                               \
        CT_ASSERT(__is_write_reg(xfer));                                                            \
    }                                                                                               \
    if (__is_ct_const(count) && count <= 8)                                                         \
    {                                                                                               \
        __asm ct[command, *xfer, address_val , 0, __ct_const_val(count)]                            \
    }                                                                                               \
    else                                                                                            \
    {                                                                                               \
        _INTRINSIC_IND_ALU_COUNT_MIN_MAX(count, 1, max_ref_count);                                  \
        __asm ct[command, *xfer, address_val , 0, __ct_const_val(count)], indirect_ref              \
    }                                                                                               \
}


__intrinsic
void cmd_cluster_target_reflect_write_sig_none(
    __xwrite void *xfer,
    volatile cluster_target_reflect_address_format_t *address,
    uint32_t count
)
{
    _INTRINSIC_BEGIN;
    {

        // max 16 allowed for revision C0 and greater - see errata
        if (__is_nfp_arch_or_above(__nfp6000, __REVISION_C0))
        {
            _CT_REFLECT_COMMAND_NO_SIG(reflect_write_none, 16, 0);
        }
        else
        {
            _CT_REFLECT_COMMAND_NO_SIG(reflect_write_none, 14, 0);
        }
    }
    _INTRINSIC_END;
}

__intrinsic
void cmd_cluster_target_reflect_read_sig_none(
    __xread void *xfer,
    volatile cluster_target_reflect_address_format_t *address,
    uint32_t count
)
{
    _INTRINSIC_BEGIN;
    {
        // max 16 allowed for revision C0 and greater - see errata
        if (__is_nfp_arch_or_above(__nfp6000, __REVISION_C0))
        {
            _CT_REFLECT_COMMAND_NO_SIG(reflect_read_none, 16, 1);
        }
        else
        {
            _CT_REFLECT_COMMAND_NO_SIG(reflect_read_none, 14, 1);
        }
    }
    _INTRINSIC_END;
}

/*
* Implement a reflect command with signalling
* Override the length if more than 8.
* sig_done and ctx_swap are supported.
* Set is_read flag if read operation, then read_reg is checked otherwise write_reg.
* Verify the max ref count 1 to .. max_ref_count.
*/

#define _CT_REFLECT_COMMAND_WITH_SIG(command, max_ref_count, is_read)                                                       \
{                                                                                                                           \
    __gpr  uint32_t address_val =  address->value;                                                                      \
    if (is_read == 1) {                                                                                                     \
        CT_ASSERT(__is_read_reg(xfer));                                                                                     \
    }                                                                                                                       \
    else                                                                                                                    \
    {                                                                                                                       \
        CT_ASSERT(__is_write_reg(xfer));                                                                                    \
    }                                                                                                                       \
                                                                                                                            \
    _CLUSTER_TARGET_SIGNAL_CHECK(sync);                                                                                     \
    if (sync == sig_done)                                                                                                   \
    {                                                                                                                       \
        if (__is_ct_const(count) && count <= 8)                                                                             \
        {                                                                                                                   \
            __asm ct[command, *xfer, address_val, 0, __ct_const_val(count)], sig_done[*sig_ptr]                             \
        }                                                                                                                   \
        else                                                                                                                \
        {                                                                                                                   \
            _INTRINSIC_IND_ALU_COUNT_MIN_MAX(count, 1, max_ref_count);                                                      \
            __asm ct[command, *xfer, address_val, 0, __ct_const_val(count)], sig_done[*sig_ptr], indirect_ref               \
        }                                                                                                                   \
    }                                                                                                                       \
    else                                                                                                                    \
    {                                                                                                                       \
        if (__is_ct_const(count) && count <= 8)                                                                             \
        {                                                                                                                   \
            __asm ct[command, *xfer, address_val, 0, __ct_const_val(count)], ctx_swap[*sig_ptr]                             \
        }                                                                                                                   \
        else                                                                                                                \
        {                                                                                                                   \
            _INTRINSIC_IND_ALU_COUNT_MIN_MAX(count, 1, max_ref_count);                                                      \
            __asm ct[command, *xfer, address_val, 0, __ct_const_val(count)], ctx_swap[*sig_ptr], indirect_ref               \
        }                                                                                                                   \
    }                                                                                                                       \
}


#define _CT_REFLECT_COMMAND_WITH_REMOTE_SIG(command, max_ref_count, is_read)                                                \
{                                                                                                                           \
    __gpr  uint32_t address_val =  address->value;                                                                      \
    if (is_read == 1) {                                                                                                     \
        CT_ASSERT(__is_read_reg(xfer));                                                                                     \
    }                                                                                                                       \
    else                                                                                                                    \
    {                                                                                                                       \
        CT_ASSERT(__is_write_reg(xfer));                                                                                    \
    }                                                                                                                       \
                                                                                                                            \
    CT_ASSERT(__is_ct_const(sync));                                                                                         \
    CT_ASSERT(sync == sig_done);                                                                                            \
    if (sync == sig_done)                                                                                                   \
    {                                                                                                                       \
        if (__is_ct_const(count) && count <= 8)                                                                             \
        {                                                                                                                   \
            __asm ct[command, *xfer, address_val, 0, __ct_const_val(count)], sig_done[*sig_ptr]                             \
        }                                                                                                                   \
        else                                                                                                                \
        {                                                                                                                   \
            _INTRINSIC_IND_ALU_COUNT_MIN_MAX(count, 1, max_ref_count);                                                      \
            __asm ct[command, *xfer, address_val, 0, __ct_const_val(count)], sig_done[*sig_ptr], indirect_ref               \
        }                                                                                                                   \
    }                                                                                                                       \
}

__intrinsic
void cmd_cluster_target_reflect_write_sig_both(
    __xwrite void *xfer,
    volatile cluster_target_reflect_address_format_t *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
)
{
    _INTRINSIC_BEGIN;
    {
        // max 16 allowed for revision C0 and greater - see errata
        if (__is_nfp_arch_or_above(__nfp6000, __REVISION_C0))
        {
            _CT_REFLECT_COMMAND_WITH_SIG(reflect_write_sig_both, 16, 0);
        }
        else
        {
            _CT_REFLECT_COMMAND_WITH_SIG(reflect_write_sig_both, 14, 0);
        }
    }
    _INTRINSIC_END;
}


__intrinsic
void cmd_cluster_target_reflect_read_sig_both(
    __xread void *xfer,
    volatile cluster_target_reflect_address_format_t *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
)
{
    _INTRINSIC_BEGIN;
    {
        // max 16 allowed for revision C0 and greater - see errata
        if (__is_nfp_arch_or_above(__nfp6000, __REVISION_C0))
        {
            _CT_REFLECT_COMMAND_WITH_SIG(reflect_read_sig_both, 16, 1);
        }
        else
        {
            _CT_REFLECT_COMMAND_WITH_SIG(reflect_read_sig_both, 14, 1);
        }
    }
    _INTRINSIC_END;
}

__intrinsic
void cmd_cluster_target_reflect_write_sig_init(
    __xwrite void *xfer,
    volatile cluster_target_reflect_address_format_t *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
)
{
    _INTRINSIC_BEGIN;
    {
        // max 16 allowed for revision C0 and greater - see errata
        if (__is_nfp_arch_or_above(__nfp6000, __REVISION_C0))
        {
            _CT_REFLECT_COMMAND_WITH_SIG(reflect_write_sig_init, 16, 0);
        }
        else
        {
            _CT_REFLECT_COMMAND_WITH_SIG(reflect_write_sig_init, 14, 0);
        }
    }
    _INTRINSIC_END;
}


__intrinsic
void cmd_cluster_target_reflect_read_sig_init(
    __xread void *xfer,
    volatile cluster_target_reflect_address_format_t *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
)
{
    _INTRINSIC_BEGIN;
    {
        // max 16 allowed for revision C0 and greater - see errata
        if (__is_nfp_arch_or_above(__nfp6000, __REVISION_C0))
        {
            _CT_REFLECT_COMMAND_WITH_SIG(reflect_read_sig_init, 16, 1);
        }
        else
        {
            _CT_REFLECT_COMMAND_WITH_SIG(reflect_read_sig_init, 14, 1);
        }
    }
    _INTRINSIC_END;
}

__intrinsic
void cmd_cluster_target_reflect_write_sig_remote(
    __xwrite void *xfer,
    volatile cluster_target_reflect_address_format_t *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
)
{
    _INTRINSIC_BEGIN;
    {
        // max 16 allowed for revision C0 and greater - see errata
        if (__is_nfp_arch_or_above(__nfp6000, __REVISION_C0))
        {
            _CT_REFLECT_COMMAND_WITH_REMOTE_SIG(reflect_write_sig_remote, 16, 0);
        }
        else
        {
            _CT_REFLECT_COMMAND_WITH_REMOTE_SIG(reflect_write_sig_remote, 14, 0);
        }
    }
    _INTRINSIC_END;
}


__intrinsic
void cmd_cluster_target_reflect_read_sig_remote(
    __xread void *xfer,
    volatile cluster_target_reflect_address_format_t *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
)
{
    _INTRINSIC_BEGIN;
    {
        // max 16 allowed for revision C0 and greater - see errata
        if (__is_nfp_arch_or_above(__nfp6000, __REVISION_C0))
        {
            _CT_REFLECT_COMMAND_WITH_REMOTE_SIG(reflect_read_sig_remote, 16, 1);
        }
        else
        {
            _CT_REFLECT_COMMAND_WITH_REMOTE_SIG(reflect_read_sig_remote, 14, 1);
        }
    }
    _INTRINSIC_END;
}


__intrinsic
void cmd_cluster_target_sig_me_ctx(
    volatile cluster_target_signal_me_address_format_t *address
)
{
    _INTRINSIC_BEGIN;
    {
        __gpr uint32_t address_val =  address->value;

        __asm
        {
            ct[interthread_signal, --, address_val, 0, 1]
        }
    }
    _INTRINSIC_END;
}

__intrinsic
void cmd_cluster_target_next_neighbour_write(
    __xwrite void *xfer,
    volatile cluster_target_next_neighbour_write_address_format_t *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
)
{
    _INTRINSIC_BEGIN;
    {
        __gpr uint32_t address_val =  address->value;
        generic_ind_t ind;

        CT_ASSERT(__is_write_reg(xfer));
        _CLUSTER_TARGET_SIGNAL_CHECK(sync);

        if (__is_ct_const(count) && count <= 8)
        {
            CT_ASSERT(count != 0);
            if (sync == sig_done)
            {
                __asm
                {
                    ct[ctnn_write, *xfer, address_val, 0, count], sig_done[*sig_ptr]
                }
            }
            else
            {
                __asm
                {
                    ct[ctnn_write, *xfer, address_val, 0, count], ctx_swap[*sig_ptr]
                }
            }
        }
        else
        {
            // max 16 allowed for revision C0 and greater - see errata
            if (__is_nfp_arch_or_above(__nfp6000, __REVISION_C0))
            {
                _INTRINSIC_IND_ALU_COUNT_MIN_MAX(count, 1, 16);
            }
            else
            {
                _INTRINSIC_IND_ALU_COUNT_MIN_MAX(count, 1, 14);
            }

            if (sync == sig_done)
            {
                __asm
                {
                    ct[ctnn_write, *xfer, address_val, 0, __ct_const_val(count)], sig_done[*sig_ptr], indirect_ref
                }
            }
            else
            {
                __asm
                {
                    ct[ctnn_write, *xfer, address_val, 0, __ct_const_val(count)], ctx_swap[*sig_ptr], indirect_ref
                }
            }
        }
    }
    _INTRINSIC_END;
}


__intrinsic
void cmd_cluster_target_next_neighbour_write_ind(
    __xwrite void *xfer,
    volatile cluster_target_next_neighbour_write_address_format_t *address,
    uint32_t max_nn,
    generic_ind_t ind,
    sync_t sync,
    SIGNAL *sig_ptr
)
{
    _INTRINSIC_BEGIN;
    {
        __gpr uint32_t address_val =  address->value;

        CT_ASSERT(__is_write_reg(xfer));
        _CLUSTER_TARGET_SIGNAL_CHECK(sync);
        CT_ASSERT(__is_ct_const(max_nn));

        // max 16 allowed for revision C0 and greater - see errata
        if (__is_nfp_arch_or_above(__nfp6000, __REVISION_C0))
        {
            CT_ASSERT(max_nn >= 1 && max_nn <= 16);
        }
        else
        {
            CT_ASSERT(max_nn >= 1 && max_nn <= 14);
        }

        if (sync == sig_done)
        {
            __asm local_csr_wr[CMD_INDIRECT_REF_0, CSR_INDIRECT_TYPE(ind)]
            __asm alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
            __asm ct[ctnn_write, *xfer, address_val, 0, __ct_const_val(max_nn)], sig_done[*sig_ptr], indirect_ref
        }
        else
        {
            __asm local_csr_wr[CMD_INDIRECT_REF_0, CSR_INDIRECT_TYPE(ind)]
            __asm alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
            __asm ct[ctnn_write, *xfer, address_val, 0, __ct_const_val(max_nn)], ctx_swap[*sig_ptr], indirect_ref
        }
    }
    _INTRINSIC_END;
}

#pragma diag_default 279  /* suppressed above */

#endif /* __NFP_CLUSTER_TARGET_C__ */
