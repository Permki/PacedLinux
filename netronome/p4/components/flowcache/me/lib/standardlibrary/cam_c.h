/*
 * Copyright (C) 2014-2015,  Netronome Systems, Inc.  All rights reserved.
 *
 * @file          cam_c.h
 * @brief         Interface for ME CAM functions
 */

#ifndef _NFP__CAM_C_H_
#define _NFP__CAM_C_H_

#include <nfp.h>

#if defined(__NFP_LANG_MICROC)



/** This structure is used to capture the results of a CAM lookup.
 *
 * @see cam_lookup()
 */
typedef union
{
    struct
    {
        unsigned int zeros1         : 20;   /**< All zeros.                             */
        unsigned int state          : 4;    /**< CAM entry state.                       */
        unsigned int hit            : 1;    /**< hit (1) or miss (0).                   */
        unsigned int entry_num      : 4;    /**< CAM entry number.                      */
        unsigned int zeros2         : 3;    /**< All zeros.                             */
    };
    unsigned int value;                     /**< Accessor to all fields simultaneously  */
} cam_lookup_t;



/** Clear all entries in the CAM.
 *
 */
__intrinsic void cam_clear();


/** Write an entry in the CAM.
 *
 * @param entry_num CAM entry number to write
 * @param tag       Value to set for this CAM entry
 * @param cam_state State to set for this CAM entry
 *
 * Writes an entry in the CAM specified by the argument index with the value
 * specified by tag, and sets the state to the value specified in the argument state.
 * Argument state must be a constant literal specified directly in the intrinsics
 * argument list. Otherwise, the compiler may have to generate runtime checks for the possible 16
 * values, since the microcode only accepts a constant literal for the state.
 *
 */
__intrinsic void cam_write(unsigned int entry_num, unsigned int tag, unsigned int cam_state);


/** Perform a CAM lookup.
 *
 * @param tag       The value to lookup in the CAM
 * @return         Return hit/miss status, state, and entry number as bitfields
 *
 * Perform a CAM lookup and return the hit/miss status, state, and entry number as
 * bitfields in the return value. In the event of a miss, the entry value is the
 * LRU (least recently used) entry (which is the suggested entry to replace) and state
 * bits are 0. On a CAM hit, this function has the side effect of marking the CAM entry as
 * MRU (most recently used).
 *
 */
__intrinsic cam_lookup_t cam_lookup(unsigned int tag);


#endif /* __NFP_LANG_MICROC */

#endif /* !_NFP__CAM_C_H_ */
