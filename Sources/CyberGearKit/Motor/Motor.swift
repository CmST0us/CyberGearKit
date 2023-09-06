//
// Created by Eric Wu on 2023/9/7.
//

import Foundation
import CCyberGearProtocol

extension CyberGear {
    public final class Motor {
        public let canID: Int
        private let canBus: CANBus

        public init(canID: Int, bus: CANBus) {
            self.canID = canID
            self.canBus = bus
        }
    }
}

extension CyberGear.Motor {
    public func enable() {

    }
}
