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
import QtQuick.Dialogs 1.3

import "../UIExtensions"
import "../Molecules" as M
import "../Js/IconPath.js" as IconPath
import "../Js/Colors.js" as Colors

M.Page {
    title: "Выберите группу для повторения"

    MyRoundButton {
        id: addGroupBtn
        anchors { right: parent.right; bottom: parent.bottom; margins: width / 2; }
        icon.source: IconPath.rightArrow
    }

    contentItem: ListView {
        id: groupsList
        model: groupsManager.allUuidSorted
        keyNavigationEnabled: true

        highlight: Rectangle {
            width: 200; height: 20
            color: Colors.baseLight2
            y: groupsList.currentItem.y;
        }

        delegate: Rectangle {
            id: lstDlgt
//            border.color: "lightGray"
//            border.width: 1
            anchors.left: parent.left
            anchors.right: parent.right

            property alias text: grpName.text
            property string groupUuid: modelData

            ThinLine {
                target: lstDlgt
                side: ThinLine.LineSide.Bottom
            }

            height: grCol.height
            states: State {
                name: "Current"
                when: lstDlgt.ListView.isCurrentItem
                PropertyChanges { target: lstDlgt; color: Colors.baseLight2 }
            }

            color: "transparent"

            Column {
                id: grCol

                Text {
                    id: grpName
                    topPadding: font.pixelSize / 2
                    leftPadding: font.pixelSize
                    bottomPadding: font.pixelSize / 3

                    font.weight: Font.Medium

                    color: Colors.white

                    text: groupsManager.getGroupName(modelData)
                    font.pixelSize: mainObj.getUiElementSize("text")
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignLeft
                }

                SmallInfoText {
                    description: "Uuid"
                    label: modelData
                }

                SmallInfoText {
                    description: "Last editing time"
                    label: groupsManager.getLastEditString(modelData)
                }

                SmallInfoText {
                    description: "Node count"
                    label: groupsManager.getNodesCount(modelData)

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
