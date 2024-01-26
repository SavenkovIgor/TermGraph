// Copyright © 2016-2023. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick
import QtQuick.Controls  as C
import QtQuick.Layouts

import Atoms as A
import Theme

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
        color: Theme.color.base7
        width: parent.width
        placeholderTextColor: Theme.color.base4

        font: Theme.font.h3
        wrapMode: TextEdit.WordWrap
    }
}
