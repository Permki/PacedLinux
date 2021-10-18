/*
 * Copyright (C) 2008-2015 Netronome Systems, Inc.  All rights reserved.
 */

#ifndef __RTL_C__
#define __RTL_C__

#include <nfp.h>
#include <rtl.h>

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


/* 64-bit by 64-bit multiply with 64-bit product */
/* works equally well for signed or uint32_t types */
extern int64_t _mul_64(lohi x, lohi y)
{
    return (int64_t)x.lo * (int64_t)y.lo +
            (((int64_t)x.lo * (int64_t)y.hi) << 32) +
            (((int64_t)x.hi * (int64_t)y.lo) << 32);
}

/*
 * 32-bit uint32_t divide  returning either quotient or remainder
 * if mod == 1 then x mod y
 * else x div y
 */

extern uint32_t _div_mod_32(uint32_t x, uint32_t y, uint32_t mod)
{
    int32_t lo = x;
    uint32_t hi = 0;
    uint32_t i;

    if (y == 0)
    {
        //puts("error: div or mod by zero\n");
        //exit(1);
        // we define x % 0 or x / 0 is 0xffffffff
        return 0xffffffff;
    }

    /* hi and lo form a 64-bit dividend.  We shift one bit at a time */
    /* from the lo to the hi word.  Then we check the hi word against */
    /* the divisor.  If it is greater, we subtract the divisor and put */
    /* a 1 in the low bit of lo.  After 32 iterations, lo is the quotient */
    /* and hi is the remainder */

    for (i = 0; i < 32; i++)
    {
        hi <<= 1;
        if (lo < 0)
        {
            hi |= 1;
        }
        lo <<= 1;

        if (hi >= y)
        {
            hi -= y;
            lo |= 1;
        }
    }

    if (mod)
    {
        return hi;
    }
    else
    {
        return lo;
    }
}

/* 32-bit signed remainder */

extern int32_t _mod_s32(int32_t x, int32_t y)
{
    int32_t neg = 0;
    int32_t result;

    if (y == 0)
    {
        // we define x % 0 is 0xffffffff
        return 0xffffffff;
    }

    if (x < 0)
    {
        neg = 1;
        x = -x;
    }
    if (y < 0)
    {
        y = -y;
    }

    result = _div_mod_32(x, y, 1);
    if (neg)
    {
        result = -result;
    }
    return result;
}

/* 32-bit signed quotient */

extern int32_t _div_s32(int32_t x, int32_t y)
{
    int32_t neg;
    int32_t result;

    if (y == 0)
    {
        // we define x / 0 is 0xffffffff
        return 0xffffffff;
    }

    neg = 0;
    if (x < 0)
    {
        neg = 1;
        x = -x;
    }
    if (y < 0)
    {
        neg ^= 1;
        y = -y;
    }

    result = _div_mod_32(x, y, 0);
    if (neg)
    {
        result = -result;
    }
    return result;
}

/* 32-bit uint32_t remainder */

extern int32_t _mod_u32(int32_t x, int32_t y)
{
    return _div_mod_32(x, y, 1);
}


/* 32-bit uint32_t quotient */

extern int32_t _div_u32(int32_t x, int32_t y)
{
    return _div_mod_32(x, y, 0);
}


/* 64-bit uint32_t divide returning either quotient or remainder */

extern uint64_t _div_mod_64(uint64_t x, uint64_t y, uint32_t mod)
{
    int64_t lo = x;
    uint64_t hi = 0;
    uint32_t i;

    if (y == 0)
    {
        //puts("error: div or mod by zero\n");
        // we define x % 0 or x / 0 is 0xffffffff
        return 0xffffffffffffffff;
    }

    /* hi and lo form a 128-bit dividend.  We shift one bit at a time */
    /* from the lo to the hi word.  Then we check the hi word against */
    /* the divisor.  If it is greater, we subtract the divisor and put */
    /* a 1 in the low bit of lo.  After 64 iterations, lo is the quotient */
    /* and hi is the remainder */

    for (i = 0; i < 64; i++)
    {
        hi <<= 1;
        if (lo < 0)
        {
            hi |= 1;
        }
        lo <<= 1;

        if (hi >= y)
        {
            hi -= y;
            lo |= 1;
        }
    }

    if (mod)
    {
        return hi;
    }
    else
    {
        return lo;
    }
}

/* 64-bit signed remainder */

extern int64_t _mod_s64(int64_t x, int64_t y)
{
    int32_t neg = 0;
    int64_t result;

    if (y == 0)
    {
        // we define x % 0 is 0xffffffff
        return 0xffffffffffffffff;
    }

    if (x < 0)
    {
        neg = 1;
        x = -x;
    }
    if (y < 0)
    {
        y = -y;
    }

    result = _div_mod_64(x, y, 1);
    if (neg)
    {
        result = -result;
    }
    return result;
}

/* 64-bit signed quotient */

extern int64_t _div_s64(int64_t x, int64_t y)
{
    int32_t neg;
    int64_t result;

    if (y == 0)
    {
        // we define x / 0 is 0xffffffff
        return 0xffffffffffffffff;
    }

    neg = 0;
    if (x < 0)
    {
        neg = 1;
        x = -x;
    }
    if (y < 0)
    {
        neg ^= 1;
        y = -y;
    }

    result = _div_mod_64(x, y, 0);
    if (neg)
    {
        result = -result;
    }
    return result;
}

/* 64-bit uint32_t remainder */

extern int64_t _mod_u64(int64_t x, int64_t y)
{
    return _div_mod_64(x, y, 1);
}


/* 64-bit uint32_t quotient */

extern int64_t _div_u64(int64_t x, int64_t y)
{
    return _div_mod_64(x, y, 0);
}


/* 64-bit shift left */
extern int64_t _shl_64(int64_t x, uint32_t y)
{
    int64_t result;
    int32_t thirtytwo = 32;
    int32_t y1;

    /* truncate shift count to 6 bits */

    y &= 63;

    if (y >= thirtytwo) __asm
    {
        alu         [result+4, y, AND, 0]
        alu_shf     [result, --, B, x+4, <<indirect]
    }
    else if (y != 0) __asm
    {
        alu         [y1, thirtytwo, -, y]
        alu         [--, y1, OR, 0]
        dbl_shf     [result, x, x+4, >>indirect]
        alu         [--, y, OR, 0]
        alu_shf     [result+4, --, B, x+4, <<indirect]
    }
    else
    {
        result = x;
    }
    return result;
}

extern int64_t _shr_64(int64_t x, uint32_t y)
{
    int64_t result;

    /* truncate shift count to 6 bits */

    y &= 63;

    if (y > 31) __asm
    {
        alu         [result, y, AND, 0]
        alu_shf     [result+4, --, B, x, >>indirect]
    }
    else __asm
    {
        alu         [--, y, OR, 0]
        dbl_shf     [result+4, x, x+4, >>indirect]
        alu         [--, y, OR, 0]
        alu_shf     [result, --, B, x, >>indirect]
    }
    return result;
}

extern int64_t _ashr_64(int64_t x, uint32_t y)
{
    typedef union
    {
        struct
        {
            int32_t hi;
            int32_t lo;
        } i;
        int64_t ll;
    } two_ints;
    two_ints result;

    /* get sign extend of x */

    int32_t ext = ((two_ints *)(void *)&x)->i.hi >> 31;

    /* truncate shift count to 6 bits */

    y &= 63;

    if (y > 31)
    {
        result.i.hi = ext;
        result.i.lo = dbl_shr(ext, ((two_ints *)(void *)&x)->i.hi, y - 32);
    }
    else
    {
        result.i.hi = dbl_shr(ext, ((two_ints *)(void *)&x)->i.hi, y);
        result.i.lo = dbl_shr(((two_ints *)(void *)&x)->i.hi, ((two_ints *)(void *)&x)->i.lo, y);
    }

    return result.ll;
}


/*
 * ua_set_
 */

/* unaligned local_mem pointer q:
        *(uint8_t *)((int32_t)q+offset) = (uint8_t)val;
 */

void ua_set_8_lmem(__lmem void *q, uint32_t offset, uint32_t val)
{
    uint32_t v;
    uint32_t shift;

    __lmem uint32_t *p = (__lmem uint32_t *)((uint32_t)q + offset);
    v = *p;
    shift = 24 - (24 & ((uint32_t)p << 3));
    v = v & ~(0xff << shift) | ((val & 0xff) << shift);
    *p = v;
}


/* unaligned local_mem pointer q:
        *(uint16_t *)((int32_t)q+offset) = (uint16_t)val;
 */
void ua_set_16_lmem(__lmem void *q, uint32_t offset, uint32_t val)
{
    __lmem uint32_t *ap;
    uint32_t shift;
    uint32_t mask1, mask2, v1, v2;
    uint32_t p = (uint32_t)q + offset;
    uint64_t l;

    ap = (void *)p;
    shift = (((uint32_t)p << 3) & (3 << 3));
    v2 = val << 16;
    v1 = dbl_shr(0, v2, shift);
    v2 = dbl_shr(v2, 0, shift);
    mask1 = dbl_shr(0, 0xffff0000 , shift);
    mask2 = dbl_shr(0xffff0000, 0, shift);
    *ap++ = (*ap & ~ mask1) | (v1 & mask1);
    *ap = (*ap & ~mask2) | (v2 & mask2);
}


/* unaligned local_mem pointer q:
        *(uint32_t *)((int32_t)q+offset) = (uint32_t)val;
 */
void ua_set_32_lmem(__lmem void *q, uint32_t offset, uint32_t val)
{
    __lmem uint32_t *ap;
    uint32_t shift;
    uint32_t mask1, mask2, v1, v2;
    uint32_t p = (uint32_t) q + offset;
    uint64_t l;

    ap = (void *)p;
    shift = (((uint32_t)p << 3) & (3 << 3));
    v2 = val;
    v1 = dbl_shr(0, v2, shift);
    v2 = dbl_shr(v2, 0, shift);
    mask1 = dbl_shr(0, 0xffffffff , shift);
    mask2 = dbl_shr(0xffffffff, 0, shift);
    *ap++ = (*ap & ~ mask1) | (v1 & mask1);
    *ap = (*ap & ~mask2) | (v2 & mask2);
}


/* unaligned cls pointer q:
        *(uint8_t *)((int32_t)q+offset) = (uint8_t)val;
 */
void ua_set_8_cls(__cls void *q, uint32_t offset, uint32_t val)
{
    __cls uint8_t *p = (__cls uint8_t *)((__cls uint8_t *)q + offset);

    *p = val;
}


/* unaligned cls pointer q:
        *(uint16_t *)((int32_t)q+offset) = (uint16_t)val;
 */
void ua_set_16_cls(__cls void *q, uint32_t offset, uint32_t val)
{
    __cls uint16_t *p = (__cls uint16_t *)((__cls uint8_t *)q + offset);

    *p = val;
}


/* unaligned cls pointer q:
        *(uint32_t *)((int32_t)q+offset) = (uint32_t)val;
 */
void ua_set_32_cls(__cls void *q, uint32_t offset, uint32_t val)
{
    __cls uint32_t *p = (__cls uint32_t *)((__cls uint8_t *)q + offset);

    *p = val;
}


typedef union
{
    struct
    {
        uint32_t a;
        uint32_t b;
        uint32_t c;
    };
    int64_t ll;
} threelong;


/* unaligned cls pointer q:
        *(uint64_t *)((int32_t)q+offset) = val;
 */
void ua_set_64_cls(__cls void *q, uint32_t offset, uint64_t val)
{
    __cls uint64_t *p = (__cls uint64_t *)((__cls uint8_t *)q + offset);

    *p = val;
}


/* unaligned sram pointer q:
        *(uint64_t *)((int32_t)q+offset) = val;
 */
void ua_set_64_sram(__sram void *q, uint32_t offset, uint64_t val)
{
    threelong v;
    uint32_t shift, a, b, c;
    __sram uint32_t *p = (__sram uint32_t *)((int32_t)q + offset);

    v = *(__sram threelong *)p;
    shift = ((int32_t)p & 3) << 3;
    if (shift)
    {
        a = (uint32_t)(val >> 32);
        b = (uint32_t)val;

        c = b << (32 - shift);
        b = dbl_shr(a, b, shift);
        a = a >> shift;

        v.c = v.c & (0xffffffff >> shift) | c;
        v.b = b;
        v.a = v.a & (0xffffffff << (32 - shift)) | a;
    }
    else
    {
        v.a = (uint32_t)(val >> 32);
        v.b = (uint32_t)val;
    }

    *(__sram threelong *)p = v;
}


/* unaligned local_mem pointer q:
        *(uint64_t *)((int32_t)q+offset) = val;
 */
void ua_set_64_lmem(__lmem void *q, uint32_t offset, uint64_t val)
{
    threelong v;
    uint32_t shift, a, b, c;
    __lmem uint32_t *p = (__lmem uint32_t *)((int32_t)q + offset);

    v = *(__lmem threelong *)p;
    shift = ((int32_t)p & 3) << 3;
    if (shift)
    {
        a = (uint32_t)(val >> 32);
        b = (uint32_t)val;

        c = b << (32 - shift);
        b = dbl_shr(a, b, shift);
        a = a >> shift;

        v.c = v.c & (0xffffffff >> shift) | c;
        v.b = b;
        v.a = v.a & (0xffffffff << (32 - shift)) | a;
    }
    else
    {
        v.a = (uint32_t)(val >> 32);
        v.b = (uint32_t)val;
    }

    *(__lmem threelong *)p = v;
}

/* unaligned local_mem pointer q:
        return *(uint8_t *)((int32_t)q+offset);
 */
uint32_t ua_get_u8_lmem(__lmem void *q, uint32_t offset)
{
    uint32_t v;
    uint32_t shift;
    __lmem uint32_t *p = (__lmem uint32_t *)((int32_t)q + offset);

    v = *p;
    shift = 24 - (24 & ((uint32_t)p << 3));
    return (v >> shift) & 0xff;
}


/* unaligned sram pointer q:
        return *(uint8_t *)((int32_t)q+offset);
 */
// TODO This function implementation is not used even when called implicitly,
// instead a nfcc (intrinsic) implementation is used
// Investigate if this can be removed (and prototype in rtl.h)
uint32_t ua_get_u8_sram(__sram void *q, uint32_t offset)
{
    uint32_t v;
    uint32_t shift;
    __sram uint32_t *p = (__sram uint32_t *)((int32_t)q + offset);

    v = *p;
    local_csr_write(local_csr_mailbox1, 0xab);
//     shift = (3 - (int32_t)p) << 3;
    shift = 24 - (24 & ((uint32_t)p << 3));
    return (v >> shift) & 0xff;
}


/* unaligned cls pointer q:
        return *(uint8_t *)((int32_t)q+offset);
 */
uint32_t ua_get_u8_cls(__cls void *q, uint32_t offset)
{
    __cls uint8_t *p = (__cls uint8_t *)q + offset;

    return *p;
}


/* unaligned local_mem pointer q:
        return *(uint16_t *)((int32_t)q+offset);
 */
uint32_t ua_get_u16_lmem(__lmem void *q, uint32_t offset)
{
    uint64_t v;
    uint32_t shift;
    __lmem uint64_t *p = (__lmem uint64_t *)((int32_t)q + offset);

    v = *p;
    shift = (6 - ((int32_t)p & 3)) << 3;
    return (uint32_t)(v >> shift) & 0xffff;
}


/* unaligned sram pointer q:
        return *(uint16_t *)((int32_t)q+offset);
 */
uint32_t ua_get_u16_sram(__sram void *q, uint32_t offset)
{
    uint64_t v;
    uint32_t shift;

    __sram uint64_t *p = (__sram uint64_t *)((int32_t)q + offset);
    v = *p;
    shift = (6 - ((int32_t)p & 3)) << 3;
    return (uint32_t)(v >> shift) & 0xffff;
}


/* unaligned cls pointer q:
        return *(uint16_t *)((int32_t)q+offset);
 */
uint32_t ua_get_u16_cls(__cls void *q, uint32_t offset)
{
    __cls uint16_t *p = (__cls uint16_t *)((__cls uint8_t *)q + offset);

    return *p;
}


/* unaligned local_mem pointer q:
        return *(uint32_t *)((int32_t)q+offset);
 */
uint32_t ua_get_u32_lmem(__lmem void *q, uint32_t offset)
{
    uint64_t v;
    uint32_t shift;
    __lmem uint64_t *p = (__lmem uint64_t *)((int32_t)q + offset);

    v = *p;
    shift = (4 - ((int32_t)p & 3)) << 3;
    return (uint32_t)(v >> shift);
}


/* unaligned sram pointer q:
        return *(uint32_t *)((int32_t)q+offset);
 */
uint32_t ua_get_u32_sram(__sram void *q, uint32_t offset)
{
    uint64_t v;
    uint32_t shift;
    __sram uint64_t *p = (__sram uint64_t *)((int32_t)q + offset);

    v = *p;
    shift = (4 - ((int32_t)p & 3)) << 3;
    return (uint32_t)(v >> shift);
}


/* unaligned cls pointer q:
        return *(uint32_t *)((int32_t)q+offset);
 */
uint32_t ua_get_u32_cls(__cls void *q, uint32_t offset)
{
    __cls uint32_t *p = (__cls uint32_t *)((__cls uint8_t *)q + offset);

    return *p;
}


/* unaligned local_mem pointer q:
        return *(int8_t *)((int32_t)q+offset);
 */
int32_t ua_get_s8_lmem(__lmem void *q, uint32_t offset)
{
    uint32_t v;
    uint32_t shift;
    __lmem uint32_t *p = (__lmem uint32_t *)((int32_t)q + offset);

    v = *p;
    shift = 24 - (24 & ((uint32_t)p << 3));
    return (int32_t)(int8_t)((v >> shift) & 0xff);
}


/* unaligned sram pointer q:
        return *(int8_t *)((int32_t)q+offset);
 */
int32_t ua_get_s8_sram(__sram void *q, uint32_t offset)
{
    uint32_t v;
    uint32_t shift;
    __sram uint32_t *p = (__sram uint32_t *)((int32_t)q + offset);

    v = *p;
    shift = 24 - (24 & ((uint32_t)p << 3));
    return (int32_t)(int8_t)((v >> shift) & 0xff);
}


/* unaligned cls pointer q:
        return *(int8_t *)((int32_t)q+offset);
 */
int32_t ua_get_s8_cls(__cls void *q, uint32_t offset)
{
    __cls int8_t *p = (__cls int8_t *)q + offset;

    return *p;
}


/* unaligned local_mem pointer q:
        return *(int16_t *)((int32_t)q+offset);
 */
int32_t ua_get_s16_lmem(__lmem void *q, uint32_t offset)
{
    uint64_t v;
    uint32_t shift;
    __lmem uint64_t *p = (__lmem uint64_t *)((int32_t)q + offset);

    v = *p;
    shift = (6 - ((int32_t)p & 3)) << 3;
    return (int32_t)(int16_t)((uint32_t)(v >> shift) & 0xffff);
}


/* unaligned sram pointer q:
        return *(int16_t *)((int32_t)q+offset);
 */
int32_t ua_get_s16_sram(__sram void *q, uint32_t offset)
{
    uint64_t v;
    uint32_t shift;
    __sram uint64_t *p = (__sram uint64_t *)((int32_t)q + offset);

    v = *p;
    shift = (6 - ((int32_t)p & 3)) << 3;
    return (int32_t)(int16_t)((uint32_t)(v >> shift) & 0xffff);
}


/* unaligned cls pointer q:
        return *(int16_t *)((int32_t)q+offset);
 */
int32_t ua_get_s16_cls(__cls void *q, uint32_t offset)
{
    __cls int16_t *p = (__cls int16_t *)((__cls uint8_t *)q + offset);

    return *p;
}


/* unaligned local_mem pointer q:
        return *(int32_t *)((int32_t)q+offset);
 */
int32_t ua_get_s32_lmem(__lmem void *q, uint32_t offset)
{
    uint64_t v;
    uint32_t shift;
    __lmem uint64_t *p = (__lmem uint64_t *)((int32_t)q + offset);

    v = *p;
    shift = (4 - ((int32_t)p & 3)) << 3;
    return (int32_t)(v >> shift);
}


/* unaligned sram pointer q:
        return *(int32_t *)((int32_t)q+offset);
 */
int32_t ua_get_s32_sram(__sram void *q, uint32_t offset)
{
    uint64_t v;
    uint32_t shift;
    __sram uint64_t *p = (__sram uint64_t *)((int32_t)q + offset);

    v = *p;
    shift = (4 - ((int32_t)p & 3)) << 3;
    return (int32_t)(v >> shift);
}


/* unaligned cls pointer q:
        return *(int32_t *)((int32_t)q+offset);
 */
int32_t ua_get_s32_cls(__cls void *q, uint32_t offset)
{
    __cls int32_t *p = (__cls int32_t *)((__cls uint8_t *)q + offset);

    return *p;
}


/* unaligned sram pointer q:
        return *(uint64_t *)((int32_t)q+offset);
 */
uint64_t ua_get_64_sram(__sram void *q, uint32_t offset)
{
    threelong v;
    uint32_t shift, a, b;
    __sram uint32_t *p = (__sram uint32_t *)((int32_t)q + offset);

    v = *(__sram threelong *)p;
    shift = ((int32_t)p & 3) << 3;
    if (shift)
    {
        a = dbl_shl(v.a, v.b, shift);
        b = dbl_shl(v.b, v.c, shift);
        v.a = a;
        v.b = b;
    }

    return (uint64_t)v.ll;
}


/* unaligned cls pointer q:
        return *(uint64_t *)((int32_t)q+offset);
 */
uint64_t ua_get_64_cls(__cls void *q, uint32_t offset)
{
    __cls uint64_t *p = (__cls uint64_t *)((__cls uint8_t *)q + offset);

    return *p;
}


uint64_t ua_get_64_cls40(__cls __addr40 void *q, uint32_t offset)
{
    __cls __addr40 uint64_t *p = (__cls __addr40 uint64_t *)((__cls __addr40 uint8_t *)q + offset);

    return *p;
}


/* unaligned local_mem pointer q:
        return *(uint64_t *)((int32_t)q+offset);
 */
uint64_t ua_get_64_lmem(__lmem void *q, uint32_t offset)
{
    threelong v;
    uint32_t shift, a, b;
    __lmem uint32_t *p = (__lmem uint32_t *)((int32_t)q + offset);

    v = *(__lmem threelong *)p;
    shift = ((int32_t)p & 3) << 3;
    if (shift)
    {
        a = dbl_shl(v.a, v.b, shift);
        b = dbl_shl(v.b, v.c, shift);
        v.a = a;
        v.b = b;
    }

    return (uint64_t)v.ll;
}


/* unaligned sram pointer q:
        return *(uint64_t *)((int32_t)q+offset);
 */
uint64_t ua_get_u64_sram(__sram void *p, uint32_t offset)
{
    return ua_get_64_sram(p, offset);
}


/* unaligned cls pointer q:
        return *(uint64_t *)((int32_t)q+offset);
 */
uint64_t ua_get_u64_cls(__cls void *p, uint32_t offset)
{
    return ua_get_64_cls(p, offset);
}


uint64_t ua_get_u64_cls40(__cls __addr40 void *p, uint32_t offset)
{
    return ua_get_64_cls40(p, offset);
}


/* unaligned local_mem pointer q:
        return *(uint64_t *)((int32_t)q+offset);
 */
uint64_t ua_get_u64_lmem(__lmem void *p, uint32_t offset)
{
    return ua_get_64_lmem(p, offset);
}


/* unaligned sram pointer q:
        return *(int64_t *)((int32_t)q+offset);
 */
int64_t ua_get_s64_sram(__sram void *p, uint32_t offset)
{
    return (int64_t)ua_get_64_sram(p, offset);
}


/* unaligned cls pointer q:
        return *(int64_t *)((int32_t)q+offset);
 */
int64_t ua_get_s64_cls(__cls void *p, uint32_t offset)
{
    return (int64_t)ua_get_64_cls(p, offset);
}


int64_t ua_get_s64_cls40(__cls __addr40 void *p, uint32_t offset)
{
    return (int64_t)ua_get_64_cls40(p, offset);
}


/* unaligned local_mem pointer q:
        return *(int64_t *)((int32_t)q+offset);
 */
int64_t ua_get_s64_lmem(__lmem void *p, uint32_t offset)
{
    return (int64_t)ua_get_64_lmem(p, offset);
}


/*
   unaligned memory copy: sram <- sram
 */
void ua_memcpy_sram_sram(
    __sram void *dst, uint32_t dst_off,
    __sram void *src, uint32_t src_off,
    uint32_t length)
{
    while (length >= 8)
    {
        ua_set_64(dst, dst_off, ua_get_u64(src, src_off));
        length -= 8;
        dst_off += 8;
        src_off += 8;
    }
    if (length >= 4)
    {
        ua_set_32(dst, dst_off, ua_get_u32(src, src_off));
        length -= 4;
        src_off += 4;
        dst_off += 4;
    }
    if (length >= 2)
    {
        ua_set_16(dst, dst_off, ua_get_u16(src, src_off));
        length -= 2;
        src_off += 2;
        dst_off += 2;
    }
    if (length)
    {
        ua_set_8(dst, dst_off, ua_get_u8(src, src_off));
    }
}


/*
   unaligned memory copy: lmem <- lmem
 */
void ua_memcpy_lmem_lmem(
    __lmem void *dst, uint32_t dst_off,
    __lmem void *src, uint32_t src_off,
    uint32_t length)
{
    __lmem int8_t *p;
    __lmem int8_t *q;
    uint32_t i;

    p = ((__lmem int8_t *) dst) + dst_off;
    q = ((__lmem int8_t *) src) + src_off;
    for ( i = 0; i < length; i++ )
    {
        *p++ = *q++;
    }
}

/*
   unaligned memory copy: lmem <- sram
 */
void ua_memcpy_lmem_sram(
    __lmem void *dst, uint32_t dst_off,
    __sram void *src, uint32_t src_off,
    uint32_t length)
{
    __lmem int8_t *p;
    __sram int8_t *q;
    uint32_t i;

    p = ((__lmem int8_t *) dst) + dst_off;
    q = ((__sram int8_t *) src) + src_off;
    for ( i = 0; i < length; i++ )
    {
        *p++ = *q++;
    }
}


/*
   unaligned memory copy: lmem <- cls
 */
void ua_memcpy_lmem_cls(
    __lmem void *dst, uint32_t dst_off,
    __cls void *src, uint32_t src_off,
    uint32_t length)
{
    __lmem int8_t *p;
    __cls int8_t *q;
    uint32_t i;

    p = ((__lmem int8_t *) dst) + dst_off;
    q = ((__cls int8_t *) src) + src_off;
    for ( i = 0; i < length; i++ )
    {
        *p++ = *q++;
    }
}


/*
   unaligned memory copy: sram <- lmem
 */
void ua_memcpy_sram_lmem(
    __sram void *dst, uint32_t dst_off,
    __lmem void *src, uint32_t src_off,
    uint32_t length)
{
    __sram int8_t *p;
    __lmem int8_t *q;
    uint32_t i;

    p = ((__sram int8_t *) dst) + dst_off;
    q = ((__lmem int8_t *) src) + src_off;
    for ( i = 0; i < length; i++ )
    {
        ua_set_8_sram(p++, 0, ua_get_s8_lmem(q++, 0));
    }
}


/*
   unaligned memory copy: cls <- lmem
 */
void ua_memcpy_cls_lmem(
    __cls void *dst, uint32_t dst_off,
    __lmem void *src, uint32_t src_off,
    uint32_t length)
{
    __lmem int8_t *q;
    __cls int8_t *p;
    uint32_t i;

    p = ((__cls int8_t *) dst) + dst_off;
    q = ((__lmem int8_t *) src) + src_off;
    for ( i = 0; i < length; i++ )
    {
        *p++ = *q++;
    }
}


/* exit program */
/* Do not inline this to enable simulator to detect
 * reaching the exit label. */
void __noinline exit(int32_t status)
{
    __asm
    {
        nop
        nop
        nop
        ctx_arb[kill]
    }
}


void exit_thread(int32_t status)
{
    __asm
    {
        nop
        nop
        nop
        ctx_arb[kill]
    }
}


void ua_set_8_sram(__sram void *q, uint32_t offset, uint32_t val)
{
    SIGNAL s1;
    __xwrite int32_t sw;
    uint32_t p = (uint32_t) q + offset;
    uint32_t bs, sh, bm;

    bs = (uint32_t)p & 3;
    sh = bs << 3;

    sw = val << (24 - sh);
    bm = 8 >> bs;
    __asm
    {
        alu_shf[--, 48, OR, bm, <<16]  // set bytemask
        sram[write, sw, p, 0, 1], ctx_swap[s1], indirect_ref
    }
}


void ua_set_16_sram(__sram void *q, uint32_t offset, uint32_t val)
{
    SIGNAL s1, s2;
    __xwrite int32_t sw0, sw1;
    uint32_t p = (uint32_t) q + offset;
    uint32_t bs, sh, v, bm;

    bs = (uint32_t)p & 3;
    sh = bs << 3;
    v = val << 16;

    sw0 = v >> sh;
    sw1 = v << (32 - sh);

    bm = 12 >> bs;
    __asm
    {
        alu_shf[--, 48, OR, bm, <<16]  // set bytemask
        sram[write, sw0, p, 0, 1], sig_done[s1], indirect_ref
    }
    bm = 12 << (4 - bs);
    __asm
    {
        alu_shf[--, 48, OR, bm, <<16]  // set bytemask
        sram[write, sw1, p, 4, 1], sig_done[s2], indirect_ref
    }
    __wait_for_all(&s1, &s2);
}


void ua_set_32_sram(__sram void *q, uint32_t offset, uint32_t val)
{
    SIGNAL s1, s2;
    __xwrite int32_t sw0, sw1;
    uint32_t p = (uint32_t) q + offset;
    uint32_t bs, sh, bm;

    bs = (uint32_t)p & 3;
    sh = bs << 3;

    sw0 = val >> sh;
    sw1 = val << (32 - sh);

    bm = 15 >> bs;
    __asm
    {
        alu_shf[--, 48, OR, bm, <<16]  // set bytemask
        sram[write, sw0, p, 0, 1], sig_done[s1], indirect_ref
    }
    bm = 15 << (4 - bs);
    __asm
    {
        alu_shf[--, 48, OR, bm, <<16]  // set bytemask
        sram[write, sw1, p, 4, 1], sig_done[s2], indirect_ref
    }
    __wait_for_all(&s1, &s2);
}


/* unaligned mem pointer q:
        *(uint8_t *)((int32_t)q+offset) = (uint8_t)val;
 */
void ua_set_8_mem(__mem void *q, uint32_t offset, uint32_t val)
{
    __mem uint8_t *pp = (__mem uint8_t* )((uint32_t)q + offset);

    *pp = (uint8_t)val;
}


/* unaligned mem pointer q:
        *(uint16_t *)((int32_t)q+offset) = (uint16_t)val;
 */
void ua_set_16_mem(__mem void *q, uint32_t offset, uint32_t val)
{
    __mem uint16_t *pp = (__mem uint16_t* )((uint32_t)q + offset);

    *pp = (uint16_t)val;
}


/* unaligned mem pointer q:
        *(uint32_t *)((int32_t)q+offset) = (uint32_t)val;
 */
void ua_set_32_mem(__mem void *q, uint32_t offset, uint32_t val)
{
    __mem uint32_t *pp = (__mem uint32_t *)((int32_t)q + offset);

    *pp = val;
}

/* unaligned mem pointer q:
        *(uint64_t *)((int32_t)q+offset) = val;
 */
void ua_set_64_mem(__mem void *q, uint32_t offset, uint64_t val)
{
    __mem uint64_t *p = (__mem uint64_t *)((int32_t)q + offset);

    *p =val;
}


/* unaligned mem pointer q:
        return *(uint8_t *)((int32_t)q+offset);
 */
uint32_t ua_get_u8_mem(__mem void *q, uint32_t offset)
{
    __mem uint8_t *pp = (__mem uint8_t *)((int32_t)q + offset);

    return *pp;
}


/* unaligned mem pointer q:
        return *(uint16_t *)((int32_t)q+offset);
 */
uint32_t ua_get_u16_mem(__mem void *q, uint32_t offset)
{
    uint64_t v, v1;
    uint32_t and_result;
    __mem uint64_t *p = (__mem uint64_t *)(((int32_t)q + offset) & ~7);

    and_result = ((int32_t)q + offset) & 7;
    switch (and_result)
    {
        case 0:
            return (uint32_t)(*p >> 48);

        case 1:
            return (uint32_t)((*p >> 40) & 0xffff);

        case 2:
            return (uint32_t)((*p >> 32) & 0xffff);
        case 3:
            return (uint32_t)((*p >> 24) & 0xffff);

        case 4:
            return (uint32_t)((*p >> 16) & 0xffff);

        case 5:
            return (uint32_t)((*p >> 8) & 0xffff);

        case 6:
            return (uint32_t)((*p) & 0xffff);

        case 7:
            v = *p;
            v1 = *(p + 1);
            return (uint32_t)(((v << 8) & 0xff00) | ((v1 >> 56) & 0xff));
    }

    return 0;
}


/* unaligned mem pointer q:
        return *(uint32_t *)((int32_t)q+offset);
 */
uint32_t ua_get_u32_mem(__mem void *q, uint32_t offset)
{
    __mem uint32_t *pp = (__mem uint32_t *)((int32_t)q + offset);

    return *pp;
}



/* unaligned mem pointer q:
        return *(int8_t *)((int32_t)q+offset);
 */
int32_t ua_get_s8_mem(__mem void *q, uint32_t offset)
{
    __mem int8_t *pp = (__mem int8_t *)((int32_t)q + offset);

    return *pp;
}


/* unaligned mem pointer q:
        return *(int16_t *)((int32_t)q+offset);
 */
int32_t ua_get_s16_mem(__mem void *q, uint32_t offset)
{
    uint64_t v, v1;
    uint32_t and_result;
    __mem uint64_t *p = (__mem uint64_t *)(((int32_t)q + offset) & ~7);

    and_result = ((int32_t)q + offset) & 7;
    switch (and_result)
    {
         case 0:
            return (int16_t) (*p >> 48);

        case 1:
            return (int16_t) ((*p >> 40) & 0xffff);

        case 2:
            return (int16_t) ((*p >> 32) & 0xffff);

        case 3:
            return (int16_t) ((*p >> 24) & 0xffff);

        case 4:
            return (int16_t) ((*p >> 16) & 0xffff);

        case 5:
            return (int16_t) ((*p >> 8) & 0xffff);

        case 6:
            return (int16_t) ((*p) & 0xffff);

       case 7:
            v = *p;
            v1 = *(p + 1);
            return (int16_t) (((v << 8) & 0xff00) | ((v1 >> 56) & 0xff));
    }

    return 0;
}


/* unaligned mem pointer q:
        return *(int32_t *)((int32_t)q+offset);
 */
int32_t ua_get_s32_mem(__mem void *q, uint32_t offset)
{
    __mem int32_t *pp = (__mem int32_t *)((int32_t)q + offset);

    return *pp;
}



/* unaligned mem pointer q:
        return *(uint64_t *)((int32_t)q+offset);
 */
uint64_t ua_get_64_mem(__mem void *q, uint32_t offset)
{
    __mem uint64_t *p = (__mem uint64_t *)((int32_t)q + offset);

    return *p;
}


uint64_t ua_get_64_mem40(__addr40 __mem void *q, uint32_t offset)
{
    __addr40 __mem uint64_t *p = (__addr40 __mem uint64_t *)((__addr40 __mem uint8_t *)q + offset);

    return *p;
}


/* unaligned mem pointer q:
        return *(uint64_t *)((int32_t)q+offset);
 */
uint64_t ua_get_u64_mem(__mem void *p, uint32_t offset)
{
    return ua_get_64_mem(p, offset);
}


uint64_t ua_get_u64_mem40(__addr40 __mem void *p, uint32_t offset)
{
    return ua_get_64_mem40(p, offset);
}


/* unaligned mem pointer q:
        return *(int64_t *)((int32_t)q+offset);
 */
int64_t ua_get_s64_mem(__mem void *p, uint32_t offset)
{
    return (int64_t)ua_get_64_mem(p, offset);
}


int64_t ua_get_s64_mem40(__addr40 __mem void *p, uint32_t offset)
{
    return (int64_t)ua_get_64_mem40(p, offset);
}


void ua_set_8_mem40(__addr40 __mem void *ptr, uint32_t off, uint32_t val)
{
    __addr40 __mem uint8_t *p = (__addr40 __mem uint8_t *)ptr + off;

    *p = val;
}

void ua_set_16_mem40(__addr40 __mem void *ptr, uint32_t off, uint32_t val)
{
    __addr40 __mem uint16_t *p = (__addr40 __mem uint16_t *)((__addr40 __mem uint8_t *)ptr + off);

    *p = val;
}

void ua_set_32_mem40(__addr40 __mem void *ptr, uint32_t off, uint32_t val)
{
    __addr40 __mem uint32_t *p = (__addr40 __mem uint32_t *)((__addr40 __mem uint8_t *)ptr + off);

    *p = val;
}

void ua_set_64_mem40(__addr40 __mem void *ptr, uint32_t off, uint64_t val)
{
    __addr40 __mem uint64_t *p = (__addr40 __mem uint64_t *)((__addr40 __mem uint8_t *)ptr + off);

    *p = val;
}


/*
   unaligned memory copy: mem <- mem
 */
void ua_memcpy_mem_mem(__mem void *dst, uint32_t dst_off, __mem void *src, uint32_t src_off, uint32_t length)
{
    while (length >= 8)
    {
        __mem uint64_t *ddst = (__mem uint64_t *)((int32_t)dst + dst_off);
        __mem uint64_t *ssrc = (__mem uint64_t *)((int32_t)src + src_off);
        *ddst = *ssrc;
        length -= 8;
        dst_off += 8;
        src_off += 8;
    }

    if (length >= 4)
    {
        __mem uint32_t *ddst = (__mem uint32_t *)((int32_t)dst + dst_off);
        __mem uint32_t *ssrc = (__mem uint32_t *)((int32_t)src + src_off);
        *ddst = *ssrc;
        length -= 4;
        src_off += 4;
        dst_off += 4;
    }

    if (length >= 2)
    {
        __mem uint16_t *ddst = (__mem uint16_t *)((int32_t)dst + dst_off);
        __mem uint16_t *ssrc = (__mem uint16_t *)((int32_t)src + src_off);
        *ddst = *ssrc;
        length -= 2;
        src_off += 2;
        dst_off += 2;
    }

    if (length)
    {
        __mem uint8_t *ddst = (__mem uint8_t *)((int32_t)dst + dst_off);
        __mem uint8_t *ssrc = (__mem uint8_t *)((int32_t)src + src_off);
        *ddst = *ssrc;
    }
}

/*
   unaligned memory copy: mem <- sram
 */
void ua_memcpy_mem_sram(__mem void *dst, uint32_t dst_off, __sram void *src, uint32_t src_off, uint32_t length)
{
    while (length >= 8)
    {
        ua_set_64_mem(dst, dst_off, ua_get_u64_sram(src, src_off));
        length -= 8;
        dst_off += 8;
        src_off += 8;
    }

    if (length >= 4)
    {
        ua_set_32_mem(dst, dst_off, ua_get_u32_sram(src, src_off));
        length -= 4;
        src_off += 4;
        dst_off += 4;
    }

    if (length >= 2)
    {
        ua_set_16_mem(dst, dst_off, ua_get_u16_sram(src, src_off));
        length -= 2;
        src_off += 2;
        dst_off += 2;
    }

    if (length)
    {
        ua_set_8_mem(dst, dst_off, ua_get_u8_sram(src, src_off));
    }
}

/*
   unaligned memory copy: mem <- cls
 */
void ua_memcpy_mem_cls(__mem void *dst, uint32_t dst_off, __cls void *src, uint32_t src_off, uint32_t length)
{
    while (length >= 8)
    {
        ua_set_64_mem(dst, dst_off, ua_get_u64_cls((void *)src, src_off));
        length -= 8;
        dst_off += 8;
        src_off += 8;
    }

    if (length >= 4)
    {
        ua_set_32_mem(dst, dst_off, ua_get_u32_cls((void *)src, src_off));
        length -= 4;
        src_off += 4;
        dst_off += 4;
    }

    if (length >= 2)
    {
        ua_set_16_mem(dst, dst_off, ua_get_u16_cls((void *)src, src_off));
        length -= 2;
        src_off += 2;
        dst_off += 2;
    }

    if (length)
    {
        ua_set_8_mem(dst, dst_off, ua_get_u8_cls((void *)src, src_off));
    }
}


/*
   unaligned memory copy: sram <- mem
 */
void ua_memcpy_sram_mem(__sram void *dst, uint32_t dst_off, __mem void *src, uint32_t src_off, uint32_t length)
{
    while (length >= 8)
    {
        __mem uint64_t *ssrc = (__mem uint64_t *)((int32_t)src + src_off);
        ua_set_64(dst, dst_off, *ssrc);
        length -= 8;
        dst_off += 8;
        src_off += 8;
    }

    if (length >= 4)
    {
        __mem uint32_t *ssrc = (__mem uint32_t *)((int32_t)src + src_off);
        ua_set_32(dst, dst_off, *ssrc);
        length -= 4;
        src_off += 4;
        dst_off += 4;
    }

    if (length >= 2)
    {
        __mem uint16_t *ssrc = (__mem uint16_t *)((int32_t)src + src_off);
        ua_set_16(dst, dst_off, *ssrc);
        length -= 2;
        src_off += 2;
        dst_off += 2;
    }

    if (length)
    {
        __mem uint8_t *ssrc = (__mem uint8_t *)((int32_t)src + src_off);
        ua_set_8(dst, dst_off, *ssrc);
    }
}


/*
   unaligned memory copy: cls <- mem
 */
void ua_memcpy_cls_mem(__cls void *dst, uint32_t dst_off, __mem void *src, uint32_t src_off, uint32_t length)
{
    while (length >= 8)
    {
        __mem uint64_t *ssrc = (__mem uint64_t *)((int32_t)src + src_off);
        ua_set_64_cls(dst, dst_off, *ssrc);
        length -= 8;
        dst_off += 8;
        src_off += 8;
    }

    if (length >= 4)
    {
        __mem uint32_t *ssrc = (__mem uint32_t *)((int32_t)src + src_off);
        ua_set_32_cls(dst, dst_off, *ssrc);
        length -= 4;
        src_off += 4;
        dst_off += 4;
    }

    if (length >= 2)
    {
        __mem uint16_t *ssrc = (__mem uint16_t *)((int32_t)src + src_off);
        ua_set_16_cls(dst, dst_off, *ssrc);
        length -= 2;
        src_off += 2;
        dst_off += 2;
    }

    if (length)
    {
        __mem uint8_t *ssrc = (__mem uint8_t *)((int32_t)src + src_off);
        ua_set_8_cls(dst, dst_off, *ssrc);
    }
}


/*
   unaligned memory copy: sram <- cls
 */
void ua_memcpy_sram_cls(__sram void *dst, uint32_t dst_off, __cls void *src, uint32_t src_off, uint32_t length)
{
    while (length >= 8)
    {
        ua_set_64_sram(dst, dst_off, ua_get_u64_cls((__cls void *)src, src_off));
        length -= 8;
        dst_off += 8;
        src_off += 8;
    }

    if (length >= 4)
    {
        ua_set_32_sram(dst, dst_off, ua_get_u32_cls((__cls void *)src, src_off));
        length -= 4;
        src_off += 4;
        dst_off += 4;
    }

    if (length >= 2)
    {
        ua_set_16_sram(dst, dst_off, ua_get_u16_cls((__cls void *)src, src_off));
        length -= 2;
        src_off += 2;
        dst_off += 2;
    }

    if (length)
    {
        ua_set_8_sram(dst, dst_off, ua_get_u8_cls((__cls void *)src, src_off));
    }
}

/*
   unaligned memory copy: cls <- sram
 */
void ua_memcpy_cls_sram(__cls void *dst, uint32_t dst_off, __sram void *src, uint32_t src_off, uint32_t length)
{
    while (length >= 8)
    {
        ua_set_64_cls((__cls void *)dst, dst_off, ua_get_u64_sram(src, src_off));
        length -= 8;
        dst_off += 8;
        src_off += 8;
    }

    if (length >= 4)
    {
        ua_set_32_cls((__cls void *)dst, dst_off, ua_get_u32_sram(src, src_off));
        length -= 4;
        src_off += 4;
        dst_off += 4;
    }

    if (length >= 2)
    {
        ua_set_16_cls((__cls void *)dst, dst_off, ua_get_u16_sram(src, src_off));
        length -= 2;
        src_off += 2;
        dst_off += 2;
    }

    if (length)
    {
        ua_set_8_cls((__cls void *)dst, dst_off, ua_get_u8_sram(src, src_off));
    }
}

/*
   unaligned memory copy: cls <- cls
 */
void ua_memcpy_cls_cls(
    __cls void *dst, uint32_t dst_off,
    __cls void *src, uint32_t src_off,
    uint32_t length)
{
    while (length >= 8)
    {
        ua_set_64_cls((__cls void *)dst, dst_off, ua_get_u64_cls((__cls void *)src, src_off));
        length -= 8;
        dst_off += 8;
        src_off += 8;
    }

    if (length >= 4)
    {
        ua_set_32_cls((__cls void *)dst, dst_off, ua_get_u32_cls((__cls void *)src, src_off));
        length -= 4;
        src_off += 4;
        dst_off += 4;
    }

    if (length >= 2)
    {
        ua_set_16_cls((__cls void *)dst, dst_off, ua_get_u16_cls((__cls void *)src, src_off));
        length -= 2;
        src_off += 2;
        dst_off += 2;
    }

    if (length)
    {
        ua_set_8_cls((__cls void *)dst, dst_off, ua_get_u8_cls((__cls void *)src, src_off));
    }
}


/*
   unaligned memory copy: lmem <- mem
 */
void ua_memcpy_lmem_mem(__lmem void *dst, uint32_t dst_off, __mem void *src, uint32_t src_off, uint32_t length)
{
    __lmem int8_t *p;
    __mem int8_t *q;
    uint32_t i;

    p = ((__lmem int8_t *) dst) + dst_off;
    q = ((__mem int8_t *) src) + src_off;
    for ( i = 0; i < length; i++ )
    {
        *p++ = *q++;
    }
}


/*
   unaligned memory copy: mem <- lmem
 */
void ua_memcpy_mem_lmem(__mem void *dst, uint32_t dst_off, __lmem void *src, uint32_t src_off, uint32_t length)
{
    __mem int8_t *p;
    __lmem int8_t *q;
    uint32_t i;

    p = ((__mem int8_t *) dst) + dst_off;
    q = ((__lmem int8_t *) src) + src_off;
    for ( i = 0; i < length; i++ )
    {
        *p++ = *q++;
    }
}


void ua_memcpy_lmem_mem40(
    __lmem void *dst, uint32_t dst_off,
    __addr40 __mem void *src, uint32_t src_off,
    uint32_t length)
{
    __lmem uint8_t *p;
    __addr40 __mem uint8_t *q;
    uint32_t i;

    p = (__lmem uint8_t *)dst + dst_off;
    q = (__addr40 __mem uint8_t *)src + src_off;
    for (i = 0; i < length; i++)
    {
        *p++ = *q++;
    }
}


void ua_memcpy_lmem_cls40(
    __lmem void *dst, uint32_t dst_off,
    __cls __addr40 void *src, uint32_t src_off,
    uint32_t length)
{
    __lmem uint8_t *p;
    __cls __addr40 uint8_t *q;
    uint32_t i;

    p = (__lmem uint8_t *)dst + dst_off;
    q = (__cls __addr40 uint8_t *)src + src_off;
    for (i = 0; i < length; i++)
    {
        *p++ = *q++;
    }
}


void ua_memcpy_sram_mem40(
    __sram void *dst, uint32_t dst_off,
    __addr40 __mem void *src, uint32_t src_off,
    uint32_t length)
{
    while (length >= 8)
    {
        __addr40 __mem uint64_t *ssrc = (__addr40 __mem uint64_t *)((__addr40 __mem uint8_t *)src + src_off);
        ua_set_64(dst, dst_off, *ssrc);
        length -= 8;
        dst_off += 8;
        src_off += 8;
    }

    if (length >= 4)
    {
        __addr40 __mem uint32_t *ssrc = (__addr40 __mem uint32_t *)((__addr40 __mem uint8_t *)src + src_off);
        ua_set_32(dst, dst_off, *ssrc);
        length -= 4;
        src_off += 4;
        dst_off += 4;
    }

    if (length >= 2)
    {
        __addr40 __mem uint16_t *ssrc = (__addr40 __mem uint16_t *)((__addr40 __mem uint8_t *)src + src_off);
        ua_set_16(dst, dst_off, *ssrc);
        length -= 2;
        src_off += 2;
        dst_off += 2;
    }

    if (length)
    {
        __addr40 __mem uint8_t *ssrc = (__addr40 __mem uint8_t *)src + src_off;
        ua_set_8(dst, dst_off, *ssrc);
    }
}


void ua_memcpy_sram_cls40(
    __sram void *dst, uint32_t dst_off,
    __cls __addr40 void *src, uint32_t src_off,
    uint32_t length)
{
    while (length >= 8)
    {
        ua_set_64_sram(dst, dst_off, ua_get_u64_cls40((__cls __addr40 void *)src, src_off));
        length -= 8;
        dst_off += 8;
        src_off += 8;
    }

    if (length >= 4)
    {
        ua_set_32_sram(dst, dst_off, ua_get_u32_cls40((__cls __addr40 void *)src, src_off));
        length -= 4;
        src_off += 4;
        dst_off += 4;
    }

    if (length >= 2)
    {
        ua_set_16_sram(dst, dst_off, ua_get_u16_cls40((__cls __addr40 void *)src, src_off));
        length -= 2;
        src_off += 2;
        dst_off += 2;
    }

    if (length)
    {
        ua_set_8_sram(dst, dst_off, ua_get_u8_cls40((__cls __addr40 void *)src, src_off));
    }
}


void ua_memcpy_mem_mem40(
    __mem void *dst, uint32_t dst_off,
    __addr40 __mem void *src, uint32_t src_off,
    uint32_t length)
{
    __mem uint8_t *p;
    __addr40 __mem uint8_t *q;
    uint32_t i;

    p = (__mem uint8_t *)dst + dst_off;
    q = (__addr40 __mem uint8_t *)src + src_off;
    for (i = 0; i < length; i++)
    {
        *p++ = *q++;
    }
}


void ua_memcpy_mem_cls40(
    __mem void *dst, uint32_t dst_off,
    __cls __addr40 void *src, uint32_t src_off,
    uint32_t length)
{
    __mem uint8_t *p;
    __cls __addr40 uint8_t *q;
    uint32_t i;

    p = (__mem uint8_t *)dst + dst_off;
    q = (__cls __addr40 uint8_t *)src + src_off;
    for (i = 0; i < length; i++)
    {
        *p++ = *q++;
    }
}


void ua_memcpy_mem40_lmem(
    __addr40 __mem void *dst, uint32_t dst_off,
    __lmem void *src, uint32_t src_off,
    uint32_t length)
{
    __addr40 __mem uint8_t *p;
    __lmem uint8_t *q;
    uint32_t i;

    p = (__addr40 __mem uint8_t *)dst + dst_off;
    q = (__lmem uint8_t *)src + src_off;
    for (i = 0; i < length; i++)
    {
        *p++ = *q++;
    }
}


void ua_memcpy_mem40_sram(
    __addr40 __mem void *dst, uint32_t dst_off,
    __sram void *src, uint32_t src_off,
    uint32_t length)
{
    __addr40 __mem uint8_t *p;
    __sram uint8_t *q;
    uint32_t i;

    p = (__addr40 __mem uint8_t *)dst + dst_off;
    q = (__sram uint8_t *)src + src_off;
    for (i = 0; i < length; i++)
    {
        *p++ = *q++;
    }
}


void ua_memcpy_mem40_mem(
    __addr40 __mem void *dst, uint32_t dst_off,
    __mem void *src, uint32_t src_off,
    uint32_t length)
{
    __addr40 __mem uint8_t *p;
    __mem uint8_t *q;
    uint32_t i;

    p = (__addr40 __mem uint8_t *)dst + dst_off;
    q = (__mem uint8_t *)src + src_off;
    for (i = 0; i < length; i++)
    {
        *p++ = *q++;
    }
}


void ua_memcpy_mem40_mem40(
    __addr40 __mem void *dst, uint32_t dst_off,
    __addr40 __mem void *src, uint32_t src_off,
    uint32_t length)
{
    __addr40 __mem uint8_t *p;
    __addr40 __mem uint8_t *q;
    uint32_t i;

    p = (__addr40 __mem uint8_t *)dst + dst_off;
    q = (__addr40 __mem uint8_t *)src + src_off;
    for (i = 0; i < length; i++)
    {
        *p++ = *q++;
    }
}


void ua_memcpy_mem40_cls(
    __addr40 __mem void *dst, uint32_t dst_off,
    __cls void *src, uint32_t src_off,
    uint32_t length)
{
    __addr40 __mem uint8_t *p;
    __cls uint8_t *q;
    uint32_t i;

    p = (__addr40 __mem uint8_t *)dst + dst_off;
    q = (__cls uint8_t *)src + src_off;
    for (i = 0; i < length; i++)
    {
        *p++ = *q++;
    }
}


void ua_memcpy_mem40_cls40(
    __addr40 __mem void *dst, uint32_t dst_off,
    __cls __addr40 void *src, uint32_t src_off,
    uint32_t length)
{
    __addr40 __mem uint8_t *p;
    __cls __addr40 uint8_t *q;
    uint32_t i;

    p = (__addr40 __mem uint8_t *)dst + dst_off;
    q = (__cls __addr40 uint8_t *)src + src_off;
    for (i = 0; i < length; i++)
    {
        *p++ = *q++;
    }
}


void ua_memcpy_cls_mem40(
    __cls void *dst, uint32_t dst_off,
    __addr40 __mem void *src, uint32_t src_off,
    uint32_t length)
{
    __cls uint8_t *p;
    __addr40 __mem uint8_t *q;
    uint32_t i;

    p = (__cls uint8_t *)dst + dst_off;
    q = (__addr40 __mem uint8_t *)src + src_off;
    for (i = 0; i < length; i++)
    {
        *p++ = *q++;
    }
}


void ua_memcpy_cls_cls40(
    __cls void *dst, uint32_t dst_off,
    __cls __addr40 void *src, uint32_t src_off,
    uint32_t length)
{
    __cls uint8_t *p;
    __cls __addr40 uint8_t *q;
    uint32_t i;

    p = (__cls uint8_t *)dst + dst_off;
    q = (__cls __addr40 uint8_t *)src + src_off;
    for (i = 0; i < length; i++)
    {
        *p++ = *q++;
    }
}


void ua_memcpy_cls40_lmem(
    __cls __addr40 void *dst, uint32_t dst_off,
    __lmem void *src, uint32_t src_off,
    uint32_t length)
{
    __cls __addr40 uint8_t *p;
    __lmem uint8_t *q;
    uint32_t i;

    p = (__cls __addr40 uint8_t *)dst + dst_off;
    q = (__lmem uint8_t *)src + src_off;
    for (i = 0; i < length; i++)
    {
        *p++ = *q++;
    }
}


void ua_memcpy_cls40_sram(
    __cls __addr40 void *dst, uint32_t dst_off,
    __sram void *src, uint32_t src_off,
    uint32_t length)
{
    __cls __addr40 uint8_t *p;
    __sram uint8_t *q;
    uint32_t i;

    p = (__cls __addr40 uint8_t *)dst + dst_off;
    q = (__sram uint8_t *)src + src_off;
    for (i = 0; i < length; i++)
    {
        *p++ = *q++;
    }
}


void ua_memcpy_cls40_mem(
    __cls __addr40 void *dst, uint32_t dst_off,
    __mem void *src, uint32_t src_off,
    uint32_t length)
{
    __cls __addr40 uint8_t *p;
    __mem uint8_t *q;
    uint32_t i;

    p = (__cls __addr40 uint8_t *)dst + dst_off;
    q = (__mem uint8_t *)src + src_off;
    for (i = 0; i < length; i++)
    {
        *p++ = *q++;
    }
}


void ua_memcpy_cls40_mem40(
    __cls __addr40 void *dst, uint32_t dst_off,
    __addr40 __mem void *src, uint32_t src_off,
    uint32_t length)
{
    __cls __addr40 uint8_t *p;
    __addr40 __mem uint8_t *q;
    uint32_t i;

    p = (__cls __addr40 uint8_t *)dst + dst_off;
    q = (__addr40 __mem uint8_t *)src + src_off;
    for (i = 0; i < length; i++)
    {
        *p++ = *q++;
    }
}


void ua_memcpy_cls40_cls(
    __cls __addr40 void *dst, uint32_t dst_off,
    __cls void *src, uint32_t src_off,
    uint32_t length)
{
    __cls __addr40 uint8_t *p;
    __cls uint8_t *q;
    uint32_t i;

    p = (__cls __addr40 uint8_t *)dst + dst_off;
    q = (__cls uint8_t *)src + src_off;
    for (i = 0; i < length; i++)
    {
        *p++ = *q++;
    }
}


void ua_memcpy_cls40_cls40(
    __cls __addr40 void *dst, uint32_t dst_off,
    __cls __addr40 void *src, uint32_t src_off,
    uint32_t length)
{
    __cls __addr40 uint8_t *p;
    __cls __addr40 uint8_t *q;
    uint32_t i;

    p = (__cls __addr40 uint8_t *)dst + dst_off;
    q = (__cls __addr40 uint8_t *)src + src_off;
    for (i = 0; i < length; i++)
    {
        *p++ = *q++;
    }
}


uint32_t ua_get_u32_cls40(__cls __addr40 void *q, uint32_t offset)
{
    __cls __addr40 uint32_t *p = (__cls __addr40 uint32_t *)((__cls __addr40 uint8_t *)q + offset);

    return *p;
}

uint32_t ua_get_u16_cls40(__cls __addr40 void *q, uint32_t offset)
{
    __cls __addr40 uint16_t *p = (__cls __addr40 uint16_t *)((__cls __addr40 uint8_t *)q + offset);

    return *p;
}

uint32_t ua_get_u8_cls40(__cls __addr40 void *q, uint32_t offset)
{
    __cls __addr40 uint8_t *p = (__cls __addr40 uint8_t *)q + offset;

    return *p;
}

int32_t ua_get_s32_cls40(__cls __addr40 void *q, uint32_t offset)
{
    __cls __addr40 int32_t *p = (__cls __addr40 int32_t *)((__cls __addr40 uint8_t *)q + offset);

    return *p;
}

int32_t ua_get_s16_cls40(__cls __addr40 void *q, uint32_t offset)
{
    __cls __addr40 int16_t *p = (__cls __addr40 int16_t *)((__cls __addr40 uint8_t *)q + offset);

    return *p;
}

int32_t ua_get_s8_cls40(__cls __addr40 void *q, uint32_t offset)
{
    __cls __addr40 int8_t *p = (__cls __addr40 int8_t *)q + offset;

    return *p;
}

void ua_set_64_cls40(__cls __addr40 void *q, uint32_t offset, uint64_t val)
{
    __cls __addr40 uint64_t *p = (__cls __addr40 uint64_t *)((__cls __addr40 uint8_t *)q + offset);

    *p = val;
}

void ua_set_32_cls40(__cls __addr40 void *q, uint32_t offset, uint32_t val)
{
    __cls __addr40 uint32_t *p = (__cls __addr40 uint32_t *)((__cls __addr40 uint8_t *)q + offset);

    *p = val;
}

void ua_set_16_cls40(__cls __addr40 void *q, uint32_t offset, uint32_t val)
{
    __cls __addr40 uint16_t *p = (__cls __addr40 uint16_t *)((__cls __addr40 uint8_t *)q + offset);

    *p = val;
}

void ua_set_8_cls40(__cls __addr40 void *q, uint32_t offset, uint32_t val)
{
    __cls __addr40 uint8_t *p = (__cls __addr40 uint8_t *)((__cls __addr40 uint8_t *)q + offset);

    *p = val;
}

uint32_t ua_get_u32_mem40(__addr40 __mem void *q, uint32_t offset)
{
    __addr40 __mem uint32_t *p = (__addr40 __mem uint32_t *)((__addr40 __mem uint8_t *)q + offset);

    return *p;
}

uint32_t ua_get_u16_mem40(__addr40 __mem void *q, uint32_t offset)
{
    __addr40 __mem uint16_t *p = (__addr40 __mem uint16_t *)((__addr40 __mem uint8_t *)q + offset);

    return *p;
}

uint32_t ua_get_u8_mem40(__addr40 __mem void *q, uint32_t offset)
{
    __addr40 __mem uint8_t *p = (__addr40 __mem uint8_t *)q + offset;

    return *p;
}

int32_t ua_get_s32_mem40(__addr40 __mem void *q, uint32_t offset)
{
    __addr40 __mem int32_t *p = (__addr40 __mem int32_t *)((__addr40 __mem uint8_t *)q + offset);

    return *p;
}

int32_t ua_get_s16_mem40(__addr40 __mem void *q, uint32_t offset)
{
    __addr40 __mem int16_t *p = (__addr40 __mem int16_t *)((__addr40 __mem uint8_t *)q + offset);

    return *p;
}

int32_t ua_get_s8_mem40(__addr40 __mem void *q, uint32_t offset)
{
    __addr40 __mem int8_t *p = (__addr40 __mem int8_t *)q + offset;

    return *p;
}


/*
 * The following were originally part of intrinsic.c.
 */
__intrinsic
uint32_t local_csr_read(local_csr_t lcsr)
{
    uint32_t result;
    {
        __ct_assert(__is_ct_const(lcsr), "__is_ct_const(lcsr)");

        __asm
        {
            local_csr_rd[__ct_const_val(lcsr)]
            immed[result, 0]           /* immed immediately follows, don't even use separate __asm block */
        }
    }
    return result;
}


__intrinsic
void local_csr_write(local_csr_t lcsr, uint32_t data)
{
    {
        __ct_assert(__is_ct_const(lcsr), "__is_ct_const(lcsr)");

        __asm local_csr_wr[__ct_const_val(lcsr), data]
    }
}

__intrinsic
uint32_t dbl_shr(
    uint32_t srcA,          /* high 32 bits */
    uint32_t srcB,          /* low 32 bits */
    uint32_t shift_cnt      /* shift count */
)
{
    uint32_t result;
    {
        if (__is_ct_const(shift_cnt))
        {
            __ct_assert(shift_cnt >= 1 && shift_cnt <= 31, "shift_cnt >= 1 && shift_cnt <= 31");
            __asm dbl_shf[result, srcA, srcB, >>__ct_const_val(shift_cnt)]
        }
        else
        {
            __asm
            {
                alu[--, shift_cnt, OR, 0]
                dbl_shf[result, srcA, srcB, >>indirect]
            }
        }
    }
    return result;
}


__intrinsic
uint32_t dbl_shl(
    uint32_t srcA,          /* high 32 bits */
    uint32_t srcB,          /* low 32 bits */
    uint32_t shift_cnt      /* shift count */
)
{
    uint32_t result;
    {
        result = dbl_shr(srcA, srcB, 32 - shift_cnt);
    }
    return result;
}


#endif /* __RTL_C__ */
