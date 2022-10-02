// Copyright © 2016-2022. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

pragma Singleton

import QtQuick

QtObject {
    readonly property bool isMobile: Qt.platform.os === "android" || Qt.platform.os === "ios"

    readonly property real mm: Screen.pixelDensity
    readonly property real cm: Screen.pixelDensity * 10

    // Radial millimeter
    // Because of different user view distance for desktop monitor and mobile phone
    // elements, that has equal linear size would differ visually.
    // So radial millimeter helps to make ui elements feel more "equal" from user point of view
    readonly property real rm: (isMobile ? 1 : 2) * mm

    readonly property real halfBase : rm
    readonly property real base     : 2 * rm
    readonly property real baseX2   : 4 * rm
    readonly property real baseX3   : 6 * rm
    readonly property real baseX4   : 8 * rm
}
