import QtQuick 2.5
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import QtQuick.Window 2.3
import QtQuick.Dialogs 1.2

import "UIExtensions"

Page {
    id: root

    property StackView mainStack
    property string nodeUuid: ""

    function open() { mainStack.push(root) }

    header: MainHeader {

        id: mainHeader
        titleText: "Изменить вершину"

        Component.onCompleted: {
            mainHeader.showCheckButton()
            mainHeader.showArrowIcon()
        }

        onMenuClick: exitFromThisPage()
        onCheckClick: root.updateNode()
    }

    Keys.onEscapePressed: { exitFromThisPage() }

    Keys.onPressed: {
        if (event.modifiers === Qt.ControlModifier) {
            if (event.key === Qt.Key_Return || event.key === Qt.Key_Enter) {
                root.updateNode()
            }
        }
    }

    function exitFromThisPage() { mainStack.pop() }

    onNodeUuidChanged: {
        termName.text = sceneObj.getCurrNodeName()
        nodeGroup.selectElement(sceneObj.getCurrNodeGroupUuid())
        termDefin.takeFocus()
    }

    function updateNode() {
        if (termName.text == "") {
            emptyNodeNameDelDialog.visible = true
            return
        }

        var success = nodesManager.changeNode(
                    changeNodeRow.text,
                    termName.text,
                    "",
                    termDefin.text,
                    termDescr.text,
                    termExampl.text,
                    nodeGroup.currentText
                    )

        if (success) {
            exitFromThisPage()
        }
    }

    MyRoundButton {
        id: makeTag
        iconName: "code"
        visible: termDefin.txtFocus

        anchors { top: parent.top; right: expandTagRight.left; }

        onClicked: {
            var pos = termDefin.cursorPosition
            termDefin.text = tagProcessor.addTagInPosition( pos, termDefin.text )
            termDefin.takeFocus()
            termDefin.cursorPosition = pos;
        }
    }

    MyRoundButton {
        id: expandTagRight
        iconName: "extendRight"
        visible: termDefin.txtFocus

        anchors { top: parent.top; right: parent.right; }

        onClicked: {
            var pos = termDefin.cursorPosition
            termDefin.text = tagProcessor.expandRight( pos, termDefin.text )
            termDefin.takeFocus()
            termDefin.cursorPosition = pos
        }
    }

    ScrollView {
        id: scroll

        anchors.fill: parent
        ScrollBar.horizontal.policy: ScrollBar.AlwaysOff

        background: Rectangle { color: appColors.base }

        Item {
            width: scroll.width

            Column {
                id: viewColumn

                anchors.margins: 12
                anchors.fill: parent

                spacing: mainObj.getUiElementSize("colSpace")*Screen.pixelDensity

                MyLabelPair {
                    id: changeNodeRow
                    name: "Изменить вершину с uuid: "
                    text: root.nodeUuid
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
                    text: { [root.nodeUuid]; return sceneObj.getCurrNodeDefinition(); }
                }

                MyTextArea {
                    id: termDescr
                    labelText : "Описание"
                    placeholderText: "[Общее словестное описание, пока никак не участвует в логике]"
                    text: { [root.nodeUuid]; return sceneObj.getCurrNodeDescription() }
                }

                MyTextArea {
                    id: termExampl
                    labelText: "Примеры"
                    text: { [root.nodeUuid]; return sceneObj.getCurrNodeExamples() }
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

                    Label {
                        id: grpLabel
                        text : "Группа"
                        font.pixelSize: mainObj.getUiElementSize("inputText")*Screen.pixelDensity
                    }

                    MyComboBox {
                        id: nodeGroup
                        model: groupsManager.allUuidSorted

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
}
