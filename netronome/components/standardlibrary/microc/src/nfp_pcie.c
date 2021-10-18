/*
 * Copyright (C) 2008-2014 Netronome Systems, Inc.  All rights reserved.
 */

#ifndef __NFP_PCIE_C__
#define __NFP_PCIE_C__

#include <nfp.h>
#include <nfp_intrinsic.h>
#include <nfp_pcie.h>

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

#define _PCIe_REQUESTER_ID_CHECK(rid)                                   \
        if (__is_ct_const(rid))                                         \
        {                                                               \
            CT_ASSERT(rid <= 0xff);                                     \
        }                                                               \
        else                                                            \
        {                                                               \
            RT_RANGE_ASSERT(rid <= 0xff);                               \
        }


/* PCIe read, 40-bit addr */
__intrinsic
void cmd_pcie_read_ptr40(
    __xread void *data,                         /* xfer reg to read into */
    volatile void __addr40 *address,            /* address to read from (aligned on 4-byte boundary) */
    uint32_t count,                         /* number of longwords to read */
    sync_t sync,                                /* type of synchronization to use */
    SIGNAL *sig_ptr                             /* signal to raise upon completion */
    )
{
    _INTRINSIC_BEGIN;
    {
        uint32_t hi_addr, low_addr;

        _INTRINSIC_CONVERT_HI_LO_ADDRESS(address);
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done || sync == ctx_swap);
        if (__is_ct_const(count) && count <= 8)
        {
            CT_ASSERT(count != 0);
            if (sync == sig_done)
            {
                __asm pcie[read, *data, hi_addr, <<8, low_addr, __ct_const_val(count)], sig_done[*sig_ptr]
            }
            else
            {
                __asm pcie[read, *data, hi_addr, <<8, low_addr, __ct_const_val(count)], ctx_swap[*sig_ptr]
            }
        }
        else
        {
            _INTRINSIC_IND_ALU_COUNT_MIN_MAX(count, 1, 32);
            if (sync == sig_done)
            {
                __asm pcie[read, *data, hi_addr, <<8, low_addr, __ct_const_val(count)], sig_done[*sig_ptr], indirect_ref
            }
            else
            {
                __asm pcie[read, *data, hi_addr, <<8, low_addr, __ct_const_val(count)], ctx_swap[*sig_ptr], indirect_ref
            }
        }
    }
    _INTRINSIC_END;
}


/* PCIe write, 40-bit addr */
__intrinsic
void cmd_pcie_write_ptr40(
    __xwrite void *data,                        /* xfer reg data to write */
    volatile void __addr40 *address,            /* address to write (aligned on 4-byte boundary) */
    uint32_t count,                         /* number of longwords to write */
    sync_t sync,                                /* type of synchronization to use */
    SIGNAL *sig_ptr                             /* signal to raise upon completion */
    )
{
   _INTRINSIC_BEGIN;
   {
        uint32_t hi_addr, low_addr;

        _INTRINSIC_CONVERT_HI_LO_ADDRESS(address);
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done || sync == ctx_swap);

        if (__is_ct_const(count) && count <= 8)
        {
            CT_ASSERT(count != 0);
            if (sync == sig_done)
            {
                __asm pcie[write, *data, hi_addr, <<8, low_addr, __ct_const_val(count)], sig_done[*sig_ptr]
            }
            else
            {
                __asm pcie[write, *data, hi_addr, <<8, low_addr, __ct_const_val(count)], ctx_swap[*sig_ptr]
            }
        }
        else
        {
            _INTRINSIC_IND_ALU_COUNT_MIN_MAX(count, 1, 32);
            if (sync == sig_done)
            {
                __asm pcie[write, *data, hi_addr, <<8, low_addr, __ct_const_val(count)], sig_done[*sig_ptr], indirect_ref
            }
            else
            {
                __asm pcie[write, *data, hi_addr, <<8, low_addr, __ct_const_val(count)], ctx_swap[*sig_ptr], indirect_ref
            }
        }
    }
    _INTRINSIC_END;
}


/* PCIe read internal target, 40-bit addr */
__intrinsic
void cmd_pcie_read_pci_ptr40(
    __xread void *data,                         /* xfer reg to read into */
    volatile void __addr40 *address,            /* address, [9:16] target number, [15:0] target addr */
    uint32_t count,                         /* number of longwords to read */
    sync_t sync,                                /* type of synchronization to use */
    SIGNAL *sig_ptr                             /* signal to raise upon completion */
    )
{
    _INTRINSIC_BEGIN;
    {
        uint32_t hi_addr, low_addr;

        _INTRINSIC_CONVERT_HI_LO_ADDRESS(address);
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done || sync == ctx_swap);
        if (__is_ct_const(count) && count <= 8)
        {
            CT_ASSERT(count != 0);
            if (sync == sig_done)
            {
                __asm pcie[read_int, *data, hi_addr, <<8, low_addr, __ct_const_val(count)], sig_done[*sig_ptr]
            }
            else
            {
                __asm pcie[read_int, *data, hi_addr, <<8, low_addr, __ct_const_val(count)], ctx_swap[*sig_ptr]
            }
        }
        else
        {
            _INTRINSIC_IND_ALU_COUNT_MIN_MAX(count, 1, 32);
            if (sync == sig_done)
            {
                __asm pcie[read_int, *data, hi_addr, <<8, low_addr, __ct_const_val(count)], sig_done[*sig_ptr], indirect_ref
            }
            else
            {
                __asm pcie[read_int, *data, hi_addr, <<8, low_addr, __ct_const_val(count)], ctx_swap[*sig_ptr], indirect_ref
            }
        }
    }
    _INTRINSIC_END;
}


/* PCIe write internal target, 40-bit addr */
__intrinsic
void cmd_pcie_write_pci_ptr40(
    __xwrite void *data,                        /* xfer reg data to write */
    volatile void __addr40 *address,            /* address, [9:16] target number, [15:0] target addr */
    uint32_t count,                         /* number of longwords to write */
    sync_t sync,                                /* type of synchronization to use */
    SIGNAL *sig_ptr                             /* signal to raise upon completion */
    )
{
   _INTRINSIC_BEGIN;
   {
        uint32_t hi_addr, low_addr;

        _INTRINSIC_CONVERT_HI_LO_ADDRESS(address);
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done || sync == ctx_swap);

        if (__is_ct_const(count) && count <= 8)
        {
            CT_ASSERT(count != 0);
            if (sync == sig_done)
            {
                __asm pcie[write_int, *data, hi_addr, <<8, low_addr, __ct_const_val(count)], sig_done[*sig_ptr]
            }
            else
            {
                __asm pcie[write_int, *data, hi_addr, <<8, low_addr, __ct_const_val(count)], ctx_swap[*sig_ptr]
            }
        }
        else
        {
            _INTRINSIC_IND_ALU_COUNT_MIN_MAX(count, 1, 32);
            if (sync == sig_done)
            {
                __asm pcie[write_int, *data, hi_addr, <<8, low_addr, __ct_const_val(count)], sig_done[*sig_ptr], indirect_ref
            }
            else
            {
                __asm pcie[write_int, *data, hi_addr, <<8, low_addr, __ct_const_val(count)], ctx_swap[*sig_ptr], indirect_ref
            }
        }
    }
    _INTRINSIC_END;
}


/* PCIe read  Requester ID, ptr40 */
__intrinsic
void cmd_pcie_read_rid_ptr40(
    __xread void *data,                         /* xfer reg to read into */
    uint32_t requester_id,                  /* PCIe requester id to use for transaction */
    volatile void __addr40 *address,            /* address to read (aligned on 4-byte boundary) */
    uint32_t count,                         /* number of longwords to read */
    sync_t sync,                                /* type of synchronization to use */
    SIGNAL *sig_ptr                             /* signal to raise upon completion */
    )
{
    _INTRINSIC_BEGIN;
    {

        uint32_t hi_addr, low_addr;

        _INTRINSIC_CONVERT_HI_LO_ADDRESS(address);
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done || sync == ctx_swap);
        CT_ASSERT(__is_read_reg(data));

        _PCIe_REQUESTER_ID_CHECK(requester_id);

        if (__is_ct_const(count) && count <= 8)
        {
            CT_ASSERT(count != 0);

            if (sync == sig_done)
            {
                _INTRINSIC_IND_ALU_MASK_MAX(requester_id, 0xff);
                __asm pcie[read_rid, *data, hi_addr, <<8, low_addr, __ct_const_val(count)], sig_done[*sig_ptr], indirect_ref
            }
            else
            {
                _INTRINSIC_IND_ALU_MASK_MAX(requester_id, 0xff);
                __asm pcie[read_rid, *data, hi_addr, <<8, low_addr, __ct_const_val(count)], ctx_swap[*sig_ptr], indirect_ref
            }
        }
        else
        {
            if (!__is_ct_const(count))
            {
                CT_QPERFINFO_INDIRECT_REF(pcie_read_rid);
            }
            {
                _INTRINSIC_IND_ALU_COUNT_MIN_MAX_MASK_MAX(count,1,32, requester_id, 0xff);
                if (sync == sig_done)
                {
                    __asm pcie[read_rid, *data, hi_addr, <<8, low_addr, __ct_const_val(count)], sig_done[*sig_ptr], indirect_ref
                }
                else
                {
                    __asm pcie[read_rid, *data, hi_addr, <<8, low_addr, __ct_const_val(count)], ctx_swap[*sig_ptr], indirect_ref
                }
            }
        }

    }
    _INTRINSIC_END;
}


/* PCIe write Requester ID, ptr40 */
__intrinsic
void cmd_pcie_write_rid_ptr40(
    __xwrite void *data,                        /* data to write */
    uint32_t requester_id,                  /* PCIe requester id to use for transaction */
    volatile void __addr40 *address,            /* address to write (aligned on 4-byte boundary) */
    uint32_t count,                         /* number of longwords to write */
    sync_t sync,                                /* type of synchronization to use */
    SIGNAL *sig_ptr                             /* signal to raise upon completion */
    )
{
    _INTRINSIC_BEGIN;
    {
        uint32_t hi_addr, low_addr;
        _INTRINSIC_CONVERT_HI_LO_ADDRESS(address);
        CT_ASSERT(__is_write_reg(data));
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done || sync == ctx_swap);
        _PCIe_REQUESTER_ID_CHECK(requester_id);
        if (__is_ct_const(count) && count <= 8)
        {

            CT_ASSERT(count != 0);

            if (sync == sig_done)
            {
                _INTRINSIC_IND_ALU_MASK_MAX(requester_id, 0xff);
                __asm pcie[write_rid, *data, hi_addr, <<8, low_addr, __ct_const_val(count)], sig_done[*sig_ptr], indirect_ref
            }
            else
            {
                _INTRINSIC_IND_ALU_MASK_MAX(requester_id, 0xff);
                __asm pcie[write_rid, *data, hi_addr, <<8, low_addr, __ct_const_val(count)], ctx_swap[*sig_ptr], indirect_ref
            }
        }
        else
        {
            if (!__is_ct_const(count))
            {
                CT_QPERFINFO_INDIRECT_REF(pcie_write_rid);
            }
            {
                _INTRINSIC_IND_ALU_COUNT_MIN_MAX_MASK_MAX(count,1,32, requester_id, 0xff);
                if (sync == sig_done)
                {
                    __asm pcie[write_rid, *data, hi_addr, <<8, low_addr, __ct_const_val(count)], sig_done[*sig_ptr], indirect_ref
                }
                else
                {
                    __asm pcie[write_rid, *data, hi_addr, <<8, low_addr, __ct_const_val(count)], ctx_swap[*sig_ptr], indirect_ref
                }
            }
        }
    }
    _INTRINSIC_END;
}

/* PCIe read */
void cmd_pcie_read_ptr32(
    __xread void *data,                         /* address to read into */
    volatile void *address,                     /* address to read from (aligned on 4-byte boundary) */
    uint32_t count,                             /* number of longwords to read */
    sync_t sync,                                /* type of synchronization to use */
    SIGNAL *sig_ptr                             /* signal to raise upon completion */
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
                __asm pcie[read, *data, address, 0, __ct_const_val(count)], sig_done[*sig_ptr]
            }
            else
            {
                __asm pcie[read, *data, address, 0, __ct_const_val(count)], ctx_swap[*sig_ptr]
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
                CT_QPERFINFO_INDIRECT_REF(cmd_pcie_read);
            }

            {


                generic_ind_t ind;

                _INTRINSIC_OVERRIDE_LENGTH_AND_BYTEMASK_IN_ALU(ind, (count - 1), 0xff);

                if (sync == sig_done)
                {
                    __asm
                    {
                        alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
                        pcie[read, *data, address, 0, __ct_const_val(count)], sig_done[*sig_ptr], indirect_ref
                    }
                }
                else
                {
                    __asm
                    {
                        alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
                        pcie[read, *data, address, 0, __ct_const_val(count)], ctx_swap[*sig_ptr], indirect_ref
                    }

                }

            }
        }
    }

    _INTRINSIC_END;
}


/* PCIe read indirect */
void cmd_pcie_read_ind_ptr32(
    __xread void *data,                         /* address to read into */
    volatile void *address,                     /* address to read from (aligned on 4-byte boundary) */
    uint32_t max_nn,                            /* max. number of longwords to read */
    generic_ind_t ind,                          /* indirect word */
    sync_t sync,                                /* type of synchronization to use */
    SIGNAL *sig_ptr                             /* signal to raise upon completion */
    )
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_read_reg(data));
        CT_ASSERT(__is_ct_const(max_nn));
        CT_ASSERT(max_nn >= 1 && max_nn <= 32);
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done || sync == ctx_swap);

         __asm
        {
            local_csr_wr[CMD_INDIRECT_REF_0, CSR_INDIRECT_TYPE(ind)]
            alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
        }

        if (sync == sig_done)
        {
            __asm pcie[read, *data, address, 0, __ct_const_val(max_nn)], sig_done[*sig_ptr], indirect_ref
        }
        else
        {
            __asm pcie[read, *data, address, 0, __ct_const_val(max_nn)], ctx_swap[*sig_ptr], indirect_ref
        }
    }
    _INTRINSIC_END;
}


/* PCIe write */
void cmd_pcie_write_ptr32(
    __xwrite void *data,                        /* data to write */
    volatile void *address,                     /* address to write (aligned on 4-byte boundary) */
    uint32_t count,                             /* number of longwords to write */
    sync_t sync,                                /* type of synchronization to use */
    SIGNAL *sig_ptr                             /* signal to raise upon completion */
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
                __asm pcie[write, *data, address, 0, __ct_const_val(count)], sig_done[*sig_ptr]
            }
            else
            {
                __asm pcie[write, *data, address, 0, __ct_const_val(count)], ctx_swap[*sig_ptr]
            }
        }
        else
        {
            if (!__is_ct_const(count))
            {
                CT_QPERFINFO_INDIRECT_REF(cmd_pcie_write);
            }
            {
                _INTRINSIC_IND_ALU_COUNT_MIN_MAX(count,1,32);
                if (sync == sig_done)
                {
                    __asm pcie[write, *data, address, 0, __ct_const_val(count)], sig_done[*sig_ptr], indirect_ref
                }
                else
                {
                    __asm pcie[write, *data, address, 0, __ct_const_val(count)], ctx_swap[*sig_ptr], indirect_ref
                }
            }
        }
    }
    _INTRINSIC_END;
}


/* PCIe write indirect */
void cmd_pcie_write_ind_ptr32(
    __xwrite void *data,                        /* data to write */
    volatile void *address,                     /* address to write (aligned on 4-byte boundary) */
    uint32_t max_nn,                            /* max. number of longwords to write */
    generic_ind_t ind,                          /* indirect word */
    sync_t sync,                                /* type of synchronization to use */
    SIGNAL *sig_ptr                             /* signal to raise upon completion */
    )
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_write_reg(data));
        CT_ASSERT(__is_ct_const(max_nn));
        CT_ASSERT(max_nn >= 1 && max_nn <= 32);
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done || sync == ctx_swap);

        __asm
        {
            local_csr_wr[CMD_INDIRECT_REF_0, CSR_INDIRECT_TYPE(ind)]
            alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
        }
        if (sync == sig_done)
        {
            __asm pcie[write, *data, address, 0, __ct_const_val(max_nn)], sig_done[*sig_ptr], indirect_ref
        }
        else
        {
            __asm pcie[write, *data, address, 0, __ct_const_val(max_nn)], ctx_swap[*sig_ptr], indirect_ref
        }
    }
    _INTRINSIC_END;
}


/* PCIe internal read */
void cmd_pcie_read_pci_ptr32(
    __xread void *data,                         /* address to read into */
    volatile void *address,                     /* address to read from */
    uint32_t count,                             /* number of longwords to read */
    sync_t sync,                                /* type of synchronization to use */
    SIGNAL *sig_ptr                             /* signal to raise upon completion */
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
                __asm pcie[read_int, *data, address, 0, __ct_const_val(count)], sig_done[*sig_ptr]
            }
            else
            {
                __asm pcie[read_int, *data, address, 0, __ct_const_val(count)], ctx_swap[*sig_ptr]
            }
        }
        else
        {
            if (!__is_ct_const(count))
            {
                CT_QPERFINFO_INDIRECT_REF(cmd_pcie_read_pci);
            }
            {
                _INTRINSIC_IND_ALU_COUNT_MIN_MAX(count,1,16);
                if (sync == sig_done)
                {
                    __asm pcie[read_int, *data, address, 0, __ct_const_val(count)], sig_done[*sig_ptr], indirect_ref
                }
                else
                {
                    __asm pcie[read_int, *data, address, 0, __ct_const_val(count)], ctx_swap[*sig_ptr], indirect_ref
                }
            }
        }
    }
    _INTRINSIC_END;
}


/* PCIe internal read, indirect */
void cmd_pcie_read_pci_ind_ptr32(
    __xread void *data,                         /* address to read into */
    volatile void *address,                     /* address to read from */
    uint32_t max_nn,                            /* max. number of longwords to read */
    generic_ind_t ind,                          /* indirect word */
    sync_t sync,                                /* type of synchronization to use */
    SIGNAL *sig_ptr                             /* signal to raise upon completion */
    )
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_read_reg(data));
        CT_ASSERT(__is_ct_const(max_nn));
        CT_ASSERT(max_nn >= 1 && max_nn <= 16);
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done || sync == ctx_swap);

        __asm
        {
            local_csr_wr[CMD_INDIRECT_REF_0, CSR_INDIRECT_TYPE(ind)]
            alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
        }

        if (sync == sig_done)
        {
            __asm pcie[read_int, *data, address, 0, __ct_const_val(max_nn)], sig_done[*sig_ptr], indirect_ref
        }
        else
        {
            __asm pcie[read_int, *data, address, 0, __ct_const_val(max_nn)], ctx_swap[*sig_ptr], indirect_ref
        }
    }
    _INTRINSIC_END;
}


/* PCIe internal write */
void cmd_pcie_write_pci_ptr32(
    __xwrite void *data,                        /* data to write */
    volatile void *address,                     /* address to write (aligned on 4-byte boundary) */
    uint32_t count,                             /* number of longwords to write */
    sync_t sync,                                /* type of synchronization to use */
    SIGNAL *sig_ptr                             /* signal to raise upon completion */
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
                __asm pcie[write_int, *data, address, 0, __ct_const_val(count)], sig_done[*sig_ptr]
            }
            else
            {
                __asm pcie[write_int, *data, address, 0, __ct_const_val(count)], ctx_swap[*sig_ptr]
            }
        }
        else
        {
            if (!__is_ct_const(count))
            {
                CT_QPERFINFO_INDIRECT_REF(cmd_pcie_write_pci);
            }
            {
                _INTRINSIC_IND_ALU_COUNT_MIN_MAX(count,1,16);
                if (sync == sig_done)
                {
                    __asm pcie[write_int, *data, address, 0, __ct_const_val(count)], sig_done[*sig_ptr], indirect_ref
                }
                else
                {
                    __asm pcie[write_int, *data, address, 0, __ct_const_val(count)], ctx_swap[*sig_ptr], indirect_ref
                }
            }
        }
    }
    _INTRINSIC_END;
}


/* PCIe write internal, indirect */
void cmd_pcie_write_pci_ind_ptr32(
    __xwrite void *data,                        /* data to write */
    volatile void *address,                     /* address to write */
    uint32_t max_nn,                            /* max. number of longwords to write */
    generic_ind_t ind,                          /* indirect word */
    sync_t sync,                                /* type of synchronization to use */
    SIGNAL *sig_ptr                             /* signal to raise upon completion */
    )
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_write_reg(data));
        CT_ASSERT(__is_ct_const(max_nn));
        CT_ASSERT(max_nn >= 1 && max_nn <= 16);
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done || sync == ctx_swap);

        __asm
        {
            local_csr_wr[CMD_INDIRECT_REF_0, CSR_INDIRECT_TYPE(ind)]
            alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
        }

        if (sync == sig_done)
        {
            __asm pcie[write_int, *data, address, 0, __ct_const_val(max_nn)], sig_done[*sig_ptr], indirect_ref
        }
        else
        {
            __asm pcie[write_int, *data, address, 0, __ct_const_val(max_nn)], ctx_swap[*sig_ptr], indirect_ref
        }
    }
    _INTRINSIC_END;
}


/* PCIe read  Requester ID*/
void cmd_pcie_read_rid_ptr32(
    __xread void *data,                         /* address to read into */
    uint32_t requester_id,                      /* PCIe requester id to use for transaction */
    volatile void *address,                     /* address to read (aligned on 4-byte boundary) */
    uint32_t count,                             /* number of longwords to read */
    sync_t sync,                                /* type of synchronization to use */
    SIGNAL *sig_ptr                             /* signal to raise upon completion */
    )
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done || sync == ctx_swap);
        CT_ASSERT(__is_read_reg(data));

        _PCIe_REQUESTER_ID_CHECK(requester_id);

        if (__is_ct_const(count) && count <= 8)
        {
            CT_ASSERT(count != 0);

            if (sync == sig_done)
            {
                _INTRINSIC_IND_ALU_MASK_MAX(requester_id, 0xff);
                __asm pcie[read_rid, *data, address, 0, __ct_const_val(count)], sig_done[*sig_ptr], indirect_ref
            }
            else
            {
                _INTRINSIC_IND_ALU_MASK_MAX(requester_id, 0xff);
                __asm pcie[read_rid, *data, address, 0, __ct_const_val(count)], ctx_swap[*sig_ptr], indirect_ref
            }
        }
        else
        {
            if (!__is_ct_const(count))
            {
                CT_QPERFINFO_INDIRECT_REF(cmd_pcie_read_rid);
            }
            {
                _INTRINSIC_IND_ALU_COUNT_MIN_MAX_MASK_MAX(count,1,32, requester_id, 0xff);
                if (sync == sig_done)
                {
                    __asm pcie[read_rid, *data, address, 0, __ct_const_val(count)], sig_done[*sig_ptr], indirect_ref
                }
                else
                {
                    __asm pcie[read_rid, *data, address, 0, __ct_const_val(count)], ctx_swap[*sig_ptr], indirect_ref
                }
            }
        }

    }
    _INTRINSIC_END;
}


/* PCIe read Requester ID, indirect*/
void cmd_pcie_read_rid_ind_ptr32(
    __xread void *data,                         /* address to read into */
    volatile void *address,                     /* address to read (aligned on 4-byte boundary) */
    uint32_t max_nn,                            /* number of longwords to read */
    generic_ind_t ind,                          /* indirect word */
    sync_t sync,                                /* type of synchronization to use */
    SIGNAL *sig_ptr                             /* signal to raise upon completion */
    )
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_read_reg(data));
        CT_ASSERT(__is_ct_const(max_nn));
        CT_ASSERT(max_nn >= 1 && max_nn <= 32);
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done || sync == ctx_swap);

        __asm
        {
            local_csr_wr[CMD_INDIRECT_REF_0, CSR_INDIRECT_TYPE(ind)]
            alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
        }

        if (sync == sig_done)
        {
            __asm pcie[read_rid, *data, address, 0, __ct_const_val(max_nn)], sig_done[*sig_ptr], indirect_ref
        }
        else
        {
            __asm pcie[read_rid, *data, address, 0, __ct_const_val(max_nn)], ctx_swap[*sig_ptr], indirect_ref
        }
    }
    _INTRINSIC_END;
}


/* PCIe write Requester ID */
void cmd_pcie_write_rid_ptr32(
    __xwrite void *data,                        /* data to write */
    uint32_t requester_id,                      /* PCIe requester id to use for transaction */
    volatile void *address,                     /* address to write (aligned on 4-byte boundary) */
    uint32_t count,                             /* number of longwords to write */
    sync_t sync,                                /* type of synchronization to use */
    SIGNAL *sig_ptr                             /* signal to raise upon completion */
    )
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_write_reg(data));
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done || sync == ctx_swap);

        _PCIe_REQUESTER_ID_CHECK(requester_id);

        if (__is_ct_const(count) && count <= 8)
        {

            CT_ASSERT(count != 0);

            if (sync == sig_done)
            {
                _INTRINSIC_IND_ALU_MASK_MAX(requester_id, 0xff);
                __asm pcie[write_rid, *data, address, 0, __ct_const_val(count)], sig_done[*sig_ptr], indirect_ref
            }
            else
            {
                _INTRINSIC_IND_ALU_MASK_MAX(requester_id, 0xff);
                __asm pcie[write_rid, *data, address, 0, __ct_const_val(count)], ctx_swap[*sig_ptr], indirect_ref
            }
        }
        else
        {
            if (!__is_ct_const(count))
            {
                CT_QPERFINFO_INDIRECT_REF(cmd_pcie_write_rid);
            }
            {
                _INTRINSIC_IND_ALU_COUNT_MIN_MAX_MASK_MAX(count,1,32, requester_id, 0xff);
                if (sync == sig_done)
                {
                    __asm pcie[write_rid, *data, address, 0, __ct_const_val(count)], sig_done[*sig_ptr], indirect_ref
                }
                else
                {
                    __asm pcie[write_rid, *data, address, 0, __ct_const_val(count)], ctx_swap[*sig_ptr], indirect_ref
                }
            }
        }

    }
    _INTRINSIC_END;
}


/* PCIe write Requester ID, indirect*/
void cmd_pcie_write_rid_ind_ptr32(
    __xwrite void *data,                        /* data to write */
    volatile void *address,                     /* address to write (aligned on 4-byte boundary) */
    uint32_t max_nn,                            /* number of longwords to write */
    generic_ind_t ind,                          /* indirect word */
    sync_t sync,                                /* type of synchronization to use */
    SIGNAL *sig_ptr                             /* signal to raise upon completion */
    )
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_write_reg(data));
        CT_ASSERT(__is_ct_const(max_nn));
        CT_ASSERT(max_nn >= 1 && max_nn <= 32);
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done || sync == ctx_swap);

        __asm
        {

            local_csr_wr[CMD_INDIRECT_REF_0, CSR_INDIRECT_TYPE(ind)]

            alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
        }

        if (sync == sig_done)
        {
            __asm pcie[write_rid, *data, address, 0, __ct_const_val(max_nn)], sig_done[*sig_ptr], indirect_ref
        }
        else
        {
            __asm pcie[write_rid, *data, address, 0, __ct_const_val(max_nn)], ctx_swap[*sig_ptr], indirect_ref
        }
    }
    _INTRINSIC_END;
}

#endif /* __NFP_PCIE_C__ */
