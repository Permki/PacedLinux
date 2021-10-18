/*! @file         nfp_cpp_internal.h
 * @brief         Interface for internal only NFP CPP functions.
 *
 * Copyright (C) 2013,  Netronome Systems, Inc.  All rights reserved.
 */
#ifndef __NFP_CPP_INTERNAL_H__
#define __NFP_CPP_INTERNAL_H__

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
struct nfp_cpp;

__BEGIN_DECLS;

struct nfp_cpp_explicit_data_internal {
    uint32_t cpp_id; /* target, action, token ala nfp_cpp.h */
    uint8_t length;
    uint8_t bytemask;
    uint64_t address;
    uint8_t data_master_island;
    uint8_t data_master;
    uint16_t data_ref;
    uint8_t signal_master;
    uint8_t signal_ref;
    int8_t posted;
    uint8_t siga;
    int8_t siga_mode;
    uint8_t sigb;
    int8_t sigb_mode;
    uint8_t pull_data[128];
    uint8_t pull_len;
};

/**
 * Issue an explicit CPP transaction
 * @param dev       NFP device
 * @param cppdata   CPP transaction data
 * @param pushbuf   Buffer for push data
 * @param buflen    Length of push buffer
 * @param pushlen   Returned push data length
 *
 * @returns 0 on success, -1 on failure (sets errno accordingly)
 */
NFP_API
int nfp_cpp_explicit_do_internal(struct nfp_cpp *cpp, struct nfp_cpp_explicit_data_internal *cppdata, void *push_buf, size_t buf_len, int *push_len);


__END_DECLS;

#endif /* !__NFP_CPP_INTERNAL_H__ */
