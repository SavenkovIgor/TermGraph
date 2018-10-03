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

    Canvas {
        id: mainSceneImg
        anchors.fill: parent

        property color fillStyle: "#ae32a0" // purple

        Component.onCompleted: {
            requestPaint()
        }

        onPaint: {
            console.log("log!!!")
            var ctx = mainSceneImg.getContext('2d')

            JsPaint.paintNode(ctx, "textText", 100, 100, "#00ff00")
//            ctx.fillStyle = 'green'
//            ctx.fillRect(10, 10, 100, 100)
//            ctx.strokeRect(40, 40, 70, 70)

//            ctx.fillStyle = "#00FF00"
//            ctx.clearRect(0, 0, mainSceneImg.width, mainSceneImg.height)
//            ctx.roundedRect(10, 10, 40, 40, 5, 5)

        }
    }
}
