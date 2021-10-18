#ifndef __PIF_METER_H__
#define __PIF_METER_H__

#define PIF_METER_COLOUR_GREEN  0
#define PIF_METER_COLOUR_YELLOW 1
#define PIF_METER_COLOUR_RED    2

/* rfc2698 metering for PIF */
#define PIF_METER_CFGSHIFT_of(up_high) (((up_high) >> 24) & 0x1f)
#define PIF_METER_SHIFT_of(config) (((config) >> 0) & 0x1f)
#define PIF_METER_UPTIME_of(up_high) (((up_high)) & 0xffffff)
#define PIF_METER_UPWORD(time, shift) ((time) | ((shift) << 24))
struct pif_meter_data {
    uint32_t bucket_P; /* P value used by atomic engine - must be 16B aligned */
    uint32_t bucket_C; /* C value used by atomic engine */
    union {
        struct last_update_high_enc {
            unsigned int locked:1;
            unsigned int reserved:2;
            unsigned int cfg_shift:5;
            unsigned int last_update_upper:24;
        };
        uint32_t last_update_high_shft;  /* last ME timestamp upper and shift */
    };
    uint32_t last_update_low;
    /* all below configured by host */
    uint32_t PBS;
    uint32_t PIR;
    uint32_t config;
    uint32_t reserved_word;
};

struct pif_meter_data_wr { /* the bits that we update */
    uint32_t bucket_P;
    uint32_t bucket_C;
    uint32_t last_update_high_shft;
    uint32_t last_update_low;
};

/* tick amount is either 1 for pkt meter or bytes of the packet */
int pif_meter_execute(__mem __addr40 struct pif_meter_data *meter,
                      int colour_in,
                      int tick_amount);

#endif /* __PIF_METER_H__ */
