// Copyright © 2016-2023. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick

Rectangle {
    id: topRounding
    radius: Math.max(width * 0.02, 10)

    Rectangle {
        color: parent.color

        anchors {
            left: parent.left
            right: parent.right
            bottom: parent.bottom
        }

        height: parent.radius
    }
}
