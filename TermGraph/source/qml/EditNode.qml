/*
 *  TermGraph, build graph of knowledge.
 *  Copyright © 2016-2019. Savenkov Igor. All rights reserved
 *  Contacts: dev.savenkovigor@protonmail.com
 *
 *  This file is part of TermGraph.
 *
 *  TermGraph is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  TermGraph is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with TermGraph. If not, see <https://www.gnu.org/licenses/>.
 */

import QtQuick 2.13
import QtQuick.Controls 2.13
import QtQuick.Layouts 1.13
import QtQuick.Dialogs 1.3

import "UIExtensions"
import "Atoms" as A
import "Molecules" as M
import "Js/IconPath.js" as IconPath
import "Js/Colors.js" as Colors

M.Page {
    id: root

    title: "Изменить вершину"

    StackView.onActivating: updateInfo()

    header: M.DefaultHeader {
        title: root.title
        page: root

        A.ToolButton {
            id: checkButton
            icon.source: IconPath.check
            onClicked: root.applyNodeChange()
        }
    }

    Shortcut { sequence: "Ctrl+Enter";  onActivated: root.applyNodeChange(); }
    Shortcut { sequence: "Ctrl+Return"; onActivated: root.applyNodeChange(); }

    function updateInfo() {
        nodeUuidText.text = sceneObj.currentNode.uuid
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
            emptyNodeNameDelDialog.visible = true;
            return;
        }

        var success = nodesManager.changeNode(
                    nodeUuidText.text,
                    termName.text,
                    termDefin.text,
                    termDescr.text,
                    termExampl.text,
                    nodeGroup.currentText
                    );

        if (success) {
            root.StackView.view.pop();
        }
    }

    MyRoundButton {
        id: makeTag
        icon.source: IconPath.code
        visible: termDefin.txtFocus || makeTag.focus || expandTagRight.focus

        anchors { top: parent.top; right: expandTagRight.left; margins: width / 2; }

        onClicked: {
            var pos = termDefin.cursorPosition
            termDefin.text = tagProcessor.addTagInPosition( pos, termDefin.text )
            termDefin.takeFocus()
            termDefin.cursorPosition = pos;
        }
    }

    MyRoundButton {
        id: expandTagRight
        icon.source: IconPath.chevronRight
        visible: termDefin.txtFocus || makeTag.focus || expandTagRight.focus

        anchors { top: parent.top; right: parent.right; margins: width / 2; }

        onClicked: {
            var pos = termDefin.cursorPosition
            termDefin.text = tagProcessor.expandRight( pos, termDefin.text )
            termDefin.takeFocus()
            termDefin.cursorPosition = pos
        }
    }

    contentItem: ScrollView {
        id: scroll
        ScrollBar.horizontal.policy: ScrollBar.AlwaysOff

        ColumnLayout {
            spacing: mainObj.getUiElementSize("colSpace")
            width: scroll.width - scroll.leftPadding - scroll.rightPadding

            MyLabelPair {
                id: nodeUuidText
                name: "Изменить вершину с uuid: "
                Layout.fillWidth: true
            }

            MyTextField {
                property string prevText: ""

                id: termName
                labelText: "Название:"
                placeholderText: "[Термин]"

                Layout.fillWidth: true
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

            MyTextArea {
                id: termDefin
                Layout.fillWidth: true
                labelText: "-это"
                placeholderText: "[Определение. Ссылки формируются с помощью фигурных скобок {} ]"
            }

            MyTextArea {
                id: termDescr
                Layout.fillWidth: true
                labelText : "Описание"
                placeholderText: "[Общее словесное описание, пока никак не участвует в логике]"
            }

            MyTextArea {
                id: termExampl
                Layout.fillWidth: true
                labelText: "Примеры"
            }

            MyTextField {
                enabled: false
                visible: false
                Layout.fillWidth: true

                labelText: "Ссылка на Wiki статью"
                placeholderText: "http://"
            }

            MyTextField {
                enabled: false
                visible: false
                Layout.fillWidth: true

                labelText: "Ссылка на Wiki изображение"
                placeholderText: "http://"
            }

            // Здесь есть проблема с anchors
            RowLayout {
                id: currentGroupEditableRow

                Layout.fillWidth: true

                Label {
                    id: grpLabel
                    text : "Группа"
                    font.pixelSize: mainObj.getUiElementSize("inputText")
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
                Layout.fillWidth: true
                name: "Последняя правка:"
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
