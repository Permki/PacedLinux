/*
 * Copyright (C) 2014 Netronome Systems, Inc.  All rights reserved.
 */

#ifndef _OV_UC_
#define _OV_UC_

#include <stdmac.uc>

/** @file ov.uc Override Macros
  *
  * @brief All macros that form part of the Override Macros are contained within this file.
  *
  * These macros facilitate readable and efficient creation of indirect references for either
  * the Yeldham NFP3200 extended indirect reference format or the Thornham NFP6000 indirect
  * reference format.
  *
  * See @ref OVModuleDocumentation "Detailed Description" for details on how to use these macros.
  *
  * @addtogroup ov Override Macros
  * @{
  *
  * @anchor OVModuleDocumentation
  * @brief A collection of macros to facilitate readable and efficient creation of indirect
  * references for either the Yeldham NFP3200 extended indirect reference format or the
  * Thornham NFP6000 indirect reference format.
  *
  * The goals of these macros are specifically to:
  * - Make microcode more readable / understandable where indirect references are constructed
  *   and used, by abstracting the indirect reference as a number of fields for which values
  *   are provided.
  * - Enable the same code to be compiled for 3200 or 6000 indirect references
  * - Generate efficient code
  * - Help prevent silly mistakes (specifying overlapping data, shifting data to the wrong
  *   location, not providing everything that is required, using invalid combinations of overridden parameters)
  *
  * The same set of macro calls can be used in the following environments (as long as the
  * specific parameters to be overridden are valid in the specific indirect reference format):
  * - Thornham with NFP6000 indirect references
  * - Thornham with NFP3200 indirect references
  * - Yeldham with NFP32XX indirect references
  *
  * @note
  * It is assumed that only the indirect macros update the contents of CSR CMD_INDIRECT_REF_0.
  * If this is not the case, specify flag OVF_NO_CTX_SWAP_OFF and do NOT specify flag OVF_NO_SWAP_SINCE_USE.
  * Doing this will cause the macros to write the value of CMD_INDIRECT_REF_0 every time ov_use() is called,
  * so that CMD_INDIRECT_REF_0 will have the intended value for the following instruction which uses the
  * indirect reference.
  * \n\n
  * @note
  * These macros are rather heavy in terms of preprocessor computing in order to:
  * - Produce optimal microcode output, and
  * - Provide comprehensive error checking on the usage of the macros.
  *
  * @note
  * The intention is that an assembler programmer will be using this interface, so the
  * programmer is ultimately responsible for knowing what can be done and providing legal
  * values for overriding parameters.
  *
  * @par Checks
  *
  * @par
  * The macros perform the following checks to ensure correct and logical usage:
  * - The set of fields specified in ov_start() is valid for the indirect reference format being compiled for.
  * - Values are provided for all fields before ov_use() is permitted.
  * - Values are provided for all bits of all fields before ov_use() is permitted (unless flag
  *   OVF_PARTIAL_BITS_ON is specified to only warn of such situations).
  * - The values for the fields are provided in the optimal order (unless flag OVF_RELAXED_ORDER_ON
  *   is specified to suppress such warnings).
  *
  * @par
  * Depending on the nature of the check that fails, either a warning (to help optimise) or an error (to fix
  * a situation that is obviously wrong) will be issued.
  *
  * @par Optimal order
  *
  * @par
  * In order to generate efficient code, there is an optimal order in which parameters need to be provided
  * to the macros. The macros will provide warnings should this not be done; in the case where
  * the structure of the code does not permit optimal order, it is possible to suppress these warnings, at
  * the expense of additional opcodes.
  *
  * @par
  * The optimal order is to provide values in the following order:
  * -# All constant values
  * -# If the byte mask is being used in NFP6000 indirect references and another field is also destined for
  *   CMD_INDIRECT_REF_0, both should be provided using set_bm_and(), before any other fields in
  *   CMD_INDIRECT_REF_0
  * -# Register values
  * -# Last register value must be destined to previous ALU output
  *
  * @par Structure
  *
  * @par
  * The general idea is to call macros as follows:
  * - Optional single call to ov_global() at the start of the top level file, to set the global flags.
  * - All of the following, one or more times, each for a specific indirect reference:
  *   - Single call to ov_start(), indicating what it is that you want to override.
  *   - One or more calls to ov_set(), indicating what values to use for each of the parameters.
  *   - One or more calls to ov_use(), indicating that the following instruction is going to use the
  *     indirect override.
  *   - Single call to ov_clear(), indicating that indirect reference is no longer required.
  * - Recommended single call to ov_sanity() at the end of the top level file, to make sure that there
  *   are no loose ends.
  *
  * @note
  * ov_global() may be called multiple times, as convenient. The most recently-set flags are used when
  * ov_start() is called, and are then used by that indirect reference until the end of its life.
  *
  * @par Example usage
  *
  * @par
  * To override length:
  *
  * @par
  * @code
  * ov_start(OV_LENGTH)
  * ov_set_use(OV_LENGTH, 12, OVF_SUBTRACT_ONE)
  * command[...], indirect_ref
  * ov_clean()
  * @endcode
  *
  * @par
  * To override length, byte mask and data reference, and use the indirect reference twice:
  *
  * @par
  * @code
  * ov_start((OV_LENGTH | OV_BYTE_MASK | OV_DATA_REF), OVF_REUSE_ON)
  * ov_set(OV_BYTE_MASK, byte_mask)
  * ov_set(OV_DATA_REF, data)
  * ov_set_use(OV_LENGTH, len, OVF_SUBTRACT_ONE)
  * command[...], indirect_ref
  * // Unrelated instructions
  * ov_use() ; Use same again without recalculating
  * command[...], indirect_ref
  * ov_clean()
  * @endcode
  *
  * @par
  * Example for PacketComplete Target command (only applicable to NFP6000 indirect
  * references):
  *
  * @par
  * @code
  * ov_start((OV_LENGTH | OV_BYTE_MASK | OV_DATA_REF | OV_SEQ_NUM))
  * ov_set(OV_DATA_REF, 10, 10, 1)
  * ov_set_bits(OV_LENGTH, 4, 3, 0) // Indicates upper bits intentionally zero (no instruction cost)
  * ov_set(OV_LENGTH, 2, 0, len)
  * ov_set_bits(OV_BYTE_MASK, 7, 5, 0) // Indicates upper bits intentionally zero (no instruction cost)
  * ov_set(OV_BYTE_MASK, 4, 0, byte_mask)
  * ov_set_bits(OV_SEQ_NUM, 13, 12, 0) // Indicates upper bits intentionally zero (no instruction cost)
  * ov_set(OV_SEQ_NUM, 11, 0, data)
  * ov_set_bits(OV_DATA_REF, 11, 11, 0) // Indicates bit intentionally zero (no instruction cost)
  * ov_set(OV_DATA_REF, 13, 12, nbi)
  * ov_set_use(OV_DATA_REF, 9, 0, txqueue)
  * command[...], indirect_ref
  * ov_clean()
  * @endcode
  *
  * @par
  * or
  *
  * @par
  * @code
  * ov_start((OV_LENGTH | OV_BYTE_MASK | OV_DATA_REF | OV_SEQ_NUM))
  * ov_set(OV_DATA_REF, 10, 10, 1)
  * ov_set(OV_LENGTH, len)
  * ov_set(OV_BYTE_MASK, byte_mask)
  * ov_set(OV_SEQ_NUM, data)
  * ov_set_bits(OV_DATA_REF, 11, 11, 0) // Indicates bit intentionally zero (no instruction cost)
  * ov_set(OV_DATA_REF, 13, 12, nbi)
  * ov_set_use(OV_DATA_REF, 9, 0, txqueue)
  * command[...], indirect_ref
  * ov_clean()
  * @endcode
  *
  * @anchor ResumingAndRecalling
  * @par Resuming and recalling
  *
  * @par
  * Only one indirect reference can be worked with at a time. However, it may be necessary to interleave
  * indirect reference calculations, or to partially calculate an indirect reference and then resume from
  * that point multiple times. Support is provided for this by means of the ov_store() / ov_resume() / ov_recall()
  * mechanism. Currently there is a limitation of a maximum of 8 stored indirect references at any point
  * in time.
  *
  * @note
  * Storing an indirect reference may consume zero, one or two registers, depending on whether values have
  * already been set from registers at the point where ov_store() is called. These registers become free again
  * when ov_resume() or ov_destroy() are called.
  *
  * @par
  * <b>Resuming:</b> An indirect reference may be stored and resumed, which is to say that the indirect reference
  * is parked while something else is done, and then continued later from exactly the point where it was
  * stored. ov_resume() may be called exactly one; it destroys the stored data. For example:
  *
  * @par
  * @code
  * // First indirect reference starts
  * ov_start((OV_BYTE_MASK | OV_LENGTH))
  * ov_set(OV_BYTE_MASK, byte_mask)
  * ov_store()
  * #define_eval REF_BYTE_MASK_AND_LENGTH OV_SLOT // Store slot number for later
  *
  * // Second indirect reference starts
  * ov_start((OV_DATA_REF | OV_BYTE_MASK))
  * ov_set(OV_BYTE_MASK, byte_mask)
  * ov_store()
  * #define_eval REF_DATA_REF_AND_BYTE_MASK OV_SLOT // Store slot number for later
  *
  * // First indirect reference resumed
  * ov_resume(REF_BYTE_MASK_AND_LENGTH)
  * ov_set_use(OV_LENGTH, len, OVF_SUBTRACT_ONE)
  * command[...], indirect_ref
  * ov_clean()
  *
  * // Second indirect reference resumed
  * ov_resume(REF_DATA_REF_AND_BYTE_MASK)
  * ov_set_use(OV_DATA_REF, data)
  * command[...], indirect_ref
  * ov_clean()
  * @endcode
  *
  * @par
  * <b>Recalling:</b> An indirect reference may be stored and recalled, which is to say that the indirect
  * reference is parked, and then later continued from exactly the point where it was stored <b>and</b>
  * this may be done multiple times. When the indirect reference no longer needs to be recalled, ov_destroy()
  * must be called to release the resources it was using. For example:
  *
  * @par
  * @code
  * ov_start((OV_LENGTH | OV_SEQ_NUM))
  * ov_set(OV_LENGTH, 16, OVF_SUBTRACT_ONE)
  * ov_store()
  * #define_eval REF_LENGTH_SEQ_NUM OV_SLOT
  *
  * #define COUNT 0
  * #while (COUNT < 4)
  *
  *     ov_recall(REF_LENGTH_SEQ_NUM)
  *     ov_set_use(OV_SEQ_NUM, COUNT)
  *     command[...], indirect_ref
  *     ov_clean()
  *
  * #define_eval COUNT (COUNT + 1)
  * #endloop
  *
  * ov_dispose(REF_LENGTH_SEQ_NUM)
  * @endcode
  *
  * @par Generic structure
  *
  * @par
  * The generic structure of working with a single indirect reference is as follows. This structure
  * can be repeated as many times as necessary.
  * - Start working with an indirect reference using ov_start(), ov_recall() or ov_resume().
  * - Optionally set one or more values in indirect reference using ov_set*().
  * - Optionally use the indirect reference now (if all fields have been set or the last
  *   is now set):
  *   - ov_use() or ov_set_use(), which sets CMD_INDIRECT_REF_0 and previous ALU output appropriately
  *   - next instruction: command[...], indirect_ref
  * - Finally do one of the following:
  *   - Dispose of the indirect reference via ov_clean() (only allowed if just used), or
  *   - Store for later via ov_store() (allowed whether just used or not).
  *
  * @par
  * At one level higher, the following must be done:
  * - Every ov_store() must eventually be followed by a corresponding ov_resume() or ov_recall().
  * - Every ov_recall() must eventually be followed by a corresponding ov_resume() or ov_destroy().
  * - When ov_sanity() is called, the following must all be true, otherwise an error is issued:
  *   - The most recent indirect reference must have been completed with ov_clean().
  *   - Every indirect reference that was stored using ov_store() must have been cleared via
  *     either ov_resume() or ov_destroy().
  *
  * @note
  * ov_clean() <b>never</b> generates opcodes. It is thus permissable to put ov_clean() after ov_use()
  * and <i>before</i> the indirect reference is actually used, for example:
  *
  * @note
  * @code
  * ov_start(OV_LENGTH)
  * ov_set_use(OV_LENGTH, 12, OVF_SUBTRACT_ONE)
  * ov_clean()
  * command[...], indirect_ref
  * @endcode
  *
  * @note
  * or even
  *
  * @note
  * @code
  * #macro override_length(in_len)
  *     ov_start(OV_LENGTH)
  *     ov_set_use(OV_LENGTH, in_len, OVF_SUBTRACT_ONE)
  *     ov_clean()
  * #endm
  *
  * override_length(12)
  * command[...], indirect_ref
  * @endcode
  *
  * @note
  * which could also be written using the shorthand for a single field
  *
  * @note
  * @code
  * ov_single(OV_LENGTH, 12, OVF_SUBTRACT_ONE)
  * command[...], indirect_ref
  * @endcode
  *
  * @par Flags
  *
  * @par
  * Flags are used to fine-tune functionality within the override macros. They exist at
  * three different scopes:
  * -# Global: Defined at any point, but are only used when a new indirect reference is started,
  *    at which point the global flags are used as a starting point.
  * -# Current indirect reference: Flags provided to ov_start(), which are applicable to the
  *    current indirect reference for its lifetime, and override global flags as specified
  *    at that point in time. Flags typically used here include OVF_PARTIAL_BITS_ON and
  *    OVF_NO_CTX_SWAP_ON.
  * -# Current macro: Flags provided to the specific macro that override the flags of the
  *    indirect reference, but just for the single macro calls. Flags typically used here
  *    include OVF_SUBTRACT_ONE, OVF_PARANOID_ON and OVF_RELAXED_ORDER_ON.
  *
  * @note
  * All flags not explicitly specified at global level are assumed to have the default of _OFF.
  *
  * @par Resources consumed
  *
  * @par
  * The indirect reference macros consume the following resources:
  * - A large number of preprocessor defines and macros, all prefixed with "ov_", "OV_", "OVF_",
  *   "_OV" or "_ov".
  * - One register for each destination register (previous ALU output or CMD_INDIRECT_REF_0),
  *   as soon as a register value is provided. This is a maximum of two registers for the
  *   active indirect reference for NFP6000 indirect references.
  * - Additional zero, one or two registers each time ov_store() is called, where any
  *   registers that were being used by the stored indirect reference become dormant
  *   until the specific slot is referred to again.
  *
  * @name Override Macros
  * @{
  *
  */

/*

Implementation notes:
====================
- Only one instance can be used at a time (one set of overridden parameters)
- Indirect CSR is global for microengine and cannot be updated, so have to write to working
  register, and then copy data over just before it is required (by ov_set_use() or ov_use()).
- There is NOT a restriction to wait a number of cycles after setting Indirect CSR before issuing
  an I/O command.
- Where a register (non-constant value) is provided, it is NOT checked that it does not contain
  more than the applicable number of bits. Perhaps this should be added as an option somehow?
  Problem is what do you do when such an error is picked up? Mask it off? Set it to zero?
- Global #defines (for storing and recalling indirect reference instances):
  - _OV_STORE_USED, where each bit that is set indicates that the indicated slot is used, and
    thus the corresponding entry of _OV_STORE_STATE_x contains meaningful information.
  - _OV_STORE_STATE_x, where x = 0..(_OV_STORE_STATES_COUNT - 1), contains a concatenated version
    of all the following information related to the state of a specific indirect reference.
  - _OV_STORE_REGISTER_SET is used to uniquely identify pairs of _ov_reg_alu_x / _ov_reg_ind_csr_x
    registers, where "x" is an integer, for store / recall / resume. _OV_STORE_REGISTER_SET is
    incremented in every call to ov_start() and ov_recall() to facilitate debugging of list file
    where register names are different for different indirect references.
- Global #defines (across macro calls for tracking state of indirect reference),
  aka Indirect reference state:
  - _OV_PAR_REQUESTED: Each set bit represents a parameter that has been requested.
  - _OV_REG_ALU_SRC / _OV_REG_IND_SRC: Number of register set used for sourcing data for previous
    ALU result / Indirect CSR.
  - _OV_REG_ALU_DST / _OV_REG_IND_DST: Number of register set used for writing data to for
    previous ALU result / Indirect CSR (NOTE: not stored during ov_store() because new registers
    are assigned for output (ov_recall()) or old registers are reused (ov_resume()).
  - _OV_ALU_CONST: Bits that are ORed with previous ALU result to get correct previous ALU
    result output.
  - _OV_IND_CSR_CONST: Bits that are ORed with Indirect CSR to get correct Indirect CSR output.
  - _OV_ALU_MASK_REQUIRED: Each set bit represents data bits (not configuration bits) that
    are required for previous ALU output before the provided data can be considered complete.
  - _OV_ALU_MASK_PROVIDED: Each set bit represents data bits that have been provided so far
    for previous ALU output.
  - _OV_IND_CSR_MASK_REQUIRED: Each set bit represents data bits that are required for the
    Indirect CSR before the provided data can be considered complete.
  - _OV_IND_CSR_MASK_PROVIDED: Each set bit represents data bits that have been provided so
    far for the Indirect CSR.
  - _OVF_CURRENT: Current set of flags, derived from _OVF_GLOBAL and locally-specified flags in
    ov_start().
  - _OVF_STATUS: Status information related to flags.
  - _OV_PAR_COUNT: Contains the number of parameters provided, which also indicates the
    number of entries in _OV_ROW_PAR.
  - _OV_ROW_PAR: Contains the _OV_PAR_COUNT indices of the rows in the _OV_REF_DATA
    table that are applicable to the current configuration encoded as a "string" where each
    offset is represented as four decimal characters.
- Indication of time to execute for Thornham on a 2.9 GHz i7 (vast majority consumed by ov_start(),
  which does not produce any mnemonics but only determines whether the provided combination is
  valid, what the configuration bits must be set to, and where the data must be placed):
  - 1 parameter:   23 ms (OV_LENGTH)
  - 2 parameters:  28 ms (OV_LENGTH | OV_BYTE_MASK)
  - 3 parameters:  38 ms (OV_LENGTH | OV_BYTE_MASK | OV_DATA_REF)
  - 4 parameters:  90 ms (OV_LENGTH | OV_BYTE_MASK | OV_DATA_REF | OV_MASTER_ISLAND)
  - 5 parameters: 170 ms (OV_LENGTH | OV_BYTE_MASK | OV_DATA_REF | OV_MASTER_ISLAND | OV_DATA_MASTER)
  - 6 parameters: 140 ms (OV_LENGTH | OV_BYTE_MASK | OV_DATA_REF | OV_MASTER_ISLAND | OV_DATA_MASTER | OV_SIGNAL_MASTER)
  - 7 parameters: 180 ms (OV_LENGTH | OV_BYTE_MASK | OV_DATA_REF | OV_MASTER_ISLAND | OV_DATA_MASTER | OV_SIGNAL_MASTER | OV_SIGNAL_CTX)
  - 8 parameters: 210 ms (OV_LENGTH | OV_BYTE_MASK | OV_DATA_REF | OV_MASTER_ISLAND | OV_DATA_MASTER | OV_SIGNAL_MASTER | OV_SIGNAL_CTX | OV_SIGNAL_NUM)
  - 9 parameters: 280 ms (OV_LENGTH | OV_BYTE_MASK | OV_DATA_REF | OV_MASTER_ISLAND | OV_DATA_MASTER | OV_SIGNAL_MASTER | OV_SIGNAL_CTX | OV_SIGNAL_NUM | OV_DATA_MASTER_2bits)
- Code tested with warning level 4, which includes warning 5008 "Register X is set but not used."
  If indirect references follow ov_*use(), then this warning will NOT be generated, as an indirect
  reference counts as a use for an assigned register.
  Thus it is NOT necessary to wrap assignments with the following:
    #pragma warning(push)
    #pragma warning(disable:5008)
    // Warning NNNN disabled here
    #pragma warning(pop)
- Warning 4101 is disabled around .reg global definitions of _ov_reg_alu_/../_OV_REG_ALU_DST
  and _ov_reg_ind_csr_/../_OV_REG_IND_DST, as this simplifies the flow of the code.
- Each line that can produce assembly instructions (at the highest level) contains [asm] to make
  finding such lines easier.
- If _OV_PREVIOUS_ALU_TO_REGISTER is defined before including ov.uc, every time ov_*use() is called,
  the value of previous alu result will be written to global register _ov_previous_alu_value. This
  is to facilitate automated testing. Depending on the context, this may result in zero to three
  additional instructions, and so should be enabled for testing and off otherwise.

Future enhancements:
- Perhaps more precisely determine whether the provided value is valid, such as length value
  for ref_cnt, where allowed range depends on I/O instruction for which indirect reference
  is to be used.

*/

// Preprocessor defines across all macro and across all indirect references

/// @cond INTERNAL_DEFINES
#define _OV_MS_BIT_NUM  31 // Largest bit number in a preprocessor define. Was 63, but changed to 31 so that will work correctly in 32-bit preprocessor environment
#define _OV_32_BIT_MASK 0xFFFFFFFF // = ((1 << (_OV_MS_BIT_NUM + 1)) - 1), but not evaluated correctly in 32-bit environment
/// @endcond

#if (!defined(__PREPROC64))

    #error "ov.uc requires 64-bit preprocessor mode."

    // Note: This is currently a requirement because (1 << 32) is not computed correctly
    // in a 32-bit environment, and this is needed to calculated masks in formulas such as
    // #define _OV_TEMP_DATA_MASK (((1 << (_OV_ROW_DATA_MSbit + 1)) - 1) - ((1 << _OV_ROW_DATA_LSbit) - 1))

#endif

/// @cond INTERNAL_DEFINES

    #define _OV_STORE_STATES_COUNT 8 // Number of indirect reference states that may be simultaneously stored internally

    #if (_OV_STORE_STATES_COUNT > _OV_MS_BIT_NUM)

        #error "_OV_STORE_STATES_COUNT may not exceed _OV_MS_BIT_NUM!"

    #endif

    #ifdef _OV_STORE_REGISTER_SET

        #error "_OV_STORE_REGISTER_SET has already been defined!"

    #endif

    #define _OV_STORE_REGISTER_SET 0 // Next number to be postfixed to "_ov_reg_alu_" and "_ov_reg_ind_csr_" to form register names

    #define _OV_STORE_USED 0 // Used to track which internal slots are used

/// @endcond


/** @anchor OverrideFields
  * @name Override names
  * Names of fields that can be overridden.
  *
  * Allowed fields and allowed combinations of fields depend on architecture and indirect
  * format, i.e. either Yeldham NFP3200 extended indirect reference format or Thornham
  * NFP6000 indirect reference format. The provided data is checked for validity during
  * assembly.
  *
  * These names are used for the OVERRIDE_TYPES parameter of ov_start() and for the
  * OVERRIDE_TYPE parameter of ov_set*().
  */

/** Specifies 5-bit length field.
  * For NFP6000 indirect reference format, located at ALU_OUT[12:8].
  *
  * @note
  * If representing a length, the value typically must have one subtracted, which can be done
  * <i>before</i> providing the value to ov_set*(), or by specifying the OVF_SUBTRACT_ONE flag.
  */
#define OV_LENGTH             (1 <<  0)
/** Specifies 8-bit byte mask field.
  * For NFP6000 indirect reference format, located at CMD_INDIRECT_REF_0[7:0] or ALU_OUT[23:16], whichever is convenient.
  */
#define OV_BYTE_MASK          (1 <<  1)
/** Specifies 2-bit data master field.
  * For NFP6000 indirect reference format, located at ALU_OUT[31:30].
  *
  * @note
  * 4 bits are required to specify the data master. This specifies the two least significant bits;
  * the two most significant bits are assumed to be zero.
  */
#define OV_DATA_MASTER_2bits  (1 <<  2)
/** Specifies 4-bit data master field.
  * For NFP6000 indirect reference format, located at CMD_INDIRECT_REF_0[23:20].
  */
#define OV_DATA_MASTER        (1 <<  3)
/** Specifies 4-bit value field used for <b>both</b> the data master <b>and</b> the signal master.
  * For NFP6000 indirect reference format, located at CMD_INDIRECT_REF_0[23:20].
  */
#define OV_DATA_SIGNAL_MASTER (1 <<  4)
/** Specifies 14-bit data reference field.
  * For NFP6000 indirect reference format, located at ALU_OUT[29:16].
  */
#define OV_DATA_REF           (1 <<  5)
/** Specifies 3-bit data context field.
  * For NFP6000 indirect reference format, located at ALU_OUT[25:23].
  */
#define OV_DATA_CTX           (1 <<  6)
/** Specifies 3-bit field used for <b>both</b> data context <b>and</b> signal context.
  * For NFP6000 indirect reference format, located at ALU_OUT[25:23].
  */
#define OV_DATA_SIGNAL_CTX    (1 <<  7)
/** Specifies 7-bit data offset field.
  * For NFP6000 indirect reference format, located at ALU_OUT[22:16].
  */
#define OV_DATA_OFFSET        (1 <<  8)
/** Specifies 6-bit master island field.
  * For NFP6000 indirect reference format, located at CMD_INDIRECT_REF_0[29:24].
  */
#define OV_MASTER_ISLAND      (1 <<  9)
/** Specifies 3-bit signal context field.
  * For NFP6000 indirect reference format, located at CMD_INDIRECT_REF_0[15:13].
  */
#define OV_SIGNAL_CTX         (1 << 10)
/** Specifies 4-bit signal master field.
  * For NFP6000 indirect reference format, located at CMD_INDIRECT_REF_0[19:16].
  */
#define OV_SIGNAL_MASTER      (1 << 11)
/** Specifies 4-bit signal number field.
  * For NFP6000 indirect reference format, located at CMD_INDIRECT_REF_0[12:9].
  */
#define OV_SIGNAL_NUM         (1 << 12)
/** Specifies 14-bit sequence number field.
  * For NFP6000 indirect reference format, located at CMD_INDIRECT_REF_0[29:16].
  *
  * This is the concatenation of the fields OV_MASTER_ISLAND, OV_SIGNAL_CTX and
  * OV_SIGNAL_MASTER, accessed as a single field for the sake of convenience.
  */
#define OV_SEQ_NUM            (1 << 13)
/** Specifies 16-bit IMMED16 field, which is a superset of OV_DATA_REF.
  * For NFP6000 indirect reference format, located at ALU_OUT[31:16].
  */
#define OV_IMMED16            (1 << 14)
/** Specifies 8-bit IMMED8 field.
  * For NFP6000 indirect reference format, located at CMD_INDIRECT_REF_0[7:0].
  *
  * @note
  * This is not the same as OV_BYTE_MASK, because the location of OV_IMMED8 is fixed.
  */
#define OV_IMMED8             (1 << 15)
/// @cond INTERNAL_DEFINES
#define OV_FIELD_NEXT_BIT     (1 << 16)
/// @endcond
// Note: There is no provision for OV_TAT (TAT (TargetActionToken) CSR (for testing purposes only))
// Note: Up to (1 << _OV_MS_BIT_NUM) may be used in the above table


// Flags:

// Locations of pairs of flags within _OVF_GLOBAL and _OVF_CURRENT
// Bit ((_OVF_*_pair * 2) + 1) is set if flag is enabled.
// Bit (_OVF_*_pair * 2) is set if flag is disabled.

/// @cond INTERNAL_DEFINES
    #define _OVF_REUSE_pair             15
    #define _OVF_NO_CTX_SWAP_pair       14
    #define _OVF_NO_SWAP_SINCE_USE_pair 13
    #define _OVF_PARANOID_pair          12
    #define _OVF_RELAXED_ORDER_pair     11
    #define _OVF_PARTIAL_BITS_pair      10
    #define _OVF_SUBTRACT_pair           9

    #define _OVF_MASK_ENABLED  0xAAAAAAAA // Flag enabled if odd bit number is set.
    #define _OVF_MASK_DISABLED 0x55555555 // Flag disabled if even bit number (one less significant)
                                          // is set.
/// @endcond


// - Flags are globally set using ov_global_flags() (stored in _OVF_GLOBAL), and are used as a starting
//   point in ov_start().
// - The flags for the particular indirect reference (from ov_start() to ov_end()) are determined
//   from the global flags and the FLAGS parameter of ov_start() (stored in _OVF_CURRENT), where
//   the FLAGS parameter takes priority.
// - Within a particular call of ov_set(), ov_set_use() or ov_use(), the applicable flags are
//   determined from the parameter FLAGS and the particular indirect reference (stored in
//   _OVF_LOCAL), and are applicable for only that specific macro call.
//
// Note: A bit indicates the flag is ON (enabled), and a separate bit indicates the flag is OFF.
//       This is done so that local preferences can override global preferences, including detection
//       of setting a particular flag ON and OFF at the same time.

// _OVF_GLOBAL contains the current global set of flags. It is used in ov_start() as a starting
// point, which is then updated with the parameters provided to ov_start() to derive _OVF_CURRENT

/// @cond INTERNAL_DEFINES
    #define _OVF_GLOBAL 0

    #define _OV_FLAG_ON(bitpair)   (2 << (bitpair << 1))
    #define _OV_FLAG_OFF(bitpair)  (1 << (bitpair << 1))
    #define _OV_FLAG_BITS(bitpair) (3 << (bitpair << 1))
/// @endcond


/** @anchor OverrideFlags
  * @name Override flags
  *
  * Names of flags that can be provided to fields that can be provided to ov_global(), ov_start(),
  * ov_set*() and ov_*use() to fine-tune the functionality provided by these macros.
  *
  * Flags provided to ov_global() are the default flags used every time ov_start() is called. ov_global()
  * can be called at any time, but the value is only used during a call to ov_start().
  *
  * Flags provided to ov_start() override the current global flags, and are used for the entire indirect
  * reference, even if it is stored and later recalled or resumed.
  *
  * Flags provided to ov_set*() and ov_*use() override the current indirect reference's flags, and
  * are used for only that specific macro.
  */

/** Indicates that ov_*use() will be called more than once for the current indirect reference.
  *
  * The implication is that the final results must be stored to intermediate registers so that they
  * are available to be used again via multiple ov_*use().
  */
#define OVF_REUSE_ON          _OV_FLAG_ON(_OVF_REUSE_pair)
/** <b>Default flag</b> - Indicates that ov_*use() will be called exactly once for the current indirect reference.
  *
  * Because the indirect reference will NOT be used more than once, it may be possible to generate
  * more efficient code and use fewer registers in the situation where all data directed to previous
  * ALU result is available in ov_*use().
  */
#define OVF_REUSE_OFF         _OV_FLAG_OFF(_OVF_REUSE_pair)
// Implementation detail:
// - ov_start():
//   - _OVF_STATUS &= ~(_OVF_STATUS_USED | _OVF_STATUS_REUSED).
//   - If OVF_REUSE_ON selected, _OVF_STATUS |= _OVF_STATUS_ALWAYS_REUSE, otherwise
//     _OVF_STATUS &= ~_OVF_STATUS_ALWAYS_REUSE.
// - In every other macro call, if OVF_REUSE_ON is not selected, _OVF_STATUS &=
//   ~_OVF_STATUS_ALWAYS_REUSE.
// - In ov_*use(), if (_OVF_STATUS & _OVF_STATUS_USED) and
//   (!(_OVF_STATUS & _OVF_STATUS_ALWAYS_REUSE)), #error is issued, otherwise:
//   - If (_OVF_STATUS & _OVF_STATUS_USED), _OVF_STATUS |= _OVF_STATUS_REUSED
//   - _OVF_STATUS |= _OVF_STATUS_USED
// - In ov_clean():
//   - If !(_OVF_STATUS & (~_OVF_STATUS_USED)),
//     #warning is issued.
//   - If OVF_REUSE_ON is selected, and !(_OVF_STATUS & (~_OVF_STATUS_REUSED)),
//     #warning is issued.

/** Indicates that the macros may assume that the current microengine executes from ov_start()
  * up to the last ov_*use() without swapping context.
  *
  * The implication is that CSR CMD_INDIRECT_REF_0 may be written at the first convenient
  * opportunity and not again. This makes it possible to not store the data for this CSR
  * in a separate register, but to directly write it to CSR CMD_INDIRECT_REF_0.
  */
#define OVF_NO_CTX_SWAP_ON    _OV_FLAG_ON(_OVF_NO_CTX_SWAP_pair)
/** <b>Default flag</b> - Indicates that the macros must <b>not</b> assume that context swap
  * does not take place from ov_start() up to the last ov_*use().
  *
  * The implication is that CSR CMD_INDIRECT_REF_0 must be written every time ov_*use() is
  * called, as a different context on the same microengine could have changed the value of
  * CSR CMD_INDIRECT_REF_0.
  */
#define OVF_NO_CTX_SWAP_OFF   _OV_FLAG_OFF(_OVF_NO_CTX_SWAP_pair)
// Implementation detail:
// - When ov_start() is called, _OVF_STATUS &= ~_OVF_STATUS_IND_REF_RECENTLY_WRITTEN.
// - Whenever ov_set*() or ov_use() is called with flag OVF_NO_CTX_SWAP_OFF,
//   _OVF_STATUS &= ~_OVF_STATUS_IND_REF_RECENTLY_WRITTEN.
// - When CMD_INDIRECT_REF_0 is written,
//   _OVF_STATUS |= _OVF_STATUS_IND_REF_RECENTLY_WRITTEN
// - When the CSR CMD_INDIRECT_REF_0 needs to be written and OVF_NO_CTX_SWAP_ON is selected,
//   CSR CMD_INDIRECT_REF_0 is written if ((_OVF_STATUS & _OVF_STATUS_IND_REF_RECENTLY_WRITTEN) == 0),
//   and then _OVF_STATUS |= _OVF_STATUS_IND_REF_RECENTLY_WRITTEN.

/** Indicates that the macros can assume that a context swap has <b>not</b>
  * occurred from the previous ov_*use() to the current ov_use().
  *
  * This flag is only applicable to the second (or later) ov_*use() for a given indirect
  * reference. This flag may only be used with ov_*use(), not with ov_global() or ov_start().
  *
  * Specifying this flag permits the macros to not again write the value of CSR
  * CMD_INDIRECT_REF_0, as the user has indicated that its value has not been changed.
  */
#define OVF_NO_SWAP_SINCE_USE _OV_FLAG_ON(_OVF_NO_SWAP_SINCE_USE_pair)
// Implementation detail:
// - When ov_start() is called, _OVF_STATUS &= ~_OVF_STATUS_IND_REF_PREVIOUSLY_WRITTEN.
// - When CMD_INDIRECT_REF_0 is written,
//   _OVF_STATUS |= _OVF_STATUS_IND_REF_PREVIOUSLY_WRITTEN
// - When ov_*use() is called, if _OVF_STATUS & _OVF_STATUS_IND_REF_PREVIOUSLY_WRITTEN and
//   flag OVF_NO_SWAP_SINCE_USE is set, CMD_INDIRECT_REF_0 is NOT written.

/** Causes data provided from registers to be masked such that it cannot overflow into
  * other fields in the indirect reference.
  *
  * @note
  * This has significant additional instruction expense, but can be useful for
  * fault-finding.
  *
  * Compile-time constants are checked that they fit into the destination field (at
  * no instruction expense), and an error is issued if they do not.
  */
#define OVF_PARANOID_ON       _OV_FLAG_ON(_OVF_PARANOID_pair)
/** <b>Default flag</b> - Indicates that the ov_set*() macros should be gullible and
  * assume that data provided from registers fits within the specified field.
  *
  * @note
  * If the provided data does not fit within the specified fit, the indirect reference
  * will most likely be corrupted, as bits in fields stored in more significant fields
  * within the register will be set.
  */
#define OVF_PARANOID_OFF      _OV_FLAG_OFF(_OVF_PARANOID_pair)
// Implementation detail:
// - Every time OVF_PARANOID_ON is selected and a value is set, the value, if coming from a register,
//   is ORd with the mask of the destination field before being ORd into the destination.
//   (Constants are ORd with the mask of the destination regardless.)

/** Suppresses warnings when parameters are not provided in a sequence that enables
  * generation of optimal code.
  *
  * This should be specified when optimal code is not required, and it is more convenient
  * to provide field values in any order.
  *
  * @note
  * When provided as a parameter to ov_set*(), warnings are only suppressed for the current
  * field being provided, not subsequent fields.
  */
#define OVF_RELAXED_ORDER_ON  _OV_FLAG_ON(_OVF_RELAXED_ORDER_pair)
/** <b>Default flag</b> - warnings are generated when field values are not provided in an
  * order which allows for generation of optimal code, together with an indication of what
  * should be changed to obtain optimal order.
  *
  * To provide field values in the optimal order, the following should be adhered to: @n
  * -# In each of ALU_OUT and CMD_INDIRECT_REF_0, first provide all constants followed
  *    by all values from registers
  * -# If all 8 bits of byte mask are being set from a register <b>and</b> byte mask is
  *    located in the least significant 8 bits of CMD_INDIRECT_REF_0 <b>and</b>
  *    CMD_INDIRECT_REF_0 contains no constant values <b>and</b> CMD_INDIRECT_REF_0 does
  *    does contain other register values, use set_bm_and*() to set the byte mask value
  *    and another field in a single step
  * -# The final field provided should be for ALU_OUT, and should be combined with use,
  *    i.e. ov_set_use().
  */
#define OVF_RELAXED_ORDER_OFF _OV_FLAG_OFF(_OVF_RELAXED_ORDER_pair)
// Implementation detail:
// - In ov_start():
//   - _OVF_STATUS &= ~_OVF_STATUS_PREV_ALU_REG,
//   - _OVF_STATUS &= ~_OVF_STATUS_IND_CSR_REG,
//   - _OVF_STATUS &= ~_OVF_STATUS_PREV_ALU_NON_IMMED_CNST and
//   - _OVF_STATUS &= ~_OVF_STATUS_IND_CSR_NON_IMMED_CNST.
// - If a register value is provided to previous ALU result,
//   _OVF_STATUS |= _OVF_STATUS_PREV_ALU_REG is set.
// - If a register value is provided to Indirect CSR,
//   _OVF_STATUS |= _OVF_STATUS_IND_CSR_REG.
// - If a constant that can be put into a single instruction but does not fit within
//   MAX_IMMEDIATE is used together with a register value destined for
//   previous ALU result NOT starting at bit 0,
//   _OVF_STATUS |= _OVF_STATUS_PREV_ALU_NON_IMMED_CNST.
// - If a constant that can be put into a single instruction but does not fit within
//   MAX_IMMEDIATE is used together with a register value destined for
//   CMD_INDIRECT_REF_0 NOT starting at bit 0,
//   _OVF_STATUS |= _OVF_STATUS_IND_CSR_NON_IMMED_CNST.
// - If OVF_RELAXED_ORDER_OFF is selected, AND:
//   - A constant is provided for previous ALU result and
//     (_OVF_STATUS & _OVF_STATUS_PREV_ALU_REG), #warning is generated
//   - A constant is provided for Indirect CSR and
//     (_OVF_STATUS & _OVF_STATUS_IND_CSR_REG), #warning is generated
//   - A register is provided for previous ALU result for a field starting at bit 0 and
//     (_OVF_STATUS & _OVF_STATUS_PREV_ALU_NON_IMMED_CNST), #warning is generated
//   - A register is provided for CMD_INDIRECT_REF_0 for a field starting at bit 0 and
//     (_OVF_STATUS & _OVF_STATUS_IND_CSR_NON_IMMED_CNST), #warning is generated

/** Permits only providing partial field values by changing errors to warnings
  * when only some bits in indirect reference fields are provided.
  *
  * Bits that are not provided are assumed to be zero. At least one bit must be specified in
  * each field, otherwise error is issued because a value has not been specified for the field.
  *
  * @note
  * This should <b>not</b> be used for production code.
  */
#define OVF_PARTIAL_BITS_ON   _OV_FLAG_ON(_OVF_PARTIAL_BITS_pair)
/** <b>Default flag</b> - Issues error if all bits for every field of the indirect reference
  * are not provided.
  *
  * @note
  * This is a tool to ensure that every bit is set in an indirect reference, and should not
  * be disabled for production code.
  */
#define OVF_PARTIAL_BITS_OFF  _OV_FLAG_OFF(_OVF_PARTIAL_BITS_pair)
// Implementation detail:
//   When ov_*use() is called, the check that all bits within each field have been provided
//   results in #warning instead of #error. Note that at least one bit MUST still be specified
//   in each field.

/** When specified in ov_set*(), causes 1 to be subtracted from the provided portion
  * of the field.
  *
  * This is intended to be used with OV_LENGTH, as the encoding used is typically (length - 1).
  *
  * This flag cannot be set as a default in ov_global() or ov_start(), as that would lead
  * to unintuitive subtraction of one on <i>every</i> call to ov_set*().
  */
#define OVF_SUBTRACT_ONE      _OV_FLAG_ON(_OVF_SUBTRACT_pair)

// Flags preprocessor variables:

/// @cond INTERNAL_DEFINES
    // Bits within _OVF_STATUS:
    #define _OVF_STATUS_USED                        (1 << 0) // Set in ov_*use()
    #define _OVF_STATUS_REUSED                      (1 << 1) // Set in ov_*use(), if _OVF_STATUS_USED was already set
    #define _OVF_STATUS_ALWAYS_REUSE                (1 << 2) // Set if OVF_REUSE_ON has been continuously set since ov_start() called
    #define _OVF_STATUS_IND_REF_PREVIOUSLY_WRITTEN  (1 << 3) // Set when CSR CMD_INDIRECT_REF_0 is written
    #define _OVF_STATUS_IND_REF_RECENTLY_WRITTEN    (1 << 4) // Set when CSR CMD_INDIRECT_REF_0 is written; cleared when possible context swap
    #define _OVF_STATUS_PREV_ALU_REG                (1 << 5) // Set when a register value is provided to previous ALU result
    #define _OVF_STATUS_IND_CSR_REG                 (1 << 6) // Set when a register value is provided to Indirect CSR
    #define _OVF_STATUS_PREV_ALU_NON_IMMED_CNST     (1 << 7) // Set when a constant that can fit into a single instruction but not within
                                                             // MAX_IMMEDIATE is combined with a register value that doesn't start at
                                                             // bit 0 for previous ALU result
    #define _OVF_STATUS_IND_CSR_NON_IMMED_CNST      (1 << 8) // Set when a constant that can fit into a single instruction but not within
                                                             // MAX_IMMEDIATE is combined with a register value that doesn't start at
                                                             // bit 0 for CMD_INDIRECT_REF_0
    #define _OVF_STATUS_ALU_SRC_USED               (1 << 12) // Register being used for source for previous ALU result has been used
                                                             // Note: SRC is only relevent if corresponding DST is not set
    #define _OVF_STATUS_ALU_DST_USED               (1 << 13) // Register being used for destination for previous ALU result has been used
    #define _OVF_STATUS_IND_CSR_SRC_USED           (1 << 14) // Register being used for source for previous ALU result has been used
                                                             // Note: SRC is only relevent if corresponding DST is not set
    #define _OVF_STATUS_IND_CSR_DST_USED           (1 << 15) // Register being used for destination for previous ALU result has been used
/// @endcond


// Conventions related to registers (for each of previous ALU result and CMD_INDIRECT_REF_0):
// - There a destination register, which is where the value accumulates:
//   - A flag specifies whether the destination register is in use (_OVF_STATUS_ALU_DST_USED /
//     _OVF_STATUS_IND_CSR_DST_USED in _OVF_STATUS, and _OV_REG_DST_USED in the common code)
//   - The number of the destination register (_OV_REG_ALU_DST / _OV_REG_IND_DST) that is
//     appended to "_ov_reg_alu_" / "_ov_reg_ind_csr_", and known as _ov_reg_dst in the common code.
// - There a source register, which is where the value comes from (needed specifically when
//   recalling a previously-stored indirect reference, and we do not want to explicitly copy
//   a value before using it):
//   - A flag specifies whether the source register is in use (_OVF_STATUS_ALU_SRC_USED /
//     _OVF_STATUS_IND_CSR_SRC_USED in _OVF_STATUS, and _OV_REG_SRC_USED in the common code)
//   - The number of the source register (_OV_REG_ALU_SRC / _OV_REG_IND_SRC) that is
//     appended to "_ov_reg_alu_" / "_ov_reg_ind_csr_", and known as _ov_reg_src in the common code.
// - When data is read, the source version is used, if the source in-use flag is set.
// - When data is written, the destination version is used AND:
//   - The destination is marked as being used
//   - The source is marked as being used
//   - The number of the source register is set to the number of the destination register (source
//     effectively used once, thereafter the source is exactly equivalent to the destination)
// - When a new indirect reference is started, the source and destination point to the same
//   set of unused registers.
// - When an indirect reference is resumed, the source and destination point to the previously-
//   used set of registers.
// - When an indirect reference is recalled, the source points to the previously-used set of
//   registers, and the destination points to an unused set of registers.
// - IMPORTANT: When an indirect reference is stored, and the source is in-use, but the
//   destination is not, the data is copied from the source to the destination, so that the
//   stored copy is independent of any other stored indirect reference.


/// @cond INTERNAL_DEFINES
#if ((defined(__NFP6000) && (IS_IXPTYPE(__NFP6000) || IS_NFPTYPE(__NFP3800))) && defined(__NFP_INDIRECT_REF_FORMAT_V2))

    #define _OV_INDIRECT_FORMAT "Thornham NFP6000"

    // Data related to above-defined types for Thornham architecture:
    // Note: Order of rows in this table is irrelevant, other than for a particular "Name of override" -
    //       the first-occurring takes preference.
    //                                         Data stored Config stored (in prev ALU)
    //                                         =========== ====================
    //      Name        Name of override       (r) MSb LSb Mask       Value
    //      =========== ====================== === === === =========  =========
    #define _OV_TBL_0   OV_BYTE_MASK         ,  0, 23, 16, (7 <<  3), (6 <<  3) // Used if only OV_BYTE_MASK and possibly OV_LENGTH are specified
    #define _OV_TBL_1   OV_BYTE_MASK         ,  1,  7,  0, (1 <<  6), (1 <<  6) // More general version of BYTE_MASK
    #define _OV_TBL_2   OV_LENGTH            ,  0, 12,  8, (1 <<  7), (1 <<  7)
    #define _OV_TBL_3   OV_DATA_MASTER_2bits ,  0, 31, 30, (7 <<  3), (2 <<  3)
    #define _OV_TBL_4   OV_DATA_MASTER       ,  1, 23, 20, (3 <<  1), (1 <<  1)
    #define _OV_TBL_5   OV_DATA_SIGNAL_MASTER,  1, 23, 20, (3 <<  1), (2 <<  1)
    #define _OV_TBL_6   OV_DATA_REF          ,  0, 29, 16, (7 <<  3), (1 <<  3)
    #define _OV_TBL_7   OV_DATA_REF          ,  0, 29, 16, (7 <<  3), (2 <<  3)
    #define _OV_TBL_8   OV_DATA_CTX          ,  0, 25, 23, (7 <<  3), (3 <<  3)
    #define _OV_TBL_9   OV_DATA_SIGNAL_CTX   ,  0, 25, 23, (7 <<  3), (4 <<  3)
    #define _OV_TBL_10  OV_DATA_OFFSET       ,  0, 22, 16, (7 <<  3), (5 <<  3)
    #define _OV_TBL_11  OV_MASTER_ISLAND     ,  1, 29, 24, (3 <<  1), (1 <<  1)
    #define _OV_TBL_12  OV_MASTER_ISLAND     ,  1, 29, 24, (3 <<  1), (2 <<  1)
    #define _OV_TBL_13  OV_MASTER_ISLAND     ,  1, 29, 24, (3 <<  1), (3 <<  1)
    #define _OV_TBL_14  OV_SIGNAL_CTX        ,  1, 15, 13, (1 << 14), (1 << 14)
    #define _OV_TBL_15  OV_SIGNAL_MASTER     ,  1, 19, 16, (1 <<  0), (1 <<  0)
    #define _OV_TBL_16  OV_SIGNAL_NUM        ,  1, 12,  9, (1 << 13), (1 << 13)
    #define _OV_TBL_17  OV_SEQ_NUM           ,  1, 29, 16, (7 <<  0), (3 <<  0) // Overrides OV_MASTER_ISLAND, OV_DATA_MASTER and OV_SIGNAL_MASTER
    #define _OV_TBL_18  OV_IMMED16           ,  0, 31, 16, (7 <<  3), (2 <<  3)
    #define _OV_TBL_19  OV_IMMED8            ,  1,  7,  0, (1 <<  6), (1 <<  6)
    #define _OV_TBL_COUNT 20
    // (r): Register: 0: previous ALU result, 1: Indirect CSR

    #define _OV_SUPPORTED_PAR (OV_LENGTH | OV_BYTE_MASK | OV_DATA_MASTER_2bits | \
                               OV_DATA_MASTER | OV_DATA_SIGNAL_MASTER | OV_DATA_REF | OV_DATA_CTX | \
                               OV_DATA_SIGNAL_CTX | OV_DATA_OFFSET | OV_MASTER_ISLAND | OV_SIGNAL_CTX | \
                               OV_SIGNAL_MASTER | OV_SIGNAL_NUM | OV_SEQ_NUM | OV_IMMED16 | OV_IMMED8)

#elif (((defined(__NFP6000) && (IS_IXPTYPE(__NFP6000) || IS_NFPTYPE(__NFP3800))) && defined(__NFP_INDIRECT_REF_FORMAT_V1)) || \
       ((defined(__NFP3200) && (IS_IXPTYPE(__NFP3200))) || defined(__NFP_INDIRECT_REF_FORMAT_V1)))

    #ifndef __NFP_INDIRECT_REF_FORMAT_V1 // as opposed to __NFP_INDIRECT_REF_FORMAT_IXP

        #error "NFP_32XX indirect format is NOT currently enabled, so this code CANNOT be compiled"

    #endif

    #define _OV_INDIRECT_FORMAT "Yeldham NFP3200 extended"

    // Data related to above-defined types for Yeldham (NFP 32xx) - NFP Enhance Mode indirect format:
    // Note: Order of rows in this table is irrelevant, other than for a particular "Name of override" -
    //       the first-occurring takes preference.
    //                                         Data stored Config stored (in prev ALU)
    //                                         =========== ====================
    //      Name        Name of override       (r) MSb LSb Mask         Value
    //      =========== ====================== === === === ===========  ===========

    #warning "For" _OV_INDIRECT_FORMAT "indirect reference format, only indirect references involving only LENGTH, DATA_REF and BYTE_MASK have been implemented, due to ambiguity in PRM_ME_nfp3200.pdf"

    #define _OV_TBL_0   OV_DATA_REF          ,  0, 18,  5, (0xF << 28), (0x0 << 28)
    #define _OV_TBL_1   OV_DATA_REF          ,  0, 18,  5, (0xF << 28), (0x1 << 28)
    #define _OV_TBL_2   OV_LENGTH            ,  0,  4,  0, (0xF << 28), (0x1 << 28)
    #define _OV_TBL_3   OV_LENGTH            ,  0,  4,  0, (0xF << 28), (0x2 << 28)
    #define _OV_TBL_4   OV_LENGTH            ,  0, 12,  8, (0xF << 28), (0x3 << 28)
    #define _OV_TBL_5   OV_BYTE_MASK         ,  0,  7,  0, (0xF << 28), (0x3 << 28)
    #define _OV_TBL_6   OV_BYTE_MASK         ,  0,  7,  0, (0xF << 28), (0x4 << 28)

    //#define _OV_TBL_-   OV_                  ,  0,  -,  -, (0xF << 28), (0x- << 28) // Blank line for future use

    #define _OV_TBL_COUNT 7
    // (r): Register: 0: previous ALU result, 1: Indirect CSR

    #define _OV_SUPPORTED_PAR (OV_LENGTH | OV_DATA_REF | OV_BYTE_MASK)

#else

    #error "Indirect reference macros can only be compiled on NFP6000 (either 6000 or 3200 indirect ref format) or NFP3200 (compiled with enhanced NFP indirect references)"

#endif

#define _OV_TBL_COLUMNS 5

#ifdef _OV_PREVIOUS_ALU_TO_REGISTER

    // Every time ov_*use() is called, the ALU value will be written to register _ov_previous_alu_value
    // Note: Warning 4101 (warning of not used) is specifically NOT disabled, because this value
    //       SHOULD always be used in testing.
    .reg global _ov_previous_alu_value

#endif
/// @endcond


/*

Algorithm to select combination of _OV_TBL_ rows (such that one row is selected for each
override indicated (OV_...) and the data locations do not overlap and the configuration is
consistent), in ov_start():

 1. Finished = FALSE
 2  _OV_PAR_COUNT = Number of 1s in _OV_PAR_REQUESTED
 3. For each provided parameter, set entry offset to zero: _OV_OFFSET_PAR[i]
 4. While (!Finished) {
 5.   CheckRow = TRUE
 6.   While (!Finished && CheckRow) {
 7.     Get row (offset from beginning by _OV_OFFSET_PAR[i]) for each provided parameter from _OV_REF_DATA: _OV_ROW_PAR[i]
 8.     If (Full set of rows) {
 9.       CheckRow = FALSE
10.       Check that all parameters corresponding to the configuration are going to be provided
11.       Finished = (Data locations do not overlap) && (Configuration data is consistent)
12.       Increment offset counters - least significant++
13.     } else {
14.       If (Valid row for most significant parameter) {
15.         Finished = TRUE
16.       } else {
17.         Increment offset counters [most significant that wasn't set and all less significant = 0, and next more significant must be incremented]
18.       }
19.     }
20.   }
21. }
22. If (CheckRow) {
23.   #error "Supplied combination of overrides not supported"
24. }
25. // Proceed to process data using the current set of row data...

To implement the code above, we need two arrays - one to temporarily store the offset
for a particular overridden parameter, _OV_OFFSET_PAR[i] (only used in ov_start()), and
another to store the offset being used in the _OV_TBL_ table, _OV_ROW_PAR[i] (which
is used as values for parameters are provided). Both of these have _OV_PAR_COUNT entries,
and we need to read and write them.

Macros and defines for these arrays are defined below as
    _ov_offset_init(), _ov_offset_read(index) and _ov_offset_write(index, value)
        and
    _ov_row_init(), _ov_row_read(index) and _ov_row_write(index, value)
*/

// =========== Macros for offset "array" ========================================

/// @cond INTERNAL_MACROS
#macro _ov_offset_init()

    #define_eval _OV_OFFSET_PAR ''

#endm
/// @endcond


/// @cond INTERNAL_MACROS
#define _ov_offset_read(index) strleft(strright('_OV_OFFSET_PAR', -4 * (index)), 4)
/// @endcond


/// @cond INTERNAL_MACROS
#macro _ov_offset_write(index, value)

    #define_eval _OV_LEN strlen('_OV_OFFSET_PAR')

    #define_eval _OV_LEFT strleft('_OV_OFFSET_PAR', (index * 4))
    #if ((_OV_LEN - (index * 4) - 4) > 0)
        #define_eval _OV_RIGHT strright('_OV_OFFSET_PAR', (_OV_LEN - (index * 4) - 4))
    #else
        // strright() does not return an empty string when token2 is 0
        #define_eval _OV_RIGHT ''
    #endif

    #define_eval _OV_MIDDLE strright('0000/**/value', 4)

    #define_eval _OV_OFFSET_PAR '_OV_LEFT/**/_OV_MIDDLE/**/_OV_RIGHT'

    #undef _OV_LEFT
    #undef _OV_MIDDLE
    #undef _OV_RIGHT
    #undef _OV_LEN

#endm
/// @endcond

// =========== Macros for parameter row count "array" ===========================

/// @cond INTERNAL_MACROS
#macro _ov_parameter_row_count_init()

    #define_eval _OV_PARAMETER_ROW_COUNT_PAR ''

#endm
/// @endcond


/// @cond INTERNAL_MACROS
#define _ov_parameter_row_count_read(index) strleft(strright('_OV_PARAMETER_ROW_COUNT_PAR', -4 * (index)), 4)
/// @endcond


/// @cond INTERNAL_MACROS
#macro _ov_parameter_row_count_write(index, value)

    #define_eval _OV_LEN strlen('_OV_PARAMETER_ROW_COUNT_PAR')

    #define_eval _OV_LEFT strleft('_OV_PARAMETER_ROW_COUNT_PAR', (index * 4))
    #if ((_OV_LEN - (index * 4) - 4) > 0)
        #define_eval _OV_RIGHT strright('_OV_PARAMETER_ROW_COUNT_PAR', (_OV_LEN - (index * 4) - 4))
    #else
        // strright() does not return an empty string when token2 is 0
        #define_eval _OV_RIGHT ''
    #endif

    #define_eval _OV_MIDDLE strright('0000/**/value', 4)

    #define_eval _OV_PARAMETER_ROW_COUNT_PAR '_OV_LEFT/**/_OV_MIDDLE/**/_OV_RIGHT'

    #undef _OV_LEFT
    #undef _OV_MIDDLE
    #undef _OV_RIGHT
    #undef _OV_LEN

#endm
/// @endcond

// =========== Macros for row "array" ===========================================

/// @cond INTERNAL_MACROS
#macro _ov_row_init()

    #define_eval _OV_ROW_PAR ''

#endm
/// @endcond


/// @cond INTERNAL_MACROS
#define _ov_row_read(index) strleft(strright('_OV_ROW_PAR', -4 * (index)), 4)
/// @endcond


/// @cond INTERNAL_MACROS
#macro _ov_row_write(index, value)

    #define_eval _OV_LEN strlen('_OV_ROW_PAR')

    #define_eval _OV_LEFT strleft('_OV_ROW_PAR', (index * 4))
    #if ((_OV_LEN - (index * 4) - 4) > 0)
        #define_eval _OV_RIGHT strright('_OV_ROW_PAR', (_OV_LEN - (index * 4) - 4))
    #else
        // strright() does not return an empty string when token2 is 0
        #define_eval _OV_RIGHT ''
    #endif

    #define_eval _OV_MIDDLE strright('0000/**/value', 4)

    #define_eval _OV_ROW_PAR '_OV_LEFT/**/_OV_MIDDLE/**/_OV_RIGHT'

    #undef _OV_LEFT
    #undef _OV_MIDDLE
    #undef _OV_RIGHT
    #undef _OV_LEN

#endm
/// @endcond


// =========== Local instantiation for _ov_va_list macros ===========================

/// @cond INTERNAL_MACROS
#macro _ov_va_start(_ov_va_list)

    #if (defined(_OV_VA_LIST) + defined(_OV_VA_DELIMIT) + defined(_OV_VA_NUM))
        #error "_OV_VA variables are already defined. Was _ov_va_end() called?"
    #endif

    #define_eval _OV_VA_LIST _ov_va_list
    #define_eval _OV_VA_DELIMIT '0000'
    #define_eval _OV_VA_NUM 1

    // While there is still a comma in the list...
    #while (strstr('_OV_VA_LIST', ",") > 0)

        // Find the index of the next comma
        #define_eval _OV_VA_IDX strstr('_OV_VA_LIST', ",")
        #define_eval _OV_VA_NUM (_OV_VA_NUM + 1)

        // Replace the comma with a colon in the working copy
        #define_eval _OV_VA_LIST_LEFT strleft('_OV_VA_LIST', _OV_VA_IDX - 1)
        #define_eval _OV_VA_LIST_RIGHT strright('_OV_VA_LIST', -_OV_VA_IDX)
        #define_eval _OV_VA_LIST '_OV_VA_LIST_LEFT:_OV_VA_LIST_RIGHT'

        // Record the position of the comma as a 4 digit string
        #define_eval _OV_VA_IDX strright('0000/**/_OV_VA_IDX', 4)
        #define_eval _OV_VA_DELIMIT '_OV_VA_DELIMIT/**/_OV_VA_IDX'

        #undef _OV_VA_LIST_LEFT
        #undef _OV_VA_LIST_RIGHT

    #endloop

    // Record the end of list as a 4 digit string
    #define_eval _OV_VA_IDX strlen('_OV_VA_LIST') + 1
    #define_eval _OV_VA_IDX strright('0000/**/_OV_VA_IDX', 4)
    #define_eval _OV_VA_DELIMIT '_OV_VA_DELIMIT/**/_OV_VA_IDX'

    // Save the unmodified list
    #define_eval _OV_VA_LIST _ov_va_list

    #undef _OV_VA_IDX

#endm // _ov_va_start()
/// @endcond


/// @cond INTERNAL_MACROS
#macro _ov_va_arg_index(INDEX)

    #if ((defined(_OV_VA_LIST) + defined(_OV_VA_DELIMIT) + defined(_OV_VA_NUM)) < 3)
        #error "VA variables are not defined. Was _ov_va_start() called?"
    #endif

    #if ((INDEX) < 0 || (INDEX) >= _OV_VA_NUM)
        #error "_OV_VA_ARG index out of range" INDEX _OV_VA_NUM
    #endif

    // Get the begin and end delimiters for the item
    #define_eval _OV_VA_BEG strleft(strright('_OV_VA_DELIMIT', -4 * (INDEX)), 4)
    #define_eval _OV_VA_END strleft(strright('_OV_VA_DELIMIT', -4 * (INDEX) - 4), 4)

    #define_eval _OV_VA_ARG strright(strleft('_OV_VA_LIST', _OV_VA_END - 1), -_OV_VA_BEG)
    #define_eval _OV_VA_ARG '_OV_VA_ARG'

    #if (strlen('_OV_VA_ARG') < 1)
        #error "Not a valid argument" _OV_VA_ARG "List:" _OV_VA_LIST
    #endif

    #undef _OV_VA_BEG
    #undef _OV_VA_END

#endm
/// @endcond


/// @cond INTERNAL_MACROS
#macro _ov_va_end()

    #if ((defined(_OV_VA_LIST) + defined(_OV_VA_DELIMIT) + defined(_OV_VA_NUM)) < 3)
        #error "VA variables are not defined. Was _ov_va_start() called?"
    #endif

    #undef _OV_VA_LIST
    #undef _OV_VA_DELIMIT
    #undef _OV_VA_NUM

#endm
/// @endcond


/// @cond INTERNAL_MACROS
#macro _ov_va_disp(_ov_va_list)

    #ifdef _OV_VA_LOOP
        #error "_OV_VA_LOOP is already defined"
    #endif

    _ov_va_start(_ov_va_list)

    #warning "Contents of" _ov_va_list "("_OV_VA_NUM "items):"

    #define _OV_VA_LOOP 0
    #while (_OV_VA_LOOP < _OV_VA_NUM)

        _ov_va_arg_index(_OV_VA_LOOP)

        #warning "["_OV_VA_LOOP"] =" _OV_VA_ARG

        #define_eval _OV_VA_LOOP (_OV_VA_LOOP + 1)
    #endloop
    #undef _OV_VA_LOOP

    _ov_va_end()

#endm
/// @endcond


/// @cond INTERNAL_MACROS
#macro _ov_struct(IDX)

    #define_eval _OV_LIST '_OV_TBL_/**/IDX'
    //_ov_va_disp("_OV_LIST") // Useful for debugging

    _ov_va_start("_OV_LIST")

    _ov_va_arg_index(0)
    #define_eval _OV_ROW_OVERRIDE (_OV_VA_ARG)

    _ov_va_arg_index(1)
    #define_eval _OV_ROW_DATA_REG (_OV_VA_ARG)

    _ov_va_arg_index(2)
    #define_eval _OV_ROW_DATA_MSbit (_OV_VA_ARG)

    _ov_va_arg_index(3)
    #define_eval _OV_ROW_DATA_LSbit (_OV_VA_ARG)

    _ov_va_arg_index(4)
    #define_eval _OV_ROW_CONF_MASK (_OV_VA_ARG)

    _ov_va_arg_index(5)
    #define_eval _OV_ROW_CONF_VALUE (_OV_VA_ARG)

    _ov_va_end()

    #undef _OV_LIST

#endm
/// @endcond


/// @cond INTERNAL_MACROS
#macro _ov_va_read_OV_TBL_first(IDX)

    #define_eval _OV_VA_LIST '_OV_TBL_/**/IDX'
    #define_eval _OV_VA_NUM 0
    #define_eval _OV_VA_IDX_PREV (0)

    // Find the index of the next comma
    #define_eval _OV_VA_IDX strstr('_OV_VA_LIST', ",")

    #define_eval _OV_VA_ARG strright(strleft('_OV_VA_LIST', _OV_VA_IDX - 1), - _OV_VA_IDX_PREV)
    #define_eval _OV_VA_ARG (_OV_VA_ARG)

    #define_eval _OV_ROW_OVERRIDE (_OV_VA_ARG)

    #undef _OV_VA_LIST
    #undef _OV_VA_NUM
    #undef _OV_VA_IDX_PREV
    #undef _OV_VA_IDX

#endm
/// @endcond


/// @cond INTERNAL_MACROS
#macro _ov_va_read_OV_TBL_all(IDX)

    #define_eval _OV_VA_LIST '_OV_TBL_/**/IDX'
    #define_eval _OV_VA_NUM 0
    #define_eval _OV_VA_IDX_PREV (0)

    // While there is still a comma in the list...
    #while (strstr('_OV_VA_LIST', ",") > 0)

        // Find the index of the next comma
        #define_eval _OV_VA_IDX strstr('_OV_VA_LIST', ",")

        // Replace the comma with a colon in the working copy
        #define_eval _OV_VA_LIST_LEFT strleft('_OV_VA_LIST', _OV_VA_IDX - 1)
        #define_eval _OV_VA_LIST_RIGHT strright('_OV_VA_LIST', -_OV_VA_IDX)
        #define_eval _OV_VA_LIST '_OV_VA_LIST_LEFT:_OV_VA_LIST_RIGHT'

        #undef _OV_VA_LIST_LEFT
        #undef _OV_VA_LIST_RIGHT

        #define_eval _OV_VA_ARG strright(strleft('_OV_VA_LIST', _OV_VA_IDX - 1), - _OV_VA_IDX_PREV)
        #define_eval _OV_VA_ARG (_OV_VA_ARG)

        #if (_OV_VA_NUM == 0)

            #define_eval _OV_ROW_OVERRIDE (_OV_VA_ARG)

        #elif (_OV_VA_NUM == 1)

            #define_eval _OV_ROW_DATA_REG (_OV_VA_ARG)

        #elif (_OV_VA_NUM == 2)

            #define_eval _OV_ROW_DATA_MSbit (_OV_VA_ARG)

        #elif (_OV_VA_NUM == 3)

            #define_eval _OV_ROW_DATA_LSbit (_OV_VA_ARG)

        #elif (_OV_VA_NUM == 4)

            #define_eval _OV_ROW_CONF_MASK (_OV_VA_ARG)

        #elif (_OV_VA_NUM == 5)

            // Final entry could have a trailing comma
            #define_eval _OV_ROW_CONF_VALUE (_OV_VA_ARG)

        #endif

        #define_eval _OV_VA_IDX_PREV (_OV_VA_IDX + 1)

        #define_eval _OV_VA_NUM (_OV_VA_NUM + 1)

    #endloop

    #if (_OV_VA_NUM == _OV_TBL_COLUMNS)

        // Final entry doesn't necessarily have a trailing comma
        #define_eval _OV_VA_ARG strright('_OV_VA_LIST', - _OV_VA_IDX_PREV)
        #define_eval _OV_ROW_CONF_VALUE (_OV_VA_ARG)

    #endif

    #undef _OV_VA_LIST
    #undef _OV_VA_NUM
    #undef _OV_VA_IDX_PREV
    #undef _OV_VA_IDX

#endm
/// @endcond


/**
  * Set the values of the global flags, from this point forward.
  *
  * This macro can be called at any time. A copy of these flags is used as a starting point
  * when ov_start() is called.
  *
  * @b Example:
  * @code
  * ov_global_flags((OVF_NO_CTX_SWAP_OFF | OVF_RELAXED_ORDER_ON))
  * @endcode
  *
  * @param FLAGS   Zero or more flags from @ref OverrideFlags "Override flags", ORd together.
  *                Flags not specified are assumed to be _OFF.
  *
  * @condition_codes Unchanged
  *
  * @instruction_cnt None - only preprocessor
  */
#macro ov_global_flags(FLAGS)

    // Check that provided flags do not contradict each other...
    #if (((FLAGS & _OVF_MASK_ENABLED) & ((FLAGS << 1) & _OVF_MASK_ENABLED)) != 0)

        // There are one or more flags where both enable and disable are set
        #error "For each flag, only the flag or the inverse may be defined, not both. Note that multiple flags ORd together MUST be specified in brackets - ov_global((x | y))"

    #endif

    #if ((FLAGS & _OV_FLAG_BITS(_OVF_NO_SWAP_SINCE_USE_pair)) != 0)

        #error "The flag OVF_NO_SWAP_SINCE_USE may only be used at the second (or later) use of ov_*use()."

    #endif

    #if ((FLAGS & _OV_FLAG_BITS(_OVF_SUBTRACT_pair)) != 0)

        // May not use OVF_SUBTRACT_ONE at global level - will lead to nonintuitive results
        #error "The OVF_SUBTRACT_ONE flag may not be specified at the global level."

    #endif

    #define_eval _OVF_GLOBAL FLAGS

#endm


/// @cond INTERNAL_MACROS
#macro comment(COMMENT)

    // Empty macro, but the parameter passed to this macro is displayed in the list file -
    // method to get any text, including generated by preprocessor defines, into the list file.

#endm
/// @endcond


/// @cond INTERNAL_MACROS
#macro _ov_display_flags(LABEL, FLAGS)

    #ifdef _OV_FLAGS_STR
        #error "_OV_FLAGS_STR already defined!"
    #endif

    #define _OV_FLAGS_STR

    #if (FLAGS & OVF_REUSE_ON)
        #define_eval _OV_FLAGS_STR '_OV_FLAGS_STR "OVF_REUSE_ON"'
    #endif
    #if (FLAGS & OVF_REUSE_OFF)
        #define_eval _OV_FLAGS_STR '_OV_FLAGS_STR "OVF_REUSE_OFF"'
    #endif

    #if (FLAGS & OVF_NO_CTX_SWAP_ON)
        #define_eval _OV_FLAGS_STR '_OV_FLAGS_STR "OVF_NO_CTX_SWAP_ON"'
    #endif
    #if (FLAGS & OVF_NO_CTX_SWAP_OFF)
        #define_eval _OV_FLAGS_STR '_OV_FLAGS_STR "OVF_NO_CTX_SWAP_OFF"'
    #endif

    #if (FLAGS & OVF_NO_SWAP_SINCE_USE)
        #define_eval _OV_FLAGS_STR '_OV_FLAGS_STR "OVF_NO_SWAP_SINCE_USE"'
    #endif

    #if (FLAGS & OVF_PARANOID_ON)
        #define_eval _OV_FLAGS_STR '_OV_FLAGS_STR "OVF_PARANOID_ON"'
    #endif
    #if (FLAGS & OVF_PARANOID_OFF)
        #define_eval _OV_FLAGS_STR '_OV_FLAGS_STR "OVF_PARANOID_OFF"'
    #endif

    #if (FLAGS & OVF_RELAXED_ORDER_ON)
        #define_eval _OV_FLAGS_STR '_OV_FLAGS_STR "OVF_RELAXED_ORDER_ON"'
    #endif
    #if (FLAGS & OVF_RELAXED_ORDER_OFF)
        #define_eval _OV_FLAGS_STR '_OV_FLAGS_STR "OVF_RELAXED_ORDER_OFF"'
    #endif

    #if (FLAGS & OVF_PARTIAL_BITS_ON)
        #define_eval _OV_FLAGS_STR '_OV_FLAGS_STR "OVF_PARTIAL_BITS_ON"'
    #endif
    #if (FLAGS & OVF_PARTIAL_BITS_OFF)
        #define_eval _OV_FLAGS_STR '_OV_FLAGS_STR "OVF_PARTIAL_BITS_OFF"'
    #endif

    #if (FLAGS & OVF_SUBTRACT_ONE)
        #define_eval _OV_FLAGS_STR '_OV_FLAGS_STR "OVF_SUBTRACT_ONE"'
    #endif

    //#warning "Flags:" LABEL": "_OV_FLAGS_STR

    #ifdef _CONCATENATED
        #warning "_CONCATENATED already defined!"
    #endif

    #define_eval _CONCATENATED 'Flags: LABEL: _OV_FLAGS_STR'

    comment(_CONCATENATED)

    #undef _CONCATENATED

    #undef _OV_FLAGS_STR

#endm
/// @endcond


/// @cond INTERNAL_MACROS
#macro _ov_declare_global()

    // Define global registers if has not already been done, or if the registers are now a different set
    #if ((!defined(_OV_GLOBAL_DECLARED)) || (_OV_GLOBAL_DECLARED < _OV_STORE_REGISTER_SET))

        // Define these global variables exactly once
        #define_eval _OV_GLOBAL_DECLARED _OV_STORE_REGISTER_SET

        #define_eval _OV_REG_ALU_DST _OV_STORE_REGISTER_SET
        #define_eval _OV_REG_IND_DST _OV_STORE_REGISTER_SET
        // The _SRC version of the above is assigned when copying from a different source

        #pragma warning(push)
        #pragma warning(disable:4101) // Disable warning of not used... code simpler by defining in advance
        .reg global _ov_reg_alu_/**/_OV_REG_ALU_DST
        #pragma warning(pop)

        #if ((defined(__NFP6000) && (IS_IXPTYPE(__NFP6000) || IS_NFPTYPE(__NFP3800))) && defined(__NFP_INDIRECT_REF_FORMAT_V2))

            #pragma warning(push)
            #pragma warning(disable:4101) // Disable warning of not used... code simpler by defining in advance
            .reg global _ov_reg_ind_csr_/**/_OV_REG_IND_DST
            #pragma warning(pop)

        #endif

    #endif // #if ((!defined(_OV_GLOBAL_DECLARED)) || (_OV_GLOBAL_DECLARED < _OV_STORE_REGISTER_SET))

#endm
/// @endcond


/**
  * Start an indirect reference.
  *
  * This macro creates the context within which the indirect reference is determined.
  * It is always the first step of an indirect reference, and may only be called if an
  * indirect reference is not already in progress.
  *
  * @b Example:
  * @code
  * ov_start((OV_LENGTH | OV_DATA_REF), OVF_REUSE_ON)
  * @endcode
  *
  * @note
  * The validity of the provided parameters are checked against the indirect reference
  * environment currently being compiled for.
  *
  * @param OVERRIDE_TYPES  One or more override fields from @ref OverrideFields
  *                        "Override fields", ORd together.
  * @param FLAGS           Optional: Zero or more flags from @ref OverrideFlags "Override flags",
  *                        ORd together, which are used for the entire duration of this
  *                        indirect reference. Flags specified here override the global
  *                        flags most recently specified using ov_global_flags().
  *
  * @note
  * Constants ORd together must be enclosed within round brackets.
  *
  * @condition_codes Unchanged
  *
  * @instruction_cnt None - only preprocessor
  */
#macro ov_start(OVERRIDE_TYPES, FLAGS)
.begin

    #ifdef _OV_PAR_REQUESTED

        #error "ov_clean() must be called before ov_start() may be called again."

    #elif (OVERRIDE_TYPES == 0)

        #error "Non-zero value for OVERRIDE_TYPES must be provided, or use () around const | const."

    #elif ((OVERRIDE_TYPES & _OV_SUPPORTED_PAR) == 0)

        #error "One or more supported types must be specified for overriding."

    #elif ((OVERRIDE_TYPES & (~_OV_SUPPORTED_PAR)) != 0)

        #error "One or more unsupported types have been specified."

    #else

        // Any possible combination of types can be specified, except for no types, which
        // has already been tested for.

        #define_eval _OV_PAR_REQUESTED  (OVERRIDE_TYPES)

        // Override flags with those locally provided...
        // Check that provided flags do not contradict each other...
        #define_eval _OV_FLAG_TEST ((FLAGS & _OVF_MASK_ENABLED) & ((FLAGS << 1) & _OVF_MASK_ENABLED))
        #if (_OV_FLAG_TEST != 0)

            // There are one or more flags where both enable and disable are set
            #error "For each flag, only enable or disable may be specified, not both."

        #endif
        #undef _OV_FLAG_TEST

        #define_eval _OVF_CURRENT _OVF_GLOBAL // Flags currently applicable

        // Clear global flags where local flags are enabled
        #define_eval _OVF_CURRENT (_OVF_CURRENT & (~((FLAGS & _OVF_MASK_ENABLED) | ((FLAGS & _OVF_MASK_ENABLED) >> 1))))

        // Clear global flags where local flags are disabled
        #define_eval _OVF_CURRENT (_OVF_CURRENT & (~((FLAGS & _OVF_MASK_DISABLED) | ((FLAGS & _OVF_MASK_DISABLED) << 1))))

        // Add local flags
        #define_eval _OVF_CURRENT (_OVF_CURRENT | FLAGS)

        // If a flag is not set, must set to off, so that a flag is deterministically on or off
        #define_eval _OVF_CURRENT (_OVF_CURRENT | ((~_OVF_CURRENT & _OVF_MASK_ENABLED) >> 1))

        #if ((FLAGS & _OV_FLAG_BITS(_OVF_NO_SWAP_SINCE_USE_pair)) != 0)

            #error "The flag OVF_NO_SWAP_SINCE_USE may only be specified at the second (or later) use of ov_*use()."

        #endif

        #if ((FLAGS & _OV_FLAG_BITS(_OVF_SUBTRACT_pair)) != 0)

            // May not use OVF_SUBTRACT_ONE at global level - will lead to nonintuitive results
            #error "The OVF_SUBTRACT_ONE flag may not be specified at the ov_start() level, but only for ov_set*()"

        #endif

        _ov_display_flags("Flags for current indirect reference", _OVF_CURRENT)

        #define _OVF_STATUS 0 // Status information related to flags

        // Track relevant stati for flags...
        #define_eval _OVF_STATUS (_OVF_STATUS & (~_OVF_STATUS_USED | _OVF_STATUS_REUSED))
        #if (_OVF_CURRENT & OVF_REUSE_ON)
            #define_eval _OVF_STATUS (_OVF_STATUS | _OVF_STATUS_ALWAYS_REUSE)
        #else
            #define_eval _OVF_STATUS (_OVF_STATUS & (~_OVF_STATUS_ALWAYS_REUSE))
        #endif

        #define_eval _OVF_STATUS (_OVF_STATUS & (~_OVF_STATUS_IND_REF_PREVIOUSLY_WRITTEN))

        #define_eval _OVF_STATUS (_OVF_STATUS & (~_OVF_STATUS_IND_REF_RECENTLY_WRITTEN))

        #define_eval _OVF_STATUS (_OVF_STATUS & (~_OVF_STATUS_PREV_ALU_REG))
        #define_eval _OVF_STATUS (_OVF_STATUS & (~_OVF_STATUS_IND_CSR_REG))
        #define_eval _OVF_STATUS (_OVF_STATUS & (~_OVF_STATUS_PREV_ALU_NON_IMMED_CNST))
        #define_eval _OVF_STATUS (_OVF_STATUS & (~_OVF_STATUS_IND_CSR_NON_IMMED_CNST))

        #define_eval _OV_STORE_REGISTER_SET (_OV_STORE_REGISTER_SET + 1)
        // The above results in _OV_REG_ALU_DST and _OV_REG_IND_DST being assigned in _ov_declare_global()

        // Prepare registers
        _ov_declare_global()
        #define_eval _OV_REG_ALU_SRC _OV_REG_ALU_DST
        #define_eval _OV_REG_IND_SRC _OV_REG_IND_DST

        // Prepare working constants
        #define _OV_ALU_CONST     0
        #define _OV_IND_CSR_CONST 0

        // Record that registers have not yet been used, i.e. all values still in preprocessor defines (constants)
        #define_eval _OVF_STATUS (_OVF_STATUS & (~(_OVF_STATUS_ALU_SRC_USED | _OVF_STATUS_ALU_DST_USED)))
        #define_eval _OVF_STATUS (_OVF_STATUS & (~(_OVF_STATUS_IND_CSR_SRC_USED | _OVF_STATUS_IND_CSR_DST_USED)))

        #define _OV_ALU_MASK_PROVIDED     0
        #define _OV_IND_CSR_MASK_PROVIDED 0

        #define _OV_FINISHED 0

        #define _OV_MISSING_PARAMETERS // Used to gather list of missing parameters; shown if suitable combination NOT found

        // _OV_PAR_COUNT = Number of 1s in _OV_PAR_REQUESTED = number of requested parameters
        #define _OV_PAR_COUNT 0
        #define _OV_PAR_COUNT_TEMP _OV_PAR_REQUESTED
        #while (_OV_PAR_COUNT_TEMP != 0)

            #if ((_OV_PAR_COUNT_TEMP & (1 << _OV_MS_BIT_NUM)) != 0)
                #define_eval _OV_PAR_COUNT (_OV_PAR_COUNT + 1)
            #endif
            #define_eval _OV_PAR_COUNT_TEMP  (_OV_PAR_COUNT_TEMP << 1) // Must shift to the left, otherwise sign extension applied for negative numbers makes loop infinite if msb set
            #define_eval _OV_PAR_COUNT_TEMP  (_OV_PAR_COUNT_TEMP & _OV_32_BIT_MASK) // Required in 64-bit environment, otherwise loop endless

        #endloop
        #undef _OV_PAR_COUNT_TEMP

        _ov_offset_init()
        _ov_parameter_row_count_init()
        _ov_row_init()

        // For each provided parameter, set entry offset (_OV_OFFSET_PAR[]) to zero and count the
        // number of rows in _OV_TBL_ (_OV_PARAMETER_ROW_COUNT[]) where parameter present:
        #define _OV_PAR_COUNT_i                  0
        #define _OV_PAR_LSH         _OV_MS_BIT_NUM
        #define _OV_PAR_LSH_TEMP _OV_PAR_REQUESTED
        #while (_OV_PAR_COUNT_i < _OV_PAR_COUNT)

            //_ov_offset_write(_OV_PAR_COUNT_i, 0)
            // Unrolled version of above (to increase speed, reduce list file size)
            #define_eval _OV_LEN strlen('_OV_OFFSET_PAR')
            #define_eval _OV_LEFT strleft('_OV_OFFSET_PAR', (_OV_PAR_COUNT_i * 4))
            #if ((_OV_LEN - (_OV_PAR_COUNT_i * 4) - 4) > 0)
                #define_eval _OV_RIGHT strright('_OV_OFFSET_PAR', (_OV_LEN - (_OV_PAR_COUNT_i * 4) - 4))
            #else
                // strright() does not return an empty string when token2 is 0
                #define_eval _OV_RIGHT ''
            #endif
            #define_eval _OV_MIDDLE '0000' //strright('0000/**/value', 4) // Value is hardcoded as zero here
            #define_eval _OV_OFFSET_PAR '_OV_LEFT/**/_OV_MIDDLE/**/_OV_RIGHT'

            #define _OV_ROW_COUNT 0

            #while ((_OV_PAR_LSH_TEMP & (1 << _OV_MS_BIT_NUM)) == 0)
                #define_eval _OV_PAR_LSH (_OV_PAR_LSH - 1)
                #define_eval _OV_PAR_LSH_TEMP (_OV_PAR_LSH_TEMP << 1) // Must shift to the left, otherwise sign extension applied for negative numbers makes loop infinite if msb set
            #endloop

            #define _OV_ROW_i 0
            #define _OV_OFFSET 0
            #while (_OV_ROW_i < _OV_TBL_COUNT)

                //_ov_struct(_OV_ROW_i) // Fetch data for this row
                //_ov_va_read_OV_TBL_first(_OV_ROW_i) // Fetch only first value, because much cheaper
                // Unroll above for speed test...
                #define_eval _OV_VA_LIST '_OV_TBL_/**/_OV_ROW_i'
                #define_eval _OV_VA_NUM 0
                #define_eval _OV_VA_IDX_PREV (0)
                // Find the index of the next comma
                #define_eval _OV_VA_IDX strstr('_OV_VA_LIST', ",")
                #define_eval _OV_VA_ARG strright(strleft('_OV_VA_LIST', _OV_VA_IDX - 1), - _OV_VA_IDX_PREV)
                #define_eval _OV_VA_ARG (_OV_VA_ARG)
                #define_eval _OV_ROW_OVERRIDE (_OV_VA_ARG)
                #undef _OV_VA_LIST
                #undef _OV_VA_NUM
                #undef _OV_VA_IDX_PREV
                #undef _OV_VA_IDX

                #if ((1 << _OV_PAR_LSH) == _OV_ROW_OVERRIDE)

                    #define_eval _OV_ROW_COUNT (_OV_ROW_COUNT + 1)

                #endif

                #define_eval _OV_ROW_i (_OV_ROW_i + 1)

            #endloop

            #undef _OV_ROW_i
            #undef _OV_OFFSET

            _ov_parameter_row_count_write(_OV_PAR_COUNT_i, _OV_ROW_COUNT)

            #undef _OV_ROW_COUNT

            #define_eval _OV_PAR_LSH (_OV_PAR_LSH - 1)
            #define_eval _OV_PAR_LSH_TEMP (_OV_PAR_LSH_TEMP << 1) // Must shift to the left, otherwise sign extension applied for negative numbers makes loop infinite if msb set

            #define_eval _OV_PAR_COUNT_i (_OV_PAR_COUNT_i + 1)
        #endloop
        #undef _OV_PAR_LSH
        #undef _OV_PAR_LSH_TEMP
        #undef _OV_PAR_COUNT_i

        #define_eval _OV_VALID_SET 1

        // Search through possible combinations until finished (successfully or unsuccessfully)
        #while (!_OV_FINISHED)

            // The following are used to keep track if the combination of rows is valid
            #define_eval _OV_ALU_MASK_REQUIRED     0
            #define_eval _OV_IND_CSR_MASK_REQUIRED 0
            #define_eval _OV_CONF_MASK             0
            #define_eval _OV_CONF_VALUE            0
            #define_eval _OV_VALID_SET             1

            // Get row (offset from beginning by _OV_OFFSET_PAR[i]) for each provided parameter from _OV_REF_DATA: _OV_ROW_PAR[i]
            #define _OV_PAR_COUNT_i                  0
            #define _OV_PAR_LSH         _OV_MS_BIT_NUM
            #define _OV_PAR_LSH_TEMP _OV_PAR_REQUESTED
            #while ((_OV_PAR_COUNT_i < _OV_PAR_COUNT) && _OV_VALID_SET)

                #while ((_OV_PAR_LSH_TEMP & (1 << _OV_MS_BIT_NUM)) == 0)
                    #define_eval _OV_PAR_LSH (_OV_PAR_LSH - 1)
                    #define_eval _OV_PAR_LSH_TEMP (_OV_PAR_LSH_TEMP << 1) // Must shift to the left, otherwise sign extension applied for negative numbers makes loop infinite if msb set
                    #define_eval _OV_PAR_LSH_TEMP (_OV_PAR_LSH_TEMP & _OV_32_BIT_MASK) // Required in 64-bit environment, otherwise loop endless
                #endloop

                _ov_row_write(_OV_PAR_COUNT_i, _OV_TBL_COUNT) // Invalid value
                //// Unrolled version of above (to increase speed, reduce list file size)
                //#define_eval _OV_LEN strlen('_OV_ROW_PAR')
                //#define_eval _OV_LEFT strleft('_OV_ROW_PAR', (_OV_PAR_COUNT_i * 4))
                //#if ((_OV_LEN - (_OV_PAR_COUNT_i * 4) - 4) > 0)
                //    #define_eval _OV_RIGHT strright('_OV_ROW_PAR', (_OV_LEN - (_OV_PAR_COUNT_i * 4) - 4))
                //#else
                //    // strright() does not return an empty string when token2 is 0
                //    #define_eval _OV_RIGHT ''
                //#endif
                //#define_eval _OV_MIDDLE strright('0000/**/_OV_TBL_COUNT', 4)
                //#define_eval _OV_ROW_PAR '_OV_LEFT/**/_OV_MIDDLE/**/_OV_RIGHT'

                //#define_eval _OV_DESIRED_OFFSET (_ov_offset_read(_OV_PAR_COUNT_i)) // Offset is in "0000" format
                // Unrolled version of above (to increase speed, reduce list file size)
                #define_eval _OV_DESIRED_OFFSET (strleft(strright('_OV_OFFSET_PAR', -4 * (_OV_PAR_COUNT_i)), 4)) // Offset is in "0000" format

                #define _OV_FOUND 0
                #define _OV_ROW_i 0
                #define _OV_OFFSET 0
                #while ((_OV_ROW_i < _OV_TBL_COUNT) && (!_OV_FOUND))

                    //_ov_struct(_OV_ROW_i) // Fetch data for this row
                    //_ov_va_read_OV_TBL_first(_OV_ROW_i) // Fetch only first value, because much cheaper
                    // Unroll above for speed test...
                    #define_eval _OV_VA_LIST '_OV_TBL_/**/_OV_ROW_i'
                    #define_eval _OV_VA_NUM 0
                    #define_eval _OV_VA_IDX_PREV (0)
                    // Find the index of the next comma
                    #define_eval _OV_VA_IDX strstr('_OV_VA_LIST', ",")
                    #define_eval _OV_VA_ARG strright(strleft('_OV_VA_LIST', _OV_VA_IDX - 1), - _OV_VA_IDX_PREV)
                    #define_eval _OV_VA_ARG (_OV_VA_ARG)
                    #define_eval _OV_ROW_OVERRIDE (_OV_VA_ARG)
                    #undef _OV_VA_LIST
                    #undef _OV_VA_NUM
                    #undef _OV_VA_IDX_PREV
                    #undef _OV_VA_IDX

                    #if ((1 << _OV_PAR_LSH) == _OV_ROW_OVERRIDE)

                        // This row matches the override we're looking at
                        #if (_OV_OFFSET == _OV_DESIRED_OFFSET)

                            // This row is the offset we're interested in

                            _ov_row_write(_OV_PAR_COUNT_i, _OV_ROW_i)
                            //// Unrolled version of above (to increase speed, reduce list file size)
                            //#define_eval _OV_LEN strlen('_OV_ROW_PAR')
                            //#define_eval _OV_LEFT strleft('_OV_ROW_PAR', (_OV_PAR_COUNT_i * 4))
                            //#if ((_OV_LEN - (_OV_PAR_COUNT_i * 4) - 4) > 0)
                            //    #define_eval _OV_RIGHT strright('_OV_ROW_PAR', (_OV_LEN - (_OV_PAR_COUNT_i * 4) - 4))
                            //#else
                            //    // strright() does not return an empty string when token2 is 0
                            //    #define_eval _OV_RIGHT ''
                            //#endif
                            //#define_eval _OV_MIDDLE strright('0000/**/_OV_ROW_i', 4)
                            //#define_eval _OV_ROW_PAR '_OV_LEFT/**/_OV_MIDDLE/**/_OV_RIGHT'

                            #define_eval _OV_FOUND 1

                            //_ov_va_read_OV_TBL_all(_OV_ROW_i) // Read entire row now
                            // Unroll above for speed test...
                            #define_eval _OV_VA_LIST '_OV_TBL_/**/_OV_ROW_i'
                            #define_eval _OV_VA_NUM 0
                            #define_eval _OV_VA_IDX_PREV (0)
                            // While there is still a comma in the list...
                            #while (strstr('_OV_VA_LIST', ",") > 0)
                                // Find the index of the next comma
                                #define_eval _OV_VA_IDX strstr('_OV_VA_LIST', ",")
                                // Replace the comma with a colon in the working copy
                                #define_eval _OV_VA_LIST_LEFT strleft('_OV_VA_LIST', _OV_VA_IDX - 1)
                                #define_eval _OV_VA_LIST_RIGHT strright('_OV_VA_LIST', -_OV_VA_IDX)
                                #define_eval _OV_VA_LIST '_OV_VA_LIST_LEFT:_OV_VA_LIST_RIGHT'
                                #undef _OV_VA_LIST_LEFT
                                #undef _OV_VA_LIST_RIGHT
                                #define_eval _OV_VA_ARG strright(strleft('_OV_VA_LIST', _OV_VA_IDX - 1), - _OV_VA_IDX_PREV)
                                #define_eval _OV_VA_ARG (_OV_VA_ARG)
                                #if (_OV_VA_NUM == 0)
                                    #define_eval _OV_ROW_OVERRIDE (_OV_VA_ARG)
                                #elif (_OV_VA_NUM == 1)
                                    #define_eval _OV_ROW_DATA_REG (_OV_VA_ARG)
                                #elif (_OV_VA_NUM == 2)
                                    #define_eval _OV_ROW_DATA_MSbit (_OV_VA_ARG)
                                #elif (_OV_VA_NUM == 3)
                                    #define_eval _OV_ROW_DATA_LSbit (_OV_VA_ARG)
                                #elif (_OV_VA_NUM == 4)
                                    #define_eval _OV_ROW_CONF_MASK (_OV_VA_ARG)
                                #elif (_OV_VA_NUM == 5)
                                    // Final entry could have a trailing comma
                                    #define_eval _OV_ROW_CONF_VALUE (_OV_VA_ARG)
                                #endif
                                #define_eval _OV_VA_IDX_PREV (_OV_VA_IDX + 1)
                                #define_eval _OV_VA_NUM (_OV_VA_NUM + 1)
                            #endloop
                            #if (_OV_VA_NUM == _OV_TBL_COLUMNS)
                                // Final entry doesn't necessarily have a trailing comma
                                #define_eval _OV_VA_ARG strright('_OV_VA_LIST', - _OV_VA_IDX_PREV)
                                #define_eval _OV_ROW_CONF_VALUE (_OV_VA_ARG)
                            #endif
                            #undef _OV_VA_LIST
                            #undef _OV_VA_NUM
                            #undef _OV_VA_IDX_PREV
                            #undef _OV_VA_IDX
                            // ...end of _ov_va_read_OV_TBL_all()

                            // Incrementally calculate whether this combination is valid...
                            #define _OV_TEMP_DATA_MASK (((1 << (_OV_ROW_DATA_MSbit + 1)) - 1) - ((1 << _OV_ROW_DATA_LSbit) - 1))
                            #if (_OV_ROW_DATA_REG == 0)

                                // Previous ALU
                                #if ((_OV_ALU_MASK_REQUIRED & _OV_TEMP_DATA_MASK) != 0)

                                    // Data overlaps, so not valid set
                                    #define_eval _OV_VALID_SET 0

                                #endif

                                #define_eval _OV_ALU_MASK_REQUIRED (_OV_ALU_MASK_REQUIRED | _OV_TEMP_DATA_MASK)

                            #else

                                // Indirect CSR
                                #if ((_OV_IND_CSR_MASK_REQUIRED & _OV_TEMP_DATA_MASK) != 0)

                                    // Data overlaps, so not valid set
                                    #define_eval _OV_VALID_SET 0

                                #endif

                                #define_eval _OV_IND_CSR_MASK_REQUIRED (_OV_IND_CSR_MASK_REQUIRED | _OV_TEMP_DATA_MASK)

                            #endif
                            #undef _OV_TEMP_DATA_MASK

                            #if ((_OV_CONF_MASK & _OV_ROW_CONF_MASK & _OV_CONF_VALUE) != (_OV_CONF_MASK & _OV_ROW_CONF_MASK & _OV_ROW_CONF_VALUE))

                                // Overlapping portion of configuration data (masks) compared between
                                // incremental and desired value disagree
                                #define_eval _OV_VALID_SET 0

                            #endif

                            #define_eval _OV_CONF_MASK (_OV_CONF_MASK | _OV_ROW_CONF_MASK)
                            #define_eval _OV_CONF_VALUE (_OV_CONF_VALUE | _OV_ROW_CONF_VALUE)

                            #if ((_OV_ALU_MASK_REQUIRED & _OV_CONF_MASK) != 0)

                                // Data in previous ALU and Configuration bits overlap, so not valid set
                                #define_eval _OV_VALID_SET 0

                            #endif

                        #endif

                        #define_eval _OV_OFFSET (_OV_OFFSET + 1)

                    #endif

                    #define_eval _OV_ROW_i (_OV_ROW_i + 1)
                #endloop

                #undef _OV_OFFSET
                #undef _OV_DESIRED_OFFSET
                #undef _OV_ROW_i
                #undef _OV_FOUND

                #define_eval _OV_PAR_LSH (_OV_PAR_LSH - 1)
                #define_eval _OV_PAR_LSH_TEMP (_OV_PAR_LSH_TEMP << 1) // Must shift to the left, otherwise sign extension applied for negative numbers makes loop infinite if msb set

                #define_eval _OV_PAR_COUNT_i (_OV_PAR_COUNT_i + 1)
            #endloop // ((_OV_PAR_COUNT_i < _OV_PAR_COUNT) && _OV_VALID_SET)
            #undef _OV_PAR_COUNT_i

            #undef _OV_PAR_LSH_TEMP
            #undef _OV_PAR_LSH

            // At this point, a set of rows has been found, and _OV_VALID_SET will be true if entries did not overlap

            // Finished = (Data locations do not overlap) && (Configuration data is consistent) && (Full set of data to be provided)
            #if (_OV_VALID_SET)

                // We have to check that all parameters that match / overlap with the configuration
                // bit have been indicated as going to be provided

                #define _OV_MISSING_PARAMETERS_LOCAL // As nothing

                #define _OV_ROW_i 0
                #while (_OV_ROW_i < _OV_TBL_COUNT)

                    //_ov_struct(_OV_ROW_i) // Fetch data for this row
                    //_ov_va_read_OV_TBL_all(_OV_ROW_i) // Fetch data for this row
                    // Unroll above for speed test...
                    #define_eval _OV_VA_LIST '_OV_TBL_/**/_OV_ROW_i'
                    #define_eval _OV_VA_NUM 0
                    #define_eval _OV_VA_IDX_PREV (0)
                    // While there is still a comma in the list...
                    #while (strstr('_OV_VA_LIST', ",") > 0)
                        // Find the index of the next comma
                        #define_eval _OV_VA_IDX strstr('_OV_VA_LIST', ",")
                        // Replace the comma with a colon in the working copy
                        #define_eval _OV_VA_LIST_LEFT strleft('_OV_VA_LIST', _OV_VA_IDX - 1)
                        #define_eval _OV_VA_LIST_RIGHT strright('_OV_VA_LIST', -_OV_VA_IDX)
                        #define_eval _OV_VA_LIST '_OV_VA_LIST_LEFT:_OV_VA_LIST_RIGHT'
                        #undef _OV_VA_LIST_LEFT
                        #undef _OV_VA_LIST_RIGHT
                        #define_eval _OV_VA_ARG strright(strleft('_OV_VA_LIST', _OV_VA_IDX - 1), - _OV_VA_IDX_PREV)
                        #define_eval _OV_VA_ARG (_OV_VA_ARG)
                        #if (_OV_VA_NUM == 0)
                            #define_eval _OV_ROW_OVERRIDE (_OV_VA_ARG)
                        #elif (_OV_VA_NUM == 1)
                            #define_eval _OV_ROW_DATA_REG (_OV_VA_ARG)
                        #elif (_OV_VA_NUM == 2)
                            #define_eval _OV_ROW_DATA_MSbit (_OV_VA_ARG)
                        #elif (_OV_VA_NUM == 3)
                            #define_eval _OV_ROW_DATA_LSbit (_OV_VA_ARG)
                        #elif (_OV_VA_NUM == 4)
                            #define_eval _OV_ROW_CONF_MASK (_OV_VA_ARG)
                        #elif (_OV_VA_NUM == 5)
                            // Final entry could have a trailing comma
                            #define_eval _OV_ROW_CONF_VALUE (_OV_VA_ARG)
                        #endif
                        #define_eval _OV_VA_IDX_PREV (_OV_VA_IDX + 1)
                        #define_eval _OV_VA_NUM (_OV_VA_NUM + 1)
                    #endloop
                    #if (_OV_VA_NUM == _OV_TBL_COLUMNS)
                        // Final entry doesn't necessarily have a trailing comma
                        #define_eval _OV_VA_ARG strright('_OV_VA_LIST', - _OV_VA_IDX_PREV)
                        #define_eval _OV_ROW_CONF_VALUE (_OV_VA_ARG)
                    #endif
                    #undef _OV_VA_LIST
                    #undef _OV_VA_NUM
                    #undef _OV_VA_IDX_PREV
                    #undef _OV_VA_IDX
                    // ...end of _ov_va_read_OV_TBL_all()

                    #if (((_OV_CONF_MASK & _OV_ROW_CONF_MASK) != 0) &&              \
                         (_OV_CONF_MASK & _OV_ROW_CONF_MASK & _OV_CONF_VALUE) ==    \
                         (_OV_CONF_MASK & _OV_ROW_CONF_MASK & _OV_ROW_CONF_VALUE))

                        // Nonzero overlapping configuration data is the same, so the parameter of this row MUST be
                        // provided, but ONLY if NONE of the bits that it sets have not already been set by parameters
                        // of other names (constants in OV_... table)

                        #define _OV_COMPLETELY_SPECIFIED 1
                        #define _OV_TEMP_DATA_MASK (((1 << _OV_ROW_DATA_MSbit) - 1) - ((1 << _OV_ROW_DATA_LSbit) - 1))
                        #if (_OV_ROW_DATA_REG == 0)

                            // Previous ALU
                            #if ((_OV_ALU_MASK_REQUIRED & _OV_TEMP_DATA_MASK) == 0)

                                // Data does not overlap at all, so parameter is required
                                #define_eval _OV_COMPLETELY_SPECIFIED 0

                            #endif

                        #else

                            // Indirect CSR
                            #if ((_OV_IND_CSR_MASK_REQUIRED & _OV_TEMP_DATA_MASK) == 0)

                                // Data does not overlap at all, so parameter is required
                                #define_eval _OV_COMPLETELY_SPECIFIED 0

                            #endif

                        #endif
                        #undef _OV_TEMP_DATA_MASK

                        #if (!_OV_COMPLETELY_SPECIFIED)

                            // Now keep track of what parameter(s) being added could make set valid, to show
                            // user if no valid combination of parameters is found.
                            #if (!_OV_VALID_SET)

                                // First time, so prepend comma
                                #define_eval _OV_MISSING_PARAMETERS_LOCAL '_OV_MISSING_PARAMETERS_LOCAL, '

                            #endif

                            #define_eval OV_TEMP log2(_OV_ROW_OVERRIDE)
                            #define_eval _OV_MISSING_PARAMETERS_LOCAL '_OV_MISSING_PARAMETERS_LOCAL 1<</**/OV_TEMP'
                            #undef OV_TEMP

                            // Mark that the current combination of parameters is not valid.
                            #define_eval _OV_VALID_SET 0

                        #endif

                        #undef _OV_COMPLETELY_SPECIFIED

                    #endif

                    #define_eval _OV_ROW_i (_OV_ROW_i + 1)
                #endloop

                #if (strlen('_OV_MISSING_PARAMETERS_LOCAL') > 0)

                    #if (strlen('_OV_MISSING_PARAMETERS') > 0)

                        #define_eval _OV_MISSING_PARAMETERS '_OV_MISSING_PARAMETERS or '

                    #endif

                    #define_eval _OV_MISSING_PARAMETERS '_OV_MISSING_PARAMETERS _OV_MISSING_PARAMETERS_LOCAL'

                #endif

                // Following is useful for debugging, and indicates which rows in the table _OV_REF_DATA have been selected
                //#warning "Rows in _OV_ROW_PAR:" _OV_ROW_PAR
                //#warning "_OV_MISSING_PARAMETERS_LOCAL:" _OV_MISSING_PARAMETERS_LOCAL

                #undef _OV_MISSING_PARAMETERS_LOCAL

                #undef _OV_ROW_i

                #if (_OV_VALID_SET)

                    #define_eval _OV_FINISHED 1

                    // As we now have a finalised set of rows, store flags for later use
                    #define_eval _OV_ALU_CONST _OV_CONF_VALUE

                #endif

            #endif

            #if (_OV_FINISHED == 0)

                // Not yet finished, so calculate next set of offsets, taking number of rows into account...

                // Increment least significant offset counter
                //#define_eval OV_TEMP (_ov_offset_read(0))
                // Unrolled version of above (to increase speed, reduce list file size)
                #define_eval OV_TEMP (strleft(strright('_OV_OFFSET_PAR', -4 * (0)), 4))

                #define_eval OV_TEMP (OV_TEMP + 1)
                //_ov_offset_write(0, OV_TEMP)
                // Unrolled version of above (to increase speed, reduce list file size)
                #define_eval _OV_LEN strlen('_OV_OFFSET_PAR')
                #define_eval _OV_LEFT strleft('_OV_OFFSET_PAR', (0 * 4))
                #if ((_OV_LEN - (0 * 4) - 4) > 0)
                    #define_eval _OV_RIGHT strright('_OV_OFFSET_PAR', (_OV_LEN - (0 * 4) - 4))
                #else
                    // strright() does not return an empty string when token2 is 0
                    #define_eval _OV_RIGHT ''
                #endif
                #define_eval _OV_MIDDLE strright('0000/**/OV_TEMP', 4)
                #define_eval _OV_OFFSET_PAR '_OV_LEFT/**/_OV_MIDDLE/**/_OV_RIGHT'

                #undef OV_TEMP

                // Ripple through all offsets
                #define _OV_PAR_COUNT_i 0
                #define _OV_CARRY 0
                #while (_OV_PAR_COUNT_i < _OV_PAR_COUNT)

                    //#define_eval _OV_OFFSET_CURRENT (_ov_offset_read(_OV_PAR_COUNT_i))
                    // Unrolled version of above (to increase speed, reduce list file size)
                    #define_eval _OV_OFFSET_CURRENT (strleft(strright('_OV_OFFSET_PAR', -4 * (_OV_PAR_COUNT_i)), 4))

                    #define_eval _OV_OFFSET_MAXIMUM (_ov_parameter_row_count_read(_OV_PAR_COUNT_i))

                    #if ((_OV_OFFSET_CURRENT + _OV_CARRY) < _OV_OFFSET_MAXIMUM)

                        #if (_OV_CARRY != 0)

                            #define_eval _OV_OFFSET_CURRENT (_OV_OFFSET_CURRENT + _OV_CARRY)
                            //_ov_offset_write(_OV_PAR_COUNT_i, _OV_OFFSET_CURRENT)
                            // Unrolled version of above (to increase speed, reduce list file size)
                            #define_eval _OV_LEN strlen('_OV_OFFSET_PAR')
                            #define_eval _OV_LEFT strleft('_OV_OFFSET_PAR', (_OV_PAR_COUNT_i * 4))
                            #if ((_OV_LEN - (_OV_PAR_COUNT_i * 4) - 4) > 0)
                                #define_eval _OV_RIGHT strright('_OV_OFFSET_PAR', (_OV_LEN - (_OV_PAR_COUNT_i * 4) - 4))
                            #else
                                // strright() does not return an empty string when token2 is 0
                                #define_eval _OV_RIGHT ''
                            #endif
                            #define_eval _OV_MIDDLE strright('0000/**/_OV_OFFSET_CURRENT', 4)
                            #define_eval _OV_OFFSET_PAR '_OV_LEFT/**/_OV_MIDDLE/**/_OV_RIGHT'

                        #endif

                        // No carry, so we're done

                        #define_eval _OV_CARRY 0

                        #define_eval _OV_PAR_COUNT_i _OV_PAR_COUNT // Leave loop because finished rippling

                    #else

                        // New value does not fit in current column, so must ripple

                        //_ov_offset_write(_OV_PAR_COUNT_i, 0)
                        // Unrolled version of above (to increase speed, reduce list file size)
                        #define_eval _OV_LEN strlen('_OV_OFFSET_PAR')
                        #define_eval _OV_LEFT strleft('_OV_OFFSET_PAR', (_OV_PAR_COUNT_i * 4))
                        #if ((_OV_LEN - (_OV_PAR_COUNT_i * 4) - 4) > 0)
                            #define_eval _OV_RIGHT strright('_OV_OFFSET_PAR', (_OV_LEN - (_OV_PAR_COUNT_i * 4) - 4))
                        #else
                            // strright() does not return an empty string when token2 is 0
                            #define_eval _OV_RIGHT ''
                        #endif
                        #define_eval _OV_MIDDLE '0000' //strright('0000/**/value', 4) // Value is hardcoded as zero here
                        #define_eval _OV_OFFSET_PAR '_OV_LEFT/**/_OV_MIDDLE/**/_OV_RIGHT'

                        #define_eval _OV_CARRY 1 // Constant 1 is ok, because only ever increase LS offset by one

                    #endif

                    #undef _OV_OFFSET_CURRENT
                    #undef _OV_OFFSET_MAXIMUM

                    #define_eval _OV_PAR_COUNT_i (_OV_PAR_COUNT_i + 1)
                #endloop

                #if (_OV_CARRY > 0)

                    // Tried to carry beyond MS offset, so finished going through all combinations
                    #define_eval _OV_FINISHED 1

                #endif

                #undef _OV_CARRY
                #undef _OV_PAR_COUNT_i

            #endif

            #undef _OV_CONF_VALUE
            #undef _OV_CONF_MASK
            //#undef _OV_IND_CSR_MASK_REQUIRED // These are specifically left for future macro calls
            //#undef _OV_ALU_MASK_REQUIRED

        #endloop // #while (!_OV_FINISHED)

        #undef _OV_FINISHED

        #if (!_OV_VALID_SET)

            #if (strlen('_OV_MISSING_PARAMETERS') > 0)

                #error "Supplied combination of overrides not supported by" _OV_INDIRECT_FORMAT "indirect reference format; try adding parameters corresponding to the following constants:" _OV_MISSING_PARAMETERS

            #else

                #error "Supplied combination of overrides not supported by" _OV_INDIRECT_FORMAT "indirect reference format."

            #endif

        #else

            // Following is useful for debugging, and indicates which rows in the table _OV_REF_DATA have been selected
            //#warning "Rows in _OV_ROW_PAR:" _OV_ROW_PAR

        #endif

        #undef _OV_MISSING_PARAMETERS
        #undef _OV_VALID_SET

    #endif

.end
#endm // ov_start


/// @cond INTERNAL_MACROS
#macro ov_start(OVERRIDE_TYPES)

    ov_start(OVERRIDE_TYPES, 0)

#endm
/// @endcond


// Perform the function
//   alu[dst, a, alu_op, cnst]
// using the minimum number of instructions, where alu_op is one of B, ~B, AND, ~AND, AND~, OR
// If cnst is a compile-time constant, then this could be done in a single
// instruction as
//   alu[dst, a, alu_op, cnst8bit, <<shlft]
// if cnst can be expressed as (cnst8bit << shlft), where cnst8bit is an 8-bit value
// or as
//   alu[dst, a, alu_op, cnst8bit, >>rotrtrht]
// if cnst can be expressed as (cnst8bit rotated right rtrht bits), where cnst8bit is an 8-bit value
// Otherwise, the output is
//   move(temp, cnst) // 1 (16-bit constant) or 2 (32-bit constant) instructions
//   alu[dst, a, alu_op, temp]
// If TEST == _OV_ALU_SHF_REG_CONST_TEST, then no instructions are output, but
// symbol _OV_ALU_SHF_REG_CONST_INSTRUCTIONS is defined as maximum number of instructions that would
// be required to perform the operation.
/// @cond INTERNAL_MACROS
#define _OV_ALU_SHF_REG_CONST_TEST (0x41 << 24)
#macro _ov_alu_shf_reg_const(out_dst, in_a, alu_op, cnst, TEST)
.begin

    #if (is_ct_const(cnst))

        // Try shifting an 8-bit constant to the left
        #ifdef _OV_SHIFTCOUNT
            #error "_OV_SHIFTCOUNT is already defined"
        #endif

        #ifdef _OV_TESTVALUE
            #error "_OV_TESTVALUE is already defined"
        #endif

        #ifdef _OV_SHIFTOPTIMALWEIGHT
            #error "_OV_SHIFTOPTIMALWEIGHT is already defined"
        #endif

        #define _OV_SHIFTOPTIMALWEIGHT 0

        #define _OV_SHIFTCOUNT 0
        #define_eval _OV_TESTVALUE (cnst)
        #while (_OV_SHIFTCOUNT <= 24)

            #if (((_OV_TESTVALUE & (~MAX_IMMEDIATE)) == 0) && \
                 ((_OV_TESTVALUE << _OV_SHIFTCOUNT) == cnst)) // set bits not lost

                #define_eval _SHIFTCURRENTWEIGHT 1
                #if ((_OV_SHIFTCOUNT % 8) == 0)

                    #define_eval _SHIFTCURRENTWEIGHT 8

                #elif ((_OV_SHIFTCOUNT % 4) == 0)

                    #define_eval _SHIFTCURRENTWEIGHT 4

                #elif ((_OV_SHIFTCOUNT % 2) == 0)

                    #define_eval _SHIFTCURRENTWEIGHT 2

                #endif

                #if (_OV_SHIFTOPTIMALWEIGHT < _SHIFTCURRENTWEIGHT)

                    // Value is better aligned to a byte / nibble / 2-bits than previously, so use it
                    #define_eval _OV_SHIFTOPTIMALWEIGHT _SHIFTCURRENTWEIGHT
                    #define_eval _SHIFTOPTIMALCOUNT _OV_SHIFTCOUNT
                    #define_eval _SHIFTOPTIMALVALUE _OV_TESTVALUE

                #endif

            #endif

            #define_eval _OV_SHIFTCOUNT (_OV_SHIFTCOUNT + 1)

            #define_eval _OV_TESTVALUE (_OV_TESTVALUE >> 1)
            #define_eval _OV_TESTVALUE (_OV_TESTVALUE & ((1 << 31) - 1)) // Ensure sign extension does NOT take place

            // Note: Code is written like this because ((1 << (32 - 0)) - 1) evaluates to 0x0 in 32-bit
            // preprocessor environment, not expected 0xFFFFFFFF. Perhaps 64-bit environment for calculations
            // and then answer later truncated?
            // Also, in 32-bit environment, most significant bit is sign-extended when shifting to the right

        #endloop

        #if (_OV_SHIFTOPTIMALWEIGHT > 0)

            // A valid 8-bit constant and shift value was found, so use it

            #if (TEST == _OV_ALU_SHF_REG_CONST_TEST)

                #define_eval _OV_ALU_SHF_REG_CONST_INSTRUCTIONS 1

            #else

                #if (_SHIFTOPTIMALCOUNT == 0)

                    // Cleaner result if _SHIFTOPTIMALCOUNT is zero
                    alu[out_dst, in_a, alu_op, _SHIFTOPTIMALVALUE]

                #else

                    alu[out_dst, in_a, alu_op, _SHIFTOPTIMALVALUE, <</**/_SHIFTOPTIMALCOUNT]

                #endif

            #endif

            #undef _SHIFTOPTIMALCOUNT
            #undef _SHIFTOPTIMALVALUE

        #else

            // Try rotating an 8-bit constant to the right
            #define_eval _OV_SHIFTOPTIMALWEIGHT 0

            #define_eval _OV_SHIFTCOUNT 0
            #define_eval _OV_TESTVALUE (cnst)
            #while (_OV_SHIFTCOUNT < 32)

                #if ((_OV_TESTVALUE & (~MAX_IMMEDIATE)) == 0)

                    #define_eval _SHIFTCURRENTWEIGHT 1
                    #if ((_OV_SHIFTCOUNT % 8) == 0)

                        #define_eval _SHIFTCURRENTWEIGHT 8

                    #elif ((_OV_SHIFTCOUNT % 4) == 0)

                        #define_eval _SHIFTCURRENTWEIGHT 4

                    #elif ((_OV_SHIFTCOUNT % 2) == 0)

                        #define_eval _SHIFTCURRENTWEIGHT 2

                    #endif

                    #if (_OV_SHIFTOPTIMALWEIGHT < _SHIFTCURRENTWEIGHT)

                        // Value is better aligned to a byte / nibble / 2-bits than previously, so use it
                        #define_eval _OV_SHIFTOPTIMALWEIGHT _SHIFTCURRENTWEIGHT
                        #define_eval _SHIFTOPTIMALCOUNT _OV_SHIFTCOUNT
                        #define_eval _SHIFTOPTIMALVALUE _OV_TESTVALUE

                    #endif

                #endif

                #if (_OV_TESTVALUE & (1 << 31))

                    #if (((1 << 32) - 1) == 0)
                        #define_eval _OV_TESTVALUE (((_OV_TESTVALUE << 1)) | 1) // 32-bit space
                    #else
                        #define_eval _OV_TESTVALUE (((_OV_TESTVALUE << 1) & ((1 << 32) - 1)) | 1) // Trim to 32-bit space
                    #endif

                #else

                    #if (((1 << 32) - 1) == 0)
                        #define_eval _OV_TESTVALUE (((_OV_TESTVALUE << 1)) | 0) // 32-bit space
                    #else
                        #define_eval _OV_TESTVALUE (((_OV_TESTVALUE << 1) & ((1 << 32) - 1)) | 0) // Trim to 32-bit space
                    #endif

                #endif

                #define_eval _OV_SHIFTCOUNT (_OV_SHIFTCOUNT + 1)

            #endloop

            #if (_OV_SHIFTOPTIMALWEIGHT > 0)

                #if (TEST == _OV_ALU_SHF_REG_CONST_TEST)

                    #define_eval _OV_ALU_SHF_REG_CONST_INSTRUCTIONS 1

                #else

                    alu[out_dst, in_a, alu_op, _SHIFTOPTIMALVALUE, >>rot/**/_SHIFTOPTIMALCOUNT]

                #endif

                #undef _SHIFTOPTIMALCOUNT
                #undef _SHIFTOPTIMALVALUE

            #else

                // No other plan to optimise, so brute force...

                #if (TEST == _OV_ALU_SHF_REG_CONST_TEST)

                    #define_eval _OV_ALU_SHF_REG_CONST_INSTRUCTIONS 2

                #else

                    // Note: move() already implements the option of using a single immed[] instruction
                    // if possible to align a 16-bit constant to a byte boundary.

                    #ifdef _ov_dst
                        #error "_ov_dst has already been defined!"
                    #endif

                    #if (streq('out_dst', '--'))

                        // Have no destination, so create a temporary one to give to move()
                        .reg tempreg
                        #define _ov_dst tempreg

                    #else

                        // Have a register destination, so use it directly
                        #define_eval _ov_dst out_dst

                    #endif

                    #if (streq('alu_op', 'B'))

                        // Just want the second parameter, so a move will work
                        move(_ov_dst, cnst) // 1 (16-bit constant) or 2 (32-bit constant) instructions

                    #else

                        // Must do full-blown temporary register
                        .reg _ov_tempcnst
                        move(_ov_tempcnst, cnst) // 1 (16-bit constant) or 2 (32-bit constant) instructions
                        alu[_ov_dst, in_a, alu_op, _ov_tempcnst]

                    #endif

                    #undef _ov_dst

                #endif

            #endif

        #endif

        #undef _OV_SHIFTCOUNT
        #undef _OV_TESTVALUE
        #undef _OV_SHIFTOPTIMALWEIGHT

    #else

        // "cnst" is not a compile time constant

        #if (streq('alu_op', 'B'))

            // Just want the second parameter, so a move will work

            #if (TEST == _OV_ALU_SHF_REG_CONST_TEST)

                #define_eval _OV_ALU_SHF_REG_CONST_INSTRUCTIONS 2

            #else

                move(out_dst, cnst) // 1 (16-bit constant) or 2 (32-bit constant) instructions

            #endif

        #else

            // Must do full-blown temporary register

            #if (TEST == _OV_ALU_SHF_REG_CONST_TEST)

                #define_eval _OV_ALU_SHF_REG_CONST_INSTRUCTIONS 3

            #else

                .reg ov_temp
                move(ov_temp, cnst) // 1 (16-bit constant) or 2 (32-bit constant) instructions
                alu[out_dst, in_a, alu_op, ov_temp]

            #endif

        #endif

    #endif

.end
#endm
/// @endcond


/// @cond INTERNAL_MACROS
#macro _ov_alu_shf_reg_const(out_dst, in_a, alu_op, cnst)

    _ov_alu_shf_reg_const(out_dst, in_a, alu_op, cnst, 0)

#endm
/// @endcond


/// @cond INTERNAL_MACROS
#macro _ov_set(OVERRIDE_TYPE, in_value, FINAL, DST_MSB, DST_LSB, SRC_MSB, SRC_LSB, FLAGS, BYTE_MASK_PROVIDED, in_byte_mask, SET_BITS, BIT_MASK)
.begin

    #ifndef _OV_PAR_REQUESTED

        #error "ov_start() must be called before ov_set() may be used"

    #endif

    #if (OVERRIDE_TYPE == 0)

        #error "OVERRIDE_TYPE not specified"

    #elif ((OVERRIDE_TYPE & _OV_PAR_REQUESTED) == 0)

        #error "Specified OVERRIDE_TYPE was not specified at ov_start()"

    #else // OVERRIDE_TYPE contains an expected value

        #ifdef _OVF_LOCAL

            #error "_OVF_LOCAL has already been defined!"

        #endif

        #if (((FLAGS & _OV_FLAG_BITS(_OVF_NO_SWAP_SINCE_USE_pair)) != 0))

            #error "Flag OVF_NO_SWAP_SINCE_USE may be not be specified for ov_set*()"

        #endif

        #define_eval _OVF_LOCAL _OVF_CURRENT // Flags currently applicable

        // Clear global flags where local flags are enabled
        #define_eval _OVF_LOCAL (_OVF_LOCAL & (~((FLAGS & _OVF_MASK_ENABLED) | ((FLAGS & _OVF_MASK_ENABLED) >> 1))))

        // Clear global flags where local flags are disabled
        #define_eval _OVF_LOCAL (_OVF_LOCAL & (~((FLAGS & _OVF_MASK_DISABLED) | ((FLAGS & _OVF_MASK_DISABLED) << 1))))

        // Add local flags
        #define_eval _OVF_LOCAL (_OVF_LOCAL | FLAGS)

        // If a flag is not set, must set to off, so that a flag is deterministically on or off
        #define_eval _OVF_LOCAL (_OVF_LOCAL | ((~_OVF_LOCAL & _OVF_MASK_ENABLED) >> 1))

        _ov_display_flags("Flags for current call of _ov_set()", _OVF_LOCAL)

        // Check that still reusing...
        #if ((_OVF_LOCAL & OVF_REUSE_ON) == 0)

            #define_eval _OVF_STATUS (_OVF_STATUS & (~_OVF_STATUS_ALWAYS_REUSE))

        #endif

        // Check that context swap is still not happening
        #if (_OVF_LOCAL & OVF_NO_CTX_SWAP_OFF)

            // Context swap is no longer guaranteed to not happen, so forget about recently writing CMD_INDIRECT_REF_0
            #define_eval _OVF_STATUS (_OVF_STATUS & (~_OVF_STATUS_IND_REF_RECENTLY_WRITTEN))

        #endif

        #if (BYTE_MASK_PROVIDED)

            #if (!((defined(__NFP6000) && (IS_IXPTYPE(__NFP6000) || IS_NFPTYPE(__NFP3800))) && defined(__NFP_INDIRECT_REF_FORMAT_V2)))

                #error "ov_set_bm*() is only applicable to Thornham NFP6000 indirect references."

            #endif

        #endif

        #if defined(_ov_value)

            #error "_ov_value is already defined"

        #endif

        // See whether in_value is a binary constant

        _ov_0b_convert(in_value)

        #if (_OV_0b_IS_BINARY)

            #define_eval _ov_value _OV_0b_VALUE // _eval needed for deep copy

        #else

            #define_eval _ov_value in_value

        #endif

        #if (!SET_BITS)

            // Check that DST parameters are defaults or compile-time constants
            #if ((!streq('DST_MSB', '--') && (!streq('DST_LSB', '--')))) // Must first do string comparision, otherwise fails is_ct_const()

                #if ((!is_ct_const(DST_MSB) && (!is_ct_const(DST_LSB))))

                    #error "Both DST_MSB and DST_LSB must be compile-time constants or '--' for default value"

                #endif

            #endif

            // Check that SRC parameters are defaults or compile-time constants
            #if ((!streq('SRC_MSB', '--') && (!streq('SRC_LSB', '--')))) // Must first do string comparision, otherwise fails is_ct_const()

                #if ((!is_ct_const(SRC_MSB) && (!is_ct_const(SRC_LSB))))

                    #error "Both SRC_MSB and SRC_LSB must be compile-time constants or '--' for default value"

                #endif

            #endif

            // Get details related to byte mask, to check that is possible and advantageous to simultaneously set
            // byte mask and another field

            #define _OV_FOUND 0
            #define _OV_PAR_i 0
            #while ((_OV_PAR_i < _OV_PAR_COUNT) && (!_OV_FOUND))

                //#define_eval _OV_ROW_i (_ov_row_read(_OV_PAR_i)) // _OV_ROW_i is in "0000" format
                // Unrolled version of above (to increase speed, reduce list file size)
                #define_eval _OV_ROW_i (strleft(strright('_OV_ROW_PAR', -4 * (_OV_PAR_i)), 4)) // _OV_ROW_i is in "0000" format

                #define_eval _OV_ROW_i (_OV_ROW_i) // _OV_ROW_i is in "0" format, required for following table lookup

                //_ov_struct(_OV_ROW_i) // Fetch data for this row
                //_ov_va_read_OV_TBL_first(_OV_ROW_i) // Fetch only first value, because much cheaper
                // Unroll above for speed test...
                #define_eval _OV_VA_LIST '_OV_TBL_/**/_OV_ROW_i'
                #define_eval _OV_VA_NUM 0
                #define_eval _OV_VA_IDX_PREV (0)
                // Find the index of the next comma
                #define_eval _OV_VA_IDX strstr('_OV_VA_LIST', ",")
                #define_eval _OV_VA_ARG strright(strleft('_OV_VA_LIST', _OV_VA_IDX - 1), - _OV_VA_IDX_PREV)
                #define_eval _OV_VA_ARG (_OV_VA_ARG)
                #define_eval _OV_BM_OVERRIDE (_OV_VA_ARG)
                #undef _OV_VA_LIST
                #undef _OV_VA_NUM
                #undef _OV_VA_IDX_PREV
                #undef _OV_VA_IDX

                #if (OV_BYTE_MASK == _OV_BM_OVERRIDE) // Changed from generic version

                    //_ov_va_read_OV_TBL_all(_OV_ROW_i) // Fetch data for this row
                    // Unroll above for speed test...
                    #define_eval _OV_VA_LIST '_OV_TBL_/**/_OV_ROW_i'
                    #define_eval _OV_VA_NUM 0
                    #define_eval _OV_VA_IDX_PREV (0)
                    // While there is still a comma in the list...
                    #while (strstr('_OV_VA_LIST', ",") > 0)
                        // Find the index of the next comma
                        #define_eval _OV_VA_IDX strstr('_OV_VA_LIST', ",")
                        // Replace the comma with a colon in the working copy
                        #define_eval _OV_VA_LIST_LEFT strleft('_OV_VA_LIST', _OV_VA_IDX - 1)
                        #define_eval _OV_VA_LIST_RIGHT strright('_OV_VA_LIST', -_OV_VA_IDX)
                        #define_eval _OV_VA_LIST '_OV_VA_LIST_LEFT:_OV_VA_LIST_RIGHT'
                        #undef _OV_VA_LIST_LEFT
                        #undef _OV_VA_LIST_RIGHT
                        #define_eval _OV_VA_ARG strright(strleft('_OV_VA_LIST', _OV_VA_IDX - 1), - _OV_VA_IDX_PREV)
                        #define_eval _OV_VA_ARG (_OV_VA_ARG)
                        #if (_OV_VA_NUM == 0)
                            #define_eval _OV_BM_OVERRIDE (_OV_VA_ARG)
                        #elif (_OV_VA_NUM == 1)
                            #define_eval _OV_BM_DATA_REG (_OV_VA_ARG)
                        #elif (_OV_VA_NUM == 2)
                            #define_eval _OV_BM_DATA_MSbit (_OV_VA_ARG)
                        #elif (_OV_VA_NUM == 3)
                            #define_eval _OV_BM_DATA_LSbit (_OV_VA_ARG)
                        #elif (_OV_VA_NUM == 4)
                            #define_eval _OV_BM_CONF_MASK (_OV_VA_ARG)
                        #elif (_OV_VA_NUM == 5)
                            // Final entry could have a trailing comma
                            #define_eval _OV_BM_CONF_VALUE (_OV_VA_ARG)
                        #endif
                        #define_eval _OV_VA_IDX_PREV (_OV_VA_IDX + 1)
                        #define_eval _OV_VA_NUM (_OV_VA_NUM + 1)
                    #endloop
                    #if (_OV_VA_NUM == _OV_TBL_COLUMNS)
                        // Final entry doesn't necessarily have a trailing comma
                        #define_eval _OV_VA_ARG strright('_OV_VA_LIST', - _OV_VA_IDX_PREV)
                        #define_eval _OV_BM_CONF_VALUE (_OV_VA_ARG)
                    #endif
                    #undef _OV_VA_LIST
                    #undef _OV_VA_NUM
                    #undef _OV_VA_IDX_PREV
                    #undef _OV_VA_IDX
                    // ...end of _ov_va_read_OV_TBL_all()

                    #define_eval _OV_FOUND 1

                #endif

                #undef _OV_ROW_i

                #define_eval _OV_PAR_i (_OV_PAR_i + 1)
            #endloop
            #undef _OV_PAR_i

            #if (BYTE_MASK_PROVIDED && !_OV_FOUND)

                #error "Byte mask is not one of the fields that was specified in the call to ov_start(), so cannot use set_bm*()."

            #endif

            #undef _OV_FOUND

            // At this point, _OV_BM_DATA_REG, _OV_BM_DATA_MSbit, _OV_BM_DATA_LSbit, _OV_BM_CONF_MASK
            // and _OV_BM_CONF_VALUE have been populated for the parameter type byte mask from the set of
            // rows found during ov_start()

            #if (BYTE_MASK_PROVIDED && (!((_OV_BM_DATA_REG == 1) && (_OV_BM_DATA_LSbit == 0))))

                #error "For the fields specified in the call to ov_start(), byte mask is not located at bit 0 of CMD_INDIRECT_REF_0, so cannot use set_bm*()."

            #endif

        #endif

        // Fetch the row for this parameter out of the set of rows indicated by _OV_ROW_PAR
        #define _OV_FOUND 0
        #define _OV_PAR_i 0
        #while ((_OV_PAR_i < _OV_PAR_COUNT) && (!_OV_FOUND))

            //#define_eval _OV_ROW_i (_ov_row_read(_OV_PAR_i)) // _OV_ROW_i is in "0000" format
            // Unrolled version of above (to increase speed, reduce list file size)
            #define_eval _OV_ROW_i (strleft(strright('_OV_ROW_PAR', -4 * (_OV_PAR_i)), 4)) // _OV_ROW_i is in "0000" format

            #define_eval _OV_ROW_i (_OV_ROW_i) // _OV_ROW_i is in "0" format, required for following table lookup

            //_ov_struct(_OV_ROW_i) // Fetch data for this row
            //_ov_va_read_OV_TBL_first(_OV_ROW_i) // Fetch only first value, because much cheaper
            // Unroll above for speed test...
            #define_eval _OV_VA_LIST '_OV_TBL_/**/_OV_ROW_i'
            #define_eval _OV_VA_NUM 0
            #define_eval _OV_VA_IDX_PREV (0)
            // Find the index of the next comma
            #define_eval _OV_VA_IDX strstr('_OV_VA_LIST', ",")
            #define_eval _OV_VA_ARG strright(strleft('_OV_VA_LIST', _OV_VA_IDX - 1), - _OV_VA_IDX_PREV)
            #define_eval _OV_VA_ARG (_OV_VA_ARG)
            #define_eval _OV_ROW_OVERRIDE (_OV_VA_ARG)
            #undef _OV_VA_LIST
            #undef _OV_VA_NUM
            #undef _OV_VA_IDX_PREV
            #undef _OV_VA_IDX

            #if (OVERRIDE_TYPE == _OV_ROW_OVERRIDE)

                //_ov_va_read_OV_TBL_all(_OV_ROW_i) // Fetch data for this row
                // Unroll above for speed test...
                #define_eval _OV_VA_LIST '_OV_TBL_/**/_OV_ROW_i'
                #define_eval _OV_VA_NUM 0
                #define_eval _OV_VA_IDX_PREV (0)
                // While there is still a comma in the list...
                #while (strstr('_OV_VA_LIST', ",") > 0)
                    // Find the index of the next comma
                    #define_eval _OV_VA_IDX strstr('_OV_VA_LIST', ",")
                    // Replace the comma with a colon in the working copy
                    #define_eval _OV_VA_LIST_LEFT strleft('_OV_VA_LIST', _OV_VA_IDX - 1)
                    #define_eval _OV_VA_LIST_RIGHT strright('_OV_VA_LIST', -_OV_VA_IDX)
                    #define_eval _OV_VA_LIST '_OV_VA_LIST_LEFT:_OV_VA_LIST_RIGHT'
                    #undef _OV_VA_LIST_LEFT
                    #undef _OV_VA_LIST_RIGHT
                    #define_eval _OV_VA_ARG strright(strleft('_OV_VA_LIST', _OV_VA_IDX - 1), - _OV_VA_IDX_PREV)
                    #define_eval _OV_VA_ARG (_OV_VA_ARG)
                    #if (_OV_VA_NUM == 0)
                        #define_eval _OV_ROW_OVERRIDE (_OV_VA_ARG)
                    #elif (_OV_VA_NUM == 1)
                        #define_eval _OV_ROW_DATA_REG (_OV_VA_ARG)
                    #elif (_OV_VA_NUM == 2)
                        #define_eval _OV_ROW_DATA_MSbit (_OV_VA_ARG)
                    #elif (_OV_VA_NUM == 3)
                        #define_eval _OV_ROW_DATA_LSbit (_OV_VA_ARG)
                    #elif (_OV_VA_NUM == 4)
                        #define_eval _OV_ROW_CONF_MASK (_OV_VA_ARG)
                    #elif (_OV_VA_NUM == 5)
                        // Final entry could have a trailing comma
                        #define_eval _OV_ROW_CONF_VALUE (_OV_VA_ARG)
                    #endif
                    #define_eval _OV_VA_IDX_PREV (_OV_VA_IDX + 1)
                    #define_eval _OV_VA_NUM (_OV_VA_NUM + 1)
                #endloop
                #if (_OV_VA_NUM == _OV_TBL_COLUMNS)
                    // Final entry doesn't necessarily have a trailing comma
                    #define_eval _OV_VA_ARG strright('_OV_VA_LIST', - _OV_VA_IDX_PREV)
                    #define_eval _OV_ROW_CONF_VALUE (_OV_VA_ARG)
                #endif
                #undef _OV_VA_LIST
                #undef _OV_VA_NUM
                #undef _OV_VA_IDX_PREV
                #undef _OV_VA_IDX
                // ...end of _ov_va_read_OV_TBL_all()

                #define_eval _OV_FOUND 1

            #endif

            #define_eval _OV_PAR_i (_OV_PAR_i + 1)
        #endloop
        #undef _OV_PAR_i

        #if (!_OV_FOUND)

            // This should NEVER happen because ov_start() found the data we've just looked for, unless
            // multiples types were specified...
            #error "Only one OVERRIDE_TYPE may be specified at a time in ov_set()"

        #endif

        #undef _OV_FOUND

        // At this point, _OV_ROW_DATA_REG, _OV_ROW_DATA_MSbit, _OV_ROW_DATA_LSbit, _OV_ROW_CONF_MASK
        // and _OV_ROW_CONF_VALUE have been populated for the provided parameter type from the set of
        // rows found during ov_start()

        #if (SET_BITS)

            #if defined(_ov_bit_mask)

                #error "_ov_bit_mask is already defined"

            #endif

            // We're setting a range of bits, so see if we have a bit range (DST_MSB:DST_LSB) or a non-zero bit mask

            #if ((streq('DST_MSB', '--') && (streq('DST_LSB', '--')))) // Must first do string comparision, otherwise fails is_ct_const()

                // See whether BIT_MASK is a binary constant

                _ov_0b_convert(BIT_MASK)

                #if (_OV_0b_IS_BINARY)

                    #define _ov_bit_mask _OV_0b_VALUE

                #else

                    #define_eval _ov_bit_mask BIT_MASK

                #endif

                #if (!is_ct_const(_ov_bit_mask))

                    #error "BIT_MASK must be a compile-time constant!"

                #endif

            #else

                #if ((!is_ct_const(DST_MSB) && (!is_ct_const(DST_LSB))))

                    #error "Both DST_MSB and DST_LSB must be compile-time constants, or both must be '--' if a BIT_MASK is provided."

                #else

                    // Both are compile time constants

                    // Check for validity
                    #if (!(((DST_MSB >= 0) && (DST_MSB <= (_OV_ROW_DATA_MSbit - _OV_ROW_DATA_LSbit))) ))

                        #define_eval OV_TEMP (_OV_ROW_DATA_MSbit - _OV_ROW_DATA_LSbit)
                        #error "MSB must be in the range 0.."/**/OV_TEMP

                    #endif

                    #if (!(((DST_LSB >= 0) && (DST_LSB <= DST_MSB)) ))

                        #error "DST_LSB must be in the range 0..DST_MSB = 0.."/**/DST_MSB

                    #endif

                    #define_eval _ov_bit_mask (((1 << (DST_MSB + 1)) - 1) - ((1 << DST_LSB) - 1))

                #endif

                #define_eval _ov_value (_ov_value << DST_LSB)

            #endif

            #if (!is_ct_const(_ov_value))

                #error "BIT_CONSTANT must be a compile-time constant!"

            #endif

            #define_eval _ov_src_lsb 0 // DST_LSB has already been incorporated into value and mask

            #define_eval _OV_DESTINATION_MASK (_ov_bit_mask << _OV_ROW_DATA_LSbit)

            #define_eval _OV_FIELD_MASK (((1 << (_OV_ROW_DATA_MSbit + 1)) - 1) - ((1 << _OV_ROW_DATA_LSbit) - 1))

            #if (((_OV_DESTINATION_MASK) & (~_OV_FIELD_MASK)) != 0)

                _ov_hex(_OV_DESTINATION_MASK >> _OV_ROW_DATA_LSbit)
                #define_eval OV_TEMP '_OV_HEX_NUMBER'
                _ov_hex(_OV_FIELD_MASK >> _OV_ROW_DATA_LSbit)

                #error "The provided mask ("OV_TEMP") is not a subset of the mask of the field being set ("_OV_HEX_NUMBER")"

            #endif

            #if (_OV_DESTINATION_MASK == 0)

                #warning "ov_set_bits() has no effect if the mask is empty!"

            #endif

            // We're using the defaults from the table
            #define_eval _ov_dst_msb _OV_ROW_DATA_MSbit
            #define_eval _ov_dst_lsb _OV_ROW_DATA_LSbit

            #define_eval _OV_SOURCE_MASK_ZERO (_ov_bit_mask)

        #else

            #if (streq('DST_MSB', '--') && streq('DST_LSB', '--'))

                // We're using the defaults from the table
                #define_eval _ov_dst_msb _OV_ROW_DATA_MSbit
                #define_eval _ov_dst_lsb _OV_ROW_DATA_LSbit

                #define_eval _ov_src_lsb 0

            #else

                // We're using the values provided
                #define_eval _ov_dst_msb (_OV_ROW_DATA_LSbit + DST_MSB) // Translate to relative to register
                #define_eval _ov_dst_lsb (_OV_ROW_DATA_LSbit + DST_LSB)

                // Check for validity
                #if (!(((_ov_dst_msb >= _OV_ROW_DATA_LSbit) && (_ov_dst_msb <= _OV_ROW_DATA_MSbit)) ))

                    #define_eval OV_TEMP (_OV_ROW_DATA_MSbit - _OV_ROW_DATA_LSbit)
                    #error "DST_MSB must be in the range 0.."/**/OV_TEMP

                #endif

                #if (!(((_ov_dst_lsb >= _OV_ROW_DATA_LSbit) && (_ov_dst_lsb <= _ov_dst_msb)) ))

                    #define_eval OV_TEMP (_ov_dst_msb - _OV_ROW_DATA_LSbit)
                    #error "DST_LSB must be in the range 0..DST_MSB = 0.."/**/OV_TEMP

                #endif

            #endif

            #define _OV_MASK_SOURCE 0

            #define _OV_SOURCE_MASK_ZERO 0xFFFFFFFF // Default mask, which has no effect, to be used on constant values
                // with no penalty. Zero indicates mask is aligned to bit zero.

            #if (streq('SRC_MSB', '--') && streq('SRC_LSB', '--'))

                // Use the indicated source directly, without masking
                #define_eval _ov_src_msb (_ov_dst_msb - _ov_dst_lsb)
                #define_eval _ov_src_lsb 0

            #else

                // Mask the source to the bit range specified by SRC_MSB:SRC_LSB

                #define_eval _ov_src_msb SRC_MSB
                #define_eval _ov_src_lsb SRC_LSB

                #if ((SRC_MSB - SRC_LSB) > (_ov_dst_msb - _ov_dst_lsb))

                    #error "Source field width is greater than destination field width."

                #endif

                #if (is_ct_const(_ov_value))

                    // Only if compile time constant (not run time) can we perform the calculations now
                    #define_eval _ov_value ((_ov_value >> _ov_src_lsb) & _OV_SOURCE_MASK_ZERO)

                #else

                    // Defer masking until we have the value in a register
                    #define_eval _OV_MASK_SOURCE 1

                    #define_eval _OV_SOURCE_MASK_ZERO (((1 << (_ov_src_msb - _ov_src_lsb + 1)) - 1))

                #endif

            #endif

            #define_eval _OV_DESTINATION_MASK (((1 << (_ov_dst_msb + 1)) - 1) - ((1 << _ov_dst_lsb) - 1))

            #if ((BYTE_MASK_PROVIDED) && (_OV_ROW_DATA_REG != 1))

                // We have been given an additional byte mask, but the other field is not in CMD_INDIRECT_REF_0.
                #error "ov_set_bm*(), other than byte mask, must set a field that occurs in CMD_INDIRECT_REF_0."

            #endif

            #if (_OV_PAR_REQUESTED & OV_BYTE_MASK)

                #if ((!BYTE_MASK_PROVIDED) && \
                     (_OV_ROW_DATA_REG == 1) && (_OV_BM_DATA_REG == 1) && \
                     (_OV_BM_DATA_LSbit == 0) && (_OV_IND_CSR_MASK_PROVIDED == 0))

                    // We only get here if:
                    // - Byte mask has not been provided
                    // - Byte mask is one of the fields to be provided
                    // - Byte mask is in CMD_INDIRECT_REF_0
                    // - The value is provided for a field in CMD_INDIRECT_REF_0
                    // - Byte mask's least significant bit is at position 0
                    // - No data has yet been provided for CMD_INDIRECT_REF_0

                    #if (_OV_ROW_DATA_LSbit > _OV_BM_DATA_MSbit)

                        // We only get here if:
                        // - Least significant bit of provided field is greater than most significant bit of byte field, which
                        //   will happen if field being provided is not even partial byte mask.

                        #if (_OVF_LOCAL & OVF_RELAXED_ORDER_OFF)

                            // Flags say the user desires optimal order, so #warn!
                            #warning "Optimal order is achieved when ov_set_bm*() is used to simultaneously set byte mask and another field in CMD_INDIRECT_REF_0, and this is the first data for CMD_INDIRECT_REF_0. Use flag OVF_RELAXED_ORDER_ON to suppress this warning."

                        #endif

                    #elif (((_OV_IND_CSR_MASK_REQUIRED & (~(((1 << (_OV_BM_DATA_MSbit + 1)) - 1) - ((1 << _OV_BM_DATA_LSbit) - 1)))) != 0) && \
                           (_ov_dst_msb == _OV_ROW_DATA_MSbit) && (_ov_dst_lsb == _OV_ROW_DATA_LSbit) && \
                           (_ov_src_msb == (_ov_dst_msb - _ov_dst_lsb)) && (_ov_src_lsb == 0))

                        // We only get here if:
                        // - More data than just byte mask is required, which will happen if byte mask is the first-provided data.
                        // - Source and destination bits are defaults

                        #if (_OVF_LOCAL & OVF_RELAXED_ORDER_OFF)

                            // Flags say the user desires optimal order, so #warn!
                            #warning "Optimal order is achieved when ov_set_bm*() is used to simultaneously set byte mask and another field in CMD_INDIRECT_REF_0, and this is the first data for CMD_INDIRECT_REF_0. Use flag OVF_RELAXED_ORDER_ON to suppress this warning."

                        #endif

                    #endif

                #endif

            #endif

        #endif

        #define _OV_ADDED_VALUE_COMPILE_TIME_ZERO 0
        #if ((is_ct_const(_ov_value)) && (_ov_value == 0))

            #define_eval _OV_ADDED_VALUE_COMPILE_TIME_ZERO 1

        #endif

        // To have only one set of code, create common set of symbols for processing parameters
        // for both previous ALU result and Indirect CSR.

        #if (_OV_ROW_DATA_REG == 0)

            #define_eval _ov_reg_src _ov_reg_alu_/**/_OV_REG_ALU_SRC
            #define_eval _ov_reg_dst _ov_reg_alu_/**/_OV_REG_ALU_DST
            #define _ov_reg_english "Previous ALU result"

            #define _OV_REG_MASK_PROVIDED _OV_ALU_MASK_PROVIDED
            #define _OV_REG_MASK_REQUIRED _OV_ALU_MASK_REQUIRED
            #define_eval _OV_REG_SRC_USED (_OVF_STATUS & _OVF_STATUS_ALU_SRC_USED)
            #define_eval _OV_REG_DST_USED (_OVF_STATUS & _OVF_STATUS_ALU_DST_USED)
            #define _OV_CONST             _OV_ALU_CONST

            #if ((is_ct_const(_ov_value)) || (is_rt_const(_ov_value)))

                #if (_OVF_STATUS & _OVF_STATUS_PREV_ALU_REG)

                    // A register value was previously provided, and now we have a constant value

                    #if (_OVF_LOCAL & OVF_RELAXED_ORDER_OFF)

                        // Flags say the user desires optimal order, so #warn!
                        #warning "Constant value provided for previous ALU result after register value. Either provide constant values before register values (optimal order) or use flag OVF_RELAXED_ORDER_ON (suppress this warning)."

                    #endif

                #endif

            #else

                #define_eval _OVF_STATUS (_OVF_STATUS | _OVF_STATUS_PREV_ALU_REG)

                #if (_ov_dst_lsb == 0)

                    #if (_OVF_STATUS & _OVF_STATUS_PREV_ALU_NON_IMMED_CNST)

                        #if (_OVF_LOCAL & OVF_RELAXED_ORDER_OFF)

                            #warning "More efficient code will be generated if this register (LSB at bit 0) is provided first after all constants for previous ALU result. Use flag OVF_RELAXED_ORDER_ON to suppress this warning."

                        #endif

                    #endif

                #endif

            #endif

        #elif (_OV_ROW_DATA_REG == 1)

            #define_eval _ov_reg_src _ov_reg_ind_csr_/**/_OV_REG_IND_SRC
            #define_eval _ov_reg_dst _ov_reg_ind_csr_/**/_OV_REG_IND_DST
            #define _ov_reg_english "Indirect CSR"

            #define _OV_REG_MASK_PROVIDED _OV_IND_CSR_MASK_PROVIDED
            #define _OV_REG_MASK_REQUIRED _OV_IND_CSR_MASK_REQUIRED
            #define_eval _OV_REG_SRC_USED (_OVF_STATUS & _OVF_STATUS_IND_CSR_SRC_USED)
            #define_eval _OV_REG_DST_USED (_OVF_STATUS & _OVF_STATUS_IND_CSR_DST_USED)
            #define _OV_CONST             _OV_IND_CSR_CONST

            #if ((BYTE_MASK_PROVIDED) && (_OV_REG_MASK_PROVIDED & (((1 << (_OV_BM_DATA_MSbit + 1)) - 1) - ((1 << _OV_BM_DATA_LSbit) - 1)) != 0))

                #error "Cannot use ov_set_bm*() because byte mask has already been at least partially provided."

            #endif

            #if ((is_ct_const(_ov_value)) || (is_rt_const(_ov_value)))

                #if (_OVF_STATUS & _OVF_STATUS_IND_CSR_REG)

                    // A register value was previously provided, and now we have a constant value

                    #if (_OVF_LOCAL & OVF_RELAXED_ORDER_OFF)

                        // Options flags say order is important, so #warn!
                        #warning "Constant value provided for CMD_INDIRECT_REF_0 after register value. Either provide constant first (optimal order) or use flag OVF_RELAXED_ORDER_ON (suppress this warning)."

                    #endif

                #endif

            #else

                #define_eval _OVF_STATUS (_OVF_STATUS | _OVF_STATUS_IND_CSR_REG)

                #if (_ov_dst_lsb == 0)

                    #if (_OVF_STATUS & _OVF_STATUS_IND_CSR_NON_IMMED_CNST)

                        #if (_OVF_LOCAL & OVF_RELAXED_ORDER_OFF)

                            #warning "More efficient code will be generated if this register (LSB at bit 0) is provided first after all constants for CMD_INDIRECT_REF_0. Use flag OVF_RELAXED_ORDER_ON to suppress this warning."

                        #endif

                    #endif

                #endif

            #endif

        #else

            #error "Only previous ALU and Indirect CSR output are supported by this code."

        #endif

        // Check whether these bits have been previously provided
        #if ((_OV_REG_MASK_PROVIDED & _OV_DESTINATION_MASK) != 0)

            _ov_hex(((_OV_REG_MASK_PROVIDED & _OV_DESTINATION_MASK) >> _OV_ROW_DATA_LSbit))
            #error "Not allowed to write these bits in field in _OV_TBL_"_OV_ROW_i "in" _ov_reg_english "again:" _OV_HEX_NUMBER

        #endif

        // Record which bits were previously known
        #define_eval _OV_REG_MASK_PREVIOUS (_OV_REG_MASK_PROVIDED) // #define_eval required to make copy of value

        // Record which bits are being added now...
        #define_eval _OV_REG_MASK_PROVIDED (_OV_REG_MASK_PROVIDED | _OV_DESTINATION_MASK)

        #if ((is_ct_const(_ov_value)) || (is_rt_const(_ov_value)))

            // Partial range of bits are being provided, so check that fits within allowed mask
            #if ((((((_ov_value >> _ov_src_lsb) & _OV_SOURCE_MASK_ZERO) - ((_OVF_LOCAL & OVF_SUBTRACT_ONE) ? 1 : 0)) << _ov_dst_lsb) & (~_OV_DESTINATION_MASK)) != 0)

                // Give English explanation of problem
                #error "The provided constant does not fit into the available bits. It may be appropriate to specify the OVF_SUBTRACT_ONE flag."

            #endif

        #endif

        #if ((_OV_ROW_DATA_REG == 0) && (FINAL))

            // Only execute this code if the current parameter is previous ALU output and this is the final call,
            // meaning that Indirect CSR value must be output now.

            // Note: References to _OV_IND_CSR_MASK_PROVIDED, _OV_IND_CSR_CONST and (_OVF_STATUS & _OVF_STATUS_IND_CSR_SRC_USED) are safe
            // to use within this #if (_OV_ROW_DATA_REG == 0), because they are effectively constants here.

            #if (_OV_IND_CSR_MASK_PROVIDED != 0)

                // A value must be output for Indirect CSR

                #if (!(_OVF_STATUS & _OVF_STATUS_IND_REF_RECENTLY_WRITTEN))

                    // CMD_INDIRECT_REF_0 must be written because
                    // Flag OVF_NO_CTX_SWAP_ON is set AND _OVF_STATUS_IND_REF_RECENTLY_WRITTEN is clear

                    #if (_OVF_STATUS & _OVF_STATUS_IND_CSR_SRC_USED)

                        // There is a register component
                        #if (_OV_IND_CSR_CONST != 0)

                            // Constant is nonzero, so must incorporate into _ov_reg_ind_csr_/**/_OV_REG_IND_SRC
                            _ov_alu_shf_reg_const(_ov_reg_ind_csr_/**/_OV_REG_IND_DST, _ov_reg_ind_csr_/**/_OV_REG_IND_SRC, OR, _OV_IND_CSR_CONST) ; ov_set*() // [asm]
                            #define_eval _OV_REG_IND_SRC _OV_REG_IND_DST // Now refer to DST version!
                            #define_eval _OVF_STATUS (_OVF_STATUS | (_OVF_STATUS_IND_CSR_DST_USED | _OVF_STATUS_IND_CSR_SRC_USED))

                            #define_eval _OV_IND_CSR_CONST 0 // Do not process this constant value again

                        #endif

                        // Populate CMD_INDIRECT_REF_0, as final is set
                        local_csr_wr[CMD_INDIRECT_REF_0, _ov_reg_ind_csr_/**/_OV_REG_IND_SRC] ; ov_set*() // [asm]

                    #else

                        // Value for Indirect CSR stored only in constant
                        #if ((_OV_IND_CSR_CONST & (~MAX_IMMEDIATE)) == 0)

                            // Can use immediate constant
                            local_csr_wr[CMD_INDIRECT_REF_0, _OV_IND_CSR_CONST] ; ov_set*() // [asm]

                        #else

                            // Cannot use immediate constant
                            _ov_alu_shf_reg_const(_ov_reg_ind_csr_/**/_OV_REG_IND_DST, --, B, _OV_IND_CSR_CONST) ; ov_set*() // [asm] Optimal instruction(s) determined by constant's value
                            #define_eval _OV_REG_IND_SRC _OV_REG_IND_DST // Now refer to DST version!
                            #define_eval _OVF_STATUS (_OVF_STATUS | (_OVF_STATUS_IND_CSR_DST_USED | _OVF_STATUS_IND_CSR_SRC_USED))

                            #define_eval _OV_IND_CSR_CONST 0 // Do not process this constant value again

                            local_csr_wr[CMD_INDIRECT_REF_0, _ov_reg_ind_csr_/**/_OV_REG_IND_SRC] ; ov_set*() // [asm]

                        #endif

                    #endif

                    #define_eval _OVF_STATUS (_OVF_STATUS | _OVF_STATUS_IND_REF_RECENTLY_WRITTEN)
                    #define_eval _OVF_STATUS (_OVF_STATUS | _OVF_STATUS_IND_REF_PREVIOUSLY_WRITTEN)

                #endif

            #endif

        #endif

        // Process the value that has just been provided

        #define _OV_PREV_ALU_ALREADY_WRITTEN 0

        #if ((is_ct_const(_ov_value) || is_rt_const(_ov_value)))

            #define_eval _OV_CONST (_OV_CONST | ((((_ov_value >> _ov_src_lsb) - ((_OVF_LOCAL & OVF_SUBTRACT_ONE) ? 1 : 0)) & _OV_SOURCE_MASK_ZERO) << _ov_dst_lsb))

            #if ((_OV_ROW_DATA_REG == 1) && \
                   ((_OV_REG_MASK_PREVIOUS == 0) && (_OV_REG_MASK_PROVIDED == _OV_REG_MASK_REQUIRED)) && \
                   ((_OV_CONST & (~MAX_IMMEDIATE)) == 0) && \
                   (_OVF_LOCAL & OVF_NO_CTX_SWAP_ON))

                // We have a special case where ALL of the following are true:
                // - We're working with data destined for CMD_INDIRECT_REF_0
                // - _OV_CONST contains all data going to CMD_INDIRECT_REF_0
                // - _OV_CONST fits within MAX_IMMEDIATE
                // - Flag OVF_NO_CTX_SWAP_ON is set (we're not going to context swap)
                // Under these conditions, we can directly write the value to CMD_INDIRECT_REF_0 and
                // then we don't need to update CMD_INDIRECT_REF_0 again.

                local_csr_wr[CMD_INDIRECT_REF_0, _OV_CONST] ; ov_set*() // [asm]
                #define_eval _OVF_STATUS (_OVF_STATUS | _OVF_STATUS_IND_REF_RECENTLY_WRITTEN)
                #define_eval _OVF_STATUS (_OVF_STATUS | _OVF_STATUS_IND_REF_PREVIOUSLY_WRITTEN)

            #endif

        #else

            // Provided value comes from a register; must use it now.

            #ifdef _VALUE_REGISTER
                #error "_VALUE_REGISTER is defined!"
            #endif

            #define_eval _VALUE_REGISTER _ov_value // Register where value of interest is stored - simplifies code below

            #if ((_OV_MASK_SOURCE) || (_OVF_LOCAL & OVF_SUBTRACT_ONE))

                .reg ov_temp

            #endif

            #if (_OV_MASK_SOURCE)

                bitfield_extract(ov_temp, _VALUE_REGISTER, _ov_src_msb, _ov_src_lsb) ; ov_set*() // [asm]

                #define_eval _VALUE_REGISTER ov_temp

            #endif

            // At this point, register _VALUE_REGISTER contains the value of interest, with LS bit at bit 0.

            #if (_OVF_LOCAL & OVF_SUBTRACT_ONE)

                // One must be subtracted from value
                alu[ov_temp, _VALUE_REGISTER, -, 1] ; ov_set*() // [asm]

                #define_eval _VALUE_REGISTER ov_temp

            #endif

            #if ((_OVF_LOCAL & OVF_PARANOID_ON) && (!_OV_MASK_SOURCE))

                // We're paranoid, and the value hasn't been extracted from a register, so mask value
                // according to the SUBSET of the field to which it is destined (BEFORE being shifted to the left)

                .reg tempparanoid

                _ov_alu_shf_reg_const(tempparanoid, _VALUE_REGISTER, AND, ((1 << (_ov_dst_msb - _ov_dst_lsb + 1)) - 1)) ; ov_set*() // [asm]

                #define_eval _VALUE_REGISTER tempparanoid

            #endif

            // Now use the data / send it to where it needs to be...

            #if ((_ov_dst_lsb != 0) && (_OV_CONST != 0))

                _ov_alu_shf_reg_const(--, --, B, _OV_CONST, _OV_ALU_SHF_REG_CONST_TEST) // [!asm - test only]

                #if (_OV_ALU_SHF_REG_CONST_INSTRUCTIONS == 1)

                    // _OV_CONST can be represented in a single instruction with possibly shift or rotate (by a constant)

                    #if (_OV_ROW_DATA_REG == 0)

                        #define_eval _OVF_STATUS (_OVF_STATUS | _OVF_STATUS_PREV_ALU_NON_IMMED_CNST)

                    #elif (_OV_ROW_DATA_REG == 1)

                        #define_eval _OVF_STATUS (_OVF_STATUS | _OVF_STATUS_IND_CSR_NON_IMMED_CNST)

                    #endif

                #endif

            #endif

            #if ((_OV_ROW_DATA_REG == 1) && \
                   ((_OV_REG_MASK_PREVIOUS == 0) && (_OV_REG_MASK_PROVIDED == _OV_REG_MASK_REQUIRED)) && \
                   (_ov_dst_lsb == 0) && (_OV_CONST == 0) && \
                   (_OVF_LOCAL & OVF_NO_CTX_SWAP_ON))

                // We have a special case where ALL of the following are true:
                // - We're working with data destined for CMD_INDIRECT_REF_0
                // - _VALUE_REGISTER contains all data going to CMD_INDIRECT_REF_0, with the least
                //   significant bit at bit zero
                // - There is no constant component
                // - Flag OVF_NO_CTX_SWAP_ON is set (we're not going to context swap)
                // Under these conditions, we can directly write the value to CMD_INDIRECT_REF_0 and
                // then we don't need to update CMD_INDIRECT_REF_0 again.

                local_csr_wr[CMD_INDIRECT_REF_0, _VALUE_REGISTER] ; ov_set*() // [asm]
                #define_eval _OVF_STATUS (_OVF_STATUS | _OVF_STATUS_IND_REF_RECENTLY_WRITTEN)
                #define_eval _OVF_STATUS (_OVF_STATUS | _OVF_STATUS_IND_REF_PREVIOUSLY_WRITTEN)

            #elif (!_OV_REG_DST_USED)

                // Contents of register have not yet been defined, so value must overwrite register

                #if ((FINAL) && \
                     (_OV_ROW_DATA_REG == 0) && \
                     (_OV_IND_CSR_MASK_PROVIDED == _OV_IND_CSR_MASK_REQUIRED) && \
                     (_OVF_LOCAL & OVF_REUSE_OFF))

                    // We have a special case where ALL of the following are true:
                    // - This is the final step in the indirect reference macro process
                    // - We're working with data destined for previous ALU result.
                    // - This data is the LAST outstanding item (CMD_INDIRECT_REF_0, if required, has already
                    //   been written).
                    // - Flag OVF_REUSE_OFF is set (we're not going to reuse).
                    // Under these conditions, we do not need a destination for the ALU instruction, and we're done.
                    // Note: _VALUE_REGISTER does not necessarily contain the last complete data, but that
                    //       will be caught by _ov_check_all_data_provided() so we don't need to enforce it
                    //       now (enforcing now is difficult because partial fields may be provided)

                    #define _ov_reg_dst_reg_or_null --

                    #define_eval _OV_PREV_ALU_ALREADY_WRITTEN 1

                #else

                    #define _ov_reg_dst_reg_or_null _ov_reg_dst

                    #define_eval _OV_REG_DST_USED 1 // By end of this #if, register content will have been defined

                #endif

                // Parameter value is contained in register named _VALUE_REGISTER, must be shifted to
                // the left by _ov_dst_lsb (if nonzero), and result must be ORd with _OV_CONST (if nonzero),
                // and the result of this put in the destination register _ov_reg_dst.

                #if (BYTE_MASK_PROVIDED)

                    #if ((_ov_dst_lsb != 0) && (_OV_CONST == 0))

                        alu[_ov_reg_dst_reg_or_null, in_byte_mask, OR, _VALUE_REGISTER, <</**/_ov_dst_lsb] ; ov_set*() // [asm]
                        #if (!_OV_PREV_ALU_ALREADY_WRITTEN)
                            // Being written to register, so update
                            #define_eval _ov_reg_src _ov_reg_dst // Now refer to DST version!
                            #define_eval _OV_REG_DST_USED 1
                            #define_eval _OV_REG_SRC_USED 1
                        #endif

                        // Record that byte mask has already been provided
                        #define_eval _OV_REG_MASK_PROVIDED (_OV_REG_MASK_PROVIDED | (((1 << (_OV_BM_DATA_MSbit + 1)) - 1) - ((1 << _OV_BM_DATA_LSbit) - 1)))

                    #else

                        #error "Cannot use ov_set_bm*() when a constant has already been provided for CMD_INDIRECT_REF_0."

                    #endif

                #elif ((_ov_dst_lsb != 0) && (_OV_CONST != 0))

                    #if ((_OV_CONST & (~MAX_IMMEDIATE)) == 0)

                        alu[_ov_reg_dst_reg_or_null, _OV_CONST, OR, _VALUE_REGISTER, <</**/_ov_dst_lsb] ; ov_set*() // [asm]
                        #if (defined(_OV_PREVIOUS_ALU_TO_REGISTER) && FINAL && (_OV_ROW_DATA_REG == 0))
                            alu[_ov_previous_alu_value, _OV_CONST, OR, _VALUE_REGISTER, <</**/_ov_dst_lsb] ; ov_set*use() test code // [asm]
                        #endif
                        #if (!_OV_PREV_ALU_ALREADY_WRITTEN)
                            // Being written to register, so update
                            #define_eval _ov_reg_src _ov_reg_dst // Now refer to DST version!
                            #define_eval _OV_REG_DST_USED 1
                            #define_eval _OV_REG_SRC_USED 1
                        #endif

                    #else

                        _ov_alu_shf_reg_const(_ov_reg_dst, --, B, _OV_CONST) ; ov_set*() // [asm]
                        #define_eval _ov_reg_src _ov_reg_dst // Now refer to DST version!
                        #define_eval _OV_REG_DST_USED 1
                        #define_eval _OV_REG_SRC_USED 1

                        alu[_ov_reg_dst, _ov_reg_src, OR, _VALUE_REGISTER, <</**/_ov_dst_lsb] ; ov_set*() // [asm]
                        #if (defined(_OV_PREVIOUS_ALU_TO_REGISTER) && FINAL && (_OV_ROW_DATA_REG == 0))
                            alu[_ov_previous_alu_value, --, B, _ov_reg_dst] ; ov_set*use() test code // [asm]
                        #endif

                    #endif

                #elif (_ov_dst_lsb != 0)

                    // _ov_dst_lsb is nonzero, _OV_CONST is zero
                    alu[_ov_reg_dst_reg_or_null, --, B, _VALUE_REGISTER, <</**/_ov_dst_lsb] ; ov_set*() // [asm]
                    #if (defined(_OV_PREVIOUS_ALU_TO_REGISTER) && FINAL && (_OV_ROW_DATA_REG == 0))
                        alu[_ov_previous_alu_value, --, B, _VALUE_REGISTER, <</**/_ov_dst_lsb] ; ov_set*use() test code // [asm]
                    #endif
                    #if (!_OV_PREV_ALU_ALREADY_WRITTEN)
                        // Being written to register, so update
                        #define_eval _ov_reg_src _ov_reg_dst // Now refer to DST version!
                        #define_eval _OV_REG_DST_USED 1
                        #define_eval _OV_REG_SRC_USED 1
                    #endif

                #else

                    // _ov_dst_lsb is zero
                    _ov_alu_shf_reg_const(_ov_reg_dst_reg_or_null, _VALUE_REGISTER, OR, _OV_CONST) ; ov_set*() // [asm]
                    #if (defined(_OV_PREVIOUS_ALU_TO_REGISTER) && FINAL && (_OV_ROW_DATA_REG == 0))
                        _ov_alu_shf_reg_const(_ov_previous_alu_value, _VALUE_REGISTER, OR, _OV_CONST) ; ov_set*use() test code // [asm]
                    #endif
                    #if (!_OV_PREV_ALU_ALREADY_WRITTEN)
                        // Being written to register, so update
                        #define_eval _ov_reg_src _ov_reg_dst // Now refer to DST version!
                        #define_eval _OV_REG_DST_USED 1
                        #define_eval _OV_REG_SRC_USED 1
                    #endif

                #endif

                #define_eval _OV_CONST 0 // Value of constant has now been processed

                #undef _ov_reg_dst_reg_or_null

            #else

                // Contents of register have been previously defined, so must OR calculated value with existing contents

                #if (BYTE_MASK_PROVIDED)

                    #error "ov_set_bm*() must be used the first time a value for a field is provided for CMD_INDIRECT_REF_0."

                #endif

                // Parameter value is contained in register named _VALUE_REGISTER, must be shifted to
                // the left by _ov_dst_lsb (if nonzero), and result must be ORd with _OV_CONST (if nonzero), and
                // ORd with the destination register _ov_reg_dst.

                #if ((_ov_dst_lsb != 0) && (_OV_CONST != 0))

                    .reg tempint

                    #if ((_OV_CONST & (~MAX_IMMEDIATE)) == 0)

                        alu[tempint, _OV_CONST, OR, _VALUE_REGISTER, <</**/_ov_dst_lsb] ; ov_set*() // [asm]
                        alu[_ov_reg_dst, _ov_reg_src, OR, tempint] ; ov_set*() // [asm]
                        #if (defined(_OV_PREVIOUS_ALU_TO_REGISTER) && FINAL && (_OV_ROW_DATA_REG == 0))
                            alu[_ov_previous_alu_value, --, B, _ov_reg_dst] ; ov_set*use() test code // [asm]
                        #endif
                        #define_eval _ov_reg_src _ov_reg_dst // Now refer to DST version!
                        #define_eval _OV_REG_DST_USED 1
                        #define_eval _OV_REG_SRC_USED 1

                    #else

                        alu[tempint, --, B, _VALUE_REGISTER, <</**/_ov_dst_lsb] ; ov_set*() // [asm]
                        _ov_alu_shf_reg_const(tempint, tempint, OR, _OV_CONST) ; ov_set*() // [asm]
                        alu[_ov_reg_dst, _ov_reg_src, OR, tempint] ; ov_set*() // [asm]
                        #if (defined(_OV_PREVIOUS_ALU_TO_REGISTER) && FINAL && (_OV_ROW_DATA_REG == 0))
                            alu[_ov_previous_alu_value, --, B, _ov_reg_dst] ; ov_set*use() test code // [asm]
                        #endif
                        #define_eval _ov_reg_src _ov_reg_dst // Now refer to DST version!
                        #define_eval _OV_REG_DST_USED 1
                        #define_eval _OV_REG_SRC_USED 1

                    #endif

                #elif (_ov_dst_lsb != 0)

                    // _ov_dst_lsb is nonzero, _OV_CONST is zero
                    alu[_ov_reg_dst, _ov_reg_src, OR, _VALUE_REGISTER, <</**/_ov_dst_lsb] ; ov_set*() // [asm]
                    #if (defined(_OV_PREVIOUS_ALU_TO_REGISTER) && FINAL && (_OV_ROW_DATA_REG == 0))
                        alu[_ov_previous_alu_value, --, B, _ov_reg_dst] ; ov_set*use() test code // [asm]
                    #endif
                    #define_eval _ov_reg_src _ov_reg_dst // Now refer to DST version!
                    #define_eval _OV_REG_DST_USED 1
                    #define_eval _OV_REG_SRC_USED 1

                #elif (_OV_CONST != 0)

                    // _ov_dst_lsb is zero, _OV_CONST is nonzero
                    .reg tempint
                    _ov_alu_shf_reg_const(tempint, _VALUE_REGISTER, OR, _OV_CONST) ; ov_set*() // [asm]
                    alu[_ov_reg_dst, _ov_reg_src, OR, tempint] ; ov_set*() // [asm]
                    #if (defined(_OV_PREVIOUS_ALU_TO_REGISTER) && FINAL && (_OV_ROW_DATA_REG == 0))
                        alu[_ov_previous_alu_value, --, B, _ov_reg_dst] ; ov_set*use() test code // [asm]
                    #endif
                    #define_eval _ov_reg_src _ov_reg_dst // Now refer to DST version!
                    #define_eval _OV_REG_DST_USED 1
                    #define_eval _OV_REG_SRC_USED 1

                #else

                    // Both _ov_dst_lsb and _OV_CONST are zero
                    alu[_ov_reg_dst, _ov_reg_src, OR, _VALUE_REGISTER] ; ov_set*() // [asm]
                    #if (defined(_OV_PREVIOUS_ALU_TO_REGISTER) && FINAL && (_OV_ROW_DATA_REG == 0))
                        alu[_ov_previous_alu_value, --, B, _ov_reg_dst] ; ov_set*use() test code // [asm]
                    #endif
                    #define_eval _ov_reg_src _ov_reg_dst // Now refer to DST version!
                    #define_eval _OV_REG_DST_USED 1
                    #define_eval _OV_REG_SRC_USED 1

                #endif

                #define_eval _OV_CONST 0 // Value of constant has now been processed

            #endif

            #undef _VALUE_REGISTER

        #endif

        #undef _OV_REG_MASK_PREVIOUS

        #if (FINAL)

            #if (_OV_REG_SRC_USED)

                // There is data in _ov_reg_src, so update it to include the constant value, if nonzero

                #if (_OV_CONST != 0)

                    #if ((!_OV_REG_DST_USED) && (_OV_ROW_DATA_REG == 0) && (!(_OVF_LOCAL & OVF_REUSE_ON)))

                        // There is data in the source, but NOT in the destination, and this is previous
                        // ALU result, and FINAL is set and we're NOT reusing, so don't need to refer
                        // to destination register.

                        #if (!(defined(_OV_PREVIOUS_ALU_TO_REGISTER) && FINAL && (_OV_ROW_DATA_REG == 0)))
                            _ov_alu_shf_reg_const(--, _ov_reg_src, OR, _OV_CONST) ; ov_set*() // [asm]
                        #else
                            _ov_alu_shf_reg_const(_ov_previous_alu_value, _ov_reg_src, OR, _OV_CONST) ; ov_set*use() code modified for testability // [asm]
                        #endif

                    #else

                        _ov_alu_shf_reg_const(_ov_reg_dst, _ov_reg_src, OR, _OV_CONST) ; ov_set*() // [asm]
                        #if (defined(_OV_PREVIOUS_ALU_TO_REGISTER) && FINAL && (_OV_ROW_DATA_REG == 0))
                            alu[_ov_previous_alu_value, --, B, _ov_reg_dst] ; ov_set*use() test code // [asm]
                        #endif
                        #define_eval _ov_reg_src _ov_reg_dst // Now refer to DST version!
                        #define_eval _OV_REG_DST_USED 1
                        #define_eval _OV_REG_SRC_USED 1

                        #define_eval _OV_CONST 0 // Value of constant has now been processed

                    #endif

                #endif

            #else

                // Register is not in use, so constant can be perhaps used directly, so defer to end

            #endif

        #endif

        // To not lose changes, copy data back from common symbols

        #if (_OV_ROW_DATA_REG == 0)

            #define_eval _OV_ALU_MASK_PROVIDED _OV_REG_MASK_PROVIDED

            // Copy _OV_REG_SRC_USED and _OV_REG_DST_USED back to _OVF_STATUS
            #define_eval _OVF_STATUS (_OVF_STATUS & (~_OVF_STATUS_ALU_SRC_USED))
            #define_eval _OVF_STATUS (_OVF_STATUS | (_OV_REG_SRC_USED ? _OVF_STATUS_ALU_SRC_USED : 0))
            #define_eval _OVF_STATUS (_OVF_STATUS & (~_OVF_STATUS_ALU_DST_USED))
            #define_eval _OVF_STATUS (_OVF_STATUS | (_OV_REG_DST_USED ? _OVF_STATUS_ALU_DST_USED : 0))

            #define_eval _OV_ALU_CONST             _OV_CONST
            #if (streq('_ov_reg_dst', '_ov_reg_src'))
                #define_eval _OV_REG_ALU_SRC _OV_REG_ALU_DST // Now refer to DST version!
            #endif

        #elif (_OV_ROW_DATA_REG == 1)

            #define_eval _OV_IND_CSR_MASK_PROVIDED _OV_REG_MASK_PROVIDED

            // Copy _OV_REG_SRC_USED and _OV_REG_DST_USED back to _OVF_STATUS
            #define_eval _OVF_STATUS (_OVF_STATUS & (~_OVF_STATUS_IND_CSR_SRC_USED))
            #define_eval _OVF_STATUS (_OVF_STATUS | (_OV_REG_SRC_USED ? _OVF_STATUS_IND_CSR_SRC_USED : 0))
            #define_eval _OVF_STATUS (_OVF_STATUS & (~_OVF_STATUS_IND_CSR_DST_USED))
            #define_eval _OVF_STATUS (_OVF_STATUS | (_OV_REG_DST_USED ? _OVF_STATUS_IND_CSR_DST_USED : 0))

            #define_eval _OV_IND_CSR_CONST         _OV_CONST
            #if (streq('_ov_reg_dst', '_ov_reg_src'))
                #define_eval _OV_REG_IND_SRC _OV_REG_IND_DST // Now refer to DST version!
            #endif

        #endif

        #undef _OV_REG_MASK_REQUIRED
        #undef _OV_REG_MASK_PROVIDED
        #undef _OV_REG_SRC_USED
        #undef _OV_REG_DST_USED
        #undef _OV_CONST

        #if (FINAL)

            // At this point:
            // - if (_OV_ALU_MASK_REQUIRED != 0):
            //   - if (_OVF_STATUS & _OVF_STATUS_ALU_SRC_USED), _ov_reg_alu_/**/_OV_REG_ALU_SRC contains the register component of previous ALU output
            //   - _OV_ALU_CONST contains the constant component for previous ALU output
            // - if (_OV_IND_CSR_MASK_REQUIRED != 0):
            //   - if (_OVF_STATUS & _OVF_STATUS_IND_CSR_SRC_USED), _ov_reg_ind_csr_/**/_OV_REG_IND_SRC contains the register component for CMD_INDIRECT_REF_0
            //   - _OV_IND_CSR_CONST contains the constant component for CMD_INDIRECT_REF_0
            // - if (_OV_ROW_DATA_REG == 0), CMD_INDIRECT_REF_0 has already been written

            #if (_OV_ROW_DATA_REG == 0)

                // Previous ALU result

                #if (_OVF_STATUS & _OVF_STATUS_ALU_SRC_USED)

                    // _ov_reg_alu_/**/_OV_REG_ALU_SRC is used directly, but will have been done above, so nothing further to be done here

                #else

                    // Common case at end

                #endif

            #elif (_OV_ROW_DATA_REG == 1)

                // CMD_INDIRECT_REF_0 value must be populated appropriately, and then the previous ALU value put into place

                #if (_OVF_STATUS & _OVF_STATUS_IND_CSR_SRC_USED)

                    // At this point, _ov_reg_ind_csr_/**/_OV_REG_IND_SRC will already include any constants, so just copy over directly
                    local_csr_wr[CMD_INDIRECT_REF_0, _ov_reg_ind_csr_/**/_OV_REG_IND_SRC] ; ov_set*() // [asm]

                #else

                    // At this point, _ov_reg_ind_csr_/**/_OV_REG_IND_SRC is not used, and the relevant constant is contained in _OV_IND_CSR_CONST
                    #if ((_OV_IND_CSR_CONST & (~MAX_IMMEDIATE)) == 0)

                        // Can use immediate constant
                        local_csr_wr[CMD_INDIRECT_REF_0, _OV_IND_CSR_CONST] ; ov_set*() // [asm]

                    #else

                        // Cannot use immediate constant
                        _ov_alu_shf_reg_const(_ov_reg_ind_csr_/**/_OV_REG_IND_DST, --, B, _OV_IND_CSR_CONST) ; ov_set*() // [asm]
                        #define_eval _OV_REG_IND_SRC _OV_REG_IND_DST // Now refer to DST version!
                        #define_eval _OVF_STATUS (_OVF_STATUS | (_OVF_STATUS_IND_CSR_DST_USED | _OVF_STATUS_IND_CSR_SRC_USED))

                        #define_eval _OV_IND_CSR_CONST 0 // Value of constant has now been processed

                        local_csr_wr[CMD_INDIRECT_REF_0, _ov_reg_ind_csr_/**/_OV_REG_IND_SRC] ; ov_set*() // [asm]

                    #endif

                #endif

                #define_eval _OVF_STATUS (_OVF_STATUS | _OVF_STATUS_IND_REF_RECENTLY_WRITTEN)
                #define_eval _OVF_STATUS (_OVF_STATUS | _OVF_STATUS_IND_REF_PREVIOUSLY_WRITTEN)

                #if (_OVF_STATUS & _OVF_STATUS_ALU_SRC_USED)

                    // Previous ALU result destroyed by calculations above, so repopulate
                    #if (!(defined(_OV_PREVIOUS_ALU_TO_REGISTER) && FINAL))
                        alu[--, --, B, _ov_reg_alu_/**/_OV_REG_ALU_SRC] ; ov_set*() // [asm]
                    #else
                        alu[_ov_previous_alu_value, --, B, _ov_reg_alu_/**/_OV_REG_ALU_SRC] ; ov_set*use() code modified for testability // [asm]
                    #endif

                #else

                    // Common case at end

                #endif

            #endif

            #if ((!(_OVF_STATUS & _OVF_STATUS_ALU_SRC_USED)) && !_OV_PREV_ALU_ALREADY_WRITTEN)

                // For both cases above, just have a constant for previous ALU history, so put into previous ALU history...
                // Alternatively, the last field was a compile time zero added to previous ALU result, so
                // it HAS to be written now.

                #if (_OVF_LOCAL & OVF_REUSE_ON)

                    _ov_alu_shf_reg_const(_ov_reg_alu_/**/_OV_REG_ALU_DST, --, B, _OV_ALU_CONST, _OV_ALU_SHF_REG_CONST_TEST) // [!asm - test only]
                    // Note: No adjustments to _OV_REG_ALU_SRC and _OVF_STATUS because above does NOT generate code!

                    #if (_OV_ALU_SHF_REG_CONST_INSTRUCTIONS == 1)

                        // Single instruction, so do NOT put into register

                        #if (!(defined(_OV_PREVIOUS_ALU_TO_REGISTER) && FINAL))
                            _ov_alu_shf_reg_const(--, --, B, _OV_ALU_CONST) ; ov_set*() // [asm]
                        #else
                            _ov_alu_shf_reg_const(_ov_previous_alu_value, --, B, _OV_ALU_CONST) ; ov_set*use() code modified for testability // [asm]
                        #endif

                    #else

                        // More than one instruction and going to be reused, so put into a register
                        _ov_alu_shf_reg_const(_ov_reg_alu_/**/_OV_REG_ALU_DST, --, B, _OV_ALU_CONST) ; ov_set*() // [asm]
                        #if (defined(_OV_PREVIOUS_ALU_TO_REGISTER) && FINAL)
                            alu[_ov_previous_alu_value, --, B, _ov_reg_alu_/**/_OV_REG_ALU_DST] ; ov_set*use() test code // [asm]
                        #endif
                        #define_eval _OV_REG_ALU_SRC _OV_REG_ALU_DST // Now refer to DST version!
                        #define_eval _OVF_STATUS (_OVF_STATUS | (_OVF_STATUS_ALU_DST_USED | _OVF_STATUS_ALU_SRC_USED))

                        #define_eval _OV_ALU_CONST 0 // Value of constant has now been processed

                    #endif

                #else

                    // Not going to be reused, so skip putting into register
                    #if (!(defined(_OV_PREVIOUS_ALU_TO_REGISTER) && FINAL))
                        _ov_alu_shf_reg_const(--, --, B, _OV_ALU_CONST) ; ov_set*() // [asm]
                    #else
                        _ov_alu_shf_reg_const(_ov_previous_alu_value, --, B, _OV_ALU_CONST) ; ov_set*use() code modified for testability // [asm]
                    #endif

                #endif

            #elif (_OV_ADDED_VALUE_COMPILE_TIME_ZERO)

                // Last field value supplied was a zero compile-time constant, so must explicitly output value here

                #if (_OVF_STATUS & _OVF_STATUS_ALU_SRC_USED)

                    // There is a value in the register, so use it
                    #if (!(defined(_OV_PREVIOUS_ALU_TO_REGISTER) && FINAL))
                        _ov_alu_shf_reg_const(--, _ov_reg_alu_/**/_OV_REG_ALU_SRC, OR, _OV_ALU_CONST) ; ov_set*() // [asm]
                    #else
                        _ov_alu_shf_reg_const(_ov_previous_alu_value, _ov_reg_alu_/**/_OV_REG_ALU_SRC, OR, _OV_ALU_CONST) ; ov_set*use() code modified for testability // [asm]
                    #endif

                #else

                    // There is NOT a value in the register, so use the constant
                    #if (!(defined(_OV_PREVIOUS_ALU_TO_REGISTER) && FINAL))
                        _ov_alu_shf_reg_const(--, --, B, _OV_ALU_CONST) ; ov_set*() // [asm]
                    #else
                        _ov_alu_shf_reg_const(_ov_previous_alu_value, --, B, _OV_ALU_CONST) ; ov_set*use() code modified for testability // [asm]
                    #endif

                #endif

            #endif

        #endif

        #undef _OV_ADDED_VALUE_COMPILE_TIME_ZERO

        #undef _OV_PREV_ALU_ALREADY_WRITTEN

        #undef _ov_reg_dst
        #undef _ov_reg_src
        #undef _ov_reg_english

        #ifdef _OV_MASK_SOURCE
            #undef _OV_MASK_SOURCE
        #endif

        #undef _OV_SOURCE_MASK_ZERO

        #undef _OV_DESTINATION_MASK

        #ifdef _ov_src_msb
            #undef _ov_src_msb
        #endif

        #ifdef _ov_src_lsb
            #undef _ov_src_lsb
        #endif

        #ifdef _ov_dst_msb
            #undef _ov_dst_msb
        #endif

        #ifdef _ov_dst_lsb
            #undef _ov_dst_lsb
        #endif


        #ifdef _OV_ROW_i
            #undef _OV_ROW_i
        #endif

        #if ((((_OV_ALU_MASK_REQUIRED == _OV_ALU_MASK_PROVIDED) && (_OV_IND_CSR_MASK_REQUIRED == _OV_IND_CSR_MASK_PROVIDED)) || FINAL) && \
             (_OV_ROW_DATA_REG != 0) && \
             (_OV_ALU_MASK_REQUIRED != 0))

            // ALL of the following are true:
            // - The last value has just been provided (by required and provided bits matching or FINAL flag being set)
            // - The last value is not for previous ALU
            // - Fields in previous ALU are used
            // so the order of data provided for overridden fields is not optimal

            #if (_OVF_LOCAL & OVF_RELAXED_ORDER_OFF)

                // Options flags say the user desires optimal order, so #warn!
                #warning "The last provided value should be for previous ALU result (such as OV_LENGTH) for optimal order. Use flag OVF_RELAXED_ORDER_ON to suppress this warning."

            #endif

        #endif

        #if (FINAL)

            // Have been asked to produce appropriate output, so check that all required
            // input has been provided. This is after the output, but still breaks
            // compilation, so isn't too late.

            // This must be at the end, because the code above updates the preprocessor
            // values we now need to check.

            _ov_check_all_data_provided(_OVF_LOCAL) // Produces #error if all required data has not yet been provided

        #endif

        #undef _OVF_LOCAL

        #undef _ov_value

        #if (SET_BITS)

            #undef _ov_bit_mask

        #endif

    #endif

.end
#endm // _ov_set
/// @endcond


/*
    Convert the input VAL (constant expression) to a hexadecimal number of
    the form 0x%04x. The result is stored in _OV_HEX_NUMBER.

    _OV_HEX_NUMBER may safely be used as input (VAL).
*/
/// @cond INTERNAL_MACROS
#macro _ov_hex(VAL)

    #define_eval _OV_HEX_IN (VAL)

    #define_eval _OV_HEX_OUT ''

    #define_eval _OV_HEX_LOOP (32-4) // Number of bits to show - 4
    #while (_OV_HEX_LOOP >= 0)

        #define_eval _OV_TMP ((_OV_HEX_IN >> _OV_HEX_LOOP) & 0xf)
        #define_eval _OV_TMP strleft(strright("0123456789abcdef", 16 - _OV_TMP), 1)
        #define_eval _OV_HEX_OUT '_OV_HEX_OUT/**/_OV_TMP'

        #define_eval _OV_HEX_LOOP (_OV_HEX_LOOP - 4)

    #endloop
    #undef _OV_HEX_LOOP

    #define_eval _OV_HEX_NUMBER '0x/**/_OV_HEX_OUT'

#endm
/// @endcond


/// @cond INTERNAL_MACROS
#macro _ov_check_all_data_provided(_OVF_LOCAL)

    #if ((_OV_ALU_MASK_PROVIDED != _OV_ALU_MASK_REQUIRED) || (_OV_IND_CSR_MASK_PROVIDED != _OV_IND_CSR_MASK_REQUIRED))

        // Not all required data has been provided, so give meaningful warnings or errors as appropriate.

        // Fetch the row for each parameter so that we can indicate where the issue is / issues are.
        #define _OV_PAR_i 0
        #while (_OV_PAR_i < _OV_PAR_COUNT)

            //#define_eval _OV_ROW_i (_ov_row_read(_OV_PAR_i)) // _OV_ROW_i is in "0000" format
            // Unrolled version of above (to increase speed, reduce list file size)
            #define_eval _OV_ROW_i (strleft(strright('_OV_ROW_PAR', -4 * (_OV_PAR_i)), 4)) // _OV_ROW_i is in "0000" format

            #define_eval _OV_ROW_i (_OV_ROW_i) // _OV_ROW_i is in "0" format, required for following table lookup

            //_ov_va_read_OV_TBL_all(_OV_ROW_i) // Fetch data for this row
            // Unroll above for speed test...
            #define_eval _OV_VA_LIST '_OV_TBL_/**/_OV_ROW_i'
            #define_eval _OV_VA_NUM 0
            #define_eval _OV_VA_IDX_PREV (0)
            // While there is still a comma in the list...
            #while (strstr('_OV_VA_LIST', ",") > 0)
                // Find the index of the next comma
                #define_eval _OV_VA_IDX strstr('_OV_VA_LIST', ",")
                // Replace the comma with a colon in the working copy
                #define_eval _OV_VA_LIST_LEFT strleft('_OV_VA_LIST', _OV_VA_IDX - 1)
                #define_eval _OV_VA_LIST_RIGHT strright('_OV_VA_LIST', -_OV_VA_IDX)
                #define_eval _OV_VA_LIST '_OV_VA_LIST_LEFT:_OV_VA_LIST_RIGHT'
                #undef _OV_VA_LIST_LEFT
                #undef _OV_VA_LIST_RIGHT
                #define_eval _OV_VA_ARG strright(strleft('_OV_VA_LIST', _OV_VA_IDX - 1), - _OV_VA_IDX_PREV)
                #define_eval _OV_VA_ARG (_OV_VA_ARG)
                #if (_OV_VA_NUM == 0)
                    #define_eval _OV_ROW_OVERRIDE (_OV_VA_ARG)
                #elif (_OV_VA_NUM == 1)
                    #define_eval _OV_ROW_DATA_REG (_OV_VA_ARG)
                #elif (_OV_VA_NUM == 2)
                    #define_eval _OV_ROW_DATA_MSbit (_OV_VA_ARG)
                #elif (_OV_VA_NUM == 3)
                    #define_eval _OV_ROW_DATA_LSbit (_OV_VA_ARG)
                #elif (_OV_VA_NUM == 4)
                    #define_eval _OV_ROW_CONF_MASK (_OV_VA_ARG)
                #elif (_OV_VA_NUM == 5)
                    // Final entry could have a trailing comma
                    #define_eval _OV_ROW_CONF_VALUE (_OV_VA_ARG)
                #endif
                #define_eval _OV_VA_IDX_PREV (_OV_VA_IDX + 1)
                #define_eval _OV_VA_NUM (_OV_VA_NUM + 1)
            #endloop
            #if (_OV_VA_NUM == _OV_TBL_COLUMNS)
                // Final entry doesn't necessarily have a trailing comma
                #define_eval _OV_VA_ARG strright('_OV_VA_LIST', - _OV_VA_IDX_PREV)
                #define_eval _OV_ROW_CONF_VALUE (_OV_VA_ARG)
            #endif
            #undef _OV_VA_LIST
            #undef _OV_VA_NUM
            #undef _OV_VA_IDX_PREV
            #undef _OV_VA_IDX
            // ...end of _ov_va_read_OV_TBL_all()

            // _OV_ROW_OVERRIDE, _OV_ROW_DATA_REG, _OV_ROW_DATA_MSbit, _OV_ROW_DATA_LSbit,
            // _OV_ROW_CONF_MASK and _OV_ROW_CONF_VALUE now contain data for one of the rows
            // which had to be populated, so check that all of those bits have been provided.

            #ifdef _OV_DESTINATION_MASK
                #warning "_OV_DESTINATION_MASK already defined!"
            #endif
            #ifdef _OV_MASK_PROVIDED
                #warning "_OV_MASK_PROVIDED already defined!"
            #endif
            #ifdef _OV_REG
                #warning "_OV_REG already defined!"
            #endif

            #define_eval _OV_DESTINATION_MASK (((1 << (_OV_ROW_DATA_MSbit + 1)) - 1) - ((1 << _OV_ROW_DATA_LSbit) - 1))

            #if (_OV_ROW_DATA_REG == 0)

                // Previous ALU
                #define _OV_MASK_PROVIDED _OV_ALU_MASK_PROVIDED
                #define _OV_REG "previous ALU result"

            #else

                // Indirect CSR
                #define _OV_MASK_PROVIDED _OV_IND_CSR_MASK_PROVIDED
                #define _OV_REG "CMD_INDIRECT_REF_0"

            #endif

            #if ((_OV_DESTINATION_MASK & (~_OV_MASK_PROVIDED)) != 0)

                #if ((_OV_DESTINATION_MASK & _OV_MASK_PROVIDED) == 0)

                    // No bits were set for this field, so #error
                    #error "The field specified in _OV_TBL_"_OV_ROW_i "does not have any bits set."

                #else

                    // Not all bits in the destination mask were set, so give appropriate #warning / #error
                    _ov_hex(((_OV_DESTINATION_MASK & (~_OV_MASK_PROVIDED)) >> _OV_ROW_DATA_LSbit))

                    #if (_OVF_LOCAL & OVF_PARTIAL_BITS_ON)

                        // Partial bits allowed, so just #warning
                        #warning "The field specified in _OV_TBL_"_OV_ROW_i "(in" _OV_REG") does not have the following bits set:" _OV_HEX_NUMBER "- zero assumed for these bits for testing purposes."

                    #else

                        // Partial bits NOT allowed, so #error
                        #error "The field specified in _OV_TBL_"_OV_ROW_i "(in" _OV_REG") does not have the following bits set:" _OV_HEX_NUMBER". Use flag OVF_PARTIAL_BITS_ON to convert #error to #warning for testing."

                    #endif

                #endif

            #endif

            #undef _OV_REG
            #undef _OV_MASK_PROVIDED
            #undef _OV_DESTINATION_MASK

            #undef _OV_ROW_i

            #define_eval _OV_PAR_i (_OV_PAR_i + 1)
        #endloop
        #undef _OV_PAR_i

    #endif

    // Check that only being used more than once if configured to do this!
    #if (_OVF_STATUS & _OVF_STATUS_USED)

        #if (!(_OVF_STATUS & _OVF_STATUS_ALWAYS_REUSE))

            #error "ov_*use() only permitted once for current configuration flags! Specify flag OVF_REUSE_ON to allow reuse."

        #endif

    #endif

    #define_eval _OVF_STATUS (_OVF_STATUS | _OVF_STATUS_USED)

#endm
/// @endcond


/// @cond INTERNAL_MACROS
#macro _ov_0b_convert(VALUE)

    #define_eval _OV_0b_IS_BINARY 0

    #define_eval _OV_0b_VALUE 0

    #if ((strlen('VALUE') >= 2) && streq(strleft('VALUE', 2), '0b'))

        // VALUE starts with '0b', so assume binary for the moment

        #define_eval _OV_0b_IS_BINARY 1

        #define_eval _OV_0b_STR (strright('VALUE', -2))

        #define _OV_0b_WEIGHT 1

        #define_eval _OV_0b_VALUE 0

        #while (strlen('_OV_0b_STR') > 0)

            #if (streq(strright('_OV_0b_STR', 1), '1'))

                #define_eval _OV_0b_VALUE (_OV_0b_VALUE + _OV_0b_WEIGHT)

            #elif (!streq(strright('_OV_0b_STR', 1), '0'))

                // Character is not a zero or a 1, so not a valid binary value
                #define_eval _OV_0b_IS_BINARY 0

            #endif

            #define_eval _OV_0b_WEIGHT (_OV_0b_WEIGHT << 1)

            #define_eval _OV_0b_STR (strleft('_OV_0b_STR', strlen('_OV_0b_STR') - 1))

        #endloop

        #undef _OV_0b_WEIGHT

    #endif

#endm
/// @endcond


/**
  * Set bits within the mask of the indicated field to the provided value.
  *
  * This macro is typically useful to specify constant bits that indicate which
  * subcommand is required.
  *
  * @b Example:
  * @code
  * ov_start(OV_LENGTH)
  * ov_set_bits(OV_LENGTH, 0b11000, 0b01000) // Clear bit 4 and set bit 3
  * ov_set(OV_LENGTH, 2, 0, 3)
  * ov_use()
  * command[...], indirect_ref
  * ov_clean()
  * @endcode
  *
  * @param OVERRIDE_TYPE  One override field from @ref OverrideFields "Override fields"
  *                       that was specified in ov_start().
  * @param BIT_MASK       Mask indicating which bits must be set. Valid formats are
  *                       decimal, hexadecimal and binary (prefixed with "0b").
  * @param BIT_CONSTANT   Constant indicating value of bit for each bit set in @a BIT_MASK.
  *                       Valid formats are decimal, hexadecimal and binary (prefixed
  *                       with "0b").
  *
  * @note
  * @a BIT_MASK and @a BIT_CONSTANT must be constant values.
  *
  * @condition_codes Unchanged
  *
  * @instruction_cnt None - only preprocessor
  */
#macro ov_set_bits(OVERRIDE_TYPE, BIT_MASK, BIT_CONSTANT)

    _ov_set(OVERRIDE_TYPE, BIT_CONSTANT, 0, --, --, --, --, 0, 0, 0, 1, BIT_MASK)

#endm // ov_set


/**
  * Set the specific range of bits in the indicated field to the provided value.
  *
  * This macro is typically useful to specify a range of constant bits that are
  * not used in the instruction that this indirect reference is going to be used
  * with.
  *
  * @b Example:
  * @code
  * ov_start(OV_LENGTH)
  * ov_set_bits(OV_LENGTH, 4, 3, 0) // Clear bits 4 and 3
  * ov_set(OV_LENGTH, 2, 0, 3)
  * ov_use()
  * command[...], indirect_ref
  * ov_clean()
  * @endcode
  *
  * @param OVERRIDE_TYPE  One override field from @ref OverrideFields "Override fields"
  *                       that was specified in ov_start().
  * @param DST_MSB        Most significant bit within field @a OVERRIDE_TYPE to set.
  * @param DST_LSB        Least significant bit within field @a OVERRIDE_TYPE to set.
  * @param BIT_CONSTANT   Constant to set the bits @a DST_MSB:@a DST_LSB to.
  *
  * @note
  * @a BIT_CONSTANT is relative to @a DST_LSB, i.e. BIT_CONSTANT will be shifted to
  * the left by @a DST_LSB before being used.
  *
  * @condition_codes Unchanged
  *
  * @instruction_cnt None - only preprocessor
  */
#macro ov_set_bits(OVERRIDE_TYPE, DST_MSB, DST_LSB, BIT_CONSTANT)

    _ov_set(OVERRIDE_TYPE, BIT_CONSTANT, 0, DST_MSB, DST_LSB, --, --, 0, 0, 0, 1, 0)

#endm // ov_set


/**
  * Set the specific range of bits in the indicated field to the provided value, taking
  * the provided flags into account.
  *
  * @b Example:
  * @code
  * ov_start(OV_LENGTH)
  * ov_set(OV_LENGTH, 4, 4, 0)
  * ov_set(OV_LENGTH, 3, 0, 1, OVF_RELAXED_ORDER_ON)
  * ov_use()
  * command[...], indirect_ref
  * ov_clean()
  * @endcode
  *
  * @param OVERRIDE_TYPE  One override field from @ref OverrideFields "Override fields"
  *                       that was specified in ov_start().
  * @param DST_MSB        Most significant bit within field @a OVERRIDE_TYPE to set.
  * @param DST_LSB        Least significant bit within field @a OVERRIDE_TYPE to set.
  * @param in_value       Value to set the bits @a DST_MSB:@a DST_LSB to. May be a constant
  *                       or a register value.
  * @param FLAGS          Optional: Zero or more flags from @ref OverrideFlags "Override flags",
  *                       ORd together, which, for just this macro call, overrides the indirect
  *                       reference's flags.
  *
  * @note
  * @a in_value is relative to @a DST_LSB, i.e. in_value will be shifted to the left
  * by @a DST_LSB before being used.
  *
  * @condition_codes Clobbered (in_value a register value) or unchanged (in_value a constant).
  *
  * @instruction_cnt One or more (in_value a register value) or zero (in_value a constant).
  */
#macro ov_set(OVERRIDE_TYPE, DST_MSB, DST_LSB, in_value, FLAGS)

    _ov_set(OVERRIDE_TYPE, in_value, 0, DST_MSB, DST_LSB, --, --, FLAGS, 0, 0, 0, 0)

#endm // ov_set


/// @cond INTERNAL_MACROS // Not documentating this macro because similar to previous
#macro ov_set(OVERRIDE_TYPE, DST_MSB, DST_LSB, in_value)

    _ov_set(OVERRIDE_TYPE, in_value, 0, DST_MSB, DST_LSB, --, --, 0, 0, 0, 0, 0)

#endm // ov_set
/// @endcond


/**
  * Set the indicated field to the provided value, taking the provided flags into account.
  *
  * @b Example:
  * @code
  * ov_start(OV_LENGTH)
  * ov_set(OV_LENGTH, 12, OVF_SUBTRACT_ONE)
  * ov_use()
  * command[...], indirect_ref
  * ov_clean()
  * @endcode
  *
  * @param OVERRIDE_TYPE  One override field from @ref OverrideFields "Override fields"
  *                       that was specified in ov_start().
  * @param in_value       Value to set field @a OVERRIDE_TYPE to. May be a constant
  *                       or a register value.
  * @param FLAGS          Optional: Zero or more flags from @ref OverrideFlags "Override flags",
  *                       ORd together, which, for just this macro call, overrides the indirect
  *                       reference's flags.
  *
  * @condition_codes Clobbered (in_value a register value) or unchanged (in_value a constant).
  *
  * @instruction_cnt One or more (in_value a register value) or zero (in_value a constant).
  */
#macro ov_set(OVERRIDE_TYPE, in_value, FLAGS)

    _ov_set(OVERRIDE_TYPE, in_value, 0, --, --, --, --, FLAGS, 0, 0, 0, 0)

#endm // ov_set


/// @cond INTERNAL_MACROS // Not documentating this macro because similar to previous
#macro ov_set(OVERRIDE_TYPE, in_value)

    _ov_set(OVERRIDE_TYPE, in_value, 0, --, --, --, --, 0, 0, 0, 0, 0)

#endm // ov_set
/// @endcond


/**
  * Set the OV_BYTE_MASK field to the provided value and also
  * set the specific range of bits in the indicated field to the provided value, taking
  * the provided flags into account.
  *
  * @note
  * This macro is functionally equivalent to the following, except that it produces more
  * efficient code:
  * @code
  * ov_set(OV_BYTE_MASK, in_byte_mask)
  * ov_set(OVERRIDE_TYPE, DST_MSB, DST_LSB, in_value, FLAGS)
  * @endcode
  *
  * @b Example:
  * @code
  * ov_start((OV_LENGTH | OV_BYTE_MASK | OV_DATA_REF | OV_DATA_MASTER |
  *     OV_SIGNAL_MASTER | OV_MASTER_ISLAND))
  * ov_set(OV_DATA_MASTER, 3, 2, 0)
  * ov_set_bm_and(byte_mask, OV_DATA_MASTER, 1, 0, data_master, OVF_PARANOID_ON)
  * ov_set(OV_DATA_REF, data_ref)
  * ov_set(OV_SIGNAL_MASTER, signal_master)
  * ov_set(OV_MASTER_ISLAND, mater_island)
  * ov_set(OV_LENGTH, len, OVF_SUBTRACT_ONE)
  * ov_use()
  * command[...], indirect_ref
  * ov_clean()
  * @endcode
  *
  * @param in_byte_mask   Value to be used for override field OV_BYTE_MASK.
  * @param OVERRIDE_TYPE  One override field from @ref OverrideFields "Override fields"
  *                       that was specified in ov_start().
  * @param DST_MSB        Most significant bit within field @a OVERRIDE_TYPE to set.
  * @param DST_LSB        Least significant bit within field @a OVERRIDE_TYPE to set.
  * @param in_value       Value to set the bits @a DST_MSB:@a DST_LSB in @a OVERRIDE_TYPE to.
  *                       May be a constant or a register value.
  * @param FLAGS          Optional: Zero or more flags from @ref OverrideFlags "Override flags",
  *                       ORd together, which, for just this macro call, overrides the indirect
  *                       reference's flags.
  *
  * @note
  * The parameter @a in_byte_mask is used in its entirety as is. No provided parameters are
  * applicable to it.
  * \n\n
  * @note
  * @a in_value is relative to @a DST_LSB, i.e. in_value will be shifted to the left
  * by @a DST_LSB before being used.
  *
  * @condition_codes Clobbered
  *
  * @instruction_cnt One or more
  */
#macro ov_set_bm_and(in_byte_mask, OVERRIDE_TYPE, DST_MSB, DST_LSB, in_value, FLAGS)

    _ov_set(OVERRIDE_TYPE, in_value, 0, DST_MSB, DST_LSB, --, --, FLAGS, 1, in_byte_mask, 0, 0)

#endm // ov_set_bm_and


/// @cond INTERNAL_MACROS // Not documentating this macro because similar to previous
#macro ov_set_bm_and(in_byte_mask, OVERRIDE_TYPE, DST_MSB, DST_LSB, in_value)

    _ov_set(OVERRIDE_TYPE, in_value, 0, DST_MSB, DST_LSB, --, --, 0, 1, in_byte_mask, 0, 0)

#endm // ov_set_bm_and
/// @endcond


/**
  * Set the OV_BYTE_MASK field to the provided value and also
  * set the indicated field to the provided value, taking
  * the provided flags into account.
  *
  * @note
  * This macro is functionally equivalent to the following, except that it produces more
  * efficient code:
  * @code
  * ov_set(OV_BYTE_MASK, in_byte_mask)
  * ov_set(OVERRIDE_TYPE, in_value, FLAGS)
  * @endcode
  *
  * @b Example:
  * @code
  * ov_start((OV_LENGTH | OV_BYTE_MASK | OV_DATA_REF | OV_DATA_MASTER |
  *     OV_SIGNAL_MASTER | OV_MASTER_ISLAND))
  * ov_set_bm_and(byte_mask, OV_DATA_MASTER, data_master, OVF_PARANOID_ON)
  * ov_set(OV_DATA_REF, data_ref)
  * ov_set(OV_SIGNAL_MASTER, signal_master)
  * ov_set(OV_MASTER_ISLAND, mater_island)
  * ov_set(OV_LENGTH, len, OVF_SUBTRACT_ONE)
  * ov_use()
  * command[...], indirect_ref
  * ov_clean()
  * @endcode
  *
  * @param in_byte_mask   Value to be used for override field OV_BYTE_MASK.
  * @param OVERRIDE_TYPE  One override field from @ref OverrideFields "Override fields"
  *                       that was specified in ov_start().
  * @param in_value       Value to set the field @a OVERRIDE_TYPE to. May be a constant
  *                       or a register value.
  * @param FLAGS          Optional: Zero or more flags from @ref OverrideFlags "Override flags",
  *                       ORd together, which, for just this macro call, overrides the indirect
  *                       reference's flags.
  *
  * @note
  * The parameter @a in_byte_mask is used in its entirety as is. No provided parameters are
  * applicable to it.
  *
  * @condition_codes Clobbered
  *
  * @instruction_cnt One or more
  */
#macro ov_set_bm_and(in_byte_mask, OVERRIDE_TYPE, in_value, FLAGS)

    _ov_set(OVERRIDE_TYPE, in_value, 0, --, --, --, --, FLAGS, 1, in_byte_mask, 0, 0)

#endm // ov_set_bm_and


/// @cond INTERNAL_MACROS // Not documentating this macro because similar to previous
#macro ov_set_bm_and(in_byte_mask, OVERRIDE_TYPE, in_value)

    _ov_set(OVERRIDE_TYPE, in_value, 0, --, --, --, --, 0, 1, in_byte_mask, 0, 0)

#endm // ov_set_bm_and
/// @endcond


/**
  * Set the specific range of bits in the indicated field to the extracted range of bits
  * in the provided value, taking the provided flags into account.
  *
  * The data is extracted from @a in_value, meaning that bits set in @a in_value outside
  * the range @a SRC_MSB:@a SRC_LSB have no effect on the indirect reference.
  *
  * @b Example:
  * @code
  * ov_start(OV_LENGTH)
  * ov_set(OV_LENGTH, 4, 4, 0)
  * ov_set_extract(OV_LENGTH, 3, 0, value, 12, 9, OVF_RELAXED_ORDER_ON)
  * ov_use()
  * command[...], indirect_ref
  * ov_clean()
  * @endcode
  *
  * @param OVERRIDE_TYPE  One override field from @ref OverrideFields "Override fields"
  *                       that was specified in ov_start().
  * @param DST_MSB        Most significant bit within field @a OVERRIDE_TYPE to set.
  * @param DST_LSB        Least significant bit within field @a OVERRIDE_TYPE to set.
  * @param in_value       Value to set the bits @a DST_MSB:@a DST_LSB to. May be a constant
  *                       or a register value.
  * @param SRC_MSB        Most significant bit within field @a in_value to use.
  * @param SRC_LSB        Least significant bit within field @a in_value to use.
  * @param FLAGS          Optional: Zero or more flags from @ref OverrideFlags "Override flags",
  *                       ORd together, which, for just this macro call, overrides the indirect
  *                       reference's flags.
  *
  * @note
  * Bits in the range @a SRC_MSB:@a SRC_LSB are extracted from in_value, and are placed in the
  * indicated field relative to @a DST_LSB and @a, i.e. in_value will be masked and
  * effectively shifted to the left by (@a DST_LSB - @a SRC_LSB) bit positions before being used.
  *
  * @condition_codes Clobbered (in_value a register value) or unchanged (in_value a constant).
  *
  * @instruction_cnt Two or more (in_value a register value) or zero (in_value a constant).
  */
#macro ov_set_extract(OVERRIDE_TYPE, DST_MSB, DST_LSB, in_value, SRC_MSB, SRC_LSB, FLAGS)

    _ov_set(OVERRIDE_TYPE, in_value, 0, DST_MSB, DST_LSB, SRC_MSB, SRC_LSB, FLAGS, 0, 0, 0, 0)

#endm // ov_set_extract


/// @cond INTERNAL_MACROS // Not documentating this macro because similar to previous
#macro ov_set_extract(OVERRIDE_TYPE, DST_MSB, DST_LSB, in_value, SRC_MSB, SRC_LSB)

    _ov_set(OVERRIDE_TYPE, in_value, 0, DST_MSB, DST_LSB, SRC_MSB, SRC_LSB, 0, 0, 0, 0, 0)

#endm // ov_set_extract
/// @endcond


/**
  * Set the value of the indicated field to the extracted range of bits
  * in the provided value, taking the provided flags into account.
  *
  * The data is extracted from @a in_value, meaning that bits set in @a in_value outside
  * the range @a SRC_MSB:@a SRC_LSB have no effect on the indirect reference.
  *
  * @b Example:
  * @code
  * ov_start(OV_LENGTH)
  * ov_set_extract(OV_LENGTH, value, 13, 9)
  * ov_use()
  * command[...], indirect_ref
  * ov_clean()
  * @endcode
  *
  * @param OVERRIDE_TYPE  One override field from @ref OverrideFields "Override fields"
  *                       that was specified in ov_start().
  * @param in_value       Value to set the bits @a DST_MSB:@a DST_LSB to. May be a constant
  *                       or a register value.
  * @param SRC_MSB        Most significant bit within field @a in_value to use.
  * @param SRC_LSB        Least significant bit within field @a in_value to use.
  * @param FLAGS          Optional: Zero or more flags from @ref OverrideFlags "Override flags",
  *                       ORd together, which, for just this macro call, overrides the indirect
  *                       reference's flags.
  *
  * @note
  * Bits in the range @a SRC_MSB:@a SRC_LSB are extracted from in_value, and are placed in the
  * indicated field, i.e. in_value will be masked and
  * effectively shifted to the right by @a SRC_LSB bit positions before being used.
  *
  * @condition_codes Clobbered (in_value a register value) or unchanged (in_value a constant).
  *
  * @instruction_cnt Two or more (in_value a register value) or zero (in_value a constant).
  */
#macro ov_set_extract(OVERRIDE_TYPE, in_value, SRC_MSB, SRC_LSB, FLAGS)

    _ov_set(OVERRIDE_TYPE, in_value, 0, --, --, SRC_MSB, SRC_LSB, FLAGS, 0, 0, 0, 0)

#endm // ov_set_extract


/// @cond INTERNAL_MACROS // Not documentating this macro because similar to previous
#macro ov_set_extract(OVERRIDE_TYPE, in_value, SRC_MSB, SRC_LSB)

    _ov_set(OVERRIDE_TYPE, in_value, 0, --, --, SRC_MSB, SRC_LSB, 0, 0, 0, 0, 0)

#endm // ov_set_extract
/// @endcond


/**
  * Set the OV_BYTE_MASK field to the provided value and also
  * set the specific range of bits in the indicated field to the extracted range of bits
  * in the provided value, taking the provided flags into account.
  *
  * @note
  * This macro is functionally equivalent to the following, except that it produces more
  * efficient code:
  * @code
  * ov_set(OV_BYTE_MASK, in_byte_mask)
  * ov_set_extract(OVERRIDE_TYPE, DST_MSB, DST_LSB, in_value, SRC_MSB,
  *     SRC_LSB, FLAGS)
  * @endcode
  *
  * The data for the indicated field is extracted from @a in_value, meaning that bits set
  * in @a in_value outside the range @a SRC_MSB:@a SRC_LSB have no effect on the indirect
  * reference.
  *
  * @b Example:
  * @code
  * ov_start((OV_LENGTH | OV_BYTE_MASK | OV_DATA_REF | OV_DATA_MASTER |
  *     OV_SIGNAL_MASTER | OV_MASTER_ISLAND))
  * ov_set(OV_DATA_MASTER, 3, 2, 0)
  * ov_set_bm_and_extract(byte_mask, OV_DATA_MASTER, 1, 0, data_master, 5, 4)
  * ov_set(OV_DATA_REF, data_ref)
  * ov_set(OV_SIGNAL_MASTER, signal_master)
  * ov_set(OV_MASTER_ISLAND, mater_island)
  * ov_set(OV_LENGTH, len, OVF_SUBTRACT_ONE)
  * ov_use()
  * command[...], indirect_ref
  * ov_clean()
  * @endcode
  *
  * @param in_byte_mask   Value to be used for override field OV_BYTE_MASK.
  * @param OVERRIDE_TYPE  One override field from @ref OverrideFields "Override fields"
  *                       that was specified in ov_start().
  * @param DST_MSB        Most significant bit within field @a OVERRIDE_TYPE to set.
  * @param DST_LSB        Least significant bit within field @a OVERRIDE_TYPE to set.
  * @param in_value       Value to set the bits @a DST_MSB:@a DST_LSB to. May be a constant
  *                       or a register value.
  * @param SRC_MSB        Most significant bit within field @a in_value to use.
  * @param SRC_LSB        Least significant bit within field @a in_value to use.
  * @param FLAGS          Optional: Zero or more flags from @ref OverrideFlags "Override flags",
  *                       ORd together, which, for just this macro call, overrides the indirect
  *                       reference's flags.
  *
  * @note
  * Bits in the range @a SRC_MSB:@a SRC_LSB are extracted from in_value, and are placed in the
  * indicated field relative to @a DST_LSB and @a, i.e. in_value will be masked and
  * effectively shifted to the left by (@a DST_LSB - @a SRC_LSB) bit positions before being used.
  *
  * @condition_codes Clobbered (in_value a register value) or unchanged (in_value a constant).
  *
  * @instruction_cnt Two or more (in_value a register value) or zero (in_value a constant).
  */
#macro ov_set_bm_and_extract(in_byte_mask, OVERRIDE_TYPE, DST_MSB, DST_LSB, in_value, SRC_MSB, SRC_LSB, FLAGS)

    _ov_set(OVERRIDE_TYPE, in_value, 0, DST_MSB, DST_LSB, SRC_MSB, SRC_LSB, FLAGS, 1, in_byte_mask, 0, 0)

#endm // ov_set_bm_and_extract


/// @cond INTERNAL_MACROS // Not documentating this macro because similar to previous
#macro ov_set_bm_and_extract(in_byte_mask, OVERRIDE_TYPE, DST_MSB, DST_LSB, in_value, SRC_MSB, SRC_LSB)

    _ov_set(OVERRIDE_TYPE, in_value, 0, DST_MSB, DST_LSB, SRC_MSB, SRC_LSB, 0, 1, in_byte_mask, 0, 0)

#endm // ov_set_bm_and_extract
/// @endcond


/**
  * Set the OV_BYTE_MASK field to the provided value and also
  * set the value of the indicated field to the extracted range of bits
  * in the provided value, taking the provided flags into account.
  *
  * @note
  * This macro is functionally equivalent to the following, except that it produces more
  * efficient code:
  * @code
  * ov_set(OV_BYTE_MASK, in_byte_mask)
  * ov_set_extract(OVERRIDE_TYPE, in_value, SRC_MSB, SRC_LSB, FLAGS)
  * @endcode
  *
  * The data for the indicated field is extracted from @a in_value, meaning that bits set
  * in @a in_value outside the range @a SRC_MSB:@a SRC_LSB have no effect on the indirect
  * reference.
  *
  * @b Example:
  * @code
  * ov_start((OV_LENGTH | OV_BYTE_MASK | OV_DATA_REF | OV_DATA_MASTER
  *     | OV_SIGNAL_MASTER | OV_MASTER_ISLAND))
  * ov_set_bm_and_extract(byte_mask, OV_DATA_MASTER, data_master, 7, 4)
  * ov_set(OV_DATA_REF, data_ref)
  * ov_set(OV_SIGNAL_MASTER, signal_master)
  * ov_set(OV_MASTER_ISLAND, mater_island)
  * ov_set(OV_LENGTH, len, OVF_SUBTRACT_ONE)
  * ov_use()
  * command[...], indirect_ref
  * ov_clean()
  * @endcode
  *
  * @param in_byte_mask   Value to be used for override field OV_BYTE_MASK.
  * @param OVERRIDE_TYPE  One override field from @ref OverrideFields "Override fields"
  *                       that was specified in ov_start().
  * @param in_value       Value to set the bits @a DST_MSB:@a DST_LSB to. May be a constant
  *                       or a register value.
  * @param SRC_MSB        Most significant bit within field @a in_value to use.
  * @param SRC_LSB        Least significant bit within field @a in_value to use.
  * @param FLAGS          Optional: Zero or more flags from @ref OverrideFlags "Override flags",
  *                       ORd together, which, for just this macro call, overrides the indirect
  *                       reference's flags.
  *
  * @note
  * Bits in the range @a SRC_MSB:@a SRC_LSB are extracted from in_value, and are placed in the
  * indicated field, i.e. in_value will be masked and
  * effectively shifted to the right by @a SRC_LSB bit positions before being used.
  *
  * @condition_codes Clobbered (in_value a register value) or unchanged (in_value a constant).
  *
  * @instruction_cnt Two or more (in_value a register value) or zero (in_value a constant).
  */
#macro ov_set_bm_and_extract(in_byte_mask, OVERRIDE_TYPE, in_value, SRC_MSB, SRC_LSB, FLAGS)

    _ov_set(OVERRIDE_TYPE, in_value, 0, --, --, SRC_MSB, SRC_LSB, FLAGS, 1, in_byte_mask, 0, 0)

#endm // ov_set_bm_and_extract


/// @cond INTERNAL_MACROS // Not documentating this macro because similar to previous
#macro ov_set_bm_and_extract(in_byte_mask, OVERRIDE_TYPE, in_value, SRC_MSB, SRC_LSB)

    _ov_set(OVERRIDE_TYPE, in_value, 0, --, --, SRC_MSB, SRC_LSB, 0, 1, in_byte_mask, 0, 0)

#endm // ov_set_bm_and_extract
/// @endcond


/**
  * Set the specific range of bits in the indicated field to the provided value, taking
  * the provided flags into account,
  * and then populates CMD_INDIRECT_REF_0, if appropriate, and sets the value of
  * previous ALU output.
  *
  * @note
  * This macro is functionally equivalent to the following, except that it produces more
  * efficient code:
  * @code
  * ov_set(OVERRIDE_TYPE, DST_MSB, DST_LSB, in_value, FLAGS)
  * ov_use()
  * @endcode
  *
  * @b Example:
  * @code
  * ov_start(OV_LENGTH)
  * ov_set(OV_LENGTH, 4, 4, 0)
  * ov_set_use(OV_LENGTH, 3, 0, 1, OVF_RELAXED_ORDER_ON)
  * command[...], indirect_ref
  * ov_clean()
  * @endcode
  *
  * @param OVERRIDE_TYPE  One override field from @ref OverrideFields "Override fields"
  *                       that was specified in ov_start().
  * @param DST_MSB        Most significant bit within field @a OVERRIDE_TYPE to set.
  * @param DST_LSB        Least significant bit within field @a OVERRIDE_TYPE to set.
  * @param in_value       Value to set the bits @a DST_MSB:@a DST_LSB to. May be a constant
  *                       or a register value.
  * @param FLAGS          Optional: Zero or more flags from @ref OverrideFlags "Override flags",
  *                       ORd together, which, for just this macro call, overrides the indirect
  *                       reference's flags.
  *
  * @note
  * @a in_value is relative to @a DST_LSB, i.e. in_value will be shifted to the left
  * by @a DST_LSB before being used.
  *
  * @condition_codes Clobbered
  *
  * @instruction_cnt One to six
  */
#macro ov_set_use(OVERRIDE_TYPE, DST_MSB, DST_LSB, in_value, FLAGS)

    _ov_set(OVERRIDE_TYPE, in_value, 1, DST_MSB, DST_LSB, --, --, FLAGS, 0, 0, 0, 0)

#endm // ov_set_use


/// @cond INTERNAL_MACROS // Not documentating this macro because similar to previous
#macro ov_set_use(OVERRIDE_TYPE, DST_MSB, DST_LSB, in_value)

    _ov_set(OVERRIDE_TYPE, in_value, 1, DST_MSB, DST_LSB, --, --, 0, 0, 0, 0, 0)

#endm // ov_set_use
/// @endcond


/**
  * Set the indicated field to the provided value, taking the provided flags into account,
  * and then populates CMD_INDIRECT_REF_0, if appropriate, and sets the value of
  * previous ALU output.
  *
  * @note
  * This macro is functionally equivalent to the following, except that it produces more
  * efficient code:
  * @code
  * ov_set(OVERRIDE_TYPE, in_value, FLAGS)
  * ov_use()
  * @endcode
  *
  * @b Example:
  * @code
  * ov_start(OV_LENGTH)
  * ov_set_use(OV_LENGTH, 12, OVF_SUBTRACT_ONE)
  * command[...], indirect_ref
  * ov_clean()
  * @endcode
  *
  * @param OVERRIDE_TYPE  One override field from @ref OverrideFields "Override fields"
  *                       that was specified in ov_start().
  * @param in_value       Value to set field @a OVERRIDE_TYPE to. May be a constant
  *                       or a register value.
  * @param FLAGS          Optional: Zero or more flags from @ref OverrideFlags "Override flags",
  *                       ORd together, which, for just this macro call, overrides the indirect
  *                       reference's flags.
  *
  * @condition_codes Clobbered
  *
  * @instruction_cnt One to six
  */
#macro ov_set_use(OVERRIDE_TYPE, in_value, FLAGS)

    _ov_set(OVERRIDE_TYPE, in_value, 1, --, --, --, --, FLAGS, 0, 0, 0, 0)

#endm // ov_set_use


/// @cond INTERNAL_MACROS // Not documentating this macro because similar to previous
#macro ov_set_use(OVERRIDE_TYPE, in_value)

    _ov_set(OVERRIDE_TYPE, in_value, 1, --, --, --, --, 0, 0, 0, 0, 0)

#endm // ov_set_use
/// @endcond


/**
  * Set the OV_BYTE_MASK field to the provided value and also
  * set the specific range of bits in the indicated field to the provided value, taking
  * the provided flags into account,
  * and then populates CMD_INDIRECT_REF_0, if appropriate, and sets the value of
  * previous ALU output.
  *
  * @note
  * This macro is functionally equivalent to the following, except that it produces more
  * efficient code:
  * @code
  * ov_set(OV_BYTE_MASK, in_byte_mask)
  * ov_set(OVERRIDE_TYPE, DST_MSB, DST_LSB, in_value, FLAGS)
  * ov_use()
  * @endcode
  *
  * @b Example:
  * @code
  * ov_start((OV_LENGTH | OV_BYTE_MASK | OV_DATA_REF | OV_DATA_MASTER))
  * ov_set(OV_DATA_REF, data_ref)
  * ov_set(OV_LENGTH, len, OVF_SUBTRACT_ONE)
  * ov_set(OV_DATA_MASTER, 3, 2, 0)
  * ov_set_bm_and_use(byte_mask, OV_DATA_MASTER, 1, 0, data_master, OVF_PARANOID_ON)
  * command[...], indirect_ref
  * ov_clean()
  * @endcode
  *
  * @param in_byte_mask   Value to be used for override field OV_BYTE_MASK.
  * @param OVERRIDE_TYPE  One override field from @ref OverrideFields "Override fields"
  *                       that was specified in ov_start().
  * @param DST_MSB        Most significant bit within field @a OVERRIDE_TYPE to set.
  * @param DST_LSB        Least significant bit within field @a OVERRIDE_TYPE to set.
  * @param in_value       Value to set the bits @a DST_MSB:@a DST_LSB in @a OVERRIDE_TYPE to.
  *                       May be a constant or a register value.
  * @param FLAGS          Optional: Zero or more flags from @ref OverrideFlags "Override flags",
  *                       ORd together, which, for just this macro call, overrides the indirect
  *                       reference's flags.
  *
  * @note
  * The parameter @a in_byte_mask is used in its entirety as is. No provided parameters are
  * applicable to it.
  * \n\n
  * @note
  * @a in_value is relative to @a DST_LSB, i.e. in_value will be shifted to the left
  * by @a DST_LSB before being used.
  *
  * @condition_codes Clobbered
  *
  * @instruction_cnt One to six
  */
#macro ov_set_bm_and_use(in_byte_mask, OVERRIDE_TYPE, DST_MSB, DST_LSB, in_value, FLAGS)

    _ov_set(OVERRIDE_TYPE, in_value, 1, DST_MSB, DST_LSB, --, --, FLAGS, 1, in_byte_mask, 0, 0)

#endm // ov_set_bm_and_use


/// @cond INTERNAL_MACROS // Not documentating this macro because similar to previous
#macro ov_set_bm_and_use(in_byte_mask, OVERRIDE_TYPE, DST_MSB, DST_LSB, in_value)

    _ov_set(OVERRIDE_TYPE, in_value, 1, DST_MSB, DST_LSB, --, --, 0, 1, in_byte_mask, 0, 0)

#endm // ov_set_bm_and_use
/// @endcond


/**
  * Set the OV_BYTE_MASK field to the provided value and also
  * set the indicated field to the provided value, taking
  * the provided flags into account,
  * and then populates CMD_INDIRECT_REF_0, if appropriate, and sets the value of
  * previous ALU output.
  *
  * @note
  * This macro is functionally equivalent to the following, except that it produces more
  * efficient code:
  * @code
  * ov_set(OV_BYTE_MASK, in_byte_mask)
  * ov_set(OVERRIDE_TYPE, in_value, FLAGS)
  * ov_use()
  * @endcode
  *
  * @b Example:
  * @code
  * ov_start((OV_LENGTH | OV_BYTE_MASK | OV_DATA_REF | OV_DATA_MASTER))
  * ov_set(OV_LENGTH, len, OVF_SUBTRACT_ONE)
  * ov_set(OV_DATA_REF, data_ref)
  * ov_set_bm_and_use(byte_mask, OV_DATA_MASTER, data_master, OVF_PARANOID_ON)
  * command[...], indirect_ref
  * ov_clean()
  * @endcode
  *
  * @param in_byte_mask   Value to be used for override field OV_BYTE_MASK.
  * @param OVERRIDE_TYPE  One override field from @ref OverrideFields "Override fields"
  *                       that was specified in ov_start().
  * @param in_value       Value to set the field @a OVERRIDE_TYPE to. May be a constant
  *                       or a register value.
  * @param FLAGS          Optional: Zero or more flags from @ref OverrideFlags "Override flags",
  *                       ORd together, which, for just this macro call, overrides the indirect
  *                       reference's flags.
  *
  * @note
  * The parameter @a in_byte_mask is used in its entirety as is. No provided parameters are
  * applicable to it.
  *
  * @condition_codes Clobbered
  *
  * @instruction_cnt Two to six, or more, depending on flags
  */
#macro ov_set_bm_and_use(in_byte_mask, OVERRIDE_TYPE, in_value, FLAGS)

    _ov_set(OVERRIDE_TYPE, in_value, 1, --, --, --, --, FLAGS, 1, in_byte_mask, 0, 0)

#endm // ov_set_bm_and_use


/// @cond INTERNAL_MACROS // Not documentating this macro because similar to previous
#macro ov_set_bm_and_use(in_byte_mask, OVERRIDE_TYPE, in_value)

    _ov_set(OVERRIDE_TYPE, in_value, 1, --, --, --, --, 0, 1, in_byte_mask, 0, 0)

#endm // ov_set_bm_and_use
/// @endcond


/**
  * Set the specific range of bits in the indicated field to the extracted range of bits
  * in the provided value, taking the provided flags into account,
  * and then populates CMD_INDIRECT_REF_0, if appropriate, and sets the value of
  * previous ALU output.
  *
  * @note
  * This macro is functionally equivalent to the following, except that it produces more
  * efficient code:
  * @code
  * ov_set_extract(OVERRIDE_TYPE, DST_MSB, DST_LSB, in_value,
  *     SRC_MSB, SRC_LSB, FLAGS)
  * ov_use()
  * @endcode
  *
  * The data is extracted from @a in_value, meaning that bits set in @a in_value outside
  * the range @a SRC_MSB:@a SRC_LSB have no effect on the indirect reference.
  *
  * @b Example:
  * @code
  * ov_start(OV_LENGTH)
  * ov_set(OV_LENGTH, 4, 4, 0)
  * ov_set_extract_use(OV_LENGTH, 3, 0, value, 12, 9, OVF_RELAXED_ORDER_ON)
  * command[...], indirect_ref
  * ov_clean()
  * @endcode
  *
  * @param OVERRIDE_TYPE  One override field from @ref OverrideFields "Override fields"
  *                       that was specified in ov_start().
  * @param DST_MSB        Most significant bit within field @a OVERRIDE_TYPE to set.
  * @param DST_LSB        Least significant bit within field @a OVERRIDE_TYPE to set.
  * @param in_value       Value to set the bits @a DST_MSB:@a DST_LSB to. May be a constant
  *                       or a register value.
  * @param SRC_MSB        Most significant bit within field @a in_value to use.
  * @param SRC_LSB        Least significant bit within field @a in_value to use.
  * @param FLAGS          Optional: Zero or more flags from @ref OverrideFlags "Override flags",
  *                       ORd together, which, for just this macro call, overrides the indirect
  *                       reference's flags.
  *
  * @note
  * Bits in the range @a SRC_MSB:@a SRC_LSB are extracted from in_value, and are placed in the
  * indicated field relative to @a DST_LSB and @a, i.e. in_value will be masked and
  * effectively shifted to the left by (@a DST_LSB - @a SRC_LSB) bit positions before being used.
  *
  * @condition_codes Clobbered
  *
  * @instruction_cnt Two to six, or more, depending on flags
  */
#macro ov_set_extract_use(OVERRIDE_TYPE, DST_MSB, DST_LSB, in_value, SRC_MSB, SRC_LSB, FLAGS)

    _ov_set(OVERRIDE_TYPE, in_value, 1, DST_MSB, DST_LSB, SRC_MSB, SRC_LSB, FLAGS, 0, 0, 0, 0)

#endm // ov_set_extract_use


/// @cond INTERNAL_MACROS // Not documentating this macro because similar to previous
#macro ov_set_extract_use(OVERRIDE_TYPE, DST_MSB, DST_LSB, in_value, SRC_MSB, SRC_LSB)

    _ov_set(OVERRIDE_TYPE, in_value, 1, DST_MSB, DST_LSB, SRC_MSB, SRC_LSB, 0, 0, 0, 0, 0)

#endm // ov_set_extract_use
/// @endcond


/**
  * Set the value of the indicated field to the extracted range of bits
  * in the provided value, taking the provided flags into account,
  * and then populates CMD_INDIRECT_REF_0, if appropriate, and sets the value of
  * previous ALU output.
  *
  * @note
  * This macro is functionally equivalent to the following, except that it produces more
  * efficient code:
  * @code
  * ov_set_extract(OVERRIDE_TYPE, in_value, SRC_MSB, SRC_LSB, FLAGS)
  * ov_use()
  * @endcode
  *
  * The data is extracted from @a in_value, meaning that bits set in @a in_value outside
  * the range @a SRC_MSB:@a SRC_LSB have no effect on the indirect reference.
  *
  * @b Example:
  * @code
  * ov_start(OV_LENGTH)
  * ov_set_extract_use(OV_LENGTH, value, 13, 9)
  * command[...], indirect_ref
  * ov_clean()
  * @endcode
  *
  * @param OVERRIDE_TYPE  One override field from @ref OverrideFields "Override fields"
  *                       that was specified in ov_start().
  * @param in_value       Value to set the bits @a DST_MSB:@a DST_LSB to. May be a constant
  *                       or a register value.
  * @param SRC_MSB        Most significant bit within field @a in_value to use.
  * @param SRC_LSB        Least significant bit within field @a in_value to use.
  * @param FLAGS          Optional: Zero or more flags from @ref OverrideFlags "Override flags",
  *                       ORd together, which, for just this macro call, overrides the indirect
  *                       reference's flags.
  *
  * @note
  * Bits in the range @a SRC_MSB:@a SRC_LSB are extracted from in_value, and are placed in the
  * indicated field, i.e. in_value will be masked and
  * effectively shifted to the right by @a SRC_LSB bit positions before being used.
  *
  * @condition_codes Clobbered
  *
  * @instruction_cnt Two to six, or more, depending on flags
  */
#macro ov_set_extract_use(OVERRIDE_TYPE, in_value, SRC_MSB, SRC_LSB, FLAGS)

    _ov_set(OVERRIDE_TYPE, in_value, 1, --, --, SRC_MSB, SRC_LSB, FLAGS, 0, 0, 0, 0)

#endm // ov_set_extract_use


/// @cond INTERNAL_MACROS // Not documentating this macro because similar to previous
#macro ov_set_extract_use(OVERRIDE_TYPE, in_value, SRC_MSB, SRC_LSB)

    _ov_set(OVERRIDE_TYPE, in_value, 1, --, --, SRC_MSB, SRC_LSB, 0, 0, 0, 0, 0)

#endm // ov_set_extract_use
/// @endcond


/**
  * Set the OV_BYTE_MASK field to the provided value and also
  * set the specific range of bits in the indicated field to the extracted range of bits
  * in the provided value, taking the provided flags into account,
  * and then populates CMD_INDIRECT_REF_0, if appropriate, and sets the value of
  * previous ALU output.
  *
  * @note
  * This macro is functionally equivalent to the following, except that it produces more
  * efficient code:
  * @code
  * ov_set(OV_BYTE_MASK, in_byte_mask)
  * ov_set_extract(OVERRIDE_TYPE, DST_MSB, DST_LSB, in_value, SRC_MSB,
  *     SRC_LSB, FLAGS)
  * ov_use()
  * @endcode
  *
  * The data for the indicated field is extracted from @a in_value, meaning that bits set
  * in @a in_value outside the range @a SRC_MSB:@a SRC_LSB have no effect on the indirect
  * reference.
  *
  * @b Example:
  * @code
  * ov_start((OV_LENGTH | OV_BYTE_MASK | OV_DATA_REF | OV_DATA_MASTER))
  * ov_set(OV_DATA_MASTER, 3, 2, 0)
  * ov_set(OV_DATA_REF, data_ref)
  * ov_set(OV_LENGTH, len, OVF_SUBTRACT_ONE)
  * ov_set_bm_and_extract_use(byte_mask, OV_DATA_MASTER, 1, 0, data_master, 5, 4)
  * command[...], indirect_ref
  * ov_clean()
  * @endcode
  *
  * @param in_byte_mask   Value to be used for override field OV_BYTE_MASK.
  * @param OVERRIDE_TYPE  One override field from @ref OverrideFields "Override fields"
  *                       that was specified in ov_start().
  * @param DST_MSB        Most significant bit within field @a OVERRIDE_TYPE to set.
  * @param DST_LSB        Least significant bit within field @a OVERRIDE_TYPE to set.
  * @param in_value       Value to set the bits @a DST_MSB:@a DST_LSB to. May be a constant
  *                       or a register value.
  * @param SRC_MSB        Most significant bit within field @a in_value to use.
  * @param SRC_LSB        Least significant bit within field @a in_value to use.
  * @param FLAGS          Optional: Zero or more flags from @ref OverrideFlags "Override flags",
  *                       ORd together, which, for just this macro call, overrides the indirect
  *                       reference's flags.
  *
  * @note
  * Bits in the range @a SRC_MSB:@a SRC_LSB are extracted from in_value, and are placed in the
  * indicated field relative to @a DST_LSB and @a, i.e. in_value will be masked and
  * effectively shifted to the left by (@a DST_LSB - @a SRC_LSB) bit positions before being used.
  *
  * @condition_codes Clobbered
  *
  * @instruction_cnt Two to six, or more, depending on flags
  */
#macro ov_set_bm_and_extract_use(in_byte_mask, OVERRIDE_TYPE, DST_MSB, DST_LSB, in_value, SRC_MSB, SRC_LSB, FLAGS)

    _ov_set(OVERRIDE_TYPE, in_value, 1, DST_MSB, DST_LSB, SRC_MSB, SRC_LSB, FLAGS, 1, in_byte_mask, 0, 0)

#endm // ov_set_bm_and_extract_use


/// @cond INTERNAL_MACROS // Not documentating this macro because similar to previous
#macro ov_set_bm_and_extract_use(in_byte_mask, OVERRIDE_TYPE, DST_MSB, DST_LSB, in_value, SRC_MSB, SRC_LSB)

    _ov_set(OVERRIDE_TYPE, in_value, 1, DST_MSB, DST_LSB, SRC_MSB, SRC_LSB, 0, 1, in_byte_mask, 0, 0)

#endm // ov_set_bm_and_extract_use
/// @endcond


/**
  * Set the OV_BYTE_MASK field to the provided value and also
  * set the value of the indicated field to the extracted range of bits
  * in the provided value, taking the provided flags into account,
  * and then populates CMD_INDIRECT_REF_0, if appropriate, and sets the value of
  * previous ALU output.
  *
  * @note
  * This macro is functionally equivalent to the following, except that it produces more
  * efficient code:
  * @code
  * ov_set(OV_BYTE_MASK, in_byte_mask)
  * ov_set_extract(OVERRIDE_TYPE, in_value, SRC_MSB, SRC_LSB, FLAGS)
  * ov_use()
  * @endcode
  *
  * The data for the indicated field is extracted from @a in_value, meaning that bits set
  * in @a in_value outside the range @a SRC_MSB:@a SRC_LSB have no effect on the indirect
  * reference.
  *
  * @b Example:
  * @code
  * ov_start((OV_LENGTH | OV_BYTE_MASK | OV_DATA_REF | OV_DATA_MASTER))
  * ov_set(OV_DATA_REF, data_ref)
  * ov_set(OV_LENGTH, len, OVF_SUBTRACT_ONE)
  * ov_set_bm_and_extract_use(byte_mask, OV_DATA_MASTER, data_master, 7, 4)
  * command[...], indirect_ref
  * ov_clean()
  * @endcode
  *
  * @param in_byte_mask   Value to be used for override field OV_BYTE_MASK.
  * @param OVERRIDE_TYPE  One override field from @ref OverrideFields "Override fields"
  *                       that was specified in ov_start().
  * @param in_value       Value to set the bits @a DST_MSB:@a DST_LSB to. May be a constant
  *                       or a register value.
  * @param SRC_MSB        Most significant bit within field @a in_value to use.
  * @param SRC_LSB        Least significant bit within field @a in_value to use.
  * @param FLAGS          Optional: Zero or more flags from @ref OverrideFlags "Override flags",
  *                       ORd together, which, for just this macro call, overrides the indirect
  *                       reference's flags.
  *
  * @note
  * Bits in the range @a SRC_MSB:@a SRC_LSB are extracted from in_value, and are placed in the
  * indicated field, i.e. in_value will be masked and
  * effectively shifted to the right by @a SRC_LSB bit positions before being used.
  *
  * @condition_codes Clobbered
  *
  * @instruction_cnt Two to six, or more, depending on flags
  */
#macro ov_set_bm_and_extract_use(in_byte_mask, OVERRIDE_TYPE, in_value, SRC_MSB, SRC_LSB, FLAGS)

    _ov_set(OVERRIDE_TYPE, in_value, 1, --, --, SRC_MSB, SRC_LSB, FLAGS, 1, in_byte_mask, 0, 0)

#endm // ov_set_bm_and_extract_use


/// @cond INTERNAL_MACROS // Not documentating this macro because similar to previous
#macro ov_set_bm_and_extract_use(in_byte_mask, OVERRIDE_TYPE, in_value, SRC_MSB, SRC_LSB)

    _ov_set(OVERRIDE_TYPE, in_value, 1, --, --, SRC_MSB, SRC_LSB, 0, 1, in_byte_mask, 0, 0)

#endm // ov_set_bm_and_extract_use
/// @endcond


/// @cond INTERNAL_MACROS
#macro ov_set_command(UNIT, COMMAND)

    // Just a demonstration version of this command - the contents of this must actually be
    // programmatically extracted from the source description of each command, and probably
    // put into another file for maintainability purposes.

    #if (streq(UNIT, 'mu'))

        // Memory Unit - data derived from "EAS_memory_unit.pdf" from
        // sftp://mahome.netronome.com/tools/thornham_docbook/output

        #if (streq(COMMAND, 'cam512_lookup8'))

            ov_set_bits(OV_LENGTH, 0b11111, 0b01100)

        #elif (streq(COMMAND, 'cam512_lookup16'))

            ov_set_bits(OV_LENGTH, 0b11111, 0b01101)

        #else

            #error "Command" COMMAND "within unit "UNIT" is unknown - this macro may need to be expanded"

        #endif

    #else

        #error "Unit" UNIT "is unknown - this macro may need to be expanded"

    #endif

#endm
/// @endcond


/**
  * Use the current indirect reference, in accordance with the provided flags.
  *
  * This macro populates CMD_INDIRECT_REF_0, if appropriate, and then sets the value of
  * previous ALU output.
  *
  * @note
  * The next instruction after calling this macro must be the instruction that
  * requires the indirect reference.
  *
  * @b Example:
  * @code
  * ov_start(OV_LENGTH, OVF_REUSE_ON)
  * ov_set(OV_LENGTH, 12, OVF_SUBTRACT_ONE)
  * ov_use()
  * command[...], indirect_ref
  * ov_use(OVF_NO_SWAP_SINCE_USE)
  * command[...], indirect_ref
  * ov_clean()
  * @endcode
  *
  * @param FLAGS  Optional: The only flag that may be specified is OVF_NO_SWAP_SINCE_USE.
  *               This flag may only be specified on the second or subsequent
  *               use of the current indirect reference.
  *
  * @condition_codes Clobbered
  *
  * @instruction_cnt One to six
  */
#macro ov_use(FLAGS)
.begin

    #ifndef _OV_PAR_REQUESTED

        #error "ov_start() and ov_set() must be called before ov_use() may be used"

    #endif

    #ifdef _OVF_LOCAL

        #error "_OVF_LOCAL has already been defined!"

    #endif

    #if (((FLAGS & (~_OV_FLAG_BITS(_OVF_NO_SWAP_SINCE_USE_pair))) != 0))

        #warning "Only flag OVF_NO_SWAP_SINCE_USE may be specified for ov_use()"

    #endif

    #define_eval _OVF_LOCAL _OVF_CURRENT // Flags currently applicable

    // Clear global flags where local flags are enabled
    #define_eval _OVF_LOCAL (_OVF_LOCAL & (~((FLAGS & _OVF_MASK_ENABLED) | ((FLAGS & _OVF_MASK_ENABLED) >> 1))))

    // Clear global flags where local flags are disabled
    #define_eval _OVF_LOCAL (_OVF_LOCAL & (~((FLAGS & _OVF_MASK_DISABLED) | ((FLAGS & _OVF_MASK_DISABLED) << 1))))

    // Add local flags
    #define_eval _OVF_LOCAL (_OVF_LOCAL | FLAGS)

    // If a flag is not set, must set to off, so that a flag is deterministically on or off
    #define_eval _OVF_LOCAL (_OVF_LOCAL | ((~_OVF_LOCAL & _OVF_MASK_ENABLED) >> 1))

    _ov_display_flags("Flags for current call of ov_use()", _OVF_LOCAL)

    // Check that context swap is still not happening
    #if (_OVF_LOCAL & OVF_NO_CTX_SWAP_OFF)

        // Context swap is no longer guaranteed to not happen, so forget about recently writing CMD_INDIRECT_REF_0
        #define_eval _OVF_STATUS (_OVF_STATUS & (~_OVF_STATUS_IND_REF_RECENTLY_WRITTEN))

    #endif

    #if (_OVF_STATUS & _OVF_STATUS_USED)

        // Has already been used, so set reused
        #define_eval _OVF_STATUS (_OVF_STATUS | _OVF_STATUS_REUSED)

    #endif

    _ov_check_all_data_provided(_OVF_LOCAL) // Produces #error if all required data has not yet been provided

    #if ((_OVF_LOCAL & OVF_NO_SWAP_SINCE_USE) && \
         (!(_OVF_STATUS & _OVF_STATUS_IND_REF_PREVIOUSLY_WRITTEN)))

        #error "The flag OVF_NO_SWAP_SINCE_USE may only be used at the second (or later) use of ov_*use(), not the first."

    #endif

    #if (((_OVF_LOCAL & OVF_NO_SWAP_SINCE_USE) && (!(_OVF_STATUS & _OVF_STATUS_IND_REF_PREVIOUSLY_WRITTEN))) || \
         ((!(_OVF_LOCAL & OVF_NO_SWAP_SINCE_USE)) && \
            (_OVF_LOCAL & OVF_NO_CTX_SWAP_OFF) || \
            ((_OVF_LOCAL & OVF_NO_CTX_SWAP_ON) && (!(_OVF_STATUS & _OVF_STATUS_IND_REF_RECENTLY_WRITTEN)))))

        // CMD_INDIRECT_REF_0 must be written because at least one of the following is true:
        // - Flag OVF_NO_SWAP_SINCE_USE is set and _OVF_STATUS_IND_REF_PREVIOUSLY_WRITTEN is clear
        // - Flag OVF_NO_SWAP_SINCE_USE is clear AND
        //   (Flag OVF_NO_CTX_SWAP_OFF is set OR
        //     (Flag OVF_NO_CTX_SWAP_ON is set AND _OVF_STATUS_IND_REF_RECENTLY_WRITTEN is clear))

        #if (_OV_IND_CSR_MASK_REQUIRED != 0)

            #if (_OVF_STATUS & _OVF_STATUS_IND_CSR_SRC_USED)

                // There is a register component
                #if (_OV_IND_CSR_CONST != 0)

                    // Constant is nonzero, so must incorporate into _ov_reg_ind_csr_/**/_OV_REG_IND_DST
                    _ov_alu_shf_reg_const(_ov_reg_ind_csr_/**/_OV_REG_IND_DST, _ov_reg_ind_csr_/**/_OV_REG_IND_SRC, OR, _OV_IND_CSR_CONST) ; ov_use() // [asm]
                    #define_eval _OV_REG_IND_SRC _OV_REG_IND_DST // Now refer to DST version!
                    #define_eval _OVF_STATUS (_OVF_STATUS | (_OVF_STATUS_IND_CSR_DST_USED | _OVF_STATUS_IND_CSR_SRC_USED))

                    #define_eval _OV_IND_CSR_CONST 0 // Do not process this constant value again

                #endif

                // Populate CMD_INDIRECT_REF_0, as final is set
                local_csr_wr[CMD_INDIRECT_REF_0, _ov_reg_ind_csr_/**/_OV_REG_IND_SRC] ; ov_use() // [asm]

            #else

                // Value for Indirect CSR stored only in constant
                #if ((_OV_IND_CSR_CONST & (~MAX_IMMEDIATE)) == 0)

                    // Can use immediate constant
                    local_csr_wr[CMD_INDIRECT_REF_0, _OV_IND_CSR_CONST] ; ov_use() // [asm]

                #else

                    // Cannot use immediate constant
                    _ov_alu_shf_reg_const(_ov_reg_ind_csr_/**/_OV_REG_IND_DST, --, B, _OV_IND_CSR_CONST) ; ov_use() // [asm] Optimal instruction(s) determined by constant's value
                    #define_eval _OV_REG_IND_SRC _OV_REG_IND_DST // Now refer to DST version!
                    #define_eval _OVF_STATUS (_OVF_STATUS | (_OVF_STATUS_IND_CSR_DST_USED | _OVF_STATUS_IND_CSR_SRC_USED))

                    #define_eval _OV_IND_CSR_CONST 0 // Do not process this constant value again

                    local_csr_wr[CMD_INDIRECT_REF_0, _ov_reg_ind_csr_/**/_OV_REG_IND_SRC] ; ov_use() // [asm]

                #endif

            #endif

            #define_eval _OVF_STATUS (_OVF_STATUS | _OVF_STATUS_IND_REF_PREVIOUSLY_WRITTEN)
            #define_eval _OVF_STATUS (_OVF_STATUS | _OVF_STATUS_IND_REF_RECENTLY_WRITTEN)

        #endif

    #endif

    #if (!(_OVF_STATUS & _OVF_STATUS_ALU_SRC_USED))

        // No register component, so must use constant (previous ALU value constant MUST be present!)
        #if (!(defined(_OV_PREVIOUS_ALU_TO_REGISTER)))
            _ov_alu_shf_reg_const(--, --, B, _OV_ALU_CONST) ; ov_use() // [asm] Optimal instruction(s) determined by constant's value
        #else
            _ov_alu_shf_reg_const(_ov_previous_alu_value, --, B, _OV_ALU_CONST) ; ov_use() code modified for testability // [asm] Optimal instruction(s) determined by constant's value
        #endif

    #else

        #if (_OV_ALU_CONST != 0)

            // Possible to get here were previous ALU has been defined and constant is nonzero, if parameters were
            // not provided in optimal order, which is legal if flag OVF_RELAXED_ORDER_ON is specified.

            // Still need to combine constant with _ov_reg_alu_/**/_OV_REG_ALU_SRC...
            _ov_alu_shf_reg_const(_ov_reg_alu_/**/_OV_REG_ALU_DST, _ov_reg_alu_/**/_OV_REG_ALU_SRC, OR, _OV_ALU_CONST) ; ov_use() // [asm] Optimal instruction(s) determined by constant's value
            #if (defined(_OV_PREVIOUS_ALU_TO_REGISTER))
                alu[_ov_previous_alu_value, --, B, _ov_reg_alu_/**/_OV_REG_ALU_DST] ; ov_use() test code // [asm]
            #endif
            #define_eval _OV_REG_ALU_SRC _OV_REG_ALU_DST // Now refer to DST version!
            #define_eval _OVF_STATUS (_OVF_STATUS | (_OVF_STATUS_ALU_DST_USED | _OVF_STATUS_ALU_SRC_USED))

            #define_eval _OV_ALU_CONST 0 // Value of constant has now been processed

        #else

            #if (!(defined(_OV_PREVIOUS_ALU_TO_REGISTER)))
                alu[--, --, B, _ov_reg_alu_/**/_OV_REG_ALU_SRC] ; ov_use() // [asm]
            #else
                alu[_ov_previous_alu_value, --, B, _ov_reg_alu_/**/_OV_REG_ALU_SRC] ; ov_use() code modified for testability // [asm]
            #endif

        #endif

    #endif

    #undef _OVF_LOCAL

.end
#endm // ov_use


/// @cond INTERNAL_MACROS
#macro ov_use()

    ov_use(0)

#endm // ov_use
/// @endcond


/// @cond INTERNAL_MACROS
#macro _ov_clean(CHECK_USE)

    #if (CHECK_USE)

        #if defined(_OVF_STATUS)

            // First time ov_clean() is being called, so check that indirect reference has been used at least once and possibly reused, depending on flags

            #if (!(_OVF_STATUS & _OVF_STATUS_USED))

                #warning "Current indirect reference has not yet been used."

            #elif (_OVF_CURRENT & OVF_REUSE_ON)

                #if ((!(_OVF_STATUS & _OVF_STATUS_REUSED)) && (_OVF_STATUS & _OVF_STATUS_ALWAYS_REUSE))

                    #warning "Current indirect reference has not yet been reused. To prevent this warning, do NOT specify flag OVF_REUSE_ON or specify flag OVF_REUSE_OFF to generate more efficient code."

                #endif

            #endif

        #else

            #error "ov_clean() must be called exactly once after ov_start(), ov_recall() or ov_resume(), when ov_store() is not called."

        #endif

    #endif

    #ifdef _OV_ROW_PAR

        #undef _OV_ROW_PAR

    #endif

    #ifdef _OV_PAR_COUNT

        #undef _OV_PAR_COUNT

    #endif

    #ifdef _OV_IND_CSR_MASK_PROVIDED

        #undef _OV_IND_CSR_MASK_PROVIDED

    #endif

    #ifdef _OV_IND_CSR_MASK_REQUIRED

        #undef _OV_IND_CSR_MASK_REQUIRED

    #endif

    #ifdef _OV_ALU_MASK_PROVIDED

        #undef _OV_ALU_MASK_PROVIDED

    #endif

    #ifdef _OV_ALU_MASK_REQUIRED

        #undef _OV_ALU_MASK_REQUIRED

    #endif

    #ifdef _OV_IND_CSR_CONST

        #undef _OV_IND_CSR_CONST

    #endif

    #ifdef _OV_ALU_CONST

        #undef _OV_ALU_CONST

    #endif

    #ifdef _OVF_STATUS

        #undef _OVF_STATUS

    #endif

    #ifdef _OVF_CURRENT

        #undef _OVF_CURRENT

    #endif

    #ifdef _OV_PAR_REQUESTED

        #undef _OV_PAR_REQUESTED

    #endif

#endm // _ov_clean
/// @endcond


/**
  * Release resources used by the current indirect reference.
  *
  * After calling this macro, the indirect reference no longer exists.
  *
  * @b Example:
  * @code
  * ov_start(OV_LENGTH)
  * ov_set(OV_LENGTH, 12, OVF_SUBTRACT_ONE)
  * ov_use()
  * command[...], indirect_ref
  * ov_clean()
  * @endcode
  *
  * @condition_codes Unchanged
  *
  * @instruction_cnt None - only preprocessor
  */
#macro ov_clean()

    _ov_clean(1)

#endm // _ov_clean


/**
  * Shorthand for an indirect reference containing a single field.
  *
  * @param OVERRIDE_TYPE   One override field from @ref OverrideFields
  *                        "Override fields".
  * @param in_value        Value to set field @a OVERRIDE_TYPE to. May be a constant
  *                        or a register value.
  * @param FLAGS           Optional: Zero or more flags from @ref OverrideFlags "Override flags",
  *                        ORd together, which, for just this macro call, overrides the indirect
  *                        reference's flags.
  *
  * The following shorthand:
  *
  * @code
  * ov_single(OV_LENGTH, 12, OVF_SUBTRACT_ONE)
  * command[...], indirect_ref
  * @endcode
  *
  * is equivalent to
  *
  * @code
  * ov_start(OV_LENGTH)
  * ov_set_use(OV_LENGTH, 12, OVF_SUBTRACT_ONE)
  * ov_clean()
  * command[...], indirect_ref
  * @endcode
  */
#macro ov_single(OVERRIDE_TYPE, in_value, FLAGS)

    ov_start(OVERRIDE_TYPE)
    ov_set_use(OVERRIDE_TYPE, in_value, FLAGS)
    ov_clean()

#endm // ov_single


/// @cond INTERNAL_MACROS
#macro ov_single(OVERRIDE_TYPE, in_value)

    ov_single(OVERRIDE_TYPE, in_value, 0)

#endm
/// @endcond


/// @cond INTERNAL_MACROS
#macro _ov_serialise_state()

    // Takes the current state of the indirect reference and writes it to preprocessor
    // variable OV_STATE

    #define_eval OV_STATE '1,' // 1 is version of map
    #define_eval OV_STATE 'OV_STATE/**/_OV_PAR_REQUESTED,'
    #define_eval OV_STATE 'OV_STATE/**/_OV_REG_ALU_DST,' // Set of registers containing incremental data
    #define_eval OV_STATE 'OV_STATE/**/_OV_REG_IND_DST,' // Set of registers containing incremental data
    #define_eval OV_STATE 'OV_STATE/**/_OV_ALU_CONST,'
    #define_eval OV_STATE 'OV_STATE/**/_OV_IND_CSR_CONST,'
    #define_eval OV_STATE 'OV_STATE/**/_OV_ALU_MASK_REQUIRED,'
    #define_eval OV_STATE 'OV_STATE/**/_OV_ALU_MASK_PROVIDED,'
    #define_eval OV_STATE 'OV_STATE/**/_OV_IND_CSR_MASK_REQUIRED,'
    #define_eval OV_STATE 'OV_STATE/**/_OV_IND_CSR_MASK_PROVIDED,'
    #define_eval OV_STATE 'OV_STATE/**/_OVF_CURRENT,'
    #define_eval OV_STATE 'OV_STATE/**/_OVF_STATUS,'
    #define_eval OV_STATE 'OV_STATE/**/_OV_PAR_COUNT,'
    #define_eval OV_STATE 'OV_STATE/**/s/**/_OV_ROW_PAR' // "s" is to keep leading zeros!

#endm // _ov_serialise_state
/// @endcond


/// @cond INTERNAL_MACROS
#macro _ov_deserialise_state(OV_STATE)

    // Takes the preprocessor variable OV_STATE and populates the current indirect
    // reference state

    _ov_va_start(OV_STATE)

    _ov_va_arg_index(0)
    #define _OV_MAP_VERSION (_OV_VA_ARG)

    // Copy stored state to current state, if known format
    #if ((_OV_VA_NUM == 14) && (_OV_MAP_VERSION == 1))

        _ov_va_arg_index(1)
        #define_eval _OV_PAR_REQUESTED (_OV_VA_ARG)

        _ov_va_arg_index(2)
        #define_eval _OV_REG_ALU_SRC (_OV_VA_ARG) // Previous destination now becomes source

        _ov_va_arg_index(3)
        #define_eval _OV_REG_IND_SRC (_OV_VA_ARG) // Previous destination now becomes source

        _ov_va_arg_index(4)
        #define_eval _OV_ALU_CONST (_OV_VA_ARG)

        _ov_va_arg_index(5)
        #define_eval _OV_IND_CSR_CONST (_OV_VA_ARG)

        _ov_va_arg_index(6)
        #define_eval _OV_ALU_MASK_REQUIRED (_OV_VA_ARG)

        _ov_va_arg_index(7)
        #define_eval _OV_ALU_MASK_PROVIDED (_OV_VA_ARG)

        _ov_va_arg_index(8)
        #define_eval _OV_IND_CSR_MASK_REQUIRED (_OV_VA_ARG)

        _ov_va_arg_index(9)
        #define_eval _OV_IND_CSR_MASK_PROVIDED (_OV_VA_ARG)

        _ov_va_arg_index(10)
        #define_eval _OVF_CURRENT (_OV_VA_ARG)

        _ov_va_arg_index(11)
        #define_eval _OVF_STATUS (_OV_VA_ARG)

        _ov_va_arg_index(12)
        #define_eval _OV_PAR_COUNT (_OV_VA_ARG)

        _ov_va_arg_index(13)
        #define_eval _OV_ROW_PAR (strright(_OV_VA_ARG, -1))

    #else

        #error "Invalid state / unknown map version"

    #endif

    #undef _OV_MAP_VERSION

    _ov_va_end()

#endm // _ov_deserialise_state
/// @endcond


/**
  * Store the current indirect reference for later use.
  *
  * After calling this macro, the indirect reference no longer exists.
  *
  * @note
  * After calling this macro, the number of the slot in which the indirect
  * reference has been stored is available in the preprocessor define OV_SLOT.
  * This must be stored to be able to recall / resume the indirect reference
  * later.
  * \n\n
  * @note
  * After calling this macro, a new indirect reference may be started using
  * ov_start(), or a previously stored macro may be resumed using ov_resume()
  * or recalled using ov_recall().
  *
  * @b Example:
  * @code
  * ov_start(OV_LENGTH)
  * ov_set(OV_LENGTH, 12, OVF_SUBTRACT_ONE)
  * ov_store()
  * #define_eval IND_REF_ONE OV_SLOT
  * // ...
  * ov_resume(IND_REF_ONE)
  * ov_use()
  * command[...], indirect_ref
  * ov_clean()
  * @endcode
  *
  * @see @ref ResumingAndRecalling "Resuming and recalling"
  *
  * @condition_codes Unchanged
  *
  * @instruction_cnt Zero to two, depending on whether registers have been been implicitly copied or not
  */
#macro ov_store()

    // Check that there is a state to save
    #ifndef _OV_PAR_REQUESTED

        #error "ov_store() must be called after ov_start() / ov_recall() / ov_resume() and is called instead of ov_clean()!"

    #endif

    #if ((_OVF_STATUS & _OVF_STATUS_ALU_SRC_USED) && (!(_OVF_STATUS & _OVF_STATUS_ALU_DST_USED)))

        // ALU SRC is used, but NOT destination
        // In order to make what we are storing work correctly, we have to copy from SRC to DST to
        // create an independent deep copy
        move(_ov_reg_alu_/**/_OV_REG_ALU_DST, _ov_reg_alu_/**/_OV_REG_ALU_SRC) ; ov_store() // [asm]
        #define_eval _OV_REG_ALU_SRC _OV_REG_ALU_DST // Now refer to DST version!
        #define_eval _OVF_STATUS (_OVF_STATUS | (_OVF_STATUS_ALU_DST_USED | _OVF_STATUS_ALU_SRC_USED))

    #endif

    #if ((_OVF_STATUS & _OVF_STATUS_IND_CSR_SRC_USED) && (!(_OVF_STATUS & _OVF_STATUS_IND_CSR_DST_USED)))

        // IND CSR SRC is used, but NOT destination
        // In order to make what we are storing work correctly, we have to copy from SRC to DST to
        // create an independent deep copy
        move(_ov_reg_ind_csr_/**/_OV_REG_IND_DST, _ov_reg_ind_csr_/**/_OV_REG_IND_SRC) ; ov_store() // [asm]
        #define_eval _OV_REG_IND_SRC _OV_REG_IND_DST // Now refer to DST version!
        #define_eval _OVF_STATUS (_OVF_STATUS | (_OVF_STATUS_IND_CSR_DST_USED | _OVF_STATUS_IND_CSR_SRC_USED))

    #endif

    // Find the next available slot
    #define_eval OV_SLOT 0

    #while ((OV_SLOT < _OV_STORE_STATES_COUNT) && (_OV_STORE_USED & (1 << OV_SLOT)))

        #define_eval OV_SLOT (OV_SLOT + 1)

    #endloop

    #if (OV_SLOT >= _OV_STORE_STATES_COUNT)

        #error "There are no more slots available within which to store the state!"

    #endif

    // Store the current state in this slot
    _ov_serialise_state()

    #if (OV_SLOT == 0)
        #define_eval _OV_STORE_STATE_0 'OV_STATE'
    #elif (OV_SLOT == 1)
        #define_eval _OV_STORE_STATE_1 'OV_STATE'
    #elif (OV_SLOT == 2)
        #define_eval _OV_STORE_STATE_2 'OV_STATE'
    #elif (OV_SLOT == 3)
        #define_eval _OV_STORE_STATE_3 'OV_STATE'
    #elif (OV_SLOT == 4)
        #define_eval _OV_STORE_STATE_4 'OV_STATE'
    #elif (OV_SLOT == 5)
        #define_eval _OV_STORE_STATE_5 'OV_STATE'
    #elif (OV_SLOT == 6)
        #define_eval _OV_STORE_STATE_6 'OV_STATE'
    #elif (OV_SLOT == 7)
        #define_eval _OV_STORE_STATE_7 'OV_STATE'
    #endif

    #if (_OV_STORE_STATES_COUNT != 8)
        #error "Implementation of ov_store() does not match value of _OV_STORE_STATES_COUNT"
    #endif

    #define_eval _OV_STORE_USED (_OV_STORE_USED | (1 << OV_SLOT))

    _ov_clean(0) // Specifically do NOT check use, as this indirect reference is being stored.

#endm // ov_store


/// @cond INTERNAL_MACROS
#macro _ov_load_state(SLOT)

    // Check that there is not already a state
    #ifdef _OV_PAR_REQUESTED

        #error "ov_recall() may only be called after ov_store() or ov_clean()!"

    #endif

    // Check that the provided slot number is legal
    #if (!((0 <= OV_SLOT) && (OV_SLOT < _OV_STORE_STATES_COUNT)))

        #error "Slot "SLOT" is not a legal slot number."

    #endif

    // Check that there is data at the indicated slot
    #if (!(_OV_STORE_USED & (1 << SLOT)))

        #error "Slot "SLOT" does not currently contain data."

    #endif

    // Recall the specific state
    #define_eval _OV_STORE_STATE_READ '_OV_STORE_STATE_/**/SLOT'
    _ov_deserialise_state("_OV_STORE_STATE_READ")
    #undef _OV_STORE_STATE_READ

    #if (_OV_STORE_STATES_COUNT != 8)
        #error "Implementation of ov_store() does not match value of _OV_STORE_STATES_COUNT"
    #endif

#endm // _ov_load_state
/// @endcond


/**
  * Recall a previously-stored indirect reference for use now.
  *
  * Recalling makes the previously-stored indirect reference the currently
  * active indirect reference, <b>but without influencing the stored indirect
  * reference</b>.
  *
  * @note
  * There may be more than one ov_recall() corresponding to ov_store() for
  * a specific slot.
  * \n\n
  * @note
  * When the stored indirect reference is no longer required, ov_destroy()
  * must be called.
  *
  * @b Example:
  * @code
  * ov_start((OV_LENGTH | OV_SEQ_NUM))
  * ov_set(OV_LENGTH, 16, OVF_SUBTRACT_ONE)
  * ov_store()
  * #define_eval REF_LENGTH_SEQ_NUM OV_SLOT
  *
  * #define COUNT 0
  * #while (COUNT < 4)
  *
  *     ov_recall(REF_LENGTH_SEQ_NUM)
  *     ov_set_use(OV_SEQ_NUM, COUNT)
  *     command[...], indirect_ref
  *     ov_clean()
  *
  * #define_eval COUNT (COUNT + 1)
  * #endloop
  *
  * ov_dispose(REF_LENGTH_SEQ_NUM)
  * @endcode
  *
  * @param SLOT    The slot number that was returned in OV_SLOT by ov_store().
  *
  * @see @ref ResumingAndRecalling "Resuming and recalling"
  *
  * @condition_codes Unchanged
  *
  * @instruction_cnt None - only preprocessor
  */
#macro ov_recall(SLOT)

    // This macro recalls the indirect reference calculation previously stored
    // in slot SLOT by ov_store(), in such a way that it can be resumed from
    // the point it was at when ov_store() was called, multiple times, until
    // ov_dispose(SLOT) is called.

    // Recall details for this particular slot
    _ov_load_state(SLOT)

    #define_eval _OV_STORE_REGISTER_SET (_OV_STORE_REGISTER_SET + 1)
    // The above results in _OV_REG_ALU_DST and _OV_REG_IND_DST being assigned in _ov_declare_global()

    // Prepare a new set of destination global variables
    _ov_declare_global()

    // Because we're recalling, the new copy is independent of the old copy.
    // This means that we must NOT consider the new destination registers as
    // being used at this stage!
    #define_eval _OVF_STATUS (_OVF_STATUS & (~(_OVF_STATUS_ALU_DST_USED | _OVF_STATUS_IND_CSR_DST_USED)))

#endm // ov_recall


/**
  * Resume a previously-stored indirect reference.
  *
  * Resuming carries on with the previously-stored indirect reference, as though
  * nothing had occurred in the interim.
  *
  * @note
  * There may be exactly one ov_resume() corresponding to ov_store() for
  * a specific slot.
  * \n\n
  * @note
  * ov_resume() implicitly destroys the stored indirect reference.
  *
  * @b Example:
  * @code
  * // First indirect reference starts
  * ov_start((OV_BYTE_MASK | OV_LENGTH))
  * ov_set(OV_BYTE_MASK, byte_mask)
  * ov_store()
  * #define_eval REF_BYTE_MASK_AND_LENGTH OV_SLOT // Store slot number for later
  *
  * // Second indirect reference starts
  * ov_start((OV_DATA_REF | OV_BYTE_MASK))
  * ov_set(OV_BYTE_MASK, byte_mask)
  * ov_store()
  * #define_eval REF_DATA_REF_AND_BYTE_MASK OV_SLOT // Store slot number for later
  *
  * // First indirect reference resumed
  * ov_resume(REF_BYTE_MASK_AND_LENGTH)
  * ov_set_use(OV_LENGTH, len, OVF_SUBTRACT_ONE)
  * command[...], indirect_ref
  * ov_clean()
  *
  * // Second indirect reference resumed
  * ov_resume(REF_DATA_REF_AND_BYTE_MASK)
  * ov_set_use(OV_DATA_REF, data)
  * command[...], indirect_ref
  * ov_clean()
  * @endcode
  *
  * @param SLOT    The slot number that was returned in OV_SLOT by ov_store().
  *
  * @see @ref ResumingAndRecalling "Resuming and recalling"
  *
  * @condition_codes Unchanged
  *
  * @instruction_cnt None - only preprocessor
  */
#macro ov_resume(SLOT)

    // This macro recalls the indirect reference calculation previously stored
    // in slot SLOT by ov_store(), in such a way that the indirect reference
    // calculation can be continued from the point when ov_store() was called.
    // ov_resume(SLOT) may only be called once (it implicitly calls
    // ov_dispose(SLOT)); to later resume again, ov_store() must again be called
    // followed by ov_resume() (with potentially a new slot number).

    // Recall details for this particular slot
    _ov_load_state(SLOT)

    // Because we're resuming, destination is the same as source, retrieved above
    #define_eval _OV_REG_ALU_DST _OV_REG_ALU_SRC
    #define_eval _OV_REG_IND_DST _OV_REG_IND_SRC

    // Global variables already exist, so no call to _ov_declare_global()

    // Because we're resuming, the new set of registers are equivalent to the
    // old set, so the used state of the source and destination registers must
    // be unchanged.

    // Because we're resuming, destroy the state in this slot
    ov_dispose(SLOT)

#endm // ov_resume


/**
  * Destroy the indirect reference previously stored at the indicated slot.
  *
  * After a previously-stored indirect reference has been destroyed, it can
  * no longer be referred to.
  *
  * @note
  * If an indirect reference is recalled using ov_recall(), the only way to
  * release the resource is via ov_dipose().
  *
  * @param SLOT    The slot number that was returned in OV_SLOT by ov_store().
  *
  * @see @ref ResumingAndRecalling "Resuming and recalling"
  *
  * @condition_codes Unchanged
  *
  * @instruction_cnt None - only preprocessor
  */
#macro ov_dispose(SLOT)

    // This macro disposes of slot SLOT and all associated data.
    // After this, ov_recall(SLOT) and ov_resume(SLOT) are no longer allowed.

    #if (!((0 <= OV_SLOT) && (OV_SLOT < _OV_STORE_STATES_COUNT)))

        #error "Slot "SLOT" is not a legal slot number."

    #endif

    #if (!(_OV_STORE_USED & (1 << SLOT)))

        #error "Slot "SLOT" is not currently used, and so cannot be disposed of."

    #endif

    #define_eval _OV_STORE_USED (_OV_STORE_USED & (~(1 << SLOT)))

#endm // ov_dispose


// The following two macros can be used to extract the value that will be used for previous ALU output
// and CMD_INDIRECT_REF_0. These macros must be called BEFORE ov_clean(), and may be called before
// ov_use(), but NOT before ov_set_use(), unless partially-calculated data is desired. For example:
// ov_start(OV_LENGTH)
// ov_set_use(OV_LENGTH, 32, OVF_SUBTRACT_ONE)
// cls[set_imm, --, a1, a2, 0], indirect_ref ; Consume indirect reference to prevent warning 5008 - register {x} is set but not used
// _ov_extract_prev_alu(out_prev_alu)
// _ov_extract_ind_csr(out_ind_csr)
// ov_clean()
// .if (out_prev_alu != (126 << 6))
//     br[error_handling#]
// .endif
// .if (out_ind_csr != 0)
//     br[error_handling#]
// .endif

/// @cond INTERNAL_MACROS
#macro _ov_extract_prev_alu(out_prev_alu)

    #if (_OVF_STATUS & _OVF_STATUS_ALU_DST_USED)

        _ov_alu_shf_reg_const(out_prev_alu, _ov_reg_alu_/**/_OV_REG_ALU_DST, OR, _OV_ALU_CONST)

    #elif (_OVF_STATUS & _OVF_STATUS_ALU_SRC_USED)

        _ov_alu_shf_reg_const(out_prev_alu, _ov_reg_alu_/**/_OV_REG_ALU_SRC, OR, _OV_ALU_CONST)

    #else

        _ov_alu_shf_reg_const(out_prev_alu, --, B, _OV_ALU_CONST)

    #endif

#endm
/// @endcond


/// @cond INTERNAL_MACROS
#macro _ov_extract_ind_csr(out_ind_csr)

    #if (_OVF_STATUS & _OVF_STATUS_IND_CSR_DST_USED)

        _ov_alu_shf_reg_const(out_ind_csr, _ov_reg_ind_csr_/**/_OV_REG_IND_DST, OR, _OV_IND_CSR_CONST)

    #elif (_OVF_STATUS & _OVF_STATUS_IND_CSR_SRC_USED)

        _ov_alu_shf_reg_const(out_ind_csr, _ov_reg_ind_csr_/**/_OV_REG_IND_SRC, OR, _OV_IND_CSR_CONST)

    #else

        _ov_alu_shf_reg_const(out_ind_csr, --, B, _OV_IND_CSR_CONST)

    #endif

#endm
/// @endcond


/// @cond INTERNAL_MACROS
#macro ov_store_external()

    // This macro makes the current indirect reference calculation available in
    // a way such that the state and register values calculated so far (including
    // any constants that have been provided), can be stored in a convenient
    // way by the user.
    // This makes it possible to obtain partially pre-calculated register values
    // from some external source.
    // This information can then again be provided to ov_start() to recreate the
    // environment, except for the data sourced in registers.






#endm // ov_store_external
/// @endcond


/**
  * Performs sanity checks to ensure correct usage of the indirect reference
  * macros.
  *
  * Checks that:
  *   - The most recent indirect reference must have been completed with ov_clean().
  *   - Every indirect reference that was stored using ov_store() must have been cleared via
  *     either ov_resume() or ov_destroy().
  *
  * @note
  * It is recommended that ov_sanity is called at the end of the top-level microcode file.
  * It may be called at any convenient time when there should be no current indirect
  * reference and there should be no stored indirect references.
  *
  * @condition_codes Unchanged
  *
  * @instruction_cnt None - only preprocessor
  */
#macro ov_sanity()

    // This macro may be called at the end of the root microcode file to check that
    // the appropriate cleanup operations have been called:
    // - The most recent ov_start(), ov_resume() or ov_recall() has a corresponding
    //   ov_clean()
    // - Every call to ov_store() has a matching call to ov_resume() or ov_dispose()

    #if defined(_OVF_STATUS)

        #error "ov_clean() must be called exactly once after ov_start(), ov_recall() or ov_resume(), when ov_store() is not called."

    #endif

    // Every call to ov_store() has a matching call to ov_dispose()

    // Count the number of used slots
    #define_eval OV_SLOT 0
    #define_eval _OV_COUNT 0
    #define_eval _OV_USED_SLOTS ''
    #while (OV_SLOT < _OV_STORE_STATES_COUNT)

        #if (_OV_STORE_USED & (1 << OV_SLOT))

            #define_eval _OV_COUNT (_OV_COUNT + 1)

            #if (strlen('_OV_USED_SLOTS') != 0)

                #define_eval _OV_USED_SLOTS '_OV_USED_SLOTS, '

            #endif

            #define_eval _OV_USED_SLOTS '_OV_USED_SLOTS OV_SLOT'

        #endif

        #define_eval OV_SLOT (OV_SLOT + 1)

    #endloop

    #if (_OV_COUNT != 0)

        #if (_OV_COUNT == 1)

            #error "The slot" _OV_USED_SLOTS "has been used but not released. Every call to ov_store() must have a corresponding call to ov_resume() or ov_dispose()."

        #else

            #error "The slots" _OV_USED_SLOTS "have been used but not released. Every call to ov_store() must have a corresponding call to ov_resume() or ov_dispose()."

        #endif

    #endif

#endm // ov_sanity

/** @}
 * @}
 */

#endif // _OV_UC_
