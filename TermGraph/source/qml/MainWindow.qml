import QtQuick 2.13
import QtQuick.Layouts 1.13
import QtQuick.Controls 2.13
import QtQuick.Window 2.13
import QtQuick.Dialogs 1.3
//import QtQuick.Controls.Material 2.2

import "UIExtensions"
import "Learning"
import "Js/IconPath.js" as IconPath
import "Js/Colors.js" as Colors

ApplicationWindow {
    id: window
    visible: true

    width:  1000
    height: 800

//    Component.onCompleted: {
//        showMaximized();
//    }

    Component {
        id: mainSceneItem
        MainSceneView {
            sideMenu: appSideMenu

            onShowInfo: infoPanel.showInfo(info)
            onShowWarning: infoPanel.showWarning(warning)
            onShowError: infoPanel.showError(error)
        }
    }

    Component { id: groupsListComponent; TermGroupsList { } }
    Component { id: learnGroupsComponent; LearnGroupsList { } }
    Component { id: settingsComponent;   MySettings { } }
    Component { id: helpPageComponent;   HelpPage { } }

    Drawer {
        id : appSideMenu
        height: window.height

        interactive: stackView.depth === 1

        onOpened: groupMenuButton.forceActiveFocus()

        background: Rectangle {
            color: Colors.baseLight
            ThinLine { target: parent; side: ThinLine.LineSide.Right; color: Colors.white; }
        }

        dragMargin: Qt.styleHints.startDragDistance * 2

        function openItem(item) {
            item.open()
        }

        contentItem: Control {

            padding: 25

            contentItem: ColumnLayout {

                spacing: 10

                SideMenuButton {
                    id: groupMenuButton
                    labelText: "Группы"
                    iconSource: IconPath.list
                    Layout.fillWidth: true

                    onClicked: stackView.push(groupsListComponent)
                }

                SideMenuButton {
                    labelText: "Изучение"
                    iconSource: IconPath.book
                    Layout.fillWidth: true
                    visible: false
                    onClicked: stackView.push(learnGroupsComponent)
                }

                SideMenuButton {
                    labelText: "Синхронизация"
                    iconSource: IconPath.loopCircular
                    Layout.fillWidth: true
                    visible: false
                }

                SideMenuButton {
                    labelText: "Настройки"
                    iconSource: IconPath.cog
                    Layout.fillWidth: true

                    onClicked: stackView.push(settingsComponent)
                }

                SideMenuButton {
                    labelText: "Справка"
                    iconSource: IconPath.questionMark
                    Layout.fillWidth: true

                    onClicked: stackView.push(helpPageComponent)
                }

                Item { Layout.fillHeight: true; }
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
            font.pixelSize: mainObj.getUiElementSize("infoLabel") * Screen.pixelDensity
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
