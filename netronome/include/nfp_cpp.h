/*
 * Copyright (C) 2008-2010,2017 Netronome Systems, Inc.  All rights reserved.
 *
 * @file          nfp_cpp.h
 * @brief         Interface for low-level NFP CPP access.
 *
 */
#ifndef __NFP_CPP_H__
#define __NFP_CPP_H__

#include <nfp-common/nfp_platform.h>
#include <nfp-common/nfp_resid.h>

#ifdef __KERNEL__
#define NO_NFP_API_EXPORT
#endif

/* API is part of libnfp */
#include "_nfp_apiex.h"

/**
 * @defgroup nfp6000-only NFP6000 Only Functions
 */

/**
 * NFP CPP device area handle
 */
struct nfp_cpp_area;

/**
 * NFP CPP handle
 */
struct nfp_cpp;

__BEGIN_DECLS;

/**
 * Don't capture CPP read actions.
 */
#define NFP_CPP_TRACE_FLG_DROP_READ         BIT(0)

/**
 * Don't capture CPP write actions.
 */
#define NFP_CPP_TRACE_FLG_DROP_WRITE        BIT(1)

/**
 * Don't capture CPP explicit actions.
 */
#define NFP_CPP_TRACE_FLG_DROP_EXPL         BIT(2)

#define NFP_CPP_TRACE_FLGS_DROP_ALL         (NFP_CPP_TRACE_FLG_DROP_READ | \
					     NFP_CPP_TRACE_FLG_DROP_WRITE | \
					     NFP_CPP_TRACE_FLG_DROP_EXPL)

/**
 * Ignore the values that were read.
 */
#define NFP_CPP_TRACE_FLG_IGN_RDVAL         BIT(3)

/**
 * Wait a long time for the expected value, useful when polling for
 * a specific event or state. Not useful if NFP_CPP_TRACE_FLG_DROP_READ or
 * NFP_CPP_TRACE_FLG_IGN_RDVAL is also set.
 */
#define NFP_CPP_TRACE_FLG_POLL              BIT(4)

/*
 * NFP CPP core interface for CPP clients.
 */

/**
 * Open a NFP CPP handle to a CPP device
 *
 * @param[in]	id	0-based ID for the CPP interface to use
 *
 * @return NFP CPP handle, or NULL on failure (and set errno
 * accordingly).
 */
NFP_API
struct nfp_cpp *nfp_cpp_from_device_id(int id);

/**
 * Free a NFP CPP handle
 *
 * @param[in]	cpp	NFP CPP handle
 */
NFP_API
void nfp_cpp_free(struct nfp_cpp *cpp);

/**
 * \brief NFP_CPP_MODEL_INVALID - invalid model id
 */
#define NFP_CPP_MODEL_INVALID   0xffffffff

/**
 * \brief NFP_CPP_MODEL_CHIP_of - retrieve the chip ID from the model ID
 *
 * The chip ID is a 16-bit BCD+A-F encoding for the chip type.
 *
 * @param[in]   model   NFP CPP model id
 * @return      NFP CPP chip id
 */
#define NFP_CPP_MODEL_CHIP_of(model)        (((model) >> 16) & 0xffff)

/**
 * \brief NFP_CPP_MODEL_FAMILY_of - retrieve the chip family from the model ID
 *
 * The chip family is one of:
 * NFP_CHIP_FAMILY_NFP3800
 * NFP_CHIP_FAMILY_NFP6000
 *
 * @param[in]   model   NFP CPP model id
 * @return      NFP Chip family, -1 if family undetermined
 */
#define NFP_CPP_MODEL_FAMILY_of(model) \
	(NFP_CPP_MODEL_IS_6000(model) ? NFP_CHIP_FAMILY_NFP6000 : \
	 NFP_CPP_MODEL_IS_3800(model) ? NFP_CHIP_FAMILY_NFP3800 : -1)

/**
 * \brief NFP_CPP_MODEL_STEPPING_of - retrieve the revision ID from the model ID
 *
 * The revison ID is a 8-bit encoding of the chip revision.
 * Model A0 is 0x00, B4 is 0x14, G2 is 0x62 etc.
 *
 * @param[in]	model	NFP CPP model id
 * @return		NFP CPP stepping id
 */
#define NFP_CPP_MODEL_STEPPING_of(model)	(((model) >>  0) & 0x00ff)

/**
 * \brief NFP_CPP_STEPPING - Generate a NFP CPP stepping code
 *
 * The revison ID is a 8-bit encoding of the chip revision.
 * Stepping A0 is 0x00, B4 is 0x14, G9 is 0x69 etc.
 *
 * @param[in]	_str_major_minor	NFP CPP stepping major minor
 * @return		NFP CPP stepping
 */
#ifdef __KERNEL__
#include <linux/ctype.h>
#else
#include <ctype.h>
#endif
static nfp_inline int NFP_CPP_STEPPING_decode(const char *_str_major_minor)
{
	return ((toupper(_str_major_minor[0]) -
		 'A') << 4) | ((_str_major_minor[1] - '0'));
}

#define NFP_CPP_STEPPING(major_minor)	NFP_CPP_STEPPING_decode(#major_minor)

/**
 * \brief NFP_CPP_MODEL_IS_6000 - Check for the NFP6000 family of devices
 *
 * NOTE: The NFP4000 series is considered as a NFP6000 series variant.
 *
 * @param[in]	model	NFP CPP model id
 * @return		true if model is in the NFP6000 family, false otherwise.
 */
#define NFP_CPP_MODEL_IS_6000(model)		     \
	((NFP_CPP_MODEL_CHIP_of(model) >= 0x4000) && \
	 (NFP_CPP_MODEL_CHIP_of(model) < 0x7000))

/**
 * \brief NFP_CPP_MODEL_IS_3800 - Check for the NFP3800 family of devices
 *
 * @param[in]	model	NFP CPP model id
 * @return		true if model is in the NFP3800 family, false otherwise.
 */
#define NFP_CPP_MODEL_IS_3800(model)		     \
	((NFP_CPP_MODEL_CHIP_of(model) >= 0x3800) && \
	 (NFP_CPP_MODEL_CHIP_of(model) < 0x3900))

/**
 * \brief NFP_CPP_MODEL_IS_THORNHAM - Check for the Thornham family of devices
 *
 * @param[in]	model	NFP CPP model id
 * @return		true if model is in the NFP3800 or NFP6000 family, false otherwise.
 */
#define NFP_CPP_MODEL_IS_THORNHAM(model)		     \
    ((NFP_CPP_MODEL_IS_6000(model))||(NFP_CPP_MODEL_IS_3800(model)))

/**
 * \brief nfp_cpp_model - Retrieve the Model ID of the NFP
 *
 * @param[in]	cpp	NFP CPP handle
 * @return		NFP CPP Model ID
 */
NFP_API
uint32_t nfp_cpp_model(struct nfp_cpp *cpp);

/**
 * NFP Interface types - logical interface for this CPP connection
 * 4 bits are reserved for interface type.
 */
#define NFP_CPP_INTERFACE_TYPE_INVALID		0x0
#define NFP_CPP_INTERFACE_TYPE_PCI		0x1
#define NFP_CPP_INTERFACE_TYPE_ARM		0x2
#define NFP_CPP_INTERFACE_TYPE_RPC		0x3
#define NFP_CPP_INTERFACE_TYPE_ILA		0x4

/**
 * Construct a 16-bit NFP Interface ID
 *
 * Interface IDs consists of 4 bits of interface type,
 * 4 bits of unit identifier, and 8 bits of channel identifier.
 *
 * The NFP Interface ID is used in the implementation of
 * NFP CPP API mutexes, which use the MU Atomic CompareAndWrite
 * operation - hence the limit to 16 bits to be able to
 * use the NFP Interface ID as a lock owner.
 *
 * @param[in]	type	NFP Interface Type
 * @param[in]	unit	Unit identifier for the interface type
 * @param[in]	channel	Channel identifier for the interface unit
 * @return		Interface ID
 */
#define NFP_CPP_INTERFACE(type, unit, channel)	\
	((((type) & 0xf) << 12) | \
	 (((unit) & 0xf) <<  8) | \
	 (((channel) & 0xff) << 0))

/**
 * Get the interface type of a NFP Interface ID
 * @param[in]	interface	NFP Interface ID
 * @return			NFP Interface ID's type
 */
#define NFP_CPP_INTERFACE_TYPE_of(interface)	(((interface) >> 12) & 0xf)

/**
 * Get the interface unit of a NFP Interface ID
 * @param[in]	interface	NFP Interface ID
 * @return			NFP Interface ID's unit
 */
#define NFP_CPP_INTERFACE_UNIT_of(interface)	(((interface) >>  8) & 0xf)

/**
 * Get the interface channel of a NFP Interface ID
 * @param[in]	interface	NFP Interface ID
 * @return			NFP Interface ID's channel
 */
#define NFP_CPP_INTERFACE_CHANNEL_of(interface)	(((interface) >>  0) & 0xff)

/**
 * Retrieve the Interface ID of the NFP
 * @param[in]	cpp	NFP CPP handle
 * @return		NFP CPP Interface ID
 */
NFP_API
uint16_t nfp_cpp_interface(struct nfp_cpp *cpp);

/**
 * Retrieve the NFP Serial Number (unique per NFP)
 * @param[in]	cpp	NFP CPP handle
 * @param[out]	serial	Pointer to reference the serial number array
 *
 * @return	size of the NFP6000 serial number, in bytes
 */
NFP_API
int nfp_cpp_serial(struct nfp_cpp *cpp,
		   const uint8_t **serial);

/**
 * @ingroup nfp6000-only
 * Retrieve the IMB CPPAT CSR for the given CPP target from the local driver
 * island.
 * @param[in]   cpp NFP CPP handle
 * @param[in]   cpp_tgt CPP Target ID
 *
 * @return  CSR value, UINT32_MAX on error.
 */
NFP_API
uint32_t nfp_cpp_local_imb_cppat(struct nfp_cpp *cpp,
				 int cpp_tgt);

/**
 * Allocate a NFP CPP area handle, as an offset into a CPP ID
 * @param[in]	cpp	NFP CPP handle
 * @param[in]	cpp_id	NFP CPP ID
 * @param[in]	address	Offset into the NFP CPP ID address space
 * @param[in]	size	Size of the area to reserve
 *
 * @return NFP CPP handle, or NULL on failure (and set errno accordingly).
 */
NFP_API
struct nfp_cpp_area *nfp_cpp_area_alloc(struct nfp_cpp *cpp,
					uint32_t cpp_id,
					unsigned long long address,
					unsigned long size);

/**
 * Allocate a NFP CPP area handle, as an offset into a CPP ID, by a named owner
 * @param[in]	cpp	NFP CPP handle
 * @param[in]	cpp_id	NFP CPP ID
 * @param[in]	name	Name of owner of the area
 * @param[in]	address	Offset into the NFP CPP ID address space
 * @param[in]	size	Size of the area to reserve
 *
 * @return NFP CPP handle, or NULL on failure (and set errno accordingly).
 *
 */
NFP_API
struct nfp_cpp_area *nfp_cpp_area_alloc_with_name(struct nfp_cpp *cpp,
						  uint32_t cpp_id,
						  const char *name,
						  unsigned long long address,
						  unsigned long size);

/**
 * Free an allocated NFP CPP area handle
 * @param[in]	area	NFP CPP area handle
 */
NFP_API
void nfp_cpp_area_free(struct nfp_cpp_area *area);

/**
 * Acquire the resources needed to access the NFP CPP area handle
 *
 * @param[in]	area	NFP CPP area handle
 *
 * @return 0 on success, -1 on failure (and set errno accordingly).
 */
NFP_API
int nfp_cpp_area_acquire(struct nfp_cpp_area *area);

/**
 * Release the resources needed to access the NFP CPP area handle
 *
 * @param[in]	area	NFP CPP area handle
 */
NFP_API
void nfp_cpp_area_release(struct nfp_cpp_area *area);

/**
 * Allocate, then acquire the resources needed to access the NFP CPP area handle
 * @param[in]	cpp	NFP CPP handle
 * @param[in]	cpp_id	NFP CPP ID
 * @param[in]	address	Offset into the NFP CPP ID address space
 * @param[in]	size	Size of the area to reserve
 *
 * @return NFP CPP handle, or NULL on failure (and set errno accordingly).
 */
NFP_API
struct nfp_cpp_area *nfp_cpp_area_alloc_acquire(struct nfp_cpp *cpp,
						uint32_t cpp_id,
						unsigned long long address,
						unsigned long size);

/**
 * Release the resources, then free the NFP CPP area handle
 * @param[in]	area	NFP CPP area handle
 */
NFP_API
void nfp_cpp_area_release_free(struct nfp_cpp_area *area);

/**
 * Return an IO pointer to the beginning of the NFP CPP area handle
 * The area must be acquired with 'nfp_cpp_area_acquire()' before
 * calling this operation.
 *
 * @param[in]	area	NFP CPP area handle
 *
 * @return Pointer to IO memory, or NULL on failure (and set errno
 * accordingly).
 */
NFP_API
void *nfp_cpp_area_mapped(struct nfp_cpp_area *area);

/**
 * Read from a NFP CPP area handle into a buffer
 * The area must be acquired with 'nfp_cpp_area_acquire()' before
 * calling this operation.
 *
 * @param[in]	area	NFP CPP area handle
 * @param[in]	offset	Offset into the area
 * @param[in]	buffer	Location of buffer to receive the data
 * @param[in]	length	Length of the data to read
 *
 * @return bytes read on success, -1 on failure (and set errno
 * accordingly).
 *
 * KERNEL: This operation is safe to call in interrupt or softirq context.
 *
 */
NFP_API
int nfp_cpp_area_read(struct nfp_cpp_area *area,
		      unsigned long offset,
		      void *buffer,
		      size_t length);
/**
 * Write to a NFP CPP area handle from a buffer
 * The area must be acquired with 'nfp_cpp_area_acquire()' before
 * calling this operation.
 *
 * @param[in]	area	NFP CPP area handle
 * @param[in]	offset	Offset into the area
 * @param[in]	buffer	Location of buffer that holds the data
 * @param[in]	length	Length of the data to read
 *
 * @return bytes written on success, -1 on failure (and set errno
 * accordingly).
 *
 * KERNEL: This operation is safe to call in interrupt or softirq context.
 *
 */
NFP_API
int nfp_cpp_area_write(struct nfp_cpp_area *area,
		       unsigned long offset,
		       const void *buffer,
		       size_t length);
/**
 * Verify that IO can be performed on an offset in an area
 *
 * @param[in]	area	NFP CPP area handle
 * @param[in]	offset	Offset into the area
 * @param[in]	size	Size of region to validate
 *
 * @return 0 on success, -1 on failure (and set errno accordingly).
 */
NFP_API
int nfp_cpp_area_check_range(struct nfp_cpp_area *area,
			     unsigned long long offset,
			     unsigned long size);

/**
 * Get the NFP CPP handle that is the parent of a NFP CPP area handle
 *
 * @param	cpp_area	NFP CPP area handle
 * @return			NFP CPP handle
 */
NFP_API
struct nfp_cpp *nfp_cpp_area_cpp(struct nfp_cpp_area *cpp_area);

/**
 * Get the name passed during allocation of the NFP CPP area handle
 *
 * @param	cpp_area	NFP CPP area handle
 * @return			Pointer to the area's name
 */
NFP_API
const char *nfp_cpp_area_name(struct nfp_cpp_area *cpp_area);

/**
 * Read a block of data from a NFP CPP ID
 *
 * @param[in]	cpp	NFP CPP handle
 * @param[in]	cpp_id	NFP CPP ID
 * @param[in]	address	Offset into the NFP CPP ID address space
 * @param[in]	kernel_vaddr	Buffer to copy read data to
 * @param[in]	length	Size of the area to reserve
 *
 * @return bytes read on success, -1 on failure (and set errno
 * accordingly).
 */
NFP_API
int nfp_cpp_read(struct nfp_cpp *cpp,
		 uint32_t cpp_id,
		 unsigned long long address,
		 void *kernel_vaddr,
		 size_t length);
/**
 * Write a block of data to a NFP CPP ID
 *
 * @param[in]	cpp	NFP CPP handle
 * @param[in]	cpp_id	NFP CPP ID
 * @param[in]	address	Offset into the NFP CPP ID address space
 * @param[in]	kernel_vaddr	Buffer to copy write data from
 * @param[in]	length	Size of the area to reserve
 *
 * @return bytes written on success, -1 on failure (and set errno
 * accordingly).
 */
NFP_API
int nfp_cpp_write(struct nfp_cpp *cpp,
		  uint32_t cpp_id,
		  unsigned long long address,
		  const void *kernel_vaddr,
		  size_t length);
/**
 * Fill a NFP CPP area handle and offset with a value
 *
 * @param[in]	area	NFP CPP area handle
 * @param[in]	offset	Offset into the NFP CPP ID address space
 * @param[in]	value	32-bit value to fill area with
 * @param[in]	length	Size of the area to reserve
 *
 * @return bytes written on success, -1 on failure (and set errno
 * accordingly).
 *
 * KERNEL: This operation is safe to call in interrupt or softirq context.
 *
 */
NFP_API
int nfp_cpp_area_fill(struct nfp_cpp_area *area,
		      unsigned long offset,
		      uint32_t value,
		      size_t length);

/**
 * Read a single 32-bit value from a NFP CPP area handle
 *
 * @param area		NFP CPP area handle
 * @param offset	offset into NFP CPP area handle
 * @param value		output value
 *
 * The area must be acquired with 'nfp_cpp_area_acquire()' before
 * calling this operation.
 *
 * NOTE: offset must be 32-bit aligned.
 *
 * KERNEL: This operation is safe to call in interrupt or softirq context.
 *
 * @return 0 on success, or -1 on error (and set errno accordingly).
 */
NFP_API
int nfp_cpp_area_readl(struct nfp_cpp_area *area,
		       unsigned long offset,
		       uint32_t *value);

/**
 * Write a single 32-bit value to a NFP CPP area handle
 *
 * @param area		NFP CPP area handle
 * @param offset	offset into NFP CPP area handle
 * @param value		value to write
 *
 * The area must be acquired with 'nfp_cpp_area_acquire()' before
 * calling this operation.
 *
 * NOTE: offset must be 32-bit aligned.
 *
 * KERNEL: This operation is safe to call in interrupt or softirq context.
 *
 * @return 0 on success, or -1 on error (and set errno accordingly).
 */
NFP_API
int nfp_cpp_area_writel(struct nfp_cpp_area *area,
			unsigned long offset,
			uint32_t value);

/**
 * Read a single 64-bit value from a NFP CPP area handle
 *
 * @param area		NFP CPP area handle
 * @param offset	offset into NFP CPP area handle
 * @param value		output value
 *
 * The area must be acquired with 'nfp_cpp_area_acquire()' before
 * calling this operation.
 *
 * NOTE: offset must be 64-bit aligned.
 *
 * KERNEL: This operation is safe to call in interrupt or softirq context.
 *
 * @return 0 on success, or -1 on error (and set errno accordingly).
 */
NFP_API
int nfp_cpp_area_readq(struct nfp_cpp_area *area,
		       unsigned long offset,
		       uint64_t *value);

/**
 * Write a single 64-bit value to a NFP CPP area handle
 *
 * @param area		NFP CPP area handle
 * @param offset	offset into NFP CPP area handle
 * @param value		value to write
 *
 * The area must be acquired with 'nfp_cpp_area_acquire()' before
 * calling this operation.
 *
 * NOTE: offset must be 64-bit aligned.
 *
 * KERNEL: This operation is safe to call in interrupt or softirq context.
 *
 * @return 0 on success, or -1 on error (and set errno accordingly).
 */
NFP_API
int nfp_cpp_area_writeq(struct nfp_cpp_area *area,
			unsigned long offset,
			uint64_t value);

/**
 * Write a single 32-bit value on the XPB bus
 *
 * @param cpp           NFP CPP device handle
 * @param xpb_tgt	XPB target and address
 * @param value         value to write
 *
 * KERNEL: This operation is safe to call in interrupt or softirq context.
 *
 * @return 0 on success, or -1 on failure (and set errno accordingly).
 */
NFP_API
int nfp_xpb_writel(struct nfp_cpp *cpp,
		   uint32_t xpb_tgt,
		   uint32_t value);

/**
 * Read a single 32-bit value from the XPB bus
 *
 * @param cpp           NFP CPP device handle
 * @param xpb_tgt	XPB target and address
 * @param value         output value
 *
 * KERNEL: This operation is safe to call in interrupt or softirq context.
 *
 * @return 0 on success, or -1 on failure (and set errno accordingly).
 */
NFP_API
int nfp_xpb_readl(struct nfp_cpp *cpp,
		  uint32_t xpb_tgt,
		  uint32_t *value);

/**
 * Modify bits of a 32-bit value from the XPB bus
 *
 * @param cpp           NFP CPP device handle
 * @param xpb_tgt       XPB target and address
 * @param mask          mask of bits to alter
 * @param value         value to modify
 *
 * KERNEL: This operation is safe to call in interrupt or softirq context.
 *
 * @return 0 on success, or -1 on failure (and set errno accordingly).
 */
NFP_API
int nfp_xpb_writelm(struct nfp_cpp *cpp,
		    uint32_t xpb_tgt,
		    uint32_t mask,
		    uint32_t value);

/**
 * Modify bits of a 32-bit value from the XPB bus
 *
 * @param cpp           NFP CPP device handle
 * @param xpb_tgt       XPB target and address
 * @param mask          mask of bits to alter
 * @param value         value to monitor for
 * @param timeout_us    maximum number of us to wait (-1 for forever)
 *
 * @return >= 0 on success, or -1 on failure (and set errno accordingly).
 */
NFP_API
int nfp_xpb_waitlm(struct nfp_cpp *cpp,
		   uint32_t xpb_tgt,
		   uint32_t mask,
		   uint32_t value,
		   int timeout_us);

/**
 * Read a 32-bit word from a NFP CPP ID
 *
 * @param cpp           NFP CPP handle
 * @param cpp_id        NFP CPP ID
 * @param address       offset into the NFP CPP ID address space
 * @param value         output value
 *
 * @return 0 on success, or -1 on failure (and set errno accordingly).
 */
NFP_API
int nfp_cpp_readl(struct nfp_cpp *cpp,
		  uint32_t cpp_id,
		  unsigned long long address,
		  uint32_t *value);

/**
 * Write a 32-bit value to a NFP CPP ID
 *
 * @param cpp           NFP CPP handle
 * @param cpp_id        NFP CPP ID
 * @param address       offset into the NFP CPP ID address space
 * @param value         value to write
 *
 * @return 0 on success, or -1 on failure (and set errno accordingly).
 *
 */
NFP_API
int nfp_cpp_writel(struct nfp_cpp *cpp,
		   uint32_t cpp_id,
		   unsigned long long address,
		   uint32_t value);

/**
 * Read a 64-bit work from a NFP CPP ID
 *
 * @param cpp           NFP CPP handle
 * @param cpp_id        NFP CPP ID
 * @param address       offset into the NFP CPP ID address space
 * @param value         output value
 *
 * @return 0 on success, or -1 on failure (and set errno accordingly).
 */
NFP_API
int nfp_cpp_readq(struct nfp_cpp *cpp,
		  uint32_t cpp_id,
		  unsigned long long address,
		  uint64_t *value);

/**
 * Write a 64-bit value to a NFP CPP ID
 *
 * @param cpp           NFP CPP handle
 * @param cpp_id        NFP CPP ID
 * @param address       offset into the NFP CPP ID address space
 * @param value         value to write
 *
 * @return 0 on success, or -1 on failure (and set errno accordingly).
 */
NFP_API
int nfp_cpp_writeq(struct nfp_cpp *cpp,
		   uint32_t cpp_id,
		   unsigned long long address,
		   uint64_t value);

struct nfp_cpp_mutex;

/**
 * Initialize a mutex location

 * The CPP target:address must point to a 64-bit aligned location, and
 * will initialize 64 bits of data at the location.
 *
 * This creates the initial mutex state, as locked by this
 * nfp_cpp_interface().
 *
 * This function should only be called when setting up
 * the initial lock state upon boot-up of the system.
 *
 * @param cpp       NFP CPP handle
 * @param target	NFP CPP target ID (ie NFP_CPP_TARGET_CLS or NFP_CPP_TARGET_MU)
 * @param address	Offset into the address space of the NFP CPP target ID
 * @param key_id	Unique 32-bit value for this mutex
 *
 * @return 0 on success, or -1 on failure (and set errno accordingly).
 */
NFP_API
int nfp_cpp_mutex_init(struct nfp_cpp *cpp,
		       int target,
		       unsigned long long address,
		       uint32_t key_id);

/**
 * Create a mutex handle from an address controlled by a MU Atomic engine
 *
 * The CPP target:address must point to a 64-bit aligned location, and
 * reserve 64 bits of data at the location for use by the handle.
 *
 * Only target/address pairs that point to entities that support the
 * MU Atomic Engine's CmpAndSwap32 command are supported.
 *
 * @param cpp		NFP CPP handle
 * @param target	NFP CPP target ID (ie NFP_CPP_TARGET_CLS or NFP_CPP_TARGET_MU)
 * @param address	Offset into the address space of the NFP CPP target ID
 * @param key_id    32-bit unique key (must match the key at this location)
 *
 * @return		A non-NULL struct nfp_cpp_mutex * on success, NULL on failure.
 */
NFP_API
struct nfp_cpp_mutex *nfp_cpp_mutex_alloc(struct nfp_cpp *cpp,
					  int target,
					  unsigned long long address,
					  uint32_t key_id);

/**
 * Get the NFP CPP handle the mutex was created with
 *
 * @param   mutex   NFP mutex handle
 * @return          NFP CPP handle
 */
NFP_API
struct nfp_cpp *nfp_cpp_mutex_cpp(struct nfp_cpp_mutex *mutex);

/**
 * Get the mutex key
 *
 * @param   mutex   NFP mutex handle
 * @return          Mutex key
 */
NFP_API
uint32_t nfp_cpp_mutex_key(struct nfp_cpp_mutex *mutex);

/**
 * Get the mutex owner
 *
 * @param   mutex   NFP mutex handle
 * @return          Interface ID of the mutex owner
 *
 * NOTE: This is for debug purposes ONLY - the owner may change
 *       at any time, unless it has been locked by this NFP CPP handle.
 */
NFP_API
uint16_t nfp_cpp_mutex_owner(struct nfp_cpp_mutex *mutex);

/**
 * Get the mutex target
 *
 * @param   mutex   NFP mutex handle
 * @return          Mutex CPP target (ie NFP_CPP_TARGET_MU)
 */
NFP_API
int nfp_cpp_mutex_target(struct nfp_cpp_mutex *mutex);

/**
 * Get the mutex address
 *
 * @param   mutex   NFP mutex handle
 * @return          Mutex CPP address
 */
NFP_API
uint64_t nfp_cpp_mutex_address(struct nfp_cpp_mutex *mutex);

/**
 * Free a mutex handle - does not alter the lock state
 *
 * @param mutex		NFP CPP Mutex handle
 */
NFP_API
void nfp_cpp_mutex_free(struct nfp_cpp_mutex *mutex);

/**
 * Lock a mutex handle, using the NFP MU Atomic Engine
 *
 * @param mutex		NFP CPP Mutex handle
 *
 * @return 0 on success, or -1 on failure (and set errno accordingly).
 */
NFP_API
int nfp_cpp_mutex_lock(struct nfp_cpp_mutex *mutex);

/**
 * Unlock a mutex handle, using the NFP MU Atomic Engine
 *
 * @param mutex		NFP CPP Mutex handle
 *
 * @return 0 on success, or -1 on failure (and set errno accordingly).
 */
NFP_API
int nfp_cpp_mutex_unlock(struct nfp_cpp_mutex *mutex);

/**
 * Attempt to lock a mutex handle, using the NFP MU Atomic Engine
 *
 * @param mutex		NFP CPP Mutex handle
 * @return		0 if the lock succeeded, -1 on failure (and errno set appropriately).
 */
NFP_API
int nfp_cpp_mutex_trylock(struct nfp_cpp_mutex *mutex);

struct nfp_cpp_event;
struct sigaction;

/**
 * Create a NFP CPP event handle.
 *
 * Creates a handle to monitor Event Manager detected events.
 *
 * @param	cpp		NFP CPP handle
 * @param	event_match	EventManager style event match
 * @param	event_mask	EventManager style event mask
 * @param	type		EventManager event filter type (-1 for 'autoselect')
 *
 * @return	Non-NULL NFP CPP Signal handle, or NULL on error
 */
NFP_API
struct nfp_cpp_event *nfp_cpp_event_alloc(struct nfp_cpp *cpp,
					  uint32_t event_match,
					  uint32_t event_mask,
					  int type);

/**
 * Get the NFP CPP handle that is the parent of a NFP CPP event handle
 *
 * @param	cpp_event	NFP CPP event handle
 * @return			NFP CPP handle
 */
NFP_API
struct nfp_cpp *nfp_cpp_event_cpp(struct nfp_cpp_event *cpp_event);

/**
 * Monitor a NFP CPP event handle via a POSIX signal
 *
 * @param	event		NFP CPP Event handle
 * @param	signum		POSIX signal
 * @param	act		    struct sigaction for the handler
 * @return			0 on success, -1 (and errno set) on failure.
 */
NFP_API
int nfp_cpp_event_as_signal(struct nfp_cpp_event *event,
			    int signum,
			    const struct sigaction *act);

/**
 * Release an NFP CPP Event handle
 *
 * Removes any registered POSIX signal handlers, and sets the
 * signal be handled by the SIG_DFL handler.
 *
 * @param	event		NFP CPP event handle
 */
NFP_API
void nfp_cpp_event_free(struct nfp_cpp_event *event);

/**
 * Set flags to control the behaviour of CPP tracing when the
 * environment variable NFP_CPP_TRACE is set.
 * This is not a bit-wise OR, it is an assignment.
 *
 * To reset the flags to default, set 0.
 *
 * @param	cpp		NFP CPP handle
 * @param	flags	Bit mask of NFP_CPP_TRACE_FLG_*
 */
NFP_API
void nfp_cpp_trace_flags_set(struct nfp_cpp *cpp,
			     unsigned int flags);

/**
 * Get the current CPP trace flag mask.
 *
 * @param	cpp		NFP CPP handle
 * @return          Flags
 */
NFP_API
unsigned int nfp_cpp_trace_flags(struct nfp_cpp *cpp);

/**
 * Check if CPP tracing is enabled.
 *
 * @param	cpp		NFP CPP handle
 * @return 1 if enabled, 0 if none or error
 */
NFP_API
int nfp_cpp_trace_enabled(struct nfp_cpp *cpp);

/**
 * Used internally to add bitmasks to succeeding actions
 */
NFP_API
void nfp_cpp_trace_mask_put(struct nfp_cpp *cpp,
			    int byte_width,
			    void *buff,
			    size_t bytes);

__END_DECLS;

#endif /* !__NFP_CPP_H__ */
