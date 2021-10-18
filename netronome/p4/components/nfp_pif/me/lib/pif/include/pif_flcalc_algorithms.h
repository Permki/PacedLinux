/* Copyright (C) 2016,  Netronome Systems, Inc.  All rights reserved. */

#ifndef __PIF_FLCALC_ALGORITHMS_H__
#define __PIF_FLCALC_ALGORITHMS_H__

/*
 * Static implementations of supported calculated field algorithms.
 */

/**
 * @brief Computes the 16-bit sum of data, taken two bytes at a time.
 *
 * @param init          Initial value of calculated field.
 * @param lm_ptr        Pointer to fields in local memory.
 * @param lm_len        Length in bytes of the input data in local memory.
 * @param cls_ptr       Pointer to variable field in CLS.
 * @param cls_len       Length in bytes of variable field.
 * @param payld_offset  Offset in bytes of the payload from the start of the
 *                      packet. Ignored if payld_len is zero.
 * @param payld_len     Length in bytes of the payload. Set to zero if the
 *                      payload is not part of the field calculation.
 * @param odd_byte_offset_bmask Indicates wheter byte offset of data in lmem,
 *                              CLS or CTM/emem is odd.
 *
 * @returns 16-bit checksum.
 *
 * @note    See https://tools.ietf.org/html/rfc791#page-14 for detailed
 *          description of algorithm.
 */
uint16_t pif_flcalc_csum16(
    uint16_t init,
    __lmem uint8_t *lm_ptr,
    __gpr size_t lm_len,
    __cls uint8_t *cls_ptr,
    __gpr uint32_t cls_len,
    __gpr uint32_t payld_offset,
    __gpr uint32_t payld_len,
    __gpr uint8_t odd_byte_offset_bmask
    );
#define PIF_FLCALC_CSUM16_INIT  (0x0)

/**
 * @brief Computes the 16-bit sum of 4-byte aligned data in local memory, taken
 *        two bytes at a time.
 *
 * @param init          Initial value of calculated field.
 * @param lm_ptr        Pointer to fields in local memory.
 * @param lm_len        Length in bytes of the input data in local memory.
 *
 * @returns 16-bit sum.
 *
 * @note This function does not deal with the case where the alignment of the 
 *       first byte of data is an odd number. This can be the case when a fixed
 *       length field follows a variable length field in a P4 field list. An odd
 *       alignment changes which byte of a 2-byte pair is the most/least
 *       significant.
 *       pif_flcalc_csum16() should handle the general case.
 */
uint16_t pif_flcalc_csum16_lmem(
    uint16_t init,
    __lmem uint32_t *lm_ptr,
    __gpr size_t lm_len
    );

/**
 * @brief Computes the 16-bit sum of 2-byte aligned data in bulk memory (CTM or
 *        EMEM), taken two bytes at a time.
 *
 * @param init          Initial value of calculated field.
 * @param payld_offset  Offset in bytes of the payload from the start of the
 *                      packet.
 * @param payld_len     Length in bytes of the payload.
 *
 * @returns 16-bit sum.
 *
 * @note This function does not deal with the case where the length of the data 
 *       up to the payload in the P4 field list is odd. An odd alignment changes
 *       which byte of a 2-byte pair is the most/least significant.
 *       pif_flcalc_csum16() should handle the general case.
 */
uint16_t pif_flcalc_csum16_mem(
    uint16_t init,
    __gpr uint32_t payld_offset,
    __gpr uint32_t payld_len
    );

/**
 * @brief Updates one's complement of the 16-bit sum of data, taken two bytes
 *        at a time, when replacing the value 'lm_old' with 'lm_new'.
 *
 * @param init              Initial value of calculated field.
 * @param lm_old            Pointer to old value.
 * @param lm_new            Pointer to new value.
 * @param lm_len            Length in bytes of value.
 * @param odd_byte_offset   Set to 1 if byte offset of updated value is odd,
 *                          0 if even.
 * @param byte_mask_first   Mask to apply to first byte of new value.
 * @param byte_mask_last    Mask to apply to last byte of new value
 *
 * @returns 16-bit checksum.
 */
uint16_t pif_flcalc_csum16_update(
    uint16_t init,
    __lmem uint8_t *lm_old,
    __lmem uint8_t *lm_new,
    __gpr size_t lm_len,
    __gpr uint8_t odd_byte_offset,
    __gpr uint8_t byte_mask_first,
    __gpr uint8_t byte_mask_last
    );

/**
 * @brief Updates one's complement of the 16-bit sum with one 32-bit word as
 * input
 *
 * @param init              Initial value of calculated field.
 * @param val               Value to update
 * @param val_mask          Which bits of the value have changed
 * @param is_rem            Set to non-zero to remove, zero to add
 *
 * @returns 16-bit checksum.
 */
uint16_t pif_flcalc_csum16_update_lw(
    uint16_t init,
    uint32_t val,
    uint32_t val_mask,
    int is_rem);

/**
 * @brief Updates 16-bit xor sum with one 32-bit word as input
 *
 * @param init              Initial value of calculated field.
 * @param val               Value to update
 * @param val_mask          Which bits of the value have changed
 * @param is_rem            Set to non-zero to remove, zero to add
 *
 * @returns 16-bit checksum.
 */
uint16_t pif_flcalc_xor16_update_lw(
    uint16_t init,
    uint32_t val,
    uint32_t val_mask,
    int is_rem);

/**
 * @brief Computes exclusive-or of data taken two bytes at a time.
 *
 * @param init          Initial value of calculated field.
 * @param lm_ptr        Pointer to fields in local memory.
 * @param lm_len        Length in bytes of the input data in local memory.
 * @param lm_offset     Offset into local memory input data.
 * @param cls_ptr       Pointer to variable field in CLS.
 * @param cls_len       Length in bytes of variable field.
 * @param payld_offset  Offset in bytes of the payload from the start of the
 *                      packet. Ignored if payld_len is zero.
 * @param payld_len     Length in bytes of the payload. Set to zero if the
 *                      payload is not part of the field calculation.
 * @param odd_byte_offset_bmask Indicates wheter byte offset of data in lmem,
 *                              CLS or CTM/emem is odd.
 *
 * @returns 16-bit exclusive or.
 */
uint16_t pif_flcalc_xor16(
    uint16_t init,
    __lmem uint8_t *lm_ptr,
    __gpr size_t lm_len,
    __cls uint8_t *cls_ptr,
    __gpr uint32_t cls_len,
    __gpr uint32_t payld_offset,
    __gpr uint32_t payld_len,
    __gpr uint8_t odd_byte_offset_bmask
    );
#define PIF_FLCALC_XOR16_INIT  (0x0)

/**
 * @brief Updates one's complement of the 16-bit sum of data, taken two bytes
 *        at a time, when replacing the value 'lm_old' with 'lm_new'.
 *
 * @param init              Initial value of calculated field.
 * @param lm_old            Pointer to old value.
 * @param lm_new            Pointer to new value.
 * @param lm_len            Offset into local memory input struct.
 * @param odd_byte_offset   Set to 1 if byte offset of updated value is odd,
 *                          0 if even.
 * @param byte_mask_first   Mask to apply to first byte of new value.
 * @param byte_mask_last    Mask to apply to last byte of new value
 *
 * @returns 16-bit checksum.
 */
uint16_t pif_flcalc_xor16_update(
    uint16_t init,
    __lmem uint8_t *lm_old,
    __lmem uint8_t *lm_new,
    __gpr size_t lm_len,
    __gpr uint8_t odd_byte_offset,
    __gpr uint8_t byte_mask_first,
    __gpr uint8_t byte_mask_last
    );

/**
 * @brief Computes CRC-16-CCITT using normal polynomial representation 0x1021.
 *
 * @param init          Initial value of calculated field.
 * @param lm_ptr        Pointer to fields in local memory.
 * @param lm_len        Length in bytes of the input data in local memory.
 * @param cls_ptr       Pointer to variable field in CLS.
 * @param cls_len       Length in bytes of variable field.
 * @param payld_offset  Offset in bytes of the payload from the start of the
 *                      packet. Ignored if payld_len is zero.
 * @param payld_len     Length in bytes of the payload. Set to zero if the
 *                      payload is not part of the field calculation.
 *
 * @returns 16-bit crc.
 */
uint16_t pif_flcalc_crc16(
    uint16_t init,
    __lmem uint8_t *lm_ptr,
    __gpr size_t lm_len,
    __cls uint8_t *cls_ptr,
    __gpr uint32_t cls_len,
    __gpr uint32_t payld_offset,
    __gpr uint32_t payld_len
    );
#define PIF_FLCALC_CRC16_INIT  (0xffff)

/**
 * @brief Computes the 16-bit CRC of 4-byte aligned data in local memory..
 *
 * @param init          Initial value of calculated field.
 * @param lm_ptr        Pointer to fields in local memory.
 * @param lm_len        Length in bytes of the input data in local memory.
 *
 * @returns 16-bit CRC.
 *
 * @note This function does not deal with the case where the lm_ptr does not 
 *       point to the first byte of a 32-bit longword in local memory. This can
 *       be the case when a fixed length field follows a variable length field
 *       in a P4 field list. The CRC command byte source tokens will not be used
 *       correctly in this case.
 *       pif_flcalc_crc16() should handle the general case.
 */
uint16_t pif_flcalc_crc16_lmem(
    uint16_t init,
    __lmem uint32_t *lm_ptr,
    __gpr size_t lm_len
    );

/**
 * @brief Computes CRC-32 using normal polynomial representation 0x04C11DB7.
 *
 * @param init          Initial value of calculated field.
 * @param lm_ptr        Pointer to fields in local memory.
 * @param lm_len        Length in bytes of the input data in local memory.
 * @param cls_ptr       Pointer to variable field in CLS.
 * @param cls_len       Length in bytes of variable field.
 * @param payld_offset  Offset in bytes of the payload from the start of the
 *                      packet. Ignored if payld_len is zero.
 * @param payld_len     Length in bytes of the payload. Set to zero if the
 *                      payload is not part of the field calculation.
 *
 * @returns 32-bit crc.
 */
uint32_t pif_flcalc_crc32(
    uint32_t init,
    __lmem uint8_t *lm_ptr,
    __gpr size_t lm_len,
    __cls uint8_t *cls_ptr,
    __gpr uint32_t cls_len,
    __gpr uint32_t payld_offset,
    __gpr uint32_t payld_len
    );
#define PIF_FLCALC_CRC32_INIT  (0xffffffff)

/**
 * @brief Computes the 32-bit CRC of 4-byte aligned data in local memory..
 *
 * @param init          Initial value of calculated field.
 * @param lm_ptr        Pointer to fields in local memory.
 * @param lm_len        Length in bytes of the input data in local memory.
 *
 * @returns 32-bit CRC.
 *
 * @note This function does not deal with the case where the lm_ptr does not 
 *       point to the first byte of a 32-bit longword in local memory. This can
 *       be the case when a fixed length field follows a variable length field
 *       in a P4 field list. The CRC command byte source tokens will not be used
 *       correctly in this case.
 *       pif_flcalc_crc32() should handle the general case.
 */
uint32_t pif_flcalc_crc32_lmem(
    uint32_t init,
    __lmem uint32_t *lm_ptr,
    __gpr size_t lm_len_b
    );

/**
 * @brief Reflects then inverts all bits in CRC-32 remainder.
 *
 * @param in_crc32  The remainder being reflected, then inverted.
 *
 * @returns 32-bit CRC.
 */
__gpr uint32_t pif_flcalc_crc32_reflect(
    __gpr uint32_t in_crc32
    );

/* Each bit of the 'odd_byte_offset_bmask' indicates that the byte offset of
 * data in field list(lmem), variable field(CLS) or payload(CTM/emem) is odd. */
#define PIF_FLCALC_ODD_LMEM 0x1
#define PIF_FLCALC_ODD_CLS  0x2
#define PIF_FLCALC_ODD_MEM  0x4

#endif /* __PIF_FLCALC_ALGORITHMS_H__ */
