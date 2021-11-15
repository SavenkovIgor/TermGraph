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
    id: root

    readonly property color black         : "#000000"
    readonly property color white         : "#e8e8e8"
    readonly property color whiteDisabled : "#9c9c9c"
    readonly property color base          : "#332f30"
    readonly property color baseLight     : "#4f4f4f"
    readonly property color baseLight2    : "#464544"
    readonly property color baseLight3    : "#6f6f6f"
    readonly property color accent        : "#2f7ba3"
    readonly property color warning       : "#e5b41f"

    readonly property ColorGroup group : ColorGroup {
        text:          root.white
        base:          root.base
        alternateBase: root.baseLight
        highlight:     root.accent
    }
}
