//
// Created by Eric Wu on 2023/9/7.
//

import Foundation
import CCyberGearProtocol
import Swift_SocketCAN

extension CyberGear.Frame {
    public final class Builder {
        private var motorID: Int?
        private var hostID: Int?
        private var communicationType: CommunicationType?
        private var settingMotorID: Int?

        private var parameterIndex: ParameterIndex?
        private var parameterFloatValue: Float?
        private var parameterIntValue: Int?

        private var motionControl: MotionControl?

        static private var communicationTypeBuilderMap: [CommunicationType: ((Builder) -> CAN.Frame?)] = [
            .motionControlCommand: buildMotionControlFrame,
            .enableDevice: buildEnableDeviceFrame,
            .disableDevice: buildDisableDeviceFrame,
            .setMechanicalZeroPosition: buildSetMechanicalZeroPositionFrame,
            .setCANId: buildSetCANIDFrame,
            .readSingleParam: buildReadSingleParamFrame,
            .writeSingleParam: buildWriteSingleParamFrame
        ]

        public init() {

        }
    }
}

extension CyberGear.Frame.Builder {
    public func build() -> CAN.Frame? {
        // 创建一个libcybergear can frame
        var cyberGearCANFrame = cyber_gear_can_t()
        cyber_gear_can_init(&cyberGearCANFrame)

        // 拿到communicationType
        guard let communicationType else {
            return nil
        }

        // 用 communicationType 查找 builder
        if let builder = Self.communicationTypeBuilderMap[communicationType] {
            return builder(self)
        }

        return nil
    }

    @discardableResult
    public func clear() -> Self {
        motorID = nil
        hostID = nil
        communicationType = nil
        parameterIndex = nil
        parameterFloatValue = nil
        parameterIntValue = nil
        return self
    }
}

extension CyberGear.Frame.Builder {
    private static func buildMotionControlFrame(builder: CyberGear.Frame.Builder) -> CAN.Frame? {
        var cyberGearFrame = cyber_gear_can_t()
        cyber_gear_can_init(&cyberGearFrame)

        guard let motorID = builder.motorID,
            let motionControl = builder.motionControl else {
            return nil
        }
        cyber_gear_set_can_id_target_can_id(&cyberGearFrame, Int32(motorID))

        var controlPack = cyber_gear_motion_control_t()
        controlPack.motor_can_id = UInt8(motionControl.motorCANID)
        controlPack.target_location = motionControl.targetLocation
        controlPack.target_speed = motionControl.targetSpeed
        controlPack.kp = motionControl.kp
        controlPack.kd = motionControl.kd
        controlPack.torque = motionControl.torque

        cyber_gear_build_motion_control_frame(
                &cyberGearFrame,
                controlPack)

        return CAN.Frame(id: cyberGearFrame.can_id.value, unpadded: CyberGear.Utils.tuple8ToArray(cyberGearFrame.can_data.bytes))
    }

    private static func buildEnableDeviceFrame(builder: CyberGear.Frame.Builder) -> CAN.Frame? {
        var cyberGearFrame = cyber_gear_can_t()
        cyber_gear_can_init(&cyberGearFrame)

        guard let motorID = builder.motorID,
              let hostID = builder.hostID else {
            return nil
        }
        cyber_gear_set_can_id_target_can_id(&cyberGearFrame, Int32(motorID))
        cyber_gear_set_can_id_host_can_id(&cyberGearFrame, Int32(hostID))

        cyber_gear_set_can_id_communication_type(&cyberGearFrame, CyberGear.Frame.CommunicationType.enableDevice.c_enum)
        return CAN.Frame(id: cyberGearFrame.can_id.value, unpadded: CyberGear.Utils.tuple8ToArray(cyberGearFrame.can_data.bytes))
    }

    private static func buildDisableDeviceFrame(builder: CyberGear.Frame.Builder) -> CAN.Frame? {
        var cyberGearFrame = cyber_gear_can_t()
        cyber_gear_can_init(&cyberGearFrame)

        guard let motorID = builder.motorID,
              let hostID = builder.hostID else {
            return nil
        }
        cyber_gear_set_can_id_target_can_id(&cyberGearFrame, Int32(motorID))
        cyber_gear_set_can_id_host_can_id(&cyberGearFrame, Int32(hostID))

        cyber_gear_set_can_id_communication_type(&cyberGearFrame, CyberGear.Frame.CommunicationType.disableDevice.c_enum)
        return CAN.Frame(id: cyberGearFrame.can_id.value, unpadded: CyberGear.Utils.tuple8ToArray(cyberGearFrame.can_data.bytes))
    }

    private static func buildSetMechanicalZeroPositionFrame(builder: CyberGear.Frame.Builder) -> CAN.Frame? {
        var cyberGearFrame = cyber_gear_can_t()
        cyber_gear_can_init(&cyberGearFrame)

        guard let motorID = builder.motorID,
              let hostID = builder.hostID else {
            return nil
        }
        cyber_gear_set_can_id_target_can_id(&cyberGearFrame, Int32(motorID))
        cyber_gear_set_can_id_host_can_id(&cyberGearFrame, Int32(hostID))

        cyber_gear_set_can_id_communication_type(&cyberGearFrame, CyberGear.Frame.CommunicationType.setMechanicalZeroPosition.c_enum)
        cyberGearFrame.can_data.bytes.0 = 1
        return CAN.Frame(id: cyberGearFrame.can_id.value, unpadded: CyberGear.Utils.tuple8ToArray(cyberGearFrame.can_data.bytes))
    }

    private static func buildSetCANIDFrame(builder: CyberGear.Frame.Builder) -> CAN.Frame? {
        var cyberGearFrame = cyber_gear_can_t()
        cyber_gear_can_init(&cyberGearFrame)

        guard let motorID = builder.motorID,
              let hostID = builder.hostID,
              let settingMotorID = builder.settingMotorID else {
            return nil
        }
        cyber_gear_set_can_id_target_can_id(&cyberGearFrame, Int32(motorID))
        cyber_gear_set_can_id_host_can_id(&cyberGearFrame, Int32(hostID))

        cyber_gear_build_set_can_id_frame(&cyberGearFrame, Int32(settingMotorID));
        return CAN.Frame(id: cyberGearFrame.can_id.value, unpadded: CyberGear.Utils.tuple8ToArray(cyberGearFrame.can_data.bytes))
    }

    private static func buildReadSingleParamFrame(builder: CyberGear.Frame.Builder) -> CAN.Frame? {
        var cyberGearFrame = cyber_gear_can_t()
        cyber_gear_can_init(&cyberGearFrame)

        guard let motorID = builder.motorID,
              let hostID = builder.hostID,
              let index = builder.parameterIndex else {
            return nil
        }
        cyber_gear_set_can_id_target_can_id(&cyberGearFrame, Int32(motorID))
        cyber_gear_set_can_id_host_can_id(&cyberGearFrame, Int32(hostID))

        cyber_gear_build_parameter_read_frame(&cyberGearFrame, index.c_enum)
        return CAN.Frame(id: cyberGearFrame.can_id.value, unpadded: CyberGear.Utils.tuple8ToArray(cyberGearFrame.can_data.bytes))
    }

    private static func buildWriteSingleParamFrame(builder: CyberGear.Frame.Builder) -> CAN.Frame? {
        var cyberGearFrame = cyber_gear_can_t()
        cyber_gear_can_init(&cyberGearFrame)

        guard let motorID = builder.motorID,
            let hostID = builder.hostID,
            let parameterIndex = builder.parameterIndex else {
            return nil
        }
        cyber_gear_set_can_id_target_can_id(&cyberGearFrame, Int32(motorID))
        cyber_gear_set_can_id_host_can_id(&cyberGearFrame, Int32(hostID))

        if let intValue = builder.parameterIntValue {
            cyber_gear_build_parameter_write_frame_with_int_value(&cyberGearFrame, parameterIndex.c_enum, Int32(intValue))
        } else if let floatValue = builder.parameterFloatValue {
            cyber_gear_build_parameter_write_frame_with_float_value(&cyberGearFrame, parameterIndex.c_enum, floatValue)
        }

        return CAN.Frame(id: cyberGearFrame.can_id.value, unpadded: CyberGear.Utils.tuple8ToArray(cyberGearFrame.can_data.bytes))
    }
}

extension CyberGear.Frame.Builder {
    @discardableResult
    public func hostID(_ value: Int) -> Self {
        self.hostID = value
        return self
    }

    @discardableResult
    public func motorID(_ value: Int) -> Self {
        self.motorID = value
        return self
    }

    @discardableResult
    public func communitaionType(_ value: CyberGear.Frame.CommunicationType) -> Self {
        self.communicationType = value
        return self
    }

    @discardableResult
    public func parameterIndex(_ value: CyberGear.Frame.ParameterIndex) -> Self {
        self.parameterIndex = value
        return self
    }

    @discardableResult
    public func parameterIntValue(_ value: Int) -> Self {
        self.parameterIntValue = value
        return self
    }

    @discardableResult
    public func parameterFloatValue(_ value: Float) -> Self {
        self.parameterFloatValue = value
        return self
    }

    @discardableResult
    public func motionControl(_ value: CyberGear.Frame.MotionControl) -> Self {
        self.motionControl = value
        return self
    }

    @discardableResult
    public func settingMotorID(_ value: Int) -> Self {
        self.settingMotorID = value
        return self
    }
}