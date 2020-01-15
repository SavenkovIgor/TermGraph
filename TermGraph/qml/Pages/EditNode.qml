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

import "../Atoms" as A
import "../Molecules" as M
import "../Js/IconPath.js" as IconPath
import "../Js/Colors.js" as Colors
import "../Js/Fonts.js" as Fonts

M.Page {
    id: root

    title: "Изменить термин"

    StackView.onActivating: updateInfo()

    header: M.DefaultHeader {
        title: root.title
        page: root

        A.ToolButton {
            icon.source: IconPath.trash
            onClicked: nodeDelDialog.visible = true
        }

        A.ToolButton {
            icon.source: IconPath.check
            onClicked: root.applyNodeChange()
        }
    }

    MessageDialog {
        id: nodeDelDialog

        title: "Удаление термина"
        text:  "Удалить выделенный термин?"

        standardButtons: StandardButton.Yes | StandardButton.No
        icon: StandardIcon.Question

        onYes: {
            root.StackView.view.pop();
            scene.deleteSelectedTerm();
        }
    }

    Shortcut { sequence: "Ctrl+Enter";  onActivated: root.applyNodeChange(); }
    Shortcut { sequence: "Ctrl+Return"; onActivated: root.applyNodeChange(); }

    function updateInfo() {
        nodeUuidText.text = scene.currentNode.uuid;
        lastEditText.text = scene.currentNode.lastEdit;
        termName.text = scene.currentNode.term;
        termDefin.text = scene.currentNode.definition;
        termDescr.text = scene.currentNode.description;
        termExampl.text = scene.currentNode.examples;
        nodeGroup.selectElement(scene.currentNode.groupUuid);
        termDefin.takeFocus();
    }

    function applyNodeChange() {
        if (termName.text == "") {
            emptyNodeNameDelDialog.visible = true;
            return;
        }

        let success = nodesManager.changeNode(
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

    property bool someOfToolButtonsHaveFocus: addTag.focus || expandTagRight.focus || removeTag.focus

    A.RoundButton {
        id: addTag
        icon.source: IconPath.code
        visible: termDefin.addTagButtonVisible || someOfToolButtonsHaveFocus

        anchors { top: parent.top; right: expandTagRight.left; margins: width / 2; }

        onClicked: termDefin.addTag()
    }

    A.RoundButton {
        id: expandTagRight
        icon.source: IconPath.chevronRight
        visible: termDefin.extendTagButtonVisible || someOfToolButtonsHaveFocus

        anchors { top: parent.top; right: parent.right; margins: width / 2; }

        onClicked: termDefin.expandTagRight()
    }

    A.RoundButton {
        id: removeTag
        icon.source: IconPath.code
        icon.color: "crimson"
        visible: termDefin.removeTagButtonVisible || someOfToolButtonsHaveFocus

        anchors { top: parent.top; right: expandTagRight.left; margins: width / 2; }

        onClicked: termDefin.removeTag()
    }

    contentItem: ScrollView {
        id: scroll
        ScrollBar.horizontal.policy: ScrollBar.AlwaysOff

        contentWidth: root.implicitContentWidth

        ColumnLayout {
            spacing: mainObj.getUiElementSize("colSpace")
            width: scroll.width

            M.LabelPair {
                id: nodeUuidText
                name: "Uuid термина:"
                textObj.wrapMode: Text.NoWrap
                textObj.minimumPixelSize: Fonts.text.pixelSize / 2
                textObj.fontSizeMode: Text.HorizontalFit
                Layout.fillWidth: true
            }

            M.TextField {
                id: termName
                Layout.fillWidth: true
                labelText: "Термин:"
            }

            M.TagEditorArea {
                id: termDefin
                Layout.fillWidth: true
                labelText: "Определение:"
                placeholderText: "- это"
            }

            M.TextArea {
                id: termDescr
                Layout.fillWidth: true
                labelText : "Описание:"
            }

            M.TextArea {
                id: termExampl
                Layout.fillWidth: true
                labelText: "Примеры:"
            }

            M.TextField {
                visible: false
                Layout.fillWidth: true

                labelText: "Ссылка на Wiki статью:"
                placeholderText: "http://"
            }

            M.TextField {
                visible: false
                Layout.fillWidth: true

                labelText: "Ссылка на Wiki изображение:"
                placeholderText: "http://"
            }

            // Здесь есть проблема с anchors
            RowLayout {
                id: currentGroupEditableRow

                Layout.fillWidth: true
                spacing: root.width * 0.025

                Label {
                    id: grpLabel
                    text : "Группа"
                    font: Fonts.inputText
                    color: Colors.accent
                }

                A.ComboBox {
                    id: nodeGroup
                    Layout.fillWidth: true

                    model: groupsManager.allUuidSorted

                    function selectElement(name) {
                        let index = find(name)
                        if (index !== -1) {
                            currentIndex = index
                        }
                    }
                }
            }

            M.LabelPair {
                id: lastEditText
                Layout.fillWidth: true
                name: "Последняя правка:"
            }
        }
    }

    MessageDialog {
        id: emptyNodeNameDelDialog

        title: "Ошибка"
        text:  "Заполните поле \"Термин\""

        standardButtons: StandardButton.Ok
        icon: StandardIcon.Warning
    }
}
