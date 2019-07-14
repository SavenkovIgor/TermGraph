import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import QtQuick.Window 2.12
import QtQuick.Dialogs 1.3

import "UIExtensions"

Page {
    id: newNodeEdit

    property string defaultUuidText: "startValue"

    property StackView mainStack

    function open() {
        mainStack.push(newNodeEdit)
        prepareForOpen()
    }

    header: MainHeader {

        id: mainHeader
        titleText: "Добавить вершину"

        Component.onCompleted: {
            mainHeader.showCheckButton()
            mainHeader.showArrowIcon()
        }

        onMenuClick: exitFromThisPage()
        onCheckClick: newNodeEdit.addNode()
    }

    Keys.onEscapePressed: {
        exitFromThisPage()
    }

    Keys.onPressed: {
        if (event.modifiers === Qt.ControlModifier) {
            if (event.key === Qt.Key_Return || event.key === Qt.Key_Enter) {
                newNodeEdit.addNode()
            }
        }
    }

    function exitFromThisPage() { mainStack.pop() }

    function prepareForOpen() {
        currentGroupFixedRow.groupUuid = sceneObj.getCurrGroupUuid()
        clearText(termName)
        clearText(termDefin)
        clearText(termDescr)
        clearText(termExampl)
        termName.takeFocus()
    }

    function clearText(textItem) { textItem.text = "" }

    function addNode() {
        if (termName.text === "") {
            emptyNodeNameDelDialog.visible = true
            return
        }

        var success = nodesManager.addNewNode(
                    termName.text,
                    "",
                    termDefin.text,
                    termDescr.text,
                    termExampl.text,
                    currentGroupFixedRow.groupUuid
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
                anchors.fill: parent
                anchors.margins: 15

                spacing: mainObj.getUiElementSize("colSpace")*Screen.pixelDensity

                MyLabelPair {
                    id: currentGroupFixedRow

                    property string groupUuid: ""

                    name: "Текущая группа: "
                    text: groupsManager.getGroupName(groupUuid)
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
                //                text: newNode ? sceneObj.currentNode.termForms : ""
                //            }

                MyTextArea {
                    id: termDefin
                    labelText: "-это"
                    placeholderText: "[Определение. Ссылки формируются с помощью фигурных скобок {} ]"
                    text: ""
                }

                MyTextArea {
                    id: termDescr
                    labelText : "Описание"
                    placeholderText: "[Общее словестное описание, пока никак не участвует в логике]"
                    text: ""
                }

                MyTextArea {
                    id: termExampl
                    labelText: "Примеры"
                    text: ""
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
