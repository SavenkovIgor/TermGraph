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

import "Learning"
import "Js/IconPath.js" as IconPath
import "Js/Colors.js" as Colors
import "Js/Fonts.js" as Fonts
import "Atoms" as A
import "Pages" as P

ApplicationWindow {
    id: window
    visible: true

    width:  1000
    height: 800

//    Component.onCompleted: {
//        showMaximized();
//    }

    Component {
        id: mainSceneComponent;

        P.MainScene {
            sideMenu: appSideMenu;
            onOpenGroupsList: openGroupsAction.trigger()
        }
    }
    Component { id: groupsListComponent;  P.GroupsList { } }
    Component { id: learnGroupsComponent; LearnGroupsList { } }
    Component { id: settingsComponent;    P.Settings { } }
    Component { id: helpPageComponent;    P.Help { } }
    Component { id: licensePage;          P.License { } }

    Action {
        id: openGroupsAction
        text: "Группы"
        icon.source: IconPath.list
        onTriggered: stackView.push(groupsListComponent)
    }

    Drawer {
        id : appSideMenu
        height: window.height

        interactive: stackView.depth === 1

        onOpened: groupMenuButton.forceActiveFocus()

        background: Rectangle {
            color: Colors.baseLight
            A.RightThinLine { color: Colors.white; }
        }

        dragMargin: Qt.styleHints.startDragDistance * 2

        contentItem: Control {

            padding: 25

            contentItem: ColumnLayout {

                spacing: 10

                A.SideMenuButton {
                    id: groupMenuButton
                    action: openGroupsAction
                    labelText: action.text
                    iconSource: action.icon.source
                    Layout.fillWidth: true
                }

                A.SideMenuButton {
                    labelText: "Изучение"
                    iconSource: IconPath.book
                    Layout.fillWidth: true
                    visible: false
                    onClicked: stackView.push(learnGroupsComponent)
                }

                A.SideMenuButton {
                    labelText: "Синхронизация"
                    iconSource: IconPath.loopCircular
                    Layout.fillWidth: true
                    visible: false
                }

                A.SideMenuButton {
                    labelText: "Настройки"
                    iconSource: IconPath.cog
                    Layout.fillWidth: true

                    onClicked: stackView.push(settingsComponent)
                }

                A.SideMenuButton {
                    labelText: "Справка"
                    iconSource: IconPath.questionMark
                    Layout.fillWidth: true

                    onClicked: stackView.push(helpPageComponent)
                }

                A.SideMenuButton {
                    labelText: "О программе"
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
        interactive: position !== 0.0
        edge: Qt.BottomEdge
        dim: false

        function setTextAndOpen(text) {
            infoLabel.text = text
            notifyDrawer.open()
            infoHideTimer.start()
        }

        function showError(error)     { setTextAndOpen("Ошибка: " + error);     }
        function showWarning(warning) { setTextAndOpen("Предупреждение: " + warning); }
        function showInfo(info)       { setTextAndOpen("Информация: " + info);       }

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
            wrapMode: TextEdit.Wrap
            leftPadding: width * 0.025
            rightPadding: width * 0.025

            horizontalAlignment: Text.AlignHCenter
            font: Fonts.inputText
        }

        Timer {
            id: infoHideTimer
            interval: 3000
            repeat: false
            onTriggered: {
                if (infoLabel.lineCount == 1) // Auto close only if label is small
                    notifyDrawer.close()
            }
        }
    }

    StackView {
        id: stackView
        anchors.fill: parent
        initialItem: mainSceneComponent
        onCurrentItemChanged: appSideMenu.close()

        Keys.onBackPressed: { // Android back button
            event.accepted = true;
            if(depth > 1)
                pop();
            else
                Qt.quit();
        }
    }

    Shortcut { sequence: "Ctrl+Right"; onActivated: appSideMenu.open(); }
    Shortcut { sequence: "Escape"; onActivated: stackView.pop(); }
}
