// Copyright © 2016-2022. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick
import QtQuick.Controls

import StyleInfo

ToolTip {
    id: root

    text: ""

    contentItem: Text {
        text: root.text
        font: Fonts.h4
        color: Colors.whiteDisabled
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
    }

    background: Rectangle {
        color: Colors.baseLight2
        border.color: Colors.baseLight3
    }
}
