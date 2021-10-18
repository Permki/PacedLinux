/*
 * Copyright (C) 2014,  Netronome Systems, Inc.  All rights reserved.
 *
 * @file          lib/dcfl/_c/field_lkp.c
 * @brief         DCFL field lookup implementation
 */

#include <assert.h>
#include <dcfl.h>
#include <nfp/mem_bulk.h>
#include <nfp/mem_atomic.h>

#include <field_lkp.h>


/* (2 << 6) -> direct access, (24 << 0) -> island # */
#define MULE_EMU0_HI_BYTE    (0x98)

/* (2 << 6) -> direct access, (25 << 0) -> island # */
#define MULE_EMU1_HI_BYTE    (0x99)

/* (2 << 6) -> direct access, (26 << 0) -> island # */
#define MULE_EMU2_HI_BYTE    (0x9A)

/* (2 << 6) -> direct access, (28 << 0) -> island # */
#define MULE_IMU0_HI_BYTE    (0x9C)

/* (2 << 6) -> direct access, (29 << 0) -> island # */
#define MULE_IMU1_HI_BYTE    (0x9D)


/** Direct lookup small table sizes (number of entries) */
typedef enum MEM_LOOKUP_DLUT_SMALL_SIZE
{
    LOOKUP_DLUT_SMALL_SIZE_1K          = 0x00,
    LOOKUP_DLUT_SMALL_SIZE_2K          = 0x01,
    LOOKUP_DLUT_SMALL_SIZE_4K          = 0x02,
    LOOKUP_DLUT_SMALL_SIZE_8K          = 0x03,
    LOOKUP_DLUT_SMALL_SIZE_16K         = 0x04,
    LOOKUP_DLUT_SMALL_SIZE_32K         = 0x05,
    LOOKUP_DLUT_SMALL_SIZE_64K         = 0x06,
    LOOKUP_DLUT_SMALL_SIZE_128K        = 0x07

}  MEM_LOOKUP_DLUT_SMALL_SIZE;

/** Direct lookup small table sizes (number of entries) */
typedef enum MEM_LOOKUP_DLUT_LARGE_SIZE
{
    LOOKUP_DLUT_LARGE_SIZE_64K         = 0x00,
    LOOKUP_DLUT_LARGE_SIZE_128K        = 0x01,
    LOOKUP_DLUT_LARGE_SIZE_256K        = 0x02,
    LOOKUP_DLUT_LARGE_SIZE_512K        = 0x03,
    LOOKUP_DLUT_LARGE_SIZE_1M          = 0x04,
    LOOKUP_DLUT_LARGE_SIZE_2M          = 0x05,
    LOOKUP_DLUT_LARGE_SIZE_4M          = 0x06,
    LOOKUP_DLUT_LARGE_SIZE_8M          = 0x07

}  MEM_LOOKUP_DLUT_LARGE_SIZE;


/** Lookup result bits */
typedef enum MEM_LOOKUP_RESULT
{
    LOOKUP_24BIT_RESULT                 = 0x00,
    LOOKUP_32BIT_RESULT                 = 0x01
}  MEM_LOOKUP_RESULT;

/** Lookup result bits */
typedef enum MEM_LOOKUP_DLUT_TYPE
{
    LOOKUP_DLUT_SMALL                 = 0x00,
    LOOKUP_DLUT_LARGE                 = 0x01
}   MEM_LOOKUP_DLUT_TYPE;

/** Command for direct lookup small table (adapted from nfp-sdk
 * standardlibrary nfp_mem_lookup_engine_6000.h) */
struct mem_lookup_dlut_small_t
{
    union {
        struct
        {
            /**< If internal memory is used. */
            unsigned int internal_memory                : 1;
            /**< 1 = direct lookup. */
            unsigned int direct_lookup                  : 1;
            /**< Base address at which the first result is located. */
            unsigned int base_address                   : 16;
            /**< Reserved. */
            unsigned int reserved_2                     : 1;
            /**< 0 = 24 bits, 1 = 32 bit. */
            MEM_LOOKUP_RESULT result                    : 1;
            /**< Reserved. */
            unsigned int reserved_1                     : 1;
            /**< Specified table size. */
            MEM_LOOKUP_DLUT_SMALL_SIZE table_size       : 3;
            /**< 0 = small table type: 1K to 128K. */
            unsigned int table_type                     : 1;
            /**< Starting bit position, bit 0 to bit 127. */
            unsigned int start_bit_position             : 7;
        };
        /**< Accessor to entire descriptor structure. */
        unsigned int raw;
    };
};

/** Command for direct lookup large table */
struct mem_lookup_dlut_large_t {
    union {
        struct {
            /**< If internal memory is used. */
            unsigned int internal_memory                    : 1;
            /**< 1 = direct lookup. */
            unsigned int direct_lookup                      : 1;
            /**< Base address at which the first result is located. */
            unsigned int base_address                       : 15;
            /**< Reserved. */
            unsigned int reserved_2                         : 2;
            /**< 0 = 24 bits, 1 = 32 bits. */
            MEM_LOOKUP_RESULT result                        : 1;
            /**< Reserved. */
            unsigned int reserved_1                         : 1;
            /**< Specified table size. */
            MEM_LOOKUP_DLUT_LARGE_SIZE table_size           : 3;
            /**< 1 = large table size. 64K to 8M. */
            unsigned int table_type                         : 1;
            /**< Starting bit position, bit 0 to bit 127. */
            unsigned int start_bit_position                 : 7;
        };
        /**< Accessor to entire descriptor structure. */
        unsigned int raw;
    };
};


/**
 * Perform a DLUT lookup for Stage0 of APM or range fields, or direct lookup.
 *
 * @label_set          LM label_set where to store the resulting labels
 * @value              Field value
 * @field_cfg          Field configuration including base.
 * @startbit           Starting bit position, differs between types of lookups.
 *
 * @return             Number of labels hit with @value
 *
 */
void
dcfl_dlut_field_lookup(__xrw uint32_t *value,
                         uint32_t field_lkp_base,
                         uint32_t field_lkp_base_hi, int startbit)
{
    volatile struct mem_lookup_dlut_large_t large_table_desc;
    volatile __xwrite unsigned int lookup_index[2];
    SIGNAL_PAIR sig_pair;
    unsigned int addr_hi;
    __gpr uint32_t result;

    large_table_desc.raw = 0;

    addr_hi = field_lkp_base_hi << 24;

    if (addr_hi >= (MULE_IMU0_HI_BYTE << 24))
        large_table_desc.internal_memory = 1;

    /* TODO use a #define */
    large_table_desc.start_bit_position = startbit;
    large_table_desc.table_type = LOOKUP_DLUT_LARGE;
    large_table_desc.table_size = LOOKUP_DLUT_LARGE_SIZE_64K;
    large_table_desc.result = LOOKUP_32BIT_RESULT;
    large_table_desc.direct_lookup = 1;
    large_table_desc.base_address = (field_lkp_base >> 18) & 0x7FFF;

    /* all fields of large_table_desc are ready, pass them in one go
     * to the lookup engine with large_table_desc.raw */
    __asm { mem[lookup, *value, addr_hi, << 8,\
              large_table_desc.raw, 2],sig_done[sig_pair] }

    wait_for_all(&sig_pair);
}


int
dcfl_field_lookup(__lmem struct dcfl_label_set *labels,
                  __gpr unsigned long valid_val,
                  __xrw uint32_t *value,
                  __xread struct dcfl_me_field_config *field_cfg,
                  unsigned int hdr_valid
                 )
{
    int nlabels;
    unsigned int wildcard;
    uint32_t label = 0, multilabel_idx;
    __addr40 __mem unsigned long *multilabel_addr;
    __xread uint32_t multilabel[16];



    /* Make sure the parameters are as we expect */
    ctassert(__is_in_reg_or_lmem((void *)labels));

#ifdef DCFL_P4_STRICT_HDR_VALID
    nlabels = 0;
    if (hdr_valid) {
#endif
    switch (field_cfg->type) {
    case DCFL_DIRECT_FIELD_TYPE:
    case DCFL_RANGE_FIELD_TYPE:
        nlabels = 0;
        dcfl_dlut_field_lookup(value, field_cfg->field_base,
                                      field_cfg->field_base_hi, 96);
        label = *(value+0);
        break;
    case DCFL_APM_IPv4_FIELD_TYPE:
    case DCFL_APM_IPv6_FIELD_TYPE:
        nlabels = 0;
        dcfl_dlut_field_lookup(value, field_cfg->field_base,
                                      field_cfg->field_base_hi, 112);
        label = *(value+0);
        break;
    case DCFL_VALID_FIELD_TYPE:
        {
            int vld, lbl;
            vld = field_cfg->valid_value == valid_val;
            if (vld)
                lbl = (field_cfg->valid_labelmap >> 0) & 0x3;
            else
                lbl = (field_cfg->valid_labelmap >> 2) & 0x3;

            if (lbl != 0) {
                labels->labels[0] = lbl;
                nlabels = 1;
            } else
                nlabels = 0;
        }
        break;
    default:
        __rt_assert();
    }

    /* Test for multilabel for fields of type range, apm (ipv4 and ipv6) */
    if (label) {
        /* got a valid label, is it a multilabel? */
        if (label & (1<<DCFL_LKP_ML_BIT)) {
            multilabel_addr = (__addr40 __mem void *)
                (((unsigned long long)field_cfg->field_base_hi << 32) |
                    (unsigned long long) (label & 0x3FFFFFFF));

            mem_read64(&multilabel, multilabel_addr, sizeof(multilabel));

            for (multilabel_idx = 0;
                    multilabel_idx < 32; multilabel_idx++) {
                if (multilabel[multilabel_idx]) {
                    labels->labels[nlabels] = multilabel[multilabel_idx];
                    nlabels += 1;
                } else
                    break;
            }
            if (multilabel[multilabel_idx]) {
                mem_incr64(&DCFL_LKP_OVERFLOW_CNTR);
            }
        } else {
            labels->labels[nlabels] = label;
            nlabels += 1;
        }
    }
#ifdef DCFL_P4_STRICT_HDR_VALID
    }
#endif

    /* and now check for wildcard label */
    wildcard = field_cfg->wildcard_label;
    if (wildcard) {
        labels->labels[nlabels] = wildcard;
        nlabels += 1;
    }


    return nlabels;
}

#ifdef DCFL_P4_STRICT_HDR_VALID
extern __forceinline void
dcfl_get_hdr_valid_val(__gpr unsigned int *hdr_valid,
                   __lmem uint32_t *localkey,
                   __xread struct dcfl_me_field_config *f_cfg)
{
    __gpr uint32_t word;

    word = localkey[f_cfg->hdr_valid_offset];
    word = word >> f_cfg->hdr_valid_shift;
    word = word & ((1 << f_cfg->hdr_valid_bitlen) - 1);
    *hdr_valid = word;
}

extern __forceinline void
dcfl_get_hdr_valid_val_from_mem(__gpr unsigned int *hdr_valid,
                   __addr40 __mem uint8_t *key,
                   __xread struct dcfl_me_field_config *f_cfg)
{
    __xread uint32_t read_buf[4];
    __gpr uint32_t word;

    /* Read 4 words from key */
    mem_read32(read_buf, (__addr40 __mem uint32_t *)key + f_cfg->hdr_valid_offset,
                   sizeof(read_buf));

    word = read_buf[0];
    word = word >> f_cfg->hdr_valid_shift;
    word = word & ((1 << f_cfg->hdr_valid_bitlen) - 1);
    *hdr_valid = word;
}
#endif

extern __forceinline void
dcfl_get_field_value(__gpr uint32_t *valid_val,
                     __xrw uint32_t *value,
                     __lmem uint32_t *localkey,
                     __xread struct dcfl_me_field_config *f_cfg)
{
    __gpr uint32_t field_mask, word, bitlen, reg_offset, i;

    bitlen = f_cfg->hdr_bitlen + 1;

#ifdef DCFL_BIG_ENDIAN
    reg_offset = 0;
#else
    reg_offset = ((128 - bitlen) / 32) & 0x3;
#endif

    word = localkey[f_cfg->hdr_offset];
    word = word >> f_cfg->hdr_shift;

    if (bitlen <= 32) {
        if (bitlen < 32) {
            field_mask = (1 << bitlen) - 1;
            word &= field_mask;
        }
        *(value+3) = *valid_val = word;
    } else {
        for (i = 0; reg_offset < 4; reg_offset++) {
            if (bitlen < 32) {
                field_mask = (1 << bitlen) - 1;
                word &= field_mask;
            }

            switch (reg_offset) {
#ifdef DCFL_BIG_ENDIAN
                /* Reverse word order of multi-word fields */
                case 0: *(value+3) = word; break;
                case 1: *(value+2) = word; break;
                case 2: *(value+1) = word; break;
                case 3: *(value+0) = word; break;
#else
                case 0: *(value+0) = word; break;
                case 1: *(value+1) = word; break;
                case 2: *(value+2) = word; break;
                case 3: *(value+3) = word; break;
#endif
                default: break;
            }

            bitlen -= 32;
            i++;
            word = localkey[f_cfg->hdr_offset + i];
        }
    }
}

extern __forceinline void
dcfl_get_field_value_from_mem(__gpr uint32_t *valid_val,
                     __xrw uint32_t *value,
                     __addr40 __mem uint8_t *key,
                     __xread struct dcfl_me_field_config *f_cfg)
{
    __xread uint32_t read_buf[4];
    __gpr uint32_t field_mask, word, bitlen, reg_offset, i;

    bitlen = f_cfg->hdr_bitlen + 1;

#ifdef DCFL_BIG_ENDIAN
    reg_offset = 0;
#else
    reg_offset = ((128 - bitlen) / 32) & 0x3;
#endif

    /* Read 4 words from key */
    mem_read32(read_buf, (__addr40 __mem uint32_t *)key + f_cfg->hdr_offset,
                   sizeof(read_buf));

    word = read_buf[0];
    word = word >> f_cfg->hdr_shift;

    if (bitlen <= 32) {
        if (bitlen < 32) {
            field_mask = (1 << bitlen) - 1;
            word &= field_mask;
        }
        *(value+3) = *valid_val = word;
    } else {
        for (i = 0; reg_offset < 4; reg_offset++) {
            if (bitlen < 32) {
                field_mask = (1 << bitlen) - 1;
                word &= field_mask;
            }

            switch (reg_offset) {
#ifdef DCFL_BIG_ENDIAN
                /* Reverse word order of multi-word fields */
                case 0: *(value+3) = word; break;
                case 1: *(value+2) = word; break;
                case 2: *(value+1) = word; break;
                case 3: *(value+0) = word; break;
#else
                case 0: *(value+0) = word; break;
                case 1: *(value+1) = word; break;
                case 2: *(value+2) = word; break;
                case 3: *(value+3) = word; break;
#endif
                default: break;
            }

            bitlen -= 32;
            i++;

            word = read_buf[i];
        }
    }
}
