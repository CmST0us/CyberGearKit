import CyberGearKit

@main
public struct CyberGearUtils {
    public private(set) var text = "Hello, World!"

    public static func main() {
        print(CyberGearUtils().text)
        CyberGear.test()
    }
}
