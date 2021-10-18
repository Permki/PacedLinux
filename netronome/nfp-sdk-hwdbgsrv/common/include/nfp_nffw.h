/*
 * Copyright (C) 2014-2015,2017 Netronome Systems, Inc.
 * All right reserved.
 *
 */

#ifndef __NFP_NFFW_H__
#define __NFP_NFFW_H__

#include <nfp-common/nfp_platform.h>
#include <nfp.h>
#include <nfp-common/nfp_resid.h>
#include <nfp-common/nfp_regdata.h>
#include <nfp-common/nfp_nffw_rtt.h>

#ifdef __KERNEL__
#define NO_NFP_NFFW_API_EXPORT
#endif

#include "_nfp_nffw_apiex.h"


__BEGIN_DECLS;

/** Init-CSR owner IDs for firmware map to firmware IDs which start at 4.
 * Lower IDs are reserved for target and loader IDs.
 */
#define NFFW_FWID_EXT   3	/* For active MEs that we didn't load. */
#define NFFW_FWID_BASE  4

#define NFFW_FWID_ALL   255

/**
 * Structure describing a run-time NFP symbol.
 *
 * The memory target of the symbol is generally the CPP target number
 * and can be used directly by the nfp_cpp API calls.  However, in
 * some cases (i.e., for local memory or control store) the target is
 * encoded using a negative number.
 *
 * When the target type can not be used to fully describe the location
 * of a symbol the domain field is used to further specify the
 * location (i.e., the specific ME or island number).
 *
 * For ME target resources, 'domain' is an MEID.
 * For Island target resources, 'domain' is an island ID, with the one
 * exception of "sram" symbols for backward compatibility, which are viewed
 * as global.
 *
 */
struct nfp_rtsym {
	const char *name;
	uint64_t addr;
	uint64_t size;
	int type;		/** NFP_RTSYM_TYPE_* in nfp-common/nfp_nffw_rtt.h */
	int target;		/** NFP_RTSYM_TARGET_* in nfp-common/nfp_nffw_rtt.h */
	int domain;
};

/**
 * Load nffw image onto NFP device.
 *
 * @param nfp           NFP device
 * @param data          NFFW binary buffer (fully resolvable)
 * @param size          Byte size of data buffer
 * @param fwid          If non-NULL, will be set to the firmware ID
 *                      assigned to this firmware load.
 *
 * Check if nffw image is ready to be loaded and load it onto the
 * indicated NFP device.  Loading will fail unless all import
 * variables in the nffw image can be resolved.
 *
 * @return 0 on success, or -1 on error (and set errno accordingly).
 */
NFP_NFFW_API
int nfp_nffw_load(struct nfp_device *nfp, const void *data, size_t size, uint8_t *fwid);

/**
 * Load nffw image onto NFP device, modifying data in place.
 *
 * @param nfp           NFP device
 * @param data          NFFW binary buffer (fully resolvable) and may get
 *                      modified.
 * @param size          Byte size of data buffer
 * @param fwid          If non-NULL, will be set to the firmware ID
 *                      assigned to this firmware load.
 *
 * Behaves the same as nfp_nffw_load, but does not duplicate the passed
 * @p data buffer and instead makes modifications to it directly. Modifications
 * are typically to resolve relocations (import variables/expressions).
 *
 * @return 0 on success, or -1 on error (and set errno accordingly).
 */
NFP_NFFW_API
int nfp_nffw_load_mutable(struct nfp_device *nfp, void *data, size_t size, uint8_t *fwid);

/**
 * Start microengines associated with firmware by enabling their contexts.
 *
 * @param nfp           NFP device
 * @param fwid          Firmware ID of firmware's MEs to start. 0 to start all.
 *
 * After c nfp_nffw_load, all memories, chip registers and ME CSRs and data
 * registers are fully initialized and ready, but CtxEnable.CtxEnables = 0.
 * This function "starts" the ME by setting CtxEnable.CtxEnables to the value
 * dictated by the firmware settings (4 or 8 context mode).
 *
 * This gives users a chance to perform custom configuration that needs to
 * operate on loaded data while MEs are still idle.
 *
 * Microengines are started in reverse order - the last loaded ME is started
 * first.
 *
 * @return 0 on success, or -1 on error (and set errno accordingly).
 */
NFP_NFFW_API
int nfp_nffw_start(struct nfp_device *nfp, uint8_t fwid);

/**
 * Unload current nffw image from NFP device.
 *
 * @param nfp           NFP device
 * @param fwid          Firmware ID of firmware to unload. 0 to unload all.
 *
 * Unload specified nffw from NFP device and halt associated MEs. If
 * @p fwid is 0, MEs will be reset if they cannot be stopped. If fwid is
 * non-zero, an ME that cannot be stopped with CtxEnables will be reset only
 * if resetting the ME group or island/cluster does not affect other loaded
 * firmware.
 *
 * @return 0 on success, or -1 on error (and set errno accordingly).
 */
NFP_NFFW_API
int nfp_nffw_unload(struct nfp_device *nfp, uint8_t fwid);

/**
 * Invalidate the internal cache of run-time symbols,
 * reload will occur on the next symbol operation.
 *
 * @param nfp           NFP device
 */
NFP_NFFW_API
void nfp_rtsym_reload(struct nfp_device *nfp);

/**
 * Get number of run-time symbols on NFP device.
 *
 * @param nfp           NFP device
 *
 * @return number of run-time symbols, or -1 on error (and set errno accordingly).
 */
NFP_NFFW_API
int nfp_rtsym_count(struct nfp_device *nfp);

/**
 * Get run-time symbol from NFP device.
 *
 * @param nfp           NFP device
 * @param idx           symbol index
 *
 * The returned symbol structure is handled internally by the library
 * and should not be explicitly freed by the caller.  However, note
 * that any subsequent call to nfp_mip_probe() returning non-zero will
 * implicitly free previously returned symbol structures and render
 * them invalid.
 *
 * @return symbol on success, or NULL on error (and set errno accordingly).
 */
NFP_NFFW_API
const struct nfp_rtsym * nfp_rtsym_get(struct nfp_device *nfp, int idx);

/**
 * Lookup run-time symbol on NFP device.
 *
 * @param nfp           NFP device
 * @param name          symbol name
 *
 * The returned symbol structure is handled internally by the library
 * and should not be explicitly freed by the caller.  However, note
 * that any subsequent call to nfp_mip_probe() returning non-zero will
 * implicitly free previously returned symbol structures and render
 * them invalid.
 *
 * @return symbol on success, or NULL on error (and set errno accordingly).
 */
NFP_NFFW_API
const struct nfp_rtsym * nfp_rtsym_lookup(struct nfp_device *nfp, const char *name);

/**
 * Read symbol contents from NFP device.
 *
 * @param nfp           NFP device
 * @param symbol        symbol structure
 * @param buffer        output buffer
 * @param buf_length    size of output buffer
 * @param offset        offset to read from from start of symbol location
 *
 * Read contents of symbol into @a buffer.
 *
 * @return number of bytes actually read, or -1 on error (and set
 * errno accordingly).
 */
NFP_NFFW_API
ssize_t nfp_rtsym_read(struct nfp_device *nfp, const struct nfp_rtsym *symbol, void *buffer, size_t buf_length, unsigned long long offset);

/**
 * Write symbol contents to NFP device.
 *
 * @param nfp           NFP device
 * @param symbol        symbol structure
 * @param buffer        input buffer
 * @param buf_length    size of input buffer
 * @param offset        offset to write to from start of symbol location
 *
 * Write contents of a buffer into the symbol contents of the NFP
 * device.
 *
 * @return number of bytes actually written, or -1 on error (and set errno accordingly).
 */
NFP_NFFW_API
ssize_t nfp_rtsym_write(struct nfp_device *nfp, const struct nfp_rtsym *symbol, const void *buffer, size_t buf_length, unsigned long long offset);

/**
 * Fill symbol contents to NFP device.
 *
 * @param nfp           NFP device
 * @param symbol        symbol structure
 * @param buffer        input buffer
 * @param buf_length    size of input buffer
 * @param offset        offset to write to from start of symbol location
 * @param fill_len      Fill length
 *
 * Fill contents of @a buffer into the symbol contents of the NFP
 * device.
 *
 * @return number of bytes actually written, or -1 on error (and set errno accordingly).
 */
NFP_NFFW_API
ssize_t nfp_rtsym_fill(struct nfp_device *nfp, const struct nfp_rtsym *symbol, const void *buffer, size_t buf_length, unsigned long long offset, unsigned long long fill_len);

/*
 * The nfp_nffw_info API below caters for multiple firmware loads at the same
 * time (modular firmware loading). Note that this feature is not currently
 * supported everywhere.
 */

/**
 * Acquire the NFFW Info resource to gain exclusive access to it.
 * This is required, but not checked for, when using the nfp_nffw_* API.
 * It is acquired inside @c nfp_nffw_load and @c nfp_nffw_unload, so there is
 * no need to do so when calling the load and unload API.
 *
 * This action loads it from the nfp.nffw resource.
 *
 * @param nfp   NFP Device
 * @return 0 on success, or -1 on error (and set errno accordingly).
 */
NFP_NFFW_API
int nfp_nffw_info_acquire(struct nfp_device *nfp);

/**
 * Release the NFFW Info resource.
 *
 * This action writes the modified nfp_nffw_info data back into the nfp.nffw
 * resource.
 *
 * @param nfp   NFP Device
 * @return 0 on success, or -1 on error (and set errno accordingly).
 */
NFP_NFFW_API
int nfp_nffw_info_release(struct nfp_device *nfp);

/**
 * Check if any firmware is currently loaded.
 *
 * @param nfp   NFP Device
 * @return 1 if firmware is loaded, 0 if not. No error condition.
 */
NFP_NFFW_API
int nfp_nffw_info_fw_loaded(struct nfp_device *nfp);

/**
 * Set the global firmware-loaded flag. Used internally by the loader.
 *
 * @param nfp           NFP Device
 * @param is_loaded     1 if firmware is loaded, 0 if not.
 * @return 0 on success, or -1 on error (and set errno accordingly).
 */
NFP_NFFW_API
int nfp_nffw_info_fw_loaded_set(struct nfp_device *nfp, int is_loaded);

/**
 * Check if the debugger is attached.
 *
 * @param nfp   NFP Device
 * @return 1 if debugger is attached, 0 if not. No error condition.
 */
NFP_NFFW_API
int nfp_nffw_info_fw_debugger_attached(struct nfp_device *nfp);

/**
 * Set the global debugger-attached flag.
 *
 * @param nfp           NFP Device
 * @param is_attached   1 if debugger is attached, 0 if not.
 * @return 0 on success, or -1 on error (and set errno accordingly).
 */
NFP_NFFW_API
int nfp_nffw_info_fw_debugger_attached_set(struct nfp_device *nfp, int is_attached);

/**
 * Check if all loaded firmware is modular, supporting loading of more
 * firmware.
 *
 * @param nfp   NFP Device
 * @return 1 if firmware is modular-capable, 0 if not. No error condition.
 */
NFP_NFFW_API
int nfp_nffw_info_fw_modular(struct nfp_device *nfp);

/**
 * Set the global firmware-modular flag. Used internally by the loader.
 *
 * @param nfp           NFP Device
 * @param is_modular    1 if firmware is modular, 0 if not.
 * @return 0 on success, or -1 on error (and set errno accordingly).
 */
NFP_NFFW_API
int nfp_nffw_info_fw_modular_set(struct nfp_device *nfp, int is_modular);

/**
 * Get the enabled contexts mask for a given ME.
 *
 * @param nfp   NFP Device
 * @param meid  ME ID
 * @return 0 if no firmware is loaded to this ME, non-zero mask otherwise.
 */
NFP_NFFW_API
uint8_t nfp_nffw_info_me_ctxmask(struct nfp_device *nfp, int meid);

/**
 * Set the enabled contexts mask for a given ME.
 *
 * @param nfp       NFP Device
 * @param meid      ME ID
 * @param ctxmask   Context mask
 * @return 0 on success, or -1 on error (and set errno accordingly).
 */
NFP_NFFW_API
int nfp_nffw_info_me_ctxmask_set(struct nfp_device *nfp, int meid, uint8_t ctxmask);

/**
 * Get the firmware ID loaded to a given ME.
 *
 * @param nfp   NFP Device
 * @param meid  ME ID
 * @return 0 if no firmware is loaded to this ME, non-zero FWID otherwise.
 */
NFP_NFFW_API
uint8_t nfp_nffw_info_me_fwid(struct nfp_device *nfp, int meid);

/**
 * Set the firmware ID for a given ME.
 *
 * @param nfp       NFP Device
 * @param meid      ME ID
 * @param fwid      Firmware ID. 0 for none.
 * @return 0 on success, or -1 on error (and set errno accordingly).
 */
NFP_NFFW_API
int nfp_nffw_info_me_fwid_set(struct nfp_device *nfp, int meid, uint8_t fwid);

/**
 * Obtain a new non-zero firmware ID for loading firmware.
 *
 * Only a limited number of firmware loads are supported.
 *
 * @param nfp   NFP Device
 * @return 0 if no new FWID could be allocated, non-zero FWID otherwise.
 */
NFP_NFFW_API
uint8_t nfp_nffw_info_fwid_alloc(struct nfp_device *nfp);

/**
 * Free a firmware ID. Used when unloading firmware.
 *
 * This action will also clear the state of all MEs associated with this
 * @p fwid.
 *
 * @param nfp   NFP Device
 * @param fwid  Non-zero firmware ID to unload specific firmware. 0 for all.
 * @return 0 on success, or -1 on error (and set errno accordingly).
 */
NFP_NFFW_API
int nfp_nffw_info_fwid_free(struct nfp_device *nfp, uint8_t fwid);

/**
 * Get the first firmware ID of loaded firmware. Can be 0 if no firmware is
 * loaded.
 *
 * @param nfp   NFP Device
 * @return Firmware ID, 0 if no firmware loaded.
 */
NFP_NFFW_API
uint8_t nfp_nffw_info_fwid_first(struct nfp_device *nfp);

/**
 * Get the next loaded firmware's firmware ID.
 *
 * @param nfp   NFP Device
 * @param fwid  Current firmware ID from which to start iterating.
 * @return Firmware ID, 0 if no more firmware loaded.
 */
NFP_NFFW_API
uint8_t nfp_nffw_info_fwid_next(struct nfp_device *nfp, uint8_t fwid);

/**
 * Obtain the MIP location for a given firmware ID.
 *
 * @param nfp       NFP Device
 * @param fwid      Firmware ID
 * @param cpp_id    Set to CPP ID of MIP location.
 * @param off       Set to offset of MIP location. If the MIP resides in an
 *                  external memory unit's cache, off<63> == 1. This is not MU
 *                  locality bits, it's only an indicater and the caller
 *                  should clear it.
 * @return 0 on success, or -1 on error (and set errno accordingly).
 */
NFP_NFFW_API
int nfp_nffw_info_fw_mip(struct nfp_device *nfp, uint8_t fwid, uint32_t *cpp_id, uint64_t *off);

/**
 * Set the MIP location for a given firmware ID.
 *
 * @param nfp       NFP Device
 * @param fwid      Firmware ID
 * @param cpp_id    CPP ID of MIP location.
 * @param off       Offset of MIP location. If the MIP resides in an
 *                  external memory unit's cache, caller must set off<63> == 1.
 * @return 0 on success, or -1 on error (and set errno accordingly).
 */
NFP_NFFW_API
int nfp_nffw_info_fw_mip_set(struct nfp_device *nfp, uint8_t fwid, uint32_t cpp_id, uint64_t off);

__END_DECLS;

#endif /* __NFP_NFFW_H__ */
