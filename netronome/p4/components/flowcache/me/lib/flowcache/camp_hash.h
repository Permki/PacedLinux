#ifndef _CAMP_HASH_H
#define _CAMP_HASH

#include <types.h>
#include <stdint.h>

void camp_hash_init();

typedef struct {
    uint32_t hi;
    uint32_t lo;
} hash_result_t;

hash_result_t camp_hash(void *data, size_t length, size_t max_length);

#endif
