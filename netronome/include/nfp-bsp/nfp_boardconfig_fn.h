/*
 * Copyright (C) 2009 Netronome Systems, Inc.  All rights reserved.
 *
 * Convenience functions for boardconfig programs
 */

#ifndef NFP_BOARDCONFIG_FN_H
#define NFP_BOARDCONFIG_FN_H

#include <errno.h>

#ifndef NULL
#define NULL ((void *)0L)
#endif

#ifndef NFP_BOARDCONFIG_FN
#error You must define NFP_BOARDCONFIG_FN to point to a struct nfp_boardconfig *
#endif

#define BCFG_VERSION_OF(sec, fn) \
	((int)(offsetof(struct nfp_boardconfig_##sec##_func, fn) / sizeof(void (*)(void))))

#define BCFG_CHECK_RET(sec, fn, ret)                                                         \
	do {                                                                                 \
		if (NFP_BOARDCONFIG_FN->sec.version < BCFG_VERSION_OF(sec, fn)) {          \
			printf("BCFG %s.%d required\n", #sec, BCFG_VERSION_OF(sec, fn)); \
			return ret;                                                          \
		}                                                                            \
	} while (0)

#define BCFG_CALL_RET(sec, fn, ret)                     \
		BCFG_CHECK_RET(sec, fn, ret);           \
		return NFP_BOARDCONFIG_FN->sec.func.fn

#define BCFG_CALL_VOID(sec, fn)    \
	BCFG_CHECK_RET(sec, fn, ); \
	NFP_BOARDCONFIG_FN->sec.func.fn

/* System functions */
static inline void bcfg_console_putc(char c)
{
	BCFG_CALL_VOID(system, console_putc)(c);
}

static inline void bcfg_console_puts(const char *s)
{
	if (!s)
		s = "(null)";
	while (*s)
		bcfg_console_putc(*(s++));
}

static inline int bcfg_console_getc(void)
{
	BCFG_CALL_RET(system, console_getc, -ENOTSUP)();
}

static inline int bcfg_console_relocate(void *addr, unsigned int size)
{
	BCFG_CALL_RET(system, console_relocate, -ENOTSUP)(addr, size);
}

static inline void bcfg_timeout_set(unsigned int usec)
{
	BCFG_CALL_VOID(system, timeout_set)(usec);
}

static inline int bcfg_timeout_tripped(void)
{
	BCFG_CALL_RET(system, timeout_tripped, 1)();
}

static inline int bcfg_modem_recv(int protocol, const struct modem_op *op)
{
	BCFG_CALL_RET(system, modem_recv, -ENOTSUP)(protocol, op);
}

static inline const struct fis_dirent *bcfg_fis_next(int *index)
{
	BCFG_CALL_RET(system, fis_next, NULL)(index);
}

static inline int bcfg_fis_load(const struct fis_dirent *de)
{
	BCFG_CALL_RET(system, fis_load, -ENOTSUP)(de);
}

static inline int bcfg_flash_read(int bus, int cs, unsigned int offset, void *buff, unsigned int len)
{
	BCFG_CALL_RET(system, flash_read, -ENOTSUP)(bus, cs, offset, buff, len);
}

static inline uint32_t bcfg_pll_rate(int pll_id)
{
	BCFG_CALL_RET(system, pll_rate, 0)(pll_id);
}

static inline uint32_t bcfg_crc32_posix(uint32_t crc, const void *buff, size_t len)
{
	BCFG_CALL_RET(system, crc32_posix, ~0)(crc, buff, len);
}

static inline uint32_t bcfg_crc32_gb(uint32_t crc, const void *buff, size_t len)
{
	BCFG_CALL_RET(system, crc32_gb, ~0)(crc, buff, len);
}

static inline int bcfg_hwinfo_set(const char *key, const char *value)
{
	BCFG_CALL_RET(hwinfo, hwinfo_set, -ENOTSUP)(key, value);
}

static inline const char *bcfg_hwinfo_get(const char *key)
{
	BCFG_CALL_RET(hwinfo, hwinfo_get, NULL)(key);
}

static inline const char *bcfg_hwinfo_key_next(void **ptr)
{
	BCFG_CALL_RET(hwinfo, hwinfo_key_next, NULL)(ptr);
}

static inline int bcfg_hwinfo_load(void *db)
{
	BCFG_CALL_RET(hwinfo, hwinfo_load, -ENOTSUP)(db);
}

static inline int bcfg_hwinfo_load_at(void *db, const char *prefix)
{
	BCFG_CALL_RET(hwinfo, hwinfo_load_at, -ENOTSUP)(db, prefix);
}

static inline int bcfg_hwinfo_relocate(void *addr, unsigned int size)
{
	BCFG_CALL_RET(hwinfo, hwinfo_relocate, -ENOTSUP)(addr, size);
}

static inline int bcfg_xpb_readl(uint32_t xpb_addr, uint32_t *val)
{
	BCFG_CALL_RET(cpp, xpb_readl, -ENOTSUP)(xpb_addr, val);
}

static inline int bcfg_xpb_writel(uint32_t xpb_addr, uint32_t value)
{
	BCFG_CALL_RET(cpp, xpb_writel, -ENOTSUP)(xpb_addr, value);
}

static inline int bcfg_cpp_read(uint32_t cpp_id, uint64_t addr, void *buff, size_t len)
{
	BCFG_CALL_RET(cpp, cpp_read, -ENOTSUP)(cpp_id, addr, buff, len);
}

static inline int bcfg_cpp_write(uint32_t cpp_id, uint64_t addr, const void *buff, size_t len)
{
	BCFG_CALL_RET(cpp, cpp_write, -ENOTSUP)(cpp_id, addr, buff, len);
}

static inline int bcfg_cpp_readl(uint32_t source, uint64_t address, uint32_t *val)
{
	return bcfg_cpp_read(source, address, val, sizeof(*val));
}

static inline int bcfg_cpp_writel(uint32_t source, uint64_t address, uint32_t val)
{
	return bcfg_cpp_write(source, address, &val, sizeof(val));
}

static inline uint32_t bcfg_cpp_model(void)
{
	BCFG_CALL_RET(cpp, cpp_model, 0)();
}

static inline struct nfp_cpp_explicit *bcfg_explicit_acquire(void)
{
	BCFG_CALL_RET(cpp, explicit_acquire, NULL)();
}

static inline int bcfg_explicit_set_target(struct nfp_cpp_explicit *expl,
					   uint32_t cpp_id,
					   uint8_t len,
					   uint8_t mask)
{
	BCFG_CALL_RET(cpp, explicit_set_target, -ENOTSUP)(expl, cpp_id, len, mask);
}

static inline int bcfg_explicit_set_data(struct nfp_cpp_explicit *expl,
					 uint8_t data_master,
					 uint16_t data_ref)
{
	BCFG_CALL_RET(cpp, explicit_set_data, -ENOTSUP)(expl, data_master, data_ref);
}

static inline int bcfg_explicit_set_signal(struct nfp_cpp_explicit *expl,
					   uint8_t signal_master,
					   uint8_t signal_ref)
{
	BCFG_CALL_RET(cpp, explicit_set_signal, -ENOTSUP)(expl, signal_master, signal_ref);
}

static inline int bcfg_explicit_set_posted(struct nfp_cpp_explicit *expl,
					   int posted,
					   uint8_t siga,
					   enum nfp_cpp_explicit_signal_mode siga_mode,
					   uint8_t sigb,
					   enum nfp_cpp_explicit_signal_mode sigb_mode)
{
	BCFG_CALL_RET(cpp, explicit_set_posted, -ENOTSUP)
	(expl, posted, siga, siga_mode, sigb, sigb_mode);
}

static inline int bcfg_explicit_put(struct nfp_cpp_explicit *expl, const void *buff, size_t len)
{
	BCFG_CALL_RET(cpp, explicit_put, -ENOTSUP)(expl, buff, len);
}

static inline int bcfg_explicit_get(struct nfp_cpp_explicit *expl, void *buff, size_t len)
{
	BCFG_CALL_RET(cpp, explicit_get, -ENOTSUP)(expl, buff, len);
}

static inline int bcfg_explicit_do(struct nfp_cpp_explicit *expl, uint64_t address)
{
	BCFG_CALL_RET(cpp, explicit_do, -ENOTSUP)(expl, address);
}

static inline void bcfg_explicit_release(struct nfp_cpp_explicit *expl)
{
	BCFG_CALL_VOID(cpp, explicit_release)(expl);
}

#endif /* NFP_BOARDCONFIG_FN_H */
