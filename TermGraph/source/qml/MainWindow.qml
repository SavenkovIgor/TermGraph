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

    header: MainHeader {
        id: mainHeader

        onMenuClick: {
            if (stackView.depth > 1) {
                stackView.pop()
            } else {
                appSideMenu.open()
            }
        }
    }

    Component {
        id: mainSchemeItem
        MainScheme {
            mainStack: stackView
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

    Drawer {
        id : appSideMenu
        height: window.height
        interactive: stackView.depth === 1

        onOpened: { listView.forceActiveFocus() }

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

        onOpened: { infoHideTimer.start() }

        function setTextAndOpen(text) {
            infoLabel.text = text
            infoPanel.open()
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

        anchors {
            top: header.bottom
            left: parent.left
            right: parent.right
            bottom: parent.bottom
        }

        initialItem: mainSchemeItem

        onCurrentItemChanged: {
            if (depth > 1) {
                mainHeader.showArrowIcon()
            } else {
                mainHeader.showMenuIcon()
            }

            if (currentItem.objectName == "mainScheme") {
                mainHeader.titleText = "TermGraph"
                currentItem.toTop()
                currentItem.setOffs(header.height + 11)
            } else if (currentItem.objectName == "grpLst") {
                mainHeader.titleText = "Список групп"
                currentItem.groupListOpen()
            }
        }

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
