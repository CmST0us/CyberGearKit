//
// Created by Eric Wu on 2023/9/4.
//

#include <string.h>
#include <stdio.h>
#include <bitfield/bitfield.h>
#include <bitfield/8byte.h>
#include <canutil/read.h>
#include <canutil/write.h>

#include "cyber_gear_protocol.h"

// CAN ID 通讯类型比特位偏移
#define kCAN_ID_COMM_TYPE_BIT_OFFSET (24)
// CAN ID 通讯类型比特长度
#define kCAN_ID_COMM_TYPE_LENGTH (5)

// CAN ID 主机ID比特位偏移
#define kCAN_ID_HOST_CAN_ID_BIT_OFFSET (8)
// CAN ID 主机ID比特长度
#define kCAN_ID_HOST_CAN_ID_LENGTH (8)

// CAN ID 目标ID比特位偏移
#define kCAN_ID_TARGET_CAN_ID_BIT_OFFSET (0)
// CAN ID 目标ID比特长度
#define kCAN_ID_TARGET_CAN_ID_LENGTH (8)

typedef union {
    float v;
    uint8_t bytes[4];
    uint32_t bytes_uint32;
} float_memory_t;


void cyber_gear_can_init(cyber_gear_can_t *frame) {
    memset(frame, 0, sizeof(cyber_gear_can_t));
}

void cyber_gear_set_can_id_int_value(cyber_gear_can_t *frame, int bit_start, int bit_length, int value) {
    set_bitfield(value, bit_start, bit_length, frame->can_id, sizeof(frame->can_id));
}

void cyber_gear_can_dump(cyber_gear_can_t *frame) {
    printf("CAN_ID[3,0]: %02X %02X %02X %02X\n\tNUM:%02X%02X%02X%02X\nCAN_DATA[7,0]: %02X %02X %02X %02X %02X %02X %02X %02X\n\tNUM:%02X%02X%02X%02X%02X%02X%02X%02X\n\tcansend: %02X%02X%02X%02X#%02X%02X%02X%02X%02X%02X%02X%02X\n",
           frame->can_id[3], frame->can_id[2], frame->can_id[1], frame->can_id[0],
           frame->can_id[3], frame->can_id[2], frame->can_id[1], frame->can_id[0],
           frame->bytes[7], frame->bytes[6], frame->bytes[5], frame->bytes[4], frame->bytes[3], frame->bytes[2], frame->bytes[1], frame->bytes[0],
           frame->bytes[7], frame->bytes[6], frame->bytes[5], frame->bytes[4], frame->bytes[3], frame->bytes[2], frame->bytes[1], frame->bytes[0],
           frame->can_id[3], frame->can_id[2], frame->can_id[1], frame->can_id[0],
           frame->bytes[7], frame->bytes[6], frame->bytes[5], frame->bytes[4], frame->bytes[3], frame->bytes[2], frame->bytes[1], frame->bytes[0]);
}

void cyber_gear_set_can_id_communation_type(cyber_gear_can_t *frame, cyber_gear_can_communication_type_t type) {
    cyber_gear_set_can_id_int_value(frame, kCAN_ID_COMM_TYPE_BIT_OFFSET, kCAN_ID_COMM_TYPE_LENGTH, (int)type);
}

void cyber_gear_set_can_id_host_can_id(cyber_gear_can_t *frame, int value) {
    cyber_gear_set_can_id_int_value(frame, kCAN_ID_HOST_CAN_ID_BIT_OFFSET, kCAN_ID_HOST_CAN_ID_LENGTH, value);
}

void cyber_gear_set_can_id_target_can_id(cyber_gear_can_t *frame, int value) {
    cyber_gear_set_can_id_int_value(frame, kCAN_ID_TARGET_CAN_ID_BIT_OFFSET, kCAN_ID_TARGET_CAN_ID_LENGTH, value);
}

void cyber_gear_build_parameter_write_frame_with_int_value(cyber_gear_can_t *frame, cyber_gear_parameter_index_t index, int value) {
    cyber_gear_set_can_id_communation_type(frame, COMMUNICATION_WRITE_SINGLE_PARAM);
    
    eightbyte_set_bitfield((int)index, 0, 16, (uint64_t *)frame->bytes);
    eightbyte_set_bitfield(value, 32, 32, (uint64_t *)frame->bytes);
}

void cyber_gear_build_parameter_write_frame_with_float_value(cyber_gear_can_t *frame, cyber_gear_parameter_index_t index, float value) {
    float_memory_t v;
    v.v = value;
    
    cyber_gear_set_can_id_communation_type(frame, COMMUNICATION_WRITE_SINGLE_PARAM);
    eightbyte_set_bitfield((int)index, 0, 16, (uint64_t *)frame->bytes);
    eightbyte_set_bitfield(v.bytes_uint32, 32, 32, (uint64_t *)frame->bytes);
}
