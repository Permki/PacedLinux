/*
 * Copyright (C) 2009-2010, Netronome Systems, Inc.  All rights reserved.
 *
 */

#ifndef CRC_H
#define CRC_H

#include <stdint.h>

/* 16bit polynomial (X^16 + X^12 + X^5 + 1)
 * (Polynomial = 0x1021)
 * LSB first variant
 */
static inline uint16_t crc16_lsb(uint16_t crc16, uint8_t c)
{
    crc16 = (crc16 << 8) | ((crc16 >> 8) & 0xff);
    crc16 ^= c;
    crc16 ^= (crc16 & 0xff) >> 4;
    crc16 ^= (crc16 << 8) << 4;
    crc16 ^= ((crc16 & 0xff) << 4) << 1;

    return crc16;
}

/* 16bit polynomial (X^16 + X^12 + X^5 + 1)
 * (Polynomial = 0x1021)
 * MSB first variant
 */
static inline uint16_t crc16_msb(uint16_t crc16, uint8_t c)
{
    int i;

    crc16 ^= ((uint16_t)c << 8);
    for (i = 0; i < 8; i++) {
        if (crc16 & 0x8000) {
            crc16 = (crc16 << 1) ^ 0x1021;
        } else {
            crc16 <<= 1;
        }
    }

    return crc16;
}

static inline uint16_t crc16_ccitt(uint16_t crc16, uint8_t c)
{
    return crc16_lsb(crc16, c);
}

static inline uint16_t crc16_jedec(uint16_t crc16, uint8_t c)
{
    return crc16_msb(crc16, c);
}

static inline uint16_t crc16_xmodem(uint16_t crc16, uint8_t c)
{
    return crc16_msb(crc16, c);
}

#endif /* CRC_H */
/* vim: set tabstop=4 softtabstop=4 shiftwidth=4 expandtab: */
