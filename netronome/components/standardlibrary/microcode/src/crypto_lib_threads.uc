/*
 * Copyright (C) 2014 Netronome Systems, Inc. All rights reserved.
 */


#ifndef __CRYPTO_LIB_THREADS_UC__
#define __CRYPTO_LIB_THREADS_UC__

#ifndef NFP_LIB_ANY_NFAS_VERSION
    #if (!defined(__NFP_TOOL_NFAS) ||                       \
        (__NFP_TOOL_NFAS < NFP_SW_VERSION(5, 0, 0, 0)) ||   \
        (__NFP_TOOL_NFAS > NFP_SW_VERSION(6, 1, 0, 255)))
        #error "This standard library is not supported for the version of \
                  the SDK currently in use."
    #endif
#endif

/** @file crypto_lib_threads.uc CRYPTO Threads Operation Macros
 * @addtogroup cryptothreads CRYPTO Threads Operation
 * @{
 *
 * @name CRYPTO Threads Operation Macros
 * @{
 *
 *  Crypto logic support macros facilitating encryption, decryption, and
 *  authentication of ip packets. Utilizes multiple crypto cores, packet buffers,
 *  and threads in a pipeline in order to maximize utilization of crypto hardware
 *  and maximize packet processing performance.
 *
 *  Intended to be run on one or more microengines in the crypto island of
 *  NFP6000
 */

// API Quick Reference:
//
// crypto_threads_input(_me_ctx, _crypto_ctx, _in_ring_type, _in_ring_num )
// crypto_threads_output(_me_ctx, _crypto_ctx )
// crypto_threads_init()


#include "aggregate.uc"
#include "crypto_lib.uc"
#include "crypto_lib_ipsec.uc"
#include "localmem.uc"
#include "limit.uc"
#include "sig_macros.uc"
#include "thread.uc"
#include "threads.uc"
#include "ring_utils.uc"
#include "stdmac.uc"

#include "../include/crypto_lib_threads.h" // include shared defines

//
// When using crypto_threads_init, the build system or another file should define
// CRYPTO_NUM_THREADS to be the number of threads to run on the micro engine, and
// CRYPTO_START_CTX to be the 1st crypto core/ctx to be used by the micro engine.
// Refer to description of crypto_threads_init for details.
//

#ifndef CRYPTO_NUM_THREADS
#define CRYPTO_NUM_THREADS 6
#endif

#ifndef CRYPTO_START_CTX
#define CRYPTO_START_CTX 0
#endif

// ring type
#define CRYPTO_RING_CTM  0
#define CRYPTO_RING_EMU0 1
#define CRYPTO_RING_EMU1 2
#define CRYPTO_RING_EMU2 3
#define CRYPTO_RING_WQ   4

// ring size ( in lwords )
#define CRYPTO_RING_SIZE_128   128
#define CRYPTO_RING_SIZE_256   256
#define CRYPTO_RING_SIZE_512   512
#define CRYPTO_RING_SIZE_1K   1024
#define CRYPTO_RING_SIZE_2K   2048
#define CRYPTO_RING_SIZE_4K   4096
#define CRYPTO_RING_SIZE_8K   8192
#define CRYPTO_RING_SIZE_16K 16384
#define CRYPTO_RING_SIZE_32K 32768
#define CRYPTO_RING_SIZE_64K 64536


// Note: below configuration definitions can be overriden by user by adding
// a #define for the definition prior to inclusion of this file

// override the default response ring code, in order to customize the behavior
// for a specific application. user should #define OVERRIDE_RESPONSE_RING 1 and
// define a #macro crypto_threads_send_response () to handle output thread
// generation of response status
#ifndef OVERRIDE_RESPONSE_RING
#define OVERRIDE_RESPONSE_RING 0
#endif
// response ring type
#ifndef RESPONSE_RING_TYPE
#define RESPONSE_RING_TYPE CRYPTO_RING_CTM
#endif
// buffer ring type (for return of 'start of packet' buffers used to
// send re-generated beginning of packet during encryption operations )
// buffer ring is allocated and initialized by user
#ifndef BUF_RING_TYPE
#define BUF_RING_TYPE CRYPTO_RING_CTM
#endif
// buffer ring number
#ifndef BUF_RING_NUM
#define BUF_RING_NUM 0
#endif
// buffer ring island id. set to island id of
// island containing ctm based buffer ring.
// 0x00 means same island crypto_lib_threads is running on.
#ifndef BUF_RING_ISLAND_ID
#define BUF_RING_ISLAND_ID 0x00
#endif


// INIT_FLAG_USE_ALLOC_MEM == 1 causes the nfas/loader directive to be used
// to allocate init flag memory. Note that init flag is now in CLS.
#ifndef INIT_FLAG_USE_ALLOC_MEM
#define INIT_FLAG_USE_ALLOC_MEM 1
#endif
// when using alloc_mem, user can specify a fixed memory location at which to
// locate the init flag. otherwise loader will pick location
#ifndef INIT_FLAG_ALLOCATION_FIXED
#define INIT_FLAG_ALLOCATION_FIXED 0
#endif
// Set the initialization flag location
#ifndef INIT_FLAG_ADDR
#define INIT_FLAG_ADDR 0x00000020
#endif
// For linker, need to manually specify island here
#ifndef INIT_FLAG_ISLAND
#define INIT_FLAG_ISLAND 12
#endif
#if INIT_FLAG_ISLAND == 12
#define INIT_FLAG_ISLAND_TXT i12
#else
#define INIT_FLAG_ISLAND_TXT i13
#endif
#if ( INIT_FLAG_USE_ALLOC_MEM == 1 )
    #if (INIT_FLAG_ALLOCATION_FIXED == 1 )
        .alloc_mem crypto_init_flag INIT_FLAG_ISLAND_TXT.cls+INIT_FLAG_ADDR global 4 4 addr40
    #else
        .alloc_mem crypto_init_flag INIT_FLAG_ISLAND_TXT.cls global 4 4 addr40
    #endif
#else
    #define crypto_init_flag INIT_FLAG_ADDR
#endif

// enable use of SA 'flush' mechanism
#ifndef ENABLE_SA_FLUSH
#define ENABLE_SA_FLUSH     0
#endif
#if ( ENABLE_SA_FLUSH == 1 )
// SA_FLUSH_FLAGS_USE_ALLOC_MEM == 1 causes the nfas/loader directive to be used
// to allocate sa flush flags memory. Note that flags are now in CLS.
#ifndef SA_FLUSH_FLAGS_USE_ALLOC_MEM
#define SA_FLUSH_FLAGS_USE_ALLOC_MEM 1
#endif
// when using alloc_mem, user can specify a fixed memory location at which to
// locate the sa flush flags. otherwise loader will pick location
#ifndef SA_FLUSH_FLAGS_ALLOCATION_FIXED
#define SA_FLUSH_FLAGS_ALLOCATION_FIXED 0
#endif
// Set the sa flush flags location
#ifndef SA_FLUSH_FLAGS_ADDR
#define SA_FLUSH_FLAGS_ADDR 0x00000024
#endif
#if ( SA_FLUSH_FLAGS_USE_ALLOC_MEM == 1 )
    #if (SA_FLUSH_FLAGS_ALLOCATION_FIXED == 1 )
        .alloc_mem crypto_sa_flush_flags cls+SA_FLUSH_FLAGS_ADDR island 4 4 addr32
    #else
        .alloc_mem crypto_sa_flush_flags cls island 4 4 addr32
    #endif
#else
    #define crypto_sa_flush_flags SA_FLUSH_FLAGS_ADDR
#endif
#endif // ENABLE_SA_FLUSH == 1


// REQ_RING_TYPE is one of CRYPTO_RING_CTM, EMU0, EMU1, or EMU2
#ifndef REQ_RING_TYPE
#define REQ_RING_TYPE CRYPTO_RING_CTM
#endif
// REQ_RING_USE_ONE_EMU_RING is for running with multiple
// crypto islands, all using a single req ring in emu
#ifndef REQ_RING_USE_ONE_EMU_RING
#define REQ_RING_USE_ONE_EMU_RING 0
#endif
// REQ_RING_SIZE is one of CRYPTO_RING_SIZE_N, in lwords
#ifndef REQ_RING_SIZE
#define REQ_RING_SIZE CRYPTO_RING_SIZE_2K
#endif
// REQ_RING_NUM is the ring number, 0-13 for CTM or 0-1024 for EMU
#ifndef REQ_RING_NUM
#define REQ_RING_NUM 0
#endif
// REQ_RING_USE_ALLOC_MEM == 1 causes the nfas/loader directive to be used
// to allocate request ring memory
#ifndef REQ_RING_USE_ALLOC_MEM
#define REQ_RING_USE_ALLOC_MEM 1
#endif
// when using alloc_mem, user can specify a fixed memory location at which to
// locate the request ring. otherwise loader will pick location. has to be ring
// size aligned
#ifndef REQ_RING_ALLOCATION_FIXED
#define REQ_RING_ALLOCATION_FIXED 0
#endif
// Set the request ring location
#ifndef REQ_RING_ADDR
#define REQ_RING_ADDR 0x00000000
#endif
#ifndef REQ_RING_DESC_ADDR
#define_eval REQ_RING_DESC_ADDR (REQ_RING_ADDR + (REQ_RING_SIZE << 2))
#endif
#if ( REQ_RING_USE_ALLOC_MEM == 1 )
    #define_eval _SIZE (REQ_RING_SIZE << 2) // in bytes here
    #if (REQ_RING_ALLOCATION_FIXED == 1 )
        #define_eval _ADDR REQ_RING_ADDR
        #if ( REQ_RING_TYPE == CRYPTO_RING_CTM )
            .alloc_mem crypto_req_ring ctm+_ADDR island \
                        _SIZE _SIZE addr32
        #elif ( REQ_RING_TYPE == CRYPTO_RING_EMU0 )
            .alloc_mem crypto_req_ring emem0+_ADDR global \
                        _SIZE _SIZE addr40
            .alloc_mem crypto_req_ring_desc emem0+_ADDR global \
                        16 16 addr40
        #elif ( REQ_RING_TYPE == CRYPTO_RING_EMU1 )
            .alloc_mem crypto_req_ring emem1+_ADDR global \
                        _SIZE _SIZE addr40
            .alloc_mem crypto_req_ring_desc emem1+_ADDR global \
                        16 16 addr40
        #elif ( REQ_RING_TYPE == CRYPTO_RING_EMU2 )
            .alloc_mem crypto_req_ring emem2+_ADDR global \
                        _SIZE _SIZE addr40
            .alloc_mem crypto_req_ring_desc emem2+_ADDR global \
                        16 16 addr40
        #else //( REQ_RING_TYPE == CRYPTO_RING_WQ )
            .alloc_mem crypto_req_ring emem+_ADDR global \
                        _SIZE _SIZE addr40
            .alloc_mem crypto_req_ring_desc emem+_ADDR global \
                        16 16 addr40
        #endif
        #undef _ADDR
    #else
        #if ( REQ_RING_TYPE == CRYPTO_RING_CTM )
            .alloc_mem crypto_req_ring ctm island \
                        _SIZE _SIZE addr32
        #elif ( REQ_RING_TYPE == CRYPTO_RING_EMU0 )
            .alloc_mem crypto_req_ring emem0 global \
                        _SIZE _SIZE addr40
            .alloc_mem crypto_req_ring_desc emem0 global \
                        16 16 addr40
        #elif ( REQ_RING_TYPE == CRYPTO_RING_EMU1 )
            .alloc_mem crypto_req_ring emem1 global \
                        _SIZE _SIZE addr40
            .alloc_mem crypto_req_ring_desc emem1 global \
                        16 16 addr40
        #elif ( REQ_RING_TYPE == CRYPTO_RING_EMU2 )
            .alloc_mem crypto_req_ring emem2 global \
                        _SIZE _SIZE addr40
            .alloc_mem crypto_req_ring_desc emem2 global \
                        16 16 addr40
        #else //( REQ_RING_TYPE == CRYPTO_RING_WQ )
            .alloc_mem crypto_req_ring emem global \
                        _SIZE _SIZE addr40
            .alloc_mem crypto_req_ring_desc emem global \
                        16 16 addr40
        #endif
    #endif
#else
    #define crypto_req_ring REQ_RING_ADDR
    #define crypto_req_ring_desc REQ_RING_DESC_ADDR
#endif


// override the default counter update code, in order to customize the behavior
// for a specific application. user should #define OVERRIDE_UPDATE_COUNTER 1 and
// define a #macro crypto_threads_update_counter () to handle counter updates,
// and a #macro crypto_threads_clear_counters() to clear the counters.
#ifndef OVERRIDE_UPDATE_COUNTER
#define OVERRIDE_UPDATE_COUNTER 0
#endif
// when using default counter update code, user can specify location of counters
// in either crypto island cls or ctm using #define COUNTER_MEMORY_TYPE.
// default value is cls.
#define COUNTER_MEMORY_TYPE_CLS 0
#define COUNTER_MEMORY_TYPE_CTM 1
#ifndef COUNTER_MEMORY_TYPE
#define COUNTER_MEMORY_TYPE COUNTER_MEMORY_TYPE_CLS
#endif
// COUNTER_USE_ALLOC_MEM == 1 causes the nfas/loader directive to be used
// to allocate counter memory
#ifndef COUNTER_USE_ALLOC_MEM
#define COUNTER_USE_ALLOC_MEM 1
#endif
// when using alloc_mem, user can specify a fixed memory location at which to
// locate the counters. otherwise loader will pick location
#ifndef COUNTER_ALLOCATION_FIXED
#define COUNTER_ALLOCATION_FIXED 0
#endif
// counter base address, when using COUNTER_ALLOCATION_FIXED, or when not using
// alloc_mem
#ifndef COUNTER_BASE_ADDR
#define COUNTER_BASE_ADDR 0x1000 // default starting location of counters
#endif                           // has to be aligned to counter memory size
// counters can be set to be 64 or 32 bits long, in crypto_lib_threads.h
// ( but there seems to be some problem with cls / 64 bit cntrs )
#if ( COUNTER_USE_ALLOC_MEM == 1 )
    #define_eval COUNTER_MEM_SIZE ((CRYPTO_COUNTER_LENGTH/8) * CRYPTO_CNTR_MAX)
    #if (COUNTER_ALLOCATION_FIXED == 1 )
        #if (COUNTER_MEMORY_TYPE == COUNTER_MEMORY_TYPE_CLS)
            .alloc_mem ces_mem_cntrs cls+COUNTER_BASE_ADDR island \
                COUNTER_MEM_SIZE COUNTER_MEM_SIZE addr32
        #elif (COUNTER_MEMORY_TYPE == COUNTER_MEMORY_TYPE_CTM)
            .alloc_mem ces_mem_cntrs ctm+COUNTER_BASE_ADDR island \
                COUNTER_MEM_SIZE COUNTER_MEM_SIZE addr32
        #endif
    #else
        #if (COUNTER_MEMORY_TYPE == COUNTER_MEMORY_TYPE_CLS)
            .alloc_mem ces_mem_cntrs cls island \
                COUNTER_MEM_SIZE COUNTER_MEM_SIZE addr32
        #elif (COUNTER_MEMORY_TYPE == COUNTER_MEMORY_TYPE_CTM)
            .alloc_mem ces_mem_cntrs ctm island \
                COUNTER_MEM_SIZE COUNTER_MEM_SIZE addr32
        #endif
    #endif
#else
    #define ces_mem_cntrs COUNTER_BASE_ADDR
#endif


// enable clear crypto sram on startup, needed on real hardware
// to set ecc bits in case using nfp-mem to examine sram. For
// simulation use can disable this to speed up simulation run.
#ifndef ENABLE_CLEAR_SRAM
#define ENABLE_CLEAR_SRAM 1
#endif

// enable handling for jumbo packets
#ifndef ENABLE_JUMBO_PKTS
#define ENABLE_JUMBO_PKTS  1
#endif


// enable error checking
#ifndef ENABLE_ERROR_CHECKS
#define ENABLE_ERROR_CHECKS  0
#endif


// enable anti-replay handling
#ifndef ENABLE_ANTI_REPLAY
#define ENABLE_ANTI_REPLAY  1
#endif


// save and restore crypto state when continue packet
// across crypto buffers. added only for the purpose of
// testing hardware operation. enables save/restore of
// crypto engine state between aesgcm encrypt 'jumbo'
// start and end packet buffers. By default the state
// is just maintained across buffers and nothing is
// done to the specific crypto engine between buffers
#ifndef ENABLE_CRYPTO_STATE_SAVE_RESTORE
#define ENABLE_CRYPTO_STATE_SAVE_RESTORE 0
#endif


// 'detailed' counter ( i.e. pkt types )
#ifndef ENABLE_DETAILED_COUNTERS
#define ENABLE_DETAILED_COUNTERS 0
#endif


// debug
#ifndef ENABLE_DEBUG_COUNTERS
#define ENABLE_DEBUG_COUNTERS 0
#endif

#ifndef HALT_ON_ERROR
#define HALT_ON_ERROR         0
#endif

/// @cond INTERNAL_MACROS


// min/max crypto ctx, used to reduce code size if not all in use
#define_eval CRYPTO_MIN_CTX CRYPTO_START_CTX
#define_eval CRYPTO_MAX_CTX (CRYPTO_START_CTX + (CRYPTO_NUM_THREADS / 2 ) - 1)


// enable chained commands to crypto cores
#define ENABLE_CHAIN_MODE 1
#if (ENABLE_CHAIN_MODE == 1 )
#define CHAINED          1
#define CHAINED_LAST     1
#define CHAINED_NOT_LAST 0
#else
#define CHAINED          0
#define CHAINED_LAST     0
#define CHAINED_NOT_LAST 0
#endif

// support for performance profiling, has to be defined in
// wrapper file that includes crypto_lib_threads.uc
#ifndef CRYPTO_PROFILE_ENTRY
#define CRYPTO_PROFILE_ENTRY(_ID)
#endif


/**
 *  Request ring entry format
 *
 *   Lword Bits  Description
 *
 *   0     31:24 Status Return Queue Address bits 39:32
 *   0     23:16 Packet Write Back Address 39:32
 *   0        15 Must be set to 1
 *   0        14 Decryption Option
 *               0 - transfer entire packet
 *               1 - transfer only encrypted/decrypted payload data
 *            13 Free Start of Packet Buffer
 *               0 - do not free start of packet buffer
 *               1 - free buffer on cmd completion
 *                   ( writes Start of Packet Address into ring
 *                     passed via parameters to crypto_threads_init )
 *   0        12 Test Flag
 *               1 - test in progress
 *               0 - test not in progress
 *   0     11:10 CTM Buffer Size
 *               0 - 256
 *               1 - 512
 *               2 - 1024
 *               3 - 2048
 *   0       9:0 Status Return Queue Que Number
 *
 *   1     31:24 SA (Security Association ) Address 39:32
 *   1     23:16 Start of Packet Address Bits 39:32
 *   1      15:8 Continuation of Packet Address Bits 39:32
 *   1       7:0  End of Packet Address Bits 39:32
 *
 *   2      31:9 SA Addr 31:9, bits 8:0 implied 0
 *   2       8:0 SA Version
 *
 *   3      31:0 Start of Packet Addr 31:0
 *
 *   4      31:0 Continuation of Packet Addr 31:0
 *
 *   5      31:0 End of Packet Addr 31:0
 *
 *   6      31:0 Packet Write Back Addr 31:0
 *
 *   7     31:16 Start of Packet Length ( Bytes )
 *   7      15:0 Continuation of Packet Length ( Bytes )
 *
 *   8     31:16 End of Packet Length ( Bytes )
 *   8      15:0 Packet Write Back Length ( Bytes )
 *
 *   9     31:16 Packet Write Back End of Packet Length ( Bytes )
 *   9      15:0 Packet Plaintext or Encrypted Text Length (Bytes)
 *                Including Pad Bytes
 *
 *   10    31:16 Authentication Header ( AH or ESP ) Offset
 *   10     15:0 IV Offset
 *
 *   11    31:16 Plaintext or Encrypted Text Offset
 *   11     15:0 Authentication Data Offset
 *
 *   12    31:0  SA Control Word
 *
 *   13    31:12 Spare
 *   13    11:0  User Control Field
 *
 *   14    31:0  Sequence Number 63:32 for ESN
 *
 *   Offsets are byte offset from start of packet. If offset is > start of
 *   packet length, byte offset refers to a byte in the continuation of packet,
 *   or in end of packet if offset is > start of packet length + continuation of
 *   packet length
 */

// request ring entry constants

#define CRYPTO_REQ_LENGTH                    15    // 32 bit words
#define CRYPTO_REQ_ENTRY_VALID               $req[0]
#define CRYPTO_REQ_ENTRY_VALID_BIT           15
#define CRYPTO_REQ_DECRYPT_OPTION            $req[0]
#define CRYPTO_REQ_DECRYPT_OPTION_BIT        14
#define CRYPTO_REQ_FREE_BUF                  $req[0]
#define CRYPTO_REQ_FREE_BUF_BIT              13
#define CRYPTO_REQ_TEST_FLAG                 $req[0]
#define CRYPTO_REQ_TEST_FLAG_BIT             12
#define CRYPTO_REQ_SA_ADDR                   $req[2]
#define CRYPTO_REQ_SA_ADDR_UPPER             $req[1]
#define CRYPTO_REQ_SA_ADDR_UPPER_SHIFT       24
#define CRYPTO_REQ_SA_VERSION_MASK           0x1ff
#define CRYPTO_REQ_PKT_STRT_ADDR             $req[3]
#define CRYPTO_REQ_PKT_STRT_ADDR_UPPER       $req[1]
#define CRYPTO_REQ_PKT_STRT_ADDR_UPPER_SHIFT 16
#define CRYPTO_REQ_PKT_CONT_ADDR             $req[4]
#define CRYPTO_REQ_PKT_CONT_ADDR_UPPER       $req[1]
#define CRYPTO_REQ_PKT_CONT_ADDR_UPPER_SHIFT 8
#define CRYPTO_REQ_PKT_END_ADDR              $req[5]
#define CRYPTO_REQ_PKT_END_ADDR_UPPER        $req[1]
#define CRYPTO_REQ_PKT_END_ADDR_UPPER_SHIFT  0
#define CRYPTO_REQ_PKT_WRBK_ADDR             $req[6]
#define CRYPTO_REQ_PKT_WRBK_ADDR_UPPER       $req[0]
#define CRYPTO_REQ_PKT_WRBK_ADDR_UPPER_SHIFT 16
#define CRYPTO_REQ_PKT_STRT_LENGTH           $req[7]
#define CRYPTO_REQ_PKT_STRT_LENGTH_SHIFT     16
#define CRYPTO_REQ_PKT_CONT_LENGTH           $req[7]
#define CRYPTO_REQ_PKT_CONT_LENGTH_SHIFT     0
#define CRYPTO_REQ_PKT_END_LENGTH            $req[8]
#define CRYPTO_REQ_PKT_END_LENGTH_SHIFT      16
#define CRYPTO_REQ_PKT_WRBK_LENGTH           $req[8]
#define CRYPTO_REQ_PKT_WRBK_LENGTH_SHIFT     0
#define CRYPTO_REQ_PKT_WRBK_END_LENGTH       $req[9]
#define CRYPTO_REQ_PKT_WRBK_END_LENGTH_SHIFT 16
#define CRYPTO_REQ_PTEXT_LENGTH              $req[9]
#define CRYPTO_REQ_PTEXT_LENGTH_SHIFT        0
#define CRYPTO_REQ_AUTH_HDR_OFFSET           $req[10]
#define CRYPTO_REQ_AUTH_HDR_OFFSET_SHIFT     16
#define CRYPTO_REQ_IV_OFFSET                 $req[10]
#define CRYPTO_REQ_IV_OFFSET_SHIFT           0
#define CRYPTO_REQ_PTEXT_OFFSET              $req[11]
#define CRYPTO_REQ_PTEXT_OFFSET_SHIFT        16
#define CRYPTO_REQ_AUTH_DATA_OFFSET          $req[11]
#define CRYPTO_REQ_AUTH_DATA_OFFSET_SHIFT    0
#define CRYPTO_REQ_SA_CONTROL_WORD           $req[12]
#define CRYPTO_REQ_USER_CONTROL_OFFSET_SHIFT 0
#define CRYPTO_REQ_USER_CONTROL              $req[13]
#define CRYPTO_REQ_SEQ_UPPER_SHIFT           0
#define CRYPTO_REQ_SEQ_UPPER                 $req[14]

/**
 *  Response ring entry format
 *
 *   Lword 0: 31    Must be 1
 *   Lword 0: 30    Direction
 *                  0 = Encrypt, 1 = Decrypt
 *   Lword 0: 29:26 Spare
 *   Lword 0: 25:24 CTM Buffer Size, pass through from request
 *   Lword 0: 23:16 Packet Write Back Address Bits 39:32
 *   Lword 0: 15:8  End of Packet Address Bits 39:32
 *   Lword 0: 7:0   Status Return Value one of CRYPTO_RSP_STATUS_*
 *   Lword 1: 31:0  Packet Write Back Addr 31:0
 *   Lword 2: 31:0  End of Packet Addr 31:0
 *   Lword 3: 31:16 End of Packet Length ( Bytes )
 *   Lword 3: 15:0  Packet Write Back Length ( Bytes )
 *   Lword 4: 31:9  SA Address 31:9, Bits 8:0 implied 0
 *   Lword 4: 8:0   SA Version
 *   Lword 5: 31:20 Spare
 *   Lword 5: 19:8  User Control Field
 *   Lword 5: 7:0   Next Header Value
 */


// response ring entry constants

#define CRYPTO_RSP_LENGTH 6

// response status codes in crypto_lib_threads.h


/**
 *  SA (Security Association) Format
 *
 *   Lword 0-7:      Cipher Key ( 8 lwords, up to 32 bytes )
 *   Lword 8-23:     Auth Key ( 16 lwords, up to 64 bytes ) or
 *                   AES-GCM fields:
 *                   Lword 8:    salt
 *                   Lword 9-10  iv
 *                   Lword 11    counter
 *                   Lword 12-15 zeros
 *                   Lword 16-17 len A
 *                   Lword 18-19 len C
 *                   Lword 20-23 unused
 *   Lword 24:       SA Control Word
 *   Lword 24: 31:20 Not used in this code
 *   Lword 24: 19:   Anti-replay window size 1:64 0:32
 *   Lword 24: 18:   Sequence Number Size 1:64 0:32
 *   Lword 24: 17:   Enable Anti-replay handling if 1
 *   Lword 24: 16    Direction, one of CRYPTO_SA_DIR_*
 *   Lword 24: 15:14 Protocol, one of CRYPTO_SA_PROTOCOL_*
 *   Lword 24: 13:12 Mode, one of CRYPTO_SA_PROTMODE_*
 *   Lword 24: 11:8  Cipher, one of CRYPTO_SA_CIPHER_*
 *   Lword 24:  7:4  Cipher Mode, one of CRYPTO_SA_CIMODE_*
 *   Lword 24:  3:0  Hash, one of CRYPTO_SA_HASH_*
 *   Lword 25: 31:0  SPI (Security Parameters Index)
 *   Lword 26-63    Unused by this code
 *   Lword 64       Sequence Number 31:0
 *   Lword 65       Sequence Number 63:32
 *   Lword 66:      Anti-replay window counter 31:0
 *   Lword 67:      Anti-replay window counter 63:32
 *   Lword 68:      Anti-replay window bitmap 31:0
 *   Lword 69:      Anti-replay window bitmap 63:32
 *   Lword 70: 31:1 Unused by this code
 *   Lword 70: 0:   Anti-replay SA lock bit
 *   Lword 71-72    Unused by this code
 *   Lword 73:      Authentication Check Failure Counter
 *   Lword 74:      Mode, Cipher, or Hash Selection Error Counter
 *   Lword 75:      Other Internally Detected Error Counter
 *   Lword 76-78:   Unused by this code
 *   Lword 79:      Anti-replay sequence number too small
 *   Lword 80:      Anti-replay sequence number already received
 *   Lword 81-128:  Unused by this code
 *
 *   NOTE:
 *   Lwords 0-63   may only be accessed using bulk memory operations
 *   Lwords 64-128 may only be accessed using atomic memory operations
 *
 *   CRYPTO_DIR_*, CRYPTO_CIPHER_*, CRYPTO_CIMODE_*, CRYPTO_HASH_*
 *   from crypto_lib.uc
 */

// SA Constants

// field values
#define CRYPTO_SA_PROTOCOL_AH        0
#define CRYPTO_SA_PROTOCOL_ESP       1
#define CRYPTO_SA_PROTOCOL_TLS       2

#define CRYPTO_SA_PROTMODE_TRANSPORT 0
#define CRYPTO_SA_PROTMODE_TUNNEL    1

#define CRYPTO_SA_DIR_ENCRYPT        0
#define CRYPTO_SA_DIR_DECRYPT        1

#define CRYPTO_SA_CIPHER_NULL        0
#define CRYPTO_SA_CIPHER_3DES        1
#define CRYPTO_SA_CIPHER_AES128      2
#define CRYPTO_SA_CIPHER_AES192      3
#define CRYPTO_SA_CIPHER_AES256      4
#define CRYPTO_SA_CIPHER_AES128_NULL 5
#define CRYPTO_SA_CIPHER_AES192_NULL 6
#define CRYPTO_SA_CIPHER_AES256_NULL 7
#define CRYPTO_SA_CIPHER_CHACHA20    8

#define CRYPTO_SA_CIMODE_ECB         0
#define CRYPTO_SA_CIMODE_CBC         1
#define CRYPTO_SA_CIMODE_CFB         2
#define CRYPTO_SA_CIMODE_OFB         3
#define CRYPTO_SA_CIMODE_CTR         4

#define CRYPTO_SA_HASH_NONE          0
#define CRYPTO_SA_HASH_MD5_96        1
#define CRYPTO_SA_HASH_SHA1_96       2
#define CRYPTO_SA_HASH_SHA256_96     3
#define CRYPTO_SA_HASH_SHA384_96     4
#define CRYPTO_SA_HASH_SHA512_96     5
#define CRYPTO_SA_HASH_MD5_128       6
#define CRYPTO_SA_HASH_SHA1_80       7
#define CRYPTO_SA_HASH_SHA256_128    8
#define CRYPTO_SA_HASH_SHA384_192    9
#define CRYPTO_SA_HASH_SHA512_256   10
#define CRYPTO_SA_HASH_GF128_128    11
#define CRYPTO_SA_HASH_POLY1305_128 12

// bit positions within lwords
#define CRYPTO_SA_GEN_SEQ_MASK     0x1
#define CRYPTO_SA_GEN_SEQ_SHIFT     30
#define CRYPTO_SA_AR_WIN_64_MASK   0x1
#define CRYPTO_SA_AR_WIN_64_SHIFT   19
#define CRYPTO_SA_SEQNUM_64_MASK   0x1
#define CRYPTO_SA_SEQNUM_64_SHIFT   18
#define CRYPTO_SA_AR_ENA_MASK      0x1
#define CRYPTO_SA_AR_ENA_SHIFT      17
#define CRYPTO_SA_DIR_MASK         0x1
#define CRYPTO_SA_DIR_SHIFT         16
#define CRYPTO_SA_PROTOCOL_MASK    0x3
#define CRYPTO_SA_PROTOCOL_SHIFT    14
#define CRYPTO_SA_PROTMODE_MASK    0x3
#define CRYPTO_SA_PROTMODE_SHIFT    12
#define CRYPTO_SA_CIPHER_MASK      0xf
#define CRYPTO_SA_CIPHER_SHIFT       8
#define CRYPTO_SA_CIMODE_MASK      0xf
#define CRYPTO_SA_CIMODE_SHIFT       4
#define CRYPTO_SA_HASH_MASK        0xf
#define CRYPTO_SA_HASH_SHIFT         0
#define CRYPTO_SA_AR_LOCK_MASK     0x1
#define CRYPTO_SA_AR_LOCK_SHIFT      0

// Lword offsets to words in the SA
// these should be accessed only with bulk memory ops
#define CRYPTO_SA_CTLWRD_OFFSET   24
#define CRYPTO_SA_KEYS_OFFSET      0

// these should be accessed only with atomic memory ops
#define CRYPTO_SA_AR_CNTR_LOW     66
#define CRYPTO_SA_AR_CNTR_HIGH    67
#define CRYPTO_SA_AR_BMAP_LOW     68
#define CRYPTO_SA_AR_BMAP_HIGH    69
#define CRYPTO_SA_AR_LOCK_OFFSET  70
#define CRYPTO_SA_AUTH_FAIL_CNTR  73
#define CRYPTO_SA_SELECT_ERR_CNTR 74
#define CRYPTO_SA_OTHER_ERR_CNTR  75
#define CRYPTO_SA_AR_SEQLOW_CNTR  79
#define CRYPTO_SA_AR_SEQRCV_CNTR  80


/**
 *  Crypto SRAM Layout ( location in hex):
 *
 *  Core/Ctx      0     1     2     3     4     5
 *  Temp       0100  0200  0300  0400  0500  0600  // temp vars
 *  SA 0       0800  1000  1800  2000  2800  3000  // SA Data ( 256B )
 *  Vars 0     0900  1100  1900  2100  2900  3100  // Variables ( 128B )
 *  ICV/HMac 0 0980  1180  1980  2180  2980  3180  // ICV/HMAC ( 128B )
 *  SA 1       0A00  1200  1A00  2200  2A00  3200  // SA Data ( 256B )
 *  Vars 1     0B00  1300  1B00  2300  2B00  3300  // Variables ( 128B )
 *  ICV/HMac 1 0B80  1380  1B80  2380  2B80  3380  // ICV/HMAC ( 128B )
 *  SA 2       0C00  1400  1C00  2400  2C00  3400  // SA Data ( 256B )
 *  Vars 2     0D00  1500  1D00  2500  2D00  3500  // Variables ( 128B )
 *  ICV/HMac 2 0D80  1580  1D80  2580  2D80  3580  // ICV/HMAC ( 128B )
 *  SA 3       0E00  1600  1E00  2600  2E00  3600  // SA Data ( 256B )
 *  Vars 3     0F00  1700  1F00  2700  2F00  3700  // Variables ( 128B )
 *  ICV/HMac 3 0F80  1780  1F80  2780  2F80  3780  // ICV/HMAC ( 128B )
 *  Pkt Buff 0 4000  6000  8000  A000  C000  E000  // packet data
 *  Pkt Buff 1 4800  6800  8800  A800  C800  E800  // packet data
 *  Pkt Buff 2 5000  7000  9000  B000  D000  F000  // packet data
 *  Pkt Buff 3 5800  7800  9800  B800  D800  F800  // packet data
 */

// Sram Constants

#define CRYPTO_SA_LEN             0x100 // reserved area in sram
#define CRYPTO_SA_DMA_KEYS_LEN    0x060 // cipher/hash key dma len (32+64 bytes)
#define CRYPTO_SA_DMA_CKEY_LEN    0x020 // cipher key dma len (32) bytes)
#define CRYPTO_SA_DMA_KEYS_OFFSET 0x020 // offset to keys from start of sram sa
#define CRYPTO_SA_DMA_SALT_OFFSET 0x020 // offset from keys to gcm salt
#define CRYPTO_SA_DMA_ZROS_OFFSET 0x030 // offset from keys to gcm zeros
#define CRYPTO_SA_DMA_LENS_OFFSET 0x040 // offset from keys to gcm len fields
#define CRYPTO_SA_DMA_AUTH_OFFSET 0x060 // offset from keys to gcm auth data
#define CRYPTO_SA_DMA_ECTR_OFFSET 0x080 // offset from sa area to 'fake' gcm constr iv
#define CRYPTO_SA_START           0x800 // start of sa area for core/ctx 0
#define CRYPTO_SA_BLOCK_SIZE      0x200 // length between start of sa's
#define CRYPTO_VARS_SEQ_UA_OFFSET 0x020 // vars area offset to seq 63:32 addr
#define CRYPTO_TEMP_START         0x100 // start of temp area for core/ctx 0
#define CRYPTO_TEMP_LEN           0x100 // length of temp area
#define CRYPTO_NUM_BUFS               4 // number of buffers / core used (1-4)
#ifndef CRYPTO_MAX_SRAM_BUF_BYTES       // (overide max bytes for testing )
#define CRYPTO_MAX_SRAM_BUF_BYTES  2016 // maximum bytes to use in sram pkt buf
#endif                                  // needs to be an 8 byte multiple
                                        // 2048-32 byte alignment buffer

#define crypto_threads_get_temp_addr_const(_crypto_ctx) (CRYPTO_TEMP_START + \
                                                 (_crypto_ctx * CRYPTO_TEMP_LEN))
#define crypto_threads_get_SA_addr_const(_crypto_ctx)   (CRYPTO_SA_START   + \
                                        (_crypto_ctx * (4*CRYPTO_SA_BLOCK_SIZE)))

/**
 *  Local Memory Use ( hex addr, 64 lwords per buffer allocated ):
 *
 *  ME Threads  0/1   2/3   4/5   6/7
 *  Buffer 0   0000  0400  0800  0C00  // buffer set 0
 *  Buffer 1   0100  0500  0900  0D00  // buffer set 1
 *  Buffer 2   0200  0600  0A00  0E00  // buffer set 2
 *  Buffer 3   0300  0700  0B00  0F00  // buffer set 3
 *
 *  Local Memory format:
 *
 *  0x0000: Request Ring Entry Lword 0   ( Addr bits 39:32, stat rtrn que # )
 *  0x0004: Request Ring Entry Lword 1   ( Address bits 39:32, see above )
 *  0x0008: Request Ring Entry Lword 2   ( SA Addr )
 *  0x000C: Request Ring Entry Lword 3   ( Start of Packet Addr )
 *  0x0010: Request Ring Entry Lword 4   ( Continuation of Packet Addr )
 *  0x0014: Request Ring Entry Lword 5   ( End of Packet Addr )
 *  0x0018: Request Ring Entry Lword 6   ( Write Back Packet Addr )
 *  0x001C: Request Ring Entry Lword 7   ( Start/Continue Packet Length )
 *  0x0020: Request Ring Entry Lword 8   ( End/Write Back Packet Length )
 *  0x0024: Request Ring Entry Lword 9   ( Write Back End Length/Text Length )
 *  0x0028: Request Ring Entry Lword 10  ( Auth Hdr Offset/ IV Offset )
 *  0x002C: Request Ring Entry Lword 11  ( Text Offset / Auth Data Offset )
 *  0x0030: Request Ring Entry Lword 12  ( Security Association Ctl Wrd )
 *  0x0034: Request Ring Entry Lword 13  ( User Control Field )
 *  0x0038: Request Ring Entry Lword 14  ( Sequence Number 63:32 )
 *  0x003C: Unused                       ( not used )
 *  0x0040: Jumbo State                  ( jumbo processing state )
 *  0x0044: Jumbo Pkt Plaintext Addr     ( jumbo 'plaintext' addr in sram )
 *  0x0048: Jumbo Pkt Auth Data Addr     ( jumbo pkt auth data addr in sram )
 *  0x004C: Jumbo Pkt End Addr           ( jumbo pkt end addr in memory )
 *  0x0050: Jumbo WriteBack Remain Addr  ( jumbo writeback remaining addr )
 *  0x0054: Jumbo WriteBack Remain Len   ( jumbo writeback remaining length )
 *  0x0058: Pkt Sequence Num 31:0        ( sequence # from auth hdr low )
 *  0x005C: Pkt Sequence Num 63:32       ( sequence # from auth hdr high )
 *
 *  Common Local Memory Locations used across all 4 buffers,
 *  offsets are from start of Buffer 0 ( i.e. 0x000, 0x400, 0x800, 0xC00 )
 *
 *  Following are used in the input thread loop while processing a jumbo
 *  packet; mainly because ran out of GPRs to do this. The equivalent var
 *  in the buffer specific local mem area is for output thread use
 *  0x00C0: Jumbo State                  ( jumbo processing state )
 *  0x00C4: Jumbo Pkt End Offset         ( jumbo working end of pkt offset )
 *  0x00C8: Jumbo Pkt Plaintext Offset   ( jumbo 'plaintext' working offset )
 *  0x00CC: Jumbo Pkt Wrtback End Offset ( jumbo working wrtback end offset )
 *  0x00D0: Jumbo Pkt Hash Key Addr      ( jumbo addr of hash key in use )
 *  0x00D4: Jumbo WriteBack Remain Addr  ( jumbo writeback remaining addr )
 *  0x00D8: Jumbo WriteBack Remain Len   ( jumbo writeback remaining length )
 *  0x00DC: Jumbo First Remain Len       ( jumbo 1st buf dma in remain length )
 *  0x00E0: Jumbo First Remain Addr Hi   ( jumbo 1st buf dma in remain hi addr)
 *  0x00E4: Jumbo First Remain Addr Lo   ( jumbo 1st buf dma in remain lo addr)
 *
 *  Following used to retain value in case packet runs across multiple
 *  buffers ( i.e. jumbo pkt )
 *  0x00E8: Packet Next Hdr Value        ( next header field from packet )
 */
#define CRYPTO_LM_SA_ADDR                   0x08
#define CRYPTO_LM_PKT_STRT_ADDR             0x0C
#define CRYPTO_LM_PKT_CONT_ADDR             0x10
#define CRYPTO_LM_PKT_CONT_ADDR_UPPER       0x04
#define CRYPTO_LM_PKT_CONT_ADDR_UPPER_SHIFT 8
#define CRYPTO_LM_PKT_END_ADDR              0x14
#define CRYPTO_LM_PKT_END_ADDR_UPPER        0x04
#define CRYPTO_LM_PKT_END_ADDR_UPPER_SHIFT  0
#define CRYPTO_LM_PKT_WRBK_ADDR             0x18
#define CRYPTO_LM_PKT_WRBK_ADDR_UPPER       0x00
#define CRYPTO_LM_PKT_WRBK_ADDR_UPPER_SHIFT 16
#define CRYPTO_LM_PKT_STRT_LENGTH           0x1C
#define CRYPTO_LM_PKT_STRT_LENGTH_SHIFT     16
#define CRYPTO_LM_PKT_CONT_LENGTH           0x1C
#define CRYPTO_LM_PKT_CONT_LENGTH_SHIFT     0
#define CRYPTO_LM_PKT_END_LENGTH            0x20
#define CRYPTO_LM_PKT_END_LENGTH_SHIFT      16
#define CRYPTO_LM_PKT_WRBK_LENGTH           0x20
#define CRYPTO_LM_PKT_WRBK_LENGTH_SHIFT     0
#define CRYPTO_LM_PKT_WRBK_END_LENGTH       0x24
#define CRYPTO_LM_PKT_WRBK_END_LENGTH_SHIFT 16
#define CRYPTO_LM_PKT_TEXT_LENGTH           0x24
#define CRYPTO_LM_PKT_TEXT_LENGTH_SHIFT     0
#define CRYPTO_LM_AUTH_HDR_OFFSET           0x28
#define CRYPTO_LM_AUTH_HDR_OFFSET_SHIFT     16
#define CRYPTO_LM_IV_OFFSET                 0x28
#define CRYPTO_LM_IV_OFFSET_SHIFT           0
#define CRYPTO_LM_PTEXT_OFFSET              0x2C
#define CRYPTO_LM_PTEXT_OFFSET_SHIFT        16
#define CRYPTO_LM_AUTH_DATA_OFFSET          0x2C
#define CRYPTO_LM_AUTH_DATA_OFFSET_SHIFT    0
#define CRYPTO_LM_SA_CTLWRD                 0x30
#define CRYPTO_LM_USER_CTL_FIELD            0x34
#define CRYPTO_LM_SEQ_UPPER                 0x38
#define CRYPTO_LM_UNUSED                    0x3C
#define CRYPTO_LM_STATUS_QUE_SHIFT          0
#define CRYPTO_LM_STATUS_QUE_MASK           0x3ff
#define CRYPTO_LM_JUMBO_STATE               0x40
#define CRYPTO_LM_JUMBO_BUF_SRAM_PTEXT      0x44
#define CRYPTO_LM_JUMBO_BUF_SRAM_AUTH_DATA  0x48
#define CRYPTO_LM_JUMBO_BUF_END_ADDR        0x4C
#define CRYPTO_LM_JUMBO_WRBK_REM_ADDR       0x50
#define CRYPTO_LM_JUMBO_WRBK_REM_LEN        0x54
#define CRYPTO_LM_PKT_SEQ_NUM_LOW           0x58
#define CRYPTO_LM_PKT_SEQ_NUM_HIGH          0x5C
#define CRYPTO_LM_COMM_JUMBO_STATE          0xC0
#define CRYPTO_LM_COMM_JUMBO_PKT_END_OFF    0xC4
#define CRYPTO_LM_COMM_JUMBO_PKT_PTEXT_OFF  0xC8
#define CRYPTO_LM_COMM_JUMBO_PKT_WRBK_END_OFF 0xCC
#define CRYPTO_LM_COMM_JUMBO_HKEY_ADDR      0xD0
#define CRYPTO_LM_COMM_JUMBO_WRBK_REM_ADDR  0xD4
#define CRYPTO_LM_COMM_JUMBO_WRBK_REM_LEN   0xD8
#define CRYPTO_LM_COMM_JUMBO_FIRST_REM_LEN  0xDC
#define CRYPTO_LM_COMM_JUMBO_FIRST_REM_ADDR_HI 0xE0
#define CRYPTO_LM_COMM_JUMBO_FIRST_REM_ADDR 0xE4
#define CRYPTO_LM_COMM_NXT_HDR_VAL          0xE8

// jumbo packet 'state' values
#define CRYPTO_LM_JUMBO_STATE_NON_JUMBO     0x00
#define CRYPTO_LM_JUMBO_STATE_FIRST_BUF     0x01
#define CRYPTO_LM_JUMBO_STATE_CONTINUE_BUF  0x02
#define CRYPTO_LM_JUMBO_STATE_END_BUF       0x03


// counter offsets are in crypto_lib_threads.h

#if (OVERRIDE_UPDATE_COUNTER == 0 )

#macro crypto_threads_clear_counters()
.begin
    .sig s
    .reg r_cnt, r_xfr_cnt, r_addr, r_addr_inc
    .reg $xfr[4]
    .xfer_order $xfr

    move($xfr[0], 0)
    move($xfr[1], 0)
    move($xfr[2], 0)
    move($xfr[3], 0)

    #if ( CRYPTO_COUNTER_LENGTH == 64 )
    #define_eval _LWORD_COUNT ( CRYPTO_CNTR_LAST << 1 )
    #else
    #define_eval _LWORD_COUNT ( CRYPTO_CNTR_LAST << 0 )
    #endif
    move(r_cnt, _LWORD_COUNT)
    #undef _LWORD_COUNT
    move(r_xfr_cnt, 4)
    move(r_addr, ces_mem_cntrs)

    .repeat

        limit_min(r_xfr_cnt, r_xfr_cnt, r_cnt)
        #if ( COUNTER_MEMORY_TYPE == COUNTER_MEMORY_TYPE_CLS )
            alu[--, 0x80, OR, r_xfr_cnt, <<8]
            cls[write, $xfr[0], r_addr, 0, MAX_4], indirect_ref, ctx_swap[s]
        #else
            alu[--, 0x80, OR, r_xfr_cnt, <<8]
            mem[write32, $xfr[0], 0, <<8, r_addr, MAX_4], \
                                    indirect_ref, ctx_swap[s]
        #endif
        alu[r_addr_inc, --, B, r_xfr_cnt, <<2]
        alu[r_addr, r_addr, +, r_addr_inc]
        alu[r_cnt, r_cnt, -, r_xfr_cnt]

    .until ( r_cnt <= 0 )

.end
#endm

#macro crypto_threads_update_counter(_cntr_num)
    #if ( is_ct_const(_cntr_num) && (_cntr_num < CRYPTO_CNTR_DEBUG_LAST) && \
                                               (ENABLE_DEBUG_COUNTERS == 0))
       // don't build in debug counters
    #elif ( is_ct_const(_cntr_num) )
        .begin
            #if ( CRYPTO_COUNTER_LENGTH == 64 )
                #define_eval _CNTR_OFFSET (_cntr_num << 3)
                #define_eval _INCR_CMD incr64
            #else
                #define_eval _CNTR_OFFSET (_cntr_num << 2)
                #define_eval _INCR_CMD incr
            #endif

            .reg r_ca
            #if ( COUNTER_MEMORY_TYPE == COUNTER_MEMORY_TYPE_CLS )
                immed[r_ca, (ces_mem_cntrs + _CNTR_OFFSET)] // ok, cls only 64K
                cls[_INCR_CMD, --, r_ca, 0]
            #else
                move(r_ca, (ces_mem_cntrs + _CNTR_OFFSET))
                mem[_INCR_CND, --, 0, <<8, r_ca]
            #endif
            #undef _CNTR_OFFSET
            #undef _INCR_CMD
        .end
    #else
        .begin
            #if ( CRYPTO_COUNTER_LENGTH == 64 )
                #define_eval _CNTR_OFFSET_SHIFT 3
                #define_eval _INCR_CMD incr64
            #else
                #define_eval _CNTR_OFFSET_SHIFT 2
                #define_eval _INCR_CMD incr
            #endif

            .reg r_ca
             move(r_ca, ces_mem_cntrs)
             alu[r_ca, r_ca, OR, _cntr_num, <<_CNTR_OFFSET_SHIFT]

            #if ( COUNTER_MEMORY_TYPE == COUNTER_MEMORY_TYPE_CLS )
                cls[_INCR_CMD, --, r_ca, 0]
            #else
                mem[_INCR_CND, --, 0, <<8, r_ca]
            #endif

            #undef _CNTR_OFFSET_SHIFT
            #undef _INCR_CMD
        .end
    #endif
#endm

#endif // OVERRIDE_UPDATE_COUNTER


/**
 *  Update error counters in SA ( security association )
 *
 *  @param  _sa_addr_upper GPR,CONST bits 39:32 of sa base address
 *                         in bits 7:0
 *  @param  _sa_addr       GPR bits 31:0 of sa base address
 *                         ( bits 8:0 are masked off to 0 )
 *  @param  _cntr_num      GPR, CONST counter lword offset in sa
 *
 *  assumes sa_addr is on an sa length alignment ( currently 32 lwords )
 */
#macro crypto_threads_update_sa_counter(_sa_addr_upper, _sa_addr, _cntr_num)
    .begin
        .reg r_tmpa, r_tmpb

        move(r_tmpa, CRYPTO_REQ_SA_VERSION_MASK) // mask off version bits
        alu[r_tmpb, _sa_addr, AND~, r_tmpa]
        alu[r_tmpb, r_tmpb, OR, _cntr_num, <<2] // offset to counter
        alu[r_tmpa, --, B, _sa_addr_upper, <<24]
        mem[incr, --, r_tmpa, <<8, r_tmpb] // don't wait for completion
    .end
#endm


/**
 *  Update packet counters for specific modes,
 *  ciphers, hashes.
 *
 *  @param  _sa_ctlwrd GPR, Control Word of SA used for packet
 */
#macro crypto_threads_update_pkt_counter(_sa_ctlwrd)
.begin

    .reg r_cntr, r_val

    // increment the protocol/prot mode counter
    move(r_cntr, CRYPTO_CNTR_PKTS_ENCRYPT_ESP_TUNNEL)
    // pick protocol mode
    ld_field_w_clr[r_val, 0001, _sa_ctlwrd, >>CRYPTO_SA_PROTMODE_SHIFT]
    alu[r_val, r_val, AND, CRYPTO_SA_PROTMODE_MASK]
    .if ( r_val == CRYPTO_SA_PROTMODE_TRANSPORT )
        alu[r_cntr, r_cntr, +, 1]
    .endif
    // pick encrypt vs decrypt
    ld_field_w_clr[r_val, 0001, _sa_ctlwrd, >>CRYPTO_SA_DIR_SHIFT]
    alu[r_val, r_val, AND, CRYPTO_SA_DIR_MASK]
    .if ( r_val == CRYPTO_SA_DIR_DECRYPT )
        alu[r_cntr, r_cntr, +, 4]
    .endif
    // update the counter
    crypto_threads_update_counter(r_cntr)


    // increment the cipher counter
    // pick cipher
    ld_field_w_clr[r_val, 0001, _sa_ctlwrd, >>CRYPTO_SA_CIPHER_SHIFT]
    alu[r_val, r_val, AND, CRYPTO_SA_CIPHER_MASK]

    jump[r_val, ctupcjt#], targets [j0#, j1#, j2#, j3#, j4#, j5#, j6#, j7#, j8#]

    ctupcjt#:
        j0#: br[ctupcjt0#]
        j1#: br[ctupcjt1#]
        j2#: br[ctupcjt2#]
        j3#: br[ctupcjt3#]
        j4#: br[ctupcjt4#]
        j5#: br[ctupcjt5#]
        j6#: br[ctupcjt6#]
        j7#: br[ctupcjt7#]
        j8#: br[ctupcjt8#]

    ctupcjt0#:
        move(r_cntr, CRYPTO_CNTR_PKTS_ENCRYPT_NULL)
        br[ctupcjte#]
    ctupcjt1#:
        move(r_cntr, CRYPTO_CNTR_PKTS_ENCRYPT_3DES)
        br[ctupcjte#]
    ctupcjt2#:
        move(r_cntr, CRYPTO_CNTR_PKTS_ENCRYPT_AES128)
        br[ctupcjte#]
    ctupcjt3#:
        move(r_cntr, CRYPTO_CNTR_PKTS_ENCRYPT_AES192)
        br[ctupcjte#]
    ctupcjt4#:
        move(r_cntr, CRYPTO_CNTR_PKTS_ENCRYPT_AES256)
        br[ctupcjte#]
    ctupcjt5#:
        move(r_cntr, CRYPTO_CNTR_PKTS_ENCRYPT_AES128_NULL)
        br[ctupcjte#]
    ctupcjt6#:
        move(r_cntr, CRYPTO_CNTR_PKTS_ENCRYPT_AES192_NULL)
        br[ctupcjte#]
    ctupcjt7#:
        move(r_cntr, CRYPTO_CNTR_PKTS_ENCRYPT_AES256_NULL)
        br[ctupcjte#]
    ctupcjt8#:
        move(r_cntr, CRYPTO_CNTR_PKTS_ENCRYPT_CHACHA20)
        br[ctupcjte#]

    ctupcjte#:

    // pick encrypt vs decrypt
    ld_field_w_clr[r_val, 0001, _sa_ctlwrd, >>CRYPTO_SA_DIR_SHIFT]
    alu[r_val, r_val, AND, CRYPTO_SA_DIR_MASK]
    .if ( r_val == CRYPTO_SA_DIR_DECRYPT )
        alu[r_cntr, r_cntr, +, 22]
    .endif
    // update the counter
    crypto_threads_update_counter(r_cntr)


    // increment the hash counter
    // pick hash
    ld_field_w_clr[r_val, 0001, _sa_ctlwrd, >>CRYPTO_SA_HASH_SHIFT]
    alu[r_val, r_val, AND, CRYPTO_SA_HASH_MASK]

    jump[r_val, ctupchjt#], targets [jh0#, jh1#, jh2#, jh3#, jh4#, jh5#, \
                                     jh6#, jh7#, jh8#, jh9#, jh10#, jh11#, jh12#]

    ctupchjt#:
        jh0#: br[ctupchjt0#]
        jh1#: br[ctupchjt1#]
        jh2#: br[ctupchjt2#]
        jh3#: br[ctupchjt3#]
        jh4#: br[ctupchjt4#]
        jh5#: br[ctupchjt5#]
        jh6#: br[ctupchjt6#]
        jh7#: br[ctupchjt7#]
        jh8#: br[ctupchjt8#]
        jh9#: br[ctupchjt9#]
        jh10#: br[ctupchjt10#]
        jh11#: br[ctupchjt11#]
        jh12#: br[ctupchjt12#]

    ctupchjt0#:
        move(r_cntr, CRYPTO_CNTR_PKTS_ENCRYPT_HASH_NONE)
        br[ctupchjte#]
    ctupchjt1#:
        move(r_cntr, CRYPTO_CNTR_PKTS_ENCRYPT_MD5_96)
        br[ctupchjte#]
    ctupchjt2#:
        move(r_cntr, CRYPTO_CNTR_PKTS_ENCRYPT_SHA1_96)
        br[ctupchjte#]
    ctupchjt3#:
        move(r_cntr, CRYPTO_CNTR_PKTS_ENCRYPT_SHA256_96)
        br[ctupchjte#]
    ctupchjt4#:
        move(r_cntr, CRYPTO_CNTR_PKTS_ENCRYPT_SHA384_96)
        br[ctupchjte#]
    ctupchjt5#:
        move(r_cntr, CRYPTO_CNTR_PKTS_ENCRYPT_SHA512_96)
        br[ctupchjte#]
    ctupchjt6#:
        move(r_cntr, CRYPTO_CNTR_PKTS_ENCRYPT_MD5_128)
        br[ctupchjte#]
    ctupchjt7#:
        move(r_cntr, CRYPTO_CNTR_PKTS_ENCRYPT_SHA1_80)
        br[ctupchjte#]
    ctupchjt8#:
        move(r_cntr, CRYPTO_CNTR_PKTS_ENCRYPT_SHA256_128)
        br[ctupchjte#]
    ctupchjt9#:
        move(r_cntr, CRYPTO_CNTR_PKTS_ENCRYPT_SHA384_192)
        br[ctupchjte#]
    ctupchjt10#:
        move(r_cntr, CRYPTO_CNTR_PKTS_ENCRYPT_SHA512_256)
        br[ctupchjte#]
    ctupchjt11#:
        move(r_cntr, CRYPTO_CNTR_PKTS_ENCRYPT_GF128_128)
        br[ctupchjte#]
    ctupchjt12#:
        move(r_cntr, CRYPTO_CNTR_PKTS_ENCRYPT_POLY1305_128)
        br[ctupchjte#]

    ctupchjte#:

    // pick encrypt vs decrypt
    ld_field_w_clr[r_val, 0001, _sa_ctlwrd, >>CRYPTO_SA_DIR_SHIFT]
    alu[r_val, r_val, AND, CRYPTO_SA_DIR_MASK]
    .if ( r_val == CRYPTO_SA_DIR_DECRYPT )
        alu[r_cntr, r_cntr, +, 22]
    .endif
    // update the counter
    crypto_threads_update_counter(r_cntr)

.end
#endm

/**
 * Clear out crypto sram. Need this in order to
 * be able to dump sram for debug, because sram
 * has to be written at least once after power
 * up, or read access to an unwritten location
 * will hang
 */
#macro crypto_threads_clear_sram()
.begin
    .reg $xw[8]
    .xfer_order $xw
    .reg r_sram_addr
    .sig z

    move($xw[0], 0)
    move($xw[1], 0)
    move($xw[2], 0)
    move($xw[3], 0)
    move($xw[4], 0)
    move($xw[5], 0)
    move($xw[6], 0)
    move($xw[7], 0)

    move(r_sram_addr,0)

    .repeat
        crypto[write, $xw[0], 0, r_sram_addr, 4], ctx_swap[z]
        alu[r_sram_addr, r_sram_addr, +, 32]
    .until ( r_sram_addr > 0xffff )
.end
#endm


/**
 *  Dump debug vars to cls location 0x300
 *
 *  @param  _p1   1st reg param to dump
 *  @param  _p2   2nd reg param to dump
 *  @param  _p3   3rd reg param to dump
 *  @param  _p4   4th reg param to dump
 */
#macro cls_debug_dump4(_p1, _p2, _p3, _p4)
.begin
    .reg $x[4]
    .xfer_order $x
    .reg t
    .sig z
    move(t,0x300)
    move($x[0], _p1)
    move($x[1], _p2)
    move($x[2], _p3)
    move($x[3], _p4)
    cls[write, $x[0], 0, t, 4], ctx_swap[z]
.end
#endm

#macro cls_debug_dump2(_p1, _p2)
.begin
    .reg $x[2]
    .xfer_order $x
    .reg t
    .sig z
    move(t,0x300)
    move($x[0], _p1)
    move($x[1], _p2)
    cls[write, $x[0], 0, t, 2], ctx_swap[z]
.end
#endm

#macro cls_debug_dump1(_addr, _p1)
.begin
    .reg $x
    .reg t
    .sig z
    move(t, _addr)
    move($x, _p1)
    cls[write, $x, 0, t, 1], ctx_swap[z]
.end
#endm

/**
 *  For debug, copy 4 lwords from crypto sram to cls
 */
#macro cls_debug_copy_sram(_sram_addr, _cls_addr)
.begin
    .reg $xr[4]
    .xfer_order $xr
    .reg $xw[4]
    .xfer_order $xw
    .reg t
    .sig z

    crypto[read, $xr[0], 0, _sram_addr, 2], ctx_swap[z]

    move($xw[0], $xr[0])
    move($xw[1], $xr[1])
    move($xw[2], $xr[2])
    move($xw[3], $xr[3])
    move(t,_cls_addr)
    cls[write, $xw[0], 0, t, 4], ctx_swap[z]
.end
#endm



/**
 *  Get Sram Temp address, based on sram layout shown above
 *
 *  @param  _temp_addr  GPR, returned sram address of start of temp area
 *  @param  _crypto_ctx GPR, crypto context ( 0 through 5 )
 */
#macro crypto_threads_get_temp_addr(_temp_addr, _crypto_ctx)
    alu[_temp_addr, _crypto_ctx, +, 1]
    alu[_temp_addr, --, B, _temp_addr, <<8]
#endm


/**
 *  Get Sram SA Buffer address, based on sram layout shown above
 *
 *  @param  _sa_addr    GPR, returned sram address of start of sa
 *  @param  _crypto_ctx GPR, crypto context ( 0 through 5 )
 *  @param  _buf_sel    GPR, buffer ( 0 through 3 )
 */
#macro crypto_threads_get_sa_addr(_sa_addr, _crypto_ctx, _buf_sel)
    alu[_sa_addr, _crypto_ctx, +, 1]
    alu[_sa_addr, --, B, _sa_addr, <<11]
    alu[_sa_addr, _sa_addr, OR, _buf_sel, <<9]
#endm


/**
 *  Get Sram Vars buffer address, based on sram layout shown above
 *
 *  @param  _vars_addr  GPR, returned sram address of start of vars
 *  @param  _crypto_ctx GPR, crypto context ( 0 through 5 )
 *  @param  _buf_sel    GPR, buffer ( 0 through 3 )
 */
#macro crypto_threads_get_vars_addr(_vars_addr, _crypto_ctx, _buf_sel)
    crypto_threads_get_sa_addr(_vars_addr, _crypto_ctx, _buf_sel)
    alu[_vars_addr, _vars_addr, OR, 1, <<8]
#endm


/**
 *  Get Sram ICV/Auth Data ( a.k.a. hash value) buffer address, based on sram
 *  layout shown above
 *
 *  @param  _hash_addr  GPR, returned sram address of start of icv/auth data/hash
 *  @param  _crypto_ctx GPR, crypto context ( 0 through 5 )
 *  @param  _buf_sel    GPR, buffer ( 0 through 3 )
 */
#macro crypto_threads_get_hash_addr(_hash_addr, _crypto_ctx, _buf_sel)
    crypto_threads_get_sa_addr(_hash_addr, _crypto_ctx, _buf_sel)
    alu[_hash_addr, _hash_addr, OR, 0x18, <<4]
#endm


/**
 *  Get Sram Authentication Key buffer address, based on sram
 *  layout shown above
 *
 *  @param  _hash_key_addr GPR, returned sram address of icv/auth data/hash
 *  @param  _crypto_ctx    GPR, crypto context ( 0 through 5 )
 *  @param  _buf_sel       GPR, buffer ( 0 through 3 )
 */
#macro crypto_threads_get_hash_key_addr(_hash_key_addr, _crypto_ctx, _buf_sel)
    crypto_threads_get_sa_addr(_hash_key_addr, _crypto_ctx, _buf_sel)
    alu[_hash_key_addr, _hash_key_addr, OR, 0x40]
#endm


/**
 *  Get Sram Packet Buffer address, based on sram layout shown above
 *
 *  @param  _pkt_addr   GPR, returned sram address of start of pkt buffer
 *  @param  _crypto_ctx GPR, crypto context ( 0 through 5 )
 *  @param  _buf_sel    GPR, buffer ( 0 through 3 )
 */
#macro crypto_threads_get_pkt_addr(_pkt_addr, _crypto_ctx, _buf_sel)
    alu[_pkt_addr, --, B, _crypto_ctx, <<1]
    alu[_pkt_addr, _pkt_addr, +, 4]
    alu[_pkt_addr, --, B, _pkt_addr, <<12]
    alu[_pkt_addr, _pkt_addr, OR, _buf_sel, <<11]
#endm


/**
 *  Get Local Memory Temp address, based on local memory layout shown above
 *
 *  @param  _temp_addr  GPR, returned local memory address of start of temp area
 *  @param  _me_ctx     GPR, me context ( 0 through 7 )
 *  @param  _buf_sel    GPR, buffer ( 0 through 3 )
 */
#macro crypto_threads_get_lm_temp_addr(_temp_addr, _me_ctx, _buf_sel)
    alu[_temp_addr, 0xc, AND, _me_ctx, <<1]
    alu[_temp_addr, _temp_addr, +, _buf_sel]
    alu[_temp_addr, --, B, _temp_addr, <<8]
#endm


/**
 *  Save Next Hdr value from sram to Local Memory 'Common' location
 *
 *  @param  _me_ctx     GPR, me context ( 0 through 7 )
 *  @param  _sram_addr  GPR, byte address in sram with next hdr
 */
#macro crypto_threads_save_nxt_hdr( _me_ctx, _sram_addr )
.begin

    .sig snhs
    .reg r_temp, r_temp1
    .reg $xd[2]
    .xfer_order $xd

    // read 8 byte block containing nxt hdr field
    alu[r_temp, _sram_addr, AND~, 7]
    crypto[read, $xd[0], 0, r_temp, 1], ctx_swap[snhs]

    // extract the next hdr byte
    alu[r_temp, _sram_addr, AND, 7]
    jump[r_temp, enhjt#], targets [enhj0#, enhj1#, enhj2#, enhj3#, \
                                     enhj4#, enhj5#, enhj6#, enhj7# ]
    enhjt#:
    enhj0#: br[enhjt0#]
    enhj1#: br[enhjt1#]
    enhj2#: br[enhjt2#]
    enhj3#: br[enhjt3#]
    enhj4#: br[enhjt4#]
    enhj5#: br[enhjt5#]
    enhj6#: br[enhjt6#]
    enhj7#: br[enhjt7#]

    enhjt0#:
        ld_field_w_clr[r_temp, 0001, $xd[0], >>24]
        br[enh_done#]
    enhjt1#:
        ld_field_w_clr[r_temp, 0001, $xd[0], >>16]
        br[enh_done#]
    enhjt2#:
        ld_field_w_clr[r_temp, 0001, $xd[0], >>8 ]
        br[enh_done#]
    enhjt3#:
        ld_field_w_clr[r_temp, 0001, $xd[0], >>0 ]
        br[enh_done#]
    enhjt4#:
        ld_field_w_clr[r_temp, 0001, $xd[1], >>24]
        br[enh_done#]
    enhjt5#:
        ld_field_w_clr[r_temp, 0001, $xd[1], >>16]
        br[enh_done#]
    enhjt6#:
        ld_field_w_clr[r_temp, 0001, $xd[1], >>8 ]
        br[enh_done#]
    enhjt7#:
        ld_field_w_clr[r_temp, 0001, $xd[1], >>0 ]

    enh_done#:
        crypto_threads_get_lm_temp_addr(r_temp1, _me_ctx, 0)
        localmem_write1(r_temp, \
                        r_temp1, \
                        CRYPTO_LM_COMM_NXT_HDR_VAL)
.end
#endm


/**
 *  Compare memory ranges in crypto sram
 *
 *  @param  _res     GPR, returns 1 if mismatch, 0 if match
 *  @param  _addr1   GPR, start of memory area 1 ( byte address, may be 1 byte
 *                        aligned )
 *  @param  _addr2   GPR, start of memory area 2 ( byte address, must be 8 byte
                          aligned )
 *  @param  _len     GPR, byte length to compare
 *
 *  Crypto read only operates on 8 byte aligned address, 8 byte length transfer.
 *  So need to assemble aligned longwords at _addr1 for compare to aligned
 *  data at _addr2
 */
#macro crypto_threads_compare(_res, _addr1, _addr2, _len)
.begin
    .sig ctcs
    .reg r_cnt, r_addra, r_da[2], r_db[2], r_msk[2], r_temp
    .reg $xfra[4]
    .xfer_order $xfra
    .reg $xfrb[16]
    .xfer_order $xfrb

    // read entire _addr2 ( aligned ) sram compare region
    // into transfer registers in one cmd
    .if ( _len & 0x7 )
        alu[r_temp, _len, +, 8] // round up # 8 byte xfrs
    .else
        move(r_temp, _len)
    .endif
    alu[r_temp, --, B, r_temp, >>3] // 8 bytes per transfer
    alu[r_temp, r_temp, -, 1] // 0 based for indirect
    alu[--, 0x80, OR, r_temp, <<8]
    crypto[read, $xfrb[0], 0, _addr2, MAX_8], indirect_ref, ctx_swap[ctcs]

    move(r_cnt, _len)
    move(r_addra, _addr1)

    .repeat

    // get next 16 bytes from addr a into 4 GPR's
    alu[r_tmp, r_addra, AND~,  0x7]
    crypto[read, $xfra[0], 0, r_tmp, 2], ctx_swap[ctcs]


    // align into GPR's, based on starting address byte
    alu[r_tmp, r_addra, AND, 7]
    jump[r_tmp, ctcjt#], targets [j0#, j1#, j2#, j3#, j4#, j5#, j6#, j7#]

    ctcjt#:
        j0#: br[ctcjt0#]
        j1#: br[ctcjt1#]
        j2#: br[ctcjt2#]
        j3#: br[ctcjt3#]
        j4#: br[ctcjt4#]
        j5#: br[ctcjt5#]
        j6#: br[ctcjt6#]
        j7#: br[ctcjt7#]

    ctcjt0#:
        move(r_da[0], $xfra[0])
        move(r_da[1], $xfra[1])
        br[ctc_rdb#]

    ctcjt1#:
        alu[r_da[0], --, B, $xfra[0], <<8]
        ld_field[r_da[0], 0001, $xfra[1], >>24]
        alu[r_da[1], --, B, $xfra[1], <<8]
        ld_field[r_da[1], 0001, $xfra[2], >>24]
        br[ctc_rdb#]

    ctcjt2#:
        alu[r_da[0], --, B, $xfra[0], <<16]
        ld_field[r_da[0], 0011, $xfra[1], >>16]
        alu[r_da[1], --, B, $xfra[1], <<16]
        ld_field[r_da[1], 0011, $xfra[2], >>16]
        br[ctc_rdb#]

    ctcjt3#:
        alu[r_da[0], --, B, $xfra[0], <<24]
        ld_field[r_da[0], 0111, $xfra[1], >>8]
        alu[r_da[1], --, B, $xfra[1], <<24]
        ld_field[r_da[1], 0111, $xfra[2], >>8]
        br[ctc_rdb#]

    ctcjt4#:
        alu[r_da[0], --, B, $xfra[1]]
        alu[r_da[1], --, B, $xfra[2]]
        br[ctc_rdb#]

    ctcjt5#:
        alu[r_da[0], --, B, $xfra[1], <<8]
        ld_field[r_da[0], 0001, $xfra[2], >>24]
        alu[r_da[1], --, B, $xfra[2], <<8]
        ld_field[r_da[1], 0001, $xfra[3], >>24]
        br[ctc_rdb#]

    ctcjt6#:
        alu[r_da[0], --, B, $xfra[1], <<16]
        ld_field[r_da[0], 0011, $xfra[2], >>16]
        alu[r_da[1], --, B, $xfra[2], <<16]
        ld_field[r_da[1], 0011, $xfra[3], >>16]
        br[ctc_rdb#]

    ctcjt7#:
        alu[r_da[0], --, B, $xfra[1], <<24]
        ld_field[r_da[0], 0111, $xfra[2], >>8]
        alu[r_da[1], --, B, $xfra[2], <<24]
        ld_field[r_da[1], 0111, $xfra[3], >>8]
        br[ctc_rdb#]


    // move 2 lwords read for aligned region b from xfr regs
    // into gprs for compare
    ctc_rdb#:

    alu[r_temp, _len, -, r_cnt]
    alu[r_temp, --, B, r_temp, >>3]
    jump[r_temp, ctcbjt#], targets [k0#, k1#, k2#, k3#, k4#, k5#, k6#, k7#]

    ctcbjt#:
        k0#: br[ctcbjt0#]
        k1#: br[ctcbjt1#]
        k2#: br[ctcbjt2#]
        k3#: br[ctcbjt3#]
        k4#: br[ctcbjt4#]
        k5#: br[ctcbjt5#]
        k6#: br[ctcbjt6#]
        k7#: br[ctcbjt7#]

    ctcbjt0#:
        move(r_db[0], $xfrb[0])
        move(r_db[1], $xfrb[1])
        br[ctc_genm#]

    ctcbjt1#:
        move(r_db[0], $xfrb[2])
        move(r_db[1], $xfrb[3])
        br[ctc_genm#]

    ctcbjt2#:
        move(r_db[0], $xfrb[4])
        move(r_db[1], $xfrb[5])
        br[ctc_genm#]

    ctcbjt3#:
        move(r_db[0], $xfrb[6])
        move(r_db[1], $xfrb[7])
        br[ctc_genm#]

    ctcbjt4#:
        move(r_db[0], $xfrb[8])
        move(r_db[1], $xfrb[9])
        br[ctc_genm#]

    ctcbjt5#:
        move(r_db[0], $xfrb[10])
        move(r_db[1], $xfrb[11])
        br[ctc_genm#]

    ctcbjt6#:
        move(r_db[0], $xfrb[12])
        move(r_db[1], $xfrb[13])
        br[ctc_genm#]

    ctcbjt7#:
        move(r_db[0], $xfrb[14])
        move(r_db[1], $xfrb[15])
        br[ctc_genm#]


    // generate mask to mask off unused bytes based on
    // remaining length
    ctc_genm#:

    .if ( r_cnt > 7 ) // compare all 8 bytes
        move(r_msk[0], 0xffffffff)
        move(r_msk[1], 0xffffffff)
    .else
        jump[r_cnt, ctcmjt#], targets [mj0#, mj1#, mj2#, mj3#, mj4#, mj5#, \
                                                                 mj6#, mj7#]

        ctcmjt#:
            mj0#: br[ctcmjt0#]
            mj1#: br[ctcmjt1#]
            mj2#: br[ctcmjt2#]
            mj3#: br[ctcmjt3#]
            mj4#: br[ctcmjt4#]
            mj5#: br[ctcmjt5#]
            mj6#: br[ctcmjt6#]
            mj7#: br[ctcmjt7#]

        ctcmjt0#:  // this shouldn't happen
            move(r_msk[0], 0x00000000)
            move(r_msk[1], 0x00000000)
            br[ctc_cmp#]

        ctcmjt1#:
            move(r_msk[0], 0xff000000)
            move(r_msk[1], 0x00000000)
            br[ctc_cmp#]

        ctcmjt2#:
            move(r_msk[0], 0xffff0000)
            move(r_msk[1], 0x00000000)
            br[ctc_cmp#]

        ctcmjt3#:
            move(r_msk[0], 0xffffff00)
            move(r_msk[1], 0x00000000)
            br[ctc_cmp#]

        ctcmjt4#:
            move(r_msk[0], 0xffffffff)
            move(r_msk[1], 0x00000000)
            br[ctc_cmp#]

        ctcmjt5#:
            move(r_msk[0], 0xffffffff)
            move(r_msk[1], 0xff000000)
            br[ctc_cmp#]

        ctcmjt6#:
            move(r_msk[0], 0xffffffff)
            move(r_msk[1], 0xffff0000)
            br[ctc_cmp#]

        ctcmjt7#:
            move(r_msk[0], 0xffffffff)
            move(r_msk[1], 0xffffff00)
    .endif

    // compare the two lwords
    ctc_cmp#:
    alu[r_da[0], r_da[0], XOR, r_db[0]]
    alu[--, r_da[0], AND, r_msk[0]]
    bne[crypto_threads_compare_failed#]

    alu[r_da[1], r_da[1], XOR, r_db[1]]
    alu[--, r_da[1], AND, r_msk[1]]
    bne[crypto_threads_compare_failed#]


    // next address, count
    alu[r_addra, r_addra, +, 8]
    alu[r_cnt, r_cnt, -, 8]

    .until ( r_cnt <= 0)


crypto_threads_compare_ok#:
    immed[_res, 0]
    br[crypto_threads_compare_done#]


crypto_threads_compare_failed#:
    immed[_res, 1]


crypto_threads_compare_done#:

.end
#endm


/**
 *  Clear memory range in local ctm
 *
 *  @param  _addr    GPR, start of memory area
 *  @param  _len     GPR, length to clear, # lwords
 *
 *  _len has to be a multiple of 32
 */
#macro crypto_threads_clr_ctm(_addr, _len)
.begin
    .reg $xfr[32]
    .xfer_order $xfr
    .sig z
    .reg r_addr, r_cnt, r_xfr_cnt

    move(r_cnt, 0)
    move(r_addr, _addr)
    move(r_xfr_cnt, 15)
    aggregate_zero($xfr, 32)

    .repeat
        alu[--, 0x80, OR, r_xfr_cnt, <<8]
        mem[write, $xfr[0], 0, <<8, r_addr, MAX_16], \
            indirect_ref, ctx_swap[z]

        alu[r_cnt, r_cnt, +, 32]
        alu[r_addr, r_addr, +, 128]
    .until ( r_cnt >= _len )
.end
#endm


/**
 *  Clear memory range in emem
 *
 *  @param  _addr    GPR, start of memory area
 *  @param  _len     GPR, length to clear, # lwords
 *
 *  _len has to be a multiple of 32
 */
#macro crypto_threads_clr_emem(_addr, _len)
.begin
    .reg $xfr[32]
    .xfer_order $xfr
    .sig z
    .reg r_addr_hi, r_addr_low, r_cnt, r_xfr_cnt

    move(r_cnt, 0)
    move(r_xfr_cnt, 15)
    aggregate_zero($xfr, 32)
    immed40(r_addr_hi, r_addr_low, _addr)

    .repeat
        alu[--, 0x80, OR, r_xfr_cnt, <<8]
        mem[write, $xfr[0], r_addr_hi, <<8, r_addr_low, MAX_16], \
            indirect_ref, ctx_swap[z]

        alu[r_cnt, r_cnt, +, 32]
        alu[r_addr_low, r_addr_low, +, 128]
    .until ( r_cnt >= _len )
.end
#endm


/**
 *  Clear Initialization complete flag.
 *  For now, supports a flag in CLS only
 */
#macro crypto_threads_clear_init_flag()
.begin
    .reg addr
    .reg $x
    .sig z
    move(addr, crypto_init_flag)
    move($x, 0x0)
    cls[write, $x, addr, 0, 1], ctx_swap[z]
.end
#endm


/**
 *  Set Initialization complete flag.
 *  For now, supports a flag in CLS only
 */
#macro crypto_threads_set_init_flag()
.begin
    .reg addr
    .reg $x
    .sig z
    move(addr, crypto_init_flag)
    move($x, 0x05170522)
    cls[write, $x, addr, 0, 1], ctx_swap[z]
.end
#endm


/**
 *  Wait for Initialization complete flag
 *  For now, supports a flag in CLS only
 */
#macro crypto_threads_wait_init_flag()
.begin
    .reg addr_lo, addr_hi
    .reg $x
    .sig z
    #if (INIT_FLAG_USE_ALLOC_MEM == 1)
        immed40(addr_hi, addr_lo, crypto_init_flag)
    #else
        move(addr_lo, crypto_init_flag)
        #if (INIT_FLAG_ISLAND == 12)
            move(addr_hi, 0x30000000) // per bsp init
        #else
            move(addr_hi, 0x34000000)
        #endif
    #endif
    ctw_loop#:
    cls[read, $x, addr_hi, <<8, addr_lo, 1], ctx_swap[z]
    .if ($x != 0x05170522)
        br[ctw_loop#]
    .endif
.end
#endm


#if ( ENABLE_SA_FLUSH == 1 )
/**
 *  Set SA Flush flags.
 *  For now, supports a flag word in CLS only
 */
#macro crypto_threads_set_sa_flush_flags()
.begin
    .reg addr
    .reg $x
    .sig z
    move(addr, crypto_sa_flush_flags)
    move($x, 0x00ffffff) // 1 bit / sram pkt buffer
    cls[write, $x, addr, 0, 1], ctx_swap[z]
.end
#endm


/**
 *  Clear SA Flush flags.
 *  For now, supports a flag word in CLS only
 */
#macro crypto_threads_clear_sa_flush_flags()
.begin
    .reg addr
    .reg $x
    .sig z
    move(addr, crypto_sa_flush_flags)
    move($x, 0)
    cls[write, $x, addr, 0, 1], ctx_swap[z]
.end
#endm


/**
 *  Test/Clr SA Flush flag.
 *  For now, supports a flag word in CLS only
 *  Clears a single bit in the flag word and
 *  returns original value, using atomic operation
 */
#macro crypto_threads_tst_clr_sa_flush_flag(_flag, _crypto_ctx, _buf_num)
.begin
    .reg addr, bit_mask, t
    .reg $x
    .sig z

    alu[--, _buf_num, OR, 0]
    alu[bit_mask, --, B, 1, <<indirect]
    alu[t, --, B, _crypto_ctx, <<2]
    alu[ --, t, OR, 0]
    alu[bit_mask, --, B, bit_mask, <<indirect]

    move($x, bit_mask) // 1 bit / sram pkt buffer
    move(addr, crypto_sa_flush_flags)
    cls[test_clr, $x, addr, 0, 1], ctx_swap[z]
    alu[_flag, $x, AND, bit_mask]
.end
#endm
#endif // ENABLE_SA_FLUSH == 1


/**
 *  Check SA control word for valid/supported values
 *
 *  @param  _result   GPR, set to 1 on error or 0 if ok
 *  @param  _sa_ctlwrd GPR, control word of SA to be checked
 */
#macro crypto_threads_check_sa(_result, _sa_ctlwrd)
.begin

    .reg r_val

    move(_result, 0)

    // check 'protocol'
    ld_field_w_clr[r_val, 0001, _sa_ctlwrd, >>CRYPTO_SA_PROTOCOL_SHIFT]
    alu[r_val, r_val, AND, CRYPTO_SA_PROTOCOL_MASK]
    .if ( r_val > CRYPTO_SA_PROTOCOL_ESP )
        crypto_threads_update_counter(CRYPTO_CNTR_SA_PROTOCOL_SEL_ERR)
        br[ctcsa_failed#]
    .endif

    // check 'protocol mode'
    ld_field_w_clr[r_val, 0001, _sa_ctlwrd, >>CRYPTO_SA_PROTMODE_SHIFT]
    alu[r_val, r_val, AND, CRYPTO_SA_PROTMODE_MASK]
    .if ( r_val > CRYPTO_SA_PROTMODE_TUNNEL )
        crypto_threads_update_counter(CRYPTO_CNTR_SA_PROT_MODE_SEL_ERR)
        br[ctcsa_failed#]
    .endif

    // check cipher
    ld_field_w_clr[r_val, 0001, _sa_ctlwrd, >>CRYPTO_SA_CIPHER_SHIFT]
    alu[r_val, r_val, AND, CRYPTO_SA_CIPHER_MASK]
    #if (IS_NFPTYPE(__NFP3800))
        .if ( r_val == CRYPTO_SA_CIPHER_3DES )
            crypto_threads_update_counter(CRYPTO_CNTR_SA_CIPHER_SEL_ERR)
            br[ctcsa_failed#]
        .elif ( r_val > CRYPTO_SA_CIPHER_CHACHA20)
            crypto_threads_update_counter(CRYPTO_CNTR_SA_CIPHER_SEL_ERR)
            br[ctcsa_failed#]
        .endif
    #else
        .if ( r_val > CRYPTO_SA_CIPHER_AES256_NULL )
            crypto_threads_update_counter(CRYPTO_CNTR_SA_CIPHER_SEL_ERR)
            br[ctcsa_failed#]
        .endif
    #endif

    // check cipher mode
    ld_field_w_clr[r_val, 0001, _sa_ctlwrd, >>CRYPTO_SA_CIMODE_SHIFT]
    alu[r_val, r_val, AND, CRYPTO_SA_CIMODE_MASK]
    .if ( r_val < CRYPTO_SA_CIMODE_CBC )
        crypto_threads_update_counter(CRYPTO_CNTR_SA_CIPHER_MODE_SEL_ERR)
        br[ctcsa_failed#]
    .elif ( r_val > CRYPTO_SA_CIMODE_CTR )
        crypto_threads_update_counter(CRYPTO_CNTR_SA_CIPHER_MODE_SEL_ERR)
        br[ctcsa_failed#]
    .endif

    // check hash
    ld_field_w_clr[r_val, 0001, _sa_ctlwrd, >>CRYPTO_SA_HASH_SHIFT]
    alu[r_val, r_val, AND, CRYPTO_SA_HASH_MASK]
    #if (IS_NFPTYPE(__NFP3800))
        .if ( r_val > CRYPTO_SA_HASH_POLY1305_128 )
            crypto_threads_update_counter(CRYPTO_CNTR_SA_HASH_SEL_ERR)
            br[ctcsa_failed#]
        .endif
    #else
        .if ( r_val > CRYPTO_SA_HASH_GF128_128 )
            crypto_threads_update_counter(CRYPTO_CNTR_SA_HASH_SEL_ERR)
            br[ctcsa_failed#]
        .endif
    #endif

    // check ok
    br[ctcsa_done#]

    // check failed
    ctcsa_failed#:
        move(_result, 1)

    ctcsa_done#:

.end
#endm


/**
 *  Convert cipher select value from sa to one of the
 *  cipher select values for use with crypto_setup_configs
 *
 *  @param  _cipher_sel    GPR, crypto_setup_configs value returned
 *  @param  _sa_cipher_sel GPR, value from SA
 */
#macro crypto_threads_get_cipher_val(_cipher_sel, _sa_cipher_sel)
.begin

    jump [_sa_cipher_sel, ctgcvjt#], targets [j0#, j1#, j2#, j3#, j4#, \
                                              j5#, j6#, j7#, j8#]
    ctgcvjt#:
        j0#: br [ctgcv0#]
        j1#: br [ctgcv1#]
        j2#: br [ctgcv2#]
        j3#: br [ctgcv3#]
        j4#: br [ctgcv4#]
        j5#: br [ctgcv5#]
        j6#: br [ctgcv6#]
        j7#: br [ctgcv7#]
        j8#: br [ctgcv8#]

    ctgcv0#:
         move(_cipher_sel, CRYPTO_CIPHER_NONE)
         br[ctgcve#]
    ctgcv1#:
         move(_cipher_sel, CRYPTO_CIPHER_3DES)
         br[ctgcve#]
    ctgcv2#:
         move(_cipher_sel, CRYPTO_CIPHER_AES_128)
         br[ctgcve#]
    ctgcv3#:
         move(_cipher_sel, CRYPTO_CIPHER_AES_192)
         br[ctgcve#]
    ctgcv4#:
         move(_cipher_sel, CRYPTO_CIPHER_AES_256)
         br[ctgcve#]
    ctgcv5#:
         move(_cipher_sel, CRYPTO_CIPHER_AES_128)
         br[ctgcve#]
    ctgcv6#:
         move(_cipher_sel, CRYPTO_CIPHER_AES_192)
         br[ctgcve#]
    ctgcv7#:
         move(_cipher_sel, CRYPTO_CIPHER_AES_256)
         br[ctgcve#]
    ctgcv8#:
         move(_cipher_sel, CRYPTO_CIPHER_CHACHA)
         br[ctgcve#]

    ctgcve#:
.end
#endm


/**
 *  Convert cipher mode select value from sa to one of the
 *  cipher mode select value for use with crypto_setup_configs
 *
 *  @param  _cipher_mode_sel    GPR, crypto_setup_configs value returned
 *  @param  _sa_cipher_mode_sel GPR, value from SA
 */
#macro crypto_threads_get_cipher_mode_val(_cipher_mode_sel, _sa_cipher_mode_sel)
.begin
    alu[_cipher_mode_sel, --, B, _sa_cipher_mode_sel] // they are the same
.end
#endm


/**
 *  Convert hash select value from sa to one of the
 *  cipher hash select values for use with crypto_setup_configs
 *
 *  @param  _hash_sel    GPR, crypto_setup_configs value returned
 *  @param  _sa_hash_sel GPR, value from SA
 */
#macro crypto_threads_get_hash_val(_hash_sel, _sa_hash_sel)
.begin

    jump [_sa_hash_sel, ctghvjt#], targets [j0#, j1#, j2#, j3#, j4#, \
                                            j5#, j6#, j7#, j8#, j9#, \
                                            j10#, j11#, j12#]
    ctghvjt#:
        j0#: br [ctghv0#]
        j1#: br [ctghv1#]
        j2#: br [ctghv2#]
        j3#: br [ctghv3#]
        j4#: br [ctghv4#]
        j5#: br [ctghv5#]
        j6#: br [ctghv1#]
        j7#: br [ctghv2#]
        j8#: br [ctghv3#]
        j9#: br [ctghv4#]
        j10#: br [ctghv5#]
        j11#: br [ctghv6#]
        j12#: br [ctghv7#]

    ctghv0#:
         move(_hash_sel, CRYPTO_HASH_NONE)
         br[ctghve#]
    ctghv1#:
         move(_hash_sel, CRYPTO_HASH_MD5)
         br[ctghve#]
    ctghv2#:
         move(_hash_sel, CRYPTO_HASH_SHA_1)
         br[ctghve#]
    ctghv3#:
         move(_hash_sel, CRYPTO_HASH_SHA_256)
         br[ctghve#]
    ctghv4#:
         move(_hash_sel, CRYPTO_HASH_SHA_384)
         br[ctghve#]
    ctghv5#:
         move(_hash_sel, CRYPTO_HASH_SHA_512)
         br[ctghve#]
    ctghv6#:
         move(_hash_sel, CRYPTO_HASH_GF128)
         br[ctghve#]
    ctghv7#:
         move(_hash_sel, CRYPTO_HASH_POLY1305_S)
         br[ctghve#]

    ctghve#:
.end
#endm


/**
 *  Get the initialization vector (iv) length from the
 *  cipher and cipher mode values
 *
 *  @param  _iv_len              GPR, returned IV length
 *  @param  _sa_cipher_sel       GPR, value from SA
 *  @param  _sa_cipher_mode_sel  GPR, crypto_setup_configs value returned
 */
#macro crypto_threads_get_iv_len( _iv_len, _sa_cipher_sel, _sa_cipher_mode_sel )
.begin

    jump [_sa_cipher_sel, ctgivljt#], targets [j0#, j1#, j2#, j3#, j4#, \
                                              j5#, j6#, j7#, j8#]
    ctgivljt#:
        j0#: br [ctgivl0#]
        j1#: br [ctgivl1#]
        j2#: br [ctgivl2#]
        j3#: br [ctgivl2#]
        j4#: br [ctgivl2#]
        j5#: br [ctgivl2#]
        j6#: br [ctgivl2#]
        j7#: br [ctgivl2#]
        j8#: br [ctgivl1#]

    ctgivl0#:
         move(_iv_len, 0)
         br[ctgivle#]
    ctgivl1#:
         move(_iv_len, 8)
         br[ctgivle#]
    ctgivl2#:
         move(_iv_len, 16)
         br[ctgivle#]

    ctgivle#:

   .if (_sa_cipher_mode_sel == CRYPTO_SA_CIMODE_CTR)
       move(_iv_len, 8)
   .endif

.end
#endm


/**
 *  Get the integrity check value (icv) length, in the packet,
 *  from the hash select
 *
 *  @param  _icv_len     GPR, the icv length in the packet
 *  @param  _sa_hash_sel GPR, hash select value from SA
 */
#macro crypto_threads_get_icv_len(_icv_len, _sa_hash_sel)
.begin

    jump [_sa_hash_sel, ctgicvljt#], targets [j0#, j1#, j2#, j3#, j4#, \
                                              j5#, j6#, j7#, j8#, j9#, \
                                              j10#, j11#, j12#]
    ctgicvljt#:
        j0#: br [ctgicvl0#]
        j1#: br [ctgicvl1#]
        j2#: br [ctgicvl1#]
        j3#: br [ctgicvl1#]
        j4#: br [ctgicvl1#]
        j5#: br [ctgicvl1#]
        j6#: br [ctgicvl2#]
        j7#: br [ctgicvl3#]
        j8#: br [ctgicvl2#]
        j9#: br [ctgicvl4#]
        j10#: br [ctgicvl5#]
        j11#: br [ctgicvl2#]
        j12#: br [ctgicvl2#]

    ctgicvl0#:
         move(_icv_len, 0)
         br[ctgicvle#]
    ctgicvl1#:
         move(_icv_len, 12) // 96 bits, 12 bytes
         br[ctgicvle#]
    ctgicvl2#:
         move(_icv_len, 16) // 128 bits, 16 bytes
         br[ctgicvle#]
    ctgicvl3#:
         move(_icv_len, 10) // 80 bits, 10 bytes
         br[ctgicvle#]
    ctgicvl4#:
         move(_icv_len, 24) // 192 bits, 24 bytes
         br[ctgicvle#]
    ctgicvl5#:
         move(_icv_len, 32) // 256 bits, 32 bytes
         br[ctgicvle#]

    ctgicvle#:
.end
#endm


/**
 *  Get a hash block size mask value from the hash select
 *  For example, if the block size is 16 bytes, the mask
 *  value is 0x0f. The inversion of this mask can be used
 *  to mask off bits in an address or length.
 *
 *  @param  _mask        GPR, the mask based on the block size
 *  @param  _sa_hash_sel GPR, hash select value from SA
 */
#macro crypto_threads_get_hash_mask(_mask, _sa_hash_sel)
.begin

    jump [_sa_hash_sel, ctghmjt#], targets [j0#, j1#, j2#, j3#, j4#, \
                                            j5#, j6#, j7#, j8#, j9#, \
                                            j10#, j11#, j12#]
    ctghmjt#:
        j0#: br [ctghm0#]
        j1#: br [ctghm1#]
        j2#: br [ctghm1#]
        j3#: br [ctghm1#]
        j4#: br [ctghm2#]
        j5#: br [ctghm2#]
        j6#: br [ctghm1#]
        j7#: br [ctghm1#]
        j8#: br [ctghm1#]
        j9#: br [ctghm2#]
        j10#: br [ctghm2#]
        j11#: br [ctghm3#]
        j12#: br [ctghm3#]

    ctghm0#:
         move(_mask, 0) // no hash, no mask
         br[ctghme#]
    ctghm1#:
         move(_mask, 0x3f) // 512 bits, 64 bytes
         br[ctghme#]
    ctghm2#:
         move(_mask, 0x7f) // 1024 bits, 128 bytes
         br[ctghme#]
    ctghm3#:
         move(_mask, 0x0f) // 128 bits, 16 bytes
         br[ctghme#]

    ctghme#:
.end
#endm


/**
 *  Send Response to Response Ring
 *
 *  @param  _me_ctx  GPR, me context ( 0 through 7 )
 *  @param  _buf_sel GPR, buffer select 0-3
 *  @param  _status  CONST or GPR, one of CRYPTO_RSP_STATUS_*
 *
 *  Default implementation, can be overridden by user using OVERIDE_RESPONSE_RING
 *  Sends response into response ring, increments error counters in sa, plus
 *  sends start of packet address into buffer free ring if free is enabled in
 *  cmd packet.
 *
 */
#if (OVERRIDE_RESPONSE_RING == 0 )
#macro crypto_threads_send_response(_me_ctx, _buf_sel, _status)

.begin

    .sig srs
    .reg r_temp, r_temp1, r_temp2, r_lm_base, r_rsp_ring_num, r_poll_count

    .reg $rsp[CRYPTO_RSP_LENGTH]
    .xfer_order $rsp

    // setup direct index/offset access to lm vars here
    crypto_threads_get_lm_temp_addr(r_lm_base, _me_ctx, _buf_sel)
    localmem_set_address(r_lm_base, 0, LM_HANDLE_0)
    #define_eval CRYPTO_LM_PKT_WRBK_ADDR_UPPER_LW (CRYPTO_LM_PKT_WRBK_ADDR_UPPER >> 2)
    #define_eval CRYPTO_LM_PKT_END_ADDR_UPPER_LW  (CRYPTO_LM_PKT_END_ADDR_UPPER >> 2)
    #define_eval CRYPTO_LM_SA_ADDR_LW             (CRYPTO_LM_SA_ADDR >> 2)
    #define_eval CRYPTO_LM_PKT_STRT_ADDR_LW       (CRYPTO_LM_PKT_STRT_ADDR >> 2)
    #define_eval CRYPTO_LM_PKT_CONT_ADDR_LW       (CRYPTO_LM_PKT_CONT_ADDR >> 2)
    #define_eval CRYPTO_LM_PKT_END_ADDR_LW        (CRYPTO_LM_PKT_END_ADDR >> 2)
    #define_eval CRYPTO_LM_PKT_WRBK_ADDR_LW       (CRYPTO_LM_PKT_WRBK_ADDR >> 2)
    #define_eval CRYPTO_LM_PKT_STRT_LENGTH_LW     (CRYPTO_LM_PKT_STRT_LENGTH >> 2)
    #define_eval CRYPTO_LM_PKT_END_LENGTH_LW      (CRYPTO_LM_PKT_END_LENGTH >> 2)
    #define_eval CRYPTO_LM_PKT_WRBK_END_LENGTH_LW (CRYPTO_LM_PKT_WRBK_END_LENGTH >> 2)
    #define_eval CRYPTO_LM_SA_CTLWRD_LW           (CRYPTO_LM_SA_CTLWRD >> 2)
    #define_eval CRYPTO_LM_USER_CTL_LW            (CRYPTO_LM_USER_CTL_FIELD >> 2)
    #define_eval r_pkt_upper_addr1 *l$index0[CRYPTO_LM_PKT_WRBK_ADDR_UPPER_LW]
    #define_eval r_pkt_upper_addr2 *l$index0[CRYPTO_LM_PKT_END_ADDR_UPPER_LW]
    #define_eval r_pkt_strt_addr   *l$index0[CRYPTO_LM_PKT_STRT_ADDR_LW]
    #define_eval r_pkt_cont_addr   *l$index0[CRYPTO_LM_PKT_CONT_ADDR_LW]
    #define_eval r_pkt_end_addr    *l$index0[CRYPTO_LM_PKT_END_ADDR_LW]
    #define_eval r_pkt_wrbk_addr   *l$index0[CRYPTO_LM_PKT_WRBK_ADDR_LW]
    #define_eval r_pkt_lengths1    *l$index0[CRYPTO_LM_PKT_STRT_LENGTH_LW]
    #define_eval r_pkt_lengths2    *l$index0[CRYPTO_LM_PKT_END_LENGTH_LW]
    #define_eval r_pkt_lengths3    *l$index0[CRYPTO_LM_PKT_WRBK_END_LENGTH_LW]
    #define_eval r_sa_addr         *l$index0[CRYPTO_LM_SA_ADDR_LW]
    #define_eval r_sa_ctlwrd       *l$index0[CRYPTO_LM_SA_CTLWRD_LW]
    #define_eval r_user_ctl        *l$index0[CRYPTO_LM_USER_CTL_LW]


    // if response is an error code, update the sa error
    // counters
    move(r_temp2, _status)
    .if ( r_temp2 )

        ld_field_w_clr[r_temp, 0001, \
            r_pkt_upper_addr2, >>CRYPTO_REQ_SA_ADDR_UPPER_SHIFT]

        .if ( r_temp2 == CRYPTO_RSP_STATUS_AUTH_CHECK_FAIL )
            move(r_temp2, CRYPTO_SA_AUTH_FAIL_CNTR)
        .elif ( r_temp2 == CRYPTO_RSP_STATUS_SA_SELECTION_ERR )
            move(r_temp2, CRYPTO_SA_SELECT_ERR_CNTR)
        .elif ( r_temp2 == CRYPTO_RSP_STATUS_REPLAY_TOO_SMALL )
            move(r_temp2, CRYPTO_SA_AR_SEQLOW_CNTR)
        .elif ( r_temp2 == CRYPTO_RSP_STATUS_REPLAY_ALREADY_USED )
            move(r_temp2, CRYPTO_SA_AR_SEQRCV_CNTR)
        .else
            move(r_temp2, CRYPTO_SA_OTHER_ERR_CNTR)
        .endif

        crypto_threads_update_sa_counter(r_temp, r_sa_addr, r_temp2)

        // reload idx reg in case of use in other ctx
        localmem_set_address(r_lm_base, 0, LM_HANDLE_0)
    .endif


    // for decrypt option 1, ESP, tunnel mode, the write back address is
    // the pointer to the plaintext we are writing back and the actual start
    // of packet is in the continuation of packet address

    move(r_temp, r_sa_ctlwrd)
    alu[r_temp1, CRYPTO_SA_PROTOCOL_MASK, AND, r_temp, \
                                  >>CRYPTO_SA_PROTOCOL_SHIFT]
    alu[r_temp2, CRYPTO_SA_PROTMODE_MASK, AND, r_temp, \
                                  >>CRYPTO_SA_PROTMODE_SHIFT]
    .if ( (r_temp1 == CRYPTO_SA_PROTOCOL_ESP) && \
          (r_temp2 == CRYPTO_SA_PROTMODE_TUNNEL) && \
          (r_pkt_upper_addr1 & (1 << CRYPTO_REQ_DECRYPT_OPTION_BIT)))

        ld_field_w_clr[r_temp, 0110, r_pkt_upper_addr2, <<8] // upper addrs
        ld_field[r_temp, 0001, _status, >>0] // status
        alu[r_temp, r_temp, OR, 1, <<31] // must be 1
        move(r_temp1, r_sa_ctlwrd)
        alu[r_temp1, CRYPTO_SA_DIR_MASK, AND, r_temp1, \
                         >>CRYPTO_SA_DIR_SHIFT]
        alu[r_temp, r_temp, OR, r_temp1, <<30] // dir

        alu[r_temp1, --, B, r_pkt_upper_addr1, >>10]
        alu[r_temp1, r_temp1, AND, 3]
        alu[r_temp, r_temp, OR, r_temp1, <<24] //CTM buff size

        // status, etc. in rsp[0]
        alu[$rsp[0], --, B, r_temp]

        // cont addr in rsp[1]
        move($rsp[1], r_pkt_cont_addr)

        // end addr in rsp[2]
        move($rsp[2], r_pkt_end_addr)

        ld_field_w_clr[r_temp, 0011, r_pkt_lengths1, <<0]
        move(r_temp1, r_pkt_lengths3)
        ld_field[r_temp, 1100, r_temp1, <<0]

        // cont, end lengths in rsp[3]
        alu[$rsp[3], --, B, r_temp]

    .else

        ld_field_w_clr[r_temp, 0100, r_pkt_upper_addr1, >>0]
        ld_field[r_temp, 0010, r_pkt_upper_addr2, <<8] // upper addrs
        ld_field[r_temp, 0001, _status, >>0] // status
        alu[r_temp, r_temp, OR, 1, <<31] // must be 1
        move(r_temp1, r_sa_ctlwrd)
        alu[r_temp1, CRYPTO_SA_DIR_MASK, AND, r_temp1, \
                         >>CRYPTO_SA_DIR_SHIFT]
        alu[r_temp, r_temp, OR, r_temp1, <<30] // dir

        alu[r_temp1, --, B, r_pkt_upper_addr1, >>10]
        alu[r_temp1, r_temp1, AND, 3]
        alu[r_temp, r_temp, OR, r_temp1, <<24] //CTM buff size

        // status, etc. in rsp[0]
        alu[$rsp[0], --, B, r_temp]

        // writeback addr in rsp[1]
        move($rsp[1], r_pkt_wrbk_addr)

        // end addr in rsp[2]
        move($rsp[2], r_pkt_end_addr)

        move(r_temp1, r_pkt_lengths2)
        ld_field_w_clr[r_temp, 0011, r_temp1, <<0]
        move(r_temp1, r_pkt_lengths3)
        ld_field[r_temp, 1100, r_temp1, <<0]

        // writeback, end lengths in rsp[3]
        alu[$rsp[3], --, B, r_temp]

    .endif

    // sa addr in rsp[4]
    move($rsp[4], r_sa_addr)

    // for decrypt, put the next hdr field into rsp[5]
    move(r_temp, 0)
    move(r_temp1, r_sa_ctlwrd)
    alu[r_temp1, CRYPTO_SA_DIR_MASK, AND, r_temp1,
                                 >>CRYPTO_SA_DIR_SHIFT]
    .if ( r_temp1 == CRYPTO_SA_DIR_DECRYPT )

        crypto_threads_get_lm_temp_addr(r_temp1, _me_ctx, 0)
        localmem_read1(r_temp, \
                       r_temp1, \
                       CRYPTO_LM_COMM_NXT_HDR_VAL)

        // restore value in lm idx reg
        localmem_set_address(r_lm_base, 0, LM_HANDLE_0)
        nop
        nop
        nop
    .endif

    // user control field value in rsp[5]
    move(r_temp1, r_user_ctl)
    ld_field[r_temp, 0110, r_temp1, <<8]
    move($rsp[5], r_temp)


    // write the response ring

    ld_field_w_clr[r_rsp_ring_num, 0011, r_pkt_upper_addr1, \
                                   >>CRYPTO_LM_STATUS_QUE_SHIFT]
    move(r_temp, CRYPTO_LM_STATUS_QUE_MASK)
    alu[r_rsp_ring_num, r_rsp_ring_num, AND, r_temp]

    #if ( RESPONSE_RING_TYPE == CRYPTO_RING_EMU0 \
          || RESPONSE_RING_TYPE == CRYPTO_RING_EMU1 \
          || RESPONSE_RING_TYPE == CRYPTO_RING_EMU2 )

        move(r_poll_count, 100)

        ld_field_w_clr[r_temp, 1000, r_pkt_upper_addr1]

        .repeat

            #define_eval _PUT_LENGTH CRYPTO_RSP_LENGTH
            mem[put, $rsp[0], r_temp, <<8, r_rsp_ring_num, _PUT_LENGTH], \
                                                                sig_done[srs]
            #undef _PUT_LENGTH

            ctx_arb[srs]

            alu[--, $rsp[0], AND, 1, <<31]
            bne[crypto_threads_send_response_done#]

            alu[r_poll_count, r_poll_count, -, 1]

        .until ( r_poll_count <= 0 )

        // failed to put to ring, incr counter & go on without writing ring
        crypto_threads_update_counter(CRYPTO_CNTR_RSP_RING_WRITE_ERR)
        br[crypto_threads_send_response_done#]


    #elif ( RESPONSE_RING_TYPE == CRYPTO_RING_CTM )

        move(r_poll_count, 100)
       .repeat

            jump [r_rsp_ring_num, ctsrjt#], targets [j0#, j1#, j2#, j3#, j4#, \
                                                     j5#, j6#, j7#, j8#, j9#, \
                                                     j10#, j11#, j12#, j13#]
            ctsrjt#:
                j0#: br [ctsr0#]
                j1#: br [ctsr1#]
                j2#: br [ctsr2#]
                j3#: br [ctsr3#]
                j4#: br [ctsr4#]
                j5#: br [ctsr5#]
                j6#: br [ctsr6#]
                j7#: br [ctsr7#]
                j8#: br [ctsr8#]
                j9#: br [ctsr9#]
                j10#: br [ctsr10#]
                j11#: br [ctsr11#]
                j12#: br [ctsr12#]
                j13#: br [ctsr13#]

            ctsr0#:
                br_!inp_state[CTM_Ring0_Status, crypto_threads_rsp_ring_write#]
                br[ctsrpi#]
            ctsr1#:
                br_!inp_state[CTM_Ring1_Status, crypto_threads_rsp_ring_write#]
                br[ctsrpi#]
            ctsr2#:
                br_!inp_state[CTM_Ring2_Status, crypto_threads_rsp_ring_write#]
                br[ctsrpi#]
            ctsr3#:
                br_!inp_state[CTM_Ring3_Status, crypto_threads_rsp_ring_write#]
                br[ctsrpi#]
            ctsr4#:
                br_!inp_state[CTM_Ring4_Status, crypto_threads_rsp_ring_write#]
                br[ctsrpi#]
            ctsr5#:
                br_!inp_state[CTM_Ring5_Status, crypto_threads_rsp_ring_write#]
                br[ctsrpi#]
            ctsr6#:
                br_!inp_state[CTM_Ring6_Status, crypto_threads_rsp_ring_write#]
                br[ctsrpi#]
            ctsr7#:
                br_!inp_state[CTM_Ring7_Status, crypto_threads_rsp_ring_write#]
                br[ctsrpi#]
            ctsr8#:
                br_!inp_state[CTM_Ring8_Status, crypto_threads_rsp_ring_write#]
                br[ctsrpi#]
            ctsr9#:
                br_!inp_state[CTM_Ring9_Status, crypto_threads_rsp_ring_write#]
                br[ctsrpi#]
            ctsr10#:
                br_!inp_state[CTM_Ring10_Status, crypto_threads_rsp_ring_write#]
                br[ctsrpi#]
            ctsr11#:
                br_!inp_state[CTM_Ring11_Status, crypto_threads_rsp_ring_write#]
                br[ctsrpi#]
            ctsr12#:
                br_!inp_state[CTM_Ring12_Status, crypto_threads_rsp_ring_write#]
                br[ctsrpi#]
            ctsr13#:
                br_!inp_state[CTM_Ring13_Status, crypto_threads_rsp_ring_write#]

            ctsrpi#:
            alu[r_poll_count, r_poll_count, -, 1]

        .until ( r_poll_count <= 0 )

        // failed to see ring not full, incr counter & go on without writing ring
        crypto_threads_update_counter(CRYPTO_CNTR_RSP_RING_WRITE_ERR)
        br[crypto_threads_send_response_done#]

        crypto_threads_rsp_ring_write#:
        #define_eval _PUT_LENGTH CRYPTO_RSP_LENGTH
        ru_ctm_ring_put($rsp[0], r_rsp_ring_num, --, _PUT_LENGTH, srs, SIG_WAIT)
        #undef _PUT_LENGTH

    #elif ( RESPONSE_RING_TYPE == CRYPTO_RING_WQ )

        // write work queue. no flow control; assumes wq cannot become full

        ld_field_w_clr[r_temp, 1000, r_pkt_upper_addr1]

        #define_eval _PUT_LENGTH CRYPTO_RSP_LENGTH
        mem[qadd_work, $rsp[0], r_temp, <<8, r_rsp_ring_num, _PUT_LENGTH], \
                                                       ctx_swap[srs]
        #undef _PUT_LENGTH

    #else
        #error "crypto_threads_send_response: invalid response ring type."
    #endif

    crypto_threads_send_response_done#:


    // send response done, now free start of packet buffer
    .if (r_pkt_upper_addr1 & (1 << CRYPTO_REQ_FREE_BUF_BIT))

        // reload idx reg in case of use in other ctx
        localmem_set_address(r_lm_base, 0, LM_HANDLE_0)

        #if (BUF_RING_TYPE == CRYPTO_RING_EMU0 \
                || BUF_RING_TYPE == CRYPTO_RING_EMU1 \
                || BUF_RING_TYPE == CRYPTO_RING_EMU2 )

        move(r_poll_count, 100)
        move($rsp[0], r_pkt_strt_addr)

        .repeat

            #define_eval _PUT_LENGTH 1
            move(r_temp, BUF_RING_NUM)
            mem[put, $rsp[0], BUF_RING_ISLAND_ID, <<8, r_temp, _PUT_LENGTH], \
                                                                   sig_done[srs]
            #undef _PUT_LENGTH

            ctx_arb[srs]

            alu[--, $rsp[0], AND, 1, <<31]
            bne[crypto_threads_free_buf_done#]

            alu[r_poll_count, r_poll_count, -, 1]

        .until ( r_poll_count <= 0 )

        // failed to put to ring, incr counter & go on without writing ring
        crypto_threads_update_counter(CRYPTO_CNTR_BUF_RING_WRITE_ERR)
        br[crypto_threads_free_buf_done#]


        #elif ( BUF_RING_TYPE == CRYPTO_RING_CTM )

            #if ( BUF_RING_ISLAND_ID == 0x00 )
            move(r_poll_count, 100)
            .repeat

                move(r_temp, BUF_RING_NUM)
                jump [r_temp, ctfbjt#], targets [jt0#, jt1#, jt2#, jt3#, jt4#, jt5#, \
                                                jt6#, jt7#, jt8#, jt9#, jt10#, \
                                                jt11#, jt12#, jt13#]
                ctfbjt#:
                    jt0#: br [ctfb0#]
                    jt1#: br [ctfb1#]
                    jt2#: br [ctfb2#]
                    jt3#: br [ctfb3#]
                    jt4#: br [ctfb4#]
                    jt5#: br [ctfb5#]
                    jt6#: br [ctfb6#]
                    jt7#: br [ctfb7#]
                    jt8#: br [ctfb8#]
                    jt9#: br [ctfb9#]
                    jt10#: br [ctfb10#]
                    jt11#: br [ctfb11#]
                    jt12#: br [ctfb12#]
                    jt13#: br [ctfb13#]

                ctfb0#:
                    br_!inp_state[CTM_Ring0_Status, crypto_threads_buf_ring_write#]
                    br[ctfbpi#]
                ctfb1#:
                    br_!inp_state[CTM_Ring1_Status, crypto_threads_buf_ring_write#]
                    br[ctfbpi#]
                ctfb2#:
                    br_!inp_state[CTM_Ring2_Status, crypto_threads_buf_ring_write#]
                    br[ctfbpi#]
                ctfb3#:
                    br_!inp_state[CTM_Ring3_Status, crypto_threads_buf_ring_write#]
                    br[ctfbpi#]
                ctfb4#:
                    br_!inp_state[CTM_Ring4_Status, crypto_threads_buf_ring_write#]
                    br[ctfbpi#]
                ctfb5#:
                    br_!inp_state[CTM_Ring5_Status, crypto_threads_buf_ring_write#]
                    br[ctfbpi#]
                ctfb6#:
                    br_!inp_state[CTM_Ring6_Status, crypto_threads_buf_ring_write#]
                    br[ctfbpi#]
                ctfb7#:
                    br_!inp_state[CTM_Ring7_Status, crypto_threads_buf_ring_write#]
                    br[ctfbpi#]
                ctfb8#:
                    br_!inp_state[CTM_Ring8_Status, crypto_threads_buf_ring_write#]
                        br[ctfbpi#]
                ctfb9#:
                    br_!inp_state[CTM_Ring9_Status, crypto_threads_buf_ring_write#]
                    br[ctfbpi#]
                ctfb10#:
                    br_!inp_state[CTM_Ring10_Status, crypto_threads_buf_ring_write#]
                    br[ctfbpi#]
                ctfb11#:
                    br_!inp_state[CTM_Ring11_Status, crypto_threads_buf_ring_write#]
                    br[ctfbpi#]
                ctfb12#:
                    br_!inp_state[CTM_Ring12_Status, crypto_threads_buf_ring_write#]
                    br[ctfbpi#]
                ctfb13#:
                    br_!inp_state[CTM_Ring13_Status, crypto_threads_buf_ring_write#]

                ctfbpi#:
                alu[r_poll_count, r_poll_count, -, 1]

            .until ( r_poll_count <= 0 )

            // failed to see ring not full, incr counter & go on without writing ring
            crypto_threads_update_counter(CRYPTO_CNTR_BUF_RING_WRITE_ERR)
            br[crypto_threads_free_buf_done#]

            crypto_threads_buf_ring_write#:
            #define_eval _PUT_LENGTH 1
            move($rsp[0], r_pkt_strt_addr)
            move(r_temp, BUF_RING_NUM)
            ru_ctm_ring_put($rsp[0], r_temp, --, _PUT_LENGTH, srs, SIG_WAIT)
            #undef _PUT_LENGTH

            #else

            // CTM ring in external island, can't check the status.
            // Assume can't become full. Note the <<8 for 40 bit
            // addressing to the island not used with ct instruction.
            move($rsp[0], r_pkt_strt_addr)
            ld_field_w_clr[r_temp, 1000, r_pkt_upper_addr2, <<8]
            alu[r_temp, r_temp, AND, 0x3f, <<24]
            move(r_temp1, BUF_RING_NUM << 2)
            ct[ring_put, $rsp[0], r_temp,  r_temp1, 1], ctx_swap[srs]

            #endif
    #else
        #error "crypto_threads_send_response: invalid buf ring type."
    #endif

    crypto_threads_free_buf_done#:

    .endif


    #if ( HALT_ON_ERROR == 1 ) // for DEBUG
        .if ( _status != CRYPTO_RSP_STATUS_OK )
            ctx_arb[bpt]
        .endif
    #endif

    #undef r_pkt_upper_addr1
    #undef r_pkt_upper_addr2
    #undef r_pkt_strt_addr
    #undef r_pkt_cont_addr
    #undef r_pkt_end_addr
    #undef r_pkt_wrbk_addr
    #undef r_pkt_lengths1
    #undef r_pkt_lengths2
    #undef r_pkt_lengths3
    #undef r_sa_addr
    #undef r_sa_ctlwrd
    #undef r_user_ctl

.end

#endm
#endif // OVERRIDE_RESPONSE_RING == 0


#if ( ENABLE_ANTI_REPLAY == 1 )
/**
 *  Processing for anti-replay
 *  Invoked in input thread for prelimary check, and at end
 *  of output thread on good decrypts
 *
 *  Does the following:
 *
 *  a) Checks sequence number in packet against anti replay window for the
 *     current SA. If to the left of window ( i.e. sequence number in packet
 *     is < lowest number in window ), marks status as bad and updates sa
 *     resident error counter
 *
 *  b) If to the right of window ( i.e. sequence number in packet is > highest
 *     number in window ), updates highest number in window, and updates window
 *     status bit mask
 *
 *  c) If in the window and already received, marks status as bad and updates
 *     sa resident error counter
 *
 *  d) If in the window and not already received, leaves status as good and
 *     updates the window status bit mask.
 *
 *
 *  @param  _me_ctx  GPR,   me context ( 0 through 7 )
 *  @param  _buf_sel GPR,   buffer select 0-3
 *  @param  _status  GPR,   one of CRYPTO_RSP_STATUS_* to be
 *                          sent in response, may be modified
 *                          here on ar check failure
 *  @param  _UPDATE  CONST, if 1, will do the replay check plus update
 *                          replay window data as needed. if 0, will only
 *                          do the check. Check only is done in input
 *                          thread, Check plus update in output thread.
 */
#macro crypto_threads_anti_replay (_me_ctx, _buf_sel, _status, _UPDATE )
.begin

    .sig ars
    .reg r_temp, r_temp1, r_temp2
    .reg r_seq_num, r_seq_num_hi, r_window_size, r_bl
    .reg $xd[4]
    .xfer_order $xd

    // setup direct index/offset access to lm vars here
    crypto_threads_get_lm_temp_addr(r_temp, _me_ctx, _buf_sel)
    localmem_set_address(r_temp, 0, LM_HANDLE_0)
    nop
    nop
    nop
    #define_eval CRYPTO_LM_PKT_END_ADDR_UPPER_LW (CRYPTO_LM_PKT_END_ADDR_UPPER >> 2)
    #define_eval CRYPTO_LM_SA_ADDR_LW            (CRYPTO_LM_SA_ADDR >> 2)
    #define_eval CRYPTO_LM_SA_CTLWRD_LW          (CRYPTO_LM_SA_CTLWRD >> 2)
    #define_eval r_pkt_upper_addr2        *l$index0[CRYPTO_LM_PKT_END_ADDR_UPPER_LW]
    #define_eval r_sa_addr                *l$index0[CRYPTO_LM_SA_ADDR_LW]
    #define_eval r_sa_ctlwrd              *l$index0[CRYPTO_LM_SA_CTLWRD_LW]

    // if anti replay is enabled in SA..
    move(r_temp1, r_sa_ctlwrd)
    .if ( r_temp1 & ( 1 << CRYPTO_SA_AR_ENA_SHIFT ))

        // move sequence num into gpr
        localmem_read1(r_seq_num, r_temp, CRYPTO_LM_PKT_SEQ_NUM_LOW)
        // fix index reg for later use
        localmem_set_address(r_temp, 0, LM_HANDLE_0)
        nop
        nop
        nop

        // lock access to the ar data in sa
        #if ( _UPDATE == 1 )
        move(r_temp1, CRYPTO_REQ_SA_VERSION_MASK) // mask off version bits
        alu[r_temp2, r_sa_addr, AND~, r_temp1]
        alu[r_temp2, r_temp2, OR, CRYPTO_SA_AR_LOCK_OFFSET, <<2]
        ld_field_w_clr[r_temp1, 0001, \
            r_pkt_upper_addr2, >>CRYPTO_REQ_SA_ADDR_UPPER_SHIFT]
        alu[r_temp1, --, B, r_temp1, <<24] // sa addr upper byte
        move($xd[0], (1 << CRYPTO_SA_AR_LOCK_SHIFT))
        .repeat
            mem[test_set, $xd[0], r_temp1, <<8, r_temp2, 1], sig_done[ars]
            ctx_arb[ars]
            // potential for hang here
        .until ( ($xd[0] & (1 << CRYPTO_SA_AR_LOCK_SHIFT)) == 0)
        #endif


        // read the ar data from the sa
        move(r_temp1, CRYPTO_REQ_SA_VERSION_MASK) // mask off version bits
        alu[r_temp2, r_sa_addr, AND~, r_temp1]
        alu[r_temp2, r_temp2, OR, CRYPTO_SA_AR_CNTR_LOW, <<2]
        ld_field_w_clr[r_temp1, 0001, \
            r_pkt_upper_addr2, >>CRYPTO_REQ_SA_ADDR_UPPER_SHIFT]
        alu[r_temp1, --, B, r_temp1, <<24] // sa addr upper byte
        #pragma warning(disable:5008) // $xd[1] not used now...
        mem[atomic_read, $xd[0], r_temp1, <<8, r_temp2, 4], ctx_swap[ars]
        #pragma warning(default:5008)

        #define r_ar_ctr_lo $xd[0]
        #define r_ar_ctr_hi $xd[1]
        #define r_ar_bm_lo  $xd[2]
        #define r_ar_bm_hi  $xd[3]


        // create anti-replay window size
        move(r_temp1, r_sa_ctlwrd)
        move(r_window_size, 64) // window size 64
        .if (!(r_temp1 & ( 1 << CRYPTO_SA_AR_WIN_64_SHIFT)))
            move(r_window_size, 32) // window size 32
        .endif


        // create hi 32 bits of Sequence Number if required
        // see RFC 4303, Appendix A2 for details

        move(r_seq_num_hi, r_ar_ctr_hi)

        move(r_temp1, r_sa_ctlwrd)
        .if (r_temp1 & ( 1 << CRYPTO_SA_SEQNUM_64_SHIFT))

            alu[r_bl, r_ar_ctr_lo, -, r_window_size]
            alu[r_bl, r_bl, +, 1]

            alu[r_temp2, r_window_size, -, 1]
            .if_unsigned (r_ar_ctr_lo >= r_temp2)
                // Case A
                .if_unsigned (r_seq_num < r_bl)
                    alu[r_seq_num_hi, r_ar_ctr_hi, +, 1]
                .endif
            .else
                // Case B
                .if_unsigned (r_seq_num >= r_bl)
                    alu[r_seq_num_hi, r_ar_ctr_hi, -, 1]
                .endif
            .endif
        .endif


        // bit map stores bits for sequence numbers "r_ar_ctr_lo" through
        //  "r_ar_ctr_lo - r_window_size + 1"

        // r_temp1:r_temp = 64-bit value of Lower bound of window

        alu[r_temp,  r_window_size, -, 1]
        alu[r_temp,  r_ar_ctr_lo, -, r_temp]
        alu[r_temp1, r_ar_ctr_hi, -carry, 0]
        blo[ct_anti_replay_chk_chk_right#]

        // ok to do 'to the left' comparison
        //
        // if rcv'd seq num is to 'left' of window, mark as bad
        //

        // r_seq_num_hi:r_seq_num - r_temp1:r_temp

        alu[r_temp2, r_seq_num, -, r_temp]
        alu[r_temp2, r_seq_num_hi, -carry, r_temp1]
        bhs[ct_anti_replay_chk_chk_right#]

        // Sequence Number outside of the window on the "left" side
        move(_status, CRYPTO_RSP_STATUS_REPLAY_TOO_SMALL)
        br[car_done#]

ct_anti_replay_chk_chk_right#:

        //
        // if rcv'd seq num is to 'right' of window, make right of window
        // equal to the sequence number, shift the window bit mask, and
        // write out the window data
        //

        // r_ar_ctr_hi:r_ar_ctr_lo - r_seq_num_hi:r_seq_num

        alu[r_temp, r_ar_ctr_lo, -, r_seq_num]
        alu[r_temp2, r_ar_ctr_hi, -carry, r_seq_num_hi]
        blo[crypto_threads_anti_replay_outside#]

        // sequence number is within the window

        #define r_bit_num r_temp

        #define r_rcvd_flag r_temp1
        #pragma warning(disable:5008) // not sure why nfas flags this...
        move(r_rcvd_flag, 0 )
        #pragma warning(default:5008)

        .if ( r_bit_num < 32 )

            alu[r_temp2, 31, -, r_bit_num]
            .if ( r_window_size == 64 )
                alu[--, r_temp2, OR, 0]
                alu[r_rcvd_flag, r_ar_bm_hi, AND, 1, <<indirect]
            .else
                alu[--, r_temp2, OR, 0]
                alu[r_rcvd_flag, r_ar_bm_lo, AND, 1, <<indirect]
            .endif

        .else // (r_bit_num >= 32)

            alu[r_temp2, r_bit_num, -, 32]
            alu[r_temp2, 31, -, r_temp2]
            alu[--, r_temp2, OR, 0]
            alu[r_rcvd_flag, r_ar_bm_lo, AND, 1, <<indirect]
        .endif

        // if pkt already rcvd, mark it bad and increment
        // sa counter
        .if ( r_rcvd_flag )

            move(_status, CRYPTO_RSP_STATUS_REPLAY_ALREADY_USED)

        // if pkt not already rcvd, update ar bitmap and write it
        // out to sa
        .else

            #if ( _UPDATE == 1 )

            .if ( r_window_size == 64 )
                .if ( r_bit_num < 32 )
                    alu[r_temp2, 31, -, r_bit_num]
                    alu[--, r_temp2, OR, 0]
                    alu[$xd[0], r_ar_bm_hi, OR, 1, <<indirect]

                    move(r_temp2, (CRYPTO_SA_AR_BMAP_HIGH << 2))

                .else // (r_bit_num >= 32)

                    alu[r_temp2, r_bit_num, -, 32]
                    alu[r_temp2, 31, -, r_temp2]
                    alu[--, r_temp2, OR, 0]
                    alu[$xd[0], r_ar_bm_lo, OR, 1, <<indirect]

                    move(r_temp2, (CRYPTO_SA_AR_BMAP_LOW << 2))
                .endif

                .else
                    alu[r_temp2, 31, -, r_bit_num]
                    alu[--, r_temp2, OR, 0]
                    alu[$xd[0], r_ar_bm_lo, OR, 1, <<indirect]

                    move(r_temp2, (CRYPTO_SA_AR_BMAP_LOW << 2))
                .endif

            // write the ar bmap word back to the sa
            move(r_temp1, CRYPTO_REQ_SA_VERSION_MASK) // mask off version bits
            alu[r_temp1, r_sa_addr, AND~, r_temp1]
            alu[r_temp2, r_temp2, OR, r_temp1]

            ld_field_w_clr[r_temp1, 0001, \
                    r_pkt_upper_addr2, >>CRYPTO_REQ_SA_ADDR_UPPER_SHIFT]
            alu[r_temp1, --, B, r_temp1, <<24] // sa addr upper byte

            mem[atomic_write, $xd[0], r_temp1, <<8, r_temp2, 1], ctx_swap[ars]

            #endif // _UPDATE == 1

        .endif

        br[car_done#]

crypto_threads_anti_replay_outside#:

        // rcv'd seq num is to 'right' of window

        #if ( _UPDATE == 1 )

        move($xd[0], r_seq_num)     // ar cntr lo
        move($xd[1], r_seq_num_hi)  // ar cntr hi

        #define r_shift r_temp

        alu[r_shift, 0, -, r_shift]

        .if ( r_window_size == 32 )

            .if_unsigned ( r_shift > 32 )
                move($xd[2], 1 << 31) // bm lo
            .else
                alu[--, r_shift, OR, 0]
                alu[r_temp2, --, B, r_ar_bm_lo, >>indirect] // bm lo
                alu[$xd[2], r_temp2, OR, 1, <<31]
            .endif
            move($xd[3], 0)

        .else // (r_window_size == 64 )

            .if_unsigned ( r_shift > 64 )
                move($xd[2], 0) // bm lo
                move($xd[3], 1 << 31) // bm hi
            .elif ( r_shift >= 32 )
                alu[r_shift, r_shift, -, 32]
                alu[--, r_shift, OR, 0]
                alu[$xd[2], --, B, r_ar_bm_hi, >>indirect] // bm lo
                alu[$xd[3], --, B, 1, <<31] // bm hi
            .else // ( r_shift < 32 )
                alu[r_temp2, --, B, r_ar_bm_hi]
                alu[--, r_shift, OR, 0]
                dbl_shf[$xd[2], r_temp2, r_ar_bm_lo, >>indirect] // bm lo
                alu[--, r_shift, OR, 0]
                alu[r_temp2, --, B, r_temp2, >>indirect]
                alu[$xd[3], r_temp2, OR, 1, <<31] // bm hi
            .endif
        .endif


        // write the ar data back to the sa
        ld_field_w_clr[r_temp1, 0001, \
                r_pkt_upper_addr2, >>CRYPTO_REQ_SA_ADDR_UPPER_SHIFT]
        alu[r_temp1, --, B, r_temp1, <<24] // sa addr upper byte
        move(r_temp2, CRYPTO_REQ_SA_VERSION_MASK) // mask off version bits
        alu[r_temp2, r_sa_addr, AND~, r_temp2]
        alu[r_temp2, r_temp2, OR, CRYPTO_SA_AR_CNTR_LOW, <<2]
        mem[atomic_write, $xd[0], r_temp1, <<8, r_temp2, 4], ctx_swap[ars]

        #endif // _UPDATE == 1


        // crypto anti replay handling done
        car_done#:



        // release access to the ar data in sa

        #if ( _UPDATE == 1 )

        move(r_temp1, CRYPTO_REQ_SA_VERSION_MASK) // mask off version bits
        alu[r_temp2, r_sa_addr, AND~, r_temp1]
        alu[r_temp2, r_temp2, OR, CRYPTO_SA_AR_LOCK_OFFSET, <<2]
        ld_field_w_clr[r_temp1, 0001, \
            r_pkt_upper_addr2, >>CRYPTO_REQ_SA_ADDR_UPPER_SHIFT]
        alu[r_temp1, --, B, r_temp1, <<24] // sa addr upper byte
        move($xd[0], (1 << CRYPTO_SA_AR_LOCK_SHIFT))
        mem[clr, $xd[0], r_temp1, <<8, r_temp2, 1], ctx_swap[ars]
        #endif // _UPDATE == 1

        #undef r_pkt_upper_addr2
        #undef r_sa_addr
        #undef r_sa_ctlwrd
        #undef r_ar_ctr_lo
        #undef r_ar_ctr_hi
        #undef r_ar_bm_lo
        #undef r_ar_bm_hi

    .endif

.end
#endm
#endif // ENABLE_ANTI_REPLAY

/**
 *  Load core specific constant values to CIB for use by
 *  CIB sequences
 *
 *  @param  _cr          XFR transfer registers to use
 *  @param  _crypto_ctx  CONST, crypto context ( 0 through 5 )
 */
#macro crypto_threads_setup_CIB_constants (_cr, _crypto_ctx)
    #define_eval TBASE  crypto_threads_get_temp_addr_const(_crypto_ctx)
    crypto_library_load_constants (_cr, _crypto_ctx, TBASE)
#endm

/**
 *  Load crypto sequences into compressed instruction buffer (CIB)
 *
 *  @param  _cr   XFR transfer registers to use
 *  @param  _ctx  CONST, crypto context ( 0 through 5 )
 */
#macro crypto_threads_load_CIB_sequences (_cr, _ctx)
    #define CLSTATIC CRYPTO_LOAD_STATIC
    crypto_library_load(CLSTATIC,_cr,_ctx,ipsec_enc,_ctx)
    crypto_library_load(CLSTATIC,_cr,_ctx,ipsec_enc_esn,_ctx)
    crypto_library_load(CLSTATIC,_cr,_ctx,ipsec_enc_strt,_ctx)
    crypto_library_load(CLSTATIC,_cr,_ctx,ipsec_enc_cont,_ctx)
    crypto_library_load(CLSTATIC,_cr,_ctx,ipsec_enc_end,_ctx)
    crypto_library_load(CLSTATIC,_cr,_ctx,ipsec_enc_end_esn,_ctx)
    crypto_library_load(CLSTATIC,_cr,_ctx,ipsec_enc_aesgcm,_ctx)
    crypto_library_load(CLSTATIC,_cr,_ctx,ipsec_enc_aesgcm_strt,_ctx)
    crypto_library_load(CLSTATIC,_cr,_ctx,ipsec_enc_aesgcm_end,_ctx)
    #if ENABLE_CRYPTO_STATE_SAVE_RESTORE == 1
    crypto_library_load(CLSTATIC,_cr,_ctx,ipsec_enc_aesgcm_strt_save,_ctx)
    crypto_library_load(CLSTATIC,_cr,_ctx,ipsec_enc_aesgcm_end_restore,_ctx)
    #endif
    crypto_library_load(CLSTATIC,_cr,_ctx,ipsec_dec,_ctx)
    crypto_library_load(CLSTATIC,_cr,_ctx,ipsec_dec_esn,_ctx)
    crypto_library_load(CLSTATIC,_cr,_ctx,ipsec_dec_strt,_ctx)
    crypto_library_load(CLSTATIC,_cr,_ctx,ipsec_dec_strt_nw,_ctx)
    crypto_library_load(CLSTATIC,_cr,_ctx,ipsec_dec_cont,_ctx)
    crypto_library_load(CLSTATIC,_cr,_ctx,ipsec_dec_cont_nw,_ctx)
    crypto_library_load(CLSTATIC,_cr,_ctx,ipsec_dec_end,_ctx)
    crypto_library_load(CLSTATIC,_cr,_ctx,ipsec_dec_end_esn,_ctx)
    crypto_library_load(CLSTATIC,_cr,_ctx,ipsec_dec_aesgcm,_ctx)
    crypto_library_load(CLSTATIC,_cr,_ctx,ipsec_dec_aesgcm_strt,_ctx)
    crypto_library_load(CLSTATIC,_cr,_ctx,ipsec_dec_aesgcm_end,_ctx)
#endm

/**
 *  Allocate a bulk crypto core and load it with library sequences
 *
 *  @param  _crypto_ctx  GPR, crypto core/context ( 0 through 5 )
 */
#macro crypto_threads_init_core (_crypto_ctx)
.begin

    .reg r_temp_strt
    .reg $cr[32]
    .xfer_order $cr

    // enable the bulk core ( toggle the enable bit in bulk cfg reg )
    #if (IS_NFPTYPE(__NFP6000) || IS_NFPTYPE(__NFP3800)) // not NFP3200
        .sig z
        .reg r_tmp, r_addr, r_offset

        #define CRYPTO_BULK_XPB_DEVICE_ID 23
        // EAS says 0x70000 for offset; must include 4 bits of XPB ID
        #define CRYPTO_BULK_BASIC_CONFIG_OFFSET 0x0000

        // using local, self-island address ( i.e. 0 )
        alu_shf[r_addr, --, B, CRYPTO_BULK_XPB_DEVICE_ID, <<16]
        move(r_offset, CRYPTO_BULK_BASIC_CONFIG_OFFSET)
        ct[xpb_read, $cr[0], r_addr, r_offset, 1], ctx_swap[z]
        alu[--, _crypto_ctx, OR, 0]
        alu[r_tmp, --, B, 1, <<indirect]
        alu[$cr[0], $cr[0], AND~, r_tmp] // clear the enable bit
        ct[xpb_write, $cr[0], r_addr, r_offset, 1], ctx_swap[z]
        alu[$cr[0], $cr[0], OR, r_tmp] // set the enable bit
        ct[xpb_write, $cr[0], r_addr, r_offset, 1], ctx_swap[z]
        nop // time for enable to take effect..?
        nop // during Kestrel simulation debug appears there
        nop // may be a race condition between the enable bit
        nop // and the get core instruction. so added
        nop // more nop's here
        nop
        nop
        nop
        nop
        nop

        #undef CRYPTO_BULK_XPB_DEVICE_ID
        #undef CRYPTO_BULK_BASIC_CONFIG_OFFSET
    #endif


    // Get the bulk core and assign it to crypto dispatcher context.
    crypto_get_core ($cr, _crypto_ctx)
    crypto_send ($cr)


    // Calculate temp address for this crypto context/core
    crypto_threads_get_temp_addr(r_temp_strt, _crypto_ctx)

    #pragma warning(disable:5151) // not sure why nfas warns here..

    // switching on context is needed because of crypto lib macros setup
    .if ( _crypto_ctx == 0 )
        #if ( CRYPTO_MIN_CTX <= 0 && CRYPTO_MAX_CTX >= 0 )
          // address within CIB buffer where we start loading code sequences;
          // variables & constants come first
          #define_eval CIB_static_start_core_address 16
          // init for loading sequences, use temp var area as staging memory
          crypto_init_CIB_setup (r_temp_strt, 0, CIB_static_start_core_address)
          // Load the library constants into the core's CIB
          crypto_threads_setup_CIB_constants ($cr, 0)
          // Send compressed crypto sequences to the core for use during
          // run-time packet processing.
          crypto_threads_load_CIB_sequences ($cr, 0)
        #endif
    .elif ( _crypto_ctx == 1 )
        #if ( CRYPTO_MIN_CTX <= 1 && CRYPTO_MAX_CTX >= 1 )
          #define_eval CIB_static_start_core_address  16
          crypto_init_CIB_setup (r_temp_strt, 0, CIB_static_start_core_address)
          crypto_threads_setup_CIB_constants ($cr, 1)
          crypto_threads_load_CIB_sequences ($cr, 1)
        #endif
    .elif ( _crypto_ctx == 2 )
        #if ( CRYPTO_MIN_CTX <= 2 && CRYPTO_MAX_CTX >= 2 )
          #define_eval CIB_static_start_core_address  16
          crypto_init_CIB_setup (r_temp_strt, 0, CIB_static_start_core_address)
          crypto_threads_setup_CIB_constants ($cr, 2)
          crypto_threads_load_CIB_sequences ($cr, 2)
        #endif
    .elif ( _crypto_ctx == 3 )
        #if ( CRYPTO_MIN_CTX <= 3 && CRYPTO_MAX_CTX >= 3 )
          #define_eval CIB_static_start_core_address  16
          crypto_init_CIB_setup (r_temp_strt, 0, CIB_static_start_core_address)
          crypto_threads_setup_CIB_constants ($cr, 3)
          crypto_threads_load_CIB_sequences ($cr, 3)
        #endif
    .elif ( _crypto_ctx == 4 )
        #if ( CRYPTO_MIN_CTX <= 4 && CRYPTO_MAX_CTX >= 4 )
          #define_eval CIB_static_start_core_address  16
          crypto_init_CIB_setup (r_temp_strt, 0, CIB_static_start_core_address)
          crypto_threads_setup_CIB_constants ($cr, 4)
          crypto_threads_load_CIB_sequences ($cr, 4)
        #endif
    .else // _crypto_ctx == 5
        #if ( CRYPTO_MIN_CTX <= 5 && CRYPTO_MAX_CTX >= 5 )
          #define_eval CIB_static_start_core_address  16
          crypto_init_CIB_setup (r_temp_strt, 0, CIB_static_start_core_address)
          crypto_threads_setup_CIB_constants ($cr, 5)
          crypto_threads_load_CIB_sequences ($cr, 5)
        #endif
    .endif

    #pragma warning(default:5151)

    crypto_threads_update_counter(CRYPTO_CNTR_INIT_CORE)

.end

#endm


/**
 * Load two 16 bit values into 32 bit xfr reg
 */
#macro crypto_threads_ld_xr(_xfr, _lo, _hi)
.begin
    .reg l, h
     alu[l, _lo, OR, 0]
     alu[h, --, B, _hi, <<16]
     alu[_xfr, h, OR, l]
.end
#endm


/**
 * Load crypto params into transfer regs, used for enc/dec and start
 * sequences. Needed because nfas having trouble with register allocation
 * when use crypto lib version.
 */
#macro crypto_threads_ld_parms(_xfr, _sram_ptext, _sram_ctext, _ptext_size, \
                        _sram_seq_ua, _sram_iv_spi, \
                        _hash_auth_len, _cfg_modes_select, \
                        _ck, _hk, _ha)
.begin
    crypto_threads_ld_xr(_xfr[0], _sram_ptext, _sram_ctext)
    crypto_threads_ld_xr(_xfr[1], _ptext_size, _sram_seq_ua)
    alu[_xfr[2], --, B, _sram_iv_spi]
    alu[_xfr[3], --, B, _hash_auth_len]
    alu[_xfr[4], --, B, _cfg_modes_select]
    crypto_threads_ld_xr(_xfr[5], _ck, _hk)
    crypto_threads_ld_xr(_xfr[6], _ha, 0)
    alu[_xfr[7], --, B, 0]
.end
#endm


/**
 *  Simplification macro for use in crypto_threads_encrypt, below
 */
#macro crypto_threads_encrypt_s ( _params, _runcmd, _crypto_ctx, \
                                   _sram_vars, _gcm_esn_ena, _BR_TARGET)
    #if ( CRYPTO_MIN_CTX <= _crypto_ctx  && CRYPTO_MAX_CTX >= _crypto_ctx )
        .if (_gcm_esn_ena & 2) // AES GCM ESP
            #define_eval _crypto_transfer_length 4
            crypto_load_library_sequence (_params, ipsec_enc_aesgcm, \
                                         _crypto_ctx, _sram_vars)
            crypto_run_library_sequence (_runcmd, _crypto_ctx, \
                                         ipsec_enc_aesgcm, \
                                         _crypto_ctx, _sram_vars, 0, *nosend*)
        .else
            .if (_gcm_esn_ena & 1) // NON AES GCM + ESN
                #define_eval _crypto_transfer_length 4
                crypto_load_library_sequence (_params, ipsec_enc_esn, \
                                           _crypto_ctx, _sram_vars)
                crypto_run_library_sequence (_runcmd, _crypto_ctx, \
                                            ipsec_enc_esn, \
                                           _crypto_ctx, _sram_vars, 0, *nosend*)
            .else // NON AES GCM, NON ESN
                #define_eval _crypto_transfer_length 4
                crypto_load_library_sequence (_params, ipsec_enc, \
                                           _crypto_ctx, _sram_vars)
                crypto_run_library_sequence (_runcmd, _crypto_ctx, \
                                           ipsec_enc, \
                                           _crypto_ctx, _sram_vars, 0, *nosend*)
            .endif
        .endif
    #endif
    br[_BR_TARGET]
#endm


/**
 *  Load Encrypt Sequence params to sram, and Generate Encrypt Sequence cmd
 *  in xfr regs
 *
 *  @param  _runcmd        XFR, transfer register array (2 lwords) returned with
 *                            encrypt sequence cmd
 *  @param  _buf_num       GPR, buffer selection, one of 0,1,2,3
 *  @param  _crypto_ctx    GPR, crypto context ( 0 through 5 )
 *  @param  _sa_ctlwrd     XFR, sa control word
 *  @param  _sram_vars     GPR, crypto sram address of sequence variables
 *  @param  _sram_ptext    GPR, crypto sram address of plain(in)text start
 *  @param  _sram_ctext    GPR, crypto sram address of cipher(out)text start
 *  @param  _ptext_size    GPR, length of plaintext, in bytes - 1
 *  @param  _sram_iv_spi   GPR, 15:0  sram addr of spi / aesgcm aad
 *                              31:16 sram addr of iv or aesgcm iv constructed
 *  @param  _sram_seq_ua   GPR, crypto sram address of seq number 63:32
 *                              or sram address of aesgcm len(A)/Len(C) field
 *  @param  _hash_auth_len GPR, 15:0  length of auth header data, in bytes - 1,
 *                              31:16 icv/hash length - 1 byte or sram addr of
 *                                     aesgcm zeros
 *  @param  _cfg_sel_modes GPR, 15:0  cfg_pulse_R and cfg_sel_R fields for
 *                                    Crypt_Setup_Regs_0
 *                              31:16 cfg_Crypt0_R field for Crypt_Setup_Regs_0
 */
#macro crypto_threads_encrypt (_runcmd, _buf_num, _crypto_ctx, _sa_ctlwrd, \
                               _sram_vars, _sram_ptext, _sram_ctext, \
                               _ptext_size, _sram_iv_spi, _sram_seq_ua, \
                               _hash_auth_len, _cfg_modes_select )
.begin

    .reg $params[8]
    .xfer_order $params
    .reg r_tmp, r_gcm_esn_ena, r_ptext_size_m1
    .reg r_ck, r_hk, r_ha
    .sig s

    crypto_threads_get_sa_addr(r_ck, _crypto_ctx, _buf_num)
    alu[r_ck, r_ck, +, CRYPTO_SA_DMA_KEYS_OFFSET]
    crypto_threads_get_hash_key_addr(r_hk, _crypto_ctx, _buf_num)
    crypto_threads_get_hash_addr(r_ha, _crypto_ctx, _buf_num)

    alu[r_ptext_size_m1, _ptext_size, -, 1]

    alu[r_gcm_esn_ena, CRYPTO_SA_SEQNUM_64_MASK, AND, _sa_ctlwrd, \
                                  >>CRYPTO_SA_SEQNUM_64_SHIFT]
    alu[r_tmp, 7, AND, _cfg_modes_select, >>18]
    .if (r_tmp == 7)
        alu[r_gcm_esn_ena, r_gcm_esn_ena, OR, 2]
    .endif

    // Load params to xfr regs using local macro instead of the one in
    // the crypto lib, because nfas is having register allocation issues
    crypto_threads_ld_parms($params,  \
                            _sram_ptext, _sram_ctext, \
                            r_ptext_size_m1, _sram_seq_ua, \
                            _sram_iv_spi, \
                            _hash_auth_len, \
                            _cfg_modes_select, \
                            r_ck, r_hk, r_ha)


    // switching on context needed because of crypto lib macros setup

    jump [_crypto_ctx, ctejt#], targets [je0#, je1#, je2#, je3#, je4#, je5#]
    ctejt#:
        je0#: br[cte0#]
        je1#: br[cte1#]
        je2#: br[cte2#]
        je3#: br[cte3#]
        je4#: br[cte4#]
        je5#: br[cte5#]

    cte0#:
        crypto_threads_encrypt_s ( $params, _runcmd, 0, \
                 _sram_vars, r_gcm_esn_ena, crypto_threads_encrypt_done#)
    cte1#:
        crypto_threads_encrypt_s ( $params, _runcmd, 1, \
                 _sram_vars, r_gcm_esn_ena, crypto_threads_encrypt_done#)
    cte2#:
        crypto_threads_encrypt_s ( $params, _runcmd, 2, \
                 _sram_vars, r_gcm_esn_ena, crypto_threads_encrypt_done#)
    cte3#:
        crypto_threads_encrypt_s ( $params, _runcmd, 3, \
                 _sram_vars, r_gcm_esn_ena, crypto_threads_encrypt_done#)
    cte4#:
        crypto_threads_encrypt_s ( $params, _runcmd, 4, \
                 _sram_vars, r_gcm_esn_ena, crypto_threads_encrypt_done#)
    cte5#:
        crypto_threads_encrypt_s ( $params, _runcmd, 5, \
                 _sram_vars, r_gcm_esn_ena, crypto_threads_encrypt_done#)

    crypto_threads_encrypt_done#:
.end
#endm


/**
 *  Simplification macro for use in crypto_threads_decrypt, below
 */
#macro crypto_threads_decrypt_s ( _params, _runcmd, _crypto_ctx, \
                                   _sram_vars, _gcm_esn_ena, _BR_TARGET)
    #if ( CRYPTO_MIN_CTX <= _crypto_ctx  && CRYPTO_MAX_CTX >= _crypto_ctx )
        .if (_gcm_esn_ena & 2) // AES GCM ESP
            #define_eval _crypto_transfer_length 4
            crypto_load_library_sequence (_params, ipsec_dec_aesgcm, \
                                         _crypto_ctx, _sram_vars)
            crypto_run_library_sequence (_runcmd, _crypto_ctx, \
                                         ipsec_dec_aesgcm, \
                                         _crypto_ctx, _sram_vars, 0, *nosend*)
        .else
            .if (_gcm_esn_ena & 1) // NON AES GCM + ESN
                #define_eval _crypto_transfer_length 4
                crypto_load_library_sequence (_params, ipsec_dec_esn, \
                                           _crypto_ctx, _sram_vars)
                crypto_run_library_sequence (_runcmd, _crypto_ctx, \
                                            ipsec_dec_esn, \
                                           _crypto_ctx, _sram_vars, 0, *nosend*)
            .else // NON AES GCM, NON ESN
                #define_eval _crypto_transfer_length 4
                crypto_load_library_sequence (_params, ipsec_dec, \
                                           _crypto_ctx, _sram_vars)
                crypto_run_library_sequence (_runcmd, _crypto_ctx, \
                                           ipsec_dec, \
                                           _crypto_ctx, _sram_vars, 0, *nosend*)
            .endif
        .endif
    #endif
    br[_BR_TARGET]
#endm


/**
 *  Load Decrypt Sequence params to sram, and Generate Decrypt Sequence cmd
 *  in xfr regs
 *
 *  @param  _runcmd      XFR, transfer register array (2 lwords) returned with
 *                            decrypt sequence cmd
 *  @param  _buf_num     GPR, buffer selection, one of 0,1,2,3
 *  @param  _crypto_ctx  GPR, crypto context ( 0 through 5 )
 *  @param  _sa_ctlwrd   XFR, sa control word
 *  @param  _sram_vars   GPR, crypto sram address of sequence variables
 *  @param  _sram_ptext  GPR, crypto sram address of cipher(in)text start
 *  @param  _sram_ctext  GPR, crypto sram address of plain(out)text start
 *  @param  _ptext_size  GPR, length of plaintext, in bytes - 1
 *  @param  _sram_iv_spi   GPR, 15:0  sram addr of spi / aesgcm aad
 *                              31:16 sram addr of iv or aesgcm iv constructed
 *  @param  _sram_seq_ua   GPR, crypto sram address of seq number 63:32
 *                              or sram address of aesgcm len(A)/Len(C) field
 *  @param  _hash_auth_len GPR, 15:0  length of auth header data, in bytes - 1,
 *                              31:16 icv/hash length - 1 byte or sram addr of
 *                                     aesgcm zeros
 *  @param  _cfg_sel_modes GPR, 15:0  cfg_pulse_R and cfg_sel_R fields for
 *                                    Crypt_Setup_Regs_0
 *                              31:16 cfg_Crypt0_R field for Crypt_Setup_Regs_0
 */
#macro crypto_threads_decrypt (_runcmd, _buf_num, _crypto_ctx, _sa_ctlwrd, \
                               _sram_vars, _sram_ptext, _sram_ctext, \
                               _ptext_size, _sram_iv_spi, _sram_seq_ua, \
                               _hash_auth_len, _cfg_modes_select )
.begin

    .reg $params[8]
    .xfer_order $params
    .reg r_tmp, r_gcm_esn_ena, r_ptext_size_m1
    .reg r_ck, r_hk, r_ha
    .sig s

    crypto_threads_get_sa_addr(r_ck, _crypto_ctx, _buf_num)
    alu[r_ck, r_ck, +, CRYPTO_SA_DMA_KEYS_OFFSET]
    crypto_threads_get_hash_key_addr(r_hk, _crypto_ctx, _buf_num)
    crypto_threads_get_hash_addr(r_ha, _crypto_ctx, _buf_num)

    alu[r_ptext_size_m1, _ptext_size, -, 1]

    alu[r_gcm_esn_ena, CRYPTO_SA_SEQNUM_64_MASK, AND, _sa_ctlwrd, \
                                  >>CRYPTO_SA_SEQNUM_64_SHIFT]
    alu[r_tmp, 7, AND, _cfg_modes_select, >>18]
    .if (r_tmp == 7)
        alu[r_gcm_esn_ena, r_gcm_esn_ena, OR, 2]
    .endif

    // Load params to xfr regs using local macro instead of the one in
    // the crypto lib, because nfas is having register allocation issues
    crypto_threads_ld_parms($params,  \
                            _sram_ptext, _sram_ctext, \
                            r_ptext_size_m1, _sram_seq_ua, \
                            _sram_iv_spi, \
                            _hash_auth_len, \
                            _cfg_modes_select, \
                            r_ck, r_hk, r_ha)

    // switching on context needed because of crypto lib macros setup

    jump [_crypto_ctx, ctdjt#], targets [jd0#, jd1#, jd2#, jd3#, jd4#, jd5#]
    ctdjt#:
        jd0#: br[ctd0#]
        jd1#: br[ctd1#]
        jd2#: br[ctd2#]
        jd3#: br[ctd3#]
        jd4#: br[ctd4#]
        jd5#: br[ctd5#]

    ctd0#:
        crypto_threads_decrypt_s ( $params, _runcmd, 0, \
                 _sram_vars, r_gcm_esn_ena, crypto_threads_decrypt_done#)
    ctd1#:
        crypto_threads_decrypt_s ( $params, _runcmd, 1, \
                 _sram_vars, r_gcm_esn_ena, crypto_threads_decrypt_done#)
    ctd2#:
        crypto_threads_decrypt_s ( $params, _runcmd, 2, \
                 _sram_vars, r_gcm_esn_ena, crypto_threads_decrypt_done#)
    ctd3#:
        crypto_threads_decrypt_s ( $params, _runcmd, 3, \
                 _sram_vars, r_gcm_esn_ena, crypto_threads_decrypt_done#)
    ctd4#:
        crypto_threads_decrypt_s ( $params, _runcmd, 4, \
                 _sram_vars, r_gcm_esn_ena, crypto_threads_decrypt_done#)
    ctd5#:
        crypto_threads_decrypt_s ( $params, _runcmd, 5, \
                 _sram_vars, r_gcm_esn_ena, crypto_threads_decrypt_done#)

    crypto_threads_decrypt_done#:
.end
#endm


#if ( ENABLE_JUMBO_PKTS == 1 )

/**
 *  Simplification macro for use in crypto_threads_encrypt_strt, below
 */
#macro crypto_threads_encrypt_strt_s ( _params, _runcmd, _crypto_ctx, \
                                   _sram_vars, _gcm_ena, _BR_TARGET)
    #if ( CRYPTO_MIN_CTX <= _crypto_ctx  && CRYPTO_MAX_CTX >= _crypto_ctx )
        .if (_gcm_ena ) // AES GCM ESP
            #if ENABLE_CRYPTO_STATE_SAVE_RESTORE == 1
            #define_eval _crypto_transfer_length 4
            crypto_load_library_sequence (_params, ipsec_enc_aesgcm_strt_save, \
                                         _crypto_ctx, _sram_vars)
            crypto_run_library_sequence (_runcmd, _crypto_ctx, \
                                         ipsec_enc_aesgcm_strt_save, \
                                         _crypto_ctx, _sram_vars, 0, *nosend*)
            #else
            #define_eval _crypto_transfer_length 4
            crypto_load_library_sequence (_params, ipsec_enc_aesgcm_strt, \
                                         _crypto_ctx, _sram_vars)
            crypto_run_library_sequence (_runcmd, _crypto_ctx, \
                                         ipsec_enc_aesgcm_strt, \
                                         _crypto_ctx, _sram_vars, 0, *nosend*)
            #endif
        .else
            #define_eval _crypto_transfer_length 4
            crypto_load_library_sequence (_params, ipsec_enc_strt, \
                                          _crypto_ctx, _sram_vars)
            crypto_run_library_sequence (_runcmd, _crypto_ctx, \
                                           ipsec_enc_strt, \
                                          _crypto_ctx, _sram_vars, 0, *nosend*)
        .endif
    #endif
    br[_BR_TARGET]
#endm


/**
 *  Load Encrypt Sequence params to sram, and Generate Encrypt Sequence cmd
 *  in xfr regs for start of jumbo packet
 *
 *  @param  _runcmd        XFR, transfer register array (2 lwords) returned with
 *                              encrypt sequence cmd
 *  @param  _buf_num       GPR, buffer selection, one of 0,1,2,3
 *  @param  _crypto_ctx    GPR, crypto context ( 0 through 5 )
 *  @param  _sa_ctlwrd     XFR, sa control word
 *  @param  _sram_vars     GPR, crypto sram address of sequence variables
 *  @param  _sram_ptext    GPR, crypto sram address of plain(in)text start
 *  @param  _sram_ctext    GPR, crypto sram address of cipher(out)text start
 *  @param  _ptext_size    GPR, length of plaintext, in bytes - 1
 *  @param  _sram_iv_spi   GPR, 15:0  sram addr of spi / aesgcm aad
 *                              31:16 sram addr of iv or aesgcm iv constructed
 *  @param  _sram_seq_ua   GPR, crypto sram address of seq number 63:32
 *                              or sram address of aesgcm len(A)/Len(C) field
 *  @param  _hash_auth_len GPR, 15:0  length of auth header data, in bytes - 1,
 *                              31:16 icv/hash length - 1 byte or sram addr of
 *                                     aesgcm zeros
 *  @param  _cfg_modes_sel GPR, 15:0  cfg_pulse_R and cfg_sel_R fields for
 *                                    Crypt_Setup_Regs_0
 *                              31:16 cfg_Crypt0_R field for Crypt_Setup_Regs_0
 */
#macro crypto_threads_encrypt_strt (_runcmd, _buf_num, _crypto_ctx, _sa_ctlwrd, \
                                    _sram_vars, _sram_ptext, _sram_ctext, \
                                    _ptext_size, _sram_iv_spi, _sram_seq_ua, \
                                    _hash_auth_len, _cfg_modes_select )
.begin

    .reg $params[8]
    .xfer_order $params
    .reg r_tmp, r_gcm_ena, r_ptext_size_m1
    .reg r_ck, r_hk, r_ha
    .sig s

    crypto_threads_get_sa_addr(r_ck, _crypto_ctx, _buf_num)
    alu[r_ck, r_ck, +, CRYPTO_SA_DMA_KEYS_OFFSET]
    crypto_threads_get_hash_key_addr(r_hk, _crypto_ctx, _buf_num)
    crypto_threads_get_hash_addr(r_ha, _crypto_ctx, _buf_num)

    alu[r_ptext_size_m1, _ptext_size, -, 1]

    alu[r_tmp, 7, AND, _cfg_modes_select, >>18]
    .if (r_tmp == 7)
        alu[r_gcm_ena, --, B, 1]
    .else
        alu[r_gcm_ena, --, B, 0]
    .endif

    // Load params to xfr regs using local macro instead of the one in
    // the crypto lib, because nfas is having register allocation issues
    crypto_threads_ld_parms($params,  \
                            _sram_ptext, _sram_ctext, \
                            r_ptext_size_m1, _sram_seq_ua, \
                            _sram_iv_spi, \
                            _hash_auth_len, \
                            _cfg_modes_select, \
                            r_ck, r_hk, r_ha)


    // switching on context needed because of crypto lib macros setup

    jump [_crypto_ctx, ctesjt#], targets [jes0#, jes1#, jes2#, jes3#, jes4#, jes5#]
    ctesjt#:
        jes0#: br[ctes0#]
        jes1#: br[ctes1#]
        jes2#: br[ctes2#]
        jes3#: br[ctes3#]
        jes4#: br[ctes4#]
        jes5#: br[ctes5#]

    ctes0#:
        crypto_threads_encrypt_strt_s ( $params, _runcmd, 0, \
                 _sram_vars, r_gcm_ena, crypto_threads_encrypt_strt_done#)
    ctes1#:
        crypto_threads_encrypt_strt_s ( $params, _runcmd, 1, \
                 _sram_vars, r_gcm_ena, crypto_threads_encrypt_strt_done#)
    ctes2#:
        crypto_threads_encrypt_strt_s ( $params, _runcmd, 2, \
                 _sram_vars, r_gcm_ena, crypto_threads_encrypt_strt_done#)
    ctes3#:
        crypto_threads_encrypt_strt_s ( $params, _runcmd, 3, \
                 _sram_vars, r_gcm_ena, crypto_threads_encrypt_strt_done#)
    ctes4#:
        crypto_threads_encrypt_strt_s ( $params, _runcmd, 4, \
                 _sram_vars, r_gcm_ena, crypto_threads_encrypt_strt_done#)
    ctes5#:
        crypto_threads_encrypt_strt_s ( $params, _runcmd, 5, \
                 _sram_vars, r_gcm_ena, crypto_threads_encrypt_strt_done#)

    crypto_threads_encrypt_strt_done#:
.end
#endm


/**
 *  Simplification macro for use in crypto_threads_encrypt_cont, below
 */
#macro crypto_threads_encrypt_cont_s ( _params, _runcmd, _crypto_ctx, \
                                       _sram_vars, _BR_TARGET)
    #if ( CRYPTO_MIN_CTX <= _crypto_ctx  && CRYPTO_MAX_CTX >= _crypto_ctx )
        #define_eval _crypto_transfer_length 1
        crypto_load_library_sequence (_params, ipsec_enc_cont, \
                                         _crypto_ctx, _sram_vars)
        crypto_run_library_sequence (_runcmd, _crypto_ctx, \
               ipsec_enc_cont, _crypto_ctx, _sram_vars, 0, *nosend*)
    #endif
    br[_BR_TARGET]
#endm


/**
 *  Load Encrypt Sequence params to sram, and Generate Encrypt Sequence cmd
 *  in xfr regs, for continuation of jumbo packet
 *
 *  @param  _runcmd      XFR, transfer register array (2 lwords) returned with
 *                            encrypt sequence cmd
 *  @param  _crypto_ctx  GPR, crypto context ( 0 through 5 )
 *  @param  _sram_vars   GPR, crypto sram address of sequence variables
 *  @param  _sram_ptext  GPR, crypto sram address of plaintext start
 *  @param  _sram_ctext  GPR, crypto sram address of ciphertext start
 *  @param  _ptext_size  GPR, length of plaintext, in bytes - 1
 */
#macro crypto_threads_encrypt_cont (_runcmd, _crypto_ctx, _sram_vars, \
                                    _sram_ptext, _sram_ctext, _ptext_size)
.begin

    .reg $params[4]
    .xfer_order $params

    .reg r_ptext_size_m1

    alu[r_ptext_size_m1, _ptext_size, -, 1]

    crypto_load_ipsec_enc_cont($params, _crypto_ctx, \
                      _sram_ptext, _sram_ctext, r_ptext_size_m1)

    // switching on context needed because of crypto lib macros setup

    jump [_crypto_ctx, ctecjt#], targets [jec0#, jec1#, jec2#, jec3#, jec4#, jec5#]
    ctecjt#:
        jec0#: br[ctec0#]
        jec1#: br[ctec1#]
        jec2#: br[ctec2#]
        jec3#: br[ctec3#]
        jec4#: br[ctec4#]
        jec5#: br[ctec5#]

    ctec0#:
        crypto_threads_encrypt_cont_s ( $params, _runcmd, 0, \
                 _sram_vars, crypto_threads_encrypt_cont_done#)
    ctec1#:
        crypto_threads_encrypt_cont_s ( $params, _runcmd, 1, \
                 _sram_vars, crypto_threads_encrypt_cont_done#)
    ctec2#:
        crypto_threads_encrypt_cont_s ( $params, _runcmd, 2, \
                 _sram_vars, crypto_threads_encrypt_cont_done#)
    ctec3#:
        crypto_threads_encrypt_cont_s ( $params, _runcmd, 3, \
                 _sram_vars, crypto_threads_encrypt_cont_done#)
    ctec4#:
        crypto_threads_encrypt_cont_s ( $params, _runcmd, 4, \
                 _sram_vars, crypto_threads_encrypt_cont_done#)
    ctec5#:
        crypto_threads_encrypt_cont_s ( $params, _runcmd, 5, \
                 _sram_vars, crypto_threads_encrypt_cont_done#)

    crypto_threads_encrypt_cont_done#:
.end
#endm


/**
 *  Simplification macro for use in crypto_threads_encrypt_end, below
 */
#macro crypto_threads_encrypt_end_s ( _params, _runcmd, _crypto_ctx, \
                                      _sram_vars, _gcm_esn_ena, _BR_TARGET)
    #if ( CRYPTO_MIN_CTX <= _crypto_ctx  && CRYPTO_MAX_CTX >= _crypto_ctx )
        .if (_gcm_esn_ena & 2) // AES GCM ESP
             #define_eval _crypto_transfer_length 2
             #if ENABLE_CRYPTO_STATE_SAVE_RESTORE == 1
             crypto_load_library_sequence (_params, ipsec_enc_aesgcm_end_restore, \
                                            _crypto_ctx, _sram_vars)
             crypto_run_library_sequence (_runcmd, _crypto_ctx, \
                 ipsec_enc_aesgcm_end_restore, _crypto_ctx, _sram_vars, 0, *nosend*)
             #else
             crypto_load_library_sequence (_params, ipsec_enc_aesgcm_end, \
                                            _crypto_ctx, _sram_vars)
             crypto_run_library_sequence (_runcmd, _crypto_ctx, \
                 ipsec_enc_aesgcm_end, _crypto_ctx, _sram_vars, 0, *nosend*)
             #endif
        .else
            .if (_gcm_esn_ena & 1) // ESN
                 #define_eval _crypto_transfer_length 2
                 crypto_load_library_sequence (_params, ipsec_enc_end_esn, \
                                               _crypto_ctx, _sram_vars)
                 crypto_run_library_sequence (_runcmd, _crypto_ctx, \
                     ipsec_enc_end_esn, _crypto_ctx, _sram_vars, 0, *nosend*)
            .else
                 #define_eval _crypto_transfer_length 2
                 crypto_load_library_sequence (_params, ipsec_enc_end, \
                                               _crypto_ctx, _sram_vars)
                 crypto_run_library_sequence (_runcmd, _crypto_ctx, \
                     ipsec_enc_end, _crypto_ctx, _sram_vars, 0, *nosend*)
            .endif
        .endif
    #endif
    br[_BR_TARGET]
#endm


/**
 *  Load Encrypt Sequence params to sram, and Generate Encrypt Sequence cmd
 *  in xfr regs, for end of jumbo packet
 *
 *  @param  _runcmd        XFR, transfer register array (2 lwords) returned with
 *                              encrypt sequence cmd
 *  @param  _crypto_ctx    GPR, crypto context ( 0 through 5 )
 *  @param  _sa_ctlwrd     XFR, sa control word
 *  @param  _sram_vars     GPR, crypto sram address of sequence variables
 *  @param  _sram_ptext    GPR, crypto sram address of plaintext start
 *  @param  _sram_ctext    GPR, crypto sram address of ciphertext start
 *  @param  _ptext_size    GPR, length of plaintext, in bytes - 1
 *  @param  _sram_seq_ua   GPR, crypto sram address of seq number 63:32, or
 *                              crypto sram address of aesgcm len(a)/(c) vector
 *  @param  _hkey_addr     GPR, sram address of hash key, unused in normal aesgcm,
 *                              used to calc addr of cipher key and cntr value
 *                              for aesgcm with save/restore
 *  @param  _hkey_len      GPR, hash/authentication key length, unused for aesgcm
 *  @param  _hres_addr     GPR, sram address of hash result
 *  @param  _cfg_sel_modes GPR, 31:16 cfg_pulse_R and cfg_sel_R fields for
 *                                    Crypt_Setup_Regs_0
 *                              15:0  cfg_Crypt0_R field for Crypt_Setup_Regs_0
 */
#macro crypto_threads_encrypt_end (_runcmd, _crypto_ctx, _sa_ctlwrd, _sram_vars, \
                                   _sram_ptext, _sram_ctext, _ptext_size, \
                                   _sram_seq_ua, _hkey_addr, _hkey_len, \
                                   _hres_addr, _cfg_modes_select )
.begin

    .reg $params[4]
    .xfer_order $params
    .reg r_gcm_esn_ena, r_ptext_size_m1

    alu[r_ptext_size_m1, _ptext_size, -, 1]

    alu[r_gcm_esn_ena, CRYPTO_SA_SEQNUM_64_MASK, AND, _sa_ctlwrd, \
                                  >>CRYPTO_SA_SEQNUM_64_SHIFT]
    alu[r_tmp, 7, AND, _cfg_modes_select, >>18]
    .if (r_tmp == 7)
        alu[r_gcm_esn_ena, r_gcm_esn_ena, OR, 2]
    .endif

    #if ENABLE_CRYPTO_STATE_SAVE_RESTORE == 0

        // common format used for all ipsec_enc_end variants
        crypto_load_ipsec_enc_end($params, _crypto_ctx, \
                    _sram_ptext, _sram_ctext, r_ptext_size_m1, \
                    _sram_seq_ua, _hkey_addr, _hkey_len, _hres_addr)

    #else

        .reg r_ck, r_ctr_prev, r_hres_prev, r_t1, r_t2

        .if (r_gcm_esn_ena & 2) // aes gcm esp

            // restore crypto state

            // r_hres_prev points to hash result from previous buffer
            // of this crypto engine
            immed[r_t1, 0x0600]
            alu[r_t2, r_t1, AND, _hres_addr]
            immed[r_t1, 0x0100]
            alu[r_t2, r_t2, -, r_t1]
            immed[r_t1, 0x0600]
            alu[r_t2, r_t2, AND, r_t1]
            immed[r_t1, 0xf9ff]
            alu[r_hres_prev, _hres_addr, AND, r_t1]
            alu[r_hres_prev, r_hres_prev, OR, r_t2]
            // cipher key is 0x20 before hash key
            alu[r_ck, _hkey_addr, -, 0x20]
            // fake constructed iv with counter value from previous buffer
            alu[r_ctr_prev, _hkey_addr, +, 0x40]

            crypto_load_ipsec_enc_aesgcm_end_restore($params, _crypto_ctx, \
                          _sram_ptext, r_ptext_size_m1, \
                          _lenac_addr, _cfg_modes, \
                          r_ctr_prev, r_ck, r_hres_prev, _hres_addr)
        .else

            // common format used for all ipsec_enc_end variants
            crypto_load_ipsec_enc_end($params, _crypto_ctx, \
                    _sram_ptext, _sram_ctext, r_ptext_size_m1, \
                    _sram_seq_ua, _hkey_addr, _hkey_len, _hres_addr)

        .endif
    #endif

    // switching on context needed because of crypto lib macros setup

    jump [_crypto_ctx, cteejt#], targets [jee0#, jee1#, jee2#, jee3#, jee4#, jee5#]
    cteejt#:
        jee0#: br[ctee0#]
        jee1#: br[ctee1#]
        jee2#: br[ctee2#]
        jee3#: br[ctee3#]
        jee4#: br[ctee4#]
        jee5#: br[ctee5#]

    ctee0#:
        crypto_threads_encrypt_end_s ( $params, _runcmd, 0, \
                 _sram_vars, r_gcm_esn_ena, crypto_threads_encrypt_end_done#)
    ctee1#:
        crypto_threads_encrypt_end_s ( $params, _runcmd, 1, \
                 _sram_vars, r_gcm_esn_ena, crypto_threads_encrypt_end_done#)
    ctee2#:
        crypto_threads_encrypt_end_s ( $params, _runcmd, 2, \
                 _sram_vars, r_gcm_esn_ena, crypto_threads_encrypt_end_done#)
    ctee3#:
        crypto_threads_encrypt_end_s ( $params, _runcmd, 3, \
                 _sram_vars, r_gcm_esn_ena, crypto_threads_encrypt_end_done#)
    ctee4#:
        crypto_threads_encrypt_end_s ( $params, _runcmd, 4, \
                 _sram_vars, r_gcm_esn_ena, crypto_threads_encrypt_end_done#)
    ctee5#:
        crypto_threads_encrypt_end_s ( $params, _runcmd, 5, \
                 _sram_vars, r_gcm_esn_ena, crypto_threads_encrypt_end_done#)

    crypto_threads_encrypt_end_done#:
.end
#endm


/**
 *  Simplification macro for use in crypto_threads_decrypt_strt, below
 */
#macro crypto_threads_decrypt_strt_s ( _params, _runcmd, _crypto_ctx, \
                                   _sram_vars, _gcm_ena, _NO_WAIT, _BR_TARGET)
    #if ( CRYPTO_MIN_CTX <= _crypto_ctx  && CRYPTO_MAX_CTX >= _crypto_ctx )
        .if (_gcm_ena ) // AES GCM ESP
            #define_eval _crypto_transfer_length 4
            crypto_load_library_sequence (_params, ipsec_dec_aesgcm_strt, \
                                         _crypto_ctx, _sram_vars)
            crypto_run_library_sequence (_runcmd, _crypto_ctx, \
                                         ipsec_dec_aesgcm_strt, \
                                         _crypto_ctx, _sram_vars, 0, *nosend*)
        .else
            #if (_NO_WAIT == 1)
                #define_eval _crypto_transfer_length 4
                crypto_load_library_sequence (_params, ipsec_dec_strt_nw, \
                                          _crypto_ctx, _sram_vars)
                crypto_run_library_sequence (_runcmd, _crypto_ctx, \
                                           ipsec_dec_strt_nw, \
                                          _crypto_ctx, _sram_vars, 0, *nosend*)
            #else
                #define_eval _crypto_transfer_length 4
                crypto_load_library_sequence (_params, ipsec_dec_strt, \
                                          _crypto_ctx, _sram_vars)
                crypto_run_library_sequence (_runcmd, _crypto_ctx, \
                                           ipsec_dec_strt, \
                                          _crypto_ctx, _sram_vars, 0, *nosend*)
            #endif
        .endif
    #endif
    br[_BR_TARGET]
#endm


/**
 *  Load Decrypt Sequence params to sram, and Generate Decrypt Sequence cmd
 *  in xfr regs for start of jumbo packet
 *
 *  @param  _runcmd     XFR, transfer register array (2 lwords) returned with
 *                            encrypt sequence cmd
 *  @param  _buf_num     GPR, buffer selection, one of 0,1,2,3
 *  @param  _crypto_ctx  GPR, crypto context ( 0 through 5 )
 *  @param  _sa_ctlwrd   XFR, sa control word
 *  @param  _sram_vars   GPR, crypto sram address of sequence variables
 *  @param  _sram_ptext  GPR, crypto sram address of cipher(in)text start
 *  @param  _sram_ctext  GPR, crypto sram address of plain(out)text start
 *  @param  _ptext_size  GPR, length of plaintext, in bytes - 1
 *  @param  _sram_iv_spi   GPR, 15:0  sram addr of spi / aesgcm aad
 *                              31:16 sram addr of iv or aesgcm iv constructed
 *  @param  _sram_seq_ua   GPR, crypto sram address of seq number 63:32
 *                              or sram address of aesgcm len(A)/Len(C) field
 *  @param  _hash_auth_len GPR, 15:0  length of auth header data, in bytes - 1,
 *                              31:16 icv/hash length - 1 byte or sram addr of
 *                                     aesgcm zeros
 *  @param  _cfg_modes_sel GPR, 15:0  cfg_pulse_R and cfg_sel_R fields for
 *                                    Crypt_Setup_Regs_0
 *                              31:16 cfg_Crypt0_R field for Crypt_Setup_Regs_0
 *  @param  _NO_WAIT       CNST If 1, invokes the 'no wait' start sequence
 */
#macro crypto_threads_decrypt_strt (_runcmd, _buf_num, _crypto_ctx, _sa_ctlwrd, \
                                    _sram_vars, _sram_ptext, _sram_ctext, \
                                    _ptext_size, _sram_iv_spi, _sram_seq_ua, \
                                    _hash_auth_len, _cfg_modes_select, _NO_WAIT )
.begin

    .reg $params[8]
    .xfer_order $params
    .reg r_tmp, r_gcm_ena, r_ptext_size_m1
    .reg r_ck, r_hk, r_ha
    .sig s

    crypto_threads_get_sa_addr(r_ck, _crypto_ctx, _buf_num)
    alu[r_ck, r_ck, +, CRYPTO_SA_DMA_KEYS_OFFSET]
    crypto_threads_get_hash_key_addr(r_hk, _crypto_ctx, _buf_num)
    crypto_threads_get_hash_addr(r_ha, _crypto_ctx, _buf_num)

    alu[r_ptext_size_m1, _ptext_size, -, 1]

    alu[r_tmp, 7, AND, _cfg_modes_select, >>18]
    .if (r_tmp == 7)
        alu[r_gcm_ena, --, B, 1]
    .else
        alu[r_gcm_ena, --, B, 0]
    .endif

    // Load params to xfr regs using local macro instead of the one in
    // the crypto lib, because nfas is having register allocation issues
    crypto_threads_ld_parms($params,  \
                            _sram_ptext, _sram_ctext, \
                            r_ptext_size_m1, _sram_seq_ua, \
                            _sram_iv_spi, \
                            _hash_auth_len, \
                            _cfg_modes_select, \
                            r_ck, r_hk, r_ha)


    // switching on context needed because of crypto lib macros setup

    jump [_crypto_ctx, ctdsjt#], targets [jds0#, jds1#, jds2#, jds3#, jds4#, jds5#]
    ctdsjt#:
        jds0#: br[ctds0#]
        jds1#: br[ctds1#]
        jds2#: br[ctds2#]
        jds3#: br[ctds3#]
        jds4#: br[ctds4#]
        jds5#: br[ctds5#]

    ctds0#:
        crypto_threads_decrypt_strt_s ( $params, _runcmd, 0, \
          _sram_vars, r_gcm_ena, _NO_WAIT, crypto_threads_decrypt_strt_done#)
    ctds1#:
        crypto_threads_decrypt_strt_s ( $params, _runcmd, 1, \
          _sram_vars, r_gcm_ena, _NO_WAIT, crypto_threads_decrypt_strt_done#)
    ctds2#:
        crypto_threads_decrypt_strt_s ( $params, _runcmd, 2, \
          _sram_vars, r_gcm_ena, _NO_WAIT, crypto_threads_decrypt_strt_done#)
    ctds3#:
        crypto_threads_decrypt_strt_s ( $params, _runcmd, 3, \
          _sram_vars, r_gcm_ena, _NO_WAIT, crypto_threads_decrypt_strt_done#)
    ctds4#:
        crypto_threads_decrypt_strt_s ( $params, _runcmd, 4, \
          _sram_vars, r_gcm_ena, _NO_WAIT, crypto_threads_decrypt_strt_done#)
    ctds5#:
        crypto_threads_decrypt_strt_s ( $params, _runcmd, 5, \
          _sram_vars, r_gcm_ena, _NO_WAIT, crypto_threads_decrypt_strt_done#)

    crypto_threads_decrypt_strt_done#:
.end
#endm


/**
 *  Simplification macro for use in crypto_threads_decrypt_cont, below
 */
#macro crypto_threads_decrypt_cont_s ( _params, _runcmd, _crypto_ctx, \
                                       _sram_vars, _NO_WAIT, _BR_TARGET)
    #if ( CRYPTO_MIN_CTX <= _crypto_ctx  && CRYPTO_MAX_CTX >= _crypto_ctx )
        #if (_NO_WAIT == 1)
            #define_eval _crypto_transfer_length 1
            crypto_load_library_sequence (_params, ipsec_dec_cont_nw, \
                                         _crypto_ctx, _sram_vars)
            crypto_run_library_sequence (_runcmd, _crypto_ctx, \
               ipsec_dec_cont_nw, _crypto_ctx, _sram_vars, 0, *nosend*)
        #else
            #define_eval _crypto_transfer_length 1
            crypto_load_library_sequence (_params, ipsec_dec_cont, \
                                         _crypto_ctx, _sram_vars)
            crypto_run_library_sequence (_runcmd, _crypto_ctx, \
               ipsec_dec_cont, _crypto_ctx, _sram_vars, 0, *nosend*)
        #endif
     #endif
     br[_BR_TARGET]
#endm


/**
 *  Load Decrypt Sequence params to sram, and Generate Decrypt Sequence cmd
 *  in xfr regs, for continuation of jumbo packet
 *
 *  @param  _runcmd      XFR, transfer register array (2 lwords) returned with
 *                            decrypt sequence cmd
 *  @param  _crypto_ctx  GPR, crypto context ( 0 through 5 )
 *  @param  _sram_vars   GPR, crypto sram address of sequence variables
 *  @param  _sram_ptext  GPR, crypto sram address of ciphertext start
 *  @param  _sram_ctext  GPR, crypto sram address of plaintext start
 *  @param  _ptext_size  GPR, length of plaintext, in bytes - 1
 *  @param  _NO_WAIT     CNST If 1, invokes the 'no wait' continue sequence
 */
#macro crypto_threads_decrypt_cont (_runcmd, _crypto_ctx, _sram_vars, \
                                    _sram_ptext, _sram_ctext, _ptext_size, \
                                    _NO_WAIT)
.begin

    .reg $params[4]
    .xfer_order $params

    .reg r_ptext_size_m1

    alu[r_ptext_size_m1, _ptext_size, -, 1]

    crypto_load_ipsec_enc_cont($params, _crypto_ctx, \
                      _sram_ptext, _sram_ctext, r_ptext_size_m1)

    // switching on context needed because of crypto lib macros setup

    jump [_crypto_ctx, ctdcjt#], targets [jdc0#, jdc1#, jdc2#, jdc3#, jdc4#, jdc5#]
    ctdcjt#:
        jdc0#: br[ctdc0#]
        jdc1#: br[ctdc1#]
        jdc2#: br[ctdc2#]
        jdc3#: br[ctdc3#]
        jdc4#: br[ctdc4#]
        jdc5#: br[ctdc5#]

    ctdc0#:
        crypto_threads_decrypt_cont_s ( $params, _runcmd, 0, \
           _sram_vars, _NO_WAIT, crypto_threads_decrypt_cont_done#)
    ctdc1#:
        crypto_threads_decrypt_cont_s ( $params, _runcmd, 1, \
           _sram_vars, _NO_WAIT, crypto_threads_decrypt_cont_done#)
    ctdc2#:
        crypto_threads_decrypt_cont_s ( $params, _runcmd, 2, \
           _sram_vars, _NO_WAIT, crypto_threads_decrypt_cont_done#)
    ctdc3#:
        crypto_threads_decrypt_cont_s ( $params, _runcmd, 3, \
           _sram_vars, _NO_WAIT, crypto_threads_decrypt_cont_done#)
    ctdc4#:
        crypto_threads_decrypt_cont_s ( $params, _runcmd, 4, \
           _sram_vars, _NO_WAIT, crypto_threads_decrypt_cont_done#)
    ctdc5#:
        crypto_threads_decrypt_cont_s ( $params, _runcmd, 5, \
          _sram_vars, _NO_WAIT, crypto_threads_decrypt_cont_done#)

    crypto_threads_decrypt_cont_done#:
.end
#endm


/**
 *  Simplification macro for use in crypto_threads_decrypt_end, below
 */
#macro crypto_threads_decrypt_end_s ( _params, _runcmd, _crypto_ctx, \
                                      _sram_vars, _gcm_esn_ena, _BR_TARGET)
    #if ( CRYPTO_MIN_CTX <= _crypto_ctx  && CRYPTO_MAX_CTX >= _crypto_ctx )
        .if (_gcm_esn_ena & 2) // AES GCM ES
             #define_eval _crypto_transfer_length 2
             crypto_load_library_sequence (_params, ipsec_dec_aesgcm_end, \
                                            _crypto_ctx, _sram_vars)
             crypto_run_library_sequence (_runcmd, _crypto_ctx, \
                 ipsec_dec_aesgcm_end, _crypto_ctx, _sram_vars, 0, *nosend*)
        .else
            .if (_gcm_esn_ena & 1) // ESN
                 #define_eval _crypto_transfer_length 2
                 crypto_load_library_sequence (_params, ipsec_dec_end_esn, \
                                               _crypto_ctx, _sram_vars)
                 crypto_run_library_sequence (_runcmd, _crypto_ctx, \
                     ipsec_dec_end_esn, _crypto_ctx, _sram_vars, 0, *nosend*)
            .else
                 #define_eval _crypto_transfer_length 2
                 crypto_load_library_sequence (_params, ipsec_dec_end, \
                                               _crypto_ctx, _sram_vars)
                 crypto_run_library_sequence (_runcmd, _crypto_ctx, \
                     ipsec_dec_end, _crypto_ctx, _sram_vars, 0, *nosend*)
            .endif
        .endif
    #endif
    br[_BR_TARGET]
#endm


/**
 *  Load Decrypt Sequence params to sram, and Generate Decrypt Sequence cmd
 *  in xfr regs, for end of jumbo packet
 *
 *  @param  _runcmd        XFR, transfer register array (2 lwords) returned with
 *                              decrypt sequence cmd
 *  @param  _crypto_ctx    GPR, crypto context ( 0 through 5 )
 *  @param  _sa_ctlwrd     XFR, sa control word
 *  @param  _sram_vars     GPR, crypto sram address of sequence variables
 *  @param  _sram_ptext    GPR, crypto sram address of ciphertext start
 *  @param  _sram_ctext    GPR, crypto sram address of plaintext start
 *  @param  _ptext_size    GPR, length of plaintext, in bytes - 1
 *  @param  _sram_seq_ua   GPR, crypto sram address of seq number 63:32, or
 *                              crypto sram address of aesgcm len(a)/(c) vector
 *  @param  _hkey_addr     GPR, sram address of hash key, unused in aesgcm
 *  @param  _hkey_len      GPR, hash/authentication key length, unused for aesgcm
 *  @param  _hres_addr     GPR, sram address of hash result
 *  @param  _cfg_modes_sel GPR, 15:0  cfg_pulse_R and cfg_sel_R fields for
 *                                    Crypt_Setup_Regs_0
 *                              31:16 cfg_Crypt0_R field for Crypt_Setup_Regs_0
 */
#macro crypto_threads_decrypt_end (_runcmd, _crypto_ctx, _sa_ctlwrd, _sram_vars, \
                                   _sram_ptext, _sram_ctext, _ptext_size, \
                                   _sram_seq_ua, _hkey_addr, _hkey_len, \
                                   _hres_addr, _cfg_modes_select )
.begin

    .reg $params[4]
    .xfer_order $params
    .reg r_gcm_esn_ena, r_ptext_size_m1

    alu[r_ptext_size_m1, _ptext_size, -, 1]

    alu[r_gcm_esn_ena, CRYPTO_SA_SEQNUM_64_MASK, AND, _sa_ctlwrd, \
                                      >>CRYPTO_SA_SEQNUM_64_SHIFT]
    alu[r_tmp, 7, AND, _cfg_modes_select, >>18]
    .if (r_tmp == 7)
        alu[r_gcm_esn_ena, r_gcm_esn_ena, OR, 2]
    .endif

    // common format used for all ipsec_dec_end variants

    crypto_load_ipsec_dec_end($params, _crypto_ctx, \
                _sram_ptext, _sram_ctext, r_ptext_size_m1, \
                _sram_seq_ua, _hkey_addr, _hkey_len, _hres_addr)

    // switching on context needed because of crypto lib macros setup

    jump [_crypto_ctx, ctdejt#], targets [jde0#, jde1#, jde2#, jde3#, jde4#, jde5#]
    ctdejt#:
        jde0#: br[ctde0#]
        jde1#: br[ctde1#]
        jde2#: br[ctde2#]
        jde3#: br[ctde3#]
        jde4#: br[ctde4#]
        jde5#: br[ctde5#]

    ctde0#:
        crypto_threads_decrypt_end_s ( $params, _runcmd, 0, \
                 _sram_vars, r_gcm_esn_ena, crypto_threads_decrypt_end_done#)
    ctde1#:
        crypto_threads_decrypt_end_s ( $params, _runcmd, 1, \
                 _sram_vars, r_gcm_esn_ena, crypto_threads_decrypt_end_done#)
    ctde2#:
        crypto_threads_decrypt_end_s ( $params, _runcmd, 2, \
                 _sram_vars, r_gcm_esn_ena, crypto_threads_decrypt_end_done#)
    ctde3#:
        crypto_threads_decrypt_end_s ( $params, _runcmd, 3, \
                 _sram_vars, r_gcm_esn_ena, crypto_threads_decrypt_end_done#)
    ctde4#:
        crypto_threads_decrypt_end_s ( $params, _runcmd, 4, \
                 _sram_vars, r_gcm_esn_ena, crypto_threads_decrypt_end_done#)
    ctde5#:
        crypto_threads_decrypt_end_s ( $params, _runcmd, 5, \
                 _sram_vars, r_gcm_esn_ena, crypto_threads_decrypt_end_done#)

    crypto_threads_decrypt_end_done#:
.end
#endm

#endif /* ENABLE_JUMBO_PKTS */


/**
 *  Re-enable 'ready' signal for buffer
 *
 *  @param  _bufr  GPR, buffer selection, one of 0-3
 */
#macro crypto_threads_reenable_buf(_buf)
.begin
    .reg r_temp
    alu[r_temp, _buf, OR, 0x8] // bufs using sigs 8-11
    alu[r_temp, r_me_ctx, OR, r_temp, <<3]
    local_csr_wr[SAME_ME_SIGNAL, r_temp]
.end
#endm


/**
 *  Helper macro to invoke send_status_response as a
 *  'subroutine' within the input thread. Note that
 *  crypto_threads_reenable_buf is also invoked because,
 *  whenever the input thread does send_response, it is
 *  because of an error and the buffer won't be passed
 *  along to the output thread, so we have to 'reuse' it
 */
#define SEND_RESPONSE_SUBROUTINE 1 // use subroutine here for send response
#macro crypto_threads_input_send_response(_ctx, _buf, _stat)
    #if ( SEND_RESPONSE_SUBROUTINE == 1 )
        move(r_stat, _stat )
        load_addr[r_rtn, cti_rtn#]
        br[send_response_sub#]
        cti_rtn#:
    #else
        crypto_threads_send_response( _ctx, _buf, _stat )
        crypto_threads_reenable_buf(_buf) // reuse this buffer
    #endif
#endm


/// @endcond


/**
 *  Crypto input thread
 *
 *  @param  _me_ctx       GPR, me context of this thread
 *  @param  _crypto_ctx   GPR, crypto core context to be used by this thread.
 *                        One context is used per core, so context will be equal
 *                        to the core # in use by this thread
 *  @param  _in_ring_type CONST, ring type. one of CRYTPO_RING_CTM,
 *                        CRYPTO_RING_IMU, CRYPTO_RING_EMU
 *  @param  _in_ring_num  CONST or GPR, ring number of input request ring.
 *
 *  Input thread dedicated to a single crypto context / core. Typically run on
 *  even #'d thread, i.e. me threads 0,2,4,6 while corresponding output thread
 *  is run on odd #'d threads, i.e. me threads 1,3,5,7
 *
 *  Utilizes 1/6 of crypto sram buffer; each crypto core/ctx is allocated 1/6 of
 *  the sram buffer. Within each 1/6 of the sram buffer, space is allocated for
 *  4x (2KB packet buffer, SA struct, and temp area)
 *
 *  The 4 packet buffers are used to create a pipeline to keep the crypto core
 *  active. The 4 buffers are in one of the following states:
 *  - data being dma'd from system memory into crypto sram buffer
 *  - ready to be operated on by crypto core when current crypto operation done
 *  - being operated on by crypto core
 *  - being dma'd from crypto sram to system memory
 *
 *  Source packet data may be stored contiguously in one buffer, or split into
 *  two or three buffers. One, two, or three dma transfers will be performed to
 *  move the buffers into crypt sram for encryption/decryption. The buffers
 *  will be placed in crypto sram contiguously, starting with data at Start of
 *  Packet Address, followed by Continuation of Packet Address, followed by End
 *  of Packet Address. If a length field is 0, no dma will occur for the
 *  corresponding buffer. ( addresses and length are part of the request ring
 *  entry, refer to request ring entry in code file for format)
 */
#macro crypto_threads_input(_me_ctx, _crypto_ctx, _in_ring_type, _in_ring_num )

    .begin

    //
    // When split into two buffers the intended use is as follows:
    // Start of Packet Address refers to a byte address in a CTM buffer, not in
    // a Crypto Island, where the start of a received packet is stored. End of
    // Packet Address refers to an 8 byte aligned address in Emu/Imu Memory,
    // which is the rest of the packet, following the portion in ctm. Note that
    // the ctm start/length must result in an 8 byte aligned ending address in
    // ctm if the end of packet length is non-zero. The start byte need not be
    // 8 byte aligned, and the dma will begin at the nearest 8 byte aligned
    // address in ctm which contains the starting byte; hence, extra bytes may
    // be read but will not be used. The 2 buffer split is likely to be used
    // for decryption operations. In the case of decrypt option 1, in
    // which only the decrypted plain text is written back out, the continue
    // address is used to pass the address of the start of the output packet,
    // and is passed through into the response ring entry.
    //
    // When split into three buffers, the intended use is as follows:
    // Start of Packet Address refers to a byte address in a Crypto Island
    // resident CTM buffer (although it does not have to) where the start of a
    // received packet is stored after having been read from it's original
    // location, modified to insert the various ipsec related fields to convert
    // a non-ipsec packet to an ipsec packet, and re-written into Crypto CTM.
    // Continuation of Packet Address refers to the portion of the original
    // received packet, in the non-Crypto Island CTM where it was received,
    // which follows the beginning of the packet which was modified. End of
    // Packet Address refers to an 8 byte aligned address in Emu/Imu Memory,
    // which is the rest of the packet, following the portion in ctm. Note that
    // the Start of Packet start/length must result in an 8 byte aligned ending
    // address in ctm if either Continue or End of Packet length is non-zero.
    // The start byte need not be 8 byte aligned, and the dma will begin at
    // the nearest 8 byte aligned address in ctm which contains the starting
    // byte; hence, extra bytes may be read but will not be used. Continuation
    // and End Addresses must be 8 byte aligned. The Continuation Length must be
    // an 8 byte multiple if End of Packet Length is non-zero. The 3 buffer
    // split is expected to be used for encryption operations.
    //
    // In all cases, the resulting output packet is written back at Packet Write
    // Back address, from the Request Ring entry, The intended use is that
    // Packet Write Back address is in the original CTM receive buffer, and may
    // be the original CTM packet starting address, offset by 1-7 bytes as
    // needed to align the portion of the packet written back to CTM such that
    // the starting address and length results in an 8 byte aligned ending
    // address in the CTM buffer. The balance of the packet, if any will be
    // written starting at End of Packet address which is typically in dram
    // memory. The actual addresses and byte counts will be returned in the
    // output response. In the case of Decrypt option 1, in which only the
    // decrypted plain text is written back out, the write back address
    // corresponds to the address of the start of the plain text data, rather
    // than the start of the packet.
    //
    // Thread processing is as follows:
    //  1) wait for next buffer section ( one of 4 ) to be available
    //  2) read next request from input ring
    //  3) read in Security Association data from memory to crypto sram
    //  4) read in packet data to be encrypted or decrypted to crypto sram
    //  5) generate vars for CIB based crypto sequence
    //  6) queue execution of crypto sequence
    //  7) go back to step 1
    //
    // For jumbo packets > 2KB, processing is altered because the crypto hardware
    // can only process 2KB at a time, and the sram packet buffer allocation
    // is only 2KB. Therefore, processing is sequenced so that the 1st 2KB
    // of packet data is loaded and processed using buffer n, the next up to
    // 2KB using buffer n+1, etc. Processing actions specific to the start
    // of the packet are carried out only on the 1st part of the packet,
    // and actions specific to the last part of the packet ( i.e. hash
    // transfer and checking ) are carried out only on the last part of the
    // packet.

    .sig s
    .reg r_buf, r_sram_sa, r_sram_vars, r_sram_pkt
    .reg r_pkt_end_addr, r_pkt_wrbk_addr
    .reg r_pkt_strt_addr_hi, r_pkt_cont_addr_hi, r_pkt_end_addr_hi
    .reg r_pkt_strt_len, r_pkt_cont_len, r_pkt_end_len
    .reg r_pkt_wrbk_len, r_pkt_wrbk_end_len
    .reg r_tmp, r_tmp1, r_tmp2
    .reg r_test_flag
    .reg r_auth_data_addr

    .reg r_sa_ctlwrd, r_dir
    .reg r_sram_ptext, r_sram_ctext, r_ptext_size
    .reg r_sram_spi, r_sram_iv, r_sram_iv_spi
    .reg r_cfg_select, r_cfg_modes, r_cfg_modes_select
    .reg r_auth_len, r_cfg_hlen, r_hash_auth_len
    .reg r_sram_seq_ua
    .reg r_hres, r_hkey
    .reg r_cipher, r_cimode, r_hash
    .reg r_iv_len, r_icv_len

    #if ( SEND_RESPONSE_SUBROUTINE == 1 )
    .reg r_stat, r_rtn
    #endif

    .reg $req[CRYPTO_REQ_LENGTH]
    .xfer_order $req


    .sig volatile buf_sig_A, buf_sig_B, buf_sig_C, buf_sig_D
    .addr buf_sig_A 8
    .addr buf_sig_B 9
    .addr buf_sig_C 10
    .addr buf_sig_D 11

    .set_sig buf_sig_A
    .set_sig buf_sig_B
    .set_sig buf_sig_C
    .set_sig buf_sig_D


    crypto_threads_update_counter(CRYPTO_CNTR_INPUT_THREAD_FLAG)

    // init internal vars
    move(r_test_flag, 0)
    move(r_buf, 0)

    // init sa addr store in lm because read it before it is written
    // same for jumbo state and writeback remaining length
    move(r_tmp1, 0xffffffff)
    .repeat
        crypto_threads_get_lm_temp_addr(r_tmp, _me_ctx, r_buf)
        localmem_write1(r_tmp1, r_tmp, CRYPTO_LM_SA_ADDR)
        #if ( ENABLE_JUMBO_PKTS == 1 )
        localmem_write1(CRYPTO_LM_JUMBO_STATE_NON_JUMBO, \
                        r_tmp, CRYPTO_LM_JUMBO_STATE)
        localmem_write1(0, \
                        r_tmp, CRYPTO_LM_JUMBO_WRBK_REM_LEN)
        .if ( r_buf == 0 )
            localmem_write1(CRYPTO_LM_JUMBO_STATE_NON_JUMBO, \
                            r_tmp, CRYPTO_LM_COMM_JUMBO_STATE)
            localmem_write1(0, \
                            r_tmp, CRYPTO_LM_COMM_JUMBO_WRBK_REM_LEN)
        .endif
        #endif
        alu[r_buf, r_buf, +, 1]
    .until ( r_buf > 3 )


    // set all buffer ready signals to start
    alu[r_tmp, r_me_ctx, OR, (&buf_sig_A<<3)]
    local_csr_wr[SAME_ME_SIGNAL, r_tmp]
    alu[r_tmp, r_me_ctx, OR, (&buf_sig_B<<3)]
    local_csr_wr[SAME_ME_SIGNAL, r_tmp]
    alu[r_tmp, r_me_ctx, OR, (&buf_sig_C<<3)]
    local_csr_wr[SAME_ME_SIGNAL, r_tmp]
    alu[r_tmp, r_me_ctx, OR, (&buf_sig_D<<3)]
    local_csr_wr[SAME_ME_SIGNAL, r_tmp]


    // start with buffer A
    immed[r_buf, 0]



    //
    // input processing loop
    //

    crypto_input_thread_loop#:


    // wait for next buffer to be available. sigs 8,9,10,11, for buf_sig_A,B,C,D
    // are set when output thread is done with respective buffer
    move(r_tmp, 0x100)
    alu[--, r_buf, OR, 0]
    alu[r_tmp, --, B, r_tmp, <<indirect]
    ctx_arb[--], defer[1]
    local_csr_wr[ACTIVE_CTX_WAKEUP_EVENTS, r_tmp]



    //
    // handling when in midst of a jumbo packet
    //
    #if ( ENABLE_JUMBO_PKTS == 1 )
    crypto_threads_get_lm_temp_addr(r_tmp, _me_ctx, 0)
    localmem_read1(r_tmp1, r_tmp, CRYPTO_LM_COMM_JUMBO_STATE)
    .if ( r_tmp1 == CRYPTO_LM_JUMBO_STATE_FIRST_BUF \
           || r_tmp1 == CRYPTO_LM_JUMBO_STATE_CONTINUE_BUF )
        // last buffer was 1st or continuation part of jumbo. this
        // buffer will be either a continuation, if the packet goes beyond
        // another 2KB, or the end, if the packet end is within next 2KB
        // skip reading request ring
        br[crypto_input_thread_gen_jumbo_params#]
    .endif
    #endif



    //
    // read the request ring
    //

    crypto_threads_input_read_req_ring#:

    crypto_threads_update_counter(CRYPTO_CNTR_CMD_RING_READ)

    #if ( _in_ring_type == CRYPTO_RING_WQ )

        // get entry off work queue, stalls until entry ready

        immed[r_tmp, ((crypto_req_ring >>24) & 0xFF00), <<16]

        immed[r_tmp1, (CRYPTO_REQ_LENGTH-1)]   // override zero indexed
        alu[--, 0x80, OR, r_tmp1, <<8]
        mem[qadd_thread, $req[0], r_tmp, <<8, _in_ring_num, MAX_15], \
           indirect_ref, ctx_swap[s]

        // bump free entries counter by PP cmd size
        alu[$req[0], --, B, 8]
        move(r_tmp, ipsec_pp_ring_words_free_lo_32)
        move(r_tmp1, (ipsec_pp_ring_words_free_hi_8 <<24))
        mem[add, $req[0], r_tmp1, <<8,  r_tmp, 1], ctx_swap[s]

    #elif ( _in_ring_type == CRYPTO_RING_EMU0 \
            || _in_ring_type == CRYPTO_RING_EMU1 \
            || _in_ring_type == CRYPTO_RING_EMU2 )


        // this code polls request ring continuously. could add mechanism,
        // potentially using a work queue, to stop polling request ring when
        // found empty for n polls in a row, such as n == 3, in order to reduce
        // cpp bus accesses.


        #pragma warning(disable:5008,disable:5004)
        ru_emem_ring_get(crypto_req_ring, $req[0], _in_ring_num, CRYPTO_REQ_LENGTH, s)
        #pragma warning(default:5008,default:5004)
        br_signal[s[1],crypto_threads_input_req_ring_empty#] // not enough words in ring
        br_bset[CRYPTO_REQ_ENTRY_VALID, CRYPTO_REQ_ENTRY_VALID_BIT, \
                         crypto_threads_input_req_ring_not_empty#] // valid cmd

        // increment empty ring seen counter while a test is running
        crypto_threads_input_req_ring_empty#:
        .if ( r_test_flag )
            crypto_threads_update_counter(CRYPTO_CNTR_CMD_RING_EMPTY)
        .endif
        br[crypto_threads_input_read_req_ring#]

    #elif ( _in_ring_type == CRYPTO_RING_CTM )

        #pragma warning(disable:5008)
        alu[r_tmp1, --, B, _in_ring_num, <<2]
        immed[r_tmp2, (CRYPTO_REQ_LENGTH - 1)]
        alu[--, 0x80, or, r_tmp2, <<8]
        ct[ring_get, $req[0], 0, r_tmp1, MAX_15], indirect_ref, sig_done[s]
        ctx_arb[s]
        #pragma warning(default:5008)
        br_bset[CRYPTO_REQ_ENTRY_VALID, CRYPTO_REQ_ENTRY_VALID_BIT, \
                         crypto_threads_input_req_ring_not_empty#] // valid cmd
        // increment empty ring seen counter while a test is running
        .if ( r_test_flag )
            crypto_threads_update_counter(CRYPTO_CNTR_CMD_RING_EMPTY)
        .endif
        br[crypto_threads_input_read_req_ring#]

    #else
        #error "crypto_threads_input: invalid input ring type."
    #endif


    crypto_threads_input_req_ring_not_empty#:

    aggregate_directive(.set, $req, 13) // for nfas warning

    alu[r_test_flag, CRYPTO_REQ_TEST_FLAG, AND, 1, <<CRYPTO_REQ_TEST_FLAG_BIT]

    crypto_threads_update_counter(CRYPTO_CNTR_CMD_READ)

    CRYPTO_PROFILE_ENTRY(0xf0)

    // Read sa addr, for re-use check below, before re-write it
    crypto_threads_get_lm_temp_addr(r_tmp, _me_ctx, r_buf)
    localmem_read1(r_tmp1, r_tmp, CRYPTO_LM_SA_ADDR)

    // Save request params needed by output thread in local memory
    localmem_write8(CRYPTO_REQ_PKT_WRBK_ADDR_UPPER, \
                    CRYPTO_REQ_PKT_END_ADDR_UPPER, \
                    CRYPTO_REQ_SA_ADDR, \
                    CRYPTO_REQ_PKT_STRT_ADDR, \
                    CRYPTO_REQ_PKT_CONT_ADDR, \
                    CRYPTO_REQ_PKT_END_ADDR, \
                    CRYPTO_REQ_PKT_WRBK_ADDR, \
                    CRYPTO_REQ_PKT_STRT_LENGTH, \
                    r_tmp, CRYPTO_LM_PKT_WRBK_ADDR_UPPER)
    localmem_write_next(CRYPTO_REQ_PKT_END_LENGTH)
    localmem_write_next(CRYPTO_REQ_PKT_WRBK_END_LENGTH)
    localmem_write_next(CRYPTO_REQ_AUTH_HDR_OFFSET)
    localmem_write_next(CRYPTO_REQ_AUTH_DATA_OFFSET)
    localmem_write_next(CRYPTO_REQ_SA_CONTROL_WORD)
    localmem_write_next(CRYPTO_REQ_USER_CONTROL)
    localmem_write_next(CRYPTO_REQ_SEQ_UPPER)

    // move sa control word into gpr
    move(r_sa_ctlwrd, CRYPTO_REQ_SA_CONTROL_WORD)


    // tell compiler scope of $xfr
    .begin
    .reg $xfr[16]
    .xfer_order $xfr


    // If SA already in the current buffer is same as needed for this packet,
    // skip reading it in again. Assuming all sa's will be provided from same
    // memory island, so not comparing upper address byte. Bits 8:0 of SA
    // Address are the SA 'version' and are part of comparison but not used
    // for addressing. If host re-uses SA table location for a new SA, it
    // must change the version so we know the cached SA, if any, is not
    // valid anymore. Note version bits located in the sa itself are not used.
    // Optionally, the 'sa flush' flag word in cls provides a way for user code
    // to 'flush' any saved sa data by invoking crypto_threads_set_sa_flush_flags().
    // This flush mechanism should be used if an sa may be updated 512 times
    // without a packet being transmitted, as the sa 'version' field could wrap
    // and the change missed by this code. To enable the flush mechanism, the file
    // should be built with #define ENABLE_SA_FLUSH 1. By default it is disabled.
    #if ( ENABLE_SA_FLUSH == 1 )
    crypto_threads_tst_clr_sa_flush_flag(r_tmp2, _crypto_ctx, r_buf)
    .if ( r_tmp2 == 0 && \
            CRYPTO_REQ_SA_ADDR == r_tmp1 )
    #else
    .if (CRYPTO_REQ_SA_ADDR == r_tmp1 )
    #endif
        // re-use sa
        localmem_read1(r_sa_ctlwrd, r_tmp, CRYPTO_LM_SA_CTLWRD)
        crypto_threads_update_counter(CRYPTO_CNTR_SA_REUSE)
        br[crypto_input_thread_replay_check#]

    .else

        // TBD implement mechanism to cache SA data in crypto island CTM to avoid
        // reading it from MU on every packet. or user of this code may do it

        // check sa ctlwrd for invalid cipher, mode, etc.
        #if ( ENABLE_ERROR_CHECKS == 1 )
        crypto_threads_check_sa(r_tmp, r_sa_ctlwrd)
        .if ( r_tmp != 0 )
            // trash saved value of sa addr so won't get a match
            // on next packet
            crypto_threads_get_lm_temp_addr(r_tmp, _me_ctx, r_buf)
            move(r_tmp1, 0xffffffff)
            localmem_write1(r_tmp1, r_tmp, CRYPTO_LM_SA_ADDR)
            // send failure response
            crypto_threads_input_send_response( _me_ctx, r_buf, \
                     CRYPTO_RSP_STATUS_SA_SELECTION_ERR )
            br[crypto_input_thread_loop#]
        .endif
        #endif

        // read SA key data to crypto sram
        crypto_threads_get_sa_addr(r_sram_sa, _crypto_ctx, r_buf)
        alu[r_sram_sa, r_sram_sa, +, CRYPTO_SA_DMA_KEYS_OFFSET]
        move(r_tmp, CRYPTO_REQ_SA_VERSION_MASK) // mask off version bits
        alu[r_tmp1, CRYPTO_REQ_SA_ADDR, AND~, r_tmp]
        alu[r_tmp1, r_tmp1, OR, CRYPTO_SA_KEYS_OFFSET, <<2]
        // special sa handling for aesgcm
        alu[r_tmp2, CRYPTO_SA_CIMODE_MASK, AND, r_sa_ctlwrd, \
                                  >>CRYPTO_SA_CIMODE_SHIFT]
        .if (r_tmp2 == CRYPTO_SA_CIMODE_CTR )
            // for aesgcm, the sram locations that usually hold the
            // 1st 4 lwords of the hash key instead hold the constructed
            // initialization vector, which is 4 lwords: salt, iv, iv, counter.
            // The salt word is the 1st lword just following the cipher key
            // in the sa in dram. The microcde has to put together the
            // constructed iv before the cmd sequence is issued to the
            // crypto engine. So, we are forced to 'manually' read in the
            // salt word now. (Another alternative would be to read in
            // both the cipher key and salt word now and wait for them)
            ld_field_w_clr[r_tmp, 0001,                             \
                CRYPTO_REQ_SA_ADDR_UPPER, >>CRYPTO_REQ_SA_ADDR_UPPER_SHIFT]
            alu[r_tmp, --, B, r_tmp, <<24] // upper byte
            alu[r_tmp2, r_tmp1, +, CRYPTO_SA_DMA_SALT_OFFSET]
            mem[read32, $xfr[0], r_tmp, <<8, r_tmp2, 1], ctx_swap[s]
            alu[$xfr[0], --, B, $xfr[0]]
            alu[$xfr[1], --, B, 0]
            alu[r_tmp2, r_sram_sa, +, CRYPTO_SA_DMA_SALT_OFFSET]
            crypto[write, $xfr[0], 0, r_tmp2, 1], ctx_swap[s]
            // Immediately following the constructed iv we need 4 lwords
            // of zeros used in the gcm algorithm, and inited here
            alu[r_tmp2, r_sram_sa, +, CRYPTO_SA_DMA_ZROS_OFFSET]
            aggregate_zero($xfr, 4)
            crypto[write, $xfr[0], 0, r_tmp2, 2], ctx_swap[s]
            // the dma count = cipher key only
            alu[r_tmp2, --, B, CRYPTO_SA_DMA_CKEY_LEN]
        .else // for others, dma count is cipher + hash keys
            alu[r_tmp2,--, B, CRYPTO_SA_DMA_KEYS_LEN]
        .endif
        // start up the key data dma
        ld_field_w_clr[r_tmp, 0001, \
            CRYPTO_REQ_SA_ADDR_UPPER, >>CRYPTO_REQ_SA_ADDR_UPPER_SHIFT]
        crypto_dma_in ($xfr, _crypto_ctx, CHAINED, CHAINED_NOT_LAST, \
                                               r_sram_sa, r_tmp1, \
                                                r_tmp, r_tmp2, --)
    .endif



    //
    // do preliminary anti replay check here
    //
    crypto_input_thread_replay_check#:
    CRYPTO_PROFILE_ENTRY(0xf1)
    #if ( ENABLE_ANTI_REPLAY == 1 )
    alu[r_dir, CRYPTO_SA_DIR_MASK, AND, r_sa_ctlwrd, >>CRYPTO_SA_DIR_SHIFT]
    .if ( r_dir == CRYPTO_SA_DIR_DECRYPT )
        .if ( r_sa_ctlwrd & ( 1 << CRYPTO_SA_AR_ENA_SHIFT ))
            .begin
            .sig z
            .reg r_status
            .reg $x

            // read sequence number 31:0 from the packet.
            // assuming it is in the start of packet area. if not,
            // need to fix this
            ld_field_w_clr[r_tmp, 0001, \
                    CRYPTO_REQ_PKT_STRT_ADDR_UPPER, \
                    >>CRYPTO_REQ_PKT_STRT_ADDR_UPPER_SHIFT]
            alu[r_tmp, --, B, r_tmp, <<24] // upper byte
            ld_field_w_clr[r_tmp1, 0011, CRYPTO_REQ_AUTH_HDR_OFFSET, \
                                     >>CRYPTO_REQ_AUTH_HDR_OFFSET_SHIFT]
            alu[r_tmp1, r_tmp1, +, CRYPTO_REQ_PKT_STRT_ADDR]
            alu[r_tmp2, CRYPTO_SA_PROTOCOL_MASK, AND, r_sa_ctlwrd, \
                                  >>CRYPTO_SA_PROTOCOL_SHIFT]
            .if ( r_tmp2 == CRYPTO_SA_PROTOCOL_ESP )
                alu[r_tmp1, r_tmp1, +, 4] // seq is 2nd lword in auth hdr
            .else
                alu[r_tmp1, r_tmp1, +, 8] // seq is 3rd lword in auth hdr
            .endif
            mem[read32, $x, r_tmp, <<8, r_tmp1, 1], ctx_swap[z]

            // save the seq number (all 64 bits) in lm for processing in output thread
            crypto_threads_get_lm_temp_addr(r_tmp, _me_ctx, r_buf)
            localmem_write2($x, CRYPTO_REQ_SEQ_UPPER, r_tmp, CRYPTO_LM_PKT_SEQ_NUM_LOW)

            // now do the anti replay check
            move(r_status, CRYPTO_RSP_STATUS_OK)
            crypto_threads_anti_replay(_me_ctx, r_buf, r_status, 0)
            .if ( r_status != CRYPTO_RSP_STATUS_OK )
                crypto_threads_input_send_response( _me_ctx, r_buf, r_status)
                br[crypto_input_thread_loop#]
            .endif
            .end
        .endif
    .endif
    #endif



    //
    // generate params for use by crypto sequence
    //
    crypto_input_thread_gen_params#:
    CRYPTO_PROFILE_ENTRY(0xf2)

    crypto_threads_get_pkt_addr(r_sram_pkt, _crypto_ctx, r_buf)
    alu[r_tmp, CRYPTO_REQ_PKT_STRT_ADDR, AND, 7]
    // actual pkt start in sram is offset for non-8 byte alignment
    alu[r_sram_pkt, r_sram_pkt, +, r_tmp]

    alu[r_tmp, CRYPTO_SA_PROTOCOL_MASK, AND, r_sa_ctlwrd, \
                                  >>CRYPTO_SA_PROTOCOL_SHIFT]

    // ESP, either transport or tunnel
    .if ( r_tmp == CRYPTO_SA_PROTOCOL_ESP )
        // common params
        ld_field_w_clr[r_tmp, 0011, CRYPTO_REQ_PTEXT_OFFSET, \
                                 >>CRYPTO_REQ_PTEXT_OFFSET_SHIFT]
        alu[r_sram_ptext, r_sram_pkt, +, r_tmp]
        ld_field_w_clr[r_ptext_size, 0011, CRYPTO_REQ_PTEXT_LENGTH, \
                                 >>CRYPTO_REQ_PTEXT_LENGTH_SHIFT]
        alu[r_dir, CRYPTO_SA_DIR_MASK, AND, r_sa_ctlwrd, >>CRYPTO_SA_DIR_SHIFT]
        alu[r_cipher, CRYPTO_SA_CIPHER_MASK, AND, r_sa_ctlwrd, \
                                        >>CRYPTO_SA_CIPHER_SHIFT]
        alu[r_cimode, CRYPTO_SA_CIMODE_MASK, AND, r_sa_ctlwrd, \
                                        >>CRYPTO_SA_CIMODE_SHIFT]
        alu[r_hash, CRYPTO_SA_HASH_MASK, AND, r_sa_ctlwrd, >>CRYPTO_SA_HASH_SHIFT]
        .if ( r_hash == CRYPTO_SA_HASH_NONE ) // hash none hangs crypto sequence
            move(r_hash, CRYPTO_SA_HASH_SHA1_96)
        .endif
        #pragma warning(disable:5008)
        crypto_threads_get_cipher_val(r_tmp, r_cipher)
        crypto_threads_get_cipher_mode_val(r_tmp1, r_cimode)
        crypto_threads_get_hash_val(r_tmp2, r_hash)
        crypto_setup_configs(r_cfg_hlen, r_iv_len, r_cfg_select, r_cfg_modes, \
                               r_dir, r_tmp, r_tmp1, r_tmp2)
        #pragma warning(default:5008)
        crypto_threads_get_vars_addr(r_sram_vars, _crypto_ctx, r_buf)

        // param setup that depends on cipher type
        alu[r_tmp2, CRYPTO_SA_CIMODE_MASK, AND, r_sa_ctlwrd, \
                                  >>CRYPTO_SA_CIMODE_SHIFT]
        .if (r_tmp2 != CRYPTO_SA_CIMODE_CTR )

            // For non CTR mode (i.e. not AES GCM ESP)

            // spi, seq(31:0) in packet data
            ld_field_w_clr[r_tmp, 0011, CRYPTO_REQ_AUTH_HDR_OFFSET, \
                              >>CRYPTO_REQ_AUTH_HDR_OFFSET_SHIFT]
            alu[r_sram_spi, r_sram_pkt, +, r_tmp]

            // crypt output location in sram
            alu[r_sram_ctext, --, B, r_sram_ptext]

            // iv in packet data
            ld_field_w_clr[r_tmp, 0011, CRYPTO_REQ_IV_OFFSET, \
                                 >>CRYPTO_REQ_IV_OFFSET_SHIFT]
            alu[r_sram_iv, r_sram_pkt, +, r_tmp]

            // auth data length
            alu[r_auth_len, r_iv_len, +, 7] // spi + seq(31:0) + iv - 1

            // move seq(63:32) to sram
            alu[r_sram_seq_ua, r_sram_vars, +, CRYPTO_VARS_SEQ_UA_OFFSET]
            alu[$xfr[0], --, B, CRYPTO_REQ_SEQ_UPPER]
            alu[$xfr[1], --, B, 0]
            crypto[write, $xfr[0], r_sram_seq_ua, 1], ctx_swap[s]

        .else

            // For CTR Mode (AES GCM ESP) setup the fields
            // needed in the sram to do the crypt. These are fields that
            // are updated for each packet. The salt and zero field were
            // setup during the sa read in.

            // read the salt back out from sram, because we have to write
            // sram in 8 byte chunks when we write the iv
            crypto_threads_get_sa_addr(r_sram_sa, _crypto_ctx, r_buf)
            alu[r_sram_sa, r_sram_sa, +, CRYPTO_SA_DMA_KEYS_OFFSET]
            alu[r_tmp2, r_sram_sa, +, CRYPTO_SA_DMA_SALT_OFFSET]
            crypto[read, $xfr[0], 0, r_tmp2, 1], ctx_swap[s]
            // move to write xfr reg
            alu[$xfr[0], --, B, $xfr[0]]
            // read spi, seq 31:0, and iv from packet.
            ld_field_w_clr[r_tmp, 0001,                     \
                             CRYPTO_REQ_PKT_STRT_ADDR_UPPER, \
                             >>CRYPTO_REQ_PKT_STRT_ADDR_UPPER_SHIFT]
            alu[r_tmp, --, B, r_tmp, <<24] // upper byte
            ld_field_w_clr[r_tmp1, 0011, CRYPTO_REQ_AUTH_HDR_OFFSET, \
                              >>CRYPTO_REQ_AUTH_HDR_OFFSET_SHIFT]
            alu[r_tmp1, r_tmp1, +, CRYPTO_REQ_PKT_STRT_ADDR]
            mem[read32, $xfr[0], r_tmp, <<8, r_tmp1, 4], ctx_swap[s]
            // move iv to write xfr regs
            alu[$xfr[1], --, B, $xfr[2]]
            alu[$xfr[2], --, B, $xfr[3]]
            // zero the counter field
            alu[$xfr[3], --, B, 0]
            // write the sram with the 4 lwords for 'constructed' iv
            crypto[write, $xfr[0], 0, r_tmp2, 2], ctx_swap[s]

            // now setup the auth data and the auth_length. 4 different cases
            alu[$xfr[0], --, B, $xfr[0]] // spi
           .if ((r_cipher >= CRYPTO_SA_CIPHER_AES128_NULL) && \
                (r_cipher <= CRYPTO_SA_CIPHER_AES256_NULL))
                // aes gcm null cipher, need to include iv
                alu[r_tmp2, CRYPTO_SA_SEQNUM_64_MASK, AND, r_sa_ctlwrd, \
                       >>CRYPTO_SA_SEQNUM_64_SHIFT]
                .if (r_tmp2)
                    // add seq(63:32) + seq(31:0) + iv
                    alu[$xfr[1], --, B, CRYPTO_REQ_SEQ_UPPER] // seq(63:32)
                    alu[$xfr[2], --, B, $xfr[1]] // seq(31:0)
                    alu[$xfr[3], --, B, $xfr[2]] // iv
                    alu[$xfr[4], --, B, $xfr[3]] // iv
                    alu[r_auth_len, r_iv_len, +, 11] // esn = iv + 12 bytes
                    alu[r_tmp2, --, B, 2] // # sram writes - 1
                .else
                    // add seq(31:0) + iv
                    alu[$xfr[1], --, B, $xfr[1]] // seq(31:0)
                    alu[$xfr[2], --, B, $xfr[2]] // iv
                    alu[$xfr[3], --, B, $xfr[3]] // iv
                    alu[r_auth_len, r_iv_len, +, 7] // not esn = iv + 8 bytes
                    alu[r_tmp2, --, B, 1] // # sram writes - 1
                .endif
            .else
                // aes gcm cipher, no iv needed
                alu[r_tmp2, CRYPTO_SA_SEQNUM_64_MASK, AND, r_sa_ctlwrd, \
                       >>CRYPTO_SA_SEQNUM_64_SHIFT]
                .if (r_tmp2)
                    // add seq(63:32) + seq(31:0)
                    alu[$xfr[1], --, B, CRYPTO_REQ_SEQ_UPPER] // seq(63:32)
                    alu[$xfr[2], --, B, $xfr[1]] // seq(31:0)
                    alu[r_auth_len, --, B, 11] // esn = 12 bytes
                    alu[r_tmp2, --, B, 1] // # sram writes - 1
                .else
                    // seq(31:0)
                    alu[$xfr[1], --, B, $xfr[1]] // seq(31:0)
                    alu[r_auth_len, --, B,  7] // not esn, 8 bytes
                    alu[r_tmp2, --, B, 0] // # sram writes - 1
                .endif
            .endif

            // write the auth data to sram, 8 bytes x 1,2, or 3
            alu[r_tmp1, r_sram_sa, +, CRYPTO_SA_DMA_AUTH_OFFSET]
            alu[--, 0x80, OR, r_tmp2, <<8]
            #pragma warning(disable:4700)
            #pragma warning(disable:4701)
            crypto[write, $xfr[0], 0, r_tmp1, MAX_8], indirect_ref, ctx_swap[s]
            #pragma warning(default:4700)
            #pragma warning(default:4701)

            // now setup the len(A) and len(C) fields
            .if ((r_cipher >= CRYPTO_SA_CIPHER_AES128_NULL) && \
                 (r_cipher <= CRYPTO_SA_CIPHER_AES256_NULL))
                // for aes_null_gmac mode, the plain text length is '0'
                // and all the data is part of the aad
                alu[r_tmp2, CRYPTO_SA_SEQNUM_64_MASK, AND, r_sa_ctlwrd, \
                        >>CRYPTO_SA_SEQNUM_64_SHIFT]
                .if (r_tmp2)
                    alu[r_tmp2, r_ptext_size, +, 20] // text+spi+seq(64)+iv
                .else
                    alu[r_tmp2, r_ptext_size, +, 16] // text+spi+seq(32)+iv
                .endif
                alu[$xfr[0], 0x7, AND, r_tmp2, >>29] // aad length in bits
                alu[$xfr[1], --, B, r_tmp2, <<3]
                alu[$xfr[2], --, B, 0]  // text length = 0
                alu[$xfr[3], --, B, 0]
            .else
                alu[$xfr[0], --, B, 0]  // AAD = spi + seq
                alu[r_tmp2, CRYPTO_SA_SEQNUM_64_MASK, AND, r_sa_ctlwrd, \
                        >>CRYPTO_SA_SEQNUM_64_SHIFT]
                .if (r_tmp2)
                    alu[$xfr[1], --, B, 0x60] // 64 bit seq, 96 bit AAD
                .else
                    alu[$xfr[1], --, B, 0x40] // 32 bit seq, 64 bit AAD
                .endif
                alu[$xfr[2], 0x7, AND, r_ptext_size, >>29] // ptext in bits
                alu[$xfr[3], --, B, r_ptext_size, <<3]
            .endif
            // write the sram with the 4 lwords
            alu[r_tmp2, r_sram_sa, +, CRYPTO_SA_DMA_LENS_OFFSET]
            crypto[write, $xfr[0], 0, r_tmp2, 2], ctx_swap[s]

            // now set crypto sequence parameters for aesgcm

            // sram addr of auth data - spi + seq(31:0) + seq(63:32)
            alu[r_sram_spi, r_sram_sa, +, CRYPTO_SA_DMA_AUTH_OFFSET]

            // sram addr of iv constructed
            alu[r_sram_iv, r_sram_sa, +, CRYPTO_SA_DMA_SALT_OFFSET]

            // sram addr of zeros (dual use of r_cfg_hlen)
            alu[r_cfg_hlen, r_sram_sa, +, CRYPTO_SA_DMA_ZROS_OFFSET]

            // sram addr of len(A)|len(C) field (dual use of r_sram_seq_ua)
            alu[r_sram_seq_ua, r_sram_sa, +, CRYPTO_SA_DMA_LENS_OFFSET]

            // if a 'null' mode cipher, r_ctext will point to the 'next'
            // buffer section, so that the crypt output can be discarded.
            .if ((r_cipher >= CRYPTO_SA_CIPHER_AES128_NULL) && \
                 (r_cipher <= CRYPTO_SA_CIPHER_AES256_NULL))
                // wait for next buffer to be available.
                // sigs 8,9,10,11, for buf_sig_A,B,C,D
                // are set when output thread is done with respective buffer
                alu[r_tmp, r_buf, +, 1]
                alu[r_tmp, r_tmp, AND, 3] // r_tmp is idx of next buffer
                move(r_tmp1, 0x100)
                alu[--, r_tmp, OR, 0]
                alu[r_tmp1, --, B, r_tmp1, <<indirect] // sig bit of next buffer
                ctx_arb[--], defer[1]
                local_csr_wr[ACTIVE_CTX_WAKEUP_EVENTS, r_tmp1]
                // next buffer is available, set r_sram_ctext
                crypto_threads_get_pkt_addr(r_tmp1, _crypto_ctx, r_tmp)
                move(r_tmp2, 0x3ff)
                alu[r_sram_ctext, r_sram_ptext, AND, r_tmp2]
                alu[r_sram_ctext, r_sram_ctext, OR, r_tmp1]
            .else
                alu[r_sram_ctext, --, B, r_sram_ptext] // crypt in place
            .endif
        .endif

        // put r_cfg_modes & r_cfg_select into a single reg to help nfas
        ld_field_w_clr[r_cfg_modes_select, 1100, r_cfg_modes, <<16]
        ld_field[r_cfg_modes_select, 0011, r_cfg_select]
        // put r_cfg_hlen & r_auth_len into a single reg to help nfas
        ld_field_w_clr[r_hash_auth_len, 1100, r_cfg_hlen, <<16]
        ld_field[r_hash_auth_len, 0011, r_auth_len]
        // put r_sram_iv & r_sram_spi into a single reg to help nfas
        ld_field_w_clr[r_sram_iv_spi, 1100, r_sram_iv, <<16]
        ld_field[r_sram_iv_spi, 0011, r_sram_spi]

    .else
        // shouldn't get here
        crypto_threads_input_send_response(  _me_ctx, r_buf, \
                                      CRYPTO_RSP_STATUS_SA_SELECTION_ERR )
        br[crypto_input_thread_loop#]
    .endif


    #if ( ENABLE_JUMBO_PKTS == 1 )
    // adjust ptext_size if needed, will only come through
    // here on the 1st buffer of a jumbo pkt. this is 1st
    // spot that the jumbo pkt is detected, and the jumbo
    // 'state' is updated here
    alu[r_tmp1, r_sram_ptext, +, r_ptext_size]
    // Figure out the ICV length in the packet
    crypto_threads_get_icv_len(r_icv_len, r_hash)
    // for decrypt, ICV has to be entirely in one buffer
    .if ( r_dir == CRYPTO_SA_DIR_DECRYPT )
        alu[r_tmp1, r_tmp1, +, r_icv_len]
    .endif
    crypto_threads_get_pkt_addr(r_tmp, _crypto_ctx, r_buf)
    alu[r_tmp1, r_tmp1, -, r_tmp] // byte count to end of ptext
    move( r_tmp, CRYPTO_MAX_SRAM_BUF_BYTES )
    .if ( r_tmp1 > r_tmp )
        // must be a jumbo, ptext runs past end of buffer
        alu[r_tmp2, r_tmp1, -, r_tmp] // bytes exceeding buffer size
        crypto_threads_get_pkt_addr(r_tmp1, _crypto_ctx, r_buf)
        alu[r_ptext_size, r_sram_ptext, -, r_tmp1]
        alu[r_ptext_size, r_tmp, -, r_ptext_size]
        // for decrypt, make sure the entire icv plus at least
        // one byte of ciphertext is in the end buffer
        .if ( r_dir == CRYPTO_SA_DIR_DECRYPT )
            .if (r_tmp2 > r_icv_len)
                // there is at least 1 byte of ciphertext in the
                // jumbo end buffer, so just continue
            .else
                // there is NO ciphertext in the jumbo end buffer,
                // so we have to create some
                alu[r_ptext_size, r_ptext_size, -, r_icv_len]
            .endif
        .endif
        // text length has to be a multiple of the hash block size
        crypto_threads_get_hash_mask(r_tmp, r_hash)
        alu[r_ptext_size, r_ptext_size, AND~, r_tmp]

        #if ENABLE_CRYPTO_STATE_SAVE_RESTORE == 1
        // For aes-gcm, in order to continue processing a packet
        // across multiple buffers, if the crypto engine is reset
        // or reused  between buffers, we need to restore the counter
        // value that would exist at the end of the previous buffer.
        // The hardware does not have a way to save out and
        // restore the counter state. So here we are setting up a 2nd,
        // fake, constructed iv with a counter value that we think
        // will exist in the chip after the present buffer is processed.
        // This counter value will be used to re-initialize the
        // counter when the subsequent buffer is processed. Note all this is
        // really for test purposes, we don't really need to do this for the
        // present IPsec design.
        // read the current constructed iv from sram
        crypto_threads_get_sa_addr(r_tmp, _crypto_ctx, r_buf)
        alu[r_tmp, r_tmp, +, CRYPTO_SA_DMA_KEYS_OFFSET]
        alu[r_tmp, r_tmp, +, CRYPTO_SA_DMA_SALT_OFFSET]
        crypto[read, $xfr[0], 0, r_tmp, 2], ctx_swap[s]
        // setup the 'fake' constructed iv for use on the restore
        alu[$xfr[0], --, B, $xfr[0]]
        alu[$xfr[1], --, B, $xfr[1]]
        alu[$xfr[2], --, B, $xfr[2]]
        // updated counter value.
        alu[r_tmp, --, B, r_ptext_size, >>4] // 16B blocks
        alu[$xfr[3], r_tmp, +, 2] // this works for aesgcm
        crypto_threads_get_sa_addr(r_tmp, _crypto_ctx, r_buf)
        alu[r_tmp, r_tmp, +, CRYPTO_SA_DMA_ECTR_OFFSET]
        crypto[write, $xfr[0], 0, r_tmp, 2], ctx_swap[s]
        #endif

        // save working ptext offset for use on next buffer
        crypto_threads_get_lm_temp_addr(r_tmp, _me_ctx, 0)
        localmem_write1(r_ptext_size, r_tmp, \
                    CRYPTO_LM_COMM_JUMBO_PKT_PTEXT_OFF)

        // save address of hash key for use on next buffer
        crypto_threads_get_hash_key_addr(r_tmp1, _crypto_ctx, r_buf)
        localmem_write1(r_tmp1, r_tmp, \
                    CRYPTO_LM_COMM_JUMBO_HKEY_ADDR)

        // set jumbo state to 1st buffer of jumbo pkt
        localmem_write1(CRYPTO_LM_JUMBO_STATE_FIRST_BUF, \
                        r_tmp, CRYPTO_LM_COMM_JUMBO_STATE)

        // set jumbo state in the buffer specific lm
        crypto_threads_get_lm_temp_addr(r_tmp, _me_ctx, r_buf)
        localmem_write1(CRYPTO_LM_JUMBO_STATE_FIRST_BUF, \
                        r_tmp, CRYPTO_LM_JUMBO_STATE)

        crypto_threads_update_counter(CRYPTO_CNTR_JUMBO_FIRST)

    .endif

    br[crypto_input_thread_gjp_end#] // normal flow skips next section


    // code to generate crypto sequence parameters for jumbo packets,
    // when processing 'continuation' or 'end' packet buffer
    // top of input loop branches here when previous buffer processed
    // was for a jumbo packet. the jumbo state is also updated here.
    crypto_input_thread_gen_jumbo_params#:

    // update sequence vars temp address for this buffer
    crypto_threads_get_vars_addr(r_sram_vars, _crypto_ctx, r_buf)

    // 'plaintext' location will be at start of sram, adjusted for
    // alignment of end of previous buffer
    #pragma warning(disable:4701) // r_sram_ptext, r_ptext_size set from prev. buffer
    alu[r_tmp, r_sram_ptext, +, r_ptext_size]
    #pragma warning(default:4701)
    alu[r_tmp, r_tmp, AND, 7]
    crypto_threads_get_pkt_addr(r_sram_ptext, _crypto_ctx, r_buf)
    alu[r_sram_ptext, r_sram_ptext, +, r_tmp]

    // 'plaintext' size is rest of packet that fits in sram buffer
    #pragma warning(disable:4701) // CRYPTO_REQ_PTEXT_LENGTH still set from
                                  // previous buffer
    ld_field_w_clr[r_ptext_size, 0011, CRYPTO_REQ_PTEXT_LENGTH, \
                                 >>CRYPTO_REQ_PTEXT_LENGTH_SHIFT]
    #pragma warning(default:4701)
    crypto_threads_get_lm_temp_addr(r_tmp, _me_ctx, 0)
    localmem_read1(r_tmp1, r_tmp, CRYPTO_LM_COMM_JUMBO_PKT_PTEXT_OFF)
    alu[r_ptext_size, r_ptext_size, -, r_tmp1] // bytes left to go

    // figure max buffer available
    move(r_tmp, CRYPTO_MAX_SRAM_BUF_BYTES)
    alu[r_tmp1, r_sram_ptext, AND, 7]
    alu[r_tmp, r_tmp, -, r_tmp1]

    // Figure out the ICV length in the packet
    #pragma warning(disable:4701) // r_hash set from prev. buffer
    crypto_threads_get_icv_len(r_icv_len, r_hash)
    #pragma warning(default:4701)

    // for decrypt, ICV has to be entirely in one buffer
    #pragma warning(disable:4701) // r_dir always set
    .if ( r_dir == CRYPTO_SA_DIR_DECRYPT )
    #pragma warning(default:4701)
        alu[r_tmp1, r_ptext_size, +, r_icv_len]
    .else
        move(r_tmp1, r_ptext_size)
    .endif

    .if ( r_tmp1 > r_tmp )
        // this is a 'continuation', still more to go after
        // this buffer, set the ptext size to the max
        move(r_ptext_size, r_tmp)
        // for decrypt, make sure the entire icv plus at least
        // one byte of ciphertext is in the end buffer
        #pragma warning(disable:4701) // r_dir always set
        .if ( r_dir == CRYPTO_SA_DIR_DECRYPT )
        #pragma warning(default:4701)
            alu[r_tmp2, r_tmp1, -, r_tmp]
            .if (r_tmp2 > r_icv_len)
                // there is at least 1 byte of ciphertext in the
                // jumbo end buffer, so just continue
            .else
                // there is NO ciphertext in the jumbo end buffer,
                // so we have to create some
                alu[r_ptext_size, r_ptext_size, -, r_icv_len]
            .endif
        .endif
        #pragma warning(disable:4701) // r_hash still set from previous buffer
        // text length has to be a multiple of the hash block size
        crypto_threads_get_hash_mask(r_tmp, r_hash)
        alu[r_ptext_size, r_ptext_size, AND~, r_tmp]

        // save working ptext offset for use on next buffer
        crypto_threads_get_lm_temp_addr(r_tmp, _me_ctx, 0)
        localmem_read1(r_tmp1, r_tmp, \
                    CRYPTO_LM_COMM_JUMBO_PKT_PTEXT_OFF)
        alu[r_tmp1, r_tmp1, +, r_ptext_size]
        localmem_write1(r_tmp1, r_tmp, \
                    CRYPTO_LM_COMM_JUMBO_PKT_PTEXT_OFF)

        // update jumbo state
        localmem_write1(CRYPTO_LM_JUMBO_STATE_CONTINUE_BUF, \
                        r_tmp, CRYPTO_LM_COMM_JUMBO_STATE)

        // update buffer specific jumbo state
        crypto_threads_get_lm_temp_addr(r_tmp, _me_ctx, r_buf)
        localmem_write1(CRYPTO_LM_JUMBO_STATE_CONTINUE_BUF, \
                        r_tmp, CRYPTO_LM_JUMBO_STATE)

        crypto_threads_update_counter(CRYPTO_CNTR_JUMBO_CONTINUE)
    .else
        // this is the last buffer of the packet,
        // ptext_size is fine as is

        // update jumbo state
        crypto_threads_get_lm_temp_addr(r_tmp, _me_ctx, 0)
        localmem_write1(CRYPTO_LM_JUMBO_STATE_END_BUF, \
                        r_tmp, CRYPTO_LM_COMM_JUMBO_STATE)

        // update buffer specific jumbo state
        crypto_threads_get_lm_temp_addr(r_tmp, _me_ctx, r_buf)
        localmem_write1(CRYPTO_LM_JUMBO_STATE_END_BUF, \
                        r_tmp, CRYPTO_LM_JUMBO_STATE)

        crypto_threads_update_counter(CRYPTO_CNTR_JUMBO_END)

    .endif

    // for jumbo continue/end, setup crypt output addr in r_sram_ctext
    // if a 'null' mode cipher, r_ctext will point to the 'next'
    // buffer section, so that the crypt output can be discarded.
    .if ((r_cipher >= CRYPTO_SA_CIPHER_AES128_NULL) && \
         (r_cipher <= CRYPTO_SA_CIPHER_AES256_NULL))
         // wait for next buffer to be available.
         // sigs 8,9,10,11, for buf_sig_A,B,C,D
         // are set when output thread is done with respective buffer
         alu[r_tmp, r_buf, +, 1]
         alu[r_tmp, r_tmp, AND, 3] // r_tmp is idx of next buffer
         move(r_tmp1, 0x100)
         alu[--, r_tmp, OR, 0]
         alu[r_tmp1, --, B, r_tmp1, <<indirect] // sig bit of next buffer
         ctx_arb[--], defer[1]
         local_csr_wr[ACTIVE_CTX_WAKEUP_EVENTS, r_tmp1]
         // next buffer is available, set r_sram_ctext
         crypto_threads_get_pkt_addr(r_tmp1, _crypto_ctx, r_tmp)
         move(r_tmp2, 0x3ff)
         alu[r_sram_ctext, r_sram_ptext, AND, r_tmp2]
         alu[r_sram_ctext, r_sram_ctext, OR, r_tmp1]
         // restore r_tmp for use below
         crypto_threads_get_lm_temp_addr(r_tmp, _me_ctx, r_buf)
    .else
         alu[r_sram_ctext, --, B, r_sram_ptext] // crypt in place
    .endif


    // also for jumbo continue/end buffers, pass req params to output thread
    // (r_tmp set in both cases above)
    #pragma warning(disable:4701) // REQ_* still set from previous buffer
    localmem_write8(CRYPTO_REQ_PKT_WRBK_ADDR_UPPER, \
                    CRYPTO_REQ_PKT_END_ADDR_UPPER, \
                    CRYPTO_REQ_SA_ADDR, \
                    CRYPTO_REQ_PKT_STRT_ADDR, \
                    CRYPTO_REQ_PKT_CONT_ADDR, \
                    CRYPTO_REQ_PKT_END_ADDR, \
                    CRYPTO_REQ_PKT_WRBK_ADDR, \
                    CRYPTO_REQ_PKT_STRT_LENGTH, \
                    r_tmp, CRYPTO_LM_PKT_WRBK_ADDR_UPPER)
    localmem_write_next(CRYPTO_REQ_PKT_END_LENGTH)
    localmem_write_next(CRYPTO_REQ_PKT_WRBK_END_LENGTH)
    localmem_write_next(CRYPTO_REQ_AUTH_HDR_OFFSET)
    localmem_write_next(CRYPTO_REQ_AUTH_DATA_OFFSET)
    localmem_write_next(r_sa_ctlwrd)
    localmem_write_next(CRYPTO_REQ_USER_CONTROL)
    localmem_write_next(CRYPTO_REQ_SEQ_UPPER)
    #pragma warning(default:4701)

    // also for jumbo continue/end buffers, pass params to output thread
    // if the write back area was not fully written when processing the
    // previous buffer
    crypto_threads_get_lm_temp_addr(r_tmp, _me_ctx, 0)
    localmem_read1(r_tmp1, r_tmp, CRYPTO_LM_COMM_JUMBO_WRBK_REM_LEN)
    .if (r_tmp1)
        localmem_read1(r_tmp2, r_tmp, CRYPTO_LM_COMM_JUMBO_WRBK_REM_ADDR)
        crypto_threads_get_lm_temp_addr(r_tmp, _me_ctx, r_buf)
        localmem_write1(r_tmp2, r_tmp, CRYPTO_LM_JUMBO_WRBK_REM_ADDR)
        localmem_write_next(r_tmp1)
    .else
        crypto_threads_get_lm_temp_addr(r_tmp, _me_ctx, r_buf)
        localmem_write1(0, r_tmp, CRYPTO_LM_JUMBO_WRBK_REM_LEN)
    .endif

    // skip down to the dma in section
    br[crypto_input_thread_dma_in#]
    crypto_input_thread_gjp_end#:

    #endif // ENABLE_JUMBO_PKTS



    //
    // read packet data to crypto sram
    //

    // move lengths to gprs
    ld_field_w_clr[r_pkt_strt_len, 0011, CRYPTO_REQ_PKT_STRT_LENGTH, \
                                           >>CRYPTO_REQ_PKT_STRT_LENGTH_SHIFT]

    ld_field_w_clr[r_pkt_cont_len, 0011, CRYPTO_REQ_PKT_CONT_LENGTH, \
                                           >>CRYPTO_REQ_PKT_CONT_LENGTH_SHIFT]

    ld_field_w_clr[r_pkt_end_len, 0011, CRYPTO_REQ_PKT_END_LENGTH, \
                                           >>CRYPTO_REQ_PKT_END_LENGTH_SHIFT]

    ld_field_w_clr[r_pkt_wrbk_end_len, 0011, CRYPTO_REQ_PKT_WRBK_END_LENGTH, \
                                          >>CRYPTO_REQ_PKT_WRBK_END_LENGTH_SHIFT]

    ld_field_w_clr[r_pkt_end_addr_hi, 0001, CRYPTO_REQ_PKT_END_ADDR_UPPER, \
                                         >>CRYPTO_REQ_PKT_END_ADDR_UPPER_SHIFT]


    // for ESP Tunnel decrypt option 1 the continuation of packet length is
    // used to 'pass through' the writeback length for the response, so don't
    // use it to dma in data. Other operations may also not require a continue
    // dma in and should set the continuation of packet length in the req to 0.
    alu[r_tmp, CRYPTO_SA_PROTOCOL_MASK, AND, r_sa_ctlwrd, \
                                  >>CRYPTO_SA_PROTOCOL_SHIFT]
    alu[r_tmp1, CRYPTO_SA_PROTMODE_MASK, AND, r_sa_ctlwrd, \
                                  >>CRYPTO_SA_PROTMODE_SHIFT]
    .if ( (r_tmp == CRYPTO_SA_PROTOCOL_ESP) && \
           (r_tmp1 == CRYPTO_SA_PROTMODE_TUNNEL) && \
            (CRYPTO_REQ_DECRYPT_OPTION & (1 << CRYPTO_REQ_DECRYPT_OPTION_BIT)))
        move(r_pkt_cont_len,0)
    .endif


    crypto_input_thread_dma_in#: // br point for jumbo handling
    CRYPTO_PROFILE_ENTRY(0xf3)

    #pragma warning(disable:4701) // CRYPTO_REQ_* still set from previous buffer
    move(r_pkt_end_addr, CRYPTO_REQ_PKT_END_ADDR)
    #pragma warning(default:4701)



    #if ( ENABLE_JUMBO_PKTS == 1 )
    // adjust dma in operations for handling jumbo packets

    crypto_threads_get_lm_temp_addr(r_tmp, _me_ctx, 0)
    localmem_read1(r_tmp1, r_tmp, CRYPTO_LM_COMM_JUMBO_STATE)

    #pragma warning(disable:4701) // r_pkt_strt_len & r_pkt_cont_len are set
    .if ( r_tmp1 == CRYPTO_LM_JUMBO_STATE_FIRST_BUF )

        // processing 1st buffer of a jumbo. we already know the
        // entire packet can't fit in one sram buffer.

        // r_tmp is # bytes to be read in with jumbo 1st buffer,
        // as adjusted above for sram buffer length, hash block size, etc.
        ld_field_w_clr[r_tmp, 0011, CRYPTO_REQ_PTEXT_OFFSET, \
                                 >>CRYPTO_REQ_PTEXT_OFFSET_SHIFT]
        alu[r_tmp, r_tmp, +, r_ptext_size]

        // r_tmp1 is total # bytes in start + continue memory buffers
        alu[r_tmp1, r_pkt_strt_len, +, r_pkt_cont_len]

        .if (r_tmp < r_tmp1)
            // If not reading in the entire start + continue buffers with
            // the first jumbo buffer, save away the remaining addr and
            // length and it will be read in with the subsequent
            // jumbo continue or end buffer

            // N.B. as is, this will not work if we don't read in the full
            // start buffer and there is also a continue buffer.

            crypto_threads_get_lm_temp_addr(r_tmp1, _me_ctx, 0)

            .if (r_tmp < r_pkt_strt_len ) // not reading in full strt buffer

                // save remaining length
                alu[r_tmp2, r_pkt_strt_len, -, r_tmp]
                localmem_write1(r_tmp2, r_tmp1, \
                            CRYPTO_LM_COMM_JUMBO_FIRST_REM_LEN)

                // save low bits of address
                alu[r_tmp2, CRYPTO_REQ_PKT_STRT_ADDR, +, r_tmp]
                localmem_write1(r_tmp2, r_tmp1, \
    	                    CRYPTO_LM_COMM_JUMBO_FIRST_REM_ADDR)

                // save high bits of address
                ld_field_w_clr[r_tmp2, 0001, \
                            CRYPTO_REQ_PKT_STRT_ADDR_UPPER, \
                            >>CRYPTO_REQ_PKT_STRT_ADDR_UPPER_SHIFT]
                localmem_write1(r_tmp2, r_tmp1, \
                            CRYPTO_LM_COMM_JUMBO_FIRST_REM_ADDR_HI)

                // adjust start length
                alu[r_pkt_strt_len, --, B, r_tmp]

            .else // not reading in full cont buffer

                // save remaining length
                alu[r_tmp2, r_pkt_strt_len, +, r_pkt_cont_len]
                alu[r_tmp2, r_tmp2, -, r_tmp]
                localmem_write1(r_tmp2, r_tmp1, \
                            CRYPTO_LM_COMM_JUMBO_FIRST_REM_LEN)

                // save low bits of address
                alu[r_tmp2, r_tmp, -, r_pkt_strt_len]
                alu[r_tmp2, r_tmp2, +, CRYPTO_REQ_PKT_CONT_ADDR]
                localmem_write1(r_tmp2, r_tmp1, \
    	                    CRYPTO_LM_COMM_JUMBO_FIRST_REM_ADDR)

                // save high bits of address
                ld_field_w_clr[r_tmp2, 0001, \
                            CRYPTO_REQ_PKT_CONT_ADDR_UPPER, \
                            >>CRYPTO_REQ_PKT_CONT_ADDR_UPPER_SHIFT]
                localmem_write1(r_tmp2, r_tmp1, \
                            CRYPTO_LM_COMM_JUMBO_FIRST_REM_ADDR_HI)

                // adjust continue length
                alu[r_pkt_cont_len, r_tmp, -, r_pkt_strt_len]

            .endif

             move(r_pkt_end_len, 0) // not reading in anything from end
             localmem_write1(0, r_tmp1, \
                         CRYPTO_LM_COMM_JUMBO_PKT_END_OFF)
        .else

            // reading in the entire start + continue buffers with the
            // first jumbo buffer, so adjust the end length to be
            // processed
            alu[r_pkt_end_len, r_tmp, -, r_pkt_strt_len] // total less strt
            alu[r_pkt_end_len, r_pkt_end_len, -, r_pkt_cont_len] // less cont

            // save current end of packet area offset in 'common' lm
            crypto_threads_get_lm_temp_addr(r_tmp, _me_ctx, 0)
            localmem_write1(r_pkt_end_len, r_tmp, \
                         CRYPTO_LM_COMM_JUMBO_PKT_END_OFF)

            // clear the jumbo first remaining length
            localmem_write1(0, r_tmp, \
                            CRYPTO_LM_COMM_JUMBO_FIRST_REM_LEN)
        .endif
        #pragma warning(default:4701)


    .elif ( r_tmp1 == CRYPTO_LM_JUMBO_STATE_CONTINUE_BUF )

        // processing a jumbo 'continue' buffer,
        // size of buffer for start/continue is 0
        move(r_pkt_strt_len, 0)
        move(r_pkt_cont_len, 0)


        // reset sram addr to start of buffer
        crypto_threads_get_pkt_addr(r_sram_pkt, _crypto_ctx, r_buf)


        // if the start or continue buffer was not completely read
        // in during the jumbo first buffer, read it in now
        crypto_threads_get_lm_temp_addr(r_tmp, _me_ctx, 0)
        localmem_read1(r_tmp1, r_tmp, CRYPTO_LM_COMM_JUMBO_FIRST_REM_LEN)
        .if (r_tmp1)

            // clear CRYPTO_LM_COMM_JUMBO_FIRST_REM_LEN now
            localmem_write1(0, r_tmp, CRYPTO_LM_COMM_JUMBO_FIRST_REM_LEN)

            // r_pkt_end_len is ptext_size less the remaining strt/cont
            // data we are reading in here
            alu[r_pkt_end_len, r_ptext_size, -, r_tmp1]

           .if (r_tmp1 & 7) // make transfer an 8 byte multiple
                alu[r_tmp1, r_tmp1, +, 8]
                alu[r_tmp1, r_tmp1, AND~, 7]
            .endif

            localmem_read1(r_tmp2, r_tmp, \
                              CRYPTO_LM_COMM_JUMBO_FIRST_REM_ADDR)
            alu[r_tmp2, r_tmp2, AND~, 7] // 8 byte align addr

            localmem_read1(r_pkt_cont_addr_hi, r_tmp, \
                           CRYPTO_LM_COMM_JUMBO_FIRST_REM_ADDR_HI)

            crypto_dma_in ($xfr, _crypto_ctx, CHAINED, CHAINED_NOT_LAST, \
                            r_sram_pkt, r_tmp2, \
                            r_pkt_cont_addr_hi, r_tmp1, --)

    	    alu[r_sram_pkt, r_sram_pkt, +, r_tmp1] // advance r_sram_pkt
        .else
            move(r_pkt_end_len, r_ptext_size) // end len same as ptext size
        .endif


        // advance the end addr by the amount of 'end' data
        // already read in
        crypto_threads_get_lm_temp_addr(r_tmp, _me_ctx, 0)
        localmem_read1(r_tmp1, r_tmp, CRYPTO_LM_COMM_JUMBO_PKT_END_OFF)
        #pragma warning(disable:4701) // CRYPTO_REQ_* are valid from prev. buffer
        alu[r_pkt_end_addr, CRYPTO_REQ_PKT_END_ADDR, +, r_tmp1]
        #pragma warning(default:4701)
        // and save current end of packet area offset
        alu[r_tmp1, r_tmp1, +, r_pkt_end_len]
        localmem_write1(r_tmp1, r_tmp, \
            CRYPTO_LM_COMM_JUMBO_PKT_END_OFF)

        // save the end addr & ptext addr  in buffer specific
        // lm for output thread to copy out non-8 byte aligned data
        crypto_threads_get_lm_temp_addr(r_tmp, _me_ctx, r_buf)
        localmem_write1(r_pkt_end_addr, r_tmp, \
            CRYPTO_LM_JUMBO_BUF_END_ADDR)
        localmem_write1(r_sram_ptext, r_tmp, \
            CRYPTO_LM_JUMBO_BUF_SRAM_PTEXT)

        // if the new end addr is not 8 byte aligned,
        // need to back it up to previous 8 byte address
        .if ( r_pkt_end_addr & 7 )
            alu[r_pkt_end_addr, r_pkt_end_addr, AND~, 7]
            // and add one more 8 byte dma in to compensate
            alu[r_pkt_end_len, r_pkt_end_len, +, 8]
        .endif

    .elif ( r_tmp1 == CRYPTO_LM_JUMBO_STATE_END_BUF )

        // processing a jumbo 'end' buffer, so it has the remaining
        // size of buffer for 'end' and 0 for start/continue
        move(r_pkt_strt_len, 0)
        move(r_pkt_cont_len, 0)

        // reset sram addr to start of buffer
        crypto_threads_get_pkt_addr(r_sram_pkt, _crypto_ctx, r_buf)


        // if the start or continue buffer was not completely read
        // in during the jumbo first buffer, read it in now
        crypto_threads_get_lm_temp_addr(r_tmp, _me_ctx, 0)
        localmem_read1(r_tmp1, r_tmp, CRYPTO_LM_COMM_JUMBO_FIRST_REM_LEN)
        .if (r_tmp1)

            // clear CRYPTO_LM_COMM_JUMBO_FIRST_REM_LEN now
            localmem_write1(0, r_tmp, CRYPTO_LM_COMM_JUMBO_FIRST_REM_LEN)

           .if (r_tmp1 & 7) // make transfer an 8 byte multiple
                alu[r_tmp1, r_tmp1, +, 8]
                alu[r_tmp1, r_tmp1, AND~, 7]
            .endif

            localmem_read1(r_tmp2, r_tmp, \
                              CRYPTO_LM_COMM_JUMBO_FIRST_REM_ADDR)
            alu[r_tmp2, r_tmp2, AND~, 7] // 8 byte align addr

            localmem_read1(r_pkt_cont_addr_hi, r_tmp, \
                           CRYPTO_LM_COMM_JUMBO_FIRST_REM_ADDR_HI)

            crypto_dma_in ($xfr, _crypto_ctx, CHAINED, CHAINED_NOT_LAST, \
                            r_sram_pkt, r_tmp2, \
                            r_pkt_cont_addr_hi, r_tmp1, --)

    	    alu[r_sram_pkt, r_sram_pkt, +, r_tmp1] // advance r_sram_pkt

            alu[r_tmp2, --, B, r_sram_pkt] // for below set of auth addr
        .else
            alu[r_tmp2, --, B, r_sram_ptext] // for below set of auth addr
        .endif


        // end length is original total end length less amount
        // already read in ( this includes auth data for esp decrypt,
        // which is not included in 'plaintext' length)
        #pragma warning(disable:4701) // CRYPTO_REQ_* are valid from prev. buffer
        ld_field_w_clr[r_pkt_end_len, 0011, CRYPTO_REQ_PKT_END_LENGTH, \
                                           >>CRYPTO_REQ_PKT_END_LENGTH_SHIFT]
        #pragma warning(default:4701)
        crypto_threads_get_lm_temp_addr(r_tmp, _me_ctx, 0)
        localmem_read1(r_tmp1, r_tmp, CRYPTO_LM_COMM_JUMBO_PKT_END_OFF)
        alu[r_pkt_end_len, r_pkt_end_len, -, r_tmp1]


        // advance the end addr by the amount of 'end' data
        // already read in
        #pragma warning(disable:4701) // CRYPTO_REQ_* are valid from prev. buffer
        alu[r_pkt_end_addr, CRYPTO_REQ_PKT_END_ADDR, +, r_tmp1]
        #pragma warning(default:4701)

        // save the ptext sram addr in buffer
        // specific lm for output thread to copy out non-8 byte aligned data
        crypto_threads_get_lm_temp_addr(r_tmp, _me_ctx, r_buf)
        localmem_write1(r_sram_ptext, r_tmp, \
            CRYPTO_LM_JUMBO_BUF_SRAM_PTEXT)

        // save the auth data sram addr in buffer specific
        // lm for output thread to compare with calculated value
        // during esp decrypts.
        //
        // r_tmp2 set above depending on need to read in lefover start/continue
        // data that was not read in during the jumbo first buffer
        alu[r_tmp1, r_tmp2, +, r_pkt_end_len]
        #pragma warning(disable:4701)
        alu[r_tmp2, CRYPTO_SA_HASH_MASK, AND, r_sa_ctlwrd,\
                                         >>CRYPTO_SA_HASH_SHIFT]
        .if ( r_tmp2 != CRYPTO_SA_HASH_NONE )
            // Get the ICV length in the packet
            crypto_threads_get_icv_len(r_icv_len, r_tmp2)
            alu[r_tmp1, r_tmp1, -, r_icv_len]
        .endif
        #pragma warning(default:4701)
        localmem_write1(r_tmp1, r_tmp, \
            CRYPTO_LM_JUMBO_BUF_SRAM_AUTH_DATA)

        // if the new end addr is not 8 byte aligned,
        // need to back it up to previous 8 byte address
        alu[r_tmp, r_pkt_end_addr, AND, 7]
        .if ( r_tmp != 0 )
            alu[r_pkt_end_addr, r_pkt_end_addr, AND~, 7]
            // and add address offset to compensate
            alu[r_pkt_end_len, r_pkt_end_len, +, r_tmp]
        .endif

    .endif
    #endif

    #if ( ENABLE_JUMBO_PKTS == 1 )
    #pragma warning(disable:4701) // r_pkt_strt_len, etc. are set from read in.
                                  // problem flagged due to jump to
                                  // crypto_input_thread_dma_in# for jumbos
    #endif

    // start of packet ( typically in ctm )
    .if ( r_pkt_strt_len > 0 )
        alu[r_tmp, r_pkt_strt_len, AND, 7]
        alu[r_tmp1, CRYPTO_REQ_PKT_STRT_ADDR, AND, 7]
        .if ( r_tmp == 0 ) // 8 byte multiple length
            .if ( r_tmp1 == 0 ) // 8 byte aligned address
                // do nothing
            .else
                // non 8 byte aligned addr, add another 8 byte block to length
                alu[r_pkt_strt_len, r_pkt_strt_len, +, 8]
            .endif
        .else // non 8 byte multiple length
            .if ( r_tmp1 == 0 ) // 8 byte aligned address
                // round up length to next 8 byte multiple
                alu[r_pkt_strt_len, r_pkt_strt_len, +, 8]
                alu[r_pkt_strt_len, r_pkt_strt_len, -, r_tmp]
            .else
                // non 8 byte aligned address,
                // and non 8 byte length, round up length to next 8 byte multiple
                alu[r_pkt_strt_len, r_pkt_strt_len, +, 8]
                alu[r_pkt_strt_len, r_pkt_strt_len, -, r_tmp]
            .endif
        .endif
        crypto_threads_get_pkt_addr(r_sram_pkt, _crypto_ctx, r_buf)
        // N.B. the hardware accepts system mem addr bits 2:0 but zeros them out
        ld_field_w_clr[r_pkt_strt_addr_hi, 0001, \
                       CRYPTO_REQ_PKT_STRT_ADDR_UPPER, \
                       >>CRYPTO_REQ_PKT_STRT_ADDR_UPPER_SHIFT]

        .if ( r_pkt_end_len ) // have an end of packet transfer
            crypto_dma_in ($xfr, _crypto_ctx, CHAINED, CHAINED_NOT_LAST, \
                                    r_sram_pkt, CRYPTO_REQ_PKT_STRT_ADDR, \
                                    r_pkt_strt_addr_hi, r_pkt_strt_len, --)
        .elif ( r_pkt_cont_len ) // have a continue of packet transfer
            crypto_dma_in ($xfr, _crypto_ctx, CHAINED, CHAINED_NOT_LAST, \
                                    r_sram_pkt, CRYPTO_REQ_PKT_STRT_ADDR, \
                                    r_pkt_strt_addr_hi, r_pkt_strt_len, --)
        .else // only have the start of packet transfer
            // need 1 more block if was odd alignment
            alu[r_pkt_strt_len, r_pkt_strt_len, +, 8]
            crypto_dma_in ($xfr, _crypto_ctx, CHAINED, CHAINED_LAST, \
                                    r_sram_pkt, CRYPTO_REQ_PKT_STRT_ADDR, \
                                    r_pkt_strt_addr_hi, r_pkt_strt_len, --)
        .endif
        alu[r_sram_pkt, r_sram_pkt, +, r_pkt_strt_len]
    .endif


    // continuation of packet ( typically in ctm ) must be 8 byte aligned start
    .if ( r_pkt_cont_len > 0 )
        ld_field_w_clr[r_pkt_cont_addr_hi, 0001, \
                  CRYPTO_REQ_PKT_CONT_ADDR_UPPER, \
                   >>CRYPTO_REQ_PKT_CONT_ADDR_UPPER_SHIFT]
        .if ( r_pkt_end_len )
            crypto_dma_in ($xfr, _crypto_ctx, CHAINED, CHAINED_NOT_LAST, \
                            r_sram_pkt, CRYPTO_REQ_PKT_CONT_ADDR, \
                            r_pkt_cont_addr_hi, r_pkt_cont_len, --)
        .else
            // no end of packet, so ok if continue is not an 8 byte multiple
            // but need to round up the dma to get the last 1-7 bytes
            .if ( r_pkt_cont_len & 7 )
                alu[r_tmp, r_pkt_cont_len, +, 8]
                alu[r_tmp, r_tmp, AND~, 7]
            .else
                alu[r_tmp, --, B, r_pkt_cont_len]
            .endif

            crypto_dma_in ($xfr, _crypto_ctx, CHAINED, CHAINED_LAST, \
                            r_sram_pkt, CRYPTO_REQ_PKT_CONT_ADDR, \
                            r_pkt_cont_addr_hi, r_tmp, --)
        .endif
        alu[r_sram_pkt, r_sram_pkt, +, r_pkt_cont_len]
    .endif

    // end of packet ( typically in dram ) must be 8 byte aligned start address
    .if ( r_pkt_end_len > 0 )

        // if not an 8 byte multiple, round up dma to get last
        // 1-7 bytes
        .if ( r_pkt_end_len & 7 )
            alu[r_tmp, r_pkt_end_len, +, 8]
            alu[r_tmp, r_tmp, AND~, 7]
        .else
            alu[r_tmp, --, B, r_pkt_end_len]
        .endif

        crypto_dma_in ($xfr, _crypto_ctx, CHAINED, CHAINED_LAST, \
                                     r_sram_pkt, r_pkt_end_addr, \
                                          r_pkt_end_addr_hi, r_tmp, --)
    .endif

    #if ( ENABLE_JUMBO_PKTS == 1 )
    #pragma warning(default:4701) // r_pkt_strt_len, etc. set from read in
    #endif

    // end of tell compiler scope of $xfr
    .end


    // tell compiler scope of $runcmd, $dma_pkt_wrbk_out, etc.
    .begin
    .reg $runcmd[2], $dma_pkt_wrbk_out[4], $dma_pkt_end_out[4], $int_sig_msg[2]
    .xfer_order $runcmd, $dma_pkt_wrbk_out, $dma_pkt_end_out, $int_sig_msg


    //
    // load sequence params and generate crypto sequence cmd
    //
    crypto_input_thread_gen_crypto_sequence#:
    CRYPTO_PROFILE_ENTRY(0xf4)

    /// @cond INTERNAL CRYPTO_PARAMS
    #define CRYPTO_PARAMS     r_sa_ctlwrd, r_sram_vars,  \
                              r_sram_ptext, r_sram_ctext, r_ptext_size, \
                              r_sram_iv_spi, r_sram_seq_ua, \
                              r_hash_auth_len, r_cfg_modes_select
    /// @endcond
    /// @cond INTERNAL CRYPTO_CONT_PARAMS
    #define CRYPTO_CONT_PARAMS r_sram_vars, r_sram_ptext, r_sram_ctext, \
                               r_ptext_size
    /// @endcond
    /// @cond INTERNAL CRYPTO_END_PARAMS
    #define CRYPTO_END_PARAMS r_sa_ctlwrd, r_sram_vars, \
                              r_sram_ptext, r_sram_ctext, r_ptext_size, \
                              r_sram_seq_ua, r_hkey, r_cfg_hlen, r_hres, \
                              r_cfg_modes_select
    /// @endcond

    #if ( ENABLE_JUMBO_PKTS == 1 )
    #pragma warning(disable:4701) // the CRYPTO_PARAMS are setup during the read
                                  // in of the 1st buffer, and are retained or
                                  // adjusted for subsequent jumbo pkt buffers
    #endif

    #pragma warning(disable:5008) // $runcmd may be setup but not used in case of
                                  // errors

    #if ( ENABLE_JUMBO_PKTS == 1 )
    crypto_threads_get_lm_temp_addr(r_tmp, _me_ctx, 0)
    localmem_read1(r_tmp1, r_tmp, CRYPTO_LM_COMM_JUMBO_STATE)
    #endif

    #define NO_WAIT 1 // use the 'no wait' sequence
    #define WAIT    0 // use the 'normal' sequence

    .if ( r_dir ==  CRYPTO_SA_DIR_ENCRYPT )

        #if ( ENABLE_JUMBO_PKTS == 1 )
        .if ( (r_tmp1 == CRYPTO_LM_JUMBO_STATE_NON_JUMBO) \
              || (r_tmp1 == CRYPTO_LM_JUMBO_STATE_FIRST_BUF ))
            crypto_threads_update_counter(CRYPTO_CNTR_CMD_READ_ENCRYPT)
            alu[r_tmp, CRYPTO_CNTR_PKTS_ENCRYPT_CORE_0, +, _crypto_ctx]
            crypto_threads_update_counter(r_tmp)
        .endif
        #else
        crypto_threads_update_counter(CRYPTO_CNTR_CMD_READ_ENCRYPT)
        alu[r_tmp, CRYPTO_CNTR_PKTS_ENCRYPT_CORE_0, +, _crypto_ctx]
        crypto_threads_update_counter(r_tmp)
        #endif

        .if ( r_cipher == CRYPTO_SA_CIPHER_NULL )
            // CIPHER_NULL hangs if use route code cipher then hash in serial.
            // Instead, have to use cipher and hash, parallel, which does not
            // hang. As it turns out, this is the same as the decrypt sequence.
            // So to work around this problem, do decrypt instead of encrypt.
            // Also note that while r_dir ( encrypt vs. decrypt ) figures into
            // the r_cfg_modes param value, it does not have any effect for
            // CIPHER_NONE.

            #if ( ENABLE_JUMBO_PKTS == 1 )
            .if ( r_tmp1 == CRYPTO_LM_JUMBO_STATE_NON_JUMBO )
            #endif
                crypto_threads_decrypt($runcmd, r_buf, _crypto_ctx, \
                                            CRYPTO_PARAMS)
            #if ( ENABLE_JUMBO_PKTS == 1 )
            .elif ( r_tmp1 == CRYPTO_LM_JUMBO_STATE_FIRST_BUF )
                crypto_threads_decrypt_strt($runcmd, r_buf, _crypto_ctx, \
                                            CRYPTO_PARAMS, NO_WAIT)
            .elif ( r_tmp1 == CRYPTO_LM_JUMBO_STATE_CONTINUE_BUF )
                crypto_threads_decrypt_cont($runcmd, _crypto_ctx,
                                            CRYPTO_CONT_PARAMS, NO_WAIT)
            .elif ( r_tmp1 == CRYPTO_LM_JUMBO_STATE_END_BUF )
                localmem_read1(r_hkey, r_tmp, CRYPTO_LM_COMM_JUMBO_HKEY_ADDR)
                crypto_threads_get_hash_addr(r_hres, _crypto_ctx, r_buf)
                crypto_threads_decrypt_end($runcmd, _crypto_ctx,
                                            CRYPTO_END_PARAMS)
            .endif
            #endif
        .else
             alu[r_tmp2, --, B, 0]
            .if ((r_cipher >= CRYPTO_SA_CIPHER_AES128_NULL) && \
                 (r_cipher <= CRYPTO_SA_CIPHER_AES256_NULL))
                 alu[r_tmp2, --, B, 1]
            .endif

            #if ( ENABLE_JUMBO_PKTS == 1 )
            .if ( r_tmp1 == CRYPTO_LM_JUMBO_STATE_NON_JUMBO )
            #endif
                .if (r_tmp2)
                    // for aesgcm-null (rfc4345) do decrypt for encrypt
                    crypto_threads_decrypt($runcmd, r_buf, _crypto_ctx, \
                                            CRYPTO_PARAMS)
                .else
                    crypto_threads_encrypt($runcmd, r_buf, _crypto_ctx, \
                                            CRYPTO_PARAMS)
                .endif
            #if ( ENABLE_JUMBO_PKTS == 1 )
            .elif ( r_tmp1 == CRYPTO_LM_JUMBO_STATE_FIRST_BUF )
                .if (r_tmp2)
                    // for aesgcm-null (rfc4345) do decrypt for encrypt
                    crypto_threads_decrypt_strt($runcmd, r_buf, _crypto_ctx, \
                                            CRYPTO_PARAMS, WAIT)
                .else
                    crypto_threads_encrypt_strt($runcmd, r_buf, _crypto_ctx, \
                                            CRYPTO_PARAMS)
                .endif
            .elif ( r_tmp1 == CRYPTO_LM_JUMBO_STATE_CONTINUE_BUF)
                .if (r_tmp2)
                    // for aesgcm-null (rfc4345) do decrypt for encrypt
                    crypto_threads_decrypt_cont($runcmd, _crypto_ctx,
                                            CRYPTO_CONT_PARAMS, WAIT)
                 .else
                    crypto_threads_encrypt_cont($runcmd, _crypto_ctx,
                                            CRYPTO_CONT_PARAMS)
                 .endif
            .elif ( r_tmp1 == CRYPTO_LM_JUMBO_STATE_END_BUF )
                localmem_read1(r_hkey, r_tmp, CRYPTO_LM_COMM_JUMBO_HKEY_ADDR)
                crypto_threads_get_hash_addr(r_hres, _crypto_ctx, r_buf)
                .if (r_tmp2)
                    // for aesgcm-null (rfc4345) do decrypt for encrypt
                    crypto_threads_decrypt_end($runcmd, _crypto_ctx,
                                            CRYPTO_END_PARAMS)
                .else
                    crypto_threads_encrypt_end($runcmd, _crypto_ctx,
                                            CRYPTO_END_PARAMS)
                .endif
            .endif
            #endif
        .endif

    .else // decrypt

        #if ( ENABLE_JUMBO_PKTS == 1 )
        .if ( (r_tmp1 == CRYPTO_LM_JUMBO_STATE_NON_JUMBO) \
              || (r_tmp1 == CRYPTO_LM_JUMBO_STATE_FIRST_BUF ))
            crypto_threads_update_counter(CRYPTO_CNTR_CMD_READ_DECRYPT)
            alu[r_tmp, CRYPTO_CNTR_PKTS_DECRYPT_CORE_0, +, _crypto_ctx]
            crypto_threads_update_counter(r_tmp)
        .endif
        #else
        crypto_threads_update_counter(CRYPTO_CNTR_CMD_READ_DECRYPT)
        alu[r_tmp, CRYPTO_CNTR_PKTS_DECRYPT_CORE_0, +, _crypto_ctx]
        crypto_threads_update_counter(r_tmp)
        #endif

        #if ( ENABLE_JUMBO_PKTS == 1 )
        .if ( r_tmp1 == CRYPTO_LM_JUMBO_STATE_NON_JUMBO )
        #endif
            crypto_threads_decrypt($runcmd, r_buf, _crypto_ctx, \
                                            CRYPTO_PARAMS)
        #if ( ENABLE_JUMBO_PKTS == 1 )
        .elif ( r_tmp1 == CRYPTO_LM_JUMBO_STATE_FIRST_BUF )
            // see above re crypto engine hangs on ciper none if have a wait
            // coded; this sequence does not have a wait
            .if ( r_cipher == CRYPTO_SA_CIPHER_NULL )
                crypto_threads_decrypt_strt($runcmd, r_buf, _crypto_ctx, \
                                            CRYPTO_PARAMS, NO_WAIT)
            .else
                crypto_threads_decrypt_strt($runcmd, r_buf, _crypto_ctx, \
                                            CRYPTO_PARAMS, WAIT)
            .endif
        .elif ( r_tmp1 == CRYPTO_LM_JUMBO_STATE_CONTINUE_BUF )
            // see above re crypto engine hangs on ciper none if have a wait
            // coded; this sequence does not have a wait
            .if ( r_cipher == CRYPTO_SA_CIPHER_NULL )
                crypto_threads_decrypt_cont($runcmd, _crypto_ctx,
                                            CRYPTO_CONT_PARAMS, NO_WAIT)
            .else
                crypto_threads_decrypt_cont($runcmd, _crypto_ctx,
                                            CRYPTO_CONT_PARAMS, WAIT)
            .endif
        .elif ( r_tmp1 == CRYPTO_LM_JUMBO_STATE_END_BUF )
            localmem_read1(r_hkey, r_tmp, CRYPTO_LM_COMM_JUMBO_HKEY_ADDR)
            crypto_threads_get_hash_addr(r_hres, _crypto_ctx, r_buf)
            crypto_threads_decrypt_end($runcmd, _crypto_ctx,
                                            CRYPTO_END_PARAMS)
        .endif
        #endif
    .endif

    #if ( ENABLE_JUMBO_PKTS == 1 )
    #pragma warning(default:4701) // see above
    #endif

    #pragma warning(default:5008) // see above

    .set $runcmd[0], $runcmd[1]


    //
    // generate cmds to dma the en/decrypted data back out, send a done signal,
    // and then send the cmds to the crypto core
    //
    CRYPTO_PROFILE_ENTRY(0xf5)

    #if ( ENABLE_JUMBO_PKTS == 1 )
    // for jumbo 'continue' or 'end' buffers, skip past the
    // dma for the 'writeback' area, which was already done with
    // the 1st jumbo buffer
    crypto_threads_get_lm_temp_addr(r_tmp, _me_ctx, 0)
    localmem_read1(r_tmp1, r_tmp, CRYPTO_LM_COMM_JUMBO_STATE)
    .if ( r_tmp1 == CRYPTO_LM_JUMBO_STATE_CONTINUE_BUF \
           || r_tmp1 == CRYPTO_LM_JUMBO_STATE_END_BUF )
        br[crypto_input_thread_dma_wrback_end#]
    .endif
    #endif

    #if ( ENABLE_JUMBO_PKTS == 1 )
    #pragma warning(disable:4701) // r_sa_ctlwrd, etc. are set from read in.
                                  // problem flagged due to jump to
                                  // crypto_input_thread_dma_in# for jumbos
    #endif


    alu[r_tmp, CRYPTO_SA_PROTOCOL_MASK, AND, r_sa_ctlwrd, \
                                  >>CRYPTO_SA_PROTOCOL_SHIFT]

    // ESP, either transport or tunnel
    .if ( r_tmp == CRYPTO_SA_PROTOCOL_ESP )

      //
      // encrypt, or decrypt option '2'. writes back entire packet
      //
      .if (!(CRYPTO_REQ_DECRYPT_OPTION & (1 << CRYPTO_REQ_DECRYPT_OPTION_BIT)))

        // figure starting address in sram
        crypto_threads_get_pkt_addr(r_sram_pkt, _crypto_ctx, r_buf)

        // if pkt start address not 8 byte aligned, dma has to start at next
        // 8 byte aligned address
        alu[r_tmp, CRYPTO_REQ_PKT_STRT_ADDR, AND, 7]
        .if ( r_tmp != 0 )
            alu[r_sram_pkt, r_sram_pkt, +, 8]
        .endif

        // make sure write back address is aligned same as start packet address
        alu[r_pkt_wrbk_addr, --, B, CRYPTO_REQ_PKT_WRBK_ADDR]
        alu[r_tmp1, r_pkt_wrbk_addr, AND, 7]
        #if ( ENABLE_ERROR_CHECKS == 1 )
        .if ( r_tmp1 != r_tmp )
            crypto_threads_input_send_response( _me_ctx, r_buf, \
                     CRYPTO_RSP_STATUS_WRITE_BACK_ALIGN_ERR )
            br[crypto_input_thread_loop#]
        .endif
        #endif

        // if pkt write back address not 8 byte aligned, dma has to start at
        // next 8 byte aligned address
        .if ( r_tmp1 != 0 )
            alu[r_pkt_wrbk_addr, r_pkt_wrbk_addr, +, 8]
            alu[r_pkt_wrbk_addr, r_pkt_wrbk_addr, -, r_tmp1]
        .endif

        // figure write back length,  make sure the alignment matches the address
        ld_field_w_clr[r_pkt_wrbk_len, 0011, CRYPTO_REQ_PKT_WRBK_LENGTH, \
                                       >>CRYPTO_REQ_PKT_WRBK_LENGTH_SHIFT]


        // if the write back length in the request is greater than the total
        // amount of data we have to write back, adjust the write back length.
        // save the remaining wrt back addr and length and it will be written
        // with the next buffer in the output thread. this can happen with
        // 1st buffer of a jumbo packet. for subsequent buffers of a jumbo
        // packet this code is skipped over, above.
        //
        // in these cases ( encrypt or decrypt option 2 ) the write back length
        // includes the packet data starting from the mac header.
        crypto_threads_get_lm_temp_addr(r_tmp, _me_ctx, 0)
        ld_field_w_clr[r_tmp1, 0011, CRYPTO_REQ_PTEXT_OFFSET, \
                                >>CRYPTO_REQ_PTEXT_OFFSET_SHIFT]
        alu[r_tmp1, r_tmp1, +, r_ptext_size]
        .if ( r_pkt_wrbk_len > r_tmp1 )
            alu[r_tmp2, CRYPTO_REQ_PKT_WRBK_ADDR, +, r_tmp1]
            localmem_write1(r_tmp2, r_tmp, CRYPTO_LM_COMM_JUMBO_WRBK_REM_ADDR)
            alu[r_tmp2, r_pkt_wrbk_len, -, r_tmp1]
            localmem_write1(r_tmp2, r_tmp, CRYPTO_LM_COMM_JUMBO_WRBK_REM_LEN)
            move(r_pkt_wrbk_len, r_tmp1) // amount we have to write
            alu[r_tmp1, r_tmp1, AND, 7]  // write out 1-7 extra bytes
            .if ( r_tmp1 != 0 )          //  to make 8 byte multiple
                alu[r_pkt_wrbk_len, r_pkt_wrbk_len, +, 8]
                alu[r_pkt_wrbk_len, r_pkt_wrbk_len, AND~, 7]
            .endif
            move(r_pkt_wrbk_end_len, 0) // have no data to write to wrbk_end
        .else
            localmem_write1(0, r_tmp, CRYPTO_LM_COMM_JUMBO_WRBK_REM_LEN)
        .endif


        // if end of packet buffer, write back must end on 8 byte alignment
        .if ( r_pkt_wrbk_end_len > 0 )

            #if ( ENABLE_ERROR_CHECKS == 1 )
            alu[r_tmp1, r_pkt_wrbk_len, AND, 7]
            alu[r_tmp1, 8, -, r_tmp1]
            alu[r_tmp1, r_tmp1, AND, 7] // need in case of already 8 byte aligned
            alu[r_tmp, CRYPTO_REQ_PKT_STRT_ADDR, AND, 7]
           .if ( r_tmp != r_tmp1 )
                crypto_threads_input_send_response( _me_ctx, r_buf, \
                                       CRYPTO_RSP_STATUS_WRITE_BACK_LENGTH_ERR )
                br[crypto_input_thread_loop#]
            .endif
            #endif

            // handle encrypt case where end of packet buffer only contains
            // authentication data. adjust r_pkt_wrbk_end_len so that no dma
            // will be started for the end of packet area. ( output thread writes
            // auth data using non-dma out )
            .if ( r_dir ==  CRYPTO_SA_DIR_ENCRYPT )
                alu[r_tmp1, CRYPTO_SA_HASH_MASK, AND, \
                     r_sa_ctlwrd, >>CRYPTO_SA_HASH_SHIFT]
                .if ( r_tmp1 != CRYPTO_SA_HASH_NONE )
                    // Get the ICV length in the packet
                    crypto_threads_get_icv_len(r_icv_len, r_tmp1)
                    .if ( r_pkt_wrbk_end_len <= r_icv_len )
                        move(r_pkt_wrbk_end_len, 0)
                    .endif
                .endif
            .endif

        .else // r_pkt_wrbk_end_len <= 0

            // if pkt write back address not 8 byte aligned, dma has to start at
            // next 8 byte aligned address, so reduce dma length
            alu[r_tmp1, CRYPTO_REQ_PKT_STRT_ADDR, AND, 7]
            .if ( r_tmp1 != 0 )
                alu[r_pkt_wrbk_len, r_pkt_wrbk_len, -, 8]
                alu[r_pkt_wrbk_len, r_pkt_wrbk_len, +, r_tmp1]
            .endif

            // handle case where transfer ends in writeback area
            // and does not end on 8 byte alignment
            alu[r_tmp1, r_pkt_wrbk_len, +, r_pkt_wrbk_addr]
            alu[r_tmp1, r_tmp1, AND, 7]
            .if ( r_tmp1 != 0 )
                alu[r_pkt_wrbk_len, r_pkt_wrbk_len, +, 8]
            .endif

        .endif


        // if pkt write back address not 8 byte aligned, dma has to skip
        // 1st non-8 byte aligned bytes
        alu[r_tmp, CRYPTO_REQ_PKT_STRT_ADDR, AND, 7]
        .if ( r_tmp != 0 )
            alu[r_pkt_wrbk_len, r_pkt_wrbk_len, AND~, 7]
        .endif


      .else

        //
        // decrypt option '1'. writes back only the decrypted plain text data
        //

        // figure starting address in sram
        crypto_threads_get_pkt_addr(r_sram_pkt, _crypto_ctx, r_buf)

        // advance to 1st byte of packet in sram ( for non 8-byte alignment )
        alu[r_tmp, CRYPTO_REQ_PKT_STRT_ADDR, AND, 7]
        alu[r_sram_pkt, r_sram_pkt, +, r_tmp]

        // advance to start of text offset, skipping over fields prior to
        // decrypted text
        ld_field_w_clr[r_tmp, 0011, CRYPTO_REQ_PTEXT_OFFSET, \
                                >>CRYPTO_REQ_PTEXT_OFFSET_SHIFT]
        alu[r_sram_pkt, r_sram_pkt, +, r_tmp]


        alu[r_pkt_wrbk_addr, --, B, CRYPTO_REQ_PKT_WRBK_ADDR]
        // for transport mode, mem transfer begins at auth hdr offset
        alu[r_tmp, CRYPTO_SA_PROTMODE_MASK, AND, r_sa_ctlwrd, \
                                  >>CRYPTO_SA_PROTMODE_SHIFT]
        .if ( r_tmp == CRYPTO_SA_PROTMODE_TRANSPORT)
            ld_field_w_clr[r_tmp, 0011, CRYPTO_REQ_AUTH_HDR_OFFSET, \
                                     >>CRYPTO_REQ_AUTH_HDR_OFFSET_SHIFT]
            alu[r_pkt_wrbk_addr, r_pkt_wrbk_addr, +, r_tmp]
        .endif


        // make sure write back address is aligned the same as plain text address
        alu[r_tmp, r_sram_pkt, AND, 7]
        alu[r_tmp1, r_pkt_wrbk_addr, AND, 7]
        #if ( ENABLE_ERROR_CHECKS == 1 )
        .if ( r_tmp1 != r_tmp )
            crypto_threads_input_send_response( _me_ctx, r_buf, \
                       CRYPTO_RSP_STATUS_WRITE_BACK_ALIGN_ERR )
            br[crypto_input_thread_loop#]
        .endif
        #endif

        // if pkt write back address not 8 byte aligned, dma has to start at
        // next 8 byte aligned address
        .if ( r_tmp1 != 0 )
            alu[r_pkt_wrbk_addr, r_pkt_wrbk_addr, +, 8]
            alu[r_pkt_wrbk_addr, r_pkt_wrbk_addr, -, r_tmp1]

            // and same thing for the sram address, which we already know is
            // aligned the same
            alu[r_sram_pkt, r_sram_pkt, +, 8]
            alu[r_sram_pkt, r_sram_pkt, -, r_tmp1]
        .endif


        // figure the write back length and make sure the alignment matches the
        // address
        ld_field_w_clr[r_pkt_wrbk_len, 0011, CRYPTO_REQ_PKT_WRBK_LENGTH, \
                                          >>CRYPTO_REQ_PKT_WRBK_LENGTH_SHIFT]


        // for transport mode, mem transfer begins at auth hdr offset
        // but the pkt_wrbk_len param in the input cmd includes the
        // full packet data starting from the mac header. So subtract
        // the length up to the auth hdr offset
        alu[r_tmp1, CRYPTO_SA_PROTMODE_MASK, AND, r_sa_ctlwrd, \
                                  >>CRYPTO_SA_PROTMODE_SHIFT]
        .if ( r_tmp1 == CRYPTO_SA_PROTMODE_TRANSPORT)
            ld_field_w_clr[r_tmp1, 0011, CRYPTO_REQ_AUTH_HDR_OFFSET, \
                                     >>CRYPTO_REQ_AUTH_HDR_OFFSET_SHIFT]
            alu[r_pkt_wrbk_len, r_pkt_wrbk_len, -, r_tmp1]
        .endif


        // if the write back length in the request is greater than the total
        // amount of data we have to write back, adjust the write back length.
        // save the remaining wrt back addr and length and it will be written
        // with the next buffer in the output thread. this can happen with
        // 1st buffer of a jumbo packet. for subsequent buffers of a jumbo
        // packet this code is skipped over, above.
        //
        // in these cases ( decrypt option 1 ) the write back length
        // only includes the decrypted data, after adjustment for transport
        .if ( r_pkt_wrbk_len > r_ptext_size )

             // figure out remaining wrt bk starting addr
             alu[r_tmp1, CRYPTO_SA_PROTMODE_MASK, AND, r_sa_ctlwrd, \
                                  >>CRYPTO_SA_PROTMODE_SHIFT]
            .if ( r_tmp1 == CRYPTO_SA_PROTMODE_TRANSPORT)
                ld_field_w_clr[r_tmp2, 0011, CRYPTO_REQ_AUTH_HDR_OFFSET, \
                                     >>CRYPTO_REQ_AUTH_HDR_OFFSET_SHIFT]
                alu[r_tmp2, r_tmp2, +, CRYPTO_REQ_PKT_WRBK_ADDR]
                alu[r_tmp2, r_tmp2, +, r_ptext_size]
            .else
                alu[r_tmp2, CRYPTO_REQ_PKT_WRBK_ADDR, +, r_ptext_size]
            .endif
            crypto_threads_get_lm_temp_addr(r_tmp1, _me_ctx, 0)
            localmem_write1(r_tmp2, r_tmp1, CRYPTO_LM_COMM_JUMBO_WRBK_REM_ADDR)

            // figure out remaining wrt bk length
            alu[r_tmp2, r_pkt_wrbk_len, -, r_ptext_size]
            localmem_write1(r_tmp2, r_tmp1, CRYPTO_LM_COMM_JUMBO_WRBK_REM_LEN)

            // adjust wrt bk len
            move(r_pkt_wrbk_len, r_ptext_size) // amount we have to write
            alu[r_tmp2, r_ptext_size, AND, 7]  // write out 1-7 extra bytes to
            .if ( r_tmp2 != 0 )                //  make an 8 byte multiple
                alu[r_pkt_wrbk_len, r_pkt_wrbk_len, +, 8]
                alu[r_pkt_wrbk_len, r_pkt_wrbk_len, AND~, 7]
            .endif
            move(r_pkt_wrbk_end_len, 0) // have no data to write to wrbk_end
        .else
            crypto_threads_get_lm_temp_addr(r_tmp1, _me_ctx, 0)
            localmem_write1(0, r_tmp1, CRYPTO_LM_COMM_JUMBO_WRBK_REM_LEN)
        .endif


        // if end of packet buffer, write back must end on 8 byte alignment
        #if ( ENABLE_ERROR_CHECKS == 1 )
        .if ( r_pkt_wrbk_end_len > 0 )
            alu[r_tmp1, r_pkt_wrbk_len, AND, 7]
            alu[r_tmp1, 8, -, r_tmp1]
            alu[r_tmp1, r_tmp1, AND, 7] // need in case of already 8 byte aligned
            .if ( r_tmp != r_tmp1 )
                crypto_threads_input_send_response( _me_ctx, r_buf, \
                        CRYPTO_RSP_STATUS_WRITE_BACK_LENGTH_ERR )
                br[crypto_input_thread_loop#]
            .endif
        .endif
        #endif


        // handle case where transfer ends in writeback area
        // and does not end on 8 byte alignment
        .if ( r_pkt_wrbk_end_len == 0 )
            alu[r_tmp1, r_pkt_wrbk_len, +, r_pkt_wrbk_addr]
            alu[r_tmp1, r_tmp1, AND, 7]
            .if ( r_tmp1 != 0 )
                alu[r_pkt_wrbk_len, r_pkt_wrbk_len, +, 8]
            .endif
        .endif


        // if pkt write back address not 8 byte aligned, dma has to skip
        // 1st non-8 byte aligned bytes
        .if ( r_tmp != 0 )
            alu[r_pkt_wrbk_len, r_pkt_wrbk_len, AND~, 7]
        .endif

      .endif

    .else
        // shouldn't get here now due to check_sa. removing to save code/regs
        /*
        crypto_threads_input_send_response(  _me_ctx, r_buf, \
                                      CRYPTO_RSP_STATUS_SA_SELECTION_ERR )
        br[crypto_input_thread_loop#]
        */

    .endif

    #define r_pkt_wrbk_addr_hi r_tmp
    ld_field_w_clr[r_pkt_wrbk_addr_hi, 0001, CRYPTO_REQ_PKT_WRBK_ADDR_UPPER, \
                                         >>CRYPTO_REQ_PKT_WRBK_ADDR_UPPER_SHIFT]

    // finally, generate the dma out for the 1st part of the packet

    /* To be fixed: for unknown reasons, the following debug lines  had to be
       left in the code in order to make all the jumbo permutations work.
       This failed without them:
       ./packet_loopback -i eth6 -p esp -e 1 -n 1 -l 1977 -m transport \
         -c aes256 -y ctr -h gf128_128 -f icmp -k 1 */
    cls_debug_dump1(0x3f0, r_sram_pkt)
    cls_debug_dump1(0x3f4, r_pkt_wrbk_addr)
    cls_debug_dump1(0x3f8, r_pkt_wrbk_len)

    #if ( ENABLE_JUMBO_PKTS == 0 )
    #pragma warning(disable:4701)
    #endif
    .if ( r_pkt_wrbk_end_len > 0 )
        crypto_dma_out ($dma_pkt_wrbk_out, _crypto_ctx, \
                         CHAINED, CHAINED_NOT_LAST, \
                         r_sram_pkt, r_pkt_wrbk_addr, r_pkt_wrbk_addr_hi, \
                         r_pkt_wrbk_len, *nosend*)
    .else
        crypto_dma_out ($dma_pkt_wrbk_out, _crypto_ctx, CHAINED, CHAINED_LAST, \
                         r_sram_pkt, r_pkt_wrbk_addr, r_pkt_wrbk_addr_hi, \
                         r_pkt_wrbk_len, *nosend*)
    .endif
    #if ( ENABLE_JUMBO_PKTS == 0 )
    #pragma warning(default:4701)
    #endif
    #undef r_pkt_wrbk_addr_hi


    .if ( r_pkt_wrbk_end_len > 0 )

        // br point for jumbo handling
        crypto_input_thread_dma_wrback_end#:

        // split packet, do two dma outs
        #if ( ENABLE_JUMBO_PKTS == 0 )
        #pragma warning(disable:4701)
        #endif
        move(r_pkt_end_addr, CRYPTO_REQ_PKT_END_ADDR)
        alu[r_sram_pkt, r_sram_pkt, +, r_pkt_wrbk_len]
        #if ( ENABLE_JUMBO_PKTS == 0 )
        #pragma warning(default:4701)
        #endif

        #if ( ENABLE_JUMBO_PKTS == 1 )
            // if processing a jumbo packet, determine writeback end of
            // packet address and length, and starting sram address
            crypto_threads_get_lm_temp_addr(r_tmp, _me_ctx, 0)
            localmem_read1(r_tmp1, r_tmp, CRYPTO_LM_COMM_JUMBO_STATE)

            .if ( r_tmp1 == CRYPTO_LM_JUMBO_STATE_FIRST_BUF)
                // r_sram_pkt and r_pkt_end_addr stay as is

                // figure total packet bytes in this buffer
                // (adjusted for max buffer size already via ptext_size)
                // if decrypt option 1, only decrypted data is xfrd
                .if (CRYPTO_REQ_DECRYPT_OPTION & \
                        (1 << CRYPTO_REQ_DECRYPT_OPTION_BIT))
                    move(r_tmp, r_ptext_size)
                .else // otherwise, transferring all data
                    ld_field_w_clr[r_tmp, 0011, CRYPTO_REQ_PTEXT_OFFSET, \
                                >>CRYPTO_REQ_PTEXT_OFFSET_SHIFT]
                    alu[r_tmp, r_tmp, +, r_ptext_size]
                .endif

                // wrbk end length is total, less how many transfered
                // in writeback area. N.B. if we are here, it means
                // we wrote the full amount requested into the write back
                // area.
                ld_field_w_clr[r_tmp1, 0011, CRYPTO_REQ_PKT_WRBK_LENGTH, \
                                    >>CRYPTO_REQ_PKT_WRBK_LENGTH_SHIFT]
                alu[r_pkt_wrbk_end_len, r_tmp, -, r_tmp1]

                // for ESP Transport Decrypt Option 1, CRYPTO_REQ_PKT_WRBK_LENGTH
                // includes MAC and IP hdr lengths, have to remove them
                .if (r_dir ==  CRYPTO_SA_DIR_DECRYPT)
                    .if (CRYPTO_REQ_DECRYPT_OPTION & \
                          (1 << CRYPTO_REQ_DECRYPT_OPTION_BIT))
                        alu[r_tmp2, CRYPTO_SA_PROTOCOL_MASK, AND, r_sa_ctlwrd, \
                              >>CRYPTO_SA_PROTOCOL_SHIFT]
                        .if (r_tmp2 == CRYPTO_SA_PROTOCOL_ESP)
                            alu[r_tmp2, CRYPTO_SA_PROTMODE_MASK, AND, r_sa_ctlwrd,\
                                 >>CRYPTO_SA_PROTMODE_SHIFT]
                            .if (r_tmp2 == CRYPTO_SA_PROTMODE_TRANSPORT)
                                ld_field_w_clr[r_tmp1, 0011, \
                                          CRYPTO_REQ_AUTH_HDR_OFFSET, \
                                          >>CRYPTO_REQ_AUTH_HDR_OFFSET_SHIFT]
                                alu[r_pkt_wrbk_end_len, r_pkt_wrbk_end_len, +, r_tmp1]
                            .endif
                        .endif
                    .endif
                .endif

                // save the writeback end offset for next buffer
                crypto_threads_get_lm_temp_addr(r_tmp, _me_ctx, 0)
                localmem_write1(r_pkt_wrbk_end_len, r_tmp, \
                        CRYPTO_LM_COMM_JUMBO_PKT_WRBK_END_OFF)


                // if non 8 byte multiple length, round up, so all
                // bytes transferred. extra bytes transferred will
                // be overwritten by jumbo continue or end transfer
                alu[r_tmp, r_pkt_wrbk_end_len, AND, 7]
                .if ( r_tmp )
                    alu[r_pkt_wrbk_end_len, r_pkt_wrbk_end_len, +, 8]
                    alu[r_pkt_wrbk_end_len, r_pkt_wrbk_end_len, AND~, 7]
                .endif

            .elif ( (r_tmp1 == CRYPTO_LM_JUMBO_STATE_CONTINUE_BUF) \
                    || ( r_tmp1 == CRYPTO_LM_JUMBO_STATE_END_BUF) )

                // transfer starts from beginning of sram
                crypto_threads_get_pkt_addr(r_sram_pkt, _crypto_ctx, r_buf)

                // in case the write back (i.e. ctm) area wasn't completely
                // written during the previous buffer, the start of this
                // buffer will be written to the end of the write back area
                // in the output thread. so skip past this area for the dma out
                localmem_read1(r_tmp2, \
                       r_tmp, CRYPTO_LM_COMM_JUMBO_WRBK_REM_LEN)
                // add in the skip over length
                alu[r_sram_pkt, r_sram_pkt, +, r_tmp2]


                // but if not 8-byte aligned start addr, advance to next
                // 8 byte block; the first non-byte aligned bytes will be
                // transferred after the dma by the output thread
                .if ( r_sram_ptext & 7 )
                    alu[r_sram_pkt, r_sram_pkt, +, 8]
                .endif

                .if (r_tmp1 == CRYPTO_LM_JUMBO_STATE_CONTINUE_BUF)

                    // in case the write back (i.e. ctm) area wasn't completely
                    // written during the previous buffer, the start of this
                    // buffer will be written to the end of the write back area
                    // in the output thread. so the dma out will start at the
                    // beginning of the end of packet buffer
                    .if (r_tmp2 )
                        // don't adjust r_pkt_end_addr

                        // current wrbk end offset is 0, for use below
                        alu[r_tmp1, --, B, 0]

                        // transfer length is ptext size for this buffer, less
                        // the amount written to write back area
                        alu[r_pkt_wrbk_end_len, r_ptext_size, -, r_tmp2]

                        // clear the writeback remaining length, done with it now
                        localmem_write1(0, r_tmp, \
                                        CRYPTO_LM_COMM_JUMBO_WRBK_REM_LEN)
                    .else
                        // advance memory address by working writeback offset
                        localmem_read1(r_tmp1, r_tmp, \
                                CRYPTO_LM_COMM_JUMBO_PKT_WRBK_END_OFF)
                        alu[r_pkt_end_addr, r_pkt_end_addr, +, r_tmp1]

                        // transfer length is ptext size for this buffer
                        alu[r_pkt_wrbk_end_len, --, B, r_ptext_size]
                    .endif


                    // save starting memory address in lm for
                    // output thread to use to copy un-aligned
                    // bytes at start of buffer
                    crypto_threads_get_lm_temp_addr(r_tmp, _me_ctx, r_buf)
                    localmem_write1(r_pkt_end_addr, r_tmp, \
                            CRYPTO_LM_JUMBO_BUF_END_ADDR)


                    // if non 8 byte aligned, advance to next 8 byte
                    // block, output thread will write back the 1st
                    // non 8 byte aligned bytes
                    .if ( r_pkt_end_addr & 7 )
                        alu[r_pkt_end_addr, r_pkt_end_addr, +, 8]
                        alu[r_pkt_end_addr, r_pkt_end_addr, AND~, 7]
                    .endif

                    // save updated working writeback offset
                    crypto_threads_get_lm_temp_addr(r_tmp, _me_ctx, 0)
                    alu[r_tmp1, r_tmp1, +, r_pkt_wrbk_end_len]
                    localmem_write1(r_tmp1, r_tmp, \
                            CRYPTO_LM_COMM_JUMBO_PKT_WRBK_END_OFF)

                    // if non 8 byte multiple length, round up
                    alu[r_tmp, r_pkt_wrbk_end_len, AND, 7]
                    .if ( r_tmp )
                        alu[r_pkt_wrbk_end_len, r_pkt_wrbk_end_len, +, 8]
                    .endif

                .else // ( r_tmp1 == CRYPTO_LM_JUMBO_STATE_END_BUF)

                    // in case the write back (i.e. ctm) area wasn't completely
                    // written during the previous buffer, the start of this
                    // buffer will be written to the end of the write back area
                    // in the output thread. so the dma out will start at the
                    // beginning of the end of packet buffer
                    .if (r_tmp2 )
                        // don't adjust r_pkt_end_addr

                        // current wrbk end offset is 0, for use below
                        alu[r_tmp1,--,B,0]

                        // clear the writeback remaining length, done with it now
                        localmem_write1(0, r_tmp, \
                            CRYPTO_LM_COMM_JUMBO_WRBK_REM_LEN)
                    .else
                        // advance memory address by working writeback offset
                        localmem_read1(r_tmp1, r_tmp, \
                                CRYPTO_LM_COMM_JUMBO_PKT_WRBK_END_OFF)
                        alu[r_pkt_end_addr, r_pkt_end_addr, +, r_tmp1]
                    .endif


                    // save starting memory address in lm for
                    // output thread to use to copy un-aligned
                    // bytes at start of buffer
                    crypto_threads_get_lm_temp_addr(r_tmp, _me_ctx, r_buf)
                    localmem_write1(r_pkt_end_addr, r_tmp, \
                            CRYPTO_LM_JUMBO_BUF_END_ADDR)


                    // if non 8 byte aligned, advance to next 8 byte
                    // block, output thread will write back the 1st
                    // non 8 byte aligned bytes
                    .if ( r_pkt_end_addr & 7 )
                        alu[r_pkt_end_addr, r_pkt_end_addr, +, 8]
                        alu[r_pkt_end_addr, r_pkt_end_addr, AND~, 7]
                    .endif


                    // length is remainder of end length
                    ld_field_w_clr[r_pkt_wrbk_end_len, 0011, \
                                    CRYPTO_REQ_PKT_WRBK_END_LENGTH, \
                                    >>CRYPTO_REQ_PKT_WRBK_END_LENGTH_SHIFT]
                    alu[r_pkt_wrbk_end_len, r_pkt_wrbk_end_len, -, r_tmp1]

                    // for encrypt, write back length includes ICV, remove it
                    .if ( r_dir == CRYPTO_SA_DIR_ENCRYPT )
                        alu[r_tmp, CRYPTO_SA_HASH_MASK, AND, \
                              r_sa_ctlwrd, >>CRYPTO_SA_HASH_SHIFT]
                       .if ( r_tmp != CRYPTO_SA_HASH_NONE )
                           crypto_threads_get_icv_len(r_icv_len, r_hash)
                           alu[r_pkt_wrbk_end_len, r_pkt_wrbk_end_len, -, r_icv_len]
                       .endif
                    .endif

                    // if non 8 byte multiple length, round up
                    alu[r_tmp, r_pkt_wrbk_end_len, AND, 7]
                    .if ( r_tmp )
                        alu[r_pkt_wrbk_end_len, r_pkt_wrbk_end_len, +, 8]
                    .endif

                    // finished with this packet in input thread, reset jumbo state
                    crypto_threads_get_lm_temp_addr(r_tmp, _me_ctx, 0)
                    localmem_write1(CRYPTO_LM_JUMBO_STATE_NON_JUMBO, \
                            r_tmp, CRYPTO_LM_COMM_JUMBO_STATE)
                .endif

                // for jumbo continue or end, we didn't do a writeback
                // transfer. so need to use the writeback and writeback end
                // transfer regs for the writeback end and signal cmds

                crypto_dma_out ($dma_pkt_wrbk_out, _crypto_ctx, \
                                 CHAINED, CHAINED_LAST, \
                                 r_sram_pkt, r_pkt_end_addr, \
                                 r_pkt_end_addr_hi, r_pkt_wrbk_end_len, *nosend*)

                #define r_island r_tmp1
                #define r_ppid7 r_tmp2
                crypto_get_island_and_PPID7 (r_island, r_ppid7)
                alu[r_tmp, _me_ctx, +, 1] // send sig to corresponding output thread
                .if ( r_buf == 0 )
                    crypto_send_done_signal ($dma_pkt_end_out, _crypto_ctx, r_tmp, \
                                    CHAINED, r_island, r_ppid7, buf_sig_A, *nosend*)
                .elif ( r_buf == 1 )
                    crypto_send_done_signal ($dma_pkt_end_out, _crypto_ctx, r_tmp, \
                                    CHAINED, r_island, r_ppid7, buf_sig_B, *nosend*)
                .elif ( r_buf == 2 )
                    crypto_send_done_signal ($dma_pkt_end_out, _crypto_ctx, r_tmp, \
                                    CHAINED, r_island, r_ppid7, buf_sig_C, *nosend*)
                .elif ( r_buf == 3 )
                    crypto_send_done_signal ($dma_pkt_end_out, _crypto_ctx, r_tmp, \
                                    CHAINED, r_island, r_ppid7, buf_sig_D, *nosend*)
                .endif
                #undef r_island
                #undef r_ppid7

                #define_eval _xfer_len 4
                #pragma warning(disable:5151) // not sure why nfas warns here..
                crypto_send (write_fifo, $runcmd, --, 0, _xfer_len, --)
                #pragma warning(default:5151)
                crypto_threads_update_counter(CRYPTO_CNTR_CMD_SEND_2)

                br[crypto_input_thread_next_buf#]

            .else // not handling a jumbo
                // if non 8 byte multiple length, round up
                alu[r_tmp, r_pkt_wrbk_end_len, AND, 7]
                .if ( r_tmp )
                    alu[r_pkt_wrbk_end_len, r_pkt_wrbk_end_len, +, 8]
                .endif
            .endif
        #else
        // if non 8 byte multiple length, round up
        alu[r_tmp, r_pkt_wrbk_end_len, AND, 7]
        .if ( r_tmp )
            alu[r_pkt_wrbk_end_len, r_pkt_wrbk_end_len, +, 8]
        .endif
        #endif

        crypto_dma_out ($dma_pkt_end_out, _crypto_ctx, CHAINED, CHAINED_LAST, \
                         r_sram_pkt, r_pkt_end_addr, \
                         r_pkt_end_addr_hi, r_pkt_wrbk_end_len, *nosend*)

        #define r_island r_tmp1
        #define r_ppid7 r_tmp2
        crypto_get_island_and_PPID7 (r_island, r_ppid7)
        alu[r_tmp, _me_ctx, +, 1] // send sig to corresponding output thread
        .if ( r_buf == 0 )
            crypto_send_done_signal ($int_sig_msg, _crypto_ctx, r_tmp, CHAINED, \
                                      r_island, r_ppid7, buf_sig_A, *nosend*)
        .elif ( r_buf == 1 )
            crypto_send_done_signal ($int_sig_msg, _crypto_ctx, r_tmp, CHAINED, \
                                      r_island, r_ppid7, buf_sig_B, *nosend*)
        .elif ( r_buf == 2 )
            crypto_send_done_signal ($int_sig_msg, _crypto_ctx, r_tmp, CHAINED, \
                                      r_island, r_ppid7, buf_sig_C, *nosend*)
        .elif ( r_buf == 3 )
            crypto_send_done_signal ($int_sig_msg, _crypto_ctx, r_tmp, CHAINED, \
                                      r_island, r_ppid7, buf_sig_D, *nosend*)
        .endif
        #undef r_island
        #undef r_ppid7

        // send the crypto sequence into the crypto input fifo. per EAS,
        // will block until sufficient space available in input fifo
        #define_eval _xfer_len 6
        #pragma warning(disable:5151) // not sure why nfas warns here..
        #if ( ENABLE_JUMBO_PKTS == 0 )
        #pragma warning(disable:4701)
        #endif
        crypto_send (write_fifo, $runcmd, --, 0, _xfer_len, --)
        #if ( ENABLE_JUMBO_PKTS == 0 )
        #pragma warning(default:4701)
        #endif
        #pragma warning(default:5151)
        crypto_threads_update_counter(CRYPTO_CNTR_CMD_SEND_3)

    .else

        // not a split packet, use the 2nd dma out xfr regs for the done sig cmd
        #define r_island r_tmp1
        #define r_ppid7 r_tmp2
        crypto_get_island_and_PPID7 (r_island, r_ppid7)
        alu[r_tmp, _me_ctx, +, 1] // send sig to corresponding output thread
        .if ( r_buf == 0 )
            crypto_send_done_signal ($dma_pkt_end_out, _crypto_ctx, r_tmp, \
                               CHAINED, r_island, r_ppid7, buf_sig_A, *nosend*)
        .elif ( r_buf == 1 )
            crypto_send_done_signal ($dma_pkt_end_out, _crypto_ctx, r_tmp, \
                               CHAINED, r_island, r_ppid7, buf_sig_B, *nosend*)
        .elif ( r_buf == 2 )
            crypto_send_done_signal ($dma_pkt_end_out, _crypto_ctx, r_tmp, \
                               CHAINED, r_island, r_ppid7, buf_sig_C, *nosend*)
        .elif ( r_buf == 3 )
            crypto_send_done_signal ($dma_pkt_end_out, _crypto_ctx, r_tmp, \
                               CHAINED, r_island, r_ppid7, buf_sig_D, *nosend*)
        .endif
        #undef r_island
        #undef r_ppid7

        #define_eval _xfer_len 4
        #pragma warning(disable:5151) // not sure why nfas warns here..
        #if ( ENABLE_JUMBO_PKTS == 0 )
        #pragma warning(disable:4701)
        #endif
        crypto_send (write_fifo, $runcmd, --, 0, _xfer_len, --)
        #if ( ENABLE_JUMBO_PKTS == 0 )
        #pragma warning(default:4701)
        #endif
        #pragma warning(default:5151)
        crypto_threads_update_counter(CRYPTO_CNTR_CMD_SEND_4)
    .endif

    // end of tell compiler scope of $runcmd, $dma_pkt_wrbk_out, etc.
    .end


    // next buffer
    crypto_input_thread_next_buf#:
    CRYPTO_PROFILE_ENTRY(0xf6)
    #define_eval _MAX_BUF ( CRYPTO_NUM_BUFS - 1 )
    .if ( r_buf == _MAX_BUF )
        immed[r_buf, 0]
    .else
        alu[r_buf, r_buf, +, 1]
    .endif


    // if this was an aes-gcm null cipher, skip past
    // the next buffer as well; we used it to write out
    // cipher data we didn't want in the output packet
    .if ((r_cipher >= CRYPTO_SA_CIPHER_AES128_NULL) && \
             (r_cipher <= CRYPTO_SA_CIPHER_AES256_NULL))
        .if ( r_buf == _MAX_BUF )
            immed[r_buf, 0]
        .else
            alu[r_buf, r_buf, +, 1]
        .endif
    .endif


    br[crypto_input_thread_loop#]


    // subroutine for send response to reduce code size
    // invoked via helper macro crypto_threads_input_send_response
    #if ( SEND_RESPONSE_SUBROUTINE == 1 )
    .subroutine
    send_response_sub#:
        crypto_threads_send_response( _me_ctx, r_buf, r_stat)
        crypto_threads_reenable_buf(r_buf) // reuse this buffer
        rtn[r_rtn]
    .endsub
    #endif

    .end

#endm


/**
 *  Crypto output thread
 *
 *  @param  _me_ctx       GPR, me context of this thread
 *  @param  _crypto_ctx   GPR, crypto core context to be used by this thread.
 *                        One context is used per core, so context will be equal
 *                        to the core # in use by this thread
 *
 *  Output thread dedicated to a single crypto context / core. Typically run on
 *  odd #'d thread, i.e. me threads 1,3,5,7, while corresponding input thread is
 *  run on even #'d threads, i.e. me threads 0,2,4,6.
 *
 *  See above re crypto_threads_input for buffer utilization description
 */
#macro crypto_threads_output(_me_ctx, _crypto_ctx)

    .begin

    //
    // Thread processing is as follows:
    // 1) wait for signal from crypto core indicating next buffer is processed
    // 2) if decrypt:
    //   a) write non-8 byte aligned start of packet bytes into memory
    //   b) compare calculated ICV / authentication data to received value
    //   c) increment stats
    //   d) write output status into status ring
    //   e) free start of packet buffer if requested in request
    //   f) go to step 1
    // 3) if encrypt:
    //   a) write non-8 byte aligned start of packet bytes into memory
    //   b) transfer ICV / authentication data from sram to memory
    //   c) increment stats
    //   d) write output status into status ring
    //   e) free start of packet buffer if requested in request
    //   f) go to step 1


    .reg r_buf, r_tmp, r_tmp1, r_sram_pkt_addr, r_status
    .reg r_pkt_addr_upper1, r_pkt_addr_upper2, r_sa_addr, r_pkt_strt_addr
    .reg r_pkt_cont_addr, r_pkt_end_addr, r_pkt_wrbk_addr
    .reg r_pkt_lengths1, r_pkt_lengths2, r_pkt_lengths3
    .reg r_pkt_offsets1, r_pkt_offsets2, r_sa_ctlwrd
    .reg r_user_ctl, r_seq_upper, r_dir
    .reg r_addr_lo, r_addr_hi, r_pkt_wrbk_len, r_pkt_wrbk_end_len
    .reg r_auth_hdr_offset, r_auth_data_offset
    .reg r_sram_hash_addr, r_sram_pkt_auth_addr
    .reg r_protocol, r_cipher, r_hash, r_icv_len

     #if ( ENABLE_JUMBO_PKTS == 1 )
    .reg r_jumbo_state
     #endif


    .reg $xfr[2]
    .xfer_order $xfr
    .sig s

    .sig volatile buf_sig_A, buf_sig_B, buf_sig_C, buf_sig_D
    .addr buf_sig_A 8
    .addr buf_sig_B 9
    .addr buf_sig_C 10
    .addr buf_sig_D 11

    .set_sig buf_sig_A
    .set_sig buf_sig_B
    .set_sig buf_sig_C
    .set_sig buf_sig_D


    crypto_threads_update_counter(CRYPTO_CNTR_OUTPUT_THREAD_FLAG)

    // start with buffer A
    immed[r_buf, 0]



    //
    // output processing loop
    //

    crypto_output_thread_loop#:


    // wait for next buffer to be ready. sigs 8,9,10,11, for buf_sig_A,B,C,D
    // are set when crypto core is done with respective buffer
    move(r_tmp, 0x100)
    alu[--, r_buf, OR, 0]
    alu[r_tmp, --, B, r_tmp, <<indirect]
    ctx_arb[--], defer[1]
    local_csr_wr[ACTIVE_CTX_WAKEUP_EVENTS, r_tmp]

    CRYPTO_PROFILE_ENTRY(0xf7)

    crypto_threads_update_counter(CRYPTO_CNTR_OUTPUT_PKT_SIG)

    move(r_status, CRYPTO_RSP_STATUS_OK) // default status to OK


    // move data in local memory to GPR's
    #pragma warning(disable:5008) // r_sa_addr & r_pkt_cont_addr are not used
    crypto_threads_get_lm_temp_addr(r_tmp, _me_ctx, r_buf)
    localmem_read8(r_pkt_addr_upper1, \
                   r_pkt_addr_upper2, \
                   r_sa_addr, \
                   r_pkt_strt_addr, \
                   r_pkt_cont_addr, \
                   r_pkt_end_addr, \
                   r_pkt_wrbk_addr, \
                   r_pkt_lengths1, \
                   r_tmp, CRYPTO_LM_PKT_WRBK_ADDR_UPPER)
    #pragma warning(default:5008)
    localmem_read8(r_pkt_lengths2, \
                   r_pkt_lengths3, \
                   r_pkt_offsets1, \
                   r_pkt_offsets2, \
                   r_sa_ctlwrd, \
                   r_user_ctl, \
                   r_seq_upper, \
                   r_tmp1, \
                   r_tmp, CRYPTO_LM_PKT_WRBK_LENGTH)

    #if ( ENABLE_JUMBO_PKTS == 1 )
    // read and clear this buffer's jumbo state var
    localmem_read1(r_jumbo_state, \
                   r_tmp, CRYPTO_LM_JUMBO_STATE)
    localmem_write1(CRYPTO_LM_JUMBO_STATE_NON_JUMBO, \
                   r_tmp, CRYPTO_LM_JUMBO_STATE)
    #endif


    alu[r_protocol, CRYPTO_SA_PROTOCOL_MASK, AND, r_sa_ctlwrd, \
                                    >>CRYPTO_SA_PROTOCOL_SHIFT]

    alu[r_dir, CRYPTO_SA_DIR_MASK, AND, r_sa_ctlwrd, >>CRYPTO_SA_DIR_SHIFT]

    alu[r_hash, CRYPTO_SA_HASH_MASK, AND, r_sa_ctlwrd, >>CRYPTO_SA_HASH_SHIFT]


    // for jumbo continue or end buffers...
    #if ( ENABLE_JUMBO_PKTS == 1 )
    .if ( (r_jumbo_state == CRYPTO_LM_JUMBO_STATE_CONTINUE_BUF) \
         || (r_jumbo_state == CRYPTO_LM_JUMBO_STATE_END_BUF ))


        localmem_read1(r_tmp1, \
                       r_tmp, CRYPTO_LM_JUMBO_WRBK_REM_LEN)

        .if (r_tmp1)

            // transfer the start of the sram buffer which was not dma'd
            // because it is going into the 'write back' buffer
            // (i.e. the end of the ctm buffer )

            localmem_read1(r_sram_pkt_addr,  \
                       r_tmp, CRYPTO_LM_JUMBO_BUF_SRAM_PTEXT)

            localmem_read1(r_addr_lo, \
                       r_tmp, CRYPTO_LM_JUMBO_WRBK_REM_ADDR)

            ld_field_w_clr[r_addr_hi, 0001, r_pkt_addr_upper1, \
                              >>CRYPTO_LM_PKT_WRBK_ADDR_UPPER_SHIFT]

            crypto_non_dma_out(r_sram_pkt_addr, r_addr_lo, r_addr_hi, r_tmp1, \
                                                      CRYPTO_NDO_DEFAULT)

        .else

            // transfer start of buffer which was not dma'd into the
            // end of packet buffer due to non-8 byte alignment

            localmem_read1(r_sram_pkt_addr,                 \
                       r_tmp, CRYPTO_LM_JUMBO_BUF_SRAM_PTEXT)

            .if ( r_sram_pkt_addr & 7 ) // was not 8 byte aligned

                localmem_read1(r_tmp1, \
                       r_tmp, CRYPTO_LM_JUMBO_BUF_END_ADDR)

                // length is from 1 to 7 bytes depending on starting address offset
                alu[r_tmp, r_sram_pkt_addr, AND, 7]
                alu[r_tmp, 8, -, r_tmp]
                ld_field_w_clr[r_addr_hi, 0001, r_pkt_addr_upper2, \
                              >>CRYPTO_LM_PKT_END_ADDR_UPPER_SHIFT]
                crypto_non_dma_out(r_sram_pkt_addr, r_tmp1, r_addr_hi, r_tmp, \
                                   CRYPTO_NDO_7B)
            .endif

        .endif

        .if ( r_jumbo_state == CRYPTO_LM_JUMBO_STATE_CONTINUE_BUF )
            br[crypto_output_thread_buf_done#] // done with this buffer
        .elif ( r_jumbo_state == CRYPTO_LM_JUMBO_STATE_END_BUF )
            br[crypto_output_thread_hash_processing#] // move or check hash
        .endif
    .endif
    #endif


    // ESP, either transport or tunnel
    .if ( r_protocol == CRYPTO_SA_PROTOCOL_ESP )

        // transfer start of packet which was not dma'd due to non-8 byte
        // alignment

        // for decrypt option 1, transport mode, mem transfer begins at
        // auth hdr offset
        alu[r_tmp, CRYPTO_SA_PROTMODE_MASK, AND, r_sa_ctlwrd, \
                                      >>CRYPTO_SA_PROTMODE_SHIFT]
        .if ((r_pkt_addr_upper1 & (1 << CRYPTO_REQ_DECRYPT_OPTION_BIT)) && \
             ( r_tmp == CRYPTO_SA_PROTMODE_TRANSPORT))
            ld_field_w_clr[r_auth_hdr_offset, 0011, r_pkt_offsets1, \
                                        >>CRYPTO_LM_AUTH_HDR_OFFSET_SHIFT]
            alu[r_pkt_wrbk_addr, r_pkt_wrbk_addr, +, r_auth_hdr_offset]
        .endif

        alu[r_tmp, r_pkt_wrbk_addr, AND, 7]
        .if ( r_tmp != 0 )
            crypto_threads_get_pkt_addr(r_sram_pkt_addr, _crypto_ctx, r_buf)

            // advance to 1st byte of packet in sram ( for non 8-byte alignment )
            alu[r_tmp1, r_pkt_strt_addr, AND, 7]
            alu[r_sram_pkt_addr, r_sram_pkt_addr, +, r_tmp1]

            // if decrypt option 1..
            .if (r_pkt_addr_upper1 & (1 << CRYPTO_REQ_DECRYPT_OPTION_BIT))
                // advance to start of text offset, skipping over fields prior to
                // decrypted text
                ld_field_w_clr[r_tmp1, 0011, r_pkt_offsets2, \
                                 >>CRYPTO_LM_PTEXT_OFFSET_SHIFT]
                alu[r_sram_pkt_addr, r_sram_pkt_addr, +, r_tmp1]
            .endif

            // length is from 1 to 7 bytes depending on starting address offset
            alu[r_tmp, 8, -, r_tmp]
            ld_field_w_clr[r_addr_hi, 0001, r_pkt_addr_upper1, \
                              >>CRYPTO_LM_PKT_WRBK_ADDR_UPPER_SHIFT]
            crypto_non_dma_out(r_sram_pkt_addr, r_pkt_wrbk_addr, r_addr_hi, \
                                r_tmp, CRYPTO_NDO_7B)
        .endif

    .else
        move(r_status, CRYPTO_RSP_STATUS_SA_SELECTION_ERR )
        br[crypto_output_thread_send_status#]
    .endif



    // alter flow for jumbo packets
    #if ( ENABLE_JUMBO_PKTS == 1 )
    .if ( r_jumbo_state == CRYPTO_LM_JUMBO_STATE_FIRST_BUF )
        br[crypto_output_thread_buf_done#] // done with this buffer
    .endif
    #endif


    crypto_output_thread_hash_processing#:
    CRYPTO_PROFILE_ENTRY(0xf8)

    // encrypt / a.k.a. 'outbound'
    .if ( r_dir == CRYPTO_SA_DIR_ENCRYPT )

        // ESP, either transport or tunnel
        .if ( r_protocol == CRYPTO_SA_PROTOCOL_ESP )
            #if ( ENABLE_DETAILED_COUNTERS == 1 )
            crypto_threads_update_counter(CRYPTO_CNTR_OUTPUT_ENCRYPT_ESP)
            #endif
            move(r_tmp, 1)
        .else
            move(r_tmp, 0)
        .endif

        .if ( r_tmp == 1 )

            .if ( r_hash != CRYPTO_SA_HASH_NONE ) // unless HASH_NONE was selected...
                // transfer icv/auth data from sram to system memory

                ld_field_w_clr[r_auth_data_offset, 0011, r_pkt_offsets2, \
                                         >>CRYPTO_LM_AUTH_DATA_OFFSET_SHIFT]
                ld_field_w_clr[r_pkt_wrbk_len, 0011, r_pkt_lengths2, \
                                          >>CRYPTO_LM_PKT_WRBK_LENGTH_SHIFT]

                crypto_threads_get_icv_len(r_icv_len, r_hash)

                crypto_threads_get_hash_addr(r_sram_hash_addr, _crypto_ctx, r_buf)

                // auth/icv data located entirely in end of packet
                .if ( r_auth_data_offset >= r_pkt_wrbk_len )
                    ld_field_w_clr[r_addr_hi, 0001, r_pkt_addr_upper2, \
                                       >>CRYPTO_LM_PKT_END_ADDR_UPPER_SHIFT]
                    alu[r_addr_lo, r_pkt_end_addr, +, r_auth_data_offset]
                    alu[r_addr_lo, r_addr_lo, -, r_pkt_wrbk_len]
                    .if ( r_icv_len  & 0x3 ) // not even # lwords, do default non dma out
                        crypto_non_dma_out(r_sram_hash_addr, r_addr_lo, r_addr_hi, \
                                                 r_icv_len, CRYPTO_NDO_DEFAULT)
                    .else // otherwise can use the lword non dma out
                        crypto_non_dma_out(r_sram_hash_addr, r_addr_lo, r_addr_hi, \
                                                 r_icv_len, CRYPTO_NDO_16LW)
                    .endif
                .else
                    // handle part of auth data located in packet write back area
                    ld_field_w_clr[r_addr_hi, 0001, r_pkt_addr_upper1, \
                                      >>CRYPTO_LM_PKT_WRBK_ADDR_UPPER_SHIFT]
                    alu[r_addr_lo, r_pkt_wrbk_addr, +, r_auth_data_offset]
                    alu[r_tmp, r_pkt_wrbk_len, -, r_auth_data_offset]
                    limit_min(r_tmp, r_tmp, r_icv_len)
                    .if ( r_tmp & 0x3 ) // not even # lwords, do default non dma out
                        crypto_non_dma_out(r_sram_hash_addr, r_addr_lo, r_addr_hi, \
                                            r_tmp, CRYPTO_NDO_DEFAULT)
                    .else // otherwise can use the lword non dma out
                        crypto_non_dma_out(r_sram_hash_addr, r_addr_lo, r_addr_hi, \
                                            r_tmp, CRYPTO_NDO_16LW)
                    .endif

                    // handle part of auth data located in end of packet area
                    .if ( r_tmp < r_icv_len )
                        // start addr of remainder of icv/auth data
                        alu[r_sram_hash_addr, r_sram_hash_addr, +, r_tmp]
                        // length of remainder of icv/auth data
                        alu[r_tmp, r_icv_len, -, r_tmp]
                        ld_field_w_clr[r_addr_hi, 0001, r_pkt_addr_upper2, \
                                      >>CRYPTO_LM_PKT_END_ADDR_UPPER_SHIFT]
                        .if (( r_tmp & 0x7 ) || ( r_sram_hash_addr & 0x7 ))
                            // if not on 8-byte boundary, or if not
                            // even # lwords, or if not even #
                            // of 8 bytes, ( which makes sram addr
                            // non 8 byte aligned ), have to use
                            // default non-dma out
                            crypto_non_dma_out(r_sram_hash_addr, r_pkt_end_addr, \
                                               r_addr_hi, r_tmp, CRYPTO_NDO_DEFAULT)
                        .else
                            crypto_non_dma_out(r_sram_hash_addr, r_pkt_end_addr, \
                                               r_addr_hi, r_tmp, CRYPTO_NDO_16LW)
                        .endif
                    .endif
                .endif
            .endif
        .else
            move(r_status, CRYPTO_RSP_STATUS_SA_SELECTION_ERR )
        .endif

    // decrypt
    .else

        // ESP transport or tunnel
        .if ( r_protocol == CRYPTO_SA_PROTOCOL_ESP )

            #if ( ENABLE_DETAILED_COUNTERS == 1 )
            crypto_threads_update_counter(CRYPTO_CNTR_OUTPUT_DECRYPT_ESP)
            #endif

            #if ( ENABLE_JUMBO_PKTS == 1 )
            // sram addr of auth data saved by input thread
            .if ( r_jumbo_state == CRYPTO_LM_JUMBO_STATE_END_BUF )
                crypto_threads_get_lm_temp_addr(r_tmp, _me_ctx, r_buf)
                localmem_read1(r_sram_pkt_auth_addr, \
                           r_tmp, CRYPTO_LM_JUMBO_BUF_SRAM_AUTH_DATA)
            .else
            #endif
            crypto_threads_get_pkt_addr(r_sram_pkt_auth_addr, _crypto_ctx, \
                                                                          r_buf)
            alu[r_tmp, r_pkt_strt_addr, AND, 7] // pkt start offset in sram
            alu[r_sram_pkt_auth_addr, r_sram_pkt_auth_addr, +, r_tmp]
            ld_field_w_clr[r_auth_data_offset, 0011, r_pkt_offsets2, \
                                      >>CRYPTO_LM_AUTH_DATA_OFFSET_SHIFT]
            alu[r_sram_pkt_auth_addr, r_sram_pkt_auth_addr, +, \
                                                      r_auth_data_offset]
            #if ( ENABLE_JUMBO_PKTS == 1 )
            .endif
            #endif

            // save next hdr field, which is 1 byte before the
            // auth data, from sram to local memory
            // so send_response can put it into the response
            alu[r_tmp, r_sram_pkt_auth_addr, -, 1]
            crypto_threads_save_nxt_hdr( _me_ctx, r_tmp )


            .if ( r_hash != CRYPTO_SA_HASH_NONE ) // unless HASH_NONE was selected...
                // compare calculated icv/auth data to received value

                crypto_threads_get_hash_addr(r_sram_hash_addr, _crypto_ctx, \
                                                                        r_buf)
                crypto_threads_get_icv_len(r_icv_len, r_hash)
                crypto_threads_compare(r_tmp, r_sram_pkt_auth_addr, \
                                          r_sram_hash_addr, r_icv_len)
                .if ( r_tmp != 0 )
                    move(r_status, CRYPTO_RSP_STATUS_AUTH_CHECK_FAIL )
                .endif
            .endif


            // if decrypt option 1..
            .if (r_pkt_addr_upper1 & (1 << CRYPTO_REQ_DECRYPT_OPTION_BIT))

                // adjust writeback and/or end of packet length to account for
                // removal of padding bytes. ( can't do this until after
                // decryption, so have to do it here )

                // pad length is 2 bytes before authentication data
                // read 8 byte block containing pad length from sram
                alu[r_tmp, r_sram_pkt_auth_addr, -, 2]
                alu[r_tmp1, r_tmp, AND~, 7]
                crypto[read, $xfr[0], 0, r_tmp1, 1], ctx_swap[s]

                // extract the pad length byte
                alu[r_tmp1, r_tmp, AND, 7]
                jump[r_tmp1, epljt#], targets [eplj0#, eplj1#, eplj2#, eplj3#, \
                                               eplj4#, eplj5#, eplj6#, eplj7# ]
                epljt#:
                eplj0#: br[epljt0#]
                eplj1#: br[epljt1#]
                eplj2#: br[epljt2#]
                eplj3#: br[epljt3#]
                eplj4#: br[epljt4#]
                eplj5#: br[epljt5#]
                eplj6#: br[epljt6#]
                eplj7#: br[epljt7#]

                epljt0#:
                    ld_field_w_clr[r_tmp, 0001, $xfr[0], >>24]
                    br[epl_done#]
                epljt1#:
                    ld_field_w_clr[r_tmp, 0001, $xfr[0], >>16]
                    br[epl_done#]
                epljt2#:
                    ld_field_w_clr[r_tmp, 0001, $xfr[0], >>8 ]
                    br[epl_done#]
                epljt3#:
                    ld_field_w_clr[r_tmp, 0001, $xfr[0], >>0 ]
                    br[epl_done#]
                epljt4#:
                    ld_field_w_clr[r_tmp, 0001, $xfr[1], >>24]
                    br[epl_done#]
                epljt5#:
                    ld_field_w_clr[r_tmp, 0001, $xfr[1], >>16]
                    br[epl_done#]
                epljt6#:
                    ld_field_w_clr[r_tmp, 0001, $xfr[1], >>8 ]
                    br[epl_done#]
                epljt7#:
                    ld_field_w_clr[r_tmp, 0001, $xfr[1], >>0 ]

                epl_done#:

                // pad length byte is in r_tmp. Now subtract r_tmp + 2 from
                // end of packet length and/or writeback length. send_response
                // will pick that up from lm and write it into response ring
                alu[r_tmp, r_tmp, +, 2]

                // for tunnel mode, the actual 'writeback' length is
                // sent in the 'continue' length
                alu[r_tmp1, CRYPTO_SA_PROTMODE_MASK, AND, r_sa_ctlwrd, \
                                      >>CRYPTO_SA_PROTMODE_SHIFT]
                .if ( r_tmp1 == CRYPTO_SA_PROTMODE_TUNNEL)
                    ld_field_w_clr[r_pkt_wrbk_len, 0011, r_pkt_lengths1, \
                                          >>CRYPTO_LM_PKT_CONT_LENGTH_SHIFT]
                .else
                    ld_field_w_clr[r_pkt_wrbk_len, 0011, r_pkt_lengths2, \
                                          >>CRYPTO_LM_PKT_WRBK_LENGTH_SHIFT]
                .endif

                ld_field_w_clr[r_pkt_wrbk_end_len, 0011, r_pkt_lengths3, \
                                      >>CRYPTO_LM_PKT_WRBK_END_LENGTH_SHIFT]

                .if ( r_pkt_wrbk_end_len >= r_tmp )

                    // length of pad + 2 is contained within the end of packet
                    // area, so just reduce it from there
                    alu[r_pkt_wrbk_end_len, r_pkt_wrbk_end_len, -, r_tmp]
                    ld_field[r_pkt_lengths3, 1100, r_pkt_wrbk_end_len, \
                                      <<CRYPTO_LM_PKT_WRBK_END_LENGTH_SHIFT]
                    // save it in local mem for crypto_threads_send_response
                    crypto_threads_get_lm_temp_addr(r_tmp, _me_ctx, r_buf)
                    localmem_write1(r_pkt_lengths3, r_tmp, \
                                     CRYPTO_LM_PKT_WRBK_END_LENGTH)

                .elif ( r_pkt_wrbk_end_len )

                    // length of pad + 2 is contained partially  within the end
                    // of packet area, and partially in the writeback area.

                    // subtract out of write back length
                    // what is not included in the end of packet area
                    alu[r_tmp, r_tmp, -, r_pkt_wrbk_end_len]
                    alu[r_pkt_wrbk_len, r_pkt_wrbk_len, -, r_tmp]

                    .if ( r_tmp1 == CRYPTO_SA_PROTMODE_TUNNEL) //tnl mode in cont len
                        ld_field_w_clr[r_pkt_lengths1, 0011, r_pkt_wrbk_len, \
                                          <<CRYPTO_LM_PKT_CONT_LENGTH_SHIFT]
                    .else
                        ld_field_w_clr[r_pkt_lengths2, 0011, r_pkt_wrbk_len, \
                                          <<CRYPTO_LM_PKT_WRBK_LENGTH_SHIFT]
                    .endif

                    // write back end of packet length is now 0
                    move(r_pkt_wrbk_end_len, 0)
                    ld_field[r_pkt_lengths3, 1100, r_pkt_wrbk_end_len, \
                                      <<CRYPTO_LM_PKT_WRBK_END_LENGTH_SHIFT]

                    // save to local mem for crypto_threads_send_response
                    crypto_threads_get_lm_temp_addr(r_tmp, _me_ctx, r_buf)
                    localmem_write3(r_pkt_lengths1, \
                                    r_pkt_lengths2, \
                                    r_pkt_lengths3, \
                                    r_tmp, \
                                    CRYPTO_LM_PKT_CONT_LENGTH)

                .else // length pad + 2 contained entirely in write back area

                    alu[r_pkt_wrbk_len, r_pkt_wrbk_len, -, r_tmp]

                    .if ( r_tmp1 == CRYPTO_SA_PROTMODE_TUNNEL) //tnl mode in cont len
                        ld_field_w_clr[r_pkt_lengths1, 0011, r_pkt_wrbk_len, \
                                          <<CRYPTO_LM_PKT_CONT_LENGTH_SHIFT]
                    .else
                        ld_field_w_clr[r_pkt_lengths2, 0011, r_pkt_wrbk_len, \
                                          <<CRYPTO_LM_PKT_WRBK_LENGTH_SHIFT]
                    .endif


                    // save it in local mem for crypto_threads_send_response
                    crypto_threads_get_lm_temp_addr(r_tmp, _me_ctx, r_buf)
                    localmem_write2(r_pkt_lengths1, \
                                    r_pkt_lengths2, \
                                    r_tmp, \
                                    CRYPTO_LM_PKT_CONT_LENGTH)
                .endif

            .endif

        .else
            move(r_status, CRYPTO_RSP_STATUS_SA_SELECTION_ERR )
        .endif

    .endif


    // do anti-replay processing
    CRYPTO_PROFILE_ENTRY(0xf9)
    #if ( ENABLE_ANTI_REPLAY == 1 )
    .if ( r_status == CRYPTO_RSP_STATUS_OK )
        .if ( r_dir == CRYPTO_SA_DIR_DECRYPT )
            crypto_threads_anti_replay(_me_ctx, r_buf, r_status, 1)
        .endif
    .endif
    #endif


    // send status to response ring
    crypto_output_thread_send_status#:
    CRYPTO_PROFILE_ENTRY(0xfa)
    crypto_threads_send_response( _me_ctx, r_buf, r_status )


    // update prot/protmode/cipher/hash/etc. specific packet counter
    #if ( ENABLE_DETAILED_COUNTERS == 1 )
    crypto_threads_update_pkt_counter(r_sa_ctlwrd)
    #endif

    // branch point for jumbo handling
    crypto_output_thread_buf_done#:

    #if ( ENABLE_JUMBO_PKTS == 1 )
    // for jumbo continue & end buffers, invalidate saved sa addr
    // in lm, because we haven't really read in sa info ( the
    // the keys and sa ctlwrd ), and we don't want the input thread
    // sa re-use code to think the info can be re-used
    .if ( (r_jumbo_state == CRYPTO_LM_JUMBO_STATE_CONTINUE_BUF) \
         || (r_jumbo_state == CRYPTO_LM_JUMBO_STATE_END_BUF ))
        crypto_threads_get_lm_temp_addr(r_tmp, _me_ctx, r_buf)
        move(r_tmp1, 0xffffffff)
        localmem_write1(r_tmp1, r_tmp, CRYPTO_LM_SA_ADDR)
    .endif
    #endif


    // set signal to input thread, done with buffer
    alu[r_tmp, _me_ctx, AND, 0x6] // previous ctx
    alu[r_tmp, r_tmp, OR, r_buf, <<3] // signal = buffer + 8
    alu[r_tmp, r_tmp, +, (8<<3)]
    local_csr_wr[SAME_ME_SIGNAL, r_tmp]


    // next buffer
    CRYPTO_PROFILE_ENTRY(0xfb)
    #define_eval _MAX_BUF ( CRYPTO_NUM_BUFS - 1 )
    .if ( r_buf == _MAX_BUF )
        immed[r_buf, 0]
    .else
        alu[r_buf, r_buf, +, 1]
    .endif


    // if this was an aes-gcm null cipher, set done signal
    // for next buffer as well; we used it to write out
    // cipher data we didn't want in the output packet
    alu[r_cipher, CRYPTO_SA_CIPHER_MASK, AND, r_sa_ctlwrd, \
                                     >>CRYPTO_SA_CIPHER_SHIFT]
    .if ((r_cipher >= CRYPTO_SA_CIPHER_AES128_NULL) && \
         (r_cipher <= CRYPTO_SA_CIPHER_AES256_NULL))

        alu[r_tmp, _me_ctx, AND, 0x6] // previous ctx
        alu[r_tmp, r_tmp, OR, r_buf, <<3] // signal = buffer + 8
        alu[r_tmp, r_tmp, +, (8<<3)]
        local_csr_wr[SAME_ME_SIGNAL, r_tmp]

        .if ( r_buf == _MAX_BUF )
            immed[r_buf, 0]
        .else
            alu[r_buf, r_buf, +, 1]
        .endif
    .endif


    br[crypto_output_thread_loop#]

    .end

#endm


/**
 *  Initialize crypto threads
 *
 *  @b Example:
 *  @code
 *  crypto_threads_init()
 *  @endcode

 *  Initializes input request ring, initializes crypto cores, and initializes and
 *  starts crypto input and output threads. CRYPTO_NUM_THREADS, CRYPTO_START_CTX
 *  determine how many threads are started and what contexts they are started on.
 *
 *  The Request Ring params are defined as REQ_RING_xyz, above.
 *
 *  Typically used via a wrapper microcode file that may override some of the
 *  parameters defined above, invokes this macro, and does nothing else.
 *
 *  Threads that wish to send requests to the request ring may use macro:
 *  crypto_threads_wait_init_flag()
 *  which loops waiting for crypto_threads_init to complete.
 */
#macro crypto_threads_init()

    .begin

    // Threads in use start with thread 0 through thread CRYPTO_NUM_THREADS - 1
    // To run 6 crypto cores in the NFP6000 crypto island, the 1st me could have
    // the default value CRYPTO_NUM_THREADS 6, and the 2nd me could have the value
    // CRYPTO_NUM_THREADS 6, for a total of 12 threads, 2/core
    //
    // Similarly, CRYPTO_START_CTX could use the default value 0 for the 1st micro
    // engine and the value 3 for the 2nd micro engine, assuming the first me is
    // running 3 crypto cores and the 2nd me is running 3
    //
    // In order to initialize the input request ring before all the input threads
    // start, Thread 0 on the me with CRYPTO_START_CTX == 0 will initialize the
    // crypto input request ring, and the remaining threads are held off until the
    // ring is initialized.
    //
    // If the _ring_size param to crypto_threads_init is zero, the ring is assumed
    // to be already initialized and will not be initialized by crypto_threads_init.
    //
    // Thread 0 on the me with CRYPTO_START_CTX == 0 will also clear the crypto
    // counters if COUNTER_USE_ALLOC_MEM == 0
    //
    // Threads on an me are sequenced such that the init_core is done one core
    // at a time.
    //
    // Thread 7 on the me with CRYPTO_START_CTX == 0 will signal the neighbor me.
    // Thread 0 on the me  with CRYPTO_START_CTX != 0 will wait to be signaled by
    // the previous neighbor me. Thus when using crypto_threads_init for
    // initialization and startup, it is required that the 1st me at a minimum
    // uses crypto core/ctx 0, and that if a 2nd me is used, it must be the next
    // neighbor of the 1st me and must not use crypto core/ctx 0.


    .sig _ss, _st
    .addr _ss 12
    .addr _st 13
    .reg r_me_ctx, r_crypto_ctx


    // figure me and crypto contexts for this thread
    thread_id(r_me_ctx, 0)
    alu [ r_crypto_ctx, --, B, r_me_ctx, >>1 ]
    alu [ r_crypto_ctx, r_crypto_ctx, +, CRYPTO_START_CTX]


    //
    // initialize input request ring, clear counters, clear sa flush flags,
    // clear crypto sram
    //
    .if ( r_me_ctx == 0 )
        .if ( r_crypto_ctx == 0 )
            #if (REQ_RING_SIZE > 0 )
                #if ( REQ_RING_TYPE == CRYPTO_RING_EMU0 \
                      || REQ_RING_TYPE == CRYPTO_RING_EMU1 \
                      || REQ_RING_TYPE == CRYPTO_RING_EMU2 \
                      || REQ_RING_TYPE == CRYPTO_RING_WQ )
                    #if ( REQ_RING_USE_ONE_EMU_RING == 1 )
                    // to run with 2 islands & 1 req ring, don't
                    // init ring if on 2nd island
                    .begin
                    .reg r_isld
                    local_csr_rd[ACTIVE_CTX_STS]
                    immed[r_isld, 0]
                    alu[r_isld, 0x3f, AND, r_isld, >>25]
                    .if ( r_isld != 12 )
                        // instead, wait for init flag from
                        // other crypto island init
                        crypto_threads_wait_init_flag()
                    .else
                    #endif /* REQ_RING_USE_ONE_EMU_RING */
                        #if ( REQ_RING_USE_ALLOC_MEM == 0 )
                        .begin
                            .reg r_addr_hi, r_addr_lo
                            crypto_threads_clr_emem(crypto_req_ring, REQ_RING_SIZE)
                            #define_eval _ADDR_HI (crypto_req_ring >> 32) & 0xff
                            #define_eval _ADDR_LO (crypto_req_ring & 0xffffffff)
                            move(r_addr_hi, _ADDR_HI)
                            move(r_addr_lo, _ADDR_LO)
                            ru_emem_ring_setup(r_addr_hi,r_addr_lo, r_addr_lo, \
                                                REQ_RING_NUM, REQ_RING_SIZE, 0)
                            #undef _ADDR_HI
                            #undef _ADDR_LO
                        .end
                        #else
                            #pragma warning(disable:5008)
                            ru_emem_ring_setup(crypto_req_ring_desc, crypto_req_ring, \
                                                REQ_RING_NUM, REQ_RING_SIZE, 0)
                            #pragma warning(default:5008)
                        #endif
                    #if ( REQ_RING_USE_ONE_EMU_RING == 1 )
                    .endif
                    .end
                    #endif
                #elif ( REQ_RING_TYPE == CRYPTO_RING_CTM )
                    #if ( REQ_RING_USE_ALLOC_MEM == 0 )
                        crypto_threads_clr_ctm(crypto_req_ring, REQ_RING_SIZE)
                    #endif
                    ru_ctm_ring_setup(REQ_RING_NUM, crypto_req_ring, \
                                                  REQ_RING_SIZE, FULL )
                #else
                    #error "crypto_threads_init: invalid input ring type."
                #endif
            #endif
            #if ( INIT_FLAG_USE_ALLOC_MEM == 0 )
                crypto_threads_clear_init_flag()
            #endif
            #if ( COUNTER_USE_ALLOC_MEM == 0 )
                crypto_threads_clear_counters()
            #endif
            #if ( ENABLE_SA_FLUSH && SA_FLUSH_FLAGS_USE_ALLOC_MEM == 0 )
                crypto_threads_clear_sa_flush_flags()
            #endif
            #if (ENABLE_CLEAR_SRAM == 1 )
                crypto_threads_clear_sram()
            #endif
        .endif
    .endif


    //
    // synchronize two me's
    //

    .if ( r_me_ctx == 0 )
        .if ( r_crypto_ctx )
            // this is the 1st thread on the 2nd me.
            // wait for signal from 1st me
            #pragma warning(disable:4700)
            signal_wait(_ss, THD_SWAP)
            #pragma warning(default:4700)
        .endif
    .endif


    //
    // do the init cores for this me one core at a time
    //

    .if ( r_me_ctx == 7 ) // signal ctx 0 to start
        signal_next_ctx(&_st)
    .endif

    #pragma warning(disable:4700)
    signal_wait(_st, THD_SWAP) // wait for prior thread to signal
    #pragma warning(default:4700)

    .if ( r_me_ctx < CRYPTO_NUM_THREADS )
        .if ( (r_me_ctx & 1) == 0 )
            // init core
            crypto_threads_init_core (r_crypto_ctx)
        .endif
    .endif

    // signal next thread to go
    signal_next_ctx(&_st)

    // wait again for prior thread to be done with init core
    #pragma warning(disable:4700)
    signal_wait(_st, THD_SWAP)
    #pragma warning(default:4700)

    // signal next thread to go
    signal_next_ctx(&_st)


    //
    // synchronize two me's
    //
    .if ( r_me_ctx == 7 )
        // this is the last thread on the 1st me.
        // signal thread 0 in the next neighbor
        // signal_next_me(&_ss)
        signal_next_me(12) // make warning go away
    .endif


    //
    // flag init done, first me will do this. could update
    // this logic so if 2 me's in use, will wait for 2nd me
    // to be done. however, it will work as is.
    //
    .if ( r_me_ctx == 7 )
        crypto_threads_update_counter(CRYPTO_CNTR_INIT_FLAG)
        crypto_threads_set_init_flag()
    .endif



    //
    // now, run the input or output thread if enabled
    //
    .if ( r_me_ctx < CRYPTO_NUM_THREADS )

        .if ( (r_me_ctx & 1) == 0 )
            //init input thread
            crypto_threads_input(r_me_ctx, r_crypto_ctx, REQ_RING_TYPE, \
                                                           REQ_RING_NUM )
        .else
            // init output thread
            crypto_threads_output(r_me_ctx, r_crypto_ctx)
        .endif

    .else
        // this thread is not going to run the crypto thread code;
        // if want to continue to run other code in this thread,
        // comment out next line
        ctx_arb[kill]

    .endif

     nop // need to stop nfas error

    .end

#endm

/** @}
 * @}
 */

#endif /* __CRYPTO_LIB_THREADS_UC__ */
