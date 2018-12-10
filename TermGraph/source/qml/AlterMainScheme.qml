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

    NewNodeEdit {
        id: newNodePage
        mainStack: stackView
    }

    TermView {
        id: termView
        mainStack: stackView
    }

    Rectangle {
        id: sceneBackground
        anchors.fill: parent
        z: 1

        color: sceneObj.getSceneBackgroundColor()
    }

    MyRoundButton {
        id: addNodeButton
        z: 3

        anchors {
            right: showGroupListButton.left
            top: parent.top
        }

        Shortcut {
            sequence: "Ctrl+n"
            onActivated: addNodeButton.openNewNodePage()
        }

        onClicked: openNewNodePage()
        Component.onCompleted: loadIcon( "qrc:/icons/plus" )

        function openNewNodePage() {
            newNodePage.prepare("")
            alterMainScheme.mainStack.push(newNodePage)
        }
    }

    MyRoundButton {
        id: showGroupListButton
        z: 3

        anchors {
            right: parent.right
            top: parent.top
        }

        onClicked: groupSelectDrw.open()
        Component.onCompleted: loadIcon( "qrc:/icons/spreadsheet")
    }

    MyRoundButton {
        id: editNodeButton
        z: 3
        visible: false

        anchors {
            right: parent.right
            bottom: parent.bottom
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
                alterMainScheme.mainStack.push(newNodePage)
            }
        }
    }

    MyRoundButton {
        id : deleteNodeButton
        z: 3
        visible: false

        anchors {
            right: parent.right
            bottom: editNodeButton.top
        }

        onClicked: nodeDelDialog.visible = true
        Component.onCompleted: loadIcon( "qrc:/icons/ban" )
    }

    MessageDialog {
        id: nodeDelDialog

        title: "Удаление вершины"
        text:  "Удалить выделенную вершину?"

        standardButtons: StandardButton.Yes | StandardButton.No
        icon: StandardIcon.Question

        onYes: {
            sceneObj.deleteSelectedNode()
            nodeDelDialog.visible = false
        }
    }

    MyRoundButton {
        id : nodeInfoButton
        z: 3
        visible: false

        anchors.right: editNodeButton.left
        anchors.bottom: parent.bottom

        Shortcut {
            sequence: "Ctrl+i"
            onActivated: nodeInfoButton.openTerm()
        }

        onClicked: openTerm()
        Component.onCompleted: loadIcon( "qrc:/icons/chevron-top" )

        function openTerm() {
            if (sceneObj.hasSelection()) {
                alterMainScheme.mainStack.push(termView)
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

            property bool paintGroups: false
            property bool paintNode: false
            property bool clearAll: false

            renderStrategy: Canvas.Cooperative
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
                    sceneImage.requestPaint()
                }
                onPaintNodeQueue: {
                    sceneImage.paintNode = true
                    sceneImage.requestPaint()
                }
                onClearAll: {
                    sceneImage.clearAll = true
                    sceneImage.requestPaint()
                }
            }

            function updateSize() {
                sceneImage.height = sceneObj.rect.height
                sceneImage.width = sceneObj.rect.width
            }

            onPaint: {
                console.log("Paint call")
                paintManager.setPaintInProcessFlag(true)
                var ctx = sceneImage.getContext('2d')

                if (sceneImage.paintGroups) {
                    paintAll(ctx)
                    sceneImage.paintGroups = false
                }

                if (sceneImage.paintNode) {
                    sceneObj.restartCheckTimer("signalReceived");
                    paintNodes(ctx)
                    sceneObj.restartCheckTimer("paintEnded");
                    sceneImage.paintNode = false
                }

                if (sceneImage.clearAll) {
                    var rect = {x:0, y:0, width:sceneImage.width, height:sceneImage.height}
                    JsPaint.clearRect(ctx, rect, 0)
                    sceneImage.clearAll = false
                }

                paintManager.setPaintInProcessFlag(false)
            }

            function paintAll(ctx) {
                paintGroupRects(ctx)
                paintGroupNames(ctx)
                paintEdges(ctx)
                paintNodes(ctx)
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

    Drawer {
        id : groupSelectDrw

        width: window.width*0.6
        height: window.height

        interactive: true
        edge: Qt.RightEdge

        onOpened: groupListView.forceActiveFocus()

        ListView {

            id: groupListView
            anchors.fill: parent
            model: groupsManager.getAllGroupsNames()

            function refreshModel() {
                model = groupsManager.getAllGroupsNames()
            }

            keyNavigationEnabled: true

            highlight: Rectangle {
                width: 200; height: 20
                color: "#FFFF88"
                y: groupListView.currentItem.y;
            }


            delegate: Rectangle {
                id: groupLstDlgt
                border.color: "lightGray"
                border.width: 1
                anchors.left: parent.left
                anchors.right: parent.right

                property alias text: curText.text

                height: curText.height
                states: State {
                    name: "Current"
                    when: groupLstDlgt.ListView.isCurrentItem
                    PropertyChanges { target: groupLstDlgt; color: "darkGray" }
                }

                Text{
                    id: curText
                    padding: 30

                    font.weight: Font.Thin
                    height: Math.floor( font.pixelSize*2.0 )

                    text: modelData
                    font.pixelSize: mainObj.getUiElementSize("text")*Screen.pixelDensity
                    anchors.fill: parent
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignLeft
                }

                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        groupSelectDrw.close()
                        sceneObj.showGroup(curText.text)
                    }
                }
            }
        }
    }
}
