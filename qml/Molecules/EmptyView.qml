// Copyright © 2016-2025. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import Theme
import Atoms as A
import Molecules as M

Control {
    id: root

    property alias mainText: mainText.text
    property alias detailedText: detailedText.text
    property alias buttonText: button.text

    property alias detailsVisible: detailedText.visible
    property alias buttonVisible: button.visible

    signal clicked()

    background: Rectangle { color: Theme.color.base0 }

    ColumnLayout {
        anchors.centerIn: parent

        width: button.width * 2
        spacing: root.height * 0.05

        Text {
            id: mainText
            horizontalAlignment: Text.AlignHCenter
            color: Theme.color.base7
            font: Theme.font.h3
            Layout.alignment: Qt.AlignHCenter
            Layout.fillWidth: true
        }

        Text {
            id: detailedText
            horizontalAlignment: Text.AlignHCenter
            color: Theme.color.base4
            font: Theme.font.h4
            Layout.alignment: Qt.AlignHCenter
            Layout.fillWidth: true
        }

        A.SquareButton {
            id: button
            width: implicitWidth
            Layout.alignment: Qt.AlignHCenter
            onClicked: root.clicked()
        }
    }
}
