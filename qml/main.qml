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

import QtQuick 2.14
import QtQuick.Layouts 1.14
import QtQuick.Controls 2.14
import QtQuick.Dialogs 1.3
//import QtQuick.Controls.Material 2.2

import "Learning"
import "Js/IconPath.js" as IconPath
import "Js/Colors.js" as Colors
import "Atoms" as A
import "Molecules" as M
import "Pages" as P

ApplicationWindow {
    id: window
    visible: true

    width:  1000
    height: 800

//    Component.onCompleted: {
//        showMaximized();
//    }

    color: Colors.base

    Component {
        id: mainSceneComponent;

        P.MainScene {
            onOpenMainMenu: appSideMenu.open()
            onOpenGroupsList: openGroupsAction.trigger()
        }
    }
    Component { id: groupsListComponent;  P.GroupsList { } }
    Component { id: learnGroupsComponent; LearnGroupsList { } }
    Component { id: settingsComponent;    P.Settings { } }
    Component { id: helpPageComponent;    P.Help { } }
    Component { id: licensePageComponent; P.License { } }
    Component {
        id: onBoardComponent
        P.OnBoard { onFinished: stackView.replace(mainSceneComponent) }
    }

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
                    text: action.text
                    icon.source: action.icon.source
                    Layout.fillWidth: true
                }

                A.SideMenuButton {
                    text: "Изучение"
                    icon.source: IconPath.book
                    Layout.fillWidth: true
                    visible: false
                    onClicked: stackView.push(learnGroupsComponent)
                }

                A.SideMenuButton {
                    text: "Синхронизация"
                    icon.source: IconPath.loopCircular
                    Layout.fillWidth: true
                    visible: false
                }

                A.SideMenuButton {
                    text: "Настройки"
                    icon.source: IconPath.cog
                    Layout.fillWidth: true

                    onClicked: stackView.push(settingsComponent)
                }

                A.SideMenuButton {
                    text: "Справка"
                    icon.source: IconPath.questionMark
                    Layout.fillWidth: true

                    onClicked: stackView.push(helpPageComponent)
                }

                A.SideMenuButton {
                    text: "О программе"
                    Layout.fillWidth: true

                    onClicked: stackView.push(licensePageComponent)
                }

                Item { Layout.fillHeight: true; }
            }
        }
    }

    M.NotificationDrawer {
        width: window.width
        height: Math.min(implicitHeight, window.height * 0.8)
    }

    StackView {
        id: stackView
        anchors.fill: parent
        initialItem: groupsManager.hasAnyGroup ? mainSceneComponent : onBoardComponent
        onCurrentItemChanged: appSideMenu.close()

        Keys.onBackPressed: { // Android back button
            event.accepted = true;
            if(depth > 1)
                pop();
            else
                Qt.quit();
        }
    }

    Shortcut { sequence: "Escape"; onActivated: stackView.pop(); }
}
