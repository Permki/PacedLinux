/*
 * Copyright (C) 2013, Netronome Systems, Inc.  All rights reserved.
 *
 * Disclaimer: this file is provided without any warranty
 * as part of an early-access program, and the content is
 * bound to change before the final release.
 */

#ifndef NFP3800_SPI_H
#define NFP3800_SPI_H


/* HGID: c3e5162fe280 */

#ifndef NFP3800_LONGNAMES

/*----------------------------------------------------------------
  Register: PhysicalPort[4]Config - Configuration for physical port X
    Bits: 31    Mode(rw) - Port operating mode.
    Bits: 30    Busy(ro) - In automatic mode this bit indicates if the port is busy. In manual mode this bit indicates if the port is actively counting out a burst.
    Bits: 29    SenseHoldn(ro) - This represents the current state of the HOLDn pin assigned to this port (this bit is set if no pin is assigned to this function on this port).
    Bits: 28    SenseClk(ro) - This represents the current state of the CLK pin assigned to this port.
    Bits: 27:24 SenseSelect(ro) - This represents the current state of the SEL pins assigned to this port (each bit is set if no pin is assigned to this function on this port).
    Bits: 23:20 SenseData(ro) - This represents the current state of the D pins assigned to this port (each bit is set if no pin is assigned to this function on this port).
    Bits: 19:16 SelectIdle(rw) - This represents the state of the Select pins assigned to this port when no device is selected.
    Bits: 15:0  Timeout(rw) - This value represents the number of SPI clock periods to count out before declaring a timeout for any reason.
*/
#define NFP3800_SPI_SPIPORTCFG(_x)                           (0x00000000 + (0x4 * ((_x) & 0x3)))
#define   NFP3800_SPI_SPIPORTCFG_MODE                        BIT(31)
#define     NFP3800_SPI_SPIPORTCFG_MODE_AUTOMATIC            (0 << 31)
#define     NFP3800_SPI_SPIPORTCFG_MODE_MANUAL               BIT(31)
#define   NFP3800_SPI_SPIPORTCFG_BUSY                        BIT(30)
#define   NFP3800_SPI_SPIPORTCFG_SENSEHOLDN                  BIT(29)
#define   NFP3800_SPI_SPIPORTCFG_SENSECLK                    BIT(28)
#define   NFP3800_SPI_SPIPORTCFG_SENSESELECT(_x)             (((_x) & 0xf) << 24)
#define   NFP3800_SPI_SPIPORTCFG_SENSESELECT_of(_x)          (((_x) >> 24) & 0xf)
#define   NFP3800_SPI_SPIPORTCFG_SENSEDATA(_x)               (((_x) & 0xf) << 20)
#define   NFP3800_SPI_SPIPORTCFG_SENSEDATA_of(_x)            (((_x) >> 20) & 0xf)
#define   NFP3800_SPI_SPIPORTCFG_SELECTIDLE(_x)              (((_x) & 0xf) << 16)
#define   NFP3800_SPI_SPIPORTCFG_SELECTIDLE_of(_x)           (((_x) >> 16) & 0xf)
#define   NFP3800_SPI_SPIPORTCFG_TIMEOUT(_x)                 (((_x) & 0xffff) << 0)
#define   NFP3800_SPI_SPIPORTCFG_TIMEOUT_of(_x)              (((_x) >> 0) & 0xffff)
/*----------------------------------------------------------------
  Register: PhysicalPort[4]ManualControl - Manual control for physical port X
  Register: BootPxMCR - Boot shadow copy of the appropriate Manual Control Register
    Bits: 31    DataDriveDisable(rw) - Disable the data driver(s). Use this to control data bits that are bi-directional within a SPI access.
    Bits: 29    ClockIdle(rw) - Aka CPOL. This is the state of the SPI clock pin when idle. During Sample this bit is set from the clock IO pad state.
    Bits: 28    ClockPhase(rw) - Aka CPHA. This is the relative phase of the SPI clock edges to the data edges. During Sample this bit is set from the clock IO pad state.
    Bits: 27:24 Select(rw) - This is the value driven to the select bit(s) for this port.
    Bits: 23    HoldTimeout(ro) - When set indicates an overly long active HOLDn is still stalling the port.
    Bits: 22    HoldEnable(rw) - This bit defines whether the SPI state machine allows the SPI_HOLDn input pin to control the flow of SPI transactions to the external targets.
    Bits: 21:20 DataWidth(rw) - This field defines the data transfer width.
    Bits: 19    DataInTrail(rw) - When set, data in is sampled on the trailing SPI clock edge.
    Bits: 18    DataInLead(rw) - When set, data in is sampled on the leading SPI clock edge.
    Bits: 17    DataOutTrail(rw) - When set, data out changes on the trailing SPI clock edge.
    Bits: 16    DataOutLead(rw) - When set, data out changes on the leading SPI clock edge.
    Bits: 15    ClockDisable(rw) - If set, no SPI clock edges are generated for this burst (but the equivalent time is taken).
    Bits: 14:8  ClockEdgeCount(rw) - This value defines the number of SPI half-clocks to generate for this burst.
    Bits:  7:0  ClockDivisor(rw) - This value defines the number of P_CLK ticks (typically 1 nanosecond) per SPI half-clock, i.e. the SPI clock low and high times. Note during power on configuration the clock base is REF_CLOCK (nominally 100 MHz).
*/
#define NFP3800_SPI_SPIPORTMC(_x)                            (0x00000010 + (0x4 * ((_x) & 0x3)))
#define NFP3800_SPI_BOOTPXMCR                                0x00000134
#define   NFP3800_SPI_PHYSICALPORTMANUALCONTROL_DATADRIVEDISABLE BIT(31)
#define   NFP3800_SPI_PHYSICALPORTMANUALCONTROL_CLOCKIDLE    BIT(29)
#define   NFP3800_SPI_PHYSICALPORTMANUALCONTROL_CLOCKPHASE   BIT(28)
#define   NFP3800_SPI_PHYSICALPORTMANUALCONTROL_SELECT(_x)   (((_x) & 0xf) << 24)
#define   NFP3800_SPI_PHYSICALPORTMANUALCONTROL_SELECT_of(_x) (((_x) >> 24) & 0xf)
#define   NFP3800_SPI_PHYSICALPORTMANUALCONTROL_HOLDTIMEOUT  BIT(23)
#define   NFP3800_SPI_PHYSICALPORTMANUALCONTROL_HOLDENABLE   BIT(22)
#define     NFP3800_SPI_PHYSICALPORTMANUALCONTROL_HOLDENABLE_IGNORE (0 << 22)
#define     NFP3800_SPI_PHYSICALPORTMANUALCONTROL_HOLDENABLE_ALLOW BIT(22)
#define   NFP3800_SPI_PHYSICALPORTMANUALCONTROL_DATAWIDTH(_x) (((_x) & 0x3) << 20)
#define   NFP3800_SPI_PHYSICALPORTMANUALCONTROL_DATAWIDTH_of(_x) (((_x) >> 20) & 0x3)
#define     NFP3800_SPI_PHYSICALPORTMANUALCONTROL_DATAWIDTH_ONE (0)
#define     NFP3800_SPI_PHYSICALPORTMANUALCONTROL_DATAWIDTH_DUAL (1)
#define     NFP3800_SPI_PHYSICALPORTMANUALCONTROL_DATAWIDTH_TWO (2)
#define     NFP3800_SPI_PHYSICALPORTMANUALCONTROL_DATAWIDTH_FOUR (3)
#define   NFP3800_SPI_PHYSICALPORTMANUALCONTROL_DATAINTRAIL  BIT(19)
#define   NFP3800_SPI_PHYSICALPORTMANUALCONTROL_DATAINLEAD   BIT(18)
#define   NFP3800_SPI_PHYSICALPORTMANUALCONTROL_DATAOUTTRAIL BIT(17)
#define   NFP3800_SPI_PHYSICALPORTMANUALCONTROL_DATAOUTLEAD  BIT(16)
#define   NFP3800_SPI_PHYSICALPORTMANUALCONTROL_CLOCKDISABLE BIT(15)
#define   NFP3800_SPI_PHYSICALPORTMANUALCONTROL_CLOCKEDGECOUNT(_x) (((_x) & 0x7f) << 8)
#define   NFP3800_SPI_PHYSICALPORTMANUALCONTROL_CLOCKEDGECOUNT_of(_x) (((_x) >> 8) & 0x7f)
#define   NFP3800_SPI_PHYSICALPORTMANUALCONTROL_CLOCKDIVISOR(_x) (((_x) & 0xff) << 0)
#define   NFP3800_SPI_PHYSICALPORTMANUALCONTROL_CLOCKDIVISOR_of(_x) (((_x) >> 0) & 0xff)
/*----------------------------------------------------------------
  Register: PhysicalPort[4]ManualDataOut - Manual data out for physical port X
    Bits: 31:0  DataOut(rw) - This register provides data output from the NFP-3800 to an external device. The MSB (bit 31) is sent first. The number of bits sent is a function of writing the manual control register.
*/
#define NFP3800_SPI_SPIPORTMDO(_x)                           (0x00000020 + (0x4 * ((_x) & 0x3)))
#define   NFP3800_SPI_SPIPORTMDO_DATAOUT(_x)                 (((_x) & 0xffffffff) << 0)
#define   NFP3800_SPI_SPIPORTMDO_DATAOUT_of(_x)              (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: PhysicalPort[4]ManualDataIn - Manual data in for physical port X
    Bits: 31:0  DataIn(ro) - This register provides data input from an external device to the NFP-3800. The data shifts in via the LSB (so bit 0 is the most recently received bit). The number of bits received is a function of writing the manual control register.
*/
#define NFP3800_SPI_SPIPORTMDI(_x)                           (0x00000030 + (0x4 * ((_x) & 0x3)))
#define   NFP3800_SPI_SPIPORTMDI_DATAIN(_x)                  (((_x) & 0xffffffff) << 0)
#define   NFP3800_SPI_SPIPORTMDI_DATAIN_of(_x)               (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: VirtualDevice[16]ConfigA_SerialConfigSpiA - Part of the configuration for virtual device [0-15] as SPI
  Register: BootV00ConfigA_SerialConfigSpiA - Part of the configuration for virtual device [0-15] as SPI
    Bits: 31:26 DataLen(rw) - This field specifies the number of SPI_SCK clock cycles in the data phase of the SPI transaction. The typical value for a single data rate device is 32. For 32-bits of data for a dual data rate device, the value would be 16. For dual data rate devices, only even length number of bits reads/writes are supported. For 32-bits of data for a quad data rate device, the value would be 8. For quad data rate devices, only multiples of four number of bits reads/writes are supported. The legal range is 0 to 32.
    Bits: 25:22 CmdLen(rw) - This field specifies the (number of SPI_SCK clock cycles - 1) in the command phase of the SPI transaction. Typical values range from 2 to 7, representing 3 to 8 command bits. The legal range is from 0 to 15, representing 1 to 16. Note for dual or quad rate command phases this field represents the number of clocks, not the number of command bits.
    Bits: 20:16 AddrLen(rw) - This field provides the ability to select the number of address bits in the address phase of the SPI transaction. Legal values range from 0 to 24, inclusive. Note for dual or quad rate address phases this field represents the number of clocks, not the number of address bits.
    Bits: 15:8  AltReadCmd(rw) - This field can be used to supply an alternate SPI Read command to replace the default read command value (0x3) that would otherwise be used. Note: If the CmdLen field is specified with a value of 8 or greater (indicating a command length of 9 bits or more), this field is combined with the AltWriteCmd field to create a single 16-bit alternative command that is used for both Reads and Writes. If the CmdLen field specifies a SPI command less than 16 bits, the SPI Alternate Command must be right-justified in the 16-bit field (unused bits in the MSBs).
    Bits:  7:0  AltWriteCmd(rw) - This field can be used to supply an alternate SPI Write command to replace the default write command value (0x2) that would otherwise be used. Note: If the CmdLen field is specified with a value of 8 or greater (indicating a command length of 9 bits or more), this field is combined with the AltReadCmd field to create a single 16-bit alternative command that is used for both Reads and Writes. If the CmdLen field specifies a SPI command less than 16 bits, the SPI Alternate Command must be right-justified in the 16-bit field (unused bits in the MSBs).
*/
#define NFP3800_SPI_VirtualDeviceConfigA_SERIALCONFIGSPIA(_x) (0x00000040 + (0x4 * ((_x) & 0xf)))
#define NFP3800_SPI_BootV00ConfigA_SERIALCONFIGSPIA          0x00000120
#define   NFP3800_SPI_VIRTUALDEVICECONFIGA_SERIALCONFIGSPIA_DATALEN(_x) (((_x) & 0x3f) << 26)
#define   NFP3800_SPI_VIRTUALDEVICECONFIGA_SERIALCONFIGSPIA_DATALEN_of(_x) (((_x) >> 26) & 0x3f)
#define   NFP3800_SPI_VIRTUALDEVICECONFIGA_SERIALCONFIGSPIA_CMDLEN(_x) (((_x) & 0xf) << 22)
#define   NFP3800_SPI_VIRTUALDEVICECONFIGA_SERIALCONFIGSPIA_CMDLEN_of(_x) (((_x) >> 22) & 0xf)
#define   NFP3800_SPI_VIRTUALDEVICECONFIGA_SERIALCONFIGSPIA_ADDRLEN(_x) (((_x) & 0x1f) << 16)
#define   NFP3800_SPI_VIRTUALDEVICECONFIGA_SERIALCONFIGSPIA_ADDRLEN_of(_x) (((_x) >> 16) & 0x1f)
#define   NFP3800_SPI_VIRTUALDEVICECONFIGA_SERIALCONFIGSPIA_ALTREADCMD(_x) (((_x) & 0xff) << 8)
#define   NFP3800_SPI_VIRTUALDEVICECONFIGA_SERIALCONFIGSPIA_ALTREADCMD_of(_x) (((_x) >> 8) & 0xff)
#define   NFP3800_SPI_VIRTUALDEVICECONFIGA_SERIALCONFIGSPIA_ALTWRITECMD(_x) (((_x) & 0xff) << 0)
#define   NFP3800_SPI_VIRTUALDEVICECONFIGA_SERIALCONFIGSPIA_ALTWRITECMD_of(_x) (((_x) >> 0) & 0xff)
/*----------------------------------------------------------------
  Register: VirtualDevice[16]ConfigA_SerialConfigMdioA - Part of the configuration for virtual device [0-15] as MDIO
  Register: BootV00ConfigA_SerialConfigMdioA - Part of the configuration for virtual device [0-15] as MDIO
    Bits: 31:26 DataLen(rw) - This field specifies the number of bits in the Data phase of the MDIO management frame. Typical value is 16. The legal range is 0 to 32.
    Bits: 25:24 TALen(rw) - This field specifies the number of bits in the Turn-around phase of the MDIO management frame. Legal range is from 0 to 3. Note, however, that the currently legal value per IEEE 802.3, clauses 22 and 45 is 2. This field is supplied to handle either new definitions for MDIO or another serial IO standard
    Bits: 20:16 HdrLen(rw) - This field specifies the number of bits in the Header of the MDIO management frame. The header is considered to be the bits starting with the Start-of-Frame bits up to but not including the Turn-around bits. Legal values range from 0 to 24, inclusive. Note, however, that the MDIO standard, IEEE 802.3, clauses 22 and 45, to which this interface is designed, specifies this range as 14 bits. The expanded range allowed in this implementation is designed to handle as yet unspecified MDIO capabilities, and possible additional serial IO standards.
    Bits: 15:8  HeaderMid(rw) - This field is used to specify the actual MDIO management frame header bits to be sent to the MDIO device on the Serial Device. The MSB of the header is the first start-of-frame bit sent to the MDIO device. The LSB of the header is the last bit of the REGADR (clause 22) or DEVTYPE (clause 45) sent to the MDIO device.
    Bits:  7:0  HeaderLow(rw) - This field is used to specify the actual MDIO management frame header bits to be sent to the MDIO device on the Serial Device. The MSB of the header is the first start-of-frame bit sent to the MDIO device. The LSB of the header is the last bit of the REGADR (clause 22) or DEVTYPE (clause 45) sent to the MDIO device.
*/
#define NFP3800_SPI_VirtualDeviceConfigA_SERIALCONFIGMDIOA(_x) (0x00000040 + (0x4 * ((_x) & 0xf)))
#define NFP3800_SPI_BootV00ConfigA_SERIALCONFIGMDIOA         0x00000120
#define   NFP3800_SPI_VIRTUALDEVICECONFIGA_SERIALCONFIGMDIOA_DATALEN(_x) (((_x) & 0x3f) << 26)
#define   NFP3800_SPI_VIRTUALDEVICECONFIGA_SERIALCONFIGMDIOA_DATALEN_of(_x) (((_x) >> 26) & 0x3f)
#define   NFP3800_SPI_VIRTUALDEVICECONFIGA_SERIALCONFIGMDIOA_TALEN(_x) (((_x) & 0x3) << 24)
#define   NFP3800_SPI_VIRTUALDEVICECONFIGA_SERIALCONFIGMDIOA_TALEN_of(_x) (((_x) >> 24) & 0x3)
#define   NFP3800_SPI_VIRTUALDEVICECONFIGA_SERIALCONFIGMDIOA_HDRLEN(_x) (((_x) & 0x1f) << 16)
#define   NFP3800_SPI_VIRTUALDEVICECONFIGA_SERIALCONFIGMDIOA_HDRLEN_of(_x) (((_x) >> 16) & 0x1f)
#define   NFP3800_SPI_VIRTUALDEVICECONFIGA_SERIALCONFIGMDIOA_HDRHI(_x) (((_x) & 0xff) << 8)
#define   NFP3800_SPI_VIRTUALDEVICECONFIGA_SERIALCONFIGMDIOA_HDRHI_of(_x) (((_x) >> 8) & 0xff)
#define   NFP3800_SPI_VIRTUALDEVICECONFIGA_SERIALCONFIGMDIOA_HDRLO(_x) (((_x) & 0xff) << 0)
#define   NFP3800_SPI_VIRTUALDEVICECONFIGA_SERIALCONFIGMDIOA_HDRLO_of(_x) (((_x) >> 0) & 0xff)
/*----------------------------------------------------------------
  Register: VirtualDevice[16]ConfigB_SerialConfigSpiB - Part of the configuration for virtual device [0-15] as SPI
  Register: BootV00ConfigB_SerialConfigSpiB - Part of the configuration for virtual device [0-15] as SPI
    Bits: 31    MDIOSel(rw) - This bit chooses between treating the Serial Device as a SPI device or a MDIO device. Note that in typical usage physical ports 0 and 1 have this bit as 0; physical ports 2 and 3 have this bit as 1
    Bits: 30    AltReadEn(rw) - This bit enables the use of the AltReadCmd specified in the SerialConfigSpiA register for this configuration. Note: If using the combined SPI alternate command (AltReadCmd and AltWriteCmd) fields, due to CmdLen indicating the SPI command is 9 bits or greater, this bit enables the use of the combined SPI Alternate Command field only for commands entered as SSB reads.
    Bits: 29    AltWriteEn(rw) - This bit enables the use of the AltWriteCmd specified in the SerialConfigSpiA Register for this configuration. Note: If using the combined SPI alternate command (AltReadCmd and AltWriteCmd) fields, due to CmdLen indicating the SPI command is 9 bits or greater, this bit enables the use of the combined SPI Alternate Command field only for commands entered as SSB writes.
    Bits: 28:24 CSDelay(rw) - This field specifies the number of 20 nanosecond periods to wait between de-assertion and assertion of the SPI Chip Select, presuming a P_CLK period of 1 nanosecond.
    Bits: 23:22 Address(rw) - This field specifies the two MSB of the 24 bit address.
    Bits: 21    Busy(rw) - This bit can be set to '1' by the user when setting up a transaction for the SPI device. If the user does not set it, the Serial Device Controller will set it when it starts executing the command. In either case, when the Serial Device Controller completes the command and the SPI device is ready to accept a new command, the Serial Device Controller will clear this bit to '0'.
    Bits: 20    BootPromWriteEn(rw) - This bit provides some protection against software bugs accidentally causing a write to the SPI Serial Boot PROM space that overwrites good data. An attempt to write to a virtual SPI device without first setting this bit will be ignored.
    Bits: 19:18 Mode(rw) - Select the mode of operation for the SPI bus. During Sample the state of the SPI[0/1]_CLK (for even/odd numbered virtual ports) is used to select Mode = 0 or Mode = 3.
    Bits: 17:16 ReadMode(rw) - SPI mode to use for reading
    Bits: 15    HoldEn(rw) - This bit defines whether the SPI state machine allows the SPI_HOLDn input pin to control the flow of SPI transactions to the external targets.
    Bits: 14    HoldTimeout(ro) - When set indicates an overly long active HOLDn is still stalling the port.
    Bits: 13:8  DummyLen(rw) - This value specifies the number of Dummy bits to be sent in the SPI frame. Dummy bits are always sent after the command and address bits have been sent. Legal values range from 0 to 32.
    Bits:  7:0  Divider(rw) - The value provided here is the number of P_CLK clocks per SPI clock low period and per SPI clock high period.
*/
#define NFP3800_SPI_VirtualDeviceConfigB_SERIALCONFIGSPIB(_x) (0x00000080 + (0x4 * ((_x) & 0xf)))
#define NFP3800_SPI_BootV00ConfigB_SERIALCONFIGSPIB          0x00000124
#define   NFP3800_SPI_VIRTUALDEVICECONFIGB_SERIALCONFIGSPIB_SELECT BIT(31)
#define     NFP3800_SPI_VIRTUALDEVICECONFIGB_SERIALCONFIGSPIB_SELECT_SPI (0 << 31)
#define     NFP3800_SPI_VIRTUALDEVICECONFIGB_SERIALCONFIGSPIB_SELECT_MDIO BIT(31)
#define   NFP3800_SPI_VIRTUALDEVICECONFIGB_SERIALCONFIGSPIB_ALTREAD BIT(30)
#define     NFP3800_SPI_VIRTUALDEVICECONFIGB_SERIALCONFIGSPIB_ALTREAD_DISABLED (0 << 30)
#define     NFP3800_SPI_VIRTUALDEVICECONFIGB_SERIALCONFIGSPIB_ALTREAD_ENABLED BIT(30)
#define   NFP3800_SPI_VIRTUALDEVICECONFIGB_SERIALCONFIGSPIB_ALTWRITE BIT(29)
#define     NFP3800_SPI_VIRTUALDEVICECONFIGB_SERIALCONFIGSPIB_ALTWRITE_DISABLED (0 << 29)
#define     NFP3800_SPI_VIRTUALDEVICECONFIGB_SERIALCONFIGSPIB_ALTWRITE_ENABLED BIT(29)
#define   NFP3800_SPI_VIRTUALDEVICECONFIGB_SERIALCONFIGSPIB_ENABLEDELAY(_x) (((_x) & 0x1f) << 24)
#define   NFP3800_SPI_VIRTUALDEVICECONFIGB_SERIALCONFIGSPIB_ENABLEDELAY_of(_x) (((_x) >> 24) & 0x1f)
#define   NFP3800_SPI_VIRTUALDEVICECONFIGB_SERIALCONFIGSPIB_ADDRTRANSBITS(_x) (((_x) & 0x3) << 22)
#define   NFP3800_SPI_VIRTUALDEVICECONFIGB_SERIALCONFIGSPIB_ADDRTRANSBITS_of(_x) (((_x) >> 22) & 0x3)
#define   NFP3800_SPI_VIRTUALDEVICECONFIGB_SERIALCONFIGSPIB_BUSY BIT(21)
#define     NFP3800_SPI_VIRTUALDEVICECONFIGB_SERIALCONFIGSPIB_BUSY_NOT_BUSY (0 << 21)
#define     NFP3800_SPI_VIRTUALDEVICECONFIGB_SERIALCONFIGSPIB_BUSY_BUSY BIT(21)
#define   NFP3800_SPI_VIRTUALDEVICECONFIGB_SERIALCONFIGSPIB_BOOTPROMWRITEEN BIT(20)
#define     NFP3800_SPI_VIRTUALDEVICECONFIGB_SERIALCONFIGSPIB_BOOTPROMWRITEEN_DISCARD (0 << 20)
#define     NFP3800_SPI_VIRTUALDEVICECONFIGB_SERIALCONFIGSPIB_BOOTPROMWRITEEN_ENABLE BIT(20)
#define   NFP3800_SPI_VIRTUALDEVICECONFIGB_SERIALCONFIGSPIB_MODESELECT(_x) (((_x) & 0x3) << 18)
#define   NFP3800_SPI_VIRTUALDEVICECONFIGB_SERIALCONFIGSPIB_MODESELECT_of(_x) (((_x) >> 18) & 0x3)
#define     NFP3800_SPI_VIRTUALDEVICECONFIGB_SERIALCONFIGSPIB_MODESELECT_FIRST_EDGE_CLK0 (0)
#define     NFP3800_SPI_VIRTUALDEVICECONFIGB_SERIALCONFIGSPIB_MODESELECT_SECOND_EDGE_CLK0 (1)
#define     NFP3800_SPI_VIRTUALDEVICECONFIGB_SERIALCONFIGSPIB_MODESELECT_FIRST_EDGE_CLK1 (2)
#define     NFP3800_SPI_VIRTUALDEVICECONFIGB_SERIALCONFIGSPIB_MODESELECT_SECOND_EDGE_CLK1 (3)
#define   NFP3800_SPI_VIRTUALDEVICECONFIGB_SERIALCONFIGSPIB_READMODE(_x) (((_x) & 0x3) << 16)
#define   NFP3800_SPI_VIRTUALDEVICECONFIGB_SERIALCONFIGSPIB_READMODE_of(_x) (((_x) >> 16) & 0x3)
#define     NFP3800_SPI_VIRTUALDEVICECONFIGB_SERIALCONFIGSPIB_READMODE_SLOW (0)
#define     NFP3800_SPI_VIRTUALDEVICECONFIGB_SERIALCONFIGSPIB_READMODE_FAST_SINGLE (1)
#define     NFP3800_SPI_VIRTUALDEVICECONFIGB_SERIALCONFIGSPIB_READMODE_FAST_DUAL (3)
#define   NFP3800_SPI_VIRTUALDEVICECONFIGB_SERIALCONFIGSPIB_HOLD BIT(15)
#define     NFP3800_SPI_VIRTUALDEVICECONFIGB_SERIALCONFIGSPIB_HOLD_IGNORE (0 << 15)
#define     NFP3800_SPI_VIRTUALDEVICECONFIGB_SERIALCONFIGSPIB_HOLD_ALLOW BIT(15)
#define   NFP3800_SPI_VIRTUALDEVICECONFIGB_SERIALCONFIGSPIB_HOLDTIMEOUT BIT(14)
#define   NFP3800_SPI_VIRTUALDEVICECONFIGB_SERIALCONFIGSPIB_DUMMYLEN(_x) (((_x) & 0x3f) << 8)
#define   NFP3800_SPI_VIRTUALDEVICECONFIGB_SERIALCONFIGSPIB_DUMMYLEN_of(_x) (((_x) >> 8) & 0x3f)
#define   NFP3800_SPI_VIRTUALDEVICECONFIGB_SERIALCONFIGSPIB_PCLKDIV(_x) (((_x) & 0xff) << 0)
#define   NFP3800_SPI_VIRTUALDEVICECONFIGB_SERIALCONFIGSPIB_PCLKDIV_of(_x) (((_x) >> 0) & 0xff)
/*----------------------------------------------------------------
  Register: VirtualDevice[16]ConfigB_SerialConfigMdioB - Part of the configuration for virtual device [0-15] as MDIO
  Register: BootV00ConfigB_SerialConfigMdioB - Part of the configuration for virtual device [0-15] as MDIO
    Bits: 31    MDIOSel(rw) - This bit chooses between treating the Serial Device as a SPI device or a MDIO device.
    Bits: 30    PreambleEn(rw) - This bit enables the use of the 32-bit Preamble in the MDIO management frame.
    Bits: 26:24 TAData(rw) - This field specifies the actual turn-around bits to be sent to the MDIO device on a MDIO Write. In the case of a MDIO Read, the MDIO data bit output enable is disabled, and will be pulled up by the external pull-up resistor.
    Bits: 23    DataOutLead(rw) - This bit controls the active clock edge for data output.
    Bits: 22    DataInLead(rw) - This bit controls the active clock edge for data input.
    Bits: 21    Busy(rw) - This bit can be set to '1' by the user when setting up a transaction for the SPI device. If the user does not set it, the Serial Device Controller will set it when it starts executing the command. In either case, when the Serial Device Controller completes the command and the SPI device is ready to accept a new command, the Serial Device Controller will clear this bit to '0'.
    Bits: 19:12 HeaderHigh(rw) - This field is used to specify the actual MDIO management frame header bits [23:16] to be sent to the MDIO device. The MSB of the header is the first start-of-frame bit sent to the MDIO device. The LSB of the header is the last bit of the REGADR (clause 22) or DEVTYPE (clause 45) sent to the MDIO device. The bits of the MDIO header which are actually used are right-justified inside the overall 24-bit header field. That is bit 0 of the actual header field goes at bit 0 of the overall 24-bit header provisioned in these registers. Unused bits are ignored.
    Bits:  9:0  Divider(rw) - The value provided here is the number of P_CLK clocks per MDIO clock low period and per MDIO clock high period.
*/
#define NFP3800_SPI_VirtualDeviceConfigB_SERIALCONFIGMDIOB(_x) (0x00000080 + (0x4 * ((_x) & 0xf)))
#define NFP3800_SPI_BootV00ConfigB_SERIALCONFIGMDIOB         0x00000124
#define   NFP3800_SPI_VIRTUALDEVICECONFIGB_SERIALCONFIGMDIOB_SELECT BIT(31)
#define     NFP3800_SPI_VIRTUALDEVICECONFIGB_SERIALCONFIGMDIOB_SELECT_SPI (0 << 31)
#define     NFP3800_SPI_VIRTUALDEVICECONFIGB_SERIALCONFIGMDIOB_SELECT_MDIO BIT(31)
#define   NFP3800_SPI_VIRTUALDEVICECONFIGB_SERIALCONFIGMDIOB_PREAMBLE BIT(30)
#define     NFP3800_SPI_VIRTUALDEVICECONFIGB_SERIALCONFIGMDIOB_PREAMBLE_DISABLE (0 << 30)
#define     NFP3800_SPI_VIRTUALDEVICECONFIGB_SERIALCONFIGMDIOB_PREAMBLE_ENABLE BIT(30)
#define   NFP3800_SPI_VIRTUALDEVICECONFIGB_SERIALCONFIGMDIOB_TABITS(_x) (((_x) & 0x7) << 24)
#define   NFP3800_SPI_VIRTUALDEVICECONFIGB_SERIALCONFIGMDIOB_TABITS_of(_x) (((_x) >> 24) & 0x7)
#define   NFP3800_SPI_VIRTUALDEVICECONFIGB_SERIALCONFIGMDIOB_DATAOUTLEAD BIT(23)
#define     NFP3800_SPI_VIRTUALDEVICECONFIGB_SERIALCONFIGMDIOB_DATAOUTLEAD_TRAIL (0 << 23)
#define     NFP3800_SPI_VIRTUALDEVICECONFIGB_SERIALCONFIGMDIOB_DATAOUTLEAD_LEAD BIT(23)
#define   NFP3800_SPI_VIRTUALDEVICECONFIGB_SERIALCONFIGMDIOB_DATAINLEAD BIT(22)
#define     NFP3800_SPI_VIRTUALDEVICECONFIGB_SERIALCONFIGMDIOB_DATAINLEAD_TRAIL (0 << 22)
#define     NFP3800_SPI_VIRTUALDEVICECONFIGB_SERIALCONFIGMDIOB_DATAINLEAD_LEAD BIT(22)
#define   NFP3800_SPI_VIRTUALDEVICECONFIGB_SERIALCONFIGMDIOB_BUSY BIT(21)
#define     NFP3800_SPI_VIRTUALDEVICECONFIGB_SERIALCONFIGMDIOB_BUSY_NOT_BUSY (0 << 21)
#define     NFP3800_SPI_VIRTUALDEVICECONFIGB_SERIALCONFIGMDIOB_BUSY_BUSY BIT(21)
#define   NFP3800_SPI_VIRTUALDEVICECONFIGB_SERIALCONFIGMDIOB_HDR2316(_x) (((_x) & 0xff) << 12)
#define   NFP3800_SPI_VIRTUALDEVICECONFIGB_SERIALCONFIGMDIOB_HDR2316_of(_x) (((_x) >> 12) & 0xff)
#define   NFP3800_SPI_VIRTUALDEVICECONFIGB_SERIALCONFIGMDIOB_DEVPCLKDIV(_x) (((_x) & 0x3ff) << 0)
#define   NFP3800_SPI_VIRTUALDEVICECONFIGB_SERIALCONFIGMDIOB_DEVPCLKDIV_of(_x) (((_x) >> 0) & 0x3ff)
/*----------------------------------------------------------------
  Register: VirtualDevice[16]ConfigC_SerialConfigSpiC - Part of the configuration for virtual device [0-15] as SPI
  Register: BootV00ConfigC_SerialConfigSpiC - Part of the configuration for virtual device [0-15] as SPI
    Bits: 31:30 ClockDelay(rw) - This field controls the delay of the clock sent out the IO pad.
    Bits: 29:28 DataWidthDat(rw) - Specify the data width during the data phase.
    Bits: 27:26 DataWidthAdr(rw) - Specify the data width during the address phase.
    Bits: 25:24 DataWidthCmd(rw) - Specify the data width during the command phase.
    Bits: 23:20 DataOutDelay(rw) - Specify the number of P_CLK ticks to delay the output data relative to the SPI clock. A value of 0 corresponds to the data edge being lined up with the clock edge.
    Bits: 19:16 DataInDelay(rw) - Specify the number of P_CLK ticks to delay the sampling of input data relative to the SPI clock. A value of 3 corresponds to the data edge being lined up with the clock edge.
    Bits: 15:14 DataOutSyncMode(rw) - This field controls the synchronous and asynchronous modes of the data outputs.
    Bits: 13:12 DataInSyncMode(rw) - This field controls the synchronous and asynchronous modes of the data inputs.
    Bits: 11:8  Select(rw) - This field specifies the value to put out on the select pins of the physical port associated with this virtual device.
    Bits:  7    FastRelinqEn(rw) - When this bit is set and this virtual device has a burst in progress and another virtual device wants the same port, this device will immediately terminate the burst and release the port.
    Bits:  6    IgnoreBusyEn(rw) - When this bit is set writes to this virtual port's config A, B, C registers are permitted even when the port is busy. A write with this bit set is permitted even if the registered value does not permit the write.
    Bits:  5    Sequential(rw) - When this bit is set and DataLen multiplied by the effective data width is 32 bits, the address used must be sequential to continue a burst read transaction. If the address is not sequential, the current burst is terminated and the new access performs the full command, address, dummy, data transfer.
    Bits:  4    PrefetchEnable(rw) - If BurstEnable is 1, this bit controls if the SPI hardware will attempt to prefetch read data during a burst read transaction. Only up to one DataLen burst will be prefetched.
    Bits:  3    TerminateBurst(w1c) - If a transaction burst is in progress, writing a one to this bit will terminate the burst. Termination will stop any prefetch in progress, discard any prefetched data, deselect the device, and clear the Busy bit.
    Bits:  2    BurstEnable(rw) - This bit enables burst transactions, whereby the hardware keeps the device selected for multiple sequential accesses.
    Bits:  1:0  PhysicalPort(rw) - This field specifies the physical port associated with this virtual device. After reset all even virtual devices are set to physical port 0, all odd virtual devices are set to physical port 1.
*/
#define NFP3800_SPI_VirtualDeviceConfigC_SERIALCONFIGSPIC(_x) (0x000000c0 + (0x4 * ((_x) & 0xf)))
#define NFP3800_SPI_BootV00ConfigC_SERIALCONFIGSPIC          0x00000128
#define   NFP3800_SPI_VIRTUALDEVICECONFIGC_SERIALCONFIGSPIC_CLOCKDELAY(_x) (((_x) & 0x3) << 30)
#define   NFP3800_SPI_VIRTUALDEVICECONFIGC_SERIALCONFIGSPIC_CLOCKDELAY_of(_x) (((_x) >> 30) & 0x3)
#define     NFP3800_SPI_VIRTUALDEVICECONFIGC_SERIALCONFIGSPIC_CLOCKDELAY_DELAY_0 (0)
#define     NFP3800_SPI_VIRTUALDEVICECONFIGC_SERIALCONFIGSPIC_CLOCKDELAY_DELAY_8 (1)
#define     NFP3800_SPI_VIRTUALDEVICECONFIGC_SERIALCONFIGSPIC_CLOCKDELAY_DELAY_16 (2)
#define     NFP3800_SPI_VIRTUALDEVICECONFIGC_SERIALCONFIGSPIC_CLOCKDELAY_DELAY_24 (3)
#define   NFP3800_SPI_VIRTUALDEVICECONFIGC_SERIALCONFIGSPIC_DATAWIDTHDAT(_x) (((_x) & 0x3) << 28)
#define   NFP3800_SPI_VIRTUALDEVICECONFIGC_SERIALCONFIGSPIC_DATAWIDTHDAT_of(_x) (((_x) >> 28) & 0x3)
#define     NFP3800_SPI_VIRTUALDEVICECONFIGC_SERIALCONFIGSPIC_DATAWIDTHDAT_WIDTH_IS_1 (0)
#define     NFP3800_SPI_VIRTUALDEVICECONFIGC_SERIALCONFIGSPIC_DATAWIDTHDAT_WIDTH_IS_2 (2)
#define     NFP3800_SPI_VIRTUALDEVICECONFIGC_SERIALCONFIGSPIC_DATAWIDTHDAT_WIDTH_IS_4 (3)
#define   NFP3800_SPI_VIRTUALDEVICECONFIGC_SERIALCONFIGSPIC_DATAWIDTHADR(_x) (((_x) & 0x3) << 26)
#define   NFP3800_SPI_VIRTUALDEVICECONFIGC_SERIALCONFIGSPIC_DATAWIDTHADR_of(_x) (((_x) >> 26) & 0x3)
#define     NFP3800_SPI_VIRTUALDEVICECONFIGC_SERIALCONFIGSPIC_DATAWIDTHADR_WIDTH_IS_1 (0)
#define     NFP3800_SPI_VIRTUALDEVICECONFIGC_SERIALCONFIGSPIC_DATAWIDTHADR_WIDTH_IS_2 (2)
#define     NFP3800_SPI_VIRTUALDEVICECONFIGC_SERIALCONFIGSPIC_DATAWIDTHADR_WIDTH_IS_4 (3)
#define   NFP3800_SPI_VIRTUALDEVICECONFIGC_SERIALCONFIGSPIC_DATAWIDTHCMD(_x) (((_x) & 0x3) << 24)
#define   NFP3800_SPI_VIRTUALDEVICECONFIGC_SERIALCONFIGSPIC_DATAWIDTHCMD_of(_x) (((_x) >> 24) & 0x3)
#define     NFP3800_SPI_VIRTUALDEVICECONFIGC_SERIALCONFIGSPIC_DATAWIDTHCMD_WIDTH_IS_1 (0)
#define     NFP3800_SPI_VIRTUALDEVICECONFIGC_SERIALCONFIGSPIC_DATAWIDTHCMD_WIDTH_IS_2 (2)
#define     NFP3800_SPI_VIRTUALDEVICECONFIGC_SERIALCONFIGSPIC_DATAWIDTHCMD_WIDTH_IS_4 (3)
#define   NFP3800_SPI_VIRTUALDEVICECONFIGC_SERIALCONFIGSPIC_DATAOUTDELAY(_x) (((_x) & 0xf) << 20)
#define   NFP3800_SPI_VIRTUALDEVICECONFIGC_SERIALCONFIGSPIC_DATAOUTDELAY_of(_x) (((_x) >> 20) & 0xf)
#define   NFP3800_SPI_VIRTUALDEVICECONFIGC_SERIALCONFIGSPIC_DATAINDELAY(_x) (((_x) & 0xf) << 16)
#define   NFP3800_SPI_VIRTUALDEVICECONFIGC_SERIALCONFIGSPIC_DATAINDELAY_of(_x) (((_x) >> 16) & 0xf)
#define   NFP3800_SPI_VIRTUALDEVICECONFIGC_SERIALCONFIGSPIC_DATAOUTSYNCMODE(_x) (((_x) & 0x3) << 14)
#define   NFP3800_SPI_VIRTUALDEVICECONFIGC_SERIALCONFIGSPIC_DATAOUTSYNCMODE_of(_x) (((_x) >> 14) & 0x3)
#define     NFP3800_SPI_VIRTUALDEVICECONFIGC_SERIALCONFIGSPIC_DATAOUTSYNCMODE_SYNC (0)
#define     NFP3800_SPI_VIRTUALDEVICECONFIGC_SERIALCONFIGSPIC_DATAOUTSYNCMODE_RISE (1)
#define     NFP3800_SPI_VIRTUALDEVICECONFIGC_SERIALCONFIGSPIC_DATAOUTSYNCMODE_FALL (2)
#define     NFP3800_SPI_VIRTUALDEVICECONFIGC_SERIALCONFIGSPIC_DATAOUTSYNCMODE_DDR (3)
#define   NFP3800_SPI_VIRTUALDEVICECONFIGC_SERIALCONFIGSPIC_DATAINSYNCMODE(_x) (((_x) & 0x3) << 12)
#define   NFP3800_SPI_VIRTUALDEVICECONFIGC_SERIALCONFIGSPIC_DATAINSYNCMODE_of(_x) (((_x) >> 12) & 0x3)
#define     NFP3800_SPI_VIRTUALDEVICECONFIGC_SERIALCONFIGSPIC_DATAINSYNCMODE_SYNC (0)
#define     NFP3800_SPI_VIRTUALDEVICECONFIGC_SERIALCONFIGSPIC_DATAINSYNCMODE_RISE (1)
#define     NFP3800_SPI_VIRTUALDEVICECONFIGC_SERIALCONFIGSPIC_DATAINSYNCMODE_FALL (2)
#define     NFP3800_SPI_VIRTUALDEVICECONFIGC_SERIALCONFIGSPIC_DATAINSYNCMODE_DDR (3)
#define   NFP3800_SPI_VIRTUALDEVICECONFIGC_SERIALCONFIGSPIC_SELECT(_x) (((_x) & 0xf) << 8)
#define   NFP3800_SPI_VIRTUALDEVICECONFIGC_SERIALCONFIGSPIC_SELECT_of(_x) (((_x) >> 8) & 0xf)
#define   NFP3800_SPI_VIRTUALDEVICECONFIGC_SERIALCONFIGSPIC_FASTRELINQEN BIT(7)
#define   NFP3800_SPI_VIRTUALDEVICECONFIGC_SERIALCONFIGSPIC_IGNOREBUSYEN BIT(6)
#define   NFP3800_SPI_VIRTUALDEVICECONFIGC_SERIALCONFIGSPIC_SEQUENTIAL BIT(5)
#define   NFP3800_SPI_VIRTUALDEVICECONFIGC_SERIALCONFIGSPIC_PREFETCHENABLE BIT(4)
#define   NFP3800_SPI_VIRTUALDEVICECONFIGC_SERIALCONFIGSPIC_TERMINATEBURST BIT(3)
#define   NFP3800_SPI_VIRTUALDEVICECONFIGC_SERIALCONFIGSPIC_BURSTENABLE BIT(2)
#define   NFP3800_SPI_VIRTUALDEVICECONFIGC_SERIALCONFIGSPIC_PHYSICALPORT(_x) (((_x) & 0x3) << 0)
#define   NFP3800_SPI_VIRTUALDEVICECONFIGC_SERIALCONFIGSPIC_PHYSICALPORT_of(_x) (((_x) >> 0) & 0x3)
/*----------------------------------------------------------------
  Register: VirtualDevice[16]ConfigC_SerialConfigMdioC - Part of the configuration for virtual device [0-15] as MDIO
  Register: BootV00ConfigC_SerialConfigMdioC - Part of the configuration for virtual device [0-15] as MDIO
    Bits: 31:30 ClockDelay(rw) - This field controls the delay of the clock sent out the IO pad.
    Bits: 23:20 DataOutDelay(rw) - Specify the number of P_CLK ticks to delay the output data relative to the SPI clock. A value of 0 corresponds to the data edge being lined up with the clock edge.
    Bits: 19:16 DataInDelay(rw) - Specify the number of P_CLK ticks to delay the sampling of input data relative to the SPI clock. A value of 3 corresponds to the data edge being lined up with the clock edge.
    Bits: 15:14 DataOutSyncMode(rw) - This field controls the synchronous and asynchronous modes of the data outputs.
    Bits: 13:12 DataInSyncMode(rw) - This field controls the synchronous and asynchronous modes of the data inputs.
    Bits:  1:0  PhysicalPort(rw) - This field specifies the physical port associated with this virtual device.
*/
#define NFP3800_SPI_VirtualDeviceConfigC_SERIALCONFIGMDIOC(_x) (0x000000c0 + (0x4 * ((_x) & 0xf)))
#define NFP3800_SPI_BootV00ConfigC_SERIALCONFIGMDIOC         0x00000128
#define   NFP3800_SPI_VIRTUALDEVICECONFIGC_SERIALCONFIGMDIOC_CLOCKDELAY(_x) (((_x) & 0x3) << 30)
#define   NFP3800_SPI_VIRTUALDEVICECONFIGC_SERIALCONFIGMDIOC_CLOCKDELAY_of(_x) (((_x) >> 30) & 0x3)
#define     NFP3800_SPI_VIRTUALDEVICECONFIGC_SERIALCONFIGMDIOC_CLOCKDELAY_DELAY_0 (0)
#define     NFP3800_SPI_VIRTUALDEVICECONFIGC_SERIALCONFIGMDIOC_CLOCKDELAY_DELAY_8 (1)
#define     NFP3800_SPI_VIRTUALDEVICECONFIGC_SERIALCONFIGMDIOC_CLOCKDELAY_DELAY_16 (2)
#define     NFP3800_SPI_VIRTUALDEVICECONFIGC_SERIALCONFIGMDIOC_CLOCKDELAY_DELAY_24 (3)
#define   NFP3800_SPI_VIRTUALDEVICECONFIGC_SERIALCONFIGMDIOC_DATAOUTDELAY(_x) (((_x) & 0xf) << 20)
#define   NFP3800_SPI_VIRTUALDEVICECONFIGC_SERIALCONFIGMDIOC_DATAOUTDELAY_of(_x) (((_x) >> 20) & 0xf)
#define   NFP3800_SPI_VIRTUALDEVICECONFIGC_SERIALCONFIGMDIOC_DATAINDELAY(_x) (((_x) & 0xf) << 16)
#define   NFP3800_SPI_VIRTUALDEVICECONFIGC_SERIALCONFIGMDIOC_DATAINDELAY_of(_x) (((_x) >> 16) & 0xf)
#define   NFP3800_SPI_VIRTUALDEVICECONFIGC_SERIALCONFIGMDIOC_DATAOUTSYNCMODE(_x) (((_x) & 0x3) << 14)
#define   NFP3800_SPI_VIRTUALDEVICECONFIGC_SERIALCONFIGMDIOC_DATAOUTSYNCMODE_of(_x) (((_x) >> 14) & 0x3)
#define     NFP3800_SPI_VIRTUALDEVICECONFIGC_SERIALCONFIGMDIOC_DATAOUTSYNCMODE_SYNC (0)
#define     NFP3800_SPI_VIRTUALDEVICECONFIGC_SERIALCONFIGMDIOC_DATAOUTSYNCMODE_RISE (1)
#define     NFP3800_SPI_VIRTUALDEVICECONFIGC_SERIALCONFIGMDIOC_DATAOUTSYNCMODE_FALL (2)
#define     NFP3800_SPI_VIRTUALDEVICECONFIGC_SERIALCONFIGMDIOC_DATAOUTSYNCMODE_DDR (3)
#define   NFP3800_SPI_VIRTUALDEVICECONFIGC_SERIALCONFIGMDIOC_DATAINSYNCMODE(_x) (((_x) & 0x3) << 12)
#define   NFP3800_SPI_VIRTUALDEVICECONFIGC_SERIALCONFIGMDIOC_DATAINSYNCMODE_of(_x) (((_x) >> 12) & 0x3)
#define     NFP3800_SPI_VIRTUALDEVICECONFIGC_SERIALCONFIGMDIOC_DATAINSYNCMODE_SYNC (0)
#define     NFP3800_SPI_VIRTUALDEVICECONFIGC_SERIALCONFIGMDIOC_DATAINSYNCMODE_RISE (1)
#define     NFP3800_SPI_VIRTUALDEVICECONFIGC_SERIALCONFIGMDIOC_DATAINSYNCMODE_FALL (2)
#define     NFP3800_SPI_VIRTUALDEVICECONFIGC_SERIALCONFIGMDIOC_DATAINSYNCMODE_DDR (3)
#define   NFP3800_SPI_VIRTUALDEVICECONFIGC_SERIALCONFIGMDIOC_PHYSICALPORT(_x) (((_x) & 0x3) << 0)
#define   NFP3800_SPI_VIRTUALDEVICECONFIGC_SERIALCONFIGMDIOC_PHYSICALPORT_of(_x) (((_x) >> 0) & 0x3)
/*----------------------------------------------------------------
  Register: SpiIOConfig - IO pad configuration
  Register: BootIOConfig - Boot shadow copy of the IO pad configuration
    Bits:  9:8  SPIXDAT23(rw) - This field controls which port the SPIXDAT[2-3] pins are associated with.
    Bits:  7:6  SPIXHOLD(rw) - This field controls which port the SPIXHOLDn pin is associated with.
    Bits:  5:4  SPIXSEL3(rw) - This field controls which port the SPIXSEL3 pin is associated with.
    Bits:  3:2  SPIXSEl2(rw) - This field controls which port the SPIXSEL2 pin is associated with.
    Bits:  1:0  SPIXSEL1(rw) - This field controls which port the SPIXSEL1 pin is associated with.
*/
#define NFP3800_SPI_SPIIOCONFIG                              0x00000100
#define NFP3800_SPI_BOOTIOCONFIG                             0x0000012c
#define   NFP3800_SPI_SERIALIOCONFIG_SPIXDAT23(_x)           (((_x) & 0x3) << 8)
#define   NFP3800_SPI_SERIALIOCONFIG_SPIXDAT23_of(_x)        (((_x) >> 8) & 0x3)
#define     NFP3800_SPI_SERIALIOCONFIG_SPIXDAT23_PORT0DAT23  (0)
#define     NFP3800_SPI_SERIALIOCONFIG_SPIXDAT23_PORT1DAT23  (1)
#define     NFP3800_SPI_SERIALIOCONFIG_SPIXDAT23_PORT2DAT23  (2)
#define     NFP3800_SPI_SERIALIOCONFIG_SPIXDAT23_PORT3DAT23  (3)
#define   NFP3800_SPI_SERIALIOCONFIG_SPIXHOLD(_x)            (((_x) & 0x3) << 6)
#define   NFP3800_SPI_SERIALIOCONFIG_SPIXHOLD_of(_x)         (((_x) >> 6) & 0x3)
#define     NFP3800_SPI_SERIALIOCONFIG_SPIXHOLD_PORT0HOLD    (0)
#define     NFP3800_SPI_SERIALIOCONFIG_SPIXHOLD_PORT1HOLD    (1)
#define     NFP3800_SPI_SERIALIOCONFIG_SPIXHOLD_PORT2HOLD    (2)
#define     NFP3800_SPI_SERIALIOCONFIG_SPIXHOLD_PORT3HOLD    (3)
#define   NFP3800_SPI_SERIALIOCONFIG_SPIXSEL3(_x)            (((_x) & 0x3) << 4)
#define   NFP3800_SPI_SERIALIOCONFIG_SPIXSEL3_of(_x)         (((_x) >> 4) & 0x3)
#define     NFP3800_SPI_SERIALIOCONFIG_SPIXSEL3_PORT0SEL3    (0)
#define     NFP3800_SPI_SERIALIOCONFIG_SPIXSEL3_PORT1SEL3    (1)
#define     NFP3800_SPI_SERIALIOCONFIG_SPIXSEL3_PORT2SEL3    (2)
#define     NFP3800_SPI_SERIALIOCONFIG_SPIXSEL3_PORT3SEL3    (3)
#define   NFP3800_SPI_SERIALIOCONFIG_SPIXSEL2(_x)            (((_x) & 0x3) << 2)
#define   NFP3800_SPI_SERIALIOCONFIG_SPIXSEL2_of(_x)         (((_x) >> 2) & 0x3)
#define     NFP3800_SPI_SERIALIOCONFIG_SPIXSEL2_PORT0SEL2    (0)
#define     NFP3800_SPI_SERIALIOCONFIG_SPIXSEL2_PORT1SEL2    (1)
#define     NFP3800_SPI_SERIALIOCONFIG_SPIXSEL2_PORT2SEL2    (2)
#define     NFP3800_SPI_SERIALIOCONFIG_SPIXSEL2_PORT3SEL2    (3)
#define   NFP3800_SPI_SERIALIOCONFIG_SPIXSEL1(_x)            (((_x) & 0x3) << 0)
#define   NFP3800_SPI_SERIALIOCONFIG_SPIXSEL1_of(_x)         (((_x) >> 0) & 0x3)
#define     NFP3800_SPI_SERIALIOCONFIG_SPIXSEL1_PORT0SEL1    (0)
#define     NFP3800_SPI_SERIALIOCONFIG_SPIXSEL1_PORT1SEL1    (1)
#define     NFP3800_SPI_SERIALIOCONFIG_SPIXSEL1_PORT2SEL1    (2)
#define     NFP3800_SPI_SERIALIOCONFIG_SPIXSEL1_PORT3SEL1    (3)
/*----------------------------------------------------------------
  Register: SpiIOIdleStatus - IO pad idle status
    Bits: 31    SampleTrigger(rw) - This bit represents the current sample state. Write a 1 to trigger the sample process, wait until the bit reads as 0 before attempting to access other SPI registers.
    Bits: 17    SPIxD3Idle(rw) - This bit represents the state of the SPIxD3 io pin when not driven. During Sample this bit is set from the SPIxD3 IO pad state.
    Bits: 16    SPIxD2Idle(rw) - This bit represents the state of the SPIxD2 io pin when not driven. During Sample this bit is set from the SPIxD2 IO pad state.
    Bits: 15    SPIxHoldnIdle(rw) - This bit represents the state of the SPIxHOLDn io pin when not driven. During Sample this bit is set from the SPIxHOLDn IO pad state.
    Bits: 14    SPIxSel3Idle(rw) - This bit represents the state of the SPIxSEL3 io pin when not driven. During Sample this bit is set from the SPIxSEL3 IO pad state.
    Bits: 13    SPIxSel2Idle(rw) - This bit represents the state of the SPIxSEL2 io pin when not driven. During Sample this bit is set from the SPIxSEL2 IO pad state.
    Bits: 12    SPIxSel1Idle(rw) - This bit represents the state of the SPIxSEL1 io pin when not driven. During Sample this bit is set from the SPIxSEL1 IO pad state.
    Bits: 11    SPI3D0Idle(rw) - This bit represents the state of the SPI3D0 io pin when not driven. During Sample this bit is set from the SPI3D0 IO pad state.
    Bits: 10    SPI3ClkIdle(rw) - This bit represents the state of the SPI3CLK io pin when not driven. During Sample this bit is set from the SPI3CLK IO pad state.
    Bits:  9    SPI2D0Idle(rw) - This bit represents the state of the SPI2D0 io pin when not driven. During Sample this bit is set from the SPI2D0 IO pad state.
    Bits:  8    SPI2ClkIdle(rw) - This bit represents the state of the SPI2CLK io pin when not driven. During Sample this bit is set from the SPI2CLK IO pad state.
    Bits:  7    SPI1Sel0Idle(rw) - This bit represents the state of the SPI1SEL0 io pin when not driven. During Sample this bit is set from the SPI1SEL0 IO pad state.
    Bits:  6    SPI1D1Idle(rw) - This bit represents the state of the SPI1D1 io pin when not driven. During Sample this bit is set from the SPI1D1 IO pad state.
    Bits:  5    SPI1D0Idle(rw) - This bit represents the state of the SPI1D0 io pin when not driven. During Sample this bit is set from the SPI1D0 IO pad state.
    Bits:  4    SPI1ClkIdle(rw) - This bit represents the state of the SPI1CLK io pin when not driven. During Sample this bit is set from the SPI1CLK IO pad state.
    Bits:  3    SPI0Sel0Idle(rw) - This bit represents the state of the SPI0SEL0 io pin when not driven. During Sample this bit is set from the SPI0SEL0 IO pad state.
    Bits:  2    SPI0D1Idle(rw) - This bit represents the state of the SPI0D1 io pin when not driven. During Sample this bit is set from the SPI0D1 IO pad state.
    Bits:  1    SPI0D0Idle(rw) - This bit represents the state of the SPI0D0 io pin when not driven. During Sample this bit is set from the SPI0D0 IO pad state.
    Bits:  0    SPI0ClkIdle(rw) - This bit represents the state of the SPI0CLK io pin when not driven. During Sample this bit is set from the SPI0CLK IO pad state.
*/
#define NFP3800_SPI_SPIIOIDLESTATUS                          0x00000104
#define   NFP3800_SPI_SPIIOIDLESTATUS_SAMPLETRIGGER          BIT(31)
#define   NFP3800_SPI_SPIIOIDLESTATUS_SPIXD3IDLE             BIT(17)
#define   NFP3800_SPI_SPIIOIDLESTATUS_SPIXD2IDLE             BIT(16)
#define   NFP3800_SPI_SPIIOIDLESTATUS_SPIXHOLDNIDLE          BIT(15)
#define   NFP3800_SPI_SPIIOIDLESTATUS_SPIXSEL3IDLE           BIT(14)
#define   NFP3800_SPI_SPIIOIDLESTATUS_SPIXSEL2IDLE           BIT(13)
#define   NFP3800_SPI_SPIIOIDLESTATUS_SPIXSEL1IDLE           BIT(12)
#define   NFP3800_SPI_SPIIOIDLESTATUS_SPI3D0IDLE             BIT(11)
#define   NFP3800_SPI_SPIIOIDLESTATUS_SPI3CLKIDLE            BIT(10)
#define   NFP3800_SPI_SPIIOIDLESTATUS_SPI2D0IDLE             BIT(9)
#define   NFP3800_SPI_SPIIOIDLESTATUS_SPI2CLKIDLE            BIT(8)
#define   NFP3800_SPI_SPIIOIDLESTATUS_SPI1SEL0IDLE           BIT(7)
#define   NFP3800_SPI_SPIIOIDLESTATUS_SPI1D1IDLE             BIT(6)
#define   NFP3800_SPI_SPIIOIDLESTATUS_SPI1D0IDLE             BIT(5)
#define   NFP3800_SPI_SPIIOIDLESTATUS_SPI1CLKIDLE            BIT(4)
#define   NFP3800_SPI_SPIIOIDLESTATUS_SPI0SEL0IDLE           BIT(3)
#define   NFP3800_SPI_SPIIOIDLESTATUS_SPI0D1IDLE             BIT(2)
#define   NFP3800_SPI_SPIIOIDLESTATUS_SPI0D0IDLE             BIT(1)
#define   NFP3800_SPI_SPIIOIDLESTATUS_SPI0CLKIDLE            BIT(0)
/*----------------------------------------------------------------
  Register: SpiWriteEnableMask - Write enable mask for virtual port configuration registers
    Bits: 31:0  WriteEnableMask(rw) - This register permits writes to the virtual port configuration registers without writing all 32 bits. Bit-for-bit, if the mask bit is a 1 the target register bit is written, if the mask bit is a 0 the target register bit is unaffected. Also see SerialWriteEnableTarget register.
*/
#define NFP3800_SPI_SPIWRITEENABLEMASK                       0x00000108
#define   NFP3800_SPI_SPIWRITEENABLEMASK_WRITEENABLEMASK(_x) (((_x) & 0xffffffff) << 0)
#define   NFP3800_SPI_SPIWRITEENABLEMASK_WRITEENABLEMASK_of(_x) (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: SpiWriteEnableTarget - Write enable target for virtual port configuration registers
    Bits:  4    WriteEnableAvailable(rw) - This bit set indicates the write enable logic is available for use. When the WriteEnableAvailable bit is already set, or a write will set it, the write to this register will be able to set the WriteEnableTarget. When neither case is true, writes to this register have no affect, so this bit acts as a semaphore. A process may attempt to acquire this resource by writing this bit to zero and the target field as desired. If the write succeeds reading this register will return what was written, if not then no action is needed beyond trying again later. To release the resource write this bit to one.
    Bits:  3:0  WriteEnableTarget(rw) - This field specifies the target virtual port that the WriteEnableMask will apply to if the WriteEnableAvailable bit is clear.
*/
#define NFP3800_SPI_SPIWRITEENABLETARGET                     0x0000010c
#define   NFP3800_SPI_SPIWRITEENABLETARGET_WRITEENABLEAVAILABLE BIT(4)
#define   NFP3800_SPI_SPIWRITEENABLETARGET_WRITEENABLETARGET(_x) (((_x) & 0xf) << 0)
#define   NFP3800_SPI_SPIWRITEENABLETARGET_WRITEENABLETARGET_of(_x) (((_x) >> 0) & 0xf)
/*----------------------------------------------------------------
  Register: PhysicalPort[4]ManualConfiguration - Manual configuration for physical port X
    Bits: 31:30 ClockDelay(rw) - This field controls the delay of the clock sent out the IO pad.
    Bits: 23:20 DataOutDelay(rw) - Specify the number of internal clocks to delay the output data relative to the SPI clock when DataSync is set.
    Bits: 19:16 DataInDelay(rw) - Specify the number of internal clocks to delay the sampling of input data relative to the SPI clock when DataSync is set. This value is offset by 3 P_CLK periods (low values sample the input data before the clock edge).
    Bits: 15:14 DataOutSyncMode(rw) - This field controls the synchronous and asynchronous modes of the data outputs.
    Bits: 13:12 DataInSyncMode(rw) - This field controls the synchronous and asynchronous modes of the data inputs.
*/
#define NFP3800_SPI_SPIPORTMCFG(_x)                          (0x00000110 + (0x4 * ((_x) & 0x3)))
#define   NFP3800_SPI_SPIPORTMCFG_CLOCKDELAY(_x)             (((_x) & 0x3) << 30)
#define   NFP3800_SPI_SPIPORTMCFG_CLOCKDELAY_of(_x)          (((_x) >> 30) & 0x3)
#define     NFP3800_SPI_SPIPORTMCFG_CLOCKDELAY_DELAY_0       (0)
#define     NFP3800_SPI_SPIPORTMCFG_CLOCKDELAY_DELAY_8       (1)
#define     NFP3800_SPI_SPIPORTMCFG_CLOCKDELAY_DELAY_16      (2)
#define     NFP3800_SPI_SPIPORTMCFG_CLOCKDELAY_DELAY_24      (3)
#define   NFP3800_SPI_SPIPORTMCFG_DATAOUTDELAY(_x)           (((_x) & 0xf) << 20)
#define   NFP3800_SPI_SPIPORTMCFG_DATAOUTDELAY_of(_x)        (((_x) >> 20) & 0xf)
#define   NFP3800_SPI_SPIPORTMCFG_DATAINDELAY(_x)            (((_x) & 0xf) << 16)
#define   NFP3800_SPI_SPIPORTMCFG_DATAINDELAY_of(_x)         (((_x) >> 16) & 0xf)
#define   NFP3800_SPI_SPIPORTMCFG_DATAOUTSYNCMODE(_x)        (((_x) & 0x3) << 14)
#define   NFP3800_SPI_SPIPORTMCFG_DATAOUTSYNCMODE_of(_x)     (((_x) >> 14) & 0x3)
#define     NFP3800_SPI_SPIPORTMCFG_DATAOUTSYNCMODE_SYNC     (0)
#define     NFP3800_SPI_SPIPORTMCFG_DATAOUTSYNCMODE_RISE     (1)
#define     NFP3800_SPI_SPIPORTMCFG_DATAOUTSYNCMODE_FALL     (2)
#define     NFP3800_SPI_SPIPORTMCFG_DATAOUTSYNCMODE_DDR      (3)
#define   NFP3800_SPI_SPIPORTMCFG_DATAINSYNCMODE(_x)         (((_x) & 0x3) << 12)
#define   NFP3800_SPI_SPIPORTMCFG_DATAINSYNCMODE_of(_x)      (((_x) >> 12) & 0x3)
#define     NFP3800_SPI_SPIPORTMCFG_DATAINSYNCMODE_SYNC      (0)
#define     NFP3800_SPI_SPIPORTMCFG_DATAINSYNCMODE_RISE      (1)
#define     NFP3800_SPI_SPIPORTMCFG_DATAINSYNCMODE_FALL      (2)
#define     NFP3800_SPI_SPIPORTMCFG_DATAINSYNCMODE_DDR       (3)

#else /* NFP3800_LONGNAMES */

/*----------------------------------------------------------------
  Register: PhysicalPort[4]Config - Configuration for physical port X
    Bits: 31    Mode(rw) - Port operating mode.
    Bits: 30    Busy(ro) - In automatic mode this bit indicates if the port is busy. In manual mode this bit indicates if the port is actively counting out a burst.
    Bits: 29    SenseHoldn(ro) - This represents the current state of the HOLDn pin assigned to this port (this bit is set if no pin is assigned to this function on this port).
    Bits: 28    SenseClk(ro) - This represents the current state of the CLK pin assigned to this port.
    Bits: 27:24 SenseSelect(ro) - This represents the current state of the SEL pins assigned to this port (each bit is set if no pin is assigned to this function on this port).
    Bits: 23:20 SenseData(ro) - This represents the current state of the D pins assigned to this port (each bit is set if no pin is assigned to this function on this port).
    Bits: 19:16 SelectIdle(rw) - This represents the state of the Select pins assigned to this port when no device is selected.
    Bits: 15:0  Timeout(rw) - This value represents the number of SPI clock periods to count out before declaring a timeout for any reason.
*/
#define NFP3800_SPI_PhysicalPortConfig(_x)                   (0x00000000 + (0x4 * ((_x) & 0x3)))
#define   NFP3800_SPI_PhysicalPortConfig_Mode                BIT(31)
#define     NFP3800_SPI_PhysicalPortConfig_Mode_Automatic    (0 << 31)
#define     NFP3800_SPI_PhysicalPortConfig_Mode_Manual       BIT(31)
#define   NFP3800_SPI_PhysicalPortConfig_Busy                BIT(30)
#define   NFP3800_SPI_PhysicalPortConfig_SenseHoldn          BIT(29)
#define   NFP3800_SPI_PhysicalPortConfig_SenseClk            BIT(28)
#define   NFP3800_SPI_PhysicalPortConfig_SenseSelect(_x)     (((_x) & 0xf) << 24)
#define   NFP3800_SPI_PhysicalPortConfig_SenseSelect_of(_x)  (((_x) >> 24) & 0xf)
#define   NFP3800_SPI_PhysicalPortConfig_SenseData(_x)       (((_x) & 0xf) << 20)
#define   NFP3800_SPI_PhysicalPortConfig_SenseData_of(_x)    (((_x) >> 20) & 0xf)
#define   NFP3800_SPI_PhysicalPortConfig_SelectIdle(_x)      (((_x) & 0xf) << 16)
#define   NFP3800_SPI_PhysicalPortConfig_SelectIdle_of(_x)   (((_x) >> 16) & 0xf)
#define   NFP3800_SPI_PhysicalPortConfig_Timeout(_x)         (((_x) & 0xffff) << 0)
#define   NFP3800_SPI_PhysicalPortConfig_Timeout_of(_x)      (((_x) >> 0) & 0xffff)
/*----------------------------------------------------------------
  Register: PhysicalPort[4]ManualControl - Manual control for physical port X
  Register: BootPxMCR - Boot shadow copy of the appropriate Manual Control Register
    Bits: 31    DataDriveDisable(rw) - Disable the data driver(s). Use this to control data bits that are bi-directional within a SPI access.
    Bits: 29    ClockIdle(rw) - Aka CPOL. This is the state of the SPI clock pin when idle. During Sample this bit is set from the clock IO pad state.
    Bits: 28    ClockPhase(rw) - Aka CPHA. This is the relative phase of the SPI clock edges to the data edges. During Sample this bit is set from the clock IO pad state.
    Bits: 27:24 Select(rw) - This is the value driven to the select bit(s) for this port.
    Bits: 23    HoldTimeout(ro) - When set indicates an overly long active HOLDn is still stalling the port.
    Bits: 22    HoldEnable(rw) - This bit defines whether the SPI state machine allows the SPI_HOLDn input pin to control the flow of SPI transactions to the external targets.
    Bits: 21:20 DataWidth(rw) - This field defines the data transfer width.
    Bits: 19    DataInTrail(rw) - When set, data in is sampled on the trailing SPI clock edge.
    Bits: 18    DataInLead(rw) - When set, data in is sampled on the leading SPI clock edge.
    Bits: 17    DataOutTrail(rw) - When set, data out changes on the trailing SPI clock edge.
    Bits: 16    DataOutLead(rw) - When set, data out changes on the leading SPI clock edge.
    Bits: 15    ClockDisable(rw) - If set, no SPI clock edges are generated for this burst (but the equivalent time is taken).
    Bits: 14:8  ClockEdgeCount(rw) - This value defines the number of SPI half-clocks to generate for this burst.
    Bits:  7:0  ClockDivisor(rw) - This value defines the number of P_CLK ticks (typically 1 nanosecond) per SPI half-clock, i.e. the SPI clock low and high times. Note during power on configuration the clock base is REF_CLOCK (nominally 100 MHz).
*/
#define NFP3800_SPI_PhysicalPortManualControl(_x)            (0x00000010 + (0x4 * ((_x) & 0x3)))
#define NFP3800_SPI_BootPxMCR                                0x00000134
#define   NFP3800_SPI_PhysicalPortManualControl_DataDriveDisable BIT(31)
#define   NFP3800_SPI_PhysicalPortManualControl_ClockIdle    BIT(29)
#define   NFP3800_SPI_PhysicalPortManualControl_ClockPhase   BIT(28)
#define   NFP3800_SPI_PhysicalPortManualControl_Select(_x)   (((_x) & 0xf) << 24)
#define   NFP3800_SPI_PhysicalPortManualControl_Select_of(_x) (((_x) >> 24) & 0xf)
#define   NFP3800_SPI_PhysicalPortManualControl_HoldTimeout  BIT(23)
#define   NFP3800_SPI_PhysicalPortManualControl_HoldEnable   BIT(22)
#define     NFP3800_SPI_PhysicalPortManualControl_HoldEnable_IGNORE (0 << 22)
#define     NFP3800_SPI_PhysicalPortManualControl_HoldEnable_ALLOW BIT(22)
#define   NFP3800_SPI_PhysicalPortManualControl_DataWidth(_x) (((_x) & 0x3) << 20)
#define   NFP3800_SPI_PhysicalPortManualControl_DataWidth_of(_x) (((_x) >> 20) & 0x3)
#define     NFP3800_SPI_PhysicalPortManualControl_DataWidth_One (0)
#define     NFP3800_SPI_PhysicalPortManualControl_DataWidth_Dual (1)
#define     NFP3800_SPI_PhysicalPortManualControl_DataWidth_Two (2)
#define     NFP3800_SPI_PhysicalPortManualControl_DataWidth_Four (3)
#define   NFP3800_SPI_PhysicalPortManualControl_DataInTrail  BIT(19)
#define   NFP3800_SPI_PhysicalPortManualControl_DataInLead   BIT(18)
#define   NFP3800_SPI_PhysicalPortManualControl_DataOutTrail BIT(17)
#define   NFP3800_SPI_PhysicalPortManualControl_DataOutLead  BIT(16)
#define   NFP3800_SPI_PhysicalPortManualControl_ClockDisable BIT(15)
#define   NFP3800_SPI_PhysicalPortManualControl_ClockEdgeCount(_x) (((_x) & 0x7f) << 8)
#define   NFP3800_SPI_PhysicalPortManualControl_ClockEdgeCount_of(_x) (((_x) >> 8) & 0x7f)
#define   NFP3800_SPI_PhysicalPortManualControl_ClockDivisor(_x) (((_x) & 0xff) << 0)
#define   NFP3800_SPI_PhysicalPortManualControl_ClockDivisor_of(_x) (((_x) >> 0) & 0xff)
/*----------------------------------------------------------------
  Register: PhysicalPort[4]ManualDataOut - Manual data out for physical port X
    Bits: 31:0  DataOut(rw) - This register provides data output from the NFP-3800 to an external device. The MSB (bit 31) is sent first. The number of bits sent is a function of writing the manual control register.
*/
#define NFP3800_SPI_PhysicalPortManualDataOut(_x)            (0x00000020 + (0x4 * ((_x) & 0x3)))
#define   NFP3800_SPI_PhysicalPortManualDataOut_DataOut(_x)  (((_x) & 0xffffffff) << 0)
#define   NFP3800_SPI_PhysicalPortManualDataOut_DataOut_of(_x) (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: PhysicalPort[4]ManualDataIn - Manual data in for physical port X
    Bits: 31:0  DataIn(ro) - This register provides data input from an external device to the NFP-3800. The data shifts in via the LSB (so bit 0 is the most recently received bit). The number of bits received is a function of writing the manual control register.
*/
#define NFP3800_SPI_PhysicalPortManualDataIn(_x)             (0x00000030 + (0x4 * ((_x) & 0x3)))
#define   NFP3800_SPI_PhysicalPortManualDataIn_DataIn(_x)    (((_x) & 0xffffffff) << 0)
#define   NFP3800_SPI_PhysicalPortManualDataIn_DataIn_of(_x) (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: VirtualDevice[16]ConfigA_SerialConfigSpiA - Part of the configuration for virtual device [0-15] as SPI
  Register: BootV00ConfigA_SerialConfigSpiA - Part of the configuration for virtual device [0-15] as SPI
    Bits: 31:26 DataLen(rw) - This field specifies the number of SPI_SCK clock cycles in the data phase of the SPI transaction. The typical value for a single data rate device is 32. For 32-bits of data for a dual data rate device, the value would be 16. For dual data rate devices, only even length number of bits reads/writes are supported. For 32-bits of data for a quad data rate device, the value would be 8. For quad data rate devices, only multiples of four number of bits reads/writes are supported. The legal range is 0 to 32.
    Bits: 25:22 CmdLen(rw) - This field specifies the (number of SPI_SCK clock cycles - 1) in the command phase of the SPI transaction. Typical values range from 2 to 7, representing 3 to 8 command bits. The legal range is from 0 to 15, representing 1 to 16. Note for dual or quad rate command phases this field represents the number of clocks, not the number of command bits.
    Bits: 20:16 AddrLen(rw) - This field provides the ability to select the number of address bits in the address phase of the SPI transaction. Legal values range from 0 to 24, inclusive. Note for dual or quad rate address phases this field represents the number of clocks, not the number of address bits.
    Bits: 15:8  AltReadCmd(rw) - This field can be used to supply an alternate SPI Read command to replace the default read command value (0x3) that would otherwise be used. Note: If the CmdLen field is specified with a value of 8 or greater (indicating a command length of 9 bits or more), this field is combined with the AltWriteCmd field to create a single 16-bit alternative command that is used for both Reads and Writes. If the CmdLen field specifies a SPI command less than 16 bits, the SPI Alternate Command must be right-justified in the 16-bit field (unused bits in the MSBs).
    Bits:  7:0  AltWriteCmd(rw) - This field can be used to supply an alternate SPI Write command to replace the default write command value (0x2) that would otherwise be used. Note: If the CmdLen field is specified with a value of 8 or greater (indicating a command length of 9 bits or more), this field is combined with the AltReadCmd field to create a single 16-bit alternative command that is used for both Reads and Writes. If the CmdLen field specifies a SPI command less than 16 bits, the SPI Alternate Command must be right-justified in the 16-bit field (unused bits in the MSBs).
*/
#define NFP3800_SPI_VirtualDeviceConfigA_SerialConfigSpiA(_x) (0x00000040 + (0x4 * ((_x) & 0xf)))
#define NFP3800_SPI_BootV00ConfigA_SerialConfigSpiA          0x00000120
#define   NFP3800_SPI_VirtualDeviceConfigA_SerialConfigSpiA_DataLen(_x) (((_x) & 0x3f) << 26)
#define   NFP3800_SPI_VirtualDeviceConfigA_SerialConfigSpiA_DataLen_of(_x) (((_x) >> 26) & 0x3f)
#define   NFP3800_SPI_VirtualDeviceConfigA_SerialConfigSpiA_CmdLen(_x) (((_x) & 0xf) << 22)
#define   NFP3800_SPI_VirtualDeviceConfigA_SerialConfigSpiA_CmdLen_of(_x) (((_x) >> 22) & 0xf)
#define   NFP3800_SPI_VirtualDeviceConfigA_SerialConfigSpiA_AddrLen(_x) (((_x) & 0x1f) << 16)
#define   NFP3800_SPI_VirtualDeviceConfigA_SerialConfigSpiA_AddrLen_of(_x) (((_x) >> 16) & 0x1f)
#define   NFP3800_SPI_VirtualDeviceConfigA_SerialConfigSpiA_AltReadCmd(_x) (((_x) & 0xff) << 8)
#define   NFP3800_SPI_VirtualDeviceConfigA_SerialConfigSpiA_AltReadCmd_of(_x) (((_x) >> 8) & 0xff)
#define   NFP3800_SPI_VirtualDeviceConfigA_SerialConfigSpiA_AltWriteCmd(_x) (((_x) & 0xff) << 0)
#define   NFP3800_SPI_VirtualDeviceConfigA_SerialConfigSpiA_AltWriteCmd_of(_x) (((_x) >> 0) & 0xff)
/*----------------------------------------------------------------
  Register: VirtualDevice[16]ConfigA_SerialConfigMdioA - Part of the configuration for virtual device [0-15] as MDIO
  Register: BootV00ConfigA_SerialConfigMdioA - Part of the configuration for virtual device [0-15] as MDIO
    Bits: 31:26 DataLen(rw) - This field specifies the number of bits in the Data phase of the MDIO management frame. Typical value is 16. The legal range is 0 to 32.
    Bits: 25:24 TALen(rw) - This field specifies the number of bits in the Turn-around phase of the MDIO management frame. Legal range is from 0 to 3. Note, however, that the currently legal value per IEEE 802.3, clauses 22 and 45 is 2. This field is supplied to handle either new definitions for MDIO or another serial IO standard
    Bits: 20:16 HdrLen(rw) - This field specifies the number of bits in the Header of the MDIO management frame. The header is considered to be the bits starting with the Start-of-Frame bits up to but not including the Turn-around bits. Legal values range from 0 to 24, inclusive. Note, however, that the MDIO standard, IEEE 802.3, clauses 22 and 45, to which this interface is designed, specifies this range as 14 bits. The expanded range allowed in this implementation is designed to handle as yet unspecified MDIO capabilities, and possible additional serial IO standards.
    Bits: 15:8  HeaderMid(rw) - This field is used to specify the actual MDIO management frame header bits to be sent to the MDIO device on the Serial Device. The MSB of the header is the first start-of-frame bit sent to the MDIO device. The LSB of the header is the last bit of the REGADR (clause 22) or DEVTYPE (clause 45) sent to the MDIO device.
    Bits:  7:0  HeaderLow(rw) - This field is used to specify the actual MDIO management frame header bits to be sent to the MDIO device on the Serial Device. The MSB of the header is the first start-of-frame bit sent to the MDIO device. The LSB of the header is the last bit of the REGADR (clause 22) or DEVTYPE (clause 45) sent to the MDIO device.
*/
#define NFP3800_SPI_VirtualDeviceConfigA_SerialConfigMdioA(_x) (0x00000040 + (0x4 * ((_x) & 0xf)))
#define NFP3800_SPI_BootV00ConfigA_SerialConfigMdioA         0x00000120
#define   NFP3800_SPI_VirtualDeviceConfigA_SerialConfigMdioA_DataLen(_x) (((_x) & 0x3f) << 26)
#define   NFP3800_SPI_VirtualDeviceConfigA_SerialConfigMdioA_DataLen_of(_x) (((_x) >> 26) & 0x3f)
#define   NFP3800_SPI_VirtualDeviceConfigA_SerialConfigMdioA_TALen(_x) (((_x) & 0x3) << 24)
#define   NFP3800_SPI_VirtualDeviceConfigA_SerialConfigMdioA_TALen_of(_x) (((_x) >> 24) & 0x3)
#define   NFP3800_SPI_VirtualDeviceConfigA_SerialConfigMdioA_HdrLen(_x) (((_x) & 0x1f) << 16)
#define   NFP3800_SPI_VirtualDeviceConfigA_SerialConfigMdioA_HdrLen_of(_x) (((_x) >> 16) & 0x1f)
#define   NFP3800_SPI_VirtualDeviceConfigA_SerialConfigMdioA_HeaderMid(_x) (((_x) & 0xff) << 8)
#define   NFP3800_SPI_VirtualDeviceConfigA_SerialConfigMdioA_HeaderMid_of(_x) (((_x) >> 8) & 0xff)
#define   NFP3800_SPI_VirtualDeviceConfigA_SerialConfigMdioA_HeaderLow(_x) (((_x) & 0xff) << 0)
#define   NFP3800_SPI_VirtualDeviceConfigA_SerialConfigMdioA_HeaderLow_of(_x) (((_x) >> 0) & 0xff)
/*----------------------------------------------------------------
  Register: VirtualDevice[16]ConfigB_SerialConfigSpiB - Part of the configuration for virtual device [0-15] as SPI
  Register: BootV00ConfigB_SerialConfigSpiB - Part of the configuration for virtual device [0-15] as SPI
    Bits: 31    MDIOSel(rw) - This bit chooses between treating the Serial Device as a SPI device or a MDIO device. Note that in typical usage physical ports 0 and 1 have this bit as 0; physical ports 2 and 3 have this bit as 1
    Bits: 30    AltReadEn(rw) - This bit enables the use of the AltReadCmd specified in the SerialConfigSpiA register for this configuration. Note: If using the combined SPI alternate command (AltReadCmd and AltWriteCmd) fields, due to CmdLen indicating the SPI command is 9 bits or greater, this bit enables the use of the combined SPI Alternate Command field only for commands entered as SSB reads.
    Bits: 29    AltWriteEn(rw) - This bit enables the use of the AltWriteCmd specified in the SerialConfigSpiA Register for this configuration. Note: If using the combined SPI alternate command (AltReadCmd and AltWriteCmd) fields, due to CmdLen indicating the SPI command is 9 bits or greater, this bit enables the use of the combined SPI Alternate Command field only for commands entered as SSB writes.
    Bits: 28:24 CSDelay(rw) - This field specifies the number of 20 nanosecond periods to wait between de-assertion and assertion of the SPI Chip Select, presuming a P_CLK period of 1 nanosecond.
    Bits: 23:22 Address(rw) - This field specifies the two MSB of the 24 bit address.
    Bits: 21    Busy(rw) - This bit can be set to '1' by the user when setting up a transaction for the SPI device. If the user does not set it, the Serial Device Controller will set it when it starts executing the command. In either case, when the Serial Device Controller completes the command and the SPI device is ready to accept a new command, the Serial Device Controller will clear this bit to '0'.
    Bits: 20    BootPromWriteEn(rw) - This bit provides some protection against software bugs accidentally causing a write to the SPI Serial Boot PROM space that overwrites good data. An attempt to write to a virtual SPI device without first setting this bit will be ignored.
    Bits: 19:18 Mode(rw) - Select the mode of operation for the SPI bus. During Sample the state of the SPI[0/1]_CLK (for even/odd numbered virtual ports) is used to select Mode = 0 or Mode = 3.
    Bits: 17:16 ReadMode(rw) - SPI mode to use for reading
    Bits: 15    HoldEn(rw) - This bit defines whether the SPI state machine allows the SPI_HOLDn input pin to control the flow of SPI transactions to the external targets.
    Bits: 14    HoldTimeout(ro) - When set indicates an overly long active HOLDn is still stalling the port.
    Bits: 13:8  DummyLen(rw) - This value specifies the number of Dummy bits to be sent in the SPI frame. Dummy bits are always sent after the command and address bits have been sent. Legal values range from 0 to 32.
    Bits:  7:0  Divider(rw) - The value provided here is the number of P_CLK clocks per SPI clock low period and per SPI clock high period.
*/
#define NFP3800_SPI_VirtualDeviceConfigB_SerialConfigSpiB(_x) (0x00000080 + (0x4 * ((_x) & 0xf)))
#define NFP3800_SPI_BootV00ConfigB_SerialConfigSpiB          0x00000124
#define   NFP3800_SPI_VirtualDeviceConfigB_SerialConfigSpiB_MDIOSel BIT(31)
#define     NFP3800_SPI_VirtualDeviceConfigB_SerialConfigSpiB_MDIOSel_SPI (0 << 31)
#define     NFP3800_SPI_VirtualDeviceConfigB_SerialConfigSpiB_MDIOSel_MDIO BIT(31)
#define   NFP3800_SPI_VirtualDeviceConfigB_SerialConfigSpiB_AltReadEn BIT(30)
#define     NFP3800_SPI_VirtualDeviceConfigB_SerialConfigSpiB_AltReadEn_DISABLED (0 << 30)
#define     NFP3800_SPI_VirtualDeviceConfigB_SerialConfigSpiB_AltReadEn_ENABLED BIT(30)
#define   NFP3800_SPI_VirtualDeviceConfigB_SerialConfigSpiB_AltWriteEn BIT(29)
#define     NFP3800_SPI_VirtualDeviceConfigB_SerialConfigSpiB_AltWriteEn_DISABLED (0 << 29)
#define     NFP3800_SPI_VirtualDeviceConfigB_SerialConfigSpiB_AltWriteEn_ENABLED BIT(29)
#define   NFP3800_SPI_VirtualDeviceConfigB_SerialConfigSpiB_CSDelay(_x) (((_x) & 0x1f) << 24)
#define   NFP3800_SPI_VirtualDeviceConfigB_SerialConfigSpiB_CSDelay_of(_x) (((_x) >> 24) & 0x1f)
#define   NFP3800_SPI_VirtualDeviceConfigB_SerialConfigSpiB_Address(_x) (((_x) & 0x3) << 22)
#define   NFP3800_SPI_VirtualDeviceConfigB_SerialConfigSpiB_Address_of(_x) (((_x) >> 22) & 0x3)
#define   NFP3800_SPI_VirtualDeviceConfigB_SerialConfigSpiB_Busy BIT(21)
#define     NFP3800_SPI_VirtualDeviceConfigB_SerialConfigSpiB_Busy_NOT_BUSY (0 << 21)
#define     NFP3800_SPI_VirtualDeviceConfigB_SerialConfigSpiB_Busy_BUSY BIT(21)
#define   NFP3800_SPI_VirtualDeviceConfigB_SerialConfigSpiB_BootPromWriteEn BIT(20)
#define     NFP3800_SPI_VirtualDeviceConfigB_SerialConfigSpiB_BootPromWriteEn_DISCARD (0 << 20)
#define     NFP3800_SPI_VirtualDeviceConfigB_SerialConfigSpiB_BootPromWriteEn_ENABLE BIT(20)
#define   NFP3800_SPI_VirtualDeviceConfigB_SerialConfigSpiB_Mode(_x) (((_x) & 0x3) << 18)
#define   NFP3800_SPI_VirtualDeviceConfigB_SerialConfigSpiB_Mode_of(_x) (((_x) >> 18) & 0x3)
#define     NFP3800_SPI_VirtualDeviceConfigB_SerialConfigSpiB_Mode_FIRST_EDGE_CLK0 (0)
#define     NFP3800_SPI_VirtualDeviceConfigB_SerialConfigSpiB_Mode_SECOND_EDGE_CLK0 (1)
#define     NFP3800_SPI_VirtualDeviceConfigB_SerialConfigSpiB_Mode_FIRST_EDGE_CLK1 (2)
#define     NFP3800_SPI_VirtualDeviceConfigB_SerialConfigSpiB_Mode_SECOND_EDGE_CLK1 (3)
#define   NFP3800_SPI_VirtualDeviceConfigB_SerialConfigSpiB_ReadMode(_x) (((_x) & 0x3) << 16)
#define   NFP3800_SPI_VirtualDeviceConfigB_SerialConfigSpiB_ReadMode_of(_x) (((_x) >> 16) & 0x3)
#define     NFP3800_SPI_VirtualDeviceConfigB_SerialConfigSpiB_ReadMode_SLOW (0)
#define     NFP3800_SPI_VirtualDeviceConfigB_SerialConfigSpiB_ReadMode_FAST_SINGLE (1)
#define     NFP3800_SPI_VirtualDeviceConfigB_SerialConfigSpiB_ReadMode_FAST_DUAL (3)
#define   NFP3800_SPI_VirtualDeviceConfigB_SerialConfigSpiB_HoldEn BIT(15)
#define     NFP3800_SPI_VirtualDeviceConfigB_SerialConfigSpiB_HoldEn_IGNORE (0 << 15)
#define     NFP3800_SPI_VirtualDeviceConfigB_SerialConfigSpiB_HoldEn_ALLOW BIT(15)
#define   NFP3800_SPI_VirtualDeviceConfigB_SerialConfigSpiB_HoldTimeout BIT(14)
#define   NFP3800_SPI_VirtualDeviceConfigB_SerialConfigSpiB_DummyLen(_x) (((_x) & 0x3f) << 8)
#define   NFP3800_SPI_VirtualDeviceConfigB_SerialConfigSpiB_DummyLen_of(_x) (((_x) >> 8) & 0x3f)
#define   NFP3800_SPI_VirtualDeviceConfigB_SerialConfigSpiB_Divider(_x) (((_x) & 0xff) << 0)
#define   NFP3800_SPI_VirtualDeviceConfigB_SerialConfigSpiB_Divider_of(_x) (((_x) >> 0) & 0xff)
/*----------------------------------------------------------------
  Register: VirtualDevice[16]ConfigB_SerialConfigMdioB - Part of the configuration for virtual device [0-15] as MDIO
  Register: BootV00ConfigB_SerialConfigMdioB - Part of the configuration for virtual device [0-15] as MDIO
    Bits: 31    MDIOSel(rw) - This bit chooses between treating the Serial Device as a SPI device or a MDIO device.
    Bits: 30    PreambleEn(rw) - This bit enables the use of the 32-bit Preamble in the MDIO management frame.
    Bits: 26:24 TAData(rw) - This field specifies the actual turn-around bits to be sent to the MDIO device on a MDIO Write. In the case of a MDIO Read, the MDIO data bit output enable is disabled, and will be pulled up by the external pull-up resistor.
    Bits: 23    DataOutLead(rw) - This bit controls the active clock edge for data output.
    Bits: 22    DataInLead(rw) - This bit controls the active clock edge for data input.
    Bits: 21    Busy(rw) - This bit can be set to '1' by the user when setting up a transaction for the SPI device. If the user does not set it, the Serial Device Controller will set it when it starts executing the command. In either case, when the Serial Device Controller completes the command and the SPI device is ready to accept a new command, the Serial Device Controller will clear this bit to '0'.
    Bits: 19:12 HeaderHigh(rw) - This field is used to specify the actual MDIO management frame header bits [23:16] to be sent to the MDIO device. The MSB of the header is the first start-of-frame bit sent to the MDIO device. The LSB of the header is the last bit of the REGADR (clause 22) or DEVTYPE (clause 45) sent to the MDIO device. The bits of the MDIO header which are actually used are right-justified inside the overall 24-bit header field. That is bit 0 of the actual header field goes at bit 0 of the overall 24-bit header provisioned in these registers. Unused bits are ignored.
    Bits:  9:0  Divider(rw) - The value provided here is the number of P_CLK clocks per MDIO clock low period and per MDIO clock high period.
*/
#define NFP3800_SPI_VirtualDeviceConfigB_SerialConfigMdioB(_x) (0x00000080 + (0x4 * ((_x) & 0xf)))
#define NFP3800_SPI_BootV00ConfigB_SerialConfigMdioB         0x00000124
#define   NFP3800_SPI_VirtualDeviceConfigB_SerialConfigMdioB_MDIOSel BIT(31)
#define     NFP3800_SPI_VirtualDeviceConfigB_SerialConfigMdioB_MDIOSel_SPI (0 << 31)
#define     NFP3800_SPI_VirtualDeviceConfigB_SerialConfigMdioB_MDIOSel_MDIO BIT(31)
#define   NFP3800_SPI_VirtualDeviceConfigB_SerialConfigMdioB_PreambleEn BIT(30)
#define     NFP3800_SPI_VirtualDeviceConfigB_SerialConfigMdioB_PreambleEn_DISABLE (0 << 30)
#define     NFP3800_SPI_VirtualDeviceConfigB_SerialConfigMdioB_PreambleEn_ENABLE BIT(30)
#define   NFP3800_SPI_VirtualDeviceConfigB_SerialConfigMdioB_TAData(_x) (((_x) & 0x7) << 24)
#define   NFP3800_SPI_VirtualDeviceConfigB_SerialConfigMdioB_TAData_of(_x) (((_x) >> 24) & 0x7)
#define   NFP3800_SPI_VirtualDeviceConfigB_SerialConfigMdioB_DataOutLead BIT(23)
#define     NFP3800_SPI_VirtualDeviceConfigB_SerialConfigMdioB_DataOutLead_TRAIL (0 << 23)
#define     NFP3800_SPI_VirtualDeviceConfigB_SerialConfigMdioB_DataOutLead_LEAD BIT(23)
#define   NFP3800_SPI_VirtualDeviceConfigB_SerialConfigMdioB_DataInLead BIT(22)
#define     NFP3800_SPI_VirtualDeviceConfigB_SerialConfigMdioB_DataInLead_TRAIL (0 << 22)
#define     NFP3800_SPI_VirtualDeviceConfigB_SerialConfigMdioB_DataInLead_LEAD BIT(22)
#define   NFP3800_SPI_VirtualDeviceConfigB_SerialConfigMdioB_Busy BIT(21)
#define     NFP3800_SPI_VirtualDeviceConfigB_SerialConfigMdioB_Busy_NOT_BUSY (0 << 21)
#define     NFP3800_SPI_VirtualDeviceConfigB_SerialConfigMdioB_Busy_BUSY BIT(21)
#define   NFP3800_SPI_VirtualDeviceConfigB_SerialConfigMdioB_HeaderHigh(_x) (((_x) & 0xff) << 12)
#define   NFP3800_SPI_VirtualDeviceConfigB_SerialConfigMdioB_HeaderHigh_of(_x) (((_x) >> 12) & 0xff)
#define   NFP3800_SPI_VirtualDeviceConfigB_SerialConfigMdioB_Divider(_x) (((_x) & 0x3ff) << 0)
#define   NFP3800_SPI_VirtualDeviceConfigB_SerialConfigMdioB_Divider_of(_x) (((_x) >> 0) & 0x3ff)
/*----------------------------------------------------------------
  Register: VirtualDevice[16]ConfigC_SerialConfigSpiC - Part of the configuration for virtual device [0-15] as SPI
  Register: BootV00ConfigC_SerialConfigSpiC - Part of the configuration for virtual device [0-15] as SPI
    Bits: 31:30 ClockDelay(rw) - This field controls the delay of the clock sent out the IO pad.
    Bits: 29:28 DataWidthDat(rw) - Specify the data width during the data phase.
    Bits: 27:26 DataWidthAdr(rw) - Specify the data width during the address phase.
    Bits: 25:24 DataWidthCmd(rw) - Specify the data width during the command phase.
    Bits: 23:20 DataOutDelay(rw) - Specify the number of P_CLK ticks to delay the output data relative to the SPI clock. A value of 0 corresponds to the data edge being lined up with the clock edge.
    Bits: 19:16 DataInDelay(rw) - Specify the number of P_CLK ticks to delay the sampling of input data relative to the SPI clock. A value of 3 corresponds to the data edge being lined up with the clock edge.
    Bits: 15:14 DataOutSyncMode(rw) - This field controls the synchronous and asynchronous modes of the data outputs.
    Bits: 13:12 DataInSyncMode(rw) - This field controls the synchronous and asynchronous modes of the data inputs.
    Bits: 11:8  Select(rw) - This field specifies the value to put out on the select pins of the physical port associated with this virtual device.
    Bits:  7    FastRelinqEn(rw) - When this bit is set and this virtual device has a burst in progress and another virtual device wants the same port, this device will immediately terminate the burst and release the port.
    Bits:  6    IgnoreBusyEn(rw) - When this bit is set writes to this virtual port's config A, B, C registers are permitted even when the port is busy. A write with this bit set is permitted even if the registered value does not permit the write.
    Bits:  5    Sequential(rw) - When this bit is set and DataLen multiplied by the effective data width is 32 bits, the address used must be sequential to continue a burst read transaction. If the address is not sequential, the current burst is terminated and the new access performs the full command, address, dummy, data transfer.
    Bits:  4    PrefetchEnable(rw) - If BurstEnable is 1, this bit controls if the SPI hardware will attempt to prefetch read data during a burst read transaction. Only up to one DataLen burst will be prefetched.
    Bits:  3    TerminateBurst(w1c) - If a transaction burst is in progress, writing a one to this bit will terminate the burst. Termination will stop any prefetch in progress, discard any prefetched data, deselect the device, and clear the Busy bit.
    Bits:  2    BurstEnable(rw) - This bit enables burst transactions, whereby the hardware keeps the device selected for multiple sequential accesses.
    Bits:  1:0  PhysicalPort(rw) - This field specifies the physical port associated with this virtual device. After reset all even virtual devices are set to physical port 0, all odd virtual devices are set to physical port 1.
*/
#define NFP3800_SPI_VirtualDeviceConfigC_SerialConfigSpiC(_x) (0x000000c0 + (0x4 * ((_x) & 0xf)))
#define NFP3800_SPI_BootV00ConfigC_SerialConfigSpiC          0x00000128
#define   NFP3800_SPI_VirtualDeviceConfigC_SerialConfigSpiC_ClockDelay(_x) (((_x) & 0x3) << 30)
#define   NFP3800_SPI_VirtualDeviceConfigC_SerialConfigSpiC_ClockDelay_of(_x) (((_x) >> 30) & 0x3)
#define     NFP3800_SPI_VirtualDeviceConfigC_SerialConfigSpiC_ClockDelay_Delay_0 (0)
#define     NFP3800_SPI_VirtualDeviceConfigC_SerialConfigSpiC_ClockDelay_Delay_8 (1)
#define     NFP3800_SPI_VirtualDeviceConfigC_SerialConfigSpiC_ClockDelay_Delay_16 (2)
#define     NFP3800_SPI_VirtualDeviceConfigC_SerialConfigSpiC_ClockDelay_Delay_24 (3)
#define   NFP3800_SPI_VirtualDeviceConfigC_SerialConfigSpiC_DataWidthDat(_x) (((_x) & 0x3) << 28)
#define   NFP3800_SPI_VirtualDeviceConfigC_SerialConfigSpiC_DataWidthDat_of(_x) (((_x) >> 28) & 0x3)
#define     NFP3800_SPI_VirtualDeviceConfigC_SerialConfigSpiC_DataWidthDat_Width_is_1 (0)
#define     NFP3800_SPI_VirtualDeviceConfigC_SerialConfigSpiC_DataWidthDat_Width_is_2 (2)
#define     NFP3800_SPI_VirtualDeviceConfigC_SerialConfigSpiC_DataWidthDat_Width_is_4 (3)
#define   NFP3800_SPI_VirtualDeviceConfigC_SerialConfigSpiC_DataWidthAdr(_x) (((_x) & 0x3) << 26)
#define   NFP3800_SPI_VirtualDeviceConfigC_SerialConfigSpiC_DataWidthAdr_of(_x) (((_x) >> 26) & 0x3)
#define     NFP3800_SPI_VirtualDeviceConfigC_SerialConfigSpiC_DataWidthAdr_Width_is_1 (0)
#define     NFP3800_SPI_VirtualDeviceConfigC_SerialConfigSpiC_DataWidthAdr_Width_is_2 (2)
#define     NFP3800_SPI_VirtualDeviceConfigC_SerialConfigSpiC_DataWidthAdr_Width_is_4 (3)
#define   NFP3800_SPI_VirtualDeviceConfigC_SerialConfigSpiC_DataWidthCmd(_x) (((_x) & 0x3) << 24)
#define   NFP3800_SPI_VirtualDeviceConfigC_SerialConfigSpiC_DataWidthCmd_of(_x) (((_x) >> 24) & 0x3)
#define     NFP3800_SPI_VirtualDeviceConfigC_SerialConfigSpiC_DataWidthCmd_Width_is_1 (0)
#define     NFP3800_SPI_VirtualDeviceConfigC_SerialConfigSpiC_DataWidthCmd_Width_is_2 (2)
#define     NFP3800_SPI_VirtualDeviceConfigC_SerialConfigSpiC_DataWidthCmd_Width_is_4 (3)
#define   NFP3800_SPI_VirtualDeviceConfigC_SerialConfigSpiC_DataOutDelay(_x) (((_x) & 0xf) << 20)
#define   NFP3800_SPI_VirtualDeviceConfigC_SerialConfigSpiC_DataOutDelay_of(_x) (((_x) >> 20) & 0xf)
#define   NFP3800_SPI_VirtualDeviceConfigC_SerialConfigSpiC_DataInDelay(_x) (((_x) & 0xf) << 16)
#define   NFP3800_SPI_VirtualDeviceConfigC_SerialConfigSpiC_DataInDelay_of(_x) (((_x) >> 16) & 0xf)
#define   NFP3800_SPI_VirtualDeviceConfigC_SerialConfigSpiC_DataOutSyncMode(_x) (((_x) & 0x3) << 14)
#define   NFP3800_SPI_VirtualDeviceConfigC_SerialConfigSpiC_DataOutSyncMode_of(_x) (((_x) >> 14) & 0x3)
#define     NFP3800_SPI_VirtualDeviceConfigC_SerialConfigSpiC_DataOutSyncMode_SYNC (0)
#define     NFP3800_SPI_VirtualDeviceConfigC_SerialConfigSpiC_DataOutSyncMode_RISE (1)
#define     NFP3800_SPI_VirtualDeviceConfigC_SerialConfigSpiC_DataOutSyncMode_FALL (2)
#define     NFP3800_SPI_VirtualDeviceConfigC_SerialConfigSpiC_DataOutSyncMode_DDR (3)
#define   NFP3800_SPI_VirtualDeviceConfigC_SerialConfigSpiC_DataInSyncMode(_x) (((_x) & 0x3) << 12)
#define   NFP3800_SPI_VirtualDeviceConfigC_SerialConfigSpiC_DataInSyncMode_of(_x) (((_x) >> 12) & 0x3)
#define     NFP3800_SPI_VirtualDeviceConfigC_SerialConfigSpiC_DataInSyncMode_SYNC (0)
#define     NFP3800_SPI_VirtualDeviceConfigC_SerialConfigSpiC_DataInSyncMode_RISE (1)
#define     NFP3800_SPI_VirtualDeviceConfigC_SerialConfigSpiC_DataInSyncMode_FALL (2)
#define     NFP3800_SPI_VirtualDeviceConfigC_SerialConfigSpiC_DataInSyncMode_DDR (3)
#define   NFP3800_SPI_VirtualDeviceConfigC_SerialConfigSpiC_Select(_x) (((_x) & 0xf) << 8)
#define   NFP3800_SPI_VirtualDeviceConfigC_SerialConfigSpiC_Select_of(_x) (((_x) >> 8) & 0xf)
#define   NFP3800_SPI_VirtualDeviceConfigC_SerialConfigSpiC_FastRelinqEn BIT(7)
#define   NFP3800_SPI_VirtualDeviceConfigC_SerialConfigSpiC_IgnoreBusyEn BIT(6)
#define   NFP3800_SPI_VirtualDeviceConfigC_SerialConfigSpiC_Sequential BIT(5)
#define   NFP3800_SPI_VirtualDeviceConfigC_SerialConfigSpiC_PrefetchEnable BIT(4)
#define   NFP3800_SPI_VirtualDeviceConfigC_SerialConfigSpiC_TerminateBurst BIT(3)
#define   NFP3800_SPI_VirtualDeviceConfigC_SerialConfigSpiC_BurstEnable BIT(2)
#define   NFP3800_SPI_VirtualDeviceConfigC_SerialConfigSpiC_PhysicalPort(_x) (((_x) & 0x3) << 0)
#define   NFP3800_SPI_VirtualDeviceConfigC_SerialConfigSpiC_PhysicalPort_of(_x) (((_x) >> 0) & 0x3)
/*----------------------------------------------------------------
  Register: VirtualDevice[16]ConfigC_SerialConfigMdioC - Part of the configuration for virtual device [0-15] as MDIO
  Register: BootV00ConfigC_SerialConfigMdioC - Part of the configuration for virtual device [0-15] as MDIO
    Bits: 31:30 ClockDelay(rw) - This field controls the delay of the clock sent out the IO pad.
    Bits: 23:20 DataOutDelay(rw) - Specify the number of P_CLK ticks to delay the output data relative to the SPI clock. A value of 0 corresponds to the data edge being lined up with the clock edge.
    Bits: 19:16 DataInDelay(rw) - Specify the number of P_CLK ticks to delay the sampling of input data relative to the SPI clock. A value of 3 corresponds to the data edge being lined up with the clock edge.
    Bits: 15:14 DataOutSyncMode(rw) - This field controls the synchronous and asynchronous modes of the data outputs.
    Bits: 13:12 DataInSyncMode(rw) - This field controls the synchronous and asynchronous modes of the data inputs.
    Bits:  1:0  PhysicalPort(rw) - This field specifies the physical port associated with this virtual device.
*/
#define NFP3800_SPI_VirtualDeviceConfigC_SerialConfigMdioC(_x) (0x000000c0 + (0x4 * ((_x) & 0xf)))
#define NFP3800_SPI_BootV00ConfigC_SerialConfigMdioC         0x00000128
#define   NFP3800_SPI_VirtualDeviceConfigC_SerialConfigMdioC_ClockDelay(_x) (((_x) & 0x3) << 30)
#define   NFP3800_SPI_VirtualDeviceConfigC_SerialConfigMdioC_ClockDelay_of(_x) (((_x) >> 30) & 0x3)
#define     NFP3800_SPI_VirtualDeviceConfigC_SerialConfigMdioC_ClockDelay_Delay_0 (0)
#define     NFP3800_SPI_VirtualDeviceConfigC_SerialConfigMdioC_ClockDelay_Delay_8 (1)
#define     NFP3800_SPI_VirtualDeviceConfigC_SerialConfigMdioC_ClockDelay_Delay_16 (2)
#define     NFP3800_SPI_VirtualDeviceConfigC_SerialConfigMdioC_ClockDelay_Delay_24 (3)
#define   NFP3800_SPI_VirtualDeviceConfigC_SerialConfigMdioC_DataOutDelay(_x) (((_x) & 0xf) << 20)
#define   NFP3800_SPI_VirtualDeviceConfigC_SerialConfigMdioC_DataOutDelay_of(_x) (((_x) >> 20) & 0xf)
#define   NFP3800_SPI_VirtualDeviceConfigC_SerialConfigMdioC_DataInDelay(_x) (((_x) & 0xf) << 16)
#define   NFP3800_SPI_VirtualDeviceConfigC_SerialConfigMdioC_DataInDelay_of(_x) (((_x) >> 16) & 0xf)
#define   NFP3800_SPI_VirtualDeviceConfigC_SerialConfigMdioC_DataOutSyncMode(_x) (((_x) & 0x3) << 14)
#define   NFP3800_SPI_VirtualDeviceConfigC_SerialConfigMdioC_DataOutSyncMode_of(_x) (((_x) >> 14) & 0x3)
#define     NFP3800_SPI_VirtualDeviceConfigC_SerialConfigMdioC_DataOutSyncMode_SYNC (0)
#define     NFP3800_SPI_VirtualDeviceConfigC_SerialConfigMdioC_DataOutSyncMode_RISE (1)
#define     NFP3800_SPI_VirtualDeviceConfigC_SerialConfigMdioC_DataOutSyncMode_FALL (2)
#define     NFP3800_SPI_VirtualDeviceConfigC_SerialConfigMdioC_DataOutSyncMode_DDR (3)
#define   NFP3800_SPI_VirtualDeviceConfigC_SerialConfigMdioC_DataInSyncMode(_x) (((_x) & 0x3) << 12)
#define   NFP3800_SPI_VirtualDeviceConfigC_SerialConfigMdioC_DataInSyncMode_of(_x) (((_x) >> 12) & 0x3)
#define     NFP3800_SPI_VirtualDeviceConfigC_SerialConfigMdioC_DataInSyncMode_SYNC (0)
#define     NFP3800_SPI_VirtualDeviceConfigC_SerialConfigMdioC_DataInSyncMode_RISE (1)
#define     NFP3800_SPI_VirtualDeviceConfigC_SerialConfigMdioC_DataInSyncMode_FALL (2)
#define     NFP3800_SPI_VirtualDeviceConfigC_SerialConfigMdioC_DataInSyncMode_DDR (3)
#define   NFP3800_SPI_VirtualDeviceConfigC_SerialConfigMdioC_PhysicalPort(_x) (((_x) & 0x3) << 0)
#define   NFP3800_SPI_VirtualDeviceConfigC_SerialConfigMdioC_PhysicalPort_of(_x) (((_x) >> 0) & 0x3)
/*----------------------------------------------------------------
  Register: SpiIOConfig - IO pad configuration
  Register: BootIOConfig - Boot shadow copy of the IO pad configuration
    Bits:  9:8  SPIXDAT23(rw) - This field controls which port the SPIXDAT[2-3] pins are associated with.
    Bits:  7:6  SPIXHOLD(rw) - This field controls which port the SPIXHOLDn pin is associated with.
    Bits:  5:4  SPIXSEL3(rw) - This field controls which port the SPIXSEL3 pin is associated with.
    Bits:  3:2  SPIXSEl2(rw) - This field controls which port the SPIXSEL2 pin is associated with.
    Bits:  1:0  SPIXSEL1(rw) - This field controls which port the SPIXSEL1 pin is associated with.
*/
#define NFP3800_SPI_SpiIOConfig                              0x00000100
#define NFP3800_SPI_BootIOConfig                             0x0000012c
#define   NFP3800_SPI_SerialIOConfig_SPIXDAT23(_x)           (((_x) & 0x3) << 8)
#define   NFP3800_SPI_SerialIOConfig_SPIXDAT23_of(_x)        (((_x) >> 8) & 0x3)
#define     NFP3800_SPI_SerialIOConfig_SPIXDAT23_PORT0DAT23  (0)
#define     NFP3800_SPI_SerialIOConfig_SPIXDAT23_PORT1DAT23  (1)
#define     NFP3800_SPI_SerialIOConfig_SPIXDAT23_PORT2DAT23  (2)
#define     NFP3800_SPI_SerialIOConfig_SPIXDAT23_PORT3DAT23  (3)
#define   NFP3800_SPI_SerialIOConfig_SPIXHOLD(_x)            (((_x) & 0x3) << 6)
#define   NFP3800_SPI_SerialIOConfig_SPIXHOLD_of(_x)         (((_x) >> 6) & 0x3)
#define     NFP3800_SPI_SerialIOConfig_SPIXHOLD_PORT0HOLD    (0)
#define     NFP3800_SPI_SerialIOConfig_SPIXHOLD_PORT1HOLD    (1)
#define     NFP3800_SPI_SerialIOConfig_SPIXHOLD_PORT2HOLD    (2)
#define     NFP3800_SPI_SerialIOConfig_SPIXHOLD_PORT3HOLD    (3)
#define   NFP3800_SPI_SerialIOConfig_SPIXSEL3(_x)            (((_x) & 0x3) << 4)
#define   NFP3800_SPI_SerialIOConfig_SPIXSEL3_of(_x)         (((_x) >> 4) & 0x3)
#define     NFP3800_SPI_SerialIOConfig_SPIXSEL3_PORT0SEL3    (0)
#define     NFP3800_SPI_SerialIOConfig_SPIXSEL3_PORT1SEL3    (1)
#define     NFP3800_SPI_SerialIOConfig_SPIXSEL3_PORT2SEL3    (2)
#define     NFP3800_SPI_SerialIOConfig_SPIXSEL3_PORT3SEL3    (3)
#define   NFP3800_SPI_SerialIOConfig_SPIXSEl2(_x)            (((_x) & 0x3) << 2)
#define   NFP3800_SPI_SerialIOConfig_SPIXSEl2_of(_x)         (((_x) >> 2) & 0x3)
#define     NFP3800_SPI_SerialIOConfig_SPIXSEl2_PORT0SEL2    (0)
#define     NFP3800_SPI_SerialIOConfig_SPIXSEl2_PORT1SEL2    (1)
#define     NFP3800_SPI_SerialIOConfig_SPIXSEl2_PORT2SEL2    (2)
#define     NFP3800_SPI_SerialIOConfig_SPIXSEl2_PORT3SEL2    (3)
#define   NFP3800_SPI_SerialIOConfig_SPIXSEL1(_x)            (((_x) & 0x3) << 0)
#define   NFP3800_SPI_SerialIOConfig_SPIXSEL1_of(_x)         (((_x) >> 0) & 0x3)
#define     NFP3800_SPI_SerialIOConfig_SPIXSEL1_PORT0SEL1    (0)
#define     NFP3800_SPI_SerialIOConfig_SPIXSEL1_PORT1SEL1    (1)
#define     NFP3800_SPI_SerialIOConfig_SPIXSEL1_PORT2SEL1    (2)
#define     NFP3800_SPI_SerialIOConfig_SPIXSEL1_PORT3SEL1    (3)
/*----------------------------------------------------------------
  Register: SpiIOIdleStatus - IO pad idle status
    Bits: 31    SampleTrigger(rw) - This bit represents the current sample state. Write a 1 to trigger the sample process, wait until the bit reads as 0 before attempting to access other SPI registers.
    Bits: 17    SPIxD3Idle(rw) - This bit represents the state of the SPIxD3 io pin when not driven. During Sample this bit is set from the SPIxD3 IO pad state.
    Bits: 16    SPIxD2Idle(rw) - This bit represents the state of the SPIxD2 io pin when not driven. During Sample this bit is set from the SPIxD2 IO pad state.
    Bits: 15    SPIxHoldnIdle(rw) - This bit represents the state of the SPIxHOLDn io pin when not driven. During Sample this bit is set from the SPIxHOLDn IO pad state.
    Bits: 14    SPIxSel3Idle(rw) - This bit represents the state of the SPIxSEL3 io pin when not driven. During Sample this bit is set from the SPIxSEL3 IO pad state.
    Bits: 13    SPIxSel2Idle(rw) - This bit represents the state of the SPIxSEL2 io pin when not driven. During Sample this bit is set from the SPIxSEL2 IO pad state.
    Bits: 12    SPIxSel1Idle(rw) - This bit represents the state of the SPIxSEL1 io pin when not driven. During Sample this bit is set from the SPIxSEL1 IO pad state.
    Bits: 11    SPI3D0Idle(rw) - This bit represents the state of the SPI3D0 io pin when not driven. During Sample this bit is set from the SPI3D0 IO pad state.
    Bits: 10    SPI3ClkIdle(rw) - This bit represents the state of the SPI3CLK io pin when not driven. During Sample this bit is set from the SPI3CLK IO pad state.
    Bits:  9    SPI2D0Idle(rw) - This bit represents the state of the SPI2D0 io pin when not driven. During Sample this bit is set from the SPI2D0 IO pad state.
    Bits:  8    SPI2ClkIdle(rw) - This bit represents the state of the SPI2CLK io pin when not driven. During Sample this bit is set from the SPI2CLK IO pad state.
    Bits:  7    SPI1Sel0Idle(rw) - This bit represents the state of the SPI1SEL0 io pin when not driven. During Sample this bit is set from the SPI1SEL0 IO pad state.
    Bits:  6    SPI1D1Idle(rw) - This bit represents the state of the SPI1D1 io pin when not driven. During Sample this bit is set from the SPI1D1 IO pad state.
    Bits:  5    SPI1D0Idle(rw) - This bit represents the state of the SPI1D0 io pin when not driven. During Sample this bit is set from the SPI1D0 IO pad state.
    Bits:  4    SPI1ClkIdle(rw) - This bit represents the state of the SPI1CLK io pin when not driven. During Sample this bit is set from the SPI1CLK IO pad state.
    Bits:  3    SPI0Sel0Idle(rw) - This bit represents the state of the SPI0SEL0 io pin when not driven. During Sample this bit is set from the SPI0SEL0 IO pad state.
    Bits:  2    SPI0D1Idle(rw) - This bit represents the state of the SPI0D1 io pin when not driven. During Sample this bit is set from the SPI0D1 IO pad state.
    Bits:  1    SPI0D0Idle(rw) - This bit represents the state of the SPI0D0 io pin when not driven. During Sample this bit is set from the SPI0D0 IO pad state.
    Bits:  0    SPI0ClkIdle(rw) - This bit represents the state of the SPI0CLK io pin when not driven. During Sample this bit is set from the SPI0CLK IO pad state.
*/
#define NFP3800_SPI_SpiIOIdleStatus                          0x00000104
#define   NFP3800_SPI_SpiIOIdleStatus_SampleTrigger          BIT(31)
#define   NFP3800_SPI_SpiIOIdleStatus_SPIxD3Idle             BIT(17)
#define   NFP3800_SPI_SpiIOIdleStatus_SPIxD2Idle             BIT(16)
#define   NFP3800_SPI_SpiIOIdleStatus_SPIxHoldnIdle          BIT(15)
#define   NFP3800_SPI_SpiIOIdleStatus_SPIxSel3Idle           BIT(14)
#define   NFP3800_SPI_SpiIOIdleStatus_SPIxSel2Idle           BIT(13)
#define   NFP3800_SPI_SpiIOIdleStatus_SPIxSel1Idle           BIT(12)
#define   NFP3800_SPI_SpiIOIdleStatus_SPI3D0Idle             BIT(11)
#define   NFP3800_SPI_SpiIOIdleStatus_SPI3ClkIdle            BIT(10)
#define   NFP3800_SPI_SpiIOIdleStatus_SPI2D0Idle             BIT(9)
#define   NFP3800_SPI_SpiIOIdleStatus_SPI2ClkIdle            BIT(8)
#define   NFP3800_SPI_SpiIOIdleStatus_SPI1Sel0Idle           BIT(7)
#define   NFP3800_SPI_SpiIOIdleStatus_SPI1D1Idle             BIT(6)
#define   NFP3800_SPI_SpiIOIdleStatus_SPI1D0Idle             BIT(5)
#define   NFP3800_SPI_SpiIOIdleStatus_SPI1ClkIdle            BIT(4)
#define   NFP3800_SPI_SpiIOIdleStatus_SPI0Sel0Idle           BIT(3)
#define   NFP3800_SPI_SpiIOIdleStatus_SPI0D1Idle             BIT(2)
#define   NFP3800_SPI_SpiIOIdleStatus_SPI0D0Idle             BIT(1)
#define   NFP3800_SPI_SpiIOIdleStatus_SPI0ClkIdle            BIT(0)
/*----------------------------------------------------------------
  Register: SpiWriteEnableMask - Write enable mask for virtual port configuration registers
    Bits: 31:0  WriteEnableMask(rw) - This register permits writes to the virtual port configuration registers without writing all 32 bits. Bit-for-bit, if the mask bit is a 1 the target register bit is written, if the mask bit is a 0 the target register bit is unaffected. Also see SerialWriteEnableTarget register.
*/
#define NFP3800_SPI_SpiWriteEnableMask                       0x00000108
#define   NFP3800_SPI_SpiWriteEnableMask_WriteEnableMask(_x) (((_x) & 0xffffffff) << 0)
#define   NFP3800_SPI_SpiWriteEnableMask_WriteEnableMask_of(_x) (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: SpiWriteEnableTarget - Write enable target for virtual port configuration registers
    Bits:  4    WriteEnableAvailable(rw) - This bit set indicates the write enable logic is available for use. When the WriteEnableAvailable bit is already set, or a write will set it, the write to this register will be able to set the WriteEnableTarget. When neither case is true, writes to this register have no affect, so this bit acts as a semaphore. A process may attempt to acquire this resource by writing this bit to zero and the target field as desired. If the write succeeds reading this register will return what was written, if not then no action is needed beyond trying again later. To release the resource write this bit to one.
    Bits:  3:0  WriteEnableTarget(rw) - This field specifies the target virtual port that the WriteEnableMask will apply to if the WriteEnableAvailable bit is clear.
*/
#define NFP3800_SPI_SpiWriteEnableTarget                     0x0000010c
#define   NFP3800_SPI_SpiWriteEnableTarget_WriteEnableAvailable BIT(4)
#define   NFP3800_SPI_SpiWriteEnableTarget_WriteEnableTarget(_x) (((_x) & 0xf) << 0)
#define   NFP3800_SPI_SpiWriteEnableTarget_WriteEnableTarget_of(_x) (((_x) >> 0) & 0xf)
/*----------------------------------------------------------------
  Register: PhysicalPort[4]ManualConfiguration - Manual configuration for physical port X
    Bits: 31:30 ClockDelay(rw) - This field controls the delay of the clock sent out the IO pad.
    Bits: 23:20 DataOutDelay(rw) - Specify the number of internal clocks to delay the output data relative to the SPI clock when DataSync is set.
    Bits: 19:16 DataInDelay(rw) - Specify the number of internal clocks to delay the sampling of input data relative to the SPI clock when DataSync is set. This value is offset by 3 P_CLK periods (low values sample the input data before the clock edge).
    Bits: 15:14 DataOutSyncMode(rw) - This field controls the synchronous and asynchronous modes of the data outputs.
    Bits: 13:12 DataInSyncMode(rw) - This field controls the synchronous and asynchronous modes of the data inputs.
*/
#define NFP3800_SPI_PhysicalPortManualConfiguration(_x)      (0x00000110 + (0x4 * ((_x) & 0x3)))
#define   NFP3800_SPI_PhysicalPortManualConfiguration_ClockDelay(_x) (((_x) & 0x3) << 30)
#define   NFP3800_SPI_PhysicalPortManualConfiguration_ClockDelay_of(_x) (((_x) >> 30) & 0x3)
#define     NFP3800_SPI_PhysicalPortManualConfiguration_ClockDelay_Delay_0 (0)
#define     NFP3800_SPI_PhysicalPortManualConfiguration_ClockDelay_Delay_8 (1)
#define     NFP3800_SPI_PhysicalPortManualConfiguration_ClockDelay_Delay_16 (2)
#define     NFP3800_SPI_PhysicalPortManualConfiguration_ClockDelay_Delay_24 (3)
#define   NFP3800_SPI_PhysicalPortManualConfiguration_DataOutDelay(_x) (((_x) & 0xf) << 20)
#define   NFP3800_SPI_PhysicalPortManualConfiguration_DataOutDelay_of(_x) (((_x) >> 20) & 0xf)
#define   NFP3800_SPI_PhysicalPortManualConfiguration_DataInDelay(_x) (((_x) & 0xf) << 16)
#define   NFP3800_SPI_PhysicalPortManualConfiguration_DataInDelay_of(_x) (((_x) >> 16) & 0xf)
#define   NFP3800_SPI_PhysicalPortManualConfiguration_DataOutSyncMode(_x) (((_x) & 0x3) << 14)
#define   NFP3800_SPI_PhysicalPortManualConfiguration_DataOutSyncMode_of(_x) (((_x) >> 14) & 0x3)
#define     NFP3800_SPI_PhysicalPortManualConfiguration_DataOutSyncMode_SYNC (0)
#define     NFP3800_SPI_PhysicalPortManualConfiguration_DataOutSyncMode_RISE (1)
#define     NFP3800_SPI_PhysicalPortManualConfiguration_DataOutSyncMode_FALL (2)
#define     NFP3800_SPI_PhysicalPortManualConfiguration_DataOutSyncMode_DDR (3)
#define   NFP3800_SPI_PhysicalPortManualConfiguration_DataInSyncMode(_x) (((_x) & 0x3) << 12)
#define   NFP3800_SPI_PhysicalPortManualConfiguration_DataInSyncMode_of(_x) (((_x) >> 12) & 0x3)
#define     NFP3800_SPI_PhysicalPortManualConfiguration_DataInSyncMode_SYNC (0)
#define     NFP3800_SPI_PhysicalPortManualConfiguration_DataInSyncMode_RISE (1)
#define     NFP3800_SPI_PhysicalPortManualConfiguration_DataInSyncMode_FALL (2)
#define     NFP3800_SPI_PhysicalPortManualConfiguration_DataInSyncMode_DDR (3)

#endif /* NFP3800_LONGNAMES */


#endif /* NFP3800_SPI_H */
