/*
 * Copyright (C) 2015,2017  Netronome Systems, Inc.  All rights reserved.
 *
 * @file          nfp_nffw_rtt.h
 * @brief         Run-time data structures and types for NFFW, such as MIP.
 *
 * The structs in this file are packed and will most commonly be data the linker
 * places in the nffw file and the loader and other tools then use these
 * for runtime details and config, such as the Microcode Information Page
 * which then contains an entry that is the runtime symbol table.
 *
 * This file only provides the types for these data structures, no APIs, since
 * these types need to stay in sync across BSP and SDK and be process across
 * various platforms in the same manner.
 *
 */
#ifndef __NFP_NFFW_RTT_H__
#define __NFP_NFFW_RTT_H__

#include <nfp-common/nfp_platform.h>

__BEGIN_DECLS

#pragma pack(push, 1)
#define NFP_MIP_SIGNATURE 0x0050494d /* "MIP\0" (little-endian) */

/**
 * This is the maximum range of the first DDR resource that can be scanned
 * to find a MIP if the NFFW did not indicate where the MIP is (built with
 * an old SDK toolchain).
 *
 * The MIP location is now marked in the NFFW file with a .note section and
 * the loader then updates the nfp.nffw resource to hold this pointer. The
 * MIP location can then be anywhere.
 */
#define NFP_MIP_MAX_SCAN_OFFSET (256 * 1024)
#define NFP_MIP_VERSION 2
#define NFP_MIP_QC_VERSION 1
#define NFP_MIP_VPCI_VERSION 1
enum nfp_mip_entry_type {
	NFP_MIP_TYPE_NONE = 0,
	NFP_MIP_TYPE_QC = 1,
	NFP_MIP_TYPE_VPCI = 2,

	/* For SDK and BSP use, linker/loader etc. */
	NFP_MIP_TYPE_RSVD_BEGIN = 0x70000000,
	NFP_MIP_TYPE_RTSYMTAB = 0x70000000,
	NFP_MIP_TYPE_RTSTRTAB = 0x70000001,
	NFP_MIP_TYPE_RSVD_END = 0x7fffffff
};

/* NFP_MIP_MEMTYPEs above 0 are CPP target IDs.
 * So for NFP-6xxx:
 * 7: MU, 'domain' is island ID which determines EMEM, IMEM or CTM
 * 15: CLS, 'domain' is island ID.
 * -7: MU with Direct Access, EMEM islands only (emem_cache_upper)
 * -1: ME local memory, domain == MEID
 * -2: Same as -1, but exists purely for completeness and won't be used in the
 *     foreseeable future.
 */
#define NFP_MIP_MEMTYPE_NONE (0)
#define NFP_MIP_MEMTYPE_LMEM (-1)
#define NFP_MIP_MEMTYPE_USTORE (-2)
#define NFP_MIP_MEMTYPE_EMU_CACHE (-7)

/* Each packed struct field is stored as
 * Little Endian and loaded as Host Endian. The nfp_mip API will handle
 * endian conversions and present host endianness.
 */

/** 'nfp_mip_v0' is the part of all MIP versions that must always have the same
 * layout. Used in offsetof() for example.
 */
struct nfp_mip_v0 {
	uint32_t signature;   /* 0 */
	uint32_t mip_version; /* 4 */
	uint32_t _pad_0;      /* 8 */
	uint32_t _pad_1;      /* 12 */

	uint32_t fw_version;   /* 16 */
	uint32_t fw_buildnum;  /* 20 */
	uint32_t fw_buildtime; /* 24 */
	uint32_t fw_loadtime;  /* 28 */
			       /* 32 */
};

struct nfp_mip_v1 {
	uint32_t signature;
	uint32_t mip_version;
	/* mip_size */
	uint32_t mip_size;
	uint32_t first_entry;

	uint32_t version;
	uint32_t buildnum;
	uint32_t buildtime;
	uint32_t loadtime;

	uint32_t symtab_addr;
	uint32_t symtab_size;
	uint32_t strtab_addr;
	uint32_t strtab_size;

	char name[16];
	char toolchain[32];
};

struct nfp_mip_v2_entry_loc {
	int8_t mem_type;  /* 0 */
	uint16_t domain;  /* 1 */
	uint8_t addr_hi;  /* 3 */
	uint32_t addr_lo; /* 4 */
			  /* 8 */
};

struct nfp_mip_v2 {
	uint32_t signature;   /* 0 */
	uint32_t mip_version; /* 4 */
	uint32_t _rsvd_0;     /* 8 */
	uint32_t _rsvd_1;     /* 12 */

	uint32_t fw_version;   /* 16 */
	uint32_t fw_buildnum;  /* 20 */
	uint32_t fw_buildtime; /* 24 */
	uint32_t fw_loadtime;  /* 28 */

	struct nfp_mip_v2_entry_loc first_entry; /* 32 */

	char fw_name[16];      /* 40 */
	char fw_toolchain[32]; /* 48 */
			       /* 80 */
};

struct nfp_mip_v1_entry {
	uint32_t type;
	uint32_t version;
	uint32_t offset_next;
};

/* Linked list ends when next_entry.mem_type == 0 or type == 0.
 */
struct nfp_mip_v2_entry {
	uint32_t type;				/* 0 */
	uint32_t version;			/* 4 */
	uint32_t size;				/* 8 */
	struct nfp_mip_v2_entry_loc next_entry; /* 12 */
						/* 20 */
};

struct nfp_mip_qc_v1 {
	uint32_t type;
	uint32_t version;
	uint32_t offset_next;
	uint32_t type_config;
	uint32_t type_config_size;
	uint32_t host_config;
	uint32_t host_config_size;
	uint32_t config_signal;
	uint32_t nfp_queue_size;
	uint32_t queue_base;
	uint32_t sequence_base;
	uint32_t sequence_type;
	uint32_t status_base;
	uint32_t status_version;
	uint32_t error_base;
};

struct nfp_mip_vpci_v1 {
	uint32_t type;
	uint32_t version;
	uint32_t offset_next;
	uint32_t vpci_epconfig;
	uint32_t vpci_epconfig_size;
};

#define NFP_RTSYM_V1_TGT_LMEM 0
#define NFP_RTSYM_V1_TGT_UMEM 0xFE /* Only NFP-32xx */
#define NFP_RTSYM_V1_TGT_EMU_CACHE 0x17

#define NFP_RTSYM_TYPE_NONE (0)
#define NFP_RTSYM_TYPE_OBJECT (1)
#define NFP_RTSYM_TYPE_FUNCTION (2)
#define NFP_RTSYM_TYPE_ABS (3)

#define NFP_RTSYM_TARGET_NONE NFP_MIP_MEMTYPE_NONE
#define NFP_RTSYM_TARGET_LMEM NFP_MIP_MEMTYPE_LMEM
#define NFP_RTSYM_TARGET_USTORE NFP_MIP_MEMTYPE_USTORE
#define NFP_RTSYM_TARGET_EMU_CACHE NFP_MIP_MEMTYPE_EMU_CACHE

struct nfp_rtsymtab_v1_entry {
	uint8_t type;
	uint8_t target;
	union {
		uint8_t val;
		/* N/A, island or linear menum, depends on 'target' */
		uint8_t nfp3200_domain;
		/* 0xff if N/A */
		uint8_t nfp6000_island;
	} domain1;
	uint8_t addr_hi;
	uint32_t addr_lo;
	uint16_t name;
	union {
		uint8_t val;
		uint8_t nfp3200___rsvd;
		/* 0xff if N/A */
		uint8_t nfp6000_menum;
	} domain2;
	uint8_t size_hi;
	uint32_t size_lo;
};

struct nfp_rtsymtab_v2_entry {
	/* mem_type and domain use the same values and
	 * format as nfp_mip entries
	 */
	uint8_t sym_type;
	int8_t mem_type;
	uint16_t domain;

	uint8_t _rsvd_0; /* Future feature */
	uint8_t _rsvd_1; /* Future feature */
	uint8_t addr_hi;
	uint32_t addr_lo;
	uint8_t size_hi;
	uint32_t size_lo;

	uint32_t name; /* rtstrtab byte index */
};

#pragma pack(pop)

__END_DECLS

#endif /* __NFP_NFFW_RTT_H__ */
