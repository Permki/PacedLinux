/*
 * Copyright (C) 2008-2014 Netronome Systems, Inc.  All rights reserved.
 */
#ifndef __NFP_OVERRIDE_C__
#define __NFP_OVERRIDE_C__

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

#pragma diag_suppress 279  /* ignore while(0) etc in macros */

#define _OVR_LIMIT_CHECK(_value, _min, _max)                            \
do                                                                      \
{                                                                       \
    if (__is_ct_const(_value))                                          \
    {                                                                   \
        CT_ASSERT(((_value) >= (_min)) && ((_value) <= (_max)));        \
    }                                                                   \
    else                                                                \
    {                                                                   \
        RT_RANGE_ASSERT(((_value) >= (_min)) && ((_value) <= (_max)));  \
    }                                                                   \
} while(0)


#define _OVR_ERROR(f, s)                                                \
do                                                                      \
{                                                                       \
    if (__is_ct_const(f))                                               \
    {                                                                   \
        __ct_assert(0, s);                                              \
    }                                                                   \
    else                                                                \
    {                                                                   \
       RT_ASSERT(0);                                                    \
    }                                                                   \
} while(0)

#define _OVR_BYTE_MASK_CHECK(value)     _OVR_LIMIT_CHECK(value, 0, 0xff)
#define _OVR_LENGTH_CHECK(value)        _OVR_LIMIT_CHECK(value, 0, 0x1f)
#define _OVR_SIGNAL_NUMBER_CHECK(value) _OVR_LIMIT_CHECK(value, 0, 0xf)
#define _OVR_CTX_CHECK(value)           _OVR_LIMIT_CHECK(value, 0, 0x7)


#define _OVR_ISLAND_MASTER_CHECK(value)         _OVR_LIMIT_CHECK(value, 0, 0x3f)
#define _OVR_SIGNAL_DATA_MASTER_CHECK(value)    _OVR_LIMIT_CHECK(value, 0, 0x3fff)
#define _OVR_ISLAND_DATA_MASTER_CHECK(value)          _OVR_LIMIT_CHECK(value, 0, 0x3ff)

#define _OVR_DATA_FULL_REF_CHECK(value)         _OVR_LIMIT_CHECK(value, 0, 0x3fff)
#define _OVR_DATA_16BIT_IMM_CHECK(value)        _OVR_LIMIT_CHECK(value, 0, 0xffff)
#define _OVR_DATA_CTX(value)                    _OVR_LIMIT_CHECK(value, 0, 0x7)
#define _OVR_DATA_SIGNAL_CTX(value)             _OVR_LIMIT_CHECK(value, 0, 0x7)
#define _OVR_DATA_OFFSET_CTX(value)             _OVR_LIMIT_CHECK(value, 0, 0x7f)

#define _OVR_SIGNAL_MASTER_CHECK(value)         _OVR_LIMIT_CHECK(value, 0, 0xf)




/* Initialize an override word */
__intrinsic
void ovr_init(generic_ind_t *ind, uint32_t fields)
{

    uint32_t alu = fields;

    (*ind).alu_ind.value = 0;
    (*ind).csr_ind.value = 0;


    /* Check that only one override uses the data field in the ALU result */
    if ((fields & ovr_data_byte_mask) &&
        (fields & (ovr_data_full_ref | ovr_data_16bit_imm | ovr_data_ctx
            | ovr_data_and_signal_ctx | ovr_data_ctx_offset))) {
        _OVR_ERROR(alu, "Unsupported override");
    } else if ((fields & ovr_data_full_ref) &&
        (fields & (ovr_data_byte_mask | ovr_data_16bit_imm | ovr_data_ctx
                | ovr_data_and_signal_ctx | ovr_data_ctx_offset))) {
        _OVR_ERROR(alu, "Unsupported override");
    } else if ((fields & ovr_data_16bit_imm) &&
        (fields & (ovr_data_byte_mask | ovr_data_full_ref | ovr_data_ctx
                | ovr_data_and_signal_ctx | ovr_data_ctx_offset))) {
        _OVR_ERROR(alu, "Unsupported override");
    } else if ((fields & ovr_data_ctx) &&
       (fields & (ovr_data_byte_mask | ovr_data_16bit_imm | ovr_data_full_ref
                | ovr_data_and_signal_ctx | ovr_data_ctx_offset))) {
        _OVR_ERROR(alu, "Unsupported override");
    } else if ((fields & ovr_data_and_signal_ctx) &&
       (fields & (ovr_data_byte_mask | ovr_data_16bit_imm | ovr_data_ctx | ovr_data_full_ref
                | ovr_data_ctx_offset))) {
        _OVR_ERROR(alu, "Unsupported override");
    } else if ((fields & ovr_data_ctx_offset) &&
            (fields & (ovr_data_byte_mask | ovr_data_16bit_imm | ovr_data_ctx | ovr_data_full_ref
                | ovr_data_and_signal_ctx ))) {
        _OVR_ERROR(alu, "Unsupported override");
    }

    /* Check that only one override uses the island master in the CSR IND result */
    if ((fields & ovr_island_and_data_master) &&
        (fields & (ovr_signal_island_and_data_master | ovr_island_master))) {
        _OVR_ERROR(alu, "Unsupported override");
    } else if ((fields & ovr_signal_island_and_data_master) &&
        (fields & (ovr_island_and_data_master | ovr_island_master))) {
        _OVR_ERROR(alu, "Unsupported override");
    } else if ((fields & ovr_island_master) &&
        (fields & (ovr_signal_island_and_data_master | ovr_island_and_data_master))) {
        _OVR_ERROR(alu, "Unsupported override");
    }

    /* Check that only one override uses the signal master in the CSR IND result */
    if ((fields & ovr_signal_master) &&
        (fields & ovr_signal_island_and_data_master)) {
        _OVR_ERROR(alu, "Unsupported override");
    }

            /* Bytemask  */
        if (fields & ovr_byte_mask)
        {
            (*ind).alu_ind.value |= ovr_byte_mask_bit;
        }
        if (fields & ovr_data_byte_mask)
        {
            (*ind).alu_ind.value |= ovr_data_byte_mask_bit;
        }
        if (fields & ovr_length)
        {
            (*ind).alu_ind.value |= ovr_length_bit;
        }

        /* Data fields */
        if (fields & ovr_data_full_ref)
        {
            (*ind).alu_ind.value |= ovr_data_full_ref_bit;
        }
        if (fields & ovr_data_16bit_imm)
        {
            (*ind).alu_ind.value |= ovr_data_16bit_imm_bit;
        }
        if (fields & ovr_data_ctx)
        {
            (*ind).alu_ind.value |= ovr_data_ctx_bit;
        }
        if (fields & ovr_data_and_signal_ctx)
        {
            (*ind).alu_ind.value |= ovr_data_and_signal_ctx_bit;
        }
        if (fields & ovr_data_ctx_offset)
        {
            (*ind).alu_ind.value |= ovr_data_ctx_offset_bit;
        }

        if (fields & ovr_signal_ctx)
        {
            (*ind).alu_ind.value |= ovr_signal_ctx_bit;
        }
        if (fields & ovr_signal_master)
        {
            (*ind).alu_ind.value |= ovr_signal_master_bit;
        }
        if (fields & ovr_signal_number)
        {
            (*ind).alu_ind.value |= ovr_signal_number_bit;
        }

        /* Master fields */
        if (fields & ovr_island_and_data_master)
        {
            (*ind).alu_ind.value |= ovr_data_master_bit;
        }

        if (fields & ovr_signal_island_and_data_master)
        {
            (*ind).alu_ind.value |= ovr_signal_and_data_master_bit;
        }

        if (fields & ovr_island_master)
        {
            (*ind).alu_ind.value |= ovr_island_master_bit;
        }


}


/* Set the value of an override field */
__intrinsic
void ovr_set(generic_ind_t *ind, ovr_field_t field, int32_t value)
{
    {
    uint32_t alu = (*ind).alu_ind.value;

    CT_ASSERT(__is_ct_const(field));

        /* Bytemask in CSR */
        if (field & ovr_byte_mask)
        {
            uint32_t result = alu & ovr_byte_mask_bit;
            _OVR_BYTE_MASK_CHECK(value);

            if (result)
            {
                (*ind).csr_ind.byte_mask = value;
            } else {
                _OVR_ERROR(result, "Cannot set ovr_byte_mask");
            }
        }
        /* Bytemask in data (ALU) */
        if (field & ovr_data_byte_mask)
        {
            uint32_t result = alu & ovr_data_byte_mask_bit;
            _OVR_BYTE_MASK_CHECK(value);

            if (result)
            {
                (*ind).alu_ind.data16 = value;
            } else {
                _OVR_ERROR(result, "Cannot set ovr_data_byte_mask");
            }
        }
        if (field & ovr_length)
        {
            uint32_t result = alu & ovr_length_bit;
            _OVR_LENGTH_CHECK(value);

            if (result)
            {
                (*ind).alu_ind.ref_count = value;
            } else {
                _OVR_ERROR(result, "Cannot set ovr_length");
            }
        }

        /* Data fields */
        if (field & ovr_data_full_ref)
        {
            uint32_t result = alu & ovr_data_full_ref_bit;
            _OVR_DATA_FULL_REF_CHECK(value);

            if (result)
            {
                (*ind).alu_ind.data16 = value;
            } else {
                _OVR_ERROR(result, "Cannot set ovr_data_full_ref");
            }
        }
        if (field & ovr_data_16bit_imm)
        {
            uint32_t result = alu & ovr_data_16bit_imm_bit;
           _OVR_DATA_16BIT_IMM_CHECK(value);

            if (result)
            {
                (*ind).alu_ind.data16 = value;
            } else {
                _OVR_ERROR(result, "Cannot set ovr_data_16bit_imm");
            }
        }
        if (field & ovr_data_ctx)
        {
            uint32_t result = alu & ovr_data_ctx_bit;
            _OVR_DATA_CTX(value);

            if (result)
            {
                (*ind).alu_ind.data16 = value;
            } else {
                _OVR_ERROR(result, "Cannot set ovr_data_ctx");
            }
        }
        if (field & ovr_data_and_signal_ctx)
        {
            uint32_t result = alu & ovr_data_and_signal_ctx_bit;
            _OVR_DATA_SIGNAL_CTX(value);

            if (result)
            {
                (*ind).alu_ind.data16 = value;
            } else {
                _OVR_ERROR(result, "Cannot set ovr_data_and_signal_ctx");
            }
         }
        if (field & ovr_data_ctx_offset)
        {
            uint32_t result = alu & ovr_data_ctx_offset_bit;
           _OVR_DATA_OFFSET_CTX(value);

            if (result)
            {
                (*ind).alu_ind.data16 = value << 7;
            } else {
                _OVR_ERROR(result, "Cannot set ovr_data_ctx_offset");
            }
        }

        if (field & ovr_signal_ctx)
        {
            uint32_t result = alu & ovr_signal_ctx_bit;
            _OVR_CTX_CHECK(value);

            if (result)
            {
                (*ind).csr_ind.signal_ctx = value;
            } else {
                _OVR_ERROR(result, "Cannot set ovr_signal_ctx");
            }
        }
        if (field & ovr_signal_master)
        {
            uint32_t result = alu & ovr_signal_master_bit;
            _OVR_SIGNAL_MASTER_CHECK(value);

            if (result)
            {
                (*ind).csr_ind.signal_master = value;
            } else {
                _OVR_ERROR(result, "Cannot set ovr_signal_master");
            }
        }
        if (field & ovr_signal_number)
        {
            uint32_t result = alu & ovr_signal_number_bit;
            _OVR_SIGNAL_NUMBER_CHECK(value);

            if (result)
            {
                (*ind).csr_ind.signal_num = value;
            } else {
                _OVR_ERROR(result, "Cannot set ovr_signal_number");
            }
        }

        /* Master fields */
        if (field & ovr_island_and_data_master)
        {
            uint32_t result = alu & ovr_data_master_bit;
            _OVR_ISLAND_DATA_MASTER_CHECK(value);

            if (result)
            {
                (*ind).csr_ind.value |= ((value & 0x3ff) << 20);      // 10 bits - island+master
            } else {
                _OVR_ERROR(result, "Cannot set ovr_island_and_data_master");
            }
        }

        if (field & ovr_signal_island_and_data_master)
        {
            uint32_t result = alu & ovr_signal_and_data_master_bit;
            _OVR_SIGNAL_DATA_MASTER_CHECK(value);

            if (result)
            {
                (*ind).csr_ind.value |=  ((value & 0x3fff) << 16);     // 14 bits - island+master+signal_master
            } else {
                _OVR_ERROR(result, "Cannot set ovr_signal_and_data_master");
            }
        }

        if (field & ovr_island_master)
        {
            uint32_t result = alu & ovr_island_master_bit;
            _OVR_ISLAND_MASTER_CHECK(value);

            if (result)
            {
                (*ind).csr_ind.island = value;      // 6 bits - island
            } else {
                _OVR_ERROR(result, "Cannot set ovr_island_master");
            }
        }
    }

}

#pragma diag_default 279  /* suppressed above */

#endif /* __NFP_OVERRIDE_C__ */
