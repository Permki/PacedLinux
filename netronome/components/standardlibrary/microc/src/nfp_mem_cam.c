/*
 * Copyright (C) 2008-2014 Netronome Systems, Inc.  All rights reserved.
 */

#ifndef __NFP_MEM_CAM_C__
#define __NFP_MEM_CAM_C__

#include <nfp.h>
#include <nfp_intrinsic.h>
#include <nfp_mem.h>
#include <nfp_mem_cam.h>

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


#define _MEM_CAM_PARAM_ALIGN_16_CHECK(addr)                 \
        if (__is_ct_const(addr))                            \
        {                                                   \
            CT_ASSERT((addr & 0xf) == 0);                   \
        }                                                   \
        else                                                \
        {                                                   \
            RT_ALIGN_ASSERT((addr & 0xf) == 0);             \
        }

#define _MEM_CAM_PARAM_SIGNAL_CHECK(sig)                    \
        if (__is_ct_const(sig))                             \
        {                                                   \
            CT_ASSERT(sig == sig_done || sig == ctx_swap);  \
        }

#define _MEM_CAM_PARAM_SIGNAL_PAIR_CHECK(sync)              \
        if (__is_ct_const(sync))                            \
        {                                                   \
            CT_ASSERT(sync == sig_done);                    \
        }

#define _MEM_CAM_INDEX_CHECK(index, max_value)              \
        if (__is_ct_const(index))                           \
        {                                                   \
            CT_ASSERT((index) < (max_value));               \
        }                                                   \
        else                                                \
        {                                                   \
            RT_RANGE_ASSERT((index) < (max_value));         \
        }

#define _MEM_CAM_ENTRY_RANGE_CHECK(value, max_value)        \
        if (__is_ct_const(value))                           \
        {                                                   \
            CT_ASSERT((value) <= (max_value));              \
        }                                                   \
        else                                                \
        {                                                   \
            RT_RANGE_ASSERT((value) <= (max_value));        \
        }

__intrinsic
void cmd_mem_cam128_init_ptr32(
    __mem mem_cam128_in_mem_t *cam
)
{
    cmd_mem_write_atomic_imm_ptr32(0, (volatile __addr32 __mem void *)cam, sizeof(*cam) / sizeof(uint32_t));
}

__intrinsic
void cmd_mem_cam128_init_ptr40(
    mem_cam128_ptr40_t cam
)
{
    cmd_mem_write_atomic_imm_ptr40(0, cam, sizeof(*cam) / sizeof(uint32_t));
}

__intrinsic
void cmd_mem_cam256_init_ptr32(
    __mem mem_cam256_in_mem_t *cam
)
{
    cmd_mem_write_atomic_imm_ptr32(0, (void __addr32 __mem *)cam, sizeof(*cam) / sizeof(uint32_t));
}

__intrinsic
void cmd_mem_cam256_init_ptr40(
    mem_cam256_ptr40_t cam
)
{
    cmd_mem_write_atomic_imm_ptr40(0, cam, sizeof(*cam) / sizeof(uint32_t));
}

__intrinsic
void cmd_mem_cam384_init_ptr32(
    __mem mem_cam384_in_mem_t *cam
)
{
    __addr32 __mem uint32_t *fp = (__addr32 __mem uint32_t *)cam;
    cmd_mem_write_atomic_imm_ptr32(0, (void __addr32 __mem *)cam, 8);
    cmd_mem_write_atomic_imm_ptr32(0, fp + 8, 4);
}

__intrinsic
void cmd_mem_cam384_init_ptr40(
    mem_cam384_ptr40_t cam
)
{
    __addr40 __mem uint32_t *fp = (__addr40 __mem uint32_t *)cam;
    cmd_mem_write_atomic_imm_ptr40(0, cam, 8);
    cmd_mem_write_atomic_imm_ptr40(0, fp + 8, 4);
}

__intrinsic
void cmd_mem_cam512_init_ptr32(
    __mem mem_cam512_in_mem_t *cam
)
{
    __addr32 __mem uint32_t *fp = (__addr32 __mem uint32_t *)cam;
    cmd_mem_write_atomic_imm_ptr32(0, (void __addr32 __mem *)cam, sizeof(*cam) / sizeof(uint32_t) / 2);
    cmd_mem_write_atomic_imm_ptr32(0, fp + sizeof(*cam) / sizeof(uint32_t) / 2, sizeof(*cam) / sizeof(uint32_t) / 2);
}

__intrinsic
void cmd_mem_cam512_init_ptr40(
    mem_cam512_ptr40_t cam
)
{
    __addr40 __mem uint32_t *fp = (__addr40 __mem uint32_t *)cam;
    cmd_mem_write_atomic_imm_ptr40(0, cam, sizeof(*cam) / sizeof(uint32_t) / 2);
    cmd_mem_write_atomic_imm_ptr40(0, fp + sizeof(*cam) / sizeof(uint32_t) / 2, sizeof(*cam) / sizeof(uint32_t) / 2);
}

void mem_cam128_set8_ptr32(
    __mem mem_cam128_in_mem_t *cam,
    uint32_t index,
    uint32_t value,
    sync_t sync,
    SIGNAL *sig_ptr
    )
{
    _MEM_CAM_INDEX_CHECK(index,sizeof(*cam));
    _MEM_CAM_ENTRY_RANGE_CHECK(value,0xff);
    CT_ASSERT(__is_ct_const(sync));
    {
        __xwrite uint32_t v;
        generic_ind_t ind;
        __addr32 __mem uint8_t *ptr = (__addr32 __mem  uint8_t *)cam + index;
        v = (value << 24) >> ((index & 3) << 3);
        _INTRINSIC_OVERRIDE_BYTEMASK(ind, 8 >> (index & 3));
        cmd_mem_write_atomic_ind_ptr32(&v,(__addr32 __mem void *)ptr,1,ind,sync,sig_ptr);
    }
}

void mem_cam256_set8_ptr32(
    __mem mem_cam256_in_mem_t *cam,
    uint32_t index,
    uint32_t value,
    sync_t sync,
    SIGNAL *sig_ptr
    )
{
    _MEM_CAM_INDEX_CHECK(index,sizeof(*cam));
    _MEM_CAM_ENTRY_RANGE_CHECK(value,0xff);
    CT_ASSERT(__is_ct_const(sync));
    {
        __xwrite uint32_t v;
        generic_ind_t ind;
        __addr32 __mem uint8_t *ptr = (__addr32 __mem  uint8_t *)cam + index;
        v = (value << 24) >> ((index & 3) << 3);
        _INTRINSIC_OVERRIDE_BYTEMASK(ind, 8 >> (index & 3));
        cmd_mem_write_atomic_ind_ptr32(&v,(__addr32 __mem void *)ptr,1,ind,sync,sig_ptr);
    }
}

void mem_cam384_set8_ptr32(
    __mem mem_cam384_in_mem_t *cam,
    uint32_t index,
    uint32_t value,
    sync_t sync,
    SIGNAL *sig_ptr
    )
{
    _MEM_CAM_INDEX_CHECK(index,sizeof(*cam));
    _MEM_CAM_ENTRY_RANGE_CHECK(value,0xff);
    CT_ASSERT(__is_ct_const(sync));
    {
        __xwrite uint32_t v;
        generic_ind_t ind;
        __addr32 __mem uint8_t *ptr = (__addr32 __mem  uint8_t *)cam + index;
        v = (value << 24) >> ((index & 3) << 3);
        _INTRINSIC_OVERRIDE_BYTEMASK(ind, 8 >> (index & 3));
        cmd_mem_write_atomic_ind_ptr32(&v,(__addr32 __mem void *)ptr,1,ind,sync,sig_ptr);
    }
}

void mem_cam512_set8_ptr32(
    __mem mem_cam512_in_mem_t *cam,
    uint32_t index,
    uint32_t value,
    sync_t sync,
    SIGNAL *sig_ptr
    )
{
    _MEM_CAM_INDEX_CHECK(index,sizeof(*cam));
    _MEM_CAM_ENTRY_RANGE_CHECK(value,0xff);
    CT_ASSERT(__is_ct_const(sync));
    {
        __xwrite uint32_t v;
        generic_ind_t ind;
        __addr32 __mem uint8_t *ptr = (__addr32 __mem  uint8_t *)cam + index;
        v = (value << 24) >> ((index & 3) << 3);
        _INTRINSIC_OVERRIDE_BYTEMASK(ind, 8 >> (index & 3));
        cmd_mem_write_atomic_ind_ptr32(&v,(__addr32 __mem void *)ptr,1,ind,sync,sig_ptr);
    }
}


__intrinsic
void mem_cam128_set8_ptr40(
    mem_cam128_ptr40_t cam,
    uint32_t index,
    uint32_t value,
    sync_t sync,
    SIGNAL *sig_ptr
    )
{
    _MEM_CAM_INDEX_CHECK(index,sizeof(*cam));
    _MEM_CAM_ENTRY_RANGE_CHECK(value,0xff);
    CT_ASSERT(__is_ct_const(sync));
    {
        __xwrite uint32_t v;
        generic_ind_t ind;
        __addr40 __mem uint8_t *ptr = (__addr40 __mem uint8_t *)cam + index;
        v = (value << 24) >> ((index & 3) << 3);
        _INTRINSIC_OVERRIDE_BYTEMASK(ind, 8 >> (index & 3));
        cmd_mem_write_atomic_ind_ptr40(&v,(__addr40 __mem void *)ptr,1,ind,sync,sig_ptr);
    }
}

__intrinsic
void mem_cam256_set8_ptr40(
    mem_cam256_ptr40_t cam,
    uint32_t index,
    uint32_t value,
    sync_t sync,
    SIGNAL *sig_ptr
    )
{
    _MEM_CAM_INDEX_CHECK(index,sizeof(*cam));
    _MEM_CAM_ENTRY_RANGE_CHECK(value,0xff);
    CT_ASSERT(__is_ct_const(sync));
    {
        __xwrite uint32_t v;
        generic_ind_t ind;
        __addr40 __mem uint8_t *ptr = (__addr40 __mem uint8_t *)cam + index;
        v = (value << 24) >> ((index & 3) << 3);
        _INTRINSIC_OVERRIDE_BYTEMASK(ind, 8 >> (index & 3));
        cmd_mem_write_atomic_ind_ptr40(&v,(__addr40 __mem void *)ptr,1,ind,sync,sig_ptr);
    }
}

__intrinsic
void mem_cam384_set8_ptr40(
    mem_cam384_ptr40_t cam,
    uint32_t index,
    uint32_t value,
    sync_t sync,
    SIGNAL *sig_ptr
    )
{
    _MEM_CAM_INDEX_CHECK(index,sizeof(*cam));
    _MEM_CAM_ENTRY_RANGE_CHECK(value,0xff);
    CT_ASSERT(__is_ct_const(sync));
    {
        __xwrite uint32_t v;
        generic_ind_t ind;
        __addr40 __mem uint8_t *ptr = (__addr40 __mem uint8_t *)cam + index;
        v = (value << 24) >> ((index & 3) << 3);
        _INTRINSIC_OVERRIDE_BYTEMASK(ind, 8 >> (index & 3));
        cmd_mem_write_atomic_ind_ptr40(&v,(__addr40 __mem void *)ptr,1,ind,sync,sig_ptr);
    }
}

__intrinsic
void mem_cam512_set8_ptr40(
    mem_cam512_ptr40_t cam,
    uint32_t index,
    uint32_t value,
    sync_t sync,
    SIGNAL *sig_ptr
    )
{
    _MEM_CAM_INDEX_CHECK(index,sizeof(*cam));
    _MEM_CAM_ENTRY_RANGE_CHECK(value,0xff);
    CT_ASSERT(__is_ct_const(sync));
    {
        __xwrite uint32_t v;
        generic_ind_t ind;
        __addr40 __mem uint8_t *ptr = (__addr40 __mem uint8_t *)cam + index;
        v = (value << 24) >> ((index & 3) << 3);
        _INTRINSIC_OVERRIDE_BYTEMASK(ind, 8 >> (index & 3));
        cmd_mem_write_atomic_ind_ptr40(&v,(__addr40 __mem void *)ptr,1,ind,sync,sig_ptr);
    }
}


__intrinsic
void mem_cam128_set16_ptr32(
    __mem mem_cam128_in_mem_t *cam,
    uint32_t index,
    uint32_t value,
    sync_t sync,
    SIGNAL *sig_ptr
    )
{
    _MEM_CAM_INDEX_CHECK(index,sizeof(*cam)/2);
    _MEM_CAM_ENTRY_RANGE_CHECK(value,0xffff);
    CT_ASSERT(__is_ct_const(sync));
    {
        __xwrite uint32_t v;
        generic_ind_t ind;
        __addr32 __mem uint16_t *ptr = (__addr32 __mem uint16_t *)cam + index;
        v = (value << 16) >> ((index & 1) << 4);
        _INTRINSIC_OVERRIDE_BYTEMASK(ind, 0x0c >> ((index & 1) << 1));
        cmd_mem_write_atomic_ind_ptr32(&v,(__addr32 __mem void*)ptr,1,ind,sync,sig_ptr);
    }
}

__intrinsic
void mem_cam256_set16_ptr32(
    __mem mem_cam256_in_mem_t *cam,
    uint32_t index,
    uint32_t value,
    sync_t sync,
    SIGNAL *sig_ptr
    )
{
    _MEM_CAM_INDEX_CHECK(index,sizeof(*cam)/2);
    _MEM_CAM_ENTRY_RANGE_CHECK(value,0xffff);
    CT_ASSERT(__is_ct_const(sync));
    {
        __xwrite uint32_t v;
        generic_ind_t ind;
        __addr32 __mem uint16_t *ptr = (__addr32 __mem uint16_t *)cam + index;
        v = (value << 16) >> ((index & 1) << 4);
        _INTRINSIC_OVERRIDE_BYTEMASK(ind, 0x0c >> ((index & 1) << 1));
        cmd_mem_write_atomic_ind_ptr32(&v,(__addr32 __mem void*)ptr,1,ind,sync,sig_ptr);
    }
}

__intrinsic
void mem_cam384_set16_ptr32(
    __mem mem_cam384_in_mem_t *cam,
    uint32_t index,
    uint32_t value,
    sync_t sync,
    SIGNAL *sig_ptr
    )
{
    _MEM_CAM_INDEX_CHECK(index,sizeof(*cam)/2);
    _MEM_CAM_ENTRY_RANGE_CHECK(value,0xffff);
    CT_ASSERT(__is_ct_const(sync));
    {
        __xwrite uint32_t v;
        generic_ind_t ind;
        __addr32 __mem uint16_t *ptr = (__addr32 __mem uint16_t *)cam + index;
        v = (value << 16) >> ((index & 1) << 4);
        _INTRINSIC_OVERRIDE_BYTEMASK(ind, 0x0c >> ((index & 1) << 1));
        cmd_mem_write_atomic_ind_ptr32(&v,(__addr32 __mem void*)ptr,1,ind,sync,sig_ptr);
    }
}

__intrinsic
void mem_cam512_set16_ptr32(
    __mem mem_cam512_in_mem_t *cam,
    uint32_t index,
    uint32_t value,
    sync_t sync,
    SIGNAL *sig_ptr
    )
{
    _MEM_CAM_INDEX_CHECK(index,sizeof(*cam)/2);
    _MEM_CAM_ENTRY_RANGE_CHECK(value,0xffff);
    CT_ASSERT(__is_ct_const(sync));
    {
        __xwrite uint32_t v;
        generic_ind_t ind;
        __addr32 __mem uint16_t *ptr = (__addr32 __mem uint16_t *)cam + index;
        v = (value << 16) >> ((index & 1) << 4);
        _INTRINSIC_OVERRIDE_BYTEMASK(ind, 0x0c >> ((index & 1) << 1));
        cmd_mem_write_atomic_ind_ptr32(&v,(__addr32 __mem void*)ptr,1,ind,sync,sig_ptr);
    }
}


__intrinsic
void mem_cam128_set16_ptr40(
    mem_cam128_ptr40_t cam,
    uint32_t index,
    uint32_t value,
    sync_t sync,
    SIGNAL *sig_ptr
    )
{
    _MEM_CAM_INDEX_CHECK(index,sizeof(*cam)/2);
    _MEM_CAM_ENTRY_RANGE_CHECK(value,0xffff);
    CT_ASSERT(__is_ct_const(sync));
    {
        __xwrite uint32_t v;
        generic_ind_t ind;
        __addr40 __mem uint16_t *ptr = (__addr40 __mem uint16_t *)cam + index;
        v = (value << 16) >> ((index & 1) << 4);
        _INTRINSIC_OVERRIDE_BYTEMASK(ind, 0x0c >> ((index & 1) << 1));
        cmd_mem_write_atomic_ind_ptr40(&v,(__addr40 __mem void*)ptr,1,ind,sync,sig_ptr);
    }
}

__intrinsic
void mem_cam256_set16_ptr40(
    mem_cam256_ptr40_t cam,
    uint32_t index,
    uint32_t value,
    sync_t sync,
    SIGNAL *sig_ptr
    )
{
    _MEM_CAM_INDEX_CHECK(index,sizeof(*cam)/2);
    _MEM_CAM_ENTRY_RANGE_CHECK(value,0xffff);
    CT_ASSERT(__is_ct_const(sync));
    {
        __xwrite uint32_t v;
        generic_ind_t ind;
        __addr40 __mem uint16_t *ptr = (__addr40 __mem uint16_t *)cam + index;
        v = (value << 16) >> ((index & 1) << 4);
        _INTRINSIC_OVERRIDE_BYTEMASK(ind, 0x0c >> ((index & 1) << 1));
        cmd_mem_write_atomic_ind_ptr40(&v,(__addr40 __mem void*)ptr,1,ind,sync,sig_ptr);
    }
}

__intrinsic
void mem_cam384_set16_ptr40(
    mem_cam384_ptr40_t cam,
    uint32_t index,
    uint32_t value,
    sync_t sync,
    SIGNAL *sig_ptr
    )
{
    _MEM_CAM_INDEX_CHECK(index,sizeof(*cam)/2);
    _MEM_CAM_ENTRY_RANGE_CHECK(value,0xffff);
    CT_ASSERT(__is_ct_const(sync));
    {
        __xwrite uint32_t v;
        generic_ind_t ind;
        __addr40 __mem uint16_t *ptr = (__addr40 __mem uint16_t *)cam + index;
        v = (value << 16) >> ((index & 1) << 4);
        _INTRINSIC_OVERRIDE_BYTEMASK(ind, 0x0c >> ((index & 1) << 1));
        cmd_mem_write_atomic_ind_ptr40(&v,(__addr40 __mem void*)ptr,1,ind,sync,sig_ptr);
    }
}

__intrinsic
void mem_cam512_set16_ptr40(
    mem_cam512_ptr40_t cam,
    uint32_t index,
    uint32_t value,
    sync_t sync,
    SIGNAL *sig_ptr
    )
{
    _MEM_CAM_INDEX_CHECK(index,sizeof(*cam)/2);
    _MEM_CAM_ENTRY_RANGE_CHECK(value,0xffff);
    CT_ASSERT(__is_ct_const(sync));
    {
        __xwrite uint32_t v;
        generic_ind_t ind;
        __addr40 __mem uint16_t *ptr = (__addr40 __mem uint16_t *)cam + index;
        v = (value << 16) >> ((index & 1) << 4);
        _INTRINSIC_OVERRIDE_BYTEMASK(ind, 0x0c >> ((index & 1) << 1));
        cmd_mem_write_atomic_ind_ptr40(&v,(__addr40 __mem void*)ptr,1,ind,sync,sig_ptr);
    }
}


__intrinsic
void mem_cam128_set24_ptr32(
    __mem mem_cam128_in_mem_t *cam,
    uint32_t index,
    uint32_t value,
    sync_t sync,
    SIGNAL *sig_ptr
    )
{
    _MEM_CAM_INDEX_CHECK(index,(sizeof(*cam)/sizeof(uint32_t)));
    CT_ASSERT(__is_ct_const(sync));
    {
        __xwrite uint32_t v = value;
        __addr32 __mem uint32_t *ptr = (__addr32 __mem uint32_t *)cam + index;
        cmd_mem_write_atomic_ptr32(&v,(__addr32 __mem void*)ptr,1,sync,sig_ptr);
    }
}

__intrinsic
void mem_cam256_set24_ptr32(
    __mem mem_cam256_in_mem_t *cam,
    uint32_t index,
    uint32_t value,
    sync_t sync,
    SIGNAL *sig_ptr
    )
{
    _MEM_CAM_INDEX_CHECK(index,(sizeof(*cam)/sizeof(uint32_t)));
    CT_ASSERT(__is_ct_const(sync));
    {
        __xwrite uint32_t v = value;
        __addr32 __mem uint32_t *ptr = (__addr32 __mem uint32_t *)cam + index;
        cmd_mem_write_atomic_ptr32(&v,(__addr32 __mem void*)ptr,1,sync,sig_ptr);
    }
}

__intrinsic
void mem_cam384_set24_ptr32(
    __mem mem_cam384_in_mem_t *cam,
    uint32_t index,
    uint32_t value,
    sync_t sync,
    SIGNAL *sig_ptr
    )
{
    _MEM_CAM_INDEX_CHECK(index,(sizeof(*cam)/sizeof(uint32_t)));
    CT_ASSERT(__is_ct_const(sync));
    {
        __xwrite uint32_t v = value;
        __addr32 __mem uint32_t *ptr = (__addr32 __mem uint32_t *)cam + index;
        cmd_mem_write_atomic_ptr32(&v,(__addr32 __mem void*)ptr,1,sync,sig_ptr);
    }
}

__intrinsic
void mem_cam512_set24_ptr32(
    __mem mem_cam512_in_mem_t *cam,
    uint32_t index,
    uint32_t value,
    sync_t sync,
    SIGNAL *sig_ptr
    )
{
    _MEM_CAM_INDEX_CHECK(index,(sizeof(*cam)/sizeof(uint32_t)));
    CT_ASSERT(__is_ct_const(sync));
    {
        __xwrite uint32_t v = value;
        __addr32 __mem uint32_t *ptr = (__addr32 __mem uint32_t *)cam + index;
        cmd_mem_write_atomic_ptr32(&v,(__addr32 __mem void*)ptr,1,sync,sig_ptr);
    }
}


__intrinsic
void mem_cam128_set24_ptr40(
    mem_cam128_ptr40_t cam,
    uint32_t index,
    uint32_t value,
    sync_t sync,
    SIGNAL *sig_ptr
    )
{
    _MEM_CAM_INDEX_CHECK(index,(sizeof(*cam)/sizeof(uint32_t)));
    CT_ASSERT(__is_ct_const(sync));
    {
        __xwrite uint32_t v = value;
        __addr40 __mem uint32_t *ptr = (__addr40 __mem uint32_t *)cam + index;
        cmd_mem_write_atomic_ptr40(&v,(__addr40 __mem void*)ptr,1,sync,sig_ptr);
    }
}

__intrinsic
void mem_cam256_set24_ptr40(
    mem_cam256_ptr40_t cam,
    uint32_t index,
    uint32_t value,
    sync_t sync,
    SIGNAL *sig_ptr
    )
{
    _MEM_CAM_INDEX_CHECK(index,(sizeof(*cam)/sizeof(uint32_t)));
    CT_ASSERT(__is_ct_const(sync));
    {
        __xwrite uint32_t v = value;
        __addr40 __mem uint32_t *ptr = (__addr40 __mem uint32_t *)cam + index;
        cmd_mem_write_atomic_ptr40(&v,(__addr40 __mem void*)ptr,1,sync,sig_ptr);
    }
}

__intrinsic
void mem_cam384_set24_ptr40(
    mem_cam384_ptr40_t cam,
    uint32_t index,
    uint32_t value,
    sync_t sync,
    SIGNAL *sig_ptr
    )
{
    _MEM_CAM_INDEX_CHECK(index,(sizeof(*cam)/sizeof(uint32_t)));
    CT_ASSERT(__is_ct_const(sync));
    {
        __xwrite uint32_t v = value;
        __addr40 __mem uint32_t *ptr = (__addr40 __mem uint32_t *)cam + index;
        cmd_mem_write_atomic_ptr40(&v,(__addr40 __mem void*)ptr,1,sync,sig_ptr);
    }
}

__intrinsic
void mem_cam512_set24_ptr40(
    mem_cam512_ptr40_t cam,
    uint32_t index,
    uint32_t value,
    sync_t sync,
    SIGNAL *sig_ptr
    )
{
    _MEM_CAM_INDEX_CHECK(index,(sizeof(*cam)/sizeof(uint32_t)));
    CT_ASSERT(__is_ct_const(sync));
    {
        __xwrite uint32_t v = value;
        __addr40 __mem uint32_t *ptr = (__addr40 __mem uint32_t *)cam + index;
        cmd_mem_write_atomic_ptr40(&v,(__addr40 __mem void*)ptr,1,sync,sig_ptr);
    }
}


__intrinsic
void mem_cam128_set32_ptr32(
    __mem mem_cam128_in_mem_t *cam,
    uint32_t index,
    uint32_t value,
    sync_t sync,
    SIGNAL *sig_ptr
    )
{
    _MEM_CAM_INDEX_CHECK(index,(sizeof(*cam)/sizeof(uint32_t)));
    CT_ASSERT(__is_ct_const(sync));
    {
        __xwrite uint32_t v = value;
        __addr32 __mem uint32_t *ptr = (__addr32 __mem uint32_t*)cam + index;
        cmd_mem_write_atomic_ptr32(&v,(__addr32 __mem void*)ptr,1,sync,sig_ptr);
    }
}

__intrinsic
void mem_cam256_set32_ptr32(
    __mem mem_cam256_in_mem_t *cam,
    uint32_t index,
    uint32_t value,
    sync_t sync,
    SIGNAL *sig_ptr
    )
{
    _MEM_CAM_INDEX_CHECK(index,(sizeof(*cam)/sizeof(uint32_t)));
    CT_ASSERT(__is_ct_const(sync));
    {
        __xwrite uint32_t v = value;
        __addr32 __mem uint32_t *ptr = (__addr32 __mem uint32_t*)cam + index;
        cmd_mem_write_atomic_ptr32(&v,(__addr32 __mem void*)ptr,1,sync,sig_ptr);
    }
}

__intrinsic
void mem_cam384_set32_ptr32(
    __mem mem_cam384_in_mem_t *cam,
    uint32_t index,
    uint32_t value,
    sync_t sync,
    SIGNAL *sig_ptr
    )
{
    _MEM_CAM_INDEX_CHECK(index,(sizeof(*cam)/sizeof(uint32_t)));
    CT_ASSERT(__is_ct_const(sync));
    {
        __xwrite uint32_t v = value;
        __addr32 __mem uint32_t *ptr = (__addr32 __mem uint32_t*)cam + index;
        cmd_mem_write_atomic_ptr32(&v,(__addr32 __mem void*)ptr,1,sync,sig_ptr);
    }
}

__intrinsic
void mem_cam512_set32_ptr32(
    __mem mem_cam512_in_mem_t *cam,
    uint32_t index,
    uint32_t value,
    sync_t sync,
    SIGNAL *sig_ptr
    )
{
    _MEM_CAM_INDEX_CHECK(index,(sizeof(*cam)/sizeof(uint32_t)));
    CT_ASSERT(__is_ct_const(sync));
    {
        __xwrite uint32_t v = value;
        __addr32 __mem uint32_t *ptr = (__addr32 __mem uint32_t*)cam + index;
        cmd_mem_write_atomic_ptr32(&v,(__addr32 __mem void*)ptr,1,sync,sig_ptr);
    }
}


__intrinsic
void mem_cam128_set32_ptr40(
    mem_cam128_ptr40_t cam,
    uint32_t index,
    uint32_t value,
    sync_t sync,
    SIGNAL *sig_ptr
    )
{
    _MEM_CAM_INDEX_CHECK(index,(sizeof(*cam)/sizeof(uint32_t)));
    CT_ASSERT(__is_ct_const(sync));
    {
        __xwrite uint32_t v = value;
        __addr40 __mem uint32_t *ptr = (__addr40 __mem uint32_t*)cam + index;
        cmd_mem_write_atomic_ptr40(&v,(__addr40 __mem void*)ptr,1,sync,sig_ptr);
    }
}

__intrinsic
void mem_cam256_set32_ptr40(
    mem_cam256_ptr40_t cam,
    uint32_t index,
    uint32_t value,
    sync_t sync,
    SIGNAL *sig_ptr
    )
{
    _MEM_CAM_INDEX_CHECK(index,(sizeof(*cam)/sizeof(uint32_t)));
    CT_ASSERT(__is_ct_const(sync));
    {
        __xwrite uint32_t v = value;
        __addr40 __mem uint32_t *ptr = (__addr40 __mem uint32_t*)cam + index;
        cmd_mem_write_atomic_ptr40(&v,(__addr40 __mem void*)ptr,1,sync,sig_ptr);
    }
}

__intrinsic
void mem_cam384_set32_ptr40(
    mem_cam384_ptr40_t cam,
    uint32_t index,
    uint32_t value,
    sync_t sync,
    SIGNAL *sig_ptr
    )
{
    _MEM_CAM_INDEX_CHECK(index,(sizeof(*cam)/sizeof(uint32_t)));
    CT_ASSERT(__is_ct_const(sync));
    {
        __xwrite uint32_t v = value;
        __addr40 __mem uint32_t *ptr = (__addr40 __mem uint32_t*)cam + index;
        cmd_mem_write_atomic_ptr40(&v,(__addr40 __mem void*)ptr,1,sync,sig_ptr);
    }
}

__intrinsic
void mem_cam512_set32_ptr40(
    mem_cam512_ptr40_t cam,
    uint32_t index,
    uint32_t value,
    sync_t sync,
    SIGNAL *sig_ptr
    )
{
    _MEM_CAM_INDEX_CHECK(index,(sizeof(*cam)/sizeof(uint32_t)));
    CT_ASSERT(__is_ct_const(sync));
    {
        __xwrite uint32_t v = value;
        __addr40 __mem uint32_t *ptr = (__addr40 __mem uint32_t*)cam + index;
        cmd_mem_write_atomic_ptr40(&v,(__addr40 __mem void*)ptr,1,sync,sig_ptr);
    }
}


__intrinsic
__xread mem_cam_lookup8_out_t *mem_cam128_lookup8_ptr32(
    __mem mem_cam128_in_mem_t *cam,
    __xwrite mem_cam_lookup8_in_t *xfer,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
    )
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_write_reg(xfer));
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done);
        __asm
        {
            mem[cam128_lookup8, *xfer, cam, 0 ], sig_done[*sig_pair_ptr]
        }
    }
    return (__xread mem_cam_lookup8_out_t *)(xfer);
    _INTRINSIC_END;
}

__intrinsic
__xread mem_cam_lookup8_out_t *mem_cam256_lookup8_ptr32(
    __mem mem_cam256_in_mem_t *cam,
    __xwrite mem_cam_lookup8_in_t *xfer,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
    )
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_write_reg(xfer));
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done);
        __asm
        {
            mem[cam256_lookup8, *xfer, cam, 0 ], sig_done[*sig_pair_ptr]
        }
    }
    return (__xread mem_cam_lookup8_out_t *)(xfer);
    _INTRINSIC_END;
}

__intrinsic
__xread mem_cam_lookup16_out_t *mem_cam128_lookup16_ptr32(
    __mem mem_cam128_in_mem_t *cam,
    __xwrite mem_cam_lookup16_in_t *xfer,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
    )
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_write_reg(xfer));
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done);
        __asm
        {
            mem[cam128_lookup16, *xfer, cam, 0 ], sig_done[*sig_pair_ptr]
        }
    }
    return (__xread mem_cam_lookup16_out_t *)(xfer);
    _INTRINSIC_END;
}

__intrinsic
__xread mem_cam_lookup16_out_t *mem_cam256_lookup16_ptr32(
    __mem mem_cam256_in_mem_t *cam,
    __xwrite mem_cam_lookup16_in_t *xfer,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
    )
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_write_reg(xfer));
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done);
        __asm
        {
            mem[cam256_lookup16, *xfer, cam, 0 ], sig_done[*sig_pair_ptr]
        }
    }
    return (__xread mem_cam_lookup16_out_t *)(xfer);
    _INTRINSIC_END;
}

__intrinsic
__xread mem_cam_lookup24_out_t *mem_cam128_lookup24_ptr32(
    __mem mem_cam128_in_mem_t *cam,
    __xwrite mem_cam_lookup24_in_t *xfer,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
    )
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_write_reg(xfer));
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done);
        __asm
        {
            mem[cam128_lookup24, *xfer, cam, 0 ], sig_done[*sig_pair_ptr]
        }
    }
    return (__xread mem_cam_lookup24_out_t *)(xfer);
    _INTRINSIC_END;
}

__intrinsic
__xread mem_cam_lookup24_out_t *mem_cam256_lookup24_ptr32(
    __mem mem_cam256_in_mem_t *cam,
    __xwrite mem_cam_lookup24_in_t *xfer,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
    )
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_write_reg(xfer));
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done);
        __asm
        {
            mem[cam256_lookup24, *xfer, cam, 0 ], sig_done[*sig_pair_ptr]
        }
    }
    return (__xread mem_cam_lookup24_out_t *)(xfer);
    _INTRINSIC_END;
}

__intrinsic
__xread mem_cam_lookup32_out_t *mem_cam128_lookup32_ptr32(
    __mem mem_cam128_in_mem_t *cam,
    __xwrite mem_cam_lookup32_in_t *xfer,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
    )
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_write_reg(xfer));
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done);
        __asm
        {
            mem[cam128_lookup32, *xfer, cam, 0 ], sig_done[*sig_pair_ptr]
        }
    }
    return (__xread mem_cam_lookup32_out_t *)(xfer);
    _INTRINSIC_END;
}

__intrinsic
__xread mem_cam_lookup32_out_t *mem_cam256_lookup32_ptr32(
    __mem mem_cam256_in_mem_t *cam,
    __xwrite mem_cam_lookup32_in_t *xfer,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
    )
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_write_reg(xfer));
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done);
        __asm
        {
            mem[cam256_lookup32, *xfer, cam, 0 ], sig_done[*sig_pair_ptr]
        }
    }
    return (__xread mem_cam_lookup32_out_t *)(xfer);
    _INTRINSIC_END;
}


__intrinsic
__xread mem_cam_lookup8_out_t *mem_cam128_lookup8_ptr40(
    mem_cam128_ptr40_t cam,
    __xwrite mem_cam_lookup8_in_t *xfer,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
    )
{
    _INTRINSIC_BEGIN;
    {
        __gpr uint32_t hi_addr, low_addr;
        CT_ASSERT(__is_write_reg(xfer));
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done);
        _INTRINSIC_CONVERT_HI_LO_ADDRESS(cam);
        __asm
        {
            mem[cam128_lookup8, *xfer, hi_addr, <<8, low_addr], sig_done[*sig_pair_ptr]
        }
    }
    return (__xread mem_cam_lookup8_out_t *)(xfer);
    _INTRINSIC_END;
}

__intrinsic
__xread mem_cam_lookup8_out_t *mem_cam256_lookup8_ptr40(
    mem_cam256_ptr40_t cam,
    __xwrite mem_cam_lookup8_in_t *xfer,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
    )
{
    _INTRINSIC_BEGIN;
    {
        __gpr uint32_t hi_addr, low_addr;
        CT_ASSERT(__is_write_reg(xfer));
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done);
        _INTRINSIC_CONVERT_HI_LO_ADDRESS(cam);
        __asm
        {
            mem[cam256_lookup8, *xfer, hi_addr, <<8, low_addr], sig_done[*sig_pair_ptr]
        }
    }
    return (__xread mem_cam_lookup8_out_t *)(xfer);
    _INTRINSIC_END;
}

__intrinsic
__xread mem_cam_lookup16_out_t *mem_cam128_lookup16_ptr40(
    mem_cam128_ptr40_t cam,
    __xwrite mem_cam_lookup16_in_t *xfer,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
    )
{
    _INTRINSIC_BEGIN;
    {
        __gpr uint32_t hi_addr, low_addr;
        CT_ASSERT(__is_write_reg(xfer));
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done);
        _INTRINSIC_CONVERT_HI_LO_ADDRESS(cam);
        __asm
        {
            mem[cam128_lookup16, *xfer, hi_addr, <<8, low_addr], sig_done[*sig_pair_ptr]
        }
    }
    return (__xread mem_cam_lookup16_out_t *)(xfer);
    _INTRINSIC_END;
}

__intrinsic
__xread mem_cam_lookup16_out_t *mem_cam256_lookup16_ptr40(
    mem_cam256_ptr40_t cam,
    __xwrite mem_cam_lookup16_in_t *xfer,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
    )
{
    _INTRINSIC_BEGIN;
    {
        __gpr uint32_t hi_addr, low_addr;
        CT_ASSERT(__is_write_reg(xfer));
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done);
        _INTRINSIC_CONVERT_HI_LO_ADDRESS(cam);
        __asm
        {
            mem[cam256_lookup16, *xfer, hi_addr, <<8, low_addr], sig_done[*sig_pair_ptr]
        }
    }
    return (__xread mem_cam_lookup16_out_t *)(xfer);
    _INTRINSIC_END;
}

__intrinsic
__xread mem_cam_lookup24_out_t *mem_cam128_lookup24_ptr40(
    mem_cam128_ptr40_t cam,
    __xwrite mem_cam_lookup24_in_t *xfer,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
    )
{
    _INTRINSIC_BEGIN;
    {
        __gpr uint32_t hi_addr, low_addr;
        CT_ASSERT(__is_write_reg(xfer));
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done);
        _INTRINSIC_CONVERT_HI_LO_ADDRESS(cam);
        __asm
        {
            mem[cam128_lookup24, *xfer, hi_addr, <<8, low_addr], sig_done[*sig_pair_ptr]
        }
    }
    return (__xread mem_cam_lookup24_out_t *)(xfer);
    _INTRINSIC_END;
}

__intrinsic
__xread mem_cam_lookup24_out_t *mem_cam256_lookup24_ptr40(
    mem_cam256_ptr40_t cam,
    __xwrite mem_cam_lookup24_in_t *xfer,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
    )
{
    _INTRINSIC_BEGIN;
    {
        __gpr uint32_t hi_addr, low_addr;
        CT_ASSERT(__is_write_reg(xfer));
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done);
        _INTRINSIC_CONVERT_HI_LO_ADDRESS(cam);
        __asm
        {
            mem[cam256_lookup24, *xfer, hi_addr, <<8, low_addr], sig_done[*sig_pair_ptr]
        }
    }
    return (__xread mem_cam_lookup24_out_t *)(xfer);
    _INTRINSIC_END;
}

__intrinsic
__xread mem_cam_lookup32_out_t *mem_cam128_lookup32_ptr40(
    mem_cam128_ptr40_t cam,
    __xwrite mem_cam_lookup32_in_t *xfer,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
    )
{
    _INTRINSIC_BEGIN;
    {
        __gpr uint32_t hi_addr, low_addr;
        CT_ASSERT(__is_write_reg(xfer));
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done);
        _INTRINSIC_CONVERT_HI_LO_ADDRESS(cam);
        __asm
        {
            mem[cam128_lookup32, *xfer, hi_addr, <<8, low_addr], sig_done[*sig_pair_ptr]
        }
    }
    return (__xread mem_cam_lookup32_out_t *)(xfer);
    _INTRINSIC_END;
}

__intrinsic
__xread mem_cam_lookup32_out_t *mem_cam256_lookup32_ptr40(
    mem_cam256_ptr40_t cam,
    __xwrite mem_cam_lookup32_in_t *xfer,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
    )
{
    _INTRINSIC_BEGIN;
    {
        __gpr uint32_t hi_addr, low_addr;
        CT_ASSERT(__is_write_reg(xfer));
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done);
        _INTRINSIC_CONVERT_HI_LO_ADDRESS(cam);
        __asm
        {
            mem[cam256_lookup32, *xfer, hi_addr, <<8, low_addr], sig_done[*sig_pair_ptr]
        }
    }
    return (__xread mem_cam_lookup32_out_t *)(xfer);
    _INTRINSIC_END;
}


enum
{
    CAM_REF_COUNT_128_8 = 0,
    CAM_REF_COUNT_128_16,
    CAM_REF_COUNT_128_24,
    CAM_REF_COUNT_128_32,

    CAM_REF_COUNT_256_8 = 4,
    CAM_REF_COUNT_256_16,
    CAM_REF_COUNT_256_24,
    CAM_REF_COUNT_256_32,

    CAM_REF_COUNT_384_8 = 8,
    CAM_REF_COUNT_384_16,
    CAM_REF_COUNT_384_24,
    CAM_REF_COUNT_384_32,

    CAM_REF_COUNT_512_8 = 12,
    CAM_REF_COUNT_512_16,
    CAM_REF_COUNT_512_24,
    CAM_REF_COUNT_512_32
};

__intrinsic
__xread mem_cam_lookup8_out_t *mem_cam384_lookup8_ptr32(
    __mem mem_cam384_in_mem_t *cam,
    __xwrite mem_cam_lookup8_in_t *xfer,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
    )
{
    _INTRINSIC_BEGIN;
    {
        generic_ind_t ind;
        CT_ASSERT(__is_write_reg(xfer));
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done);
        _INTRINSIC_OVERRIDE_LENGTH(ind, CAM_REF_COUNT_384_8);
        __asm alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
        __asm mem[cam384_lookup8, *xfer, cam, 0 ], sig_done[*sig_pair_ptr], indirect_ref
    }
    return (__xread mem_cam_lookup8_out_t *)(xfer);
    _INTRINSIC_END;
}

__intrinsic
__xread mem_cam_lookup8_out_t *mem_cam512_lookup8_ptr32(
    __mem mem_cam512_in_mem_t *cam,
    __xwrite mem_cam_lookup8_in_t *xfer,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
    )
{
    _INTRINSIC_BEGIN;
    {
        generic_ind_t ind;
        CT_ASSERT(__is_write_reg(xfer));
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done);
        _INTRINSIC_OVERRIDE_LENGTH(ind, CAM_REF_COUNT_512_8);
        __asm alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
        __asm mem[cam512_lookup8, *xfer, cam, 0 ], sig_done[*sig_pair_ptr], indirect_ref
    }
    return (__xread mem_cam_lookup8_out_t *)(xfer);
    _INTRINSIC_END;
}

__intrinsic
__xread mem_cam_lookup16_out_t *mem_cam384_lookup16_ptr32(
    __mem mem_cam384_in_mem_t *cam,
    __xwrite mem_cam_lookup16_in_t *xfer,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
    )
{
    _INTRINSIC_BEGIN;
    {
        generic_ind_t ind;
        CT_ASSERT(__is_write_reg(xfer));
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done);
        _INTRINSIC_OVERRIDE_LENGTH(ind, CAM_REF_COUNT_384_16);
        __asm alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
        __asm mem[cam384_lookup16, *xfer, cam, 0 ], sig_done[*sig_pair_ptr], indirect_ref
    }
    return (__xread mem_cam_lookup16_out_t *)(xfer);
    _INTRINSIC_END;
}

__intrinsic
__xread mem_cam_lookup16_out_t *mem_cam512_lookup16_ptr32(
    __mem mem_cam512_in_mem_t *cam,
    __xwrite mem_cam_lookup16_in_t *xfer,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
    )
{
    _INTRINSIC_BEGIN;
    {
        generic_ind_t ind;
        CT_ASSERT(__is_write_reg(xfer));
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done);
        _INTRINSIC_OVERRIDE_LENGTH(ind, CAM_REF_COUNT_512_16);
        __asm alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
        __asm mem[cam512_lookup16, *xfer, cam, 0 ], sig_done[*sig_pair_ptr], indirect_ref
    }
    return (__xread mem_cam_lookup16_out_t *)(xfer);
    _INTRINSIC_END;
}

__intrinsic
__xread mem_cam_lookup24_out_t *mem_cam384_lookup24_ptr32(
    __mem mem_cam384_in_mem_t *cam,
    __xwrite mem_cam_lookup24_in_t *xfer,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
    )
{
    _INTRINSIC_BEGIN;
    {
        generic_ind_t ind;
        CT_ASSERT(__is_write_reg(xfer));
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done);
        _INTRINSIC_OVERRIDE_LENGTH(ind, CAM_REF_COUNT_384_24);
        __asm alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
        __asm mem[cam384_lookup24, *xfer, cam, 0 ], sig_done[*sig_pair_ptr], indirect_ref
    }
    return (__xread mem_cam_lookup24_out_t *)(xfer);
    _INTRINSIC_END;
}

__intrinsic
__xread mem_cam_lookup24_out_t *mem_cam512_lookup24_ptr32(
    __mem mem_cam512_in_mem_t *cam,
    __xwrite mem_cam_lookup24_in_t *xfer,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
    )
{
    _INTRINSIC_BEGIN;
    {
        generic_ind_t ind;
        CT_ASSERT(__is_write_reg(xfer));
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done);
        _INTRINSIC_OVERRIDE_LENGTH(ind, CAM_REF_COUNT_512_24);
        __asm alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
        __asm mem[cam512_lookup24, *xfer, cam, 0 ], sig_done[*sig_pair_ptr], indirect_ref
    }
    return (__xread mem_cam_lookup24_out_t *)(xfer);
    _INTRINSIC_END;
}

__intrinsic
__xread mem_cam_lookup32_out_t *mem_cam384_lookup32_ptr32(
    __mem mem_cam384_in_mem_t *cam,
    __xwrite mem_cam_lookup32_in_t *xfer,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
    )
{
    _INTRINSIC_BEGIN;
    {
        generic_ind_t ind;
        CT_ASSERT(__is_write_reg(xfer));
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done);
        _INTRINSIC_OVERRIDE_LENGTH(ind, CAM_REF_COUNT_384_32);
        __asm alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
        __asm mem[cam384_lookup32, *xfer, cam, 0 ], sig_done[*sig_pair_ptr], indirect_ref
    }
    return (__xread mem_cam_lookup32_out_t *)(xfer);
    _INTRINSIC_END;
}

__intrinsic
__xread mem_cam_lookup32_out_t *mem_cam512_lookup32_ptr32(
    __mem mem_cam512_in_mem_t *cam,
    __xwrite mem_cam_lookup32_in_t *xfer,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
    )
{
    _INTRINSIC_BEGIN;
    {
        generic_ind_t ind;
        CT_ASSERT(__is_write_reg(xfer));
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done);
        _INTRINSIC_OVERRIDE_LENGTH(ind, CAM_REF_COUNT_512_32);
        __asm alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
        __asm mem[cam512_lookup32, *xfer, cam, 0 ], sig_done[*sig_pair_ptr], indirect_ref
    }
    return (__xread mem_cam_lookup32_out_t *)(xfer);
    _INTRINSIC_END;
}


__intrinsic
__xread mem_cam_lookup8_out_t *mem_cam384_lookup8_ptr40(
    mem_cam384_ptr40_t cam,
    __xwrite mem_cam_lookup8_in_t *xfer,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
    )
{
    _INTRINSIC_BEGIN;
    {
        generic_ind_t ind;
        uint32_t hi_addr, low_addr;
        CT_ASSERT(__is_write_reg(xfer));
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done);
        _INTRINSIC_CONVERT_HI_LO_ADDRESS(cam);
        _INTRINSIC_OVERRIDE_LENGTH(ind, CAM_REF_COUNT_384_8);
        __asm alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
        __asm mem[cam384_lookup8, *xfer, hi_addr, <<8, low_addr], sig_done[*sig_pair_ptr], indirect_ref
    }
    return (__xread mem_cam_lookup8_out_t *)(xfer);
    _INTRINSIC_END;
}



__intrinsic
__xread mem_cam_lookup8_out_t *mem_cam512_lookup8_ptr40(
    mem_cam512_ptr40_t cam,
    __xwrite mem_cam_lookup8_in_t *xfer,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
    )
{
    _INTRINSIC_BEGIN;
    {
        generic_ind_t ind;
        uint32_t hi_addr, low_addr;
        CT_ASSERT(__is_write_reg(xfer));
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done);
        _INTRINSIC_CONVERT_HI_LO_ADDRESS(cam);
        _INTRINSIC_OVERRIDE_LENGTH(ind, CAM_REF_COUNT_512_8);
        __asm alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
        __asm mem[cam512_lookup8, *xfer, hi_addr, <<8, low_addr], sig_done[*sig_pair_ptr], indirect_ref
    }
    return (__xread mem_cam_lookup8_out_t *)(xfer);
    _INTRINSIC_END;
}

__intrinsic
__xread mem_cam_lookup16_out_t *mem_cam384_lookup16_ptr40(
    mem_cam384_ptr40_t cam,
    __xwrite mem_cam_lookup16_in_t *xfer,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
    )
{
    _INTRINSIC_BEGIN;
    {
        generic_ind_t ind;
        uint32_t hi_addr, low_addr;
        CT_ASSERT(__is_write_reg(xfer));
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done);
        _INTRINSIC_CONVERT_HI_LO_ADDRESS(cam);
        _INTRINSIC_OVERRIDE_LENGTH(ind, CAM_REF_COUNT_384_16);
        __asm alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
        __asm mem[cam384_lookup16, *xfer, hi_addr, <<8, low_addr], sig_done[*sig_pair_ptr], indirect_ref
    }
    return (__xread mem_cam_lookup16_out_t *)(xfer);
    _INTRINSIC_END;
}

__intrinsic
__xread mem_cam_lookup16_out_t *mem_cam512_lookup16_ptr40(
    mem_cam512_ptr40_t cam,
    __xwrite mem_cam_lookup16_in_t *xfer,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
    )
{
    _INTRINSIC_BEGIN;
    {
        generic_ind_t ind;
        uint32_t hi_addr, low_addr;
        CT_ASSERT(__is_write_reg(xfer));
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done);
        _INTRINSIC_CONVERT_HI_LO_ADDRESS(cam);
        _INTRINSIC_OVERRIDE_LENGTH(ind, CAM_REF_COUNT_512_16);
        __asm alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
        __asm mem[cam512_lookup16, *xfer, hi_addr, <<8, low_addr], sig_done[*sig_pair_ptr], indirect_ref
    }
    return (__xread mem_cam_lookup16_out_t *)(xfer);
    _INTRINSIC_END;
}

__intrinsic
__xread mem_cam_lookup24_out_t *mem_cam384_lookup24_ptr40(
    mem_cam384_ptr40_t cam,
    __xwrite mem_cam_lookup24_in_t *xfer,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
    )
{
    _INTRINSIC_BEGIN;
    {
        generic_ind_t ind;
        uint32_t hi_addr, low_addr;
        CT_ASSERT(__is_write_reg(xfer));
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done);
        _INTRINSIC_CONVERT_HI_LO_ADDRESS(cam);
        _INTRINSIC_OVERRIDE_LENGTH(ind, CAM_REF_COUNT_384_24);
        __asm alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
        __asm mem[cam384_lookup24, *xfer, hi_addr, <<8, low_addr], sig_done[*sig_pair_ptr], indirect_ref
    }
    return (__xread mem_cam_lookup24_out_t *)(xfer);
    _INTRINSIC_END;
}

__intrinsic
__xread mem_cam_lookup24_out_t *mem_cam512_lookup24_ptr40(
    mem_cam512_ptr40_t cam,
    __xwrite mem_cam_lookup24_in_t *xfer,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
    )
{
    _INTRINSIC_BEGIN;
    {
        generic_ind_t ind;
        uint32_t hi_addr, low_addr;
        CT_ASSERT(__is_write_reg(xfer));
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done);
        _INTRINSIC_CONVERT_HI_LO_ADDRESS(cam);
        _INTRINSIC_OVERRIDE_LENGTH(ind, CAM_REF_COUNT_512_24);
        __asm alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
        __asm mem[cam512_lookup24, *xfer, hi_addr, <<8, low_addr], sig_done[*sig_pair_ptr], indirect_ref
    }
    return (__xread mem_cam_lookup24_out_t *)(xfer);
    _INTRINSIC_END;
}

__intrinsic
__xread mem_cam_lookup32_out_t *mem_cam384_lookup32_ptr40(
    mem_cam384_ptr40_t cam,
    __xwrite mem_cam_lookup32_in_t *xfer,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
    )
{
    _INTRINSIC_BEGIN;
    {
        generic_ind_t ind;
        uint32_t hi_addr, low_addr;
        CT_ASSERT(__is_write_reg(xfer));
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done);
        _INTRINSIC_CONVERT_HI_LO_ADDRESS(cam);
        _INTRINSIC_OVERRIDE_LENGTH(ind, CAM_REF_COUNT_384_32);
        __asm alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
        __asm mem[cam384_lookup32, *xfer, hi_addr, <<8, low_addr], sig_done[*sig_pair_ptr], indirect_ref
    }
    return (__xread mem_cam_lookup32_out_t *)(xfer);
    _INTRINSIC_END;
}

__intrinsic
__xread mem_cam_lookup32_out_t *mem_cam512_lookup32_ptr40(
    mem_cam512_ptr40_t cam,
    __xwrite mem_cam_lookup32_in_t *xfer,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
    )
{
    _INTRINSIC_BEGIN;
    {
        generic_ind_t ind;
        uint32_t hi_addr, low_addr;
        CT_ASSERT(__is_write_reg(xfer));
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done);
        _INTRINSIC_CONVERT_HI_LO_ADDRESS(cam);
        _INTRINSIC_OVERRIDE_LENGTH(ind, CAM_REF_COUNT_512_32);
        __asm alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
        __asm mem[cam512_lookup32, *xfer, hi_addr, <<8, low_addr], sig_done[*sig_pair_ptr], indirect_ref
    }
    return (__xread mem_cam_lookup32_out_t *)(xfer);
    _INTRINSIC_END;
}


__intrinsic
__xread mem_cam_lookup8_add_out_t *mem_cam128_lookup8_add_ptr32(
    __mem mem_cam128_in_mem_t *cam,
    __xwrite mem_cam_lookup8_in_t *xfer,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
    )
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_write_reg(xfer));
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done);
        __asm
        {
            mem[cam128_lookup8_add, *xfer, cam, 0 ], sig_done[*sig_pair_ptr]
        }
    }
    return (__xread mem_cam_lookup8_add_out_t *)(xfer);
    _INTRINSIC_END;
}

__intrinsic
__xread mem_cam_lookup8_add_out_t *mem_cam256_lookup8_add_ptr32(
    __mem mem_cam256_in_mem_t *cam,
    __xwrite mem_cam_lookup8_in_t *xfer,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
    )
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_write_reg(xfer));
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done);
        __asm
        {
            mem[cam256_lookup8_add, *xfer, cam, 0 ], sig_done[*sig_pair_ptr]
        }
    }
    return (__xread mem_cam_lookup8_add_out_t *)(xfer);
    _INTRINSIC_END;
}

__intrinsic
__xread mem_cam_lookup16_add_out_t *mem_cam128_lookup16_add_ptr32(
    __mem mem_cam128_in_mem_t *cam,
    __xwrite mem_cam_lookup16_in_t *xfer,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
    )
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_write_reg(xfer));
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done);
        __asm
        {
            mem[cam128_lookup16_add, *xfer, cam, 0 ], sig_done[*sig_pair_ptr]
        }
    }
    return (__xread mem_cam_lookup16_add_out_t *)(xfer);
    _INTRINSIC_END;
}

__intrinsic
__xread mem_cam_lookup16_add_out_t *mem_cam256_lookup16_add_ptr32(
    __mem mem_cam256_in_mem_t *cam,
    __xwrite mem_cam_lookup16_in_t *xfer,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
    )
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_write_reg(xfer));
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done);
        __asm
        {
            mem[cam256_lookup16_add, *xfer, cam, 0 ], sig_done[*sig_pair_ptr]
        }
    }
    return (__xread mem_cam_lookup16_add_out_t *)(xfer);
    _INTRINSIC_END;
}

__intrinsic
__xread mem_cam_lookup32_add_out_t *mem_cam128_lookup32_add_ptr32(
    __mem mem_cam128_in_mem_t *cam,
    __xwrite mem_cam_lookup32_in_t *xfer,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
    )
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_write_reg(xfer));
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done);
        __asm
        {
            mem[cam128_lookup32_add, *xfer, cam, 0 ], sig_done[*sig_pair_ptr]
        }
    }
    return (__xread mem_cam_lookup32_add_out_t *)(xfer);
    _INTRINSIC_END;
}

__intrinsic
__xread mem_cam_lookup32_add_out_t *mem_cam256_lookup32_add_ptr32(
    __mem mem_cam256_in_mem_t *cam,
    __xwrite mem_cam_lookup32_in_t *xfer,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
    )
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_write_reg(xfer));
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done);
        __asm
        {
            mem[cam256_lookup32_add, *xfer, cam, 0 ], sig_done[*sig_pair_ptr]
        }
    }
    return (__xread mem_cam_lookup32_add_out_t *)(xfer);
    _INTRINSIC_END;
}


__intrinsic
__xread mem_cam_lookup8_add_out_t *mem_cam128_lookup8_add_ptr40(
    mem_cam128_ptr40_t cam,
    __xwrite mem_cam_lookup8_in_t *xfer,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
    )
{
    _INTRINSIC_BEGIN;
    {
        uint32_t hi_addr, low_addr;
        CT_ASSERT(__is_write_reg(xfer));
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done);
        _INTRINSIC_CONVERT_HI_LO_ADDRESS(cam);
        __asm
        {
            mem[cam128_lookup8_add, *xfer, hi_addr, <<8, low_addr], sig_done[*sig_pair_ptr]
        }
    }
    return (__xread mem_cam_lookup8_add_out_t *)(xfer);
    _INTRINSIC_END;
}

__intrinsic
__xread mem_cam_lookup8_add_out_t *mem_cam256_lookup8_add_ptr40(
    mem_cam256_ptr40_t cam,
    __xwrite mem_cam_lookup8_in_t *xfer,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
    )
{
    _INTRINSIC_BEGIN;
    {
        uint32_t hi_addr, low_addr;
        CT_ASSERT(__is_write_reg(xfer));
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done);
        _INTRINSIC_CONVERT_HI_LO_ADDRESS(cam);
        __asm
        {
            mem[cam256_lookup8_add, *xfer, hi_addr, <<8, low_addr], sig_done[*sig_pair_ptr]
        }
    }
    return (__xread mem_cam_lookup8_add_out_t *)(xfer);
    _INTRINSIC_END;
}

__intrinsic
__xread mem_cam_lookup16_add_out_t *mem_cam128_lookup16_add_ptr40(
    mem_cam128_ptr40_t cam,
    __xwrite mem_cam_lookup16_in_t *xfer,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
    )
{
    _INTRINSIC_BEGIN;
    {
        uint32_t hi_addr, low_addr;
        CT_ASSERT(__is_write_reg(xfer));
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done);
        _INTRINSIC_CONVERT_HI_LO_ADDRESS(cam);
        __asm
        {
            mem[cam128_lookup16_add, *xfer, hi_addr, <<8, low_addr], sig_done[*sig_pair_ptr]
        }
    }
    return (__xread mem_cam_lookup16_add_out_t *)(xfer);
    _INTRINSIC_END;
}

__intrinsic
__xread mem_cam_lookup16_add_out_t *mem_cam256_lookup16_add_ptr40(
    mem_cam256_ptr40_t cam,
    __xwrite mem_cam_lookup16_in_t *xfer,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
    )
{
    _INTRINSIC_BEGIN;
    {
        uint32_t hi_addr, low_addr;
        CT_ASSERT(__is_write_reg(xfer));
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done);
        _INTRINSIC_CONVERT_HI_LO_ADDRESS(cam);
        __asm
        {
            mem[cam256_lookup16_add, *xfer, hi_addr, <<8, low_addr], sig_done[*sig_pair_ptr]
        }
    }
    return (__xread mem_cam_lookup16_add_out_t *)(xfer);
    _INTRINSIC_END;
}

__intrinsic
__xread mem_cam_lookup32_add_out_t *mem_cam128_lookup32_add_ptr40(
    mem_cam128_ptr40_t cam,
    __xwrite mem_cam_lookup32_in_t *xfer,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
    )
{
    _INTRINSIC_BEGIN;
    {
        uint32_t hi_addr, low_addr;
        CT_ASSERT(__is_write_reg(xfer));
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done);
        _INTRINSIC_CONVERT_HI_LO_ADDRESS(cam);
        __asm
        {
            mem[cam128_lookup32_add, *xfer, hi_addr, <<8, low_addr], sig_done[*sig_pair_ptr]
        }
    }
    return (__xread mem_cam_lookup32_add_out_t *)(xfer);
    _INTRINSIC_END;
}

__intrinsic
__xread mem_cam_lookup32_add_out_t *mem_cam256_lookup32_add_ptr40(
    mem_cam256_ptr40_t cam,
    __xwrite mem_cam_lookup32_in_t *xfer,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
    )
{
    _INTRINSIC_BEGIN;
    {
        uint32_t hi_addr, low_addr;
        CT_ASSERT(__is_write_reg(xfer));
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done);
        _INTRINSIC_CONVERT_HI_LO_ADDRESS(cam);
        __asm
        {
            mem[cam256_lookup32_add, *xfer, hi_addr, <<8, low_addr], sig_done[*sig_pair_ptr]
        }
    }
    return (__xread mem_cam_lookup32_add_out_t *)(xfer);
    _INTRINSIC_END;
}


__intrinsic
__xread mem_cam_lookup8_add_out_t *mem_cam384_lookup8_add_ptr32(
    __mem mem_cam384_in_mem_t *cam,
    __xwrite mem_cam_lookup8_in_t *xfer,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
    )
{
    _INTRINSIC_BEGIN;
    {
        generic_ind_t ind;
        CT_ASSERT(__is_write_reg(xfer));
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done);
        _INTRINSIC_OVERRIDE_LENGTH(ind, CAM_REF_COUNT_384_8);
        __asm alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
        __asm mem[cam384_lookup8_add, *xfer, cam, 0 ], sig_done[*sig_pair_ptr], indirect_ref
    }
    return (__xread mem_cam_lookup8_add_out_t *)(xfer);
    _INTRINSIC_END;
}

__intrinsic
__xread mem_cam_lookup8_add_out_t *mem_cam512_lookup8_add_ptr32(
    __mem mem_cam512_in_mem_t *cam,
    __xwrite mem_cam_lookup8_in_t *xfer,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
    )
{
    _INTRINSIC_BEGIN;
    {
        generic_ind_t ind;
        CT_ASSERT(__is_write_reg(xfer));
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done);
        _INTRINSIC_OVERRIDE_LENGTH(ind, CAM_REF_COUNT_512_8);
        __asm alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
        __asm mem[cam512_lookup8_add, *xfer, cam, 0 ], sig_done[*sig_pair_ptr], indirect_ref
    }
    return (__xread mem_cam_lookup8_add_out_t *)(xfer);
    _INTRINSIC_END;
}

__intrinsic
__xread mem_cam_lookup16_add_out_t *mem_cam384_lookup16_add_ptr32(
    __mem mem_cam384_in_mem_t *cam,
    __xwrite mem_cam_lookup16_in_t *xfer,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
    )
{
    _INTRINSIC_BEGIN;
    {
        generic_ind_t ind;
        CT_ASSERT(__is_write_reg(xfer));
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done);
        _INTRINSIC_OVERRIDE_LENGTH(ind, CAM_REF_COUNT_384_16);
        __asm alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
        __asm mem[cam384_lookup16_add, *xfer, cam, 0 ], sig_done[*sig_pair_ptr], indirect_ref
    }
    return (__xread mem_cam_lookup16_add_out_t *)(xfer);
    _INTRINSIC_END;
}

__intrinsic
__xread mem_cam_lookup16_add_out_t *mem_cam512_lookup16_add_ptr32(
    __mem mem_cam512_in_mem_t *cam,
    __xwrite mem_cam_lookup16_in_t *xfer,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
    )
{
    _INTRINSIC_BEGIN;
    {
        generic_ind_t ind;
        CT_ASSERT(__is_write_reg(xfer));
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done);
        _INTRINSIC_OVERRIDE_LENGTH(ind, CAM_REF_COUNT_512_16);
        __asm alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
        __asm mem[cam512_lookup16_add, *xfer, cam, 0 ], sig_done[*sig_pair_ptr], indirect_ref
    }
    return (__xread mem_cam_lookup16_add_out_t *)(xfer);
    _INTRINSIC_END;
}

__intrinsic
__xread mem_cam_lookup32_add_out_t *mem_cam384_lookup32_add_ptr32(
    __mem mem_cam384_in_mem_t *cam,
    __xwrite mem_cam_lookup32_in_t *xfer,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
    )
{
    _INTRINSIC_BEGIN;
    {
        generic_ind_t ind;
        CT_ASSERT(__is_write_reg(xfer));
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done);
        _INTRINSIC_OVERRIDE_LENGTH(ind, CAM_REF_COUNT_384_32);
        __asm alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
        __asm mem[cam384_lookup32_add, *xfer, cam, 0 ], sig_done[*sig_pair_ptr], indirect_ref
    }
    return (__xread mem_cam_lookup32_add_out_t *)(xfer);
    _INTRINSIC_END;
}

__intrinsic
__xread mem_cam_lookup32_add_out_t *mem_cam512_lookup32_add_ptr32(
    __mem mem_cam512_in_mem_t *cam,
    __xwrite mem_cam_lookup32_in_t *xfer,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
    )
{
    _INTRINSIC_BEGIN;
    {
        generic_ind_t ind;
        CT_ASSERT(__is_write_reg(xfer));
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done);
        _INTRINSIC_OVERRIDE_LENGTH(ind, CAM_REF_COUNT_512_32);
        __asm alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
        __asm mem[cam512_lookup32_add, *xfer, cam, 0 ], sig_done[*sig_pair_ptr], indirect_ref
    }
    return (__xread mem_cam_lookup32_add_out_t *)(xfer);
    _INTRINSIC_END;
}


__intrinsic
__xread mem_cam_lookup8_add_out_t *mem_cam384_lookup8_add_ptr40(
    mem_cam384_ptr40_t cam,
    __xwrite mem_cam_lookup8_in_t *xfer,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
    )
{
    _INTRINSIC_BEGIN;
    {
        generic_ind_t ind;
        uint32_t hi_addr, low_addr;
        CT_ASSERT(__is_write_reg(xfer));
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done);
        _INTRINSIC_CONVERT_HI_LO_ADDRESS(cam);
        _INTRINSIC_OVERRIDE_LENGTH(ind, CAM_REF_COUNT_384_8);
        __asm alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
        __asm mem[cam384_lookup8_add, *xfer, hi_addr, <<8, low_addr], sig_done[*sig_pair_ptr], indirect_ref
    }
    return (__xread mem_cam_lookup8_add_out_t *)(xfer);
    _INTRINSIC_END;
}

__intrinsic
__xread mem_cam_lookup8_add_out_t *mem_cam512_lookup8_add_ptr40(
    mem_cam512_ptr40_t cam,
    __xwrite mem_cam_lookup8_in_t *xfer,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
    )
{
    _INTRINSIC_BEGIN;
    {
        generic_ind_t ind;
        uint32_t hi_addr, low_addr;
        CT_ASSERT(__is_write_reg(xfer));
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done);
        _INTRINSIC_CONVERT_HI_LO_ADDRESS(cam);
        _INTRINSIC_OVERRIDE_LENGTH(ind, CAM_REF_COUNT_512_8);
        __asm alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
        __asm mem[cam512_lookup8_add, *xfer, hi_addr, <<8, low_addr], sig_done[*sig_pair_ptr], indirect_ref
    }
    return (__xread mem_cam_lookup8_add_out_t *)(xfer);
    _INTRINSIC_END;
}

__intrinsic
__xread mem_cam_lookup16_add_out_t *mem_cam384_lookup16_add_ptr40(
    mem_cam384_ptr40_t cam,
    __xwrite mem_cam_lookup16_in_t *xfer,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
    )
{
    _INTRINSIC_BEGIN;
    {
        generic_ind_t ind;
        uint32_t hi_addr, low_addr;
        CT_ASSERT(__is_write_reg(xfer));
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done);
        _INTRINSIC_CONVERT_HI_LO_ADDRESS(cam);
        _INTRINSIC_OVERRIDE_LENGTH(ind, CAM_REF_COUNT_384_16);
        __asm alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
        __asm mem[cam384_lookup16_add, *xfer, hi_addr, <<8, low_addr], sig_done[*sig_pair_ptr], indirect_ref
    }
    return (__xread mem_cam_lookup16_add_out_t *)(xfer);
    _INTRINSIC_END;
}

__intrinsic
__xread mem_cam_lookup16_add_out_t *mem_cam512_lookup16_add_ptr40(
    mem_cam512_ptr40_t cam,
    __xwrite mem_cam_lookup16_in_t *xfer,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
    )
{
    _INTRINSIC_BEGIN;
    {
        generic_ind_t ind;
        uint32_t hi_addr, low_addr;
        CT_ASSERT(__is_write_reg(xfer));
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done);
        _INTRINSIC_CONVERT_HI_LO_ADDRESS(cam);
        _INTRINSIC_OVERRIDE_LENGTH(ind, CAM_REF_COUNT_512_16);
        __asm alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
        __asm mem[cam512_lookup16_add, *xfer, hi_addr, <<8, low_addr], sig_done[*sig_pair_ptr], indirect_ref
    }
    return (__xread mem_cam_lookup16_add_out_t *)(xfer);
    _INTRINSIC_END;
}

__intrinsic
__xread mem_cam_lookup32_add_out_t *mem_cam384_lookup32_add_ptr40(
    mem_cam384_ptr40_t cam,
    __xwrite mem_cam_lookup32_in_t *xfer,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
    )
{
    _INTRINSIC_BEGIN;
    {
        generic_ind_t ind;
        uint32_t hi_addr, low_addr;
        CT_ASSERT(__is_write_reg(xfer));
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done);
        _INTRINSIC_CONVERT_HI_LO_ADDRESS(cam);
        _INTRINSIC_OVERRIDE_LENGTH(ind, CAM_REF_COUNT_384_32);
        __asm alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
        __asm mem[cam384_lookup32_add, *xfer, hi_addr, <<8, low_addr], sig_done[*sig_pair_ptr], indirect_ref
    }
    return (__xread mem_cam_lookup32_add_out_t *)(xfer);
    _INTRINSIC_END;
}

__intrinsic
__xread mem_cam_lookup32_add_out_t *mem_cam512_lookup32_add_ptr40(
    mem_cam512_ptr40_t cam,
    __xwrite mem_cam_lookup32_in_t *xfer,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
    )
{
    _INTRINSIC_BEGIN;
    {
        generic_ind_t ind;
        uint32_t hi_addr, low_addr;
        CT_ASSERT(__is_write_reg(xfer));
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done);
        _INTRINSIC_CONVERT_HI_LO_ADDRESS(cam);
        _INTRINSIC_OVERRIDE_LENGTH(ind, CAM_REF_COUNT_512_32);
        __asm alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
        __asm mem[cam512_lookup32_add, *xfer, hi_addr, <<8, low_addr], sig_done[*sig_pair_ptr], indirect_ref
    }
    return (__xread mem_cam_lookup32_add_out_t *)(xfer);
    _INTRINSIC_END;
}

__intrinsic
__xread mem_cam_lookup24_add_out_t *mem_cam128_lookup24_add_ptr32(
    __mem mem_cam128_in_mem_t *cam,
    __xwrite mem_cam_lookup24_add_in_t *xfer,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
    )
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_write_reg(xfer));
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done);
        __asm
        {
            mem[cam128_lookup24_add, *xfer, cam, 0 ], sig_done[*sig_pair_ptr]
        }
    }
    return (__xread mem_cam_lookup24_add_out_t *)(xfer);
    _INTRINSIC_END;
}

__intrinsic
__xread mem_cam_lookup24_add_out_t *mem_cam256_lookup24_add_ptr32(
    __mem mem_cam256_in_mem_t *cam,
    __xwrite mem_cam_lookup24_add_in_t *xfer,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
    )
{
    _INTRINSIC_BEGIN;
    {
        CT_ASSERT(__is_write_reg(xfer));
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done);
        __asm
        {
            mem[cam256_lookup24_add, *xfer, cam, 0 ], sig_done[*sig_pair_ptr]
        }
    }
    return (__xread mem_cam_lookup24_add_out_t *)(xfer);
    _INTRINSIC_END;
}

/*
 * Lookup24 Add takes different parameters due to high byte usage
 */

__intrinsic
__xread mem_cam_lookup24_add_out_t *mem_cam128_lookup24_add_ptr40(
    mem_cam128_ptr40_t cam,
    __xwrite mem_cam_lookup24_add_in_t *xfer,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
    )
{
    _INTRINSIC_BEGIN;
    {
        uint32_t hi_addr, low_addr;
        CT_ASSERT(__is_write_reg(xfer));
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done);
        _INTRINSIC_CONVERT_HI_LO_ADDRESS(cam);
        __asm
        {
            mem[cam128_lookup24_add, *xfer, hi_addr, <<8, low_addr], sig_done[*sig_pair_ptr]
        }
    }
    return (__xread mem_cam_lookup24_add_out_t *)(xfer);
    _INTRINSIC_END;
}

__intrinsic
__xread mem_cam_lookup24_add_out_t *mem_cam256_lookup24_add_ptr40(
    mem_cam256_ptr40_t cam,
    __xwrite mem_cam_lookup24_add_in_t *xfer,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
    )
{
    _INTRINSIC_BEGIN;
    {
        uint32_t hi_addr, low_addr;
        CT_ASSERT(__is_write_reg(xfer));
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done);
        _INTRINSIC_CONVERT_HI_LO_ADDRESS(cam);
        __asm
        {
            mem[cam256_lookup24_add, *xfer, hi_addr, <<8, low_addr], sig_done[*sig_pair_ptr]
        }
    }
    return (__xread mem_cam_lookup24_add_out_t *)(xfer);
    _INTRINSIC_END;
}


__intrinsic
__xread mem_cam_lookup24_add_out_t *mem_cam384_lookup24_add_ptr32(
    __mem mem_cam384_in_mem_t *cam,
    __xwrite mem_cam_lookup24_add_in_t *xfer,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
    )
{
    _INTRINSIC_BEGIN;
    {
        generic_ind_t ind;
        CT_ASSERT(__is_write_reg(xfer));
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done);
        _INTRINSIC_OVERRIDE_LENGTH(ind, CAM_REF_COUNT_384_24);
        __asm alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
        __asm mem[cam384_lookup24_add, *xfer, cam, 0 ], sig_done[*sig_pair_ptr], indirect_ref
    }
    return (__xread mem_cam_lookup24_add_out_t *)(xfer);
    _INTRINSIC_END;
}

__intrinsic
__xread mem_cam_lookup24_add_out_t *mem_cam512_lookup24_add_ptr32(
    __mem mem_cam512_in_mem_t *cam,
    __xwrite mem_cam_lookup24_add_in_t *xfer,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
    )
{
    _INTRINSIC_BEGIN;
    {
        generic_ind_t ind;
        CT_ASSERT(__is_write_reg(xfer));
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done);
        _INTRINSIC_OVERRIDE_LENGTH(ind, CAM_REF_COUNT_512_24);
        __asm alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
        __asm mem[cam512_lookup24_add, *xfer, cam, 0 ], sig_done[*sig_pair_ptr], indirect_ref
    }
    return (__xread mem_cam_lookup24_add_out_t *)(xfer);
    _INTRINSIC_END;
}


__intrinsic
__xread mem_cam_lookup24_add_out_t *mem_cam384_lookup24_add_ptr40(
    mem_cam384_ptr40_t cam,
    __xwrite mem_cam_lookup24_add_in_t *xfer,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
    )
{
    _INTRINSIC_BEGIN;
    {
        generic_ind_t ind;
        uint32_t hi_addr, low_addr;
        CT_ASSERT(__is_write_reg(xfer));
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done);
        _INTRINSIC_CONVERT_HI_LO_ADDRESS(cam);
        _INTRINSIC_OVERRIDE_LENGTH(ind, CAM_REF_COUNT_384_24);
        __asm alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
        __asm mem[cam384_lookup24_add, *xfer, hi_addr, <<8, low_addr], sig_done[*sig_pair_ptr], indirect_ref
    }
    return (__xread mem_cam_lookup24_add_out_t *)(xfer);
    _INTRINSIC_END;
}

__intrinsic
__xread mem_cam_lookup24_add_out_t *mem_cam512_lookup24_add_ptr40(
    mem_cam512_ptr40_t cam,
    __xwrite mem_cam_lookup24_add_in_t *xfer,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
    )
{
    _INTRINSIC_BEGIN;
    {
        generic_ind_t ind;
        uint32_t hi_addr, low_addr;
        CT_ASSERT(__is_write_reg(xfer));
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done);
        _INTRINSIC_CONVERT_HI_LO_ADDRESS(cam);
        _INTRINSIC_OVERRIDE_LENGTH(ind, CAM_REF_COUNT_512_24);
        __asm alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
        __asm mem[cam512_lookup24_add, *xfer, hi_addr, <<8, low_addr], sig_done[*sig_pair_ptr], indirect_ref
    }
    return (__xread mem_cam_lookup24_add_out_t *)(xfer);
    _INTRINSIC_END;
}

/*
 * Add or Inc
 */

__intrinsic
__xread mem_cam_lookup24_add_inc_out_t *mem_cam128_lookup24_add_inc_ptr32(
    __mem mem_cam128_in_mem_t *cam,
    __xwrite mem_cam_lookup24_add_inc_in_t *xfer,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
    )
{
    _INTRINSIC_BEGIN;
    {
        generic_ind_t ind;
        __ct_assert( !( __is_nfp_arch(__nfp3800, __REVISION_A0)),"mem_cam128_lookup24_add_inc_ptr32 is not supported with the selected chip");
        CT_ASSERT(__is_write_reg(xfer));
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done);
        _INTRINSIC_OVERRIDE_LENGTH(ind, CAM_REF_COUNT_128_24);
        __asm alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
        __asm mem[cam_lookup24_add_inc, *xfer, cam, 0 ], sig_done[*sig_pair_ptr], indirect_ref
    }
    return (__xread mem_cam_lookup24_add_inc_out_t *)(xfer);
    _INTRINSIC_END;
}

__intrinsic
__xread mem_cam_lookup24_add_inc_out_t *mem_cam256_lookup24_add_inc_ptr32(
    __mem mem_cam256_in_mem_t *cam,
    __xwrite mem_cam_lookup24_add_inc_in_t *xfer,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
    )
{
    _INTRINSIC_BEGIN;
    {
        generic_ind_t ind;
        __ct_assert( !( __is_nfp_arch(__nfp3800, __REVISION_A0)),"mem_cam256_lookup24_add_inc_ptr32 is not supported with the selected chip");
        CT_ASSERT(__is_write_reg(xfer));
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done);
        _INTRINSIC_OVERRIDE_LENGTH(ind, CAM_REF_COUNT_256_24);
        __asm alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
        __asm mem[cam_lookup24_add_inc, *xfer, cam, 0 ], sig_done[*sig_pair_ptr], indirect_ref
    }
    return (__xread mem_cam_lookup24_add_inc_out_t *)(xfer);
    _INTRINSIC_END;
}


__intrinsic
__xread mem_cam_lookup24_add_inc_out_t *mem_cam128_lookup24_add_inc_ptr40(
    mem_cam128_ptr40_t cam,
    __xwrite mem_cam_lookup24_add_inc_in_t *xfer,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
    )
{
    _INTRINSIC_BEGIN;
    {
        generic_ind_t ind;
        uint32_t hi_addr, low_addr;
        __ct_assert( !( __is_nfp_arch(__nfp3800, __REVISION_A0)),"mem_cam128_lookup24_add_inc_ptr40 is not supported with the selected chip");
        CT_ASSERT(__is_write_reg(xfer));
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done);
        _INTRINSIC_CONVERT_HI_LO_ADDRESS(cam);
        _INTRINSIC_OVERRIDE_LENGTH(ind, CAM_REF_COUNT_128_24);
        __asm alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
        __asm mem[cam_lookup24_add_inc, *xfer, hi_addr, <<8, low_addr], sig_done[*sig_pair_ptr], indirect_ref
    }
    return (__xread mem_cam_lookup24_add_inc_out_t *)(xfer);
    _INTRINSIC_END;
}

__intrinsic
__xread mem_cam_lookup24_add_inc_out_t *mem_cam256_lookup24_add_inc_ptr40(
    mem_cam256_ptr40_t cam,
    __xwrite mem_cam_lookup24_add_inc_in_t *xfer,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
    )
{
    _INTRINSIC_BEGIN;
    {
        generic_ind_t ind;
        uint32_t hi_addr, low_addr;
        __ct_assert( !( __is_nfp_arch(__nfp3800, __REVISION_A0)),"mem_cam256_lookup24_add_inc_ptr40 is not supported with the selected chip");
        CT_ASSERT(__is_write_reg(xfer));
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done);
        _INTRINSIC_CONVERT_HI_LO_ADDRESS(cam);
        _INTRINSIC_OVERRIDE_LENGTH(ind, CAM_REF_COUNT_256_24);
        __asm alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
        __asm mem[cam_lookup24_add_inc, *xfer, hi_addr, <<8, low_addr], sig_done[*sig_pair_ptr], indirect_ref
    }
    return (__xread mem_cam_lookup24_add_inc_out_t *)(xfer);
    _INTRINSIC_END;
}


__intrinsic
__xread mem_cam_lookup24_add_inc_out_t *mem_cam384_lookup24_add_inc_ptr32(
    __mem mem_cam384_in_mem_t *cam,
    __xwrite mem_cam_lookup24_add_inc_in_t *xfer,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
    )
{
    _INTRINSIC_BEGIN;
    {
        generic_ind_t ind;
        CT_ASSERT(__is_write_reg(xfer));
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done);
        _INTRINSIC_OVERRIDE_LENGTH(ind, CAM_REF_COUNT_384_24);
        __asm alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
        __asm mem[cam384_lookup24_add_inc, *xfer, cam, 0 ], sig_done[*sig_pair_ptr], indirect_ref
    }
    return (__xread mem_cam_lookup24_add_inc_out_t *)(xfer);
    _INTRINSIC_END;
}

__intrinsic
__xread mem_cam_lookup24_add_inc_out_t *mem_cam512_lookup24_add_inc_ptr32(
    __mem mem_cam512_in_mem_t *cam,
    __xwrite mem_cam_lookup24_add_inc_in_t *xfer,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
    )
{
    _INTRINSIC_BEGIN;
    {
        generic_ind_t ind;
        CT_ASSERT(__is_write_reg(xfer));
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done);
        _INTRINSIC_OVERRIDE_LENGTH(ind, CAM_REF_COUNT_512_24);
        __asm alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
        __asm mem[cam512_lookup24_add_inc, *xfer, cam, 0 ], sig_done[*sig_pair_ptr], indirect_ref
    }
    return (__xread mem_cam_lookup24_add_inc_out_t *)(xfer);
    _INTRINSIC_END;
}


__intrinsic
__xread mem_cam_lookup24_add_inc_out_t *mem_cam384_lookup24_add_inc_ptr40(
    mem_cam384_ptr40_t cam,
    __xwrite mem_cam_lookup24_add_inc_in_t *xfer,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
    )
{
    _INTRINSIC_BEGIN;
    {
        generic_ind_t ind;
        uint32_t hi_addr, low_addr;
        CT_ASSERT(__is_write_reg(xfer));
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done);
        _INTRINSIC_CONVERT_HI_LO_ADDRESS(cam);
        _INTRINSIC_OVERRIDE_LENGTH(ind, CAM_REF_COUNT_384_24);
        __asm alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
        __asm mem[cam384_lookup24_add_inc, *xfer, hi_addr, <<8, low_addr], sig_done[*sig_pair_ptr], indirect_ref
    }
    return (__xread mem_cam_lookup24_add_inc_out_t *)(xfer);
    _INTRINSIC_END;
}

__intrinsic
__xread mem_cam_lookup24_add_inc_out_t *mem_cam512_lookup24_add_inc_ptr40(
    mem_cam512_ptr40_t cam,
    __xwrite mem_cam_lookup24_add_inc_in_t *xfer,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
    )
{
    _INTRINSIC_BEGIN;
    {
        generic_ind_t ind;
        uint32_t hi_addr, low_addr;
        CT_ASSERT(__is_write_reg(xfer));
        CT_ASSERT(__is_ct_const(sync));
        CT_ASSERT(sync == sig_done);
        _INTRINSIC_CONVERT_HI_LO_ADDRESS(cam);
        _INTRINSIC_OVERRIDE_LENGTH(ind, CAM_REF_COUNT_512_24);
        __asm alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
        __asm mem[cam512_lookup24_add_inc, *xfer, hi_addr, <<8, low_addr], sig_done[*sig_pair_ptr], indirect_ref
    }
    return (__xread mem_cam_lookup24_add_inc_out_t *)(xfer);
    _INTRINSIC_END;
}

#endif  /* __NFP_MEM_CAM_C__ */
