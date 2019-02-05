import QtQuick 2.2
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.2
import QtQuick.Window 2.3
import QtQuick.Dialogs 1.2
//import QtQuick.Controls.Material 2.2

import "UIExtensions"

ApplicationWindow {
    id: window
    visible: true

    width:  1000
    height: 800

//    Component.onCompleted: {
//        showMaximized();
//    }

    Component {
        id: mainSchemeItem
        AlterMainScheme {
            mainStack: stackView
            sideMenu: appSideMenu

            onShowInfo: infoPanel.showInfo(info)
            onShowWarning: infoPanel.showWarning(warning)
            onShowError: infoPanel.showError(error)
        }
    }

    TermGroupsList {
        id: groupsListPage
        mainStack: stackView
    }

    MySettings {
        id: settingsPage
        mainStack: stackView
    }

    Drawer {
        id : appSideMenu
        height: window.height

        interactive: stackView.depth === 1

        onOpened: { groupMenuButton.forceActiveFocus() }

        background: Rectangle {
            Rectangle {
                x: parent.width - width
                width: parent.width / 100
                height: parent.height
                color: "#e8e8e8"
                z: 1
            }

            Rectangle {
                color: "#4f4f4f"
                width: parent.width
                height: parent.height
            }
        }

        function isOpen() { return position == 1.0 }
        function isClose(){ return position == 0.0 }

        dragMargin: Qt.styleHints.startDragDistance*2

        function openItem(item) {
            item.open()
            appSideMenu.close()
        }

        ColumnLayout {

            anchors.fill: parent

            SideMenuButton {
                id: groupMenuButton
                labelText: "Группы    "
                iconPath: "qrc:/icons/list"

                baseColor: "#e8e8e8"

                Layout.fillWidth: true
                Layout.alignment: Qt.AlignHCenter
                Layout.topMargin: 25
                Layout.bottomMargin: 5
                Layout.leftMargin: 25
                Layout.rightMargin: 25

                onClicked: appSideMenu.openItem(groupsListPage)
            }

            SideMenuButton {
                id: settingsMenuButton
                labelText: "Настройки"
                iconPath: "qrc:/icons/cog"

                baseColor: "#e8e8e8"

                Layout.fillWidth: true
                Layout.alignment: Qt.AlignHCenter
                Layout.topMargin: 5
                Layout.bottomMargin: 5
                Layout.leftMargin: 25
                Layout.rightMargin: 25

                onClicked: appSideMenu.openItem(settingsPage)
            }

            Item {
                Layout.fillHeight: true
            }
        }
    }

    Drawer {
        id : infoPanel
        width: window.width
        height: infoLabel.height
        interactive: false
        edge: Qt.BottomEdge
        dim: false

        function setTextAndOpen(text) {
            infoLabel.text = text
            infoPanel.open()
            infoHideTimer.start()
        }

        function showError(error) {
            setTextAndOpen("Error: " + error)
        }

        function showWarning(warning) {
            setTextAndOpen("Warning: " + warning)
        }

        function showInfo(info) {
            setTextAndOpen("Info: " + info)
        }

        Connections {
            target: networkManager
            onShowInfo: infoPanel.showInfo(info)
            onShowWarning: infoPanel.showWarning(warning)
            onShowError: infoPanel.showError(error)
        }

        Connections {
            target: groupsManager
            onShowInfo: infoPanel.showInfo(info)
            onShowWarning: infoPanel.showWarning(warning)
            onShowError: infoPanel.showError(error)
        }

        Connections {
            target: nodesManager
            onShowInfo: infoPanel.showInfo(info)
            onShowWarning: infoPanel.showWarning(warning)
            onShowError: infoPanel.showError(error)
        }

        Connections {
            target: sceneObj
            onShowInfo: infoPanel.showInfo(info)
            onShowWarning: infoPanel.showWarning(warning)
            onShowError: infoPanel.showError(error)
        }

        TextArea {
            id: infoLabel
            width: infoPanel.width

            text: ""
            readOnly: true
            wrapMode: TextEdit.WordWrap

            horizontalAlignment: Text.AlignHCenter
            font.pixelSize: mainObj.getUiElementSize("infoLabel")*Screen.pixelDensity
        }

        Timer {
            id: infoHideTimer
            interval: 1700
            repeat: false
            onTriggered: infoPanel.close()
        }
    }

    StackView {

        id: stackView
        initialItem: mainSchemeItem

        anchors.fill: parent

        Keys.onPressed: {
            if (event.modifiers === Qt.ControlModifier) {
                if (event.key === Qt.Key_Right) {
                    if (appSideMenu.isClose()) {
                        appSideMenu.open()
                    }
                }

                if (event.key === Qt.Key_Left) {
                    if (currentItem.objectName == "mainScheme") {
                        stackView.currentItem.openGroupList()
                        stackView.forceActiveFocus()
                    }
                }
            }

            if (event.key === Qt.Key_Back) {
                if (stackView.depth > 1) {
                    stackView.pop()
                    event.accepted = true
                }
            }
        }
    }
}
