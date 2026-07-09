// Copyright © 2016-2025. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick
import QtQuick.Controls

import Theme

Drawer {
    id: root

    property color backgroundColor: Theme.color.base2

    edge: Qt.BottomEdge

    topPadding:    Theme.spacing.s04
    leftPadding:   Theme.spacing.s04
    rightPadding:  Theme.spacing.s04
    bottomPadding: Theme.spacing.s04

    dim: false

    implicitWidth:  leftPadding + implicitContentWidth  + rightPadding
    implicitHeight: topPadding  + implicitContentHeight + bottomPadding

    background: TopRoundedRect {
        PinBar {
            anchors {
                top: parent.top
                topMargin: height / 1.5
                horizontalCenter: parent.horizontalCenter
            }

            width: parent.width * 0.3
        }

        color: root.backgroundColor
    }
}
