/**
 * Copyright (C) 2008-2014 Netronome Systems, Inc.  All rights reserved.
 *
 * @file nfp_nbi_phy_vsemi.h
 *
 */

#ifndef __NFP_NBI_PHY_VSEMI_H__
#define __NFP_NBI_PHY_VSEMI_H__

#include <stdint.h>
#include <nfp.h>
#include <nfp_serdes.h>

#define SERDES_DEBUG 0

#define serdes_debug_print(fmt, ...) \
	if (SERDES_DEBUG) { fprintf(stdout, fmt, __VA_ARGS__); }

#define POLL_ATTEMPTS 5
enum NFP_PHY_OPERATION { NFP_PHY_READ, NFP_PHY_WRITE };
enum NFP_PHY_BEHAVIOR { NFP_PHY_POLL, NFP_PHY_IGNORE, NFP_PHY_NOTIFY };

#define FIELD_SIZE 96
struct serdes_transaction {
	const char field[FIELD_SIZE];
	int lane;
    const enum NFP_PHY_OPERATION op;
	const uint32_t value;
    const enum NFP_PHY_BEHAVIOR behavior;
};

struct serdes_sequence {
	char name[FIELD_SIZE];
	char source[FIELD_SIZE];
	int num_instructions;
	const struct serdes_transaction transactions[];
};

#define NUM_LANES 4
#define POLLING_SLEEP 5
#define POLLING_RETRIES 10

#ifdef DEBUG
#define debug(msg) printf("DEBUG: %s" % msg);
#else
#define debug(msg)
#endif

int execute_sequence(struct nfp_serdes *serdes, int group,
                     struct serdes_transaction *sequence, int num_instructions);

#endif
