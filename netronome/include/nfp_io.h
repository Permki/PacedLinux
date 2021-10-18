/*
 * Copyright (C) 2015 Netronome Systems, Inc. All rights reserved.
 * Author: Jason McMullan <jason.mcmullan@netronome.com>
 *
 */

#ifndef __NFP_IO_H__
#define __NFP_IO_H__

struct nfp_io_pin;

struct nfp_io_pin_ops {
	void *(*open)(struct nfp_device *nfp, const char *storage);
	void (*close)(void *pin);

	int (*get_value)(void *pin, int *value);
	int (*set_value)(void *pin, int value);

	int (*set_direction)(void *pin, int is_output);
};

struct nfp_io_bus;

struct nfp_io_bus_ops {
	void *(*open)(struct nfp_device *nfp, const char *storage);
	void (*close)(void *bus);

	int (*select)(void *bus, int is_selected);
	int (*set_speed)(void *bus, uint32_t freq_hz);

	int (*read8)(void *bus, uint32_t reg, uint8_t *val);
	int (*write8)(void *bus, uint32_t reg, uint8_t val);

	int (*read16)(void *bus, uint32_t reg, uint16_t *val);
	int (*write16)(void *bus, uint32_t reg, uint16_t val);
};

/**
 * Register an external IO Pin interface type
 *
 * @return      0 on success, -1 and errno on failure
 */
int nfp_io_pin_interface_register(struct nfp_device *dev,
				  const char *prefix,
				  const struct nfp_io_pin_ops *ops);

/**
 * Get a pin from a pin storage identifier
 *
 * @return      nfp_pin identifier
 */
struct nfp_io_pin *nfp_io_pin_open(struct nfp_device *dev, const char *storage);

void nfp_io_pin_close(struct nfp_io_pin *pin);

int nfp_io_pin_set_direction(struct nfp_io_pin *pin, int is_output);

int nfp_io_pin_set_active(struct nfp_io_pin *pin, int is_active);
int nfp_io_pin_get_active(struct nfp_io_pin *pin, int *is_active);

/**
 * Register a PHY Bus interface type
 *
 * @return      0 on success, -1 and errno on failure
 */
int nfp_io_bus_interface_register(struct nfp_device *dev,
				  const char *prefix,
				  const struct nfp_io_bus_ops *ops);

struct nfp_io_bus *nfp_io_bus_open(struct nfp_device *dev, const char *storage);

void nfp_io_bus_close(struct nfp_io_bus *bus);

int nfp_io_bus_select(struct nfp_io_bus *bus, int is_selected);

int nfp_io_bus_set_speed(struct nfp_io_bus *bus, uint32_t hz);

int nfp_io_bus_read8(struct nfp_io_bus *bus, uint32_t reg, uint8_t *val);
int nfp_io_bus_write8(struct nfp_io_bus *bus, uint32_t reg, uint8_t val);

int nfp_io_bus_read16(struct nfp_io_bus *bus, uint32_t reg, uint16_t *val);
int nfp_io_bus_write16(struct nfp_io_bus *bus, uint32_t reg, uint16_t val);

struct ratelimitedread_reg {
	uint32_t reg;
	uint32_t val;
	uint64_t period_ms;
	uint64_t last_ms;
};

struct ratelimitedread_reglist {
	struct ratelimitedread_reg *regs;
	int cnt;
};

struct writecached_reg {
	uint32_t reg;
	uint32_t val;
	int initialized;
};

struct writecached_reglist {
	struct writecached_reg *regs;
	int cnt;
};

/*
 * use with caution, these APIs do no coherency enforcing among multiple
 * threads or processes.
 */
int ratelimitedread_reg_due(struct ratelimitedread_reg *r);
int ratelimitedread_reg_add(struct ratelimitedread_reglist *rl, uint32_t reg,
			    uint64_t period);
struct ratelimitedread_reg *ratelimitedread_reg_lookup(
			      struct ratelimitedread_reglist *rl, uint32_t reg);

int writecached_reg_add(struct writecached_reglist *rl, uint32_t reg);
int writecached_reg_set(struct writecached_reg *r, uint32_t val);
struct writecached_reg *writecached_reg_lookup(struct writecached_reglist *rl,
					       uint32_t reg);

#endif /* __NFP_IO_H__ */
