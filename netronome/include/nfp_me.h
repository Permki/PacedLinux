/*
 * Copyright (C) 2014, Netronome Systems, Inc.
 * All right reserved.
 *
 * This file holds chip family-specific APIs for access to microengine
 * related memory, ME control, ME register access and accesses that may
 * require loading ME code to perform their function.
 *
 */

#ifndef __NFP_ME_H__
#define __NFP_ME_H__

#include <nfp-common/nfp_platform.h>
#include <nfp-common/nfp_resid.h>
#include <stdint.h>

#define NFP6000_MAX_ISLAND_ID 63

#define NFP_USTORE_INSTRUCTIONS 8192

#define NFP_ME_INSTR_SAVE_CNT 10
#define NFP_ME_XFER_SAVE_CNT 8
#define NFP_ME_CTX_SAVE_CNT 1

/* When loading codestore, writing some extra instructions after the block
 * that will be executed ensures correct ECC bits for instructions that
 * will get pre-fetched into the pipeline (and ECC verified), even though
 * these are later aborted. For NFP-6xxx A0:B0, this needs to be enough for
 * one pipeline, but we round it to 8. For ME v2.9, this needs to be at
 * least 20, but we'll round that up to 32 and use that for A0 and B0 as well
 * to keep code simpler.
 */
#define NFP6000_ME_USTORE_ECC_PADDING 32

struct nfp_device;

struct nfp6000_me_state {
	uint32_t rd_xfer[NFP_ME_XFER_SAVE_CNT];
	uint32_t wr_xfer[NFP_ME_XFER_SAVE_CNT];
	uint32_t active_ctx_sts, ctx_enables, misc_control, ctx_arb_ctl;
	uint32_t cc_enable, t_index_byte, csr_ctx_ptr;
	struct {
		uint32_t lm_addr_0, ctx_sts;
	} ctx[NFP_ME_CTX_SAVE_CNT];

	uint32_t instr_saved : 1;
	uint64_t instr[NFP_ME_INSTR_SAVE_CNT];
};

static nfp_inline int nfp6000_island_mecnt(int island_id)
{
	switch (island_id) {
	case NFP6000_IID_ARM:
	case NFP6000_IID_PCIE0:
	case NFP6000_IID_PCIE1:
	case NFP6000_IID_PCIE2:
	case NFP6000_IID_PCIE3:
	case NFP6000_IID_CRYPTO0:
	case NFP6000_IID_CRYPTO1:
	case NFP6000_IID_ILA0:
	case NFP6000_IID_ILA1:
		return 4;
	case NFP6000_IID_MEI0:
	case NFP6000_IID_MEI1:
	case NFP6000_IID_MEI2:
	case NFP6000_IID_MEI3:
	case NFP6000_IID_MEI4:
	case NFP6000_IID_MEI5:
	case NFP6000_IID_MEI6:
		return 12;
	default:
		break;
	};
	return 0;
};

static nfp_inline int nfp3800_island_mecnt(int island_id)
{
	switch (island_id) {
	case NFP3800_IID_ARM:
		return 4;
	case NFP3800_IID_PCIE0:
	case NFP3800_IID_CRYPTO0:
		return 8;
	case NFP3800_IID_MEI0:
	case NFP3800_IID_MEI1:
	case NFP3800_IID_MEI2:
		return 12;
	default:
		break;
	};
	return 0;
};

int nfp_island_mecnt(struct nfp_device *nfp, int iid);

int nfp_has_meid(struct nfp_device *nfp, int meid);
int nfp_meid_first(struct nfp_device *nfp);
int nfp_meid_last(struct nfp_device *nfp);
int nfp_meid_next(struct nfp_device *nfp, int meid);
int nfp_meid_prev(struct nfp_device *nfp, int meid);

int nfp6000_crypto_sram_write(struct nfp_device *nfp,
			      int island_id,
			      uint64_t offset,
			      size_t size,
			      const void *src);

int nfp6000_crypto_sram_read(struct nfp_device *nfp,
			     int island_id,
			     uint64_t offset,
			     size_t size,
			     void *dst);

/**** NFP-6xxx memories ****/
int nfp6000_cls_memset(struct nfp_device *nfp,
		       int island_id,
		       uint64_t offset,
		       size_t size,
		       int value);

int nfp6000_cls_write(struct nfp_device *nfp,
		      int island_id,
		      uint64_t offset,
		      size_t size,
		      const void *src);

int nfp6000_cls_read(struct nfp_device *nfp, int island_id, uint64_t offset, size_t size, void *dst);

int nfp6000_lmem_memset(struct nfp_device *nfp, int meid, uint64_t offset, size_t size, int value);

int nfp6000_lmem_write(struct nfp_device *nfp,
		       int meid,
		       uint64_t offset,
		       size_t size,
		       const void *src);

int nfp6000_lmem_read(struct nfp_device *nfp, int meid, uint64_t offset, size_t size, void *dst);

int nfp6000_ustore_write(struct nfp_device *nfp,
			 int meid,
			 uint64_t offset,
			 size_t size,
			 const void *src);

int nfp6000_ustore_read(struct nfp_device *nfp, int meid, uint64_t offset, size_t size, void *dst);

int nfp6000_sram_me_memset(struct nfp_device *nfp,
			   int meid,
			   uint64_t offset,
			   size_t size,
			   int value);

int nfp6000_sram_write(struct nfp_device *nfp, uint64_t offset, size_t size, const void *src);

int nfp6000_sram_read(struct nfp_device *nfp, uint64_t offset, size_t size, void *dst);

int nfp6000_sram_memset(struct nfp_device *nfp, uint64_t offset, size_t size, int value);

int nfp6000_mumem_me_memset(struct nfp_device *nfp,
			    int meid,
			    int island_id,
			    uint8_t locality,
			    uint64_t offset,
			    size_t size,
			    int value);

int nfp6000_mumem_write(struct nfp_device *nfp,
			int island_id,
			uint8_t locality,
			uint64_t offset,
			size_t size,
			const void *src);

int nfp6000_mumem_read(struct nfp_device *nfp,
		       int island_id,
		       uint8_t locality,
		       uint64_t offset,
		       size_t size,
		       void *dst);

int nfp6000_mumem_memset(struct nfp_device *nfp,
			 int island_id,
			 uint8_t locality,
			 uint64_t offset,
			 size_t size,
			 int value);

/**** NFP-6xxx ME control and registers ****/
int nfp6000_read_mecsr(struct nfp_device *nfp, int meid, uint32_t off, uint32_t *valuep);

int nfp6000_write_mecsr(struct nfp_device *nfp, int meid, uint32_t off, uint32_t value);

int nfp6000_read_mecsr_ctx(struct nfp_device *nfp,
			   int meid,
			   int ctx,
			   uint32_t off,
			   uint32_t *valuep);

int nfp6000_write_mecsr_ctx(struct nfp_device *nfp, int meid, int ctx, uint32_t off, uint32_t value);

int nfp6000_is_ctx_mecsr(struct nfp_device *nfp, uint32_t off);

int nfp6000_read_mereg(struct nfp_device *nfp,
		       int meid,
		       uint32_t off,
		       uint32_t *valuep,
		       int busy_check);

int nfp6000_write_mereg(struct nfp_device *nfp,
			int meid,
			uint32_t off,
			uint32_t value,
			int busy_check);

int nfp6000_read_gprA(struct nfp_device *nfp,
		      int meid,
		      uint32_t idx,
		      uint32_t *value,
		      int busy_check);

int nfp6000_write_gprA(struct nfp_device *nfp,
		       int meid,
		       uint32_t idx,
		       uint32_t value,
		       int busy_check);

int nfp6000_read_gprB(struct nfp_device *nfp,
		      int meid,
		      uint32_t idx,
		      uint32_t *value,
		      int busy_check);

int nfp6000_write_gprB(struct nfp_device *nfp,
		       int meid,
		       uint32_t idx,
		       uint32_t value,
		       int busy_check);

int nfp6000_read_xferin(struct nfp_device *nfp,
			int meid,
			uint32_t idx,
			uint32_t *value,
			int busy_check);

int nfp6000_write_xferin(struct nfp_device *nfp, int meid, uint32_t idx, uint32_t value);

int nfp6000_read_xferout(struct nfp_device *nfp, int meid, uint32_t idx, uint32_t *value);

int nfp6000_write_xferout(struct nfp_device *nfp,
			  int meid,
			  uint32_t idx,
			  uint32_t value,
			  int busy_check);

int nfp6000_read_nnreg(struct nfp_device *nfp,
		       int meid,
		       uint32_t idx,
		       uint32_t *value,
		       int busy_check);

int nfp6000_write_nnreg(struct nfp_device *nfp,
			int meid,
			uint32_t idx,
			uint32_t value,
			int busy_check);

int nfp6000_me_running(struct nfp_device *nfp, int meid);

int nfp6000_me_active(struct nfp_device *nfp, int meid);

int nfp6000_stop_me(struct nfp_device *nfp, int meid);

/**
 * Reset a ME group
 *
 * @param nfp   NFP Device handle
 * @param meid  Can be either odd or even numbered ME in a group. Both MEs
 *              in that group will be reset.
 */
int nfp6000_reset_megrp(struct nfp_device *nfp, int meid);
int nfp3800_reset_megrp(struct nfp_device *nfp, int meid);

int nfp6000_halt_mes(struct nfp_device *nfp);
int nfp3800_halt_mes(struct nfp_device *nfp);

int nfp6000_load_me_code(struct nfp_device *nfp,
			 int meid,
			 uint64_t *instr,
			 int icnt,
			 uint32_t start,
			 int padding);

int nfp6000_run_me_code(struct nfp_device *nfp,
			int meid,
			uint32_t ctx_mask,
			uint32_t start,
			uint32_t max_runtime);

int nfp6000_execute_me_instr(struct nfp_device *nfp,
			     int meid,
			     uint32_t ctx_mask,
			     uint64_t *instr,
			     int icnt,
			     uint32_t start);

int nfp6000_stop_and_save_me_state(struct nfp_device *nfp, int meid, struct nfp6000_me_state *st);

int nfp6000_restore_and_start_me_state(struct nfp_device *nfp,
				       int meid,
				       struct nfp6000_me_state *st);

#endif /* __NFP_ME_H__ */
