/*
 * Copyright (C) 2014-2017,  Netronome Systems, Inc.  All rights reserved.
 *
 * @file        camp_hash.c
 * @brief       camp hash implementation
 */

#include <nfp/me.h>
#include <nfp/mem_bulk.h>
#include <std/reg_utils.h>

#include <assert.h>
#include <unroll.h>
#include <camp_hash.h>

#define CAMP_HASH_STEP(IDX) do { \
    __asm alu[result.hi, result.hi, +, *l$index3++] \
    __asm crc_be[crc_32, cpy, result.hi] \
    __asm alu[result.lo, result.lo, XOR, result.hi] \
    __asm dbl_shf[result.hi, cpy, result.hi, >>indirect] \
} while (0);

__intrinsic hash_result_t camp_hash(void *data, size_t length, size_t max_length)
{
    hash_result_t result;
    uint32_t cpy, crc, rot;
    uint32_t len = length;

    ctassert(__is_in_lmem(data));
    local_csr_write(local_csr_active_lm_addr_3, (unsigned int) data);

    crc_write(length);
    result.hi = len;
    result.lo = ~len;


    UNROLL(CAMP_HASH_STEP, length, max_length)

    __critical_path();

    result.lo += rotr32(result.lo, result.hi);

    crc = crc_read();
    result.hi += crc;

    result.lo ^= rotr32(result.hi, result.lo);
    result.lo ^= crc;

    return result;
}
