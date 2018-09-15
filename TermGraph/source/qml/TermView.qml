import QtQuick 2.7
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

import "UIExtensions"

Page {
    id: termView

    property StackView mainStack

    header: MainHeader {
        id: mainHeader
        titleText: "Термин"

        Component.onCompleted: mainHeader.showArrowIcon()

        onMenuClick: mainStack.pop()
    }

    function loadSelectedNode() {

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

    ScrollView {
        anchors.fill: parent

        ColumnLayout {
            spacing: 14
//            padding: 10
            width: termView.width

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
}
