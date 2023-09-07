//
// Created by Eric Wu on 2023/9/7.
//

import Foundation

extension CyberGear.Frame {
    public struct ParameterRead {
        // 主机 CAN_ID
        public let hostCANID: Int

        // 目标电机 CAN ID
        public let motorCANID: Int

        // 参数索引
        public let index: ParameterIndex

        // 参数字节值
        public let rawValue: (UInt8, UInt8, UInt8, UInt8)

        public var intValue: Int {
            let bytes = CyberGear.Utils.tuple4ToArray(rawValue)
            let data = Data(bytes)
            return Int(UInt32(littleEndian: data.withUnsafeBytes { $0.load(as: UInt32.self) }))
        }

        public var floatValue: Float {
            let bytes = CyberGear.Utils.tuple4ToArray(rawValue)
            let data = Data(bytes)
            return Float(bitPattern: UInt32(littleEndian: data.withUnsafeBytes { $0.load(as: UInt32.self) }))
        }
    }
}
