/*
 * Copyright (C) 2013, Netronome Systems, Inc.  All rights reserved.
 *
 * Disclaimer: this file is provided without any warranty
 * as part of an early-access program, and the content is
 * bound to change before the final release.
 */

#ifndef NFP6000_QCTLR_H
#define NFP6000_QCTLR_H


/* HGID: 60515d2e075a */

#ifndef NFP6000_LONGNAMES

/*----------------------------------------------------------------
  Register: AddToReadPtr - Add to a queue's read pointer
    Bits: 17:0  Operand(wo) - Amount to add to the read pointer; if this would underflow the queue then the add will not happen and an underflow event is generated.
*/
#define NFP_QCTLR_ADD_RPTR                                   0x00000000
#define   NFP_QCTLR_ADD_RPTR_VAL(_x)                         (((_x) & 0x3ffff) << 0)
#define   NFP_QCTLR_ADD_RPTR_VAL_of(_x)                      (((_x) >> 0) & 0x3ffff)
/*----------------------------------------------------------------
  Register: AddToWritePtr - Add to a queue's write pointer
    Bits: 17:0  Operand(wo) - Amount to add to the write pointer; if this would overflow the queue then the add will not happen and an overflow event is generated.
*/
#define NFP_QCTLR_ADD_WPTR                                   0x00000004
#define   NFP_QCTLR_ADD_WPTR_VAL(_x)                         (((_x) & 0x3ffff) << 0)
#define   NFP_QCTLR_ADD_WPTR_VAL_of(_x)                      (((_x) >> 0) & 0x3ffff)
/*----------------------------------------------------------------
  Register: ConfigStatusLow - Configuration and Status Register for the Queue
    Bits: 31    ReadPtrEnable(wo) - If 1 then the writing of the read pointer is enabled and ECC checking disabled for this access; if 0 then ECC checking is not disabled and only the EventData and EventType fields are written. Events may be generated if the new event type differs from the previous event type.
    Bits: 30    Overflowed(ro) - Asserted if the queue has overflowed
    Bits: 29    Underflowed(ro) - Asserted if the queue has underflowed
    Bits: 28    WMReached(ro) - Asserted if the queue has more than or equal to Watermark entries
    Bits: 27    Full(ro) - Asserted if the queue is full
    Bits: 26    Empty(ro) - Asserted if the queue is empty
    Bits: 25:20 EventData(rw) - Data for top 6 bits of any generated event's 'data' field. The lower 6 bits of the queue number will be used as the lower 6-bits of any event's 'data' field. The most significant 4 bits of this field are used if the queue controller is configured to send 8 bits of queue number in the event 'data' field. This configuration is controlled by an external pin to this peripheral that should be controlled by a CSR of the instantiating design.
    Bits: 19:18 EventType(rw) - When to generate an event
    Bits: 17:0  ReadPtr(rw) - Value to write into the read pointer, if ReadPtrEnable bit is asserted
*/
#define NFP_QCTLR_STS_LO                                     0x00000008
#define   NFP_QCTLR_STS_LO_RPTR_ENABLE                       BIT(31)
#define   NFP_QCTLR_STS_LO_OVERFLOWED                        BIT(30)
#define   NFP_QCTLR_STS_LO_UNDERFLOWED                       BIT(29)
#define   NFP_QCTLR_STS_LO_WMREACHED                         BIT(28)
#define   NFP_QCTLR_STS_LO_FULL                              BIT(27)
#define   NFP_QCTLR_STS_LO_EMPTY                             BIT(26)
#define   NFP_QCTLR_STS_LO_EVENT_DATA(_x)                    (((_x) & 0x3f) << 20)
#define   NFP_QCTLR_STS_LO_EVENT_DATA_of(_x)                 (((_x) >> 20) & 0x3f)
#define   NFP_QCTLR_STS_LO_EVENT_TYPE(_x)                    (((_x) & 0x3) << 18)
#define   NFP_QCTLR_STS_LO_EVENT_TYPE_of(_x)                 (((_x) >> 18) & 0x3)
#define     NFP_QCTLR_STS_LO_EVENT_TYPE_NEVER                (0)
#define     NFP_QCTLR_STS_LO_EVENT_TYPE_NOT_EMPTY            (1)
#define     NFP_QCTLR_STS_LO_EVENT_TYPE_LO_WATERMARK         (2)
#define     NFP_QCTLR_STS_LO_EVENT_TYPE_HI_WATERMARK         (3)
#define   NFP_QCTLR_STS_LO_READPTR(_x)                       (((_x) & 0x3ffff) << 0)
#define   NFP_QCTLR_STS_LO_READPTR_of(_x)                    (((_x) >> 0) & 0x3ffff)
/*----------------------------------------------------------------
  Register: ConfigStatusHigh - Configuration and Status Register for the Queue
    Bits: 30    Overflowed(rw) - Asserted if the queue has overflowed
    Bits: 29    Underflowed(rw) - Asserted if the queue has underflowed
    Bits: 28    WMReached(rw) - Asserted if the queue has more than or equal to Watermark entries
    Bits: 27    Full(rw) - Asserted if the queue is full
    Bits: 26    Empty(rw) - Asserted if the queue is empty
    Bits: 24:22 Watermark(rw) - Logarithmic number of entries that makes the 'Watermark' for the queue.
    Bits: 21:18 Size(rw) - Logarithmic size of the queue
    Bits: 17:0  WritePtr(rw) - Write pointer for the queue
*/
#define NFP_QCTLR_STS_HI                                     0x0000000c
#define   NFP_QCTLR_STS_HI_OVERFLOWED                        BIT(30)
#define   NFP_QCTLR_STS_HI_UNDERFLOWED                       BIT(29)
#define   NFP_QCTLR_STS_HI_WMREACHED                         BIT(28)
#define   NFP_QCTLR_STS_HI_FULL                              BIT(27)
#define   NFP_QCTLR_STS_HI_EMPTY                             BIT(26)
#define   NFP_QCTLR_STS_HI_WATERMARK(_x)                     (((_x) & 0x7) << 22)
#define   NFP_QCTLR_STS_HI_WATERMARK_of(_x)                  (((_x) >> 22) & 0x7)
#define     NFP_QCTLR_STS_HI_WATERMARK_4                     (0)
#define     NFP_QCTLR_STS_HI_WATERMARK_8                     (1)
#define     NFP_QCTLR_STS_HI_WATERMARK_16                    (2)
#define     NFP_QCTLR_STS_HI_WATERMARK_32                    (3)
#define     NFP_QCTLR_STS_HI_WATERMARK_64                    (4)
#define     NFP_QCTLR_STS_HI_WATERMARK_128                   (5)
#define     NFP_QCTLR_STS_HI_WATERMARK_256                   (6)
#define     NFP_QCTLR_STS_HI_WATERMARK_512                   (7)
#define   NFP_QCTLR_STS_HI_SIZE(_x)                          (((_x) & 0xf) << 18)
#define   NFP_QCTLR_STS_HI_SIZE_of(_x)                       (((_x) >> 18) & 0xf)
#define     NFP_QCTLR_STS_HI_SIZE_256                        (0)
#define     NFP_QCTLR_STS_HI_SIZE_512                        (1)
#define     NFP_QCTLR_STS_HI_SIZE_1K                         (2)
#define     NFP_QCTLR_STS_HI_SIZE_2K                         (3)
#define     NFP_QCTLR_STS_HI_SIZE_4K                         (4)
#define     NFP_QCTLR_STS_HI_SIZE_8K                         (5)
#define     NFP_QCTLR_STS_HI_SIZE_16K                        (6)
#define     NFP_QCTLR_STS_HI_SIZE_32K                        (7)
#define     NFP_QCTLR_STS_HI_SIZE_64K                        (8)
#define     NFP_QCTLR_STS_HI_SIZE_128K                       (9)
#define     NFP_QCTLR_STS_HI_SIZE_256K                       (10)
#define   NFP_QCTLR_STS_HI_WRITEPTR(_x)                      (((_x) & 0x3ffff) << 0)
#define   NFP_QCTLR_STS_HI_WRITEPTR_of(_x)                   (((_x) >> 0) & 0x3ffff)

#else /* NFP6000_LONGNAMES */

/*----------------------------------------------------------------
  Register: AddToReadPtr - Add to a queue's read pointer
    Bits: 17:0  Operand(wo) - Amount to add to the read pointer; if this would underflow the queue then the add will not happen and an underflow event is generated.
*/
#define NFP_QCTLR_AddToReadPtr                               0x00000000
#define   NFP_QCTLR_AddToReadPtr_Operand(_x)                 (((_x) & 0x3ffff) << 0)
#define   NFP_QCTLR_AddToReadPtr_Operand_of(_x)              (((_x) >> 0) & 0x3ffff)
/*----------------------------------------------------------------
  Register: AddToWritePtr - Add to a queue's write pointer
    Bits: 17:0  Operand(wo) - Amount to add to the write pointer; if this would overflow the queue then the add will not happen and an overflow event is generated.
*/
#define NFP_QCTLR_AddToWritePtr                              0x00000004
#define   NFP_QCTLR_AddToWritePtr_Operand(_x)                (((_x) & 0x3ffff) << 0)
#define   NFP_QCTLR_AddToWritePtr_Operand_of(_x)             (((_x) >> 0) & 0x3ffff)
/*----------------------------------------------------------------
  Register: ConfigStatusLow - Configuration and Status Register for the Queue
    Bits: 31    ReadPtrEnable(wo) - If 1 then the writing of the read pointer is enabled and ECC checking disabled for this access; if 0 then ECC checking is not disabled and only the EventData and EventType fields are written. Events may be generated if the new event type differs from the previous event type.
    Bits: 30    Overflowed(ro) - Asserted if the queue has overflowed
    Bits: 29    Underflowed(ro) - Asserted if the queue has underflowed
    Bits: 28    WMReached(ro) - Asserted if the queue has more than or equal to Watermark entries
    Bits: 27    Full(ro) - Asserted if the queue is full
    Bits: 26    Empty(ro) - Asserted if the queue is empty
    Bits: 25:20 EventData(rw) - Data for top 6 bits of any generated event's 'data' field. The lower 6 bits of the queue number will be used as the lower 6-bits of any event's 'data' field. The most significant 4 bits of this field are used if the queue controller is configured to send 8 bits of queue number in the event 'data' field. This configuration is controlled by an external pin to this peripheral that should be controlled by a CSR of the instantiating design.
    Bits: 19:18 EventType(rw) - When to generate an event
    Bits: 17:0  ReadPtr(rw) - Value to write into the read pointer, if ReadPtrEnable bit is asserted
*/
#define NFP_QCTLR_ConfigStatusLow                            0x00000008
#define   NFP_QCTLR_ConfigStatusLow_ReadPtrEnable            BIT(31)
#define   NFP_QCTLR_ConfigStatusLow_Overflowed               BIT(30)
#define   NFP_QCTLR_ConfigStatusLow_Underflowed              BIT(29)
#define   NFP_QCTLR_ConfigStatusLow_WMReached                BIT(28)
#define   NFP_QCTLR_ConfigStatusLow_Full                     BIT(27)
#define   NFP_QCTLR_ConfigStatusLow_Empty                    BIT(26)
#define   NFP_QCTLR_ConfigStatusLow_EventData(_x)            (((_x) & 0x3f) << 20)
#define   NFP_QCTLR_ConfigStatusLow_EventData_of(_x)         (((_x) >> 20) & 0x3f)
#define   NFP_QCTLR_ConfigStatusLow_EventType(_x)            (((_x) & 0x3) << 18)
#define   NFP_QCTLR_ConfigStatusLow_EventType_of(_x)         (((_x) >> 18) & 0x3)
#define     NFP_QCTLR_ConfigStatusLow_EventType_NEVER        (0)
#define     NFP_QCTLR_ConfigStatusLow_EventType_NOT_EMPTY    (1)
#define     NFP_QCTLR_ConfigStatusLow_EventType_LO_WATERMARK (2)
#define     NFP_QCTLR_ConfigStatusLow_EventType_HI_WATERMARK (3)
#define   NFP_QCTLR_ConfigStatusLow_ReadPtr(_x)              (((_x) & 0x3ffff) << 0)
#define   NFP_QCTLR_ConfigStatusLow_ReadPtr_of(_x)           (((_x) >> 0) & 0x3ffff)
/*----------------------------------------------------------------
  Register: ConfigStatusHigh - Configuration and Status Register for the Queue
    Bits: 30    Overflowed(rw) - Asserted if the queue has overflowed
    Bits: 29    Underflowed(rw) - Asserted if the queue has underflowed
    Bits: 28    WMReached(rw) - Asserted if the queue has more than or equal to Watermark entries
    Bits: 27    Full(rw) - Asserted if the queue is full
    Bits: 26    Empty(rw) - Asserted if the queue is empty
    Bits: 24:22 Watermark(rw) - Logarithmic number of entries that makes the 'Watermark' for the queue.
    Bits: 21:18 Size(rw) - Logarithmic size of the queue
    Bits: 17:0  WritePtr(rw) - Write pointer for the queue
*/
#define NFP_QCTLR_ConfigStatusHigh                           0x0000000c
#define   NFP_QCTLR_ConfigStatusHigh_Overflowed              BIT(30)
#define   NFP_QCTLR_ConfigStatusHigh_Underflowed             BIT(29)
#define   NFP_QCTLR_ConfigStatusHigh_WMReached               BIT(28)
#define   NFP_QCTLR_ConfigStatusHigh_Full                    BIT(27)
#define   NFP_QCTLR_ConfigStatusHigh_Empty                   BIT(26)
#define   NFP_QCTLR_ConfigStatusHigh_Watermark(_x)           (((_x) & 0x7) << 22)
#define   NFP_QCTLR_ConfigStatusHigh_Watermark_of(_x)        (((_x) >> 22) & 0x7)
#define     NFP_QCTLR_ConfigStatusHigh_Watermark_4           (0)
#define     NFP_QCTLR_ConfigStatusHigh_Watermark_8           (1)
#define     NFP_QCTLR_ConfigStatusHigh_Watermark_16          (2)
#define     NFP_QCTLR_ConfigStatusHigh_Watermark_32          (3)
#define     NFP_QCTLR_ConfigStatusHigh_Watermark_64          (4)
#define     NFP_QCTLR_ConfigStatusHigh_Watermark_128         (5)
#define     NFP_QCTLR_ConfigStatusHigh_Watermark_256         (6)
#define     NFP_QCTLR_ConfigStatusHigh_Watermark_512         (7)
#define   NFP_QCTLR_ConfigStatusHigh_Size(_x)                (((_x) & 0xf) << 18)
#define   NFP_QCTLR_ConfigStatusHigh_Size_of(_x)             (((_x) >> 18) & 0xf)
#define     NFP_QCTLR_ConfigStatusHigh_Size_256              (0)
#define     NFP_QCTLR_ConfigStatusHigh_Size_512              (1)
#define     NFP_QCTLR_ConfigStatusHigh_Size_1K               (2)
#define     NFP_QCTLR_ConfigStatusHigh_Size_2K               (3)
#define     NFP_QCTLR_ConfigStatusHigh_Size_4K               (4)
#define     NFP_QCTLR_ConfigStatusHigh_Size_8K               (5)
#define     NFP_QCTLR_ConfigStatusHigh_Size_16K              (6)
#define     NFP_QCTLR_ConfigStatusHigh_Size_32K              (7)
#define     NFP_QCTLR_ConfigStatusHigh_Size_64K              (8)
#define     NFP_QCTLR_ConfigStatusHigh_Size_128K             (9)
#define     NFP_QCTLR_ConfigStatusHigh_Size_256K             (10)
#define   NFP_QCTLR_ConfigStatusHigh_WritePtr(_x)            (((_x) & 0x3ffff) << 0)
#define   NFP_QCTLR_ConfigStatusHigh_WritePtr_of(_x)         (((_x) >> 0) & 0x3ffff)

#endif /* NFP6000_LONGNAMES */


#endif /* NFP6000_QCTLR_H */
