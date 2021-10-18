/*
 * Copyright (C) 2009-2010, Netronome Systems, Inc.  All rights reserved.
 *
 */

#ifndef NFP_BSP_CRC32_H
#define NFP_BSP_CRC32_H

#include <stdint.h>
#include <sys/types.h>

uint32_t crc32_posix_add(uint32_t crc, const void *buff, size_t len);
uint32_t crc32_posix_end(uint32_t crc, size_t total_len);
static inline uint32_t crc32_posix(const void *buff, size_t len)
{
    return crc32_posix_end(crc32_posix_add(0, buff, len), len);
}

uint32_t crc32_gb(uint32_t crc, const void *buff, size_t len);

#endif /* NFP_BSP_CRC32_H */
/* vim: set tabstop=4 softtabstop=4 shiftwidth=4 expandtab: */
