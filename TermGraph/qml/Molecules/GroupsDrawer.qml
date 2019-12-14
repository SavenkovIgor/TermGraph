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

import "../Js/Colors.js" as Colors
import "../Js/Fonts.js" as Fonts
import "../Atoms" as A

Drawer {
    id: drawer

    property alias maxWidth: groupListView.maxWidth

    edge: Qt.RightEdge
    onOpened: groupListView.forceActiveFocus()

    ListView {
        id: groupListView
        anchors.fill: parent

        property real maxWidth: 0

        model: groupsManager.allUuidSorted

        Rectangle {
            anchors.fill: parent
            color: Colors.baseLight
            z: -1
        }

        keyNavigationEnabled: true

        delegate: Rectangle {
            id: groupLstDlgt

            anchors { left: parent.left; right: parent.right; }
            height: curText.height

            color: "transparent"

            states: State {
                name: "Current"
                when: groupLstDlgt.ListView.isCurrentItem
                PropertyChanges { target: groupLstDlgt; color: "darkGray" }
            }

            Text {
                id: curText

                padding: 30
                height: Math.floor( font.pixelSize * 2.0 )
                anchors.fill: parent

                color: Colors.white
                font: Fonts.setWeight(Fonts.text, Font.Thin)
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignLeft

                text: groupsManager.getGroupName(modelData)

                onContentWidthChanged: {
                    if (contentWidth > groupListView.maxWidth) {
                        groupListView.maxWidth = contentWidth + padding * 2
                    }
                }
            }

            A.BottomThinLine { color: Colors.white; }

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    drawer.close();
                    sceneObj.showGroup(modelData);
                }
            }
        }
    }
}
