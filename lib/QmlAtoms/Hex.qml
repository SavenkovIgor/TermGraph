// Copyright © 2016-2023. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick
import QtQuick.Shapes

Shape {
    id: hex
    width: 100
    height: width
    anchors.centerIn: parent

    property alias color: shp.fillColor
    property alias borderColor: shp.strokeColor

    ShapePath {
        id: shp
        strokeColor: Qt.darker(shp.fillColor, 1.15)

        readonly property point center: Qt.point(hex.width / 2, hex.height / 2)
        readonly property real radius: hex.width / 2
        readonly property point pt1: Qt.point(Math.cos(0 * (Math.PI / 3)), Math.sin(0 * (Math.PI / 3)))
        readonly property point pt2: Qt.point(Math.cos(1 * (Math.PI / 3)), Math.sin(1 * (Math.PI / 3)))
        readonly property point pt3: Qt.point(Math.cos(2 * (Math.PI / 3)), Math.sin(2 * (Math.PI / 3)))
        readonly property point pt4: Qt.point(Math.cos(3 * (Math.PI / 3)), Math.sin(3 * (Math.PI / 3)))
        readonly property point pt5: Qt.point(Math.cos(4 * (Math.PI / 3)), Math.sin(4 * (Math.PI / 3)))
        readonly property point pt6: Qt.point(Math.cos(5 * (Math.PI / 3)), Math.sin(5 * (Math.PI / 3)))

        PathMove { x: shp.pt1.x * shp.radius + shp.center.x; y: shp.pt1.y * shp.radius + shp.center.y; }
        PathLine { x: shp.pt2.x * shp.radius + shp.center.x; y: shp.pt2.y * shp.radius + shp.center.y; }
        PathLine { x: shp.pt3.x * shp.radius + shp.center.x; y: shp.pt3.y * shp.radius + shp.center.y; }
        PathLine { x: shp.pt4.x * shp.radius + shp.center.x; y: shp.pt4.y * shp.radius + shp.center.y; }
        PathLine { x: shp.pt5.x * shp.radius + shp.center.x; y: shp.pt5.y * shp.radius + shp.center.y; }
        PathLine { x: shp.pt6.x * shp.radius + shp.center.x; y: shp.pt6.y * shp.radius + shp.center.y; }
        PathLine { x: shp.pt1.x * shp.radius + shp.center.x; y: shp.pt1.y * shp.radius + shp.center.y; }
    }
}
