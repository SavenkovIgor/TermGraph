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

        Component.onCompleted: mainHeader.showMenuIcon()

        onMenuClick: sideMenu.open()
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
                mainSceneImg.height = sceneObj.size.height
                mainSceneImg.width = sceneObj.size.width
            }

            onPaint: {
                var ctx = mainSceneImg.getContext('2d')

                sceneObj.startGroupIterator()
                for (var i = 0; i < 1000000; i++) {
                    if (sceneObj.groupIteratorAtEnd()) {
                        break;
                    }

                    var name = sceneObj.startNodeIterator()
                    for (var j = 0; j < 1000000; j++) {
                        if (sceneObj.nodeIteratorAtEnd()) {
                            break;
                        }

                        var nodeName = sceneObj.currentNodeText()
                        var rect = sceneObj.currentNodeRect()
                        var color = sceneObj.currentNodeColor()

                        JsPaint.paintNode(ctx, nodeName, rect, color)

                        sceneObj.nextNode()
                    }

                    sceneObj.nextGroup()
                }
            }
        }
    }
}
