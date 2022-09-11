// Copyright © 2016-2022. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import Api

import Atoms as A
import Molecules as M
import Theme

M.Page {
    id: root

    title: "Группы"
    padding: 0

    contentItem: ListView {
        id: groupsList
        model: GroupsManager.allUuidSorted
        keyNavigationEnabled: true

        delegate: ItemDelegate {
            id: delegate

            property string groupUuid: modelData
            property string groupName: GroupsManager.getGroupName(groupUuid)
            property string lastEdit: GroupsManager.getLastEditString(groupUuid)
            property int nodesCount: GroupsManager.getNodesCount(groupUuid)

            property real basePadding: Fonts.h4.pixelSize

            readonly property bool isCurrentItem: ListView.isCurrentItem

            padding: basePadding / 2
            width: ListView.view.width

            contentItem: Column {
                Text {
                    width: parent.width
                    leftPadding: delegate.basePadding
                    bottomPadding: delegate.basePadding / 3

                    color: Colors.white
                    font: Fonts.setWeight(Fonts.h4, Font.Medium)
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
                color: parent.isCurrentItem ? Colors.baseLight2 : "transparent"
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
        visible: !GroupsManager.hasAnyGroup

        mainText: "Пусто..."
        detailedText: ""
        buttonVisible: false
    }

    A.RoundButton {
        id: addGroupBtn
        icon.source: IconPath.plus

        A.ToolTip {
            text: "Нажмите чтобы добавить группу"
            visible: !GroupsManager.hasAnyGroup
        }

        anchors { right: parent.right; bottom: parent.bottom; margins: width / 2; }

        onClicked: newGroupDrawer.open()
    }

    A.RoundButton {
        id: deleteGroupBtn
        icon.source: IconPath.trash
        visible: GroupsManager.hasAnyGroup

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
            GroupsManager.exportGrpToJson(groupsList.currentItem.text)
            var path = "Группа экспортирована в папку GroupsJson\n. Путь к папке:" + GroupsManager.getExportPath()
            groupExportedDialog.text = path
            groupExportedDialog.visible = true
        }
    }
    */

    A.RoundButton {
        icon.source: IconPath.share
        visible: GroupsManager.hasAnyGroup

        anchors { right: addGroupBtn.left; bottom: parent.bottom; margins: width / 2; }

        onClicked: SyncManager.sendGroupByNetwork(groupsList.currentItem.groupUuid)
    }

    M.StdDialog {
        id: groupExportedDialog
        title:  "Группа экспортирована в папку GroupsJson"
        standardButtons: M.StdDialog.Ok
    }

    M.StdDialog {
        id: groupDeleteDialog
        title: "Удалить этот термин?"
        onAccepted: GroupsManager.deleteGroup(groupsList.currentItem.groupUuid)
    }

    // TODO: Replace with popup
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

        leftPadding:  Sizes.baseX3
        rightPadding: Sizes.baseX3
        topPadding:    Sizes.baseX2
        bottomPadding: Sizes.baseX2

        contentItem: RowLayout {

            M.TextField {
                id: newGroupName
                Layout.fillWidth: true

                labelText: "Название новой группы"
            }

            A.RoundButton {
                icon.source: IconPath.check
                onClicked: {
                    GroupsManager.addNewGroup(newGroupName.text, "")
                    newGroupDrawer.close()
                }
            }
        }

        background: Rectangle { color: Colors.baseLight2 }
    }
}

