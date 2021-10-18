/*
 * Copyright (C) 2015 Netronome Systems, Inc. All rights reserved.
 * Author: Jason McMullan <jason.mcmullan@netronome.com>
 *
 */

#ifndef NFP_NSP_H
#define NFP_NSP_H

#include <nfp_nsp_log.h>

#define NFP_RESOURCE_NSP        "nfp.sp"
#define NSP_CSR_LENGTH        (sizeof(uint64_t) * 4)

/* Offsets relative to the CSR base */
#define NSP_STATUS            0x00
#define   NSP_STATUS_MAGIC_of(x)      (((x) >> 48) & 0xffff)
#define   NSP_STATUS_MAGIC(x)         (((x) & 0xffffULL) << 48)
#define   NSP_STATUS_MAJOR_of(x)      (((x) >> 44) & 0xf)
#define   NSP_STATUS_MAJOR(x)         (((x) & 0xfULL) << 44)
#define   NSP_STATUS_MINOR_of(x)      (((x) >> 32) & 0xfff)
#define   NSP_STATUS_MINOR(x)         (((x) & 0xfffULL) << 32)
#define   NSP_STATUS_CODE_of(x)       (((x) >> 16) & 0xffff)
#define   NSP_STATUS_CODE(x)          (((x) & 0xffffULL) << 16)
#define   NSP_STATUS_RESULT_of(x)     (((x) >>  8) & 0xff)
#define   NSP_STATUS_RESULT(x)        (((x) & 0xffULL) << 8)
#define   NSP_STATUS_BUSY             BIT_ULL(0)

#define NSP_COMMAND           0x08
#define   NSP_COMMAND_OPTION(x)       ((uint64_t)((x) & 0xffffffff) << 32)
#define   NSP_COMMAND_OPTION_of(x)    (((x) >> 32) & 0xffffffff)
#define   NSP_COMMAND_CODE(x)         ((uint32_t)((x) & 0xffff) << 16)
#define   NSP_COMMAND_CODE_of(x)      (((x) >> 16) & 0xffff)
#define   NSP_COMMAND_START           BIT_ULL(0)

/* CPP address to retrieve the data from */
#define NSP_BUFFER            0x10
#define   NSP_BUFFER_CPP(x)           ((uint64_t)(((x) >> 8) & 0xffffff) << 40)
#define   NSP_BUFFER_CPP_of(x)        ((((x) >> 40) & 0xffffff) << 8)
#define   NSP_BUFFER_ADDRESS(x)       (((x) & ((1ULL << 40) - 1)) << 0)
#define   NSP_BUFFER_ADDRESS_of(x)    (((x) >> 0) & ((1ULL << 40) - 1))

/* CPP address to retrieve the data from */
#define NSP_DEFAULT_BUFFER    0x18

#define NSP_DEFAULT_BUFFER_CONFIG 0x20
#define   NSP_DEF_BUFFER_SIZE_MB(x)    (((x) & ((1ULL << 8) - 1)) << 0)
#define   NSP_DEF_BUFFER_SIZE_MB_of(x) (((x) >> 0) & ((1ULL << 8) - 1))

#ifndef MB
#define MB(x)                         ((x) * 1024 * 1024)
#endif

/* Get the nth entry in an eth table buffer */
#define NSP_ETH_ENTRY(x)        ((x) * 0x20)

/* Port geometry */
#define NSP_ETH_PORT            0x00
#define   NSP_ETH_PORT_FEC_RS(x)        ((uint64_t)((x) & 0x1) << 61)
#define   NSP_ETH_PORT_FEC_RS_of(x)     (((x) >> 61) & 0x1)
#define   NSP_ETH_PORT_FEC_BASER(x)     ((uint64_t)((x) & 0x1) << 60)
#define   NSP_ETH_PORT_FEC_BASER_of(x)  (((x) >> 60) & 0x1)
#define   NSP_ETH_PORT_PHYLABEL(x)      ((uint64_t)((x) & 0x3f) << 54)
#define   NSP_ETH_PORT_PHYLABEL_of(x)   (((x) >> 54) & 0x3f)
#define   NSP_ETH_PORT_LABEL(x)         ((uint64_t)((x) & 0x3f) << 48)
#define   NSP_ETH_PORT_LABEL_of(x)      (((x) >> 48) & 0x3f)
#define   NSP_ETH_PORT_ECHANNELS(x)     ((uint64_t)((x) & 0xf) << 40)
#define   NSP_ETH_PORT_ECHANNELS_of(x)  (((x) >> 40) & 0xf)
#define   NSP_ETH_PORT_ECHANNEL(x)      ((uint64_t)((x) & 0x7f) << 32)
#define   NSP_ETH_PORT_ECHANNEL_of(x)   (((x) >> 32) & 0x7f)
#define   NSP_ETH_PORT_ICHANNELS(x)     (((x) & 0xf) << 24)
#define   NSP_ETH_PORT_ICHANNELS_of(x)  (((x) >> 24) & 0xf)
#define   NSP_ETH_PORT_ICHANNEL(x)      (((x) & 0x7f) << 16)
#define   NSP_ETH_PORT_ICHANNEL_of(x)   (((x) >> 16) & 0x7f)
#define   NSP_ETH_PORT_INDEX(x)         (((x) & 0xff) << 8)
#define   NSP_ETH_PORT_INDEX_of(x)      (((x) >>  8) & 0xff)
#define   NSP_ETH_PORT_LANES(x)         (((x) & 0xf) >> 0)
#define   NSP_ETH_PORT_LANES_of(x)      (((x) >>  0) & 0xf)
#define NSP_ETH_STATE           0x08
#define   NSP_ETH_STATE_FEC(x)          (((x) & 3) << 26)
#define   NSP_ETH_STATE_FEC_of(x)       (((x) >> 26) & 0x3)
#define      NSP_ETH_FEC_AUTO 0
#define      NSP_ETH_FEC_FC   1
#define      NSP_ETH_FEC_RS   2
#define      NSP_ETH_FEC_NONE 3
#define   NSP_ETH_STATE_ANEG(x)         (((x) & 7) << 23)
#define   NSP_ETH_STATE_ANEG_of(x)      (((x) >> 23) & 0x7)
#define      NSP_ETH_ANEG_AUTO   0
#define      NSP_ETH_ANEG_SEARCH 1
#define      NSP_ETH_ANEG_CONS   2
#define      NSP_ETH_ANEG_IEEE   3
#define      NSP_ETH_ANEG_FORCED 4
#define   NSP_ETH_STATE_MCHG(x)         (((x) & 1) << 22)
#define   NSP_ETH_STATE_MCHG_of(x)      (((x) >> 22) & 0x1)
#define   NSP_ETH_STATE_MEDIA(x)        (((x) & 3) << 20)
#define   NSP_ETH_STATE_MEDIA_of(x)     (((x) >> 20) & 0x3)
#define      NSP_ETH_MEDIA_COPPER_PASSIVE 0
#define      NSP_ETH_MEDIA_COPPER_ACTIVE  1
#define      NSP_ETH_MEDIA_OPTICAL        2
#define   NSP_ETH_STATE_INTERFACE(x)    (((x) & 0xff) << 12)
#define   NSP_ETH_STATE_INTERFACE_of(x) (((x) >> 12) & 0xff)
#define      NSP_ETH_INTERFACE_UNKNOWN  0
#define      NSP_ETH_INTERFACE_SFP      1
#define      NSP_ETH_INTERFACE_SFP_PLUS 10
#define      NSP_ETH_INTERFACE_SFP28    28
#define      NSP_ETH_INTERFACE_QSFP     40
#define      NSP_ETH_INTERFACE_RJ45     45
#define      NSP_ETH_INTERFACE_CXP      100
#define      NSP_ETH_INTERFACE_QSFP28   112
#define   NSP_ETH_STATE_RATE(x)         (((x) & 0xf) << 8)
#define   NSP_ETH_STATE_RATE_of(x)      (((x) >> 8) & 0xf)
#define      NSP_ETH_RATE_10M           1
#define      NSP_ETH_RATE_100M          2
#define      NSP_ETH_RATE_1G            3
#define      NSP_ETH_RATE_10G           4
#define      NSP_ETH_RATE_25G           5
#define   NSP_ETH_STATE_BOOTABLE        BIT(6)
#define   NSP_ETH_STATE_ACTIVE          BIT(5)
#define   NSP_ETH_STATE_LINK            BIT(4)
#define   NSP_ETH_STATE_ENABLED_RX      BIT(3)
#define   NSP_ETH_STATE_ENABLED_TX      BIT(2)
#define   NSP_ETH_STATE_ENABLED         BIT(1)
#define   NSP_ETH_STATE_CONFIGURED      BIT(0)

/* MAC address, 0-padded in the upper 16 bits
 */
#define NSP_ETH_MAC             0x10

#define NSP_ETH_CONTROL         0x18
#define   NSP_ETH_CONTROL_SET_FEC       BIT(7)
#define   NSP_ETH_CONTROL_SET_ANEG      BIT(6)
#define   NSP_ETH_CONTROL_SET_LANES     BIT(5)
#define   NSP_ETH_CONTROL_SET_RATE      BIT(4)
#define   NSP_ETH_CONTROL_ENABLE_RX     BIT(3)
#define   NSP_ETH_CONTROL_ENABLE_TX     BIT(2)
#define   NSP_ETH_CONTROL_ENABLE        BIT(1)
#define   NSP_ETH_CONTROL_CONFIGURE     BIT(0)

#define NSP_MDIO_PROXY          0x00
#define   NSP_MDIO_PROXY_PRLEN(x)       (((x) & 0x3fULL) << 54)
#define   NSP_MDIO_PROXY_PRLEN_of(x)    (((x) >> 54) & 0x3f)
#define   NSP_MDIO_PROXY_CL45(x)        (((x) & 0x1ULL) << 53)
#define   NSP_MDIO_PROXY_CL45_of(x)     (((x) >> 53) & 0x1)
#define   NSP_MDIO_PROXY_BUS(x)         (((x) & 0x3ULL) << 51)
#define   NSP_MDIO_PROXY_BUS_of(x)      (((x) >> 51) & 0x3)
/* MDIO bus frequency in multiples of 100KHz */
#define   NSP_MDIO_PROXY_100KHZ(x)      (((x) & 0xffULL) << 43)
#define   NSP_MDIO_PROXY_100KHZ_of(x)   (((x) >> 43) & 0xff)
#define   NSP_MDIO_PROXY_RNW(x)         (((x) & 0x1ULL) << 42)
#define   NSP_MDIO_PROXY_RNW_of(x)      (((x) >> 42) & 0x1)
#define   NSP_MDIO_PROXY_PHY(x)         (((x) & 0x1fULL) << 37)
#define   NSP_MDIO_PROXY_PHY_of(x)      (((x) >> 37) & 0x1f)
#define   NSP_MDIO_PROXY_DEV(x)         (((x) & 0x1fULL) << 32)
#define   NSP_MDIO_PROXY_DEV_of(x)      (((x) >> 32) & 0x1f)
#define   NSP_MDIO_PROXY_ADDR(x)        (((x) & 0xffffULL) << 16)
#define   NSP_MDIO_PROXY_ADDR_of(x)     (((x) >> 16) & 0xffff)
#define   NSP_MDIO_PROXY_DATA(x)        (((x) & 0xffffULL) << 0)
#define   NSP_MDIO_PROXY_DATA_of(x)     (((x) >> 0) & 0xffff)

#ifndef BIT
#define BIT(x)				(1 << (x))
#endif

#ifndef BIT
#define BIT(x)		(1 << (x))
#endif

#define BITFIELD_SET(wd, offs, v)       (((v) & ((1 << wd) - 1)) << offs)
#define BITFIELD_GET(wd, offs, v)       (((v) >> offs) & ((1 << wd) - 1))

#ifndef BITFIELD_SET_ULL
# define BITFIELD_SET_ULL(wd, offs, v)   (((v) & ((1ULL << wd) - 1)) << offs)
#endif
#ifndef BITFIELD_GET_ULL
# define BITFIELD_GET_ULL(wd, offs, v)   (((v) >> offs) & ((1ULL << wd) - 1))
#endif

#define NSP_SENSOR_ID_CHIPTEMP		0
#define NSP_SENSOR_ID_ASMBYPOWER	1
#define NSP_SENSOR_ID_ASMBY12V		2
#define NSP_SENSOR_ID_ASMBY3V3		3
#define NSP_SENSOR_COUNT		4

#define NSP_SENSOR_CHIPTEMP_MAX		95
#define NSP_SENSOR_CHIPTEMP_CRIT	105

#define NSP_SENSOR_ASMBYPOWER_MAX	25

#define NSP_SENSOR_MASK(x)             BITFIELD_SET(32,  0, x)
#define NSP_SENSOR_MASK_of(x)          BITFIELD_GET(32,  0, x)

#define NSP_SENSOR_STATE0               0x00
#define  NSP_SENSOR_CHIPTEMP(x)          BITFIELD_SET_ULL(32,  0, x)
#define  NSP_SENSOR_CHIPTEMP_of(x)       BITFIELD_GET_ULL(32,  0, x)
#define  NSP_SENSOR_ASMBYPOWER(x)        BITFIELD_SET_ULL(32, 32, x)
#define  NSP_SENSOR_ASMBYPOWER_of(x)     BITFIELD_GET_ULL(32, 32, x)

#define NSP_SENSOR_STATE1               0x08
#define  NSP_SENSOR_ASMBY12V(x)          BITFIELD_SET_ULL(32,  0, x)
#define  NSP_SENSOR_ASMBY12V_of(x)       BITFIELD_GET_ULL(32,  0, x)
#define  NSP_SENSOR_ASMBY3V3(x)          BITFIELD_SET_ULL(32, 32, x)
#define  NSP_SENSOR_ASMBY3V3_of(x)       BITFIELD_GET_ULL(32, 32, x)

#define NSP_BSPVER_STR                  0x00
#define NSP_BSPVER_BIN0                 0x28
#define  NSP_BSPVER_FLAGS(x)             BITFIELD_SET_ULL(8,  0, x)
#define  NSP_BSPVER_FLAGS_of(x)          BITFIELD_GET_ULL(8,  0, x)
#define  NSP_BSPVER_BR_PRI(x)            BITFIELD_SET_ULL(8,  8, x)
#define  NSP_BSPVER_BR_PRI_of(x)         BITFIELD_GET_ULL(8,  8, x)
#define  NSP_BSPVER_BR_SEC(x)            BITFIELD_SET_ULL(8, 16, x)
#define  NSP_BSPVER_BR_SEC_of(x)         BITFIELD_GET_ULL(8, 16, x)
#define  NSP_BSPVER_BR_NSP(x)            BITFIELD_SET_ULL(8, 24, x)
#define  NSP_BSPVER_BR_NSP_of(x)         BITFIELD_GET_ULL(8, 24, x)

#define  NSP_BSPVER_PRI(x)               BITFIELD_SET_ULL(16, 32, x)
#define  NSP_BSPVER_PRI_of(x)            BITFIELD_GET_ULL(16, 32, x)
#define  NSP_BSPVER_SEC(x)               BITFIELD_SET_ULL(16, 48, x)
#define  NSP_BSPVER_SEC_of(x)            BITFIELD_GET_ULL(16, 48, x)
#define NSP_BSPVER_BIN1                 0x30
#define  NSP_BSPVER_NSP(x)               BITFIELD_SET_ULL(16,  0, x)
#define  NSP_BSPVER_NSP_of(x)            BITFIELD_GET_ULL(16,  0, x)

#define NSP_SENSORMASK                  0x38
#define  NSP_IDENT_SENSORMASK(x)         BITFIELD_SET_ULL(64,  0, x)
#define  NSP_IDENT_SENSORMASK_of(x)      BITFIELD_GET_ULL(64,  0, x)

#define NSP_MAGIC                   0xab10
#define NSP_MAJOR                   0
#define NSP_MINOR                   25

#define NSP_CODE(code)              ((NSP_MAJOR << 12) | (code & 0xfff))
#define NSP_CODE_MAJOR_of(code)     (((code) >> 12) & 0xf)
#define NSP_CODE_MINOR_of(code)     (((code) >>  0) & 0xfff)

#define NSP_CODE_NOOP             0	/* No operation */
#define NSP_CODE_SOFT_RESET       1	/* Soft reset the NFP */
#define NSP_CODE_FW_DEFAULT       2	/* Load default (UNDI) FW to buffer */
#define NSP_CODE_PHY_INIT         3	/* Initialize the PHY */
#define NSP_CODE_MAC_INIT         4	/* Initialize the MAC */
#define NSP_CODE_PHY_RXADAPT      5	/* Re-run PHY RX Adaptation */
#define NSP_CODE_FW_LOAD          6	/* Load firmware from buffer, len in option */
#define NSP_CODE_ETH_RESCAN       7	/* Rescan ETHs, update ETH_TABLE */
#define NSP_CODE_ETH_CONTROL      8	/* Perform ETH control action */
#define NSP_CODE_MDIO_PROXY       9	/* NSP MDIO proxy */
#define NSP_CODE_RPC              10	/* NSP RPC */
#define NSP_CODE_ARM_BIN_LOAD     11	/* Load bianry to arm */
#define NSP_CODE_SENSORS          12	/* Retrieve sensor state */
#define NSP_CODE_BSP_VERSION      13	/* Retrieve BSP version */
#define NSP_CODE_RTSYM_LOOKUP     14	/* Look up real time symbol*/
#define NSP_CODE_MAC_CONFIG	  15	/* MAC configuration with supplied parameter overrides */
#define NSP_CODE_FW_STORED	  16	/* Explicit load application firmware from flash */
#define NSP_CODE_HWINFO_LOOKUP	  17	/* Lookup hwinfo key including overrides*/

#define NSP_CODE_TERMINATE        0xffff	/* Stop the NSP */

struct nsp_abi_version {
	int major;
	int minor;
};

struct nspd;	/* server process handle */
struct nsp;	/* NSP ABI server side library/API handle(used solely by struct nspd) */

struct nspd *nfp_get_nspd(struct nfp_device *nfp);

struct offload_info {
	int id;
};

void nfp_nspd_set_offload_task(struct nfp_device *nfp,
			       struct offload_info *oi);

struct nfp_nsp_log;
struct nfp_nsp_log *nfp_nspd_get_log(struct nfp_device *nfp);

#define NFP_BINCHK_FILE_MAGIC   0x2d214ea9
#define NFP_BINCHK_FILE_EC      4
#define NFP_BINCHK_TAG_MAGIC    0x9f06b6c0
#define NFP_BINCHK_TAG_EC       7
#define NFP_BINCHK_TYPE_ARM     1
#define NFP_BINCHK_TYPE_FLASH   2
#define NFP_BINCHK_TYPE_ONE     3
#define NFP_BINCHK_TYPE_COMMENT 4
#define NFP_BINCHK_TYPE_CONFIG  5

typedef struct {
	uint32_t magic;
	uint32_t entry_count;
	uint32_t tag_count;
	uint32_t tag_offset;
} nfp_binchk_file;

typedef struct {
	uint32_t magic;
	uint32_t entry_count;
	uint32_t type;
	uint32_t blob_length;
	uint32_t blob_offset;
	uint32_t blob_crc;
	uint32_t tag_offset;
} nfp_binchk_tag;

typedef int (*nfp_binchk_callback) (void *, uint32_t, uint32_t, void *);

struct nsp *nsp_acquire(struct nfp_device *nfp);
void nsp_release(struct nsp *nsp);

int nsp_exec(struct nsp *nsp, uint16_t code, uint32_t option,
	     uint32_t base_cpp, uint64_t base_addr, uint32_t size, int quiet);
int nsp_check(struct nsp *nsp, int specific_op);

int nfp_nsp_command(struct nfp_device *nfp, uint16_t code, uint32_t option,
		    uint32_t buff_cpp, uint64_t buff_addr,
		    struct nsp_abi_version *abiver);

#define NSP_MBC_FLAG_READ       BIT(0)
#define NSP_MBC_FLAG_WRITE      BIT(1)
int nfp_nsp_managed_buffer_command(struct nfp_device *nfp, uint16_t code,
				   uint32_t option, void *buffer, size_t size,
				   uint32_t rw_flags);

int nfp_nsp_noop(struct nfp_device *nfp);

int nfp_nsp_mdio_proxy(struct nfp_device *nfp, int rnw, int preamble, int hz,
		       int bus, int phy, int dev, uint16_t addr, uint16_t *data);

int nfp_nsp_rpc(struct nfp_device *nfp, void *data, int size);

int nfp_binchk_tag_file(FILE *in, FILE *out, uint32_t type);

int nfp_binchk_do(void *buf, nfp_binchk_callback callback, void *context);

int nfp_binchk_check(void *buf);

#endif /* NFP_NSP_H */
