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
        .package(url: "https://github.com/AutomotiveSwift/Swift-SocketCAN.git", branch: "master")
    ],
    targets: [
        .target(name: "CCyberGearProtocol",
                exclude: ["test"]),
        .target(name: "CyberGearKit",
                dependencies: [
                    .target(name: "CCyberGearProtocol"),
                    .product(name: "Swift-SocketCAN", package: "Swift-SocketCAN")
                ]),
        .executableTarget(
            name: "CyberGearUtils",
            dependencies: [
                .target(name: "CyberGearKit")
            ]),
        .testTarget(
            name: "CyberGearUtilsTests",
            dependencies: ["CyberGearUtils"]),
    ]
)
