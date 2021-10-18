/*
 * Copyright (C) 2013, Netronome Systems, Inc.  All rights reserved.
 *
 * Disclaimer: this file is provided without any warranty
 * as part of an early-access program, and the content is
 * bound to change before the final release.
 */

#ifndef NFP6000_IM_H
#define NFP6000_IM_H


/* HGID: 60515d2e075a */

#ifndef NFP6000_LONGNAMES

/*----------------------------------------------------------------
  Register: Status - 32-bits of status in
    Bits: 31:0  Status(ro) - Status from the input to the module
*/
#define NFP_IM_PINSTATUS                                     0x00000000
#define   NFP_IM_PINSTATUS_STATUS(_x)                        (((_x) & 0xffffffff) << 0)
#define   NFP_IM_PINSTATUS_STATUS_of(_x)                     (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: IntDisable - 32-bits indicating which status bits have interrupt generation disabled
  Register: IntEnableLow - 32-bits indicating which status bits generate low priority interrupts
  Register: IntEnableMid - 32-bits indicating which status bits generate medium priority interrupts
  Register: IntEnableHigh - 32-bits indicating which status bits generate high priority interrupts
    Bits: 31:0  IntEnables(rw) - When read a bit is set if the corresponding status bit is assigned to the interrupt level of the register being read; write a 1 in a bit to assign the corresponding status bit to the interrupt level of the register being written.
*/
#define NFP_IM_PINDISABLE                                    0x00000004
#define NFP_IM_PINENABLELOW                                  0x0000000c
#define NFP_IM_PINENABLEMED                                  0x00000014
#define NFP_IM_PINENABLEHIGH                                 0x0000001c
#define   NFP_IM_IMGRENABLE_MASK(_x)                         (((_x) & 0xffffffff) << 0)
#define   NFP_IM_IMGRENABLE_MASK_of(_x)                      (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: IntStatusLow - Status & IntEnableLow
  Register: IntStatusMid - Status & IntEnableMid
  Register: IntStatusHigh - Status & IntEnableHigh
    Bits: 31:0  MaskedStatus(ro) - Status from the input to the module, ANDed with the interrupt priority for the register being read.
*/
#define NFP_IM_PINSTATUSLOW                                  0x00000008
#define NFP_IM_INTRSTSMI                                     0x00000010
#define NFP_IM_PINSTATUSHIGH                                 0x00000018
#define   NFP_IM_IMGRENABLEDSTATUS_MASKED_STATUS(_x)         (((_x) & 0xffffffff) << 0)
#define   NFP_IM_IMGRENABLEDSTATUS_MASKED_STATUS_of(_x)      (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: StatusEventConfig0 - Which status changes on bits 0-15 generate events
  Register: StatusEventConfig1 - What status changes on bits 16-31 generate events
    Bits: 31:30 Edge15(rw) - Type of edge to watch Status15/31 for, to automatically generate events on that change.
    Bits: 29:28 Edge14(rw) - Type of edge to watch Status14/30 for, to automatically generate events on that change.
    Bits: 27:26 Edge13(rw) - Type of edge to watch Status13/29 for, to automatically generate events on that change.
    Bits: 25:24 Edge12(rw) - Type of edge to watch Status12/28 for, to automatically generate events on that change.
    Bits: 23:22 Edge11(rw) - Type of edge to watch Status11/27 for, to automatically generate events on that change.
    Bits: 21:20 Edge10(rw) - Type of edge to watch Status10/26 for, to automatically generate events on that change.
    Bits: 19:18 Edge9(rw) - Type of edge to watch Status9/25 for, to automatically generate events on that change.
    Bits: 17:16 Edge8(rw) - Type of edge to watch Status8/24 for, to automatically generate events on that change.
    Bits: 15:14 Edge7(rw) - Type of edge to watch Status7/23 for, to automatically generate events on that change.
    Bits: 13:12 Edge6(rw) - Type of edge to watch Status6/22 for, to automatically generate events on that change.
    Bits: 11:10 Edge5(rw) - Type of edge to watch Status5/21 for, to automatically generate events on that change.
    Bits:  9:8  Edge4(rw) - Type of edge to watch Status4/20 for, to automatically generate events on that change.
    Bits:  7:6  Edge3(rw) - Type of edge to watch Status3/19 for, to automatically generate events on that change.
    Bits:  5:4  Edge2(rw) - Type of edge to watch Status2/18 for, to automatically generate events on that change.
    Bits:  3:2  Edge1(rw) - Type of edge to watch Status1/17 for, to automatically generate events on that change.
    Bits:  1:0  Edge0(rw) - Type of edge to watch Status0/16 for, to automatically generate events on that change.
*/
#define NFP_IM_EVENTCONFIG0                                  0x00000028
#define NFP_IM_EVENTCONFIG1                                  0x0000002c
#define   NFP_IM_IMGRSTATUSEVENTCONFIG_EDGE15(_x)            (((_x) & 0x3) << 30)
#define   NFP_IM_IMGRSTATUSEVENTCONFIG_EDGE15_of(_x)         (((_x) >> 30) & 0x3)
#define   NFP_IM_IMGRSTATUSEVENTCONFIG_EDGE14(_x)            (((_x) & 0x3) << 28)
#define   NFP_IM_IMGRSTATUSEVENTCONFIG_EDGE14_of(_x)         (((_x) >> 28) & 0x3)
#define   NFP_IM_IMGRSTATUSEVENTCONFIG_EDGE13(_x)            (((_x) & 0x3) << 26)
#define   NFP_IM_IMGRSTATUSEVENTCONFIG_EDGE13_of(_x)         (((_x) >> 26) & 0x3)
#define   NFP_IM_IMGRSTATUSEVENTCONFIG_EDGE12(_x)            (((_x) & 0x3) << 24)
#define   NFP_IM_IMGRSTATUSEVENTCONFIG_EDGE12_of(_x)         (((_x) >> 24) & 0x3)
#define   NFP_IM_IMGRSTATUSEVENTCONFIG_EDGE11(_x)            (((_x) & 0x3) << 22)
#define   NFP_IM_IMGRSTATUSEVENTCONFIG_EDGE11_of(_x)         (((_x) >> 22) & 0x3)
#define   NFP_IM_IMGRSTATUSEVENTCONFIG_EDGE10(_x)            (((_x) & 0x3) << 20)
#define   NFP_IM_IMGRSTATUSEVENTCONFIG_EDGE10_of(_x)         (((_x) >> 20) & 0x3)
#define   NFP_IM_IMGRSTATUSEVENTCONFIG_EDGE9(_x)             (((_x) & 0x3) << 18)
#define   NFP_IM_IMGRSTATUSEVENTCONFIG_EDGE9_of(_x)          (((_x) >> 18) & 0x3)
#define   NFP_IM_IMGRSTATUSEVENTCONFIG_EDGE8(_x)             (((_x) & 0x3) << 16)
#define   NFP_IM_IMGRSTATUSEVENTCONFIG_EDGE8_of(_x)          (((_x) >> 16) & 0x3)
#define   NFP_IM_IMGRSTATUSEVENTCONFIG_EDGE7(_x)             (((_x) & 0x3) << 14)
#define   NFP_IM_IMGRSTATUSEVENTCONFIG_EDGE7_of(_x)          (((_x) >> 14) & 0x3)
#define   NFP_IM_IMGRSTATUSEVENTCONFIG_EDGE6(_x)             (((_x) & 0x3) << 12)
#define   NFP_IM_IMGRSTATUSEVENTCONFIG_EDGE6_of(_x)          (((_x) >> 12) & 0x3)
#define   NFP_IM_IMGRSTATUSEVENTCONFIG_EDGE5(_x)             (((_x) & 0x3) << 10)
#define   NFP_IM_IMGRSTATUSEVENTCONFIG_EDGE5_of(_x)          (((_x) >> 10) & 0x3)
#define   NFP_IM_IMGRSTATUSEVENTCONFIG_EDGE4(_x)             (((_x) & 0x3) << 8)
#define   NFP_IM_IMGRSTATUSEVENTCONFIG_EDGE4_of(_x)          (((_x) >> 8) & 0x3)
#define   NFP_IM_IMGRSTATUSEVENTCONFIG_EDGE3(_x)             (((_x) & 0x3) << 6)
#define   NFP_IM_IMGRSTATUSEVENTCONFIG_EDGE3_of(_x)          (((_x) >> 6) & 0x3)
#define   NFP_IM_IMGRSTATUSEVENTCONFIG_EDGE2(_x)             (((_x) & 0x3) << 4)
#define   NFP_IM_IMGRSTATUSEVENTCONFIG_EDGE2_of(_x)          (((_x) >> 4) & 0x3)
#define   NFP_IM_IMGRSTATUSEVENTCONFIG_EDGE1(_x)             (((_x) & 0x3) << 2)
#define   NFP_IM_IMGRSTATUSEVENTCONFIG_EDGE1_of(_x)          (((_x) >> 2) & 0x3)
#define   NFP_IM_IMGRSTATUSEVENTCONFIG_EDGE0(_x)             (((_x) & 0x3) << 0)
#define   NFP_IM_IMGRSTATUSEVENTCONFIG_EDGE0_of(_x)          (((_x) >> 0) & 0x3)
/*----------------------------------------------------------------
  Register: EventOut - Write to add event to FIFO
    Bits: 15:4  Source(wo) - Value for the source field of the event.
    Bits:  3:0  Type(wo) - Value for the type field of the event.
*/
#define NFP_IM_EVENTTEST                                     0x00000030
#define   NFP_IM_EVENTTEST_SOURCE(_x)                        (((_x) & 0xfff) << 4)
#define   NFP_IM_EVENTTEST_SOURCE_of(_x)                     (((_x) >> 4) & 0xfff)
#define   NFP_IM_EVENTTEST_TYPE(_x)                          (((_x) & 0xf) << 0)
#define   NFP_IM_EVENTTEST_TYPE_of(_x)                       (((_x) >> 0) & 0xf)
/*----------------------------------------------------------------
  Register: CaptureTimerStatus - Set which edge detect captures a timer value
    Bits: 31:0  Edges(rw) - Mask of the edges which, when detected with StatusEdgeN, should cause a timer value to be captured.
*/
#define NFP_IM_CAPTIMERSTATUS                                0x00000038
#define   NFP_IM_CAPTIMERSTATUS_EDGES(_x)                    (((_x) & 0xffffffff) << 0)
#define   NFP_IM_CAPTIMERSTATUS_EDGES_of(_x)                 (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: CaptureTimerValue - Set which edge detect captures a timer value
    Bits: 31    Valid(ro) - Asserted if the timer value is valid; cleared on read and write.
    Bits: 30:0  Value(ro) - Timer value at last edge detected on 'CaptureEdge' status bit.
*/
#define NFP_IM_CAPTIMERVALUE                                 0x0000003c
#define   NFP_IM_CAPTIMERVALUE_VALID                         BIT(31)
#define   NFP_IM_CAPTIMERVALUE_VALUE(_x)                     (((_x) & 0x7fffffff) << 0)
#define   NFP_IM_CAPTIMERVALUE_VALUE_of(_x)                  (((_x) >> 0) & 0x7fffffff)

#else /* NFP6000_LONGNAMES */

/*----------------------------------------------------------------
  Register: Status - 32-bits of status in
    Bits: 31:0  Status(ro) - Status from the input to the module
*/
#define NFP_IM_Status                                        0x00000000
#define   NFP_IM_Status_Status(_x)                           (((_x) & 0xffffffff) << 0)
#define   NFP_IM_Status_Status_of(_x)                        (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: IntDisable - 32-bits indicating which status bits have interrupt generation disabled
  Register: IntEnableLow - 32-bits indicating which status bits generate low priority interrupts
  Register: IntEnableMid - 32-bits indicating which status bits generate medium priority interrupts
  Register: IntEnableHigh - 32-bits indicating which status bits generate high priority interrupts
    Bits: 31:0  IntEnables(rw) - When read a bit is set if the corresponding status bit is assigned to the interrupt level of the register being read; write a 1 in a bit to assign the corresponding status bit to the interrupt level of the register being written.
*/
#define NFP_IM_IntDisable                                    0x00000004
#define NFP_IM_IntEnableLow                                  0x0000000c
#define NFP_IM_IntEnableMid                                  0x00000014
#define NFP_IM_IntEnableHigh                                 0x0000001c
#define   NFP_IM_IMgrEnable_IntEnables(_x)                   (((_x) & 0xffffffff) << 0)
#define   NFP_IM_IMgrEnable_IntEnables_of(_x)                (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: IntStatusLow - Status & IntEnableLow
  Register: IntStatusMid - Status & IntEnableMid
  Register: IntStatusHigh - Status & IntEnableHigh
    Bits: 31:0  MaskedStatus(ro) - Status from the input to the module, ANDed with the interrupt priority for the register being read.
*/
#define NFP_IM_IntStatusLow                                  0x00000008
#define NFP_IM_IntStatusMid                                  0x00000010
#define NFP_IM_IntStatusHigh                                 0x00000018
#define   NFP_IM_IMgrEnabledStatus_MaskedStatus(_x)          (((_x) & 0xffffffff) << 0)
#define   NFP_IM_IMgrEnabledStatus_MaskedStatus_of(_x)       (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: StatusEventConfig0 - Which status changes on bits 0-15 generate events
  Register: StatusEventConfig1 - What status changes on bits 16-31 generate events
    Bits: 31:30 Edge15(rw) - Type of edge to watch Status15/31 for, to automatically generate events on that change.
    Bits: 29:28 Edge14(rw) - Type of edge to watch Status14/30 for, to automatically generate events on that change.
    Bits: 27:26 Edge13(rw) - Type of edge to watch Status13/29 for, to automatically generate events on that change.
    Bits: 25:24 Edge12(rw) - Type of edge to watch Status12/28 for, to automatically generate events on that change.
    Bits: 23:22 Edge11(rw) - Type of edge to watch Status11/27 for, to automatically generate events on that change.
    Bits: 21:20 Edge10(rw) - Type of edge to watch Status10/26 for, to automatically generate events on that change.
    Bits: 19:18 Edge9(rw) - Type of edge to watch Status9/25 for, to automatically generate events on that change.
    Bits: 17:16 Edge8(rw) - Type of edge to watch Status8/24 for, to automatically generate events on that change.
    Bits: 15:14 Edge7(rw) - Type of edge to watch Status7/23 for, to automatically generate events on that change.
    Bits: 13:12 Edge6(rw) - Type of edge to watch Status6/22 for, to automatically generate events on that change.
    Bits: 11:10 Edge5(rw) - Type of edge to watch Status5/21 for, to automatically generate events on that change.
    Bits:  9:8  Edge4(rw) - Type of edge to watch Status4/20 for, to automatically generate events on that change.
    Bits:  7:6  Edge3(rw) - Type of edge to watch Status3/19 for, to automatically generate events on that change.
    Bits:  5:4  Edge2(rw) - Type of edge to watch Status2/18 for, to automatically generate events on that change.
    Bits:  3:2  Edge1(rw) - Type of edge to watch Status1/17 for, to automatically generate events on that change.
    Bits:  1:0  Edge0(rw) - Type of edge to watch Status0/16 for, to automatically generate events on that change.
*/
#define NFP_IM_StatusEventConfig0                            0x00000028
#define NFP_IM_StatusEventConfig1                            0x0000002c
#define   NFP_IM_IMgrStatusEventConfig_Edge15(_x)            (((_x) & 0x3) << 30)
#define   NFP_IM_IMgrStatusEventConfig_Edge15_of(_x)         (((_x) >> 30) & 0x3)
#define   NFP_IM_IMgrStatusEventConfig_Edge14(_x)            (((_x) & 0x3) << 28)
#define   NFP_IM_IMgrStatusEventConfig_Edge14_of(_x)         (((_x) >> 28) & 0x3)
#define   NFP_IM_IMgrStatusEventConfig_Edge13(_x)            (((_x) & 0x3) << 26)
#define   NFP_IM_IMgrStatusEventConfig_Edge13_of(_x)         (((_x) >> 26) & 0x3)
#define   NFP_IM_IMgrStatusEventConfig_Edge12(_x)            (((_x) & 0x3) << 24)
#define   NFP_IM_IMgrStatusEventConfig_Edge12_of(_x)         (((_x) >> 24) & 0x3)
#define   NFP_IM_IMgrStatusEventConfig_Edge11(_x)            (((_x) & 0x3) << 22)
#define   NFP_IM_IMgrStatusEventConfig_Edge11_of(_x)         (((_x) >> 22) & 0x3)
#define   NFP_IM_IMgrStatusEventConfig_Edge10(_x)            (((_x) & 0x3) << 20)
#define   NFP_IM_IMgrStatusEventConfig_Edge10_of(_x)         (((_x) >> 20) & 0x3)
#define   NFP_IM_IMgrStatusEventConfig_Edge9(_x)             (((_x) & 0x3) << 18)
#define   NFP_IM_IMgrStatusEventConfig_Edge9_of(_x)          (((_x) >> 18) & 0x3)
#define   NFP_IM_IMgrStatusEventConfig_Edge8(_x)             (((_x) & 0x3) << 16)
#define   NFP_IM_IMgrStatusEventConfig_Edge8_of(_x)          (((_x) >> 16) & 0x3)
#define   NFP_IM_IMgrStatusEventConfig_Edge7(_x)             (((_x) & 0x3) << 14)
#define   NFP_IM_IMgrStatusEventConfig_Edge7_of(_x)          (((_x) >> 14) & 0x3)
#define   NFP_IM_IMgrStatusEventConfig_Edge6(_x)             (((_x) & 0x3) << 12)
#define   NFP_IM_IMgrStatusEventConfig_Edge6_of(_x)          (((_x) >> 12) & 0x3)
#define   NFP_IM_IMgrStatusEventConfig_Edge5(_x)             (((_x) & 0x3) << 10)
#define   NFP_IM_IMgrStatusEventConfig_Edge5_of(_x)          (((_x) >> 10) & 0x3)
#define   NFP_IM_IMgrStatusEventConfig_Edge4(_x)             (((_x) & 0x3) << 8)
#define   NFP_IM_IMgrStatusEventConfig_Edge4_of(_x)          (((_x) >> 8) & 0x3)
#define   NFP_IM_IMgrStatusEventConfig_Edge3(_x)             (((_x) & 0x3) << 6)
#define   NFP_IM_IMgrStatusEventConfig_Edge3_of(_x)          (((_x) >> 6) & 0x3)
#define   NFP_IM_IMgrStatusEventConfig_Edge2(_x)             (((_x) & 0x3) << 4)
#define   NFP_IM_IMgrStatusEventConfig_Edge2_of(_x)          (((_x) >> 4) & 0x3)
#define   NFP_IM_IMgrStatusEventConfig_Edge1(_x)             (((_x) & 0x3) << 2)
#define   NFP_IM_IMgrStatusEventConfig_Edge1_of(_x)          (((_x) >> 2) & 0x3)
#define   NFP_IM_IMgrStatusEventConfig_Edge0(_x)             (((_x) & 0x3) << 0)
#define   NFP_IM_IMgrStatusEventConfig_Edge0_of(_x)          (((_x) >> 0) & 0x3)
/*----------------------------------------------------------------
  Register: EventOut - Write to add event to FIFO
    Bits: 15:4  Source(wo) - Value for the source field of the event.
    Bits:  3:0  Type(wo) - Value for the type field of the event.
*/
#define NFP_IM_EventOut                                      0x00000030
#define   NFP_IM_EventOut_Source(_x)                         (((_x) & 0xfff) << 4)
#define   NFP_IM_EventOut_Source_of(_x)                      (((_x) >> 4) & 0xfff)
#define   NFP_IM_EventOut_Type(_x)                           (((_x) & 0xf) << 0)
#define   NFP_IM_EventOut_Type_of(_x)                        (((_x) >> 0) & 0xf)
/*----------------------------------------------------------------
  Register: CaptureTimerStatus - Set which edge detect captures a timer value
    Bits: 31:0  Edges(rw) - Mask of the edges which, when detected with StatusEdgeN, should cause a timer value to be captured.
*/
#define NFP_IM_CaptureTimerStatus                            0x00000038
#define   NFP_IM_CaptureTimerStatus_Edges(_x)                (((_x) & 0xffffffff) << 0)
#define   NFP_IM_CaptureTimerStatus_Edges_of(_x)             (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: CaptureTimerValue - Set which edge detect captures a timer value
    Bits: 31    Valid(ro) - Asserted if the timer value is valid; cleared on read and write.
    Bits: 30:0  Value(ro) - Timer value at last edge detected on 'CaptureEdge' status bit.
*/
#define NFP_IM_CaptureTimerValue                             0x0000003c
#define   NFP_IM_CaptureTimerValue_Valid                     BIT(31)
#define   NFP_IM_CaptureTimerValue_Value(_x)                 (((_x) & 0x7fffffff) << 0)
#define   NFP_IM_CaptureTimerValue_Value_of(_x)              (((_x) >> 0) & 0x7fffffff)

#endif /* NFP6000_LONGNAMES */


#endif /* NFP6000_IM_H */
