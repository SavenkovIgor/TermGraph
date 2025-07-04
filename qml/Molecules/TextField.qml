// Copyright © 2016-2025. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick
import QtQuick.Controls as C

import Atoms  as A
import Theme

Column {
    property alias labelText: label.text
    property alias placeholderText: txtField.placeholderText
    property alias text: txtField.text

    signal newText()

    function takeFocus() {
        txtField.forceActiveFocus()
    }

    spacing: 6

    A.AccentText { id: label; width: parent.width; }

    C.TextField {
        id : txtField
        width: parent.width
        font: Theme.font.h3
        selectByMouse: true

        onTextChanged: newText()

        color: Theme.color.base7
        placeholderTextColor: Theme.color.base4
    }
}
