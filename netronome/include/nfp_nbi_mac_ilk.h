/**
 * Copyright (C) 2013-2017 Netronome Systems, Inc.  All rights reserved.
 *
 * @file nfp_nbi_mac_ilk.h
 * nfp6000 MAC API functions
 *
 */

#ifndef __NBI_MAC_ILK_H__
#define __NBI_MAC_ILK_H__

#include <stdint.h>
#include <nfp6000/nfp_ecc.h>

#include "nfp_nbi_mac_config.h"
#include "nfp_nbi_mac_misc.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Port channel assignment structure for two Interlaken ports.
 * Specifies the range of channels associated with each port.
 */
struct nfp_nbi_mac_ilk_portchan {
	struct nfp_nbi_mac_portchan pc[2];
};

/**
 * Interlaken BurstMax codes
 */
typedef enum {
	NFP_NBI_MAC_ILK_BURSTMAX64 = 0x0,
	NFP_NBI_MAC_ILK_BURSTMAX128 = 0x1,
	NFP_NBI_MAC_ILK_BURSTMAX192 = 0x2,
	NFP_NBI_MAC_ILK_BURSTMAX256 = 0x3,
	NFP_NBI_MAC_ILK_BURSTMAX320 = 0x4,
	NFP_NBI_MAC_ILK_BURSTMAX384 = 0x5,
	NFP_NBI_MAC_ILK_BURSTMAX448 = 0x6,
	NFP_NBI_MAC_ILK_BURSTMAX512 = 0x7,
} nfp_nbi_mac_ilk_burstmax_t;

/**
 * Interlaken BurstMin codes
 */
typedef enum {
	NFP_NBI_MAC_ILK_BURSTMIN32 = 0x0,
	NFP_NBI_MAC_ILK_BURSTMIN64 = 0x1,
	NFP_NBI_MAC_ILK_BURSTMIN96 = 0x2,
	NFP_NBI_MAC_ILK_BURSTMIN128 = 0x3,
	NFP_NBI_MAC_ILK_BURSTMIN160 = 0x4,
	NFP_NBI_MAC_ILK_BURSTMIN192 = 0x5,
	NFP_NBI_MAC_ILK_BURSTMIN224 = 0x6,
	NFP_NBI_MAC_ILK_BURSTMIN256 = 0x7,
} nfp_nbi_mac_ilk_burstmin_t;

/**
 * Interlaken BurstShort codes
 */
typedef enum {
	NFP_NBI_MAC_ILK_BURSTSHORT8 = 0x7,
	NFP_NBI_MAC_ILK_BURSTSHORT16 = 0x2,
	NFP_NBI_MAC_ILK_BURSTSHORT24 = 0x4,
	NFP_NBI_MAC_ILK_BURSTSHORT32 = 0x0,
	NFP_NBI_MAC_ILK_BURSTSHORT64 = 0x1,
} nfp_nbi_mac_ilk_burstshort_t;

/**
 * Structure encapsulating Interlaken burst configuration
 */
struct nfp_nbi_mac_ilk_burst_limits {
	nfp_nbi_mac_ilk_burstmax_t burstmax;
	nfp_nbi_mac_ilk_burstmin_t burstmin;
	nfp_nbi_mac_ilk_burstshort_t burstshort;
	int burstmax_disable;
	int burstmin_disable;
};

/**
 * Interlaken flow control calendar
 */
struct nfp_nbi_mac_ilk_cal {
	int cal[64];
};

/**
 * Disable all Interlaken loopbacks
 */
#define NFP_NBI_MAC_ILK_LOOP_OFF 0
/**
 * Enable system-side loopback
 */
#define NFP_NBI_MAC_ILK_LOOPSYS 1
/**
 * Enable line-side loopback
 */
#define NFP_NBI_MAC_ILK_LOOPLINE 2

/**
 * Disable an Interlaken port.
 * @ingroup nfp6000-only
 *
 * This function disables Rx & Tx and deactivates the specified port.
 *
 *
 * @param nbi NBI device
 * @param core Interlaken core: [0-1]
 *
 * @return 0 on success. Set errno and return -1 on error.
 */
NFP_NBI_API
int nfp_nbi_mac_ilk_ifdown(struct nfp_nbi_dev *nbi, int core);

/**
 * Enable an Interlaken port.
 * @ingroup nfp6000-only
 *
 * This function enables Rx & Tx and activates the specified port.  It
 * assumes that the configuration parameters for the port have been
 * initialized elsewhere.
 *
 *
 * @param nbi NBI device
 * @param core Interlaken core: [0-1]
 *
 * @return 0 on success. Set errno and return -1 on error.
 */
NFP_NBI_API
int nfp_nbi_mac_ilk_ifup(struct nfp_nbi_dev *nbi, int core);

/**
 * Set/Unset the Interlaken core reset bits
 * @ingroup nfp6000-only
 *
 * @param nbi NBI device
 * @param core Interlaken core: [0-1]
 * @param mask specifies the resets to be set/unset
 * @return 0 on success. Set errno and return -1 on error.
 */
NFP_NBI_API
int nfp_nbi_mac_ilk_write_resets(struct nfp_nbi_dev *nbi,
				 int core,
				 uint32_t mask);

/**
 * Check the link state of an Interlaken port.
 * @ingroup nfp6000-only
 *
 * This function returns 1 if the specified port has link aligned.
 * It returns zero if the link is down. If linkstate parameter
 * is not NULL this function will use it to return more detail for the
 * link down state.
 *
 * @param nbi NBI device
 * @param core Interlaken core: [0-1]
 * @param[out] linkstate state detail
 *
 * @return 1 link up.
 * @return 0 link down.
 * @return 0 on success. Set errno and return -1 on error.
 */
NFP_NBI_API
int nfp_nbi_mac_ilk_read_linkstate(struct nfp_nbi_dev *nbi,
				   int core,
				   uint32_t *linkstate);

/**
 * Return the number of packets dropped on ingress due to overskid or
 * free buffer underrun.  This counter is maintained by the MAC
 * statistics daemon.
 * @ingroup nfp6000-only
 *
 *
 * @param nbi NBI device
 * @param core Interlaken core: [0-1]
 *
 * @return 0 on success. Set errno and return -1 on error.
 */
NFP_NBI_API
int nfp_nbi_mac_ilk_read_headdrop(struct nfp_nbi_dev *nbi, int core);

/**
 * Write Interlaken lane disable register. (Init only)
 * @ingroup nfp6000-only
 *
 * This function disables the lanes specified by mask.  There are
 * twelve lanes corresponding to bit positions 0 - 11.  A 1 bit
 * disables a lane; a zero bit enables a lane.
 *
 * Note: This function will return an error if the Interlaken is not
 * in reset.  I.e. it can only be used during initialization.
 *
 * @param nbi NBI device
 * @param core Interlaken core: [0-1]
 * @param mask Lane disable mask
 * @return 0 on success. Set errno and return -1 on error.
 */
NFP_NBI_API
int nfp_nbi_mac_ilk_write_lane_disable(struct nfp_nbi_dev *nbi,
				       int core,
				       uint32_t mask);

/**
 * Write Interlaken lane disable register.
 * @ingroup nfp6000-only
 *
 * This function returns a mask indicating the disabled Interlaken
 * lanes.  There are twelve lanes corresponding to bit positions 0 -
 * 11.  A 1 bit indicates a disabled lane; a zero bit indicates and
 * enabled lane.
 *
 *
 * @param nbi NBI device
 * @param core Interlaken core: [0-1]
 *
 * @return disabled lane mask
 * @return 0 on success. Set errno and return -1 on error.
 */
NFP_NBI_API
int nfp_nbi_mac_ilk_read_lane_disable(struct nfp_nbi_dev *nbi, int core);

/**
 * Write the port-channel assignments for both Interlaken ports.
 * @ingroup nfp6000-only
 *
 * Each Interlaken port is assigned a set of channels to use when
 * passing packets to the NBI interface (ingress) and receiving
 * packets from the NBI interface (egress).  Up to 128 ingress
 * channels and 128 egress channels may be assigned to Interlaken
 * port 0 if core 1 is unused.  If both cores are used the limit is
 * 64 channels per port.  This function specifies the "base" channel
 * and the number of channels for both Interlaken ports on the
 * specified mac.
 *
 *
 * @param nbi NBI device
 * @param ig ingress assignments
 * @param eg egress assignments
 *
 * @return
 */
NFP_NBI_API
int nfp_nbi_mac_ilk_write_portchan(struct nfp_nbi_dev *nbi,
				   struct nfp_nbi_mac_ilk_portchan *ig,
				   struct nfp_nbi_mac_ilk_portchan *eg);

/**
 * Read the port-channel assignments for both Interlaken ports.
 * @ingroup nfp6000-only
 *
 * Each Interlaken port is assigned a set of channels to use when
 * passing packets to the NBI interface (ingress) and receiving
 * packets from the NBI interface (egress).  Up to 128 ingress
 * channels and 128 egress channels may be assigned to Interlaken
 * port 0 if core 1 is unused.  If both cores are used the limit is
 * 64 channels per port.  This function reads the "base"
 * channel and the number of channels for both Interlaken ports on
 * the specified mac.
 *
 *
 * @param nbi NBI device
 * @param[out] ig ingress assignments
 * @param[out] eg egress assignments
 *
 * @return
 */
NFP_NBI_API
int nfp_nbi_mac_ilk_read_portchan(struct nfp_nbi_dev *nbi,
				  struct nfp_nbi_mac_ilk_portchan *ig,
				  struct nfp_nbi_mac_ilk_portchan *eg);

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
 * @param core Interlaken core: [0-1]
 * @param octets Number of octets to skip [0,4,8]
 * @return
 */
NFP_NBI_API
int nfp_nbi_mac_ilk_write_egress_skip(struct nfp_nbi_dev *nbi,
				      int core,
				      int octets);

/**
 * Enable/disable timestamp/parse-result prepend on ingress.
 * @ingroup nfp6000-only
 *
 * The MAC can prepend a timestamp and the parse result to the packet
 * header when it passes the packet to the NBI on ingress.  This
 * function enables/disables either or both of these features.
 *
 * @param nbi NBI device
 * @param core Interlaken core: [0-1]
 * @param feature disable = 0; parse-result = 1; timestamp = 2; both = 3.
 * @return
 */
NFP_NBI_API
int nfp_nbi_mac_ilk_write_ingress_prepend_enable(struct nfp_nbi_dev *nbi,
						 int core,
						 int feature);

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
 * @param core Interlaken core: [0-1]
 * @param octets Number of octets to skip [0,4,8]
 * @return
 */
NFP_NBI_API
int nfp_nbi_mac_ilk_write_ingress_skip(struct nfp_nbi_dev *nbi,
				       int core,
				       int octets);

/**
 * Write DSA mode for ingress packets.
 * @ingroup nfp6000-only
 *
 * The ingress parser has the ability to skip DSA tags within the MAC
 * header.  Either 0, 4 or 8 bytes, immediately after the source
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
 * @param core Interlaken core: [0-1]
 * @param octets Number of octets to reserve for DSA use. [4,8,0 => disabled]
 * @return
 */
NFP_NBI_API
int nfp_nbi_mac_ilk_write_ingress_dsa(struct nfp_nbi_dev *nbi,
				      int core,
				      int octets);

/**
 * Read DSA ingress mode.
 * @ingroup nfp6000-only
 *
 * @param nbi NBI device
 * @param core Interlaken core: [0-1]
 * @return >= 0: Number of octets reserved for DSA use. [4,8,0 => disabled]
 * @return 0 on success. Set errno and return -1 on error.
 */
NFP_NBI_API
int nfp_nbi_mac_ilk_read_ingress_dsa(struct nfp_nbi_dev *nbi, int core);

/**
 * Write DSA mode for egress packets.
 * @ingroup nfp6000-only
 *
 * The egress parser has the ability to skip DSA tags within the MAC
 * header. Either 0, 4 or 8 bytes, immediately after the source
 * address, can be used for either extended in standard DSA tags. The
 * MAC can allow these octets to pass through the egress without
 * affecting packet parsing.
 *
 * This function sets the number of octets reserved in each packet
 * header for DSA tagging.  If DSA tagging is enabled, it is assumed
 * to be present in every packet. This is a static setting and should
 * not be changed with traffic flowing.
 *
 * @param nbi NBI device
 * @param core Interlaken core: [0-1]
 * @param octets Number of octets to reserve for DSA use. [4,8, 0 => disabled]
 * @return
 */
NFP_NBI_API
int nfp_nbi_mac_ilk_write_egress_dsa(struct nfp_nbi_dev *nbi,
				     int core,
				     int octets);

/**
 * Read DSA mode.
 * @ingroup nfp6000-only
 *
 * @param nbi NBI device
 * @param core Interlaken core: [0-1]
 * @return >= 0: Number of octets reserved for DSA use. [4,8,0 => disabled]
 * @return 0 on success. Set errno and return -1 on error.
 */
NFP_NBI_API
int nfp_nbi_mac_ilk_read_egress_dsa(struct nfp_nbi_dev *nbi, int core);

/**
 * Set the Max, Min and Short burst limits. (Init only)
 * @ingroup nfp6000-only
 *
 * @param nbi NBI device
 * @param core Interlaken core: [0-1]
 * @param burst_limits Max, Min, Short burst limits and enables.
 *
 * @return 0 on success. Set errno and return -1 on error.
 */
NFP_NBI_API
int nfp_nbi_mac_ilk_write_burst_limits(
	struct nfp_nbi_dev nbi,
	int core,
	struct nfp_nbi_mac_ilk_burst_limits burst_limits);

/**
 * Set the Interlaken metaframe period. (Init only)
 * @ingroup nfp6000-only
 *
 * One unit corresponds to 67 bits on the serial lane (64 bits data
 * plus 3 bits framing).  Setting the periiod to zero in the TX
 * disables the generation of alignment frames. This value must be set
 * the same in both the RX and TX. The minimum value is 64. Default is
 * 2048.
 *
 *
 * @param nbi NBI device
 * @param core Interlaken core: [0-1]
 * @param period Metaframe period
 *
 * @return 0 on success. Set errno and return -1 on error.
 */
NFP_NBI_API
int nfp_nbi_mac_ilk_write_metaframe(struct nfp_nbi_dev nbi,
				    int core,
				    int period);

/**
 * Write Interlaken programmable flow control calendar
 * @ingroup nfp6000-only
 *
 * @param nbi NBI device
 * @param core Interlaken core: [0-1]
 * @param cal User calendar
 * @return 0 on success. Set errno and return -1 on error.
 */
NFP_NBI_API
int nfp_nbi_mac_ilk_write_calendar(struct nfp_nbi_dev nbi,
				   int core,
				   struct nfp_nbi_mac_ilk_cal *cal);

/**
 * Enable/Disable Interlaken Out-of-band flow control
 * @ingroup nfp6000-only
 *
 * Interlaken flow control can be configured to be in-band or
 * out-of-band.  The default is in-band.  The selection may be
 * different for ingress and egress.  This function enables or
 * disables ingress and or egress out-of-band flow control for the
 * specified core.
 *
 * @param nbi NBI device
 * @param core Interlaken core: [0-1]
 * @param enables enable mask for ingress and egress
 *
 * @return 0 on success. Set errno and return -1 on error.
 */
NFP_NBI_API
int nfp_nbi_mac_ilk_oobfc_ena(struct nfp_nbi_dev nbi,
			      int core,
			      uint32_t enables);

/**
 * Return the code corresponding to a given burst max size.
 * @ingroup nfp6000-only
 *
 * Encoded_burst_max:{64:0, 128:1, 192:2, 256:3, 320:4, 384:5, 448:6, 512:7}
 *
 * @param burst_max Maximum burst size
 * @return Code corresponding to the specified burst max size.
 */
NFP_NBI_API
int nfp_nbi_mac_ilk_encode_burst_max(int burst_max);

/**
 * Return the code corresponding to a given burst min size.
 * @ingroup nfp6000-only
 *
 * Encoded_burst_min:{32:0,  64:1,  96:2, 128:3, 160:4, 192:5, 224:6, 256:7}
 *
 * @param burst_min Minimum burst size
 * @return Code corresponding to the specified burst min size.
 *
 */
NFP_NBI_API
int nfp_nbi_mac_ilk_encode_burst_min(int burst_min);

/**
 * Return the code corresponding to a given burst short size.
 * @ingroup nfp6000-only
 *
 * Encoded_burst_short:{8: 7, 16: 2, 24: 4, 32: 0, 64: 1}
 *
 * @param burst_short Short burst size
 * @return Code corresponding to the specified burst short size.
 * Set errno and return -1 on error.
 *
 */
NFP_NBI_API
int nfp_nbi_mac_ilk_encode_burst_short(int burst_short);

/**
 * Return the burst max corresponding to a given code.
 * @ingroup nfp6000-only
 *
 * Encoded_burst_max:{64:0, 128:1, 192:2, 256:3, 320:4, 384:5, 448:6, 512:7}
 *
 * @param ebm Burst max code
 * @return Burst max corresponding to the specified code.
 */
NFP_NBI_API
int nfp_nbi_mac_ilk_decode_burst_max(int ebm);

/**
 * Return the burst min corresponding to a given code.
 * @ingroup nfp6000-only
 *
 * Encoded_burst_min:{32:0,  64:1,  96:2, 128:3, 160:4, 192:5, 224:6, 256:7}
 *
 * @param ebm burst min code
 * @return burst min corresponding to the specified code.
 *
 */
NFP_NBI_API
int nfp_nbi_mac_ilk_decode_burst_min(int ebm);

/**
 * Return the burst short corresponding to a given code.
 * @ingroup nfp6000-only
 *
 * Encoded_burst_short:{8: 7, 16: 2, 24: 4, 32: 0, 64: 1}
 *
 * @param ebs burst short code
 * @return burst short corresponding to the specified code.
 *
 */
NFP_NBI_API
int nfp_nbi_mac_ilk_decode_burst_short(int ebs);

/**
 * Interlaken ingress dequeue DWRR memory credit write
 * @ingroup nfp6000-only
 *
 * Ingress dequeue DWRR memory credits are set in units of bytes,
 * based on port speeds.
 *
 * @param nbi NBI device
 * @param core Interlaken core: [0-1]
 * @param en enable
 * @param weight port credit weight value
 *
 * @return DWRR weight for specified core
 */
NFP_NBI_API
int nfp_nbi_mac_ilk_write_ingress_dqdwrr(struct nfp_nbi_dev *nbi,
					 int core,
					 int en,
					 int weight);

/**
 * Interlaken ingress dequeue DWRR memory credit read
 * @ingroup nfp6000-only
 *
 * Ingress dequeue DWRR memory credits are set in units of bytes,
 * based on port speeds.
 *
 * @param nbi NBI device
 * @param core Interlaken core: [0-1]
 *
 * @return DWRR weight for specified core
 */
NFP_NBI_API
int nfp_nbi_mac_ilk_read_ingress_dqdwrr(struct nfp_nbi_dev *nbi, int core);

/**
 * Set the highwater mark & drop delta for an Interlaken core.
 * @ingroup nfp6000-only
 *
 * Port level flow control is based on a per-port highwater mark and
 * delta (skid).  Exceeding the highwater mark will cause pause frames
 * to be transmitted.  If the pause frames do not have the desired
 * effect and the traffic rate causes the highwater mark plus delta to
 * be exceeded packets will be dropped.
 *
 * @param nbi NBI device
 * @param core Interlaken core: [0-1]
 * @param hwm Highwater mark
 * @param delta Delta or skid above highwater mark.
 *
 * @return 0 on success. Set errno and return -1 on error.
 */
NFP_NBI_API
int nfp_nbi_mac_ilk_write_port_hwm(struct nfp_nbi_dev *nbi,
				   int core,
				   int hwm,
				   int delta);

/**
 * Enable/disable Interlaken loopbacks.
 * @ingroup nfp6000-only
 *
 * Each Interlaken core supports a system side loopback and a line side
 * loopback.
 *
 * These loopbacks may be enabled/disabled in various combinations for
 * testing.  These loopbacks are defined in nfp_nbi_mac_ilk.h and can be
 * ORed together to enable multiple loopbacks.
 *
 * @see NFP_NBI_MAC_ILK_LOOP_OFF   Disable all Interlaken loopbacks
 * @see NFP_NBI_MAC_ILK_LOOPSYS    Enable system-side loopback
 * @see NFP_NBI_MAC_ILK_LOOPLINE   Enable line-side loopback
 *
 * @param nbi NBI device
 * @param core Interlaken core: [0-1]
 * @param mode loopbacks to enable/disable
 * @return 0 on success. Set errno and return -1 on error.
 */
NFP_NBI_API
int nfp_nbi_mac_ilk_write_loopback_mode(struct nfp_nbi_dev *nbi,
					int core,
					uint32_t mode);

/**
 * Read state of Interlaken loopbacks.
 * @ingroup nfp6000-only
 *
 * Each Interlaken core supports a system side loopback and a line side
 * loopback.
 *
 * These loopbacks may be enabled/disabled in various combinations for
 * testing.  These loopbacks are defined in nfp_nbi_mac_ilk.h.
 *
 * This function returns the state of Interlaken loopbacks.
 *
 * @see NFP_NBI_MAC_ILK_LOOP_OFF   Disable all Interlaken loopbacks
 * @see NFP_NBI_MAC_ILK_LOOPSYS    Enable system-side loopback
 * @see NFP_NBI_MAC_ILK_LOOPLINE   Enable line-side loopback
 *
 * @param nbi NBI device
 * @param core Interlaken core: [0-1]
 * @return mode on success. Set errno and return -1 on error.
 */
NFP_NBI_API
int nfp_nbi_mac_ilk_read_loopback_mode(struct nfp_nbi_dev *nbi, int core);

#ifdef __cplusplus
}
#endif

#endif
