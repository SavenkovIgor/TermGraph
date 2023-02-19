// Copyright © 2016-2023. Savenkov Igor
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

    title: Api.scene.hasCurrentGroup ? Api.scene.currentGroup.name : "TermGraph"

    Connections {
        target: Api.scene

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
            linkHardenerDrawer.openWithNode(Api.scene.currentNode);
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

            onClicked: Api.scene.setMouseClick(scenePt.x, scenePt.y)

            onWheel: {
                if (wheel.angleDelta.y > 0)
                    sceneCanvas.upScale();
                else
                    sceneCanvas.downScale();

                wheel.accepted = true;
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
            let termUuid = Api.scene.termNameToUuid(termName);
            if (termUuid !== "")
                selectUuid(termUuid);
        }

        function selectUuid(termUuid) {
            if (termUuid === "")
                return;

            let pt = sceneCanvas.getTermPosition(termUuid);
            Api.scene.selectTerm(termUuid);
            pointToCenter(pt);
        }

        function dropSelection() {
            moveToOrigin();
            Api.scene.selectTerm("");
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
            function dropScale() { scaleToFitInCanvasWidth(); }

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
                let pt = Api.scene.getTermPosition(termUuid);
                return Tools.scalePoint(pt, scale);
            }

            function scaleToFitInCanvasWidth() {
                let newScale = sceneFlick.effectiveSceneSize.width / sceneSize.width;
                print(newScale)
                setNewScale(newScale, Qt.point(width / 2, height / 2));
            }
        }
    }

    A.RoundButton {
        id: addNodeButton
        visible: !Api.scene.hasSelection && !Api.readonlyMode
        anchors { right: parent.right; bottom: parent.bottom; margins: width / 2; }

        A.ToolTip {
            property bool isEmptyGroup: {
                if (Api.groups.hasAnyGroup) {
                    if (Api.scene.hasCurrentGroup) {
                        return Api.groups.isEmptyGroup(Api.scene.currentGroup.uuid);
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
            enabled: root.thisPageVisible && !Api.readonlyMode
            onTriggered: {
                if (Api.groups.hasAnyGroup) {
                    root.StackView.view.push(newNodeComponent)
                }
            }
        }
    }

    A.RoundButton {
        id: editNodeButton
        visible: Api.scene.hasSelection && !Api.readonlyMode
        anchors { right: parent.right; bottom: parent.bottom; margins: width / 2; }

        action: Action {
            id: changeNodeAction
            text: "Изменить вершину"
            icon.source: Theme.icon.pencil
            shortcut: "Ctrl+e"
            enabled: root.thisPageVisible && !Api.readonlyMode
            onTriggered: {
                if (!Api.scene.currentNode.isNull()) {
                    root.StackView.view.push(editNodeComponent)
                }
            }
        }
    }

    A.RoundButton {
        id : nodeInfoButton
        visible: Api.scene.hasSelection
        anchors { right: editNodeButton.left; bottom: parent.bottom; margins: width / 2; }

        action: Action {
            id: nodeInfoAction
            icon.source: Theme.icon.info
            shortcut: "Ctrl+i"
            onTriggered: {
                if (Api.scene.hasSelection)
                    termDrawer.openWithNode(Api.scene.currentNode);
            }
        }
    }

    M.LoadingInProcess {
        anchors.fill: sceneFlick
        showLoading: Api.scene.groupLoading
    }

    M.EmptyView {
        anchors.fill: sceneFlick
        visible: !Api.groups.hasAnyGroup

        mainText: "Группы отсутствуют"
        detailedText: "Думаю вам стоит создать одну"
        buttonText: "Перейти в список групп"
        onClicked: root.openGroupsList()
    }

    M.EmptyView {
        anchors.fill: sceneFlick
        visible: Api.groups.hasAnyGroup && !Api.scene.hasCurrentGroup && !Api.scene.groupLoading

        mainText: "Группа не выбрана"
        detailedText: ""
        buttonText: "Открыть список групп"
        onClicked: root.openMainMenu()
    }
}
