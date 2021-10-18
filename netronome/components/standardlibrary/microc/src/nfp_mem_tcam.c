/*
 * Copyright (C) 2008-2014 Netronome Systems, Inc.  All rights reserved.
 */

#ifndef __NFP_MEM_TCAM_C__
#define __NFP_MEM_TCAM_C__

#include <nfp.h>
#include <nfp_intrinsic.h>
#include <nfp_mem.h>
#include <nfp_mem_tcam.h>

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

#define _MEM_TCAM_PARAM_ALIGN_16_CHECK(addr)                \
        if (__is_ct_const(addr))                            \
        {                                                   \
            CT_ASSERT((addr & 0xf) == 0);                   \
        }                                                   \
        else                                                \
        {                                                   \
            RT_ALIGN_ASSERT((addr & 0xf) == 0);             \
        }

#define _MEM_TCAM_PARAM_SIGNAL_CHECK(sig)                   \
        if (__is_ct_const(sig))                             \
        {                                                   \
            CT_ASSERT(sig == sig_done || sig == ctx_swap);  \
        }

#define _MEM_TCAM_PARAM_SIGNAL_PAIR_CHECK(sync)             \
        if (__is_ct_const(sync))                            \
        {                                                   \
            CT_ASSERT(sync == sig_done);                    \
        }

#define _MEM_TCAM_INDEX_CHECK(index, max_value)             \
        if (__is_ct_const(index))                           \
        {                                                   \
            CT_ASSERT((index) < (max_value));               \
        }                                                   \
        else                                                \
        {                                                   \
            RT_RANGE_ASSERT((index) < (max_value));         \
        }

#define _MEM_TCAM_ENTRY_RANGE_CHECK(value, max_value)       \
        if (__is_ct_const(value))                           \
        {                                                   \
            CT_ASSERT((value) <= (max_value));              \
        }                                                   \
        else                                                \
        {                                                   \
            RT_RANGE_ASSERT((value) <= (max_value));        \
        }


__intrinsic
void cmd_mem_tcam128_init_ptr32(
    __mem mem_tcam128_in_mem_t *tcam
)
{
    cmd_mem_write_atomic_imm_ptr32(0, (void __addr32 __mem *) tcam, sizeof(*tcam) / sizeof(uint32_t));
}

__intrinsic
void cmd_mem_tcam128_init_ptr40(
    mem_tcam128_ptr40_t tcam
)
{
    cmd_mem_write_atomic_imm_ptr40(0, tcam, sizeof(*tcam) / sizeof(uint32_t));
}

__intrinsic
void cmd_mem_tcam256_init_ptr32(
    __mem mem_tcam256_in_mem_t *tcam
)
{
    cmd_mem_write_atomic_imm_ptr32(0, (void __addr32 __mem *)tcam, sizeof(*tcam) / sizeof(uint32_t));
}

__intrinsic
void cmd_mem_tcam256_init_ptr40(
    mem_tcam256_ptr40_t tcam
)
{
    cmd_mem_write_atomic_imm_ptr40(0, tcam, sizeof(*tcam) / sizeof(uint32_t));
}

__intrinsic
void cmd_mem_tcam384_init_ptr32(
    __mem mem_tcam384_in_mem_t *tcam
)
{
    __addr32 __mem uint32_t *fp = (__addr32 __mem uint32_t *)tcam;
    cmd_mem_write_atomic_imm_ptr32(0, (void __addr32 __mem *)tcam, 8);
    cmd_mem_write_atomic_imm_ptr32(0, fp + 8, 4);
}

__intrinsic
void cmd_mem_tcam384_init_ptr40(
    mem_tcam384_ptr40_t tcam
)
{
    __addr40 __mem uint32_t *fp = (__addr40 __mem uint32_t *)tcam;
    cmd_mem_write_atomic_imm_ptr40(0, tcam, 8);
    cmd_mem_write_atomic_imm_ptr40(0, fp + 8, 4);
}

__intrinsic
void cmd_mem_tcam512_init_ptr32(
    __mem mem_tcam512_in_mem_t *tcam
)
{
    __addr32 __mem uint32_t *fp = (__addr32 __mem uint32_t *)tcam;
    cmd_mem_write_atomic_imm_ptr32(0, (void __addr32 __mem *)tcam, sizeof(*tcam) / sizeof(uint32_t) / 2);
    cmd_mem_write_atomic_imm_ptr32(0, fp + sizeof(*tcam) / sizeof(uint32_t) / 2, sizeof(*tcam) / sizeof(uint32_t) / 2);
}

__intrinsic
void cmd_mem_tcam512_init_ptr40(
    mem_tcam512_ptr40_t tcam
)
{
    __addr40 __mem uint32_t *fp = (__addr40 __mem uint32_t *)tcam;
    cmd_mem_write_atomic_imm_ptr40(0, tcam, sizeof(*tcam) / sizeof(uint32_t) / 2);
    cmd_mem_write_atomic_imm_ptr40(0, fp + sizeof(*tcam) / sizeof(uint32_t) / 2, sizeof(*tcam) / sizeof(uint32_t) / 2);
}

#define _MEM_TCAMNNN_SET8_WORD_PTR32(nnn)                           \
__intrinsic                                                         \
void mem_tcam##nnn##_set8_word_ptr32(                               \
    __mem mem_tcam##nnn##_in_mem_t *tcam,                           \
    uint32_t index,                                                 \
    uint32_t value,                                                 \
    sync_t sync,                                                    \
    SIGNAL *sig_ptr                                                 \
    )                                                               \
{                                                                   \
    CT_ASSERT(__is_ct_const(sync));                                 \
    _MEM_TCAM_INDEX_CHECK(index, sizeof(*tcam));                    \
    _MEM_TCAM_ENTRY_RANGE_CHECK(value, 0xff);                       \
    {                                                               \
        __xwrite uint32_t v;                                        \
        generic_ind_t ind;                                          \
        __addr32 __mem uint32_t *ptr = (__addr32 __mem uint32_t *)tcam; \
        v = (value << 24) >> ((index & 3) << 3);                    \
        ptr += (index >> 2);                                        \
        _INTRINSIC_OVERRIDE_BYTEMASK(ind, 8 >> (index & 3));        \
        cmd_mem_write_atomic_ind_ptr32(&v,(__addr32 __mem void*)ptr,1,ind,sync,sig_ptr); \
    }                                                               \
}

#define _MEM_TCAMNNN_SET8_WORD_PTR40(nnn)                           \
__intrinsic                                                         \
void mem_tcam##nnn##_set8_word_ptr40(                               \
    mem_tcam##nnn##_ptr40_t tcam,                                   \
    uint32_t index,                                                 \
    uint32_t value,                                                 \
    sync_t sync,                                                    \
    SIGNAL *sig_ptr                                                 \
    )                                                               \
{                                                                   \
    CT_ASSERT(__is_ct_const(sync));                                 \
    _MEM_TCAM_INDEX_CHECK(index, sizeof(*tcam));                    \
    _MEM_TCAM_ENTRY_RANGE_CHECK(value, 0xff);                       \
    {                                                               \
        __xwrite uint32_t v;                                        \
        generic_ind_t ind;                                          \
        __addr40 __mem uint32_t *ptr = (__addr40 __mem uint32_t *)tcam; \
        v = (value << 24) >> ((index & 3) << 3);                    \
        ptr += (index >> 2);                                        \
        _INTRINSIC_OVERRIDE_BYTEMASK(ind, 8 >> (index & 3));        \
        cmd_mem_write_atomic_ind_ptr40(&v,(__addr40 __mem void*)ptr,1,ind,sync,sig_ptr); \
    }                                                               \
}

#define _MEM_TCAMNNN_SET8_ENTRY_VALUE_PTR32(nnn)                    \
__intrinsic                                                         \
void mem_tcam##nnn##_set8_entry_value_ptr32(                        \
    __mem mem_tcam##nnn##_in_mem_t *tcam,                           \
    uint32_t entry_index,                                           \
    uint32_t value,                                                 \
    sync_t sync,                                                    \
    SIGNAL *sig_ptr                                                 \
    )                                                               \
{                                                                   \
    CT_ASSERT(__is_ct_const(sync));                                 \
    _MEM_TCAM_INDEX_CHECK(entry_index, (sizeof(*tcam) / 2));        \
    _MEM_TCAM_ENTRY_RANGE_CHECK(value, 0xff);                       \
    {                                                               \
        __xwrite uint32_t v;                                        \
        generic_ind_t ind;                                          \
        __addr32 __mem uint32_t *ptr = (__addr32 __mem uint32_t *)tcam; \
        uint32_t word_index =                                       \
            ((entry_index << 1) & ~0x7) | (entry_index & 0x3);      \
        v = (value << 24) >> ((entry_index & 3) << 3);              \
        ptr += (word_index >> 2);                                   \
        _INTRINSIC_OVERRIDE_BYTEMASK(ind, 8 >> (entry_index & 3));  \
        cmd_mem_write_atomic_ind_ptr32(&v,(__addr32 __mem void*)ptr,1,ind,sync,sig_ptr);     \
    }                                                               \
}

#define _MEM_TCAMNNN_SET8_ENTRY_VALUE_PTR40(nnn)                    \
__intrinsic                                                         \
void mem_tcam##nnn##_set8_entry_value_ptr40(                        \
    mem_tcam##nnn##_ptr40_t tcam,                                   \
    uint32_t entry_index,                                           \
    uint32_t value,                                                 \
    sync_t sync,                                                    \
    SIGNAL *sig_ptr                                                 \
    )                                                               \
{                                                                   \
    CT_ASSERT(__is_ct_const(sync));                                 \
    _MEM_TCAM_INDEX_CHECK(entry_index, (sizeof(*tcam) / 2));        \
    _MEM_TCAM_ENTRY_RANGE_CHECK(value, 0xff);                       \
    {                                                               \
        __xwrite uint32_t v;                                        \
        generic_ind_t ind;                                          \
        __addr40 __mem uint32_t *ptr = (__addr40 __mem uint32_t *)tcam; \
        uint32_t word_index =                                       \
            ((entry_index << 1) & ~0x7) | (entry_index & 0x3);      \
        v = (value << 24) >> ((entry_index & 3) << 3);              \
        ptr += (word_index >> 2);                                   \
        _INTRINSIC_OVERRIDE_BYTEMASK(ind, 8 >> (entry_index & 3));  \
        cmd_mem_write_atomic_ind_ptr40(&v,(__addr40 __mem void*)ptr,1,ind,sync,sig_ptr); \
    }                                                               \
}

#define _MEM_TCAMNNN_SET8_ENTRY_MASK_PTR32(nnn)                     \
__intrinsic                                                         \
void mem_tcam##nnn##_set8_entry_mask_ptr32(                         \
    __mem mem_tcam##nnn##_in_mem_t *tcam,                           \
    uint32_t entry_index,                                           \
    uint32_t mask,                                                  \
    sync_t sync,                                                    \
    SIGNAL *sig_ptr                                                 \
    )                                                               \
{                                                                   \
    CT_ASSERT(__is_ct_const(sync));                                 \
    _MEM_TCAM_INDEX_CHECK(entry_index, (sizeof(*tcam) / 2));        \
    _MEM_TCAM_ENTRY_RANGE_CHECK(mask, 0xff);                        \
    {                                                               \
        __xwrite uint32_t v;                                        \
        generic_ind_t ind;                                          \
        __addr32 __mem uint32_t *ptr = (__addr32 __mem uint32_t *)tcam; \
        uint32_t word_index =                                       \
            ((entry_index << 1) & ~0x7) | (entry_index & 0x3) +     \
            4;                                                      \
        v = (mask << 24) >> ((entry_index & 3) << 3);               \
        ptr += (word_index >> 2);                                   \
        _INTRINSIC_OVERRIDE_BYTEMASK(ind, 8 >> (entry_index & 3));  \
        cmd_mem_write_atomic_ind_ptr32(&v,(__addr32 __mem void*)ptr,1,ind,sync,sig_ptr);     \
    }                                                               \
}

#define _MEM_TCAMNNN_SET8_ENTRY_MASK_PTR40(nnn)                     \
__intrinsic                                                         \
void mem_tcam##nnn##_set8_entry_mask_ptr40(                         \
    mem_tcam##nnn##_ptr40_t tcam,                                   \
    uint32_t entry_index,                                           \
    uint32_t mask,                                                  \
    sync_t sync,                                                    \
    SIGNAL *sig_ptr                                                 \
    )                                                               \
{                                                                   \
    CT_ASSERT(__is_ct_const(sync));                                 \
    _MEM_TCAM_INDEX_CHECK(entry_index, (sizeof(*tcam) / 2));        \
    _MEM_TCAM_ENTRY_RANGE_CHECK(mask, 0xff);                        \
    {                                                               \
        __xwrite uint32_t v;                                        \
        generic_ind_t ind;                                          \
        __addr40 __mem uint32_t *ptr = (__addr40 __mem uint32_t *)tcam; \
        uint32_t word_index =                                       \
            ((entry_index << 1) & ~0x7) | (entry_index & 0x3) +     \
            4;                                                      \
        v = (mask << 24) >> ((entry_index & 3) << 3);               \
        ptr += (word_index >> 2);                                   \
        _INTRINSIC_OVERRIDE_BYTEMASK(ind, 8 >> (entry_index & 3));  \
        cmd_mem_write_atomic_ind_ptr40(&v,(__addr40 __mem void*)ptr,1,ind,sync,sig_ptr);     \
    }                                                               \
}

#define _MEM_TCAMNNN_SET8_ENTRY_PTR32(nnn)                          \
__intrinsic                                                         \
void mem_tcam##nnn##_set8_entry_ptr32(                              \
    __mem mem_tcam##nnn##_in_mem_t *tcam,                           \
    uint32_t entry_index,                                           \
    uint32_t value,                                                 \
    uint32_t mask,                                                  \
    sync_t sync,                                                    \
    SIGNAL *sig_ptr                                                 \
    )                                                               \
{                                                                   \
    CT_ASSERT(__is_ct_const(sync));                                 \
    _MEM_TCAM_INDEX_CHECK(entry_index, (sizeof(*tcam) / 2));        \
    _MEM_TCAM_ENTRY_RANGE_CHECK(value, 0xff);                       \
    _MEM_TCAM_ENTRY_RANGE_CHECK(mask, 0xff);                        \
    {                                                               \
        __xwrite uint32_t v[2];                                     \
        generic_ind_t ind;                                          \
        __addr32 __mem uint32_t *ptr = (__addr32 __mem uint32_t *)tcam; \
        uint32_t word_index =                                       \
            ((entry_index << 1) & ~0x7) | (entry_index & 0x3);      \
        v[0] = (value << 24) >> ((entry_index & 3) << 3);           \
        v[1] = (mask << 24) >> ((entry_index & 3) << 3);            \
        ptr += (word_index >> 2);                                   \
        _INTRINSIC_OVERRIDE_BYTEMASK(ind, 0x88 >> (entry_index & 3)); \
        cmd_mem_write_atomic_ind_ptr32(&v,(__addr32 __mem void*)ptr,2,ind,sync,sig_ptr);     \
    }                                                               \
}

#define _MEM_TCAMNNN_SET8_ENTRY_PTR40(nnn)                          \
__intrinsic                                                         \
void mem_tcam##nnn##_set8_entry_ptr40(                              \
    mem_tcam##nnn##_ptr40_t tcam,                                   \
    uint32_t entry_index,                                           \
    uint32_t value,                                                 \
    uint32_t mask,                                                  \
    sync_t sync,                                                    \
    SIGNAL *sig_ptr                                                 \
    )                                                               \
{                                                                   \
    CT_ASSERT(__is_ct_const(sync));                                 \
    _MEM_TCAM_INDEX_CHECK(entry_index, (sizeof(*tcam) / 2));        \
    _MEM_TCAM_ENTRY_RANGE_CHECK(value, 0xff);                       \
    _MEM_TCAM_ENTRY_RANGE_CHECK(mask, 0xff);                        \
    {                                                               \
        __xwrite uint32_t v[2];                                     \
        generic_ind_t ind;                                          \
        __addr40 __mem uint32_t *ptr = (__addr40 __mem uint32_t *)tcam; \
        uint32_t word_index =                                       \
            ((entry_index << 1) & ~0x7) | (entry_index & 0x3);      \
        v[0] = (value << 24) >> ((entry_index & 3) << 3);           \
        v[1] = (mask << 24) >> ((entry_index & 3) << 3);            \
        ptr += (word_index >> 2);                                   \
        _INTRINSIC_OVERRIDE_BYTEMASK(ind, 0x88 >> (entry_index & 3)); \
        cmd_mem_write_atomic_ind_ptr40(&v,(__addr40 __mem void*)ptr,2,ind,sync,sig_ptr); \
    }                                                               \
}

_MEM_TCAMNNN_SET8_WORD_PTR32(128)
_MEM_TCAMNNN_SET8_ENTRY_VALUE_PTR32(128)
_MEM_TCAMNNN_SET8_ENTRY_MASK_PTR32(128)
_MEM_TCAMNNN_SET8_ENTRY_PTR32(128)
_MEM_TCAMNNN_SET8_WORD_PTR32(256)
_MEM_TCAMNNN_SET8_ENTRY_VALUE_PTR32(256)
_MEM_TCAMNNN_SET8_ENTRY_MASK_PTR32(256)
_MEM_TCAMNNN_SET8_ENTRY_PTR32(256)
_MEM_TCAMNNN_SET8_WORD_PTR32(384)
_MEM_TCAMNNN_SET8_ENTRY_VALUE_PTR32(384)
_MEM_TCAMNNN_SET8_ENTRY_MASK_PTR32(384)
_MEM_TCAMNNN_SET8_ENTRY_PTR32(384)
_MEM_TCAMNNN_SET8_WORD_PTR32(512)
_MEM_TCAMNNN_SET8_ENTRY_VALUE_PTR32(512)
_MEM_TCAMNNN_SET8_ENTRY_MASK_PTR32(512)
_MEM_TCAMNNN_SET8_ENTRY_PTR32(512)

_MEM_TCAMNNN_SET8_WORD_PTR40(128)
_MEM_TCAMNNN_SET8_ENTRY_VALUE_PTR40(128)
_MEM_TCAMNNN_SET8_ENTRY_MASK_PTR40(128)
_MEM_TCAMNNN_SET8_ENTRY_PTR40(128)
_MEM_TCAMNNN_SET8_WORD_PTR40(256)
_MEM_TCAMNNN_SET8_ENTRY_VALUE_PTR40(256)
_MEM_TCAMNNN_SET8_ENTRY_MASK_PTR40(256)
_MEM_TCAMNNN_SET8_ENTRY_PTR40(256)
_MEM_TCAMNNN_SET8_WORD_PTR40(384)
_MEM_TCAMNNN_SET8_ENTRY_VALUE_PTR40(384)
_MEM_TCAMNNN_SET8_ENTRY_MASK_PTR40(384)
_MEM_TCAMNNN_SET8_ENTRY_PTR40(384)
_MEM_TCAMNNN_SET8_WORD_PTR40(512)
_MEM_TCAMNNN_SET8_ENTRY_VALUE_PTR40(512)
_MEM_TCAMNNN_SET8_ENTRY_MASK_PTR40(512)
_MEM_TCAMNNN_SET8_ENTRY_PTR40(512)

#define _MEM_TCAMNNN_SET16_WORD_PTR32(nnn)                          \
__intrinsic                                                         \
void mem_tcam##nnn##_set16_word_ptr32(                              \
    __mem mem_tcam##nnn##_in_mem_t *tcam,                           \
    uint32_t index,                                                 \
    uint32_t value,                                                 \
    sync_t sync,                                                    \
    SIGNAL *sig_ptr                                                 \
    )                                                               \
{                                                                   \
    _MEM_TCAM_INDEX_CHECK(index, (sizeof(*tcam) / 2));              \
    _MEM_TCAM_ENTRY_RANGE_CHECK(value, 0xffff);                     \
    CT_ASSERT(__is_ct_const(sync));                                 \
    {                                                               \
        __xwrite uint32_t v;                                        \
        generic_ind_t ind;                                          \
        __addr32 __mem uint32_t *ptr = (__addr32 __mem uint32_t *)tcam;\
        v = (value << 16) >> ((index & 1) << 4);                    \
        ptr += (index >> 1);                                        \
        _INTRINSIC_OVERRIDE_BYTEMASK(ind, 0x0c >> ((index & 1) << 1));    \
        cmd_mem_write_atomic_ind_ptr32(&v,(__addr32 __mem void*)ptr,1,ind,sync,sig_ptr);     \
    }                                                               \
}

#define _MEM_TCAMNNN_SET16_WORD_PTR40(nnn)                          \
__intrinsic                                                         \
void mem_tcam##nnn##_set16_word_ptr40(                              \
    mem_tcam##nnn##_ptr40_t tcam,                                   \
    uint32_t index,                                                 \
    uint32_t value,                                                 \
    sync_t sync,                                                    \
    SIGNAL *sig_ptr                                                 \
    )                                                               \
{                                                                   \
    _MEM_TCAM_INDEX_CHECK(index, (sizeof(*tcam) / 2));              \
    _MEM_TCAM_ENTRY_RANGE_CHECK(value, 0xffff);                     \
    CT_ASSERT(__is_ct_const(sync));                                 \
    {                                                               \
        __xwrite uint32_t v;                                        \
        generic_ind_t ind;                                          \
        __addr40 __mem uint32_t *ptr = (__addr40 __mem uint32_t *)tcam; \
        v = (value << 16) >> ((index & 1) << 4);                    \
        ptr += (index >> 1);                                        \
       _INTRINSIC_OVERRIDE_BYTEMASK(ind, 0x0c >> ((index & 1) << 1));    \
        cmd_mem_write_atomic_ind_ptr40(&v,(__addr40 __mem void*)ptr,1,ind,sync,sig_ptr); \
    }                                                               \
}

#define _MEM_TCAMNNN_SET16_ENTRY_VALUE_PTR32(nnn)                   \
__intrinsic                                                         \
void mem_tcam##nnn##_set16_entry_value_ptr32(                       \
    __mem mem_tcam##nnn##_in_mem_t *tcam,                           \
    uint32_t entry_index,                                           \
    uint32_t value,                                                 \
    sync_t sync,                                                    \
    SIGNAL *sig_ptr                                                 \
    )                                                               \
{                                                                   \
    _MEM_TCAM_INDEX_CHECK(entry_index, (sizeof(*tcam) / 4));        \
    _MEM_TCAM_ENTRY_RANGE_CHECK(value, 0xffff);                     \
    CT_ASSERT(__is_ct_const(sync));                                 \
    {                                                               \
        __xwrite uint32_t v;                                        \
        generic_ind_t ind;                                          \
        __addr32 __mem uint32_t *ptr = (__addr32 __mem uint32_t *)tcam; \
        uint32_t word_index =                                       \
            ((entry_index << 1) & ~0x3) | (entry_index & 0x1);      \
        v = (value << 16) >> ((entry_index & 1) << 4);              \
        ptr += (word_index >> 1);                                   \
        _INTRINSIC_OVERRIDE_BYTEMASK(ind, 0x0c >> ((entry_index & 1) << 1));    \
        cmd_mem_write_atomic_ind_ptr32(&v,(__addr32 __mem void*)ptr,1,ind,sync,sig_ptr);     \
    }                                                               \
}

#define _MEM_TCAMNNN_SET16_ENTRY_VALUE_PTR40(nnn)                   \
__intrinsic                                                         \
void mem_tcam##nnn##_set16_entry_value_ptr40(                       \
    mem_tcam##nnn##_ptr40_t tcam,                                   \
    uint32_t entry_index,                                           \
    uint32_t value,                                                 \
    sync_t sync,                                                    \
    SIGNAL *sig_ptr                                                 \
    )                                                               \
{                                                                   \
    _MEM_TCAM_INDEX_CHECK(entry_index, (sizeof(*tcam) / 4));        \
    _MEM_TCAM_ENTRY_RANGE_CHECK(value, 0xffff);                     \
    CT_ASSERT(__is_ct_const(sync));                                 \
    {                                                               \
        __xwrite uint32_t v;                                        \
        generic_ind_t ind;                                          \
        __addr40 __mem uint32_t *ptr = (__addr40 __mem uint32_t *)tcam; \
        uint32_t word_index =                                       \
            ((entry_index << 1) & ~0x3) | (entry_index & 0x1);      \
        v = (value << 16) >> ((entry_index & 1) << 4);              \
        ptr += (word_index >> 1);                                   \
        _INTRINSIC_OVERRIDE_BYTEMASK(ind, 0x0c >> ((entry_index & 1) << 1));    \
        cmd_mem_write_atomic_ind_ptr40(&v,(__addr40 __mem void*)ptr,1,ind,sync,sig_ptr); \
    }                                                               \
}

#define _MEM_TCAMNNN_SET16_ENTRY_MASK_PTR32(nnn)                    \
__intrinsic                                                         \
void mem_tcam##nnn##_set16_entry_mask_ptr32(                        \
    __mem mem_tcam##nnn##_in_mem_t *tcam,                           \
    uint32_t entry_index,                                           \
    uint32_t mask,                                                  \
    sync_t sync,                                                    \
    SIGNAL *sig_ptr                                                 \
    )                                                               \
{                                                                   \
    _MEM_TCAM_INDEX_CHECK(entry_index, (sizeof(*tcam) / 4));        \
    _MEM_TCAM_ENTRY_RANGE_CHECK(mask, 0xffff);                      \
    CT_ASSERT(__is_ct_const(sync));                                 \
    {                                                               \
        __xwrite uint32_t v;                                        \
        generic_ind_t ind;                                          \
        __addr32 __mem uint32_t *ptr = (__addr32 __mem uint32_t *)tcam;\
        uint32_t word_index =                                       \
            ((entry_index << 1) & ~0x3) | (entry_index & 0x1) +     \
            2;                                                      \
        v = (mask << 16) >> ((entry_index & 1) << 4);               \
        ptr += (word_index >> 1);                                   \
        _INTRINSIC_OVERRIDE_BYTEMASK(ind, 0x0c >> ((entry_index & 1) << 1));    \
        cmd_mem_write_atomic_ind_ptr32(&v,(__addr32 __mem void*)ptr,1,ind,sync,sig_ptr);     \
    }                                                               \
}

#define _MEM_TCAMNNN_SET16_ENTRY_MASK_PTR40(nnn)                    \
__intrinsic                                                         \
void mem_tcam##nnn##_set16_entry_mask_ptr40(                        \
    mem_tcam##nnn##_ptr40_t tcam,                                   \
    uint32_t entry_index,                                           \
    uint32_t mask,                                                  \
    sync_t sync,                                                    \
    SIGNAL *sig_ptr                                                 \
    )                                                               \
{                                                                   \
    _MEM_TCAM_INDEX_CHECK(entry_index, (sizeof(*tcam) / 4));        \
    _MEM_TCAM_ENTRY_RANGE_CHECK(mask, 0xffff);                      \
    CT_ASSERT(__is_ct_const(sync));                                 \
    {                                                               \
        __xwrite uint32_t v;                                        \
        generic_ind_t ind;                                          \
        __addr40 __mem uint32_t *ptr = (__addr40 __mem uint32_t *)tcam; \
        uint32_t word_index =                                       \
            ((entry_index << 1) & ~0x3) | (entry_index & 0x1) +     \
            2;                                                      \
        v = (mask << 16) >> ((entry_index & 1) << 4);               \
        ptr += (word_index >> 1);                                   \
        _INTRINSIC_OVERRIDE_BYTEMASK(ind, 0x0c >> ((entry_index & 1) << 1));    \
        cmd_mem_write_atomic_ind_ptr40(&v,(__addr40 __mem void*)ptr,1,ind,sync,sig_ptr); \
    }                                                               \
}

#define _MEM_TCAMNNN_SET16_ENTRY_PTR32(nnn)                         \
__intrinsic                                                         \
void mem_tcam##nnn##_set16_entry_ptr32(                             \
    __mem mem_tcam##nnn##_in_mem_t *tcam,                           \
    uint32_t entry_index,                                           \
    uint32_t value,                                                 \
    uint32_t mask,                                                  \
    sync_t sync,                                                    \
    SIGNAL *sig_ptr                                                 \
    )                                                               \
{                                                                   \
    _MEM_TCAM_INDEX_CHECK(entry_index, (sizeof(*tcam) / 4));        \
    _MEM_TCAM_ENTRY_RANGE_CHECK(value, 0xffff);                     \
    _MEM_TCAM_ENTRY_RANGE_CHECK(mask, 0xffff);                      \
    CT_ASSERT(__is_ct_const(sync));                                 \
    {                                                               \
        __xwrite uint32_t v[2];                                     \
        generic_ind_t ind;                                          \
        __addr32 __mem uint32_t *ptr = (__addr32 __mem uint32_t *)tcam; \
        uint32_t word_index =                                       \
            ((entry_index << 1) & ~0x3) | (entry_index & 0x1);      \
        v[0] = (value << 16) >> ((entry_index & 1) << 4);           \
        v[1] = (mask << 16) >> ((entry_index & 1) << 4);            \
        ptr += (word_index >> 1);                                   \
        _INTRINSIC_OVERRIDE_BYTEMASK(ind, 0xcc >> ((entry_index & 1) << 1));    \
        cmd_mem_write_atomic_ind_ptr32(&v,(__addr32 __mem void*)ptr,2,ind,sync,sig_ptr);     \
    }                                                               \
}

#define _MEM_TCAMNNN_SET16_ENTRY_PTR40(nnn)                         \
__intrinsic                                                         \
void mem_tcam##nnn##_set16_entry_ptr40(                             \
    mem_tcam##nnn##_ptr40_t tcam,                                   \
    uint32_t entry_index,                                           \
    uint32_t value,                                                 \
    uint32_t mask,                                                  \
    sync_t sync,                                                    \
    SIGNAL *sig_ptr                                                 \
    )                                                               \
{                                                                   \
    _MEM_TCAM_INDEX_CHECK(entry_index, (sizeof(*tcam) / 4));        \
    _MEM_TCAM_ENTRY_RANGE_CHECK(value, 0xffff);                     \
    _MEM_TCAM_ENTRY_RANGE_CHECK(mask, 0xffff);                      \
    CT_ASSERT(__is_ct_const(sync));                                 \
    {                                                               \
        __xwrite uint32_t v[2];                                     \
        generic_ind_t ind;                                          \
        __addr40 __mem uint32_t *ptr = (__addr40 __mem uint32_t *)tcam; \
        uint32_t word_index =                                       \
            ((entry_index << 1) & ~0x3) | (entry_index & 0x1);      \
        v[0] = (value << 16) >> ((entry_index & 1) << 4);           \
        v[1] = (mask << 16) >> ((entry_index & 1) << 4);            \
        ptr += (word_index >> 1);                                   \
        _INTRINSIC_OVERRIDE_BYTEMASK(ind, 0xcc >> ((entry_index & 1) << 1));    \
        cmd_mem_write_atomic_ind_ptr40(&v,(__addr40 __mem void*)ptr,2,ind,sync,sig_ptr); \
    }                                                               \
}

_MEM_TCAMNNN_SET16_WORD_PTR32(128)
_MEM_TCAMNNN_SET16_ENTRY_VALUE_PTR32(128)
_MEM_TCAMNNN_SET16_ENTRY_MASK_PTR32(128)
_MEM_TCAMNNN_SET16_ENTRY_PTR32(128)
_MEM_TCAMNNN_SET16_WORD_PTR32(256)
_MEM_TCAMNNN_SET16_ENTRY_VALUE_PTR32(256)
_MEM_TCAMNNN_SET16_ENTRY_MASK_PTR32(256)
_MEM_TCAMNNN_SET16_ENTRY_PTR32(256)
_MEM_TCAMNNN_SET16_WORD_PTR32(384)
_MEM_TCAMNNN_SET16_ENTRY_VALUE_PTR32(384)
_MEM_TCAMNNN_SET16_ENTRY_MASK_PTR32(384)
_MEM_TCAMNNN_SET16_ENTRY_PTR32(384)
_MEM_TCAMNNN_SET16_WORD_PTR32(512)
_MEM_TCAMNNN_SET16_ENTRY_VALUE_PTR32(512)
_MEM_TCAMNNN_SET16_ENTRY_MASK_PTR32(512)
_MEM_TCAMNNN_SET16_ENTRY_PTR32(512)

_MEM_TCAMNNN_SET16_WORD_PTR40(128)
_MEM_TCAMNNN_SET16_ENTRY_VALUE_PTR40(128)
_MEM_TCAMNNN_SET16_ENTRY_MASK_PTR40(128)
_MEM_TCAMNNN_SET16_ENTRY_PTR40(128)
_MEM_TCAMNNN_SET16_WORD_PTR40(256)
_MEM_TCAMNNN_SET16_ENTRY_VALUE_PTR40(256)
_MEM_TCAMNNN_SET16_ENTRY_MASK_PTR40(256)
_MEM_TCAMNNN_SET16_ENTRY_PTR40(256)
_MEM_TCAMNNN_SET16_WORD_PTR40(384)
_MEM_TCAMNNN_SET16_ENTRY_VALUE_PTR40(384)
_MEM_TCAMNNN_SET16_ENTRY_MASK_PTR40(384)
_MEM_TCAMNNN_SET16_ENTRY_PTR40(384)
_MEM_TCAMNNN_SET16_WORD_PTR40(512)
_MEM_TCAMNNN_SET16_ENTRY_VALUE_PTR40(512)
_MEM_TCAMNNN_SET16_ENTRY_MASK_PTR40(512)
_MEM_TCAMNNN_SET16_ENTRY_PTR40(512)

#define _MEM_TCAMNNN_SETMM_WORD_PTR32(nnn,mm)                       \
__intrinsic                                                         \
void mem_tcam##nnn##_set##mm##_word_ptr32(                          \
    __mem mem_tcam##nnn##_in_mem_t *tcam,                           \
    uint32_t index,                                                 \
    uint32_t value,                                                 \
    sync_t sync,                                                    \
    SIGNAL *sig_ptr                                                 \
    )                                                               \
{                                                                   \
    _MEM_TCAM_INDEX_CHECK(index, (sizeof(*tcam) / 4));              \
    CT_ASSERT(__is_ct_const(sync));                                 \
    {                                                               \
        __xwrite uint32_t v = value;                                \
        __addr32 __mem uint32_t *ptr = (__addr32 __mem uint32_t*)tcam + index;            \
        cmd_mem_write_atomic_ptr32(&v,(__addr32 __mem void*)ptr,1,sync,sig_ptr);             \
    }                                                               \
}

#define _MEM_TCAMNNN_SETMM_WORD_PTR40(nnn,mm)                       \
__intrinsic                                                         \
void mem_tcam##nnn##_set##mm##_word_ptr40(                          \
    mem_tcam##nnn##_ptr40_t tcam,                                   \
    uint32_t index,                                                 \
    uint32_t value,                                                 \
    sync_t sync,                                                    \
    SIGNAL *sig_ptr                                                 \
    )                                                               \
{                                                                   \
    _MEM_TCAM_INDEX_CHECK(index, (sizeof(*tcam) / 4));              \
    CT_ASSERT(__is_ct_const(sync));                                 \
    {                                                               \
        __xwrite uint32_t v = value;                                \
        __addr40 __mem uint32_t *ptr = (__addr40 __mem uint32_t*)tcam + index; \
        cmd_mem_write_atomic_ptr40(&v,(__addr40 __mem void*)ptr,1,sync,sig_ptr);             \
    }                                                               \
}

#define _MEM_TCAMNNN_SETMM_ENTRY_VALUE_PTR32(nnn,mm)                \
__intrinsic                                                         \
void mem_tcam##nnn##_set##mm##_entry_value_ptr32(                   \
    __mem mem_tcam##nnn##_in_mem_t *tcam,                           \
    uint32_t entry_index,                                           \
    uint32_t value,                                                 \
    sync_t sync,                                                    \
    SIGNAL *sig_ptr                                                 \
    )                                                               \
{                                                                   \
    _MEM_TCAM_INDEX_CHECK(entry_index, (sizeof(*tcam) / 8));        \
    CT_ASSERT(__is_ct_const(sync));                                 \
    {                                                               \
        uint32_t word_index = (entry_index << 1);                   \
        __xwrite uint32_t v = value;                                \
        __addr32 __mem uint32_t *ptr = (__addr32 __mem uint32_t*)tcam + word_index;       \
        cmd_mem_write_atomic_ptr32(&v,(__addr32 __mem void*)ptr,1,sync,sig_ptr);             \
    }                                                               \
}

#define _MEM_TCAMNNN_SETMM_ENTRY_VALUE_PTR40(nnn,mm)                \
__intrinsic                                                         \
void mem_tcam##nnn##_set##mm##_entry_value_ptr40(                   \
    mem_tcam##nnn##_ptr40_t tcam,                                   \
    uint32_t entry_index,                                           \
    uint32_t value,                                                 \
    sync_t sync,                                                    \
    SIGNAL *sig_ptr                                                 \
    )                                                               \
{                                                                   \
    _MEM_TCAM_INDEX_CHECK(entry_index, (sizeof(*tcam) / 8));        \
    CT_ASSERT(__is_ct_const(sync));                                 \
    {                                                               \
        uint32_t word_index = (entry_index << 1);                   \
        __xwrite uint32_t v = value;                                \
        __addr40 __mem uint32_t *ptr = (__addr40 __mem uint32_t*)tcam + word_index; \
        cmd_mem_write_atomic_ptr40(&v,(__addr40 __mem void*)ptr,1,sync,sig_ptr); \
    }                                                               \
}

#define _MEM_TCAMNNN_SETMM_ENTRY_MASK_PTR32(nnn,mm)                 \
__intrinsic                                                         \
void mem_tcam##nnn##_set##mm##_entry_mask_ptr32(                    \
    __mem mem_tcam##nnn##_in_mem_t *tcam,                           \
    uint32_t entry_index,                                           \
    uint32_t mask,                                                  \
    sync_t sync,                                                    \
    SIGNAL *sig_ptr                                                 \
    )                                                               \
{                                                                   \
    _MEM_TCAM_INDEX_CHECK(entry_index, (sizeof(*tcam) / 8));        \
    CT_ASSERT(__is_ct_const(sync));                                 \
    {                                                               \
        uint32_t word_index = (entry_index << 1) + 1;               \
        __xwrite uint32_t v = mask;                \
        __addr32 __mem uint32_t *ptr = (__addr32 __mem uint32_t*)tcam + word_index;       \
        cmd_mem_write_atomic_ptr32(&v,(__addr32 __mem void*)ptr,1,sync,sig_ptr);             \
    }                                                               \
}

#define _MEM_TCAMNNN_SETMM_ENTRY_MASK_PTR40(nnn,mm)                 \
__intrinsic                                                         \
void mem_tcam##nnn##_set##mm##_entry_mask_ptr40(                    \
    mem_tcam##nnn##_ptr40_t tcam,                                   \
    uint32_t entry_index,                                           \
    uint32_t mask,                                                  \
    sync_t sync,                                                    \
    SIGNAL *sig_ptr                                                 \
    )                                                               \
{                                                                   \
    _MEM_TCAM_INDEX_CHECK(entry_index, (sizeof(*tcam) / 8));        \
    CT_ASSERT(__is_ct_const(sync));                                 \
    {                                                               \
        uint32_t word_index = (entry_index << 1) + 1;               \
        __xwrite uint32_t v = mask;                \
        __addr40 __mem uint32_t *ptr = (__addr40 __mem uint32_t*)tcam + word_index; \
        cmd_mem_write_atomic_ptr40(&v,(__addr40 __mem void*)ptr,1,sync,sig_ptr); \
    }                                                               \
}

#define _MEM_TCAMNNN_SETMM_ENTRY_PTR32(nnn,mm)                      \
__intrinsic                                                         \
void mem_tcam##nnn##_set##mm##_entry_ptr32(                         \
    __mem mem_tcam##nnn##_in_mem_t *tcam,                           \
    uint32_t entry_index,                                           \
    uint32_t value,                                                 \
    uint32_t mask,                                                  \
    sync_t sync,                                                    \
    SIGNAL *sig_ptr                                                 \
    )                                                               \
{                                                                   \
    _MEM_TCAM_INDEX_CHECK(entry_index, (sizeof(*tcam) / 8));        \
    CT_ASSERT(__is_ct_const(sync));                                 \
    {                                                               \
        __xwrite uint32_t v[2];                                     \
        __addr32 __mem uint32_t *ptr;                               \
        uint32_t word_index = (entry_index << 1);                   \
        v[0] = value;                                               \
        v[1] = mask;                                                \
        ptr = (__addr32 __mem uint32_t*)tcam + word_index; \
        cmd_mem_write_atomic_ptr32(&v,(__addr32 __mem void*)ptr,2,sync,sig_ptr);             \
    }                                                               \
}

#define _MEM_TCAMNNN_SETMM_ENTRY_PTR40(nnn,mm)                      \
__intrinsic                                                         \
void mem_tcam##nnn##_set##mm##_entry_ptr40(                         \
    mem_tcam##nnn##_ptr40_t tcam,                                   \
    uint32_t entry_index,                                           \
    uint32_t value,                                                 \
    uint32_t mask,                                                  \
    sync_t sync,                                                    \
    SIGNAL *sig_ptr                                                 \
    )                                                               \
{                                                                   \
    _MEM_TCAM_INDEX_CHECK(entry_index, (sizeof(*tcam) / 8));        \
    CT_ASSERT(__is_ct_const(sync));                                 \
    {                                                               \
        __xwrite uint32_t v[2];                                     \
        __addr40 __mem uint32_t *ptr;                               \
        uint32_t word_index = (entry_index << 1);                   \
        v[0] = value;                                               \
        v[1] = mask;                                                \
        ptr = (__addr40 __mem uint32_t*)tcam + word_index; \
        cmd_mem_write_atomic_ptr40(&v,(__addr40 __mem void*)ptr,2,sync,sig_ptr);             \
    }                                                               \
}


_MEM_TCAMNNN_SETMM_WORD_PTR32(128, 32)
_MEM_TCAMNNN_SETMM_ENTRY_VALUE_PTR32(128, 32)
_MEM_TCAMNNN_SETMM_ENTRY_MASK_PTR32(128, 32)
_MEM_TCAMNNN_SETMM_ENTRY_PTR32(128, 32)
_MEM_TCAMNNN_SETMM_WORD_PTR32(256, 32)
_MEM_TCAMNNN_SETMM_ENTRY_VALUE_PTR32(256, 32)
_MEM_TCAMNNN_SETMM_ENTRY_MASK_PTR32(256, 32)
_MEM_TCAMNNN_SETMM_ENTRY_PTR32(256, 32)
_MEM_TCAMNNN_SETMM_WORD_PTR32(384, 32)
_MEM_TCAMNNN_SETMM_ENTRY_VALUE_PTR32(384, 32)
_MEM_TCAMNNN_SETMM_ENTRY_MASK_PTR32(384, 32)
_MEM_TCAMNNN_SETMM_ENTRY_PTR32(384, 32)
_MEM_TCAMNNN_SETMM_WORD_PTR32(512, 32)
_MEM_TCAMNNN_SETMM_ENTRY_VALUE_PTR32(512, 32)
_MEM_TCAMNNN_SETMM_ENTRY_MASK_PTR32(512, 32)
_MEM_TCAMNNN_SETMM_ENTRY_PTR32(512, 32)

_MEM_TCAMNNN_SETMM_WORD_PTR40(128, 32)
_MEM_TCAMNNN_SETMM_ENTRY_VALUE_PTR40(128, 32)
_MEM_TCAMNNN_SETMM_ENTRY_MASK_PTR40(128, 32)
_MEM_TCAMNNN_SETMM_ENTRY_PTR40(128, 32)
_MEM_TCAMNNN_SETMM_WORD_PTR40(256, 32)
_MEM_TCAMNNN_SETMM_ENTRY_VALUE_PTR40(256, 32)
_MEM_TCAMNNN_SETMM_ENTRY_MASK_PTR40(256, 32)
_MEM_TCAMNNN_SETMM_ENTRY_PTR40(256, 32)
_MEM_TCAMNNN_SETMM_WORD_PTR40(384, 32)
_MEM_TCAMNNN_SETMM_ENTRY_VALUE_PTR40(384, 32)
_MEM_TCAMNNN_SETMM_ENTRY_MASK_PTR40(384, 32)
_MEM_TCAMNNN_SETMM_ENTRY_PTR40(384, 32)
_MEM_TCAMNNN_SETMM_WORD_PTR40(512, 32)
_MEM_TCAMNNN_SETMM_ENTRY_VALUE_PTR40(512, 32)
_MEM_TCAMNNN_SETMM_ENTRY_MASK_PTR40(512, 32)
_MEM_TCAMNNN_SETMM_ENTRY_PTR40(512, 32)

#define _MEM_TCAM128_256_LOOKUP_PTR32(mmm,nn)                                               \
__intrinsic                                                                                 \
__xread mem_tcam_lookup##nn##_out_t *mem_tcam##mmm##_lookup##nn##_ptr32(                    \
    __mem mem_tcam##mmm##_in_mem_t *tcam,                                                   \
    __xwrite mem_tcam_lookup##nn##_in_t *xfer,                                              \
    sync_t sync,                                                                            \
    SIGNAL_PAIR *sig_pair_ptr                                                               \
    )                                                                                       \
{                                                                                           \
    _INTRINSIC_BEGIN;                                                                       \
    {                                                                                       \
        CT_ASSERT(__is_write_reg(xfer));                                                    \
        CT_ASSERT(__is_ct_const(sync));                                                     \
        CT_ASSERT(sync == sig_done);                                                        \
        __asm                                                                               \
        {                                                                                   \
            mem[tcam##mmm##_lookup##nn##, *xfer, tcam, 0], sig_done[*sig_pair_ptr]          \
        }                                                                                   \
    }                                                                                       \
    return (__xread mem_tcam_lookup##nn##_out_t *)(xfer);                                   \
    _INTRINSIC_END;                                                                         \
}

#define _MEM_TCAM128_256_LOOKUP_PTR40(mmm,nn)                                               \
__intrinsic                                                                                 \
__xread mem_tcam_lookup##nn##_out_t *mem_tcam##mmm##_lookup##nn##_ptr40(                    \
    mem_tcam##mmm##_ptr40_t tcam,                                                           \
    __xwrite mem_tcam_lookup##nn##_in_t *xfer,                                              \
    sync_t sync,                                                                            \
    SIGNAL_PAIR *sig_pair_ptr                                                               \
    )                                                                                       \
{                                                                                           \
    _INTRINSIC_BEGIN;                                                                       \
    {                                                                                       \
        uint32_t hi_addr, low_addr;                                                         \
        CT_ASSERT(__is_ct_const(sync));                                                     \
        CT_ASSERT(sync == sig_done);                                                        \
        _INTRINSIC_CONVERT_HI_LO_ADDRESS(tcam);                                             \
        CT_ASSERT(__is_write_reg(xfer));                                                    \
        __asm                                                                               \
        {                                                                                   \
            mem[tcam##mmm##_lookup##nn##, *xfer, hi_addr, <<8, low_addr], sig_done[*sig_pair_ptr] \
        }                                                                                   \
    }                                                                                       \
    return (__xread mem_tcam_lookup##nn##_out_t *)(xfer);                                   \
    _INTRINSIC_END;                                                                         \
}

enum
{
    TCAM_REF_COUNT_128_8 = 0,
    TCAM_REF_COUNT_128_16,
    TCAM_REF_COUNT_128_24,
    TCAM_REF_COUNT_128_32,

    TCAM_REF_COUNT_256_8 = 4,
    TCAM_REF_COUNT_256_16,
    TCAM_REF_COUNT_256_24,
    TCAM_REF_COUNT_256_32,

    TCAM_REF_COUNT_384_8 = 8,
    TCAM_REF_COUNT_384_16,
    TCAM_REF_COUNT_384_24,
    TCAM_REF_COUNT_384_32,

    TCAM_REF_COUNT_512_8 = 12,
    TCAM_REF_COUNT_512_16,
    TCAM_REF_COUNT_512_24,
    TCAM_REF_COUNT_512_32
};

#define _MEM_TCAM384_512_LOOKUP_PTR32(mmm,nn)                                                   \
__intrinsic                                                                                     \
__xread mem_tcam_lookup##nn##_out_t *mem_tcam##mmm##_lookup##nn##_ptr32(                        \
    __mem mem_tcam##mmm##_in_mem_t *tcam,                                                       \
    __xwrite mem_tcam_lookup##nn##_in_t *xfer,                                                  \
    sync_t sync,                                                                               \
    SIGNAL_PAIR *sig_pair_ptr                                                                  \
    )                                                                                          \
{                                                                                              \
    _INTRINSIC_BEGIN;                                                                          \
    {                                                                                          \
        generic_ind_t ind;                                                                     \
        CT_ASSERT(__is_write_reg(xfer));                                                       \
        CT_ASSERT(__is_ct_const(sync));                                                        \
        CT_ASSERT(sync == sig_done);                                                           \
        _INTRINSIC_OVERRIDE_LENGTH(ind, TCAM_REF_COUNT_##mmm##_##nn);                          \
        __asm alu[--, --, B, ALU_INDIRECT_TYPE(ind)]                                           \
        __asm mem[tcam##mmm##_lookup##nn##, *xfer, tcam, 0], sig_done[*sig_pair_ptr], indirect_ref          \
    }                                                                                          \
    return (__xread mem_tcam_lookup##nn##_out_t *)(xfer);                                       \
    _INTRINSIC_END;                                                                            \
}

#define _MEM_TCAM384_512_LOOKUP_PTR40(mmm,nn)                                                  \
__intrinsic                                                                                    \
__xread mem_tcam_lookup##nn##_out_t *mem_tcam##mmm##_lookup##nn##_ptr40(                        \
    mem_tcam##mmm##_ptr40_t tcam,                                                              \
    __xwrite mem_tcam_lookup##nn##_in_t *xfer,                                                  \
    sync_t sync,                                                                               \
    SIGNAL_PAIR *sig_pair_ptr                                                                  \
    )                                                                                          \
{                                                                                              \
    _INTRINSIC_BEGIN;                                                                          \
    {                                                                                          \
        generic_ind_t ind;                                                                     \
        uint32_t hi_addr, low_addr;                                                             \
        CT_ASSERT(__is_write_reg(xfer));                                                       \
        CT_ASSERT(__is_ct_const(sync));                                                        \
        CT_ASSERT(sync == sig_done);                                                           \
        _INTRINSIC_CONVERT_HI_LO_ADDRESS(tcam);                                                \
        _INTRINSIC_OVERRIDE_LENGTH(ind, TCAM_REF_COUNT_##mmm##_##nn);                          \
        __asm alu[--, --, B, ALU_INDIRECT_TYPE(ind)]                                           \
        __asm mem[tcam##mmm##_lookup##nn##, *xfer, hi_addr, <<8, low_addr], sig_done[*sig_pair_ptr], indirect_ref \
    }                                                                                          \
    return (__xread mem_tcam_lookup##nn##_out_t *)(xfer);                                       \
    _INTRINSIC_END;                                                                            \
}

_MEM_TCAM128_256_LOOKUP_PTR32(128, 8)
_MEM_TCAM128_256_LOOKUP_PTR32(256, 8)
_MEM_TCAM384_512_LOOKUP_PTR32(384, 8)
_MEM_TCAM384_512_LOOKUP_PTR32(512, 8)

_MEM_TCAM128_256_LOOKUP_PTR32(128, 16)
_MEM_TCAM128_256_LOOKUP_PTR32(256, 16)
_MEM_TCAM384_512_LOOKUP_PTR32(384, 16)
_MEM_TCAM384_512_LOOKUP_PTR32(512, 16)

_MEM_TCAM128_256_LOOKUP_PTR32(128, 24)
_MEM_TCAM128_256_LOOKUP_PTR32(256, 24)
_MEM_TCAM384_512_LOOKUP_PTR32(384, 24)
_MEM_TCAM384_512_LOOKUP_PTR32(512, 24)

_MEM_TCAM128_256_LOOKUP_PTR32(128, 32)
_MEM_TCAM128_256_LOOKUP_PTR32(256, 32)
_MEM_TCAM384_512_LOOKUP_PTR32(384, 32)
_MEM_TCAM384_512_LOOKUP_PTR32(512, 32)

_MEM_TCAM128_256_LOOKUP_PTR40(128, 8)
_MEM_TCAM128_256_LOOKUP_PTR40(256, 8)
_MEM_TCAM384_512_LOOKUP_PTR40(384, 8)
_MEM_TCAM384_512_LOOKUP_PTR40(512, 8)

_MEM_TCAM128_256_LOOKUP_PTR40(128, 16)
_MEM_TCAM128_256_LOOKUP_PTR40(256, 16)
_MEM_TCAM384_512_LOOKUP_PTR40(384, 16)
_MEM_TCAM384_512_LOOKUP_PTR40(512, 16)

_MEM_TCAM128_256_LOOKUP_PTR40(128, 24)
_MEM_TCAM128_256_LOOKUP_PTR40(256, 24)
_MEM_TCAM384_512_LOOKUP_PTR40(384, 24)
_MEM_TCAM384_512_LOOKUP_PTR40(512, 24)

_MEM_TCAM128_256_LOOKUP_PTR40(128, 32)
_MEM_TCAM128_256_LOOKUP_PTR40(256, 32)
_MEM_TCAM384_512_LOOKUP_PTR40(384, 32)
_MEM_TCAM384_512_LOOKUP_PTR40(512, 32)

/*
 * 8 bit tcam words
 */
#define _MEM_TCAM_WORD_TO_ENTRY_INDEX8                                          \
__intrinsic                                                                     \
uint32_t mem_tcam_word_to_entry_index8(uint32_t word_idx)                       \
{                                                                               \
    return ((word_idx >> 1) & ~0x3) | (word_idx & 0x3);                         \
}

#define _MEM_TCAM_ENTRY_VALUE_TO_WORD_INDEX8                                    \
__intrinsic                                                                     \
uint32_t mem_tcam_entry_value_to_word_index8(                                   \
    uint32_t entry_idx)                                                         \
{                                                                               \
    return ((entry_idx << 1) & ~0x7) | (entry_idx & 0x3);                       \
}

#define _MEM_TCAM_ENTRY_MASK_TO_WORD_INDEX8                                     \
__intrinsic                                                                     \
uint32_t mem_tcam_entry_mask_to_word_index8(                                    \
    uint32_t entry_idx)                                                         \
{                                                                               \
    return (((entry_idx << 1) & ~0x7) | (entry_idx & 0x3)) + 4;                 \
}

/*
 * 16 bit tcam words
 */
#define _MEM_TCAM_WORD_TO_ENTRY_INDEX16                                         \
__intrinsic                                                                     \
uint32_t mem_tcam_word_to_entry_index16(uint32_t word_idx)                      \
{                                                                               \
    return ((word_idx >> 1) & ~0x1) | (word_idx & 0x1);                         \
}

#define _MEM_TCAM_ENTRY_VALUE_TO_WORD_INDEX16                                   \
__intrinsic                                                                     \
uint32_t mem_tcam_entry_value_to_word_index16(                                  \
    uint32_t entry_idx)                                                         \
{                                                                               \
    return ((entry_idx << 1) & ~0x3) | (entry_idx & 0x1);                       \
}

#define _MEM_TCAM_ENTRY_MASK_TO_WORD_INDEX16                                    \
__intrinsic                                                                     \
uint32_t mem_tcam_entry_mask_to_word_index16(                                   \
    uint32_t entry_idx)                                                         \
{                                                                               \
    return (((entry_idx << 1) & ~0x3) | (entry_idx & 0x1)) + 4;                 \
}

/*
 * 32 bit tcam words
 */
#define _MEM_TCAM_WORD_TO_ENTRY_INDEX32                                         \
__intrinsic                                                                     \
uint32_t mem_tcam_word_to_entry_index32(uint32_t word_idx)                      \
{                                                                               \
    return (word_idx >> 1);                                                     \
}

#define _MEM_TCAM_ENTRY_VALUE_TO_WORD_INDEX32                                   \
__intrinsic                                                                     \
uint32_t mem_tcam_entry_value_to_word_index32(                                  \
    uint32_t entry_idx)                                                         \
{                                                                               \
    return (entry_idx << 1);                                                    \
}

#define _MEM_TCAM_ENTRY_MASK_TO_WORD_INDEX32                                    \
__intrinsic                                                                     \
uint32_t mem_tcam_entry_mask_to_word_index32(                                   \
    uint32_t entry_idx)                                                         \
{                                                                               \
    return (entry_idx << 1) + 4;                                                \
}

_MEM_TCAM_WORD_TO_ENTRY_INDEX8
_MEM_TCAM_ENTRY_VALUE_TO_WORD_INDEX8
_MEM_TCAM_ENTRY_MASK_TO_WORD_INDEX8

_MEM_TCAM_WORD_TO_ENTRY_INDEX16
_MEM_TCAM_ENTRY_VALUE_TO_WORD_INDEX16
_MEM_TCAM_ENTRY_MASK_TO_WORD_INDEX16

_MEM_TCAM_WORD_TO_ENTRY_INDEX32
_MEM_TCAM_ENTRY_VALUE_TO_WORD_INDEX32
_MEM_TCAM_ENTRY_MASK_TO_WORD_INDEX32

#endif /* __NFP_MEM_TCAM_C__ */
