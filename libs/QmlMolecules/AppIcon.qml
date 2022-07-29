// Copyright © 2016-2022. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick
import QtQuick.Window
import QtQuick.Controls
// NOTE: Wait for Qt6 implementation
//import QtGraphicalEffects

import StyleInfo
import Atoms as A

Item {
    id: padding

    property int size: 200

    width: size
    height: size

    Item {
        id: root

        width: padding.size * 0.95
        height: padding.size * 0.95
        anchors.centerIn: parent

        property color col1: "#6d9a28"
        property color col2: Colors.accent
        property color col3: Colors.baseLight
        property color colText: "#dcdcdc"

        A.Hex {
            id: hex1
            anchors.centerIn: parent
            width: root.width
            height: root.height
            color: root.col1
            visible: false
        }

// NOTE: Wait for Qt6 implementation
//        DropShadow {
//            anchors.fill: hex1
//            source: hex1
//            verticalOffset: 4
//            horizontalOffset: 4
//            radius: 12.0
//            samples: 20
//        }

        A.Hex {
            id: hex2
            anchors.centerIn: parent
            width: parent.width * 0.866
            height: parent.height * 0.866
            color: root.col2
            rotation: 30
            visible: false
        }

// NOTE: Wait for Qt6 implementation
//        DropShadow {
//            anchors.fill: hex2
//            source: hex2
//            verticalOffset: 3
//            horizontalOffset: 3
//            radius: 10.0
//            samples: 20
//            rotation: 30
//        }

        A.Hex {
            id: hex3
            anchors.centerIn: parent
            width: parent.width * Math.pow(0.866, 2)
            height: parent.height * Math.pow(0.866, 2)
            color: root.col3
            rotation: 60
            visible: false
        }

// NOTE: Wait for Qt6 implementation
//        DropShadow {
//            anchors.fill: hex3
//            source: hex3
//            verticalOffset: 2
//            horizontalOffset: 2
//            radius: 8.0
//            samples: 20
//            rotation: 60
//        }

        Text {
            id: text
            anchors.centerIn: parent
            anchors.verticalCenterOffset: -root.height * 0.02
            text: "Tg"
            font.pixelSize: root.height * 0.35
            color: root.colText
            visible: false
        }

// NOTE: Wait for Qt6 implementation
//        DropShadow {
//            anchors.fill: text
//            source: text
//            verticalOffset: 2
//            horizontalOffset: 2
//            radius: 8.0
//            samples: 20
//        }
    }
}