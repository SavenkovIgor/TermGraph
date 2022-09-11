// Copyright © 2016-2022. Savenkov Igor
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Layouts

import Api

import Atoms as A
import Molecules as M
import Pages as P

import Tools
import Theme

M.Page {
    id: root

    property bool thisPageVisible: StackView.visible
    property string lastChangedNodeName: ""

    signal openGroupsList()

    padding: 0

    title: Scene.hasCurrentGroup ? Scene.currentGroup.name : "TermGraph"

    state: Scene.hasSelection ? "some" : "none"

    states: [
        State {
            name: "some"
            PropertyChanges { target: editNodeButton; visible: true }
            PropertyChanges { target: nodeInfoButton; visible: true }
            PropertyChanges { target: addNodeButton;  visible: false }
        },
        State {
            name: "none"
            PropertyChanges { target: editNodeButton; visible: false }
            PropertyChanges { target: nodeInfoButton; visible: false }
            PropertyChanges { target: addNodeButton;  visible: true }
        }
    ]

    Connections {
        target: Scene

        function onSelectionDoubleClick() {
            nodeInfoAction.trigger()
        }

        function onCurrentGroupChanged() {
            sceneCanvas.dropScale();
            sceneFlick.moveToOrigin();
        }

        function onNodesChanged() {
            if (root.lastChangedNodeName !== "") {
                sceneFlick.selectName(root.lastChangedNodeName)
                root.lastChangedNodeName = "";
            }
        }
    }

    Component { id: newNodeComponent;    P.NewNode { onNewTermAdded: (termName) => { root.lastChangedNodeName = termName; } } }
    Component { id: editNodeComponent;   P.EditNode { onTermEdited:(termName) => { root.lastChangedNodeName = termName; } } }
    Component { id: groupsListComponent; P.GroupsList { } }
    Component {
        id: termViewComponent;
        P.TermView {
            onEditNode: {
                root.StackView.view.pop();
                root.StackView.view.push(editNodeComponent);
            }
        }
    }

    header: M.DefaultHeader {
        id: header
        onOpenMainMenu: root.openMainMenu()

        A.ToolButton {
            action: Action {
                text: "Поиск"
                shortcut: "Ctrl+F"
                icon.source: Theme.icon.magnifyingGlass
                onTriggered: searchDrawer.open()
            }
        }
    }

    M.SearchDrawer {
        id: searchDrawer

        width: parent.width
        height: parent.height * 0.65

        onSearchResultClicked: sceneFlick.selectUuid(nodeUuid)
    }

    M.TermInfoDrawer {
        id: termDrawer

        width: parent.width
        height: implicitHeight

        function openWithNode(node) {
            currentNode = node;
            open();
        }

        onOpenInfoPage: {
            close();
            root.StackView.view.push(termViewComponent);
        }

        onEditNode: {
            close();
            changeNodeAction.trigger();
        }

        onOpenWarningPopup: {
            close();
            linkHardenerDrawer.openWithNode(Scene.currentNode);
        }
    }

    M.EdgeHardeningDrawer {
        id: linkHardenerDrawer

        width: parent.width
        height: Math.min(implicitHeight, parent.height * 0.65)

        onShowTermUuid: sceneFlick.selectUuid(uuid)

        function openWithNode(node) {
            currentNode = node;
            open();
        }
    }

    PinchArea {
        property real initialWidth
        property real initialHeight
        property real initialScale

        anchors.fill: sceneFlick

        pinch {
            minimumRotation: 0.0
            maximumRotation: 0.0
            dragAxis: Pinch.XAndYAxis
        }

        onPinchStarted: {
            initialWidth = sceneFlick.contentWidth;
            initialHeight = sceneFlick.contentHeight;
            initialScale = sceneCanvas.scale;
        }

        onPinchUpdated: {
            // adjust content pos due to drag
            sceneFlick.contentX += pinch.previousCenter.x - pinch.center.x
            sceneFlick.contentY += pinch.previousCenter.y - pinch.center.y

            // resize content
            let pt = pinch.center;
            pt = mapToItem(sceneFlick.contentItem, pt.x, pt.y);
            pt = Tools.clampPoint(pt, sceneCanvas.scaledSize);

            sceneCanvas.setNewScale(initialScale * pinch.scale, pt)
        }

        onPinchFinished: sceneFlick.returnToBounds()

        A.MouseArea {
            id: mouse

            readonly property point scenePt: posMappedTo(sceneCanvas)

            anchors.fill: parent

            onClicked: Scene.setMouseClick(scenePt.x, scenePt.y)

            onWheel: {
                if (wheel.modifiers & Qt.ControlModifier) {
                    if (wheel.angleDelta.y > 0)
                        sceneCanvas.upScale();
                    else
                        sceneCanvas.downScale();

                    wheel.accepted = true;
                } else {
                    wheel.accepted = false;
                }
            }
        }
    }

    contentItem: Flickable {
        id: sceneFlick

        contentWidth: sceneCanvas.width
        contentHeight: sceneCanvas.height

        ScrollIndicator.vertical:   A.ScrollIndicator { }
        ScrollIndicator.horizontal: A.ScrollIndicator { }

        property bool animationEnabled: false
        readonly property int moveAnimationDuration: 800
        property size effectiveSceneSize: {
            const searchHeight = searchDrawer.visible ? searchDrawer.height : 0;
            const infoHeight = termDrawer.visible ? termDrawer.height : 0;
            const linkHeight = linkHardenerDrawer.visible ? linkHardenerDrawer.height : 0;

            const maxHeight = Math.max(searchHeight, infoHeight, linkHeight);
            return Qt.size(width, height - maxHeight);
        }

        A.Timer {
            interval: sceneFlick.moveAnimationDuration * 1.1
            startTrigger: sceneFlick.animationEnabled
            onTriggered: sceneFlick.animationEnabled = false
        }

        Behavior on contentX {
            enabled: sceneFlick.animationEnabled
            NumberAnimation { duration: sceneFlick.moveAnimationDuration; easing.type: Easing.InOutCubic; }
        }

        Behavior on contentY {
            enabled: sceneFlick.animationEnabled
            NumberAnimation { duration: sceneFlick.moveAnimationDuration; easing.type: Easing.InOutCubic; }
        }

        function moveToOrigin() {
            moveToPoint(Qt.point(0.0, 0.0), false)
        }

        function moveToPoint(pt, withAnimation = true) {
            if (withAnimation)
                sceneFlick.animationEnabled = true;

            Qt.callLater(function() {
                contentX = pt.x;
                contentY = pt.y;
            });
        }

        function pointToCenter(pt) {
            pt.x -= effectiveSceneSize.width / 2;
            pt.y -= effectiveSceneSize.height / 2;
            moveToPoint(pt);
        }

        function selectName(termName) {
            let termUuid = Scene.termNameToUuid(termName);
            if (termUuid !== "")
                selectUuid(termUuid);
        }

        function selectUuid(termUuid) {
            if (termUuid === "")
                return;

            let pt = sceneCanvas.getTermPosition(termUuid);
            Scene.selectTerm(termUuid);
            pointToCenter(pt);
        }

        function dropSelection() {
            moveToOrigin();
            Scene.selectTerm("");
        }

        Timer {
            id: returnToBoundsTimer
            interval: 2000
            onTriggered: sceneFlick.returnToBounds()
        }

        M.NodesScene {
            id: sceneCanvas

            readonly property QtObject scaleController: QtObject {
                readonly property real min: 0.125
                readonly property real max: 2.0
                readonly property real step: 0.1
            }

            readonly property size sceneSize: Qt.size(width, height)
            readonly property size scaledSize: Tools.scaleSize(sceneSize, scale)

            transformOrigin: Item.TopLeft

            function upScale()   { setScale(scale + scaleController.step); }
            function downScale() { setScale(scale - scaleController.step); }
            function dropScale() { setScale(1.0); }

            function setScale(newScale) {
                // Clamping newScale
                newScale = Tools.clamp(newScale, scaleController.min, scaleController.max);

                // Getting center point
                let pt = mouse.posMappedTo(sceneFlick.contentItem);
                pt = Tools.clampPoint(pt, scaledSize);

                // Setting new scale
                setNewScale(newScale, pt);

                // Returning to bounds
                returnToBoundsTimer.start();
            }

            function setNewScale(newScale, centerPoint) {
                newScale = Tools.clamp(newScale, scaleController.min, scaleController.max);
                if (scale === newScale)
                    return;
                scale = newScale;
                let newSize = Tools.scaleSize(sceneSize, newScale);
                sceneFlick.resizeContent(newSize.width, newSize.height, centerPoint);
            }

            function getTermPosition(termUuid) {
                let pt = Scene.getTermPosition(termUuid);
                return Tools.scalePoint(pt, scale);
            }
        }
    }

    A.RoundButton {
        id: addNodeButton

        anchors { right: parent.right; bottom: parent.bottom; margins: width / 2; }
        visible: true

        A.ToolTip {
            property bool isEmptyGroup: {
                if (GroupsManager.hasAnyGroup) {
                    if (Scene.hasCurrentGroup) {
                        return GroupsManager.isEmptyGroup(Scene.currentGroup.uuid);
                    } else {
                        return false;
                    }
                }
                return false;
            }

            visible: addNodeButton.visible && root.thisPageVisible && isEmptyGroup

            text: addNodeButton.action.text
        }

        action: Action {
            text: "Добавить\nтермин"
            shortcut: "Ctrl+n"
            icon.source: Theme.icon.plus
            enabled: root.thisPageVisible
            onTriggered: {
                if (GroupsManager.hasAnyGroup) {
                    root.StackView.view.push(newNodeComponent)
                }
            }
        }
    }

    A.RoundButton {
        id: editNodeButton
        visible: false
        anchors { right: parent.right; bottom: parent.bottom; margins: width / 2; }

        action: Action {
            id: changeNodeAction
            text: "Изменить вершину"
            icon.source: Theme.icon.pencil
            shortcut: "Ctrl+e"
            enabled: root.thisPageVisible
            onTriggered: {
                if (!Scene.currentNode.isNull()) {
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
            icon.source: Theme.icon.info
            shortcut: "Ctrl+i"
            onTriggered: {
                if (Scene.hasSelection)
                    termDrawer.openWithNode(Scene.currentNode);
            }
        }
    }

    M.LoadingInProcess {
        anchors.fill: sceneFlick
        showLoading: Scene.groupLoading
    }

    M.EmptyView {
        anchors.fill: sceneFlick
        visible: !GroupsManager.hasAnyGroup

        mainText: "Группы отсутствуют"
        detailedText: "Думаю вам стоит создать одну"
        buttonText: "Перейти в список групп"
        onClicked: root.openGroupsList()
    }

    M.EmptyView {
        anchors.fill: sceneFlick
        visible: GroupsManager.hasAnyGroup && !Scene.hasCurrentGroup && !Scene.groupLoading

        mainText: "Группа не выбрана"
        detailedText: ""
        buttonText: "Открыть список групп"
        onClicked: root.openMainMenu()
    }
}
