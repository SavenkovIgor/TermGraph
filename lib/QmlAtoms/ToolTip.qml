// Copyright © 2016-2022. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick
import QtQuick.Controls as C

import Theme

C.ToolTip {
    id: root

    text: ""

    contentItem: Text {
        text: root.text
        font: Theme.font.h4
        color: Theme.color.base4
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
    }

    background: Rectangle {
        color: Theme.color.base1
        border.color: Theme.color.base3
    }
}
