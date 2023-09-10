//
// Created by Eric Wu on 2023/9/7.
//

import Foundation
import RxSwift
import RxRelay
import Swift_SocketCAN
import CCyberGearProtocol

extension cyber_gear_can_t {
    init(frame: CAN.Frame) {
        self.init()
        self.can_id.value = frame.id
        self.can_data.bytes = CyberGear.Utils.arrayToTuple8(frame.data)
    }

    var hostID: Int {
        var v = self
        return Int(cyber_gear_get_can_id_host_id(&v))
    }

    var targetID: Int {
        var v = self
        return Int(cyber_gear_get_can_id_target_id(&v))
    }

    var communicationType: CyberGear.Frame.CommunicationType {
        var v = self
        return CyberGear.Frame.CommunicationType(cyber_gear_get_can_id_communication_type(&v))
    }
}

extension CyberGear.Frame {
    final class Parser {

        let targetID: Int
        let hostID: Int
        private var parserDisposeBag: DisposeBag?

        let onMotorStatusPublish: Observable<MotorStatus>
        let _onMotorStatusPublish = PublishRelay<MotorStatus>()

        let onMotorParameterReadPublish: Observable<ParameterRead>
        let _onMotorParameterReadPublish = PublishRelay<ParameterRead>()

        init(hostID: Int, targetID: Int) {
            self.targetID = targetID
            self.hostID = hostID

            onMotorStatusPublish = _onMotorStatusPublish.asObservable()
            onMotorParameterReadPublish = _onMotorParameterReadPublish.asObservable()
        }

        func attachFeeder(_ feeder: Observable<CAN.Frame>) {
            let disposeBag = DisposeBag()
            parserDisposeBag = disposeBag

            let readedCyberGearFrame = feeder.map { frame -> cyber_gear_can_t in
                return cyber_gear_can_t(frame: frame)
            }.share()

            readedCyberGearFrame.filter { frame in
                return frame.targetID == self.hostID &&
                        frame.communicationType == .statusReport
            }.map {
                CyberGear.Frame.Parser.parse($0)
            }.filter {  [weak self] in
                guard let self else {
                    return false
                }
                return $0.motorCANID == self.targetID
            }.bind(to: _onMotorStatusPublish)
            .disposed(by: disposeBag)

            readedCyberGearFrame.filter { frame in
                return frame.targetID == self.hostID &&
                        frame.hostID == self.targetID &&
                        frame.communicationType == .readSingleParam
            }.map {
                CyberGear.Frame.Parser.parse($0)
            }.bind(to: _onMotorParameterReadPublish)
            .disposed(by: disposeBag)
        }
    }
}

extension CyberGear.Frame.Parser {
    private static func parse(_ frame: cyber_gear_can_t) -> CyberGear.Frame.MotorStatus {
        var v = frame
        let status = cyber_gear_parse_motor_status_frame(&v)
        let result = CyberGear.Frame.MotorStatus(
                hostCANID: Int(status.motor_can_id),
                motorCANID: Int(status.host_can_id),
                currentTorque: Float(status.current_torque),
                currentLocation: Float(status.current_location),
                currentSpeed: Float(status.current_speed),
                currentTemperature: Float(status.current_temperature),
                hasCalibrationError: status.has_calibration_error == 1,
                hasHallEncodeError: status.has_hall_encode_error == 1,
                hasMagneticEncodeError: status.has_magnetic_encoding_error == 1,
                hasOverTemperatureError: status.has_over_temperature == 1,
                hasOverCurrentError: status.has_over_current == 1,
                hasUnderVoltageError: status.has_undervoltage == 1,
                mode: CyberGear.Frame.MotorStatus.Mode(rawValue: Int(status.mode_type.rawValue))!)
        return result
    }

    private static func parse(_ frame: cyber_gear_can_t) -> CyberGear.Frame.ParameterRead {
        var v = frame
        let parameter = cyber_gear_parse_parameter_read_frame(&v)
        let result = CyberGear.Frame.ParameterRead(
                hostCANID: Int(parameter.host_can_id),
                motorCANID: Int(parameter.motor_can_id),
                index: CyberGear.Frame.ParameterIndex(parameter.index),
                rawValue: parameter.data.bytes)
        return result
    }
}

