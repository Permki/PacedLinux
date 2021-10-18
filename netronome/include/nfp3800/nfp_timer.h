/*
 * Copyright (C) 2013, Netronome Systems, Inc.  All rights reserved.
 *
 * Disclaimer: this file is provided without any warranty
 * as part of an early-access program, and the content is
 * bound to change before the final release.
 */

#ifndef NFP3800_TIMER_H
#define NFP3800_TIMER_H


/* HGID: 6ea1af6fca88 */

#ifndef NFP3800_LONGNAMES

/*----------------------------------------------------------------
  Register: Control0 - Determines timer0 functions, mode, activation.
  Register: Control1 - Determines timer1 functions, mode, activation.
  Register: Control2 - Determines timer2 functions, mode, activation.
  Register: Control3 - Determines timer3 functions, mode, activation.
    Bits:  7    Active(rw) - Set if the timer is activated. When activated the timer counter value is decremented on every trigger; if the value is zero already, though, the TimerCounterLoad value is placed in the counter instead.
    Bits:  3:2  Prescaler(rw) - Select the pre-scaler for a timer counter; this can be a division of the P_CLK clock, or based on pulses (rising edges) on GPIO pins. GPIO pin 0 is used for timer 0, GPIO pin 1 for timer 1, GPIO pin 2 for timer 2, GPIO pin 3 for timer 3.
*/
#define NFP3800_TIMER_CONTROL0                               0x00000000
#define NFP3800_TIMER_CONTROL1                               0x00000004
#define NFP3800_TIMER_CONTROL2                               0x00000008
#define NFP3800_TIMER_CONTROL3                               0x0000000c
#define   NFP3800_TIMER_TIMERCONTROL_ACTIVE                  BIT(7)
#define   NFP3800_TIMER_TIMERCONTROL_PRESCALER(_x)           (((_x) & 0x3) << 2)
#define   NFP3800_TIMER_TIMERCONTROL_PRESCALER_of(_x)        (((_x) >> 2) & 0x3)
#define     NFP3800_TIMER_TIMERCONTROL_PRESCALER_PS1         (0)
#define     NFP3800_TIMER_TIMERCONTROL_PRESCALER_PS16        (1)
#define     NFP3800_TIMER_TIMERCONTROL_PRESCALER_PS256       (2)
#define     NFP3800_TIMER_TIMERCONTROL_PRESCALER_PSGPIO      (3)
/*----------------------------------------------------------------
  Register: LoadValue0 - Holds the timer0 counter initial value.
  Register: LoadValue1 - Holds the timer1 counter initial value.
  Register: LoadValue2 - Holds the timer2 counter initial value.
  Register: LoadValue3 - Holds the timer3 counter initial value.
    Bits: 31:0  LoadValue(rw) - The counter load value for the timer. Writing to these registers causes the reload of the counters to this value.
*/
#define NFP3800_TIMER_LOADVALUE0                             0x00000010
#define NFP3800_TIMER_LOADVALUE1                             0x00000014
#define NFP3800_TIMER_LOADVALUE2                             0x00000018
#define NFP3800_TIMER_LOADVALUE3                             0x0000001c
#define   NFP3800_TIMER_TIMERCOUNTERLOADING_LOADVALUE(_x)    (((_x) & 0xffffffff) << 0)
#define   NFP3800_TIMER_TIMERCOUNTERLOADING_LOADVALUE_of(_x) (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: CounterValue0 - Current timer0 Counter Value.
  Register: CounterValue1 - Current timer1 Counter Value.
  Register: CounterValue2 - Current timer2 Counter Value.
  Register: CounterValue3 - Current timer3 Counter Value.
    Bits: 31:0  CounterValue(ro) - The current counter value for the timer.
*/
#define NFP3800_TIMER_COUNTERVALUE0                          0x00000020
#define NFP3800_TIMER_COUNTERVALUE1                          0x00000024
#define NFP3800_TIMER_COUNTERVALUE2                          0x00000028
#define NFP3800_TIMER_COUNTERVALUE3                          0x0000002c
#define   NFP3800_TIMER_TIMERCOUNTERSTATUS_COUNTERVALUE(_x)  (((_x) & 0xffffffff) << 0)
#define   NFP3800_TIMER_TIMERCOUNTERSTATUS_COUNTERVALUE_of(_x) (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: InterruptClear0 - Write 1 to clear the timer0 interrupt.
  Register: InterruptClear1 - Write 1 to clear the timer1 interrupt.
  Register: InterruptClear2 - Write 1 to clear the timer2 interrupt.
  Register: InterruptClear3 - Write 1 to clear the timer3 interrupt.
    Bits:  0    ClrInt(w1c) - Write a 1 to clear the interrupt status from the timer.
*/
#define NFP3800_TIMER_INTERRUPTCLEAR0                        0x00000030
#define NFP3800_TIMER_INTERRUPTCLEAR1                        0x00000034
#define NFP3800_TIMER_INTERRUPTCLEAR2                        0x00000038
#define NFP3800_TIMER_INTERRUPTCLEAR3                        0x0000003c
#define   NFP3800_TIMER_TIMERCOUNTERCLEAR_CLRINT             BIT(0)
#define     NFP3800_TIMER_TIMERCOUNTERCLEAR_CLRINT_NOOP      (0 << 0)
#define     NFP3800_TIMER_TIMERCOUNTERCLEAR_CLRINT_CI        BIT(0)
/*----------------------------------------------------------------
  Register: WatchdogEnable - Enables timer3 as a watchdog timer
    Bits:  0    WdogEn(rw) - Write a 1 to enable the watchdog; only cleared at reset. Read back 1 if watchdog function is enabled, else read back 0.
*/
#define NFP3800_TIMER_WATCHDOGENABLE                         0x00000040
#define   NFP3800_TIMER_WATCHDOGENABLE_WDOGEN                BIT(0)
#define     NFP3800_TIMER_WATCHDOGENABLE_WDOGEN_NOOP         (0 << 0)
#define     NFP3800_TIMER_WATCHDOGENABLE_WDOGEN_CI           BIT(0)

#else /* NFP3800_LONGNAMES */

/*----------------------------------------------------------------
  Register: Control0 - Determines timer0 functions, mode, activation.
  Register: Control1 - Determines timer1 functions, mode, activation.
  Register: Control2 - Determines timer2 functions, mode, activation.
  Register: Control3 - Determines timer3 functions, mode, activation.
    Bits:  7    Active(rw) - Set if the timer is activated. When activated the timer counter value is decremented on every trigger; if the value is zero already, though, the TimerCounterLoad value is placed in the counter instead.
    Bits:  3:2  Prescaler(rw) - Select the pre-scaler for a timer counter; this can be a division of the P_CLK clock, or based on pulses (rising edges) on GPIO pins. GPIO pin 0 is used for timer 0, GPIO pin 1 for timer 1, GPIO pin 2 for timer 2, GPIO pin 3 for timer 3.
*/
#define NFP3800_TIMER_Control0                               0x00000000
#define NFP3800_TIMER_Control1                               0x00000004
#define NFP3800_TIMER_Control2                               0x00000008
#define NFP3800_TIMER_Control3                               0x0000000c
#define   NFP3800_TIMER_TimerControl_Active                  BIT(7)
#define   NFP3800_TIMER_TimerControl_Prescaler(_x)           (((_x) & 0x3) << 2)
#define   NFP3800_TIMER_TimerControl_Prescaler_of(_x)        (((_x) >> 2) & 0x3)
#define     NFP3800_TIMER_TimerControl_Prescaler_PS1         (0)
#define     NFP3800_TIMER_TimerControl_Prescaler_PS16        (1)
#define     NFP3800_TIMER_TimerControl_Prescaler_PS256       (2)
#define     NFP3800_TIMER_TimerControl_Prescaler_PSGPIO      (3)
/*----------------------------------------------------------------
  Register: LoadValue0 - Holds the timer0 counter initial value.
  Register: LoadValue1 - Holds the timer1 counter initial value.
  Register: LoadValue2 - Holds the timer2 counter initial value.
  Register: LoadValue3 - Holds the timer3 counter initial value.
    Bits: 31:0  LoadValue(rw) - The counter load value for the timer. Writing to these registers causes the reload of the counters to this value.
*/
#define NFP3800_TIMER_LoadValue0                             0x00000010
#define NFP3800_TIMER_LoadValue1                             0x00000014
#define NFP3800_TIMER_LoadValue2                             0x00000018
#define NFP3800_TIMER_LoadValue3                             0x0000001c
#define   NFP3800_TIMER_TimerCounterLoading_LoadValue(_x)    (((_x) & 0xffffffff) << 0)
#define   NFP3800_TIMER_TimerCounterLoading_LoadValue_of(_x) (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: CounterValue0 - Current timer0 Counter Value.
  Register: CounterValue1 - Current timer1 Counter Value.
  Register: CounterValue2 - Current timer2 Counter Value.
  Register: CounterValue3 - Current timer3 Counter Value.
    Bits: 31:0  CounterValue(ro) - The current counter value for the timer.
*/
#define NFP3800_TIMER_CounterValue0                          0x00000020
#define NFP3800_TIMER_CounterValue1                          0x00000024
#define NFP3800_TIMER_CounterValue2                          0x00000028
#define NFP3800_TIMER_CounterValue3                          0x0000002c
#define   NFP3800_TIMER_TimerCounterStatus_CounterValue(_x)  (((_x) & 0xffffffff) << 0)
#define   NFP3800_TIMER_TimerCounterStatus_CounterValue_of(_x) (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: InterruptClear0 - Write 1 to clear the timer0 interrupt.
  Register: InterruptClear1 - Write 1 to clear the timer1 interrupt.
  Register: InterruptClear2 - Write 1 to clear the timer2 interrupt.
  Register: InterruptClear3 - Write 1 to clear the timer3 interrupt.
    Bits:  0    ClrInt(w1c) - Write a 1 to clear the interrupt status from the timer.
*/
#define NFP3800_TIMER_InterruptClear0                        0x00000030
#define NFP3800_TIMER_InterruptClear1                        0x00000034
#define NFP3800_TIMER_InterruptClear2                        0x00000038
#define NFP3800_TIMER_InterruptClear3                        0x0000003c
#define   NFP3800_TIMER_TimerCounterClear_ClrInt             BIT(0)
#define     NFP3800_TIMER_TimerCounterClear_ClrInt_NOOP      (0 << 0)
#define     NFP3800_TIMER_TimerCounterClear_ClrInt_CI        BIT(0)
/*----------------------------------------------------------------
  Register: WatchdogEnable - Enables timer3 as a watchdog timer
    Bits:  0    WdogEn(rw) - Write a 1 to enable the watchdog; only cleared at reset. Read back 1 if watchdog function is enabled, else read back 0.
*/
#define NFP3800_TIMER_WatchdogEnable                         0x00000040
#define   NFP3800_TIMER_WatchdogEnable_WdogEn                BIT(0)
#define     NFP3800_TIMER_WatchdogEnable_WdogEn_NOOP         (0 << 0)
#define     NFP3800_TIMER_WatchdogEnable_WdogEn_CI           BIT(0)

#endif /* NFP3800_LONGNAMES */


#endif /* NFP3800_TIMER_H */
