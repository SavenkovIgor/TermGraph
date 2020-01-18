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
import QtQuick.Controls 2.13
import QtQuick.Controls.Material 2.0
import QtQuick.Layouts 1.13
import QtQuick.Dialogs 1.3

import Notification 1.0

import "../Atoms" as A
import "../Molecules" as M
import "../Pages" as P

import "../Js/Fonts.js" as Fonts
import "../Js/IconPath.js" as IconPath
import "../Js/Colors.js" as Colors

M.Page {
    id: root

    property Drawer sideMenu
    property bool currentPageOpened: StackView.visible

    signal openGroupsList()

    padding: 0

    title: "TermGraph"
    onOpenMainMenu: sideMenu.open()

    state: scene.hasSelection ? "some" : "none"

    states: [
        State {
            name: "some"
            PropertyChanges { target: editNodeButton;   visible: true }
            PropertyChanges { target: nodeInfoButton;   visible: true }
            PropertyChanges { target: addNodeButton;    visible: false }
        },
        State {
            name: "none"
            PropertyChanges { target: editNodeButton;   visible: false }
            PropertyChanges { target: nodeInfoButton;   visible: false }
            PropertyChanges { target: addNodeButton;    visible: true }
        }
    ]

    Connections {
        target: scene
        onSelectionDoubleClick: nodeInfoAction.trigger()
        onCurrentGroupChanged: sceneView.moveToOrigin()
    }

    Component { id: newNodeComponent;    P.NewNode { } }
    Component { id: editNodeComponent;   P.EditNode { } }
    Component { id: groupsListComponent; P.GroupsList { } }
    Component { id: termViewComponent;   P.TermView { } }

    background: Rectangle { color: Colors.base }

    header: M.DefaultHeader {
        id: header
        title: root.title
        page: root
        onOpenMainMenu: root.openMainMenu()

        TextField {
            id : searchText
            Layout.fillWidth: true
            font: Fonts.inputText
            selectByMouse: true

            color: Colors.white
            placeholderTextColor: Colors.whiteDisabled
            placeholderText: "Поиск"

            onTextChanged: {
                if (text !== "")
                    header.setSelectionWithResults();
            }

            Keys.forwardTo: searchResult

            Keys.onReturnPressed: {
                let firstNodeUuid = searchResult.getFirst();
                if (firstNodeUuid !== "")
                    sceneView.select(firstNodeUuid);
                header.setNoSelection();
            }
        }

        states: [
            State {
                name: "selection"
                PropertyChanges { target: header; titleVisible: false; }
                PropertyChanges { target: searchText; visible: true; }
                StateChangeScript {
                    script: {
                        searchText.forceActiveFocus();
                        searchText.text = "";
                        searchResult.close();
                    }
                }
            },

            State {
                name: "selectionWithResults"
                PropertyChanges { target: header; titleVisible: false; }
                PropertyChanges { target: searchText; visible: true; }
                StateChangeScript { script: { searchResult.open(); } }
            },

            State {
                name: "noSelection"
                PropertyChanges { target: header; titleVisible: true; }
                PropertyChanges { target: searchText; visible: false; }
                StateChangeScript {
                    script: {
                        sceneView.forceActiveFocus();
                        searchResult.close();
                    }
                }
            }
        ]

        function setNoSelection() { state = "noSelection"; }
        function setSelectionWithResults() { state = "selectionWithResults"; }
        function setSelection() { state = "selection"; }

        function switchSelection() {
            if (state === "selection" || state === "selectionWithResults")
                setNoSelection();
            else
                setSelection();
        }

        state: "noSelection"

        A.ToolButton {
            id: searchBtn

            action: Action {
                text: "Поиск"
                shortcut: "Ctrl+F"
                icon.source: IconPath.magnifyingGlass
                onTriggered: header.switchSelection()
            }
        }
    }

    M.SearchResults {
        id: searchResult

        x: searchText.x
        y: 0
        width: searchText.width
        model: scene.search(searchText.text);

        onOneVariant: sceneView.select(nodeUuid);
        onNoResults: sceneView.dropSelection();

        onClicked: {
            sceneView.select(nodeUuid);
            header.setNoSelection();
        }
    }

    MouseArea {
        id: sceneMouse

        anchors.fill: sceneView
        hoverEnabled: true

        onClicked: {
            const pt = scenePt();
            scene.setMouseClick(pt.x, pt.y)
        }

        function scenePt() { return sceneMouse.mapToItem(sceneCanvas, mouseX, mouseY); }
    }

    contentItem: Flickable {
        id: sceneView

        contentWidth: sceneCanvas.width
        contentHeight: sceneCanvas.height

        boundsBehavior: Flickable.StopAtBounds

        function moveToOrigin() {
            moveToPoint(Qt.point(0.0, 0.0))
        }

        function moveToPoint(pt) {
            contentX = pt.x;
            contentY = pt.y;
        }

        function pointToCenter(pt) {
            contentX = pt.x - sceneView.width / 2;
            contentY = pt.y - sceneView.height / 2;
        }

        function select(nodeUuid) {
            let pt = scene.getTermPosition(nodeUuid);
            scene.selectTerm(nodeUuid);
            pointToCenter(pt);
        }

        function dropSelection() {
            moveToOrigin();
            scene.selectTerm("");
        }

        M.NodesScene { id: sceneCanvas }
    }

    A.RoundButton {
        id: addNodeButton

        anchors { right: parent.right; bottom: parent.bottom; margins: width / 2; }
        visible: true

        A.ToolTip {
            text: addNodeButton.action.text
            visible: {
                let vis = groupsManager.hasAnyGroup;
                vis = vis && currentPageOpened;
                vis = vis && groupsManager.isEmptyGroup(scene.currentGroup);
                return vis;
            }
        }

        action: Action {
            text: "Добавить\nтермин"
            shortcut: "Ctrl+n"
            icon.source: IconPath.plus
            enabled: root.currentPageOpened
            onTriggered: {
                if (groupsManager.hasAnyGroup) {
                    root.StackView.view.push(newNodeComponent)
                } else {
                    Notification.showWarning("Нет групп для добавления вершин. Создайте группу!")
                    root.StackView.view.push(groupsListComponent)
                }
            }
        }
    }

    A.RoundButton {
        id: showGroupListButton
        icon.source: IconPath.spreadsheet
        visible: groupsManager.hasAnyGroup

        anchors { top: parent.top; right: parent.right; margins: width / 2; }

        onClicked: groupsList.open()
    }

    A.RoundButton {
        id: screenShotButton
        visible: false
        anchors { top: showGroupListButton.bottom; right: parent.right; margins: width / 2; }

        icon.source: IconPath.share

        onClicked: sceneCanvas.makeScreenshot()
    }

    A.RoundButton {
        id: editNodeButton
        visible: false
        anchors { right: parent.right; bottom: parent.bottom; margins: width / 2; }

        action: Action {
            text: "Изменить вершину"
            icon.source: IconPath.pencil
            shortcut: "Ctrl+e"
            enabled: root.currentPageOpened
            onTriggered: {
                if (!scene.currentNode.isNull()) {
                    root.StackView.view.push(editNodeComponent)
                }
            }
        }
    }

    A.RoundButton {
        id : nodeInfoButton
        anchors { right: editNodeButton.left; bottom: parent.bottom; margins: width / 2; }
        visible: false

        action: Action {
            id: nodeInfoAction
            icon.source: IconPath.info
            shortcut: "Ctrl+i"
            onTriggered: {
                if (scene.hasSelection)
                    root.StackView.view.push(termViewComponent)
            }
        }
    }

    M.GroupsDrawer {
        id : groupsList

        y: root.header.height
        width: Math.min(window.width * 0.8, groupsList.maxWidth)
        height: sceneView.height

        clip: true
        interactive: root.currentPageOpened

        Shortcut {
            sequence: "Ctrl+Left"
            onActivated: {
                groupsList.open();
                groupsList.forceActiveFocus();
            }
        }
    }

    M.EmptyView {
        anchors.fill: sceneView
        visible: !groupsManager.hasAnyGroup

        mainText: "Группы отсутствуют"
        detailedText: "Думаю вам стоит создать одну"
        buttonText: "Перейти в список групп"
        onClicked: root.openGroupsList()
    }
}
