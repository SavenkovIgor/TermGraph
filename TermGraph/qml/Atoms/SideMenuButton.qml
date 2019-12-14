/*
 *  TermGraph, build graph of knowledge.
 *  Copyright © 2016-2019. Savenkov Igor. All rights reserved
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

import QtQuick 2.14
import QtQuick.Controls 2.14

import "../Js/Colors.js" as Colors
import "../Js/Fonts.js" as Fonts
import "../Atoms" as A

Button {
    rightPadding: 12
    leftPadding: 12
    bottomPadding: 10
    topPadding: 10

    flat: true

    property alias labelText: label.text
    property alias iconSource: icon.icon.source

    property string baseColor: Colors.white

    onEnabledChanged: {
        baseColor = Colors.whiteDisabled
    }

    onBaseColorChanged: {
        icon.color = baseColor
        label.color = baseColor
    }

    contentItem: Row {

        spacing: icon.width / 5

        A.RoundButton {
            id: icon
            color: Colors.white
            backgroundHidden: true

            icon.width: label.font.pixelSize * 1.8
            icon.height: label.font.pixelSize * 1.8
        }

        Text {
            id: label
            height: icon.height

            color: Colors.white
            font: Fonts.setWeight(Fonts.capitalText, Font.Thin)
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }
    }
}
