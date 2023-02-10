// Copyright © 2016-2023. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import Api

import Theme

import Atoms as A

Control {
    id: root

    signal settingsClicked

    contentItem: RowLayout {
        AppIcon {
            size: Sizes.baseX4
            Layout.margins: 10
        }

        Text {
            Layout.fillWidth: true

            font: Theme.font.setWeight(Theme.font.h2, 30)
            minimumPixelSize: font.pixelSize / 2
            fontSizeMode: Text.Fit
            elide: Text.ElideRight
            color: Theme.color.base7
            horizontalAlignment: Qt.AlignLeft
            verticalAlignment: Qt.AlignVCenter
            text: "TermGraph"
        }

        A.ToolButton {
            Layout.margins: 5
            width: Sizes.baseX3
            height: Sizes.baseX3
            icon.color: Theme.color.base4
            icon.source: Theme.icon.cog
            onClicked: root.settingsClicked()
            visible: !Api.readonlyMode
        }
    }

    background: Rectangle { color: Theme.color.base0 }
}
