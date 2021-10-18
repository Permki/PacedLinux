/*
 * Copyright (C) 2009-2014, Netronome Systems, Inc.  All rights reserved.
 *
 */

#ifndef PROG_VERSIONS_H
#define PROG_VERSIONS_H

struct nfp_device;

/* keys:
 * "CONFIGURATOR", "ARM_FLASH", "MINILOADER", "PREINIT.SETUP", "BOARD.SETUP",
 * "MEM.SETUP", "PLATFORM.SETUP", "ASSEMBLY.MODEL", "ASSEMBLY.PARTNO",
 * "ASSEMBLY.REVISION", "ASSEMBLY.SERIAL", "ASSEMBLY.VENDOR", "DDR", "CPLD", "VMON"
 */
#define MAX_PROGVER_LEN             128
int nfp_version_lookup(struct nfp_device *nfp, const char *key,
		       char val[MAX_PROGVER_LEN]);

#endif /* PROG_VERSIONS_H */
