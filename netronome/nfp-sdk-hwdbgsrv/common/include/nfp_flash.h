/*
 * Copyright (C) 2014,2017 Netronome Systems, Inc.
 * All right reserved.
 *
 */

#ifndef NFP_FLASH_H
#define NFP_FLASH_H

#include <nfp.h>

/* API is part of libnfp */
#include "_nfp_apiex.h"

struct nfp_flash_info {
	struct {
		uint8_t manufacturer;
		uint8_t device_id[2];
		uint8_t device_ext[2];
	} rdid; /* JEDEC Command 0x9f */
	struct {
		uint8_t manufacturer;
		uint8_t device;
	} read_id;		 /* JEDEC Command 0x90 */
	unsigned int block_size; /* Erase block size */
	unsigned int blocks;     /* Total blocks in the device */
};

/**
 * Handle to a NFP Flash device
 */
struct nfp_flash;

/**
 * Open a nfp_flash SPI Flash device for read/write
 *
 * @param dev           NFP interface
 * @param spi_bus       SPI Flash Bus (0..n)
 * @param spi_cs        SPI Flash Chip Select (0..n)
 * @return              NFP Flash handle
 */
NFP_API
struct nfp_flash *nfp_flash_open_spi(struct nfp_device *dev, int spi_bus, int spi_cs);

/**
 * Open a nfp_flash file image for read/write
 *
 * @param file          Image file path (image must already exist)
 * @param block_size    Image file block size (0 = default of 64K)
 * @return              NFP Flash handle
 */
NFP_API
struct nfp_flash *nfp_flash_open_file(const char *file, size_t block_size);

/**
 * Open a nfp_flash memory image for read/write
 *
 * @param image         Image memory buffer (must already exist)
 * @param size          Size of image memory buffer
 * @param block_size    Image file block size (0 = default of 64K)
 * @return              NFP Flash handle
 */
NFP_API
struct nfp_flash *nfp_flash_open_buffer(void *image, size_t size, size_t block_size);

/**
 * Close a nfp_flash handle
 *
 * @param flash         NFP Flash handle
 */
NFP_API
void nfp_flash_close(struct nfp_flash *flash);

/**
 * Get information about the SPI Flash
 *
 * @param flash         NFP Flash handle
 * @param info          SPI Information structure
 *
 * Retrieves information about the SPI Flash on SPI Chip Select cs.
 * The info parameter should point to an allocated, cleared-to-zero
 * area of memory.
 *
 * @return 0 on success, or -1 on error (and set errno accordingly).
 */
NFP_API
int nfp_flash_info(struct nfp_flash *flash, struct nfp_flash_info *info);

/**
 * Erases one or more flash blocks on the SPI Flash on SPI Chip Select cs.
 *
 * @param flash         NFP Flash handle
 * @param offset        Offset into flash to erase
 * @param len           Length to erase
 *
 * The offset and len must be block-size aligned - use
 * the nfp_flash_info() routine to determine the block size of the
 * flash.
 *
 * @return 0 on success, or -1 on error (and set errno accordingly).
 */
NFP_API
int nfp_flash_erase(struct nfp_flash *flash, unsigned int offset, unsigned int len);

/**
 * Program SPI Flash
 *
 * @param flash         NFP Flash handle
 * @param buff          Data to program into the SPI Flash
 * @param offset        Offset into flash to program
 * @param len           Length to program
 *
 * The byte range (offset, len) in the flash will be programmed
 * to the values in buff.
 *
 * The offset and len may address any byte addressable location
 * in the flash.
 *
 * Please note that this routine expects that the region to be programmed
 * has already been erased.
 *
 * @return 0 on success, or -1 on error (and set errno accordingly).
 */
NFP_API
int nfp_flash_program(struct nfp_flash *flash,
		      unsigned int offset,
		      const void *buff,
		      size_t len);

/**
 * Read SPI Flash
 *
 * @param flash         NFP Flash handle
 * @param buff          Buffer to receive the SPI Flash data
 * @param offset        Offset into flash to read from
 * @param len           Length to read
 *
 * The byte range (offset, len) of the flash image is read into buff.
 *
 * The offset and len may address any byte addressable location
 * in the flash.
 *
 * @return 0 on success, or -1 on error (and set errno accordingly).
 */
NFP_API
int nfp_flash_read(struct nfp_flash *flash, unsigned int offset, void *buff, size_t len);

/**
 * Typedef for progress callbacks
 *
 * @param	priv	Private data
 * @param	pos	Current location
 * @param	action	'R'ead, 'W'rite, 'V'erify, 'E'rase
 */
typedef void (*nfp_flash_progress_t)(void *priv, nfp_off64_t pos, char action);

/**
 * Register a callback for printing progress of flash operations
 *
 * @param	flash	NFP flash structure pointer
 * @param	func	callback function (NULL to remove)
 * @param	priv	private data for the callback function
 */
NFP_API
void nfp_flash_callback_progress(struct nfp_flash *flash,
				 nfp_flash_progress_t func,
				 void *priv);

typedef void (*nfp_flash_progress_callback)(int, int, char);

struct nfp_flash_write_parameters {
	const uint8_t *fullbuf;
	size_t buf_pos;
	size_t buf_size;
	struct nfp_device *nfp;
	struct nfp_flash *flash;
	int block_size;
	int flash_address;
	int length_to_write;
	int completed;
	uint8_t *data;
	uint8_t *verify_data;
	nfp_flash_progress_callback progress;
	int bus;
	int cs;
};

/**
 * Write either a configurator or miniloader image.
 *
 * @param	fwp	NFP flash write paramaters pointer
 */
int nfp_flash_protected_program(struct nfp_flash_write_parameters *fwp);

/**
 * Write a flash-nic image.
 *
 * @param nfp    NFP interface
 * @param buff	 buffer pointer
 * @param length Length of image
 * @param save   Save the user written hwinfo overrides
 */
int nfp_flash_write_flash_nic(struct nfp_device *nfp, void *buff, uint32_t length, int save, nfp_flash_progress_callback progress);

/**
 * Write a flash-one image.
 *
 * @param nfp    NFP interface
 * @param buff	 buffer pointer
 * @param length Length of image
 */
int nfp_flash_write_flash_one(struct nfp_device *nfp, void *buff, uint32_t length);

/**
 * Write a configurator image.
 *
 * @param nfp    NFP interface
 * @param buff	 buffer pointer
 * @param length Length of image
 */
int nfp_flash_write_flash_config(struct nfp_device *nfp, void *buff, uint32_t length);

#endif /* NFP_FLASH_H */
