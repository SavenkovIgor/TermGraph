/*
 *  TermGraph, build graph of knowledge.
 *  Copyright © 2016-2020. Savenkov Igor. All rights reserved
 *  Contacts: dev.savenkovigor@protonmail.com
 *
 *  This file is part of TermGraph.
 *
 *  TermGraph is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  TermGraph is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with TermGraph. If not, see <https://www.gnu.org/licenses/>.
 */

pragma Singleton

import QtQml
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

    readonly property real base   : 2 * rm
    readonly property real baseX2 : 4 * rm
    readonly property real baseX3 : 6 * rm
    readonly property real baseX4 : 8 * rm
}
