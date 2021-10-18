/*
 * Copyright (C) 2014-2017  Netronome Systems, Inc.  All rights reserved.
 *
 * @file          nfp_elf.h
 * @brief         API and definitions for NFP ELF Firmware files (NFFW files)
 *
 */
#ifndef __NFP_ELF_H__
#define __NFP_ELF_H__

#include <nfp-common/nfp_platform.h>
#include <nfp-common/nfp_resid.h>
#include <nfp-common/nfp_nffw_rtt.h>

#include "_nfp_common_apiex.h"

__BEGIN_DECLS

/* ==== Standard ELF ==== */
#define NFP_ELF_EI_NIDENT 16
#define NFP_ELF_EI_MAG0 0
#define NFP_ELF_EI_MAG1 1
#define NFP_ELF_EI_MAG2 2
#define NFP_ELF_EI_MAG3 3
#define NFP_ELF_EI_CLASS 4
#define NFP_ELF_EI_DATA 5
#define NFP_ELF_EI_VERSION 6
#define NFP_ELF_EI_PAD 7
#define NFP_ELF_ELFMAG0 0x7f
#define NFP_ELF_ELFMAG1 'E'
#define NFP_ELF_ELFMAG2 'L'
#define NFP_ELF_ELFMAG3 'F'
#define NFP_ELF_ELFCLASSNONE 0
#define NFP_ELF_ELFCLASS32 1
#define NFP_ELF_ELFCLASS64 2
#define NFP_ELF_ELFDATANONE 0
#define NFP_ELF_ELFDATA2LSB 1
#define NFP_ELF_ELFDATA2MSB 2

#define NFP_ELF_ET_NONE 0
#define NFP_ELF_ET_REL 1
#define NFP_ELF_ET_EXEC 2
#define NFP_ELF_ET_DYN 3
#define NFP_ELF_ET_CORE 4
#define NFP_ELF_ET_LOPROC 0xFF00
#define NFP_ELF_ET_HIPROC 0xFFFF

#define NFP_ELF_EM_NONE 0

/** Officially assigned EM_NFP
 * https://groups.google.com/forum/#!topic/generic-abi/7QVcsE1quQ4
 */
#define NFP_ELF_EM_NFP 250

#define NFP_ELF_EV_NONE 0
#define NFP_ELF_EV_CURRENT 1

#define NFP_ELF_SHN_UNDEF 0
#define NFP_ELF_SHN_LORESERVE 0xFF00
#define NFP_ELF_SHN_LOPROC 0xFF00
#define NFP_ELF_SHN_HIPROC 0xFF1F
#define NFP_ELF_SHN_ABS 0xFFF1
#define NFP_ELF_SHN_COMMON 0xFFF2
#define NFP_ELF_SHN_HIRESERVE 0xFFFF
#define NFP_ELF_SHN_MAX (NFP_ELF_SHN_LORESERVE - 1)

#define NFP_ELF_SHT_NULL 0
#define NFP_ELF_SHT_PROGBITS 1
#define NFP_ELF_SHT_SYMTAB 2
#define NFP_ELF_SHT_STRTAB 3
#define NFP_ELF_SHT_RELA 4
#define NFP_ELF_SHT_HASH 5
#define NFP_ELF_SHT_DYNAMIC 6
#define NFP_ELF_SHT_NOTE 7
#define NFP_ELF_SHT_NOBITS 8
#define NFP_ELF_SHT_REL 9
#define NFP_ELF_SHT_SHLIB 10
#define NFP_ELF_SHT_DYNSYM 11
#define NFP_ELF_SHT_LOPROC 0x70000000
#define NFP_ELF_SHT_HIPROC 0x7fffffff
#define NFP_ELF_SHT_LOUSER 0x80000000
#define NFP_ELF_SHT_HIUSER 0x8fffffff

#define NFP_ELF_SHF_WRITE 0x00000001
#define NFP_ELF_SHF_ALLOC 0x00000002
#define NFP_ELF_SHF_EXECINSTR 0x00000004
#define NFP_ELF_SHF_MASKPROC 0xf0000000

#define NFP_ELF_STB_LOCAL 0
#define NFP_ELF_STB_GLOBAL 1
#define NFP_ELF_STB_WEAK 2
#define NFP_ELF_STB_LOPROC 13
#define NFP_ELF_STB_HIPROC 15
#define NFP_ELF_STT_NOTYPE 0
#define NFP_ELF_STT_OBJECT 1
#define NFP_ELF_STT_FUNC 2
#define NFP_ELF_STT_SECTION 3
#define NFP_ELF_STT_FILE 4
#define NFP_ELF_STT_LOPROC 13
#define NFP_ELF_STT_HIPROC 15
#define NFP_ELF_ELF_ST_BIND(ST_INFO) (((ST_INFO) >> 4) & 0xF)
#define NFP_ELF_ELF_ST_TYPE(ST_INFO) ((ST_INFO) & 0xF)
#define NFP_ELF_ELF_ST_INFO(ST_BIND, ST_TYPE) ((((ST_BIND) & 0xF) << 4) + ((ST_TYPE) & 0xF))

#define NFP_ELF_ELF64_R_SYM(R_INFO) (((R_INFO) >> 32) & 0xFFFFFFFF)
#define NFP_ELF_ELF64_R_TYPE(R_INFO) ((R_INFO) & 0xFFFFFFFF)
#define NFP_ELF_ELF64_R_INFO(R_SYM, R_TYPE) \
	(((uint64_t)((R_SYM) & 0xFFFFFFFF) << 32) | (uint64_t)((R_TYPE) & 0xFFFFFFFF))

/* ==== NFP Specific ==== */
#define NFP_ELF_EM_NFP3200 0x3200
#define NFP_ELF_EM_NFP6000 0x6000
#define NFP_ELF_ET_NFP_PARTIAL_REL (NFP_ELF_ET_LOPROC + NFP_ELF_ET_REL)
#define NFP_ELF_ET_NFP_PARTIAL_EXEC (NFP_ELF_ET_LOPROC + NFP_ELF_ET_EXEC)

/* == NFP-32xx ELF flags == */
#define NFP_ELF_EF_NFP3200_DRAM_DUAL_SIGNALS_MASK (0x1)
#define NFP_ELF_EF_NFP3200_DRAM_DUAL_SIGNALS_LSB (31)
#define NFP_ELF_EF_NFP3200_DRAM_IGN_ALIGN_MASK (0x1)
#define NFP_ELF_EF_NFP3200_DRAM_IGN_ALIGN_LSB (30)
#define NFP_ELF_EF_NFP3200_DRAM_LOCALITY_MASK (0x3)
#define NFP_ELF_EF_NFP3200_DRAM_LOCALITY_LSB (28)
#define NFP_ELF_EF_NFP3200_MECONFIG_IDX_MASK (0xFF)
#define NFP_ELF_EF_NFP3200_MECONFIG_IDX_LSB (0)

/* == EM_NFP ELF flags */
/** Valid values for FAMILY are:
 * 0x3200 - NFP-32xx
 * 0x6000 - NFP-6xxx/NFP-4xxx
 * 0x3800 - NFP-38xx
 */
#define NFP_ELF_EF_NFP_FAMILY_MASK (0xFFFF)
#define NFP_ELF_EF_NFP_FAMILY_LSB (8)

#define NFP_ELF_SHT_NFP_MECONFIG (NFP_ELF_SHT_LOPROC + 1)
#define NFP_ELF_SHT_NFP_INITREG (NFP_ELF_SHT_LOPROC + 2)
#define NFP_ELF_SHT_UOF_DEBUG (NFP_ELF_SHT_LOUSER)

/* Init sections are all sections that should be loaded before
 * microcode init sections, if any, and executed.
 * Init2 SHT_NFP_INITREG sections are sections that should be loaded just
 * before the ME init code is loaded and executed. This allows the loader to
 * decide when to load memory sections that may modify ME CSRs.
 */
#define NFP_ELF_SHF_NFP_INIT_SECTION 0x80000000
#define NFP_ELF_SHF_NFP_INIT2_SECTION 0x40000000

/* ELF-64 sh_flags is 64-bit, but there is no info on what the upper 32 bits
 * are expected to be used for, it is not marked reserved either.
 * We'll use them for NFP-specific flags since we don't use ELF-32.
 *
 * NFP_ELF_SHF_GET_SCS(val) returns the number of additional codestores being
 * shared with the base MEID, e.g. 0 for no SCS,
 * 1 for dual SCS, 3 for quad SCS.
 */
#define NFP_ELF_SHF_SCS_MASK 0xff
#define NFP_ELF_SHF_SCS_LSB 32
#define NFP_ELF_SHF_GET_SCS(val) \
	((((uint64_t)(val)) >> NFP_ELF_SHF_SCS_LSB) & NFP_ELF_SHF_SCS_MASK)
#define NFP_ELF_SHF_SET_SCS(val) \
	((((uint64_t)(val)) & NFP_ELF_SHF_SCS_MASK) << NFP_ELF_SHF_SCS_LSB)

#define NFP_ELF_SHI_NFP_MEMTYPE_MASK 0xFFFF
#define NFP_ELF_SHI_NFP_MEMTYPE_LSB 0
#define NFP_ELF_SHI_NFP_DOMAIN_MASK 0xFFFF
#define NFP_ELF_SHI_NFP_DOMAIN_LSB 16
#define NFP_ELF_SHI_INITREG_ISLAND_MASK 0x3F
#define NFP_ELF_SHI_INITREG_ISLAND_LSB 26
#define NFP_ELF_SHI_INITREG_CPPTGT_MASK 0xF
#define NFP_ELF_SHI_INITREG_CPPTGT_LSB 22
#define NFP_ELF_SHI_INITREG_CPPACT_MASK 0x1F
#define NFP_ELF_SHI_INITREG_CPPTOK_MASK 0x3
#define NFP_ELF_SHI_INITREG_CPPACTRD_LSB 17
#define NFP_ELF_SHI_INITREG_CPPTOKRD_LSB 15
#define NFP_ELF_SHI_INITREG_CPPACTWR_LSB 10
#define NFP_ELF_SHI_INITREG_CPPTOKWR_LSB 8
#define NFP_ELF_SHI_INITREG_ORDER_MASK 0xFF
#define NFP_ELF_SHI_INITREG_ORDER_LSB 0
/* Transient init-csr sections will be validated against the target
 * but will not be kept - validate, write or read and discard.
 * They will still be handled last (in order).
 */
#define NFP_ELF_SHI_INITREG_ORDER_TRANSIENT 0xFF
#define NFP3200_ELF_GET_SHI_MEMTYPE(val) \
	(((val) >> NFP_ELF_SHI_NFP_MEMTYPE_LSB) & NFP_ELF_SHI_NFP_MEMTYPE_MASK)
#define NFP3200_ELF_SET_SHI_MEMTYPE(val) \
	(((val) & NFP_ELF_SHI_NFP_MEMTYPE_MASK) << NFP_ELF_SHI_NFP_MEMTYPE_LSB)
#define NFP3200_ELF_GET_SHI_DOMAIN(val) \
	(((val) >> NFP_ELF_SHI_NFP_DOMAIN_LSB) & NFP_ELF_SHI_NFP_DOMAIN_MASK)
#define NFP3200_ELF_SET_SHI_DOMAIN(val) \
	(((val) & NFP_ELF_SHI_NFP_DOMAIN_MASK) << NFP_ELF_SHI_NFP_DOMAIN_LSB)
#define NFP3200_ELF_GET_SHI_CLUSTER(val) (NFP3200_ELF_GET_SHI_DOMAIN(val))
#define NFP3200_ELF_SET_SHI_CLUSTER(val) (NFP3200_ELF_SET_SHI_DOMAIN(val))
#define NFP3200_ELF_GET_SHI_MEID(val) \
	((int)NFP3200_MELIN2MEID(NFP3200_ELF_GET_SHI_DOMAIN(val)))
#define NFP3200_ELF_SET_SHI_MEID(val) \
	(NFP3200_ELF_SET_SHI_DOMAIN(NFP3200_MEID2MELIN(val)))
#define NFP6000_ELF_GET_SHI_MEMTYPE(val) (NFP3200_ELF_GET_SHI_MEMTYPE(val))
#define NFP6000_ELF_SET_SHI_MEMTYPE(val) (NFP3200_ELF_SET_SHI_MEMTYPE(val))
#define NFP6000_ELF_GET_SHI_DOMAIN(val) (NFP3200_ELF_GET_SHI_DOMAIN(val))
#define NFP6000_ELF_SET_SHI_DOMAIN(val) (NFP3200_ELF_SET_SHI_DOMAIN(val))
#define NFP6000_ELF_GET_SHI_ISLAND(val) (((NFP6000_ELF_GET_SHI_DOMAIN(val)) >> 8) & 0xFF)
#define NFP6000_ELF_SET_SHI_ISLAND(val) (NFP6000_ELF_SET_SHI_DOMAIN(((val) & 0xFF) << 8))
#define NFP6000_ELF_GET_SHI_MEID(val)                  \
	(NFP6000_MEID(NFP6000_ELF_GET_SHI_ISLAND(val), \
		      (NFP6000_ELF_GET_SHI_DOMAIN(val) & 0xFF)))
#define NFP6000_ELF_SET_SHI_MEID(val)                              \
	(NFP6000_ELF_SET_SHI_ISLAND(NFP6000_MEID_ISLAND_of(val)) | \
	 NFP6000_ELF_SET_SHI_DOMAIN(NFP6000_MEID_MENUM_of((val) & 0xFF)))
#define NFP_ELF_GET_SHI_MEMTYPE(chip_family, val)                                        \
	(NFP_FAMILY_IS_ARCH_TH(chip_family) ?                                            \
		 NFP6000_ELF_GET_SHI_MEMTYPE(val) :                                      \
		 NFP_FAMILY_IS_ARCH_YD(chip_family) ? NFP3200_ELF_GET_SHI_MEMTYPE(val) : \
						      0)
#define NFP_ELF_SET_SHI_MEMTYPE(chip_family, val)                                        \
	(NFP_FAMILY_IS_ARCH_TH(chip_family) ?                                            \
		 NFP6000_ELF_SET_SHI_MEMTYPE(val) :                                      \
		 NFP_FAMILY_IS_ARCH_YD(chip_family) ? NFP3200_ELF_SET_SHI_MEMTYPE(val) : \
						      0)
#define NFP_ELF_GET_SHI_ISLAND(chip_family, val)                                         \
	(NFP_FAMILY_IS_ARCH_TH(chip_family) ?                                            \
		 NFP6000_ELF_GET_SHI_ISLAND(val) :                                       \
		 NFP_FAMILY_IS_ARCH_YD(chip_family) ? NFP3200_ELF_GET_SHI_CLUSTER(val) : \
						      0)
#define NFP_ELF_SET_SHI_ISLAND(chip_family, val)                                         \
	(NFP_FAMILY_IS_ARCH_TH(chip_family) ?                                            \
		 NFP6000_ELF_SET_SHI_ISLAND(val) :                                       \
		 NFP_FAMILY_IS_ARCH_YD(chip_family) ? NFP3200_ELF_SET_SHI_CLUSTER(val) : \
						      0)
#define NFP_ELF_GET_SHI_MEID(chip_family, val)   \
	(NFP_FAMILY_IS_ARCH_TH(chip_family) ?    \
		 NFP6000_ELF_GET_SHI_MEID(val) : \
		 NFP_FAMILY_IS_ARCH_YD(chip_family) ? NFP3200_ELF_GET_SHI_MEID(val) : 0)
#define NFP_ELF_SET_SHI_MEID(chip_family, val)   \
	(NFP_FAMILY_IS_ARCH_TH(chip_family) ?    \
		 NFP6000_ELF_SET_SHI_MEID(val) : \
		 NFP_FAMILY_IS_ARCH_YD(chip_family) ? NFP3200_ELF_SET_SHI_MEID(val) : 0)

#define NFP6000_ELF_GET_SHI_INITREG_ISLAND(val) \
	(((val) >> NFP_ELF_SHI_INITREG_ISLAND_LSB) & NFP_ELF_SHI_INITREG_ISLAND_MASK)
#define NFP6000_ELF_SET_SHI_INITREG_ISLAND(val) \
	(((val) & NFP_ELF_SHI_INITREG_ISLAND_MASK) << NFP_ELF_SHI_INITREG_ISLAND_LSB)
#define NFP6000_ELF_GET_SHI_INITREG_CPPTGT(val) \
	(((val) >> NFP_ELF_SHI_INITREG_CPPTGT_LSB) & NFP_ELF_SHI_INITREG_CPPTGT_MASK)
#define NFP6000_ELF_SET_SHI_INITREG_CPPTGT(val) \
	(((val) & NFP_ELF_SHI_INITREG_CPPTGT_MASK) << NFP_ELF_SHI_INITREG_CPPTGT_LSB)
#define NFP6000_ELF_GET_SHI_INITREG_CPPACTRD(val) \
	(((val) >> NFP_ELF_SHI_INITREG_CPPACTRD_LSB) & NFP_ELF_SHI_INITREG_CPPACT_MASK)
#define NFP6000_ELF_SET_SHI_INITREG_CPPACTRD(val) \
	(((val) & NFP_ELF_SHI_INITREG_CPPACT_MASK) << NFP_ELF_SHI_INITREG_CPPACTRD_LSB)
#define NFP6000_ELF_GET_SHI_INITREG_CPPTOKRD(val) \
	(((val) >> NFP_ELF_SHI_INITREG_CPPTOKRD_LSB) & NFP_ELF_SHI_INITREG_CPPTOK_MASK)
#define NFP6000_ELF_SET_SHI_INITREG_CPPTOKRD(val) \
	(((val) & NFP_ELF_SHI_INITREG_CPPTOK_MASK) << NFP_ELF_SHI_INITREG_CPPTOKRD_LSB)
#define NFP6000_ELF_GET_SHI_INITREG_CPPACTWR(val) \
	(((val) >> NFP_ELF_SHI_INITREG_CPPACTWR_LSB) & NFP_ELF_SHI_INITREG_CPPACT_MASK)
#define NFP6000_ELF_SET_SHI_INITREG_CPPACTWR(val) \
	(((val) & NFP_ELF_SHI_INITREG_CPPACT_MASK) << NFP_ELF_SHI_INITREG_CPPACTWR_LSB)
#define NFP6000_ELF_GET_SHI_INITREG_CPPTOKWR(val) \
	(((val) >> NFP_ELF_SHI_INITREG_CPPTOKWR_LSB) & NFP_ELF_SHI_INITREG_CPPTOK_MASK)
#define NFP6000_ELF_SET_SHI_INITREG_CPPTOKWR(val) \
	(((val) & NFP_ELF_SHI_INITREG_CPPTOK_MASK) << NFP_ELF_SHI_INITREG_CPPTOKWR_LSB)
#define NFP6000_ELF_GET_SHI_INITREG_ORDER(val) \
	(((val) >> NFP_ELF_SHI_INITREG_ORDER_LSB) & NFP_ELF_SHI_INITREG_ORDER_MASK)
#define NFP6000_ELF_SET_SHI_INITREG_ORDER(val) \
	(((val) & NFP_ELF_SHI_INITREG_ORDER_MASK) << NFP_ELF_SHI_INITREG_ORDER_LSB)
#define NFP_ELF_GET_SHI_INITREG_ISLAND(chip_family, val) \
	(NFP_FAMILY_IS_ARCH_TH(chip_family) ? NFP6000_ELF_GET_SHI_INITREG_ISLAND(val) : 0)
#define NFP_ELF_SET_SHI_INITREG_ISLAND(chip_family, val) \
	(NFP_FAMILY_IS_ARCH_TH(chip_family) ? NFP6000_ELF_SET_SHI_INITREG_ISLAND(val) : 0)
#define NFP_ELF_GET_SHI_INITREG_CPPTGT(chip_family, val) \
	(NFP_FAMILY_IS_ARCH_TH(chip_family) ? NFP6000_ELF_GET_SHI_INITREG_CPPTGT(val) : 0)
#define NFP_ELF_SET_SHI_INITREG_CPPTGT(chip_family, val) \
	(NFP_FAMILY_IS_ARCH_TH(chip_family) ? NFP6000_ELF_SET_SHI_INITREG_CPPTGT(val) : 0)
#define NFP_ELF_GET_SHI_INITREG_CPPACTRD(chip_family, val)   \
	(NFP_FAMILY_IS_ARCH_TH(chip_family) ?                \
		 NFP6000_ELF_GET_SHI_INITREG_CPPACTRD(val) : \
		 0)
#define NFP_ELF_SET_SHI_INITREG_CPPACTRD(chip_family, val)   \
	(NFP_FAMILY_IS_ARCH_TH(chip_family) ?                \
		 NFP6000_ELF_SET_SHI_INITREG_CPPACTRD(val) : \
		 0)
#define NFP_ELF_GET_SHI_INITREG_CPPTOKRD(chip_family, val)   \
	(NFP_FAMILY_IS_ARCH_TH(chip_family) ?                \
		 NFP6000_ELF_GET_SHI_INITREG_CPPTOKRD(val) : \
		 0)
#define NFP_ELF_SET_SHI_INITREG_CPPTOKRD(chip_family, val)   \
	(NFP_FAMILY_IS_ARCH_TH(chip_family) ?                \
		 NFP6000_ELF_SET_SHI_INITREG_CPPTOKRD(val) : \
		 0)
#define NFP_ELF_GET_SHI_INITREG_CPPACTWR(chip_family, val)   \
	(NFP_FAMILY_IS_ARCH_TH(chip_family) ?                \
		 NFP6000_ELF_GET_SHI_INITREG_CPPACTWR(val) : \
		 0)
#define NFP_ELF_SET_SHI_INITREG_CPPACTWR(chip_family, val)   \
	(NFP_FAMILY_IS_ARCH_TH(chip_family) ?                \
		 NFP6000_ELF_SET_SHI_INITREG_CPPACTWR(val) : \
		 0)
#define NFP_ELF_GET_SHI_INITREG_CPPTOKWR(chip_family, val)   \
	(NFP_FAMILY_IS_ARCH_TH(chip_family) ?                \
		 NFP6000_ELF_GET_SHI_INITREG_CPPTOKWR(val) : \
		 0)
#define NFP_ELF_SET_SHI_INITREG_CPPTOKWR(chip_family, val)   \
	(NFP_FAMILY_IS_ARCH_TH(chip_family) ?                \
		 NFP6000_ELF_SET_SHI_INITREG_CPPTOKWR(val) : \
		 0)
#define NFP_ELF_GET_SHI_INITREG_ORDER(chip_family, val) \
	(NFP_FAMILY_IS_ARCH_TH(chip_family) ? NFP6000_ELF_GET_SHI_INITREG_ORDER(val) : 0)
#define NFP_ELF_SET_SHI_INITREG_ORDER(chip_family, val) \
	(NFP_FAMILY_IS_ARCH_TH(chip_family) ? NFP6000_ELF_SET_SHI_INITREG_ORDER(val) : 0)

#define NFP_ELF_SHI_NFP_MEMTYPE_NONE 0
#define NFP_ELF_SHI_NFP_MEMTYPE_USTORE 1
#define NFP_ELF_SHI_NFP_MEMTYPE_LMEM 2
#define NFP_ELF_SHI_NFP_MEMTYPE_CLS 3
#define NFP_ELF_SHI_NFP_MEMTYPE_DRAM 4
#define NFP_ELF_SHI_NFP_MEMTYPE_MU 4
#define NFP_ELF_SHI_NFP_MEMTYPE_SRAM 5
#define NFP_ELF_SHI_NFP_MEMTYPE_GS 6
#define NFP_ELF_SHI_NFP_MEMTYPE_PPC_LMEM 7
#define NFP_ELF_SHI_NFP_MEMTYPE_PPC_SMEM 8
#define NFP_ELF_SHI_NFP_MEMTYPE_EMU_CACHE 9

#define NFP_ELF_STT_NFP_EXPR NFP_ELF_STT_LOPROC

#define NFP_ELF_R_NFP3200_NOTYPE 0
#define NFP_ELF_R_NFP3200_W32LE 1 /* For progbits */
#define NFP_ELF_R_NFP3200_SRC8_A 2
#define NFP_ELF_R_NFP3200_SRC8_B 3
#define NFP_ELF_R_NFP3200_IMMED8_I 4
#define NFP_ELF_R_NFP3200_SC 5
#define NFP_ELF_R_NFP3200_IMMED_LO16_I_A 6
#define NFP_ELF_R_NFP3200_IMMED_LO16_I_B 7
#define NFP_ELF_R_NFP3200_SRC7_B 8
#define NFP_ELF_R_NFP3200_SRC7_A 9
#define NFP_ELF_R_NFP3200_SRC8_I_B 10
#define NFP_ELF_R_NFP3200_SRC8_I_A 11
#define NFP_ELF_R_NFP3200_IMMED_HI16_I_A 12
#define NFP_ELF_R_NFP3200_IMMED_HI16_I_B 13
#define NFP_ELF_R_NFP3200_RSVD_0 14
#define NFP_ELF_R_NFP3200_RSVD_1 15
#define NFP_ELF_R_NFP3200_RSVD_2 16
#define NFP_ELF_R_NFP3200_RSVD_3 17
#define NFP_ELF_R_NFP3200_RSVD_4 18
#define NFP_ELF_R_NFP3200_RSVD_5 19
#define NFP_ELF_R_NFP3200_RSVD_6 20
#define NFP_ELF_R_NFP3200_W64LE 21     /* For progbits */
#define NFP_ELF_R_NFP3200_W32BE 22     /* For progbits */
#define NFP_ELF_R_NFP3200_W64BE 23     /* For progbits */
#define NFP_ELF_R_NFP3200_W32LE_AND 24 /* For progbits */
#define NFP_ELF_R_NFP3200_W32BE_AND 25 /* For progbits */
#define NFP_ELF_R_NFP3200_W32LE_OR 26  /* For progbits */
#define NFP_ELF_R_NFP3200_W32BE_OR 27  /* For progbits */
#define NFP_ELF_R_NFP3200_W64LE_AND 28 /* For progbits */
#define NFP_ELF_R_NFP3200_W64BE_AND 29 /* For progbits */
#define NFP_ELF_R_NFP3200_W64LE_OR 30  /* For progbits */
#define NFP_ELF_R_NFP3200_W64BE_OR 31  /* For progbits */

#define NFP_ELF_R_NFP6000_NOTYPE 0
#define NFP_ELF_R_NFP6000_W32LE 1 /* For progbits */
#define NFP_ELF_R_NFP6000_SRC8_A 2
#define NFP_ELF_R_NFP6000_SRC8_B 3
#define NFP_ELF_R_NFP6000_IMMED8_I 4
#define NFP_ELF_R_NFP6000_SC 5
#define NFP_ELF_R_NFP6000_IMMED_LO16_I_A 6
#define NFP_ELF_R_NFP6000_IMMED_LO16_I_B 7
#define NFP_ELF_R_NFP6000_SRC7_B 8
#define NFP_ELF_R_NFP6000_SRC7_A 9
#define NFP_ELF_R_NFP6000_SRC8_I_B 10
#define NFP_ELF_R_NFP6000_SRC8_I_A 11
#define NFP_ELF_R_NFP6000_IMMED_HI16_I_A 12
#define NFP_ELF_R_NFP6000_IMMED_HI16_I_B 13
#define NFP_ELF_R_NFP6000_W64LE 14 /* For progbits */
/* ElfN_Shdr.sh_info<r_offset<12:8>:r_offset<4:0>> = value */
#define NFP_ELF_R_NFP6000_SH_INFO 15
#define NFP_ELF_R_NFP6000_W32BE 16 /* For progbits */
#define NFP_ELF_R_NFP6000_W64BE 17 /* For progbits */
#define NFP_ELF_R_NFP6000_W32_29_24 18
#define NFP_ELF_R_NFP6000_W32LE_AND 19 /* For progbits */
#define NFP_ELF_R_NFP6000_W32BE_AND 20 /* For progbits */
#define NFP_ELF_R_NFP6000_W32LE_OR 21  /* For progbits */
#define NFP_ELF_R_NFP6000_W32BE_OR 22  /* For progbits */
#define NFP_ELF_R_NFP6000_W64LE_AND 23 /* For progbits */
#define NFP_ELF_R_NFP6000_W64BE_AND 24 /* For progbits */
#define NFP_ELF_R_NFP6000_W64LE_OR 25  /* For progbits */
#define NFP_ELF_R_NFP6000_W64BE_OR 26  /* For progbits */

/* NFP target revision note type */
#define NFP_ELT_NOTE_NAME_NFP "NFP\0"
#define NFP_ELT_NOTE_NAME_NFP_SZ 4
#define NFP_ELT_NOTE_NAME_NFP_USER "NFP_USR\0"
#define NFP_ELT_NOTE_NAME_NFP_USER_SZ 8
#define NFP_ELF_NT_NFP_BUILD_INFO 0x100
#define NFP_ELF_NT_NFP_REVS 0x101
#define NFP_ELF_NT_NFP_MIP_LOCATION 0x102
/* The format of user note descriptions must be:
 * <unique_name>0x00{pad}<content>
 * Padding is inserted until <content> starts on a 4-byte
 * aligned offset.
 */
#define NFP_ELF_NT_NFP_USER 0xf0000000
#define NFP_ELF_IMPORT_EXPR_FIELD_CNT 4
#define NFP_ELF_FILE_ALIGN 8

/* These values currently fit into a 3 bit field. */
#define NFP_ELF_INITREG_VALTYPE_FORCE 0 /* Linker/Loader only. */
#define NFP_ELF_INITREG_VALTYPE_CONST 1
#define NFP_ELF_INITREG_VALTYPE_REQUIRED 2
#define NFP_ELF_INITREG_VALTYPE_VOLATILE_INIT 3
#define NFP_ELF_INITREG_VALTYPE_VOLATILE_NOINIT 4
#define NFP_ELF_INITREG_VALTYPE_INVALID 5

/* ==== Standard ELF structures ==== */
struct nfp_elf_Elf64_Ehdr {
	uint8_t e_ident[NFP_ELF_EI_NIDENT];
	uint16_t e_type;
	uint16_t e_machine;
	uint32_t e_version;
	uint64_t e_entry;
	uint64_t e_phoff;
	uint64_t e_shoff;
	uint32_t e_flags;
	uint16_t e_ehsize;
	uint16_t e_phentsize;
	uint16_t e_phnum;
	uint16_t e_shentsize;
	uint16_t e_shnum;
	uint16_t e_shstrndx;
};

struct nfp_elf_Elf64_Shdr {
	uint32_t sh_name;
	uint32_t sh_type;
	uint64_t sh_flags;
	uint64_t sh_addr;
	uint64_t sh_offset;
	uint64_t sh_size;
	uint32_t sh_link;
	uint32_t sh_info;
	uint64_t sh_addralign;
	uint64_t sh_entsize;
};

struct nfp_elf_Elf64_Sym {
	uint32_t st_name;
	uint8_t st_info;
	uint8_t st_other;
	uint16_t st_shndx;
	uint64_t st_value;
	uint64_t st_size;
};

struct nfp_elf_Elf64_Rel {
	uint64_t r_offset;
	uint64_t r_info;
};

struct nfp_elf_Elf64_Nhdr {
	uint32_t n_namesz;
	uint32_t n_descsz;
	uint32_t n_type;
};

/* ==== NFP specific structures ==== */

struct nfp_elf_Elf_meconfig {
	uint32_t ctx_enables;
	uint32_t entry;
	uint32_t misc_control;
	uint32_t reserved;
};

#define NFP_ELF_INITREGENTRY_WO_NLW(v) (((v) & 0x1) << 31)
#define NFP_ELF_INITREGENTRY_WO_CTX(v) (((v) & 0x7) << 28)
#define NFP_ELF_INITREGENTRY_WO_VTP(v) (((v) & 0x7) << 25)
#define NFP_ELF_INITREGENTRY_WO_COH(v) (((v) & 0xFF) << 0)

#define NFP_ELF_INITREGENTRY_WO_NLW_of(_w0_) (((_w0_) >> 31) & 0x1)
#define NFP_ELF_INITREGENTRY_WO_CTX_of(_w0_) (((_w0_) >> 28) & 0x7)
#define NFP_ELF_INITREGENTRY_WO_VTP_of(_w0_) (((_w0_) >> 25) & 0x7)
#define NFP_ELF_INITREGENTRY_WO_COH_of(_w0_) (((_w0_) >> 0) & 0xFF)

#define NFP_ELF_INITREGENTRY_W0(_not_last_word_, _ctx_, _valtype_, _cpp_offset_hi_)    \
	(NFP_ELF_INITREGENTRY_WO_NLW(_not_last_word_) |                                \
	 NFP_ELF_INITREGENTRY_WO_CTX(_ctx_) | NFP_ELF_INITREGENTRY_WO_VTP(_valtype_) | \
	 NFP_ELF_INITREGENTRY_WO_COH(_cpp_offset_hi_))

struct nfp_elf_Elf_initRegEntry {
	uint32_t w0;
	uint32_t cpp_offset_lo;
	uint32_t val;
	uint32_t mask;
};

struct nfp_elf_impexpr_field {
	uint8_t count;
	uint8_t r_type;
	uint8_t word_size;
	uint8_t big_endian;
	char op;
	uint8_t fields[NFP_ELF_IMPORT_EXPR_FIELD_CNT * 3];
};

/* ==== NFP NFFW ELF struct and API ==== */

struct nfp_elf_user_note {
	const char *name;
	uint32_t data_sz;
	void *data;
};

/** nfp_elf_fw_mip contains firmware related fields from the MIP as well as the
 *  MIP location in the NFFW file. All fields are only valid if shndx > 0.
 *
 * This struct will only be available if the firmware contains a .note section
 * with a note of type NFP_ELF_NT_NFP_MIP_LOCATION.
 */
struct nfp_elf_fw_mip {
	size_t shndx;
	uint64_t sh_offset;
	uint32_t mip_ver; /* Version of the format of the MIP itself */

	uint32_t fw_version;
	uint32_t fw_buildnum;
	uint32_t fw_buildtime;
	char fw_name[20]; /* At most 16 chars, 17 ensures '\0', round up */
};

/** It is preferred to access this struct via the nfp_elf functions
 *  rather than directly.
 */
struct nfp_elf {
	struct nfp_elf_Elf64_Ehdr *ehdr;
	struct nfp_elf_Elf64_Shdr *shdrs;
	size_t shdrs_cnt;
	void **shdrs_data;

	/* True if section data has been endian swapped */
	uint8_t *shdrs_host_endian;

	size_t shdr_idx_symtab;

	struct nfp_elf_Elf64_Sym *syms;
	size_t syms_cnt;

	char *shstrtab;
	size_t shstrtab_sz;

	char *symstrtab;
	size_t symstrtab_sz;

	/* For NFP-32xx NFFW files only */
	struct nfp_elf_Elf_meconfig *meconfs;
	size_t meconfs_cnt;

	/* ==== .note data start ==== */
	/* Following data derived from SHT_NOTE sections for read-only usage.
	 * These fields are not used in nfp_elf_to_buf()
	 */
	int rev_min; /* -1 if file did not specify */
	int rev_max; /* -1 if file did not specify */

	/* If mip_shndx == 0 and mip_sh_off == 0, the .note stated there is no MIP.
	 * If mip_shndx == 0 and mip_sh_off == UINT64_MAX, there was no .note and
	 * a MIP _may_ still be found in the first 256KiB of DRAM/EMEM data.
	 */
	size_t mip_shndx;    /* Section in which MIP resides, 0 if no MIP */
	uint64_t mip_sh_off; /* Offset within section (not address) */

	struct nfp_elf_fw_mip fw_mip;
	/* ==== .note data end ==== */

	/* ==== .note.user data start ==== */
	size_t user_note_cnt;
	struct nfp_elf_user_note *user_notes;
	/* ==== .note.user data start ==== */

	void *dbgdata;

	int family;

	/* For const entry points in the API, we allocate and keep a buffer
	 * and for mutable entry points we assume the buffer remains valid
	 * and we just set pointers to it.
	 */
	void *_buf;
	size_t _bufsz;
};

NFP_COMMON_API
struct nfp_elf *nfp_elf_from_file(const char *filename);

NFP_COMMON_API
struct nfp_elf *nfp_elf_from_buf(const void *buf, size_t buf_len);

NFP_COMMON_API
struct nfp_elf *nfp_elf_mutable_buf(void *buf, size_t buf_len);

NFP_COMMON_API
int nfp_elf_to_file(struct nfp_elf *ectx, const char *filename);

NFP_COMMON_API
size_t nfp_elf_to_buf(struct nfp_elf *ectx, void **buf_out);

NFP_COMMON_API
size_t nfp_elf_borrow_buf(struct nfp_elf *ectx, void **buf_out);

NFP_COMMON_API
void nfp_elf_free(struct nfp_elf *ectx);

NFP_COMMON_API
int nfp_elf_strip_debug(struct nfp_elf *ectx);

NFP_COMMON_API
int nfp_elf_arch(struct nfp_elf *ectx);

NFP_COMMON_API
int nfp_elf_family(struct nfp_elf *ectx);

NFP_COMMON_API
int nfp_elf_is_elf64(struct nfp_elf *ectx);

NFP_COMMON_API
int nfp_elf_is_exec(struct nfp_elf *ectx);

NFP_COMMON_API
int nfp_elf_rev_min(struct nfp_elf *ectx);

NFP_COMMON_API
int nfp_elf_rev_max(struct nfp_elf *ectx);

NFP_COMMON_API
int nfp_elf_mip_note(struct nfp_elf *ectx,
		     size_t *mip_shndx,
		     uint64_t *mip_sh_offset);

NFP_COMMON_API
const struct nfp_elf_fw_mip *nfp_elf_mip(struct nfp_elf *ectx);

NFP_COMMON_API
struct nfp_elf_Elf64_Ehdr *nfp_elf_filehdr(struct nfp_elf *ectx);

NFP_COMMON_API
size_t nfp_elf_sections(struct nfp_elf *ectx,
			struct nfp_elf_Elf64_Shdr **shdrs);

NFP_COMMON_API
size_t nfp_elf_symbols(struct nfp_elf *ectx,
		       struct nfp_elf_Elf64_Sym **syms);

NFP_COMMON_API
int nfp_elf_sec_has_reloc(struct nfp_elf *ectx, size_t secidx);

NFP_COMMON_API
int nfp_elf_resolve(struct nfp_elf *ectx, int allow_undefined_sym);

NFP_COMMON_API
int nfp_elf_sec_is_text(struct nfp_elf *ectx, size_t secidx);

NFP_COMMON_API
int nfp_elf_sec_is_init_text(struct nfp_elf *ectx, size_t secidx);

NFP_COMMON_API
int nfp_elf_sym_is_import_var(struct nfp_elf *ectx,
			      const struct nfp_elf_Elf64_Sym *sym);

NFP_COMMON_API
int nfp_elf_import_var_is_defined(struct nfp_elf *ectx,
				  const struct nfp_elf_Elf64_Sym *sym);

NFP_COMMON_API
int nfp_elf_import_var_undefine(struct nfp_elf *ectx,
				struct nfp_elf_Elf64_Sym *sym);

NFP_COMMON_API
int nfp_elf_import_var_set(struct nfp_elf *ectx,
			   struct nfp_elf_Elf64_Sym *sym,
			   uint64_t val);

NFP_COMMON_API
struct nfp_elf_Elf64_Sym *nfp_elf_find_sym(struct nfp_elf *ectx,
					   const char *name);

NFP_COMMON_API
size_t nfp_elf_data(struct nfp_elf *ectx, size_t secidx, void **data);

/* For NFP-32xx firmware only */
NFP_COMMON_API
struct nfp_elf_Elf_meconfig *nfp_elf_meconf(struct nfp_elf *ectx,
					    int meid);

NFP_COMMON_API
int nfp_elf_me_has_text(struct nfp_elf *ectx, int meid);

NFP_COMMON_API
int nfp_elf_me_has_init_text(struct nfp_elf *ectx, int meid);

NFP_COMMON_API
void nfp_elf_ustore_ecc_calculate_and_set_bits(struct nfp_elf *ectx,
					       uint64_t *word);

NFP_COMMON_API
const char *nfp_elf_sec_name(struct nfp_elf *ectx, size_t secidx);

NFP_COMMON_API
const char *nfp_elf_sym_name(struct nfp_elf *ectx,
			     const struct nfp_elf_Elf64_Sym *sym);

NFP_COMMON_API
size_t nfp_elf_sec_initreg_cnt(struct nfp_elf *ectx, size_t secidx);

NFP_COMMON_API
size_t nfp_elf_sec_initregs(struct nfp_elf *ectx,
			    size_t secidx,
			    const struct nfp_elf_Elf_initRegEntry **entries);

NFP_COMMON_API
size_t nfp_elf_impexpr_fields(int chip_family,
			      const struct nfp_elf_impexpr_field **fields);

/** Find a NFP_ELF_NT_NFP_USER note with a given user-note name.
 *
 * @param name  The name of the note, which is at the start of the note
 *              description and not the ELF note name.
 * @param dsz   Will be set to the size of the note data. If the note has no
 *              data, @p dsz will be set to 0. Can be NULL.
 * @param data  Will be set to the note data if the note is found and has data.
 *              Can be NULL.
 *
 * @return  Returns 0 if the note was found, -1 if an error occurred and
 *          +1 if the note was not found.
 */
NFP_COMMON_API
int nfp_elf_find_user_note(struct nfp_elf *ectx,
			   const char *name,
			   uint32_t *dsz,
			   const void **data);

NFP_COMMON_API
size_t nfp_elf_user_notes(struct nfp_elf *ectx,
			  const struct nfp_elf_user_note **user_notes);

__END_DECLS

#endif /* __NFP_ELF_H__ */
