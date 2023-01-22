// Copyright © 2016-2022. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick

import Tools


Rectangle {
    id: debugFrame
    anchors.fill: parent
    color: "transparent"

    readonly property string size: `${width.toFixed(1)}x${height.toFixed(1)}`
    property color baseColor: Debug.randomColor()

    border { width: 1; color: baseColor; }

    Rectangle {
        anchors { horizontalCenter: parent.horizontalCenter; bottom: parent.bottom; }
        width: sizeText.implicitWidth
        height: sizeText.implicitHeight

        opacity: 0.8

        color: parent.baseColor

        Text {
            id: sizeText
            anchors.fill: parent
            color: "black"
            text: debugFrame.size
        }
    }
}
