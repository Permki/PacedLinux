/*
 * Copyright (C) 2013, Netronome Systems, Inc.  All rights reserved.
 *
 * Disclaimer: this file is provided without any warranty
 * as part of an early-access program, and the content is
 * bound to change before the final release.
 */

#ifndef NFP6000_ISLAND_H
#define NFP6000_ISLAND_H

/*================================================================*/

/*----------------------------------------------------------------*/
/* XPB Address Map (XPB_BASE = NFP_XPB_OVERLAY(island))           */

/* XPB_BASE + NFP_ISLAND_* */
/*================================================================*/


/* HGID: 60515d2e075a */

#ifndef NFP6000_LONGNAMES

/*----------------------------------------------------------------
  Register: EventFilterMask[8] - Event Filter Mask Reg
    Bits: 23:0  EventMask(rw) - Mask for which of the event bits must be set; see also the Peripheral Event Manager FilterMask
*/
#define NFP_ISLAND_FILT_MASK(_x)                             (0x00044000 + (0x8 * ((_x) & 0x7)))
#define   NFP_ISLAND_FILT_MASK_MASK(_x)                      (((_x) & 0xffffff) << 0)
#define   NFP_ISLAND_FILT_MASK_MASK_of(_x)                   (((_x) >> 0) & 0xffffff)
/*----------------------------------------------------------------
  Register: EventFilterMatch[8] - Event Filter Match Reg
    Bits: 23:0  EventMatch(rw) - Values of the event bits, masked by FilterMask, which must be set for the event to be 'filtered' onto the global event chain; see also the Peripheral Event Manager EventFilterMatch
*/
#define NFP_ISLAND_FILT_MATCH(_x)                            (0x00044004 + (0x8 * ((_x) & 0x7)))
#define   NFP_ISLAND_FILT_MATCH_MATCH(_x)                    (((_x) & 0xffffff) << 0)
#define   NFP_ISLAND_FILT_MATCH_MATCH_of(_x)                 (((_x) >> 0) & 0xffffff)
/*----------------------------------------------------------------
  Register: EventRouteConfig - Event plane routing configuration, for local ring and global chain as it passes through the island
    Bits: 23    BypassInterior(rw) - Bypass Mux - for bypassing the interior of the island.
    Bits: 22:21 GlobalInjection(rw) - Selects to which corner overlay port in the event bus plane to send events filtered from the local chain as 'global'
    Bits: 20:19 GlobalMux(rw) - For the ARM island only, selects which overlay corner input port drives the global event bus into the ARM for use with the global event manger
    Bits: 18:16 IntInput(rw) - Selects which event plane input (from one of the 4 corners of the island or from the island interior) to use as the input to the interior output mux to drive the overlay interior output
    Bits: 15:13 BRInput(rw) - Selects which event plane input (from one of the 4 corners of the island or from the island interior) to use as the input to the bottom-right corner output mux to drive the overlay bottom-right corner output
    Bits: 12:10 BLInput(rw) - Selects which event plane input (from one of the 4 corners of the island or from the island interior) to use as the input to the bottom-left corner output mux to drive the overlay bottom-left corner output
    Bits:  9:7  TRInput(rw) - Selects which event plane input (from one of the 4 corners of the island or from the island interior) to use as the input to the top-right corner output mux to drive the overlay top-right corner output
    Bits:  6:4  TLInput(rw) - Selects which event plane input (from one of the 4 corners of the island or from the island interior) to use as the input to the top-left corner output mux to drive the overlay top-left corner output
    Bits:  3    RouteBR(rw) - REMOVED NOW DO NOT USE If asserted, route the bottom-right island ports in/out of the center; if clear, engage loopbacks (external loop to external, internal to internal)
    Bits:  2    RouteBL(rw) - REMOVED NOW DO NOT USE If asserted, route the bottom-left island ports in/out of the center; if clear, engage loopbacks (external loop to external, internal to internal)
    Bits:  1    RouteTR(rw) - REMOVED NOW DO NOT USE If asserted, route the top-right island ports in/out of the center; if clear, engage loopbacks (external loop to external, internal to internal)
    Bits:  0    RouteTL(rw) - REMOVED NOW DO NOT USE If asserted, route the top-left island ports in/out of the center; if clear, engage loopbacks (external loop to external, internal to internal)
*/
#define NFP_ISLAND_EVT_CFG                                   0x00044040
#define   NFP_ISLAND_EVT_CFG_BYPASS_MUX                      BIT(23)
#define     NFP_ISLAND_EVT_CFG_BYPASS_MUX_NORMAL             (0 << 23)
#define     NFP_ISLAND_EVT_CFG_BYPASS_MUX_BYPASS             BIT(23)
#define   NFP_ISLAND_EVT_CFG_CHKPT_NODE_SEL(_x)              (((_x) & 0x3) << 21)
#define   NFP_ISLAND_EVT_CFG_CHKPT_NODE_SEL_of(_x)           (((_x) >> 21) & 0x3)
#define     NFP_ISLAND_EVT_CFG_CHKPT_NODE_SEL_TL             (0)
#define     NFP_ISLAND_EVT_CFG_CHKPT_NODE_SEL_TR             (1)
#define     NFP_ISLAND_EVT_CFG_CHKPT_NODE_SEL_BL             (2)
#define     NFP_ISLAND_EVT_CFG_CHKPT_NODE_SEL_BR             (3)
#define   NFP_ISLAND_EVT_CFG_GLOBAL_MUX(_x)                  (((_x) & 0x3) << 19)
#define   NFP_ISLAND_EVT_CFG_GLOBAL_MUX_of(_x)               (((_x) >> 19) & 0x3)
#define     NFP_ISLAND_EVT_CFG_GLOBAL_MUX_TL                 (0)
#define     NFP_ISLAND_EVT_CFG_GLOBAL_MUX_TR                 (1)
#define     NFP_ISLAND_EVT_CFG_GLOBAL_MUX_BL                 (2)
#define     NFP_ISLAND_EVT_CFG_GLOBAL_MUX_BR                 (3)
#define   NFP_ISLAND_EVT_CFG_MUX_INT(_x)                     (((_x) & 0x7) << 16)
#define   NFP_ISLAND_EVT_CFG_MUX_INT_of(_x)                  (((_x) >> 16) & 0x7)
#define     NFP_ISLAND_EVT_CFG_MUX_INT_TL                    (0)
#define     NFP_ISLAND_EVT_CFG_MUX_INT_TR                    (1)
#define     NFP_ISLAND_EVT_CFG_MUX_INT_BL                    (2)
#define     NFP_ISLAND_EVT_CFG_MUX_INT_BR                    (3)
#define     NFP_ISLAND_EVT_CFG_MUX_INT_INTERIOR              (4)
#define   NFP_ISLAND_EVT_CFG_MUX_BR2(_x)                     (((_x) & 0x7) << 13)
#define   NFP_ISLAND_EVT_CFG_MUX_BR2_of(_x)                  (((_x) >> 13) & 0x7)
#define     NFP_ISLAND_EVT_CFG_MUX_BR2_TL                    (0)
#define     NFP_ISLAND_EVT_CFG_MUX_BR2_TR                    (1)
#define     NFP_ISLAND_EVT_CFG_MUX_BR2_BL                    (2)
#define     NFP_ISLAND_EVT_CFG_MUX_BR2_BR                    (3)
#define     NFP_ISLAND_EVT_CFG_MUX_BR2_INTERIOR              (4)
#define   NFP_ISLAND_EVT_CFG_MUX_BL2(_x)                     (((_x) & 0x7) << 10)
#define   NFP_ISLAND_EVT_CFG_MUX_BL2_of(_x)                  (((_x) >> 10) & 0x7)
#define     NFP_ISLAND_EVT_CFG_MUX_BL2_TL                    (0)
#define     NFP_ISLAND_EVT_CFG_MUX_BL2_TR                    (1)
#define     NFP_ISLAND_EVT_CFG_MUX_BL2_BL                    (2)
#define     NFP_ISLAND_EVT_CFG_MUX_BL2_BR                    (3)
#define     NFP_ISLAND_EVT_CFG_MUX_BL2_INTERIOR              (4)
#define   NFP_ISLAND_EVT_CFG_MUX_TR2(_x)                     (((_x) & 0x7) << 7)
#define   NFP_ISLAND_EVT_CFG_MUX_TR2_of(_x)                  (((_x) >> 7) & 0x7)
#define     NFP_ISLAND_EVT_CFG_MUX_TR2_TL                    (0)
#define     NFP_ISLAND_EVT_CFG_MUX_TR2_TR                    (1)
#define     NFP_ISLAND_EVT_CFG_MUX_TR2_BL                    (2)
#define     NFP_ISLAND_EVT_CFG_MUX_TR2_BR                    (3)
#define     NFP_ISLAND_EVT_CFG_MUX_TR2_INTERIOR              (4)
#define   NFP_ISLAND_EVT_CFG_MUX_TL2(_x)                     (((_x) & 0x7) << 4)
#define   NFP_ISLAND_EVT_CFG_MUX_TL2_of(_x)                  (((_x) >> 4) & 0x7)
#define     NFP_ISLAND_EVT_CFG_MUX_TL2_TL                    (0)
#define     NFP_ISLAND_EVT_CFG_MUX_TL2_TR                    (1)
#define     NFP_ISLAND_EVT_CFG_MUX_TL2_BL                    (2)
#define     NFP_ISLAND_EVT_CFG_MUX_TL2_BR                    (3)
#define     NFP_ISLAND_EVT_CFG_MUX_TL2_INTERIOR              (4)
#define   NFP_ISLAND_EVT_CFG_RBR                             BIT(3)
#define   NFP_ISLAND_EVT_CFG_RBL                             BIT(2)
#define   NFP_ISLAND_EVT_CFG_RTR                             BIT(1)
#define   NFP_ISLAND_EVT_CFG_RTL                             BIT(0)
/*----------------------------------------------------------------
  Register: ClockResetControl - Clock and reset control for the island
    Bits: 31:24 ClockEnable(rw) - Set to enable the clock for a clock domain. Each bit (0-7) applies to each reset/clock domain (0-7) for the current island.
    Bits: 23:16 RemoveReset(rw) - Set to deassert soft reset for a clock/reset domain (unless the global soft reset signal is asserted and not maksed). Each bit (0-7) applies to each reset/clock domain (0-7) for the current island.
    Bits: 15:8  ResetMask(rw) - When set, masks (ignores) the Global soft reset signal. Each bit (0-7) applies to each reset/clock domain (0-7) for the current island.
    Bits:  7:0  DomainLock(rw) - When set, prevents modifying the corresponding bits in registers OvlClkRstControl and OvlMemControl for the current island. For instance, when bit 0 of Lock is set, the settings in OvlClkRstControl and OvlMemControl corresponding to reset/clock domain 0 cannot be modified.
*/
#define NFP_ISLAND_RST_CLK_CTRL0                             0x00045400
#define   NFP_ISLAND_RST_CLK_CTRL0_CLK_EN(_x)                (((_x) & 0xff) << 24)
#define   NFP_ISLAND_RST_CLK_CTRL0_CLK_EN_of(_x)             (((_x) >> 24) & 0xff)
#define     NFP_ISLAND_RST_CLK_CTRL0_CLK_EN_CLK_DIS          (0)
#define     NFP_ISLAND_RST_CLK_CTRL0_CLK_EN_CLK_EN           (1)
#define   NFP_ISLAND_RST_CLK_CTRL0_MANUAL_SRESET(_x)         (((_x) & 0xff) << 16)
#define   NFP_ISLAND_RST_CLK_CTRL0_MANUAL_SRESET_of(_x)      (((_x) >> 16) & 0xff)
#define     NFP_ISLAND_RST_CLK_CTRL0_MANUAL_SRESET_SRESET    (0)
#define     NFP_ISLAND_RST_CLK_CTRL0_MANUAL_SRESET_NORMAL    (1)
#define   NFP_ISLAND_RST_CLK_CTRL0_SRESET_MASK(_x)           (((_x) & 0xff) << 8)
#define   NFP_ISLAND_RST_CLK_CTRL0_SRESET_MASK_of(_x)        (((_x) >> 8) & 0xff)
#define     NFP_ISLAND_RST_CLK_CTRL0_SRESET_MASK_NORMAL      (0)
#define     NFP_ISLAND_RST_CLK_CTRL0_SRESET_MASK_MASK        (1)
#define   NFP_ISLAND_RST_CLK_CTRL0_LOCK(_x)                  (((_x) & 0xff) << 0)
#define   NFP_ISLAND_RST_CLK_CTRL0_LOCK_of(_x)               (((_x) >> 0) & 0xff)
#define     NFP_ISLAND_RST_CLK_CTRL0_LOCK_NORMAL             (0)
#define     NFP_ISLAND_RST_CLK_CTRL0_LOCK_LOCKED             (1)
/*----------------------------------------------------------------
  Register: MemControl - Island-level memory control, for sleep etc
    Bits:  7:0  Enable(rw) - Set to enable memories in the reset/clock domains. Each bit (0-7) applies to each reset/clock domain (0-7) for the current island.
*/
#define NFP_ISLAND_RST_CLK_CTRL1                             0x00045404
#define   NFP_ISLAND_RST_CLK_CTRL1_MEM_SHUT_DOWN(_x)         (((_x) & 0xff) << 0)
#define   NFP_ISLAND_RST_CLK_CTRL1_MEM_SHUT_DOWN_of(_x)      (((_x) >> 0) & 0xff)
#define     NFP_ISLAND_RST_CLK_CTRL1_MEM_SHUT_DOWN_SHUT_DOWN (0)
#define     NFP_ISLAND_RST_CLK_CTRL1_MEM_SHUT_DOWN_NORMAL    (1)
/*----------------------------------------------------------------
  Register: MiscControl - Island-specific miscellaneous control
    Bits: 31:0  MiscCtrl(rw) - Miscellaneous Control Register - function depends on island.
*/
#define NFP_ISLAND_MISC_CTRL                                 0x00045410
#define   NFP_ISLAND_MISC_CTRL_MISC_CTRL(_x)                 (((_x) & 0xffffffff) << 0)
#define   NFP_ISLAND_MISC_CTRL_MISC_CTRL_of(_x)              (((_x) >> 0) & 0xffffffff)

#else /* NFP6000_LONGNAMES */

/*----------------------------------------------------------------
  Register: EventFilterMask[8] - Event Filter Mask Reg
    Bits: 23:0  EventMask(rw) - Mask for which of the event bits must be set; see also the Peripheral Event Manager FilterMask
*/
#define NFP_ISLAND_EventFilterMask(_x)                       (0x00044000 + (0x8 * ((_x) & 0x7)))
#define   NFP_ISLAND_EventFilterMask_EventMask(_x)           (((_x) & 0xffffff) << 0)
#define   NFP_ISLAND_EventFilterMask_EventMask_of(_x)        (((_x) >> 0) & 0xffffff)
/*----------------------------------------------------------------
  Register: EventFilterMatch[8] - Event Filter Match Reg
    Bits: 23:0  EventMatch(rw) - Values of the event bits, masked by FilterMask, which must be set for the event to be 'filtered' onto the global event chain; see also the Peripheral Event Manager EventFilterMatch
*/
#define NFP_ISLAND_EventFilterMatch(_x)                      (0x00044004 + (0x8 * ((_x) & 0x7)))
#define   NFP_ISLAND_EventFilterMatch_EventMatch(_x)         (((_x) & 0xffffff) << 0)
#define   NFP_ISLAND_EventFilterMatch_EventMatch_of(_x)      (((_x) >> 0) & 0xffffff)
/*----------------------------------------------------------------
  Register: EventRouteConfig - Event plane routing configuration, for local ring and global chain as it passes through the island
    Bits: 23    BypassInterior(rw) - Bypass Mux - for bypassing the interior of the island.
    Bits: 22:21 GlobalInjection(rw) - Selects to which corner overlay port in the event bus plane to send events filtered from the local chain as 'global'
    Bits: 20:19 GlobalMux(rw) - For the ARM island only, selects which overlay corner input port drives the global event bus into the ARM for use with the global event manger
    Bits: 18:16 IntInput(rw) - Selects which event plane input (from one of the 4 corners of the island or from the island interior) to use as the input to the interior output mux to drive the overlay interior output
    Bits: 15:13 BRInput(rw) - Selects which event plane input (from one of the 4 corners of the island or from the island interior) to use as the input to the bottom-right corner output mux to drive the overlay bottom-right corner output
    Bits: 12:10 BLInput(rw) - Selects which event plane input (from one of the 4 corners of the island or from the island interior) to use as the input to the bottom-left corner output mux to drive the overlay bottom-left corner output
    Bits:  9:7  TRInput(rw) - Selects which event plane input (from one of the 4 corners of the island or from the island interior) to use as the input to the top-right corner output mux to drive the overlay top-right corner output
    Bits:  6:4  TLInput(rw) - Selects which event plane input (from one of the 4 corners of the island or from the island interior) to use as the input to the top-left corner output mux to drive the overlay top-left corner output
    Bits:  3    RouteBR(rw) - REMOVED NOW DO NOT USE If asserted, route the bottom-right island ports in/out of the center; if clear, engage loopbacks (external loop to external, internal to internal)
    Bits:  2    RouteBL(rw) - REMOVED NOW DO NOT USE If asserted, route the bottom-left island ports in/out of the center; if clear, engage loopbacks (external loop to external, internal to internal)
    Bits:  1    RouteTR(rw) - REMOVED NOW DO NOT USE If asserted, route the top-right island ports in/out of the center; if clear, engage loopbacks (external loop to external, internal to internal)
    Bits:  0    RouteTL(rw) - REMOVED NOW DO NOT USE If asserted, route the top-left island ports in/out of the center; if clear, engage loopbacks (external loop to external, internal to internal)
*/
#define NFP_ISLAND_EventRouteConfig                          0x00044040
#define   NFP_ISLAND_EventRouteConfig_BypassInterior         BIT(23)
#define     NFP_ISLAND_EventRouteConfig_BypassInterior_NORMAL (0 << 23)
#define     NFP_ISLAND_EventRouteConfig_BypassInterior_BYPASS BIT(23)
#define   NFP_ISLAND_EventRouteConfig_GlobalInjection(_x)    (((_x) & 0x3) << 21)
#define   NFP_ISLAND_EventRouteConfig_GlobalInjection_of(_x) (((_x) >> 21) & 0x3)
#define     NFP_ISLAND_EventRouteConfig_GlobalInjection_TL   (0)
#define     NFP_ISLAND_EventRouteConfig_GlobalInjection_TR   (1)
#define     NFP_ISLAND_EventRouteConfig_GlobalInjection_BL   (2)
#define     NFP_ISLAND_EventRouteConfig_GlobalInjection_BR   (3)
#define   NFP_ISLAND_EventRouteConfig_GlobalMux(_x)          (((_x) & 0x3) << 19)
#define   NFP_ISLAND_EventRouteConfig_GlobalMux_of(_x)       (((_x) >> 19) & 0x3)
#define     NFP_ISLAND_EventRouteConfig_GlobalMux_TL         (0)
#define     NFP_ISLAND_EventRouteConfig_GlobalMux_TR         (1)
#define     NFP_ISLAND_EventRouteConfig_GlobalMux_BL         (2)
#define     NFP_ISLAND_EventRouteConfig_GlobalMux_BR         (3)
#define   NFP_ISLAND_EventRouteConfig_IntInput(_x)           (((_x) & 0x7) << 16)
#define   NFP_ISLAND_EventRouteConfig_IntInput_of(_x)        (((_x) >> 16) & 0x7)
#define     NFP_ISLAND_EventRouteConfig_IntInput_TL          (0)
#define     NFP_ISLAND_EventRouteConfig_IntInput_TR          (1)
#define     NFP_ISLAND_EventRouteConfig_IntInput_BL          (2)
#define     NFP_ISLAND_EventRouteConfig_IntInput_BR          (3)
#define     NFP_ISLAND_EventRouteConfig_IntInput_INTERIOR    (4)
#define   NFP_ISLAND_EventRouteConfig_BRInput(_x)            (((_x) & 0x7) << 13)
#define   NFP_ISLAND_EventRouteConfig_BRInput_of(_x)         (((_x) >> 13) & 0x7)
#define     NFP_ISLAND_EventRouteConfig_BRInput_TL           (0)
#define     NFP_ISLAND_EventRouteConfig_BRInput_TR           (1)
#define     NFP_ISLAND_EventRouteConfig_BRInput_BL           (2)
#define     NFP_ISLAND_EventRouteConfig_BRInput_BR           (3)
#define     NFP_ISLAND_EventRouteConfig_BRInput_INTERIOR     (4)
#define   NFP_ISLAND_EventRouteConfig_BLInput(_x)            (((_x) & 0x7) << 10)
#define   NFP_ISLAND_EventRouteConfig_BLInput_of(_x)         (((_x) >> 10) & 0x7)
#define     NFP_ISLAND_EventRouteConfig_BLInput_TL           (0)
#define     NFP_ISLAND_EventRouteConfig_BLInput_TR           (1)
#define     NFP_ISLAND_EventRouteConfig_BLInput_BL           (2)
#define     NFP_ISLAND_EventRouteConfig_BLInput_BR           (3)
#define     NFP_ISLAND_EventRouteConfig_BLInput_INTERIOR     (4)
#define   NFP_ISLAND_EventRouteConfig_TRInput(_x)            (((_x) & 0x7) << 7)
#define   NFP_ISLAND_EventRouteConfig_TRInput_of(_x)         (((_x) >> 7) & 0x7)
#define     NFP_ISLAND_EventRouteConfig_TRInput_TL           (0)
#define     NFP_ISLAND_EventRouteConfig_TRInput_TR           (1)
#define     NFP_ISLAND_EventRouteConfig_TRInput_BL           (2)
#define     NFP_ISLAND_EventRouteConfig_TRInput_BR           (3)
#define     NFP_ISLAND_EventRouteConfig_TRInput_INTERIOR     (4)
#define   NFP_ISLAND_EventRouteConfig_TLInput(_x)            (((_x) & 0x7) << 4)
#define   NFP_ISLAND_EventRouteConfig_TLInput_of(_x)         (((_x) >> 4) & 0x7)
#define     NFP_ISLAND_EventRouteConfig_TLInput_TL           (0)
#define     NFP_ISLAND_EventRouteConfig_TLInput_TR           (1)
#define     NFP_ISLAND_EventRouteConfig_TLInput_BL           (2)
#define     NFP_ISLAND_EventRouteConfig_TLInput_BR           (3)
#define     NFP_ISLAND_EventRouteConfig_TLInput_INTERIOR     (4)
#define   NFP_ISLAND_EventRouteConfig_RouteBR                BIT(3)
#define   NFP_ISLAND_EventRouteConfig_RouteBL                BIT(2)
#define   NFP_ISLAND_EventRouteConfig_RouteTR                BIT(1)
#define   NFP_ISLAND_EventRouteConfig_RouteTL                BIT(0)
/*----------------------------------------------------------------
  Register: ClockResetControl - Clock and reset control for the island
    Bits: 31:24 ClockEnable(rw) - Set to enable the clock for a clock domain. Each bit (0-7) applies to each reset/clock domain (0-7) for the current island.
    Bits: 23:16 RemoveReset(rw) - Set to deassert soft reset for a clock/reset domain (unless the global soft reset signal is asserted and not maksed). Each bit (0-7) applies to each reset/clock domain (0-7) for the current island.
    Bits: 15:8  ResetMask(rw) - When set, masks (ignores) the Global soft reset signal. Each bit (0-7) applies to each reset/clock domain (0-7) for the current island.
    Bits:  7:0  DomainLock(rw) - When set, prevents modifying the corresponding bits in registers OvlClkRstControl and OvlMemControl for the current island. For instance, when bit 0 of Lock is set, the settings in OvlClkRstControl and OvlMemControl corresponding to reset/clock domain 0 cannot be modified.
*/
#define NFP_ISLAND_ClockResetControl                         0x00045400
#define   NFP_ISLAND_ClockResetControl_ClockEnable(_x)       (((_x) & 0xff) << 24)
#define   NFP_ISLAND_ClockResetControl_ClockEnable_of(_x)    (((_x) >> 24) & 0xff)
#define     NFP_ISLAND_ClockResetControl_ClockEnable_CLK_DIS (0)
#define     NFP_ISLAND_ClockResetControl_ClockEnable_CLK_EN  (1)
#define   NFP_ISLAND_ClockResetControl_RemoveReset(_x)       (((_x) & 0xff) << 16)
#define   NFP_ISLAND_ClockResetControl_RemoveReset_of(_x)    (((_x) >> 16) & 0xff)
#define     NFP_ISLAND_ClockResetControl_RemoveReset_SRESET  (0)
#define     NFP_ISLAND_ClockResetControl_RemoveReset_NORMAL  (1)
#define   NFP_ISLAND_ClockResetControl_ResetMask(_x)         (((_x) & 0xff) << 8)
#define   NFP_ISLAND_ClockResetControl_ResetMask_of(_x)      (((_x) >> 8) & 0xff)
#define     NFP_ISLAND_ClockResetControl_ResetMask_NORMAL    (0)
#define     NFP_ISLAND_ClockResetControl_ResetMask_MASK      (1)
#define   NFP_ISLAND_ClockResetControl_DomainLock(_x)        (((_x) & 0xff) << 0)
#define   NFP_ISLAND_ClockResetControl_DomainLock_of(_x)     (((_x) >> 0) & 0xff)
#define     NFP_ISLAND_ClockResetControl_DomainLock_NORMAL   (0)
#define     NFP_ISLAND_ClockResetControl_DomainLock_LOCKED   (1)
/*----------------------------------------------------------------
  Register: MemControl - Island-level memory control, for sleep etc
    Bits:  7:0  Enable(rw) - Set to enable memories in the reset/clock domains. Each bit (0-7) applies to each reset/clock domain (0-7) for the current island.
*/
#define NFP_ISLAND_MemControl                                0x00045404
#define   NFP_ISLAND_MemControl_Enable(_x)                   (((_x) & 0xff) << 0)
#define   NFP_ISLAND_MemControl_Enable_of(_x)                (((_x) >> 0) & 0xff)
#define     NFP_ISLAND_MemControl_Enable_SHUT_DOWN           (0)
#define     NFP_ISLAND_MemControl_Enable_NORMAL              (1)
/*----------------------------------------------------------------
  Register: MiscControl - Island-specific miscellaneous control
    Bits: 31:0  MiscCtrl(rw) - Miscellaneous Control Register - function depends on island.
*/
#define NFP_ISLAND_MiscControl                               0x00045410
#define   NFP_ISLAND_MiscControl_MiscCtrl(_x)                (((_x) & 0xffffffff) << 0)
#define   NFP_ISLAND_MiscControl_MiscCtrl_of(_x)             (((_x) >> 0) & 0xffffffff)

#endif /* NFP6000_LONGNAMES */


#endif /* NFP6000_ISLAND_H */
