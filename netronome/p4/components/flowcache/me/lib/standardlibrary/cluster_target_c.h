/*
 * Copyright (C) 2014-2016,  Netronome Systems, Inc.  All rights reserved.
 *
 * @file          cluster_target_c.h
 * @brief         Interface for cluster target functionality.
 */


#ifndef _NFP__CLUSTER_TARGET_C_H_
#define _NFP__CLUSTER_TARGET_C_H_

#include <nfp.h>

#if defined(__NFP_LANG_MICROC)


/**
 * Wait for signal.
 * @param dst_island    Island number of the target device
 * @param dst_me        ME numnber to signal, see DB table 3-4
 * @param dst_ctx       Thread/context number to signal
 * @param sig_no        Signal number
 *
 */
__intrinsic void __signal_me(unsigned int dst_island, unsigned int dst_me, unsigned int dst_ctx, unsigned int sig_no);



#endif /* __NFP_LANG_MICROC */

#endif /* !_NFP__CLUSTER_TARGET_C_H_ */
