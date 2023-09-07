//
// Created by Eric Wu on 2023/9/7.
//

import Foundation

extension CyberGear.Frame {
    public struct MotorStatus {
        public enum Mode: Int {
            case reset
            case cali
            case motor
        }
        // 主机 CAN_ID
        public let hostCANID: Int

        // 目标电机 CAN ID
        public let motorCANID: Int

        // 力矩 [-12, 12] 单位 N/m
        public let currentTorque: Float

        // 当前角度 [-4pi, 4pi]
        public let currentLocation: Float

        // 当前角速度 [-30rad/s, 30rad/s]
        public let currentSpeed: Float

        // 当前温度:Temp(摄氏度)
        public let currentTemperature: Float

        // 标定错误
        public let hasCalibrationError: Bool

        // HALL 编码故障
        public let hasHallEncodeError: Bool

        // 磁编码错误
        public let hasMagneticEncodeError: Bool

        // 过温故障
        public let hasOverTemperatureError: Bool

        // 过流故障
        public let hasOverCurrentError: Bool

        // 欠压故障
        public let hasUnderVoltageError: Bool

        // 运行模式
        public let mode: Mode

        static var empty: MotorStatus {
            return MotorStatus(hostCANID: 0, motorCANID: 0, currentTorque: 0, currentLocation: 0, currentSpeed: 0,
                    currentTemperature: 0, hasCalibrationError: false, hasHallEncodeError: false,
                    hasMagneticEncodeError: false, hasOverTemperatureError: false, hasOverCurrentError: false,
                    hasUnderVoltageError: false, mode: .reset)
        }
    }
}
