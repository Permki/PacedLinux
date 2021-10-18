/**
 * Copyright (C) 2008-2014 Netronome Systems, Inc.  All rights reserved.
 *
 * @file machxo2_prog_data.h
 *
 */


#ifndef __MACHXO2_PROG_DATA_H__
#define __MACHXO2_PROG_DATA_H__
/******************************************************************************/

#define MACH_XO2_DEFAULT_I2C_CFG_ADDR    0x40

/* page size in bytes */
#define MACH_XO2_PAGE_SZ          16
/* feature row size in bytes */
#define MACH_XO2_FEATURE_SZ        8
/* feature bits size in bytes */
#define MACH_XO2_FEABITS_SZ        2
/* Trace ID size in bytes */
#define MACH_XO2_TRACEID_SZ        8

struct jedec_note {
    char *note;
    struct dl_list list;
};

struct machxo2_page_data {
    u32 page_start;
    u32 page_count;
    u8 *data;
    struct dl_list list;
};

/* Programming flags */
#define CHECK_ID               (1 << 0)
#define OFFLINE_CFG            (1 << 1)
#define PROGRAM_CFG            (1 << 2)
#define PROGRAM_UFM            (1 << 3)
#define PROGRAM_USERCODE       (1 << 4)
#define VERIFY_USERCODE        (1 << 5)
#define VERIFY_CFG_FLASH       (1 << 6)
#define VERIFY_UFM             (1 << 7)
#define WRITE_FEAT_ROW         (1 << 8)
#define WRITE_FEAT_BITS        (1 << 9)
#define WRITE_SEC_BIT          (1 << 10)
#define WRITE_SECPLUS_BIT      (1 << 11)
#define PROGRAM_OTP_FUSES      (1 << 12)
#define VERIFY_OTP_FUSES       (1 << 13)
#define REFRESH_CFG            (1 << 14)

/* Parser flags */
#define HAVE_IDCODE            (1 << 0)
#define HAVE_USERCODE          (1 << 1)
#define HAVE_FEAT_ROW          (1 << 2)
#define HAVE_FEAT_BITS         (1 << 3)
#define HAVE_CFG_DATA          (1 << 4)
#define HAVE_UFM_DATA          (1 << 5)
#define HAVE_FUSE_CHECKSUM     (1 << 6)
#define HAVE_TRANS_CHECKSUM    (1 << 7)

struct machxo2_prog_data {

    u32 prog_flags;
    u32 parse_flags;

    u32 fuse_count;
    u32 pin_count;
    u32 test_vector_count;
    u16 fuse_checksum;
    u16 transmission_checksum;

    u32 security_otp;
    u32 default_fuse;

    u32 dev_id;
    u32 usercode;

    u8 feature_row[8];
    u8 feature_bits[2];
    u8 otp_fuses;

    /* struct machxo2_page_data  page data lists */
    struct dl_list cfg_page_data_list;
    struct dl_list ufm_page_data_list;

    /* notes */
    struct dl_list notes;
    char *pretext;
};

/******************************************************************************/
#endif // __MACHXO2_PROG_DATA_H__
