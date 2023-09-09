//
// Created by Eric Wu on 2023/9/4.
//

#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <limits.h>
#include <stdlib.h>

#include <libcybergear/cyber_gear_protocol.h>
#include "utils.h"
#include "float16.h"

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

#define kM_PI (3.14159265358979323846264338327950288)

typedef union {
    float v;
    uint8_t bytes[4];
    uint32_t bytes_uint32;
} float_memory_t;

typedef union {
    uint64_t v;
    uint8_t bytes[4];
} uint64_memory_t;

static float map(float x, float in_min, float in_max, float out_min, float out_max) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}


void cyber_gear_can_init(const cyber_gear_can_t *frame) {
    memset((void *)frame, 0, sizeof(cyber_gear_can_t));
}

void cyber_gear_set_can_id_int_value(const cyber_gear_can_t *frame, int bit_start, int bit_length, int value) {
    bit_value_t setting_value = {0};
    setting_value.value = value;
    bit_utils_set_value(setting_value,
                        bit_start,
                        bit_length,
                        (uint8_t *)frame->can_id.bytes,
                        sizeof(frame->can_id.bytes));
}

int cyber_gear_get_can_id_int_value(const cyber_gear_can_t * const frame, int bit_start, int bit_length) {
    bit_value_t value = {0};
    memcpy(value.bytes, frame->can_id.bytes, sizeof(frame->can_id.bytes));
    
    bit_value_t read_value = bit_utils_get_value(value,
                                                 sizeof(frame->can_id.bytes),
                                                 bit_start,
                                                 bit_length);
    return (int)read_value.value;
}

void cyber_gear_can_dump(const cyber_gear_can_t * const frame) {
    printf("CAN_ID[3,0]: %02X %02X %02X %02X\n\tNUM:%02X%02X%02X%02X\nCAN_DATA[7,0]: %02X %02X %02X %02X %02X %02X %02X %02X\n\tNUM:%02X%02X%02X%02X%02X%02X%02X%02X\n\tcansend: cansend can0 %02X%02X%02X%02X#%02X%02X%02X%02X%02X%02X%02X%02X\n",
           frame->can_id.bytes[3], frame->can_id.bytes[2], frame->can_id.bytes[1], frame->can_id.bytes[0],
           frame->can_id.bytes[3], frame->can_id.bytes[2], frame->can_id.bytes[1], frame->can_id.bytes[0],
           frame->can_data.bytes[0], frame->can_data.bytes[1], frame->can_data.bytes[2], frame->can_data.bytes[3], frame->can_data.bytes[4], frame->can_data.bytes[5], frame->can_data.bytes[6], frame->can_data.bytes[7],
           frame->can_data.bytes[0], frame->can_data.bytes[1], frame->can_data.bytes[2], frame->can_data.bytes[3], frame->can_data.bytes[4], frame->can_data.bytes[5], frame->can_data.bytes[6], frame->can_data.bytes[7],
           frame->can_id.bytes[3], frame->can_id.bytes[2], frame->can_id.bytes[1], frame->can_id.bytes[0],
           frame->can_data.bytes[0], frame->can_data.bytes[1], frame->can_data.bytes[2], frame->can_data.bytes[3], frame->can_data.bytes[4], frame->can_data.bytes[5], frame->can_data.bytes[6], frame->can_data.bytes[7]);
}

void cyber_gear_set_can_id_communication_type(const cyber_gear_can_t *frame, cyber_gear_can_communication_type_t type) {
    cyber_gear_set_can_id_int_value(frame, kCAN_ID_COMM_TYPE_BIT_OFFSET, kCAN_ID_COMM_TYPE_LENGTH, (int)type);
}

void cyber_gear_set_can_id_host_can_id(const cyber_gear_can_t *frame, int value) {
    cyber_gear_set_can_id_int_value(frame, kCAN_ID_HOST_CAN_ID_BIT_OFFSET, kCAN_ID_HOST_CAN_ID_LENGTH, value);
}

void cyber_gear_set_can_id_target_can_id(const cyber_gear_can_t *frame, int value) {
    cyber_gear_set_can_id_int_value(frame, kCAN_ID_TARGET_CAN_ID_BIT_OFFSET, kCAN_ID_TARGET_CAN_ID_LENGTH, value);
}

void cyber_gear_build_motion_control_frame(const cyber_gear_can_t *frame, const cyber_gear_motion_control_t control_param) {
    cyber_gear_set_can_id_target_can_id(frame, control_param.motor_can_id);
    cyber_gear_set_can_id_communication_type(frame, COMMUNICATION_MOTION_CONTROL_COMMAND);
    
    uint16_t torque = (uint16_t)map(control_param.torque, -12, 12, 0, 65535);
    uint16_t location = (uint16_t)map(control_param.target_location, -4 * kM_PI, 4 * kM_PI, 0, 65535);
    uint16_t speed = (uint16_t)map(control_param.target_speed, -30, 30, 0, 65535);
    uint16_t kp = (uint16_t)map(control_param.kp, 0, 500, 0, 65535);
    uint16_t kd = (uint16_t)map(control_param.kd, 0, 5, 0, 65535);
    
    bit_value_t setting_value = {0};
    setting_value.value = bit_utils_swap_host_endian_value_into_little_endian16(location);
    
    bit_utils_set_value(setting_value,
                        0 * CHAR_BIT, 2 * CHAR_BIT,
                        (uint8_t *)frame->can_data.bytes,
                        sizeof(frame->can_data.bytes));
    
    setting_value.value = bit_utils_swap_host_endian_value_into_little_endian16(speed);
    bit_utils_set_value(setting_value,
                        2 * CHAR_BIT, 2 * CHAR_BIT,
                        (uint8_t *)frame->can_data.bytes,
                        sizeof(frame->can_data.bytes));
    
    setting_value.value = bit_utils_swap_host_endian_value_into_little_endian16(kp);
    bit_utils_set_value(setting_value,
                        4 * CHAR_BIT, 2 * CHAR_BIT,
                        (uint8_t *)frame->can_data.bytes,
                        sizeof(frame->can_data.bytes));
    
    setting_value.value = bit_utils_swap_host_endian_value_into_little_endian16(kd);
    bit_utils_set_value(setting_value,
                        6 * CHAR_BIT, 2 * CHAR_BIT,
                        (uint8_t *)frame->can_data.bytes,
                        sizeof(frame->can_data.bytes));
    
    setting_value.value = bit_utils_swap_host_endian_value_into_little_endian16(torque);
    bit_utils_set_value(setting_value,
                        8, 2 * CHAR_BIT,
                        (uint8_t *)frame->can_id.bytes,
                        sizeof(frame->can_id.bytes));
}

void cyber_gear_build_parameter_write_frame_with_int_value(const cyber_gear_can_t *frame, cyber_gear_read_write_parameter_index_t index, int value) {
    cyber_gear_set_can_id_communication_type(frame, COMMUNICATION_WRITE_SINGLE_PARAM);
    
    bit_value_t setting_value = {0};
    setting_value.value = bit_utils_swap_host_endian_value_into_little_endian16(index);
    bit_utils_set_value(setting_value,
                        0,
                        16,
                        (uint8_t *)frame->can_data.bytes,
                        sizeof(frame->can_data.bytes));
    
    setting_value.value = bit_utils_swap_host_endian_value_into_little_endian32(value);
    bit_utils_set_value(setting_value,
                        32,
                        32,
                        (uint8_t *)frame->can_data.bytes,
                        sizeof(frame->can_data.bytes));
    
}

void cyber_gear_build_parameter_write_frame_with_float_value(const cyber_gear_can_t *frame, cyber_gear_read_write_parameter_index_t index, float value) {
    float_memory_t v;
    v.v = value;
    
    cyber_gear_set_can_id_communication_type(frame, COMMUNICATION_WRITE_SINGLE_PARAM);
    
    bit_value_t setting_value = {0};
    setting_value.value = bit_utils_swap_host_endian_value_into_little_endian16(index);
    
    bit_utils_set_value(setting_value,
                        0,
                        16,
                        (uint8_t *)frame->can_data.bytes,
                        sizeof(frame->can_data.bytes));
    
    setting_value.value = bit_utils_swap_host_endian_value_into_little_endian32(v.bytes_uint32);
    bit_utils_set_value(setting_value,
                        32,
                        32,
                        (uint8_t *)frame->can_data.bytes,
                        sizeof(frame->can_data.bytes));
}

void cyber_gear_build_parameter_read_frame(const cyber_gear_can_t *frame, cyber_gear_read_write_parameter_index_t index) {
    cyber_gear_set_can_id_communication_type(frame, COMMUNICATION_READ_SINGLE_PARAM);
    
    bit_value_t setting_value = {0};
    setting_value.value = bit_utils_swap_host_endian_value_into_little_endian16(index);
    
    bit_utils_set_value(setting_value,
                        0 * CHAR_BIT, 2 * CHAR_BIT,
                        (uint8_t *)frame->can_data.bytes,
                        sizeof(frame->can_data.bytes));
}

cyber_gear_single_parameter_t cyber_gear_parse_parameter_read_frame(const cyber_gear_can_t *frame) {
    cyber_gear_can_communication_type_t comm_type = cyber_gear_get_can_id_communication_type(frame);
    assert(comm_type == COMMUNICATION_READ_SINGLE_PARAM);
    
    int host_id = cyber_gear_get_can_id_host_id(frame);
    int target_id = cyber_gear_get_can_id_target_id(frame);
    
    bit_value_t source = {0};
    memcpy(source.bytes, frame->can_data.bytes, sizeof(frame->can_data.bytes));
    
    bit_value_t index_raw = bit_utils_get_value(source,
                                                sizeof(frame->can_data.bytes),
                                                0 * CHAR_BIT, 2 * CHAR_BIT);
    
    int index = bit_utils_swap_little_endian_value_into_host_endian16(index_raw.value);
    
    cyber_gear_read_write_parameter_index_t param_index = index;
    
    bit_value_t param_raw = bit_utils_get_value(source,
                                                sizeof(frame->can_data.bytes),
                                                4 * CHAR_BIT, 4 * CHAR_BIT);
    
    bit32_value_t param_raw_32 = {0};
    param_raw_32.value = bit_utils_swap_little_endian_value_into_host_endian32((uint32_t)param_raw.value);
    
    cyber_gear_single_parameter_t result = {0};
    result.index = param_index;
    result.motor_can_id = target_id;
    result.host_can_id = host_id;
    result.data.value = param_raw_32.value;
    
    return result;
}

cyber_gear_can_communication_type_t cyber_gear_get_can_id_communication_type(const cyber_gear_can_t * const frame) {
    return (cyber_gear_can_communication_type_t)cyber_gear_get_can_id_int_value(frame, 24, 5);
}

int cyber_gear_get_can_id_target_id(const cyber_gear_can_t * const frame) {
    return cyber_gear_get_can_id_int_value(frame, 0, 8);
}

int cyber_gear_get_can_id_host_id(const cyber_gear_can_t * const frame) {
    return cyber_gear_get_can_id_int_value(frame, 8, 8);
}

static int16_t rebound_value(int16_t input, int max) {
    if (input == 0) {
        return 0;
    }
    int16_t abs_value = abs(input);
    int sign_value= abs_value / input;
    return (abs_value - max) * sign_value;
}

cyber_gear_motor_status_t cyber_gear_parse_motor_status_frame(const cyber_gear_can_t * const frame) {
    cyber_gear_can_communication_type_t comm_type = cyber_gear_get_can_id_communication_type(frame);
    assert(comm_type == COMMUNICATION_STATUS_REPORT);
    
    bit_value_t source = {0};
    memcpy(source.bytes, frame->can_data.bytes, sizeof(frame->can_data.bytes));
    
    bit_value_t location_raw = bit_utils_get_value(source,
                                                   sizeof(frame->can_data.bytes),
                                                   0 * CHAR_BIT,
                                                   2 * CHAR_BIT);
    bit16_value_t location_raw_16_bit = {
        .value = location_raw.value
    };
    int16_t location_value = (int16_t)bit_utils_swap_big_endian_value_into_host_endian16(location_raw_16_bit.value);
    float location = map((int16_t)rebound_value(location_value, 32768), -32768.0, 32767.0, -4 * kM_PI, 4 * kM_PI);
    
    bit_value_t speed_raw = bit_utils_get_value(source,
                                                sizeof(frame->can_data.bytes),
                                                2 * CHAR_BIT,
                                                2 * CHAR_BIT);
    bit16_value_t speed_raw_16_bit = {
        .value = speed_raw.value
    };
    int16_t speed_value = (int16_t)bit_utils_swap_big_endian_value_into_host_endian16(speed_raw_16_bit.value);
    float speed = map((int16_t)rebound_value(speed_value, 32768), -32768.0, 32767.0, -30, 30);
    
    bit_value_t torque_raw = bit_utils_get_value(source,
                                                 sizeof(frame->can_data.bytes),
                                                 4 * CHAR_BIT,
                                                 2 * CHAR_BIT);
    bit16_value_t torque_raw_16_bit = {
        .value = torque_raw.value
    };
    int16_t torque_value = (int16_t)bit_utils_swap_big_endian_value_into_host_endian16(torque_raw_16_bit.value);
    float torque = map((int16_t)rebound_value(torque_value, 32768), -32768.0, 32767.0, -12, 12);
    
    bit_value_t temperature_raw = bit_utils_get_value(source,
                                                      sizeof(frame->can_data.bytes),
                                                      6 * CHAR_BIT,
                                                      2 * CHAR_BIT);
    bit16_value_t temperature_raw_16_bit = {
        .value = temperature_raw.value
    };
    float temperature = bit_utils_swap_big_endian_value_into_host_endian16(temperature_raw_16_bit.value) / 10.0;
    
    cyber_gear_motor_status_t motor_status = {
        .motor_can_id = cyber_gear_get_can_id_target_id(frame),
        .host_can_id = cyber_gear_get_can_id_int_value(frame, 8, 8),
        .current_location = location,
        .current_speed = speed,
        .current_torque = torque,
        .current_temperature = temperature,
        .has_calibration_error = cyber_gear_get_can_id_int_value(frame, 21, 1),
        .has_hall_encode_error = cyber_gear_get_can_id_int_value(frame, 20, 1),
        .has_magnetic_encoding_error = cyber_gear_get_can_id_int_value(frame, 19, 1),
        .has_over_temperature = cyber_gear_get_can_id_int_value(frame, 18, 1),
        .has_over_current = cyber_gear_get_can_id_int_value(frame, 17, 1),
        .has_undervoltage = cyber_gear_get_can_id_int_value(frame, 16, 1),
        .mode_type = cyber_gear_get_can_id_int_value(frame, 22, 2),
    };
    
    return motor_status;
}

void cyber_gear_build_set_mechanical_zero_position_frame(const cyber_gear_can_t * frame) {
    cyber_gear_set_can_id_communication_type(frame, COMMUNICATION_SET_MECHANICAL_ZERO_POSITION);

    uint8_t *bytes = (uint8_t *)frame->can_data.bytes;
    bytes[0] = 1;
}

void cyber_gear_build_set_can_id_frame(const cyber_gear_can_t * frame, int setting_can_id) {
    cyber_gear_set_can_id_communication_type(frame, COMMUNICATION_SET_CAN_ID);

    bit_value_t setting_value = {0};
    setting_value.value = bit_utils_swap_host_endian_value_into_little_endian16(setting_can_id);

    bit_utils_set_value(setting_value,
                        16, 8,
                        (uint8_t *)frame->can_id.bytes,
                        sizeof(frame->can_id.bytes));
}

void cyber_gear_dump_motor_status_frame(const cyber_gear_motor_status_t status) {
    printf("[MOTOR STATUS]:\n\thost can id %d\n\tmotor can id %d\n\ttorque %f\n\tlocation %f\n\tspeed %f\n\ttemp %f\n", status.host_can_id, status.motor_can_id, status.current_torque, status.current_location, status.current_speed, status.current_temperature);
}
