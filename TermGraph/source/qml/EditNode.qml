import QtQuick 2.13
import QtQuick.Controls 2.13
import QtQuick.Layouts 1.13
import QtQuick.Window 2.13
import QtQuick.Dialogs 1.3

import "UIExtensions"

Page {
    id: root

    property StackView mainStack
    property string nodeUuid: ""

    function open() {
        mainStack.push(root)
        updateInfo()
    }

    header: MainHeader {

        id: mainHeader
        titleText: "Изменить вершину"

        Component.onCompleted: {
            mainHeader.showCheckButton()
            mainHeader.showArrowIcon()
        }

        onMenuClick: exitFromThisPage()
        onCheckClick: root.applyNodeChange()
    }

    Shortcut { sequence: "Ctrl+Enter";  onActivated: root.applyNodeChange(); }
    Shortcut { sequence: "Ctrl+Return"; onActivated: root.applyNodeChange(); }

    background: Rectangle { color: appColors.base }

    function exitFromThisPage() { root.StackView.view.pop() }

    function updateInfo() {
        lastEditText.text = sceneObj.currentNode.lastEdit
        termName.text = sceneObj.currentNode.term
        termDefin.text = sceneObj.currentNode.definition
        termDescr.text = sceneObj.currentNode.description
        termExampl.text = sceneObj.currentNode.examples
        nodeGroup.selectElement(sceneObj.currentNode.groupUuid)
        termDefin.takeFocus()
    }

    function applyNodeChange() {
        if (termName.text == "") {
            emptyNodeNameDelDialog.visible = true
            return
        }

        var success = nodesManager.changeNode(
                    nodeUuidText.text,
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

        contentItem: Item {
            width: scroll.width

            Column {
                id: viewColumn

                anchors.margins: 12
                anchors.fill: parent

                spacing: mainObj.getUiElementSize("colSpace")*Screen.pixelDensity

                MyLabelPair {
                    id: nodeUuidText
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
                //                text: newNode ? sceneObj.currentNode.termForms : ""
                //            }

                MyTextArea {
                    id: termDefin
                    labelText: "-это"
                    placeholderText: "[Определение. Ссылки формируются с помощью фигурных скобок {} ]"
                }

                MyTextArea {
                    id: termDescr
                    labelText : "Описание"
                    placeholderText: "[Общее словестное описание, пока никак не участвует в логике]"
                }

                MyTextArea {
                    id: termExampl
                    labelText: "Примеры"
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

                MyLabelPair {
                    id: lastEditText
                    name: "Последняя правка:"
                }

                MessageDialog {
                    id: emptyNodeNameDelDialog

                    title: "Невозможно создать вершину"
                    text:  "Невозможно создать пустой термин.\nЗаполните поле \"Название\""

                    standardButtons: StandardButton.Ok
                    icon: StandardIcon.Warning
                }
            }
        }
    }
}
