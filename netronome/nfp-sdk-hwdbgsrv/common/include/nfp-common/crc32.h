/*
 * Copyright (C) 2009-2010, Netronome Systems, Inc.  All rights reserved.
 *
 */

#ifndef NFP_COMMON_CRC32_H
#define NFP_COMMON_CRC32_H

#include <stdint.h>
#include <nfp-common/nfp_platform.h>

#include "_nfp_common_apiex.h"

__BEGIN_DECLS

NFP_COMMON_API
uint32_t crc32_posix_add(uint32_t crc32, const void *buff, size_t len);

NFP_COMMON_API
uint32_t crc32_posix_end(uint32_t crc32, size_t total_len);

NFP_COMMON_API
uint32_t crc32_posix(const void *buff, size_t len);

NFP_COMMON_API
uint32_t crc32_gb(uint32_t crc, const void *buff, size_t len);

__END_DECLS

#endif /* NFP_COMMON_CRC32_H */
