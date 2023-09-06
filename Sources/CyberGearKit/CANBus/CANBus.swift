//
// Created by Eric Wu on 2023/9/7.
//

import Foundation
import Swift_SocketCAN

extension CyberGear {
    public final class CANBus {
        let hostID: Int

        private let socketCAN: SocketCAN

        deinit {
            self.socketCAN.close()
        }

        public init(hostID: Int, socketCANInterfaceName: String = "can0") {
            self.hostID = hostID
            self.socketCAN = SocketCAN(iface: socketCANInterfaceName)
        }

        public func open(bitrate: Int = 0) throws {
            try self.socketCAN.open(bitrate: bitrate)
        }

        public func close() {
            self.socketCAN.close()
        }

        public func read(timeout: Int = 0) throws -> CAN.Frame {
            try self.socketCAN.read(timeout: timeout)
        }

        public func write(_ frame: CAN.Frame) throws {
            try self.socketCAN.write(frame)
        }
    }
}