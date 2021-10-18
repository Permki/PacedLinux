/*
 * Copyright (C) 2008-2014 Netronome Systems, Inc.  All rights reserved.
 */

#ifndef __NFP_MEM_LOCKQ_C__
#define __NFP_MEM_LOCKQ_C__

#include <nfp.h>
#include <nfp_intrinsic.h>
#include <nfp_mem_lockq.h>

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

#define _MEM_LOCKQ_ALIGN_16_CHECK(addr)                     \
        if (__is_ct_const(addr))                            \
        {                                                   \
            CT_ASSERT((addr & 0xf) == 0);                   \
        }                                                   \
        else                                                \
        {                                                   \
            RT_ALIGN_ASSERT((addr & 0xf) == 0);             \
        }

#define _MEM_LOCKQ_ZERO_OR_ONE_CHECK(z_or_o)                \
        if (__is_ct_const(z_or_o))                          \
        {                                                   \
            CT_ASSERT(z_or_o == 0 || z_or_o == 1);          \
        }                                                   \
        else                                                \
        {                                                   \
            RT_RANGE_ASSERT(z_or_o == 0 || z_or_o == 1);    \
        }

#define _MEM_LOCKQ_SIGNAL_PAIR_CHECK(sync)                  \
        if (__is_ct_const(sync))                            \
        {                                                   \
            CT_ASSERT(sync == sig_done);                    \
        }

#define _MEM_LOCKQ_EVENT_SOURCE_CHECK(event_source)         \
        if (__is_ct_const(event_source))                    \
        {                                                   \
            CT_ASSERT(event_source < (1 << 12));            \
        }                                                   \
        else                                                \
        {                                                   \
            RT_RANGE_ASSERT(event_source < (1 << 12));      \
        }


__intrinsic
void cmd_mem_lockq128_lock_ptr32(
    __mem mem_lockq128_in_mem_t *lockq,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
    )
{
    _INTRINSIC_BEGIN;
    {
        _MEM_LOCKQ_ALIGN_16_CHECK((uint32_t)lockq);
        _MEM_LOCKQ_SIGNAL_PAIR_CHECK(sync);
        __asm mem[queue128_lock, --, lockq, 0 ], sig_done[*sig_pair_ptr]
    }
    _INTRINSIC_END;
}


__intrinsic
void cmd_mem_lockq128_lock_ind_ptr32(
    __mem mem_lockq128_in_mem_t *lockq,
    generic_ind_t ind,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
    )
{
    _INTRINSIC_BEGIN;
    {
        _MEM_LOCKQ_ALIGN_16_CHECK((uint32_t)lockq);
        _MEM_LOCKQ_SIGNAL_PAIR_CHECK(sync);
        __asm
        {
            alu[--, --, B, ind]
            mem[queue128_lock, --, lockq, 0 ], sig_done[*sig_pair_ptr], indirect_ref
        }
    }
    _INTRINSIC_END;
}


__intrinsic
void cmd_mem_lockq128_lock_ptr40(
    mem_lockq128_ptr40_t lockq,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
    )
{
    _INTRINSIC_BEGIN;
    {
        uint32_t hi_addr, low_addr;

        _INTRINSIC_CONVERT_HI_LO_ADDRESS(lockq);
        _MEM_LOCKQ_ALIGN_16_CHECK((uint32_t)lockq);
        _MEM_LOCKQ_SIGNAL_PAIR_CHECK(sync);
        __asm mem[queue128_lock, --, hi_addr, <<8, low_addr], sig_done[*sig_pair_ptr]
    }
    _INTRINSIC_END;
}


__intrinsic
void cmd_mem_lockq128_lock_ind_ptr40(
    mem_lockq128_ptr40_t lockq,
    generic_ind_t ind,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
    )
{
    _INTRINSIC_BEGIN;
    {
        uint32_t hi_addr, low_addr;

        _INTRINSIC_CONVERT_HI_LO_ADDRESS(lockq);
        _MEM_LOCKQ_ALIGN_16_CHECK((uint32_t)lockq);
        _MEM_LOCKQ_SIGNAL_PAIR_CHECK(sync);
        __asm
        {
            alu[--, --, B, ind]
            mem[queue128_lock, --, hi_addr, <<8, low_addr], sig_done[*sig_pair_ptr], indirect_ref
        }
    }
    _INTRINSIC_END;
}


__intrinsic
void cmd_mem_lockq256_lock_ptr32(
    __mem mem_lockq256_in_mem_t *lockq,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
    )
{
    _INTRINSIC_BEGIN;
    {
        _MEM_LOCKQ_ALIGN_16_CHECK((uint32_t)lockq);
        _MEM_LOCKQ_SIGNAL_PAIR_CHECK(sync);
        __asm mem[queue256_lock, --, lockq, 0 ], sig_done[*sig_pair_ptr]
    }
    _INTRINSIC_END;
}


__intrinsic
void cmd_mem_lockq256_lock_ind_ptr32(
    __mem mem_lockq256_in_mem_t *lockq,
    generic_ind_t ind,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
    )
{
    _INTRINSIC_BEGIN;
    {
        _MEM_LOCKQ_ALIGN_16_CHECK((uint32_t)lockq);
        _MEM_LOCKQ_SIGNAL_PAIR_CHECK(sync);
        __asm
        {
            alu[--, --, B, ind]
            mem[queue256_lock, --, lockq, 0 ], sig_done[*sig_pair_ptr], indirect_ref
        }
    }
    _INTRINSIC_END;
}


__intrinsic
void cmd_mem_lockq256_lock_ptr40(
    mem_lockq256_ptr40_t lockq,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
    )
{
    _INTRINSIC_BEGIN;
    {
        uint32_t hi_addr, low_addr;

        _INTRINSIC_CONVERT_HI_LO_ADDRESS(lockq);
        _MEM_LOCKQ_ALIGN_16_CHECK((uint32_t)lockq);
        _MEM_LOCKQ_SIGNAL_PAIR_CHECK(sync);
       __asm mem[queue256_lock, --, hi_addr, <<8, low_addr], sig_done[*sig_pair_ptr]
    }
    _INTRINSIC_END;
}


__intrinsic
void cmd_mem_lockq256_lock_ind_ptr40(
    mem_lockq256_ptr40_t lockq,
    generic_ind_t ind,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
    )
{
    _INTRINSIC_BEGIN;
    {
        uint32_t hi_addr, low_addr;

        _INTRINSIC_CONVERT_HI_LO_ADDRESS(lockq);
        _MEM_LOCKQ_ALIGN_16_CHECK((uint32_t)lockq);
        _MEM_LOCKQ_SIGNAL_PAIR_CHECK(sync);
        __asm
        {
            alu[--, --, B, ind]
            mem[queue256_lock, --, hi_addr, <<8, low_addr, 0 ], sig_done[*sig_pair_ptr], indirect_ref
        }
    }
    _INTRINSIC_END;
}


__intrinsic
void cmd_mem_lockq128_unlock_ptr32(
    __mem mem_lockq128_in_mem_t *lockq
    )
{
    _INTRINSIC_BEGIN;
    {
        _MEM_LOCKQ_ALIGN_16_CHECK((uint32_t)lockq);
        __asm mem[queue128_unlock, --, lockq, 0]
    }
    _INTRINSIC_END;
}


__intrinsic
void cmd_mem_lockq128_unlock_ind_ptr32(
    __mem mem_lockq128_in_mem_t *lockq,
    generic_ind_t ind
    )
{
    _INTRINSIC_BEGIN;
    {
        _MEM_LOCKQ_ALIGN_16_CHECK((uint32_t)lockq);
        __asm
        {
            alu[--, --, B, ind]
            mem[queue128_unlock, --, lockq, 0], indirect_ref
        }
    }
    _INTRINSIC_END;
}


__intrinsic
void cmd_mem_lockq128_unlock_ptr40(
    mem_lockq128_ptr40_t lockq
    )
{
    _INTRINSIC_BEGIN;
    {
        uint32_t hi_addr, low_addr;

        _INTRINSIC_CONVERT_HI_LO_ADDRESS(lockq);
        _MEM_LOCKQ_ALIGN_16_CHECK((uint32_t)lockq);
        __asm mem[queue128_unlock, --, hi_addr, <<8, low_addr]
    }
    _INTRINSIC_END;
}


__intrinsic
void cmd_mem_lockq128_unlock_ind_ptr40(
    mem_lockq128_ptr40_t lockq,
    generic_ind_t ind
    )
{
    _INTRINSIC_BEGIN;
    {
        uint32_t hi_addr, low_addr;

        _INTRINSIC_CONVERT_HI_LO_ADDRESS(lockq);
        _MEM_LOCKQ_ALIGN_16_CHECK((uint32_t)lockq);
        __asm
        {
            alu[--, --, B, ind]
            mem[queue128_unlock, --, hi_addr, <<8, low_addr], indirect_ref
        }
    }
    _INTRINSIC_END;
}


__intrinsic
void cmd_mem_lockq256_unlock_ptr32(
    __mem mem_lockq256_in_mem_t *lockq
    )
{
    _INTRINSIC_BEGIN;
    {
        _MEM_LOCKQ_ALIGN_16_CHECK((uint32_t)lockq);
        __asm mem[queue256_unlock, --, lockq, 0]
    }
    _INTRINSIC_END;
}


__intrinsic
void cmd_mem_lockq256_unlock_ind_ptr32(
    __mem mem_lockq256_in_mem_t *lockq,
    generic_ind_t ind
    )
{
    _INTRINSIC_BEGIN;
    {
        _MEM_LOCKQ_ALIGN_16_CHECK((uint32_t)lockq);
        __asm
        {
            alu[--, --, B, ind]
            mem[queue256_unlock, --, lockq, 0], indirect_ref
        }
    }
    _INTRINSIC_END;
}


__intrinsic
void cmd_mem_lockq256_unlock_ptr40(
    mem_lockq256_ptr40_t lockq
    )
{
    _INTRINSIC_BEGIN;
    {
        uint32_t hi_addr, low_addr;

        _INTRINSIC_CONVERT_HI_LO_ADDRESS(lockq);
        _MEM_LOCKQ_ALIGN_16_CHECK((uint32_t)lockq);
        __asm mem[queue256_unlock, --, hi_addr, <<8, low_addr]
    }
    _INTRINSIC_END;
}


__intrinsic
void cmd_mem_lockq256_unlock_ind_ptr40(
    mem_lockq256_ptr40_t lockq,
    generic_ind_t ind
    )
{
    _INTRINSIC_BEGIN;
    {
        uint32_t hi_addr, low_addr;

        _INTRINSIC_CONVERT_HI_LO_ADDRESS(lockq);
        _MEM_LOCKQ_ALIGN_16_CHECK((uint32_t)lockq);
        __asm
        {
            alu[--, --, B, ind]
            mem[queue256_unlock, --, hi_addr, <<8, low_addr], indirect_ref
        }
    }
    _INTRINSIC_END;
}


__intrinsic
void cmd_mem_lockq128_init_ptr32(
    volatile __addr32 __mem __align16 mem_lockq128_t *lockq,
    uint32_t generate_event,                                    /* Generate event on under or overflow */
    uint32_t event_source
)
{
    _INTRINSIC_BEGIN;
    {
        mem_lockq_desc_t lq128;
        _MEM_LOCKQ_ALIGN_16_CHECK((uint32_t)lockq);
        _MEM_LOCKQ_ZERO_OR_ONE_CHECK(generate_event);
        _MEM_LOCKQ_EVENT_SOURCE_CHECK(event_source);

        lockq->desc.value = 0;
        lockq->desc.event_source = event_source;
        lockq->desc.generate_event = generate_event;
        {
            // Read back
            volatile mem_lockq_desc_t        lqrb;
            lqrb.value  = lockq->desc.value;
        }

    }
    _INTRINSIC_END;
}

__intrinsic
void cmd_mem_lockq128_init_ptr40(
    mem_lockq128_ptr40_t lockq,
    uint32_t generate_event,            /* Generate event on under or overflow */
    uint32_t event_source
)
{
    _INTRINSIC_BEGIN;
    {
        mem_lockq_desc_t lq128;
        _MEM_LOCKQ_ALIGN_16_CHECK((uint32_t)lockq);
        _MEM_LOCKQ_ZERO_OR_ONE_CHECK(generate_event);
        _MEM_LOCKQ_EVENT_SOURCE_CHECK(event_source);

        lockq->desc.value = 0;
        lockq->desc.event_source = event_source;
        lockq->desc.generate_event = generate_event;
        {
            /* Read back */
            volatile mem_lockq_desc_t  lqrb;
            lqrb.value  = lockq->desc.value;
        }
    }
    _INTRINSIC_END;
}

__intrinsic
void cmd_mem_lockq256_init_ptr32(
    __mem mem_lockq256_in_mem_t *lockq,
    uint32_t generate_event,                /* Generate event on under or overflow */
    uint32_t event_source
)
{
    _INTRINSIC_BEGIN;
    {
        mem_lockq_desc_t lq256;
        _MEM_LOCKQ_ALIGN_16_CHECK((uint32_t)lockq);
        _MEM_LOCKQ_ZERO_OR_ONE_CHECK(generate_event);
        _MEM_LOCKQ_EVENT_SOURCE_CHECK(event_source);

        lockq->desc.value = 0;
        lockq->desc.event_source = event_source;
        lockq->desc.generate_event = generate_event;
        {
            /* Read back */
            volatile mem_lockq_desc_t  lqrb;
            lqrb.value  = lockq->desc.value;
        }
    }
    _INTRINSIC_END;
}

__intrinsic
void cmd_mem_lockq256_init_ptr40(
    mem_lockq256_ptr40_t lockq,
    uint32_t generate_event,            /* Generate event on under or overflow */
    uint32_t event_source
)
{
    _INTRINSIC_BEGIN;
    {
        mem_lockq_desc_t lq256;
        _MEM_LOCKQ_ALIGN_16_CHECK((uint32_t)lockq);
        _MEM_LOCKQ_ZERO_OR_ONE_CHECK(generate_event);
        _MEM_LOCKQ_EVENT_SOURCE_CHECK(event_source);

        lockq->desc.value = 0;
        lockq->desc.event_source = event_source;
        lockq->desc.generate_event = generate_event;
        {
            /* Read back */
            volatile mem_lockq_desc_t  lqrb;
            lqrb.value  = lockq->desc.value;
        }
    }
    _INTRINSIC_END;
}

#endif /* __NFP_MEM_LOCKQ_C__ */
