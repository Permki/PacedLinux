/*
 * Copyright (C) 2014, Netronome Systems, Inc.
 * All right reserved.
 *
 */

#ifndef LIBNFP_NFFW_LIBNFFW_H
#define LIBNFP_NFFW_LIBNFFW_H

#include <nfp.h>

#include <nfp-common/nfp_platform.h>

#if DEBUG
#define nfp_nffw_dbg(nfp, format, ...) \
    fprintf(stderr, "NFP%d: "format, nfp_device_number(nfp), ##__VA_ARGS__)
#else
#define nfp_nffw_dbg(nfp, format, ...) \
    do { if (0) snprintf(NULL, 0, format, ##__VA_ARGS__); } while (0)
#endif
#define nfp_nffw_info(nfp, format, ...) \
    fprintf(stderr, "NFP%d: "format, nfp_device_number(nfp), ##__VA_ARGS__)
#define nfp_nffw_warn(nfp, format, ...) \
    fprintf(stderr, "NFP%d: "format, nfp_device_number(nfp), ##__VA_ARGS__)
#define nfp_nffw_err(nfp, format, ...) \
    fprintf(stderr, "NFP%d: "format, nfp_device_number(nfp), ##__VA_ARGS__)

#define NFP_MAX_MICROENGINES    40
#define NFP_MAX_CLUSTERS        5
#define NFP_MES_PER_CLUSTER     8

#ifndef _WIN32
#define NFP_SYSFS_DEVICE_DIR    "/sys/bus/platform/devices/nfp.%d"
#endif

#define CONFIG_NFE_VPCI         1

#endif /* LIBNFP_NFFW_LIBNFFW_H */
