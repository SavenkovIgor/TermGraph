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
import QtQuick.Layouts 1.15

import Api 1.0

import Atoms 1.0 as A
import Molecules 1.0 as M
import StyleInfo 1.0

M.Page {
    title: "Выберите группу для повторения"

    A.RoundButton {
        id: addGroupBtn
        anchors { right: parent.right; bottom: parent.bottom; margins: width / 2; }
        icon.source: IconPath.rightArrow
    }

    contentItem: ListView {
        id: groupsList
        model: GroupsManager.allUuidSorted
        keyNavigationEnabled: true

        highlight: Rectangle {
            width: 200; height: 20
            color: Colors.baseLight2
            y: groupsList.currentItem.y;
        }

        delegate: Rectangle {
            id: lstDlgt

            property alias text: grpName.text
            property string groupUuid: modelData

            anchors { left: parent.left; right: parent.right; }
            height: grCol.height

            states: State {
                name: "Current"
                when: lstDlgt.ListView.isCurrentItem
                PropertyChanges { target: lstDlgt; color: Colors.baseLight2 }
            }

            color: "transparent"

            A.BottomThinLine { }

            Column {
                id: grCol

                Text {
                    id: grpName
                    topPadding: font.pixelSize / 2
                    leftPadding: font.pixelSize
                    bottomPadding: font.pixelSize / 3

                    color: Colors.white
                    font: Fonts.setWeight(Fonts.text, Font.Medium)
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignLeft

                    text: GroupsManager.getGroupName(modelData)
                }

                A.SmallInfoText {
                    description: "Uuid"
                    label: modelData
                }

                A.SmallInfoText {
                    description: "Last editing time"
                    label: GroupsManager.getLastEditString(modelData)
                }

                A.SmallInfoText {
                    description: "Node count"
                    label: GroupsManager.getNodesCount(modelData)

                    bottomPadding: grpName.font.pixelSize / 2
                }
            }

            MouseArea{
                anchors.fill: parent
                onClicked: {
                    lstDlgt.forceActiveFocus()
                    onClicked: lstDlgt.ListView.view.currentIndex = index
                }
            }
        }
    }
}
