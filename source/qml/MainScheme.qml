import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.0
import QtQuick.Window 2.3
import QtQuick.Layouts 1.3
import QtQuick.Dialogs 1.2

import "UIExtensions"

Item {

    anchors.fill: parent

    id: mainScheme
    objectName: "mainScheme"

    property int scOffset: 0

    function setOffs(hg) {
        console.log("hg " + hg)
        scOffset = hg
    }

    function openGroupList() {
        groupSelectDrw.open()
    }

    Keys.onPressed: {
//        console.log("someGoHere");
    }

    function toTop() {
        mouseAr.forceActiveFocus()
    }

    /*RowLayout {

        id: topTools
        Layout.maximumHeight: 15
        anchors.top : parent.top
        spacing: 15

        Item { width: 1 }


        /*Button {
            objectName: "Browse"
            text: "Browse"
        }*//*
    }*/

    function showButtons() {
        deleteConnectionsBtn.visible = true
        changePointBtn.visible = true
        deleteSelNodeBtn.visible = true
        opInfoBtn.visible = true
    }

    function hideButtons() {
        deleteConnectionsBtn.visible = false
        changePointBtn.visible = false
        deleteSelNodeBtn.visible = false
        opInfoBtn.visible = false
    }

    Connections {
        target: sceneObj
        onSomeSelected : {
            showButtons()
        }
        onSelectDrop : {
            hideButtons()
        }
    }

    Frame {
        id: sceneFrame

//        anchors{
//            top:topTools.bottom
//            left:   parent.left
//            right:  parent.right
//            bottom: parent.bottom
//        }
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
                sceneObj.dropSelectedNode()
                nodeDelDialog.visible = false
            }

        }

        Canvas {
            id: mainSceneImg
            anchors.fill: parent

            function sendSizeChange() {
                mainObj.setSceneSize(width,height)
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
                anchors.right: mainSceneImg.right
                anchors.top: mainSceneImg.top

                onClicked: listView.openNewNode()
                Component.onCompleted: loadIcon( "qrc:/icons/plus" )
            }

            MyRoundButton {
                id : deleteConnectionsBtn

                anchors.right: changePointBtn.left
                anchors.bottom: mainSceneImg.bottom

                onClicked: sceneObj.dropEdgesOnSelected()
                Component.onCompleted: loadIcon( "qrc:/icons/link-broken")
            }

            MyRoundButton {
                id: changePointBtn

                anchors.right: mainSceneImg.right
                anchors.bottom: mainSceneImg.bottom

                onClicked: {
                    mainObj.sceneChangePoint()
                    listView.openNewNode()
                }

                Component.onCompleted: loadIcon( "qrc:/icons/aperture" )
            }

            MyRoundButton {
                id : deleteSelNodeBtn

                anchors.right: mainSceneImg.right
                anchors.bottom: changePointBtn.top

                onClicked: mainSceneImg.nodeDeleteCheck()
                Component.onCompleted: loadIcon( "qrc:/icons/ban" )
            }

            MyRoundButton {
                id : opInfoBtn

                anchors.right: deleteConnectionsBtn.left
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
                    ptDbgInfo.text     = sceneObj.getCurrNodeDebugInfo()
                    ptName.text        = sceneObj.getCurrNodeName()
                    ptDefinition.text  = sceneObj.getCurrNodeDefinition()
                    ptDescription.text = sceneObj.getCurrNodeDescription()
                    ptExamples.text    = sceneObj.getCurrNodeExamples()

                    ptDbgInfo.visible     = ( ptDbgInfo.text     != "" )
                    ptName.visible        = ( ptName.text        != "" )
                    ptDefinition.visible  = ( ptDefinition.text  != "" )
                    ptDescription.visible = ( ptDescription.text != "" )
                    ptExamples.visible    = ( ptExamples.text    != "" )

                    ptIsRoot.visible      = sceneObj.getCurrNodeIsRoot()
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

                    MyLabelPair {
                        id: ptDescription
                        name: "Описание:"
                    }

                    MyLabelPair{
                        id: ptExamples
                        name: "Пример:"
                    }

                    MyLabelPair{
                        id: ptIsRoot
                        name: "Является корневым элементом в группе"
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

                //                onClicked: { }

                //                onWidthChanged: sendResize()
                //                onHeightChanged: sendResize()
                //                Component.onCompleted: sendResize()

                //Функция отправляет ресайз во вьюер sceneView
                //                function sendResize() {
                //                    mainViewObj.setScSize(mainSceneImg.width,mainSceneImg.height)
                //                }

                //Функция коррекции по вертикали
                function getHgh() {
                    return mainScheme.scOffset //+ topTools.height
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
                            mainObj.scaleUp()
                        else
                            mainObj.scaleDown()
                    }
                }

                Keys.onPressed: {

                    if( event.modifiers & Qt.ControlModifier ) {
                        if( event.key === Qt.Key_BracketLeft ) {
                            mainObj.ctrlMove("left")
                            event.accepted = true
                        }

                        if( event.key === Qt.Key_BracketRight ) {
                            mainObj.ctrlMove("right")
                            event.accepted = true
                        }

                        if( event.key === Qt.Key_E ) {
                            mainObj.sceneChangePoint()
                            listView.openNewNode()
                            event.accepted = true
                        }

                        if( event.key === Qt.Key_N ) {
                            mainObj.setChangeNum("")
                            listView.openNewNode()
                            event.accepted = true
                        }

                        if( event.key === Qt.Key_Plus ) {
                            mainObj.scaleUp();
                            event.accepted = true
                        }

                        if( event.key === Qt.Key_Minus ) {
                            mainObj.scaleDown();
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
                Item {
                    id: grpLst
                    objectName: "grpLst"

                    function groupListOpen() {
                        groupListView.forceActiveFocus()
                    }

                    Drawer {

                        id : groupSelectDrw
                        width: mainSceneImg.width
                        height: mainSceneImg.height*0.9
                        interactive: false //True - уже проверял. по дурацки работает из за списка

                        edge: Qt.TopEdge

                        onOpened: groupListView.forceActiveFocus()

                        ListView {

                            id: groupListView
                            anchors.fill: parent
                            model: mainObj.groupsList
                            keyNavigationEnabled: true

                            highlight: Rectangle {
                                width: 200; height: 20
                                color: "#FFFF88"
                                y: listView.currentItem.y;
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
    //                                    groupLstDlgt.ListView.view.currentIndex = index
                                    }
                                }
                            }
                        }
                    }
                }
            }


        }
    }
}
