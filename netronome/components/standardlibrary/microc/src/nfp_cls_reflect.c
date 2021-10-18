/*
 * Copyright (C) 2008-2014 Netronome Systems, Inc.  All rights reserved.
 */

#ifndef __NFP_CLS_REFLECT_C__
#define __NFP_CLS_REFLECT_C__

#include <nfp.h>
#include <nfp_intrinsic.h>
#include <nfp_cls_reflect.h>


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


#pragma diag_suppress 279  /* ignore while(0) etc in macros */

// 4 bits allowed for ME
#define _CLS_REFLECT_ME_RANGE_CHECK()           \
    if (__is_ct_const(remote_ME))               \
    {                                           \
        CT_ASSERT((remote_ME & (~0xf))==0);     \
    }

// 6 bits allowed for island number
#define _CLS_REFLECT_ISLAND_RANGE_CHECK()       \
    if(__is_ct_const(remote_island))            \
    {                                           \
        CT_ASSERT((remote_island&(~0x3f))==0);  \
    }
/*
 * Implement the CLS 32-bit pointer reflect command with sig_done or ctx swap
 * Address encoding formula for cls reflect
 *          bits[11:0]  = xfer register address (reg number)
 *          bits[15:12] = FPC/ME
 *          bits[23:16] = 8b0
 *          bits[27:24] = Signal number
 *          bits[30:28] = ctx
 *          bit[31]     = Signal reference (If set, signal number comes from bits [27:24]
 */

#define _CLS_IMPLEMENT_REFLECT_COMMAND(command, is_read)                                                                \
    if (is_read == 1) {                                                                                                 \
        CT_ASSERT(__is_read_reg(data));                                                                                 \
    }                                                                                                                   \
    else                                                                                                                \
    {                                                                                                                   \
        CT_ASSERT(__is_write_reg(data));                                                                                \
    }                                                                                                                   \
    CT_ASSERT(__is_ct_const(sync));                                                                                     \
    CT_ASSERT(sync == sig_done || sync == ctx_swap);                                                                    \
    _CLS_REFLECT_ME_RANGE_CHECK()                                                                                       \
                                                                                                                        \
    {                                                                                                                   \
        uint32_t address = ((remote_ME & 0xf) << 12) | (remote_xfer_reg_number & 0xfff);                            \
        if (__is_ct_const(count) && count<=8)                                                                           \
        {                                                                                                               \
            CT_ASSERT(count!=0);                                                                                        \
            if (sync == sig_done)                                                                                       \
            {                                                                                                           \
                __asm cls[command, *data, address, 0, __ct_const_val(count)], sig_done[*sig_ptr]                        \
            }                                                                                                           \
            else                                                                                                        \
            {                                                                                                           \
                __asm cls[command, *data, address, 0, __ct_const_val(count)], ctx_swap[*sig_ptr]                        \
            }                                                                                                           \
        }                                                                                                               \
        else                                                                                                            \
        {                                                                                                               \
            if (__is_ct_const(count))                                                                                   \
            {                                                                                                           \
                CT_ASSERT(count<=16);                                                                                   \
            }                                                                                                           \
            else                                                                                                        \
            {                                                                                                           \
                CT_QPERFINFO_INDIRECT_REF(command);                                                                     \
            }                                                                                                           \
            {                                                                                                           \
                generic_ind_t ind;                                                                                      \
                _INTRINSIC_OVERRIDE_LENGTH(ind, (count - 1));                                                           \
                if (sync == sig_done)                                                                                   \
                {                                                                                                       \
                    __asm alu[--, --, B, ALU_INDIRECT_TYPE(ind)]                                                        \
                    __asm cls[command, *data, address, 0, __ct_const_val(count)], sig_done[*sig_ptr], indirect_ref      \
                }                                                                                                       \
                else                                                                                                    \
                {                                                                                                       \
                    __asm  alu[--, --, B, ALU_INDIRECT_TYPE(ind)]                                                       \
                    __asm cls[command, *data, address, 0, __ct_const_val(count)], ctx_swap[*sig_ptr], indirect_ref      \
                }                                                                                                       \
            }                                                                                                           \
        }                                                                                                               \
    }


/*
 * Implement the CLS 40-bit pointer reflect command with sig_done or ctx swap
 * Address encoding formula for cls reflect
 *          bits[11:0]  = xfer register address (reg number)
 *          bits[15:12] = FPC/ME
 *          bits[23:16] = 8b0
 *          bits[27:24] = Signal number
 *          bits[30:28] = ctx
 *          bit[31]     = Signal reference (If set, signal number comes from bits [27:24]
 *          bit[33:32]  = 2b0
 *          bit[39:34]  = island number
 */
#define _CLS_IMPLEMENT_REFLECT_COMMAND_PTR40(command, is_read)                                                          \
    if (is_read == 1) {                                                                                                 \
        CT_ASSERT(__is_read_reg(data));                                                                                 \
    }                                                                                                                   \
    else                                                                                                                \
    {                                                                                                                   \
        CT_ASSERT(__is_write_reg(data));                                                                                \
    }                                                                                                                   \
    CT_ASSERT(__is_ct_const(sync));                                                                                     \
    CT_ASSERT(sync == sig_done || sync == ctx_swap);                                                                    \
    _CLS_REFLECT_ME_RANGE_CHECK()                                                                                       \
    _CLS_REFLECT_ISLAND_RANGE_CHECK()                                                                                   \
    {                                                                                                                   \
        uint32_t low_addr = ((remote_ME & 0xf) << 12) | (remote_xfer_reg_number & 0xfff);                            \
        uint32_t hi_addr = (remote_island & 0x3f) << 26;                                                            \
        if (__is_ct_const(count) && count<=8)                                                                           \
        {                                                                                                               \
            CT_ASSERT(count!=0);                                                                                        \
            if (sync == sig_done)                                                                                       \
            {                                                                                                           \
                __asm cls[command, *data, hi_addr, << 8, low_addr, __ct_const_val(count)], sig_done[*sig_ptr]            \
            }                                                                                                           \
            else                                                                                                        \
            {                                                                                                           \
                __asm cls[command, *data, hi_addr, << 8, low_addr, __ct_const_val(count)], ctx_swap[*sig_ptr]            \
            }                                                                                                           \
        }                                                                                                               \
        else                                                                                                            \
        {                                                                                                               \
            if (__is_ct_const(count))                                                                                   \
            {                                                                                                           \
                CT_ASSERT(count<=16);                                                                                   \
            }                                                                                                           \
            else                                                                                                        \
            {                                                                                                           \
                CT_QPERFINFO_INDIRECT_REF(command);                                                                     \
            }                                                                                                           \
            {                                                                                                           \
                generic_ind_t ind;                                                                                      \
                _INTRINSIC_OVERRIDE_LENGTH(ind, (count - 1));                                                           \
                if (sync == sig_done)                                                                                   \
                {                                                                                                       \
                    __asm alu[--, --, B, ALU_INDIRECT_TYPE(ind)]                                                        \
                    __asm cls[command, *data, hi_addr, << 8, low_addr, __ct_const_val(count)], sig_done[*sig_ptr], indirect_ref      \
                }                                                                                                       \
                else                                                                                                    \
                {                                                                                                       \
                    __asm  alu[--, --, B, ALU_INDIRECT_TYPE(ind)]                                                       \
                    __asm cls[command, *data, hi_addr, << 8, low_addr, __ct_const_val(count)], ctx_swap[*sig_ptr], indirect_ref      \
                }                                                                                                       \
            }                                                                                                           \
        }                                                                                                               \
    }


/* Local Scratch reflect from signal src */

__intrinsic
void cmd_cls_reflect_write_sig_local(
    __xwrite void *data,                    /* data to reflect */
    uint32_t remote_ME,                     /* remote ME number */
    uint32_t remote_xfer_reg_number,        /* remote xfer register number */
    uint32_t count,                         /* number of longwords to reflect */
    sync_t sync,                            /* type of synchronization to use */
    volatile SIGNAL *sig_ptr                /* signal to raise upon completion */
)
{
    _INTRINSIC_BEGIN;
    {
        _CLS_IMPLEMENT_REFLECT_COMMAND(reflect_write_sig_local, 0);
    }
    _INTRINSIC_END;
}

__intrinsic
void cmd_cls_reflect_write_sig_local_ptr40(
    __xwrite void *data,                    /* data to reflect */
    uint32_t remote_island,                 /* remote island number */
    uint32_t remote_ME,                     /* remote ME number */
    uint32_t remote_xfer_reg_number,        /* remote xfer register number */
    uint32_t count,                         /* number of longwords to reflect */
    sync_t sync,                            /* type of synchronization to use */
    volatile SIGNAL *sig_ptr                /* signal to raise upon completion */
)
{
    _INTRINSIC_BEGIN;
    {
        _CLS_IMPLEMENT_REFLECT_COMMAND_PTR40(reflect_write_sig_local, 0);
    }
    _INTRINSIC_END;
}


/* Local Scratch reflect from signal dest */
void cmd_cls_reflect_write_sig_remote(
    __xwrite void *data,                    /* data to reflect */
    uint32_t remote_ME,                     /* remote ME number */
    uint32_t remote_xfer_reg_number,        /* remote xfer register number */
    int32_t sig,                            /* triggered signal number*/
    uint32_t count                          /* number of longwords to reflect */
)
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_write_reg(data));
        CT_ASSERT(__is_ct_const(sig) && sig<16);
        _CLS_REFLECT_ME_RANGE_CHECK();

        {
            /* Address encoding formula for cls reflect
            bits[11:0]  = xfer register address (reg number)
            bits[15:12] = FPC/ME
            bits[23:16] = 8b0
            bits[27:24] = Signal number
            bits[30:28] = ctx
            bit[31]     = Signal reference (If set, signal number comes from bits [27:24]
            */

            uint32_t address = (1 << 31) | (sig << 24) | ((remote_ME & 0xf) << 12) | (remote_xfer_reg_number & 0xfff);

            if (__is_ct_const(count) && count<=8)
            {
                CT_ASSERT(count!=0);
                __asm cls[reflect_write_sig_remote, *data, address, 0, __ct_const_val(count)]
            } else
            {
                if (__is_ct_const(count))
                {
                    CT_ASSERT(count<=16);
                }
                else
                {
                    CT_QPERFINFO_INDIRECT_REF(reflect_write_sig_remote);
                }

                {
                   generic_ind_t ind;
                    _INTRINSIC_OVERRIDE_LENGTH(ind, count - 1);
                    __asm
                    {
                        alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
                        cls[reflect_write_sig_remote, *data, address, 0, __ct_const_val(count)], indirect_ref
                    }
                }
            }
        }
    }
    _INTRINSIC_END;
}

void cmd_cls_reflect_write_sig_remote_ptr40(
    __xwrite void *data,                    /* data to reflect */
    uint32_t remote_island,                 /* remote island number */
    uint32_t remote_ME,                     /* remote ME number */
    uint32_t remote_xfer_reg_number,        /* remote xfer register number */
    int32_t sig,                            /* triggered signal number*/
    uint32_t count                          /* number of longwords to reflect */
)
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_write_reg(data));
        CT_ASSERT(__is_ct_const(sig) && sig<16);

        _CLS_REFLECT_ME_RANGE_CHECK();
        _CLS_REFLECT_ISLAND_RANGE_CHECK();

        {
            /* Address encoding formula for cls reflect
            bits[11:0]  = xfer register address (reg number)
            bits[15:12] = FPC/ME
            bits[23:16] = 8b0
            bits[27:24] = Signal number
            bits[30:28] = ctx
            bit[31]     = Signal reference (If set, signal number comes from bits [27:24]
            */
            uint32_t low_addr = (1 << 31) | (sig << 24) | ((remote_ME & 0xf) << 12) | (remote_xfer_reg_number & 0xfff);
            uint32_t hi_addr = (remote_island & 0x3f) << 26;

            if (__is_ct_const(count) && count<=8)
            {
                CT_ASSERT(count!=0);
                __asm cls[reflect_write_sig_remote, *data, hi_addr, << 8, low_addr, __ct_const_val(count)]
            } else
            {
                if (__is_ct_const(count))
                {
                    CT_ASSERT(count<=16);
                }
                else
                {
                    CT_QPERFINFO_INDIRECT_REF(reflect_write_sig_remote);
                }

                {
                   generic_ind_t ind;
                    _INTRINSIC_OVERRIDE_LENGTH(ind, count - 1);
                    __asm
                    {
                        alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
                        cls[reflect_write_sig_remote, *data, hi_addr, << 8, low_addr, __ct_const_val(count)], indirect_ref
                    }
                }
            }
        }
    }
    _INTRINSIC_END;
}


/* Local Scratch reflect from signal both */

void cmd_cls_reflect_write_sig_both(
    __xwrite void *data,                    /* data to reflect */
    uint32_t remote_ME,                     /* remote ME number */
    uint32_t remote_xfer_reg_number,        /* remote xfer register number */
    uint32_t count,                         /* number of longwords to reflect */
    sync_t sync,                            /* type of synchronization to use */
    volatile SIGNAL *sig_ptr                /* signal to raise upon completion */
)
{
    _INTRINSIC_BEGIN;
    {
        _CLS_IMPLEMENT_REFLECT_COMMAND(reflect_write_sig_both, 0);
    }
    _INTRINSIC_END;
}



void cmd_cls_reflect_write_sig_both_ptr40(
    __xwrite void *data,                    /* data to reflect */
    uint32_t remote_island,                 /* remote island number */
    uint32_t remote_ME,                     /* remote ME number */
    uint32_t remote_xfer_reg_number,        /* remote xfer register number */
    uint32_t count,                         /* number of longwords to reflect */
    sync_t sync,                            /* type of synchronization to use */
    volatile SIGNAL *sig_ptr                /* signal to raise upon completion */
)
{
    _INTRINSIC_BEGIN;
    {
        _CLS_IMPLEMENT_REFLECT_COMMAND_PTR40(reflect_write_sig_both, 0);
    }
    _INTRINSIC_END;
}


/* Local Scratch reflect to signal src */

void cmd_cls_reflect_read_sig_remote(
    __xread void *data,                     /* data from reflect */
    uint32_t remote_ME,                       /* remote ME number */
    uint32_t remote_xfer_reg_number,          /* remote xfer register number */
    int32_t sig,                              /* triggered signal number*/
    uint32_t count                            /* number of longwords to reflect */
)
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_read_reg(data));
        CT_ASSERT(__is_ct_const(sig) && sig<16);
        _CLS_REFLECT_ME_RANGE_CHECK();

        {
            /* Address encoding formula for cls reflect
            bits[11:0]  = xfer register address (reg number)
            bits[15:12] = FPC/ME
            bits[23:16] = 8b0
            bits[27:24] = Signal number
            bits[30:28] = ctx
            bit[31]     = Signal reference (If set, signal number comes from bits [27:24]
            */

            uint32_t address = (1 << 31) | (sig << 24) | ((remote_ME & 0xf) << 12) | (remote_xfer_reg_number & 0xfff);

            if (__is_ct_const(count) && count<=8)
            {
                CT_ASSERT(count!=0);
                __asm cls[reflect_read_sig_remote, *data, address, 0, __ct_const_val(count)]
            } else
            {
                if (__is_ct_const(count))
                {
                    CT_ASSERT(count<=16);
                }
                else
                {
                    CT_QPERFINFO_INDIRECT_REF(reflect_read_sig_remote);
                }

                {

                    generic_ind_t ind;
                    _INTRINSIC_OVERRIDE_LENGTH(ind, count - 1);
                    __asm
                    {
                        alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
                        cls[reflect_read_sig_remote, *data, address, 0, __ct_const_val(count)], indirect_ref
                    }
                }
            }
        }
    }
    _INTRINSIC_END;
}

void cmd_cls_reflect_read_sig_remote_ptr40(
    __xread void *data,                     /* data from reflect */
    uint32_t remote_island,                   /* remote island number */
    uint32_t remote_ME,                       /* remote ME number */
    uint32_t remote_xfer_reg_number,          /* remote xfer register number */
    int32_t sig,                              /* triggered signal number*/
    uint32_t count                            /* number of longwords to reflect */
)
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_read_reg(data));
        CT_ASSERT(__is_ct_const(sig) && sig<16);

        _CLS_REFLECT_ME_RANGE_CHECK();
        _CLS_REFLECT_ISLAND_RANGE_CHECK();
        {
            /* Address encoding formula for cls reflect
            bits[11:0]  = xfer register address (reg number)
            bits[15:12] = FPC/ME
            bits[23:16] = 8b0
            bits[27:24] = Signal number
            bits[30:28] = ctx
            bit[31]     = Signal reference (If set, signal number comes from bits [27:24]
            bit[33:32]  = 2b0
            bit[39:34]  = island number
            */

            uint32_t low_addr = (1 << 31) | (sig << 24) | ((remote_ME & 0xf) << 12) | (remote_xfer_reg_number & 0xfff);
            uint32_t hi_addr = (remote_island & 0x3f) << 26;

            if (__is_ct_const(count) && count<=8)
            {
                CT_ASSERT(count!=0);
                __asm cls[reflect_read_sig_remote, *data, hi_addr, << 8, low_addr, __ct_const_val(count)]
            } else
            {
                if (__is_ct_const(count))
                {
                    CT_ASSERT(count<=16);
                }
                else
                {
                    CT_QPERFINFO_INDIRECT_REF(reflect_read_sig_remote);
                }

                {

                    generic_ind_t ind;
                    _INTRINSIC_OVERRIDE_LENGTH(ind, count - 1);
                    __asm
                    {
                        alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
                        cls[reflect_read_sig_remote, *data, hi_addr, << 8, low_addr, __ct_const_val(count)], indirect_ref
                    }
                }
            }
        }
    }
    _INTRINSIC_END;
}


/* Local Scratch reflect to signal dest */

void cmd_cls_reflect_read_sig_local(
    __xread void *data,                     /* data from reflect */
    uint32_t remote_ME,                     /* remote ME number */
    uint32_t remote_xfer_reg_number,        /* remote xfer register number */
    uint32_t count,                         /* number of longwords to reflect */
    sync_t sync,                            /* type of synchronization to use */
    volatile SIGNAL *sig_ptr                /* signal to raise upon completion */
)
{
    _INTRINSIC_BEGIN;
    {
        _CLS_IMPLEMENT_REFLECT_COMMAND(reflect_read_sig_local, 1);
    }
    _INTRINSIC_END;
}


/* Local Scratch reflect to signal dest */

void cmd_cls_reflect_read_sig_local_ptr40(
    __xread void *data,                     /* data from reflect */
    uint32_t remote_island,             /* remote island number */
    uint32_t remote_ME,                 /* remote ME number */
    uint32_t remote_xfer_reg_number,    /* remote xfer register number */
    uint32_t count,                     /* number of longwords to reflect */
    sync_t sync,                        /* type of synchronization to use */
    volatile SIGNAL *sig_ptr            /* signal to raise upon completion */
)
{
    _INTRINSIC_BEGIN;
    {
        _CLS_IMPLEMENT_REFLECT_COMMAND_PTR40(reflect_read_sig_local, 1);
    }
    _INTRINSIC_END;
}

/* Local Scratch reflect to signal both */

void cmd_cls_reflect_read_sig_both(
    __xread void *data,                     /* data from reflect */
    uint32_t remote_ME,                     /* remote ME number */
    uint32_t remote_xfer_reg_number,        /* remote xfer register number */
    uint32_t count,                         /* number of longwords to reflect */
    sync_t sync,                            /* type of synchronization to use */
    volatile SIGNAL *sig_ptr                /* signal to raise upon completion */
)
{
    _INTRINSIC_BEGIN;
    {
        _CLS_IMPLEMENT_REFLECT_COMMAND(reflect_read_sig_both, 1);
    }
    _INTRINSIC_END;
}


void cmd_cls_reflect_read_sig_both_ptr40(
    __xread void *data,                     /* data from reflect */
    uint32_t remote_island,                 /* remote island number */
    uint32_t remote_ME,                     /* remote ME number */
    uint32_t remote_xfer_reg_number,        /* remote xfer register number */
    uint32_t count,                         /* number of longwords to reflect */
    sync_t sync,                            /* type of synchronization to use */
    volatile SIGNAL *sig_ptr                /* signal to raise upon completion */
)
{
    _INTRINSIC_BEGIN;
    {
        _CLS_IMPLEMENT_REFLECT_COMMAND_PTR40(reflect_read_sig_both, 1);
    }
    _INTRINSIC_END;
}


__intrinsic
void cmd_cls_reflect_write_sig_local_ind_ptr40(
    __xwrite void *data,                    /* data to reflect */
    uint32_t remote_island,                 /* remote island number */
    uint32_t remote_ME,                     /* remote ME number */
    uint32_t remote_xfer_reg_number,        /* remote xfer register number */
    uint32_t max_nn,                        /* max number of longwords to reflect */
    generic_ind_t ind,                      /* indirect */
    sync_t sync,                            /* type of synchronization to use */
    volatile SIGNAL *sig_ptr                /* signal to raise upon completion */
    )
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_write_reg(data));
        CT_ASSERT(__is_ct_const(max_nn) && max_nn <= 16);
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done || sync == ctx_swap);
        _CLS_REFLECT_ME_RANGE_CHECK();
        _CLS_REFLECT_ISLAND_RANGE_CHECK();

        {
            /* Address encoding formula for cls reflect
            *          bits[11:0]  = xfer register address (reg number)
            *          bits[15:12] = FPC/ME
            *          bits[23:16] = 8b0
            *          bits[27:24] = Signal number
            *          bits[30:28] = ctx
            *          bit[31]     = Signal reference (If set, signal number comes from bits [27:24]
            *          bit[33:32]  = 2b0
            *          bit[39:34]  = island number
            */
            uint32_t low_addr = ((remote_ME & 0xf) << 12) | (remote_xfer_reg_number & 0xfff);
            uint32_t hi_addr = (remote_island & 0x3f) << 26;

            __asm local_csr_wr[CMD_INDIRECT_REF_0, CSR_INDIRECT_TYPE(ind)]
            __asm alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
            if (sync == sig_done)
            {
                __asm cls[reflect_write_sig_local, *data, hi_addr, <<8, low_addr, __ct_const_val(max_nn)], sig_done[*sig_ptr], indirect_ref
            }
            else
            {
                __asm cls[reflect_write_sig_local, *data, hi_addr, <<8, low_addr, __ct_const_val(max_nn)], ctx_swap[*sig_ptr], indirect_ref
            }
        }
    }
    _INTRINSIC_END;
}


__intrinsic
void cmd_cls_reflect_write_sig_local_ind(
    __xwrite void *data,                    /* data to reflect */
    uint32_t remote_ME,                     /* remote ME number */
    uint32_t remote_xfer_reg_number,        /* remote xfer register number */
    uint32_t max_nn,                        /* max number of longwords to reflect */
    generic_ind_t ind,                      /* indirect */
    sync_t sync,                            /* type of synchronization to use */
    volatile SIGNAL *sig_ptr                /* signal to raise upon completion */
    )
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_write_reg(data));
        CT_ASSERT(__is_ct_const(max_nn) && max_nn <= 16);
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done || sync == ctx_swap);
        _CLS_REFLECT_ME_RANGE_CHECK();

        {
            /* Address encoding formula
            * Address encoding formula for cls reflect
            *          bits[11:0]  = xfer register address (reg number)
            *          bits[15:12] = FPC/ME
            *          bits[23:16] = 8b0
            *          bits[27:24] = Signal number
            *          bits[30:28] = ctx
            *          bit[31]     = Signal reference (If set, signal number comes from bits [27:24]
            */
            uint32_t address = ((remote_ME & 0xf) << 12) | (remote_xfer_reg_number & 0xfff);

            __asm local_csr_wr[CMD_INDIRECT_REF_0, CSR_INDIRECT_TYPE(ind)]
            __asm alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
            if (sync == sig_done)
            {
                __asm cls[reflect_write_sig_local, *data, address, 0, __ct_const_val(max_nn)], sig_done[*sig_ptr], indirect_ref
            }
            else
            {
                __asm cls[reflect_write_sig_local, *data, address, 0, __ct_const_val(max_nn)], ctx_swap[*sig_ptr], indirect_ref
            }
        }
    }
    _INTRINSIC_END;
}


__intrinsic
void cmd_cls_reflect_write_sig_remote_ind_ptr40(
    __xwrite void *data,                    /* data to reflect */
    uint32_t remote_island,                 /* remote island number */
    uint32_t remote_ME,                     /* remote ME number */
    uint32_t remote_xfer_reg_number,        /* remote xfer register number */
    int32_t sig,                            /* triggered signal number*/
    uint32_t max_nn,                        /* max number of longwords to reflect */
    generic_ind_t ind                       /* indirect */
    )
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_write_reg(data));
        CT_ASSERT(__is_ct_const(sig) && sig < 16);
        CT_ASSERT(__is_ct_const(max_nn) && max_nn <= 16);
        _CLS_REFLECT_ME_RANGE_CHECK();
        _CLS_REFLECT_ISLAND_RANGE_CHECK();

        {
            /* Address encoding formula for cls reflect
            *          bits[11:0]  = xfer register address (reg number)
            *          bits[15:12] = FPC/ME
            *          bits[23:16] = 8b0
            *          bits[27:24] = Signal number
            *          bits[30:28] = ctx
            *          bit[31]     = Signal reference (If set, signal number comes from bits [27:24]
            *          bit[33:32]  = 2b0
            *          bit[39:34]  = island number
            */
        uint32_t low_addr = (1 << 31) | (sig << 24) | ((remote_ME & 0xf) << 12) | (remote_xfer_reg_number & 0xfff);
        uint32_t hi_addr = (remote_island & 0x3f) << 26;

            __asm local_csr_wr[CMD_INDIRECT_REF_0, CSR_INDIRECT_TYPE(ind)]
            __asm alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
            __asm cls[reflect_write_sig_remote, *data, hi_addr, <<8, low_addr, __ct_const_val(max_nn)], indirect_ref
        }
    }
    _INTRINSIC_END;
}


__intrinsic
void cmd_cls_reflect_write_sig_remote_ind(
    __xwrite void *data,                    /* data to reflect */
    uint32_t remote_ME,                     /* remote ME number */
    uint32_t remote_xfer_reg_number,        /* remote xfer register number */
    int32_t sig,                            /* triggered signal number*/
    uint32_t max_nn,                        /* max number of longwords to reflect */
    generic_ind_t ind                       /* indirect */
    )
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_write_reg(data));
        CT_ASSERT(__is_ct_const(sig) && sig < 16);
        CT_ASSERT(__is_ct_const(max_nn) && max_nn <= 16);
        if(__is_ct_const(remote_ME))
        _CLS_REFLECT_ME_RANGE_CHECK();

        {
            /* Address encoding formula
            * Address encoding formula for cls reflect
            *          bits[11:0]  = xfer register address (reg number)
            *          bits[15:12] = FPC/ME
            *          bits[23:16] = 8b0
            *          bits[27:24] = Signal number
            *          bits[30:28] = ctx
            *          bit[31]     = Signal reference (If set, signal number comes from bits [27:24]
            */
            uint32_t address = (1 << 31) | (sig << 24) | ((remote_ME & 0xf) << 12) | (remote_xfer_reg_number & 0xfff);

            __asm local_csr_wr[CMD_INDIRECT_REF_0, CSR_INDIRECT_TYPE(ind)]
            __asm alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
            __asm cls[reflect_write_sig_remote, *data, address, 0, __ct_const_val(max_nn)], indirect_ref
        }
    }
    _INTRINSIC_END;
}


__intrinsic
void cmd_cls_reflect_write_sig_both_ind_ptr40(
    __xwrite void *data,                        /* data to reflect */
    uint32_t remote_island,                 /* remote island number */
    uint32_t remote_ME,                     /* remote ME number */
    uint32_t remote_xfer_reg_number,        /* remote xfer register number */
    uint32_t max_nn,                        /* max number of longwords to reflect */
    generic_ind_t ind,                          /* indirect */
    sync_t sync,                                /* type of synchronization to use */
    volatile SIGNAL *sig_ptr                    /* signal to raise upon completion */
    )
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_write_reg(data));
        CT_ASSERT(__is_ct_const(max_nn) && max_nn <= 16);
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done || sync == ctx_swap);
        _CLS_REFLECT_ME_RANGE_CHECK();
        _CLS_REFLECT_ISLAND_RANGE_CHECK();

        {
            /* Address encoding formula for cls reflect
            *          bits[11:0]  = xfer register address (reg number)
            *          bits[15:12] = FPC/ME
            *          bits[23:16] = 8b0
            *          bits[27:24] = Signal number
            *          bits[30:28] = ctx
            *          bit[31]     = Signal reference (If set, signal number comes from bits [27:24]
            *          bit[33:32]  = 2b0
            *          bit[39:34]  = island number
            */
        uint32_t low_addr = ((remote_ME & 0xf) << 12) | (remote_xfer_reg_number & 0xfff);
        uint32_t hi_addr = (remote_island & 0x3f) << 26;

            __asm local_csr_wr[CMD_INDIRECT_REF_0, CSR_INDIRECT_TYPE(ind)]
            __asm alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
            if (sync == sig_done)
            {
                __asm cls[reflect_write_sig_both, *data, hi_addr, <<8, low_addr, __ct_const_val(max_nn)], sig_done[*sig_ptr], indirect_ref
            }
            else
            {
                __asm cls[reflect_write_sig_both, *data, hi_addr, <<8, low_addr, __ct_const_val(max_nn)], ctx_swap[*sig_ptr], indirect_ref
            }
        }
    }
    _INTRINSIC_END;
}


__intrinsic
void cmd_cls_reflect_write_sig_both_ind(
    __xwrite void *data,                        /* data to reflect */
    uint32_t remote_ME,                     /* remote ME number */
    uint32_t remote_xfer_reg_number,        /* remote xfer register number */
    uint32_t max_nn,                        /* max number of longwords to reflect */
    generic_ind_t ind,                          /* indirect */
    sync_t sync,                                /* type of synchronization to use */
    volatile SIGNAL *sig_ptr                    /* signal to raise upon completion */
    )
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_write_reg(data));
        CT_ASSERT(__is_ct_const(max_nn) && max_nn <= 16);
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done || sync == ctx_swap);
        _CLS_REFLECT_ME_RANGE_CHECK();

        {
            /* Address encoding formula for reflect
            *          bits[11:0]  = xfer register address (reg number)
            *          bits[15:12] = FPC/ME
            *          bits[23:16] = 8b0
            *          bits[27:24] = Signal number
            *          bits[30:28] = ctx
            *          bit[31]     = Signal reference (If set, signal number comes from bits [27:24]
            */
            uint32_t address = ((remote_ME & 0xf) << 12) | (remote_xfer_reg_number & 0xfff);

            __asm local_csr_wr[CMD_INDIRECT_REF_0, CSR_INDIRECT_TYPE(ind)]
            __asm alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
            if (sync == sig_done)
            {
                __asm cls[reflect_write_sig_both, *data, address, 0, __ct_const_val(max_nn)], sig_done[*sig_ptr], indirect_ref
            }
            else
            {
                __asm cls[reflect_write_sig_both, *data, address, 0, __ct_const_val(max_nn)], ctx_swap[*sig_ptr], indirect_ref
            }
        }
    }
    _INTRINSIC_END;
}


__intrinsic
void cmd_cls_reflect_read_sig_remote_ind_ptr40(
    __xread void *data,                     /* data from reflect */
    uint32_t remote_island,                 /* remote island number */
    uint32_t remote_ME,                     /* remote ME number */
    uint32_t remote_xfer_reg_number,        /* remote xfer register number */
    int32_t sig,                            /* triggered signal number*/
    uint32_t max_nn,                        /* max number of longwords to reflect */
    generic_ind_t ind                       /* indirect */
    )
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_read_reg(data));
        CT_ASSERT(__is_ct_const(sig) && sig < 16);
        CT_ASSERT(__is_ct_const(max_nn) && max_nn <= 16);
        _CLS_REFLECT_ME_RANGE_CHECK();
        _CLS_REFLECT_ISLAND_RANGE_CHECK();

        {
            /* Address encoding formula for cls reflect
            *          bits[11:0]  = xfer register address (reg number)
            *          bits[15:12] = FPC/ME
            *          bits[23:16] = 8b0
            *          bits[27:24] = Signal number
            *          bits[30:28] = ctx
            *          bit[31]     = Signal reference (If set, signal number comes from bits [27:24]
            *          bit[33:32]  = 2b0
            *          bit[39:34]  = island number
            */
        uint32_t low_addr = (1 << 31) | (sig << 24) | ((remote_ME & 0xf) << 12) | (remote_xfer_reg_number & 0xfff);
        uint32_t hi_addr = (remote_island & 0x3f) << 26;

            __asm local_csr_wr[CMD_INDIRECT_REF_0, CSR_INDIRECT_TYPE(ind)]
            __asm alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
            __asm cls[reflect_read_sig_remote, *data, hi_addr, <<8, low_addr, __ct_const_val(max_nn)], indirect_ref
        }
    }
    _INTRINSIC_END;
}


__intrinsic
void cmd_cls_reflect_read_sig_remote_ind(
    __xread void *data,                     /* data from reflect */
    uint32_t remote_ME,                     /* remote ME number */
    uint32_t remote_xfer_reg_number,        /* remote xfer register number */
    int32_t sig,                            /* triggered signal number*/
    uint32_t max_nn,                        /* max number of longwords to reflect */
    generic_ind_t ind                       /* indirect */
    )
{
    _INTRINSIC_BEGIN;
    {

        CT_ASSERT(__is_read_reg(data));
        CT_ASSERT(__is_ct_const(sig) && sig < 16);
        CT_ASSERT(__is_ct_const(max_nn) && max_nn <= 16);
        _CLS_REFLECT_ME_RANGE_CHECK();

        {
            /* Address encoding formula
            * Address encoding formula for cls reflect
            *          bits[11:0]  = xfer register address (reg number)
            *          bits[15:12] = FPC/ME
            *          bits[23:16] = 8b0
            *          bits[27:24] = Signal number
            *          bits[30:28] = ctx
            *          bit[31]     = Signal reference (If set, signal number comes from bits [27:24]
            */
            uint32_t address = (1 << 31) | (sig << 24) | ((remote_ME & 0xf) << 12) | (remote_xfer_reg_number & 0xfff);

            __asm local_csr_wr[CMD_INDIRECT_REF_0, CSR_INDIRECT_TYPE(ind)]
            __asm alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
            __asm cls[reflect_read_sig_remote, *data, address, 0, __ct_const_val(max_nn)], indirect_ref
        }

    }
    _INTRINSIC_END;
}


__intrinsic
void cmd_cls_reflect_read_sig_local_ind_ptr40(
    __xread void *data,                     /* data from reflect */
    uint32_t remote_island,                 /* remote island number */
    uint32_t remote_ME,                     /* remote ME number */
    uint32_t remote_xfer_reg_number,        /* remote xfer register number */
    uint32_t max_nn,                        /* max number of longwords to reflect */
    generic_ind_t ind,                      /* indirect */
    sync_t sync,                            /* type of synchronization to use */
    volatile SIGNAL *sig_ptr                /* signal to raise upon completion */
    )
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_read_reg(data));
        CT_ASSERT(__is_ct_const(max_nn) && max_nn <= 16);
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done || sync == ctx_swap);
        _CLS_REFLECT_ME_RANGE_CHECK();
        _CLS_REFLECT_ISLAND_RANGE_CHECK();

        {
            /* Address encoding formula for cls reflect
            *          bits[11:0]  = xfer register address (reg number)
            *          bits[15:12] = FPC/ME
            *          bits[23:16] = 8b0
            *          bits[27:24] = Signal number
            *          bits[30:28] = ctx
            *          bit[31]     = Signal reference (If set, signal number comes from bits [27:24]
            *          bit[33:32]  = 2b0
            *          bit[39:34]  = island number
            */
        uint32_t low_addr = ((remote_ME & 0xf) << 12) | (remote_xfer_reg_number & 0xfff);
        uint32_t hi_addr = (remote_island & 0x3f) << 26;

            __asm local_csr_wr[CMD_INDIRECT_REF_0, CSR_INDIRECT_TYPE(ind)]
            __asm alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
            if (sync == sig_done)
            {
                __asm cls[reflect_read_sig_local, *data, hi_addr, <<8, low_addr, __ct_const_val(max_nn)], sig_done[*sig_ptr], indirect_ref
            }
            else
            {
                __asm cls[reflect_read_sig_local, *data, hi_addr, <<8, low_addr, __ct_const_val(max_nn)], ctx_swap[*sig_ptr], indirect_ref
            }
        }
    }
    _INTRINSIC_END;
}


__intrinsic
void cmd_cls_reflect_read_sig_local_ind(
    __xread void *data,                     /* data from reflect */
    uint32_t remote_ME,                     /* remote ME number */
    uint32_t remote_xfer_reg_number,        /* remote xfer register number */
    uint32_t max_nn,                        /* max number of longwords to reflect */
    generic_ind_t ind,                      /* indirect */
    sync_t sync,                            /* type of synchronization to use */
    volatile SIGNAL *sig_ptr                /* signal to raise upon completion */
    )
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_read_reg(data));
        CT_ASSERT(__is_ct_const(max_nn) && max_nn <= 16);
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done || sync == ctx_swap);
        if(__is_ct_const(remote_ME))
        _CLS_REFLECT_ME_RANGE_CHECK();

        {
            /* Address encoding formula
            * Address encoding formula for cls reflect
            *          bits[11:0]  = xfer register address (reg number)
            *          bits[15:12] = FPC/ME
            *          bits[23:16] = 8b0
            *          bits[27:24] = Signal number
            *          bits[30:28] = ctx
            *          bit[31]     = Signal reference (If set, signal number comes from bits [27:24]
            */
            uint32_t address = ((remote_ME & 0xf) << 12) | (remote_xfer_reg_number & 0xfff);

            __asm local_csr_wr[CMD_INDIRECT_REF_0, CSR_INDIRECT_TYPE(ind)]
            __asm alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
            if (sync == sig_done)
            {
                __asm cls[reflect_read_sig_local, *data, address, 0, __ct_const_val(max_nn)], sig_done[*sig_ptr], indirect_ref
            }
            else
            {
                __asm cls[reflect_read_sig_local, *data, address, 0, __ct_const_val(max_nn)], ctx_swap[*sig_ptr], indirect_ref
            }
        }
    }
    _INTRINSIC_END;
}

__intrinsic
void cmd_cls_reflect_read_sig_both_ind_ptr40(
    __xread void *data,                     /* data from reflect */
    uint32_t remote_island,                 /* remote island number */
    uint32_t remote_ME,                     /* remote ME number */
    uint32_t remote_xfer_reg_number,        /* remote xfer register number */
    uint32_t max_nn,                        /* max number of longwords to reflect */
    generic_ind_t ind,                      /* indirect */
    sync_t sync,                            /* type of synchronization to use */
    volatile SIGNAL *sig_ptr                /* signal to raise upon completion */
    )
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_read_reg(data));
        CT_ASSERT(__is_ct_const(max_nn) && max_nn <= 16);
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done || sync == ctx_swap);
        _CLS_REFLECT_ME_RANGE_CHECK();
        _CLS_REFLECT_ISLAND_RANGE_CHECK();

        {
            /* Address encoding formula for cls reflect
            *          bits[11:0]  = xfer register address (reg number)
            *          bits[15:12] = FPC/ME
            *          bits[23:16] = 8b0
            *          bits[27:24] = Signal number
            *          bits[30:28] = ctx
            *          bit[31]     = Signal reference (If set, signal number comes from bits [27:24]
            *          bit[33:32]  = 2b0
            *          bit[39:34]  = island number
            */
        uint32_t low_addr = ((remote_ME & 0xf) << 12) | (remote_xfer_reg_number & 0xfff);
        uint32_t hi_addr = (remote_island & 0x3f) << 26;

            __asm local_csr_wr[CMD_INDIRECT_REF_0, CSR_INDIRECT_TYPE(ind)]
            __asm alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
            if (sync == sig_done)
            {
                __asm cls[reflect_read_sig_both, *data, hi_addr, <<8, low_addr, __ct_const_val(max_nn)], sig_done[*sig_ptr], indirect_ref
            }
            else
            {
                __asm cls[reflect_read_sig_both, *data, hi_addr, <<8, low_addr, __ct_const_val(max_nn)], ctx_swap[*sig_ptr], indirect_ref
            }
        }
    }
    _INTRINSIC_END;
}


__intrinsic
void cmd_cls_reflect_read_sig_both_ind(
    __xread void *data,                     /* data from reflect */
    uint32_t remote_ME,                     /* remote ME number */
    uint32_t remote_xfer_reg_number,        /* remote xfer register number */
    uint32_t max_nn,                        /* max number of longwords to reflect */
    generic_ind_t ind,                      /* indirect */
    sync_t sync,                            /* type of synchronization to use */
    volatile SIGNAL *sig_ptr                /* signal to raise upon completion */
    )
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_read_reg(data));
        CT_ASSERT(__is_ct_const(max_nn) && max_nn <= 16);
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done || sync == ctx_swap);
        _CLS_REFLECT_ME_RANGE_CHECK();

        {
            /* Address encoding formula
            * Address encoding formula for cls reflect
            *          bits[11:0]  = xfer register address (reg number)
            *          bits[15:12] = FPC/ME
            *          bits[23:16] = 8b0
            *          bits[27:24] = Signal number
            *          bits[30:28] = ctx
            *          bit[31]     = Signal reference (If set, signal number comes from bits [27:24]
            */
            uint32_t address = ((remote_ME & 0xf) << 12) | (remote_xfer_reg_number & 0xfff);

            __asm local_csr_wr[CMD_INDIRECT_REF_0, CSR_INDIRECT_TYPE(ind)]
            __asm alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
            if (sync == sig_done)
            {
                __asm cls[reflect_read_sig_both, *data, address, 0, __ct_const_val(max_nn)], sig_done[*sig_ptr], indirect_ref
            }
            else
            {
                __asm cls[reflect_read_sig_both, *data, address, 0, __ct_const_val(max_nn)], ctx_swap[*sig_ptr], indirect_ref
            }
        }
    }
    _INTRINSIC_END;
}

#pragma diag_default 279  /* suppressed above */

#endif /* __NFP_CLS_REFLECT_C__ */
