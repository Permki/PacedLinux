/**
 * Copyright (C) 2013-2014 Netronome Systems, Inc.  All rights reserved.
 *
 * @file nfp_nbi_phymod.h
 *
 * This header file declares the API functions relating to
 * the PHY modules (SFP, SFP+, QSFP, CXP) used with Netronome systems.
 *
 * This API is designed to operate with devices that conform to the
 * following specifications:

 * SFF-8472 Diagnostic Monitoring Interface for Optical Transceivers (SFP+)

 * SFF-8436 QSFP+ 10 Gbs 4X PLUGGABLE TRANSCEIVER

 * Supplement to InfiniBandTM Architecture Specification Volume 2 Release 1.2.1
 * Annex A6:120 Gb/s 12x Small Form-factor Pluggable (CXP)

 * Many parts of the diagnosic monitoring information described in these
 * specifications are optional.  This API uses a subset of the
 * information that may or may not be available on a specific device.
 * This API has been tested on the following devices:

 *
 * **** ADD REFERENCE TO DATABOOK OR OTHER SOURCE OF VERIFIED DEVICES LIST ****

 */

#ifndef __NFP_PHYMOD_H__
#define __NFP_PHYMOD_H__

#include <stdint.h>
#include <nfp-common/nfp_platform.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * No module present
 */
#define NFP_PHYMOD_TYPE_NONE 0x00

/**
 * SFP  module
 */
#define NFP_PHYMOD_TYPE_SFP  1

/**
 * SFP+  module
 */
#define NFP_PHYMOD_TYPE_SFPP 10

/**
 * SFP28  module
 */
#define NFP_PHYMOD_TYPE_SFP28 25

/**
 * QSFP  module
 */
#define NFP_PHYMOD_TYPE_QSFP 40

/**
 * QSFP28  module
 */
#define NFP_PHYMOD_TYPE_QSFP28 112

/**
 * CXP  module
 */
#define NFP_PHYMOD_TYPE_CXP  100

/**
 * Fixed Copper RJ45
 */
#define NFP_PHYMOD_TYPE_RJ45  45

#define NFP_PHYMOD_CONNECTOR_UNKNOWN        0x00
#define NFP_PHYMOD_CONNECTOR_SC             0x01
#define NFP_PHYMOD_CONNECTOR_FC_STYLE2_CU   0x02
#define NFP_PHYMOD_CONNECTOR_FC_STYLE1_CU   0x03
#define NFP_PHYMOD_CONNECTOR_BNCTNC         0x04
#define NFP_PHYMOD_CONNECTOR_FIBRECHANCOAX  0x05
#define NFP_PHYMOD_CONNECTOR_FIBERJACK      0x06
#define NFP_PHYMOD_CONNECTOR_LC             0x07
#define NFP_PHYMOD_CONNECTOR_MTRJ           0x08
#define NFP_PHYMOD_CONNECTOR_MU             0x09
#define NFP_PHYMOD_CONNECTOR_SG             0x0A
#define NFP_PHYMOD_CONNECTOR_OPTICAL_PIGTAIL 0x0B
#define NFP_PHYMOD_CONNECTOR_MPO1x12        0x0C
#define NFP_PHYMOD_CONNECTOR_MPO2x16        0x0D
#define NFP_PHYMOD_CONNECTOR_HSSDCII        0x20
#define NFP_PHYMOD_CONNECTOR_CU_PIGTAIL     0x21
#define NFP_PHYMOD_CONNECTOR_RJ45           0x22
#define NFP_PHYMOD_CONNECTOR_NO_SEP         0x23
#define NFP_PHYMOD_CONNECTOR_MXC2x16        0x24
#define NFP_PHYMOD_CONNECTOR_START_VEND     0x80

#define NFP_PHYMOD_EXT_SPEC_CODE_25G_SR     0x02
#define NFP_PHYMOD_EXT_SPEC_CODE_25G_LR     0x03
#define NFP_PHYMOD_EXT_SPEC_CODE_25G_ER     0x04
#define NFP_PHYMOD_EXT_SPEC_CODE_25G_CR_CAL 0x0b
#define NFP_PHYMOD_EXT_SPEC_CODE_25G_CR_CAS 0x0c
#define NFP_PHYMOD_EXT_SPEC_CODE_25G_CR_CAN 0x0d
#define NFP_PHYMOD_EXT_SPEC_CODE_AOC        0x18

/**
 * PHY module summary status
 */
#define NFP_PHYMOD_SUMSTAT_LOS 0x00000001

/**
 * PHY module summary status
 */
#define NFP_PHYMOD_SUMSTAT_FAULT 0x00000002

/**
 * PHY module summary status
 */
#define NFP_PHYMOD_SUMSTAT_OPTPWR 0x00000004

/**
 * PHY module summary status
 */
#define NFP_PHYMOD_SUMSTAT_OPTBIAS 0x00000008

/**
 * PHY module summary status
 */
#define NFP_PHYMOD_SUMSTAT_HILOVOLT 0x00000010

/**
 * PHY module summary status
 */
#define NFP_PHYMOD_SUMSTAT_HILOTEMP 0x00000020

struct nfp_phymod;
struct nfp_phymod_eth;

/**
 * PHY Module enumeration
 * @ingroup nfp6000-only
 *
 * This function allows enumeration of the PHY Modules
 * attached to the system.
 *
 * @param nfp   NFP device
 * @param ptr   Abstract pointer, must be NULL to get the first port
 * @return  On succes: phymod
 * @return  On error: NULL
 */
struct nfp_phymod *nfp_phymod_next(struct nfp_device *nfp, void **ptr);

/**
 * PHY Module internal re-enumeration
 * @ingroup nfp6000-only
 *
 * This function updates internal cached state used for PHY and ETH enumeration
 * This needs called after setting media overrides.
 *
 * @param nfp   NFP device
 * @return 0 on success, -1 and errno on error
 */
int nfp_reinitialize_phy_eth_list(struct nfp_device *nfp);

/**
 * Get the index for a phymod
 * @ingroup nfp6000-only
 *
 * @param phymod
 * @param index	Pointer to a int for the index
 * @return 0 on success, -1 and errno on error
 */
int nfp_phymod_get_index(struct nfp_phymod *phymod, int *index);

/**
 * Get the string (UTF8) label for a phymod
 * @ingroup nfp6000-only
 *
 * @param phymod
 * @param label		Pointer to a const char * for the label
 * @return 0 on success, -1 and errno on error
 */
int nfp_phymod_get_label(struct nfp_phymod *phymod, const char **label);

/**
 * Get the NBI ID for a phymod
 * @ingroup nfp6000-only
 *
 * @param phymod
 * @param nbi		Pointer to a int for the NBI
 * @return 0 on success, -1 and errno on error
 */
int nfp_phymod_get_nbi(struct nfp_phymod *phymod, int *nbi);

/**
 * Get the base port and/or lanes
 * @ingroup nfp6000-only
 *
 * @param phymod
 * @param base		Pointer to a int for base port (0..23)
 * @param lanes		Pointer to a int for number of phy lanes
 * @return 0 on success, -1 and errno on error
 */
int nfp_phymod_get_port(struct nfp_phymod *phymod, int *base, int *lanes);

/**
 * Get the type ID for the port
 * @ingroup nfp6000-only
 *
 * @param phymod
 * @param type		Pointer to a int for the type (see NFP_PHYMOD_TYPE_*)
 * @return 0 on success, -1 and errno on error
 */
int nfp_phymod_get_type(struct nfp_phymod *phymod, int *type);

/**
 * Indicate logical link on PHY LEDs
 *
 * @param phymod	PHY module
 * @param is_on		0 to disable link indicator, 1 to enable
 */
int nfp_phymod_indicate_link(struct nfp_phymod *phymod, int is_on);

/**
 * Indicate logical activity on PHY LEDs
 *
 * @param phymod	PHY module
 * @param is_on		0 to disable activity indicator, 1 to enable
 */
int nfp_phymod_indicate_activity(struct nfp_phymod *phymod, int is_on);

/**
 * Report status for a PHY module.
 * @ingroup nfp6000-only
 *
 * The returned txstatus parameter indicates the type of PHY module in
 * the most significant byte.  The other bytes contain the summary
 * status for Transmit Loss of Signal (LOS), Fault, Optical Power,
 * Optical Bias, High/Low Voltage and High/Low Temperature.
 *
 * The returned rxstatus parameter contains the summary status for
 * Receive Loss of Signal (LOS), Fault, Optical Power, High/Low
 * Voltage and High/Low Temperature.
 *
 * For the SFP(+) case these summary statuses are the full status for
 * these alarms.  For CXP and QSFP a detailed per-lane status can be
 * obtained for each of these alarms using the associated
 * type-specific function.
 *
 * @param phymod PHY module
 * @param[out] txstatus Transmit status summary for the module
 * @param[out] rxstatus Receive status summary for the module
 *
 * @return 0 on success. Set errno and return -1 on error.
 *
 */
int nfp_phymod_read_status(struct nfp_phymod *phymod, uint32_t *txstatus,
			   uint32_t *rxstatus);

/**
 * Report Loss Of Signal status for a PHY module.
 * @ingroup nfp6000-only
 *
 * The returned txstatus parameter indicates the type of PHY module in
 * the most significant byte.  The other bytes contain the status
 * of Transmit Loss of Signal (LOS) for each lane.  For the SFP(+) case
 * the LOS status is in bit zero; for QSFP bits 0-4 and for CXP
 * bits 0-9.
 *
 * The returned rxstatus parameter indicates the status of Receive
 * Loss of Signal (LOS) for each lane.  For the SFP(+) case the LOS
 * status is in bit zero; for QSFP bits 0-4 and for CXP bits 0-9.
 *
 * @param phymod PHY module
 * @param[out] txstatus Transmit LOS status for the module
 * @param[out] rxstatus Receive LOS status for the module
 *
 * @return 0 on success. Set errno and return -1 on error.
 *
 */
int nfp_phymod_read_status_los(struct nfp_phymod *phymod, uint32_t *txstatus,
			       uint32_t *rxstatus);

/**
 * Report Fault status for a PHY module.
 *
 * The returned txstatus parameter indicates the type of PHY module in
 * the most significant byte.  The other bytes contain the Transmit
 * Fault status for each lane.  For the SFP(+) case the Fault status is
 * in bit zero; for QSFP bits 0-4 and for CXP bits 0-9.
 *
 * The returned rxstatus parameter indicates the Receive Fault status
 * for each lane.  For the SFP(+) case the LOS status is in bit zero;
 * for QSFP bits 0-4 and for CXP bits 0-9.
 *
 * @param phymod PHY module
 * @param[out] txstatus Transmit Fault status for the module
 * @param[out] rxstatus Receive Fault status for the module
 *
 * @return 0 on success. Set errno and return -1 on error.
 *
 */
int nfp_phymod_read_status_fault(struct nfp_phymod *phymod, uint32_t *txstatus,
				 uint32_t *rxstatus);

/**
 * Report Optical Power status for a PHY module.
 * @ingroup nfp6000-only
 *
 * The returned txstatus parameter indicates the type of PHY module in
 * the most significant byte.  The other bytes contain the status of
 * Transmit Optical Power for each lane.  Two bits are used to
 * represent the status for each lane - the MSB for High Power and the
 * LSB for Low Power.  For the SFP(+) case the Optical Power status will
 * be in bits 0-1; for QSFP bits 0-7 and for CXP bits 0-19.
 *
 * The returned rxstatus parameter indicates the status of
 * Receive Optical Power for each lane.  Two bits are used to
 * represent the status for each lane - the MSB for High Power and the
 * LSB for Low Power.  For the SFP(+) case the Optical Power status will
 * be in bits 0-1; for QSFP bits 0-7 and for CXP bits 0-19.
 *
 * @param phymod PHY module
 * @param[out] txstatus Transmit Optical Power status for the module
 * @param[out] rxstatus Receive Optical Power status for the module
 *
 * @return 0 on success. Set errno and return -1 on error.
 *
 */
int nfp_phymod_read_status_optpower(struct nfp_phymod *phymod,
				    uint32_t *txstatus, uint32_t *rxstatus);

/**
 * Report Optical Bias status for a PHY module.
 * @ingroup nfp6000-only
 *
 * The returned txstatus parameter indicates the type of PHY module in
 * the most significant byte.  The other bytes contain the status of
 * Transmit Optical Bias for each lane.  Two bits are used to
 * represent the status for each lane - the MSB for High Bias and the
 * LSB for Low Bias.  For the SFP(+) case the Optical Bias status will
 * be in bits 0-1; for QSFP bits 0-7 and for CXP bits 0-19.
 *
 * @param phymod PHY module
 * @param[out] txstatus Transmit Optical Bias status for the module
 * @param[out] rxstatus Receive Optical Bias status for the module
 *
 * @return 0 on success. Set errno and return -1 on error.
 *
 */
int nfp_phymod_read_status_optbias(struct nfp_phymod *phymod, uint32_t *rxstatus,
				   uint32_t *txstatus);

/**
 * Report High/Low Voltage status for a PHY module.
 * @ingroup nfp6000-only
 *
 * The returned txstatus parameter indicates the type of PHY module in
 * the most significant byte.  The other bytes contain the status of
 * Transmit High/Low Voltage alarms for the module.  Two bits are used
 * to represent the status for each monitored voltage - the most
 * significant bit for High Voltage and the least significant bit for
 * Low Voltage.  For the SFP(+) case only one voltage is monitored and
 * the High/Low Voltage status will be in bits 0-1.  For the QSFP and
 * CXP two voltages are monitored Vcc12 and Vcc3.3.  Two bits are used
 * for each voltage - Vcc3.3 status is in bits 0-1; Vcc12 status is in
 * bits 2-3.
 *
 * The returned rxstatus parameter indicates the status of Receive
 * High/Low Voltage for the module.  Two bits are used to represent
 * the status for each monitored voltage - the most significant bit
 * for High Voltage and the least significant bit for Low Voltage.
 * For the SFP(+) case only one voltage is monitored and the High/Low
 * Voltage status will be in bits 0-1.  For the QSFP and CXP two
 * voltages are monitored Vcc12 and Vcc3.3.  Two bits are used for
 * each voltage - Vcc3.3 status is in bits 0-1; Vcc12 status is in
 * bits 2-3.
 *
 * @param phymod PHY module
 * @param[out] txstatus Transmit High/Low Voltage status for the module
 * @param[out] rxstatus Receive High/Low Voltage status for the module
 *
 * @return 0 on success. Set errno and return -1 on error.
 *
 */
int nfp_phymod_read_status_voltage(struct nfp_phymod *phymod, uint32_t *txstatus,
				   uint32_t *rxstatus);

/**
 * Report High/Low Temperature status for a PHY module.
 * @ingroup nfp6000-only
 *
 * The returned txstatus parameter indicates the type of PHY module in
 * the most significant byte.  The other bytes contain the status of
 * Transmit High/Low Temperature alarms for the module.  Two bits are
 * used to represent the status for temperature - the most significant
 * bit for High Temperature and the least significant bit for Low
 * Temperature.  For all modules the High/Low Temperature status will
 * be in bits 0-1.
 *
 * The returned rxstatus parameter indicates the status of Receive
 * High/Low Temperature for the module.  Two bits are used to
 * represent the status for temperature - the most significant bit for
 * High Temperature and the least significant bit for Low
 * Temperature. For all modules the High/Low Temperature status will be
 * in bits 0-1.
 *
 *
 * @param phymod PHY module
 * @param[out] txstatus Transmit High/Low Temperature status for the module
 * @param[out] rxstatus Receive High/Low Temperature status for the module
 *
 * @return 0 on success. Set errno and return -1 on error.
 *
 */
int nfp_phymod_read_status_temp(struct nfp_phymod *phymod, uint32_t *txstatus,
				uint32_t *rxstatus);

/**
 * Read Lane Disable state for a PHY module.
 * @ingroup nfp6000-only
 *
 * The returned txstatus parameter indicates the type of PHY module in
 * the most significant byte.  The other bytes contain the status of
 * Lane Disable for each lane in the module.
 *
 * The SFP(+) only supports TX_DISABLE.
 * These is returned in bit 0 of txstatus.
 *
 * The QSFP supports independent Transmit and Receive software
 * disables for each lane.  The Transmit Lane Disable states are
 * returned in txstatus bits 0-3, the Receive Lane Disable states are
 * returned in rxstatus bits 0-3.
 *
 * The CXP supports independent Transmit and Receive software disables
 * for each lane.
 *
 * The Transmit Lane Disable states are returned in txstatus bits 0-9
 * The Receive Lane Disable states are returned in txstatus bits 0-9
 *
 *
 * @param phymod PHY module
 * @param[out] txstatus Lane Disable status for the module
 * @param[out] rxstatus Lane Disable status for the module
 *
 * @return 0 on success. Set errno and return -1 on error.
 *
 */
int nfp_phymod_read_lanedisable(struct nfp_phymod *phymod, uint32_t *txstatus,
				uint32_t *rxstatus);

/**
 * Write Lane Disable state for a PHY module.
 * @ingroup nfp6000-only
 *
 * Enable/Disable lanes in a PHY module as specified by the txstates
 * (transmit) and rxstates (receive) parameters.
 *
 * The SFP(+) only supports TX_DISABLE.
 * This is specified in bit 0 of txstatus.
 *
 * The QSFP supports independent Transmit and Receive software
 * disables for each lane.  The Transmit Lane Disable states are
 * specified in txstatus bits 0-3, the Receive Lane Disable statesare
 * specified in rxstatus bits 0-3.
 *
 * The CXP supports independent Transmit and Receive software disables
 * for each lane.
 *
 * The Transmit Lane Disable states are specified in txstatus bits 0-9
 * The Receive Lane Disable states are specified in txstatus bits 0-9
 *
 *
 * @param phymod PHY module
 * @param[in] txstate Lane Disable states for the module
 * @param[in] rxstate Lane Disable states for the module
 *
 * @return 0 on success. Set errno and return -1 on error.
 *
 */
int nfp_phymod_write_lanedisable(struct nfp_phymod *phymod, uint32_t txstate,
				 uint32_t rxstate);

/**
 * Verify SFF address map check code.
 * @ingroup nfp6000-only
 *
 * @param phymod          PHY module
 * @param cc_status[out]  Check code status flags
 *
 * cc_status bit 0 is set if the base check code is incorrect.
 * cc_status bit 1 is set if the extended check code is incorrect.
 * a value of 0 for cc_status indicates all check codes are correct.
 *
 * @return 0 on success. return -1 on error.
 *
 */
int nfp_phymod_verify_sff_checkcode(struct nfp_phymod *phymod, int *cc_status);

/**
 * Enumertate SFF address map check-code regions.
 * @ingroup nfp6000-only
 *
 * @param phymod          PHY module
 * @param idx[in]
 * @param cc_offs[out]
 * @param base[out]
 * @param len[out]
 *
 *
 * @return 0 on success. return -1 on error.
 *
 */
int nfp_phymod_get_cc_info(struct nfp_phymod *phymod, int *idx, uint8_t *cc_offs,
			   uint8_t *base, uint8_t *len);

/**
 * Populates eeprom data cache for module.
 * @ingroup nfp6000-only
 *
 * @param phymod          PHY module
 *
 */
int nfp_phymod_eeprom_cache(struct nfp_phymod *phy);

/**
 * Disables use of eeprom caching for module.
 * @ingroup nfp6000-only
 *
 * @param phymod          PHY module
 *
 */
int nfp_phymod_eeprom_cache_clear(struct nfp_phymod *phy);

/**
 * Determines optimal io bus speed
 * @ingroup nfp6000-only
 *
 * @param phymod          PHY module
 *
 */
int nfp_phymod_io_bus_setup(struct nfp_phymod *phy);

/**
 * Retrieve ascii vendor name
 * @ingroup nfp6000-only
 *
 * @param phymod PHY module
 * @param name[out]  location to copy name string to.
 * @param size[in]   size of memory pointed to by name
 *
 * Upon success name is guaranteed to be NULL terminated.
 *
 * @return 0 on success. return -1 on error.
 *
 */
int nfp_phymod_read_vendor(struct nfp_phymod *phymod, char *name, uint32_t size);

/**
 * Retrieve vendor OUI code
 * @ingroup nfp6000-only
 *
 * @param phymod PHY module
 * @param oui[out] OUI value
 *
 * @return 0 on success. return -1 on error.
 *
 */
int nfp_phymod_read_oui(struct nfp_phymod *phymod, uint32_t *oui);

/**
 * Retrieve ascii product name
 * @ingroup nfp6000-only
 *
 * @param phymod PHY module
 * @param product[out]  location to copy product string to.
 * @param size[in]      size of memory pointed to by product
 *
 * Upon success product is guaranteed to be NULL terminated.
 *
 * @return 0 on success. return -1 on error.
 *
 */
int nfp_phymod_read_product(struct nfp_phymod *phymod, char *product, uint32_t size);

/**
 * Retrieve ascii product serial string
 * @ingroup nfp6000-only
 *
 * @param phymod PHY module
 * @param serial[out]  location to copy string to.
 * @param size[in]      size of memory pointed to by serial
 *
 * Upon success product is guaranteed to be NULL terminated.
 *
 * @return 0 on success. return -1 on error.
 *
 */
int nfp_phymod_read_serial(struct nfp_phymod *phymod, char *serial, uint32_t size);

/**
 * Read module type
 * @ingroup nfp6000-only
 *
 * @param      phymod PHY module
 * @param[out] type   Type of PHY module
 *
 * @return 0 on success. return -1 on error.
 *
 */
int nfp_phymod_read_type(struct nfp_phymod *phymod, int *type);

/**
 * Read module connector type
 * @ingroup nfp6000-only
 *
 * @param      phymod PHY module
 * @param[out] type   Type of PHY module
 *
 * @return 0 on success. return -1 on error.
 *
 */
int nfp_phymod_read_connector(struct nfp_phymod *phymod, int *connector);

/**
 * Retrieve supported link length of the PHY module
 * @ingroup nfp6000-only
 *
 * @param      phymod PHY module
 * @param[out] length Supported length of PHY
 *
 * @return 0 on success. return -1 on error.
 *
 */
int nfp_phymod_read_length(struct nfp_phymod *phymod, int *length);

/**
 * Retrieve transceiver extended compliance code of the PHY module
 * @ingroup nfp6000-only
 *
 * @param      phymod PHY module
 * @val[out]   Ext. Comp. code of PHY
 *
 * @return 0 on success. return -1 on error.
 *
 */
int nfp_phymod_read_extended_compliance_code(struct nfp_phymod *phymod,
					     uint32_t *val);

/**
 * Determine if the PHY is active or passive
 * @ingroup nfp6000-only
 *
 * @param      phymod PHY module
 * @param[out] active
 *
 * @return 0 on success. return -1 on error.
 *
 */
int nfp_phymod_get_active_or_passive(struct nfp_phymod *phymod, int *active);

/**
 * Read a PHY module address (8-bit).
 * @ingroup nfp6000-only
 *
 * @param phymod PHY module
 * @param[in] addr address
 * @param[out] data return value
 *
 * @return 0 on success. Set errno and return -1 on error.
 *
 */
int nfp_phymod_read8(struct nfp_phymod *phymod, uint32_t addr, uint8_t *data);

/**
 * Write a PHY module address (8-bit).
 * @ingroup nfp6000-only
 *
 * @param phymod PHY module
 * @param[in] addr address
 * @param[in] data value
 *
 * @return 0 on success. Set errno and return -1 on error.
 *
 */
int nfp_phymod_write8(struct nfp_phymod *phymod, uint32_t addr, uint8_t data);

/**
 * Read a PHY module address (16-bit data).
 * @ingroup nfp6000-only
 *
 * @param phymod PHY module
 * @param[in] addr address
 * @param[out] data return value
 *
 * @return 0 on success. Set errno and return -1 on error.
 *
 */
int nfp_phymod_read16(struct nfp_phymod *phymod, uint32_t addr, uint16_t *data);

/**
 * Write a PHY module address (8-bit data).
 * @ingroup nfp6000-only
 *
 * @param phymod PHY module
 * @param[in] addr address
 * @param[in] data value
 *
 * @return 0 on success. Set errno and return -1 on error.
 *
 */
int nfp_phymod_write16(struct nfp_phymod *phymod, uint32_t addr, uint16_t data);

/**
 * Initilize a PHY .
 * @ingroup nfp6000-only
 *
 * @param phymod PHY module
 *
 * @return 0 on success. Set errno and return -1 on error.
 *
 */
int nfp_phymod_init_device(struct nfp_phymod *phymod);

/**
 * PHY Module Ethernet port enumeration
 * @ingroup nfp6000-only
 *
 * This function allows enumeration of the Ethernet ports
 * attached to a PHY module
 *
 * @param nfp   NFP Device
 * @param phy   PHY module, or NULL for all ethernet ports
 * @param ptr   Abstract pointer, must be NULL to get the first port
 * @return  On succes: phymod
 * @return  On error: NULL
 */
struct nfp_phymod_eth *nfp_phymod_eth_next(struct nfp_device *nfp,
					   struct nfp_phymod *phy, void **ptr);

/**
 * Get the index for a phymod's eth interface
 * @ingroup nfp6000-only
 *
 * @param eth		PHY module ethernet interface
 * @param index	Pointer to a int for the index (unique for all eths)
 * @return 0 on success, -1 and errno on error
 */
int nfp_phymod_eth_get_index(struct nfp_phymod_eth *eth, int *index);

/**
 * Get the flag for whether the eth is bootable
 * @ingroup nfp6000-only
 *
 * @param eth		PHY module ethernet interface
 * @param bootable	Pointer to a int for flag (0 or 1)
 * @return 0 on success, -1 and errno on error
 */
int nfp_phymod_eth_get_bootable(struct nfp_phymod_eth *eth, int *bootable);

/**
 * Get the phymod and base lane for a phymod's eth interface
 * @ingroup nfp6000-only
 *
 * @param eth		PHY module ethernet interface
 * @param phy		Pointer to a phymod, set to the parent PHY of this eth
 * @param[out] lane		Pointer to a int for the PHY lane for this eth
 * @return 0 on success, -1 and errno on error
 */
int nfp_phymod_eth_get_phymod(struct nfp_phymod_eth *eth,
			      struct nfp_phymod **phy, int *lane);

/**
 * Get the MAC address of and ethernet port
 * @ingroup nfp6000-only
 *
 * @param eth		PHY module ethernet interface
 * @param mac		Pointer to a const uint8_t * for the 6-byte MAC
 * @return 0 on success, -1 and errno on error
 */
int nfp_phymod_eth_get_mac(struct nfp_phymod_eth *eth, const uint8_t **mac);

/**
 * Get the string (UTF8) label for a phymod's Ethernet interface
 * @ingroup nfp6000-only
 *
 * @param eth		PHY module ethernet interface
 * @param label		Pointer to a const char * for the label
 * @return 0 on success, -1 and errno on error
 */
int nfp_phymod_eth_get_label(struct nfp_phymod_eth *eth, const char **label);

/**
 * Get the NBI ID for a phymod's Ethernet interface
 * @ingroup nfp6000-only
 *
 * @param eth		PHY module ethernet interface
 * @param nbi		Pointer to a int for the NBI
 * @return 0 on success, -1 and errno on error
 */
int nfp_phymod_eth_get_nbi(struct nfp_phymod_eth *eth, int *nbi);

/**
 * Get the base port and/or lanes
 * @ingroup nfp6000-only
 *
 * @param eth		PHY module ethernet interface
 * @param base		Pointer to a int for base port (0..23)
 * @param lanes		Pointer to a int for number of phy lanes
 * @return 0 on success, -1 and errno on error
 */
int nfp_phymod_eth_get_port(struct nfp_phymod_eth *eth, int *base, int *lanes);

/**
 * Get the speed of the Ethernet port (in megabits/sec)
 * @ingroup nfp6000-only
 *
 * @param eth		PHY module ethernet interface
 * @param speed		Pointer to a int for speed (in megabits/sec)
 * @return 0 on success, -1 and errno on error
 */
int nfp_phymod_eth_get_speed(struct nfp_phymod_eth *eth, int *speed);

/**
 * Retrieve the fail-to-wire TX partner for an ethernet port
 * This is the label of the port that, when the port is in fail-to-wire
 * mode, all inbound packets are redirected to via external switching
 * hardware.
 *
 * Note that this is a system-wide label, and may not be in the ethernet
 * port set for this PHY, NBI, or even NFP.
 *
 * @param eth           PHY module ethernet interface
 * @param eth_label     Pointer to a const char * to receive the label,
 *                      or NULL if there is no fail-to-wire partner.
 * @param active        Pointer to a int, which will hold 0 or 1 to indicate
 *                      whether the fail-to-wire mode is active.
 * @return 0 on success, -1 and errno on error
 */
int nfp_phymod_eth_get_fail_to_wire(struct nfp_phymod_eth *eth,
				    const char **eth_label, int *active);

/**
 * Force fail-to-wire mode, if available.
 *
 * @param eth           PHY module ethernet interface
 * @param force         0 for automatic fail to wire, 1 to force
 * @return 0 on success, -1 and errno on error
 */
int nfp_phymod_eth_set_fail_to_wire(struct nfp_phymod_eth *eth, int force);

/**
 * Read PHY Disable state for an eth port
 * @ingroup nfp6000-only
 *
 * @param eth           PHY module ethernet interface
 * @param[out] txstatus Disable status for the ethernet port
 * @param[out] rxstatus Disable status for the ethernet port
 *
 * @return 0 on success. Set errno and return -1 on error.
 *
 * For both rxstatus and txstatus, 0 = active, 1 = disabled
 */
int nfp_phymod_eth_read_disable(struct nfp_phymod_eth *eth, uint32_t *txstatus,
				uint32_t *rxstatus);

/**
 * Write PHY Disable state for an eth port
 * @ingroup nfp6000-only
 *
 * @param eth           PHY module ethernet interface
 * @param[in] txstate Disable states for the module
 * @param[in] rxstate Disable states for the module
 *
 * @return 0 on success. Set errno and return -1 on error.
 *
 * For both rxstatus and txstatus, 0 = active, 1 = disabled
 */
int nfp_phymod_eth_write_disable(struct nfp_phymod_eth *eth, uint32_t txstate,
				 uint32_t rxstate);
#ifdef __cplusplus
}
#endif
#endif
