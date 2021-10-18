/**
 * Copyright (C) 2013-2017 Netronome Systems, Inc.  All rights reserved.
 *
 * @file nfp_nbi_mac_eth.h
 *
 * This header file declares the nfp6000 MAC API functions relating to
 * the Ethernet core and the NBI gasket interface.
 *
 * The nfp6000 contains two MACs.  Access to a MAC is provided through
 * a struct nfp_nbi_dev device handle which is returned by the
 * nfp_nbi_open() function (See nfp_nbi.h).
 *
 * Each nfp6000 MAC contains two ethernet cores each capable of
 * supporting up to twelve 10GE ports.  Many of the functions in this
 * header file accept a "core" parameter specifying the Ethernet core
 * and a "port" parameter specifying the port on that core.
 *
 * When PCP is enabled the traffic on a port can be classified into a
 * maximum of eight channels.  The flow control mechanisms are based
 * on the capacity assigned to these channels. Many of the gasket
 * control functions accept a "chan" parameter specifying the channel.
 *
 */

#ifndef __NFP_NBI_MAC_ETH_H__
#define __NFP_NBI_MAC_ETH_H__

#include <stdint.h>
#include <time.h>
#include <nfp6000/nfp_ecc.h>

#include "nfp_nbi_mac_config.h"
#include "nfp_nbi_mac_misc.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Loopbacks (TH-7382) */
/**
 * Disable all Ethernet port loopbacks
 */
#define NFP_NBI_MAC_LOOP_OFF 0x0
/**
 * Enable Tx-Rx loopback at the PCS
 */
#define NFP_NBI_MAC_LOOP_SYSPCS 0x1

/**
 * Enable External Phy Loopbacks
 */
#define NFP_EXTPHY_SYS_SHALLOWPCS 0x02
#define NFP_EXTPHY_SYS_DEEPPCS 0x04
#define NFP_EXTPHY_SYS_DEEPPMA 0x08
#define NFP_EXTPHY_LINE_SHALLOWPCS 0x10
#define NFP_EXTPHY_LINE_DEEPPCS 0x20
#define NFP_EXTPHY_LINE_DEEPPMA 0x40
/**
 * Maximum supported frame length
 */
#define NFP_MAX_ETH_FRAME_LEN 16352

/* TM OOBFC */
/**
 * Enable out-of-band flow control for Traffic Manager queues 0-511
 */
#define NFP_NBI_MAC_Oob511to0en 0x1
/**
 * Enable out-of-band flow control for Traffic Manager queues 512-1023
 */
#define NFP_NBI_MAC_Oob1023to512en 0x10000

/**
 * Mask used for writes to EthCmdConfig register
 */
#define EthCmdConfigMask 0x387f7bfb

/// @cond
/**
 * Ethernet link state indication for register MacMuxCtrl Ilk selected.
 */
#define NFP_NBI_MAC_MacMuxCtrl_Error 0x1

/**
 * Ethernet link state indication for register MacEthGlobal_EthActCtlSeg segment
 * active
 */
#define NFP_NBI_MAC_EthActCtlSeg_Disabled 0x2

/**
 * Ethernet link state indication for register
 * MacEthChPcsSeg_EthChPcsStatus1 receive link down.
 */
#define NFP_NBI_MAC_EthChPcsStatus1_EthPcsRcvLinkStatus_Down 0x4

/**
 * Ethernet link state indication for register EthChPcsBaseRStatus1
 * receive link down.
 */
#define NFP_NBI_MAC_EthChPcsBaseRStatus1_EthPcsRcvLinkStatus_Down 0x8

/**
 * Ethernet link state indication for register EthChPcsBaseRStatus1
 * block not locked.
 */
#define NFP_NBI_MAC_EthChPcsBaseRStatus1_EthPcsBlockLocked_False 0x10

/**
 * Ethernet link state indication for register EthCmdConfig transmit not
 * enabled.
 */
#define NFP_NBI_MAC_EthCmdConfig_EthTxEna_False 0x20

/**
 * Ethernet link state indication for register EthCmdConfig receive not enabled.
 */
#define NFP_NBI_MAC_EthCmdConfig_EthRxEna_False 0x40

/**
 * Interlaken link state indication for register LkRxAlignStatus not aligned.
 */
#define NFP_NBI_MACX_ILK_LkRxAlignStatus_False 0x80

/**
 * Interlaken link state indication for register LkRxStatusMessage remote Rx not
 * aligned.
 */
#define NFP_NBI_MACX_ILK_LkRxStatusMessage_False 0x100

typedef enum {
	nfp_nbi_mac_eth_aneg_link_unknown,
	nfp_nbi_mac_eth_aneg_link_none,
	nfp_nbi_mac_eth_aneg_link_forced,
	nfp_nbi_mac_eth_aneg_link_aneg
} nfp_nbi_mac_eth_aneg_link;

typedef enum {
	nfp_nbi_mac_eth_aneg_type_unknown,
	nfp_nbi_mac_eth_aneg_type_1g_forced,
	nfp_nbi_mac_eth_aneg_type_10g_forced,
	nfp_nbi_mac_eth_aneg_type_25g_forced,
	nfp_nbi_mac_eth_aneg_type_40g_forced,
	nfp_nbi_mac_eth_aneg_type_50g_forced,
	nfp_nbi_mac_eth_aneg_type_100g_forced,
	nfp_nbi_mac_eth_aneg_type_10g_kr,
	nfp_nbi_mac_eth_aneg_type_25g_ieee_cr,
	nfp_nbi_mac_eth_aneg_type_25g_ieee_kr,
	nfp_nbi_mac_eth_aneg_type_25g_consortium_cr,
	nfp_nbi_mac_eth_aneg_type_25g_consortium_kr,
	nfp_nbi_mac_eth_aneg_type_40g_cr,
	nfp_nbi_mac_eth_aneg_type_40g_kr,
	nfp_nbi_mac_eth_aneg_type_50g_cr,
	nfp_nbi_mac_eth_aneg_type_50g_kr,
	nfp_nbi_mac_eth_aneg_type_100g_cr,
	nfp_nbi_mac_eth_aneg_type_100g_kr
} nfp_nbi_mac_eth_aneg_type;

typedef enum {
	nfp_nbi_mac_eth_aneg_fec_unknown,
	nfp_nbi_mac_eth_aneg_fec_none,
	nfp_nbi_mac_eth_aneg_fec_br,
	nfp_nbi_mac_eth_aneg_fec_rs
} nfp_nbi_mac_eth_aneg_fec;

typedef enum {
	nfp_nbi_mac_eth_aneg_pf_unknown,
	nfp_nbi_mac_eth_aneg_pf_none,
	nfp_nbi_mac_eth_aneg_pf_symmetrical
} nfp_nbi_mac_eth_aneg_pf;

typedef struct {
	nfp_nbi_mac_eth_aneg_link link;
	nfp_nbi_mac_eth_aneg_type type;
	nfp_nbi_mac_eth_aneg_fec fec;
	nfp_nbi_mac_eth_aneg_pf pf;
} nfp_nbi_mac_eth_aneg_status_s;

/// @endcond

/**
 * Get Ethernet portcount.
 *
 * This function returns the number of ports per core
 *
 * @param nbi NBI device
 * @return Number of ports per core
 */
NFP_NBI_API
int nfp_nbi_mac_portcount(struct nfp_nbi_dev *nbi);

/**
 * Get Ethernet port up/down state.
 * @ingroup nfp6000-only
 *
 * This function returns the interface's up/down status
 *
 *
 * @param nbi NBI device
 * @param core MAC ethernet core: [0-1]
 * @param port MAC ethernet port: [0-11]
 * @param state Ethernet port state: [0-1(down-up)]
 *
 * @return 0 on success. Set errno and return -1 on error.
 */
NFP_NBI_API
int nfp_nbi_mac_eth_ifstate(struct nfp_nbi_dev *nbi,
			    int core,
			    int port,
			    int *state);

/**
 * Disable an Ethernet port.
 * @ingroup nfp6000-only
 *
 * This function disables Rx & Tx, initiates a PCS reset and
 * deactivates the specified port.
 *
 *
 * @param nbi NBI device
 * @param core MAC ethernet core: [0-1]
 * @param port MAC ethernet port: [0-11]
 *
 * @return 0 on success. Set errno and return -1 on error.
 */
NFP_NBI_API
int nfp_nbi_mac_eth_ifdown(struct nfp_nbi_dev *nbi, int core, int port);

/**
 * Enable an Ethernet port.
 * @ingroup nfp6000-only
 *
 * This function enables Rx & Tx and initiates a PCS reset and
 * activates the specified port.  It assumes that the port speed and
 * all other configuration parameters for the port have been
 * initialized elsewhere.
 *
 *
 * @param nbi NBI device
 * @param core MAC ethernet core: [0-1]
 * @param port MAC ethernet port: [0-11]
 *
 * @return 0 on success. Set errno and return -1 on error.
 */
NFP_NBI_API
int nfp_nbi_mac_eth_ifup(struct nfp_nbi_dev *nbi, int core, int port);

/**
 * Check the link state of an Ethernet port.
 * @ingroup nfp6000-only
 *
 * This function returns 1 if the specified port has link up and block
 * lock.  It returns zero if the link is down.  If linkstate parameter
 * is not NULL this function will use it to return more detail for the
 * link down state.
 *
 *
 * @param nbi NBI device
 * @param core MAC ethernet core: [0-1]
 * @param port MAC ethernet port: [0-11]
 * @param[out] linkstate state detail
 *
 * @return 0: link down; 1: link up; Sets errno and returns -1 on error.
 *
 */
NFP_NBI_API
int nfp_nbi_mac_eth_read_linkstate(struct nfp_nbi_dev *nbi,
				   int core,
				   int port,
				   uint32_t *linkstate);

/**
 * Return the mode for an Ethernet port.
 * @ingroup nfp6000-only
 *
 * This function returns the mode for the specified port.  Modes are
 * enumerated in nfp_nbi_mac_lanemode_t.
 *
 * @param nbi NBI device
 * @param core MAC ethernet core: [0-1]
 * @param port MAC ethernet port: [0-11]
 *
 * @return Return mode on success. Set errno and return -1 on error.
 */
NFP_NBI_API
int nfp_nbi_mac_eth_read_mode(struct nfp_nbi_dev *nbi, int core, int port);

/**
 * Return the number of packets dropped on ingress due to overskid or
 * free buffer underrun.
 * @ingroup nfp6000-only
 *
 * This function reads a counter maintained by the statistics daemon.  It
 * will return -1 if the statistics daemon is not running.
 *
 * @param nbi NBI device
 * @param core MAC ethernet core: [0-1]
 * @param port MAC ethernet port: [0-11]
 *
 * @return Number of packets dropped. Set errno and return -1 on error.
 */
NFP_NBI_API
int nfp_nbi_mac_eth_read_headdrop(struct nfp_nbi_dev *nbi, int core, int port);

/**
 * Write the MAC address for a port
 * @ingroup nfp6000-only
 *
 * @param nbi NBI device
 * @param core MAC ethernet core: [0-1]
 * @param port MAC ethernet port: [0-11]
 * @param hwaddr MAC address (48-bits)
 *
 * @return 0 on success. Set errno and return -1 on error.
 */
NFP_NBI_API
int nfp_nbi_mac_eth_write_mac_addr(struct nfp_nbi_dev *nbi,
				   int core,
				   int port,
				   uint64_t hwaddr);

/**
 * Read the MAC address for a port
 * @ingroup nfp6000-only
 *
 * @param nbi NBI device
 * @param core MAC ethernet core: [0-1]
 * @param port MAC ethernet port: [0-11]
 * @param[out] hwaddr MAC address (48-bits)
 *
 * @return 0 on success. Set errno and return -1 on error.
 */
NFP_NBI_API
int nfp_nbi_mac_eth_read_mac_addr(struct nfp_nbi_dev *nbi,
				  int core,
				  int port,
				  uint64_t *hwaddr);

/**
 * Write DSA mode for ingress packets.
 * @ingroup nfp6000-only
 *
 * The ingress parser has the ability to skip DSA tags within the MAC
 * header.  Zero, four or eight bytes, immediately after the source
 * address, can be used for either extended in standard DSA tags.  The
 * MAC can allow these octets to pass through the ingress without
 * affecting packet parsing.
 *
 * This function sets the number of octets reserved in each packet
 * header for DSA tagging.  If DSA tagging is enabled, it is assumed
 * to be present in every packet. This is a static setting and should
 * not be changed with traffic flowing.
 *
 * @param nbi NBI device
 * @param core MAC ethernet core: [0-1]
 * @param port MAC ethernet port: [0-11]
 * @param octets Number of octets to reserve for DSA use. [0,4,8]
 * @return 0 on success. Set errno and return -1 on error.
 */
NFP_NBI_API
int nfp_nbi_mac_eth_write_ingress_dsa(struct nfp_nbi_dev *nbi,
				      int core,
				      int port,
				      int octets);

/**
 * Read DSA mode for ingress packets.
 * @ingroup nfp6000-only
 *
 * The ingress parser has the ability to skip DSA tags within the MAC
 * header.  Zero, four or eight bytes, immediately after the source
 * address, can be used for either extended in standard DSA tags.  The
 * MAC can allow these octets to pass through the ingress without
 * affecting packet parsing.
 *
 * @param nbi NBI device
 * @param core MAC ethernet core: [0-1]
 * @param port MAC ethernet port: [0-11]
 *
 * @return Number of octets reserved for DSA use. [0,4,8]
 *
 */
NFP_NBI_API
int nfp_nbi_mac_eth_read_ingress_dsa(struct nfp_nbi_dev *nbi,
				     int core,
				     int port);

/**
 * Write DSA mode for egress packets.
 * @ingroup nfp6000-only
 *
 * The egress parser has the ability to skip DSA tags within the MAC
 * header.  Zero, four or eight bytes, immediately after the source
 * address, can be used for either extended in standard DSA tags.  The
 * MAC can allow these octets to pass through the egress without
 * affecting packet parsing.
 *
 * This function sets the number of octets reserved in each packet
 * header for DSA tagging.  If DSA tagging is enabled, it is assumed
 * to be present in every packet. This is a static setting and should
 * not be changed with traffic flowing.
 *
 * @param nbi NBI device
 * @param core MAC ethernet core: [0-1]
 * @param port MAC ethernet port: [0-11]
 * @param octets Number of octets to reserve for DSA use. [0,4,8]
 * @return 0 on success. Set errno and return -1 on error.
 */
NFP_NBI_API
int nfp_nbi_mac_eth_write_egress_dsa(struct nfp_nbi_dev *nbi,
				     int core,
				     int port,
				     int octets);

/**
 * Read DSA mode for egress packets.
 * @ingroup nfp6000-only
 *
 * The egress parser has the ability to skip DSA tags within the MAC
 * header.  Zero, four or eight bytes, immediately after the source
 * address, can be used for either extended in standard DSA tags.  The
 * MAC can allow these octets to pass through the egress without
 * affecting packet parsing.
 *
 * @param nbi NBI device
 * @param core MAC ethernet core: [0-1]
 * @param port MAC ethernet port: [0-11]
 *
 * @return Number of octets reserved for DSA use. [0,4,8].  Set errno
 * and return -1 on error.
 *
 */
NFP_NBI_API
int nfp_nbi_mac_eth_read_egress_dsa(struct nfp_nbi_dev *nbi,
				    int core,
				    int port);

/**
 * Enable/Disable command prepend support on an egress channel.
 * @ingroup nfp6000-only
 *
 * The MAC supports features such as checksum and timestamp
 * insertion. A command prepend must be added to the front of egress
 * packets to enable these features.  The contents of the command
 * prepend specifies which features are enabled.  This function
 * informs the MAC that all packets on the specified channel will
 * include the 4-byte command prepend. (See nfp_nbi_mac_eth_write_egress_crc).
 *
 * @param nbi NBI device
 * @param chan Channel [0-127]
 * @param state 1 - Enable; 0 - Disable
 * @return 0 on success. Set errno and return -1 on error.
 */
NFP_NBI_API
int nfp_nbi_mac_write_egress_prepend_enable(struct nfp_nbi_dev *nbi,
					    int chan,
					    int state);

/**
 * Return setting of command prepend enable on an egress channel.
 * @ingroup nfp6000-only
 *
 * The MAC supports features such as checksum and timestamp
 * insertion. A command prepend must be added to the front of egress
 * packets to enable these features.  The contents of the command
 * prepend specifies which features are enabled.  This function
 * returns one if command prepends are enabled for the specified
 * channel.  Zero indicates command prepends are disabled and -1
 * indicates error.
 *
 * @param nbi NBI device
 * @param chan Channel [0-127]
 * @return State of prepend enable.  Set errno and return -1 on error.
 */
int nfp_nbi_mac_read_egress_prepend_enable(struct nfp_nbi_dev *nbi, int chan);

/**
 * Set the number of octets to skip on egress packets.
 * @ingroup nfp6000-only
 *
 * The MAC egress parser can handle a Switch Support prepend in the
 * header of transmit packets.  This function tells the parser how
 * many octets are reserved for the Switch Support prepend.  The
 * parser will skip this number of octets at the beginning of all
 * egress packets before looking for the MAC header.
 *
 * Skip bytes must be set correctly for channel statistics to be
 * accurate.  Port statistics will not account for prepends.
 *
 * @param nbi NBI device
 * @param core mac ethernet core: [0-1]
 * @param port mac ethernet port: [0-11]
 * @param octets Number of octets to skip [0,2,4,8]
 * @return 0 on success. Set errno and return -1 on error.
 */
NFP_NBI_API
int nfp_nbi_mac_eth_write_egress_skip(struct nfp_nbi_dev *nbi,
				      int core,
				      int port,
				      int octets);

/**
 * Read the setting for skip bytes on egress packets.
 * @ingroup nfp6000-only
 *
 * The MAC egress parser can handle a Switch Support prepend in the
 * header of transmit packets.  This function indicates how
 * many octets are reserved for the Switch Support prepend.  The
 * parser will skip this number of octets at the beginning of all
 * egress packets before looking for the MAC header.
 *
 * Skip bytes must be set correctly for channel statistics to be
 * accurate.  Port statistics will not account for prepends.
 *
 * @param nbi NBI device
 * @param core mac ethernet core: [0-1]
 * @param port mac ethernet port: [0-11]
 * @return skip bytes. Set errno and return -1 on error.
 */
NFP_NBI_API
int nfp_nbi_mac_eth_read_egress_skip(struct nfp_nbi_dev *nbi,
				     int core,
				     int port);

/**
 * Enable/disable timestamp/parse-result prepend on ingress.
 * @ingroup nfp6000-only
 *
 * The MAC can prepend a timestamp and the parse result to the packet
 * header when it passes the packet to the NBI on ingress.  This
 * function enables/disables either or both of these features.
 *
 * @param nbi NBI device
 * @param core mac ethernet core: [0-1]
 * @param port mac ethernet port: [0-11]
 * @param feature disable = 0; parse-result = 1; timestamp = 2; both = 3.
 * @return 0 on success. Set errno and return -1 on error.
 */
NFP_NBI_API
int nfp_nbi_mac_eth_write_ingress_prepend_enable(struct nfp_nbi_dev *nbi,
						 int core,
						 int port,
						 int feature);

/**
 * Return the setting of timestamp/parse-result prepend on ingress.
 * @ingroup nfp6000-only
 *
 * The MAC can prepend a timestamp and the parse result to the packet
 * header when it passes the packet to the NBI on ingress.  This
 * function indicates which of these features are enabled on the
 * specified port.
 *
 * @param nbi NBI device
 * @param core mac ethernet core: [0-1]
 * @param port mac ethernet port: [0-11]
 *
 * @return 0: disabled; 1: parse-result; 2: timestamp; 3: both.  Set
 * errno and return -1 on error.
 *
 */
NFP_NBI_API
int nfp_nbi_mac_eth_read_ingress_prepend_enable(struct nfp_nbi_dev *nbi,
						int core,
						int port);

/**
 * Set the number of octets to skip on ingress packets.
 * @ingroup nfp6000-only
 *
 * The MAC ingress parser can handle a Switch Support prepend in the
 * header of received packets.  This function tells the parser how
 * many octets are reserved for the Switch Support prepend.  The
 * parser will skip this number of octets at the beginning of all
 * ingress packets before looking for the MAC Header.
 *
 * Skip bytes must be set correctly for channel statistics to be
 * accurate.  Port statistics will not account for prepends.
 *
 * @param nbi NBI device
 * @param core mac ethernet core: [0-1]
 * @param port mac ethernet port: [0-11]
 * @param octets Number of octets to skip [0,2,4,8]
 * @return 0 on success. Set errno and return -1 on error.
 */
NFP_NBI_API
int nfp_nbi_mac_eth_write_ingress_skip(struct nfp_nbi_dev *nbi,
				       int core,
				       int port,
				       int octets);

/**
 * Read the setting for skip bytes on ingress packets.
 * @ingroup nfp6000-only
 *
 * The MAC ingress parser can handle a Switch Support prepend in the
 * header of received packets.  This function reads the setting for how
 * many octets are reserved for the Switch Support prepend.  The
 * parser will skip this number of octets at the beginning of all
 * ingress packets before looking for the MAC Header.
 *
 * Skip bytes must be set correctly for channel statistics to be
 * accurate.  Port statistics will not account for prepends.
 *
 * @param nbi NBI device
 * @param core mac ethernet core: [0-1]
 * @param port mac ethernet port: [0-11]
 * @return Skip bytes.  Set errno and return -1 on error.
 */
NFP_NBI_API
int nfp_nbi_mac_eth_read_ingress_skip(struct nfp_nbi_dev *nbi,
				      int core,
				      int port);

/**
 * Enable/disable Ethernet PCS loopback.
 * @ingroup nfp6000-only
 *
 * The MAC Ethernet core supports a system side loopback within the
 * PCS that can be enabled independently on each port.
 *
 * (See nfp_nbi_phy_write_loopback_mode for SerDes lane loopbacks.)
 *
 * @param nbi NBI device
 * @param core MAC ethernet core: [0-1]
 * @param port MAC ethernet port: [0-11]
 * @param mode loopbacks to enable/disable
 * @return 0 on success. Set errno and return -1 on error.
 */
NFP_NBI_API
int nfp_nbi_mac_eth_write_loopback_mode(struct nfp_nbi_dev *nbi,
					int core,
					int port,
					int mode);

/**
 * Read state of Ethernet PCS loopback.
 *
 * The MAC Ethernet core supports a system side loopback within the
 * PCS that can be enabled independently on each port.
 *
 * This function returns the state of PCS loopback for a single port.
 *
 * @param nbi NBI device
 * @param core MAC ethernet core: [0-1]
 * @param port MAC ethernet port: [0-11]
 * @return 0: PCS loopback disabled; 1: PCS loopback enabled. Set errno and
 * return -1 on error.
 */
NFP_NBI_API
int nfp_nbi_mac_eth_read_loopback_mode(struct nfp_nbi_dev *nbi,
				       int core,
				       int port);

/**
 * Enable/disable External Phy loopback.
 * @ingroup nfp6000-only
 *
 * The External Phy supports one shallow PCS, one deep PCS
 * and one dep PMA loopbacks for both system side and line side.
 *
 * @param nbi NBI device
 * @param core external phy core: [0-1]
 * @param port external phy port: [0-3]
 * @param mode external phy loopbacks : [0:7]
 * @return 0 on success. Return -1 on error.
 */
NFP_NBI_API
int nfp_nbi_mac_eth_write_extphy_loopback_mode(struct nfp_nbi_dev *nbi,
					       int core,
					       int port,
					       int mode);

/**
 * Read state of External phy loopback.
 *
 * The External Phy supports one shallow PCS, one deep PCS
 * and one dep PMA loopbacks for both system side and line side.
 *
 * This function returns the state of loopback for a single port.
 *
 * @param nbi NBI device
 * @param core MAC ethernet core: [0-1]
 * @param port MAC ethernet port: [0-11]
 * @return 0: PCS loopback disabled; 1: PCS loopback enabled. Set errno and
 * return -1 on error.
 */
NFP_NBI_API
int nfp_nbi_mac_eth_read_extphy_loopback_mode(struct nfp_nbi_dev *nbi,
					      int core,
					      int port);

/**
 * Set maximum receive frame length (mru) for an ethernet port.
 * @ingroup nfp6000-only
 *
 * The default frame length at reset is 1536 octets.  The maximum
 * allowed is 16352.
 *
 * @param nbi NBI device
 * @param core MAC ethernet core: [0-1]
 * @param port MAC ethernet port: [0-11]
 * @param framelen Frame length in bytes (<= 16352).
 *
 * @return 0 on success. Set errno and return -1 on error.
 */
NFP_NBI_API
int nfp_nbi_mac_eth_write_mru(struct nfp_nbi_dev *nbi,
			      int core,
			      int port,
			      int framelen);

/**
 * Read maximum receive frame length (mru) setting for an ethernet port.
 * @ingroup nfp6000-only
 *
 * Returns the current maximum receive frame length for the specified port
 *
 *
 * @param nbi NBI device
 * @param core MAC ethernet core: [0-1]
 * @param port MAC ethernet port: [0-11]
 *
 * @return Frame length in bytes. Set errno and return -1 on error.
 *
 */
NFP_NBI_API
int nfp_nbi_mac_eth_read_mru(struct nfp_nbi_dev *nbi, int core, int port);

/**
 * Set the pause quantum for a port.
 * @ingroup nfp6000-only
 *
 * When the MAC is in PFC mode. There can be up to eight classes of
 * traffic each assigned it's own pause quantum.  The pause quantum
 * specifies the pause duration specified in any pause frame the MAC
 * sends for the associated class of traffic. This function sets the
 * pause quantum for a single class on a single port.
 *
 * @param nbi NBI device
 * @param core MAC ethernet core: [0-1]
 * @param port MAC ethernet port: [0-11]
 * @param pcpclass packet PCP class [0-7]
 * @param quant pause duration for the PCP class.
 *
 * @return 0 on success. Set errno and return -1 on error.
 */
NFP_NBI_API
int nfp_nbi_mac_eth_write_pause_quant(struct nfp_nbi_dev *nbi,
				      int core,
				      int port,
				      int pcpclass,
				      int quant);

/**
 * Read pause quant.
 * @ingroup nfp6000-only
 *
 * When the MAC is in PFC mode. There can be up to eight classes of
 * traffic each assigned it's own pause quantum.  The pause quantum
 * specifies the pause duration specified in any pause frame the MAC
 * sends for the associated class of traffic.  This function returns
 * the pause quantum setting for the specified port.
 *
 * @param nbi NBI device
 * @param core MAC ethernet core: [0-1]
 * @param port MAC ethernet port: [0-11]
 * @param pcpclass packet PCP class [0-7]
 *
 * @return Pause quantum for specified port. Set errno and return -1 on error.
 *
 */
NFP_NBI_API
int nfp_nbi_mac_eth_read_pause_quant(struct nfp_nbi_dev *nbi,
				     int core,
				     int port,
				     int pcpclass);

/**
 * Set the pause threshold for a port.
 * @ingroup nfp6000-only
 *
 * When the MAC is in PFC mode. There can be up to eight classes of
 * traffic each assigned it's own pause quantum.  The pause quantum is
 * the pause duration specified in any pause frames sent by the MAC
 * for the associated class of traffic. When the MAC sends a pause
 * frame it simultaneously initializes a timer with the pause quantum
 * value.  The timer counts down from this value and if it reaches the
 * pause threshold value the MAC sends another pause frame and
 * restarts the countdown timer.  This function sets the pause
 * threshold for a single class on a single port.
 *
 * @param nbi NBI device
 * @param core MAC ethernet core: [0-1]
 * @param port MAC ethernet port: [0-11]
 * @param pcpclass packet PCP class [0-7]
 * @param thresh value of quantum timer that triggers resending a pause
 *        frame for the PCP class.
 *
 * @return 0 on success. Set errno and return -1 on error.
 */
NFP_NBI_API
int nfp_nbi_mac_eth_write_pause_thresh(struct nfp_nbi_dev *nbi,
				       int core,
				       int port,
				       int pcpclass,
				       int thresh);

/**
 * Read pause threshold.
 * @ingroup nfp6000-only
 *
 * When the MAC is in PFC mode. There can be up to eight classes of
 * traffic each assigned it's own pause quantum.  The pause quantum is
 * the pause duration specified in any pause frames sent by the MAC
 * for the associated class of traffic. When the MAC sends a pause
 * frame it simultaneously initializes a timer with the pause quantum
 * value.  The timer counts down from this value and if it reaches the
 * pause threshold value the MAC sends another pause frame and
 * restarts the countdown timer.  This function reads the pause
 * threshold for the specified class and port.
 *
 * @param nbi NBI device
 * @param core MAC ethernet core: [0-1]
 * @param port MAC ethernet port: [0-11]
 * @param pcpclass packet PCP class [0-7]
 *
 * @return Pause threshold for specified class and port.  Set errno
 * and return -1 on error.
 *
 */
NFP_NBI_API
int nfp_nbi_mac_eth_read_pause_thresh(struct nfp_nbi_dev *nbi,
				      int core,
				      int port,
				      int pcpclass);

/**
 * Read pause watermark for a channel.
 * @ingroup nfp6000-only
 *
 * When the pause watermark for a channel is exceeded the MAC sends a
 * pause frame for any PCP classes associated with that channel.
 *
 * This function returns the pause watermark for the specified channel.
 *
 * @param nbi NBI device handle
 * @param chan channel [0-127]
 *
 * @return Pausewater mark for specified port.  Set errno and return
 * -1 on error.
 *
 */
NFP_NBI_API
int nfp_nbi_mac_read_chan_pausewm(struct nfp_nbi_dev *nbi, int chan);

/**
 * Write pause watermark for a channel.
 * @ingroup nfp6000-only
 *
 * When the pause watermark for a channel is exceeded the MAC sends a
 * pause frame for any PCP classes associated with that channel.
 *
 * This function sets the pause watermark for the specified channel.
 *
 * @param nbi NBI device handle
 * @param chan channel [0-127]
 * @param pwm pause watermark
 *
 * @return 0 on success. Set errno and return -1 on error.
 */
NFP_NBI_API
int nfp_nbi_mac_write_chan_pausewm(struct nfp_nbi_dev *nbi, int chan, int pwm);

/**
 * Read pause status for a port.
 * @ingroup nfp6000-only
 *
 * @param nbi NBI device
 * @param core MAC ethernet core: [0-1]
 * @param port MAC ethernet port: [0-11]
 *
 * @return 0: not paused; 1: paused
 *
 */
NFP_NBI_API
int nfp_nbi_mac_eth_read_pause_status(struct nfp_nbi_dev *nbi,
				      int core,
				      int port);

/**
 * Read the channel remap assignments for an ethernet port.
 * @ingroup nfp6000-only
 *
 * Ethernet packets on a port can be classified based on the PCP bits
 * in the headers.  Each packet class can be assigned to a channel in
 * the NBI interface by setting a base channel for the port (see
 * nfp_nbi_mac_eth_write_portchan) and specifying eight offsets, one for
 * each class, allowing up to one channel per class.  See
 * nfp_nbi_mac_write_pcpremap.
 *
 * This function copies the channel remap assignment settings into cmap.
 *
 * @param nbi NBI device
 * @param core MAC ethernet core: [0-1]
 * @param port MAC ethernet port: [0-11]
 * @param[out] cmap Struct containing array of 8 offsets from the
 *             base channel and absolute channel for untagged traffic
 *
 * @return 0 on success. Set errno and return -1 on error.
 */
NFP_NBI_API
int nfp_nbi_mac_eth_read_pcpremap(struct nfp_nbi_dev *nbi,
				  int core,
				  int port,
				  struct nfp_nbi_mac_chanremap *cmap);

/**
 * Write the channel remap assignments for an ethernet port.
 * @ingroup nfp6000-only
 *
 * Ethernet packets on a port can be classified based on the PCP bits
 * in the headers.  Each packet class can be assigned to a channel in
 * the NBI interface by setting a base channel for the port (see
 * nfp_nbi_mac_eth_write_portchan) and specifying eight offsets, one for
 * each class, allowing up to one channel per class.
 *
 * Multiple classes may be mapped to a single channel by specifying
 * the same offset in the channel remap assignment for the port.
 * Packets that have no PCP tag are assigned to an "untagged" channel
 * which is specified as an absolute channel rather than an offset.
 *
 * @param nbi NBI device
 * @param core MAC ethernet core: [0-1]
 * @param port MAC ethernet port: [0-11]
 * @param[out] cmap Struct containing array of 8 offsets from the
 *             base channel and absolute channel for untagged traffic
 *
 * @return 0 on success. Set errno and return -1 on error.
 */
NFP_NBI_API
int nfp_nbi_mac_eth_write_pcpremap(struct nfp_nbi_dev *nbi,
				   int core,
				   int port,
				   struct nfp_nbi_mac_chanremap *cmap);

/**
 * Write the port-channel assignments.
 * @ingroup nfp6000-only
 *
 * Each Ethernet port is assigned a set of channels to use when
 * passing packets to the NBI interface (ingress) and receiving
 * packets from the NBI interface (egress).  Up to eight ingress
 * channels and eight egress channels may be assigned to an Ethernet
 * port.  This function specifies the "base" channel and the number of
 * channels for all twenty four ports on the specified core.
 *
 * @param nbi NBI device handle
 * @param core MAC ethernet core: [0-1]
 * @param ig ingress assignments
 * @param eg egress assignments
 *
 * @return 0 on success. Set errno and return -1 on error.
 */
NFP_NBI_API
int nfp_nbi_mac_eth_write_portchan(struct nfp_nbi_dev *nbi,
				   int core,
				   struct nfp_nbi_mac_portchan *ig,
				   struct nfp_nbi_mac_portchan *eg);

/**
 * Read the port-channel assignments.
 * @ingroup nfp6000-only
 *
 * Each Ethernet port is assigned a set of channels to use when
 * passing packets to the NBI interface (ingress) and receiving
 * packets from the NBI interface (egress).  Up to eight ingress
 * channels and eight egress channels may be assigned to an Ethernet
 * port.  This function reads the "base" channel and the number of
 * channels for all ports on the specified core.
 *
 * Note: The caller is responsible for allocating and freeing
 * memory for the two output parameters, ig and eg.
 *
 *
 * @param nbi NBI device handle
 * @param core MAC ethernet core: [0-1]
 * @param[out] ig returned ingress assignments
 * @param[out] eg returned egress assignments
 *
 * @return 0 on success. Set errno and return -1 on error.
 */
NFP_NBI_API
int nfp_nbi_mac_eth_read_portchan(struct nfp_nbi_dev *nbi,
				  int core,
				  struct nfp_nbi_mac_portchan *ig,
				  struct nfp_nbi_mac_portchan *eg);

struct timespec;
/**
 * Initialize the MAC timestamp.
 * @ingroup nfp6000-only
 *
 * This sets the MacTimeStampSetSec and the MacTimeStampSetNsec
 * registers and triggers a load to the MacTimeStampSec and
 * MacTimeStampNsec registers.  This is a very coarse way setting the
 * timestamp due to the latencies in the API library.
 *
 * @param nbi NBI device
 * @param ts_sec  seconds part of timestamp.
 * @param ts_nsec  nanoseconds part of timestamp.
 *
 * @return 0 on success. Set errno and return -1 on error.
 */
NFP_NBI_API
int nfp_nbi_mac_write_timestamp(struct nfp_nbi_dev *nbi,
				uint64_t ts_sec,
				uint64_t ts_nsec);

/**
 * Read the MAC timestamp.
 * @ingroup nfp6000-only
 *
 * This function reads the MacTimeStampSec and MacTimeStampNsec
 * registers.  This is a very coarse way reading the timestamp due to
 * the latencies in the API library.
 *
 * @param nbi NBI device
 * @param[out] ts_sec pointer to seconds part of timestamp.
 * @param[out] ts_nsec pointer to nanoseconds part of timestamp.
 *
 * @return 0 on success. Set errno and return -1 on error.
 */
NFP_NBI_API
int nfp_nbi_mac_read_timestamp(struct nfp_nbi_dev *nbi,
			       uint64_t *ts_sec,
			       uint64_t *ts_nsec);

/**
 * Enable/Disable MAC clocks.
 * @ingroup nfp6000-only
 *
 * This function controls the clock enables for various components
 * within the MAC.  This function is only used during MAC
 * initialization.
 *
 * @param nbi NBI device
 * @param mask specifies the clocks to be enabled/disabled
 * @param state specifies the state. 1 = enabled, 0 = off
 *
 * @return 0 on success. Set errno and return -1 on error.
 */
NFP_NBI_API
int nfp_nbi_mac_write_clock_enables(struct nfp_nbi_dev *nbi,
				    uint32_t mask,
				    uint32_t state);

/**
 * Set/Unset the hydra reset bits
 * @ingroup nfp6000-only
 *
 * This function controls the resets for various components
 * within the MAC.  This function is only used during MAC
 * initialization.
 *
 * @param nbi NBI device
 * @param core MAC ethernet core: [0-1]
 * @param mask specifies the resets to be set/unset
 *
 * @return 0 on success. Set errno and return -1 on error.
 */
NFP_NBI_API
int nfp_nbi_mac_eth_write_resets(struct nfp_nbi_dev *nbi,
				 int core,
				 uint32_t mask);

/**
 * Set/Unset the hydra reset bits holding them for 1ms
 * @ingroup nfp6000-only
 *
 * This function controls the resets for various components
 * within the MAC.  This function is only used during MAC
 * initialization.
 *
 * @param nbi NBI device
 * @param core MAC ethernet core: [0-1]
 *
 * @return 0 on success. Set errno and return -1 on error.
 */
NFP_NBI_API
int nfp_nbi_mac_eth_reset_all(struct nfp_nbi_dev *nbi, int core);

/**
 * Enable/Disable CRC insertion on egress ports.
 * @ingroup nfp6000-only
 *
 * The MAC can automatically calculate and insert the frame checksum
 * or it can allow the application to insert it.  Automatic CRC
 * insertion is required whenever packet modifications are performed
 * by the MAC (See nfp_nbi_mac_write_egress_prepend_enable).
 *
 * @param nbi NBI device
 * @param core Ethernet core [0,1]
 * @param mask 12-bit mask specifying which ports have MAC CRC
 * insertion enabled
 *
 * @return 0 on success. Set errno and return -1 on error.
 */
NFP_NBI_API
int nfp_nbi_mac_eth_write_egress_crc(struct nfp_nbi_dev *nbi,
				     int core,
				     uint32_t mask);

/**
 * Read CRC insertion setting on egress ports.
 * @ingroup nfp6000-only
 *
 * The MAC can automatically calculate and insert the frame checksum
 * or it can allow the application to insert it.  This function
 * returns the setting for all ports on the specified core.
 *
 * @param nbi NBI device
 * @param core Ethernet core [0,1]
 *
 * @return 12-bit mask specifying which ports have MAC CRC insertion
 * enabled.  Set errno and return -1 on error.
 */
NFP_NBI_API
int nfp_nbi_mac_eth_read_egress_crc(struct nfp_nbi_dev *nbi, int core);

/**
 * Enable/Disable out-of-band flow control for Traffic Manager queues.
 * @ingroup nfp6000-only
 *
 * The MAC can receive flow control signals from an external hardware
 * source to control 1024 traffic manager queues.  This out of band
 * flow control may be enabled/disabled independently for two sets of
 * TM queues, 0-511 and 512-1023.  The mask should be created by ORing
 * together the required defines from nfp_nbi_mac_eth.h. See register
 * MacOobFcTmCntl in the databook.
 *
 *
 *
 * @param nbi NBI device
 * @param mask Mask specifying queues and control bits.
 *
 * @return 0 on success. Set errno and return -1 on error.
 */
NFP_NBI_API
int nfp_nbi_mac_write_tmoobfc_enables(struct nfp_nbi_dev *nbi, uint32_t mask);

/**
 * Return settings of out-of-band flow control for Traffic Manager queues.
 * @ingroup nfp6000-only
 *
 * @param nbi NBI device
 *
 * @return MacOobFcTmCntl register - specifies queues and control
 * bits.  Set errno and return -1 on error.
 */
NFP_NBI_API
int nfp_nbi_mac_read_tmoobfc_enables(struct nfp_nbi_dev *nbi);

/**
 * Write the VLAN match registers.
 * @ingroup nfp6000-only
 *
 * The MAC parser can recognize multiple VLAN levels within the packet
 * header.  Up to eight different VLAN Tag Protocol Identifiers can be
 * specified.  The first four of these, slots 0 to 3, are set by
 * mac_init to 0x88a8, 0x9100, 0x9200, 0x9300.
 *
 * @param nbi NBI device
 * @param slot specifies one of 8 available matching registers
 * @param vlanid the VLAN TPID.
 *
 * @return 0 on success. Set errno and return -1 on error.
 */
NFP_NBI_API
int nfp_nbi_mac_eth_write_vlantpid(struct nfp_nbi_dev *nbi,
				   int slot,
				   int vlanid);

/**
 * Calculate the TDM slots for hyd/ilk cores and ingress dequeue.
 * @ingroup nfp6000-only
 *
 * Each Ethernet/Interlaken core uses a 12-slot TDM wheel to pass
 * packet data to the NBI gasket interface.  This function calculates
 * the slot settings based on the settings for each port for the
 * specified core in the configuration structure. This function is
 * only used during MAC initialization.
 *
 * @param nbi NBI device
 * @param mcfg MAC configuration struct
 * @param core Core
 *
 * @return 0 on success. Set errno and return -1 on error.
 */
NFP_NBI_API
int nfp_nbi_mac_write_tdm_cycles(struct nfp_nbi_dev *nbi,
				 struct nfp_nbi_mac_config *mcfg,
				 int core);

/**
 * Ingress dequeue DWRR memory credit write.
 * @ingroup nfp6000-only
 *
 * Ingress dequeue DWRR memory credits are set in units of bytes,
 * based on port speeds. This function is only used during MAC
 * initialization.
 *
 * @param nbi NBI device
 * @param core MAC ethernet core: [0-1]
 * @param port MAC ethernet port: [0-11]
 * @param en enable
 * @param weight port credit weight value
 *
 * @return 0 on success. Set errno and return -1 on error.
 */
NFP_NBI_API
int nfp_nbi_mac_eth_write_ingress_dqdwrr(struct nfp_nbi_dev *nbi,
					 int core,
					 int port,
					 int en,
					 int weight);

/**
 * Ingress dequeue DWRR memory credit read
 * @ingroup nfp6000-only
 *
 * Ingress dequeue DWRR memory credits are set in units of bytes,
 * based on port speeds.
 *
 * @param nbi NBI device
 * @param core MAC ethernet core: [0-1]
 * @param port MAC ethernet port: [0-11]
 *
 * @return DWRR weight for specified port. Set errno and return -1 on
 * error.
 */
NFP_NBI_API
int nfp_nbi_mac_eth_read_ingress_dqdwrr(struct nfp_nbi_dev *nbi,
					int core,
					int port);

/**
 * Set the highwater mark & drop delta for a port.
 * @ingroup nfp6000-only
 *
 * Port level flow control is based on a per-port highwater mark and
 * delta (skid).  Exceeding the highwater mark will cause pause frames
 * to be transmitted.  If the pause frames do not have the desired
 * effect and the traffic rate causes the highwater mark plus delta to
 * be exceeded packets will be dropped.
 *
 * @param nbi NBI device
 * @param core MAC ethernet core: [0-1]
 * @param port MAC ethernet port: [0-11]
 * @param hwm Highwater mark
 * @param delta Delta or skid above highwater mark.
 *
 * @return 0 on success. Set errno and return -1 on error.
 */
NFP_NBI_API
int nfp_nbi_mac_eth_write_port_hwm(struct nfp_nbi_dev *nbi,
				   int core,
				   int port,
				   int hwm,
				   int delta);

/**
 * Read highwater mark & drop delta for a port.
 * @ingroup nfp6000-only
 *
 * @param nbi NBI device
 * @param core MAC ethernet core: [0-1]
 * @param port MAC ethernet port: [0-11]
 *
 * @return highwater mark. Set errno and return -1 on error.
 *
 */
NFP_NBI_API
int nfp_nbi_mac_eth_read_port_hwm(struct nfp_nbi_dev *nbi, int core, int port);

#define ARB_CODE_ETH_CMD_CFG_NOOP 0
#define ARB_CODE_ETH_CMD_CFG_RECACHE 1
#define ARB_CODE_ETH_CMD_CFG_ENABLE_RX 2
#define ARB_CODE_ETH_CMD_CFG_ENABLE_TX 3
#define ARB_CODE_ETH_CMD_CFG_DISABLE_RX 4
#define ARB_CODE_ETH_CMD_CFG_DISABLE_TX 5
#define ARB_CODE_ETH_CMD_CFG_ENABLE_FLUSH 6
#define ARB_CODE_ETH_CMD_CFG_DISABLE_FLUSH 7

/***
 *
 */
NFP_NBI_API
int nfp_nbi_mac_ethcmdconfig_arbiter_is_enabled(struct nfp_device *nfp);

/***
 *
 */
NFP_NBI_API
int nfp_nbi_mac_ethcmdconfig_arbiter_enable(struct nfp_device *nfp);

/***
 *
 */
NFP_NBI_API
int nfp_nbi_mac_ethcmdconfig_arbiter_disable(struct nfp_device *nfp);

/**
 * Synchronous set of EthCmdConfig register via ME arbiter
 * @ingroup nfp6000-only
 *
 * @param nbi  NBI device
 * @param port MAC ethernet port: [0-11]
 * @param core MAC core: [0-1]
 * @param cmd  Arbiter bit control command
 *
 * @return 0 on success. Set errno and return -1 on error.
 */
NFP_NBI_API
int nfp_nbi_mac_ethcmdconfig_sync(struct nfp_nbi_dev *nbi,
				  int port,
				  int core,
				  int cmd);

/**
 * Read the Ethernet parameters from EthCmdConfig register.
 * @ingroup nfp6000-only
 *
 * Returns the current value of the EthCmdConfig register for the
 * specified port. See nfp_nbi_mac_eth_write_cmdconfig.
 *
 *
 * @param nbi NBI device
 * @param core MAC ethernet core: [0-1]
 * @param port MAC ethernet port: [0-11]
 *
 * @return value of the EthCmdConfig. Set errno and return -1 on error.
 *
 */
NFP_NBI_API
int nfp_nbi_mac_ethcmdconfig_read(struct nfp_nbi_dev *nbi, int core, int port);

/**
 * Read the base lane and number of lanes assigned to an Ethernet port.
 * @ingroup nfp6000-only
 *
 * Returns the current value of the EthCmdConfig register for the
 * specified port. See nfp_nbi_mac_eth_write_cmdconfig.
 *
 *
 * @param nbi NBI device
 * @param core MAC ethernet core: [0-1]
 * @param port MAC ethernet port: [0-11]
 * @param[out] lbase Base lane
 * @param[out] lnum Number of lanes
 *
 * @return 0 on success. Set errno and return -1 on error.
 *
 */
NFP_NBI_API
int nfp_nbi_mac_eth_read_portlanes(struct nfp_nbi_dev *nbi,
				   int core,
				   int port,
				   int *lbase,
				   int *lnum);

/**
 * Read the status of the auto-negotiation on an Ethernet port.
 * @ingroup nfp6000-only
 *
 * This function reads the status of the auto-negotiation. If there
 * isn't currently link on the port, the status of the
 * auto-negotiation is undefined.
 *
 *
 * @param nbi NBI device
 * @param core MAC ethernet core: [0-1]
 * @param port MAC ethernet port: [0-11]
 * @param status returned
 *
 * @return 0 on success. Set errno and return -1 on error.
 */
NFP_NBI_API
int nfp_nbi_mac_eth_aneg_status(struct nfp_nbi_dev *nbi,
				int core,
				int port,
				nfp_nbi_mac_eth_aneg_status_s *status);

#ifdef __cplusplus
}
#endif

#endif
