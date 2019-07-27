import QtQuick 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12
import QtQuick.Window 2.12
import QtQuick.Dialogs 1.3
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

    // Qml colors object
    QtObject {
        id: appColors
        property color black:         "#000000"
        property color white:         "#e8e8e8"
        property color whiteDisabled: "#9c9c9c"
        property color base:          "#332f30"
        property color baseLight:     "#4f4f4f"
        property color baseLight2:    "#464544"
        property color baseLight3:    "#6f6f6f"
        property color accent:        "#2f7ba3"
    }

    Component {
        id: mainSceneItem
        MainSceneView {
            mainStack: stackView
            sideMenu: appSideMenu

            onShowInfo: infoPanel.showInfo(info)
            onShowWarning: infoPanel.showWarning(warning)
            onShowError: infoPanel.showError(error)
        }
    }

    Component {
        id: groupsListComponent
        TermGroupsList { }
    }

    Component {
        id: settingsComponent
        MySettings { }
    }

    Component {
        id: helpPageComponent
        HelpPage { }
    }

    Drawer {
        id : appSideMenu
        height: window.height

        interactive: stackView.depth === 1

        onOpened: groupMenuButton.forceActiveFocus()

        background: Rectangle {
            color: appColors.baseLight
            ThinLine { target: parent; side: ThinLine.LineSide.Right; color: appColors.white; }
        }

        function isOpen() { return position == 1.0 }
        function isClose(){ return position == 0.0 }

        dragMargin: Qt.styleHints.startDragDistance*2

        function openItem(item) {
            item.open()
        }

        ColumnLayout {

            anchors.fill: parent

            SideMenuButton {
                id: groupMenuButton
                labelText: "Группы    "
                iconName: "list"

                Layout.fillWidth: true
                Layout.alignment: Qt.AlignHCenter
                Layout.topMargin: 25
                Layout.bottomMargin: 5
                Layout.leftMargin: 25
                Layout.rightMargin: 25

                onClicked: stackView.push(groupsListComponent)
            }

            SideMenuButton {
                id: syncMenuButton
                enabled: false
                labelText: "Синхронизация"
                iconName: "loop-circular"

                Layout.fillWidth: true
                Layout.alignment: Qt.AlignHCenter
                Layout.topMargin: 5
                Layout.bottomMargin: 5
                Layout.leftMargin: 25
                Layout.rightMargin: 25
            }

            SideMenuButton {
                id: settingsMenuButton
                labelText: "Настройки"
                iconName: "cog"

                Layout.fillWidth: true
                Layout.alignment: Qt.AlignHCenter
                Layout.topMargin: 5
                Layout.bottomMargin: 5
                Layout.leftMargin: 25
                Layout.rightMargin: 25

                onClicked: stackView.push(settingsComponent)
            }

            SideMenuButton {
                id: helpMenuButton
                labelText: "Справка"
                iconName: "book"

                Layout.fillWidth: true
                Layout.alignment: Qt.AlignHCenter
                Layout.topMargin: 5
                Layout.bottomMargin: 5
                Layout.leftMargin: 25
                Layout.rightMargin: 25

                onClicked: stackView.push(helpPageComponent)
            }

            Item { Layout.fillHeight: true; }
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

        function showError(error)     { setTextAndOpen("Error: " + error);     }
        function showWarning(warning) { setTextAndOpen("Warning: " + warning); }
        function showInfo(info)       { setTextAndOpen("Info: " + info);       }

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
        anchors.fill: parent
        initialItem: mainSceneItem
        onCurrentItemChanged: appSideMenu.close()
    }

    Shortcut { sequence: "Ctrl+Right"; onActivated: appSideMenu.open(); }
    Shortcut { sequence: "Escape"; onActivated: stackView.pop(); }
}
