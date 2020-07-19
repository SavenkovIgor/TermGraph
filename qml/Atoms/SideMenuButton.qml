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

import QtQuick 2.15
import QtQuick.Controls 2.15

import StyleInfo 1.0
import "../Atoms" as A

Button {
    id: root

    horizontalPadding: Sizes.baseR25
    verticalPadding: Sizes.baseR75
    topInset: 0
    bottomInset: 0

    flat: true

    property color baseColor: {
        if (!enabled)
            return Colors.whiteDisabled;

        return highlighted ? Colors.baseLight3 : Colors.white
    }

    contentItem: Row {

        spacing: icon.width / 5

        A.ToolButton {
            id: icon

            width: Sizes.baseR25
            height: Sizes.baseR25
            icon.color: root.baseColor
            icon.source: root.icon.source
        }

        Text {
            id: label
            height: icon.height

            color: root.baseColor
            font: Fonts.setWeight(Fonts.capitalText, Font.Thin)
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter

            text: root.text
        }
    }
}
