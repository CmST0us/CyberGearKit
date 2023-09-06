import CyberGearKit
import Foundation
@main
public struct CyberGearUtils {
    public static func main() {
        let builder = CyberGear.Frame.Builder()
        do {
            let frameRunMode = builder.clear()
                    .motorID(127)
                    .hostID(1)
                    .communitaionType(.writeSingleParam)
                    .parameterIndex(.runMode)
                    .parameterIntValue(2)
                    .build()

            let frameEnable = builder.clear()
                    .motorID(127)
                    .hostID(1)
                    .communitaionType(.enableDevice)
                    .build()

            var frameSpeedSet = builder.clear()
                    .motorID(127)
                    .hostID(1)
                    .communitaionType(.writeSingleParam)
                    .parameterIndex(.spdRef)
                    .parameterFloatValue(30)
                    .build()

            let canBus = CyberGear.CANBus(hostID: 1)
            try canBus.open(bitrate: 0)
            print(frameRunMode!)
            try canBus.write(frameRunMode!)

            print(frameEnable!)
            try canBus.write(frameEnable!)

            print(frameSpeedSet!)
            try canBus.write(frameSpeedSet!)

            DispatchQueue.global().async {
                var c: Float = 0
                while true {
                    let v = sin(c) * 30
                    c += 0.01
                    Thread.sleep(forTimeInterval: 0.01)

                    frameSpeedSet = builder.clear()
                            .motorID(127)
                            .hostID(1)
                            .communitaionType(.writeSingleParam)
                            .parameterIndex(.spdRef)
                            .parameterFloatValue(v)
                            .build()
                    try! canBus.write(frameSpeedSet!)
                }
            }
        } catch {
            print(error)
        }
        RunLoop.main.run()
    }
}
