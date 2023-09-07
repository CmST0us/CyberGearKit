import CyberGearKit
import Foundation
import RxSwift
import RxRelay

@main
public struct CyberGearUtils {
    public static func main() {
        let disposeBag = DisposeBag()

        let canBus = CyberGear.CANBus(hostID: 1)
        let motor = CyberGear.Motor(canID: 127, bus: canBus)
        do {
            try canBus.open()
            try motor.runMode(.location)
            try motor.enable()

            DispatchQueue.global().async {
                var c: Float = 0
                while true {
                    let v = sin(c) * 30
                    c += 0.01
                    Thread.sleep(forTimeInterval: 0.01)
                    try! motor.setTargetLocation(v)
                }
            }

            motor.motorStaus
                    .observe(on: MainScheduler.asyncInstance)
                    .subscribe { (v: CyberGearKit.CyberGear.Frame.MotorStatus) in
                        print("""
                              -------------------------------------
                              |电机模式:     |    \(v.mode)
                              |力矩:        |    \(v.currentTorque)
                              |目标角度:     |    \(v.currentLocation)
                              |角速度:       |    \(v.currentSpeed)
                              |温度:         |    \(v.currentTemperature)
                              -------------------------------------
                              """)
                    }
                    .disposed(by: disposeBag)
        } catch {
            print(error)
        }
        RunLoop.main.run()
    }
}
