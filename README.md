# CyberGearKit

![logo](Asset/logo.svg)

CyberGearKit is a framework of communicating to xiaomi CyberGear.


# How to integrate

This is an SPM-compliant Swift Package: First, add the following line in Package.swift to your package dependencies:

```swift
.package(url: "https://github.com/CmST0us/CyberGearKit.git", branch: "master")
```
Then, add the module CyberGearKit – where necessary – to your target dependencies.

# Usage

Create `CyberGear.CANBus` to communicate with socketcan interface

```swift
let canBus = CyberGear.CANBus(hostID: 1)
do {
    try canBus.open()
} catch {
    // do some catch
}
```

Create `CyberGear.Motor` to control motor. For example, you can make motor run in speed mode.

```swift
let motor = CyberGear.Motor(canID: 127, bus: canBus)
try! motor.runMode(.speed)
try! motor.enable()
try! motor.setTargetSpeed(5)
```

Use `RxSwift` to listen frame parse

```swift
let disposeBag = DisposeBag()
motor.motorStaus
    .observe(on: MainScheduler.asyncInstance)
    .subscribe { (v: CyberGearKit.CyberGear.Frame.MotorStatus) in
        // do some thing
    }
    .disposed(by: disposeBag)
```

# License and Contributions
This package is licensed under the term of the MIT License. Contributions are always welcome!