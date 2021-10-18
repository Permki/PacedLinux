/*
 * Copyright (C) 2014, Netronome Systems, Inc.
 * All right reserved.
 *
 */

#ifndef NFP_I2C_H
#define NFP_I2C_H

/* API is part of libnfp */
#include "_nfp_apiex.h"

struct nfp_i2c;

/**
 * NFP I2C Bus creation
 *
 * @param dev          NFP device
 * @param gpio_scl     GPIO pin to use for SCL
 * @param gpio_sda     GPIO pin to use for SDA
 *
 * @return struct nfp_i2c on success, NULL on error (and sets errno accordingly)
 */
NFP_API
struct nfp_i2c *nfp_i2c_alloc(struct nfp_device *dev, int gpio_scl, int gpio_sda);

/**
 * NFP I2C Bus creation
 *
 * @param dev          NFP device
 * @param gpio_scl     GPIO pin to use for SCL
 * @param gpio_sda     GPIO pin to use for SDA
 *
 * @return struct nfp_i2c on success, NULL on error (and sets errno accordingly)
 */
NFP_API
struct nfp_i2c *nfp_i2c_alloc_bus(struct nfp_device *dev, int bus);

/**
 * Release a NFP I2C bus, and free its memory
 *
 * @param bus		I2C Bus
 *
 * As a side effect, the GPIO pins used for SCL and SDA will
 * be set to the 'input' direction when this call returns.
 */
NFP_API
void nfp_i2c_free(struct nfp_i2c *bus);

/**
 * NFP I2C clock rate
 *
 * @param bus		I2C Bus
 * @param speed_hz     Speed in HZ. Use 0 for the default (100Khz)
 *
 * @return 0 on success, or -1 on error (and set errno accordingly).
 */
NFP_API
int nfp_i2c_set_speed(struct nfp_i2c *bus, unsigned int speed_hz);

/**
 * NFP I2C Timeout setup
 *
 * @param bus		I2C Bus
 * @param timeout_ms	Timeout in milliseconds, -1 for forever
 *
 * @return 0 on success, or -1 on error (and set errno accordingly).
 */
NFP_API
int nfp_i2c_set_timeout(struct nfp_i2c *bus, long timeout_ms);

/**
 * NFP I2C Command
 *
 * @param bus		I2C Bus
 * @param i2c_dev	I2C Device ( 7-bit address )
 * @param w_buff	Data to write to device
 * @param w_len		Length in bytes to write (must be >= 1)
 * @param r_buff	Data to read from device (can be NULL if r_len == 0)
 * @param r_len		Length in bytes to read (must be >= 0)
 *
 * @return 0 on success, or -1 on error (and set errno accordingly).
 */
NFP_API
int nfp_i2c_cmd(struct nfp_i2c *bus,
		int i2c_dev,
		const void *w_buff,
		size_t w_len,
		void *r_buff,
		size_t r_len,
		uint32_t nostop);

/**
 * NFP I2C Read
 *
 * @param bus		I2C Bus
 * @param i2c_dev	I2C Device ( 7-bit address )
 * @param addr		Device address
 * @param a_len		Length in bytes of address
 * @param r_buff	Data to read from device (can be NULL if r_len == 0)
 * @param r_len		Length in bytes to read (must be >= 0)
 *
 * @return 0 on success, or -1 on error (and set errno accordingly).
 */
NFP_API
int nfp_i2c_read(struct nfp_i2c *bus,
		 int i2c_dev,
		 uint32_t addr,
		 size_t a_len,
		 void *r_buff,
		 size_t r_len);

/**
 * NFP I2C Write
 *
 * @param bus		I2C Bus
 * @param i2c_dev	I2C Device ( 7-bit address )
 * @param address	Device address
 * @param a_len		Length in bytes of address
 * @param w_buff	Data to write to device
 * @param w_len		Length in bytes to write (must be >= 1)
 *
 * @return 0 on success, or -1 on error (and set errno accordingly).
 */
NFP_API
int nfp_i2c_write(struct nfp_i2c *bus,
		  int i2c_dev,
		  uint32_t address,
		  size_t a_len,
		  const void *w_buff,
		  size_t w_len);

NFP_API
int nfp_i2c_cpld_read8(struct nfp_device *nfp, int bus, uint8_t addr, uint8_t reg, uint8_t *val);
NFP_API
int nfp_i2c_cpld_write8(struct nfp_device *nfp, int bus, uint8_t addr, uint8_t reg, uint8_t val);
NFP_API
int nfp_i2c_cpld_read16(struct nfp_device *nfp, int bus, uint8_t addr, uint8_t reg, uint16_t *val);
NFP_API
int nfp_i2c_cpld_write16(struct nfp_device *nfp, int bus, uint8_t addr, uint8_t reg, uint16_t val);

#endif /* NFP_I2C_H */
