/*
 * Copyright (C) 2008-2011 Netronome Systems, Inc.  All rights reserved.
 */

#ifndef __CAM_UC__
#define __CAM_UC__

#ifndef NFP_LIB_ANY_NFAS_VERSION
    #if (!defined(__NFP_TOOL_NFAS) ||                       \
        (__NFP_TOOL_NFAS < NFP_SW_VERSION(5, 0, 0, 0)) ||   \
        (__NFP_TOOL_NFAS > NFP_SW_VERSION(6, 1, 0, 255)))
        #error "This standard library is not supported for the version of the SDK currently in use."
    #endif
#endif

#include <stdmac.uc>

/** @file cam.uc Microengine CAM Operation Macros
 * @addtogroup me_cam Microengine CAM Operation
 * @{
 *
 * @name Microengine CAM Operation Macros
 * @{
 * Microengine CAM Operation Macros
 */

// API Quick Reference:
//
// cam_clear_all()
// cam_read_entry out_data, out_state, in_entry_num)
// cam_write_entry(in_entry_num, in_data, in_state)
// cam_match(out_status, out_entry_num, out_state, in_data)



/** Clear all the CAM entries.
 *
 * @instruction_cnt 1
 */
#macro cam_clear_all()
    cam_clear
#endm


/** Read the data and state for the specified CAM entry.
 *
 * @param out_data      32-bit data read from entry
 * @param out_state     4-bit state data associated with this CAM entry
 * @param in_entry_num  CAM entry number
 *
 * @instruction_cnt 3
 */
#macro cam_read_entry(out_data, out_state, in_entry_num)
    cam_read_tag[out_data, in_entry_num]    // Read data from in_entry_num#
    cam_read_state[out_state, in_entry_num] // Read state from in_entry_num#
    and_shf_right(out_state, 0xF, out_state, 8)
#endm


/** Write the data and state for the specified CAM entry.
 *
 * @param in_entry_num  CAM entry number
 * @param in_data       32-bit data to be written to specified CAM entry
 * @param in_state      4-bit state data to be associated with this CAM entry
 *
 * @instruction_cnt 1 to 3
 */
#macro cam_write_entry(in_entry_num, in_data, in_state)
.begin

    #if (is_ct_const(in_entry_num) || isimport(in_entry_num))
        // if both entry_num and in_data are numeric then we need to put in_data into gpr
        #if (is_ct_const(in_data) || isimport(in_data))
            .reg _in_data_reg
            immed32(_in_data_reg, in_data)
            #define_eval _IN_DATA_REG_NAME _in_data_reg
        #else
            #define_eval _IN_DATA_REG_NAME in_data
        #endif
    #else
        #if (is_ct_const(in_data) || isimport(in_data))
            // if the value > MAX_IMMEDIATE then move it into register
            #if ( (in_data > MAX_IMMEDIATE) || (in_data < 0 ))
                .reg _in_data_reg
                immed32(_in_data_reg, in_data)
                #define_eval _IN_DATA_REG_NAME _in_data_reg
            #else
                #define_eval _IN_DATA_REG_NAME in_data
            #endif
        #else
            #define_eval _IN_DATA_REG_NAME in_data
        #endif
    #endif

    cam_write[in_entry_num, _IN_DATA_REG_NAME, in_state] // Write data and state into in_entry_num#

.end
#endm


/** Read the data for the specified CAM entry.
 *
 * @param out_data      32-bit data read from entry
 * @param in_entry_num  CAM entry number
 *
 * @instruction_cnt 1
 */
#macro cam_read_data(out_data, in_entry_num)
    cam_read_tag[out_data, in_entry_num] // Read data from in_entry_num#
#endm


/** Perform content match to get a CAM entry.
 *
 * @param out_state         If @p out_status is 1 (hit), this contains the appropriate state value.@n
 *                          If @p out_status is 0 (miss), this contains 0.
 * @param out_status        Match status. A value of 1 means Hit, 0 means Miss.
 * @param out_entry_num     If @p out_status is 1 (hit), this contains the matched entry number.@n
 *                          If @p out_status is 0 (miss), this contains the least recently used (LRU) entry number.@n
 *                          The LRU entry number can be used as a hint for writing new data into CAM.
 * @param in_data           32-bit data to match
 *
 * @instruction_cnt 4 to 6
 */
#macro cam_match(out_state, out_status, out_entry_num, in_data)
.begin

    .reg out_result

    #if (is_ct_const(in_data) || isimport(in_data))
        // if the value > MAX_IMMEDIATE then move it into register
        #if ( (in_data > MAX_IMMEDIATE) || (in_data < 0 ))
            .reg _out_reg
            immed32(_out_reg, in_data)
            #define_eval _NEW_REG_NAME _out_reg
        #else
            #define_eval _NEW_REG_NAME in_data
        #endif
    #else
        #define_eval _NEW_REG_NAME in_data
    #endif

    cam_lookup[out_result, _NEW_REG_NAME]

    and_shf_right(out_entry_num, 0xF, out_result, 3)
    and_shf_right(out_state, 0xF, out_result, 8)
    and_shf_right(out_status, 0x1, out_result, 7)

.end
#endm


/** @}
 * @}
 */

#endif /* __CAM_UC__ */
