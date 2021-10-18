/*
 * Copyright (C) 2013, Netronome Systems, Inc.  All rights reserved.
 *
 * Disclaimer: this file is provided without any warranty
 * as part of an early-access program, and the content is
 * bound to change before the final release.
 */

#ifndef NFP3800_PL_H
#define NFP3800_PL_H

/*----------------------------------------------------------------*/
/* XPB Address Map (XPB_BASE = NFP3800_XPB_ISLAND(1) + NFP3800_ARMX_PLU)    */


/* HGID: 6ea1af6fca88 */

#ifndef NFP3800_LONGNAMES

/*----------------------------------------------------------------
  Register: PluMisc_PluMasterReset - Programmable Reset Controls
    Bits: 28    WDToMClkPD(rw) - When 1, Watchdog Timer Pop causes M_CLK PLL to power down
    Bits: 27    WDToAClkPD(rw) - When 1, Watchdog Timer Pop causes A_CLK PLL to power down
    Bits: 26    WDToPClkPD(rw) - When 1, Watchdog Timer Pop causes P_CLK PLL to power down
    Bits: 25    WDToTClkPD(rw) - When 1, Watchdog Timer Pop causes T_CLK PLL to power down
    Bits: 24    WDToClkNResetOut(rw) - When 1, Watchdog Timer Pop causes asserion low of CLK_NRESET_OUT_L
    Bits: 23    TSTempAlrtToMClkPD(rw) - When 1, TSTempAlrt causes M_CLK PLL to power down
    Bits: 22    TSTempAlrtToAClkPD(rw) - When 1, TSTempAlrt causes A_CLK PLL to power down
    Bits: 21    TSTempAlrtToPClkPD(rw) - When 1, TSTempAlrt causes P_CLK PLL to power down
    Bits: 20    TSTempAlrtToTClkPD(rw) - When 1, TSTempAlrt causes T_CLK PLL to power down
    Bits: 19    TSTempAlrtToClkNResetOut(rw) - When 1, TSTempAlrt causes asserion low of CLK_NRESET_OUT_L
    Bits: 18    TSTempAlrtToSoftReset(rw) - When 1, TSTempAlrt causes a chip-level Soft Reset
    Bits: 17    TSTempTripToMClkPD(rw) - When 1, TSTempTrip causes M_CLK PLL to power down
    Bits: 16    TSTempTripToAClkPD(rw) - When 1, TSTempTrip causes A_CLK PLL to power down
    Bits: 15    TSTempTripToPClkPD(rw) - When 1, TSTempTrip causes P_CLK PLL to power down
    Bits: 14    TSTempTripToTClkPD(rw) - When 1, TSTempTrip causes T_CLK PLL to power down
    Bits: 13    TSTempTripToClkNResetOut(rw) - When 1, TSTempTrip causes assertion low of CLK_NRESET_OUT_L
    Bits: 12    TSTempTripToSoftReset(rw) - When 1, TSTempTrip causes a chip-level Soft Reset
    Bits:  8    PCIE0_RESET_MASK(rw) - When set to 1, PCIE0_RESET_OUT_L will not be reset by SRESETN
    Bits:  4    PCIE0_RESET_OUT_L(rw) - PCIE0 Reset Out
    Bits:  3    SResetToClkNResetOut(rw) - When 1, Chip Pin SRESETN==0 causes CLK_NRESET_OUT_L set to 0
    Bits:  2    ProgSResetToClkNResetOut(rw) - When 1, ProgSReset==0 causes CLK_NRESET_OUT_L set to 0
    Bits:  1    PROG_SRESET_L(rw) - Programmable Chip Soft Reset; Asserted Low
    Bits:  0    CLK_NRESET_OUT_L(rw) - PowerOn Reset Out; Asserted Low
*/
#define NFP3800_PL_PluMisc_PLU_MASTER_RESET                  0x00000000
#define   NFP3800_PL_PLUMISC_PLU_MASTER_RESET_WDTOMCLKPD     BIT(28)
#define   NFP3800_PL_PLUMISC_PLU_MASTER_RESET_WDTOACLKPD     BIT(27)
#define   NFP3800_PL_PLUMISC_PLU_MASTER_RESET_WDTOPCLKPD     BIT(26)
#define   NFP3800_PL_PLUMISC_PLU_MASTER_RESET_WDTOTCLKPD     BIT(25)
#define   NFP3800_PL_PLUMISC_PLU_MASTER_RESET_WDTOCLKNRESETOUT BIT(24)
#define   NFP3800_PL_PLUMISC_PLU_MASTER_RESET_TSTEMPALRTTOMCLKPD BIT(23)
#define   NFP3800_PL_PLUMISC_PLU_MASTER_RESET_TSTEMPALRTTOACLKPD BIT(22)
#define   NFP3800_PL_PLUMISC_PLU_MASTER_RESET_TSTEMPALRTTOPCLKPD BIT(21)
#define   NFP3800_PL_PLUMISC_PLU_MASTER_RESET_TSTEMPALRTTOTCLKPD BIT(20)
#define   NFP3800_PL_PLUMISC_PLU_MASTER_RESET_TSTEMPALRTTOCLKNRESETOUT BIT(19)
#define   NFP3800_PL_PLUMISC_PLU_MASTER_RESET_TSTEMPALRTTOSOFTRESET BIT(18)
#define   NFP3800_PL_PLUMISC_PLU_MASTER_RESET_TSTEMPTRIPTOMCLKPD BIT(17)
#define   NFP3800_PL_PLUMISC_PLU_MASTER_RESET_TSTEMPTRIPTOACLKPD BIT(16)
#define   NFP3800_PL_PLUMISC_PLU_MASTER_RESET_TSTEMPTRIPTOPCLKPD BIT(15)
#define   NFP3800_PL_PLUMISC_PLU_MASTER_RESET_TSTEMPTRIPTOTCLKPD BIT(14)
#define   NFP3800_PL_PLUMISC_PLU_MASTER_RESET_TSTEMPTRIPTOCLKNRESETOUT BIT(13)
#define   NFP3800_PL_PLUMISC_PLU_MASTER_RESET_TSTEMPTRIPTOSOFTRESET BIT(12)
#define   NFP3800_PL_PLUMISC_PLU_MASTER_RESET_PCIE0_RESET_MASK BIT(8)
#define   NFP3800_PL_PLUMISC_PLU_MASTER_RESET_PCIE0_RESET_OUT_L BIT(4)
#define   NFP3800_PL_PLUMISC_PLU_MASTER_RESET_SRESETTOCLKNRESETOUT BIT(3)
#define   NFP3800_PL_PLUMISC_PLU_MASTER_RESET_PROGSRESETTOCLKNRESETOUT BIT(2)
#define   NFP3800_PL_PLUMISC_PLU_MASTER_RESET_PROG_SRESET_L  BIT(1)
#define   NFP3800_PL_PLUMISC_PLU_MASTER_RESET_CLK_NRESET_OUT_L BIT(0)
/*----------------------------------------------------------------
  Register: PluMisc_PluDeviceID - Chip Device ID Register.
    Bits: 31:16 PartNumber(ro) - Major Part Number Designation (Hard Coded)
    Bits: 15:8  SKU(ro) - SKU Designation (From Fuses)
    Bits:  7:4  MajorRevID(ro) - Major Revision ID (Hard Coded)
    Bits:  3:0  MinorRevID(ro) - Minor Revision ID (From Fuses or Special Metal Structure)
*/
#define NFP3800_PL_PluMisc_PLU_DEVICE_ID                     0x00000004
#define   NFP3800_PL_PLUMISC_PLU_DEVICE_ID_PART_NUM(_x)      (((_x) & 0xffff) << 16)
#define   NFP3800_PL_PLUMISC_PLU_DEVICE_ID_PART_NUM_of(_x)   (((_x) >> 16) & 0xffff)
#define   NFP3800_PL_PLUMISC_PLU_DEVICE_ID_SKU(_x)           (((_x) & 0xff) << 8)
#define   NFP3800_PL_PLUMISC_PLU_DEVICE_ID_SKU_of(_x)        (((_x) >> 8) & 0xff)
#define   NFP3800_PL_PLUMISC_PLU_DEVICE_ID_MAJOR_REV_ID(_x)  (((_x) & 0xf) << 4)
#define   NFP3800_PL_PLUMISC_PLU_DEVICE_ID_MAJOR_REV_ID_of(_x) (((_x) >> 4) & 0xf)
#define   NFP3800_PL_PLUMISC_PLU_DEVICE_ID_MINOR_REV_ID(_x)  (((_x) & 0xf) << 0)
#define   NFP3800_PL_PLUMISC_PLU_DEVICE_ID_MINOR_REV_ID_of(_x) (((_x) >> 0) & 0xf)
/*----------------------------------------------------------------
  Register: PluMisc_PluPowerState - Indicators for Power and Power-Domain Status
    Bits:  2    CRYPTO_PDL(ro) - Crypto Power Domain Locked
    Bits:  1    POWERGOOD_IO(ro) - IO Power Domain Power Good Indicator.
    Bits:  0    POWERGOOD_CORE(ro) - Core Power Domain Power Good Indicator.
*/
#define NFP3800_PL_PluMisc_PLU_POWER_STATE                   0x0000000c
#define   NFP3800_PL_PLUMISC_PLU_POWER_STATE_CRYPTO_PDL      BIT(2)
#define   NFP3800_PL_PLUMISC_PLU_POWER_STATE_POWERGOOD_IO    BIT(1)
#define   NFP3800_PL_PLUMISC_PLU_POWER_STATE_POWERGOOD_CORE  BIT(0)
/*----------------------------------------------------------------
  Register: PluMisc_PluBaudRateGen - Control Register for Baud Rate Generator.
    Bits: 31:16 BaudControlA(rw) - Control A for Baud Rate Generator. See Baud Rate Generator Table
    Bits: 15:0  BaudControlB(rw) - Control B for Baud Rate Generator. See Baud Rate Generator Table
*/
#define NFP3800_PL_PluMisc_PLU_BAUD_RATE_GEN                 0x00000018
#define   NFP3800_PL_PLUMISC_PLU_BAUD_RATE_GEN_BAUD_CTRL_A(_x) (((_x) & 0xffff) << 16)
#define   NFP3800_PL_PLUMISC_PLU_BAUD_RATE_GEN_BAUD_CTRL_A_of(_x) (((_x) >> 16) & 0xffff)
#define   NFP3800_PL_PLUMISC_PLU_BAUD_RATE_GEN_BAUD_CTRL_B(_x) (((_x) & 0xffff) << 0)
#define   NFP3800_PL_PLUMISC_PLU_BAUD_RATE_GEN_BAUD_CTRL_B_of(_x) (((_x) >> 0) & 0xffff)
/*----------------------------------------------------------------
  Register: PluMisc_PluMiscClockDivControl - Miscellaneous Clock Divider Control
    Bits: 23:16 RefClkDiv10DivRatio(rw) - REF_CLK divider ratio to internally generate REF_CLK_DIV10 solely for Power Switches. Legal values: 0-255
    Bits: 15:8  PCIEPwrMgmtClkDivRatio(rw) - REF_CLK divider ratio to internally generate PCIE_PWR_MGMT_CLK. Legal values: 0-255
    Bits:  7:0  PClkDiv4DivRatio(rw) - P_CLK divider ratio to internally generate P_CLK_DIV4. Legal values: 0-255
*/
#define NFP3800_PL_PluMisc_PLU_MISC_CLOCK_DIV_CONTROL        0x00000034
#define   NFP3800_PL_PLUMISC_PLU_MISC_CLOCK_DIV_CONTROL_REFCLK_DIV10_DIV_RATIO(_x) (((_x) & 0xff) << 16)
#define   NFP3800_PL_PLUMISC_PLU_MISC_CLOCK_DIV_CONTROL_REFCLK_DIV10_DIV_RATIO_of(_x) (((_x) >> 16) & 0xff)
#define     NFP3800_PL_PLUMISC_PLU_MISC_CLOCK_DIV_CONTROL_REFCLK_DIV10_DIV_RATIO_OFF (0)
#define     NFP3800_PL_PLUMISC_PLU_MISC_CLOCK_DIV_CONTROL_REFCLK_DIV10_DIV_RATIO_BYP (1)
#define   NFP3800_PL_PLUMISC_PLU_MISC_CLOCK_DIV_CONTROL_PCIE_PWR_MGMT_CLK_DIV_RATIO(_x) (((_x) & 0xff) << 8)
#define   NFP3800_PL_PLUMISC_PLU_MISC_CLOCK_DIV_CONTROL_PCIE_PWR_MGMT_CLK_DIV_RATIO_of(_x) (((_x) >> 8) & 0xff)
#define     NFP3800_PL_PLUMISC_PLU_MISC_CLOCK_DIV_CONTROL_PCIE_PWR_MGMT_CLK_DIV_RATIO_OFF (0)
#define     NFP3800_PL_PLUMISC_PLU_MISC_CLOCK_DIV_CONTROL_PCIE_PWR_MGMT_CLK_DIV_RATIO_BYP (1)
#define   NFP3800_PL_PLUMISC_PLU_MISC_CLOCK_DIV_CONTROL_PCLKDIV4_DIV_RATIO(_x) (((_x) & 0xff) << 0)
#define   NFP3800_PL_PLUMISC_PLU_MISC_CLOCK_DIV_CONTROL_PCLKDIV4_DIV_RATIO_of(_x) (((_x) >> 0) & 0xff)
#define     NFP3800_PL_PLUMISC_PLU_MISC_CLOCK_DIV_CONTROL_PCLKDIV4_DIV_RATIO_OFF (0)
#define     NFP3800_PL_PLUMISC_PLU_MISC_CLOCK_DIV_CONTROL_PCLKDIV4_DIV_RATIO_BYP (1)
/*----------------------------------------------------------------
  Register: PluMisc_PluJtagTckControl - JTAG_TCK Internal Generation Control
    Bits:  7:0  ClkDivRatio(rw) - REF_CLK divider ratio to internally generate JTAG_TCK. Legal values: 0-255
*/
#define NFP3800_PL_PluMisc_PLU_JTAG_TCK_CONTROL              0x00000038
#define   NFP3800_PL_PLUMISC_PLU_JTAG_TCK_CONTROL_CLK_DIV_RATIO(_x) (((_x) & 0xff) << 0)
#define   NFP3800_PL_PLUMISC_PLU_JTAG_TCK_CONTROL_CLK_DIV_RATIO_of(_x) (((_x) >> 0) & 0xff)
#define     NFP3800_PL_PLUMISC_PLU_JTAG_TCK_CONTROL_CLK_DIV_RATIO_OFF (0)
#define     NFP3800_PL_PLUMISC_PLU_JTAG_TCK_CONTROL_CLK_DIV_RATIO_BYP (1)
/*----------------------------------------------------------------
  Register: PluMisc_PluSMClockControl - SM CLock Internal Generation Control
    Bits:  8    ClkSource(rw) - When 0, Clock Divider input is REF_CLK; when 1, MAC0_CLK PLL Output.
    Bits:  7:0  ClkDivRatio(rw) - REF_CLK divider ratio to internally generate SM_CLK. Legal values: 0-255
*/
#define NFP3800_PL_PluMisc_PLU_SM_CLOCK_CONTROL              0x0000003c
#define   NFP3800_PL_PLUMISC_PLU_SM_CLOCK_CONTROL_CLK_SOURCE BIT(8)
#define   NFP3800_PL_PLUMISC_PLU_SM_CLOCK_CONTROL_CLK_DIV_RATIO(_x) (((_x) & 0xff) << 0)
#define   NFP3800_PL_PLUMISC_PLU_SM_CLOCK_CONTROL_CLK_DIV_RATIO_of(_x) (((_x) >> 0) & 0xff)
#define     NFP3800_PL_PLUMISC_PLU_SM_CLOCK_CONTROL_CLK_DIV_RATIO_OFF (0)
#define     NFP3800_PL_PLUMISC_PLU_SM_CLOCK_CONTROL_CLK_DIV_RATIO_BYP (1)
/*----------------------------------------------------------------
  Register: PluMisc_PluChipSerialNumber - Per Chip Serial Number
    Bits: 31:0  ChipSerialNumber(ro) - Per Chip Serial Number
*/
#define NFP3800_PL_PluMisc_PLU_CHIP_SERIAL_NUMBER            0x00000040
#define   NFP3800_PL_PLUMISC_PLU_CHIP_SERIAL_NUMBER_CHIP_SERIAL_NUMBER(_x) (((_x) & 0xffffffff) << 0)
#define   NFP3800_PL_PLUMISC_PLU_CHIP_SERIAL_NUMBER_CHIP_SERIAL_NUMBER_of(_x) (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: PluMisc_PluMacClockDiv4Control - MACCLK Divider Control
    Bits:  7:0  ClkDivRatio(rw) - MAC_CLK divider ratio to generate MAC0_CLK_DIV4. Legal values: 0-255
*/
#define NFP3800_PL_PluMisc_PLU_MAC_CLOCK_DIV4_CONTROL        0x00000050
#define   NFP3800_PL_PLUMISC_PLU_MAC_CLOCK_DIV4_CONTROL_CLK_DIV_RATIO(_x) (((_x) & 0xff) << 0)
#define   NFP3800_PL_PLUMISC_PLU_MAC_CLOCK_DIV4_CONTROL_CLK_DIV_RATIO_of(_x) (((_x) >> 0) & 0xff)
#define     NFP3800_PL_PLUMISC_PLU_MAC_CLOCK_DIV4_CONTROL_CLK_DIV_RATIO_OFF (0)
#define     NFP3800_PL_PLUMISC_PLU_MAC_CLOCK_DIV4_CONTROL_CLK_DIV_RATIO_BYP (1)
/*----------------------------------------------------------------
  Register: PluTSXpb_PluTSRegIntfc - Thermal Sensor Register Interface
    Bits: 31:30 Start(ro) - Start bits for Thermal Sensor Register Interface Frame
    Bits: 29:28 Op(rw) - OpCode for Thermal Sensor Register Interface
    Bits: 27:18 RegAddr(rw) - Register Address for Thermal Sensor Register Interface
    Bits: 15:0  Data(rw) - Register Write/Read Data for Thermal Sensor Register Interface
*/
#define NFP3800_PL_PluTSXpb_PLU_TS_REG_INTFC                 0x00010000
#define   NFP3800_PL_PLUTSXPB_PLU_TS_REG_INTFC_START(_x)     (((_x) & 0x3) << 30)
#define   NFP3800_PL_PLUTSXPB_PLU_TS_REG_INTFC_START_of(_x)  (((_x) >> 30) & 0x3)
#define   NFP3800_PL_PLUTSXPB_PLU_TS_REG_INTFC_OP(_x)        (((_x) & 0x3) << 28)
#define   NFP3800_PL_PLUTSXPB_PLU_TS_REG_INTFC_OP_of(_x)     (((_x) >> 28) & 0x3)
#define     NFP3800_PL_PLUTSXPB_PLU_TS_REG_INTFC_OP_NOOP     (0)
#define     NFP3800_PL_PLUTSXPB_PLU_TS_REG_INTFC_OP_WRITE    (1)
#define   NFP3800_PL_PLUTSXPB_PLU_TS_REG_INTFC_REGADDR(_x)   (((_x) & 0x3ff) << 18)
#define   NFP3800_PL_PLUTSXPB_PLU_TS_REG_INTFC_REGADDR_of(_x) (((_x) >> 18) & 0x3ff)
#define   NFP3800_PL_PLUTSXPB_PLU_TS_REG_INTFC_DATA(_x)      (((_x) & 0xffff) << 0)
#define   NFP3800_PL_PLUTSXPB_PLU_TS_REG_INTFC_DATA_of(_x)   (((_x) >> 0) & 0xffff)
/*----------------------------------------------------------------
  Register: PluTSXpb_PluTSReadings - Filtered Thermal Sensor Readings. (HIGHLY RECOMMENDED)
    Bits: 14    TSotstemperaturetrip(ro) - Temperature catastrophic Trip signal based on the threshold set by TSTripThreshold; Temperature must rise above TSTripThreshold for Trip to assert. Trip will remain asserted until the TSTripThreshold is disabled and re-enabled.
    Bits: 13    TSotstemperaturealrt(ro) - Temperature Alert signal based on the thresholds set by TSAlertThreshold and TSAlertVariation; Temperature must rise above Threshold for Alert to assert; Temperature must fall below TSAlertThreshold-TSAlertVariation for Alert to de-assert.
    Bits: 12    TSotstemperaturevalid(ro) - Temperature Reading has been Updated and is Valid. When this register is read, this bit will be cleared until the next temperature reading arrives.
    Bits: 11:0  TSotstemperature(ro) - 12 bit temperature reading in degC
*/
#define NFP3800_PL_PluTSXpb_PLU_TS_READINGS                  0x00010004
#define   NFP3800_PL_PLUTSXPB_PLU_TS_READINGS_TS_TEMPERATURE_TRIP BIT(14)
#define   NFP3800_PL_PLUTSXPB_PLU_TS_READINGS_TS_TEMPERATURE_ALERT BIT(13)
#define   NFP3800_PL_PLUTSXPB_PLU_TS_READINGS_TS_TEMPERATURE_VALID BIT(12)
#define   NFP3800_PL_PLUTSXPB_PLU_TS_READINGS_TS_TEMPERATURE(_x) (((_x) & 0xfff) << 0)
#define   NFP3800_PL_PLUTSXPB_PLU_TS_READINGS_TS_TEMPERATURE_of(_x) (((_x) >> 0) & 0xfff)
/*----------------------------------------------------------------
  Register: PluTSXpb_PluTSTemperatureTrip - Thermal Sensor Catastrophic Trip Control Register
    Bits: 31    TSTripThresholdEnable(rw) - Must be set to 1 for the TSTripThreshold to be checked
    Bits: 30    TSTripThresholdSelect(rw) - Selects the trip comparison to be used to generate an trigger for the reset logic.
    Bits: 11:0  TSTripThreshold(rw) - 12 bit temperature Catastrophic Trip Threshold.
*/
#define NFP3800_PL_PluTSXpb_PLU_TS_TEMPERATURE_TRIP          0x00010008
#define   NFP3800_PL_PLUTSXPB_PLU_TS_TEMPERATURE_TRIP_TS_TRIP_THRESHOLD_ENABLE BIT(31)
#define   NFP3800_PL_PLUTSXPB_PLU_TS_TEMPERATURE_TRIP_TS_TRIP_THRESHOLD_SELECT BIT(30)
#define     NFP3800_PL_PLUTSXPB_PLU_TS_TEMPERATURE_TRIP_TS_TRIP_THRESHOLD_SELECT_FILTERED (0 << 30)
#define     NFP3800_PL_PLUTSXPB_PLU_TS_TEMPERATURE_TRIP_TS_TRIP_THRESHOLD_SELECT_RAW BIT(30)
#define   NFP3800_PL_PLUTSXPB_PLU_TS_TEMPERATURE_TRIP_TS_TRIP_THRESHOLD(_x) (((_x) & 0xfff) << 0)
#define   NFP3800_PL_PLUTSXPB_PLU_TS_TEMPERATURE_TRIP_TS_TRIP_THRESHOLD_of(_x) (((_x) >> 0) & 0xfff)
/*----------------------------------------------------------------
  Register: PluTSXpb_PluTSTemperatureAlert - Thermal Sensor Alert (w/hysteresis) Control Register
    Bits: 31    TSAlertThresholdEnable(rw) - Must be set to 1 for the TSAlertThreshold to be checked
    Bits: 30    TSAlertThresholdSelect(rw) - Selects the alert comparison to be used to generate an trigger for the reset logic
    Bits: 27:16 TSAlertVariation(rw) - 12 bit temperature Alert Variation. Provides Hysteresis. Temperature must fall below Threshold-Variation for Alert to de-assert.
    Bits: 11:0  TSAlertThreshold(rw) - 12 bit temperature Alert Threshold. Temperature must rise above Threshold for Alert to assert
*/
#define NFP3800_PL_PluTSXpb_PLU_TS_TEMPERATURE_ALERT         0x0001000c
#define   NFP3800_PL_PLUTSXPB_PLU_TS_TEMPERATURE_ALERT_TS_ALERT_THRESHOLD_ENABLE BIT(31)
#define   NFP3800_PL_PLUTSXPB_PLU_TS_TEMPERATURE_ALERT_TS_ALERT_THRESHOLD_SELECT BIT(30)
#define     NFP3800_PL_PLUTSXPB_PLU_TS_TEMPERATURE_ALERT_TS_ALERT_THRESHOLD_SELECT_FILTERED (0 << 30)
#define     NFP3800_PL_PLUTSXPB_PLU_TS_TEMPERATURE_ALERT_TS_ALERT_THRESHOLD_SELECT_RAW BIT(30)
#define   NFP3800_PL_PLUTSXPB_PLU_TS_TEMPERATURE_ALERT_TS_ALERT_VARIATION(_x) (((_x) & 0xfff) << 16)
#define   NFP3800_PL_PLUTSXPB_PLU_TS_TEMPERATURE_ALERT_TS_ALERT_VARIATION_of(_x) (((_x) >> 16) & 0xfff)
#define   NFP3800_PL_PLUTSXPB_PLU_TS_TEMPERATURE_ALERT_TS_ALERT_THRESHOLD(_x) (((_x) & 0xfff) << 0)
#define   NFP3800_PL_PLUTSXPB_PLU_TS_TEMPERATURE_ALERT_TS_ALERT_THRESHOLD_of(_x) (((_x) >> 0) & 0xfff)
/*----------------------------------------------------------------
  Register: PluTSXpb_PluTSRawReadings - Raw Thermal Sensor Readings fed directly from thermal sensor. Readings will be noisy. (NOT RECOMMENDED)
    Bits: 27:16 TSFilterOutput(ro) - 12 bit Noise Filter output. (Value/8)-50 = degC
    Bits: 14    TSotstemperaturetrip(ro) - Temperature catastrophic Trip signal based on the threshold set by TSTripThreshold; Temperature must rise above TSTripThreshold for Trip to assert. Trip will remain asserted until the TSTripThreshold is disabled and re-enabled.
    Bits: 13    TSotstemperaturealrt(ro) - Temperature Alert signal based on the thresholds set by TSAlertThreshold and TSAlertVariation; Temperature must rise above Threshold for Alert to assert; Temperature must fall below TSAlertThreshold-TSAlertVariation for Alert to de-assert.
    Bits: 12    TSotstemperaturevalid(ro) - Temperature Reading has been Updated and is Valid. When this register is read, this bit will be cleared until the next temperature reading arrives.
    Bits: 11:0  TSotstemperature(ro) - 12 bit temperature reading. (Value/8)-50 = degC
*/
#define NFP3800_PL_PluTSXpb_PLU_TS_RAW_READINGS              0x00010010
#define   NFP3800_PL_PLUTSXPB_PLU_TS_RAW_READINGS_TS_FILTER_OUTPUT(_x) (((_x) & 0xfff) << 16)
#define   NFP3800_PL_PLUTSXPB_PLU_TS_RAW_READINGS_TS_FILTER_OUTPUT_of(_x) (((_x) >> 16) & 0xfff)
#define   NFP3800_PL_PLUTSXPB_PLU_TS_RAW_READINGS_TS_TEMPERATURE_TRIP BIT(14)
#define   NFP3800_PL_PLUTSXPB_PLU_TS_RAW_READINGS_TS_TEMPERATURE_ALERT BIT(13)
#define   NFP3800_PL_PLUTSXPB_PLU_TS_RAW_READINGS_TS_TEMPERATURE_VALID BIT(12)
#define   NFP3800_PL_PLUTSXPB_PLU_TS_RAW_READINGS_TS_TEMPERATURE(_x) (((_x) & 0xfff) << 0)
#define   NFP3800_PL_PLUTSXPB_PLU_TS_RAW_READINGS_TS_TEMPERATURE_of(_x) (((_x) >> 0) & 0xfff)
/*----------------------------------------------------------------
  Register: PLL0Controls_PLLControlAndStatus - Primary Control and Status Register
  Register: PLL1Controls_PLLControlAndStatus - Primary Control and Status Register
  Register: PLL2Controls_PLLControlAndStatus - Primary Control and Status Register
  Register: PLL3Controls_PLLControlAndStatus - Primary Control and Status Register
  Register: PLL4Controls_PLLControlAndStatus - Primary Control and Status Register
    Bits: 31    PLLPd(rw) - FSM Control: 1=Normal Power-Up 0=PowerDown. When transitioned to 1, the PLL State Machine will execute an automatic PLL bringup sequence and lockout rights to all but this registers and the Manual Override configuration register.
    Bits: 24    PLLAutoReLock(rw) - Mode bit to select behavior upon the occurence of a Lock Fault
    Bits: 23:8  PLLLockFaultCount(ro) - Once the PLL has been spun-up, counts how many times that lock is lost. This field will be cleared whenever PLLPd transitions from 0 to 1.
    Bits:  7    PLLLockFault(ro) - Indicates that the PLL has lost lock during normal operation. This field will be cleared whenever PLLPd transitions from 0 to 1.
    Bits:  6    PLLLockOut(ro) - PLL Lock Indicator
    Bits:  5    PLLLockDetectTimeOut(ro) - PLL Lock Timeout Indicator
    Bits:  4    PLLStartLockDetect(ro) - FSM Output: Start Lock Detect
    Bits:  1    PLLByPass(ro) - FSM Output: Bypass the PLL, putting REFCLK on PLLOUT
    Bits:  0    PLLWrapOutByPass(ro) - FSM Output: Bypass the PLL with REFCLK outside the PLL
*/
#define NFP3800_PL_PLL0Controls_PLL_CONTROL_AND_STATUS       0x00020000
#define NFP3800_PL_PLL1Controls_PLL_CONTROL_AND_STATUS       0x00030000
#define NFP3800_PL_PLL2Controls_PLL_CONTROL_AND_STATUS       0x00060000
#define NFP3800_PL_PLL3Controls_PLL_CONTROL_AND_STATUS       0x00070000
#define NFP3800_PL_PLL4Controls_PLL_CONTROL_AND_STATUS       0x00080000
#define   NFP3800_PL_PLL0CONTROLS_PLLCONTROLANDSTATUS_PLL_PD BIT(31)
#define   NFP3800_PL_PLL0CONTROLS_PLLCONTROLANDSTATUS_PLL_AUTO_RELOCK BIT(24)
#define     NFP3800_PL_PLL0CONTROLS_PLLCONTROLANDSTATUS_PLL_AUTO_RELOCK_IDLE (0 << 24)
#define     NFP3800_PL_PLL0CONTROLS_PLLCONTROLANDSTATUS_PLL_AUTO_RELOCK_RELOCK BIT(24)
#define   NFP3800_PL_PLL0CONTROLS_PLLCONTROLANDSTATUS_PLL_LOCK_FAULT_COUNT(_x) (((_x) & 0xffff) << 8)
#define   NFP3800_PL_PLL0CONTROLS_PLLCONTROLANDSTATUS_PLL_LOCK_FAULT_COUNT_of(_x) (((_x) >> 8) & 0xffff)
#define   NFP3800_PL_PLL0CONTROLS_PLLCONTROLANDSTATUS_PLL_LOCK_FAULT BIT(7)
#define   NFP3800_PL_PLL0CONTROLS_PLLCONTROLANDSTATUS_PLL_LOCK_OUT BIT(6)
#define   NFP3800_PL_PLL0CONTROLS_PLLCONTROLANDSTATUS_PLL_LOCKDETECT_TIMEOUT BIT(5)
#define   NFP3800_PL_PLL0CONTROLS_PLLCONTROLANDSTATUS_PLL_START_LOCK_DETECT BIT(4)
#define   NFP3800_PL_PLL0CONTROLS_PLLCONTROLANDSTATUS_PLL_SYNTHOUT_BYPASS BIT(1)
#define   NFP3800_PL_PLL0CONTROLS_PLLCONTROLANDSTATUS_PLL_WRAPOUT_BYPASS BIT(0)
/*----------------------------------------------------------------
  Register: PLL0Controls_PLLConfig - Configuration Settings for the PLL
  Register: PLL1Controls_PLLConfig - Configuration Settings for the PLL
  Register: PLL2Controls_PLLConfig - Configuration Settings for the PLL
  Register: PLL3Controls_PLLConfig - Configuration Settings for the PLL
  Register: PLL4Controls_PLLConfig - Configuration Settings for the PLL
    Bits: 31    PLLLockMult(rw) - Write 1 to Set, cannot be cleared without hard_reset. Setting the lock bit captures the current values of the multipliers as limits for all future settings
    Bits: 26:24 PLLRefDiv(rw) - Reference clock input divider ratio: 1-7; 0 also indicates 1. (div_ctrl<6:4> in PLL Spec)
    Bits: 23:16 PLLMDiv(rw) - PLL feedback divider value: 1-255; 0 also indicates 1. (DIV[7:0] in PLL Spec)
    Bits:  4:3  PLLOutDivMult(rw) - Multiplier for Output Divider. This means that the achievable Output Dividers are 1,2,3,4,5,6,7,8,12,16,20,24,28,32,40,48,56. WARNING: Only Output Dividers 1,2,4,8,16,32 produce an ~50% duty cycle, all others should be used with extreme caution! (div_ctrl<1:0> in PLL Spec)
    Bits:  2:0  PLLOutDiv(rw) - Mutiplicand of PLL Output Divider. This value times PLLOutDivMult determines the Output Divider. WARNING: Only Output Dividers 1,2,4,8,16,32 produce an ~50% duty cycle, all others should be used with extreme caution! (OUTDIV<2:0> in PLL Spec)
*/
#define NFP3800_PL_PLL0Controls_PLL_CONFIG                   0x00020004
#define NFP3800_PL_PLL1Controls_PLL_CONFIG                   0x00030004
#define NFP3800_PL_PLL2Controls_PLL_CONFIG                   0x00060004
#define NFP3800_PL_PLL3Controls_PLL_CONFIG                   0x00070004
#define NFP3800_PL_PLL4Controls_PLL_CONFIG                   0x00080004
#define   NFP3800_PL_PLL0CONTROLS_PLLCONFIG_PLL_LOCK_MULT    BIT(31)
#define   NFP3800_PL_PLL0CONTROLS_PLLCONFIG_PLL_REFDIV(_x)   (((_x) & 0x7) << 24)
#define   NFP3800_PL_PLL0CONTROLS_PLLCONFIG_PLL_REFDIV_of(_x) (((_x) >> 24) & 0x7)
#define   NFP3800_PL_PLL0CONTROLS_PLLCONFIG_PLL_MDIV(_x)     (((_x) & 0xff) << 16)
#define   NFP3800_PL_PLL0CONTROLS_PLLCONFIG_PLL_MDIV_of(_x)  (((_x) >> 16) & 0xff)
#define   NFP3800_PL_PLL0CONTROLS_PLLCONFIG_PLL_OUTDIVMULT(_x) (((_x) & 0x3) << 3)
#define   NFP3800_PL_PLL0CONTROLS_PLLCONFIG_PLL_OUTDIVMULT_of(_x) (((_x) >> 3) & 0x3)
#define     NFP3800_PL_PLL0CONTROLS_PLLCONFIG_PLL_OUTDIVMULT_X1 (0)
#define     NFP3800_PL_PLL0CONTROLS_PLLCONFIG_PLL_OUTDIVMULT_X4 (1)
#define     NFP3800_PL_PLL0CONTROLS_PLLCONFIG_PLL_OUTDIVMULT_X8 (2)
#define   NFP3800_PL_PLL0CONTROLS_PLLCONFIG_PLL_OUTDIV(_x)   (((_x) & 0x7) << 0)
#define   NFP3800_PL_PLL0CONTROLS_PLLCONFIG_PLL_OUTDIV_of(_x) (((_x) >> 0) & 0x7)
/*----------------------------------------------------------------
  Register: PLL0Controls_PLLMiscControls - Misc Tuning and Configuration Settings
  Register: PLL1Controls_PLLMiscControls - Misc Tuning and Configuration Settings
  Register: PLL2Controls_PLLMiscControls - Misc Tuning and Configuration Settings
  Register: PLL3Controls_PLLMiscControls - Misc Tuning and Configuration Settings
  Register: PLL4Controls_PLLMiscControls - Misc Tuning and Configuration Settings
    Bits: 31:16 PLLTimeOutCountVal(rw) - If PLLLockOut is not asserted before the time-out counter expires, PLLLockDetectTimeOut is asserted. This value must be higher than 7x PLLLockDetectCount.
    Bits: 15    misc_ctrl_7(rw) - Control Lock Detector. (misc_ctrl<7> in PLL Spec)
    Bits: 14:13 PLLLoopFilter(rw) - Controls Loop Filter. (pll_reg_in<4:3> in PLL Spec)
    Bits: 12:10 PLLChargePump(rw) - Controls Charge Pump. (pll_reg_in<2:0> in PLL Spec)
    Bits:  9:6  PLLTest(rw) - Test control pins to observe VCO, VCTRL, VBGR at TESTPAD. (pll_reg_in<15:12> in PLL Spec)
    Bits:  5:3  PLLVCO(rw) - Controls VCO. (pll_reg_in<7:5> in PLL Spec)
    Bits:  2:0  PLLResCtrl(rw) - Controls Bias Gen (res_ctrl<2:0> in PLL Spec)
*/
#define NFP3800_PL_PLL0Controls_PLL_MISC_CONTROLS            0x0002000c
#define NFP3800_PL_PLL1Controls_PLL_MISC_CONTROLS            0x0003000c
#define NFP3800_PL_PLL2Controls_PLL_MISC_CONTROLS            0x0006000c
#define NFP3800_PL_PLL3Controls_PLL_MISC_CONTROLS            0x0007000c
#define NFP3800_PL_PLL4Controls_PLL_MISC_CONTROLS            0x0008000c
#define   NFP3800_PL_PLL0CONTROLS_PLLMISCCONTROLS_PLL_TIMEOUT_COUNT_VAL(_x) (((_x) & 0xffff) << 16)
#define   NFP3800_PL_PLL0CONTROLS_PLLMISCCONTROLS_PLL_TIMEOUT_COUNT_VAL_of(_x) (((_x) >> 16) & 0xffff)
#define   NFP3800_PL_PLL0CONTROLS_PLLMISCCONTROLS_MISC_CTRL_7 BIT(15)
#define   NFP3800_PL_PLL0CONTROLS_PLLMISCCONTROLS_PLL_LOOPFILTER(_x) (((_x) & 0x3) << 13)
#define   NFP3800_PL_PLL0CONTROLS_PLLMISCCONTROLS_PLL_LOOPFILTER_of(_x) (((_x) >> 13) & 0x3)
#define   NFP3800_PL_PLL0CONTROLS_PLLMISCCONTROLS_PLL_CHARGEPUMP(_x) (((_x) & 0x7) << 10)
#define   NFP3800_PL_PLL0CONTROLS_PLLMISCCONTROLS_PLL_CHARGEPUMP_of(_x) (((_x) >> 10) & 0x7)
#define   NFP3800_PL_PLL0CONTROLS_PLLMISCCONTROLS_PLL_TEST(_x) (((_x) & 0xf) << 6)
#define   NFP3800_PL_PLL0CONTROLS_PLLMISCCONTROLS_PLL_TEST_of(_x) (((_x) >> 6) & 0xf)
#define   NFP3800_PL_PLL0CONTROLS_PLLMISCCONTROLS_PLL_VCO(_x) (((_x) & 0x7) << 3)
#define   NFP3800_PL_PLL0CONTROLS_PLLMISCCONTROLS_PLL_VCO_of(_x) (((_x) >> 3) & 0x7)
#define   NFP3800_PL_PLL0CONTROLS_PLLMISCCONTROLS_PLL_RES_CTRL(_x) (((_x) & 0x7) << 0)
#define   NFP3800_PL_PLL0CONTROLS_PLLMISCCONTROLS_PLL_RES_CTRL_of(_x) (((_x) >> 0) & 0x7)
/*----------------------------------------------------------------
  Register: PLL0Controls_PLLManualOverride - Manual Override Register
  Register: PLL1Controls_PLLManualOverride - Manual Override Register
  Register: PLL2Controls_PLLManualOverride - Manual Override Register
  Register: PLL3Controls_PLLManualOverride - Manual Override Register
  Register: PLL4Controls_PLLManualOverride - Manual Override Register
    Bits: 31    PLLManualOverride(rw) - Setting this bit to 1 enables manual override control for the PLLs. This takes control away from the PLL State Machine. This will also enable writes to the other config regs that would normally be blocked by the PLL State Machine
    Bits: 30    OvdPLLPd(rw) - Manual Override: PLL Power Down Mode: 1=Normal 0=PowerDown
    Bits: 29    OvdPLLLockResetB(rw) - Manual Override: Reset mode: 1=Normal, 0=Reset
    Bits:  2    OvdPLLStartLockDetect(rw) - Manual Override: Signals the start for lock detector operation. If this signal is never asserted, LOCK_OUT will never be asserted.
    Bits:  1    OvdPLLByPass(rw) - Manual Override: Bypass the PLL, putting REFCLK on PLLOUT.
    Bits:  0    OvdPLLWrapOutByPass(rw) - Manual Override: Bypass the PLL with REFCLK outside the PLL
*/
#define NFP3800_PL_PLL0Controls_PLL_MAN_OVERRIDE             0x00020010
#define NFP3800_PL_PLL1Controls_PLL_MAN_OVERRIDE             0x00030010
#define NFP3800_PL_PLL2Controls_PLL_MAN_OVERRIDE             0x00060010
#define NFP3800_PL_PLL3Controls_PLL_MAN_OVERRIDE             0x00070010
#define NFP3800_PL_PLL4Controls_PLL_MAN_OVERRIDE             0x00080010
#define   NFP3800_PL_PLL0CONTROLS_PLLMANUALOVERRIDE_PLL_MANUAL_OVERRIDE BIT(31)
#define   NFP3800_PL_PLL0CONTROLS_PLLMANUALOVERRIDE_OVD_PLL_PD BIT(30)
#define   NFP3800_PL_PLL0CONTROLS_PLLMANUALOVERRIDE_OVD_PLL_LOCK_RESETB BIT(29)
#define   NFP3800_PL_PLL0CONTROLS_PLLMANUALOVERRIDE_OVD_PLL_START_LOCK_DETECT BIT(2)
#define   NFP3800_PL_PLL0CONTROLS_PLLMANUALOVERRIDE_OVD_PLL_BYPASS BIT(1)
#define   NFP3800_PL_PLL0CONTROLS_PLLMANUALOVERRIDE_OVD_PLL_WRAPOUT_BYPASS BIT(0)
/*----------------------------------------------------------------
  Register: PLL0Controls_PLLAuxDivider - Auxilliary Clock Divider
  Register: PLL1Controls_PLLAuxDivider - Auxilliary Clock Divider
  Register: PLL2Controls_PLLAuxDivider - Auxilliary Clock Divider
  Register: PLL3Controls_PLLAuxDivider - Auxilliary Clock Divider
  Register: PLL4Controls_PLLAuxDivider - Auxilliary Clock Divider
    Bits:  7:0  PLLAuxDivider(rw) - Additional Clock Divider Ratio: The auxilliary divider will produce a 50% duty cycle for all divide ratios as long as the PLL output feeding it has a 50% duty cycle. This divider cannot be used with a PLL output frequency faster than TBD MHz.
*/
#define NFP3800_PL_PLL0Controls_PLL_AUX_DIVIDER              0x00020014
#define NFP3800_PL_PLL1Controls_PLL_AUX_DIVIDER              0x00030014
#define NFP3800_PL_PLL2Controls_PLL_AUX_DIVIDER              0x00060014
#define NFP3800_PL_PLL3Controls_PLL_AUX_DIVIDER              0x00070014
#define NFP3800_PL_PLL4Controls_PLL_AUX_DIVIDER              0x00080014
#define   NFP3800_PL_PLL0CONTROLS_PLLAUXDIVIDER_PLL_AUX_DIVIDER(_x) (((_x) & 0xff) << 0)
#define   NFP3800_PL_PLL0CONTROLS_PLLAUXDIVIDER_PLL_AUX_DIVIDER_of(_x) (((_x) >> 0) & 0xff)
#define     NFP3800_PL_PLL0CONTROLS_PLLAUXDIVIDER_PLL_AUX_DIVIDER_BYP (0)
/*----------------------------------------------------------------
  Register: PLL0Controls_PLLMiscControls2 - Misc Tuning and Configuration Settings; all the rest of the PLL pins.
  Register: PLL1Controls_PLLMiscControls2 - Misc Tuning and Configuration Settings; all the rest of the PLL pins.
  Register: PLL2Controls_PLLMiscControls2 - Misc Tuning and Configuration Settings; all the rest of the PLL pins.
  Register: PLL3Controls_PLLMiscControls2 - Misc Tuning and Configuration Settings; all the rest of the PLL pins.
  Register: PLL4Controls_PLLMiscControls2 - Misc Tuning and Configuration Settings; all the rest of the PLL pins.
    Bits: 24:23 pll_reg_in_9to8(rw) - (pll_reg_in<9:8> in PLL Spec)
    Bits: 22:17 pll_reg_in_22to17(rw) - (pll_reg_in<22:17> in PLL Spec)
    Bits: 16:15 unused_REFDIV(rw) - (REFDIV<1:0> in PLL Spec)
    Bits: 14    unused_div_ctrl_7(rw) - (div_ctrl<7> in PLL Spec)
    Bits: 13:12 unused_div_ctrl_3to2(rw) - (div_ctrl<3:2> in PLL Spec)
    Bits: 11    unused_RESETB(rw) - (RESETB in PLL Spec)
    Bits: 10    unused_Div_8(rw) - (Div<8> in PLL Spec)
    Bits:  9:3  unused_misc_ctrl(rw) - (misc_ctrl<6:0> in PLL Spec)
    Bits:  2:1  unused_pll_reg_in_11to10(rw) - (pll_reg_in<11:10> in PLL Spec)
    Bits:  0    unused_pll_reg_in_23(rw) - (pll_reg_in<23> in PLL Spec)
*/
#define NFP3800_PL_PLL0Controls_PLL_MISC_CONTROLS2           0x00020018
#define NFP3800_PL_PLL1Controls_PLL_MISC_CONTROLS2           0x00030018
#define NFP3800_PL_PLL2Controls_PLL_MISC_CONTROLS2           0x00060018
#define NFP3800_PL_PLL3Controls_PLL_MISC_CONTROLS2           0x00070018
#define NFP3800_PL_PLL4Controls_PLL_MISC_CONTROLS2           0x00080018
#define   NFP3800_PL_PLL0CONTROLS_PLLMISCCONTROLS2_PLL_REG_IN_9TO8(_x) (((_x) & 0x3) << 23)
#define   NFP3800_PL_PLL0CONTROLS_PLLMISCCONTROLS2_PLL_REG_IN_9TO8_of(_x) (((_x) >> 23) & 0x3)
#define   NFP3800_PL_PLL0CONTROLS_PLLMISCCONTROLS2_PLL_REG_IN_22TO17(_x) (((_x) & 0x3f) << 17)
#define   NFP3800_PL_PLL0CONTROLS_PLLMISCCONTROLS2_PLL_REG_IN_22TO17_of(_x) (((_x) >> 17) & 0x3f)
#define   NFP3800_PL_PLL0CONTROLS_PLLMISCCONTROLS2_UNUSED_REFDIV(_x) (((_x) & 0x3) << 15)
#define   NFP3800_PL_PLL0CONTROLS_PLLMISCCONTROLS2_UNUSED_REFDIV_of(_x) (((_x) >> 15) & 0x3)
#define   NFP3800_PL_PLL0CONTROLS_PLLMISCCONTROLS2_UNUSED_DIV_CTRL_7 BIT(14)
#define   NFP3800_PL_PLL0CONTROLS_PLLMISCCONTROLS2_UNUSED_DIV_CTRL_3TO2(_x) (((_x) & 0x3) << 12)
#define   NFP3800_PL_PLL0CONTROLS_PLLMISCCONTROLS2_UNUSED_DIV_CTRL_3TO2_of(_x) (((_x) >> 12) & 0x3)
#define   NFP3800_PL_PLL0CONTROLS_PLLMISCCONTROLS2_UNUSED_RESETB BIT(11)
#define   NFP3800_PL_PLL0CONTROLS_PLLMISCCONTROLS2_UNUSED_DIV_8 BIT(10)
#define   NFP3800_PL_PLL0CONTROLS_PLLMISCCONTROLS2_UNUSED_MISC_CTRL(_x) (((_x) & 0x7f) << 3)
#define   NFP3800_PL_PLL0CONTROLS_PLLMISCCONTROLS2_UNUSED_MISC_CTRL_of(_x) (((_x) >> 3) & 0x7f)
#define   NFP3800_PL_PLL0CONTROLS_PLLMISCCONTROLS2_UNUSED_PLL_REG_IN_11TO10(_x) (((_x) & 0x3) << 1)
#define   NFP3800_PL_PLL0CONTROLS_PLLMISCCONTROLS2_UNUSED_PLL_REG_IN_11TO10_of(_x) (((_x) >> 1) & 0x3)
#define   NFP3800_PL_PLL0CONTROLS_PLLMISCCONTROLS2_UNUSED_PLL_REG_IN_23 BIT(0)
/*----------------------------------------------------------------
  Register: PluIOControls_PluLvcmos0IOCtrl - LVCMOS0 IO Control
    Bits: 27    clk_nreset_out_l_rcven(rw) - CLK_NRESET_OUT_L pin buffer input enable.
    Bits: 26    clk_nreset_out_l_oenb(rw) - CLK_NRESET_OUT_L pin output enable, active low.
    Bits: 25:24 clk_nreset_out_l_drv(rw) - CLK_NRESET_OUT_L pin drive strength setting.
    Bits: 23    pwrgood_core_wkpuen(rw) - RESERVED
    Bits: 22    pwrgood_core_wkpden(rw) - RESERVED
    Bits: 21    pwrgoodio_wkpuen(rw) - RESERVED
    Bits: 20    pwrgoodio_wkpden(rw) - RESERVED
    Bits: 19    sreset_l_wkpuen(rw) - RESERVED
    Bits: 18    sreset_l_wkpden(rw) - RESERVED
    Bits: 17    clk_nreset_l_wkpuen(rw) - RESERVED
    Bits: 16    clk_nreset_l_wkpden(rw) - RESERVED
    Bits: 11    clk_ref_out_rcven(rw) - Buffer input enable for CLK_REF_OUT pin.
    Bits: 10    clk_ref_out_oenb(rw) - CLK_REF_OUT pin output enable, active low.
    Bits:  9:8  clk_ref_out_drv(rw) - CLK_REF_OUT pin drive strength setting.
    Bits:  6    clk_ref_in_rcven(rw) - CLK_REF_IN pin buffer input enable.
    Bits:  5    clk_xtal_bypass(rw) - When set to 1, puts CLK_XTAL pad in bypass.
    Bits:  4    clk_xtal_gmsel_override(rw) - When set to 1, override GMSEL with the CLK_XTAL_GMSEL field in this register.
    Bits:  3:0  clk_xtal_gmsel(rw) - Gain control setting for crystal oscillator. Default 4'b1000
*/
#define NFP3800_PL_PluIOControls_PLU_LVCMOS0_IO_CTRL         0x00040000
#define   NFP3800_PL_PLUIOCONTROLS_PLU_LVCMOS0_IO_CTRL_CLK_NRESET_OUT_L_RCVEN BIT(27)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_LVCMOS0_IO_CTRL_CLK_NRESET_OUT_L_OENB BIT(26)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_LVCMOS0_IO_CTRL_CLK_NRESET_OUT_L_DRV(_x) (((_x) & 0x3) << 24)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_LVCMOS0_IO_CTRL_CLK_NRESET_OUT_L_DRV_of(_x) (((_x) >> 24) & 0x3)
#define     NFP3800_PL_PLUIOCONTROLS_PLU_LVCMOS0_IO_CTRL_CLK_NRESET_OUT_L_DRV_1MA (0)
#define     NFP3800_PL_PLUIOCONTROLS_PLU_LVCMOS0_IO_CTRL_CLK_NRESET_OUT_L_DRV_2MA (1)
#define     NFP3800_PL_PLUIOCONTROLS_PLU_LVCMOS0_IO_CTRL_CLK_NRESET_OUT_L_DRV_4MA (2)
#define     NFP3800_PL_PLUIOCONTROLS_PLU_LVCMOS0_IO_CTRL_CLK_NRESET_OUT_L_DRV_8MA (3)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_LVCMOS0_IO_CTRL_PWRGOOD_CORE_WKPUEN BIT(23)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_LVCMOS0_IO_CTRL_PWRGOOD_CORE_WKPDEN BIT(22)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_LVCMOS0_IO_CTRL_PWRGOODIO_WKPUEN BIT(21)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_LVCMOS0_IO_CTRL_PWRGOODIO_WKPDEN BIT(20)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_LVCMOS0_IO_CTRL_SRESET_L_WKPUEN BIT(19)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_LVCMOS0_IO_CTRL_SRESET_L_WKPDEN BIT(18)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_LVCMOS0_IO_CTRL_CLK_NRESET_L_WKPUEN BIT(17)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_LVCMOS0_IO_CTRL_CLK_NRESET_L_WKPDEN BIT(16)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_LVCMOS0_IO_CTRL_CLK_REF_OUT_RCVEN BIT(11)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_LVCMOS0_IO_CTRL_CLK_REF_OUT_OENB BIT(10)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_LVCMOS0_IO_CTRL_CLK_REF_OUT_DRV(_x) (((_x) & 0x3) << 8)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_LVCMOS0_IO_CTRL_CLK_REF_OUT_DRV_of(_x) (((_x) >> 8) & 0x3)
#define     NFP3800_PL_PLUIOCONTROLS_PLU_LVCMOS0_IO_CTRL_CLK_REF_OUT_DRV_1MA (0)
#define     NFP3800_PL_PLUIOCONTROLS_PLU_LVCMOS0_IO_CTRL_CLK_REF_OUT_DRV_2MA (1)
#define     NFP3800_PL_PLUIOCONTROLS_PLU_LVCMOS0_IO_CTRL_CLK_REF_OUT_DRV_4MA (2)
#define     NFP3800_PL_PLUIOCONTROLS_PLU_LVCMOS0_IO_CTRL_CLK_REF_OUT_DRV_8MA (3)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_LVCMOS0_IO_CTRL_CLK_REF_IN_RCVEN BIT(6)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_LVCMOS0_IO_CTRL_CLK_XTAL_BYPASS BIT(5)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_LVCMOS0_IO_CTRL_CLK_XTAL_GMSEL_OVERRIDE BIT(4)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_LVCMOS0_IO_CTRL_CLK_XTAL_GMSEL(_x) (((_x) & 0xf) << 0)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_LVCMOS0_IO_CTRL_CLK_XTAL_GMSEL_of(_x) (((_x) >> 0) & 0xf)
/*----------------------------------------------------------------
  Register: PluIOControls_GpioB0Drive - GPIO driver controls for GPIO[15:0].
    Bits: 31:30 gpio_15_drv(rw) - Output drive strength setting of GPIO[15].
    Bits: 29:28 gpio_14_drv(rw) - Output drive strength setting of GPIO[14].
    Bits: 27:26 gpio_13_drv(rw) - Output drive strength setting of GPIO[13].
    Bits: 25:24 gpio_12_drv(rw) - Output drive strength setting of GPIO[12].
    Bits: 23:22 gpio_11_drv(rw) - Output drive strength setting of GPIO[11].
    Bits: 21:20 gpio_10_drv(rw) - Output drive strength setting of GPIO[10].
    Bits: 19:18 gpio_9_drv(rw) - Output drive strength setting of GPIO[9].
    Bits: 17:16 gpio_8_drv(rw) - Output drive strength setting of GPIO[8].
    Bits: 15:14 gpio_7_drv(rw) - Output drive strength setting of GPIO[7].
    Bits: 13:12 gpio_6_drv(rw) - Output drive strength setting of GPIO[6].
    Bits: 11:10 gpio_5_drv(rw) - Output drive strength setting of GPIO[5].
    Bits:  9:8  gpio_4_drv(rw) - Output drive strength setting of GPIO[4].
    Bits:  7:6  gpio_3_drv(rw) - Output drive strength setting of GPIO[3].
    Bits:  5:4  gpio_2_drv(rw) - Output drive strength setting of GPIO[2].
    Bits:  3:2  gpio_1_drv(rw) - Output drive strength setting of GPIO[1].
    Bits:  1:0  gpio_0_drv(rw) - Output drive strength setting of GPIO[0].
*/
#define NFP3800_PL_PluIOControls_PLU_GPIO_B0_DRIVE           0x00040004
#define   NFP3800_PL_PLUIOCONTROLS_PLU_GPIO_B0_DRIVE_GPIO_15_DRV(_x) (((_x) & 0x3) << 30)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_GPIO_B0_DRIVE_GPIO_15_DRV_of(_x) (((_x) >> 30) & 0x3)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_GPIO_B0_DRIVE_GPIO_14_DRV(_x) (((_x) & 0x3) << 28)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_GPIO_B0_DRIVE_GPIO_14_DRV_of(_x) (((_x) >> 28) & 0x3)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_GPIO_B0_DRIVE_GPIO_13_DRV(_x) (((_x) & 0x3) << 26)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_GPIO_B0_DRIVE_GPIO_13_DRV_of(_x) (((_x) >> 26) & 0x3)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_GPIO_B0_DRIVE_GPIO_12_DRV(_x) (((_x) & 0x3) << 24)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_GPIO_B0_DRIVE_GPIO_12_DRV_of(_x) (((_x) >> 24) & 0x3)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_GPIO_B0_DRIVE_GPIO_11_DRV(_x) (((_x) & 0x3) << 22)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_GPIO_B0_DRIVE_GPIO_11_DRV_of(_x) (((_x) >> 22) & 0x3)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_GPIO_B0_DRIVE_GPIO_10_DRV(_x) (((_x) & 0x3) << 20)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_GPIO_B0_DRIVE_GPIO_10_DRV_of(_x) (((_x) >> 20) & 0x3)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_GPIO_B0_DRIVE_GPIO_9_DRV(_x) (((_x) & 0x3) << 18)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_GPIO_B0_DRIVE_GPIO_9_DRV_of(_x) (((_x) >> 18) & 0x3)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_GPIO_B0_DRIVE_GPIO_8_DRV(_x) (((_x) & 0x3) << 16)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_GPIO_B0_DRIVE_GPIO_8_DRV_of(_x) (((_x) >> 16) & 0x3)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_GPIO_B0_DRIVE_GPIO_7_DRV(_x) (((_x) & 0x3) << 14)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_GPIO_B0_DRIVE_GPIO_7_DRV_of(_x) (((_x) >> 14) & 0x3)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_GPIO_B0_DRIVE_GPIO_6_DRV(_x) (((_x) & 0x3) << 12)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_GPIO_B0_DRIVE_GPIO_6_DRV_of(_x) (((_x) >> 12) & 0x3)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_GPIO_B0_DRIVE_GPIO_5_DRV(_x) (((_x) & 0x3) << 10)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_GPIO_B0_DRIVE_GPIO_5_DRV_of(_x) (((_x) >> 10) & 0x3)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_GPIO_B0_DRIVE_GPIO_4_DRV(_x) (((_x) & 0x3) << 8)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_GPIO_B0_DRIVE_GPIO_4_DRV_of(_x) (((_x) >> 8) & 0x3)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_GPIO_B0_DRIVE_GPIO_3_DRV(_x) (((_x) & 0x3) << 6)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_GPIO_B0_DRIVE_GPIO_3_DRV_of(_x) (((_x) >> 6) & 0x3)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_GPIO_B0_DRIVE_GPIO_2_DRV(_x) (((_x) & 0x3) << 4)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_GPIO_B0_DRIVE_GPIO_2_DRV_of(_x) (((_x) >> 4) & 0x3)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_GPIO_B0_DRIVE_GPIO_1_DRV(_x) (((_x) & 0x3) << 2)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_GPIO_B0_DRIVE_GPIO_1_DRV_of(_x) (((_x) >> 2) & 0x3)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_GPIO_B0_DRIVE_GPIO_0_DRV(_x) (((_x) & 0x3) << 0)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_GPIO_B0_DRIVE_GPIO_0_DRV_of(_x) (((_x) >> 0) & 0x3)
/*----------------------------------------------------------------
  Register: PluIOControls_GpioB1Drive - GPIO driver controls for GPIO[31:16].
    Bits: 31:30 gpio_31_drv(rw) - Output drive strength setting of GPIO[31].
    Bits: 29:28 gpio_30_drv(rw) - Output drive strength setting of GPIO[30].
    Bits: 27:26 gpio_29_drv(rw) - Output drive strength setting of GPIO[29].
    Bits: 25:24 gpio_28_drv(rw) - Output drive strength setting of GPIO[28].
    Bits: 23:22 gpio_27_drv(rw) - Output drive strength setting of GPIO[27].
    Bits: 21:20 gpio_26_drv(rw) - Output drive strength setting of GPIO[26].
    Bits: 19:18 gpio_25_drv(rw) - Output drive strength setting of GPIO[25].
    Bits: 17:16 gpio_24_drv(rw) - Output drive strength setting of GPIO[24].
    Bits: 15:14 gpio_23_drv(rw) - Output drive strength setting of GPIO[23].
    Bits: 13:12 gpio_22_drv(rw) - Output drive strength setting of GPIO[22].
    Bits: 11:10 gpio_21_drv(rw) - Output drive strength setting of GPIO[21].
    Bits:  9:8  gpio_20_drv(rw) - Output drive strength setting of GPIO[20].
    Bits:  7:6  gpio_19_drv(rw) - Output drive strength setting of GPIO[19].
    Bits:  5:4  gpio_18_drv(rw) - Output drive strength setting of GPIO[18].
    Bits:  3:2  gpio_17_drv(rw) - Output drive strength setting of GPIO[17].
    Bits:  1:0  gpio_16_drv(rw) - Output drive strength setting of GPIO[16].
*/
#define NFP3800_PL_PluIOControls_PLU_GPIO_B1_DRIVE           0x00040008
#define   NFP3800_PL_PLUIOCONTROLS_PLU_GPIO_B1_DRIVE_GPIO_31_DRV(_x) (((_x) & 0x3) << 30)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_GPIO_B1_DRIVE_GPIO_31_DRV_of(_x) (((_x) >> 30) & 0x3)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_GPIO_B1_DRIVE_GPIO_30_DRV(_x) (((_x) & 0x3) << 28)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_GPIO_B1_DRIVE_GPIO_30_DRV_of(_x) (((_x) >> 28) & 0x3)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_GPIO_B1_DRIVE_GPIO_29_DRV(_x) (((_x) & 0x3) << 26)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_GPIO_B1_DRIVE_GPIO_29_DRV_of(_x) (((_x) >> 26) & 0x3)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_GPIO_B1_DRIVE_GPIO_28_DRV(_x) (((_x) & 0x3) << 24)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_GPIO_B1_DRIVE_GPIO_28_DRV_of(_x) (((_x) >> 24) & 0x3)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_GPIO_B1_DRIVE_GPIO_27_DRV(_x) (((_x) & 0x3) << 22)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_GPIO_B1_DRIVE_GPIO_27_DRV_of(_x) (((_x) >> 22) & 0x3)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_GPIO_B1_DRIVE_GPIO_26_DRV(_x) (((_x) & 0x3) << 20)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_GPIO_B1_DRIVE_GPIO_26_DRV_of(_x) (((_x) >> 20) & 0x3)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_GPIO_B1_DRIVE_GPIO_25_DRV(_x) (((_x) & 0x3) << 18)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_GPIO_B1_DRIVE_GPIO_25_DRV_of(_x) (((_x) >> 18) & 0x3)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_GPIO_B1_DRIVE_GPIO_24_DRV(_x) (((_x) & 0x3) << 16)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_GPIO_B1_DRIVE_GPIO_24_DRV_of(_x) (((_x) >> 16) & 0x3)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_GPIO_B1_DRIVE_GPIO_23_DRV(_x) (((_x) & 0x3) << 14)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_GPIO_B1_DRIVE_GPIO_23_DRV_of(_x) (((_x) >> 14) & 0x3)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_GPIO_B1_DRIVE_GPIO_22_DRV(_x) (((_x) & 0x3) << 12)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_GPIO_B1_DRIVE_GPIO_22_DRV_of(_x) (((_x) >> 12) & 0x3)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_GPIO_B1_DRIVE_GPIO_21_DRV(_x) (((_x) & 0x3) << 10)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_GPIO_B1_DRIVE_GPIO_21_DRV_of(_x) (((_x) >> 10) & 0x3)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_GPIO_B1_DRIVE_GPIO_20_DRV(_x) (((_x) & 0x3) << 8)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_GPIO_B1_DRIVE_GPIO_20_DRV_of(_x) (((_x) >> 8) & 0x3)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_GPIO_B1_DRIVE_GPIO_19_DRV(_x) (((_x) & 0x3) << 6)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_GPIO_B1_DRIVE_GPIO_19_DRV_of(_x) (((_x) >> 6) & 0x3)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_GPIO_B1_DRIVE_GPIO_18_DRV(_x) (((_x) & 0x3) << 4)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_GPIO_B1_DRIVE_GPIO_18_DRV_of(_x) (((_x) >> 4) & 0x3)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_GPIO_B1_DRIVE_GPIO_17_DRV(_x) (((_x) & 0x3) << 2)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_GPIO_B1_DRIVE_GPIO_17_DRV_of(_x) (((_x) >> 2) & 0x3)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_GPIO_B1_DRIVE_GPIO_16_DRV(_x) (((_x) & 0x3) << 0)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_GPIO_B1_DRIVE_GPIO_16_DRV_of(_x) (((_x) >> 0) & 0x3)
/*----------------------------------------------------------------
  Register: PluIOControls_SpiDrive - SPI driver controls.
    Bits: 27:26 spix_d3_drv(rw) - Output drive strength setting of SPIX_D3.
    Bits: 25:24 spix_d2_drv(rw) - Output drive strength setting of SPIX_D2.
    Bits: 23:22 spi3_mosi_drv(rw) - Output drive strength setting of SPI3_MOSI.
    Bits: 21:20 spi3_sck_drv(rw) - Output drive strength setting of SPI3_SCK.
    Bits: 19:18 spi2_mosi_drv(rw) - Output drive strength setting of SPI2_MOSI.
    Bits: 17:16 spi2_sck_drv(rw) - Output drive strength setting of SPI2_SCK.
    Bits: 15:14 spi1_cfg_mosi_drv(rw) - Output drive strength setting of SPI1_CFG_MOSI.
    Bits: 13:12 spi1_cfg_miso_drv(rw) - Output drive strength setting of SPI1_CFG_MISO.
    Bits: 11:10 spi1_cfg_sel_drv(rw) - Output drive strength setting of SPI1_CFG_SEL.
    Bits:  9:8  spi1_cfg_sck_drv(rw) - Output drive strength setting of SPI1_CFG_SCK.
    Bits:  7:6  spi0_mosi_drv(rw) - Output drive strength setting of SPI0_MOSI.
    Bits:  5:4  spi0_miso_drv(rw) - Output drive strength setting of SPI0_MISO.
    Bits:  3:2  spi0_sel_drv(rw) - Output drive strength setting of SPI0_SEL.
    Bits:  1:0  spi0_sck_drv(rw) - Output drive strength setting of SPI0_SCK.
*/
#define NFP3800_PL_PluIOControls_PLU_SPI_DRIVE               0x0004000c
#define   NFP3800_PL_PLUIOCONTROLS_PLU_SPI_DRIVE_SPIX_D3_DRV(_x) (((_x) & 0x3) << 26)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_SPI_DRIVE_SPIX_D3_DRV_of(_x) (((_x) >> 26) & 0x3)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_SPI_DRIVE_SPIX_D2_DRV(_x) (((_x) & 0x3) << 24)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_SPI_DRIVE_SPIX_D2_DRV_of(_x) (((_x) >> 24) & 0x3)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_SPI_DRIVE_SPI3_MOSI_DRV(_x) (((_x) & 0x3) << 22)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_SPI_DRIVE_SPI3_MOSI_DRV_of(_x) (((_x) >> 22) & 0x3)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_SPI_DRIVE_SPI3_SCK_DRV(_x) (((_x) & 0x3) << 20)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_SPI_DRIVE_SPI3_SCK_DRV_of(_x) (((_x) >> 20) & 0x3)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_SPI_DRIVE_SPI2_MOSI_DRV(_x) (((_x) & 0x3) << 18)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_SPI_DRIVE_SPI2_MOSI_DRV_of(_x) (((_x) >> 18) & 0x3)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_SPI_DRIVE_SPI2_SCK_DRV(_x) (((_x) & 0x3) << 16)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_SPI_DRIVE_SPI2_SCK_DRV_of(_x) (((_x) >> 16) & 0x3)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_SPI_DRIVE_SPI1_CFG_MOSI_DRV(_x) (((_x) & 0x3) << 14)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_SPI_DRIVE_SPI1_CFG_MOSI_DRV_of(_x) (((_x) >> 14) & 0x3)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_SPI_DRIVE_SPI1_CFG_MISO_DRV(_x) (((_x) & 0x3) << 12)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_SPI_DRIVE_SPI1_CFG_MISO_DRV_of(_x) (((_x) >> 12) & 0x3)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_SPI_DRIVE_SPI1_CFG_SEL_DRV(_x) (((_x) & 0x3) << 10)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_SPI_DRIVE_SPI1_CFG_SEL_DRV_of(_x) (((_x) >> 10) & 0x3)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_SPI_DRIVE_SPI1_CFG_SCK_DRV(_x) (((_x) & 0x3) << 8)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_SPI_DRIVE_SPI1_CFG_SCK_DRV_of(_x) (((_x) >> 8) & 0x3)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_SPI_DRIVE_SPI0_MOSI_DRV(_x) (((_x) & 0x3) << 6)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_SPI_DRIVE_SPI0_MOSI_DRV_of(_x) (((_x) >> 6) & 0x3)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_SPI_DRIVE_SPI0_MISO_DRV(_x) (((_x) & 0x3) << 4)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_SPI_DRIVE_SPI0_MISO_DRV_of(_x) (((_x) >> 4) & 0x3)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_SPI_DRIVE_SPI0_SEL_DRV(_x) (((_x) & 0x3) << 2)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_SPI_DRIVE_SPI0_SEL_DRV_of(_x) (((_x) >> 2) & 0x3)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_SPI_DRIVE_SPI0_SCK_DRV(_x) (((_x) & 0x3) << 0)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_SPI_DRIVE_SPI0_SCK_DRV_of(_x) (((_x) >> 0) & 0x3)
/*----------------------------------------------------------------
  Register: PluIOControls_MiscDrive - Misc. driver controls.
    Bits: 21:20 jtag_tdo_drv(rw) - Output drive strength setting of JTAG_TDO.
    Bits: 19:18 pcie_reset_out_l_drv(rw) - Output drive strength setting of PCIE_RESET_OUT_L.
    Bits: 17:16 smbalrt_drv(rw) - Output drive strength setting of SMBALRT.
    Bits: 15:14 smbdat_drv(rw) - Output drive strength setting of SMBDAT.
    Bits: 13:12 smbclk_drv(rw) - Output drive strength setting of SMBCLK.
    Bits: 11:10 sr0_rx_drv(rw) - Output drive strength setting of SR0_RX.
    Bits:  9:8  sr0_tx_drv(rw) - Output drive strength setting of SR0_TX.
    Bits:  7:6  sda1_drv(rw) - Output drive strength setting of SDA1.
    Bits:  5:4  scl1_drv(rw) - Output drive strength setting of SCL1.
    Bits:  3:2  sda0_drv(rw) - Output drive strength setting of SDA0.
    Bits:  1:0  scl0_drv(rw) - Output drive strength setting of SCL0.
*/
#define NFP3800_PL_PluIOControls_PLU_MISC_DRIVE              0x00040010
#define   NFP3800_PL_PLUIOCONTROLS_PLU_MISC_DRIVE_JTAG_TDO_DRV(_x) (((_x) & 0x3) << 20)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_MISC_DRIVE_JTAG_TDO_DRV_of(_x) (((_x) >> 20) & 0x3)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_MISC_DRIVE_PCIE_RESET_OUT_L_DRV(_x) (((_x) & 0x3) << 18)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_MISC_DRIVE_PCIE_RESET_OUT_L_DRV_of(_x) (((_x) >> 18) & 0x3)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_MISC_DRIVE_SMBALRT_DRV(_x) (((_x) & 0x3) << 16)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_MISC_DRIVE_SMBALRT_DRV_of(_x) (((_x) >> 16) & 0x3)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_MISC_DRIVE_SMBDAT_DRV(_x) (((_x) & 0x3) << 14)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_MISC_DRIVE_SMBDAT_DRV_of(_x) (((_x) >> 14) & 0x3)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_MISC_DRIVE_SMBCLK_DRV(_x) (((_x) & 0x3) << 12)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_MISC_DRIVE_SMBCLK_DRV_of(_x) (((_x) >> 12) & 0x3)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_MISC_DRIVE_SR0_RX_DRV(_x) (((_x) & 0x3) << 10)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_MISC_DRIVE_SR0_RX_DRV_of(_x) (((_x) >> 10) & 0x3)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_MISC_DRIVE_SR0_TX_DRV(_x) (((_x) & 0x3) << 8)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_MISC_DRIVE_SR0_TX_DRV_of(_x) (((_x) >> 8) & 0x3)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_MISC_DRIVE_SDA1_DRV(_x) (((_x) & 0x3) << 6)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_MISC_DRIVE_SDA1_DRV_of(_x) (((_x) >> 6) & 0x3)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_MISC_DRIVE_SCL1_DRV(_x) (((_x) & 0x3) << 4)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_MISC_DRIVE_SCL1_DRV_of(_x) (((_x) >> 4) & 0x3)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_MISC_DRIVE_SDA0_DRV(_x) (((_x) & 0x3) << 2)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_MISC_DRIVE_SDA0_DRV_of(_x) (((_x) >> 2) & 0x3)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_MISC_DRIVE_SCL0_DRV(_x) (((_x) & 0x3) << 0)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_MISC_DRIVE_SCL0_DRV_of(_x) (((_x) >> 0) & 0x3)
/*----------------------------------------------------------------
  Register: PluIOControls_GpioWkpullupEn - Weak pull up for GPIO.
    Bits: 31:0  gpio_wkpuen(rw) - Weak pull up enable for GPIO[31:0] (1: weak pull up enabled, 0: weak pull up disabled).
*/
#define NFP3800_PL_PluIOControls_PLU_GPIO_WKPULLUP_EN        0x00040014
#define   NFP3800_PL_PLUIOCONTROLS_PLU_GPIO_WKPULLUP_EN_GPIO_WKPUEN(_x) (((_x) & 0xffffffff) << 0)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_GPIO_WKPULLUP_EN_GPIO_WKPUEN_of(_x) (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: PluIOControls_GpioWkpulldnEn - Weak pull down for GPIO.
    Bits: 31:0  gpio_wkpden(rw) - Weak pull down enable for GPIO[31:0] (1: weak pull down enabled, 0: weak pull down disabled).
*/
#define NFP3800_PL_PluIOControls_PLU_GPIO_WKPULLDN_EN        0x00040018
#define   NFP3800_PL_PLUIOCONTROLS_PLU_GPIO_WKPULLDN_EN_GPIO_WKPDEN(_x) (((_x) & 0xffffffff) << 0)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_GPIO_WKPULLDN_EN_GPIO_WKPDEN_of(_x) (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: PluIOControls_SpiWkpullEn - Weak pull up and pull down for SPI pins.
    Bits: 29    spix_d3_wkpden(rw) - Weak pull down enable for SPIX_D3 (1: weak pull down enabled, 0: weak pull down disabled).
    Bits: 28    spix_d2_wkpden(rw) - Weak pull down enable for SPIX_D2 (1: weak pull down enabled, 0: weak pull down disabled).
    Bits: 27    spi3_mosi_wkpden(rw) - Weak pull down enable for SPI3_MOSI (1: weak pull down enabled, 0: weak pull down disabled).
    Bits: 26    spi3_sck_wkpden(rw) - Weak pull down enable for SPI3_SCK (1: weak pull down enabled, 0: weak pull down disabled).
    Bits: 25    spi2_mosi_wkpden(rw) - Weak pull down enable for SPI2_MOSI (1: weak pull down enabled, 0: weak pull down disabled).
    Bits: 24    spi2_sck_wkpden(rw) - Weak pull down enable for SPI2_SCK (1: weak pull down enabled, 0: weak pull down disabled).
    Bits: 23    spi1_cfg_miso_wkpden(rw) - Weak pull down enable for SPI1_CFG_MISO (1: weak pull down enabled, 0: weak pull down disabled).
    Bits: 22    spi1_cfg_mosi_wkpden(rw) - Weak pull down enable for SPI1_CFG_MOSI (1: weak pull down enabled, 0: weak pull down disabled).
    Bits: 21    spi1_cfg_sel_wkpden(rw) - Weak pull down enable for SPI1_CFG_SEL (1: weak pull down enabled, 0: weak pull down disabled).
    Bits: 20    spi1_cfg_sck_wkpden(rw) - Weak pull down enable for SPI1_CFG_SCK (1: weak pull down enabled, 0: weak pull down disabled).
    Bits: 19    spi0_miso_wkpden(rw) - Weak pull down enable for SPI0_MISO (1: weak pull down enabled, 0: weak pull down disabled).
    Bits: 18    spi0_mosi_wkpden(rw) - Weak pull down enable for SPI0_MOSI (1: weak pull down enabled, 0: weak pull down disabled).
    Bits: 17    spi0_sel_wkpden(rw) - Weak pull down enable for SPI0_SEL (1: weak pull down enabled, 0: weak pull down disabled).
    Bits: 16    spi0_sck_wkpden(rw) - Weak pull down enable for SPI0_SCK (1: weak pull down enabled, 0: weak pull down disabled).
    Bits: 13    spix_d3_wkpuen(rw) - Weak pull up enable for SPIX_D3 (1: weak pull up enabled, 0: weak pull up disabled).
    Bits: 12    spix_d2_wkpuen(rw) - Weak pull up enable for SPIX_D2 (1: weak pull up enabled, 0: weak pull up disabled).
    Bits: 11    spi3_mosi_wkpuen(rw) - Weak pull up enable for SPI3_MOSI (1: weak pull up enabled, 0: weak pull up disabled).
    Bits: 10    spi3_sck_wkpuen(rw) - Weak pull up enable for SPI3_SCK (1: weak pull up enabled, 0: weak pull up disabled).
    Bits:  9    spi2_mosi_wkpuen(rw) - Weak pull up enable for SPI2_MOSI (1: weak pull up enabled, 0: weak pull up disabled).
    Bits:  8    spi2_sck_wkpuen(rw) - Weak pull up enable for SPI2_SCK (1: weak pull up enabled, 0: weak pull up disabled).
    Bits:  7    spi1_cfg_miso_wkpuen(rw) - Weak pull up enable for SPI1_CFG_MISO (1: weak pull up enabled, 0: weak pull up disabled).
    Bits:  6    spi1_cfg_mosi_wkpuen(rw) - Weak pull up enable for SPI1_CFG_MOSI (1: weak pull up enabled, 0: weak pull up disabled).
    Bits:  5    spi1_cfg_sel_wkpuen(rw) - Weak pull up enable for SPI1_CFG_SEL (1: weak pull up enabled, 0: weak pull up disabled).
    Bits:  4    spi1_cfg_sck_wkpuen(rw) - Weak pull up enable for SPI1_CFG_SCK (1: weak pull up enabled, 0: weak pull up disabled).
    Bits:  3    spi0_miso_wkpuen(rw) - Weak pull up enable for SPI0_MISO (1: weak pull up enabled, 0: weak pull up disabled).
    Bits:  2    spi0_mosi_wkpuen(rw) - Weak pull up enable for SPI0_MOSI (1: weak pull up enabled, 0: weak pull up disabled).
    Bits:  1    spi0_sel_wkpuen(rw) - Weak pull up enable for SPI0_SEL (1: weak pull up enabled, 0: weak pull up disabled).
    Bits:  0    spi0_sck_wkpuen(rw) - Weak pull up enable for SPI0_SCK (1: weak pull up enabled, 0: weak pull up disabled).
*/
#define NFP3800_PL_PluIOControls_PLU_SPI_WKPULL_EN           0x0004001c
#define   NFP3800_PL_PLUIOCONTROLS_PLU_SPI_WKPULL_EN_SPIX_D3_WKPDEN BIT(29)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_SPI_WKPULL_EN_SPIX_D2_WKPDEN BIT(28)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_SPI_WKPULL_EN_SPI3_MOSI_WKPDEN BIT(27)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_SPI_WKPULL_EN_SPI3_SCK_WKPDEN BIT(26)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_SPI_WKPULL_EN_SPI2_MOSI_WKPDEN BIT(25)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_SPI_WKPULL_EN_SPI2_SCK_WKPDEN BIT(24)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_SPI_WKPULL_EN_SPI1_CFG_MISO_WKPDEN BIT(23)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_SPI_WKPULL_EN_SPI1_CFG_MOSI_WKPDEN BIT(22)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_SPI_WKPULL_EN_SPI1_CFG_SEL_WKPDEN BIT(21)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_SPI_WKPULL_EN_SPI1_CFG_SCK_WKPDEN BIT(20)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_SPI_WKPULL_EN_SPI0_MISO_WKPDEN BIT(19)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_SPI_WKPULL_EN_SPI0_MOSI_WKPDEN BIT(18)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_SPI_WKPULL_EN_SPI0_SEL_WKPDEN BIT(17)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_SPI_WKPULL_EN_SPI0_SCK_WKPDEN BIT(16)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_SPI_WKPULL_EN_SPIX_D3_WKPUEN BIT(13)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_SPI_WKPULL_EN_SPIX_D2_WKPUEN BIT(12)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_SPI_WKPULL_EN_SPI3_MOSI_WKPUEN BIT(11)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_SPI_WKPULL_EN_SPI3_SCK_WKPUEN BIT(10)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_SPI_WKPULL_EN_SPI2_MOSI_WKPUEN BIT(9)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_SPI_WKPULL_EN_SPI2_SCK_WKPUEN BIT(8)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_SPI_WKPULL_EN_SPI1_CFG_MISO_WKPUEN BIT(7)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_SPI_WKPULL_EN_SPI1_CFG_MOSI_WKPUEN BIT(6)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_SPI_WKPULL_EN_SPI1_CFG_SEL_WKPUEN BIT(5)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_SPI_WKPULL_EN_SPI1_CFG_SCK_WKPUEN BIT(4)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_SPI_WKPULL_EN_SPI0_MISO_WKPUEN BIT(3)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_SPI_WKPULL_EN_SPI0_MOSI_WKPUEN BIT(2)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_SPI_WKPULL_EN_SPI0_SEL_WKPUEN BIT(1)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_SPI_WKPULL_EN_SPI0_SCK_WKPUEN BIT(0)
/*----------------------------------------------------------------
  Register: PluIOControls_MiscWkpullEn - Weak pull up and pull down for pins other than GPIO or SPI .
    Bits: 31    test_jtag_bypass_wkpden(rw) - Weak pull down enable for TEST_JTAG_BYPASS (1: weak pull down enabled, 0: weak pull down disabled).
    Bits: 30    test_scan_mode_wkpden(rw) - Weak pull down enable for TEST_SCAN_MODE (1: weak pull down enabled, 0: weak pull down disabled).
    Bits: 29    jtag_trst_wkpden(rw) - Weak pull down enable for JTAG_TRST (1: weak pull down enabled, 0: weak pull down disabled).
    Bits: 28    jtag_tms_wkpden(rw) - Weak pull down enable for JTAG_TMS (1: weak pull down enabled, 0: weak pull down disabled).
    Bits: 27    jtag_tdi_wkpden(rw) - Weak pull down enable for JTAG_TDI (1: weak pull down enabled, 0: weak pull down disabled).
    Bits: 26    jtag_tck_wkpden(rw) - Weak pull down enable for JTAG_TCK (1: weak pull down enabled, 0: weak pull down disabled).
    Bits: 25    pcie_reset_in_l_wkpden(rw) - Weak pull down enable for PCIE_RESET_IN_L (1: weak pull down enabled, 0: weak pull down disabled).
    Bits: 24    sr0_rx_wkpden(rw) - Weak pull down enable for SR0_RX (1: weak pull down enabled, 0: weak pull down disabled).
    Bits: 23    sr0_tx_wkpden(rw) - Weak pull down enable for SR0_TX (1: weak pull down enabled, 0: weak pull down disabled).
    Bits: 22    smbalrt_wkpden(rw) - Weak pull down enable for SMBALRT (1: weak pull down enabled, 0: weak pull down disabled).
    Bits: 21    smbdat_wkpden(rw) - Weak pull down enable for SMBDAT (1: weak pull down enabled, 0: weak pull down disabled).
    Bits: 20    smbclk_wkpden(rw) - Weak pull down enable for SMBCLK (1: weak pull down enabled, 0: weak pull down disabled).
    Bits: 19    sda1_wkpden(rw) - Weak pull down enable for SDA1 (1: weak pull down enabled, 0: weak pull down disabled).
    Bits: 18    scl1_wkpden(rw) - Weak pull down enable for SCL1 (1: weak pull down enabled, 0: weak pull down disabled).
    Bits: 17    sda0_wkpden(rw) - Weak pull down enable for SDA0 (1: weak pull down enabled, 0: weak pull down disabled).
    Bits: 16    scl0_wkpden(rw) - Weak pull down enable for SCL0 (1: weak pull down enabled, 0: weak pull down disabled).
    Bits: 15    test_jtag_bypass_wkpuen(rw) - Weak pull up enable for TEST_JTAG_BYPASS (1: weak pull up enabled, 0: weak pull up disabled).
    Bits: 14    test_scan_mode_wkpuen(rw) - Weak pull up enable for TEST_SCAN_MODE (1: weak pull up enabled, 0: weak pull up disabled).
    Bits: 13    jtag_trst_wkpuen(rw) - Weak pull up enable for JTAG_TRST (1: weak pull up enabled, 0: weak pull up disabled).
    Bits: 12    jtag_tms_wkpuen(rw) - Weak pull up enable for JTAG_TMS (1: weak pull up enabled, 0: weak pull up disabled).
    Bits: 11    jtag_tdi_wkpuen(rw) - Weak pull up enable for JTAG_TDI (1: weak pull up enabled, 0: weak pull up disabled).
    Bits: 10    jtag_tck_wkpuen(rw) - Weak pull up enable for JTAG_TCK (1: weak pull up enabled, 0: weak pull up disabled).
    Bits:  9    pcie_reset_in_l_wkpuen(rw) - Weak pull up enable for PCIE_RESET_IN_L (1: weak pull up enabled, 0: weak pull up disabled).
    Bits:  8    sr0_rx_wkpuen(rw) - Weak pull up enable for SR0_RX (1: weak pull up enabled, 0: weak pull up disabled).
    Bits:  7    sr0_tx_wkpuen(rw) - Weak pull up enable for SR0_TX (1: weak pull up enabled, 0: weak pull up disabled).
    Bits:  6    smbalrt_wkpuen(rw) - Weak pull up enable for SMBALRT (1: weak pull up enabled, 0: weak pull up disabled).
    Bits:  5    smbdat_wkpuen(rw) - Weak pull up enable for SMBDAT (1: weak pull up enabled, 0: weak pull up disabled).
    Bits:  4    smbclk_wkpuen(rw) - Weak pull up enable for SMBCLK (1: weak pull up enabled, 0: weak pull up disabled).
    Bits:  3    sda1_wkpuen(rw) - Weak pull up enable for SDA1 (1: weak pull up enabled, 0: weak pull up disabled).
    Bits:  2    scl1_wkpuen(rw) - Weak pull up enable for SCL1 (1: weak pull up enabled, 0: weak pull up disabled).
    Bits:  1    sda0_wkpuen(rw) - Weak pull up enable for SDA0 (1: weak pull up enabled, 0: weak pull up disabled).
    Bits:  0    scl0_wkpuen(rw) - Weak pull up enable for SCL0 (1: weak pull up enabled, 0: weak pull up disabled).
*/
#define NFP3800_PL_PluIOControls_PLU_MISC_WKPULL_EN          0x00040020
#define   NFP3800_PL_PLUIOCONTROLS_PLU_MISC_WKPULL_EN_TEST_JTAG_BYPASS_WKPDEN BIT(31)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_MISC_WKPULL_EN_TEST_SCAN_MODE_WKPDEN BIT(30)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_MISC_WKPULL_EN_JTAG_TRST_WKPDEN BIT(29)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_MISC_WKPULL_EN_JTAG_TMS_WKPDEN BIT(28)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_MISC_WKPULL_EN_JTAG_TDI_WKPDEN BIT(27)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_MISC_WKPULL_EN_JTAG_TCK_WKPDEN BIT(26)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_MISC_WKPULL_EN_PCIE_RESET_IN_L_WKPDEN BIT(25)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_MISC_WKPULL_EN_SR0_RX_WKPDEN BIT(24)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_MISC_WKPULL_EN_SR0_TX_WKPDEN BIT(23)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_MISC_WKPULL_EN_SMBALRT_WKPDEN BIT(22)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_MISC_WKPULL_EN_SMBDAT_WKPDEN BIT(21)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_MISC_WKPULL_EN_SMBCLK_WKPDEN BIT(20)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_MISC_WKPULL_EN_SDA1_WKPDEN BIT(19)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_MISC_WKPULL_EN_SCL1_WKPDEN BIT(18)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_MISC_WKPULL_EN_SDA0_WKPDEN BIT(17)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_MISC_WKPULL_EN_SCL0_WKPDEN BIT(16)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_MISC_WKPULL_EN_TEST_JTAG_BYPASS_WKPUEN BIT(15)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_MISC_WKPULL_EN_TEST_SCAN_MODE_WKPUEN BIT(14)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_MISC_WKPULL_EN_JTAG_TRST_WKPUEN BIT(13)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_MISC_WKPULL_EN_JTAG_TMS_WKPUEN BIT(12)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_MISC_WKPULL_EN_JTAG_TDI_WKPUEN BIT(11)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_MISC_WKPULL_EN_JTAG_TCK_WKPUEN BIT(10)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_MISC_WKPULL_EN_PCIE_RESET_IN_L_WKPUEN BIT(9)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_MISC_WKPULL_EN_SR0_RX_WKPUEN BIT(8)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_MISC_WKPULL_EN_SR0_TX_WKPUEN BIT(7)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_MISC_WKPULL_EN_SMBALRT_WKPUEN BIT(6)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_MISC_WKPULL_EN_SMBDAT_WKPUEN BIT(5)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_MISC_WKPULL_EN_SMBCLK_WKPUEN BIT(4)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_MISC_WKPULL_EN_SDA1_WKPUEN BIT(3)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_MISC_WKPULL_EN_SCL1_WKPUEN BIT(2)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_MISC_WKPULL_EN_SDA0_WKPUEN BIT(1)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_MISC_WKPULL_EN_SCL0_WKPUEN BIT(0)
/*----------------------------------------------------------------
  Register: PluIOControls_GpioOpendrain - Open drain enable for GPIO.
    Bits: 31:0  gpio_opendrain(rw) - Open drain mode enable for GPIO[31:0] (1: Pin is in open drain mode; 0: Pin is NOT in open drain mode).
*/
#define NFP3800_PL_PluIOControls_PLU_GPIO_OPENDRAIN          0x00040024
#define   NFP3800_PL_PLUIOCONTROLS_PLU_GPIO_OPENDRAIN_GPIO_OPENDRAIN(_x) (((_x) & 0xffffffff) << 0)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_GPIO_OPENDRAIN_GPIO_OPENDRAIN_of(_x) (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: PluIOControls_MiscOpendrain - Open drain enable for pins other than GPIO.
    Bits: 25    jtag_tdo_opendrain(rw) - Open drain enable for JTAG_TDO (1: Open drain enabled, 0: Open drain disabled).
    Bits: 24    pcie_reset_out_l_opendrain(rw) - Open drain enable for PCIE_RESET_OUT_L (1: Open drain enabled, 0: Open drain disabled).
    Bits: 23    clk_nreset_out_l_opendrain(rw) - Open drain enable for CLK_NRESET_OUT_L (1: Open drain enabled, 0: Open drain disabled).
    Bits: 22    sr0_rx_opendrain(rw) - Open drain enable for SR0_RX (1: Open drain enabled, 0: Open drain disabled).
    Bits: 21    sr0_tx_opendrain(rw) - Open drain enable for SR0_TX (1: Open drain enabled, 0: Open drain disabled).
    Bits: 20    smbalrt_opendrain(rw) - Open drain enable for SMBALRT (1: Open drain enabled, 0: Open drain disabled).
    Bits: 19    smbdat_opendrain(rw) - Open drain enable for SMBDAT (1: Open drain enabled, 0: Open drain disabled).
    Bits: 18    smbclk_opendrain(rw) - Open drain enable for SMBCLK (1: Open drain enabled, 0: Open drain disabled).
    Bits: 17    sda1_opendrain(rw) - Open drain enable for SDA1 (1: Open drain enabled, 0: Open drain disabled).
    Bits: 16    scl1_opendrain(rw) - Open drain enable for SCL1 (1: Open drain enabled, 0: Open drain disabled).
    Bits: 15    sda0_opendrain(rw) - Open drain enable for SDA0 (1: Open drain enabled, 0: Open drain disabled).
    Bits: 14    scl0_opendrain(rw) - Open drain enable for SCL0 (1: Open drain enabled, 0: Open drain disabled).
    Bits: 13    spix_d3_opendrain(rw) - Open drain enable for SPIX_D3 (1: Open drain enabled, 0: Open drain disabled).
    Bits: 12    spix_d2_opendrain(rw) - Open drain enable for SPIX_D2 (1: Open drain enabled, 0: Open drain disabled).
    Bits: 11    spi3_mosi_opendrain(rw) - Open drain enable for SPI3_MOSI (1: Open drain enabled, 0: Open drain disabled).
    Bits: 10    spi3_sck_opendrain(rw) - Open drain enable for SPI3_SCK (1: Open drain enabled, 0: Open drain disabled).
    Bits:  9    spi2_mosi_opendrain(rw) - Open drain enable for SPI2_MOSI (1: Open drain enabled, 0: Open drain disabled).
    Bits:  8    spi2_sck_opendrain(rw) - Open drain enable for SPI2_SCK (1: Open drain enabled, 0: Open drain disabled).
    Bits:  7    spi1_cfg_miso_opendrain(rw) - Open drain enable for SPI1_CFG_MISO (1: Open drain enabled, 0: Open drain disabled).
    Bits:  6    spi1_cfg_mosi_opendrain(rw) - Open drain enable for SPI1_CFG_MOSI (1: Open drain enabled, 0: Open drain disabled).
    Bits:  5    spi1_cfg_sel_opendrain(rw) - Open drain enable for SPI1_CFG_SEL (1: Open drain enabled, 0: Open drain disabled).
    Bits:  4    spi1_cfg_sck_opendrain(rw) - Open drain enable for SPI1_CFG_SCK (1: Open drain enabled, 0: Open drain disabled).
    Bits:  3    spi0_miso_opendrain(rw) - Open drain enable for SPI0_MISO (1: Open drain enabled, 0: Open drain disabled).
    Bits:  2    spi0_mosi_opendrain(rw) - Open drain enable for SPI0_MOSI (1: Open drain enabled, 0: Open drain disabled).
    Bits:  1    spi0_sel_opendrain(rw) - Open drain enable for SPI0_SEL (1: Open drain enabled, 0: Open drain disabled).
    Bits:  0    spi0_sck_opendrain(rw) - Open drain enable for SPI0_SCK (1: Open drain enabled, 0: Open drain disabled).
*/
#define NFP3800_PL_PluIOControls_PLU_MISC_OPENDRAIN          0x00040028
#define   NFP3800_PL_PLUIOCONTROLS_PLU_MISC_OPENDRAIN_JTAG_TDO_OPENDRAIN BIT(25)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_MISC_OPENDRAIN_PCIE_RESET_OUT_L_OPENDRAIN BIT(24)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_MISC_OPENDRAIN_CLK_NRESET_OUT_L_OPENDRAIN BIT(23)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_MISC_OPENDRAIN_SR0_RX_OPENDRAIN BIT(22)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_MISC_OPENDRAIN_SR0_TX_OPENDRAIN BIT(21)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_MISC_OPENDRAIN_SMBALRT_OPENDRAIN BIT(20)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_MISC_OPENDRAIN_SMBDAT_OPENDRAIN BIT(19)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_MISC_OPENDRAIN_SMBCLK_OPENDRAIN BIT(18)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_MISC_OPENDRAIN_SDA1_OPENDRAIN BIT(17)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_MISC_OPENDRAIN_SCL1_OPENDRAIN BIT(16)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_MISC_OPENDRAIN_SDA0_OPENDRAIN BIT(15)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_MISC_OPENDRAIN_SCL0_OPENDRAIN BIT(14)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_MISC_OPENDRAIN_SPIX_D3_OPENDRAIN BIT(13)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_MISC_OPENDRAIN_SPIX_D2_OPENDRAIN BIT(12)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_MISC_OPENDRAIN_SPI3_MOSI_OPENDRAIN BIT(11)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_MISC_OPENDRAIN_SPI3_SCK_OPENDRAIN BIT(10)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_MISC_OPENDRAIN_SPI2_MOSI_OPENDRAIN BIT(9)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_MISC_OPENDRAIN_SPI2_SCK_OPENDRAIN BIT(8)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_MISC_OPENDRAIN_SPI1_CFG_MISO_OPENDRAIN BIT(7)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_MISC_OPENDRAIN_SPI1_CFG_MOSI_OPENDRAIN BIT(6)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_MISC_OPENDRAIN_SPI1_CFG_SEL_OPENDRAIN BIT(5)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_MISC_OPENDRAIN_SPI1_CFG_SCK_OPENDRAIN BIT(4)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_MISC_OPENDRAIN_SPI0_MISO_OPENDRAIN BIT(3)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_MISC_OPENDRAIN_SPI0_MOSI_OPENDRAIN BIT(2)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_MISC_OPENDRAIN_SPI0_SEL_OPENDRAIN BIT(1)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_MISC_OPENDRAIN_SPI0_SCK_OPENDRAIN BIT(0)
/*----------------------------------------------------------------
  Register: PluIOControls_GpioRxEn - Buffer input enable for GPIO.
    Bits: 31:0  gpio_rcven(rw) - Input buffer enable for GPIO[31:0], active high.
*/
#define NFP3800_PL_PluIOControls_PLU_GPIO_RX_EN              0x0004002c
#define   NFP3800_PL_PLUIOCONTROLS_PLU_GPIO_RX_EN_GPIO_RCVEN(_x) (((_x) & 0xffffffff) << 0)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_GPIO_RX_EN_GPIO_RCVEN_of(_x) (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: PluIOControls_MiscRxEn - Buffer input enable for pin other than GPIO.
    Bits: 23    pcie_reset_in_l_rcven(rw) - Buffer input enable for PCIE_RESET_IN_L pin.
    Bits: 22    smbalrt_rcven(rw) - Buffer input enable for SMBALRT pin.
    Bits: 21    smbdat_rcven(rw) - Buffer input enable for SMBDAT pin.
    Bits: 20    smbclk_rcven(rw) - Buffer input enable for SMBCLK pin.
    Bits: 19    sda1_rcven(rw) - Buffer input enable for SDA1 pin.
    Bits: 18    scl1_rcven(rw) - Buffer input enable for SCL1 pin.
    Bits: 17    sda0_rcven(rw) - Buffer input enable for SDA0 pin.
    Bits: 16    scl0_rcven(rw) - Buffer input enable for SCL0 pin.
    Bits: 15    sr0_tx_rcven(rw) - Buffer input enable for Serial port 0 TX pin (0: on, 1: off).
    Bits: 14    sr0_rx_rcven(rw) - Buffer input enable for Serial port 0 RX pin (0: on, 1: off).
    Bits: 13    spix_d3_rcven(rw) - Buffer input enable for SPIX_D3 pin.
    Bits: 12    spix_d2_rcven(rw) - Buffer input enable for SPIX_D2 pin.
    Bits: 11    spi3_sck_rcven(rw) - Buffer input enable for SPI3_SCK pin.
    Bits: 10    spi3_mosi_rcven(rw) - Buffer input enable for SPI3_MOSI pin.
    Bits:  9    spi2_sck_rcven(rw) - Buffer input enable for SPI2_SCK pin.
    Bits:  8    spi2_mosi_rcven(rw) - Buffer input enable for SPI2_MOSI pin.
    Bits:  7    spi1_cfg_sel_rcven(rw) - Buffer input enable for SPI_CFG_SEL pin.
    Bits:  6    spi1_cfg_sck_rcven(rw) - Buffer input enable for SPI_CFG_SCK pin.
    Bits:  5    spi1_cfg_mosi_rcven(rw) - Buffer input enable for SPI1_CFG_MOSI pin.
    Bits:  4    spi1_cfg_miso_rcven(rw) - Buffer input enable for SPI1_CFG_MISO pin.
    Bits:  3    spi0_sel_rcven(rw) - Buffer input enable for SPI0_SEL pin.
    Bits:  2    spi0_sck_rcven(rw) - Buffer input enable for SPI0_SCK pin.
    Bits:  1    spi0_mosi_rcven(rw) - Buffer input enable for SPI0_MOSI pin.
    Bits:  0    spi0_miso_rcven(rw) - Buffer input enable for SPI0_MISO pin.
*/
#define NFP3800_PL_PluIOControls_PLU_MISC_RX_EN              0x00040030
#define   NFP3800_PL_PLUIOCONTROLS_PLU_MISC_RX_EN_PCIE_RESET_IN_L_RCVEN BIT(23)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_MISC_RX_EN_SMBALRT_RCVEN BIT(22)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_MISC_RX_EN_SMBDAT_RCVEN BIT(21)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_MISC_RX_EN_SMBCLK_RCVEN BIT(20)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_MISC_RX_EN_SDA1_RCVEN BIT(19)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_MISC_RX_EN_SCL1_RCVEN BIT(18)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_MISC_RX_EN_SDA0_RCVEN BIT(17)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_MISC_RX_EN_SCL0_RCVEN BIT(16)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_MISC_RX_EN_SR0_TX_RCVEN BIT(15)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_MISC_RX_EN_SR0_RX_RCVEN BIT(14)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_MISC_RX_EN_SPIX_D3_RCVEN BIT(13)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_MISC_RX_EN_SPIX_D2_RCVEN BIT(12)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_MISC_RX_EN_SPI3_SCK_RCVEN BIT(11)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_MISC_RX_EN_SPI3_MOSI_RCVEN BIT(10)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_MISC_RX_EN_SPI2_SCK_RCVEN BIT(9)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_MISC_RX_EN_SPI2_MOSI_RCVEN BIT(8)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_MISC_RX_EN_SPI1_CFG_SEL_RCVEN BIT(7)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_MISC_RX_EN_SPI1_CFG_SCK_RCVEN BIT(6)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_MISC_RX_EN_SPI1_CFG_MOSI_RCVEN BIT(5)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_MISC_RX_EN_SPI1_CFG_MISO_RCVEN BIT(4)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_MISC_RX_EN_SPI0_SEL_RCVEN BIT(3)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_MISC_RX_EN_SPI0_SCK_RCVEN BIT(2)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_MISC_RX_EN_SPI0_MOSI_RCVEN BIT(1)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_MISC_RX_EN_SPI0_MISO_RCVEN BIT(0)
/*----------------------------------------------------------------
  Register: PluIOControls_MiscControl - Misc IO controls.
    Bits:  1    gpio33_mode_1p8(rw) - IO PAD mode control for all 3.3V IO pins
    Bits:  0    gpio_force_tristate(rw) - Force tristate on all 3.3V and 1.8V pins.
*/
#define NFP3800_PL_PluIOControls_PLU_MISC_CONTROL            0x00040034
#define   NFP3800_PL_PLUIOCONTROLS_PLU_MISC_CONTROL_GPIO33_MODE_1P8 BIT(1)
#define     NFP3800_PL_PLUIOCONTROLS_PLU_MISC_CONTROL_GPIO33_MODE_1P8_0 (0 << 1)
#define     NFP3800_PL_PLUIOCONTROLS_PLU_MISC_CONTROL_GPIO33_MODE_1P8_1 BIT(1)
#define   NFP3800_PL_PLUIOCONTROLS_PLU_MISC_CONTROL_GPIO_FORCE_TRISTATE BIT(0)
#define     NFP3800_PL_PLUIOCONTROLS_PLU_MISC_CONTROL_GPIO_FORCE_TRISTATE_0 (0 << 0)
#define     NFP3800_PL_PLUIOCONTROLS_PLU_MISC_CONTROL_GPIO_FORCE_TRISTATE_1 BIT(0)
/*----------------------------------------------------------------
  Register: PluPMonBBGenXpb_PluPMonRegIntfc - Unbiased Process Monitor JTAG Mode Register Interface
  Register: PluPMonBBGenXpb_PluBPMonRegIntfc - Biased Process Monitor JTAG Mode Register Interface
    Bits: 31:30 Start(ro) - Start bits for Process Monitor Register Interface Frame
    Bits: 29:28 Op(rw) - OpCode for Process Monitor Register Interface
    Bits: 27:18 RegAddr(rw) - Register Address for Process Monitor Register Interface
    Bits: 15:0  Data(rw) - Register Write/Read Data for Process Monitor Register Interface
*/
#define NFP3800_PL_PluPMonBBGenXpb_PLU_PMON_REG_INTFC        0x00090000
#define NFP3800_PL_PluPMonBBGenXpb_PLU_BPMON_REG_INTFC       0x00090008
#define   NFP3800_PL_PLUPMONBBGENXPB_PLUPMONREGINTFC_START(_x) (((_x) & 0x3) << 30)
#define   NFP3800_PL_PLUPMONBBGENXPB_PLUPMONREGINTFC_START_of(_x) (((_x) >> 30) & 0x3)
#define   NFP3800_PL_PLUPMONBBGENXPB_PLUPMONREGINTFC_OP(_x)  (((_x) & 0x3) << 28)
#define   NFP3800_PL_PLUPMONBBGENXPB_PLUPMONREGINTFC_OP_of(_x) (((_x) >> 28) & 0x3)
#define     NFP3800_PL_PLUPMONBBGENXPB_PLUPMONREGINTFC_OP_NOOP (0)
#define     NFP3800_PL_PLUPMONBBGENXPB_PLUPMONREGINTFC_OP_WRITE (1)
#define   NFP3800_PL_PLUPMONBBGENXPB_PLUPMONREGINTFC_REGADDR(_x) (((_x) & 0x3ff) << 18)
#define   NFP3800_PL_PLUPMONBBGENXPB_PLUPMONREGINTFC_REGADDR_of(_x) (((_x) >> 18) & 0x3ff)
#define   NFP3800_PL_PLUPMONBBGENXPB_PLUPMONREGINTFC_DATA(_x) (((_x) & 0xffff) << 0)
#define   NFP3800_PL_PLUPMONBBGENXPB_PLUPMONREGINTFC_DATA_of(_x) (((_x) >> 0) & 0xffff)
/*----------------------------------------------------------------
  Register: PluPMonBBGenXpb_PluBBGenDynamicAdjustment - Bias Generation Dynamic Adjustment Enables
    Bits:  2    StateMachineActive(ro) - This is a status bit for the Dynamic Adjustment State Machine. 1 indicates that the state machine is active. In this state, all related control registers will have inhibited write access. 0 indicates that the state machine is idle and control registers can be written.
    Bits:  1    AutoAdjustEnable(rw) - When 0, Dynamic Adjustment State Machine will only take measurements but not adjust the BBGEN controls. When 1, BBGEN controls will be dynamically adjusted.
    Bits:  0    MeasurementEnable(rw) - When set to 1, enables and activates the Dynamic Adjustment State Machine. When set 0, the Dynamic Adjustment State Machine will go to idle at the completion of the next timer cycle.
*/
#define NFP3800_PL_PluPMonBBGenXpb_PLU_BBGEN_DYNAMIC_ADJUSTMENT 0x00090004
#define   NFP3800_PL_PLUPMONBBGENXPB_PLU_BBGEN_DYNAMIC_ADJUSTMENT_STATE_MACHINE_ACTIVE BIT(2)
#define   NFP3800_PL_PLUPMONBBGENXPB_PLU_BBGEN_DYNAMIC_ADJUSTMENT_AUTO_ADJUST_ENABLE BIT(1)
#define   NFP3800_PL_PLUPMONBBGENXPB_PLU_BBGEN_DYNAMIC_ADJUSTMENT_MEASUREMENT_ENABLE BIT(0)
/*----------------------------------------------------------------
  Register: PluPMonBBGenXpb_PluPMonDAControls - Biased Process Montor Direct Access Controls
    Bits:  9    DA_JTAG_SEL(rw) - Selects between JTAG Control (0) or Direct Access Control (1).
    Bits:  8    DA_RO_EN(rw) - Ring Oscillator Enable in Direct Access Mode: 1 for Enable Ring Oscillator, 0 for Disable Ring Oscillator.
    Bits:  7    DA_RO_BYPASS(rw) - Ring Oscillator ByPass: 1 for Reference Clock to the Output, 0 for Ring Oscillator Clock to the Output.
    Bits:  6    DA_MILLER_CTRL(rw) - Control Signal to test the Miller Effect.
    Bits:  5:0  DA_DECODE(rw) - Selects between multiple ring oscillators.
*/
#define NFP3800_PL_PluPMonBBGenXpb_PLU_PMON_DA_CONTROLS      0x0009000c
#define   NFP3800_PL_PLUPMONBBGENXPB_PLU_PMON_DA_CONTROLS_DA_JTAG_SEL BIT(9)
#define   NFP3800_PL_PLUPMONBBGENXPB_PLU_PMON_DA_CONTROLS_DA_RO_EN BIT(8)
#define   NFP3800_PL_PLUPMONBBGENXPB_PLU_PMON_DA_CONTROLS_DA_RO_BYPASS BIT(7)
#define   NFP3800_PL_PLUPMONBBGENXPB_PLU_PMON_DA_CONTROLS_DA_MILLER_CTRL BIT(6)
#define   NFP3800_PL_PLUPMONBBGENXPB_PLU_PMON_DA_CONTROLS_DA_DECODE(_x) (((_x) & 0x3f) << 0)
#define   NFP3800_PL_PLUPMONBBGENXPB_PLU_PMON_DA_CONTROLS_DA_DECODE_of(_x) (((_x) >> 0) & 0x3f)
/*----------------------------------------------------------------
  Register: PluPMonBBGenXpb_PluPMonRefTimerStartValue - Reference Timer Value to Start PMon Ring Oscillator (DA_CLKOUT) Measurement
  Register: PluPMonBBGenXpb_PluPMonRefTimerStopValue - Reference Timer Value to Stop PMon Ring Oscillator (DA_CLKOUT) Measurement
    Bits: 31:0  RefTimerValue(rw) - 32-bit Timer Value.
*/
#define NFP3800_PL_PluPMonBBGenXpb_PLU_PMON_REF_TIMER_START_VALUE 0x00090010
#define NFP3800_PL_PluPMonBBGenXpb_PLU_PMON_REF_TIMER_STOP_VALUE 0x00090014
#define   NFP3800_PL_PLUPMONBBGENXPB_PLUPMONREFTIMERVALUE_REF_TIMER_VALUE(_x) (((_x) & 0xffffffff) << 0)
#define   NFP3800_PL_PLUPMONBBGENXPB_PLUPMONREFTIMERVALUE_REF_TIMER_VALUE_of(_x) (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: PluPMonBBGenXpb_PluPMonClkOutTimerTargetValue - Target DA_CLKOUT Timer Value for setting BBGEN
  Register: PluPMonBBGenXpb_PluPMonClkOutTimerDiffThreshold0 - (Target - Timer) Difference Threshold0 for BBGEN Adjustment Calculations
  Register: PluPMonBBGenXpb_PluPMonClkOutTimerDiffThreshold1 - (Target - Timer) Difference Threshold1 for BBGEN Adjustment Calculations
  Register: PluPMonBBGenXpb_PluPMonClkOutTimerDiffThreshold2 - (Target - Timer) Difference Threshold2 for BBGEN Adjustment Calculations
    Bits: 31:0  ClkOutTimerValue(rw) - 32-bit Timer Value.
*/
#define NFP3800_PL_PluPMonBBGenXpb_PLU_PMON_CLKOUT_TIMER_TARGET_VALUE 0x00090018
#define NFP3800_PL_PluPMonBBGenXpb_PLU_PMON_CLKOUT_TIMER_DIFF_THRESHOLD0 0x00090020
#define NFP3800_PL_PluPMonBBGenXpb_PLU_PMON_CLKOUT_TIMER_DIFF_THRESHOLD1 0x00090024
#define NFP3800_PL_PluPMonBBGenXpb_PLU_PMON_CLKOUT_TIMER_DIFF_THRESHOLD2 0x00090028
#define   NFP3800_PL_PLUPMONBBGENXPB_PLUPMONCLKOUTTIMERVALUE_CLKOUT_TIMER_VALUE(_x) (((_x) & 0xffffffff) << 0)
#define   NFP3800_PL_PLUPMONBBGENXPB_PLUPMONCLKOUTTIMERVALUE_CLKOUT_TIMER_VALUE_of(_x) (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: PluPMonBBGenXpb_PluPMonClkOutTimerSampleValue - DA_CLKOUT Timer Value Sampled after Reference Timer Stop Value
    Bits: 31:0  ClkOutTimerSample(ro) - 32-bit Timer Sample Value.
*/
#define NFP3800_PL_PluPMonBBGenXpb_PLU_PMON_CLKOUT_TIMER_SAMPLE_VALUE 0x0009001c
#define   NFP3800_PL_PLUPMONBBGENXPB_PLU_PMON_CLKOUT_TIMER_SAMPLE_VALUE_CLKOUT_TIMER_SAMPLE(_x) (((_x) & 0xffffffff) << 0)
#define   NFP3800_PL_PLUPMONBBGENXPB_PLU_PMON_CLKOUT_TIMER_SAMPLE_VALUE_CLKOUT_TIMER_SAMPLE_of(_x) (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: PluPMonBBGenXpb_PluBBGenAdjustLUT - BBGEN Adjustment Lookup Table
    Bits:  7:6  LUTEntry3(rw) - 2-bit Adjustment Value.
    Bits:  5:4  LUTEntry2(rw) - 2-bit Adjustment Value.
    Bits:  3:2  LUTEntry1(rw) - 2-bit Adjustment Value.
    Bits:  1:0  LUTEntry0(rw) - 2-bit Adjustment Value.
*/
#define NFP3800_PL_PluPMonBBGenXpb_PLU_BBGEN_ADJUST_LUT      0x00090030
#define   NFP3800_PL_PLUPMONBBGENXPB_PLU_BBGEN_ADJUST_LUT_LUT_ENTRY3(_x) (((_x) & 0x3) << 6)
#define   NFP3800_PL_PLUPMONBBGENXPB_PLU_BBGEN_ADJUST_LUT_LUT_ENTRY3_of(_x) (((_x) >> 6) & 0x3)
#define   NFP3800_PL_PLUPMONBBGENXPB_PLU_BBGEN_ADJUST_LUT_LUT_ENTRY2(_x) (((_x) & 0x3) << 4)
#define   NFP3800_PL_PLUPMONBBGENXPB_PLU_BBGEN_ADJUST_LUT_LUT_ENTRY2_of(_x) (((_x) >> 4) & 0x3)
#define   NFP3800_PL_PLUPMONBBGENXPB_PLU_BBGEN_ADJUST_LUT_LUT_ENTRY1(_x) (((_x) & 0x3) << 2)
#define   NFP3800_PL_PLUPMONBBGENXPB_PLU_BBGEN_ADJUST_LUT_LUT_ENTRY1_of(_x) (((_x) >> 2) & 0x3)
#define   NFP3800_PL_PLUPMONBBGENXPB_PLU_BBGEN_ADJUST_LUT_LUT_ENTRY0(_x) (((_x) & 0x3) << 0)
#define   NFP3800_PL_PLUPMONBBGENXPB_PLU_BBGEN_ADJUST_LUT_LUT_ENTRY0_of(_x) (((_x) >> 0) & 0x3)
/*----------------------------------------------------------------
  Register: PluPMonBBGenXpb_PluBBGenControl - BBGEN Control Values
    Bits: 31    BBGenLoadSeqBusy(ro) - Load Sequencer Busy: Can't handle another update yet.
    Bits: 12:8  BBGenpCtrl(rw) - Voltage Control Value: Starting 0 with steps of 57mV.
    Bits:  4:0  BBGennCtrl(rw) - Voltage Control Value: Starting 0 with steps of -57mV.
*/
#define NFP3800_PL_PluPMonBBGenXpb_PLU_BBGEN_CONTROL         0x00090034
#define   NFP3800_PL_PLUPMONBBGENXPB_PLU_BBGEN_CONTROL_BBGEN_LOAD_SEQ_BUSY BIT(31)
#define   NFP3800_PL_PLUPMONBBGENXPB_PLU_BBGEN_CONTROL_BBGENP_CTRL(_x) (((_x) & 0x1f) << 8)
#define   NFP3800_PL_PLUPMONBBGENXPB_PLU_BBGEN_CONTROL_BBGENP_CTRL_of(_x) (((_x) >> 8) & 0x1f)
#define   NFP3800_PL_PLUPMONBBGENXPB_PLU_BBGEN_CONTROL_BBGENN_CTRL(_x) (((_x) & 0x1f) << 0)
#define   NFP3800_PL_PLUPMONBBGENXPB_PLU_BBGEN_CONTROL_BBGENN_CTRL_of(_x) (((_x) >> 0) & 0x1f)
/*----------------------------------------------------------------
  Register: PluPMonBBGenXpb_PluBBGenLimit - BBGEN Control Value Limits
    Bits: 12:8  BBGenpLimit(rw) - Voltage Control Limit Value: 798mV.
    Bits:  4:0  BBGennLimit(rw) - Voltage Control Limit Value: -798mV.
*/
#define NFP3800_PL_PluPMonBBGenXpb_PLU_BBGEN_LIMIT           0x00090038
#define   NFP3800_PL_PLUPMONBBGENXPB_PLU_BBGEN_LIMIT_BBGENP_LIMIT(_x) (((_x) & 0x1f) << 8)
#define   NFP3800_PL_PLUPMONBBGENXPB_PLU_BBGEN_LIMIT_BBGENP_LIMIT_of(_x) (((_x) >> 8) & 0x1f)
#define   NFP3800_PL_PLUPMONBBGENXPB_PLU_BBGEN_LIMIT_BBGENN_LIMIT(_x) (((_x) & 0x1f) << 0)
#define   NFP3800_PL_PLUPMONBBGENXPB_PLU_BBGEN_LIMIT_BBGENN_LIMIT_of(_x) (((_x) >> 0) & 0x1f)
/*----------------------------------------------------------------
  Register: PluPMonBBGenXpb_PluFusePmonClkOutTimerTargetValue - Fused Recommended Value Dynamic BBGEN Target Value
    Bits: 31:0  TargetValue(ro) - Fused Target Value for Dynamic BBGen Algorithm from Fuse Chain 0.
*/
#define NFP3800_PL_PluPMonBBGenXpb_PLU_FUSE_PMON_CLKOUT_TIMER_TARGET_VALUE 0x0009003c
#define   NFP3800_PL_PLUPMONBBGENXPB_PLU_FUSE_PMON_CLKOUT_TIMER_TARGET_VALUE_TARGET_VALUE(_x) (((_x) & 0xffffffff) << 0)
#define   NFP3800_PL_PLUPMONBBGENXPB_PLU_FUSE_PMON_CLKOUT_TIMER_TARGET_VALUE_TARGET_VALUE_of(_x) (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: PluPMonBBGenXpb_PluFuseBBGenControl - Fused BBGEN Control Initial Values and AutoUpdate Indicator
    Bits: 12:8  BBGenpCtrl(ro) - Fused Voltage Control Value
    Bits:  4:0  BBGennCtrl(ro) - Fused Voltage Control Value
*/
#define NFP3800_PL_PluPMonBBGenXpb_PLU_FUSE_BBGEN_CONTROL    0x00090040
#define   NFP3800_PL_PLUPMONBBGENXPB_PLU_FUSE_BBGEN_CONTROL_BBGENP_CTRL(_x) (((_x) & 0x1f) << 8)
#define   NFP3800_PL_PLUPMONBBGENXPB_PLU_FUSE_BBGEN_CONTROL_BBGENP_CTRL_of(_x) (((_x) >> 8) & 0x1f)
#define   NFP3800_PL_PLUPMONBBGENXPB_PLU_FUSE_BBGEN_CONTROL_BBGENN_CTRL(_x) (((_x) & 0x1f) << 0)
#define   NFP3800_PL_PLUPMONBBGENXPB_PLU_FUSE_BBGEN_CONTROL_BBGENN_CTRL_of(_x) (((_x) >> 0) & 0x1f)
/*----------------------------------------------------------------
  Register: XPB2Jtag_MiscControl - Island-specific miscellaneous control
    Bits: 31:0  MiscCtrl(rw) - Miscellaneous Control Register - function depends on island.
*/
#define NFP3800_PL_XPB2Jtag_MISC_CTRL                        0x002f5410
#define   NFP3800_PL_XPB2JTAG_MISC_CTRL_MISC_CTRL(_x)        (((_x) & 0xffffffff) << 0)
#define   NFP3800_PL_XPB2JTAG_MISC_CTRL_MISC_CTRL_of(_x)     (((_x) >> 0) & 0xffffffff)

#else /* NFP3800_LONGNAMES */

/*----------------------------------------------------------------
  Register: PluMisc_PluMasterReset - Programmable Reset Controls
    Bits: 28    WDToMClkPD(rw) - When 1, Watchdog Timer Pop causes M_CLK PLL to power down
    Bits: 27    WDToAClkPD(rw) - When 1, Watchdog Timer Pop causes A_CLK PLL to power down
    Bits: 26    WDToPClkPD(rw) - When 1, Watchdog Timer Pop causes P_CLK PLL to power down
    Bits: 25    WDToTClkPD(rw) - When 1, Watchdog Timer Pop causes T_CLK PLL to power down
    Bits: 24    WDToClkNResetOut(rw) - When 1, Watchdog Timer Pop causes asserion low of CLK_NRESET_OUT_L
    Bits: 23    TSTempAlrtToMClkPD(rw) - When 1, TSTempAlrt causes M_CLK PLL to power down
    Bits: 22    TSTempAlrtToAClkPD(rw) - When 1, TSTempAlrt causes A_CLK PLL to power down
    Bits: 21    TSTempAlrtToPClkPD(rw) - When 1, TSTempAlrt causes P_CLK PLL to power down
    Bits: 20    TSTempAlrtToTClkPD(rw) - When 1, TSTempAlrt causes T_CLK PLL to power down
    Bits: 19    TSTempAlrtToClkNResetOut(rw) - When 1, TSTempAlrt causes asserion low of CLK_NRESET_OUT_L
    Bits: 18    TSTempAlrtToSoftReset(rw) - When 1, TSTempAlrt causes a chip-level Soft Reset
    Bits: 17    TSTempTripToMClkPD(rw) - When 1, TSTempTrip causes M_CLK PLL to power down
    Bits: 16    TSTempTripToAClkPD(rw) - When 1, TSTempTrip causes A_CLK PLL to power down
    Bits: 15    TSTempTripToPClkPD(rw) - When 1, TSTempTrip causes P_CLK PLL to power down
    Bits: 14    TSTempTripToTClkPD(rw) - When 1, TSTempTrip causes T_CLK PLL to power down
    Bits: 13    TSTempTripToClkNResetOut(rw) - When 1, TSTempTrip causes assertion low of CLK_NRESET_OUT_L
    Bits: 12    TSTempTripToSoftReset(rw) - When 1, TSTempTrip causes a chip-level Soft Reset
    Bits:  8    PCIE0_RESET_MASK(rw) - When set to 1, PCIE0_RESET_OUT_L will not be reset by SRESETN
    Bits:  4    PCIE0_RESET_OUT_L(rw) - PCIE0 Reset Out
    Bits:  3    SResetToClkNResetOut(rw) - When 1, Chip Pin SRESETN==0 causes CLK_NRESET_OUT_L set to 0
    Bits:  2    ProgSResetToClkNResetOut(rw) - When 1, ProgSReset==0 causes CLK_NRESET_OUT_L set to 0
    Bits:  1    PROG_SRESET_L(rw) - Programmable Chip Soft Reset; Asserted Low
    Bits:  0    CLK_NRESET_OUT_L(rw) - PowerOn Reset Out; Asserted Low
*/
#define NFP3800_PL_PluMisc_PluMasterReset                    0x00000000
#define   NFP3800_PL_PluMisc_PluMasterReset_WDToMClkPD       BIT(28)
#define   NFP3800_PL_PluMisc_PluMasterReset_WDToAClkPD       BIT(27)
#define   NFP3800_PL_PluMisc_PluMasterReset_WDToPClkPD       BIT(26)
#define   NFP3800_PL_PluMisc_PluMasterReset_WDToTClkPD       BIT(25)
#define   NFP3800_PL_PluMisc_PluMasterReset_WDToClkNResetOut BIT(24)
#define   NFP3800_PL_PluMisc_PluMasterReset_TSTempAlrtToMClkPD BIT(23)
#define   NFP3800_PL_PluMisc_PluMasterReset_TSTempAlrtToAClkPD BIT(22)
#define   NFP3800_PL_PluMisc_PluMasterReset_TSTempAlrtToPClkPD BIT(21)
#define   NFP3800_PL_PluMisc_PluMasterReset_TSTempAlrtToTClkPD BIT(20)
#define   NFP3800_PL_PluMisc_PluMasterReset_TSTempAlrtToClkNResetOut BIT(19)
#define   NFP3800_PL_PluMisc_PluMasterReset_TSTempAlrtToSoftReset BIT(18)
#define   NFP3800_PL_PluMisc_PluMasterReset_TSTempTripToMClkPD BIT(17)
#define   NFP3800_PL_PluMisc_PluMasterReset_TSTempTripToAClkPD BIT(16)
#define   NFP3800_PL_PluMisc_PluMasterReset_TSTempTripToPClkPD BIT(15)
#define   NFP3800_PL_PluMisc_PluMasterReset_TSTempTripToTClkPD BIT(14)
#define   NFP3800_PL_PluMisc_PluMasterReset_TSTempTripToClkNResetOut BIT(13)
#define   NFP3800_PL_PluMisc_PluMasterReset_TSTempTripToSoftReset BIT(12)
#define   NFP3800_PL_PluMisc_PluMasterReset_PCIE0_RESET_MASK BIT(8)
#define   NFP3800_PL_PluMisc_PluMasterReset_PCIE0_RESET_OUT_L BIT(4)
#define   NFP3800_PL_PluMisc_PluMasterReset_SResetToClkNResetOut BIT(3)
#define   NFP3800_PL_PluMisc_PluMasterReset_ProgSResetToClkNResetOut BIT(2)
#define   NFP3800_PL_PluMisc_PluMasterReset_PROG_SRESET_L    BIT(1)
#define   NFP3800_PL_PluMisc_PluMasterReset_CLK_NRESET_OUT_L BIT(0)
/*----------------------------------------------------------------
  Register: PluMisc_PluDeviceID - Chip Device ID Register.
    Bits: 31:16 PartNumber(ro) - Major Part Number Designation (Hard Coded)
    Bits: 15:8  SKU(ro) - SKU Designation (From Fuses)
    Bits:  7:4  MajorRevID(ro) - Major Revision ID (Hard Coded)
    Bits:  3:0  MinorRevID(ro) - Minor Revision ID (From Fuses or Special Metal Structure)
*/
#define NFP3800_PL_PluMisc_PluDeviceID                       0x00000004
#define   NFP3800_PL_PluMisc_PluDeviceID_PartNumber(_x)      (((_x) & 0xffff) << 16)
#define   NFP3800_PL_PluMisc_PluDeviceID_PartNumber_of(_x)   (((_x) >> 16) & 0xffff)
#define   NFP3800_PL_PluMisc_PluDeviceID_SKU(_x)             (((_x) & 0xff) << 8)
#define   NFP3800_PL_PluMisc_PluDeviceID_SKU_of(_x)          (((_x) >> 8) & 0xff)
#define   NFP3800_PL_PluMisc_PluDeviceID_MajorRevID(_x)      (((_x) & 0xf) << 4)
#define   NFP3800_PL_PluMisc_PluDeviceID_MajorRevID_of(_x)   (((_x) >> 4) & 0xf)
#define   NFP3800_PL_PluMisc_PluDeviceID_MinorRevID(_x)      (((_x) & 0xf) << 0)
#define   NFP3800_PL_PluMisc_PluDeviceID_MinorRevID_of(_x)   (((_x) >> 0) & 0xf)
/*----------------------------------------------------------------
  Register: PluMisc_PluPowerState - Indicators for Power and Power-Domain Status
    Bits:  2    CRYPTO_PDL(ro) - Crypto Power Domain Locked
    Bits:  1    POWERGOOD_IO(ro) - IO Power Domain Power Good Indicator.
    Bits:  0    POWERGOOD_CORE(ro) - Core Power Domain Power Good Indicator.
*/
#define NFP3800_PL_PluMisc_PluPowerState                     0x0000000c
#define   NFP3800_PL_PluMisc_PluPowerState_CRYPTO_PDL        BIT(2)
#define   NFP3800_PL_PluMisc_PluPowerState_POWERGOOD_IO      BIT(1)
#define   NFP3800_PL_PluMisc_PluPowerState_POWERGOOD_CORE    BIT(0)
/*----------------------------------------------------------------
  Register: PluMisc_PluBaudRateGen - Control Register for Baud Rate Generator.
    Bits: 31:16 BaudControlA(rw) - Control A for Baud Rate Generator. See Baud Rate Generator Table
    Bits: 15:0  BaudControlB(rw) - Control B for Baud Rate Generator. See Baud Rate Generator Table
*/
#define NFP3800_PL_PluMisc_PluBaudRateGen                    0x00000018
#define   NFP3800_PL_PluMisc_PluBaudRateGen_BaudControlA(_x) (((_x) & 0xffff) << 16)
#define   NFP3800_PL_PluMisc_PluBaudRateGen_BaudControlA_of(_x) (((_x) >> 16) & 0xffff)
#define   NFP3800_PL_PluMisc_PluBaudRateGen_BaudControlB(_x) (((_x) & 0xffff) << 0)
#define   NFP3800_PL_PluMisc_PluBaudRateGen_BaudControlB_of(_x) (((_x) >> 0) & 0xffff)
/*----------------------------------------------------------------
  Register: PluMisc_PluMiscClockDivControl - Miscellaneous Clock Divider Control
    Bits: 23:16 RefClkDiv10DivRatio(rw) - REF_CLK divider ratio to internally generate REF_CLK_DIV10 solely for Power Switches. Legal values: 0-255
    Bits: 15:8  PCIEPwrMgmtClkDivRatio(rw) - REF_CLK divider ratio to internally generate PCIE_PWR_MGMT_CLK. Legal values: 0-255
    Bits:  7:0  PClkDiv4DivRatio(rw) - P_CLK divider ratio to internally generate P_CLK_DIV4. Legal values: 0-255
*/
#define NFP3800_PL_PluMisc_PluMiscClockDivControl            0x00000034
#define   NFP3800_PL_PluMisc_PluMiscClockDivControl_RefClkDiv10DivRatio(_x) (((_x) & 0xff) << 16)
#define   NFP3800_PL_PluMisc_PluMiscClockDivControl_RefClkDiv10DivRatio_of(_x) (((_x) >> 16) & 0xff)
#define     NFP3800_PL_PluMisc_PluMiscClockDivControl_RefClkDiv10DivRatio_OFF (0)
#define     NFP3800_PL_PluMisc_PluMiscClockDivControl_RefClkDiv10DivRatio_BYP (1)
#define   NFP3800_PL_PluMisc_PluMiscClockDivControl_PCIEPwrMgmtClkDivRatio(_x) (((_x) & 0xff) << 8)
#define   NFP3800_PL_PluMisc_PluMiscClockDivControl_PCIEPwrMgmtClkDivRatio_of(_x) (((_x) >> 8) & 0xff)
#define     NFP3800_PL_PluMisc_PluMiscClockDivControl_PCIEPwrMgmtClkDivRatio_OFF (0)
#define     NFP3800_PL_PluMisc_PluMiscClockDivControl_PCIEPwrMgmtClkDivRatio_BYP (1)
#define   NFP3800_PL_PluMisc_PluMiscClockDivControl_PClkDiv4DivRatio(_x) (((_x) & 0xff) << 0)
#define   NFP3800_PL_PluMisc_PluMiscClockDivControl_PClkDiv4DivRatio_of(_x) (((_x) >> 0) & 0xff)
#define     NFP3800_PL_PluMisc_PluMiscClockDivControl_PClkDiv4DivRatio_OFF (0)
#define     NFP3800_PL_PluMisc_PluMiscClockDivControl_PClkDiv4DivRatio_BYP (1)
/*----------------------------------------------------------------
  Register: PluMisc_PluJtagTckControl - JTAG_TCK Internal Generation Control
    Bits:  7:0  ClkDivRatio(rw) - REF_CLK divider ratio to internally generate JTAG_TCK. Legal values: 0-255
*/
#define NFP3800_PL_PluMisc_PluJtagTckControl                 0x00000038
#define   NFP3800_PL_PluMisc_PluJtagTckControl_ClkDivRatio(_x) (((_x) & 0xff) << 0)
#define   NFP3800_PL_PluMisc_PluJtagTckControl_ClkDivRatio_of(_x) (((_x) >> 0) & 0xff)
#define     NFP3800_PL_PluMisc_PluJtagTckControl_ClkDivRatio_OFF (0)
#define     NFP3800_PL_PluMisc_PluJtagTckControl_ClkDivRatio_BYP (1)
/*----------------------------------------------------------------
  Register: PluMisc_PluSMClockControl - SM CLock Internal Generation Control
    Bits:  8    ClkSource(rw) - When 0, Clock Divider input is REF_CLK; when 1, MAC0_CLK PLL Output.
    Bits:  7:0  ClkDivRatio(rw) - REF_CLK divider ratio to internally generate SM_CLK. Legal values: 0-255
*/
#define NFP3800_PL_PluMisc_PluSMClockControl                 0x0000003c
#define   NFP3800_PL_PluMisc_PluSMClockControl_ClkSource     BIT(8)
#define   NFP3800_PL_PluMisc_PluSMClockControl_ClkDivRatio(_x) (((_x) & 0xff) << 0)
#define   NFP3800_PL_PluMisc_PluSMClockControl_ClkDivRatio_of(_x) (((_x) >> 0) & 0xff)
#define     NFP3800_PL_PluMisc_PluSMClockControl_ClkDivRatio_OFF (0)
#define     NFP3800_PL_PluMisc_PluSMClockControl_ClkDivRatio_BYP (1)
/*----------------------------------------------------------------
  Register: PluMisc_PluChipSerialNumber - Per Chip Serial Number
    Bits: 31:0  ChipSerialNumber(ro) - Per Chip Serial Number
*/
#define NFP3800_PL_PluMisc_PluChipSerialNumber               0x00000040
#define   NFP3800_PL_PluMisc_PluChipSerialNumber_ChipSerialNumber(_x) (((_x) & 0xffffffff) << 0)
#define   NFP3800_PL_PluMisc_PluChipSerialNumber_ChipSerialNumber_of(_x) (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: PluMisc_PluMacClockDiv4Control - MACCLK Divider Control
    Bits:  7:0  ClkDivRatio(rw) - MAC_CLK divider ratio to generate MAC0_CLK_DIV4. Legal values: 0-255
*/
#define NFP3800_PL_PluMisc_PluMacClockDiv4Control            0x00000050
#define   NFP3800_PL_PluMisc_PluMacClockDiv4Control_ClkDivRatio(_x) (((_x) & 0xff) << 0)
#define   NFP3800_PL_PluMisc_PluMacClockDiv4Control_ClkDivRatio_of(_x) (((_x) >> 0) & 0xff)
#define     NFP3800_PL_PluMisc_PluMacClockDiv4Control_ClkDivRatio_OFF (0)
#define     NFP3800_PL_PluMisc_PluMacClockDiv4Control_ClkDivRatio_BYP (1)
/*----------------------------------------------------------------
  Register: PluTSXpb_PluTSRegIntfc - Thermal Sensor Register Interface
    Bits: 31:30 Start(ro) - Start bits for Thermal Sensor Register Interface Frame
    Bits: 29:28 Op(rw) - OpCode for Thermal Sensor Register Interface
    Bits: 27:18 RegAddr(rw) - Register Address for Thermal Sensor Register Interface
    Bits: 15:0  Data(rw) - Register Write/Read Data for Thermal Sensor Register Interface
*/
#define NFP3800_PL_PluTSXpb_PluTSRegIntfc                    0x00010000
#define   NFP3800_PL_PluTSXpb_PluTSRegIntfc_Start(_x)        (((_x) & 0x3) << 30)
#define   NFP3800_PL_PluTSXpb_PluTSRegIntfc_Start_of(_x)     (((_x) >> 30) & 0x3)
#define   NFP3800_PL_PluTSXpb_PluTSRegIntfc_Op(_x)           (((_x) & 0x3) << 28)
#define   NFP3800_PL_PluTSXpb_PluTSRegIntfc_Op_of(_x)        (((_x) >> 28) & 0x3)
#define     NFP3800_PL_PluTSXpb_PluTSRegIntfc_Op_NoOp        (0)
#define     NFP3800_PL_PluTSXpb_PluTSRegIntfc_Op_Write       (1)
#define   NFP3800_PL_PluTSXpb_PluTSRegIntfc_RegAddr(_x)      (((_x) & 0x3ff) << 18)
#define   NFP3800_PL_PluTSXpb_PluTSRegIntfc_RegAddr_of(_x)   (((_x) >> 18) & 0x3ff)
#define   NFP3800_PL_PluTSXpb_PluTSRegIntfc_Data(_x)         (((_x) & 0xffff) << 0)
#define   NFP3800_PL_PluTSXpb_PluTSRegIntfc_Data_of(_x)      (((_x) >> 0) & 0xffff)
/*----------------------------------------------------------------
  Register: PluTSXpb_PluTSReadings - Filtered Thermal Sensor Readings. (HIGHLY RECOMMENDED)
    Bits: 14    TSotstemperaturetrip(ro) - Temperature catastrophic Trip signal based on the threshold set by TSTripThreshold; Temperature must rise above TSTripThreshold for Trip to assert. Trip will remain asserted until the TSTripThreshold is disabled and re-enabled.
    Bits: 13    TSotstemperaturealrt(ro) - Temperature Alert signal based on the thresholds set by TSAlertThreshold and TSAlertVariation; Temperature must rise above Threshold for Alert to assert; Temperature must fall below TSAlertThreshold-TSAlertVariation for Alert to de-assert.
    Bits: 12    TSotstemperaturevalid(ro) - Temperature Reading has been Updated and is Valid. When this register is read, this bit will be cleared until the next temperature reading arrives.
    Bits: 11:0  TSotstemperature(ro) - 12 bit temperature reading in degC
*/
#define NFP3800_PL_PluTSXpb_PluTSReadings                    0x00010004
#define   NFP3800_PL_PluTSXpb_PluTSReadings_TSotstemperaturetrip BIT(14)
#define   NFP3800_PL_PluTSXpb_PluTSReadings_TSotstemperaturealrt BIT(13)
#define   NFP3800_PL_PluTSXpb_PluTSReadings_TSotstemperaturevalid BIT(12)
#define   NFP3800_PL_PluTSXpb_PluTSReadings_TSotstemperature(_x) (((_x) & 0xfff) << 0)
#define   NFP3800_PL_PluTSXpb_PluTSReadings_TSotstemperature_of(_x) (((_x) >> 0) & 0xfff)
/*----------------------------------------------------------------
  Register: PluTSXpb_PluTSTemperatureTrip - Thermal Sensor Catastrophic Trip Control Register
    Bits: 31    TSTripThresholdEnable(rw) - Must be set to 1 for the TSTripThreshold to be checked
    Bits: 30    TSTripThresholdSelect(rw) - Selects the trip comparison to be used to generate an trigger for the reset logic.
    Bits: 11:0  TSTripThreshold(rw) - 12 bit temperature Catastrophic Trip Threshold.
*/
#define NFP3800_PL_PluTSXpb_PluTSTemperatureTrip             0x00010008
#define   NFP3800_PL_PluTSXpb_PluTSTemperatureTrip_TSTripThresholdEnable BIT(31)
#define   NFP3800_PL_PluTSXpb_PluTSTemperatureTrip_TSTripThresholdSelect BIT(30)
#define     NFP3800_PL_PluTSXpb_PluTSTemperatureTrip_TSTripThresholdSelect_FILTERED (0 << 30)
#define     NFP3800_PL_PluTSXpb_PluTSTemperatureTrip_TSTripThresholdSelect_RAW BIT(30)
#define   NFP3800_PL_PluTSXpb_PluTSTemperatureTrip_TSTripThreshold(_x) (((_x) & 0xfff) << 0)
#define   NFP3800_PL_PluTSXpb_PluTSTemperatureTrip_TSTripThreshold_of(_x) (((_x) >> 0) & 0xfff)
/*----------------------------------------------------------------
  Register: PluTSXpb_PluTSTemperatureAlert - Thermal Sensor Alert (w/hysteresis) Control Register
    Bits: 31    TSAlertThresholdEnable(rw) - Must be set to 1 for the TSAlertThreshold to be checked
    Bits: 30    TSAlertThresholdSelect(rw) - Selects the alert comparison to be used to generate an trigger for the reset logic
    Bits: 27:16 TSAlertVariation(rw) - 12 bit temperature Alert Variation. Provides Hysteresis. Temperature must fall below Threshold-Variation for Alert to de-assert.
    Bits: 11:0  TSAlertThreshold(rw) - 12 bit temperature Alert Threshold. Temperature must rise above Threshold for Alert to assert
*/
#define NFP3800_PL_PluTSXpb_PluTSTemperatureAlert            0x0001000c
#define   NFP3800_PL_PluTSXpb_PluTSTemperatureAlert_TSAlertThresholdEnable BIT(31)
#define   NFP3800_PL_PluTSXpb_PluTSTemperatureAlert_TSAlertThresholdSelect BIT(30)
#define     NFP3800_PL_PluTSXpb_PluTSTemperatureAlert_TSAlertThresholdSelect_FILTERED (0 << 30)
#define     NFP3800_PL_PluTSXpb_PluTSTemperatureAlert_TSAlertThresholdSelect_RAW BIT(30)
#define   NFP3800_PL_PluTSXpb_PluTSTemperatureAlert_TSAlertVariation(_x) (((_x) & 0xfff) << 16)
#define   NFP3800_PL_PluTSXpb_PluTSTemperatureAlert_TSAlertVariation_of(_x) (((_x) >> 16) & 0xfff)
#define   NFP3800_PL_PluTSXpb_PluTSTemperatureAlert_TSAlertThreshold(_x) (((_x) & 0xfff) << 0)
#define   NFP3800_PL_PluTSXpb_PluTSTemperatureAlert_TSAlertThreshold_of(_x) (((_x) >> 0) & 0xfff)
/*----------------------------------------------------------------
  Register: PluTSXpb_PluTSRawReadings - Raw Thermal Sensor Readings fed directly from thermal sensor. Readings will be noisy. (NOT RECOMMENDED)
    Bits: 27:16 TSFilterOutput(ro) - 12 bit Noise Filter output. (Value/8)-50 = degC
    Bits: 14    TSotstemperaturetrip(ro) - Temperature catastrophic Trip signal based on the threshold set by TSTripThreshold; Temperature must rise above TSTripThreshold for Trip to assert. Trip will remain asserted until the TSTripThreshold is disabled and re-enabled.
    Bits: 13    TSotstemperaturealrt(ro) - Temperature Alert signal based on the thresholds set by TSAlertThreshold and TSAlertVariation; Temperature must rise above Threshold for Alert to assert; Temperature must fall below TSAlertThreshold-TSAlertVariation for Alert to de-assert.
    Bits: 12    TSotstemperaturevalid(ro) - Temperature Reading has been Updated and is Valid. When this register is read, this bit will be cleared until the next temperature reading arrives.
    Bits: 11:0  TSotstemperature(ro) - 12 bit temperature reading. (Value/8)-50 = degC
*/
#define NFP3800_PL_PluTSXpb_PluTSRawReadings                 0x00010010
#define   NFP3800_PL_PluTSXpb_PluTSRawReadings_TSFilterOutput(_x) (((_x) & 0xfff) << 16)
#define   NFP3800_PL_PluTSXpb_PluTSRawReadings_TSFilterOutput_of(_x) (((_x) >> 16) & 0xfff)
#define   NFP3800_PL_PluTSXpb_PluTSRawReadings_TSotstemperaturetrip BIT(14)
#define   NFP3800_PL_PluTSXpb_PluTSRawReadings_TSotstemperaturealrt BIT(13)
#define   NFP3800_PL_PluTSXpb_PluTSRawReadings_TSotstemperaturevalid BIT(12)
#define   NFP3800_PL_PluTSXpb_PluTSRawReadings_TSotstemperature(_x) (((_x) & 0xfff) << 0)
#define   NFP3800_PL_PluTSXpb_PluTSRawReadings_TSotstemperature_of(_x) (((_x) >> 0) & 0xfff)
/*----------------------------------------------------------------
  Register: PLL0Controls_PLLControlAndStatus - Primary Control and Status Register
  Register: PLL1Controls_PLLControlAndStatus - Primary Control and Status Register
  Register: PLL2Controls_PLLControlAndStatus - Primary Control and Status Register
  Register: PLL3Controls_PLLControlAndStatus - Primary Control and Status Register
  Register: PLL4Controls_PLLControlAndStatus - Primary Control and Status Register
    Bits: 31    PLLPd(rw) - FSM Control: 1=Normal Power-Up 0=PowerDown. When transitioned to 1, the PLL State Machine will execute an automatic PLL bringup sequence and lockout rights to all but this registers and the Manual Override configuration register.
    Bits: 24    PLLAutoReLock(rw) - Mode bit to select behavior upon the occurence of a Lock Fault
    Bits: 23:8  PLLLockFaultCount(ro) - Once the PLL has been spun-up, counts how many times that lock is lost. This field will be cleared whenever PLLPd transitions from 0 to 1.
    Bits:  7    PLLLockFault(ro) - Indicates that the PLL has lost lock during normal operation. This field will be cleared whenever PLLPd transitions from 0 to 1.
    Bits:  6    PLLLockOut(ro) - PLL Lock Indicator
    Bits:  5    PLLLockDetectTimeOut(ro) - PLL Lock Timeout Indicator
    Bits:  4    PLLStartLockDetect(ro) - FSM Output: Start Lock Detect
    Bits:  1    PLLByPass(ro) - FSM Output: Bypass the PLL, putting REFCLK on PLLOUT
    Bits:  0    PLLWrapOutByPass(ro) - FSM Output: Bypass the PLL with REFCLK outside the PLL
*/
#define NFP3800_PL_PLL0Controls_PLLControlAndStatus          0x00020000
#define NFP3800_PL_PLL1Controls_PLLControlAndStatus          0x00030000
#define NFP3800_PL_PLL2Controls_PLLControlAndStatus          0x00060000
#define NFP3800_PL_PLL3Controls_PLLControlAndStatus          0x00070000
#define NFP3800_PL_PLL4Controls_PLLControlAndStatus          0x00080000
#define   NFP3800_PL_PLL0Controls_PLLControlAndStatus_PLLPd  BIT(31)
#define   NFP3800_PL_PLL0Controls_PLLControlAndStatus_PLLAutoReLock BIT(24)
#define     NFP3800_PL_PLL0Controls_PLLControlAndStatus_PLLAutoReLock_IDLE (0 << 24)
#define     NFP3800_PL_PLL0Controls_PLLControlAndStatus_PLLAutoReLock_RELOCK BIT(24)
#define   NFP3800_PL_PLL0Controls_PLLControlAndStatus_PLLLockFaultCount(_x) (((_x) & 0xffff) << 8)
#define   NFP3800_PL_PLL0Controls_PLLControlAndStatus_PLLLockFaultCount_of(_x) (((_x) >> 8) & 0xffff)
#define   NFP3800_PL_PLL0Controls_PLLControlAndStatus_PLLLockFault BIT(7)
#define   NFP3800_PL_PLL0Controls_PLLControlAndStatus_PLLLockOut BIT(6)
#define   NFP3800_PL_PLL0Controls_PLLControlAndStatus_PLLLockDetectTimeOut BIT(5)
#define   NFP3800_PL_PLL0Controls_PLLControlAndStatus_PLLStartLockDetect BIT(4)
#define   NFP3800_PL_PLL0Controls_PLLControlAndStatus_PLLByPass BIT(1)
#define   NFP3800_PL_PLL0Controls_PLLControlAndStatus_PLLWrapOutByPass BIT(0)
/*----------------------------------------------------------------
  Register: PLL0Controls_PLLConfig - Configuration Settings for the PLL
  Register: PLL1Controls_PLLConfig - Configuration Settings for the PLL
  Register: PLL2Controls_PLLConfig - Configuration Settings for the PLL
  Register: PLL3Controls_PLLConfig - Configuration Settings for the PLL
  Register: PLL4Controls_PLLConfig - Configuration Settings for the PLL
    Bits: 31    PLLLockMult(rw) - Write 1 to Set, cannot be cleared without hard_reset. Setting the lock bit captures the current values of the multipliers as limits for all future settings
    Bits: 26:24 PLLRefDiv(rw) - Reference clock input divider ratio: 1-7; 0 also indicates 1. (div_ctrl<6:4> in PLL Spec)
    Bits: 23:16 PLLMDiv(rw) - PLL feedback divider value: 1-255; 0 also indicates 1. (DIV[7:0] in PLL Spec)
    Bits:  4:3  PLLOutDivMult(rw) - Multiplier for Output Divider. This means that the achievable Output Dividers are 1,2,3,4,5,6,7,8,12,16,20,24,28,32,40,48,56. WARNING: Only Output Dividers 1,2,4,8,16,32 produce an ~50% duty cycle, all others should be used with extreme caution! (div_ctrl<1:0> in PLL Spec)
    Bits:  2:0  PLLOutDiv(rw) - Mutiplicand of PLL Output Divider. This value times PLLOutDivMult determines the Output Divider. WARNING: Only Output Dividers 1,2,4,8,16,32 produce an ~50% duty cycle, all others should be used with extreme caution! (OUTDIV<2:0> in PLL Spec)
*/
#define NFP3800_PL_PLL0Controls_PLLConfig                    0x00020004
#define NFP3800_PL_PLL1Controls_PLLConfig                    0x00030004
#define NFP3800_PL_PLL2Controls_PLLConfig                    0x00060004
#define NFP3800_PL_PLL3Controls_PLLConfig                    0x00070004
#define NFP3800_PL_PLL4Controls_PLLConfig                    0x00080004
#define   NFP3800_PL_PLL0Controls_PLLConfig_PLLLockMult      BIT(31)
#define   NFP3800_PL_PLL0Controls_PLLConfig_PLLRefDiv(_x)    (((_x) & 0x7) << 24)
#define   NFP3800_PL_PLL0Controls_PLLConfig_PLLRefDiv_of(_x) (((_x) >> 24) & 0x7)
#define   NFP3800_PL_PLL0Controls_PLLConfig_PLLMDiv(_x)      (((_x) & 0xff) << 16)
#define   NFP3800_PL_PLL0Controls_PLLConfig_PLLMDiv_of(_x)   (((_x) >> 16) & 0xff)
#define   NFP3800_PL_PLL0Controls_PLLConfig_PLLOutDivMult(_x) (((_x) & 0x3) << 3)
#define   NFP3800_PL_PLL0Controls_PLLConfig_PLLOutDivMult_of(_x) (((_x) >> 3) & 0x3)
#define     NFP3800_PL_PLL0Controls_PLLConfig_PLLOutDivMult_x1 (0)
#define     NFP3800_PL_PLL0Controls_PLLConfig_PLLOutDivMult_x4 (1)
#define     NFP3800_PL_PLL0Controls_PLLConfig_PLLOutDivMult_x8 (2)
#define   NFP3800_PL_PLL0Controls_PLLConfig_PLLOutDiv(_x)    (((_x) & 0x7) << 0)
#define   NFP3800_PL_PLL0Controls_PLLConfig_PLLOutDiv_of(_x) (((_x) >> 0) & 0x7)
/*----------------------------------------------------------------
  Register: PLL0Controls_PLLMiscControls - Misc Tuning and Configuration Settings
  Register: PLL1Controls_PLLMiscControls - Misc Tuning and Configuration Settings
  Register: PLL2Controls_PLLMiscControls - Misc Tuning and Configuration Settings
  Register: PLL3Controls_PLLMiscControls - Misc Tuning and Configuration Settings
  Register: PLL4Controls_PLLMiscControls - Misc Tuning and Configuration Settings
    Bits: 31:16 PLLTimeOutCountVal(rw) - If PLLLockOut is not asserted before the time-out counter expires, PLLLockDetectTimeOut is asserted. This value must be higher than 7x PLLLockDetectCount.
    Bits: 15    misc_ctrl_7(rw) - Control Lock Detector. (misc_ctrl<7> in PLL Spec)
    Bits: 14:13 PLLLoopFilter(rw) - Controls Loop Filter. (pll_reg_in<4:3> in PLL Spec)
    Bits: 12:10 PLLChargePump(rw) - Controls Charge Pump. (pll_reg_in<2:0> in PLL Spec)
    Bits:  9:6  PLLTest(rw) - Test control pins to observe VCO, VCTRL, VBGR at TESTPAD. (pll_reg_in<15:12> in PLL Spec)
    Bits:  5:3  PLLVCO(rw) - Controls VCO. (pll_reg_in<7:5> in PLL Spec)
    Bits:  2:0  PLLResCtrl(rw) - Controls Bias Gen (res_ctrl<2:0> in PLL Spec)
*/
#define NFP3800_PL_PLL0Controls_PLLMiscControls              0x0002000c
#define NFP3800_PL_PLL1Controls_PLLMiscControls              0x0003000c
#define NFP3800_PL_PLL2Controls_PLLMiscControls              0x0006000c
#define NFP3800_PL_PLL3Controls_PLLMiscControls              0x0007000c
#define NFP3800_PL_PLL4Controls_PLLMiscControls              0x0008000c
#define   NFP3800_PL_PLL0Controls_PLLMiscControls_PLLTimeOutCountVal(_x) (((_x) & 0xffff) << 16)
#define   NFP3800_PL_PLL0Controls_PLLMiscControls_PLLTimeOutCountVal_of(_x) (((_x) >> 16) & 0xffff)
#define   NFP3800_PL_PLL0Controls_PLLMiscControls_misc_ctrl_7 BIT(15)
#define   NFP3800_PL_PLL0Controls_PLLMiscControls_PLLLoopFilter(_x) (((_x) & 0x3) << 13)
#define   NFP3800_PL_PLL0Controls_PLLMiscControls_PLLLoopFilter_of(_x) (((_x) >> 13) & 0x3)
#define   NFP3800_PL_PLL0Controls_PLLMiscControls_PLLChargePump(_x) (((_x) & 0x7) << 10)
#define   NFP3800_PL_PLL0Controls_PLLMiscControls_PLLChargePump_of(_x) (((_x) >> 10) & 0x7)
#define   NFP3800_PL_PLL0Controls_PLLMiscControls_PLLTest(_x) (((_x) & 0xf) << 6)
#define   NFP3800_PL_PLL0Controls_PLLMiscControls_PLLTest_of(_x) (((_x) >> 6) & 0xf)
#define   NFP3800_PL_PLL0Controls_PLLMiscControls_PLLVCO(_x) (((_x) & 0x7) << 3)
#define   NFP3800_PL_PLL0Controls_PLLMiscControls_PLLVCO_of(_x) (((_x) >> 3) & 0x7)
#define   NFP3800_PL_PLL0Controls_PLLMiscControls_PLLResCtrl(_x) (((_x) & 0x7) << 0)
#define   NFP3800_PL_PLL0Controls_PLLMiscControls_PLLResCtrl_of(_x) (((_x) >> 0) & 0x7)
/*----------------------------------------------------------------
  Register: PLL0Controls_PLLManualOverride - Manual Override Register
  Register: PLL1Controls_PLLManualOverride - Manual Override Register
  Register: PLL2Controls_PLLManualOverride - Manual Override Register
  Register: PLL3Controls_PLLManualOverride - Manual Override Register
  Register: PLL4Controls_PLLManualOverride - Manual Override Register
    Bits: 31    PLLManualOverride(rw) - Setting this bit to 1 enables manual override control for the PLLs. This takes control away from the PLL State Machine. This will also enable writes to the other config regs that would normally be blocked by the PLL State Machine
    Bits: 30    OvdPLLPd(rw) - Manual Override: PLL Power Down Mode: 1=Normal 0=PowerDown
    Bits: 29    OvdPLLLockResetB(rw) - Manual Override: Reset mode: 1=Normal, 0=Reset
    Bits:  2    OvdPLLStartLockDetect(rw) - Manual Override: Signals the start for lock detector operation. If this signal is never asserted, LOCK_OUT will never be asserted.
    Bits:  1    OvdPLLByPass(rw) - Manual Override: Bypass the PLL, putting REFCLK on PLLOUT.
    Bits:  0    OvdPLLWrapOutByPass(rw) - Manual Override: Bypass the PLL with REFCLK outside the PLL
*/
#define NFP3800_PL_PLL0Controls_PLLManualOverride            0x00020010
#define NFP3800_PL_PLL1Controls_PLLManualOverride            0x00030010
#define NFP3800_PL_PLL2Controls_PLLManualOverride            0x00060010
#define NFP3800_PL_PLL3Controls_PLLManualOverride            0x00070010
#define NFP3800_PL_PLL4Controls_PLLManualOverride            0x00080010
#define   NFP3800_PL_PLL0Controls_PLLManualOverride_PLLManualOverride BIT(31)
#define   NFP3800_PL_PLL0Controls_PLLManualOverride_OvdPLLPd BIT(30)
#define   NFP3800_PL_PLL0Controls_PLLManualOverride_OvdPLLLockResetB BIT(29)
#define   NFP3800_PL_PLL0Controls_PLLManualOverride_OvdPLLStartLockDetect BIT(2)
#define   NFP3800_PL_PLL0Controls_PLLManualOverride_OvdPLLByPass BIT(1)
#define   NFP3800_PL_PLL0Controls_PLLManualOverride_OvdPLLWrapOutByPass BIT(0)
/*----------------------------------------------------------------
  Register: PLL0Controls_PLLAuxDivider - Auxilliary Clock Divider
  Register: PLL1Controls_PLLAuxDivider - Auxilliary Clock Divider
  Register: PLL2Controls_PLLAuxDivider - Auxilliary Clock Divider
  Register: PLL3Controls_PLLAuxDivider - Auxilliary Clock Divider
  Register: PLL4Controls_PLLAuxDivider - Auxilliary Clock Divider
    Bits:  7:0  PLLAuxDivider(rw) - Additional Clock Divider Ratio: The auxilliary divider will produce a 50% duty cycle for all divide ratios as long as the PLL output feeding it has a 50% duty cycle. This divider cannot be used with a PLL output frequency faster than TBD MHz.
*/
#define NFP3800_PL_PLL0Controls_PLLAuxDivider                0x00020014
#define NFP3800_PL_PLL1Controls_PLLAuxDivider                0x00030014
#define NFP3800_PL_PLL2Controls_PLLAuxDivider                0x00060014
#define NFP3800_PL_PLL3Controls_PLLAuxDivider                0x00070014
#define NFP3800_PL_PLL4Controls_PLLAuxDivider                0x00080014
#define   NFP3800_PL_PLL0Controls_PLLAuxDivider_PLLAuxDivider(_x) (((_x) & 0xff) << 0)
#define   NFP3800_PL_PLL0Controls_PLLAuxDivider_PLLAuxDivider_of(_x) (((_x) >> 0) & 0xff)
#define     NFP3800_PL_PLL0Controls_PLLAuxDivider_PLLAuxDivider_BYP (0)
/*----------------------------------------------------------------
  Register: PLL0Controls_PLLMiscControls2 - Misc Tuning and Configuration Settings; all the rest of the PLL pins.
  Register: PLL1Controls_PLLMiscControls2 - Misc Tuning and Configuration Settings; all the rest of the PLL pins.
  Register: PLL2Controls_PLLMiscControls2 - Misc Tuning and Configuration Settings; all the rest of the PLL pins.
  Register: PLL3Controls_PLLMiscControls2 - Misc Tuning and Configuration Settings; all the rest of the PLL pins.
  Register: PLL4Controls_PLLMiscControls2 - Misc Tuning and Configuration Settings; all the rest of the PLL pins.
    Bits: 24:23 pll_reg_in_9to8(rw) - (pll_reg_in<9:8> in PLL Spec)
    Bits: 22:17 pll_reg_in_22to17(rw) - (pll_reg_in<22:17> in PLL Spec)
    Bits: 16:15 unused_REFDIV(rw) - (REFDIV<1:0> in PLL Spec)
    Bits: 14    unused_div_ctrl_7(rw) - (div_ctrl<7> in PLL Spec)
    Bits: 13:12 unused_div_ctrl_3to2(rw) - (div_ctrl<3:2> in PLL Spec)
    Bits: 11    unused_RESETB(rw) - (RESETB in PLL Spec)
    Bits: 10    unused_Div_8(rw) - (Div<8> in PLL Spec)
    Bits:  9:3  unused_misc_ctrl(rw) - (misc_ctrl<6:0> in PLL Spec)
    Bits:  2:1  unused_pll_reg_in_11to10(rw) - (pll_reg_in<11:10> in PLL Spec)
    Bits:  0    unused_pll_reg_in_23(rw) - (pll_reg_in<23> in PLL Spec)
*/
#define NFP3800_PL_PLL0Controls_PLLMiscControls2             0x00020018
#define NFP3800_PL_PLL1Controls_PLLMiscControls2             0x00030018
#define NFP3800_PL_PLL2Controls_PLLMiscControls2             0x00060018
#define NFP3800_PL_PLL3Controls_PLLMiscControls2             0x00070018
#define NFP3800_PL_PLL4Controls_PLLMiscControls2             0x00080018
#define   NFP3800_PL_PLL0Controls_PLLMiscControls2_pll_reg_in_9to8(_x) (((_x) & 0x3) << 23)
#define   NFP3800_PL_PLL0Controls_PLLMiscControls2_pll_reg_in_9to8_of(_x) (((_x) >> 23) & 0x3)
#define   NFP3800_PL_PLL0Controls_PLLMiscControls2_pll_reg_in_22to17(_x) (((_x) & 0x3f) << 17)
#define   NFP3800_PL_PLL0Controls_PLLMiscControls2_pll_reg_in_22to17_of(_x) (((_x) >> 17) & 0x3f)
#define   NFP3800_PL_PLL0Controls_PLLMiscControls2_unused_REFDIV(_x) (((_x) & 0x3) << 15)
#define   NFP3800_PL_PLL0Controls_PLLMiscControls2_unused_REFDIV_of(_x) (((_x) >> 15) & 0x3)
#define   NFP3800_PL_PLL0Controls_PLLMiscControls2_unused_div_ctrl_7 BIT(14)
#define   NFP3800_PL_PLL0Controls_PLLMiscControls2_unused_div_ctrl_3to2(_x) (((_x) & 0x3) << 12)
#define   NFP3800_PL_PLL0Controls_PLLMiscControls2_unused_div_ctrl_3to2_of(_x) (((_x) >> 12) & 0x3)
#define   NFP3800_PL_PLL0Controls_PLLMiscControls2_unused_RESETB BIT(11)
#define   NFP3800_PL_PLL0Controls_PLLMiscControls2_unused_Div_8 BIT(10)
#define   NFP3800_PL_PLL0Controls_PLLMiscControls2_unused_misc_ctrl(_x) (((_x) & 0x7f) << 3)
#define   NFP3800_PL_PLL0Controls_PLLMiscControls2_unused_misc_ctrl_of(_x) (((_x) >> 3) & 0x7f)
#define   NFP3800_PL_PLL0Controls_PLLMiscControls2_unused_pll_reg_in_11to10(_x) (((_x) & 0x3) << 1)
#define   NFP3800_PL_PLL0Controls_PLLMiscControls2_unused_pll_reg_in_11to10_of(_x) (((_x) >> 1) & 0x3)
#define   NFP3800_PL_PLL0Controls_PLLMiscControls2_unused_pll_reg_in_23 BIT(0)
/*----------------------------------------------------------------
  Register: PluIOControls_PluLvcmos0IOCtrl - LVCMOS0 IO Control
    Bits: 27    clk_nreset_out_l_rcven(rw) - CLK_NRESET_OUT_L pin buffer input enable.
    Bits: 26    clk_nreset_out_l_oenb(rw) - CLK_NRESET_OUT_L pin output enable, active low.
    Bits: 25:24 clk_nreset_out_l_drv(rw) - CLK_NRESET_OUT_L pin drive strength setting.
    Bits: 23    pwrgood_core_wkpuen(rw) - RESERVED
    Bits: 22    pwrgood_core_wkpden(rw) - RESERVED
    Bits: 21    pwrgoodio_wkpuen(rw) - RESERVED
    Bits: 20    pwrgoodio_wkpden(rw) - RESERVED
    Bits: 19    sreset_l_wkpuen(rw) - RESERVED
    Bits: 18    sreset_l_wkpden(rw) - RESERVED
    Bits: 17    clk_nreset_l_wkpuen(rw) - RESERVED
    Bits: 16    clk_nreset_l_wkpden(rw) - RESERVED
    Bits: 11    clk_ref_out_rcven(rw) - Buffer input enable for CLK_REF_OUT pin.
    Bits: 10    clk_ref_out_oenb(rw) - CLK_REF_OUT pin output enable, active low.
    Bits:  9:8  clk_ref_out_drv(rw) - CLK_REF_OUT pin drive strength setting.
    Bits:  6    clk_ref_in_rcven(rw) - CLK_REF_IN pin buffer input enable.
    Bits:  5    clk_xtal_bypass(rw) - When set to 1, puts CLK_XTAL pad in bypass.
    Bits:  4    clk_xtal_gmsel_override(rw) - When set to 1, override GMSEL with the CLK_XTAL_GMSEL field in this register.
    Bits:  3:0  clk_xtal_gmsel(rw) - Gain control setting for crystal oscillator. Default 4'b1000
*/
#define NFP3800_PL_PluIOControls_PluLvcmos0IOCtrl            0x00040000
#define   NFP3800_PL_PluIOControls_PluLvcmos0IOCtrl_clk_nreset_out_l_rcven BIT(27)
#define   NFP3800_PL_PluIOControls_PluLvcmos0IOCtrl_clk_nreset_out_l_oenb BIT(26)
#define   NFP3800_PL_PluIOControls_PluLvcmos0IOCtrl_clk_nreset_out_l_drv(_x) (((_x) & 0x3) << 24)
#define   NFP3800_PL_PluIOControls_PluLvcmos0IOCtrl_clk_nreset_out_l_drv_of(_x) (((_x) >> 24) & 0x3)
#define     NFP3800_PL_PluIOControls_PluLvcmos0IOCtrl_clk_nreset_out_l_drv_1ma (0)
#define     NFP3800_PL_PluIOControls_PluLvcmos0IOCtrl_clk_nreset_out_l_drv_2ma (1)
#define     NFP3800_PL_PluIOControls_PluLvcmos0IOCtrl_clk_nreset_out_l_drv_4ma (2)
#define     NFP3800_PL_PluIOControls_PluLvcmos0IOCtrl_clk_nreset_out_l_drv_8ma (3)
#define   NFP3800_PL_PluIOControls_PluLvcmos0IOCtrl_pwrgood_core_wkpuen BIT(23)
#define   NFP3800_PL_PluIOControls_PluLvcmos0IOCtrl_pwrgood_core_wkpden BIT(22)
#define   NFP3800_PL_PluIOControls_PluLvcmos0IOCtrl_pwrgoodio_wkpuen BIT(21)
#define   NFP3800_PL_PluIOControls_PluLvcmos0IOCtrl_pwrgoodio_wkpden BIT(20)
#define   NFP3800_PL_PluIOControls_PluLvcmos0IOCtrl_sreset_l_wkpuen BIT(19)
#define   NFP3800_PL_PluIOControls_PluLvcmos0IOCtrl_sreset_l_wkpden BIT(18)
#define   NFP3800_PL_PluIOControls_PluLvcmos0IOCtrl_clk_nreset_l_wkpuen BIT(17)
#define   NFP3800_PL_PluIOControls_PluLvcmos0IOCtrl_clk_nreset_l_wkpden BIT(16)
#define   NFP3800_PL_PluIOControls_PluLvcmos0IOCtrl_clk_ref_out_rcven BIT(11)
#define   NFP3800_PL_PluIOControls_PluLvcmos0IOCtrl_clk_ref_out_oenb BIT(10)
#define   NFP3800_PL_PluIOControls_PluLvcmos0IOCtrl_clk_ref_out_drv(_x) (((_x) & 0x3) << 8)
#define   NFP3800_PL_PluIOControls_PluLvcmos0IOCtrl_clk_ref_out_drv_of(_x) (((_x) >> 8) & 0x3)
#define     NFP3800_PL_PluIOControls_PluLvcmos0IOCtrl_clk_ref_out_drv_1ma (0)
#define     NFP3800_PL_PluIOControls_PluLvcmos0IOCtrl_clk_ref_out_drv_2ma (1)
#define     NFP3800_PL_PluIOControls_PluLvcmos0IOCtrl_clk_ref_out_drv_4ma (2)
#define     NFP3800_PL_PluIOControls_PluLvcmos0IOCtrl_clk_ref_out_drv_8ma (3)
#define   NFP3800_PL_PluIOControls_PluLvcmos0IOCtrl_clk_ref_in_rcven BIT(6)
#define   NFP3800_PL_PluIOControls_PluLvcmos0IOCtrl_clk_xtal_bypass BIT(5)
#define   NFP3800_PL_PluIOControls_PluLvcmos0IOCtrl_clk_xtal_gmsel_override BIT(4)
#define   NFP3800_PL_PluIOControls_PluLvcmos0IOCtrl_clk_xtal_gmsel(_x) (((_x) & 0xf) << 0)
#define   NFP3800_PL_PluIOControls_PluLvcmos0IOCtrl_clk_xtal_gmsel_of(_x) (((_x) >> 0) & 0xf)
/*----------------------------------------------------------------
  Register: PluIOControls_GpioB0Drive - GPIO driver controls for GPIO[15:0].
    Bits: 31:30 gpio_15_drv(rw) - Output drive strength setting of GPIO[15].
    Bits: 29:28 gpio_14_drv(rw) - Output drive strength setting of GPIO[14].
    Bits: 27:26 gpio_13_drv(rw) - Output drive strength setting of GPIO[13].
    Bits: 25:24 gpio_12_drv(rw) - Output drive strength setting of GPIO[12].
    Bits: 23:22 gpio_11_drv(rw) - Output drive strength setting of GPIO[11].
    Bits: 21:20 gpio_10_drv(rw) - Output drive strength setting of GPIO[10].
    Bits: 19:18 gpio_9_drv(rw) - Output drive strength setting of GPIO[9].
    Bits: 17:16 gpio_8_drv(rw) - Output drive strength setting of GPIO[8].
    Bits: 15:14 gpio_7_drv(rw) - Output drive strength setting of GPIO[7].
    Bits: 13:12 gpio_6_drv(rw) - Output drive strength setting of GPIO[6].
    Bits: 11:10 gpio_5_drv(rw) - Output drive strength setting of GPIO[5].
    Bits:  9:8  gpio_4_drv(rw) - Output drive strength setting of GPIO[4].
    Bits:  7:6  gpio_3_drv(rw) - Output drive strength setting of GPIO[3].
    Bits:  5:4  gpio_2_drv(rw) - Output drive strength setting of GPIO[2].
    Bits:  3:2  gpio_1_drv(rw) - Output drive strength setting of GPIO[1].
    Bits:  1:0  gpio_0_drv(rw) - Output drive strength setting of GPIO[0].
*/
#define NFP3800_PL_PluIOControls_GpioB0Drive                 0x00040004
#define   NFP3800_PL_PluIOControls_GpioB0Drive_gpio_15_drv(_x) (((_x) & 0x3) << 30)
#define   NFP3800_PL_PluIOControls_GpioB0Drive_gpio_15_drv_of(_x) (((_x) >> 30) & 0x3)
#define   NFP3800_PL_PluIOControls_GpioB0Drive_gpio_14_drv(_x) (((_x) & 0x3) << 28)
#define   NFP3800_PL_PluIOControls_GpioB0Drive_gpio_14_drv_of(_x) (((_x) >> 28) & 0x3)
#define   NFP3800_PL_PluIOControls_GpioB0Drive_gpio_13_drv(_x) (((_x) & 0x3) << 26)
#define   NFP3800_PL_PluIOControls_GpioB0Drive_gpio_13_drv_of(_x) (((_x) >> 26) & 0x3)
#define   NFP3800_PL_PluIOControls_GpioB0Drive_gpio_12_drv(_x) (((_x) & 0x3) << 24)
#define   NFP3800_PL_PluIOControls_GpioB0Drive_gpio_12_drv_of(_x) (((_x) >> 24) & 0x3)
#define   NFP3800_PL_PluIOControls_GpioB0Drive_gpio_11_drv(_x) (((_x) & 0x3) << 22)
#define   NFP3800_PL_PluIOControls_GpioB0Drive_gpio_11_drv_of(_x) (((_x) >> 22) & 0x3)
#define   NFP3800_PL_PluIOControls_GpioB0Drive_gpio_10_drv(_x) (((_x) & 0x3) << 20)
#define   NFP3800_PL_PluIOControls_GpioB0Drive_gpio_10_drv_of(_x) (((_x) >> 20) & 0x3)
#define   NFP3800_PL_PluIOControls_GpioB0Drive_gpio_9_drv(_x) (((_x) & 0x3) << 18)
#define   NFP3800_PL_PluIOControls_GpioB0Drive_gpio_9_drv_of(_x) (((_x) >> 18) & 0x3)
#define   NFP3800_PL_PluIOControls_GpioB0Drive_gpio_8_drv(_x) (((_x) & 0x3) << 16)
#define   NFP3800_PL_PluIOControls_GpioB0Drive_gpio_8_drv_of(_x) (((_x) >> 16) & 0x3)
#define   NFP3800_PL_PluIOControls_GpioB0Drive_gpio_7_drv(_x) (((_x) & 0x3) << 14)
#define   NFP3800_PL_PluIOControls_GpioB0Drive_gpio_7_drv_of(_x) (((_x) >> 14) & 0x3)
#define   NFP3800_PL_PluIOControls_GpioB0Drive_gpio_6_drv(_x) (((_x) & 0x3) << 12)
#define   NFP3800_PL_PluIOControls_GpioB0Drive_gpio_6_drv_of(_x) (((_x) >> 12) & 0x3)
#define   NFP3800_PL_PluIOControls_GpioB0Drive_gpio_5_drv(_x) (((_x) & 0x3) << 10)
#define   NFP3800_PL_PluIOControls_GpioB0Drive_gpio_5_drv_of(_x) (((_x) >> 10) & 0x3)
#define   NFP3800_PL_PluIOControls_GpioB0Drive_gpio_4_drv(_x) (((_x) & 0x3) << 8)
#define   NFP3800_PL_PluIOControls_GpioB0Drive_gpio_4_drv_of(_x) (((_x) >> 8) & 0x3)
#define   NFP3800_PL_PluIOControls_GpioB0Drive_gpio_3_drv(_x) (((_x) & 0x3) << 6)
#define   NFP3800_PL_PluIOControls_GpioB0Drive_gpio_3_drv_of(_x) (((_x) >> 6) & 0x3)
#define   NFP3800_PL_PluIOControls_GpioB0Drive_gpio_2_drv(_x) (((_x) & 0x3) << 4)
#define   NFP3800_PL_PluIOControls_GpioB0Drive_gpio_2_drv_of(_x) (((_x) >> 4) & 0x3)
#define   NFP3800_PL_PluIOControls_GpioB0Drive_gpio_1_drv(_x) (((_x) & 0x3) << 2)
#define   NFP3800_PL_PluIOControls_GpioB0Drive_gpio_1_drv_of(_x) (((_x) >> 2) & 0x3)
#define   NFP3800_PL_PluIOControls_GpioB0Drive_gpio_0_drv(_x) (((_x) & 0x3) << 0)
#define   NFP3800_PL_PluIOControls_GpioB0Drive_gpio_0_drv_of(_x) (((_x) >> 0) & 0x3)
/*----------------------------------------------------------------
  Register: PluIOControls_GpioB1Drive - GPIO driver controls for GPIO[31:16].
    Bits: 31:30 gpio_31_drv(rw) - Output drive strength setting of GPIO[31].
    Bits: 29:28 gpio_30_drv(rw) - Output drive strength setting of GPIO[30].
    Bits: 27:26 gpio_29_drv(rw) - Output drive strength setting of GPIO[29].
    Bits: 25:24 gpio_28_drv(rw) - Output drive strength setting of GPIO[28].
    Bits: 23:22 gpio_27_drv(rw) - Output drive strength setting of GPIO[27].
    Bits: 21:20 gpio_26_drv(rw) - Output drive strength setting of GPIO[26].
    Bits: 19:18 gpio_25_drv(rw) - Output drive strength setting of GPIO[25].
    Bits: 17:16 gpio_24_drv(rw) - Output drive strength setting of GPIO[24].
    Bits: 15:14 gpio_23_drv(rw) - Output drive strength setting of GPIO[23].
    Bits: 13:12 gpio_22_drv(rw) - Output drive strength setting of GPIO[22].
    Bits: 11:10 gpio_21_drv(rw) - Output drive strength setting of GPIO[21].
    Bits:  9:8  gpio_20_drv(rw) - Output drive strength setting of GPIO[20].
    Bits:  7:6  gpio_19_drv(rw) - Output drive strength setting of GPIO[19].
    Bits:  5:4  gpio_18_drv(rw) - Output drive strength setting of GPIO[18].
    Bits:  3:2  gpio_17_drv(rw) - Output drive strength setting of GPIO[17].
    Bits:  1:0  gpio_16_drv(rw) - Output drive strength setting of GPIO[16].
*/
#define NFP3800_PL_PluIOControls_GpioB1Drive                 0x00040008
#define   NFP3800_PL_PluIOControls_GpioB1Drive_gpio_31_drv(_x) (((_x) & 0x3) << 30)
#define   NFP3800_PL_PluIOControls_GpioB1Drive_gpio_31_drv_of(_x) (((_x) >> 30) & 0x3)
#define   NFP3800_PL_PluIOControls_GpioB1Drive_gpio_30_drv(_x) (((_x) & 0x3) << 28)
#define   NFP3800_PL_PluIOControls_GpioB1Drive_gpio_30_drv_of(_x) (((_x) >> 28) & 0x3)
#define   NFP3800_PL_PluIOControls_GpioB1Drive_gpio_29_drv(_x) (((_x) & 0x3) << 26)
#define   NFP3800_PL_PluIOControls_GpioB1Drive_gpio_29_drv_of(_x) (((_x) >> 26) & 0x3)
#define   NFP3800_PL_PluIOControls_GpioB1Drive_gpio_28_drv(_x) (((_x) & 0x3) << 24)
#define   NFP3800_PL_PluIOControls_GpioB1Drive_gpio_28_drv_of(_x) (((_x) >> 24) & 0x3)
#define   NFP3800_PL_PluIOControls_GpioB1Drive_gpio_27_drv(_x) (((_x) & 0x3) << 22)
#define   NFP3800_PL_PluIOControls_GpioB1Drive_gpio_27_drv_of(_x) (((_x) >> 22) & 0x3)
#define   NFP3800_PL_PluIOControls_GpioB1Drive_gpio_26_drv(_x) (((_x) & 0x3) << 20)
#define   NFP3800_PL_PluIOControls_GpioB1Drive_gpio_26_drv_of(_x) (((_x) >> 20) & 0x3)
#define   NFP3800_PL_PluIOControls_GpioB1Drive_gpio_25_drv(_x) (((_x) & 0x3) << 18)
#define   NFP3800_PL_PluIOControls_GpioB1Drive_gpio_25_drv_of(_x) (((_x) >> 18) & 0x3)
#define   NFP3800_PL_PluIOControls_GpioB1Drive_gpio_24_drv(_x) (((_x) & 0x3) << 16)
#define   NFP3800_PL_PluIOControls_GpioB1Drive_gpio_24_drv_of(_x) (((_x) >> 16) & 0x3)
#define   NFP3800_PL_PluIOControls_GpioB1Drive_gpio_23_drv(_x) (((_x) & 0x3) << 14)
#define   NFP3800_PL_PluIOControls_GpioB1Drive_gpio_23_drv_of(_x) (((_x) >> 14) & 0x3)
#define   NFP3800_PL_PluIOControls_GpioB1Drive_gpio_22_drv(_x) (((_x) & 0x3) << 12)
#define   NFP3800_PL_PluIOControls_GpioB1Drive_gpio_22_drv_of(_x) (((_x) >> 12) & 0x3)
#define   NFP3800_PL_PluIOControls_GpioB1Drive_gpio_21_drv(_x) (((_x) & 0x3) << 10)
#define   NFP3800_PL_PluIOControls_GpioB1Drive_gpio_21_drv_of(_x) (((_x) >> 10) & 0x3)
#define   NFP3800_PL_PluIOControls_GpioB1Drive_gpio_20_drv(_x) (((_x) & 0x3) << 8)
#define   NFP3800_PL_PluIOControls_GpioB1Drive_gpio_20_drv_of(_x) (((_x) >> 8) & 0x3)
#define   NFP3800_PL_PluIOControls_GpioB1Drive_gpio_19_drv(_x) (((_x) & 0x3) << 6)
#define   NFP3800_PL_PluIOControls_GpioB1Drive_gpio_19_drv_of(_x) (((_x) >> 6) & 0x3)
#define   NFP3800_PL_PluIOControls_GpioB1Drive_gpio_18_drv(_x) (((_x) & 0x3) << 4)
#define   NFP3800_PL_PluIOControls_GpioB1Drive_gpio_18_drv_of(_x) (((_x) >> 4) & 0x3)
#define   NFP3800_PL_PluIOControls_GpioB1Drive_gpio_17_drv(_x) (((_x) & 0x3) << 2)
#define   NFP3800_PL_PluIOControls_GpioB1Drive_gpio_17_drv_of(_x) (((_x) >> 2) & 0x3)
#define   NFP3800_PL_PluIOControls_GpioB1Drive_gpio_16_drv(_x) (((_x) & 0x3) << 0)
#define   NFP3800_PL_PluIOControls_GpioB1Drive_gpio_16_drv_of(_x) (((_x) >> 0) & 0x3)
/*----------------------------------------------------------------
  Register: PluIOControls_SpiDrive - SPI driver controls.
    Bits: 27:26 spix_d3_drv(rw) - Output drive strength setting of SPIX_D3.
    Bits: 25:24 spix_d2_drv(rw) - Output drive strength setting of SPIX_D2.
    Bits: 23:22 spi3_mosi_drv(rw) - Output drive strength setting of SPI3_MOSI.
    Bits: 21:20 spi3_sck_drv(rw) - Output drive strength setting of SPI3_SCK.
    Bits: 19:18 spi2_mosi_drv(rw) - Output drive strength setting of SPI2_MOSI.
    Bits: 17:16 spi2_sck_drv(rw) - Output drive strength setting of SPI2_SCK.
    Bits: 15:14 spi1_cfg_mosi_drv(rw) - Output drive strength setting of SPI1_CFG_MOSI.
    Bits: 13:12 spi1_cfg_miso_drv(rw) - Output drive strength setting of SPI1_CFG_MISO.
    Bits: 11:10 spi1_cfg_sel_drv(rw) - Output drive strength setting of SPI1_CFG_SEL.
    Bits:  9:8  spi1_cfg_sck_drv(rw) - Output drive strength setting of SPI1_CFG_SCK.
    Bits:  7:6  spi0_mosi_drv(rw) - Output drive strength setting of SPI0_MOSI.
    Bits:  5:4  spi0_miso_drv(rw) - Output drive strength setting of SPI0_MISO.
    Bits:  3:2  spi0_sel_drv(rw) - Output drive strength setting of SPI0_SEL.
    Bits:  1:0  spi0_sck_drv(rw) - Output drive strength setting of SPI0_SCK.
*/
#define NFP3800_PL_PluIOControls_SpiDrive                    0x0004000c
#define   NFP3800_PL_PluIOControls_SpiDrive_spix_d3_drv(_x)  (((_x) & 0x3) << 26)
#define   NFP3800_PL_PluIOControls_SpiDrive_spix_d3_drv_of(_x) (((_x) >> 26) & 0x3)
#define   NFP3800_PL_PluIOControls_SpiDrive_spix_d2_drv(_x)  (((_x) & 0x3) << 24)
#define   NFP3800_PL_PluIOControls_SpiDrive_spix_d2_drv_of(_x) (((_x) >> 24) & 0x3)
#define   NFP3800_PL_PluIOControls_SpiDrive_spi3_mosi_drv(_x) (((_x) & 0x3) << 22)
#define   NFP3800_PL_PluIOControls_SpiDrive_spi3_mosi_drv_of(_x) (((_x) >> 22) & 0x3)
#define   NFP3800_PL_PluIOControls_SpiDrive_spi3_sck_drv(_x) (((_x) & 0x3) << 20)
#define   NFP3800_PL_PluIOControls_SpiDrive_spi3_sck_drv_of(_x) (((_x) >> 20) & 0x3)
#define   NFP3800_PL_PluIOControls_SpiDrive_spi2_mosi_drv(_x) (((_x) & 0x3) << 18)
#define   NFP3800_PL_PluIOControls_SpiDrive_spi2_mosi_drv_of(_x) (((_x) >> 18) & 0x3)
#define   NFP3800_PL_PluIOControls_SpiDrive_spi2_sck_drv(_x) (((_x) & 0x3) << 16)
#define   NFP3800_PL_PluIOControls_SpiDrive_spi2_sck_drv_of(_x) (((_x) >> 16) & 0x3)
#define   NFP3800_PL_PluIOControls_SpiDrive_spi1_cfg_mosi_drv(_x) (((_x) & 0x3) << 14)
#define   NFP3800_PL_PluIOControls_SpiDrive_spi1_cfg_mosi_drv_of(_x) (((_x) >> 14) & 0x3)
#define   NFP3800_PL_PluIOControls_SpiDrive_spi1_cfg_miso_drv(_x) (((_x) & 0x3) << 12)
#define   NFP3800_PL_PluIOControls_SpiDrive_spi1_cfg_miso_drv_of(_x) (((_x) >> 12) & 0x3)
#define   NFP3800_PL_PluIOControls_SpiDrive_spi1_cfg_sel_drv(_x) (((_x) & 0x3) << 10)
#define   NFP3800_PL_PluIOControls_SpiDrive_spi1_cfg_sel_drv_of(_x) (((_x) >> 10) & 0x3)
#define   NFP3800_PL_PluIOControls_SpiDrive_spi1_cfg_sck_drv(_x) (((_x) & 0x3) << 8)
#define   NFP3800_PL_PluIOControls_SpiDrive_spi1_cfg_sck_drv_of(_x) (((_x) >> 8) & 0x3)
#define   NFP3800_PL_PluIOControls_SpiDrive_spi0_mosi_drv(_x) (((_x) & 0x3) << 6)
#define   NFP3800_PL_PluIOControls_SpiDrive_spi0_mosi_drv_of(_x) (((_x) >> 6) & 0x3)
#define   NFP3800_PL_PluIOControls_SpiDrive_spi0_miso_drv(_x) (((_x) & 0x3) << 4)
#define   NFP3800_PL_PluIOControls_SpiDrive_spi0_miso_drv_of(_x) (((_x) >> 4) & 0x3)
#define   NFP3800_PL_PluIOControls_SpiDrive_spi0_sel_drv(_x) (((_x) & 0x3) << 2)
#define   NFP3800_PL_PluIOControls_SpiDrive_spi0_sel_drv_of(_x) (((_x) >> 2) & 0x3)
#define   NFP3800_PL_PluIOControls_SpiDrive_spi0_sck_drv(_x) (((_x) & 0x3) << 0)
#define   NFP3800_PL_PluIOControls_SpiDrive_spi0_sck_drv_of(_x) (((_x) >> 0) & 0x3)
/*----------------------------------------------------------------
  Register: PluIOControls_MiscDrive - Misc. driver controls.
    Bits: 21:20 jtag_tdo_drv(rw) - Output drive strength setting of JTAG_TDO.
    Bits: 19:18 pcie_reset_out_l_drv(rw) - Output drive strength setting of PCIE_RESET_OUT_L.
    Bits: 17:16 smbalrt_drv(rw) - Output drive strength setting of SMBALRT.
    Bits: 15:14 smbdat_drv(rw) - Output drive strength setting of SMBDAT.
    Bits: 13:12 smbclk_drv(rw) - Output drive strength setting of SMBCLK.
    Bits: 11:10 sr0_rx_drv(rw) - Output drive strength setting of SR0_RX.
    Bits:  9:8  sr0_tx_drv(rw) - Output drive strength setting of SR0_TX.
    Bits:  7:6  sda1_drv(rw) - Output drive strength setting of SDA1.
    Bits:  5:4  scl1_drv(rw) - Output drive strength setting of SCL1.
    Bits:  3:2  sda0_drv(rw) - Output drive strength setting of SDA0.
    Bits:  1:0  scl0_drv(rw) - Output drive strength setting of SCL0.
*/
#define NFP3800_PL_PluIOControls_MiscDrive                   0x00040010
#define   NFP3800_PL_PluIOControls_MiscDrive_jtag_tdo_drv(_x) (((_x) & 0x3) << 20)
#define   NFP3800_PL_PluIOControls_MiscDrive_jtag_tdo_drv_of(_x) (((_x) >> 20) & 0x3)
#define   NFP3800_PL_PluIOControls_MiscDrive_pcie_reset_out_l_drv(_x) (((_x) & 0x3) << 18)
#define   NFP3800_PL_PluIOControls_MiscDrive_pcie_reset_out_l_drv_of(_x) (((_x) >> 18) & 0x3)
#define   NFP3800_PL_PluIOControls_MiscDrive_smbalrt_drv(_x) (((_x) & 0x3) << 16)
#define   NFP3800_PL_PluIOControls_MiscDrive_smbalrt_drv_of(_x) (((_x) >> 16) & 0x3)
#define   NFP3800_PL_PluIOControls_MiscDrive_smbdat_drv(_x)  (((_x) & 0x3) << 14)
#define   NFP3800_PL_PluIOControls_MiscDrive_smbdat_drv_of(_x) (((_x) >> 14) & 0x3)
#define   NFP3800_PL_PluIOControls_MiscDrive_smbclk_drv(_x)  (((_x) & 0x3) << 12)
#define   NFP3800_PL_PluIOControls_MiscDrive_smbclk_drv_of(_x) (((_x) >> 12) & 0x3)
#define   NFP3800_PL_PluIOControls_MiscDrive_sr0_rx_drv(_x)  (((_x) & 0x3) << 10)
#define   NFP3800_PL_PluIOControls_MiscDrive_sr0_rx_drv_of(_x) (((_x) >> 10) & 0x3)
#define   NFP3800_PL_PluIOControls_MiscDrive_sr0_tx_drv(_x)  (((_x) & 0x3) << 8)
#define   NFP3800_PL_PluIOControls_MiscDrive_sr0_tx_drv_of(_x) (((_x) >> 8) & 0x3)
#define   NFP3800_PL_PluIOControls_MiscDrive_sda1_drv(_x)    (((_x) & 0x3) << 6)
#define   NFP3800_PL_PluIOControls_MiscDrive_sda1_drv_of(_x) (((_x) >> 6) & 0x3)
#define   NFP3800_PL_PluIOControls_MiscDrive_scl1_drv(_x)    (((_x) & 0x3) << 4)
#define   NFP3800_PL_PluIOControls_MiscDrive_scl1_drv_of(_x) (((_x) >> 4) & 0x3)
#define   NFP3800_PL_PluIOControls_MiscDrive_sda0_drv(_x)    (((_x) & 0x3) << 2)
#define   NFP3800_PL_PluIOControls_MiscDrive_sda0_drv_of(_x) (((_x) >> 2) & 0x3)
#define   NFP3800_PL_PluIOControls_MiscDrive_scl0_drv(_x)    (((_x) & 0x3) << 0)
#define   NFP3800_PL_PluIOControls_MiscDrive_scl0_drv_of(_x) (((_x) >> 0) & 0x3)
/*----------------------------------------------------------------
  Register: PluIOControls_GpioWkpullupEn - Weak pull up for GPIO.
    Bits: 31:0  gpio_wkpuen(rw) - Weak pull up enable for GPIO[31:0] (1: weak pull up enabled, 0: weak pull up disabled).
*/
#define NFP3800_PL_PluIOControls_GpioWkpullupEn              0x00040014
#define   NFP3800_PL_PluIOControls_GpioWkpullupEn_gpio_wkpuen(_x) (((_x) & 0xffffffff) << 0)
#define   NFP3800_PL_PluIOControls_GpioWkpullupEn_gpio_wkpuen_of(_x) (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: PluIOControls_GpioWkpulldnEn - Weak pull down for GPIO.
    Bits: 31:0  gpio_wkpden(rw) - Weak pull down enable for GPIO[31:0] (1: weak pull down enabled, 0: weak pull down disabled).
*/
#define NFP3800_PL_PluIOControls_GpioWkpulldnEn              0x00040018
#define   NFP3800_PL_PluIOControls_GpioWkpulldnEn_gpio_wkpden(_x) (((_x) & 0xffffffff) << 0)
#define   NFP3800_PL_PluIOControls_GpioWkpulldnEn_gpio_wkpden_of(_x) (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: PluIOControls_SpiWkpullEn - Weak pull up and pull down for SPI pins.
    Bits: 29    spix_d3_wkpden(rw) - Weak pull down enable for SPIX_D3 (1: weak pull down enabled, 0: weak pull down disabled).
    Bits: 28    spix_d2_wkpden(rw) - Weak pull down enable for SPIX_D2 (1: weak pull down enabled, 0: weak pull down disabled).
    Bits: 27    spi3_mosi_wkpden(rw) - Weak pull down enable for SPI3_MOSI (1: weak pull down enabled, 0: weak pull down disabled).
    Bits: 26    spi3_sck_wkpden(rw) - Weak pull down enable for SPI3_SCK (1: weak pull down enabled, 0: weak pull down disabled).
    Bits: 25    spi2_mosi_wkpden(rw) - Weak pull down enable for SPI2_MOSI (1: weak pull down enabled, 0: weak pull down disabled).
    Bits: 24    spi2_sck_wkpden(rw) - Weak pull down enable for SPI2_SCK (1: weak pull down enabled, 0: weak pull down disabled).
    Bits: 23    spi1_cfg_miso_wkpden(rw) - Weak pull down enable for SPI1_CFG_MISO (1: weak pull down enabled, 0: weak pull down disabled).
    Bits: 22    spi1_cfg_mosi_wkpden(rw) - Weak pull down enable for SPI1_CFG_MOSI (1: weak pull down enabled, 0: weak pull down disabled).
    Bits: 21    spi1_cfg_sel_wkpden(rw) - Weak pull down enable for SPI1_CFG_SEL (1: weak pull down enabled, 0: weak pull down disabled).
    Bits: 20    spi1_cfg_sck_wkpden(rw) - Weak pull down enable for SPI1_CFG_SCK (1: weak pull down enabled, 0: weak pull down disabled).
    Bits: 19    spi0_miso_wkpden(rw) - Weak pull down enable for SPI0_MISO (1: weak pull down enabled, 0: weak pull down disabled).
    Bits: 18    spi0_mosi_wkpden(rw) - Weak pull down enable for SPI0_MOSI (1: weak pull down enabled, 0: weak pull down disabled).
    Bits: 17    spi0_sel_wkpden(rw) - Weak pull down enable for SPI0_SEL (1: weak pull down enabled, 0: weak pull down disabled).
    Bits: 16    spi0_sck_wkpden(rw) - Weak pull down enable for SPI0_SCK (1: weak pull down enabled, 0: weak pull down disabled).
    Bits: 13    spix_d3_wkpuen(rw) - Weak pull up enable for SPIX_D3 (1: weak pull up enabled, 0: weak pull up disabled).
    Bits: 12    spix_d2_wkpuen(rw) - Weak pull up enable for SPIX_D2 (1: weak pull up enabled, 0: weak pull up disabled).
    Bits: 11    spi3_mosi_wkpuen(rw) - Weak pull up enable for SPI3_MOSI (1: weak pull up enabled, 0: weak pull up disabled).
    Bits: 10    spi3_sck_wkpuen(rw) - Weak pull up enable for SPI3_SCK (1: weak pull up enabled, 0: weak pull up disabled).
    Bits:  9    spi2_mosi_wkpuen(rw) - Weak pull up enable for SPI2_MOSI (1: weak pull up enabled, 0: weak pull up disabled).
    Bits:  8    spi2_sck_wkpuen(rw) - Weak pull up enable for SPI2_SCK (1: weak pull up enabled, 0: weak pull up disabled).
    Bits:  7    spi1_cfg_miso_wkpuen(rw) - Weak pull up enable for SPI1_CFG_MISO (1: weak pull up enabled, 0: weak pull up disabled).
    Bits:  6    spi1_cfg_mosi_wkpuen(rw) - Weak pull up enable for SPI1_CFG_MOSI (1: weak pull up enabled, 0: weak pull up disabled).
    Bits:  5    spi1_cfg_sel_wkpuen(rw) - Weak pull up enable for SPI1_CFG_SEL (1: weak pull up enabled, 0: weak pull up disabled).
    Bits:  4    spi1_cfg_sck_wkpuen(rw) - Weak pull up enable for SPI1_CFG_SCK (1: weak pull up enabled, 0: weak pull up disabled).
    Bits:  3    spi0_miso_wkpuen(rw) - Weak pull up enable for SPI0_MISO (1: weak pull up enabled, 0: weak pull up disabled).
    Bits:  2    spi0_mosi_wkpuen(rw) - Weak pull up enable for SPI0_MOSI (1: weak pull up enabled, 0: weak pull up disabled).
    Bits:  1    spi0_sel_wkpuen(rw) - Weak pull up enable for SPI0_SEL (1: weak pull up enabled, 0: weak pull up disabled).
    Bits:  0    spi0_sck_wkpuen(rw) - Weak pull up enable for SPI0_SCK (1: weak pull up enabled, 0: weak pull up disabled).
*/
#define NFP3800_PL_PluIOControls_SpiWkpullEn                 0x0004001c
#define   NFP3800_PL_PluIOControls_SpiWkpullEn_spix_d3_wkpden BIT(29)
#define   NFP3800_PL_PluIOControls_SpiWkpullEn_spix_d2_wkpden BIT(28)
#define   NFP3800_PL_PluIOControls_SpiWkpullEn_spi3_mosi_wkpden BIT(27)
#define   NFP3800_PL_PluIOControls_SpiWkpullEn_spi3_sck_wkpden BIT(26)
#define   NFP3800_PL_PluIOControls_SpiWkpullEn_spi2_mosi_wkpden BIT(25)
#define   NFP3800_PL_PluIOControls_SpiWkpullEn_spi2_sck_wkpden BIT(24)
#define   NFP3800_PL_PluIOControls_SpiWkpullEn_spi1_cfg_miso_wkpden BIT(23)
#define   NFP3800_PL_PluIOControls_SpiWkpullEn_spi1_cfg_mosi_wkpden BIT(22)
#define   NFP3800_PL_PluIOControls_SpiWkpullEn_spi1_cfg_sel_wkpden BIT(21)
#define   NFP3800_PL_PluIOControls_SpiWkpullEn_spi1_cfg_sck_wkpden BIT(20)
#define   NFP3800_PL_PluIOControls_SpiWkpullEn_spi0_miso_wkpden BIT(19)
#define   NFP3800_PL_PluIOControls_SpiWkpullEn_spi0_mosi_wkpden BIT(18)
#define   NFP3800_PL_PluIOControls_SpiWkpullEn_spi0_sel_wkpden BIT(17)
#define   NFP3800_PL_PluIOControls_SpiWkpullEn_spi0_sck_wkpden BIT(16)
#define   NFP3800_PL_PluIOControls_SpiWkpullEn_spix_d3_wkpuen BIT(13)
#define   NFP3800_PL_PluIOControls_SpiWkpullEn_spix_d2_wkpuen BIT(12)
#define   NFP3800_PL_PluIOControls_SpiWkpullEn_spi3_mosi_wkpuen BIT(11)
#define   NFP3800_PL_PluIOControls_SpiWkpullEn_spi3_sck_wkpuen BIT(10)
#define   NFP3800_PL_PluIOControls_SpiWkpullEn_spi2_mosi_wkpuen BIT(9)
#define   NFP3800_PL_PluIOControls_SpiWkpullEn_spi2_sck_wkpuen BIT(8)
#define   NFP3800_PL_PluIOControls_SpiWkpullEn_spi1_cfg_miso_wkpuen BIT(7)
#define   NFP3800_PL_PluIOControls_SpiWkpullEn_spi1_cfg_mosi_wkpuen BIT(6)
#define   NFP3800_PL_PluIOControls_SpiWkpullEn_spi1_cfg_sel_wkpuen BIT(5)
#define   NFP3800_PL_PluIOControls_SpiWkpullEn_spi1_cfg_sck_wkpuen BIT(4)
#define   NFP3800_PL_PluIOControls_SpiWkpullEn_spi0_miso_wkpuen BIT(3)
#define   NFP3800_PL_PluIOControls_SpiWkpullEn_spi0_mosi_wkpuen BIT(2)
#define   NFP3800_PL_PluIOControls_SpiWkpullEn_spi0_sel_wkpuen BIT(1)
#define   NFP3800_PL_PluIOControls_SpiWkpullEn_spi0_sck_wkpuen BIT(0)
/*----------------------------------------------------------------
  Register: PluIOControls_MiscWkpullEn - Weak pull up and pull down for pins other than GPIO or SPI .
    Bits: 31    test_jtag_bypass_wkpden(rw) - Weak pull down enable for TEST_JTAG_BYPASS (1: weak pull down enabled, 0: weak pull down disabled).
    Bits: 30    test_scan_mode_wkpden(rw) - Weak pull down enable for TEST_SCAN_MODE (1: weak pull down enabled, 0: weak pull down disabled).
    Bits: 29    jtag_trst_wkpden(rw) - Weak pull down enable for JTAG_TRST (1: weak pull down enabled, 0: weak pull down disabled).
    Bits: 28    jtag_tms_wkpden(rw) - Weak pull down enable for JTAG_TMS (1: weak pull down enabled, 0: weak pull down disabled).
    Bits: 27    jtag_tdi_wkpden(rw) - Weak pull down enable for JTAG_TDI (1: weak pull down enabled, 0: weak pull down disabled).
    Bits: 26    jtag_tck_wkpden(rw) - Weak pull down enable for JTAG_TCK (1: weak pull down enabled, 0: weak pull down disabled).
    Bits: 25    pcie_reset_in_l_wkpden(rw) - Weak pull down enable for PCIE_RESET_IN_L (1: weak pull down enabled, 0: weak pull down disabled).
    Bits: 24    sr0_rx_wkpden(rw) - Weak pull down enable for SR0_RX (1: weak pull down enabled, 0: weak pull down disabled).
    Bits: 23    sr0_tx_wkpden(rw) - Weak pull down enable for SR0_TX (1: weak pull down enabled, 0: weak pull down disabled).
    Bits: 22    smbalrt_wkpden(rw) - Weak pull down enable for SMBALRT (1: weak pull down enabled, 0: weak pull down disabled).
    Bits: 21    smbdat_wkpden(rw) - Weak pull down enable for SMBDAT (1: weak pull down enabled, 0: weak pull down disabled).
    Bits: 20    smbclk_wkpden(rw) - Weak pull down enable for SMBCLK (1: weak pull down enabled, 0: weak pull down disabled).
    Bits: 19    sda1_wkpden(rw) - Weak pull down enable for SDA1 (1: weak pull down enabled, 0: weak pull down disabled).
    Bits: 18    scl1_wkpden(rw) - Weak pull down enable for SCL1 (1: weak pull down enabled, 0: weak pull down disabled).
    Bits: 17    sda0_wkpden(rw) - Weak pull down enable for SDA0 (1: weak pull down enabled, 0: weak pull down disabled).
    Bits: 16    scl0_wkpden(rw) - Weak pull down enable for SCL0 (1: weak pull down enabled, 0: weak pull down disabled).
    Bits: 15    test_jtag_bypass_wkpuen(rw) - Weak pull up enable for TEST_JTAG_BYPASS (1: weak pull up enabled, 0: weak pull up disabled).
    Bits: 14    test_scan_mode_wkpuen(rw) - Weak pull up enable for TEST_SCAN_MODE (1: weak pull up enabled, 0: weak pull up disabled).
    Bits: 13    jtag_trst_wkpuen(rw) - Weak pull up enable for JTAG_TRST (1: weak pull up enabled, 0: weak pull up disabled).
    Bits: 12    jtag_tms_wkpuen(rw) - Weak pull up enable for JTAG_TMS (1: weak pull up enabled, 0: weak pull up disabled).
    Bits: 11    jtag_tdi_wkpuen(rw) - Weak pull up enable for JTAG_TDI (1: weak pull up enabled, 0: weak pull up disabled).
    Bits: 10    jtag_tck_wkpuen(rw) - Weak pull up enable for JTAG_TCK (1: weak pull up enabled, 0: weak pull up disabled).
    Bits:  9    pcie_reset_in_l_wkpuen(rw) - Weak pull up enable for PCIE_RESET_IN_L (1: weak pull up enabled, 0: weak pull up disabled).
    Bits:  8    sr0_rx_wkpuen(rw) - Weak pull up enable for SR0_RX (1: weak pull up enabled, 0: weak pull up disabled).
    Bits:  7    sr0_tx_wkpuen(rw) - Weak pull up enable for SR0_TX (1: weak pull up enabled, 0: weak pull up disabled).
    Bits:  6    smbalrt_wkpuen(rw) - Weak pull up enable for SMBALRT (1: weak pull up enabled, 0: weak pull up disabled).
    Bits:  5    smbdat_wkpuen(rw) - Weak pull up enable for SMBDAT (1: weak pull up enabled, 0: weak pull up disabled).
    Bits:  4    smbclk_wkpuen(rw) - Weak pull up enable for SMBCLK (1: weak pull up enabled, 0: weak pull up disabled).
    Bits:  3    sda1_wkpuen(rw) - Weak pull up enable for SDA1 (1: weak pull up enabled, 0: weak pull up disabled).
    Bits:  2    scl1_wkpuen(rw) - Weak pull up enable for SCL1 (1: weak pull up enabled, 0: weak pull up disabled).
    Bits:  1    sda0_wkpuen(rw) - Weak pull up enable for SDA0 (1: weak pull up enabled, 0: weak pull up disabled).
    Bits:  0    scl0_wkpuen(rw) - Weak pull up enable for SCL0 (1: weak pull up enabled, 0: weak pull up disabled).
*/
#define NFP3800_PL_PluIOControls_MiscWkpullEn                0x00040020
#define   NFP3800_PL_PluIOControls_MiscWkpullEn_test_jtag_bypass_wkpden BIT(31)
#define   NFP3800_PL_PluIOControls_MiscWkpullEn_test_scan_mode_wkpden BIT(30)
#define   NFP3800_PL_PluIOControls_MiscWkpullEn_jtag_trst_wkpden BIT(29)
#define   NFP3800_PL_PluIOControls_MiscWkpullEn_jtag_tms_wkpden BIT(28)
#define   NFP3800_PL_PluIOControls_MiscWkpullEn_jtag_tdi_wkpden BIT(27)
#define   NFP3800_PL_PluIOControls_MiscWkpullEn_jtag_tck_wkpden BIT(26)
#define   NFP3800_PL_PluIOControls_MiscWkpullEn_pcie_reset_in_l_wkpden BIT(25)
#define   NFP3800_PL_PluIOControls_MiscWkpullEn_sr0_rx_wkpden BIT(24)
#define   NFP3800_PL_PluIOControls_MiscWkpullEn_sr0_tx_wkpden BIT(23)
#define   NFP3800_PL_PluIOControls_MiscWkpullEn_smbalrt_wkpden BIT(22)
#define   NFP3800_PL_PluIOControls_MiscWkpullEn_smbdat_wkpden BIT(21)
#define   NFP3800_PL_PluIOControls_MiscWkpullEn_smbclk_wkpden BIT(20)
#define   NFP3800_PL_PluIOControls_MiscWkpullEn_sda1_wkpden  BIT(19)
#define   NFP3800_PL_PluIOControls_MiscWkpullEn_scl1_wkpden  BIT(18)
#define   NFP3800_PL_PluIOControls_MiscWkpullEn_sda0_wkpden  BIT(17)
#define   NFP3800_PL_PluIOControls_MiscWkpullEn_scl0_wkpden  BIT(16)
#define   NFP3800_PL_PluIOControls_MiscWkpullEn_test_jtag_bypass_wkpuen BIT(15)
#define   NFP3800_PL_PluIOControls_MiscWkpullEn_test_scan_mode_wkpuen BIT(14)
#define   NFP3800_PL_PluIOControls_MiscWkpullEn_jtag_trst_wkpuen BIT(13)
#define   NFP3800_PL_PluIOControls_MiscWkpullEn_jtag_tms_wkpuen BIT(12)
#define   NFP3800_PL_PluIOControls_MiscWkpullEn_jtag_tdi_wkpuen BIT(11)
#define   NFP3800_PL_PluIOControls_MiscWkpullEn_jtag_tck_wkpuen BIT(10)
#define   NFP3800_PL_PluIOControls_MiscWkpullEn_pcie_reset_in_l_wkpuen BIT(9)
#define   NFP3800_PL_PluIOControls_MiscWkpullEn_sr0_rx_wkpuen BIT(8)
#define   NFP3800_PL_PluIOControls_MiscWkpullEn_sr0_tx_wkpuen BIT(7)
#define   NFP3800_PL_PluIOControls_MiscWkpullEn_smbalrt_wkpuen BIT(6)
#define   NFP3800_PL_PluIOControls_MiscWkpullEn_smbdat_wkpuen BIT(5)
#define   NFP3800_PL_PluIOControls_MiscWkpullEn_smbclk_wkpuen BIT(4)
#define   NFP3800_PL_PluIOControls_MiscWkpullEn_sda1_wkpuen  BIT(3)
#define   NFP3800_PL_PluIOControls_MiscWkpullEn_scl1_wkpuen  BIT(2)
#define   NFP3800_PL_PluIOControls_MiscWkpullEn_sda0_wkpuen  BIT(1)
#define   NFP3800_PL_PluIOControls_MiscWkpullEn_scl0_wkpuen  BIT(0)
/*----------------------------------------------------------------
  Register: PluIOControls_GpioOpendrain - Open drain enable for GPIO.
    Bits: 31:0  gpio_opendrain(rw) - Open drain mode enable for GPIO[31:0] (1: Pin is in open drain mode; 0: Pin is NOT in open drain mode).
*/
#define NFP3800_PL_PluIOControls_GpioOpendrain               0x00040024
#define   NFP3800_PL_PluIOControls_GpioOpendrain_gpio_opendrain(_x) (((_x) & 0xffffffff) << 0)
#define   NFP3800_PL_PluIOControls_GpioOpendrain_gpio_opendrain_of(_x) (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: PluIOControls_MiscOpendrain - Open drain enable for pins other than GPIO.
    Bits: 25    jtag_tdo_opendrain(rw) - Open drain enable for JTAG_TDO (1: Open drain enabled, 0: Open drain disabled).
    Bits: 24    pcie_reset_out_l_opendrain(rw) - Open drain enable for PCIE_RESET_OUT_L (1: Open drain enabled, 0: Open drain disabled).
    Bits: 23    clk_nreset_out_l_opendrain(rw) - Open drain enable for CLK_NRESET_OUT_L (1: Open drain enabled, 0: Open drain disabled).
    Bits: 22    sr0_rx_opendrain(rw) - Open drain enable for SR0_RX (1: Open drain enabled, 0: Open drain disabled).
    Bits: 21    sr0_tx_opendrain(rw) - Open drain enable for SR0_TX (1: Open drain enabled, 0: Open drain disabled).
    Bits: 20    smbalrt_opendrain(rw) - Open drain enable for SMBALRT (1: Open drain enabled, 0: Open drain disabled).
    Bits: 19    smbdat_opendrain(rw) - Open drain enable for SMBDAT (1: Open drain enabled, 0: Open drain disabled).
    Bits: 18    smbclk_opendrain(rw) - Open drain enable for SMBCLK (1: Open drain enabled, 0: Open drain disabled).
    Bits: 17    sda1_opendrain(rw) - Open drain enable for SDA1 (1: Open drain enabled, 0: Open drain disabled).
    Bits: 16    scl1_opendrain(rw) - Open drain enable for SCL1 (1: Open drain enabled, 0: Open drain disabled).
    Bits: 15    sda0_opendrain(rw) - Open drain enable for SDA0 (1: Open drain enabled, 0: Open drain disabled).
    Bits: 14    scl0_opendrain(rw) - Open drain enable for SCL0 (1: Open drain enabled, 0: Open drain disabled).
    Bits: 13    spix_d3_opendrain(rw) - Open drain enable for SPIX_D3 (1: Open drain enabled, 0: Open drain disabled).
    Bits: 12    spix_d2_opendrain(rw) - Open drain enable for SPIX_D2 (1: Open drain enabled, 0: Open drain disabled).
    Bits: 11    spi3_mosi_opendrain(rw) - Open drain enable for SPI3_MOSI (1: Open drain enabled, 0: Open drain disabled).
    Bits: 10    spi3_sck_opendrain(rw) - Open drain enable for SPI3_SCK (1: Open drain enabled, 0: Open drain disabled).
    Bits:  9    spi2_mosi_opendrain(rw) - Open drain enable for SPI2_MOSI (1: Open drain enabled, 0: Open drain disabled).
    Bits:  8    spi2_sck_opendrain(rw) - Open drain enable for SPI2_SCK (1: Open drain enabled, 0: Open drain disabled).
    Bits:  7    spi1_cfg_miso_opendrain(rw) - Open drain enable for SPI1_CFG_MISO (1: Open drain enabled, 0: Open drain disabled).
    Bits:  6    spi1_cfg_mosi_opendrain(rw) - Open drain enable for SPI1_CFG_MOSI (1: Open drain enabled, 0: Open drain disabled).
    Bits:  5    spi1_cfg_sel_opendrain(rw) - Open drain enable for SPI1_CFG_SEL (1: Open drain enabled, 0: Open drain disabled).
    Bits:  4    spi1_cfg_sck_opendrain(rw) - Open drain enable for SPI1_CFG_SCK (1: Open drain enabled, 0: Open drain disabled).
    Bits:  3    spi0_miso_opendrain(rw) - Open drain enable for SPI0_MISO (1: Open drain enabled, 0: Open drain disabled).
    Bits:  2    spi0_mosi_opendrain(rw) - Open drain enable for SPI0_MOSI (1: Open drain enabled, 0: Open drain disabled).
    Bits:  1    spi0_sel_opendrain(rw) - Open drain enable for SPI0_SEL (1: Open drain enabled, 0: Open drain disabled).
    Bits:  0    spi0_sck_opendrain(rw) - Open drain enable for SPI0_SCK (1: Open drain enabled, 0: Open drain disabled).
*/
#define NFP3800_PL_PluIOControls_MiscOpendrain               0x00040028
#define   NFP3800_PL_PluIOControls_MiscOpendrain_jtag_tdo_opendrain BIT(25)
#define   NFP3800_PL_PluIOControls_MiscOpendrain_pcie_reset_out_l_opendrain BIT(24)
#define   NFP3800_PL_PluIOControls_MiscOpendrain_clk_nreset_out_l_opendrain BIT(23)
#define   NFP3800_PL_PluIOControls_MiscOpendrain_sr0_rx_opendrain BIT(22)
#define   NFP3800_PL_PluIOControls_MiscOpendrain_sr0_tx_opendrain BIT(21)
#define   NFP3800_PL_PluIOControls_MiscOpendrain_smbalrt_opendrain BIT(20)
#define   NFP3800_PL_PluIOControls_MiscOpendrain_smbdat_opendrain BIT(19)
#define   NFP3800_PL_PluIOControls_MiscOpendrain_smbclk_opendrain BIT(18)
#define   NFP3800_PL_PluIOControls_MiscOpendrain_sda1_opendrain BIT(17)
#define   NFP3800_PL_PluIOControls_MiscOpendrain_scl1_opendrain BIT(16)
#define   NFP3800_PL_PluIOControls_MiscOpendrain_sda0_opendrain BIT(15)
#define   NFP3800_PL_PluIOControls_MiscOpendrain_scl0_opendrain BIT(14)
#define   NFP3800_PL_PluIOControls_MiscOpendrain_spix_d3_opendrain BIT(13)
#define   NFP3800_PL_PluIOControls_MiscOpendrain_spix_d2_opendrain BIT(12)
#define   NFP3800_PL_PluIOControls_MiscOpendrain_spi3_mosi_opendrain BIT(11)
#define   NFP3800_PL_PluIOControls_MiscOpendrain_spi3_sck_opendrain BIT(10)
#define   NFP3800_PL_PluIOControls_MiscOpendrain_spi2_mosi_opendrain BIT(9)
#define   NFP3800_PL_PluIOControls_MiscOpendrain_spi2_sck_opendrain BIT(8)
#define   NFP3800_PL_PluIOControls_MiscOpendrain_spi1_cfg_miso_opendrain BIT(7)
#define   NFP3800_PL_PluIOControls_MiscOpendrain_spi1_cfg_mosi_opendrain BIT(6)
#define   NFP3800_PL_PluIOControls_MiscOpendrain_spi1_cfg_sel_opendrain BIT(5)
#define   NFP3800_PL_PluIOControls_MiscOpendrain_spi1_cfg_sck_opendrain BIT(4)
#define   NFP3800_PL_PluIOControls_MiscOpendrain_spi0_miso_opendrain BIT(3)
#define   NFP3800_PL_PluIOControls_MiscOpendrain_spi0_mosi_opendrain BIT(2)
#define   NFP3800_PL_PluIOControls_MiscOpendrain_spi0_sel_opendrain BIT(1)
#define   NFP3800_PL_PluIOControls_MiscOpendrain_spi0_sck_opendrain BIT(0)
/*----------------------------------------------------------------
  Register: PluIOControls_GpioRxEn - Buffer input enable for GPIO.
    Bits: 31:0  gpio_rcven(rw) - Input buffer enable for GPIO[31:0], active high.
*/
#define NFP3800_PL_PluIOControls_GpioRxEn                    0x0004002c
#define   NFP3800_PL_PluIOControls_GpioRxEn_gpio_rcven(_x)   (((_x) & 0xffffffff) << 0)
#define   NFP3800_PL_PluIOControls_GpioRxEn_gpio_rcven_of(_x) (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: PluIOControls_MiscRxEn - Buffer input enable for pin other than GPIO.
    Bits: 23    pcie_reset_in_l_rcven(rw) - Buffer input enable for PCIE_RESET_IN_L pin.
    Bits: 22    smbalrt_rcven(rw) - Buffer input enable for SMBALRT pin.
    Bits: 21    smbdat_rcven(rw) - Buffer input enable for SMBDAT pin.
    Bits: 20    smbclk_rcven(rw) - Buffer input enable for SMBCLK pin.
    Bits: 19    sda1_rcven(rw) - Buffer input enable for SDA1 pin.
    Bits: 18    scl1_rcven(rw) - Buffer input enable for SCL1 pin.
    Bits: 17    sda0_rcven(rw) - Buffer input enable for SDA0 pin.
    Bits: 16    scl0_rcven(rw) - Buffer input enable for SCL0 pin.
    Bits: 15    sr0_tx_rcven(rw) - Buffer input enable for Serial port 0 TX pin (0: on, 1: off).
    Bits: 14    sr0_rx_rcven(rw) - Buffer input enable for Serial port 0 RX pin (0: on, 1: off).
    Bits: 13    spix_d3_rcven(rw) - Buffer input enable for SPIX_D3 pin.
    Bits: 12    spix_d2_rcven(rw) - Buffer input enable for SPIX_D2 pin.
    Bits: 11    spi3_sck_rcven(rw) - Buffer input enable for SPI3_SCK pin.
    Bits: 10    spi3_mosi_rcven(rw) - Buffer input enable for SPI3_MOSI pin.
    Bits:  9    spi2_sck_rcven(rw) - Buffer input enable for SPI2_SCK pin.
    Bits:  8    spi2_mosi_rcven(rw) - Buffer input enable for SPI2_MOSI pin.
    Bits:  7    spi1_cfg_sel_rcven(rw) - Buffer input enable for SPI_CFG_SEL pin.
    Bits:  6    spi1_cfg_sck_rcven(rw) - Buffer input enable for SPI_CFG_SCK pin.
    Bits:  5    spi1_cfg_mosi_rcven(rw) - Buffer input enable for SPI1_CFG_MOSI pin.
    Bits:  4    spi1_cfg_miso_rcven(rw) - Buffer input enable for SPI1_CFG_MISO pin.
    Bits:  3    spi0_sel_rcven(rw) - Buffer input enable for SPI0_SEL pin.
    Bits:  2    spi0_sck_rcven(rw) - Buffer input enable for SPI0_SCK pin.
    Bits:  1    spi0_mosi_rcven(rw) - Buffer input enable for SPI0_MOSI pin.
    Bits:  0    spi0_miso_rcven(rw) - Buffer input enable for SPI0_MISO pin.
*/
#define NFP3800_PL_PluIOControls_MiscRxEn                    0x00040030
#define   NFP3800_PL_PluIOControls_MiscRxEn_pcie_reset_in_l_rcven BIT(23)
#define   NFP3800_PL_PluIOControls_MiscRxEn_smbalrt_rcven    BIT(22)
#define   NFP3800_PL_PluIOControls_MiscRxEn_smbdat_rcven     BIT(21)
#define   NFP3800_PL_PluIOControls_MiscRxEn_smbclk_rcven     BIT(20)
#define   NFP3800_PL_PluIOControls_MiscRxEn_sda1_rcven       BIT(19)
#define   NFP3800_PL_PluIOControls_MiscRxEn_scl1_rcven       BIT(18)
#define   NFP3800_PL_PluIOControls_MiscRxEn_sda0_rcven       BIT(17)
#define   NFP3800_PL_PluIOControls_MiscRxEn_scl0_rcven       BIT(16)
#define   NFP3800_PL_PluIOControls_MiscRxEn_sr0_tx_rcven     BIT(15)
#define   NFP3800_PL_PluIOControls_MiscRxEn_sr0_rx_rcven     BIT(14)
#define   NFP3800_PL_PluIOControls_MiscRxEn_spix_d3_rcven    BIT(13)
#define   NFP3800_PL_PluIOControls_MiscRxEn_spix_d2_rcven    BIT(12)
#define   NFP3800_PL_PluIOControls_MiscRxEn_spi3_sck_rcven   BIT(11)
#define   NFP3800_PL_PluIOControls_MiscRxEn_spi3_mosi_rcven  BIT(10)
#define   NFP3800_PL_PluIOControls_MiscRxEn_spi2_sck_rcven   BIT(9)
#define   NFP3800_PL_PluIOControls_MiscRxEn_spi2_mosi_rcven  BIT(8)
#define   NFP3800_PL_PluIOControls_MiscRxEn_spi1_cfg_sel_rcven BIT(7)
#define   NFP3800_PL_PluIOControls_MiscRxEn_spi1_cfg_sck_rcven BIT(6)
#define   NFP3800_PL_PluIOControls_MiscRxEn_spi1_cfg_mosi_rcven BIT(5)
#define   NFP3800_PL_PluIOControls_MiscRxEn_spi1_cfg_miso_rcven BIT(4)
#define   NFP3800_PL_PluIOControls_MiscRxEn_spi0_sel_rcven   BIT(3)
#define   NFP3800_PL_PluIOControls_MiscRxEn_spi0_sck_rcven   BIT(2)
#define   NFP3800_PL_PluIOControls_MiscRxEn_spi0_mosi_rcven  BIT(1)
#define   NFP3800_PL_PluIOControls_MiscRxEn_spi0_miso_rcven  BIT(0)
/*----------------------------------------------------------------
  Register: PluIOControls_MiscControl - Misc IO controls.
    Bits:  1    gpio33_mode_1p8(rw) - IO PAD mode control for all 3.3V IO pins
    Bits:  0    gpio_force_tristate(rw) - Force tristate on all 3.3V and 1.8V pins.
*/
#define NFP3800_PL_PluIOControls_MiscControl                 0x00040034
#define   NFP3800_PL_PluIOControls_MiscControl_gpio33_mode_1p8 BIT(1)
#define     NFP3800_PL_PluIOControls_MiscControl_gpio33_mode_1p8_0 (0 << 1)
#define     NFP3800_PL_PluIOControls_MiscControl_gpio33_mode_1p8_1 BIT(1)
#define   NFP3800_PL_PluIOControls_MiscControl_gpio_force_tristate BIT(0)
#define     NFP3800_PL_PluIOControls_MiscControl_gpio_force_tristate_0 (0 << 0)
#define     NFP3800_PL_PluIOControls_MiscControl_gpio_force_tristate_1 BIT(0)
/*----------------------------------------------------------------
  Register: PluPMonBBGenXpb_PluPMonRegIntfc - Unbiased Process Monitor JTAG Mode Register Interface
  Register: PluPMonBBGenXpb_PluBPMonRegIntfc - Biased Process Monitor JTAG Mode Register Interface
    Bits: 31:30 Start(ro) - Start bits for Process Monitor Register Interface Frame
    Bits: 29:28 Op(rw) - OpCode for Process Monitor Register Interface
    Bits: 27:18 RegAddr(rw) - Register Address for Process Monitor Register Interface
    Bits: 15:0  Data(rw) - Register Write/Read Data for Process Monitor Register Interface
*/
#define NFP3800_PL_PluPMonBBGenXpb_PluPMonRegIntfc           0x00090000
#define NFP3800_PL_PluPMonBBGenXpb_PluBPMonRegIntfc          0x00090008
#define   NFP3800_PL_PluPMonBBGenXpb_PluPMonRegIntfc_Start(_x) (((_x) & 0x3) << 30)
#define   NFP3800_PL_PluPMonBBGenXpb_PluPMonRegIntfc_Start_of(_x) (((_x) >> 30) & 0x3)
#define   NFP3800_PL_PluPMonBBGenXpb_PluPMonRegIntfc_Op(_x)  (((_x) & 0x3) << 28)
#define   NFP3800_PL_PluPMonBBGenXpb_PluPMonRegIntfc_Op_of(_x) (((_x) >> 28) & 0x3)
#define     NFP3800_PL_PluPMonBBGenXpb_PluPMonRegIntfc_Op_NoOp (0)
#define     NFP3800_PL_PluPMonBBGenXpb_PluPMonRegIntfc_Op_Write (1)
#define   NFP3800_PL_PluPMonBBGenXpb_PluPMonRegIntfc_RegAddr(_x) (((_x) & 0x3ff) << 18)
#define   NFP3800_PL_PluPMonBBGenXpb_PluPMonRegIntfc_RegAddr_of(_x) (((_x) >> 18) & 0x3ff)
#define   NFP3800_PL_PluPMonBBGenXpb_PluPMonRegIntfc_Data(_x) (((_x) & 0xffff) << 0)
#define   NFP3800_PL_PluPMonBBGenXpb_PluPMonRegIntfc_Data_of(_x) (((_x) >> 0) & 0xffff)
/*----------------------------------------------------------------
  Register: PluPMonBBGenXpb_PluBBGenDynamicAdjustment - Bias Generation Dynamic Adjustment Enables
    Bits:  2    StateMachineActive(ro) - This is a status bit for the Dynamic Adjustment State Machine. 1 indicates that the state machine is active. In this state, all related control registers will have inhibited write access. 0 indicates that the state machine is idle and control registers can be written.
    Bits:  1    AutoAdjustEnable(rw) - When 0, Dynamic Adjustment State Machine will only take measurements but not adjust the BBGEN controls. When 1, BBGEN controls will be dynamically adjusted.
    Bits:  0    MeasurementEnable(rw) - When set to 1, enables and activates the Dynamic Adjustment State Machine. When set 0, the Dynamic Adjustment State Machine will go to idle at the completion of the next timer cycle.
*/
#define NFP3800_PL_PluPMonBBGenXpb_PluBBGenDynamicAdjustment 0x00090004
#define   NFP3800_PL_PluPMonBBGenXpb_PluBBGenDynamicAdjustment_StateMachineActive BIT(2)
#define   NFP3800_PL_PluPMonBBGenXpb_PluBBGenDynamicAdjustment_AutoAdjustEnable BIT(1)
#define   NFP3800_PL_PluPMonBBGenXpb_PluBBGenDynamicAdjustment_MeasurementEnable BIT(0)
/*----------------------------------------------------------------
  Register: PluPMonBBGenXpb_PluPMonDAControls - Biased Process Montor Direct Access Controls
    Bits:  9    DA_JTAG_SEL(rw) - Selects between JTAG Control (0) or Direct Access Control (1).
    Bits:  8    DA_RO_EN(rw) - Ring Oscillator Enable in Direct Access Mode: 1 for Enable Ring Oscillator, 0 for Disable Ring Oscillator.
    Bits:  7    DA_RO_BYPASS(rw) - Ring Oscillator ByPass: 1 for Reference Clock to the Output, 0 for Ring Oscillator Clock to the Output.
    Bits:  6    DA_MILLER_CTRL(rw) - Control Signal to test the Miller Effect.
    Bits:  5:0  DA_DECODE(rw) - Selects between multiple ring oscillators.
*/
#define NFP3800_PL_PluPMonBBGenXpb_PluPMonDAControls         0x0009000c
#define   NFP3800_PL_PluPMonBBGenXpb_PluPMonDAControls_DA_JTAG_SEL BIT(9)
#define   NFP3800_PL_PluPMonBBGenXpb_PluPMonDAControls_DA_RO_EN BIT(8)
#define   NFP3800_PL_PluPMonBBGenXpb_PluPMonDAControls_DA_RO_BYPASS BIT(7)
#define   NFP3800_PL_PluPMonBBGenXpb_PluPMonDAControls_DA_MILLER_CTRL BIT(6)
#define   NFP3800_PL_PluPMonBBGenXpb_PluPMonDAControls_DA_DECODE(_x) (((_x) & 0x3f) << 0)
#define   NFP3800_PL_PluPMonBBGenXpb_PluPMonDAControls_DA_DECODE_of(_x) (((_x) >> 0) & 0x3f)
/*----------------------------------------------------------------
  Register: PluPMonBBGenXpb_PluPMonRefTimerStartValue - Reference Timer Value to Start PMon Ring Oscillator (DA_CLKOUT) Measurement
  Register: PluPMonBBGenXpb_PluPMonRefTimerStopValue - Reference Timer Value to Stop PMon Ring Oscillator (DA_CLKOUT) Measurement
    Bits: 31:0  RefTimerValue(rw) - 32-bit Timer Value.
*/
#define NFP3800_PL_PluPMonBBGenXpb_PluPMonRefTimerStartValue 0x00090010
#define NFP3800_PL_PluPMonBBGenXpb_PluPMonRefTimerStopValue  0x00090014
#define   NFP3800_PL_PluPMonBBGenXpb_PluPMonRefTimerValue_RefTimerValue(_x) (((_x) & 0xffffffff) << 0)
#define   NFP3800_PL_PluPMonBBGenXpb_PluPMonRefTimerValue_RefTimerValue_of(_x) (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: PluPMonBBGenXpb_PluPMonClkOutTimerTargetValue - Target DA_CLKOUT Timer Value for setting BBGEN
  Register: PluPMonBBGenXpb_PluPMonClkOutTimerDiffThreshold0 - (Target - Timer) Difference Threshold0 for BBGEN Adjustment Calculations
  Register: PluPMonBBGenXpb_PluPMonClkOutTimerDiffThreshold1 - (Target - Timer) Difference Threshold1 for BBGEN Adjustment Calculations
  Register: PluPMonBBGenXpb_PluPMonClkOutTimerDiffThreshold2 - (Target - Timer) Difference Threshold2 for BBGEN Adjustment Calculations
    Bits: 31:0  ClkOutTimerValue(rw) - 32-bit Timer Value.
*/
#define NFP3800_PL_PluPMonBBGenXpb_PluPMonClkOutTimerTargetValue 0x00090018
#define NFP3800_PL_PluPMonBBGenXpb_PluPMonClkOutTimerDiffThreshold0 0x00090020
#define NFP3800_PL_PluPMonBBGenXpb_PluPMonClkOutTimerDiffThreshold1 0x00090024
#define NFP3800_PL_PluPMonBBGenXpb_PluPMonClkOutTimerDiffThreshold2 0x00090028
#define   NFP3800_PL_PluPMonBBGenXpb_PluPMonClkOutTimerValue_ClkOutTimerValue(_x) (((_x) & 0xffffffff) << 0)
#define   NFP3800_PL_PluPMonBBGenXpb_PluPMonClkOutTimerValue_ClkOutTimerValue_of(_x) (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: PluPMonBBGenXpb_PluPMonClkOutTimerSampleValue - DA_CLKOUT Timer Value Sampled after Reference Timer Stop Value
    Bits: 31:0  ClkOutTimerSample(ro) - 32-bit Timer Sample Value.
*/
#define NFP3800_PL_PluPMonBBGenXpb_PluPMonClkOutTimerSampleValue 0x0009001c
#define   NFP3800_PL_PluPMonBBGenXpb_PluPMonClkOutTimerSampleValue_ClkOutTimerSample(_x) (((_x) & 0xffffffff) << 0)
#define   NFP3800_PL_PluPMonBBGenXpb_PluPMonClkOutTimerSampleValue_ClkOutTimerSample_of(_x) (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: PluPMonBBGenXpb_PluBBGenAdjustLUT - BBGEN Adjustment Lookup Table
    Bits:  7:6  LUTEntry3(rw) - 2-bit Adjustment Value.
    Bits:  5:4  LUTEntry2(rw) - 2-bit Adjustment Value.
    Bits:  3:2  LUTEntry1(rw) - 2-bit Adjustment Value.
    Bits:  1:0  LUTEntry0(rw) - 2-bit Adjustment Value.
*/
#define NFP3800_PL_PluPMonBBGenXpb_PluBBGenAdjustLUT         0x00090030
#define   NFP3800_PL_PluPMonBBGenXpb_PluBBGenAdjustLUT_LUTEntry3(_x) (((_x) & 0x3) << 6)
#define   NFP3800_PL_PluPMonBBGenXpb_PluBBGenAdjustLUT_LUTEntry3_of(_x) (((_x) >> 6) & 0x3)
#define   NFP3800_PL_PluPMonBBGenXpb_PluBBGenAdjustLUT_LUTEntry2(_x) (((_x) & 0x3) << 4)
#define   NFP3800_PL_PluPMonBBGenXpb_PluBBGenAdjustLUT_LUTEntry2_of(_x) (((_x) >> 4) & 0x3)
#define   NFP3800_PL_PluPMonBBGenXpb_PluBBGenAdjustLUT_LUTEntry1(_x) (((_x) & 0x3) << 2)
#define   NFP3800_PL_PluPMonBBGenXpb_PluBBGenAdjustLUT_LUTEntry1_of(_x) (((_x) >> 2) & 0x3)
#define   NFP3800_PL_PluPMonBBGenXpb_PluBBGenAdjustLUT_LUTEntry0(_x) (((_x) & 0x3) << 0)
#define   NFP3800_PL_PluPMonBBGenXpb_PluBBGenAdjustLUT_LUTEntry0_of(_x) (((_x) >> 0) & 0x3)
/*----------------------------------------------------------------
  Register: PluPMonBBGenXpb_PluBBGenControl - BBGEN Control Values
    Bits: 31    BBGenLoadSeqBusy(ro) - Load Sequencer Busy: Can't handle another update yet.
    Bits: 12:8  BBGenpCtrl(rw) - Voltage Control Value: Starting 0 with steps of 57mV.
    Bits:  4:0  BBGennCtrl(rw) - Voltage Control Value: Starting 0 with steps of -57mV.
*/
#define NFP3800_PL_PluPMonBBGenXpb_PluBBGenControl           0x00090034
#define   NFP3800_PL_PluPMonBBGenXpb_PluBBGenControl_BBGenLoadSeqBusy BIT(31)
#define   NFP3800_PL_PluPMonBBGenXpb_PluBBGenControl_BBGenpCtrl(_x) (((_x) & 0x1f) << 8)
#define   NFP3800_PL_PluPMonBBGenXpb_PluBBGenControl_BBGenpCtrl_of(_x) (((_x) >> 8) & 0x1f)
#define   NFP3800_PL_PluPMonBBGenXpb_PluBBGenControl_BBGennCtrl(_x) (((_x) & 0x1f) << 0)
#define   NFP3800_PL_PluPMonBBGenXpb_PluBBGenControl_BBGennCtrl_of(_x) (((_x) >> 0) & 0x1f)
/*----------------------------------------------------------------
  Register: PluPMonBBGenXpb_PluBBGenLimit - BBGEN Control Value Limits
    Bits: 12:8  BBGenpLimit(rw) - Voltage Control Limit Value: 798mV.
    Bits:  4:0  BBGennLimit(rw) - Voltage Control Limit Value: -798mV.
*/
#define NFP3800_PL_PluPMonBBGenXpb_PluBBGenLimit             0x00090038
#define   NFP3800_PL_PluPMonBBGenXpb_PluBBGenLimit_BBGenpLimit(_x) (((_x) & 0x1f) << 8)
#define   NFP3800_PL_PluPMonBBGenXpb_PluBBGenLimit_BBGenpLimit_of(_x) (((_x) >> 8) & 0x1f)
#define   NFP3800_PL_PluPMonBBGenXpb_PluBBGenLimit_BBGennLimit(_x) (((_x) & 0x1f) << 0)
#define   NFP3800_PL_PluPMonBBGenXpb_PluBBGenLimit_BBGennLimit_of(_x) (((_x) >> 0) & 0x1f)
/*----------------------------------------------------------------
  Register: PluPMonBBGenXpb_PluFusePmonClkOutTimerTargetValue - Fused Recommended Value Dynamic BBGEN Target Value
    Bits: 31:0  TargetValue(ro) - Fused Target Value for Dynamic BBGen Algorithm from Fuse Chain 0.
*/
#define NFP3800_PL_PluPMonBBGenXpb_PluFusePmonClkOutTimerTargetValue 0x0009003c
#define   NFP3800_PL_PluPMonBBGenXpb_PluFusePmonClkOutTimerTargetValue_TargetValue(_x) (((_x) & 0xffffffff) << 0)
#define   NFP3800_PL_PluPMonBBGenXpb_PluFusePmonClkOutTimerTargetValue_TargetValue_of(_x) (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: PluPMonBBGenXpb_PluFuseBBGenControl - Fused BBGEN Control Initial Values and AutoUpdate Indicator
    Bits: 12:8  BBGenpCtrl(ro) - Fused Voltage Control Value
    Bits:  4:0  BBGennCtrl(ro) - Fused Voltage Control Value
*/
#define NFP3800_PL_PluPMonBBGenXpb_PluFuseBBGenControl       0x00090040
#define   NFP3800_PL_PluPMonBBGenXpb_PluFuseBBGenControl_BBGenpCtrl(_x) (((_x) & 0x1f) << 8)
#define   NFP3800_PL_PluPMonBBGenXpb_PluFuseBBGenControl_BBGenpCtrl_of(_x) (((_x) >> 8) & 0x1f)
#define   NFP3800_PL_PluPMonBBGenXpb_PluFuseBBGenControl_BBGennCtrl(_x) (((_x) & 0x1f) << 0)
#define   NFP3800_PL_PluPMonBBGenXpb_PluFuseBBGenControl_BBGennCtrl_of(_x) (((_x) >> 0) & 0x1f)
/*----------------------------------------------------------------
  Register: XPB2Jtag_MiscControl - Island-specific miscellaneous control
    Bits: 31:0  MiscCtrl(rw) - Miscellaneous Control Register - function depends on island.
*/
#define NFP3800_PL_XPB2Jtag_MiscControl                      0x002f5410
#define   NFP3800_PL_XPB2Jtag_MiscControl_MiscCtrl(_x)       (((_x) & 0xffffffff) << 0)
#define   NFP3800_PL_XPB2Jtag_MiscControl_MiscCtrl_of(_x)    (((_x) >> 0) & 0xffffffff)

#endif /* NFP3800_LONGNAMES */


#endif /* NFP3800_PL_H */
