/*! @file         nfp_internal_interface.h
 * @brief         Interface for internal only NFP functions.
 *
 * Copyright (C) 2013,  Netronome Systems, Inc.  All rights reserved.
 */
#ifndef __NFP_INTERNAL_H__
#define __NFP_INTERNAL_H__

#include <nfp-common/nfp_platform.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/cdefs.h>

#ifdef __KERNEL__
#define NO_NFP_API_EXPORT
#endif

/* API is part of libnfp in the SDK */
#include "_nfp_apiex.h"


/** Opaque NFP device handles. */
struct nfp_device;

__BEGIN_DECLS;

/**
 * Get hwinfo from the server
 * @param dev     NFP device
 * @param buffer  return buffer
 * @param length  return buffer length
 *
 * Data is returned as a series of null terminated strings in the return buffer;
 * the strings are ordered key then data
 *
 * @returns -1 on error, otherwise strings return buffer
 */
NFP_API
int nfp_get_hwinfo(struct nfp_device *dev, void *buffer, int length);


__END_DECLS;

#endif /* !__NFP_INTERNAL_H__ */
