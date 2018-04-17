import QtQuick 2.2
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.2
import QtQuick.Window 2.3
import QtQuick.Dialogs 1.2
//import QtQuick.Controls.Material 2.2

import "UIExtensions"

ApplicationWindow {
    id: window
    visible: true

    width:  500
    height: 500

//    MainScheme {//NOTE: possible problems
//        id: mainSc
//        visible: false
//    }

//    Component.onCompleted: {
//        showMaximized();
//    }

    header: ToolBar {
        id: header
//        Material.foreground: "white"
        RowLayout {
            spacing: 20
            anchors.fill: parent

            MyRoundButton {
                id: mainMenuBtn
                Component.onCompleted: {
                    loadIcon( "qrc:/icons/menu")
                    hideBack()
                }

                onClicked: {
                    console.log("clicked")
                    if ( stackView.depth > 1 ) {
                        stackView.popItem()
                        listView.currentIndex = 0
                    } else {
                        drawer.open()
                    }
                }
            }

            Label {
                id: titleLabel
                //                text: drawer.listView.currentItem ? drawer.listView.currentItem.text : "TermGraph"
                text: "TermGraph"
                font.pixelSize: mainObj.getUiElementSize("appHeader")*Screen.pixelDensity
                elide: Label.ElideRight
                horizontalAlignment: Qt.AlignHCenter
                verticalAlignment: Qt.AlignVCenter
                Layout.fillWidth: true
            }


            MyRoundButton {
                rightPadding: 20
                Component.onCompleted: {
                    loadIcon( "qrc:/icons/pencil")
                }
                checkable: true
                onClicked: {
                    mainViewObj.setDragMove( !checked )
                    sceneObj.setEditEdgeMode( checked )
                }
            }
        }
    }

    Drawer {
        id : drawer
        width: Math.min(window.width, window.height) / 3 * 2
        height: window.height
        interactive: stackView.depth === 1

        onOpened: { listView.forceActiveFocus() }

        function isOpen() { return position == 1.0 }
        function isClose(){ return position == 0.0 }

        dragMargin: Qt.styleHints.startDragDistance*2

        FileDialog {
            id: fileDialog
            title: "Пожалуйста выберите файл базы данных"

            onAccepted: {
                console.log("You chose: " + fileDialog.fileUrls)
            }
        }

        MyRoundButton {
            id: openFolder

            onClicked: fileDialog.open()

            anchors.top: parent.top
            anchors.right: parent.right

            Component.onCompleted: loadIcon( "qrc:/icons/folder" )
        }

        ListView {
            id: listView

            currentIndex: 0
            anchors {
                top: openFolder.bottom
                left: parent.left
                right: parent.right
                bottom: parent.bottom
            }

            function openNewNode() {
                stackView.pushItem(winModel.get(1).source)
                drawer.close()
            }

            function openMainWindow() {
                while( stackView.depth > 1) {
                    stackView.popItem()
                }
                drawer.close()
            }

            function openGroups() {
                stackView.pushItem(winModel.get(2).source)
            }

            Keys.onPressed: {
                if( event.modifiers | Qt.ControlModifier && event.key === Qt.Key_Left)
                    drawer.close()
            }

            delegate: ItemDelegate {
                id: listViewDgl
                width: parent.width
                height: Math.floor( font.pixelSize * 2.7 )
                leftPadding: 50
                text: model.title
                font.weight: Font.Medium
                font.pixelSize: mainObj.getUiElementSize("capitalText")*Screen.pixelDensity
                highlighted: ListView.isCurrentItem
                onClicked: {

                    stackView.pushItem(model.source)
                    drawer.close()

                }

            }

            model: ListModel {
                id: winModel
                ListElement { title: "Схема";               source: "qrc:/qml/MainScheme.qml" }
                ListElement { title: "Добавить вершину";    source: "qrc:/qml/NewNodeEdit.qml" }
                ListElement { title: "Группы";              source: "qrc:/qml/TermGroupsList.qml" }
                ListElement { title: "Настройки";           source: "qrc:/qml/Settings.qml" }
                //                ListElement { title: "Изучение";            source: "" }
                //                ListElement { title: "Базы данных";         source: "" }
                //                ListElement { title: "Настройки";           source: "" }
                //                ListElement { title: "Справка";             source: "" }
            }

            ScrollIndicator.vertical: ScrollIndicator { }
        }
    }

    StackView {
        id: stackView

        anchors.top: header.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom

        initialItem: "qrc:/qml/MainScheme.qml"

        property string lastItem: "qrc:/qml/MainScheme.qml"

        function pushItem( itmStr ){
            console.log( lastItem + " " + itmStr )
            if( lastItem !== itmStr ){
                lastItem = itmStr
                push(itmStr)
            }
        }

        function popItem(){
            lastItem = ""
            pop()
        }

        onCurrentItemChanged:  {
            if( depth > 1 )
                mainMenuBtn.loadIcon( "qrc:/icons/arrow-thick-left" )
            else
                mainMenuBtn.loadIcon( "qrc:/icons/menu" )
            //            console.log(currentItem)
            //            if( currentItem == newNodeEdit)
            //                console.log("newNodeEdit!!!")
            //            console.log( "itemChange!" + currentItem.objectName )
            if( currentItem.objectName == "mainScheme") {
                titleLabel.text = "TermGraph"
                currentItem.toTop()
                currentItem.setOffs( header.height + 11 )
                showGlpLst.visible = true
            } else if( currentItem.objectName == "newNodeEdit" ) {
                titleLabel.text = "Правка вершин"
                currentItem.prepare();
            } else if( currentItem.objectName == "grpLst" ) {
                titleLabel.text = "Список групп"
                currentItem.groupListOpen()
            }
        }

        Keys.onPressed: {
            if( event.modifiers === Qt.ControlModifier) {
                if( event.key === Qt.Key_Right )
                    if( drawer.isClose() )
                        drawer.open()

                if( event.key === Qt.Key_Left )
                    if( currentItem.objectName == "mainScheme" ) {
                        stackView.currentItem.openGroupList()
                        stackView.forceActiveFocus()
                    }
            }

            if( event.key === Qt.Key_Back ) {
                if( stackView.depth > 1) {
                    stackView.popItem()
                    event.accepted = true
                }
            }

            console.log("presses in stack view")
        }

    }
}
