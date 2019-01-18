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
            ptDefinition.text = sceneObj.getCurrNodeNameAndDefinition()
            ptDefinition.visible = true
        } else {
            ptDefinition.visible = false
        }

        if (sceneObj.getCurrNodeHierarchyDefinition() !== "") {
            hierarchyDefinition.text = sceneObj.getCurrNodeHierarchyDefinition()
            hierarchyDefinition.visible = true
        } else {
            hierarchyDefinition.visible = false
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
        clip: true
        ScrollBar.horizontal.policy: ScrollBar.AlwaysOff

        ColumnLayout {
            spacing: 14
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
                id: hierarchyDefinition
                name: "Иерархия определений:"
            }

            MyLabelPair{
                id: ptDescription
                name: "Описание:"
            }

            MyLabelPair{
                id: ptExamples
                name: "Пример:"
            }
        }
    }
}
