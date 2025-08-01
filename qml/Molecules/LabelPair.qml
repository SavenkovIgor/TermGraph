// Copyright © 2016-2025. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

import Atoms as A
import Theme

ColumnLayout {

    property alias name: nameLbl.text
    property alias text: textLbl.text
    property alias textObj: textLbl

    spacing: 6

    A.AccentText { id: nameLbl; Layout.fillWidth: true; }

    Text {
        id: textLbl
        font: Theme.font.h4
        wrapMode: TextEdit.WordWrap
        elide: Text.ElideRight
        anchors.leftMargin: 30

        color: Theme.color.base7

        Layout.fillWidth: true
    }
}
