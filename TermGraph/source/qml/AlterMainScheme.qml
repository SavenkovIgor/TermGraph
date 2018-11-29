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
        }

        Canvas {
            id: sceneImage

            height: 100
            width: 100

            renderStrategy: Canvas.Immediate
            renderTarget: Canvas.Image

            Component.onCompleted: {
                updateSize()
                sceneObj.resetPaintFlags()
            }

            Connections {
                target: sceneObj
                onSceneUpdated: sceneImage.updateSize()
                onRepaintQmlScene: sceneImage.requestPaint()
            }

            function updateSize() {
                sceneImage.height = sceneObj.rect.height
                sceneImage.width = sceneObj.rect.width
            }

            onPaint: {
                sceneObj.setPaintInProcess(true)
                var ctx = sceneImage.getContext('2d')

                paintAll(ctx)
                sceneObj.setPaintInProcess(false)
            }

            function paintAll(ctx) {
                sceneObj.startCheckTimer()

                sceneObj.updatePaintQueuesInAllGroups()

                while (true) {
                    if (sceneObj.groupPaintQueueEmpty())
                        break;

                    var groupRect = sceneObj.currentGroupRect()
                    // JsPaint.clearRect(ctx, groupRect, 2)
                    JsPaint.paintRect(ctx, groupRect, "#FFFFFF")

                    var groupName = sceneObj.currentGroupName()
                    var groupNamePos = sceneObj.currentGroupNamePos()
                    JsPaint.paintGroupName(ctx, groupName, groupNamePos)

                    paintAllEdgesInThisGroup(ctx)
                    paintAllRectsInThisGroup(ctx)

                    sceneObj.nextPaintGroup()
                }
            }

            function paintAllEdgesInThisGroup(ctx) {
                JsPaint.prepareEdge(ctx, sceneObj.getEdgeColor())

                while (true) {
                    if (sceneObj.edgeQueueEmpty()) {
                        break;
                    }

                    var pt1 = sceneObj.currentFirstEdgePoint()
                    var pt2 = sceneObj.currentLastEdgePoint()

                    JsPaint.paintLine(ctx, pt1, pt2)

                    sceneObj.nextEdge()
                }
            }

            function paintAllRectsInThisGroup(ctx) {
                JsPaint.prepareRoundedRects(ctx)
                JsPaint.prepareText(ctx)

                for (var j = 0; j < 1000000; j++) {
                    if (sceneObj.nodeQueueEmpty()) {
                        break;
                    }

                    var rect = sceneObj.currentNodeRect()
                    var color = sceneObj.currentNodeColor()
                    var radius = sceneObj.currentNodeRadius()

                    JsPaint.paintRoundedRect(ctx, rect, color, radius)

                    var center = sceneObj.currentNodeCenter()
                    var text = sceneObj.currentNodeText()

                    JsPaint.paintTextWithSplit(ctx, text, center, rect)

                    sceneObj.nextNode()
                }
            }
        }
    }
}
