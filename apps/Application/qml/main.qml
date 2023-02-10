// Copyright © 2016-2023. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts  // NOTE: Need for proper plugins initialization on Android
import QtQuick.Controls.Material

import Api

//import Learning
import Theme
import Atoms as A
import Molecules as M
import Pages as P

ApplicationWindow {
    id: window
    visible: true

    width:  1000
    height: 800

    Material.theme: Material.Dark
    Material.accent: Material.Purple

//    Component.onCompleted: {
//        showMaximized();
//    }

    color: Theme.color.base0

    Component {
        id: mainSceneComponent;

        P.MainScene {
            onOpenMainMenu: appSideMenu.open()
            onOpenGroupsList: openGroupsAction.trigger()
        }
    }
    Component { id: groupsListComponent;  P.GroupsList { } }
//    Component { id: learnGroupsComponent; LearnGroupsList { } }
    Component { id: settingsComponent;    P.Settings { } }
    Component {
        id: onBoardComponent
        P.OnBoard { onFinished: stackView.replace(mainSceneComponent) }
    }

    Action {
        id: openGroupsAction
        text: "Группы"
        icon.source: Theme.icon.list
        onTriggered: stackView.push(groupsListComponent)
    }

    M.MainAppDrawer {
        id : appSideMenu

        height: window.height
        interactive: stackView.depth === 1

        groupAction: openGroupsAction

        maxWidth: window.width * 0.8

        onOpenSettings: stackView.push(settingsComponent)
    }

    M.NotificationDrawer {
        width: window.width
        height: Math.min(implicitHeight, window.height * 0.8)
    }

    Component.onCompleted: Api.groups.init()

    Connections {
        target: Api.groups

        function onGroupListLoaded() {
            if (Api.groups.hasAnyGroup)
                stackView.push(mainSceneComponent);
            else
                stackView.push(onBoardComponent);
        }
    }

    // App background
    Page {
        anchors.fill:parent
        visible: stackView.empty

        M.LoadingIcon {
            anchors.centerIn: parent
            size: Math.min(parent.width, parent.height) * 0.15
        }
    }

    StackView {
        id: stackView

        anchors.fill: parent
        visible: !empty
        initialItem: null

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
