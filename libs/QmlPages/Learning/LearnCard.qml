// Copyright © 2016-2022. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Templates  as T

import Molecules as M
import StyleInfo

M.Page {
    id: root

    contentItem: ColumnLayout {

        spacing: root.height * 0.05

        Rectangle {
            id: card

            radius: root.width * 0.02
            color: Colors.baseLight3

            Layout.fillHeight: true
            Layout.fillWidth: true

            anchors.margins: root.height * 0.05

            ColumnLayout {
                anchors.fill: parent

                Text {
                    text: card
                    Layout.fillHeight: true
                }

                Rectangle {
                    id: line
                    height: 5
                    color: "red"
                }

                Text {
                    height: card.height * 0.2
                    color: "transparent"
                }
            }
        }

        RowLayout {

            height: root.height * 0.2
            Layout.fillWidth: true

            spacing: root.height * 0.05

            Button {
                id: badLearn
                text: "Button1"

                background: Rectangle { color: Colors.baseLight3 }
                Layout.fillWidth: true
            }

            Button {
                id: wellLearn
                text: "Button2"

                background: Rectangle { color: Colors.baseLight3 }
                Layout.fillWidth: true
            }

            Button {
                id: excellentLearn
                text: "Button3"

                background: Rectangle { color: Colors.baseLight3 }
                Layout.fillWidth: true
            }
        }

    }
}
