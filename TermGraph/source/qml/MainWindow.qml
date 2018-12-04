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

    width:  500
    height: 500

//    Component.onCompleted: {
//        showMaximized();
//    }

    Component {
        id: mainSchemeItem
        AlterMainScheme {
            mainStack: stackView
            sideMenu: appSideMenu
        }
    }

    Component {
        id: groupsListItem
        TermGroupsList {
            mainStack: stackView
        }
    }

    Component {
        id: settingsItem
        MySettings {
            mainStack: stackView
        }
    }

    Component {
        id: alterScheme
        AlterMainScheme {
            mainStack: stackView
            sideMenu: appSideMenu
        }
    }

    Drawer {
        id : appSideMenu
        height: window.height
        interactive: stackView.depth === 1

        onOpened: { groupMenuButton.forceActiveFocus() }

        function isOpen() { return position == 1.0 }
        function isClose(){ return position == 0.0 }

        dragMargin: Qt.styleHints.startDragDistance*2

        function openItem(item) {
            stackView.push(item)
            appSideMenu.close()
        }

        ColumnLayout {

            anchors.fill: parent

            SideMenuButton {
                id: groupMenuButton
                text: "Группы"

                Layout.fillWidth: true
                Layout.alignment: Qt.AlignHCenter
                Layout.topMargin: 5
                Layout.bottomMargin: 5
                Layout.leftMargin: 25
                Layout.rightMargin: 25

                onClicked: appSideMenu.openItem(groupsListItem)
            }

            SideMenuButton {
                id: settingsMenuButton
                text: "Настройки"

                Layout.fillWidth: true
                Layout.alignment: Qt.AlignHCenter
                Layout.topMargin: 5
                Layout.bottomMargin: 5
                Layout.leftMargin: 25
                Layout.rightMargin: 25

                onClicked: appSideMenu.openItem(settingsItem)
            }

            SideMenuButton {
                id: alterSchemeMenuButton
                text: "Альтернативная схема"

                Layout.fillWidth: true
                Layout.alignment: Qt.AlignHCenter
                Layout.topMargin: 5
                Layout.bottomMargin: 5
                Layout.leftMargin: 25
                Layout.rightMargin: 25

                onClicked: appSideMenu.openItem(alterScheme)
            }

            Item {
                Layout.fillHeight: true
            }
        }
    }

    Drawer {
        id : infoPanel
        width: window.width
        height: mainObj.getUiElementSize("infoLabel")*Screen.pixelDensity*2
        interactive: false
        edge: Qt.BottomEdge

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
            target: groupsManager
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

        Label {
            id: infoLabel
            text: ""
            horizontalAlignment: Text.AlignHCenter
            font.pixelSize: mainObj.getUiElementSize("infoLabel")*Screen.pixelDensity
            anchors.fill: parent
            anchors.margins: mainObj.getUiElementSize("infoLabel")*Screen.pixelDensity*0.5
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
