/**
 * Copyright (C) 2008-2014 Netronome Systems, Inc.  All rights reserved.
 *
 * @file machxo2.h
 *
 */


#ifndef _LATTICE_MACH_XO2_PROG_H_
#define _LATTICE_MACH_XO2_PROG_H_
/******************************************************************************/

#define NIBBLE(word, nib)         (((word) >> (((nib) & 7) * 4)) & 0xf)
#define XO_IDCODE_TO_DEV(id)      ((NIBBLE(id, 7) << 4) | NIBBLE(id, 3))

#define MACHXO_IDCODE_STD_MASK    0x0fff0fff
#define MACHXO_IDCODE_STD_BITS    0x012b0043
#define IS_MACHXO_IDCODE(idcode)  ((idcode & MACHXO_IDCODE_STD_MASK) \
                                   == MACHXO_IDCODE_STD_BITS)

#define XODEV(a, b)           ((((a) & 0xf) << 4) | ((b) & 0xf))

#define MXO2_640              XODEV(0x0, 0x9)
#define MXO2_7000             XODEV(0x0, 0xd)

#define MXO3_1300             XODEV(0xc, 0xb)

#define CONFIG_FLASH          0
#define UFM_FLASH             4

#define IDCODE_PUB            0xe0
#define ISC_ENABLE_X          0x74
#define ISC_ENABLE            0xc6
#define LSC_CHECK_BUSY        0xf0
#define LSC_READ_STATUS       0x3c
#define ISC_ERASE             0x0e
#define LSC_ERASE_TAG         0xcb
#define LSC_INIT_ADDRESS      0x46
#define LSC_WRITE_ADDRESS     0xb4
#define LSC_PROG_INCR_NV      0x70
#define LSC_INIT_ADDR_UFM     0x47
#define LSC_PROG_TAG          0xc9
#define ISC_PROGRAM_USERCODE  0xc2
#define USERCODE              0xc0
#define LSC_PROG_FEATURE      0xe4
#define LSC_READ_FEATURE      0xe7
#define LSC_PROG_FEABITS      0xf8
#define LSC_READ_FEABITS      0xfb
#define LSC_READ_INCR_NV      0x73
#define LSC_READ_UFM          0xca
#define ISC_PROGRAM_DONE      0x5e
#define LSC_PROG_OTP          0xf9
#define LSC_READ_OTP          0xfa
#define ISC_DISABLE           0x26
#define ISC_NOOP              0xff
#define LSC_REFRESH           0x79
#define ISC_PROGRAM_SECURITY  0xce
#define ISC_PROGRAM_SECPLUS   0xcf
#define UIDCODE_PUB           0x19

#define MXO2_JTAG_PROGINTF    0
#define MXO2_I2C_PROGINTF     1
#define MXO2_SPI_PROGINTF     2

u8   feature_row_i2c_slave(u8 *fr);
void decode_feature_bits  (u8 *fb, u32 sz);
void decode_feature_row   (u8 *fr, u32 sz);

struct mxo2_priv;
struct mxo2_ops {
    int (*read_device_id)       (struct mxo2_priv *, u32 *dev_id);
    int (*enable_config_intface)(struct mxo2_priv *,u32 mode);
    int (*read_busy_flag)       (struct mxo2_priv *, u8 *busy);
    int (*read_status_register) (struct mxo2_priv *, u32 *status);
    int (*erase)                (struct mxo2_priv *, u32 targets);
    int (*erase_ufm)            (struct mxo2_priv *);
    int (*reset_configuration_flash_address)(struct mxo2_priv *);
    int (*set_address)          (struct mxo2_priv *, u32 mem_space, u32 page);
    int (*reset_ufm_address)    (struct mxo2_priv *);
    int (*program_page)         (struct mxo2_priv *, u32 mem_space,
                            u8 *data, u32 size);
    int (*program_usercode)     (struct mxo2_priv *, u32 usercode);
    int (*read_usercode)        (struct mxo2_priv *, u32 *usercode);
    int (*write_feature_row)    (struct mxo2_priv *, u8 *data, u32 size);
    int (*read_feature_row)     (struct mxo2_priv *, u8 *data, u32 size);
    int (*write_feabits)        (struct mxo2_priv *, u8 *data, u32 size);
    int (*read_feabits)         (struct mxo2_priv *, u8 *data, u32 size);
    int (*read_flash_page)      (struct mxo2_priv *, u32 mem_space,
                            u8 *data, u32 size);
    int (*program_done)         (struct mxo2_priv *);
    int (*program_otp_fuses)    (struct mxo2_priv *, u8 fuses);
    int (*read_otp_fuses)       (struct mxo2_priv *, u8 *fuses);
    int (*disable_config_intface)(struct mxo2_priv *);
    int (*bypass)               (struct mxo2_priv *);
    int (*refresh)              (struct mxo2_priv *);
    int (*program_security)     (struct mxo2_priv *);
    int (*program_security_plus)(struct mxo2_priv *);
    int (*read_traceid_code)    (struct mxo2_priv *, u8 *data, u32 size);
};

struct mxo2_priv {
    void *data;         /* I2C-devid or JTAG-scanchain index */
    void *intf;         /* interface context */
    int type;           /* interfcae type JTAG/I2C/SPI */
    struct mxo2_ops *ops;
    struct machxo2_prog_intf *parent;
    u32 bus_errs;
    u32 busy_wait_timeout;
};

struct jtag_prog_intf_data {
    u32 discard_first_page; /* discard dummy page after
                    setting of flash address */
    u32 scan_chain_index;
    struct jtag_tap_info *taps;
    u32 tap_count;
    u32 matching_probed_tap_count;
};
struct i2c_prog_intf_data {
    u8 addr;
};
struct spi_prog_intf_data {
};

struct machxo2_prog_intf * machxo2_prog_intf_init(u32 type, void *prog_intf, void *prog_data);
void                       machxo2_prog_intf_free(struct machxo2_prog_intf *pi);

/******************************************************************************/
#endif // _LATTICE_MACH_XO2_PROG_H_

