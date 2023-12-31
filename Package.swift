// swift-tools-version: 5.7
// The swift-tools-version declares the minimum version of Swift required to build this package.

import PackageDescription

let package = Package(
    name: "CyberGearKit",
    products: [
        .library(name: "CyberGearKit", targets: [
            "CyberGearKit"
        ])
    ],
    dependencies: [
        .package(url: "https://github.com/AutomotiveSwift/Swift-SocketCAN.git", branch: "master"),
        .package(url: "https://github.com/ReactiveX/RxSwift.git", branch: "6.6.0"),
        .package(url: "https://github.com/ctreffs/SwiftImGui.git", branch: "1.89.5"),
        .package(url: "https://github.com/CmST0us/AppBootstrapKit.git", branch: "main")
    ],
    targets: [
        .target(name: "CCyberGearProtocol",
                exclude: ["test"]),
        .target(name: "CyberGearKit",
                dependencies: [
                    .target(name: "CCyberGearProtocol"),
                    .product(name: "Swift-SocketCAN", package: "Swift-SocketCAN"),
                    .product(name: "RxSwift", package: "RxSwift"),
                    .product(name: "RxRelay", package: "RxSwift")
                ]),
        .executableTarget(
            name: "CyberGearUtils",
            dependencies: [
                .target(name: "CyberGearKit"),
                .product(name: "ImGui", package: "SwiftImGui"),
                .product(name: "AppBootstrapKit", package: "AppBootstrapKit")
            ]),
        .testTarget(
            name: "CyberGearUtilsTests",
            dependencies: ["CyberGearUtils"]),
    ]
)
