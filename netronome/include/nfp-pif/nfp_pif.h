/*
 * Copyright (C) 2017  Netronome Systems, Inc.  All rights reserved.
 *
 * @file          nfp_pif.h
 * @brief         Main interface for NFP PIF library 
 *
 */

#ifndef __NFP_PIF_H__
#define __NFP_PIF_H__

#include <stdint.h>
#include <sys/cdefs.h>

#include "_nfp_pif_apiex.h"
#include "nfp_pif_debugctl.h"
#include "nfp_pif_digests.h"
#include "nfp_pif_p4entries.h"
#include "nfp_pif_p4info_defs.h"
#include "nfp_pif_p4entries.h"
#include "nfp_pif_p4pipeline_config.h"
#include "nfp_pif_tables.h"
#include "nfp_pif_tbl_entries.h"

__BEGIN_DECLS

typedef struct {
    int index;
    int nbi;
    int type;
    char type_str[4 +1];
} nfp_pif_hwinfo_phy;

typedef struct {
    int index;
    int phy_idx;
    int base;
    int lanes;
    int speed;
    char speed_str[10+1];
} nfp_pif_hwinfo_port;

typedef struct {
    /* flag for simulator specific behaviour */
    int is_simulator;
    /* me clock speed */
    double me_speed_mhz;
    /* flag indicates whether firmware is loaded */
    int fw_loaded;
    /* flag inidcates whether digests are declared in the design */
    int digests;
    /* flag indicates whether SMARTNIC is running in debug mode */
    int debug_mode;
    /* UUID for the firmware loaded */
    #define PIF_CFG_DESIGN_UUID_WORDS 4
    uint32_t uuid[PIF_CFG_DESIGN_UUID_WORDS];
    /* Flag to indicate that firmware include UUID */
    int uuid_loaded;
    /* time in seconds when the design was loaded */
    uint32_t load_time;
    /* Design build info */
    char *frontend_build_date;
    char *frontend_source;
    char *frontend_version;
    /* PHY information */
    nfp_pif_hwinfo_phy *phys;
    int phy_cnt;
    /* Port information */
    nfp_pif_hwinfo_port *ports;
    int port_cnt;
    /* VF count */
    int vf_cnt;
    /* Current rule commit version */
    int64_t rule_commit_version;
} nfp_pif_info;

typedef enum {
    PIF_STS_READY,              // Everything is ready for next request
    PIF_STS_UPGRADING,          // Upgrading NSP on SmartNIC
    PIF_STS_LOADING,            // Loading P$ design
    PIF_STS_BF_UPGRADE_NEEDED,  // Bootflash on SmartNIC needs to be ugraded
} pif_status_code;

typedef void (pif_cb_func)(pif_status_code status);

/**
 * Attach client calling the function to the PIF instance for the specified 
 * SmartNIC. 
 *
 * @param devnum        NFP device number
 * @param ctl_script    Path and name of control script to load and unload
 *                      device
 * @param cbfunction    Pointer to callback function
 *
 * @return session handle ID that must used for all future calls for this 
 * smartnic, or 0 on failure (and set errno accordingly).
 */
PIF_PUBLIC
uint64_t nfp_pif_attach(uint32_t devnum, 
                        char * ctl_script, 
                        pif_cb_func *cbfunction);

/**
 * Detach client with specified handle from the PIF instance. 
 *
 * @param handle        Handle previously returned by nfp_pif_attach
 *
 * @return 0 on succesfull detaching otherwise return -1 (and set errno 
 * accordingly)
 */
PIF_PUBLIC
int nfp_pif_detach(uint64_t handle);

/**
 * Service function for device coupled to specified handle. Will write rules to 
 * device if it changed from lat being run, process parser value sets and process
 * rule timeouts. WARNING THIS FUNCTION WILL MOST PROBABLY DEPRECATE AND BE 
 * REMOVED BEFORE THE FIRST OFFICIAL RELEASE.
 *
 * @param handle        Handle previously returned by nfp_pif_attach
 *
 * @return 0 if nothing changed and needed processing, 1 if there was changes to
 * the rules that was loaded on the SmartNic otherwise return -1 (and set errno 
 * accordingly) which means a serious error occured and application might as well
 * exit
 */
PIF_PUBLIC
int nfp_pif_service(uint64_t handle);

/** 
* Retrieve information for the target coupled to the given handle
*
* @param handle Handle for nfp_pif target as returned by nfp_pif_attach

* @returns   Pointer to the info structure that will be created in which the 
             information is to be returned. Caller must free this structure, 
             nfp_pif_free_info is a helper function created to assist with this.
             Returns NULL if error occured with errno populated with correct 
             error number
*
*/
PIF_PUBLIC
nfp_pif_info *nfp_pif_get_info(uint64_t handle);

/** 
* Free the given nfp_pif_info structure and all its subfields
*
* @param info   Pointer to the nfp_pif_info structure that needs to be freed.
*/
PIF_PUBLIC
void nfp_pif_free_info(nfp_pif_info *info);

__END_DECLS

#endif /* !__NFP_PIF_H__ */