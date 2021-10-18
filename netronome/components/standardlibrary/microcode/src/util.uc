/*
 * Copyright (C) 2008-2013 Netronome Systems, Inc.  All rights reserved.
 */

#ifndef __UTIL_UC__
#define __UTIL_UC__

#ifndef NFP_LIB_ANY_NFAS_VERSION
    #if (!defined(__NFP_TOOL_NFAS) ||                       \
        (__NFP_TOOL_NFAS < NFP_SW_VERSION(5, 0, 0, 0)) ||   \
        (__NFP_TOOL_NFAS > NFP_SW_VERSION(6, 1, 0, 255)))
        #error "This standard library is not supported for the version of the SDK currently in use."
    #endif
#endif

#include <constants.uc>

/** @file util.uc Utility Macros
 * @addtogroup util Utility
 * @{
 *
 * @name Utility Macros
 * @{
 *
 */

// API Quick Reference:
//
//  General notation: _DEF* means #define tokens
//                    _GET* will #define tokens and possibly insert instructions
//
//      _DEF_MEM_TOKENS             define _MEM_TOKENS. optional tokens for any memory operation
//      _DEF_STATUS_IS_DRAM_XFER    define _STATUS. test whether register is an dram transfer reg
//      _DEF_STATUS_IS_SRAM_XFER    define _STATUS. test whether register is an sram transfer reg
//      _GET_ADDRESSES              define _ADDR_1 and _ADDR2, load immediates if needed
//      _FREE_ADDRESSES()             if local temp reg was used by _GET_ADDRESSES, free it
//      _DEF_NAME_FROM_ARRAY_TOK    extract name from an array token string
//      _DEF_INDEX_FROM_ARRAY_TOK   extract index from an array token string
//      _DEF_LOAD_CC_TOKEN          define _LOAD_CC_TOKEN



#define_eval    DO_LOAD_CC  1
#define_eval    NO_LOAD_CC  0

/** Produce optional tokens for any memory operation.
 *
 * @note This will allow the definition of these arguments to change at a future date.
 *       For example, the Assembler at some point will support symbolic signals
 *       and a signals() expression for listing wakeup signals.
 *       At that time this will be changed to support string args.
 *
 * @param IN_REQ_SIG       Signal allocated with .sig, or signal number
 * @param IN_WAKEUP_SIGS   Signal allocated with .sig, or signal number, or signals expression
 * @param IN_Q_OPTION      Optimize_mem, unordered, priority, or ___
 * @param op_type          If 'dual_sig_op', this operation requires 2 wakeup signals
 *
 * @note UNTIL ASSEMBLER SUPPORTS signals() expression, there can only be one signal in IN_WAKEUP_SIGS.
 *
 * @b Example:
 * @code
 *  in sram_read($xfer1, addr, 0, 3, SIG_SRAM, SIG_SRAM, optimize_mem)
 *
 *  call:  _GET_MEM_TOKENS(SIG_SRAM, SIG_SRAM, optimize_mem, )
 *
 * sram[read, $xfer1, 0, addr, 3], ctx_swap, optimize_mem
 *
 * sram[read, $xfer1, 0, addr, 3], ctx_swap[1], unordered
 * @endcode
 */
#macro _DEF_MEM_TOKENS(IN_REQ_SIG, IN_WAKEUP_SIGS, IN_Q_OPTION, op_type)
    #ifdef _MEM_TOKENS
        #undef _MEM_TOKENS
    #endif
    #ifdef _q_token
        #undef _q_token
    #endif


    #define_eval _REQ_SIG IN_REQ_SIG

    #define_eval _Q_OPTION (IN_Q_OPTION)
    #if (!(is_ct_const(IN_Q_OPTION) || isimport(IN_Q_OPTION)))
        #define _q_token , _Q_OPTION
    #else
        #define _q_token
    #endif


    #if (strstr( 'IN_WAKEUP_SIGS', signals))
        #define _MEM_TOKENS sig_done[_REQ_SIG]_q_token
        // #error [1] signals IN_WAKEUP_SIGS _REQ_SIG _q_token
        #ifndef _HW_SIG_EXPR
            #define _HW_SIG_EXPR
        #endif
    #elif (is_ct_const(IN_WAKEUP_SIGS) || isimport(IN_WAKEUP_SIGS))
        #define_eval _WAKEUP_SIGS IN_WAKEUP_SIGS
        #if (_WAKEUP_SIGS == SIG_NONE)
            #define _MEM_TOKENS sig_done[_REQ_SIG]_q_token

        #else
            // force a separate context arb if this is a 2 signal op
            #if (streq(op_type, dual_sig_op))
                #define _HW_SIG_EXPR
                #define _MEM_TOKENS sig_done[_REQ_SIG]_q_token
            #else
                #define _MEM_TOKENS ctx_swap[_REQ_SIG]_q_token
            #endif
        #endif
        #undef _WAKEUP_SIGS
    #elif (streq(IN_WAKEUP_SIGS, IN_REQ_SIG))
        // force a separate context arb if this is a 2 signal op
        #if (streq(op_type, dual_sig_op))
            #define _HW_SIG_EXPR
            #define _MEM_TOKENS sig_done[_REQ_SIG]_q_token
        #else
            #define _MEM_TOKENS ctx_swap[_REQ_SIG]_q_token
        #endif
    #else
        #define _MEM_TOKENS sig_done[_REQ_SIG]_q_token
    #endif
#endm


/** Check whether a register is a dram transfer register.
 *
 */
#macro _DEF_STATUS_IS_DRAM_XFER(in_reg_name)
    #ifdef _STATUS
        #undef _STATUS
    #endif

    #if ( defined(__NFP_INDIRECT_REF_FORMAT_V1) || defined(__NFP_INDIRECT_REF_FORMAT_V2) )
        #if (strstr('in_reg_name', $))
            #define _STATUS TRUE
        #else
            #define _STATUS FALSE
        #endif
    #else
        #if (strstr('in_reg_name', $$))
            #define _STATUS TRUE
        #else
            #define _STATUS FALSE
        #endif
    #endif
#endm


/** Check whether a register is an sram transfer register.
//  Note: in near future Assembler will support type checks for xfers
//          we may need to revisit this to support xfers without $ in their name
 */
#macro _DEF_STATUS_IS_SRAM_XFER(in_reg_name)
    #ifdef _STATUS
        #undef _STATUS
    #endif

    #if ( defined(__NFP_INDIRECT_REF_FORMAT_V1) || defined(__NFP_INDIRECT_REF_FORMAT_V2) )
        #if (strstr('in_reg_name', $))
            #define _STATUS TRUE
        #else
            #define _STATUS FALSE
        #endif
    #else
        #if ( (strstr('in_reg_name', $)) && (!strstr('in_reg_name', $$)) )
            #define _STATUS TRUE
        #else
            #define _STATUS FALSE
        #endif
    #endif
#endm


/** This is a specialized macro that supports other macros which are given two memory addresses as input
 * and must use the memory addresses in a memory access instruction.
 *
 * The memory addresses could be in GPRs, they could be immediate
 * values, or they could be any combination of the two.
 * Memory access instructions have restrictions (eg. only one
 * immediate memory address can be used in each instruction).
 * This macro takes as input the two memory addresses and outputs
 * the most efficient and legal combination that can be used in a
 * memory access instruction.
 *
 * This macro #defines _MEM_ADDR0 and _MEM_ADDR1 for use in any memory operation.
 * If necessary, constants will be loaded into temporary registers
 * If a local register is used, define _GET_ADDRESSES_USE_LOCAL.
 *
 * @note The output values are ALWAYS stored in _MEM_ADDR0 and _MEM_ADDR1.
 *       The caller of this macro must always use "_MEM_ADDR0" and "_MEM_ADDR1" as first
 *       two paramters.
 *
 * @param _MEM_ADDR0     Output constant or GPR address to be used in memory access
 * @param _MEM_ADDR1     Output constant or GPR address to be used in memory access
 * @param in_mem_addr0   Constant or GPR address
 * @param in_mem_addr1   Constant or GPR offset that will be added to memory address
 * @param MEM_TYPE       SRAM, SCRATCH, DRAM, RBUF, TBUF.
 *                       Enables this macro to convert to/from longword or quadword/byte address.
 *
 * @see _FREE_ADDRESSES(), which terminates the .local used for temp address.
 *
 * @b Example:
 * @code
 * _GET_ADDRESSES (_MEM_ADDR0, _MEM_ADDR1, addr0, addr1, SRAM)
 * sram_read(data, _MEM_ADDR0, _MEM_ADDR1, 1, SIG_SRAM, SIG_SRAM, ___)
 * _FREE_ADDRESSES()
 * @endcode
 *
 */ // dummy defines at initialization
#macro _GET_ADDRESSES(OUT_MEM_ADDR0, OUT_MEM_ADDR1, in_mem_addr0, in_mem_addr1, MEM_TYPE)
_GET_ADDRESSES(OUT_MEM_ADDR0, OUT_MEM_ADDR1, in_mem_addr0, in_mem_addr1, MEM_TYPE, --)
#endm

/** This is a specialized macro that supports other macros which are given two memory addresses as input
 * and must use the memory addresses in a memory access instruction.
 *
 * The memory addresses could be in GPRs, they could be immediate
 * values, or they could be any combination of the two.
 * Memory access instructions have restrictions (eg. only one
 * immediate memory address can be used in each instruction).
 * This macro takes as input the two memory addresses and outputs
 * the most efficient and legal combination that can be used in a
 * memory access instruction.
 *
 * This macro #defines _MEM_ADDR0 and _MEM_ADDR1 for use in any memory operation.
 * If necessary, constants will be loaded into temporary registers
 * If a local register is used, define _GET_ADDRESSES_USE_LOCAL.
 *
 * @note The output values are ALWAYS stored in _MEM_ADDR0 and _MEM_ADDR1.
 *       The caller of this macro must always use "_MEM_ADDR0" and "_MEM_ADDR1" as first
 *       two paramters.
 *
 * @param _MEM_ADDR0     Output constant or GPR address to be used in memory access
 * @param _MEM_ADDR1     Output constant or GPR address to be used in memory access
 * @param in_mem_addr0   Constant or GPR address
 * @param in_mem_addr1   Constant or GPR offset that will be added to memory address
 * @param MEM_TYPE       SRAM, SCRATCH, DRAM, RBUF, TBUF.
 *                       Enables this macro to convert to/from longword or quadword/byte address.
 * @param in_addr_shift  'A<<8' or 'B<<8' for 40-bit addressing mode, else '--'.
 *
 * @see _FREE_ADDRESSES(), which terminates the .local used for temp address.
 *
 * @b Example:
 * @code
 * _GET_ADDRESSES (_MEM_ADDR0, _MEM_ADDR1, addr0, addr1, SRAM)
 * sram_read(data, _MEM_ADDR0, _MEM_ADDR1, 1, SIG_SRAM, SIG_SRAM, ___)
 * _FREE_ADDRESSES()
 * @endcode
 *
 */ // dummy defines at initialization

#macro _GET_ADDRESSES(OUT_MEM_ADDR0, OUT_MEM_ADDR1, in_mem_addr0, in_mem_addr1, MEM_TYPE, in_addr_shift)
    //TBD deal with MEM_TYPE
    #if (!streq(OUT_MEM_ADDR0, _MEM_ADDR0))
        #error "_GET_ADDRESSES -> Call parameter 0 must be '_MEM_ADDR0'"
    #endif
    #if (!streq(OUT_MEM_ADDR1, _MEM_ADDR1))
        #error "_GET_ADDRESSES -> Call parameter 1 must be '_MEM_ADDR1'"
    #endif

    // check DRAM address
    #if ( (is_ct_const(in_mem_addr0) || isimport(in_mem_addr0)) && (is_ct_const(in_mem_addr1) || isimport(in_mem_addr1)) && \
		 !(streq('in_addr_shift', 'A<<8') || streq('in_addr_shift', 'B<<8')) )
        // both immediates and using 32-bit addressing
        #define_eval _ADDR_SUM ((in_mem_addr0) + (in_mem_addr1))

        // need to have at least one GPR
        #define _GET_ADDRESSES_USE_LOCAL0
        .local tmp_mem_addr0
        immed32(tmp_mem_addr0, _ADDR_SUM)
        #undef _ADDR_SUM

        #define_eval _MEM_ADDR0 tmp_mem_addr0
        #define_eval _MEM_ADDR1 0

    #else   // both are not immediates. check each one seperately for next neighbor or immed > (MAX_IMMEDIATE_ADDR)
        #if (is_ct_const(in_mem_addr0) || isimport(in_mem_addr0))

            #define_eval _TMP_MEM_ADDR0 in_mem_addr0

            #if (_TMP_MEM_ADDR0 > (MAX_IMMEDIATE_ADDR))
                #define _GET_ADDRESSES_USE_LOCAL0
                .local tmp_mem_addr0
                move(tmp_mem_addr0, _TMP_MEM_ADDR0)
                #define_eval _MEM_ADDR0 tmp_mem_addr0

            #else // in_mem_addr0 < (MAX_IMMEDIATE_ADDR)
                #define_eval _MEM_ADDR0 _TMP_MEM_ADDR0
            #endif // in_mem_addr0 < MAX_IMMEDIATE_ADDR
        #elif (strstr('in_mem_addr0', 'n$')) // if next neighbor register
            #define _GET_ADDRESSES_USE_LOCAL0
            .local tmp_mem_addr0
            alu[tmp_mem_addr0, --, b, in_mem_addr0]

            #define_eval _MEM_ADDR0 tmp_mem_addr0
        #else // in_mem_addr0 is a gpr

            #define_eval _MEM_ADDR0 in_mem_addr0

        #endif
        #if (is_ct_const(in_mem_addr1) || isimport(in_mem_addr1))
            #define_eval _TMP_MEM_ADDR1 in_mem_addr1

            #if (_TMP_MEM_ADDR1 > MAX_IMMEDIATE_ADDR)
                #define _GET_ADDRESSES_USE_LOCAL1
                .local tmp_mem_addr1

                move(tmp_mem_addr1, _TMP_MEM_ADDR1)

                #define_eval _MEM_ADDR1 tmp_mem_addr1

            #else // in_mem_addr1 < MAX_IMMEDIATE_ADDR
                    #define_eval _MEM_ADDR1 _TMP_MEM_ADDR1
            #endif // in_mem_addr1 < MAX_IMMEDIATE_ADDR
        #elif (strstr('in_mem_addr1', 'n$')) // if next neighbor register

                #define _GET_ADDRESSES_USE_LOCAL1
                .local tmp_mem_addr1

                    move(tmp_mem_addr1, in_mem_addr1)

            #define_eval _MEM_ADDR1 tmp_mem_addr1

        #else // in_mem_addr1 is a gpr
                #define_eval _MEM_ADDR1 in_mem_addr1
        #endif
    #endif  // both mem addresses are immediates
#endm


#macro _FREE_ADDRESSES()
    #ifdef _GET_ADDRESSES_USE_LOCAL0

        .endlocal
        #undef _GET_ADDRESSES_USE_LOCAL0
    #endif
    #ifdef _GET_ADDRESSES_USE_LOCAL1

        .endlocal
        #undef _GET_ADDRESSES_USE_LOCAL1
    #endif
    #ifdef _MEM_ADDR0
        #undef _MEM_ADDR0
    #endif
    #ifdef _MEM_ADDR1
        #undef _MEM_ADDR1
    #endif

    #ifdef _TMP_MEM_ADDR0
        #undef _TMP_MEM_ADDR0
    #endif

    #ifdef _TMP_MEM_ADDR1
        #undef _TMP_MEM_ADDR1
    #endif
#endm

/** Extract name from an array token string that consists of name[index] or nameindex.
 */
#macro _DEF_NAME_FROM_ARRAY_TOK(_NAME, array_tok)
    #define_eval _left_bracket_index (strstr('array_tok', '['))
    #if (_left_bracket_index > 0)
        #define_eval _NAME (strleft(array_tok, (_left_bracket_index -1)))
    #else
        #define_eval _array_tok_len (strlen(array_tok))
        #define_eval _right_2digits (strright (array_tok, 2))
        #define_eval _num_right_2digits (_right_2digits)
        #if (is_ct_const (_num_right_2digits) || isimport(_num_right_2digits))
            #define_eval _NAME (strleft(array_tok, (_array_tok_len - 2)))
        #else
            #define_eval _right_digit (strright (array_tok, 1))
            #define_eval _num_right_digit (_right_digit)
            #if (is_ct_const (_num_right_digit) || isimport(_num_right_digit))
                #define_eval _NAME (strleft(array_tok, (_array_tok_len - 1)))
            #else
                #define_eval _NAME array_tok
            #endif
        #endif
    #endif
#endm

/** Extract index from an array token string that consists of name[index] or nameindex.
 */
#macro _DEF_INDEX_FROM_ARRAY_TOK(_INDEX, array_tok)
    #define_eval _left_bracket_index (strstr('array_tok', '['))
    #if (_left_bracket_index > 0)
        #define_eval _array_tok_len (strlen(array_tok))
        #define_eval _left_array_tok (strleft (array_tok, (_array_tok_len - 1)))
        #define_eval _INDEX (strright('_left_array_tok', ((_array_tok_len - _left_bracket_index) - 1)))
    #else
        #define_eval _array_tok_len (strlen(array_tok))
        #define_eval _right_digit (strright (array_tok, 1))
        #define_eval _INDEX (_right_digit)
        #define_eval _array_tok_len (strlen(array_tok))
        #define_eval _left_array_tok (strleft (array_tok, (_array_tok_len - 1)))
        #define_eval _right_2nd_digit (strright (_left_array_tok, 1))
        #define_eval _num_2nd_right (_right_2nd_digit)
        #if (is_ct_const (_num_2nd_right) || isimport(_num_2nd_right))
            #define_eval _right_digits (strright (array_tok, 2))
            #define_eval _INDEX (_right_digits)
            #if !(is_ct_const (_INDEX) || isimport(_INDEX))
                #define_eval _INDEX -1
            #endif
        #endif
    #endif
#endm

/** This macro checks if in_name can fit into immediate parameter,
 * if required allocates a register and puts the value of in_name
 * into _NEW_REG_NAME.
 * It uses _END_GET_NEW_REG_NAME after the register is used.
 */
#macro _GET_NEW_REG_NAME(in_name)
.begin

    #if (is_ct_const(in_name) || isimport(in_name))
        // if the value > MAX_IMMEDIATE then move it into register
        #if ( (in_name > MAX_IMMEDIATE) || (in_name < 0 ))
            .reg __out_reg
            immed32(__out_reg, in_name)
            #define_eval _NEW_REG_NAME __out_reg
        #else
            #define_eval _NEW_REG_NAME  in_name
        #endif
    #else
        #define_eval _NEW_REG_NAME  in_name
    #endif

/* Note that this macro must not contain ".end" to match the ".begin"
 * at the start of the macro because "_END_GET_NEW_REG_NAME" will supply
 * the ".end". */
#endm

#define _END_GET_NEW_REG_NAME   .end


/** This macro produces load_cc token for any operation.
 *
 * @param in_load_cc   CONSTANT to specify whether user wants load ALU condition codes
 *                     based on the result performed. Possible values: @n
 *                     @arg NO_LOAD_CC: Define _LOAD_CC_TOKEN as empty string
 *                     @arg DO_LOAD_CC: Define _LOAD_CC_TOKEN as load_cc.
 *
 * @b Example:
 * @code
 * swap(res, aop, in_load_cc)
 *
 * call:  _DEF_LOAD_CC_TOKEN(in_load_cc)
 * create: ld_field[res, 1010, aop, <<rot24], load_cc
 * @endcode
 */
#macro _DEF_LOAD_CC_TOKEN(in_load_cc)
    #ifdef _LOAD_CC_TOKEN
        #undef _LOAD_CC_TOKEN
    #endif

    #if (in_load_cc == DO_LOAD_CC)
        #define _LOAD_CC_TOKEN  , load_cc
    #else
        #define _LOAD_CC_TOKEN
    #endif
#endm


/** @}
 * @}
 */

#endif /* __UTIL_UC__ */
