import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.0
import QtQuick.Window 2.3
import QtQuick.Layouts 1.3
import QtQuick.Dialogs 1.2

import "UIExtensions"

Page {

    id: mainScheme

    property StackView mainStack
    property Drawer sideMenu
    property int scOffset: mainHeader.height

    anchors.fill: parent

    function openGroupList() {
        groupSelectDrw.open()
    }

    function toTop() {
        mouseAr.forceActiveFocus()
    }

    header: MainHeader {

        id: mainHeader
        titleText: "TermGraph"

        Component.onCompleted: mainHeader.showMenuIcon()

        onMenuClick: sideMenu.open()
    }

    function showButtons() {
        changePointBtn.visible = true
        deleteSelNodeBtn.visible = true
        opInfoBtn.visible = true
    }

    function hideButtons() {
        changePointBtn.visible = false
        deleteSelNodeBtn.visible = false
        opInfoBtn.visible = false
    }

    Connections {
        target: sceneObj
        onSomeSelected: {
            showButtons()
        }
        onSelectionDrop: {
            hideButtons()
        }
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

    NewNodeEdit {
        id: newNodePage
        mainStack: stackView
    }

    function openNewNodePage() {
        newNodePage.prepare("")
        mainStack.push(newNodePage)
    }

    function openEditNodePage(nodeUuid) {
        newNodePage.prepare(nodeUuid)
        mainStack.push(newNodePage)
    }

    Frame {
        id: sceneFrame

        anchors.fill: parent

        anchors.margins: 0
        padding: 0

        Timer {
            id: tim
            repeat: true
            interval: 2000

            Component.onCompleted: {
                tim.start();
            }

            onTriggered: {
                mainSceneImg.requestPaint()
            }
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

        Canvas {
            id: mainSceneImg
            anchors.fill: parent

            function sendSizeChange() {
                mainViewObj.setSceneSize(width,height)
            }

            function nodeDeleteCheck() {
                nodeDelDialog.visible = true
            }

            onWidthChanged: sendSizeChange()
            onHeightChanged: sendSizeChange()

            property string imgPath: "image://sceneimage/main2"
            property int imgNum: 0

            onImageLoaded: {
                requestPaint()
            }

            Component.onCompleted: {
                loadImage(imgPath + mainSceneImg.imgNum)
            }

            onPaint: {
                var ctx = mainSceneImg.getContext('2d')
                ctx.drawImage(imgPath + imgNum,0,0)
                unloadImage(imgPath + imgNum)
                imgNum++
                loadImage( imgPath + imgNum )
            }

            MyRoundButton {
                id: addNode

                anchors {
                    right: showGlpLst.left
                    top: mainSceneImg.top
                }

                onClicked: openNewNodePage()
                Component.onCompleted: loadIcon( "qrc:/icons/plus" )
            }

            MyRoundButton {
                id: showGlpLst

                anchors {
                    right: mainSceneImg.right
                    top: mainSceneImg.top
                }

                onClicked: openGroupList()
                Component.onCompleted: loadIcon( "qrc:/icons/spreadsheet")
            }

            MyRoundButton {
                id: changePointBtn

                anchors {
                    right: mainSceneImg.right
                    bottom: mainSceneImg.bottom
                }

                onClicked: {
                    openEditNodePage(sceneObj.getCurrNodeLongUid())
                }

                Component.onCompleted: loadIcon( "qrc:/icons/aperture" )
            }

            MyRoundButton {
                id : deleteSelNodeBtn

                anchors {
                    right: mainSceneImg.right
                    bottom: changePointBtn.top
                }

                onClicked: mainSceneImg.nodeDeleteCheck()
                Component.onCompleted: loadIcon( "qrc:/icons/ban" )
            }

            MyRoundButton {
                id : opInfoBtn

                anchors.right: changePointBtn.left
                anchors.bottom: mainSceneImg.bottom

                Shortcut {
                    sequence: "Ctrl+i"
                    onActivated: {
                        if( sceneObj.hasSelection() )
                            nodeInfoDrw.open()
                    }
                }

                onClicked: nodeInfoDrw.open()
                Component.onCompleted: loadIcon( "qrc:/icons/chevron-top" )
            }

            Drawer {
                id : nodeInfoDrw
                width: mainSceneImg.width
                height: mainSceneImg.height*0.9
                interactive: true //TODO: Переделать. Опять таки прокрутка плохо работает
                dragMargin: 0

                edge: Qt.BottomEdge

                onOpened: {
                    if (sceneObj.getCurrNodeDebugInfo() !== "") {
                        ptDbgInfo.text = sceneObj.getCurrNodeDebugInfo()
                        ptDbgInfo.visible = true
                    } else {
                        ptDbgInfo.visible = false
                    }

                    if (sceneObj.getCurrNodeName() !== "") {
                        ptName.text = sceneObj.getCurrNodeName()
                        ptName.visible = true
                    } else {
                        ptName.visible = false
                    }

                    if (sceneObj.getCurrNodeDefinition() !== "") {
                        ptDefinition.text = sceneObj.getCurrNodeName() + " - это " + sceneObj.getCurrNodeDefinition()
                        ptDefinition.visible = true
                    } else {
                        ptDefinition.visible = false
                    }

                    if (sceneObj.getCurrNodeDescription() !== "") {
                        ptDescription.text = sceneObj.getCurrNodeDescription()
                        ptDescription.visible = true
                    } else {
                        ptDescription.visible = false
                    }

                    if (sceneObj.getCurrNodeExamples() !== "") {
                        ptExamples.text = sceneObj.getCurrNodeExamples()
                        ptExamples.visible = true
                    } else {
                        ptExamples.visible = false
                    }
                }

                onClosed: {
                    ptDbgInfo.text     = ""
                    ptName.text        = ""
                    ptDefinition.text  = ""
                    ptDescription.text = ""
                    ptExamples.text    = ""
                }

                Column {
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.top: parent.top

                    spacing: 14

                    anchors.margins: {
                        left: 40
                        bottom: 40
                        right: 40
                    }

                    MyLabelPair{
                        id: ptDbgInfo
                        name: "Dbg:"
                    }

                    MyLabelPair{
                        id: ptName
                        name: "Название:"
                    }

                    MyLabelPair{
                        id: ptDefinition
                        name: "Определение:"
                        onHeightChanged: {
                            console.log("hgc")
                        }
                    }

                    MyLabelPair{
                        id: ptDescription
                        name: "Описание:"
                    }

                    MyLabelPair{
                        id: ptExamples
                        name: "Пример:"
                    }

//                    Item{
//                        Layout.fillHeight: true
//                    }
                }
            }

            MouseArea {
                id: mouseAr
                anchors.fill: parent
                hoverEnabled: true

                acceptedButtons: Qt.AllButtons;

                //Функция коррекции по вертикали
                function getHgh() {
                    return mainScheme.scOffset
                }

                //Вызов ивентов мышки
                function sendMouseEvt(actType,btnPress) {

                    if( btnPress & Qt.LeftButton ) {
                        mainObj.takeSceneMouse(mouseX,mouseY,window.x,window.y,getHgh(),actType,"left");
                    } else if( btnPress & Qt.RightButton ) {
                        mainObj.takeSceneMouse(mouseX,mouseY,window.x,window.y,getHgh(),actType,"right");
                    } else {
                        mainObj.takeSceneMouse(mouseX,mouseY,window.x,window.y,getHgh(),actType,"");
                    }
                }

                onPressed: {
                    forceActiveFocus()
                    sendMouseEvt("move", this.pressedButtons )
                    sendMouseEvt ("press", this.pressedButtons )
                    mouse.accepted = true
                }

                onReleased: {
                    sendMouseEvt("move", this.pressedButtons )
                    sendMouseEvt ("release", this.pressedButtons )
                    mouse.accepted = true
                }

                onMouseXChanged: {
                    sendMouseEvt ("move", this.pressedButtons )
                    mouse.accepted = true
                }
                onMouseYChanged: {
                    sendMouseEvt ("move", this.pressedButtons )
                    mouse.accepted = true
                }

                Keys.onDeletePressed: {
                    mainSceneImg.nodeDeleteCheck()
                }

                onWheel: {
                    if( wheel.modifiers & Qt.ControlModifier ) {
                        if( wheel.pixelDelta.y > 0 )
                            mainViewObj.scaleUp()
                        else
                            mainViewObj.scaleDown()
                    }
                }

                Keys.onPressed: {

                    if( event.modifiers & Qt.ControlModifier ) {
                        if( event.key === Qt.Key_BracketLeft ) {
                            sceneObj.toPreviousGroup()
                            event.accepted = true
                        }

                        if( event.key === Qt.Key_BracketRight ) {
                            sceneObj.toNextGroup()
                            event.accepted = true
                        }

                        if( event.key === Qt.Key_E ) {
                            openEditNodePage(sceneObj.getCurrNodeLongUid())
                            event.accepted = true
                        }

                        if( event.key === Qt.Key_N ) {
                            openNewNodePage()
                            event.accepted = true
                        }

                        if( event.key === Qt.Key_Plus ) {
                            mainViewObj.scaleUp();
                            event.accepted = true
                        }

                        if( event.key === Qt.Key_Minus ) {
                            mainViewObj.scaleDown();
                            event.accepted = true
                        }
                    }
                    if( event.modifiers & Qt.AltModifier ) {
                        if( event.key === Qt.Key_Left ) {
                            mainViewObj.moveLeft()
                            event.accepted = true
                        }

                        if( event.key === Qt.Key_Right ) {
                            mainViewObj.moveRight()
                            event.accepted = true
                        }

                        if( event.key === Qt.Key_Up ) {
                            mainViewObj.moveUp()
                            event.accepted = true
                        }

                        if( event.key === Qt.Key_Down ) {
                            mainViewObj.moveDown()
                            event.accepted = true
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
                        model: groupsManager.getAllGroupsNames(true)

                        function refreshModel() {
                            model = groupsManager.getAllGroupsNames(true)
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
        }
    }
}
