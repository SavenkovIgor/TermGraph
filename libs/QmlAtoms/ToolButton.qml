// Copyright © 2016-2022. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick
import QtQuick.Controls

import StyleInfo

RoundButton {
    id: root

    property real size: Sizes.baseX4
    readonly property real iconPart: 0.46

    implicitWidth:  root.size
    implicitHeight: root.size

    icon.width: implicitWidth * iconPart
    icon.height: implicitHeight * iconPart

    icon.color: Colors.white

    display: AbstractButton.IconOnly

    background: Round { color: highlighted ? "#00000099" : "transparent"; }
}
