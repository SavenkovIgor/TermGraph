// Copyright © 2016-2023. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick
import QtQuick.Window
import QtQuick.Controls
import QtQuick.Effects

import Theme
import Atoms as A

Item {
    id: padding

    property int size: 200
    property bool dropShadow: false
    property alias hex1Visible: hex1.visible
    property alias hex2Visible: hex2.visible
    property alias hex3Visible: hex3.visible
    property alias textVisible: text.visible

    width: size
    height: size

    Item {
        id: root

        width:  padding.size * 0.95
        height: padding.size * 0.95
        anchors.centerIn: parent

        readonly property color col1: Theme.color.term.orphanNode
        readonly property color col2: Theme.color.term.leafNode
        readonly property color col3: Theme.color.base1
        readonly property color colText: Theme.color.base7

        component Shadow: MultiEffect {
            shadowEnabled: true
            shadowVerticalOffset: shadowHorizontalOffset / 2
            shadowScale: 1.2
        }

        A.Hex {
            id: hex1
            anchors.centerIn: parent
            width:  parent.width
            height: parent.height
            color: root.col1

            layer.enabled: padding.dropShadow
            layer.effect: Shadow {
                shadowHorizontalOffset: hex1.width / 30
            }
        }

        A.Hex {
            id: hex2
            anchors.centerIn: parent
            width:  parent.width  * 0.86
            height: parent.height * 0.86
            color: root.col2
            rotation: 30

            layer.enabled: padding.dropShadow
            layer.effect: Shadow {
                shadowHorizontalOffset: hex2.width / 30
            }
        }

        A.Hex {
            id: hex3
            anchors.centerIn: parent
            width:  parent.width  * Math.pow(0.86, 2)
            height: parent.height * Math.pow(0.86, 2)
            color: root.col3

            layer.enabled: padding.dropShadow
            layer.effect: Shadow {
                shadowHorizontalOffset: hex3.width / 30
            }
        }

        Text {
            id: text
            anchors.centerIn: parent
            anchors.verticalCenterOffset: -root.height * 0.02
            text: "Tg"
            font.pixelSize: root.height * 0.35
            color: root.colText

            layer.enabled: padding.dropShadow
            layer.effect: Shadow {
                shadowHorizontalOffset: text.font.pixelSize / 12
            }
        }
    }
}
