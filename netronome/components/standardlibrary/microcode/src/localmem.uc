/*
 * Copyright (C) 2008-2013 Netronome Systems, Inc.  All rights reserved.
 */

#ifndef __LOCALMEM_UC__
#define __LOCALMEM_UC__

#ifndef NFP_LIB_ANY_NFAS_VERSION
    #if (!defined(__NFP_TOOL_NFAS) ||                       \
        (__NFP_TOOL_NFAS < NFP_SW_VERSION(5, 0, 0, 0)) ||   \
        (__NFP_TOOL_NFAS > NFP_SW_VERSION(6, 1, 0, 255)))
        #error "This standard library is not supported for the version of the SDK currently in use."
    #endif
#endif

/** @file localmem.uc Microengine Local Memory Macros
 * @addtogroup me_lmem Microengine Local Memory
 * @{
 *
 * @name Microengine Local Memory Macros
 * @{
 *
 */

#include <stdmac.uc>
#include <util.uc>

// API Quick Reference:
//
// localmem_set_address(in_lmaddr, in_addr_offset, LM_HANDLE)
// localmem_set_active_handle(in_reg_num)
// localmem_read1(out_data, in_lmaddr, in_addr_offset)
// localmem_read2(out_data0, out_data1, in_lmaddr, in_addr_offset)
//
// localmem_read8(out_data0, out_data1, out_data2, out_data3, out_data4,
//                out_data5, out_data6, out_data7, in_lmaddr, in_addr_offset)
// localmem_write1(in_data, in_lmaddr, in_addr_offset)
// localmem_write2(in_data0, in_data1, in_lmaddr, in_addr_offset)
//
// localmem_write8(in_data0, in_data1, in_data2, in_data3, in_data4, in_data5,
//                 in_data6, in_data7, in_lmaddr, in_addr_offset)
// localmem_read_next(out_data)
// localmem_write_next(in_data)
// localmem_bzero()



// local memory handle constants
#ifdef LM_HANDLE_0
    #warning "LM_HANDLE_0 is being redefined"
#endif

#ifdef LM_HANDLE_1
    #warning "LM_HANDLE_1 is being redefined"
#endif

#if (IS_NFPTYPE(__NFP6000) || IS_NFPTYPE(__NFP3800))
    #ifdef LM_HANDLE_2
           #warning "LM_HANDLE_2 is being redefined"
    #endif

    #ifdef LM_HANDLE_3
           #warning "LM_HANDLE_3 is being redefined"
    #endif
#endif

#define_eval LM_HANDLE_0    0
#define_eval LM_HANDLE_1    1

#if (IS_NFPTYPE(__NFP6000) || IS_NFPTYPE(__NFP3800))
    #define_eval LM_HANDLE_2    2
    #define_eval LM_HANDLE_3    3
#endif

#ifdef __CURR_LM_INDEX
    #warning "__CURR_LM_INDEX is being redefined"
#endif

#define_eval    __CURR_LM_INDEX         0   /**< Default to active_lm_addr_0 by setting __CURR_LM_INDEX to 0. @see localmem_set_active_handle */


/// @cond INTERNAL_MACROS
/** Internal macro used to set CSRs.
 *
 * @param in_lmaddr         Local memory base address
 * @param in_addr_offset    Local memory offset
 *
 * @note After this call to macro need to wait for 3 cycle latency for local memory.
 *
 * @instruction_cnt 1 to 4
 */
#macro _localmem_setcsr(in_lmaddr, in_addr_offset)
.begin

    #ifdef LMADDR_IS_ZERO
        #warning "LMADDR_IS_ZERO is being redefined"
    #endif

    #ifdef LM_OFFSET_IS_ZERO
        #warning "LM_OFFSET_IS_ZERO is being redefined"
    #endif

    #define_eval LMADDR_IS_ZERO     0
    #define_eval LM_OFFSET_IS_ZERO  0

    #if (is_ct_const(in_lmaddr) || isimport(in_lmaddr))
        #define_eval  __IN_LMADDR_VALUE in_lmaddr

        #if (__IN_LMADDR_VALUE == 0)
            #define_eval LMADDR_IS_ZERO     1
        #endif

        // if the value > MAX_IMMEDIATE or < 0, then move it into register
        #if ((__IN_LMADDR_VALUE > MAX_IMMEDIATE) || (__IN_LMADDR_VALUE < 0))
            .reg in_lmaddr_reg
            immed32(in_lmaddr_reg, __IN_LMADDR_VALUE)
            #define_eval __IN_LMADDR_REG_NAME   in_lmaddr_reg

        #else
            #define_eval __IN_LMADDR_REG_NAME   in_lmaddr
        #endif

        #undef __IN_LMADDR_VALUE    // clean up name space
    #else
        #define_eval __IN_LMADDR_REG_NAME in_lmaddr
    #endif


    #if (is_ct_const(in_addr_offset) || isimport(in_addr_offset))
        #define_eval  __IN_ADDR_OFFSET_VALUE in_addr_offset

        #if (__IN_ADDR_OFFSET_VALUE == 0)
            #define_eval LM_OFFSET_IS_ZERO  1
        #endif

        // if the value > MAX_IMMEDIATE or < 0, then move it into register
        #if ((__IN_ADDR_OFFSET_VALUE > MAX_IMMEDIATE) || (__IN_ADDR_OFFSET_VALUE < 0))
            .reg in_addr_offset_reg
            immed32(in_addr_offset_reg, __IN_ADDR_OFFSET_VALUE)
            #define_eval __IN_OFF_REG_NAME  in_addr_offset_reg
        #else
            #define_eval __IN_OFF_REG_NAME  in_addr_offset
        #endif

        #undef __IN_ADDR_OFFSET_VALUE

    #else
        #define_eval __IN_OFF_REG_NAME  in_addr_offset
    #endif


    #if (LMADDR_IS_ZERO)
        local_csr_wr[active_lm_addr_/**/__CURR_LM_INDEX, __IN_OFF_REG_NAME]
    #elif (LM_OFFSET_IS_ZERO)
        local_csr_wr[active_lm_addr_/**/__CURR_LM_INDEX, __IN_LMADDR_REG_NAME]
    #else
        .begin

            .reg __local_lmaddr
            alu[__local_lmaddr, __IN_LMADDR_REG_NAME, +, __IN_OFF_REG_NAME]
            local_csr_wr[active_lm_addr_/**/__CURR_LM_INDEX, __local_lmaddr]

        .end
    #endif

    #ifdef LMADDR_IS_ZERO
        #undef LMADDR_IS_ZERO
    #endif

    #ifdef LM_OFFSET_IS_ZERO
        #undef  LM_OFFSET_IS_ZERO
    #endif

    #ifdef __IN_OFF_REG_NAME
        #undef __IN_OFF_REG_NAME
    #endif

    #ifdef __IN_LMADDR_REG_NAME
        #undef __IN_LMADDR_REG_NAME
    #endif

// Note after this call to macro need to wait for 3 cycle latency for local memory

.end
#endm
/// @endcond


/// @cond INTERNAL_MACROS
/** Internal macro used to set CSRs, with wait.
 *
 * @param in_lmaddr         Local memory base address
 * @param in_addr_offset    Local memory offset
 *
 * @instruction_cnt 4 to 7
 */
#macro _localmem_setcsr_wait(in_lmaddr, in_addr_offset)
    _localmem_setcsr(in_lmaddr, in_addr_offset)

    // wait for 3 cycle latency for local memory
    nop
    nop
    nop
#endm
/// @endcond


/** Set the local memory address for the specified local memory handle and makes the handle active.
 *
 * @b Example:
 * @code
 * localmem_set_address(LM_HANDLE_1, _base, off)  // use active_lm_addr_1 for subsequent local memory access
 * // three cycle latency must be provided
 * nop
 * nop
 * nop
 * localmem_read_next(out_data)       // this would use the active LM register as specified above.
 * @endcode
 *
 * @param in_lmaddr         Local memory base address
 * @param in_addr_offset    Local memory offset
 * @param LM_HANDLE         Local memory handle: @n
 *                              @arg @c LM_HANDLE_0 : Set active register to active_lm_addr_0
 *                              @arg @c LM_HANDLE_1 : Set active register to active_lm_addr_1
 *
 * @note The user will need to give 3 cycle latency before performing any localmem read or write.
 *
 * @instruction_cnt 1 to 4
 */
#macro localmem_set_address(in_lmaddr, in_addr_offset, LM_HANDLE)
    localmem_set_active_handle(LM_HANDLE)
    _localmem_setcsr(in_lmaddr, in_addr_offset)
#endm


/** Set the active register for local memory to active_lm_addr_0 or active_lm_addr_1. The default is set to active_lm_addr_0.
 *  This is done in the preprocessor stage so no instructions are used.
 *
 * @b Example:
 * @code
 * localmem_set_active_handle(LM_HANDLE_1)    // use active_lm_addr_1 for subsequent local memory access
 * localmem_read1(out_data, addr, 0)          // this call would use the active_lm_addr_1 register
 * @endcode
 *
 * @param in_reg_num    Local memory handle: @n
 *                          @arg @c LM_HANDLE_0 : Set active register to active_lm_addr_0
 *                          @arg @c LM_HANDLE_1 : Set active register to active_lm_addr_1
 *
 * @instruction_cnt 0
 */
#macro localmem_set_active_handle(in_reg_num)
    #if !(is_ct_const(in_reg_num) || isimport(in_reg_num))
        #if (IS_NFPTYPE(__NFP6000)|| IS_NFPTYPE(__NFP3800))
            #error local memory register number must be supplied as numeric 0,1,2 or 3
        #else
            #error local memory register number must be supplied as numeric 0 or 1
        #endif
    #else
        #define_eval __REG_NUM_VALUE in_reg_num
        #if (__REG_NUM_VALUE == 0)
            #define_eval    __CURR_LM_INDEX         0
        #elif (__REG_NUM_VALUE == 1)
            #define_eval    __CURR_LM_INDEX         1
        #else
            #if (IS_NFPTYPE(__NFP6000) || IS_NFPTYPE(__NFP3800))
                #if (__REG_NUM_VALUE == 2)
                    #define_eval  __CURR_LM_INDEX   2
                #elif (__REG_NUM_VALUE == 3)
                    #define_eval  __CURR_LM_INDEX   3
                #else
                    #error local memory register number must be supplied as numeric 0,1,2 or 3
                #endif
            #else
                #error local memory register number must be supplied as numeric 0 or 1
            #endif
        #endif
        #undef __REG_NUM_VALUE // cleanup name space
    #endif
#endm


/** Read a 32-bit word from the local memory.
 *
 * @b Example:
 * @code
 * localmem_read1(out, lmaddr_base, 0)
 * @endcode
 *
 * @param out_data          GPR 32-bit word
 * @param in_lmaddr         Local memory base address
 * @param in_addr_offset    Local memory offset
 *
 * @instruction_cnt 5 to 8
 */
#macro localmem_read1(out_data, in_lmaddr, in_addr_offset)
    _localmem_setcsr_wait(in_lmaddr, in_addr_offset)

    alu[out_data, --, B, *l$index/**/__CURR_LM_INDEX/**/++]   // Read 0
#endm


/** Read two 32-bit words from the local memory.
 *
 * @b Example:
 * @code
 * localmem_read2(out0, out1, lmaddr_base, 0)
 * @endcode
 *
 * @param out_data0         GPR 32-bit word
 * @param out_data1         GPR 32-bit word
 * @param in_lmaddr         Local memory base address
 * @param in_addr_offset    Local memory offset
 *
 * @instruction_cnt 6 to 9
 */
#macro localmem_read2(out_data0, out_data1, in_lmaddr, in_addr_offset)
    _localmem_setcsr_wait(in_lmaddr, in_addr_offset)

    alu[out_data0, --, B, *l$index/**/__CURR_LM_INDEX/**/++]   // Read 0
    alu[out_data1, --, B, *l$index/**/__CURR_LM_INDEX/**/++]   // Read 1
#endm


/** Read three 32-bit words from the local memory.
 *
 * @b Example:
 * @code
 * localmem_read3(out0, out1, out2, lmaddr_base, 0)
 * @endcode
 *
 * @param out_data0         GPR 32-bit word
 * @param out_data1         GPR 32-bit word
 * @param out_data2         GPR 32-bit word
 * @param in_lmaddr         Local memory base address
 * @param in_addr_offset    Local memory offset
 *
 * @instruction_cnt 7 to 10
 */
#macro localmem_read3(out_data0, out_data1, out_data2, in_lmaddr, in_addr_offset)
    _localmem_setcsr_wait(in_lmaddr, in_addr_offset)

    alu[out_data0, --, B, *l$index/**/__CURR_LM_INDEX/**/++]   // Read 0
    alu[out_data1, --, B, *l$index/**/__CURR_LM_INDEX/**/++]   // Read 1
    alu[out_data2, --, B, *l$index/**/__CURR_LM_INDEX/**/++]   // Read 2
#endm


/** Read four 32-bit words from the local memory.
 *
 * @b Example:
 * @code
 * localmem_read4(out0, out1, out2, out3, lmaddr_base, 0)
 * @endcode
 *
 * @param out_data0         GPR 32-bit word
 * @param out_data1         GPR 32-bit word
 * @param out_data2         GPR 32-bit word
 * @param out_data3         GPR 32-bit word
 * @param in_lmaddr         Local memory base address
 * @param in_addr_offset    Local memory offset
 *
 * @instruction_cnt 8 to 11
 */
#macro localmem_read4(out_data0, out_data1, out_data2, out_data3, in_lmaddr, in_addr_offset)
    _localmem_setcsr_wait(in_lmaddr, in_addr_offset)

    alu[out_data0, --, B, *l$index/**/__CURR_LM_INDEX/**/++]   // Read 0
    alu[out_data1, --, B, *l$index/**/__CURR_LM_INDEX/**/++]   // Read 1
    alu[out_data2, --, B, *l$index/**/__CURR_LM_INDEX/**/++]   // Read 2
    alu[out_data3, --, B, *l$index/**/__CURR_LM_INDEX/**/++]   // Read 3
#endm


/** Read five 32-bit words from the local memory.
 *
 * @b Example:
 * @code
 * localmem_read5(out0, out1, out2, out3, out4, lmaddr_base, 0)
 * @endcode
 *
 * @param out_data0         GPR 32-bit word
 * @param out_data1         GPR 32-bit word
 * @param out_data2         GPR 32-bit word
 * @param out_data3         GPR 32-bit word
 * @param out_data4         GPR 32-bit word
 * @param in_lmaddr         Local memory base address
 * @param in_addr_offset    Local memory offset
 *
 * @instruction_cnt 9 to 12
 */
#macro localmem_read5(out_data0, out_data1, out_data2, out_data3, out_data4, in_lmaddr, in_addr_offset)
    _localmem_setcsr_wait(in_lmaddr, in_addr_offset)

    alu[out_data0, --, B, *l$index/**/__CURR_LM_INDEX/**/++]   // Read 0
    alu[out_data1, --, B, *l$index/**/__CURR_LM_INDEX/**/++]   // Read 1
    alu[out_data2, --, B, *l$index/**/__CURR_LM_INDEX/**/++]   // Read 2
    alu[out_data3, --, B, *l$index/**/__CURR_LM_INDEX/**/++]   // Read 3
    alu[out_data4, --, B, *l$index/**/__CURR_LM_INDEX/**/++]   // Read 4
#endm


/** Read six 32-bit words from the local memory.
 *
 * @b Example:
 * @code
 * localmem_read6(out0, out1, out2, out3, out4, out5, lmaddr_base, 0)
 * @endcode
 *
 * @param out_data0         GPR 32-bit word
 * @param out_data1         GPR 32-bit word
 * @param out_data2         GPR 32-bit word
 * @param out_data3         GPR 32-bit word
 * @param out_data4         GPR 32-bit word
 * @param out_data5         GPR 32-bit word
 * @param in_lmaddr         Local memory base address
 * @param in_addr_offset    Local memory offset
 *
 * @instruction_cnt 10 to 13
 */
#macro localmem_read6(out_data0, out_data1, out_data2, out_data3, out_data4, out_data5, in_lmaddr, in_addr_offset)
    _localmem_setcsr_wait(in_lmaddr, in_addr_offset)

    alu[out_data0, --, B, *l$index/**/__CURR_LM_INDEX/**/++]   // Read 0
    alu[out_data1, --, B, *l$index/**/__CURR_LM_INDEX/**/++]   // Read 1
    alu[out_data2, --, B, *l$index/**/__CURR_LM_INDEX/**/++]   // Read 2
    alu[out_data3, --, B, *l$index/**/__CURR_LM_INDEX/**/++]   // Read 3
    alu[out_data4, --, B, *l$index/**/__CURR_LM_INDEX/**/++]   // Read 4
    alu[out_data5, --, B, *l$index/**/__CURR_LM_INDEX/**/++]   // Read 5
#endm


/** Read seven 32-bit words from the local memory.
 *
 * @b Example:
 * @code
 * localmem_read7(out0, out1, out2, out3, out4, out5, out6, lmaddr_base, 0)
 * @endcode
 *
 * @param out_data0         GPR 32-bit word
 * @param out_data1         GPR 32-bit word
 * @param out_data2         GPR 32-bit word
 * @param out_data3         GPR 32-bit word
 * @param out_data4         GPR 32-bit word
 * @param out_data5         GPR 32-bit word
 * @param out_data6         GPR 32-bit word
 * @param in_lmaddr         Local memory base address
 * @param in_addr_offset    Local memory offset
 *
 * @instruction_cnt 11 to 14
 */
#macro localmem_read7(out_data0, out_data1, out_data2, out_data3, out_data4, out_data5, out_data6, in_lmaddr, in_addr_offset)
    _localmem_setcsr_wait(in_lmaddr, in_addr_offset)

    alu[out_data0, --, B, *l$index/**/__CURR_LM_INDEX/**/++]   // Read 0
    alu[out_data1, --, B, *l$index/**/__CURR_LM_INDEX/**/++]   // Read 1
    alu[out_data2, --, B, *l$index/**/__CURR_LM_INDEX/**/++]   // Read 2
    alu[out_data3, --, B, *l$index/**/__CURR_LM_INDEX/**/++]   // Read 3
    alu[out_data4, --, B, *l$index/**/__CURR_LM_INDEX/**/++]   // Read 4
    alu[out_data5, --, B, *l$index/**/__CURR_LM_INDEX/**/++]   // Read 5
    alu[out_data6, --, B, *l$index/**/__CURR_LM_INDEX/**/++]   // Read 6
#endm


/** Read eight 32-bit words from the local memory.
 *
 * @b Example:
 * @code
 * localmem_read8(out0, out1, out2, out3, out4, out5, out6, out7, lmaddr_base, 0)
 * @endcode
 *
 * @param out_data0         GPR 32-bit word
 * @param out_data1         GPR 32-bit word
 * @param out_data2         GPR 32-bit word
 * @param out_data3         GPR 32-bit word
 * @param out_data4         GPR 32-bit word
 * @param out_data5         GPR 32-bit word
 * @param out_data6         GPR 32-bit word
 * @param out_data7         GPR 32-bit word
 * @param in_lmaddr         Local memory base address
 * @param in_addr_offset    Local memory offset
 *
 * @instruction_cnt 12 to 15
 */
#macro localmem_read8(out_data0, out_data1, out_data2, out_data3, out_data4, out_data5, out_data6, out_data7, in_lmaddr, in_addr_offset)
    _localmem_setcsr_wait(in_lmaddr, in_addr_offset)

    alu[out_data0, --, B, *l$index/**/__CURR_LM_INDEX/**/++]   // Read 0
    alu[out_data1, --, B, *l$index/**/__CURR_LM_INDEX/**/++]   // Read 1
    alu[out_data2, --, B, *l$index/**/__CURR_LM_INDEX/**/++]   // Read 2
    alu[out_data3, --, B, *l$index/**/__CURR_LM_INDEX/**/++]   // Read 3
    alu[out_data4, --, B, *l$index/**/__CURR_LM_INDEX/**/++]   // Read 4
    alu[out_data5, --, B, *l$index/**/__CURR_LM_INDEX/**/++]   // Read 5
    alu[out_data6, --, B, *l$index/**/__CURR_LM_INDEX/**/++]   // Read 6
    alu[out_data7, --, B, *l$index/**/__CURR_LM_INDEX/**/++]   // Read 7
#endm


/** Write a 32-bit word to the local memory.
 *
 * @b Example:
 * @code
 * localmem_write1(inp, lmaddr_base, 0)
 * @endcode
 *
 * @param in_data           GPR or immediate value, 32-bit word
 * @param in_lmaddr         Local memory base address
 * @param in_addr_offset    Local memory offset
 *
 * @instruction_cnt 5 to 8
 */
#macro localmem_write1(in_data, in_lmaddr, in_addr_offset)
.begin

    _localmem_setcsr_wait(in_lmaddr, in_addr_offset)

    _GET_NEW_REG_NAME(in_data)
    alu[*l$index/**/__CURR_LM_INDEX/**/++, --, B, _NEW_REG_NAME] // write 0
    _END_GET_NEW_REG_NAME

.end
#endm


/** Write two 32-bit words to the local memory.
 *
 * @b Example:
 * @code
 * localmem_write2(inp0, inp1, lmaddr_base, 0)
 * @endcode
 *
 * @param in_data0          GPR or immediate value, 32-bit word
 * @param in_data1          GPR or immediate value, 32-bit word
 * @param in_lmaddr         Local memory base address
 * @param in_addr_offset    Local memory offset
 *
 * @instruction_cnt 6 to 9
 */
#macro localmem_write2(in_data0, in_data1, in_lmaddr, in_addr_offset)
    _localmem_setcsr_wait(in_lmaddr, in_addr_offset)

    _GET_NEW_REG_NAME(in_data0)
    alu[*l$index/**/__CURR_LM_INDEX/**/++, --, B, _NEW_REG_NAME] // write 0
    _END_GET_NEW_REG_NAME


    _GET_NEW_REG_NAME(in_data1)
    alu[*l$index/**/__CURR_LM_INDEX/**/++, --, B, _NEW_REG_NAME] // write 1
    _END_GET_NEW_REG_NAME
#endm


/** Write three 32-bit words to the local memory.
 *
 * @b Example:
 * @code
 * localmem_write3(inp0, inp1, inp2, lmaddr_base, 0)
 * @endcode
 *
 * @param in_data0          GPR or immediate value, 32-bit word
 * @param in_data1          GPR or immediate value, 32-bit word
 * @param in_data2          GPR or immediate value, 32-bit word
 * @param in_lmaddr         Local memory base address
 * @param in_addr_offset    Local memory offset
 *
 * @instruction_cnt 7 to 10
 */
#macro localmem_write3(in_data0, in_data1, in_data2, in_lmaddr, in_addr_offset)
    _localmem_setcsr_wait(in_lmaddr, in_addr_offset)

    _GET_NEW_REG_NAME(in_data0)
    alu[*l$index/**/__CURR_LM_INDEX/**/++, --, B, _NEW_REG_NAME] // write 0
    _END_GET_NEW_REG_NAME

    _GET_NEW_REG_NAME(in_data1)
    alu[*l$index/**/__CURR_LM_INDEX/**/++, --, B, _NEW_REG_NAME] // write 1
    _END_GET_NEW_REG_NAME

    _GET_NEW_REG_NAME(in_data2)
    alu[*l$index/**/__CURR_LM_INDEX/**/++, --, B, _NEW_REG_NAME] // write 2
    _END_GET_NEW_REG_NAME
#endm


/** Write four 32-bit words to the local memory.
 *
 * @b Example:
 * @code
 * localmem_write4(inp0, inp1, inp2, inp3, lmaddr_base, 0)
 * @endcode
 *
 * @param in_data0          GPR or immediate value, 32-bit word
 * @param in_data1          GPR or immediate value, 32-bit word
 * @param in_data2          GPR or immediate value, 32-bit word
 * @param in_data3          GPR or immediate value, 32-bit word
 * @param in_lmaddr         Local memory base address
 * @param in_addr_offset    Local memory offset
 *
 * @instruction_cnt 8 to 11
 */
#macro localmem_write4(in_data0, in_data1, in_data2, in_data3, in_lmaddr, in_addr_offset)
    _localmem_setcsr_wait(in_lmaddr, in_addr_offset)

    _GET_NEW_REG_NAME(in_data0)
    alu[*l$index/**/__CURR_LM_INDEX/**/++, --, B, _NEW_REG_NAME] // write 0
    _END_GET_NEW_REG_NAME

    _GET_NEW_REG_NAME(in_data1)
    alu[*l$index/**/__CURR_LM_INDEX/**/++, --, B, _NEW_REG_NAME] // write 1
    _END_GET_NEW_REG_NAME

    _GET_NEW_REG_NAME(in_data2)
    alu[*l$index/**/__CURR_LM_INDEX/**/++, --, B, _NEW_REG_NAME] // write 2
    _END_GET_NEW_REG_NAME

    _GET_NEW_REG_NAME(in_data3)
    alu[*l$index/**/__CURR_LM_INDEX/**/++, --, B, _NEW_REG_NAME] // write 3
    _END_GET_NEW_REG_NAME
#endm


/** Write five 32-bit words to the local memory.
 *
 * @b Example:
 * @code
 * localmem_write5(inp0, inp1, inp2, inp3, inp4, lmaddr_base, 0)
 * @endcode
 *
 * @param in_data0          GPR or immediate value, 32-bit word
 * @param in_data1          GPR or immediate value, 32-bit word
 * @param in_data2          GPR or immediate value, 32-bit word
 * @param in_data3          GPR or immediate value, 32-bit word
 * @param in_data4          GPR or immediate value, 32-bit word
 * @param in_lmaddr         Local memory base address
 * @param in_addr_offset    Local memory offset
 *
 * @instruction_cnt 9 to 12
 */
#macro localmem_write5(in_data0, in_data1, in_data2, in_data3, in_data4, in_lmaddr, in_addr_offset)
    _localmem_setcsr_wait(in_lmaddr, in_addr_offset)

    _GET_NEW_REG_NAME(in_data0)
    alu[*l$index/**/__CURR_LM_INDEX/**/++, --, B, _NEW_REG_NAME] // write 0
    _END_GET_NEW_REG_NAME

    _GET_NEW_REG_NAME(in_data1)
    alu[*l$index/**/__CURR_LM_INDEX/**/++, --, B, _NEW_REG_NAME] // write 1
    _END_GET_NEW_REG_NAME


    _GET_NEW_REG_NAME(in_data2)
    alu[*l$index/**/__CURR_LM_INDEX/**/++, --, B, _NEW_REG_NAME] // write 2
    _END_GET_NEW_REG_NAME

    _GET_NEW_REG_NAME(in_data3)
    alu[*l$index/**/__CURR_LM_INDEX/**/++, --, B, _NEW_REG_NAME] // write 3
    _END_GET_NEW_REG_NAME

    _GET_NEW_REG_NAME(in_data4)
    alu[*l$index/**/__CURR_LM_INDEX/**/++, --, B, _NEW_REG_NAME] // write 4
    _END_GET_NEW_REG_NAME
#endm


/** Write six 32-bit words to the local memory.
 *
 * @b Example:
 * @code
 * localmem_write6(inp0, inp1, inp2, inp3, inp4, inp5, lmaddr_base, 0)
 * @endcode
 *
 * @param in_data0          GPR or immediate value, 32-bit word
 * @param in_data1          GPR or immediate value, 32-bit word
 * @param in_data2          GPR or immediate value, 32-bit word
 * @param in_data3          GPR or immediate value, 32-bit word
 * @param in_data4          GPR or immediate value, 32-bit word
 * @param in_data5          GPR or immediate value, 32-bit word
 * @param in_lmaddr         Local memory base address
 * @param in_addr_offset    Local memory offset
 *
 * @instruction_cnt 10 to 13
 */
#macro localmem_write6(in_data0, in_data1, in_data2, in_data3, in_data4, in_data5, in_lmaddr, in_addr_offset)
    _localmem_setcsr_wait(in_lmaddr, in_addr_offset)

    _GET_NEW_REG_NAME(in_data0)
    alu[*l$index/**/__CURR_LM_INDEX/**/++, --, B, _NEW_REG_NAME] // write 0
    _END_GET_NEW_REG_NAME

    _GET_NEW_REG_NAME(in_data1)
    alu[*l$index/**/__CURR_LM_INDEX/**/++, --, B, _NEW_REG_NAME] // write 1
    _END_GET_NEW_REG_NAME

    _GET_NEW_REG_NAME(in_data2)
    alu[*l$index/**/__CURR_LM_INDEX/**/++, --, B, _NEW_REG_NAME] // write 2
    _END_GET_NEW_REG_NAME

    _GET_NEW_REG_NAME(in_data3)
    alu[*l$index/**/__CURR_LM_INDEX/**/++, --, B, _NEW_REG_NAME] // write 3
    _END_GET_NEW_REG_NAME

    _GET_NEW_REG_NAME(in_data4)
    alu[*l$index/**/__CURR_LM_INDEX/**/++, --, B, _NEW_REG_NAME] // write 4
    _END_GET_NEW_REG_NAME

    _GET_NEW_REG_NAME(in_data5)
    alu[*l$index/**/__CURR_LM_INDEX/**/++, --, B, _NEW_REG_NAME] // write 5
    _END_GET_NEW_REG_NAME
#endm


/** Write seven 32-bit words to the local memory.
 *
 * @b Example:
 * @code
 * localmem_write7(inp0, inp1, inp2, inp3, inp4, inp5, inp6, lmaddr_base, 0)
 * @endcode
 *
 * @param in_data0          GPR or immediate value, 32-bit word
 * @param in_data1          GPR or immediate value, 32-bit word
 * @param in_data2          GPR or immediate value, 32-bit word
 * @param in_data3          GPR or immediate value, 32-bit word
 * @param in_data4          GPR or immediate value, 32-bit word
 * @param in_data5          GPR or immediate value, 32-bit word
 * @param in_data6          GPR or immediate value, 32-bit word
 * @param in_lmaddr         Local memory base address
 * @param in_addr_offset    Local memory offset
 *
 * @instruction_cnt 11 to 14
 */
#macro localmem_write7(in_data0, in_data1, in_data2, in_data3, in_data4, in_data5, in_data6, in_lmaddr, in_addr_offset)
    _localmem_setcsr_wait(in_lmaddr, in_addr_offset)

    _GET_NEW_REG_NAME(in_data0)
    alu[*l$index/**/__CURR_LM_INDEX/**/++, --, B, _NEW_REG_NAME] // write 0
    _END_GET_NEW_REG_NAME

    _GET_NEW_REG_NAME(in_data1)
    alu[*l$index/**/__CURR_LM_INDEX/**/++, --, B, _NEW_REG_NAME] // write 1
    _END_GET_NEW_REG_NAME

    _GET_NEW_REG_NAME(in_data2)
    alu[*l$index/**/__CURR_LM_INDEX/**/++, --, B, _NEW_REG_NAME] // write 2
    _END_GET_NEW_REG_NAME

    _GET_NEW_REG_NAME(in_data3)
    alu[*l$index/**/__CURR_LM_INDEX/**/++, --, B, _NEW_REG_NAME] // write 3
    _END_GET_NEW_REG_NAME

    _GET_NEW_REG_NAME(in_data4)
    alu[*l$index/**/__CURR_LM_INDEX/**/++, --, B, _NEW_REG_NAME] // write 4
    _END_GET_NEW_REG_NAME

    _GET_NEW_REG_NAME(in_data5)
    alu[*l$index/**/__CURR_LM_INDEX/**/++, --, B, _NEW_REG_NAME] // write 5
    _END_GET_NEW_REG_NAME

    _GET_NEW_REG_NAME(in_data6)
    alu[*l$index/**/__CURR_LM_INDEX/**/++, --, B, _NEW_REG_NAME] // write 6
    _END_GET_NEW_REG_NAME
#endm


/** Write eight 32-bit words to the local memory.
 *
 * @b Example:
 * @code
 * localmem_write8(inp0, inp1, inp2, inp3, inp4, inp5, inp6, inp7, lmaddr_base, 0)
 * @endcode
 *
 * @param in_data0          GPR or immediate value, 32-bit word
 * @param in_data1          GPR or immediate value, 32-bit word
 * @param in_data2          GPR or immediate value, 32-bit word
 * @param in_data3          GPR or immediate value, 32-bit word
 * @param in_data4          GPR or immediate value, 32-bit word
 * @param in_data5          GPR or immediate value, 32-bit word
 * @param in_data6          GPR or immediate value, 32-bit word
 * @param in_data7          GPR or immediate value, 32-bit word
 * @param in_lmaddr         Local memory base address
 * @param in_addr_offset    Local memory offset
 *
 * @instruction_cnt 12 to 15
 */
#macro localmem_write8(in_data0, in_data1, in_data2, in_data3, in_data4, in_data5, in_data6, in_data7, in_lmaddr, in_addr_offset)
    _localmem_setcsr_wait(in_lmaddr, in_addr_offset)

    _GET_NEW_REG_NAME(in_data0)
    alu[*l$index/**/__CURR_LM_INDEX/**/++, --, B, _NEW_REG_NAME] // write 0
    _END_GET_NEW_REG_NAME

    _GET_NEW_REG_NAME(in_data1)
    alu[*l$index/**/__CURR_LM_INDEX/**/++, --, B, _NEW_REG_NAME] // write 1
    _END_GET_NEW_REG_NAME

    _GET_NEW_REG_NAME(in_data2)
    alu[*l$index/**/__CURR_LM_INDEX/**/++, --, B, _NEW_REG_NAME] // write 2
    _END_GET_NEW_REG_NAME

    _GET_NEW_REG_NAME(in_data3)
    alu[*l$index/**/__CURR_LM_INDEX/**/++, --, B, _NEW_REG_NAME] // write 3
    _END_GET_NEW_REG_NAME

    _GET_NEW_REG_NAME(in_data4)
    alu[*l$index/**/__CURR_LM_INDEX/**/++, --, B, _NEW_REG_NAME] // write 4
    _END_GET_NEW_REG_NAME

    _GET_NEW_REG_NAME(in_data5)
    alu[*l$index/**/__CURR_LM_INDEX/**/++, --, B, _NEW_REG_NAME] // write 5
    _END_GET_NEW_REG_NAME

    _GET_NEW_REG_NAME(in_data6)
    alu[*l$index/**/__CURR_LM_INDEX/**/++, --, B, _NEW_REG_NAME] // write 6
    _END_GET_NEW_REG_NAME

    _GET_NEW_REG_NAME(in_data7)
    alu[*l$index/**/__CURR_LM_INDEX/**/++, --, B, _NEW_REG_NAME] // write 7
    _END_GET_NEW_REG_NAME
#endm


/** Read a next 32-bit word from the local memory.
 *
 * @b Example:
 * @code
 * localmem_read1(out1)
 * localmem_read_next(out)
 * @endcode
 *
 * @param out_data  GPR 32-bit word
 *
 * @note This macro should be called only after the call to macro localmem_write or localmem_set_address
 * @see localmem_write
 *      localmem_set_address
 *
 * @instruction_cnt 1
 */
#macro localmem_read_next(out_data)
    alu[out_data, --, B, *l$index/**/__CURR_LM_INDEX/**/++]   // Read next
#endm


/** Write a next 32-bit word from the local memory.
 *
 * @b Example:
 * @code
 * localmem_write1(inp1)
 * localmem_write_next(inp2)
 * @endcode
 *
 * @param in_data   GPR 32-bit word
 *
 * @note This macro should be called only after the call to macro localmem_write or localmem_set_address
 * @see localmem_write
 *      localmem_set_address
 *
 * @instruction_cnt 1
 */
#macro localmem_write_next(in_data)
    _GET_NEW_REG_NAME(in_data)
    alu[*l$index/**/__CURR_LM_INDEX/**/++, --, B, _NEW_REG_NAME] // write next
    _END_GET_NEW_REG_NAME
#endm


/** Zero all of local memory.
 *
 */
#macro localmem_bzero()
.begin

    .reg base
    .reg byte_offset

    #define_eval __MAX_OFFSET__ ( 16 )

    immed[base, 0]
    alu[byte_offset, --, b, base, <<2]

    #if (IS_NFPTYPE(__NFP3200) || IS_NFPTYPE(__NFP6000) || IS_NFPTYPE(__NFP3800))
    .while (base < 1023)
    #else
    .while (base < 640)
    #endif
        local_csr_wr[active_lm_addr_0, byte_offset]
        nop
        nop
        nop
        nop
        nop
        #define_eval __OFFSET__ 0
        #while(__OFFSET__ < __MAX_OFFSET__)
            immed[*l$index0[__OFFSET__], 0]
            #define_eval __OFFSET__ ( __OFFSET__ + 1 )
        #endloop
        alu[base, base, +, 16]
        alu[byte_offset, --, b, base, <<2]
    .endw

    #undef __OFFSET__
    #undef __MAX_OFFSET__

.end
#endm


/** @}
 * @}
 */

#endif /* __LOCALMEM_UC__ */
