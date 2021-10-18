/*
 * Copyright (C) 2008-2014 Netronome Systems, Inc.  All rights reserved.
 */

#ifndef __NFP_MEM_MICROQ_C__
#define __NFP_MEM_MICROQ_C__

#include <nfp.h>
#include <nfp_intrinsic.h>
#include <nfp_mem_microq.h>

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

#define _MEM_MICROQ_ALIGN_16_CHECK(addr)                    \
        if (__is_ct_const(addr))                            \
        {                                                   \
            CT_ASSERT((addr & 0xf) == 0);                   \
        }                                                   \
        else                                                \
        {                                                   \
            RT_ALIGN_ASSERT((addr & 0xf) == 0);             \
        }

#define _MEM_MICROQ_ZERO_OR_ONE_CHECK(z_or_o)               \
        if (__is_ct_const(z_or_o))                          \
        {                                                   \
            CT_ASSERT(z_or_o == 0 || z_or_o == 1);          \
        }                                                   \
        else                                                \
        {                                                   \
            RT_RANGE_ASSERT(z_or_o == 0 || z_or_o == 1);    \
        }

#define _MEM_MICROQ_SIGNAL_CHECK(sig)                       \
        if (__is_ct_const(sig))                             \
        {                                                   \
            CT_ASSERT(sig == sig_done || sig == ctx_swap);  \
        }

#define _MEM_MICROQ_SIGNAL_PAIR_CHECK(sync)                 \
        if (__is_ct_const(sync))                            \
        {                                                   \
            CT_ASSERT(sync == sig_done);                    \
        }

#define _MEM_MICROQ_EVENT_SOURCE_CHECK(event_source)        \
        if (__is_ct_const(event_source))                    \
        {                                                   \
            CT_ASSERT(event_source < (1 << 12));            \
        }                                                   \
        else                                                \
        {                                                   \
            RT_RANGE_ASSERT(event_source < (1 << 12));      \
        }

#define _MEM_DECLARE_AND_SET_HI_AND_LOW_ADDR(addr)              \
        uint32_t hi_addr, low_addr;                             \
        hi_addr = (uint32_t)((uint64_t)addr>>8 & 0xFFFFFFFF);   \
        low_addr = (uint32_t)((uint64_t)addr & 0xFF);

__intrinsic
void cmd_mem_microq128_put_ptr32(
   __mem mem_microq128_in_mem_t *microq,
    __xwrite void *xfer,
    sync_t sync,
    SIGNAL *sig_ptr
)
{
    _INTRINSIC_BEGIN;
    {
        _MEM_MICROQ_ALIGN_16_CHECK((uint32_t)microq);
        CT_ASSERT(__is_write_reg(xfer));
        _MEM_MICROQ_SIGNAL_CHECK(sync);
        if (sync == sig_done)
        {
            __asm
            {
                mem[microq128_put, *xfer, microq, 0 ], sig_done[*sig_ptr]
            }
        }
        else
        {
            __asm
            {
                mem[microq128_put, *xfer, microq, 0 ], ctx_swap[*sig_ptr]
            }
        }
    }
    _INTRINSIC_END;
}

__intrinsic
void cmd_mem_microq128_put_ptr40(
    mem_microq128_ptr40_t microq,
    __xwrite void *xfer,
    sync_t sync,
    SIGNAL *sig_ptr
)
{
    _INTRINSIC_BEGIN;
    {
        uint32_t hi_addr, low_addr;
        _INTRINSIC_CONVERT_HI_LO_ADDRESS(microq);
        _MEM_MICROQ_ALIGN_16_CHECK((uint32_t)microq);
        CT_ASSERT(__is_write_reg(xfer));
        _MEM_MICROQ_SIGNAL_CHECK(sync);
        if (sync == sig_done)
        {
            __asm
            {
                mem[microq128_put, *xfer, hi_addr, <<8, low_addr ], sig_done[*sig_ptr]
            }
        }
        else
        {
            __asm
            {
                mem[microq128_put, *xfer, hi_addr, <<8, low_addr ], ctx_swap[*sig_ptr]
            }
        }
    }
    _INTRINSIC_END;
}

__intrinsic
void cmd_mem_microq256_put_ptr32(
    __mem mem_microq256_in_mem_t *microq,
    __xwrite void *xfer,
    sync_t sync,
    SIGNAL *sig_ptr
)
{
    _INTRINSIC_BEGIN;
    {
        _MEM_MICROQ_ALIGN_16_CHECK((uint32_t)microq);
        CT_ASSERT(__is_write_reg(xfer));
        _MEM_MICROQ_SIGNAL_CHECK(sync);
        if (sync == sig_done)
        {
            __asm
            {
                mem[microq256_put, *xfer, microq, 0 ], sig_done[*sig_ptr]
            }
        }
        else
        {
            __asm
            {
                mem[microq256_put, *xfer, microq, 0 ], ctx_swap[*sig_ptr]
            }
        }
    }
    _INTRINSIC_END;
}

__intrinsic
void cmd_mem_microq256_put_ptr40(
    mem_microq256_ptr40_t microq,
    __xwrite void *xfer,
    sync_t sync,
    SIGNAL *sig_ptr
)
{
    _INTRINSIC_BEGIN;
    {
        uint32_t hi_addr, low_addr;
        _INTRINSIC_CONVERT_HI_LO_ADDRESS(microq);
        _MEM_MICROQ_ALIGN_16_CHECK((uint32_t)microq);
        CT_ASSERT(__is_write_reg(xfer));
        _MEM_MICROQ_SIGNAL_CHECK(sync);
        if (sync == sig_done)
        {
            __asm
            {
                mem[microq256_put, *xfer, hi_addr, <<8, low_addr ], sig_done[*sig_ptr]
            }
        }
        else
        {
            __asm
            {
                mem[microq256_put, *xfer, hi_addr, <<8, low_addr ], ctx_swap[*sig_ptr]
            }
        }
    }
    _INTRINSIC_END;
}

__intrinsic
void cmd_mem_microq128_get_ptr32(
    __mem mem_microq128_in_mem_t *microq,
    __xread void *xfer,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
)
{
    _INTRINSIC_BEGIN;
    {
        _MEM_MICROQ_ALIGN_16_CHECK((uint32_t)microq);
        CT_ASSERT(__is_read_reg(xfer));
        _MEM_MICROQ_SIGNAL_PAIR_CHECK(sync);
        __asm
        {
            mem[microq128_get, *xfer, microq, 0 ], sig_done[*sig_pair_ptr]
        }
    }
    _INTRINSIC_END;
}

__intrinsic
void cmd_mem_microq128_get_ptr40(
    mem_microq128_ptr40_t microq,
    __xread void *xfer,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
)
{
    _INTRINSIC_BEGIN;
    {
        uint32_t hi_addr, low_addr;
        _INTRINSIC_CONVERT_HI_LO_ADDRESS(microq);
        _MEM_MICROQ_ALIGN_16_CHECK((uint32_t)microq);
        CT_ASSERT(__is_read_reg(xfer));
        _MEM_MICROQ_SIGNAL_PAIR_CHECK(sync);
        __asm
        {
            mem[microq128_get, *xfer, hi_addr, <<8, low_addr ], sig_done[*sig_pair_ptr]
        }
    }
    _INTRINSIC_END;
}

__intrinsic
void cmd_mem_microq256_get_ptr32(
    __mem mem_microq256_in_mem_t *microq,
    __xread void *xfer,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
)

{
    _INTRINSIC_BEGIN;
    {
        _MEM_MICROQ_ALIGN_16_CHECK((uint32_t)microq);
        CT_ASSERT(__is_read_reg(xfer));
        _MEM_MICROQ_SIGNAL_PAIR_CHECK(sync);
        __asm
        {
            mem[microq256_get, *xfer, microq, 0 ], sig_done[*sig_pair_ptr]
        }
    }
    _INTRINSIC_END;
}

__intrinsic
void cmd_mem_microq256_get_ptr40(
    mem_microq256_ptr40_t microq,
    __xread void *xfer,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
)

{
    _INTRINSIC_BEGIN;
    {
        uint32_t hi_addr, low_addr;
        _INTRINSIC_CONVERT_HI_LO_ADDRESS(microq);
        _MEM_MICROQ_ALIGN_16_CHECK((uint32_t)microq);
        CT_ASSERT(__is_read_reg(xfer));
        _MEM_MICROQ_SIGNAL_PAIR_CHECK(sync);
        __asm
        {
            mem[microq256_get, *xfer, hi_addr, <<8, low_addr ], sig_done[*sig_pair_ptr]
        }
    }
    _INTRINSIC_END;
}

__intrinsic
void cmd_mem_microq128_pop_ptr32(
    __mem mem_microq128_in_mem_t *microq,
    __xread void *xfer,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
)

{
    _INTRINSIC_BEGIN;
    {
        _MEM_MICROQ_ALIGN_16_CHECK((uint32_t)microq);
        CT_ASSERT(__is_read_reg(xfer));
        _MEM_MICROQ_SIGNAL_PAIR_CHECK(sync);
        __asm
        {
            mem[microq128_pop, *xfer, microq, 0 ], sig_done[*sig_pair_ptr]
        }
    }
    _INTRINSIC_END;
}

__intrinsic
void cmd_mem_microq128_pop_ptr40(
    mem_microq128_ptr40_t microq,
    __xread void *xfer,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
)

{
    _INTRINSIC_BEGIN;
    {
        uint32_t hi_addr, low_addr;
        _INTRINSIC_CONVERT_HI_LO_ADDRESS(microq);
        _MEM_MICROQ_ALIGN_16_CHECK((uint32_t)microq);
        CT_ASSERT(__is_read_reg(xfer));
        _MEM_MICROQ_SIGNAL_PAIR_CHECK(sync);
        __asm
        {
            mem[microq128_pop, *xfer, hi_addr, <<8, low_addr ], sig_done[*sig_pair_ptr]
        }
    }
    _INTRINSIC_END;
}


__intrinsic
void cmd_mem_microq256_pop_ptr32(
    __mem mem_microq256_in_mem_t *microq,
    __xread void  *xfer,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
)

{
    _INTRINSIC_BEGIN;
    {
        _MEM_MICROQ_ALIGN_16_CHECK((uint32_t)microq);
        CT_ASSERT(__is_read_reg(xfer));
        _MEM_MICROQ_SIGNAL_PAIR_CHECK(sync);
        __asm
        {
            mem[microq256_pop, *xfer, microq, 0 ], sig_done[*sig_pair_ptr]
        }
    }
    _INTRINSIC_END;
}

__intrinsic
void cmd_mem_microq256_pop_ptr40(
    mem_microq256_ptr40_t microq,
    __xread void  *xfer,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
)

{
    _INTRINSIC_BEGIN;
    {
        uint32_t hi_addr, low_addr;
        _INTRINSIC_CONVERT_HI_LO_ADDRESS(microq);
        _MEM_MICROQ_ALIGN_16_CHECK((uint32_t)microq);
        CT_ASSERT(__is_read_reg(xfer));
        _MEM_MICROQ_SIGNAL_PAIR_CHECK(sync);
        __asm
        {
            mem[microq256_pop, *xfer, hi_addr, <<8, low_addr ], sig_done[*sig_pair_ptr]
        }
    }
    _INTRINSIC_END;
}

__intrinsic
void cmd_mem_microq128_init_ptr32(
    __mem mem_microq128_in_mem_t *microq,
    enum MICROQ_ENTRY_SIZE entry_size,      /* 1 = 32-bit, 0 = 16-bit */
    uint32_t generate_event,                /* Generate event on under or overflow */
    uint32_t event_source
)
{
    _INTRINSIC_BEGIN;
    {
        mem_microq_desc_t mq128;
        _MEM_MICROQ_ALIGN_16_CHECK((uint32_t)microq);
        _MEM_MICROQ_ZERO_OR_ONE_CHECK(entry_size);
        _MEM_MICROQ_ZERO_OR_ONE_CHECK(generate_event);
        _MEM_MICROQ_EVENT_SOURCE_CHECK(event_source);

        microq->desc.value = 0;
        microq->desc.event_source = event_source;
        microq->desc.generate_event = generate_event;
        microq->desc.entry_size = entry_size;

        {
            /* Read back */
            volatile mem_microq_desc_t mqrb;
            mqrb.value  = microq->desc.value;
        }
    }
    _INTRINSIC_END;
}

__intrinsic
void cmd_mem_microq128_init_ptr40(
    mem_microq128_ptr40_t microq,
    enum MICROQ_ENTRY_SIZE entry_size,      /* 1 = 32-bit, 0 = 16-bit */
    uint32_t generate_event,                /* Generate event on under or overflow */
    uint32_t event_source
)
{
    _INTRINSIC_BEGIN;
    {
        mem_microq_desc_t mq128;
        _MEM_MICROQ_ALIGN_16_CHECK((uint32_t)microq);
        _MEM_MICROQ_ZERO_OR_ONE_CHECK(entry_size);
        _MEM_MICROQ_ZERO_OR_ONE_CHECK(generate_event);
        _MEM_MICROQ_EVENT_SOURCE_CHECK(event_source);

        microq->desc.value = 0;
        microq->desc.event_source = event_source;
        microq->desc.generate_event = generate_event;
        microq->desc.entry_size = entry_size;

        {
            /* Read back */
            volatile mem_microq_desc_t mqrb;
            mqrb.value  = microq->desc.value;
        }
    }
    _INTRINSIC_END;
}

__intrinsic
void cmd_mem_microq256_init_ptr32(
    __mem mem_microq256_in_mem_t *microq,
    enum MICROQ_ENTRY_SIZE entry_size,      /* 1 = 32-bit, 0 = 16-bit */
    uint32_t generate_event,                /* Generate event on under or overflow */
    uint32_t event_source
)
{
    _INTRINSIC_BEGIN;
    {
        mem_microq_desc_t mq256;
        _MEM_MICROQ_ALIGN_16_CHECK((uint32_t)microq);
        _MEM_MICROQ_ZERO_OR_ONE_CHECK(entry_size);
        _MEM_MICROQ_ZERO_OR_ONE_CHECK(generate_event);
        _MEM_MICROQ_EVENT_SOURCE_CHECK(event_source);

        microq->desc.value = 0;
        microq->desc.event_source = event_source;
        microq->desc.generate_event = generate_event;
        microq->desc.entry_size = entry_size;

        {
            /* Read back */
            volatile mem_microq_desc_t mqrb;
            mqrb.value  = microq->desc.value;
        }
    }
    _INTRINSIC_END;
}

__intrinsic
void cmd_mem_microq256_init_ptr40(
    mem_microq256_ptr40_t microq,
    enum MICROQ_ENTRY_SIZE entry_size,      /* 1 = 32-bit, 0 = 16-bit */
    uint32_t generate_event,                /* Generate event on under or overflow */
    uint32_t event_source
)
{
    _INTRINSIC_BEGIN;
    {
        mem_microq_desc_t mq256;
        _MEM_MICROQ_ALIGN_16_CHECK((uint32_t)microq);
        _MEM_MICROQ_ZERO_OR_ONE_CHECK(entry_size);
        _MEM_MICROQ_ZERO_OR_ONE_CHECK(generate_event);
        _MEM_MICROQ_EVENT_SOURCE_CHECK(event_source);


        microq->desc.value = 0;
        microq->desc.event_source = event_source;
        microq->desc.generate_event = generate_event;
        microq->desc.entry_size = entry_size;

        {
            /* Read back */
            volatile mem_microq_desc_t mqrb;
            mqrb.value  = microq->desc.value;
        }
    }
    _INTRINSIC_END;
}

#endif /* __NFP_MEM_MICROQ_C__ */
