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

import "UIExtensions"
import "Atoms" as A
import "Molecules" as M
import "Js/IconPath.js" as IconPath
import "Js/Colors.js" as Colors

M.Page {
    id: root

    title: "Список групп"
    padding: 0

    MyRoundButton {
        id: addGroupBtn
        icon.source: IconPath.plus

        anchors { right: parent.right; bottom: parent.bottom; margins: width / 2; }

        onClicked: newGroupDrawer.open()
    }

    MyRoundButton {
        id: deleteGroupBtn
        icon.source: IconPath.trash

        anchors { right: parent.right; bottom: addGroupBtn.top; margins: width / 2; }

        onClicked: groupDeleteDialog.visible = true
    }

    // Temporary hide FileExportButton
    // TODO: Restore File exporting later!
    /*
    MyRoundButton {
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

    MyRoundButton {
        id: sendByNetworkButton
        icon.source: IconPath.share

        anchors { right: addGroupBtn.left; bottom: parent.bottom; margins: width / 2; }

        onClicked: syncManager.sendGroupByNetwork(groupsList.currentItem.groupUuid)
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

            property alias text: grpName.text
            property string groupUuid: modelData

            anchors { left: parent.left; right: parent.right; }
            height: grCol.height

            color: "transparent"

            states: State {
                name: "Current"
                when: lstDlgt.ListView.isCurrentItem
                PropertyChanges { target: lstDlgt; color: Colors.baseLight2 }
            }

            A.BottomThinLine { }

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

        onYes: {
            groupsManager.deleteGroup(groupsList.currentItem.groupUuid)
        }
    }

    Drawer {
        id : newGroupDrawer
        width: root.width
        height: addGroupLay.height
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

        RowLayout {
            id: addGroupLay

            width: root.width

            spacing: newGroupAddButton.width / 2

            ColumnLayout {
                id: newGroupName

                Layout.topMargin: addGroupLay.spacing
                Layout.leftMargin: addGroupLay.spacing
                Layout.bottomMargin: addGroupLay.spacing

                property alias text: txtField.text

                function takeFocus() {
                    txtField.forceActiveFocus()
                }

                Label {
                    id: label
                    text: "Название новой группы"

                    Layout.fillWidth: true

                    font.pixelSize: mainObj.getUiElementSize("inputLabel")
                }

                TextField {
                    id : txtField
                    placeholderText: "[Введите название]"

                    Layout.fillWidth: true

                    font.pixelSize: mainObj.getUiElementSize("inputText")
                    selectByMouse: true
                }
            }

            MyRoundButton {
                id: newGroupAddButton
                icon.source: IconPath.check

                Layout.rightMargin: addGroupLay.spacing

                onClicked: {
                    groupsManager.addNewGroup(newGroupName.text, "")
                    newGroupDrawer.close()
                }
            }
        }
    }
}

