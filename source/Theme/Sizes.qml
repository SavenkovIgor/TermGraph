// Copyright © 2016-2025. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

pragma Singleton

import QtQuick

QtObject {
    // Radial millimeter
    // Because of different user view distance for desktop monitor and mobile phone
    // elements, that has equal linear size would differ visually.
    // So radial millimeter helps to make ui elements feel more "equal" from user point of view
    readonly property real rm: 2 * Screen.pixelDensity

    readonly property real halfBase : rm
    readonly property real base     : 2 * rm
    readonly property real baseX2   : 4 * rm
    readonly property real baseX3   : 6 * rm
    readonly property real baseX4   : 8 * rm
}
