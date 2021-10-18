/*
 * Copyright (C) 2013, Netronome Systems, Inc.  All rights reserved.
 *
 * Disclaimer: this file is provided without any warranty
 * as part of an early-access program, and the content is
 * bound to change before the final release.
 */

#ifndef NFP6000_ECC_H
#define NFP6000_ECC_H


/* HGID: 60515d2e075a */

#ifndef NFP6000_LONGNAMES

/*----------------------------------------------------------------
  Register: ECCEnable - ECC detection and correction enable
    Bits:  0    Enable(rw) - ECCEnable Register Description
*/
#define NFP_ECC_ENABLE                                       0x00000000
#define   NFP_ECC_ENABLE_ALL                                 BIT(0)
#define     NFP_ECC_ENABLE_ALL_DIS                           (0 << 0)
#define     NFP_ECC_ENABLE_ALL_EN                            BIT(0)
/*----------------------------------------------------------------
  Register: InjectClear - Clear any outstanding ECC error injection
*/
#define NFP_ECC_INJECT_CLEAR                                 0x00000004
/*----------------------------------------------------------------
  Register: InjectECCPermanent - Inject ECC errors into the ECC field continuously on matching writes
  Register: InjectDataPermanent - Inject ECC errors into the data field continuously on matching writes
  Register: InjectECCOneShot - Inject an ECC error into the ECC field on next matching write
  Register: InjectDataOneShot - Inject an ECC error into the data field on next matching write
    Bits: 15:8  AddressMatch(wo) - 8-bit address to match for ECC poisoning
    Bits:  7:0  ECCDelta(wo) - ECC Delta to derive a single bit data error from or to be inserted if ECC poisoning requested
*/
#define NFP_ECC_INJECT_ECC_PERM                              0x00000008
#define NFP_ECC_INJECT_DATA_PERM                             0x0000000c
#define NFP_ECC_INJECT_ECC_ONCE                              0x00000010
#define NFP_ECC_INJECT_DATA_ONCE                             0x00000014
#define   NFP_ECC_ECCINJECTERROR_ADDR_MATCH(_x)              (((_x) & 0xff) << 8)
#define   NFP_ECC_ECCINJECTERROR_ADDR_MATCH_of(_x)           (((_x) >> 8) & 0xff)
#define   NFP_ECC_ECCINJECTERROR_DELTA(_x)                   (((_x) & 0xff) << 0)
#define   NFP_ECC_ECCINJECTERROR_DELTA_of(_x)                (((_x) >> 0) & 0xff)
/*----------------------------------------------------------------
  Register: FirstECCError - First ECC error to occur since errors were reset.
  Register: LastECCError - Read only - Most recent ECC error
    Bits: 31    Valid(ro) - Contents of reg are valid
    Bits: 25    MultibitError(ro) - MultibitError - Set if the error was a multi-bit (uncorrectable) error
    Bits: 24    SinglebitError(ro) - SinglebitError - Set if the error was a single-bit (corrected) error
    Bits: 23:16 ECCDelta(ro) - ECCDelta - ECC delta of the error (XOR of expected and read)
    Bits: 15:0  Address(ro) - Address - 16-bit address where the error occurred
*/
#define NFP_ECC_ERR_FIRST                                    0x00000020
#define NFP_ECC_ERR_LAST                                     0x00000024
#define   NFP_ECC_ECCERROR_VALID                             BIT(31)
#define     NFP_ECC_ECCERROR_VALID_NOERRS                    (0 << 31)
#define     NFP_ECC_ECCERROR_VALID_ERRS                      BIT(31)
#define   NFP_ECC_ECCERROR_MULTIBIT                          BIT(25)
#define   NFP_ECC_ECCERROR_SINGLEBIT                         BIT(24)
#define   NFP_ECC_ECCERROR_DELTA(_x)                         (((_x) & 0xff) << 16)
#define   NFP_ECC_ECCERROR_DELTA_of(_x)                      (((_x) >> 16) & 0xff)
#define   NFP_ECC_ECCERROR_ADDR(_x)                          (((_x) & 0xffff) << 0)
#define   NFP_ECC_ECCERROR_ADDR_of(_x)                       (((_x) >> 0) & 0xffff)
/*----------------------------------------------------------------
  Register: ErrorCounts - Read only - Count of ECC errors, without clearing count
  Register: ErrorCountsReset - Read only - Count of ECC errors, clears count on read
    Bits: 31:16 MultibitCount(ro) - Multibit error count - 16-bit saturation count of multibit errors
    Bits: 15:0  SinglebitCount(ro) - Singlebit error count - 16-bit saturated count of singlebit errors
*/
#define NFP_ECC_ERR_COUNT                                    0x00000028
#define NFP_ECC_ERR_COUNT_RESET                              0x0000002c
#define   NFP_ECC_ECCERRORCOUNTS_MULTIS(_x)                  (((_x) & 0xffff) << 16)
#define   NFP_ECC_ECCERRORCOUNTS_MULTIS_of(_x)               (((_x) >> 16) & 0xffff)
#define   NFP_ECC_ECCERRORCOUNTS_SINGLES(_x)                 (((_x) & 0xffff) << 0)
#define   NFP_ECC_ECCERRORCOUNTS_SINGLES_of(_x)              (((_x) >> 0) & 0xffff)
/*----------------------------------------------------------------
  Register: LongAddress - Read-only, for long address ECC (eg. DRAM interface) only
    Bits: 31:0  Address(ro) - Full address of error from last read of FirstECCError or LastECCError for any of the ECC monitor spaces
*/
#define NFP_ECC_LONG_ADDR                                    0x00000034
#define   NFP_ECC_LONG_ADDR_LADDR(_x)                        (((_x) & 0xffffffff) << 0)
#define   NFP_ECC_LONG_ADDR_LADDR_of(_x)                     (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: ClearErrors - Write only - clears all error counts
*/
#define NFP_ECC_CLEAR_ERR                                    0x00000038

#else /* NFP6000_LONGNAMES */

/*----------------------------------------------------------------
  Register: ECCEnable - ECC detection and correction enable
    Bits:  0    Enable(rw) - ECCEnable Register Description
*/
#define NFP_ECC_ECCEnable                                    0x00000000
#define   NFP_ECC_ECCEnable_Enable                           BIT(0)
#define     NFP_ECC_ECCEnable_Enable_DIS                     (0 << 0)
#define     NFP_ECC_ECCEnable_Enable_EN                      BIT(0)
/*----------------------------------------------------------------
  Register: InjectClear - Clear any outstanding ECC error injection
*/
#define NFP_ECC_InjectClear                                  0x00000004
/*----------------------------------------------------------------
  Register: InjectECCPermanent - Inject ECC errors into the ECC field continuously on matching writes
  Register: InjectDataPermanent - Inject ECC errors into the data field continuously on matching writes
  Register: InjectECCOneShot - Inject an ECC error into the ECC field on next matching write
  Register: InjectDataOneShot - Inject an ECC error into the data field on next matching write
    Bits: 15:8  AddressMatch(wo) - 8-bit address to match for ECC poisoning
    Bits:  7:0  ECCDelta(wo) - ECC Delta to derive a single bit data error from or to be inserted if ECC poisoning requested
*/
#define NFP_ECC_InjectECCPermanent                           0x00000008
#define NFP_ECC_InjectDataPermanent                          0x0000000c
#define NFP_ECC_InjectECCOneShot                             0x00000010
#define NFP_ECC_InjectDataOneShot                            0x00000014
#define   NFP_ECC_ECCInjectError_AddressMatch(_x)            (((_x) & 0xff) << 8)
#define   NFP_ECC_ECCInjectError_AddressMatch_of(_x)         (((_x) >> 8) & 0xff)
#define   NFP_ECC_ECCInjectError_ECCDelta(_x)                (((_x) & 0xff) << 0)
#define   NFP_ECC_ECCInjectError_ECCDelta_of(_x)             (((_x) >> 0) & 0xff)
/*----------------------------------------------------------------
  Register: FirstECCError - First ECC error to occur since errors were reset.
  Register: LastECCError - Read only - Most recent ECC error
    Bits: 31    Valid(ro) - Contents of reg are valid
    Bits: 25    MultibitError(ro) - MultibitError - Set if the error was a multi-bit (uncorrectable) error
    Bits: 24    SinglebitError(ro) - SinglebitError - Set if the error was a single-bit (corrected) error
    Bits: 23:16 ECCDelta(ro) - ECCDelta - ECC delta of the error (XOR of expected and read)
    Bits: 15:0  Address(ro) - Address - 16-bit address where the error occurred
*/
#define NFP_ECC_FirstECCError                                0x00000020
#define NFP_ECC_LastECCError                                 0x00000024
#define   NFP_ECC_ECCError_Valid                             BIT(31)
#define     NFP_ECC_ECCError_Valid_NOERRS                    (0 << 31)
#define     NFP_ECC_ECCError_Valid_ERRS                      BIT(31)
#define   NFP_ECC_ECCError_MultibitError                     BIT(25)
#define   NFP_ECC_ECCError_SinglebitError                    BIT(24)
#define   NFP_ECC_ECCError_ECCDelta(_x)                      (((_x) & 0xff) << 16)
#define   NFP_ECC_ECCError_ECCDelta_of(_x)                   (((_x) >> 16) & 0xff)
#define   NFP_ECC_ECCError_Address(_x)                       (((_x) & 0xffff) << 0)
#define   NFP_ECC_ECCError_Address_of(_x)                    (((_x) >> 0) & 0xffff)
/*----------------------------------------------------------------
  Register: ErrorCounts - Read only - Count of ECC errors, without clearing count
  Register: ErrorCountsReset - Read only - Count of ECC errors, clears count on read
    Bits: 31:16 MultibitCount(ro) - Multibit error count - 16-bit saturation count of multibit errors
    Bits: 15:0  SinglebitCount(ro) - Singlebit error count - 16-bit saturated count of singlebit errors
*/
#define NFP_ECC_ErrorCounts                                  0x00000028
#define NFP_ECC_ErrorCountsReset                             0x0000002c
#define   NFP_ECC_ECCErrorCounts_MultibitCount(_x)           (((_x) & 0xffff) << 16)
#define   NFP_ECC_ECCErrorCounts_MultibitCount_of(_x)        (((_x) >> 16) & 0xffff)
#define   NFP_ECC_ECCErrorCounts_SinglebitCount(_x)          (((_x) & 0xffff) << 0)
#define   NFP_ECC_ECCErrorCounts_SinglebitCount_of(_x)       (((_x) >> 0) & 0xffff)
/*----------------------------------------------------------------
  Register: LongAddress - Read-only, for long address ECC (eg. DRAM interface) only
    Bits: 31:0  Address(ro) - Full address of error from last read of FirstECCError or LastECCError for any of the ECC monitor spaces
*/
#define NFP_ECC_LongAddress                                  0x00000034
#define   NFP_ECC_LongAddress_Address(_x)                    (((_x) & 0xffffffff) << 0)
#define   NFP_ECC_LongAddress_Address_of(_x)                 (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: ClearErrors - Write only - clears all error counts
*/
#define NFP_ECC_ClearErrors                                  0x00000038

#endif /* NFP6000_LONGNAMES */


#endif /* NFP6000_ECC_H */
