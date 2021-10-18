/*
 * Copyright (C) 2013 Netronome Systems, Inc. All rights reserved.
 */


#ifndef __CRYPTO_LIB_UC__
#define __CRYPTO_LIB_UC__

#ifndef NFP_LIB_ANY_NFAS_VERSION
    #if (!defined(__NFP_TOOL_NFAS) ||                       \
        (__NFP_TOOL_NFAS < NFP_SW_VERSION(5, 0, 0, 0)) ||   \
        (__NFP_TOOL_NFAS > NFP_SW_VERSION(6, 1, 0, 255)))
        #error "This standard library is not supported for the version of the SDK currently in use."
    #endif
#endif

/** @file crypto_lib.uc CRYPTO Operation Macros
 * @addtogroup crypto CRYPTO Operation
 * @{
 *
 * @name CRYPTO Operation Macros
 * @{
 *
 * Crypto logic support macros facilitating set up and run of precompiled compressed crypto instruction
 * sequences which conform to the crypto-library standard.
 */

// API Quick Reference:
//
// crypto_get_core (cr_xfr, cr_ctx)
// crypto_free_core (cr_xfr, cr_ctx)
// crypto_setup_configs (drn, cipher, mode, keysize, hash)
// crypto_setup_configs (hlen, select, modes, drn, cipher, mode, keysize, hash)
// crypto_send (cmd, xfr, a, b, n, xsig)
// crypto_send (cmd, xfer, a, b, n)
// crypto_send (cmd, xfer, a, b)
// crypto_send (xfer)
// crypto_build_xfer_reg (res, lo, hi)
// crypto_load_library_sequence (cr_xfr, seq, core, vars, sigspec)
// crypto_load_library_sequence (cr_xfr, seq, core, vars)
// crypto_run_library_sequence (cr_xfr, cr_ctx, seq, core, vars, base, sigspec)
// crypto_run_library_sequence (cr_xfr, cr_ctx, seq, core, vars, base)
// crypto_run_library_sequence (cr_xfr, cr_ctx, seq, core, vars)
// crypto_get_island_and_PPID7 (out_island, out_PPID7)
// crypto_send_signal (cr_xfr, cr_ctx, me_ctx, island, PPID7, sigspec, cmdsigspec)
// crytpo_send_signal (cr_xfr, cr_ctx, me_ctx, island, PPID7, sigspec)
// crypto_send_signal (cr_xfr, cr_ctx, me_ctx, sigspec, cmdsigspec)
// crypto_send_signal (cr_xfr, cr_ctx, me_ctx, sigspec)
// crytpo_send_done_signal (cr_xfr, cr_ctx, me_ctx, dispatch, island, PPID7, sigspec, iosigspec)
// crypto_send_done_signal (cr_xfr, cr_ctx, me_ctx, dispatch, island, PPID7, sigspec)
// crypto_send_done_signal (cr_xfr, cr_ctx, me_ctx, dispatch, sigspec, iosigspec)
// crypto_send_done_signal (cr_xfr, cr_ctx, me_ctx, dispatch, sigspec)
// crypto_init_CIB_setup (stat, dyn, code)
// crypto_library_load_constants (cr_xfr, cr_ctx, n, base0, base1, base2, base3, base4, base5)
// crypto_library_load_constants (cr_xfr, cr_ctx)
// crypto_library_load_constants (cr_xfr, cr_ctx, b0)
// crypto_library_load_constants (cr_xfr, cr_ctx, b0, b1)
// crypto_library_load_constants (cr_xfr, cr_ctx, b0, b1, b2)
// crypto_library_load_constants (cr_xfr, cr_ctx, b0, b1, b2, b3)
// crypto_library_load_constants (cr_xfr, cr_ctx, b0, b1, b2, b3, b4)
// crypto_library_load_constants (cr_xfr, cr_ctx, b0, b1, b2, b3, b4, b5)
// crypto_library_load (LOAD_TYPE, xfr, ctx, seq, core)
// crypto_library_load (LOAD_TYPE, xfr, ctx, seq)
// crypto_setup_dma_desc_cycle1 (dest_lo, dest_hi, ctx, xlen, prefetch, addr_lo, addr_hi)
// crypto_setup_dma_desc_cycle2 (dest_lo, dest_hi, ctx, dir, token, action, target, w32, chain, last, cr_addr)
// crypto_dma_out (cr, dma_ctx, chain, last, sram_addr, addr32_lo, addr32_hi, nbytes, sent_sig)
// crypto_dma_out (cr, dma_ctx, chain, last, sram_addr, addr32_lo, addr32_hi, nbytes)
// crypto_dma_in (cr, dma_ctx, chain, last, sram_addr, addr32_lo, addr32_hi, nbytes, sent_sig)
// crypto_dma_in (cr, dma_ctx, chain, last, sram_addr, addr32_lo, addr32_hi, nbytes)
// crypto_non_dma_out(sram_addr, addr32_lo, addr32_hi, nbytes, mode)

#include <stdmac.uc>
#include <limit.uc>

/// @cond INTERNAL_MACROS
#ifdef NFP_38XX  // temporary use until IS_NFPTYPE(__NFP38XX) is implemented
    #if (NFP_38XX==1)
        #define_eval nfp_38xx 1
    #else
        #define_eval nfp_38xx 0
    #endif
#else
    #define_eval nfp_38xx 0
#endif

#if nfp_38xx==1
    #define CRYPTO_INSTRUCTION_BUFFER_SIZE  0x200 // NFP38xxC CIB size is 2048B, or 512 4-byte words
    #define CRYPTO_NFP_MODE   1
#elif (IS_NFPTYPE(__NFP6000))
    #define CRYPTO_INSTRUCTION_BUFFER_SIZE  0x100 // NFP6xxxC CIB size is 1024B, or 256 4-byte words
    #define CRYPTO_NFP_MODE   1
#else
    #define CRYPTO_INSTRUCTION_BUFFER_SIZE   0x80 // NFP3xxxC CIB size is 512B, or 128 4-byte words
    #define CRYPTO_NFP_MODE   0
#endif

#define is_numeric(x) (isnum(x) || isimport(x))

// Definitions used within this library

#define CRYPTO_LOAD_STATIC                      0x0C    // used to select static load/run type
#define CRYPTO_LOAD_DYNAMIC                     0x0D    // used to select dynamic load/run type
#define CRYPTO_LIBDESC_STATIC_BIT               30      // bit in library descriptor word indicating static vs dynamic
/// @endcond


// used for crypto_setup_configs

#define CRYPTO_DIR_ENCRYPT  0
#define CRYPTO_DIR_DECRYPT  1

#define CRYPTO_CIPHER_NONE          0
#define CRYPTO_CIPHER_NULL_RELOCATE 1
#define CRYPTO_CIPHER_NULL_DISCARD  2
#define CRYPTO_CIPHER_NULL_INSERT   3
#define CRYPTO_CIPHER_AES_128       4
#define CRYPTO_CIPHER_AES_192       5
#define CRYPTO_CIPHER_AES_256       6
#define CRYPTO_CIPHER_3DES          7
#define CRYPTO_CIPHER_2DES          8
#define CRYPTO_CIPHER_ARC4          9
#define CRYPTO_CIPHER_UEA1         10
#define CRYPTO_CIPHER_UEA2         11
#define CRYPTO_CIPHER_UIA1         12
#define CRYPTO_CIPHER_UIA2         13
#define CRYPTO_CIPHER_CHACHA       14

#define CRYPTO_CIMODE_ECB   0
#define CRYPTO_CIMODE_CBC   1
#define CRYPTO_CIMODE_CFB   2
#define CRYPTO_CIMODE_OFB   3
#define CRYPTO_CIMODE_CTR   4

#define CRYPTO_HASH_NONE        0
#define CRYPTO_HASH_MD5         1
#define CRYPTO_HASH_GF128       2
#define CRYPTO_HASH_GF128_S     3
#define CRYPTO_HASH_SHA_1       4
#define CRYPTO_HASH_SHA_256     5
#define CRYPTO_HASH_SHA_384     6
#define CRYPTO_HASH_SHA_512     7
#define CRYPTO_HASH_CRC16       8
#define CRYPTO_HASH_CRC16_REVI  9
#define CRYPTO_HASH_CRC16_REVO  10
#define CRYPTO_HASH_CRC16_REVIO 11
#define CRYPTO_HASH_POLY1305_S  12
#define CRYPTO_HASH_POLY1305_SH 13

// used for crypto_non_dma_out
#define CRYPTO_NDO_DEFAULT 0
#define CRYPTO_NDO_7B      1
#define CRYPTO_NDO_16LW    2

/// @cond INTERNAL_MACROS

// Crypto Configuration definitions

// config_pulse bit definitions
#if nfp_38xx==1
#define CRYPTO_CFG_PULSE_HASH_CLEAR             0x800
#define CRYPTO_CFG_PULSE_DES_KEY_CLEAR          0x000
#define CRYPTO_CFG_PULSE_BLK_STATE_CLEAR        0x200
#else
#define CRYPTO_CFG_PULSE_HASH_CLEAR             0xC00
#define CRYPTO_CFG_PULSE_DES_KEY_CLEAR          0x100
#define CRYPTO_CFG_PULSE_BLK_STATE_CLEAR        0x200
#endif
#define CRYPTO_CFG_PULSE_CLEAR_ALL              (CRYPTO_CFG_PULSE_HASH_CLEAR | CRYPTO_CFG_PULSE_DES_KEY_CLEAR | CRYPTO_CFG_PULSE_BLK_STATE_CLEAR)

// config_select bit definitions
#define CRYPTO_CFG_SEL_CIPHER_NONE              0x00
#define CRYPTO_CFG_SEL_CIPHER_NULL_RELOCATE     0x04
#define CRYPTO_CFG_SEL_CIPHER_NULL_DISCARD      0x08
#define CRYPTO_CFG_SEL_CIPHER_NULL_INSERT       0x0C
#define CRYPTO_CFG_SEL_CIPHER_BLOCK_AES         0x01
#define CRYPTO_CFG_SEL_CIPHER_BLOCK_DES         0x05
#define CRYPTO_CFG_SEL_CIPHER_ARC4              0x02
#define CRYPTO_CFG_SEL_CIPHER_UEA1              0x0A
#define CRYPTO_CFG_SEL_CIPHER_UEA2              0x06
#define CRYPTO_CFG_SEL_CIPHER_CHACHA            0x0A
#define CRYPTO_CFG_SEL_CIPHER_UIA1              0x03
#define CRYPTO_CFG_SEL_CIPHER_UIA2              0x07
#define CRYPTO_CFG_SEL_HASH_NONE                0x00
#define CRYPTO_CFG_SEL_HASH_MD5                 0x10
#define CRYPTO_CFG_SEL_HASH_GF128               0xD0
#define CRYPTO_CFG_SEL_HASH_GF128_S             0x50
#define CRYPTO_CFG_SEL_HASH_SHA_1               0x20
#define CRYPTO_CFG_SEL_HASH_SHA_256             0x60
#define CRYPTO_CFG_SEL_HASH_SHA_384             0xA0
#define CRYPTO_CFG_SEL_HASH_SHA_512             0xE0
#define CRYPTO_CFG_SEL_HASH_CRC16               0x30
#define CRYPTO_CFG_SEL_HASH_CRC16_REVI          0x70
#define CRYPTO_CFG_SEL_HASH_CRC16_REVO          0xB0
#define CRYPTO_CFG_SEL_HASH_CRC16_REVIO         0xF0
#define CRYPTO_CFG_SEL_HASH_POLY1305            0xD0
#define CRYPTO_CFG_SEL_HASH_POLY1305_S          0x50

// hash algorithm state structure lengths
#define CRYPTO_HASH_STATE_LEN_NONE              0x1F
#define CRYPTO_HASH_STATE_LEN_MD5               0x1F
#define CRYPTO_HASH_STATE_LEN_SNOW              0x1F
#define CRYPTO_HASH_STATE_LEN_SHA_1             0x1F
#define CRYPTO_HASH_STATE_LEN_SHA_256           0x2F
#define CRYPTO_HASH_STATE_LEN_SHA_384           0x4F
#define CRYPTO_HASH_STATE_LEN_SHA_512           0x4F

// Crypto cmd bit definitions

// Bitfields for first 64-bit command word in DMA opcode

#define   NFP_CRYPTO_C1_CMD_ID_bf                  3, 31, 24
#define   NFP_CRYPTO_C1_CMD_ID_mask                (0xFF)
#define   NFP_CRYPTO_C1_CMD_ID_shift               (24)

#define   NFP_CRYPTO_C1_CTX_bf                     3, 23, 20
#define   NFP_CRYPTO_C1_CTX_mask                   (0x0F)
#define   NFP_CRYPTO_C1_CTX_shift                  (20)

#define   NFP_CRYPTO_DMA_LENGTH_bf                 3, 19, 12
#define   NFP_CRYPTO_DMA_LENGTH_mask               (0xFF)
#define   NFP_CRYPTO_DMA_LENGTH_shift              (12)

#define   NFP_CRYPTO_DMA_PREFETCH_REQ_bf           3, 11, 11
#define   NFP_CRYPTO_DMA_PREFETCH_REQ_mask         (0x01)
#define   NFP_CRYPTO_DMA_PREFETCH_REQ_bit          (11)

#define   NFP_CRYPTO_DMA_MADDR_HI_bf               3, 7, 0
#define   NFP_CRYPTO_DMA_MADDR_HI_mask             (0xFF)
#define   NFP_CRYPTO_DMA_MADDR_HI_shift            (0)

#define   NFP_CRYPTO_DMA_MADDR_LO_bf               2, 31, 0
#define   NFP_CRYPTO_DMA_MADDR_LO_mask             (0xFFFFFFFF)
#define   NFP_CRYPTO_DMA_MADDR_LO_shift            (0)

// Bitfields for second 64-bit command word in crypto DMA opcode

// High word (bits 63..32)

#define   NFP_CRYPTO_C2_CMD_ID_bf                  1, 31, 24
#define   NFP_CRYPTO_C2_CMD_ID_mask                (0xFF)
#define   NFP_CRYPTO_C2_CMD_ID_shift               (24)

#define   NFP_CRYPTO_C2_CTX_bf                     1, 23, 20
#define   NFP_CRYPTO_C2_CTX_mask                   (0x0F)
#define   NFP_CRYPTO_C2_CTX_shift                  (20)

#define   NFP_CRYPTO_DMA_DIR_bf                    1, 19, 19
#define   NFP_CRYPTO_DMA_DIR_mask                  (0x01)
#define   NFP_CRYPTO_DMA_DIR_bit                   (19)

#define   NFP_CRYPTO_DMA_TOKEN_bf                  1, 18, 17
#define   NFP_CRYPTO_DMA_TOKEN_mask                (0x03)
#define   NFP_CRYPTO_DMA_TOKEN_shift               (17)

#define   NFP_CRYPTO_DMA_ACTION_bf                 1, 16, 12
#define   NFP_CRYPTO_DMA_ACTION_mask               (0x1F)
#define   NFP_CRYPTO_DMA_ACTION_shift              (12)

#define   NFP_CRYPTO_DMA_TARGET_bf                 1, 11, 8
#define   NFP_CRYPTO_DMA_TARGET_mask               (0x0F)
#define   NFP_CRYPTO_DMA_TARGET_shift              (8)

// Low word (bits 31..0)

#define   NFP_CRYPTO_DMA_W32_bf                    0, 16, 16
#define   NFP_CRYPTO_DMA_W32_mask                  (0x01)
#define   NFP_CRYPTO_DMA_W32_bit                   (16)

#define   NFP_CRYPTO_DMA_CHAIN_bf                  0, 14, 14
#define   NFP_CRYPTO_DMA_CHAIN_mask                (0x01)
#define   NFP_CRYPTO_DMA_CHAIN_bit                 (14)

#define   NFP_CRYPTO_DMA_LAST_bf                   0, 13, 13
#define   NFP_CRYPTO_DMA_LAST_mask                 (0x01)
#define   NFP_CRYPTO_DMA_LAST_bit                  (13)

#define   NFP_CRYPTO_DMA_LOCAL_ADDR_bf             0, 12, 0
#define   NFP_CRYPTO_DMA_LOCAL_ADDR_mask           (0x1FFF)
#define   NFP_CRYPTO_DMA_LOCAL_ADDR_shift          (0)

/// @endcond

/**
 *  Allocate a core to a Dispatcher context.
 *
 *  @b Example:
 *  @code
 *  .reg $cr[2]
 *  #define CRYPTO_CTX 0
 *  crypto_get_core ($cr, CRYPTO_CTX)
 *  crypto_send (write_fifo, $cr, 0, 0)
 *  @endcode
 *
 *  @param cr_xfr   Destination transfer register array,  2 registers
 *  @param cr_ctx   Crypto Dispatcher context whose bulk core will get the data and run command
 *
 *  This macro prepares transfer regs for get core operation
 */
#macro crypto_get_core (cr_xfr, cr_ctx)
    immed32 (cr_xfr[0], 0x00000000)
    or_shf_left (cr_xfr[1], 0x04000000, cr_ctx, 20)
    #define_eval _crypto_transfer_length 1
#endm


/**
 *  Frees a previously allocated core from a Dispatcher context.
 *
 *  @b Example:
 *  @code
 *  .reg $cr[2]
 *  #define CRYPTO_CTX 0
 *  crypto_free_core ($cr, CRYPTO_CTX)
 *  crypto_send (write_fifo, $cr, 0, 0)
 *  @endcode
 *
 * @param cr_xfr   Destination transfer register array,  2 registers
 * @param cr_ctx   Crypto Dispatcher context whose bulk core will be released
 *
 */
#macro crypto_free_core (cr_xfr, cr_ctx)
    immed32 (cr_xfr[0], 0x00000001)
    or_shf_left (cr_xfr[1], 0x04000000, cr_ctx, 20)
    #define_eval _crypto_transfer_length 1
#endm


/**
 * Create configuration values for setting up the crypto unit for commonly used ciphers and hashes.
 *
 * @b Example:
 * @code
 * crypto_setup_configs (enc, AES, CBC, 256, MD5)
 * @endcode
 *
 * @param drn      CONST, selects direction ('encrypt' or 'decrypt' or 'enc' or 'dec')
 * @param cipher   CONST, selects cipher type: 'none', 'null', 'aes', or 'des' supported
 * @param mode     CONST, selects block cipher mode: 'ECB' or 'CBC' supported
 * @param keysize  CONST, bit length of key: 128, 192, 256 supported for AES; use '--' for DES
 * @param hash     CONST, selects hash type: MD5, SHA1, SHA256, SHA384, SHA512 supported
 *
 * Generates the following #defined values, and no code:
 *
 *  _crypt_select               provides the cfg_pulse_R and cfg_sel_R fields suitable for use in a
 *                               Crypt_Setup_Regs_0 instruction; includes bits to clear hash and cipher states
 *  _crypt_select_no_clear      same as _crypt_select but does not include cipher and hash state-clearing bits
 *  _crypt_modes                provides the cfg_Crypt0_R field for use in a crypt_setup_regs_0 instruction
 *  _crypt_hmac_len             provides the length of the hmac, -1 byte
 *  _crypt_iv_len               provides the length of the iv
 */
#macro crypto_setup_configs (drn, cipher, mode, keysize, hash)
    #define_eval _crypt_select   0
    #define_eval _crypt_modes    0
    #define_eval _crypt_hmac_len 0
    #define_eval _crypt_iv_len   0

    #if streq('drn','enc') || streq('drn','encrypt') || streq('drn','ENC') || streq('drn','ENCRYPT')
        #define_eval _crypt_modes (_crypt_modes & 0xFFFB)
    #elif streq('drn', 'dec') || streq('drn', 'decrypt') || streq('drn', 'DEC') || streq('drn', 'DECRYPT')
        #define_eval _crypt_modes (_crypt_modes | 0x04)
    #else
        #error "Undecodable direction keyword" drn "... supported options are 'encrypt' and 'decrypt'"
    #endif

    #if streq('cipher','--') || streq('cipher','none') || streq('cipher','NONE')
        #define_eval _crypt_select (_crypt_select & 0xFFFE)
    #elif streq('cipher','null') || streq('cipher','NULL') || streq('cipher','relocate') || streq('cipher','RELOCATE')
        #define_eval _crypt_select (_crypt_select | 0x07)
    #elif streq('cipher','AES') || streq('cipher','aes')
        #define_eval _crypt_select (_crypt_select | 0x01)
        #define_eval _crypt_iv_len 16
    #elif streq('cipher','DES') || streq('cipher','des') || streq('cipher','3DES') || streq('cipher','3des')
        #define_eval _crypt_select (_crypt_select | 0x05)
        #define_eval _crypt_modes   (_crypt_modes | 0x100)
        #define_eval _crypt_iv_len 8
    #else
        #error "Undecodable cipher-type keyword" mode "... supported options are 'AES' and 'DES'"
    #endif

    #if streq('mode', 'CBC') || streq('mode', 'cbc')
        #define_eval _crypt_modes (_crypt_modes | 0x08)
    #elif streq('mode', 'ECB') || streq('mode', 'ecb')
    #else
        #error "Undecodable cipher-mode keyword" mode "... supported options are 'ECB' and 'CBC'"
    #endif

    #if !streq('keysize', '--')
        #if (_crypt_modes & 0x0100)==0x100
            #error "key-size must be specified as '--' for DES cipher-type"
        #else
            #if streq('keysize', '128')
            #elif streq('keysize', '192')
                #define_eval _crypt_modes (_crypt_modes | 0x01)
            #elif streq('keysize', '256')
                #define_eval _crypt_modes (_crypt_modes | 0x02)
            #endif
        #endif
    #endif

    #if streq('hash', '--') || streq('hash', 'none') || streq('hash', 'NONE') || streq('hash', 'null') || streq('hash', 'NULL')
    #elif streq('hash', 'md5') || streq('hash', 'MD5') || streq('hash', 'md-5') || streq('hash', 'MD-5')
        #define_eval _crypt_select (_crypt_select | 0x10)
        #define_eval _crypt_hmac_len 15
    #elif streq('hash', 'sha1') || streq('hash', 'SHA1') || streq('hash', 'sha-1') || streq('hash', 'SHA-1')
        #define_eval _crypt_select (_crypt_select | 0x20)
        #define_eval _crypt_hmac_len 19
    #elif streq('hash', 'sha256') || streq('hash', 'SHA256') || streq('hash', 'sha-256') || streq('hash', 'SHA-256')
        #define_eval _crypt_select (_crypt_select | 0x60)
        #define_eval _crypt_hmac_len 31
    #elif streq('hash', 'sha384') || streq('hash', 'SHA384') || streq('hash', 'sha-384') || streq('hash', 'SHA-384')
        #define_eval _crypt_select (_crypt_select | 0xA0)
        #define_eval _crypt_hmac_len 47
    #elif streq('hash', 'sha512') || streq('hash', 'SHA512') || streq('hash', 'sha-512') || streq('hash', 'SHA-512')
        #define_eval _crypt_select (_crypt_select | 0xE0)
        #define_eval _crypt_hmac_len 63
    #else
        #error "undecodable hash-type keyword " hash "... supported types are '--', 'MD5', 'SHA1', 'SHA256', 'SHA384', 'SHA512'"
    #endif

    #define_eval _crypt_select_no_clear (_crypt_select)
    #define_eval _crypt_select          (_crypt_select | CRYPTO_CFG_PULSE_CLEAR_ALL)
#endm


/**
 * Create configuration values for setting up the crypto unit for commonly used ciphers and hashes.
 *
 * @b Example:
 * @code
 * .reg hlen, ivlen, select, mode
 * .reg dir, cipher, cimode, hash
 * move (dir, CRYPTO_DIR_ENCRYPT)
 * move (cipher, CRYPTO_CIPHER_AES_256
 * etc.
 * crypto_setup_configs (hlen, ivlen, select, mode, \
 *                        dir, cipher, cimode, keylen, hash)
 * @endcode
 *
 * @param _hlen     GPR, returns icv/hash length, -1 byte
 * @param _ivlen    GPR, returns iv length
 * @param _select   GPR, returns cfg_pulse_R and cfg_sel_R fields suitable for use in a
 *                      Crypt_Setup_Regs_0 instruction; includes bits to clear hash and cipher states
 * @param _modes    GPR, returns cfg_Crypt0_R field for use in a Crypt_Setup_Regs_0 instruction
 * @param _dir      GPR, selects direction, one of CRYPTO_DIR_*
 * @param _cipher   GPR, selects cipher type: one of CRYPTO_CIPHER_*
 * @param _mode     GPR, selects block cipher mode: one of CRYPT_CIMODE_*
 * @param _hash     GPR, selects hash type: one of CRYPTO_HASH_*
 *
 * Similar to above macro, but uses GPR's instead of constants
 */
#macro crypto_setup_configs (_hlen, _ivlen, _select, _modes, _dir, _cipher, _mode, _hash)

     move(_hlen, 0)
     move(_ivlen, 0)
     move(_select, 0)
    .if ( _dir == CRYPTO_DIR_ENCRYPT)
        move(_modes, 0)
    .else
        move(_modes, 0x04)
    .endif


    // cipher setup

    jump[_cipher, cscsjt#], targets [scsj0#, scsj1#, scsj2#, scsj3#,  scsj4#,  scsj5#,  scsj6#, \
                                     scsj7#, scsj8#, scsj9#, scsj10#, scsj11#, scsj12#, scsj13#]
    cscsjt#:
        scsj0#: br[cscsjt0#]
        scsj1#: br[cscsjt1#]
        scsj2#: br[cscsjt2#]
        scsj3#: br[cscsjt3#]
        scsj4#: br[cscsjt4#]
        scsj5#: br[cscsjt5#]
        scsj6#: br[cscsjt6#]
        scsj7#: br[cscsjt7#]
        scsj8#: br[cscsjt8#]
        scsj9#: br[cscsjt9#]
        scsj10#: br[cscsjt10#]
        scsj11#: br[cscsjt11#]
        scsj12#: br[cscsjt12#]
        scsj13#: br[cscsjt13#]

    cscsjt0#:
        move(_select, CRYPTO_CFG_SEL_CIPHER_NONE )
        br[csc_mode#]
    cscsjt1#:
        move(_select, CRYPTO_CFG_SEL_CIPHER_NULL_RELOCATE )
        br[csc_mode#]
    cscsjt2#:
        move(_select, CRYPTO_CFG_SEL_CIPHER_NULL_DISCARD )
        br[csc_mode#]
    cscsjt3#:
        move(_select, CRYPTO_CFG_SEL_CIPHER_NULL_INSERT )
        br[csc_mode#]
    cscsjt4#:
    cscsjt5#:
    cscsjt6#:
        move(_select, CRYPTO_CFG_SEL_CIPHER_BLOCK_AES)
        .if (_mode == CRYPTO_CIMODE_CTR)
            move(_ivlen, 8) // aes gcm
        .else
            move(_ivlen, 16) // should be 8?
        .endif
        br[csc_mode#]
    cscsjt7#:
	move(_select, CRYPTO_CFG_SEL_CIPHER_BLOCK_DES)
        move(_ivlen, 8)
        or_shf_left(_modes, _modes, 0x01, 8)
        br[csc_mode#]
    cscsjt8#:
	move(_select, CRYPTO_CFG_SEL_CIPHER_BLOCK_DES)
        move(_ivlen, 8)
        br[csc_mode#]
    cscsjt9#:
        move(_select, CRYPTO_CFG_SEL_CIPHER_ARC4 )
        br[csc_mode#]
    cscsjt10#:
        move(_select, CRYPTO_CFG_SEL_CIPHER_UEA1 )
        br[csc_mode#]
    cscsjt11#:
        move(_select, CRYPTO_CFG_SEL_CIPHER_UEA2 )
        br[csc_mode#]
    cscsjt12#:
        move(_select, CRYPTO_CFG_SEL_CIPHER_UIA1 )
        br[csc_mode#]
    cscsjt13#:
        move(_select, CRYPTO_CFG_SEL_CIPHER_UIA2 )
        br[csc_mode#]


    // cipher mode setup

    csc_mode#:

    jump[_mode, cscmjt#], targets [scmj0#, scmj1#, scmj2#, scmj3#,  scmj4#]

    cscmjt#:
        scmj0#: br[cscmjt0#]
        scmj1#: br[cscmjt1#]
        scmj2#: br[cscmjt2#]
        scmj3#: br[cscmjt3#]
        scmj4#: br[cscmjt4#]

    cscmjt0#:
        // ECB (no change needed)
        br[csc_mode1#]
    cscmjt1#:
        bits_set(_modes, 2, 0x02 ) // CBC
        br[csc_mode1#]
    cscmjt2#:
        bits_set(_modes, 2, 0x04 ) // CFB
        br[csc_mode1#]
    cscmjt3#:
        move(_modes, (0x06 << 2) ) // OFB
        br[csc_mode1#]
    cscmjt4#:
        // CTR mode
        //  cntr size 4 bytes
        //  position 0 in ctr in block
        //  enable fast 128b path for aesgcm
        #define_eval mode_val ( \
                         (3 << 12) | \
                         (0 <<  8) | \
                         (1 <<  7) | \
                         (0x07 << 2))
        move(_modes, mode_val)
        br[csc_mode1#]

    csc_mode1#:

    .if ( _cipher == CRYPTO_CIPHER_AES_256 )
        bits_set(_modes, 0, 0x02 )
    .elif ( _cipher == CRYPTO_CIPHER_AES_192 )
        bits_set(_modes, 0, 0x01 )
    .else
        // no change
    .endif


    // hash setup

    jump[_hash, cschjt#], targets [schj0#, schj1#, schj2#, schj3#,  schj4#,  schj5#, \
                                   schj6#, schj7#, schj8#, schj9#,  schj10#, schj11# ]

    cschjt#:
        schj0#: br[cschjt0#]
        schj1#: br[cschjt1#]
        schj2#: br[cschjt2#]
        schj3#: br[cschjt3#]
        schj4#: br[cschjt4#]
        schj5#: br[cschjt5#]
        schj6#: br[cschjt6#]
        schj7#: br[cschjt7#]
        schj8#: br[cschjt8#]
        schj9#: br[cschjt9#]
        schj10#: br[cschjt10#]
        schj11#: br[cschjt11#]

    cschjt0#:
        // NONE (no change needed)
        br[csc_clr#]
    cschjt1#:
        or_shf_left(_select, _select, CRYPTO_CFG_SEL_HASH_MD5, 0)
        move(_hlen, 15)
        br[csc_clr#]
    cschjt2#:
        or_shf_left(_select, _select, CRYPTO_CFG_SEL_HASH_GF128, 0)
        br[csc_clr#]
    cschjt3#:
        or_shf_left(_select, _select, CRYPTO_CFG_SEL_HASH_GF128_S, 0)
        br[csc_clr#]
    cschjt4#:
        or_shf_left(_select, _select, CRYPTO_CFG_SEL_HASH_SHA_1, 0)
        move(_hlen, 19)
        br[csc_clr#]
    cschjt5#:
        or_shf_left(_select, _select, CRYPTO_CFG_SEL_HASH_SHA_256, 0)
        move(_hlen, 31)
        br[csc_clr#]
    cschjt6#:
        or_shf_left(_select, _select, CRYPTO_CFG_SEL_HASH_SHA_384, 0)
        move(_hlen, 47)
        br[csc_clr#]
    cschjt7#:
        or_shf_left(_select, _select, CRYPTO_CFG_SEL_HASH_SHA_512, 0)
        move(_hlen, 63)
        br[csc_clr#]
    cschjt8#:
        or_shf_left(_select, _select, CRYPTO_CFG_SEL_HASH_CRC16, 0)
        br[csc_clr#]
    cschjt9#:
        or_shf_left(_select, _select, CRYPTO_CFG_SEL_HASH_CRC16_REVI, 0)
        br[csc_clr#]
    cschjt10#:
        or_shf_left(_select, _select, CRYPTO_CFG_SEL_HASH_CRC16_REVO, 0)
        br[csc_clr#]
    cschjt11#:
        or_shf_left(_select, _select, CRYPTO_CFG_SEL_HASH_CRC16_REVIO, 0)
        br[csc_clr#]

    csc_clr#:

    or_shf_left(_select, _select, CRYPTO_CFG_PULSE_CLEAR_ALL, 0)

#endm


/**
 *  Send a specified number of 8-byte words to the Crypto unit.
 *
 * @b Example:
 * @code
 * .sig s
 * .reg $cmd[4]
 * #define_eval xfer_length 2
 * crypto_send (write_fifo, $cmd, --, 0, xfer_length, s)
 * @endcode
 *
 * @param cmd      type of transfer, must be either "write" or "write_fifo"
 * @param xfr      transfer register array to use in the write
 * @param a        crypto sram address to be written (only used in "write" case)
 *                   can be constant or GPR
 *                   use -- for "write_fifo" operations
 * @param b        offset to crypto sram address a (only used in "write" case)
 *                   must be constant
 *                   use -- or 0 for "write_fifo" operations
 * @param n        optional number of pairs of registers from xfr will be sent to the crypto unit
 *                   must be constant
 * @param xsig     optional ME signal to be used for the I/O operation
 *                   use -- to generate a local signal and wait for it
 *                   use a signal name to do sig_done[] on that signal and return without waiting
 *                   use *nosend* to prevent send
 */
#macro crypto_send (cmd, xfr, a, b, n, xsig)
    #if !streq('xsig','*nosend*')
        .begin
        #if streq('a','--')
            .reg ax
            immed [ax, 0]
        #elif (isnum(a) || isimport(a))
            .reg ax
            immed32 (ax, a)
        #else
            #define_eval ax 'a'
        #endif
        #if streq('xsig','--')
            .sig sig1
            #define_eval _crsend_sig_spec ('ctx_swap[sig1]')
        #else
            #define_eval _crsend_sig_spec ('sig_done[xsig]')
        #endif
        #if streq('b', '--')
            #define_eval _crsend_b 0
        #else
            #define_eval _crsend_b b
        #endif
	#if (n < 9)
            crypto [cmd, xfr[0], ax, b, n], _crsend_sig_spec
            #if defined(log_crypto) && (log_crypto > 0) && streq(cmd, "write_fifo")
                crypto [write, xfr[0], log_crypto_ptr, 0, n], _crsend_sig_spec
                alu [log_crypto_ptr, log_crypto_ptr, +, (n<<3)]
            #endif
	#else
            // Handle transfers of >8 pairs of registers (won't fit in a single write instruction)
            #if !defined(CRYPTO_NFP_MODE) || (CRYPTO_NFP_MODE==0)
                // NFP3xxxC version
                .reg xx
                #if isnum(xlen)
                    immed [xx, (xlen-1)]
                #else
                    alu [xx, xlen, -, 1]
                #endif
                alu_shf [--, xx, OR, 0x02, <<28]
                crypto [cmd, xfr[0], ax, b, max_/**/n], _crsend_sig_spec, indirect_ref
            #else
                // NFP6xxxC version
                // *** TODO: Replace this with a single 'indirect' length command, once indirection syntax is settled ***
                .sig _first8
                crypto [cmd, xfr[0], ax, b, 8], ctx_swap[_first8]
                crypto [cmd, xfr[16], ax, (b+64), (n-8)], _crsend_sig_spec
            #endif
            #if defined(log_crypto) && (log_crypto > 0) && streq(cmd, "write_fifo")
                crypto [write, xfr[0], log_crypto_ptr, 0, 8]
                crypto [write, xfr[0], log_crypto_ptr, 64, (n-8)], _crsend_sig_spec
                alu [log_crypto_ptr, log_crypto_ptr, +, (n<<3)]
            #endif
	#endif
        .end
        #undef _crsend_sig_spec
        #undef _crsend_b
    #endif
#endm


/**
 *  Send a specified number of 8-byte words to the Crypto unit.
 *
 *  Invokes crypto_send, above, with parameter
 *    xsig set to "--"
 */
#macro crypto_send (cmd, xfer, a, b, n)
    crypto_send (cmd, xfer, a, b, n, --)
#endm


/**
 *  Send a specified number of 8-byte words to the Crypto unit.
 *
 *  Invokes crypto_send, above, with parameters:
 *    xsig set to "--"
 *    n set to _crypto_transfer_length
 *
 *  _crypto_transfer_length is set by other macros in this library.
 */
 #macro crypto_send (cmd, xfer, a, b)
    crypto_send (cmd, xfer, a, b, _crypto_transfer_length, --)
 #endm


/**
 *  Send a specified number of 8-byte words to the Crypto unit.
 *
 *  Invokes crypto_send, above, with parameters:
 *    cmd set to "write_fifo"
 *    a set to "--" ( ignored for write_fifo )
 *    b set to "--" ( ignored for write_fifo )
 *    xsig set to "--"
 *    n set to _crypto_transfer_length
 *
 *  _crypto_transfer_length is set by other macros in this library.
 */
#macro crypto_send (xfer)
    .begin
    crypto_send (write_fifo, xfer, --, 0, _crypto_transfer_length, --)
    .end
#endm


/**
 *  Load a 32-bit value in a register from two 16-bit values
 *
 *  @param res           GPR destination
 *  @param lo            GPR or CONSTANT, lower 16 bits
 *  @param hi            GPR or CONSTANT, upper 16 bits
 */
#macro crypto_build_xfer_reg (res, lo, hi)
    #if (isnum(hi) || isimport(hi))
        #if (isnum(lo) || isimport(lo))
            immed32 (res, (lo | (hi << 16)))
        #elif (hi>0xFF || hi<0)
        .begin
            .reg _x
            immed32 (_x, hi)
            alu [res, lo, OR, _x, <<16]
            //or_shf_left (res, lo, _x, 16)
        .end
        #else
            alu [res, lo, OR, hi, <<16]
            //or_shf_left (res, lo, hi, 16)
        #endif
    #else
        #if (isnum(lo) || isimport(lo))
        .begin
            .reg _x
            immed32 (_x, lo)
            alu [res, _x, OR, hi, <<16]
        .end
        #else
            alu [res, lo, OR, hi, <<16]
        #endif
    #endif
#endm


/// @cond INTERNAL_MACROS
/**
 * Transfer data from crypto SRAM to the core CIB buffer and optionally execute a sequence.
 *
 * @param cr_xfr        Destination transfer register array (normally 2 registers will be
 *                          used beginning at cr_xfr[0], but could use more for larger transfers)
 * @param cr_ctx        Crypto Dispatcher context whose bulk core will get the data and run command
 * @param idx           index to start placing output data within cr_xfr[] array
 *                          must be constant
 * @param sram_addr     source Crypto SRAM address supplying the data
 *                          can be constant or GPR
 * @param offs          offset to apply to sram_addr
 *                          must be constant
 * @param cib_addr      destination CIB word address where the data will be transferred to
 *                          Must be a constant (normally two fixed locations are used)
 * @param n             number of 16B words to transfer
 * @param exec_addr     optional 'start' address in CIB of sequence to execute after data transfer
 *                          must be a constant, CIB address (in units of 4B words with 0 = start of CIB)
 *                          use -- if doing a transfer-only with no sequence execution
 * @param base          optional base selection (0 or 1) applied if sequence is executed (NFP6xxxC only)
 *                          use -- or 0 for NFP32xxxC processors
 *
 * Assumes the transfer will be done with one instruction (size is less than 12 CIB addresses).
 *
 * This macro is used internally by the several other macros within the crypto support system.
 */
#macro _crypto_transfer_to_CIB_single (cr_xfr, cr_ctx, idx, sram_addr, offs, cib_addr, n, exec_addr, base)
    #if nfp_38xx==1
        #define_eval _start_spec (0xFE00 & (cib_addr << 7))
        #if !streq('exec_addr', '--')
            #define_eval _start_spec (_start_spec | (exec_addr))
        #endif
        #if (isnum(sram_addr) || isimport(sram_addr))
            #define_eval _load_spec ((sram_addr + offs) | (n-1))
            immed32 (cr_xfr[idx], (_load_spec | (_start_spec << 16)))
        #else
            .begin
                .reg _xx
                immed32 (_xx, (offs | (n-1) | (_start_spec << 16)))
                alu [cr_xfr[idx], _xx, OR, sram_addr]
            .end
        #endif

       #define_eval _nxt_idx (idx+1)
       #if !streq('exec_addr', '--')
           #if isnum(base) || isimport(base)
               #define_eval _upper_word (0x05030002 | (base & 1))
           #else
               #define_eval _upper_word (0x05030002)
           #endif
           or_shf_left (cr_xfr[_nxt_idx], _upper_word, cr_ctx, 20)
           #undef _upper_word
       #else
           or_shf_left (cr_xfr[_nxt_idx], 0x05030000, cr_ctx, 20)
       #endif
       #undef _nxt_idx

    #else

        #define_eval _start_spec (0x3F00 & (cib_addr << 6))
        #if !streq('exec_addr', '--')
            #define_eval _start_spec (_start_spec | 0x8000 | (exec_addr))
            #if isnum(base) || isimport(base)
                #define_eval _start_spec (_start_spec | ((base & 1) << 14))
            #endif
        #endif
        #if (isnum(sram_addr) || isimport(sram_addr))
            #define_eval _load_spec ((sram_addr + offs) | (n-1))
            #if isnum(base) || isimport(base)
                immed32 (cr_xfr[idx], (_load_spec | (_start_spec << 16)))
            #else
                .reg _xx
                immed32 (_xx, (_load_spec | (_start_spec << 16)))
                alu [cr_xfr[idx], _xx, OR, base, <<30]
            #endif
        #else
            .begin
                .reg _xx
                immed32 (_xx, (offs | (n-1) | (_start_spec << 16)))
                #if !isnum(base) && !isimport(base)
                    alu [_xx, _xx, OR, base, <<30]
                #endif
                alu [cr_xfr[idx], _xx, OR, sram_addr]
            .end
        #endif

       #define_eval _nxt_idx (idx+1)
       or_shf_left (cr_xfr[_nxt_idx], 0x05030000, cr_ctx, 20)
       #undef _nxt_idx
    #endif

    #define_eval _crypto_transfer_length 1
#endm
/// @endcond


/// @cond INTERNAL_MACROS
/**
 *  Transfer data from crypto SRAM to the core CIB buffer and optionally execute a sequence.
 *
 *  @param cr_xfr        Destination transfer register array (normally 2 registers will be
 *                          used beginning at cr_xfr[0], but could use more for larger transfers)
 *  @param cr_ctx        Crypto Dispatcher context whose bulk core will get the data and run command
 *  @param sram_addr     source Crypto SRAM address supplying the data
 *                          Can be constant or GPR
 *  @param cib_addr      destination CIB word address where the data will be transferred to
 *                          Must be a constant (normally two fixed locations are used)
 *  @param n             number of 16B words to transfer
 *  @param exec_addr     optional 'start' address in CIB of sequence to execute after data transfer
 *                          must be a constant, CIB address (in units of 4B words with 0 = start of CIB)
 *                          use -- if doing a transfer-only with no sequence execution
 *  @param base          optional base selection (0 or 1) applied if sequence is executed (NFP6xxxC only)
 *                          use -- or 0 for NFP32xxxC processors
 *
 *  Supports transfers of arbitrary length, by doing multiple transfer operations.  The last one will
 *  contain the code to start at the specified execution address, if there is one.
 *
 *  This macro is used internally by the several other macros within the crypto support system.
 */
#macro _crypto_transfer_to_CIB (cr_xfr, cr_ctx, sram_addr, cib_addr, n, exec_addr, base)
    #define_eval _rem_indx     0
    #define_eval _rem_xfer     (n)
    #define_eval _rem_offs     0
    #define_eval _rem_ciba     (cib_addr)
    #while (_rem_xfer >= 12)
        _crypto_transfer_to_CIB_single (cr_xfr, cr_ctx, _rem_indx, sram_addr, _rem_offs, _rem_ciba, 11, --, --)
        #define_eval _rem_ciba (_rem_ciba + 11)
        #define_eval _rem_offs (_rem_offs + 11*16)
        #define_eval _rem_xfer (_rem_xfer - 11)
        #define_eval _rem_indx (_rem_indx + (_crypto_transfer_length << 1))
    #endloop

    _crypto_transfer_to_CIB_single (cr_xfr, cr_ctx, _rem_indx, sram_addr, _rem_offs, _rem_ciba, _rem_xfer, exec_addr, base)
    #define_eval _crypto_transfer_length   (_crypto_transfer_length + (_rem_indx >> 1))
#endm
/// @endcond


/// @cond INTERNAL_MACROS
/**
 *  'Dynamically' run a compressed sequence as generated by the Crypto Assembler.
 *
 *  @param cr_xfr        Destination transfer register array (4 registers will be
 *                          used beginning at cr_xfr[0])
 *  @param cr_ctx        Crypto Dispatcher context which will be used to run the sequence
 *  @param sequence      Name of sequence to be run
 *  @param sram_addr     Crypto SRAM address to be used to hold parameter data and compressed code
 *                          Can be constant or GPR
 *  @param base          Optional base index (0 or 1) to pass to CIB run instruction (NFP6xxxC only)
 *                          Use 0 for NFP32xxC processor applications
 *                          Can be constant or GPR
 *
 *  'dynamic' sequence calling means the sequence is preloaded into crypto SRAM at init time,
 *  and then 'called' at run time by transferring the parameters into crypto SRAM immediately
 *  preceeding the compressed code, and then transferring parameters and code to CIB memory,
 *  and finally running the compressed instructions.
 *
 *  This macro is used internally by the more generic run_library_sequence() macro.
 */
#macro _crypto_run_dynamic (cr_xfr, cr_ctx, sequence, sram_addr, base)
    #define_eval _cib_exec (_cib_vars_size_/**/sequence + _cib_vars_base_/**/sequence)
    #define_eval _cib_dest (_cib_vars_base_/**/sequence)
    #define_eval _xfr_size ((_cib_xfer_size_/**/sequence + 3) >> 2)
    _crypto_transfer_to_CIB_single (cr_xfr, cr_ctx, 0, sram_addr, 0, _cib_dest, _xfr_size, --, base)
    _crypto_transfer_to_CIB_single (cr_xfr, cr_ctx, 2, sram_addr, 0, _cib_dest, 1, _cib_exec, base)
    #define_eval _crypto_transfer_length (1 + _crypto_transfer_length)
#endm
/// @endcond


/// @cond INTERNAL_MACROS
/**
 *  'Dynamically' run a compressed sequence as generated by the Crypto Assembler.
 *
 *  Invokes crypto_run_dynamic, above, with parameters:
 *    base set to "0"
 */
#macro _crypto_run_dynamic (cr_xfr, cr_ctx, sequence, sram_addr)
    _crypto_run_dynamic (cr_xfr, cr_ctx, sequence, sram_addr, 0)
#endm
/// @endcond


/// @cond INTERNAL_MACROS
/**
 *  'Statically' run a compressed sequence as generated by the Crypto Assembler.
 *
 *  @param cr_xfr        Destination transfer register array (2 registers will be
 *                          used beginning at cr_xfr[0])
 *  @param cr_ctx        Crypto Dispatcher context which will be used to run the sequence
 *  @param sequence      Name of sequence to be run
 *  @param sram_addr     Crypto SRAM address to be used to hold parameter variables (static sequences only)
 *                          Ignored for 'dynamic' loaded sequences (-- can be used in this case)
 *                          Can be constant or GPR
 *  @param preload       Address where the sequence was previously built/transferred into core CIB memory
 *  @param base          Optional base index (0 or 1) to pass to CIB run instruction (NFP6xxxC only)
 *                          Use 0 for NFP32xxC processor applications
 *                          Can be constant or GPR
 *
 *  'static' sequence calling means the sequence is preloaded into the bulk core's CIB memory
 *  at initialization time, and then 'called' at run time by transferring only the parameters
 *  to crypto SRAM and then to CIB memory, and running the address where the sequence was
 *  preloaded.
 *
 *  This macro is used internally by the more generic run_library_sequence() macro.
 */
#macro _crypto_run_static (cr_xfr, cr_ctx, sequence, sram_addr, preload, base)
    #define_eval _cib_dest (_cib_vars_base_/**/sequence)
    #define_eval _xfr_size ((_cib_vars_size_/**/sequence + 3) >> 2)
    _crypto_transfer_to_CIB (cr_xfr, cr_ctx, sram_addr, _cib_dest, _xfr_size, preload, base)
#endm
/// @endcond


/// @cond INTERNAL_MACROS
/**
 *  'Statically' run a compressed sequence as generated by the Crypto Assembler.
 *
 *  Invokes crypto_run_static, above, with parameters:
 *    base set to "0"
 */
#macro _crypto_run_static (cr_xfr, cr_ctx, sequence, sram_addr, preload)
    _crypto_run_static (cr_xfr, cr_ctx, sequence, sram_addr, preload, 0)
#endm
/// @endcond


#define crypto_load_address(seq) (seq & 0xFFFF)


/**
 *  Load the parameters for a library sequence into crypto SRAM to prepare for running the sequence.
 *
 *
 *  @param cr_xfr        Destination transfer register array (up to 4 registers will be
 *                          used beginning at cr_xfr[0])
 *  @param seq           Name of sequence to be run
 *  @param core          Core number used when the sequence was loaded into the library
 *  @param vars          Crypto SRAM address to be used to hold parameter variables (static sequences only)
 *                          Ignored for 'dynamic' loaded sequences (-- can be used in this case)
 *                          Can be constant or GPR
 *  @param sigspec       Optional signal specification, determines when the macro returns:
 *                          --          : creates a local signal and waits for it to be raised
 *                          signal name : returns immediately, caller is responsible for waiting for named signal
 *                          missing     : same as '--'
 *
 *  Calling sequence to execute a CIB sequence that has been placed into the compressed library:
 *     crypto_load_<sequence_name> ()  - used to place parameter data to be passed to the sequence, into transfer registers
 *     crypto_load_library_sequence () - writes the parameters to crypto SRAM
 *     crypto_run_library_sequence ()  - transfers parameters (and dynamic code) to bulk unit and runs the sequence
 */
#macro crypto_load_library_sequence (cr_xfr, seq, core, vars, sigspec)
    // tbd, check for length=0 which is the valid case where the sequence does not have any runtime parameters
    #if (_crypto_transfer_length > 0)
        #if ((seq/**/_desc_/**/core & (1 << CRYPTO_LIBDESC_STATIC_BIT)) == (1 << CRYPTO_LIBDESC_STATIC_BIT))
            // write parameter values to the specified var area in crypto SRAM
            crypto_send (write, cr_xfr, vars, 0, _crypto_transfer_length, sigspec)
        #else
            // write parameter values to this sequence's var area in crypto SRAM
            crypto_send (write, cr_xfr, crypto_load_address(seq/**/_desc_/**/core), 0, _crypto_transfer_length, sigspec)
        #endif
    #endif
#endm


/**
 *  Load the parameters for a library sequence into crypto SRAM to prepare for running the sequence.
 *
 *  Invokes crypto_load_library_sequence, above, with parameters:
 *    sigspec set to "--"
 */
#macro crypto_load_library_sequence (cr_xfr, seq, core, vars)
    crypto_load_library_sequence (cr_xfr, seq, core, vars, --)
#endm


/**
 *  Run a sequence previously loaded into the compressed library via library_load_static or library_load_dynamic
 *
 *  @param cr_xfr        Destination transfer register array (up to 4 registers will be
 *                          used beginning at cr_xfr[0])
 *  @param cr_ctx        Crypto Dispatcher context which will be used to run the sequence
 *  @param seq           Name of sequence to be run
 *  @param core          Core number used when the sequence was loaded into the library
 *  @param vars          Crypto SRAM address to be used to hold parameter variables (static sequences only)
 *                          Ignored for 'dynamic' loaded sequences (-- can be used in this case)
 *                          Can be constant or GPR
 *  @param base          Optional base index (0 or 1) to pass to CIB run instruction (NFP6xxxC only)
 *                          Use 0 for NFP32xxC processor applications
 *                          Can be constant or GPR
 *  @param sigspec       Optional signal specification, determines when the macro returns:
 *                          --          : creates a local signal and waits for it to be raised
 *                          signal name : returns immediately, caller is responsible for waiting for named signal
 *                          missing     : same as '--'
 */
#macro crypto_run_library_sequence (cr_xfr, cr_ctx, seq, core, vars, base, sigspec)
    #if ((seq/**/_desc_/**/core & (1 << CRYPTO_LIBDESC_STATIC_BIT)) == (1 << CRYPTO_LIBDESC_STATIC_BIT))
        #define_eval _cr_preload    (seq/**/_desc_/**/core & 0x3FF)
        _crypto_run_static (cr_xfr, cr_ctx, seq, vars, _cr_preload, base)
    #else
        #define_eval _cr_addr       (seq/**/_desc_/**/core & 0xFFFF)
        _crypto_run_dynamic (cr_xfr, cr_ctx, seq, _cr_addr, base)
    #endif
    crypto_send (write_fifo, cr_xfr, --, 0, _crypto_transfer_length, sigspec)
#endm


/**
 *  Run a sequence previously loaded into the compressed library via library_load_static or library_load_dynamic
 *
 *  Invokes crypto_run_library_sequence, above, with parameters:
 *    sigspec set to "--"
 */
#macro crypto_run_library_sequence (cr_xfr, cr_ctx, seq, core, vars, base)
    crypto_run_library_sequence (cr_xfr, cr_ctx, seq, core, vars, base, --)
#endm


/**
 *  Run a sequence previously loaded into the compressed library via library_load_static or library_load_dynamic
 *
 *  Invokes crypto_run_library_sequence, above, with parameters:
 *    base set to "0"
 *    sigspec set to "--"
 */
#macro crypto_run_library_sequence (cr_xfr, cr_ctx, seq, core, vars)
    crypto_run_library_sequence (cr_xfr, cr_ctx, seq, core, vars, 0, --)
#endm


/**
 * Generate island and PPID7 values.
 *
 *
 * @param out_island    Destination register to contain the island number on return
 *                          For NFP3xxxC processors '--' can be used, otherwise the supplied GPR will be set to zero
 * @param out_PPID7     Destination register to contain the PPID value, left-shift by 7 bits, on return
 *
 *  Used in support of signal-related macros in this library.
 */
#macro crypto_get_island_and_PPID7 (out_island, out_PPID7)
    .begin
    .reg _ctx_tmp, _me_tmp
    local_csr_rd [ACTIVE_CTX_STS]                           // get active context data (island, ME context, etc)
    immed [_ctx_tmp, 0]                                     // complete the local_csr_rd instruction
    #if CRYPTO_NFP_MODE==0
        // NFP32xx version
        alu_shf [_me_tmp, 0x0F, AND, _ctx_tmp, >>3]         // bits [3..0] = ME number within cluster
        alu_shf [_ctx_tmp, 0x0F, AND, _ctx_tmp, >>25]       // get cluster ID
        alu_shf [_ctx_tmp, _me_tmp, OR, _ctx_tmp, <<4]      // merge cluster and ME bits
        alu_shf [_ctx_tmp, --, B, _ctx_tmp, <<7]            // shift cluster/ME number left by 7 bits
        alu [out_PPID7, --, B, _ctx_tmp]                    // store result in destination register
        #if !streq('out_island','--')
            immed [out_island, 0]                           // island number is not used in this case
        #endif
    #else
        // NFP6xxx version
        alu_shf [out_island, 0x3F, AND, _ctx_tmp, >>25]     // isolate bits that make up the island number
        alu_shf [_me_tmp, 0x0F, AND, _ctx_tmp, >>3]         // bits [3..0] = ME number within island
        alu_shf [_ctx_tmp, --, B, _me_tmp, <<7]             // shift cluster/ME number left by 7 bits
        alu [out_PPID7, --, B, _ctx_tmp]                    // store result in destination register
    #endif
    .end
#endm


/// @cond INTERNAL_MACROS
/**
 *  Enqueue a return-signal op to the Crypt queue.
 *
 *  @param cr_xfr        Destination transfer register array (cr_xfr[0] and cr_xfr[1] will be used)
 *  @param cr_ctx        Crypto Dispatcher context which will be used to run the sequence
 *  @param me_ctx        ME context to receive the signal
 *                         can be constant or GPR
 *  @param island        island number (NFP6xxxC only) [as returned by get_island_and_PPID7()]
 *                         use -- for NFP3xxxC applications
 *  @param PPID7         local PPID left-shifted by 7 bits [as returned by get_island_and_PPID7()]
 *  @param sig           ME signal to be raised by the crypto unit
 *
 *  This macro populates the destination transfer registers with the crypto instruction
 *  code necessary to enqueue the return-signal operation, but it is up to the caller to actually
 *  write the instruction to the Dispatcher FIFO.
 *
 *  Used interally in support of other signal-related macros in this module.
 */
#macro _crypto_signal_return (cr_xfr, cr_ctx, me_ctx, island, PPID7, sig)
    .begin
    .reg xx
    #if (isnum(me_ctx) || isimport(me_ctx))
        immed [xx, (&sig | (me_ctx << 4))]      // build cmd word, from signal addr and context bits
    #else
        alu_shf [xx, &sig, OR, me_ctx, <<4]     // build cmd word, from signal addr and context bits
    #endif
    #if defined(CRYPTO_NFP_MODE) && (CRYPTO_NFP_MODE==1)
        // NFP6xxxC uses island, NFP3xxxC does not
        #if !streq('island', '--')
            alu_shf [xx, xx, OR, island, <<30]  // include bottom two bits of island ID, if specified
        #endif
    #endif
    alu [cr_xfr[0], xx, OR, PPID7]          // [15:0] contains PPID, ME context, and signal number

    #if (isnum(cr_ctx) || isimport(cr_ctx))
        immed32 (xx, ((0xa0+cr_ctx)<<20))
    #else
        alu [xx, cr_ctx, OR, 0xa0]
        alu_shf [xx, --, B, xx, <<20]
    #endif
    #if defined(CRYPTO_NFP_MODE) && (CRYPTO_NFP_MODE==1)
        // NFP6xxxC uses island, NFP3xxxC does not
        #if !streq('island', '--')              //  include top four bits of island ID, if specified
            alu_shf [cr_xfr[1], xx, OR, island, >>2]
        #else
            alu [cr_xfr[1], --, B, xx]
        #endif
    #else
        alu_shf [cr_xfr[1], --, B, xx]
    #endif
    .end
    #define_eval _crypto_transfer_length 1
#endm
/// @endcond


/// @cond INTERNAL_MACROS
/**
 *  Enqueue a return-signal op to the Crypt queue.
 *
 *  Invokes _crypto_signal_return, above, but generates island and PPID internally
 */
#macro _crypto_signal_return (cr_xfr, cr_ctx, me_ctx, sig)
    .begin
    .reg x_island, x_PPID7
    crypto_get_island_and_PPID7 (x_island, x_PPID7) // get data needed for generating signal
    _crypto_signal_return (cr_xfr, cr_ctx, me_ctx, x_island, x_PPID7, sig)
    .end
#endm
/// @endcond


/// @cond INTERNAL_MACROS
/**
 *  Enqueue a return-signal op to the DMA queue
 *
 *  @param cr_xfr      Destination transfer register array (cr_xfr[0] and cr_xfr[1] will be used)
 *  @param cr_ctx      Crypto Dispatcher context which will be used to run the sequence
 *  @param me_ctx      ME context to receive the signal
 *                        can be constant or GPR
 *  @param island      island number (NFP6xxxC only) [as returned by get_island_and_PPID7()]
 *                        use -- for NFP3xxxC applications
 *  @param PPID7       local PPID left-shifted by 7 bits [as returned by get_island_and_PPID7()]
 *  @param sig         ME signal to be raised by the crypto unit
 *
 *  This macro populates the destination transfer registers with the crypto instruction
 *  code necessary to enqueue the return-signal operation, but it is up to the caller to actually
 *  write the instruction to the Dispatcher FIFO.
 *
 * Used internally in support of other signal-related macros in this module.
 */
#macro _crypto_dma_signal_return (cr_xfr, cr_ctx, me_ctx, island, PPID7, sig)
    .begin
    .reg xx
    alu_shf [xx, &sig, OR, me_ctx, <<4]         // build cmd word, from signal addr and context bits
    #if defined(CRYPTO_NFP_MODE) && (CRYPTO_NFP_MODE==1)
        // NFP6xxxC uses island, NFP3xxxC does not
        alu_shf [xx, xx, OR, island, <<30]      // include bottom two bits of island ID
    #endif
    alu [cr_xfr[0], xx, OR, PPID7]              // [15:0] contains PPID, ME context, and signal number

    #if (isnum(cr_ctx) || isimport(cr_ctx))
        immed32 (xx, (((0xa0+cr_ctx)<<20) | (1<<14)))
    #else
        alu [xx, cr_ctx, OR, 0xa0]
        alu_shf [xx, --, B, xx, <<20]
        alu_shf [xx, xx, OR, 1, <<14]
    #endif
    #if defined(CRYPTO_NFP_MODE) && (CRYPTO_NFP_MODE==1)
        // NFP6xxxC uses island, NFP3xxxC does not
        alu_shf [cr_xfr[1], xx, OR, island, >>2] // include top four bits of island ID
    #else
        alu_shf [cr_xfr[1], --, B, xx]
    #endif
    .end
    #define_eval _crypto_transfer_length 1
#endm
/// @endcond


/// @cond INTERNAL_MACROS
/**
 *  Enqueue a return-signal op to the Dma queue.
 *
 *  Invokes _crypto_dma_signal_return, above, but generates island and PPID internally
 */
#macro _crypto_dma_signal_return (cr_xfr, cr_ctx, me_ctx, sig)
    .begin
    .reg x_island, x_PPID7
    get_island_and_PPID7 (x_island, x_PPID7)    // get data needed for generating signal
    _dma_signal_return (cr_xfr, cr_ctx, me_ctx, x_island, x_PPID7, sig)
    .end
#endm
/// @endcond


/**
 *  Enqueue and send a return-signal operation, with flexible signal support.
 *
 *  @param cr_xfr        Destination transfer register array (cr_xfr[0] and cr_xfr[1] will be used)
 *  @param cr_ctx        Crypto Dispatcher context which will generate the signal
 *  @param me_ctx        ME context to receive the signal
 *                          can be constant or GPR
 *  @param island        island number (NFP6xxxC only) [as returned by get_island_and_PPID7()]
 *                          use -- for NFP3xxxC applications
 *  @param PPID7         local PPID left-shifted by 7 bits [as returned by get_island_and_PPID7()]
 *  @param sigspec       ME signal to be raised by the crypto unit
 *                          use -- to generate a local signal and wait for it
 *                          use a signal name to enqueue that signal and return without waiting
 *  @param cmdsigspec    ME signal to be raised when the command has been delivered to the crypto FIFO
 *                          use -- to generate a local signal and wait for it
 *                          use a signal name to use that signal and return without waiting
 *
 *  If sigspec is set to "--", cmdsigspec parameter is ignored and "--" is assumed
 */
#macro crypto_send_signal (cr_xfr, cr_ctx, me_ctx, island, PPID7, sigspec, cmdsigspec)
    #if streq('sigspec','--')
        // send 'signal-return' instruction using a locally-defined signal, and wait for it
        .begin
           .sig _cr_sig
            _cryto__signal_return (cr_xfr, cr_ctx, me_ctx, island, PPID7, _cr_sig)
            .set_sig _cr_sig
            crypto_send (write_fifo, cr_xfr, --, 0, _crypto_transfer_length)
            ctx_arb [_cr_sig]
        .end
    #else
        // send the 'signal-return' instruction using specified signal, and return without waiting
        _crypto__signal_return (cr_xfr, cr_ctx, me_ctx, island, PPID7, sigspec)
        crypto_send (write_fifo, cr_xfr, --, 0, _crypto_transfer_length, cmdsigspec)
        .set_sig sigspec
    #endif
#endm


/**
 *  Enqueue and send a return-signal operation, with flexible signal support.
 *
 *  Invokes crypto_send_signal, above, with parameters:
 *    cmdsigspec set to "--"
 */
#macro crytpo_send_signal (cr_xfr, cr_ctx, me_ctx, island, PPID7, sigspec)
    crypto_send_signal (cr_xfr, cr_ctx, me_ctx, island, PPID7, sigspec, --)
#endm


/**
 *  Enqueue and send a return-signal operation to the local thread.
 *
 *  Invokes crypto_send_signal, above, with parameters:
 *    signal generated from get_island_and_PPID7
 *    PPID7  generated from get_island_and_PPID7
 */
#macro crypto_send_signal (cr_xfr, cr_ctx, me_ctx, sigspec, cmdsigspec)
    .begin
    .reg x_island, x_PPID7
    get_island_and_PPID7 (x_island, x_PPID7)    // get data needed for generating signal
    send_crypto_signal (cr_xfr, cr_ctx, me_ctx, x_island, x_PPID7, sigspec, cmdsigspec)
    .end
#endm


/**
 *  Enqueue and send a return-signal operation to the local thread.
 *
 *  Invokes crypto_send_signal, above, with parameters:
 *    cmdsigspec set to "--"
 *    signal generated from get_island_and_PPID7
 *    PPID7  generated from get_island_and_PPID7
 */
#macro crypto_send_signal (cr_xfr, cr_ctx, me_ctx, sigspec)
    crypto_send_signal (cr_xfr, cr_ctx, me_ctx, sigspec, --)
#endm


/**
 *  Enqueue and send a return-signal operation to a selected dispatch queue
 *
 *  @param cr_xfr        Destination transfer register array (cr_xfr[0] and cr_xfr[1] will be used)
 *  @param cr_ctx        Crypto Dispatcher context which will be used to run the sequence
 *  @param me_ctx        ME context to receive the signal
 *                          can be constant or GPR
 *  @dispatch             0 = normal / Bulk Core queue, 1 = 'chaining' / DMA Out queue
 *  @param island        island number [as returned by get_island_and_PPID7()]
 *  @param PPID7         local PPID left-shifted by 7 bits [as returned by get_island_and_PPID7()]
 *  @param sigspec       ME signal to be raised by the crypto unit
 *                          use -- to generate a local signal and wait for it
 *                          use a signal name to enqueue that signal and return without waiting
 *  @param iosigspec     ME signal to use for the I/O (write_fifo) operation
 *                          use -- to generate a local signal and wait for it
 *                          use a signal name to enqueue that signal and return without waiting
 *
 *  For use with NFP6xxx series processors only.
 */
#macro crypto_send_done_signal (cr_xfr, cr_ctx, me_ctx, dispatch, island, PPID7, sigspec, iosigspec)
    #if (!(IS_NFPTYPE(__NFP6000) || IS_NFPTYPE(__NFP3800)))
        #error "crytpo_send_done_signal only supported on NFP6XXX."
    #endif */
    #if streq('sigspec','--')
        // send 'signal-return' instruction using a locally-defined signal, and wait for it
        .begin
           .sig volatile _cr_sig
            #if (dispatch==0)
                _crypto_signal_return (cr_xfr, cr_ctx, me_ctx, island, PPID7, _cr_sig)
            #else
                _crypto_dma_signal_return (cr_xfr, cr_ctx, me_ctx, island, PPID7, _cr_sig)
            #endif
            .set_sig _cr_sig
	    crypto_send (write_fifo, cr_xfr, --, 0, _crypto_transfer_length, iosigspec)
            ctx_arb [_cr_sig]
        .end
    #else
        // send the 'signal-return' instruction using specified signal, and return without waiting
        #if (dispatch==0)
            _crypto_signal_return (cr_xfr, cr_ctx, me_ctx, island, PPID7, sigspec)
        #else
            _crypto_dma_signal_return (cr_xfr, cr_ctx, me_ctx, island, PPID7, sigspec)
        #endif
       .set_sig sigspec
       crypto_send (write_fifo, cr_xfr, --, 0, _crypto_transfer_length, iosigspec)
    #endif
#endm


/**
 *  Enqueue and send a return-signal operation to a selected dispatch queue
 *
 *  Invokes crypto_send_done_signal, above, with parameters:
 *    iosigspec set to "--"
 */
#macro crypto_send_done_signal (cr_xfr, cr_ctx, me_ctx, dispatch, island, PPID7, sigspec)
    crypto_send_done_signal (cr_xfr, cr_ctx, me_ctx, dispatch, island, PPID7, sigspec, --)
#endm


/**
 *  Enqueue and send a return-signal operation to a selected dispatch queue
 *
 *  Invokes crypto_send_done_signal, above, with parameters:
 *    signal generated from get_island_and_PPID7
 *    PPID7  generated from get_island_and_PPID7
 */
#macro crypto_send_done_signal (cr_xfr, cr_ctx, me_ctx, dispatch, sigspec, iosigspec)
    .begin
    .reg x_island, x_PPID7
    crypto_get_island_and_PPID7 (x_island, x_PPID7)
    crypto_send_done_signal (cr_xfr, cr_ctx, me_ctx, dispatch, x_island, x_PPID7, sigspec, iosigspec)
    .end
#endm


/**
 *  Enqueue and send a return-signal operation to a selected dispatch queue
 *
 *  Invokes crypto_send_done_signal, above, with parameters:
 *    iosigspec set to "--"
 *    signal generated from get_island_and_PPID7
 *    PPID7  generated from get_island_and_PPID7
 */
#macro crypto_send_done_signal (cr_xfr, cr_ctx, me_ctx, dispatch, sigspec)
    crypto_send_done_signal (cr_xfr, cr_ctx, me_ctx, dispatch, sigspec, --)
#endm


/**
 *  Initialize the process of preparing the bulk core for compressed-library loading and usage.
 *
 *  @param stat          starting location in crypto SRAM where 'static' sequences will be staged for
 *                          transfer to bulk core CIB space.  This is a temporary buffer that should
 *                          be available for exclusive use by one crypto Dispatcher context during the
 *                          initialization and library loading process.  Static sequences reside in
 *                          the CIB space of each core, for the life of the program, and as such can
 *                          be accessed immediately on-demand as needed.
 *  @param dyn           starting location in crypto SRAM where 'dynamic' sequences will be loaded.
 *                          Dynamic sequences are code sequences that are kept in crypto SRAM long-term
 *                          (i.e. for the life of the program), and are copied to the core CIB only
 *                          when needed.  They allow the use of a larger compressed library since the
 *                          compressed code for all the sequences reside in SRAM rather than CIB space.
 *  @param code          starting location within the CIB space, where static compressed sequences will
 *                          be loaded.  This is a word address (in units of 4-byte words) and must be
 *                          set to a value beyond the space reserved for constants and variables.
 **/
#macro crypto_init_CIB_setup (stat, dyn, code)
    #define_eval static_load_core    (code)
    #define_eval dynamic_load_sram   (dyn)
    #define_eval static_load_sram    (stat)

    // clean up any _static_leftover_X #defines from previous library setup calls
    #if defined(_static_leftover_0)
        #undef _static_leftover_0
    #endif
    #if defined(_static_leftover_1)
        #undef _static_leftover_1
    #endif
    #if defined(_static_leftover_2)
        #undef _static_leftover_2
    #endif
#endm


/**
 *  Load CIB memory of a crypto bulk core unit with constants needed for compressed code sequences.
 *
 *  @param cr_xfr        Destination transfer register array.  Normally 2 registers will be
 *                          used beginning at cr_xfr[0], more may be used depending on number
 *                          of constants defined.
 *  @param cr_ctx        Crypto Dispatcher context to do the transfer (the context must already
 *                          have a bulk core allocated to it via get_core().
 *  @param n             Number of constants to load, valid values 0 - 6
 *  @param b0-b5         Optional base values.  These supply the base Crypto SRAM addresses for
 *                          'based' variables used by the crypto-library sequences.  Up to 6
 *                          base values are supported.  The user must supply the same number
 *                          of base values as are used in the crypto-library.
 *
 *  Constants are presented in the crypto-library file similarly to a 'static' CIB sequence, with a sequence
 *  name of 'compr_constants'.  library_load_constants() must be called first after init_CIB_setup(), before
 *  static-mode sequences are loaded via library_load_static().  Note that library_load_constants() must be
 *  called even if no static sequences are going to be loaded, because constants are also used by dynamic
 *  sequences.
 */
#macro crypto_library_load_constants (cr_xfr, cr_ctx, n, base0, base1, base2, base3, base4, base5)
    // Generate data containing the CIB constants and put it into transfer registers.
    // Supports up to 6 'base constants' which are used to modify constant data.
    #if n==0
        crypto_gen_compr_constants ()
    #elif n==1
        crypto_gen_compr_constants (base0)
    #elif n==2
        crypto_gen_compr_constants (base0, base1)
    #elif n==3
        crypto_gen_compr_constants (base0, base1, base2)
    #elif n==4
        crypto_gen_compr_constants (base0, base1, base2, base3)
    #elif n==5
        crypto_gen_compr_constants (base0, base1, base2, base3, base4)
    #elif n==6
        crypto_gen_compr_constants (base0, base1, base2, base3, base4, base5)
    #endif

    #define_eval _preserve_static_load_core (static_load_core)
    #define_eval static_load_core    (_cib_base_addr_compr_constants)
    #define_eval _library_descriptor (static_load_core | (1 << CRYPTO_LIBDESC_STATIC_BIT))
    #if ((!isnum(cr_ctx) && !isimport(cr_ctx)) || (cr_ctx!=0x10000))
        _crypto_library_transfer_static_code (cr_xfr, cr_ctx, 0, compr_constants, 0)
    #endif
    #define cib_descriptor_compr_constants _library_descriptor
    #define_eval static_load_core    (_preserve_static_load_core)

    // clean up any _static_leftover_X #defines from above call to _library_transfer_static_code
    #if defined(_static_leftover_0)
        #undef _static_leftover_0
    #endif
    #if defined(_static_leftover_1)
        #undef _static_leftover_1
        #endif
        #if defined(_static_leftover_2)
            #undef _static_leftover_2
    #endif
#endm


/**
 *  Load CIB memory of a crypto bulk core unit with constants needed for compressed code sequences.
 *
 *  Invokes crypto_library_load_constants, above, with parameters:
 *    n set to 0
 *    b0-b5 set to --
 */
#macro crypto_library_load_constants (cr_xfr, cr_ctx)
    crypto_library_load_constants (cr_xfr, cr_ctx, 0, --, --, --, --, --, --)
#endm


/**
 *  Load CIB memory of a crypto bulk core unit with constants needed for compressed code sequences.
 *
 *  Invokes crypto_library_load_constants, above, with parameters:
 *    n set to 1
 *    b1-b5 set to --
 */
#macro crypto_library_load_constants (cr_xfr, cr_ctx, b0)
    crypto_library_load_constants (cr_xfr, cr_ctx, 1, b0, --, --, --, --, --)
#endm


/**
 *  Load CIB memory of a crypto bulk core unit with constants needed for compressed code sequences.
 *
 *  Invokes crypto_library_load_constants, above, with parameters:
 *    n set to 2
 *    b2-b5 set to --
 */
#macro crypto_library_load_constants (cr_xfr, cr_ctx, b0, b1)
    crypto_library_load_constants (cr_xfr, cr_ctx, 2, b0, b1, --, --, --, --)
#endm


/**
 *  Load CIB memory of a crypto bulk core unit with constants needed for compressed code sequences.
 *
 *  Invokes crypto_library_load_constants, above, with parameters:
 *    n set to 3
 *    b3-b5 set to --
 */
#macro crypto_library_load_constants (cr_xfr, cr_ctx, b0, b1, b2)
    crypto_library_load_constants (cr_xfr, cr_ctx, 3, b0, b1, b2, --, --, --)
#endm


/**
 *  Load CIB memory of a crypto bulk core unit with constants needed for compressed code sequences.
 *
 *  Invokes crypto_library_load_constants, above, with parameters:
 *    n set to 4
 *    b4-b5 set to --
 */
#macro crypto_library_load_constants (cr_xfr, cr_ctx, b0, b1, b2, b3)
    crypto_library_load_constants (cr_xfr, cr_ctx, 4, b0, b1, b2, b3, --, --)
#endm


/**
 *  Load CIB memory of a crypto bulk core unit with constants needed for compressed code sequences.
 *
 *  Invokes crypto_library_load_constants, above, with parameters:
 *    n set to 5
 *    b5 set to --
 */
#macro crypto_library_load_constants (cr_xfr, cr_ctx, b0, b1, b2, b3, b4)
    crypto_library_load_constants (cr_xfr, cr_ctx, 5, b0, b1, b2, b3, b4, --)
#endm


/**
 *  Load CIB memory of a crypto bulk core unit with constants needed for compressed code sequences.
 *
 *  Invokes crypto_library_load_constants, above, with parameters:
 *    n set to 6
 */
#macro crypto_library_load_constants (cr_xfr, cr_ctx, b0, b1, b2, b3, b4, b5)
    crypto_library_load_constants (cr_xfr, cr_ctx, 6, b0, b1, b2, b3, b4, b5)
#endm


/**
 *  Add a crypto-library sequence to the run-time-available CIB sequences
 *
 *  @param LOAD_TYPE     Use either CRYPTO_LOAD_STATIC or CRYPTO_LOAD_DYNAMIC to select run type
 *  @param cr_xfr        Destination transfer register array.  Normally 2 registers will be
 *                        used beginning at cr_xfr[0], more may be used depending on number
 *                        of instructions the library sequence contains.
 *  @param cr_ctx        Crypto Dispatcher context to do the transfer (the context must already
 *                        have a bulk core allocated to it via get_core().
 *  @param seq           Name of sequence to be added to the run-time library.
 *  @param core          Bulk core number must be a constant.  This is only used if a different
 *                        set of static sequences is being loaded to each core, a non-typical
 *                        situation. The value must be 0-3 for NFP32xxxC, 0-5 for NFP6xxxC.
 *
 *  Sequences can be loaded for 'static' or 'dynamic' use models. This macro calls either
 *  _crypto_library_load_static or _crypto_library_load_dynamic, depending on the LOAD_TYPE parameter.
 */
#macro crypto_library_load (LOAD_TYPE, xfr, ctx, seq, core)
    #if (LOAD_TYPE == CRYPTO_LOAD_STATIC)
        _crypto_library_load_static (xfr, ctx, seq, core)
    #elif (LOAD_TYPE == CRYPTO_LOAD_DYNAMIC)
        _crypto_library_load_dynamic (xfr, seq, core)
    #else
        #error 4 "crypto_library_load macro called with invalid load type" LOAD_TYPE
    #endif
#endm


/**
 *  Add a crypto-library sequence to the run-time-available CIB sequences
 *
 *  Invokes crypto_library_load, above, with parameters:
 *    core set to 0
 */
#macro  crypto_library_load (LOAD_TYPE, xfr, ctx, seq)
    crypto_library_load (LOAD_TYPE, xfr, ctx, seq, 0)
#endm


/// @cond INTERNAL_MACROS
/**
 *  Add a 'static use' crypto-library sequence to the run-time-available CIB sequences
 *
 *  @param cr_xfr        Destination transfer register array.  Normally 2 registers will be
 *                          used beginning at cr_xfr[0], more may be used depending on number
 *                          of instructions the library sequence contains.
 *  @param cr_ctx        Crypto Dispatcher context to do the transfer (the context must already
 *                          have a bulk core allocated to it via get_core().  See note below.
 *  @param seq           Name of sequence to be transferred to the CIB buffer.
 *  @param core          Optional constant specifying bulk core number.  Used only if a different
 *                          set of static sequences is being loaded to each core, a non-typical
 *                          situation.  The value must be 0-3 for NFP32xxxC, 0-5 for NFP6xxxC.
 *
 *  'Static use model' means that the compressed code for the sequence will be transferred to the
 *  bulk core unit's CIB buffer space so that it can be called as needed during run-time.
 *
 *  This macro is used internally by the crypto_library_load() macro.
 *
 *  cr_ctx can be a GPR or a numeric constant.  If a numeric constant is used, it can be 0-15 to
 *  specify a single context; or, it can have bit 16 set, which indicates that bits 15:0 are to be
 *  interpreted as a bit mask where a '1' in each bit position b indicates that context b will be
 *  used to transfer the opcodes.  In this bit-mask mode, multiple bits can be set, which allows
 *  multiple cores to be initialized with one call and with one set of transfer registers.
 *
 *  This macro maintains a #defined value in static_load_core, which is updated for every sequence
 *  added, to point to the next available CIB location.
 *
 *  On exit, there should be a #defined value (created by the source crypto-library function itself),
 *  named <seq>_desc_N where <seq> is the sequence name and N is the core number.  The value defined
 *  contains all the information needed by the rest of the crypto support subsystem to load and call
 *  the sequence, including the fact that it was added to the library as a 'static'.
 */
#macro _crypto_library_load_static (cr_xfr, cr_ctx, seq, core)
    #define_eval _static_xfr 0

    // Handle 'leftovers' from any previous sequence being loaded by this macro
    #if defined(_static_leftover_0)
        immed32 (cr_xfr[_static_xfr], _static_leftover_0)
        #define_eval _static_xfr (_static_xfr+1)
    #endif
    #if defined(_static_leftover_1)
        immed32 (cr_xfr[_static_xfr], _static_leftover_1)
        #define_eval _static_xfr (_static_xfr+1)
    #endif
    #if defined(_static_leftover_2)
        immed32 (cr_xfr[_static_xfr], _static_leftover_2)
        #define_eval _static_xfr (_static_xfr+1)
    #endif

    // Generate opcode data using output macro produced by crypto assembler
    #define_eval _library_descriptor (static_load_core | (1 << CRYPTO_LIBDESC_STATIC_BIT))
    crypto_gen_/**/seq (core, _library_descriptor)
    #if ((!isnum(cr_ctx) && !isimport(cr_ctx)) || (cr_ctx!=0x10000))
        _crypto_library_transfer_static_code (cr_xfr, cr_ctx, _static_xfr, seq, core)
    #endif
    #undef _static_xfr
#endm
/// @endcond


/// @cond INTERNAL_MACROS
/**
 *  Add a 'static use' crypto-library sequence to the run-time-available CIB sequences
 *
 *  Invokes _crypto_library_load_static, above, with parameters:
 *    core set to 0
 */
#macro _crypto_library_load_static (cr_xfr, cr_ctx, seq)
    _crypto_library_load_static (cr_xfr, cr_ctx, seq, 0)
#endm
/// @endcond


/// @cond INTERNAL_MACROS
/**
 *  Transfer opcode or constants data into the bulk core CIB space.
 *
 *  @param cr_xfr        Destination transfer register array.  Normally 2 registers will be
 *                          used beginning at cr_xfr[indx], more may be used depending on the
 *                          number of instructions the library sequence contains.
 *  @param cr_ctx        Crypto Dispatcher context to do the transfer (the context must already
 *                          have a bulk core allocated to it via get_core().
 *  @param indx          Index to apply to cr_xfr for starting transfer register.
 *  @param seq           Name of sequence to be transferred to the CIB buffer.
 *  @param core          Bulk core number; must be a constant.  This is only used if a different
 *                          set of static sequences is being loaded to each core, a non-typical
 *                          situation. The value must be 0-3 for NFP32xxxC, 0-5 for NFP6xxxC.
 *
 * This macro prepares for the next static sequence load by setting up 'leftover' defines.
 *
 * This macro is used internally by _library_load_static() and library_load_constants() macros.
 */
#macro _crypto_library_transfer_static_code (cr_xfr, cr_ctx, indx, seq, core)

    // Transfer generated opcode data for the specified sequence and put it into transfer registers
    #define_eval _xfr       (indx)
    #define_eval _gen       (_cib_vars_size_/**/seq)
    #define_eval _gen_end   (_gen + (_cib_code_size_/**/seq))
    #while (_gen < _gen_end)
        immed32 (cr_xfr[_xfr], gen_data_/**/_gen)
        #define_eval _xfr (_xfr+1)
        #define_eval _gen (_gen+1)
    #endloop

    // Force 4 lword alignment for sequences in CIB; should not be
    // needed but was needed prior to leftover fix for 2 lword
    // sequences. Uses more CIB memory if enabled
    #define pad_cib_to_4W       0
    #ifdef pad_cib_to_4W
        #while (_xfr & 0x03) > 0
            immed32 (cr_xfr[_xfr], 0)
            #define_eval _xfr (_xfr+1)
            #define_eval _gen (_gen+1)
        #endloop
    #endif

    // If we have at least 4 words to transfer, then 'old' leftovers are no longer needed/valid
    #if (_xfr >= 4)
        #if defined(_static_leftover_0)
            #undef _static_leftover_0
        #endif
        #if defined(_static_leftover_1)
            #undef _static_leftover_1
        #endif
        #if defined(_static_leftover_2)
            #undef _static_leftover_2
        #endif
    #endif


    // Generate 'leftovers', which are just defines for opcodes beyond the last 4-word boundary in CIB space
    #define_eval _gen_extra (_xfr & 0x03)
    #define_eval _gen       (_gen - _gen_extra)
    #if (_gen_extra == 1)
        #if !defined(_static_leftover_0)
            #define_eval _static_leftover_0 (gen_data_/**/_gen)
        #endif
        #if defined(_static_leftover_1)
            #undef _static_leftover_1
        #endif
        #if defined(_static_leftover_2)
            #undef _static_leftover_2
        #endif
        #define_eval _gen (_gen+4)
        immed32 (cr_xfr[_xfr], 0)
        #define_eval _xfr (_xfr+1)
        immed32 (cr_xfr[_xfr], 0)
        #define_eval _xfr (_xfr+1)
        immed32 (cr_xfr[_xfr], 0)
        #define_eval _xfr (_xfr+1)
    #elif (_gen_extra == 2)
        #if !defined(_static_leftover_0)
            #define_eval _static_leftover_0 (gen_data_/**/_gen)
        #endif
        #define_eval _gen (_gen+1)
        #if !defined(_static_leftover_1)
            #define_eval _static_leftover_1 (gen_data_/**/_gen)
        #endif
        #define_eval _gen (_gen+3)
        #if defined(_static_leftover_2)
            #undef _static_leftover_2
        #endif
        immed32 (cr_xfr[_xfr], 0)
        #define_eval _xfr (_xfr+1)
        immed32 (cr_xfr[_xfr], 0)
        #define_eval _xfr (_xfr+1)
    #elif (_gen_extra == 3)
        #if !defined(_static_leftover_0)
            #define_eval _static_leftover_0 (gen_data_/**/_gen)
        #endif
        #define_eval _gen (_gen+1)
        #if !defined(_static_leftover_1)
            #define_eval _static_leftover_1 (gen_data_/**/_gen)
        #endif
        #define_eval _gen (_gen+1)
        #if !defined(_static_leftover_2)
            #define_eval _static_leftover_2 (gen_data_/**/_gen)
        #endif
        #define_eval _gen (_gen+2)
        immed32 (cr_xfr[_xfr], 0)
        #define_eval _xfr (_xfr+1)
    #endif  // generate leftovers

    // Write the opcode data into crypto SRAM staging area
    #define_eval _crypto_transfer_length    (_xfr >> 1)
    crypto_send (write, cr_xfr, static_load_sram, 0)

    // Transfer the opcode data into the core CIB space
    #define_eval _xfr_size (_xfr >> 2)
    #define_eval _cib_dest (crypto_load_address(_library_descriptor))
    #if ((isnum(cr_ctx) || isimport(cr_ctx)) && ((cr_ctx & 0x10000) == 0x10000))
        #define_eval _ctx 0
        #while (_ctx < 16)
            #if ((cr_ctx & (1 << _ctx)) != 0)
                _crypto_transfer_to_CIB (cr_xfr, _ctx, static_load_sram, _cib_dest, _xfr_size, --, 0)
                crypto_send (write_fifo, cr_xfr, --, 0)
            #endif
            #define_eval _ctx (_ctx + 1)
        #endloop
    #else
        _crypto_transfer_to_CIB (cr_xfr, cr_ctx, static_load_sram, _cib_dest, _xfr_size, --, 0)
        crypto_send (write_fifo, cr_xfr, --, 0)
    #endif

    // Clean-up / prepare for exit
    #define_eval static_load_core       (static_load_core+_cib_code_size_/**/seq)

    // pad to 4-word boundaries
    #ifdef pad_cib_to_4W
        #define_eval static_load_core  ((static_load_core+0x03) & 0xFFFC)
    #endif

    #if (static_load_core > CRYPTO_INSTRUCTION_BUFFER_SIZE)
        #error "Crypto compressed instruction buffer size exceeded"
    #endif

    #undef _xfr
    #undef _gen
    #undef _gen_end
#endm
/// @endcond


/// @cond INTERNAL_MACROS
/**
 *  Add a 'dynamic use' crypto-library sequence to the run-time-available CIB sequences
 *
 *  @param cr_xfr        Destination transfer register array.  Normally 2 registers will be
 *                          used beginning at cr_xfr[0], more may be used depending on number
 *                          of instructions the library sequence contains.
 *  @param seq           Name of sequence to be transferred to the CIB buffer.
 *  @param core          Optional constant specifying bulk core number.  Used only if a different
 *                          set of dynamic sequences is being prepared for each core, a non-typical
 *                          situation.  The value must be 0-3 for NFP32xxxC, 0-5 for NFP6xxxC.
 *
 *  'Dynamic Use' means that the compressed code for the sequence will be transferred to the
 *  crypto SRAM and a known constant location, so that when needed it can be transferred to CIB
 *  space and executed.
 *
 *  This macro is used internally by the crypto_library_load() macro.
 *
 *  This macro maintains a #defined value in dynamic_load_core, which is updated for every sequence
 *  added, to point to the next available Crypto SRAM location.
 *
 *  On exit, there should be a #defined value (created by the source crypto-library function itself),
 *  named <seq>_desc_N where <seq> is the sequence name and N is the core number.  The value defined
 *  contains all the information needed by the rest of the crypto support subsystem to load and call
 *  the sequence, including the fact that it was added to the library as a 'dynamic'.
 */
#macro _crypto_library_load_dynamic (cr_xfr, seq, core)
    #define_eval _library_descriptor (dynamic_load_sram)
    #define_eval _xfr 0

    // Generate opcode data for the specified sequence and put it into transfer registers
    crypto_gen_/**/seq (core, _library_descriptor)
    #define_eval _gen       0
    #while (_gen < _cib_xfer_size_/**/seq)
        immed32 (cr_xfr[_xfr], gen_data_/**/_gen)
        #define_eval _xfr (_xfr+1)
        #define_eval _gen (_gen+1)
    #endloop

    #define_eval dynamic_load_sram          (dynamic_load_sram + (_cib_xfer_size_/**/seq << 2))
    #define_eval _crypto_transfer_length    (_xfr >> 1)
    crypto_send (write, cr_xfr, crypto_load_address(_library_descriptor), 0)
#endm
/// @endcond


/// @cond INTERNAL_MACROS
/**
 *  Add a 'dynamic use' crypto-library sequence to the run-time-available CIB sequences
 *
 *  Invokes _crypto_library_load_dynamic, above, with parameters:
 *    core set to 0
 */
#macro _crypto_library_load_dynamic (cr_xfr, seq)
    _crypto_library_load_dynamic (cr_xfr, seq, 0)
#endm
/// @endcond


/// @cond INTERNAL_MACROS
/**
 *  Attach a numeric value as a bitfield in a 32-bit constant
 *
 *  @param p      Is the value of the bitfield.  If p is not numeric, then this macro does nothing.
 *  @param prefix Is the prefix name of the set of #defines that describe the bitfield.
 *
 *  This macro is used in support of other macros within this file that build a numeric constant
 *  from a set of bitfields.  It takes as an argument, the prefix of the name of a bitfield definition.
 *  This prefix is then used to generate #defined names.  The prefix concatenated with 'mask' will
 *  be used as the #defined name of the mask for the bitfield.  If the mask is equal to 1, then the
 *  field is a single bit and there should exist a #define for the prefix concatenated with 'bit'.
 *  If the mask is greater than 1, then the field is a multiple bit field and there shoud exist a
 *  #define for the prefix concatenated with 'shift'.  These conventions are followed in system files
 *  such as nfp_pcie.h.
 *
 *  The result of this macro is that the variable _bits_temp gets updated to include the incoming
 *  bitfield value, if the value is numeric.  Otherwise, _bits_temp is unmodified.  The caller
 *  of this macro is presumed to have initialized _bits_temp before calling the macro.
 */
#macro _crypto_gen_bitfield_num (p, prefix)
    #if is_numeric(p)
        #if (prefix/**/mask) > 1
            #define_eval _bits_temp (_bits_temp | (((p) & prefix/**/mask) << prefix/**/shift))
        #else
            #define_eval _bits_temp (_bits_temp | (((p) & prefix/**/mask) << prefix/**/bit))
        #endif
    #endif
#endm
/// @endcond


/// @cond INTERNAL_MACROS
/**
 *  Attach a register-based value as a bitfield in a 32-bit destination register
 *
 *  @param xreg   Is a register that will have the new bitfield merged into it.
 *  @param p      Is the value of the bitfield.  If p is numeric, then this macro does nothing.
 *  @param prefix Is the prefix name of the set of #defines that describe the bitfield.

 *  This macro is used in support of other macros within this file that build a value
 *  from a set of bitfields.  It takes as an argument, the prefix of the name of a bitfield definition.
 *  This prefix is then used to generate #defined names.  The prefix concatenated with 'mask' will
 *  be #defined as the mask for the bitfield.  If the mask is equal to 1, then the field is a single
 *  bit and there will exist a #define for the prefix concatenated with 'bit'.  If the mask is greater
 *  than 1, then the field is a multiple bit field and there will exist a #define for the prefix
 *  concatenated with 'shift'.  These conventions are followed in system files such as nfp_pcie.h.
 */
#macro _crypto_attach_bitfield_reg (xreg, p, prefix)
    #if !is_numeric(p)
        .begin
        .reg _bits_yy
        #if (prefix/**/mask) > 1
            #if ((prefix/**/mask) > 0xFF)
                immed32 (_bits_yy, prefix/**/mask)
                alu [_bits_yy, _bits_yy, AND, p]
            #else
                alu [_bits_yy, p, AND, prefix/**/mask]
            #endif
            alu_shf [xreg, xreg, OR, _bits_yy, <<prefix/**/shift]
        #else
            alu [_bits_yy, p, AND, prefix/**/mask]
            alu_shf [xreg, xreg, OR, _bits_yy, <<prefix/**/bit]
        #endif
        .end
    #endif
#endm
/// @endcond


/**
 * Generate a crypto DMA instruction (cycle 1)
 *
 * @b Example:
 * @code
 *  .reg $io[4]
 *  .xfer_oreder $io
 *  crypto_setup_dma_desc_cycle1 ($io[0], $io[1], ...)
 *  crypto_setup_dma_desc_cycle2 ($io[2], $io[3], ...)
 *  crypto [write_fifo, $io[0], x, 0, 2]
 * @endcode
 *
 * @param dest_lo  register to receive the result, low 32-bits (this can be a transfer register)
 * @param dest_hi  register to receive the result, high 32-bits (this can be a transfer register)
 * @param ctx      crypto Dispatcher context to receive the resulting DMA instruction ( GPR or CONST)
 * @param xlen     DMA transfer length, minus 1, in 8-byte words (0 to 255 valid) (0 transfers 8 bytes) (GPR or CONST)
 * @param prefetch set to 1 for prefetch/instruction chaining, 0 otherwise (GPR or CONST)
 * @param addr_lo  system memory address, low word ( GPR or CONST)
 * @param addr_hi  system memory address, high word (GPR or CONST)
 *
 * This macro generatates cycle 1 of an NFPxxxx crypto DMA operation. It is intended to be
 * used in conjuction with the crypto_setup_dma_desc_cycle2 macro.
 */
#macro crypto_setup_dma_desc_cycle1 (dest_lo, dest_hi, ctx, xlen, prefetch, addr_lo, addr_hi)
    #define_eval _bits_temp  0

    _crypto_gen_bitfield_num (0x01, NFP_CRYPTO_C1_CMD_ID_)
    _crypto_gen_bitfield_num (ctx, NFP_CRYPTO_C1_CTX_)
    _crypto_gen_bitfield_num (xlen, NFP_CRYPTO_DMA_LENGTH_)
    _crypto_gen_bitfield_num (prefetch, NFP_CRYPTO_DMA_PREFETCH_REQ_)
    _crypto_gen_bitfield_num (addr_hi, NFP_CRYPTO_DMA_MADDR_HI_)

    #if (is_numeric(ctx) & is_numeric(xlen) & is_numeric(prefetch) & is_numeric(addr_hi))
        immed32 (dest_hi, _bits_temp)                   // if everything is numeric then just need one move-immediate
    #else                                               // otherwise we will need to add fields at runtime
        .begin
        #if (strstr(dest_hi, $))                        // check if destination is a transfer reg
            .reg _bits_xregs                            // if so then we need an intermediate reg for building the value
        #else
            #define_eval _bits_xregs dest_hi            // otherwise just rename the incoming destination reg
        #endif

        immed32 (_bits_xregs, _bits_temp)
        _crypto_attach_bitfield_reg (_bits_xregs, ctx, NFP_CRYPTO_C1_CTX_)
        _crypto_attach_bitfield_reg (_bits_xregs, xlen, NFP_CRYPTO_DMA_LENGTH_)
        _crypto_attach_bitfield_reg (_bits_xregs, prefetch, NFP_CRYPTO_DMA_PREFETCH_REQ_)
        _crypto_attach_bitfield_reg (_bits_xregs, addr_hi, NFP_CRYPTO_DMA_MADDR_HI_)

        #if (strstr(dest_hi, $))                        // check if destination is a transfer reg, again
            alu [dest_hi, --, B, _bits_xregs]           // if so copy final result to the destination transfer register
        #else
            #undef _bits_xregs
        #endif
        .end
    #endif

    #if is_numeric(addr_lo)
        immed32 (dest_lo, addr_lo)                      // if addr_lo is numeric then just need one move-immediate
    #else
        alu [dest_lo, --, B, addr_lo]                   // otherwise transfer it as a register
    #endif
#endm


/**
 * Generate a crypto DMA instruction (cycle 2)
 *
 * @b Example:
 * @code
 *  .reg $io[4]
 *  .xfer_oreder $io
 *  crypto_setup_dma_desc_cycle1 ($io[0], $io[1], ...)
 *  crypto_setup_dma_desc_cycle2 ($io[2], $io[3], ...)
 *  crypto [write_fifo, $io[0], x, 0, 2]
 * @endcode
 *
 * @param dest_lo  register to receive the result, low 32-bits (this can be a transfer register)
 * @param dest_hi  register to receive the result, high 32-bits (this can be a transfer register)
 * @param ctx      crypto Dispatcher context to receive the resulting DMA instruction ( GPR or CONST)
 * @param dir      DMA transfer direction; 0=inbound, 1=outbound (GPR or CONST)
 * @param token    token field for CPP transactions driven by DMA (GPR or CONST)
 * @param action   action field for CPP transactions driven by DMA (GPR or CONST)
 * @param target   target field for CPP transactions driven by DMA (GPR or CONST)
 * @param w32      target width; 0=64-bit, 1=32-bit (GPR or CONST)
 * @param chain    enable instruction-chaining (GPR or CONST)
 * @param last     mark this operation as the 'last' DMA in the instruction chain ( GPR or CONST)
 * @param cr_addr  crypto SRAM address (upper 13 bits only; bottom 3 bits are assumed to be zero) (GPR or CONST)
 *
 * This macro generatates cycle 2 of an NFPxxxx crypto DMA operation. It is intended to be
 * used in conjuction with the crypto_setup_dma_desc_cycle2 macro.
 */
#macro crypto_setup_dma_desc_cycle2 (dest_lo, dest_hi, ctx, dir, token, action, target, w32, chain, last, cr_addr)

    // First assemble upper destination value

    #define_eval _bits_temp  0

    _crypto_gen_bitfield_num (0x01, NFP_CRYPTO_C2_CMD_ID_)
    _crypto_gen_bitfield_num (ctx, NFP_CRYPTO_C2_CTX_)
    _crypto_gen_bitfield_num (dir, NFP_CRYPTO_DMA_DIR_)
    _crypto_gen_bitfield_num (token, NFP_CRYPTO_DMA_TOKEN_)
    _crypto_gen_bitfield_num (action, NFP_CRYPTO_DMA_ACTION_)
    _crypto_gen_bitfield_num (target, NFP_CRYPTO_DMA_TARGET_)

    #if (is_numeric(ctx) & is_numeric(dir) & is_numeric(token) & is_numeric(action) & is_numeric(target))
        immed32 (dest_hi, _bits_temp)                   // if everything is numeric then just need one move-immediate

    #else                                               // otherwise we will need to add fields at runtime
        .begin
        #if (strstr(dest_hi, $))                        // check if destination is a transfer reg
            .reg _bits_xregs                            // if so then we need an intermediate reg for building the value
        #else
            #define_eval _bits_xregs dest_lo            // otherwise just rename the incoming destination reg
        #endif

        immed32 (_bits_xregs, _bits_temp)
        _crypto_attach_bitfield_reg (_bits_xregs, ctx, NFP_CRYPTO_C2_CTX_)
        _crypto_attach_bitfield_reg (_bits_xregs, dir, NFP_CRYPTO_DMA_DIR_)
        _crypto_attach_bitfield_reg (_bits_xregs, token, NFP_CRYPTO_DMA_TOKEN_)
        _crypto_attach_bitfield_reg (_bits_xregs, action, NFP_CRYPTO_DMA_ACTION_)
        _crypto_attach_bitfield_reg (_bits_xregs, target, NFP_CRYPTO_DMA_TARGET_)

        #if (strstr(dest_hi, $))                        // check if destination is a transfer reg, again
            alu [dest_hi, --, B, _bits_xregs]           // if so copy final result to the destination transfer register
        #else
            #undef _bits_xregs
        #endif
        .end
    #endif

    // now assemble lower destination value

    #define_eval _bits_temp  0

    _crypto_gen_bitfield_num (w32, NFP_CRYPTO_DMA_W32_)
    _crypto_gen_bitfield_num (chain, NFP_CRYPTO_DMA_CHAIN_)
    _crypto_gen_bitfield_num (last, NFP_CRYPTO_DMA_LAST_)
    _crypto_gen_bitfield_num (cr_addr, NFP_CRYPTO_DMA_LOCAL_ADDR_)

    #if (is_numeric(w32) & is_numeric(chain) & is_numeric(last) & is_numeric(cr_addr))
        immed32 (dest_lo, _bits_temp)                   // if everything is numeric then just need one move-immediate

    #else                                               // otherwise we will need to add fields at runtime
        .begin
        #if (strstr(dest_lo, $))                        // check if destination is a transfer reg
            .reg _bits_xregs                            // if so then we need an intermediate reg for building the value
        #else
            #define_eval _bits_xregs dest_lo            // otherwise just rename the incoming destination reg
        #endif

        immed32 (_bits_xregs, _bits_temp)
        _crypto_attach_bitfield_reg (_bits_xregs, w32, NFP_CRYPTO_DMA_W32_)
        _crypto_attach_bitfield_reg (_bits_xregs, chain, NFP_CRYPTO_DMA_CHAIN_)
        _crypto_attach_bitfield_reg (_bits_xregs, last, NFP_CRYPTO_DMA_LAST_)
        _crypto_attach_bitfield_reg (_bits_xregs, cr_addr, NFP_CRYPTO_DMA_LOCAL_ADDR_)

        #if (strstr(dest_lo, $))                        // check if destination is a transfer reg, again
            alu [dest_lo, --, B, _bits_xregs]           // if so copy final result to the destination transfer register
        #else
            #undef _bits_xregs
        #endif
        .end
    #endif
#endm


/**
 * Set up a Crypto DMA operation from Crypto SRAM to Memory (target ID 7)
 *
 * @param   cr          transfer register array to build the instructions into
 * @param   dma_ctx     crypto dispatcher context which will handle the DMA operation
 * @param   chain       constant =1 for chained operation, =0 for non-chained
 * @param   last        constant =1 if this is the last DMA in the chain, =0 otherwise (only valid if chain=1)
 * @param   sram_addr   address in Crypto SRAM where the data will be copied from (GPR or constant)
 * @param   addr32_lo   address in DRAM memory where the data will be copied to, low word
 * @param   addr32_hi   address in DRAM memory where the data will be copied to, high word
 * @param   nbytes      number of bytes to be transferred (must be a multiple of 8) (GPR or constant)
 * @param   send_sig    optional signal to set when the instruction has been written to the FIFO
 *                      use -- to generate an internal signal and wait for it before returning
 */
#macro crypto_dma_out (cr, dma_ctx, chain, last, sram_addr, addr32_lo, addr32_hi, nbytes, sent_sig)
    .begin
    #if isnum(nbytes) || isimport(nbytes)
        #define_eval len8 ((nbytes>>3)-1)
    #else
        .reg len8
        alu [len8, nbytes, -, 8]
        alu [len8, --, B, len8, >>3]
    #endif
    crypto_setup_dma_desc_cycle1 (cr[0], cr[1], dma_ctx, len8 /*byte-len*/, chain /*prefetch*/, addr32_lo, addr32_hi)
    #if isnum(sram_addr)
        #define_eval sram8 (sram_addr>>3)
    #else
        .reg sram8
        alu_shf [sram8, --, B, sram_addr, >>3]
    #endif
    crypto_setup_dma_desc_cycle2 (cr[2], cr[3], dma_ctx, 1 /*dir*/, 0 /*token*/, 1 /*action*/, 7 /*target*/, 0 /*32-bit*/, chain, last, sram8)
    #define_eval _crypto_transfer_length 2
    crypto_send (write_fifo, cr, --, 0, _crypto_transfer_length, sent_sig)
    #if isnum(nbytes)
        #undef len8
    #endif
    #if isnum(sram_addr)
        #undef sram8
    #endif
    .end
#endm


/**
 * Set up a Crypto DMA operation from Crypto SRAM to Memory (target ID 7)
 *
 *  Invokes crypto_dma_out, above, with parameters:
 *    send_sig set to "--"
 */
#macro crypto_dma_out (cr, dma_ctx, chain, last, sram_addr, addr32_lo, addr32_hi, nbytes)
    crypto_dma_out (cr, dma_ctx, chain, last, sram_addr, addr32_lo, addr32_hi, nbytes, --)
#endm


/**
 * Set up a Crypto DMA operation from Memory (target ID 7) to Crypto SRAM
 *
 * @param   cr          transfer register array to build the instructions into
 * @param   dma_ctx     crypto dispatcher context which will handle the DMA operation
 * @param   chain       constant =1 for chained operation, =0 for non-chained
 * @param   last        constant =1 if this is the last DMA in the chain, =0 otherwise (only valid if chain=1)
 * @param   sram_addr   address in Crypto SRAM where the data will be copied to (GPR or constant)
 * @param   addr32_lo   address in DRAM memory where the data will be copied from, low word
 * @param   addr32_hi   address in DRAM memory where the data will be copied from, high word
 * @param   nbytes      number of bytes to be transferred (must be a multiple of 8) (GPR or constant)
 * @param   sent_sig    optional signal to set when the instruction has been written to the FIFO
 *                      use -- to generate an internal signal and wait for it before returning
 */
#macro crypto_dma_in (cr, dma_ctx, chain, last, sram_addr, addr32_lo, addr32_hi, nbytes, sent_sig)
    .begin
    #if isnum(nbytes) || isimport(nbytes)
        #define_eval len8 ((nbytes>>3)-1)
    #else
        .reg len8
        alu [len8, nbytes, -, 8]
        alu [len8, --, B, len8, >>3]
    #endif
    crypto_setup_dma_desc_cycle1 (cr[0], cr[1], dma_ctx, len8 /*length-1*/, chain /*prefetch*/, addr32_lo, addr32_hi)
    #if isnum(sram_addr)
        #define_eval sram8 (sram_addr>>3)
    #else
        .reg sram8
        alu_shf [sram8, --, B, sram_addr, >>3]
    #endif
    crypto_setup_dma_desc_cycle2 (cr[2], cr[3], dma_ctx, 0 /*dir*/, 0 /*token*/, 0 /*action*/, 7 /*target*/, 0 /*32-bit*/, chain, last, sram8 /*local addr*/)
    #define_eval _crypto_transfer_length 2
    crypto_send (write_fifo, cr, --, 0, _crypto_transfer_length, sent_sig)
    #if isnum(nbytes)
        #undef len8
    #endif
    #if isnum(sram_addr)
        #undef sram8
    #endif
    .end
#endm


/**
 * Set up a Crypto DMA operation from Memory (target ID 7) to Crypto SRAM
 *
 *  Invokes crypto_dma_in, above, with parameters:
 *    send_sig set to "--"
 */
#macro crypto_dma_in (cr, dma_ctx, chain, last, sram_addr, addr32_lo, addr32_hi, nbytes)
    crypto_dma_in (cr, dma_ctx, chain, last, sram_addr, addr32_lo, addr32_hi, nbytes, --)
#endm


/**
 *  Transfer data from Crypto SRAM to Memory without using DMA
 *
 *  @param   _sram_addr   byte address in Crypto SRAM where the data will be copied from (GPR or constant)
 *  @param   _addr32_lo   byte address in memory where the data will be copied to, low word
 *  @param   _addr32_hi   address in memory where the data will be copied to, high word
 *  @param   _nbytes      number of bytes to transfer
 *  @param   _MODE        constant, mode of operation, one of:
 *                        CRYPTO_NDO_DEFAULT unlimited transfer length & alignment
 *                        CRYPTO_NDO_7B      max 7 byte transfer, any alignment
 *                        CRYPTO_NDO_16LW    max 16 lwords transfer, sram aligned
 *
 *                        default mode does a nonaligned transfer at beginning and end, and
 *                        8 byte aligned transfers in between, as needed
 *
 *                        max 7 byte mode does a single non-aligned transfer of 1-7 bytes
 *
 *                        max 16 lword mode does a single transfer of 1 to 16 lwords, data must
 *                        be aligned on 8 byte boundary in sram, but may be on any alignment in
 *                        bulk memory
 *
 *  Needed when memory address is not 8 byte aligned and/or transfer length is not an 8 byte multiple,
 *  because Crypto DMA does not support these transfers. Also needed to transfer crypto hash data.
 */
#macro crypto_non_dma_out(_sram_addr, _addr32_lo, _addr32_hi, _nbytes, _MODE)
    .begin
    .sig cnds

    #if ( _MODE == CRYPTO_NDO_16LW )
        .reg r_temp, r_adr_hi, r_sram_addr
        .reg $xd[16]
    #else
        .reg r_temp, r_cnt, r_xfrcnt, r_adr_hi, r_sram_addr, r_mem_addr
        .reg $xd[2]
    #endif
    .xfer_order $xd

    #if ( _MODE == CRYPTO_NDO_16LW )

        // fastest way to transfer ipsec hash code from
        // sram to memory

        #pragma warning(push)
        #pragma warning(disable:4700)
        #pragma warning(disable:4701)
        #pragma warning(disable:5008)

        // read from sram
        alu[r_sram_addr, _sram_addr, AND~, 7]
        alu[r_temp, _nbytes, +, 4] // round up if odd # lwords
        alu[r_temp, --, B, r_temp, >>3] // # 8 byte words
        alu[r_temp, r_temp, -, 1] // 0 based for indirect
        alu[--, 0x80, OR, r_temp, <<8]
        crypto[read, $xd[0], 0, r_sram_addr, MAX_8], \
                                    indirect_ref, ctx_swap[cnds]

        // rd xfr regs -> wrt xfr regs
        // hash sizes in use are 16,20,32,48,64 bytes
        alu[$xd[0], --, B, $xd[0]]
        alu[$xd[1], --, B, $xd[1]]
        alu[$xd[2], --, B, $xd[2]]
        alu[$xd[3], --, B, $xd[3]]
        .if ( _nbytes > 16 )
            alu[$xd[4], --, B, $xd[4]]
            .if ( _nbytes > 20 )
                alu[$xd[5], --, B, $xd[5]]
                alu[$xd[6], --, B, $xd[6]]
                alu[$xd[7], --, B, $xd[7]]
                .if ( _nbytes > 32 )
                    alu[$xd[8], --, B, $xd[8]]
                    alu[$xd[9], --, B, $xd[9]]
                    alu[$xd[10], --, B, $xd[10]]
                    alu[$xd[11], --, B, $xd[11]]
                    .if ( _nbytes > 48 )
                        alu[$xd[12], --, B, $xd[12]]
                        alu[$xd[13], --, B, $xd[13]]
                        alu[$xd[14], --, B, $xd[14]]
                        alu[$xd[15], --, B, $xd[15]]
                    .endif
                .endif
            .endif
        .endif

        // write to memory
        alu[r_temp, --, B, _nbytes, >>2] // # lwords
        alu[r_temp, r_temp, -, 1] // 0 based for indirect
        alu[r_adr_hi, --, B, _addr32_hi, <<24]
        alu[--, 0x80, OR, r_temp, <<8]
        mem[write32, $xd[0], r_adr_hi, <<8, _addr32_lo, MAX_16], \
                                    indirect_ref, ctx_swap[cnds]
        #pragma warning(pop)

    #else // _MODE == CRYPTO_NDO_DEFAULT or CRYPTO_NDO_7B

        // setup internal regs
        move(r_cnt, _nbytes)
        move(r_sram_addr, _sram_addr)
        move(r_mem_addr, _addr32_lo)
        alu[r_adr_hi, --, B, _addr32_hi, <<24]


        // read 8 byte sram block containing start of transfer
        alu[r_temp, r_sram_addr, AND~, 7]
        crypto[read, $xd[0], 0, r_temp, 1], ctx_swap[cnds]


        // re-align data in transfer regs for write to mem

        alu[r_temp, r_sram_addr, AND, 7] // starting byte addr in sram
        jump[r_temp, cndojt#], targets [j0#, j1#, j2#, j3#, j4#, j5#, j6#, j7#]

        cndojt#:
            j0#: br[cndojt0#]
            j1#: br[cndojt1#]
            j2#: br[cndojt2#]
            j3#: br[cndojt3#]
            j4#: br[cndojt4#]
            j5#: br[cndojt5#]
            j6#: br[cndojt6#]
            j7#: br[cndojt7#]

        cndojt0#:
            alu[$xd[0], --, B, $xd[0]]
            alu[$xd[1], --, B, $xd[1]]
            br[cndo_blk1_xfr#]

        cndojt1#:
            alu[r_temp, --, B, $xd[0], <<8]
            ld_field[r_temp, 0001, $xd[1], >>24]
            alu[$xd[0], --, B, r_temp]
                alu[r_temp, --, B, $xd[1], <<8]
            alu[$xd[1], --, B, r_temp]
            br[cndo_blk1_xfr#]

        cndojt2#:
            alu[r_temp, --, B, $xd[0], <<16]
            ld_field[r_temp, 0011, $xd[1], >>16]
            alu[$xd[0], --, B, r_temp]
            alu[$xd[1], --, B, $xd[1], <<16]
            br[cndo_blk1_xfr#]

        cndojt3#:
            alu[r_temp, --, B, $xd[0], <<24]
            ld_field[r_temp, 0111, $xd[1], >>8]
            alu[$xd[0], --, B, r_temp]
            alu[$xd[1], --, B, $xd[1], <<24]
            br[cndo_blk1_xfr#]

        cndojt4#:
            alu[$xd[0], --, B, $xd[1]]
            br[cndo_blk1_xfr#]

        cndojt5#:
                alu[$xd[0], --, B, $xd[1], <<8]
            br[cndo_blk1_xfr#]

        cndojt6#:
            alu[$xd[0], --, B, $xd[1], <<16]
            br[cndo_blk1_xfr#]

        cndojt7#:
            alu[$xd[0], --, B, $xd[1], <<24]
            br[cndo_blk1_xfr#]

        cndo_blk1_xfr#:

        // figure bytes available from 1st 8 byte block
        alu[r_xfrcnt, r_sram_addr, AND, 7]
        alu[ r_xfrcnt, 8, -, r_xfrcnt]
        limit_min(r_xfrcnt, r_xfrcnt, r_cnt)

        // write bytes in 1st sram block to mem. bulk engine in mem aligns
        // to starting mem addr
        #pragma warning(push)
        #pragma warning(disable:4701)
        alu[r_temp, r_xfrcnt, -, 1] // adjust for mem cmd
        alu[--, 0x80, OR, r_temp, <<8]
        mem[write8, $xd[0], r_adr_hi, <<8, r_mem_addr, MAX_8], indirect_ref, ctx_swap[cnds]
        #pragma warning(pop)

    #endif // _MODE == CRYPTO_NDO_DEFAULT or CRYPTO_NDO_7B

    #if ( _MODE == CRYPTO_NDO_DEFAULT )

        // adjust remaining byte count
        alu[r_cnt, r_cnt, -, r_xfrcnt]

        // check if done and branch out
        beq[crypto_non_dma_out_done#]

        alu[r_sram_addr, r_sram_addr, +, r_xfrcnt]
        alu[r_mem_addr, r_mem_addr, +, r_xfrcnt]


        crypto_non_dma_out_loop#:

        // check for less than 8 bytes to go
        .if ( r_cnt < 8 )
            br[crypto_non_dma_out_last#]
        .endif


        // otherwise, transfer an 8 byte block, now sram_addr should be 8 byte aligned

        crypto[read, $xd[0], 0, r_sram_addr, 1], ctx_swap[cnds]
        move($xd[0], $xd[0])
        move($xd[1], $xd[1])
        mem[write, $xd[0], r_adr_hi, <<8, r_mem_addr, 1], ctx_swap[cnds]

        alu[r_cnt, r_cnt, -, 8]
        alu[r_sram_addr, r_sram_addr, +, 8]
        alu[r_mem_addr, r_mem_addr, +, 8]

        br[crypto_non_dma_out_loop#]


        crypto_non_dma_out_last#:

        // check for done
        .if ( r_cnt <= 0 )
            br[crypto_non_dma_out_done#]
        .endif

        // read in last 8 byte block from sram
        crypto[read, $xd[0], 0, r_sram_addr, 1], ctx_swap[s]
        move($xd[0], $xd[0])
        move($xd[1], $xd[1])


        // write bytes in last sram block to mem. bulk engine in mem aligns
        // to starting mem addr
        alu[r_cnt, r_cnt, -, 1] // adjust for mem cmd
        alu[--, 0x80, OR, r_cnt, <<8]
        mem[write8, $xd[0], r_adr_hi, <<8, r_mem_addr, MAX_8], indirect_ref, ctx_swap[cnds]

    #endif // _MODE == CRYPTO_NDO_DEFAULT

    crypto_non_dma_out_done#:

    .end
#endm


/** @}
 * @}
 */

#endif /* __CRYPTO_LIB_UC__ */
