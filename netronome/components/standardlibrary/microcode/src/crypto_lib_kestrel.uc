/*
* Copyright (C) 2018 Netronome Systems, Inc. All rights reserved.
*/
/** @file crypto_lib_kestrel.uc CRYPTO Operation Macros
* @addtogroup crypto CRYPTO Operation
* @{
*
* @name CRYPTO Operation Macros
* @{
*
* This file contains a set of crypto-library sequences.  The sequences are
* designed to be compatible with Netronome's crypto_support facility. They are
* implemented as 'compressed' sequences, which can be preloaded to the CIB
* memory space of the bulk core units at initialization time, and then invoked
* at run-time on a per-packet basis quickly and efficiently.
*
* The source file for these sequences is crypto_lib_kestrel.crypt.
* The source file is processed by the Netronome utility ca2.py to generate the
* file crypto_lib_kestrel.uc, which is included in the microcode.
* The macros defined in crypto_lib_kestrel.uc are used by the macros in
* crypto_lib.uc.
*
*/
#macro crypto_load_generic_chacha20 (cr_xfr, cr_ctx, payload, in_len, iv_constr)
    crypto_build_xfer_reg (cr_xfr[0], payload, in_len)
    crypto_build_xfer_reg (cr_xfr[1], iv_constr, 0)
    #define_eval _crypto_transfer_length 1
#endm

#macro crypto_gen_generic_chacha20 (core, desc)
    #if core==5
        #define_eval generic_chacha20_desc_5 (desc)
    #elif core==4
        #define_eval generic_chacha20_desc_4 (desc)
    #elif core==3
        #define_eval generic_chacha20_desc_3 (desc)
    #elif core==2
        #define_eval generic_chacha20_desc_2 (desc)
    #elif core==1
        #define_eval generic_chacha20_desc_1 (desc)
    #elif core==0
        #define_eval generic_chacha20_desc_0 (desc)
    #endif
    #define_eval gen_data_0 0
    #define_eval gen_data_1 0
    #define_eval gen_data_2 0x08600e00
    #define_eval gen_data_3 0x0a93010f
    #define_eval gen_data_4 0x0a23025a
    #define_eval gen_data_5 0x0a33025a
    #define_eval gen_data_6 0x08180358
    #define_eval gen_data_7 0x12035958
    #define_eval _cib_xfer_size_generic_chacha20 8
    #define_eval _cib_code_size_generic_chacha20 6
    #define_eval _cib_vars_base_generic_chacha20 20
    #define_eval _cib_vars_size_generic_chacha20 2
    #define_eval _cib_vars_xfer_generic_chacha20 2
#endm

#macro crypto_load_generic_poly1305 (cr_xfr, cr_ctx, payload, in_len)
    crypto_build_xfer_reg (cr_xfr[0], payload, in_len)
    immed [cr_xfr[1], 0]
    #define_eval _crypto_transfer_length 1
#endm

#macro crypto_gen_generic_poly1305 (core, desc)
    #if core==5
        #define_eval generic_poly1305_desc_5 (desc)
    #elif core==4
        #define_eval generic_poly1305_desc_4 (desc)
    #elif core==3
        #define_eval generic_poly1305_desc_3 (desc)
    #elif core==2
        #define_eval generic_poly1305_desc_2 (desc)
    #elif core==1
        #define_eval generic_poly1305_desc_1 (desc)
    #elif core==0
        #define_eval generic_poly1305_desc_0 (desc)
    #endif
    #define_eval gen_data_0 0
    #define_eval gen_data_1 0
    #define_eval gen_data_2 0x08601c03
    #define_eval gen_data_3 0x0a93010f
    #define_eval gen_data_4 0x08280d03
    #define_eval gen_data_5 0x11325958
    #define_eval gen_data_6 0
    #define_eval gen_data_7 0
    #define_eval _cib_xfer_size_generic_poly1305 8
    #define_eval _cib_code_size_generic_poly1305 4
    #define_eval _cib_vars_base_generic_poly1305 20
    #define_eval _cib_vars_size_generic_poly1305 2
    #define_eval _cib_vars_xfer_generic_poly1305 2
#endm

#define CRYPTO_NFP_MODE 1  // NFP6xxx or NFP38xx mode

#macro crypto_gen_compr_constants (base0, base1)
    #define_eval gen_data_0 0x000b1f42
    #define_eval gen_data_1 0x00000000
    #define_eval gen_data_2 0x01800000
    #define_eval gen_data_2 (gen_data_2 + base0)
    #define_eval gen_data_2 (gen_data_2 + (base1 << 16))
    #define_eval gen_data_3 0x00200f0a
    #define_eval gen_data_3 (gen_data_3 + (base1 << 16))
    #define_eval gen_data_4 0x00000000
    #define_eval gen_data_5 0x00000000
    #define_eval gen_data_6 0x00000ff0
    #define_eval gen_data_7 0
    #define_eval _cib_xfer_size_compr_constants 0x0008
    #define_eval _cib_code_size_compr_constants 0x0007
    #define_eval _cib_vars_size_compr_constants 0
    #define_eval _cib_base_addr_compr_constants 0x0000
#endm

