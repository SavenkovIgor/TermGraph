// Copyright © 2016-2025. Savenkov Igor
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
        model: Api.groups.allUuidSorted
        keyNavigationEnabled: true

        delegate: ItemDelegate {
            id: delegate

            property string groupUuid: modelData
            property string groupName: Api.groups.getGroupName(groupUuid)
            property string lastEdit: Api.groups.getLastEditString(groupUuid)
            property int nodesCount: Api.groups.getNodesCount(groupUuid)

            property real basePadding: Theme.font.h4.pixelSize

            readonly property bool isCurrentItem: ListView.isCurrentItem

            padding: basePadding / 2
            width: ListView.view.width

            contentItem: Column {
                Text {
                    width: parent.width
                    leftPadding: delegate.basePadding
                    bottomPadding: delegate.basePadding / 3

                    color: Theme.color.base7
                    font: Theme.font.setWeight(Theme.font.h4, Font.Medium)
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
                color: parent.isCurrentItem ? Theme.color.base1 : "transparent"
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
        visible: !Api.groups.hasAnyGroup

        mainText: "Пусто..."
        detailedText: ""
        buttonVisible: false
    }

    A.RoundButton {
        id: addGroupBtn
        icon.source: Theme.icon.plus

        A.ToolTip {
            text: "Нажмите чтобы добавить группу"
            visible: !Api.groups.hasAnyGroup
        }

        anchors { right: parent.right; bottom: parent.bottom; margins: width / 2; }

        onClicked: newGroupDrawer.open()
    }

    A.RoundButton {
        id: deleteGroupBtn
        icon.source: Theme.icon.trash
        visible: Api.groups.hasAnyGroup

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
            Api.groups.exportGrpToJson(groupsList.currentItem.text)
            var path = "Группа экспортирована в папку GroupsJson\n. Путь к папке:" + Api.groups.getExportPath()
            groupExportedDialog.text = path
            groupExportedDialog.visible = true
        }
    }
    */

    A.RoundButton {
        icon.source: Theme.icon.share
        visible: Api.groups.hasAnyGroup

        anchors { right: addGroupBtn.left; bottom: parent.bottom; margins: width / 2; }

        onClicked: Api.sync.sendGroupByNetwork(groupsList.currentItem.groupUuid)
    }

    M.StdDialog {
        id: groupExportedDialog
        title:  "Группа экспортирована в папку GroupsJson"
        standardButtons: M.StdDialog.Ok
    }

    M.StdDialog {
        id: groupDeleteDialog
        title: "Удалить этот термин?"
        onAccepted: Api.groups.deleteGroup(groupsList.currentItem.groupUuid)
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
                icon.source: Theme.icon.check
                onClicked: {
                    Api.groups.addNewGroup(newGroupName.text, "")
                    newGroupDrawer.close()
                }
            }
        }

        background: Rectangle { color: Theme.color.base1 }
    }
}
