/*
 * Copyright (C) 2008-2014 Netronome Systems, Inc.  All rights reserved.
 */

#ifndef __NFP_CRYPTO_C__
#define __NFP_CRYPTO_C__

#include <nfp.h>
#include <nfp_intrinsic.h>
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

/* CRYPTO read */
__intrinsic
void cmd_crypto_read(
    __xread void *data,                     /* address to read into */
    uint32_t address,                       /* address to read from */
    uint32_t count,                         /* number of quadwords to read */
    sync_t sync,                            /* type of synchronization to use */
    SIGNAL *sig_ptr                         /* signal to raise upon completion */
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
                __asm crypto[read, *data, address, 0, __ct_const_val(count)], sig_done[*sig_ptr]
            }
            else
            {
                __asm crypto[read, *data, address, 0, __ct_const_val(count)], ctx_swap[*sig_ptr]
            }
        }
        else
        {
            if (!__is_ct_const(count))
            {
                CT_QPERFINFO_INDIRECT_REF(cmd_crypto_read);
            }
            {
                _INTRINSIC_IND_ALU_COUNT_MIN_MAX(count, 1, 16);
                if (sync == sig_done)
                {
                    __asm crypto[read, *data, address, 0, __ct_const_val(count)], sig_done[*sig_ptr], indirect_ref
                }
                else
                {
                    __asm crypto[read, *data, address, 0, __ct_const_val(count)], ctx_swap[*sig_ptr], indirect_ref
                }
            }
        }
    }
    _INTRINSIC_END;
}


/* CRYPTO write */
__intrinsic
void cmd_crypto_write(
    __xwrite void *data,                    /* data to write */
    uint32_t address,                       /* address to read from */
    uint32_t count,                         /* number of quadwords to write */
    sync_t sync,                            /* type of synchronization to use */
    SIGNAL *sig_ptr                         /* signal to raise upon completion */
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
                __asm crypto[write, *data, address, 0, __ct_const_val(count)], sig_done[*sig_ptr]
            }
            else
            {
                __asm crypto[write, *data, address, 0, __ct_const_val(count)], ctx_swap[*sig_ptr]
            }
        }
        else
        {
            if (!__is_ct_const(count))
            {
                CT_QPERFINFO_INDIRECT_REF(cmd_crypto_write);
            }
            {
                _INTRINSIC_IND_ALU_COUNT_MIN_MAX(count, 1, 16);
                if (sync == sig_done)
                {
                    __asm crypto[write, *data, address, 0, __ct_const_val(count)], sig_done[*sig_ptr], indirect_ref
                }
                else
                {
                    __asm crypto[write, *data, address, 0, __ct_const_val(count)], ctx_swap[*sig_ptr], indirect_ref
                }
            }
        }
    }
    _INTRINSIC_END;
}


/* CRYPTO write FIFO */

__intrinsic
void cmd_crypto_write_fifo(
    __xwrite void *data,                    /* data to write */
    uint32_t address,                       /* address to read from */
    uint32_t count,                         /* number of quadwords to write */
    sync_t sync,                            /* type of synchronization to use */
    SIGNAL *sig_ptr                         /* signal to raise upon completion */
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
                __asm crypto[write_fifo, *data, address, 0, __ct_const_val(count)], sig_done[*sig_ptr]
            }
            else
            {
                __asm crypto[write_fifo, *data, address, 0, __ct_const_val(count)], ctx_swap[*sig_ptr]
            }
        }
        else
        {
            if (!__is_ct_const(count))
            {
                CT_QPERFINFO_INDIRECT_REF(cmd_crypto_write);
            }

            {
                _INTRINSIC_IND_ALU_COUNT_MIN_MAX(count, 1, 16);
                if (sync == sig_done)
                {
                    __asm crypto[write_fifo, *data, address, 0, __ct_const_val(count)], sig_done[*sig_ptr], indirect_ref
                }
                else
                {
                    __asm crypto[write_fifo, *data, address, 0, __ct_const_val(count)], ctx_swap[*sig_ptr], indirect_ref
                }
            }
        }
    }
    _INTRINSIC_END;
}


/* CRYPTO read indirect */
__intrinsic
void cmd_crypto_read_ind(
    __xread void *data,                     /* address to read into */
    uint32_t address,                       /* address to read from */
    uint32_t max_nn,                        /* max. number of quadwords to read */
    generic_ind_t ind,                      /* indirect word */
    sync_t sync,                            /* type of synchronization to use */
    SIGNAL *sig_ptr                         /* signal to raise upon completion */
)
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_read_reg(data));
        CT_ASSERT(__is_ct_const(max_nn));
        CT_ASSERT(max_nn >= 1 && max_nn <= 16);
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done || sync == ctx_swap);
        if (sync == sig_done)
        {
            __asm local_csr_wr[CMD_INDIRECT_REF_0, CSR_INDIRECT_TYPE(ind)]
            __asm alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
            __asm crypto[read, *data, address, 0, __ct_const_val(max_nn)], sig_done[*sig_ptr], indirect_ref
        }
        else
        {
                __asm local_csr_wr[CMD_INDIRECT_REF_0, CSR_INDIRECT_TYPE(ind)]
                __asm alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
                __asm crypto[read, *data, address, 0, __ct_const_val(max_nn)], ctx_swap[*sig_ptr], indirect_ref
        }
    }
    _INTRINSIC_END;
}


/* CRYPTO write indirect */
__intrinsic
void cmd_crypto_write_ind(
    __xwrite void *data,                    /* data to write */
    uint32_t address,                       /* address to read from */
    uint32_t max_nn,                        /* max. number of quadwords to write */
    generic_ind_t ind,                      /* indirect word */
    sync_t sync,                            /* type of synchronization to use */
    SIGNAL *sig_ptr                         /* signal to raise upon completion */
)
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_write_reg(data));
        CT_ASSERT(__is_ct_const(max_nn));
        CT_ASSERT(max_nn >= 1 && max_nn <= 16);
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done || sync == ctx_swap);
        if (sync == sig_done)
        {
            __asm local_csr_wr[CMD_INDIRECT_REF_0, CSR_INDIRECT_TYPE(ind)]
            __asm alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
            __asm crypto[write, *data, address, 0, __ct_const_val(max_nn)], sig_done[*sig_ptr], indirect_ref
        }
        else
        {
                __asm local_csr_wr[CMD_INDIRECT_REF_0, CSR_INDIRECT_TYPE(ind)]
                __asm alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
                __asm crypto[write, *data, address, 0, __ct_const_val(max_nn)], ctx_swap[*sig_ptr], indirect_ref
        }
    }
    _INTRINSIC_END;
}

/* CRYPTO write FIFO indirect */
__intrinsic
void cmd_crypto_write_fifo_ind(
    __xwrite void *data,                    /* data to write */
    uint32_t address,                       /* address to read from */
    uint32_t max_nn,                        /* max. number of quadwords to write */
    generic_ind_t ind,                      /* indirect word */
    sync_t sync,                            /* type of synchronization to use */
    SIGNAL *sig_ptr                         /* signal to raise upon completion */
)
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_write_reg(data));
        CT_ASSERT(__is_ct_const(max_nn));
        CT_ASSERT(max_nn >= 1 && max_nn <= 16);
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done || sync == ctx_swap);
        if (sync == sig_done)
        {
            __asm local_csr_wr[CMD_INDIRECT_REF_0, CSR_INDIRECT_TYPE(ind)]
            __asm alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
            __asm crypto[write_fifo, *data, address, 0, __ct_const_val(max_nn)], sig_done[*sig_ptr], indirect_ref
        }
        else
        {
                __asm local_csr_wr[CMD_INDIRECT_REF_0, CSR_INDIRECT_TYPE(ind)]

                __asm alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
                __asm crypto[write_fifo, *data, address, 0, __ct_const_val(max_nn)], ctx_swap[*sig_ptr], indirect_ref
        }
    }
    _INTRINSIC_END;
}


#endif /* __NFP_CRYPTO_C__ */
