/*
 * Copyright (C) 2009-2013 Netronome Systems, Inc.  All rights reserved.
 */

/////////////////////////////////////////////////////////////////////////////
//
//      File Name: nfp3200_hardware.h
//
//      Purpose: NFP3200 definitions that can be used in all microblocks.
//
/////////////////////////////////////////////////////////////////////////////

#ifndef __NFP3200_HARDWARE_H__
#define __NFP3200_HARDWARE_H__

// MSF_TX_CONTROL register field values

#define RBUF_ELEMENT_SIZE_64    0x0     // 2 bits field -  64 bytes per element
#define RBUF_ELEMENT_SIZE_128   0x1     // 2 bits field - 128 bytes per element
#define RBUF_ELEMENT_SIZE_256   0x2     // 2 bits field - 156 bytes per element

#define CSIX_FREELIST_DIFF      0x0
#define CSIX_FREELIST_SAME      0x1

#define TX_CBUS_MODE_SIMPLEX    0x0
#define TX_CBUS_MODE_DUPLEX     0x1

#define RX_MPHY_POLL_MODE_DRCT  0x0     // FIFO status is carried on the RXFA[3:0], only MPHY-4
#define RX_MPHY_POLL_MODE_POLL  0x1     // FIFO status must be polled and is carried on the RXFA
                                        // pin can be used in MPHY-4 or MPHY-6

#define RX_MPHY_MODE_4          0x0     // Maximum of four ports (default)
#define RX_MPHY_MODE_16         0x1     // Maximum of sixteen ports

#define RX_MPHY_EN_SPHY         0x0     // Single PHY (default)
#define RX_MPHY_EN_MPHY         0x1     // Multi PHY

#define RX_WIDTH_1_32           0x0     // Default width
#define RX_WIDTH_2_16           0x1
#define RX_WIDTH_4_8            0x2
#define RX_WIDTH_1_16_2_8       0x3

#define RX_MODE_UOPIA_POS       0x0     // Default UTOPIA/POS mode
#define RX_MODE_CSIX            0x1     // CSIX mode

#define RX_EN_0_ALL             0x1     // bit 28 Channel 0 (SPHY) or all channels (MPHY)
#define RX_EN_1                 0x1     // bit 29 Channel 1 (SPHY) not used in MPHY mode
#define RX_EN_2                 0x1     // bit 30 Channel 2 (SPHY) not used in MPHY mode
#define RX_EN_3                 0x1     // bit 31 Channel 3 (SPHY) not used in MPHY mode

#define RBUF_ELEMENT_SIZE_BITS  2
#define CSIX_FREELIST_BITS      9
#define TX_CBUS_MODE_BITS       16
#define RX_MPHY_POLL_MODE_BITS  17
#define RX_MPHY_MODE_BITS       18
#define RX_MPHY_EN_BITS         19
#define RX_WIDTH_BITS           20
#define RX_MODE_BITS            22
#define RX_EN_0_ALL_BITS        28
#define RX_EN_1_BITS            29
#define RX_EN_2_BITS            30
#define RX_EN_3_BITS            31

//  MSF_TX_CONTROL register field values

#define TBUF_ELEMENT_SIZE_64    0x0     // 2 bits field -  64 bytes per element
#define TBUF_ELEMENT_SIZE_128   0x1     // 2 bits field - 128 bytes per element
#define TBUF_ELEMENT_SIZE_256   0x2     // 2 bits field - 256 bytes per element

#define RX_CBUS_MODE_SIMPLEX    0x0
#define RX_CBUS_MODE_DUPLEX     0x1

#define TX_MPHY_POLL_MODE_DRCT  0x0     // FIFO status is carred on the TXFA[3:0], only  MPHY-4
#define TX_MPHY_POLL_MODE_POLL  0x1     // FIFO status must be polled and is carred on the
                                        // TXFA pin. which can be used in MPHY-4 or MPHY-6

#define TX_MPHY_MODE_4          0x0     // Default mode, a maximum of four ports
#define TX_MPHY_MODE_16         0x1     // a maximum of sixteen ports

#define TX_MPHY_EN_SPHY         0x0     // Single PHY (default)
#define TX_MPHY_EN_MPHY         0x1     // Multi PHY

#define TX_WIDTH_1_32           0x0     // Default width
#define TX_WIDTH_2_16           0x1
#define TX_WIDTH_4_8            0x2
#define TX_WIDTH_1_16_2_8       0x3

#define TX_MODE_UOPIA_POS       0x0     // Default UTOPIA/POS mode
#define TX_MODE_CSIX            0x1     // CSIX mode

#define TX_FLUSH_3              0x1     // TX_FLUSH[3] for TBUF partition 3
#define RX_FLUSH_2              0x1     // TX_FLUSH[2] for TBUF partition 2
#define RX_FLUDH_1              0x1     // TX_FLUSH[1] for TBUF partition 1
#define RX_FLUSH_0              0x1     // TX_FLUSH[0] for TBUF partition 0

#define TX_EN_0                 0x1     // TBUF segment 0
#define TX_EN_1                 0x1     // TBUF segment 1
#define TX_EN_2                 0x1     // TBUF segment 2
#define TX_EN_3                 0x1     // TBUF segment 3

#define TBUF_ELEMENT_SIXZE_BITS 2
#define RX_CBUS_MODE_BITS       16
#define TX_MPHY_POLL_MODE_BITS  17
#define TX_MPHY_MODE_BITS       18
#define TX_MPHY_EN_BITS         19
#define TX_WIDTH_BITS           20
#define TX_MODE_BITS            22
#define TX_EN_0__ALL_BITS       28
#define TX_EN_1_BITS            29
#define TX_EN_2_BITS            30
#define TX_EN_3_BITS            31

//  MSF_Interrupt_Status register field values

#define HP_ERROR                0x00000001
#define VP_ERROR                0x00000002
#define TBUF_ERROR              0x00000010
#define FCIFIFO_ERROR           0x00000040
#define RBUF_OVFL_COUNT         0x0000FF00
#define FCEFIFO_OVFL            0x00FF0000


#define HP_EROOR_BITS           0
#define VP_ERROR_BITS           1
#define TBUF_ERROR_BITS         4
#define FCIFIFO_ERROR_BITS      6
#define RBUF_OV_CO_BITS         8
#define FCEFIFO_OV_BITS         16


//  MSF_Interrupt_Enable register field values

#define HP_ERROR_EN             0x00000001
#define VP_ERROR_EN             0x00000002
#define TBUF_ERROR_EN           0x00000010
#define FCIFIFO_ERROR_EN        0x00000040
#define RBUF_OVFL_COUNT_EN      0x00000100
#define FCEFIFO_OVFL_EN         0x00010000

#define HP_EROOR_EN_BITS        0
#define VP_ERROR_EN_BITS        1
#define TBUF_ERROR_EN_EN_BITS   4
#define FCIFIFO_ERROR_EN_BITS   6
#define RBUF_OV_CO_EN_BITS      8
#define FCEFIFO_OV_EN_BITS      16

//  CSIX Type map register field values

#define CSIX_TYPE_MAP_0         0x00000003
#define CSIX_TYPE_MAP_1         0x0000000c
#define CSIX_TYPE_MAP_2         0x00000030
#define CSIX_TYPE_MAP_3         0x000000c0
#define CSIX_TYPE_MAP_4         0x00000300
#define CSIX_TYPE_MAP_5         0x00000c00
#define CSIX_TYPE_MAP_6         0x00003000
#define CSIX_TYPE_MAP_7         0x0000c000
#define CSIX_TYPE_MAP_8         0x00030000
#define CSIX_TYPE_MAP_9         0x000c0000
#define CSIX_TYPE_MAP_10        0x00300000
#define CSIX_TYPE_MAP_11        0x00c00000
#define CSIX_TYPE_MAP_12        0x03000000
#define CSIX_TYPE_MAP_13        0x0c000000
#define CSIX_TYPE_MAP_14        0x30000000
#define CSIX_TYPE_MAP_15        0xc0000000

#define CSIX_TYPE_MAP_0_BITS    0
#define CSIX_TYPE_MAP_1_BITS    2
#define CSIX_TYPE_MAP_2_BITS    4
#define CSIX_TYPE_MAP_3_BITS    6
#define CSIX_TYPE_MAP_4_BITS    8
#define CSIX_TYPE_MAP_5_BITS    10
#define CSIX_TYPE_MAP_6_BITS    12
#define CSIX_TYPE_MAP_7_BITS    14
#define CSIX_TYPE_MAP_8_BITS    16
#define CSIX_TYPE_MAP_9_BITS    18
#define CSIX_TYPE_MAP_10_BITS   20
#define CSIX_TYPE_MAP_11_BITS   22
#define CSIX_TYPE_MAP_12_BITS   24
#define CSIX_TYPE_MAP_13_BITS   26
#define CSIX_TYPE_MAP_14_BITS   28
#define CSIX_TYPE_MAP_15_BITS   30

//  FC_EGRESS_STATUS register field masks and bit numbers
//  This is a read only register.

#define EGRESS_SF_CREADY        0x00000001
#define EGRESS_SF_DREADY        0x00000002
#define EGRESS_TM_CREADY        0x00000004
#define EGRESS_TM_DREADY        0x00000008
#define EGRESS_FCEFIFO_FULL     0x00000010

#define EGRESS_SF_CREADY_BITS       0
#define EGRESS_SF_DREADY_BITS       1
#define EGRESS_TM_CREADY_BITS       2
#define EGRESS_TM_DREADY_BITS       3
#define EGRESS_FCEFIFO_FULL_BITS    4


//  FC_INGRESS_STATUS register field masks and bit numbers
//  This is a read only register.

#define INGRESS_SF_CREADY       0x00000001
#define INGRESS_SF_DREADY       0x00000002
#define INGRESS_TM_CREADY       0x00000004
#define INGRESS_TM_DREADY       0x00000008

#define INGRESS_SF_CREADY_BITS      0
#define INGRESS_SF_DREADY_BITS      1
#define INGRESS_TM_CREADY_BITS      2
#define INGRESS_TM_DREADY_BITS      3

//  TX_validate register field masks and bit numbers

#define ELEMENT_NUMBER          0x00000007F

//  HWM register field values

#define RBUF_C_HWM_3_4          0x0     // 3/4 entries
#define RBUF_C_HWM_1_2          0x1     // 1/2 entries
#define RBUF_C_HWM_1_4          0x2     // 1/4 entries
#define RBUF_C_HWM_1_8          0x3     // 1/8 entries
#define RBUF_D_HWM_3_4          0x0     // 3/4 entries
#define RBUF_D_HWM_1_2          0x1     // 1/2 entries
#define RBUF_D_HWM_1_4          0x2     // 1/4 entries
#define RBUF_D_HWM_1_8          0x3     // 1/8 entries
#define FCEFIFO_HWM_3_4         0x0     // 3/4 CWords
#define FCEFIFO_HWM_1_2         0x1     // 1/2 CWords
#define FCEFIFO_HWM_1_4         0x2     // 1/4 CWords
#define FCEFIFO_HWM_1_8         0x3     // 1/8 CWords
#define FCEFIFO_INT_HWM_3_4     0x0     // 3/4 CWords
#define FCEFIFO_INT_HWM_1_2     0x1     // 1/2 CWords
#define FCEFIFO_INT_HWM_1_4     0x2     // 1/4 CWords
#define FCEFIFO_INT_HWM_1_8     0x3     // 1/8 CWords
#define FCEFIFO_EXT_HWM_3_4     0x0     // 3/4 CWords
#define FCEFIFO_EXT_HWM_1_2     0x1     // 1/2 CWords
#define FCEFIFO_EXT_HWM_1_4     0x2     // 1/4 CWords
#define FCEFIFO_EXT_HWM_1_8     0x3     // 1/8 CWords

#define RBUF_C_HWM_BITS         0
#define RBUF_D_HWM_BITS         2
#define FCEFIFO_HWM_BITS        6
#define FCEFIFO_INT_HWM_BITS    8
#define FCEFIFO_EXT_HWM_BITS    10

//  SRB_Override register field values

#define RXCSRB_FORCE_SF_CRDY    0x3     // SF_CRDY
#define RXCSRB_FORCE_SF_DRDY    0x2     // SF_DRDY
#define RXCSRB_FORCE_TM_CRDY    0x1     // TM_CRDY
#define RXCSRB_FORCE_TM_DRDY    0x0     // TM_DRDY
#define TXCSRB_FORCE_SF_CRDY    0x7     // SF_CRDY
#define TXCSRB_FORCE_SF_DRDY    0x6     // SF_DRDY
#define TXCSRB_FORCE_TM_CRDY    0x5     // TM_CRDY
#define TXCSRB_FORCE_TM_DRDY    0x4     // TM_DRDY
#define RXCSRB_FORCE_EN_INPUT   0x0
#define RXCSRB_FORCE_EN_FORCE   0x1
#define TXCSRB_FORCE_EN_INPUT   0x0
#define TXCSRB_FORCE_EN_FORCE   0x1

#define RXCSRB_FORCE_BITS       0
#define TXCSRB_FORCE_BITS       4
#define RXCSRB_FORCE_EN_BITS    8
#define TXCSRB_FORCE_EN_BITS    9

//  RX_THREAD_FREELIST register bit numbers
//  This is a write only register.

#define RXTF_SIGNAL             12      // 12-15 Signal number
#define RXTF_ME                 7       // 7-9   Microengine number
#define RXTF_THREAD             4       // 4-6   Thread number
#define EXTF_XFER               0       // 0-3   Transfer register number

//  RBUF_Element_done register field

#define ELEMENT_TO_FREE         0x07F   // Indicates the number of the element to free

//  RX_MPHY_Poll_limit register field mask

#define RX_POLL_LIMIT_MASK      0xF     // Highest port number

//  FCEFIFO_validate register

#define UNUSED_MASK             0xFFFFFFFF

//  Rx_Thread_Freelist_Timeout register mask

#define TIMEOUT_INTERVAL_MASK       0x0x1FFF

//  Tx_Sequence register field mask

#define SEQUENCE_MASK               0xFF
#define SEQUENCE_BITS               0
#define EMPTY_BITS                  31

//  Tx_MPHY_Poll_Limit register fields

#define TX_POLL_LIMIT_MASK          0xF

//  Tx_MPHY_Status register fields

#define TX_STATUS_MASK              0xFFFF

#define TX_STATUS_BITS              0
#define TX_PENDING_BITS             16      // 16 to 31 => 16 bits

//  Rx_UP_Control register fields

#define RX_DR_TIME_1_CLOCK          0x0
#define RX_DR_TIME_2_CLOCK          0x1
#define RX_CELL_SIZE_52_BYTES       0x0
#define RX_CELL_SIZE_OTHER_BYTES    0x1     // 53(X8) 54(X16) or 56(X32) byte cell
#define RX_PARITY_NONE              0x0
#define RX_PARITY_ODD               0x2
#define RX_PARITY_EVEN              0x3
#define RX_CP_MODE_CELL             0x0     // Cell UTOPIA
#define RX_CP_MODE_PACKET           0x1     // Packet POS

#define RX_DR_TIME_BITS             0
#define RX_CELL_SIZE_BITS           1
#define RX_PARITY_BITS              2
#define RX_CP_MODE_BITS             4

//  Tx_UP_Control register fields

#define TX_DR_TIME_1_CLOCK          0x0
#define TX_DR_TIME_2_CLOCK          0x1
#define TX_CELL_SIZE_52_BYTES       0x0
#define TX_CELL_SIZE_OTHER_BYTES    0x1     // 53(X8) 54(X16) or 56(X32) byte cell
#define TX_PARITY_NONE              0x0
#define TX_PARITY_ODD               0x2
#define TX_PARITY_EVEN              0x3
#define TX_CP_MODE_CELL             0x0     // Cell UTOPIA
#define TX_CP_MODE_PACKET           0x1     // Packet POS

#define TX_DR_TIME_BITS             0
#define TX_CELL_SIZE_BITS           1
#define TX_PARITY_BITS              2
#define TX_CP_MODE_BITS             4

//  FCIFIFO register fields

#define FCIFIFO_CWORD_MASK          0xFFFFFFFF

//  FCEFIFO register fields

#define FCEFIFO_CWORD_MASK          0xFFFFFFFF

//  RX_THREAD_FREELIST register field bits
//  This is a write only register.

#define SIGNAL_NUMBER_BITS          12
#define ME_NUMBER_BITS              7
#define THREAD_BITS                 4
#define TRNSFER_REG_BITS            0

/////////////////////////////////////////////////////////////////////////////

//  MSF register addresses

#define MSF_RX_CNTL                 0x0000
#define MSF_TX_CNTL                 0x0004
#define MSF_INTERRUPT_STATUS        0x0008
#define MSF_INTERRUPT_ENABLE        0x000c
#define CSIX_TYPE_MAP               0x0010
#define FC_EGRESS_STATUS            0x0014
#define FC_INGRESS_STATUS           0x0018
#define MSF_RESERVED_1              0x001c
#define TX_VALIDATE                 0x0020
#define HWM_CONTROL                 0x0024
#define SRB_OVERRIDE                0x0028
#define MSF_RESERVED_2              0x002c
#define RX_THREAD_FREELIST_0        0x0030
#define RX_THREAD_FREELIST_1        0x0034
#define RX_THREAD_FREELIST_2        0x0038
#define RX_THREAD_FREELIST_3        0x003c
#define RX_PORT_MAP                 0x0040
#define RBUF_ELEMENT_DONE           0x0044
#define RX_MPHY_POLL_LIMIT          0x0048
#define FCEFIFO_VALIDATE            0x004c
#define RX_THREAD_FREELIST_TO_0     0x0050
#define RX_THREAD_FREELIST_TO_1     0x0054
#define RX_THREAD_FREELIST_TO_2     0x0058
#define RX_THREAD_FREELIST_TO_3     0x005c
#define TX_SEQUENCE_0               0x0060
#define TX_SEQUENCE_1               0x0064
#define TX_SEQUENCE_2               0x0068
#define TX_SEQUENCE_3               0x006c
#define TX_CALENDAR_LENGTH          0x0070
#define TX_MPHY_STATUS              0x0074
#define MSF_RESERVED_4              0x0078
#define MSF_RESERVED_5              0x007c
#define RX_UP_CTRL_0                0x0080
#define RX_UP_CTRL_1                0x0084
#define RX_UP_CTRL_2                0x0088
#define RX_UP_CTRL_3                0x008c
#define TX_UP_CTRL_0                0x0090
#define TX_UP_CTRL_1                0x0094
#define TX_UP_CTRL_2                0x0098
#define TX_UP_CTRL_3                0x009c
#define FCIFIFO                     0x0100      // To 0x013c 16 address for burst access
#define FCEFIFO                     0x0140      // To 0x017c 16 address for burst access
#define TX_MULTIPLE_PORT_STATUS_0   0x0380
#define TX_CALENDAR_0               0x1000
#define TX_PORT_STATUS_0            0x1400
#define TBUF_ELEMENT_CTRL_VALID     0x1800      // To 0x1BFC TBUF_Element_Control_#
#define RBUF_TBUF_BASE              0x2000      // To 0x3FFF Read =RBUF Write=TBUF

#endif /* __NFP3200_HARDWARE_H__ */
