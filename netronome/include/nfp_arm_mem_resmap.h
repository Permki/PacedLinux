/*
 * Copyright (C) 2016, Netronome Systems, Inc.
 * All right reserved.
 *
 */

#ifndef ARM_MEM_RESMAP_H
#define ARM_MEM_RESMAP_H

#define KB(x)                           ((x) * 0x400)
#define MB(x)                           ((x) * KB(1) * KB(1))

/* arm mem resource 1 */
#define NFP_POWMON_RES_SIZE             MB(16)
#define NFP_POWMON_RES_NAME             "nfp.pmon"
#define NFP_POWMON_RES_COUNT            ((NFP_POWMON_RES_SIZE -        \
					  (3 * sizeof(uint64_t))) /     \
					  sizeof(struct power_log_entry))

/* arm mem resource 2 */
#define NFP_NSPLOG_RES_SIZE             MB(4)
#define NFP_NSPLOG_RES_NAME             "nsp.log"

/* arm mem resource 3 */
#define NFP_PHYMOD_RES_SIZE             KB(5)
#define NFP_PHYMOD_RES_NAME             "phymod"

/* sum of sizes of all arm mem resources */
#define ARM_MEM_RES_TOTAL_SIZE          (NFP_POWMON_RES_SIZE + \
					 NFP_NSPLOG_RES_SIZE + \
					 NFP_PHYMOD_RES_SIZE)

/*
 * for NSP usage need ~4MB (.text, .data, and minimal heap) plus worst case
 *  ~20MB for FW (nffw/elf) loading
 */
#define MIN_ARM_WORKSPACE_SIZE_MB	32

int nfp_arm_resource_base(struct nfp_device *nfp, char *resname, uint64_t *base);

#define NSPLOG_TYPE_TERM	0xffff
#define NSPLOG_TYPE_ROLLEDTERM	0xfffe

#endif /* ARM_MEM_RESMAP_H */
