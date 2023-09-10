//
// Created by Eric Wu on 2023/9/7.
//

import Foundation
import CCyberGearProtocol
import RxRelay
import RxSwift

extension CyberGear {
    public final class Motor {
        public enum RunMode: Int {
            case unknown = -1
            case motionControl = 0
            case location = 1
            case speed = 2
            case current = 3
        }

        public let canID: Int

        public let motorStaus: BehaviorRelay<CyberGear.Frame.MotorStatus> = BehaviorRelay(value: .empty)

        private var parameterCache: [CyberGear.Frame.ParameterIndex: CyberGear.Frame.ParameterRead] = [:]

        private var cacheLock: NSLock = NSLock()

        public var onParameterUpdate: PublishRelay<CyberGear.Frame.ParameterRead> = PublishRelay()

        private let canBus: CANBus

        private var readFrameDisposeBag: DisposeBag = DisposeBag()

        private let parser: CyberGear.Frame.Parser

        public let currentRunMode: BehaviorRelay<RunMode> = BehaviorRelay(value: .unknown)

        public init(canID: Int, bus: CANBus) {
            self.canID = canID
            self.canBus = bus
            self.parser = CyberGear.Frame.Parser(hostID: bus.hostID, targetID: canID)

            attachCANBus(bus)
        }
    }
}

extension CyberGear.Motor {
    private func attachCANBus(_ bus: CyberGear.CANBus) {
        readFrameDisposeBag = DisposeBag()

        parser.attachFeeder(bus.onCANFrameRead)

        parser.onMotorStatusPublish.bind(to: motorStaus)
                .disposed(by: readFrameDisposeBag)

        parser.onMotorParameterReadPublish.bind(to: onParameterUpdate)
                .disposed(by: readFrameDisposeBag)

        onParameterUpdate.subscribe { [weak self] (v: CyberGear.Frame.ParameterRead) in
            guard let self else {
                return
            }
            self.cacheLock.lock()
            self.parameterCache[v.index] = v
            self.cacheLock.unlock()
        }.disposed(by: readFrameDisposeBag)

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

    public func setZeroPosition() throws {
        if let frame = CyberGear.Frame.Builder()
                .hostID(canBus.hostID)
                .motorID(canID)
                .communitaionType(.setMechanicalZeroPosition)
                .build() {
            try canBus.write(frame)
        }
    }


    public func runMode(_ runMode: RunMode) throws {
        self.currentRunMode.accept(runMode)
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

    public func updateParameterCache(_ index: CyberGear.Frame.ParameterIndex) throws {
        if let frame = CyberGear.Frame.Builder()
                .hostID(canBus.hostID)
                .motorID(canID)
                .communitaionType(.readSingleParam)
                .parameterIndex(index)
                .build() {
            try canBus.write(frame)
        }
    }

    public func parameter(_ index: CyberGear.Frame.ParameterIndex) -> Float? {
        cacheLock.lock()
        defer {
            cacheLock.unlock()
        }

        return parameterCache[index]?.floatValue
    }

    public func parameter(_ index: CyberGear.Frame.ParameterIndex) -> Int? {
        cacheLock.lock()
        defer {
            cacheLock.unlock()
        }

        return parameterCache[index]?.intValue
    }
}
