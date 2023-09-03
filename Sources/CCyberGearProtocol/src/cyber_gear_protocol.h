//
// Created by Eric Wu on 2023/9/4.
//

#pragma once

#include <sys/types.h>

#if defined _WIN32 || defined __CYGWIN__
  #ifdef BUILDING_DLL
    #ifdef __GNUC__
      #define CYBERGEARAPI __attribute__ ((dllexport))
    #else
      #define CYBERGEARAPI __declspec(dllexport) // Note: actually gcc seems to also supports this syntax.
    #endif
  #else
    #ifdef __GNUC__
      #define CYBERGEARAPI __attribute__ ((dllimport))
    #else
      #define CYBERGEARAPI __declspec(dllimport) // Note: actually gcc seems to also supports this syntax.
    #endif
  #endif
  #define DLL_LOCAL
#else
  #if __GNUC__ >= 4
    #define CYBERGEARAPI __attribute__ ((visibility ("default")))
    #define CYBERGEARHIDE  __attribute__ ((visibility ("hidden")))
  #else
    #define CYBERGEARAPI
    #define CYBERGEARHIDE
  #endif
#endif


typedef enum {
    /// 获取设备 ID (通信类型 0) ;获取设备的 ID 和 64 位 MCU 唯一 标识符
    COMMUNICATION_FETCH_DEVICE_ID = 0,
    /// 运控模式电机控制指令 (通信类型 1)用来向电机发送控制指令
    COMMUNICATION_MOTION_CONTROL_COMMAND = 1,
    /// 电机反馈数据 (通信类型 2) 用来向主机反馈电机运行状态
    COMMUNICATION_STATUS_REPORT = 2,
    /// 电机使能运行 (通信类型 3)
    COMMUNICATION_ENABLE_DEVICE = 3,
    /// 电机停止运行 (通信类型 4)
    COMMUNICATION_DISABLE_DEVICE = 4,
    /// 设置电机机械零位(通信类型 6)会把当前电机位置设为机械零 位(掉电丢失)
    COMMUNICATION_SET_MECHANICAL_ZERO_POSITION = 6,
    /// 设置电机 CAN_ID(通信类型 7)更改当前电机 CAN_ID , 立即生 效。
    COMMUNICATION_SET_CAN_ID = 7,
    /// 单个参数读取(通信类型 17)
    COMMUNICATION_READ_SINGLE_PARAM = 17,
    /// 单个参数写入(通信类型 18) (掉电丢失)
    COMMUNICATION_WRITE_SINGLE_PARAM = 18,
    /// 故障反馈帧(通信类型 21)
    COMMUNICATION_ERROR_REPORT = 21
} cyber_gear_can_communication_type_t;

typedef enum {
    /* 只读参数 */
    /* [TODO] */
    
    /* 可读写参数 */
    /// 运控模式
    /// 0: 运控模式 1: 位置模式 2: 速度模式 3: 电流模式
    /// 参数类型 uint8_t
    /// 参数字节数 1
    /// 参数说明
    PARAMETER_RUN_MODE = 0x7005,
    /// 电流模式 Iq 指令
    /// 参数类型 float
    /// 参数字节数 4
    /// 参数说明 -27~27A
    PARAMETER_IQ_REF = 0x7006,
    /// 转速模式转速指令
    /// 参数类型 float
    /// 参数字节数 4
    /// 参数说明 -30~30rad/s
    PARAMETER_SPD_REF = 0x700A,
    /// 转矩限制
    /// 参数类型 float
    /// 参数字节数 4
    /// 参数说明 0~12Nm
    PARAMETER_IMIT_TORQUE = 0x700B,
    /// 电流的 Kp
    /// 参数类型 float
    /// 参数字节数 4
    /// 参数说明 默认值 0.125
    PARAMETER_CUR_KP = 0x7010,
    /// 电流的 Ki
    /// 参数类型 float
    /// 参数字节数 4
    /// 参数说明 默认值 0.0158
    PARAMETER_CUR_KI = 0x7011,
    /// 电流滤波系数 filt_gain
    /// 参数类型 float
    /// 参数字节数 4
    /// 参数说明 默认值 0~1.0，默认值 W/R 0.1
    PARAMETER_CUR_FILT_GAIN = 0x7014,
    /// 位置模式角度指令
    /// 参数类型 float
    /// 参数字节数 4
    /// 参数说明 rad
    PARAMETER_LOC_REF = 0x7016,
    /// 位置模式速度设置
    /// 参数类型 float
    /// 参数字节数 4
    /// 参数说明 0~30rad/s
    PARAMETER_LIMIT_SPD = 0x7017,
    /// 速度位置模式电流设置
    /// 参数类型 float
    /// 参数字节数 4
    /// 参数说明 0~27A
    PARAMETER_LIMIT_CUR = 0x7018,
} cyber_gear_parameter_index_t;


/*
 * CyberGear can 包定义
 */
typedef struct {
  uint8_t can_id[4]; // CAN ID, 29位有效数据
  uint8_t bytes[8]; // 数据区, 8字节
} CYBERGEARAPI cyber_gear_can_t;


/* 初始化一个 CyberGear 的 CAN 帧 */
CYBERGEARAPI void cyber_gear_can_init(cyber_gear_can_t *frame);

/*  Dump 一个 CyberGear 的 CAN 帧 */
CYBERGEARAPI void cyber_gear_can_dump(cyber_gear_can_t *frame);

/* 对 CAN ID 区域特定的比特位进行赋值整数
 * @param: frame 要设置的帧
 * @param: bit_start 比特开始位
 * @param: bit_length 比特长度
 * @param: value 设置值(整数)
 * */
CYBERGEARAPI void cyber_gear_set_can_id_int_value(cyber_gear_can_t *frame, int bit_start, int bit_length, int value);

/* 对 CAN ID 设置通讯类型
 * @param: frame 要设置的帧
 * @param: type 通信类型
 * */
CYBERGEARAPI void cyber_gear_set_can_id_communation_type(cyber_gear_can_t *frame, cyber_gear_can_communication_type_t type);

/* 对 CAN ID 设置主机CANID
 * @param: frame 要设置的帧
 * @param: value 主机CANID
 * */
CYBERGEARAPI void cyber_gear_set_can_id_host_can_id(cyber_gear_can_t *frame, int value);

/* 对 CAN ID 设置目标电机CANID
 * @param: frame 要设置的帧
 * @param: value 电机CANID
 * */
CYBERGEARAPI void cyber_gear_set_can_id_target_can_id(cyber_gear_can_t *frame, int value);


/* 构造一个参数写入的CAN包, 参数值为整数
 * @param: frame 要设置的帧
 * @param: index 参数index
 * @param: value 参数值
 * */
CYBERGEARAPI void cyber_gear_build_parameter_write_frame_with_int_value(cyber_gear_can_t *frame, cyber_gear_parameter_index_t index, int value);


/* 构造一个参数写入的CAN包, 参数值为浮点
 * @param: frame 要设置的帧
 * @param: index 参数index
 * @param: value 参数值
 * */
CYBERGEARAPI void cyber_gear_build_parameter_write_frame_with_float_value(cyber_gear_can_t *frame, cyber_gear_parameter_index_t index, float value);
