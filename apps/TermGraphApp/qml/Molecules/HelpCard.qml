/*
 *  TermGraph, build graph of knowledge.
 *  Copyright © 2016-2020. Savenkov Igor. All rights reserved
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

import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Templates 2.15 as T

import StyleInfo 1.0

T.Control {
    id: root

    property alias text: tipTitle.text
    property real minSizing: Math.min(width, height);

    background: Rectangle { color: Colors.base; }

    contentItem: Rectangle {
        property real marginVal: root.minSizing * 0.05

        anchors { fill: parent; margins: marginVal; }

        radius: root.minSizing * 0.08
        color: Colors.baseLight3

        Item {
            id: item
            anchors {
                fill: parent
                leftMargin: parent.width * 0.075
                rightMargin: parent.width * 0.075
                topMargin: parent.height * 0.075
                bottomMargin: parent.height * 0.075
            }

            Flickable {
                id: flick
                anchors.fill: parent

                clip: true
                ScrollBar.vertical: ScrollBar {
                    policy: flick.height < tipTitle.height ? ScrollBar.AlwaysOn : ScrollBar.AsNeeded
                }

                contentWidth: tipTitle.width
                contentHeight: tipTitle.height * 1.05

                boundsBehavior: Flickable.StopAtBounds

                TextEdit {
                    id: tipTitle

                    width: flick.width

                    textFormat: TextEdit.RichText
                    wrapMode: TextEdit.WordWrap
                    readOnly: true

                    font: Fonts.inputText
                    color: "white"
                }
            }

            Rectangle {
                anchors { left: flick.left; bottom: flick.bottom; }

                width: flick.width
                height: flick.height * 0.05

                gradient: Gradient {
                    GradientStop { position: 0.0; color: "transparent"; }
                    GradientStop { position: 1.0; color: Colors.baseLight3; }
                }
            }
        }
    }
}