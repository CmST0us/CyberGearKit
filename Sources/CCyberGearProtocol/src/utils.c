//
//  utils.c
//  test_bit
//
//  Created by Eric Wu on 2023/9/5.
//

#include "utils.h"
#include <string.h>
#include <assert.h>
#include <sys/param.h>

#define BSWAP_8(x) ((x) & 0xff)
#define BSWAP_16(x) ((BSWAP_8(x) << 8) | BSWAP_8((x) >> 8))
#define BSWAP_32(x) ((BSWAP_16(x) << 16) | BSWAP_16((x) >> 16))
#define BSWAP_64(x) ((BSWAP_32(x) << 32) | BSWAP_32((x) >> 32))

uint16_t bswap16(uint16_t v)
{
    return BSWAP_16(v);
}

uint32_t bswap32(uint32_t v)
{
    return BSWAP_32(v);
}

uint64_t bswap64(uint64_t v)
{
    return BSWAP_64(v);
}

bool is_little_endian(void)
{
    return BYTE_ORDER == LITTLE_ENDIAN;
}

uint64_t bitmask (const uint8_t bit_count) {
    if (bit_count >= 64) {
        return 0xFFFFFFFFFFFFFFFF;
    } else {
        return (((uint64_t)0x1) << bit_count) - 1;
    }
}

uint16_t bit_utils_swap_little_endian_value_into_host_endian16(uint16_t v) {
    if (is_little_endian()) {
        return v;
    }
    return bswap16(v);
}

uint32_t bit_utils_swap_little_endian_value_into_host_endian32(uint32_t v) {
    if (is_little_endian()) {
        return v;
    }
    return bswap32(v);
}

uint64_t bit_utils_swap_little_endian_value_into_host_endian64(uint64_t v) {
    if (is_little_endian()) {
        return v;
    }
    return bswap64(v);
}

uint16_t bit_utils_swap_big_endian_value_into_host_endian16(uint16_t v) {
    if (is_little_endian()) {
        return bswap16(v);
    }
    return v;
}

uint32_t bit_utils_swap_big_endian_value_into_host_endian32(uint32_t v) {
    if (is_little_endian()) {
        return bswap32(v);
    }
    return v;
}

uint64_t bit_utils_swap_big_endian_value_into_host_endian64(uint64_t v) {
    if (is_little_endian()) {
        return bswap64(v);
    }
    return v;
}

uint16_t bit_utils_swap_host_endian_value_into_little_endian16(uint16_t v) {
    if (is_little_endian()) {
        return v;
    }
    return bswap16(v);
}

uint32_t bit_utils_swap_host_endian_value_into_little_endian32(uint32_t v) {
    if (is_little_endian()) {
        return v;
    }
    return bswap32(v);
}

uint64_t bit_utils_swap_host_endian_value_into_little_endian64(uint64_t v) {
    if (is_little_endian()) {
        return v;
    }
    return bswap64(v);
}

uint16_t bit_utils_swap_host_endian_value_into_big_endian16(uint16_t v) {
    if (is_little_endian()) {
        return bswap16(v);
    }
    return v;
}

uint32_t bit_utils_swap_host_endian_value_into_big_endian32(uint32_t v) {
    if (is_little_endian()) {
        return bswap32(v);
    }
    return v;
}

uint64_t bit_utils_swap_host_endian_value_into_big_endian64(uint64_t v) {
    if (is_little_endian()) {
        return bswap64(v);
    }
    return v;
}

bit_value_t bit_utils_get_value(const bit_value_t source,
                                const uint16_t source_length,
                                const uint16_t offset,
                                const uint16_t bit_count) {
    uint64_t source_value = source.value;
    source_value = source_value >> offset;
    source_value = source_value & bitmask(bit_count);
    
    bit_value_t result = {0};
    result.value = source_value;
    
    return result;
}

bool bit_utils_set_value(const bit_value_t value,
                         const uint16_t offset,
                         const uint16_t bit_count,
                         uint8_t *destination,
                         uint16_t destinatin_length) {
    assert(destinatin_length <= 8);
    
    bit_value_t result = {0};
    memcpy(result.bytes, destination, destinatin_length);
    
    uint64_t value_after_offset = value.value << offset;
    uint64_t mask = bitmask(bit_count) << offset;
    
    uint64_t value_after_mask = value_after_offset & mask;
    result.value = (result.value & ~mask) | value_after_mask;
    memcpy(destination, result.bytes, destinatin_length);
    return true;
}

