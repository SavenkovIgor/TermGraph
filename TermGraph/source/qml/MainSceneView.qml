import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.0
import QtQuick.Window 2.12
import QtQuick.Layouts 1.12
import QtQuick.Dialogs 1.3

import "UIExtensions"

import "JsExtensions/nodePaint.js" as JsPaint

Page {
    id: mainSceneView

    property StackView mainStack
    property Drawer sideMenu
    property int scOffset: mainHeader.height

    signal showInfo(string info)
    signal showWarning(string warning)
    signal showError(string error)

    header: MainHeader {

        id: mainHeader
        titleText: "TermGraph"

        Component.onCompleted: mainHeader.showMenuIcon()
        onMenuClick: sideMenu.open()
    }

    property string someSelectedSt: "SomeSelected"
    property string noneSelectedSt: "NoneSelected"

    state: sceneObj.hasSelection ? someSelectedSt : noneSelectedSt

    states: [
        State {
            name: someSelectedSt
            PropertyChanges { target: editNodeButton;   visible: true }
            PropertyChanges { target: deleteNodeButton; visible: true }
            PropertyChanges { target: nodeInfoButton;   visible: true }
            PropertyChanges { target: addNodeButton;    visible: false }
        },
        State {
            name: noneSelectedSt
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

    NewNode {
        id: newNodePage
        mainStack: stackView
    }

    EditNode {
        id: editNodePage
        mainStack: stackView
    }

    TermGroupsList {
        id: groupsListPage
    }

    TermView {
        id: termViewPage
        mainStack: stackView
    }

    Rectangle {
        id: sceneBackground

        z: 1
        anchors.fill: parent

        color: sceneObj.getSceneBackgroundColor()
    }

    MyRoundButton {
        id: addNodeButton

        z: 3
        anchors { right: parent.right; bottom: parent.bottom; }
        visible: true

        iconName: "plus"
        onClicked: openNewNodePage()

        function openNewNodePage() {
            if (groupsManager.hasAnyGroup) {
                newNodePage.open()
            } else {
                showWarning("Create group first!")
                groupsListPage.open()
            }
        }

        Shortcut { sequence: "Ctrl+n"; onActivated: addNodeButton.openNewNodePage(); }
    }

    MyRoundButton {
        id: showGroupListButton
        z: 3
        iconName: "spreadsheet"
        visible: groupsManager.hasAnyGroup

        anchors { top: parent.top; right: parent.right; }

        onClicked: groupsList.open()
    }

    MyRoundButton {
        id: editNodeButton
        z: 3
        iconName: "pencil"
        visible: false

        anchors { right: parent.right; bottom: parent.bottom; }

        Shortcut {
            sequence: "Ctrl+e"
            onActivated: editNodeButton.openEditNodePage(sceneObj.currentNode.uuid)
        }

        onClicked: openEditNodePage(sceneObj.currentNode.uuid)


        function openEditNodePage(nodeUuid) {
            if (nodeUuid !== "") {
                editNodePage.open()
                editNodePage.nodeUuid = nodeUuid
            }
        }
    }

    MyRoundButton {
        id : deleteNodeButton
        z: 3
        iconName: "trash"
        visible: false

        anchors { right: parent.right; bottom: editNodeButton.top; }

        onClicked: nodeDelDialog.visible = true
    }

    MessageDialog {
        id: nodeDelDialog

        title: "Удаление вершины"
        text:  "Удалить выделенную вершину?"

        standardButtons: StandardButton.Yes | StandardButton.No
        icon: StandardIcon.Question

        onYes: sceneObj.deleteSelectedNode()
    }

    MyRoundButton {
        id : nodeInfoButton
        z: 3
        iconName: "info"
        visible: false

        anchors { right: editNodeButton.left; bottom: parent.bottom; }

        Shortcut {
            sequence: "Ctrl+i"
            onActivated: nodeInfoButton.openTerm()
        }

        onClicked: openTerm()

        function openTerm() {
            if (sceneObj.hasSelection) {
                termViewPage.open()
            }
        }
    }

    GroupsDrawer {
        id : groupsList

        y: mainHeader.height
        width: Math.min(window.width * 0.8, groupsList.maxWidth)
        height: sceneView.height

        clip: true
        interactive: mainStack.currentItem == mainSceneView

        Shortcut {
            sequence: "Ctrl+Left"
            onActivated: {
                groupsList.open();
                groupsList.forceActiveFocus();
            }
        }
    }

    Flickable {
        id: sceneView
        anchors.fill: parent
        z: 2

        contentWidth: sceneCanvas.width
        contentHeight: sceneCanvas.height

        boundsBehavior: Flickable.StopAtBounds

        onContentXChanged: sendSceneViewRect()
        onContentYChanged: sendSceneViewRect()
        onWidthChanged: sendSceneViewRect()
        onHeightChanged: sendSceneViewRect()

        function sendSceneViewRect() {
            sceneObj.setSceneViewRect(sceneView.contentX, sceneView.contentY, sceneView.width, sceneView.height)
        }

        MouseArea {
            id: sceneMouse

            anchors.fill: sceneCanvas

            hoverEnabled: true

            onMouseXChanged: sceneObj.setMousePos(sceneMouse.mouseX, sceneMouse.mouseY)
            onMouseYChanged: sceneObj.setMousePos(sceneMouse.mouseX, sceneMouse.mouseY)

            onClicked: sceneObj.setMouseClick(sceneMouse.mouseX, sceneMouse.mouseY)
        }

        Canvas {
            id: sceneCanvas

            height: 100
            width: 100

            property bool paintGroups: false
            property bool paintNode: false

            renderStrategy: Canvas.Cooperative
            renderTarget: Canvas.Image

            Component.onCompleted: {
                // Try to show first group
                var groupsUuids = groupsManager.getAllUuidStringsSortedByLastEdit()

                if (groupsUuids.length !== 0) {
                    sceneObj.showGroup(groupsUuids[0])
                }
            }

            Connections {
                target: sceneObj
                onSceneContentUpdated: sceneCanvas.updateSize()
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

            function updateSize() {
                sceneCanvas.height = sceneObj.getSceneRect().height
                sceneCanvas.width = sceneObj.getSceneRect().width
            }

            onPaint: {
                // console.log("Paint call")
                paintManager.setPaintInProcessFlag(true)
                var ctx = sceneCanvas.getContext('2d')

                if (sceneCanvas.paintGroups) {
                    paintAll(ctx)
                    sceneCanvas.paintGroups = false
                }

                if (sceneCanvas.paintNode) {
                    paintNodes(ctx)
                    sceneCanvas.paintNode = false
                }

                paintManager.setPaintInProcessFlag(false)
            }

            function paintAll(ctx) {
                clearRects(ctx)
                paintGroupRects(ctx)
                paintGroupNames(ctx)
                paintEdges(ctx)
                paintNodes(ctx)
            }

            function clearRects(ctx) {

                while (true) {
                    if (paintManager.clearQueueEmpty())
                        break;

                    var rect = paintManager.currentClearRect()
                    JsPaint.clearRect(ctx, rect, 0)

                    paintManager.nextClearRect()
                }
            }

            function paintGroupRects(ctx) {

                while (true) {
                    if (paintManager.groupRectQueueEmpty())
                        break;

                    var groupRect = paintManager.currentGroupRect()
                    // JsPaint.clearRect(ctx, groupRect, 2)
                    JsPaint.paintRect(ctx, groupRect, "#FFFFFF")

                    paintManager.nextGroupRect()
                }
            }

            function paintGroupNames(ctx) {

                while (true) {
                    if (paintManager.groupNamesQueueEmpty())
                        break;

                    var groupName = paintManager.currentGroupName()
                    var groupNamePos = paintManager.currentGroupNamePos()
                    JsPaint.paintGroupName(ctx, groupName, groupNamePos)

                    paintManager.nextGroupName()
                }
            }

            function paintEdges(ctx) {
                JsPaint.prepareEdge(ctx)

                while (true) {
                    if (paintManager.edgeQueueEmpty()) {
                        break;
                    }

                    var pt1 = paintManager.currentFirstEdgePoint()
                    var pt2 = paintManager.currentLastEdgePoint()
                    var col = paintManager.getEdgeColor()

                    JsPaint.paintLine(ctx, pt1, pt2, col)

                    paintManager.nextEdge()
                }
            }

            function paintNodes(ctx) {
                JsPaint.prepareRects(ctx)

                while (true) {
                    if (paintManager.nodeQueueEmpty()) {
                        break;
                    }

                    var rect = paintManager.currentNodeRect()
                    var color = paintManager.currentNodeColor()
                    var radius = paintManager.currentNodeRadius()

                    JsPaint.paintRoundedRect(ctx, rect, color, radius)

                    var center = paintManager.currentNodeCenter()
                    var text = paintManager.currentNodeText()

                    JsPaint.paintTextWithSplit(ctx, text, center, rect)

                    paintManager.nextNode()
                }
            }
        }
    }
}
