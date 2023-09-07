//
// Created by Eric Wu on 2023/9/7.
//

import Foundation
import CCyberGearProtocol

extension CyberGear.Frame {
    public enum ParameterIndex: Int {
        /// 运控模式
        /// 0: 运控模式 1: 位置模式 2: 速度模式 3: 电流模式
        /// 参数类型 uint8_t
        /// 参数字节数 1
        /// 参数说明
        case runMode = 0x7005
        /// 电流模式 Iq 指令
        /// 参数类型 float
        /// 参数字节数 4
        /// 参数说明 -27~27A
        case iqRef = 0x7006
        /// 转速模式转速指令
        /// 参数类型 float
        /// 参数字节数 4
        /// 参数说明 -30~30rad/s
        case spdRef = 0x700A
        /// 转矩限制
        /// 参数类型 float
        /// 参数字节数 4
        /// 参数说明 0~12Nm
        case limitTorque = 0x700B
        /// 电流的 Kp
        /// 参数类型 float
        /// 参数字节数 4
        /// 参数说明 默认值 0.125
        case curKp = 0x7010
        /// 电流的 Ki
        /// 参数类型 float
        /// 参数字节数 4
        /// 参数说明 默认值 0.0158
        case curKi = 0x7011
        /// 电流滤波系数 filt_gain
        /// 参数类型 float
        /// 参数字节数 4
        /// 参数说明 默认值 0~1.0，默认值 W/R 0.1
        case curFiltGain = 0x7014
        /// 位置模式角度指令
        /// 参数类型 float
        /// 参数字节数 4
        /// 参数说明 rad
        case locRef = 0x7016
        /// 位置模式速度设置
        /// 参数类型 float
        /// 参数字节数 4
        /// 参数说明 0~30rad/s
        case limitSpd = 0x7017
        /// 速度位置模式电流设置
        /// 参数类型 float
        /// 参数字节数 4
        /// 参数说明 0~27A
        case limitCur = 0x7018

        init(_ value: cyber_gear_read_write_parameter_index_t) {
            self.init(rawValue: Int(value.rawValue))!
        }

        var c_enum: cyber_gear_read_write_parameter_index_t {
        return cyber_gear_read_write_parameter_index_t(rawValue: UInt32(self.rawValue))
        }
    }
}