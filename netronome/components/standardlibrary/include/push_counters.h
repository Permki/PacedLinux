/*
 * Copyright (C) 2010 Netronome Systems, Inc.  All rights reserved.
 */

#ifndef __PUSH_COUNTERS_H__
#define __PUSH_COUNTERS_H__

/*
 * To use the push counters you must define 3 values plus 2 more per counter
 * for a given counter set:
 *
 * <setname>_PCTR_G_BASE The base address of the counters in "global" memory
 * <setname>_PCTR_L_BASE The base address of the counters in "local" memory
 * <setname>_PCTR_NUM32  The number of 32-bit counter slots to reserve
 * <setname>_PCTR_NUM64  The number of 64-bit counter slots to reserve
 * <setname>_PCSD        A descriptor to pass to the pctr_* microcode macros
 *
 * The PCSD format is:
 *  <global mem type>, <name>_PCTR_G_BASE, <local mem type>, \
 *  <name>_PCTR_L_BASE, <setname>_PCTR_NUM32, <setname>_PCTR_NUM64
 *
 * Typical use:
 *
 * #define FOO_PCTR_G_BASE      0x100   ; must BE DEFINED to get the address
 * #define FOO_PCTR_L_BASE      0x200   ; must BE DEFINED if caching is enabled
 * #define FOO_PCTR_NUM32       2       ; must BE DEFINED to get the address
 * #define FOO_PCTR_NUM64       2       ; must BE DEFINED to get the address
 * #define FOO_PCSD             _FOO, dram, FOO_PCTR_G_BASE, sram, \
 *                              FOO_PCTR_L_BASE, FOO_PCTR_NUM32, FOO_PCTR_NUM64
 * ; PCSD must BE DEFINED to generate counter symbols
 *
 * ; Here we declare counters themselves
 *
 * #define FOO_IS_BAD_CTR       PUSH_COUNTER(FOO, 0)
 * #define FOO_IS_BAD_SCRADDR   PUSH_COUNTER_ADDR(FOO, 0)
 *
 * #define FOO_HAS_BAR_CTR      PUSH_COUNTER(FOO, 1)
 * #define FOO_HAS_BAR_SCRADDR  PUSH_COUNTER_ADDR(FOO, 1)
 *
 * #define FOO_SOME64_CTR       PUSH_COUNTER64(FOO, 0)
 * #define FOO_SOME64_SCRADDR   PUSH_COUNTER64(FOO, 0)
 *
 *
 * To turn off caching one would do:
 *
 * #define FOO_PCSD     _FOO, NONE, NONE, sram, FOO_PCTR_G_BASE, \
 *                      FOO_PCTR_NUM32, FOO_PCTR_NUM64
 *
 * Note that in this case, the FOO_PCTR_L_BASE is unnecessary.  The token NONE
 * for the global memory type _is_ necessary.
 *
 */

#define PUSH_COUNTER(SETNAME, IDX)      SETNAME##_PCSD, IDX, 0
#define PUSH_COUNTER_ADDR(SETNAME, IDX) (SETNAME##_PCTR_G_BASE + (IDX * 4))

#define PUSH_COUNTER64(SETNAME, IDX)    SETNAME##_PCSD, IDX, 1
#define PUSH_COUNTER64_ADDR(SETNAME, IDX)       \
    (SETNAME##_PCTR_G_BASE + (((IDX * 2) + SETNAME##_PCTR_NUM32) * 4))


#define PUSH_COUNTERS_LEN(SETNAME) \
    ((SETNAME##_PCTR_NUM32 + SETNAME##_PCTR_NUM64 * 2) * 4)

#endif /* __PUSH_COUNTERS_H__ */
