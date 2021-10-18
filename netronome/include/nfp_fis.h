/*
 * Copyright (C) 2010-2011,  Netronome Systems, Inc.  All rights reserved.
 *
 * @file          nfp_fis.h
 * @brief         Interface for NFP SPI device access and query functions.
 *
 */
#ifndef __NFP_FIS_H__
#define __NFP_FIS_H__

#ifdef __KERNEL__
#else
#include <stdint.h>
#include <sys/types.h>
#include <sys/cdefs.h>
#endif

#include <nfp.h>
#include <nfp_flash.h>

/* API is part of libnfp */
#include "_nfp_apiex.h"

/**
 * Structure describing a Netronome extension tag
 */
struct nfp_fis_netro_ext_tag {
	uint8_t type;
	char *data;
};

/**
 * Structure describing Netronome extensions to a RedBoot FIS flash directory entry
 */
struct nfp_fis_netro_ext {
	uint32_t magic;
	uint32_t image_timestamp;   /* Seconds since Jan 1, 2000 UCT */
	uint32_t updated_timestamp; /* Seconds since Jan 1, 2000 UCT */
	int tags;
	struct nfp_fis_netro_ext_tag *tag;
};

/**
 * Initializes a netro_ext tag list
 *
 * @param	tl	nfp_fis_netro_ext handle
 *
 * @return	0 on success, or -1 if error.
 */
NFP_API
int netro_ext_tag_init(struct nfp_fis_netro_ext *tl);

/**
 * Add/update a netro_ext tag
 *
 * @param	tl	nfp_fis_netro_ext handle
 * @param	id	tag type
 * @param	data	tag data
 *
 * @return	0 on success, or -1 if error.
 */
NFP_API
int netro_ext_tag_set(struct nfp_fis_netro_ext *tl, uint8_t id, const char *data);

/**
 * Free/release a netro_ext tag list and data
 *
 * @param	tl	nfp_fis_netro_ext handle
 *
 * @return	0 on success, or -1 if error.
 */
NFP_API
int netro_ext_tag_free(struct nfp_fis_netro_ext *tl);

/**
 * Merges two netro_ext tag lists, the current list will be replaced
 *  with the union, any overlaps will use new data. free_cur controls
 *  whether existing list is to be automatically freed, or left to the caller
 *  The caller must always free/manage newl
 *
 * @param	curl	  current/target tag list
 * @param	newl	  new tag list to overlay on existing
 * @param	free_cur  free curl
 *
 * @return	0 on success, or -1 if error.
 */
NFP_API
int netro_ext_tag_merge(struct nfp_fis_netro_ext *curl,
			struct nfp_fis_netro_ext *newl,
			int free_cur);

/**
 * Structure describing a RedBoot FIS flash directory entry
 */

struct nfp_fis_entry {
	int slot; /* 256-byte slot into the "FIS Directory" block */
	char name[17];
	uint32_t block_size;  /* Size of an erase block */
	uint32_t flash_block; /* Offset into flash in erase blocks */
	uint32_t flash_size;  /* Size in erase blocks */
	uint32_t image_size;  /* Size in bytes */
	uint32_t image_csum;  /* CRC32-GB of the image */
	uint32_t load_addr;   /* Address to load image to */
	uint32_t entry_addr;  /* Addresses to execute at */
	struct nfp_fis_netro_ext netro_ext;
};

/**
 * Handle to a RedBoot FIS partitioned flash
 */

struct nfp_fis;

/**
 * Open a handle to a RedBoot FIS partitioned flash
 *
 * @param	flash	NFP flash handle
 * @param	nfp_fis_block	Block where the FIS directory starts
 *
 * Creates and returns a handle for managing the FIS partition of a
 * NFP flash
 *
 * The nfp_fis_block parameter is the 0-based block number to scan
 * for a RedBoot FIS partition map.
 *
 * Conventionally, nfp_fis_block is set to 1 (to specify the 2nd
 * block in the SPI Flash, immediately after the '(reserved') miniloader.bin
 * area) when calling this function.
 *
 * @return	Pointer to a FIS handle, or NULL if error.
 */
NFP_API
struct nfp_fis *nfp_fis_open(struct nfp_flash *flash, unsigned int nfp_fis_block);

/**
 * Close a RedBoot FIS partition handle
 *
 * @param	nfp_fis	FIS structure pointer
 * @param	commit	0 = don't commit changes, 1 = commit changes
 *
 * If commit is 0, no changes will be performed to the FIS Directory.
 *
 */
NFP_API
void nfp_fis_close(struct nfp_fis *nfp_fis, int commit);

/**
 * Get the underlying flash of the FIS handle
 *
 * @param	nfp_fis	FIS structure pointer
 *
 * Return struct nfp_flash, or NULL
 */
NFP_API
struct nfp_flash *nfp_fis_flash(struct nfp_fis *nfp_fis);

/**
 * Get last error message from the FIS library routines as a string
 *
 * @param	nfp_fis	FIS structure pointer
 *
 * @return	NULL if no error, otherwise string of error message
 */
NFP_API
const char *nfp_fis_strerror(struct nfp_fis *nfp_fis);

/**
 * Erase all RedBoot FIS information
 *
 * @param	nfp_fis	FIS structure pointer
 * @param	nfp_fis_block	Block where the FIS directory starts
 * @param	flash_base	Address to use as the RedBoot flash offset
 * @param	secure	Erase all information after the FIS.
 *
 * Does not erase any other portion of the flash.
 *
 * Conventionally, nfp_fis_block is set to 1 (to specify the 2nd
 * block in the SPI Flash, immediately after the '(reserved') miniloader.bin
 *
 * The flash_base parameter should be set to the same location as RedBoot's
 * mapping of the SPI Flash (ie 0xc4000000), or 0.
 *
 * @return	Pointer to a FIS handle, or NULL if error.
 */
NFP_API
int nfp_fis_init(struct nfp_fis *nfp_fis,
		 unsigned int nfp_fis_block,
		 uint32_t flash_base,
		 int secure);

/**
 * Return the total size of the flash
 *
 * @param	nfp_fis	FIS structure pointer
 *
 * @return      size    Total size (in bytes) of the flash
 */
NFP_API
int nfp_fis_flash_size(struct nfp_fis *nfp_fis);

/**
 * Return the block size of the flash
 *
 * @param	nfp_fis	FIS structure pointer
 *
 * @return      size    Block size (in bytes) of the flash
 */
NFP_API
int nfp_fis_block_size(struct nfp_fis *nfp_fis);

/**
 * Get the next (or first)  RedBoot FIS directory entry
 *
 * @param	nfp_fis	FIS structure pointer
 *
 * @param	prev	Previous FIS. Use NULL to get the first entry.
 */
NFP_API
const struct nfp_fis_entry *nfp_fis_next(struct nfp_fis *nfp_fis, const struct nfp_fis_entry *prev);

/**
 * Find a RedBoot FIS directory entry by name.
 *
 * @param	nfp_fis	FIS structure pointer
 * @param	name	FIS dirent name
 *
 * @return		FIS dirent, or NULL
 */
NFP_API
const struct nfp_fis_entry *nfp_fis_find(struct nfp_fis *nfp_fis, const char *name);

/**
 * Create a RedBoot FIS directory entry.
 *
 * @param	nfp_fis	FIS structure pointer
 * @param	de	FIS dirent template
 * @param	image	Pointer to image data (can be NULL)
 *
 * The new directory entry in the RedBoot FIS table is based upon
 * the template de passed in.
 *
 * To auto-allocate a directory entry, ensure that de->slot=0,
 * de->flash_offset=0, and de->flash_size=0.
 *
 * On success, the de template will be modified with the
 * allocated slot, flash_offset, and flash_size.
 *
 * @return	0 on success, -errno on failure.
 */
NFP_API
int nfp_fis_create(struct nfp_fis *nfp_fis, struct nfp_fis_entry *de, void *image);

/**
 * Create a RedBoot FIS directory entry.
 *
 * @param	nfp_fis	    FIS structure pointer
 * @param	fisname	    Name for new FIS dirent
 * @param	load_addr   Specifies load address, ~0 for default
 * @param	entry_addr  Specifies entry address, ~0 for default
 * @param	flash_block Specify flash block, ~0 to auto select
 * @param	image	    Pointer to image data (can be NULL)
 * @param	isize       Length of image buffer
 * @param	fsize       Length of flash region, allows allocating additional
 *                           flash blocks beyond those needed to fit image.
 *                           set to 0 for default/auto sizing.
 * @param	itime       image timestamp
 * @param	utime       fis update timestamp
 * @param	tl          Tag list to overlay on existing, may be NULL
 * @param	de_only     Only create FIS dir entry, don't write image
 *
 * set load_addr or entry_addr to ~0 for default.
 * set flash_block to ~0 to auto-allocate a directory entry
 *
 * @return	0 on success, -errno on failure.
 */
NFP_API
int nfp_fis_create_no_template(struct nfp_fis *nfp_fis,
			       const char *fisname,
			       uint32_t load_addr,
			       uint32_t entry_addr,
			       uint32_t flash_block,
			       void *image,
			       uint32_t isize,
			       uint32_t fsize,
			       time_t itime,
			       time_t utime,
			       struct nfp_fis_netro_ext *tl,
			       int de_only);

/**
 * Remove a RedBoot FIS directory entry
 *
 * @param	nfp_fis	FIS structure pointer
 * @param	name	FIS dirent name
 * @param	secure	Erase the existing contents of the dirent
 *
 * If secure is 1, the flash region pointed to by the dirent
 * will be erased, along with the directory entry.
 *
 * @return	0 on success, -errno on failure.
 */
NFP_API
int nfp_fis_delete(struct nfp_fis *nfp_fis, const char *name, int secure);

/**
 * Erase the area referenced by a RedBoot FIS directory entry
 *
 * @param	nfp_fis	FIS structure pointer
 * @param	name	FIS dirent name
 *
 * This will also set the dirent's image size to 0.
 *
 * @return	0 on success, -errno on failure.
 */
NFP_API
int nfp_fis_erase(struct nfp_fis *nfp_fis, const char *name);

/**
 * Read the area in flash referenced by a RedBoot FIS directory entry
 *
 * @param	nfp_fis	FIS structure pointer
 * @param	name	FIS dirent name
 * @param	buffer  Buffer to read into
 * @param	size    Length of buffer
 *
 * @return	size on success, -errno on failure.
 */

NFP_API
int nfp_fis_read(struct nfp_fis *nfp_fis, const char *name, void *buffer, size_t size);

/**
 * Peek into the area in flash referenced by a RedBoot FIS directory entry
 *
 * @param	nfp_fis	FIS structure pointer
 * @param	name	FIS dirent name
 * @param	buffer  Buffer to read into
 * @param	size    Length of buffer
 * @param       offset  Offset into the partition
 * @param       length  Length of read
 *
 * @return	0 on success, -errno on failure.
 */
NFP_API
int nfp_fis_peek(struct nfp_fis *nfp_fis, const char *name, void *buffer, size_t size, size_t offset, size_t length);

/**
 *  Over/re-write the area in flash referenced by an existing RedBoot FIS
 *  directory entry
 *
 * @param	nfp_fis	    FIS structure pointer
 * @param	name	    FIS dirent name
 * @param	secure	    Erase the existing contents of the dirent
 * @param	load_addr   force load address, set to ~0 for default
 * @param	entry_addr  force entry address, set to ~0 for default
 * @param	buffer      Buffer to write from
 * @param	size        Length of buffer
 * @param	itime       image timestamp
 * @param	utime       fis update timestamp
 * @param	tl          Tag list to overlay on existing, may be NULL
 *
 * @return	size on success, -errno on failure.
 */
NFP_API
int nfp_fis_write(struct nfp_fis *nfp_fis,
		  const char *fisname,
		  int secure,
		  uint32_t load_addr,
		  uint32_t entry_addr,
		  void *buffer,
		  uint32_t size,
		  time_t itime,
		  time_t utime,
		  struct nfp_fis_netro_ext *tl);

/**
 * Resize the flash area referenced by a RedBoot FIS directory entry
 *
 * @param	nfp_fis	FIS structure pointer
 * @param	name	FIS dirent name
 * @param	flash_blocks	New block size
 *
 * To succeed, flash_blocks * block_size must be greater than or
 * equal to image_size, and there must be a contiguous free area
 * following the RedBoot FIS entry.
 *
 * @return	0 on success, -errno on failure.
 */
NFP_API
int nfp_fis_resize(struct nfp_fis *nfp_fis, const char *name, int flash_blocks);

/**
 * Rename a RedBoot FIS directory entry
 *
 * @param	nfp_fis	FIS structure pointer
 * @param	name	Old name
 * @param	new_name	New name
 *
 * @return	0 on success, -errno on failure.
 */
NFP_API
int nfp_fis_rename(struct nfp_fis *nfp_fis, const char *name, const char *new_name);

/**
 * Typedef for progress callbacks
 *
 * @param	priv	Private data
 * @param	name	Name of FIS entry being read/modified (NULL for dir ops)
 * @param	total	Total bytes that will be modified
 * @param	pos	Current location
 * @param	action	'R'ead, 'W'rite, 'V'erify, 'E'rase
 */
typedef void (*nfp_fis_progress_t)(void *priv,
				   const char *name,
				   off_t total,
				   off_t pos,
				   char action);

/**
 * Register a callback for printing progress of FIS operations
 *
 * @param	nfp_fis	FIS structure pointer
 * @param	func	callback function (NULL to remove)
 * @param	priv	private data for the callback function
 */
NFP_API
void nfp_fis_callback_progress(struct nfp_fis *nfp_fis, nfp_fis_progress_t func, void *priv);

__END_DECLS;

#endif /* !__NFP_FIS_H__ */
