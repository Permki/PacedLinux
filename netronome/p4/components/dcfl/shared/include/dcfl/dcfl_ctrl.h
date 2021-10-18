/**
 * Copyright (C) 2015,  Netronome Systems, Inc.  All rights reserved.
 *
 * @file        dcfl_ctrl.h
 * @brief       DCFL controlling structures, shared between host and ME.
 */
#ifndef _DCFL_CTRL_H_
#define _DCFL_CTRL_H_

#ifndef DCFL_MAX_NUM_TABLES
/* Support up to 256 DCFL tables */
#define DCFL_MAX_NUM_TABLES         256
#endif

#ifndef DCFL_MAX_NUM_FIELDS
/* Support up to 40 fields */
#define DCFL_MAX_NUM_FIELDS         40
#endif

/* Number of long words per field config */
#define DCFL_ME_FIELD_CONFIG_LW     6

/* Number of long words per block in AN, same for PMM32 12.9d */
#define DCFL_MEM_AN_BLOCK_LW    16
#define DCFL_AN_BLOCK_NUM_ENTRIES   (DCFL_MEM_AN_BLOCK_LW - 1)
/* To add priorities in last stage, each rule takes up 1.5 LW instead of 1,
 * so divide number of entries by 1.5 */
#define DCFL_AN_FIN_BLOCK_NUM_RULES      (DCFL_AN_BLOCK_NUM_ENTRIES * 2/3)

#define DCFL_AN_PMM_BLOCK_NUM_ENTRIES        9

/* EAS MU lookup engine section 2.1.2.3 "
 * Large tables should be 256K byte aligned
 */
#define DCFL_NFPMEM_DLUT_ALIGNMENT  262144

#define DCFL_NFPMEM_LW_ALIGNMENT    4
#define DCFL_NFPMEM_PMM_ALIGNMENT   64

/* DCFL per table configuration */
#ifdef DCFL_CUSTOM_TABLE_LW
#define DCFL_ME_TABLE_CONFIG_LW  ((DCFL_MAX_NUM_FIELDS *\
                                   DCFL_ME_FIELD_CONFIG_LW) + 1\
                                   + DCFL_CUSTOM_TABLE_LW)
#else
#define DCFL_ME_TABLE_CONFIG_LW  ((DCFL_MAX_NUM_FIELDS *\
                                   DCFL_ME_FIELD_CONFIG_LW) + 1)
#endif

/* DCFL global configuration */
/* Power of 2, large enough to hold two buffers and one LW for active_buffer */
#define DCFL_ME_TABLE_CONFIG_BUFFERS_LW   (1 << 9)

#define DCFL_ME_CONFIG_LW         (DCFL_MAX_NUM_TABLES *\
                                   DCFL_ME_TABLE_CONFIG_BUFFERS_LW)

/* Bit flag that MULE engine uses to indicate recursive lookup */
#define DCFL_LKP_RECURSIVE_BIT      31

/* Number of xpbm:...AlgorithmicTableLocation registers */
#define DCFL_NUM_ALUT_LOCATION_REG  8

/* ALUT address bits encoded in MU data */
#define DCFL_ALUT_ADDR_BITS         28

/* Bit flag indicating a multilabel value in lookup data */
#define DCFL_LKP_ML_BIT             30

/* A hack to get around NFCC defining MSC_VER */
#ifdef _WIN32
#ifndef __NFP_LANG_MICROC
#define __MSC_VER
#endif
#endif

/* Pack all struct for _WIN32, __GNUC__ and __NFP_LANG_MICROC */
#ifndef __NFP_LANG_ASM
#pragma pack(push, 1)
#endif

#if defined(__STDC__) || defined(__NFP_LANG_MICROC) || defined(__MSC_VER)
#include <stdint.h>

/* is this a common host C compiler and common host architecture (x86)? */
#if defined(__GNUC__) &&  (__BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__)
struct dcfl_me_field_config {
    union {
        struct {
            /* this is the known bit order for this compiler and x86 */
            uint32_t type           :3;      /* The lookup type */
            uint32_t hdr_offset     :8;      /* The word offset */
            uint32_t hdr_bitlen     :7;     /* The field bit length */
            uint32_t hdr_shift      :5;     /* The bit offset */
            uint32_t an_alut_loc_reg     :3;
            uint32_t reserved0      :6;

            uint32_t wildcard_label :16; /* Wildcard label if exists*/
            uint32_t valid_value    :12; /* Value for valid match comparison */
            uint32_t valid_labelmap :4;  /* Label assignments for valid match */

            /* 40 bit address to field's lookup table */
            uint64_t field_base     :40;

#ifdef DCFL_P4_STRICT_HDR_VALID
            uint64_t hdr_valid_offset : 4;  /* Offset to valid word */
            uint64_t hdr_valid_shift  : 5;  /* Bit shift for valid value */
            uint64_t hdr_valid_bitlen : 6;  /* Bit length, 0=no valid check */
            uint64_t reserved1        : 9;
#else
            uint64_t reserved1        :24;
#endif

            /* 40 bit address to field's stage in AN  */
            uint64_t an_stage_base;
        };
        uint32_t raw[DCFL_ME_FIELD_CONFIG_LW];
    };
};

/* EAS MU lookup engine
 * Table 2.30. PMM32 12.9d Memory Contents
 */
struct pmm_mask_val {
    uint16_t mask:          4;
    uint16_t val:          12;
};

struct PMM32_12_9d {
    union {
        struct {
            struct pmm_mask_val pmv_a[7];
            uint16_t encoding;   /* 0xA2 for this table type */
            struct pmm_mask_val pmv_b[2];
            uint32_t reserved0;
            uint32_t default_value;
            uint32_t result[9];
        };
        uint32_t raw[DCFL_MEM_AN_BLOCK_LW];
    };
};

#elif defined(__NFP_LANG_MICROC)

struct PMM32_12_9d {
    uint32_t raw[DCFL_MEM_AN_BLOCK_LW];
};

struct dcfl_me_field_config {
    union {
        __packed struct {
            /* nfcc bit order is GCC&x86 reverse one   */
            uint32_t reserved0      :6;
            uint32_t an_alut_loc_reg     :3;
            uint32_t hdr_shift      :5;     /* The bit offset */
            uint32_t hdr_bitlen     :7;     /* The field bit length */
            uint32_t hdr_offset     :8;      /* The word offset */
            uint32_t type           :3;      /* The lookup type */

            uint32_t valid_labelmap :4;  /* Label assignments for valid match */
            uint32_t valid_value    :12; /* Value for valid match comparison */
            uint32_t wildcard_label :16; /* Wildcard label if exists*/

            /* 40 bit address to field's lookup table */
            uint32_t field_base;

#ifdef DCFL_P4_STRICT_HDR_VALID
            uint32_t reserved1        : 9;
            uint32_t hdr_valid_bitlen : 6;  /* Bit length, 0=no valid check */
            uint32_t hdr_valid_shift  : 5;  /* Bit shift for valid value */
            uint32_t hdr_valid_offset : 4;  /* Offset to valid word */
#else
            uint32_t reserved1        :24;
#endif

            uint32_t field_base_hi    : 8;

            /* 40 bit address to field's stage in AN  */
            uint32_t an_stage_base;
            uint32_t reserved2        :24;
            uint32_t an_stage_base_hi : 8;
        };
        uint32_t raw[DCFL_ME_FIELD_CONFIG_LW];
    };
};
#elif defined(__MSC_VER) && (__BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__)
struct dcfl_me_field_config {
    union {
        struct {
            /* this is the known bit order for this compiler and x86 */
            uint32_t type           :3;      /* The lookup type */
            uint32_t hdr_offset     :8;      /* The word offset */
            uint32_t hdr_bitlen     :7;     /* The field bit length */
            uint32_t hdr_shift      :5;     /* The bit offset */
            uint32_t an_alut_loc_reg     :3;
            uint32_t reserved0      :6;

            uint32_t wildcard_label :16; /* Wildcard label if exists*/
            uint32_t valid_value    :12; /* Value for valid match comparison */
            uint32_t valid_labelmap :4;  /* Label assignments for valid match */

            /* 40 bit address to field's lookup table */
            uint64_t field_base     :40;

#ifdef DCFL_P4_STRICT_HDR_VALID
            uint64_t hdr_valid_offset : 4;  /* Offset to valid word */
            uint64_t hdr_valid_shift  : 5;  /* Bit shift for valid value */
            uint64_t hdr_valid_bitlen : 6;  /* Bit length, 0=no valid check */
            uint64_t reserved1        : 9;
#else
            uint64_t reserved1        :24;
#endif

            /* 40 bit address to field's stage in AN  */
            uint64_t an_stage_base;
        };
        uint32_t raw[DCFL_ME_FIELD_CONFIG_LW];
    };
};
#else
    #error "Wrong compiler, " \
    "only GCC in x86, MSVC in x86 and nfcc are supported." \
    "Need to  manually adapt the struct with bitfields to the specific" \
    "bit order this compiler chooses"
#endif

/* Boundary to align AN and field lookups on */
#define DCFL_NFPMEM_ALIGN           DCFL_MEM_AN_BLOCK_LW * sizeof(uint32_t)
/* 40 bit mask for address for fields and stages */
#define DCFL_NFPMEM_BASE_MASK       0xFFFFFFFFFF
/* For DMA, the high byte of 40 bit address is calculated from island# */
#define DCFL_NFPMEM_CALC_DMA_HI_BYTE(isl) \
        (((isl) >= 24 && (isl) < 28) ? \
         ((0 << 6) | (1 << 5) | (((isl) & 0x3) << 3)) : /* emem case */ \
         ((2 << 6) | (isl))) /* imem/ctm case */

/* ME Config and table sizes */

/* APM lookups constants */
#define DCFL_APM_STAGE0_LEN         16
#define DCFL_APM_STAGE0_SIZE        (1<<DCFL_APM_STAGE0_LEN) * sizeof(uint32_t)

/* Size of AN in blocks: All offsets in all stages */
#define DCFL_NFPMEM_AN_MAX_NUM_BLOCKS   (1<<10)
/*(DCFL_MAX_NUM_FIELDS - 1) \
                                        * DCFL_MAX_LABELS \
                                        * DCFL_NFPMEM_AN_MAX_BLOCKS_PER_OFFSET
*/

/* Bit flag indicating a multilabel value in label data */
#define DCFL_LABEL_ML_BIT           17

struct dcfl_me_table_config {
    union {
        struct {
#ifdef DCFL_CUSTOM_TABLE_LW
           uint32_t custom_data[DCFL_CUSTOM_TABLE_LW];
#endif
           /* The number of fields used */
           uint32_t num_fields;

           /* Per field configuration */
           /* The order of the fields in the array
           determines the lookup order */
           struct dcfl_me_field_config f_cfg[DCFL_MAX_NUM_FIELDS];
        };
        uint32_t raw[DCFL_ME_TABLE_CONFIG_LW];
    };
};

/* DCFL per table configuration, both buffers and active buf_idx */
struct dcfl_me_table_config_buffers {
     union {
         struct {
             /* Active buffer: 0 - none, 1 - primary, 2 - secondary */
             uint32_t active_buffer;
             struct dcfl_me_table_config buf[2];
         };
         uint32_t raw[DCFL_ME_TABLE_CONFIG_BUFFERS_LW];
     };
};

struct dcfl_me_config {
    union {
        struct {
            struct dcfl_me_table_config_buffers tables[DCFL_MAX_NUM_TABLES];
        };
        uint32_t raw[DCFL_ME_CONFIG_LW];
    };
};

/* AN */
struct dcfl_me_an_entry {
    uint32_t metalabel  :16;
    uint32_t label      :16;
};

struct dcfl_me_an_block {
    union {
        struct {
            struct dcfl_me_an_entry entries[DCFL_AN_BLOCK_NUM_ENTRIES];
            uint32_t linked_block_ptr;
        };
        struct PMM32_12_9d pmm;
    };
};

/* Final stage of AN has different struct, but must be the same size */
#if (defined(__GNUC__) &&  (__BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__)) \
    || (defined(__MSC_VER) &&  (__BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__))
struct dcfl_me_an_final_block {
#elif defined(__NFP_LANG_MICROC)
    __packed struct dcfl_me_an_final_block {
#else
    #error "Wrong compiler, " \
    "only GCC in x86, MSVC in x86 and nfcc are supported." \
    "Need to  manually adapt the struct with bitfields to the specific " \
    "bit order this compiler chooses"
#endif
    struct {
        uint32_t metalabel  :16;
        uint32_t label      :16;
    } entries[DCFL_AN_FIN_BLOCK_NUM_RULES];
    uint16_t priorities[DCFL_AN_FIN_BLOCK_NUM_RULES];
    uint32_t linked_block_ptr;
};

struct dcfl_me_an {
    union {
        struct dcfl_me_an_block *blocks;
        struct dcfl_me_an_final_block *fin_blocks;
        uint32_t *raw;
    };
};

#if defined(__NFP_LANG_MICROC)

/* The number of fields retrieved at a time for processing */
#define DCFL_ME_TABLE_CONFIG_SMALL_NUM_FIELDS    1

#define DCFL_ME_TABLE_CONFIG_SMALL_LW \
        (DCFL_ME_TABLE_CONFIG_SMALL_NUM_FIELDS * DCFL_ME_FIELD_CONFIG_LW + 1)

/* a copy of the dcfl_me_table_config with only 13 LWs,
 * starting after custom data in dcfl_me_table_config */
struct dcfl_me_table_config_small {
     union {
         struct {
             /* The number of fields used */
             uint32_t num_fields;

             /* Per field configuration */
             /* The order of the fields in the array
                determines the lookup order */
             struct dcfl_me_field_config f_cfg
             [DCFL_ME_TABLE_CONFIG_SMALL_NUM_FIELDS];
         };
         uint32_t raw[DCFL_ME_TABLE_CONFIG_SMALL_LW];
     };
};

#endif /* #if defined(__NFP_LANG_MICROC) */

#elif defined(__NFP_LANG_ASM)
/* Assembler/Microcode definitions */

/* Per field configuration */
#define      DCFL_ME_BUF_IDX_SIZE       4
#define      DCFL_FLD_VAL_MAX_LW        4
#define      DCFL_ME_FLD_NUM_SIZE       4
#define_eval DCFL_ME_FLD_SIZE           (DCFL_ME_FIELD_CONFIG_LW * 4)
#define_eval DCFL_FLD_REF_CNT           (DCFL_ME_FIELD_CONFIG_LW / 2)
#define_eval DCFL_ME_TBL_SIZE           (DCFL_ME_FLD_SIZE \
                                        * DCFL_MAX_NUM_FIELDS \
                                        + DCFL_ME_FLD_NUM_SIZE)
#define_eval DCFL_ME_BUF_SIZE           (DCFL_ME_TABLE_CONFIG_BUFFERS_LW * 4)


/* Bitfields for dcfl_me_field_config mem struct */
#define DCFL_FIELD_CFG_TYPE_bf              0, 2, 0
#define DCFL_FIELD_CFG_HDR_OFFSET_bf        0,10, 3
#define DCFL_FIELD_CFG_HDR_BITLEN_bf        0,17,11
#define DCFL_FIELD_CFG_HDR_SHF_bf           0,22,18

#define DCFL_FIELD_CFG_WILDCARD_LABEL_bf    1,15, 0

#define DCFL_FIELD_CFG_FIELD_ADDR_LO_bf     2,31, 0
#define DCFL_FIELD_CFG_FIELD_ADDR_HI_bf     3, 7, 0

#define DCFL_FIELD_CFG_STAGE_ADDR_LO_bf     4,31, 0
#define DCFL_FIELD_CFG_STAGE_ADDR_HI_bf     5, 7, 0

#define DCFL_CONFIG_FIELDS_ORDER_LW 16

#endif /* #if defined(__STDC__) || defined(__NFP_LANG_MICROC) */

#ifndef __NFP_LANG_ASM
    #pragma pack(pop)
#endif

#endif /* _DCFL_CTRL_H_ */
