/*
 * Copyright (C) 2008-2014 Netronome Systems, Inc.  All rights reserved.
 */

#ifndef __NFP_CLUSTER_TARGET_RING_C__
#define __NFP_CLUSTER_TARGET_RING_C__

#include <nfp.h>
#include <nfp_intrinsic.h>
#include <nfp_cluster_target.h>
#include <nfp_cluster_target_ring.h>
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

#define _CT_RING_NUMBER_CHECK(ring_number)                              \
        if (__is_ct_const(ring_number))                                 \
        {                                                               \
            CT_ASSERT(ring_number <= 15);                               \
        }                                                               \
        else                                                            \
        {                                                               \
            RT_RANGE_ASSERT(ring_number <= 15);                         \
        }

#define _CT_RING_COUNT_CHECK_16(count)                                  \
        if (__is_ct_const(count))                                       \
        {                                                               \
            CT_ASSERT(count >= 1 && count <= 16);                       \
        }                                                               \
        else                                                            \
        {                                                               \
            RT_RANGE_ASSERT(count >= 1 && count <= 16);                 \
        }
#define _CT_RING_COUNT_CHECK_14(count)                                  \
        if (__is_ct_const(count))                                       \
        {                                                               \
            CT_ASSERT(count >= 1 && count <= 14);                       \
        }                                                               \
        else                                                            \
        {                                                               \
            RT_RANGE_ASSERT(count >= 1 && count <= 14);                 \
        }
#define _CT_RING_ISLAND_CHECK(id)                                       \
        if (__is_ct_const(id))                                          \
        {                                                               \
            CT_ASSERT(id <= 0x3f);                                      \
        }                                                               \
        else                                                            \
        {                                                               \
            RT_RANGE_ASSERT(id <= 0x3f);                                \
        }

#define _CT_RING_STATUS_CHECK(status)                                   \
        if (__is_ct_const(status))                                      \
        {                                                               \
            CT_ASSERT(status <= 1);                                     \
        }                                                               \
        else                                                            \
        {                                                               \
            RT_RANGE_ASSERT(status <= 1);                               \
        }

#define _CT_RING_ADDRESS_WITH_OFFSET(address, ring, offset)                    \
        address = ((ring & 0x0f) << 16) | ((offset & 0x3fff) << 2) | (1 << 20);

#define _CT_RING_ADDRESS(address, ring)                                        \
        address = ((ring & 0x0f) << 2);

/*
 * Verify that base address is properly aligned.
 */
#define _CT_RING_BASE_ADDRESS_ALIGN_CHECK(addr)         \
    if (__is_ct_const(addr))                            \
    {                                                   \
        CT_ASSERT((addr & 0x1ff) == 0);                 \
    }                                                   \
    else                                                \
    {                                                   \
        RT_ALIGN_ASSERT((addr & 0x1ff) == 0);           \
    }

/** CLSRingsMap Address map
*/
typedef enum
{
    CT_RING_BASE_0     =   0x100,

    CT_RING_HEAD_0     =   0x104,

    CT_RING_TAIL_0     =   0x108

} CT_RING_MAP;


/*
* Ring base address
*
*/
#ifdef __BIGENDIAN

typedef union ct_ring_base_t
{
    struct
    {
        uint32_t size           : 3;
        uint32_t status         : 1;
        uint32_t reserved_2     : 10;
        uint32_t base_address   : 9;
        uint32_t reserved_3     : 9;
    };
    uint32_t value;
} ct_ring_base_t;

#else

typedef union ct_ring_base_t
{
    struct
    {
        uint32_t reserved_3     : 9;
        uint32_t base_address   : 9;
        uint32_t reserved_2     : 10;
        uint32_t status         : 1;
        uint32_t size           : 3;
    };
    uint32_t value;
} ct_ring_base_t;

#endif


/*
* Ring head address and ring tail address.
*
*/
#ifdef __BIGENDIAN

typedef union ct_ring_head_tail_t
{
    struct
    {
        uint32_t reserved_2     : 15;
        uint32_t offset         : 15;
        uint32_t reserved_3     : 2;
    };
    uint32_t value;
} ct_ring_head_tail_t;

#else

typedef union ct_ring_head_tail_t
{
    struct
    {
        uint32_t reserved_3     : 2;
        uint32_t offset         : 15;
        uint32_t reserved_2     : 15;
    };
    uint32_t value;
} ct_ring_head_tail_t;

#endif

/* Cluster target put ring  */

void cmd_cluster_target_ring_put(
    __xwrite void *data,
    uint32_t island_number,
    uint32_t ring_number,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
)
{
    _INTRINSIC_BEGIN;
    {
        __gpr uint32_t ring_address;

        // max 16 allowed for revision C0 and greater - see errata
        if (__is_nfp_arch_or_above(__nfp6000, __REVISION_C0))
        {
            _CT_RING_COUNT_CHECK_16(count);
        }
        else
        {
            _CT_RING_COUNT_CHECK_14(count);
        }

        CT_ASSERT(__is_write_reg(data));
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done || sync == ctx_swap);
        _CT_RING_NUMBER_CHECK(ring_number);
        _CT_RING_ISLAND_CHECK(island_number);

        ring_address = ((island_number & 0x3f) << 24) | ((ring_number & 0xf) << 2);

        if (__is_ct_const(count) && count <= 8)
        {
            CT_ASSERT(count != 0);
            if (sync == sig_done)
            {
                __asm ct[ring_put, *data, ring_address , 0, __ct_const_val(count)], sig_done[*sig_ptr]
            }
            else
            {
                __asm ct[ring_put, *data, ring_address , 0, __ct_const_val(count)], ctx_swap[*sig_ptr]
            }
        }
        else
        {
            if (!__is_ct_const(count))
            {
                CT_QPERFINFO_INDIRECT_REF(cmd_cls_ring_put_ptr32);
            }

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
                __asm ct[ring_put, *data, ring_address , 0, __ct_const_val(count)], sig_done[*sig_ptr], indirect_ref
            }
            else
            {
                __asm ct[ring_put, *data, ring_address , 0, __ct_const_val(count)], ctx_swap[*sig_ptr], indirect_ref
            }
        }
    }
    _INTRINSIC_END;
}



/* Cluster Target get ring          */

void cmd_cluster_target_ring_get(
    __xread void *data,
    uint32_t island_number,
    uint32_t ring_number,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
)
{
    _INTRINSIC_BEGIN;
    {
        __gpr uint32_t ring_address;

        CT_ASSERT(__is_read_reg(data));
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done || sync == ctx_swap);
        _CT_RING_NUMBER_CHECK(ring_number);
        _CT_RING_ISLAND_CHECK(island_number);
        _CT_RING_COUNT_CHECK_16(count);

        ring_address = ((island_number & 0x3f) << 24) | ((ring_number & 0xf) << 2);

        if (__is_ct_const(count) && count <= 8)
        {
            CT_ASSERT(count != 0);
            if (sync == sig_done)
            {
                __asm ct[ring_get, *data, ring_address , 0, __ct_const_val(count)], sig_done[*sig_ptr]
            }
            else
            {
                __asm ct[ring_get, *data, ring_address , 0, __ct_const_val(count)], ctx_swap[*sig_ptr]
            }
        }
        else
        {
            if (!__is_ct_const(count))
            {
                CT_QPERFINFO_INDIRECT_REF(cmd_cls_ring_put_ptr32);
            }

            {
                _INTRINSIC_IND_ALU_COUNT_MIN_MAX(count, 1, 16);
                if (sync == sig_done)
                {
                    __asm ct[ring_get, *data, ring_address , 0, __ct_const_val(count)], sig_done[*sig_ptr], indirect_ref
                }
                else
                {
                    __asm ct[ring_get, *data, ring_address , 0, __ct_const_val(count)], ctx_swap[*sig_ptr], indirect_ref
                }
            }
        }
    }
    _INTRINSIC_END;
}



/* Cluster Target ring init 32-bit addressing (local CTM) */
__intrinsic
void cmd_cluster_target_ring_init_ptr32(
    uint32_t ring_number,
    volatile void __addr32 __ctm *base_ptr,
    enum CT_RING_SIZE size,
    enum CT_RING_STATUS status_events,
    sync_t sync,
    SIGNAL *sig_ptr
)
{
    _INTRINSIC_BEGIN;
    {
        uint32_t base_address = (uint32_t) (base_ptr) >> 9;

        _CT_RING_BASE_ADDRESS_ALIGN_CHECK((uint32_t)base_ptr);

        cmd_cluster_target_ring_init
        (
            0,
            ring_number,
            base_address,
            size,
            status_events,
            sync,
            sig_ptr
        );
    }
    _INTRINSIC_END;
}

/* Cluster Target ring init 40-bit addressing (non-local CTM) */
__intrinsic
void cmd_cluster_target_ring_init_ptr40(
    uint32_t ring_number,
    volatile void __addr40 __ctm *base_ptr,
    enum CT_RING_SIZE size,
    enum CT_RING_STATUS status_events,
    sync_t sync,
    SIGNAL *sig_ptr
)
{
    _INTRINSIC_BEGIN;
    {
        uint32_t base_address = (uint32_t)(base_ptr) >> 9;
        uint32_t island_number = (uint32_t)((uint64_t) base_ptr >> 32) & 0x3f;

        _CT_RING_BASE_ADDRESS_ALIGN_CHECK((uint32_t)base_ptr);

        cmd_cluster_target_ring_init
        (
            island_number,
            ring_number,
            base_address,
            size,
            status_events,
            sync,
            sig_ptr
        );
    }
    _INTRINSIC_END;
}


__intrinsic
void cmd_cluster_target_ring_init(
    uint32_t island_number,
    uint32_t ring_number,
    uint32_t base_address,
    enum CT_RING_SIZE size,
    enum CT_RING_STATUS status_events,
    sync_t sync,
    SIGNAL *sig_ptr
)
{
    _INTRINSIC_BEGIN;

    CT_ASSERT(__is_ct_const(sync));
    CT_ASSERT(sync == sig_done || sync == ctx_swap);
    _CT_RING_NUMBER_CHECK(ring_number);
    _CT_RING_STATUS_CHECK(status_events);

    {
        __xwrite uint32_t          xfer[2];
        cluster_target_xpb_address_format_t         xpb_command;

        // Setup xpb command address
        xpb_command.value =  (island_number << 24) | (0x08 << 16) | (((CT_RING_BASE_0 + (ring_number * 0x10)) >> 2) << 2);


        /* Setup ring base values */
        xfer[0] = (((uint32_t)size & 0x07) << 29) | ((status_events & 0x1) << 28) |  ((base_address & 0x1ff) << 9);

        if (sync == sig_done)
        {
            cmd_cluster_target_xpb_write((void *)&xfer[0], &xpb_command, 1, sig_done, sig_ptr);
            __wait_for_all(sig_ptr);
        }
        else
        {
            cmd_cluster_target_xpb_write((void *)&xfer[0], &xpb_command, 1, ctx_swap, sig_ptr);
        }

        /* Setup ring head - offset to 0 */
        xfer[1] = 0x00;
        xpb_command.address = (CT_RING_HEAD_0 + (ring_number * 0x10)) >> 2;

        if (sync == sig_done)
        {
            cmd_cluster_target_xpb_write((void *)&xfer[1], &xpb_command, 1, sig_done, sig_ptr);
            __wait_for_all(sig_ptr);
        }
        else
        {
            cmd_cluster_target_xpb_write((void *)&xfer[1], &xpb_command, 1, ctx_swap, sig_ptr);
        }

        /* Setup ring tail - offset to 0 */
        xpb_command.address = (CT_RING_TAIL_0 + (ring_number * 0x10)) >> 2;

        if (sync == sig_done)
        {
            cmd_cluster_target_xpb_write((void *)&xfer[1], &xpb_command, 1, sig_done, sig_ptr);
            __wait_for_all(sig_ptr);
        }
        else
        {
            cmd_cluster_target_xpb_write((void *)&xfer[1], &xpb_command, 1, ctx_swap, sig_ptr);
        }


        /* Do a readback of ring tail to ensure the CSR update is complete before accessing the rings*/
        {
            __xread int read_xfer;
            if (sync == sig_done)
            {
                cmd_cluster_target_xpb_read((void *)&read_xfer, &xpb_command, 1, sig_done, sig_ptr);
            }
            else
            {
                cmd_cluster_target_xpb_read((void *)&read_xfer, &xpb_command, 1, ctx_swap, sig_ptr);
            }
        }
    }

_INTRINSIC_END;
}

#endif /* __NFP_CLUSTER_TARGET_RING_C__ */
