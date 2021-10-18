/*
 * Copyright (C) 2008-2011 Netronome Systems, Inc.  All rights reserved.
 */

#ifndef __BUF_UC__
#define __BUF_UC__

#ifndef NFP_LIB_ANY_NFAS_VERSION
    #if (!defined(__NFP_TOOL_NFAS) ||                       \
        (__NFP_TOOL_NFAS < NFP_SW_VERSION(5, 0, 0, 0)) ||   \
        (__NFP_TOOL_NFAS > NFP_SW_VERSION(6, 1, 0, 255)))
        #error "This standard library is not supported for the version of the SDK currently in use."
    #endif
#endif

#include <constants.uc>
#include <util.uc>
#include <stdmac.uc>

/** @file buf.uc Memory Allocation Macros
 * @addtogroup mem_alloc Memory_Allocation
 * @{
 *
 * @name Memory Allocation Macros general info
 * @{
 * For some macros a virtual parameter, called the @b freelist @b handle, is used to
 * refer to a specific group of macro parameters. It is composed of:
 * @arg @b POOL_ID Which freelist: POOL0, POOL1, POOL2 ...
 * @arg @b D_BASE  Base byte address of DRAM buffers. Must be aligned to 32 byte boundary.
 * @arg @b D_SIZE  Byte size of DRAM buffers. Must be power of 2.
 * @arg @b S_BASE  Base byte address of SRAM buffers.   Must be aligned to 4 byte boundary.
 * @arg @b S_SIZE  Byte size of SRAM buffers. Must be power of 2.
 *
 * For example:
 * @code
 * #define BUF_FH    BUF_FREE_LIST0, BUF_DRAM_BASE, BUFFER_SIZE, BUF_SRAM_BASE, META_DATA_SIZE
 * @endcode
 *
 * All of these addresses are byte addresses, and all of these sizes refer to the number of bytes.
 */


// API Quick Reference:
//
// buf_dram_addr_from_index(out_address, in_index, POOL_ID, D_BASE, D_SIZE, S_BASE, S_SIZE)
// buf_dram_addr_from_sram_addr(out_dram_addr, in_sram_addr, POOL_ID, D_BASE, D_SIZE, S_BASE, S_SIZE)
// buf_index_from_sram_addr(out_index, in_address, POOL_ID, D_BASE, D_SIZE, S_BASE, S_SIZE)
// buf_index_from_dram_addr(out_index, in_address, POOL_ID, D_BASE, D_SIZE, S_BASE, S_SIZE)
// buf_sram_addr_from_index(out_address, in_index, POOL_ID, D_BASE, D_SIZE, S_BASE, S_SIZE)
// buf_sram_addr_from_dram_addr(out_sram_addr, in_dram_addr, POOL_ID, D_BASE, D_SIZE, S_BASE, S_SIZE)
// buf_freelist_create(in_num_buffers, POOL_ID, D_BASE, D_SIZE, S_BASE, S_SIZE)
// buf_alloc(out_sram_addr, POOL_ID, D_BASE, D_SIZE, S_BASE, S_SIZE, REQ_SIG, in_wakeup_sigs, Q_OPTIONS)
// buf_free(in_sram_addr, POOL_ID, D_BASE, D_SIZE, S_BASE, S_SIZE)



/// @cond INTERNAL_DEFINES
#ifdef _SRAM_QA_ENTRY_BIT_OFFSET
    #error buf.uc uses the definition _SRAM_QA_ENTRY_BIT_OFFSET
#else
    #define _SRAM_QA_ENTRY_BIT_OFFSET        24 /**< SRAM Queue Array Entry Number bit offset */
#endif
/// @endcond

/** Calculate DRAM Buffer Address from Index.
 *
 * This macro calculates:
 * @code
 * out_address = (in_index * D_SIZE) + D_BASE
 * @endcode
 *
 * @param out_address A DRAM buffer address
 * @param in_index    A relative index that identifies DRAM buffer and SRAM buffer
 *                    descriptor address
 * @param POOL_ID     Which freelist: POOL0, POOL1, POOL2 ...
 * @param D_BASE      Base byte address of DRAM buffers. Must be aligned to 32 byte boundary.
 * @param D_SIZE      Byte size of DRAM buffers. Must be power of 2.
 * @param S_BASE      Base byte address of SRAM buffers. Must be aligned to 4 byte boundary.
 * @param S_SIZE      Byte size of SRAM buffers. Must be power of 2.
 *
 * @instruction_cnt 3 to 4
 */
#macro buf_dram_addr_from_index(out_address, in_index, POOL_ID, D_BASE, D_SIZE, S_BASE, S_SIZE)
.begin

    elem_addr_from_array_index(out_address, in_index, D_BASE, D_SIZE)

.end
#endm


/** Given a freelist handle and SRAM address, calculate DRAM buffer address.
 *
 * This macro calculates:
 * @code
 * Idx = (sram_addr - sram_base)/ sram_size
 * dram_addr = Idx * dram_size
 *
 * dram_addr =     (sram_addr - sram_base)
 *     `         ( ----------------------- ) * dram_size
 *                        sram_size
 *
 * dram_addr = (sram_addr - sram_base) * (dram_size/sram_size)
 * @endcode
 *
 * @param out_dram_addr A DRAM buffer address
 * @param in_sram_addr  An SRAM buffer descriptor address
 * @param POOL_ID       Which freelist: POOL0, POOL1, POOL2 ...
 * @param D_BASE        Base byte address of DRAM buffers. Must be aligned to 32 byte boundary.
 * @param D_SIZE        Byte size of DRAM buffers. Must be power of 2.
 * @param S_BASE        Base byte address of SRAM buffers. Must be aligned to 4 byte boundary.
 * @param S_SIZE        Byte size of SRAM buffers. Must be power of 2.
 *
 * @instruction_cnt 5 to 6
 */
#macro buf_dram_addr_from_sram_addr(out_dram_addr, in_sram_addr, POOL_ID, D_BASE, D_SIZE, S_BASE, S_SIZE)
.begin

    .reg buffer_base descriptor_base relative_addr

    move(buffer_base, D_BASE)
    move(descriptor_base, S_BASE)

    #if (is_ct_const(in_sram_addr) || isimport(in_sram_addr))
        #if ( (in_sram_addr > MAX_IMMEDIATE) || (in_sram_addr < 0) )
            .reg tmp_in_sram_addr
            move(tmp_in_sram_addr, in_sram_addr)
            alu[relative_addr, tmp_in_sram_addr, -, descriptor_base]
        #else
            alu[relative_addr, in_sram_addr, -, descriptor_base]
        #endif
    #else
        alu[relative_addr, in_sram_addr, -, descriptor_base]
    #endif

    #ifdef SD_SHIFT
        #warning "SD_SHIFT" is being redefined
    #endif

    #ifdef BUF_TWO2SD_SHIFT
        #warning "BUF_TWO2SD_SHIFT" is being redefined
    #endif

    #ifdef SD_MULTIPLIER
        #warning "SD_MULTIPLIER" is being redefined
    #endif


    // calculate ratio, to get correct shift amount
    // this will support up to 64kB sdram buffer with 4 word sram buffer
    #define_eval SD_MULTIPLIER (D_SIZE / S_SIZE)

    #define_eval SD_SHIFT 0
    #define_eval BUF_TWO2SD_SHIFT 1

    #while(SD_MULTIPLIER > BUF_TWO2SD_SHIFT)
        #define_eval BUF_TWO2SD_SHIFT (BUF_TWO2SD_SHIFT * 2)
        #define_eval SD_SHIFT (SD_SHIFT + 1)
    #endloop

    // special case to cover signed compare above
    #ifdef __PREPROC32
        #if (SD_MULTIPLIER == 0x80000000)
            #define_eval BUF_TWO2SD_SHIFT SD_MULTIPLIER
            #define_eval SD_SHIFT 31
        #endif
    #endif

    // if power of 2
    #if (BUF_TWO2SD_SHIFT == SD_MULTIPLIER)
            _nfp_alu_shf(out_dram_addr, buffer_base, +, relative_addr, <</**/SD_SHIFT/**/)  // calculate sdram address
    #else
        #error 2 "Error: buf_dram_addr_calc. dram buffer size/ sram buffer size not a power of 2"
    #endif

    #undef SD_SHIFT             // clean up name space
    #undef BUF_TWO2SD_SHIFT     // clean up name space
    #undef SD_MULTIPLIER        // clean up name space

.end
#endm // buf_dram_addr_from_sram_addr


/** Calculate array index from DRAM buffer address.
 *
 * This macro calculates:
 * @code
 * out_index = (in_address - D_BASE) / D_SIZE
 * @endcode
 *
 * @param out_index  A relative index that identifies DRAM buffer and SRAM
 *                   buffer descriptor addresses
 * @param in_address A DRAM buffer address
 * @param POOL_ID    Which freelist: POOL0, POOL1, POOL2 ...
 * @param D_BASE     Base byte address of DRAM buffers. Must be aligned to 32 byte boundary.
 * @param D_SIZE     Byte size of DRAM buffers. Must be power of 2.
 * @param S_BASE     Base byte address of SRAM buffers. Must be aligned to 4 byte boundary.
 * @param S_SIZE     Byte size of SRAM buffers. Must be power of 2.
 *
 * @instruction_cnt 2 to 4
 */
#macro buf_index_from_dram_addr(out_index, in_address, POOL_ID, D_BASE, D_SIZE, S_BASE, S_SIZE)
.begin

    array_index_from_elem_addr(out_index, in_address, D_BASE, D_SIZE)

.end
#endm


/** Calculate Array Index from Buffer Descriptor Address.
 *
 * This macro calculates:
 * @code
 * out_index = (in_address - S_BASE) / S_SIZE
 * @endcode
 *
 * @param out_index  A relative index that identifies DRAM buffer and SRAM
 *                   buffer descriptor addresses
 * @param in_address An SRAM buffer address
 * @param POOL_ID    Which freelist: POOL0, POOL1, POOL2 ...
 * @param D_BASE     Base byte address of DRAM buffers. Must be aligned to 32 byte boundary.
 * @param D_SIZE     Byte size of DRAM buffers. Must be power of 2.
 * @param S_BASE     Base byte address of SRAM buffers. Must be aligned to 4 byte boundary.
 * @param S_SIZE     Byte size of SRAM buffers. Must be power of 2.
 *
 * @instruction_cnt 2 to 4
 */
#macro buf_index_from_sram_addr(out_index, in_address, POOL_ID, D_BASE, D_SIZE, S_BASE, S_SIZE)
.begin

    array_index_from_elem_addr(out_index, in_address, S_BASE, S_SIZE)

.end
#endm


/** Calculate SRAM Buffer Address from Index.
 *
 * This macro calculates:
 * @code
 * out_address = (in_index * S_SIZE) + S_BASE
 * @endcode
 *
 * @param out_address An SRAM buffer address
 * @param in_index    A relative index that identifies DRAM buffer and SRAM
 *                    buffer descriptor address
 * @param POOL_ID     Which freelist: POOL0, POOL1, POOL2 ...
 * @param D_BASE      Base byte address of DRAM buffers. Must be aligned to 32 byte boundary.
 * @param D_SIZE      Byte size of DRAM buffers. Must be power of 2.
 * @param S_BASE      Base byte address of SRAM buffers. Must be aligned to 4 byte boundary.
 * @param S_SIZE      Byte size of SRAM buffers. Must be power of 2.
 *
 * @instruction_cnt 3 to 4
 */
#macro buf_sram_addr_from_index(out_address, in_index, POOL_ID, D_BASE, D_SIZE, S_BASE, S_SIZE)
.begin

    elem_addr_from_array_index(out_address, in_index, S_BASE, S_SIZE)

.end
#endm


/** Given a freelist handle and DRAM address, calculate SRAM buffer address.
 *
 * This macro calculates:
 * @code
 * Idx = (dram_addr - dram_base) / dram_size
 * sram_addr = Idx * sram_size
 *
 * sram_addr =     (dram_addr - dram_base)
 *               ( ----------------------- ) * sram_size
 *                        dram_size
 *
 * sram_addr = (dram_addr - dram_base) * (sram_size/dram_size)
 * @endcode
 *
 * @param out_sram_addr An SRAM buffer address
 * @param in_dram_addr  A DRAM buffer descriptor address
 * @param POOL_ID       Which freelist: POOL0, POOL1, POOL2 ...
 * @param D_BASE        Base byte address of DRAM buffers. Must be aligned to 32 byte boundary.
 * @param D_SIZE        Byte size of DRAM buffers. Must be power of 2.
 * @param S_BASE        Base byte address of SRAM buffers. Must be aligned to 4 byte boundary.
 * @param S_SIZE        Byte size of SRAM buffers. Must be power of 2.
 *
 * @instruction_cnt 5 to 6
 */
#macro buf_sram_addr_from_dram_addr(out_sram_addr, in_dram_addr, POOL_ID, D_BASE, D_SIZE, S_BASE, S_SIZE)
.begin

    .reg buffer_base descriptor_base relative_addr

    move(buffer_base, S_BASE)
    move(descriptor_base, D_BASE)

    #if (is_ct_const(in_dram_addr) || isimport(in_sram_addr))
        #if ( (in_dram_addr > MAX_IMMEDIATE) || (in_dram_addr < 0) )
            .reg tmp_in_dram_addr
            move(tmp_in_dram_addr, in_dram_addr)
            alu[relative_addr, tmp_in_dram_addr, -, descriptor_base]
        #else
            alu[relative_addr, in_dram_addr, -, descriptor_base]
        #endif
    #else
        alu[relative_addr, in_dram_addr, -, descriptor_base]
    #endif

    #ifdef SD_SHIFT
        #warning "SD_SHIFT" is being redefined
    #endif

    #ifdef BUF_TWO2SD_SHIFT
        #warning "BUF_TWO2SD_SHIFT" is being redefined
    #endif

    #ifdef SD_MULTIPLIER
        #warning "SD_MULTIPLIER" is being redefined
    #endif


    // calculate ratio, to get correct shift amount
    // this will support up to 64kB sdram buffer with 4 word sram buffer
    // since d_size is > s_size we calculate the multiplier and finally divide
    // it with the relative address
    #define_eval SD_MULTIPLIER (D_SIZE / S_SIZE)

    #define_eval SD_SHIFT 0
    #define_eval BUF_TWO2SD_SHIFT 1
    #while(SD_MULTIPLIER > BUF_TWO2SD_SHIFT)
        #define_eval BUF_TWO2SD_SHIFT (BUF_TWO2SD_SHIFT * 2)
        #define_eval SD_SHIFT (SD_SHIFT + 1)
    #endloop

    // special case to cover signed compare above
    #ifdef __PREPROC32
        #if (SD_MULTIPLIER == 0x80000000)
            #define_eval BUF_TWO2SD_SHIFT SD_MULTIPLIER
            #define_eval SD_SHIFT 31
        #endif
    #endif

    // if power of 2
    #if (BUF_TWO2SD_SHIFT == SD_MULTIPLIER)
            _nfp_alu_shf(out_sram_addr, buffer_base, +, relative_addr, >>/**/SD_SHIFT/**/)  // calculate sram address
    #else
        #error 2 "Error: buf_sram_addr_from_dram_addr. dram buffer size/ sram buffer size not a power of 2"
    #endif

    #undef SD_SHIFT             // clean up name space
    #undef BUF_TWO2SD_SHIFT     // clean up name space
    #undef SD_MULTIPLIER        // clean up name space

.end
#endm // buf_dram_addr_from_sram_addr


/// @cond INTERNAL_MACROS
/** Internal function to add the address into the queue.
 *
 * @param sram_addr The address to enqueue
 *
 * @instruction_cnt 1 to 3
 */
#macro _buf_add_into_list(sram_addr)
.begin

    #ifdef _BUF_AIL_SRAM_ADDR_REG
        #error "_BUF_AIL_SRAM_ADDR_REG is being redefined"
    #endif

    #if (is_ct_const(sram_addr) || isimport(sram_addr))
        .reg _in_sram_addr
        move(_sram_addr, sram_addr)
        #define_eval _BUF_AIL_SRAM_ADDR_REG _sram_addr
    #else
        #define_eval _BUF_AIL_SRAM_ADDR_REG sram_addr

    #endif

    // By default EOP & SOP is set
    // Note we need have EOP bit set otherwise the dequeue operation will
    // not update the packet[count]
    sram[enqueue, --, _BUF_AIL_SRAM_ADDR_REG, 0]

    #undef _BUF_AIL_SRAM_ADDR_REG    // clean up name space

.end
#endm
/// @endcond


/** Create SRAM buffer (descriptor) freelist using sram queues.
 *
 * The freelist termination is indicated by a return value of 0 from
 * buf_alloc(). The freelist is permanent. This library does not make provision
 * for destroying the freelist. Freelist creation should be performed at
 * initialization time only, so as not to interfere with runtime performance.
 *
 * @param in_num_buffers Number of buffers put on the freelist. The minimum
 *                       value is 1. The maximum value is arbitrary, chosen
 *                       based on the amount of memory available for buffer
 *                       space (in_num_buffers x D_SIZE) and
 *                       (in_num_buffers x S_SIZE).
 * @param POOL_ID        Which freelist: POOL0, POOL1, POOL2 ...
 * @param D_BASE         Base byte address of DRAM buffers. Must be aligned to 32 byte boundary.
 * @param D_SIZE         Byte size of DRAM buffers. Must be power of 2.
 * @param S_BASE         Base byte address of SRAM buffers. Must be aligned to 4 byte boundary.
 * @param S_SIZE         Byte size of SRAM buffers. Must be power of 2.
 *
 * @instruction_cnt (11 to 14) + 7*in_num_buffers
 */
#macro buf_freelist_create(in_num_buffers, POOL_ID, D_BASE, D_SIZE, S_BASE, S_SIZE)
.begin

    .reg $x1 $x2 $x3 $x4 sram_addr sram_qa_addr _buf_fc_cnt
    .xfer_order $x1 $x2 $x3 $x4
    .sig enq_sig

    #ifdef S_BASE_LW
        #warning S_BASE_LW is being redefined
    #endif

    #ifdef S_SIZE_LW
        #warning S_SIZE_LW is being redefined
    #endif

        // Convert to longword address for enqueue operations,
        // Retain the channel nos (bits 30-31) when converting to longword
        #if (is_ct_const(S_BASE) || isimport(S_BASE))
            #define_eval S_BASE_LW  ( (S_BASE & 0xC0000000) | ((S_BASE & 0x3FFFFFFF)>>2) )
        #else
            .reg s_base_lw_reg mask channel_num

            move(mask, 0xC0000000)
            alu[channel_num, S_BASE, AND, mask]
            move(mask, 0x3fffffff)
            alu[s_base_lw_reg, S_BASE, AND, mask]
            alu[s_base_lw_reg, channel_num, OR, s_base_lw_reg, >>2]

            #define S_BASE_LW   s_base_lw_reg

        #endif

        #if (is_ct_const(S_SIZE) || isimport(S_SIZE))

            #define_eval S_SIZE_LW  (S_SIZE >>2)
        #else
            .reg s_size_lw_reg

            alu_shf[s_size_lw_reg, --, B, S_SIZE, >>2]
            #define S_SIZE_LW   s_size_lw_reg
        #endif

    // Note that S_BASE_LW contains a longword address combined with
    // the channel number (the latter in bits 30-31)
    move(sram_qa_addr, S_BASE_LW)

    // Put the pool_id into the address
    // For  POOL_ID==0  we don't want to do anything - so check if POOL_ID is non zero
    // or register and only then perform alu_shf
    #if (is_ct_const(POOL_ID) || isimport(POOL_ID))
        #if (POOL_ID != 0)
            alu_shf[sram_qa_addr, sram_qa_addr, OR, POOL_ID, <<_SRAM_QA_ENTRY_BIT_OFFSET]
        #endif
    #else
        alu_shf[sram_qa_addr, sram_qa_addr, OR, POOL_ID, <<_SRAM_QA_ENTRY_BIT_OFFSET]
    #endif

    // initialize the tail ptr of the Q descriptor
    move($x1, 0)
    move($x2, 0)
    move($x3, 0)
    move($x4, 0)

    move(sram_addr, S_BASE)

    // Write dummy data for reading into Q descriptor register
    sram[write, $x1, sram_addr, 0, 4], ctx_swap[enq_sig]
    // Read the data back so we can be sure it's in memeory
    sram[read, $x1, sram_addr, 0, 4], ctx_swap[enq_sig]

    // Initialize the Q descriptor
    sram[rd_qdesc_tail, $x1, sram_qa_addr, 0, 2], ctx_swap[enq_sig]
    sram[rd_qdesc_other, --, sram_qa_addr, 0]
    // mark $x1 as used
    .use $x1

    move(_buf_fc_cnt, in_num_buffers)

    // enqueue the user specified SRAM BASE address
    // Note that as this address will be a byte address, it (and S_SIZE)
    // has to be divisible by 4 (size of LWord)
    move(sram_addr, S_BASE_LW)

    // Put the pool_id into the address
    // For POOL_ID==0 we don't need to do anything - so check if POOL_ID is non zero
    // or register and only then perform alu_shf
    #if (is_ct_const(POOL_ID) || isimport(POOL_ID))
        #if (POOL_ID != 0)
            alu_shf[sram_addr, sram_addr, OR, POOL_ID, <<_SRAM_QA_ENTRY_BIT_OFFSET]
        #endif
    #else
        alu_shf[sram_addr, sram_addr, OR, POOL_ID, <<_SRAM_QA_ENTRY_BIT_OFFSET]
    #endif

    // keep adding into the list until count becomes 0
    create_free_list#:
        alu[--, --, B, _buf_fc_cnt]
        beq[end_of_buf_freelist_create#]
        #ifdef CHANNEL_NUMBER_IN_BUFFER_HANDLE
            #ifdef MULTIPLE_FREELIST_ENABLE
                // Insert channel number into bits 1 and 2 of the address
                alu_shf[sram_addr, sram_addr, OR, channel_num, >>29]
            #endif
        #endif
        _buf_add_into_list(sram_addr)
        add(sram_addr, sram_addr, S_SIZE_LW)
        sub(_buf_fc_cnt, _buf_fc_cnt, 1)
        br[create_free_list#]

    end_of_buf_freelist_create#:

    #ifdef S_BASE_BYTE
        #undef S_BASE_BYTE // clean up name space
    #endif

    #undef S_BASE_LW    // clean up name space
    #undef S_SIZE_LW    // clean up name space

.end
#endm


/** Allocate SRAM/DRAM buffers from a buffer freelist identified by
 *  the freelist handle.
 *
 * @param out_sram_addr  Allocated SRAM address.
 * @param POOL_ID        Which freelist: POOL0, POOL1, POOL2 ...
 * @param D_BASE         Base byte address of DRAM buffers. Must be aligned to 32 byte boundary.
 * @param D_SIZE         Byte size of DRAM buffers. Must be power of 2.
 * @param S_BASE         Base byte address of SRAM buffers. Must be aligned to 4 byte boundary.
 * @param S_SIZE         Byte size of SRAM buffers. Must be power of 2.
 * @param REQ_SIG        Requested signal
 * @param in_wakeup_sigs List of signals causing thread to swap/wakeup
 * @param Q_OPTIONS      Directive for memory controller queue selection.
 *                       (Currently not applicable: pass the value ___.)
 *
 * @instruction_cnt 2 to 4
 */
#macro buf_alloc(out_sram_addr, POOL_ID, D_BASE, D_SIZE, S_BASE, S_SIZE, REQ_SIG, in_wakeup_sigs, Q_OPTIONS)
.begin

    .reg sram_qa_addr
    #ifdef _BUF_ALLOC_CHNL_NUM
        #warning "_BUF_ALLOC_CHNL_NUM is being redefined"
    #endif

    #if (is_ct_const(S_BASE) || isimport(S_BASE))
        // Note: & 0x3 required to make sure that result is a non-negative number
        #define_eval _BUF_ALLOC_CHNL_NUM    ((S_BASE >> 30) & 0x3)
    #else
        .reg channel_number
        alu_shf[channel_number, 0x3, AND, S_BASE, >>30]

        #define _BUF_ALLOC_CHNL_NUM     channel_number
    #endif

    // Get the channel number
    alu_shf[sram_qa_addr, --, B, _BUF_ALLOC_CHNL_NUM, <<30]

    #undef _BUF_ALLOC_CHNL_NUM  // clean up the name space

    // Put the pool_id into the address
    // For  POOL_ID==0  we don't want to do anything - so check if POOL_ID is non zero
    // or register and only then perform alu_shf
    #if (is_ct_const(POOL_ID) || isimport(POOL_ID))
        #if (POOL_ID != 0)
            alu_shf[sram_qa_addr, sram_qa_addr, OR, POOL_ID, <<_SRAM_QA_ENTRY_BIT_OFFSET]
        #endif
    #else
        alu_shf[sram_qa_addr, sram_qa_addr, OR, POOL_ID, <<_SRAM_QA_ENTRY_BIT_OFFSET]
    #endif

    sram[dequeue, out_sram_addr, sram_qa_addr, 0], sig_done[REQ_SIG]

    #if (!streq('in_wakeup_sigs', 'SIG_NONE'))
        ctx_arb[in_wakeup_sigs]
    #endif

.end
#endm


/** Return the specified SRAM address to the buffer pool identified by
 *  the freelist handle.
 *
 * @param in_sram_addr   An SRAM address
 * @param POOL_ID        Which freelist: POOL0, POOL1, POOL2 ...
 * @param D_BASE         Base byte address of DRAM buffers. Must be aligned to 32 byte boundary.
 * @param D_SIZE         Byte size of DRAM buffers. Must be power of 2.
 * @param S_BASE         Base byte address of SRAM buffers. Must be aligned to 4 byte boundary.
 * @param S_SIZE         Byte size of SRAM buffers. Must be power of 2.
 *
 * @instruction_cnt 3 to 6
 */
#macro buf_free(in_sram_addr, POOL_ID, D_BASE, D_SIZE, S_BASE, S_SIZE)
.begin

    .reg _sram_addr

    #ifdef _BUF_FREE_SRAM_ADDR_REG
        #error "_BUF_FREE_SRAM_ADDR_REG is being redefined"
    #endif

    #if (is_ct_const(in_sram_addr) || isimport(is_sram_addr))
        .reg _in_sram_addr
        move(_in_sram_addr, in_sram_addr)
        #define_eval _BUF_FREE_SRAM_ADDR_REG _in_sram_addr
    #else
        #define_eval _BUF_FREE_SRAM_ADDR_REG in_sram_addr
    #endif

    #ifdef _BUF_FREE_CHNL_NUM
        #error "_BUF_FREE_CHNL_NUM is being redefined"
    #endif

    // Get the channel number from S_BASE
    #if (is_ct_const(S_BASE) || isimport(S_BASE))
        // Note: & 0x3 required to make sure that result is a non-negative number
        #define_eval _BUF_FREE_CHNL_NUM ((S_BASE >> 30) & 0x3)
    #else
        .reg channel_number
        alu_shf[channel_number, 0x3, AND, S_BASE, >>30]

        #define _BUF_FREE_CHNL_NUM      channel_number
    #endif

    alu_shf[_sram_addr, --, B, _BUF_FREE_CHNL_NUM, <<30]

    #undef _BUF_FREE_CHNL_NUM   // clean up the name space

    // Load bits 0 to 23 of the supplied value into _sram_addr
    ld_field[_sram_addr, 0111, _BUF_FREE_SRAM_ADDR_REG]

    #undef _BUF_FREE_SRAM_ADDR_REG // clean up name space

    // Put the pool_id into the address
    // For  POOL_ID==0  we don't need to do anything - so check if POOL_ID is non zero
    // or register and only then perform alu_shf
    #if (is_ct_const(POOL_ID) || isimport(POOL_ID))
        #if (POOL_ID != 0)
            alu_shf[_sram_addr, _sram_addr, OR, POOL_ID, <<_SRAM_QA_ENTRY_BIT_OFFSET]
        #endif
    #else
        alu_shf[_sram_addr, _sram_addr, OR, POOL_ID, <<_SRAM_QA_ENTRY_BIT_OFFSET]
    #endif

    _buf_add_into_list(_sram_addr)

    #ifdef DEBUG_BUF_FREELIST
        .reg buf_cntr_base
        immed[buf_cntr_base, DL_BUF_DEBUG_BASE]
        scratch[incr, --, buf_cntr_base, NUM_DL_BUF_FREES]
    #endif

.end
#endm


/** @}
 * @}
 */

#endif /* __BUF_UC__ */
