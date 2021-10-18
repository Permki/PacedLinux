/*
 * Copyright (C) 2014 Netronome Systems, Inc. All rights reserved.
 */

#ifndef __CRYPTO_LIB_IPSEC_UC__
#define __CRYPTO_LIB_IPSEC_UC__

#ifndef NFP_LIB_ANY_NFAS_VERSION
    #if (!defined(__NFP_TOOL_NFAS) ||                       \
        (__NFP_TOOL_NFAS < NFP_SW_VERSION(5, 0, 0, 0)) ||   \
        (__NFP_TOOL_NFAS > NFP_SW_VERSION(6, 1, 0, 255)))
        #error "This standard library is not supported for the version of \
                  the SDK currently in use."
    #endif
#endif

/** @file crypto_lib_ipsec.uc CRYPTO IPSec Operation Macros
 * @addtogroup cryptoipsec CRYPTO IPSec Operation
 * @{
 *
 * @name CRYPTO IPSec Operation Macros
 * @{
 *
 * This file contains a set of crypto-library sequences.  The sequences are
 * designed to be compatible with Netronome's crypto_support facility. They are
 * implemented as 'compressed' sequences, which can be preloaded to the CIB
 * memory space of the bulk core units at initialization time, and then invoked
 * at run-time on a per-packet basis quickly and efficiently.
 *
 * These sequences implement encryption, decryption, and authentication that is
 * intended to be usable on IPSec-formatted packets encapsulated with ESP or AH.
 * The intention is for the ME to issue a single get_core() at initialization
 * time for a specific Crypto Dispatcher context, then keep that core 'pinned' to
 * that context for the life of the program. The selection of which bulk core
 * unit works on a particular packet is thus pushed to earlier in the process,
 * when the packet is assigned to an ME. This technique has the advantage that it
 * avoids the processing delay associated with executing the get_core() and
 * free_core() instructions for every packet. It also eliminates some of the
 * variables that need to be passed to the CIB code sequences, because each core
 * can use a fixed set of pre-allocated buffers that are statically assigned to
 * the ME's.
 *
 * The source file for these sequences is crypto_lib_ipsec.crypt  The source file
 * is processed by the Netronome utility ca2.py to generate the file
 * crypto_lib_ipsec.uc, which is included in the microcode. The macros defined in
 * crypto_lib_ipsec.uc are used by the macros in crypto_lib.uc
 */
/*
// API Quick Reference:
//
// crypto_load_ipsec_enc (cr_xfr, cr_ctx, packet, in_len, auth_only_data, iv, auth_length, hmac_keylen, crypt_select, crypt_modes)
// crypto_gen_ipsec_enc (core, desc)
// crypto_load_ipsec_dec (cr_xfr, cr_ctx, packet, in_len, auth_only_data, iv, auth_length, hmac_keylen, crypt_select, crypt_modes)
// crypto_gen_ipsec_dec (core, desc)
// crypto_load_ipsec_enc_strt (cr_xfr, cr_ctx, packet, in_len, auth_only_data, iv, auth_length, hmac_keylen, crypt_select, crypt_modes)
// crypto_gen_ipsec_enc_strt (core, desc)
// crypto_load_ipsec_enc_cont (cr_xfr, cr_ctx, packet, in_len, dummy1, dummy2, dummy3)
// crypto_gen_ipsec_enc_cont (core, desc)
// crypto_load_ipsec_enc_end (cr_xfr, cr_ctx, packet, in_len, hmac_resadr, hmac_keyadr, hmac_keylen)
// crypto_gen_ipsec_enc_end (core, desc)
// crypto_load_ipsec_dec_strt (cr_xfr, cr_ctx, packet, in_len, auth_only_data, iv, auth_length, hmac_keylen, crypt_select, crypt_modes)
// crypto_gen_ipsec_dec_strt (core, desc)
// crypto_load_ipsec_dec_strt_nw (cr_xfr, cr_ctx, packet, in_len, auth_only_data, iv, auth_length, hmac_keylen, crypt_select, crypt_modes)
// crypto_gen_ipsec_dec_strt_nw (core, desc)
// crypto_load_ipsec_dec_cont (cr_xfr, cr_ctx, packet, in_len, dummy1, dummy2, dummy3)
// crypto_gen_ipsec_dec_cont (core, desc)
// crypto_load_ipsec_dec_cont_nw (cr_xfr, cr_ctx, packet, in_len, dummy1, dummy2, dummy3)
// crypto_gen_ipsec_dec_cont_nw (core, desc)
// crypto_load_ipsec_dec_end (cr_xfr, cr_ctx, packet, in_len, hmac_resadr, hmac_keyadr, hmac_keylen)
// crypto_gen_ipsec_dec_end (core, desc)
// crypto_gen_compr_constants (base0, base1, base2, base3, base4)
*/
/**
 * ipsec_enc
 *
 * Sequence to do IPSec-compatible encryption of a packet, using bulk cores that
 * stay 'pinned'to a Dispatcher context.  This allows operation without per-packet
 * get_core()/free_core() ops. One context will do a get_core() at init time and will
 * hold onto that core for the life of the program.
 *
 * In most cases, the 'output' (ciphertext) data is at the same address as the 'input'
 * (plaintext) data. Only in the case of the 'aes-gcm null' ciphers the output is
 * sent to a different buffer than the input.
 *
 * Supports AES, DES, and NO encryption with MD5 or SHA-x HMAC auth.
 *
 * Calling the sequence:
 *
 * crypto_load_ipsec_enc (packet_in, packet_out, in_len, seq_ua, auth_only_data,
 *                        iv, auth_length, hmac_keylen, crypt_select, crypt_modes,
 *                        sa_cik, sa_hmk, sa_hmr)
 *
 * @param packet_in             crypto SRAM address of the start of plaintext
 * @param packet_out            crypto SRAM address of the start of ciphertext
 * @param in_len                length of data to be encrypted
 * @param seq_ua                crypto SRAM addr of seq number 63:32 (unused)
 * @param auth_only_data        crypto SRAM addr of auth-only data SPI/Seq
 * @param iv                    crypto SRAM address of the Initialization Vector
 * @param auth_length           byte length of (SPI/Seq
 * @param hmac_keylen           byte length of HMAC key, minus 1 (MD5-15, SHA1-19)
 * @param crypt_select          config word 1, produced by crypto_setup_configs
 * @param crypt_modes           config word 2, produced by crypto_setup_configs
 * @param sa_cik                crypto SRAM address of start of cipher key
 * @param sa_hmk                crypto SRAM address of start of hash key
 * @param sa_hmr                crypto SRAM address of hash result
 **/

/**
 *  Prepare transfer regs to load static (aka 'pinned') encryption sequence
 *
 */
#macro crypto_load_ipsec_enc (cr_xfr, cr_ctx, packet_in, packet_out, in_len, seq_ua, auth_only_data, iv, auth_length, hmac_keylen, crypt_select, crypt_modes, sa_cik, sa_hmk, sa_hmr)
    crypto_build_xfer_reg (cr_xfr[0], packet_in, packet_out)
    crypto_build_xfer_reg (cr_xfr[1], in_len, seq_ua)
    crypto_build_xfer_reg (cr_xfr[2], auth_only_data, iv)
    crypto_build_xfer_reg (cr_xfr[3], auth_length, hmac_keylen)
    crypto_build_xfer_reg (cr_xfr[4], crypt_select, crypt_modes)
    crypto_build_xfer_reg (cr_xfr[5], sa_cik, sa_hmk)
    crypto_build_xfer_reg (cr_xfr[6], sa_hmr, 0)
    immed [cr_xfr[7], 0]
    #define_eval _crypto_transfer_length 4
#endm


/**
 * Generate cmd sequence as constant data
 *
 *  @param core  crypto bulk core, 0 - 3 for NFP3XXX or 0 - 5 for NFP6XXX
 *  @param desc  sequence 'descriptor', contains sram location address
 *
 *  used in _crypto_library_load_dynamic in crypto_lib.uc
 */
#macro crypto_gen_ipsec_enc (core, desc)
    #if core==5
        #define_eval ipsec_enc_desc_5 (desc)
    #elif core==4
        #define_eval ipsec_enc_desc_4 (desc)
    #elif core==3
        #define_eval ipsec_enc_desc_3 (desc)
    #elif core==2
        #define_eval ipsec_enc_desc_2 (desc)
    #elif core==1
        #define_eval ipsec_enc_desc_1 (desc)
    #elif core==0
        #define_eval ipsec_enc_desc_0 (desc)
    #endif
    #define_eval gen_data_0 0
    #define_eval gen_data_1 0
    #define_eval gen_data_2 0
    #define_eval gen_data_3 0
    #define_eval gen_data_4 0
    #define_eval gen_data_5 0
    #define_eval gen_data_6 0
    #define_eval gen_data_7 0
    #define_eval gen_data_8 0x08701819
    #define_eval gen_data_9 0x0a13221a
    #define_eval gen_data_10 0x08382809
    #define_eval gen_data_11 0x01900f1b
    #define_eval gen_data_12 0x01000e0c
    #define_eval gen_data_13 0x0a230e0d
    #define_eval gen_data_14 0x03730a08
    #define_eval gen_data_15 0x08281c23
    #define_eval gen_data_16 0x01d00f1b
    #define_eval gen_data_17 0x11720f28
    #define_eval gen_data_18 0
    #define_eval gen_data_19 0
    #define_eval _cib_xfer_size_ipsec_enc 20
    #define_eval _cib_code_size_ipsec_enc 10
    #define_eval _cib_vars_base_ipsec_enc 0
    #define_eval _cib_vars_size_ipsec_enc 8
    #define_eval _cib_vars_xfer_ipsec_enc 8
#endm

/**
 * ipsec_enc_esn
 *
 * Similiar to ipsec_enc, but works for a 64 bit sequence number (ESN)
 *
 * Calling the sequence:
 *
 * crypto_load_ipsec_enc_esn (packet_in, packet_out, in_len, seq_ua,
 *                            auth_only_data, iv, auth_length, hmac_keylen,
 *                            crypt_select, crypt_modes,
 *                            sa_cik, sa_hmk, sa_hmr)
 *
 * @param packet_in             crypto SRAM address of the start of plaintext
 * @param packet_out            crypto SRAM address of the start of ciphertext
 * @param in_len                length of data to be encrypted
 * @param seq_ua                crypto SRAM addr of seq number 63:32
 * @param auth_only_data        crypto SRAM addr of auth-only data SPI/Seq
 * @param iv                    crypto SRAM addr of the Initialization Vector
 * @param auth_length           byte length of (SPI/Seq
 * @param hmac_keylen           byte length of HMAC key, minus 1 (MD5-15, SHA1-19)
 * @param crypt_select          config word 1, produced by crypto_setup_configs
 * @param crypt_modes           config word 2, produced by crypto_setup_configs
 * @param sa_cik                crypto SRAM address of start of cipher key
 * @param sa_hmk                crypto SRAM address of start of hash key
 * @param sa_hmr                crypto SRAM address of hash result
 **/

/**
 *  Prepare transfer regs to load static (aka 'pinned') encryption sequence
 *
 */
#macro crypto_load_ipsec_enc_esn (cr_xfr, cr_ctx, packet_in, packet_out, in_len, seq_ua, auth_only_data, iv, auth_length, hmac_keylen, crypt_select, crypt_modes, sa_cik, sa_hmk, sa_hmr)
    crypto_build_xfer_reg (cr_xfr[0], packet_in, packet_out)
    crypto_build_xfer_reg (cr_xfr[1], in_len, seq_ua)
    crypto_build_xfer_reg (cr_xfr[2], auth_only_data, iv)
    crypto_build_xfer_reg (cr_xfr[3], auth_length, hmac_keylen)
    crypto_build_xfer_reg (cr_xfr[4], crypt_select, crypt_modes)
    crypto_build_xfer_reg (cr_xfr[5], sa_cik, sa_hmk)
    crypto_build_xfer_reg (cr_xfr[6], sa_hmr, 0)
    immed [cr_xfr[7], 0]
    #define_eval _crypto_transfer_length 4
#endm


/**
 * Generate cmd sequence as constant data
 *
 *  @param core  crypto bulk core, 0 - 3 for NFP3XXX or 0 - 5 for NFP6XXX
 *  @param desc  sequence 'descriptor', contains sram location address
 *
 *  used in _crypto_library_load_dynamic in crypto_lib.uc
 */
#macro crypto_gen_ipsec_enc_esn (core, desc)
    #if core==5
        #define_eval ipsec_enc_esn_desc_5 (desc)
    #elif core==4
        #define_eval ipsec_enc_esn_desc_4 (desc)
    #elif core==3
        #define_eval ipsec_enc_esn_desc_3 (desc)
    #elif core==2
        #define_eval ipsec_enc_esn_desc_2 (desc)
    #elif core==1
        #define_eval ipsec_enc_esn_desc_1 (desc)
    #elif core==0
        #define_eval ipsec_enc_esn_desc_0 (desc)
    #endif
    #define_eval gen_data_0 0
    #define_eval gen_data_1 0
    #define_eval gen_data_2 0
    #define_eval gen_data_3 0
    #define_eval gen_data_4 0
    #define_eval gen_data_5 0
    #define_eval gen_data_6 0
    #define_eval gen_data_7 0
    #define_eval gen_data_8 0x08701819
    #define_eval gen_data_9 0x0a13221a
    #define_eval gen_data_10 0x08382809
    #define_eval gen_data_11 0x01900f1b
    #define_eval gen_data_12 0x01000e0c
    #define_eval gen_data_13 0x0a230e0d
    #define_eval gen_data_14 0x03030a08
    #define_eval gen_data_15 0x0172240b
    #define_eval gen_data_16 0x08281c23
    #define_eval gen_data_17 0x01d00f1b
    #define_eval gen_data_18 0x11720f28
    #define_eval gen_data_19 0
    #define_eval _cib_xfer_size_ipsec_enc_esn 20
    #define_eval _cib_code_size_ipsec_enc_esn 11
    #define_eval _cib_vars_base_ipsec_enc_esn 0
    #define_eval _cib_vars_size_ipsec_enc_esn 8
    #define_eval _cib_vars_xfer_ipsec_enc_esn 8
#endm

/**
 * ipsec_enc_strt
 *
 * Similar to ipsec_enc but used to begin an encryption sequence
 * that will span multiple buffers, needed to handle jumbo packets.
 *
 * Calling the sequence:
 *
 * load_ipsec_enc_strt (packet_in, packet_out, in_len, seq_ua auth_only_data,
 *                      iv, auth_length, hmac_keylen, crypt_select, crypt_modes,
 *                      sa_cik, sa_hmk, sa_hmr)
 *
 * @param packet_in           crypto SRAM address of the start of plaintext
 * @param packet_out          crypto SRAM address of the start of ciphertext
 * @param in_len              length of data to be encrypted
 * @param seq_ua              crypto SRAM addr of seq number 63:32 (unused)
 * @param auth_only_data      crypto SRAM addr of auth-only data SPI/Seq
 * @param iv                  crypto SRAM address of the Initialization Vector
 * @param auth_length         byte length of (SPI/Seq
 * @param hmac_keylen         byte length of HMAC key, minus 1
 * @param crypt_select        config word 1, produced by crypto_setup_configs
 * @param crypt_modes         config word 2, produced by crypto_setup_configs
 * @param sa_cik              crypto SRAM address of start of cipher key
 * @param sa_hmk              crypto SRAM address of start of hash key
 * @param sa_hmr              crypto SRAM address of hash result
 **/

/**
 *  Prepare transfer regs to load static (aka 'pinned') encryption sequence
 *
 */
#macro crypto_load_ipsec_enc_strt (cr_xfr, cr_ctx, packet_in, packet_out, in_len, seq_ua, auth_only_data, iv, auth_length, hmac_keylen, crypt_select, crypt_modes, sa_cik, sa_hmk, sa_hmr)
    crypto_build_xfer_reg (cr_xfr[0], packet_in, packet_out)
    crypto_build_xfer_reg (cr_xfr[1], in_len, seq_ua)
    crypto_build_xfer_reg (cr_xfr[2], auth_only_data, iv)
    crypto_build_xfer_reg (cr_xfr[3], auth_length, hmac_keylen)
    crypto_build_xfer_reg (cr_xfr[4], crypt_select, crypt_modes)
    crypto_build_xfer_reg (cr_xfr[5], sa_cik, sa_hmk)
    crypto_build_xfer_reg (cr_xfr[6], sa_hmr, 0)
    immed [cr_xfr[7], 0]
    #define_eval _crypto_transfer_length 4
#endm


/**
 * Generate cmd sequence as constant data
 *
 *  @param core  crypto bulk core, 0 - 3 for NFP3XXX or 0 - 5 for NFP6XXX
 *  @param desc  sequence 'descriptor', contains sram location address
 *
 *  used in _crypto_library_load_dynamic in crypto_lib.uc
 */
#macro crypto_gen_ipsec_enc_strt (core, desc)
    #if core==5
        #define_eval ipsec_enc_strt_desc_5 (desc)
    #elif core==4
        #define_eval ipsec_enc_strt_desc_4 (desc)
    #elif core==3
        #define_eval ipsec_enc_strt_desc_3 (desc)
    #elif core==2
        #define_eval ipsec_enc_strt_desc_2 (desc)
    #elif core==1
        #define_eval ipsec_enc_strt_desc_1 (desc)
    #elif core==0
        #define_eval ipsec_enc_strt_desc_0 (desc)
    #endif
    #define_eval gen_data_0 0
    #define_eval gen_data_1 0
    #define_eval gen_data_2 0
    #define_eval gen_data_3 0
    #define_eval gen_data_4 0
    #define_eval gen_data_5 0
    #define_eval gen_data_6 0
    #define_eval gen_data_7 0
    #define_eval gen_data_8 0x08701819
    #define_eval gen_data_9 0x0a13221a
    #define_eval gen_data_10 0x08382809
    #define_eval gen_data_11 0x01900f1b
    #define_eval gen_data_12 0x01000e0c
    #define_eval gen_data_13 0x0a230e0d
    #define_eval gen_data_14 0x13030a08
    #define_eval gen_data_15 0
    #define_eval _cib_xfer_size_ipsec_enc_strt 16
    #define_eval _cib_code_size_ipsec_enc_strt 7
    #define_eval _cib_vars_base_ipsec_enc_strt 0
    #define_eval _cib_vars_size_ipsec_enc_strt 8
    #define_eval _cib_vars_xfer_ipsec_enc_strt 8
#endm

/**
 * ipsec_enc_cont
 *
 * Used after ipsec_enc_strt to continue encrypting a packet on a buffer of data
 * following the first part of the packet. Needed to handle jumbo packets.
 * Setup and ending condition from prior use of ipsec_enc_strt is
 * required prior to invoking this sequence. In particular, the keys, config
 * registers, hash address, etc. must remain intact when this sequence is
 * started.
 *
 * Calling the sequence:
 *
 * load_ipsec_enc_cont (packet_in, packet_out, in_len)
 *
 * @param packet_in       crypto SRAM address of the start of plaintext
 * @param packet_out      crypto SRAM address of the start of ciphertext
 * @param in_len          length of data to encrypt
 **/

/**
 *  Prepare transfer regs to load static (aka 'pinned') encryption sequence
 *
 */
#macro crypto_load_ipsec_enc_cont (cr_xfr, cr_ctx, packet_in, packet_out, in_len)
    crypto_build_xfer_reg (cr_xfr[0], packet_in, packet_out)
    crypto_build_xfer_reg (cr_xfr[1], in_len, 0)
    #define_eval _crypto_transfer_length 1
#endm


/**
 * Generate cmd sequence as constant data
 *
 *  @param core  crypto bulk core, 0 - 3 for NFP3XXX or 0 - 5 for NFP6XXX
 *  @param desc  sequence 'descriptor', contains sram location address
 *
 *  used in _crypto_library_load_dynamic in crypto_lib.uc
 */
#macro crypto_gen_ipsec_enc_cont (core, desc)
    #if core==5
        #define_eval ipsec_enc_cont_desc_5 (desc)
    #elif core==4
        #define_eval ipsec_enc_cont_desc_4 (desc)
    #elif core==3
        #define_eval ipsec_enc_cont_desc_3 (desc)
    #elif core==2
        #define_eval ipsec_enc_cont_desc_2 (desc)
    #elif core==1
        #define_eval ipsec_enc_cont_desc_1 (desc)
    #elif core==0
        #define_eval ipsec_enc_cont_desc_0 (desc)
    #endif
    #define_eval gen_data_0 0
    #define_eval gen_data_1 0
    #define_eval gen_data_2 0x08182309
    #define_eval gen_data_3 0x13030a08
    #define_eval _cib_xfer_size_ipsec_enc_cont 4
    #define_eval _cib_code_size_ipsec_enc_cont 2
    #define_eval _cib_vars_base_ipsec_enc_cont 0
    #define_eval _cib_vars_size_ipsec_enc_cont 2
    #define_eval _cib_vars_xfer_ipsec_enc_cont 2
#endm

/**
 * ipsec_enc_end
 *
 * Used after ipsec_enc_strt and possibly ipsec_enc_cont, to complete
 * encrypting a packet on the last buffer of data of the packet.
 * Needed to handle jumbo packets. Setup and ending condition from prior use of
 * ipsec_enc_strt is required prior to invoking this sequence. In
 * particular, the keys, config registers, etc. must remain intact when this
 * sequence is started.
 *
 * The hash key address and result address for the packet must be specified as
 * parameters to this sequence.
 *
 * The key address should be equal to the sram location for the hash key
 * corresponding to the buffer ( A,B,C or D) used for the 1st part of the
 * packet. For e.g., if the 1st part of the packet was loaded using buffer A,
 * the hash key address would be the same as provided in the variable sa_hmk
 * provided with ipsec_enc_strt since that is where the key was loaded.
 *
 * The hash key result address should be the sram address corresponding to the
 * hash result for the buffer being used when this macro ( ipsec_enc_end ) is
 * invoked. For example, if using buffer B, the hash result address would be
 * equal to location of the hash for buffer B.
 *
 * Calling the sequence:
 *
 * crypto_load_ipsec_enc_end (packet_in, packet_out, in_len, seq_ua,
 *                            hmac_resadr, hmac_keyadr, hmac_keylen)
 *
 * @param packet_in      crypto SRAM address of the start of plaintext
 * @param packet_out     crypto SRAM address of the start of ciphertext
 * @param in_len         length of data to be encrypted
 * @param seq_ua         crypto SRAM addr of seq number 63:32 (unused)
 * @param hmac_keyadr    crypto SRAM address of the start of the HMAC key
 * @param hmac_keylen    byte length of HMAC key, minus 1
 * @param hmac_resadr    crypto SRAM address of the HMAC key calculation result
 **/

/**
 *  Prepare transfer regs to load static (aka 'pinned') encryption sequence
 *
 */
#macro crypto_load_ipsec_enc_end (cr_xfr, cr_ctx, packet_in, packet_out, in_len, seq_ua, hmac_keyadr, hmac_keylen, hmac_resadr)
    crypto_build_xfer_reg (cr_xfr[0], packet_in, packet_out)
    crypto_build_xfer_reg (cr_xfr[1], in_len, seq_ua)
    crypto_build_xfer_reg (cr_xfr[2], hmac_keyadr, hmac_keylen)
    crypto_build_xfer_reg (cr_xfr[3], hmac_resadr, 0)
    #define_eval _crypto_transfer_length 2
#endm


/**
 * Generate cmd sequence as constant data
 *
 *  @param core  crypto bulk core, 0 - 3 for NFP3XXX or 0 - 5 for NFP6XXX
 *  @param desc  sequence 'descriptor', contains sram location address
 *
 *  used in _crypto_library_load_dynamic in crypto_lib.uc
 */
#macro crypto_gen_ipsec_enc_end (core, desc)
    #if core==5
        #define_eval ipsec_enc_end_desc_5 (desc)
    #elif core==4
        #define_eval ipsec_enc_end_desc_4 (desc)
    #elif core==3
        #define_eval ipsec_enc_end_desc_3 (desc)
    #elif core==2
        #define_eval ipsec_enc_end_desc_2 (desc)
    #elif core==1
        #define_eval ipsec_enc_end_desc_1 (desc)
    #elif core==0
        #define_eval ipsec_enc_end_desc_0 (desc)
    #endif
    #define_eval gen_data_0 0
    #define_eval gen_data_1 0
    #define_eval gen_data_2 0
    #define_eval gen_data_3 0
    #define_eval gen_data_4 0x08182309
    #define_eval gen_data_5 0x03730a08
    #define_eval gen_data_6 0x08280e23
    #define_eval gen_data_7 0x01d00d0c
    #define_eval gen_data_8 0x11720d28
    #define_eval gen_data_9 0
    #define_eval gen_data_10 0
    #define_eval gen_data_11 0
    #define_eval _cib_xfer_size_ipsec_enc_end 12
    #define_eval _cib_code_size_ipsec_enc_end 5
    #define_eval _cib_vars_base_ipsec_enc_end 0
    #define_eval _cib_vars_size_ipsec_enc_end 4
    #define_eval _cib_vars_xfer_ipsec_enc_end 4
#endm

/**
 * ipsec_enc_end_esn
 *
 * Similiar to ipsec_enc_end, but works for a 64 bit sequence number (ESN)
 *
 * Calling the sequence:
 *
 * crypto_load_ipsec_enc_end_esn (packet_in, packet_out, in_len, seq_ua,
 *                                hmac_keyadr, hmac_keylen, hmac_resadr)
 *
 * @param packet_in      crypto SRAM address of the start of plaintext
 * @param packet_out     crypto SRAM address of the start of ciphertext
 * @param in_len         length of data to be encrypted
 * @param seq_ua         crypto SRAM addr of seq number 63:32
 * @param hmac_keyadr    crypto SRAM address of the start of the HMAC key
 * @param hmac_keylen    byte length of HMAC key, minus 1
 * @param hmac_resadr    crypto SRAM address of the HMAC key calculation result
 **/

/**
 *  Prepare transfer regs to load static (aka 'pinned') encryption sequence
 *
 */
#macro crypto_load_ipsec_enc_end_esn (cr_xfr, cr_ctx, packet_in, packet_out, in_len, seq_ua, hmac_keyadr, hmac_keylen, hmac_resadr)
    crypto_build_xfer_reg (cr_xfr[0], packet_in, packet_out)
    crypto_build_xfer_reg (cr_xfr[1], in_len, seq_ua)
    crypto_build_xfer_reg (cr_xfr[2], hmac_keyadr, hmac_keylen)
    crypto_build_xfer_reg (cr_xfr[3], hmac_resadr, 0)
    #define_eval _crypto_transfer_length 2
#endm


/**
 * Generate cmd sequence as constant data
 *
 *  @param core  crypto bulk core, 0 - 3 for NFP3XXX or 0 - 5 for NFP6XXX
 *  @param desc  sequence 'descriptor', contains sram location address
 *
 *  used in _crypto_library_load_dynamic in crypto_lib.uc
 */
#macro crypto_gen_ipsec_enc_end_esn (core, desc)
    #if core==5
        #define_eval ipsec_enc_end_esn_desc_5 (desc)
    #elif core==4
        #define_eval ipsec_enc_end_esn_desc_4 (desc)
    #elif core==3
        #define_eval ipsec_enc_end_esn_desc_3 (desc)
    #elif core==2
        #define_eval ipsec_enc_end_esn_desc_2 (desc)
    #elif core==1
        #define_eval ipsec_enc_end_esn_desc_1 (desc)
    #elif core==0
        #define_eval ipsec_enc_end_esn_desc_0 (desc)
    #endif
    #define_eval gen_data_0 0
    #define_eval gen_data_1 0
    #define_eval gen_data_2 0
    #define_eval gen_data_3 0
    #define_eval gen_data_4 0x08182309
    #define_eval gen_data_5 0x03030a08
    #define_eval gen_data_6 0x0172240b
    #define_eval gen_data_7 0x08280e23
    #define_eval gen_data_8 0x01d00d0c
    #define_eval gen_data_9 0x11720d28
    #define_eval gen_data_10 0
    #define_eval gen_data_11 0
    #define_eval _cib_xfer_size_ipsec_enc_end_esn 12
    #define_eval _cib_code_size_ipsec_enc_end_esn 6
    #define_eval _cib_vars_base_ipsec_enc_end_esn 0
    #define_eval _cib_vars_size_ipsec_enc_end_esn 4
    #define_eval _cib_vars_xfer_ipsec_enc_end_esn 4
#endm

/**
 * ipsec_enc_aesgcm
 *
 * Similar to ipsec_enc, but used for gcm (galois counter mode) for
 * aes-gcm-esp, esn or non-esn and either regular or 'null' (rfc4543) encrypt.
 * In the normal case, the ciphertext is written to the same location as the
 * plaintext. In the 'null' case, the ciphertext is written to another buffer
 * instead; this buffer is used as a temp area and is not transmitted.
 *
 * N.B. this 'encrypt' sequence should actually not be used for 'null' (rfc4543)
 *      because we have to do crypt_hash parallel ( not serial ) because the
 *      hash is to be generated using the original plaintext. Use the 'decrypt'
 *      sequence instead for encrypt; it is the same except for serial/parallel
 *
 *
 * This sequence is used for encrypt. Decrypt is the same except for the use of
 * crypt serial for encrypt and crypt parallel for decrypt
 *
 * Calling the sequence:
 *
 * load_ipsec_enc_aesgcm (packet_in, packet_out, in_len, length_vector,
 *                        auth_only_data, iv_constr, auth_length, zero,
 *                        crypt_select, crypt_modes,
 *                        sa_cik, sa_hmk, sa_hmr)
 *
 * @param packet_in             crypto SRAM address of the start of plaintext
 * @param packet_out            crypto SRAM address of the start of ciphertext
 * @param in_len                length of data to be encrypted
 * @param length_vector         crypto SRAM address of the len(A)||len(C) vector
 * @param auth_only_data        crypto SRAM addr of SPI + SeqLo + SeqHi(esn)
 * @param iv_constr             crypto SRAM address of constructed Initialization
 *                              Vector / Counter initialization
 * @param auth_length           byte length of (SPI + SeqLo + seqHi(esn)), minus 1
 * @param zero                  crypto SRAM address of a 16B block of zeros
 * @param crypt_select          config word 1, produced by crypto_setup_configs
 * @param crypt_modes           config word 2, produced by crypto_setup_configs
 * @param sa_cik                crypto SRAM address of start of cipher key
 * @param sa_hmk                crypto SRAM address of start of hash key(unused)
 * @param sa_hmr                crypto SRAM address of hash result
 **/

/**
 *  Prepare transfer regs to load static (aka 'pinned') encryption sequence
 *
 */
#macro crypto_load_ipsec_enc_aesgcm (cr_xfr, cr_ctx, packet_in, packet_out, in_len, length_vector, auth_only_data, iv_constr, auth_length, zero, crypt_select, crypt_modes, sa_cik, sa_hmk, sa_hmr)
    crypto_build_xfer_reg (cr_xfr[0], packet_in, packet_out)
    crypto_build_xfer_reg (cr_xfr[1], in_len, length_vector)
    crypto_build_xfer_reg (cr_xfr[2], auth_only_data, iv_constr)
    crypto_build_xfer_reg (cr_xfr[3], auth_length, zero)
    crypto_build_xfer_reg (cr_xfr[4], crypt_select, crypt_modes)
    crypto_build_xfer_reg (cr_xfr[5], sa_cik, sa_hmk)
    crypto_build_xfer_reg (cr_xfr[6], sa_hmr, 0)
    immed [cr_xfr[7], 0]
    #define_eval _crypto_transfer_length 4
#endm


/**
 * Generate cmd sequence as constant data
 *
 *  @param core  crypto bulk core, 0 - 3 for NFP3XXX or 0 - 5 for NFP6XXX
 *  @param desc  sequence 'descriptor', contains sram location address
 *
 *  used in _crypto_library_load_dynamic in crypto_lib.uc
 */
#macro crypto_gen_ipsec_enc_aesgcm (core, desc)
    #if core==5
        #define_eval ipsec_enc_aesgcm_desc_5 (desc)
    #elif core==4
        #define_eval ipsec_enc_aesgcm_desc_4 (desc)
    #elif core==3
        #define_eval ipsec_enc_aesgcm_desc_3 (desc)
    #elif core==2
        #define_eval ipsec_enc_aesgcm_desc_2 (desc)
    #elif core==1
        #define_eval ipsec_enc_aesgcm_desc_1 (desc)
    #elif core==0
        #define_eval ipsec_enc_aesgcm_desc_0 (desc)
    #endif
    #define_eval gen_data_0 0
    #define_eval gen_data_1 0
    #define_eval gen_data_2 0
    #define_eval gen_data_3 0
    #define_eval gen_data_4 0
    #define_eval gen_data_5 0
    #define_eval gen_data_6 0
    #define_eval gen_data_7 0
    #define_eval gen_data_8 0x08702c19
    #define_eval gen_data_9 0x0a93221a
    #define_eval gen_data_10 0x0a23250d
    #define_eval gen_data_11 0x0a33250d
    #define_eval gen_data_12 0x08381c28
    #define_eval gen_data_13 0x0341250f
    #define_eval gen_data_14 0x0393250f
    #define_eval gen_data_15 0x08182309
    #define_eval gen_data_16 0x01100e0c
    #define_eval gen_data_17 0x03130a08
    #define_eval gen_data_18 0x1132250b
    #define_eval gen_data_19 0
    #define_eval _cib_xfer_size_ipsec_enc_aesgcm 20
    #define_eval _cib_code_size_ipsec_enc_aesgcm 11
    #define_eval _cib_vars_base_ipsec_enc_aesgcm 0
    #define_eval _cib_vars_size_ipsec_enc_aesgcm 8
    #define_eval _cib_vars_xfer_ipsec_enc_aesgcm 8
#endm

/**
 * ipsec_enc_aesgcm_strt
 *
 * Similar to ipsec_enc_aesgcm, but used to begin an encryption
 * sequence that will span multiple buffers, needed to handle jumbo packets.
 * Calling the sequence:
 *
 * load_ipsec_enc_aesgcm_strt (packet_in, packet_out, in_len, length_vector,
 *                        auth_only_data, iv_constr, auth_length, zero,
 *                        crypt_select, crypt_modes,
 *                        sa_cik, sa_hmk, sa_hmr)
 *
 * @param packet_in             crypto SRAM address of the start of plaintext
 * @param packet_out            crypto SRAM address of the start of ciphertext
 * @param in_len                length of data to be encrypted
 * @param length_vector         crypto SRAM address of the len(A)||len(C) vector
 * @param auth_only_data        crypto SRAM addr of SPI + SeqLo + SeqHi(esn)
 * @param iv_constr             crypto SRAM address of constructed Initialization
 *                              Vector / Counter initialization
 * @param auth_length           byte length of (SPI + SeqLo + seqHi(esn)), minus 1
 * @param zero                  crypto SRAM address of a 16B block of zeros
 * @param crypt_select          config word 1, produced by crypto_setup_configs
 * @param crypt_modes           config word 2, produced by crypto_setup_configs
 * @param sa_cik                crypto SRAM address of start of cipher key
 * @param sa_hmk                crypto SRAM address of start of hash key
 * @param sa_hmr                crypto SRAM address of hash result
 **/

/**
 *  Prepare transfer regs to load static (aka 'pinned') encryption sequence
 *
 */
#macro crypto_load_ipsec_enc_aesgcm_strt (cr_xfr, cr_ctx, packet_in, packet_out, in_len, length_vector, auth_only_data, iv_constr, auth_length, zero, crypt_select, crypt_modes, sa_cik, sa_hmk, sa_hmr)
    crypto_build_xfer_reg (cr_xfr[0], packet_in, packet_out)
    crypto_build_xfer_reg (cr_xfr[1], in_len, length_vector)
    crypto_build_xfer_reg (cr_xfr[2], auth_only_data, iv_constr)
    crypto_build_xfer_reg (cr_xfr[3], auth_length, zero)
    crypto_build_xfer_reg (cr_xfr[4], crypt_select, crypt_modes)
    crypto_build_xfer_reg (cr_xfr[5], sa_cik, sa_hmk)
    crypto_build_xfer_reg (cr_xfr[6], sa_hmr, 0)
    immed [cr_xfr[7], 0]
    #define_eval _crypto_transfer_length 4
#endm


/**
 * Generate cmd sequence as constant data
 *
 *  @param core  crypto bulk core, 0 - 3 for NFP3XXX or 0 - 5 for NFP6XXX
 *  @param desc  sequence 'descriptor', contains sram location address
 *
 *  used in _crypto_library_load_dynamic in crypto_lib.uc
 */
#macro crypto_gen_ipsec_enc_aesgcm_strt (core, desc)
    #if core==5
        #define_eval ipsec_enc_aesgcm_strt_desc_5 (desc)
    #elif core==4
        #define_eval ipsec_enc_aesgcm_strt_desc_4 (desc)
    #elif core==3
        #define_eval ipsec_enc_aesgcm_strt_desc_3 (desc)
    #elif core==2
        #define_eval ipsec_enc_aesgcm_strt_desc_2 (desc)
    #elif core==1
        #define_eval ipsec_enc_aesgcm_strt_desc_1 (desc)
    #elif core==0
        #define_eval ipsec_enc_aesgcm_strt_desc_0 (desc)
    #endif
    #define_eval gen_data_0 0
    #define_eval gen_data_1 0
    #define_eval gen_data_2 0
    #define_eval gen_data_3 0
    #define_eval gen_data_4 0
    #define_eval gen_data_5 0
    #define_eval gen_data_6 0
    #define_eval gen_data_7 0
    #define_eval gen_data_8 0x08702c19
    #define_eval gen_data_9 0x0a93221a
    #define_eval gen_data_10 0x0a23250d
    #define_eval gen_data_11 0x0a33250d
    #define_eval gen_data_12 0x08381c28
    #define_eval gen_data_13 0x0341250f
    #define_eval gen_data_14 0x0393250f
    #define_eval gen_data_15 0x08182309
    #define_eval gen_data_16 0x01100e0c
    #define_eval gen_data_17 0x13330a08
    #define_eval gen_data_18 0
    #define_eval gen_data_19 0
    #define_eval _cib_xfer_size_ipsec_enc_aesgcm_strt 20
    #define_eval _cib_code_size_ipsec_enc_aesgcm_strt 10
    #define_eval _cib_vars_base_ipsec_enc_aesgcm_strt 0
    #define_eval _cib_vars_size_ipsec_enc_aesgcm_strt 8
    #define_eval _cib_vars_xfer_ipsec_enc_aesgcm_strt 8
#endm

/**
 * ipsec_enc_aesgcm_strt_save
 *
 * Similiar to ipsec_enc_aesgcm_strt, but saves the hash state at the end.
 * Used when need to restart crypt operation on next buffer in packet, when the
 * crypt engine is interrupted between buffers
 *
 * load_ipsec_enc_aesgcm_strt_save (packet_in, packet_out, in_len, length_vector,
 *                        auth_only_data, iv_constr, auth_length, zero,
 *                        crypt_select, crypt_modes,
 *                        sa_cik, sa_hmk, sa_hmr)
 *
 * @param packet_in             crypto SRAM address of the start of plaintext
 * @param packet_out            crypto SRAM address of the start of ciphertext
 * @param in_len                length of data to be encrypted
 * @param length_vector         crypto SRAM address of the len(A)||len(C) vector
 * @param auth_only_data        crypto SRAM addr of SPI + SeqLo + SeqHi(esn)
 * @param iv_constr             crypto SRAM address of constructed Initialization
 *                              Vector / Counter initialization
 * @param auth_length           byte length of (SPI + SeqLo + seqHi(esn)), minus 1
 * @param zero                  crypto SRAM address of a 16B block of zeros
 * @param crypt_select          config word 1, produced by crypto_setup_configs
 * @param crypt_modes           config word 2, produced by crypto_setup_configs
 * @param sa_cik                crypto SRAM address of start of cipher key
 * @param sa_hmk                crypto SRAM address of start of hash key
 * @param sa_hmr                crypto SRAM address of hash result
 **/

/**
 *  Prepare transfer regs to load static (aka 'pinned') encryption sequence
 *
 */
#macro crypto_load_ipsec_enc_aesgcm_strt_save (cr_xfr, cr_ctx, packet_in, packet_out, in_len, length_vector, auth_only_data, iv_constr, auth_length, zero, crypt_select, crypt_modes, sa_cik, sa_hmk, sa_hmr)
    crypto_build_xfer_reg (cr_xfr[0], packet_in, packet_out)
    crypto_build_xfer_reg (cr_xfr[1], in_len, length_vector)
    crypto_build_xfer_reg (cr_xfr[2], auth_only_data, iv_constr)
    crypto_build_xfer_reg (cr_xfr[3], auth_length, zero)
    crypto_build_xfer_reg (cr_xfr[4], crypt_select, crypt_modes)
    crypto_build_xfer_reg (cr_xfr[5], sa_cik, sa_hmk)
    crypto_build_xfer_reg (cr_xfr[6], sa_hmr, 0)
    immed [cr_xfr[7], 0]
    #define_eval _crypto_transfer_length 4
#endm


/**
 * Generate cmd sequence as constant data
 *
 *  @param core  crypto bulk core, 0 - 3 for NFP3XXX or 0 - 5 for NFP6XXX
 *  @param desc  sequence 'descriptor', contains sram location address
 *
 *  used in _crypto_library_load_dynamic in crypto_lib.uc
 */
#macro crypto_gen_ipsec_enc_aesgcm_strt_save (core, desc)
    #if core==5
        #define_eval ipsec_enc_aesgcm_strt_save_desc_5 (desc)
    #elif core==4
        #define_eval ipsec_enc_aesgcm_strt_save_desc_4 (desc)
    #elif core==3
        #define_eval ipsec_enc_aesgcm_strt_save_desc_3 (desc)
    #elif core==2
        #define_eval ipsec_enc_aesgcm_strt_save_desc_2 (desc)
    #elif core==1
        #define_eval ipsec_enc_aesgcm_strt_save_desc_1 (desc)
    #elif core==0
        #define_eval ipsec_enc_aesgcm_strt_save_desc_0 (desc)
    #endif
    #define_eval gen_data_0 0
    #define_eval gen_data_1 0
    #define_eval gen_data_2 0
    #define_eval gen_data_3 0
    #define_eval gen_data_4 0
    #define_eval gen_data_5 0
    #define_eval gen_data_6 0
    #define_eval gen_data_7 0
    #define_eval gen_data_8 0x08702d19
    #define_eval gen_data_9 0x0a93221a
    #define_eval gen_data_10 0x0a23250d
    #define_eval gen_data_11 0x0a33250d
    #define_eval gen_data_12 0x08381c28
    #define_eval gen_data_13 0x0341250f
    #define_eval gen_data_14 0x0393250f
    #define_eval gen_data_15 0x08182309
    #define_eval gen_data_16 0x01100e0c
    #define_eval gen_data_17 0x08281c23
    #define_eval gen_data_18 0x13330a08
    #define_eval gen_data_19 0
    #define_eval _cib_xfer_size_ipsec_enc_aesgcm_strt_save 20
    #define_eval _cib_code_size_ipsec_enc_aesgcm_strt_save 11
    #define_eval _cib_vars_base_ipsec_enc_aesgcm_strt_save 0
    #define_eval _cib_vars_size_ipsec_enc_aesgcm_strt_save 8
    #define_eval _cib_vars_xfer_ipsec_enc_aesgcm_strt_save 8
#endm

/**
 * ipsec_enc_aesgcm_end
 *
 * Used after ipsec_enc_aesgcm_strt and possibly ipsec_enc_cont,
 * to complete encrypting a packet on the last buffer of data of the packet.
 * Needed to handle jumbo packets. Setup and ending condition from prior use of
 * ipsec_enc_aesgcm_strt is required prior to invoking this sequence. In
 * particular, the keys, config registers, etc. must remain intact when this
 * sequence is started.
 *
 * The hash result address should be the sram address corresponding to the
 * hash result for the buffer being used when this macro ( ipsec_enc_aesgcm_end )
 * is invoked. For example, if using buffer B, the hash result address would be
 * equal to the hash result address for buffer B
 *
 * Calling the sequence:
 *
 * crypto_load_ipsec_enc_aesgcm_end (packet_in, packet_out, in_len,
 *                              length_vector, unused, unused, hash_resadr)
 *
 * @param packet_in      crypto SRAM address of the start of plaintext
 * @param packet_out     crypto SRAM address of the start of ciphertext
 * @param in_len         length of data to be encrypted
 * @param length_vector  crypto SRAM address of the len(A)||len(C) vector
 * @param unused         unused
 * @param unused         unused
 * @param hash_resadr    crypto SRAM address of the hash calculation result
 **/

/**
 *  Prepare transfer regs to load static (aka 'pinned') encryption sequence
 *
 */
#macro crypto_load_ipsec_enc_aesgcm_end (cr_xfr, cr_ctx, packet_in, packet_out, in_len, length_vector, unused1, unused2, hash_resadr)
    crypto_build_xfer_reg (cr_xfr[0], packet_in, packet_out)
    crypto_build_xfer_reg (cr_xfr[1], in_len, length_vector)
    crypto_build_xfer_reg (cr_xfr[2], unused1, unused2)
    crypto_build_xfer_reg (cr_xfr[3], hash_resadr, 0)
    #define_eval _crypto_transfer_length 2
#endm


/**
 * Generate cmd sequence as constant data
 *
 *  @param core  crypto bulk core, 0 - 3 for NFP3XXX or 0 - 5 for NFP6XXX
 *  @param desc  sequence 'descriptor', contains sram location address
 *
 *  used in _crypto_library_load_dynamic in crypto_lib.uc
 */
#macro crypto_gen_ipsec_enc_aesgcm_end (core, desc)
    #if core==5
        #define_eval ipsec_enc_aesgcm_end_desc_5 (desc)
    #elif core==4
        #define_eval ipsec_enc_aesgcm_end_desc_4 (desc)
    #elif core==3
        #define_eval ipsec_enc_aesgcm_end_desc_3 (desc)
    #elif core==2
        #define_eval ipsec_enc_aesgcm_end_desc_2 (desc)
    #elif core==1
        #define_eval ipsec_enc_aesgcm_end_desc_1 (desc)
    #elif core==0
        #define_eval ipsec_enc_aesgcm_end_desc_0 (desc)
    #endif
    #define_eval gen_data_0 0
    #define_eval gen_data_1 0
    #define_eval gen_data_2 0
    #define_eval gen_data_3 0
    #define_eval gen_data_4 0x08380e09
    #define_eval gen_data_5 0x03330a08
    #define_eval gen_data_6 0x08280e23
    #define_eval gen_data_7 0x1132250b
    #define_eval _cib_xfer_size_ipsec_enc_aesgcm_end 8
    #define_eval _cib_code_size_ipsec_enc_aesgcm_end 4
    #define_eval _cib_vars_base_ipsec_enc_aesgcm_end 0
    #define_eval _cib_vars_size_ipsec_enc_aesgcm_end 4
    #define_eval _cib_vars_xfer_ipsec_enc_aesgcm_end 4
#endm

/**
 * ipsec_enc_aesgcm_end_restore
 *
 * Similar to ipsec_enc_aesgcm_end, but restores the crypto state before doing
 * the crypt operation. This seqeunce would be used when the processing of a packet
 * was split across different crypto engines, or if a single crypto engine that is
 * processing a packet using multiple buffers was interrupted in between the
 * multiple buffers.
 *
 * Calling the sequence:
 *
 * load_ipsec_enc_aesgcm_end_restore (packet_in, packet_out, in_len, length_vector,
 *                                    crypt_modes, iv_constr_prev,
 *                                    sa_cik, sa_hmr_prev, sa_hmr)
 *
 * @param packet_in      crypto SRAM address of the start of plaintext
 * @param packet_out     crypto SRAM address of the start of ciphertext
 * @param in_len         length of data to be encrypted
 * @param length_vector  crypto SRAM address of the len(A)||len(C) vector
 * @param crypt_modes    config word 2, produced by crypto_setup_configs
 * @param iv_constr_prv  iv constr with counter value at end of previous buffer
 * @param sa_cik         crypto SRAM address of start of cipher key
 * @param sa_hmr_prev    crypto SRAM address of hash result from previous buffer
 * @param sa_hmr         crypto SRAM address of hash result
 **/

/**
 *  Prepare transfer regs to load static (aka 'pinned') encryption sequence
 *
 */
#macro crypto_load_ipsec_enc_aesgcm_end_restore (cr_xfr, cr_ctx, packet_in, packet_out, in_len, length_vector, crypt_modes, iv_constr_prv, sa_cik, sa_hmr_prev, sa_hmr)
    crypto_build_xfer_reg (cr_xfr[0], packet_in, packet_out)
    crypto_build_xfer_reg (cr_xfr[1], in_len, length_vector)
    crypto_build_xfer_reg (cr_xfr[2], crypt_modes, iv_constr_prv)
    crypto_build_xfer_reg (cr_xfr[3], sa_cik, sa_hmr_prev)
    crypto_build_xfer_reg (cr_xfr[4], sa_hmr, 0)
    immed [cr_xfr[5], 0]
    #define_eval _crypto_transfer_length 3
#endm


/**
 * Generate cmd sequence as constant data
 *
 *  @param core  crypto bulk core, 0 - 3 for NFP3XXX or 0 - 5 for NFP6XXX
 *  @param desc  sequence 'descriptor', contains sram location address
 *
 *  used in _crypto_library_load_dynamic in crypto_lib.uc
 */
#macro crypto_gen_ipsec_enc_aesgcm_end_restore (core, desc)
    #if core==5
        #define_eval ipsec_enc_aesgcm_end_restore_desc_5 (desc)
    #elif core==4
        #define_eval ipsec_enc_aesgcm_end_restore_desc_4 (desc)
    #elif core==3
        #define_eval ipsec_enc_aesgcm_end_restore_desc_3 (desc)
    #elif core==2
        #define_eval ipsec_enc_aesgcm_end_restore_desc_2 (desc)
    #elif core==1
        #define_eval ipsec_enc_aesgcm_end_restore_desc_1 (desc)
    #elif core==0
        #define_eval ipsec_enc_aesgcm_end_restore_desc_0 (desc)
    #endif
    #define_eval gen_data_0 0
    #define_eval gen_data_1 0
    #define_eval gen_data_2 0
    #define_eval gen_data_3 0
    #define_eval gen_data_4 0
    #define_eval gen_data_5 0
    #define_eval gen_data_6 0x08402e23
    #define_eval gen_data_7 0x09d02323
    #define_eval gen_data_8 0x09d02323
    #define_eval gen_data_9 0x0830260c
    #define_eval gen_data_10 0x0a93220e
    #define_eval gen_data_11 0x0a33250d
    #define_eval gen_data_12 0x0a53270f
    #define_eval gen_data_13 0x08381809
    #define_eval gen_data_14 0x03370a08
    #define_eval gen_data_15 0x08281823
    #define_eval gen_data_16 0x1132250b
    #define_eval gen_data_17 0
    #define_eval gen_data_18 0
    #define_eval gen_data_19 0
    #define_eval _cib_xfer_size_ipsec_enc_aesgcm_end_restore 20
    #define_eval _cib_code_size_ipsec_enc_aesgcm_end_restore 11
    #define_eval _cib_vars_base_ipsec_enc_aesgcm_end_restore 0
    #define_eval _cib_vars_size_ipsec_enc_aesgcm_end_restore 6
    #define_eval _cib_vars_xfer_ipsec_enc_aesgcm_end_restore 6
#endm

/**
 * ipsec_dec
 *
 * Sequence to do IPSec-compatible decryption of a packet, using bulk cores that
 * stay 'pinned'to a Dispatcher context. This allows operation without per-packet
 * get_core()/free_core() ops. One context will do a get_core() at init time and
 * will hold onto that core for the life of the program.
 *
 * In most cases, the 'output' (plaintext) data is at the same address as the
 * 'input' (ciphertext) data. Only in the case of the 'aes-gcm null' ciphers the
 * output is sent to a different buffer than the input.
 *
 * Supports AES, DES, and NO encryption with MD5 or SHA-x HMAC auth.
 *
 * Calling the sequence:
 *
 * crypto_load_ipsec_dec (packet_in, packet_out, in_len, seq_ua, auth_only_data,
 *                        iv, auth_length, hmac_keylen, crypt_select, crypt_modes,
 *                        sa_cik, sa_hmk, sa_hmr)
 *
 * @param packet_in             crypto SRAM address of the start of ciphertext
 * @param packet_out            crypto SRAM address of the start of plaintext
 * @param in_len                length of data to be decrypted
 * @param seq_ua                crypto SRAM addr of seq number 63:32 (unused)
 * @param auth_only_data        crypto SRAM addr of auth-only data SPI/Seq
 * @param iv                    crypto SRAM address of the Initialization Vector
 * @param auth_length           byte length of (SPI/Seq
 * @param hmac_keylen           byte length of HMAC key, minus 1 (MD5-15, SHA1-19)
 * @param crypt_select          config word 1, produced by crypto_setup_configs
 * @param crypt_modes           config word 2, produced by crypto_setup_configs
 * @param sa_cik                crypto SRAM address of start of cipher key
 * @param sa_hmk                crypto SRAM address of start of hash key
 * @param sa_hmr                crypto SRAM address of hash result
 **/

/**
 *  Prepare transfer regs to load static (aka 'pinned') encryption sequence
 *
 */
#macro crypto_load_ipsec_dec (cr_xfr, cr_ctx, packet_in, packet_out, in_len, seq_ua, auth_only_data, iv, auth_length, hmac_keylen, crypt_select, crypt_modes, sa_cik, sa_hmk, sa_hmr)
    crypto_build_xfer_reg (cr_xfr[0], packet_in, packet_out)
    crypto_build_xfer_reg (cr_xfr[1], in_len, seq_ua)
    crypto_build_xfer_reg (cr_xfr[2], auth_only_data, iv)
    crypto_build_xfer_reg (cr_xfr[3], auth_length, hmac_keylen)
    crypto_build_xfer_reg (cr_xfr[4], crypt_select, crypt_modes)
    crypto_build_xfer_reg (cr_xfr[5], sa_cik, sa_hmk)
    crypto_build_xfer_reg (cr_xfr[6], sa_hmr, 0)
    immed [cr_xfr[7], 0]
    #define_eval _crypto_transfer_length 4
#endm


/**
 * Generate cmd sequence as constant data
 *
 *  @param core  crypto bulk core, 0 - 3 for NFP3XXX or 0 - 5 for NFP6XXX
 *  @param desc  sequence 'descriptor', contains sram location address
 *
 *  used in _crypto_library_load_dynamic in crypto_lib.uc
 */
#macro crypto_gen_ipsec_dec (core, desc)
    #if core==5
        #define_eval ipsec_dec_desc_5 (desc)
    #elif core==4
        #define_eval ipsec_dec_desc_4 (desc)
    #elif core==3
        #define_eval ipsec_dec_desc_3 (desc)
    #elif core==2
        #define_eval ipsec_dec_desc_2 (desc)
    #elif core==1
        #define_eval ipsec_dec_desc_1 (desc)
    #elif core==0
        #define_eval ipsec_dec_desc_0 (desc)
    #endif
    #define_eval gen_data_0 0
    #define_eval gen_data_1 0
    #define_eval gen_data_2 0
    #define_eval gen_data_3 0
    #define_eval gen_data_4 0
    #define_eval gen_data_5 0
    #define_eval gen_data_6 0
    #define_eval gen_data_7 0
    #define_eval gen_data_8 0x08701819
    #define_eval gen_data_9 0x0a13221a
    #define_eval gen_data_10 0x08382809
    #define_eval gen_data_11 0x01900f1b
    #define_eval gen_data_12 0x01000e0c
    #define_eval gen_data_13 0x0a230e0d
    #define_eval gen_data_14 0x00730a08
    #define_eval gen_data_15 0x08281c23
    #define_eval gen_data_16 0x01d00f1b
    #define_eval gen_data_17 0x11720f28
    #define_eval gen_data_18 0
    #define_eval gen_data_19 0
    #define_eval _cib_xfer_size_ipsec_dec 20
    #define_eval _cib_code_size_ipsec_dec 10
    #define_eval _cib_vars_base_ipsec_dec 0
    #define_eval _cib_vars_size_ipsec_dec 8
    #define_eval _cib_vars_xfer_ipsec_dec 8
#endm

/**
 * ipsec_dec_esn
 *
 * Similiar to ipsec_dec, but works for a 64 bit sequence number (ESN)
 *
 * Calling the sequence:
 *
 * crypto_load_ipsec_dec_esn (packet_in, packet_out, in_len, seq_ua,
 *                            auth_only_data, iv, auth_length, hmac_keylen,
 *                            crypt_select, crypt_modes, sa_cik, sa_hmk, sa_hmr)
 *
 * @param packet_in             crypto SRAM address of the start of ciphertext
 * @param packet_out            crypto SRAM address of the start of plaintext
 * @param in_len                length of data to be decrypted
 * @param seq_ua                crypto SRAM addr of seq number 63:32
 * @param auth_only_data        crypto SRAM addr of auth-only data SPI/Seq
 * @param iv                    crypto SRAM addr of the Initialization Vector
 * @param auth_length           byte length of (SPI/Seq
 * @param hmac_keylen           byte length of HMAC key, minus 1 (MD5-15, SHA1-19)
 * @param crypt_select          config word 1, produced by crypto_setup_configs
 * @param crypt_modes           config word 2, produced by crypto_setup_configs
 * @param sa_cik                crypto SRAM address of start of cipher key
 * @param sa_hmk                crypto SRAM address of start of hash key
 * @param sa_hmr                crypto SRAM address of hash result
 **/

/**
 *  Prepare transfer regs to load static (aka 'pinned') encryption sequence
 *
 */
#macro crypto_load_ipsec_dec_esn (cr_xfr, cr_ctx, packet_in, packet_out, in_len, seq_ua, auth_only_data, iv, auth_length, hmac_keylen, crypt_select, crypt_modes, sa_cik, sa_hmk, sa_hmr)
    crypto_build_xfer_reg (cr_xfr[0], packet_in, packet_out)
    crypto_build_xfer_reg (cr_xfr[1], in_len, seq_ua)
    crypto_build_xfer_reg (cr_xfr[2], auth_only_data, iv)
    crypto_build_xfer_reg (cr_xfr[3], auth_length, hmac_keylen)
    crypto_build_xfer_reg (cr_xfr[4], crypt_select, crypt_modes)
    crypto_build_xfer_reg (cr_xfr[5], sa_cik, sa_hmk)
    crypto_build_xfer_reg (cr_xfr[6], sa_hmr, 0)
    immed [cr_xfr[7], 0]
    #define_eval _crypto_transfer_length 4
#endm


/**
 * Generate cmd sequence as constant data
 *
 *  @param core  crypto bulk core, 0 - 3 for NFP3XXX or 0 - 5 for NFP6XXX
 *  @param desc  sequence 'descriptor', contains sram location address
 *
 *  used in _crypto_library_load_dynamic in crypto_lib.uc
 */
#macro crypto_gen_ipsec_dec_esn (core, desc)
    #if core==5
        #define_eval ipsec_dec_esn_desc_5 (desc)
    #elif core==4
        #define_eval ipsec_dec_esn_desc_4 (desc)
    #elif core==3
        #define_eval ipsec_dec_esn_desc_3 (desc)
    #elif core==2
        #define_eval ipsec_dec_esn_desc_2 (desc)
    #elif core==1
        #define_eval ipsec_dec_esn_desc_1 (desc)
    #elif core==0
        #define_eval ipsec_dec_esn_desc_0 (desc)
    #endif
    #define_eval gen_data_0 0
    #define_eval gen_data_1 0
    #define_eval gen_data_2 0
    #define_eval gen_data_3 0
    #define_eval gen_data_4 0
    #define_eval gen_data_5 0
    #define_eval gen_data_6 0
    #define_eval gen_data_7 0
    #define_eval gen_data_8 0x08701819
    #define_eval gen_data_9 0x0a13221a
    #define_eval gen_data_10 0x08382809
    #define_eval gen_data_11 0x01900f1b
    #define_eval gen_data_12 0x01000e0c
    #define_eval gen_data_13 0x0a230e0d
    #define_eval gen_data_14 0x00010a08
    #define_eval gen_data_15 0x0172240b
    #define_eval gen_data_16 0x08281c23
    #define_eval gen_data_17 0x01d00f1b
    #define_eval gen_data_18 0x11720f28
    #define_eval gen_data_19 0
    #define_eval _cib_xfer_size_ipsec_dec_esn 20
    #define_eval _cib_code_size_ipsec_dec_esn 11
    #define_eval _cib_vars_base_ipsec_dec_esn 0
    #define_eval _cib_vars_size_ipsec_dec_esn 8
    #define_eval _cib_vars_xfer_ipsec_dec_esn 8
#endm

/**
 * ipsec_dec_strt
 *
 * Similar to ipsec_dec but used to begin a decryption sequence
 * that will span multiple buffers, needed to handle jumbo packets.
 *
 * Calling the sequence:
 *
 * crypto_load_ipsec_dec_strt (packet_in, packet_out, in_len, seq_ua,
 *                            auth_only_data, iv, auth_length, hmac_keylen,
 *                            crypt_select, crypt_modes, sa_cik, sa_hmk, sa_hmr)
 *
 * @param packet_in           crypto SRAM address of the start of plaintext
 * @param packet_out          crypto SRAM address of the start of ciphertext
 * @param in_len              length of data to be decrypted
 * @param seq_ua              crypto SRAM addr of seq number 63:32 (unused)
 * @param auth_only_data      crypto SRAM addr of auth-only data SPI/Seq
 * @param iv                  crypto SRAM address of the Initialization Vector
 * @param auth_length         byte length of (SPI/Seq
 * @param hmac_keylen         byte length of HMAC key, minus 1
 * @param crypt_select        config word 1, produced by crypto_setup_configs
 * @param crypt_modes         config word 2, produced by crypto_setup_configs
 * @param sa_cik              crypto SRAM address of start of cipher key
 * @param sa_hmk              crypto SRAM address of start of hash key
 * @param sa_hmr              crypto SRAM address of hash result
 **/

/**
 *  Prepare transfer regs to load static (aka 'pinned') encryption sequence
 *
 */
#macro crypto_load_ipsec_dec_strt (cr_xfr, cr_ctx, packet_in, packet_out, in_len, seq_ua, auth_only_data, iv, auth_length, hmac_keylen, crypt_select, crypt_modes, sa_cik, sa_hmk, sa_hmr)
    crypto_build_xfer_reg (cr_xfr[0], packet_in, packet_out)
    crypto_build_xfer_reg (cr_xfr[1], in_len, seq_ua)
    crypto_build_xfer_reg (cr_xfr[2], auth_only_data, iv)
    crypto_build_xfer_reg (cr_xfr[3], auth_length, hmac_keylen)
    crypto_build_xfer_reg (cr_xfr[4], crypt_select, crypt_modes)
    crypto_build_xfer_reg (cr_xfr[5], sa_cik, sa_hmk)
    crypto_build_xfer_reg (cr_xfr[6], sa_hmr, 0)
    immed [cr_xfr[7], 0]
    #define_eval _crypto_transfer_length 4
#endm


/**
 * Generate cmd sequence as constant data
 *
 *  @param core  crypto bulk core, 0 - 3 for NFP3XXX or 0 - 5 for NFP6XXX
 *  @param desc  sequence 'descriptor', contains sram location address
 *
 *  used in _crypto_library_load_dynamic in crypto_lib.uc
 */
#macro crypto_gen_ipsec_dec_strt (core, desc)
    #if core==5
        #define_eval ipsec_dec_strt_desc_5 (desc)
    #elif core==4
        #define_eval ipsec_dec_strt_desc_4 (desc)
    #elif core==3
        #define_eval ipsec_dec_strt_desc_3 (desc)
    #elif core==2
        #define_eval ipsec_dec_strt_desc_2 (desc)
    #elif core==1
        #define_eval ipsec_dec_strt_desc_1 (desc)
    #elif core==0
        #define_eval ipsec_dec_strt_desc_0 (desc)
    #endif
    #define_eval gen_data_0 0
    #define_eval gen_data_1 0
    #define_eval gen_data_2 0
    #define_eval gen_data_3 0
    #define_eval gen_data_4 0
    #define_eval gen_data_5 0
    #define_eval gen_data_6 0
    #define_eval gen_data_7 0
    #define_eval gen_data_8 0x08701819
    #define_eval gen_data_9 0x0a13221a
    #define_eval gen_data_10 0x08382809
    #define_eval gen_data_11 0x01900f1b
    #define_eval gen_data_12 0x01000e0c
    #define_eval gen_data_13 0x0a230e0d
    #define_eval gen_data_14 0x10030a08
    #define_eval gen_data_15 0
    #define_eval _cib_xfer_size_ipsec_dec_strt 16
    #define_eval _cib_code_size_ipsec_dec_strt 7
    #define_eval _cib_vars_base_ipsec_dec_strt 0
    #define_eval _cib_vars_size_ipsec_dec_strt 8
    #define_eval _cib_vars_xfer_ipsec_dec_strt 8
#endm

/**
 * ipsec_dec_strt_nw
 *
 * Similar to ipsec_dec_strt, used to begin a decryption sequence
 * that will span multiple buffers, when no cipher is selected, so the sequence
 * does not have a wait for the cipher to complete. used to handle jumbo packets.
 *
 * Calling the sequence:
 *
 * crypto_load_ipsec_dec_strt_nw (packet_in, packet_out, in_len, seq_ua,
 *                            auth_only_data, iv, auth_length, hmac_keylen,
 *                            crypt_select, crypt_modes, sa_cik, sa_hmk, sa_hmr)
 *
 * @param packet_in           crypto SRAM address of the start of plaintext
 * @param packet_out          crypto SRAM address of the start of ciphertext
 * @param in_len              length of data to be decrypted
 * @param seq_ua              crypto SRAM addr of seq number 63:32 (unused)
 * @param auth_only_data      crypto SRAM addr of auth-only data SPI/Seq
 * @param iv                  crypto SRAM address of the Initialization Vector
 * @param auth_length         byte length of (SPI/Seq
 * @param hmac_keylen         byte length of HMAC key, minus 1
 * @param crypt_select        config word 1, produced by crypto_setup_configs
 * @param crypt_modes         config word 2, produced by crypto_setup_configs
 * @param sa_cik              crypto SRAM address of start of cipher key
 * @param sa_hmk              crypto SRAM address of start of hash key
 * @param sa_hmr              crypto SRAM address of hash result
 **/

/**
 *  Prepare transfer regs to load static (aka 'pinned') encryption sequence
 *
 */
#macro crypto_load_ipsec_dec_strt_nw (cr_xfr, cr_ctx, packet_in, packet_out, in_len, seq_ua, auth_only_data, iv, auth_length, hmac_keylen, crypt_select, crypt_modes, sa_cik, sa_hmk, sa_hmr)
    crypto_build_xfer_reg (cr_xfr[0], packet_in, packet_out)
    crypto_build_xfer_reg (cr_xfr[1], in_len, seq_ua)
    crypto_build_xfer_reg (cr_xfr[2], auth_only_data, iv)
    crypto_build_xfer_reg (cr_xfr[3], auth_length, hmac_keylen)
    crypto_build_xfer_reg (cr_xfr[4], crypt_select, crypt_modes)
    crypto_build_xfer_reg (cr_xfr[5], sa_cik, sa_hmk)
    crypto_build_xfer_reg (cr_xfr[6], sa_hmr, 0)
    immed [cr_xfr[7], 0]
    #define_eval _crypto_transfer_length 4
#endm


/**
 * Generate cmd sequence as constant data
 *
 *  @param core  crypto bulk core, 0 - 3 for NFP3XXX or 0 - 5 for NFP6XXX
 *  @param desc  sequence 'descriptor', contains sram location address
 *
 *  used in _crypto_library_load_dynamic in crypto_lib.uc
 */
#macro crypto_gen_ipsec_dec_strt_nw (core, desc)
    #if core==5
        #define_eval ipsec_dec_strt_nw_desc_5 (desc)
    #elif core==4
        #define_eval ipsec_dec_strt_nw_desc_4 (desc)
    #elif core==3
        #define_eval ipsec_dec_strt_nw_desc_3 (desc)
    #elif core==2
        #define_eval ipsec_dec_strt_nw_desc_2 (desc)
    #elif core==1
        #define_eval ipsec_dec_strt_nw_desc_1 (desc)
    #elif core==0
        #define_eval ipsec_dec_strt_nw_desc_0 (desc)
    #endif
    #define_eval gen_data_0 0
    #define_eval gen_data_1 0
    #define_eval gen_data_2 0
    #define_eval gen_data_3 0
    #define_eval gen_data_4 0
    #define_eval gen_data_5 0
    #define_eval gen_data_6 0
    #define_eval gen_data_7 0
    #define_eval gen_data_8 0x08701819
    #define_eval gen_data_9 0x0a13221a
    #define_eval gen_data_10 0x08382809
    #define_eval gen_data_11 0x01900f1b
    #define_eval gen_data_12 0x01000e0c
    #define_eval gen_data_13 0x0a230e0d
    #define_eval gen_data_14 0x10010a08
    #define_eval gen_data_15 0
    #define_eval _cib_xfer_size_ipsec_dec_strt_nw 16
    #define_eval _cib_code_size_ipsec_dec_strt_nw 7
    #define_eval _cib_vars_base_ipsec_dec_strt_nw 0
    #define_eval _cib_vars_size_ipsec_dec_strt_nw 8
    #define_eval _cib_vars_xfer_ipsec_dec_strt_nw 8
#endm

/**
 * ipsec_dec_cont
 *
 * Used after ipsec_dec_strt to continue decrypting a packet on a buffer of data
 * following the first part of the packet. Needed to handle jumbo packets.
 * Setup and ending condition from prior use of ipsec_dec_strt is
 * required prior to invoking this sequence. In particular, the keys, config
 * registers, hash address, etc. must remain intact when this sequence is
 * started.
 *
 * Calling the sequence:
 *
 * load_ipsec_dec_cont (packet_in, packet_out, in_len)
 *
 * @param packet_in       crypto SRAM address of the start of ciphertext
 * @param packet_out      crypto SRAM address of the start of plaintext
 * @param in_len          length of data to decrypt
 **/

/**
 *  Prepare transfer regs to load static (aka 'pinned') encryption sequence
 *
 */
#macro crypto_load_ipsec_dec_cont (cr_xfr, cr_ctx, packet_in, packet_out, in_len)
    crypto_build_xfer_reg (cr_xfr[0], packet_in, packet_out)
    crypto_build_xfer_reg (cr_xfr[1], in_len, 0)
    #define_eval _crypto_transfer_length 1
#endm


/**
 * Generate cmd sequence as constant data
 *
 *  @param core  crypto bulk core, 0 - 3 for NFP3XXX or 0 - 5 for NFP6XXX
 *  @param desc  sequence 'descriptor', contains sram location address
 *
 *  used in _crypto_library_load_dynamic in crypto_lib.uc
 */
#macro crypto_gen_ipsec_dec_cont (core, desc)
    #if core==5
        #define_eval ipsec_dec_cont_desc_5 (desc)
    #elif core==4
        #define_eval ipsec_dec_cont_desc_4 (desc)
    #elif core==3
        #define_eval ipsec_dec_cont_desc_3 (desc)
    #elif core==2
        #define_eval ipsec_dec_cont_desc_2 (desc)
    #elif core==1
        #define_eval ipsec_dec_cont_desc_1 (desc)
    #elif core==0
        #define_eval ipsec_dec_cont_desc_0 (desc)
    #endif
    #define_eval gen_data_0 0
    #define_eval gen_data_1 0
    #define_eval gen_data_2 0x08182309
    #define_eval gen_data_3 0x10030a08
    #define_eval _cib_xfer_size_ipsec_dec_cont 4
    #define_eval _cib_code_size_ipsec_dec_cont 2
    #define_eval _cib_vars_base_ipsec_dec_cont 0
    #define_eval _cib_vars_size_ipsec_dec_cont 2
    #define_eval _cib_vars_xfer_ipsec_dec_cont 2
#endm

/**
 * ipsec_dec_cont_nw
 *
 * Similar to ipsec_dec_cont, but used on no cipher selection, so it does
 * not have a wait for cipher. used for jumbo packets
 *
 * Calling the sequence:
 *
 * load_ipsec_dec_cont_nw (packet_in, packet_out, in_len)
 *
 * @param packet_in       crypto SRAM address of the start of ciphertext
 * @param packet_out      crypto SRAM address of the start of plaintext
 * @param in_len          length of data to decrypt
 **/

/**
 *  Prepare transfer regs to load static (aka 'pinned') encryption sequence
 *
 */
#macro crypto_load_ipsec_dec_cont_nw (cr_xfr, cr_ctx, packet_in, packet_out, in_len)
    crypto_build_xfer_reg (cr_xfr[0], packet_in, packet_out)
    crypto_build_xfer_reg (cr_xfr[1], in_len, 0)
    #define_eval _crypto_transfer_length 1
#endm


/**
 * Generate cmd sequence as constant data
 *
 *  @param core  crypto bulk core, 0 - 3 for NFP3XXX or 0 - 5 for NFP6XXX
 *  @param desc  sequence 'descriptor', contains sram location address
 *
 *  used in _crypto_library_load_dynamic in crypto_lib.uc
 */
#macro crypto_gen_ipsec_dec_cont_nw (core, desc)
    #if core==5
        #define_eval ipsec_dec_cont_nw_desc_5 (desc)
    #elif core==4
        #define_eval ipsec_dec_cont_nw_desc_4 (desc)
    #elif core==3
        #define_eval ipsec_dec_cont_nw_desc_3 (desc)
    #elif core==2
        #define_eval ipsec_dec_cont_nw_desc_2 (desc)
    #elif core==1
        #define_eval ipsec_dec_cont_nw_desc_1 (desc)
    #elif core==0
        #define_eval ipsec_dec_cont_nw_desc_0 (desc)
    #endif
    #define_eval gen_data_0 0
    #define_eval gen_data_1 0
    #define_eval gen_data_2 0x08182309
    #define_eval gen_data_3 0x10010a08
    #define_eval _cib_xfer_size_ipsec_dec_cont_nw 4
    #define_eval _cib_code_size_ipsec_dec_cont_nw 2
    #define_eval _cib_vars_base_ipsec_dec_cont_nw 0
    #define_eval _cib_vars_size_ipsec_dec_cont_nw 2
    #define_eval _cib_vars_xfer_ipsec_dec_cont_nw 2
#endm

/**
 * ipsec_dec_end
 *
 * Used after ipsec_dec_strt and possibly ipsec_dec_cont, to complete
 * decrypting a packet on the last buffer of data of the packet.
 * Needed to handle jumbo packets. Setup and ending condition from prior use of
 * ipsec_dec_strt is required prior to invoking this sequence. In
 * particular, the keys, config registers, etc. must remain intact when this
 * sequence is started.
 *
 * The hash key address and result address for the packet must be specified as
 * parameters to this sequence.
 *
 * The key address should be equal to the sram location for the hash key
 * corresponding to the buffer ( A,B,C or D) used for the 1st part of the
 * packet. For e.g., if the 1st part of the packet was loaded using buffer A,
 * the hash key address would be the same as provided in the variable sa_hmk
 * provided with ipsec_dec_strt since that is where the key was loaded.
 *
 * The hash key result address should be the sram address corresponding to the
 * hash result for the buffer being used when this macro ( ipsec_dec_end ) is
 * invoked. For example, if using buffer B, the hash result address would be
 * equal to location of the hash for buffer B.
 *
 * Calling the sequence:
 *
 * crypto_load_ipsec_dec_end (packet_in, packet_out, in_len, seq_ua,
 *                            hmac_keyadr, hmac_keylen, hmac_resadr)
 *
 * @param packet_in      crypto SRAM address of the start of ciphertext
 * @param packet_out     crypto SRAM address of the start of plaintext
 * @param in_len         length of data to be decrypted
 * @param seq_ua         crypto SRAM addr of seq number 63:32 (unused)
 * @param hmac_keyadr    crypto SRAM address of the start of the HMAC key
 * @param hmac_keylen    byte length of HMAC key, minus 1
 * @param hmac_resadr    crypto SRAM address of the HMAC key calculation result
 **/

/**
 *  Prepare transfer regs to load static (aka 'pinned') encryption sequence
 *
 */
#macro crypto_load_ipsec_dec_end (cr_xfr, cr_ctx, packet_in, packet_out, in_len, seq_ua, hmac_keyadr, hmac_keylen, hmac_resadr)
    crypto_build_xfer_reg (cr_xfr[0], packet_in, packet_out)
    crypto_build_xfer_reg (cr_xfr[1], in_len, seq_ua)
    crypto_build_xfer_reg (cr_xfr[2], hmac_keyadr, hmac_keylen)
    crypto_build_xfer_reg (cr_xfr[3], hmac_resadr, 0)
    #define_eval _crypto_transfer_length 2
#endm


/**
 * Generate cmd sequence as constant data
 *
 *  @param core  crypto bulk core, 0 - 3 for NFP3XXX or 0 - 5 for NFP6XXX
 *  @param desc  sequence 'descriptor', contains sram location address
 *
 *  used in _crypto_library_load_dynamic in crypto_lib.uc
 */
#macro crypto_gen_ipsec_dec_end (core, desc)
    #if core==5
        #define_eval ipsec_dec_end_desc_5 (desc)
    #elif core==4
        #define_eval ipsec_dec_end_desc_4 (desc)
    #elif core==3
        #define_eval ipsec_dec_end_desc_3 (desc)
    #elif core==2
        #define_eval ipsec_dec_end_desc_2 (desc)
    #elif core==1
        #define_eval ipsec_dec_end_desc_1 (desc)
    #elif core==0
        #define_eval ipsec_dec_end_desc_0 (desc)
    #endif
    #define_eval gen_data_0 0
    #define_eval gen_data_1 0
    #define_eval gen_data_2 0
    #define_eval gen_data_3 0
    #define_eval gen_data_4 0x08182309
    #define_eval gen_data_5 0x00730a08
    #define_eval gen_data_6 0x08280e23
    #define_eval gen_data_7 0x01d00d0c
    #define_eval gen_data_8 0x11720d28
    #define_eval gen_data_9 0
    #define_eval gen_data_10 0
    #define_eval gen_data_11 0
    #define_eval _cib_xfer_size_ipsec_dec_end 12
    #define_eval _cib_code_size_ipsec_dec_end 5
    #define_eval _cib_vars_base_ipsec_dec_end 0
    #define_eval _cib_vars_size_ipsec_dec_end 4
    #define_eval _cib_vars_xfer_ipsec_dec_end 4
#endm

/**
 * ipsec_dec_end_esn
 *
 * Similiar to ipsec_dec_end, but works for a 64 bit sequence number (ESN)
 *
 * Calling the sequence:
 *
 * crypto_load_ipsec_dec_end_esn (packet_in, packet_out, in_len, seq_ua,
 *                                hmac_keyadr, hmac_keylen, hmac_resadr)
 *
 * @param packet_in      crypto SRAM address of the start of cipherext
 * @param packet_out     crypto SRAM address of the start of plaintext
 * @param in_len         length of data to be decrypted
 * @param seq_ua         crypto SRAM addr of seq number 63:32
 * @param hmac_keyadr    crypto SRAM address of the start of the HMAC key
 * @param hmac_keylen    byte length of HMAC key, minus 1
 * @param hmac_resadr    crypto SRAM address of the HMAC key calculation result
 **/

/**
 *  Prepare transfer regs to load static (aka 'pinned') encryption sequence
 *
 */
#macro crypto_load_ipsec_dec_end_esn (cr_xfr, cr_ctx, packet_in, packet_out, in_len, seq_ua, hmac_keyadr, hmac_keylen, hmac_resadr)
    crypto_build_xfer_reg (cr_xfr[0], packet_in, packet_out)
    crypto_build_xfer_reg (cr_xfr[1], in_len, seq_ua)
    crypto_build_xfer_reg (cr_xfr[2], hmac_keyadr, hmac_keylen)
    crypto_build_xfer_reg (cr_xfr[3], hmac_resadr, 0)
    #define_eval _crypto_transfer_length 2
#endm


/**
 * Generate cmd sequence as constant data
 *
 *  @param core  crypto bulk core, 0 - 3 for NFP3XXX or 0 - 5 for NFP6XXX
 *  @param desc  sequence 'descriptor', contains sram location address
 *
 *  used in _crypto_library_load_dynamic in crypto_lib.uc
 */
#macro crypto_gen_ipsec_dec_end_esn (core, desc)
    #if core==5
        #define_eval ipsec_dec_end_esn_desc_5 (desc)
    #elif core==4
        #define_eval ipsec_dec_end_esn_desc_4 (desc)
    #elif core==3
        #define_eval ipsec_dec_end_esn_desc_3 (desc)
    #elif core==2
        #define_eval ipsec_dec_end_esn_desc_2 (desc)
    #elif core==1
        #define_eval ipsec_dec_end_esn_desc_1 (desc)
    #elif core==0
        #define_eval ipsec_dec_end_esn_desc_0 (desc)
    #endif
    #define_eval gen_data_0 0
    #define_eval gen_data_1 0
    #define_eval gen_data_2 0
    #define_eval gen_data_3 0
    #define_eval gen_data_4 0x08182309
    #define_eval gen_data_5 0x00010a08
    #define_eval gen_data_6 0x0172240b
    #define_eval gen_data_7 0x08280e23
    #define_eval gen_data_8 0x01d00d0c
    #define_eval gen_data_9 0x11720d28
    #define_eval gen_data_10 0
    #define_eval gen_data_11 0
    #define_eval _cib_xfer_size_ipsec_dec_end_esn 12
    #define_eval _cib_code_size_ipsec_dec_end_esn 6
    #define_eval _cib_vars_base_ipsec_dec_end_esn 0
    #define_eval _cib_vars_size_ipsec_dec_end_esn 4
    #define_eval _cib_vars_xfer_ipsec_dec_end_esn 4
#endm

/**
 * ipsec_dec_aesgcm
 *
 * Similar to ipsec_dec, but used for gcm (galois counter mode) for
 * aes-gcm-esp, esn or non-esn and either regular or 'null' (rfc4543) decrypt.
 * In the normal case, the plaintext is written to the same location as the
 * ciphertext. In the 'null' case, the plaintext is written to another buffer
 * instead; this buffer is used as a temp area and is not transmitted.
 *
 * Calling the sequence:
 *
 * load_ipsec_dec_aesgcm (packet_in, packet_out, in_len, length_vector,
 *                        auth_only_data, iv_constr, auth_length, zero,
 *                        crypt_select, crypt_modes, sa_cik, sa_hmr)
 *
 * @param packet_in             crypto SRAM address of the start of ciphertext
 * @param packet_out            crypto SRAM address of the start of plaintext
 * @param in_len                length of data to be decrypted
 * @param length_vector         crypto SRAM address of the len(A)||len(C) vector
 * @param auth_only_data        crypto SRAM addr of SPI + SeqLo + SeqHi(esn)
 * @param iv_constr             crypto SRAM address of constructed Initialization
 *                              Vector / Counter initialization
 * @param auth_length           byte length of (SPI + SeqLo + seqHi(esn)), minus 1
 * @param zero                  crypto SRAM address of a 16B block of zeros
 * @param crypt_select          config word 1, produced by crypto_setup_configs
 * @param crypt_modes           config word 2, produced by crypto_setup_configs
 * @param sa_cik                crypto SRAM address of start of cipher key
 * @param sa_hmk                crypto SRAM address of start of hash key (unused)
 * @param sa_hmr                crypto SRAM address of hash result
 **/

/**
 *  Prepare transfer regs to load static (aka 'pinned') encryption sequence
 *
 */
#macro crypto_load_ipsec_dec_aesgcm (cr_xfr, cr_ctx, packet_in, packet_out, in_len, length_vector, auth_only_data, iv_constr, auth_length, zero, crypt_select, crypt_modes, sa_cik, sa_hmk, sa_hmr)
    crypto_build_xfer_reg (cr_xfr[0], packet_in, packet_out)
    crypto_build_xfer_reg (cr_xfr[1], in_len, length_vector)
    crypto_build_xfer_reg (cr_xfr[2], auth_only_data, iv_constr)
    crypto_build_xfer_reg (cr_xfr[3], auth_length, zero)
    crypto_build_xfer_reg (cr_xfr[4], crypt_select, crypt_modes)
    crypto_build_xfer_reg (cr_xfr[5], sa_cik, sa_hmk)
    crypto_build_xfer_reg (cr_xfr[6], sa_hmr, 0)
    immed [cr_xfr[7], 0]
    #define_eval _crypto_transfer_length 4
#endm


/**
 * Generate cmd sequence as constant data
 *
 *  @param core  crypto bulk core, 0 - 3 for NFP3XXX or 0 - 5 for NFP6XXX
 *  @param desc  sequence 'descriptor', contains sram location address
 *
 *  used in _crypto_library_load_dynamic in crypto_lib.uc
 */
#macro crypto_gen_ipsec_dec_aesgcm (core, desc)
    #if core==5
        #define_eval ipsec_dec_aesgcm_desc_5 (desc)
    #elif core==4
        #define_eval ipsec_dec_aesgcm_desc_4 (desc)
    #elif core==3
        #define_eval ipsec_dec_aesgcm_desc_3 (desc)
    #elif core==2
        #define_eval ipsec_dec_aesgcm_desc_2 (desc)
    #elif core==1
        #define_eval ipsec_dec_aesgcm_desc_1 (desc)
    #elif core==0
        #define_eval ipsec_dec_aesgcm_desc_0 (desc)
    #endif
    #define_eval gen_data_0 0
    #define_eval gen_data_1 0
    #define_eval gen_data_2 0
    #define_eval gen_data_3 0
    #define_eval gen_data_4 0
    #define_eval gen_data_5 0
    #define_eval gen_data_6 0
    #define_eval gen_data_7 0
    #define_eval gen_data_8 0x08702c19
    #define_eval gen_data_9 0x0a93221a
    #define_eval gen_data_10 0x0a23250d
    #define_eval gen_data_11 0x0a33250d
    #define_eval gen_data_12 0x08381c28
    #define_eval gen_data_13 0x0341250f
    #define_eval gen_data_14 0x0393250f
    #define_eval gen_data_15 0x08182309
    #define_eval gen_data_16 0x01100e0c
    #define_eval gen_data_17 0x00130a08
    #define_eval gen_data_18 0x1132250b
    #define_eval gen_data_19 0
    #define_eval _cib_xfer_size_ipsec_dec_aesgcm 20
    #define_eval _cib_code_size_ipsec_dec_aesgcm 11
    #define_eval _cib_vars_base_ipsec_dec_aesgcm 0
    #define_eval _cib_vars_size_ipsec_dec_aesgcm 8
    #define_eval _cib_vars_xfer_ipsec_dec_aesgcm 8
#endm

/**
 * ipsec_dec_aesgcm_strt
 *
 * Similar to ipsec_dec_aesgcm, but used to begin an encryption
 * sequence that will span multiple buffers, needed to handle jumbo packets.
 *
 * Calling the sequence:
 *
 * load_ipsec_dec_aesgcm_strt (packet_in, packet_out, in_len, length_vector,
 *                             auth_only_data, iv_constr, auth_length, zero,
 *                             crypt_select, crypt_modes, sa_cik, sa_hmr)
 *
 * @param packet_in             crypto SRAM address of the start of ciphertext
 * @param packet_out            crypto SRAM address of the start of plaintext
 * @param in_len                length of data to be decrypted
 * @param length_vector         crypto SRAM address of the len(A)||len(C) vector
 * @param auth_only_data        crypto SRAM addr of data SPI + SeqLo + SeqHi(esn)
 * @param iv_constr             crypto SRAM address of constructed Initialization
 *                              Vector / Counter initialization
 * @param auth_length           byte length of (SPI + SeqLo + seqHi(esn)), minus 1
 * @param zero                  crypto SRAM address of a 16B block of zeros
 * @param crypt_select          config word 1, produced by crypto_setup_configs
 * @param crypt_modes           config word 2, produced by crypto_setup_configs
 * @param sa_cik                crypto SRAM address of start of cipher key
 * @param sa_hmk                crypto SRAM address of start of hash key (unused)
 * @param sa_hmr                crypto SRAM address of hash result
 **/

/**
 *  Prepare transfer regs to load static (aka 'pinned') encryption sequence
 *
 */
#macro crypto_load_ipsec_dec_aesgcm_strt (cr_xfr, cr_ctx, packet_in, packet_out, in_len, length_vector, auth_only_data, iv_constr, auth_length, zero, crypt_select, crypt_modes, sa_cik, sa_hmk, sa_hmr)
    crypto_build_xfer_reg (cr_xfr[0], packet_in, packet_out)
    crypto_build_xfer_reg (cr_xfr[1], in_len, length_vector)
    crypto_build_xfer_reg (cr_xfr[2], auth_only_data, iv_constr)
    crypto_build_xfer_reg (cr_xfr[3], auth_length, zero)
    crypto_build_xfer_reg (cr_xfr[4], crypt_select, crypt_modes)
    crypto_build_xfer_reg (cr_xfr[5], sa_cik, sa_hmk)
    crypto_build_xfer_reg (cr_xfr[6], sa_hmr, 0)
    immed [cr_xfr[7], 0]
    #define_eval _crypto_transfer_length 4
#endm


/**
 * Generate cmd sequence as constant data
 *
 *  @param core  crypto bulk core, 0 - 3 for NFP3XXX or 0 - 5 for NFP6XXX
 *  @param desc  sequence 'descriptor', contains sram location address
 *
 *  used in _crypto_library_load_dynamic in crypto_lib.uc
 */
#macro crypto_gen_ipsec_dec_aesgcm_strt (core, desc)
    #if core==5
        #define_eval ipsec_dec_aesgcm_strt_desc_5 (desc)
    #elif core==4
        #define_eval ipsec_dec_aesgcm_strt_desc_4 (desc)
    #elif core==3
        #define_eval ipsec_dec_aesgcm_strt_desc_3 (desc)
    #elif core==2
        #define_eval ipsec_dec_aesgcm_strt_desc_2 (desc)
    #elif core==1
        #define_eval ipsec_dec_aesgcm_strt_desc_1 (desc)
    #elif core==0
        #define_eval ipsec_dec_aesgcm_strt_desc_0 (desc)
    #endif
    #define_eval gen_data_0 0
    #define_eval gen_data_1 0
    #define_eval gen_data_2 0
    #define_eval gen_data_3 0
    #define_eval gen_data_4 0
    #define_eval gen_data_5 0
    #define_eval gen_data_6 0
    #define_eval gen_data_7 0
    #define_eval gen_data_8 0x08702c19
    #define_eval gen_data_9 0x0a93221a
    #define_eval gen_data_10 0x0a23250d
    #define_eval gen_data_11 0x0a33250d
    #define_eval gen_data_12 0x08381c28
    #define_eval gen_data_13 0x0341250f
    #define_eval gen_data_14 0x0393250f
    #define_eval gen_data_15 0x08182309
    #define_eval gen_data_16 0x01100e0c
    #define_eval gen_data_17 0x10330a08
    #define_eval gen_data_18 0
    #define_eval gen_data_19 0
    #define_eval _cib_xfer_size_ipsec_dec_aesgcm_strt 20
    #define_eval _cib_code_size_ipsec_dec_aesgcm_strt 10
    #define_eval _cib_vars_base_ipsec_dec_aesgcm_strt 0
    #define_eval _cib_vars_size_ipsec_dec_aesgcm_strt 8
    #define_eval _cib_vars_xfer_ipsec_dec_aesgcm_strt 8
#endm

/**
 * ipsec_dec_aesgcm_end
 *
 * Used after ipsec_dec_aesgcm_strt and possibly ipsec_dec_cont,
 * to complete decrypting a packet on the last buffer of data of the packet.
 * Needed to handle jumbo packets. Setup and ending condition from prior use of
 * ipsec_dec_aesgcm_strt is required prior to invoking this sequence. In
 * particular, the keys, config registers, etc. must remain intact when this
 * sequence is started.
 *
 * The hash result address should be the sram address corresponding to the
 * hash result for the buffer being used when this macro ( ipsec_dec_aesgcm_end )
 * is invoked. For example, if using buffer B, the hash result address would be
 * equal to the hash result address for buffer B
 *
 * Calling the sequence:
 *
 * crypto_load_ipsec_dec_aesgcm_end (packet_in, packet_out, in_len, seq_ua,
 *                                   unused, length_vector, hash_resadr)
 *
 * @param packet_in      crypto SRAM address of the start of plaintext
 * @param packet_out     crypto SRAM address of the start of ciphertext
 * @param in_len         length of data to be decrypted
 * @param length_vector  crypto SRAM address of the len(A)||len(C) vector
 * @param unused1        unused
 * @param unused2        unused
 * @param hash_resadr    crypto SRAM address of the hash calculation result
 **/

/**
 *  Prepare transfer regs to load static (aka 'pinned') encryption sequence
 *
 */
#macro crypto_load_ipsec_dec_aesgcm_end (cr_xfr, cr_ctx, packet_in, packet_out, in_len, length_vector, unused1, unused2, hash_resadr)
    crypto_build_xfer_reg (cr_xfr[0], packet_in, packet_out)
    crypto_build_xfer_reg (cr_xfr[1], in_len, length_vector)
    crypto_build_xfer_reg (cr_xfr[2], unused1, unused2)
    crypto_build_xfer_reg (cr_xfr[3], hash_resadr, 0)
    #define_eval _crypto_transfer_length 2
#endm


/**
 * Generate cmd sequence as constant data
 *
 *  @param core  crypto bulk core, 0 - 3 for NFP3XXX or 0 - 5 for NFP6XXX
 *  @param desc  sequence 'descriptor', contains sram location address
 *
 *  used in _crypto_library_load_dynamic in crypto_lib.uc
 */
#macro crypto_gen_ipsec_dec_aesgcm_end (core, desc)
    #if core==5
        #define_eval ipsec_dec_aesgcm_end_desc_5 (desc)
    #elif core==4
        #define_eval ipsec_dec_aesgcm_end_desc_4 (desc)
    #elif core==3
        #define_eval ipsec_dec_aesgcm_end_desc_3 (desc)
    #elif core==2
        #define_eval ipsec_dec_aesgcm_end_desc_2 (desc)
    #elif core==1
        #define_eval ipsec_dec_aesgcm_end_desc_1 (desc)
    #elif core==0
        #define_eval ipsec_dec_aesgcm_end_desc_0 (desc)
    #endif
    #define_eval gen_data_0 0
    #define_eval gen_data_1 0
    #define_eval gen_data_2 0
    #define_eval gen_data_3 0
    #define_eval gen_data_4 0x08380e09
    #define_eval gen_data_5 0x00330a08
    #define_eval gen_data_6 0x08280e23
    #define_eval gen_data_7 0x1132250b
    #define_eval _cib_xfer_size_ipsec_dec_aesgcm_end 8
    #define_eval _cib_code_size_ipsec_dec_aesgcm_end 4
    #define_eval _cib_vars_base_ipsec_dec_aesgcm_end 0
    #define_eval _cib_vars_size_ipsec_dec_aesgcm_end 4
    #define_eval _cib_vars_xfer_ipsec_dec_aesgcm_end 4
#endm

/**
 * SA layout for IPSec-oriented crypto instruction sequences
 *
 * byte offset   description
 *
 * 0x20-0x3F     cipher key (up to 32 bytes)
 * 0x40-0x7F     authentication key (up to 64 bytes)
 *
 * Based constants:
 * temp0 and temp1        - used for scratch space
 *
 **/
#define CRYPTO_NFP_MODE 1  // NFP6xxx or NFP38xx mode

#macro crypto_gen_compr_constants (base0)
    #define_eval gen_data_0 0x001f0000
    #define_eval gen_data_0 (gen_data_0 + base0)
    #define_eval gen_data_1 0x2fd10f03
    #define_eval gen_data_2 0x0f510fd1
    #define_eval gen_data_3 0x00000f00
    #define_eval _cib_xfer_size_compr_constants 0x0004
    #define_eval _cib_code_size_compr_constants 0x0004
    #define_eval _cib_vars_size_compr_constants 0
    #define_eval _cib_base_addr_compr_constants 0x0008
#endm

#endif
