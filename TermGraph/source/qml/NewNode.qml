import QtQuick 2.13
import QtQuick.Controls 2.13
import QtQuick.Layouts 1.13
import QtQuick.Window 2.13
import QtQuick.Dialogs 1.3

import "UIExtensions"

Page {
    id: root

    property string defaultUuidText: "startValue"

    StackView.onActivating: prepareForOpen()

    header: MainHeader {

        id: mainHeader
        title: "Добавить вершину"

        Component.onCompleted: {
            mainHeader.showCheckButton()
            mainHeader.showArrowIcon()
        }

        onMenuClick: exitFromThisPage()
        onCheckClick: root.addNode()
    }

    Shortcut { sequence: "Ctrl+Enter";  onActivated: root.addNode(); }
    Shortcut { sequence: "Ctrl+Return"; onActivated: root.addNode(); }

    background: Rectangle { color: appColors.base }

    function exitFromThisPage() { root.StackView.view.pop() }

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
        visible: termDefin.txtFocus || makeTag.focus || expandTagRight.focus

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
        visible: termDefin.txtFocus || makeTag.focus || expandTagRight.focus

        anchors { top: parent.top; right: parent.right; }

        onClicked: {
            var pos = termDefin.cursorPosition
            termDefin.text = tagProcessor.expandRight( pos, termDefin.text )
            termDefin.takeFocus()
            termDefin.cursorPosition = pos
        }
    }

    contentItem: ScrollView {
        id: scroll
        padding: 12
        ScrollBar.horizontal.policy: ScrollBar.AlwaysOff

        ColumnLayout {
            spacing: mainObj.getUiElementSize("colSpace") * Screen.pixelDensity
            width: scroll.width - scroll.leftPadding - scroll.rightPadding

            MyLabelPair {
                id: currentGroupFixedRow

                property string groupUuid: ""

                name: "Текущая группа: "
                text: groupsManager.getGroupName(groupUuid)
                Layout.fillWidth: true
            }

            MyTextField {
                property string prevText: ""

                id: termName
                labelText: "Название:"
                placeholderText: "[Термин]"

                onNewText: checkNewText(text)
                Layout.fillWidth: true

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

            MyTextArea {
                id: termDefin
                labelText: "-это"
                placeholderText: "[Определение. Ссылки формируются с помощью фигурных скобок {} ]"
                text: ""
                Layout.fillWidth: true
            }

            MyTextArea {
                id: termDescr
                labelText : "Описание"
                placeholderText: "[Общее словестное описание, пока никак не участвует в логике]"
                text: ""
                Layout.fillWidth: true
            }

            MyTextArea {
                id: termExampl
                labelText: "Примеры"
                text: ""
                Layout.fillWidth: true
            }

            MyTextField {
                enabled: false
                visible: false

                labelText: "Ссылка на Wiki статью"
                placeholderText: "http://"
                Layout.fillWidth: true
            }

            MyTextField {
                enabled: false
                visible: false

                labelText: "Ссылка на Wiki изображение"
                placeholderText: "http://"
                Layout.fillWidth: true
            }

        }
    }

    MessageDialog {
        id: emptyNodeNameDelDialog

        title: "Невозможно создать вершину"
        text:  "Невозможно создать пустой термин.\nЗаполните поле \"Название\""

        standardButtons: StandardButton.Ok
        icon: StandardIcon.Warning
    }
}
