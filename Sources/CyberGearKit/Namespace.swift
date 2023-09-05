//
// Created by Eric Wu on 2023/9/4.
//

import Foundation
import CCyberGearProtocol

public enum CyberGear {
    // Namespace of CyberGearKit
}

extension CyberGear {
    public static func test() {
        var c = cyber_gear_can_t()
        c.can_id = .init(value: 0x12345678)
        c.can_data = .init(value: 0x1234567812345678)
        cyber_gear_can_dump(&c)
    }
}
