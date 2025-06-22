// Copyright © 2016-2025. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

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
