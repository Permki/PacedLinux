/*
 * Copyright (C) 2010-2011,2014,2017  Netronome Systems, Inc.  All rights reserved.
 *
 * @file          nfp.h
 * @brief         Interface for NFP device access and query functions.
 *
 */
#ifndef __NFP_H__
#define __NFP_H__

#ifdef __KERNEL__
#define __BEGIN_DECLS
#define __END_DECLS
#else
#include <stdint.h>
#include <sys/types.h>
#include <sys/cdefs.h>
#endif

#include <nfp-common/nfp_platform.h>

#ifndef __KERNEL__
#include <nfp-common/nfp_regdata.h>
#else
#define NO_NFP_API_EXPORT
#endif

#include "_nfp_apiex.h"


/** Opaque NFP device handle. */
struct nfp_device;
struct nfp_cpp;
struct nfp_chipdata_chip;


/** Maximum device number for an NFP device. */
#define NFP_MAX_DEVICE_NUM              63
__BEGIN_DECLS;

/**
 * Get a mask of valid NFP device numbers.
 *
 * @return bitmask indicating which device numbers are valid.
 */
NFP_API
uint64_t nfp_device_mask(void);

/**
 * Get number of NFP devices in the system.
 *
 * The device numbering may be non-contigous.  Use nfp_device_mask()
 * to determine which device numbers are valid.
 *
 * @return number of valid NFP devices.
 */
NFP_API
int nfp_num_devices(void);

/**
 * Open NFP device.
 *
 * @param devnum        NFP device number
 *
 * @return device struct pointer, or NULL on failure (and set errno
 * accordingly).
 */
NFP_API
struct nfp_device * nfp_device_open(unsigned int devnum);

/**
 * Create NFP device handle using pre-existing CPP handle
 *
 * @param cpp           NFP CPP handle
 *
 * @return device struct pointer, or NULL on failure (and set errno
 * accordingly).
 */
NFP_API
struct nfp_device * nfp_device_from_cpp(struct nfp_cpp *cpp);

/**
 * Free up resources and close NFP device.
 *
 * @param dev           NFP device
 */
NFP_API
void nfp_device_close(struct nfp_device *dev);

/**
 * Get device number.
 *
 * @param dev           NFP device
 */
NFP_API
int nfp_device_number(struct nfp_device *dev);

/**
 * Get device number from pcie bus number.
 *
 * @param dev           Bus number or
 *                      Domain:Bus:Device.Function
 * @return device number or errno on fail accordingly.
 */
NFP_API
int nfp_device_number_from_pcie(char *dev);

/**
 * Open NFP device.
 *
 * @param dev           Bus number or
 *                      Domain:Bus:Device.Function
 *
 * @return device struct pointer, or NULL on failure (and set errno
 * accordingly).
 */
NFP_API
struct nfp_device * nfp_device_open_pcie(char *dev);

/**
 * Get NFP CPP access handle.
 *
 * @param dev           NFP device
 *
 * @return NFP CPP handle, or NULL on failure (and set errno
 * accordingly).
 */
NFP_API
struct nfp_cpp * nfp_device_cpp(struct nfp_device *dev);

/**
 * Get NFP ChipData chip handle for the given device.
 *
 * @param dev           NFP device
 *
 * @return Mutable NFP ChipData chip handle, or NULL on failure (and set
 * errno accordingly).
 */
NFP_API
const struct nfp_chipdata_chip * nfp_device_chip(struct nfp_device *dev);

/**
 * Return a private memory area, identified by the constructor,
 * that will automatically be freed on nfp_device_close().
 *
 * @param dev           NFP device
 * @param constructor   Constructor for the private area
 */
NFP_API
void * nfp_device_private(struct nfp_device *dev, void *(*constructor)(struct nfp_device *dev));

/**
 * Allocate your private area - must be called in the constructor
 * function passed to nfp_device_private().
 *
 * @param dev           NFP device
 * @param private_size  Size to allocate
 * @param destructor    Destructor function to call on device close, or NULL
 */
NFP_API
void * nfp_device_private_alloc(struct nfp_device *dev, size_t private_size, void (*destructor)(void *private_data));

/**
 * Perform an advisory trylock on the NFP device
 *
 * @param dev           NFP device
 *
 * @return 0 on success, or -1 on error (and set errno accordingly)
 */
NFP_API
int nfp_device_trylock(struct nfp_device *dev);

/**
 * Perform an advisory lock on the NFP device
 *
 * @param dev           NFP device
 *
 * @return 0 on success, or -1 on error (and set errno accordingly)
 */
NFP_API
int nfp_device_lock(struct nfp_device *dev);

/**
 * Perform an advisory unlock on the NFP device
 *
 * @param dev           NFP device
 *
 * @return 0 on success, or -1 on error (and set errno accordingly)
 */
NFP_API
int nfp_device_unlock(struct nfp_device *dev);

/**
 * Read memory from NFP SRAM.
 *
 * @param dev           NFP device
 * @param buffer        output buffer
 * @param length        number of bytes to read
 * @param offset        offset into NFP SRAM
 *
 * Read contents of NFP SRAM into @a buffer.  @a length and @a offset must
 * both be 4-byte aligned.
 *
 * @return number of bytes actually read, or -1 on error (and set
 * errno accordingly).
 */
NFP_API
ssize_t nfp_sram_read(struct nfp_device *dev, void *buffer, unsigned long long length, unsigned long long offset);

/**
 * Write to NFP SRAM.
 *
 * @param dev           NFP device
 * @param buffer        input buffer
 * @param length        number of bytes to write
 * @param offset        offset into NFP SRAM
 *
 * Write contents of @a buffer into NFP SRAM.  @a length and @a offset must
 * both be 4-byte aligned.
 *
 * @return number of bytes actually written, or -1 on error (and set
 * errno accordingly).
 */
NFP_API
ssize_t nfp_sram_write(struct nfp_device *dev, const void *buffer, unsigned long long length, unsigned long long offset);

/**
 * Fill NFP SRAM with given pattern.
 *
 * @param dev           NFP device
 * @param buffer        input buffer to fill with
 * @param buf_length    number of bytes in input buffer
 * @param offset        offset into NFP SRAM
 * @param write_length  number of bytes to write
 *
 * Fill indicated NFP SRAM area with contents of @a buffer.  If
 * @a write_length is larger than @a buf_length the contents of @a buffer
 * will be repeated until the whole area is filled.  @a buf_length,
 * @a offset, and @a write_length must all be 4-byte aligned.
 *
 * @return number of bytes actually written, or -1 on error (and set
 * errno accordingly).
 */
NFP_API
ssize_t nfp_sram_fill(struct nfp_device *dev, const void *buffer, unsigned long long buf_length, unsigned long long offset, unsigned long long write_length);

/**
 * @ingroup nfp6000-only
 * Read memory from NFP External Memory
 *
 * @param dev           NFP device
 * @param islnum        the island number
 * @param buffer        output buffer
 * @param length        number of bytes to read
 * @param offset        offset into NFP External Memory
 *
 * Read contents of NFP External Memory into @a buffer.  @a length and @a offset must
 * both be 4-byte aligned.
 *
 * If @a islnum is 0, the @a offset will be used unchanged, which means the
 * caller has to ensure the address is translated so that the IMB which
 * this command will be sent through can route the command correctly.
 * If @a islnum is not 0, the @a offset will be modified to be routable to
 * the destination island.
 *
 * @return number of bytes actually read, or -1 on error (and set errno accordingly).
 */
NFP_API
ssize_t nfp_emem_read(struct nfp_device *dev, int islnum, void *buffer, unsigned long long length, unsigned long long offset);

/**
 * @ingroup nfp6000-only
 * Write to NFP External Memory.
 *
 * @param dev           NFP device
 * @param islnum        the island number
 * @param buffer        input buffer
 * @param length        number of bytes to write
 * @param offset        offset into NFP External Memory
 *
 * Write contents of @a buffer into NFP External Memory.  @a length and @a offset must
 * both be 4-byte aligned.
 *
 * If @a islnum is 0, the @a offset will be used unchanged, which means the
 * caller has to ensure the address is translated so that the IMB which
 * this command will be sent through can route the command correctly.
 * If @a islnum is not 0, the @a offset will be modified to be routable to
 * the destination island.
 *
 * @return number of bytes actually written, or -1 on error (and set errno accordingly).
 */
NFP_API
ssize_t nfp_emem_write(struct nfp_device *dev, int islnum, const void *buffer, unsigned long long length, unsigned long long offset);

/**
 * @ingroup nfp6000-only
 * Fill NFP External Memory with given pattern.
 *
 * @param dev           NFP device
 * @param islnum        the island number
 * @param buffer        input buffer to fill with
 * @param buf_length    number of bytes in input buffer
 * @param offset        offset into NFP External Memory
 * @param write_length  number of bytes to write
 *
 * Fill indicated NFP External Memory area with contents of @a buffer.  If
 * @a write_length is larger than @a buf_length the contents of @a buffer
 * will be repeated until the whole area is filled.  @a buf_length,
 * @a offset, and @a write_length must all be 4-byte aligned.
 *
 * If @a islnum is 0, the @a offset will be used unchanged, which means the
 * caller has to ensure the address is translated so that the IMB which
 * this command will be sent through can route the command correctly.
 * If @a islnum is not 0, the @a offset will be modified to be routable to
 * the destination island.
 *
 * @return number of bytes actually written, or -1 on error (and set errno accordingly).
 */
NFP_API
ssize_t nfp_emem_fill(struct nfp_device *dev, int islnum, const void *buffer, unsigned long long buf_length, unsigned long long offset, unsigned long long write_length);

/**
 * @ingroup nfp6000-only
 * Read memory from NFP External Memory
 *
 * @param dev           NFP device
 * @param islnum        the island number
 * @param buffer        output buffer
 * @param length        number of bytes to read
 * @param offset        offset into NFP External Memory
 *
 * Read contents of NFP External Memory into @a buffer.  @a length and @a offset must
 * both be 8-byte aligned.
 *
 * If @a islnum is 0, the @a offset will be used unchanged, which means the
 * caller has to ensure the address is translated so that the IMB which
 * this command will be sent through can route the command correctly.
 * If @a islnum is not 0, the @a offset will be modified to be routable to
 * the destination island.
 *
 * @return number of bytes actually read, or -1 on error (and set errno accordingly).
 */
NFP_API
ssize_t nfp_emem64_read(struct nfp_device *dev, int islnum, void *buffer, unsigned long long length, unsigned long long offset);

/**
 * @ingroup nfp6000-only
 * Write to NFP External Memory.
 *
 * @param dev           NFP device
 * @param islnum        the island number
 * @param buffer        input buffer
 * @param length        number of bytes to write
 * @param offset        offset into NFP External Memory
 *
 * Write contents of @a buffer into NFP External Memory.  @a length and @a offset must
 * both be 8-byte aligned.
 *
 * If @a islnum is 0, the @a offset will be used unchanged, which means the
 * caller has to ensure the address is translated so that the IMB which
 * this command will be sent through can route the command correctly.
 * If @a islnum is not 0, the @a offset will be modified to be routable to
 * the destination island.
 *
 * @return number of bytes actually written, or -1 on error (and set errno accordingly).
 */
NFP_API
ssize_t nfp_emem64_write(struct nfp_device *dev, int islnum, const void *buffer, unsigned long long length, unsigned long long offset);

/**
 * @ingroup nfp6000-only
 * Fill NFP External Memory with given pattern.
 *
 * @param dev           NFP device
 * @param islnum        the island number
 * @param buffer        input buffer to fill with
 * @param buf_length    number of bytes in input buffer
 * @param offset        offset into NFP External Memory
 * @param write_length  number of bytes to write
 *
 * Fill indicated NFP External Memory area with contents of @a buffer.  If
 * @a write_length is larger than @a buf_length the contents of @a buffer
 * will be repeated until the whole area is filled.  @a buf_length,
 * @a offset, and @a write_length must all be 8-byte aligned.
 *
 * If @a islnum is 0, the @a offset will be used unchanged, which means the
 * caller has to ensure the address is translated so that the IMB which
 * this command will be sent through can route the command correctly.
 * If @a islnum is not 0, the @a offset will be modified to be routable to
 * the destination island.
 *
 * @return number of bytes actually written, or -1 on error (and set errno accordingly).
 */
NFP_API
ssize_t nfp_emem64_fill(struct nfp_device *dev, int islnum, const void *buffer, unsigned long long buf_length, unsigned long long offset, unsigned long long write_length);

/**
 * @ingroup nfp6000-only
 * Read memory from NFP Internal Memory
 *
 * @param dev           NFP device
 * @param islnum        island number
 * @param buffer        output buffer
 * @param length        number of bytes to read
 * @param offset        offset into Internal Memory
 *
 * Read contents of Internal Memory into @a buffer.  @a length
 * and @a offset must both be 4-byte aligned.
 *
 * If @a islnum is 0, the @a offset will be used unchanged, which means the
 * caller has to ensure the address is translated so that the IMB which
 * this command will be sent through can route the command correctly.
 * If @a islnum is not 0, the @a offset will be modified to be routable to
 * the destination island. This includes ensuring that the @a offset is
 * using Direct Access locality.
 *
 * @return number of bytes actually read, or -1 on error (and set errno accordingly).
 */
NFP_API
ssize_t nfp_imem_read(struct nfp_device *dev, int islnum, void *buffer, unsigned long long length, unsigned long long offset);

/**
 * @ingroup nfp6000-only
 * Write to Internal Memory
 *
 * @param dev           NFP device
 * @param islnum        island number
 * @param buffer        input buffer
 * @param length        number of bytes to write
 * @param offset        offset into internal memory
 *
 * Write contents of @a buffer into internal memory.  @a length
 * and @a offset must both be 4-byte aligned.
 *
 * If @a islnum is 0, the @a offset will be used unchanged, which means the
 * caller has to ensure the address is translated so that the IMB which
 * this command will be sent through can route the command correctly.
 * If @a islnum is not 0, the @a offset will be modified to be routable to
 * the destination island. This includes ensuring that the @a offset is
 * using Direct Access locality.
 *
 * @return number of bytes actually written, or -1 on error (and set errno accordingly).
 */
NFP_API
ssize_t nfp_imem_write(struct nfp_device *dev, int islnum, const void *buffer, unsigned long long length, unsigned long long offset);

/**
 * @ingroup nfp6000-only
 * Fill Internal Memory with given pattern.
 *
 * @param dev           NFP device
 * @param islnum        island number
 * @param buffer        input buffer to fill with
 * @param buf_length    number of bytes in input buffer
 * @param offset        offset into Internal Memory
 * @param write_length  number of bytes to write
 *
 * Fill indicated Internal Memory area with contents of
 * @a buffer.  If @a write_length is larger than @a buf_length the contents
 * of @a buffer will be repeated until the whole area is filled.
 * @a buf_length, @a offset, and @a write_length must all be 4-byte aligned.
 *
 * If @a islnum is 0, the @a offset will be used unchanged, which means the
 * caller has to ensure the address is translated so that the IMB which
 * this command will be sent through can route the command correctly.
 * If @a islnum is not 0, the @a offset will be modified to be routable to
 * the destination island. This includes ensuring that the @a offset is
 * using Direct Access locality.
 *
 * @return zero, or -1 on error
 */
NFP_API
ssize_t nfp_imem_fill(struct nfp_device *dev, int islnum, const void *buffer, unsigned long long buf_length, unsigned long long offset, unsigned long long write_length);

/**
 * @ingroup nfp6000-only
 * Read memory from NFP Internal Memory
 *
 * @param dev           NFP device
 * @param islnum        island number
 * @param buffer        output buffer
 * @param length        number of bytes to read
 * @param offset        offset into Internal Memory
 *
 * Read contents of Internal Memory into @a buffer.  @a length
 * and @a offset must both be 8-byte aligned.
 *
 * If @a islnum is 0, the @a offset will be used unchanged, which means the
 * caller has to ensure the address is translated so that the IMB which
 * this command will be sent through can route the command correctly.
 * If @a islnum is not 0, the @a offset will be modified to be routable to
 * the destination island. This includes ensuring that the @a offset is
 * using Direct Access locality.
 *
 * @return number of bytes actually read, or -1 on error (and set errno accordingly).
 */
NFP_API
ssize_t nfp_imem64_read(struct nfp_device *dev, int islnum, void *buffer, unsigned long long length, unsigned long long offset);

/**
 * @ingroup nfp6000-only
 * Write to Internal Memory
 *
 * @param dev           NFP device
 * @param islnum        island number
 * @param buffer        input buffer
 * @param length        number of bytes to write
 * @param offset        offset into internal memory
 *
 * Write contents of @a buffer into internal memory.  @a length
 * and @a offset must both be 8-byte aligned.
 *
 * If @a islnum is 0, the @a offset will be used unchanged, which means the
 * caller has to ensure the address is translated so that the IMB which
 * this command will be sent through can route the command correctly.
 * If @a islnum is not 0, the @a offset will be modified to be routable to
 * the destination island. This includes ensuring that the @a offset is
 * using Direct Access locality.
 *
 * @return number of bytes actually written, or -1 on error (and set errno accordingly).
 */
NFP_API
ssize_t nfp_imem64_write(struct nfp_device *dev, int islnum, const void *buffer, unsigned long long length, unsigned long long offset);

/**
 * @ingroup nfp6000-only
 * Fill Internal Memory with given pattern.
 *
 * @param dev           NFP device
 * @param islnum        island number
 * @param buffer        input buffer to fill with
 * @param buf_length    number of bytes in input buffer
 * @param offset        offset into Internal Memory
 * @param write_length  number of bytes to write
 *
 * Fill indicated Internal Memory area with contents of
 * @a buffer.  If @a write_length is larger than @a buf_length the contents
 * of @a buffer will be repeated until the whole area is filled.
 * @a buf_length, @a offset, and @a write_length must all be 8-byte aligned.
 *
 * If @a islnum is 0, the @a offset will be used unchanged, which means the
 * caller has to ensure the address is translated so that the IMB which
 * this command will be sent through can route the command correctly.
 * If @a islnum is not 0, the @a offset will be modified to be routable to
 * the destination island. This includes ensuring that the @a offset is
 * using Direct Access locality.
 *
 * @return zero, or -1 on error
 */
NFP_API
ssize_t nfp_imem64_fill(struct nfp_device *dev, int islnum, const void *buffer, unsigned long long buf_length, unsigned long long offset, unsigned long long write_length);

/**
 * @ingroup nfp6000-only
 * Read memory from NFP Cluster Target Memory
 *
 * @param dev           NFP device
 * @param islnum        the island number
 * @param buffer        output buffer
 * @param length        number of bytes to read
 * @param offset        offset into NFP Cluster Target Memory
 *
 * Read contents of NFP Cluster Target Memory into @a buffer.  @a length and @a offset must
 * both be 4-byte aligned.
 *
 * If @a islnum is 0, the @a offset will be used unchanged, which means the
 * caller has to ensure the address is translated so that the IMB which
 * this command will be sent through can route the command correctly.
 * If @a islnum is not 0, the @a offset will be modified to be routable to
 * the destination island. This includes ensuring that the @a offset is
 * using Direct Access locality.
 *
 * @return number of bytes actually read, or -1 on error (and set errno accordingly).
 */
NFP_API
ssize_t nfp_ctm_read(struct nfp_device *dev, int islnum, void *buffer, unsigned long long length, unsigned long long offset);

/**
 * @ingroup nfp6000-only
 * Write to NFP Cluster Target Memory.
 *
 * @param dev           NFP device
 * @param islnum        the island number
 * @param buffer        input buffer
 * @param length        number of bytes to write
 * @param offset        offset into NFP Cluster Target Memory
 *
 * Write contents of @a buffer into NFP Cluster Target Memory.  @a length and @a offset must
 * both be 4-byte aligned.
 *
 * If @a islnum is 0, the @a offset will be used unchanged, which means the
 * caller has to ensure the address is translated so that the IMB which
 * this command will be sent through can route the command correctly.
 * If @a islnum is not 0, the @a offset will be modified to be routable to
 * the destination island. This includes ensuring that the @a offset is
 * using Direct Access locality.
 *
 * @return number of bytes actually written, or -1 on error (and set errno accordingly).
 */
NFP_API
ssize_t nfp_ctm_write(struct nfp_device *dev, int islnum, const void *buffer, unsigned long long length, unsigned long long offset);

/**
 * @ingroup nfp6000-only
 * Fill NFP Cluster Target Memory with given pattern.
 *
 * @param dev           NFP device
 * @param islnum        the island number
 * @param buffer        input buffer to fill with
 * @param buf_length    number of bytes in input buffer
 * @param offset        offset into NFP Cluster Target Memory
 * @param write_length  number of bytes to write
 *
 * Fill indicated NFP Cluster Target Memory area with contents of @a buffer.  If
 * @a write_length is larger than @a buf_length the contents of @a buffer
 * will be repeated until the whole area is filled.  @a buf_length,
 * @a offset, and @a write_length must all be 4-byte aligned.
 *
 * If @a islnum is 0, the @a offset will be used unchanged, which means the
 * caller has to ensure the address is translated so that the IMB which
 * this command will be sent through can route the command correctly.
 * If @a islnum is not 0, the @a offset will be modified to be routable to
 * the destination island. This includes ensuring that the @a offset is
 * using Direct Access locality.
 *
 * @return number of bytes actually written, or -1 on error (and set errno accordingly).
 */
NFP_API
ssize_t nfp_ctm_fill(struct nfp_device *dev, int islnum, const void *buffer, unsigned long long buf_length, unsigned long long offset, unsigned long long write_length);

/**
 * @ingroup nfp6000-only
 * Read memory from NFP Cluster Target Memory
 *
 * @param dev           NFP device
 * @param islnum        the island number
 * @param buffer        output buffer
 * @param length        number of bytes to read
 * @param offset        offset into NFP Cluster Target Memory
 *
 * Read contents of NFP Cluster Target Memory into @a buffer.  @a length and @a offset must
 * both be 8-byte aligned.
 *
 * If @a islnum is 0, the @a offset will be used unchanged, which means the
 * caller has to ensure the address is translated so that the IMB which
 * this command will be sent through can route the command correctly.
 * If @a islnum is not 0, the @a offset will be modified to be routable to
 * the destination island. This includes ensuring that the @a offset is
 * using Direct Access locality.
 *
 * @return number of bytes actually read, or -1 on error (and set errno accordingly).
 */
NFP_API
ssize_t nfp_ctm64_read(struct nfp_device *dev, int islnum, void *buffer, unsigned long long length, unsigned long long offset);

/**
 * @ingroup nfp6000-only
 * Write to NFP Cluster Target Memory.
 *
 * @param dev           NFP device
 * @param islnum        the island number
 * @param buffer        input buffer
 * @param length        number of bytes to write
 * @param offset        offset into NFP Cluster Target Memory
 *
 * Write contents of @a buffer into NFP Cluster Target Memory.  @a length and @a offset must
 * both be 8-byte aligned.
 *
 * If @a islnum is 0, the @a offset will be used unchanged, which means the
 * caller has to ensure the address is translated so that the IMB which
 * this command will be sent through can route the command correctly.
 * If @a islnum is not 0, the @a offset will be modified to be routable to
 * the destination island. This includes ensuring that the @a offset is
 * using Direct Access locality.
 *
 * @return number of bytes actually written, or -1 on error (and set errno accordingly).
 */
NFP_API
ssize_t nfp_ctm64_write(struct nfp_device *dev, int islnum, const void *buffer, unsigned long long length, unsigned long long offset);

/**
 * @ingroup nfp6000-only
 * Fill NFP Cluster Target Memory with given pattern.
 *
 * @param dev           NFP device
 * @param islnum        the island number
 * @param buffer        input buffer to fill with
 * @param buf_length    number of bytes in input buffer
 * @param offset        offset into NFP Cluster Target Memory
 * @param write_length  number of bytes to write
 *
 * Fill indicated NFP Cluster Target Memory area with contents of @a buffer.  If
 * @a write_length is larger than @a buf_length the contents of @a buffer
 * will be repeated until the whole area is filled.  @a buf_length,
 * @a offset, and @a write_length must all be 8-byte aligned.
 *
 * If @a islnum is 0, the @a offset will be used unchanged, which means the
 * caller has to ensure the address is translated so that the IMB which
 * this command will be sent through can route the command correctly.
 * If @a islnum is not 0, the @a offset will be modified to be routable to
 * the destination island. This includes ensuring that the @a offset is
 * using Direct Access locality.
 *
 * @return number of bytes actually written, or -1 on error (and set errno accordingly).
 */
NFP_API
ssize_t nfp_ctm64_fill(struct nfp_device *dev, int islnum, const void *buffer, unsigned long long buf_length, unsigned long long offset, unsigned long long write_length);

#ifndef __KERNEL__
/* == Start of microengine related access APIs == */

/**
 * Read memory from NFP Cluser Local Scratch.
 *
 * @param dev           NFP device
 * @param clnum         cluster number
 * @param buffer        output buffer
 * @param length        number of bytes to read
 * @param offset        offset into NFP Cluster Local Scratch
 *
 * Read contents of NFP Cluster Local Scratch into @a buffer.  @a length
 * and @a offset must both be 4-byte aligned.
 *
 * @return number of bytes actually read, or -1 on error (and set
 * errno accordingly).
 */
NFP_API
ssize_t nfp_cls_read(struct nfp_device *dev, int clnum, void *buffer, unsigned long long length, unsigned long long offset);

/**
 * Write to NFP Cluster Local Scratch.
 *
 * @param dev           NFP device
 * @param clnum         cluster number
 * @param buffer        input buffer
 * @param length        number of bytes to write
 * @param offset        offset into NFP Cluster Local Scratch
 *
 * Write contents of @a buffer into NFP Cluster Local Scratch.  @a length
 * and @a offset must both be 4-byte aligned.
 *
 * @return number of bytes actually written, or -1 on error (and set
 * errno accordingly).
 */
NFP_API
ssize_t nfp_cls_write(struct nfp_device *dev, int clnum, const void *buffer, unsigned long long length, unsigned long long offset);

/**
 * Fill NFP Cluster Local Scratch with given pattern.
 *
 * @param dev           NFP device
 * @param clnum         cluster number
 * @param buffer        input buffer to fill with
 * @param buf_length    number of bytes in input buffer
 * @param offset        offset into NFP Cluster Local Scratch
 * @param write_length  number of bytes to write
 *
 * Fill indicated NFP Cluster Local Scratch area with contents of
 * @a buffer.  If @a write_length is larger than @a buf_length the contents
 * of @a buffer will be repeated until the whole area is filled.
 * @a buf_length, @a offset, and @a write_length must all be 4-byte aligned.
 *
 * @return number of bytes actually written, or -1 on error (and set
 * errno accordingly).
 */
NFP_API
ssize_t nfp_cls_fill(struct nfp_device *dev, int clnum, const void *buffer, unsigned long long buf_length, unsigned long long offset, unsigned long long write_length);

/**
 * Read memory from ME Local Memory.
 *
 * @param dev           NFP device
 * @param meid          ME ID
 * @param buffer        output buffer
 * @param length        number of bytes to read
 * @param offset        offset into ME Local Memory
 *
 * Read contents of ME Local Memory into @a buffer.  @a length and @a offset
 * must both be 4-byte aligned.
 *
 * @return number of bytes actually read, or -1 on error (and set
 * errno accordingly).
 */
NFP_API
ssize_t nfp_lmem_read(struct nfp_device *dev, int meid, void *buffer, unsigned long long length, unsigned long long offset);

/**
 * Write to ME Local Memory.
 *
 * @param dev           NFP device
 * @param meid          ME ID
 * @param buffer        input buffer
 * @param length        number of bytes to write
 * @param offset        offset into NFP Cluster Local Scratch
 *
 * Write contents of @a buffer into ME Local Memory.  @a length and
 * @a offset must both be 4-byte aligned.
 *
 * @return number of bytes actually written, or -1 on error (and set
 * errno accordingly).
 */
NFP_API
ssize_t nfp_lmem_write(struct nfp_device *dev, int meid, const void *buffer, unsigned long long length, unsigned long long offset);

/**
 * Fill ME Local Memory with given pattern.
 *
 * @param dev           NFP device
 * @param meid          ME ID
 * @param buffer        input buffer to fill with
 * @param buf_length    number of bytes in input buffer
 * @param offset        offset into ME Local Memory
 * @param write_length  number of bytes to write
 *
 * Fill indicated ME Local Memory area with contents of @a buffer.  If
 * @a write_length is larger than @a buf_length the contents of @a buffer
 * will be repeated until the whole area is filled.  @a buf_length,
 * @a offset, and @a write_length must all be 4-byte aligned.
 *
 * @return number of bytes actually written, or -1 on error (and set
 * errno accordingly).
 */
NFP_API
ssize_t nfp_lmem_fill(struct nfp_device *dev, int meid, const void *buffer, unsigned long long buf_length, unsigned long long offset, unsigned long long write_length);

/**
 * Read memory from ME Control Store.
 *
 * @param dev           NFP device
 * @param meid          ME ID
 * @param buffer        output buffer
 * @param length        number of bytes to read
 * @param offset        offset into ME Control Store
 *
 * Read contents of ME Control Store into @a buffer.  @a length and
 * @a offset must both be 8-byte aligned.
 *
 * @return number of bytes actually read, or -1 on error (and set
 * errno accordingly).
 */
NFP_API
ssize_t nfp_ustore_read(struct nfp_device *dev, int meid, void *buffer, unsigned long long length, unsigned long long offset);

/**
 * Write to ME Control Store.
 *
 * @param dev           NFP device
 * @param meid          ME ID
 * @param buffer        input buffer
 * @param length        number of bytes to write
 * @param offset        offset into ME Control Store
 *
 * Write contents of @a buffer into ME Local Memory.  @a length and
 * @a offset must both be 8-byte aligned.
 *
 * @return number of bytes actually written, or -1 on error (and set
 * errno accordingly).
 */
NFP_API
ssize_t nfp_ustore_write(struct nfp_device *dev, int meid, const void *buffer, unsigned long long length, unsigned long long offset);

/**
 * Fill ME Control Store with given pattern.
 *
 * @param dev           NFP device
 * @param meid          ME ID
 * @param buffer        input buffer to fill with
 * @param buf_length    number of bytes in input buffer
 * @param offset        offset into ME Control Store
 * @param write_length  number of bytes to write
 *
 * Fill indicated ME Control Store area with contents of @a buffer.  If
 * @a write_length is larger than @a buf_length the contents of @a buffer
 * will be repeated until the whole area is filled.  @a buf_length,
 * @a offset, and @a write_length must all be 8-byte aligned.
 *
 * @return number of bytes actually written, or -1 on error (and set
 * errno accordingly).
 */
NFP_API
ssize_t nfp_ustore_fill(struct nfp_device *dev, int meid, const void *buffer, unsigned long long buf_length, unsigned long long offset, unsigned long long write_length);

/**
 * Read memory from Crypto Island Sram.
 *
 * @param dev           NFP device
 * @param islnum        the island number
 * @param buffer        output buffer
 * @param length        number of bytes to read
 * @param offset        offset into Crypto Sram
 *
 * Read contents of crypto island sram into @a buffer.  @a length and
 * @a offset must both be 8-byte aligned.
 *
 * @return number of bytes actually read, or -1 on error (and set
 * errno accordingly).
 */
NFP_API
ssize_t nfp_crypto_sram_read(struct nfp_device *dev, int islnum, void *buffer, unsigned long long length, unsigned long long offset);

/**
 * Write to Crypto Island Sram.
 *
 * @param dev           NFP device
 * @param islnum        the island number
 * @param buffer        input buffer
 * @param length        number of bytes to write
 * @param offset        offset into Crypto Sram
 *
 * Write contents of @a buffer into crypto island sram.  @a length and
 * @a offset must both be 8-byte aligned.
 *
 * @return number of bytes actually written, or -1 on error (and set
 * errno accordingly).
 */
NFP_API
ssize_t nfp_crypto_sram_write(struct nfp_device *dev, int islnum, const void *buffer, unsigned long long length, unsigned long long offset);

/**
 * Fill Crypto Island Sram with given pattern.
 *
 * @param dev           NFP device
 * @param islnum        the island number
 * @param buffer        input buffer to fill with
 * @param buf_length    number of bytes in input buffer
 * @param offset        offset into Crypto Sram
 * @param write_length  number of bytes to write
 *
 * Fill indicated crytpo island sram area with contents of @a buffer.  If
 * @a write_length is larger than @a buf_length the contents of @a buffer
 * will be repeated until the whole area is filled.  @a buf_length,
 * @a offset, and @a write_length must all be 8-byte aligned.
 *
 * @return number of bytes actually written, or -1 on error (and set
 * errno accordingly).
 */
NFP_API
ssize_t nfp_crypto_sram_fill(struct nfp_device *dev, int islnum, const void *buffer, unsigned long long buf_length, unsigned long long offset, unsigned long long write_length);

/**
 * Read ME CSR on NFP device.
 *
 * @param dev           NFP device
 * @param meid          ME ID
 * @param ctx           context number (ignored for non-ctx CSRs)
 * @param csr           CSR offset
 * @param value         return value
 *
 * For context specific CSRs the CSR_CONTEXT_POINTER CSR is
 * temporarily written with the value of @a ctx in order to read the CSR
 * of the right context.  If @a ctx is specified as -1 the
 * CSR_CONTEXT_CSR is not modidied, using the currently active context
 * pointer is used instead.  The @a ctx parameter is ignored for
 * non-context specific CSRs.
 *
 * @return 0 on success, or -1 on failure (and set errno accordingly).
 */
NFP_API
int nfp_mecsr_read(struct nfp_device *dev, int meid, int ctx, unsigned int csr, uint32_t *value);

/**
 * Write ME CSR on NFP device.
 *
 * @param dev           NFP device
 * @param meid          ME ID
 * @param ctx           context number (ignored for non-ctx CSRs)
 * @param csr           CSR offset
 * @param value         value to write
 *
 * For context specific CSRs the CSR_CONTEXT_POINTER CSR is
 * temporarily written with the value of @a ctx in order to write the
 * CSR of the right context.  If @a ctx is specified as -1 the
 * CSR_CONTEXT_CSR is not modidied, using the currently active context
 * pointer is used instead.  The @a ctx parameter is ignored for
 * non-context specific CSRs.
 *
 * @return 0 on success, or -1 on failure (and set errno accordingly).
 */
NFP_API
int nfp_mecsr_write(struct nfp_device *dev, int meid, int ctx, unsigned int csr, uint32_t value);

static nfp_inline unsigned int nfp_mereg(int type, int index)
{
	return ((type & 0xff) << 16) | ((index << 2) & 0xffff);
}

static nfp_inline unsigned int nfp_mereg_gprA(int index)
{
	return nfp_mereg(NFP_MEREG_TYPE_GPRA, index);
}

static nfp_inline unsigned int nfp_mereg_gprA_ctx(int index, int ctx)
{
	return nfp_mereg(NFP_MEREG_TYPE_GPRA, index + (ctx * 16));
}

static nfp_inline unsigned int nfp_mereg_gprB(int index)
{
	return nfp_mereg(NFP_MEREG_TYPE_GPRB, index);
}

static nfp_inline unsigned int nfp_mereg_gprB_ctx(int index, int ctx)
{
	return nfp_mereg(NFP_MEREG_TYPE_GPRB, index + (ctx * 16));
}

static nfp_inline unsigned int nfp_mereg_xfer_in(int index)
{
	return nfp_mereg(NFP_MEREG_TYPE_XFER_IN, index);
}

static nfp_inline unsigned int nfp_mereg_xfer_in_ctx(int index, int ctx)
{
	return nfp_mereg(NFP_MEREG_TYPE_XFER_IN, index + (ctx * 32));
}

static nfp_inline unsigned int nfp_mereg_xfer_out(int index)
{
	return nfp_mereg(NFP_MEREG_TYPE_XFER_OUT, index);
}

static nfp_inline unsigned int nfp_mereg_xfer_out_ctx(int index, int ctx)
{
	return nfp_mereg(NFP_MEREG_TYPE_XFER_OUT, index + (ctx * 32));
}

static nfp_inline unsigned int nfp_mereg_nn(int index)
{
	return nfp_mereg(NFP_MEREG_TYPE_NN, index);
}

static nfp_inline unsigned int nfp_mereg_nn_ctx(int index, int ctx)
{
	return nfp_mereg(NFP_MEREG_TYPE_NN, index + (ctx * 16));
}

/**
 * Read ME register on NFP device.
 *
 * @param dev           NFP device
 * @param meid          ME ID
 * @param reg           register to read
 * @param value         return value
 *
 * Read ME register.  Bits [23:16] of REG indicate the register type
 * to read and bits [15:2] indicate the register index.  It is
 * recommended that the nfp_mereg() family of functions be used to
 * supply the REG parameter.
 *
 * Most of the ME registers can not be read unless the ME has been
 * halted first.  The function will in these cases return -1 with
 * errno set to EBUSY.
 *
 * @return 0 on success, or -1 on failure (and set errno accordingly).
 */
NFP_API
int nfp_me_register_read(struct nfp_device *dev, int meid, unsigned int reg, uint32_t *value);

/**
 * Write ME register on NFP device.
 *
 * @param dev           NFP device
 * @param meid          ME ID
 * @param reg           register to write
 * @param value         value to write
 *
 * Write ME register.  Bits [23:16] of REG indicate the register type
 * to write and bits [15:2] indicate the register index.  It is
 * recommended that the nfp_mereg() family of functions be used to
 * supply the REG parameter.
 *
 * Most of the ME registers can not be written unless the ME has been
 * halted first.  The function will in these cases return -1 with
 * errno set to EBUSY.
 *
 * @return 0 on success, or -1 on failure (and set errno accordingly).
 */
NFP_API
int nfp_me_register_write(struct nfp_device *dev, int meid, unsigned int reg, uint32_t value);

#endif /* __KERNEL */
/* == End of microengine related access APIs == */

/**
 * Query on-die temperature sensor of chip.
 *
 * @param dev           NFP device
 * @param raw_value     output raw value (or NULL)
 * @param temp_c        output degrees in C (or NULL)
 * @param temp_err      output error estimate in C (or NULL)
 *
 * Get internal value for on-die temperature sensor and calculate
 * estimated temperature.
 *
 * @return 0 on success, or -1 on error (and set errno accordingly).
 */
NFP_API
int nfp_temp(struct nfp_device *dev, uint32_t *raw_value, float *temp_c, float *temp_err);

/**
 * MDIO Clause 22 write via the NFP's MDIO controller
 *
 * @param dev           NFP device
 * @param bus           MDIO bus (0 or 1)
 * @param addr          MDIO address
 * @param reg           MDIO register
 * @param data          16-bit data to write
 *
 * Bus 0 is mapped to GPIO pins 8 and 9
 * Bus 1 is mapped to GPIO pins 10 and 11
 *
 * @return 0 on success, or -1 on error (and set errno accordingly).
 */
NFP_API
int nfp_mdio_cl22_write(struct nfp_device *dev, int bus, uint8_t addr, uint8_t reg, uint16_t data);

/**
 * MDIO Clause 22 read via the NFP's MDIO controller
 *
 * @param dev           NFP device
 * @param bus           MDIO bus (0 or 1)
 * @param addr          MDIO address
 * @param reg           MDIO register
 * @param data          Pointer to a 16-bit data area
 *
 * Bus 0 is mapped to GPIO pins 8 and 9
 * Bus 1 is mapped to GPIO pins 10 and 11
 *
 * @return 0 on success, or -1 on error (and set errno accordingly).
 */
NFP_API
int nfp_mdio_cl22_read(struct nfp_device *dev, int bus, uint8_t addr, uint8_t reg, uint16_t *data);

/**
 * MDIO Clause 45 write via the NFP's MDIO controller
 *
 * @param dev           NFP device
 * @param bus           MDIO bus (0 or 1)
 * @param addr          MDIO address
 * @param subdev        MDIO subdevice select
 * @param reg           MDIO subdevice register
 * @param data          16-bit data to write
 *
 * Please note that the Clause 45 register 'X.YYYY'
 * is considered as subdevice X, register YYYY in
 * this interface.
 *
 * Bus 0 is mapped to GPIO pins 8 and 9
 * Bus 1 is mapped to GPIO pins 10 and 11
 *
 * @return 0 on success, or -1 on error (and set errno accordingly).
 */
NFP_API
int nfp_mdio_cl45_write(struct nfp_device *dev, int bus, uint8_t addr, uint8_t subdev, uint16_t reg, uint16_t data);

/**
 * MDIO Clause 45 read via the NFP's MDIO controller
 *
 * @param dev           NFP device
 * @param bus           MDIO bus (0 or 1)
 * @param addr          MDIO address
 * @param subdev        MDIO subdevice select
 * @param reg           MDIO subdevice register
 * @param data          Pointer to a 16-bit data area
 *
 * Please note that the Clause 45 register 'X.YYYY'
 * is considered as subdevice X, register YYYY in
 * this interface.
 *
 * Bus 0 is mapped to GPIO pins 8 and 9
 * Bus 1 is mapped to GPIO pins 10 and 11
 *
 * @return 0 on success, or -1 on error (and set errno accordingly).
 */
NFP_API
int nfp_mdio_cl45_read(struct nfp_device *dev, int bus, uint8_t addr, uint8_t subdev, uint16_t reg, uint16_t *data);

/**
 * Open per bus MDIO controller handle
 *
 * @param dev           NFP device
 * @param bus           MDIO bus
 *
 */
NFP_API
struct nfp_mdio * nfp_mdio_bus_open(struct nfp_device *dev, int bus);

/**
 * Close MDIO controller handle
 *
 * @param mdio          MDIO device
 *
 */
NFP_API
void nfp_mdio_bus_close(struct nfp_mdio *mdio);

/**
 * Per bus MDIO controller lock
 *
 * @param mdio          MDIO device
 *
 */
NFP_API
int nfp_mdio_bus_lock(struct nfp_mdio *mdio);

/**
 * Per bus MDIO controller unlock
 *
 * @param mdio          MDIO device
 *
 */
NFP_API
void nfp_mdio_bus_unlock(struct nfp_mdio *mdio);

/**
 * MDIO Clause 45 read/write via the NFP's MDIO controller
 *
 * @param mdio          MDIO device
 * @param hz            MDIO bus frequency (uses default if <= 0)
 * @param pream_len     preamble length, max 32 (uses default if <= 0)
 * @param phy           MDIO phy
 * @param dev           subdevice select
 * @param addr          MDIO subdevice register
 * @param data          Pointer to a 16-bit data area
 *
 * a low-level API which does no locking.
 *
 * @return 0 on success, or -1 on error (and set errno accordingly).
 */
NFP_API
int nfp_mdio45_transact(struct nfp_mdio *mdio, int hz, int pream_len, int phy, int dev, int addr, int rnw, uint16_t *data);

/**
 * MDIO Clause 45 read/write via the NFP's MDIO controller
 *
 * @param mdio          MDIO device
 * @param hz            MDIO bus frequency (uses default if <= 0)
 * @param pream_len     preamble length, max 32 (uses default if <= 0)
 * @param phy           MDIO phy
 * @param dev           subdevice select
 * @param addr          MDIO subdevice register
 * @param data          Pointer to a 16-bit data area
 *
 * a low-level API which does locking.
 *
 * @return 0 on success, or -1 on error (and set errno accordingly).
 */
NFP_API
int nfp_mdio45_locked_transact(struct nfp_mdio *mdio, int hz, int pream_len, int phy, int dev, int addr, int rnw, uint16_t *data, int proxy_check);

/**
 * Open NFP error reporting device.
 *
 * @param devnum        NFP device number
 * @param flags         extra flags to open call (O_RDONLY implied)
 *
 * Convenience wrapper around open(2) on NFP error device file.  The
 * @a flags parameter is passed on to the open call and can be used for
 * setting things like O_NONBLOCK.
 *
 * The returned file descriptor can be used in read(2), poll(2),
 * select(2), or similar calls to poll for NFP errors.  A read call
 * must always be exactly two 32-bit words long.  The read call will
 * block until errors are observed (unless the file was opened with
 * O_NONBLOCK).  The first word returned contains number of observed
 * errors since previous read, and the next word contains the last
 * error event as seen on the NFP event bus.  See Netronome NFP-32xx
 * Databook for a description of the event bus error events.
 *
 * Note that if errors are occurring too frequently the event filters
 * may be overloaded and some events may be lost.  The returned error
 * count should therefore only be seen as the minimum number of errors
 * that are known to have happened.
 *
 * @return file descriptor on success, or -1 on error (and set errno
 * accordingly).
 */
NFP_API
int nfp_err_open(unsigned int devnum, int flags);

__END_DECLS;

#endif /* !__NFP_H__ */
