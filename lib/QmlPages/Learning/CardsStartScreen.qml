// Copyright © 2016-2022. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick
import QtQuick.Controls
import QtQuick.Templates  as T

import Theme

T.Control {
    id: root

    property real baseSize: 50
    property real minSizing: Math.min(width, height);

    background: Rectangle { color: Theme.color.base0; }

    contentItem: Rectangle {
        property real marginVal: root.minSizing * 0.05

        anchors { fill: parent; margins: marginVal; }

        radius: root.minSizing * 0.08
        color: Theme.color.base3

        Column {
            id: item
            anchors {
                fill: parent
                leftMargin: parent.width * 0.1
                rightMargin: parent.width * 0.1
                topMargin: parent.height * 0.1
                bottomMargin: parent.height * 0.1
            }

            Text {
                width: parent.width
                horizontalAlignment: Text.AlignHCenter
                textFormat: TextEdit.RichText
                text: "Карточки"

                font: Theme.font.h1
                color: "white"
            }

            Text {
                topPadding: 40
                font: Theme.font.h3
                color: "white"
                width: parent.width
                wrapMode: Text.Wrap
                text: "В упражнении нужно постараться вспомнить определение " +
                      "на карточке, а затем оценить, насколько вы были уверены в ответе. " +
                      "В зависимости от оценки, карточки в дальнейшем будут показываться чаще или реже."
            }

            Button {
                text: "Погнали"
                font: Theme.font.h3
            }
        }
    }
}
