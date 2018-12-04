import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.0
import QtQuick.Window 2.3
import QtQuick.Layouts 1.3
import QtQuick.Dialogs 1.2

import "UIExtensions"

import "JsExtensions/nodePaint.js" as JsPaint

Page {

    id: alterMainScheme

    property StackView mainStack
    property Drawer sideMenu
    property int scOffset: mainHeader.height

    anchors.fill: parent

    header: MainHeader {

        id: mainHeader
        titleText: "TermGraph"

        Component.onCompleted: mainHeader.showArrowIcon()
        onMenuClick: mainStack.pop()
    }

    function showButtons() {
        editNodeButton.visible = true
        deleteNodeButton.visible = true
        nodeInfoButton.visible = true
    }

    function hideButtons() {
        editNodeButton.visible = false
        deleteNodeButton.visible = false
        nodeInfoButton.visible = false
    }

    Connections {
        target: sceneObj
        onSomeSelected: showButtons()
        onSelectionDrop: hideButtons()
    }

    Connections {
        target: groupsManager
        onGroupsListChanged: groupListView.refreshModel()
    }

    Connections {
        target: nodesManager
        onNodeChanged: groupListView.refreshModel()
    }

    Rectangle {
        id: sceneBackground
        anchors.fill: parent
        z: 1

        color: sceneObj.getSceneBackgroundColor()
    }

    MyRoundButton {
        id: editNodeButton
        z: 3
        visible: false

        anchors {
            right: alterMainScheme.right
            bottom: alterMainScheme.bottom
        }

        Shortcut {
            sequence: "Ctrl+e"
            onActivated: editNodeButton.openEditNodePage(sceneObj.getCurrNodeLongUid())
        }

        onClicked: openEditNodePage(sceneObj.getCurrNodeLongUid())
        Component.onCompleted: loadIcon( "qrc:/icons/aperture" )

        function openEditNodePage(nodeUuid) {
            if (nodeUuid !== "") {
                newNodePage.prepare(nodeUuid)
                mainStack.push(newNodePage)
            }
        }
    }

    MyRoundButton {
        id : deleteNodeButton
        z: 3
        visible: false

        anchors {
            right: alterMainScheme.right
            bottom: editNodeButton.top
        }

        onClicked: mainSceneImg.nodeDeleteCheck()
        Component.onCompleted: loadIcon( "qrc:/icons/ban" )
    }

    MyRoundButton {
        id : nodeInfoButton
        z: 3
        visible: false

        anchors.right: editNodeButton.left
        anchors.bottom: alterMainScheme.bottom

        Shortcut {
            sequence: "Ctrl+i"
            onActivated: nodeInfoButton.openTerm()
        }

        onClicked: openTerm()
        Component.onCompleted: loadIcon( "qrc:/icons/chevron-top" )

        function openTerm() {
            if (sceneObj.hasSelection()) {
                mainStack.push(termView)
                termView.loadSelectedNode()
            }
        }
    }

    Flickable {
        id: sceneView
        anchors.fill: parent
        z: 2

        contentWidth: sceneImage.width
        contentHeight: sceneImage.height

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

            anchors.fill: sceneImage

            hoverEnabled: true

            onMouseXChanged: sceneObj.setMousePos(sceneMouse.mouseX, sceneMouse.mouseY)
            onMouseYChanged: sceneObj.setMousePos(sceneMouse.mouseX, sceneMouse.mouseY)

            onClicked: sceneObj.setMouseClick(sceneMouse.mouseX, sceneMouse.mouseY)
        }

        Canvas {
            id: sceneImage

            height: 100
            width: 100

            property bool paintGroups: true
            property bool paintNode: false

            renderStrategy: Canvas.Immediate
            renderTarget: Canvas.Image

            Component.onCompleted: {
                updateSize()
                sceneObj.resetPaintFlags()
            }

            Connections {
                target: sceneObj
                onSceneUpdated: sceneImage.updateSize()
            }

            Connections {
                target: paintManager
                onPaintGroupQueue: {
                    sceneImage.paintGroups = true
                    sceneImage.paintNode = false
                    sceneImage.requestPaint()
                }
                onPaintNodeQueue: {
                    sceneImage.paintGroups = false
                    sceneImage.paintNode = true
                    sceneImage.requestPaint()
                }
            }

            function updateSize() {
                sceneImage.height = sceneObj.rect.height
                sceneImage.width = sceneObj.rect.width
            }

            onPaint: {
                paintManager.setPaintInProcessFlag(true)
                var ctx = sceneImage.getContext('2d')

                if (sceneImage.paintGroups)
                    paintAll(ctx)

                if (sceneImage.paintNode)
                    paintNodesOnly(ctx)

                paintManager.setPaintInProcessFlag(false)
            }

            function paintAll(ctx) {
                sceneObj.startCheckTimer()

                while (true) {
                    if (paintManager.groupQueueEmpty())
                        break;

                    paintManager.fillNodeAndEdgeQueuesFromCurrentGroup()

                    var groupRect = paintManager.currentGroupRect()
                    // JsPaint.clearRect(ctx, groupRect, 2)
                    JsPaint.paintRect(ctx, groupRect, "#FFFFFF")

                    var groupName = paintManager.currentGroupName()
                    var groupNamePos = paintManager.currentGroupNamePos()
                    JsPaint.paintGroupName(ctx, groupName, groupNamePos)

                    paintAllEdgesInThisGroup(ctx)
                    paintAllRectsInThisGroup(ctx)

                    paintManager.nextGroup()
                }
            }


            function paintNodesOnly(ctx) {

                JsPaint.prepareRoundedRects(ctx)
                JsPaint.prepareText(ctx)

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

            function paintAllEdgesInThisGroup(ctx) {
                JsPaint.prepareEdge(ctx, paintManager.getEdgeColor())

                while (true) {
                    if (paintManager.edgeQueueEmpty()) {
                        break;
                    }

                    var pt1 = paintManager.currentFirstEdgePoint()
                    var pt2 = paintManager.currentLastEdgePoint()

                    JsPaint.paintLine(ctx, pt1, pt2)

                    paintManager.nextEdge()
                }
            }

            function paintAllRectsInThisGroup(ctx) {
                JsPaint.prepareRoundedRects(ctx)
                JsPaint.prepareText(ctx)

                for (var j = 0; j < 1000000; j++) {
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
