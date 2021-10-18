/*
 * Copyright (C) 2008-2015 Netronome Systems, Inc.  All rights reserved.
 */

/* NFP definitions */

#ifndef __NFP_H__
#define __NFP_H__

#ifndef NFP_LIB_ANY_NFCC_VERSION
    #if (!defined(__NFP_TOOL_NFCC) ||                       \
        (__NFP_TOOL_NFCC < NFP_SW_VERSION(5, 0, 0, 0)) ||   \
        (__NFP_TOOL_NFCC > NFP_SW_VERSION(6, 1, 0, 255)))
        #error "This standard library is not supported for the version of the SDK currently in use."
    #endif
#endif

#include <stdint.h>

/*
 * convenience macros
 */

#define __atomic        __declspec(atomic)
#define __sram          __declspec(sram)
#define __imem          __declspec(imem)
#define __imem_n(x)     __declspec(imem##x)
#define __emem          __declspec(emem)
#define __emem_n(x)     __declspec(emem##x)
#define __dram          __declspec(dram)
#define __mem           __declspec(mem)
#define __cls           __declspec(cls)
#define __cls_n(x)      __declspec(cls##x)
#define __ctm           __declspec(ctm)
#define __ctm_n(x)      __declspec(ctm##x)
#define __lmem          __declspec(local_mem)
#define __export        __declspec(export)
#define __import        __declspec(import)
#define __shared        __declspec(shared)
#define __visible       __declspec(visible)
#define __remote        __declspec(remote)
#define __addr40        __declspec(addr40)
#define __addr32        __declspec(addr32)
#define __xread         __declspec(read_reg)
#define __xwrite        __declspec(write_reg)
#define __xrw           __declspec(read_write_reg)
#define __gpr           __declspec(gp_reg)
#define __nnr           __declspec(nn_local_reg)
#define __huge          __declspec(huge)

#define __mem32         __declspec(addr32 mem)
#define __mem40         __declspec(addr40 mem)
#define __cls32         __declspec(addr32 cls)
#define __cls40         __declspec(addr40 cls)
#define __ctm32         __declspec(addr32 ctm)
#define __ctm40         __declspec(addr40 ctm)

#define __align(x)      __declspec(aligned(x))
#define __align2        __declspec(aligned(2))
#define __align4        __declspec(aligned(4))
#define __align8        __declspec(aligned(8))
#define __align16       __declspec(aligned(16))
#define __align32       __declspec(aligned(32))
#define __align64       __declspec(aligned(64))
#define __align128      __declspec(aligned(128))
#define __align256      __declspec(aligned(256))
#define __align512      __declspec(aligned(512))
#define __align1K       __declspec(aligned(SZ_1K))
#define __align2K       __declspec(aligned(SZ_2K))
#define __align4K       __declspec(aligned(SZ_4K))
#define __align8K       __declspec(aligned(SZ_8K))
#define __align16K      __declspec(aligned(SZ_16K))
#define __align32K      __declspec(aligned(SZ_32K))
#define __align64K      __declspec(aligned(SZ_64K))
#define __align128K     __declspec(aligned(SZ_128K))
#define __align256K     __declspec(aligned(SZ_256K))
#define __align512K     __declspec(aligned(SZ_512K))
#define __align1M       __declspec(aligned(SZ_1M))
#define __align2M       __declspec(aligned(SZ_2M))
#define __align4M       __declspec(aligned(SZ_4M))
#define __align8M       __declspec(aligned(SZ_8M))
#define __align16M      __declspec(aligned(SZ_16M))
#define __align32M      __declspec(aligned(SZ_32M))
#define __align64M      __declspec(aligned(SZ_64M))
#define __align128M     __declspec(aligned(SZ_128M))
#define __align256M     __declspec(aligned(SZ_256M))
#define __align512M     __declspec(aligned(SZ_512M))
#define __align1G       __declspec(aligned(SZ_1G))
#define __align2G       __declspec(aligned(SZ_2G))
#define __packed        __declspec(packed)
#define __packed_bits   __declspec(packed_bits)


#define LoadTimeConstant(a)     __LoadTimeConstant(a)
#define ctx()                   __ctx()
#define free_write_buffer(a)    __free_write_buffer(a)
#define global_label(a)         __global_label(a)
#define lmpt_reserve            __lmpt_reserve
#define n_ctx()                 __n_ctx()
#define nctx_mode()             __nctx_mode()
#define wait_for_all            __wait_for_all
#define wait_for_all_single     __wait_for_all_single
#define wait_for_any            __wait_for_any
#define wait_for_any_single     __wait_for_any_single

/*
 * inline assembler attributes
 */
#define ASM_HAS_JUMP            0x0001
#define LITERAL_ASM             0x0002
#define LITERAL_ASM_HAS_JUMP    0x0003

#define __nfp6000   0x80000
#define __nfp6xxx   __nfp6000
#define __nfp3800   0x100000
#define __nfp38xx   __nfp3800

#define __nfp_stepping(major,minor)  (((major-'A')<<4)|minor&0xf)

#define __REVISION_A0  __nfp_stepping('A', 0)
#define __REVISION_A1  __nfp_stepping('A', 1)
#define __REVISION_B0  __nfp_stepping('B', 0)
#define __REVISION_C0  __nfp_stepping('C', 0)

#define __is_in_mem             __is_in_dram                                /* content pointed to is in CTM. EMEM or IMEM, or p==NULL */
#define __is_read_write_reg(_x) (__is_read_reg(_x) && __is_write_reg(_x))   /* must be read_reg and write_reg */

#if !defined(offsetof)
#define offsetof(s,m)  ((size_t)&(((s *)0)->m))
#endif



/** @addtogroup signal Synchronization Intrinsics
 *  @{
 * This section describes the data types and functions used for synchronization.
 */

/** SIGNAL_MASK data type is used for masks specifying one or more signal registers. */
typedef int32_t SIGNAL_MASK;

/** SIGNAL data type is used to declare signal variables. */
typedef __declspec(signal) int32_t SIGNAL;

/** SIGNAL_PAIR container type for even/odd signal pair. */
typedef __declspec(signal_pair) struct SIGNAL_PAIR {
    int32_t even;
    int32_t odd;
} SIGNAL_PAIR;

/** Sync types for I/O operations. */
typedef enum
{
    sig_done,           /**< Continue execution and set signal when operation is done. */
    ctx_swap            /**< Swap thread out and wait until operation is complete.  */
} sync_t;

/** Signal types for ctx_wait other than mask. */
typedef enum
{
    kill,       /**< Suspend the context.
                 * The kill signal puts the context into the Sleep state and does not return to the Ready state.
                 */
    voluntary,  /**< Put the context in the sleep state.
                 * The voluntary signal puts the context into the Sleep state. The context is
                 * put back into the Ready state in one cycle since the Voluntary Event Signal is always set.
                 */
    bpt         /** The bpt (breakpoint) stops all contexts, interrupts the ARM processor, and puts the current
                 * context into the Sleep state. It also sets the CTX_Enable[Breakpoint] bit.
                 * This value is typically used for debugging purposes. For more information the use of this
                 * value, refer to the ctx_arb instruction in the @prm
                 */
} signal_t;
/**
 * @}
 */


/** @addtogroup csr Control and Status Register Access Intrinsics
 * @{
 */

/** This enumeration specifies the CSRs used in local_csr_read and
 * local_csr_write functions.
 */
typedef enum
{
    local_csr_ustore_address                = 0x000,    /**< Used to load programs into the Control Store.   */
    local_csr_ustore_data_lower             = 0x001,    /**< Control Store Data - lower.   */
    local_csr_ustore_data_upper             = 0x002,    /**< Control Store Data - upper.   */
    local_csr_ustore_error_status           = 0x003,    /**< ECC errors during Control Store reads.   */
    local_csr_alu_out                       = 0x004,    /**< Debug to show state of ALU.  */
    local_csr_ctx_arb_cntl                  = 0x005,    /**< Context Arbiter Control - used by the context
                                                            arbiter and for debug.
                                                        */
    local_csr_ctx_enables                   = 0x006,    /**< Context Enables - used by the context arbiter
                                                            and for debug.
                                                        */
    local_csr_cc_enable                     = 0x007,    /**< Condition Code Enable.   */
    local_csr_csr_ctx_pointer               = 0x008,    /**< CSR Context Pointer.   */

    local_csr_pc_breakpoint_0               = 0x009,    /**< PC Breakpoint 1 - PCB system. */
    local_csr_pc_breakpoint_1               = 0x00a,    /**< PC Breakpoint 1 - PCB system. */
    local_csr_pc_breakpoint_status          = 0x00b,    /**< PC Breakpoint - Status register associated with the PCB system. */
    local_csr_lm_register_error_status      = 0x00c,    /**< Information about parity errors detected on Datapath Regs. */
    local_csr_lm_error_status               = 0x00d,    /**< Status on ECC errors recorded on Local Memory reads. */
    local_csr_lm_error_mask                 = 0x00e,    /**< Controls Error Injection bits into an LM data-path word. */

    local_csr_indirect_ctx_sts              = 0x010,    /**< Indirect Context Status Register.   */
    local_csr_active_ctx_sts                = 0x011,    /**< Active Context Status Register.  */

    local_csr_indirect_ctx_sig_events       = 0x012,    /**< Indirect Context Signal Events Register.  */
    local_csr_active_ctx_sig_events         = 0x013,    /**< Active Context Signal Events Register   */
    local_csr_indirect_ctx_wakeup_events    = 0x014,    /**< Indirect Context Wakeup Events Register.   */
    local_csr_active_ctx_wakeup_events      = 0x015,    /**< Active Context Wakeup Events Register.   */
    local_csr_indirect_ctx_future_count     = 0x016,    /**< Indirect Context Future Count Register.   */
    local_csr_active_ctx_future_count       = 0x017,    /**< Active Context Future Count Register.   */
    local_csr_byte_index                    = 0x01C,    /**< Byte Index Register.   */

    local_csr_t_index                       = 0x01D,    /**< Transfer Index Register.   */
    local_csr_indirect_future_count_signal  = 0x01E,    /**< Which signal to set when FUTURE_COUNT == TIMESTAMP.   */
    local_csr_active_future_count_signal    = 0x01F,    /**< Which signal to set when FUTURE_COUNT == TIMESTAMP.   */
    local_csr_nn_put                        = 0x020,    /**< Next Neighbor Put Register.   */
    local_csr_nn_get                        = 0x021,    /**< Next Neighbor Get Register.   */

    local_csr_indirect_lm_addr_0            = 0x018,    /**< Indirect Local Memory Address 0 Register.   */
    local_csr_active_lm_addr_0              = 0x019,    /**< Active Local Memory Address 0 Register.   */
    local_csr_indirect_lm_addr_1            = 0x01A,    /**< Indirect Local Memory Address 1 Register.   */
    local_csr_active_lm_addr_1              = 0x01B,    /**< Active Local Memory Address 1 Register.   */

    local_csr_indirect_lm_addr_2            = 0x024,    /**< Indirect Local Memory Address 2 Register.   */
    local_csr_active_lm_addr_2              = 0x025,    /**< Active Local Memory Address 2 Register.   */
    local_csr_indirect_lm_addr_3            = 0x026,    /**< Indirect Local Memory Address 3 Register.   */
    local_csr_active_lm_addr_3              = 0x027,    /**< Active Local Memory Address 3 Register.   */

    local_csr_indirect_lm_addr_0_byte_index = 0x038,    /**< Alias of IndLMAddr0 and ByteIndex.   */
    local_csr_active_lm_addr_0_byte_index   = 0x039,    /**< Alias of ActLMAddr0 and ByteIndex.   */
    local_csr_indirect_lm_addr_1_byte_index = 0x03A,    /**< Alias of IndLMAddr1 and ByteIndex.   */
    local_csr_active_lm_addr_1_byte_index   = 0x03B,    /**< Alias of ActLMAddr1 and ByteIndex.   */

    local_csr_indirect_lm_addr_2_byte_index = 0x028,    /**< Alias of IndLMAddr2 and ByteIndex.   */
    local_csr_active_lm_addr_2_byte_index   = 0x029,    /**< Alias of ActLMAddr2 and ByteIndex.   */
    local_csr_indirect_lm_addr_3_byte_index = 0x02A,    /**< Alias of IndLMAddr3 and ByteIndex.   */
    local_csr_active_lm_addr_3_byte_index   = 0x02B,    /**< Alias of ActLMAddr3 and ByteIndex.   */

    local_csr_indirect_predicate_cc         = 0x02C,    /**< Indirect Predicate CC select. */

    local_csr_t_index_byte_index            = 0x03D,    /**< This register is used when Transfer registers
                                                            are accessed via indexed mode.
                                                        */

    local_csr_timestamp_low                 = 0x030,    /**< Timestamp is 64 bits. It counts up by one every
                                                            sixteen cycles.
                                                        */
    local_csr_timestamp_high                = 0x031,    /**< Timestamp is 64 bits. It counts up by one every
                                                            sixteen cycles.
                                                        */
    local_csr_next_neighbor_signal          = 0x040,    /**< Signal a Context in Next Neighbor.   */
    local_csr_prev_neighbor_signal          = 0x041,    /**< Signal a Context in Previous Neighbor.   */
    local_csr_same_me_signal                = 0x042,    /**< Signal another Context in same Microengine.   */
    local_csr_crc_remainder                 = 0x050,    /**< Result of the CRC operation after a crc instruction.   */
    local_csr_profile_count                 = 0x051,    /**< The profile count is used for code profiling
                                                            and tuning.
                                                        */
    local_csr_pseudo_random_number          = 0x052,    /**< Random number generator.   */
    local_csr_misc_control                  = 0x058,    /**< Miscellaneous Control Register.   */

    pc_breakpoint_0_mask                    = 0x059,    /**< Mask register associated with PC Breakpoint 0. */
    pc_breakpoint_1_mask                    = 0x05a,    /**< Mask register associated with PC Breakpoint 1. */

    local_csr_mailbox0                      = 0x05c,    /**< Mailbox Register 0.  */
    local_csr_mailbox1                      = 0x05d,    /**< Mailbox Register 1.   */
    local_csr_mailbox2                      = 0x05e,    /**< Mailbox Register 2.   */
    local_csr_mailbox3                      = 0x05f,    /**< Mailbox Register 3.   */
    local_csr_mailbox_0                     = 0x05c,    /**< Alias of Mailbox0.    */
    local_csr_mailbox_1                     = 0x05d,    /**< Alias of Mailbox1.   */
    local_csr_mailbox_2                     = 0x05e,    /**< Alias of Mailbox2.   */
    local_csr_mailbox_3                     = 0x05f,    /**< Alias of Mailbox3.   */

    local_csr_cmd_indirect_ref0             = 0x064,    /**< Indirect reference to control register 0 */
    local_csr_cmd_indirect_ref1             = 0x065,    /**< Indirect reference to control register 1 */

    local_csr_cmd_indirect_ref_0            = 0x064,    /**< Alias of local_csr_cmd_indirect_ref0 */
    local_csr_cmd_indirect_ref_1            = 0x065,    /**< Alias of local_csr_cmd_indirect_ref1 */

    local_csr_reserved                      = 0x0FF     /**< Reserved  */
} local_csr_t;



/**  Active_CTX_Status CSR format.
 *
 * The Active_CTX_Status CSR maintains the context number of the Context currently executing.
 * Each Microengine supports eight contexts (0 through 7).
 */
typedef union
{
    struct
    {
        unsigned int AB0: 1;            /**< If set, the Microengine has a Context in the Executing state.
                                        *   If clear, no Context is in Executing State.
                                        */
        unsigned int ISLAND_ID: 6;      /**< Island number where the ME is instantiated. */
        unsigned int ATXPC: 17;         /**< Program count of the Executing Context. Only valid if AB0 is a 1.
                                        *   Valid values are 0 to 131071.
                                        *   This field provides a snapshot value of the PC. This value is
                                        *   used for tracking/code profiling purposes. When issued as
                                        *   a local_csr_read from the Microengine, the PC value may
                                        *   not be the exact PC value of the local_csr_rd instruction.
                                        */
        unsigned int RESERVED: 1;       /**< Reserved. */
        unsigned int ME_ID: 4;          /**< The unique ME number which identifies it within the
                                        *   cluster where it happens to be instantiated.
                                        */
        unsigned int ACNO: 3;           /**< The number of the Executing context. Only valid if AB0 bit is a 1.*/
    };
    unsigned int value;                 /**< Accessor to all bits simultaneously. */
} ACTIVE_CTX_STS_t;


/**
 * @}
 */


/**
 * @addtogroup misc Miscellaneous Intrinsics
 * @{
 */

/** Switch annotation.
 *
 * @note "Pack" refers to pack case body to the same size so jump[] src can be calculated by
 * zero-based index multiply max-size.
 */
typedef enum
{
    swpack_none,    /**< No pack, jump[] to a sequence of branch.                       */
    swpack_lmem,    /**< No pack, use local memory to hold branch table.                */
    swpack_auto,    /**< Auto pack when appropriate.                                    */
    swpack_1,       /**< Pack it, using up to 1 extra instruction to compute new src.   */
    swpack_2,       /**< Pack it, using up to 2 extra instructions to compute new src.  */
    swpack_3,       /**< Pack it, using up to 3 extra instructions to compute new src.  */
    swpack_4,       /**< Pack it, using up to 4 extra instructions to compute new src.  */
    swpack_5,       /**< Pack it, using up to 5 extra instructions to compute new src.  */
    swpack_6,       /**< Pack it, using up to 6 extra instructions to compute new src.  */
    swpack_7,       /**< Pack it, using up to 7 extra instructions to compute new src.  */
    swpack_8,       /**< Pack it, using up to 8 extra instructions to compute new src.  */
} swpack_t;

/**
 * @}
 */


__intrinsic void local_csr_write(local_csr_t csr, uint32_t data);
__intrinsic uint32_t local_csr_read(local_csr_t csr);

__intrinsic uint32_t dbl_shr(uint32_t srcA, uint32_t srcB, uint32_t shift_cnt);
__intrinsic uint32_t dbl_shl(uint32_t srcA, uint32_t srcB, uint32_t shift_cnt);


#define SZ_512          (1U <<  9)
#define SZ_1K           (1U << 10)
#define SZ_2K           (1U << 11)
#define SZ_4K           (1U << 12)
#define SZ_8K           (1U << 13)
#define SZ_16K          (1U << 14)
#define SZ_32K          (1U << 15)
#define SZ_64K          (1U << 16)
#define SZ_128K         (1U << 17)
#define SZ_256K         (1U << 18)
#define SZ_512K         (1U << 19)
#define SZ_1M           (1U << 20)
#define SZ_2M           (1U << 21)
#define SZ_4M           (1U << 22)
#define SZ_8M           (1U << 23)
#define SZ_16M          (1U << 24)
#define SZ_32M          (1U << 25)
#define SZ_64M          (1U << 26)
#define SZ_128M         (1U << 27)
#define SZ_256M         (1U << 28)
#define SZ_512M         (1U << 29)
#define SZ_1G           (1U << 30)
#define SZ_2G           (1U << 31)

#define __is_log2(val)  ((((val)-1)&(val))==0)

#define __log2(val)          \
    ((val) >= SZ_2G   ? 31 : \
     (val) >= SZ_1G   ? 30 : \
     (val) >= SZ_512M ? 29 : \
     (val) >= SZ_256M ? 28 : \
     (val) >= SZ_128M ? 27 : \
     (val) >= SZ_64M  ? 26 : \
     (val) >= SZ_32M  ? 25 : \
     (val) >= SZ_16M  ? 24 : \
     (val) >= SZ_8M   ? 23 : \
     (val) >= SZ_4M   ? 22 : \
     (val) >= SZ_2M   ? 21 : \
     (val) >= SZ_1M   ? 20 : \
     (val) >= SZ_512K ? 19 : \
     (val) >= SZ_256K ? 18 : \
     (val) >= SZ_128K ? 17 : \
     (val) >= SZ_64K  ? 16 : \
     (val) >= SZ_32K  ? 15 : \
     (val) >= SZ_16K  ? 14 : \
     (val) >= SZ_8K   ? 13 : \
     (val) >= SZ_4K   ? 12 : \
     (val) >= SZ_2K   ? 11 : \
     (val) >= SZ_1K   ? 10 : \
     (val) >= 512     ?  9 : \
     (val) >= 256     ?  8 : \
     (val) >= 128     ?  7 : \
     (val) >= 64      ?  6 : \
     (val) >= 32      ?  5 : \
     (val) >= 16      ?  4 : \
     (val) >= 8       ?  3 : \
     (val) >= 4       ?  2 : \
     (val) >= 2       ?  1 : \
     (val) >= 1       ?  0 : -1)

#define __is_aligned(base,align)  ((((unsigned)(base))&((align)-1))==0)

#define __MEID    __LoadTimeConstant("__UENGINE_ID")
#define __MENUM   __LoadTimeConstant("__menum")
#define __ISLAND  __LoadTimeConstant("__island")


/*
 * compiler intrinsic functions
 */
   int64_t __LoadTimeConstant();
   void __assign_relative_register();
/* LEGACY __associate_dram_read_write_reg_pair_no_spill */
   void __associate_read_write_reg_pair_no_spill();
/* LEGACY __associate_sram_read_write_reg_pair_no_spill */
   void __critical_path();
   void __ct_Qperfinfo();
   void __ct_assert();
   uint32_t __ctx();
/* UNPROT __disam_assert_on_no_overlap */
/* UNPROT __disam_assert_on_overlap */
   int32_t __elem_size();
   void __free_write_buffer();
   void __global_label();
/* UNPROT __gp_reg_number */
   void __implicit_read();
   void __implicit_undef();
   void __implicit_write();
   void __impossible_path();
/* UNPROT __index_xfer */
   void __intrinsic_begin();
   void __intrinsic_end();
   int32_t __is_ct_const();
/* LEGACY __is_dram_read_reg */
/* LEGACY __is_dram_reg */
/* LEGACY __is_dram_write_reg */
   int32_t __is_in_cls();
   int32_t __is_in_ctm();
   int32_t __is_in_dram();
   int32_t __is_in_emem();
   int32_t __is_in_emem_cache();
   int32_t __is_in_imem();
   int32_t __is_in_lmem();
   int32_t __is_in_reg();
   int32_t __is_in_reg_or_lmem();
/* LEGACY __is_in_scratch */
   int32_t __is_in_sram();
/* LEGACY __is_in_ustore */
   int32_t __is_nfp_arch();
   int32_t __is_nfp_arch_or_above();
   int32_t __is_read_reg();
/* LEGACY __is_sram_read_reg */
/* LEGACY __is_sram_reg */
/* LEGACY __is_sram_write_reg */
   int32_t __is_write_reg();
   int32_t __is_xfer_reg();
   int64_t __link_sym();
   int64_t __load_expr();
   uint32_t __n_ctx();
   uint32_t __nctx_mode();
   int32_t __nfp_idstr2meid();
   int32_t __nfp_meid();
   void __no_spill_begin();
   void __no_spill_end();
   void __no_swap_begin();
   void __no_swap_end();
/* PRINTF __pflush */
/* PRINTF __plinebuf */
/* PRINTF __printf */
/* PRINTF __put */
/* PRINTF __puthc */
/* PRINTF __puthi */
/* PRINTF __puthll */
/* PRINTF __putns */
/* PRINTF __puts */
/* PRINTF __putsi */
/* PRINTF __putsll */
/* PRINTF __putui */
/* PRINTF __putull */
/* UNPROT __reserve_relative_register */
   int32_t __signal_number();
   SIGNAL_MASK __signals();
   void __switch_pack();
   void __timer_report();
   void __timer_report_all_ctx();
   void __timer_reset();
   void __timer_start();
   void __timer_stop();
   void __timer_strobe();
   void __wait_for_all();
   void __wait_for_all_single();
   void __wait_for_any();
   void __wait_for_any_single();
   int32_t __xfer_reg_number();
/* PRINTF printf */
   int32_t signal_test();
   int32_t ua_get_s16();
   int32_t ua_get_s32();
   int64_t ua_get_s64();
   int32_t ua_get_s8();
   uint32_t ua_get_u16();
   uint32_t ua_get_u32();
   uint64_t ua_get_u64();
   uint32_t ua_get_u8();
   uint32_t ua_get_u8_cls();
/* LEGACY ua_get_u8_dram */
/* LEGACY ua_get_u8_scratch */
   uint32_t ua_get_u8_sram();
   void ua_memcpy();
   void ua_set_16();
   void ua_set_32();
   void ua_set_64();
   void ua_set_8();

#ifdef __NFCC_NG
   uint32_t ua_get_u8_mem();
   uint32_t __ilexpr();
   uint32_t __vlexpr();
#else
   uint64_t __alloc_resource();
#endif

   void __lmpt_reserve(void);

#endif /* __NFP_H__ */
