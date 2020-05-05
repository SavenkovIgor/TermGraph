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
import QtQuick.Layouts 1.14

import "../Atoms" as A
import "../Js/Colors.js" as Colors
import "../Js/Fonts.js" as Fonts

Column {

    property alias labelText: label.text
    property alias placeholderText:  txtArea.placeholderText
    property alias text: txtArea.text
    property alias txtFocus: txtArea.focus
    property alias cursorPosition: txtArea.cursorPosition
    property alias inputMethodHints: txtArea.inputMethodHints

    function takeFocus() {
        txtArea.forceActiveFocus()
        txtArea.cursorPosition = txtArea.text.length
    }

    A.AccentText { id: label; width: parent.width; }

    TextArea {
        id: txtArea
        color: Colors.white
        width: parent.width
        placeholderTextColor: Colors.whiteDisabled

        font: Fonts.inputText
        wrapMode: TextEdit.WordWrap
    }
}
