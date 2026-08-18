// Copyright © 2016-2025. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick
import QtQuick.Controls

import Theme

RoundButton {
    id: root

    property real size: Theme.spacing.s16
    readonly property real iconPart: 0.46

    implicitWidth:  root.size
    implicitHeight: root.size

    icon.width: Math.round(implicitWidth * iconPart)
    icon.height: Math.round(implicitHeight * iconPart)

    icon.color: Theme.color.base6

    display: AbstractButton.IconOnly

    background: Round { color: highlighted ? "#00000099" : "transparent"; }
}
