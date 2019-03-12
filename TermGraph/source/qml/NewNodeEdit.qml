import QtQuick 2.5
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import QtQuick.Window 2.3
import QtQuick.Dialogs 1.2

import "UIExtensions"

Page {
    id: newNodeEdit

    property string defaultUuidText: "startValue"

    property StackView mainStack
    property string nodeUuid: defaultUuidText
    property bool newNode: nodeUuid === ""

    function open() { mainStack.push(newNodeEdit) }

    header: MainHeader {

        id: mainHeader
        titleText: newNode ? "Добавить вершину" : "Изменить вершину"

        Component.onCompleted: {
            mainHeader.showCheckButton()
            mainHeader.showArrowIcon()
        }

        onMenuClick: exitFromThisPage()
        onCheckClick: newNodeEdit.addOrChangeNode()
    }

    Keys.onEscapePressed: {
        exitFromThisPage()
    }

    Keys.onPressed: {
        if (event.modifiers === Qt.ControlModifier) {
            if (event.key === Qt.Key_Return || event.key === Qt.Key_Enter) {
                newNodeEdit.addOrChangeNode()
            }
        }
    }

    function exitFromThisPage() {
        mainStack.pop()
        nodeUuid = defaultUuidText
    }

    onNodeUuidChanged: {
        var newNode = nodeUuid === ""
        if (newNode) {
            termName.text    = ""
            currentGroupFixedRow.groupUuid = sceneObj.getCurrGroupUuid()
            termName.takeFocus()
        } else {
            termName.text    = sceneObj.getCurrNodeName()
            nodeGroup.selectElement(sceneObj.getCurrNodeGroupUuid())
            termDefin.takeFocus()
        }
    }

    function addOrChangeNode() {
        if (termName.text == "") {
            emptyNodeNameDelDialog.visible = true
            return
        }

        var success = true;
        var newNode = changeNodeRow.text === ""  // bool

        if (newNode) {
            success = nodesManager.addNewNode(
                        termName.text,
                        "",
                        termDefin.text,
                        termDescr.text,
                        termExampl.text,
                        currentGroupFixedRow.groupUuid
                        )
        } else {
            success = nodesManager.changeNode(
                        changeNodeRow.text,
                        termName.text,
                        "",
                        termDefin.text,
                        termDescr.text,
                        termExampl.text,
                        nodeGroup.currentText
                        )
        }

        if (success) {
            exitFromThisPage()
        }
    }


    MyRoundButton {
        id: makeTag

        anchors.right: expandTagRight.left
        anchors.top: parent.top

        visible: termDefin.txtFocus

        onClicked: {
            var pos = termDefin.cursorPosition
            termDefin.text = tagProcessor.addTagInPosition( pos, termDefin.text )
            termDefin.takeFocus()
            termDefin.cursorPosition = pos;
        }
        Component.onCompleted: loadIcon("qrc:/icons/code")
    }

    MyRoundButton {
        id: expandTagRight

        anchors.right: parent.right
        anchors.top: parent.top

        visible: termDefin.txtFocus

        onClicked: {
            var pos = termDefin.cursorPosition
            termDefin.text = tagProcessor.expandRight( pos, termDefin.text )
            termDefin.takeFocus()
            termDefin.cursorPosition = pos
        }
        Component.onCompleted: loadIcon("qrc:/icons/extendRight")
    }

    ScrollView {
        anchors.fill: parent
        ScrollBar.horizontal.policy: ScrollBar.AlwaysOff

        background: Rectangle {
            color: "#332f30"
        }

        Column {
            id: viewColumn

            anchors.margins: 12
            width: newNodeEdit.width

            spacing: mainObj.getUiElementSize("colSpace")*Screen.pixelDensity

            MyLabelPair {
                id: changeNodeRow
                name: "Изменить вершину с uuid: "
                text: newNodeEdit.nodeUuid
                visible: text !== ""
            }

            MyLabelPair {
                id: currentGroupFixedRow
                name: "Текущая группа: "

                property string groupUuid: ""
                onGroupUuidChanged: currentGroupFixedRow.text = groupsManager.getGroupName(groupUuid)

                visible: newNode
            }

            MyTextField {
                property string prevText: ""

                id: termName
                labelText: "Название:"
                placeholderText: "[Термин]"

                onNewText: checkNewText(text)

                function checkNewText(text) {
                    var differ = prevText.length - text.length
                    if (differ >= 2 || differ <= -2) {
                        if (textProcessor.isTermWithDefinition(text)) {
                            termName.text = textProcessor.getTerm(text)
                            termDefin.text = textProcessor.getDefinition(text)
                        }
                    }
                    prevText = text
                }
            }

//            MyTextField {
//                id : termForms
//                labelText : "Грамматические формы:"
//                placeholderText: "[Альтернативные окончания термина, как слОва]"
//                text: newNode ? sceneObj.getCurrNodeForms() : ""
//            }

            MyTextArea {
                id: termDefin
                labelText: "-это"
                placeholderText: "[Определение. Ссылки формируются с помощью фигурных скобок {} ]"
                text: newNode ? sceneObj.getCurrNodeDefinition() : ""
            }

            MyTextArea {
                id: termDescr
                labelText : "Описание"
                placeholderText: "[Общее словестное описание, пока никак не участвует в логике]"
                text: newNode ? sceneObj.getCurrNodeDescription() : ""
            }

            MyTextArea {
                id: termExampl
                labelText: "Примеры"
                text: newNode ? sceneObj.getCurrNodeExamples() : ""
            }

            MyTextField {
                enabled: false
                visible: false

                labelText: "Ссылка на Wiki статью"
                placeholderText: "http://"
            }

            MyTextField {
                enabled: false
                visible: false

                labelText: "Ссылка на Wiki изображение"
                placeholderText: "http://"
            }

            // Здесь есть проблема с anchors
            RowLayout {
                id: currentGroupEditableRow

                width: parent.width
                visible: !newNode

                Label {
                    id: grpLabel
                    text : "Группа"
                    font.pixelSize: mainObj.getUiElementSize("inputText")*Screen.pixelDensity
                }

                Connections {
                    target: groupsManager
                    onGroupsListChanged: nodeGroup.refreshModel()
                }

                Connections {
                    target: nodesManager
                    onNodeChanged: nodeGroup.refreshModel()
                }

                MyComboBox {
                    id: nodeGroup
                    model: groupsManager.getAllUuidStringsSortedByLastEdit()

                    function refreshModel() {
                        model = groupsManager.getAllUuidStringsSortedByLastEdit()
                    }

                    function selectElement(name) {
                        var index = find(name)
                        if (index !== -1) {
                            currentIndex = index
                        }
                    }
                    Layout.fillWidth: true
                }
            }

            MessageDialog {
                id: emptyNodeNameDelDialog

                title: "Невозможно создать вершину"
                text:  "Невозможно создать пустой термин.\nЗаполните поле \"Название\""

                standardButtons: StandardButton.Ok
                icon: StandardIcon.Warning
            }

            //            Item{
            //                Layout.fillHeight: true
            //            }
        }
    }
}
