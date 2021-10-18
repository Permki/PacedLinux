/* Copyright (C) 2015,  Netronome Systems, Inc.  All rights reserved. */

#include <stdint.h>
#include <nfp.h>
#include <pif_common.h>

#include "pif_counters.h"

/*
 * counter declarations
 */

#define PIF_COUNTER(_suffix) \
    __export __emem uint64_t pif_counter_##_suffix = 0;

#define PIF_COUNTER_ARRAY(_suffix, _sz) \
    __export __emem uint64_t pif_counter_##_suffix[_sz] = {0};

#include "pif_counter.inc"
