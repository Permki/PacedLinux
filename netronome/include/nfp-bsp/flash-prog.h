#ifndef __FLASHPROG__
#define __FLASHPROG__
/******************************************************************************/

#define STATUS_BEGIN_WAIT   0
#define STATUS_WAIT         1
#define STATUS_END_WAIT     2
#define STATUS_BEGIN_ERASE  3
#define STATUS_ERASE        4
#define STATUS_END_ERASE    5
#define STATUS_BEGIN_READ   6
#define STATUS_READ         7
#define STATUS_END_READ     8
#define STATUS_BEGIN_PROG   9
#define STATUS_PROG         10
#define STATUS_END_PROG     11
#define STATUS_LOCKBITS     12

struct chip_info {
    char *desc;
    unsigned int id;
    int sector_sz;
    int sectors;
    int page_sz;
};

struct spi_flash {
    int cs;
    struct spi_intf *si;
    u8 id[4];
    struct chip_info *ci;

    void (*status_cb)(int status, int min, int max, int cur,
                        void *user_data); 
    void *user_data;
};

int spi_flash_init(struct spi_flash *sf, u32 id, int cs,
                            struct spi_intf *si,
                            void (*init_cb)(struct chip_info *, void *),
                            void (*status_cb)(int, int, int, int, void *), 
                            void *user_data);

int flash_read    (struct spi_flash *sf, unsigned int addr,
                                                    void* data, size_t len);
int flash_erase   (struct spi_flash *sf, unsigned int addr, size_t len);
int flash_program (struct spi_flash *sf, unsigned int addr,
                                            const void *data, size_t tx_len);
int flash_get_lockbits (struct spi_flash *sf);

/******************************************************************************/
#endif /* __FLASHPROG__ */
