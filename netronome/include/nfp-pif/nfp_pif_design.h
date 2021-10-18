/*
 * Copyright (C) 2017  Netronome Systems, Inc.  All rights reserved.
 *
 * @file          nfp_pif_design.h
 * @brief         Interface for loading/unloading designs, configs and firmware
 *
 */
#ifndef __NFP_PIF_DESIGN_H__
#define __NFP_PIF_DESIGN_H__

#include <stdint.h>
#include <stdbool.h>
#include <sys/cdefs.h>

#include "_nfp_pif_apiex.h"

__BEGIN_DECLS

/**
* Load  given firmware, design data (P4 design) and optionally user
* configuration data (user rules). Can not load firmware and design data if a
* design is currently loaded. Set configlen to 0 if no user configuration data
* must be loaded with the design.
*
* @param handle     Handle for nfp_pif target as returned by nfp_pif_attach
* @param fwdata     Pointer to firmware data
* @param fwlen      Length of the firmware data
* @param designdata Pointer to design data
* @param designlen  Length of the design data length
* @param configdata Pointer to user configuration data
* @param configlen  Length of the configuration data
*
* @returns  Returns 0 if loaded correctly otherwise return -1 with errno 
*           populated with correct error number
*/
PIF_PUBLIC
int nfp_pif_design_load(uint64_t handle,
                        const char *fwdata, int fwlen, bool skip_fw_load,
                        const char *designdata, int designlen,
                        const char *configdata, int configlen);

/**
* Load given user configuration data (user rules), if append flag is set then
* configuration data will be appended to the existing user data otherwise any
* existing user rules will be overwritten
*
* @param handle     Handle for nfp_pif target as returned by nfp_pif_attach
* @param configdata Pointer to user configuration data
* @param configlen  Length of the configuration data
* @param append     Boolean flag to indicate whether data must be appended
*
* @returns    Returns 0 if loaded correctly otherwise return -1 with errno 
*             populated with correct error number
*/
PIF_PUBLIC
int nfp_pif_design_load_config(uint64_t handle,
                               const char *configdata,
                               int configlen,
                               bool append);

/**
* Unload currently loaded deisgn from host and SmartNIC
* 
* @param handle     Handle for nfp_pif target as returned by nfp_pif_attach
*
* @return   Returns 0 if unload operation was success otherwise return -1 with errno 
*           populated with correct error number
*/
PIF_PUBLIC
int nfp_pif_design_unload(uint64_t handle);

/**
* Load firmware, PIF design and user config data from given files
*
* @param handle         Handle for nfp_pif target as returned by nfp_pif_attach
* @param fwfilename     Filename of the firmware file to be loaded
* @param skip_fw_load   Flag that indicates whether firmware loading should b skipped
* @param dfilename      Filename of the PIF design file to be loaded
* @param cfilename      Filename of the user config data file to be loaded
*
* @returns    Returns 0 if loaded correctly otherwise return -1 with errno 
*             populated with correct error number
*/
PIF_PUBLIC
int nfp_pif_design_load_files(uint64_t handle,
                              const char *fwfilename,
                              bool skip_fw_load,
                              const char *dfilename,
                              const char *cfilename);

/**
* Reload the last used firmware and PIF design file
*
* @param handle     Handle for nfp_pif target as returned by nfp_pif_attach
* @returns    Returns 0 if loaded correctly otherwise return -1 with errno 
*             populated with correct error number
*/
PIF_PUBLIC
int nfp_pif_design_reload(uint64_t handle);

/**
* Load the last successfully loaded configuration set (firmware, design and
* rules configuration)
*
* @param handle     Handle for nfp_pif target as returned by nfp_pif_attach
*
* @returns    Returns 0 if loaded correctly otherwise return -1 with errno 
*             populated with correct error number
*/
PIF_PUBLIC
int nfp_pif_design_load_last_config_set(uint64_t handle);

__END_DECLS

#endif /* __NFP_PIF_DESIGN_H__ */
