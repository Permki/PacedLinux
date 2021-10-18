/*
 * Copyright (C) 2008-2014 Netronome Systems, Inc.  All rights reserved.
 */

#ifndef __ERRCODE_H__
#define __ERRCODE_H__

#ifndef NFP_LIB_ANY_NFCC_VERSION
    #if (!defined(__NFP_TOOL_NFCC) ||                       \
        (__NFP_TOOL_NFCC < NFP_SW_VERSION(5, 0, 0, 0)) ||   \
        (__NFP_TOOL_NFCC > NFP_SW_VERSION(6, 1, 0, 255)))
        #error "This standard library is not supported for the version of the SDK currently in use."
    #endif
#endif

/** @file errcode.h
 * @addtogroup mutex Mutex/Semaphore Functions
 * @{
 */

/** @name Common error codes for mutex/semaphores library
 * @{
 */

/**
 * Available error codes
 */

typedef enum
{
    /** No Error (OK)
     */
    ERRCODE_EOK,

    /** Could not lock mutex/semaphore
     */
    ERRCODE_ENOLCK,

    /** Mutex/semaphore is in use (busy or spinning)
     */
    ERRCODE_EBUSY,

    /** Invalid mutex/semaphore handle
     */
    ERRCODE_EINVAL,

    /** Failed to get mutex/semaphore (as result of try)
     */
    ERRCODE_AGAIN
} ERRCODE;

/** @}
 * @}
 */

#endif /* __ERRCODE_H__ */
