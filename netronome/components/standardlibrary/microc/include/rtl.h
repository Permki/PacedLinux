/*
 * Copyright (C) 2008-2015 Netronome Systems, Inc.  All rights reserved.
 */

/* NFP RTL definitions */

#ifndef __NFP_RTL_H__
#define __NFP_RTL_H__

#ifndef NFP_LIB_ANY_NFCC_VERSION
    #if (!defined(__NFP_TOOL_NFCC) ||                       \
        (__NFP_TOOL_NFCC < NFP_SW_VERSION(5, 0, 0, 0)) ||   \
        (__NFP_TOOL_NFCC > NFP_SW_VERSION(6, 1, 0, 255)))
        #error "This standard library is not supported for the version of the SDK currently in use."
    #endif
#endif


typedef struct
{
#ifdef __BIGENDIAN
    uint32_t hi;
    uint32_t lo;
#else
    uint32_t lo;
    uint32_t hi;
#endif
} lohi;

int64_t _mul_64(lohi x, lohi y);
uint32_t _div_mod_32(uint32_t x, uint32_t y, uint32_t mod);
int32_t _mod_s32(int32_t x, int32_t y);
int32_t _div_s32(int32_t x, int32_t y);
int32_t _mod_u32(int32_t x, int32_t y);
int32_t _div_u32(int32_t x, int32_t y);
uint64_t _div_mod_64(uint64_t x, uint64_t y, uint32_t mod);
int64_t _mod_s64(int64_t x, int64_t y);
int64_t _div_s64(int64_t x, int64_t y);
int64_t _mod_u64(int64_t x, int64_t y);
int64_t _div_u64(int64_t x, int64_t y);
int64_t _shl_64(int64_t x, uint32_t y);
int64_t _shr_64(int64_t x, uint32_t y);
int64_t _ashr_64(int64_t x, uint32_t y);

void ua_set_8_lmem(__lmem void *q, uint32_t offset, uint32_t val);
void ua_set_16_lmem(__lmem void *q, uint32_t offset, uint32_t val);
void ua_set_32_lmem(__lmem void *q, uint32_t offset, uint32_t val);
void ua_set_8_cls(__cls void *q, uint32_t offset, uint32_t val);
void ua_set_16_cls(__cls void *q, uint32_t offset, uint32_t val);
void ua_set_32_cls(__cls void *q, uint32_t offset, uint32_t val);
void ua_set_64_cls(__cls void *q, uint32_t offset, uint64_t val);
void ua_set_64_sram(__sram void *q, uint32_t offset, uint64_t val);
void ua_set_64_lmem(__lmem void *q, uint32_t offset, uint64_t val);
uint32_t ua_get_u8_lmem(__lmem void *q, uint32_t offset);
uint32_t ua_get_u8_sram(__sram void *q, uint32_t offset);
uint32_t ua_get_u8_cls(__cls void *q, uint32_t offset);
uint32_t ua_get_u16_lmem(__lmem void *q, uint32_t offset);
uint32_t ua_get_u16_sram(__sram void *q, uint32_t offset);
uint32_t ua_get_u16_cls(__cls void *q, uint32_t offset);
uint32_t ua_get_u32_lmem(__lmem void *q, uint32_t offset);
uint32_t ua_get_u32_sram(__sram void *q, uint32_t offset);
uint32_t ua_get_u32_cls(__cls void *q, uint32_t offset);
int32_t ua_get_s8_lmem(__lmem void *q, uint32_t offset);
int32_t ua_get_s8_sram(__sram void *q, uint32_t offset);
int32_t ua_get_s8_cls(__cls void *q, uint32_t offset);
int32_t ua_get_s16_lmem(__lmem void *q, uint32_t offset);
int32_t ua_get_s16_sram(__sram void *q, uint32_t offset);
int32_t ua_get_s16_cls(__cls void *q, uint32_t offset);
int32_t ua_get_s32_lmem(__lmem void *q, uint32_t offset);
int32_t ua_get_s32_sram(__sram void *q, uint32_t offset);
int32_t ua_get_s32_cls(__cls void *q, uint32_t offset);
uint64_t ua_get_64_sram(__sram void *q, uint32_t offset);
uint64_t ua_get_64_cls(__cls void *q, uint32_t offset);
uint64_t ua_get_64_cls40(__cls __addr40 void *q, uint32_t offset);
uint64_t ua_get_64_lmem(__lmem void *q, uint32_t offset);
uint64_t ua_get_u64_sram(__sram void *p, uint32_t offset);
uint64_t ua_get_u64_cls(__cls void *p, uint32_t offset);
uint64_t ua_get_u64_cls40(__cls __addr40 void *p, uint32_t offset);
uint64_t ua_get_u64_lmem(__lmem void *p, uint32_t offset);
int64_t ua_get_s64_sram(__sram void *p, uint32_t offset);
int64_t ua_get_s64_cls(__cls void *p, uint32_t offset);
int64_t ua_get_s64_cls40(__cls __addr40 void *p, uint32_t offset);
int64_t ua_get_s64_lmem(__lmem void *p, uint32_t offset);
void ua_memcpy_sram_sram(__sram void *dst, uint32_t dst_off,
  __sram void *src, uint32_t src_off, uint32_t length);
void ua_memcpy_lmem_lmem(__lmem void *dst, uint32_t dst_off,
  __lmem void *src, uint32_t src_off, uint32_t length);
void ua_memcpy_lmem_sram(__lmem void *dst, uint32_t dst_off,
  __sram void *src, uint32_t src_off, uint32_t length);
void ua_memcpy_lmem_cls(__lmem void *dst, uint32_t dst_off,
  __cls void *src, uint32_t src_off, uint32_t length);
void ua_memcpy_sram_lmem(__sram void *dst, uint32_t dst_off,
  __lmem void *src, uint32_t src_off, uint32_t length);
void ua_memcpy_cls_lmem(__cls void *dst, uint32_t dst_off,
  __lmem void *src, uint32_t src_off, uint32_t length);

void ua_set_8_sram(__sram void *q, uint32_t offset, uint32_t val);
void ua_set_16_sram(__sram void *q, uint32_t offset, uint32_t val);
void ua_set_32_sram(__sram void *q, uint32_t offset, uint32_t val);

void ua_set_8_mem(__mem void *q, uint32_t offset, uint32_t val);
void ua_set_16_mem(__mem void *q, uint32_t offset, uint32_t val);
void ua_set_32_mem(__mem void *q, uint32_t offset, uint32_t val);
void ua_set_64_mem(__mem void *q, uint32_t offset, uint64_t val);

uint32_t ua_get_u8_mem(__mem void *q, uint32_t offset);
uint32_t ua_get_u16_mem(__mem void *q, uint32_t offset);
uint32_t ua_get_u32_mem(__mem void *q, uint32_t offset);

int32_t ua_get_s8_mem(__mem void *q, uint32_t offset);
int32_t ua_get_s16_mem(__mem void *q, uint32_t offset);
int32_t ua_get_s32_mem(__mem void *q, uint32_t offset);

uint64_t ua_get_64_mem(__mem void *q, uint32_t offset);
uint64_t ua_get_64_mem40(__addr40 __mem void *q, uint32_t offset);
uint64_t ua_get_u64_mem(__mem void *p, uint32_t offset);
uint64_t ua_get_u64_mem40(__addr40 __mem void *p, uint32_t offset);
int64_t ua_get_s64_mem(__mem void *p, uint32_t offset);
int64_t ua_get_s64_mem40(__addr40 __mem void *p, uint32_t offset);

void ua_set_8_mem40(__addr40 __mem void *ptr, uint32_t off, uint32_t val);
void ua_set_16_mem40(__addr40 __mem void *ptr, uint32_t off, uint32_t val);
void ua_set_32_mem40(__addr40 __mem void *ptr, uint32_t off, uint32_t val);
void ua_set_64_mem40(__addr40 __mem void *ptr, uint32_t off, uint64_t val);

void ua_memcpy_mem_mem(__mem void *dst, uint32_t dst_off, __mem void *src, uint32_t src_off, uint32_t length);
void ua_memcpy_mem_sram(__mem void *dst, uint32_t dst_off, __sram void *src, uint32_t src_off, uint32_t length);
void ua_memcpy_mem_cls(__mem void *dst, uint32_t dst_off, __cls void *src, uint32_t src_off, uint32_t length);
void ua_memcpy_sram_mem(__sram void *dst, uint32_t dst_off, __mem void *src, uint32_t src_off, uint32_t length);
void ua_memcpy_cls_mem(__cls void *dst, uint32_t dst_off, __mem void *src, uint32_t src_off, uint32_t length);
void ua_memcpy_sram_cls(__sram void *dst, uint32_t dst_off, __cls void *src, uint32_t src_off, uint32_t length);
void ua_memcpy_cls_sram(__cls void *dst, uint32_t dst_off, __sram void *src, uint32_t src_off, uint32_t length);
void ua_memcpy_cls_cls(__cls void *dst, uint32_t dst_off, __cls void *src, uint32_t src_off, uint32_t length);
void ua_memcpy_lmem_mem(__lmem void *dst, uint32_t dst_off, __mem void *src, uint32_t src_off, uint32_t length);
void ua_memcpy_mem_lmem(__mem void *dst, uint32_t dst_off, __lmem void *src, uint32_t src_off, uint32_t length);

void ua_memcpy_lmem_mem40(__lmem void *dst, uint32_t dst_off,
  __addr40 __mem void *src, uint32_t src_off, uint32_t length);
void ua_memcpy_lmem_cls40(__lmem void *dst, uint32_t dst_off,
  __cls __addr40 void *src, uint32_t src_off, uint32_t length);
void ua_memcpy_sram_mem40(__sram void *dst, uint32_t dst_off,
  __addr40 __mem void *src, uint32_t src_off, uint32_t length);
void ua_memcpy_sram_cls40(__sram void *dst, uint32_t dst_off,
  __cls __addr40 void *src, uint32_t src_off, uint32_t length);
void ua_memcpy_mem_mem40(__mem void *dst, uint32_t dst_off,
  __addr40 __mem void *src, uint32_t src_off, uint32_t length);
void ua_memcpy_mem_cls40(__mem void *dst, uint32_t dst_off,
  __cls __addr40 void *src, uint32_t src_off, uint32_t length);
void ua_memcpy_mem40_lmem(__addr40 __mem void *dst, uint32_t dst_off,
  __lmem void *src, uint32_t src_off, uint32_t length);
void ua_memcpy_mem40_sram(__addr40 __mem void *dst, uint32_t dst_off,
  __sram void *src, uint32_t src_off, uint32_t length);
void ua_memcpy_mem40_mem(__addr40 __mem void *dst, uint32_t dst_off,
  __mem void *src, uint32_t src_off, uint32_t length);
void ua_memcpy_mem40_mem40(__addr40 __mem void *dst, uint32_t dst_off,
  __addr40 __mem void *src, uint32_t src_off, uint32_t length);
void ua_memcpy_mem40_cls(__addr40 __mem void *dst, uint32_t dst_off,
  __cls void *src, uint32_t src_off, uint32_t length);
void ua_memcpy_mem40_cls40(__addr40 __mem void *dst, uint32_t dst_off,
  __cls __addr40 void *src, uint32_t src_off, uint32_t length);
void ua_memcpy_cls_mem40(__cls void *dst, uint32_t dst_off,
  __addr40 __mem void *src, uint32_t src_off, uint32_t length);
void ua_memcpy_cls_cls40(__cls void *dst, uint32_t dst_off,
  __cls __addr40 void *src, uint32_t src_off, uint32_t length);
void ua_memcpy_cls40_lmem(__cls __addr40 void *dst, uint32_t dst_off,
  __lmem void *src, uint32_t src_off, uint32_t length);
void ua_memcpy_cls40_sram(__cls __addr40 void *dst, uint32_t dst_off,
  __sram void *src, uint32_t src_off, uint32_t length);
void ua_memcpy_cls40_mem(__cls __addr40 void *dst, uint32_t dst_off,
  __mem void *src, uint32_t src_off, uint32_t length);
void ua_memcpy_cls40_mem40( __cls __addr40 void *dst, uint32_t dst_off,
  __addr40 __mem void *src, uint32_t src_off, uint32_t length);
void ua_memcpy_cls40_cls(__cls __addr40 void *dst, uint32_t dst_off,
  __cls void *src, uint32_t src_off, uint32_t length);
void ua_memcpy_cls40_cls40(__cls __addr40 void *dst, uint32_t dst_off,
  __cls __addr40 void *src, uint32_t src_off, uint32_t length);

uint32_t ua_get_u32_cls40(__cls __addr40 void *q, uint32_t offset);
uint32_t ua_get_u16_cls40(__cls __addr40 void *q, uint32_t offset);
uint32_t ua_get_u8_cls40(__cls __addr40 void *q, uint32_t offset);
int32_t ua_get_s32_cls40(__cls __addr40 void *q, uint32_t offset);
int32_t ua_get_s16_cls40(__cls __addr40 void *q, uint32_t offset);
int32_t ua_get_s8_cls40(__cls __addr40 void *q, uint32_t offset);
void ua_set_64_cls40(__cls __addr40 void *q, uint32_t offset, uint64_t val);
void ua_set_32_cls40(__cls __addr40 void *q, uint32_t offset, uint32_t val);
void ua_set_16_cls40(__cls __addr40 void *q, uint32_t offset, uint32_t val);
void ua_set_8_cls40(__cls __addr40 void *q, uint32_t offset, uint32_t val);

uint32_t ua_get_u32_mem40(__addr40 __mem void *q, uint32_t offset);
uint32_t ua_get_u16_mem40(__addr40 __mem void *q, uint32_t offset);
uint32_t ua_get_u8_mem40(__addr40 __mem void *q, uint32_t offset);
int32_t ua_get_s32_mem40(__addr40 __mem void *q, uint32_t offset);
int32_t ua_get_s16_mem40(__addr40 __mem void *q, uint32_t offset);
int32_t ua_get_s8_mem40(__addr40 __mem void *q, uint32_t offset);

__noinline void exit(int32_t status);
void exit_thread(int32_t status);


#endif /* __NFP_RTL_H__ */
