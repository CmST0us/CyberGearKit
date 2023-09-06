//
// Created by Eric Wu on 2023/9/7.
//

import Foundation

extension CyberGear {
    enum Utils {

    }
}

extension CyberGear.Utils {
    static func tuple8ToArray<Element>(_ tuple: (Element, Element, Element, Element,
                                                 Element, Element, Element, Element)) -> [Element] {
        return [tuple.0, tuple.1, tuple.2, tuple.3,
                tuple.4, tuple.5, tuple.6, tuple.7]
    }

    static func tuple4ToArray<Element>(_ tuple: (Element, Element, Element, Element)) -> [Element] {
        return [tuple.0, tuple.1, tuple.2, tuple.3]
    }

    static func tuple2ToArray<Element>(_ tuple: [Element]) -> (Element, Element) {
        return (tuple.0, tuple.1)
    }

    static func arrayToTuple8<Element>(_ tuple: [Element]) -> (Element, Element, Element, Element,
                                                      Element, Element, Element, Element) {
        return (tuple.0, tuple.1, tuple.2, tuple.3,
                tuple.4, tuple.5, tuple.6, tuple.7)
    }

    static func arrayToTuple4<Element>(_ tuple: [Element]) -> (Element, Element, Element, Element) {
        return (tuple.0, tuple.1, tuple.2, tuple.3)
    }

    static func arrayToTuple2<Element>(_ tuple: [Element]) -> (Element, Element) {
        return (tuple.0, tuple.1)
    }
}
