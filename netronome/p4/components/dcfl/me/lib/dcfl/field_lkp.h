/*
 * Copyright (C) 2014,  Netronome Systems, Inc.  All rights reserved.
 *
 * @file          lib/dcfl/field_lkp.h
 * @brief         Field lookups.
 *
 */
#ifndef _DCFL__FIELD_LKP_H_
#define _DCFL__FIELD_LKP_H_

#if defined(__NFP_LANG_MICROC)
#include <stdint.h>
#include <types.h>
#endif

#define DCFL_DIRECT_FIELD_TYPE      0
#define DCFL_RANGE_FIELD_TYPE       1
#define DCFL_APM_IPv4_FIELD_TYPE    2
#define DCFL_APM_IPv6_FIELD_TYPE    3
#define DCFL_VALID_FIELD_TYPE       4

#if defined(__NFP_LANG_MICROC)
/**
 * Perform a field lookup.
 *
 * @labels             LM label_set where to store the resulting labels
 * @valid_val          Valid value if field is DCFL_VALID_FIELD_TYPE
 * @value              Field value
 * @field_cfg          Field configuration including base.
 * @hdr_valid          Flag indicating whether header is valid, for P4 strict
 *                     header valid checking
 *
 * @return             Number of labels hit with @value
 *
 */
int dcfl_field_lookup(__lmem struct dcfl_label_set *labels,
                      __gpr unsigned long valid_val,
                      __xrw uint32_t *value,
                      __xread struct dcfl_me_field_config *field_cfg,
                      __gpr unsigned int hdr_valid);

/**
 * Get value of field from extracted key, and copy into write transfer
 * register, right shifted word-wise, ie a 32 bit value should populate
 * value[3], while a 128 bit value should populate value[0..3]. This is
 * determined by the NFP6000 lookup engine, as well as the order of words
 * in the header extract.
 *
 * @param valid_val Single word value to use for valid matching
 * @param value     Lookup key copied into write transfer register
 * @param localkey  Header extracted into local memory
 * @param f_cfg     Field config structure in read transfer register
 */
void
dcfl_get_field_value(__gpr uint32_t* valid_val,
                     __xrw uint32_t *value,
                     __lmem uint32_t *localkey,
                     __xread struct dcfl_me_field_config *f_cfg);

/**
 * Get value of field from extracted key, and copy into write transfer
 * register, right shifted word-wise, ie a 32 bit value should populate
 * value[3], while a 128 bit value should populate value[0..3]. This is
 * determined by the NFP6000 lookup engine, as well as the order of words
 * in the header extract.
 *
 * @param valid_val Single word value to use for valid matching
 * @param value     Lookup key copied into write transfer register
 * @param key_addr  Address in memory to extracted header
 * @param f_cfg     Field config structure in read transfer register
 */
void
dcfl_get_field_value_from_mem(__gpr uint32_t* valid_val,
                     __xrw uint32_t *value,
                     __addr40 __mem uint8_t *key,
                     __xread struct dcfl_me_field_config *f_cfg);
#else

#define DCFL_EMEM   0
#define DCFL_IMEM   1

/*
 * Large direct lookup table (64K to 8M entries) address:
 * Bit 31-> 1 == don't care/recursive lookup
 * Bit 30-> 1 == DLUT
 * Bits [29:15] -> (Address >> 18) & 0x7FFF
 * Bit 12-> 1 == result size of 32 bits
 * Bits [10:8] -> log2(size / 64K) (size in number of entries in table)
 * Bit 7-> 1 == use large tables
 * Bits [6:0] -> 32 - log2(size) == start bit in address lookup
 */
#define DCFL_LARGE_DLUT_ADDR_LO(BASE, STARTBIT)   \
    ((0x1 << 30)                            | \
    (((BASE >> 18) & 0x7FFF) << 15)         | \
    (1 << 12)                               | \
    (1 << 7)                                | \
    (STARTBIT))

#endif /* defined(__NFP_LANG_MICROC) */

#endif /* !_DCFL__FIELD_LKP_H_ */
