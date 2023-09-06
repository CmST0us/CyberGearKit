//
// Created by Eric Wu on 2023/9/7.
//

import Foundation
import CCyberGearProtocol

extension CyberGear.Frame {
    public enum CommunicationType: Int {
        /// 获取设备 ID (通信类型 0) ;获取设备的 ID 和 64 位 MCU 唯一 标识符
        case fetchDeviceID = 0
        /// 运控模式电机控制指令 (通信类型 1)用来向电机发送控制指令
        case motionControlCommand = 1
        /// 电机反馈数据 (通信类型 2) 用来向主机反馈电机运行状态
        case statusReport = 2
        /// 电机使能运行 (通信类型 3)
        case enableDevice = 3
        /// 电机停止运行 (通信类型 4)
        case disableDevice = 4
        /// 设置电机机械零位(通信类型 6)会把当前电机位置设为机械零 位(掉电丢失)
        case setMechanicalZeroPosition = 6
        /// 设置电机 CANID(通信类型 7)更改当前电机 CANID  立即生 效。
        case setCANId = 7
        /// 单个参数读取(通信类型 17)
        case readSingleParam = 17
        /// 单个参数写入(通信类型 18) (掉电丢失)
        case writeSingleParam = 18
        /// 故障反馈帧(通信类型 21)
        case errorReport = 21

        init(_ value: cyber_gear_can_communication_type_t) {
            self.init(rawValue: Int(value.rawValue))!
        }

        var c_enum: cyber_gear_can_communication_type_t {
            return cyber_gear_can_communication_type_t(rawValue: UInt32(self.rawValue))
        }
    }
}