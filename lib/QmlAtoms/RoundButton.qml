// Copyright © 2016-2023. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick
import QtQuick.Controls as C

import Theme

C.RoundButton {
    id: root

    property real size: Sizes.baseX4
    readonly property real iconPart: 0.46
    property color bgColor: Theme.color.base6

    implicitWidth:  root.size
    implicitHeight: root.size

    icon.width: implicitWidth * iconPart
    icon.height: implicitHeight * iconPart
    icon.color: "black"

    display: AbstractButton.IconOnly

    background: Round { color: root.bgColor }
}

