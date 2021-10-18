/*
 * Copyright (C) 2014,2017 Netronome Systems, Inc.
 * All right reserved.
 *
 */

#ifndef NFP_OS_UPDATE_PIPE_DEF_H
#define NFP_OS_UPDATE_PIPE_DEF_H

/* These are the required symbols for os update to work */
#define OS_UPDATE_BUFFER "os_update_buffer"
#define OS_UPDATE_CONTROL "os_update_control"

/* If the symbols are not available in the firmware */
#define OS_UPDATE_DEFAULT_CONTROL_ADDR (0x2000000u)
#define OS_UPDATE_DEFAULT_CONTROL_SIZE (0x400u)
#define OS_UPDATE_DEFAULT_BUFFER_ADDR  (0x3000000u)
#define OS_UPDATE_DEFAULT_BUFFER_SIZE  (0x400000u)
#define OS_UPDATE_DEFAULT_DOMAIN (24u)
#define OS_UPDATE_DEFAULT_TARGET (7u)

/*
 * Meta structs go into the control block and can only be written
 * by the master. The data structs/types go into the buffer block
 * and may be written by either the master, or the worker.
 *
 * Typically, for raw block transfers from the master to the worker
 * no type is declared here as the user will simply use a pointer
 * to the block. In the same way, in the case where the master
 * request a single transaction request from the worker, no meta
 * block is needed, and only the structure used for interpreting the
 * reply in the buffer.
 */

/*
 * FLASH OPERATIONS CUSTOM BITS
 *
 * [31-24]  :   [23-16]   :   [15-8]     :   [7-0]
 *
 * Unused       Size Option   Device       Operation
 *
 * Size Option: 0 - Last written image size if applicable, else full flash
 *              1 - Full flash size
 *              2 - First 32Mb of flash
 */

#define PIPE_OP_MMC_DEV(x) (((x) >> 8) & 0xFF)

#define PIPE_OP_MMC_SZ_OPTION(x) (((x) >> 16) & 0xFF)
#define PIPE_OP_MMC_SZ_OPTION_IMG_SIZE (0u)
#define PIPE_OP_MMC_SZ_OPTION_FLASH_SIZE (1u)
#define PIPE_OP_MMC_SZ_OPTION_FLASH_32MB (2u)

#define PIPE_OP_MMC_CREATE(op, dev, z) (op | ((dev & 0xFF) << 8) | ((z & 0xFF) << 16))

#define PIPE_OPERATION_MMC_WRITE (0x1u)
struct pipe_operation_mmc_write_meta {
	uint64_t write_size;
	uint32_t transaction_size;
	uint32_t transaction_count;
	uint32_t transaction_total;
};

#define PIPE_OPERATION_MMC_READ  (0x2u)
struct pipe_operation_mmc_read_meta {
	uint64_t read_size;
	uint32_t transaction_size;
	uint32_t transaction_count;
	uint32_t transaction_total;
};

#define PIPE_OPERATION_MMC_INFO  (0x3u)
struct pipe_operation_mmc_info_data {
	uint32_t mmc_valid;
	uint32_t mmc_sector_size;
	uint32_t mmc_sector_count;
};

#define PIPE_OPERATION_MMC_ERASE  (0x4u)
struct pipe_operation_mmc_erase_meta {
	uint64_t read_size;
	uint32_t transaction_size;
	uint32_t transaction_count;
	uint32_t transaction_total;
};

#define OS_UPDATE_OPERATION_MAX_META_SIZE (64u)

#endif /* NFP_OS_UPDATE_PIPE_DEF_H */
