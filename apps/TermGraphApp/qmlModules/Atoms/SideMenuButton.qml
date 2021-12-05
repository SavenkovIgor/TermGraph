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

import StyleInfo
import Atoms as A

Button {
    id: root

    horizontalPadding: Sizes.baseX3
    verticalPadding: Sizes.base
    topInset: 0
    bottomInset: 0

    flat: true

    readonly property bool iconVisible: {
        return display == AbstractButton.IconOnly || display == AbstractButton.TextBesideIcon
    }
    readonly property bool textVisible: {
        return display == AbstractButton.TextOnly || display == AbstractButton.TextBesideIcon
    }

    display: AbstractButton.TextBesideIcon

    property color baseColor: {
        if (!enabled)
            return Colors.whiteDisabled;

        return highlighted ? Colors.baseLight3 : Colors.white
    }

    contentItem: Row {

        spacing: buttonIcon.width / 5

        A.ToolButton {
            id: buttonIcon

            width: Sizes.baseX3
            height: Sizes.baseX3
            icon.color: root.baseColor
            icon.source: root.icon.source
            visible: root.iconVisible
        }

        Text {
            id: label
            height: buttonIcon.height

            color: root.baseColor
            font: Fonts.setWeight(Fonts.h3, Font.Thin)
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter

            text: root.text
            visible: root.textVisible
        }
    }
}
