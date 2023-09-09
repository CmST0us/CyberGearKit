import CyberGearKit
import Foundation
import RxSwift
import RxRelay
import AppBootstrapKit
import ImGui

public class CyberGearUtilsApp: Application {
    private let canBus: CyberGear.CANBus

    private let motor: CyberGear.Motor

    private let disposeBag: DisposeBag = DisposeBag()

    var torqueAssistance: Bool = false

    var motorSpeed: Float = 0

    var motorCurrent: Float = 0

    var motorLocation: Float = 0

    var motorSpeedLimit: Float = 0

    var motorSpeedValueBuffer: [Float] = []

    var speedValueLock: NSLock = NSLock()

    var motorTorqueValueBuffer: [Float] = []

    var torqueValueLock: NSLock = NSLock()

    var motorLocationValueBuffer: [Float] = []

    var motorLocationValueLock: NSLock = NSLock()

    private func appendSpeedValue(_ value: Float) {
        speedValueLock.lock()
        defer {
            speedValueLock.unlock()
        }
        motorSpeedValueBuffer.append(value)
        if motorSpeedValueBuffer.count > 256 {
            motorSpeedValueBuffer.removeFirst()
        }
    }

    private func appendTorqueValue(_ value: Float) {
        torqueValueLock.lock()
        defer {
            torqueValueLock.unlock()
        }
        motorTorqueValueBuffer.append(value)
        if motorTorqueValueBuffer.count > 256 {
            motorTorqueValueBuffer.removeFirst()
        }
    }

    private func appendLocationValue(_ value: Float) {
        motorLocationValueLock.lock()
        defer {
            motorLocationValueLock.unlock()
        }
        motorLocationValueBuffer.append(value)
        if motorLocationValueBuffer.count > 256 {
            motorLocationValueBuffer.removeFirst()
        }
    }

    public override init(configuration: Application.Configuration) {
        let canBus = CyberGear.CANBus(hostID: 1)
        let motor = CyberGear.Motor(canID: 127, bus: canBus)
        self.canBus = canBus
        self.motor = motor

        super.init(configuration: configuration)
    }

    public override func setup() {
        motor.motorStaus
                .observe(on: SerialDispatchQueueScheduler(internalSerialQueueName: "statue report"))
                .subscribe { [weak self] (v: CyberGearKit.CyberGear.Frame.MotorStatus) in
                    self?.appendSpeedValue(v.currentSpeed)
                    self?.appendLocationValue(v.currentLocation)
                    self?.appendTorqueValue(v.currentTorque)
                }
                .disposed(by: disposeBag)
    }

    private func onStartMotor() {

    }

    private func onStopMotor() {

    }

    public override func update() {
        ImGuiBegin("CyberGearUtils", nil, 0)
        ImGuiSetWindowSize(ImVec2(x: 720, y: 480), 0)

        ImGuiColumns(2, "Start Stop", true)
        if ImGuiButton("Start", ImVec2(x: 100, y: 50)) {
            try? canBus.open()
            try? motor.runMode(.speed)
            try? motor.enable()
            onStartMotor()
        }

        ImGuiNextColumn()

        if ImGuiButton("Stop", ImVec2(x: 100, y: 50)) {
            try? motor.disable()
            onStopMotor()
        }
        ImGuiEndColumns()


        ImGuiColumns(3, "Run Mode", true)
        if ImGuiButton("Speed Mode", ImVec2(x: 100, y: 50)) {
            try? motor.runMode(.speed)
        }

        ImGuiNextColumn()

        if ImGuiButton("Location Mode", ImVec2(x: 100, y: 50)) {
            try? motor.runMode(.location)
        }

        ImGuiNextColumn()

        if ImGuiButton("Current Mode", ImVec2(x: 100, y: 50)) {
            try? motor.runMode(.current)
        }
        ImGuiEndColumns()

        ImGuiColumns(2, "Control And Plot", true)

        if motor.currentRunMode.value == .location {
            if ImGuiButton("Set Zero Position", ImVec2(x: 100, y: 50)) {
                try? motor.setZeroPosition()
            }
        }

        switch motor.currentRunMode.value {
            case .current:
                ImGuiVSliderFloat("Current", ImVec2(x: 100, y: 250), &self.motorCurrent, -1.0, 1.0, nil, 0)

                ImGuiCheckbox("Torque Assistance", &self.torqueAssistance)
                if (self.torqueAssistance) {
                    if self.motor.motorStaus.value.currentSpeed > 0 {
                        try? motor.setTargetCurrent(abs(self.motorCurrent))
                    } else {
                        try? motor.setTargetCurrent(-abs(self.motorCurrent))
                    }
                } else {
                    if motor.motorStaus.value.mode == .motor {
                        try? motor.setTargetCurrent(self.motorCurrent)
                    }
                }
            case .location:
                ImGuiVSliderFloat("Location", ImVec2(x: 100, y: 250), &self.motorLocation, -Float.pi, Float.pi, nil, 0)
                if motor.motorStaus.value.mode == .motor {
                    try? motor.setTargetLocation(self.motorLocation)
                }
                ImGuiSameLine(0, 24)
                ImGuiVSliderFloat("SpeedLimit", ImVec2(x: 100, y: 250), &self.motorSpeedLimit, 0, 30, nil, 0)
                if motor.motorStaus.value.mode == .motor {
                    try? motor.setSpeedLimit(self.motorSpeedLimit)
                }
            case .speed:
                ImGuiVSliderFloat("Speed", ImVec2(x: 100, y: 250), &self.motorSpeed, -30.0, 30.0, nil, 0)
                if motor.motorStaus.value.mode == .motor {
                    try? motor.setTargetSpeed(self.motorSpeed)
                }
            default:
                break
        }

        ImGuiNextColumn()

        self.motorLocationValueLock.lock()
        ImGuiPlotLines("Location", &self.motorLocationValueBuffer, Int32(self.motorLocationValueBuffer.count), 0, nil, -Float.pi, Float.pi, ImVec2(x: 200, y: 100), 4)
        self.motorLocationValueLock.unlock()

        self.speedValueLock.lock()
        ImGuiPlotLines("Speed", &self.motorSpeedValueBuffer, Int32(self.motorSpeedValueBuffer.count), 0, nil, -30, 30, ImVec2(x: 200, y: 100), 4)
        self.speedValueLock.unlock()

        self.torqueValueLock.lock()
        ImGuiPlotLines("Torque", &self.motorTorqueValueBuffer, Int32(self.motorTorqueValueBuffer.count), 0, nil, -12, 12, ImVec2(x: 200, y: 100), 4)
        self.torqueValueLock.unlock()

        ImGuiEndColumns()

        ImGuiEnd()
    }
}

@main
public struct CyberGearUtils {
    public static func main() {
        let config = Application.Configuration(pipeline: .sdl2_imgui, openglVersion: .gles2_glsl100,
                windowName: "CyberGearUtils",
                clearColor: (0.1, 0.1, 0.1, 1.0))
        let app = CyberGearUtilsApp(configuration: config)
        app.run()
    }
}
