//
// Created by Eric Wu on 2023/9/7.
//

import Foundation

extension CyberGear.Frame {
    public struct MotionControl {
        // 目标电机 CAN ID
        public let motorCANID: Int

        // 目标角度 (-4π~4π)
        public let targetLocation: Float

        // 目标角速度 (-30rad/s~30rad/s)
        public let targetSpeed: Float

        // Kp (0.0~500.0)
        public let kp: Float

        // Kd (0.0~5.0)
        public let kd: Float

        // 力矩对应 (-12Nm~12Nm)
        public let torque: Float
    }
}
