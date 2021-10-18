/*
 * Copyright (C) 2005-2013 Netronome Systems, Inc.  All rights reserved.
 */

#ifndef __LMQUEUE_UC__
#define __LMQUEUE_UC__

#ifndef NFP_LIB_ANY_NFAS_VERSION
    #if (!defined(__NFP_TOOL_NFAS) ||                       \
        (__NFP_TOOL_NFAS < NFP_SW_VERSION(5, 0, 0, 0)) ||   \
        (__NFP_TOOL_NFAS > NFP_SW_VERSION(6, 1, 0, 255)))
        #error "This standard library is not supported for the version of the SDK currently in use."
    #endif
#endif

#include <lmqueue.h>

/** @file lmqueue.uc Local Memory Queue Operations
 * @addtogroup Local Memory Queue Operations
 * @{
 *
 * @name Local Memory Queue Operation Macros
 * @{
 *
 * These macros implement queues using local memory
 * Three different mechanisms are used to implement the LM queues. The most
 *  appropriate approach is automatically selected depending on the sizes.
 *
 *  The most efficient is if the queue size (number of items * size of item [bytes]) is 256.
 *  The worst is if the queue size is not a power-of-2, as conditional branches have to be used.
 *
 *  No assumption is made about the alignment of the buffer in local memory.
 *
 *  The programmer must ensure that the queue do not overflow. The *_enqueue macros
 *  enqueues the new item without checking for overflow.
 *
 *  General use:
 *
 *      The enqueue/dequeue macros do not directly put/get data. They adjust
 *      the head/tail/count variables and initialize the specified LM index to
 *      point to the relevant slot. The caller can then read/write the data
 *      using the specified LM index.
 *
 *  Example use:
 *
 *      //Parameters: _NAME_PREFIX, FLAGS, NUM_ITEMS, ITEM_SIZE
 *      #define SOME_QUEUE _SOME_QUEUE, 0, 16, 4
 *
 *      lmqueue_init(SOME_QUEUE)
 *
 *      lmqueue_enqueue(SOME_QUEUE, ACTIVE_LM_ADDR_0)
 *      move(*l$index0[0], 0)
 *      move(*l$index0[1], 100)
 *      ...
 *      lmqueue_dequeue(SOME_QUEUE, ACTIVE_LM_ADDR_0)
 *      move(tmp1, *l$index0[0])
 *      move(tmp2, *l$index0[1])
 *
 */

// API Quick Reference:
//
// lm_handle_alloc(HANDLE)
// lm_handle_free(HANDLE)
// lm_handle_verify(NUM_IN_USE)
// incr_lm_base(NAME)
// lmqueue_query(NAME, FLAGS, NUM_ITEMS, ITEM_SIZE, PROPERTY)
// lmqueue_init(NAME, FLAGS, NUM_ITEMS, ITEM_SIZE)
// lmqueue_zero_contents(NAME, FLAGS, NUM_ITEMS, ITEM_SIZE, LMPTR)
// lm_zero_contents(BASE, BYTE_SIZE)
// lmqueue_test(NAME, FLAGS, NUM_ITEMS, ITEM_SIZE, TEST, LABEL, DEF_COUNT)
// lmqueue_enqueue(NAME, FLAGS, NUM_ITEMS, ITEM_SIZE, LMPTR)
// lmqueue_enqueue_leave_counter(NAME, FLAGS, NUM_ITEMS, ITEM_SIZE, LMPTR)
// lmqueue_enqueue_leave_counter(NAME, FLAGS, NUM_ITEMS, ITEM_SIZE, LMPTR, save_tail)
// lmqueue_dequeue(NAME, FLAGS, NUM_ITEMS, ITEM_SIZE, LMPTR)
// lmqueue_dequeue_leave_counter(NAME, FLAGS, NUM_ITEMS, ITEM_SIZE, LMPTR)
// lmqueue_increment_counter(NAME, FLAGS, NUM_ITEMS, ITEM_SIZE, count)
// lmqueue_decrement_counter(NAME, FLAGS, NUM_ITEMS, ITEM_SIZE, count)
// lmqueue_skip_head(NAME, FLAGS, NUM_ITEMS, ITEM_SIZE, LMPTR, skip_bytes)
// lmqueue_load_tail(NAME, FLAGS, NUM_ITEMS, ITEM_SIZE, LMPTR, offset_bytes)
// lmqueue_dequeue_body(NAME, FLAGS, NUM_ITEMS, ITEM_SIZE, LMPTR)
// lmqueue_local_csr_wr(register, NAME, FLAGS, NUM_ITEMS, ITEM_SIZE, var)

#define _LM_HANDLES_IN_USE  0   // Bit mask


/** Allocate Handle to local memory control logic.
 *
 * @b Example:
 * @code
 * lm_handle_alloc(SOME_LM_HANDLE)
 * #define_eval SOME_LM_HANDLE    _LM_NEXT_HANDLE
 * #define_eval SOME_LM_INDEX     _LM_NEXT_INDEX
 * @endcode
 *
 * @param HANDLE  Constant handle name
 *
 * @instruction_cnt 0
 */
#macro lm_handle_alloc(HANDLE)

    #if ( is_ct_const(HANDLE) )

        #error "Handle is already defined as" HANDLE

    #endif

    #if ( (_LM_HANDLES_IN_USE & 1) == 0 )

        // Handle 0 is free
        #define_eval _LM_NEXT_HANDLE    0
        #define_eval _LM_NEXT_INDEX     *l$index0
        #define_eval _LM_HANDLES_IN_USE (_LM_HANDLES_IN_USE | 1)

    #elif  ( (_LM_HANDLES_IN_USE & 2) == 0 )

        // Handle 1 is free
        #define_eval _LM_NEXT_HANDLE    1
        #define_eval _LM_NEXT_INDEX     *l$index1
        #define_eval _LM_HANDLES_IN_USE (_LM_HANDLES_IN_USE | 2)

    #else

        #error "Both LM handles are already in use"

    #endif

#endm


/** Free Handle allocated with lm_handle_alloc
 *
 * @b Example:
 * @code
 * lm_handle_free(SOME_LM_HANDLE)
 * #undef SOME_LM_HANDLE
 * #undef SOME_LM_INDEX
 * @endcode
 *
 * @param HANDLE  Constant handle name
 *
 * @instruction_cnt 0
 */
#macro lm_handle_free(HANDLE)

    #if ( HANDLE == 0 )

        #define_eval _LM_HANDLES_IN_USE (_LM_HANDLES_IN_USE & ~1)

    #elif ( HANDLE == 1 )

        #define_eval _LM_HANDLES_IN_USE (_LM_HANDLES_IN_USE & ~2)

    #else

        #error "Handle was not allocated" HANDLE

    #endif
#endm


/** Verifies the number of LM  handles in use
 *
 * @b Example:
 * @code
 * lm_handle_verify(1)
 * @endcode
 *
 * @param NUM_IN_USE  Constant, expected number of handles in use
 *
 * @instruction_cnt 0
 */
#macro lm_handle_verify(NUM_IN_USE)

    #if ( ( (_LM_HANDLES_IN_USE & 1) + ((_LM_HANDLES_IN_USE>>1) & 1) ) != NUM_IN_USE )
        #error "LM handles in use" _LM_HANDLES_IN_USE
    #endif

#endm


/** Increment base address of lm block used for queue data
 *
 *   LM must be manually assigned as parts of the addresses may be used in
 *   preprocessor macros.
 *
 *   The lmqueue macros require LM_NAME_BASE and LM_NAME_SIZE.
 *
 *   Note that the blocks must be naturally aligned for some of the uses.
 *
 *   Each LM block can be specified by the triplet:
 *
 * @b Example:
 * @code
 * #define_eval LM_SOMENAME_BASE   (_LM_BASE)
 * #define_eval LM_SOMENAME_SIZE   256
 * incr_lm_base(LM_SOMENAME)
 * @endcode
 *
 * @param NAME  Constant, Name of queue
 *
 * @instruction_cnt 0
 *
 *  The BASE and the SIZE is specified in bytes.
 */
#macro incr_lm_base(NAME)

    #ifndef _LM_BASE
        #error "_LM_BASE is not defined"
    #endif

    #if ( (NAME/**/_SIZE) == 0 )
        #error "LM block size was zero"
    #endif

    #if ( (((NAME/**/_SIZE)&((NAME/**/_SIZE)-1))==0) && (_LM_BASE&((NAME/**/_SIZE)-1)) )
        #warning "Block size is a power-of-2, but not naturally aligned"
    #endif

    #define_eval _LM_BASE (_LM_BASE + (NAME/**/_SIZE))

    #if ( _LM_BASE > (640*4) )
        #error "_LM_BASE points outside the 640DW LM"
    #endif

    #ifdef LM_REPORT_VAL
        #define_eval _PRINT_VAL1 (NAME/**/_BASE)
        #define_eval _PRINT_VAL2 (NAME/**/_SIZE)
        #warning "Print :" NAME _PRINT_VAL1 _PRINT_VAL2
    #endif

#endm


/// @cond INTERNAL_MACROS
// Internal macro
#macro _lmqueue_prologue(NAME, FLAGS, NUM_ITEMS, ITEM_SIZE)

    #ifdef _LMQ_TOTAL_BYTE_SIZE
        #error Incorrect _lmqueue_prologue / _lmqueue_epilogue use
    #endif

    #ifdef _LMQ_FLAGS
        #error Incorrect _lmqueue_prologue / _lmqueue_epilogue use
    #endif

    #define_eval _LMQ_TOTAL_BYTE_SIZE ((NUM_ITEMS)*(ITEM_SIZE)*4)
    #define_eval _LMQ_FLAGS (FLAGS)

#endm


/// @cond INTERNAL_MACROS
// Internal macro
#macro _lmqueue_epilogue()

    #ifndef _LMQ_TOTAL_BYTE_SIZE
        #error Incorrect _lmqueue_prologue / _lmqueue_epilogue use
    #endif

    #ifndef _LMQ_FLAGS
        #error Incorrect _lmqueue_prologue / _lmqueue_epilogue use
    #endif

    #undef _LMQ_TOTAL_BYTE_SIZE
    #undef _LMQ_FLAGS

#endm


/// @cond INTERNAL_MACROS
// Internal macro
#macro _lmqueue_assert(TRUE_FLAGS, FALSE_FLAGS)

    #if ( (_LMQ_FLAGS & (TRUE_FLAGS)) != (TRUE_FLAGS) )
        #error "Some flags are not defined that are required for this use"
    #endif

    #if ( (_LMQ_FLAGS & (FALSE_FLAGS)) != 0 )
        #error "Some flags are defined that precludes this use"
    #endif

#endm


/** Retrieve value of selected queue parameter
 *
 * @b Example:
 * @code
 * #define SOME_QUEUE _SOME_QUEUE, 0, 16, 4
 * lmqueue_query(SOME_QUEUE, NUM_ITEMS)
 * #if (LMQ_RESULT != 16 )
 *   #error "num items incorrect."
 * #endif
 * @endcode
 *
 * @param NAME..ITEM_SIZE  Constants, queue parameters
 * @param PROPERTY         One of NUM_ITEMS, ITEM_BSIZE, ITEM_DSIZE, ITEM_BSIZE_LOG2, QUEUE_BSIZE, FLAGS, COUNTER
 *
 * @instruction_cnt 0
 */
#macro lmqueue_query(NAME, FLAGS, NUM_ITEMS, ITEM_SIZE, PROPERTY)

    #if ( streq(PROPERTY, "NUM_ITEMS") )

        #define_eval LMQ_RESULT (NUM_ITEMS)

    #elif ( streq(PROPERTY, "ITEM_BSIZE") )

        #define_eval LMQ_RESULT ((ITEM_SIZE)*4)

    #elif ( streq(PROPERTY, "ITEM_DSIZE") )

        #define_eval LMQ_RESULT (ITEM_SIZE)

    #elif ( streq(PROPERTY, "ITEM_BSIZE_LOG2") )

        #if ( log2(ITEM_SIZE, 1) != log2(ITEM_SIZE, -1) )
            #error Incorrect use
        #endif

        #define_eval LMQ_RESULT log2((ITEM_SIZE)*4)

    #elif ( streq(PROPERTY, "QUEUE_BSIZE") )

        #define_eval LMQ_RESULT ((NUM_ITEMS)*(ITEM_SIZE)*4)

    #elif ( streq(PROPERTY, "FLAGS") )

        #define_eval LMQ_RESULT (FLAGS)

    #elif ( streq(PROPERTY, "COUNTER") )

        #define_eval LMQ_RESULT @/**/NAME/**/_count

    #else

        #error "Unsupported property"

    #endif

#endm


/** Initialize local memory queue
 *
 * @b Example:
 * @code
 * #define SOME_QUEUE _SOME_QUEUE, 0, 16, 4
 * lmqueue_init(SOME_QUEUE)
 * @endcode
 * @param NAME      Constant, queue name
 * @param FLAGS     Constant, one or more of LMQ_KEEP_HEAD, LMQ_KEEP_TAIL, LMQ_NO_COUNTER, LMQ_USE_BODY, LMQ_ZERO
 * @param NUM_ITEMS Constant, number of items in queue
 * @param ITEM_SIZE Constant, size of queue item in lwords
 *
 * @instruction_cnt depends on FLAGS
 */
#macro lmqueue_init(NAME, FLAGS, NUM_ITEMS, ITEM_SIZE)

    _lmqueue_prologue(NAME, FLAGS, NUM_ITEMS, ITEM_SIZE)

    #if ( (LM_/**/NAME/**/_SIZE) != (_LMQ_TOTAL_BYTE_SIZE) )
        #error "The manually assigned LM size does not match the queue parameters"
    #endif

    .reg volatile @/**/NAME/**/_base
    immed[@/**/NAME/**/_base, LM_/**/NAME/**/_BASE]

    .reg volatile @/**/NAME/**/_next_head
    immed[@/**/NAME/**/_next_head, LM_/**/NAME/**/_BASE]

    .reg volatile @/**/NAME/**/_next_tail
    immed[@/**/NAME/**/_next_tail, LM_/**/NAME/**/_BASE]

    #if ( !(_LMQ_FLAGS & LMQ_NO_COUNTER) )

        .reg volatile @/**/NAME/**/_count
        immed[@/**/NAME/**/_count, 0]

    #endif

    #if ( _LMQ_FLAGS & LMQ_KEEP_HEAD )

        .reg volatile @/**/NAME/**/_head
        immed[@/**/NAME/**/_head, LM_/**/NAME/**/_BASE] // Invalid if queue empty - here to stop warnings

    #endif

    #if ( _LMQ_FLAGS & LMQ_KEEP_TAIL )

        .reg volatile @/**/NAME/**/_tail
        immed[@/**/NAME/**/_tail, LM_/**/NAME/**/_BASE] // Invalid if queue empty - here to stop warnings

    #endif

    #if ( _LMQ_FLAGS & LMQ_USE_BODY )

        .reg volatile @/**/NAME/**/_next_body
        immed[@/**/NAME/**/_next_body, LM_/**/NAME/**/_BASE]

        .reg volatile @/**/NAME/**/_body
        immed[@/**/NAME/**/_body, LM_/**/NAME/**/_BASE] // Invalid if queue empty - here to stop warnings

        .reg volatile @/**/NAME/**/_next_body_offset
        immed[@/**/NAME/**/_next_body_offset, 0]

        .reg volatile @/**/NAME/**/_bodycount
        immed[@/**/NAME/**/_bodycount, 0]

    #endif

    #if ( _LMQ_TOTAL_BYTE_SIZE == 256 )

        .reg volatile @/**/NAME/**/_next_head_offset
        immed[@/**/NAME/**/_next_head_offset, 0]

        .reg volatile @/**/NAME/**/_next_tail_offset
        immed[@/**/NAME/**/_next_tail_offset, 0]

    #elif ( log2(_LMQ_TOTAL_BYTE_SIZE, 1) == log2(_LMQ_TOTAL_BYTE_SIZE, -1) )

        .reg volatile @/**/NAME/**/_next_head_offset
        immed[@/**/NAME/**/_next_head_offset, 0]

        .reg volatile @/**/NAME/**/_next_tail_offset
        immed[@/**/NAME/**/_next_tail_offset, 0]

        .reg volatile @/**/NAME/**/_mask
        immed[@/**/NAME/**/_mask, (_LMQ_TOTAL_BYTE_SIZE-1)]

    #else

        #warning "Not all functions implemented for this size - check"

        .reg volatile @/**/NAME/**/_top
        immed[@/**/NAME/**/_top, (LM_/**/NAME/**/_BASE+_LMQ_TOTAL_BYTE_SIZE)]

    #endif

    #if ( _LMQ_FLAGS & LMQ_ZERO )

        lmqueue_zero_contents(NAME, FLAGS, NUM_ITEMS, ITEM_SIZE, ACTIVE_LM_ADDR_0)

    #endif


    _lmqueue_epilogue()

#endm


/** Zero queue data, using queue configuration
 *
 * @b Example:
 * @code
 * #define SOME_QUEUE _SOME_QUEUE, 0, 16, 4
 * lmqueue_init(SOME_QUEUE)
 * lmqueue_zero_contents(SOME_QUEUE, ACTIVE_LM_ADDR_0)
 * @endcode
 *
 * @param NAME..ITEM_SIZE  Constants, queue parameters
 * @param LMPTR            Constant, indirect local memory address reg, one of
 *                         ACTIVE_LM_ADDR_0 or ACTIVE_LM_ADDR_1
 *
 */
#macro lmqueue_zero_contents(NAME, FLAGS, NUM_ITEMS, ITEM_SIZE, LMPTR)
.begin

    #if ( ((NUM_ITEMS)*(ITEM_SIZE)) & 3 )

        #error "This queue size not yet supported here"

    #endif

    #if ( streq(LMPTR, "ACTIVE_LM_ADDR_0") )

        #define_eval _LMQ_LM_IDX 0

    #elif ( streq(LMPTR, "ACTIVE_LM_ADDR_1") )

        #define_eval _LMQ_LM_IDX 1

    #else

        #error "Invalid LMPTR"

    #endif


    .reg num_4dw

    local_csr_wr[LMPTR, @/**/NAME/**/_base]
    immed[num_4dw, ( ((NUM_ITEMS)*(ITEM_SIZE)) >> 2)]
    nop

zero_loop#:

    alu[num_4dw, num_4dw, -, 1]
    immed[*l$index/**/_LMQ_LM_IDX/**/++, 0]
    immed[*l$index/**/_LMQ_LM_IDX/**/++, 0]
    bne[zero_loop#], defer[2]
    immed[*l$index/**/_LMQ_LM_IDX/**/++, 0]
    immed[*l$index/**/_LMQ_LM_IDX/**/++, 0]

    #undef _LMQ_LM_IDX

.end
#endm


/** Zero local memory
 *
 * @b Example:
 * @code
 * lm_zero_contents(0, 0x10)
 * @endcode
 *
 * @param BASE       Constant, local memory starting addredss to zero
 * @param BYTE_SIZE  Constant, size in bytes of area to zero
 */
#macro lm_zero_contents(BASE, BYTE_SIZE)
.begin

    #if ( (BASE) & 3 )

        #error "This base address is not supported"

    #endif

    #if ( (BYTE_SIZE) & 3 )

        #error "This queue size is not supported"

    #endif

    .reg old_lm_addr

    immed[old_lm_addr, (BASE)]
    local_csr_wr[ACTIVE_LM_ADDR_0, old_lm_addr]
    local_csr_rd[ACTIVE_LM_ADDR_0]
    immed[old_lm_addr, 0]

    #if ( (BYTE_SIZE) > 16 )

        .reg cnt
        immed[cnt, ( (BYTE_SIZE) >> 4)]

    zero_loop#:

        alu[cnt, cnt, -, 1]
        immed[*l$index0++, 0]
        immed[*l$index0++, 0]
        bne[zero_loop#], defer[2]
        immed[*l$index0++, 0]
        immed[*l$index0++, 0]

    #endif

    #if ( ((BYTE_SIZE) & 15) >= 4 )

        immed[*l$index0++, 0]

    #endif

    #if ( ((BYTE_SIZE) & 15) >= 8 )

        immed[*l$index0++, 0]

    #endif

    #if ( ((BYTE_SIZE) & 15) >= 12 )

        immed[*l$index0++, 0]

    #endif

    local_csr_wr[ACTIVE_LM_ADDR_0, old_lm_addr]
    nop
    nop
    nop

.end
#endm


/** Test status of queue
 *
 * @b Example:
 * @code
 * #define SOME_QUEUE _SOME_QUEUE, 0, 16, 4
 * lmqueue_test(SOME_QUEUE, IF_FULL, queue_full#, 0)
 * br[queue_not_full#]
 * queue_full#:
 *  move(status,1)
 *  br[done#]
 * queue_not_full#:
 *  move(status,0)
 * done#:
 * @endcode
 *
 * @param NAME..ITEM_SIZE  Constants, queue parameters
 * @param TEST             Constant, test type, one of IF_FULL, IF_NOT_FULL, IF_EMPTY, IF_NOT_EMPTY, IF_BODY_EMPTY
 * @param LABEL            Branch target label if test true
 * @param DEF_COUNT        Constant, defer count on branch to LABEL
 *
 * @instruction_cnt 2
 */
#macro lmqueue_test(NAME, FLAGS, NUM_ITEMS, ITEM_SIZE, TEST, LABEL, DEF_COUNT)

    _lmqueue_prologue(NAME, FLAGS, NUM_ITEMS, ITEM_SIZE)

    _lmqueue_assert(0, LMQ_NO_COUNTER)

    #if ( streq( TEST, "IF_FULL" ))

        alu[--, (NUM_ITEMS), -, @/**/NAME/**/_count]
        #if ( DEF_COUNT > 0 )
            beq[LABEL], defer[DEF_COUNT]
        #else
            beq[LABEL]
        #endif

    #elif ( streq( TEST, "IF_NOT_FULL" ))

        alu[--, (NUM_ITEMS), -, @/**/NAME/**/_count]
        #if ( DEF_COUNT > 0 )
            bne[LABEL], defer[DEF_COUNT]
        #else
            bne[LABEL]
        #endif

    #elif ( streq( TEST, "IF_EMPTY" ))

        alu[--, --, B, @/**/NAME/**/_count]
        #if ( DEF_COUNT > 0 )
            beq[LABEL], defer[DEF_COUNT]
        #else
            beq[LABEL]
        #endif

    #elif ( streq( TEST, "IF_NOT_EMPTY" ))

        alu[--, --, B, @/**/NAME/**/_count]
        #if ( DEF_COUNT > 0 )
            bne[LABEL], defer[DEF_COUNT]
        #else
            bne[LABEL]
        #endif

    #elif ( streq( TEST, "IF_BODY_EMPTY" ))

        _lmqueue_assert(LMQ_USE_BODY, 0)

        alu[--, --, B, @/**/NAME/**/_bodycount]
        #if ( DEF_COUNT > 0 )
            beq[LABEL], defer[DEF_COUNT]
        #else
            beq[LABEL]
        #endif

    #else

        #error TEST operation is not supported

    #endif

    _lmqueue_epilogue()

#endm


/** Enqueue to queue
 *
 * @b Example:
 * @code
 * #define SOME_QUEUE _SOME_QUEUE, 0, 16, 4
 * lmqueue_init(SOME_QUEUE)
 * lmqueue_enqueue(SOME_QUEUE, ACTIVE_LM_ADDR_0)
 * .reg data_val
 * move(*l$index0, data_val)
 *
 * @endcode
 *
 * @param NAME..ITEM_SIZE  Constants, queue parameters
 * @param LMPTR            Constant, indirect local memory address reg, one of
 *                         ACTIVE_LM_ADDR_0 or ACTIVE_LM_ADDR_1
 */
#macro lmqueue_enqueue(NAME, FLAGS, NUM_ITEMS, ITEM_SIZE, LMPTR)

    _lmqueue_prologue(NAME, FLAGS, NUM_ITEMS, ITEM_SIZE)

    local_csr_wr[LMPTR, @/**/NAME/**/_next_tail]

    #if ( _LMQ_FLAGS & LMQ_KEEP_TAIL )

        alu[@/**/NAME/**/_tail, --, B, @/**/NAME/**/_next_tail]

    #endif

    #if ( _LMQ_FLAGS & LMQ_USE_BODY )

            alu[@/**/NAME/**/_bodycount, @/**/NAME/**/_bodycount, +, 1]

    #endif

    #if ( _LMQ_TOTAL_BYTE_SIZE == 256 )

        alu_op[@/**/NAME/**/_next_tail_offset, @/**/NAME/**/_next_tail_offset, +, (ITEM_SIZE*4)]
        alu[@/**/NAME/**/_next_tail, @/**/NAME/**/_base, +8, @/**/NAME/**/_next_tail_offset]
        #if ( !(_LMQ_FLAGS & LMQ_NO_COUNTER) )
            alu[@/**/NAME/**/_count, @/**/NAME/**/_count, +, 1]
        #elif ( !(_LMQ_FLAGS & LMQ_KEEP_TAIL) )
            nop
        #endif

    #elif ( log2(_LMQ_TOTAL_BYTE_SIZE, 1) == log2(_LMQ_TOTAL_BYTE_SIZE, -1) )

        alu_op[@/**/NAME/**/_next_tail_offset, @/**/NAME/**/_next_tail_offset, +, (ITEM_SIZE*4)]
        alu[@/**/NAME/**/_next_tail_offset, @/**/NAME/**/_next_tail_offset, AND, @/**/NAME/**/_mask]
        alu[@/**/NAME/**/_next_tail, @/**/NAME/**/_base, +, @/**/NAME/**/_next_tail_offset]
        #if ( !(_LMQ_FLAGS & LMQ_NO_COUNTER) )
            alu[@/**/NAME/**/_count, @/**/NAME/**/_count, +, 1]
        #endif

    #else

        alu_op[@/**/NAME/**/_next_tail, @/**/NAME/**/_next_tail, +, (ITEM_SIZE*4)]
        alu[--, @/**/NAME/**/_top, -, @/**/NAME/**/_next_tail]
        bne[done#], defer[1]
        alu[@/**/NAME/**/_count, @/**/NAME/**/_count, +, 1]

        alu[@/**/NAME/**/_next_tail, --, B, @/**/NAME/**/_base]
        done#:

    #endif

    _lmqueue_epilogue()

#endm


/** Enqueue to queue, don't maintain count
 *
 * @b Example:
 * @code
 * #define SOME_QUEUE _SOME_QUEUE, 0, 16, 4
 * lmqueue_init(SOME_QUEUE)
 * lmqueue_enqueue_leave_counter(SOME_QUEUE, ACTIVE_LM_ADDR_0)
 * .reg data_val
 * move(*l$index0, data_val)
 *
 * @endcode
 *
 * @param NAME..ITEM_SIZE  Constants, queue parameters
 * @param LMPTR            Constant, indirect local memory address reg, one of
 *                         ACTIVE_LM_ADDR_0 or ACTIVE_LM_ADDR_1
 *
 * Saves 1 instruction compared to lmqueue_enqueue()
 */
#macro lmqueue_enqueue_leave_counter(NAME, FLAGS, NUM_ITEMS, ITEM_SIZE, LMPTR)

    _lmqueue_prologue(NAME, FLAGS, NUM_ITEMS, ITEM_SIZE)

    local_csr_wr[LMPTR, @/**/NAME/**/_next_tail]

    #if ( _LMQ_FLAGS & LMQ_KEEP_TAIL )

        alu[@/**/NAME/**/_tail, --, B, @/**/NAME/**/_next_tail]

    #endif

    #if ( _LMQ_TOTAL_BYTE_SIZE == 256 )

        alu_op[@/**/NAME/**/_next_tail_offset, @/**/NAME/**/_next_tail_offset, +, (ITEM_SIZE*4)]
        alu[@/**/NAME/**/_next_tail, @/**/NAME/**/_base, +8, @/**/NAME/**/_next_tail_offset]
        #if ( !(_LMQ_FLAGS & LMQ_KEEP_TAIL) )
            nop
        #endif

    #elif ( log2(_LMQ_TOTAL_BYTE_SIZE, 1) == log2(_LMQ_TOTAL_BYTE_SIZE, -1) )

        alu_op[@/**/NAME/**/_next_tail_offset, @/**/NAME/**/_next_tail_offset, +, (ITEM_SIZE*4)]
        alu[@/**/NAME/**/_next_tail_offset, @/**/NAME/**/_next_tail_offset, AND, @/**/NAME/**/_mask]
        alu[@/**/NAME/**/_next_tail, @/**/NAME/**/_base, +, @/**/NAME/**/_next_tail_offset]

    #else

        #error This mode is not supported

    #endif

    _lmqueue_epilogue()

#endm

/** Enqueue to queue, don't maintain count, return current tail
 *
 * @b Example:
 * @code
 * #define SOME_QUEUE _SOME_QUEUE, 0, 16, 4
 * lmqueue_init(SOME_QUEUE)
 * .reg r_tail
 * lmqueue_enqueue_leave_counter(SOME_QUEUE, ACTIVE_LM_ADDR_0, r_tail)
 * move(*l$index0, data_val)
 *
 * @endcode
 *
 * @param NAME..ITEM_SIZE  Constants, queue parameters
 * @param LMPTR            Constant, indirect local memory address reg, one of
 *                         ACTIVE_LM_ADDR_0 or ACTIVE_LM_ADDR_1
 * @pararm save_tail       GPR, written with current queue tail value
 *
 */
#macro lmqueue_enqueue_leave_counter(NAME, FLAGS, NUM_ITEMS, ITEM_SIZE, LMPTR, save_tail)

    alu[save_tail, --, B, @/**/NAME/**/_next_tail]
    lmqueue_enqueue_leave_counter(NAME, FLAGS, NUM_ITEMS, ITEM_SIZE, LMPTR)

#endm


/** Dequeue from queue
 *
 * @b Example:
 * @code
 * #define SOME_QUEUE _SOME_QUEUE, 0, 16, 4
 * lmqueue_init(SOME_QUEUE)
 * lmqueue_dequeue(SOME_QUEUE, ACTIVE_LM_ADDR_0)
 * .reg data_val
 * move(data_val, *l$index0)
 *
 * @endcode
 *
 * @param NAME..ITEM_SIZE  Constants, queue parameters
 * @param LMPTR            Constant, indirect local memory address reg, one of
 *                         ACTIVE_LM_ADDR_0 or ACTIVE_LM_ADDR_1
 */
#macro lmqueue_dequeue(NAME, FLAGS, NUM_ITEMS, ITEM_SIZE, LMPTR)

    _lmqueue_prologue(NAME, FLAGS, NUM_ITEMS, ITEM_SIZE)

    local_csr_wr[LMPTR, @/**/NAME/**/_next_head]

    #if ( _LMQ_FLAGS & LMQ_KEEP_HEAD )

        alu[@/**/NAME/**/_head, --, B, @/**/NAME/**/_next_head]

    #endif

    #if ( _LMQ_TOTAL_BYTE_SIZE == 256 )

        alu_op[@/**/NAME/**/_next_head_offset, @/**/NAME/**/_next_head_offset, +, (ITEM_SIZE*4)]
        alu[@/**/NAME/**/_next_head, @/**/NAME/**/_base, +8, @/**/NAME/**/_next_head_offset]
        #if ( !(_LMQ_FLAGS & LMQ_NO_COUNTER) )
            alu[@/**/NAME/**/_count, @/**/NAME/**/_count, -, 1]
        #elif ( !(_LMQ_FLAGS & LMQ_KEEP_HEAD) )
            nop
        #endif

    #elif ( log2(_LMQ_TOTAL_BYTE_SIZE, 1) == log2(_LMQ_TOTAL_BYTE_SIZE, -1) )

        alu_op[@/**/NAME/**/_next_head_offset, @/**/NAME/**/_next_head_offset, +, (ITEM_SIZE*4)]
        alu[@/**/NAME/**/_next_head_offset, @/**/NAME/**/_next_head_offset, AND, @/**/NAME/**/_mask]
        alu[@/**/NAME/**/_next_head, @/**/NAME/**/_base, +, @/**/NAME/**/_next_head_offset]
        #if ( !(_LMQ_FLAGS & LMQ_NO_COUNTER) )
            alu[@/**/NAME/**/_count, @/**/NAME/**/_count, -, 1]
        #endif

    #else

        alu_op[@/**/NAME/**/_next_head, @/**/NAME/**/_next_head, +, (ITEM_SIZE*4)]
        alu[--, @/**/NAME/**/_top, -, @/**/NAME/**/_next_head]
        bne[done#], defer[1]
        alu[@/**/NAME/**/_count, @/**/NAME/**/_count, -, 1]

        alu[@/**/NAME/**/_next_head, --, B, @/**/NAME/**/_base]
        done#:

    #endif

    _lmqueue_epilogue()

#endm

/** Dequeue from queue, don't maintain count
 *
 * @b Example:
 * @code
 * #define SOME_QUEUE _SOME_QUEUE, 0, 16, 4
 * lmqueue_init(SOME_QUEUE)
 * lmqueue_dequeue_leave_counter(SOME_QUEUE, ACTIVE_LM_ADDR_0)
 * .reg data_val
 * move(*l$index0, data_val)
 *
 * @endcode
 *
 * @param NAME..ITEM_SIZE  Constants, queue parameters
 * @param LMPTR            Constant, indirect local memory address reg, one of
 *                         ACTIVE_LM_ADDR_0 or ACTIVE_LM_ADDR_1
 *
 * Saves 1 instruction compared to lmqueue_dequeue()
 */
#macro lmqueue_dequeue_leave_counter(NAME, FLAGS, NUM_ITEMS, ITEM_SIZE, LMPTR)

    _lmqueue_prologue(NAME, FLAGS, NUM_ITEMS, ITEM_SIZE)

    local_csr_wr[LMPTR, @/**/NAME/**/_next_head]

    #if ( _LMQ_FLAGS & LMQ_KEEP_HEAD )

        alu[@/**/NAME/**/_head, --, B, @/**/NAME/**/_next_head]

    #endif

    #if ( _LMQ_TOTAL_BYTE_SIZE == 256 )

        alu_op[@/**/NAME/**/_next_head_offset, @/**/NAME/**/_next_head_offset, +, (ITEM_SIZE*4)]
        alu[@/**/NAME/**/_next_head, @/**/NAME/**/_base, +8, @/**/NAME/**/_next_head_offset]
        #if ( !(_LMQ_FLAGS & LMQ_KEEP_HEAD) )
            nop
        #endif

    #elif ( log2(_LMQ_TOTAL_BYTE_SIZE, 1) == log2(_LMQ_TOTAL_BYTE_SIZE, -1) )

        alu_op[@/**/NAME/**/_next_head_offset, @/**/NAME/**/_next_head_offset, +, (ITEM_SIZE*4)]
        alu[@/**/NAME/**/_next_head_offset, @/**/NAME/**/_next_head_offset, AND, @/**/NAME/**/_mask]
        alu[@/**/NAME/**/_next_head, @/**/NAME/**/_base, +, @/**/NAME/**/_next_head_offset]

    #else

        #error This mode is not supported

    #endif

    _lmqueue_epilogue()

#endm


/** Increment queue counter
 *
 * @b Example:
 * @code
 * #define SOME_QUEUE _SOME_QUEUE, 0, 16, 4
 * lmqueue_init(SOME_QUEUE)
 * lmqueue_enqueue_leave_counter(SOME_QUEUE, ACTIVE_LM_ADDR_0)
 * move(*l$index0, data_val1)
 * lmqueue_enqueue_leave_counter(SOME_QUEUE, ACTIVE_LM_ADDR_0)
 * move(*l$index0, data_val2)
 * lmqueue_increment_counter(SOME_QUEUE, 2)
 * @endcode
 *
 * @param NAME..ITEM_SIZE  Constants, queue parameters
 * @param count            GPR, amount to increment counter
 *
 * @instruction_cnt 1
 */
#macro lmqueue_increment_counter(NAME, FLAGS, NUM_ITEMS, ITEM_SIZE, count)

    _lmqueue_prologue(NAME, FLAGS, NUM_ITEMS, ITEM_SIZE)

    _lmqueue_assert(0, LMQ_NO_COUNTER)

    #if ( _LMQ_FLAGS & LMQ_USE_BODY )

            alu[@/**/NAME/**/_bodycount, @/**/NAME/**/_bodycount, +, 1]

    #endif

    #if ( _LMQ_TOTAL_BYTE_SIZE == 256 )

        alu[@/**/NAME/**/_count, @/**/NAME/**/_count, +, count]

    #elif ( log2(_LMQ_TOTAL_BYTE_SIZE, 1) == log2(_LMQ_TOTAL_BYTE_SIZE, -1) )

        alu[@/**/NAME/**/_count, @/**/NAME/**/_count, +, count]

    #else

        #error This mode is not supported

    #endif

    _lmqueue_epilogue()

#endm


/** Decrement queue counter
 *
 * @b Example:
 * @code
 * #define SOME_QUEUE _SOME_QUEUE, 0, 16, 4
 * lmqueue_init(SOME_QUEUE)
 * lmqueue_dequeue_leave_counter(SOME_QUEUE, ACTIVE_LM_ADDR_0)
 * move(data_val1, *l$index0)
 * lmqueue_enqueue_leave_counter(SOME_QUEUE, ACTIVE_LM_ADDR_0)
 * move(data_val2, *l$index0)
 * lmqueue_decrement_counter(SOME_QUEUE, 2)
 * @endcode
 *
 * @param NAME..ITEM_SIZE  Constants, queue parameters
 * @param count            GPR, amount to decrement counter
 *
 * @instruction_cnt 1
 */
#macro lmqueue_decrement_counter(NAME, FLAGS, NUM_ITEMS, ITEM_SIZE, count)

    _lmqueue_prologue(NAME, FLAGS, NUM_ITEMS, ITEM_SIZE)

    _lmqueue_assert(0, LMQ_NO_COUNTER)

    #if ( _LMQ_TOTAL_BYTE_SIZE == 256 )

        alu[@/**/NAME/**/_count, @/**/NAME/**/_count, -, count]

    #elif ( log2(_LMQ_TOTAL_BYTE_SIZE, 1) == log2(_LMQ_TOTAL_BYTE_SIZE, -1) )

        alu[@/**/NAME/**/_count, @/**/NAME/**/_count, -, count]

    #else

        #error This mode is not supported

    #endif

    _lmqueue_epilogue()

#endm


/** Advance queue head address, skipping over queue bytes,
 *  and load local memory pointer reg
 *
 * @b Example:
 * @code
 * #define SOME_QUEUE _SOME_QUEUE, 0, 16, 4
 * lmqueue_skip_head(SOME_QUEUE, 4)
 * move(data_val, *l$index0)
 * @endcode
 *
 * @param NAME..ITEM_SIZE  Constants, queue parameters
 * @param count            GPR, amount to increment counter
 *
 * @instruction_cnt 3 or 4
 */
#macro lmqueue_skip_head(NAME, FLAGS, NUM_ITEMS, ITEM_SIZE, LMPTR, skip_bytes)

    _lmqueue_prologue(NAME, FLAGS, NUM_ITEMS, ITEM_SIZE)

    #if ( _LMQ_TOTAL_BYTE_SIZE == 256 )

        alu[@/**/NAME/**/_next_head_offset, @/**/NAME/**/_next_head_offset, +, skip_bytes]
        alu[@/**/NAME/**/_head, @/**/NAME/**/_base, +8, @/**/NAME/**/_next_head_offset]
        local_csr_wr[LMPTR, @/**/NAME/**/_head]

    #elif ( log2(_LMQ_TOTAL_BYTE_SIZE, 1) == log2(_LMQ_TOTAL_BYTE_SIZE, -1) )

        alu[@/**/NAME/**/_next_head_offset, @/**/NAME/**/_next_head_offset, +, skip_bytes]
        alu[@/**/NAME/**/_next_head_offset, @/**/NAME/**/_next_head_offset, AND, @/**/NAME/**/_mask]
        alu[@/**/NAME/**/_head, @/**/NAME/**/_base, +, @/**/NAME/**/_next_head_offset]
        local_csr_wr[LMPTR, @/**/NAME/**/_head]

    #else

        #error This mode is not supported

    #endif

    _lmqueue_epilogue()

#endm


/** Load queue tail address and load local memory pointer reg
 *
 * @b Example:
 * @code
 * #define SOME_QUEUE _SOME_QUEUE, 0, 64, 4
 * lmqueue_load_tail(SOME_QUEUE, 32)
 * move(*l$index0, data_val)
 * @endcode
 *
 * @param NAME..ITEM_SIZE  Constants, queue parameters
 * @param count            GPR, amount to increment counter
 *
 * @instruction_cnt 2
 */
#macro lmqueue_load_tail(NAME, FLAGS, NUM_ITEMS, ITEM_SIZE, LMPTR, offset_bytes)

    _lmqueue_prologue(NAME, FLAGS, NUM_ITEMS, ITEM_SIZE)

    #if ( _LMQ_TOTAL_BYTE_SIZE == 256 )

        alu[@/**/NAME/**/_tail, @/**/NAME/**/_base, +8, offset_bytes]
        local_csr_wr[LMPTR, @/**/NAME/**/_tail]

    #elif ( log2(_LMQ_TOTAL_BYTE_SIZE, 1) == log2(_LMQ_TOTAL_BYTE_SIZE, -1) )

        #error This mode is not supported

    #else

        #error This mode is not supported

    #endif

    _lmqueue_epilogue()

#endm

/** Dequeue body from queue
 *
 * @b Example:
 * @code
 * #define SOME_QUEUE _SOME_QUEUE, LMQ_USE_BODY, 16, 4
 * lmqueue_init(SOME_QUEUE)
 * lmqueue_enqueue(SOME_QUEUE, ACTIVE_LM_ADDR_0)
 * .reg data_val1
 * move(*l$index0, data_val1)
 * lmqueue_dequeue_body(SOME_QUEUE, ACTIVE_LM_ADDR_0)
 * .reg data_val2
 * move(data_val2, *l$index0)
 * @endcode
 *
 * @param NAME..ITEM_SIZE  Constants, queue parameters
 * @param LMPTR            Constant, indirect local memory address reg, one of
 *                         ACTIVE_LM_ADDR_0 or ACTIVE_LM_ADDR_1
 */
#macro lmqueue_dequeue_body(NAME, FLAGS, NUM_ITEMS, ITEM_SIZE, LMPTR)

    _lmqueue_prologue(NAME, FLAGS, NUM_ITEMS, ITEM_SIZE)

    local_csr_wr[LMPTR, @/**/NAME/**/_next_body]

    alu[@/**/NAME/**/_body, --, B, @/**/NAME/**/_next_body]

    #if ( _LMQ_TOTAL_BYTE_SIZE == 256 )

        alu_op[@/**/NAME/**/_next_body_offset, @/**/NAME/**/_next_body_offset, +, (ITEM_SIZE*4)]
        alu[@/**/NAME/**/_next_body, @/**/NAME/**/_base, +8, @/**/NAME/**/_next_body_offset]
        alu[@/**/NAME/**/_bodycount, @/**/NAME/**/_bodycount, -, 1]

    #elif ( log2(_LMQ_TOTAL_BYTE_SIZE, 1) == log2(_LMQ_TOTAL_BYTE_SIZE, -1) )

        alu_op[@/**/NAME/**/_next_body_offset, @/**/NAME/**/_next_body_offset, +, (ITEM_SIZE*4)]
        alu[@/**/NAME/**/_next_body, @/**/NAME/**/_next_body_offset, AND, @/**/NAME/**/_mask]
        alu[@/**/NAME/**/_next_body, @/**/NAME/**/_base, +, @/**/NAME/**/_next_body]
        alu[@/**/NAME/**/_bodycount, @/**/NAME/**/_bodycount, -, 1]

    #else

        #error This mode is not supported

    #endif

    _lmqueue_epilogue()

#endm


/** Write queue variable to local csr
 *
 * @b Example:
 * @code
 * #define SOME_QUEUE _SOME_QUEUE, 0, 64, 4
 * lmqueue_local_csr_wr(ACTIVE_LM_ADDR_0, SOME_QUEUE, tail)
 * @endcode
 *
 * @param register         Constant, name of local csr
 * @param NAME..ITEM_SIZE  Constants, queue parameters
 * @param var              Constant, name of queue variable, one of:
 *                         base, count, bodycount, mask, top,
 *                         head, next_head, next_head_offset,
 *                         tail, next_tail, next_tail_offset,
 *                         body, next_body, next_body_offset
 *
 * @instruction_cnt 1
 */
#macro lmqueue_local_csr_wr(register, NAME, FLAGS, NUM_ITEMS, ITEM_SIZE, var)

    local_csr_wr[register, @/**/NAME/**/_/**/var]

#endm

/** @}
 * @}
 */

#endif // __LMQUEUE_UC__
