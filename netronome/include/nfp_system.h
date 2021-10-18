#ifndef __NFP_SYSTEM_H__
#define __NFP_SYSTEM_H__
#include <jansson.h>
#include <stdint.h>

struct nfp_sys;

/**
 * Open handle to NFP system.
 *
 * @return handle on success, or NULL on error (and set errno accordingly).
 */
struct nfp_sys *nfp_sys_open(void);

/**
 * Close NFP system handle.
 *
 * @param       handle          system handle
 *
 * @return 0 on success, or -1 on error (and set errno accordingly).
 */
int nfp_sys_close(struct nfp_sys *handle);

/**
 * Get NFP system label.
 *
 * @param       handle          system handle
 * @param       label           pointer to char pointer to be populated
 *
 * @return 0 on success, or -1 on error (and set errno accordingly).
 */
int nfp_sys_label(struct nfp_sys *handle, const char **label);

/**
 * Get system port count.
 *
 * @param       handle          system handle
 * @param       port_count      pointer to count value to be populated
 *
 * @return 0 on success, or -1 on error (and set errno accordingly).
 */
int nfp_sys_port_count(struct nfp_sys *handle, int *port_count);

/**
 * Get platform label for a specific port.
 *
 * @param       handle          system handle
 * @param       port_id         port index
 * @param       label           pointer to char pointer to be populated
 *
 * @return 0 on success, or -1 on error (and set errno accordingly).
 */
int nfp_sys_port_get_label(struct nfp_sys *handle, int port_id, const char **label);

/**
 * Get NFP ID, NBI ID and port number.
 *
 * @param       handle          system handle
 * @param       port_id         port index (within nfp_system)
 * @param       nfp_id          NFP index
 * @param       nbi_id          NBI index
 * @param       port_num        port index (within NBI)
 *
 * @return 0 on success, or -1 on error (and set errno accordingly).
 */
int nfp_sys_port_get_info(struct nfp_sys *s, int port_id,
			  int *nfp_id, int *nbi_id, int *port_num);

/**
 * Get port state, up/down.
 *
 * @param       handle          system handle
 * @param       port_id         port index
 * @param       state           ...
 *
 * @return 0 on success, or -1 on error (and set errno accordingly).
 */
int nfp_sys_port_state(struct nfp_sys *handle, int port_id, int *state);

/**
 * Bring up port.
 *
 * @param       handle          system handle
 * @param       port_id         port index
 *
 * @return 0 on success, or -1 on error (and set errno accordingly).
 */
int nfp_sys_port_ifup(struct nfp_sys *handle, int port_id);

/**
 * Bring down port.
 *
 * @param       handle          system handle
 * @param       port_id         port index
 *
 * @return 0 on success, or -1 on error (and set errno accordingly).
 */
int nfp_sys_port_ifdown(struct nfp_sys *handle, int port_id);

struct port_stats {
};

/**
 * Get statistics for specified port.
 *
 * @param       handle          system handle
 * @param       port_id         port index
 * @param       stats           pointer to stats struct to be populated
 *
 * @return 0 on success, or -1 on error (and set errno accordingly).
 */
int nfp_sys_port_stats(struct nfp_sys *handle, int port_id,
		       struct port_stats *stats);

/**
 * Get internal NFP port id.
 *
 * @param       handle          system handle
 * @param       port_id         system port index
 * @param       nfp_port_id     pointer to NFP port index value to be populated
 *
 * @return 0 on success, or -1 on error (and set errno accordingly).
 */
int nfp_sys_port_nfp_port_id(struct nfp_sys *handle, int port_id,
			     int *nfp_port_id);

/**
 * Get Plugable module index of specified port.
 *
 * @param       handle          system handle
 * @param       port_id         port index
 * @param       plugmod_id      pointer to plugable mod id value to be populated
 *
 * @return 0 on success, or -1 on error (and set errno accordingly).
 */
int nfp_sys_port_plugable_module_id(struct nfp_sys *handle, int port_id,
				    int *plugmod_id);

/**
 * Get NFP index of specified port.
 *
 * @param       handle          system handle
 * @param       port_id         port index
 * @param       nfp_id          pointer to NFP id value to be populated
 *
 * @return 0 on success, or -1 on error (and set errno accordingly).
 */
int nfp_sys_port_nfp_id(struct nfp_sys *handle, int port_id, int *nfp_id);

/**
 * Get PCIe bus device of specified port.
 *
 * @param       handle          system handle
 * @param       port_id         port index
 * @param       dom             PCIE domain
 * @param       bus             PCIE bus
 * @param       dev             PCIE dev
 * @param       fn              PCIE function
 *
 * @return 0 on success, or -1 on error (and set errno accordingly).
 */
int nfp_sys_port_nfp_pci_dev(struct nfp_sys *handle, int port_id, int *dom,
			     int *bus, int *dev, int *fn);

/**
 * Get system plugable module count.
 *
 * @param       handle          system handle
 * @param       plugmod_count   pointer to count value to be populated
 *
 * @return 0 on success, or -1 on error (and set errno accordingly).
 */
int nfp_sys_plugable_module_count(struct nfp_sys *handle, int *plugmod_count);

/**
 * Get plugable module NFP index.
 *
 * @param       handle          system handle
 * @param       plugmod_id      plugmod index
 * @param       nfp_id          pointer to NFP id value to be populated
 *
 * @return 0 on success, or -1 on error (and set errno accordingly).
 */
int nfp_sys_get_plugable_module_nfp_id(struct nfp_sys *s, int plugmod_id,
				       int *nfp_id);

/**
 * Get plugable module manufacturer string.
 *
 * @param       handle          system handle
 * @param       plugmod_id      plugmod index
 * @param       manufacturer    pointer to char pointer to be populated
 *
 * @return 0 on success, or -1 on error (and set errno accordingly).
 */
int nfp_sys_get_plugable_module_manufacturer(struct nfp_sys *handle,
					     int plugmod_id,
					     const char **manufacturer);

/**
 * Get plugable module product string.
 *
 * @param       handle          system handle
 * @param       plugmod_id      plugmod index
 * @param       product         pointer to char pointer to be populated
 *
 * @return 0 on success, or -1 on error (and set errno accordingly).
 */
int nfp_sys_get_plugable_module_product(struct nfp_sys *handle,
					int plugmod_id, const char **product);

/**
 * Get plugable module serial string.
 *
 * @param       handle          system handle
 * @param       plugmod_id      plugmod index
 * @param       serial          pointer to char pointer to be populated
 *
 * @return 0 on success, or -1 on error (and set errno accordingly).
 */
int nfp_sys_get_plugable_module_serial(struct nfp_sys *handle,
				       int plugmod_id, const char **serial);

/**
 * Get count of NFPs in system.
 *
 * @param       handle          system handle
 * @param       nfp_count       pointer to count value to be populated
 *
 * @return 0 on success, or -1 on error (and set errno accordingly).
 */
int nfp_sys_nfp_count(struct nfp_sys *handle, int *nfp_count);

/**
 * Get nfp_device struct of indexed NFP.
 *
 * @param       handle          system handle
 * @param       nfp_id          NFP index
 *
 * @return nfp_device on success, or NULL on error (and set errno accordingly).
 */
struct nfp_device *nfp_sys_get_nfp(struct nfp_sys *handle, int nfp_id);

/**
 * Get mask of all /dev/nfp-cpp-X files associated with the NFP.
 *
 * @param       handle          system handle
 * @param       nfp_id          NFP index
 * @param       dev_nfp_cpp     pointer to mask to be populated
 *
 * @return 0 on success, or -1 on error (and set errno accordingly).
 */
int nfp_sys_get_nfp_cpp_dev_mask(struct nfp_sys *s, int nfp_id,
				 uint32_t *dev_nfp_cpp_mask);

/**
 * Get board identification string of a specific NFP.
 *
 * @param       handle          system handle
 * @param       nfp_id          NFP index
 * @param       product_id      pointer to char pointer to be populated
 * @param       revision        pointer to char pointer to be populated
 * @param       model           pointer to char pointer to be populated
 *
 * @return 0 on success, or -1 on error (and set errno accordingly).
 */
int nfp_sys_get_nfp_identification(struct nfp_sys *s, int nfp_id,
				   const char **product_id,
				   const char **revision,
				   const char **model);

/**
 * Get configurator version string of a specific NFP.
 *
 * @param       handle          system handle
 * @param       nfp_id          NFP index
 * @param       configurator    pointer to char pointer to be populated
 *
 * @return 0 on success, or -1 on error (and set errno accordingly).
 */
int nfp_sys_get_nfp_configurator(struct nfp_sys *s, int nfp_id,
				 const char **configurator);

/**
 * Get BSP version string of a specific NFP.
 *
 * @param       handle          system handle
 * @param       nfp_id          NFP index
 * @param       bspver          pointer to char pointer to be populated
 *
 * @return 0 on success, or -1 on error (and set errno accordingly).
 */
int nfp_sys_get_nfp_bsp_version(struct nfp_sys *s, int nfp_id,
				const char **bspver);

/**
 * ...
 *
 * @param       handle          system handle
 * @param       nfp_id          NFP index
 * @param       type            ...
 *
 * @return 0 on success, or -1 on error (and set errno accordingly).
 */
int nfp_sys_get_nfp_type(struct nfp_sys *handle, int nfp_id, int *type);

/**
 * ...
 *
 * @param       handle          system handle
 * @param       nfp_id          NFP index
 * @param       state           ...
 *
 * @return 0 on success, or -1 on error (and set errno accordingly).
 */
int nfp_sys_get_nfp_state(struct nfp_sys *handle, int nfp_id, int *state);

/**
 * ...
 *
 * @param       handle          system handle
 * @param       nfp_id          NFP index
 * @param       prop            ...
 * @param       val             ...
 *
 * @return 0 on success, or -1 on error (and set errno accordingly).
 */
int nfp_sys_get_nfp_props(struct nfp_sys *handle, int nfp_id, int prop, int *val);

/**
 * Get a complete summary of the system in JSON form
 *
 * @param       handle          system handle
 *
 * @return a pointer to a json_t object on success, or NULL on error.
 */
json_t *nfp_sys_get_json_summary(struct nfp_sys *s);

#endif /* __NFP_SYSTEM_H__ */
