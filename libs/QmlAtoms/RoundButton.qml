// Copyright © 2016-2022. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick
import QtQuick.Controls

import Theme

RoundButton {
    id: root

    property real size: Sizes.baseX4
    readonly property real iconPart: 0.46
    property color bgColor: Theme.color.white

    implicitWidth:  root.size
    implicitHeight: root.size

    icon.width: implicitWidth * iconPart
    icon.height: implicitHeight * iconPart
    icon.color: "black"

    display: AbstractButton.IconOnly

    background: Round { color: root.bgColor }
}

