import QtQuick 2.7
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

import "UIExtensions"

Page {
    id: termViewPage

    property StackView mainStack
    function open() {
        mainStack.push(termViewPage)
        loadSelectedNode()
    }

    header: MainHeader {
        id: mainHeader
        titleText: "Термин"

        Component.onCompleted: mainHeader.showArrowIcon()

        onMenuClick: mainStack.pop()
    }

    Rectangle {  // Background
        anchors.fill: parent
        color: appColors.base
    }

    function loadSelectedNode() {
        ptDbgInfo.text           = sceneObj.getCurrNodeDebugInfo()
        ptName.text              = sceneObj.getCurrNodeName()
        ptDefinition.text        = tagProcessor.decorateTags(sceneObj.getCurrNodeNameAndDefinition())
        hierarchyDefinition.text = sceneObj.getCurrNodeHierarchyDefinition()
        ptDescription.text       = sceneObj.getCurrNodeDescription()
        ptExamples.text          = sceneObj.getCurrNodeExamples()
    }

    ScrollView {
        anchors.fill: parent
        clip: true
        ScrollBar.horizontal.policy: ScrollBar.AlwaysOff

        ColumnLayout {
            spacing: 14
            width: termViewPage.width

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
