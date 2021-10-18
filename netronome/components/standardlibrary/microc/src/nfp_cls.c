/*
 * Copyright (C) 2008-2014 Netronome Systems, Inc.  All rights reserved.
 */

#ifndef __NFP_CLS_C__
#define __NFP_CLS_C__

#include <nfp.h>
#include <nfp_intrinsic.h>
#include <nfp_cls.h>

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

/*
 *
 * NFP 6000 INDIRECT MACROS
 *
 */


/*
 * 6000 INDIRECT:
 * MACRO implementation of immediate command i.e. add_imm/sub_imm
 * length override or data master and xfer override
 */
#define _CLS_IMPLEMENT_IMM_COMMAND(cls_opr)                         \
{                                                                   \
    if (__is_ct_const(value) && (value < 8) && (value != 0))        \
    {                                                               \
        __asm cls[cls_opr, --, address, 0, __ct_const_val(value)]   \
    }                                                               \
    else                                                            \
    {                                                               \
        if (value > 0 && value < 32)                                \
        {                                                           \
            generic_ind_t ind;                                      \
            _INTRINSIC_OVERRIDE_LENGTH(ind, value);                 \
                                                                    \
            __asm alu[--, --, B, ALU_INDIRECT_TYPE(ind)]            \
            __asm cls[cls_opr, --, address, 0, max_1], indirect_ref \
        }                                                           \
        else                                                        \
        {                                                           \
            generic_ind_t ind;                                      \
            RT_RANGE_ASSERT(value < (1<<16));                       \
            _INTRINSIC_OVERRIDE_DATA_IN_ALU(ind, value);            \
                                                                    \
            __asm alu[--, --, B, ALU_INDIRECT_TYPE(ind)]            \
            __asm cls[cls_opr, --, address, 0, --], indirect_ref    \
        }                                                           \
    }                                                               \
}

/*
 * 6000 INDIRECT:
 * MACRO implementation of immediate command i.e. add_imm/sub_imm PTR40 version
 * length override or data master and xfer override
 */
#define _CLS_IMPLEMENT_IMM_COMMAND_PTR40(cls_opr)                                   \
{                                                                                   \
    uint32_t hi_addr, low_addr;                                                 \
    _INTRINSIC_CONVERT_HI_LO_ADDRESS(address);                                      \
    if (__is_ct_const(value) && (value < 8) && (value != 0))                        \
    {                                                                               \
        __asm cls[cls_opr, --, hi_addr, << 8, low_addr, __ct_const_val(value)]      \
    }                                                                               \
    else                                                                            \
    {                                                                               \
        if (value > 0 && value < 32)                                                \
        {                                                                           \
            generic_ind_t ind;                                                      \
            _INTRINSIC_OVERRIDE_LENGTH(ind, value);                                 \
            __asm alu[--, --, B, ALU_INDIRECT_TYPE(ind)]                            \
            __asm cls[cls_opr, --, hi_addr, << 8, low_addr, max_1], indirect_ref    \
        }                                                                           \
        else                                                                        \
        {                                                                           \
            generic_ind_t ind;                                                      \
            RT_RANGE_ASSERT(value < (1<<16));                                       \
            _INTRINSIC_OVERRIDE_DATA_IN_ALU(ind, value);                            \
            __asm alu[--, --, B, ALU_INDIRECT_TYPE(ind)]                            \
            __asm cls[cls_opr, --, hi_addr, << 8, low_addr, --], indirect_ref       \
        }                                                                           \
    }                                                                               \
}


/*
 *  MACRO implementation of vanilla commands
 * length override when ref count > 8 or not constant.
 */
#define _CLS_IMPLEMENT_COMMAND_PTR40(command, max_ref_count, is_read, xfer)                                                     \
{                                                                                                                               \
    if (is_read == 1) {                                                                                                         \
        CT_ASSERT(__is_read_reg(xfer));                                                                                         \
    }                                                                                                                           \
    else                                                                                                                        \
    {                                                                                                                           \
        CT_ASSERT(__is_write_reg(xfer));                                                                                        \
    }                                                                                                                           \
    {                                                                                                                           \
        uint32_t hi_addr, low_addr;                                                                                         \
        _INTRINSIC_CONVERT_HI_LO_ADDRESS(address);                                                                              \
        CT_ASSERT(__is_ct_const(sync));                                                                                         \
        CT_ASSERT(sync == sig_done || sync == ctx_swap);                                                                        \
        if (__is_ct_const(count) && count <= 8)                                                                                 \
        {                                                                                                                       \
            CT_ASSERT(count != 0);                                                                                              \
            if (sync == sig_done)                                                                                               \
            {                                                                                                                   \
                __asm cls[command, *xfer, hi_addr, <<8, low_addr, __ct_const_val(count)], sig_done[*sig_ptr]                    \
            }                                                                                                                   \
            else                                                                                                                \
            {                                                                                                                   \
                __asm cls[command, *xfer, hi_addr, <<8, low_addr, __ct_const_val(count)], ctx_swap[*sig_ptr]                    \
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
                    __asm cls[command, *xfer, hi_addr, <<8, low_addr, __ct_const_val(count)], sig_done[*sig_ptr], indirect_ref  \
                }                                                                                                               \
                else                                                                                                            \
                {                                                                                                               \
                    __asm cls[command, *xfer, hi_addr, <<8, low_addr, __ct_const_val(count)], ctx_swap[*sig_ptr], indirect_ref  \
                }                                                                                                               \
            }                                                                                                                   \
        }                                                                                                                       \
    }                                                                                                                           \
}


/*
 *  MACRO implementation of test commands - PTR40 version
 * length override when ref count > 8 or not constant. read/write register
 */
#define _CLS_IMPLEMENT_TEST_COMMAND_PTR40(command, max_ref_count, mask)                                                     \
{                                                                                                                           \
   uint32_t hi_addr, low_addr;                                                                                          \
   _INTRINSIC_CONVERT_HI_LO_ADDRESS(address);                                                                               \
    CT_ASSERT(__is_read_reg(val));                                                                                          \
    CT_ASSERT(__is_write_reg(mask));                                                                                        \
    CT_ASSERT(__is_ct_const(sync));                                                                                         \
    CT_ASSERT(sync == sig_done || sync == ctx_swap);                                                                        \
    __associate_read_write_reg_pair_no_spill(val, mask);                                                                    \
    if (__is_ct_const(count) && count <= 8)                                                                                 \
    {                                                                                                                       \
        CT_ASSERT(count != 0);                                                                                              \
        if (sync == sig_done)                                                                                               \
        {                                                                                                                   \
            __asm cls[command, *val, hi_addr, <<8, low_addr, __ct_const_val(count)], sig_done[*sig_ptr]                     \
        }                                                                                                                   \
        else                                                                                                                \
        {                                                                                                                   \
            __asm cls[command, *val, hi_addr, <<8, low_addr, __ct_const_val(count)], ctx_swap[*sig_ptr]                     \
        }                                                                                                                   \
    }                                                                                                                       \
    else                                                                                                                    \
    {                                                                                                                       \
        if (!__is_ct_const(count))                                                                                          \
        {                                                                                                                   \
            CT_QPERFINFO_INDIRECT_REF(command);                                                                             \
        }                                                                                                                   \
        {                                                                                                                   \
            _INTRINSIC_IND_ALU_COUNT_MIN_MAX(count, 1, max_ref_count);                                                      \
            if (sync == sig_done)                                                                                           \
            {                                                                                                               \
                __asm cls[command, *val, hi_addr, <<8, low_addr, __ct_const_val(count)], sig_done[*sig_ptr], indirect_ref   \
            }                                                                                                               \
            else                                                                                                            \
            {                                                                                                               \
                __asm cls[command, *val, hi_addr, <<8, low_addr, __ct_const_val(count)], ctx_swap[*sig_ptr], indirect_ref   \
            }                                                                                                               \
        }                                                                                                                   \
    }                                                                                                                       \
}

/*
 *  MACRO implementation of CAM commands where ref_count max_1 - PTR40 version
 * length override when ref count > 8 or not constant. read/write register
 */
#define _CLS_IMPLEMENT_CAM_MAX_1_COMMAND_PTR40(command, max_ref_count, read_xfer, write_xfer)                               \
{                                                                                                                           \
   uint32_t hi_addr, low_addr;                                                                                          \
   _INTRINSIC_CONVERT_HI_LO_ADDRESS(address);                                                                               \
    CT_ASSERT(__is_read_reg(read_xfer));                                                                                    \
    CT_ASSERT(__is_write_reg(write_xfer));                                                                                  \
    CT_ASSERT(__is_ct_const(sync));                                                                                         \
    CT_ASSERT(sync == sig_done || sync == ctx_swap);                                                                        \
    __associate_read_write_reg_pair_no_spill(read_xfer, write_xfer);                                                        \
    if (__is_ct_const(count))                                                                                               \
    {                                                                                                                       \
        CT_ASSERT(count <= max_ref_count);                                                                                  \
    }                                                                                                                       \
    else                                                                                                                    \
    {                                                                                                                       \
        CT_QPERFINFO_INDIRECT_REF(command);                                                                                 \
    }                                                                                                                       \
    {                                                                                                                       \
        _INTRINSIC_IND_ALU_COUNT_MIN_MAX(count, 1, max_ref_count);                                                          \
        if (sync == sig_done)                                                                                               \
        {                                                                                                                   \
            __asm cls[command, *read_xfer, hi_addr, <<8, low_addr, max_1], sig_done[*sig_ptr], indirect_ref                 \
        }                                                                                                                   \
        else                                                                                                                \
        {                                                                                                                   \
            __asm cls[command, *read_xfer, hi_addr, <<8, low_addr, max_1], ctx_swap[*sig_ptr], indirect_ref                 \
        }                                                                                                                   \
   }                                                                                                                        \
}

/*
 *  MACRO implementation of CAM commands where ref_count max_2 - PTR40 version
 * length override when ref count > 8 or not constant. read/write register
 */
#define _CLS_IMPLEMENT_CAM_MAX_2_COMMAND_PTR40(command, max_ref_count, read_xfer, write_xfer)                               \
{                                                                                                                           \
   uint32_t hi_addr, low_addr;                                                                                          \
   _INTRINSIC_CONVERT_HI_LO_ADDRESS(address);                                                                               \
    CT_ASSERT(__is_read_reg(read_xfer));                                                                                    \
    CT_ASSERT(__is_write_reg(write_xfer));                                                                                  \
    CT_ASSERT(__is_ct_const(sync));                                                                                         \
    CT_ASSERT(sync == sig_done || sync == ctx_swap);                                                                        \
    __associate_read_write_reg_pair_no_spill(read_xfer, write_xfer);                                                        \
    if (__is_ct_const(count))                                                                                               \
    {                                                                                                                       \
        CT_ASSERT(count <= max_ref_count);                                                                                  \
    }                                                                                                                       \
    else                                                                                                                    \
    {                                                                                                                       \
        CT_QPERFINFO_INDIRECT_REF(command);                                                                                 \
    }                                                                                                                       \
    {                                                                                                                       \
        _INTRINSIC_IND_ALU_COUNT_MIN_MAX(count, 1, max_ref_count);                                                          \
        if (sync == sig_done)                                                                                               \
        {                                                                                                                   \
            __asm cls[command, *read_xfer, hi_addr, <<8, low_addr, max_2], sig_done[*sig_ptr], indirect_ref                 \
        }                                                                                                                   \
        else                                                                                                                \
        {                                                                                                                   \
            __asm cls[command, *read_xfer, hi_addr, <<8, low_addr, max_2], ctx_swap[*sig_ptr], indirect_ref                 \
        }                                                                                                                   \
    }                                                                                                                       \
}

/*
 *  MACRO implementation of test immediate where value can only be up to 32 and should be overridden as
 *  length (5 bits)
 */
#define _CLS_IMPLEMENT_TEST_IMMEDIATE_PTR32(command, read_xfer)                                                             \
{                                                                                                                           \
    CT_ASSERT(__is_read_reg(val));                                                                                          \
    CT_ASSERT(__is_ct_const(sync));                                                                                         \
    CT_ASSERT(sync == sig_done || sync == ctx_swap);                                                                        \
    if (__is_ct_const(value) && (value <= 8) && (value != 0))                                                               \
    {                                                                                                                       \
        if (sync == sig_done)                                                                                               \
        {                                                                                                                   \
            __asm cls[command, *read_xfer, address, 0, __ct_const_val(value)], sig_done[*sig_ptr]                           \
        }                                                                                                                   \
        else                                                                                                                \
        {                                                                                                                   \
            __asm cls[command, *read_xfer, address, 0, __ct_const_val(value)], ctx_swap[*sig_ptr]                           \
        }                                                                                                                   \
    }                                                                                                                       \
    else                                                                                                                    \
    {                                                                                                                       \
        generic_ind_t ind;                                                                                                  \
                                                                                                                            \
        if (__is_ct_const(value))                                                                                           \
        {                                                                                                                   \
            CT_ASSERT(value > 0 && value < 32);                                                                             \
        }                                                                                                                   \
        else                                                                                                                \
        {                                                                                                                   \
            RT_RANGE_ASSERT(value > 0 && value < 32);                                                                       \
        }                                                                                                                   \
                                                                                                                            \
        _INTRINSIC_OVERRIDE_LENGTH(ind, value);                                                                             \
                                                                                                                            \
        if (sync == sig_done)                                                                                               \
        {                                                                                                                   \
            __asm  alu[--, --, B, ALU_INDIRECT_TYPE(ind)]                                                                   \
            __asm  cls[command, *read_xfer, address, 0, max_1], sig_done[*sig_ptr], indirect_ref                            \
        }                                                                                                                   \
        else                                                                                                                \
        {                                                                                                                   \
            __asm  alu[--, --, B, ALU_INDIRECT_TYPE(ind)]                                                                   \
            __asm  cls[command, *read_xfer, address, 0, max_1], ctx_swap[*sig_ptr], indirect_ref                            \
        }                                                                                                                   \
    }                                                                                                                       \
}

#define _CLS_IMPLEMENT_TEST_IMMEDIATE_PTR40(command, read_xfer)                                                             \
{                                                                                                                           \
    uint32_t hi_addr, low_addr;                                                                                         \
    _INTRINSIC_CONVERT_HI_LO_ADDRESS(address);                                                                              \
    CT_ASSERT(__is_read_reg(val));                                                                                          \
    CT_ASSERT(__is_ct_const(sync));                                                                                         \
    CT_ASSERT(sync == sig_done || sync == ctx_swap);                                                                        \
    if (__is_ct_const(value) && (value <= 8) && (value != 0))                                                               \
    {                                                                                                                       \
        if (sync == sig_done)                                                                                               \
        {                                                                                                                   \
            __asm cls[command, *read_xfer, hi_addr, <<8, low_addr, __ct_const_val(value)], sig_done[*sig_ptr]               \
        }                                                                                                                   \
        else                                                                                                                \
        {                                                                                                                   \
            __asm cls[command, *read_xfer, hi_addr, <<8, low_addr, __ct_const_val(value)], ctx_swap[*sig_ptr]               \
        }                                                                                                                   \
    }                                                                                                                       \
    else                                                                                                                    \
    {                                                                                                                       \
        generic_ind_t ind;                                                                                                  \
                                                                                                                            \
        if (__is_ct_const(value))                                                                                           \
        {                                                                                                                   \
            CT_ASSERT(value > 0 && value < 32);                                                                             \
        }                                                                                                                   \
        else                                                                                                                \
        {                                                                                                                   \
            RT_RANGE_ASSERT(value > 0 && value < 32);                                                                       \
        }                                                                                                                   \
                                                                                                                            \
        _INTRINSIC_OVERRIDE_LENGTH(ind, value);                                                                             \
                                                                                                                            \
        if (sync == sig_done)                                                                                               \
        {                                                                                                                   \
            __asm  alu[--, --, B, ALU_INDIRECT_TYPE(ind)]                                                                   \
            __asm  cls[command, *read_xfer, hi_addr, <<8, low_addr, max_1], sig_done[*sig_ptr], indirect_ref                \
        }                                                                                                                   \
        else                                                                                                                \
        {                                                                                                                   \
            __asm  alu[--, --, B, ALU_INDIRECT_TYPE(ind)]                                                                   \
            __asm  cls[command, *read_xfer, hi_addr, <<8, low_addr, max_1], ctx_swap[*sig_ptr], indirect_ref                \
        }                                                                                                                   \
    }                                                                                                                       \
}

/* Local scratch memory read              */

__intrinsic
void cmd_cls_read_be_ptr32(
    __xread void *data,                                 /* data to read                           */
    volatile void __cls *address,                       /* address to read from                   */
    uint32_t count,                                     /* number of words to read            */
    sync_t sync,                                        /* type of synchronization to use         */
    SIGNAL *sig_ptr                                     /* signal to raise upon completion        */
)
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_read_reg(data));
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done || sync == ctx_swap);

        if (__is_ct_const(count) && count <= 8)
        {
            CT_ASSERT(count != 0);
            if (sync == sig_done)
            {
                __asm cls[read_be, *data, address, 0, __ct_const_val(count)], sig_done[*sig_ptr]
            }
            else
            {
                __asm cls[read_be, *data, address, 0, __ct_const_val(count)], ctx_swap[*sig_ptr]
            }
        }
        else
        {
            if (!__is_ct_const(count))
            {
                CT_QPERFINFO_INDIRECT_REF(cls_read_be);
            }
            {
                _INTRINSIC_IND_ALU_COUNT_MIN_MAX(count, 1, INTRINSIC_IND_ALU_COUNT_MAX);
                if (sync == sig_done)
                {
                    __asm cls[read_be, *data, address, 0, __ct_const_val(count)], sig_done[*sig_ptr], indirect_ref
                }
                else
                {
                    __asm cls[read_be, *data, address, 0, __ct_const_val(count)], ctx_swap[*sig_ptr], indirect_ref
                }
            }
        }
    }
    _INTRINSIC_END;
}


/* Local scratch memory read little endian    */

void cmd_cls_read_le_ptr32(
    __xread void *data,                                 /* data to read                           */
    volatile void __cls *address,                       /* address to read from                   */
    uint32_t count,                                     /* number of words to read            */
    sync_t sync,                                        /* type of synchronization to use         */
    SIGNAL *sig_ptr                                     /* signal to raise upon completion        */
)
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_read_reg(data));
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done || sync == ctx_swap);

        if (__is_ct_const(count) && count <= 8)
        {
            CT_ASSERT(count != 0);
            if (sync == sig_done)
            {
                __asm cls[read_le, *data, address, 0, __ct_const_val(count)], sig_done[*sig_ptr]
            }
            else
            {
                __asm cls[read_le, *data, address, 0, __ct_const_val(count)], ctx_swap[*sig_ptr]
            }
        }
        else
        {
            if (!__is_ct_const(count))
            {
                CT_QPERFINFO_INDIRECT_REF(cls_read_le);
            }
            {
                _INTRINSIC_IND_ALU_COUNT_MIN_MAX(count, 1, INTRINSIC_IND_ALU_COUNT_MAX);
                if (sync == sig_done)
                {
                    __asm cls[read_le, *data, address, 0, __ct_const_val(count)], sig_done[*sig_ptr], indirect_ref
                }
                else
                {
                    __asm cls[read_le, *data, address, 0, __ct_const_val(count)], ctx_swap[*sig_ptr], indirect_ref
                }
            }
        }
    }
    _INTRINSIC_END;
}

/* Read words from Cluster Local Scratch.    */

void cmd_cls_read_ptr32(
    __xread void *data,                                 /* data to read                           */
    volatile void __cls *address,                       /* address to read from                   */
    uint32_t count,                                     /* number of words to read                */
    sync_t sync,                                        /* type of synchronization to use         */
    SIGNAL *sig_ptr                                     /* signal to raise upon completion        */
    )
{
#ifdef __BIGENDIAN
    cmd_cls_read_be_ptr32(data, address, count, sync, sig_ptr);
#else
    cmd_cls_read_le_ptr32(data, address, count, sync, sig_ptr);
#endif
}

__intrinsic
void cmd_cls_read_be_ptr40(
    __xread void *data,
    volatile void __addr40 __cls *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
)
{
    _INTRINSIC_BEGIN;
    {
        _CLS_IMPLEMENT_COMMAND_PTR40(read_be, INTRINSIC_IND_ALU_COUNT_MAX, 1, data);
    }
    _INTRINSIC_END;
}

__intrinsic
void cmd_cls_read_le_ptr40(
    __xread void *data,
    volatile void __addr40 __cls *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
)
{
    _INTRINSIC_BEGIN;
    {
        _CLS_IMPLEMENT_COMMAND_PTR40(read_le, INTRINSIC_IND_ALU_COUNT_MAX, 1, data);
    }
    _INTRINSIC_END;
}

__intrinsic
void cmd_cls_read_ptr40(
    __xread void *data,
    volatile void __addr40 __cls *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
)
{
#ifdef __BIGENDIAN
    cmd_cls_read_be_ptr40(data, address, count, sync, sig_ptr);
#else
    cmd_cls_read_le_ptr40(data, address, count, sync, sig_ptr);
#endif
}


/* Local scratch memory write             */

void cmd_cls_write_be_ptr32(
    __xwrite void *data,                                /* data to write                          */
    volatile void __addr32 __cls *address,              /* address to write                       */
    uint32_t count,                                     /* number of words to write               */
    sync_t sync,                                        /* type of synchronization to use         */
    SIGNAL *sig_ptr                                     /* signal to raise upon completion        */
)
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_write_reg(data));
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done || sync == ctx_swap);

        if (__is_ct_const(count) && count <= 8)
        {
            CT_ASSERT(count != 0);
            if (sync == sig_done)
            {
                __asm cls[write_be, *data, address, 0, __ct_const_val(count)], sig_done[*sig_ptr]
            }
            else
            {
                __asm cls[write_be, *data, address, 0, __ct_const_val(count)], ctx_swap[*sig_ptr]
            }
        }
        else
        {
            if (!__is_ct_const(count))
            {
                CT_QPERFINFO_INDIRECT_REF(cls_write_be);
            }
            {
                _INTRINSIC_IND_ALU_COUNT_MIN_MAX(count, 1, INTRINSIC_IND_ALU_COUNT_MAX);
                if (sync == sig_done)
                {
                    __asm cls[write_be, *data, address, 0, __ct_const_val(count)], sig_done[*sig_ptr], indirect_ref
                }
                else
                {
                    __asm cls[write_be, *data, address, 0, __ct_const_val(count)], ctx_swap[*sig_ptr], indirect_ref
                }
            }
        }
    }
    _INTRINSIC_END;
}

/* Write words to Cluster Local Scratch in Little Endian mode.    */

void cmd_cls_write_le_ptr32(
    __xwrite void *data,                                /* data to write                          */
    volatile void __cls *address,                       /* address to write                       */
    uint32_t count,                                 /* number of words to write           */
    sync_t sync,                                        /* type of synchronization to use         */
    SIGNAL *sig_ptr                                     /* signal to raise upon completion        */
)
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_write_reg(data));
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done || sync == ctx_swap);

        if (__is_ct_const(count) && count <= 8)
        {
            CT_ASSERT(count != 0);
            if (sync == sig_done)
            {
                __asm cls[write_le, *data, address, 0, __ct_const_val(count)], sig_done[*sig_ptr]
            }
            else
            {
                __asm cls[write_le, *data, address, 0, __ct_const_val(count)], ctx_swap[*sig_ptr]
            }
        }
        else
        {
            if (!__is_ct_const(count))
            {
                CT_QPERFINFO_INDIRECT_REF(cls_write_le);
            }
            {
                _INTRINSIC_IND_ALU_COUNT_MIN_MAX(count, 1, INTRINSIC_IND_ALU_COUNT_MAX);
                if (sync == sig_done)
                {
                    __asm cls[write_le, *data, address, 0, __ct_const_val(count)], sig_done[*sig_ptr], indirect_ref
                }
                else
                {
                    __asm cls[write_le, *data, address, 0, __ct_const_val(count)], ctx_swap[*sig_ptr], indirect_ref
                }
            }
        }
    }
    _INTRINSIC_END;
}



/* Write words to Cluster Local Scratch.      */

void cmd_cls_write_ptr32(
    __xwrite void *data,                                /* data to write                          */
    volatile void __cls *address,                       /* address to write                       */
    uint32_t count,                                 /* number of words to write           */
    sync_t sync,                                        /* type of synchronization to use         */
    SIGNAL *sig_ptr                                     /* signal to raise upon completion        */
)
{
#ifdef __BIGENDIAN
    cmd_cls_write_be_ptr32(data, address, count, sync, sig_ptr);
#else
    cmd_cls_write_le_ptr32(data, address, count, sync, sig_ptr);
#endif
}


/* Local scratch memory write             */

void cmd_cls_write_be_ptr40(
    __xwrite void *data,
    volatile void __addr40 __cls *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
)
{
    _INTRINSIC_BEGIN;
    {
        _CLS_IMPLEMENT_COMMAND_PTR40(write_be, INTRINSIC_IND_ALU_COUNT_MAX, 0, data);
    }
    _INTRINSIC_END;
}

__intrinsic
void cmd_cls_write_le_ptr40(
    __xwrite void *data,
    volatile void __addr40 __cls *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
)
{
    _INTRINSIC_BEGIN;
    {
        _CLS_IMPLEMENT_COMMAND_PTR40(write_le, INTRINSIC_IND_ALU_COUNT_MAX, 0, data);
    }
    _INTRINSIC_END;
}

__intrinsic
void cmd_cls_write_ptr40(
    __xwrite void *data,
    volatile void __addr40 __cls *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
)
{
#ifdef __BIGENDIAN
    cmd_cls_write_be_ptr40(data, address, count, sync, sig_ptr);
#else
    cmd_cls_write_le_ptr40(data, address, count, sync, sig_ptr);
#endif
}

__intrinsic
void cmd_cls_write8_be_ind_ptr40(
    __xwrite void *data,                                /* data to write                          */
    volatile void __addr40 __cls *address,              /* address to write to                    */
    uint32_t max_nn,                                    /* max. number of bytes to write          */
    generic_ind_t ind,                                  /* indirect word                          */
    sync_t sync,                                        /* type of synchronization to use         */
    SIGNAL *sig_ptr                                     /* signal to raise upon completion        */
    )
{
    _INTRINSIC_BEGIN;
    {
        uint32_t hi_addr, low_addr;

        _INTRINSIC_CONVERT_HI_LO_ADDRESS(address);
        CT_ASSERT(__is_write_reg(data));
        CT_ASSERT(__is_ct_const(max_nn));
        CT_ASSERT(max_nn > 0 && max_nn <= 32);
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done || sync == ctx_swap);

        __asm local_csr_wr[CMD_INDIRECT_REF_0, CSR_INDIRECT_TYPE(ind)]
        __asm  alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
        if (sync == sig_done)
        {
            __asm cls[write8_be, *data, hi_addr, <<8, low_addr, __ct_const_val(max_nn)], sig_done[*sig_ptr], indirect_ref
        }
        else
        {
            __asm cls[write8_be, *data, hi_addr, <<8, low_addr, __ct_const_val(max_nn)], ctx_swap[*sig_ptr], indirect_ref
        }
    }
    _INTRINSIC_END;
}


__intrinsic
void cmd_cls_write8_be_ind_ptr32(
    __xwrite void *data,                                /* data to write                          */
    volatile void __cls *address,                       /* address to write to                    */
    uint32_t max_nn,                                    /* max. number of bytes to write          */
    generic_ind_t ind,                                  /* indirect word                          */
    sync_t sync,                                        /* type of synchronization to use         */
    SIGNAL *sig_ptr                                     /* signal to raise upon completion        */
    )
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_write_reg(data));
        CT_ASSERT(__is_ct_const(max_nn));
        CT_ASSERT(max_nn > 0 && max_nn <= 32);
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done || sync == ctx_swap);

        __asm local_csr_wr[CMD_INDIRECT_REF_0, CSR_INDIRECT_TYPE(ind)]
        __asm  alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
        if (sync == sig_done)
        {
            __asm cls[write8_be, *data, address, 0, __ct_const_val(max_nn)], sig_done[*sig_ptr], indirect_ref
        }
        else
        {
            __asm cls[write8_be, *data, address, 0, __ct_const_val(max_nn)], ctx_swap[*sig_ptr], indirect_ref
        }
    }
    _INTRINSIC_END;
}


__intrinsic
void cmd_cls_write8_le_ind_ptr40(
    __xwrite void *data,                                /* data to write                          */
    volatile void __addr40 __cls *address,              /* address to write to                    */
    uint32_t max_nn,                                    /* max. number of bytes to write          */
    generic_ind_t ind,                                  /* indirect word                          */
    sync_t sync,                                        /* type of synchronization to use         */
    SIGNAL *sig_ptr                                     /* signal to raise upon completion        */
    )
{
    _INTRINSIC_BEGIN;
    {
        uint32_t hi_addr, low_addr;

        _INTRINSIC_CONVERT_HI_LO_ADDRESS(address);
        CT_ASSERT(__is_write_reg(data));
        CT_ASSERT(__is_ct_const(max_nn));
        CT_ASSERT(max_nn > 0 && max_nn <= 32);
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done || sync == ctx_swap);

        __asm local_csr_wr[CMD_INDIRECT_REF_0, CSR_INDIRECT_TYPE(ind)]
        __asm  alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
        if (sync == sig_done)
        {
            __asm cls[write8_le, *data, hi_addr, <<8, low_addr, __ct_const_val(max_nn)], sig_done[*sig_ptr], indirect_ref
        }
        else
        {
            __asm cls[write8_le, *data, hi_addr, <<8, low_addr, __ct_const_val(max_nn)], ctx_swap[*sig_ptr], indirect_ref
        }
    }
    _INTRINSIC_END;
}

__intrinsic
void cmd_cls_write8_le_ind_ptr32(
    __xwrite void *data,                                /* data to write                          */
    volatile void __cls *address,                       /* address to write to                    */
    uint32_t max_nn,                                    /* max. number of bytes to write          */
    generic_ind_t ind,                                  /* indirect word                          */
    sync_t sync,                                        /* type of synchronization to use         */
    SIGNAL *sig_ptr                                     /* signal to raise upon completion        */
    )
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_write_reg(data));
        CT_ASSERT(__is_ct_const(max_nn));
        CT_ASSERT(max_nn > 0 && max_nn <= 32);
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done || sync == ctx_swap);

        __asm local_csr_wr[CMD_INDIRECT_REF_0, CSR_INDIRECT_TYPE(ind)]
        __asm  alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
        if (sync == sig_done)
        {
            __asm cls[write8_le, *data, address, 0, __ct_const_val(max_nn)], sig_done[*sig_ptr], indirect_ref
        }
        else
        {
            __asm cls[write8_le, *data, address, 0, __ct_const_val(max_nn)], ctx_swap[*sig_ptr], indirect_ref
        }
    }
    _INTRINSIC_END;
}



void cmd_cls_write8_ind_ptr40(
    __xwrite void *data,                                /* data to write                          */
    volatile void __addr40 __cls *address,              /* address to write to                    */
    uint32_t max_nn,                                    /* number of bytes to write               */
    generic_ind_t ind,                                  /* indirect word                          */
    sync_t sync,                                        /* type of synchronization to use         */
    SIGNAL *sig_ptr                                     /* signal to raise upon completion        */
    )
{
#ifdef __BIGENDIAN
    cmd_cls_write8_be_ind_ptr40(data, address, max_nn, ind, sync, sig_ptr);
#else
    cmd_cls_write8_le_ind_ptr40(data, address, max_nn, ind, sync, sig_ptr);
#endif
}


void cmd_cls_write8_ind_ptr32(
    __xwrite void *data,                                /* data to write                          */
    volatile void __cls *address,                       /* address to write to                    */
    uint32_t max_nn,                                    /* number of bytes to write               */
    generic_ind_t ind,                                  /* indirect word                          */
    sync_t sync,                                        /* type of synchronization to use         */
    SIGNAL *sig_ptr                                     /* signal to raise upon completion        */
    )
{
#ifdef __BIGENDIAN
    cmd_cls_write8_be_ind_ptr32(data, address, max_nn, ind, sync, sig_ptr);
#else
    cmd_cls_write8_le_ind_ptr32(data, address, max_nn, ind, sync, sig_ptr);
#endif
}


void cmd_cls_write8_be_ptr32(
    __xwrite void *data,                                /* data to write                          */
    volatile void __cls *address,                       /* address to write                       */
    uint32_t count,                                 /* number of bytes to write               */
    sync_t sync,                                        /* type of synchronization to use         */
    SIGNAL *sig_ptr                                     /* signal to raise upon completion        */
    )
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_write_reg(data));
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done || sync == ctx_swap);

        if (__is_ct_const(count) && count <= 8)
        {
            CT_ASSERT(count != 0);
            if (sync == sig_done)
            {
                __asm cls[write8_be, *data, address, 0, __ct_const_val(count)], sig_done[*sig_ptr]
            }
            else
            {
                __asm cls[write8_be, *data, address, 0, __ct_const_val(count)], ctx_swap[*sig_ptr]
            }
        }
        else
        {
            if (!__is_ct_const(count))
            {
                CT_QPERFINFO_INDIRECT_REF(cls_write8_be);
            }
            {
                _INTRINSIC_IND_ALU_COUNT_MIN_MAX(count, 1, INTRINSIC_IND_ALU_COUNT_MAX);
                if (sync == sig_done)
                {
                    __asm cls[write8_be, *data, address, 0, __ct_const_val(count)], sig_done[*sig_ptr], indirect_ref
                }
                else
                {
                    __asm cls[write8_be, *data, address, 0, __ct_const_val(count)], ctx_swap[*sig_ptr], indirect_ref
                }
            }
        }
    }
    _INTRINSIC_END;
}

/* Write bytes to Cluster Local Scratch in Little Endian mode.      */

void cmd_cls_write8_le_ptr32(
    __xwrite void *data,                                /* data to write                          */
    volatile void __cls *address,                       /* address to write                       */
    uint32_t count,                                     /* number of bytes to write               */
    sync_t sync,                                        /* type of synchronization to use         */
    SIGNAL *sig_ptr                                     /* signal to raise upon completion        */
)
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_write_reg(data));
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done || sync == ctx_swap);

        if (__is_ct_const(count) && count <= 8)
        {
            CT_ASSERT(count != 0);
            if (sync == sig_done)
            {
                __asm cls[write8_le, *data, address, 0, __ct_const_val(count)], sig_done[*sig_ptr]
            }
            else
            {
                __asm cls[write8_le, *data, address, 0, __ct_const_val(count)], ctx_swap[*sig_ptr]
            }
        }
        else
        {
            if (!__is_ct_const(count))
            {
                CT_QPERFINFO_INDIRECT_REF(cls_write8_le);
            }
            {
                _INTRINSIC_IND_ALU_COUNT_MIN_MAX(count, 1, INTRINSIC_IND_ALU_COUNT_MAX);
                if (sync == sig_done)
                {
                    __asm cls[write8_le, *data, address, 0, __ct_const_val(count)], sig_done[*sig_ptr], indirect_ref
                }
                else
                {
                    __asm cls[write8_le, *data, address, 0, __ct_const_val(count)], ctx_swap[*sig_ptr], indirect_ref
                }
            }
        }
    }
    _INTRINSIC_END;
}

/* Write bytes to Cluster Local Scratch.      */

void cmd_cls_write8_ptr32(
    __xwrite void *data,                                /* data to write                          */
    volatile void __cls *address,                       /* address to write                       */
    uint32_t count,                                     /* number of bytes to write               */
    sync_t sync,                                        /* type of synchronization to use         */
    SIGNAL *sig_ptr                                     /* signal to raise upon completion        */
)
{
#ifdef __BIGENDIAN
    cmd_cls_write8_be_ptr32(data, address, count, sync, sig_ptr);
#else
    cmd_cls_write8_le_ptr32(data, address, count, sync, sig_ptr);
#endif
}


__intrinsic
void cmd_cls_write8_be_ptr40(
    __xwrite void *data,
    volatile void __addr40 __cls *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
)
{
    _INTRINSIC_BEGIN;
    {
        _CLS_IMPLEMENT_COMMAND_PTR40(write8_be, INTRINSIC_IND_ALU_COUNT_MAX, 0, data);
    }
    _INTRINSIC_END;
}

__intrinsic
void cmd_cls_write8_le_ptr40(
    __xwrite void *data,
    volatile void __addr40 __cls *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
)
{
    _INTRINSIC_BEGIN;
    {
        _CLS_IMPLEMENT_COMMAND_PTR40(write8_le, INTRINSIC_IND_ALU_COUNT_MAX, 0, data);
    }
    _INTRINSIC_END;
}

__intrinsic
void cmd_cls_write8_ptr40(
    __xwrite void *data,
    volatile void __addr40 __cls *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
)
{
#ifdef __BIGENDIAN
    cmd_cls_write8_be_ptr40(data, address, count, sync, sig_ptr);
#else
    cmd_cls_write8_le_ptr40(data, address, count, sync, sig_ptr);
#endif
}



__intrinsic
void cmd_cls_clear_bits_ind_ptr40(
    __xwrite uint32_t *mask,                            /* mask of bits to clear                  */
    volatile void __addr40 __cls *address,              /* address of words to clear              */
    uint32_t max_nn,                                    /* number of words to clear               */
    generic_ind_t ind,                                  /* indirect word                          */
    sync_t sync,                                        /* type of synchronization to use         */
    SIGNAL *sig_ptr                                     /* signal to raise upon completion        */
    )
{
    _INTRINSIC_BEGIN;
    {
        uint32_t hi_addr, low_addr;

        _INTRINSIC_CONVERT_HI_LO_ADDRESS(address);
        CT_ASSERT(__is_write_reg(mask));
        CT_ASSERT(__is_ct_const(max_nn));
        CT_ASSERT(max_nn > 0 && max_nn <= 32);
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done || sync == ctx_swap);

        __asm local_csr_wr[CMD_INDIRECT_REF_0, CSR_INDIRECT_TYPE(ind)]
        __asm  alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
        if (sync == sig_done)
        {
            __asm cls[clr, *mask, hi_addr, <<8, low_addr, __ct_const_val(max_nn)], sig_done[*sig_ptr], indirect_ref
        }
        else
        {
            __asm cls[clr, *mask, hi_addr, <<8, low_addr, __ct_const_val(max_nn)], ctx_swap[*sig_ptr], indirect_ref
        }
    }
    _INTRINSIC_END;
}


__intrinsic
void cmd_cls_clear_bits_ind_ptr32(
    __xwrite uint32_t *mask,                            /* mask of bits to clear                  */
    volatile void __cls *address,                       /* address of words to clear          */
    uint32_t max_nn,                                    /* number of words to clear           */
    generic_ind_t ind,                                  /* indirect word                          */
    sync_t sync,                                        /* type of synchronization to use         */
    SIGNAL *sig_ptr                                     /* signal to raise upon completion        */
    )
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_write_reg(mask));
        CT_ASSERT(__is_ct_const(max_nn));
        CT_ASSERT(max_nn > 0 && max_nn <= 32);
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done || sync == ctx_swap);

        __asm local_csr_wr[CMD_INDIRECT_REF_0, CSR_INDIRECT_TYPE(ind)]
        __asm  alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
        if (sync == sig_done)
        {
            __asm cls[clr, *mask, address, 0, __ct_const_val(max_nn)], sig_done[*sig_ptr], indirect_ref
        }
        else
        {
            __asm cls[clr, *mask, address, 0, __ct_const_val(max_nn)], ctx_swap[*sig_ptr], indirect_ref
        }
    }
    _INTRINSIC_END;
}



__intrinsic
void cmd_cls_set_bits_ind_ptr40(
    __xwrite uint32_t *mask,                            /* mask of bits to set                    */
    volatile void __addr40 __cls *address,              /* address of words to set            */
    uint32_t max_nn,                                    /* number of words to clear           */
    generic_ind_t ind,                                  /* indirect word                          */
    sync_t sync,                                        /* type of synchronization to use         */
    SIGNAL *sig_ptr                                     /* signal to raise upon completion        */
    )
{
    _INTRINSIC_BEGIN;
    {
        uint32_t hi_addr, low_addr;

        _INTRINSIC_CONVERT_HI_LO_ADDRESS(address);
        CT_ASSERT(__is_write_reg(mask));
        CT_ASSERT(__is_ct_const(max_nn));
        CT_ASSERT(max_nn > 0 && max_nn <= 32);
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done || sync == ctx_swap);

        __asm local_csr_wr[CMD_INDIRECT_REF_0, CSR_INDIRECT_TYPE(ind)]
        __asm  alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
        if (sync == sig_done)
        {
            __asm cls[set, *mask, hi_addr, <<8, low_addr, __ct_const_val(max_nn)], sig_done[*sig_ptr], indirect_ref
        }
        else
        {
            __asm cls[set, *mask, hi_addr, <<8, low_addr, __ct_const_val(max_nn)], ctx_swap[*sig_ptr], indirect_ref
        }
    }
    _INTRINSIC_END;
}



__intrinsic
void cmd_cls_set_bits_ind_ptr32(
    __xwrite uint32_t *mask,                            /* mask of bits to set                    */
    volatile void __cls *address,                       /* address of words to set                */
    uint32_t max_nn,                                    /* number of words to set                 */
    generic_ind_t ind,                                  /* indirect word                          */
    sync_t sync,                                        /* type of synchronization to use         */
    SIGNAL *sig_ptr                                     /* signal to raise upon completion        */
    )
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_write_reg(mask));
        CT_ASSERT(__is_ct_const(max_nn));
        CT_ASSERT(max_nn > 0 && max_nn <= 32);
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done || sync == ctx_swap);

        __asm local_csr_wr[CMD_INDIRECT_REF_0, CSR_INDIRECT_TYPE(ind)]
        __asm  alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
        if (sync == sig_done)
        {
            __asm cls[set, *mask, address, 0, __ct_const_val(max_nn)], sig_done[*sig_ptr], indirect_ref
        }
        else
        {
            __asm cls[set, *mask, address, 0, __ct_const_val(max_nn)], ctx_swap[*sig_ptr], indirect_ref
        }
    }
    _INTRINSIC_END;
}


/* Local scratch memory clear bits        */
void cmd_cls_clear_bits_ptr32(
    __xwrite uint32_t *mask,                            /* mask of bits to clear                  */
    volatile void __cls *address,                       /* address to clear                       */
    uint32_t count,                                     /* number of words to clear               */
    sync_t sync,                                        /* type of synchronization to use         */
    SIGNAL *sig_ptr                                     /* signal to raise upon completion        */
)
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_write_reg(mask));
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done || sync == ctx_swap);

        if (__is_ct_const(count) && count <= 8)
        {
            CT_ASSERT(count != 0);
            if (sync == sig_done)
            {
                __asm cls[clr, *mask, address, 0, __ct_const_val(count)], sig_done[*sig_ptr]
            }
            else
            {
                __asm cls[clr, *mask, address, 0, __ct_const_val(count)], ctx_swap[*sig_ptr]
            }
        }
        else
        {
            if (!__is_ct_const(count))
            {
                CT_QPERFINFO_INDIRECT_REF(cls_clear_bits);
            }
            {
                _INTRINSIC_IND_ALU_COUNT_MIN_MAX(count, 1, 8);
                if (sync == sig_done)
                {
                    __asm cls[clr, *mask, address, 0, __ct_const_val(count)], sig_done[*sig_ptr], indirect_ref
                }
                else
                {
                    __asm cls[clr, *mask, address, 0, __ct_const_val(count)], ctx_swap[*sig_ptr], indirect_ref
                }
            }
        }
    }
    _INTRINSIC_END;
}

/* Local scratch memory set bits          */
void cmd_cls_set_bits_ptr32(
    __xwrite uint32_t *mask,                            /* mask of bits to set                    */
    volatile void __cls *address,                       /* address to set                         */
    uint32_t count,                                     /* number of words to set                 */
    sync_t sync,                                        /* type of synchronization to use         */
    SIGNAL *sig_ptr                                     /* signal to raise upon completion        */
)
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_write_reg(mask));
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done || sync == ctx_swap);

        if (__is_ct_const(count) && count <= 8)
        {
            CT_ASSERT(count != 0);
            if (sync == sig_done)
            {
                __asm cls[set, *mask, address, 0, __ct_const_val(count)], sig_done[*sig_ptr]
            }
            else
            {
                __asm cls[set, *mask, address, 0, __ct_const_val(count)], ctx_swap[*sig_ptr]
            }
        }
        else
        {
            if (!__is_ct_const(count))
            {
                CT_QPERFINFO_INDIRECT_REF(cls_set_bits);
            }
            {
                _INTRINSIC_IND_ALU_COUNT_MIN_MAX(count, 1, 8);
                if (sync == sig_done)
                {
                    __asm cls[set, *mask, address, 0, __ct_const_val(count)], sig_done[*sig_ptr], indirect_ref
                }
                else
                {
                    __asm cls[set, *mask, address, 0, __ct_const_val(count)], ctx_swap[*sig_ptr], indirect_ref
                }
            }
        }
    }
    _INTRINSIC_END;
}


__intrinsic
void cmd_cls_xor_bits_ind_ptr40(
    __xwrite void *mask,                                /* mask of bits to xor                    */
    volatile void __addr40 __cls *address,              /* address to write to                    */
    uint32_t max_nn,                                    /* max. number of words to xor            */
    generic_ind_t ind,                                  /* indirect word                          */
    sync_t sync,                                        /* type of synchronization to use         */
    SIGNAL *sig_ptr                                     /* signal to raise upon completion        */
    )
{
    _INTRINSIC_BEGIN;
    {
        uint32_t hi_addr, low_addr;

        _INTRINSIC_CONVERT_HI_LO_ADDRESS(address);
        CT_ASSERT(__is_write_reg(mask));
        CT_ASSERT(__is_ct_const(max_nn));
        CT_ASSERT(max_nn > 0 && max_nn <= 32);
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done || sync == ctx_swap);

        __asm local_csr_wr[CMD_INDIRECT_REF_0, CSR_INDIRECT_TYPE(ind)]
        __asm  alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
        if (sync == sig_done)
        {
            __asm cls[xor, *mask, hi_addr, <<8, low_addr, __ct_const_val(max_nn)], sig_done[*sig_ptr], indirect_ref
        }
        else
        {
            __asm cls[xor, *mask, hi_addr, <<8, low_addr, __ct_const_val(max_nn)], ctx_swap[*sig_ptr], indirect_ref
        }
    }
    _INTRINSIC_END;
}


__intrinsic
void cmd_cls_xor_bits_ind_ptr32(
    __xwrite void *mask,                                /* mask of bits to xor                    */
    volatile void __cls *address,                       /* address to write to                    */
    uint32_t max_nn,                                    /* max. number of words to xor            */
    generic_ind_t ind,                                  /* indirect word                          */
    sync_t sync,                                        /* type of synchronization to use         */
    SIGNAL *sig_ptr                                     /* signal to raise upon completion        */
    )
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_write_reg(mask));
        CT_ASSERT(__is_ct_const(max_nn));
        CT_ASSERT(max_nn > 0 && max_nn <= 32);
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done || sync == ctx_swap);

        __asm local_csr_wr[CMD_INDIRECT_REF_0, CSR_INDIRECT_TYPE(ind)]
        __asm  alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
        if (sync == sig_done)
        {
            __asm cls[xor, *mask, address, 0, __ct_const_val(max_nn)], sig_done[*sig_ptr], indirect_ref
        }
        else
        {
            __asm cls[xor, *mask, address, 0, __ct_const_val(max_nn)], ctx_swap[*sig_ptr], indirect_ref
        }
    }
    _INTRINSIC_END;
}


/* Local scratch memory XOR bits          */
void cmd_cls_xor_bits_ptr32(
    __xwrite void *mask,                                /* mask of bits to xor                    */
    volatile void __cls *address,                       /* address to xor                         */
    uint32_t count,                                     /* number of words                        */
    sync_t sync,                                        /* type of synchronization to use         */
    SIGNAL *sig_ptr                                     /* signal to raise upon completion        */
)
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_write_reg(mask));
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done || sync == ctx_swap);

        if (__is_ct_const(count) && count <= 8)
        {
            CT_ASSERT(count != 0);
            if (sync == sig_done)
            {
                __asm cls[xor, *mask, address, 0, __ct_const_val(count)], sig_done[*sig_ptr]
            }
            else
            {
                __asm cls[xor, *mask, address, 0, __ct_const_val(count)], ctx_swap[*sig_ptr]
            }
        }
        else
        {
            if (!__is_ct_const(count))
            {
                CT_QPERFINFO_INDIRECT_REF(cls_xor);
            }
            {
                _INTRINSIC_IND_ALU_COUNT_MIN_MAX(count, 1, 8);
                if (sync == sig_done)
                {
                    __asm cls[xor, *mask, address, 0, __ct_const_val(count)], sig_done[*sig_ptr], indirect_ref
                }
                else
                {
                    __asm cls[xor, *mask, address, 0, __ct_const_val(count)], ctx_swap[*sig_ptr], indirect_ref
                }
            }
        }
    }
    _INTRINSIC_END;
}

/* Local scratch memory add               */
void cmd_cls_add_ptr32(
    __xwrite void *data,                                /* data to add                            */
    volatile void __cls *address,                       /* address to add                         */
    uint32_t count,                                     /* number of words                        */
    sync_t sync,                                        /* type of synchronization to use         */
    SIGNAL *sig_ptr                                     /* signal to raise upon completion        */
    )
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_write_reg(data));
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done || sync == ctx_swap);

        if (__is_ct_const(count) && count <= 8)
        {
            CT_ASSERT(count != 0);
            if (sync == sig_done)
            {
                __asm cls[add, *data, address, 0, __ct_const_val(count)], sig_done[*sig_ptr]
            }
            else
            {
                __asm cls[add, *data, address, 0, __ct_const_val(count)], ctx_swap[*sig_ptr]
            }
        }
        else
        {
            if (!__is_ct_const(count))
            {
                CT_QPERFINFO_INDIRECT_REF(cls_add);
            }
            {
                _INTRINSIC_IND_ALU_COUNT_MIN_MAX(count, 1, 8);
                if (sync == sig_done)
                {
                    __asm cls[add, *data, address, 0, __ct_const_val(count)], sig_done[*sig_ptr], indirect_ref
                }
                else
                {
                    __asm cls[add, *data, address, 0, __ct_const_val(count)], ctx_swap[*sig_ptr], indirect_ref
                }
            }
        }
    }
    _INTRINSIC_END;
}

/* Local scratch memory add saturates     */

void cmd_cls_add_sat_ptr32(
    __xwrite void *data,                                /* data to add                            */
    volatile void __cls *address,                       /* address to add                         */
    uint32_t count,                                     /* number of words                        */
    sync_t sync,                                        /* type of synchronization to use         */
    SIGNAL *sig_ptr                                     /* signal to raise upon completion        */
    )
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_write_reg(data));
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done || sync == ctx_swap);

        if (__is_ct_const(count) && count <= 8)
        {
            CT_ASSERT(count != 0);
            if (sync == sig_done)
            {
                __asm cls[addsat, *data, address, 0, __ct_const_val(count)], sig_done[*sig_ptr]
            }
            else
            {
                __asm cls[addsat, *data, address, 0, __ct_const_val(count)], ctx_swap[*sig_ptr]
            }
        }
        else
        {
            if (!__is_ct_const(count))
            {
                CT_QPERFINFO_INDIRECT_REF(cls_add_sat);
            }
            {
                _INTRINSIC_IND_ALU_COUNT_MIN_MAX(count, 1, 8);
                if (sync == sig_done)
                {
                    __asm cls[addsat, *data, address, 0, __ct_const_val(count)], sig_done[*sig_ptr], indirect_ref
                }
                else
                {
                    __asm cls[addsat, *data, address, 0, __ct_const_val(count)], ctx_swap[*sig_ptr], indirect_ref
                }
            }
        }
    }
    _INTRINSIC_END;
}


__intrinsic
void cmd_cls_add_ind_ptr40(
    __xwrite void *data,                                /* data to add                            */
    volatile void __addr40 __cls *address,              /* address to add to                      */
    uint32_t max_nn,                                    /* max. number of words to add            */
    generic_ind_t ind,                                  /* indirect word                          */
    sync_t sync,                                        /* type of synchronization to use         */
    SIGNAL *sig_ptr                                     /* signal to raise upon completion        */
    )
{
    _INTRINSIC_BEGIN;
    {
        uint32_t hi_addr, low_addr;

        _INTRINSIC_CONVERT_HI_LO_ADDRESS(address);
        CT_ASSERT(__is_write_reg(data));
        CT_ASSERT(__is_ct_const(max_nn));
        CT_ASSERT(max_nn > 0 && max_nn <= 32);
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done || sync == ctx_swap);

        __asm local_csr_wr[CMD_INDIRECT_REF_0, CSR_INDIRECT_TYPE(ind)]
        __asm  alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
        if (sync == sig_done)
        {
            __asm cls[add, *data, hi_addr, <<8, low_addr, __ct_const_val(max_nn)], sig_done[*sig_ptr], indirect_ref
        }
        else
        {
            __asm cls[add, *data, hi_addr, <<8, low_addr, __ct_const_val(max_nn)], ctx_swap[*sig_ptr], indirect_ref
        }
    }
    _INTRINSIC_END;
}


__intrinsic
void cmd_cls_add_ind_ptr32(
    __xwrite void *data,                                /* data to add                            */
    volatile void __cls *address,                       /* address to add to                      */
    uint32_t max_nn,                                    /* max. number of words to add            */
    generic_ind_t ind,                                  /* indirect word                          */
    sync_t sync,                                        /* type of synchronization to use         */
    SIGNAL *sig_ptr                                     /* signal to raise upon completion        */
    )
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_write_reg(data));
        CT_ASSERT(__is_ct_const(max_nn));
        CT_ASSERT(max_nn > 0 && max_nn <= 32);
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done || sync == ctx_swap);

        __asm local_csr_wr[CMD_INDIRECT_REF_0, CSR_INDIRECT_TYPE(ind)]
        __asm  alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
        if (sync == sig_done)
        {
            __asm cls[add, *data, address, 0, __ct_const_val(max_nn)], sig_done[*sig_ptr], indirect_ref
        }
        else
        {
            __asm cls[add, *data, address, 0, __ct_const_val(max_nn)], ctx_swap[*sig_ptr], indirect_ref
        }
    }
    _INTRINSIC_END;
}


// add64 ind  ptr40/ptr32
__intrinsic
void cmd_cls_add64_ind_ptr40(
    __xwrite void *data,                                /* data to add                            */
    volatile void __addr40 __cls *address,              /* address to add to                      */
    uint32_t max_nn,                                    /* max. number of words to add            */
    generic_ind_t ind,                                  /* indirect word                          */
    sync_t sync,                                        /* type of synchronization to use         */
    SIGNAL *sig_ptr                                     /* signal to raise upon completion        */
    )
{
    _INTRINSIC_BEGIN;
    {
        uint32_t hi_addr, low_addr;

        _INTRINSIC_CONVERT_HI_LO_ADDRESS(address);
        CT_ASSERT(__is_write_reg(data));
        CT_ASSERT(__is_ct_const(max_nn));
        CT_ASSERT(max_nn >= 2 && max_nn <= 16);
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done || sync == ctx_swap);

        __asm local_csr_wr[CMD_INDIRECT_REF_0, CSR_INDIRECT_TYPE(ind)]
        __asm  alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
        if (sync == sig_done)
        {
            __asm cls[add64, *data, hi_addr, <<8, low_addr, __ct_const_val(max_nn)], sig_done[*sig_ptr], indirect_ref
        }
        else
        {
            __asm cls[add64, *data, hi_addr, <<8, low_addr, __ct_const_val(max_nn)], ctx_swap[*sig_ptr], indirect_ref
        }
    }
    _INTRINSIC_END;
}


__intrinsic
void cmd_cls_add64_ind_ptr32(
    __xwrite void *data,                                /* data to add                            */
    volatile void __cls *address,                       /* address to add to                      */
    uint32_t max_nn,                                    /* max. number of words to add            */
    generic_ind_t ind,                                  /* indirect word                          */
    sync_t sync,                                        /* type of synchronization to use         */
    SIGNAL *sig_ptr                                     /* signal to raise upon completion        */
    )
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_write_reg(data));
        CT_ASSERT(__is_ct_const(max_nn));
        CT_ASSERT(max_nn >= 2 && max_nn <= 16);
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done || sync == ctx_swap);

        __asm local_csr_wr[CMD_INDIRECT_REF_0, CSR_INDIRECT_TYPE(ind)]
        __asm  alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
        if (sync == sig_done)
        {
            __asm cls[add64, *data, address, 0, __ct_const_val(max_nn)], sig_done[*sig_ptr], indirect_ref
        }
        else
        {
            __asm cls[add64, *data, address, 0, __ct_const_val(max_nn)], ctx_swap[*sig_ptr], indirect_ref
        }
    }
    _INTRINSIC_END;
}



// add sat ind  ptr40/ptr32
__intrinsic
void cmd_cls_add_sat_ind_ptr40(
    __xwrite void *data,                                /* data to add                            */
    volatile void __addr40 __cls *address,              /* address to add to                      */
    uint32_t max_nn,                                    /* max. number of words to add            */
    generic_ind_t ind,                                  /* indirect word                          */
    sync_t sync,                                        /* type of synchronization to use         */
    SIGNAL *sig_ptr                                     /* signal to raise upon completion        */
    )
{
    _INTRINSIC_BEGIN;
    {
        uint32_t hi_addr, low_addr;

        _INTRINSIC_CONVERT_HI_LO_ADDRESS(address);
        CT_ASSERT(__is_write_reg(data));
        CT_ASSERT(__is_ct_const(max_nn));
        CT_ASSERT(max_nn > 0 && max_nn <= 32);
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done || sync == ctx_swap);

        __asm local_csr_wr[CMD_INDIRECT_REF_0, CSR_INDIRECT_TYPE(ind)]
        __asm  alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
        if (sync == sig_done)
        {
            __asm cls[addsat, *data, hi_addr, <<8, low_addr, __ct_const_val(max_nn)], sig_done[*sig_ptr], indirect_ref
        }
        else
        {
            __asm cls[addsat, *data, hi_addr, <<8, low_addr, __ct_const_val(max_nn)], ctx_swap[*sig_ptr], indirect_ref
        }
    }
    _INTRINSIC_END;
}


__intrinsic
void cmd_cls_add_sat_ind_ptr32(
    __xwrite void *data,                                /* data to add                            */
    volatile void __cls *address,                       /* address to add to                      */
    uint32_t max_nn,                                    /* max. number of words to add            */
    generic_ind_t ind,                                  /* indirect word                          */
    sync_t sync,                                        /* type of synchronization to use         */
    SIGNAL *sig_ptr                                     /* signal to raise upon completion        */
    )
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_write_reg(data));
        CT_ASSERT(__is_ct_const(max_nn));
        CT_ASSERT(max_nn > 0 && max_nn <= 32);
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done || sync == ctx_swap);

        __asm local_csr_wr[CMD_INDIRECT_REF_0, CSR_INDIRECT_TYPE(ind)]
        __asm  alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
        if (sync == sig_done)
        {
            __asm cls[add, *data, address, 0, __ct_const_val(max_nn)], sig_done[*sig_ptr], indirect_ref
        }
        else
        {
            __asm cls[add, *data, address, 0, __ct_const_val(max_nn)], ctx_swap[*sig_ptr], indirect_ref
        }
    }
    _INTRINSIC_END;
}


__intrinsic
void cmd_cls_sub_ind_ptr40(
    __xwrite void *data,                                /* data to subtract                       */
    volatile void __addr40 __cls *address,              /* address to subtract to                 */
    uint32_t max_nn,                                    /* max. number of words to subtract       */
    generic_ind_t ind,                                  /* indirect word                          */
    sync_t sync,                                        /* type of synchronization to use         */
    SIGNAL *sig_ptr                                     /* signal to raise upon completion        */
    )
{
    _INTRINSIC_BEGIN;
    {
        uint32_t hi_addr, low_addr;

        _INTRINSIC_CONVERT_HI_LO_ADDRESS(address);
        CT_ASSERT(__is_write_reg(data));
        CT_ASSERT(__is_ct_const(max_nn));
        CT_ASSERT(max_nn > 0 && max_nn <= 32);
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done || sync == ctx_swap);

        __asm local_csr_wr[CMD_INDIRECT_REF_0, CSR_INDIRECT_TYPE(ind)]
        __asm  alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
        if (sync == sig_done)
        {
            __asm cls[sub, *data, hi_addr, <<8, low_addr, __ct_const_val(max_nn)], sig_done[*sig_ptr], indirect_ref
        }
        else
        {
            __asm cls[sub, *data, hi_addr, <<8, low_addr, __ct_const_val(max_nn)], ctx_swap[*sig_ptr], indirect_ref
        }
    }
    _INTRINSIC_END;
}


__intrinsic
void cmd_cls_sub_ind_ptr32(
    __xwrite void *data,                                /* data to subtract                       */
    volatile void __cls *address,                       /* address to subtract to                 */
    uint32_t max_nn,                                    /* max. number of words to subtract       */
    generic_ind_t ind,                                  /* indirect word                          */
    sync_t sync,                                        /* type of synchronization to use         */
    SIGNAL *sig_ptr                                     /* signal to raise upon completion        */
    )
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_write_reg(data));
        CT_ASSERT(__is_ct_const(max_nn));
        CT_ASSERT(max_nn > 0 && max_nn <= 32);
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done || sync == ctx_swap);

        __asm local_csr_wr[CMD_INDIRECT_REF_0, CSR_INDIRECT_TYPE(ind)]
        __asm  alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
        if (sync == sig_done)
        {
            __asm cls[sub, *data, address, 0, __ct_const_val(max_nn)], sig_done[*sig_ptr], indirect_ref
        }
        else
        {
            __asm cls[sub, *data, address, 0, __ct_const_val(max_nn)], ctx_swap[*sig_ptr], indirect_ref
        }
    }
    _INTRINSIC_END;
}


__intrinsic
void cmd_cls_sub64_ind_ptr40(
    __xwrite void *data,                                /* data to subtract                       */
    volatile void __addr40 __cls *address,              /* address to subtract to                 */
    uint32_t max_nn,                                    /* max. number of words to subtract       */
    generic_ind_t ind,                                  /* indirect word                          */
    sync_t sync,                                        /* type of synchronization to use         */
    SIGNAL *sig_ptr                                     /* signal to raise upon completion        */
    )
{
    _INTRINSIC_BEGIN;
    {
        uint32_t hi_addr, low_addr;

        _INTRINSIC_CONVERT_HI_LO_ADDRESS(address);
        CT_ASSERT(__is_write_reg(data));
        CT_ASSERT(__is_ct_const(max_nn));
        CT_ASSERT(max_nn >= 2 && max_nn <= 16);
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done || sync == ctx_swap);

        __asm local_csr_wr[CMD_INDIRECT_REF_0, CSR_INDIRECT_TYPE(ind)]
        __asm  alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
        if (sync == sig_done)
        {
            __asm cls[sub64, *data, hi_addr, <<8, low_addr, __ct_const_val(max_nn)], sig_done[*sig_ptr], indirect_ref
        }
        else
        {
            __asm cls[sub64, *data, hi_addr, <<8, low_addr, __ct_const_val(max_nn)], ctx_swap[*sig_ptr], indirect_ref
        }
    }
    _INTRINSIC_END;
}


__intrinsic
void cmd_cls_sub64_ind_ptr32(
    __xwrite void *data,                                /* data to subtract                       */
    volatile void __cls *address,                       /* address to subtract to                 */
    uint32_t max_nn,                                    /* max. number of words to subtract       */
    generic_ind_t ind,                                  /* indirect word                          */
    sync_t sync,                                        /* type of synchronization to use         */
    SIGNAL *sig_ptr                                     /* signal to raise upon completion        */
    )
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_write_reg(data));
        CT_ASSERT(__is_ct_const(max_nn));
        CT_ASSERT(max_nn >= 2 && max_nn <= 16);
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done || sync == ctx_swap);

        __asm local_csr_wr[CMD_INDIRECT_REF_0, CSR_INDIRECT_TYPE(ind)]
        __asm  alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
        if (sync == sig_done)
        {
            __asm cls[sub64, *data, address, 0, __ct_const_val(max_nn)], sig_done[*sig_ptr], indirect_ref
        }
        else
        {
            __asm cls[sub64, *data, address, 0, __ct_const_val(max_nn)], ctx_swap[*sig_ptr], indirect_ref
        }
    }
    _INTRINSIC_END;
}


__intrinsic
void cmd_cls_sub_sat_ind_ptr40(
    __xwrite void *data,                                /* data to subtract                       */
    volatile void __addr40 __cls *address,              /* address to subtract to                 */
    uint32_t max_nn,                                    /* max. number of words to subtract       */
    generic_ind_t ind,                                  /* indirect word                          */
    sync_t sync,                                        /* type of synchronization to use         */
    SIGNAL *sig_ptr                                     /* signal to raise upon completion        */
    )
{
    _INTRINSIC_BEGIN;
    {
        uint32_t hi_addr, low_addr;

        _INTRINSIC_CONVERT_HI_LO_ADDRESS(address);
        CT_ASSERT(__is_write_reg(data));
        CT_ASSERT(__is_ct_const(max_nn));
        CT_ASSERT(max_nn > 0 && max_nn <= 32);
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done || sync == ctx_swap);

        __asm local_csr_wr[CMD_INDIRECT_REF_0, CSR_INDIRECT_TYPE(ind)]
        __asm  alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
        if (sync == sig_done)
        {
            __asm cls[subsat, *data, hi_addr, <<8, low_addr, __ct_const_val(max_nn)], sig_done[*sig_ptr], indirect_ref
        }
        else
        {
            __asm cls[subsat, *data, hi_addr, <<8, low_addr, __ct_const_val(max_nn)], ctx_swap[*sig_ptr], indirect_ref
        }
    }
    _INTRINSIC_END;
}


__intrinsic
void cmd_cls_sub_sat_ind_ptr32(
    __xwrite void *data,                                /* data to subtract                       */
    volatile void __cls *address,                       /* address to subtract to                 */
    uint32_t max_nn,                                    /* max. number of words to subtract       */
    generic_ind_t ind,                                  /* indirect word                          */
    sync_t sync,                                        /* type of synchronization to use         */
    SIGNAL *sig_ptr                                     /* signal to raise upon completion        */
    )
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_write_reg(data));
        CT_ASSERT(__is_ct_const(max_nn));
        CT_ASSERT(max_nn > 0 && max_nn <= 32);
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done || sync == ctx_swap);

        __asm local_csr_wr[CMD_INDIRECT_REF_0, CSR_INDIRECT_TYPE(ind)]
        __asm  alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
        if (sync == sig_done)
        {
            __asm cls[subsat, *data, address, 0, __ct_const_val(max_nn)], sig_done[*sig_ptr], indirect_ref
        }
        else
        {
            __asm cls[subsat, *data, address, 0, __ct_const_val(max_nn)], ctx_swap[*sig_ptr], indirect_ref
        }
    }
    _INTRINSIC_END;
}


/* Local scratch memory sub               */
void cmd_cls_sub_ptr32(
    __xwrite void *data,                                /* data to sub                            */
    volatile void __cls *address,                       /* address to sub                         */
    uint32_t count,                                     /* number of words                        */
    sync_t sync,                                        /* type of synchronization to use         */
    SIGNAL *sig_ptr                                     /* signal to raise upon completion        */
)
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_write_reg(data));
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done || sync == ctx_swap);

        if (__is_ct_const(count) && count <= 8)
        {
            CT_ASSERT(count != 0);
            if (sync == sig_done)
            {
                __asm cls[sub, *data, address, 0, __ct_const_val(count)], sig_done[*sig_ptr]
            }
            else
            {
                __asm cls[sub, *data, address, 0, __ct_const_val(count)], ctx_swap[*sig_ptr]
            }
        }
        else
        {
            if (!__is_ct_const(count))
            {
                CT_QPERFINFO_INDIRECT_REF(cls_sub);
            }
            {
                _INTRINSIC_IND_ALU_COUNT_MIN_MAX(count, 1, 8);
                if (sync == sig_done)
                {
                    __asm cls[sub, *data, address, 0, __ct_const_val(count)], sig_done[*sig_ptr], indirect_ref
                }
                else
                {
                    __asm cls[sub, *data, address, 0, __ct_const_val(count)], ctx_swap[*sig_ptr], indirect_ref
                }
            }
        }

    }
    _INTRINSIC_END;
}


/* Local scratch memory sub saturates     */
void cmd_cls_sub_sat_ptr32(
    __xwrite void *data,                                /* data to sub                            */
    volatile void __cls *address,                       /* address to sub                         */
    uint32_t count,                                     /* number of words                        */
    sync_t sync,                                        /* type of synchronization to use         */
    SIGNAL *sig_ptr                                     /* signal to raise upon completion        */
)
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_write_reg(data));
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done || sync == ctx_swap);

        if (__is_ct_const(count) && count <= 8)
        {
            CT_ASSERT(count != 0);
            if (sync == sig_done)
            {
                __asm cls[subsat, *data, address, 0, __ct_const_val(count)], sig_done[*sig_ptr]
            }
            else
            {
                __asm cls[subsat, *data, address, 0, __ct_const_val(count)], ctx_swap[*sig_ptr]
            }
        }
        else
        {
            if (!__is_ct_const(count))
            {
                CT_QPERFINFO_INDIRECT_REF(cls_sub_sat);
            }
            {
                _INTRINSIC_IND_ALU_COUNT_MIN_MAX(count, 1, 8);
                if (sync == sig_done)
                {
                    __asm cls[subsat, *data, address, 0, __ct_const_val(count)], sig_done[*sig_ptr], indirect_ref
                }
                else
                {
                    __asm cls[subsat, *data, address, 0, __ct_const_val(count)], ctx_swap[*sig_ptr], indirect_ref
                }
            }
        }
    }
    _INTRINSIC_END;
}

void cmd_cls_clear_bits_ptr40(
    __xwrite uint32_t *mask,
    volatile void __addr40 __cls *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
)
{
    _INTRINSIC_BEGIN;
    {
        _CLS_IMPLEMENT_COMMAND_PTR40(clr, 8, 0, mask);
    }
    _INTRINSIC_END;
}


void cmd_cls_set_bits_ptr40(
    __xwrite uint32_t *mask,
    volatile void __addr40 __cls *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
)
{
    _INTRINSIC_BEGIN;
    {
        _CLS_IMPLEMENT_COMMAND_PTR40(set, 8, 0, mask);
    }
    _INTRINSIC_END;
}

void cmd_cls_xor_bits_ptr40(
    __xwrite void *mask,
    volatile void __addr40 __cls *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
)
{
    _INTRINSIC_BEGIN;
    {
        _CLS_IMPLEMENT_COMMAND_PTR40(xor, 8, 0, mask);
    }
    _INTRINSIC_END;
}


void cmd_cls_add_ptr40(
    __xwrite void *data,
    volatile void __addr40 __cls *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
)
{
    _INTRINSIC_BEGIN;
    {
        _CLS_IMPLEMENT_COMMAND_PTR40(add, 8, 0, data);
    }
    _INTRINSIC_END;
}


void cmd_cls_add_sat_ptr40(
    __xwrite void *data,
    volatile void __addr40 __cls *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
)
{
    _INTRINSIC_BEGIN;
    {
        _CLS_IMPLEMENT_COMMAND_PTR40(addsat, 8, 0, data);
    }
    _INTRINSIC_END;
}


void cmd_cls_sub_ptr40(
    __xwrite void *data,
    volatile void __addr40 __cls *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
)
{
    _INTRINSIC_BEGIN;
    {
        _CLS_IMPLEMENT_COMMAND_PTR40(sub, 8, 0, data);
    }
    _INTRINSIC_END;
}


void cmd_cls_sub_sat_ptr40(
    __xwrite void *data,
    volatile void __addr40 __cls *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
)
{
    _INTRINSIC_BEGIN;
    {
        _CLS_IMPLEMENT_COMMAND_PTR40(subsat, 8, 0, data);
    }
    _INTRINSIC_END;
}


__intrinsic
void cmd_cls_read_le_ind_ptr40(
    __xread void *data,                                 /* data to read                           */
    volatile void __addr40 __cls *address,              /* address to read from                   */
    uint32_t max_nn,                                    /* max. number of words to read       */
    generic_ind_t ind,                                  /* indirect word                          */
    sync_t sync,                                        /* type of synchronization to use         */
    SIGNAL *sig_ptr                                     /* signal to raise upon completion        */
    )
{
    _INTRINSIC_BEGIN;
    {
        uint32_t hi_addr, low_addr;

        _INTRINSIC_CONVERT_HI_LO_ADDRESS(address);
        CT_ASSERT(__is_read_reg(data));
        CT_ASSERT(__is_ct_const(max_nn));
        CT_ASSERT(max_nn > 0 && max_nn <= 32);
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done || sync == ctx_swap);

        __asm local_csr_wr[CMD_INDIRECT_REF_0, CSR_INDIRECT_TYPE(ind)]
        __asm  alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
        if (sync == sig_done)
        {
            __asm cls[read_le, *data, hi_addr, <<8, low_addr, __ct_const_val(max_nn)], sig_done[*sig_ptr], indirect_ref
        }
        else
        {
            __asm cls[read_le, *data, hi_addr, <<8, low_addr, __ct_const_val(max_nn)], ctx_swap[*sig_ptr], indirect_ref
        }
    }
    _INTRINSIC_END;
}


__intrinsic
void cmd_cls_read_le_ind_ptr32(
    __xread void *data,                                 /* data to read                           */
    volatile void __cls *address,                       /* address to read from                   */
    uint32_t max_nn,                                    /* max. number of words to read       */
    generic_ind_t ind,                                  /* indirect word                          */
    sync_t sync,                                        /* type of synchronization to use         */
    SIGNAL *sig_ptr                                     /* signal to raise upon completion        */
    )
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_read_reg(data));
        CT_ASSERT(__is_ct_const(max_nn));
        CT_ASSERT(max_nn > 0 && max_nn <= 32);
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done || sync == ctx_swap);

        __asm local_csr_wr[CMD_INDIRECT_REF_0, CSR_INDIRECT_TYPE(ind)]
        __asm  alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
        if (sync == sig_done)
        {
            __asm cls[read_le, *data, address, 0, __ct_const_val(max_nn)], sig_done[*sig_ptr], indirect_ref
        }
        else
        {
            __asm cls[read_le, *data, address, 0, __ct_const_val(max_nn)], ctx_swap[*sig_ptr], indirect_ref
        }
    }
    _INTRINSIC_END;
}


__intrinsic
void cmd_cls_read_be_ind_ptr40(
    __xread void *data,                                 /* data to read                           */
    volatile void __addr40 __cls *address,              /* address to read from                   */
    uint32_t max_nn,                                    /* max. number of words to read           */
    generic_ind_t ind,                                  /* indirect word                          */
    sync_t sync,                                        /* type of synchronization to use         */
    SIGNAL *sig_ptr                                     /* signal to raise upon completion        */
    )
{
    _INTRINSIC_BEGIN;
    {
        uint32_t hi_addr, low_addr;

        _INTRINSIC_CONVERT_HI_LO_ADDRESS(address);
        CT_ASSERT(__is_read_reg(data));
        CT_ASSERT(__is_ct_const(max_nn));
        CT_ASSERT(max_nn > 0 && max_nn <= 32);
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done || sync == ctx_swap);

        __asm local_csr_wr[CMD_INDIRECT_REF_0, CSR_INDIRECT_TYPE(ind)]
        __asm  alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
        if (sync == sig_done)
        {
            __asm cls[read_be, *data, hi_addr, <<8, low_addr, __ct_const_val(max_nn)], sig_done[*sig_ptr], indirect_ref
        }
        else
        {
            __asm cls[read_be, *data, hi_addr, <<8, low_addr, __ct_const_val(max_nn)], ctx_swap[*sig_ptr], indirect_ref
        }
    }
    _INTRINSIC_END;
}

__intrinsic
void cmd_cls_read_be_ind_ptr32(
    __xread void *data,                                 /* data to read                           */
    volatile void __cls *address,                       /* address to read from                   */
    uint32_t max_nn,                                    /* max. number of words to read       */
    generic_ind_t ind,                                  /* indirect word                          */
    sync_t sync,                                        /* type of synchronization to use         */
    SIGNAL *sig_ptr                                     /* signal to raise upon completion        */
    )
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_read_reg(data));
        CT_ASSERT(__is_ct_const(max_nn));
        CT_ASSERT(max_nn > 0 && max_nn <= 32);
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done || sync == ctx_swap);

        __asm local_csr_wr[CMD_INDIRECT_REF_0, CSR_INDIRECT_TYPE(ind)]
        __asm  alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
        if (sync == sig_done)
        {
            __asm cls[read_be, *data, address, 0, __ct_const_val(max_nn)], sig_done[*sig_ptr], indirect_ref
        }
        else
        {
            __asm cls[read_be, *data, address, 0, __ct_const_val(max_nn)], ctx_swap[*sig_ptr], indirect_ref
        }
    }
    _INTRINSIC_END;
}


void cmd_cls_read_ind_ptr40(
    __xread void *data,                                 /* data to read                           */
    volatile void __addr40 __cls *address,              /* address to read from                   */
    uint32_t max_nn,                                    /* number of words to read            */
    generic_ind_t ind,                                  /* indirect word                          */
    sync_t sync,                                        /* type of synchronization to use         */
    SIGNAL *sig_ptr                                     /* signal to raise upon completion        */
    )
{
#ifdef __BIGENDIAN
    cmd_cls_read_be_ind_ptr40(data, address, max_nn, ind, sync, sig_ptr);
#else
    cmd_cls_read_le_ind_ptr40(data, address, max_nn, ind, sync, sig_ptr);
#endif
}



void cmd_cls_read_ind_ptr32(
    __xread void *data,                                 /* data to read                           */
    volatile void __cls *address,                       /* address to read from                   */
    uint32_t max_nn,                                    /* number of words to read            */
    generic_ind_t ind,                                  /* indirect word                          */
    sync_t sync,                                        /* type of synchronization to use         */
    SIGNAL *sig_ptr                                     /* signal to raise upon completion        */
    )
{
#ifdef __BIGENDIAN
    cmd_cls_read_be_ind_ptr32(data, address, max_nn, ind, sync, sig_ptr);
#else
    cmd_cls_read_le_ind_ptr32(data, address, max_nn, ind, sync, sig_ptr);
#endif
}


__intrinsic
void cmd_cls_write_le_ind_ptr40(
    __xwrite void *data,                                /* data to write                          */
    volatile void __addr40 __cls *address,              /* address to write to                    */
    uint32_t max_nn,                                    /* max. number of words to write      */
    generic_ind_t ind,                                  /* indirect word                          */
    sync_t sync,                                        /* type of synchronization to use         */
    SIGNAL *sig_ptr                                     /* signal to raise upon completion        */
    )
{
    _INTRINSIC_BEGIN;
    {
        uint32_t hi_addr, low_addr;

        _INTRINSIC_CONVERT_HI_LO_ADDRESS(address);
        CT_ASSERT(__is_write_reg(data));
        CT_ASSERT(__is_ct_const(max_nn));
        CT_ASSERT(max_nn > 0 && max_nn <= 32);
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done || sync == ctx_swap);

        __asm local_csr_wr[CMD_INDIRECT_REF_0, CSR_INDIRECT_TYPE(ind)]
        __asm  alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
        if (sync == sig_done)
        {
            __asm cls[write_le, *data, hi_addr, <<8, low_addr, __ct_const_val(max_nn)], sig_done[*sig_ptr], indirect_ref
        }
        else
        {
            __asm cls[write_le, *data, hi_addr, <<8, low_addr, __ct_const_val(max_nn)], ctx_swap[*sig_ptr], indirect_ref
        }
    }
    _INTRINSIC_END;
}


__intrinsic
void cmd_cls_write_le_ind_ptr32(
    __xwrite void *data,                                /* data to write                          */
    volatile void __cls *address,                       /* address to write to                    */
    uint32_t max_nn,                                    /* max. number of words to write      */
    generic_ind_t ind,                                  /* indirect word                          */
    sync_t sync,                                        /* type of synchronization to use         */
    SIGNAL *sig_ptr                                     /* signal to raise upon completion        */
    )
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_write_reg(data));
        CT_ASSERT(__is_ct_const(max_nn));
        CT_ASSERT(max_nn > 0 && max_nn <= 32);
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done || sync == ctx_swap);

        __asm local_csr_wr[CMD_INDIRECT_REF_0, CSR_INDIRECT_TYPE(ind)]
        __asm  alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
        if (sync == sig_done)
        {
            __asm cls[write_le, *data, address, 0, __ct_const_val(max_nn)], sig_done[*sig_ptr], indirect_ref
        }
        else
        {
            __asm cls[write_le, *data, address, 0, __ct_const_val(max_nn)], ctx_swap[*sig_ptr], indirect_ref
        }
    }
    _INTRINSIC_END;
}


__intrinsic
void cmd_cls_write_be_ind_ptr40(
    __xwrite void *data,                                /* data to write                          */
    volatile void __addr40 __cls *address,              /* address to write to                    */
    uint32_t max_nn,                                    /* max. number of words to write      */
    generic_ind_t ind,                                  /* indirect word                          */
    sync_t sync,                                        /* type of synchronization to use         */
    SIGNAL *sig_ptr                                     /* signal to raise upon completion        */
    )
{
    _INTRINSIC_BEGIN;
    {
        uint32_t hi_addr, low_addr;

        _INTRINSIC_CONVERT_HI_LO_ADDRESS(address);
        CT_ASSERT(__is_write_reg(data));
        CT_ASSERT(__is_ct_const(max_nn));
        CT_ASSERT(max_nn > 0 && max_nn <= 32);
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done || sync == ctx_swap);


        __asm local_csr_wr[CMD_INDIRECT_REF_0, CSR_INDIRECT_TYPE(ind)]
        __asm  alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
        if (sync == sig_done)
        {
            __asm cls[write_be, *data, hi_addr, <<8, low_addr, __ct_const_val(max_nn)], sig_done[*sig_ptr], indirect_ref
        }
        else
        {
            __asm cls[write_be, *data, hi_addr, <<8, low_addr, __ct_const_val(max_nn)], ctx_swap[*sig_ptr], indirect_ref
        }
    }
    _INTRINSIC_END;
}


__intrinsic
void cmd_cls_write_be_ind_ptr32(
    __xwrite void *data,                                /* data to write                          */
    volatile void __cls *address,                       /* address to write to                    */
    uint32_t max_nn,                                    /* max. number of words to write      */
    generic_ind_t ind,                                  /* indirect word                          */
    sync_t sync,                                        /* type of synchronization to use         */
    SIGNAL *sig_ptr                                     /* signal to raise upon completion        */
    )
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_write_reg(data));
        CT_ASSERT(__is_ct_const(max_nn));
        CT_ASSERT(max_nn > 0 && max_nn <= 32);
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done || sync == ctx_swap);

        __asm local_csr_wr[CMD_INDIRECT_REF_0, CSR_INDIRECT_TYPE(ind)]
        __asm  alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
        if (sync == sig_done)
        {
            __asm cls[write_be, *data, address, 0, __ct_const_val(max_nn)], sig_done[*sig_ptr], indirect_ref
        }
        else
        {
            __asm cls[write_be, *data, address, 0, __ct_const_val(max_nn)], ctx_swap[*sig_ptr], indirect_ref
        }
    }
    _INTRINSIC_END;
}



void cmd_cls_write_ind_ptr32(
    __xwrite void *data,                                /* data to write                          */
    volatile void __cls *address,                       /* address to write to                    */
    uint32_t max_nn,                                    /* number of words to write               */
    generic_ind_t ind,                                  /* indirect word                          */
    sync_t sync,                                        /* type of synchronization to use         */
    SIGNAL *sig_ptr                                     /* signal to raise upon completion        */
    )
{
#ifdef __BIGENDIAN
    cmd_cls_write_be_ind_ptr32(data, address, max_nn, ind, sync, sig_ptr);
#else
    cmd_cls_write_le_ind_ptr32(data, address, max_nn, ind, sync, sig_ptr);
#endif
}


void cmd_cls_write_ind_ptr40(
    __xwrite void *data,                                /* data to write                          */
    volatile void __addr40 __cls *address,              /* address to write to                    */
    uint32_t max_nn,                                    /* number of words to read                */
    generic_ind_t ind,                                  /* indirect word                          */
    sync_t sync,                                        /* type of synchronization to use         */
    SIGNAL *sig_ptr                                     /* signal to raise upon completion        */
    )
{
#ifdef __BIGENDIAN
    cmd_cls_write_be_ind_ptr40(data, address, max_nn, ind, sync, sig_ptr);
#else
    cmd_cls_write_le_ind_ptr40(data, address, max_nn, ind, sync, sig_ptr);
#endif
}


/* Local scratch memory set bits immediate */
void cmd_cls_set_bits_imm_ptr32(
    volatile void __cls *address,                       /* address to set                         */
    uint32_t value                                      /* values to set                          */
)
{
    _INTRINSIC_BEGIN;
    {
        _CLS_IMPLEMENT_IMM_COMMAND(set_imm);
    }
    _INTRINSIC_END;
}


void cmd_cls_set_bits_imm_ptr40(
    volatile void __addr40 __cls *address,
    uint32_t value                                  /* values to set                          */
)
{
    _INTRINSIC_BEGIN;
    {
        _CLS_IMPLEMENT_IMM_COMMAND_PTR40(set_imm);
    }
    _INTRINSIC_END;
}


/* Local scratch memory clear bits immediate  */
void cmd_cls_clear_bits_imm_ptr32(
    volatile void __cls *address,                       /* address to clear                       */
    uint32_t value                                      /* immediate value                        */
)
{
    _INTRINSIC_BEGIN;
    {
        _CLS_IMPLEMENT_IMM_COMMAND(clr_imm);
    }
    _INTRINSIC_END;
}


__intrinsic
void cmd_cls_clear_bits_imm_ptr40(
    volatile void __addr40 __cls *address,
    uint32_t value
)
{
    _INTRINSIC_BEGIN;
    {
        _CLS_IMPLEMENT_IMM_COMMAND_PTR40(clr_imm);
    }
    _INTRINSIC_END;
}


/* Local scratch memory add immediate     */
void cmd_cls_add_imm_ptr32(
    volatile void __cls *address,                       /* address to add                         */
    uint32_t value                                      /* immediate value                        */
)
{
    _INTRINSIC_BEGIN;
    {
        _CLS_IMPLEMENT_IMM_COMMAND(add_imm);
    }
    _INTRINSIC_END;
}




void cmd_cls_add_imm_ptr40(
    volatile void __addr40 __cls *address,
    uint32_t value
)
{
    _INTRINSIC_BEGIN;
    {
        _CLS_IMPLEMENT_IMM_COMMAND_PTR40(add_imm);
    }
    _INTRINSIC_END;
}


/* Local scratch memory add saturates immediate   */

void cmd_cls_add_imm_sat_ptr32(
    volatile void __cls *address,                       /* address to add                         */
    uint32_t value                                      /* immediate value                        */
)
{
    _INTRINSIC_BEGIN;
    {
        _CLS_IMPLEMENT_IMM_COMMAND(addsat_imm);
    }
    _INTRINSIC_END;
}


void cmd_cls_add_imm_sat_ptr40(
    volatile void __addr40 __cls *address,
    uint32_t value
)
{
    _INTRINSIC_BEGIN;
    {
        _CLS_IMPLEMENT_IMM_COMMAND_PTR40(addsat_imm);
    }
    _INTRINSIC_END;
}


/* Local scratch memory add 64-bits       */

void cmd_cls_add64_ptr32(
    __xwrite void *data,                                /* data to add                            */
    volatile void __cls *address,                       /* address to add                         */
    uint32_t count,                                     /* number of words                        */
    sync_t sync,                                        /* type of synchronization to use         */
    SIGNAL *sig_ptr                                     /* signal to raise upon completion        */
)
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_write_reg(data));
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done || sync == ctx_swap);

        if (__is_ct_const(count) && count <= 8)
        {
            CT_ASSERT(count != 0);
            CT_ASSERT((count & 1) == 0);
            if (sync == sig_done)
            {
                __asm cls[add64, *data, address, 0, __ct_const_val(count)], sig_done[*sig_ptr]
            }
            else
            {
                __asm cls[add64, *data, address, 0, __ct_const_val(count)], ctx_swap[*sig_ptr]
            }
        }
        else
        {
            if (__is_ct_const(count))
            {
                CT_ASSERT((count & 1) == 0);
            }
            else
            {
                CT_QPERFINFO_INDIRECT_REF(cls_add64);
                RT_RANGE_ASSERT((count & 1) == 0);
            }
            {
                _INTRINSIC_IND_ALU_COUNT_MIN_MAX(count, 1, 16);
                if (sync == sig_done)
                {
                    __asm cls[add64, *data, address, 0, __ct_const_val(count)], sig_done[*sig_ptr], indirect_ref
                }
                else
                {
                    __asm cls[add64, *data, address, 0, __ct_const_val(count)], ctx_swap[*sig_ptr], indirect_ref
                }
            }
        }
    }
    _INTRINSIC_END;
}

void cmd_cls_add64_ptr40(
    __xwrite void *data,
    volatile void __addr40 __cls *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
)
{
    _INTRINSIC_BEGIN;
    {
        _CLS_IMPLEMENT_COMMAND_PTR40(add64, 16, 0, data);
    }
    _INTRINSIC_END;
}

/* Local scratch memory add 64-bits immediate */

void cmd_cls_add64_imm_ptr32(
    volatile void __cls *address,                       /* address to add                         */
    uint32_t value                                      /* immediate value                        */
)
{
    _INTRINSIC_BEGIN;
    {
        _CLS_IMPLEMENT_IMM_COMMAND(add64_imm);
    }
    _INTRINSIC_END;
}


void cmd_cls_add64_imm_ptr40(
    volatile void __addr40 __cls *address,
    uint32_t value
)
{
    _INTRINSIC_BEGIN;
    {
        _CLS_IMPLEMENT_IMM_COMMAND_PTR40(add64_imm);
    }
    _INTRINSIC_END;
}


/* Local scratch memory sub 64-bits       */
void cmd_cls_sub64_ptr32(
    __xwrite void *data,                                /* data to sub                            */
    volatile void __cls *address,                       /* address to sub                         */
    uint32_t count,                                     /* number of words                        */
    sync_t sync,                                        /* type of synchronization to use         */
    SIGNAL *sig_ptr                                     /* signal to raise upon completion        */
)
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_write_reg(data));
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done || sync == ctx_swap);

        if (__is_ct_const(count) && count <= 8)
        {
            CT_ASSERT(count != 0);
            if (sync == sig_done)
            {
                __asm cls[sub64, *data, address, 0, __ct_const_val(count)], sig_done[*sig_ptr]
            }
            else
            {
                __asm cls[sub64, *data, address, 0, __ct_const_val(count)], ctx_swap[*sig_ptr]
            }
        }
        else
        {
            if (__is_ct_const(count))
            {
                CT_ASSERT((count & 1) == 0);
            }
            else
            {
                CT_QPERFINFO_INDIRECT_REF(cls_sub64);
                RT_RANGE_ASSERT((count & 1) == 0);
            }
            {
                _INTRINSIC_IND_ALU_COUNT_MIN_MAX(count, 1, 16);
                if (sync == sig_done)
                {
                    __asm cls[sub64, *data, address, 0, __ct_const_val(count)], sig_done[*sig_ptr], indirect_ref
                }
                else
                {
                    __asm cls[sub64, *data, address, 0, __ct_const_val(count)], ctx_swap[*sig_ptr], indirect_ref
                }
            }
        }

    }
    _INTRINSIC_END;
}


void cmd_cls_sub64_ptr40(
    __xwrite void *data,
    volatile void __addr40 __cls *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
)
{
    _INTRINSIC_BEGIN;
    {
        _CLS_IMPLEMENT_COMMAND_PTR40(sub64, 16, 0, data);
    }
    _INTRINSIC_END;
}


/* Local scratch memory sub immediate     */

void cmd_cls_sub_imm_ptr32(
    volatile void __cls *address,                       /* address to sub                         */
    uint32_t value                                      /* immediate value                        */
)
{
    _INTRINSIC_BEGIN;
    {
        _CLS_IMPLEMENT_IMM_COMMAND(sub_imm);
    }
    _INTRINSIC_END;
}

void cmd_cls_sub_imm_ptr40(
    volatile void __addr40 __cls *address,
    uint32_t value
)
{
    _INTRINSIC_BEGIN;
    {
        _CLS_IMPLEMENT_IMM_COMMAND_PTR40(sub_imm);
    }
    _INTRINSIC_END;
}

/* Local scratch memory sub saturates immediate   */
void cmd_cls_sub_imm_sat_ptr32(
    volatile void __cls *address,                       /* address to add                         */
    uint32_t value                                      /* immediate value                        */
)
{
    _INTRINSIC_BEGIN;
    {
        _CLS_IMPLEMENT_IMM_COMMAND(subsat_imm);
    }
    _INTRINSIC_END;
}


void cmd_cls_sub_imm_sat_ptr40(
    volatile void __addr40 __cls *address,
    uint32_t value
)
{
    _INTRINSIC_BEGIN;
    {
        _CLS_IMPLEMENT_IMM_COMMAND_PTR40(subsat_imm);
    }
    _INTRINSIC_END;
}

/* Local scratch memory sub 64-bits immediate */
void cmd_cls_sub64_imm_ptr32(
    volatile void __cls *address,                       /* address to sub                         */
    uint32_t value                                      /* immediate value                        */
)
{
    _INTRINSIC_BEGIN;
    {
        _CLS_IMPLEMENT_IMM_COMMAND(sub64_imm);
    }
    _INTRINSIC_END;
}

void cmd_cls_sub64_imm_ptr40(
    volatile void __addr40 __cls *address,
    uint32_t value
)
{
    _INTRINSIC_BEGIN;
    {
        _CLS_IMPLEMENT_IMM_COMMAND_PTR40(sub64_imm);
    }
    _INTRINSIC_END;
}

/* Local scratch memory test and clear bits immediate */
void cmd_cls_test_and_clear_bits_imm_ptr32(
    __xread uint32_t *val,                              /* returned pre-modified value            */
    volatile void __cls *address,                       /* address to clear                       */
    uint32_t value,                                     /* immediate value                        */
    sync_t sync,                                        /* must be sig_done                       */
    SIGNAL *sig_ptr                                     /* signal to raise upon completion        */
)
{
    _INTRINSIC_BEGIN;
    {
        _CLS_IMPLEMENT_TEST_IMMEDIATE_PTR32(test_clr_imm, val);
    }
    _INTRINSIC_END;
}


void cmd_cls_test_and_clear_bits_imm_ptr40(
    __xread uint32_t *val,
    volatile void __addr40 __cls *address,
    uint32_t value,
    sync_t sync,
    SIGNAL *sig_ptr
)
{
    _INTRINSIC_BEGIN;
    {
        _CLS_IMPLEMENT_TEST_IMMEDIATE_PTR40(test_clr_imm, val);
    }
    _INTRINSIC_END;
}

/* Local scratch memory test and set bits */

void cmd_cls_test_and_set_bits_ptr32(
    __xread uint32_t *val,                              /* returned pre-modified value            */
    __xwrite uint32_t *mask,                            /* mask of bits to set                    */
    volatile void __cls *address,                       /* address to set                         */
    uint32_t count,                                     /* number of words                        */
    sync_t sync,                                        /* must be sig_done                       */
    SIGNAL *sig_ptr                                     /* signal to raise upon completion        */
)
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_read_reg(val));
        CT_ASSERT(__is_write_reg(mask));
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done || sync == ctx_swap);

        __associate_read_write_reg_pair_no_spill(val, mask);
        if (__is_ct_const(count) && count < 8)
        {
            CT_ASSERT(count != 0);
            if (sync == sig_done)
            {
                __asm cls[test_set, *val, address, 0, __ct_const_val(count)], sig_done[*sig_ptr]
            }
            else
            {
                __asm cls[test_set, *val, address, 0, __ct_const_val(count)], ctx_swap[*sig_ptr]
            }
        }
        else
        {
            if (!__is_ct_const(count))
            {
                CT_QPERFINFO_INDIRECT_REF(cls_test_set);
            }
            {
                _INTRINSIC_IND_ALU_COUNT_MIN_MAX(count, 1, 8);
                if (sync == sig_done)
                {
                    __asm cls[test_set, *val, address, 0, __ct_const_val(count)], sig_done[*sig_ptr], indirect_ref
                }
                else
                {
                    __asm cls[test_set, *val, address, 0, __ct_const_val(count)], ctx_swap[*sig_ptr], indirect_ref
                }
            }
        }
    }
    _INTRINSIC_END;
}

void cmd_cls_test_and_set_bits_ptr40(
    __xread uint32_t *val,
    __xwrite uint32_t *mask,
    volatile void __addr40 __cls *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
)
{
    _INTRINSIC_BEGIN;
    {
        _CLS_IMPLEMENT_TEST_COMMAND_PTR40(test_set, 8, mask);
    }
    _INTRINSIC_END;
}

/* Local scratch memory test and set bits immediate   */

void cmd_cls_test_and_set_bits_imm_ptr32(
    __xread uint32_t *val,                              /* returned pre-modified value            */
    volatile void __cls *address,                       /* address to set                         */
    uint32_t value,                                     /* immediate value                        */
    sync_t sync,                                        /* must be sig_done                       */
    SIGNAL *sig_ptr                                     /* signal to raise upon completion        */
)
{
    _INTRINSIC_BEGIN;
    {
        _CLS_IMPLEMENT_TEST_IMMEDIATE_PTR32(test_set_imm, val);
    }
    _INTRINSIC_END;
}


void cmd_cls_test_and_set_bits_imm_ptr40(
    __xread uint32_t *val,
    volatile void __addr40 __cls *address,
    uint32_t value,
    sync_t sync,
    SIGNAL *sig_ptr
)
{
    _INTRINSIC_BEGIN;
    {
        _CLS_IMPLEMENT_TEST_IMMEDIATE_PTR40(test_set_imm, val);
    }
    _INTRINSIC_END;
}


/* Local scratch memory test and add immediate saturated   */
void cmd_cls_test_and_add_imm_sat_ptr32(
    __xread uint32_t *val,                              /* returned pre-modified value            */
    volatile void __cls *address,                       /* address to add                         */
    uint32_t value,                                     /* immediate value                        */
    sync_t sync,                                        /* must be sig_done                       */
    SIGNAL *sig_ptr                                     /* signal to raise upon completion        */
)
{
    _INTRINSIC_BEGIN;
    {
        _CLS_IMPLEMENT_TEST_IMMEDIATE_PTR32(test_addsat_imm, *val) ;
    }
    _INTRINSIC_END;
}


void cmd_cls_test_and_add_imm_sat_ptr40(
    __xread uint32_t *val,
    volatile void __addr40 __cls *address,
    uint32_t value,
    sync_t sync,
    SIGNAL *sig_ptr
)
{
    _INTRINSIC_BEGIN;
    {
        _CLS_IMPLEMENT_TEST_IMMEDIATE_PTR40(test_addsat_imm, *val) ;
    }
    _INTRINSIC_END;
}


/* Local scratch memory test and sub with saturation  */
void cmd_cls_test_and_sub_sat_ptr32(
    __xread uint32_t *val,                              /* returned pre-modified value            */
    __xwrite uint32_t *data,                            /* data to sub                            */
    volatile void __cls *address,                       /* address to sub                         */
    uint32_t count,                                     /* number of words                        */
    sync_t sync,                                        /* must be sig_done                       */
    SIGNAL *sig_ptr                                     /* signal to raise upon completion        */
)
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_read_reg(val));
        CT_ASSERT(__is_write_reg(data));
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done || sync == ctx_swap);

        __associate_read_write_reg_pair_no_spill(val, data);
        if (__is_ct_const(count) && count <= 8)
        {
            CT_ASSERT(count != 0);
            if (sync == sig_done)
            {
                __asm cls[test_subsat, *val, address, 0, __ct_const_val(count)], sig_done[*sig_ptr]
            }
            else
            {
                __asm cls[test_subsat, *val, address, 0, __ct_const_val(count)], ctx_swap[*sig_ptr]
            }
        }
        else
        {
            if (!__is_ct_const(count))
            {
                CT_QPERFINFO_INDIRECT_REF(cls_test_subsat);
            }
            {
                _INTRINSIC_IND_ALU_COUNT_MIN_MAX(count, 1, 8);
                if (sync == sig_done)
                {
                    __asm cls[test_subsat, *val, address, 0, __ct_const_val(count)], sig_done[*sig_ptr], indirect_ref
                }
                else
                {
                    __asm cls[test_subsat, *val, address, 0, __ct_const_val(count)], ctx_swap[*sig_ptr], indirect_ref
                }
            }
        }
    }
    _INTRINSIC_END;
}



void cmd_cls_test_and_sub_sat_ptr40(
    __xread uint32_t *val,
    __xwrite uint32_t *data,
    volatile void __addr40 __cls *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
)
{
    _INTRINSIC_BEGIN;
    {
        _CLS_IMPLEMENT_TEST_COMMAND_PTR40(test_subsat, 8, data);
    }
    _INTRINSIC_END;
}

/* Local scratch memory test and sub immediate saturated   */
void cmd_cls_test_and_sub_imm_sat_ptr32(
    __xread uint32_t *val,                              /* returned pre-modified value            */
    volatile void __cls *address,                       /* address to add                         */
    uint32_t value,                                     /* immediate value                        */
    sync_t sync,                                        /* must be sig_done                       */
    SIGNAL *sig_ptr                                     /* signal to raise upon completion        */
)
{
    _INTRINSIC_BEGIN;
    {
        _CLS_IMPLEMENT_TEST_IMMEDIATE_PTR32(test_subsat_imm, *val) ;
    }
    _INTRINSIC_END;
}


void cmd_cls_test_and_sub_imm_sat_ptr40(
    __xread uint32_t *val,
    volatile void __addr40 __cls *address,
    uint32_t value,
    sync_t sync,
    SIGNAL *sig_ptr
)
{
    _INTRINSIC_BEGIN;
    {
        _CLS_IMPLEMENT_TEST_IMMEDIATE_PTR40(test_subsat_imm, *val) ;
    }
    _INTRINSIC_END;
}

void cmd_cls_incr_ptr32(
    volatile void __addr32 __cls *address
)
{
    _INTRINSIC_BEGIN;
    {
        _asm cls[incr, --, address, 0, --]
    }
    _INTRINSIC_END;
}

void cmd_cls_incr_ptr40(
    volatile void __addr40 __cls *address
)
{
    _INTRINSIC_BEGIN;
    {
        uint32_t hi_addr, low_addr;
        _INTRINSIC_CONVERT_HI_LO_ADDRESS(address);

        _asm cls[incr, --, hi_addr, << 8, low_addr, --]
    }
    _INTRINSIC_END;
}

void cmd_cls_decr_ptr32(
    volatile void __addr32 __cls *address
)
{
    _INTRINSIC_BEGIN;
    {
        _asm cls[decr, --, address, 0, --]
    }
    _INTRINSIC_END;
}


void cmd_cls_decr_ptr40(
    volatile void __addr40 __cls *address
)
{
    _INTRINSIC_BEGIN;
    {
        uint32_t hi_addr, low_addr;
        _INTRINSIC_CONVERT_HI_LO_ADDRESS(address);

        _asm cls[decr, --, hi_addr, << 8, low_addr, --]
    }
    _INTRINSIC_END;
}

void cmd_cls_incr64_ptr32(
    volatile void __addr32 __cls *address
)
{
    _INTRINSIC_BEGIN;
    {
        _asm cls[incr64, --, address, 0, --]
    }
    _INTRINSIC_END;
}

void cmd_cls_incr64_ptr40(
    volatile void __addr40 __cls *address
)
{
    _INTRINSIC_BEGIN;
    {
        uint32_t hi_addr, low_addr;
        _INTRINSIC_CONVERT_HI_LO_ADDRESS(address);

        _asm cls[incr64, --, hi_addr, << 8, low_addr, --]
    }
    _INTRINSIC_END;
}

void cmd_cls_decr64_ptr32(
    volatile void __addr32 __cls *address
)
{
    _INTRINSIC_BEGIN;
    {
        _asm cls[decr64, --, address, 0, --]
    }
    _INTRINSIC_END;
}


void cmd_cls_decr64_ptr40(
    volatile void __addr40 __cls *address
)
{
    _INTRINSIC_BEGIN;
    {
        uint32_t hi_addr, low_addr;
        _INTRINSIC_CONVERT_HI_LO_ADDRESS(address);

        _asm cls[decr64, --, hi_addr, << 8, low_addr, --]
    }
    _INTRINSIC_END;
}

/* Local scratch memory compare read/write*/

void cmd_cls_cmp_read_write_ptr32(
    __xrw uint32_t *data,                               /* data to read into                      */
    uint32_t mask,                                      /* mask                                   */
    volatile void __cls *address,                       /* address to read/write from             */
    uint32_t count,                                     /* number of words to read/write          */
    sync_t sync,                                        /* type of synchronization to use         */
    SIGNAL *sig_ptr                                     /* signal to raise upon completion        */
)
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_ct_const(mask));
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done || sync == ctx_swap);

        if (__is_ct_const(count))
        {
            CT_ASSERT((count > 0) && (count <= 32));
        }
        else
        {
            CT_QPERFINFO_INDIRECT_REF(cls_cmd_read_write);
            RT_RANGE_ASSERT((count > 0) && (count <= 32));
        }
        {
            generic_ind_t ind;
            _INTRINSIC_OVERRIDE_LENGTH_AND_BYTEMASK_IN_ALU(ind, count - 1, mask);

            if (sync == sig_done)
            {
                __asm
                {
                    alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
                    cls[test_compare_write, *data, address, 0, __ct_const_val(count)], sig_done[*sig_ptr], indirect_ref
                }
            }
            else
            {
                __asm
                {
                    alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
                    cls[test_compare_write, *data, address, 0, __ct_const_val(count)], ctx_swap[*sig_ptr], indirect_ref
                }
            }
        }
    }
    _INTRINSIC_END;
}


void cmd_cls_cmp_read_write_ptr40(
    __xrw uint32_t *data,
    uint32_t mask,
    volatile void __addr40 __cls *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
)
{
    _INTRINSIC_BEGIN;
    {
         uint32_t hi_addr, low_addr;
        _INTRINSIC_CONVERT_HI_LO_ADDRESS(address);

        CT_ASSERT(__is_ct_const(mask));
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done || sync == ctx_swap);

        if (__is_ct_const(count))
        {
            CT_ASSERT((count > 0) && (count <= 32));
        }
        else
        {
            CT_QPERFINFO_INDIRECT_REF(cls_cmd_read_write);
            RT_RANGE_ASSERT((count > 0) && (count <= 32));
        }
        {
            generic_ind_t ind;
            _INTRINSIC_OVERRIDE_LENGTH_AND_BYTEMASK_IN_ALU(ind, count - 1, mask);

            if (sync == sig_done)
            {
                __asm
                {
                    alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
                    cls[test_compare_write, *data,  hi_addr, << 8, low_addr, __ct_const_val(count)], sig_done[*sig_ptr], indirect_ref
                }
            }
            else
            {
                __asm
                {
                    alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
                    cls[test_compare_write, *data,  hi_addr, << 8, low_addr, __ct_const_val(count)], ctx_swap[*sig_ptr], indirect_ref
                }
            }
        }
    }
    _INTRINSIC_END;
}


__intrinsic
void cmd_cls_cmp_read_write_ind_ptr40(
    __xrw void *data,                                   /* address to read/write into             */
    volatile void __addr40 __cls *address,              /* address to read/write from             */
    uint32_t max_nn,                                    /* max. number of words to read/write */
    generic_ind_t ind,                                  /* indirect word                          */
    sync_t sync,                                        /* type of synchronization to use         */
    SIGNAL *sig_ptr                                     /* signal to raise upon completion        */
    )
{
    _INTRINSIC_BEGIN;
    {
         uint32_t hi_addr, low_addr;

         _INTRINSIC_CONVERT_HI_LO_ADDRESS(address);
         CT_ASSERT(__is_ct_const(max_nn));
         CT_ASSERT(max_nn >= 1 && max_nn <= 32);
         CT_ASSERT(__is_ct_const(sync));
         CT_ASSERT(sync == sig_done || sync == ctx_swap);
         __asm local_csr_wr[CMD_INDIRECT_REF_0, CSR_INDIRECT_TYPE(ind)]
         __asm  alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
        if (sync == sig_done)
        {
            __asm cls[test_compare_write, *data, hi_addr, <<8, low_addr, __ct_const_val(max_nn)], sig_done[*sig_ptr], indirect_ref
        }
        else
        {
            __asm cls[test_compare_write, *data, hi_addr, <<8, low_addr, __ct_const_val(max_nn)], ctx_swap[*sig_ptr], indirect_ref
        }
    }
    _INTRINSIC_END;
}


__intrinsic
void cmd_cls_cmp_read_write_ind_ptr32(
    __xrw void *data,                                   /* address to read/write into             */
    volatile void __cls *address,                       /* address to read/write from             */
    uint32_t max_nn,                                    /* max. number of words to read/write     */
    generic_ind_t ind,                                  /* indirect word                          */
    sync_t sync,                                        /* type of synchronization to use         */
    SIGNAL *sig_ptr                                     /* signal to raise upon completion        */
    )
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_ct_const(max_nn));
        CT_ASSERT(max_nn >= 1 && max_nn <= 32);
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done || sync == ctx_swap);
         __asm local_csr_wr[CMD_INDIRECT_REF_0, CSR_INDIRECT_TYPE(ind)]
         __asm  alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
        if (sync == sig_done)
        {
            __asm cls[test_compare_write, *data, address, 0, __ct_const_val(max_nn)], sig_done[*sig_ptr], indirect_ref
        }
        else
        {
            __asm cls[test_compare_write, *data, address, 0, __ct_const_val(max_nn)], ctx_swap[*sig_ptr], indirect_ref
        }
    }
    _INTRINSIC_END;
}


/* Local scratch Queue Lock Claim indirect*/
void cmd_cls_queue_lock_ind_ptr40(
    volatile void __addr40 __cls *address,              /* address to read into                   */
    generic_ind_t ind,                                  /* indirect                               */
    sync_t sync,                                        /* type of synchronization to use         */
    SIGNAL_PAIR *sig_pair_ptr                           /* signal pair to raise upon completion   */
    )
{
    _INTRINSIC_BEGIN;
    {
        uint32_t hi_addr, low_addr;

        _INTRINSIC_CONVERT_HI_LO_ADDRESS(address);
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done);
        __asm local_csr_wr[CMD_INDIRECT_REF_0, CSR_INDIRECT_TYPE(ind)]
        __asm
        {
            alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
            cls[queue_lock, --, hi_addr, <<8, low_addr, 0], sig_done[*__ct_sig_pair(sig_pair_ptr)], indirect_ref
        }
    }
    _INTRINSIC_END;
}


/* Local scratch memory Hash              */
void cmd_cls_hash_mask_ptr32(
    __xwrite void *data,                                /* data to hash                           */
    volatile void __cls *address,                       /* address to hash                        */
    uint32_t count,                                     /* number of words to hash                */
    sync_t sync,                                        /* type of synchronization to use         */
    SIGNAL *sig_ptr                                     /* signal to raise upon completion        */
)
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_write_reg(data));
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done || sync == ctx_swap);

        if (__is_ct_const(count) && count <= 8)
        {
            CT_ASSERT(count != 0);
            if (sync == sig_done)
            {
                __asm cls[hash_mask, *data, address, 0, __ct_const_val(count)], sig_done[*sig_ptr]
            }
            else
            {
                __asm cls[hash_mask, *data, address, 0, __ct_const_val(count)], ctx_swap[*sig_ptr]
            }
        }
        else
        {
            if (!__is_ct_const(count))
            {
                CT_QPERFINFO_INDIRECT_REF(cmd_cls_hash_mask_ptr32);
            }
            {
                _INTRINSIC_IND_ALU_COUNT_MIN_MAX(count, 1, 32);
                if (sync == sig_done)
                {
                    __asm cls[hash_mask, *data, address, 0, __ct_const_val(count)], sig_done[*sig_ptr], indirect_ref
                }
                else
                {
                    __asm cls[hash_mask, *data, address, 0, __ct_const_val(count)], ctx_swap[*sig_ptr], indirect_ref
                }
            }
        }
    }
    _INTRINSIC_END;
}



__intrinsic
void cmd_cls_hash_mask_ptr40(
    __xwrite void *data,
    volatile void __addr40 __cls *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
)
{
    _INTRINSIC_BEGIN;
    {
        _CLS_IMPLEMENT_COMMAND_PTR40(hash_mask, 32, 0, data);
    }
    _INTRINSIC_END;
}


__intrinsic
void cmd_cls_hash_mask_ind_ptr40(
    __xwrite void *data,                                /* data to hash                           */
    volatile void __addr40 __cls *address,              /* address to hash                        */
    uint32_t max_nn,                                    /* max. number of words to hash           */
    generic_ind_t ind,                                  /* indirect word                          */
    sync_t sync,                                        /* type of synchronization to use         */
    SIGNAL *sig_ptr                                     /* signal to raise upon completion        */
    )
{
    _INTRINSIC_BEGIN;
    {
        uint32_t hi_addr, low_addr;

        _INTRINSIC_CONVERT_HI_LO_ADDRESS(address);
        CT_ASSERT(__is_write_reg(data));
        CT_ASSERT(__is_ct_const(max_nn));
        CT_ASSERT(max_nn >= 1 && max_nn <= 32);
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done || sync == ctx_swap);
        __asm local_csr_wr[CMD_INDIRECT_REF_0, CSR_INDIRECT_TYPE(ind)]
        __asm  alu[--, --, B, ALU_INDIRECT_TYPE(ind)]

        if (sync == sig_done)
        {
            __asm cls[hash_mask, *data, hi_addr, <<8, low_addr, __ct_const_val(max_nn)], sig_done[*sig_ptr], indirect_ref
        }
        else
        {
            __asm cls[hash_mask, *data, hi_addr, <<8, low_addr, __ct_const_val(max_nn)], ctx_swap[*sig_ptr], indirect_ref
        }
    }
    _INTRINSIC_END;
}


__intrinsic
void cmd_cls_hash_mask_ind_ptr32(
    __xwrite void *data,                                /* data to hash                           */
    volatile void __cls *address,                       /* address to hash                        */
    uint32_t max_nn,                                    /* max. number of words to hash           */
    generic_ind_t ind,                                  /* indirect word                          */
    sync_t sync,                                        /* type of synchronization to use         */
    SIGNAL *sig_ptr                                     /* signal to raise upon completion        */
    )
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_write_reg(data));
        CT_ASSERT(__is_ct_const(max_nn));
        CT_ASSERT(max_nn >= 1 && max_nn <= 32);
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done || sync == ctx_swap);

        __asm local_csr_wr[CMD_INDIRECT_REF_0, CSR_INDIRECT_TYPE(ind)]
        __asm  alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
        if (sync == sig_done)
        {
            __asm cls[hash_mask, *data, address, 0, __ct_const_val(max_nn)], sig_done[*sig_ptr], indirect_ref
        }
        else
        {
            __asm cls[hash_mask, *data, address, 0, __ct_const_val(max_nn)], ctx_swap[*sig_ptr], indirect_ref
        }
    }
    _INTRINSIC_END;
}


/* Local scratch memory Hash Clear        */
void cmd_cls_hash_mask_clear_ptr32(
    __xwrite void *data,                                /* data to hash                           */
    volatile void __cls *address,                       /* address to hash                        */
    uint32_t count,                                     /* number of words to hash                */
    sync_t sync,                                        /* type of synchronization to use         */
    SIGNAL *sig_ptr                                     /* signal to raise upon completion        */
)
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_write_reg(data));
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done || sync == ctx_swap);

        if (__is_ct_const(count) && count <= 8)
        {
            CT_ASSERT(count != 0);
            if (sync == sig_done)
            {
                __asm cls[hash_mask_clear, *data, address, 0, __ct_const_val(count)], sig_done[*sig_ptr]
            }
            else
            {
                __asm cls[hash_mask_clear, *data, address, 0, __ct_const_val(count)], ctx_swap[*sig_ptr]
            }
        }
        else
        {
            if (!__is_ct_const(count))
            {
                CT_QPERFINFO_INDIRECT_REF(cmd_cls_hash_mask_clear_ptr32);
            }
            {
                _INTRINSIC_IND_ALU_COUNT_MIN_MAX(count, 1, 32);
                if (sync == sig_done)
                {
                    __asm cls[hash_mask_clear, *data, address, 0, __ct_const_val(count)], sig_done[*sig_ptr], indirect_ref
                }
                else
                {
                    __asm cls[hash_mask_clear, *data, address, 0, __ct_const_val(count)], ctx_swap[*sig_ptr], indirect_ref
                }
            }
        }
    }
    _INTRINSIC_END;
}


void cmd_cls_hash_mask_clear_ptr40(
    __xwrite void *data,
    volatile void __addr40 __cls *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
)
{
    _INTRINSIC_BEGIN;
    {
        _CLS_IMPLEMENT_COMMAND_PTR40(hash_mask_clear, 32, 0, data);
    }
    _INTRINSIC_END;
}


__intrinsic
void cmd_cls_cam_lookup_ind_ptr40(
    __xread void *data_out,                             /* lookuped data */
    __xwrite void *data_in,                             /* data to lookup */
    volatile void __addr40 __cls *address,              /* address to lookup */
    generic_ind_t ind,                                  /* indirect word */
    sync_t sync,                                        /* type of synchronization to use */
    SIGNAL *sig_ptr                                     /* signal to raise upon completion */
    )
{
    _INTRINSIC_BEGIN;
    {
        uint32_t hi_addr, low_addr;

        _INTRINSIC_CONVERT_HI_LO_ADDRESS(address);
        CT_ASSERT(__is_read_reg(data_out));
        CT_ASSERT(__is_write_reg(data_in));
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done || sync == ctx_swap);
        __associate_read_write_reg_pair_no_spill(data_out,data_in);
        __asm local_csr_wr[CMD_INDIRECT_REF_0, CSR_INDIRECT_TYPE(ind)]
        __asm  alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
        if (sync == sig_done)
        {
            __asm cls[cam_lookup32, *data_out, hi_addr, <<8, low_addr, max_1], sig_done[*sig_ptr], indirect_ref
        }
        else
        {
            __asm cls[cam_lookup32, *data_out, hi_addr, <<8, low_addr, max_1], ctx_swap[*sig_ptr], indirect_ref
        }
    }
    _INTRINSIC_END;
}


__intrinsic
void cmd_cls_cam_lookup_ind_ptr32(
    __xread void *data_out,                     /* lookuped data */
    __xwrite void *data_in,                     /* data to lookup */
    volatile void __cls *address,               /* address to lookup */
    generic_ind_t ind,                          /* indirect word */
    sync_t sync,                                /* type of synchronization to use */
    SIGNAL *sig_ptr                             /* signal to raise upon completion */
    )
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_read_reg(data_out));
        CT_ASSERT(__is_write_reg(data_in));
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done || sync == ctx_swap);

        __associate_read_write_reg_pair_no_spill(data_out,data_in);
        __asm local_csr_wr[CMD_INDIRECT_REF_0, CSR_INDIRECT_TYPE(ind)]
        __asm  alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
        if (sync == sig_done)
        {
            __asm cls[cam_lookup32, *data_out, address, 0, max_1], sig_done[*sig_ptr], indirect_ref
        }
        else
        {
            __asm cls[cam_lookup32, *data_out, address, 0, max_1], ctx_swap[*sig_ptr], indirect_ref
        }
    }
    _INTRINSIC_END;
}


__intrinsic
void cmd_cls_cam_lookup_add_ind_ptr40(
    __xread void *data_out,                             /* lookuped data */
    __xwrite void *data_in,                             /* data to lookup */
    volatile void __addr40 __cls *address,              /* address to lookup */
    generic_ind_t ind,                                  /* indirect word */
    sync_t sync,                                        /* type of synchronization to use */
    SIGNAL *sig_ptr                                     /* signal to raise upon completion */
    )
{
    _INTRINSIC_BEGIN;
    {
        uint32_t hi_addr, low_addr;

        _INTRINSIC_CONVERT_HI_LO_ADDRESS(address);
        CT_ASSERT(__is_read_reg(data_out));
        CT_ASSERT(__is_write_reg(data_in));
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done || sync == ctx_swap);

        __associate_read_write_reg_pair_no_spill(data_out,data_in);
        __asm local_csr_wr[CMD_INDIRECT_REF_0, CSR_INDIRECT_TYPE(ind)]
        __asm  alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
        if (sync == sig_done)
        {
            __asm cls[cam_lookup32_add, *data_out, hi_addr, <<8, low_addr, max_1], sig_done[*sig_ptr], indirect_ref
        }
        else
        {
            __asm cls[cam_lookup32_add, *data_out, hi_addr, <<8, low_addr, max_1], ctx_swap[*sig_ptr], indirect_ref
        }
    }
    _INTRINSIC_END;
}


__intrinsic
void cmd_cls_cam_lookup_add_ind_ptr32(
    __xread void *data_out,                     /* lookuped data */
    __xwrite void *data_in,                     /* data to lookup */
    volatile void __cls *address,               /* address to lookup */
    generic_ind_t ind,                          /* indirect word */
    sync_t sync,                                /* type of synchronization to use */
    SIGNAL *sig_ptr                             /* signal to raise upon completion */
    )
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_read_reg(data_out));
        CT_ASSERT(__is_write_reg(data_in));
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done || sync == ctx_swap);

        __associate_read_write_reg_pair_no_spill(data_out,data_in);
        __asm local_csr_wr[CMD_INDIRECT_REF_0, CSR_INDIRECT_TYPE(ind)]
        __asm  alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
        if (sync == sig_done)
        {
            __asm cls[cam_lookup32_add, *data_out, address, 0, max_1], sig_done[*sig_ptr], indirect_ref
        }
        else
        {
            __asm cls[cam_lookup32_add, *data_out, address, 0, max_1], ctx_swap[*sig_ptr], indirect_ref
        }
    }
    _INTRINSIC_END;
}


__intrinsic
void cmd_cls_cam_lookup24_ind_ptr40(
    __xread void *data_out,                             /* lookuped data */
    __xwrite void *data_in,                             /* data to lookup */
    volatile void __addr40 __cls *address,              /* address to lookup */
    generic_ind_t ind,                                  /* indirect word */
    sync_t sync,                                        /* type of synchronization to use */
    SIGNAL *sig_ptr                                     /* signal to raise upon completion */
    )
{
    _INTRINSIC_BEGIN;
    {
        uint32_t hi_addr, low_addr;

        _INTRINSIC_CONVERT_HI_LO_ADDRESS(address);
        CT_ASSERT(__is_read_reg(data_out));
        CT_ASSERT(__is_write_reg(data_in));
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done || sync == ctx_swap);

        __associate_read_write_reg_pair_no_spill(data_out,data_in);
        __asm local_csr_wr[CMD_INDIRECT_REF_0, CSR_INDIRECT_TYPE(ind)]
        __asm  alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
        if (sync == sig_done)
        {
            __asm cls[cam_lookup24, *data_out, hi_addr, <<8, low_addr, max_1], sig_done[*sig_ptr], indirect_ref
        }
        else
        {
            __asm cls[cam_lookup24, *data_out, hi_addr, <<8, low_addr, max_1], ctx_swap[*sig_ptr], indirect_ref
        }
    }
    _INTRINSIC_END;
}


__intrinsic
void cmd_cls_cam_lookup24_ind_ptr32(
    __xread void *data_out,                     /* lookuped data */
    __xwrite void *data_in,                     /* data to lookup */
    volatile void __cls *address,               /* address to lookup */
    generic_ind_t ind,                          /* indirect word */
    sync_t sync,                                /* type of synchronization to use */
    SIGNAL *sig_ptr                             /* signal to raise upon completion */
    )
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_read_reg(data_out));
        CT_ASSERT(__is_write_reg(data_in));
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done || sync == ctx_swap);

        __associate_read_write_reg_pair_no_spill(data_out,data_in);
        __asm local_csr_wr[CMD_INDIRECT_REF_0, CSR_INDIRECT_TYPE(ind)]
        __asm  alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
        if (sync == sig_done)
        {
            __asm cls[cam_lookup24, *data_out, address, 0, max_1], sig_done[*sig_ptr], indirect_ref
        }
        else
        {
            __asm cls[cam_lookup24, *data_out, address, 0, max_1], ctx_swap[*sig_ptr], indirect_ref
        }
    }
    _INTRINSIC_END;
}


__intrinsic
void cmd_cls_cam_lookup24_add_ind_ptr40(
    __xread void *data_out,                             /* lookuped data */
    __xwrite void *data_in,                             /* data to lookup */
    volatile void __addr40 __cls *address,              /* address to lookup */
    generic_ind_t ind,                                  /* indirect word */
    sync_t sync,                                        /* type of synchronization to use */
    SIGNAL *sig_ptr                                     /* signal to raise upon completion */
    )
{
    _INTRINSIC_BEGIN;
    {
        uint32_t hi_addr, low_addr;

        _INTRINSIC_CONVERT_HI_LO_ADDRESS(address);
        CT_ASSERT(__is_read_reg(data_out));
        CT_ASSERT(__is_write_reg(data_in));
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done || sync == ctx_swap);

        __associate_read_write_reg_pair_no_spill(data_out,data_in);
        __asm local_csr_wr[CMD_INDIRECT_REF_0, CSR_INDIRECT_TYPE(ind)]
        __asm  alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
        if (sync == sig_done)
        {
            __asm cls[cam_lookup24_add, *data_out, hi_addr, <<8, low_addr, max_1], sig_done[*sig_ptr], indirect_ref
        }
        else
        {
            __asm cls[cam_lookup24_add, *data_out, hi_addr, <<8, low_addr, max_1], ctx_swap[*sig_ptr], indirect_ref
        }
    }
    _INTRINSIC_END;
}


__intrinsic
void cmd_cls_cam_lookup24_add_ind_ptr32(
    __xread void *data_out,                     /* lookuped data */
    __xwrite void *data_in,                     /* data to lookup */
    volatile void __cls *address,               /* address to lookup */
    generic_ind_t ind,                          /* indirect word */
    sync_t sync,                                /* type of synchronization to use */
    SIGNAL *sig_ptr                             /* signal to raise upon completion */
    )
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_read_reg(data_out));
        CT_ASSERT(__is_write_reg(data_in));
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done || sync == ctx_swap);

        __associate_read_write_reg_pair_no_spill(data_out,data_in);
        __asm local_csr_wr[CMD_INDIRECT_REF_0, CSR_INDIRECT_TYPE(ind)]
        __asm  alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
        if (sync == sig_done)
        {
            __asm cls[cam_lookup24_add, *data_out, address, 0, max_1], sig_done[*sig_ptr], indirect_ref
        }
        else
        {
            __asm cls[cam_lookup24_add, *data_out, address, 0, max_1], ctx_swap[*sig_ptr], indirect_ref
        }
    }
    _INTRINSIC_END;
}


__intrinsic
void cmd_cls_cam_lookup24_add_inc_ind_ptr40(
    __xread void *data_out,                             /* lookuped data */
    __xwrite void *data_in,                             /* data to lookup */
    volatile void __addr40 __cls *address,              /* address to lookup */
    generic_ind_t ind,                                  /* indirect word */
    sync_t sync,                                        /* type of synchronization to use */
    SIGNAL *sig_ptr                                     /* signal to raise upon completion */
    )
{
    _INTRINSIC_BEGIN;
    {
        uint32_t hi_addr, low_addr;
        __ct_assert(!( __is_nfp_arch(__nfp3800, __REVISION_A0)),"cmd_cls_cam_lookup24_add_inc_ind_ptr40 is not supported with the selected chip");
        _INTRINSIC_CONVERT_HI_LO_ADDRESS(address);
        CT_ASSERT(__is_read_reg(data_out));
        CT_ASSERT(__is_write_reg(data_in));
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done || sync == ctx_swap);

        __associate_read_write_reg_pair_no_spill(data_out,data_in);
        __asm local_csr_wr[CMD_INDIRECT_REF_0, CSR_INDIRECT_TYPE(ind)]
        __asm  alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
        if (sync == sig_done)
        {
            __asm cls[cam_lookup24_add_inc, *data_out, hi_addr, <<8, low_addr, max_1], sig_done[*sig_ptr], indirect_ref
        }
        else
        {
            __asm cls[cam_lookup24_add_inc, *data_out, hi_addr, <<8, low_addr, max_1], ctx_swap[*sig_ptr], indirect_ref
        }
    }
    _INTRINSIC_END;
}


__intrinsic
void cmd_cls_cam_lookup24_add_inc_ind_ptr32(
    __xread void *data_out,                     /* lookuped data */
    __xwrite void *data_in,                     /* data to lookup */
    volatile void __cls *address,               /* address to lookup */
    generic_ind_t ind,                          /* indirect word */
    sync_t sync,                                /* type of synchronization to use */
    SIGNAL *sig_ptr                             /* signal to raise upon completion */
    )
{
    _INTRINSIC_BEGIN;
    {
        __ct_assert(!( __is_nfp_arch(__nfp3800, __REVISION_A0)),"cmd_cls_cam_lookup24_add_inc_ind_ptr32 is not supported with the selected chip");
        CT_ASSERT(__is_read_reg(data_out));
        CT_ASSERT(__is_write_reg(data_in));
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done || sync == ctx_swap);

        __associate_read_write_reg_pair_no_spill(data_out,data_in);
        __asm local_csr_wr[CMD_INDIRECT_REF_0, CSR_INDIRECT_TYPE(ind)]
        __asm  alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
        if (sync == sig_done)
        {
            __asm cls[cam_lookup24_add_inc, *data_out, address, 0, max_1], sig_done[*sig_ptr], indirect_ref
        }
        else
        {
            __asm cls[cam_lookup24_add_inc, *data_out, address, 0, max_1], ctx_swap[*sig_ptr], indirect_ref
        }
    }
    _INTRINSIC_END;
}


__intrinsic
void cmd_cls_cam_lookup16_ind_ptr40(
    __xread void *data_out,                             /* lookuped data */
    __xwrite void *data_in,                             /* data to lookup */
    volatile void __addr40 __cls *address,              /* address to lookup */
    generic_ind_t ind,                                  /* indirect word */
    sync_t sync,                                        /* type of synchronization to use */
    SIGNAL *sig_ptr                                     /* signal to raise upon completion */
    )
{
    _INTRINSIC_BEGIN;
    {
        uint32_t hi_addr, low_addr;

        _INTRINSIC_CONVERT_HI_LO_ADDRESS(address);
        CT_ASSERT(__is_read_reg(data_out));
        CT_ASSERT(__is_write_reg(data_in));
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done || sync == ctx_swap);

        __associate_read_write_reg_pair_no_spill(data_out,data_in);
        __asm local_csr_wr[CMD_INDIRECT_REF_0, CSR_INDIRECT_TYPE(ind)]
        __asm  alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
        if (sync == sig_done)
        {
            __asm cls[cam_lookup16, *data_out, hi_addr, <<8, low_addr, max_2], sig_done[*sig_ptr], indirect_ref
        }
        else
        {
            __asm cls[cam_lookup16, *data_out, hi_addr, <<8, low_addr, max_2], ctx_swap[*sig_ptr], indirect_ref
        }
    }
    _INTRINSIC_END;
}


__intrinsic
void cmd_cls_cam_lookup16_ind_ptr32(
    __xread void *data_out,                     /* lookuped data */
    __xwrite void *data_in,                     /* data to lookup */
    volatile void __cls *address,               /* address to lookup */
    generic_ind_t ind,                          /* indirect word */
    sync_t sync,                                /* type of synchronization to use */
    SIGNAL *sig_ptr                             /* signal to raise upon completion */
    )
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_read_reg(data_out));
        CT_ASSERT(__is_write_reg(data_in));
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done || sync == ctx_swap);

        __associate_read_write_reg_pair_no_spill(data_out,data_in);
        __asm local_csr_wr[CMD_INDIRECT_REF_0, CSR_INDIRECT_TYPE(ind)]
        __asm  alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
        if (sync == sig_done)
        {
            __asm cls[cam_lookup16, *data_out, address, 0, max_2], sig_done[*sig_ptr], indirect_ref
        }
        else
        {
            __asm cls[cam_lookup16, *data_out, address, 0, max_2], ctx_swap[*sig_ptr], indirect_ref
        }
    }
    _INTRINSIC_END;
}



__intrinsic
void cmd_cls_cam_lookup16_add_ind_ptr40(
    __xread void *data_out,                             /* lookuped data */
    __xwrite void *data_in,                             /* data to lookup */
    volatile void __addr40 __cls *address,              /* address to lookup */
    generic_ind_t ind,                                  /* indirect word */
    sync_t sync,                                        /* type of synchronization to use */
    SIGNAL *sig_ptr                                     /* signal to raise upon completion */
    )
{
    _INTRINSIC_BEGIN;
    {
        uint32_t hi_addr, low_addr;

        _INTRINSIC_CONVERT_HI_LO_ADDRESS(address);
        CT_ASSERT(__is_read_reg(data_out));
        CT_ASSERT(__is_write_reg(data_in));
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done || sync == ctx_swap);

        __associate_read_write_reg_pair_no_spill(data_out,data_in);
        __asm local_csr_wr[CMD_INDIRECT_REF_0, CSR_INDIRECT_TYPE(ind)]
        __asm  alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
        if (sync == sig_done)
        {
            __asm cls[cam_lookup16_add, *data_out, hi_addr, <<8, low_addr, max_2], sig_done[*sig_ptr], indirect_ref
        }
        else
        {
            __asm cls[cam_lookup16_add, *data_out, hi_addr, <<8, low_addr, max_2], ctx_swap[*sig_ptr], indirect_ref
        }
    }
    _INTRINSIC_END;
}


__intrinsic
void cmd_cls_cam_lookup16_add_ind_ptr32(
    __xread void *data_out,                         /* lookuped data */
    __xwrite void *data_in,                         /* data to lookup */
    volatile void __cls *address,                   /* address to lookup */
    generic_ind_t ind,                              /* indirect word */
    sync_t sync,                                    /* type of synchronization to use */
    SIGNAL *sig_ptr                                 /* signal to raise upon completion */
    )
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_read_reg(data_out));
        CT_ASSERT(__is_write_reg(data_in));
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done || sync == ctx_swap);

        __associate_read_write_reg_pair_no_spill(data_out,data_in);
        __asm local_csr_wr[CMD_INDIRECT_REF_0, CSR_INDIRECT_TYPE(ind)]
        __asm  alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
        if (sync == sig_done)
        {
            __asm cls[cam_lookup16_add, *data_out, address, 0, max_2], sig_done[*sig_ptr], indirect_ref
        }
        else
        {
            __asm cls[cam_lookup16_add, *data_out, address, 0, max_2], ctx_swap[*sig_ptr], indirect_ref
        }
    }
    _INTRINSIC_END;
}


__intrinsic
void cmd_cls_cam_lookup8_ind_ptr40(
    __xread void *data_out,                             /* lookuped data */
    __xwrite void *data_in,                             /* data to lookup */
    volatile void __addr40 __cls *address,              /* address to lookup */
    generic_ind_t ind,                                  /* indirect word */
    sync_t sync,                                        /* type of synchronization to use */
    SIGNAL *sig_ptr                                     /* signal to raise upon completion */
    )
{
    _INTRINSIC_BEGIN;
    {
        uint32_t hi_addr, low_addr;

        _INTRINSIC_CONVERT_HI_LO_ADDRESS(address);
        CT_ASSERT(__is_read_reg(data_out));
        CT_ASSERT(__is_write_reg(data_in));
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done || sync == ctx_swap);

        __associate_read_write_reg_pair_no_spill(data_out,data_in);
        __asm local_csr_wr[CMD_INDIRECT_REF_0, CSR_INDIRECT_TYPE(ind)]
        __asm  alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
        if (sync == sig_done)
        {
            __asm cls[cam_lookup8, *data_out, hi_addr, <<8, low_addr, max_2], sig_done[*sig_ptr], indirect_ref
        }
        else
        {
            __asm cls[cam_lookup8, *data_out, hi_addr, <<8, low_addr, max_2], ctx_swap[*sig_ptr], indirect_ref
        }
    }
    _INTRINSIC_END;
}


__intrinsic
void cmd_cls_cam_lookup8_ind_ptr32(
    __xread void *data_out,                     /* lookuped data */
    __xwrite void *data_in,                     /* data to lookup */
    volatile void __cls *address,               /* address to lookup */
    generic_ind_t ind,                          /* indirect word */
    sync_t sync,                                /* type of synchronization to use */
    SIGNAL *sig_ptr                             /* signal to raise upon completion */
    )
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_read_reg(data_out));
        CT_ASSERT(__is_write_reg(data_in));
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done || sync == ctx_swap);

        __associate_read_write_reg_pair_no_spill(data_out,data_in);
        __asm local_csr_wr[CMD_INDIRECT_REF_0, CSR_INDIRECT_TYPE(ind)]
        __asm  alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
        if (sync == sig_done)
        {
            __asm cls[cam_lookup8, *data_out, address, 0, max_2], sig_done[*sig_ptr], indirect_ref
        }
        else
        {
            __asm cls[cam_lookup8, *data_out, address, 0, max_2], ctx_swap[*sig_ptr], indirect_ref
        }
    }
    _INTRINSIC_END;
}


__intrinsic
void cmd_cls_cam_lookup8_add_ind_ptr40(
    __xread void *data_out,                             /* lookuped data */
    __xwrite void *data_in,                             /* data to lookup */
    volatile void __addr40 __cls *address,              /* address to lookup */
    generic_ind_t ind,                                  /* indirect word */
    sync_t sync,                                        /* type of synchronization to use */
    SIGNAL *sig_ptr                                     /* signal to raise upon completion */
    )
{
    _INTRINSIC_BEGIN;
    {
        uint32_t hi_addr, low_addr;

        _INTRINSIC_CONVERT_HI_LO_ADDRESS(address);
        CT_ASSERT(__is_read_reg(data_out));
        CT_ASSERT(__is_write_reg(data_in));
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done || sync == ctx_swap);

        __associate_read_write_reg_pair_no_spill(data_out,data_in);
        __asm local_csr_wr[CMD_INDIRECT_REF_0, CSR_INDIRECT_TYPE(ind)]
        __asm  alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
        if (sync == sig_done)
        {
            __asm cls[cam_lookup8_add, *data_out, hi_addr, <<8, low_addr, max_2], sig_done[*sig_ptr], indirect_ref
        }
        else
        {
            __asm cls[cam_lookup8_add, *data_out, hi_addr, <<8, low_addr, max_2], ctx_swap[*sig_ptr], indirect_ref
        }
    }
    _INTRINSIC_END;
}



__intrinsic
void cmd_cls_cam_lookup8_add_ind_ptr32(
    __xread void *data_out,                     /* lookuped data */
    __xwrite void *data_in,                     /* data to lookup */
    volatile void __cls *address,               /* address to lookup */
    generic_ind_t ind,                          /* indirect word */
    sync_t sync,                                /* type of synchronization to use */
    SIGNAL *sig_ptr                             /* signal to raise upon completion */
    )
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_read_reg(data_out));
        CT_ASSERT(__is_write_reg(data_in));
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done || sync == ctx_swap);

        __associate_read_write_reg_pair_no_spill(data_out,data_in);
        __asm local_csr_wr[CMD_INDIRECT_REF_0, CSR_INDIRECT_TYPE(ind)]
        __asm  alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
        if (sync == sig_done)
        {
            __asm cls[cam_lookup8_add, *data_out, address, 0, max_2], sig_done[*sig_ptr], indirect_ref
        }
        else
        {
            __asm cls[cam_lookup8_add, *data_out, address, 0, max_2], ctx_swap[*sig_ptr], indirect_ref
        }
    }
    _INTRINSIC_END;
}


__intrinsic
void cmd_cls_tcam_lookup_ind_ptr40(
    __xread void *data_out,                             /* lookuped data */
    __xwrite void *data_in,                             /* data to lookup */
    volatile void __addr40 __cls *address,              /* address to lookup */
    generic_ind_t ind,                                  /* indirect word */
    sync_t sync,                                        /* type of synchronization to use */
    SIGNAL *sig_ptr                                     /* signal to raise upon completion */
    )
{
    _INTRINSIC_BEGIN;
    {
        uint32_t hi_addr, low_addr;

        _INTRINSIC_CONVERT_HI_LO_ADDRESS(address);
        CT_ASSERT(__is_read_reg(data_out));
        CT_ASSERT(__is_write_reg(data_in));
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done || sync == ctx_swap);

        __associate_read_write_reg_pair_no_spill(data_out,data_in);
        __asm local_csr_wr[CMD_INDIRECT_REF_0, CSR_INDIRECT_TYPE(ind)]
        __asm  alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
        if (sync == sig_done)
        {
            __asm cls[tcam_lookup32, *data_out, hi_addr, <<8, low_addr, max_1], sig_done[*sig_ptr], indirect_ref
        }
        else
        {
            __asm cls[tcam_lookup32, *data_out, hi_addr, <<8, low_addr, max_1], ctx_swap[*sig_ptr], indirect_ref
        }
    }
    _INTRINSIC_END;
}


__intrinsic
void cmd_cls_tcam_lookup_ind_ptr32(
    __xread void *data_out,                     /* lookuped data */
    __xwrite void *data_in,                     /* data to lookup */
    volatile void __cls *address,               /* address to lookup */
    generic_ind_t ind,                          /* indirect word */
    sync_t sync,                                /* type of synchronization to use */
    SIGNAL *sig_ptr                             /* signal to raise upon completion */
    )
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_read_reg(data_out));
        CT_ASSERT(__is_write_reg(data_in));
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done || sync == ctx_swap);

        __associate_read_write_reg_pair_no_spill(data_out,data_in);
        __asm local_csr_wr[CMD_INDIRECT_REF_0, CSR_INDIRECT_TYPE(ind)]
        __asm  alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
        if (sync == sig_done)
        {
            __asm cls[tcam_lookup32, *data_out, address, 0, max_1], sig_done[*sig_ptr], indirect_ref
        }
        else
        {
            __asm cls[tcam_lookup32, *data_out, address, 0, max_1], ctx_swap[*sig_ptr], indirect_ref
        }
    }
    _INTRINSIC_END;
}


__intrinsic
void cmd_cls_tcam_lookup24_ind_ptr40(
    __xread void *data_out,                             /* lookuped data */
    __xwrite void *data_in,                             /* data to lookup */
    volatile void __addr40 __cls *address,              /* address to lookup */
    generic_ind_t ind,                                  /* indirect word */
    sync_t sync,                                        /* type of synchronization to use */
    SIGNAL *sig_ptr                                     /* signal to raise upon completion */
    )
{
    _INTRINSIC_BEGIN;
    {
        uint32_t hi_addr, low_addr;

        _INTRINSIC_CONVERT_HI_LO_ADDRESS(address);
        CT_ASSERT(__is_read_reg(data_out));
        CT_ASSERT(__is_write_reg(data_in));
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done || sync == ctx_swap);

        __associate_read_write_reg_pair_no_spill(data_out,data_in);
        __asm local_csr_wr[CMD_INDIRECT_REF_0, CSR_INDIRECT_TYPE(ind)]
        __asm  alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
        if (sync == sig_done)
        {
            __asm cls[tcam_lookup24, *data_out, hi_addr, <<8, low_addr, max_1], sig_done[*sig_ptr], indirect_ref
        }
        else
        {
            __asm cls[tcam_lookup24, *data_out, hi_addr, <<8, low_addr, max_1], ctx_swap[*sig_ptr], indirect_ref
        }
    }
    _INTRINSIC_END;
}


__intrinsic
void cmd_cls_tcam_lookup24_ind_ptr32(
    __xread void *data_out,                     /* lookuped data */
    __xwrite void *data_in,                     /* data to lookup */
    volatile void __cls *address,               /* address to lookup */
    generic_ind_t ind,                          /* indirect word */
    sync_t sync,                                /* type of synchronization to use */
    SIGNAL *sig_ptr                             /* signal to raise upon completion */
    )
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_read_reg(data_out));
        CT_ASSERT(__is_write_reg(data_in));
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done || sync == ctx_swap);

        __associate_read_write_reg_pair_no_spill(data_out,data_in);
        __asm local_csr_wr[CMD_INDIRECT_REF_0, CSR_INDIRECT_TYPE(ind)]
        __asm  alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
        if (sync == sig_done)
        {
            __asm cls[tcam_lookup24, *data_out, address, 0, max_1], sig_done[*sig_ptr], indirect_ref
        }
        else
        {
            __asm cls[tcam_lookup24, *data_out, address, 0, max_1], ctx_swap[*sig_ptr], indirect_ref
        }
    }
    _INTRINSIC_END;
}



__intrinsic
void cmd_cls_tcam_lookup16_ind_ptr40(
    __xread void *data_out,                             /* lookuped data */
    __xwrite void *data_in,                             /* data to lookup */
    volatile void __addr40 __cls *address,              /* address to lookup */
    generic_ind_t ind,                                  /* indirect word */
    sync_t sync,                                        /* type of synchronization to use */
    SIGNAL *sig_ptr                                     /* signal to raise upon completion */
    )
{
    _INTRINSIC_BEGIN;
    {
        uint32_t hi_addr, low_addr;

        _INTRINSIC_CONVERT_HI_LO_ADDRESS(address);
        CT_ASSERT(__is_read_reg(data_out));
        CT_ASSERT(__is_write_reg(data_in));
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done || sync == ctx_swap);

        __associate_read_write_reg_pair_no_spill(data_out,data_in);
        __asm local_csr_wr[CMD_INDIRECT_REF_0, CSR_INDIRECT_TYPE(ind)]
        __asm  alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
        if (sync == sig_done)
        {
            __asm cls[tcam_lookup16, *data_out, hi_addr, <<8, low_addr, max_2], sig_done[*sig_ptr], indirect_ref
        }
        else
        {
            __asm cls[tcam_lookup16, *data_out, hi_addr, <<8, low_addr, max_2], ctx_swap[*sig_ptr], indirect_ref
        }
    }
    _INTRINSIC_END;
}




__intrinsic
void cmd_cls_tcam_lookup16_ind_ptr32(
    __xread void *data_out,                     /* lookuped data */
    __xwrite void *data_in,                     /* data to lookup */
    volatile void __cls *address,               /* address to lookup */
    generic_ind_t ind,                          /* indirect word */
    sync_t sync,                                /* type of synchronization to use */
    SIGNAL *sig_ptr                             /* signal to raise upon completion */
    )
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_read_reg(data_out));
        CT_ASSERT(__is_write_reg(data_in));
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done || sync == ctx_swap);

        __associate_read_write_reg_pair_no_spill(data_out,data_in);
        __asm local_csr_wr[CMD_INDIRECT_REF_0, CSR_INDIRECT_TYPE(ind)]
        __asm  alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
        if (sync == sig_done)
        {
            __asm cls[tcam_lookup16, *data_out, address, 0, max_2], sig_done[*sig_ptr], indirect_ref
        }
        else
        {
            __asm cls[tcam_lookup16, *data_out, address, 0, max_2], ctx_swap[*sig_ptr], indirect_ref
        }
    }
    _INTRINSIC_END;
}


__intrinsic
void cmd_cls_tcam_lookup8_ind_ptr40(
    __xread void *data_out,                             /* lookuped data */
    __xwrite void *data_in,                             /* data to lookup */
    volatile void __addr40 __cls *address,              /* address to lookup */
    generic_ind_t ind,                                  /* indirect word */
    sync_t sync,                                        /* type of synchronization to use */
    SIGNAL *sig_ptr                                     /* signal to raise upon completion */
    )
{
    _INTRINSIC_BEGIN;
    {
        uint32_t hi_addr, low_addr;

        _INTRINSIC_CONVERT_HI_LO_ADDRESS(address);
        CT_ASSERT(__is_read_reg(data_out));
        CT_ASSERT(__is_write_reg(data_in));
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done || sync == ctx_swap);

        __associate_read_write_reg_pair_no_spill(data_out,data_in);
        __asm local_csr_wr[CMD_INDIRECT_REF_0, CSR_INDIRECT_TYPE(ind)]
        __asm  alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
        if (sync == sig_done)
        {
            __asm cls[tcam_lookup8, *data_out, hi_addr, <<8, low_addr, max_2], sig_done[*sig_ptr], indirect_ref
        }
        else
        {
            __asm cls[tcam_lookup8, *data_out, hi_addr, <<8, low_addr, max_2], ctx_swap[*sig_ptr], indirect_ref
        }
    }
    _INTRINSIC_END;
}


__intrinsic
void cmd_cls_tcam_lookup8_ind_ptr32(
    __xread void *data_out,                     /* lookuped data */
    __xwrite void *data_in,                     /* data to lookup */
    volatile void __cls *address,               /* address to lookup */
    generic_ind_t ind,                          /* indirect word */
    sync_t sync,                                /* type of synchronization to use */
    SIGNAL *sig_ptr                             /* signal to raise upon completion */
    )
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_read_reg(data_out));
        CT_ASSERT(__is_write_reg(data_in));
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done || sync == ctx_swap);

        __associate_read_write_reg_pair_no_spill(data_out,data_in);
        __asm local_csr_wr[CMD_INDIRECT_REF_0, CSR_INDIRECT_TYPE(ind)]
        __asm  alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
        if (sync == sig_done)
        {
            __asm cls[tcam_lookup8, *data_out, address, 0, max_2], sig_done[*sig_ptr], indirect_ref
        }
        else
        {
            __asm cls[tcam_lookup8, *data_out, address, 0, max_2], ctx_swap[*sig_ptr], indirect_ref
        }
    }
    _INTRINSIC_END;
}


/* Local scratch Content Addressable Memory lookup word. */
__intrinsic
void cmd_cls_cam_lookup_ptr32(
    __xread void *data_out,                         /* lookuped data */
    __xwrite void *data_in,                         /* data to lookup */
    volatile void __cls *address,                   /* address to lookup */
    uint32_t count,                             /* number of 64-bits entries */
    sync_t sync,                                    /* type of synchronization to use */
    SIGNAL *sig_ptr                                 /* signal to raise upon completion */
)
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_read_reg(data_out));
        CT_ASSERT(__is_write_reg(data_in));
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done || sync == ctx_swap);

        __associate_read_write_reg_pair_no_spill(data_out,data_in);

        {
            generic_ind_t ind;
            _INTRINSIC_OVERRIDE_LENGTH(ind, count - 1);
            if (__is_ct_const(count))
            {
                CT_ASSERT(count <= INTRINSIC_IND_ALU_COUNT_MAX);
            }
            else
            {
                CT_QPERFINFO_INDIRECT_REF(cmd_cls_cam_lookup_ptr32);
            }

            if (sync == sig_done)
            {
                __asm
                {
                    alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
                    cls[cam_lookup32, *data_out, address, 0, max_1], sig_done[*sig_ptr], indirect_ref
                }
            }
            else
            {
                __asm
                {
                    alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
                    cls[cam_lookup32, *data_out, address, 0, max_1], ctx_swap[*sig_ptr], indirect_ref
                }
            }
        }
    }
    _INTRINSIC_END;
}


void cmd_cls_cam_lookup_ptr40(
    __xread void *data_out,
    __xwrite void *data_in,
    volatile void __addr40 __cls *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
)
{
    _INTRINSIC_BEGIN;
    {
        _CLS_IMPLEMENT_CAM_MAX_1_COMMAND_PTR40(cam_lookup32, INTRINSIC_IND_ALU_COUNT_MAX, data_out, data_in);
    }
    _INTRINSIC_END;
}

/* Local scratch Content Addressable Memory lookup word with add */

__intrinsic
void cmd_cls_cam_lookup_add_ptr32(
    __xread void *data_out,                         /* lookuped data */
    __xwrite void *data_in,                         /* data to lookup */
    volatile void __cls *address,                   /* address to lookup */
    uint32_t count,                             /* number of 64-bits entries */
    sync_t sync,                                    /* type of synchronization to use */
    SIGNAL *sig_ptr                                 /* signal to raise upon completion */
)
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_read_reg(data_out));
        CT_ASSERT(__is_write_reg(data_in));
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done || sync == ctx_swap);

        __associate_read_write_reg_pair_no_spill(data_out,data_in);

        {
            generic_ind_t ind;
            _INTRINSIC_OVERRIDE_LENGTH(ind, count - 1);
            if (__is_ct_const(count))
            {
                CT_ASSERT(count<=16);
            }
            else
            {
                CT_QPERFINFO_INDIRECT_REF(cmd_cls_cam_lookup_add_ptr32);
            }

            if (sync == sig_done)
            {
                __asm
                {
                    alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
                    cls[cam_lookup32_add, *data_out, address, 0, max_1], sig_done[*sig_ptr], indirect_ref
                }
            }
            else
            {
                __asm
                {
                    alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
                    cls[cam_lookup32_add, *data_out, address, 0, max_1], ctx_swap[*sig_ptr], indirect_ref
                }
            }
        }
    }
    _INTRINSIC_END;
}


void cmd_cls_cam_lookup_add_ptr40(
    __xread void *data_out,
    __xwrite void *data_in,
    volatile void __addr40 __cls *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
)
{
    _INTRINSIC_BEGIN;
    {
        _CLS_IMPLEMENT_CAM_MAX_1_COMMAND_PTR40(cam_lookup32_add, 16, data_out, data_in);
    }
    _INTRINSIC_END;
}


/* Local scratch Content Addressable Memory lookup 24-bit */

__intrinsic
void cmd_cls_cam_lookup24_ptr32(
    __xread void *data_out,                     /* lookuped data */
    __xwrite void *data_in,                     /* data to lookup */
    volatile void __cls *address,               /* address to lookup */
    uint32_t count,                         /* number of 64-bits entries */
    sync_t sync,                                /* type of synchronization to use */
    SIGNAL *sig_ptr                             /* signal to raise upon completion */
)
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_read_reg(data_out));
        CT_ASSERT(__is_write_reg(data_in));
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done || sync == ctx_swap);

        __associate_read_write_reg_pair_no_spill(data_out,data_in);

        {
           generic_ind_t ind;
            _INTRINSIC_OVERRIDE_LENGTH(ind, count - 1);
            if (__is_ct_const(count))
            {
                CT_ASSERT(count <= INTRINSIC_IND_ALU_COUNT_MAX);
            }
            else
            {
                CT_QPERFINFO_INDIRECT_REF(cmd_cls_cam_lookup24_ptr32);
            }

            if (sync == sig_done)
            {
                __asm
                {
                    alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
                    cls[cam_lookup24, *data_out, address, 0, max_1], sig_done[*sig_ptr], indirect_ref
                }
            }
            else
            {
                __asm
                {
                    alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
                    cls[cam_lookup24, *data_out, address, 0, max_1], ctx_swap[*sig_ptr], indirect_ref
                }
            }
        }
    }
    _INTRINSIC_END;
}


void cmd_cls_cam_lookup24_ptr40(
    __xread void *data_out,
    __xwrite void *data_in,
    volatile void __addr40 __cls *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
)
{
    _INTRINSIC_BEGIN;
    {
        _CLS_IMPLEMENT_CAM_MAX_1_COMMAND_PTR40(cam_lookup24, INTRINSIC_IND_ALU_COUNT_MAX, data_out, data_in);
    }
    _INTRINSIC_END;
}


/* Local scratch Content Addressable Memory lookup 24-bits with add */
__intrinsic
void cmd_cls_cam_lookup24_add_ptr32(
    __xread void *data_out,                     /* lookuped data */
    __xwrite void *data_in,                     /* data to lookup */
    volatile void __cls *address,               /* address to lookup */
    uint32_t count,                             /* number of 64-bits entries */
    sync_t sync,                                /* type of synchronization to use */
    SIGNAL *sig_ptr                             /* signal to raise upon completion */
)
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_read_reg(data_out));
        CT_ASSERT(__is_write_reg(data_in));
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done || sync == ctx_swap);

        __associate_read_write_reg_pair_no_spill(data_out,data_in);
        {
            generic_ind_t ind;
            _INTRINSIC_OVERRIDE_LENGTH(ind, count - 1);
            if (__is_ct_const(count))
            {
                CT_ASSERT(count<=16);
            }
            else
            {
                CT_QPERFINFO_INDIRECT_REF(cmd_cls_cam_lookup24_add_ptr32);
            }

            if (sync == sig_done)
            {
                __asm
                {
                    alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
                    cls[cam_lookup24_add, *data_out, address, 0, max_1], sig_done[*sig_ptr], indirect_ref
                }
            }
            else
            {
                __asm
                {
                    alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
                    cls[cam_lookup24_add, *data_out, address, 0, max_1], ctx_swap[*sig_ptr], indirect_ref
                }
            }
        }
    }
    _INTRINSIC_END;
}

void cmd_cls_cam_lookup24_add_ptr40(
    __xread void *data_out,
    __xwrite void *data_in,
    volatile void __addr40 __cls *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
)
{
    _INTRINSIC_BEGIN;
    {
        _CLS_IMPLEMENT_CAM_MAX_1_COMMAND_PTR40(cam_lookup24_add, 16, data_out, data_in);
    }
    _INTRINSIC_END;
}

/* Local scratch Content Addressable Memory lookup 24-bits with add or increment usage */
__intrinsic
void cmd_cls_cam_lookup24_add_inc_ptr32(
    __xread void *data_out,                         /* lookuped data */
    __xwrite void *data_in,                         /* data to lookup */
    volatile void __cls *address,                   /* address to lookup */
    uint32_t count,                                 /* number of 64-bits entries */
    sync_t sync,                                    /* type of synchronization to use */
    SIGNAL *sig_ptr                                 /* signal to raise upon completion */
)
{
    _INTRINSIC_BEGIN;
    {
        __ct_assert(!( __is_nfp_arch(__nfp3800, __REVISION_A0)),"cmd_cls_cam_lookup24_add_inc_ptr32 is not supported with the selected chip");
        CT_ASSERT(__is_read_reg(data_out));
        CT_ASSERT(__is_write_reg(data_in));
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done || sync == ctx_swap);

        __associate_read_write_reg_pair_no_spill(data_out,data_in);

        {
            generic_ind_t ind;
            _INTRINSIC_OVERRIDE_LENGTH(ind, count - 1);

            if (__is_ct_const(count))
            {
                CT_ASSERT(count<=16);
            }
            else
            {
                CT_QPERFINFO_INDIRECT_REF(cmd_cls_cam_lookup24_add_inc_ptr32);
            }

            if (sync == sig_done)
            {
                __asm
                {
                    alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
                    cls[cam_lookup24_add_inc, *data_out, address, 0, max_1], sig_done[*sig_ptr], indirect_ref
                }
            }
            else
            {
                __asm
                {
                    alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
                    cls[cam_lookup24_add_inc, *data_out, address, 0, max_1], ctx_swap[*sig_ptr], indirect_ref
                }
            }
        }
    }
    _INTRINSIC_END;
}


void cmd_cls_cam_lookup24_add_inc_ptr40(
    __xread void *data_out,
    __xwrite void *data_in,
    volatile void __addr40 __cls *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
)
{
    __ct_assert(!( __is_nfp_arch(__nfp3800, __REVISION_A0)),"cmd_cls_cam_lookup24_add_inc_ptr40 is not supported with the selected chip");
    _INTRINSIC_BEGIN;
    {
        _CLS_IMPLEMENT_CAM_MAX_1_COMMAND_PTR40(cam_lookup24_add_inc, 16, data_out, data_in);
    }
    _INTRINSIC_END;
}

/* Local scratch Content Addressable Memory lookup 24-bits with add lock. */
__intrinsic
void cmd_cls_cam_lookup24_add_lock_ptr32(
    __xread void *data_out,
    __xwrite void *data_in,
    volatile void __cls *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
)
{
    _INTRINSIC_BEGIN;
    {
        __ct_assert(!( __is_nfp_arch(__nfp3800, __REVISION_A0)),"cmd_cls_cam_lookup24_add_lock_ptr32 is not supported with the selected chip");
        CT_ASSERT(__is_read_reg(data_out));
        CT_ASSERT(__is_write_reg(data_in));
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done || sync == ctx_swap);

        __associate_read_write_reg_pair_no_spill(data_out,data_in);
        {
            generic_ind_t ind;
            _INTRINSIC_OVERRIDE_LENGTH(ind, count - 1);
            if (__is_ct_const(count))
            {
                CT_ASSERT(count<=16);
            }
            else
            {
                CT_QPERFINFO_INDIRECT_REF(cmd_cls_cam_lookup24_add_lock_ptr32);
            }

            if (sync == sig_done)
            {
                __asm
                {
                    alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
                    cls[cam_lookup24_add_lock, *data_out, address, 0, max_1], sig_done[*sig_ptr], indirect_ref
                }
            }
            else
            {
                __asm
                {
                    alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
                    cls[cam_lookup24_add_lock, *data_out, address, 0, max_1], ctx_swap[*sig_ptr], indirect_ref
                }
            }
        }
    }
    _INTRINSIC_END;
}

void cmd_cls_cam_lookup24_add_lock_ptr40(
    __xread void *data_out,
    __xwrite void *data_in,
    volatile void __addr40 __cls *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
)
{
    _INTRINSIC_BEGIN;
    {
        __ct_assert(!( __is_nfp_arch(__nfp3800, __REVISION_A0)),"cmd_cls_cam_lookup24_add_lock_ptr40 is not supported with the selected chip");
        _CLS_IMPLEMENT_CAM_MAX_1_COMMAND_PTR40(cam_lookup24_add_lock, 16, data_out, data_in);
    }
    _INTRINSIC_END;
}


/* Local scratch Content Addressable Memory lookup 24-bits with add extend. */
__intrinsic
void cmd_cls_cam_lookup24_add_extend_ptr32(
    __xread void *data_out,
    __xwrite void *data_in,
    volatile void __cls *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
)
{
    _INTRINSIC_BEGIN;
    {
        __ct_assert(!( __is_nfp_arch(__nfp3800, __REVISION_A0)),"cmd_cls_cam_lookup24_add_extend_ptr32 is not supported with the selected chip");
        CT_ASSERT(__is_read_reg(data_out));
        CT_ASSERT(__is_write_reg(data_in));
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done || sync == ctx_swap);

        __associate_read_write_reg_pair_no_spill(data_out,data_in);
        {
            generic_ind_t ind;
            _INTRINSIC_OVERRIDE_LENGTH(ind, count - 1);
            if (__is_ct_const(count))
            {
                CT_ASSERT(count<=16);
            }
            else
            {
                CT_QPERFINFO_INDIRECT_REF(cmd_cls_cam_lookup24_add_extend_ptr32);
            }

            if (sync == sig_done)
            {
                __asm
                {
                    alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
                    cls[cam_lookup24_add_extend, *data_out, address, 0, max_1], sig_done[*sig_ptr], indirect_ref
                }
            }
            else
            {
                __asm
                {
                    alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
                    cls[cam_lookup24_add_extend, *data_out, address, 0, max_1], ctx_swap[*sig_ptr], indirect_ref
                }
            }
        }
    }
    _INTRINSIC_END;
}

void cmd_cls_cam_lookup24_add_extend_ptr40(
    __xread void *data_out,
    __xwrite void *data_in,
    volatile void __addr40 __cls *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
)
{
    _INTRINSIC_BEGIN;
    {
        __ct_assert(!( __is_nfp_arch(__nfp3800, __REVISION_A0)),"cmd_cls_cam_lookup24_add_extend_ptr40 is not supported with the selected chip");
        _CLS_IMPLEMENT_CAM_MAX_1_COMMAND_PTR40(cam_lookup24_add_extend, 16, data_out, data_in);
    }
    _INTRINSIC_END;
}

/* Local scratch Content Addressable Memory lookup 16-bits */
__intrinsic
void cmd_cls_cam_lookup16_ptr32(
    __xread void *data_out,                         /* lookuped data */
    __xwrite void *data_in,                         /* data to lookup */
    volatile void __cls *address,                   /* address to lookup */
    uint32_t count,                                 /* number of 64-bits entries */
    sync_t sync,                                    /* type of synchronization to use */
    SIGNAL *sig_ptr                                 /* signal to raise upon completion */
)
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_read_reg(data_out));
        CT_ASSERT(__is_write_reg(data_in));
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done || sync == ctx_swap);

        __associate_read_write_reg_pair_no_spill(data_out,data_in);

        {
            generic_ind_t ind;
            _INTRINSIC_OVERRIDE_LENGTH(ind, count - 1);

            if (__is_ct_const(count))
            {
                CT_ASSERT(count <= INTRINSIC_IND_ALU_COUNT_MAX);
            }
            else
            {
                CT_QPERFINFO_INDIRECT_REF(cmd_cls_cam_lookup16_ptr32);
            }

            if (sync == sig_done)
            {
                __asm
                {
                    alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
                    cls[cam_lookup16, *data_out, address, 0, max_2], sig_done[*sig_ptr], indirect_ref
                }
            }
            else
            {
                __asm
                {
                    alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
                    cls[cam_lookup16, *data_out, address, 0, max_2], ctx_swap[*sig_ptr], indirect_ref
                }
            }
        }
    }
    _INTRINSIC_END;
}


void cmd_cls_cam_lookup16_ptr40(
    __xread void *data_out,
    __xwrite void *data_in,
    volatile void __addr40 __cls *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
)
{
    _INTRINSIC_BEGIN;
    {
        _CLS_IMPLEMENT_CAM_MAX_2_COMMAND_PTR40(cam_lookup16, INTRINSIC_IND_ALU_COUNT_MAX, data_out, data_in);
    }
    _INTRINSIC_END;
}

/* Local scratch Content Addressable Memory lookup 16-bits with add */
__intrinsic
void cmd_cls_cam_lookup16_add_ptr32(
    __xread void *data_out,                         /* lookuped data */
    __xwrite void *data_in,                         /* data to lookup */
    volatile void __cls *address,                   /* address to lookup */
    uint32_t count,                                 /* number of 64-bits entries */
    sync_t sync,                                    /* type of synchronization to use */
    SIGNAL *sig_ptr                                 /* signal to raise upon completion */
)
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_read_reg(data_out));
        CT_ASSERT(__is_write_reg(data_in));
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done || sync == ctx_swap);

        __associate_read_write_reg_pair_no_spill(data_out,data_in);
        {
            generic_ind_t ind;
            _INTRINSIC_OVERRIDE_LENGTH(ind, count - 1);
            if (__is_ct_const(count))
            {
                CT_ASSERT(count<=16);
            }
            else
            {
                CT_QPERFINFO_INDIRECT_REF(cmd_cls_cam_lookup16_add_ptr32);
            }

            if (sync == sig_done)
            {
                __asm
                {
                    alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
                    cls[cam_lookup16_add, *data_out, address, 0, max_2], sig_done[*sig_ptr], indirect_ref
                }
            }
            else
            {
                __asm
                {
                    alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
                    cls[cam_lookup16_add, *data_out, address, 0, max_2], ctx_swap[*sig_ptr], indirect_ref
                }
            }
        }
    }
    _INTRINSIC_END;
}



void cmd_cls_cam_lookup16_add_ptr40(
    __xread void *data_out,
    __xwrite void *data_in,
    volatile void __addr40 __cls *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
)
{
    _INTRINSIC_BEGIN;
    {
        _CLS_IMPLEMENT_CAM_MAX_2_COMMAND_PTR40(cam_lookup16_add, 16, data_out, data_in);
    }
    _INTRINSIC_END;
}


/* Local scratch Content Addressable Memory lookup 8-bits */
__intrinsic
void cmd_cls_cam_lookup8_ptr32(
    __xread void *data_out,                     /* lookuped data */
    __xwrite void *data_in,                     /* data to lookup */
    volatile void __cls *address,               /* address to lookup */
    uint32_t count,                             /* number of 64-bits entries */
    sync_t sync,                                /* type of synchronization to use */
    SIGNAL *sig_ptr                             /* signal to raise upon completion */
)
{
    _INTRINSIC_BEGIN;
    {
        generic_ind_t ind;

        CT_ASSERT(__is_read_reg(data_out));
        CT_ASSERT(__is_write_reg(data_in));
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done || sync == ctx_swap);

        __associate_read_write_reg_pair_no_spill(data_out,data_in);
        {
            _INTRINSIC_OVERRIDE_LENGTH(ind, count - 1);
            if (__is_ct_const(count))
            {
                CT_ASSERT(count <= INTRINSIC_IND_ALU_COUNT_MAX);
            }
            else
            {
                CT_QPERFINFO_INDIRECT_REF(cmd_cls_cam_lookup8_ptr32);
            }

            if (sync == sig_done)
            {

                __asm
                {
                    alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
                    cls[cam_lookup8, *data_out, address, 0, max_2], sig_done[*sig_ptr], indirect_ref
                }
            }
            else
            {
                __asm
                {
                    alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
                    cls[cam_lookup8, *data_out, address, 0, max_2], ctx_swap[*sig_ptr], indirect_ref
                }
            }
        }
    }
    _INTRINSIC_END;
}

void cmd_cls_cam_lookup8_ptr40(
    __xread void *data_out,
    __xwrite void *data_in,
    volatile void __addr40 __cls *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
)
{
    _INTRINSIC_BEGIN;
    {
        _CLS_IMPLEMENT_CAM_MAX_2_COMMAND_PTR40(cam_lookup8, INTRINSIC_IND_ALU_COUNT_MAX, data_out, data_in);
    }
    _INTRINSIC_END;
}

/* Local scratch Content Addressable Memory lookup 8-bits with add */
__intrinsic
void cmd_cls_cam_lookup8_add_ptr32(
    __xread void *data_out,                     /* lookuped data */
    __xwrite void *data_in,                     /* data to lookup */
    volatile void __cls *address,               /* address to lookup */
    uint32_t count,                             /* number of 64-bits entries */
    sync_t sync,                                /* type of synchronization to use */
    SIGNAL *sig_ptr                             /* signal to raise upon completion */
)
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_read_reg(data_out));
        CT_ASSERT(__is_write_reg(data_in));
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done || sync == ctx_swap);

        __associate_read_write_reg_pair_no_spill(data_out,data_in);

        {
            generic_ind_t ind;
            _INTRINSIC_OVERRIDE_LENGTH(ind, count - 1);
            if (__is_ct_const(count))
            {
                CT_ASSERT(count<=16);
            }
            else
            {
                CT_QPERFINFO_INDIRECT_REF(cmd_cls_cam_lookup8_add_ptr32);
            }

            if (sync == sig_done)
            {
                __asm
                {
                    alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
                    cls[cam_lookup8_add, *data_out, address, 0, max_2], sig_done[*sig_ptr], indirect_ref
                }
            }
            else
            {
                __asm
                {
                    alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
                    cls[cam_lookup8_add, *data_out, address, 0, max_2], ctx_swap[*sig_ptr], indirect_ref
                }
            }
        }
    }
    _INTRINSIC_END;
}


void cmd_cls_cam_lookup8_add_ptr40(
    __xread void *data_out,
    __xwrite void *data_in,
    volatile void __addr40 __cls *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
)
{
    _INTRINSIC_BEGIN;
    {
        _CLS_IMPLEMENT_CAM_MAX_2_COMMAND_PTR40(cam_lookup8_add, 16, data_out, data_in);
    }
    _INTRINSIC_END;
}


/* Local scratch TCAM lookup word */
__intrinsic
void cmd_cls_tcam_lookup_ptr32(
    __xread void *data_out,                         /* lookuped data */
    __xwrite void *data_in,                         /* data to lookup */
    volatile void __cls *address,                   /* address to lookup */
    uint32_t count,                                 /* number of 64-bits entries */
    sync_t sync,                                    /* type of synchronization to use */
    SIGNAL *sig_ptr                                 /* signal to raise upon completion */
)
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_read_reg(data_out));
        CT_ASSERT(__is_write_reg(data_in));
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done || sync == ctx_swap);

        __associate_read_write_reg_pair_no_spill(data_out,data_in);

        {

            generic_ind_t ind;
            _INTRINSIC_OVERRIDE_LENGTH(ind, count - 1);
            if (__is_ct_const(count))
            {
                CT_ASSERT(count<=16);
            }
            else
            {
                CT_QPERFINFO_INDIRECT_REF(cmd_cls_tcam_lookup_ptr32);
            }

            if (sync == sig_done)
            {
                __asm
                {
                    alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
                    cls[tcam_lookup32, *data_out, address, 0, max_1], sig_done[*sig_ptr], indirect_ref
                }
            }
            else
            {
                __asm
                {
                    alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
                    cls[tcam_lookup32, *data_out, address, 0, max_1], ctx_swap[*sig_ptr], indirect_ref
                }
            }
        }
    }
    _INTRINSIC_END;
}


void cmd_cls_tcam_lookup_ptr40(
    __xread void *data_out,
    __xwrite void *data_in,
    volatile void __addr40 __cls *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
)
{
    _INTRINSIC_BEGIN;
    {
        _CLS_IMPLEMENT_CAM_MAX_1_COMMAND_PTR40(tcam_lookup32, 16, data_out, data_in);
    }
    _INTRINSIC_END;
}


/* Local scratch TCAM lookup 24-bits  */
__intrinsic
void cmd_cls_tcam_lookup24_ptr32(
    __xread void *data_out,                         /* lookuped data */
    __xwrite void *data_in,                         /* data to lookup */
    volatile void __cls *address,                   /* address to lookup */
    uint32_t count,                                 /* number of 64-bits entries */
    sync_t sync,                                    /* type of synchronization to use */
    SIGNAL *sig_ptr                                 /* signal to raise upon completion */
)
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_read_reg(data_out));
        CT_ASSERT(__is_write_reg(data_in));
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done || sync == ctx_swap);

        __associate_read_write_reg_pair_no_spill(data_out,data_in);

        {
            generic_ind_t ind;
            _INTRINSIC_OVERRIDE_LENGTH(ind, count - 1);
            if (__is_ct_const(count))
            {
                CT_ASSERT(count<=16);
            }
            else
            {
                CT_QPERFINFO_INDIRECT_REF(cmd_cls_tcam_lookup24_ptr32);
            }

            if (sync == sig_done)
            {
                __asm
                {
                    alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
                    cls[tcam_lookup24, *data_out, address, 0, max_1], sig_done[*sig_ptr], indirect_ref
                }
            }
            else
            {
                __asm
                {
                    alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
                    cls[tcam_lookup24, *data_out, address, 0, max_1], ctx_swap[*sig_ptr], indirect_ref
                }
            }
        }
    }
    _INTRINSIC_END;
}

void cmd_cls_tcam_lookup24_ptr40(
    __xread void *data_out,
    __xwrite void *data_in,
    volatile void __addr40 __cls *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
)
{
    _INTRINSIC_BEGIN;
    {
        _CLS_IMPLEMENT_CAM_MAX_1_COMMAND_PTR40(tcam_lookup24, 16, data_out, data_in);
    }
    _INTRINSIC_END;
}


/* Local scratch TCAM lookup 16-bits */
__intrinsic
void cmd_cls_tcam_lookup16_ptr32(
    __xread void *data_out,                         /* lookuped data */
    __xwrite void *data_in,                         /* data to lookup */
    volatile void __cls *address,                   /* address to lookup */
    uint32_t count,                                 /* number of 64-bits entries */
    sync_t sync,                                    /* type of synchronization to use */
    SIGNAL *sig_ptr                                 /* signal to raise upon completion */
)
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_read_reg(data_out));
        CT_ASSERT(__is_write_reg(data_in));
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done || sync == ctx_swap);

        __associate_read_write_reg_pair_no_spill(data_out,data_in);

        {
            generic_ind_t ind;
            _INTRINSIC_OVERRIDE_LENGTH(ind, count - 1);
            if (__is_ct_const(count))
            {
                CT_ASSERT(count<=16);
            }
            else
            {
                CT_QPERFINFO_INDIRECT_REF(cmd_cls_tcam_lookup16_ptr32);
            }

            if (sync == sig_done)
            {
                __asm
                {
                    alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
                    cls[tcam_lookup16, *data_out, address, 0, max_2], sig_done[*sig_ptr], indirect_ref
                }
            }
            else
            {
                __asm
                {
                    alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
                    cls[tcam_lookup16, *data_out, address, 0, max_2], ctx_swap[*sig_ptr], indirect_ref
                }
            }
        }
    }
    _INTRINSIC_END;
}

void cmd_cls_tcam_lookup16_ptr40(
    __xread void *data_out,
    __xwrite void *data_in,
    volatile void __addr40 __cls *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
)
{
    _INTRINSIC_BEGIN;
    {
        _CLS_IMPLEMENT_CAM_MAX_2_COMMAND_PTR40(tcam_lookup16, 16, data_out, data_in);
    }
    _INTRINSIC_END;
}




/* Local scratch TCAM lookup 8-bits */

__intrinsic
void cmd_cls_tcam_lookup8_ptr32(
    __xread void *data_out,                         /* lookuped data */
    __xwrite void *data_in,                         /* data to lookup */
    volatile void __cls *address,                   /* address to lookup */
    uint32_t count,                                 /* number of 64-bits entries */
    sync_t sync,                                    /* type of synchronization to use */
    SIGNAL *sig_ptr                                 /* signal to raise upon completion */
)
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_read_reg(data_out));
        CT_ASSERT(__is_write_reg(data_in));
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done || sync == ctx_swap);

        __associate_read_write_reg_pair_no_spill(data_out,data_in);

        {
            generic_ind_t ind;
            _INTRINSIC_OVERRIDE_LENGTH(ind, count - 1);
           if (__is_ct_const(count))
            {
                CT_ASSERT(count<=16);
            }
            else
            {
                CT_QPERFINFO_INDIRECT_REF(cmd_cls_tcam_lookup8_ptr32);
            }

            if (sync == sig_done)
            {
                __asm
                {
                    alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
                    cls[tcam_lookup8, *data_out, address, 0, max_2], sig_done[*sig_ptr], indirect_ref
                }
            }
            else
            {
                __asm
                {
                    alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
                    cls[tcam_lookup8, *data_out, address, 0, max_2], ctx_swap[*sig_ptr], indirect_ref
                }
            }
        }
    }
    _INTRINSIC_END;
}

void cmd_cls_tcam_lookup8_ptr40(
    __xread void *data_out,
    __xwrite void *data_in,
    volatile void __addr40 __cls *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
)
{
    _INTRINSIC_BEGIN;
    {
        _CLS_IMPLEMENT_CAM_MAX_2_COMMAND_PTR40(tcam_lookup8, 16, data_out, data_in);
    }
    _INTRINSIC_END;
}


/* Local scratch memory test and clear bits   */

void cmd_cls_test_and_clear_bits_ptr32(
    __xread uint32_t *val,                              /* returned pre-modified value            */
    __xwrite uint32_t *mask,                            /* mask of bits to clear                  */
    volatile void __cls *address,                       /* address to clear                       */
    uint32_t count,                                     /* number of words                        */
    sync_t sync,                                        /* must be sig_done                       */
    SIGNAL *sig_ptr                                     /* signal to raise upon completion        */
)
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_read_reg(val));
        CT_ASSERT(__is_write_reg(mask));
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done || sync == ctx_swap);

        __associate_read_write_reg_pair_no_spill(val, mask);
        if (__is_ct_const(count) && count <= 8)
        {
            CT_ASSERT(count != 0);
            if (sync == sig_done)
            {
                __asm cls[test_clr, *val, address, 0, __ct_const_val(count)], sig_done[*sig_ptr]
            }
            else
            {
                __asm cls[test_clr, *val, address, 0, __ct_const_val(count)], ctx_swap[*sig_ptr]
            }
        }
        else
        {
            if (!__is_ct_const(count))
            {
                CT_QPERFINFO_INDIRECT_REF(cls_test_clr);
            }
            {
                _INTRINSIC_IND_ALU_COUNT_MIN_MAX(count, 1, 8);
                if (sync == sig_done)
                {
                    __asm cls[test_clr, *val, address, 0, __ct_const_val(count)], sig_done[*sig_ptr], indirect_ref
                }
                else
                {
                    __asm cls[test_clr, *val, address, 0, __ct_const_val(count)], ctx_swap[*sig_ptr], indirect_ref
                }
            }
        }
    }
    _INTRINSIC_END;
}

void cmd_cls_test_and_clear_bits_ptr40(
    __xread uint32_t *val,
    __xwrite uint32_t *mask,
    volatile void __addr40 __cls *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
)
{
    _INTRINSIC_BEGIN;
    {
        _CLS_IMPLEMENT_TEST_COMMAND_PTR40(test_clr, 8, mask);
    }
    _INTRINSIC_END;
}


__intrinsic
void cmd_cls_test_and_clear_bits_ind_ptr40(
    __xread uint32_t *val,
    __xwrite uint32_t *mask,
    volatile void __addr40 __cls *address,
    uint32_t max_nn,
    generic_ind_t ind,
    sync_t sync,
    SIGNAL *sig_ptr
    )
{
    _INTRINSIC_BEGIN;
    {
        uint32_t hi_addr, low_addr;

        _INTRINSIC_CONVERT_HI_LO_ADDRESS(address);
        CT_ASSERT(__is_read_reg(val));
        CT_ASSERT(__is_write_reg(mask));
        CT_ASSERT(__is_ct_const(max_nn));
        CT_ASSERT(max_nn > 0 && max_nn <= 32);
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done || sync == ctx_swap);
        __associate_read_write_reg_pair_no_spill(val, mask);
        __asm local_csr_wr[CMD_INDIRECT_REF_0, CSR_INDIRECT_TYPE(ind)]
        __asm  alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
        if (sync == sig_done)
        {
            __asm cls[test_clr, *val, hi_addr, <<8, low_addr, __ct_const_val(max_nn)], sig_done[*sig_ptr], indirect_ref
        }
        else
        {
            __asm cls[test_clr, *val, hi_addr, <<8, low_addr, __ct_const_val(max_nn)], ctx_swap[*sig_ptr], indirect_ref
        }
    }
    _INTRINSIC_END;
}


__intrinsic
void cmd_cls_test_and_clear_bits_ind_ptr32(
    __xread uint32_t *val,
    __xwrite uint32_t *mask,
    volatile void __cls *address,
    uint32_t max_nn,
    generic_ind_t ind,
    sync_t sync,
    SIGNAL *sig_ptr
    )
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_read_reg(val));
        CT_ASSERT(__is_write_reg(mask));
        CT_ASSERT(__is_ct_const(max_nn));
        CT_ASSERT(max_nn > 0 && max_nn <= 32);
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done || sync == ctx_swap);
        __associate_read_write_reg_pair_no_spill(val, mask);
        __asm local_csr_wr[CMD_INDIRECT_REF_0, CSR_INDIRECT_TYPE(ind)]
        __asm  alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
        if (sync == sig_done)
        {
            __asm cls[test_clr, *val, address, 0, __ct_const_val(max_nn)], sig_done[*sig_ptr], indirect_ref
        }
        else
        {
            __asm cls[test_clr, *val, address, 0, __ct_const_val(max_nn)], ctx_swap[*sig_ptr], indirect_ref
        }
    }
    _INTRINSIC_END;
}


__intrinsic
void cmd_cls_test_and_set_bits_ind_ptr40(
    __xread uint32_t *val,
    __xwrite uint32_t *mask,
    volatile void __addr40 __cls *address,
    uint32_t max_nn,
    generic_ind_t ind,
    sync_t sync,
    SIGNAL *sig_ptr
    )
{
    _INTRINSIC_BEGIN;
    {
        uint32_t hi_addr, low_addr;

        _INTRINSIC_CONVERT_HI_LO_ADDRESS(address);
        CT_ASSERT(__is_read_reg(val));
        CT_ASSERT(__is_write_reg(mask));
        CT_ASSERT(__is_ct_const(max_nn));
        CT_ASSERT(max_nn > 0 && max_nn <= 32);
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done || sync == ctx_swap);
        __associate_read_write_reg_pair_no_spill(val, mask);
        __asm local_csr_wr[CMD_INDIRECT_REF_0, CSR_INDIRECT_TYPE(ind)]
        __asm  alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
        if (sync == sig_done)
        {
            __asm cls[test_set, *val, hi_addr, <<8, low_addr, __ct_const_val(max_nn)], sig_done[*sig_ptr], indirect_ref
        }
        else
        {
            __asm cls[test_set, *val, hi_addr, <<8, low_addr, __ct_const_val(max_nn)], ctx_swap[*sig_ptr], indirect_ref
        }
    }
    _INTRINSIC_END;
}


__intrinsic
void cmd_cls_test_and_set_bits_ind_ptr32(
    __xread uint32_t *val,
    __xwrite uint32_t *mask,
    volatile void __cls *address,
    uint32_t max_nn,
    generic_ind_t ind,
    sync_t sync,
    SIGNAL *sig_ptr
    )
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_read_reg(val));
        CT_ASSERT(__is_write_reg(mask));
        CT_ASSERT(__is_ct_const(max_nn));
        CT_ASSERT(max_nn > 0 && max_nn <= 32);
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done || sync == ctx_swap);
        __associate_read_write_reg_pair_no_spill(val, mask);
        __asm local_csr_wr[CMD_INDIRECT_REF_0, CSR_INDIRECT_TYPE(ind)]
        __asm  alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
        if (sync == sig_done)
        {
            __asm cls[test_set, *val, address, 0, __ct_const_val(max_nn)], sig_done[*sig_ptr], indirect_ref
        }
        else
        {
            __asm cls[test_set, *val, address, 0, __ct_const_val(max_nn)], ctx_swap[*sig_ptr], indirect_ref
        }
    }
    _INTRINSIC_END;
}


/* Local scratch memory test and add with saturation  */

void cmd_cls_test_and_add_sat_ptr32(
    __xread uint32_t *val,                              /* returned pre-modified value            */
    __xwrite uint32_t *data,                            /* data to add                            */
    volatile void __cls *address,                       /* address to add                         */
    uint32_t count,                                     /* number of words                        */
    sync_t sync,                                        /* must be sig_done                       */
    SIGNAL *sig_ptr                                     /* signal to raise upon completion        */
)
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_read_reg(val));
        CT_ASSERT(__is_write_reg(data));
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done || sync == ctx_swap);

        __associate_read_write_reg_pair_no_spill(val, data);
        if (__is_ct_const(count) && count <= 8)
        {
            CT_ASSERT(count != 0);
            if (sync == sig_done)
            {
                __asm cls[test_addsat, *val, address, 0, __ct_const_val(count)], sig_done[*sig_ptr]
            }
            else
            {
                __asm cls[test_addsat, *val, address, 0, __ct_const_val(count)], ctx_swap[*sig_ptr]
            }
        }
        else
        {
            if (!__is_ct_const(count))
            {
                CT_QPERFINFO_INDIRECT_REF(cls_test_addsat);
            }
            {
                _INTRINSIC_IND_ALU_COUNT_MIN_MAX(count, 1, 8);
                if (sync == sig_done)
                {
                    __asm cls[test_addsat, *val, address, 0, __ct_const_val(count)], sig_done[*sig_ptr], indirect_ref
                }
                else
                {
                    __asm cls[test_addsat, *val, address, 0, __ct_const_val(count)], ctx_swap[*sig_ptr], indirect_ref
                }
            }
        }
    }
    _INTRINSIC_END;
}

void cmd_cls_test_and_add_sat_ptr40(
    __xread uint32_t *val,
    __xwrite uint32_t *data,
    volatile void __addr40 __cls *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
)
{
    _INTRINSIC_BEGIN;
    {
        _CLS_IMPLEMENT_TEST_COMMAND_PTR40(test_addsat, 8, data);
    }
    _INTRINSIC_END;
}


__intrinsic
void cmd_cls_test_and_add_sat_ind_ptr40(
    __xread uint32_t *val,
    __xwrite uint32_t *data,
    volatile void __addr40 __cls *address,
    uint32_t max_nn,
    generic_ind_t ind,
    sync_t sync,
    SIGNAL *sig_ptr
    )
{
    _INTRINSIC_BEGIN;
    {
        uint32_t hi_addr, low_addr;

        _INTRINSIC_CONVERT_HI_LO_ADDRESS(address);
        CT_ASSERT(__is_read_reg(val));
        CT_ASSERT(__is_write_reg(data));
        CT_ASSERT(__is_ct_const(max_nn));
        CT_ASSERT(max_nn > 0 && max_nn <= 32);
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done || sync == ctx_swap);
        __associate_read_write_reg_pair_no_spill(val, data);
        __asm local_csr_wr[CMD_INDIRECT_REF_0, CSR_INDIRECT_TYPE(ind)]
        __asm  alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
        if (sync == sig_done)
        {
            __asm cls[test_addsat, *val, hi_addr, <<8, low_addr, __ct_const_val(max_nn)], sig_done[*sig_ptr], indirect_ref
        }
        else
        {
            __asm cls[test_addsat, *val, hi_addr, <<8, low_addr, __ct_const_val(max_nn)], ctx_swap[*sig_ptr], indirect_ref
        }
    }
    _INTRINSIC_END;
}


__intrinsic
void cmd_cls_test_and_add_sat_ind_ptr32(
    __xread uint32_t *val,
    __xwrite uint32_t *data,
    volatile void __cls *address,
    uint32_t max_nn,
    generic_ind_t ind,
    sync_t sync,
    SIGNAL *sig_ptr
    )
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_read_reg(val));
        CT_ASSERT(__is_write_reg(data));
        CT_ASSERT(__is_ct_const(max_nn));
        CT_ASSERT(max_nn > 0 && max_nn <= 32);
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done || sync == ctx_swap);
        __associate_read_write_reg_pair_no_spill(val, data);
        __asm  alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
        if (sync == sig_done)
        {
            __asm cls[test_addsat, *val, address, 0, __ct_const_val(max_nn)], sig_done[*sig_ptr], indirect_ref
        }
        else
        {
            __asm cls[test_addsat, *val, address, 0, __ct_const_val(max_nn)], ctx_swap[*sig_ptr], indirect_ref
        }
    }
    _INTRINSIC_END;
}



__intrinsic
void cmd_cls_test_and_sub_sat_ind_ptr40(
    __xread uint32_t *val,
    __xwrite uint32_t *data,
    volatile void __addr40 __cls *address,
    uint32_t max_nn,
    generic_ind_t ind,
    sync_t sync,
    SIGNAL *sig_ptr
    )
{
    _INTRINSIC_BEGIN;
    {
        uint32_t hi_addr, low_addr;

        _INTRINSIC_CONVERT_HI_LO_ADDRESS(address);
        CT_ASSERT(__is_read_reg(val));
        CT_ASSERT(__is_write_reg(data));
        CT_ASSERT(__is_ct_const(max_nn));
        CT_ASSERT(max_nn > 0 && max_nn <= 32);
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done || sync == ctx_swap);
        __associate_read_write_reg_pair_no_spill(val, data);
        __asm local_csr_wr[CMD_INDIRECT_REF_0, CSR_INDIRECT_TYPE(ind)]
        __asm  alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
        if (sync == sig_done)
        {
            __asm cls[test_subsat, *val, hi_addr, <<8, low_addr, __ct_const_val(max_nn)], sig_done[*sig_ptr], indirect_ref
        }
        else
        {
            __asm cls[test_subsat, *val, hi_addr, <<8, low_addr, __ct_const_val(max_nn)], ctx_swap[*sig_ptr], indirect_ref
        }
    }
    _INTRINSIC_END;
}


__intrinsic
void cmd_cls_test_and_sub_sat_ind_ptr32(
    __xread uint32_t *val,
    __xwrite uint32_t *data,
    volatile void __cls *address,
    uint32_t max_nn,
    generic_ind_t ind,
    sync_t sync,
    SIGNAL *sig_ptr
    )
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_read_reg(val));
        CT_ASSERT(__is_write_reg(data));
        CT_ASSERT(__is_ct_const(max_nn));
        CT_ASSERT(max_nn > 0 && max_nn <= 32);
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done || sync == ctx_swap);
        __associate_read_write_reg_pair_no_spill(val, data);
        __asm  alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
        if (sync == sig_done)
        {
            __asm cls[test_subsat, *val, address, 0, __ct_const_val(max_nn)], sig_done[*sig_ptr], indirect_ref
        }
        else
        {
            __asm cls[test_subsat, *val, address, 0, __ct_const_val(max_nn)], ctx_swap[*sig_ptr], indirect_ref
        }
    }
    _INTRINSIC_END;
}


/* Local scratch memory swap bits          */

void cmd_cls_swap_ptr32(
    __xwrite void *data,                                /* data to swap                          */
    volatile void __cls *address,                       /* address to swap                        */
    uint32_t count,                                     /* number of words                        */
    sync_t sync,                                        /* type of synchronization to use         */
    SIGNAL *sig_ptr                                     /* signal to raise upon completion        */
)
{
    _INTRINSIC_BEGIN;

    CT_ASSERT(__is_write_reg(data));
    CT_ASSERT(__is_ct_const(sync));
    CT_ASSERT(sync == sig_done || sync == ctx_swap);

    if (__is_ct_const(count) && count <= 8)
    {

        CT_ASSERT(count != 0);
        if (sync == sig_done)
        {
            __asm cls[swap, *data, address, 0, __ct_const_val(count)], sig_done[*sig_ptr]
        }
        else
        {
            __asm cls[swap, *data,  address, 0, __ct_const_val(count)], ctx_swap[*sig_ptr]
        }
    }
    else
    {
        if (__is_ct_const(count))
        {
            CT_ASSERT(count <= 32);
        }
        else
        {
            RT_RANGE_ASSERT(count <= 32);                    /* Enable trapping of out of bound values during run time */
        }
        {
            generic_ind_t ind;

            _INTRINSIC_OVERRIDE_LENGTH(ind, count - 1);

            if (sync == sig_done)
            {
                __asm
                {
                    alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
                    cls[swap, *data,  address, 0, __ct_const_val(count)], sig_done[*sig_ptr], indirect_ref
                }
            }
            else
            {
                __asm
                {
                    alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
                    cls[swap, *data, address, 0, __ct_const_val(count)], ctx_swap[*sig_ptr], indirect_ref
                }
            }
        }
   }
    _INTRINSIC_END;
}

__intrinsic
void cmd_cls_swap_ptr40(
    __xwrite void *data,
    volatile void __addr40 __cls *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
)
{
    _INTRINSIC_BEGIN;
    {
        _CLS_IMPLEMENT_COMMAND_PTR40(swap, 32, 0, data);
    }
    _INTRINSIC_END;
}

/* Local scratch memory metering operation         */
void cmd_cls_meter_ptr32(
    __xread uint32_t *val,
    __xwrite uint32_t *data,
    volatile void __cls *address,
    uint32_t rfc_type,
    uint32_t color,
    sync_t sync,
    SIGNAL *sig_ptr
)
{
    _INTRINSIC_BEGIN;

    CT_ASSERT(__is_read_reg(val));
    CT_ASSERT(__is_write_reg(data));
    CT_ASSERT(__is_ct_const(sync));
    CT_ASSERT(sync == sig_done || sync == ctx_swap);

    __associate_read_write_reg_pair_no_spill(val, data);

    if (__is_ct_const(rfc_type))
    {
        CT_ASSERT(rfc_type <= 1);
    }
    else
    {
        RT_RANGE_ASSERT(rfc_type <= 1);                    /* Enable trapping of out of bound values during run time */
    }

    if (__is_ct_const(color))
    {
        CT_ASSERT(color <= 2);
    }
    else
    {
        RT_RANGE_ASSERT(color <= 2);                    /* Enable trapping of out of bound values during run time */
    }
    {
        __gpr uint32_t meter_address;

        meter_address = ((rfc_type & 0x1) << 18) | ((color & 0x3) << 16) | ((uint32_t) address & 0xFFF8);

        if (sync == sig_done)
        {
            __asm
            {
                cls[meter, *val,  meter_address, 0, __ct_const_val(1)], sig_done[*sig_ptr]
            }
        }
        else
        {
            __asm
            {
                cls[meter, *val, meter_address, 0, __ct_const_val(1)], ctx_swap[*sig_ptr]
           }
        }
    }
    _INTRINSIC_END;
}

void cmd_cls_meter_ptr40(
    __xread uint32_t *val,                          /* returned pre-modified value            */
    __xwrite uint32_t *data,                        /* data to add                            */
    volatile void __addr40 __cls *address,
    uint32_t rfc_type,
    uint32_t color,
    sync_t sync,
    SIGNAL *sig_ptr
)
{
    _INTRINSIC_BEGIN;

    CT_ASSERT(__is_read_reg(val));
    CT_ASSERT(__is_write_reg(data));
    CT_ASSERT(__is_ct_const(sync));
    CT_ASSERT(sync == sig_done || sync == ctx_swap);

    __associate_read_write_reg_pair_no_spill(val, data);

    if (__is_ct_const(rfc_type))
    {
        CT_ASSERT(rfc_type <= 1);
    }
    else
    {
        RT_RANGE_ASSERT(rfc_type <= 1);                    /* Enable trapping of out of bound values during run time */
    }

    if (__is_ct_const(color))
    {
        CT_ASSERT(color <= 2);
    }
    else
    {
        RT_RANGE_ASSERT(color <= 2);                    /* Enable trapping of out of bound values during run time */
    }
    {
        __gpr uint32_t island_id = (uint32_t)((uint64_t) address >> 8) & 0xfc000000;
        __gpr uint32_t meter_address = (uint32_t) address;

        meter_address = ((rfc_type & 0x1) << 18) | ((color & 0x3) << 16) | ((uint32_t) address & 0xFFF8);
        if (sync == sig_done)
        {
            __asm
            {
                cls[meter, *val,  island_id, << 8, meter_address, __ct_const_val(1)], sig_done[*sig_ptr]
            }
        }
        else
        {
            __asm
            {
                cls[meter, *val, island_id, << 8, meter_address, __ct_const_val(1)], ctx_swap[*sig_ptr]
           }
        }
    }
    _INTRINSIC_END;
}


/* Local scratch memory statistic         */

void cmd_cls_statistic_ptr32(
    __xwrite void *data,                                /* data to swap                          */
    volatile void __cls *address,                       /* address to swap                        */
    uint32_t count,                                     /* number of words                        */
    sync_t sync,                                        /* type of synchronization to use         */
    SIGNAL *sig_ptr                                     /* signal to raise upon completion        */
)
{
    _INTRINSIC_BEGIN;

    CT_ASSERT(__is_write_reg(data));
    CT_ASSERT(__is_ct_const(sync));
    CT_ASSERT(sync == sig_done || sync == ctx_swap);

    if (__is_ct_const(count))
    {
        CT_ASSERT(count == 1);
    }
    else
    {
        RT_RANGE_ASSERT(count == 1);                    /* Enable trapping of out of bound values during run time */
    }
    {
        if (sync == sig_done)
        {
            __asm
            {
                cls[statistic, *data,  address, 0, count], sig_done[*sig_ptr]
            }
        }
        else
        {
            __asm
            {
                cls[statistic, *data, address, 0, count], ctx_swap[*sig_ptr]
            }
        }
    }
    _INTRINSIC_END;
}


__intrinsic
void cmd_cls_statistic_ptr40(
    __xwrite void *data,
    volatile void __addr40 __cls *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
)
{
    _INTRINSIC_BEGIN;

    CT_ASSERT(__is_write_reg(data));
    CT_ASSERT(__is_ct_const(sync));
    CT_ASSERT(sync == sig_done || sync == ctx_swap);

    if (__is_ct_const(count))
    {
        CT_ASSERT(count == 1);
    }
    else
    {
        RT_RANGE_ASSERT(count == 1);                    /* Enable trapping of out of bound values during run time */
    }
    {
        uint32_t hi_addr, low_addr;
        _INTRINSIC_CONVERT_HI_LO_ADDRESS(address);

        if (sync == sig_done)
        {
            __asm
            {
                cls[statistic, *data,  hi_addr, << 8, low_addr, count], sig_done[*sig_ptr]
            }
        }
        else
        {
            __asm
            {
                cls[statistic, *data, hi_addr, << 8, low_addr, count], ctx_swap[*sig_ptr]
            }
        }
    }
    _INTRINSIC_END;
}


/* Local scratch memory statistic immediate     */

void cmd_cls_statistic_imm_ptr32(
    volatile void __cls *address,                       /* address to sub                         */
    uint32_t value                                      /* immediate value                        */
)
{
    _INTRINSIC_BEGIN;
    {
        _CLS_IMPLEMENT_IMM_COMMAND(statistic_imm);
    }
    _INTRINSIC_END;
}



__intrinsic
void cmd_cls_statistic_imm_ptr40(
    volatile void __addr40 __cls *address,
    uint32_t value
)
{
    _INTRINSIC_BEGIN;
    {
        _CLS_IMPLEMENT_IMM_COMMAND_PTR40(statistic_imm);
    }
    _INTRINSIC_END;
}

/* Local scratch memory test and add  */

void cmd_cls_test_and_add_ptr32(
    __xread uint32_t *val,                              /* returned pre-modified value            */
    __xwrite uint32_t *data,                            /* data to add                            */
    volatile void __cls *address,                       /* address to add                         */
    uint32_t count,                                     /* number of words                        */
    sync_t sync,                                        /* must be sig_done                       */
    SIGNAL *sig_ptr                                     /* signal to raise upon completion        */
)
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_read_reg(val));
        CT_ASSERT(__is_write_reg(data));
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done || sync == ctx_swap);

        __associate_read_write_reg_pair_no_spill(val, data);
        if (__is_ct_const(count) && count <= 8)
        {
            CT_ASSERT(count != 0);
            if (sync == sig_done)
            {
                __asm cls[test_add, *val, address, 0, __ct_const_val(count)], sig_done[*sig_ptr]
            }
            else
            {
                __asm cls[test_add, *val, address, 0, __ct_const_val(count)], ctx_swap[*sig_ptr]
            }
        }
        else
        {
            if (!__is_ct_const(count))
            {
                CT_QPERFINFO_INDIRECT_REF(cls_test_add);
            }
            {
                _INTRINSIC_IND_ALU_COUNT_MIN_MAX(count, 1, 32);

                if (sync == sig_done)
                {
                    __asm cls[test_add, *val, address, 0, __ct_const_val(count)], sig_done[*sig_ptr], indirect_ref
                }
                else
                {
                    __asm cls[test_add, *val, address, 0, __ct_const_val(count)], ctx_swap[*sig_ptr], indirect_ref
                }
            }
        }
    }
    _INTRINSIC_END;
}


void cmd_cls_test_and_add_ptr40(
    __xread uint32_t *val,                              /* returned pre-modified value            */
    __xwrite uint32_t *data,                            /* data to add                            */
    volatile void __addr40 __cls *address,              /* address to add                         */
    uint32_t count,                                     /* number of words                        */
    sync_t sync,                                        /* must be sig_done                       */
    SIGNAL *sig_ptr                                     /* signal to raise upon completion        */
)
{
    _INTRINSIC_BEGIN;
    {
        _CLS_IMPLEMENT_TEST_COMMAND_PTR40(test_add, 32, data);
    }
    _INTRINSIC_END;
}

/* Local scratch memory test and add64  */

void cmd_cls_test_and_add64_ptr32(
    __xread uint32_t *val,                              /* returned pre-modified value            */
    __xwrite uint32_t *data,                            /* data to add                            */
    volatile void __cls *address,                       /* address to add                         */
    uint32_t count,                                     /* number of words                        */
    sync_t sync,                                        /* must be sig_done                       */
    SIGNAL *sig_ptr                                     /* signal to raise upon completion        */
)
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_read_reg(val));
        CT_ASSERT(__is_write_reg(data));
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done || sync == ctx_swap);

        __associate_read_write_reg_pair_no_spill(val, data);
        if (__is_ct_const(count) && count <= 8)
        {
            CT_ASSERT(count != 0);
            if (sync == sig_done)
            {
                __asm cls[test_add64, *val, address, 0, __ct_const_val(count)], sig_done[*sig_ptr]
            }
            else
            {
                __asm cls[test_add64, *val, address, 0, __ct_const_val(count)], ctx_swap[*sig_ptr]
            }
        }
        else
        {
            if (__is_ct_const(count))
            {
                CT_ASSERT((count & 1) == 0);
            }
            else
            {
                CT_QPERFINFO_INDIRECT_REF(cls_test_add64);
                RT_RANGE_ASSERT((count & 1) == 0);
            }
            {
                _INTRINSIC_IND_ALU_COUNT_MIN_MAX(count, 1, 16);

                if (sync == sig_done)
                {
                    __asm cls[test_add64, *val, address, 0, __ct_const_val(count)], sig_done[*sig_ptr], indirect_ref
                }
                else
                {
                    __asm cls[test_add64, *val, address, 0, __ct_const_val(count)], ctx_swap[*sig_ptr], indirect_ref
                }
            }
        }
    }
    _INTRINSIC_END;
}


/* Local scratch memory test and add64 for 40 bit address */

void cmd_cls_test_and_add64_ptr40(
    __xread uint32_t *val,                              /* returned pre-modified value            */
    __xwrite uint32_t *data,                            /* data to add                            */
    volatile void __addr40 __cls *address,              /* address to add                         */
    uint32_t count,                                     /* number of words                        */
    sync_t sync,                                        /* must be sig_done                       */
    SIGNAL *sig_ptr                                     /* signal to raise upon completion        */
)
{
    _INTRINSIC_BEGIN;
    {
        _CLS_IMPLEMENT_TEST_COMMAND_PTR40(test_add64, 16, data);
    }
    _INTRINSIC_END;
}


/* Local scratch memory test and add imm  */
__intrinsic
void cmd_cls_test_and_add_imm_ptr32(
    __xread uint32_t *val,
    volatile void __cls *address,
    uint32_t value,
    sync_t sync,
    SIGNAL *sig_ptr
)
{
    _INTRINSIC_BEGIN;
    {
        _CLS_IMPLEMENT_TEST_IMMEDIATE_PTR32(test_add_imm, *val) ;
    }
    _INTRINSIC_END;
}

void cmd_cls_test_and_add_imm_ptr40(
    __xread uint32_t *val,
    volatile void __addr40 __cls *address,
    uint32_t value,
    sync_t sync,
    SIGNAL *sig_ptr
)
{
    _INTRINSIC_BEGIN;
    {
        _CLS_IMPLEMENT_TEST_IMMEDIATE_PTR40(test_add_imm, *val) ;
    }
    _INTRINSIC_END;
}



/* Local scratch memory test and add64 imm  */
void cmd_cls_test_and_add64_imm_ptr32(
    __xread uint32_t *val,
    volatile void __cls *address,
    uint32_t value,
    sync_t sync,
    SIGNAL *sig_ptr
)
{
    _INTRINSIC_BEGIN;
    {
        _CLS_IMPLEMENT_TEST_IMMEDIATE_PTR32(test_add64_imm, *val) ;
    }
    _INTRINSIC_END;
}


__intrinsic
void cmd_cls_test_and_add64_imm_ptr40(
    __xread uint32_t *val,
    volatile void __addr40 __cls *address,
    uint32_t value,
    sync_t sync,
    SIGNAL *sig_ptr
)
{
    _INTRINSIC_BEGIN;
    {
        _CLS_IMPLEMENT_TEST_IMMEDIATE_PTR40(test_add64_imm, *val) ;
    }
    _INTRINSIC_END;
}


/* Local scratch memory test and sub  */

void cmd_cls_test_and_sub_ptr32(
    __xread uint32_t *val,                              /* returned pre-modified value            */
    __xwrite uint32_t *data,                            /* data to subtract                       */
    volatile void __cls *address,                       /* address to subtract from               */
    uint32_t count,                                     /* number of words                        */
    sync_t sync,                                        /* must be sig_done                       */
    SIGNAL *sig_ptr                                     /* signal to raise upon completion        */
)
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_read_reg(val));
        CT_ASSERT(__is_write_reg(data));
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done || sync == ctx_swap);

        __associate_read_write_reg_pair_no_spill(val, data);
        if (__is_ct_const(count) && count <= 8)
        {
            CT_ASSERT(count != 0);
            if (sync == sig_done)
            {
                __asm cls[test_sub, *val, address, 0, __ct_const_val(count)], sig_done[*sig_ptr]
            }
            else
            {
                __asm cls[test_sub, *val, address, 0, __ct_const_val(count)], ctx_swap[*sig_ptr]
            }
        }
        else
        {
            if (!__is_ct_const(count))
            {
                CT_QPERFINFO_INDIRECT_REF(cls_test_sub);
            }
            {
                _INTRINSIC_IND_ALU_COUNT_MIN_MAX(count, 1, 32);

                if (sync == sig_done)
                {
                    __asm cls[test_sub, *val, address, 0, __ct_const_val(count)], sig_done[*sig_ptr], indirect_ref
                }
                else
                {
                    __asm cls[test_sub, *val, address, 0, __ct_const_val(count)], ctx_swap[*sig_ptr], indirect_ref
                }
            }
        }
    }
    _INTRINSIC_END;
}

void cmd_cls_test_and_sub_ptr40(
    __xread uint32_t *val,
    __xwrite uint32_t *data,
    volatile void __addr40 __cls *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
)
{
    _INTRINSIC_BEGIN;
    {
        _CLS_IMPLEMENT_TEST_COMMAND_PTR40(test_sub, 32, data);
    }
    _INTRINSIC_END;
}


/* Local scratch memory test and sub64  */

void cmd_cls_test_and_sub64_ptr32(
    __xread uint32_t *val,                              /* returned pre-modified value            */
    __xwrite uint32_t *data,                            /* data to subtract                       */
    volatile void __cls *address,                       /* address to subtract from               */
    uint32_t count,                                     /* number of words                        */
    sync_t sync,                                        /* must be sig_done                       */
    SIGNAL *sig_ptr                                     /* signal to raise upon completion        */
)
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_read_reg(val));
        CT_ASSERT(__is_write_reg(data));
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done || sync == ctx_swap);

        __associate_read_write_reg_pair_no_spill(val, data);
        if (__is_ct_const(count) && count <= 8)
        {

            CT_ASSERT(count != 0);
            if (sync == sig_done)
            {
                __asm cls[test_sub64, *val, address, 0, __ct_const_val(count)], sig_done[*sig_ptr]
            }
            else
            {
                __asm cls[test_sub64, *val, address, 0, __ct_const_val(count)], ctx_swap[*sig_ptr]
            }
        }
        else
        {

            if (!__is_ct_const(count))
            {
                CT_QPERFINFO_INDIRECT_REF(cls_test_sub64);
            }
            {
                _INTRINSIC_IND_ALU_COUNT_MIN_MAX(count, 1, 16);

                if (sync == sig_done)
                {
                    __asm cls[test_sub64, *val, address, 0, __ct_const_val(count)], sig_done[*sig_ptr], indirect_ref
                }
                else
                {
                    __asm cls[test_sub64, *val, address, 0, __ct_const_val(count)], ctx_swap[*sig_ptr], indirect_ref
                }
            }
        }
    }
    _INTRINSIC_END;
}


void cmd_cls_test_and_sub64_ptr40(
    __xread uint32_t *val,
    __xwrite uint32_t *data,
    volatile void __addr40 __cls *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
)
{
    _INTRINSIC_BEGIN;
    {
        _CLS_IMPLEMENT_TEST_COMMAND_PTR40(test_sub64, 16, data);
    }
    _INTRINSIC_END;
}


/* Local scratch memory test and sub imm  */
__intrinsic
void cmd_cls_test_and_sub_imm_ptr32(
    __xread uint32_t *val,
    volatile void __cls *address,
    uint32_t value,
    sync_t sync,
    SIGNAL *sig_ptr
)
{
    _INTRINSIC_BEGIN;
    {
        _CLS_IMPLEMENT_TEST_IMMEDIATE_PTR32(test_sub_imm, *val) ;
    }
    _INTRINSIC_END;
}


void cmd_cls_test_and_sub_imm_ptr40(
    __xread uint32_t *val,
    volatile void __addr40 __cls *address,
    uint32_t value,
    sync_t sync,
    SIGNAL *sig_ptr
)
{
    _INTRINSIC_BEGIN;
    {
        _CLS_IMPLEMENT_TEST_IMMEDIATE_PTR40(test_sub_imm, *val) ;
    }
    _INTRINSIC_END;
}


/* Local scratch memory test and sub64 imm  */
__intrinsic
void cmd_cls_test_and_sub64_imm_ptr32(
    __xread uint32_t *val,
    volatile void __cls *address,
    uint32_t value,
    sync_t sync,
    SIGNAL *sig_ptr
)
{
    _INTRINSIC_BEGIN;
    {
        _CLS_IMPLEMENT_TEST_IMMEDIATE_PTR32(test_sub64_imm, *val) ;
    }
    _INTRINSIC_END;
}

void cmd_cls_test_and_sub64_imm_ptr40(
    __xread uint32_t *val,
    volatile void __addr40 __cls *address,
    uint32_t value,
    sync_t sync,
    SIGNAL *sig_ptr
)
{
    _INTRINSIC_BEGIN;
    {
        _CLS_IMPLEMENT_TEST_IMMEDIATE_PTR40(test_sub64_imm, *val) ;
    }
    _INTRINSIC_END;
}



/* Local scratch Queue Lock Claim         */
void cmd_cls_queue_lock_ptr32(
    volatile void __cls *address,                       /* address to read into                   */
    sync_t sync,                                        /* type of synchronization to use         */
    SIGNAL_PAIR *sig_pair_ptr                           /* signal pair to raise upon completion   */
)
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done);

        __asm cls[queue_lock, --, address, 0], sig_done[*__ct_sig_pair(sig_pair_ptr)]
    }
    _INTRINSIC_END;
}


void cmd_cls_queue_lock_ptr40(
    volatile void __addr40 __cls *address,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
)
{
    _INTRINSIC_BEGIN;
    {
        uint32_t hi_addr, low_addr;
        _INTRINSIC_CONVERT_HI_LO_ADDRESS(address);

        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done);

        __asm cls[queue_lock, --, hi_addr, << 8, low_addr], sig_done[*__ct_sig_pair(sig_pair_ptr)]
    }
    _INTRINSIC_END;
}

void cmd_cls_queue_unlock_ptr40(
    volatile void __addr40 __cls *address
)
{
    _INTRINSIC_BEGIN;
    {
        uint32_t hi_addr, low_addr;
        _INTRINSIC_CONVERT_HI_LO_ADDRESS(address);

        __asm cls[queue_unlock, --, hi_addr, << 8, low_addr]
    }
    _INTRINSIC_END;
}


/* Local scratch Queue UnLock Claim       */
void cmd_cls_queue_unlock_ptr32(
    volatile void __cls *address              /* address to read into                   */
)
{
    _INTRINSIC_BEGIN;
    {
        __asm cls[queue_unlock, --, address, 0]
    }
    _INTRINSIC_END;
}

#pragma diag_default 279  /* suppressed above */

#endif /* __NFP_CLS_C__ */
