/*
 * Copyright (C) 2008-2014 Netronome Systems, Inc.  All rights reserved.
 */

#ifndef __NFP_MEM_TICKET_C__
#define __NFP_MEM_TICKET_C__

#include <nfp.h>
#include <nfp_intrinsic.h>
#include <nfp_mem_ticket.h>

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

__intrinsic
void cmd_mem_ticket_line_init_ptr32(
        mem_ticket_line_t *mem_ticket_line,
        volatile void  __addr32 __mem *address,
        sync_t sync,
        SIGNAL *sig_ptr
    )
{
    __xwrite mem_ticket_line_t dwr_ticket_line;
    CT_ASSERT(__is_ct_const(sync));
    CT_ASSERT(sync == sig_done);

    dwr_ticket_line=*mem_ticket_line;
    _INTRINSIC_BEGIN;
    {
        __asm
        {
            mem[atomic_write, dwr_ticket_line, 0, address, 4], sig_done[*sig_ptr]
        }
    }
    _INTRINSIC_END;
}

__intrinsic
void cmd_mem_ticket_line_init_ptr40(
        mem_ticket_line_t *mem_ticket_line,
        volatile void __addr40 __mem *address,
        sync_t sync,
        SIGNAL *sig_ptr
    )
{
    __xwrite mem_ticket_line_t dwr_ticket_line;
    CT_ASSERT(__is_ct_const(sync));
    CT_ASSERT(sync == sig_done);

    dwr_ticket_line=*mem_ticket_line;
    _INTRINSIC_BEGIN;
    {
        uint32_t hi_addr, low_addr;
        _INTRINSIC_CONVERT_HI_LO_ADDRESS(address);
        __asm
        {
            mem[atomic_write,dwr_ticket_line,hi_addr,<<8,low_addr,4], sig_done[*sig_ptr]
        }
    }
    _INTRINSIC_END;
}

__intrinsic
void cmd_mem_ticket_line_push_init_ptr32(
        mem_ticket_line_push_t *mem_ticket_line_push,
        volatile void  __addr32 __mem *address,
        sync_t sync,
        SIGNAL *sig_ptr
    )
{
    __xwrite mem_ticket_line_push_t dwr_ticket_line;
    CT_ASSERT(__is_ct_const(sync));
    CT_ASSERT(sync == sig_done);

    dwr_ticket_line=*mem_ticket_line_push;
    _INTRINSIC_BEGIN;
    {
        __asm
        {
            mem[atomic_write, dwr_ticket_line, 0, address, 4], sig_done[*sig_ptr]
        }
    }
    _INTRINSIC_END;
}

__intrinsic
void cmd_mem_ticket_line_push_init_ptr40(
        mem_ticket_line_push_t *mem_ticket_line_push,
        volatile void __addr40 __mem *address,
        sync_t sync,
        SIGNAL *sig_ptr
    )
{
    __xwrite mem_ticket_line_push_t dwr_ticket_line;
    CT_ASSERT(__is_ct_const(sync));
    CT_ASSERT(sync == sig_done);

    dwr_ticket_line=*mem_ticket_line_push;
    _INTRINSIC_BEGIN;
    {
        uint32_t hi_addr, low_addr;
        _INTRINSIC_CONVERT_HI_LO_ADDRESS(address);
        __asm
        {
            mem[atomic_write,dwr_ticket_line,hi_addr,<<8,low_addr,4], sig_done[*sig_ptr]
        }
    }
    _INTRINSIC_END;
}

__intrinsic
void cmd_mem_ticket_release_ptr32(
        __xrw void *xfer,
        volatile void  __addr32 __mem *address,
        sync_t sync,
        SIGNAL_PAIR *sig_pair_ptr
    )
{
    CT_ASSERT(__is_read_write_reg(xfer));
    CT_ASSERT(__is_ct_const(sync));
    CT_ASSERT(sync == sig_done);

    _INTRINSIC_BEGIN;
    {
        __asm
        {
            mem[release_ticket, *xfer, address, 0], sig_done[*sig_pair_ptr]
        }
    }
    _INTRINSIC_END;
}

__intrinsic
void cmd_mem_ticket_release_ptr40(
        __xrw void *xfer,
        volatile void __addr40 __mem *address,
        sync_t sync,
        SIGNAL_PAIR *sig_pair_ptr
    )
{
    CT_ASSERT(__is_read_write_reg(xfer));
    CT_ASSERT(__is_ct_const(sync));
    CT_ASSERT(sync == sig_done);

    _INTRINSIC_BEGIN;
    {
        uint32_t hi_addr, low_addr;
    _INTRINSIC_CONVERT_HI_LO_ADDRESS(address);
        __asm
        {
            mem[release_ticket,*xfer,hi_addr,<<8,low_addr], sig_done[*sig_pair_ptr]
        }
    }
    _INTRINSIC_END;
}

__intrinsic
void cmd_mem_ticket_release_push_ptr32(
        __xrw void *xfer,
        volatile void  __addr32 __mem *address,
        sync_t sync,
        SIGNAL *sig_ptr
    )
{
    CT_ASSERT(__is_read_write_reg(xfer));
    CT_ASSERT(__is_ct_const(sync));

    _INTRINSIC_BEGIN;
    {
        if (sync == sig_done)
        {
            __asm
            {
                mem[release_ticket_ind, *xfer, address, 0], sig_done[*sig_ptr]
            }
        }
        else
        {
            __asm
            {
                mem[release_ticket_ind, *xfer, address, 0], ctx_swap[*sig_ptr]
            }
        }
    }
    _INTRINSIC_END;
}

__intrinsic
void cmd_mem_ticket_release_push_ptr40(
        __xrw void *xfer,
        volatile void __addr40 __mem *address,
        sync_t sync,
        SIGNAL *sig_ptr
    )
{
    CT_ASSERT(__is_read_write_reg(xfer));
    CT_ASSERT(__is_ct_const(sync));

    _INTRINSIC_BEGIN;
    {
        uint32_t hi_addr, low_addr;
    _INTRINSIC_CONVERT_HI_LO_ADDRESS(address);
        if (sync == sig_done)
        {
            __asm
            {
                mem[release_ticket_ind,*xfer,hi_addr,<<8,low_addr], sig_done[*sig_ptr]
            }
        }
        else
        {
            __asm
            {
                mem[release_ticket_ind,*xfer,hi_addr,<<8,low_addr], ctx_swap[*sig_ptr]
            }
        }
    }
    _INTRINSIC_END;
}

#endif /* __NFP_MEM_TICKET_C__ */
