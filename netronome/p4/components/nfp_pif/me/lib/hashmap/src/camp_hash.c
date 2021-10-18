#include <nfp/me.h>
#include <nfp/mem_bulk.h>
#include <std/reg_utils.h>

#include <assert.h>

#include "unroll.h"

#include "camp_hash.h"

#ifdef CAMP_HASH_SECURE
#define CAMP_HASH_STEP(IDX) do { \
    __asm alu[result.hi, result.hi, +, *l$index3++] \
    __asm alu[result.hi, result.hi, XOR, *n$index++] \
    __asm crc_be[crc_32, cpy, result.hi] \
    __asm alu[result.lo, result.lo, XOR, result.hi] \
    __asm dbl_shf[result.hi, cpy, result.hi, >>indirect] \
} while (0);
#else
#define CAMP_HASH_STEP(IDX) do { \
    __asm alu[result.hi, result.hi, +, *l$index3++] \
    __asm crc_be[crc_32, cpy, result.hi] \
    __asm alu[result.lo, result.lo, XOR, result.hi] \
    __asm dbl_shf[result.hi, cpy, result.hi, >>indirect] \
} while (0);
#endif


#ifdef CAMP_HASH_SECURE
static const int32_t CAMP_HASH_PAD_NN_IDX = 64;
static const int32_t CAMP_HASH_PAD_SIZE_LW = 32;

static __mem __addr40__export __shared uint32_t CAMP_HASH_PAD[] = \
    { 0x05e37dc7, 0x820bfd7f, 0xc0a8d1d3, 0x1e7433f4, \
      0xa4e17724, 0x22efe253, 0xd04ea6bd, 0xae676609, \
      0xe4354df6, 0xa76b008c, 0xa8dec911, 0x4a573ca8, \
      0x2a85c196, 0x31d0a299, 0x5dae2d36, 0x5ba9ea1f, \
      0x6c37b38f, 0x3aee5498, 0x2421f934, 0x01f1ae08, \
      0x993a96e7, 0x9a1a6bf3, 0x0bbc6452, 0x57d85210, \
      0xb1b60e52, 0xee86be25, 0x51cdac42, 0x22d6cd1d, \
      0x15b6fa5f, 0xc56d11f3, 0xc8a2c9bb, 0xe13a69d5 };
#endif

void camp_hash_init()
{
#ifdef CAMP_HASH_SECURE
    __xread uint32_t pad[8];
    uint32_t i;

    local_csr_write(local_csr_nn_put, CAMP_HASH_PAD_NN_IDX);

    for (i = 0; i < 8; i++) {
        mem_read32(pad, (__mem __addr40 uint32_t*) CAMP_HASH_PAD + i, sizeof(pad));
        __asm {
            alu[*n$index++, --, B, pad[0]]
            alu[*n$index++, --, B, pad[1]]
            alu[*n$index++, --, B, pad[2]]
            alu[*n$index++, --, B, pad[3]]
            alu[*n$index++, --, B, pad[4]]
            alu[*n$index++, --, B, pad[5]]
            alu[*n$index++, --, B, pad[6]]
            alu[*n$index++, --, B, pad[7]]
        }
    }
#endif
}

__intrinsic hash_result_t camp_hash(void *data, size_t length, size_t max_length)
{
    hash_result_t result;
    uint32_t cpy, crc, rot;
    uint32_t len = length;

    ctassert(__is_in_lmem(data));

#ifdef CAMP_HASH_SECURE
    local_csr_write(local_csr_nn_get, CAMP_HASH_PAD_NN_IDX);
#endif
    local_csr_write(local_csr_active_lm_addr_3, (unsigned int) data);

    crc_write(length);

#ifdef CAMP_HASH_SECURE
    __asm {
        alu[result.hi, len, +, *n$index++]
        alu[result.lo, len, XOR, *n$index++]
    }
#else
    result.hi = len;
    result.lo = ~len;
#endif

    UNROLL(CAMP_HASH_STEP, length, max_length)

    __critical_path();

    result.lo += rotr32(result.lo, result.hi);

    crc = crc_read();
    result.hi += crc;

    result.lo ^= rotr32(result.hi, result.lo);
    result.lo ^= crc;

    return result;
}
