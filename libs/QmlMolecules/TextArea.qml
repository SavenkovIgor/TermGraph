// Copyright © 2016-2022. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick
import QtQuick.Controls  as C
import QtQuick.Layouts

import Atoms as A
import StyleInfo

Column {

    property alias labelText: label.text
    property alias placeholderText:  txtArea.placeholderText
    property alias text: txtArea.text
    property alias txtFocus: txtArea.focus
    property alias cursorPosition: txtArea.cursorPosition
    property alias inputMethodHints: txtArea.inputMethodHints
    property alias textFormat: txtArea.textFormat
    property alias readOnly: txtArea.readOnly

    function takeFocus() {
        txtArea.forceActiveFocus()
        txtArea.cursorPosition = txtArea.text.length
    }

    A.AccentText { id: label; width: parent.width; }

    C.TextArea {
        id: txtArea
        color: Colors.white
        width: parent.width
        placeholderTextColor: Colors.whiteDisabled

        font: Fonts.h3
        wrapMode: TextEdit.WordWrap
    }
}
