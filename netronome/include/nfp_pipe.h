/*
 * Copyright (C) 2014,2017 Netronome Systems, Inc.
 * All right reserved.
 *
 */

#ifndef NFP_PIPE_H
#define NFP_PIPE_H

/* Enable for debug visibility */
#define DEBUG_PIPE (0u)

#if DEBUG_PIPE
#define DEBUG_PRINT(...) printf(__VA_ARGS__)
#else
#define DEBUG_PRINT(...)
#endif

/* The minimum buffer size is derived from the maximum flash sector size */
#define PIPE_MINIMUM_BUFFER_SIZE (4096u)
#define PIPE_OPERATION_INVALID (0u)

/*
 * The lower 8 bits of the 32-bit unsigned operation word is the operation opcode, while the upper
 * bits provide custom space for passing info to the worker
 */
#define PIPE_OPERATION_MASK(x) (0xFFu & (x))
#define PIPE_OPERATION_CUSTOM_MASK(y) (0xFFFFFF00u & (y))
#define PIPE_OPERATION_BUILD(x, y) (PIPE_OPERATION_MASK(x) | PIPE_OPERATION_CUSTOM_MASK(y))

#define SZ_CEIL64_TYPE(x) \
			(((sizeof(x) + sizeof(uint64_t) - 1)) & ~(sizeof(uint64_t) - 1))

#define SZ_CEIL64_VAL(x) \
			((((x) + sizeof(uint64_t) - 1)) & ~(sizeof(uint64_t) - 1))

struct nfp_pipe_worker;
struct nfp_pipe_master;
struct nfp_rtsym;

enum nfp_pipe_status {
	PIPE_STATE_UNAVAILABLE = 0, /* Not in a state to receive operations */
	PIPE_STATE_WAITING, /* Worker waiting for setup */
	PIPE_STATE_SETUP, /* Master preparing for operation */
	PIPE_STATE_PROCESSING /* Busy with an operation */
};

enum nfp_pipe_transaction_status {
	PIPE_TRANSACTION_STATUS_NONE = 0,
	PIPE_TRANSACTION_STATUS_START, /* Started processing transaction */
	PIPE_TRANSACTION_STATUS_END /* Completed processing transaction */
};

enum nfp_pipe_endpoint {
	PIPE_ENDPOINT_INVALID = 0, /* Invalid to detect unintialized uses */
	PIPE_ENDPOINT_MASTER, /* Endpoint that initiates an operation */
	PIPE_ENDPOINT_WORKER /* Endpoint that follows an operation request */
};

struct nfp_pipe {
	enum nfp_pipe_endpoint type;
	const struct nfp_rtsym *os_buffer;
	const struct nfp_rtsym *os_control;
	uint8_t *nfp_pipe_buffer;
	struct nfp_pipe_worker *worker_control;
	struct nfp_pipe_master *master_control;
	uint8_t *shared_control;
	struct nfp_cpp *cpp;
};

/**
 * Initialize a pipe endpoint.
 *
 * @param pipe			Handle
 * @param cpp			CPP Handle
 * @param type			Endpoint Type
 * @param os_update_buffer	Buffer NFP Area
 * @param os_update_control	Control NFP Area
 * @param meta_size		Meta block size
 *
 * @return 0 on success
 */
int nfp_pipe_init(struct nfp_pipe *pipe, struct nfp_cpp *cpp, enum nfp_pipe_endpoint type, const struct nfp_rtsym *os_update_buffer, const struct nfp_rtsym *os_update_control, int meta_size);

/**
 * Free the pipe endpoint.
 *
 * @param pipe			Handle
 *
 * @return 0 on success
 */
int nfp_pipe_exit(struct nfp_pipe *pipe);

/**
 * Perform a read of the control block from NFP memory
 *
 * @param pipe			Handle
 *
 * @return 0 on success
 */
int nfp_pipe_control_read(struct nfp_pipe *pipe);

/**
 * Perform a write of the control block into NFP memory
 *
 * @param pipe			Handle
 *
 * @return 0 on success
 */
int nfp_pipe_control_write(struct nfp_pipe *pipe);

/**
 * Perform a read of the data buffer from NFP memory
 *
 * @param pipe			Handle
 * @param bytes_written		Bytes to read from NFP memory into buffer
 *
 * @return 0 on success
 */
int nfp_pipe_buffer_read(struct nfp_pipe *pipe, int bytes_written);

/**
 * Perform a write of the data buffer into NFP memory
 *
 * @param pipe			Handle
 * @param bytes_read		Bytes to write from buffer into NFP memory
 *
 * @return 0 on success
 */
int nfp_pipe_buffer_write(struct nfp_pipe *pipe, int bytes_read);

/**
 * Worker status get
 *
 * @param pipe			Handle
 * @return			Worker Status
 */
enum nfp_pipe_status nfp_pipe_worker_status_get(struct nfp_pipe *pipe);

/**
 * Worker status set
 *
 * @param pipe			Handle
 * @param status		Worker Status
 *
 * @return 0 on success
 */
int nfp_pipe_worker_status_set(struct nfp_pipe *pipe, enum nfp_pipe_status status);

/**
 * Master status get
 *
 * @param pipe			Handle
 * @return			Master Status
 */
enum nfp_pipe_status nfp_pipe_master_status_get(struct nfp_pipe *pipe);

/**
 * Master status set
 *
 * @param pipe			Handle
 * @param status		Master Status
 *
 * @return 0 on success
 */
int nfp_pipe_master_status_set(struct nfp_pipe *pipe, enum nfp_pipe_status status);

/**
 * Worker transaction status get
 *
 * @param pipe			Handle
 * @return			Worker Transaction Status
 */
enum nfp_pipe_transaction_status nfp_pipe_worker_transaction_status_get(struct nfp_pipe *pipe);

/**
 * Worker transaction status set
 *
 * @param pipe			Handle
 * @param status		Worker Transaction Status
 *
 * @return 0 on success
 */
int nfp_pipe_worker_transaction_status_set(struct nfp_pipe *pipe, enum nfp_pipe_transaction_status status);

/**
 * Master transaction status get
 *
 * @param pipe			Handle
 * @return			Master Transaction Status
 */
enum nfp_pipe_transaction_status nfp_pipe_master_transaction_status_get(struct nfp_pipe *pipe);

/**
 * Master transaction status set
 *
 * @param pipe			Handle
 * @param status		Master Transaction Status
 *
 * @return 0 on success
 */
int nfp_pipe_master_transaction_status_set(struct nfp_pipe *pipe, enum nfp_pipe_transaction_status status);

/**
 * Master operation get
 *
 * @param pipe			Handle
 * @return			Operation
 */
uint32_t nfp_pipe_operation_get(struct nfp_pipe *pipe);

/**
 * Master operation set
 *
 * @param pipe			Handle
 * @param operation		Operation
 * @param meta_owner		Owner
 *
 * @return 0 on success
 */
int nfp_pipe_operation_set(struct nfp_pipe *pipe, uint32_t operation, enum nfp_pipe_endpoint meta_owner);

/**
 * Get a pointer to the meta mirror
 *
 * @param pipe			Handle
 * @return			Pointer to meta
 */
void *nfp_pipe_operation_meta(struct nfp_pipe *pipe);

/**
 * Get a pointer to the buffer mirror
 *
 * @param pipe			Handle
 * @return			Pointer to buffer
 */
void *nfp_pipe_operation_buffer(struct nfp_pipe *pipe);

/**
 * Debug information to assist with protocol debug
 *
 * @param pipe			Handle
 * @return 0 on success
 */
int nfp_pipe_control_debug(struct nfp_pipe *pipe);

#endif /* NFP_PIPE_H */
