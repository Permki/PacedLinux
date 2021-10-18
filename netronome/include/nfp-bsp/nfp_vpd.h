/*
 * Copyright (C) 2010, Netronome Systems, Inc.  All rights reserved.
 *
 */

#ifndef NFP_VPD_H
#define NFP_VPD_H

#include <stdint.h>

#define VPD_MAGIC(a, b, c, d)	(((a) << 24) | ((b) << 16) | ((c) << 8) | (d))

/* VPD v1 Layout:
 *
 * 0x0000..0x0100 v1 Header
 * 0x0100..0x0104 v1 Header CRC32 (BE)
 */

union vpd_v1_header {
	uint8_t raw[256];
	struct {
		const char assembly_model[16];	/* 0x00 */
		const char assembly_part[16];	/* 0x10 */
		const char assembly_serial[16];	/* 0x20 */
		const char assembly_vendor[16];	/* 0x30 */
		const char enghw_rev[16];	/* 0x40 */
		const char nfp0_serial[16];	/* 0x50 */
		const char nfp0_part[16];	/* 0x60 */
		const char assembly_rev[16];	/* 0x70 */
		const char nfp1_serial[16];	/* 0x80 */
		const char nfp1_part[16];	/* 0x90 */
	} hdr;
};

/* VPD v2 Layout
 * 0x0200 v2 Header
 * 0x0300... v2 Data
 */

#define VPD_V2_MAGIC	VPD_MAGIC('N', 'F', 'P', 0)
#define VPD_V2_TYPE	VPD_MAGIC('V', 'P', 'D', 0)

#define VPD_ENTRY_INVALID	0xff
#define VPD_ENTRY_VPD_V1	0x01	/* V1 header */
#define VPD_ENTRY_VPD_V2	0x02	/* Self-reference */
#define VPD_ENTRY_HWINFO	0x20	/* HWInfo table */
#define VPD_ENTRY_MAC_MGMT	0x21	/* Management MAC address */
#define VPD_ENTRY_MAC_ETH(x)	(0x30 + ((x) & 0xf))	/* Interface MAC address */
#define VPD_ENTRY_IPMI_FRU	0x40	/* IPMI FRU */
#define VPD_ENTRY_DDR0_SPD	0x50    /* NFP3200 MU, DDR0 */
#define VPD_ENTRY_DDR1_SPD	0x51    /* NFP3200 MU, DDR1 */
#define VPD_ENTRY_EMU0_DDR0_SPD	0x51	/* NFP6000 EMU0, DDR0 */
#define VPD_ENTRY_EMU0_DDR1_SPD	0x55	/* NFP6000 EMU0, DDR1 */
#define VPD_ENTRY_EMU1_DDR0_SPD	0x52	/* NFP6000 EMU1, DDR2 */
#define VPD_ENTRY_EMU1_DDR1_SPD	0x56	/* NFP6000 EMU1, DDR2 */
#define VPD_ENTRY_EMU2_DDR0_SPD	0x53	/* NFP6000 EMU2, DDR3 */
#define VPD_ENTRY_EMU2_DDR1_SPD	0x57	/* NFP6000 EMU2, DDR3 */
#define VPD_ENTRY_OEM		0xc0

/* OEMs should define VPD entry tags from 0xc0..0xfe */

/* NOTE: All words are big-endian!
 *       hton*() and ntoh*() macros are useful here.
 */
union vpd_v2_header {
	uint8_t raw[256];
	struct {
		uint32_t magic;	/* Should be VPD_V2_MAGIC */
		uint32_t type;	/* Should be VPD_V2_TYPE */
		uint8_t resv_0x08[2];	/* Reserved, should be zero */
		uint16_t length;	/* Length of entire VPD, in bytes */
		uint32_t crc32;	/* Posix CRC32 from 0x40 - 0xff */
		uint8_t resv_0x10[48];	/* Reserved, should be zero */
		struct {
			uint8_t type;	/* PTE type */
			uint8_t blocks;	/* Number of 16-byte blocks */
			uint16_t block;	/* Absolute 16-byte block # */
		} entry[48];
	} hdr;
};

#endif /* NFP_VPD_H */
