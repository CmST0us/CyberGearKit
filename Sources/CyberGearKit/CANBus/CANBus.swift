//
// Created by Eric Wu on 2023/9/7.
//

import Foundation
import Swift_SocketCAN
import RxSwift
import RxRelay

extension CyberGear {
    public final class CANBus {
        let hostID: Int

        private let socketCAN: SocketCAN

        let onCANFrameRead: Observable<CAN.Frame>

        let isBusOpen: BehaviorRelay<Bool> = BehaviorRelay(value: false)

        private let _onCANFrameRead: PublishRelay<CAN.Frame>

        private let frameReadQueue: DispatchQueue

        deinit {
            self.socketCAN.close()
        }

        public init(hostID: Int, socketCANInterfaceName: String = "can0") {
            self.hostID = hostID
            self.socketCAN = SocketCAN(iface: socketCANInterfaceName)

            _onCANFrameRead = PublishRelay()
            onCANFrameRead = _onCANFrameRead.asObservable()
            frameReadQueue = DispatchQueue(label: "HOST \(hostID) READ QUEUE")
        }

        public func open(bitrate: Int = 0) throws {
            try self.socketCAN.open(bitrate: bitrate)

            isBusOpen.accept(true)
            startReadService()
        }

        public func close() {
            self.socketCAN.close()

            isBusOpen.accept(false)
        }

        public func read(timeout: Int = 0) throws -> CAN.Frame {
            try self.socketCAN.read(timeout: timeout)
        }

        public func write(_ frame: CAN.Frame) throws {
            try self.socketCAN.write(frame)
        }
    }
}

extension CyberGear.CANBus {
    private func doReadFrame() {
        guard isBusOpen.value else {
            return
        }

        if let frame = try? socketCAN.read() {
            _onCANFrameRead.accept(frame)
        }

        frameReadQueue.async { [weak self] in
            self?.doReadFrame()
        }
    }

    private func startReadService() {
        frameReadQueue.async { [weak self] in
            self?.doReadFrame()
        }
    }
}