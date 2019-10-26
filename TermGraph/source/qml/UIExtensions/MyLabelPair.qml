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

import QtQuick 2.13
import QtQuick.Layouts 1.13
import QtQuick.Controls 2.13
import QtQuick.Window 2.13

import "../Js/Colors.js" as Colors

ColumnLayout {

    property alias name: nameLbl.text
    property alias text: textLbl.text

    spacing: 6

    Label {
        id: nameLbl
        font.pixelSize: mainObj.getUiElementSize("text") * Screen.pixelDensity + 1
        font.weight: Font.DemiBold

        color: Colors.accent

        Layout.fillWidth: true
    }

    Label {
        id: textLbl
        font.pixelSize: mainObj.getUiElementSize("text") * Screen.pixelDensity
        wrapMode: TextEdit.WordWrap

        anchors.leftMargin: 30

        color: Colors.white

        Layout.fillWidth: true
    }
}
