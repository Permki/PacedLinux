/*
 * Copyright (C) 2008-2014 Netronome Systems, Inc.  All rights reserved.
 */

#ifndef __LIBC_C__
#define __LIBC_C__

#include <nfp.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#ifndef NFP_LIB_ANY_NFCC_VERSION
    #if (!defined(__NFP_TOOL_NFCC) ||                       \
        (__NFP_TOOL_NFCC < NFP_SW_VERSION(5, 0, 0, 0)) ||   \
        (__NFP_TOOL_NFCC > NFP_SW_VERSION(6, 1, 0, 255)))
        #error "This standard library is not supported for the version of the SDK currently in use."
    #endif
#endif


/*
 *  Return absolute value.
 */
int32_t abs(int32_t n)
{
    return (n>=0)? n : -n;
}


/*
 *  Return absolute value.
 */
int32_t labs(int32_t n)
{
    return abs(n);
}


/*
 *  return a non-zero value if c is a lower case character
 */
int32_t islower(int32_t c)
{
    return ((uint32_t)(c - 'a') < 26) ? 1 : 0;
}


/*
 *  return lower case character corresponding to c
 */
int32_t tolower(int32_t c)
{
    if ((uint32_t)(c-'A')<26)
    {
        c += 'a' - 'A';
    }

    return c;
}


/*
 *  return a non-zero value if c is an upper case character
 */
int32_t isupper(int32_t c)
{
    return ((uint32_t)(c - 'A') < 26) ? 1 : 0;
}


/*
 *  return upper case character corresponding to c
 */
int32_t toupper(int32_t c)
{
    if ((uint32_t)(c-'a')<26)
    {
        c += 'A' - 'a';
    }

    return c;
}

uint32_t strlen_lmem(__lmem uint8_t *s)
{
    __lmem uint8_t *p = s;
    while(*p)
    {
        p++;
    }
    return p - s;
}


int32_t strcmp_cls(__cls int8_t *s1, __cls int8_t *s2)
{
    while(*s1==*s2 && *s1)
    {
        s1++;
        s2++;
    }
    return (uint8_t)*s1 - (uint8_t)*s2;
}


int32_t strcmp_lmem(__lmem uint8_t *s1, __lmem uint8_t *s2)
{
    while(*s1==*s2 && *s1)
    {
        s1++;
        s2++;
    }
    return (uint8_t)*s1 - (uint8_t)*s2;
}


int32_t strncmp_cls(__cls int8_t *s1, __cls int8_t *s2, uint32_t count)
{
    if (count==0)
    {
        return 0;
    }

    while(--count && *s1==*s2 && *s1)
    {
        s1++;
        s2++;
    }

    return (uint8_t)*s1 - (uint8_t)*s2;
}


int32_t strncmp_lmem(__lmem uint8_t *s1, __lmem uint8_t *s2, uint32_t count)
{
    if (count==0)
    {
        return 0;
    }

    while(--count && *s1==*s2 && *s1)
    {
        s1++;
        s2++;
    }

    return (uint8_t)*s1 - (uint8_t)*s2;
}

__cls int8_t *strcpy_cls(__cls int8_t *dest, __cls int8_t *src)
{
    __cls int8_t *ret = dest;
    while((*dest++ = *src++)!=0)
        ;
    return ret;
}

__lmem uint8_t *strcpy_lmem(__lmem uint8_t *dest, __lmem uint8_t *src)
{
    __lmem uint8_t *ret = dest;
    while((*dest++ = *src++)!=0)
        ;
    return ret;
}


__cls int8_t *strncpy_cls(__cls int8_t *dest, __cls int8_t *src, uint32_t count)
{
    __cls int8_t *ret = dest;

    for(; count && (*dest++ = *src++)!=0; count--)
        ;

    while(count-- > 0)      // 0x00-padding
        *dest++ = 0x00;
    return ret;
}


__lmem uint8_t *strncpy_lmem(__lmem uint8_t *dest, __lmem uint8_t *src, uint32_t count)
{
    __lmem uint8_t *ret = dest;

    for(; count && (*dest++ = *src++)!=0; count--)
        ;

    while(count-- > 0)      // 0x00-padding
        *dest++ = 0x00;
    return ret;
}


__cls int8_t *strcat_cls(__cls int8_t *dest, __cls int8_t *src)
{
    __cls int8_t *ret;

    for(ret= dest; *dest; dest++)
        ;

    while((*dest++ = *src++)!=0)
        ;

    return ret;
}


__lmem uint8_t *strcat_lmem(__lmem uint8_t *dest, __lmem uint8_t *src)
{
    __lmem uint8_t *ret;

    for(ret= dest; *dest; dest++)
    ;

    while((*dest++ = *src++)!=0)
        ;

    return ret;
}


__cls int8_t *strncat_cls(__cls int8_t *dest, __cls int8_t *src, uint32_t count)
{
    __cls int8_t *ret;

    for(ret= dest; *dest; dest++)
        ;

    // the following differs from strncpy in that dest is ALWAYS 0x00-terminted
    while(count--)
    {
        if ((*dest++ = *src++)==0)
        {
            *dest = 0x00;
            return ret;
        }
    }
    if(*dest != 0)
        *dest = 0x00;
    return ret;
}


__lmem uint8_t *strncat_lmem(__lmem uint8_t *dest, __lmem uint8_t *src, uint32_t count)
{
    __lmem uint8_t *ret;

    for(ret= dest; *dest; dest++)
        ;

    // the following differs from strncpy in that dest is ALWAYS 0x00-terminted
    while(count--)
    {
        if ((*dest++ = *src++)==0)
        {
            *dest = 0x00;
            return ret;
        }
    }
    if(*dest != 0)
        *dest = 0x00;
    return ret;
}

__cls int8_t *strchr_cls(__cls int8_t *s, int8_t c)
{
    while(*s && *s!=c)
    {
        s++;
    }
    return (*s==c)? (__cls int8_t *)s : NULL;
}

__lmem uint8_t *strchr_lmem(__lmem uint8_t *s, int8_t c)
{
    while(*s && *s!=c)
    {
        s++;
    }
    return (*s==c)? (__lmem uint8_t *)s : NULL;
}


__cls int8_t *strrchr_cls(__cls int8_t *s, int8_t c)     // c can be 0x00
{
    __cls int8_t *head = s;
    while(*s++)
        ;
    while(--s!=head && *s!=c)
        ;
    return (*s==c)? (__cls int8_t *)s : NULL;
}

__lmem uint8_t *strrchr_lmem(__lmem uint8_t *s, int8_t c)     // c can be 0x00
{
    __lmem uint8_t *head = s;
    while(*s++)
        ;
    while(--s!=head && *s!=c)
        ;
    return (*s==c)? (__lmem uint8_t *)s : NULL;
}

static uint32_t _strto_l_ul_cls(__cls int8_t *s, __cls int8_t **ps_end, int32_t base, int32_t is_signed)
{
    int32_t neg, overflow;
    uint32_t ret, max_quo, max_rem, n_digit;

    __cls int8_t *s0 = s;

    // skip while spaces 0x09-0x0D,0x20
    while (*s==0x20 || (uint32_t)(*s-0x09)<5)
    {
        s++;
    }
    // handle '-' and '+'
    if (*s=='-')
    {
        neg = 1;
        s++;
    } else {
        neg = 0;
        if (*s=='+')
            s++;
    }
    // determine valid base (between 2..36)
    if (base==0)
    {
        if (*s=='0')
        {
            base = (s[1]=='x' || s[1]=='X')? 16 : 8;
        } else
        {
            base = 10;
        }
    } else if ((uint32_t)(base-2)>34)
    {
        if (ps_end)
            *ps_end = s0;
        return 0;
    }

    if (base==16 && *s=='0' && (s[1]=='x' || s[1]=='X'))
        s+=2;   // slip prefix "0x"

    max_quo = ULONG_MAX / (uint32_t)base;
    max_rem = ULONG_MAX % (uint32_t)base;

    // parse
    for(ret=0,n_digit=0,overflow=0; ; n_digit++,s++) {
        uint32_t digit;
        if ((uint32_t)(*s-'0')<10)
        {
            digit = *s - '0';
        } else if ((uint32_t)(*s-'A')<26)
        {
            digit = *s - 'A' + 10;
        } else if ((uint32_t)(*s-'a')<26)
        {
            digit = *s - 'a' + 10;
        } else
        {
            break;
        }
        if (digit >= (uint32_t)base)
            break;

        if (!overflow)
        {
            if (ret >= max_quo)
            {
                if (!(ret==max_quo && digit <=max_rem))
                {
                    overflow = 1;
                    continue;
                }
            }

            ret = ret * (uint32_t)base + digit;
            if (is_signed &&
                ret > (1UL<<31)-1+neg)    // (neg && ret > -LONG_MIN) || (!neg && ret > LONG_MAX)))
            {
                overflow = 1;
            }
        }
    }
    // check n_digit
    if (n_digit==0)
    {
        if (ps_end)
            *ps_end = s0;
        return 0;
    }
    // check overflow
    if (overflow)
    {
        if (is_signed)
        {
            ret = (neg)? (uint32_t)(-LONG_MIN) : LONG_MAX;
        } else
        {
            ret = ULONG_MAX;
        }
    }
    if (neg)
    // check neg
    {
        ret = -(int32_t)ret;   // according to spec, do it even when ret==ULONG_MAX!!
    }

    if (ps_end)
        *ps_end = (__cls int8_t *)s;

    return ret;
}


static uint32_t _strto_l_ul_lmem(__lmem uint8_t *s, __lmem uint8_t **ps_end, int32_t base, int32_t is_signed)
{
    int32_t neg, overflow;
    uint32_t ret, max_quo, max_rem, n_digit;

    __lmem uint8_t *s0 = s;

    // skip while spaces 0x09-0x0D,0x20
    while (*s==0x20 || (uint32_t)(*s-0x09)<5)
    {
        s++;
    }
    // handle '-' and '+'
    if (*s=='-')
    {
        neg = 1;
        s++;
    } else {
        neg = 0;
        if (*s=='+')
            s++;
    }
    // determine valid base (between 2..36)
    if (base==0)
    {
        if (*s=='0')
        {
            base = (s[1]=='x' || s[1]=='X')? 16 : 8;
        } else
        {
            base = 10;
        }
    } else if ((uint32_t)(base-2)>34)
    {
        if (ps_end)
            *ps_end = s0;
        return 0;
    }

    if (base==16 && *s=='0' && (s[1]=='x' || s[1]=='X'))
        s+=2;   // slip prefix "0x"

    max_quo = ULONG_MAX / (uint32_t)base;
    max_rem = ULONG_MAX % (uint32_t)base;

    // parse
    for(ret=0,n_digit=0,overflow=0; ; n_digit++,s++) {
        uint32_t digit;
        if ((uint32_t)(*s-'0')<10)
        {
            digit = *s - '0';
        } else if ((uint32_t)(*s-'A')<26)
        {
            digit = *s - 'A' + 10;
        } else if ((uint32_t)(*s-'a')<26)
        {
            digit = *s - 'a' + 10;
        } else
        {
            break;
        }
        if (digit >= (uint32_t)base)
            break;

        if (!overflow)
        {
            if (ret >= max_quo)
            {
                if (!(ret==max_quo && digit <=max_rem))
                {
                    overflow = 1;
                    continue;
                }
            }

            ret = ret * (uint32_t)base + digit;
            if (is_signed &&
                ret > (1UL<<31)-1+neg)    // (neg && ret > -LONG_MIN) || (!neg && ret > LONG_MAX)))
            {
                overflow = 1;
            }
        }
    }
    // check n_digit
    if (n_digit==0)
    {
        if (ps_end)
            *ps_end = s0;
        return 0;
    }
    // check overflow
    if (overflow)
    {
        if (is_signed)
        {
            ret = (neg)? (uint32_t)(-LONG_MIN) : LONG_MAX;
        } else
        {
            ret = ULONG_MAX;
        }
    }
    if (neg)
    // check neg
    {
        ret = -(int32_t)ret;   // according to spec, do it even when ret==ULONG_MAX!!
    }

    if (ps_end)
        *ps_end = (__lmem uint8_t *)s;

    return ret;
}


int32_t strtol_cls(__cls int8_t *s, __cls int8_t **ps_end, int32_t base)
{
    return (int32_t)_strto_l_ul_cls(s,ps_end,base,1/*is_signed*/);
}

int32_t strtol_lmem(__lmem uint8_t *s, __lmem uint8_t **ps_end, int32_t base)
{
    return (int32_t)_strto_l_ul_lmem(s,ps_end,base,1/*is_signed*/);
}


uint32_t strtoul_cls(__cls int8_t *s, __cls int8_t **ps_end, int32_t base)
{
    return _strto_l_ul_cls(s,ps_end,base,0/*!is_unsigned*/);
}

uint32_t strtoul_lmem(__lmem uint8_t *s, __lmem uint8_t **ps_end, int32_t base)
{
    return _strto_l_ul_lmem(s,ps_end,base,0/*!is_signed*/);
}


#define ALIGNED_U64(x) ((x&0x7)==0)
#define ALIGNED_U32(x) ((x&0x3)==0)
#define ALIGNED_U16(x) ((x&0x1)==0)


__cls void *memcpy_cls_cls(__cls void *dest, __cls void *src, uint32_t n)
{
    uint32_t  p= (uint32_t)dest;
    uint32_t  q= (uint32_t)src;

    if (ALIGNED_U64(p) && ALIGNED_U64(q))
    {
        while (n >= 8)
        {
            *(__cls uint64_t *) p = *(__cls uint64_t *) q;
            p += 8;
            q += 8;
            n -= 8;
        }
    }

    if (ALIGNED_U32(p) && ALIGNED_U32(q))
    {
        while (n >= 4)
        {
            *(__cls uint32_t *) p= *(__cls uint32_t *) q;
            p += 4;
            q += 4;
            n -= 4;
        }
    }

    while (n > 0)
    {
        *(__cls int8_t *) p= *(__cls int8_t *) q;
        p += 1;
        q += 1;
        n -= 1;
    }

    return dest;
}


__cls void *memcpy_cls_lmem(__cls void *dest, __lmem void *src, uint32_t n)
{
    uint32_t  p= (uint32_t)dest;
    uint32_t  q= (uint32_t)src;

    if (ALIGNED_U64(p) && ALIGNED_U64(q))
    {
        while (n >= 8)
        {
            *(__cls uint64_t *) p = *(__lmem uint64_t *) q;
            p += 8;
            q += 8;
            n -= 8;
        }
    }

    if (ALIGNED_U32(p) && ALIGNED_U32(q))
    {
        while (n >= 4)
        {
            *(__cls uint32_t *) p= *(__lmem uint32_t *) q;
            p += 4;
            q += 4;
            n -= 4;
        }

    }

    while (n > 0)
    {
        *(__cls int8_t *) p= *(__lmem int8_t *) q;
        p += 1;
        q += 1;
        n -= 1;
    }

    return dest;
}


__lmem void *memcpy_lmem_cls(__lmem void *dest, __cls void *src, uint32_t n)
{
    uint32_t  p= (uint32_t)dest;
    uint32_t  q= (uint32_t)src;

    if (ALIGNED_U64(p) && ALIGNED_U64(q))
    {
        while (n >= 8)
        {
            *(__lmem uint64_t *) p = *(__cls uint64_t *) q;
            p += 8;
            q += 8;
            n -= 8;
        }
    }

    if (ALIGNED_U32(p) && ALIGNED_U32(q))
    {
        while (n >= 4)
        {
            *(__lmem uint32_t *) p= *(__cls uint32_t *) q;
            p += 4;
            q += 4;
            n -= 4;
        }
    }

    while (n > 0)
    {
        *(__lmem int8_t *) p= *(__cls int8_t *) q;
        p += 1;
        q += 1;
        n -= 1;
    }

    return dest;
}

__lmem void *memcpy_lmem_lmem(__lmem void *dest, __lmem void *src, uint32_t n)
{
    uint32_t  p= (uint32_t)dest;
    uint32_t  q= (uint32_t)src;

    if (ALIGNED_U64(p) && ALIGNED_U64(q))
    {
        while (n >= 8)
        {
            *(__lmem uint64_t *) p = *(__lmem uint64_t *) q;
            p += 8;
            q += 8;
            n -= 8;
        }
    }

    if (ALIGNED_U32(p) && ALIGNED_U32(q))
    {
        while (n >= 4)
        {
            *(__lmem uint32_t *) p= *(__lmem uint32_t *) q;
            p += 4;
            q += 4;
            n -= 4;
        }
    }

    while (n > 0)
    {
        *(__lmem int8_t *) p= *(__lmem int8_t *) q;
        p += 1;
        q += 1;
        n -= 1;
    }

    return dest;
}

__cls void *memset_cls(__cls void *dest, int32_t c, uint32_t n)
{
    uint32_t   x, p= (uint32_t)dest;
    c &= 0xff;
    x  = c<<24 | c<<16 | c<<8 | c;

    if (ALIGNED_U64(p))
    {
        uint64_t   xx= (uint64_t)x<<32 | x;

        while (n >= 8)
        {
            *(__cls uint64_t *) p= xx;
            p += 8;
            n -= 8;
        }
    }

    if (ALIGNED_U32(p))
    {
        while (n >= 4)
        {
            *(__cls uint32_t *) p= x;
            p += 4;
            n -= 4;
        }
    }

    while (n > 0 )
    {
        *(__cls int8_t *) p= (int8_t)c;
        p += 1;
        n -= 1;
    }

    return dest;
}


__lmem void *memset_lmem(__lmem void *dest, int32_t c, uint32_t n)
{
    uint32_t   x, p= (uint32_t)dest;
    c &= 0xff;
    x  = c<<24 | c<<16 | c<<8 | c;

    if (ALIGNED_U64(p))
    {
        uint64_t   xx= (uint64_t)x<<32 | x;

        while (n >= 8)
        {
            *(__lmem uint64_t *) p= xx;
            p += 8;
            n -= 8;
        }
    }

    if (ALIGNED_U32(p))
    {
        while (n >= 4)
        {
            *(__lmem uint32_t *) p= x;
            p += 4;
            n -= 4;
        }
    }

    while (n > 0 )
    {
        *(__lmem int8_t *) p= (int8_t)c;
        p += 1;
        n -= 1;
    }

    return dest;
}


int32_t memcmp_cls_cls(__cls void *buf1, __cls void *buf2, uint32_t n)
{
    uint32_t  p= (uint32_t)buf1;
    uint32_t  q= (uint32_t)buf2;

    if (ALIGNED_U32(p) && ALIGNED_U32(q))
    {
        while (n >= 4)
        {
            uint32_t  v=  *(__cls uint32_t *)p;
            uint32_t  w=  *(__cls uint32_t *)q;
            if (v!=w)
            {
#ifndef __BIGENDIAN
                v = bswap(v);
                w = bswap(w);
#endif
                return (v<w) ? -1 : 1;
            }

            p += 4;
            q += 4;
            n -= 4;
        }
    }

    while (n > 0)
    {
        uint8_t  v=  *(__cls uint8_t *)p;
        uint8_t  w=  *(__cls uint8_t *)q;
        if (v!=w)
        {
            return (v<w) ? -1 : 1;
        }

        p += 1;
        q += 1;
        n -= 1;
    }

    return 0;
}


int32_t memcmp_cls_lmem(__cls void *buf1, __lmem void *buf2, uint32_t n)
{
    uint32_t  p= (uint32_t)buf1;
    uint32_t  q= (uint32_t)buf2;

    if (ALIGNED_U32(p) && ALIGNED_U32(q))
    {
        while (n >= 4)
        {
            uint32_t  v=  *(__cls uint32_t *)p;
            uint32_t  w=  *(__lmem uint32_t *)q;

            if (v!=w)
            {
#ifndef __BIGENDIAN
                v = bswap(v);
                w = bswap(w);
#endif
                return (v<w) ? -1 : 1;
            }
            p += 4;
            q += 4;
            n -= 4;
        }
    }

    while (n > 0)
    {
        uint8_t  v=  *(__cls uint8_t *)p;
        uint8_t  w=  *(__lmem uint8_t *)q;

    if (v!=w)
    {
            return (v<w) ? -1 : 1;
    }
        p += 1;
        q += 1;
        n -= 1;
    }

    return 0;
}


int32_t memcmp_lmem_cls(__lmem void *buf1, __cls void *buf2, uint32_t n)
{
    uint32_t  p= (uint32_t)buf1;
    uint32_t  q= (uint32_t)buf2;

    if (ALIGNED_U32(p) && ALIGNED_U32(q))
    {
        while (n >= 4)
        {
            uint32_t  v=  *(__lmem uint32_t *)p;
            uint32_t  w=  *(__cls uint32_t *)q;

            if (v!=w)
            {
#ifndef __BIGENDIAN
                v = bswap(v);
                w = bswap(w);
#endif
                return (v<w) ? -1 : 1;
            }

            p += 4;
            q += 4;
            n -= 4;
        }
    }

    while (n > 0)
    {
        uint8_t  v=  *(__lmem uint8_t *)p;
        uint8_t  w=  *(__cls uint8_t *)q;

        if (v!=w)
        {
            return (v<w) ? -1 : 1;
        }

        p += 1;
        q += 1;
        n -= 1;
    }

    return 0;
}

int32_t memcmp_lmem_lmem(__lmem void *buf1, __lmem void *buf2, uint32_t n)
{
    uint32_t  p= (uint32_t)buf1;
    uint32_t  q= (uint32_t)buf2;

    if (ALIGNED_U32(p) && ALIGNED_U32(q))
    {
        while (n >= 4)
        {
            uint32_t  v=  *(__lmem uint32_t *)p;
            uint32_t  w=  *(__lmem uint32_t *)q;

            if (v!=w)
            {
#ifndef __BIGENDIAN
                v = bswap(v);
                w = bswap(w);
#endif
                return (v<w) ? -1 : 1;
            }
            p += 4;
            q += 4;
            n -= 4;
        }
    }

    while (n > 0)
    {
        uint8_t  v=  *(__lmem uint8_t *)p;
        uint8_t  w=  *(__lmem uint8_t *)q;

        if (v!=w)
        {
            return (v<w) ? -1 : 1;
        }
        p += 1;
        q += 1;
        n -= 1;
    }

    return 0;
}


__lmem void *memmove_lmem_lmem(__lmem void *dest, __lmem void *src, uint32_t n)
{
    uint32_t  p= (uint32_t)dest;
    uint32_t  q= (uint32_t)src;

    if (p<=q || p>=(q+n))
    {
        return memcpy_lmem_lmem(dest, src, n);
    }

    p += n;
    q += n;
    while (n > 0)
    {
        p -= 1;
        q -= 1;
        n -= 1;
        *(__lmem int8_t *) p= *(__lmem int8_t *) q;
    }
    return dest;
}


__lmem void *memchr_lmem(__lmem void *src, int32_t c, uint32_t n)
{
    int32_t  i;
    uint32_t  p= (uint32_t)src;
    int32_t  off=0;

    if (ALIGNED_U64(p))
    {
        while (n >= 8)
        {
            uint64_t   v=  *(__lmem uint64_t *)p;

#ifdef __BIGENDIAN
            for(i=56; i>=0; i-= 8)
#else
            for(i=0; i<64; i+= 8)
#endif
            {
                if ((uint8_t)(v>>i) == (uint8_t)c)
                {
                    return (__lmem int8_t*)src + off;
                }
                off++;
            }
            p += 8;
            n -= 8;
        }
    }

    if (ALIGNED_U32(p))
    {
        while (n >= 4)
        {
            uint32_t v=  *(__lmem uint32_t *)p;

#ifdef __BIGENDIAN
            for(i=24; i>=0; i-= 8)
#else
            for(i=0; i<32; i+= 8)
#endif
            {
                if ((uint8_t)(v>>i) == (uint8_t)c)
                {
                    return (__lmem int8_t*)src + off;
                }
                off++;
            }
            p += 4;
            n -= 4;
        }
    }

    while (n > 0)
    {
        uint8_t v=  *(__lmem uint8_t *)p;

        if (v == (uint8_t)c)
        {
            return (__lmem int8_t*)src + off;
        }
        off++;
        p += 1;
        n -= 1;
    }

    return NULL;
}

__cls void *memchr_cls(__cls void *src, int32_t c, uint32_t n)
{
    int32_t  i;
    uint32_t  p= (uint32_t)src;
    int32_t  off=0;

    if (ALIGNED_U64(p))
    {
        while (n >= 8)
        {
            uint64_t   v=  *(__cls uint64_t *)p;

#ifdef __BIGENDIAN
            for(i=56; i>=0; i-= 8)
#else
            for(i=0; i<64; i+= 8)
#endif
            {
                if ((uint8_t)(v>>i) == (uint8_t)c)
                {
                    return (__cls int8_t*)src + off;
                }
                off++;
            }
            p += 8;
            n -= 8;
        }
    }

    if (ALIGNED_U32(p))
    {
        while (n >= 4)
        {
            uint32_t v=  *(__cls uint32_t *)p;

#ifdef __BIGENDIAN
            for(i=24; i>=0; i-= 8)
#else
            for(i=0; i<32; i+= 8)
#endif
            {
                if ((uint8_t)(v>>i) == (uint8_t)c)
                {
                    return (__cls int8_t*)src + off;
                }
                off++;
            }
            p += 4;
            n -= 4;
        }
    }

    while (n > 0)
    {
        uint8_t v=  *(__cls uint8_t *)p;

        if (v == (uint8_t)c)
        {
            return (__cls int8_t*)src + off;
        }
        off++;
        p += 1;
        n -= 1;
    }

    return NULL;
}


/*
 *  return the number of characters in s, excluding the terminating NULL
 */
uint32_t strlen_mem(__mem int8_t *s)
{
    __mem int8_t *p = s;
    while(*p)
    {
        p++;
    }
    return p - s;
}


/*
 *  strcmp()
 *
 *  Compare two strings. Perform unsigned comparison on each int8_t.
 *  Return:
 *       value if: s1 is smaller than s2,
 *       if s1 is equal to s2, and
 *       value if s1 is greater than s2.
 *  Similar functions are provided for operations in all memory regions
 */

int32_t strcmp_mem(__mem int8_t *s1, __mem int8_t *s2)
{
    while(*s1==*s2 && *s1)
    {
        s1++;
        s2++;
    }
    return (uint8_t)*s1 - (uint8_t)*s2;
}




/*
 *  strncmp()
 *
 *  Compare two strings  up to a specified count of characters.
 *  Perform unsigned comparison on each int8_t.
 *  Return values similar to strcmp()
 *  Similar functions are provided for operations in all memory regions.
 *  Cross product is not supported, because, clearly this is getting ridiculous.
 */

int32_t strncmp_mem(__mem int8_t *s1, __mem int8_t *s2, uint32_t count)
{
    if (count==0)
    {
        return 0;
    }

    while(--count && *s1==*s2 && *s1)
    {
        s1++;
        s2++;
    }

    return (uint8_t)*s1 - (uint8_t)*s2;
}


/*
 *  strcpy()
 *
 *  Copy source string to dest string, including the terminiating NULL character,
 *  and return the original dest.
 *  Similar functions are provided for operations in all memory regions.
 */

__mem int8_t *strcpy_mem(__mem int8_t *dest, __mem int8_t *src)
{
    __mem int8_t *ret = dest;
    while((*dest++ = *src++)!=0)
        ;
    return ret;
}


/*
 *  strncpy()
 *
 *  Copy source string to dest string, including the terminiating NULL character,
 *  up to a specified count of characters, and return the original dest.
 *  Similar functions are provided for operations in all memory regions.
 */

__mem int8_t *strncpy_mem(__mem int8_t *dest, __mem int8_t *src, uint32_t count)
{
    __mem int8_t *ret = dest;

    for(; count && (*dest++ = *src++)!=0; count--)
        ;

    while(count-- > 0)      // 0x00-padding
        *dest++ = 0x00;
    return ret;
}



/*
 *  strcat()
 *
 *  Append source string to dest string, including the terminiating NULL character,
 *  and return the original dest.
 *  Similar functions are provided for operations in all memory regions.
 */

__mem int8_t *strcat_mem(__mem int8_t *dest, __mem int8_t *src)
{
    __mem int8_t *ret;

    for(ret= dest; *dest; dest++)
        ;

    while((*dest++ = *src++)!=0)
        ;

    return ret;
}


/*
 *  strncat()
 *
 *  Append source string to dest string, including the terminiating NULL character,
 *  up to a specified count of characters, and return the original dest.
 *  Similar functions are provided for operations in all memory regions.
 */

__mem int8_t *strncat_mem(__mem int8_t *dest, __mem int8_t *src, uint32_t count)
{
    __mem int8_t *ret;

    for(ret= dest; *dest; dest++)
        ;

    // the following differs from strncpy in that dest is ALWAYS 0x00-terminted
    while(count--)
    {
        if ((*dest++ = *src++)==0)
        {
            *dest = 0x00;
            return ret;
        }
    }
    if(*dest != 0)
        *dest = 0x00;
    return ret;
}

/*
 *  strchr()
 *
 *  Return a pointer to the first occurrence of character c (could be 0x00) in
 *  string s, or NULL if none.
 *  Similar functions are provided for operations in all memory regions
 */

__mem int8_t *strchr_mem(__mem int8_t *s, int8_t c)
{
    while(*s && *s!=c)
    {
        s++;
    }
    return (*s==c)? (__mem int8_t *)s : NULL;
}



/*
 *  strrchr()
 *
 *  Return a pointer to the last occurrence of character c (could be 0x00) in
 *  string s, or NULL if none.
 *  Similar functions are provided for operations in all memory regions
 */

__mem int8_t *strrchr_mem(__mem int8_t *s, int8_t c)     // c can be 0x00
{
    __mem int8_t *head = s;
    while(*s++)
        ;
    while(--s!=head && *s!=c)
        ;
    return (*s==c)? (__mem int8_t *)s : NULL;
}


/*
 *  __strto_l_ul()
 */

static uint32_t _strto_l_ul_mem(__mem int8_t *s, __mem int8_t **ps_end, int32_t base, int32_t is_signed)
{
    int32_t neg, overflow;
    uint32_t ret, max_quo, max_rem, n_digit;

    __mem int8_t *s0 = s;

    // skip while spaces 0x09-0x0D,0x20
    while (*s==0x20 || (uint32_t)(*s-0x09)<5)
    {
        s++;
    }
    // handle '-' and '+'
    if (*s=='-')
    {
        neg = 1;
        s++;
    } else {
        neg = 0;
        if (*s=='+')
            s++;
    }
    // determine valid base (between 2..36)
    if (base==0)
    {
        if (*s=='0')
        {
            base = (s[1]=='x' || s[1]=='X')? 16 : 8;
        } else
        {
            base = 10;
        }
    } else if ((uint32_t)(base-2)>34)
    {
        if (ps_end)
            *ps_end = s0;
        return 0;
    }

    if (base==16 && *s=='0' && (s[1]=='x' || s[1]=='X'))
        s+=2;   // slip prefix "0x"

    max_quo = ULONG_MAX / (uint32_t)base;
    max_rem = ULONG_MAX % (uint32_t)base;

    // parse
    for(ret=0,n_digit=0,overflow=0; ; n_digit++,s++) {
        uint32_t digit;
        if ((uint32_t)(*s-'0')<10)
        {
            digit = *s - '0';
        } else if ((uint32_t)(*s-'A')<26)
        {
            digit = *s - 'A' + 10;
        } else if ((uint32_t)(*s-'a')<26)
        {
            digit = *s - 'a' + 10;
        } else
        {
            break;
        }
        if (digit >= (uint32_t)base)
            break;

        if (!overflow)
        {
            if (ret >= max_quo)
            {
                if (!(ret==max_quo && digit <=max_rem))
                {
                    overflow = 1;
                    continue;
                }
            }

            ret = ret * (uint32_t)base + digit;
            if (is_signed &&
                ret > (1UL<<31)-1+neg)    // (neg && ret > -LONG_MIN) || (!neg && ret > LONG_MAX)))
            {
                overflow = 1;
            }
        }
    }
    // check n_digit
    if (n_digit==0)
    {
        if (ps_end)
            *ps_end = s0;
        return 0;
    }
    // check overflow
    if (overflow)
    {
        if (is_signed)
        {
            ret = (neg)? (uint32_t)(-LONG_MIN) : LONG_MAX;
        } else
        {
            ret = ULONG_MAX;
        }
    }
    if (neg)
    // check neg
    {
        ret = -(int32_t)ret;   // according to spec, do it even when ret==ULONG_MAX!!
    }

    if (ps_end)
        *ps_end = (__mem int8_t *)s;

    return ret;
}



int32_t strtol_mem(__mem int8_t *s, __mem int8_t **ps_end, int32_t base)
{
    return (int32_t)_strto_l_ul_mem(s,ps_end,base,1/*is_signed*/);
}

uint32_t strtoul_mem(__mem int8_t *s, __mem int8_t **ps_end, int32_t base)
{
    return _strto_l_ul_mem(s,ps_end,base,0/*!is_signed*/);
}

__mem void *memcpy_mem_mem(__mem void *dest, __mem void *src, uint32_t n)
{
    uint32_t  p= (uint32_t)dest;
    uint32_t  q= (uint32_t)src;

    if (ALIGNED_U64(p) && ALIGNED_U64(q))
    {
        while (n >= 8)
        {
            *(__mem uint64_t *) p = *(__mem uint64_t *) q;
        p += 8;
        q += 8;
            n -= 8;
    }
    }

    if (ALIGNED_U32(p) && ALIGNED_U32(q))

    {
        while (n >= 4)
        {
            *(__mem uint32_t *) p= *(__mem uint32_t *) q;
        p += 4;
        q += 4;
            n -= 4;
    }
    }

    while (n > 0)
    {
        *(__mem int8_t *) p= *(__mem int8_t *) q;
    p += 1;
    q += 1;
        n -= 1;
    }

    return dest;
}

__mem void *memcpy_mem_cls(__mem void *dest, __cls void *src, uint32_t n)
{
    uint32_t  p= (uint32_t)dest;
    uint32_t  q= (uint32_t)src;

    if (ALIGNED_U64(p) && ALIGNED_U64(q))
    {
        while (n >= 8)
        {
            *(__mem uint64_t *) p = *(__cls uint64_t *) q;
        p += 8;
        q += 8;
            n -= 8;
    }
    }

    if (ALIGNED_U32(p) && ALIGNED_U32(q))
    {
        while (n >= 4)
        {
            *(__mem uint32_t *) p= *(__cls uint32_t *) q;
        p += 4;
        q += 4;
            n -= 4;
    }
    }

    while (n > 0)
    {
        *(__mem int8_t *) p= *(__cls int8_t *) q;
    p += 1;
    q += 1;
        n -= 1;
    }

    return dest;
}

__mem void *memcpy_mem_lmem(__mem void *dest, __lmem void *src, uint32_t n)
{
    uint32_t  p= (uint32_t)dest;
    uint32_t  q= (uint32_t)src;

    if (ALIGNED_U64(p) && ALIGNED_U64(q))
    {
        while (n >= 8)
        {
            *(__mem uint64_t *) p = *(__lmem uint64_t *) q;
            p += 8;
            q += 8;
            n -= 8;
        }
    }

    if (ALIGNED_U32(p) && ALIGNED_U32(q))
    {
        while (n >= 4)
        {
            *(__mem uint32_t *) p= *(__lmem uint32_t *) q;
            p += 4;
            q += 4;
            n -= 4;
        }
    }

    while (n > 0)
    {
        *(__mem int8_t *) p= *(__lmem int8_t *) q;
        p += 1;
        q += 1;
        n -= 1;
    }

    return dest;
}



__cls void *memcpy_cls_mem(__cls void *dest, __mem void *src, uint32_t n)
{
    uint32_t  p= (uint32_t)dest;
    uint32_t  q= (uint32_t)src;

    if (ALIGNED_U64(p) && ALIGNED_U64(q))
    {
        while (n >= 8)
        {
            *(__cls uint64_t *) p = *(__mem uint64_t *) q;
            p += 8;
            q += 8;
            n -= 8;
        }
    }

    if (ALIGNED_U32(p) && ALIGNED_U32(q))
    {
        while (n >= 4)
        {
            *(__cls uint32_t *) p= *(__mem uint32_t *) q;
            p += 4;
            q += 4;
            n -= 4;
        }
    }

    while (n > 0)
    {
        *(__cls int8_t *) p= *(__mem int8_t *) q;
        p += 1;
        q += 1;
        n -= 1;
    }

    return dest;
}


__lmem void *memcpy_lmem_mem(__lmem void *dest, __mem void *src, uint32_t n)
{
    uint32_t  p= (uint32_t)dest;
    uint32_t  q= (uint32_t)src;

    if (ALIGNED_U64(p) && ALIGNED_U64(q))
    {
        while (n >= 8)
        {
            *(__lmem uint64_t *) p = *(__mem uint64_t *) q;
            p += 8;
            q += 8;
            n -= 8;
        }
    }

    if (ALIGNED_U32(p) && ALIGNED_U32(q))
    {
        while (n >= 4)
        {
            *(__lmem uint32_t *) p= *(__mem uint32_t *) q;
            p += 4;
            q += 4;
            n -= 4;
        }
    }

    while (n > 0)
    {
        *(__lmem int8_t *) p= *(__mem int8_t *) q;
        p += 1;
        q += 1;
        n -= 1;
    }

    return dest;
}



/*
 *  memmove()
 *
 *  Functionality is the same as memcpy, except for overlapping buffers,
 *  which you shouldn't ever do.
 *
 *  for different memory type, they have independent address space
 *  so it is safe to directly call memcpy
 *
 */


__mem void *memmove_mem_mem(__mem void *dest, __mem void *src, uint32_t n)
{
    uint32_t  p= (uint32_t)dest;
    uint32_t  q= (uint32_t)src;

    if (p<=q || p>=(q+n))
    {
        return memcpy_mem_mem(dest, src, n);
    }

    p += n;
    q += n;
    while (n > 0 )
    {
    p -= 1;
    q -= 1;
        n -= 1;
        *(__mem int8_t *) p= *(__mem int8_t *) q;
    }
    return dest;
}


__mem void *memmove_mem_lmem(__mem void *dest, __lmem void *src, uint32_t n)
{
    return memcpy_mem_lmem(dest, src, n);
}

__lmem void *memmove_lmem_mem(__lmem void *dest, __mem void *src, uint32_t n)
{
    return memcpy_lmem_mem(dest, src, n);
}

/*
 *  memset()
 *
 *  Fill memory buffer dest with a repeated 8-bit character c and return dest.
 *  Similar functions are provided for the cross products in all memory regions
 *  { mem, cls, lmem }
 */
__mem void *memset_mem(__mem void *dest, int32_t c, uint32_t n)
{
    uint32_t   x, p= (uint32_t)dest;
    c &= 0xff;
    x  = c<<24 | c<<16 | c<<8 | c;

    if (ALIGNED_U64(p))
    {
        uint64_t   xx= (uint64_t)x<<32 | x;

        while (n >= 8)
        {
            *(__mem uint64_t *) p= xx;
            p += 8;
            n -= 8;
        }
    }

    if (ALIGNED_U32(p))
    {
        while (n >= 4)
        {
            *(__mem uint32_t *) p= x;
            p += 4;
            n -= 4;
        }
    }

    while (n > 0 )
    {
        *(__mem int8_t *) p= (int8_t)c;
        p += 1;
        n -= 1;
    }

    return dest;
}
/*
 *  memcmp()
 *
 *  Compare two (possibly misaligned) memory buffers in SRAM.
 *  Perform unsigned comparison on each byte.
 *  Return
 *      <0 value if buf1 is smaller than buf2,
 *      =0 if buf1 is equal to buf2,
 *      >0 value if buf1 is greater than buf2.
 *  Similar functions are provided for the cross products in all memory regions
 *  { mem, cls, lmem } x { mem, cls, lmem }
 */


int32_t memcmp_mem_mem(__mem void *buf1, __mem void *buf2, uint32_t n)
{
    uint32_t  p= (uint32_t)buf1;
    uint32_t  q= (uint32_t)buf2;

    if (ALIGNED_U32(p) && ALIGNED_U32(q))
    {
        while (n >= 4)
        {
            uint32_t  v=  *(__mem uint32_t *)p;
            uint32_t  w=  *(__mem uint32_t *)q;

            if (v!=w)
            {
#ifndef __BIGENDIAN
                v = bswap(v);
                w = bswap(w);
#endif
                return (v<w) ? -1 : 1;
            }

            p += 4;
            q += 4;
            n -= 4;
        }
    }

    while (n > 0 )
    {
        uint8_t  v=  *(__mem uint8_t *)p;
        uint8_t  w=  *(__mem uint8_t *)q;

        if (v!=w)
        {
            return (v<w) ? -1 : 1;
        }

        p += 1;
        q += 1;
        n -= 1;
    }

    return 0;
}

int32_t memcmp_mem_cls(__mem void *buf1, __cls void *buf2, uint32_t n)
{
    uint32_t  p= (uint32_t)buf1;
    uint32_t  q= (uint32_t)buf2;

    if (ALIGNED_U32(p) && ALIGNED_U32(q))
    {
        while (n >= 4)
        {
            uint32_t  v=  *(__mem uint32_t *)p;
            uint32_t  w=  *(__cls uint32_t *)q;

            if (v!=w)
            {
#ifndef __BIGENDIAN
                v = bswap(v);
                w = bswap(w);
#endif
                return (v<w) ? -1 : 1;
            }

            p += 4;
            q += 4;
            n -= 4;
        }
    }

    while (n > 0 )
    {
        uint8_t  v=  *(__mem uint8_t *)p;
        uint8_t  w=  *(__cls uint8_t *)q;

        if (v!=w)
        {
            return (v<w) ? -1 : 1;
        }

        p += 1;
        q += 1;
        n -= 1;
    }

    return 0;
}

int32_t memcmp_mem_lmem(__mem void *buf1, __lmem void *buf2, uint32_t n)
{
    uint32_t  p= (uint32_t)buf1;
    uint32_t  q= (uint32_t)buf2;

    if (ALIGNED_U32(p) && ALIGNED_U32(q))
    {
        while (n >= 4)
        {
            uint32_t  v=  *(__mem uint32_t *)p;
            uint32_t  w=  *(__lmem uint32_t *)q;

            if (v!=w)
            {
#ifndef __BIGENDIAN
                v = bswap(v);
                w = bswap(w);
#endif
                return (v<w) ? -1 : 1;
            }

            p += 4;
            q += 4;
            n -= 4;
        }
    }

    while (n > 0)
    {
        uint8_t  v=  *(__mem uint8_t *)p;
        uint8_t  w=  *(__lmem uint8_t *)q;

        if (v!=w)
        {
            return (v<w) ? -1 : 1;
        }

        p += 1;
        q += 1;
        n -= 1;
    }

    return 0;
}

int32_t memcmp_cls_mem(__cls void *buf1, __mem void *buf2, uint32_t n)
{
    uint32_t  p= (uint32_t)buf1;
    uint32_t  q= (uint32_t)buf2;

    if (ALIGNED_U32(p) && ALIGNED_U32(q))
    {
        while (n >= 4)
        {
            uint32_t  v=  *(__cls uint32_t *)p;
            uint32_t  w=  *(__mem uint32_t *)q;

            if (v!=w)
            {
#ifndef __BIGENDIAN
                v = bswap(v);
                w = bswap(w);
#endif
                return (v<w) ? -1 : 1;
            }

            p += 4;
            q += 4;
            n -= 4;
        }
    }

    while (n > 0)
    {
        uint8_t  v=  *(__cls uint8_t *)p;
        uint8_t  w=  *(__mem uint8_t *)q;

        if (v!=w)
        {
            return (v<w) ? -1 : 1;
        }

        p += 1;
        q += 1;
        n -= 1;
    }

    return 0;
}

int32_t memcmp_lmem_mem(__lmem void *buf1, __mem void *buf2, uint32_t n)
{
    uint32_t  p= (uint32_t)buf1;
    uint32_t  q= (uint32_t)buf2;

    if (ALIGNED_U32(p) && ALIGNED_U32(q))
    {
        while (n >= 4)
        {
            uint32_t  v=  *(__lmem uint32_t *)p;
            uint32_t  w=  *(__mem uint32_t *)q;

            if (v!=w)
            {
#ifndef __BIGENDIAN
                v = bswap(v);
                w = bswap(w);
#endif
                return (v<w) ? -1 : 1;
            }

            p += 4;
            q += 4;
            n -= 4;
        }
    }

    while (n > 0)
    {
        uint8_t  v=  *(__lmem uint8_t *)p;
        uint8_t  w=  *(__mem uint8_t *)q;

        if (v!=w)
        {
            return (v<w) ? -1 : 1;
        }

        p += 1;
        q += 1;
        n -= 1;
    }

    return 0;
}

/*
 *  memchr()
 *
 *  Return the offset to the first occurence of an 8 bit character c in memory buffer,
 *  or -1 if none.
 *
 *  Similar functions are provided for the cross products in all memory regions
 *      { mem, cls, lmem }
 */

__mem void *memchr_mem(__mem void *src, int32_t c, uint32_t n)
{
    int32_t  i;
    uint32_t  p= (uint32_t)src;
    int32_t  off=0;

    if (ALIGNED_U64(p))
    {
        while (n >= 8)
        {
            uint64_t   v=  *(__mem uint64_t *)p;

#ifdef __BIGENDIAN
            for(i=56; i>=0; i-= 8)
#else
            for(i=0; i<64; i+= 8)
#endif
            {
                if ((uint8_t)(v>>i) == (uint8_t)c)
                {
                    return (__mem int8_t*)src + off;
                }
                off++;
            }
            p += 8;
            n -= 8;
        }
    }

    if (ALIGNED_U32(p))
    {
        while (n >= 4)
        {
            uint32_t v=  *(__mem uint32_t *)p;

#ifdef __BIGENDIAN
            for(i=24; i>=0; i-= 8)
#else
            for(i=0; i<32; i+= 8)
#endif
            {
                if ((uint8_t)(v>>i) == (uint8_t)c)
                {
                    return (__mem int8_t*)src + off;
                }
                off++;
            }

            p += 4;
            n -= 4;
        }
    }

    while (n > 0)
    {
        uint8_t v=  *(__mem uint8_t *)p;

        if (v == (uint8_t)c)
        {
            return (__mem int8_t*)src + off;
        }
        off++;
        p += 1;
        n -= 1;
    }

    return NULL;
}


/*
   unaligned memory copy: LM + [0..7] <- xfer

   note: This function could be substantially smaller if
         1) byte_align_be[] can take *l$index0++
         2) byte_align_be can be separated
         3) LM offset is ADDed, not ORed
 */
void memcpy_lmem0_7_xfer_w_clr(
    __lmem void *dst,                   // aligned (>=4) dst (depends on length)
    uint32_t dst_off,               // 0..7 (better be constant)
    uint32_t ctx,                   // 0..7 (better be constant)
    uint32_t xfer_reg_number,       // __implicit_read(&xfer) should follow this call
    uint32_t src_off,               // any in range of xfer[]
    uint32_t length                 // in byte
)
{
    uint32_t ii, Z, M, lm_off;
    uint32_t d = dst_off & 3;
    uint32_t s = src_off & 3;
    uint32_t tibi = (ctx << 6)                           |
                        (xfer_reg_number + (src_off >> 2) << 2) |
                        ((src_off - dst_off) & 3);
    local_csr_write(local_csr_t_index_byte_index, tibi);
    local_csr_write(local_csr_active_lm_addr_0, (uint32_t)dst);

    // zero out header longwords

    if (length < 4)
    {
        if (length)
        {
            __asm alu[M, --, B, *$index++]
            if (s)
            {
                ii = (4 - s) << 3;
                __asm
                {
                    alu[--, ii, OR, 0]
                    dbl_shf[M, M, *$index, >>indirect]
                }
            }

            switch (length)
            {
                case 0:
                    __asm ld_field_w_clr[Z, 0000b, M, >>0]
                    break;

                case 1:
                    __asm ld_field_w_clr[Z, 1000b, M, >>0]
                    break;

                case 2:
                    __asm ld_field_w_clr[Z, 1100b, M, >>0]
                    break;

                case 3:
                    __asm ld_field_w_clr[Z, 1110b, M, >>0]
                    break;

                default:
                        __impossible_path();
                    __switch_pack(swpack_auto);
            }

            ii = d << 3;
            if (d + length <= 4)
            {
                if (dst_off >> 2)
                {
                    // dst_off = 4..7
                    __asm
                    {
                        alu[*l$index0[0], ii, B, 0]
                        alu_shf[*l$index0[1], --, B, Z, >>indirect]
                    }
                }
                else
                {
                    __asm
                    {
                        alu[--, ii, OR, 0]
                        alu_shf[*l$index0[0], --, B, Z, >>indirect]
                    }
                }
            }
            else
            {
                if (dst_off >> 2)
                {
                    // dst_off = 4..7
                    __asm
                    {
                        alu[*l$index0[0], ii, B, 0]
                        dbl_shf[*l$index0[2], Z, 0, >>indirect]
                        alu[--, ii, OR, 0]
                        alu_shf[*l$index0[1], --, B, Z, >>indirect]
                    }
                }
                else
                {
                    __asm
                    {
                        alu[--, ii, OR, 0]
                        dbl_shf[*l$index0[1], Z, 0, >>indirect]
                        alu[--, ii, OR, 0]
                        alu_shf[*l$index0[0], --, B, Z, >>indirect]

                    }
                }
            }
        } // if (length)
    }
    else
    {
        //length >= 4

        if (d < s)
        {
            switch (s)
            {
                case 0:
                    __asm ld_field_w_clr[Z, 1111b, *$index++, >>0]
                    break;

                case 1:
                    __asm ld_field_w_clr[Z, 0111b, *$index++, >>0]
                    break;

                case 2:
                    __asm ld_field_w_clr[Z, 0011b, *$index++, >>0]
                    break;

                case 3:
                    __asm ld_field_w_clr[Z, 0001b, *$index++, >>0]
                    break;

                default:
                    __impossible_path();
                    __switch_pack(swpack_auto);
            }

            if (dst_off >> 2)
            {
                // dst_off = 4..7
                __asm alu[*l$index0[0], --, B, 0]
                lm_off = 1;
            }
            else
            {
                lm_off = 0;
            }

            length += d;
        }
        else
        {
            // d >= s
            if (d == s)
            {
                switch (d)
                {
                    case 0:
                        __asm ld_field_w_clr[Z, 1111b, *$index++, >>0]
                        break;

                    case 1:
                        __asm ld_field_w_clr[Z, 0111b, *$index++, >>0]
                        break;

                    case 2:
                        __asm ld_field_w_clr[Z, 0011b, *$index++, >>0]
                        break;

                    case 3:
                        __asm ld_field_w_clr[Z, 0001b, *$index++, >>0]
                        break;

                    default:
                        __impossible_path();
                        __switch_pack(swpack_auto);
                }
            }
            else
            {
                ii = (d - s) << 3;
                switch (d)
                {
                    case 0:
                        __asm
                        {
                            alu[--, ii, OR, 0]
                            ld_field_w_clr[Z, 1111b, *$index, >>indirect]
                        }
                        break;

                    case 1:
                        __asm
                        {
                            alu[--, ii, OR, 0]
                            ld_field_w_clr[Z, 0111b, *$index, >>indirect]
                        }
                        break;

                    case 2:
                        __asm
                        {
                            alu[--, ii, OR, 0]
                            ld_field_w_clr[Z, 0011b, *$index, >>indirect]
                        }
                        break;

                    case 3:
                        __asm
                        {
                            alu[--, ii, OR, 0]
                            ld_field_w_clr[Z, 0001b, *$index, >>indirect]
                        }
                        break;

                    default:
                        __impossible_path();
                        __switch_pack(swpack_auto);
                } // switch(d)
            }

            if (dst_off >> 2)
            {
                // dst_off = 4..7
                __asm alu[*l$index0[0], --, B, 0]
                __asm alu[*l$index0[1], --, B, Z]
                lm_off = 2;
            }
            else
            {
                __asm alu[*l$index0[0], --, B, Z]
                lm_off = 1;
            }
            __asm alu[Z, --, B, *$index++]

            length -= (4 - d);
        }

        switch (length >> 2)
        {
            case 0:
                __asm
                {
                    byte_align_be[         --, Z        ]
                    byte_align_be[          Z, *$index  ]
                }
                break;

            case 1:
                if (lm_off == 0)
                {
                    __asm
                    {
                        byte_align_be[          --, Z        ]
                        byte_align_be[*l$index0[0], *$index++]
                        byte_align_be[           Z, *$index  ]
                    }
                }
                else if (lm_off == 1)
                {
                    __asm
                    {
                        byte_align_be[          --, Z        ]
                        byte_align_be[*l$index0[1], *$index++]
                        byte_align_be[           Z, *$index  ]
                    }
                }
                else
                {
                    //ToDo: lm_off must be 2 but not checked
                    __asm
                    {
                        byte_align_be[          --, Z        ]
                        byte_align_be[*l$index0[2], *$index++]
                        byte_align_be[           Z, *$index  ]
                    }
                }
                break;

            case 2:
                if (lm_off == 0)
                {
                    __asm
                    {
                        byte_align_be[          --, Z        ]
                        byte_align_be[*l$index0[0], *$index++]
                        byte_align_be[*l$index0[1], *$index++]
                        byte_align_be[           Z, *$index  ]
                    }
                }
                else if (lm_off == 1)
                {
                    __asm
                    {
                        byte_align_be[          --, Z        ]
                        byte_align_be[*l$index0[1], *$index++]
                        byte_align_be[*l$index0[2], *$index++]
                        byte_align_be[           Z, *$index  ]
                    }
                }
                else
                {
                    //ToDo: lm_off must be 2 but not checked
                    __asm
                    {
                        byte_align_be[          --, Z        ]
                        byte_align_be[*l$index0[2], *$index++]
                        byte_align_be[*l$index0[3], *$index++]
                        byte_align_be[           Z, *$index  ]
                    }
                }
                break;

            case 3:
                if (lm_off == 0)
                {
                    __asm
                    {
                        byte_align_be[          --, Z        ]
                        byte_align_be[*l$index0[0], *$index++]
                        byte_align_be[*l$index0[1], *$index++]
                        byte_align_be[*l$index0[2], *$index++]
                        byte_align_be[           Z, *$index  ]
                    }
                }
                else if (lm_off == 1)
                {
                    __asm
                    {
                        byte_align_be[          --, Z        ]
                        byte_align_be[*l$index0[1], *$index++]
                        byte_align_be[*l$index0[2], *$index++]
                        byte_align_be[*l$index0[3], *$index++]
                        byte_align_be[           Z, *$index  ]
                    }
                }
                else
                {
                    //ToDo: lm_off must be 2 but not checked
                    __asm
                    {
                        byte_align_be[          --, Z        ]
                        byte_align_be[*l$index0[2], *$index++]
                        byte_align_be[*l$index0[3], *$index++]
                        byte_align_be[*l$index0[4], *$index++]
                        byte_align_be[           Z, *$index  ]
                    }
                }
                break;

            case 4:
                if (lm_off == 0)
                {
                    __asm
                    {
                        byte_align_be[          --, Z        ]
                        byte_align_be[*l$index0[0], *$index++]
                        byte_align_be[*l$index0[1], *$index++]
                        byte_align_be[*l$index0[2], *$index++]
                        byte_align_be[*l$index0[3], *$index++]
                        byte_align_be[           Z, *$index  ]
                    }
                }
                else if (lm_off == 1)
                {
                    __asm
                    {
                        byte_align_be[          --, Z        ]
                        byte_align_be[*l$index0[1], *$index++]
                        byte_align_be[*l$index0[2], *$index++]
                        byte_align_be[*l$index0[3], *$index++]
                        byte_align_be[*l$index0[4], *$index++]
                        byte_align_be[           Z, *$index  ]
                    }
                }
                else
                {
                    //ToDo: lm_off must be 2 but not checked
                    __asm
                    {
                        byte_align_be[          --, Z        ]
                        byte_align_be[*l$index0[2], *$index++]
                        byte_align_be[*l$index0[3], *$index++]
                        byte_align_be[*l$index0[4], *$index++]
                        byte_align_be[*l$index0[5], *$index++]
                        byte_align_be[           Z, *$index  ]
                    }
                }
                break;

            case 5:
                if (lm_off == 0)
                {
                    __asm
                    {
                        byte_align_be[          --, Z        ]
                        byte_align_be[*l$index0[0], *$index++]
                        byte_align_be[*l$index0[1], *$index++]
                        byte_align_be[*l$index0[2], *$index++]
                        byte_align_be[*l$index0[3], *$index++]
                        byte_align_be[*l$index0[4], *$index++]
                        byte_align_be[           Z, *$index  ]
                    }
                }
                else if (lm_off == 1)
                {
                    __asm
                    {
                        byte_align_be[          --, Z        ]
                        byte_align_be[*l$index0[1], *$index++]
                        byte_align_be[*l$index0[2], *$index++]
                        byte_align_be[*l$index0[3], *$index++]
                        byte_align_be[*l$index0[4], *$index++]
                        byte_align_be[*l$index0[5], *$index++]
                        byte_align_be[           Z, *$index  ]
                    }
                }
                else
                {
                    //ToDo: lm_off must be 2 but not checked
                    __asm
                    {
                        byte_align_be[          --, Z        ]
                        byte_align_be[*l$index0[2], *$index++]
                        byte_align_be[*l$index0[3], *$index++]
                        byte_align_be[*l$index0[4], *$index++]
                        byte_align_be[*l$index0[5], *$index++]
                        byte_align_be[*l$index0[6], *$index++]
                        byte_align_be[           Z, *$index  ]
                    }
                }
                break;

            case 6:
                if (lm_off == 0)
                {
                    __asm
                    {
                        byte_align_be[          --, Z        ]
                        byte_align_be[*l$index0[0], *$index++]
                        byte_align_be[*l$index0[1], *$index++]
                        byte_align_be[*l$index0[2], *$index++]
                        byte_align_be[*l$index0[3], *$index++]
                        byte_align_be[*l$index0[4], *$index++]
                        byte_align_be[*l$index0[5], *$index++]
                        byte_align_be[           Z, *$index  ]
                    }
                }
                else if (lm_off == 1)
                {
                    __asm
                    {
                        byte_align_be[          --, Z        ]
                        byte_align_be[*l$index0[1], *$index++]
                        byte_align_be[*l$index0[2], *$index++]
                        byte_align_be[*l$index0[3], *$index++]
                        byte_align_be[*l$index0[4], *$index++]
                        byte_align_be[*l$index0[5], *$index++]
                        byte_align_be[*l$index0[6], *$index++]
                        byte_align_be[           Z, *$index  ]
                    }
                }
                else
                {
                    //ToDo: lm_off must be 2 but not checked
                    __asm
                    {
                        byte_align_be[          --, Z        ]
                        byte_align_be[*l$index0[2], *$index++]
                        byte_align_be[*l$index0[3], *$index++]
                        byte_align_be[*l$index0[4], *$index++]
                        byte_align_be[*l$index0[5], *$index++]
                        byte_align_be[*l$index0[6], *$index++]
                        byte_align_be[*l$index0[7], *$index++]
                        byte_align_be[           Z, *$index  ]
                    }
                }
                break;

            case 7:
                if (lm_off == 0)
                {
                    __asm
                    {
                        byte_align_be[          --, Z        ]
                        byte_align_be[*l$index0[0], *$index++]
                        byte_align_be[*l$index0[1], *$index++]
                        byte_align_be[*l$index0[2], *$index++]
                        byte_align_be[*l$index0[3], *$index++]
                        byte_align_be[*l$index0[4], *$index++]
                        byte_align_be[*l$index0[5], *$index++]
                        byte_align_be[*l$index0[6], *$index++]
                        byte_align_be[           Z, *$index  ]
                    }
                }
                else if (lm_off == 1)
                {
                    __asm
                    {
                        byte_align_be[          --, Z        ]
                        byte_align_be[*l$index0[1], *$index++]
                        byte_align_be[*l$index0[2], *$index++]
                        byte_align_be[*l$index0[3], *$index++]
                        byte_align_be[*l$index0[4], *$index++]
                        byte_align_be[*l$index0[5], *$index++]
                        byte_align_be[*l$index0[6], *$index++]
                        byte_align_be[*l$index0[7], *$index++]
                        byte_align_be[           Z, *$index  ]
                    }
                }
                else
                {
                    //ToDo: lm_off must be 2 but not checked
                    __asm
                    {
                        byte_align_be[          --, Z        ]
                        byte_align_be[*l$index0[2], *$index++]
                        byte_align_be[*l$index0[3], *$index++]
                        byte_align_be[*l$index0[4], *$index++]
                        byte_align_be[*l$index0[5], *$index++]
                        byte_align_be[*l$index0[6], *$index++]
                        byte_align_be[*l$index0[7], *$index  ]
                    }

                    local_csr_write(local_csr_active_lm_addr_0, (uint32_t)((uint32_t *)dst + 8));

                    __asm
                    {
                        byte_align_be[          --, *$index++]
                        byte_align_be[*l$index0[0], *$index++]
                        byte_align_be[           Z, *$index  ]
                    }
                }
                break;

            case 8:
                if (lm_off == 0)
                {
                    __asm
                    {
                        byte_align_be[          --, Z        ]
                        byte_align_be[*l$index0[0], *$index++]
                        byte_align_be[*l$index0[1], *$index++]
                        byte_align_be[*l$index0[2], *$index++]
                        byte_align_be[*l$index0[3], *$index++]
                        byte_align_be[*l$index0[4], *$index++]
                        byte_align_be[*l$index0[5], *$index++]
                        byte_align_be[*l$index0[6], *$index++]
                        byte_align_be[*l$index0[7], *$index++]
                        byte_align_be[           Z, *$index  ]
                    }
                }
                else if (lm_off == 1)
                {
                    __asm
                    {
                        byte_align_be[          --, Z        ]
                        byte_align_be[*l$index0[1], *$index++]
                        byte_align_be[*l$index0[2], *$index++]
                        byte_align_be[*l$index0[3], *$index++]
                        byte_align_be[*l$index0[4], *$index++]
                        byte_align_be[*l$index0[5], *$index++]
                        byte_align_be[*l$index0[6], *$index++]
                        byte_align_be[*l$index0[7], *$index  ]
                    }

                    local_csr_write(local_csr_active_lm_addr_0, (uint32_t)((uint32_t *)dst + 8));

                    __asm
                    {
                        byte_align_be[          --, *$index++]
                        byte_align_be[*l$index0[0], *$index++]
                        byte_align_be[           Z, *$index  ]
                    }
                }
                else
                {
                    //ToDo: lm_off must be 2 but not checked
                    __asm
                    {
                        byte_align_be[          --, Z        ]
                        byte_align_be[*l$index0[2], *$index++]
                        byte_align_be[*l$index0[3], *$index++]
                        byte_align_be[*l$index0[4], *$index++]
                        byte_align_be[*l$index0[5], *$index++]
                        byte_align_be[*l$index0[6], *$index++]
                        byte_align_be[*l$index0[7], *$index  ]
                    }

                    local_csr_write(local_csr_active_lm_addr_0, (uint32_t)((uint32_t *)dst + 8));

                    __asm
                    {
                        byte_align_be[          --, *$index++]
                        byte_align_be[*l$index0[0], *$index++]
                        byte_align_be[*l$index0[1], *$index++]
                        byte_align_be[           Z, *$index  ]
                    }
                }
                break;

            case 9:
                if (lm_off == 0)
                {
                    __asm
                    {
                        byte_align_be[          --, Z        ]
                        byte_align_be[*l$index0[0], *$index++]
                        byte_align_be[*l$index0[1], *$index++]
                        byte_align_be[*l$index0[2], *$index++]
                        byte_align_be[*l$index0[3], *$index++]
                        byte_align_be[*l$index0[4], *$index++]
                        byte_align_be[*l$index0[5], *$index++]
                        byte_align_be[*l$index0[6], *$index++]
                        byte_align_be[*l$index0[7], *$index  ]
                    }

                    local_csr_write(local_csr_active_lm_addr_0, (uint32_t)((uint32_t *)dst + 8));

                    __asm
                    {
                        byte_align_be[          --, *$index++]
                        byte_align_be[*l$index0[0], *$index++]
                        byte_align_be[           Z, *$index  ]
                    }
                }
                else if (lm_off == 1)
                {
                    __asm
                    {
                        byte_align_be[          --, Z        ]
                        byte_align_be[*l$index0[1], *$index++]
                        byte_align_be[*l$index0[2], *$index++]
                        byte_align_be[*l$index0[3], *$index++]
                        byte_align_be[*l$index0[4], *$index++]
                        byte_align_be[*l$index0[5], *$index++]
                        byte_align_be[*l$index0[6], *$index++]
                        byte_align_be[*l$index0[7], *$index  ]
                    }

                    local_csr_write(local_csr_active_lm_addr_0, (uint32_t)((uint32_t *)dst + 8));

                    __asm
                    {
                        byte_align_be[          --, *$index++]
                        byte_align_be[*l$index0[0], *$index++]
                        byte_align_be[*l$index0[1], *$index++]
                        byte_align_be[           Z, *$index  ]
                    }
                }
                else
                {
                    //ToDo: lm_off must be 2 but not checked
                    __asm
                    {
                        byte_align_be[          --, Z        ]
                        byte_align_be[*l$index0[2], *$index++]
                        byte_align_be[*l$index0[3], *$index++]
                        byte_align_be[*l$index0[4], *$index++]
                        byte_align_be[*l$index0[5], *$index++]
                        byte_align_be[*l$index0[6], *$index++]
                        byte_align_be[*l$index0[7], *$index  ]
                    }

                    local_csr_write(local_csr_active_lm_addr_0, (uint32_t)((uint32_t *)dst + 8));

                    __asm
                    {
                        byte_align_be[          --, *$index++]
                        byte_align_be[*l$index0[0], *$index++]
                        byte_align_be[*l$index0[1], *$index++]
                        byte_align_be[*l$index0[2], *$index++]
                        byte_align_be[           Z, *$index  ]
                    }
                }
                break;

            case 10:
                if (lm_off == 0)
                {
                    __asm
                    {
                        byte_align_be[          --, Z        ]
                        byte_align_be[*l$index0[0], *$index++]
                        byte_align_be[*l$index0[1], *$index++]
                        byte_align_be[*l$index0[2], *$index++]
                        byte_align_be[*l$index0[3], *$index++]
                        byte_align_be[*l$index0[4], *$index++]
                        byte_align_be[*l$index0[5], *$index++]
                        byte_align_be[*l$index0[6], *$index++]
                        byte_align_be[*l$index0[7], *$index  ]
                    }

                    local_csr_write(local_csr_active_lm_addr_0, (uint32_t)((uint32_t *)dst + 8));

                    __asm
                    {
                        byte_align_be[          --, *$index++]
                        byte_align_be[*l$index0[0], *$index++]
                        byte_align_be[*l$index0[1], *$index++]
                        byte_align_be[           Z, *$index  ]
                    }
                }
                else if (lm_off == 1)
                {
                    __asm
                    {
                        byte_align_be[          --, Z        ]
                        byte_align_be[*l$index0[1], *$index++]
                        byte_align_be[*l$index0[2], *$index++]
                        byte_align_be[*l$index0[3], *$index++]
                        byte_align_be[*l$index0[4], *$index++]
                        byte_align_be[*l$index0[5], *$index++]
                        byte_align_be[*l$index0[6], *$index++]
                        byte_align_be[*l$index0[7], *$index  ]
                    }

                    local_csr_write(local_csr_active_lm_addr_0, (uint32_t)((uint32_t *)dst + 8));

                    __asm
                    {
                        byte_align_be[          --, *$index++]
                        byte_align_be[*l$index0[0], *$index++]
                        byte_align_be[*l$index0[1], *$index++]
                        byte_align_be[*l$index0[2], *$index++]
                        byte_align_be[           Z, *$index  ]
                    }
                }
                else
                {
                    //ToDo: lm_off must be 2 but not checked
                    __asm
                    {
                        byte_align_be[          --, Z        ]
                        byte_align_be[*l$index0[2], *$index++]
                        byte_align_be[*l$index0[3], *$index++]
                        byte_align_be[*l$index0[4], *$index++]
                        byte_align_be[*l$index0[5], *$index++]
                        byte_align_be[*l$index0[6], *$index++]
                        byte_align_be[*l$index0[7], *$index  ]
                    }

                    local_csr_write(local_csr_active_lm_addr_0, (uint32_t)((uint32_t *)dst + 8));

                    __asm
                    {
                        byte_align_be[          --, *$index++]
                        byte_align_be[*l$index0[0], *$index++]
                        byte_align_be[*l$index0[1], *$index++]
                        byte_align_be[*l$index0[2], *$index++]
                        byte_align_be[*l$index0[3], *$index++]
                        byte_align_be[           Z, *$index  ]
                    }
                }
                break;

            case 11:
                if (lm_off == 0)
                {
                    __asm
                    {
                        byte_align_be[          --, Z        ]
                        byte_align_be[*l$index0[0], *$index++]
                        byte_align_be[*l$index0[1], *$index++]
                        byte_align_be[*l$index0[2], *$index++]
                        byte_align_be[*l$index0[3], *$index++]
                        byte_align_be[*l$index0[4], *$index++]
                        byte_align_be[*l$index0[5], *$index++]
                        byte_align_be[*l$index0[6], *$index++]
                        byte_align_be[*l$index0[7], *$index  ]
                    }

                    local_csr_write(local_csr_active_lm_addr_0, (uint32_t)((uint32_t *)dst + 8));

                    __asm
                    {
                        byte_align_be[          --, *$index++]
                        byte_align_be[*l$index0[0], *$index++]
                        byte_align_be[*l$index0[1], *$index++]
                        byte_align_be[*l$index0[2], *$index++]
                        byte_align_be[           Z, *$index  ]
                    }
                }
                else if (lm_off == 1)
                {
                    __asm
                    {
                        byte_align_be[          --, Z        ]
                        byte_align_be[*l$index0[1], *$index++]
                        byte_align_be[*l$index0[2], *$index++]
                        byte_align_be[*l$index0[3], *$index++]
                        byte_align_be[*l$index0[4], *$index++]
                        byte_align_be[*l$index0[5], *$index++]
                        byte_align_be[*l$index0[6], *$index++]
                        byte_align_be[*l$index0[7], *$index  ]
                    }

                    local_csr_write(local_csr_active_lm_addr_0, (uint32_t)((uint32_t *)dst + 8));

                    __asm
                    {
                        byte_align_be[          --, *$index++]
                        byte_align_be[*l$index0[0], *$index++]
                        byte_align_be[*l$index0[1], *$index++]
                        byte_align_be[*l$index0[2], *$index++]
                        byte_align_be[*l$index0[3], *$index++]
                        byte_align_be[           Z, *$index  ]
                    }
                }
                else
                {
                    //ToDo: lm_off must be 2 but not checked
                    __asm
                    {
                        byte_align_be[          --, Z        ]
                        byte_align_be[*l$index0[2], *$index++]
                        byte_align_be[*l$index0[3], *$index++]
                        byte_align_be[*l$index0[4], *$index++]
                        byte_align_be[*l$index0[5], *$index++]
                        byte_align_be[*l$index0[6], *$index++]
                        byte_align_be[*l$index0[7], *$index  ]
                    }

                    local_csr_write(local_csr_active_lm_addr_0, (uint32_t)((uint32_t *)dst + 8));

                    __asm
                    {
                        byte_align_be[          --, *$index++]
                        byte_align_be[*l$index0[0], *$index++]
                        byte_align_be[*l$index0[1], *$index++]
                        byte_align_be[*l$index0[2], *$index++]
                        byte_align_be[*l$index0[3], *$index++]
                        byte_align_be[*l$index0[4], *$index++]
                        byte_align_be[           Z, *$index  ]
                    }
                }
                break;

            case 12:
                if (lm_off == 0)
                {
                    __asm
                    {
                        byte_align_be[          --, Z        ]
                        byte_align_be[*l$index0[0], *$index++]
                        byte_align_be[*l$index0[1], *$index++]
                        byte_align_be[*l$index0[2], *$index++]
                        byte_align_be[*l$index0[3], *$index++]
                        byte_align_be[*l$index0[4], *$index++]
                        byte_align_be[*l$index0[5], *$index++]
                        byte_align_be[*l$index0[6], *$index++]
                        byte_align_be[*l$index0[7], *$index  ]
                    }

                    local_csr_write(local_csr_active_lm_addr_0, (uint32_t)((uint32_t *)dst + 8));

                    __asm
                    {
                        byte_align_be[          --, *$index++]
                        byte_align_be[*l$index0[0], *$index++]
                        byte_align_be[*l$index0[1], *$index++]
                        byte_align_be[*l$index0[2], *$index++]
                        byte_align_be[*l$index0[3], *$index++]
                        byte_align_be[           Z, *$index  ]
                    }
                }
                else if (lm_off == 1)
                {
                    __asm
                    {
                        byte_align_be[          --, Z        ]
                        byte_align_be[*l$index0[1], *$index++]
                        byte_align_be[*l$index0[2], *$index++]
                        byte_align_be[*l$index0[3], *$index++]
                        byte_align_be[*l$index0[4], *$index++]
                        byte_align_be[*l$index0[5], *$index++]
                        byte_align_be[*l$index0[6], *$index++]
                        byte_align_be[*l$index0[7], *$index  ]
                    }

                    local_csr_write(local_csr_active_lm_addr_0, (uint32_t)((uint32_t *)dst + 8));

                    __asm
                    {
                        byte_align_be[          --, *$index++]
                        byte_align_be[*l$index0[0], *$index++]
                        byte_align_be[*l$index0[1], *$index++]
                        byte_align_be[*l$index0[2], *$index++]
                        byte_align_be[*l$index0[3], *$index++]
                        byte_align_be[*l$index0[4], *$index++]
                        byte_align_be[           Z, *$index  ]
                    }
                }
                else
                {
                    //ToDo: lm_off must be 2 but not checked
                    __asm
                    {
                        byte_align_be[          --, Z        ]
                        byte_align_be[*l$index0[2], *$index++]
                        byte_align_be[*l$index0[3], *$index++]
                        byte_align_be[*l$index0[4], *$index++]
                        byte_align_be[*l$index0[5], *$index++]
                        byte_align_be[*l$index0[6], *$index++]
                        byte_align_be[*l$index0[7], *$index  ]
                    }

                    local_csr_write(local_csr_active_lm_addr_0, (uint32_t)((uint32_t *)dst + 8));

                    __asm
                    {
                        byte_align_be[          --, *$index++]
                        byte_align_be[*l$index0[0], *$index++]
                        byte_align_be[*l$index0[1], *$index++]
                        byte_align_be[*l$index0[2], *$index++]
                        byte_align_be[*l$index0[3], *$index++]
                        byte_align_be[*l$index0[4], *$index++]
                        byte_align_be[*l$index0[5], *$index++]
                        byte_align_be[           Z, *$index  ]
                    }
                }
                break;

            case 13:
                if (lm_off == 0)
                {
                    __asm
                    {
                        byte_align_be[          --, Z        ]
                        byte_align_be[*l$index0[0], *$index++]
                        byte_align_be[*l$index0[1], *$index++]
                        byte_align_be[*l$index0[2], *$index++]
                        byte_align_be[*l$index0[3], *$index++]
                        byte_align_be[*l$index0[4], *$index++]
                        byte_align_be[*l$index0[5], *$index++]
                        byte_align_be[*l$index0[6], *$index++]
                        byte_align_be[*l$index0[7], *$index  ]
                    }

                    local_csr_write(local_csr_active_lm_addr_0, (uint32_t)((uint32_t *)dst + 8));

                    __asm
                    {
                        byte_align_be[          --, *$index++]
                        byte_align_be[*l$index0[0], *$index++]
                        byte_align_be[*l$index0[1], *$index++]
                        byte_align_be[*l$index0[2], *$index++]
                        byte_align_be[*l$index0[3], *$index++]
                        byte_align_be[*l$index0[4], *$index++]
                        byte_align_be[           Z, *$index  ]
                    }
                }
                else if (lm_off == 1)
                {
                    __asm
                    {
                        byte_align_be[          --, Z        ]
                        byte_align_be[*l$index0[1], *$index++]
                        byte_align_be[*l$index0[2], *$index++]
                        byte_align_be[*l$index0[3], *$index++]
                        byte_align_be[*l$index0[4], *$index++]
                        byte_align_be[*l$index0[5], *$index++]
                        byte_align_be[*l$index0[6], *$index++]
                        byte_align_be[*l$index0[7], *$index  ]
                    }

                    local_csr_write(local_csr_active_lm_addr_0, (uint32_t)((uint32_t *)dst + 8));

                    __asm
                    {
                        byte_align_be[          --, *$index++]
                        byte_align_be[*l$index0[0], *$index++]
                        byte_align_be[*l$index0[1], *$index++]
                        byte_align_be[*l$index0[2], *$index++]
                        byte_align_be[*l$index0[3], *$index++]
                        byte_align_be[*l$index0[4], *$index++]
                        byte_align_be[*l$index0[5], *$index++]
                        byte_align_be[           Z, *$index  ]
                    }
                }
                else
                {
                    //ToDo: lm_off must be 2 but not checked
                    __asm
                    {
                        byte_align_be[          --, Z        ]
                        byte_align_be[*l$index0[2], *$index++]
                        byte_align_be[*l$index0[3], *$index++]
                        byte_align_be[*l$index0[4], *$index++]
                        byte_align_be[*l$index0[5], *$index++]
                        byte_align_be[*l$index0[6], *$index++]
                        byte_align_be[*l$index0[7], *$index  ]
                    }

                    local_csr_write(local_csr_active_lm_addr_0, (uint32_t)((uint32_t *)dst + 8));

                    __asm
                    {
                        byte_align_be[          --, *$index++]
                        byte_align_be[*l$index0[0], *$index++]
                        byte_align_be[*l$index0[1], *$index++]
                        byte_align_be[*l$index0[2], *$index++]
                        byte_align_be[*l$index0[3], *$index++]
                        byte_align_be[*l$index0[4], *$index++]
                        byte_align_be[*l$index0[5], *$index++]
                        byte_align_be[*l$index0[6], *$index++]
                        byte_align_be[           Z, *$index  ]
                    }
                }
                break;

            case 14:
                if (lm_off == 0)
                {
                    __asm
                    {
                        byte_align_be[          --, Z        ]
                        byte_align_be[*l$index0[0], *$index++]
                        byte_align_be[*l$index0[1], *$index++]
                        byte_align_be[*l$index0[2], *$index++]
                        byte_align_be[*l$index0[3], *$index++]
                        byte_align_be[*l$index0[4], *$index++]
                        byte_align_be[*l$index0[5], *$index++]
                        byte_align_be[*l$index0[6], *$index++]
                        byte_align_be[*l$index0[7], *$index  ]
                    }

                    local_csr_write(local_csr_active_lm_addr_0, (uint32_t)((uint32_t *)dst + 8));

                    __asm
                    {
                        byte_align_be[          --, *$index++]
                        byte_align_be[*l$index0[0], *$index++]
                        byte_align_be[*l$index0[1], *$index++]
                        byte_align_be[*l$index0[2], *$index++]
                        byte_align_be[*l$index0[3], *$index++]
                        byte_align_be[*l$index0[4], *$index++]
                        byte_align_be[*l$index0[5], *$index++]
                        byte_align_be[           Z, *$index  ]
                    }
                }
                else if (lm_off == 1)
                {
                    __asm
                    {
                        byte_align_be[          --, Z        ]
                        byte_align_be[*l$index0[1], *$index++]
                        byte_align_be[*l$index0[2], *$index++]
                        byte_align_be[*l$index0[3], *$index++]
                        byte_align_be[*l$index0[4], *$index++]
                        byte_align_be[*l$index0[5], *$index++]
                        byte_align_be[*l$index0[6], *$index++]
                        byte_align_be[*l$index0[7], *$index  ]
                    }

                    local_csr_write(local_csr_active_lm_addr_0, (uint32_t)((uint32_t *)dst + 8));

                    __asm
                    {
                        byte_align_be[          --, *$index++]
                        byte_align_be[*l$index0[0], *$index++]
                        byte_align_be[*l$index0[1], *$index++]
                        byte_align_be[*l$index0[2], *$index++]
                        byte_align_be[*l$index0[3], *$index++]
                        byte_align_be[*l$index0[4], *$index++]
                        byte_align_be[*l$index0[5], *$index++]
                        byte_align_be[*l$index0[6], *$index++]
                        byte_align_be[           Z, *$index  ]
                    }
                }
                else
                {
                    //ToDo: lm_off must be 2 but not checked
                    __asm
                    {
                        byte_align_be[          --, Z        ]
                        byte_align_be[*l$index0[2], *$index++]
                        byte_align_be[*l$index0[3], *$index++]
                        byte_align_be[*l$index0[4], *$index++]
                        byte_align_be[*l$index0[5], *$index++]
                        byte_align_be[*l$index0[6], *$index++]
                        byte_align_be[*l$index0[7], *$index  ]
                    }

                    local_csr_write(local_csr_active_lm_addr_0, (uint32_t)((uint32_t *)dst + 8));

                    __asm
                    {
                        byte_align_be[          --, *$index++]
                        byte_align_be[*l$index0[0], *$index++]
                        byte_align_be[*l$index0[1], *$index++]
                        byte_align_be[*l$index0[2], *$index++]
                        byte_align_be[*l$index0[3], *$index++]
                        byte_align_be[*l$index0[4], *$index++]
                        byte_align_be[*l$index0[5], *$index++]
                        byte_align_be[*l$index0[6], *$index++]
                        byte_align_be[*l$index0[7], *$index++]
                        byte_align_be[           Z, *$index  ]
                    }

                }
                break;

            case 15:
                if (lm_off == 0)
                {
                    __asm
                    {
                        byte_align_be[          --, Z        ]
                        byte_align_be[*l$index0[0], *$index++]
                        byte_align_be[*l$index0[1], *$index++]
                        byte_align_be[*l$index0[2], *$index++]
                        byte_align_be[*l$index0[3], *$index++]
                        byte_align_be[*l$index0[4], *$index++]
                        byte_align_be[*l$index0[5], *$index++]
                        byte_align_be[*l$index0[6], *$index++]
                        byte_align_be[*l$index0[7], *$index  ]
                    }

                    local_csr_write(local_csr_active_lm_addr_0, (uint32_t)((uint32_t *)dst + 8));

                    __asm
                    {
                        byte_align_be[          --, *$index++]
                        byte_align_be[*l$index0[0], *$index++]
                        byte_align_be[*l$index0[1], *$index++]
                        byte_align_be[*l$index0[2], *$index++]
                        byte_align_be[*l$index0[3], *$index++]
                        byte_align_be[*l$index0[4], *$index++]
                        byte_align_be[*l$index0[5], *$index++]
                        byte_align_be[*l$index0[6], *$index++]
                        byte_align_be[           Z, *$index  ]
                    }
                }
                else if (lm_off == 1)
                {
                    __asm
                    {
                        byte_align_be[          --, Z        ]
                        byte_align_be[*l$index0[1], *$index++]
                        byte_align_be[*l$index0[2], *$index++]
                        byte_align_be[*l$index0[3], *$index++]
                        byte_align_be[*l$index0[4], *$index++]
                        byte_align_be[*l$index0[5], *$index++]
                        byte_align_be[*l$index0[6], *$index++]
                        byte_align_be[*l$index0[7], *$index  ]
                    }

                    local_csr_write(local_csr_active_lm_addr_0, (uint32_t)((uint32_t *)dst + 8));

                    __asm
                    {
                        byte_align_be[          --, *$index++]
                        byte_align_be[*l$index0[0], *$index++]
                        byte_align_be[*l$index0[1], *$index++]
                        byte_align_be[*l$index0[2], *$index++]
                        byte_align_be[*l$index0[3], *$index++]
                        byte_align_be[*l$index0[4], *$index++]
                        byte_align_be[*l$index0[5], *$index++]
                        byte_align_be[*l$index0[6], *$index++]
                        byte_align_be[*l$index0[7], *$index++]
                        byte_align_be[           Z, *$index  ]
                    }
                }
                else
                {
                    //ToDo: lm_off must be 2 but not checked
                    //ToDo: dst_off + length too large but not checked
                }
                break;
        } // switch(length >> 2)

        local_csr_write(local_csr_active_lm_addr_0, (uint32_t)((uint32_t *)dst + lm_off + (length >> 2)));

        switch (length & 3)
        {
            case 1:
                __asm ld_field_w_clr[*l$index0, 1000b, Z, >>0]
                break;

            case 2:
                __asm ld_field_w_clr[*l$index0, 1100b, Z, >>0]
                break;

            case 3:
                __asm ld_field_w_clr[*l$index0, 1110b, Z, >>0]
                break;
        }

    } // if (length < 4)
}


#endif /* __LIBC_C__ */
