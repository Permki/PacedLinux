/*
 * Copyright (C) 2017  Netronome Systems, Inc.  All rights reserved.
 *
 * @file          nfp_pif_p4pipeline_config.h
 * @brief         Functions to setup and retrieve P4 pipeline configuration
 *                data. These functions were deduced from the 
 *                SetForwardingPipelineConfig and GetForwardingPipelineConfig
 *                messages in p4runtime.proto from the the P4Lang PI project on 
 *                Github (https://github.com/p4lang/PI)
 *
 */

#ifndef __NFP_PIF_P4PIPELINE_CONFIG_H__
#define __NFP_PIF_P4PIPELINE_CONFIG_H__

#include <stdint.h>
#include <sys/cdefs.h>

#include "_nfp_pif_apiex.h"
#include "nfp_pif_p4info_defs.h"

__BEGIN_DECLS

typedef enum {
    // Unspecified action, will generate error
    PIPELINECONFIG_UNSPEC,
    // Verify, only check item to not save to RTE structures
    PIPELINECONFIG_VERIFY,
    // Verify and save, check item save to RTE structure but do not commit to device
    PIPELINECONFIG_VERIFY_SAVE,
    // Verify and commit, check item, save to RTE structure and commit this and 
    // the rest of the pipeline config to the device
    PIPELINECONFIG_VERIFY_COMMIT,
    // Commit the pipeline config to the device
    PIPELINECONFIG_COMMIT
} pipeline_action_t;

/**
* Add P4 config pipeline item to the system for the device specified with the
* given handle. If ID in the config_item is set to 0 the next available unique 
* ID will assigned to it and returned.  If specified the same ID will be 
* returned.
*
* @param handle         Handle for nfp_pif target as returned by nfp_pif_attach
* @param config_item    Pointer to P4 info config item
*
* @returns              Returns unique ID for item, otherwise return -1 and 
*                       populate errno.
*/
PIF_PUBLIC
int nfp_pif_p4pipeline_config_add(uint64_t handle, 
                                  const p4info_t *config_item);
                                  

/**
* Perform the specified action on the currently stored p4pipeline config 
* that was added with nfp_pif_p4pipeline_config_add
*
* @param action         Specified what action is to be taken on config pipeline
*
* @returns              0 on succesfull action execution, otherwise return -1 
*                       and populate errno.
*/
PIF_PUBLIC 
int nfp_pif_p4pipeline_config_action(pipeline_action_t action);

/**
* Return list of all the current p4config items currently stored in design data
* of the given handle. Caller of the function is responsible for freeing the 
* memeory allocated to return the list of config items, the function 
* nfp_pif_p4pipeline_config_return was created for this purpose.
*
* @param handle         Handle for nfp_pif target as returned by nfp_pif_attach
* @param num_config     Pointer to an interger which is used to return the 
*                       number of p4config items in the returned list.
*
* @returns              Returns pointer to list of p4config items, otherwise 
*                       return NULL and populate errno.
*/
PIF_PUBLIC
p4info_t* nfp_pif_p4pipeline_config_return(uint64_t handle, 
                                           int *num_config);

/**
* Frees all the memory allocated by nfp_pif_p4pipeline_config_return to the 
* specified list of p4config items
*
* @param configs        Pointer to list of p4config items to be freed
* @param num_configs    NUmber of p4config items in the list
*/
PIF_PUBLIC
void nfp_pif_p4pipeline_config_free(p4info_t *configs, int num_configs);

__END_DECLS

#endif /* !__NFP_PIF_P4PIPELINE_CONFIG_H__ */