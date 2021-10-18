/*
 * Copyright (C) 2013, Netronome Systems, Inc.  All rights reserved.
 *
 * Disclaimer: this file is provided without any warranty
 * as part of an early-access program, and the content is
 * bound to change before the final release.
 */

#ifndef NFP3800_EM_H
#define NFP3800_EM_H


/* HGID: 6ea1af6fca88 */

#ifndef NFP3800_LONGNAMES

/*----------------------------------------------------------------
  Register: Filter[32]_FilterStatus_Count32 - For FilterType = 0. 32-bit counter of events matching the filter
    Bits: 31:0  Count(ro) - Saturating 32 bit count of events that matched the filter
*/
#define NFP3800_EM_Filter_FilterStatus_COUNT32(_x)           (0x00000000 + (0x20 * ((_x) & 0x1f)))
#define   NFP3800_EM_FILTER_FILTERSTATUS_COUNT32_CNT32(_x)   (((_x) & 0xffffffff) << 0)
#define   NFP3800_EM_FILTER_FILTERSTATUS_COUNT32_CNT32_of(_x) (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: Filter[32]_FilterStatus_Count16 - For FilterType = 1. 16-bit counter of events matching the filter, with throttling and timeout
    Bits: 31:29 Timeout(rw) - 3-bit throttle timeout value
    Bits: 28:23 Upcounter(ro) - 6-bit throttle upcounter; if non-zero then status is throttled; set on acknowledge
    Bits: 21:16 Override(rw) - Ignore throttle if Count>Override and Override!=0
    Bits: 15:0  Count(ro) - Saturating 16 bit count of events that matched the filter
*/
#define NFP3800_EM_Filter_FilterStatus_COUNT16(_x)           (0x00000000 + (0x20 * ((_x) & 0x1f)))
#define   NFP3800_EM_FILTER_FILTERSTATUS_COUNT16_TMOUT(_x)   (((_x) & 0x7) << 29)
#define   NFP3800_EM_FILTER_FILTERSTATUS_COUNT16_TMOUT_of(_x) (((_x) >> 29) & 0x7)
#define   NFP3800_EM_FILTER_FILTERSTATUS_COUNT16_UPCNT(_x)   (((_x) & 0x3f) << 23)
#define   NFP3800_EM_FILTER_FILTERSTATUS_COUNT16_UPCNT_of(_x) (((_x) >> 23) & 0x3f)
#define   NFP3800_EM_FILTER_FILTERSTATUS_COUNT16_OVERRIDE(_x) (((_x) & 0x3f) << 16)
#define   NFP3800_EM_FILTER_FILTERSTATUS_COUNT16_OVERRIDE_of(_x) (((_x) >> 16) & 0x3f)
#define   NFP3800_EM_FILTER_FILTERSTATUS_COUNT16_CNT16(_x)   (((_x) & 0xffff) << 0)
#define   NFP3800_EM_FILTER_FILTERSTATUS_COUNT16_CNT16_of(_x) (((_x) >> 0) & 0xffff)
/*----------------------------------------------------------------
  Register: Filter[32]_FilterStatus_Bitmask32 - For FilterType = 2. 32-bit bitmask of events matching the filter
    Bits: 31:0  Bitmask(ro) - Bitmask of events that matched the filter; this register is reset when it is read
*/
#define NFP3800_EM_Filter_FilterStatus_BITMASK32(_x)         (0x00000000 + (0x20 * ((_x) & 0x1f)))
#define   NFP3800_EM_FILTER_FILTERSTATUS_BITMASK32_MASK32(_x) (((_x) & 0xffffffff) << 0)
#define   NFP3800_EM_FILTER_FILTERSTATUS_BITMASK32_MASK32_of(_x) (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: Filter[32]_FilterStatus_Bitmask16 - For FilterType = 3. 16-bit bitmask of events matching the filter, with throttling and timeout
    Bits: 31:29 Timeout(rw) - 3-bit throttle timeout value
    Bits: 28:23 Upcounter(ro) - 6-bit throttle upcounter; if non-zero then status is throttled; set on acknowledge
    Bits: 22:20 Override(rw) - Ignore throttle if Count>Override and Override!=0
    Bits: 19:16 Count(rw) - Saturating 4 bit count of events that matched the filter
    Bits: 15:0  Bitmask(ro) - Bitmask of events that matched the filter; reset on acknowledge
*/
#define NFP3800_EM_Filter_FilterStatus_BITMASK16(_x)         (0x00000000 + (0x20 * ((_x) & 0x1f)))
#define   NFP3800_EM_FILTER_FILTERSTATUS_BITMASK16_TMOUT(_x) (((_x) & 0x7) << 29)
#define   NFP3800_EM_FILTER_FILTERSTATUS_BITMASK16_TMOUT_of(_x) (((_x) >> 29) & 0x7)
#define   NFP3800_EM_FILTER_FILTERSTATUS_BITMASK16_UPCNT(_x) (((_x) & 0x3f) << 23)
#define   NFP3800_EM_FILTER_FILTERSTATUS_BITMASK16_UPCNT_of(_x) (((_x) >> 23) & 0x3f)
#define   NFP3800_EM_FILTER_FILTERSTATUS_BITMASK16_OVERRIDE(_x) (((_x) & 0x7) << 20)
#define   NFP3800_EM_FILTER_FILTERSTATUS_BITMASK16_OVERRIDE_of(_x) (((_x) >> 20) & 0x7)
#define   NFP3800_EM_FILTER_FILTERSTATUS_BITMASK16_CNT4(_x)  (((_x) & 0xf) << 16)
#define   NFP3800_EM_FILTER_FILTERSTATUS_BITMASK16_CNT4_of(_x) (((_x) >> 16) & 0xf)
#define   NFP3800_EM_FILTER_FILTERSTATUS_BITMASK16_MASK16(_x) (((_x) & 0xffff) << 0)
#define   NFP3800_EM_FILTER_FILTERSTATUS_BITMASK16_MASK16_of(_x) (((_x) >> 0) & 0xffff)
/*----------------------------------------------------------------
  Register: Filter[32]_FilterStatus_Event - For FilterType = 4 (FirstEvent) or 5 (LastEvent). Recorded event which matched the filter, with throttling and timeout
    Bits: 31:29 Timeout(rw) - 3-bit throttle timeout value
    Bits: 28:23 Upcounter(ro) - 6-bit throttle upcounter; if non-zero then status is throttled; set on acknowledge
    Bits: 21:20 Count(ro) - Saturating 2 bit count of events that matched the filter; reset on acknowledge
    Bits: 19:0  Event(ro) - Provider, source and event number for the first or last event since the last acknowledge
*/
#define NFP3800_EM_Filter_FilterStatus_EVENT(_x)             (0x00000000 + (0x20 * ((_x) & 0x1f)))
#define   NFP3800_EM_FILTER_FILTERSTATUS_EVENT_TMOUT(_x)     (((_x) & 0x7) << 29)
#define   NFP3800_EM_FILTER_FILTERSTATUS_EVENT_TMOUT_of(_x)  (((_x) >> 29) & 0x7)
#define   NFP3800_EM_FILTER_FILTERSTATUS_EVENT_UPCNT(_x)     (((_x) & 0x3f) << 23)
#define   NFP3800_EM_FILTER_FILTERSTATUS_EVENT_UPCNT_of(_x)  (((_x) >> 23) & 0x3f)
#define   NFP3800_EM_FILTER_FILTERSTATUS_EVENT_CNT2(_x)      (((_x) & 0x3) << 20)
#define   NFP3800_EM_FILTER_FILTERSTATUS_EVENT_CNT2_of(_x)   (((_x) >> 20) & 0x3)
#define   NFP3800_EM_FILTER_FILTERSTATUS_EVENT_EVENT(_x)     (((_x) & 0xfffff) << 0)
#define   NFP3800_EM_FILTER_FILTERSTATUS_EVENT_EVENT_of(_x)  (((_x) >> 0) & 0xfffff)
/*----------------------------------------------------------------
  Register: Filter[32]_FilterFlag - Filter Flag Register
    Bits:  1    PendingStatus(ro) - Asserted if the filter status flag would be asserted if not for throttling
    Bits:  0    Status(ro) - Asserted if the filter status is asserted
*/
#define NFP3800_EM_Filter_FLAGS(_x)                          (0x00000010 + (0x20 * ((_x) & 0x1f)))
#define   NFP3800_EM_FILTER_FLAGS_PENDING_STATUS             BIT(1)
#define   NFP3800_EM_FILTER_FLAGS_STATUS                     BIT(0)
/*----------------------------------------------------------------
  Register: Filter[32]_FilterMask - Filter Mask Register
    Bits: 26:24 FilterType(rw) - Configuration type of the filter
    Bits: 23:0  EventMask(rw) - ANDed with event for filter event match
*/
#define NFP3800_EM_Filter_MASK(_x)                           (0x00000020 + (0x20 * ((_x) & 0x1f)))
#define   NFP3800_EM_FILTER_MASK_TYPE(_x)                    (((_x) & 0x7) << 24)
#define   NFP3800_EM_FILTER_MASK_TYPE_of(_x)                 (((_x) >> 24) & 0x7)
#define     NFP3800_EM_FILTER_MASK_TYPE_COUNT32              (0)
#define     NFP3800_EM_FILTER_MASK_TYPE_COUNT16              (1)
#define     NFP3800_EM_FILTER_MASK_TYPE_MASK32               (2)
#define     NFP3800_EM_FILTER_MASK_TYPE_MASK16               (3)
#define     NFP3800_EM_FILTER_MASK_TYPE_FIRSTEV              (4)
#define     NFP3800_EM_FILTER_MASK_TYPE_LASTEV               (5)
#define     NFP3800_EM_FILTER_MASK_TYPE_FIRSTEVUT            (6)
#define     NFP3800_EM_FILTER_MASK_TYPE_LASTEVUT             (7)
#define   NFP3800_EM_FILTER_MASK_EVENT(_x)                   (((_x) & 0xffffff) << 0)
#define   NFP3800_EM_FILTER_MASK_EVENT_of(_x)                (((_x) >> 0) & 0xffffff)
/*----------------------------------------------------------------
  Register: Filter[32]_FilterMatch - Filter Match Register
    Bits: 23:0  EventMatch(rw) - Compared with event & EventMask to filter events
*/
#define NFP3800_EM_Filter_MATCH(_x)                          (0x00000030 + (0x20 * ((_x) & 0x1f)))
#define   NFP3800_EM_FILTER_MATCH_EVENT(_x)                  (((_x) & 0xffffff) << 0)
#define   NFP3800_EM_FILTER_MATCH_EVENT_of(_x)               (((_x) >> 0) & 0xffffff)
/*----------------------------------------------------------------
  Register: Filter[32]_FilterAcknowledge - A read acknowledges the filter causing it to be reset
    Bits: 31:0  EvtStatus(rc) - Read Only. Acknowledges the filter, returning the contents of the FilterStatus Register and then resets it.
*/
#define NFP3800_EM_Filter_ACK(_x)                            (0x00000040 + (0x20 * ((_x) & 0x1f)))
#define   NFP3800_EM_FILTER_ACK_EVT_STATUS(_x)               (((_x) & 0xffffffff) << 0)
#define   NFP3800_EM_FILTER_ACK_EVT_STATUS_of(_x)            (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: CombinedStatus - 32 bits of status from the filters
    Bits: 31:0  Status(ro) - One bit per filter, indicating its status
*/
#define NFP3800_EM_ALL_STATUS                                0x00001000
#define   NFP3800_EM_ALL_STATUS_STATUS(_x)                   (((_x) & 0xffffffff) << 0)
#define   NFP3800_EM_ALL_STATUS_STATUS_of(_x)                (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: CombinedPendingStatus - 32 bits of pending status from the filters
    Bits: 31:0  PendingStatus(ro) - One bit per filter, indicating its pending status (status that would be presented if not throttling)
*/
#define NFP3800_EM_ALL_PENDING                               0x00001010
#define   NFP3800_EM_ALL_PENDING_PENDING_STATUS(_x)          (((_x) & 0xffffffff) << 0)
#define   NFP3800_EM_ALL_PENDING_PENDING_STATUS_of(_x)       (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: Config - Configuration of the event manager
    Bits:  3:0  ClockDivide(rw) - Clock divider configuration for setting rate of throttling of events
*/
#define NFP3800_EM_CONFIG                                    0x00001020
#define   NFP3800_EM_CONFIG_CLKDIV(_x)                       (((_x) & 0xf) << 0)
#define   NFP3800_EM_CONFIG_CLKDIV_of(_x)                    (((_x) >> 0) & 0xf)

#else /* NFP3800_LONGNAMES */

/*----------------------------------------------------------------
  Register: Filter[32]_FilterStatus_Count32 - For FilterType = 0. 32-bit counter of events matching the filter
    Bits: 31:0  Count(ro) - Saturating 32 bit count of events that matched the filter
*/
#define NFP3800_EM_Filter_FilterStatus_Count32(_x)           (0x00000000 + (0x20 * ((_x) & 0x1f)))
#define   NFP3800_EM_Filter_FilterStatus_Count32_Count(_x)   (((_x) & 0xffffffff) << 0)
#define   NFP3800_EM_Filter_FilterStatus_Count32_Count_of(_x) (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: Filter[32]_FilterStatus_Count16 - For FilterType = 1. 16-bit counter of events matching the filter, with throttling and timeout
    Bits: 31:29 Timeout(rw) - 3-bit throttle timeout value
    Bits: 28:23 Upcounter(ro) - 6-bit throttle upcounter; if non-zero then status is throttled; set on acknowledge
    Bits: 21:16 Override(rw) - Ignore throttle if Count>Override and Override!=0
    Bits: 15:0  Count(ro) - Saturating 16 bit count of events that matched the filter
*/
#define NFP3800_EM_Filter_FilterStatus_Count16(_x)           (0x00000000 + (0x20 * ((_x) & 0x1f)))
#define   NFP3800_EM_Filter_FilterStatus_Count16_Timeout(_x) (((_x) & 0x7) << 29)
#define   NFP3800_EM_Filter_FilterStatus_Count16_Timeout_of(_x) (((_x) >> 29) & 0x7)
#define   NFP3800_EM_Filter_FilterStatus_Count16_Upcounter(_x) (((_x) & 0x3f) << 23)
#define   NFP3800_EM_Filter_FilterStatus_Count16_Upcounter_of(_x) (((_x) >> 23) & 0x3f)
#define   NFP3800_EM_Filter_FilterStatus_Count16_Override(_x) (((_x) & 0x3f) << 16)
#define   NFP3800_EM_Filter_FilterStatus_Count16_Override_of(_x) (((_x) >> 16) & 0x3f)
#define   NFP3800_EM_Filter_FilterStatus_Count16_Count(_x)   (((_x) & 0xffff) << 0)
#define   NFP3800_EM_Filter_FilterStatus_Count16_Count_of(_x) (((_x) >> 0) & 0xffff)
/*----------------------------------------------------------------
  Register: Filter[32]_FilterStatus_Bitmask32 - For FilterType = 2. 32-bit bitmask of events matching the filter
    Bits: 31:0  Bitmask(ro) - Bitmask of events that matched the filter; this register is reset when it is read
*/
#define NFP3800_EM_Filter_FilterStatus_Bitmask32(_x)         (0x00000000 + (0x20 * ((_x) & 0x1f)))
#define   NFP3800_EM_Filter_FilterStatus_Bitmask32_Bitmask(_x) (((_x) & 0xffffffff) << 0)
#define   NFP3800_EM_Filter_FilterStatus_Bitmask32_Bitmask_of(_x) (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: Filter[32]_FilterStatus_Bitmask16 - For FilterType = 3. 16-bit bitmask of events matching the filter, with throttling and timeout
    Bits: 31:29 Timeout(rw) - 3-bit throttle timeout value
    Bits: 28:23 Upcounter(ro) - 6-bit throttle upcounter; if non-zero then status is throttled; set on acknowledge
    Bits: 22:20 Override(rw) - Ignore throttle if Count>Override and Override!=0
    Bits: 19:16 Count(rw) - Saturating 4 bit count of events that matched the filter
    Bits: 15:0  Bitmask(ro) - Bitmask of events that matched the filter; reset on acknowledge
*/
#define NFP3800_EM_Filter_FilterStatus_Bitmask16(_x)         (0x00000000 + (0x20 * ((_x) & 0x1f)))
#define   NFP3800_EM_Filter_FilterStatus_Bitmask16_Timeout(_x) (((_x) & 0x7) << 29)
#define   NFP3800_EM_Filter_FilterStatus_Bitmask16_Timeout_of(_x) (((_x) >> 29) & 0x7)
#define   NFP3800_EM_Filter_FilterStatus_Bitmask16_Upcounter(_x) (((_x) & 0x3f) << 23)
#define   NFP3800_EM_Filter_FilterStatus_Bitmask16_Upcounter_of(_x) (((_x) >> 23) & 0x3f)
#define   NFP3800_EM_Filter_FilterStatus_Bitmask16_Override(_x) (((_x) & 0x7) << 20)
#define   NFP3800_EM_Filter_FilterStatus_Bitmask16_Override_of(_x) (((_x) >> 20) & 0x7)
#define   NFP3800_EM_Filter_FilterStatus_Bitmask16_Count(_x) (((_x) & 0xf) << 16)
#define   NFP3800_EM_Filter_FilterStatus_Bitmask16_Count_of(_x) (((_x) >> 16) & 0xf)
#define   NFP3800_EM_Filter_FilterStatus_Bitmask16_Bitmask(_x) (((_x) & 0xffff) << 0)
#define   NFP3800_EM_Filter_FilterStatus_Bitmask16_Bitmask_of(_x) (((_x) >> 0) & 0xffff)
/*----------------------------------------------------------------
  Register: Filter[32]_FilterStatus_Event - For FilterType = 4 (FirstEvent) or 5 (LastEvent). Recorded event which matched the filter, with throttling and timeout
    Bits: 31:29 Timeout(rw) - 3-bit throttle timeout value
    Bits: 28:23 Upcounter(ro) - 6-bit throttle upcounter; if non-zero then status is throttled; set on acknowledge
    Bits: 21:20 Count(ro) - Saturating 2 bit count of events that matched the filter; reset on acknowledge
    Bits: 19:0  Event(ro) - Provider, source and event number for the first or last event since the last acknowledge
*/
#define NFP3800_EM_Filter_FilterStatus_Event(_x)             (0x00000000 + (0x20 * ((_x) & 0x1f)))
#define   NFP3800_EM_Filter_FilterStatus_Event_Timeout(_x)   (((_x) & 0x7) << 29)
#define   NFP3800_EM_Filter_FilterStatus_Event_Timeout_of(_x) (((_x) >> 29) & 0x7)
#define   NFP3800_EM_Filter_FilterStatus_Event_Upcounter(_x) (((_x) & 0x3f) << 23)
#define   NFP3800_EM_Filter_FilterStatus_Event_Upcounter_of(_x) (((_x) >> 23) & 0x3f)
#define   NFP3800_EM_Filter_FilterStatus_Event_Count(_x)     (((_x) & 0x3) << 20)
#define   NFP3800_EM_Filter_FilterStatus_Event_Count_of(_x)  (((_x) >> 20) & 0x3)
#define   NFP3800_EM_Filter_FilterStatus_Event_Event(_x)     (((_x) & 0xfffff) << 0)
#define   NFP3800_EM_Filter_FilterStatus_Event_Event_of(_x)  (((_x) >> 0) & 0xfffff)
/*----------------------------------------------------------------
  Register: Filter[32]_FilterFlag - Filter Flag Register
    Bits:  1    PendingStatus(ro) - Asserted if the filter status flag would be asserted if not for throttling
    Bits:  0    Status(ro) - Asserted if the filter status is asserted
*/
#define NFP3800_EM_Filter_FilterFlag(_x)                     (0x00000010 + (0x20 * ((_x) & 0x1f)))
#define   NFP3800_EM_Filter_FilterFlag_PendingStatus         BIT(1)
#define   NFP3800_EM_Filter_FilterFlag_Status                BIT(0)
/*----------------------------------------------------------------
  Register: Filter[32]_FilterMask - Filter Mask Register
    Bits: 26:24 FilterType(rw) - Configuration type of the filter
    Bits: 23:0  EventMask(rw) - ANDed with event for filter event match
*/
#define NFP3800_EM_Filter_FilterMask(_x)                     (0x00000020 + (0x20 * ((_x) & 0x1f)))
#define   NFP3800_EM_Filter_FilterMask_FilterType(_x)        (((_x) & 0x7) << 24)
#define   NFP3800_EM_Filter_FilterMask_FilterType_of(_x)     (((_x) >> 24) & 0x7)
#define     NFP3800_EM_Filter_FilterMask_FilterType_COUNT32  (0)
#define     NFP3800_EM_Filter_FilterMask_FilterType_COUNT16  (1)
#define     NFP3800_EM_Filter_FilterMask_FilterType_MASK32   (2)
#define     NFP3800_EM_Filter_FilterMask_FilterType_MASK16   (3)
#define     NFP3800_EM_Filter_FilterMask_FilterType_FIRSTEV  (4)
#define     NFP3800_EM_Filter_FilterMask_FilterType_LASTEV   (5)
#define     NFP3800_EM_Filter_FilterMask_FilterType_FIRSTEVUT (6)
#define     NFP3800_EM_Filter_FilterMask_FilterType_LASTEVUT (7)
#define   NFP3800_EM_Filter_FilterMask_EventMask(_x)         (((_x) & 0xffffff) << 0)
#define   NFP3800_EM_Filter_FilterMask_EventMask_of(_x)      (((_x) >> 0) & 0xffffff)
/*----------------------------------------------------------------
  Register: Filter[32]_FilterMatch - Filter Match Register
    Bits: 23:0  EventMatch(rw) - Compared with event & EventMask to filter events
*/
#define NFP3800_EM_Filter_FilterMatch(_x)                    (0x00000030 + (0x20 * ((_x) & 0x1f)))
#define   NFP3800_EM_Filter_FilterMatch_EventMatch(_x)       (((_x) & 0xffffff) << 0)
#define   NFP3800_EM_Filter_FilterMatch_EventMatch_of(_x)    (((_x) >> 0) & 0xffffff)
/*----------------------------------------------------------------
  Register: Filter[32]_FilterAcknowledge - A read acknowledges the filter causing it to be reset
    Bits: 31:0  EvtStatus(rc) - Read Only. Acknowledges the filter, returning the contents of the FilterStatus Register and then resets it.
*/
#define NFP3800_EM_Filter_FilterAcknowledge(_x)              (0x00000040 + (0x20 * ((_x) & 0x1f)))
#define   NFP3800_EM_Filter_FilterAcknowledge_EvtStatus(_x)  (((_x) & 0xffffffff) << 0)
#define   NFP3800_EM_Filter_FilterAcknowledge_EvtStatus_of(_x) (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: CombinedStatus - 32 bits of status from the filters
    Bits: 31:0  Status(ro) - One bit per filter, indicating its status
*/
#define NFP3800_EM_CombinedStatus                            0x00001000
#define   NFP3800_EM_CombinedStatus_Status(_x)               (((_x) & 0xffffffff) << 0)
#define   NFP3800_EM_CombinedStatus_Status_of(_x)            (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: CombinedPendingStatus - 32 bits of pending status from the filters
    Bits: 31:0  PendingStatus(ro) - One bit per filter, indicating its pending status (status that would be presented if not throttling)
*/
#define NFP3800_EM_CombinedPendingStatus                     0x00001010
#define   NFP3800_EM_CombinedPendingStatus_PendingStatus(_x) (((_x) & 0xffffffff) << 0)
#define   NFP3800_EM_CombinedPendingStatus_PendingStatus_of(_x) (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: Config - Configuration of the event manager
    Bits:  3:0  ClockDivide(rw) - Clock divider configuration for setting rate of throttling of events
*/
#define NFP3800_EM_Config                                    0x00001020
#define   NFP3800_EM_Config_ClockDivide(_x)                  (((_x) & 0xf) << 0)
#define   NFP3800_EM_Config_ClockDivide_of(_x)               (((_x) >> 0) & 0xf)

#endif /* NFP3800_LONGNAMES */


#endif /* NFP3800_EM_H */
