// Copyright © 2016-2025. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick
import QtQuick.Controls

import Theme

Drawer {
    id: root

    property color backgroundColor: Theme.color.base2

    edge: Qt.BottomEdge

    topPadding:    Theme.spacing.base
    leftPadding:   Theme.spacing.base
    rightPadding:  Theme.spacing.base
    bottomPadding: Theme.spacing.base

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
