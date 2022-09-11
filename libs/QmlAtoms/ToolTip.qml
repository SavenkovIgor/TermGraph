// Copyright © 2016-2022. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick
import QtQuick.Controls

import Theme

ToolTip {
    id: root

    text: ""

    contentItem: Text {
        text: root.text
        font: Theme.font.h4
        color: Theme.color.whiteDisabled
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
    }

    background: Rectangle {
        color: Theme.color.baseLight2
        border.color: Theme.color.baseLight3
    }
}
