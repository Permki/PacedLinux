/*
 * Copyright (C) 2013, Netronome Systems, Inc.  All rights reserved.
 *
 * Disclaimer: this file is provided without any warranty
 * as part of an early-access program, and the content is
 * bound to change before the final release.
 */

#ifndef NFP3800_NFP3800_H
#define NFP3800_NFP3800_H

/* CPP Target IDs */
#define NFP3800_CPP_TARGET_INVALID          0
#define NFP3800_CPP_TARGET_NBI              1
#define NFP3800_CPP_TARGET_QDR              2
#define NFP3800_CPP_TARGET_ILA              6
#define NFP3800_CPP_TARGET_MU               7
#define NFP3800_CPP_TARGET_PCIE             9
#define NFP3800_CPP_TARGET_ARM              10
#define NFP3800_CPP_TARGET_CRYPTO           12
#define NFP3800_CPP_TARGET_ISLAND_XPB       14	/* Shared with CAP */
#define NFP3800_CPP_TARGET_ISLAND_CAP       14	/* Shared with XPB */
#define NFP3800_CPP_TARGET_CT_XPB           14
#define NFP3800_CPP_TARGET_LOCAL_SCRATCH    15
#define NFP3800_CPP_TARGET_CLS              NFP3800_CPP_TARGET_LOCAL_SCRATCH

#endif /* NFP3800_NFP3800_H */
