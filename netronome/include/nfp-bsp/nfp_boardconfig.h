/*
 * Copyright (C) 2009 Netronome Systems, Inc.  All rights reserved.
 *
 */

#ifndef NFP_BOARDCONFIG_H
#define NFP_BOARDCONFIG_H

#include <stdint.h>
#include <sys/types.h>
#include <nfp_explicit.h>

#define PLL_CLK_R 0 /* Reference clock */
#define PLL_CLK_T 1 /* T_CLK - ME engine clock */
#define PLL_CLK_P 2 /* P_CLK - CPP bus clock */
#define PLL_CLK_A 3 /* A_CLK - ARM CPU clock */
#define PLL_CLK_U 4 /* U_CLK - UART baud rate clock */

/* NFP-BSP specific markers */
#define NFP_CPP_ID_WIDTH_32 (0 << 29)
#define NFP_CPP_ID_WIDTH_64 BIT(29)

/* Fill this structure in with function pointers, then
 * pass to the xmodem_recv or ymodem_recv calls.
 */
#define NFP_BOARDCONFIG_MODEM_INVALID 0
#define NFP_BOARDCONFIG_MODEM_XMODEM 1
#define NFP_BOARDCONFIG_MODEM_YMODEM 2

struct modem_op {
	/* Low-level modem ops */
	int (*readc)(int timeout_ms); /* Return -ETIMEDOUT on timeout */
	int (*writec)(int c);
	/* Per-file operations */
	void *(*open)(const char *name, time_t date, int mode, int perm);
	void (*close)(void *priv);
	int (*write)(void *priv, off_t offset, const void *buff, size_t len);
	int (*read)(void *priv, off_t offset, void *buff, size_t len);
};

/* Board Configuration Operations */
struct nfp_boardconfig {
	struct nfp_boardconfig_system {
		/* 'version' is the number of defined operations */
		int version;
		/* System operations */
		union {
			/* Dummy placeholders */
			int (*noop[16])(void);
			struct nfp_boardconfig_system_func {
				/* [0] Print a character to the console
				 * \param c     Character to print
				 */
				void (*console_putc)(char c);
				/* [1] Get a character from the console
				 * \return character, or < 0 if none available.
				 */
				int (*console_getc)(void);

				/* [2] Set timeout system */
				void (*timeout_set)(unsigned int usec);

				/* [3] Timeout tripped? */
				int (*timeout_tripped)(void);

				/* [4] FIS file load - REMOVED */
				int (*_fis_load)(void);

				/* [5] Get the PLL rate in Hz */
				uint32_t (*pll_rate)(int pll_id);

				/* [6] CRC32 POSIX - just the core, does not do extension with length! */
				uint32_t (*crc32_posix)(uint32_t crc, const void *buff, size_t len);

				/* [7] CRC32 Gary Brown */
				uint32_t (*crc32_gb)(uint32_t crc, const void *buff, size_t len);

				/* [8] NOTE: modem_recv will always open
				 *       the file with name == NULL
				 */
				int (*modem_recv)(int protocol, const struct modem_op *op);

				/* [9] Enumerate the FIS table */
				const struct fis_dirent *(*fis_next)(int *index);

				/* [10] Load a FIS entry */
				int (*fis_load)(const struct fis_dirent *de);

				/* [11] Set address and size for console log */
				int (*console_relocate)(void *address, unsigned int size);

				/* [12] Read from SPI Flash */
				int (*flash_read)(int bus,
						  int cs,
						  unsigned int offset,
						  void *dst,
						  unsigned int len);
			} func;
		};
	} system;

	struct nfp_boardconfig_hwinfo {
		/* 'version' is the number of defined operations */
		int version;

		union {
			/* Dummy placeholders */
			int (*noop[16])(void);
			struct nfp_boardconfig_hwinfo_func {
				/* [0] Set a key in the hwinfo to a value.
				 * \param key   Key to set/alter
				 * \param value Value to set key to
				 * \return      >= 0 on success, < 0 on failure.
				 */
				int (*hwinfo_set)(const char *key, const char *value);

				/* [1] Get a value from the hwinfo by key ID
				 * \param key   Key to get
				 * \return      pointer to value, or NULL on failure
				 */
				const char *(*hwinfo_get)(const char *key);

				/* [2] Get the next key. *ptr should be NULL
				 * on the first call
				 */
				const char *(*hwinfo_key_next)(void **ptr);

				/* [3] Load a hwinfo table into the master table */
				int (*hwinfo_load)(void *hwinfo_db);

				/* [4] Set address and size for console log */
				int (*hwinfo_relocate)(void *address, unsigned int size);

				/* [5] Load a hwinfo table into a subkey of the master */
				int (*hwinfo_load_at)(void *hwinfo_db, const char *prefix);
			} func;
		};
	} hwinfo;

	/* DEPRECATED - use functions in nfp_boardconfig_cpp */
	struct nfp_boardconfig_xpb {
		/* 'version' is the number of defined operations */
		int version;

		union {
			/* Dummy placeholders */
			int (*noop[8])(void);
			struct nfp_boardconfig_xpb_func {
				/* Read from an offset into XPB space
				 * \param xpb_addr      Offset into XPB space
				 * \return              XPB value
				 */
				uint32_t (*xpb_readl)(uint32_t xpb_addr);

				/* Write at an offset into XPB space
				 * \param xpb_addr      Offset into XPB space
				 * \param xpb_value     XPB value
				 */
				void (*xpb_writel)(uint32_t xpb_addr, uint32_t xpb_value);
			} func;
		};
	} _xpb;

	struct nfp_boardconfig_cpp {
		/* 'version' is the number of defined operations */
		int version;

		union {
			/* Dummy placeholders */
			int (*noop[32])(void);
			struct nfp_boardconfig_cpp_func {
				/* Read buffer from CPP space
				 * \param source        NFP_CPP_ID of the source (and
				 * NFP_CPP_ID_WIDTH_xx marker)
				 * \param address       40-bit address to read from
				 * \param buff          Data buffer
				 * \param len           Size of data buffer
				 * \return              >= 0 on success, < 0 on failure
				 */
				int (*cpp_read)(uint32_t source,
						uint64_t address,
						void *buff,
						size_t len);

				/* Write buffer to CPP space
				 * \param source        NFP_CPP_ID of the source (and
				 * NFP_CPP_ID_WIDTH_xx marker)
				 * \param address       40-bit address to read from
				 * \param buff          Data buffer
				 * \param len           Size of data buffer
				 * \return              >= 0 on success, < 0 on failure
				 */
				int (*cpp_write)(uint32_t source,
						 uint64_t address,
						 const void *buff,
						 size_t len);

				/* Read from an offset into XPB space
				 * \param xpb_addr      Offset into XPB space
				 * \param xpb_value     XPB value
				 * \return              >= 0 on success, < 0 on failure
				 */
				int (*xpb_readl)(uint32_t xpb_addr, uint32_t *val);

				/* Write at an offset into XPB space
				 * \param xpb_addr      Offset into XPB space
				 * \param xpb_value     XPB value
				 * \return              >= 0 on success, < 0 on failure
				 */
				int (*xpb_writel)(uint32_t xpb_addr, uint32_t xpb_value);

				/* Get the model code for this CPP interface */
				uint32_t (*cpp_model)(void);

				/* Explicit functions */
				struct nfp_cpp_explicit *(*explicit_acquire)(void);

				int (*explicit_set_target)(struct nfp_cpp_explicit *expl,
							   uint32_t cpp_id,
							   uint8_t len,
							   uint8_t mask);

				int (*explicit_set_data)(struct nfp_cpp_explicit *expl,
							 uint8_t data_master,
							 uint16_t data_ref);

				int (*explicit_set_signal)(struct nfp_cpp_explicit *expl,
							   uint8_t signal_master,
							   uint8_t signal_ref);

				int (*explicit_set_posted)(
					struct nfp_cpp_explicit *expl,
					int posted,
					uint8_t siga,
					enum nfp_cpp_explicit_signal_mode siga_mode,
					uint8_t sigb,
					enum nfp_cpp_explicit_signal_mode sigb_mode);

				int (*explicit_put)(struct nfp_cpp_explicit *expl,
						    const void *buff,
						    size_t len);

				int (*explicit_do)(struct nfp_cpp_explicit *expl, uint64_t address);

				int (*explicit_get)(struct nfp_cpp_explicit *expl,
						    void *buff,
						    size_t len);

				void (*explicit_release)(struct nfp_cpp_explicit *expl);
			} func;
		};
	} cpp;
};

#endif /* NFP_BOARDCONFIG_H */
