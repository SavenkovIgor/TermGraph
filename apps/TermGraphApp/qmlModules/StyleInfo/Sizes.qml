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

    readonly property int appHeader   : (isMobile ? 6 : 9 ) * Screen.pixelDensity
    readonly property int button      : (isMobile ? 4 : 5 ) * Screen.pixelDensity
    readonly property int capitalText : (isMobile ? 5 : 6 ) * Screen.pixelDensity
    readonly property int colSpace    : (isMobile ? 2 : 4 ) * Screen.pixelDensity
    readonly property int inputLabel  : (isMobile ? 4 : 7 ) * Screen.pixelDensity
    readonly property int inputText   : (isMobile ? 4 : 6 ) * Screen.pixelDensity
    readonly property int roundButton : (isMobile ? 9 : 15) * Screen.pixelDensity
    readonly property int text        : (isMobile ? 4 : 5 ) * Screen.pixelDensity

    readonly property int base     : roundButton
    readonly property int baseX100 : base * 2
    readonly property int baseX75  : base * 1.75
    readonly property int baseX50  : base * 1.5
    readonly property int baseX25  : base * 1.25
    readonly property int baseR25  : base * 0.75
    readonly property int baseR50  : base * 0.5
    readonly property int baseR75  : base * 0.25
}
