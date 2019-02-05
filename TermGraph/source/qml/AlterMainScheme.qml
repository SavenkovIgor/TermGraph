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

    signal showInfo(string info)
    signal showWarning(string warning)
    signal showError(string error)

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
        addNodeButton.visible = false
    }

    function hideButtons() {
        editNodeButton.visible = false
        deleteNodeButton.visible = false
        nodeInfoButton.visible = false
        addNodeButton.visible = true
    }

    Connections {
        target: sceneObj
        onSomeSelected: showButtons()
        onSelectionDropSignal: hideButtons()
        onSelectionDoubleClick: nodeInfoButton.openTerm()
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

    TermGroupsList {
        id: groupsListPage
        mainStack: stackView
    }

    TermView {
        id: termViewPage
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
        visible: true
        z: 3

        anchors {
            right: parent.right
            bottom: parent.bottom
        }

        Shortcut {
            sequence: "Ctrl+n"
            onActivated: addNodeButton.openNewNodePage()
        }

        onClicked: openNewNodePage()
        Component.onCompleted: loadIcon( "qrc:/icons/plus" )

        function openNewNodePage() {
            if (groupsManager.hasAnyGroup()) {
                newNodePage.open()
                newNodePage.prepare("")
            } else {
                showWarning("Create group first!")
                groupsListPage.open()
            }
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
            onActivated: editNodeButton.openEditNodePage(sceneObj.getCurrNodeUuid())
        }

        onClicked: openEditNodePage(sceneObj.getCurrNodeUuid())
        Component.onCompleted: loadIcon( "qrc:/icons/pencil" )

        function openEditNodePage(nodeUuid) {
            if (nodeUuid !== "") {
                newNodePage.open()
                newNodePage.prepare(nodeUuid)
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
        Component.onCompleted: loadIcon( "qrc:/icons/info" )

        function openTerm() {
            if (sceneObj.hasSelection()) {
                termViewPage.open()
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

    Drawer {
        id : groupSelectDrw

        width: Math.min(Math.max(window.width*0.5, groupListView.width), window.width*0.6)
        height: window.height

        interactive: mainStack.currentItem == alterMainScheme
        edge: Qt.RightEdge

        onOpened: groupListView.forceActiveFocus()

        ListView {
            id: groupListView
            anchors.fill: parent
            model: groupsManager.getAllUuidStringsSortedByLastEdit()

            function refreshModel() {
                model = groupsManager.getAllUuidStringsSortedByLastEdit()
            }

            Rectangle {
                anchors.fill: parent
                color: "#4f4f4f"
                z: -1
            }

            keyNavigationEnabled: true

            delegate: Rectangle {
                id: groupLstDlgt
                anchors.left: parent.left
                anchors.right: parent.right

                color: "transparent"

                height: curText.height

                states: State {
                    name: "Current"
                    when: groupLstDlgt.ListView.isCurrentItem
                    PropertyChanges { target: groupLstDlgt; color: "darkGray" }
                }

                Text {
                    id: curText
                    padding: 30

                    font.weight: Font.Thin
                    height: Math.floor( font.pixelSize*2.0 )

                    color: "#e8e8e8"

                    text: groupsManager.getGroupName(modelData)
                    font.pixelSize: mainObj.getUiElementSize("text")*Screen.pixelDensity
                    anchors.fill: parent
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignLeft
                }

                Rectangle {
                    x: Math.max((parent.width - width) / 2, 1)
                    y: parent.height - height
                    height: Math.max(parent.height / 100, 1)
                    width: parent.width * 0.95
                    color: "#e8e8e8"
                }

                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        groupSelectDrw.close()
                        sceneObj.showGroup(modelData)
                    }
                }
            }
        }
    }
}
