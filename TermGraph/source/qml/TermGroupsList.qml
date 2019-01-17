import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import QtQuick.Window 2.3
import QtQuick.Dialogs 1.2

import "UIExtensions"

Page {

    id: grpLst

    property StackView mainStack

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

        anchors.right: parent.right
        anchors.bottom: parent.bottom

        onClicked: {
            newGroupDrawer.open()
        }
        Component.onCompleted: loadIcon("qrc:/icons/plus")
    }

    MyRoundButton {
        id: deleteGroupBtn

        anchors.right: parent.right
        anchors.bottom: addGroupBtn.top

        onClicked: {
            groupDeleteDialog.visible = true
        }
        Component.onCompleted: loadIcon("qrc:/icons/x")
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

        anchors.right: addGroupBtn.left
        anchors.bottom: parent.bottom

        onClicked: { groupsManager.sendGroupByNetwork(groupsList.currentItem.groupUuid) }

        Component.onCompleted: loadIcon("qrc:/icons/share-boxed")
    }

    Connections {
        target: groupsManager
        onGroupsListChanged: groupsList.refreshModel()
    }

    Connections {
        target: nodesManager
        onNodeChanged: groupsList.refreshModel()
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
            color: "#FFFF88"
            y: groupsList.currentItem.y;
        }

        delegate: Rectangle {
            id: lstDlgt
            border.color: "lightGray"
            border.width: 1
            anchors.left: parent.left
            anchors.right: parent.right

            property alias text: grpName.text
            property string groupUuid: modelData

            height: grCol.height
            states: State {
                name: "Current"
                when: lstDlgt.ListView.isCurrentItem
                PropertyChanges { target: lstDlgt; color: "darkGray" }
            }

            Column {
                id: grCol
                height: grpName.height + grpType.height
                Text{
                    id: grpName
                    topPadding: font.pixelSize/2
                    leftPadding: font.pixelSize
                    bottomPadding: font.pixelSize/3

                    font.weight: Font.Medium

                    text: groupsManager.getGroupName(modelData)
                    font.pixelSize: mainObj.getUiElementSize("text")*Screen.pixelDensity
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignLeft
                }

                Text{
                    id: grpType
                    topPadding: 0
                    leftPadding: font.pixelSize*3
                    bottomPadding: font.pixelSize

                    font.weight: Font.Thin

                    text: "Uuid: " + modelData
                    font.pixelSize: mainObj.getUiElementSize("text")*Screen.pixelDensity*0.7

                    verticalAlignment:  Text.AlignVCenter
                    horizontalAlignment: Text.AlignLeft
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
        width: grpLst.width
        height: grpLst.height * 0.4
        interactive: false

        edge: Qt.BottomEdge

        onOpened: newGroupName.takeFocus()

        onClosed: {
            newGroupName.text = ""
//            newGroupComment.text = ""
//            newGroupType.currentIndex = 0
        }

        ColumnLayout {
            anchors.fill: parent
            anchors.margins: 40
            MyTextField {
                id: newGroupName
                labelText: "Название новой группы"
                placeholderText: "[Введите название]"
            }

            RowLayout {
                spacing: 30

                MySquareButton {
                    id: addGroup
                    text: "Добавить"

                    onClicked: {
                        groupsManager.addNewGroup(newGroupName.text,"")
                        newGroupDrawer.close()
                    }
                }

                MySquareButton {
                    id: cancelNewGrp
                    text: "Отмена"
                    onClicked: newGroupDrawer.close()
                }

//                Item {
//                    Layout.fillHeight: true
//                }
            }
        }
    }
}

