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
import QtQuick.Dialogs 1.3

import "../Atoms" as A
import "../Molecules" as M
import "../Js/IconPath.js" as IconPath
import "../Js/Colors.js" as Colors
import "../Js/Fonts.js" as Fonts

M.Page {
    id: root

    title: "Группы"
    padding: 0

    contentItem: ListView {
        id: groupsList
        model: groupsManager.allUuidSorted
        keyNavigationEnabled: true

        delegate: ItemDelegate {
            id: delegate

            property string groupUuid: modelData
            property string groupName: groupsManager.getGroupName(modelData)
            property string lastEdit: groupsManager.getLastEditString(modelData)
            property int nodesCount: groupsManager.getNodesCount(modelData)

            property real basePadding: Fonts.text.pixelSize

            padding: basePadding / 2
            width: ListView.view.width

            contentItem: Column {

                Text {
                    id: grpName

                    width: parent.width
                    leftPadding: delegate.basePadding
                    bottomPadding: delegate.basePadding / 3

                    color: Colors.white
                    font: Fonts.setWeight(Fonts.text, Font.Medium)
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignLeft

                    text: delegate.groupName
                }

                A.SmallInfoText {
                    width: parent.width
                    description: "Uuid"
                    label: delegate.groupUuid
                }

                A.SmallInfoText {
                    width: parent.width
                    description: "Last editing time"
                    label: delegate.lastEdit
                }

                A.SmallInfoText {
                    width: parent.width
                    description: "Node count"
                    label: delegate.nodesCount

                    bottomPadding: delegate.basePadding / 2
                }
            }

            background: Rectangle {
                color: delegate.ListView.isCurrentItem ? Colors.baseLight2 : "transparent"
                A.BottomThinLine { }
            }

            onClicked: {
                forceActiveFocus()
                ListView.view.currentIndex = index
            }
        }
    }

    M.EmptyView {
        anchors.fill: groupsList
        visible: !groupsManager.hasAnyGroup

        mainText: "Пусто..."
        detailedText: ""
        buttonVisible: false
    }

    A.RoundButton {
        id: addGroupBtn
        icon.source: IconPath.plus

        A.ToolTip {
            text: "Нажмите чтобы добавить группу"
            visible: !groupsManager.hasAnyGroup
        }

        anchors { right: parent.right; bottom: parent.bottom; margins: width / 2; }

        onClicked: newGroupDrawer.open()
    }

    A.RoundButton {
        id: deleteGroupBtn
        icon.source: IconPath.trash
        visible: groupsManager.hasAnyGroup

        anchors { right: parent.right; bottom: addGroupBtn.top; margins: width / 2; }

        onClicked: groupDeleteDialog.visible = true
    }

    // Temporary hide FileExportButton
    // TODO: Restore File exporting later!
    /*
    A.RoundButton {
        id: exportButton

        anchors.right: addGroupBtn.left
        anchors.bottom: parent.bottom
        anchors.margins: width / 2;

        onClicked: {
            groupsManager.exportGrpToJson(groupsList.currentItem.text)
            var path = "Группа экспортирована в папку GroupsJson\n. Путь к папке:" + groupsManager.getExportPath()
            groupExportedDialog.text = path
            groupExportedDialog.visible = true
        }
    }
    */

    A.RoundButton {
        id: sendByNetworkButton
        icon.source: IconPath.share
        visible: groupsManager.hasAnyGroup

        anchors { right: addGroupBtn.left; bottom: parent.bottom; margins: width / 2; }

        onClicked: syncManager.sendGroupByNetwork(groupsList.currentItem.groupUuid)
    }

    MessageDialog {
        id: groupExportedDialog

        title: "Группа экспортирована"
        text:  "Группа экспортирована в папку GroupsJson"

        standardButtons: StandardButton.Ok
        icon: StandardIcon.Information
    }

    MessageDialog {
        id: groupDeleteDialog

        title: "Удаление группы"
        text:  "Вы уверены, что хотите удалить эту группу?"

        standardButtons: StandardButton.Yes | StandardButton.No
        icon: StandardIcon.Question

        onYes: groupsManager.deleteGroup(groupsList.currentItem.groupUuid)
    }

    Drawer {
        id : newGroupDrawer
        width: root.width
        interactive: false

        edge: Qt.BottomEdge

        onOpened: {
            interactive = true
            newGroupName.takeFocus()
        }

        onClosed: {
            interactive = false
            newGroupName.text = ""
        }

        background: Rectangle { color: Colors.baseLight2 }

        contentItem: RowLayout {

            M.TextField {
                id: newGroupName
                Layout.fillWidth: true
                Layout.margins: implicitHeight * 0.5

                labelText: "Название новой группы"
            }

            A.RoundButton {
                id: newGroupAddButton
                icon.source: IconPath.check
                Layout.rightMargin: newGroupName.implicitHeight * 0.5

                onClicked: {
                    groupsManager.addNewGroup(newGroupName.text, "")
                    newGroupDrawer.close()
                }
            }
        }
    }
}

