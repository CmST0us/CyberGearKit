import CyberGearKit
import Foundation
@main
public struct CyberGearUtils {
    public static func main() {
        let canBus = CyberGear.CANBus(hostID: 1)
        let motor = CyberGear.Motor(canID: 127, bus: canBus)
        do {
            try canBus.open()
            try motor.runMode(.speed)
            try motor.enable()

            DispatchQueue.global().async {
                var c: Float = 0
                while true {
                    let v = sin(c) * 30
                    c += 0.01
                    Thread.sleep(forTimeInterval: 0.01)
                    try! motor.setTargetSpeed(v)
                }
            }
        } catch {
            print(error)
        }
        RunLoop.main.run()
    }
}
