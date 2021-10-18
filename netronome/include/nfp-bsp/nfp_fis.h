/*
 * Copyright (C) 2011, Netronome Systems, Inc.  All rights reserved.
 *
 */

#ifndef NFP_FIS_H
#define NFP_FIS_H

#ifndef _MSC_VER
#define NFP_FIS_PACKED	__attribute__((packed))
#else
#define NFP_FIS_PACKED
#endif

#include <nfp.h>

/* NOTE: All entries are little-endian */
#ifdef _MSC_VER
#pragma pack(push, 1)
#endif

struct NFP_FIS_PACKED fis_dirent {
	char name[16];		/* Null terminated name */
	uint32_t flash_offset;	/* Address within FLASH of image */
	uint32_t load_addr;		/* Address in memory to load to */
	uint32_t flash_size;		/* Length of actual data */
	uint32_t entry_addr;		/* Execution entry point */
	uint32_t image_size;		/* Length of image */
	union {
		uint8_t fill[256 - ((5 + 2) * sizeof(uint32_t) + 16)];
		struct fis_ext_netro {
			uint32_t magic;	/* Netronome Magic */
			uint32_t image_timestamp;	/* Image's original timestmap, in */
			/* seconds since Jan 1, 2000 UCT */
			uint32_t updated_timestamp;	/* Last update to this partition, */
			/* in seconds since Jan 1, 2000 UCT */
			uint32_t resv;	/* Padding. Must be 0xffffffff */
#ifndef _MSC_VER
			uint8_t data[];	/* (type,ASCIIZ string) tuples */
#else
			/* MSC does not support empty arrays at end of struct */
			uint8_t data[1];	/* (type,ASCIIZ string) tuples */
#endif
		} netro;
	} ext;
	uint32_t csum;		/* Checksum over this entry */
	uint32_t image_csum;		/* Checksum over image data */
};

#ifdef _MSC_VER
#pragma pack(pop)
#endif

#define FIS_EXT_NETRO_MAGIC	(0x32400000)

/* Jan 1, 2000 UCT to Jan 1, 1970 UCT timestamp conversion */
#define FIS_EXT_NETRO_TIMESTAMP_TO_UNIX(x)	((x) + 946684800ULL)
#define FIS_EXT_NETRO_UNIX_TO_TIMESTAMP(x)	((x) - 946684800)

/* Well-known types for the ext.netro.data field
 *
 * Types 0x01-0x7F are available for customer use.
 * Types 0x80 and above are reserved for Netronome definition.
 *
 * Type 0x00 is reserved, and must not be used.
 */
#define FIS_EXT_NETRO_TYPE_RELEASE	(0x80 | 'R')
#define FIS_EXT_NETRO_TYPE_VERSION      (0x80 | 'V')

#define FIS_EXT_NETRO_TYPE_END          0xff

#endif /* NFP_FIS_H */
