/*
 * Copyright (C) 2012  Netronome Systems, Inc.  All rights reserved.
 *
 * @file        nfp3200/nfp_xpb.h
 * @brief       Helper macros for XPB access
 *
 * Disclaimer: this file is provided without any warranty
 * as part of an early-access program, and the content is
 * bound to change before the final release.
 */

#ifndef __NFP_SDK__NFP3200_NFP_XPB_H__
#define __NFP_SDK__NFP3200_NFP_XPB_H__

#define NFP_XPB_SIZE            0x02000000
#define NFP_XPB_DEST_SIZE       (1 << 14)

#define NFP_XPB_DEST(cl, dev)   ((((cl) & 0x1f) << 20) | (((dev) & 0x3f) << 14))

#define NFP_XPB_DEST_CLUSTER_of(dest)   (((dest) >> 20) & 0x1f)
#define NFP_XPB_DEST_DEVICE_of(dest)    (((dest) >> 14) & 0x3f)
#define NFP_XPB_DEST_ADDR_of(dest)      ((dest) & 0x3ffc)

#define NFP_XPB_MECL(cl)                NFP_XPB_DEST(cl, 1)
#define NFP_XPB_MECL_IM(cl)             NFP_XPB_DEST(cl, 2)
#define NFP_XPB_MECL_ECC(cl)            NFP_XPB_DEST(cl, 3)

#endif /* __NFP_SDK__NFP3200_NFP_XPB_H__ */
