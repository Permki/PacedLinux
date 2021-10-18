/*
 * Copyright (C) 2008-2014 Netronome Systems, Inc.  All rights reserved.
 */

#ifndef __NFP_CLS_RING_C__
#define __NFP_CLS_RING_C__

#include <nfp.h>
#include <nfp_intrinsic.h>
#include <nfp_cls.h>
#include <nfp_cls_ring.h>

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

#define _CLS_RING_NUMBER_CHECK(ring_number)                             \
        if (__is_ct_const(ring_number))                                 \
        {                                                               \
            CT_ASSERT(ring_number <= 15);                               \
        }                                                               \
        else                                                            \
        {                                                               \
            RT_RANGE_ASSERT(ring_number <= 15);                         \
        }

#define _CLS_RING_COUNT_CHECK_16(count)                                 \
        if (__is_ct_const(count))                                       \
        {                                                               \
            CT_ASSERT(count >= 1 && count <= 16);                       \
        }                                                               \
        else                                                            \
        {                                                               \
            RT_RANGE_ASSERT(count >= 1 && count <= 16);                 \
        }

#define _CLS_RING_COUNT_CHECK_32(count)                                 \
        if (__is_ct_const(count))                                       \
        {                                                               \
            CT_ASSERT(count >= 1 && count <= 32);                       \
        }                                                               \
        else                                                            \
        {                                                               \
            RT_RANGE_ASSERT(count >= 1 && count <= 32);                 \
        }

#define _CLS_RING_OFFSET_CHECK(offset)                                  \
        if (__is_ct_const(offset))                                      \
        {                                                               \
            CT_ASSERT(offset <= 0x3fff);                                \
        }                                                               \
        else                                                            \
        {                                                               \
            RT_RANGE_ASSERT(offset <= 0x3fff);                          \
        }

#define _CLS_RING_EVENTS_CHECK(report_events)                           \
        if (__is_ct_const(report_events))                               \
        {                                                               \
            CT_ASSERT(report_events <= 0x0f);                           \
        }                                                               \
        else                                                            \
        {                                                               \
            RT_RANGE_ASSERT(report_events <= 0x0f);                     \
        }

#define _CLS_RING_ISLAND_CHECK(island_number)                           \
        if (__is_ct_const(island_number))                               \
        {                                                               \
            CT_ASSERT(island_number <= 0x3f);                           \
        }                                                               \
        else                                                            \
        {                                                               \
            RT_ASSERT(island_number <= 0x3f);                           \
        }
/*
 * Verify that base address is aligned. Base address is shifted << 7 when written to
 * ring base CSR.
 */
#define _CLS_RING_BASE_ADDRESS_ALIGN_CHECK(addr)            \
        if (__is_ct_const(addr))                            \
        {                                                   \
            CT_ASSERT((addr & 0x7f) == 0);                  \
        }                                                   \
        else                                                \
        {                                                   \
            RT_ALIGN_ASSERT((addr & 0x7f) == 0);            \
        }

/*
 * set the address with ring number and offset (seq number)
 */
#define _CLS_RING_ADDRESS_WITH_OFFSET(address, ring, offset)                    \
    address =  (1 << 20) | ((ring & 0x0f) << 16) | ((offset & 0x3fff) << 2) ;

/*
 * set the address with ring number
 */
#define _CLS_RING_ADDRESS(address, ring)                    \
    address = ((ring & 0x0f) << 2);

/*
 * set the "hi_addr" using island id
 */
#define _CLS_RING_SET_LOCALITY(locality, island)            \
    locality = (island_id & 0x3f) << 26;

/*
 * set the address for ring ordered lock and ordered unlock. The top bits are set.
 */
#define _CLS_RING_ADDRESS_ORDERED_LOCK(address, ring, offset)                    \
    address = (0xfff << 20) | ((ring & 0x0f) << 16) | ((offset & 0x3fff) << 2) ;

/** CLSRingsMap Address map
 */
typedef enum
{
    RING_BASE_0     =   0x10000,

    RING_PTR_0     =   0x10080

} CLS_RING_MAP;


/*
* Implementation for ring commands where the address parameter is already filled in
*/
#define _CLS_RING_IMPLEMENT_ADDRESS_COMMAND(command, is_read, cls_command, max_count)                           \
{                                                                                                               \
    if (is_read)                                                                                                \
    {                                                                                                           \
        CT_ASSERT(__is_read_reg(data));                                                                         \
    }                                                                                                           \
    else                                                                                                        \
    {                                                                                                           \
        CT_ASSERT(__is_write_reg(data));                                                                        \
    }                                                                                                           \
    CT_ASSERT(__is_ct_const(sync));                                                                             \
    CT_ASSERT(sync == sig_done || sync == ctx_swap);                                                            \
    if (__is_ct_const(count) && count <= 8)                                                                     \
    {                                                                                                           \
        CT_ASSERT(count != 0);                                                                                  \
        if (sync == sig_done)                                                                                   \
        {                                                                                                       \
            __asm cls[command, *data, address, 0, __ct_const_val(count)], sig_done[*sig_ptr]                    \
        }                                                                                                       \
        else                                                                                                    \
        {                                                                                                       \
            __asm cls[command, *data, address, 0, __ct_const_val(count)], ctx_swap[*sig_ptr]                    \
        }                                                                                                       \
    }                                                                                                           \
    else                                                                                                        \
    {                                                                                                           \
        if (!__is_ct_const(count))                                                                              \
        {                                                                                                       \
            CT_QPERFINFO_INDIRECT_REF(cls_command);                                                             \
        }                                                                                                       \
        {                                                                                                       \
            _INTRINSIC_IND_ALU_COUNT_MIN_MAX(count, 1, max_count);                                              \
            if (sync == sig_done)                                                                               \
            {                                                                                                   \
                __asm cls[command, *data, address, 0, __ct_const_val(count)], sig_done[*sig_ptr], indirect_ref  \
            }                                                                                                   \
            else                                                                                                \
            {                                                                                                   \
                __asm cls[command, *data, address, 0, __ct_const_val(count)], ctx_swap[*sig_ptr], indirect_ref  \
            }                                                                                                   \
        }                                                                                                       \
    }                                                                                                           \
}



/*
* Implementation for ring commands where the address parameter is already filled in
*/
#define _CLS_RING_IMPLEMENT_ADDRESS_COMMAND_PTR40(command, is_read, cls_command, max_count)                     \
{                                                                                                               \
    if (is_read)                                                                                                \
    {                                                                                                           \
        CT_ASSERT(__is_read_reg(data));                                                                         \
    }                                                                                                           \
    else                                                                                                        \
    {                                                                                                           \
        CT_ASSERT(__is_write_reg(data));                                                                        \
    }                                                                                                           \
    CT_ASSERT(__is_ct_const(sync));                                                                             \
    CT_ASSERT(sync == sig_done || sync == ctx_swap);                                                            \
    if (__is_ct_const(count) && count <= 8)                                                                     \
    {                                                                                                           \
        CT_ASSERT(count != 0);                                                                                  \
        if (sync == sig_done)                                                                                   \
        {                                                                                                       \
            __asm cls[command, *data, hi_addr, <<8, low_addr, __ct_const_val(count)], sig_done[*sig_ptr]        \
        }                                                                                                       \
        else                                                                                                    \
        {                                                                                                       \
            __asm cls[command, *data, hi_addr, <<8, low_addr, __ct_const_val(count)], ctx_swap[*sig_ptr]        \
        }                                                                                                       \
    }                                                                                                           \
    else                                                                                                        \
    {                                                                                                           \
        if (!__is_ct_const(count))                                                                              \
        {                                                                                                       \
            CT_QPERFINFO_INDIRECT_REF(cls_command);                                                             \
        }                                                                                                       \
        {                                                                                                       \
            _INTRINSIC_IND_ALU_COUNT_MIN_MAX(count, 1, max_count);                                              \
            if (sync == sig_done)                                                                               \
            {                                                                                                   \
                __asm cls[command, *data, hi_addr, <<8, low_addr, __ct_const_val(count)], sig_done[*sig_ptr], indirect_ref  \
            }                                                                                                   \
            else                                                                                                \
            {                                                                                                   \
                __asm cls[command, *data, hi_addr, <<8, low_addr, __ct_const_val(count)], ctx_swap[*sig_ptr], indirect_ref  \
            }                                                                                                   \
        }                                                                                                       \
    }                                                                                                           \
}


__intrinsic
int32_t cmd_cls_state_test(
    cls_state_t state_name              /* state to test */
)
{
    int32_t result = 1;
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_ct_const(state_name));
        __asm
        {
            br_cls_state[__ct_const_val(state_name), L1];
            alu[result, --, B, 0]
            L1:
        }
    }
    _INTRINSIC_END;
    return result;
}


/* Local scratch memory put ring          */
void cmd_cls_ring_put_ptr32(
    __xwrite void *data,                            /* data to put                            */
    volatile void __addr32 __cls *address,          /* ring address                           */
    uint32_t count,                                 /* number of longwords to put             */
    sync_t sync,                                    /* type of synchronization to use         */
    SIGNAL *sig_ptr                                 /* signal to raise upon completion        */
)
{
    _INTRINSIC_BEGIN;
    {
        _CLS_RING_IMPLEMENT_ADDRESS_COMMAND(ring_put, 0, cmd_cls_ring_put_ptr32, 16);
    }
    _INTRINSIC_END;
}


/* Local scratch memory put ring          */
void cmd_cls_ring_put_ptr40(
    __xwrite void *data,                                /* data to put                            */
    volatile void __addr40 __cls *address,              /* ring address                           */
    uint32_t count,                                     /* number of longwords to put             */
    sync_t sync,                                        /* type of synchronization to use         */
    SIGNAL *sig_ptr                                     /* signal to raise upon completion        */
)
{
    _INTRINSIC_BEGIN;
    {
        __gpr uint32_t hi_addr;
        __gpr uint32_t low_addr;
        _INTRINSIC_CONVERT_HI_LO_ADDRESS(address);

        _CLS_RING_IMPLEMENT_ADDRESS_COMMAND_PTR40(ring_put, 0, cmd_cls_ring_put_ptr32, 16);
    }
    _INTRINSIC_END;
}

/* Local scratch memory ring journal      */
void cmd_cls_ring_journal_ptr32(
    __xwrite void *data,                                /* data to put                            */
    volatile void __addr32 __cls *address,              /* ring address                           */
    uint32_t count,                                     /* number of longwords to put             */
    sync_t sync,                                        /* type of synchronization to use         */
    SIGNAL *sig_ptr                                     /* signal to raise upon completion        */
)
{
    _INTRINSIC_BEGIN;
    {
        _CLS_RING_IMPLEMENT_ADDRESS_COMMAND(ring_journal, 0, cmd_cls_ring_journal_ptr32, 16);
    }
    _INTRINSIC_END;
}


/* Local scratch memory ring journal      */
void cmd_cls_ring_journal_ptr40(
    __xwrite void *data,                                /* data to put                            */
    volatile void __addr40 __cls *address,              /* ring address                           */
    uint32_t count,                                     /* number of longwords to put             */
    sync_t sync,                                        /* type of synchronization to use         */
    SIGNAL *sig_ptr                                     /* signal to raise upon completion        */
)
{
    _INTRINSIC_BEGIN;
    {
        __gpr uint32_t hi_addr;
        __gpr uint32_t low_addr;
        _INTRINSIC_CONVERT_HI_LO_ADDRESS(address);

        _CLS_RING_IMPLEMENT_ADDRESS_COMMAND_PTR40(ring_journal, 0, cmd_cls_ring_journal_ptr32, 16);
    }
    _INTRINSIC_END;
}


/* Local scratch memory get ring          */
void cmd_cls_ring_get_ptr32(
    __xread void *data,                                 /* data to get                            */
    volatile void __addr32 __cls *address,              /* ring address                           */
    uint32_t count,                                     /* number of longwords to get             */
    sync_t sync,                                        /* type of synchronization to use         */
    SIGNAL *sig_ptr                                     /* signal to raise upon completion        */
)
{
    _INTRINSIC_BEGIN;
    {
        _CLS_RING_IMPLEMENT_ADDRESS_COMMAND(ring_get, 1, cmd_cls_ring_get_ptr32, 16);
    }
    _INTRINSIC_END;
}



void cmd_cls_ring_get_ptr40(
    __xread void *data,                                 /* data to get                            */
    volatile void __addr40 __cls *address,              /* ring address                           */
    uint32_t count,                                     /* number of longwords to get             */
    sync_t sync,                                        /* type of synchronization to use         */
    SIGNAL *sig_ptr                                     /* signal to raise upon completion        */
)
{
    _INTRINSIC_BEGIN;
    {
        __gpr uint32_t hi_addr;
        __gpr uint32_t low_addr;
        _INTRINSIC_CONVERT_HI_LO_ADDRESS(address);

        _CLS_RING_IMPLEMENT_ADDRESS_COMMAND_PTR40(ring_get, 1, cmd_cls_ring_get_ptr32, 16);
    }
    _INTRINSIC_END;
}

/* Local scratch memory pop ring          */

void cmd_cls_ring_pop_ptr32(
    __xread void *data,                                 /* data to get                            */
    volatile void __addr32 __cls *address,              /* ring address                           */
    uint32_t count,                                     /* number of longwords to get             */
    sync_t sync,                                        /* type of synchronization to use         */
    SIGNAL *sig_ptr                                     /* signal to raise upon completion        */
)
{
    _INTRINSIC_BEGIN;
    {
        _CLS_RING_IMPLEMENT_ADDRESS_COMMAND(ring_pop, 1, cmd_cls_ring_pop_ptr32, 16);
    }
    _INTRINSIC_END;
}


/* Local scratch memory pop ring          */

void cmd_cls_ring_pop_ptr40(
    __xread void *data,                                 /* data to get                            */
    volatile void __addr40 __cls *address,              /* ring address                           */
    uint32_t count,                                     /* number of longwords to get             */
    sync_t sync,                                        /* type of synchronization to use         */
    SIGNAL *sig_ptr                                     /* signal to raise upon completion        */
)
{
    _INTRINSIC_BEGIN;
    {
        __gpr uint32_t hi_addr;
        __gpr uint32_t low_addr;
        _INTRINSIC_CONVERT_HI_LO_ADDRESS(address);

        _CLS_RING_IMPLEMENT_ADDRESS_COMMAND_PTR40(ring_pop, 1, cmd_cls_ring_pop_ptr32, 16);
    }
    _INTRINSIC_END;
}

/* Local scratch memory get ring          */

void cmd_cls_ring_get_safe_ptr32(
    __xread void *data,                                 /* data to get                            */
    volatile void __addr32 __cls *address,              /* ring address                           */
    uint32_t count,                                     /* number of longwords to get             */
    sync_t sync,                                        /* type of synchronization to use         */
    SIGNAL *sig_ptr                                     /* signal to raise upon completion        */
)
{
    _INTRINSIC_BEGIN;
    {
        _CLS_RING_IMPLEMENT_ADDRESS_COMMAND(ring_get_freely, 1, cmd_cls_ring_get_safe_ptr32, 16);
    }
    _INTRINSIC_END;
}


void cmd_cls_ring_get_safe_ptr40(
    __xread void *data,                                 /* data to get                            */
    volatile void __addr40 __cls *address,              /* ring address                           */
    uint32_t count,                                     /* number of longwords to get             */
    sync_t sync,                                        /* type of synchronization to use         */
    SIGNAL *sig_ptr                                     /* signal to raise upon completion        */
)
{
    _INTRINSIC_BEGIN;
    {
        __gpr uint32_t hi_addr;
        __gpr uint32_t low_addr;
        _INTRINSIC_CONVERT_HI_LO_ADDRESS(address);

        _CLS_RING_IMPLEMENT_ADDRESS_COMMAND_PTR40(ring_get_freely, 1, cmd_cls_ring_get_safe_ptr32, 16);
    }
    _INTRINSIC_END;
}

/* Local scratch memory ring pop freely   */
void cmd_cls_ring_pop_safe_ptr32(
    __xread void *data,                                 /* data to get                            */
    volatile void __addr32 __cls *address,              /* ring address                           */
    uint32_t count,                                     /* number of longwords to get             */
    sync_t sync,                                        /* type of synchronization to use         */
    SIGNAL *sig_ptr                                     /* signal to raise upon completion        */
)
{
    _INTRINSIC_BEGIN;
    {
        _CLS_RING_IMPLEMENT_ADDRESS_COMMAND(ring_pop_freely, 1, cmd_cls_ring_pop_safe_ptr32, 16);
    }
    _INTRINSIC_END;
}


/* Local scratch memory ring pop freely   */
void cmd_cls_ring_pop_safe_ptr40(
    __xread void *data,                                 /* data to get                            */
    volatile void __addr40 __cls *address,              /* ring address                           */
    uint32_t count,                                     /* number of longwords to get             */
    sync_t sync,                                        /* type of synchronization to use         */
    SIGNAL *sig_ptr                                     /* signal to raise upon completion        */
)
{
    _INTRINSIC_BEGIN;
    {
        __gpr uint32_t hi_addr;
        __gpr uint32_t low_addr;
        _INTRINSIC_CONVERT_HI_LO_ADDRESS(address);

        _CLS_RING_IMPLEMENT_ADDRESS_COMMAND_PTR40(ring_pop_freely, 1, cmd_cls_ring_pop_safe_ptr32, 16);
    }
    _INTRINSIC_END;
}


/* Local scratch memory ring init  */
__intrinsic
void cmd_cls_ring_init_ptr32(
    uint32_t ring_number,
    volatile void __addr32 __cls *base_address,
    enum CLS_RING_SIZE size,
    uint32_t report_events,
    sync_t sync,
    SIGNAL *sig_ptr
)
{
    _INTRINSIC_BEGIN;

    CT_ASSERT(__is_ct_const(sync));
    CT_ASSERT(sync == sig_done || sync == ctx_swap);
    _CLS_RING_NUMBER_CHECK(ring_number);
    _CLS_RING_EVENTS_CHECK(report_events);
    _CLS_RING_BASE_ADDRESS_ALIGN_CHECK((uint32_t)base_address);

    {
        __addr32 __cls uint32_t            *ring_ptr_address = (__addr32 __cls uint32_t *)(RING_PTR_0 + (ring_number * 8));
        __addr32 __cls uint32_t            *ring_base_address = (__addr32 __cls uint32_t *)(RING_BASE_0 + (ring_number * 8));
        volatile __xwrite uint32_t      xfer;

        /* initialise ring base */
        {
            xfer = ((report_events & 0xf) << 24) | ( ((uint32_t)size & 0x07) << 16) | ((uint32_t)(base_address) >> 7) & 0x1ff;

            if (sync == sig_done)
            {
                cmd_cls_write_be_ptr32((void*)&xfer, ring_base_address, 1, sig_done, sig_ptr);
                __wait_for_all(sig_ptr);
            }
            else
            {
                cmd_cls_write_be_ptr32((void*)&xfer, ring_base_address, 1, ctx_swap, sig_ptr);
            }
        }

        /* clear ring pointers */
        {
            xfer = 0x00;
            if (sync == sig_done)
            {
                cmd_cls_write_be_ptr32((void*)&xfer, ring_ptr_address, 1, sig_done, sig_ptr);
            }
            else
            {
                cmd_cls_write_be_ptr32((void*)&xfer, ring_ptr_address, 1, ctx_swap, sig_ptr);
            }
        }
    }

_INTRINSIC_END;
}


/* Local scratch memory ring init  */
__intrinsic
void cmd_cls_ring_init_ptr40(
    uint32_t ring_number,
    volatile void __addr40 __cls *base_address,
    enum CLS_RING_SIZE size,
    uint32_t report_events,
    sync_t sync,
    SIGNAL *sig_ptr
)
{
    _INTRINSIC_BEGIN;

    CT_ASSERT(__is_ct_const(sync));
    CT_ASSERT(sync == sig_done || sync == ctx_swap);
    _CLS_RING_NUMBER_CHECK(ring_number);
    _CLS_RING_EVENTS_CHECK(report_events);
    _CLS_RING_BASE_ADDRESS_ALIGN_CHECK((uint32_t)base_address);

    {
        uint64_t                              island = ((uint64_t)base_address) & 0xFC00000000; // top 6 bits
        __addr40 __cls uint64_t      *ring_ptr_address = (__addr40 __cls uint64_t *)(island | RING_PTR_0 + (ring_number * 8));
        __addr40 __cls uint64_t      *ring_base_address = (__addr40 __cls uint64_t *)(island | RING_BASE_0 + (ring_number * 8));
        volatile __xwrite uint32_t     xfer;

        /* initialise ring base */
        {
            xfer = ((report_events & 0xf) << 24) | ( ((uint32_t)size & 0x07) << 16) | ((uint32_t)(base_address) >> 7) & 0x1ff;

            if (sync == sig_done)
            {
                cmd_cls_write_be_ptr40((void*)&xfer, ring_base_address, 1, sig_done, sig_ptr);
                __wait_for_all(sig_ptr);
            }
            else
            {
                cmd_cls_write_be_ptr40((void*)&xfer, ring_base_address, 1, ctx_swap, sig_ptr);
            }
        }

        /* clear ring pointers */
        {
            xfer = 0x00;
            if (sync == sig_done)
            {
                cmd_cls_write_be_ptr40((void*)&xfer, ring_ptr_address, 1, sig_done, sig_ptr);
            }
            else
            {
                cmd_cls_write_be_ptr40((void*)&xfer, ring_ptr_address, 1, ctx_swap, sig_ptr);
            }
        }
    }

_INTRINSIC_END;
}


/* Local scratch memory ring write  */
void cmd_cls_ring_write_ptr32(
    __xwrite void *data,                                /* data to write                          */
    uint32_t ring_number,                               /* ring number                            */
    uint32_t count,                                     /* number of longwords to write           */
    uint32_t offset,                                    /* offset to write at                     */
    sync_t sync,                                        /* type of synchronization to use         */
    SIGNAL *sig_ptr                                     /* signal to raise upon completion        */
)
{
    _INTRINSIC_BEGIN;
    {
        __gpr uint32_t address;
        _CLS_RING_ADDRESS_WITH_OFFSET(address, ring_number, offset);
        _CLS_RING_NUMBER_CHECK(ring_number);
        _CLS_RING_COUNT_CHECK_32(count);
        _CLS_RING_OFFSET_CHECK(offset);

        _CLS_RING_IMPLEMENT_ADDRESS_COMMAND(ring_write, 0, cls_ring_write, 32);
    }
_INTRINSIC_END;
}


/* Local scratch memory ring write  */
void cmd_cls_ring_write_ptr40(
    __xwrite void *data,                            /* data to write                          */
    uint32_t ring_number,                           /* ring number                            */
    uint32_t count,                                 /* number of longwords to write           */
    uint32_t offset,                                /* offset to write at                     */
    uint32_t island_id,                             /* CLS island where ring is located       */
    sync_t sync,                                    /* type of synchronization to use         */
    SIGNAL *sig_ptr                                 /* signal to raise upon completion        */
)
{
    _INTRINSIC_BEGIN;
    {
        __gpr uint32_t hi_addr;
        __gpr uint32_t low_addr;
        _CLS_RING_SET_LOCALITY(hi_addr, island_id);
        _CLS_RING_ADDRESS_WITH_OFFSET(low_addr, ring_number, offset);
        _CLS_RING_NUMBER_CHECK(ring_number);
        _CLS_RING_COUNT_CHECK_32(count);
        _CLS_RING_OFFSET_CHECK(offset);
        _CLS_RING_IMPLEMENT_ADDRESS_COMMAND_PTR40(ring_write, 0, cls_ring_write, 32);
    }
_INTRINSIC_END;
}


/* Local scratch memory ring read  */
void cmd_cls_ring_read_ptr32(
    __xread void *data,                                 /* data to read                          */
    uint32_t ring_number,                               /* ring number (0 to 15)                 */
    uint32_t count,                                     /* number of longwords to read           */
    uint32_t offset,                                    /* offset to write at                     */
    sync_t sync,                                        /* type of synchronization to use         */
    SIGNAL *sig_ptr                                     /* signal to raise upon completion        */
)
{
    _INTRINSIC_BEGIN;
    {
        __gpr uint32_t address;
        _CLS_RING_ADDRESS_WITH_OFFSET(address, ring_number, offset);
        _CLS_RING_NUMBER_CHECK(ring_number);
        _CLS_RING_COUNT_CHECK_16(count);
        _CLS_RING_OFFSET_CHECK(offset);
        _CLS_RING_IMPLEMENT_ADDRESS_COMMAND(ring_read, 1, cls_ring_read, 16);
    }
_INTRINSIC_END;
}


/* Local scratch memory ring read  */
void cmd_cls_ring_read_ptr40(
    __xread void *data,                                 /* data to read                          */
    uint32_t ring_number,                               /* ring number (0 to 15)                 */
    uint32_t count,                                     /* number of longwords to read           */
    uint32_t offset,                                    /* offset to write at                     */
    uint32_t island_id,                                 /* CLS island where ring is located       */
    sync_t sync,                                        /* type of synchronization to use         */
    SIGNAL *sig_ptr                                     /* signal to raise upon completion        */
)
{
    _INTRINSIC_BEGIN;
    {
        __gpr uint32_t hi_addr;
        __gpr uint32_t low_addr;
        _CLS_RING_SET_LOCALITY(hi_addr, island_id);
        _CLS_RING_ADDRESS_WITH_OFFSET(low_addr, ring_number, offset);
        _CLS_RING_NUMBER_CHECK(ring_number);
        _CLS_RING_COUNT_CHECK_16(count);
        _CLS_RING_OFFSET_CHECK(offset);
        _CLS_RING_IMPLEMENT_ADDRESS_COMMAND_PTR40(ring_read, 1, cls_ring_read, 16);
    }
_INTRINSIC_END;
}




__intrinsic
void cmd_cls_ring_ordered_lock_ptr32(
    uint32_t ring_number,
    uint32_t sequence_number,
    sync_t sync,
    SIGNAL *sig_ptr
)
{
    _INTRINSIC_BEGIN;
    {
        __gpr uint32_t ring_address;

        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done || sync == ctx_swap);

        _CLS_RING_NUMBER_CHECK(ring_number);
        _CLS_RING_OFFSET_CHECK(sequence_number);
        _CLS_RING_ADDRESS_ORDERED_LOCK(ring_address, ring_number, sequence_number);
        if (sync == sig_done)
        {
            __asm
            {
                cls[ring_ordered_lock, --, ring_address, 0], sig_done[*sig_ptr]
            }
        }
        else
        {
            __asm
            {
                cls[ring_ordered_lock, --, ring_address, 0], ctx_swap[*sig_ptr]
            }
        }
    }
_INTRINSIC_END;
}



__intrinsic
void cmd_cls_ring_ordered_lock_ptr40(
    uint32_t ring_number,
    uint32_t sequence_number,
    uint32_t island_id,
    sync_t sync,
    SIGNAL *sig_ptr
)
{
    _INTRINSIC_BEGIN;
    {
        __gpr uint32_t ring_address;
        __gpr uint32_t hi_addr;

        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done || sync == ctx_swap);

        _CLS_RING_NUMBER_CHECK(ring_number);
        _CLS_RING_OFFSET_CHECK(sequence_number);
        _CLS_RING_ISLAND_CHECK(island_id);
        _CLS_RING_SET_LOCALITY(hi_addr, island_id);
        _CLS_RING_ADDRESS_ORDERED_LOCK(ring_address, ring_number, sequence_number);
        if (sync == sig_done)
        {
            __asm
            {
                cls[ring_ordered_lock, --, hi_addr, << 8, ring_address], sig_done[*sig_ptr]
            }
        }
        else
        {
            __asm
            {
                cls[ring_ordered_lock, --, hi_addr, << 8, ring_address], ctx_swap[*sig_ptr]
            }
        }
    }
_INTRINSIC_END;
}


__intrinsic
void cmd_cls_ring_ordered_unlock_ptr32(
    uint32_t ring_number,
    uint32_t sequence_number
)
{
    _INTRINSIC_BEGIN;
    {
        __gpr uint32_t ring_address;

        _CLS_RING_NUMBER_CHECK(ring_number);
        _CLS_RING_OFFSET_CHECK(sequence_number);
        _CLS_RING_ADDRESS_ORDERED_LOCK(ring_address, ring_number, sequence_number);
        __asm
        {
            cls[ring_ordered_unlock, --, ring_address, 0]
        }
    }

_INTRINSIC_END;
}

__intrinsic
void cmd_cls_ring_ordered_unlock_ptr40(
    uint32_t ring_number,
    uint32_t sequence_number,
    uint32_t island_id
)
{
    _INTRINSIC_BEGIN;
    {
        __gpr uint32_t ring_address;
        __gpr uint32_t hi_addr;

        _CLS_RING_NUMBER_CHECK(ring_number);
        _CLS_RING_OFFSET_CHECK(sequence_number);
        _CLS_RING_ADDRESS_ORDERED_LOCK(ring_address, ring_number, sequence_number);
        _CLS_RING_SET_LOCALITY(hi_addr, island_id);
        __asm
        {
            cls[ring_ordered_unlock, --, hi_addr, << 8, ring_address]
        }
    }

_INTRINSIC_END;
}

/* Local scratch memory ring workq add thread  */
void cmd_cls_ring_workq_add_thread_ptr32(
    __xread void *data,                                 /* work returned                         */
    uint32_t ring_number,                               /* ring number (0 to 15)                 */
    uint32_t count,                                     /* number of longwords to return         */
    sync_t sync,                                        /* type of synchronization to use         */
    SIGNAL *sig_ptr                                     /* signal to raise upon completion        */
)
{
    _INTRINSIC_BEGIN;
    {
        __gpr uint32_t address;
        _CLS_RING_ADDRESS(address, ring_number);
        _CLS_RING_NUMBER_CHECK(ring_number);
        _CLS_RING_COUNT_CHECK_16(count);
        _CLS_RING_IMPLEMENT_ADDRESS_COMMAND(ring_workq_add_thread, 1, cls_ring_workq_add_thread, 16);
    }


_INTRINSIC_END;
}

/* Local scratch memory ring workq add thread  */
void cmd_cls_ring_workq_add_thread_ptr40(
    __xread void *data,                                 /* work returned                         */
    uint32_t ring_number,                               /* ring number (0 to 15)                 */
    uint32_t count,                                     /* number of longwords to return         */
    uint32_t island_id,
    sync_t sync,                                        /* type of synchronization to use         */
    SIGNAL *sig_ptr                                     /* signal to raise upon completion        */
)
{
    _INTRINSIC_BEGIN;
    {
        __gpr uint32_t low_addr;
        __gpr uint32_t hi_addr;

        _CLS_RING_ADDRESS(low_addr, ring_number);
        _CLS_RING_NUMBER_CHECK(ring_number);
        _CLS_RING_COUNT_CHECK_16(count);
        _CLS_RING_SET_LOCALITY(hi_addr, island_id);
        _CLS_RING_IMPLEMENT_ADDRESS_COMMAND_PTR40(ring_workq_add_thread, 1, cls_ring_workq_add_thread, 16);
    }


_INTRINSIC_END;
}


/* Local scratch memory ring workq add work  */
void cmd_cls_ring_workq_add_work_ptr32(
    __xwrite void *data,                                /* work to add to ring                   */
    uint32_t ring_number,                               /* ring number (0 to 15)                 */
    uint32_t count,                                     /* number of longwords to return         */
    sync_t sync,                                        /* type of synchronization to use        */
    SIGNAL *sig_ptr                                     /* signal to raise upon completion       */
)
{
    _INTRINSIC_BEGIN;
    {
        __gpr uint32_t address;
        _CLS_RING_ADDRESS(address, ring_number);
        _CLS_RING_NUMBER_CHECK(ring_number);
        _CLS_RING_COUNT_CHECK_16(count);

        _CLS_RING_IMPLEMENT_ADDRESS_COMMAND(ring_workq_add_work, 0, cls_ring_workq_add_work, 16);
    }

_INTRINSIC_END;
}


/* Local scratch memory ring workq add work  */
void cmd_cls_ring_workq_add_work_ptr40(
    __xwrite void *data,                                /* work to add to ring                   */
    uint32_t ring_number,                               /* ring number (0 to 15)                 */
    uint32_t count,                                     /* number of longwords to return         */
    uint32_t island_id,
    sync_t sync,                                        /* type of synchronization to use        */
    SIGNAL *sig_ptr                                     /* signal to raise upon completion       */
)
{
    _INTRINSIC_BEGIN;
    {
        __gpr uint32_t low_addr;
        __gpr uint32_t hi_addr;

        _CLS_RING_ADDRESS(low_addr, ring_number);
        _CLS_RING_NUMBER_CHECK(ring_number);
        _CLS_RING_COUNT_CHECK_16(count);
        _CLS_RING_SET_LOCALITY(hi_addr, island_id);
        _CLS_RING_IMPLEMENT_ADDRESS_COMMAND_PTR40(ring_workq_add_work, 0, cls_ring_workq_add_work, 16);
    }

_INTRINSIC_END;
}


__intrinsic
void cmd_cls_ring_put_ind_ptr40(
    __xwrite void *data,                                /* data to put                            */
    volatile void __addr40 __cls *address,              /* ring tail address                      */
    uint32_t max_nn,                                    /* max. number of longwords to put        */
    generic_ind_t ind,                                  /* indirect word                          */
    sync_t sync,                                        /* type of synchronization to use         */
    SIGNAL *sig_ptr                                     /* signal to raise upon completion        */
    )
{
    _INTRINSIC_BEGIN;
    {
         uint32_t hi_addr, low_addr;

       CT_ASSERT(__is_write_reg(data));
        CT_ASSERT(__is_ct_const(max_nn));
        CT_ASSERT(max_nn >= 1 && max_nn <= 16);
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done || sync == ctx_swap);
        _INTRINSIC_CONVERT_HI_LO_ADDRESS(address);
        __asm local_csr_wr[CMD_INDIRECT_REF_0, CSR_INDIRECT_TYPE(ind)]
        __asm  alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
        if (sync == sig_done)
        {
            __asm cls[ring_put, *data, hi_addr, <<8, low_addr, __ct_const_val(max_nn)], sig_done[*sig_ptr], indirect_ref
        }
        else
        {
            __asm cls[ring_put, *data, hi_addr, <<8, low_addr, __ct_const_val(max_nn)], ctx_swap[*sig_ptr], indirect_ref
        }
    }
    _INTRINSIC_END;
}


__intrinsic
void cmd_cls_ring_put_ind_ptr32(
    __xwrite void *data,                                /* data to put                            */
    volatile void __cls *address,                       /* ring tail address                      */
    uint32_t max_nn,                                    /* max. number of longwords to put        */
    generic_ind_t ind,                                  /* indirect word                          */
    sync_t sync,                                        /* type of synchronization to use         */
    SIGNAL *sig_ptr                                     /* signal to raise upon completion        */
    )
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_write_reg(data));
        CT_ASSERT(__is_ct_const(max_nn));
        CT_ASSERT(max_nn >= 1 && max_nn <= 16);
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done || sync == ctx_swap);

        __asm local_csr_wr[CMD_INDIRECT_REF_0, CSR_INDIRECT_TYPE(ind)]

        __asm  alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
        if (sync == sig_done)
        {
            __asm cls[ring_put, *data, address, 0, __ct_const_val(max_nn)], sig_done[*sig_ptr], indirect_ref
        }
        else
        {
            __asm cls[ring_put, *data, address, 0, __ct_const_val(max_nn)], ctx_swap[*sig_ptr], indirect_ref
        }
    }
    _INTRINSIC_END;
}


__intrinsic
void cmd_cls_ring_journal_ind_ptr40(
    __xwrite void *data,                                /* data to put                            */
    volatile void __addr40 __cls *address,              /* ring tail address                      */
    uint32_t max_nn,                                    /* max. number of longwords to put        */
    generic_ind_t ind,                                  /* indirect word                          */
    sync_t sync,                                        /* type of synchronization to use         */
    SIGNAL *sig_ptr                                     /* signal to raise upon completion        */
    )
{
    _INTRINSIC_BEGIN;
    {
         uint32_t hi_addr, low_addr;

        CT_ASSERT(__is_write_reg(data));
        CT_ASSERT(__is_ct_const(max_nn));
        CT_ASSERT(max_nn >= 1 && max_nn <= 16);
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done || sync == ctx_swap);
        _INTRINSIC_CONVERT_HI_LO_ADDRESS(address);

        __asm local_csr_wr[CMD_INDIRECT_REF_0, CSR_INDIRECT_TYPE(ind)]
        __asm  alu[--, --, B, ALU_INDIRECT_TYPE(ind)]

        if (sync == sig_done)
        {
            __asm cls[ring_journal, *data, hi_addr, <<8, low_addr, __ct_const_val(max_nn)], sig_done[*sig_ptr], indirect_ref
        }
        else
        {
            __asm cls[ring_journal, *data, hi_addr, <<8, low_addr, __ct_const_val(max_nn)], ctx_swap[*sig_ptr], indirect_ref
        }
    }
    _INTRINSIC_END;
}


__intrinsic
void cmd_cls_ring_journal_ind_ptr32(
    __xwrite void *data,                                /* data to put                            */
    volatile void __cls *address,                       /* ring tail address                      */
    uint32_t max_nn,                                    /* max. number of longwords to put        */
    generic_ind_t ind,                                  /* indirect word                          */
    sync_t sync,                                        /* type of synchronization to use         */
    SIGNAL *sig_ptr                                     /* signal to raise upon completion        */
    )
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_write_reg(data));
        CT_ASSERT(__is_ct_const(max_nn));
        CT_ASSERT(max_nn >= 1 && max_nn <= 16);
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done || sync == ctx_swap);

        __asm local_csr_wr[CMD_INDIRECT_REF_0, CSR_INDIRECT_TYPE(ind)]
        __asm  alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
        if (sync == sig_done)
        {
            __asm cls[ring_journal, *data, address, 0, __ct_const_val(max_nn)], sig_done[*sig_ptr], indirect_ref
        }
        else
        {
            __asm cls[ring_journal, *data, address, 0, __ct_const_val(max_nn)], ctx_swap[*sig_ptr], indirect_ref
        }
    }
    _INTRINSIC_END;
}


__intrinsic
void cmd_cls_ring_get_ind_ptr40(
    __xread void *data,                                 /* data to get                            */
    volatile void __addr40 __cls *address,              /* head of ring address                   */
    uint32_t max_nn,                                    /* max. number of longwords to get        */
    generic_ind_t ind,                                  /* indirect word                          */
    sync_t sync,                                        /* type of synchronization to use         */
    SIGNAL *sig_ptr                                     /* signal to raise upon completion        */
    )
{
    _INTRINSIC_BEGIN;
    {
         uint32_t hi_addr, low_addr;

        CT_ASSERT(__is_read_reg(data));
        CT_ASSERT(__is_ct_const(max_nn));
        CT_ASSERT(max_nn >= 1 && max_nn <= 16);
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done || sync == ctx_swap);
        _INTRINSIC_CONVERT_HI_LO_ADDRESS(address);

        __asm local_csr_wr[CMD_INDIRECT_REF_0, CSR_INDIRECT_TYPE(ind)]
        __asm  alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
        if (sync == sig_done)
        {
            __asm cls[ring_get, *data, hi_addr, <<8, low_addr, __ct_const_val(max_nn)], sig_done[*sig_ptr], indirect_ref
        }
        else
        {
            __asm cls[ring_get, *data, hi_addr, <<8, low_addr, __ct_const_val(max_nn)], ctx_swap[*sig_ptr], indirect_ref
        }
    }
    _INTRINSIC_END;
}


__intrinsic
void cmd_cls_ring_get_ind_ptr32(
    __xread void *data,                                 /* data to get                            */
    volatile void __cls *address,                       /* head of ring address                   */
    uint32_t max_nn,                                    /* max. number of longwords to get        */
    generic_ind_t ind,                                  /* indirect word                          */
    sync_t sync,                                        /* type of synchronization to use         */
    SIGNAL *sig_ptr                                     /* signal to raise upon completion        */
    )
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_read_reg(data));
        CT_ASSERT(__is_ct_const(max_nn));
        CT_ASSERT(max_nn >= 1 && max_nn <= 16);
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done || sync == ctx_swap);

        __asm local_csr_wr[CMD_INDIRECT_REF_0, CSR_INDIRECT_TYPE(ind)]

        __asm  alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
        if (sync == sig_done)
        {
            __asm cls[ring_get, *data, address, 0, __ct_const_val(max_nn)], sig_done[*sig_ptr], indirect_ref
        }
        else
        {
            __asm cls[ring_get, *data, address, 0, __ct_const_val(max_nn)], ctx_swap[*sig_ptr], indirect_ref
        }
    }
    _INTRINSIC_END;
}


__intrinsic
void cmd_cls_ring_pop_ind_ptr40(
    __xread void *data,                                 /* data to get                            */
    volatile void __addr40 __cls *address,              /* tail of ring address                   */
    uint32_t max_nn,                                    /* max. number of longwords to get        */
    generic_ind_t ind,                                  /* indirect word                          */
    sync_t sync,                                        /* type of synchronization to use         */
    SIGNAL *sig_ptr                                     /* signal to raise upon completion        */
    )
{
    _INTRINSIC_BEGIN;
    {
         uint32_t hi_addr, low_addr;

        CT_ASSERT(__is_read_reg(data));
        CT_ASSERT(__is_ct_const(max_nn));
        CT_ASSERT(max_nn >= 1 && max_nn <= 16);
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done || sync == ctx_swap);
        _INTRINSIC_CONVERT_HI_LO_ADDRESS(address);

        __asm local_csr_wr[CMD_INDIRECT_REF_0, CSR_INDIRECT_TYPE(ind)]
        __asm  alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
        if (sync == sig_done)
        {
            __asm cls[ring_pop, *data, hi_addr, <<8, low_addr, __ct_const_val(max_nn)], sig_done[*sig_ptr], indirect_ref
        }
        else
        {
            __asm cls[ring_pop, *data, hi_addr, <<8, low_addr, __ct_const_val(max_nn)], ctx_swap[*sig_ptr], indirect_ref
        }
    }
    _INTRINSIC_END;
}


__intrinsic
void cmd_cls_ring_pop_ind_ptr32(
    __xread void *data,                                 /* data to get                            */
    volatile void __cls *address,                       /* tail of ring address                   */
    uint32_t max_nn,                                    /* max. number of longwords to get        */
    generic_ind_t ind,                                  /* indirect word                          */
    sync_t sync,                                        /* type of synchronization to use         */
    SIGNAL *sig_ptr                                     /* signal to raise upon completion        */
    )
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_read_reg(data));
        CT_ASSERT(__is_ct_const(max_nn));
        CT_ASSERT(max_nn >= 1 && max_nn <= 16);
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done || sync == ctx_swap);

        __asm local_csr_wr[CMD_INDIRECT_REF_0, CSR_INDIRECT_TYPE(ind)]

        __asm  alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
        if (sync == sig_done)
        {
            __asm cls[ring_pop, *data, address, 0, __ct_const_val(max_nn)], sig_done[*sig_ptr], indirect_ref
        }
        else
        {
            __asm cls[ring_pop, *data, address, 0, __ct_const_val(max_nn)], ctx_swap[*sig_ptr], indirect_ref
        }
    }
    _INTRINSIC_END;
}


__intrinsic
void cmd_cls_ring_get_safe_ind_ptr40(
    __xread void *data,                                 /* data to get                            */
    volatile void __addr40 __cls *address,              /* head of ring address                   */
    uint32_t max_nn,                                    /* max. number of longwords to get        */
    generic_ind_t ind,                                  /* indirect word                          */
    sync_t sync,                                        /* type of synchronization to use         */
    SIGNAL *sig_ptr                                     /* signal to raise upon completion        */
    )
{
    _INTRINSIC_BEGIN;
    {
         uint32_t hi_addr, low_addr;

        CT_ASSERT(__is_read_reg(data));
        CT_ASSERT(__is_ct_const(max_nn));
        CT_ASSERT(max_nn >= 1 && max_nn <= 16);
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done || sync == ctx_swap);
        _INTRINSIC_CONVERT_HI_LO_ADDRESS(address);

        __asm local_csr_wr[CMD_INDIRECT_REF_0, CSR_INDIRECT_TYPE(ind)]

        __asm  alu[--, --, B, ALU_INDIRECT_TYPE(ind)]

        if (sync == sig_done)
        {
            __asm cls[ring_get_freely, *data, hi_addr, <<8, low_addr, __ct_const_val(max_nn)], sig_done[*sig_ptr], indirect_ref
        }
        else
        {
            __asm cls[ring_get_freely, *data, hi_addr, <<8, low_addr, __ct_const_val(max_nn)], ctx_swap[*sig_ptr], indirect_ref
        }
    }
    _INTRINSIC_END;
}


__intrinsic
void cmd_cls_ring_get_safe_ind_ptr32(
    __xread void *data,                                 /* data to get                            */
    volatile void __cls *address,                       /* head of ring address                   */
    uint32_t max_nn,                                    /* max. number of longwords to get        */
    generic_ind_t ind,                                  /* indirect word                          */
    sync_t sync,                                        /* type of synchronization to use         */
    SIGNAL *sig_ptr                                     /* signal to raise upon completion        */
    )
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_read_reg(data));
        CT_ASSERT(__is_ct_const(max_nn));
        CT_ASSERT(max_nn >= 1 && max_nn <= 16);
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done || sync == ctx_swap);

        __asm local_csr_wr[CMD_INDIRECT_REF_0, CSR_INDIRECT_TYPE(ind)]
        __asm  alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
        if (sync == sig_done)
        {
            __asm cls[ring_get_freely, *data, address, 0, __ct_const_val(max_nn)], sig_done[*sig_ptr], indirect_ref
        }
        else
        {
            __asm cls[ring_get_freely, *data, address, 0, __ct_const_val(max_nn)], ctx_swap[*sig_ptr], indirect_ref
        }
    }
    _INTRINSIC_END;
}


__intrinsic
void cmd_cls_ring_pop_safe_ind_ptr40(
    __xread void *data,                                 /* data to get                            */
    volatile void __addr40 __cls *address,              /* tail of ring address                   */
    uint32_t max_nn,                                    /* max. number of longwords to get        */
    generic_ind_t ind,                                  /* indirect word                          */
    sync_t sync,                                        /* type of synchronization to use         */
    SIGNAL *sig_ptr                                     /* signal to raise upon completion        */
    )
{
    _INTRINSIC_BEGIN;
    {
         uint32_t hi_addr, low_addr;

        CT_ASSERT(__is_read_reg(data));
        CT_ASSERT(__is_ct_const(max_nn));
        CT_ASSERT(max_nn >= 1 && max_nn <= 16);
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done || sync == ctx_swap);
        _INTRINSIC_CONVERT_HI_LO_ADDRESS(address);

        __asm local_csr_wr[CMD_INDIRECT_REF_0, CSR_INDIRECT_TYPE(ind)]
        __asm  alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
        if (sync == sig_done)
        {
            __asm cls[ring_pop_freely, *data, hi_addr, <<8, low_addr, __ct_const_val(max_nn)], sig_done[*sig_ptr], indirect_ref
        }
        else
        {
            __asm cls[ring_pop_freely, *data, hi_addr, <<8, low_addr, __ct_const_val(max_nn)], ctx_swap[*sig_ptr], indirect_ref
        }
    }
    _INTRINSIC_END;
}


__intrinsic
void cmd_cls_ring_pop_safe_ind_ptr32(
    __xread void *data,                                 /* data to get                            */
    volatile void __cls *address,                       /* tail of ring address                   */
    uint32_t max_nn,                                    /* max. number of longwords to get        */
    generic_ind_t ind,                                  /* indirect word                          */
    sync_t sync,                                        /* type of synchronization to use         */
    SIGNAL *sig_ptr                                     /* signal to raise upon completion        */
    )
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_read_reg(data));
        CT_ASSERT(__is_ct_const(max_nn));
        CT_ASSERT(max_nn >= 1 && max_nn <= 16);
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done || sync == ctx_swap);

        __asm local_csr_wr[CMD_INDIRECT_REF_0, CSR_INDIRECT_TYPE(ind)]
        __asm  alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
        if (sync == sig_done)
        {
            __asm cls[ring_pop_freely, *data, address, 0, __ct_const_val(max_nn)], sig_done[*sig_ptr], indirect_ref
        }
        else
        {
            __asm cls[ring_pop_freely, *data, address, 0, __ct_const_val(max_nn)], ctx_swap[*sig_ptr], indirect_ref
        }
    }
    _INTRINSIC_END;
}

#pragma diag_default 279  /* suppressed above */

#endif /* __NFP_CLS_RING_C__ */
