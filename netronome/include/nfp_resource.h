/*
 * Copyright (C) 2014, Netronome Systems, Inc.
 * All right reserved.
 *
 */

#ifndef NFP_RESOURCE_H
#define NFP_RESOURCE_H

#include <nfp.h>

/**
 * Opaque handle to a NFP Resource
 */
struct nfp_resource;

/**
 * Allocate and acquire a NFP Resource for exclusive use
 * @param[in]   nfp     NFP Device handle
 * @param[in]   name    Name of the NFP Resource
 * @param[in]   cpp_id  NFP CPP ID of the location to reserve
 * @param[in]   address Address in the NFP CPP ID space to reserve
 * @param[in]   size    Size of the are to reserve, in bytes
 * @return      NFP Resource handle, or NULL and errno
 */
struct nfp_resource *nfp_resource_alloc_acquire(struct nfp_device *nfp,
						const char *name,
						uint32_t cpp_id, uint64_t address,
						uint64_t size);

/**
 * Acquire a NFP Resource by name for exclusive use
 * @param[in]   nfp     NFP Device handle
 * @param[in]   name    Name of the NFP Resource
 * @return      NFP Resource handle, or NULL and errno
 */
struct nfp_resource *nfp_resource_acquire(struct nfp_device *nfp,
					  const char *name);

/**
 * Attempt acquire of a NFP Resource by name for exclusive use
 * @param[in]   nfp          NFP Device handle
 * @param[in]   name         Name of the NFP Resource
 * @param[in]   ms_timeout   millisecond timeout value
 * @return      NFP Resource handle, or NULL and errno
 */
struct nfp_resource *nfp_resource_tryacquire(struct nfp_device *nfp,
					     const char *name, int ms_timeout);

/**
 * Release a NFP Resource, and free the handle
 * @param[in]   res     NFP Resource handle
 */
void nfp_resource_release(struct nfp_resource *res);

/**
 * Release a NFP Resource, free the handle and remove the resource
 * from the resource list.
 * Note that this function will prevent removal of the root resource
 * but not of other important resources, use with caution.
 *
 * The caller must acquire and hold the root resource before calling this
 * function.
 *
 * @param[in]   res     NFP Resource handle
 * @return              0 on success; -1 on error with errno set
 */
int nfp_resource_remove(struct nfp_resource *res);

/**
 * Modify the attributes of an existing resource.
 * The caller must acquire this resource before modifying it.
 *
 * Use with extreme caution.
 *
 * @param[in]   res     NFP Resource handle
 * @param       cpp_id  NFP CPP Id
 * @param       address NFP CPP address
 * @param       size    Size of the resource
 * @return              0 on success; -1 on error with errno set
 */
int nfp_resource_modify(struct nfp_resource *res,
			uint32_t cpp_id, uint64_t address, uint64_t size);

/**
 * Obtain a list of resources defined in the root resource.
 *
 * Important notes:
 * - The caller must nfp_resource_list_free(resources) when done.
 * - Calling this function does not acquire any of the resources in the list.
 * - This function does not lock the root resource, so the data must
 *   be considered unreliable unless the root resource has been locked
 *   by the caller.
 *
 * @param[in]   nfp         NFP Device handle
 * @param[out]  resources   Will be set to a new array of pointers to resources
 * @return      Number of entries in the resources array. 0 if none, no error.
 */
size_t nfp_resource_list(struct nfp_device *nfp,
			 const struct nfp_resource ***resources);

/**
 * Free a resource list.
 *
 * @param[in]  resources   Array of pointers to resources
 * @param[in]  cnt         Number of resources in array
 */
void nfp_resource_list_free(const struct nfp_resource **resources, size_t cnt);

/**
 * Return the CPP ID of a NFP Resource
 * @param[in]   res     NFP Resource handle
 * @return      CPP ID of the NFP Resource
 */
uint32_t nfp_resource_cpp_id(const struct nfp_resource *res);

/**
 * Return the name of a NFP Resource
 * @param[in]   res     NFP Resource handle
 * @return      Name of the NFP Resource
 */
const char *nfp_resource_name(const struct nfp_resource *res);

/**
 * Return the target address of a NFP Resource
 * @param[in]   res     NFP Resource handle
 * @return      Address of the NFP Resource
 */
uint64_t nfp_resource_address(const struct nfp_resource *res);

/**
 * Return the size of a NFP Resource
 * @param[in]   res     NFP Resource handle
 * @return      Size of the NFP Resource
 */
uint64_t nfp_resource_size(const struct nfp_resource *res);

/**
 * Return the mutex of a NFP Resource
 * @param[in]   res     NFP Resource handle
 * @return      mutex pointer
 */
struct nfp_cpp_mutex *nfp_resource_mutex(const struct nfp_resource *res);

int nfp_resource_read8(const struct nfp_resource *res, uint64_t offs, uint8_t *val);

int nfp_resource_write8(const struct nfp_resource *res, uint64_t offs, uint8_t val);

int nfp_resource_read64(const struct nfp_resource *res, uint64_t offs, uint64_t *val);

int nfp_resource_write64(const struct nfp_resource *res, uint64_t offs, uint64_t val);

struct res_stat {
	char name[9];
	uint64_t tot_time;
	uint64_t min_time;
	uint64_t max_time;
	uint64_t count;
};

int nfp_resource_get_stat(struct nfp_device *nfp, int *idx,
			  struct res_stat **r);

#endif /* NFP_RESOURCE_H */
