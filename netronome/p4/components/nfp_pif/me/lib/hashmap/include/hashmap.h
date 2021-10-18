/*
 * Copyright (C) 2014-2015,  Netronome Systems, Inc.  All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * @file        hashmap.h 
 * @brief       basic lookup table implementation.
 */


#include <types.h>
#include <stdint.h>

#include <nfp.h>

#include <nfp/me.h>
#include <nfp/mem_atomic.h>
#include <nfp/mem_bulk.h>

#include <nfp6000/nfp_me.h>

#include <assert.h>

#include "camp_hash.h"
#include "unroll.h"

typedef uint8_t bool;

typedef __packed struct { 
    union {
        struct {
            uint32_t valid : 1;
            uint32_t reserved1: 15;
            uint32_t lock : 16;
        };
        uint32_t meta;
    };
    uint32_t reserved2[3];
} __hashmap_descriptor_t;

typedef enum __hashmap_op { LOOKUP, ADD, REMOVE } __hashmap_op_t;

typedef __packed struct {
    __export __mem __addr40 void *addr;
    uint16_t state;
} hashmap_lock_t; 


typedef struct {
    __hashmap_op_t op;
    uint32_t key[32];
    uint32_t mask[32];
} hashmap_config_t;

#define __HASHMAP_DESC_VALID_BIT        (1 << 31)
#define __HASHMAP_DESC_LOCK_EXCL_BIT    (1 << 14)

void __inline __hashmap_lock_init(hashmap_lock_t *lock, __export __mem __addr40 void *addr) 
{
    lock->state = 1;
    lock->addr = addr;
}

bool __inline __hashmap_lock_shared(hashmap_lock_t *lock) 
{
    __xrw __hashmap_descriptor_t desc_xfer;
    desc_xfer.meta = 1;
retry_lock: 
    mem_test_add((void *) &desc_xfer, lock->addr, 4); 
    if (desc_xfer.lock & __HASHMAP_DESC_LOCK_EXCL_BIT) { 
        mem_sub32((void *) &desc_xfer, lock->addr, 4);
        sleep(100); 
        goto retry_lock; 
    }
    return desc_xfer.valid;
}

bool __inline __hashmap_lock_upgrade(hashmap_lock_t *lock)
{
    __xrw __hashmap_descriptor_t desc_xfer; 
    desc_xfer.meta = __HASHMAP_DESC_LOCK_EXCL_BIT | __HASHMAP_DESC_VALID_BIT;
    mem_test_set((void *) &desc_xfer, lock->addr, 4);
    if (desc_xfer.lock & __HASHMAP_DESC_LOCK_EXCL_BIT) {
        desc_xfer.meta = 1;
        mem_sub32((void *) &desc_xfer, lock->addr, 4);
        sleep(100);
        return 0;
    }
    lock->state |= __HASHMAP_DESC_LOCK_EXCL_BIT;
    return 1;
}

void __intrinsic __hashmap_lock_release(hashmap_lock_t *lock)
{
    struct nfp_mecsr_prev_alu ind;
    unsigned int addr_hi, addr_lo;

    addr_hi = ((unsigned long long int)(lock->addr) >> 8) & 0xff000000;
    addr_lo = (unsigned long long int)(lock->addr) & 0xffffffff;


    ind.__raw = 0; 
    ind.ove_data = 2;
    ind.data16 = lock->state;
    ind.ov_len = 1; 
    ind.length = (1<<3);
   
    __asm { alu[--, --, B, ind.__raw] }
    __asm { mem[sub_imm, --, addr_hi, <<8, addr_lo, 1], indirect_ref }

    lock->state = 0;
}

void __inline __hashmap_lock_release_and_invalidate(hashmap_lock_t *lock) 
{
    __xwrite __hashmap_descriptor_t desc_xfer;
    desc_xfer.meta = lock->state | __HASHMAP_DESC_VALID_BIT;
    mem_sub32((void *) &desc_xfer, lock->addr, 4); 
    lock->state = 0;
}

#define __HASHMAP_DECLARE_1_PARTITION(instance, entries) \
    __export __emem __declspec(aligned(256)) __hashmap_##instance##_entry_t __HASHMAP_##instance##_DATA_0[entries]; 
 
#define __HASHMAP_DECLARE_2_PARTITION(instance, entries) \
    __export __emem_n(0) __hashmap_##instance##_entry_t __HASHMAP_##instance##_DATA_0[(entries / 2)]; \
    __export __emem_n(1) __hashmap_##instance##_entry_t __HASHMAP_##instance##_DATA_1[(entries / 2)]; 

#define __HASHMAP_DECLARE_3_PARTITION(instance, entries) \
    __export __emem_n(0) __hashmap_##instance##_entry_t __HASHMAP_##instance##_DATA_0[(entries / 3)]; \
    __export __emem_n(1) __hashmap_##instance##_entry_t __HASHMAP_##instance##_DATA_1[(entries / 3)]; \
    __export __emem_n(2) __hashmap_##instance##_entry_t __HASHMAP_##instance##_DATA_2[(entries / 3)]; 

#define __HASHMAP_SELECT_1_PARTITION(instance, hash, selection) selection = __HASHMAP_##instance##_DATA_0
    
#define __HASHMAP_SELECT_2_PARTITION(instance, hash, selection) \
    if (hash & 0x80000000) { selection = __HASHMAP_##instance##_DATA_1; } \
    else { selection = __HASHMAP_##instance##_DATA_0; }

#define __HASHMAP_SELECT_3_PARTITION(instance, hash, selection) \
    if (hash < 0x55000000) { \
        selection = __HASHMAP_##instance##_DATA_0; \
    } \
    else if (hash < 0xAA000000) { \
        selection = __HASHMAP_##instance##_DATA_1; \
    } \
    else { \
        selection = __HASHMAP_##instance##_DATA_2; \
    }

#define __HASHMAP_COMPARE(IDX) do { \
    uint32_t cmp; \
    do { \
        __asm alu[cmp, key_xfer[IDX], -, *l$index3++] \
    } while (0); \
    if (cmp != 0) goto miss; \
} while (0);

#define __HASHMAP_COPY_KEY(IDX) do { \
    __asm alu[key_xfer[IDX], --, B, *l$index3++] \
} while (0);

#define HASHMAP_DECLARE_PARTITIONED(instance, entries, key_type, value_type, partitions) \
    typedef struct { \
        __hashmap_descriptor_t desc; \
        key_type key; \
        value_type value; \
    } __hashmap_##instance##_entry_t; \
    __HASHMAP_DECLARE_##partitions##_PARTITION(instance, entries); \
    __export __mem __addr40 __inline value_type *__hashmap_##instance##_lookup(hashmap_lock_t *lock, __lmem key_type *key, uint8_t length, __hashmap_op_t op) \
    { \
        __export __mem __addr40 __hashmap_##instance##_entry_t *data; \
        __xrw key_type key_xfer; \
        hash_result_t hash = camp_hash((void *) key, length, sizeof(key_type) / 4); \
        __HASHMAP_SELECT_##partitions##_PARTITION(instance, hash.hi, data); \
        __hashmap_lock_init(lock, (__export __mem __addr40 void *) &(data[hash.hi % (entries / partitions)])); \
retry:  if (__hashmap_lock_shared(lock)) { \
            local_csr_write(local_csr_active_lm_addr_3, (unsigned int) key); \
            mem_read32((void *) &key_xfer, (__export __mem __addr40 void *) &(((__export __mem __addr40 __hashmap_##instance##_entry_t *) lock->addr)->key), (length * 4)); \
            UNROLL(__HASHMAP_COMPARE, length, sizeof(key_type) / 4) \
            if (op == REMOVE) { \
                if (__hashmap_lock_upgrade(lock)) { \
                    __hashmap_lock_release_and_invalidate(lock); \
                    return 0; \
                } \
                goto retry; \
            } \
            else { \
                return &(((__export __mem __addr40 __hashmap_##instance##_entry_t *) lock->addr)->value); \
            } \
        } \
        if (op == ADD) { \
            if (__hashmap_lock_upgrade(lock)) { \
                local_csr_write(local_csr_active_lm_addr_3, (unsigned int) key); \
                UNROLL(__HASHMAP_COPY_KEY, length, sizeof(key_type) / 4) \
                mem_write32((void *) &key_xfer, (__export __mem __addr40 void *) &(((__export __mem __addr40 __hashmap_##instance##_entry_t *) lock->addr)->key), (length * 4)); \
                return &(((__export __mem __addr40 __hashmap_##instance##_entry_t *) lock->addr)->value); \
            } \
            goto retry; \
        } \
miss:   __hashmap_lock_release(lock); \
        return 0; /* TODO: search further */ \
    }

#define HASHMAP_DECLARE(instance, entries, key_type, value_type) HASHMAP_DECLARE_PARTITIONED(instance, entries, key_type, value_type, 1)

#define HASHMAP_LOOKUP(instance, lock, key, length) __hashmap_##instance##_lookup(lock, key, length, LOOKUP) 

#define HASHMAP_LOOKUP_ADD(instance, lock, key, length) __hashmap_##instance##_lookup(lock, key, length, ADD)

#define HASHMAP_REMOVE(instance, lock, key, length) __hashmap_##instance##_lookup(lock, key, length, REMOVE)

#define HASHMAP_RELEASE(lock) __hashmap_lock_release(lock)


