/* Copyright (C) 2016,  Netronome Systems, Inc.  All rights reserved. */

#include <nfp/me.h>
#include <nfp/mem_bulk.h>
#include <nfp/cls.h>
#include <pkt/pkt.h>
#include <net/csum.h>
#include "pif_common.h"

#define _PIF_FLCALC_XFR_LW      8
#define _PIF_FLCALC_XFR_BYTES   (_PIF_FLCALC_XFR_LW * 4)

/*
 * csum16
 */

/*
 * @brief Compute the 16-bit sum of the variable field, two bytes at a time.
 *
 * @param in_sum32  Current sum (excluding the payload).
 * @param cls_ptr   Pointer to variable field in CLS.
 * @param cls_len   Length in bytes of variable field.
 * @param odd_byte_offset   Set to 1 if byte offset is odd, 0 if even.
 *
 * @returns Updated 16-bit sum in 32-bit variable.
 */
__gpr uint32_t __pif_flcalc_csum16_var(
    __gpr uint32_t in_sum32,
    __cls uint8_t *cls_ptr,
    __gpr uint32_t cls_len,
    __gpr uint8_t odd_byte_offset
    )
{
    __gpr uint32_t out_sum32 = 0;
    __gpr uint32_t cls_offset = 0;
    __gpr size_t i;
    __gpr uint16_t val16;
    __gpr uint32_t to_rd;
    __gpr uint8_t rem;
    __xread uint8_t xfr_rd[_PIF_FLCALC_XFR_BYTES];
    SIGNAL sig_rd;

    while (cls_len) {
        to_rd = (cls_len > _PIF_FLCALC_XFR_BYTES) ? _PIF_FLCALC_XFR_BYTES : cls_len;
        rem = cls_len % 4; /* Always read a multiple of 4 bytes. */
        __cls_read(xfr_rd, cls_ptr+cls_offset, (to_rd+rem), _PIF_FLCALC_XFR_BYTES,
            ctx_swap, &sig_rd);
        for (i = 0; i < to_rd; i ++) {
            if ((i % 2 ) == odd_byte_offset) /**< @note Don't need to check this for every byte. */
                val16 = (uint16_t)(xfr_rd[i]<<8);
            else
                val16 = xfr_rd[i];
            in_sum32 += val16;
        }
        cls_offset += to_rd;
        cls_len -= to_rd;
    }

    out_sum32 = in_sum32;
    return out_sum32;
}

/*
 * @brief Compute the 16-bit sum of the payload, two bytes at a time.
 *
 * @param in_sum32          Current sum (excluding the payload).
 * @param payload_base      40-bit base address used to access payload.
 * @param payload_offset    Offset from payload_base at which payload starts.
 * @param payld_len         Length in bytes of the payload.
 * @param odd_byte_offset   Set to 1 if byte offset is odd, 0 if even.
 *
 * @returns Updated 16-bit sum in 32-bit variable.
 */
__gpr uint32_t __pif_flcalc_csum16_payload(
    __gpr uint32_t in_sum32,
    __mem __addr40 void *payload_base,
    __gpr uint32_t payload_offset,
    __gpr uint32_t payload_len,
    __gpr uint8_t odd_byte_offset
    )
{
    __gpr uint32_t out_sum32 = 0;
    __gpr size_t i;
    __gpr uint16_t val16;
    __gpr uint32_t to_rd;
    __xread uint8_t xfr_rd[_PIF_FLCALC_XFR_BYTES];
    SIGNAL sig_rd;

    while (payload_len) {
        to_rd = (payload_len > _PIF_FLCALC_XFR_BYTES) ? _PIF_FLCALC_XFR_BYTES : payload_len;
        __mem_read8(xfr_rd, (__mem __addr40 uint8_t *)((uint64_t)payload_base + payload_offset),
            to_rd, _PIF_FLCALC_XFR_BYTES, ctx_swap, &sig_rd);
        for (i = 0; i < to_rd; i ++) {
            if ((i % 2 ) == odd_byte_offset) /**< @note Don't need to check this for every byte. */
                val16 = (uint16_t)(xfr_rd[i]<<8);
            else
                val16 = xfr_rd[i];
            in_sum32 += val16;
        }
        payload_offset += to_rd;
        payload_len -= to_rd;
    }

    out_sum32 = in_sum32;
    return out_sum32;
}

uint16_t pif_flcalc_csum16(
    uint16_t init,
    __lmem uint8_t *lm_ptr,
    __gpr size_t lm_len,
    __cls uint8_t *cls_ptr,
    __gpr uint32_t cls_len,
    __gpr uint32_t payld_offset,
    __gpr uint32_t payld_len,
    __gpr uint8_t odd_byte_offset_bmask
    )
{
    __gpr size_t i;
    __gpr uint16_t csum16;
    __gpr uint16_t val16;
    __gpr uint8_t odd_byte_offset;
    __gpr uint32_t sum32 = init;

    /* Input field list in lmem. */
    odd_byte_offset = odd_byte_offset_bmask & PIF_FLCALC_ODD_LMEM;
    for (i = 0; i < lm_len; i++) {
        if ((i % 2 ) == odd_byte_offset) /**< @note Don't need to check this for every byte. */
            val16 = (__lmem uint16_t)(lm_ptr[i]<<8);
        else
            val16 = lm_ptr[i];
        sum32 += val16;
    }

    /* Variable length field. */
    if (cls_len) {
        odd_byte_offset = odd_byte_offset_bmask & PIF_FLCALC_ODD_CLS;
        sum32 = __pif_flcalc_csum16_var(sum32, cls_ptr, cls_len, odd_byte_offset);
    }

    /* Include payload if required. */
    {
        __gpr uint32_t ctm_payld_len;
        __gpr uint32_t mu_payld_len;
        __mem __addr40 void* payld_base;
        __gpr uint32_t mu_offset;

        if (payld_len)
        {
            odd_byte_offset = odd_byte_offset_bmask & PIF_FLCALC_ODD_MEM;

            if (pif_pkt_info_global.p_nbi.split)
            {
                ctm_payld_len = (256 << pif_pkt_info_global.p_ctm_size) - pif_pkt_info_global.p_offset - payld_offset;
                mu_payld_len = payld_len - ctm_payld_len;
            }
            else
            {
                ctm_payld_len = payld_len;
                mu_payld_len = 0;
            }
            payld_base = (__mem __addr40 void *)pkt_ctm_ptr40(0, pif_pkt_info_global.p_nbi.pnum, pif_pkt_info_global.p_offset);
            sum32 = __pif_flcalc_csum16_payload(sum32, payld_base, payld_offset, ctm_payld_len, odd_byte_offset);
            if (mu_payld_len)
            {
                mu_offset = (256 << pif_pkt_info_global.p_ctm_size);
                payld_base = (__mem __addr40 void *)((uint64_t)pif_pkt_info_global.p_nbi.muptr << 11);
                sum32 = __pif_flcalc_csum16_payload(sum32, payld_base, mu_offset, mu_payld_len, odd_byte_offset);
            }
        }
    }

    /* Fold 32-bit sum into 16-bits. */
    while (sum32>>16) {
        sum32 = (sum32 & 0xffff) + (sum32 >> 16);
    }

    csum16 = (sum32 & 0xffff);

    return csum16;
}

uint16_t pif_flcalc_csum16_lmem(
    uint16_t init,
    __lmem uint32_t *lm_ptr,
    __gpr size_t lm_len
    )
{
    __gpr size_t i;
    __gpr uint16_t csum16;
    __gpr uint32_t rem;
    __gpr uint32_t val;
    __gpr uint32_t sum32 = init;

    for (i = 0; i < (lm_len>>2); i++)
        sum32 = ones_sum_add(sum32, lm_ptr[i]);

    /* Based on ones_sum_warr() in flowenv/me/lib/net/_c/csum.c */
    rem = lm_len & 3;
    if (rem > 0) {
        val = lm_ptr[i];
        if (rem == 1) {
            val = val & 0xFF000000;
        } else if (rem == 2) {
            val = val & 0xFFFF0000;
        } else if (rem == 3) {
            val = val & 0xFFFFFF00;
        }
        sum32 = ones_sum_add(sum32, val);
    }

    csum16 = ones_sum_fold16(sum32);
    return csum16;
}

uint16_t pif_flcalc_csum16_mem(
    uint16_t init,
    __gpr uint32_t payld_offset,
    __gpr uint32_t payld_len
    )
{
    __gpr size_t i;
    __gpr uint16_t csum16;
    __gpr uint32_t rem;
    __gpr uint32_t val;
    __gpr uint32_t sum32 = init;
    __gpr uint32_t ctm_len;
    __gpr uint32_t mu_len;
    __mem __addr40 void* mem_addr;
    __gpr uint32_t mu_offset;
    __mem __addr40 uint8_t *pkt_buf = pkt_ctm_ptr40(0, pif_pkt_info_global.p_nbi.pnum, pif_pkt_info_global.p_offset);


    if (pif_pkt_info_global.p_nbi.split)
    {
        ctm_len = (256 << pif_pkt_info_global.p_ctm_size) - pif_pkt_info_global.p_offset - payld_offset;
        mu_len = payld_len - ctm_len;
    }
    else
    {
        ctm_len = payld_len;
        mu_len = 0;
    }

    /* Compute the sum in CTM then EMEM if necessary.
     * Based on net_csum_l4_ip() in flowenv/me/lib/net/_c/csum.c */
    mem_addr = (__mem __addr40 void *)(pkt_buf + payld_offset);
    sum32 = ones_sum_add(sum32, ones_sum_mem(mem_addr, ctm_len));
    if (mu_len)
    {
        mu_offset = (256 << pif_pkt_info_global.p_ctm_size);
        mem_addr = (__mem __addr40 void *)(((uint64_t)pif_pkt_info_global.p_nbi.muptr << 11) +
                                     mu_offset);
        sum32 = ones_sum_add(sum32, ones_sum_mem(mem_addr, mu_len));
    }

    csum16 = ones_sum_fold16(sum32);
    return csum16;
}

uint16_t pif_flcalc_csum16_update(
    uint16_t init,
    __lmem uint8_t *lm_old,
    __lmem uint8_t *lm_new,
    __gpr size_t lm_len,
    __gpr uint8_t odd_byte_offset,
    __gpr uint8_t byte_mask_first,
    __gpr uint8_t byte_mask_last
    )
{
    __gpr size_t i;
    __gpr uint8_t val8;
    __gpr uint16_t val16;
    __gpr uint16_t csum16;
    __gpr uint32_t sum32 = ~init & 0xffff;

    /* Subtract old value. */
    for (i = 0; i < lm_len; i++) {
        if ((i % 2 ) == odd_byte_offset)
            val16 = (__lmem uint16_t)(lm_old[i]<<8);
        else
            val16 = lm_old[i];
        sum32 += (~val16 & 0xffff);
    }

    /* Add new value. */
    for (i = 0; i < lm_len; i++) {
        if (i == 0) {
            val8 = lm_new[i] & byte_mask_first;
        } else if (i == (lm_len-1)) {
            val8 = lm_new[i] & byte_mask_last;
        } else {
            val8 = lm_new[i];
        }
        if ((i % 2 ) == odd_byte_offset)
            val16 = (uint16_t)(val8<<8);
        else
            val16 = val8;
        sum32 += val16;
    }

    /* Fold 32-bit sum into 16-bits. */
    while (sum32>>16) {
        sum32 = (sum32 & 0xffff) + (sum32 >> 16);
    }

    csum16 = (~sum32 & 0xffff);

    return csum16;
}

/*
 * xor16
 */

 /*
 * @brief Compute the 16-bit XOR of the variable field, two bytes at a time.
 *
 * @param in_xor16  Current sum (excluding the payload).
 * @param cls_ptr   Pointer to variable field in CLS.
 * @param cls_len   Length in bytes of variable field.
 * @param odd_byte_offset   Set to 1 if byte offset is odd, 0 if even.
 *
 * @returns Updated 16-bit XOR.
 */
__gpr uint16_t __pif_flcalc_xor16_var(
    __gpr uint16_t in_xor16,
    __cls uint8_t *cls_ptr,
    __gpr uint32_t cls_len,
    __gpr uint8_t odd_byte_offset
    )
{
    __gpr uint16_t out_xor16 = 0;
    __gpr uint32_t cls_offset = 0;
    __gpr size_t i;
    __gpr uint16_t val16;
    __gpr uint32_t to_rd;
    __gpr uint8_t rem;
    __xread uint8_t xfr_rd[_PIF_FLCALC_XFR_BYTES];
    SIGNAL sig_rd;

    while (cls_len) {
        to_rd = (cls_len > _PIF_FLCALC_XFR_BYTES) ? _PIF_FLCALC_XFR_BYTES : cls_len;
        rem = cls_len % 4; /* Always read a multiple of 4 bytes. */
        __cls_read(xfr_rd, cls_ptr+cls_offset, (to_rd+rem), _PIF_FLCALC_XFR_BYTES,
            ctx_swap, &sig_rd);
        for (i = 0; i < to_rd; i ++) {
            if ((i % 2 ) == odd_byte_offset)
                val16 = (uint16_t)(xfr_rd[i]<<8);
            else
                val16 = xfr_rd[i];
            in_xor16 ^= val16;
        }
        cls_offset += to_rd;
        cls_len -= to_rd;
    }

    out_xor16 = in_xor16;
    return out_xor16;
}

/*
 * @brief Compute the 16-bit exclusive or of the payload, two bytes at a time.
 *
 * @param in_sum32          Current xor (excluding payload).
 * @param payload_base      40-bit base address used to access payload.
 * @param payload_offset    Offset from payload_base at which payload starts.
 * @param payld_len         Length in bytes of the payload.
 * @param odd_byte_offset   Set to 1 if byte offset is odd, 0 if even.
 *
 * @returns Updated 16-bit xor.
 */
__gpr uint16_t __pif_flcalc_xor16_payload(
    __gpr uint16_t in_xor16,
    __mem __addr40 void *payload_base,
    __gpr uint32_t payload_offset,
    __gpr uint32_t payload_len,
    __gpr uint8_t odd_byte_offset
    )
{
    __gpr uint16_t out_xor16 = 0;
    __gpr size_t i;
    __gpr uint16_t val16;
    __gpr uint32_t to_rd;
    __xread uint32_t xfr_rd[_PIF_FLCALC_XFR_BYTES];
    SIGNAL sig_rd;

    while (payload_len) {
        to_rd = (payload_len > _PIF_FLCALC_XFR_BYTES) ? _PIF_FLCALC_XFR_BYTES : payload_len;
        __mem_read8(xfr_rd, (__mem __addr40 uint8_t *)((uint64_t)payload_base + payload_offset),
            to_rd, _PIF_FLCALC_XFR_BYTES, ctx_swap, &sig_rd);
        for (i = 0; i < to_rd; i ++) {
            if ((i % 2 ) == odd_byte_offset)
                val16 = (uint16_t)(xfr_rd[i]<<8);
            else
                val16 = xfr_rd[i];
            in_xor16 ^= val16;
        }
        payload_offset += to_rd;
        payload_len -= to_rd;
    }

    out_xor16 = in_xor16;
    return out_xor16;
}

uint16_t pif_flcalc_xor16(
    uint16_t init,
    __lmem uint8_t *lm_ptr,
    __gpr size_t lm_len,
    __cls uint8_t *cls_ptr,
    __gpr uint32_t cls_len,
    __gpr uint32_t payld_offset,
    __gpr uint32_t payld_len,
    __gpr uint8_t odd_byte_offset_bmask
    )
{
    __gpr size_t i;
    __gpr uint8_t odd_byte_offset;
    __gpr uint16_t val16;
    __gpr uint16_t xor16 = init;

    /* Input field list in lmem. */
    odd_byte_offset = odd_byte_offset_bmask & PIF_FLCALC_ODD_LMEM;
    for (i = 0; i < lm_len; i++) {
        if ((i % 2 ) == odd_byte_offset)
            val16 = (__lmem uint16_t)(lm_ptr[i]<<8);
        else
            val16 = lm_ptr[i];
        xor16 ^= val16;
    }

    /* Include variable length field if required. */
    if (cls_len) {
        odd_byte_offset = odd_byte_offset_bmask & PIF_FLCALC_ODD_CLS;
        xor16 = __pif_flcalc_xor16_var(xor16, cls_ptr, cls_len, odd_byte_offset);
    }

    /* Include payload if required. */
    {
        __gpr uint32_t ctm_payld_len;
        __gpr uint32_t mu_payld_len;
        __mem __addr40 void* payld_base;
        __gpr uint32_t mu_offset;

        if (payld_len)
        {
            odd_byte_offset = odd_byte_offset_bmask & PIF_FLCALC_ODD_MEM;
            if (pif_pkt_info_global.p_nbi.split)
            {
                ctm_payld_len = (256 << pif_pkt_info_global.p_ctm_size) - pif_pkt_info_global.p_offset - payld_offset;
                mu_payld_len = payld_len - ctm_payld_len;
            }
            else
            {
                ctm_payld_len = payld_len;
                mu_payld_len = 0;
            }
            payld_base = pkt_ctm_ptr40(0, pif_pkt_info_global.p_nbi.pnum, pif_pkt_info_global.p_offset);
            xor16 = __pif_flcalc_xor16_payload(xor16, payld_base, payld_offset, ctm_payld_len, odd_byte_offset);
            if (mu_payld_len)
            {
                mu_offset = (256 << pif_pkt_info_global.p_ctm_size);
                payld_base = (__mem __addr40 void *)((uint64_t)pif_pkt_info_global.p_nbi.muptr << 11);
                xor16 = __pif_flcalc_xor16_payload(xor16, payld_base, mu_offset, mu_payld_len, odd_byte_offset);
            }
        }
    }

    return xor16;
}

uint16_t pif_flcalc_xor16_update(
    uint16_t init,
    __lmem uint8_t *lm_old,
    __lmem uint8_t *lm_new,
    __gpr size_t lm_len,
    __gpr uint8_t odd_byte_offset,
    __gpr uint8_t byte_mask_first,
    __gpr uint8_t byte_mask_last
    )
{
    __gpr size_t i;
    __gpr uint8_t val8;
    __gpr uint16_t val16;
    __gpr uint16_t xor16 = init;

    /* "Subtract" old value. */
    for (i = 0; i < lm_len; i++) {
        if ((i % 2 ) == odd_byte_offset)
            val16 = (__lmem uint16_t)(lm_old[i]<<8);
        else
            val16 = lm_old[i];
        xor16 ^= val16;
    }

    /* "Add" new value. */
    for (i = 0; i < lm_len; i++) {
        if (i == 0) {
            val8 = lm_new[i] & byte_mask_first;
        } else if (i == (lm_len-1)) {
            val8 = lm_new[i] & byte_mask_last;
        } else {
            val8 = lm_new[i];
        }
        if ((i % 2 ) == odd_byte_offset)
            val16 = (uint16_t)(val8<<8);
        else
            val16 = val8;
        xor16 ^= val16;
    }

    return xor16;
}

extern __forceinline uint16_t
pif_flcalc_csum16_update_lw(uint16_t init,
                            uint32_t val, uint32_t val_mask,
                            int is_rem)
{
    uint32_t sum32 = (~init) & 0xffff;

    /* NOTE: this negation is intended to be expanded at compile time */
    if (is_rem)
        val = ~val;

    /* NOTE: the valmask ifs are intended to be expanded at compile time */
    if (val_mask & 0xffff0000) {
        sum32 += (val >> 16);
        sum32 = (sum32 & 0xffff) + (sum32 >> 16);
    }

    if (val_mask & 0xffff) {
        sum32 += (val & 0xffff);
        sum32 = (sum32 & 0xffff) + (sum32 >> 16);
    }

    return ((~sum32) & 0xffff);
}

extern __forceinline uint16_t
pif_flcalc_xor16_update_lw(uint16_t init,
                           uint32_t val, uint32_t val_mask,
                           int is_rem)
{
    uint16_t xor16 = init;

    if (val_mask & 0xffff0000)
        xor16 ^= (val >> 16);

    if (val_mask & 0xffff)
        xor16 ^= (val & 0xffff);

    return xor16;
}

/*
 * crc16
 */

/*
 * @brief Update the CRC-16 for one byte of input data.
 *
 * @param in_crc16  Current 16-bit CRC.
 * @param dbyte     Input data.
 *
 * @returns Updated 16-bit CRC.
 *
 * @note Performance may be improved at cost of memory (256*4 bytes) by using a
 *       lookup table of pre-calculated results.
 */
__gpr uint16_t __pif_flcalc_crc16_byte(
    __gpr uint16_t in_crc16,
    uint8_t dbyte
    )
{
    __gpr uint16_t out_crc16;
    __gpr uint32_t ibyte = dbyte;

    local_csr_write(local_csr_crc_remainder, in_crc16);
    __asm crc_be[crc_ccitt, ibyte, ibyte], byte_3
    out_crc16 = local_csr_read(local_csr_crc_remainder);
    return out_crc16;
}

/*
 * @brief Update the CRC-16 from a buffer of transfer registers.
 *
 * @param in_crc16  Current 16-bit CRC.
 * @param xfr_buf   Read transfer registers containing input data.
 * @param xfr_len_b Length in bytes of data in transfer registers.
 *
 * @returns Updated 16-bit CRC.
 *
 * @note Based on ones_sum_warr() in flowenv csum lib.
 */
__gpr uint16_t __pif_flcalc_crc16_xfr_buf(
    __gpr uint16_t in_crc16,
    __xread uint32_t *xfr_buf,
    __gpr uint32_t xfr_len_b
    )
{
    __gpr uint16_t out_crc16;
    __gpr uint32_t t_val;
    __gpr uint32_t xfr_len_lw; /* no. of complete long words of input data. */
    /* remaining bytes of input data (xfr_len_b = 4*xfr_len_lw + rem_b) */
    __gpr uint32_t rem_b;
    __gpr uint32_t jump_idx;

    t_val = ((__ctx() << 5) | __xfer_reg_number(xfr_buf)) << 2;
    local_csr_write(local_csr_t_index, t_val);
    local_csr_write(local_csr_crc_remainder, in_crc16);

    __asm __attribute(ASM_HAS_JUMP)
    {
        alu[xfr_len_lw, --, B, xfr_len_b, >>2]
        alu[jump_idx, 8, -, xfr_len_lw]
        alu[jump_idx, --, B, jump_idx, <<1]
        jump[jump_idx, crc08w], targets[crc08w, crc07w, crc06w, crc05w,\
                                        crc04w, crc03w, crc02w, crc01w,\
                                        crc00w]

        crc08w:
            crc_be[crc_ccitt, xfr_len_lw, *$index++], bytes_0_3
            nop
        crc07w:
            crc_be[crc_ccitt, xfr_len_lw, *$index++], bytes_0_3
            nop
        crc06w:
            crc_be[crc_ccitt, xfr_len_lw, *$index++], bytes_0_3
            nop
        crc05w:
            crc_be[crc_ccitt, xfr_len_lw, *$index++], bytes_0_3
            nop
        crc04w:
            crc_be[crc_ccitt, xfr_len_lw, *$index++], bytes_0_3
            nop
        crc03w:
            crc_be[crc_ccitt, xfr_len_lw, *$index++], bytes_0_3
            nop
        crc02w:
            crc_be[crc_ccitt, xfr_len_lw, *$index++], bytes_0_3
            nop
        crc01w:
            crc_be[crc_ccitt, xfr_len_lw, *$index++], bytes_0_3
            nop
        crc00w:
    }

    /* Deal with remaining bytes of input data. */
    rem_b = xfr_len_b & 3;
    if (rem_b > 0) {
        if (rem_b == 1) {
            __asm crc_be[crc_ccitt, xfr_len_lw, *$index++], byte_0
        } else if (rem_b == 2) {
            __asm crc_be[crc_ccitt, xfr_len_lw, *$index++], bytes_0_1
        } else if (rem_b == 3) {
            __asm crc_be[crc_ccitt, xfr_len_lw, *$index++], bytes_0_2
        }
    }

    out_crc16 = local_csr_read(local_csr_crc_remainder);
    return out_crc16;
}

/*
 * @brief Compute the CRC-16 over the variable field, one byte at a time.
 *
 * @param in_crc16  Current CRC-16 (excluding payload).
 * @param cls_ptr   Pointer to variable field in CLS.
 * @param cls_len   Length in bytes of variable field.
 *
 * @returns Updated 16-bit CRC.
 */
__gpr uint16_t __pif_flcalc_crc16_var(
    __gpr uint16_t in_crc16,
    __cls uint8_t *cls_ptr,
    __gpr uint32_t cls_len
    )
{
    __gpr uint16_t out_crc16;
    __gpr uint32_t cls_offset = 0;
    __gpr size_t i;
    __gpr uint32_t to_rd;
    __xread uint32_t xfr_rd[_PIF_FLCALC_XFR_LW];
    SIGNAL sig_rd;

    while (cls_len) {
        to_rd = (cls_len > _PIF_FLCALC_XFR_BYTES) ? _PIF_FLCALC_XFR_BYTES : cls_len;
        __cls_read(xfr_rd, cls_ptr+cls_offset, to_rd + to_rd&0x3, _PIF_FLCALC_XFR_BYTES,
            ctx_swap, &sig_rd);
        in_crc16 = __pif_flcalc_crc16_xfr_buf(in_crc16, xfr_rd, to_rd);
        /* xfr_rd read using T_INDEX CSR(*$index) so __implicit_read() used here
         * to tell the compiler when xfr_rd's live range ends. */
        __implicit_read(xfr_rd);
        cls_offset += to_rd;
        cls_len -= to_rd;
    }

    out_crc16 = in_crc16;
    return out_crc16;
}

/*
 * @brief Compute the CRC-16 over the payload, one byte at a time.
 *
 * @param in_crc16          Current CRC-16 (excluding payload).
 * @param payload_base      40-bit base address used to access payload.
 * @param payload_offset    Offset from payload_base at which payload starts.
 * @param payld_len         Length in bytes of the payload.
 *
 * @returns Updated 16-bit CRC.
 */
__gpr uint16_t __pif_flcalc_crc16_payload(
    __gpr uint16_t in_crc16,
    __mem __addr40 void *payload_base,
    __gpr uint32_t payload_offset,
    __gpr uint32_t payload_len
    )
{
    __gpr uint16_t out_crc16;
    __gpr size_t i;
    __gpr uint32_t to_rd;
    __xread uint32_t xfr_rd[_PIF_FLCALC_XFR_LW];
    SIGNAL sig_rd;

    while (payload_len) {
        to_rd = (payload_len > _PIF_FLCALC_XFR_BYTES) ? _PIF_FLCALC_XFR_BYTES : payload_len;
        __mem_read8(xfr_rd, (__mem __addr40 uint8_t *)((uint64_t)payload_base + payload_offset),
            to_rd, _PIF_FLCALC_XFR_BYTES, ctx_swap, &sig_rd);
        in_crc16 = __pif_flcalc_crc16_xfr_buf(in_crc16, xfr_rd, to_rd);
        /* xfr_rd read using T_INDEX CSR(*$index) so __implicit_read() used here
         * to tell the compiler when xfr_rd's live range ends. */
        __implicit_read(xfr_rd);
        payload_offset += to_rd;
        payload_len -= to_rd;
    }

    out_crc16 = in_crc16;
    return out_crc16;
}

uint16_t pif_flcalc_crc16(
    uint16_t init,
    __lmem uint8_t *lm_ptr,
    __gpr size_t lm_len,
    __cls uint8_t *cls_ptr,
    __gpr uint32_t cls_len,
    __gpr uint32_t payld_offset,
    __gpr uint32_t payld_len
    )
{
    __gpr uint8_t i;
    __gpr uint8_t ibyte;
    __gpr uint16_t crc16 = init;

    /* Calculate crc16 over the input field list. */
    while (lm_len)
    {
        /** @todo use *lm_ptr++ as function input. */
        ibyte = *lm_ptr++;
        crc16 = __pif_flcalc_crc16_byte(crc16, ibyte);
        lm_len--;
    }

    /* Calculate crc16 over variable length field if required. */
    if (cls_len) {
        crc16 = __pif_flcalc_crc16_var(crc16, cls_ptr, cls_len);
    }

    /* Calculate crc16 over the payload if required. */
    {
        __gpr uint32_t ctm_payld_len;
        __gpr uint32_t mu_payld_len;
        __mem __addr40 void* payld_base;
        __gpr uint32_t mu_offset;

        if (payld_len)
        {
            if (pif_pkt_info_global.p_nbi.split)
            {
                ctm_payld_len = (256 << pif_pkt_info_global.p_ctm_size) - pif_pkt_info_global.p_offset - payld_offset;
                mu_payld_len = payld_len - ctm_payld_len;
            }
            else
            {
                ctm_payld_len = payld_len;
                mu_payld_len = 0;
            }
            payld_base = pkt_ctm_ptr40(0, pif_pkt_info_global.p_nbi.pnum, pif_pkt_info_global.p_offset);
            crc16 = __pif_flcalc_crc16_payload(crc16, payld_base, payld_offset, ctm_payld_len);
            if (mu_payld_len)
            {
                mu_offset = (256 << pif_pkt_info_global.p_ctm_size);
                payld_base = (__mem __addr40 void *)((uint64_t)pif_pkt_info_global.p_nbi.muptr << 11);
                crc16 = __pif_flcalc_crc16_payload(crc16, payld_base, mu_offset, mu_payld_len);
            }
        }
    }

    return crc16;
}

uint16_t pif_flcalc_crc16_lmem(
    uint16_t init,
    __lmem uint32_t *lm_ptr,
    __gpr size_t lm_len_b
    )
{
    __gpr size_t i;
    __gpr uint16_t crc16;
    __gpr uint32_t rem_b;
    __gpr uint32_t val;
    __gpr uint32_t tmp;

    local_csr_write(local_csr_crc_remainder, init);
    for (i = 0; i < (lm_len_b>>2); i++) {
        val = lm_ptr[i];
        __asm crc_be[crc_ccitt, tmp, val], bytes_0_3
    }

    /* Based on ones_sum_warr() in flowenv/me/lib/net/_c/csum.c */
    rem_b = lm_len_b & 3;
    if (rem_b > 0) {
        val = lm_ptr[i];
        if (rem_b == 1) {
            __asm crc_be[crc_ccitt, tmp, val], byte_0
        } else if (rem_b == 2) {
            __asm crc_be[crc_ccitt, tmp, val], bytes_0_1
        } else if (rem_b == 3) {
            __asm crc_be[crc_ccitt, tmp, val], bytes_0_2
        }
    }

    crc16 = local_csr_read(local_csr_crc_remainder);
    return crc16;
}

/*
 * crc32
 */

/*
 * @brief Update the CRC-32 for one byte of input data.
 *
 * @param in_crc16  Current CRC-32.
 * @param dbyte     Input data.
 *
 * @returns Updated 32-bit CRC.
 *
 * @note Performance may be improved at cost of memory (256*4 bytes) by using a
 *       lookup table of pre-calculated results.
 */
__gpr uint32_t __pif_flcalc_crc32_byte(
    __gpr uint32_t in_crc32,
    uint8_t dbyte
    )
{
    __gpr uint32_t out_crc32;
    __gpr uint32_t ibyte = dbyte;

    local_csr_write(local_csr_crc_remainder, in_crc32);
    __asm crc_be[crc_32, ibyte, ibyte], byte_3, bit_swap
    out_crc32 = local_csr_read(local_csr_crc_remainder);

    return out_crc32;
}

/*
 * @brief Update the CRC-32 from a buffer of transfer registers.
 *
 * @param in_crc32  Current 32-bit CRC.
 * @param xfr_buf   Read transfer registers containing input data.
 * @param xfr_len_b Length in bytes of data in transfer registers.
 *
 * @returns Updated 32-bit CRC.
 *
 * @note Based on ones_sum_warr() in flowenv csum lib.
 */
__gpr uint32_t __pif_flcalc_crc32_xfr_buf(
    __gpr uint32_t in_crc32,
    __xread uint32_t *xfr_buf,
    __gpr uint32_t xfr_len_b
    )
{
    __gpr uint32_t out_crc32;
    __gpr uint32_t t_val;
    __gpr uint32_t xfr_len_lw; /* no. of complete long words of input data. */
    /* remaining bytes of input data (xfr_len_b = 4*xfr_len_lw + rem_b) */
    __gpr uint32_t rem_b;
    __gpr uint32_t jump_idx;

    t_val = ((__ctx() << 5) | __xfer_reg_number(xfr_buf)) << 2;
    local_csr_write(local_csr_t_index, t_val);
    local_csr_write(local_csr_crc_remainder, in_crc32);

    __asm __attribute(ASM_HAS_JUMP)
    {
        alu[xfr_len_lw, --, B, xfr_len_b, >>2]
        alu[jump_idx, 8, -, xfr_len_lw]
        alu[jump_idx, --, B, jump_idx, <<1]
        jump[jump_idx, crc08w], targets[crc08w, crc07w, crc06w, crc05w,\
                                        crc04w, crc03w, crc02w, crc01w,\
                                        crc00w]

        crc08w:
            crc_be[crc_32, xfr_len_lw, *$index++], bytes_0_3, bit_swap
            nop
        crc07w:
            crc_be[crc_32, xfr_len_lw, *$index++], bytes_0_3, bit_swap
            nop
        crc06w:
            crc_be[crc_32, xfr_len_lw, *$index++], bytes_0_3, bit_swap
            nop
        crc05w:
            crc_be[crc_32, xfr_len_lw, *$index++], bytes_0_3, bit_swap
            nop
        crc04w:
            crc_be[crc_32, xfr_len_lw, *$index++], bytes_0_3, bit_swap
            nop
        crc03w:
            crc_be[crc_32, xfr_len_lw, *$index++], bytes_0_3, bit_swap
            nop
        crc02w:
            crc_be[crc_32, xfr_len_lw, *$index++], bytes_0_3, bit_swap
            nop
        crc01w:
            crc_be[crc_32, xfr_len_lw, *$index++], bytes_0_3, bit_swap
            nop
        crc00w:
    }

    /* Deal with remaining bytes of input data. */
    rem_b = xfr_len_b & 3;
    if (rem_b > 0) {
        if (rem_b == 1) {
            __asm crc_be[crc_32, xfr_len_lw, *$index++], byte_0, bit_swap
        } else if (rem_b == 2) {
            __asm crc_be[crc_32, xfr_len_lw, *$index++], bytes_0_1, bit_swap
        } else if (rem_b == 3) {
            __asm crc_be[crc_32, xfr_len_lw, *$index++], bytes_0_2, bit_swap
        }
    }

    out_crc32 = local_csr_read(local_csr_crc_remainder);
    return out_crc32;
}

/*
 * @brief Compute the CRC-32 over the variable field, one byte at a time.
 *
 * @param in_crc32  Current CRC-32 (excluding payload).
 * @param cls_ptr   Pointer to variable field in CLS.
 * @param cls_len   Length in bytes of variable field.
 *
 * @returns Updated 32-bit CRC.
 */
__gpr uint32_t __pif_flcalc_crc32_var(
    __gpr uint32_t in_crc32,
    __cls uint8_t *cls_ptr,
    __gpr uint32_t cls_len
    )
{
    __gpr uint32_t out_crc32;
    __gpr uint32_t cls_offset = 0;
    __gpr size_t i;
    __gpr uint32_t to_rd;
    __xread uint32_t xfr_rd[_PIF_FLCALC_XFR_LW];
    SIGNAL sig_rd;

    while (cls_len) {
        to_rd = (cls_len > _PIF_FLCALC_XFR_BYTES) ? _PIF_FLCALC_XFR_BYTES : cls_len;
        __cls_read(xfr_rd, cls_ptr+cls_offset, to_rd + to_rd&0x3, _PIF_FLCALC_XFR_BYTES,
            ctx_swap, &sig_rd);
        in_crc32 = __pif_flcalc_crc32_xfr_buf(in_crc32, xfr_rd, to_rd);
        /* xfr_rd read using T_INDEX CSR(*$index) so __implicit_read() used here
         * to tell the compiler when xfr_rd's live range ends. */
        __implicit_read(xfr_rd);
        cls_offset += to_rd;
        cls_len -= to_rd;
    }

    out_crc32 = in_crc32;
    return out_crc32;
}

/*
 * @brief Compute the CRC-32 over the payload, one byte at a time.
 *
 * @param in_crc32          Current CRC-32 (excluding payload).
 * @param payload_base      40-bit base address used to access payload.
 * @param payload_offset    Offset from payload_base at which payload starts.
 * @param payld_len         Length in bytes of the payload.
 *
 * @returns Updated 32-bit CRC.
 */
__gpr uint32_t __pif_flcalc_crc32_payload(
    __gpr uint32_t in_crc32,
    __mem __addr40 void *payload_base,
    __gpr uint32_t payload_offset,
    __gpr uint32_t payload_len
    )
{
    __gpr uint32_t out_crc32;
    __gpr size_t i;
    __gpr uint32_t to_rd;
    __xread uint32_t xfr_rd[_PIF_FLCALC_XFR_LW];
    SIGNAL sig_rd;

    while (payload_len) {
        to_rd = (payload_len > _PIF_FLCALC_XFR_BYTES) ? _PIF_FLCALC_XFR_BYTES : payload_len;
        __mem_read8(xfr_rd, (__mem __addr40 uint8_t *)((uint64_t)payload_base + payload_offset),
            to_rd, _PIF_FLCALC_XFR_BYTES, ctx_swap, &sig_rd);
        in_crc32 = __pif_flcalc_crc32_xfr_buf(in_crc32, xfr_rd, to_rd);
        /* xfr_rd read using T_INDEX CSR(*$index) so __implicit_read() used here
         * to tell the compiler when xfr_rd's live range ends. */
        __implicit_read(xfr_rd);
        payload_offset += to_rd;
        payload_len -= to_rd;
    }

    out_crc32 = in_crc32;
    return out_crc32;
}

__gpr uint32_t pif_flcalc_crc32_reflect(
    __gpr uint32_t in_crc32
    )
{
    __gpr uint32_t crc32;
    __gpr uint32_t crc32_reflected;
    __gpr uint8_t i;

    local_csr_write(local_csr_crc_remainder, 0);
    __asm crc_le[none, in_crc32, in_crc32], bit_swap
    crc32_reflected = local_csr_read(local_csr_crc_remainder);

    return ~crc32_reflected;
}

uint32_t pif_flcalc_crc32(
    uint32_t init,
    __lmem uint8_t *lm_ptr,
    __gpr size_t lm_len,
    __cls uint8_t *cls_ptr,
    __gpr uint32_t cls_len,
    __gpr uint32_t payld_offset,
    __gpr uint32_t payld_len
    )
{
    __gpr uint8_t i;
    __gpr uint8_t ibyte;
    __gpr uint32_t crc32 = init;

    /* Calculate crc32 over the input field list. */
    while (lm_len)
    {
        /** @todo use *lm_ptr++ as function input. */
        ibyte = *lm_ptr++;
        crc32 = __pif_flcalc_crc32_byte(crc32, ibyte);
        lm_len--;
    }

    /* Calculate crc32 over variable length field if required. */
    if (cls_len) {
        crc32 = __pif_flcalc_crc32_var(crc32, cls_ptr, cls_len);
    }

    /* Calculate crc32 over the payload if required. */
    {
        __gpr uint32_t ctm_payld_len;
        __gpr uint32_t mu_payld_len;
        __mem __addr40 void* payld_base;
        __gpr uint32_t mu_offset;

        if (payld_len)
        {
            if (pif_pkt_info_global.p_nbi.split)
            {
                ctm_payld_len = (256 << pif_pkt_info_global.p_ctm_size) - pif_pkt_info_global.p_offset - payld_offset;
                mu_payld_len = payld_len - ctm_payld_len;
            }
            else
            {
                ctm_payld_len = payld_len;
                mu_payld_len = 0;
            }
            payld_base = pkt_ctm_ptr40(0, pif_pkt_info_global.p_nbi.pnum, pif_pkt_info_global.p_offset);
            crc32 = __pif_flcalc_crc32_payload(crc32, payld_base, payld_offset, ctm_payld_len);
            if (mu_payld_len)
            {
                mu_offset = (256 << pif_pkt_info_global.p_ctm_size);
                payld_base = (__mem __addr40 void *)((uint64_t)pif_pkt_info_global.p_nbi.muptr << 11);
                crc32 = __pif_flcalc_crc32_payload(crc32, payld_base, mu_offset, mu_payld_len);
            }
        }
    }

    return crc32;
}

uint32_t pif_flcalc_crc32_lmem(
    uint32_t init,
    __lmem uint32_t *lm_ptr,
    __gpr size_t lm_len_b
    )
{
    __gpr size_t i;
    __gpr uint32_t crc32;
    __gpr uint32_t rem_b;
    __gpr uint32_t val;
    __gpr uint32_t tmp;

    local_csr_write(local_csr_crc_remainder, init);
    for (i = 0; i < (lm_len_b>>2); i++) {
        val = lm_ptr[i];
        __asm crc_be[crc_32, tmp, val], bytes_0_3, bit_swap
    }

    /* Based on ones_sum_warr() in flowenv/me/lib/net/_c/csum.c */
    rem_b = lm_len_b & 3;
    if (rem_b > 0) {
        val = lm_ptr[i];
        if (rem_b == 1) {
            __asm crc_be[crc_32, tmp, val], byte_0, bit_swap
        } else if (rem_b == 2) {
            __asm crc_be[crc_32, tmp, val], bytes_0_1, bit_swap
        } else if (rem_b == 3) {
            __asm crc_be[crc_32, tmp, val], bytes_0_2, bit_swap
        }
    }

    crc32 = local_csr_read(local_csr_crc_remainder);
    return crc32;
}
