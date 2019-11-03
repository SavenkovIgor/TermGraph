/*
 *  TermGraph, build graph of knowledge.
 *  Copyright © 2016-2019. Savenkov Igor. All rights reserved
 *  Contacts: dev.savenkovigor@protonmail.com
 *
 *  This file is part of TermGraph.
 *
 *  TermGraph is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  TermGraph is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with TermGraph. If not, see <https://www.gnu.org/licenses/>.
 */

import QtQuick 2.13
import QtQuick.Layouts 1.13
import QtQuick.Controls 2.13
import QtQuick.Dialogs 1.3
//import QtQuick.Controls.Material 2.2

import Notification 1.0

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

    Component { id: mainSceneComponent;   MainSceneView { sideMenu: appSideMenu; } }
    Component { id: groupsListComponent;  TermGroupsList { } }
    Component { id: learnGroupsComponent; LearnGroupsList { } }
    Component { id: settingsComponent;    MySettings { } }
    Component { id: helpPageComponent;    HelpPage { } }
    Component { id: licensePage;          LicensePage { } }

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

                SideMenuButton {
                    labelText: "О приложении"
                    Layout.fillWidth: true

                    onClicked: stackView.push(licensePage)
                }

                Item { Layout.fillHeight: true; }
            }
        }
    }

    Drawer {
        id: notifyDrawer

        width: window.width
        height: infoLabel.height
        interactive: false
        edge: Qt.BottomEdge
        dim: false

        function setTextAndOpen(text) {
            infoLabel.text = text
            notifyDrawer.open()
            infoHideTimer.start()
        }

        function showError(error)     { setTextAndOpen("Error: " + error);     }
        function showWarning(warning) { setTextAndOpen("Warning: " + warning); }
        function showInfo(info)       { setTextAndOpen("Info: " + info);       }

        Connections {
            target: Notification
            onShowInfoQml: notifyDrawer.showInfo(info)
            onShowWarningQml: notifyDrawer.showWarning(warning)
            onShowErrorQml: notifyDrawer.showError(error)
        }

        TextArea {
            id: infoLabel
            width: notifyDrawer.width

            text: ""
            readOnly: true
            wrapMode: TextEdit.WordWrap

            horizontalAlignment: Text.AlignHCenter
            font.pixelSize: mainObj.getUiElementSize("infoLabel")
        }

        Timer {
            id: infoHideTimer
            interval: 3000
            repeat: false
            onTriggered: notifyDrawer.close()
        }
    }

    StackView {
        id: stackView
        anchors.fill: parent
        initialItem: mainSceneComponent
        onCurrentItemChanged: appSideMenu.close()
    }

    Shortcut { sequence: "Ctrl+Right"; onActivated: appSideMenu.open(); }
    Shortcut { sequence: "Escape"; onActivated: stackView.pop(); }
}
