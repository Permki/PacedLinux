/*
 * Copyright (C) 2014, Netronome Systems, Inc.  All rights reserved.
 */

#ifndef __DBGSRV_ENV__
#define __DBGSRV_ENV__

/* set to 1 to fail if RPC version check fails,
 * set to 0 to warn and inform client about check failure,
 * set to -1 to warn and not inform client about check failure
 */
#define RPC_DO_VERSION_CHECK "NFP_RPC_VERSION_CHECK"

#endif /* __DBGSRV_ENV__ */
