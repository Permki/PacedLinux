/*
 * Copyright (C) 2017  Netronome Systems, Inc.  All rights reserved.
 *
 * @file          nfp_pif_debugctl.h
 * @brief         Debug control interface for NFP PIF library
 *
 */

#ifndef NFP_PIF_DEBUGCTL_H_
#define NFP_PIF_DEBUGCTL_H_

#include <stdint.h>
#include <sys/cdefs.h>

#include "_nfp_pif_apiex.h"

__BEGIN_DECLS

/**
* Parse and process the debug ctl command
*
* @param handle          Handle for nfp_pif target as returned by nfp_pif_attach
* @param return_value    Pointer to integer where return value must be stored
*                        return -1 on error or 0 on success
* @param return_data     Pointer to string where return data must be stored set
*                        to NULL if there is no return data, return data can be
*                        constant null terminated string or if data was
*                        allocated the data size will indicated in
*                        ret_data_malloc and null terminated and must then be
*                        freed by the calling function
* @param ret_data_malloc Pointer to int indicating the number bytes malloc to
*                        return data
* @param debug_cmd       String to debug ctl command
* @param input_data      String containing debug input data
* @param input_data_len  Length of input_data

*/
PIF_PUBLIC
void nfp_pif_debugctl_parse_cmd(uint64_t handle,
                                int *return_value,
                                char **return_data,
                                int *ret_data_malloc,
                                const char *debug_cmd,
                                const char *input_data,
                                int input_data_len);

__END_DECLS

#endif /* NFP_PIF_DEBUGCTL_H_ */
