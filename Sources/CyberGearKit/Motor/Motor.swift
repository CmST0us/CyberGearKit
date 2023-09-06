//
// Created by Eric Wu on 2023/9/7.
//

import Foundation
import CCyberGearProtocol

extension CyberGear {
    public final class Motor {
        public enum RunMode: Int {
            case motionControl = 0
            case location = 1
            case speed = 2
            case current = 3
        }

        public let canID: Int
        private let canBus: CANBus

        public init(canID: Int, bus: CANBus) {
            self.canID = canID
            self.canBus = bus
        }
    }
}

extension CyberGear.Motor {
    public func enable() throws {
        if let frame = CyberGear.Frame.Builder()
                .hostID(canBus.hostID)
                .motorID(canID)
                .communitaionType(.enableDevice)
                .build() {
            try canBus.write(frame)
        }
    }

    public func disable() throws {
        if let frame = CyberGear.Frame.Builder()
                .hostID(canBus.hostID)
                .motorID(canID)
                .communitaionType(.disableDevice)
                .build() {
            try canBus.write(frame)
        }
    }

    public func runMode(_ runMode: RunMode) throws {
        if let frame = CyberGear.Frame.Builder()
                .hostID(canBus.hostID)
                .motorID(canID)
                .communitaionType(.writeSingleParam)
                .parameterIndex(.runMode)
                .parameterIntValue(runMode.rawValue)
                .build() {
            try canBus.write(frame)
        }
    }

    public func setTargetSpeed(_ speed: Float) throws {
        if let frame = CyberGear.Frame.Builder()
                .hostID(canBus.hostID)
                .motorID(canID)
                .communitaionType(.writeSingleParam)
                .parameterIndex(.spdRef)
                .parameterFloatValue(speed)
                .build() {
            try canBus.write(frame)
        }
    }

    public func setTargetCurrent(_ current: Float) throws {
        if let frame = CyberGear.Frame.Builder()
                .hostID(canBus.hostID)
                .motorID(canID)
                .communitaionType(.writeSingleParam)
                .parameterIndex(.iqRef)
                .parameterFloatValue(current)
                .build() {
            try canBus.write(frame)
        }
    }

    public func setSpeedLimit(_ speedLimit: Float) throws {
        if let frame = CyberGear.Frame.Builder()
                .hostID(canBus.hostID)
                .motorID(canID)
                .communitaionType(.writeSingleParam)
                .parameterIndex(.limitSpd)
                .parameterFloatValue(speedLimit)
                .build() {
            try canBus.write(frame)
        }
    }

    public func setTargetLocation(_ location: Float) throws {
        if let frame = CyberGear.Frame.Builder()
                .hostID(canBus.hostID)
                .motorID(canID)
                .communitaionType(.writeSingleParam)
                .parameterIndex(.locRef)
                .parameterFloatValue(location)
                .build() {
            try canBus.write(frame)
        }
    }

}
