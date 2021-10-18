/*
 * Copyright (C) 2014, Netronome Systems, Inc.
 * All right reserved.
 *
 * CPP Access Width Decoder
 */

#ifndef NFP_BSP_NFP_TARGET_H
#define NFP_BSP_NFP_TARGET_H

#include <nfp-common/nfp_resid.h>
#include <nfp-common/nfp_cppat.h>
#include <nfp-common/nfp_platform.h>
#include <nfp_cpp.h>

#ifndef NFP_ERRNO
#ifdef __KERNEL__
#define NFP_ERRNO(x)    (-(x))
#define UINT64_C(x)     ((uint64_t)x)
#else
#include <errno.h>
#define NFP_ERRNO(x)    (errno = (x), -1)
#endif
#endif

#define PUSH_WIDTH(_pushpull)      pushpull_width((_pushpull) >> 0)
#define PULL_WIDTH(_pushpull)      pushpull_width((_pushpull) >> 4)

int pushpull_width(int pp);

int nfp6000_nbi_dma(uint32_t cpp_id);
int nfp6000_nbi_stats(uint32_t cpp_id);
int nfp6000_nbi_tm(uint32_t cpp_id);
int nfp6000_nbi_ppc(uint32_t cpp_id);
int nfp6000_nbi(uint32_t cpp_id, uint64_t address);
int nfp6000_mu_common(uint32_t cpp_id);
int nfp6000_mu_ctm(uint32_t cpp_id);
int nfp6000_mu_emu(uint32_t cpp_id);
int nfp6000_mu_imu(uint32_t cpp_id);
int nfp6000_mu(uint32_t cpp_id, uint64_t address);
int nfp6000_ila(uint32_t cpp_id);
int nfp6000_pci(uint32_t cpp_id);
int nfp6000_crypto(uint32_t cpp_id);
int nfp6000_cap_xpb(uint32_t cpp_id);
int nfp6000_cls(uint32_t cpp_id);

int nfp6000_target_pushpull(uint32_t cpp_id, uint64_t address);
int nfp_target_pushpull_width(int pp, int write_not_read);
int nfp6000_target_action_width(uint32_t cpp_id, uint64_t address,
						  int write_not_read);
int nfp_target_action_width(uint32_t model, uint32_t cpp_id,
					      uint64_t address, int write_not_read);
int nfp_target_cpp(uint32_t cpp_island_id, uint64_t cpp_island_address,
				     uint32_t *cpp_target_id,
				     uint64_t *cpp_target_address,
				     const uint32_t *imb_table);

#endif /* NFP_BSP_NFP_TARGET_H */
