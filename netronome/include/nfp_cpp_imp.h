/*
 * Copyright (C) 2008-2011, Netronome Systems, Inc.  All rights reserved.
 *
 * NFP CPP Implementation Specifics
 *
 * This defines the API for a NFP CPP Transport plugin
 *
 */

#ifndef NFP_CPP_IMP_H
#define NFP_CPP_IMP_H

#include <nfp_cpp.h>

struct nfp_cpp_explicit_command {
	uint32_t cpp_id;
	uint16_t data_ref;
	uint8_t data_master;
	uint8_t len;
	uint8_t byte_mask;
	uint8_t signal_master;
	uint8_t signal_ref;
	uint8_t posted;
	uint8_t siga;
	uint8_t sigb;
	int8_t siga_mode;
	int8_t sigb_mode;
};

/**
 * NFP CPP operations structure
 */
struct nfp_cpp_operations {
	/** Size of priv area in struct nfp_cpp_area */
	size_t area_priv_size;

	/** Size of priv event in struct nfp_cpp_event */
	size_t event_priv_size;

	/** Instance an NFP CPP from this template
	 */
	int (*init)(struct nfp_cpp *cpp, const char *device_id_template, int id);

	/** Free the bus
	 * Called only once, during nfp_cpp_unregister()
	 */
	void (*bus_free)(struct nfp_cpp *cpp);

	/** Initialize a new NFP CPP area
	 * NOTE: This is _not_ serialized
	 */
	int (*area_init)(struct nfp_cpp_area *area,
			 uint32_t dest,
			 unsigned long long address,
			 unsigned long size);
	/** Clean up a NFP CPP area before it is freed
	 * NOTE: This is _not_ serialized
	 */
	void (*area_cleanup)(struct nfp_cpp_area *area);

	/** Acquire resources for a NFP CPP area
	 * Serialized
	 */
	int (*area_acquire)(struct nfp_cpp_area *area);
	/** Release resources for a NFP CPP area
	 * Serialized
	 */
	void (*area_release)(struct nfp_cpp_area *area);
	/** Return a void IO pointer to a NFP CPP area
	 * NOTE: This is _not_ serialized
	 */
	void *(*area_mapped)(struct nfp_cpp_area *area);
	/** Perform a read from a NFP CPP area
	 * Serialized
	 */
	int (*area_read)(struct nfp_cpp_area *area,
			 void *kernel_vaddr,
			 unsigned long offset,
			 unsigned int length);
	/** Perform a write to a NFP CPP area
	 * Serialized
	 */
	int (*area_write)(struct nfp_cpp_area *area,
			  const void *kernel_vaddr,
			  unsigned long offset,
			  unsigned int length);

	/* Perform an explicit transaction
	 */
	int (*explicit_do)(struct nfp_cpp *cpp,
			   const struct nfp_cpp_explicit_command *cmd,
			   uint64_t address,
			   const void *in,
			   size_t in_len,
			   void *out,
			   size_t out_len);

	/** Event management
	 */
	int (*event_acquire)(struct nfp_cpp_event *event,
			     uint32_t match,
			     uint32_t mask,
			     uint32_t type,
			     int signal);
	void (*event_release)(struct nfp_cpp_event *event);
};

/**
 * This should be the only external function the transport
 * module supplies
 */
const struct nfp_cpp_operations *nfp_cpp_transport_operations(void);

/**
 * Set the model id
 *
 * @param   cpp     NFP CPP operations structure
 * @param   model   Model ID
 */
void nfp_cpp_model_set(struct nfp_cpp *cpp, uint32_t model);

/**
 * Set the private instance owned data of a nfp_cpp struct
 *
 * @param   cpp     NFP CPP operations structure
 * @param   interface Interface ID
 */
void nfp_cpp_interface_set(struct nfp_cpp *cpp, uint32_t interface);

/**
 * Set the private instance owned data of a nfp_cpp struct
 *
 * @param   cpp     NFP CPP operations structure
 * @param   serial  NFP serial byte array
 * @param   len     Length of the serial byte array
 */
int nfp_cpp_serial_set(struct nfp_cpp *cpp, const uint8_t *serial, size_t serial_len);

/**
 * Set the private data of the nfp_cpp instance
 *
 * @param   cpp NFP CPP operations structure
 * @return      Opaque device pointer
 */
void nfp_cpp_priv_set(struct nfp_cpp *cpp, void *priv);

/**
 * Return the private data of the nfp_cpp instance
 *
 * @param   cpp NFP CPP operations structure
 * @return      Opaque device pointer
 */
void *nfp_cpp_priv(struct nfp_cpp *cpp);

/**
 * Get the privately allocated portion of a NFP CPP area handle
 *
 * @param   cpp_area    NFP CPP area handle
 * @return          Pointer to the private area, or NULL on failure
 */
void *nfp_cpp_area_priv(struct nfp_cpp_area *cpp_area);

/**
 * Get the privately allocated portion of a NFP CPP event handle
 *
 * @param   cpp_event    NFP CPP event handle
 * @return          Pointer to the private event, or NULL on failure
 */
void *nfp_cpp_event_priv(struct nfp_cpp_event *cpp_event);

/**
 * KERNEL API:
 * Return the device that is the parent of the NFP CPP bus
 *
 * @param   cpp NFP CPP operations structure
 * @return      Opaque device pointer
 */

struct device *nfp_cpp_device(struct nfp_cpp *cpp);

#endif /* NFP_CPP_IMP_H */
