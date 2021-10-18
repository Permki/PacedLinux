/**
 * Copyright (C) 2012 Netronome Systems, Inc.  All rights reserved.
 *
 * Implementation of [LOCAL] functions from nfp_sal.interface.h
 *
 */
#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#include <nfp.h>
#include <nfp_cpp.h>
#include <nfp_power.h>
#include <nfp_serdes.h>
#include <nfp_hwinfo.h>
#include <nfp_i2c.h>
#include <nfp_gpio.h>
#include <nfp_spi.h>
#include <nfp_fis.h>
#include <nfp_flash.h>

#include <nfp_internal.h>
#include <nfp-common/nfp_chipdata.h>
#include <nfp-common/nfp_platform.h>
#include <nfp-common/ns_log.h>
#include <ns_ipc_buf.h>
#include <ns_ipc_client.h>

#include "nfp_device.h"

#ifdef _MSC_VER    /* This block is specific to MS VC++. */
    #include <Windows.h>
    #include <winerror.h>
    #define EPROTO WSAEPROTOTYPE
    #define SET_ERROR(x) do { SetLastError(x); } while(0)
    #define GET_ERROR() GetLastError()
#else
    #define SET_ERROR(x) do { errno = x; } while(0)
    #define GET_ERROR() (errno)
#endif

struct nfp_device_hwinfo_entry {
    char *key;
    char *value;
};

static void nfp_dev_init(struct nfp_device *dev)
{
    dev->chipdata = NULL;
    dev->chip = NULL;
    dev->hwinfo = NULL;
    dev->mip = NULL;
    dev->rtsymtab = NULL;
    dev->strtab = NULL;
    dev->private_handles = NULL;
    dev->private_handle_cnt = 0;
}

int nfp_device_open_local(struct nfp_device *dev, unsigned int devnum)
{
    nfp_dev_init(dev);
    return 0;
}

int nfp_device_from_cpp_local(struct nfp_device *dev, struct nfp_cpp *cpp)
{
    nfp_dev_init(dev);
    return 0;
}

void nfp_device_close_local(struct nfp_device *dev)
{
    int i;

    if (dev->chip)
        nfp_chipdata_chip_free(dev->chip);

    if (dev->chipdata)
        nfp_chipdata_free(dev->chipdata);

    if (dev->hwinfo) {
        for (i = 0; i < dev->hwinfo_entries; i++) {
            if (dev->hwinfo[i].key)
                free(dev->hwinfo[i].key);
            if (dev->hwinfo[i].value)
                free(dev->hwinfo[i].value);
        }
        free(dev->hwinfo);
        dev->hwinfo = NULL;
    }

    if (dev->rtsymtab) {
        free(dev->rtsymtab);
        dev->rtsymtab = NULL;
    }

    if (dev->strtab) {
        free(dev->strtab);
        dev->strtab = NULL;
    }

    if (dev->mip) {
        free(dev->mip);
        dev->mip = NULL;
    }

    for (i = 0; i < dev->private_handle_cnt; i++) {
        /* the user data is stored after the private handle buffer in the same
         * block */
        if (dev->private_handles[i]->destructor)
            (dev->private_handles[i]->destructor)(dev->private_handles[i] + 1);
        free(dev->private_handles[i]);
    }

    if (dev->private_handles)
        free(dev->private_handles);

    dev->private_handles = NULL;
    dev->private_handle_cnt = 0;

    handle_common_free(&dev->h_common);
}

int nfp_device_number(struct nfp_device *dev)
{
    return dev->id;
}

int nfp_device_number_from_pcie(char *dev)
{
    return -1;
}

struct nfp_device *nfp_device_open_pcie(char *dev)
{
    errno = ENODEV;
    return NULL;
}


/******************* HWINFO ******************/

#define LOG_ERROR(...) NS_LOG(NS_LOG_LVL_ERROR, __VA_ARGS__)

#define BLEN (1024*8)
static void populate_hwinfo(struct nfp_device *dev)
{
    struct nfp_device_hwinfo_entry *hi;
    char *buf = NULL;
    char *s, *k, *v;
    int ret, len, i;

    /* first find out the size of the hwdata buffer we need */
    len = nfp_get_hwinfo(dev, buf, 0);
    if (len <= 0) {
        len = 1024*8; /* if this fails try a fixed size */
    }

    buf = malloc(len);
    if (!buf) {
        LOG_ERROR("failed to allocate hwinfo temp buffer of size %d", len);
        return;
    }

    ret = nfp_get_hwinfo(dev, buf, len);
    /* either no hwinfo or error */
    if (ret == 0 || ret < 0) {
        free(buf);
        return;
    }

    dev->hwinfo_entries = 0;

    s = buf;
    for (i = 0; i < ret; i += 2) {
        k = s;
        s += strlen(k) + 1;
        v = s;
        s += strlen(v) + 1;
        hi = realloc(dev->hwinfo, (dev->hwinfo_entries + 1) * sizeof(*hi));
        if (!hi) {
            LOG_ERROR("failed to allocate memory for hwinfo");
            break;
        }
        hi[dev->hwinfo_entries].key = strdup(k);
        hi[dev->hwinfo_entries].value = strdup(v);
        dev->hwinfo = hi;
        if (!hi[dev->hwinfo_entries].key || !hi[dev->hwinfo_entries].value) {
            LOG_ERROR("failed to allocate memory for hwinfo entries");
            break;
        }
        dev->hwinfo_entries++;
    }
    free(buf);
}

int nfp_hwinfo_entries(struct nfp_device *dev)
{
    if (!dev)
        return 0;

    if (!dev->hwinfo)
        populate_hwinfo(dev);

    if (!dev->hwinfo)
        return 0;

    return dev->hwinfo_entries;
}

int nfp_hwinfo_entry(struct nfp_device *dev,
                     unsigned int idx,
                     const char **key,
                     const char **value)
{
    struct nfp_device_hwinfo_entry *hi;

    if (!dev) {
        SET_ERROR(EINVAL);
        return -1;
    }

    if (!dev->hwinfo)
        populate_hwinfo(dev);

    if (!dev->hwinfo) {
        SET_ERROR(EBADF);
        return -1;
    }

    if (idx >= (unsigned int)dev->hwinfo_entries) {
        SET_ERROR(ENOENT);
        return -1;
    }

    hi = &dev->hwinfo[idx];

    *key = (const char *)hi->key;
    *value = (const char *)hi->value;

    return 0;
}

const char *nfp_hwinfo_lookup(struct nfp_device *dev, const char *key)
{
    int i;

    if (!dev) {
        SET_ERROR(EINVAL);
        return NULL;
    }

    if (!dev->hwinfo)
        populate_hwinfo(dev);

    if (!dev->hwinfo) {
        SET_ERROR(EBADF);
        return NULL;
    }

    for (i = 0; i < dev->hwinfo_entries; i++) {
        if (!strcmp(dev->hwinfo[i].key, key)) /* found */
            return dev->hwinfo[i].value;
    }

    /* not found */
    SET_ERROR(ENOENT);
    return NULL;
}

int nfp_hwinfo_reload(struct nfp_device *dev, const void *hwinfo)
{
    SET_ERROR(ENOTSUP);
    return -1;
}

void *nfp_hwinfo_dup(struct nfp_device *dev)
{
    SET_ERROR(ENOTSUP);
    return NULL;
}

int nfp_hwinfo_alter(struct nfp_device *dev, const char *key, const char *value)
{
    SET_ERROR(ENOTSUP);
    return -1;
}


/************ NFP local stubs ***********/

static void
cpp_private_free(void *private_data)
{
    nfp_cpp_free(*((struct nfp_cpp **)private_data));
}

#include "nfp_cpp_handles.h"
static void *
cpp_private_alloc(struct nfp_device *dev)
{
    struct nfp_cpp *cpp;
    struct nfp_cpp **cpp_p;

    cpp = nfp_cpp_from_device_id(dev->id);
    if (!cpp)
        return NULL;

    cpp_p = nfp_device_private_alloc(dev, sizeof(*cpp_p), &cpp_private_free);
    if (!cpp_p)
        return NULL;

    *cpp_p = cpp;

    return cpp_p;
}

struct nfp_cpp * nfp_device_cpp(struct nfp_device *dev)
{
    struct nfp_cpp **priv;
    struct nfp_cpp *cpp;

    /* we need to register a destructor with the nfp handle,
     * the nfp_cpp handle must be freed along with the nfp handle
     */
    priv = nfp_device_private(dev, &cpp_private_alloc);
    if (!priv)
        return NULL;

    cpp = *priv;

    return cpp;
}

int nfp_err_open(unsigned int devnum, int flags)
{
    fprintf(stderr, "FIXME: nfp_err_open() not implemented\n");
    SET_ERROR(ENOTSUP);
    return -1;
}

/********** NFP private Data ************/

void *nfp_device_private(struct nfp_device *dev,
                         void *(*constructor)(struct nfp_device *dev))
{
    struct private_handle_data *ph;
    void *h;
    int i;

    if (!constructor || !dev) {
        SET_ERROR(EINVAL);
        return NULL;
    }

    for (i = 0; i < dev->private_handle_cnt; i++) {
        if (dev->private_handles[i]->constructor == constructor) {
            return dev->private_handles[i] + 1;
        }
    }

    h = (*constructor)(dev);
    if (!h) {
        SET_ERROR(EFAULT);
        return NULL;
    }

    /* the private data precedes the user data */
    ph = ((struct private_handle_data *) h) - 1;
    ph->constructor = constructor;

    return h;
}

void *nfp_device_private_alloc(struct nfp_device *dev,
                               size_t private_size,
                               void (*destructor)(void *private_data))
{
    struct private_handle_data **hd;
    struct private_handle_data *privatedata;
    void *userdata;
    int cnt;

    privatedata = calloc(1, private_size + sizeof(struct private_handle_data));

    if (!privatedata) {
        SET_ERROR(ENOMEM);
        return NULL;
    }

    cnt = dev->private_handle_cnt;
    hd = realloc(dev->private_handles, sizeof(*hd) * (cnt + 1));
    if (!hd) {
        SET_ERROR(ENOMEM);
        free(privatedata);
        return NULL;
    }
    dev->private_handles = hd;

    privatedata->destructor = destructor;
    dev->private_handles[cnt] = privatedata;
    dev->private_handle_cnt++;

    userdata = (void *)(privatedata + 1);

    return userdata;
}

/************ Advisory locking **************/

/* FIXME : this is all todo */

int nfp_device_trylock(struct nfp_device *dev)
{
    return 0;
}

int nfp_device_lock(struct nfp_device *dev)
{
    return 0;
}

int nfp_device_unlock(struct nfp_device *dev)
{
    return 0;
}

/*************** Chipdata ******************/

/* lifted from bsp nfp_device_chip code */
const struct nfp_chipdata_chip *nfp_device_chip(struct nfp_device *dev)
{
    struct nfp_cpp *cpp;
    char buf[32];
    int family = 0;
    int ret = 0;
    int i, pwr_state;
    int rev = -1;
    struct nfp_chipdata_chip *chip = NULL;
    uint32_t model;

    if (!dev) {
        SET_ERROR(EINVAL);
        return NULL;
    }

    if (dev->chip)
        return dev->chip;

    if (!dev->chipdata)
        dev->chipdata = nfp_chipdata_load(NULL, NULL);

    if (!dev->chipdata) {
        SET_ERROR(EFAULT);
        return NULL;
    }

    cpp = nfp_device_cpp(dev);
    model = nfp_cpp_model(cpp);

    family = NFP_CPP_MODEL_FAMILY_of(model);
    rev = NFP_CPP_MODEL_STEPPING_of(model);

    sprintf(buf, "nfp%u", dev->id);

    switch (family) {
    case NFP_CHIP_FAMILY_NFP3200:
        /* We dont support 3200 for now */
        SET_ERROR(ENOTSUP);
        return NULL;
    case NFP_CHIP_FAMILY_NFP6000:
        /* FIXME: Same as above, we need to populate the list of available
         * islands with real hwinfo instead. For now, just using full
         * NFP-6xxx.
         */
        chip = nfp_chipdata_chip_new(dev->chipdata,
            family, rev, buf, NULL);
        if (!chip)
        {
            SET_ERROR(EFAULT);
            return NULL;
        }

        if (nfp_power_get(dev, NFP6000_DEVICE_ARM(0, NFP6000_DEVICE_ARM_MEG0),
                          &pwr_state) < 0) {
            nfp_chipdata_chip_free(chip);
            return NULL;
        }
        if (pwr_state == NFP_DEVICE_STATE_ON)
            (void)nfp_chipdata_island_add(chip, 1);

        for (i = 0; i < 4; i++) {
            if (nfp_power_get(dev,
                    NFP6000_DEVICE_PCI(i, NFP6000_DEVICE_PCI_MEG0),
                    &pwr_state) < 0) {
                if (GET_ERROR() != ENODEV) {
                    nfp_chipdata_chip_free(chip);
                    return NULL;
                } else
                    break;
            }
            if (pwr_state == NFP_DEVICE_STATE_ON)
                (void)nfp_chipdata_island_add(chip, 4 + i);
        }

        for (i = 0; i < 2; i++) {
            if (nfp_power_get(dev,
                    NFP6000_DEVICE_NBI(i, NFP6000_DEVICE_NBI_CORE),
                    &pwr_state) < 0) {
                if (GET_ERROR() != ENODEV) {
                    nfp_chipdata_chip_free(chip);
                    return NULL;
                } else
                    break;
            }
            if (pwr_state == NFP_DEVICE_STATE_ON)
                (void)nfp_chipdata_island_add(chip, 8 + i);
        }

        for (i = 0; i < 2; i++) {
            if (nfp_power_get(dev,
                    NFP6000_DEVICE_CRP(i, NFP6000_DEVICE_CRP_MEG0),
                    &pwr_state) < 0) {
                if (GET_ERROR() != ENODEV) {
                    nfp_chipdata_chip_free(chip);
                    return NULL;
                } else
                    break;
            }
            if (pwr_state == NFP_DEVICE_STATE_ON)
                (void)nfp_chipdata_island_add(chip, 12 + i);
        }

        for (i = 0; i < 3; i++) {
            if (nfp_power_get(dev,
                    NFP6000_DEVICE_EMU(i, NFP6000_DEVICE_EMU_CORE),
                    &pwr_state) < 0) {
                if (GET_ERROR() != ENODEV) {
                    nfp_chipdata_chip_free(chip);
                    return NULL;
                } else
                    break;
            }
            if (pwr_state == NFP_DEVICE_STATE_ON)
                (void)nfp_chipdata_island_add(chip, 24 + i);
        }

        for (i = 0; i < 2; i++) {
            if (nfp_power_get(dev,
                    NFP6000_DEVICE_IMU(i, NFP6000_DEVICE_IMU_CORE),
                    &pwr_state) < 0) {
                if (GET_ERROR() != ENODEV) {
                    nfp_chipdata_chip_free(chip);
                    return NULL;
                } else
                    break;
            }
            if (pwr_state == NFP_DEVICE_STATE_ON)
                (void)nfp_chipdata_island_add(chip, 28 + i);
        }

        for (i = 0; i < 7; i++) {
            if (nfp_power_get(dev,
                    NFP6000_DEVICE_FPC(i, NFP6000_DEVICE_FPC_MEG0),
                    &pwr_state) < 0) {
                if (GET_ERROR() != ENODEV) {
                    nfp_chipdata_chip_free(chip);
                    return NULL;
                } else
                    break;
            }
            if (pwr_state == NFP_DEVICE_STATE_ON)
                (void)nfp_chipdata_island_add(chip, 32 + i);
        }

        for (i = 0; i < 2; i++) {
            if (nfp_power_get(dev,
                    NFP6000_DEVICE_ILA(i, NFP6000_DEVICE_ILA_MEG0),
                    &pwr_state) < 0) {
                if (GET_ERROR() != ENODEV) {
                    nfp_chipdata_chip_free(chip);
                    return NULL;
                } else
                    break;
            }
            if (pwr_state == NFP_DEVICE_STATE_ON)
                (void)nfp_chipdata_island_add(chip, 48 + i);
        }

        /* Can still set serdes_cnt here */
        break;
    default:
        SET_ERROR(ENODEV);
        return NULL;
    }

    /* Because we inherited above, we need to first clear all revisions
     * before setting the real one.
     */
    while (nfp_chipdata_revision_max(chip) != -1)
    {
        ret = nfp_chipdata_revision_rem(chip,
            nfp_chipdata_revision_max(chip));
        if (ret != 0)
        {
            nfp_chipdata_chip_free(chip);
            SET_ERROR(EFAULT);
            return NULL;
        }
    }

    if (nfp_chipdata_revision_add(chip, rev) != 0)
    {
        nfp_chipdata_chip_free(chip);
        SET_ERROR(EFAULT);
        return NULL;
    }

    dev->chip = chip;
    return dev->chip;
}

/* Unimplemented stubs for i2c, spi fis, and flash */

/* GPIO */

int nfp_gpio_pins(struct nfp_device *dev)
{
    errno = ENOSYS;
    return -1;
}

int nfp_gpio_direction(struct nfp_device *dev, int gpio_pin, int is_output)
{
    errno = ENOSYS;
    return -1;
}

int nfp_gpio_get_direction(struct nfp_device *dev, int gpio_pin, int *is_output)
{
    errno = ENOSYS;
    return -1;
}

int nfp_gpio_get(struct nfp_device *dev, int gpio_pin)
{
    errno = ENOSYS;
    return -1;
}

int nfp_gpio_set(struct nfp_device *dev, int gpio_pin, int value)
{
    errno = ENOSYS;
    return -1;
}

int nfp_gpio_mapped(struct nfp_device *dev)
{
    errno = ENOSYS;
    return -1;
}

/* i2c */

struct nfp_i2c *nfp_i2c_alloc(struct nfp_device *dev,
                  int gpio_scl, int gpio_sda)
{
    errno = ENOSYS;
    return NULL;
}

void nfp_i2c_free(struct nfp_i2c *bus)
{
    return;
}

int nfp_i2c_set_speed(struct nfp_i2c *bus, unsigned int speed_hz)
{
    errno = ENOSYS;
    return -1;
}

int nfp_i2c_set_timeout(struct nfp_i2c *bus, long timeout_ms)
{
    errno = ENOSYS;
    return -1;
}

int nfp_i2c_cmd(struct nfp_i2c *bus, int i2c_dev,
                const void *w_buff, size_t w_len,
                void *r_buff, size_t r_len, uint32_t nostop)
{
    errno = ENOSYS;
    return -1;
}

int nfp_i2c_read(struct nfp_i2c *bus, int i2c_dev,
                 uint32_t addr, size_t a_len,
                 void *r_buff, size_t r_len)
{
    errno = ENOSYS;
    return -1;
}

int nfp_i2c_write(struct nfp_i2c *bus, int i2c_dev,
                  uint32_t address, size_t a_len,
                  const void *w_buff, size_t w_len)
{
    errno = ENOSYS;
    return -1;
}

/* spi */

struct nfp_spi *nfp_spi_acquire(struct nfp_device *nfp, int bus)
{
    errno = ENOSYS;
    return NULL;
}

void nfp_spi_release(struct nfp_spi *spi)
{
    return;
}

int nfp_spi_speed_set(struct nfp_spi *spi, unsigned int hz)
{
    errno = ENOSYS;
    return -1;
}

int nfp_spi_speed_get(struct nfp_spi *spi, unsigned int *hz)
{
    errno = ENOSYS;
    return -1;
}

int nfp_spi_mode_set(struct nfp_spi *spi, int mode)
{
    errno = ENOSYS;
    return -1;
}

int nfp_spi_mode_get(struct nfp_spi *spi, int *mode)
{
    errno = ENOSYS;
    return -1;
}

int nfp6000_spi_transact(struct nfp_spi *spi, int cs, int cs_action,
                            const void *tx, uint32_t tx_bit_cnt,
                                  void *rx, uint32_t rx_bit_cnt,
                            int mdio_data_drive_disable)
{
    errno = ENOSYS;
    return -1;
}

int nfp_spi_read(struct nfp_spi *spi, int cs,
                    unsigned int cmd_len, const void *cmd,
                    unsigned int res_len, void *res)
{
    errno = ENOSYS;
    return -1;
}

int nfp_spi_write(struct nfp_spi *spi, int cs,
                    unsigned int cmd_len, const void *cmd,
                    unsigned int dat_len, const void *dat)
{
    errno = ENOSYS;
    return -1;
}

/* fis */

struct nfp_fis *nfp_fis_open(struct nfp_flash *flash, unsigned int nfp_fis_block)
{
    errno = ENOSYS;
    return NULL;
}

void nfp_fis_close(struct nfp_fis *nfp_fis, int commit)
{
    return;
}

const char *nfp_fis_strerror(struct nfp_fis *nfp_fis)
{
    errno = ENOSYS;
    return NULL;
}

int nfp_fis_init(struct nfp_fis *nfp_fis, unsigned int nfp_fis_block, uint32_t flash_base, int secure)
{
    errno = ENOSYS;
    return -1;
}

int nfp_fis_flash_size(struct nfp_fis *nfp_fis)
{
    errno = ENOSYS;
    return -1;
}

int nfp_fis_block_size(struct nfp_fis *nfp_fis)
{
    errno = ENOSYS;
    return -1;
}


const struct nfp_fis_entry *nfp_fis_next(struct nfp_fis *nfp_fis, const struct nfp_fis_entry *prev)
{
    errno = ENOSYS;
    return NULL;
}


const struct nfp_fis_entry *nfp_fis_find(struct nfp_fis *nfp_fis, const char *name)
{
    errno = ENOSYS;
    return NULL;
}


int nfp_fis_create(struct nfp_fis *nfp_fis, struct nfp_fis_entry *de, void *image)
{
    errno = ENOSYS;
    return -1;
}


int nfp_fis_delete(struct nfp_fis *nfp_fis, const char *name, int secure)
{
    errno = ENOSYS;
    return -1;
}


int nfp_fis_erase(struct nfp_fis *nfp_fis, const char *name)
{
    errno = ENOSYS;
    return -1;
}


int nfp_fis_read(struct nfp_fis *nfp_fis, const char *name, void *buffer, size_t size)
{
    errno = ENOSYS;
    return -1;
}


int nfp_fis_resize(struct nfp_fis *nfp_fis, const char *name, int flash_blocks)
{
    errno = ENOSYS;
    return -1;
}


int nfp_fis_rename(struct nfp_fis *nfp_fis, const char *name, const char *new_name)
{
    errno = ENOSYS;
    return -1;
}

void nfp_fis_callback_progress(struct nfp_fis *nfp_fis, nfp_fis_progress_t func, void *priv)
{
    return;
}

/* Flash */

struct nfp_flash *nfp_flash_open_spi(struct nfp_device *dev, int spi_bus, int spi_cs)
{
    errno = ENOSYS;
    return NULL;
}


struct nfp_flash *nfp_flash_open_file(const char *file, size_t block_size)
{
    errno = ENOSYS;
    return NULL;
}


void nfp_flash_close(struct nfp_flash *flash)
{
    errno = ENOSYS;
    return;
}


int nfp_flash_info(struct nfp_flash *flash,
                   struct nfp_flash_info *info)
{
    errno = ENOSYS;
    return -1;
}


int nfp_flash_erase(struct nfp_flash *flash,
                    unsigned int offset, unsigned int len)
{
    errno = ENOSYS;
    return -1;
}


int nfp_flash_program(struct nfp_flash *flash,
                      unsigned int offset, const void *buff, size_t len)
{
    errno = ENOSYS;
    return -1;
}


int nfp_flash_read(struct nfp_flash *flash,
                   unsigned int offset, void *buff, size_t len)
{
    errno = ENOSYS;
    return -1;
}

void nfp_flash_callback_progress(struct nfp_flash *flash, nfp_flash_progress_t func, void *priv)
{
    errno = ENOSYS;
    return;
}

