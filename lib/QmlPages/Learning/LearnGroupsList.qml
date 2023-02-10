// Copyright © 2016-2023. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import Api

import Atoms as A
import Molecules as M
import Theme

M.Page {
    title: "Выберите группу для повторения"

    A.RoundButton {
        id: addGroupBtn
        anchors { right: parent.right; bottom: parent.bottom; margins: width / 2; }
        icon.source: Theme.icon.rightArrow
    }

    contentItem: ListView {
        id: groupsList
        model: Api.groups.allUuidSorted
        keyNavigationEnabled: true

        highlight: Rectangle {
            width: 200; height: 20
            color: Theme.color.base1
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
                PropertyChanges { target: lstDlgt; color: Theme.color.base1 }
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

                    color: Theme.color.base7
                    font: Theme.font.setWeight(Theme.font.h4, Font.Medium)
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignLeft

                    text: Api.groups.getGroupName(modelData)
                }

                A.SmallInfoText {
                    description: "Uuid"
                    label: modelData
                }

                A.SmallInfoText {
                    description: "Last editing time"
                    label: Api.groups.getLastEditString(modelData)
                }

                A.SmallInfoText {
                    description: "Node count"
                    label: Api.groups.getNodesCount(modelData)

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
