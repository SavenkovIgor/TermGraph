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
import "../Js/NodePaint.js" as JsPaint
import "../Js/IconPath.js" as IconPath

M.Page {
    id: root

    property Drawer sideMenu
    property bool currentPageOpened: StackView.visible

    signal openGroupsList()

    padding: 0

    title: "TermGraph"
    onOpenMainMenu: sideMenu.open()

    state: sceneObj.hasSelection ? "some" : "none"

    states: [
        State {
            name: "some"
            PropertyChanges { target: editNodeButton;   visible: true }
            PropertyChanges { target: deleteNodeButton; visible: true }
            PropertyChanges { target: nodeInfoButton;   visible: true }
            PropertyChanges { target: addNodeButton;    visible: false }
        },
        State {
            name: "none"
            PropertyChanges { target: editNodeButton;   visible: false }
            PropertyChanges { target: deleteNodeButton; visible: false }
            PropertyChanges { target: nodeInfoButton;   visible: false }
            PropertyChanges { target: addNodeButton;    visible: true }
        }
    ]

    Connections {
        target: sceneObj
        onSelectionDoubleClick: nodeInfoButton.openTerm()
    }

    Component { id: newNodeComponent;    P.NewNode { } }
    Component { id: editNodeComponent;   P.EditNode { } }
    Component { id: groupsListComponent; P.GroupsList { } }
    Component { id: termViewComponent;   P.TermView { } }

    background: Rectangle { color: sceneObj.getSceneBackgroundColor() }

    MouseArea {
        id: sceneMouse

        anchors.fill: sceneView
        hoverEnabled: true

        onMouseXChanged: setPos()
        onMouseYChanged: setPos()

        onClicked: {
            const pt = scenePt();
            sceneObj.setMouseClick(pt.x, pt.y)
        }

        function scenePt() { return sceneMouse.mapToItem(sceneCanvas, mouseX, mouseY); }

        function setPos() {
            const pt = scenePt();
            sceneObj.setMousePos(pt.x, pt.y);
        }
    }

    contentItem: Flickable {
        id: sceneView

        contentWidth: sceneCanvas.width
        contentHeight: sceneCanvas.height

        boundsBehavior: Flickable.StopAtBounds

        Canvas {
            id: sceneCanvas

            height: sceneObj.sceneRect.height
            width: sceneObj.sceneRect.width

            property bool paintGroups: false
            property bool paintNode: false

            renderStrategy: Canvas.Cooperative
            renderTarget: Canvas.Image

            Component.onCompleted: {
                // Try to show first group
                const groupsUuids = groupsManager.getAllUuidStringsSortedByLastEdit()

                if (groupsUuids.length !== 0) {
                    sceneObj.showGroup(groupsUuids[0])
                }
            }

            Rectangle {
                // Background for scene
                // Need here because of screenshots
                z: -1
                anchors.fill: parent
                color: sceneObj.getSceneBackgroundColor()
            }

            Connections {
                target: paintManager
                onPaintGroupQueue: {
                    sceneCanvas.paintGroups = true
                    sceneCanvas.requestPaint()
                }
                onPaintNodeQueue: {
                    sceneCanvas.paintNode = true
                    sceneCanvas.requestPaint()
                }
            }

            onPaint: {
                // console.log("Paint call")
                paintManager.setPaintInProcessFlag(true);
                const ctx = sceneCanvas.getContext('2d');

                if (sceneCanvas.paintGroups) {
                    paintAll(ctx);
                    sceneCanvas.paintGroups = false;
                }

                if (sceneCanvas.paintNode) {
                    paintNodes(ctx);
                    sceneCanvas.paintNode = false;
                }

                paintManager.setPaintInProcessFlag(false);
            }

            function paintAll(ctx)
            {
                clearRects(ctx);
                paintGroupRects(ctx);
                paintGroupNames(ctx);
                paintEdges(ctx);
                paintNodes(ctx);
            }

            function clearRects(ctx)
            {
                while (true) {
                    if (paintManager.clearQueueEmpty())
                        break;

                    const rect = paintManager.currentClearRect();
                    JsPaint.clearRect(ctx, rect, 0);

                    paintManager.nextClearRect();
                }
            }

            function paintGroupRects(ctx)
            {
                while (true) {
                    if (paintManager.groupRectQueueEmpty())
                        break;

                    const groupRect = paintManager.currentGroupRect();
                    // JsPaint.clearRect(ctx, groupRect, 2)
                    JsPaint.paintRect(ctx, groupRect, "#FFFFFF");

                    paintManager.nextGroupRect();
                }
            }

            function paintGroupNames(ctx)
            {
                while (true) {
                    if (paintManager.groupNamesQueueEmpty())
                        break;

                    const groupName = paintManager.currentGroupName();
                    const groupNamePos = paintManager.currentGroupNamePos();
                    JsPaint.paintGroupName(ctx, groupName, groupNamePos);

                    paintManager.nextGroupName();
                }
            }

            function paintEdges(ctx)
            {
                JsPaint.prepareEdge(ctx);

                while (true) {
                    if (paintManager.edgeQueueEmpty())
                        break;

                    const pt1 = paintManager.currentFirstEdgePoint();
                    const pt2 = paintManager.currentLastEdgePoint();
                    const col = paintManager.getEdgeColor();

                    ctx.strokeStyle = col;
                    ctx.beginPath();
                    JsPaint.drawBLine(ctx, pt1, pt2, col);
                    ctx.stroke();

                    paintManager.nextEdge();
                }
            }

            function paintNodes(ctx)
            {
                JsPaint.prepareRects(ctx);

                while (true) {
                    if (paintManager.nodeQueueEmpty())
                        break;

                    const rect = paintManager.currentNodeRect();
                    const color = paintManager.currentNodeColor();
                    const radius = paintManager.currentNodeRadius();

                    JsPaint.paintRoundedRect(ctx, rect, color, radius);

                    const center = paintManager.currentNodeCenter();
                    const text = paintManager.currentNodeText();

                    JsPaint.paintTextWithSplit(ctx, text, center, rect);

                    paintManager.nextNode();
                }
            }
        }
    }

    A.RoundButton {
        id: addNodeButton

        anchors { right: parent.right; bottom: parent.bottom; margins: width / 2; }
        visible: true

        ToolTip.visible: {
            let vis = groupsManager.hasAnyGroup;
            vis = vis && currentPageOpened;
            vis = vis && groupsManager.isEmptyGroup(sceneObj.getCurrGroupUuid());
            return vis;
        }
        ToolTip.text: "Добавить термин"
        ToolTip.timeout: -1

        action: Action {
            text: "AddNode"
            shortcut: "Ctrl+n"
            icon.source: IconPath.plus
            enabled: root.currentPageOpened
            onTriggered: {
                if (groupsManager.hasAnyGroup) {
                    root.StackView.view.push(newNodeComponent)
                } else {
                    Notification.showWarning("Create group first!")
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
        icon.source: IconPath.share
        visible: false

        anchors { top: showGroupListButton.bottom; right: parent.right; margins: width / 2; }

        onClicked: {
            sceneCanvas.grabToImage(function(result){
                const name = mainObj.screenshotNameAndPath("GroupName"); // TODO: Add group name
                if (name !== "") {
                    result.saveToFile(name);
                    Notification.showInfo("Снимок группы создан. Путь:" + name);
                } else {
                    Notification.showInfo("Снимок не создан((");
                }
            });
        }
    }

    A.RoundButton {
        id: editNodeButton
        visible: false
        anchors { right: parent.right; bottom: parent.bottom; margins: width / 2; }

        action: Action {
            text: "EditNode"
            icon.source: IconPath.pencil
            shortcut: "Ctrl+e"
            enabled: root.currentPageOpened
            onTriggered: {
                if (!sceneObj.currentNode.isNull()) {
                    root.StackView.view.push(editNodeComponent)
                }
            }
        }
    }

    A.RoundButton {
        id : deleteNodeButton
        icon.source: IconPath.trash
        visible: false

        anchors { right: parent.right; bottom: editNodeButton.top; margins: width / 2; }

        onClicked: nodeDelDialog.visible = true
    }

    MessageDialog {
        id: nodeDelDialog

        title: "Удаление термина"
        text:  "Удалить выделенный термин?"

        standardButtons: StandardButton.Yes | StandardButton.No
        icon: StandardIcon.Question

        onYes: sceneObj.deleteSelectedNode()
    }

    A.RoundButton {
        id : nodeInfoButton
        icon.source: IconPath.info
        visible: false

        anchors { right: editNodeButton.left; bottom: parent.bottom; margins: width / 2; }

        Shortcut {
            sequence: "Ctrl+i"
            onActivated: nodeInfoButton.openTerm()
        }

        onClicked: openTerm()

        function openTerm() {
            if (sceneObj.hasSelection) {
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
