/*
 * Copyright (C) 2014,2017 Netronome Systems, Inc.
 * All right reserved.
 *
 */

#ifndef __NFP_CPPAT_H__
#define __NFP_CPPAT_H__

#include <nfp-common/nfp_platform.h>
#include <nfp-common/nfp_resid.h>

#include "_nfp_common_apiex.h"

__BEGIN_DECLS

#ifdef __KERNEL__
#define UINT64_C(x) ((uint64_t)x)
#endif

NFP_COMMON_API
int _nfp6000_encode_basic(uint64_t *addr, int dest_island, int cpp_tgt,
			  int mode, int addr40, int isld1, int isld0);

NFP_COMMON_API
int _nfp6000_decode_basic(uint64_t addr, int *dest_island, int cpp_tgt,
			  int mode, int addr40, int isld1, int isld0);

NFP_COMMON_API
int _nfp6000_cppat_mu_locality_lsb(int mode, int addr40);

NFP_COMMON_API
int _nfp6000_encode_mu(uint64_t *addr, int dest_island, int mode, int addr40,
		       int isld1, int isld0);

NFP_COMMON_API
int _nfp6000_decode_mu(uint64_t addr, int *dest_island, int mode, int addr40,
		       int isld1, int isld0);

NFP_COMMON_API
int _nfp6000_cppat_addr_encode(uint64_t *addr, int dest_island, int cpp_tgt,
			       int mode, int addr40, int isld1, int isld0);

NFP_COMMON_API
int _nfp6000_cppat_addr_decode(uint64_t addr, int *dest_island, int cpp_tgt,
			       int mode, int addr40, int isld1, int isld0);

NFP_COMMON_API
int _nfp6000_cppat_addr_iid_clear(uint64_t *addr, int cpp_tgt, int mode,
				  int addr40);

__END_DECLS

#endif /* __NFP_CPPAT_H__ */
