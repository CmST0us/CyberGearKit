//
//  utils.h
//  test_bit
//
//  Created by Eric Wu on 2023/9/5.
//

#pragma once

#include <stdint.h>
#include <stdbool.h>

typedef union {
    uint64_t value;
    uint8_t bytes[8];
} bit_value_t;

typedef union {
    uint32_t value;
    uint8_t bytes[4];
} bit32_value_t;

typedef union {
    uint16_t value;
    uint8_t bytes[2];
} bit16_value_t;

uint16_t bit_utils_swap_little_endian_value_into_host_endian16(uint16_t);
uint32_t bit_utils_swap_little_endian_value_into_host_endian32(uint32_t);
uint64_t bit_utils_swap_little_endian_value_into_host_endian64(uint64_t);

uint16_t bit_utils_swap_big_endian_value_into_host_endian16(uint16_t);
uint32_t bit_utils_swap_big_endian_value_into_host_endian32(uint32_t);
uint64_t bit_utils_swap_big_endian_value_into_host_endian64(uint64_t);

uint16_t bit_utils_swap_host_endian_value_into_little_endian16(uint16_t);
uint32_t bit_utils_swap_host_endian_value_into_little_endian32(uint32_t);
uint64_t bit_utils_swap_host_endian_value_into_little_endian64(uint64_t);

uint16_t bit_utils_swap_host_endian_value_into_big_endian16(uint16_t);
uint32_t bit_utils_swap_host_endian_value_into_big_endian32(uint32_t);
uint64_t bit_utils_swap_host_endian_value_into_big_endian64(uint64_t);

/// 读取特定位置的比特值
/// - Parameters:
///   - source: 数据
///   - source_length: 数据长度
///   - offset: 比特偏移
///   - bit_count: 比特长度
/// - Returns: 读取的数值
bit_value_t bit_utils_get_value(const bit_value_t source,
                                const uint16_t source_length,
                                const uint16_t offset,
                                const uint16_t bit_count);

/// 设置特定位置的比特值
/// - Parameters:
///   - value: 设置的值
///   - offset: 比特偏移
///   - bit_count: 比特长度
///   - destination: 目标数据
///   - destinatin_length: 目标数据长度
bool bit_utils_set_value(const bit_value_t value,
                         const uint16_t offset,
                         const uint16_t bit_count,
                         uint8_t *destination,
                         uint16_t destinatin_length);


