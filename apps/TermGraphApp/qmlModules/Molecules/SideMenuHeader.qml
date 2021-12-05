/*
 *  TermGraph, build graph of knowledge.
 *  Copyright © 2016-2020. Savenkov Igor. All rights reserved
 *  Contacts: dev.savenkovigor@protonmail.com
 *
 *  This file is part of TermGraph.
 *
 *  TermGraph is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  TermGraph is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with TermGraph. If not, see <https://www.gnu.org/licenses/>.
 */

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import StyleInfo

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

            font: Fonts.setWeight(Fonts.h4, 30)
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
