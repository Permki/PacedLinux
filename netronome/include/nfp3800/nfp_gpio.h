/*
 * Copyright (C) 2013, Netronome Systems, Inc.  All rights reserved.
 *
 * Disclaimer: this file is provided without any warranty
 * as part of an early-access program, and the content is
 * bound to change before the final release.
 */

#ifndef NFP3800_GPIO_H
#define NFP3800_GPIO_H


/* HGID: 6ea1af6fca88 */

#ifndef NFP3800_LONGNAMES

/*----------------------------------------------------------------
  Register: PinLevel - Return the current value of all the GPIO pins
    Bits: 31:0  Value(rw) - Current value of the GPIO pins (0 = Low, 1 = High)
*/
#define NFP3800_GPIO_PINLEVEL                                0x00000000
#define   NFP3800_GPIO_PINLEVEL_VALUE(_x)                    (((_x) & 0xffffffff) << 0)
#define   NFP3800_GPIO_PINLEVEL_VALUE_of(_x)                 (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: PinDirection - Configuration of each GPIO pin as either input or output
    Bits: 31:0  Dirn(rw) - Sets the direction for all GPIO pins (0 = In, 1 = Out)
*/
#define NFP3800_GPIO_PINDIRECTION                            0x00000004
#define   NFP3800_GPIO_PINDIRECTION_DIRN(_x)                 (((_x) & 0xffffffff) << 0)
#define   NFP3800_GPIO_PINDIRECTION_DIRN_of(_x)              (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: PinDirectionSet - Make any set of GPIO pins to be outputs
    Bits: 31:0  SetValue(wo) - Write a 1 to make a GPIO pin to be an output
*/
#define NFP3800_GPIO_PINDIRECTIONSET                         0x00000008
#define   NFP3800_GPIO_PINDIRECTIONSET_SETVALUE(_x)          (((_x) & 0xffffffff) << 0)
#define   NFP3800_GPIO_PINDIRECTIONSET_SETVALUE_of(_x)       (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: PinDirectionClear - Make any set of GPIO pins to be inputs
    Bits: 31:0  ClrValue(wo) - Write a 1 to make a GPIO pin to be an input
*/
#define NFP3800_GPIO_PINDIRECTIONCLEAR                       0x0000000c
#define   NFP3800_GPIO_PINDIRECTIONCLEAR_CLRVALUE(_x)        (((_x) & 0xffffffff) << 0)
#define   NFP3800_GPIO_PINDIRECTIONCLEAR_CLRVALUE_of(_x)     (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: OutputDataProgram - Set levels for all GPIO outputs
    Bits: 31:0  Value(rw) - Values to be driven out on GPIO pins, for those pins configured as outputs.
*/
#define NFP3800_GPIO_OUTPUTDATAPROGRAM                       0x00000010
#define   NFP3800_GPIO_OUTPUTDATAPROGRAM_VALUE(_x)           (((_x) & 0xffffffff) << 0)
#define   NFP3800_GPIO_OUTPUTDATAPROGRAM_VALUE_of(_x)        (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: OutputDataSet - Write 1s to set individual GPIO outputs
    Bits: 31:0  SetValue(wo) - Write 1s to set bits of the output data value
*/
#define NFP3800_GPIO_OUTPUTDATASET                           0x00000014
#define   NFP3800_GPIO_OUTPUTDATASET_SETVALUE(_x)            (((_x) & 0xffffffff) << 0)
#define   NFP3800_GPIO_OUTPUTDATASET_SETVALUE_of(_x)         (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: OutputDataClear - Write 1s to clear individual GPIO outputs
    Bits: 31:0  ClrValue(wo) - Write 1s to clear bits of the output data value
*/
#define NFP3800_GPIO_OUTPUTDATACLEAR                         0x00000018
#define   NFP3800_GPIO_OUTPUTDATACLEAR_CLRVALUE(_x)          (((_x) & 0xffffffff) << 0)
#define   NFP3800_GPIO_OUTPUTDATACLEAR_CLRVALUE_of(_x)       (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: EdgeDetectRising - Enable detects on rising edge
    Bits: 31:0  Edges(rw) - Enable rising edge detection for selected GPIO pins
*/
#define NFP3800_GPIO_EDGEDETECTRISING                        0x0000001c
#define   NFP3800_GPIO_EDGEDETECTRISING_EDGES(_x)            (((_x) & 0xffffffff) << 0)
#define   NFP3800_GPIO_EDGEDETECTRISING_EDGES_of(_x)         (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: EdgeDetectFalling - Enable detects on falling edge
    Bits: 31:0  Edges(rw) - Enable falling edge detection for selected GPIO pins
*/
#define NFP3800_GPIO_EDGEDETECTFALLING                       0x00000020
#define   NFP3800_GPIO_EDGEDETECTFALLING_EDGES(_x)           (((_x) & 0xffffffff) << 0)
#define   NFP3800_GPIO_EDGEDETECTFALLING_EDGES_of(_x)        (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: EdgeDetectStatus - Logging of detected transactions
    Bits: 31:0  EdgeStatus(rw1c) - Reads return the current edge detected status for the GPIOs. Write a 1 to a bit to clear that edge detected status bit.
*/
#define NFP3800_GPIO_EDGEDETECTSTATUS                        0x00000024
#define   NFP3800_GPIO_EDGEDETECTSTATUS_EDGESTATUS(_x)       (((_x) & 0xffffffff) << 0)
#define   NFP3800_GPIO_EDGEDETECTSTATUS_EDGESTATUS_of(_x)    (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: LevelDetectHigh - Enable Detect on level sensitive high inputs
    Bits: 31:0  LevelHigh(rw) - Enable active high level detection for selected GPIO pins
*/
#define NFP3800_GPIO_LEVELDETECTHIGH                         0x00000028
#define   NFP3800_GPIO_LEVELDETECTHIGH_LEVELHIGH(_x)         (((_x) & 0xffffffff) << 0)
#define   NFP3800_GPIO_LEVELDETECTHIGH_LEVELHIGH_of(_x)      (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: LevelDetectLow - Enable Detect on level sensitive low inputs
    Bits: 31:0  LevelLow(rw) - Enable active low level detection for selected GPIO pins
*/
#define NFP3800_GPIO_LEVELDETECTLOW                          0x0000002c
#define   NFP3800_GPIO_LEVELDETECTLOW_LEVELLOW(_x)           (((_x) & 0xffffffff) << 0)
#define   NFP3800_GPIO_LEVELDETECTLOW_LEVELLOW_of(_x)        (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: LevelDetectStatus - Log the logic level of inputs
    Bits: 31:0  Status(rw) - Reads return the current level detected status for the GPIOs. Write a 1 to a bit to clear that level detected status bit.
*/
#define NFP3800_GPIO_LEVELDETECTSTATUS                       0x00000030
#define   NFP3800_GPIO_LEVELDETECTSTATUS_STATUS(_x)          (((_x) & 0xffffffff) << 0)
#define   NFP3800_GPIO_LEVELDETECTSTATUS_STATUS_of(_x)       (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: InterruptEnable - Enable the interrupt generation
    Bits: 31:0  IntEn(rw) - Enable mask for edge and level detection status
*/
#define NFP3800_GPIO_INTERRUPTENABLE                         0x00000034
#define   NFP3800_GPIO_INTERRUPTENABLE_INTEN(_x)             (((_x) & 0xffffffff) << 0)
#define   NFP3800_GPIO_INTERRUPTENABLE_INTEN_of(_x)          (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: InterruptEnableSet - Set the interrupt enable register
    Bits: 31:0  IntSetEn(rw) - Write a 1 to enable interrupts for a GPIO on both edge/level sensitivity status
*/
#define NFP3800_GPIO_INTERRUPTENABLESET                      0x00000038
#define   NFP3800_GPIO_INTERRUPTENABLESET_INTSETEN(_x)       (((_x) & 0xffffffff) << 0)
#define   NFP3800_GPIO_INTERRUPTENABLESET_INTSETEN_of(_x)    (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: InterruptEnableClear - Reset the interrupt enable register
    Bits: 31:0  IntResetEn(rw) - Write a 1 to disable interrupts for a GPIO on both edge/level sensitivity status
*/
#define NFP3800_GPIO_INTERRUPTENABLECLEAR                    0x0000003c
#define   NFP3800_GPIO_INTERRUPTENABLECLEAR_INTRESETEN(_x)   (((_x) & 0xffffffff) << 0)
#define   NFP3800_GPIO_INTERRUPTENABLECLEAR_INTRESETEN_of(_x) (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: InterruptStatus - Capture the interrupts occurred to the corresponding pin by the external devices
    Bits: 31:0  IntStatus(rw) - Interrupt generation status; clear a bit by writing 1 to it. Interrupt status is set for a GPIO when an enabled edge/level status bit is high.
*/
#define NFP3800_GPIO_INTERRUPTSTATUS                         0x00000040
#define   NFP3800_GPIO_INTERRUPTSTATUS_INTSTATUS(_x)         (((_x) & 0xffffffff) << 0)
#define   NFP3800_GPIO_INTERRUPTSTATUS_INTSTATUS_of(_x)      (((_x) >> 0) & 0xffffffff)

#else /* NFP3800_LONGNAMES */

/*----------------------------------------------------------------
  Register: PinLevel - Return the current value of all the GPIO pins
    Bits: 31:0  Value(rw) - Current value of the GPIO pins (0 = Low, 1 = High)
*/
#define NFP3800_GPIO_PinLevel                                0x00000000
#define   NFP3800_GPIO_PinLevel_Value(_x)                    (((_x) & 0xffffffff) << 0)
#define   NFP3800_GPIO_PinLevel_Value_of(_x)                 (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: PinDirection - Configuration of each GPIO pin as either input or output
    Bits: 31:0  Dirn(rw) - Sets the direction for all GPIO pins (0 = In, 1 = Out)
*/
#define NFP3800_GPIO_PinDirection                            0x00000004
#define   NFP3800_GPIO_PinDirection_Dirn(_x)                 (((_x) & 0xffffffff) << 0)
#define   NFP3800_GPIO_PinDirection_Dirn_of(_x)              (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: PinDirectionSet - Make any set of GPIO pins to be outputs
    Bits: 31:0  SetValue(wo) - Write a 1 to make a GPIO pin to be an output
*/
#define NFP3800_GPIO_PinDirectionSet                         0x00000008
#define   NFP3800_GPIO_PinDirectionSet_SetValue(_x)          (((_x) & 0xffffffff) << 0)
#define   NFP3800_GPIO_PinDirectionSet_SetValue_of(_x)       (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: PinDirectionClear - Make any set of GPIO pins to be inputs
    Bits: 31:0  ClrValue(wo) - Write a 1 to make a GPIO pin to be an input
*/
#define NFP3800_GPIO_PinDirectionClear                       0x0000000c
#define   NFP3800_GPIO_PinDirectionClear_ClrValue(_x)        (((_x) & 0xffffffff) << 0)
#define   NFP3800_GPIO_PinDirectionClear_ClrValue_of(_x)     (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: OutputDataProgram - Set levels for all GPIO outputs
    Bits: 31:0  Value(rw) - Values to be driven out on GPIO pins, for those pins configured as outputs.
*/
#define NFP3800_GPIO_OutputDataProgram                       0x00000010
#define   NFP3800_GPIO_OutputDataProgram_Value(_x)           (((_x) & 0xffffffff) << 0)
#define   NFP3800_GPIO_OutputDataProgram_Value_of(_x)        (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: OutputDataSet - Write 1s to set individual GPIO outputs
    Bits: 31:0  SetValue(wo) - Write 1s to set bits of the output data value
*/
#define NFP3800_GPIO_OutputDataSet                           0x00000014
#define   NFP3800_GPIO_OutputDataSet_SetValue(_x)            (((_x) & 0xffffffff) << 0)
#define   NFP3800_GPIO_OutputDataSet_SetValue_of(_x)         (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: OutputDataClear - Write 1s to clear individual GPIO outputs
    Bits: 31:0  ClrValue(wo) - Write 1s to clear bits of the output data value
*/
#define NFP3800_GPIO_OutputDataClear                         0x00000018
#define   NFP3800_GPIO_OutputDataClear_ClrValue(_x)          (((_x) & 0xffffffff) << 0)
#define   NFP3800_GPIO_OutputDataClear_ClrValue_of(_x)       (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: EdgeDetectRising - Enable detects on rising edge
    Bits: 31:0  Edges(rw) - Enable rising edge detection for selected GPIO pins
*/
#define NFP3800_GPIO_EdgeDetectRising                        0x0000001c
#define   NFP3800_GPIO_EdgeDetectRising_Edges(_x)            (((_x) & 0xffffffff) << 0)
#define   NFP3800_GPIO_EdgeDetectRising_Edges_of(_x)         (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: EdgeDetectFalling - Enable detects on falling edge
    Bits: 31:0  Edges(rw) - Enable falling edge detection for selected GPIO pins
*/
#define NFP3800_GPIO_EdgeDetectFalling                       0x00000020
#define   NFP3800_GPIO_EdgeDetectFalling_Edges(_x)           (((_x) & 0xffffffff) << 0)
#define   NFP3800_GPIO_EdgeDetectFalling_Edges_of(_x)        (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: EdgeDetectStatus - Logging of detected transactions
    Bits: 31:0  EdgeStatus(rw1c) - Reads return the current edge detected status for the GPIOs. Write a 1 to a bit to clear that edge detected status bit.
*/
#define NFP3800_GPIO_EdgeDetectStatus                        0x00000024
#define   NFP3800_GPIO_EdgeDetectStatus_EdgeStatus(_x)       (((_x) & 0xffffffff) << 0)
#define   NFP3800_GPIO_EdgeDetectStatus_EdgeStatus_of(_x)    (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: LevelDetectHigh - Enable Detect on level sensitive high inputs
    Bits: 31:0  LevelHigh(rw) - Enable active high level detection for selected GPIO pins
*/
#define NFP3800_GPIO_LevelDetectHigh                         0x00000028
#define   NFP3800_GPIO_LevelDetectHigh_LevelHigh(_x)         (((_x) & 0xffffffff) << 0)
#define   NFP3800_GPIO_LevelDetectHigh_LevelHigh_of(_x)      (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: LevelDetectLow - Enable Detect on level sensitive low inputs
    Bits: 31:0  LevelLow(rw) - Enable active low level detection for selected GPIO pins
*/
#define NFP3800_GPIO_LevelDetectLow                          0x0000002c
#define   NFP3800_GPIO_LevelDetectLow_LevelLow(_x)           (((_x) & 0xffffffff) << 0)
#define   NFP3800_GPIO_LevelDetectLow_LevelLow_of(_x)        (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: LevelDetectStatus - Log the logic level of inputs
    Bits: 31:0  Status(rw) - Reads return the current level detected status for the GPIOs. Write a 1 to a bit to clear that level detected status bit.
*/
#define NFP3800_GPIO_LevelDetectStatus                       0x00000030
#define   NFP3800_GPIO_LevelDetectStatus_Status(_x)          (((_x) & 0xffffffff) << 0)
#define   NFP3800_GPIO_LevelDetectStatus_Status_of(_x)       (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: InterruptEnable - Enable the interrupt generation
    Bits: 31:0  IntEn(rw) - Enable mask for edge and level detection status
*/
#define NFP3800_GPIO_InterruptEnable                         0x00000034
#define   NFP3800_GPIO_InterruptEnable_IntEn(_x)             (((_x) & 0xffffffff) << 0)
#define   NFP3800_GPIO_InterruptEnable_IntEn_of(_x)          (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: InterruptEnableSet - Set the interrupt enable register
    Bits: 31:0  IntSetEn(rw) - Write a 1 to enable interrupts for a GPIO on both edge/level sensitivity status
*/
#define NFP3800_GPIO_InterruptEnableSet                      0x00000038
#define   NFP3800_GPIO_InterruptEnableSet_IntSetEn(_x)       (((_x) & 0xffffffff) << 0)
#define   NFP3800_GPIO_InterruptEnableSet_IntSetEn_of(_x)    (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: InterruptEnableClear - Reset the interrupt enable register
    Bits: 31:0  IntResetEn(rw) - Write a 1 to disable interrupts for a GPIO on both edge/level sensitivity status
*/
#define NFP3800_GPIO_InterruptEnableClear                    0x0000003c
#define   NFP3800_GPIO_InterruptEnableClear_IntResetEn(_x)   (((_x) & 0xffffffff) << 0)
#define   NFP3800_GPIO_InterruptEnableClear_IntResetEn_of(_x) (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: InterruptStatus - Capture the interrupts occurred to the corresponding pin by the external devices
    Bits: 31:0  IntStatus(rw) - Interrupt generation status; clear a bit by writing 1 to it. Interrupt status is set for a GPIO when an enabled edge/level status bit is high.
*/
#define NFP3800_GPIO_InterruptStatus                         0x00000040
#define   NFP3800_GPIO_InterruptStatus_IntStatus(_x)         (((_x) & 0xffffffff) << 0)
#define   NFP3800_GPIO_InterruptStatus_IntStatus_of(_x)      (((_x) >> 0) & 0xffffffff)

#endif /* NFP3800_LONGNAMES */


#endif /* NFP3800_GPIO_H */
