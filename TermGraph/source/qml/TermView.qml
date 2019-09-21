import QtQuick 2.13
import QtQuick.Controls 2.13
import QtQuick.Layouts 1.13

import "UIExtensions"

Page {
    id: root

    header: MainHeader {
        id: mainHeader
        title: "Термин"

        Component.onCompleted: mainHeader.showArrowIcon()

        onMenuClick: root.StackView.view.pop()
    }

    contentItem: ScrollView {
        clip: true
        ScrollBar.horizontal.policy: ScrollBar.AlwaysOff

        ColumnLayout {
            spacing: 14
            width: root.width

            MyLabelPair {
                name: "Dbg:"
                text: sceneObj.getCurrNodeDebugInfo()
                visible: text !== ""
            }

            MyLabelPair {
                name: "Название:"
                text: sceneObj.currentNode.term
                visible: text !== ""
            }

            MyLabelPair {
                name: "Определение:"
                text: tagProcessor.decorateTags(sceneObj.getCurrNodeNameAndDefinition())
                visible: text !== ""
            }

            MyLabelPair {
                name: "Иерархия определений:"
                text: sceneObj.getCurrNodeHierarchyDefinition()
                visible: text !== ""
            }

            MyLabelPair {
                name: "Описание:"
                text: sceneObj.currentNode.description
                visible: text !== ""
            }

            MyLabelPair {
                name: "Пример:"
                text: sceneObj.currentNode.examples
                visible: text !== ""
            }
        }
    }

    background: Rectangle { color: appColors.base }
}
