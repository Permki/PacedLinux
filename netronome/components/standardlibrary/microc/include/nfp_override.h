/*
 * Copyright (C) 2008-2014 Netronome Systems, Inc.  All rights reserved.
 */

/* NFP enhanced intrinsic function definitions */

#ifndef __NFP_OVERRIDE_H__
#define __NFP_OVERRIDE_H__

#include <stdint.h>

#ifndef NFP_LIB_ANY_NFCC_VERSION
    #if (!defined(__NFP_TOOL_NFCC) ||                       \
        (__NFP_TOOL_NFCC < NFP_SW_VERSION(5, 0, 0, 0)) ||   \
        (__NFP_TOOL_NFCC > NFP_SW_VERSION(6, 1, 0, 255)))
        #error "This standard library is not supported for the version of the SDK currently in use."
    #endif
#endif


/*
 * NOTE: When writing inline-asm, it's important not to use reserved words
 *       for variables.  Common ones are 'a','b', 'csr', 'state', and
 *       'inp_state'.
 */

/** @addtogroup indirect_ref Indirect Reference 32xx Formats and Intrinsics
 *  @{
 */


 /** @addtogroup indirect_ref_6000 Indirect Reference 6xxx Formats and Intrinsics
 *  @{
 */

/* ME v2.8 indirect format definitions */
#if __BIGENDIAN
/** Override ALU Preset value in NFP-6xxx indirect format
 */
typedef union
{
    struct
    {
            uint32_t data16                 : 16;   /**< Override data reference or byte mask if data reference is not used.  */
            uint32_t reserved               : 1;    /**< Reserved.                                                            */
            uint32_t override_signal_ctx    : 1;    /**< Override signal context.                                             */
            uint32_t override_signal_num    : 1;    /**< Override signal number.                                              */
            uint32_t ref_count              : 5;    /**< Number of bytes, long/quad-words to access.                           */
            uint32_t override_length        : 1;    /**< Override length.                                                     */
            uint32_t override_bytemask_csr  : 1;    /**< Override byte mask in IND_CSR.                                       */
            uint32_t override_data          : 3;    /**< Override data.
                                                         *   0x01: Full data reference.
                                                         *   0x02: Immediate data.
                                                         *   0x03: Data context.
                                                         *   0x04: Data and signal context.
                                                         *   0x05: Per context offset of data reference.
                                                         *   0x06: Byte mask.
                                                         */
            uint32_t override_master        : 2;    /**< Override master.
                                                         *  0x01: Island and master.
                                                         *  0x02: Island, data master, signal master.
                                                         *  0x03: Island.
                                                         */
            uint32_t override_signal_master : 1;    /**< Override signal master.                                              */
    };
    uint32_t value;                                 /**< Accessor to all bits simultaneously.       */

} override_alu_ind_t;

/** This union provides overriding using CSR Indirect in NFP-6xxx indirect format.
 *
 */
typedef union
{
    struct
    {
        uint32_t reserved_1     : 2;            /**< Reserved.                                                        */
        uint32_t island         : 6;            /**< Override island when field is set in override_alu_ind_t.          */
        uint32_t master         : 4;            /**< Override data master when field is set in override_alu_ind_t.     */
        uint32_t signal_master  : 4;            /**< Override signal master when field is set in override_alu_ind_t.   */
        uint32_t signal_ctx     : 3;            /**< Override signal context when field is set in override_alu_ind_t.  */
        uint32_t signal_num     : 4;            /**< Override signal number when field is set in override_alu_ind_t.   */
        uint32_t reserved_2     : 1;            /**< Reserved.                                  */
        uint32_t byte_mask      : 8;            /**< Override byte mask when field is set in override_alu_ind_t.       */
    };
    uint32_t value;                             /**< Accessor to all bits simultaneously.                              */
} override_csr_ind_t;


#else
/* __LITTLEENDIAN */

/* See BE description above */
 typedef union
{
    struct
    {
            uint32_t override_signal_master : 1;
            uint32_t override_master        : 2;
            uint32_t override_data          : 3;
            uint32_t override_bytemask_csr  : 1;
            uint32_t override_length        : 1;
            uint32_t ref_count              : 5;
            uint32_t override_signal_num    : 1;
            uint32_t override_signal_ctx    : 1;
            uint32_t reserved               : 1;
            uint32_t data16                 : 16;
    };
    uint32_t value;

} override_alu_ind_t;

typedef union
{
    struct
    {
        uint32_t byte_mask      : 8;
        uint32_t reserved_2     : 1;
        uint32_t signal_num     : 4;
        uint32_t signal_ctx     : 3;
        uint32_t signal_master  : 4;
        uint32_t master         : 4;
        uint32_t island         : 6;
        uint32_t reserved_1     : 2;
    };
    uint32_t value;
} override_csr_ind_t;

#endif /* __BIGENDIAN */


/** In @b NFP-32xx indirect format, generic_ind_t is a union of all the indirect qualifiers along with an uint32_t.
 *  This enables one to set the entire indirect word with an integer assignment as opposed to setting each field individually.
 *  See example of the code below where length and bytemask is overridden.
 *
 * @code
 * {
 *    SIGNAL          sig;
 *    generic_ind_t   ind;
 *
 *    uint32_t write_data = 0x12;
 *    uint32_t len = 11;                          // NOTE: ovr_length zero indexed
 *    ...
 *    ovr_init(&ind, ovr_byte_mask | ovr_length);
 *    ovr_set(&ind, ovr_length, len - 1);            // override length in previous ALU result
 *    ovr_set(&ind, ovr_byte_mask, write_data);      // override bytemask in previous ALU result
 *
 *    __asm
 *    {
 *        alu[--, --, B, ind]
 *        mem[test_add_imm, rd_back, address, 0, --], sig_done[sig], num_sigs[1], indirect_ref
 *    }
 *    __wait_for_all(&sig);
 * }
 * @endcode


 *  In @b NFP-6xxx indirect format, generic_ind_t has two words for indirect format. Previous ALU result as well as Indirect CSR can be used to override.
 *  Each field that is overridden has its own enable bit. Any combination of fields can be overridden.
 *  Override any combination of fields, where each field has its own enable bit.
 *  The previous ALU result covers the most commonly used override fields and the remaining override fields are specified by the Indirect CSR.
 *  To access the ALU indirect word or CSR indirect word, use ALU_INDIRECT_TYPE() and CSR_INDIRECT_TYPE(). See the code below.
 *
 *  @note ovr_byte_mask overrides the bytemask from the indirect CSR. Use ovr_data_byte_mask to override from ALU indirect word
 *
 * @code
 * {
 *    SIGNAL          sig;
 *    generic_ind_t   ind;
 *
 *    uint32_t write_data = 0x12;
 *    uint32_t length = 11;                   // NOTE: ovr_length zero indexed
 *    ...
 *    ovr_init(&ind, ovr_byte_mask | ovr_length);
 *    ovr_set(&ind, ovr_length, len - 1);         // override length in previous ALU result
 *    ovr_set(&ind, ovr_byte_mask, write_data);   // overrides the bytemask from the indirect CSR.
 *
 *    __asm
 *    {
 *        local_csr_wr[CMD_INDIRECT_REF_0, CSR_INDIRECT_TYPE(ind)]
 *        alu[--, --, B, ALU_INDIRECT_TYPE(ind)]
 *        mem[test_add_imm, rd_back, address, 0, --], sig_done[sig], num_sigs[1], indirect_ref
 *    }
 *    __wait_for_all(&sig);
 * }
 * @endcode
 */
typedef struct
{
    override_alu_ind_t              alu_ind;               /**< ALU result in NFP-6xxx indirect reference format.
                                                            *    @ind_6000 */
    override_csr_ind_t              csr_ind;               /**< Indirect CSR word used in NFP-6xxx indirect reference mode.
                                                            *    @ind_6000 */
} generic_ind_t;


/*
 * Generic indirect type has different formats for NFP-32xx and NFP-6xxx.
 * NFP-32xx has union of all different encoding types, ALU indirect word only.
 * NFP-6xxx has one type for ALU indirect word and one for CSR indirect word.
 */

#define ALU_INDIRECT_TYPE(nfp)              nfp.alu_ind.value
#define CSR_INDIRECT_TYPE(nfp)              nfp.csr_ind.value


/**
 * @}
 */


 /** @addtogroup indirect_ref_gen Indirect Reference General
 *  @{
 */

/** Override field specifier enum.
 *
 * This enum is used by ovr_init() and ovr_set() to specify which fields to override or set
 */
typedef enum
{
    ovr_byte_mask =                 (1 << 20),      /**< Override bytemask.
                                                     *   In NFP-6xxx indirect ref mode the bytemask is in Indirect CSR
                                                     */
    ovr_length =                    (1 << 25),      /**< Override the count/length.              */
    ovr_signal_ctx =                (1 << 6),       /**< Override signal context.                */
    ovr_signal_master =             (1 << 7),       /**< Override signal master.                 */
    ovr_signal_number =             (1 << 8),       /**< Override signal number.                 */



    ovr_data_full_ref =             (1 << 16),      /**< Override the full data reference.
                                                     *   NFP-6xxx indirect ref mode only
                                                     */
    ovr_data_16bit_imm =            (1 << 15),      /**< Override 16-bit immediate data.
                                                     *   NFP-6xxx indirect ref mode only
                                                     */
    ovr_data_ctx =                  (1 << 14),       /**< Override data context only.
                                                     *   NFP-6xxx indirect ref mode only
                                                     */
    ovr_data_and_signal_ctx =       (1 << 13),      /**< Override data and signal contexts from DATA16.
                                                     *   NFP-6xxx indirect ref mode only
                                                     */
    ovr_data_ctx_offset =           (1 << 12),      /**< Override per-context offset of the data reference.
                                                     *   NFP-6xxx indirect ref mode only
                                                     */
    ovr_data_byte_mask =            (1 << 11),      /**< Override byte mask from DATA16.
                                                     *   NFP-6xxx indirect ref mode only
                                                     */
    ovr_island_and_data_master =    (1 << 5),       /**< Override island and data master.
                                                     *   NFP-6xxx indirect ref mode only
                                                     */
    ovr_island_master =             (1 << 1),       /**< Override island master.
                                                     *   NFP-6xxx indirect ref mode only
                                                    */
    ovr_signal_island_and_data_master =    (1 << 10)    /**< Override signal, island and data master.
                                                     *   NFP-6xxx indirect ref mode only
                                                     */
} ovr_field_t;


/** Initialize an override word.
 *
 * @param ind           NFP-32xx: Pointer to indirect word being constructed. NFP-6xxx: Pointer to two words: ALU and CSR indirect.
 * @param fields        Fields to override. It is specified by OR-ing the values from ovr_field_t.
 *
 * This function is used to specify which fields the user want to override with an indirect word. The encoding
 * to use is determined from the fields specified and subsequent calls to ovr_set() only allows the corresponding
 * field values to be set.
 * NFP-32xx: An exception is ovr_me and ovr_signal_and_data_master. In NFP-32xx indirect reference mode ovr_me
 * can be set when ovr_signal_and_data_master is specified in ovr_init() or vice versa. Specifying ovr_me in ovr_set
 * will force bit 3 to be set to ensure the value placed on the bus is interpreted as an ME number for
 * NFP-32xx indirect reference mode.
 *
 * A couple of examples are given below which illustrates the usage of ovr_init() and ovr_set().
 * The first example creates an override word which will correctly override the byte mask and length in
 * both NFP-32xx and NFP-6xxx indirect reference mode.
 *
 * @code
 * {
 *    __xwrite uint32_t xfer_wr[16];
 *    SIGNAL        sig;
 *    uint32_t  length = 15;
 *    generic_ind_t ind;    // NFP-32xx: this is the union of all override fields
 *                          // NFP-6xxx: this is ALU override word and CSR indirect word
 *    ...
 *    ovr_init(&ind, ovr_byte_mask | ovr_length);
 *    ovr_set(&ind, ovr_length, length - 1);          // NOTE: zero indexed
 *    ovr_set(&ind, ovr_byte_mask, 0x0f);
 *
 *    mem_write_ind((void*)&xfer_wr, 0x100, length, ind, sig_done, &sig);
 *    __wait_for_all(&sig);
 * }
 * @endcode
 *
 * In NFP-6xxx indirect reference format any combination of fields can be overridden.
 * A few examples are given below.
 *
 * The example below shows how to override the island, signal and data master in
 * NFP-6xxx indirect reference mode.
 *
 * @code
 * {
 *   SIGNAL                             signal;
 *   generic_ind_t                      indirect;
 *   __cls_n(32) __addr40 uint32_t  address[2];
 *   volatile __xrw uint32_t        xfer_read_write[2];
 *   uint32_t                       override_island = 35; // i25
 *   uint32_t                       override_me = 4;      // ME 4
 *
 *   xfer_read_write[0] = 0x11223344;
 *   xfer_read_write[1] = 0x44332211;
 *
 *   cmd_cls_write_ptr40((void *)xfer_read_write, address, 2, sig_done, &signal);
 *   wait_for_all(&signal);
 *
 *   ovr_init(&indirect, ovr_island_and_data_master | ovr_signal_master);
 *   ovr_set(&indirect, ovr_island_and_data_master, override_island << 4 | override_me + 4);
 *   ovr_set(&indirect, ovr_signal_master, override_me + 4 );
 *
 *   cmd_cls_read_ind_ptr40((void *)xfer_read_write, address, 2, indirect, sig_done, &signal);
 * }
 * @endcode
 *
 *
 * The example below shows how to override the length and signal number in
 * NFP-6xxx indirect reference mode.
 *
 * @code
 * {
 *   generic_ind_t                                      add_sat_ind;
 *   SIGNAL                                             override_signal;
 *   uint32_t                                       length = 12;
 *   volatile __xread uint32_t                      xfer_read[12];
 *   volatile __xwrite uint32_t                     xfer_write[12];
 *
 *   ovr_init(&add_sat_ind, ovr_length | ovr_signal_number);
 *   ovr_set(&add_sat_ind, ovr_length, length - 1);
 *   ovr_set(&add_sat_ind, ovr_signal_number, __signal_number(&override_signal));
 *
 *   cmd_cls_test_and_add_sat_ind_ptr40(xfer_read, xfer_write, address, length, add_sat_ind, sig_done, &signal);
 *   wait_for_all(&override_signal);
 * }
 * @endcode
 */
__intrinsic
void ovr_init(generic_ind_t *ind, uint32_t fields);

/** Set the value of an override field.
 *
 * @param ind           Xfer register(s) to store data read from Memory
 * @param field         Field to set.
 * @param value         The value of the overridden field.
 *
 * @see ovr_init()
 */
__intrinsic
void ovr_set(generic_ind_t *ind, ovr_field_t field, int32_t value);

/**
 * @}
 */





/* Actual bit setting in ALU preset value which indicates which field is being overridden
 *
 */
typedef enum
{
    ovr_signal_ctx_bit =                        (1 << 14),       /**< Override signal context bit setting   */
    ovr_signal_number_bit =                     (1 << 13),       /**< Override signal number bit setting  */
    ovr_length_bit =                            (1 << 7),        /**< Override the count/length bit setting */
    ovr_byte_mask_bit =                         (1 << 6),        /**< Override the bytemask in Indirect CSR bit setting  */

    ovr_data_full_ref_bit =                     (1 << 3),        /**< Override the full data reference bit setting*/
    ovr_data_16bit_imm_bit =                    (2 << 3),        /**< Override 16-bit immediate data bit setting*/
    ovr_data_ctx_bit =                          (3 << 3),        /**< Override data context only bit setting*/
    ovr_data_and_signal_ctx_bit =               (4 << 3),        /**< Override data and signal contexts from DATA16 bit setting*/
    ovr_data_ctx_offset_bit =                   (5 << 3),        /**< Override per-context offset of the data reference bit setting*/
    ovr_data_byte_mask_bit =                    (6 << 3),        /**< Override byte mask from DATA16 bit setting*/

    ovr_data_master_bit =                       (1 << 1),        /**< Override island and data master bit setting*/
    ovr_signal_and_data_master_bit =            (2 << 1),        /**< Override island, data master and signal master bit setting*/
    ovr_island_master_bit =                     (3 << 1),        /**< Override island master bit setting*/

    ovr_signal_master_bit =                     (1 << 0)        /**< Override signal master bit setting */

 } override_bits_t;





#endif /* __NFP_OVERRIDE_H__ */
