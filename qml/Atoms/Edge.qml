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

import QtQuick 2.15
import QtQuick.Shapes 1.15

ShapePath {
    id: root

    property point pt1: Qt.point(0, 0)
    property point pt2: Qt.point(0, 0)
    property color edgeColor: "black"

    readonly property real halfWidth: (pt2.x - pt1.x) / 2;
    readonly property point b1: Qt.point(pt1.x + halfWidth, pt1.y)
    readonly property point b2: Qt.point(pt2.x - halfWidth, pt2.y)

    startX: pt1.x; startY: pt1.y;

    strokeWidth: Qt.platform.os === "Android" ? 3 : 1
    strokeColor: edgeColor
    fillColor: "transparent"

    PathCubic {
        x: root.pt2.x; y: root.pt2.y;
        control1X: b1.x; control1Y: b1.y;
        control2X: b2.x; control2Y: b2.y;
    }
}
