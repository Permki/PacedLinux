/*! @file         nfp_bulk.h
 * @brief         Interface for internal only NFP functions.
 *
 * Copyright (C) 2013,  Netronome Systems, Inc.  All rights reserved.
 */
#ifndef __NFP_BULK_H__
#define __NFP_BULK_H__

#include <nfp-common/nfp_platform.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/cdefs.h>
#include <nfp.h>
#include <nfp_cpp.h>

#ifdef __KERNEL__
#define NO_NFP_API_EXPORT
#endif

/* API is part of libnfp in the SDK */
#include "_nfp_apiex.h"


__BEGIN_DECLS;

/** @file nfp_bulk.h
 *  @addtogroup nfhal NFHAL
 *  @{
 */

/** @name Functions for retrieving many CSRs at once
 *  @{
 */

#define NFP_BULK_MECSR_MEID_of(v) (((v) >> 0) & 0xffff)
#define NFP_BULK_MECSR_MEID(v)    (((v) & 0xffff) << 0)
#define NFP_BULK_MECSR_CSR_of(v)  (((v) >> 16) & 0xfff)
#define NFP_BULK_MECSR_CSR(v)     (((v) & 0xfff) << 16)
#define NFP_BULK_MECSR_CTX_of(v)  (((v) >> 28) & 0xf)
#define NFP_BULK_MECSR_CTX(v)     (((v) & 0xf) << 28)

/**
 * Read many ME CSRs on NFP device.
 *
 * @param dev           NFP device
 * @param mecsrs        Array of mecsrs to read
 * @param values        Array of return values
 * @param len           input and output array sizes
 *
 * Use NFP_BULK_MECSR_* macros to build up the mecsrs values.
 *
 * @return 0 on success, or -1 on failure (and set errno accordingly).
 */
NFP_API
int nfp_bulk_mecsr_read(struct nfp_device *dev, uint32_t *mecsrs, uint32_t *values, int len);

/**
 * Write to many ME CSRs on NFP device.
 *
 * @param dev           NFP device
 * @param mecsrs        Array of mecsrs to write
 * @param values        Array of values to write
 * @param len           input and output array sizes
 *
 * Use NFP_BULK_MECSR_* macros to build up the mecsrs values.
 *
 * @return 0 on success, or -1 on failure (and set errno accordingly).
 */
NFP_API
int nfp_bulk_mecsr_write(struct nfp_device *dev, uint32_t *mecsrs, uint32_t *values, int len);

/**
 * Read an ME Register range
 *
 * @param dev           NFP device
 * @param meid          MicroEngine ID
 * @param reg           Register to read
 * @param values        Return buffer for read values
 * @param count         Number of words to read
 *
 * @return 0 on success, or -1 on failure (and set errno accordingly).
 */
NFP_API
int nfp_bulk_me_register_read(struct nfp_device *dev, unsigned int meid, unsigned int reg, uint32_t *values, int count);

/**
 * Write an ME Register range
 *
 * @param dev           NFP device
 * @param meid          MicroEngine ID
 * @param reg           Register to read
 * @param values        Values to write
 * @param count         Number of words to read
 *
 * @return 0 on success, or -1 on failure (and set errno accordingly).
 */
NFP_API
int nfp_bulk_me_register_write(struct nfp_device *dev, unsigned int meid, unsigned int reg, uint32_t *values, int count);

/**
 * Read many XPB CSRs on an NFP device.
 *
 * @param dev           NFP device
 * @param xpb_tgts      Array of XPB targets to read
 * @param values        Array of return values
 * @param len           input and output array sizes
 *
 * @return 0 on success, or -1 on failure (and set errno accordingly).
 */
NFP_API
int nfp_bulk_xpb_read(struct nfp_cpp *cpp, uint32_t *xpb_tgts, uint32_t *values, int len);

/**
 * Write to many XPB CSRs on an NFP device.
 *
 * @param dev           NFP device
 * @param xpb_tgts      Array of XPB targets to read
 * @param values        Array of values to write
 * @param len           input and output array sizes
 *
 * @return 0 on success, or -1 on failure (and set errno accordingly).
 */
NFP_API
int nfp_bulk_xpb_write(struct nfp_cpp *cpp, uint32_t *xpb_tgts, uint32_t *values, int len);

/** @}
 * @}
 */

__END_DECLS;

#endif /* !__NFP_BULK_H__ */
