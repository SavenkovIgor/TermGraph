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
        onGroupsListChanged: {
            groupListView.refreshModel()
        }
    }

    Connections {
        target: nodesManager
        onNodeChanged: {
            groupListView.refreshModel()
        }
    }

    Flickable {
        anchors.fill: parent

        contentWidth: mainSceneImg.width
        contentHeight: mainSceneImg.height

        boundsBehavior: Flickable.StopAtBounds

        Canvas {
            id: mainSceneImg

            height: 100
            width: 100

            property color fillStyle: "#ae32a0" // purple

            Component.onCompleted: {
                updateSize()
                requestPaint()
            }

            Connections {
                target: sceneObj
                onSceneUpdated: {
                    updateSize()
                }
            }

            function updateSize() {
                mainSceneImg.height = sceneObj.rect.height
                mainSceneImg.width = sceneObj.rect.width
            }


            onPaint: {
                var ctx = mainSceneImg.getContext('2d')
                paintAll(ctx)
            }

            function paintAll(ctx) {
                sceneObj.startCheckTimer()
                JsPaint.paintFilledRect(ctx, sceneObj.rect, sceneObj.getSceneBackgroundColor())
                paintAllEdges(ctx)
                sceneObj.restartCheckTimer()
                paintAllRects(ctx)
                sceneObj.restartCheckTimer()
                paintAllTexts(ctx)
                sceneObj.restartCheckTimer()
            }

            function paintAllEdges(ctx) {
                sceneObj.startGroupIterator()

                JsPaint.prepareEdge(ctx, sceneObj.getEdgeColor())
                for (var i = 0; i < 1000000; i++) {
                    if (sceneObj.groupIteratorAtEnd()) {
                        break;
                    }

                    sceneObj.startEdgeIterator()


                    for (var j = 0; j < 1000000; j++) {
                        if (sceneObj.edgeIteratorAtEnd()) {
                            break;
                        }

                        var pt1 = sceneObj.currentFirstEdgePoint()
                        var pt2 = sceneObj.currentLastEdgePoint()

                        JsPaint.paintLine(ctx, pt1, pt2)

                        sceneObj.nextEdge()
                    }
                    sceneObj.nextGroup()
                }
            }

            function paintAllRects(ctx) {
                sceneObj.startGroupIterator()

                for (var i = 0; i < 1000000; i++) {
                    if (sceneObj.groupIteratorAtEnd()) {
                        break;
                    }

                    var groupName = sceneObj.currentGroupName()
                    var groupRect = sceneObj.currentGroupRect()
                    JsPaint.paintRect(ctx, groupRect, "#FFFFFF")

                    var groupNamePos = sceneObj.currentGroupNamePos()
                    JsPaint.paintGroupName(ctx, groupName, groupNamePos)

                    sceneObj.startNodeIterator()
                    JsPaint.prepareRoundedRects(ctx)

                    for (var j = 0; j < 1000000; j++) {
                        if (sceneObj.nodeIteratorAtEnd()) {
                            break;
                        }

                        var rect = sceneObj.currentNodeRect()
                        var color = sceneObj.currentNodeColor()
                        var radius = sceneObj.currentNodeRadius()

                        JsPaint.paintRoundedRect(ctx, rect, color, radius)

                        sceneObj.nextNode()
                    }
                    sceneObj.nextGroup()
                }
            }

            function paintAllTexts(ctx) {
                sceneObj.startGroupIterator()

                for (var i = 0; i < 1000000; i++) {
                    if (sceneObj.groupIteratorAtEnd()) {
                        break;
                    }

                    sceneObj.startNodeIterator()
                    JsPaint.prepareText(ctx)

                    for (var j = 0; j < 1000000; j++) {
                        if (sceneObj.nodeIteratorAtEnd()) {
                            break;
                        }

                        var nodeName = sceneObj.currentNodeText()
                        var rect = sceneObj.currentNodeRect()
                        var center = sceneObj.currentNodeCenter()
                        JsPaint.paintText(ctx, nodeName, center, rect)

                        sceneObj.nextNode()
                    }
                    sceneObj.nextGroup()
                }
            }
        }
    }
}
