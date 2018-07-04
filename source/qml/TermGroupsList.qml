import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import QtQuick.Window 2.3
import QtQuick.Dialogs 1.2

import "UIExtensions"

Item {
    id: grpLst
    objectName: "grpLst"

    function groupListOpen() {
        groupsList.forceActiveFocus()
    }

    Keys.onPressed: {
        if( event.modifiers === Qt.ControlModifier ) {
            if( event.key === Qt.Key_N ) {
                newGroupDrawer.open()
                event.accepted = true
            }
        }
        else if( event.key === Qt.Key_Escape )
            if( newGroupDrawer.isOpen ) {
                newGroupDrawer.close()
                event.accepted = true
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

    MyRoundButton {
        id: exportButton

        anchors.right: addGroupBtn.left
        anchors.bottom: parent.bottom

        onClicked: {
            sceneObj.exportGrpToJson(groupsList.currentItem.text)
            groupExportedDialog.text = groupExportedDialog.text + "\n" + sceneObj.getExportPath()
            groupExportedDialog.visible = true
        }

        Component.onCompleted: loadIcon("qrc:/icons/share-boxed")
    }

    FileDialog {
        id: importDialog
        title: "Пожалуйста выберите файл для импорта"

        onAccepted: {
            console.log("You chose: " + importDialog.fileUrls)
            sceneObj.importFile(importDialog.fileUrl)
        }
    }

    MyRoundButton {
        id: importButton

        anchors.right: exportButton.left
        anchors.bottom: parent.bottom

        onClicked: {
            importDialog.visible = true
        }
    }

    Connections {
        target: sceneObj
        onUpdateGroupLists: {
            groupsList.refreshModel()
        }
    }

    ListView {
        id: groupsList
        anchors.fill: parent
        model: sceneObj.getGroupsNames()
        keyNavigationEnabled: true

        function refreshModel() {
            model = sceneObj.getGroupsNames()
        }

        highlight: Rectangle {
            width: 200; height: 20
            color: "#FFFF88"
            y: listView.currentItem.y;
        }

        delegate: Rectangle {
            id: lstDlgt
            border.color: "lightGray"
            border.width: 1
            anchors.left: parent.left
            anchors.right: parent.right

            property alias text: grpName.text

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

                    text: modelData
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

                    text: sceneObj.getGroupString(modelData)
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
            sceneObj.deleteGroup(groupsList.currentItem.text)
        }
    }

    Drawer {
        id : newGroupDrawer
        width: grpLst.width
        height: grpLst.height
        interactive: false

        edge: Qt.BottomEdge

        onOpened: newGroupName.takeFocus()

        onClosed: {
            newGroupName.text = ""
            newGroupComment.text = ""
            newGroupType.currentIndex = 0
        }

        ColumnLayout {
            anchors.fill: parent
            anchors.margins: 40
            MyTextField {
                id: newGroupName
                labelText: "Название новой группы"
                placeholderText: "[Введите название]"
            }

            MyTextField {
                id: newGroupComment
                labelText: "Комментарий к группе"
                placeholderText: "[Комментарий]"
            }

            MyComboBox {
                id: newGroupType
                Component.onCompleted: {
                    model = sceneObj.getGroupTypes()
                }
            }

            RowLayout {
                spacing: 30

                MySquareButton {
                    id: addGroup
                    text: "Добавить"

                    onClicked: {
                        sceneObj.addNewGroup(newGroupName.text,newGroupComment.text,newGroupType.currentIndex -1 ) //-1 for default group
                        newGroupDrawer.close()
                    }
                }

                MySquareButton {
                    id: cancelNewGrp
                    text: "Отмена"
                    onClicked: newGroupDrawer.close()
                }

                Item {
                    Layout.fillHeight: true
                }
            }
        }
    }
}

