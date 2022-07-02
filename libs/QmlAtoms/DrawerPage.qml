// Copyright © 2016-2022. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick
import QtQuick.Controls

import StyleInfo

Drawer {
    id: root

    property color backgroundColor: Colors.baseLight

    edge: Qt.BottomEdge

    topPadding:    Sizes.base
    leftPadding:   Sizes.base
    rightPadding:  Sizes.base
    bottomPadding: Sizes.base

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
