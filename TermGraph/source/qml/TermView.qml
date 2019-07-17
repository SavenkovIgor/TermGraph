import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

import "UIExtensions"

Page {
    id: root

    property StackView mainStack
    function open() {
        mainStack.push(root)
        loadSelectedNode()
    }

    header: MainHeader {
        id: mainHeader
        titleText: "Термин"

        Component.onCompleted: mainHeader.showArrowIcon()

        onMenuClick: root.StackView.view.pop()
    }

    Rectangle {  // Background
        anchors.fill: parent
        color: appColors.base
    }

    function loadSelectedNode() {
        ptDbgInfo.text           = sceneObj.getCurrNodeDebugInfo()
        ptName.text              = sceneObj.currentNode.term
        ptDefinition.text        = tagProcessor.decorateTags(sceneObj.getCurrNodeNameAndDefinition())
        hierarchyDefinition.text = sceneObj.getCurrNodeHierarchyDefinition()
        ptDescription.text       = sceneObj.currentNode.description
        ptExamples.text          = sceneObj.currentNode.examples
    }

    ScrollView {
        anchors.fill: parent
        clip: true
        ScrollBar.horizontal.policy: ScrollBar.AlwaysOff

        ColumnLayout {
            spacing: 14
            width: root.width

            MyLabelPair {
                id: ptDbgInfo
                name: "Dbg:"
                visible: false
                onTextChanged: visible = text !== ""
            }

            MyLabelPair {
                id: ptName
                name: "Название:"
                visible: false
                onTextChanged: visible = text !== ""
            }

            MyLabelPair {
                id: ptDefinition
                name: "Определение:"
                visible: false
                onTextChanged: visible = text !== ""
            }

            MyLabelPair {
                id: hierarchyDefinition
                name: "Иерархия определений:"
                visible: false
                onTextChanged: visible = text !== ""
            }

            MyLabelPair {
                id: ptDescription
                name: "Описание:"
                visible: false
                onTextChanged: visible = text !== ""
            }

            MyLabelPair {
                id: ptExamples
                name: "Пример:"
                visible: false
                onTextChanged: visible = text !== ""
            }
        }
    }
}
