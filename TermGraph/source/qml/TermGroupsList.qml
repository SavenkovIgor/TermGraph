import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import QtQuick.Window 2.3
import QtQuick.Dialogs 1.2

import "UIExtensions"

Page {
    id: groupsPage

    property StackView mainStack
    function open() { mainStack.push(groupsPage) }

    header: MainHeader {

        id: mainHeader
        titleText: "Список групп"

        Component.onCompleted: mainHeader.showArrowIcon()

        onMenuClick: mainStack.pop()
    }

    Keys.onPressed: {
        if (event.modifiers === Qt.ControlModifier) {
            if (event.key === Qt.Key_N) {
                newGroupDrawer.open()
                event.accepted = true
            }
        } else if (event.key === Qt.Key_Escape) {
            if (newGroupDrawer.isOpen) {
                newGroupDrawer.close()
                event.accepted = true
            }
        }
    }

    MyRoundButton {
        id: addGroupBtn
        iconName: "plus"

        anchors { right: parent.right; bottom: parent.bottom; }

        onClicked: newGroupDrawer.open()
    }

    MyRoundButton {
        id: deleteGroupBtn
        iconName: "trash"

        anchors { right: parent.right; bottom: addGroupBtn.top; }

        onClicked: groupDeleteDialog.visible = true
    }

    // Temporary hide FileExportButton
    // TODO: Restore File exporting later!
    /*
    MyRoundButton {
        id: exportButton

        anchors.right: addGroupBtn.left
        anchors.bottom: parent.bottom

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
        iconName: "share-boxed"

        anchors { right: addGroupBtn.left; bottom: parent.bottom; }

        onClicked: groupsManager.sendGroupByNetwork(groupsList.currentItem.groupUuid)
    }

    Connections {
        target: groupsManager
        onGroupsListChanged: groupsList.refreshModel()
    }

    Connections {
        target: nodesManager
        onNodeChanged: groupsList.refreshModel()
    }

    Rectangle {
        anchors.fill: parent
        color: appColors.base
    }

    ListView {
        id: groupsList
        anchors.fill: parent
        model: groupsManager.getAllUuidStringsSortedByLastEdit()
        keyNavigationEnabled: true

        function refreshModel() {
            model = groupsManager.getAllUuidStringsSortedByLastEdit()
        }

        highlight: Rectangle {
            width: 200; height: 20
            color: appColors.baseLight2
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
                PropertyChanges { target: lstDlgt; color: appColors.baseLight2 }
            }

            color: "transparent"

            Column {
                id: grCol

                Text {
                    id: grpName
                    topPadding: font.pixelSize/2
                    leftPadding: font.pixelSize
                    bottomPadding: font.pixelSize/3

                    font.weight: Font.Medium

                    color: appColors.white

                    text: groupsManager.getGroupName(modelData)
                    font.pixelSize: mainObj.getUiElementSize("text")*Screen.pixelDensity
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
        width: groupsPage.width
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

            width: groupsPage.width

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

                    font.pixelSize: mainObj.getUiElementSize("inputLabel")*Screen.pixelDensity
                }

                TextField {
                    id : txtField
                    placeholderText: "[Введите название]"

                    Layout.fillWidth: true

                    font.pixelSize: mainObj.getUiElementSize("inputText")*Screen.pixelDensity
                    selectByMouse: true
                }
            }

            MyRoundButton {
                id: newGroupAddButton
                iconName: "check"

                Layout.rightMargin: addGroupLay.spacing

                onClicked: {
                    groupsManager.addNewGroup(newGroupName.text, "")
                    newGroupDrawer.close()
                }
            }
        }
    }
}

