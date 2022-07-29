// Copyright © 2016-2022. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import Api

import Atoms as A
import Molecules as M
import StyleInfo

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
                    font: Fonts.setWeight(Fonts.h4, Font.Medium)
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