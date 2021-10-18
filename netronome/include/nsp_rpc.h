/**
 * Copyright (C) 2017, Netronome Systems, Inc.
 * All rights reserved.
 *
 * nsp rpc core
 *
 */

#ifndef __NFP_RPC_H__
#define __NFP_RPC_H__

#include <stdint.h>

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(x)       (sizeof((x)) / sizeof((x)[0]))
#endif

struct nfp_device;

#define NFP_RPC_MAGIC   0x6370724e	/* Nrpc  */

#define RPCT_IN_FLAG    BIT(7)
#define RPCT_OUT_FLAG   BIT(6)
#define RPCT_PTYPE(p)   ((p) & ((1 << 6) - 1))
#define I(p)            ((p) | RPCT_IN_FLAG)
#define O(p)            ((p) | RPCT_OUT_FLAG)
#define IO(p)           (I(O(p)))

#define RPCT_S8         1
#define RPCT_U8         2
#define RPCT_S16        3
#define RPCT_U16        4
#define RPCT_S32        5
#define RPCT_U32        6
#define RPCT_S64        7
#define RPCT_U64        8
#define RPCT_F          9
#define RPCT_D          10
#define RPCT_L          11
#define RPCT_B          12
#define RPCT_S          13

#define IPC_MAX_PARAMETER_COUNT     15
#define IPC_MAX_PARAMETER_SIZE      256

struct rpccall {
	uint32_t magic;
	uint32_t version;
	uint32_t result;
	uint32_t function;
	uint32_t param_size;
	char signature[IPC_MAX_PARAMETER_COUNT + 1];
	uint8_t parameters[IPC_MAX_PARAMETER_SIZE];
} __attribute__ ((__packed__));

int nsp_rpc(struct nfp_device *nfp, uint32_t *result, uint32_t function, ...);

int nsp_rpc_server(struct nfp_device *nfp, struct rpccall *v);

struct rpcfunc {
	int function;
	int (*handler)(struct nfp_device *, struct rpccall *);
	const char *signature;
};

extern uint32_t rpcfunc_list_version;
extern struct rpcfunc rpc_function_list[];
extern int rpc_function_count;

#define RPC_PRM_SZ(x)           (((x) % 4) ? ((x) + 3) & ~3 : (x))

#define rpc_arg_ptr_size(PTR, TYPE, SIZE) \
	({                                \
		TYPE *v = (TYPE *)PTR;    \
		PTR += RPC_PRM_SZ(SIZE);  \
		v;                        \
	})

#define rpc_arg_ptr(PTR, TYPE)          rpc_arg_ptr_size(PTR, TYPE, sizeof(TYPE))

#define RPC_FUNC_VOID_SIG(FUNC)         const char rpc_##FUNC##_signature[] = { '\0' };
#define RPC_FUNC_SIG(FUNC, SIG...)      const char rpc_##FUNC##_signature[] = { SIG, '\0' };
#define RPC_LIST_ENTRY(FUNC)[FUNC##__id] = { FUNC##__id, rpcs_##FUNC, rpc_##FUNC##_signature },

#define NFP_RPC(NFP, RES, FUNC, ARGS...)    nsp_rpc(NFP, RES, FUNC##__id, ## ARGS, \
						    NULL, (void *)NFP_RPC_MAGIC)
#endif /* __NFP_RPC_H__ */
