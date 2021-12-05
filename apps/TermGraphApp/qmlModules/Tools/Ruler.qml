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

import QtQuick

Item {
    id: root

    z: -10
    property bool showGrid: true

    property color barColor: "lightsteelblue"
    property color gridColor: "#44000000"

    readonly property real barStep: Screen.pixelDensity
    readonly property int  barCountWidth:  width  / barStep + 1
    readonly property int  barCountHeight: height / barStep + 1

    component VBar: Rectangle {
        width: 1
        height: 4
        color: root.barColor
    }

    component HBar: Rectangle {
        width: 4
        height: 1
        color: root.barColor
    }

    component BarNumber: Text {
        color: root.barColor
        leftPadding: 2
    }

    // Grid
    Repeater {
        model: root.barCountWidth

        VBar {
            visible: index !== 0
            x: index * root.barStep * 10
            y: 0
            height: root.height
            color: root.gridColor
        }
    }

    Repeater {
        model: root.barCountHeight

        HBar {
            visible: index !== 0
            x: 0
            y: index * root.barStep * 10
            width: root.width
            color: root.gridColor
        }
    }

    // Horizontal ruler
    Repeater {
        model: root.barCountWidth

        VBar {
            visible: index !== 0
            x: index * root.barStep
            y: 0
            height: (index % 10 == 0) ? 20 : (index % 5 == 0) ? 6 : 4
        }
    }

    Repeater {
        model: root.barCountWidth / 10

        BarNumber {
            visible: index !== 0
            x: index * root.barStep * 10
            y: 20 - height
            text: index.toString()
        }
    }

    // Vertical ruler
    Repeater {
        model: root.barCountHeight

        HBar {
            visible: index !== 0
            x: 0
            y: index * root.barStep
            width: (index % 10 == 0) ? 20 : (index % 5 == 0) ? 6 : 4
        }
    }

    Repeater {
        model: root.barCountHeight / 10

        BarNumber {
            visible: index !== 0
            x: 8
            y: index * root.barStep * 10
            text: index.toString()
        }
    }
}
