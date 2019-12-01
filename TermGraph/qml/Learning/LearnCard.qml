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
import QtQuick.Controls 2.13
import QtQuick.Layouts 1.13
import QtQuick.Templates 2.13 as T

import "../UIExtensions"
import "../Molecules" as M
import "../Js/Colors.js" as Colors

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
