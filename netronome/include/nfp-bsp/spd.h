/*
 * Copyright (C) 2009-2010, Netronome Systems, Inc.  All rights reserved.
 *
 */

#ifndef SPD_H
#define SPD_H

#include <stdint.h>

typedef union spd_s {
    uint8_t byte[256];
    struct {
        uint8_t info_size;   /*  0 # bytes written into serial memory */
        uint8_t chip_size;   /*  1 Total # bytes of SPD memory device */
        uint8_t mem_type;    /*  2 Fundamental memory type */
    } ddr;
} spd_t;

struct spd_info_ddr3 {
    /* Flags */
    unsigned int is_registered:1;   /* Set if B3[3:0] == 1 or 5*/
    unsigned int is_nonstandard:1;  /* Set if B33[7] == 1 */
    unsigned int can_VDD_1_2:1; /* B6[2] */
    unsigned int can_VDD_1_35:1;    /* B6[1] */
    unsigned int can_VDD_1_5:1; /* B6[0] == 0 */
    unsigned int can_DLL_off:1; /* B30[7] */
    unsigned int can_RZQ_7:1;   /* MR drive strength options */
    unsigned int can_RZQ_6:1;   /* MR drive strength options */
    unsigned int can_PASR:1;    /* Partial Array Self Refresh */
    unsigned int can_ODTS:1;    /* On-Die Thermal Sensor */
    unsigned int can_ASR:1;     /* Auto Self-Refresh */
    unsigned int needs_2X_refresh_above_85C:1;  /* Self Explanatory */
    unsigned int is_usable_above_85C:1; /* "" "" "" */
    unsigned int has_thermal_sensor:1;  /* "" "" "" */


    uint32_t total_mbits;
    uint32_t total_mbytes;

    /* Logical Dimensions */
    uint8_t ranks;
    uint8_t banks;
    uint8_t row_bits;
    uint8_t col_bits;
    uint8_t bus_bits;   /* Primary bus */
    uint8_t ext_bits;   /* Extension bus (ie ECC) */
    uint8_t sdram_bits; /* Data width */

    /* Timing (valid before solution) */
    uint8_t FTB_mul;    /* Fine Timebase = 1picosec * mul / div */
    uint8_t FTB_div;

    uint8_t MTB_mul;    /* Med. Timebase = 1ns * mul / div */
    uint8_t MTB_div;

    int tCK_ps_min;     /* Minimum tCK in picosecs */
    uint16_t tCL_mask;  /* Mask of supported CAS latencies,
                 * from 4 (at bit 0) to 18 (at bit 14) 
                 */
    /* Timing (valid after solution) */
    int tCK_mhz;        /* Solution clock, in Mhz */
    int tCK_ps;     /* Solution clock, in ps */
#define tAA tCL

    /* Timing in 'ps_min' is valid before solution,
     * and in 'clocks' after solution.
     */
    struct {
        int tCL;        /* CAS latency delay */
        int tWR;        /* Write Recovery delay */
        int tRCD;       /* RAS-to-CAS delay */
        int tRRD;       /* Row-to-Row delay */
        int tRP;        /* Row Precharge delay */
        int tRAS;       /* Row Active to Precharge delay */
        int tRC;        /* Row Active to Active/Refresh delay */
        int tRFC;       /* Row Refresh Recovery delay */
        int tWTR;       /* Internal Write to Read Command delay */
        int tRTP;       /* Internal Read to Precharge Command delay */
        int tFAW;       /* Four Active Window delay */
    } ps_min, clocks;

    union {
        struct {
            /* Flags */
            unsigned int is_rank_1_mirrored:1;  /* B63[1] == 1 */
        } udimm;
        struct {
            int rows;   /* B63[3:2] */
            int registers;  /* B63[1:0] */
        } rdimm;
    };

    char module_part[19];   /* B128 ... B145 */
    uint8_t module_revision[2];
    uint8_t mfg_id_bank;    /* B148: JEP-106 Bank */
    uint8_t mfg_id_code;    /* B149: JEP-106 Code */
};

enum {
    SPD_FPM_DRAM = 1,
    SPD_EDO = 2,
    SPD_PIPELINED_NIBBLE = 3,
    SPD_SDRAM = 4,
    SPD_ROM = 5,
    SPD_DDR_SGRAM = 6,
    SPD_DDR_SDRAM = 7,
    SPD_DDR2_SDRAM = 8,
    SPD_DDR2_SDRAM_FB_DIMM = 9,
    SPD_DDR2_SDRAM_FB_DIMM_PROBE = 10,
    SPD_DDR3_SDRAM = 11,
};

struct spd_info {
    uint8_t crc16_len;      /* 117 or 126, based on B0[7] */
    int spd_len;        /* Size of SPD, from B0[6:4] */
    int spd_used;       /* 128, 176, or 256. from B0[3:0] */
    uint8_t spd_encoding;   /* Should be 1. From B1[7:4] */
    uint8_t spd_additions;  /* From B1[3:0] */
    uint8_t spd_type;   /* B[2], see above table */

    union {
        struct spd_info_ddr3 ddr3;
    };
};

/* spd_solve_* error codes */
#define SPD_ERR_NO_CLOCK    (-1)
#define SPD_ERR_NO_T_CL     (-2)
#define SPD_ERR_NO_T_CL_LT_20NS (-3)

/* Given a pointer to a proposed clock,
 * and an array of DDR3 SPDs, solve
 * for CL.
 *
 * Returns common clock, in mhz.
 */
int spd_solve_ddr3(struct spd_info *info, int spds, int ddr_speed);

/* spd_decode error codes */
#define SPD_ERR_CRC     (-11)   /* Bad CRC16 */
#define SPD_ERR_LEN     (-12)   /* Unknown length encoding */
#define SPD_ERR_USED_LEN    (-13)   /* Unknown used length encoding */
#define SPD_ERR_ENCODING    (-14)   /* Unknown SPD major encoding version */
#define SPD_ERR_TYPE        (-15)   /* Unknown DIMM type */
#define SPD_ERR_NOT_PRESENT (-16)   /* SPD not present */

int spd_decode(struct spd_info *info, const spd_t *spd);
int spd_encode(spd_t *spd, const struct spd_info *info);

#endif /* SPD_H */

/* vim: set tabstop=4 softtabstop=4 shiftwidth=4 expandtab: */
