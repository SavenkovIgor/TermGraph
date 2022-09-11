// Copyright © 2016-2022. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

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

            font: Fonts.setWeight(Fonts.h2, 30)
            minimumPixelSize: font.pixelSize / 2
            fontSizeMode: Text.Fit
            elide: Text.ElideRight
            color: Colors.white
            horizontalAlignment: Qt.AlignLeft
            verticalAlignment: Qt.AlignVCenter
            text: "TermGraph"
        }

        A.ToolButton {
            Layout.margins: 5
            width: Sizes.baseX3
            height: Sizes.baseX3
            icon.color: Colors.whiteDisabled
            icon.source: IconPath.cog
            onClicked: root.settingsClicked()
        }
    }

    background: Rectangle { color: Colors.base }
}
