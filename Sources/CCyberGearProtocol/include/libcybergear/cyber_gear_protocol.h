//
// Created by Eric Wu on 2023/9/4.
//

#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "visibility.h"

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
    /// Name 电机名称
    /// 参数类型 String
    CONFIG_WR_NAME = 0x0000,
    /// BarCode
    /// 参数类型 String
    CONFIG_R_BAR_CODE = 0x0001,
    /// BootCodeVersion
    /// 参数类型 String
    CONFIG_R_BOOT_CODE_VERSION = 0x1000,
    /// BootBuildDate
    /// 参数类型 String
    CONFIG_R_BOOT_BUILD_DATE = 0x1001,
    /// BootBuildTime
    /// 参数类型 String
    CONFIG_R_BOOT_BUILD_TIME = 0x1002,
    /// AppCodeVersion
    /// 参数类型 String
    CONFIG_R_APP_CODE_VERSION = 0x1003,
    /// AppGitVersion
    /// 参数类型 String
    CONFIG_R_APP_GIT_VERSION = 0x1004,
    /// AppBuildDate
    /// 参数类型 String
    CONFIG_R_APP_BUILD_DATE = 0x1005,
    /// AppBuildTime
    /// 参数类型
    CONFIG_R_APP_BUILD_TIME = 0x1006,
    /// AppCodeName
    /// 参数类型
    CONFIG_R_APP_CODE_NAME = 0x1007,
    /// echoPara1
    /// 参数类型
    CONFIG_R_ECHO_PARA1 = 0x2000,
    /// echoPara2
    /// 参数类型
    CONFIG_R_ECHO_PARA2 = 0x2001,
    /// echoPara3
    /// 参数类型
    CONFIG_R_ECHO_PARA3 = 0x2002,
    /// echoPara4
    /// 参数类型
    CONFIG_R_ECHO_PARA4 = 0x2003,
    /// echoFreHz
    /// 参数类型
    CONFIG_WR_ECHO_FRE_HZ = 0x2004,
    /// MechOffset 电机磁编码器角度偏置
    /// 参数类型 float [-7, 7]
    CONFIG_R_MECH_OFFSET = 0x2005,
    /// MechPos_init 初始多圈时的参考角度
    /// 参数类型 float [-50, 50]
    CONFIG_WR_MECH_POS_INIT = 0x2006,
    /// limit_torque 转矩限制
    /// 参数类型 float [0, 12]
    CONFIG_WR_LIMIT_TORQUE = 0x2007,
    /// I_FW_MAX 弱磁电 流值，默认0
    /// 参数类型 float [0, 33]
    CONFIG_WR_I_FW_MAX = 0x2008,
    /// motor_index 电机 index，标记电机关节位置
    /// 参数类型 uint8_t [0, 20]
    CONFIG_WR_MOTOR_INDEX = 0x2009,
    /// CAN_ID 本节点 id
    /// 参数类型 uint8_t [0, 127]
    CONFIG_WR_CAN_ID = 0x200a,
    /// CAN_MASTER can 主机 id
    /// 参数类型 uint8_t [0, 127]
    CONFIG_WR_CAN_MASTER = 0x200b,
    /// CAN_TIMEOUT can 超时 阈值，默 认0
    /// 参数类型 uint32_t [0, 10000]
    CONFIG_WR_CAN_TIMEOUT = 0x200c,
    /// motorOverTemp 电机保护温度值，temp (度) *10
    /// 参数类型 uint16_t [0, 1500]
    CONFIG_WR_MOTOR_OVER_TEMP = 0x200d,
    /// overTempTime 过温时间
    /// 参数类型 uint32_t [0, 100000]
    CONFIG_WR_OVER_TEMP_TIME = 0x200e,
    /// GearRatio 传动比
    /// 参数类型 float [1, 64]
    CONFIG_WR_GEAR_RATIO = 0x200f,
    /// Tq_caliType 转矩标定方法设定
    /// 参数类型 uint8_t 0, 1
    CONFIG_WR_TQ_CALI_TYPE = 0x2010,
    /// cur_filt_gain 电流滤波参数
    /// 参数类型 float [0, 1]
    CONFIG_WR_CUR_FILT_GAIN = 0x2011,
    /// cur_kp 电流 kp
    /// 参数类型 float [0, 200]
    CONFIG_WR_CUR_KP = 0x2012,
    /// cur_ki 电流 ki
    /// 参数类型 float [0, 200]
    CONFIG_WR_CUR_KI = 0x2013,
    /// spd_kp 速度 kp
    /// 参数类型 float [0, 200]
    CONFIG_WR_SPD_KP = 0x2014,
    /// spd_ki 速度 ki
    /// 参数类型 float [0, 200]
    CONFIG_WR_SPD_KI = 0x2015,
    /// loc_kp 位置 kp
    /// 参数类型 float [0, 200]
    CONFIG_WR_LOC_KP = 0x2016,
    /// spd_filt_gain 速度滤波参数
    /// 参数类型 float [0, 1]
    CONFIG_WR_SPD_FILT_GAIN = 0x2017,
    /// limit_spd 位置环速度限制
    /// 参数类型 float [0, 200]
    CONFIG_WR_LIMIT_SPD = 0x2018,
    /// limit_cur 位置速度控制电流限制
    /// 参数类型 float [0, 27]
    CONFIG_WR_LIMIT_CUR = 0x2019,
    /// timeUse0
    /// 参数类型 uint16_t
    CONFIG_R_TIME_USE0 = 0x3000,
    /// timeUse1
    /// 参数类型 uint16_t
    CONFIG_R_TIME_USE1 = 0x3001,
    /// timeUse1
    /// 参数类型 uint16_t
    CONFIG_R_TIME_USE2 = 0x3002,
    /// timeUse1
    /// 参数类型 uint16_t
    CONFIG_R_TIME_USE3 = 0x3003,
    /// encoderRaw 磁编码器采样值
    /// 参数类型 uint16_t
    CONFIG_R_ENCODER_RAW = 0x3004,
    /// mcuTemp mcu 内部 温度，*10
    /// 参数类型 uint16_t
    CONFIG_R_MCU_TEMP = 0x3005,
    /// motorTemp 电机 ntc 温度， *10
    /// 参数类型 uint16_t
    CONFIG_R_MOTOR_TEMP = 0x3006,
    /// vBus(mv) 母线电 压
    /// 参数类型 uint16_t
    CONFIG_R_VBUS_MV = 0x3007,
    /// adc1Offset adc 采样 通道1 零电流 偏置
    /// 参数类型 int32_t
    CONFIG_R_ADC1_OFFSET = 0x3008,
    /// adc2Offset adc 采样 通道2 零电流 偏置
    /// 参数类型 int32_t
    CONFIG_R_ADC2_OFFSET = 0x3009,
    /// adc1Raw adc 采样 值1
    /// 参数类型 uint32_t
    CONFIG_R_ADC1_RAW = 0x300a,
    /// adc2Raw adc 采样 值2
    /// 参数类型 uint32_t
    CONFIG_R_ADC2_RAW = 0x300b,
    /// VBUS 母线电 压V
    /// 参数类型 float
    CONFIG_R_VBUS_V = 0x300c,
    /// cmdId id环指令，A
    /// 参数类型 float
    CONFIG_R_CMD_ID = 0x300d,
    /// cmdIq iq 环指令，A
    /// 参数类型 float
    CONFIG_R_CMD_IQ = 0x300e,
    /// cmdlocref 位置环指令， rad
    /// 参数类型 float
    CONFIG_R_CMD_LOC_REF = 0x300f,
    /// cmdspdref 速度环指令， rad/s
    /// 参数类型 float
    CONFIG_R_CMD_SPD_REF = 0x3010,
    /// cmdTorque 转矩指令，nm
    /// 参数类型 float
    CONFIG_R_CMD_TORQUE = 0x3011,
    /// cmdPos mit 协议 角度指令
    /// 参数类型 float
    CONFIG_R_CMD_POS = 0x3012,
    /// cmdVel mit 协议速度指令
    /// 参数类型 float
    CONFIG_R_CMD_VEL = 0x3013,
    /// rotation 圈数
    /// 参数类型 int16_t
    CONFIG_R_ROTATION = 0x3014,
    /// modPos 电机未计圈机械角度， rad
    /// 参数类型 float
    CONFIG_R_MOD_POS = 0x3015,
    /// mechPos 负载端计圈机械角度， rad
    /// 参数类型 float
    CONFIG_R_MECH_POS = 0x3016,
    /// mechVel 负载端转速,rad/ s
    /// 参数类型 float
    CONFIG_R_MECH_VEL = 0x3017,
    /// elecPos 电气角度
    /// 参数类型 float
    CONFIG_R_ELEC_POS = 0x3018,
    /// ia U线电流，A
    /// 参数类型 float
    CONFIG_R_IA = 0x3019,
    /// ib V线电流，A
    /// 参数类型 float
    CONFIG_R_IB = 0x301a,
    /// ic W线电流，A
    /// 参数类型 float
    CONFIG_R_IC = 0x301b,
    /// tick
    /// 参数类型 uint32_t
    CONFIG_R_TICK = 0x301c,
    /// phaseOrder 标定方向标记
    /// 参数类型 uint8_t
    CONFIG_R_PHASE_ORDER = 0x301d,
    /// iqf iq 滤波值，A
    /// 参数类型 float
    CONFIG_R_IQF = 0x301e,
    /// boardTemp 板上温度，*10
    /// 参数类型 int16_t
    CONFIG_R_BOARD_TEMP = 0x301f,
    /// iq iq 原值， A
    /// 参数类型 float
    CONFIG_R_IQ = 0x3020,
    /// id id 原值， A
    /// 参数类型 float
    CONFIG_R_ID = 0x3021,
    /// faultSta 故障状态值
    /// 参数类型 uint32_t
    CONFIG_R_FAULT_STATUS = 0x3022,
    /// warnSta 警告状态值
    /// 参数类型 uint32_t
    CONFIG_R_WARN_STATUS = 0x3023,
    /// drv_fault 驱动芯片故障值
    /// 参数类型 uint16_t
    CONFIG_R_DRV_FAULT = 0x3024,
    /// drv_temp 驱动芯片温度值，度
    /// 参数类型 int16_t
    CONFIG_R_DRV_TEMP = 0x3025,
    /// Uq q 轴电压
    /// 参数类型 float
    CONFIG_R_UQ = 0x3026,
    /// Ud d 轴电压
    /// 参数类型 float
    CONFIG_R_UD = 0x3027,
    /// dtc_u U 相输出 占空比
    /// 参数类型 float
    CONFIG_R_DTC_U = 0x3028,
    /// dtc_v V 相输出 占空比
    /// 参数类型 float
    CONFIG_R_DTC_V = 0x3029,
    /// dtc_w W 相输出 占空比
    /// 参数类型 float
    CONFIG_R_DTC_W = 0x302a,
    /// v_bus 闭环中 vbus
    /// 参数类型
    CONFIG_R_CLOSED_LOOP_V_BUS = 0x302b,
    /// v_ref 闭环 vq,vd 合 成电压
    /// 参数类型 float
    CONFIG_R_CLOSED_LOOP_V_REF = 0x302c,
    /// torque_fdb 转矩反 馈值，nm
    /// 参数类型 float
    CONFIG_R_TORQUE_FDB = 0x302d,
    /// rated_i 电机额 定电流
    /// 参数类型 float
    CONFIG_R_RATED_I = 0x302e,
    /// limit_i 电机限 制最大 电流
    /// 参数类型 float
    CONFIG_R_LIMIT_I = 0x302f,
} cyber_gear_config_index_t;

typedef enum {
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
} cyber_gear_read_write_parameter_index_t;


/*
 * CyberGear CAN 包定义
 */
typedef struct {
    union {
        uint32_t value;
        uint8_t bytes[4];
    } can_id; // CAN ID, 29位有效数据
  
    union {
        uint64_t value;
        uint8_t bytes[8];
    } can_data; // 数据区, 8字节
} CYBERGEARAPI cyber_gear_can_t;

/*
 * CyberGear 通信模式 1 发送控制指令
 */
typedef struct {
    uint8_t motor_can_id; // 目标电机 CAN ID
    float target_location; // 目标角度 (-4π~4π)
    float target_speed; // 目标角速度 (-30rad/s~30rad/s)
    float kp; // Kp (0.0~500.0)
    float kd; // Kd (0.0~5.0)
    float torque; // 力矩对应 (-12Nm~12Nm)
} cyber_gear_motion_control_t;

/*
 * CyberGear 通信模式 2 电机反馈数据
 */
typedef enum {
    MOTOR_MODE_RESET, // 复位模式
    MOTOR_MODE_CALI, // 标定模式
    MOTOR_MODE_MOTOR // 运行模式
} cyber_gear_motor_mode_t;

typedef struct {
    uint8_t host_can_id; // 主机 CAN_ID
    uint8_t motor_can_id; // 目标电机 CAN ID
    float current_torque; // 力矩 [-12, 12] 单位 N/m
    float current_location; // 当前角度 [-4pi, 4pi]
    float current_speed; // 当前角速度 [-30rad/s, 30rad/s]
    float current_temperature; // 当前温度:Temp(摄氏度) * 10
    int has_calibration_error; // 标定错误
    int has_hall_encode_error; // HALL 编码故障
    int has_magnetic_encoding_error; // 磁编码错误
    int has_over_temperature; // 过温故障
    int has_over_current; // 过流故障
    int has_undervoltage; // 欠压故障
    cyber_gear_motor_mode_t mode_type; // 运行模式
} cyber_gear_motor_status_t;

/*
 * CyberGear 通信模式 17 单个参数读取
 */
typedef struct {
    uint8_t host_can_id; // 主机 CAN_ID
    uint8_t motor_can_id; // 目标电机 CAN ID
    
    cyber_gear_read_write_parameter_index_t index; // 参数索引
    
    union {
        uint32_t value;
        uint8_t bytes[4];
    } data; // 参数的数值
} cyber_gear_single_parameter_t;

/* 初始化一个 CyberGear 的 CAN 帧 */
CYBERGEARAPI void cyber_gear_can_init(const cyber_gear_can_t *frame);

/*  Dump 一个 CyberGear 的 CAN 帧 */
CYBERGEARAPI void cyber_gear_can_dump(const cyber_gear_can_t * const frame);

/* 对 CAN ID 区域特定的比特位进行赋值整数
 * @param: frame 要设置的帧
 * @param: bit_start 比特开始位
 * @param: bit_length 比特长度
 * @param: value 设置值(整数)
 * */
CYBERGEARAPI void cyber_gear_set_can_id_int_value(const cyber_gear_can_t *frame, int bit_start, int bit_length, int value);

/* 获取 CAN ID 区域特定的比特位数据
 * @param: frame 要设置的帧
 * @param: bit_start 比特开始位
 * @param: bit_length 比特长度
 * @return: int 值数据
 * */
CYBERGEARAPI int cyber_gear_get_can_id_int_value(const cyber_gear_can_t *frame, int bit_start, int bit_length);

/* 对 CAN ID 设置通讯类型
 * @param: frame 要设置的帧
 * @param: type 通信类型
 * */
CYBERGEARAPI void cyber_gear_set_can_id_communication_type(const cyber_gear_can_t *frame, cyber_gear_can_communication_type_t type);

/* 对 CAN ID 设置主机CANID
 * @param: frame 要设置的帧
 * @param: value 主机CANID
 * */
CYBERGEARAPI void cyber_gear_set_can_id_host_can_id(const cyber_gear_can_t *frame, int value);

/* 对 CAN ID 设置目标电机CANID
 * @param: frame 要设置的帧
 * @param: value 电机CANID
 * */
CYBERGEARAPI void cyber_gear_set_can_id_target_can_id(const cyber_gear_can_t *frame, int value);


/* 构造一个参数写入的CAN包 (通信类型18), 参数值为整数
 * @param: frame 要设置的帧
 * @param: index 参数index
 * @param: value 参数值
 * */
CYBERGEARAPI void cyber_gear_build_parameter_write_frame_with_int_value(const cyber_gear_can_t *frame, cyber_gear_read_write_parameter_index_t index, int value);


/* 构造一个参数写入的CAN包 (通信类型18), 参数值为浮点
 * @param: frame 要设置的帧
 * @param: index 参数index
 * @param: value 参数值
 * */
CYBERGEARAPI void cyber_gear_build_parameter_write_frame_with_float_value(const cyber_gear_can_t *frame, cyber_gear_read_write_parameter_index_t index, float value);

/* 构造一个参数读取的CAN包 （通信类型17）
 * @param: frame 要设置的帧
 */
CYBERGEARAPI void cyber_gear_build_parameter_read_frame(const cyber_gear_can_t *frame, cyber_gear_read_write_parameter_index_t index);

/* 解析一个参数读取的CAN包 （通信类型17）
 * @param: frame 要设置的帧
 */
CYBERGEARAPI cyber_gear_single_parameter_t cyber_gear_parse_parameter_read_frame(const cyber_gear_can_t *frame);

/* 获取帧的通信类型
 * @param: frame 解析的帧
 * @return: cyber_gear_can_communication_type_t 通信类型
 * */
CYBERGEARAPI cyber_gear_can_communication_type_t cyber_gear_get_can_id_communication_type(const cyber_gear_can_t * const frame);

/* 获取帧的目标CAN_ID
 * @param: frame 要设置的帧
 * @return: int 目标 CAN_ID
 * */
CYBERGEARAPI int cyber_gear_get_can_id_target_id(const cyber_gear_can_t * const frame);

/* 获取帧的主机CAN_ID
 * @param: frame 要设置的帧
 * @return: int 主机 CAN_ID
 * */
CYBERGEARAPI int cyber_gear_get_can_id_host_id(const cyber_gear_can_t * const frame);

/* 运控模式电机控制指令 (通信类型 1)用来向电机发送控制指令
 * @param: control_param 控制参数
 * */
CYBERGEARAPI void cyber_gear_build_motion_control_frame(const cyber_gear_can_t *frame, const cyber_gear_motion_control_t control_param);

/* 解析通信类型2 电机运行状态帧
 * @param: frame 要解析的帧
 * @return: cyber_gear_can_communication_type_t 通信类型
 * */
CYBERGEARAPI cyber_gear_motor_status_t cyber_gear_parse_motor_status_frame(const cyber_gear_can_t * const frame);

/* 解析通信类型6 构建一个 位置机械零位帧
 * @param: frame 要解析的帧
 * @return: cyber_gear_can_communication_type_t 通信类型
 * */
CYBERGEARAPI void cyber_gear_build_set_mechanical_zero_position_frame(const cyber_gear_can_t * frame);


/* 解析通信类型7 构建一个 设置 CAN ID 帧
 * @param: frame 要解析的帧
 * @return: cyber_gear_can_communication_type_t 通信类型
 * */
CYBERGEARAPI void cyber_gear_build_set_can_id_frame(const cyber_gear_can_t * frame, int setting_can_id);

/*  Dump 一个 CyberGear 的 电机运行状态帧 帧 */
CYBERGEARAPI void cyber_gear_dump_motor_status_frame(const cyber_gear_motor_status_t status);


