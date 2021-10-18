/*
 * Copyright (C) 2013, Netronome Systems, Inc.  All rights reserved.
 *
 * Disclaimer: this file is provided without any warranty
 * as part of an early-access program, and the content is
 * bound to change before the final release.
 */

#ifndef NFP6000_PL_H
#define NFP6000_PL_H

/*----------------------------------------------------------------*/
/* XPB Address Map (XPB_BASE = NFP_XPB_ISLAND(1) + NFP_ARMX_PLU)    */


/* HGID: 60515d2e075a */

#ifndef NFP6000_LONGNAMES

/*----------------------------------------------------------------
  Register: PluMisc_PluMasterReset - Programmable Reset Controls
    Bits: 11    PCIE3_RESET_MASK(rw) - When set to 1, PCIE3_RESET_OUT_L will not be reset by SRESETN
    Bits: 10    PCIE2_RESET_MASK(rw) - When set to 1, PCIE2_RESET_OUT_L will not be reset by SRESETN
    Bits:  9    PCIE1_RESET_MASK(rw) - When set to 1, PCIE1_RESET_OUT_L will not be reset by SRESETN
    Bits:  8    PCIE0_RESET_MASK(rw) - When set to 1, PCIE0_RESET_OUT_L will not be reset by SRESETN
    Bits:  7    PCIE3_RESET_OUT_L(rw) - PCIE3 Reset Out
    Bits:  6    PCIE2_RESET_OUT_L(rw) - PCIE2 Reset Out
    Bits:  5    PCIE1_RESET_OUT_L(rw) - PCIE1 Reset Out
    Bits:  4    PCIE0_RESET_OUT_L(rw) - PCIE0 Reset Out
    Bits:  0    CLK_NRESET_OUT_L(rw) - PowerOn Reset Out
*/
#define NFP_PL_PluMisc_PLU_MASTER_RESET                      0x00000000
#define   NFP_PL_PLUMISC_PLU_MASTER_RESET_PCIE3_RESET_MASK   BIT(11)
#define   NFP_PL_PLUMISC_PLU_MASTER_RESET_PCIE2_RESET_MASK   BIT(10)
#define   NFP_PL_PLUMISC_PLU_MASTER_RESET_PCIE1_RESET_MASK   BIT(9)
#define   NFP_PL_PLUMISC_PLU_MASTER_RESET_PCIE0_RESET_MASK   BIT(8)
#define   NFP_PL_PLUMISC_PLU_MASTER_RESET_PCIE3_RESET_OUT_L  BIT(7)
#define   NFP_PL_PLUMISC_PLU_MASTER_RESET_PCIE2_RESET_OUT_L  BIT(6)
#define   NFP_PL_PLUMISC_PLU_MASTER_RESET_PCIE1_RESET_OUT_L  BIT(5)
#define   NFP_PL_PLUMISC_PLU_MASTER_RESET_PCIE0_RESET_OUT_L  BIT(4)
#define   NFP_PL_PLUMISC_PLU_MASTER_RESET_CLK_NRESET_OUT_L   BIT(0)
/*----------------------------------------------------------------
  Register: PluMisc_PluDeviceID - Chip Device ID Register.
    Bits: 31:16 PartNumber(ro) - Major Part Number Designation (Hard Coded)
    Bits: 15:8  SKU(ro) - SKU Designation (From Fuses)
    Bits:  7:4  MajorRevID(ro) - Major Revision ID (Hard Coded)
    Bits:  3:0  MinorRevID(ro) - Minor Revision ID (From Fuses or Special Metal Structure)
*/
#define NFP_PL_PluMisc_PLU_DEVICE_ID                         0x00000004
#define   NFP_PL_PLUMISC_PLU_DEVICE_ID_PART_NUM(_x)          (((_x) & 0xffff) << 16)
#define   NFP_PL_PLUMISC_PLU_DEVICE_ID_PART_NUM_of(_x)       (((_x) >> 16) & 0xffff)
#define   NFP_PL_PLUMISC_PLU_DEVICE_ID_SKU(_x)               (((_x) & 0xff) << 8)
#define   NFP_PL_PLUMISC_PLU_DEVICE_ID_SKU_of(_x)            (((_x) >> 8) & 0xff)
#define   NFP_PL_PLUMISC_PLU_DEVICE_ID_MAJOR_REV_ID(_x)      (((_x) & 0xf) << 4)
#define   NFP_PL_PLUMISC_PLU_DEVICE_ID_MAJOR_REV_ID_of(_x)   (((_x) >> 4) & 0xf)
#define   NFP_PL_PLUMISC_PLU_DEVICE_ID_MINOR_REV_ID(_x)      (((_x) & 0xf) << 0)
#define   NFP_PL_PLUMISC_PLU_DEVICE_ID_MINOR_REV_ID_of(_x)   (((_x) >> 0) & 0xf)
/*----------------------------------------------------------------
  Register: PluMisc_PluPowerState - Indicators for Power and Power-Domain Status
    Bits:  5    CRYPTO_PDL(ro) - Crypto Power Domain Locked
    Bits:  4    BOTTOM_PDL(ro) - Bottom of Chip Power Domain Locked.
    Bits:  3    POWERGOOD4(ro) - Power Domain 4 Power Good Indicator.
    Bits:  2    POWERGOOD3(ro) - Power Domain 3 Power Good Indicator.
    Bits:  1    POWERGOOD2(ro) - Power Domain 2 Power Good Indicator.
    Bits:  0    POWERGOOD1(ro) - Power Domain 1 Power Good Indicator.
*/
#define NFP_PL_PluMisc_PLU_POWER_STATE                       0x0000000c
#define   NFP_PL_PLUMISC_PLU_POWER_STATE_CRYPTO_PDL          BIT(5)
#define   NFP_PL_PLUMISC_PLU_POWER_STATE_BOTTOM_PDL          BIT(4)
#define   NFP_PL_PLUMISC_PLU_POWER_STATE_POWER_GOOD4         BIT(3)
#define   NFP_PL_PLUMISC_PLU_POWER_STATE_POWER_GOOD3         BIT(2)
#define   NFP_PL_PLUMISC_PLU_POWER_STATE_POWER_GOOD2         BIT(1)
#define   NFP_PL_PLUMISC_PLU_POWER_STATE_POWER_GOOD1         BIT(0)
/*----------------------------------------------------------------
  Register: PluMisc_PluVID - VID Control Register
    Bits: 23:16 VIDProcessMetric(ro) - 8-bit process metric used by software to determine proper VIDValue
    Bits:  8    VIDEnable(rw) - Under ideal conditions, this will Enable VID outputs. However, in B0 Stepping of NFP-6xxx, writing to this bit has no effect. VID outputs will remain enabled at all times after powering up the chip.
    Bits:  7:0  VIDValue(rw) - VID Control Bus Value: After POWERGOOD1 is asserted, VIDValue samples value seen on VID pins for 16 clock cycles and then turns on VIDEnable = 1. VID pins will only be sampled during this initial period.
*/
#define NFP_PL_PluMisc_PLU_VID                               0x00000014
#define   NFP_PL_PLUMISC_PLU_VID_VID_PROCESS_METRIC(_x)      (((_x) & 0xff) << 16)
#define   NFP_PL_PLUMISC_PLU_VID_VID_PROCESS_METRIC_of(_x)   (((_x) >> 16) & 0xff)
#define   NFP_PL_PLUMISC_PLU_VID_VID_ENABLE                  BIT(8)
#define   NFP_PL_PLUMISC_PLU_VID_VID_VALUE(_x)               (((_x) & 0xff) << 0)
#define   NFP_PL_PLUMISC_PLU_VID_VID_VALUE_of(_x)            (((_x) >> 0) & 0xff)
/*----------------------------------------------------------------
  Register: PluMisc_PluBaudRateGen - Control Register for Baud Rate Generator.
    Bits: 31:16 BaudControlA(rw) - Control A for Baud Rate Generator. See Baud Rate Generator Table
    Bits: 15:0  BaudControlB(rw) - Control B for Baud Rate Generator. See Baud Rate Generator Table
*/
#define NFP_PL_PluMisc_PLU_BAUD_RATE_GEN                     0x00000018
#define   NFP_PL_PLUMISC_PLU_BAUD_RATE_GEN_BAUD_CTRL_A(_x)   (((_x) & 0xffff) << 16)
#define   NFP_PL_PLUMISC_PLU_BAUD_RATE_GEN_BAUD_CTRL_A_of(_x) (((_x) >> 16) & 0xffff)
#define   NFP_PL_PLUMISC_PLU_BAUD_RATE_GEN_BAUD_CTRL_B(_x)   (((_x) & 0xffff) << 0)
#define   NFP_PL_PLUMISC_PLU_BAUD_RATE_GEN_BAUD_CTRL_B_of(_x) (((_x) >> 0) & 0xffff)
/*----------------------------------------------------------------
  Register: PluMisc_PluArmClockSelect - ARM Clock Generation Control
    Bits:  0    AclkSelect(rw) - Select which PLL's secondary output (PLLCLK1) will be used for generating the ARM Clock.
*/
#define NFP_PL_PluMisc_PLU_ARM_CLK_SEL                       0x00000034
#define   NFP_PL_PLUMISC_PLU_ARM_CLK_SEL_ACLK_SELECT         BIT(0)
#define     NFP_PL_PLUMISC_PLU_ARM_CLK_SEL_ACLK_SELECT_PLL0  (0 << 0)
#define     NFP_PL_PLUMISC_PLU_ARM_CLK_SEL_ACLK_SELECT_PLL1  BIT(0)
/*----------------------------------------------------------------
  Register: PluMisc_PluJtagTckControl - JTAG_TCK Internal Generation Control
    Bits:  8    ClkDivBypass(rw) - Clock Divider will be bypassed when in SCAN_MODE or in HARD_RESET or when this bit is set to 1.
    Bits:  7:0  ClkDivRatio(rw) - REF_CLK divider ratio to internally generate JTAG_TCK. Legal values: 3-255
*/
#define NFP_PL_PluMisc_PLU_JTAG_TCK_CONTROL                  0x00000038
#define   NFP_PL_PLUMISC_PLU_JTAG_TCK_CONTROL_CLK_DIV_BYPASS BIT(8)
#define   NFP_PL_PLUMISC_PLU_JTAG_TCK_CONTROL_CLK_DIV_RATIO(_x) (((_x) & 0xff) << 0)
#define   NFP_PL_PLUMISC_PLU_JTAG_TCK_CONTROL_CLK_DIV_RATIO_of(_x) (((_x) >> 0) & 0xff)
/*----------------------------------------------------------------
  Register: PluTSXpb_PluTSControl0 - Thermal Sensor Basic Controls
    Bits: 15    TSitsrst(rw) - Reset all Thermal Sensor Registers. Recommended at each power up after powergood is asserted.
    Bits: 14    TSitsen(rw) - 0 = Disable all circuits. Leave this bit always on, also in power on, beside debugging.
    Bits: 13:10 TSitstripthreshon(rw) - When temperature reaches the value indicated by this bus, trip signal, also called catastrophic (otstemperaturetrip) is turned on. 115 C + 15 steps (1 C) for trip point configuration. (Default is 130 C
    Bits:  9:2  TSitsalertthreshon(rw) - When temperature reaches the value indicated by this bus, alert signal otstemperaturealrt is turned on Where 0 is -50 C, resolution of 1 C. Default is 125 C
    Bits:  1:0  TSitsalerthysteresis(rw) - Provides hysteresis below the alert threshold trip point while temp is going down; the otstemperaturealrt gets deasserted below [alert threshold point (itsalertthreshon) + hysteresis values].
*/
#define NFP_PL_PluTSXpb_PLU_TS_CONTROL0                      0x00010000
#define   NFP_PL_PLUTSXPB_PLU_TS_CONTROL0_TS_RESET           BIT(15)
#define     NFP_PL_PLUTSXPB_PLU_TS_CONTROL0_TS_RESET_NORMAL  (0 << 15)
#define     NFP_PL_PLUTSXPB_PLU_TS_CONTROL0_TS_RESET_RESET   BIT(15)
#define   NFP_PL_PLUTSXPB_PLU_TS_CONTROL0_TS_ENABLE          BIT(14)
#define     NFP_PL_PLUTSXPB_PLU_TS_CONTROL0_TS_ENABLE_DISABLE (0 << 14)
#define     NFP_PL_PLUTSXPB_PLU_TS_CONTROL0_TS_ENABLE_ENABLE BIT(14)
#define   NFP_PL_PLUTSXPB_PLU_TS_CONTROL0_TS_TRIP_THRESH(_x) (((_x) & 0xf) << 10)
#define   NFP_PL_PLUTSXPB_PLU_TS_CONTROL0_TS_TRIP_THRESH_of(_x) (((_x) >> 10) & 0xf)
#define   NFP_PL_PLUTSXPB_PLU_TS_CONTROL0_TS_ALERT_THRESH(_x) (((_x) & 0xff) << 2)
#define   NFP_PL_PLUTSXPB_PLU_TS_CONTROL0_TS_ALERT_THRESH_of(_x) (((_x) >> 2) & 0xff)
#define   NFP_PL_PLUTSXPB_PLU_TS_CONTROL0_TS_HYSTERESIS(_x)  (((_x) & 0x3) << 0)
#define   NFP_PL_PLUTSXPB_PLU_TS_CONTROL0_TS_HYSTERESIS_of(_x) (((_x) >> 0) & 0x3)
#define     NFP_PL_PLUTSXPB_PLU_TS_CONTROL0_TS_HYSTERESIS_0_C (0)
#define     NFP_PL_PLUTSXPB_PLU_TS_CONTROL0_TS_HYSTERESIS_1_C (1)
#define     NFP_PL_PLUTSXPB_PLU_TS_CONTROL0_TS_HYSTERESIS_2_C (2)
#define     NFP_PL_PLUTSXPB_PLU_TS_CONTROL0_TS_HYSTERESIS_5_C (3)
/*----------------------------------------------------------------
  Register: PluTSXpb_PluTSControl1 - Thermal Sensor Minor Controls
    Bits: 27    TSianachopctr(rw) - Select where Chopping is Done - 0 for chopping in Sense, 1 for chopping in SD ADC
    Bits: 26    TSitssupplymode(rw) - Select signal to keep the sense stage reference signal voltage the same.
    Bits: 25    TSitsfilterrate(rw) - Selects CIC filter decimation factor.
    Bits: 24    TSitsfilterrangesel(rw) - Range select for filter output bits according to the gain of the TS analog circuit
    Bits: 22:18 TSidfttpsel(rw) - DFT test port select
    Bits: 17    TSidftatfen(rw) - Enables the ATF Circuit, allowing testin of Analog signals digitally with odftatf port to SoC
    Bits: 16    TSitsdigpwrctrlovrd(rw) - Disables the power control sequence
    Bits: 12:11 TSisnscurrentsel(rw) - Selects the current value for the four current sources within sense stage. The following are the base branch bias current values:
    Bits: 10    TSisnscurratio3p5(rw) - Sense State Current Ratio.
    Bits:  9    TSisnswastesel(rw) - Selects the wast current target (when isnscurratio3p5 = 0):
    Bits:  8    TSisnsdemen(rw) - Enable Dynamic Element Matching (DEM)
    Bits:  7    TSisnsdemstpctr(rw) - DFX mode pin (In Normal DEM operation, connect isnsdenstpctr = 1)
    Bits:  6    TSisnsintvrefen(rw) - Enables an internal reference voltage gnerated by a resistor divider. External reference voltage is used for DFT when isnsintvrefen=0, that is, with an external reference voltage of 0.920 V.
    Bits:  5    TSitsfiltertst(rw) - Input signal to test digital filter
    Bits:  4:3  TSisdfssel(rw) - Sigma Delta select bits
    Bits:  2    TSisdx2delayen(rw) - Double the internal delays in the phase generator in SD ADC
    Bits:  1    TSisdmodesel(rw) - SD ADC mode selection (Tie to 1, no voltage reference is available)
    Bits:  0    TSisdstandaloneen(rw) - SDC ADC stand-alone test (Tie to 0 for normal operation)
*/
#define NFP_PL_PluTSXpb_PLU_TS_CONTROL1                      0x00010004
#define   NFP_PL_PLUTSXPB_PLU_TS_CONTROL1_TS_CHOP_SEL        BIT(27)
#define     NFP_PL_PLUTSXPB_PLU_TS_CONTROL1_TS_CHOP_SEL_SENSE (0 << 27)
#define     NFP_PL_PLUTSXPB_PLU_TS_CONTROL1_TS_CHOP_SEL_ADC  BIT(27)
#define   NFP_PL_PLUTSXPB_PLU_TS_CONTROL1_TS_SUPPLY_MODE     BIT(26)
#define     NFP_PL_PLUTSXPB_PLU_TS_CONTROL1_TS_SUPPLY_MODE_0 (0 << 26)
#define     NFP_PL_PLUTSXPB_PLU_TS_CONTROL1_TS_SUPPLY_MODE_1 BIT(26)
#define   NFP_PL_PLUTSXPB_PLU_TS_CONTROL1_TS_FILTER_RATE     BIT(25)
#define     NFP_PL_PLUTSXPB_PLU_TS_CONTROL1_TS_FILTER_RATE_0 (0 << 25)
#define     NFP_PL_PLUTSXPB_PLU_TS_CONTROL1_TS_FILTER_RATE_1 BIT(25)
#define   NFP_PL_PLUTSXPB_PLU_TS_CONTROL1_TS_FILTER_RANGE    BIT(24)
#define   NFP_PL_PLUTSXPB_PLU_TS_CONTROL1_TS_DFT_PORT_SEL(_x) (((_x) & 0x1f) << 18)
#define   NFP_PL_PLUTSXPB_PLU_TS_CONTROL1_TS_DFT_PORT_SEL_of(_x) (((_x) >> 18) & 0x1f)
#define   NFP_PL_PLUTSXPB_PLU_TS_CONTROL1_TS_DFT_ANALOG_EN   BIT(17)
#define   NFP_PL_PLUTSXPB_PLU_TS_CONTROL1_TS_DISABLE_POWER_SEQ BIT(16)
#define   NFP_PL_PLUTSXPB_PLU_TS_CONTROL1_TS_SN_CURRENT_SEL(_x) (((_x) & 0x3) << 11)
#define   NFP_PL_PLUTSXPB_PLU_TS_CONTROL1_TS_SN_CURRENT_SEL_of(_x) (((_x) >> 11) & 0x3)
#define   NFP_PL_PLUTSXPB_PLU_TS_CONTROL1_TS_SN_CURRENT_RATIO BIT(10)
#define     NFP_PL_PLUTSXPB_PLU_TS_CONTROL1_TS_SN_CURRENT_RATIO_EP (0 << 10)
#define     NFP_PL_PLUTSXPB_PLU_TS_CONTROL1_TS_SN_CURRENT_RATIO_RC BIT(10)
#define   NFP_PL_PLUTSXPB_PLU_TS_CONTROL1_TS_SN_WASTE_SEL    BIT(9)
#define     NFP_PL_PLUTSXPB_PLU_TS_CONTROL1_TS_SN_WASTE_SEL_EP (0 << 9)
#define     NFP_PL_PLUTSXPB_PLU_TS_CONTROL1_TS_SN_WASTE_SEL_RC BIT(9)
#define   NFP_PL_PLUTSXPB_PLU_TS_CONTROL1_TS_SN_DEM_EN       BIT(8)
#define     NFP_PL_PLUTSXPB_PLU_TS_CONTROL1_TS_SN_DEM_EN_EP  (0 << 8)
#define     NFP_PL_PLUTSXPB_PLU_TS_CONTROL1_TS_SN_DEM_EN_RC  BIT(8)
#define   NFP_PL_PLUTSXPB_PLU_TS_CONTROL1_TS_SN_DEM_DFX      BIT(7)
#define   NFP_PL_PLUTSXPB_PLU_TS_CONTROL1_TS_SN_INT_VREF     BIT(6)
#define   NFP_PL_PLUTSXPB_PLU_TS_CONTROL1_TS_DS_FILTER_TEST  BIT(5)
#define   NFP_PL_PLUTSXPB_PLU_TS_CONTROL1_TS_SD_CLKSEL(_x)   (((_x) & 0x3) << 3)
#define   NFP_PL_PLUTSXPB_PLU_TS_CONTROL1_TS_SD_CLKSEL_of(_x) (((_x) >> 3) & 0x3)
#define   NFP_PL_PLUTSXPB_PLU_TS_CONTROL1_TS_SD_DOUBLE_DELAY BIT(2)
#define     NFP_PL_PLUTSXPB_PLU_TS_CONTROL1_TS_SD_DOUBLE_DELAY_EP (0 << 2)
#define     NFP_PL_PLUTSXPB_PLU_TS_CONTROL1_TS_SD_DOUBLE_DELAY_RC BIT(2)
#define   NFP_PL_PLUTSXPB_PLU_TS_CONTROL1_TS_SD_MODE_SEL     BIT(1)
#define   NFP_PL_PLUTSXPB_PLU_TS_CONTROL1_TS_SD_STAND_ALONE_EN BIT(0)
/*----------------------------------------------------------------
  Register: PluTSXpb_PluTSControl2 - Thermal Sensor Calibration Controls
    Bits: 22:18 TSicalgamma(rw) - Used to calibrate the TS for temperature curvature correction for ratiometric mode.
    Bits: 17:16 TSicalconfigsel(rw) - TS Calibration Controls
    Bits: 15:8  TSicaldatoffset(rw) - Calibration fuses used for temperature offset cancellation with resolution of 0.5 C. NOTE: During the calibration process, add an option to get these inputs from a register with the defaults declared.
    Bits:  7:0  TSicaldatslope(rw) - Calibration fuses used for temperature slope calibraryion. NOTE: During the calibration process, add an option to get these inputs from a register with the defaults declared.
*/
#define NFP_PL_PluTSXpb_PLU_TS_CONTROL2                      0x00010008
#define   NFP_PL_PLUTSXPB_PLU_TS_CONTROL2_TS_CAL_GAMMA(_x)   (((_x) & 0x1f) << 18)
#define   NFP_PL_PLUTSXPB_PLU_TS_CONTROL2_TS_CAL_GAMMA_of(_x) (((_x) >> 18) & 0x1f)
#define   NFP_PL_PLUTSXPB_PLU_TS_CONTROL2_TS_CAL_CONFIG(_x)  (((_x) & 0x3) << 16)
#define   NFP_PL_PLUTSXPB_PLU_TS_CONTROL2_TS_CAL_CONFIG_of(_x) (((_x) >> 16) & 0x3)
#define     NFP_PL_PLUTSXPB_PLU_TS_CONTROL2_TS_CAL_CONFIG_LINEAR (0)
#define     NFP_PL_PLUTSXPB_PLU_TS_CONTROL2_TS_CAL_CONFIG_BOTH (1)
#define     NFP_PL_PLUTSXPB_PLU_TS_CONTROL2_TS_CAL_CONFIG_NEITHER (2)
#define     NFP_PL_PLUTSXPB_PLU_TS_CONTROL2_TS_CAL_CONFIG_CALIBRATE (3)
#define   NFP_PL_PLUTSXPB_PLU_TS_CONTROL2_TS_CAL_OFFSET(_x)  (((_x) & 0xff) << 8)
#define   NFP_PL_PLUTSXPB_PLU_TS_CONTROL2_TS_CAL_OFFSET_of(_x) (((_x) >> 8) & 0xff)
#define   NFP_PL_PLUTSXPB_PLU_TS_CONTROL2_TS_CAL_SLOPE(_x)   (((_x) & 0xff) << 0)
#define   NFP_PL_PLUTSXPB_PLU_TS_CONTROL2_TS_CAL_SLOPE_of(_x) (((_x) >> 0) & 0xff)
/*----------------------------------------------------------------
  Register: PluTSXpb_PluTSReadings - Thermal Sensor Readings
    Bits: 11    TSotstemperaturetrip(ro) - Temperature catastrophic alert signal according to itstripthreshon[3:0] pin settings; this signal wil get asserted when temp on die goes above the threshold set by itstripthreshon[3:0]; gets deasserted only when the Thermal Sensor IP is reset.
    Bits: 10    TSotstemperaturealrt(ro) - Temperature trip point alert signal based on the thresholds set by itsalwerthysteresis[1:0] and itsalertthreshon[7:0] pins; the alert signal will be asserted when temp on the die crosses the threshold set by itsalertthreshon[7:0] bits, when temp goes down on the die, the alert signal also gets deasserted.
    Bits:  9    TSotstemperaturevalid(ro) - Valid pulse for temperature sampling (when it is 1, temp reading is valid)
    Bits:  8:0  TSotstemperature(ro) - 9 bit temperature reading Where 0 = -50 C, resolution of 0.5 C
*/
#define NFP_PL_PluTSXpb_PLU_TS_READINGS                      0x0001000c
#define   NFP_PL_PLUTSXPB_PLU_TS_READINGS_TS_TEMPERATURE_TRIP BIT(11)
#define   NFP_PL_PLUTSXPB_PLU_TS_READINGS_TS_TEMPERATURE_ALERT BIT(10)
#define   NFP_PL_PLUTSXPB_PLU_TS_READINGS_TS_TEMPERATURE_VALID BIT(9)
#define   NFP_PL_PLUTSXPB_PLU_TS_READINGS_TS_TEMPERATURE(_x) (((_x) & 0x1ff) << 0)
#define   NFP_PL_PLUTSXPB_PLU_TS_READINGS_TS_TEMPERATURE_of(_x) (((_x) >> 0) & 0x1ff)
/*----------------------------------------------------------------
  Register: PLL0Controls_PLLControlAndStatus - Primary Control and Status Register
  Register: PLL1Controls_PLLControlAndStatus - Primary Control and Status Register
    Bits: 31    PLLPdSynth(rw) - FSM Control: 1=Normal Power-Up 0=PowerDown. When transitioned to 1, the PLL State Machine will execute an automatic PLL bringup sequence and lockout rights to all but the Manual Override configuration register.
    Bits: 30    PLLCLK1OutputEn(rw) - FSM Control: Enable for PLLCLK1 Output
    Bits: 29    PLLCLK0OutputEn(rw) - FSM Control: Enable for PLLCLK0 Output
    Bits: 24    PLLAutoReLock(rw) - Mode bit to select behavior upon the occurence of a Lock Fault
    Bits: 23:8  PLLLockFaultCount(ro) - Once the PLL has been spun-up, counts how many times that lock is lost. This field will be cleared whenever PLLPdSynth transitions from 0 to 1.
    Bits:  7    PLLLockFault(ro) - Indicates that the PLL has lost lock during normal operation. This field will be cleared whenever PLLPdSynth transitions from 0 to 1.
    Bits:  6    PLLSynthLock(ro) - PLL Lock Indicator
    Bits:  5    PLLSynthOut1En(ro) - FSM Output: Enable for synthesizer clock output 1
    Bits:  4    PLLSynthOutByPass1(ro) - FSM Output: Bypass synthesizer clock output 1 with ICK_SYNTHREF with the PLL
    Bits:  3    PLLWrapOutByPass1(ro) - FSM Output: Bypass synthesizer clock output 1 with ICK_SYNTHREF outside the PLL
    Bits:  2    PLLSynthOut0En(ro) - FSM Output: Enable for synthesizer clock output 0
    Bits:  1    PLLSynthOutByPass0(ro) - FSM Output: Bypass synthesizer clock output 0 with ICK_SYNTHREF within the PLL
    Bits:  0    PLLWrapOutByPass0(ro) - FSM Output: Bypass synthesizer clock output 0 with ICK_SYNTHREF outside the PLL
*/
#define NFP_PL_PLL0Controls_PLL_CONTROL_AND_STATUS           0x00020000
#define NFP_PL_PLL1Controls_PLL_CONTROL_AND_STATUS           0x00030000
#define   NFP_PL_PLL0CONTROLS_PLLCONTROLANDSTATUS_PLL_PD_SYNTH BIT(31)
#define   NFP_PL_PLL0CONTROLS_PLLCONTROLANDSTATUS_PLLCLK1_OUTPUT_ENABLE BIT(30)
#define   NFP_PL_PLL0CONTROLS_PLLCONTROLANDSTATUS_PLLCLK0_OUTPUT_ENABLE BIT(29)
#define   NFP_PL_PLL0CONTROLS_PLLCONTROLANDSTATUS_PLL_AUTO_RELOCK BIT(24)
#define     NFP_PL_PLL0CONTROLS_PLLCONTROLANDSTATUS_PLL_AUTO_RELOCK_IDLE (0 << 24)
#define     NFP_PL_PLL0CONTROLS_PLLCONTROLANDSTATUS_PLL_AUTO_RELOCK_RELOCK BIT(24)
#define   NFP_PL_PLL0CONTROLS_PLLCONTROLANDSTATUS_PLL_LOCK_FAULT_COUNT(_x) (((_x) & 0xffff) << 8)
#define   NFP_PL_PLL0CONTROLS_PLLCONTROLANDSTATUS_PLL_LOCK_FAULT_COUNT_of(_x) (((_x) >> 8) & 0xffff)
#define   NFP_PL_PLL0CONTROLS_PLLCONTROLANDSTATUS_PLL_LOCK_FAULT BIT(7)
#define   NFP_PL_PLL0CONTROLS_PLLCONTROLANDSTATUS_PLL_SYNTH_LOCK BIT(6)
#define   NFP_PL_PLL0CONTROLS_PLLCONTROLANDSTATUS_PLL_SYNTHOUT1_EN BIT(5)
#define   NFP_PL_PLL0CONTROLS_PLLCONTROLANDSTATUS_PLL_SYNTHOUT_BYPASS1 BIT(4)
#define   NFP_PL_PLL0CONTROLS_PLLCONTROLANDSTATUS_PLL_WRAPOUT_BYPASS1 BIT(3)
#define   NFP_PL_PLL0CONTROLS_PLLCONTROLANDSTATUS_PLL_SYNTHOUT0_EN BIT(2)
#define   NFP_PL_PLL0CONTROLS_PLLCONTROLANDSTATUS_PLL_SYNTHOUT_BYPASS0 BIT(1)
#define   NFP_PL_PLL0CONTROLS_PLLCONTROLANDSTATUS_PLL_WRAPOUT_BYPASS0 BIT(0)
/*----------------------------------------------------------------
  Register: PLL0Controls_PLLConfig - Configuration Settings for the PLL
  Register: PLL1Controls_PLLConfig - Configuration Settings for the PLL
    Bits: 31    PLLLockMult(rw) - Write 1 to Set, cannot be cleared without hard_reset. Setting the lock bit captures the current values of the multipliers (integer and fraction) as limits for all future settings
    Bits: 30    PLLSynthMode(rw) - Synthesizer mode: 1=Fractional mode, 0=integer mode
    Bits: 29:24 PLLSynthRefDiv(rw) - Reference clock input divider ratio
    Bits: 23:16 PLLSynthMDiv(rw) - Synthesizer feedback divider value
    Bits: 15:0  PLLSynthSSCModGain(rw) - Signed 16-bit magnitude SSC modulation depth control word, as Fraction div ratio.
*/
#define NFP_PL_PLL0Controls_PLL_CONFIG                       0x00020004
#define NFP_PL_PLL1Controls_PLL_CONFIG                       0x00030004
#define   NFP_PL_PLL0CONTROLS_PLLCONFIG_PLL_LOCK_MULT        BIT(31)
#define   NFP_PL_PLL0CONTROLS_PLLCONFIG_PLL_SYNTH_MODE       BIT(30)
#define   NFP_PL_PLL0CONTROLS_PLLCONFIG_PLL_SYNTH_REF_DIV(_x) (((_x) & 0x3f) << 24)
#define   NFP_PL_PLL0CONTROLS_PLLCONFIG_PLL_SYNTH_REF_DIV_of(_x) (((_x) >> 24) & 0x3f)
#define   NFP_PL_PLL0CONTROLS_PLLCONFIG_PLL_SYNTH_MDIV(_x)   (((_x) & 0xff) << 16)
#define   NFP_PL_PLL0CONTROLS_PLLCONFIG_PLL_SYNTH_MDIV_of(_x) (((_x) >> 16) & 0xff)
#define   NFP_PL_PLL0CONTROLS_PLLCONFIG_PLL_SYNTH_SSC_MODE_GAIN(_x) (((_x) & 0xffff) << 0)
#define   NFP_PL_PLL0CONTROLS_PLLCONFIG_PLL_SYNTH_SSC_MODE_GAIN_of(_x) (((_x) >> 0) & 0xffff)
/*----------------------------------------------------------------
  Register: PLL0Controls_PLLOutputDividers - Configuration Settings for the Output Dividers
  Register: PLL1Controls_PLLOutputDividers - Configuration Settings for the Output Dividers
    Bits: 14    PLLSynthOutPhRst(rw) - Reset phase rotator to initial startup value.
    Bits: 13:8  PLLSynthOut1Div(rw) - Synthesizer clock output 1 divider value
    Bits:  7    PLLSynthOutPhInc1(rw) - Increments phase of synthesizer clock outputs 1 by 1/8 of 1 period on rising edge
    Bits:  6:1  PLLSynthOut0Div(rw) - Synthesizer clock output 0 divider value
    Bits:  0    PLLSynthOutPhInc0(rw) - Increments phase of synthesizer clock outputs 0 by 1/8 of 1 period on rising edge
*/
#define NFP_PL_PLL0Controls_PLL_OUTPUT_DIVIDERS              0x00020008
#define NFP_PL_PLL1Controls_PLL_OUTPUT_DIVIDERS              0x00030008
#define   NFP_PL_PLL0CONTROLS_PLLOUTPUTDIVIDERS_PLL_SYNTHOUT_PH_RST BIT(14)
#define   NFP_PL_PLL0CONTROLS_PLLOUTPUTDIVIDERS_PLL_SYNTHOUT1_DIV(_x) (((_x) & 0x3f) << 8)
#define   NFP_PL_PLL0CONTROLS_PLLOUTPUTDIVIDERS_PLL_SYNTHOUT1_DIV_of(_x) (((_x) >> 8) & 0x3f)
#define   NFP_PL_PLL0CONTROLS_PLLOUTPUTDIVIDERS_PLL_SYNTHOUT_PH_INC1 BIT(7)
#define   NFP_PL_PLL0CONTROLS_PLLOUTPUTDIVIDERS_PLL_SYNTHOUT0_DIV(_x) (((_x) & 0x3f) << 1)
#define   NFP_PL_PLL0CONTROLS_PLLOUTPUTDIVIDERS_PLL_SYNTHOUT0_DIV_of(_x) (((_x) >> 1) & 0x3f)
#define   NFP_PL_PLL0CONTROLS_PLLOUTPUTDIVIDERS_PLL_SYNTHOUT_PH_INC0 BIT(0)
/*----------------------------------------------------------------
  Register: PLL0Controls_PLLMiscControls - Misc Tuning and Configuration Settings
  Register: PLL1Controls_PLLMiscControls - Misc Tuning and Configuration Settings
    Bits: 24    PLLSynthFastLockEn(rw) - Fast lock enable.
    Bits: 23    PLLSynthOutPhIncSat(rw) - Control saturation behavior for phase increments.
    Bits: 22:16 PLLSynthPropGain(rw) - Synthesizer loop bandwidth control.
    Bits: 15:0  PLLSynthSSCGenCount(rw) - SSC Frequency Control
*/
#define NFP_PL_PLL0Controls_PLL_MISC_CONTROLS                0x0002000c
#define NFP_PL_PLL1Controls_PLL_MISC_CONTROLS                0x0003000c
#define   NFP_PL_PLL0CONTROLS_PLLMISCCONTROLS_PLL_SYNTH_FAST_LOCK_EN BIT(24)
#define   NFP_PL_PLL0CONTROLS_PLLMISCCONTROLS_PLL_SYNTHOUT_PH_INC_SAT BIT(23)
#define   NFP_PL_PLL0CONTROLS_PLLMISCCONTROLS_PLL_SYNTH_PROP_GAIN(_x) (((_x) & 0x7f) << 16)
#define   NFP_PL_PLL0CONTROLS_PLLMISCCONTROLS_PLL_SYNTH_PROP_GAIN_of(_x) (((_x) >> 16) & 0x7f)
#define   NFP_PL_PLL0CONTROLS_PLLMISCCONTROLS_PLL_SYNTH_SSC_GEN_COUNT(_x) (((_x) & 0xffff) << 0)
#define   NFP_PL_PLL0CONTROLS_PLLMISCCONTROLS_PLL_SYNTH_SSC_GEN_COUNT_of(_x) (((_x) >> 0) & 0xffff)
/*----------------------------------------------------------------
  Register: PLL0Controls_PLLManualOverride - Manual Override Register
  Register: PLL1Controls_PLLManualOverride - Manual Override Register
    Bits: 31    PLLManualOverride(rw) - Setting this bit to 1 enables manual override control for the PLLs. This takes control away from the PLL State Machine. This will also enable writes to the other config regs that would normally be blocked by the PLL State Machine
    Bits: 30    OvdPdSynth(rw) - Manual Override: PLL Power Down Mode: 1=Normal 0=PowerDown
    Bits: 29    OvdResetSynth(rw) - Manual Override: Reset mode: 1=Normal, 0=Reset
    Bits:  5    OvdSynthOut1En(rw) - Manual Override: Enable for synthesizer clock output 1
    Bits:  4    OvdSynthOutByPass1(rw) - Manual Override: Bypass synthesizer clock output 1 with ICK_SYNTHREF with the PLL
    Bits:  3    OvdWrapOutByPass1(rw) - Manual Override: Bypass synthesizer clock output 1 with ICK_SYNTHREF outside the PLL
    Bits:  2    OvdSynthOut0En(rw) - Manual Override: Enable for synthesizer clock output 0
    Bits:  1    OvdSynthOutByPass0(rw) - Manual Override: Bypass synthesizer clock output 0 with ICK_SYNTHREF within the PLL
    Bits:  0    OvdWrapOutByPass0(rw) - Manual Override: Bypass synthesizer clock output 0 with ICK_SYNTHREF outside the PLL
*/
#define NFP_PL_PLL0Controls_PLL_MAN_OVERRIDE                 0x00020014
#define NFP_PL_PLL1Controls_PLL_MAN_OVERRIDE                 0x00030014
#define   NFP_PL_PLL0CONTROLS_PLLMANUALOVERRIDE_PLL_MANUAL_OVERRIDE BIT(31)
#define   NFP_PL_PLL0CONTROLS_PLLMANUALOVERRIDE_OVD_PD_SYNTH BIT(30)
#define   NFP_PL_PLL0CONTROLS_PLLMANUALOVERRIDE_OVD_RESET_SYNTH BIT(29)
#define   NFP_PL_PLL0CONTROLS_PLLMANUALOVERRIDE_OVD_SYNTHOUT1_EN BIT(5)
#define   NFP_PL_PLL0CONTROLS_PLLMANUALOVERRIDE_OVD_SYNTHOUT_BYPASS1 BIT(4)
#define   NFP_PL_PLL0CONTROLS_PLLMANUALOVERRIDE_OVD_WRAPOUT_BYPASS1 BIT(3)
#define   NFP_PL_PLL0CONTROLS_PLLMANUALOVERRIDE_OVD_SYNTHOUT0_EN BIT(2)
#define   NFP_PL_PLL0CONTROLS_PLLMANUALOVERRIDE_OVD_SYNTHOUT_BYPASS0 BIT(1)
#define   NFP_PL_PLL0CONTROLS_PLLMANUALOVERRIDE_OVD_WRAPOUT_BYPASS0 BIT(0)
/*----------------------------------------------------------------
  Register: PluIOControls_PluLvcmos0IOCtrl - LVCMOS0 IO Control
  Register: PluIOControls_PluLvcmos1IOCtrl - LVCMOS1 IO Control
    Bits: 27    ILK1_TX_wk_pull_en(rw) - Weak pull enable for ILK1_OOB_TCLK, TSYNC, TDATA pins.
    Bits: 26    ILK1_TX_wk_polarity(rw) - Weak pull polarity for ILK1_OOB_TCLK, TSYNC, TDATA pins.
    Bits: 25    ILK1_TX_rcven(rw) - Receive enable for ILK1_OOB_TCLK, TSYNC, TDATA pins. High Active.
    Bits: 24    ILK1_TX_oenb(rw) - Output enable for ILK1_OOB_TCLK, TSYNC, TDATA pins. Low active.
    Bits: 20    ILK1_RX_wk_pull_en(rw) - Weak pull enable for ILK1_OOB_RCLK, RSYNC, RDATA pins.
    Bits: 19    ILK1_RX_wk_polarity(rw) - Weak pull polarity for ILK1_OOB_RCLK, RSYNC, RDATA pins.
    Bits: 18    ILK1_RX_rcvhys(rw) - Receiver hysterisis enable for ILK1_OOB_RCLK, RSYNC, RDATA pins. High active.
    Bits: 17    ILK1_RX_rcven(rw) - Receive enable for ILK1_OOB_RCLK, RSYNC, RDATA pins. High Active.
    Bits: 16    ILK1_RX_oenb(rw) - Output enable for ILK1_OOB_RCLK, RSYNC, RDATA pins. Low active.
    Bits: 11    ILK0_TX_wk_pull_en(rw) - Weak pull enable for ILK0_OOB_TCLK, TSYNC, TDATA pins.
    Bits: 10    ILK0_TX_wk_polarity(rw) - Weak pull polarity for ILK0_OOB_TCLK, TSYNC, TDATA pins.
    Bits:  9    ILK0_TX_rcven(rw) - Receive enable for ILK0_OOB_TCLK, TSYNC, TDATA pins. High Active.
    Bits:  8    ILK0_TX_oenb(rw) - Output enable for ILK0_OOB_TCLK, TSYNC, TDATA pins. Low active.
    Bits:  4    ILK0_RX_wk_pull_en(rw) - Weak pull enable for ILK0_OOB_RCLK, RSYNC, RDATA pins.
    Bits:  3    ILK0_RX_wk_polarity(rw) - Weak pull polarity for ILK0_OOB_RCLK, RSYNC, RDATA pins.
    Bits:  2    ILK0_RX_rcvhys(rw) - Receiver hysterisis enable for ILK0_OOB_RCLK, RSYNC, RDATA pins. High active.
    Bits:  1    ILK0_RX_rcven(rw) - Receive enable for ILK0_OOB_RCLK, RSYNC, RDATA pins. High Active.
    Bits:  0    ILK0_RX_oenb(rw) - Output enable for ILK0_OOB_RCLK, RSYNC, RDATA pins. Low active.
*/
#define NFP_PL_PluIOControls_PLU_LVCMOS0_IO_CTRL             0x00040000
#define NFP_PL_PluIOControls_PLU_LVCMOS1_IO_CTRL             0x00040020
#define   NFP_PL_PLUIOCONTROLS_PLULVCMOSIOCTRL_ILK1_TX_WK_PULL_EN BIT(27)
#define     NFP_PL_PLUIOCONTROLS_PLULVCMOSIOCTRL_ILK1_TX_WK_PULL_EN_0 (0 << 27)
#define     NFP_PL_PLUIOCONTROLS_PLULVCMOSIOCTRL_ILK1_TX_WK_PULL_EN_1 BIT(27)
#define   NFP_PL_PLUIOCONTROLS_PLULVCMOSIOCTRL_ILK1_TX_WK_POLARITY BIT(26)
#define     NFP_PL_PLUIOCONTROLS_PLULVCMOSIOCTRL_ILK1_TX_WK_POLARITY_0 (0 << 26)
#define     NFP_PL_PLUIOCONTROLS_PLULVCMOSIOCTRL_ILK1_TX_WK_POLARITY_1 BIT(26)
#define   NFP_PL_PLUIOCONTROLS_PLULVCMOSIOCTRL_ILK1_TX_RCVEN BIT(25)
#define   NFP_PL_PLUIOCONTROLS_PLULVCMOSIOCTRL_ILK1_TX_OENB  BIT(24)
#define   NFP_PL_PLUIOCONTROLS_PLULVCMOSIOCTRL_ILK1_RX_WK_PULL_EN BIT(20)
#define     NFP_PL_PLUIOCONTROLS_PLULVCMOSIOCTRL_ILK1_RX_WK_PULL_EN_0 (0 << 20)
#define     NFP_PL_PLUIOCONTROLS_PLULVCMOSIOCTRL_ILK1_RX_WK_PULL_EN_1 BIT(20)
#define   NFP_PL_PLUIOCONTROLS_PLULVCMOSIOCTRL_ILK1_RX_WK_POLARITY BIT(19)
#define     NFP_PL_PLUIOCONTROLS_PLULVCMOSIOCTRL_ILK1_RX_WK_POLARITY_0 (0 << 19)
#define     NFP_PL_PLUIOCONTROLS_PLULVCMOSIOCTRL_ILK1_RX_WK_POLARITY_1 BIT(19)
#define   NFP_PL_PLUIOCONTROLS_PLULVCMOSIOCTRL_ILK1_RX_RCVHYS BIT(18)
#define   NFP_PL_PLUIOCONTROLS_PLULVCMOSIOCTRL_ILK1_RX_RCVEN BIT(17)
#define   NFP_PL_PLUIOCONTROLS_PLULVCMOSIOCTRL_ILK1_RX_OENB  BIT(16)
#define   NFP_PL_PLUIOCONTROLS_PLULVCMOSIOCTRL_ILK0_TX_WK_PULL_EN BIT(11)
#define     NFP_PL_PLUIOCONTROLS_PLULVCMOSIOCTRL_ILK0_TX_WK_PULL_EN_0 (0 << 11)
#define     NFP_PL_PLUIOCONTROLS_PLULVCMOSIOCTRL_ILK0_TX_WK_PULL_EN_1 BIT(11)
#define   NFP_PL_PLUIOCONTROLS_PLULVCMOSIOCTRL_ILK0_TX_WK_POLARITY BIT(10)
#define     NFP_PL_PLUIOCONTROLS_PLULVCMOSIOCTRL_ILK0_TX_WK_POLARITY_0 (0 << 10)
#define     NFP_PL_PLUIOCONTROLS_PLULVCMOSIOCTRL_ILK0_TX_WK_POLARITY_1 BIT(10)
#define   NFP_PL_PLUIOCONTROLS_PLULVCMOSIOCTRL_ILK0_TX_RCVEN BIT(9)
#define   NFP_PL_PLUIOCONTROLS_PLULVCMOSIOCTRL_ILK0_TX_OENB  BIT(8)
#define   NFP_PL_PLUIOCONTROLS_PLULVCMOSIOCTRL_ILK0_RX_WK_PULL_EN BIT(4)
#define     NFP_PL_PLUIOCONTROLS_PLULVCMOSIOCTRL_ILK0_RX_WK_PULL_EN_0 (0 << 4)
#define     NFP_PL_PLUIOCONTROLS_PLULVCMOSIOCTRL_ILK0_RX_WK_PULL_EN_1 BIT(4)
#define   NFP_PL_PLUIOCONTROLS_PLULVCMOSIOCTRL_ILK0_RX_WK_POLARITY BIT(3)
#define     NFP_PL_PLUIOCONTROLS_PLULVCMOSIOCTRL_ILK0_RX_WK_POLARITY_0 (0 << 3)
#define     NFP_PL_PLUIOCONTROLS_PLULVCMOSIOCTRL_ILK0_RX_WK_POLARITY_1 BIT(3)
#define   NFP_PL_PLUIOCONTROLS_PLULVCMOSIOCTRL_ILK0_RX_RCVHYS BIT(2)
#define   NFP_PL_PLUIOCONTROLS_PLULVCMOSIOCTRL_ILK0_RX_RCVEN BIT(1)
#define   NFP_PL_PLUIOCONTROLS_PLULVCMOSIOCTRL_ILK0_RX_OENB  BIT(0)
/*----------------------------------------------------------------
  Register: PluIOControls_PluLvcmos0IOCtrl2 - LVCMOS0 IO Control 2
  Register: PluIOControls_PluLvcmos1IOCtrl2 - LVCMOS1 IO Control 2
    Bits: 31:27 tco_compr(rw) - Driver TCO delay control rise.
    Bits: 26:22 pup_scomp_code(rw) - Driver pull up slew rate control setting.
    Bits: 21:16 pup_odt_rcomp(rw) - Pull up ODT setting for all LVCMOS pins.
    Bits: 15:11 tco_compf(rw) - Driver TCO delay control fall.
    Bits: 10:6  pdn_scomp_code(rw) - Driver pull down slew rate control setting.
    Bits:  5:0  pdn_odt_rcomp(rw) - Pull down ODT setting for all LVCMOS pins.
*/
#define NFP_PL_PluIOControls_PLU_LVCMOS0_IO_CTRL2            0x00040004
#define NFP_PL_PluIOControls_PLU_LVCMOS1_IO_CTRL2            0x00040024
#define   NFP_PL_PLUIOCONTROLS_PLULVCMOSIOCTRL2_TCO_COMPR(_x) (((_x) & 0x1f) << 27)
#define   NFP_PL_PLUIOCONTROLS_PLULVCMOSIOCTRL2_TCO_COMPR_of(_x) (((_x) >> 27) & 0x1f)
#define   NFP_PL_PLUIOCONTROLS_PLULVCMOSIOCTRL2_PUP_SCOMP_CODE(_x) (((_x) & 0x1f) << 22)
#define   NFP_PL_PLUIOCONTROLS_PLULVCMOSIOCTRL2_PUP_SCOMP_CODE_of(_x) (((_x) >> 22) & 0x1f)
#define   NFP_PL_PLUIOCONTROLS_PLULVCMOSIOCTRL2_PUP_ODT_RCOMP(_x) (((_x) & 0x3f) << 16)
#define   NFP_PL_PLUIOCONTROLS_PLULVCMOSIOCTRL2_PUP_ODT_RCOMP_of(_x) (((_x) >> 16) & 0x3f)
#define   NFP_PL_PLUIOCONTROLS_PLULVCMOSIOCTRL2_TCO_COMPF(_x) (((_x) & 0x1f) << 11)
#define   NFP_PL_PLUIOCONTROLS_PLULVCMOSIOCTRL2_TCO_COMPF_of(_x) (((_x) >> 11) & 0x1f)
#define   NFP_PL_PLUIOCONTROLS_PLULVCMOSIOCTRL2_PDN_SCOMP_CODE(_x) (((_x) & 0x1f) << 6)
#define   NFP_PL_PLUIOCONTROLS_PLULVCMOSIOCTRL2_PDN_SCOMP_CODE_of(_x) (((_x) >> 6) & 0x1f)
#define   NFP_PL_PLUIOCONTROLS_PLULVCMOSIOCTRL2_PDN_ODT_RCOMP(_x) (((_x) & 0x3f) << 0)
#define   NFP_PL_PLUIOCONTROLS_PLULVCMOSIOCTRL2_PDN_ODT_RCOMP_of(_x) (((_x) >> 0) & 0x3f)
/*----------------------------------------------------------------
  Register: PluIOControls_PluLvcmos0IODFT - LVCMOS0 IO DFT Control
  Register: PluIOControls_PluLvcmos1IODFT - LVCMOS1 IO DFT Control
    Bits: 11    padrx2txlb(rw) - LVCMOS pins RX to TX internal loopback enable. High active.
    Bits: 10    tsel(rw) - Test mode select for all ILK LVCMOS pins.
    Bits:  9    toenb(rw) - Test output enable. Low active.
    Bits:  8    open_drain(rw) - Open drain mode enable for all ILK LVCMOS pins.
    Bits:  7    force_tristateb(rw) - Tri-state enable for all ILK LVCMOS pins.
    Bits:  6    rxoffseten(rw) - Receiver offset enable for all ILK LVCMOS pins.
    Bits:  5:0  rxoffset(rw) - Receiver offset setting for all ILK LVCMOS pins.
*/
#define NFP_PL_PluIOControls_PLU_LVCMOS0_IO_DFT              0x00040008
#define NFP_PL_PluIOControls_PLU_LVCMOS1_IO_DFT              0x00040028
#define   NFP_PL_PLUIOCONTROLS_PLULVCMOSIODFT_PADRX2TXLB     BIT(11)
#define   NFP_PL_PLUIOCONTROLS_PLULVCMOSIODFT_TSEL           BIT(10)
#define     NFP_PL_PLUIOCONTROLS_PLULVCMOSIODFT_TSEL_0       (0 << 10)
#define     NFP_PL_PLUIOCONTROLS_PLULVCMOSIODFT_TSEL_1       BIT(10)
#define   NFP_PL_PLUIOCONTROLS_PLULVCMOSIODFT_TOENB          BIT(9)
#define   NFP_PL_PLUIOCONTROLS_PLULVCMOSIODFT_OPEN_DRAIN     BIT(8)
#define   NFP_PL_PLUIOCONTROLS_PLULVCMOSIODFT_FORCE_TRISTATEB BIT(7)
#define   NFP_PL_PLUIOCONTROLS_PLULVCMOSIODFT_RXOFFSETEN     BIT(6)
#define   NFP_PL_PLUIOCONTROLS_PLULVCMOSIODFT_RXOFFSET(_x)   (((_x) & 0x3f) << 0)
#define   NFP_PL_PLUIOCONTROLS_PLULVCMOSIODFT_RXOFFSET_of(_x) (((_x) >> 0) & 0x3f)
/*----------------------------------------------------------------
  Register: PluIOControls_PluLvcmos0IODFT2 - For debug only.
  Register: PluIOControls_PluLvcmos1IODFT2 - For debug only.
    Bits: 22    ilk1_rdata_pododtmode(rw) - For debug only.
    Bits: 21    ilk1_rdata_dlycellmux(rw) - For debug only.
    Bits: 20    ilk1_rdata_dlycellstaticleg(rw) - For debug only.
    Bits: 19    ilk1_rdata_odtenb(rw) - For debug only.
    Bits: 18    ilk1_rdata_pdndrvodtrcompstatic(rw) - For debug only.
    Bits: 17    ilk1_rdata_pdndrvrcompstatic(rw) - For debug only.
    Bits: 16    ilk1_rdata_pupdrvodtrcompstatic(rw) - For debug only.
    Bits: 15    ilk1_rdata_pupdrvrcompstatic(rw) - For debug only.
    Bits: 14    ilk1_rdata_rxen1_15(rw) - For debug only.
    Bits: 13    ilk1_rdata_rxen1_h1x(rw) - For debug only.
    Bits: 12    ilk1_rdata_rxen2_15(rw) - For debug only.
    Bits: 11    ilk1_rdata_rxen2_h1x(rw) - For debug only.
    Bits: 10    ilk1_rdata_specmodesel(rw) - For debug only.
    Bits:  9:8  ilk1_rdata_drvodtsegen(rw) - For debug only.
    Bits:  7:6  ilk1_rdata_odtmode(rw) - For debug only.
    Bits:  5:4  ilk1_rdata_rxpathsel(rw) - For debug only.
    Bits:  3:0  ilk1_rdata_drvsegen(rw) - For debug only.
*/
#define NFP_PL_PluIOControls_PLU_LVCMOS0_IO_DFT2             0x0004000c
#define NFP_PL_PluIOControls_PLU_LVCMOS1_IO_DFT2             0x0004002c
#define   NFP_PL_PLUIOCONTROLS_PLULVCMOSIODFT2_ILK1_RDATA_PODODTMODE BIT(22)
#define   NFP_PL_PLUIOCONTROLS_PLULVCMOSIODFT2_ILK1_RDATA_DLYCELLMUX BIT(21)
#define   NFP_PL_PLUIOCONTROLS_PLULVCMOSIODFT2_ILK1_RDATA_DLYCELLSTATICLEG BIT(20)
#define   NFP_PL_PLUIOCONTROLS_PLULVCMOSIODFT2_ILK1_RDATA_ODTENB BIT(19)
#define   NFP_PL_PLUIOCONTROLS_PLULVCMOSIODFT2_ILK1_RDATA_PDNDRVODTRCOMPSTATIC BIT(18)
#define   NFP_PL_PLUIOCONTROLS_PLULVCMOSIODFT2_ILK1_RDATA_PDNDRVRCOMPSTATIC BIT(17)
#define   NFP_PL_PLUIOCONTROLS_PLULVCMOSIODFT2_ILK1_RDATA_PUPDRVODTRCOMPSTATIC BIT(16)
#define   NFP_PL_PLUIOCONTROLS_PLULVCMOSIODFT2_ILK1_RDATA_PUPDRVRCOMPSTATIC BIT(15)
#define   NFP_PL_PLUIOCONTROLS_PLULVCMOSIODFT2_ILK1_RDATA_RXEN1_15 BIT(14)
#define   NFP_PL_PLUIOCONTROLS_PLULVCMOSIODFT2_ILK1_RDATA_RXEN1_H1X BIT(13)
#define   NFP_PL_PLUIOCONTROLS_PLULVCMOSIODFT2_ILK1_RDATA_RXEN2_15 BIT(12)
#define   NFP_PL_PLUIOCONTROLS_PLULVCMOSIODFT2_ILK1_RDATA_RXEN2_H1X BIT(11)
#define   NFP_PL_PLUIOCONTROLS_PLULVCMOSIODFT2_ILK1_RDATA_SPECMODESEL BIT(10)
#define   NFP_PL_PLUIOCONTROLS_PLULVCMOSIODFT2_ILK1_RDATA_DRVODTSEGEN(_x) (((_x) & 0x3) << 8)
#define   NFP_PL_PLUIOCONTROLS_PLULVCMOSIODFT2_ILK1_RDATA_DRVODTSEGEN_of(_x) (((_x) >> 8) & 0x3)
#define   NFP_PL_PLUIOCONTROLS_PLULVCMOSIODFT2_ILK1_RDATA_ODTMODE(_x) (((_x) & 0x3) << 6)
#define   NFP_PL_PLUIOCONTROLS_PLULVCMOSIODFT2_ILK1_RDATA_ODTMODE_of(_x) (((_x) >> 6) & 0x3)
#define   NFP_PL_PLUIOCONTROLS_PLULVCMOSIODFT2_ILK1_RDATA_RXPATHSEL(_x) (((_x) & 0x3) << 4)
#define   NFP_PL_PLUIOCONTROLS_PLULVCMOSIODFT2_ILK1_RDATA_RXPATHSEL_of(_x) (((_x) >> 4) & 0x3)
#define   NFP_PL_PLUIOCONTROLS_PLULVCMOSIODFT2_ILK1_RDATA_DRVSEGEN(_x) (((_x) & 0xf) << 0)
#define   NFP_PL_PLUIOCONTROLS_PLULVCMOSIODFT2_ILK1_RDATA_DRVSEGEN_of(_x) (((_x) >> 0) & 0xf)
/*----------------------------------------------------------------
  Register: PluIOControls_PluLvcmos0IOComp - LVCMOS0 IO COMP Control.
  Register: PluIOControls_PluLvcmos1IOComp - LVCMOS1 IO COMP Control.
    Bits: 29:27 gdcdrvofst_1(rw) - Analog offset control for RCOMP pull up operation.
    Bits: 26:24 gdcdrvofst_0(rw) - Analog offset control for RCOMP pull down operation.
    Bits: 23:20 driversegen_0(rw) - Driver segment control.
    Bits: 19:18 driverodtsegen_0(rw) - Driver ODT segment control.
    Bits: 17:16 compck_div(rw) - RCOMP clock divider ratio.
    Bits: 15    rcomp_pup_en(rw) - RCOMP function pull up only enable.
    Bits: 14    rcomp_pup_ovrd_en(rw) - RCOMP pull up override enable, use override values.
    Bits: 13:8  pup_drv_ovrd(rw) - Driver pull up drive strength override value.
    Bits:  7    rcomp_pdn_en(rw) - RCOMP function pull down only enable.
    Bits:  6    rcomp_pdn_ovrd_en(rw) - RCOMP pull down override enable, use override values.
    Bits:  5:0  pdn_drv_ovrd(rw) - Driver pull down drive strength override value.
*/
#define NFP_PL_PluIOControls_PLU_LVCMOS0_IO_COMP             0x00040010
#define NFP_PL_PluIOControls_PLU_LVCMOS1_IO_COMP             0x00040030
#define   NFP_PL_PLUIOCONTROLS_PLULVCMOSIOCOMP_GDCDRVOFST_1(_x) (((_x) & 0x7) << 27)
#define   NFP_PL_PLUIOCONTROLS_PLULVCMOSIOCOMP_GDCDRVOFST_1_of(_x) (((_x) >> 27) & 0x7)
#define   NFP_PL_PLUIOCONTROLS_PLULVCMOSIOCOMP_GDCDRVOFST_0(_x) (((_x) & 0x7) << 24)
#define   NFP_PL_PLUIOCONTROLS_PLULVCMOSIOCOMP_GDCDRVOFST_0_of(_x) (((_x) >> 24) & 0x7)
#define   NFP_PL_PLUIOCONTROLS_PLULVCMOSIOCOMP_DRIVERSEGEN_0(_x) (((_x) & 0xf) << 20)
#define   NFP_PL_PLUIOCONTROLS_PLULVCMOSIOCOMP_DRIVERSEGEN_0_of(_x) (((_x) >> 20) & 0xf)
#define   NFP_PL_PLUIOCONTROLS_PLULVCMOSIOCOMP_DRIVERODTSEGEN_0(_x) (((_x) & 0x3) << 18)
#define   NFP_PL_PLUIOCONTROLS_PLULVCMOSIOCOMP_DRIVERODTSEGEN_0_of(_x) (((_x) >> 18) & 0x3)
#define   NFP_PL_PLUIOCONTROLS_PLULVCMOSIOCOMP_COMPCK_DIV(_x) (((_x) & 0x3) << 16)
#define   NFP_PL_PLUIOCONTROLS_PLULVCMOSIOCOMP_COMPCK_DIV_of(_x) (((_x) >> 16) & 0x3)
#define   NFP_PL_PLUIOCONTROLS_PLULVCMOSIOCOMP_RCOMP_PUP_EN  BIT(15)
#define   NFP_PL_PLUIOCONTROLS_PLULVCMOSIOCOMP_RCOMP_PUP_OVRD_EN BIT(14)
#define   NFP_PL_PLUIOCONTROLS_PLULVCMOSIOCOMP_PUP_DRV_OVRD(_x) (((_x) & 0x3f) << 8)
#define   NFP_PL_PLUIOCONTROLS_PLULVCMOSIOCOMP_PUP_DRV_OVRD_of(_x) (((_x) >> 8) & 0x3f)
#define   NFP_PL_PLUIOCONTROLS_PLULVCMOSIOCOMP_RCOMP_PDN_EN  BIT(7)
#define   NFP_PL_PLUIOCONTROLS_PLULVCMOSIOCOMP_RCOMP_PDN_OVRD_EN BIT(6)
#define   NFP_PL_PLUIOCONTROLS_PLULVCMOSIOCOMP_PDN_DRV_OVRD(_x) (((_x) & 0x3f) << 0)
#define   NFP_PL_PLUIOCONTROLS_PLULVCMOSIOCOMP_PDN_DRV_OVRD_of(_x) (((_x) >> 0) & 0x3f)
/*----------------------------------------------------------------
  Register: PluIOControls_PluLvcmos0IOComp2 - LVCMOS0 IO COMP Control 2.
  Register: PluIOControls_PluLvcmos1IOComp2 - LVCMOS1 IO COMP Control 2.
    Bits: 31    rcomp_start(rw) - Start RCOMP Sequence.
    Bits: 30    rcomp_reset(rw) - Synchronous high-active RCOMP Reset.
    Bits: 18    rcomp_ready(ro) - RCOMP ready signal.
    Bits: 17    rcomp_update(ro) - RCOMP FSM update signal.
    Bits: 16    rcomp_timeout(ro) - RCOMP FSM TIMEOUT - ready never came.
    Bits: 13:8  pdn_drv_rcomp(ro) - IO buffer drive strength pull down setting from RCOMP.
    Bits:  5:0  pup_drv_rcomp(ro) - IO buffer drive strength pull up setting from RCOMP.
*/
#define NFP_PL_PluIOControls_PLU_LVCMOS0_IO_COMP2            0x00040014
#define NFP_PL_PluIOControls_PLU_LVCMOS1_IO_COMP2            0x00040034
#define   NFP_PL_PLUIOCONTROLS_PLULVCMOSIOCOMP2_RCOMP_START  BIT(31)
#define   NFP_PL_PLUIOCONTROLS_PLULVCMOSIOCOMP2_RCOMP_RESET  BIT(30)
#define   NFP_PL_PLUIOCONTROLS_PLULVCMOSIOCOMP2_RCOMP_READY  BIT(18)
#define   NFP_PL_PLUIOCONTROLS_PLULVCMOSIOCOMP2_RCOMP_UPDATE BIT(17)
#define   NFP_PL_PLUIOCONTROLS_PLULVCMOSIOCOMP2_RCOMP_TIMEOUT BIT(16)
#define   NFP_PL_PLUIOCONTROLS_PLULVCMOSIOCOMP2_PDN_DRV_RCOMP(_x) (((_x) & 0x3f) << 8)
#define   NFP_PL_PLUIOCONTROLS_PLULVCMOSIOCOMP2_PDN_DRV_RCOMP_of(_x) (((_x) >> 8) & 0x3f)
#define   NFP_PL_PLUIOCONTROLS_PLULVCMOSIOCOMP2_PUP_DRV_RCOMP(_x) (((_x) & 0x3f) << 0)
#define   NFP_PL_PLUIOCONTROLS_PLULVCMOSIOCOMP2_PUP_DRV_RCOMP_of(_x) (((_x) >> 0) & 0x3f)
/*----------------------------------------------------------------
  Register: PluIOControls_GpioB0Drive - GPIO driver controls for B0.
    Bits: 31:16 gpio_b0_pup_drv(rw) - Driver pull up strength for GPIO[7:0].
    Bits: 15:0  gpio_b0_pdn_drv(rw) - Driver pull down strength for GPIO[7:0].
*/
#define NFP_PL_PluIOControls_PLU_GPIO_B0_DRIVE               0x00040038
#define   NFP_PL_PLUIOCONTROLS_PLU_GPIO_B0_DRIVE_GPIO_B0_PUP_DRV(_x) (((_x) & 0xffff) << 16)
#define   NFP_PL_PLUIOCONTROLS_PLU_GPIO_B0_DRIVE_GPIO_B0_PUP_DRV_of(_x) (((_x) >> 16) & 0xffff)
#define   NFP_PL_PLUIOCONTROLS_PLU_GPIO_B0_DRIVE_GPIO_B0_PDN_DRV(_x) (((_x) & 0xffff) << 0)
#define   NFP_PL_PLUIOCONTROLS_PLU_GPIO_B0_DRIVE_GPIO_B0_PDN_DRV_of(_x) (((_x) >> 0) & 0xffff)
/*----------------------------------------------------------------
  Register: PluIOControls_GpioB1Drive - GPIO driver controls for B1.
    Bits: 31:16 gpio_b1_pup_drv(rw) - Driver pull up strength for GPIO[15:8].
    Bits: 15:0  gpio_b1_pdn_drv(rw) - Driver pull down strength for GPIO[15:8].
*/
#define NFP_PL_PluIOControls_PLU_GPIO_B1_DRIVE               0x0004003c
#define   NFP_PL_PLUIOCONTROLS_PLU_GPIO_B1_DRIVE_GPIO_B1_PUP_DRV(_x) (((_x) & 0xffff) << 16)
#define   NFP_PL_PLUIOCONTROLS_PLU_GPIO_B1_DRIVE_GPIO_B1_PUP_DRV_of(_x) (((_x) >> 16) & 0xffff)
#define   NFP_PL_PLUIOCONTROLS_PLU_GPIO_B1_DRIVE_GPIO_B1_PDN_DRV(_x) (((_x) & 0xffff) << 0)
#define   NFP_PL_PLUIOCONTROLS_PLU_GPIO_B1_DRIVE_GPIO_B1_PDN_DRV_of(_x) (((_x) >> 0) & 0xffff)
/*----------------------------------------------------------------
  Register: PluIOControls_GpioB2Drive - GPIO driver controls for B2.
    Bits: 31:16 gpio_b2_pup_drv(rw) - Driver pull up strength for GPIO[23:16].
    Bits: 15:0  gpio_b2_pdn_drv(rw) - Driver pull down strength for GPIO[23:16].
*/
#define NFP_PL_PluIOControls_PLU_GPIO_B2_DRIVE               0x00040040
#define   NFP_PL_PLUIOCONTROLS_PLU_GPIO_B2_DRIVE_GPIO_B2_PUP_DRV(_x) (((_x) & 0xffff) << 16)
#define   NFP_PL_PLUIOCONTROLS_PLU_GPIO_B2_DRIVE_GPIO_B2_PUP_DRV_of(_x) (((_x) >> 16) & 0xffff)
#define   NFP_PL_PLUIOCONTROLS_PLU_GPIO_B2_DRIVE_GPIO_B2_PDN_DRV(_x) (((_x) & 0xffff) << 0)
#define   NFP_PL_PLUIOCONTROLS_PLU_GPIO_B2_DRIVE_GPIO_B2_PDN_DRV_of(_x) (((_x) >> 0) & 0xffff)
/*----------------------------------------------------------------
  Register: PluIOControls_GpioB3Drive - GPIO driver controls for B3.
    Bits: 31:16 gpio_b3_pup_drv(rw) - Driver pull up strength for GPIO[31:24].
    Bits: 15:0  gpio_b3_pdn_drv(rw) - Driver pull down strength for GPIO[31:24].
*/
#define NFP_PL_PluIOControls_PLU_GPIO_B3_DRIVE               0x00040044
#define   NFP_PL_PLUIOCONTROLS_PLU_GPIO_B3_DRIVE_GPIO_B3_PUP_DRV(_x) (((_x) & 0xffff) << 16)
#define   NFP_PL_PLUIOCONTROLS_PLU_GPIO_B3_DRIVE_GPIO_B3_PUP_DRV_of(_x) (((_x) >> 16) & 0xffff)
#define   NFP_PL_PLUIOCONTROLS_PLU_GPIO_B3_DRIVE_GPIO_B3_PDN_DRV(_x) (((_x) & 0xffff) << 0)
#define   NFP_PL_PLUIOCONTROLS_PLU_GPIO_B3_DRIVE_GPIO_B3_PDN_DRV_of(_x) (((_x) >> 0) & 0xffff)
/*----------------------------------------------------------------
  Register: PluIOControls_SpiDrive - SPI driver controls.
    Bits: 31:16 spi_pup_drv(rw) - Driver pull up strength for all SPI pins.
    Bits: 15:0  spi_pdn_drv(rw) - Driver pull down strength for all SPI pins.
*/
#define NFP_PL_PluIOControls_PLU_SPI_DRIVE                   0x00040048
#define   NFP_PL_PLUIOCONTROLS_PLU_SPI_DRIVE_SPI_PUP_DRV(_x) (((_x) & 0xffff) << 16)
#define   NFP_PL_PLUIOCONTROLS_PLU_SPI_DRIVE_SPI_PUP_DRV_of(_x) (((_x) >> 16) & 0xffff)
#define   NFP_PL_PLUIOCONTROLS_PLU_SPI_DRIVE_SPI_PDN_DRV(_x) (((_x) & 0xffff) << 0)
#define   NFP_PL_PLUIOCONTROLS_PLU_SPI_DRIVE_SPI_PDN_DRV_of(_x) (((_x) >> 0) & 0xffff)
/*----------------------------------------------------------------
  Register: PluIOControls_MiscDrive - Misc. driver controls.
    Bits: 31:16 misc_pup_drv(rw) - Driver pull up strength for all remaining LVTTL IO pins.
    Bits: 15:0  misc_pdn_drv(rw) - Driver pull down strength for all remaining LVTTL IO pins.
*/
#define NFP_PL_PluIOControls_PLU_MISC_DRIVE                  0x0004004c
#define   NFP_PL_PLUIOCONTROLS_PLU_MISC_DRIVE_MISC_PUP_DRV(_x) (((_x) & 0xffff) << 16)
#define   NFP_PL_PLUIOCONTROLS_PLU_MISC_DRIVE_MISC_PUP_DRV_of(_x) (((_x) >> 16) & 0xffff)
#define   NFP_PL_PLUIOCONTROLS_PLU_MISC_DRIVE_MISC_PDN_DRV(_x) (((_x) & 0xffff) << 0)
#define   NFP_PL_PLUIOCONTROLS_PLU_MISC_DRIVE_MISC_PDN_DRV_of(_x) (((_x) >> 0) & 0xffff)
/*----------------------------------------------------------------
  Register: PluIOControls_LvttlIoSlew - Drive slew rate for LVTTL IO buffers.
    Bits: 23:22 misc_pup_slew(rw) - Driver pull up slew rate for all remaining LVTTL IO pins.
    Bits: 21:20 misc_pdn_slew(rw) - Driver pull down slew rate for all remaining LVTTL IO pins.
    Bits: 19:18 spi_pup_slew(rw) - Driver pull up slew rate for all SPI pins.
    Bits: 17:16 spi_pdn_slew(rw) - Driver pull down slew rate for all SPI pins.
    Bits: 15:14 gpio_b3_pup_slew(rw) - Driver pull up slew rate for GPIO[31:24].
    Bits: 13:12 gpio_b3_pdn_slew(rw) - Driver pull down slew rate for GPIO[31:24].
    Bits: 11:10 gpio_b2_pup_slew(rw) - Driver pull up slew rate for GPIO[23:16].
    Bits:  9:8  gpio_b2_pdn_slew(rw) - Driver pull down slew rate for GPIO[23:16].
    Bits:  7:6  gpio_b1_pup_slew(rw) - Driver pull up slew rate for GPIO[15:8].
    Bits:  5:4  gpio_b1_pdn_slew(rw) - Driver pull down slew rate for GPIO[15:8].
    Bits:  3:2  gpio_b0_pup_slew(rw) - Driver pull up slew rate for GPIO[7:0].
    Bits:  1:0  gpio_b0_pdn_slew(rw) - Driver pull down slew rate for GPIO[7:0].
*/
#define NFP_PL_PluIOControls_PLU_LVTTL_IO_SLEW               0x00040050
#define   NFP_PL_PLUIOCONTROLS_PLU_LVTTL_IO_SLEW_MISC_PUP_SLEW(_x) (((_x) & 0x3) << 22)
#define   NFP_PL_PLUIOCONTROLS_PLU_LVTTL_IO_SLEW_MISC_PUP_SLEW_of(_x) (((_x) >> 22) & 0x3)
#define   NFP_PL_PLUIOCONTROLS_PLU_LVTTL_IO_SLEW_MISC_PDN_SLEW(_x) (((_x) & 0x3) << 20)
#define   NFP_PL_PLUIOCONTROLS_PLU_LVTTL_IO_SLEW_MISC_PDN_SLEW_of(_x) (((_x) >> 20) & 0x3)
#define   NFP_PL_PLUIOCONTROLS_PLU_LVTTL_IO_SLEW_SPI_PUP_SLEW(_x) (((_x) & 0x3) << 18)
#define   NFP_PL_PLUIOCONTROLS_PLU_LVTTL_IO_SLEW_SPI_PUP_SLEW_of(_x) (((_x) >> 18) & 0x3)
#define   NFP_PL_PLUIOCONTROLS_PLU_LVTTL_IO_SLEW_SPI_PDN_SLEW(_x) (((_x) & 0x3) << 16)
#define   NFP_PL_PLUIOCONTROLS_PLU_LVTTL_IO_SLEW_SPI_PDN_SLEW_of(_x) (((_x) >> 16) & 0x3)
#define   NFP_PL_PLUIOCONTROLS_PLU_LVTTL_IO_SLEW_GPIO_B3_PUP_SLEW(_x) (((_x) & 0x3) << 14)
#define   NFP_PL_PLUIOCONTROLS_PLU_LVTTL_IO_SLEW_GPIO_B3_PUP_SLEW_of(_x) (((_x) >> 14) & 0x3)
#define   NFP_PL_PLUIOCONTROLS_PLU_LVTTL_IO_SLEW_GPIO_B3_PDN_SLEW(_x) (((_x) & 0x3) << 12)
#define   NFP_PL_PLUIOCONTROLS_PLU_LVTTL_IO_SLEW_GPIO_B3_PDN_SLEW_of(_x) (((_x) >> 12) & 0x3)
#define   NFP_PL_PLUIOCONTROLS_PLU_LVTTL_IO_SLEW_GPIO_B2_PUP_SLEW(_x) (((_x) & 0x3) << 10)
#define   NFP_PL_PLUIOCONTROLS_PLU_LVTTL_IO_SLEW_GPIO_B2_PUP_SLEW_of(_x) (((_x) >> 10) & 0x3)
#define   NFP_PL_PLUIOCONTROLS_PLU_LVTTL_IO_SLEW_GPIO_B2_PDN_SLEW(_x) (((_x) & 0x3) << 8)
#define   NFP_PL_PLUIOCONTROLS_PLU_LVTTL_IO_SLEW_GPIO_B2_PDN_SLEW_of(_x) (((_x) >> 8) & 0x3)
#define   NFP_PL_PLUIOCONTROLS_PLU_LVTTL_IO_SLEW_GPIO_B1_PUP_SLEW(_x) (((_x) & 0x3) << 6)
#define   NFP_PL_PLUIOCONTROLS_PLU_LVTTL_IO_SLEW_GPIO_B1_PUP_SLEW_of(_x) (((_x) >> 6) & 0x3)
#define   NFP_PL_PLUIOCONTROLS_PLU_LVTTL_IO_SLEW_GPIO_B1_PDN_SLEW(_x) (((_x) & 0x3) << 4)
#define   NFP_PL_PLUIOCONTROLS_PLU_LVTTL_IO_SLEW_GPIO_B1_PDN_SLEW_of(_x) (((_x) >> 4) & 0x3)
#define   NFP_PL_PLUIOCONTROLS_PLU_LVTTL_IO_SLEW_GPIO_B0_PUP_SLEW(_x) (((_x) & 0x3) << 2)
#define   NFP_PL_PLUIOCONTROLS_PLU_LVTTL_IO_SLEW_GPIO_B0_PUP_SLEW_of(_x) (((_x) >> 2) & 0x3)
#define   NFP_PL_PLUIOCONTROLS_PLU_LVTTL_IO_SLEW_GPIO_B0_PDN_SLEW(_x) (((_x) & 0x3) << 0)
#define   NFP_PL_PLUIOCONTROLS_PLU_LVTTL_IO_SLEW_GPIO_B0_PDN_SLEW_of(_x) (((_x) >> 0) & 0x3)
/*----------------------------------------------------------------
  Register: PluIOControls_GpioWkpullEn - Weak pull up pull down for GPIO.
    Bits: 31:0  gpio_wkpull_en(rw) - Weak pull up pull down enable for GPIO[31:0] (1: weak pull up/down enabled, 0: weak pull up/down disabled).
*/
#define NFP_PL_PluIOControls_PLU_GPIO_WKPULL_EN              0x00040054
#define   NFP_PL_PLUIOCONTROLS_PLU_GPIO_WKPULL_EN_GPIO_WKPULL_EN(_x) (((_x) & 0xffffffff) << 0)
#define   NFP_PL_PLUIOCONTROLS_PLU_GPIO_WKPULL_EN_GPIO_WKPULL_EN_of(_x) (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: PluIOControls_GpioWkPolarity - Weak pull polarity for GPIO.
    Bits: 31:0  gpio_wk_polarity(rw) - Weak pull polarity for GPIO[31:0] (1: weak pull up effective, 0: weal pull down effective).
*/
#define NFP_PL_PluIOControls_PLU_GPIO_WK_POLARITY            0x00040058
#define   NFP_PL_PLUIOCONTROLS_PLU_GPIO_WK_POLARITY_GPIO_WK_POLARITY(_x) (((_x) & 0xffffffff) << 0)
#define   NFP_PL_PLUIOCONTROLS_PLU_GPIO_WK_POLARITY_GPIO_WK_POLARITY_of(_x) (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: PluIOControls_GpioWkpullVal - Weak pull up pull down value for GPIO.
    Bits: 31:0  gpio_wkpull_value(rw) - Weak pull up pull down value for GPIO[31:0] (1: 20kohm, 0: 2kohm).
*/
#define NFP_PL_PluIOControls_PLU_GPIO_WKPULL_VAL             0x0004005c
#define   NFP_PL_PLUIOCONTROLS_PLU_GPIO_WKPULL_VAL_GPIO_WKPULL_VALUE(_x) (((_x) & 0xffffffff) << 0)
#define   NFP_PL_PLUIOCONTROLS_PLU_GPIO_WKPULL_VAL_GPIO_WKPULL_VALUE_of(_x) (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: PluIOControls_SrTestTxEnb - Buffer output enable for Serial port, backup test pins.
    Bits:  5    test_backup3_oenb(rw) - Buffer output enable for backup3 pin.
    Bits:  4    test_backup2_oenb(rw) - Buffer output enable for backup2 pin.
    Bits:  3    test_backup1_oenb(rw) - Buffer output enable for backup1 pin.
    Bits:  2    test_backup0_oenb(rw) - Buffer output enable for backup0 pin.
    Bits:  1    sr_tx_oenb(rw) - Buffer output enable for Serial port (1: off, 0: on).
    Bits:  0    sr_rx_oenb(rw) - Buffer output enable for Serial port (1: off, 0: on).
*/
#define NFP_PL_PluIOControls_PLU_SR_TEST_TX_ENB              0x00040060
#define   NFP_PL_PLUIOCONTROLS_PLU_SR_TEST_TX_ENB_TEST_BACKUP3_OENB BIT(5)
#define   NFP_PL_PLUIOCONTROLS_PLU_SR_TEST_TX_ENB_TEST_BACKUP2_OENB BIT(4)
#define   NFP_PL_PLUIOCONTROLS_PLU_SR_TEST_TX_ENB_TEST_BACKUP1_OENB BIT(3)
#define   NFP_PL_PLUIOCONTROLS_PLU_SR_TEST_TX_ENB_TEST_BACKUP0_OENB BIT(2)
#define   NFP_PL_PLUIOCONTROLS_PLU_SR_TEST_TX_ENB_SR_TX_OENB BIT(1)
#define   NFP_PL_PLUIOCONTROLS_PLU_SR_TEST_TX_ENB_SR_RX_OENB BIT(0)
/*----------------------------------------------------------------
  Register: PluIOControls_GpioRxEnb - Buffer input enable for GPIO.
    Bits: 31:0  gpio_rcvenv(rw) - Buffer input enable for GPIO[31:0], active low.
*/
#define NFP_PL_PluIOControls_PLU_GPIO_RX_ENB                 0x00040064
#define   NFP_PL_PLUIOCONTROLS_PLU_GPIO_RX_ENB_GPIO_RCVENV(_x) (((_x) & 0xffffffff) << 0)
#define   NFP_PL_PLUIOCONTROLS_PLU_GPIO_RX_ENB_GPIO_RCVENV_of(_x) (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: PluIOControls_SpiRxEnb - Buffer input enable for SPI, serial port and backup test pins.
    Bits: 23    test_backup3_rcvenb(rw) - Buffer input enable for test backup pins.
    Bits: 22    test_backup2_rcvenb(rw) - Buffer input enable for test backup pins.
    Bits: 21    test_backup1_rcvenb(rw) - Buffer input enable for test backup pins.
    Bits: 20    test_backup0_rcvenb(rw) - Buffer input enable for test backup pins.
    Bits: 19    sr_tx_rcvenb(rw) - Buffer input enable for Serial port pins (0: on, 1: off).
    Bits: 18    sr_rx_rcvenb(rw) - Buffer input enable for Serial port pins (0: on, 1: off).
    Bits: 17    spix_sel3_rcvenb(rw) - Buffer input enable for SPI pins.
    Bits: 16    spix_sel2_rcvenb(rw) - Buffer input enable for SPI pins.
    Bits: 15    spix_sel1_rcvenb(rw) - Buffer input enable for SPI pins.
    Bits: 14    spix_hold_l_rcvenb(rw) - Buffer input enable for SPI pins.
    Bits: 13    spix_d3_rcvenb(rw) - Buffer input enable for SPI pins.
    Bits: 12    spix_d2_rcvenb(rw) - Buffer input enable for SPI pins.
    Bits: 11    spi3_sck_rcvenb(rw) - Buffer input enable for SPI pins.
    Bits: 10    spi3_mosi_rcvenb(rw) - Buffer input enable for SPI pins.
    Bits:  9    spi2_sck_rcvenb(rw) - Buffer input enable for SPI pins.
    Bits:  8    spi2_mosi_rcvenb(rw) - Buffer input enable for SPI pins.
    Bits:  7    spi1_cfg_sel_rcvenb(rw) - Buffer input enable for SPI pins.
    Bits:  6    spi1_cfg_sck_rcvenb(rw) - Buffer input enable for SPI pins.
    Bits:  5    spi1_cfg_mosi_rcvenb(rw) - Buffer input enable for SPI pins.
    Bits:  4    spi1_cfg_miso_rcvenb(rw) - Buffer input enable for SPI pins.
    Bits:  3    spi0_sel_rcvenb(rw) - Buffer input enable for SPI pins.
    Bits:  2    spi0_sck_rcvenb(rw) - Buffer input enable for SPI pins.
    Bits:  1    spi0_mosi_rcvenb(rw) - Buffer input enable for SPI pins.
    Bits:  0    spi0_miso_rcvenb(rw) - Buffer input enable for SPI pins.
*/
#define NFP_PL_PluIOControls_PLU_SPI_RX_ENB                  0x00040068
#define   NFP_PL_PLUIOCONTROLS_PLU_SPI_RX_ENB_TEST_BACKUP3_RCVENB BIT(23)
#define   NFP_PL_PLUIOCONTROLS_PLU_SPI_RX_ENB_TEST_BACKUP2_RCVENB BIT(22)
#define   NFP_PL_PLUIOCONTROLS_PLU_SPI_RX_ENB_TEST_BACKUP1_RCVENB BIT(21)
#define   NFP_PL_PLUIOCONTROLS_PLU_SPI_RX_ENB_TEST_BACKUP0_RCVENB BIT(20)
#define   NFP_PL_PLUIOCONTROLS_PLU_SPI_RX_ENB_SR_TX_RCVENB   BIT(19)
#define   NFP_PL_PLUIOCONTROLS_PLU_SPI_RX_ENB_SR_RX_RCVENB   BIT(18)
#define   NFP_PL_PLUIOCONTROLS_PLU_SPI_RX_ENB_SPIX_SEL3_RCVENB BIT(17)
#define   NFP_PL_PLUIOCONTROLS_PLU_SPI_RX_ENB_SPIX_SEL2_RCVENB BIT(16)
#define   NFP_PL_PLUIOCONTROLS_PLU_SPI_RX_ENB_SPIX_SEL1_RCVENB BIT(15)
#define   NFP_PL_PLUIOCONTROLS_PLU_SPI_RX_ENB_SPIX_HOLD_L_RCVENB BIT(14)
#define   NFP_PL_PLUIOCONTROLS_PLU_SPI_RX_ENB_SPIX_D3_RCVENB BIT(13)
#define   NFP_PL_PLUIOCONTROLS_PLU_SPI_RX_ENB_SPIX_D2_RCVENB BIT(12)
#define   NFP_PL_PLUIOCONTROLS_PLU_SPI_RX_ENB_SPI3_SCK_RCVENB BIT(11)
#define   NFP_PL_PLUIOCONTROLS_PLU_SPI_RX_ENB_SPI3_MOSI_RCVENB BIT(10)
#define   NFP_PL_PLUIOCONTROLS_PLU_SPI_RX_ENB_SPI2_SCK_RCVENB BIT(9)
#define   NFP_PL_PLUIOCONTROLS_PLU_SPI_RX_ENB_SPI2_MOSI_RCVENB BIT(8)
#define   NFP_PL_PLUIOCONTROLS_PLU_SPI_RX_ENB_SPI1_CFG_SEL_RCVENB BIT(7)
#define   NFP_PL_PLUIOCONTROLS_PLU_SPI_RX_ENB_SPI1_CFG_SCK_RCVENB BIT(6)
#define   NFP_PL_PLUIOCONTROLS_PLU_SPI_RX_ENB_SPI1_CFG_MOSI_RCVENB BIT(5)
#define   NFP_PL_PLUIOCONTROLS_PLU_SPI_RX_ENB_SPI1_CFG_MISO_RCVENB BIT(4)
#define   NFP_PL_PLUIOCONTROLS_PLU_SPI_RX_ENB_SPI0_SEL_RCVENB BIT(3)
#define   NFP_PL_PLUIOCONTROLS_PLU_SPI_RX_ENB_SPI0_SCK_RCVENB BIT(2)
#define   NFP_PL_PLUIOCONTROLS_PLU_SPI_RX_ENB_SPI0_MOSI_RCVENB BIT(1)
#define   NFP_PL_PLUIOCONTROLS_PLU_SPI_RX_ENB_SPI0_MISO_RCVENB BIT(0)
/*----------------------------------------------------------------
  Register: PluIOControls_GpioOpendrain - Open drain enable for GPIO.
    Bits: 31:0  gpio_opendrn(rw) - Open drain enable for GPIO[31:0].
*/
#define NFP_PL_PluIOControls_PLU_GPIO_OPENDRAIN              0x0004006c
#define   NFP_PL_PLUIOCONTROLS_PLU_GPIO_OPENDRAIN_GPIO_OPENDRN(_x) (((_x) & 0xffffffff) << 0)
#define   NFP_PL_PLUIOCONTROLS_PLU_GPIO_OPENDRAIN_GPIO_OPENDRN_of(_x) (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: PluIOControls_MiscOpendrain - Open drain enable for SPI, reset output and other IO pins.
    Bits: 28    test_opendrn(rw) - Open drain enable for test pin.
    Bits: 27    misc_opendrn(rw) - Open drain enable for misc pin.
    Bits: 26    test_backup3_opendrn(rw) - Open drain enable for test backup pins.
    Bits: 25    test_backup2_opendrn(rw) - Open drain enable for test backup pins.
    Bits: 24    test_backup1_opendrn(rw) - Open drain enable for test backup pins.
    Bits: 23    test_backup0_opendrn(rw) - Open drain enable for test backup pins.
    Bits: 22    sr_tx_opendrn(rw) - Open drain enable for Serial pins.
    Bits: 21    pcie3_reset_out_l_opendrn(rw) - Open drain enable for PCIE reset output pins.
    Bits: 20    pcie2_reset_out_l_opendrn(rw) - Open drain enable for PCIE reset output pins.
    Bits: 19    pcie1_reset_out_l_opendrn(rw) - Open drain enable for PCIE reset output pins.
    Bits: 18    pcie0_reset_out_l_opendrn(rw) - Open drain enable for PCIE reset output pins.
    Bits: 17    clk_nreset_out_l_opendrn(rw) - Open drain enable for reset output pins.
    Bits: 16    spix_sel3_opendrn(rw) - Open drain enable for SPI pins.
    Bits: 15    spix_sel2_opendrn(rw) - Open drain enable for SPI pins.
    Bits: 14    spix_sel1_opendrn(rw) - Open drain enable for SPI pins.
    Bits: 13    spix_d3_opendrn(rw) - Open drain enable for SPI pins.
    Bits: 12    spix_d2_opendrn(rw) - Open drain enable for SPI pins.
    Bits: 11    spi3_sck_opendrn(rw) - Open drain enable for SPI pins.
    Bits: 10    spi3_mosi_opendrn(rw) - Open drain enable for SPI pins.
    Bits:  9    spi2_sck_opendrn(rw) - Open drain enable for SPI pins.
    Bits:  8    spi2_mosi_opendrn(rw) - Open drain enable for SPI pins.
    Bits:  7    spi1_cfg_sel_opendrn(rw) - Open drain enable for SPI pins.
    Bits:  6    spi1_cfg_sck_opendrn(rw) - Open drain enable for SPI pins.
    Bits:  5    spi1_cfg_mosi_opendrn(rw) - Open drain enable for SPI pins.
    Bits:  4    spi1_cfg_miso_opendrn(rw) - Open drain enable for SPI pins.
    Bits:  3    spi0_sel_opendrn(rw) - Open drain enable for SPI pins.
    Bits:  2    spi0_sck_opendrn(rw) - Open drain enable for SPI pins.
    Bits:  1    spi0_mosi_opendrn(rw) - Open drain enable for SPI pins.
    Bits:  0    spi0_miso_opendrn(rw) - Open drain enable for SPI pins.
*/
#define NFP_PL_PluIOControls_PLU_MISC_OPENDRAIN              0x00040070
#define   NFP_PL_PLUIOCONTROLS_PLU_MISC_OPENDRAIN_TEST_OPENDRN BIT(28)
#define   NFP_PL_PLUIOCONTROLS_PLU_MISC_OPENDRAIN_MISC_OPENDRN BIT(27)
#define   NFP_PL_PLUIOCONTROLS_PLU_MISC_OPENDRAIN_TEST_BACKUP3_OPENDRN BIT(26)
#define   NFP_PL_PLUIOCONTROLS_PLU_MISC_OPENDRAIN_TEST_BACKUP2_OPENDRN BIT(25)
#define   NFP_PL_PLUIOCONTROLS_PLU_MISC_OPENDRAIN_TEST_BACKUP1_OPENDRN BIT(24)
#define   NFP_PL_PLUIOCONTROLS_PLU_MISC_OPENDRAIN_TEST_BACKUP0_OPENDRN BIT(23)
#define   NFP_PL_PLUIOCONTROLS_PLU_MISC_OPENDRAIN_SR_TX_OPENDRN BIT(22)
#define   NFP_PL_PLUIOCONTROLS_PLU_MISC_OPENDRAIN_PCIE3_RESET_OUT_L_OPENDRN BIT(21)
#define   NFP_PL_PLUIOCONTROLS_PLU_MISC_OPENDRAIN_PCIE2_RESET_OUT_L_OPENDRN BIT(20)
#define   NFP_PL_PLUIOCONTROLS_PLU_MISC_OPENDRAIN_PCIE1_RESET_OUT_L_OPENDRN BIT(19)
#define   NFP_PL_PLUIOCONTROLS_PLU_MISC_OPENDRAIN_PCIE0_RESET_OUT_L_OPENDRN BIT(18)
#define   NFP_PL_PLUIOCONTROLS_PLU_MISC_OPENDRAIN_CLK_NRESET_OUT_L_OPENDRN BIT(17)
#define   NFP_PL_PLUIOCONTROLS_PLU_MISC_OPENDRAIN_SPIX_SEL3_OPENDRN BIT(16)
#define   NFP_PL_PLUIOCONTROLS_PLU_MISC_OPENDRAIN_SPIX_SEL2_OPENDRN BIT(15)
#define   NFP_PL_PLUIOCONTROLS_PLU_MISC_OPENDRAIN_SPIX_SEL1_OPENDRN BIT(14)
#define   NFP_PL_PLUIOCONTROLS_PLU_MISC_OPENDRAIN_SPIX_D3_OPENDRN BIT(13)
#define   NFP_PL_PLUIOCONTROLS_PLU_MISC_OPENDRAIN_SPIX_D2_OPENDRN BIT(12)
#define   NFP_PL_PLUIOCONTROLS_PLU_MISC_OPENDRAIN_SPI3_SCK_OPENDRN BIT(11)
#define   NFP_PL_PLUIOCONTROLS_PLU_MISC_OPENDRAIN_SPI3_MOSI_OPENDRN BIT(10)
#define   NFP_PL_PLUIOCONTROLS_PLU_MISC_OPENDRAIN_SPI2_SCK_OPENDRN BIT(9)
#define   NFP_PL_PLUIOCONTROLS_PLU_MISC_OPENDRAIN_SPI2_MOSI_OPENDRN BIT(8)
#define   NFP_PL_PLUIOCONTROLS_PLU_MISC_OPENDRAIN_SPI1_CFG_SEL_OPENDRN BIT(7)
#define   NFP_PL_PLUIOCONTROLS_PLU_MISC_OPENDRAIN_SPI1_CFG_SCK_OPENDRN BIT(6)
#define   NFP_PL_PLUIOCONTROLS_PLU_MISC_OPENDRAIN_SPI1_CFG_MOSI_OPENDRN BIT(5)
#define   NFP_PL_PLUIOCONTROLS_PLU_MISC_OPENDRAIN_SPI1_CFG_MISO_OPENDRN BIT(4)
#define   NFP_PL_PLUIOCONTROLS_PLU_MISC_OPENDRAIN_SPI0_SEL_OPENDRN BIT(3)
#define   NFP_PL_PLUIOCONTROLS_PLU_MISC_OPENDRAIN_SPI0_SCK_OPENDRN BIT(2)
#define   NFP_PL_PLUIOCONTROLS_PLU_MISC_OPENDRAIN_SPI0_MOSI_OPENDRN BIT(1)
#define   NFP_PL_PLUIOCONTROLS_PLU_MISC_OPENDRAIN_SPI0_MISO_OPENDRN BIT(0)
/*----------------------------------------------------------------
  Register: PluIOControls_MiscControl - Misc controls.
    Bits: 13    test_force0(rw) - Force 0 on test pins.
    Bits: 12    test_force_tristate(rw) - Force tristate on test pins.
    Bits: 11    misc_force0(rw) - Force 0 on misc pins.
    Bits: 10    misc_force_tristate(rw) - Force tristate on misc pins.
    Bits:  9    spi_force0(rw) - Force 0 on spi pins.
    Bits:  8    spi_force_tristate(rw) - Force tristate on spi pins.
    Bits:  7    gpio_force0(rw) - Force 0 on gpio pins.
    Bits:  6    gpio_force_tristate(rw) - Force tristate on gpio pins.
    Bits:  5    misc_wkpull_value(rw) - Weak pull up pull down value for misc pins.
    Bits:  4    misc_wk_polarity(rw) - Weak pull polarity for misc pins.
    Bits:  3    misc_wkpull_en(rw) - Weak pull up pull down enable for misc pins.
    Bits:  2    spi_wkpull_value(rw) - Weak pull up pull down value for SPI pins.
    Bits:  1    spi_wk_polarity(rw) - Weak pull polarity for SPI pins.
    Bits:  0    spi_wkpull_en(rw) - Weak pull up pull down enable for SPI pins.
*/
#define NFP_PL_PluIOControls_PLU_MISC_CONTROL                0x00040074
#define   NFP_PL_PLUIOCONTROLS_PLU_MISC_CONTROL_TEST_FORCE0  BIT(13)
#define     NFP_PL_PLUIOCONTROLS_PLU_MISC_CONTROL_TEST_FORCE0_0 (0 << 13)
#define     NFP_PL_PLUIOCONTROLS_PLU_MISC_CONTROL_TEST_FORCE0_1 BIT(13)
#define   NFP_PL_PLUIOCONTROLS_PLU_MISC_CONTROL_TEST_FORCE_TRISTATE BIT(12)
#define     NFP_PL_PLUIOCONTROLS_PLU_MISC_CONTROL_TEST_FORCE_TRISTATE_0 (0 << 12)
#define     NFP_PL_PLUIOCONTROLS_PLU_MISC_CONTROL_TEST_FORCE_TRISTATE_1 BIT(12)
#define   NFP_PL_PLUIOCONTROLS_PLU_MISC_CONTROL_MISC_FORCE0  BIT(11)
#define     NFP_PL_PLUIOCONTROLS_PLU_MISC_CONTROL_MISC_FORCE0_0 (0 << 11)
#define     NFP_PL_PLUIOCONTROLS_PLU_MISC_CONTROL_MISC_FORCE0_1 BIT(11)
#define   NFP_PL_PLUIOCONTROLS_PLU_MISC_CONTROL_MISC_FORCE_TRISTATE BIT(10)
#define     NFP_PL_PLUIOCONTROLS_PLU_MISC_CONTROL_MISC_FORCE_TRISTATE_0 (0 << 10)
#define     NFP_PL_PLUIOCONTROLS_PLU_MISC_CONTROL_MISC_FORCE_TRISTATE_1 BIT(10)
#define   NFP_PL_PLUIOCONTROLS_PLU_MISC_CONTROL_SPI_FORCE0   BIT(9)
#define     NFP_PL_PLUIOCONTROLS_PLU_MISC_CONTROL_SPI_FORCE0_0 (0 << 9)
#define     NFP_PL_PLUIOCONTROLS_PLU_MISC_CONTROL_SPI_FORCE0_1 BIT(9)
#define   NFP_PL_PLUIOCONTROLS_PLU_MISC_CONTROL_SPI_FORCE_TRISTATE BIT(8)
#define     NFP_PL_PLUIOCONTROLS_PLU_MISC_CONTROL_SPI_FORCE_TRISTATE_0 (0 << 8)
#define     NFP_PL_PLUIOCONTROLS_PLU_MISC_CONTROL_SPI_FORCE_TRISTATE_1 BIT(8)
#define   NFP_PL_PLUIOCONTROLS_PLU_MISC_CONTROL_GPIO_FORCE0  BIT(7)
#define     NFP_PL_PLUIOCONTROLS_PLU_MISC_CONTROL_GPIO_FORCE0_0 (0 << 7)
#define     NFP_PL_PLUIOCONTROLS_PLU_MISC_CONTROL_GPIO_FORCE0_1 BIT(7)
#define   NFP_PL_PLUIOCONTROLS_PLU_MISC_CONTROL_GPIO_FORCE_TRISTATE BIT(6)
#define     NFP_PL_PLUIOCONTROLS_PLU_MISC_CONTROL_GPIO_FORCE_TRISTATE_0 (0 << 6)
#define     NFP_PL_PLUIOCONTROLS_PLU_MISC_CONTROL_GPIO_FORCE_TRISTATE_1 BIT(6)
#define   NFP_PL_PLUIOCONTROLS_PLU_MISC_CONTROL_MISC_WKPULL_VALUE BIT(5)
#define     NFP_PL_PLUIOCONTROLS_PLU_MISC_CONTROL_MISC_WKPULL_VALUE_0 (0 << 5)
#define     NFP_PL_PLUIOCONTROLS_PLU_MISC_CONTROL_MISC_WKPULL_VALUE_1 BIT(5)
#define   NFP_PL_PLUIOCONTROLS_PLU_MISC_CONTROL_MISC_WK_POLARITY BIT(4)
#define     NFP_PL_PLUIOCONTROLS_PLU_MISC_CONTROL_MISC_WK_POLARITY_0 (0 << 4)
#define     NFP_PL_PLUIOCONTROLS_PLU_MISC_CONTROL_MISC_WK_POLARITY_1 BIT(4)
#define   NFP_PL_PLUIOCONTROLS_PLU_MISC_CONTROL_MISC_WKPULL_EN BIT(3)
#define     NFP_PL_PLUIOCONTROLS_PLU_MISC_CONTROL_MISC_WKPULL_EN_0 (0 << 3)
#define     NFP_PL_PLUIOCONTROLS_PLU_MISC_CONTROL_MISC_WKPULL_EN_1 BIT(3)
#define   NFP_PL_PLUIOCONTROLS_PLU_MISC_CONTROL_SPI_WKPULL_VALUE BIT(2)
#define     NFP_PL_PLUIOCONTROLS_PLU_MISC_CONTROL_SPI_WKPULL_VALUE_0 (0 << 2)
#define     NFP_PL_PLUIOCONTROLS_PLU_MISC_CONTROL_SPI_WKPULL_VALUE_1 BIT(2)
#define   NFP_PL_PLUIOCONTROLS_PLU_MISC_CONTROL_SPI_WK_POLARITY BIT(1)
#define     NFP_PL_PLUIOCONTROLS_PLU_MISC_CONTROL_SPI_WK_POLARITY_0 (0 << 1)
#define     NFP_PL_PLUIOCONTROLS_PLU_MISC_CONTROL_SPI_WK_POLARITY_1 BIT(1)
#define   NFP_PL_PLUIOCONTROLS_PLU_MISC_CONTROL_SPI_WKPULL_EN BIT(0)
#define     NFP_PL_PLUIOCONTROLS_PLU_MISC_CONTROL_SPI_WKPULL_EN_0 (0 << 0)
#define     NFP_PL_PLUIOCONTROLS_PLU_MISC_CONTROL_SPI_WKPULL_EN_1 BIT(0)
/*----------------------------------------------------------------
  Register: PluIOControls_VidControl - VID controls.
    Bits: 15    vid_wk_polarity(rw) - Weak pull polarity for VID pins.
    Bits: 14    vid_wk_pull_en(rw) - Weak pull enable for VID pins.
    Bits: 13:8  vid_pdn_drv_rcomp(rw) - VID pins pull down drive strength.
    Bits:  7:2  vid_pup_drv_rcomp(rw) - VID pins pull up drive strength.
    Bits:  1    vid_opendrn(rw) - Open drain enable for VID pins.
    Bits:  0    vid_force_tristateb(rw) - Tri-state enable for VID pins.
*/
#define NFP_PL_PluIOControls_PLU_VID_CONTROL                 0x00040078
#define   NFP_PL_PLUIOCONTROLS_PLU_VID_CONTROL_VID_WK_POLARITY BIT(15)
#define     NFP_PL_PLUIOCONTROLS_PLU_VID_CONTROL_VID_WK_POLARITY_0 (0 << 15)
#define     NFP_PL_PLUIOCONTROLS_PLU_VID_CONTROL_VID_WK_POLARITY_1 BIT(15)
#define   NFP_PL_PLUIOCONTROLS_PLU_VID_CONTROL_VID_WK_PULL_EN BIT(14)
#define     NFP_PL_PLUIOCONTROLS_PLU_VID_CONTROL_VID_WK_PULL_EN_0 (0 << 14)
#define     NFP_PL_PLUIOCONTROLS_PLU_VID_CONTROL_VID_WK_PULL_EN_1 BIT(14)
#define   NFP_PL_PLUIOCONTROLS_PLU_VID_CONTROL_VID_PDN_DRV_RCOMP(_x) (((_x) & 0x3f) << 8)
#define   NFP_PL_PLUIOCONTROLS_PLU_VID_CONTROL_VID_PDN_DRV_RCOMP_of(_x) (((_x) >> 8) & 0x3f)
#define   NFP_PL_PLUIOCONTROLS_PLU_VID_CONTROL_VID_PUP_DRV_RCOMP(_x) (((_x) & 0x3f) << 2)
#define   NFP_PL_PLUIOCONTROLS_PLU_VID_CONTROL_VID_PUP_DRV_RCOMP_of(_x) (((_x) >> 2) & 0x3f)
#define   NFP_PL_PLUIOCONTROLS_PLU_VID_CONTROL_VID_OPENDRN   BIT(1)
#define   NFP_PL_PLUIOCONTROLS_PLU_VID_CONTROL_VID_FORCE_TRISTATEB BIT(0)

#else /* NFP6000_LONGNAMES */

/*----------------------------------------------------------------
  Register: PluMisc_PluMasterReset - Programmable Reset Controls
    Bits: 11    PCIE3_RESET_MASK(rw) - When set to 1, PCIE3_RESET_OUT_L will not be reset by SRESETN
    Bits: 10    PCIE2_RESET_MASK(rw) - When set to 1, PCIE2_RESET_OUT_L will not be reset by SRESETN
    Bits:  9    PCIE1_RESET_MASK(rw) - When set to 1, PCIE1_RESET_OUT_L will not be reset by SRESETN
    Bits:  8    PCIE0_RESET_MASK(rw) - When set to 1, PCIE0_RESET_OUT_L will not be reset by SRESETN
    Bits:  7    PCIE3_RESET_OUT_L(rw) - PCIE3 Reset Out
    Bits:  6    PCIE2_RESET_OUT_L(rw) - PCIE2 Reset Out
    Bits:  5    PCIE1_RESET_OUT_L(rw) - PCIE1 Reset Out
    Bits:  4    PCIE0_RESET_OUT_L(rw) - PCIE0 Reset Out
    Bits:  0    CLK_NRESET_OUT_L(rw) - PowerOn Reset Out
*/
#define NFP_PL_PluMisc_PluMasterReset                        0x00000000
#define   NFP_PL_PluMisc_PluMasterReset_PCIE3_RESET_MASK     BIT(11)
#define   NFP_PL_PluMisc_PluMasterReset_PCIE2_RESET_MASK     BIT(10)
#define   NFP_PL_PluMisc_PluMasterReset_PCIE1_RESET_MASK     BIT(9)
#define   NFP_PL_PluMisc_PluMasterReset_PCIE0_RESET_MASK     BIT(8)
#define   NFP_PL_PluMisc_PluMasterReset_PCIE3_RESET_OUT_L    BIT(7)
#define   NFP_PL_PluMisc_PluMasterReset_PCIE2_RESET_OUT_L    BIT(6)
#define   NFP_PL_PluMisc_PluMasterReset_PCIE1_RESET_OUT_L    BIT(5)
#define   NFP_PL_PluMisc_PluMasterReset_PCIE0_RESET_OUT_L    BIT(4)
#define   NFP_PL_PluMisc_PluMasterReset_CLK_NRESET_OUT_L     BIT(0)
/*----------------------------------------------------------------
  Register: PluMisc_PluDeviceID - Chip Device ID Register.
    Bits: 31:16 PartNumber(ro) - Major Part Number Designation (Hard Coded)
    Bits: 15:8  SKU(ro) - SKU Designation (From Fuses)
    Bits:  7:4  MajorRevID(ro) - Major Revision ID (Hard Coded)
    Bits:  3:0  MinorRevID(ro) - Minor Revision ID (From Fuses or Special Metal Structure)
*/
#define NFP_PL_PluMisc_PluDeviceID                           0x00000004
#define   NFP_PL_PluMisc_PluDeviceID_PartNumber(_x)          (((_x) & 0xffff) << 16)
#define   NFP_PL_PluMisc_PluDeviceID_PartNumber_of(_x)       (((_x) >> 16) & 0xffff)
#define   NFP_PL_PluMisc_PluDeviceID_SKU(_x)                 (((_x) & 0xff) << 8)
#define   NFP_PL_PluMisc_PluDeviceID_SKU_of(_x)              (((_x) >> 8) & 0xff)
#define   NFP_PL_PluMisc_PluDeviceID_MajorRevID(_x)          (((_x) & 0xf) << 4)
#define   NFP_PL_PluMisc_PluDeviceID_MajorRevID_of(_x)       (((_x) >> 4) & 0xf)
#define   NFP_PL_PluMisc_PluDeviceID_MinorRevID(_x)          (((_x) & 0xf) << 0)
#define   NFP_PL_PluMisc_PluDeviceID_MinorRevID_of(_x)       (((_x) >> 0) & 0xf)
/*----------------------------------------------------------------
  Register: PluMisc_PluPowerState - Indicators for Power and Power-Domain Status
    Bits:  5    CRYPTO_PDL(ro) - Crypto Power Domain Locked
    Bits:  4    BOTTOM_PDL(ro) - Bottom of Chip Power Domain Locked.
    Bits:  3    POWERGOOD4(ro) - Power Domain 4 Power Good Indicator.
    Bits:  2    POWERGOOD3(ro) - Power Domain 3 Power Good Indicator.
    Bits:  1    POWERGOOD2(ro) - Power Domain 2 Power Good Indicator.
    Bits:  0    POWERGOOD1(ro) - Power Domain 1 Power Good Indicator.
*/
#define NFP_PL_PluMisc_PluPowerState                         0x0000000c
#define   NFP_PL_PluMisc_PluPowerState_CRYPTO_PDL            BIT(5)
#define   NFP_PL_PluMisc_PluPowerState_BOTTOM_PDL            BIT(4)
#define   NFP_PL_PluMisc_PluPowerState_POWERGOOD4            BIT(3)
#define   NFP_PL_PluMisc_PluPowerState_POWERGOOD3            BIT(2)
#define   NFP_PL_PluMisc_PluPowerState_POWERGOOD2            BIT(1)
#define   NFP_PL_PluMisc_PluPowerState_POWERGOOD1            BIT(0)
/*----------------------------------------------------------------
  Register: PluMisc_PluVID - VID Control Register
    Bits: 23:16 VIDProcessMetric(ro) - 8-bit process metric used by software to determine proper VIDValue
    Bits:  8    VIDEnable(rw) - Under ideal conditions, this will Enable VID outputs. However, in B0 Stepping of NFP-6xxx, writing to this bit has no effect. VID outputs will remain enabled at all times after powering up the chip.
    Bits:  7:0  VIDValue(rw) - VID Control Bus Value: After POWERGOOD1 is asserted, VIDValue samples value seen on VID pins for 16 clock cycles and then turns on VIDEnable = 1. VID pins will only be sampled during this initial period.
*/
#define NFP_PL_PluMisc_PluVID                                0x00000014
#define   NFP_PL_PluMisc_PluVID_VIDProcessMetric(_x)         (((_x) & 0xff) << 16)
#define   NFP_PL_PluMisc_PluVID_VIDProcessMetric_of(_x)      (((_x) >> 16) & 0xff)
#define   NFP_PL_PluMisc_PluVID_VIDEnable                    BIT(8)
#define   NFP_PL_PluMisc_PluVID_VIDValue(_x)                 (((_x) & 0xff) << 0)
#define   NFP_PL_PluMisc_PluVID_VIDValue_of(_x)              (((_x) >> 0) & 0xff)
/*----------------------------------------------------------------
  Register: PluMisc_PluBaudRateGen - Control Register for Baud Rate Generator.
    Bits: 31:16 BaudControlA(rw) - Control A for Baud Rate Generator. See Baud Rate Generator Table
    Bits: 15:0  BaudControlB(rw) - Control B for Baud Rate Generator. See Baud Rate Generator Table
*/
#define NFP_PL_PluMisc_PluBaudRateGen                        0x00000018
#define   NFP_PL_PluMisc_PluBaudRateGen_BaudControlA(_x)     (((_x) & 0xffff) << 16)
#define   NFP_PL_PluMisc_PluBaudRateGen_BaudControlA_of(_x)  (((_x) >> 16) & 0xffff)
#define   NFP_PL_PluMisc_PluBaudRateGen_BaudControlB(_x)     (((_x) & 0xffff) << 0)
#define   NFP_PL_PluMisc_PluBaudRateGen_BaudControlB_of(_x)  (((_x) >> 0) & 0xffff)
/*----------------------------------------------------------------
  Register: PluMisc_PluArmClockSelect - ARM Clock Generation Control
    Bits:  0    AclkSelect(rw) - Select which PLL's secondary output (PLLCLK1) will be used for generating the ARM Clock.
*/
#define NFP_PL_PluMisc_PluArmClockSelect                     0x00000034
#define   NFP_PL_PluMisc_PluArmClockSelect_AclkSelect        BIT(0)
#define     NFP_PL_PluMisc_PluArmClockSelect_AclkSelect_PLL0 (0 << 0)
#define     NFP_PL_PluMisc_PluArmClockSelect_AclkSelect_PLL1 BIT(0)
/*----------------------------------------------------------------
  Register: PluMisc_PluJtagTckControl - JTAG_TCK Internal Generation Control
    Bits:  8    ClkDivBypass(rw) - Clock Divider will be bypassed when in SCAN_MODE or in HARD_RESET or when this bit is set to 1.
    Bits:  7:0  ClkDivRatio(rw) - REF_CLK divider ratio to internally generate JTAG_TCK. Legal values: 3-255
*/
#define NFP_PL_PluMisc_PluJtagTckControl                     0x00000038
#define   NFP_PL_PluMisc_PluJtagTckControl_ClkDivBypass      BIT(8)
#define   NFP_PL_PluMisc_PluJtagTckControl_ClkDivRatio(_x)   (((_x) & 0xff) << 0)
#define   NFP_PL_PluMisc_PluJtagTckControl_ClkDivRatio_of(_x) (((_x) >> 0) & 0xff)
/*----------------------------------------------------------------
  Register: PluTSXpb_PluTSControl0 - Thermal Sensor Basic Controls
    Bits: 15    TSitsrst(rw) - Reset all Thermal Sensor Registers. Recommended at each power up after powergood is asserted.
    Bits: 14    TSitsen(rw) - 0 = Disable all circuits. Leave this bit always on, also in power on, beside debugging.
    Bits: 13:10 TSitstripthreshon(rw) - When temperature reaches the value indicated by this bus, trip signal, also called catastrophic (otstemperaturetrip) is turned on. 115 C + 15 steps (1 C) for trip point configuration. (Default is 130 C
    Bits:  9:2  TSitsalertthreshon(rw) - When temperature reaches the value indicated by this bus, alert signal otstemperaturealrt is turned on Where 0 is -50 C, resolution of 1 C. Default is 125 C
    Bits:  1:0  TSitsalerthysteresis(rw) - Provides hysteresis below the alert threshold trip point while temp is going down; the otstemperaturealrt gets deasserted below [alert threshold point (itsalertthreshon) + hysteresis values].
*/
#define NFP_PL_PluTSXpb_PluTSControl0                        0x00010000
#define   NFP_PL_PluTSXpb_PluTSControl0_TSitsrst             BIT(15)
#define     NFP_PL_PluTSXpb_PluTSControl0_TSitsrst_Normal    (0 << 15)
#define     NFP_PL_PluTSXpb_PluTSControl0_TSitsrst_Reset     BIT(15)
#define   NFP_PL_PluTSXpb_PluTSControl0_TSitsen              BIT(14)
#define     NFP_PL_PluTSXpb_PluTSControl0_TSitsen_Disable    (0 << 14)
#define     NFP_PL_PluTSXpb_PluTSControl0_TSitsen_Enable     BIT(14)
#define   NFP_PL_PluTSXpb_PluTSControl0_TSitstripthreshon(_x) (((_x) & 0xf) << 10)
#define   NFP_PL_PluTSXpb_PluTSControl0_TSitstripthreshon_of(_x) (((_x) >> 10) & 0xf)
#define   NFP_PL_PluTSXpb_PluTSControl0_TSitsalertthreshon(_x) (((_x) & 0xff) << 2)
#define   NFP_PL_PluTSXpb_PluTSControl0_TSitsalertthreshon_of(_x) (((_x) >> 2) & 0xff)
#define   NFP_PL_PluTSXpb_PluTSControl0_TSitsalerthysteresis(_x) (((_x) & 0x3) << 0)
#define   NFP_PL_PluTSXpb_PluTSControl0_TSitsalerthysteresis_of(_x) (((_x) >> 0) & 0x3)
#define     NFP_PL_PluTSXpb_PluTSControl0_TSitsalerthysteresis_0_C (0)
#define     NFP_PL_PluTSXpb_PluTSControl0_TSitsalerthysteresis_1_C (1)
#define     NFP_PL_PluTSXpb_PluTSControl0_TSitsalerthysteresis_2_C (2)
#define     NFP_PL_PluTSXpb_PluTSControl0_TSitsalerthysteresis_5_C (3)
/*----------------------------------------------------------------
  Register: PluTSXpb_PluTSControl1 - Thermal Sensor Minor Controls
    Bits: 27    TSianachopctr(rw) - Select where Chopping is Done - 0 for chopping in Sense, 1 for chopping in SD ADC
    Bits: 26    TSitssupplymode(rw) - Select signal to keep the sense stage reference signal voltage the same.
    Bits: 25    TSitsfilterrate(rw) - Selects CIC filter decimation factor.
    Bits: 24    TSitsfilterrangesel(rw) - Range select for filter output bits according to the gain of the TS analog circuit
    Bits: 22:18 TSidfttpsel(rw) - DFT test port select
    Bits: 17    TSidftatfen(rw) - Enables the ATF Circuit, allowing testin of Analog signals digitally with odftatf port to SoC
    Bits: 16    TSitsdigpwrctrlovrd(rw) - Disables the power control sequence
    Bits: 12:11 TSisnscurrentsel(rw) - Selects the current value for the four current sources within sense stage. The following are the base branch bias current values:
    Bits: 10    TSisnscurratio3p5(rw) - Sense State Current Ratio.
    Bits:  9    TSisnswastesel(rw) - Selects the wast current target (when isnscurratio3p5 = 0):
    Bits:  8    TSisnsdemen(rw) - Enable Dynamic Element Matching (DEM)
    Bits:  7    TSisnsdemstpctr(rw) - DFX mode pin (In Normal DEM operation, connect isnsdenstpctr = 1)
    Bits:  6    TSisnsintvrefen(rw) - Enables an internal reference voltage gnerated by a resistor divider. External reference voltage is used for DFT when isnsintvrefen=0, that is, with an external reference voltage of 0.920 V.
    Bits:  5    TSitsfiltertst(rw) - Input signal to test digital filter
    Bits:  4:3  TSisdfssel(rw) - Sigma Delta select bits
    Bits:  2    TSisdx2delayen(rw) - Double the internal delays in the phase generator in SD ADC
    Bits:  1    TSisdmodesel(rw) - SD ADC mode selection (Tie to 1, no voltage reference is available)
    Bits:  0    TSisdstandaloneen(rw) - SDC ADC stand-alone test (Tie to 0 for normal operation)
*/
#define NFP_PL_PluTSXpb_PluTSControl1                        0x00010004
#define   NFP_PL_PluTSXpb_PluTSControl1_TSianachopctr        BIT(27)
#define     NFP_PL_PluTSXpb_PluTSControl1_TSianachopctr_Sense (0 << 27)
#define     NFP_PL_PluTSXpb_PluTSControl1_TSianachopctr_ADC  BIT(27)
#define   NFP_PL_PluTSXpb_PluTSControl1_TSitssupplymode      BIT(26)
#define     NFP_PL_PluTSXpb_PluTSControl1_TSitssupplymode_0  (0 << 26)
#define     NFP_PL_PluTSXpb_PluTSControl1_TSitssupplymode_1  BIT(26)
#define   NFP_PL_PluTSXpb_PluTSControl1_TSitsfilterrate      BIT(25)
#define     NFP_PL_PluTSXpb_PluTSControl1_TSitsfilterrate_0  (0 << 25)
#define     NFP_PL_PluTSXpb_PluTSControl1_TSitsfilterrate_1  BIT(25)
#define   NFP_PL_PluTSXpb_PluTSControl1_TSitsfilterrangesel  BIT(24)
#define   NFP_PL_PluTSXpb_PluTSControl1_TSidfttpsel(_x)      (((_x) & 0x1f) << 18)
#define   NFP_PL_PluTSXpb_PluTSControl1_TSidfttpsel_of(_x)   (((_x) >> 18) & 0x1f)
#define   NFP_PL_PluTSXpb_PluTSControl1_TSidftatfen          BIT(17)
#define   NFP_PL_PluTSXpb_PluTSControl1_TSitsdigpwrctrlovrd  BIT(16)
#define   NFP_PL_PluTSXpb_PluTSControl1_TSisnscurrentsel(_x) (((_x) & 0x3) << 11)
#define   NFP_PL_PluTSXpb_PluTSControl1_TSisnscurrentsel_of(_x) (((_x) >> 11) & 0x3)
#define   NFP_PL_PluTSXpb_PluTSControl1_TSisnscurratio3p5    BIT(10)
#define     NFP_PL_PluTSXpb_PluTSControl1_TSisnscurratio3p5_EP (0 << 10)
#define     NFP_PL_PluTSXpb_PluTSControl1_TSisnscurratio3p5_RC BIT(10)
#define   NFP_PL_PluTSXpb_PluTSControl1_TSisnswastesel       BIT(9)
#define     NFP_PL_PluTSXpb_PluTSControl1_TSisnswastesel_EP  (0 << 9)
#define     NFP_PL_PluTSXpb_PluTSControl1_TSisnswastesel_RC  BIT(9)
#define   NFP_PL_PluTSXpb_PluTSControl1_TSisnsdemen          BIT(8)
#define     NFP_PL_PluTSXpb_PluTSControl1_TSisnsdemen_EP     (0 << 8)
#define     NFP_PL_PluTSXpb_PluTSControl1_TSisnsdemen_RC     BIT(8)
#define   NFP_PL_PluTSXpb_PluTSControl1_TSisnsdemstpctr      BIT(7)
#define   NFP_PL_PluTSXpb_PluTSControl1_TSisnsintvrefen      BIT(6)
#define   NFP_PL_PluTSXpb_PluTSControl1_TSitsfiltertst       BIT(5)
#define   NFP_PL_PluTSXpb_PluTSControl1_TSisdfssel(_x)       (((_x) & 0x3) << 3)
#define   NFP_PL_PluTSXpb_PluTSControl1_TSisdfssel_of(_x)    (((_x) >> 3) & 0x3)
#define   NFP_PL_PluTSXpb_PluTSControl1_TSisdx2delayen       BIT(2)
#define     NFP_PL_PluTSXpb_PluTSControl1_TSisdx2delayen_EP  (0 << 2)
#define     NFP_PL_PluTSXpb_PluTSControl1_TSisdx2delayen_RC  BIT(2)
#define   NFP_PL_PluTSXpb_PluTSControl1_TSisdmodesel         BIT(1)
#define   NFP_PL_PluTSXpb_PluTSControl1_TSisdstandaloneen    BIT(0)
/*----------------------------------------------------------------
  Register: PluTSXpb_PluTSControl2 - Thermal Sensor Calibration Controls
    Bits: 22:18 TSicalgamma(rw) - Used to calibrate the TS for temperature curvature correction for ratiometric mode.
    Bits: 17:16 TSicalconfigsel(rw) - TS Calibration Controls
    Bits: 15:8  TSicaldatoffset(rw) - Calibration fuses used for temperature offset cancellation with resolution of 0.5 C. NOTE: During the calibration process, add an option to get these inputs from a register with the defaults declared.
    Bits:  7:0  TSicaldatslope(rw) - Calibration fuses used for temperature slope calibraryion. NOTE: During the calibration process, add an option to get these inputs from a register with the defaults declared.
*/
#define NFP_PL_PluTSXpb_PluTSControl2                        0x00010008
#define   NFP_PL_PluTSXpb_PluTSControl2_TSicalgamma(_x)      (((_x) & 0x1f) << 18)
#define   NFP_PL_PluTSXpb_PluTSControl2_TSicalgamma_of(_x)   (((_x) >> 18) & 0x1f)
#define   NFP_PL_PluTSXpb_PluTSControl2_TSicalconfigsel(_x)  (((_x) & 0x3) << 16)
#define   NFP_PL_PluTSXpb_PluTSControl2_TSicalconfigsel_of(_x) (((_x) >> 16) & 0x3)
#define     NFP_PL_PluTSXpb_PluTSControl2_TSicalconfigsel_Linear (0)
#define     NFP_PL_PluTSXpb_PluTSControl2_TSicalconfigsel_Both (1)
#define     NFP_PL_PluTSXpb_PluTSControl2_TSicalconfigsel_Neither (2)
#define     NFP_PL_PluTSXpb_PluTSControl2_TSicalconfigsel_Calibrate (3)
#define   NFP_PL_PluTSXpb_PluTSControl2_TSicaldatoffset(_x)  (((_x) & 0xff) << 8)
#define   NFP_PL_PluTSXpb_PluTSControl2_TSicaldatoffset_of(_x) (((_x) >> 8) & 0xff)
#define   NFP_PL_PluTSXpb_PluTSControl2_TSicaldatslope(_x)   (((_x) & 0xff) << 0)
#define   NFP_PL_PluTSXpb_PluTSControl2_TSicaldatslope_of(_x) (((_x) >> 0) & 0xff)
/*----------------------------------------------------------------
  Register: PluTSXpb_PluTSReadings - Thermal Sensor Readings
    Bits: 11    TSotstemperaturetrip(ro) - Temperature catastrophic alert signal according to itstripthreshon[3:0] pin settings; this signal wil get asserted when temp on die goes above the threshold set by itstripthreshon[3:0]; gets deasserted only when the Thermal Sensor IP is reset.
    Bits: 10    TSotstemperaturealrt(ro) - Temperature trip point alert signal based on the thresholds set by itsalwerthysteresis[1:0] and itsalertthreshon[7:0] pins; the alert signal will be asserted when temp on the die crosses the threshold set by itsalertthreshon[7:0] bits, when temp goes down on the die, the alert signal also gets deasserted.
    Bits:  9    TSotstemperaturevalid(ro) - Valid pulse for temperature sampling (when it is 1, temp reading is valid)
    Bits:  8:0  TSotstemperature(ro) - 9 bit temperature reading Where 0 = -50 C, resolution of 0.5 C
*/
#define NFP_PL_PluTSXpb_PluTSReadings                        0x0001000c
#define   NFP_PL_PluTSXpb_PluTSReadings_TSotstemperaturetrip BIT(11)
#define   NFP_PL_PluTSXpb_PluTSReadings_TSotstemperaturealrt BIT(10)
#define   NFP_PL_PluTSXpb_PluTSReadings_TSotstemperaturevalid BIT(9)
#define   NFP_PL_PluTSXpb_PluTSReadings_TSotstemperature(_x) (((_x) & 0x1ff) << 0)
#define   NFP_PL_PluTSXpb_PluTSReadings_TSotstemperature_of(_x) (((_x) >> 0) & 0x1ff)
/*----------------------------------------------------------------
  Register: PLL0Controls_PLLControlAndStatus - Primary Control and Status Register
  Register: PLL1Controls_PLLControlAndStatus - Primary Control and Status Register
    Bits: 31    PLLPdSynth(rw) - FSM Control: 1=Normal Power-Up 0=PowerDown. When transitioned to 1, the PLL State Machine will execute an automatic PLL bringup sequence and lockout rights to all but the Manual Override configuration register.
    Bits: 30    PLLCLK1OutputEn(rw) - FSM Control: Enable for PLLCLK1 Output
    Bits: 29    PLLCLK0OutputEn(rw) - FSM Control: Enable for PLLCLK0 Output
    Bits: 24    PLLAutoReLock(rw) - Mode bit to select behavior upon the occurence of a Lock Fault
    Bits: 23:8  PLLLockFaultCount(ro) - Once the PLL has been spun-up, counts how many times that lock is lost. This field will be cleared whenever PLLPdSynth transitions from 0 to 1.
    Bits:  7    PLLLockFault(ro) - Indicates that the PLL has lost lock during normal operation. This field will be cleared whenever PLLPdSynth transitions from 0 to 1.
    Bits:  6    PLLSynthLock(ro) - PLL Lock Indicator
    Bits:  5    PLLSynthOut1En(ro) - FSM Output: Enable for synthesizer clock output 1
    Bits:  4    PLLSynthOutByPass1(ro) - FSM Output: Bypass synthesizer clock output 1 with ICK_SYNTHREF with the PLL
    Bits:  3    PLLWrapOutByPass1(ro) - FSM Output: Bypass synthesizer clock output 1 with ICK_SYNTHREF outside the PLL
    Bits:  2    PLLSynthOut0En(ro) - FSM Output: Enable for synthesizer clock output 0
    Bits:  1    PLLSynthOutByPass0(ro) - FSM Output: Bypass synthesizer clock output 0 with ICK_SYNTHREF within the PLL
    Bits:  0    PLLWrapOutByPass0(ro) - FSM Output: Bypass synthesizer clock output 0 with ICK_SYNTHREF outside the PLL
*/
#define NFP_PL_PLL0Controls_PLLControlAndStatus              0x00020000
#define NFP_PL_PLL1Controls_PLLControlAndStatus              0x00030000
#define   NFP_PL_PLL0Controls_PLLControlAndStatus_PLLPdSynth BIT(31)
#define   NFP_PL_PLL0Controls_PLLControlAndStatus_PLLCLK1OutputEn BIT(30)
#define   NFP_PL_PLL0Controls_PLLControlAndStatus_PLLCLK0OutputEn BIT(29)
#define   NFP_PL_PLL0Controls_PLLControlAndStatus_PLLAutoReLock BIT(24)
#define     NFP_PL_PLL0Controls_PLLControlAndStatus_PLLAutoReLock_IDLE (0 << 24)
#define     NFP_PL_PLL0Controls_PLLControlAndStatus_PLLAutoReLock_RELOCK BIT(24)
#define   NFP_PL_PLL0Controls_PLLControlAndStatus_PLLLockFaultCount(_x) (((_x) & 0xffff) << 8)
#define   NFP_PL_PLL0Controls_PLLControlAndStatus_PLLLockFaultCount_of(_x) (((_x) >> 8) & 0xffff)
#define   NFP_PL_PLL0Controls_PLLControlAndStatus_PLLLockFault BIT(7)
#define   NFP_PL_PLL0Controls_PLLControlAndStatus_PLLSynthLock BIT(6)
#define   NFP_PL_PLL0Controls_PLLControlAndStatus_PLLSynthOut1En BIT(5)
#define   NFP_PL_PLL0Controls_PLLControlAndStatus_PLLSynthOutByPass1 BIT(4)
#define   NFP_PL_PLL0Controls_PLLControlAndStatus_PLLWrapOutByPass1 BIT(3)
#define   NFP_PL_PLL0Controls_PLLControlAndStatus_PLLSynthOut0En BIT(2)
#define   NFP_PL_PLL0Controls_PLLControlAndStatus_PLLSynthOutByPass0 BIT(1)
#define   NFP_PL_PLL0Controls_PLLControlAndStatus_PLLWrapOutByPass0 BIT(0)
/*----------------------------------------------------------------
  Register: PLL0Controls_PLLConfig - Configuration Settings for the PLL
  Register: PLL1Controls_PLLConfig - Configuration Settings for the PLL
    Bits: 31    PLLLockMult(rw) - Write 1 to Set, cannot be cleared without hard_reset. Setting the lock bit captures the current values of the multipliers (integer and fraction) as limits for all future settings
    Bits: 30    PLLSynthMode(rw) - Synthesizer mode: 1=Fractional mode, 0=integer mode
    Bits: 29:24 PLLSynthRefDiv(rw) - Reference clock input divider ratio
    Bits: 23:16 PLLSynthMDiv(rw) - Synthesizer feedback divider value
    Bits: 15:0  PLLSynthSSCModGain(rw) - Signed 16-bit magnitude SSC modulation depth control word, as Fraction div ratio.
*/
#define NFP_PL_PLL0Controls_PLLConfig                        0x00020004
#define NFP_PL_PLL1Controls_PLLConfig                        0x00030004
#define   NFP_PL_PLL0Controls_PLLConfig_PLLLockMult          BIT(31)
#define   NFP_PL_PLL0Controls_PLLConfig_PLLSynthMode         BIT(30)
#define   NFP_PL_PLL0Controls_PLLConfig_PLLSynthRefDiv(_x)   (((_x) & 0x3f) << 24)
#define   NFP_PL_PLL0Controls_PLLConfig_PLLSynthRefDiv_of(_x) (((_x) >> 24) & 0x3f)
#define   NFP_PL_PLL0Controls_PLLConfig_PLLSynthMDiv(_x)     (((_x) & 0xff) << 16)
#define   NFP_PL_PLL0Controls_PLLConfig_PLLSynthMDiv_of(_x)  (((_x) >> 16) & 0xff)
#define   NFP_PL_PLL0Controls_PLLConfig_PLLSynthSSCModGain(_x) (((_x) & 0xffff) << 0)
#define   NFP_PL_PLL0Controls_PLLConfig_PLLSynthSSCModGain_of(_x) (((_x) >> 0) & 0xffff)
/*----------------------------------------------------------------
  Register: PLL0Controls_PLLOutputDividers - Configuration Settings for the Output Dividers
  Register: PLL1Controls_PLLOutputDividers - Configuration Settings for the Output Dividers
    Bits: 14    PLLSynthOutPhRst(rw) - Reset phase rotator to initial startup value.
    Bits: 13:8  PLLSynthOut1Div(rw) - Synthesizer clock output 1 divider value
    Bits:  7    PLLSynthOutPhInc1(rw) - Increments phase of synthesizer clock outputs 1 by 1/8 of 1 period on rising edge
    Bits:  6:1  PLLSynthOut0Div(rw) - Synthesizer clock output 0 divider value
    Bits:  0    PLLSynthOutPhInc0(rw) - Increments phase of synthesizer clock outputs 0 by 1/8 of 1 period on rising edge
*/
#define NFP_PL_PLL0Controls_PLLOutputDividers                0x00020008
#define NFP_PL_PLL1Controls_PLLOutputDividers                0x00030008
#define   NFP_PL_PLL0Controls_PLLOutputDividers_PLLSynthOutPhRst BIT(14)
#define   NFP_PL_PLL0Controls_PLLOutputDividers_PLLSynthOut1Div(_x) (((_x) & 0x3f) << 8)
#define   NFP_PL_PLL0Controls_PLLOutputDividers_PLLSynthOut1Div_of(_x) (((_x) >> 8) & 0x3f)
#define   NFP_PL_PLL0Controls_PLLOutputDividers_PLLSynthOutPhInc1 BIT(7)
#define   NFP_PL_PLL0Controls_PLLOutputDividers_PLLSynthOut0Div(_x) (((_x) & 0x3f) << 1)
#define   NFP_PL_PLL0Controls_PLLOutputDividers_PLLSynthOut0Div_of(_x) (((_x) >> 1) & 0x3f)
#define   NFP_PL_PLL0Controls_PLLOutputDividers_PLLSynthOutPhInc0 BIT(0)
/*----------------------------------------------------------------
  Register: PLL0Controls_PLLMiscControls - Misc Tuning and Configuration Settings
  Register: PLL1Controls_PLLMiscControls - Misc Tuning and Configuration Settings
    Bits: 24    PLLSynthFastLockEn(rw) - Fast lock enable.
    Bits: 23    PLLSynthOutPhIncSat(rw) - Control saturation behavior for phase increments.
    Bits: 22:16 PLLSynthPropGain(rw) - Synthesizer loop bandwidth control.
    Bits: 15:0  PLLSynthSSCGenCount(rw) - SSC Frequency Control
*/
#define NFP_PL_PLL0Controls_PLLMiscControls                  0x0002000c
#define NFP_PL_PLL1Controls_PLLMiscControls                  0x0003000c
#define   NFP_PL_PLL0Controls_PLLMiscControls_PLLSynthFastLockEn BIT(24)
#define   NFP_PL_PLL0Controls_PLLMiscControls_PLLSynthOutPhIncSat BIT(23)
#define   NFP_PL_PLL0Controls_PLLMiscControls_PLLSynthPropGain(_x) (((_x) & 0x7f) << 16)
#define   NFP_PL_PLL0Controls_PLLMiscControls_PLLSynthPropGain_of(_x) (((_x) >> 16) & 0x7f)
#define   NFP_PL_PLL0Controls_PLLMiscControls_PLLSynthSSCGenCount(_x) (((_x) & 0xffff) << 0)
#define   NFP_PL_PLL0Controls_PLLMiscControls_PLLSynthSSCGenCount_of(_x) (((_x) >> 0) & 0xffff)
/*----------------------------------------------------------------
  Register: PLL0Controls_PLLManualOverride - Manual Override Register
  Register: PLL1Controls_PLLManualOverride - Manual Override Register
    Bits: 31    PLLManualOverride(rw) - Setting this bit to 1 enables manual override control for the PLLs. This takes control away from the PLL State Machine. This will also enable writes to the other config regs that would normally be blocked by the PLL State Machine
    Bits: 30    OvdPdSynth(rw) - Manual Override: PLL Power Down Mode: 1=Normal 0=PowerDown
    Bits: 29    OvdResetSynth(rw) - Manual Override: Reset mode: 1=Normal, 0=Reset
    Bits:  5    OvdSynthOut1En(rw) - Manual Override: Enable for synthesizer clock output 1
    Bits:  4    OvdSynthOutByPass1(rw) - Manual Override: Bypass synthesizer clock output 1 with ICK_SYNTHREF with the PLL
    Bits:  3    OvdWrapOutByPass1(rw) - Manual Override: Bypass synthesizer clock output 1 with ICK_SYNTHREF outside the PLL
    Bits:  2    OvdSynthOut0En(rw) - Manual Override: Enable for synthesizer clock output 0
    Bits:  1    OvdSynthOutByPass0(rw) - Manual Override: Bypass synthesizer clock output 0 with ICK_SYNTHREF within the PLL
    Bits:  0    OvdWrapOutByPass0(rw) - Manual Override: Bypass synthesizer clock output 0 with ICK_SYNTHREF outside the PLL
*/
#define NFP_PL_PLL0Controls_PLLManualOverride                0x00020014
#define NFP_PL_PLL1Controls_PLLManualOverride                0x00030014
#define   NFP_PL_PLL0Controls_PLLManualOverride_PLLManualOverride BIT(31)
#define   NFP_PL_PLL0Controls_PLLManualOverride_OvdPdSynth   BIT(30)
#define   NFP_PL_PLL0Controls_PLLManualOverride_OvdResetSynth BIT(29)
#define   NFP_PL_PLL0Controls_PLLManualOverride_OvdSynthOut1En BIT(5)
#define   NFP_PL_PLL0Controls_PLLManualOverride_OvdSynthOutByPass1 BIT(4)
#define   NFP_PL_PLL0Controls_PLLManualOverride_OvdWrapOutByPass1 BIT(3)
#define   NFP_PL_PLL0Controls_PLLManualOverride_OvdSynthOut0En BIT(2)
#define   NFP_PL_PLL0Controls_PLLManualOverride_OvdSynthOutByPass0 BIT(1)
#define   NFP_PL_PLL0Controls_PLLManualOverride_OvdWrapOutByPass0 BIT(0)
/*----------------------------------------------------------------
  Register: PluIOControls_PluLvcmos0IOCtrl - LVCMOS0 IO Control
  Register: PluIOControls_PluLvcmos1IOCtrl - LVCMOS1 IO Control
    Bits: 27    ILK1_TX_wk_pull_en(rw) - Weak pull enable for ILK1_OOB_TCLK, TSYNC, TDATA pins.
    Bits: 26    ILK1_TX_wk_polarity(rw) - Weak pull polarity for ILK1_OOB_TCLK, TSYNC, TDATA pins.
    Bits: 25    ILK1_TX_rcven(rw) - Receive enable for ILK1_OOB_TCLK, TSYNC, TDATA pins. High Active.
    Bits: 24    ILK1_TX_oenb(rw) - Output enable for ILK1_OOB_TCLK, TSYNC, TDATA pins. Low active.
    Bits: 20    ILK1_RX_wk_pull_en(rw) - Weak pull enable for ILK1_OOB_RCLK, RSYNC, RDATA pins.
    Bits: 19    ILK1_RX_wk_polarity(rw) - Weak pull polarity for ILK1_OOB_RCLK, RSYNC, RDATA pins.
    Bits: 18    ILK1_RX_rcvhys(rw) - Receiver hysterisis enable for ILK1_OOB_RCLK, RSYNC, RDATA pins. High active.
    Bits: 17    ILK1_RX_rcven(rw) - Receive enable for ILK1_OOB_RCLK, RSYNC, RDATA pins. High Active.
    Bits: 16    ILK1_RX_oenb(rw) - Output enable for ILK1_OOB_RCLK, RSYNC, RDATA pins. Low active.
    Bits: 11    ILK0_TX_wk_pull_en(rw) - Weak pull enable for ILK0_OOB_TCLK, TSYNC, TDATA pins.
    Bits: 10    ILK0_TX_wk_polarity(rw) - Weak pull polarity for ILK0_OOB_TCLK, TSYNC, TDATA pins.
    Bits:  9    ILK0_TX_rcven(rw) - Receive enable for ILK0_OOB_TCLK, TSYNC, TDATA pins. High Active.
    Bits:  8    ILK0_TX_oenb(rw) - Output enable for ILK0_OOB_TCLK, TSYNC, TDATA pins. Low active.
    Bits:  4    ILK0_RX_wk_pull_en(rw) - Weak pull enable for ILK0_OOB_RCLK, RSYNC, RDATA pins.
    Bits:  3    ILK0_RX_wk_polarity(rw) - Weak pull polarity for ILK0_OOB_RCLK, RSYNC, RDATA pins.
    Bits:  2    ILK0_RX_rcvhys(rw) - Receiver hysterisis enable for ILK0_OOB_RCLK, RSYNC, RDATA pins. High active.
    Bits:  1    ILK0_RX_rcven(rw) - Receive enable for ILK0_OOB_RCLK, RSYNC, RDATA pins. High Active.
    Bits:  0    ILK0_RX_oenb(rw) - Output enable for ILK0_OOB_RCLK, RSYNC, RDATA pins. Low active.
*/
#define NFP_PL_PluIOControls_PluLvcmos0IOCtrl                0x00040000
#define NFP_PL_PluIOControls_PluLvcmos1IOCtrl                0x00040020
#define   NFP_PL_PluIOControls_PluLvcmosIOCtrl_ILK1_TX_wk_pull_en BIT(27)
#define     NFP_PL_PluIOControls_PluLvcmosIOCtrl_ILK1_TX_wk_pull_en_0 (0 << 27)
#define     NFP_PL_PluIOControls_PluLvcmosIOCtrl_ILK1_TX_wk_pull_en_1 BIT(27)
#define   NFP_PL_PluIOControls_PluLvcmosIOCtrl_ILK1_TX_wk_polarity BIT(26)
#define     NFP_PL_PluIOControls_PluLvcmosIOCtrl_ILK1_TX_wk_polarity_0 (0 << 26)
#define     NFP_PL_PluIOControls_PluLvcmosIOCtrl_ILK1_TX_wk_polarity_1 BIT(26)
#define   NFP_PL_PluIOControls_PluLvcmosIOCtrl_ILK1_TX_rcven BIT(25)
#define   NFP_PL_PluIOControls_PluLvcmosIOCtrl_ILK1_TX_oenb  BIT(24)
#define   NFP_PL_PluIOControls_PluLvcmosIOCtrl_ILK1_RX_wk_pull_en BIT(20)
#define     NFP_PL_PluIOControls_PluLvcmosIOCtrl_ILK1_RX_wk_pull_en_0 (0 << 20)
#define     NFP_PL_PluIOControls_PluLvcmosIOCtrl_ILK1_RX_wk_pull_en_1 BIT(20)
#define   NFP_PL_PluIOControls_PluLvcmosIOCtrl_ILK1_RX_wk_polarity BIT(19)
#define     NFP_PL_PluIOControls_PluLvcmosIOCtrl_ILK1_RX_wk_polarity_0 (0 << 19)
#define     NFP_PL_PluIOControls_PluLvcmosIOCtrl_ILK1_RX_wk_polarity_1 BIT(19)
#define   NFP_PL_PluIOControls_PluLvcmosIOCtrl_ILK1_RX_rcvhys BIT(18)
#define   NFP_PL_PluIOControls_PluLvcmosIOCtrl_ILK1_RX_rcven BIT(17)
#define   NFP_PL_PluIOControls_PluLvcmosIOCtrl_ILK1_RX_oenb  BIT(16)
#define   NFP_PL_PluIOControls_PluLvcmosIOCtrl_ILK0_TX_wk_pull_en BIT(11)
#define     NFP_PL_PluIOControls_PluLvcmosIOCtrl_ILK0_TX_wk_pull_en_0 (0 << 11)
#define     NFP_PL_PluIOControls_PluLvcmosIOCtrl_ILK0_TX_wk_pull_en_1 BIT(11)
#define   NFP_PL_PluIOControls_PluLvcmosIOCtrl_ILK0_TX_wk_polarity BIT(10)
#define     NFP_PL_PluIOControls_PluLvcmosIOCtrl_ILK0_TX_wk_polarity_0 (0 << 10)
#define     NFP_PL_PluIOControls_PluLvcmosIOCtrl_ILK0_TX_wk_polarity_1 BIT(10)
#define   NFP_PL_PluIOControls_PluLvcmosIOCtrl_ILK0_TX_rcven BIT(9)
#define   NFP_PL_PluIOControls_PluLvcmosIOCtrl_ILK0_TX_oenb  BIT(8)
#define   NFP_PL_PluIOControls_PluLvcmosIOCtrl_ILK0_RX_wk_pull_en BIT(4)
#define     NFP_PL_PluIOControls_PluLvcmosIOCtrl_ILK0_RX_wk_pull_en_0 (0 << 4)
#define     NFP_PL_PluIOControls_PluLvcmosIOCtrl_ILK0_RX_wk_pull_en_1 BIT(4)
#define   NFP_PL_PluIOControls_PluLvcmosIOCtrl_ILK0_RX_wk_polarity BIT(3)
#define     NFP_PL_PluIOControls_PluLvcmosIOCtrl_ILK0_RX_wk_polarity_0 (0 << 3)
#define     NFP_PL_PluIOControls_PluLvcmosIOCtrl_ILK0_RX_wk_polarity_1 BIT(3)
#define   NFP_PL_PluIOControls_PluLvcmosIOCtrl_ILK0_RX_rcvhys BIT(2)
#define   NFP_PL_PluIOControls_PluLvcmosIOCtrl_ILK0_RX_rcven BIT(1)
#define   NFP_PL_PluIOControls_PluLvcmosIOCtrl_ILK0_RX_oenb  BIT(0)
/*----------------------------------------------------------------
  Register: PluIOControls_PluLvcmos0IOCtrl2 - LVCMOS0 IO Control 2
  Register: PluIOControls_PluLvcmos1IOCtrl2 - LVCMOS1 IO Control 2
    Bits: 31:27 tco_compr(rw) - Driver TCO delay control rise.
    Bits: 26:22 pup_scomp_code(rw) - Driver pull up slew rate control setting.
    Bits: 21:16 pup_odt_rcomp(rw) - Pull up ODT setting for all LVCMOS pins.
    Bits: 15:11 tco_compf(rw) - Driver TCO delay control fall.
    Bits: 10:6  pdn_scomp_code(rw) - Driver pull down slew rate control setting.
    Bits:  5:0  pdn_odt_rcomp(rw) - Pull down ODT setting for all LVCMOS pins.
*/
#define NFP_PL_PluIOControls_PluLvcmos0IOCtrl2               0x00040004
#define NFP_PL_PluIOControls_PluLvcmos1IOCtrl2               0x00040024
#define   NFP_PL_PluIOControls_PluLvcmosIOCtrl2_tco_compr(_x) (((_x) & 0x1f) << 27)
#define   NFP_PL_PluIOControls_PluLvcmosIOCtrl2_tco_compr_of(_x) (((_x) >> 27) & 0x1f)
#define   NFP_PL_PluIOControls_PluLvcmosIOCtrl2_pup_scomp_code(_x) (((_x) & 0x1f) << 22)
#define   NFP_PL_PluIOControls_PluLvcmosIOCtrl2_pup_scomp_code_of(_x) (((_x) >> 22) & 0x1f)
#define   NFP_PL_PluIOControls_PluLvcmosIOCtrl2_pup_odt_rcomp(_x) (((_x) & 0x3f) << 16)
#define   NFP_PL_PluIOControls_PluLvcmosIOCtrl2_pup_odt_rcomp_of(_x) (((_x) >> 16) & 0x3f)
#define   NFP_PL_PluIOControls_PluLvcmosIOCtrl2_tco_compf(_x) (((_x) & 0x1f) << 11)
#define   NFP_PL_PluIOControls_PluLvcmosIOCtrl2_tco_compf_of(_x) (((_x) >> 11) & 0x1f)
#define   NFP_PL_PluIOControls_PluLvcmosIOCtrl2_pdn_scomp_code(_x) (((_x) & 0x1f) << 6)
#define   NFP_PL_PluIOControls_PluLvcmosIOCtrl2_pdn_scomp_code_of(_x) (((_x) >> 6) & 0x1f)
#define   NFP_PL_PluIOControls_PluLvcmosIOCtrl2_pdn_odt_rcomp(_x) (((_x) & 0x3f) << 0)
#define   NFP_PL_PluIOControls_PluLvcmosIOCtrl2_pdn_odt_rcomp_of(_x) (((_x) >> 0) & 0x3f)
/*----------------------------------------------------------------
  Register: PluIOControls_PluLvcmos0IODFT - LVCMOS0 IO DFT Control
  Register: PluIOControls_PluLvcmos1IODFT - LVCMOS1 IO DFT Control
    Bits: 11    padrx2txlb(rw) - LVCMOS pins RX to TX internal loopback enable. High active.
    Bits: 10    tsel(rw) - Test mode select for all ILK LVCMOS pins.
    Bits:  9    toenb(rw) - Test output enable. Low active.
    Bits:  8    open_drain(rw) - Open drain mode enable for all ILK LVCMOS pins.
    Bits:  7    force_tristateb(rw) - Tri-state enable for all ILK LVCMOS pins.
    Bits:  6    rxoffseten(rw) - Receiver offset enable for all ILK LVCMOS pins.
    Bits:  5:0  rxoffset(rw) - Receiver offset setting for all ILK LVCMOS pins.
*/
#define NFP_PL_PluIOControls_PluLvcmos0IODFT                 0x00040008
#define NFP_PL_PluIOControls_PluLvcmos1IODFT                 0x00040028
#define   NFP_PL_PluIOControls_PluLvcmosIODFT_padrx2txlb     BIT(11)
#define   NFP_PL_PluIOControls_PluLvcmosIODFT_tsel           BIT(10)
#define     NFP_PL_PluIOControls_PluLvcmosIODFT_tsel_0       (0 << 10)
#define     NFP_PL_PluIOControls_PluLvcmosIODFT_tsel_1       BIT(10)
#define   NFP_PL_PluIOControls_PluLvcmosIODFT_toenb          BIT(9)
#define   NFP_PL_PluIOControls_PluLvcmosIODFT_open_drain     BIT(8)
#define   NFP_PL_PluIOControls_PluLvcmosIODFT_force_tristateb BIT(7)
#define   NFP_PL_PluIOControls_PluLvcmosIODFT_rxoffseten     BIT(6)
#define   NFP_PL_PluIOControls_PluLvcmosIODFT_rxoffset(_x)   (((_x) & 0x3f) << 0)
#define   NFP_PL_PluIOControls_PluLvcmosIODFT_rxoffset_of(_x) (((_x) >> 0) & 0x3f)
/*----------------------------------------------------------------
  Register: PluIOControls_PluLvcmos0IODFT2 - For debug only.
  Register: PluIOControls_PluLvcmos1IODFT2 - For debug only.
    Bits: 22    ilk1_rdata_pododtmode(rw) - For debug only.
    Bits: 21    ilk1_rdata_dlycellmux(rw) - For debug only.
    Bits: 20    ilk1_rdata_dlycellstaticleg(rw) - For debug only.
    Bits: 19    ilk1_rdata_odtenb(rw) - For debug only.
    Bits: 18    ilk1_rdata_pdndrvodtrcompstatic(rw) - For debug only.
    Bits: 17    ilk1_rdata_pdndrvrcompstatic(rw) - For debug only.
    Bits: 16    ilk1_rdata_pupdrvodtrcompstatic(rw) - For debug only.
    Bits: 15    ilk1_rdata_pupdrvrcompstatic(rw) - For debug only.
    Bits: 14    ilk1_rdata_rxen1_15(rw) - For debug only.
    Bits: 13    ilk1_rdata_rxen1_h1x(rw) - For debug only.
    Bits: 12    ilk1_rdata_rxen2_15(rw) - For debug only.
    Bits: 11    ilk1_rdata_rxen2_h1x(rw) - For debug only.
    Bits: 10    ilk1_rdata_specmodesel(rw) - For debug only.
    Bits:  9:8  ilk1_rdata_drvodtsegen(rw) - For debug only.
    Bits:  7:6  ilk1_rdata_odtmode(rw) - For debug only.
    Bits:  5:4  ilk1_rdata_rxpathsel(rw) - For debug only.
    Bits:  3:0  ilk1_rdata_drvsegen(rw) - For debug only.
*/
#define NFP_PL_PluIOControls_PluLvcmos0IODFT2                0x0004000c
#define NFP_PL_PluIOControls_PluLvcmos1IODFT2                0x0004002c
#define   NFP_PL_PluIOControls_PluLvcmosIODFT2_ilk1_rdata_pododtmode BIT(22)
#define   NFP_PL_PluIOControls_PluLvcmosIODFT2_ilk1_rdata_dlycellmux BIT(21)
#define   NFP_PL_PluIOControls_PluLvcmosIODFT2_ilk1_rdata_dlycellstaticleg BIT(20)
#define   NFP_PL_PluIOControls_PluLvcmosIODFT2_ilk1_rdata_odtenb BIT(19)
#define   NFP_PL_PluIOControls_PluLvcmosIODFT2_ilk1_rdata_pdndrvodtrcompstatic BIT(18)
#define   NFP_PL_PluIOControls_PluLvcmosIODFT2_ilk1_rdata_pdndrvrcompstatic BIT(17)
#define   NFP_PL_PluIOControls_PluLvcmosIODFT2_ilk1_rdata_pupdrvodtrcompstatic BIT(16)
#define   NFP_PL_PluIOControls_PluLvcmosIODFT2_ilk1_rdata_pupdrvrcompstatic BIT(15)
#define   NFP_PL_PluIOControls_PluLvcmosIODFT2_ilk1_rdata_rxen1_15 BIT(14)
#define   NFP_PL_PluIOControls_PluLvcmosIODFT2_ilk1_rdata_rxen1_h1x BIT(13)
#define   NFP_PL_PluIOControls_PluLvcmosIODFT2_ilk1_rdata_rxen2_15 BIT(12)
#define   NFP_PL_PluIOControls_PluLvcmosIODFT2_ilk1_rdata_rxen2_h1x BIT(11)
#define   NFP_PL_PluIOControls_PluLvcmosIODFT2_ilk1_rdata_specmodesel BIT(10)
#define   NFP_PL_PluIOControls_PluLvcmosIODFT2_ilk1_rdata_drvodtsegen(_x) (((_x) & 0x3) << 8)
#define   NFP_PL_PluIOControls_PluLvcmosIODFT2_ilk1_rdata_drvodtsegen_of(_x) (((_x) >> 8) & 0x3)
#define   NFP_PL_PluIOControls_PluLvcmosIODFT2_ilk1_rdata_odtmode(_x) (((_x) & 0x3) << 6)
#define   NFP_PL_PluIOControls_PluLvcmosIODFT2_ilk1_rdata_odtmode_of(_x) (((_x) >> 6) & 0x3)
#define   NFP_PL_PluIOControls_PluLvcmosIODFT2_ilk1_rdata_rxpathsel(_x) (((_x) & 0x3) << 4)
#define   NFP_PL_PluIOControls_PluLvcmosIODFT2_ilk1_rdata_rxpathsel_of(_x) (((_x) >> 4) & 0x3)
#define   NFP_PL_PluIOControls_PluLvcmosIODFT2_ilk1_rdata_drvsegen(_x) (((_x) & 0xf) << 0)
#define   NFP_PL_PluIOControls_PluLvcmosIODFT2_ilk1_rdata_drvsegen_of(_x) (((_x) >> 0) & 0xf)
/*----------------------------------------------------------------
  Register: PluIOControls_PluLvcmos0IOComp - LVCMOS0 IO COMP Control.
  Register: PluIOControls_PluLvcmos1IOComp - LVCMOS1 IO COMP Control.
    Bits: 29:27 gdcdrvofst_1(rw) - Analog offset control for RCOMP pull up operation.
    Bits: 26:24 gdcdrvofst_0(rw) - Analog offset control for RCOMP pull down operation.
    Bits: 23:20 driversegen_0(rw) - Driver segment control.
    Bits: 19:18 driverodtsegen_0(rw) - Driver ODT segment control.
    Bits: 17:16 compck_div(rw) - RCOMP clock divider ratio.
    Bits: 15    rcomp_pup_en(rw) - RCOMP function pull up only enable.
    Bits: 14    rcomp_pup_ovrd_en(rw) - RCOMP pull up override enable, use override values.
    Bits: 13:8  pup_drv_ovrd(rw) - Driver pull up drive strength override value.
    Bits:  7    rcomp_pdn_en(rw) - RCOMP function pull down only enable.
    Bits:  6    rcomp_pdn_ovrd_en(rw) - RCOMP pull down override enable, use override values.
    Bits:  5:0  pdn_drv_ovrd(rw) - Driver pull down drive strength override value.
*/
#define NFP_PL_PluIOControls_PluLvcmos0IOComp                0x00040010
#define NFP_PL_PluIOControls_PluLvcmos1IOComp                0x00040030
#define   NFP_PL_PluIOControls_PluLvcmosIOComp_gdcdrvofst_1(_x) (((_x) & 0x7) << 27)
#define   NFP_PL_PluIOControls_PluLvcmosIOComp_gdcdrvofst_1_of(_x) (((_x) >> 27) & 0x7)
#define   NFP_PL_PluIOControls_PluLvcmosIOComp_gdcdrvofst_0(_x) (((_x) & 0x7) << 24)
#define   NFP_PL_PluIOControls_PluLvcmosIOComp_gdcdrvofst_0_of(_x) (((_x) >> 24) & 0x7)
#define   NFP_PL_PluIOControls_PluLvcmosIOComp_driversegen_0(_x) (((_x) & 0xf) << 20)
#define   NFP_PL_PluIOControls_PluLvcmosIOComp_driversegen_0_of(_x) (((_x) >> 20) & 0xf)
#define   NFP_PL_PluIOControls_PluLvcmosIOComp_driverodtsegen_0(_x) (((_x) & 0x3) << 18)
#define   NFP_PL_PluIOControls_PluLvcmosIOComp_driverodtsegen_0_of(_x) (((_x) >> 18) & 0x3)
#define   NFP_PL_PluIOControls_PluLvcmosIOComp_compck_div(_x) (((_x) & 0x3) << 16)
#define   NFP_PL_PluIOControls_PluLvcmosIOComp_compck_div_of(_x) (((_x) >> 16) & 0x3)
#define   NFP_PL_PluIOControls_PluLvcmosIOComp_rcomp_pup_en  BIT(15)
#define   NFP_PL_PluIOControls_PluLvcmosIOComp_rcomp_pup_ovrd_en BIT(14)
#define   NFP_PL_PluIOControls_PluLvcmosIOComp_pup_drv_ovrd(_x) (((_x) & 0x3f) << 8)
#define   NFP_PL_PluIOControls_PluLvcmosIOComp_pup_drv_ovrd_of(_x) (((_x) >> 8) & 0x3f)
#define   NFP_PL_PluIOControls_PluLvcmosIOComp_rcomp_pdn_en  BIT(7)
#define   NFP_PL_PluIOControls_PluLvcmosIOComp_rcomp_pdn_ovrd_en BIT(6)
#define   NFP_PL_PluIOControls_PluLvcmosIOComp_pdn_drv_ovrd(_x) (((_x) & 0x3f) << 0)
#define   NFP_PL_PluIOControls_PluLvcmosIOComp_pdn_drv_ovrd_of(_x) (((_x) >> 0) & 0x3f)
/*----------------------------------------------------------------
  Register: PluIOControls_PluLvcmos0IOComp2 - LVCMOS0 IO COMP Control 2.
  Register: PluIOControls_PluLvcmos1IOComp2 - LVCMOS1 IO COMP Control 2.
    Bits: 31    rcomp_start(rw) - Start RCOMP Sequence.
    Bits: 30    rcomp_reset(rw) - Synchronous high-active RCOMP Reset.
    Bits: 18    rcomp_ready(ro) - RCOMP ready signal.
    Bits: 17    rcomp_update(ro) - RCOMP FSM update signal.
    Bits: 16    rcomp_timeout(ro) - RCOMP FSM TIMEOUT - ready never came.
    Bits: 13:8  pdn_drv_rcomp(ro) - IO buffer drive strength pull down setting from RCOMP.
    Bits:  5:0  pup_drv_rcomp(ro) - IO buffer drive strength pull up setting from RCOMP.
*/
#define NFP_PL_PluIOControls_PluLvcmos0IOComp2               0x00040014
#define NFP_PL_PluIOControls_PluLvcmos1IOComp2               0x00040034
#define   NFP_PL_PluIOControls_PluLvcmosIOComp2_rcomp_start  BIT(31)
#define   NFP_PL_PluIOControls_PluLvcmosIOComp2_rcomp_reset  BIT(30)
#define   NFP_PL_PluIOControls_PluLvcmosIOComp2_rcomp_ready  BIT(18)
#define   NFP_PL_PluIOControls_PluLvcmosIOComp2_rcomp_update BIT(17)
#define   NFP_PL_PluIOControls_PluLvcmosIOComp2_rcomp_timeout BIT(16)
#define   NFP_PL_PluIOControls_PluLvcmosIOComp2_pdn_drv_rcomp(_x) (((_x) & 0x3f) << 8)
#define   NFP_PL_PluIOControls_PluLvcmosIOComp2_pdn_drv_rcomp_of(_x) (((_x) >> 8) & 0x3f)
#define   NFP_PL_PluIOControls_PluLvcmosIOComp2_pup_drv_rcomp(_x) (((_x) & 0x3f) << 0)
#define   NFP_PL_PluIOControls_PluLvcmosIOComp2_pup_drv_rcomp_of(_x) (((_x) >> 0) & 0x3f)
/*----------------------------------------------------------------
  Register: PluIOControls_GpioB0Drive - GPIO driver controls for B0.
    Bits: 31:16 gpio_b0_pup_drv(rw) - Driver pull up strength for GPIO[7:0].
    Bits: 15:0  gpio_b0_pdn_drv(rw) - Driver pull down strength for GPIO[7:0].
*/
#define NFP_PL_PluIOControls_GpioB0Drive                     0x00040038
#define   NFP_PL_PluIOControls_GpioB0Drive_gpio_b0_pup_drv(_x) (((_x) & 0xffff) << 16)
#define   NFP_PL_PluIOControls_GpioB0Drive_gpio_b0_pup_drv_of(_x) (((_x) >> 16) & 0xffff)
#define   NFP_PL_PluIOControls_GpioB0Drive_gpio_b0_pdn_drv(_x) (((_x) & 0xffff) << 0)
#define   NFP_PL_PluIOControls_GpioB0Drive_gpio_b0_pdn_drv_of(_x) (((_x) >> 0) & 0xffff)
/*----------------------------------------------------------------
  Register: PluIOControls_GpioB1Drive - GPIO driver controls for B1.
    Bits: 31:16 gpio_b1_pup_drv(rw) - Driver pull up strength for GPIO[15:8].
    Bits: 15:0  gpio_b1_pdn_drv(rw) - Driver pull down strength for GPIO[15:8].
*/
#define NFP_PL_PluIOControls_GpioB1Drive                     0x0004003c
#define   NFP_PL_PluIOControls_GpioB1Drive_gpio_b1_pup_drv(_x) (((_x) & 0xffff) << 16)
#define   NFP_PL_PluIOControls_GpioB1Drive_gpio_b1_pup_drv_of(_x) (((_x) >> 16) & 0xffff)
#define   NFP_PL_PluIOControls_GpioB1Drive_gpio_b1_pdn_drv(_x) (((_x) & 0xffff) << 0)
#define   NFP_PL_PluIOControls_GpioB1Drive_gpio_b1_pdn_drv_of(_x) (((_x) >> 0) & 0xffff)
/*----------------------------------------------------------------
  Register: PluIOControls_GpioB2Drive - GPIO driver controls for B2.
    Bits: 31:16 gpio_b2_pup_drv(rw) - Driver pull up strength for GPIO[23:16].
    Bits: 15:0  gpio_b2_pdn_drv(rw) - Driver pull down strength for GPIO[23:16].
*/
#define NFP_PL_PluIOControls_GpioB2Drive                     0x00040040
#define   NFP_PL_PluIOControls_GpioB2Drive_gpio_b2_pup_drv(_x) (((_x) & 0xffff) << 16)
#define   NFP_PL_PluIOControls_GpioB2Drive_gpio_b2_pup_drv_of(_x) (((_x) >> 16) & 0xffff)
#define   NFP_PL_PluIOControls_GpioB2Drive_gpio_b2_pdn_drv(_x) (((_x) & 0xffff) << 0)
#define   NFP_PL_PluIOControls_GpioB2Drive_gpio_b2_pdn_drv_of(_x) (((_x) >> 0) & 0xffff)
/*----------------------------------------------------------------
  Register: PluIOControls_GpioB3Drive - GPIO driver controls for B3.
    Bits: 31:16 gpio_b3_pup_drv(rw) - Driver pull up strength for GPIO[31:24].
    Bits: 15:0  gpio_b3_pdn_drv(rw) - Driver pull down strength for GPIO[31:24].
*/
#define NFP_PL_PluIOControls_GpioB3Drive                     0x00040044
#define   NFP_PL_PluIOControls_GpioB3Drive_gpio_b3_pup_drv(_x) (((_x) & 0xffff) << 16)
#define   NFP_PL_PluIOControls_GpioB3Drive_gpio_b3_pup_drv_of(_x) (((_x) >> 16) & 0xffff)
#define   NFP_PL_PluIOControls_GpioB3Drive_gpio_b3_pdn_drv(_x) (((_x) & 0xffff) << 0)
#define   NFP_PL_PluIOControls_GpioB3Drive_gpio_b3_pdn_drv_of(_x) (((_x) >> 0) & 0xffff)
/*----------------------------------------------------------------
  Register: PluIOControls_SpiDrive - SPI driver controls.
    Bits: 31:16 spi_pup_drv(rw) - Driver pull up strength for all SPI pins.
    Bits: 15:0  spi_pdn_drv(rw) - Driver pull down strength for all SPI pins.
*/
#define NFP_PL_PluIOControls_SpiDrive                        0x00040048
#define   NFP_PL_PluIOControls_SpiDrive_spi_pup_drv(_x)      (((_x) & 0xffff) << 16)
#define   NFP_PL_PluIOControls_SpiDrive_spi_pup_drv_of(_x)   (((_x) >> 16) & 0xffff)
#define   NFP_PL_PluIOControls_SpiDrive_spi_pdn_drv(_x)      (((_x) & 0xffff) << 0)
#define   NFP_PL_PluIOControls_SpiDrive_spi_pdn_drv_of(_x)   (((_x) >> 0) & 0xffff)
/*----------------------------------------------------------------
  Register: PluIOControls_MiscDrive - Misc. driver controls.
    Bits: 31:16 misc_pup_drv(rw) - Driver pull up strength for all remaining LVTTL IO pins.
    Bits: 15:0  misc_pdn_drv(rw) - Driver pull down strength for all remaining LVTTL IO pins.
*/
#define NFP_PL_PluIOControls_MiscDrive                       0x0004004c
#define   NFP_PL_PluIOControls_MiscDrive_misc_pup_drv(_x)    (((_x) & 0xffff) << 16)
#define   NFP_PL_PluIOControls_MiscDrive_misc_pup_drv_of(_x) (((_x) >> 16) & 0xffff)
#define   NFP_PL_PluIOControls_MiscDrive_misc_pdn_drv(_x)    (((_x) & 0xffff) << 0)
#define   NFP_PL_PluIOControls_MiscDrive_misc_pdn_drv_of(_x) (((_x) >> 0) & 0xffff)
/*----------------------------------------------------------------
  Register: PluIOControls_LvttlIoSlew - Drive slew rate for LVTTL IO buffers.
    Bits: 23:22 misc_pup_slew(rw) - Driver pull up slew rate for all remaining LVTTL IO pins.
    Bits: 21:20 misc_pdn_slew(rw) - Driver pull down slew rate for all remaining LVTTL IO pins.
    Bits: 19:18 spi_pup_slew(rw) - Driver pull up slew rate for all SPI pins.
    Bits: 17:16 spi_pdn_slew(rw) - Driver pull down slew rate for all SPI pins.
    Bits: 15:14 gpio_b3_pup_slew(rw) - Driver pull up slew rate for GPIO[31:24].
    Bits: 13:12 gpio_b3_pdn_slew(rw) - Driver pull down slew rate for GPIO[31:24].
    Bits: 11:10 gpio_b2_pup_slew(rw) - Driver pull up slew rate for GPIO[23:16].
    Bits:  9:8  gpio_b2_pdn_slew(rw) - Driver pull down slew rate for GPIO[23:16].
    Bits:  7:6  gpio_b1_pup_slew(rw) - Driver pull up slew rate for GPIO[15:8].
    Bits:  5:4  gpio_b1_pdn_slew(rw) - Driver pull down slew rate for GPIO[15:8].
    Bits:  3:2  gpio_b0_pup_slew(rw) - Driver pull up slew rate for GPIO[7:0].
    Bits:  1:0  gpio_b0_pdn_slew(rw) - Driver pull down slew rate for GPIO[7:0].
*/
#define NFP_PL_PluIOControls_LvttlIoSlew                     0x00040050
#define   NFP_PL_PluIOControls_LvttlIoSlew_misc_pup_slew(_x) (((_x) & 0x3) << 22)
#define   NFP_PL_PluIOControls_LvttlIoSlew_misc_pup_slew_of(_x) (((_x) >> 22) & 0x3)
#define   NFP_PL_PluIOControls_LvttlIoSlew_misc_pdn_slew(_x) (((_x) & 0x3) << 20)
#define   NFP_PL_PluIOControls_LvttlIoSlew_misc_pdn_slew_of(_x) (((_x) >> 20) & 0x3)
#define   NFP_PL_PluIOControls_LvttlIoSlew_spi_pup_slew(_x)  (((_x) & 0x3) << 18)
#define   NFP_PL_PluIOControls_LvttlIoSlew_spi_pup_slew_of(_x) (((_x) >> 18) & 0x3)
#define   NFP_PL_PluIOControls_LvttlIoSlew_spi_pdn_slew(_x)  (((_x) & 0x3) << 16)
#define   NFP_PL_PluIOControls_LvttlIoSlew_spi_pdn_slew_of(_x) (((_x) >> 16) & 0x3)
#define   NFP_PL_PluIOControls_LvttlIoSlew_gpio_b3_pup_slew(_x) (((_x) & 0x3) << 14)
#define   NFP_PL_PluIOControls_LvttlIoSlew_gpio_b3_pup_slew_of(_x) (((_x) >> 14) & 0x3)
#define   NFP_PL_PluIOControls_LvttlIoSlew_gpio_b3_pdn_slew(_x) (((_x) & 0x3) << 12)
#define   NFP_PL_PluIOControls_LvttlIoSlew_gpio_b3_pdn_slew_of(_x) (((_x) >> 12) & 0x3)
#define   NFP_PL_PluIOControls_LvttlIoSlew_gpio_b2_pup_slew(_x) (((_x) & 0x3) << 10)
#define   NFP_PL_PluIOControls_LvttlIoSlew_gpio_b2_pup_slew_of(_x) (((_x) >> 10) & 0x3)
#define   NFP_PL_PluIOControls_LvttlIoSlew_gpio_b2_pdn_slew(_x) (((_x) & 0x3) << 8)
#define   NFP_PL_PluIOControls_LvttlIoSlew_gpio_b2_pdn_slew_of(_x) (((_x) >> 8) & 0x3)
#define   NFP_PL_PluIOControls_LvttlIoSlew_gpio_b1_pup_slew(_x) (((_x) & 0x3) << 6)
#define   NFP_PL_PluIOControls_LvttlIoSlew_gpio_b1_pup_slew_of(_x) (((_x) >> 6) & 0x3)
#define   NFP_PL_PluIOControls_LvttlIoSlew_gpio_b1_pdn_slew(_x) (((_x) & 0x3) << 4)
#define   NFP_PL_PluIOControls_LvttlIoSlew_gpio_b1_pdn_slew_of(_x) (((_x) >> 4) & 0x3)
#define   NFP_PL_PluIOControls_LvttlIoSlew_gpio_b0_pup_slew(_x) (((_x) & 0x3) << 2)
#define   NFP_PL_PluIOControls_LvttlIoSlew_gpio_b0_pup_slew_of(_x) (((_x) >> 2) & 0x3)
#define   NFP_PL_PluIOControls_LvttlIoSlew_gpio_b0_pdn_slew(_x) (((_x) & 0x3) << 0)
#define   NFP_PL_PluIOControls_LvttlIoSlew_gpio_b0_pdn_slew_of(_x) (((_x) >> 0) & 0x3)
/*----------------------------------------------------------------
  Register: PluIOControls_GpioWkpullEn - Weak pull up pull down for GPIO.
    Bits: 31:0  gpio_wkpull_en(rw) - Weak pull up pull down enable for GPIO[31:0] (1: weak pull up/down enabled, 0: weak pull up/down disabled).
*/
#define NFP_PL_PluIOControls_GpioWkpullEn                    0x00040054
#define   NFP_PL_PluIOControls_GpioWkpullEn_gpio_wkpull_en(_x) (((_x) & 0xffffffff) << 0)
#define   NFP_PL_PluIOControls_GpioWkpullEn_gpio_wkpull_en_of(_x) (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: PluIOControls_GpioWkPolarity - Weak pull polarity for GPIO.
    Bits: 31:0  gpio_wk_polarity(rw) - Weak pull polarity for GPIO[31:0] (1: weak pull up effective, 0: weal pull down effective).
*/
#define NFP_PL_PluIOControls_GpioWkPolarity                  0x00040058
#define   NFP_PL_PluIOControls_GpioWkPolarity_gpio_wk_polarity(_x) (((_x) & 0xffffffff) << 0)
#define   NFP_PL_PluIOControls_GpioWkPolarity_gpio_wk_polarity_of(_x) (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: PluIOControls_GpioWkpullVal - Weak pull up pull down value for GPIO.
    Bits: 31:0  gpio_wkpull_value(rw) - Weak pull up pull down value for GPIO[31:0] (1: 20kohm, 0: 2kohm).
*/
#define NFP_PL_PluIOControls_GpioWkpullVal                   0x0004005c
#define   NFP_PL_PluIOControls_GpioWkpullVal_gpio_wkpull_value(_x) (((_x) & 0xffffffff) << 0)
#define   NFP_PL_PluIOControls_GpioWkpullVal_gpio_wkpull_value_of(_x) (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: PluIOControls_SrTestTxEnb - Buffer output enable for Serial port, backup test pins.
    Bits:  5    test_backup3_oenb(rw) - Buffer output enable for backup3 pin.
    Bits:  4    test_backup2_oenb(rw) - Buffer output enable for backup2 pin.
    Bits:  3    test_backup1_oenb(rw) - Buffer output enable for backup1 pin.
    Bits:  2    test_backup0_oenb(rw) - Buffer output enable for backup0 pin.
    Bits:  1    sr_tx_oenb(rw) - Buffer output enable for Serial port (1: off, 0: on).
    Bits:  0    sr_rx_oenb(rw) - Buffer output enable for Serial port (1: off, 0: on).
*/
#define NFP_PL_PluIOControls_SrTestTxEnb                     0x00040060
#define   NFP_PL_PluIOControls_SrTestTxEnb_test_backup3_oenb BIT(5)
#define   NFP_PL_PluIOControls_SrTestTxEnb_test_backup2_oenb BIT(4)
#define   NFP_PL_PluIOControls_SrTestTxEnb_test_backup1_oenb BIT(3)
#define   NFP_PL_PluIOControls_SrTestTxEnb_test_backup0_oenb BIT(2)
#define   NFP_PL_PluIOControls_SrTestTxEnb_sr_tx_oenb        BIT(1)
#define   NFP_PL_PluIOControls_SrTestTxEnb_sr_rx_oenb        BIT(0)
/*----------------------------------------------------------------
  Register: PluIOControls_GpioRxEnb - Buffer input enable for GPIO.
    Bits: 31:0  gpio_rcvenv(rw) - Buffer input enable for GPIO[31:0], active low.
*/
#define NFP_PL_PluIOControls_GpioRxEnb                       0x00040064
#define   NFP_PL_PluIOControls_GpioRxEnb_gpio_rcvenv(_x)     (((_x) & 0xffffffff) << 0)
#define   NFP_PL_PluIOControls_GpioRxEnb_gpio_rcvenv_of(_x)  (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: PluIOControls_SpiRxEnb - Buffer input enable for SPI, serial port and backup test pins.
    Bits: 23    test_backup3_rcvenb(rw) - Buffer input enable for test backup pins.
    Bits: 22    test_backup2_rcvenb(rw) - Buffer input enable for test backup pins.
    Bits: 21    test_backup1_rcvenb(rw) - Buffer input enable for test backup pins.
    Bits: 20    test_backup0_rcvenb(rw) - Buffer input enable for test backup pins.
    Bits: 19    sr_tx_rcvenb(rw) - Buffer input enable for Serial port pins (0: on, 1: off).
    Bits: 18    sr_rx_rcvenb(rw) - Buffer input enable for Serial port pins (0: on, 1: off).
    Bits: 17    spix_sel3_rcvenb(rw) - Buffer input enable for SPI pins.
    Bits: 16    spix_sel2_rcvenb(rw) - Buffer input enable for SPI pins.
    Bits: 15    spix_sel1_rcvenb(rw) - Buffer input enable for SPI pins.
    Bits: 14    spix_hold_l_rcvenb(rw) - Buffer input enable for SPI pins.
    Bits: 13    spix_d3_rcvenb(rw) - Buffer input enable for SPI pins.
    Bits: 12    spix_d2_rcvenb(rw) - Buffer input enable for SPI pins.
    Bits: 11    spi3_sck_rcvenb(rw) - Buffer input enable for SPI pins.
    Bits: 10    spi3_mosi_rcvenb(rw) - Buffer input enable for SPI pins.
    Bits:  9    spi2_sck_rcvenb(rw) - Buffer input enable for SPI pins.
    Bits:  8    spi2_mosi_rcvenb(rw) - Buffer input enable for SPI pins.
    Bits:  7    spi1_cfg_sel_rcvenb(rw) - Buffer input enable for SPI pins.
    Bits:  6    spi1_cfg_sck_rcvenb(rw) - Buffer input enable for SPI pins.
    Bits:  5    spi1_cfg_mosi_rcvenb(rw) - Buffer input enable for SPI pins.
    Bits:  4    spi1_cfg_miso_rcvenb(rw) - Buffer input enable for SPI pins.
    Bits:  3    spi0_sel_rcvenb(rw) - Buffer input enable for SPI pins.
    Bits:  2    spi0_sck_rcvenb(rw) - Buffer input enable for SPI pins.
    Bits:  1    spi0_mosi_rcvenb(rw) - Buffer input enable for SPI pins.
    Bits:  0    spi0_miso_rcvenb(rw) - Buffer input enable for SPI pins.
*/
#define NFP_PL_PluIOControls_SpiRxEnb                        0x00040068
#define   NFP_PL_PluIOControls_SpiRxEnb_test_backup3_rcvenb  BIT(23)
#define   NFP_PL_PluIOControls_SpiRxEnb_test_backup2_rcvenb  BIT(22)
#define   NFP_PL_PluIOControls_SpiRxEnb_test_backup1_rcvenb  BIT(21)
#define   NFP_PL_PluIOControls_SpiRxEnb_test_backup0_rcvenb  BIT(20)
#define   NFP_PL_PluIOControls_SpiRxEnb_sr_tx_rcvenb         BIT(19)
#define   NFP_PL_PluIOControls_SpiRxEnb_sr_rx_rcvenb         BIT(18)
#define   NFP_PL_PluIOControls_SpiRxEnb_spix_sel3_rcvenb     BIT(17)
#define   NFP_PL_PluIOControls_SpiRxEnb_spix_sel2_rcvenb     BIT(16)
#define   NFP_PL_PluIOControls_SpiRxEnb_spix_sel1_rcvenb     BIT(15)
#define   NFP_PL_PluIOControls_SpiRxEnb_spix_hold_l_rcvenb   BIT(14)
#define   NFP_PL_PluIOControls_SpiRxEnb_spix_d3_rcvenb       BIT(13)
#define   NFP_PL_PluIOControls_SpiRxEnb_spix_d2_rcvenb       BIT(12)
#define   NFP_PL_PluIOControls_SpiRxEnb_spi3_sck_rcvenb      BIT(11)
#define   NFP_PL_PluIOControls_SpiRxEnb_spi3_mosi_rcvenb     BIT(10)
#define   NFP_PL_PluIOControls_SpiRxEnb_spi2_sck_rcvenb      BIT(9)
#define   NFP_PL_PluIOControls_SpiRxEnb_spi2_mosi_rcvenb     BIT(8)
#define   NFP_PL_PluIOControls_SpiRxEnb_spi1_cfg_sel_rcvenb  BIT(7)
#define   NFP_PL_PluIOControls_SpiRxEnb_spi1_cfg_sck_rcvenb  BIT(6)
#define   NFP_PL_PluIOControls_SpiRxEnb_spi1_cfg_mosi_rcvenb BIT(5)
#define   NFP_PL_PluIOControls_SpiRxEnb_spi1_cfg_miso_rcvenb BIT(4)
#define   NFP_PL_PluIOControls_SpiRxEnb_spi0_sel_rcvenb      BIT(3)
#define   NFP_PL_PluIOControls_SpiRxEnb_spi0_sck_rcvenb      BIT(2)
#define   NFP_PL_PluIOControls_SpiRxEnb_spi0_mosi_rcvenb     BIT(1)
#define   NFP_PL_PluIOControls_SpiRxEnb_spi0_miso_rcvenb     BIT(0)
/*----------------------------------------------------------------
  Register: PluIOControls_GpioOpendrain - Open drain enable for GPIO.
    Bits: 31:0  gpio_opendrn(rw) - Open drain enable for GPIO[31:0].
*/
#define NFP_PL_PluIOControls_GpioOpendrain                   0x0004006c
#define   NFP_PL_PluIOControls_GpioOpendrain_gpio_opendrn(_x) (((_x) & 0xffffffff) << 0)
#define   NFP_PL_PluIOControls_GpioOpendrain_gpio_opendrn_of(_x) (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: PluIOControls_MiscOpendrain - Open drain enable for SPI, reset output and other IO pins.
    Bits: 28    test_opendrn(rw) - Open drain enable for test pin.
    Bits: 27    misc_opendrn(rw) - Open drain enable for misc pin.
    Bits: 26    test_backup3_opendrn(rw) - Open drain enable for test backup pins.
    Bits: 25    test_backup2_opendrn(rw) - Open drain enable for test backup pins.
    Bits: 24    test_backup1_opendrn(rw) - Open drain enable for test backup pins.
    Bits: 23    test_backup0_opendrn(rw) - Open drain enable for test backup pins.
    Bits: 22    sr_tx_opendrn(rw) - Open drain enable for Serial pins.
    Bits: 21    pcie3_reset_out_l_opendrn(rw) - Open drain enable for PCIE reset output pins.
    Bits: 20    pcie2_reset_out_l_opendrn(rw) - Open drain enable for PCIE reset output pins.
    Bits: 19    pcie1_reset_out_l_opendrn(rw) - Open drain enable for PCIE reset output pins.
    Bits: 18    pcie0_reset_out_l_opendrn(rw) - Open drain enable for PCIE reset output pins.
    Bits: 17    clk_nreset_out_l_opendrn(rw) - Open drain enable for reset output pins.
    Bits: 16    spix_sel3_opendrn(rw) - Open drain enable for SPI pins.
    Bits: 15    spix_sel2_opendrn(rw) - Open drain enable for SPI pins.
    Bits: 14    spix_sel1_opendrn(rw) - Open drain enable for SPI pins.
    Bits: 13    spix_d3_opendrn(rw) - Open drain enable for SPI pins.
    Bits: 12    spix_d2_opendrn(rw) - Open drain enable for SPI pins.
    Bits: 11    spi3_sck_opendrn(rw) - Open drain enable for SPI pins.
    Bits: 10    spi3_mosi_opendrn(rw) - Open drain enable for SPI pins.
    Bits:  9    spi2_sck_opendrn(rw) - Open drain enable for SPI pins.
    Bits:  8    spi2_mosi_opendrn(rw) - Open drain enable for SPI pins.
    Bits:  7    spi1_cfg_sel_opendrn(rw) - Open drain enable for SPI pins.
    Bits:  6    spi1_cfg_sck_opendrn(rw) - Open drain enable for SPI pins.
    Bits:  5    spi1_cfg_mosi_opendrn(rw) - Open drain enable for SPI pins.
    Bits:  4    spi1_cfg_miso_opendrn(rw) - Open drain enable for SPI pins.
    Bits:  3    spi0_sel_opendrn(rw) - Open drain enable for SPI pins.
    Bits:  2    spi0_sck_opendrn(rw) - Open drain enable for SPI pins.
    Bits:  1    spi0_mosi_opendrn(rw) - Open drain enable for SPI pins.
    Bits:  0    spi0_miso_opendrn(rw) - Open drain enable for SPI pins.
*/
#define NFP_PL_PluIOControls_MiscOpendrain                   0x00040070
#define   NFP_PL_PluIOControls_MiscOpendrain_test_opendrn    BIT(28)
#define   NFP_PL_PluIOControls_MiscOpendrain_misc_opendrn    BIT(27)
#define   NFP_PL_PluIOControls_MiscOpendrain_test_backup3_opendrn BIT(26)
#define   NFP_PL_PluIOControls_MiscOpendrain_test_backup2_opendrn BIT(25)
#define   NFP_PL_PluIOControls_MiscOpendrain_test_backup1_opendrn BIT(24)
#define   NFP_PL_PluIOControls_MiscOpendrain_test_backup0_opendrn BIT(23)
#define   NFP_PL_PluIOControls_MiscOpendrain_sr_tx_opendrn   BIT(22)
#define   NFP_PL_PluIOControls_MiscOpendrain_pcie3_reset_out_l_opendrn BIT(21)
#define   NFP_PL_PluIOControls_MiscOpendrain_pcie2_reset_out_l_opendrn BIT(20)
#define   NFP_PL_PluIOControls_MiscOpendrain_pcie1_reset_out_l_opendrn BIT(19)
#define   NFP_PL_PluIOControls_MiscOpendrain_pcie0_reset_out_l_opendrn BIT(18)
#define   NFP_PL_PluIOControls_MiscOpendrain_clk_nreset_out_l_opendrn BIT(17)
#define   NFP_PL_PluIOControls_MiscOpendrain_spix_sel3_opendrn BIT(16)
#define   NFP_PL_PluIOControls_MiscOpendrain_spix_sel2_opendrn BIT(15)
#define   NFP_PL_PluIOControls_MiscOpendrain_spix_sel1_opendrn BIT(14)
#define   NFP_PL_PluIOControls_MiscOpendrain_spix_d3_opendrn BIT(13)
#define   NFP_PL_PluIOControls_MiscOpendrain_spix_d2_opendrn BIT(12)
#define   NFP_PL_PluIOControls_MiscOpendrain_spi3_sck_opendrn BIT(11)
#define   NFP_PL_PluIOControls_MiscOpendrain_spi3_mosi_opendrn BIT(10)
#define   NFP_PL_PluIOControls_MiscOpendrain_spi2_sck_opendrn BIT(9)
#define   NFP_PL_PluIOControls_MiscOpendrain_spi2_mosi_opendrn BIT(8)
#define   NFP_PL_PluIOControls_MiscOpendrain_spi1_cfg_sel_opendrn BIT(7)
#define   NFP_PL_PluIOControls_MiscOpendrain_spi1_cfg_sck_opendrn BIT(6)
#define   NFP_PL_PluIOControls_MiscOpendrain_spi1_cfg_mosi_opendrn BIT(5)
#define   NFP_PL_PluIOControls_MiscOpendrain_spi1_cfg_miso_opendrn BIT(4)
#define   NFP_PL_PluIOControls_MiscOpendrain_spi0_sel_opendrn BIT(3)
#define   NFP_PL_PluIOControls_MiscOpendrain_spi0_sck_opendrn BIT(2)
#define   NFP_PL_PluIOControls_MiscOpendrain_spi0_mosi_opendrn BIT(1)
#define   NFP_PL_PluIOControls_MiscOpendrain_spi0_miso_opendrn BIT(0)
/*----------------------------------------------------------------
  Register: PluIOControls_MiscControl - Misc controls.
    Bits: 13    test_force0(rw) - Force 0 on test pins.
    Bits: 12    test_force_tristate(rw) - Force tristate on test pins.
    Bits: 11    misc_force0(rw) - Force 0 on misc pins.
    Bits: 10    misc_force_tristate(rw) - Force tristate on misc pins.
    Bits:  9    spi_force0(rw) - Force 0 on spi pins.
    Bits:  8    spi_force_tristate(rw) - Force tristate on spi pins.
    Bits:  7    gpio_force0(rw) - Force 0 on gpio pins.
    Bits:  6    gpio_force_tristate(rw) - Force tristate on gpio pins.
    Bits:  5    misc_wkpull_value(rw) - Weak pull up pull down value for misc pins.
    Bits:  4    misc_wk_polarity(rw) - Weak pull polarity for misc pins.
    Bits:  3    misc_wkpull_en(rw) - Weak pull up pull down enable for misc pins.
    Bits:  2    spi_wkpull_value(rw) - Weak pull up pull down value for SPI pins.
    Bits:  1    spi_wk_polarity(rw) - Weak pull polarity for SPI pins.
    Bits:  0    spi_wkpull_en(rw) - Weak pull up pull down enable for SPI pins.
*/
#define NFP_PL_PluIOControls_MiscControl                     0x00040074
#define   NFP_PL_PluIOControls_MiscControl_test_force0       BIT(13)
#define     NFP_PL_PluIOControls_MiscControl_test_force0_0   (0 << 13)
#define     NFP_PL_PluIOControls_MiscControl_test_force0_1   BIT(13)
#define   NFP_PL_PluIOControls_MiscControl_test_force_tristate BIT(12)
#define     NFP_PL_PluIOControls_MiscControl_test_force_tristate_0 (0 << 12)
#define     NFP_PL_PluIOControls_MiscControl_test_force_tristate_1 BIT(12)
#define   NFP_PL_PluIOControls_MiscControl_misc_force0       BIT(11)
#define     NFP_PL_PluIOControls_MiscControl_misc_force0_0   (0 << 11)
#define     NFP_PL_PluIOControls_MiscControl_misc_force0_1   BIT(11)
#define   NFP_PL_PluIOControls_MiscControl_misc_force_tristate BIT(10)
#define     NFP_PL_PluIOControls_MiscControl_misc_force_tristate_0 (0 << 10)
#define     NFP_PL_PluIOControls_MiscControl_misc_force_tristate_1 BIT(10)
#define   NFP_PL_PluIOControls_MiscControl_spi_force0        BIT(9)
#define     NFP_PL_PluIOControls_MiscControl_spi_force0_0    (0 << 9)
#define     NFP_PL_PluIOControls_MiscControl_spi_force0_1    BIT(9)
#define   NFP_PL_PluIOControls_MiscControl_spi_force_tristate BIT(8)
#define     NFP_PL_PluIOControls_MiscControl_spi_force_tristate_0 (0 << 8)
#define     NFP_PL_PluIOControls_MiscControl_spi_force_tristate_1 BIT(8)
#define   NFP_PL_PluIOControls_MiscControl_gpio_force0       BIT(7)
#define     NFP_PL_PluIOControls_MiscControl_gpio_force0_0   (0 << 7)
#define     NFP_PL_PluIOControls_MiscControl_gpio_force0_1   BIT(7)
#define   NFP_PL_PluIOControls_MiscControl_gpio_force_tristate BIT(6)
#define     NFP_PL_PluIOControls_MiscControl_gpio_force_tristate_0 (0 << 6)
#define     NFP_PL_PluIOControls_MiscControl_gpio_force_tristate_1 BIT(6)
#define   NFP_PL_PluIOControls_MiscControl_misc_wkpull_value BIT(5)
#define     NFP_PL_PluIOControls_MiscControl_misc_wkpull_value_0 (0 << 5)
#define     NFP_PL_PluIOControls_MiscControl_misc_wkpull_value_1 BIT(5)
#define   NFP_PL_PluIOControls_MiscControl_misc_wk_polarity  BIT(4)
#define     NFP_PL_PluIOControls_MiscControl_misc_wk_polarity_0 (0 << 4)
#define     NFP_PL_PluIOControls_MiscControl_misc_wk_polarity_1 BIT(4)
#define   NFP_PL_PluIOControls_MiscControl_misc_wkpull_en    BIT(3)
#define     NFP_PL_PluIOControls_MiscControl_misc_wkpull_en_0 (0 << 3)
#define     NFP_PL_PluIOControls_MiscControl_misc_wkpull_en_1 BIT(3)
#define   NFP_PL_PluIOControls_MiscControl_spi_wkpull_value  BIT(2)
#define     NFP_PL_PluIOControls_MiscControl_spi_wkpull_value_0 (0 << 2)
#define     NFP_PL_PluIOControls_MiscControl_spi_wkpull_value_1 BIT(2)
#define   NFP_PL_PluIOControls_MiscControl_spi_wk_polarity   BIT(1)
#define     NFP_PL_PluIOControls_MiscControl_spi_wk_polarity_0 (0 << 1)
#define     NFP_PL_PluIOControls_MiscControl_spi_wk_polarity_1 BIT(1)
#define   NFP_PL_PluIOControls_MiscControl_spi_wkpull_en     BIT(0)
#define     NFP_PL_PluIOControls_MiscControl_spi_wkpull_en_0 (0 << 0)
#define     NFP_PL_PluIOControls_MiscControl_spi_wkpull_en_1 BIT(0)
/*----------------------------------------------------------------
  Register: PluIOControls_VidControl - VID controls.
    Bits: 15    vid_wk_polarity(rw) - Weak pull polarity for VID pins.
    Bits: 14    vid_wk_pull_en(rw) - Weak pull enable for VID pins.
    Bits: 13:8  vid_pdn_drv_rcomp(rw) - VID pins pull down drive strength.
    Bits:  7:2  vid_pup_drv_rcomp(rw) - VID pins pull up drive strength.
    Bits:  1    vid_opendrn(rw) - Open drain enable for VID pins.
    Bits:  0    vid_force_tristateb(rw) - Tri-state enable for VID pins.
*/
#define NFP_PL_PluIOControls_VidControl                      0x00040078
#define   NFP_PL_PluIOControls_VidControl_vid_wk_polarity    BIT(15)
#define     NFP_PL_PluIOControls_VidControl_vid_wk_polarity_0 (0 << 15)
#define     NFP_PL_PluIOControls_VidControl_vid_wk_polarity_1 BIT(15)
#define   NFP_PL_PluIOControls_VidControl_vid_wk_pull_en     BIT(14)
#define     NFP_PL_PluIOControls_VidControl_vid_wk_pull_en_0 (0 << 14)
#define     NFP_PL_PluIOControls_VidControl_vid_wk_pull_en_1 BIT(14)
#define   NFP_PL_PluIOControls_VidControl_vid_pdn_drv_rcomp(_x) (((_x) & 0x3f) << 8)
#define   NFP_PL_PluIOControls_VidControl_vid_pdn_drv_rcomp_of(_x) (((_x) >> 8) & 0x3f)
#define   NFP_PL_PluIOControls_VidControl_vid_pup_drv_rcomp(_x) (((_x) & 0x3f) << 2)
#define   NFP_PL_PluIOControls_VidControl_vid_pup_drv_rcomp_of(_x) (((_x) >> 2) & 0x3f)
#define   NFP_PL_PluIOControls_VidControl_vid_opendrn        BIT(1)
#define   NFP_PL_PluIOControls_VidControl_vid_force_tristateb BIT(0)

#endif /* NFP6000_LONGNAMES */


#endif /* NFP6000_PL_H */
